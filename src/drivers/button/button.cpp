#include "logger.h"
#include "device.h"
#include "display.h"
#include "storage.h"
#include "global.h"

#define TOUCH_PIN       T9
#define TOUCH_THRESHOLD 90

void force_cfg_cb(void){
  nvs_config_set_u8(NMTV_SETTINGS_NAMESPACE, JSON_SPIFFS_KEY_NEED_CFG, true);
  delay(500);
  ESP.restart();
}

// 触摸事件类型定义
typedef enum {
    TOUCH_NONE,     // 无事件
    TOUCH_SINGLE,   // 单击
    TOUCH_DOUBLE,   // 双击
    TOUCH_LONG      // 长按
} TouchEventType;

uint32_t touch_cnt = 0;

// 中断服务程序（只检测下降沿）
void IRAM_ATTR onTouch() {
  touch_cnt++;
}

#define SINGLE_CLICK_TOUCH_MIN_CNT  20

// 识别触摸事件
TouchEventType checkTouchEvent() {
    TouchEventType event = TOUCH_NONE;
    static uint32_t lastTouchcnt = 0;
    static uint8_t longPressCnt = 0;
    static uint32_t lastTouchTime = 0;
    longPressCnt = (millis() - lastTouchTime > 1000*5) ? 0 : longPressCnt; 
    if(touch_cnt - lastTouchcnt >= SINGLE_CLICK_TOUCH_MIN_CNT) {
      lastTouchTime = millis();
      event = TOUCH_SINGLE;
      lastTouchcnt = touch_cnt;
      longPressCnt++;
      if(longPressCnt >= 8) {
        event = TOUCH_LONG;
        longPressCnt = 0;
      }
    }
    return event;
}



void button_thread_entry(void *args){
  char *name = (char*)malloc(20);
  strcpy(name, (char*)args);
  LOG_I("%s thread started on core %d...", name, xPortGetCoreID());
  // free(name);

  touchAttachInterrupt(TOUCH_PIN, onTouch, TOUCH_THRESHOLD); 

  while (true){
    TouchEventType event = checkTouchEvent();
    switch (event) {
        case TOUCH_SINGLE:
            LOG_I("Single touch detected");
            ui_switch_next_page_cb();
            break;
        case TOUCH_DOUBLE:
            LOG_I("Double touch detected");
            break;
        case TOUCH_LONG:
            LOG_I("Long touch detected");
            force_cfg_cb();
            break;
        case TOUCH_NONE:
            // LOG_W("No touch event detected");
            break;
    }
    delay(30);
  }
}