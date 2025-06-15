#include <esp_task_wdt.h>
#include "global.h"
#include "logger.h"
#include "display.h"
#include "button.h"
#include "storage.h"
#include "nmapi.h"
#include "github.h"
#include "esp32/clk.h"
#include "timezone.h"
#include "helper.h"

static String  taskName = "";
TaskHandle_t   task_btn = NULL, task_nmapi = NULL, task_monitor = NULL, task_ui = NULL, task_lvgl_tick = NULL, task_ui_refresh = NULL;
nm_sal_t       g_nm;

void setup() {
  enable_usb_uart();
  /********************************************************** INIT SERIAL *****************************************************************/
  delay(1000);
  logo_print();
  /************************************************************* INIT GLOBAL **************************************************************/
  if(!load_g_nm()){
    while (true){
      LOG_E("Failed to load global data structure.");
      delay(1000);
    }
  }
  LOG_I("CPU %d core(s) detected, frequency %d MHz", (int)SOC_CPU_CORES_NUM, (int)esp_clk_cpu_freq()/1000000);
  /********************************************************* INIT THREAD WTDG *************************************************************/
  LOG_I("Initializing thread wtdg......");
  if(esp_task_wdt_init(MINER_WTDG_TIMEOUT, true) != ESP_OK){
    LOG_E("Thread wtdg initialization failed");
  }
  /**************************************************************** INIT BUTTON ***********************************************************/
  taskName = "(button)";
  xTaskCreatePinnedToCore(button_thread_entry, taskName.c_str(), 1024*3, (void*)taskName.c_str(), TASK_PRIORITY_BTN, &task_btn, BtnTaskCore);
  delay(50);
  /*********************************************************** INIT DISPLAY ***************************************************************/
  taskName = "(ui)";
  xTaskCreatePinnedToCore(display_thread, taskName.c_str(), 1024*5, (void*)taskName.c_str(), TASK_PRIORITY_UI, &task_ui, UiTaskCore);
  delay(50);
  /*********************************************************** FORCE CONFIG **************************************************************/
  if(g_nm.need_cfg){
    xSemaphoreGive(g_nm.connection.wifi.force_cfg_xsem);
    nvs_config_set_u8(NMTV_SETTINGS_NAMESPACE, JSON_SPIFFS_KEY_NEED_CFG, false);
    while(!wifi_config(true)){
      delay(1000);
    }
  }
  /************************************************************** INIT WIFI ****************************************************************/
  taskName = "(connection)";
  xTaskCreate(wifi_connect_thread_entry, taskName.c_str(), 1024*5, (void*)taskName.c_str(), TASK_PRIORITY_CONNECT, NULL);
  while (WL_CONNECTED != g_nm.connection.wifi.status_param.status){
    delay(1000);
  }
  delay(2000);
  /**************************************************************CHECK FIRMWARE RELEASE *****************************************************/
  ReleaseCheckerClass *releaseChecker = new ReleaseCheckerClass(); 
  g_nm.board.fw_latest_release = releaseChecker->get_latest_release();

  if(0 == compareVersions(g_nm.board.fw_version, g_nm.board.fw_latest_release)){
    LOG_I("Firmware is up to date: %s", g_nm.board.fw_latest_release.c_str());
  }
  else if(1 == compareVersions(g_nm.board.fw_version, g_nm.board.fw_latest_release)){
    LOG_W("Firmware seems under development: %s", g_nm.board.fw_version.c_str());
  }
  else if(-2 == compareVersions(g_nm.board.fw_version, g_nm.board.fw_latest_release)){
    LOG_W("Get release info failed, please check your network connection.");
  }
  else if(-1 == compareVersions(g_nm.board.fw_version, g_nm.board.fw_latest_release)){
    LOG_W("New version available: %s", g_nm.board.fw_latest_release.c_str());
  }
  delete releaseChecker;
  /******************************************************************TIMEZONE *************************************************************/
  // fetch timezone from ipapi
  TimezoneFetcher *tz = new TimezoneFetcher();
  if(!tz->fetch()){
      g_nm.location.tz_offest = 0; //default timezone
      g_nm.location.tz_updated = false;
      g_nm.location.coord.lat = 0;
      g_nm.location.coord.lon = 0;
      LOG_W("Timezone fetch failed, using default timezone: %.1f", g_nm.location.tz_offest); 
  }else{
      g_nm.location.tz_offest = tz->timezone.toFloat();
      g_nm.location.tz_updated = true;
      g_nm.location.coord.lat = tz->latitude;
      g_nm.location.coord.lon = tz->longitude;
      LOG_W("Timezone calibrate to : %.1f, Latitude: %.6f, Longitude: %.6f", g_nm.location.tz_offest, g_nm.location.coord.lat, g_nm.location.coord.lon);
  }

  //for test, set location to Chengdu, China
  g_nm.location.coord.lat = 30.6667;
  g_nm.location.coord.lon = 104.0667;


  delete tz;
  /************************************************************** CREATE MARKET THREAD ***************************************************/
  taskName = "(nmapi)";
  xTaskCreatePinnedToCore(nmapi_thread_entry, taskName.c_str(), 1024*8, (void*)taskName.c_str(), TASK_PRIORITY_API, &task_nmapi, ApiTaskCore);
  delay(50);
  /************************************************************** CREATE MONITOR THREAD ***************************************************/
  taskName = "(monitor)";
  xTaskCreatePinnedToCore(monitor_thread_entry, taskName.c_str(), 1024*3, (void*)taskName.c_str(), TASK_PRIORITY_MONITOR, &task_monitor, MonitorTaskCore);
  delay(50);
}

void loop() {
#if 0
  // 获取当前任务的堆栈水位线高
  static uint32_t start = millis();
  static UBaseType_t highWaterMark = uxTaskGetStackHighWaterMark(NULL);
  static char *taskName = pcTaskGetName(NULL);
  if(millis() - start > 1000*2){
      LOG_W("----------------------------------------------------------------");
      if(task_btn != NULL) {
          highWaterMark = uxTaskGetStackHighWaterMark(task_btn);
          taskName = pcTaskGetName(task_btn);
          LOG_I("%s Stack High Water Mark: %u", taskName, highWaterMark);
      }
      if(task_lvgl_tick != NULL) {
          highWaterMark = uxTaskGetStackHighWaterMark(task_lvgl_tick);
          taskName = pcTaskGetName(task_lvgl_tick);
          LOG_I("%s Stack High Water Mark: %u", taskName, highWaterMark);
      }
      if(task_ui_refresh != NULL) {
          highWaterMark = uxTaskGetStackHighWaterMark(task_ui_refresh);
          taskName = pcTaskGetName(task_ui_refresh);
          LOG_I("%s Stack High Water Mark: %u", taskName, highWaterMark);
      }
      if(task_nmapi != NULL) {
          highWaterMark = uxTaskGetStackHighWaterMark(task_nmapi);
          taskName = pcTaskGetName(task_nmapi);
          LOG_I("%s Stack High Water Mark: %u", taskName, highWaterMark);
      }
      if(task_monitor != NULL) {
          highWaterMark = uxTaskGetStackHighWaterMark(task_monitor);
          taskName = pcTaskGetName(task_monitor);
          LOG_I("%s Stack High Water Mark: %u", taskName, highWaterMark);
      }
      start = millis();
  }
#endif
  delay(1000);
}

