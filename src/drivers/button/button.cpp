#include "logger.h"
#include "device.h"
#include "display.h"
#include "storage.h"
#include "global.h"
#include "TouchButton.h"

#define TOUCH_PIN       T9
#define TOUCH_THRESHOLD 90
TouchButton tbtn(TOUCH_PIN, TOUCH_THRESHOLD); 

void force_cfg_cb(void){
  nvs_config_set_u8(NMTV_SETTINGS_NAMESPACE, JSON_SPIFFS_KEY_NEED_CFG, true);
  delay(500);
  ESP.restart();
}




void button_thread_entry(void *args){
  char *name = (char*)malloc(20);
  strcpy(name, (char*)args);
  LOG_I("%s thread started on core %d...", name, xPortGetCoreID());
  // free(name);

  tbtn.attachClick([](){ 
    LOG_I("single click detected");
    ui_switch_next_page_cb();
  });
  tbtn.attachDoubleClick([](){ LOG_W("double click detected");});
  tbtn.attachLongPress([](){ 
    LOG_I("long press detected");
    force_cfg_cb();
  });


  while (true){
    tbtn.tick();
    delay(30);
  }
}