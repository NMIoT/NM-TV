#include "monitor.h"
#include "logger.h"
#include "connection.h"
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "display.h"
#include <cfloat>
#include <esp_system.h>
#include "helper.h"
#include "storage.h"
#include <nvs_flash.h>
#include <nvs.h>
#include "global.h"
#include "soc/rtc.h"

#define UDP_BOARDCAST_ADDR           IPAddress(255,255,255,255)
#define UDP_STATUS_BOARDCAST_PORT    (12345)
#define UDP_CONFIG_BOARDCAST_PORT    (12346)
#define UDP_CONFIG_LISTEN_PORT       (12347)
#define SWARM_OFFLINE_TIMEOUT        (3*60*1000)


static WiFiUDP  udpNtpClient;
static WiFiUDP *udp_swarm_client = NULL, *udp_config_up_client = NULL, *udp_config_down_client = NULL;
static const String   ntpServerUrl= "europe.pool.ntp.org";
static const uint32_t ntpInterval = 1000*60*60*24;//24h update interval
static NTPClient ntpClient(udpNtpClient, ntpServerUrl.c_str());
static float progress = 0.0;


void monitor_thread_entry(void *args){
    char *name = (char*)malloc(20);
    strcpy(name, (char*)args);
    LOG_I("%s thread started on core %d...", name, xPortGetCoreID());
    free(name);

    uint64_t duration = 0;
    uint64_t  last_hash_cal_time = micros();
    uint32_t m_cnt = 0;
    //ntp服务初始化
    ntpClient.begin();
    ntpClient.setTimeOffset(g_nm.timezone * 3600);
    ntpClient.setUpdateInterval(ntpInterval);

    while(true){
#if 0
        // 获取当前任务的堆栈水位线高
        static uint32_t start = millis();
        static UBaseType_t highWaterMark = uxTaskGetStackHighWaterMark(NULL);
        static char *taskName = pcTaskGetName(NULL);
        if(millis() - start > 1000*2){
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
            if(task_led != NULL) {
                highWaterMark = uxTaskGetStackHighWaterMark(task_led);
                taskName = pcTaskGetName(task_led);
                LOG_I("%s Stack High Water Mark: %u", taskName, highWaterMark);
            }
            if(task_market != NULL) {
                highWaterMark = uxTaskGetStackHighWaterMark(task_market);
                taskName = pcTaskGetName(task_market);
                LOG_I("%s Stack High Water Mark: %u", taskName, highWaterMark);
            }
            if(task_swarm != NULL) {
                highWaterMark = uxTaskGetStackHighWaterMark(task_swarm);
                taskName = pcTaskGetName(task_swarm);
                LOG_I("%s Stack High Water Mark: %u", taskName, highWaterMark);
            }
            if(task_stratum != NULL) {
                highWaterMark = uxTaskGetStackHighWaterMark(task_stratum);
                taskName = pcTaskGetName(task_stratum);
                LOG_I("%s Stack High Water Mark: %u", taskName, highWaterMark);
            }
            if(task_monitor != NULL) {
                highWaterMark = uxTaskGetStackHighWaterMark(task_monitor);
                taskName = pcTaskGetName(task_monitor);
                LOG_I("%s Stack High Water Mark: %u", taskName, highWaterMark);
            }
            if(task_nminer != NULL) {
                highWaterMark = uxTaskGetStackHighWaterMark(task_nminer);
                taskName = pcTaskGetName(task_nminer);
                LOG_I("%s Stack High Water Mark: %u", taskName, highWaterMark);
            }
            if(task_tminer != NULL) {
                highWaterMark = uxTaskGetStackHighWaterMark(task_tminer);
                taskName = pcTaskGetName(task_tminer);
                LOG_I("%s Stack High Water Mark: %u", taskName, highWaterMark);
            }
            start = millis();
        }
#endif
        //WiFi连接状态
        if(WiFi.status() != WL_CONNECTED){
            g_nm.connection.wifi.status_param.status = WL_DISCONNECTED;
            g_nm.connection.wifi.status_param.rssi   = 0;
        }else{
            g_nm.connection.wifi.status_param.status = WL_CONNECTED;
        }

        delay(100);
        if(m_cnt++ % 10 == 0){
            g_nm.screen.cnt++;
        }else continue;

        //从网络更新时间
        if(ntpClient.update()){
            struct timeval tv;
            tv.tv_sec = ntpClient.getEpochTime();
            tv.tv_usec = 0;
            settimeofday(&tv, NULL);
            // g_nm.minerstatus.utc = tv.tv_sec;
            // String time_local = convert_time_to_local(g_nm.minerstatus.utc);
            // LOG_W("ntp calibrate time %s", time_local.c_str());
        }
        else{
            // 获取当前时间戳
            time_t now;
            time(&now);
            // g_nm.minerstatus.utc = now;
        }

        //WiFi重连
        if(xSemaphoreTake(g_nm.connection.wifi.reconnect_xsem, 0) == pdTRUE){
            WiFi.begin(g_nm.connection.wifi.conn_param.ssid.c_str(), g_nm.connection.wifi.conn_param.pwd.c_str());
            delay(1000);
            continue;
        }
        
        //定时保存状态到NVS
        if(g_nm.save_stats){
            static uint32_t last_save_time = 0;
          
        }
    }
    LOG_W("%s thread exit", __FUNCTION__);
    delay(1000);
    vTaskDelete(NULL);
}