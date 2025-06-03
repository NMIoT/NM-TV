#include "OneButton.h"
#include "logger.h"
#include "device.h"
#include "display.h"
#include "storage.h"
#include "global.h"

#if defined(HAS_TWO_HW_BTN)
  OneButton boot_btn(BOOT_BTN_PIN, true);
  OneButton user_btn(USER_BTN_PIN, true);
#elif defined(HAS_ONE_HW_BTN)
  OneButton boot_btn(BOOT_BTN_PIN, true);
#endif


void force_cfg_cb(void){
  nvs_config_set_u8(MINER_SETTINGS_NAMESPACE, JSON_SPIFFS_KEY_NEED_CFG, true);
  delay(500);
  ESP.restart();
}

#if defined(HAS_ONE_TOUCH_BTN)
#define TOUCH_PIN T9
#define TOUCH_THRESHOLD 90

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
    longPressCnt = (millis() - lastTouchTime > 1000*5) ? 0 : longPressCnt; // 5s后清零
    if(touch_cnt - lastTouchcnt>= SINGLE_CLICK_TOUCH_MIN_CNT) {
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
#endif


void button_thread_entry(void *args){
  char *name = (char*)malloc(20);
  strcpy(name, (char*)args);
  LOG_I("%s thread started on core %d...", name, xPortGetCoreID());
  // free(name);

// link the button functions.  
#if defined(HAS_ONE_HW_BTN)
    boot_btn.attachDuringLongPress(force_cfg_cb);
    #if defined(TFT_DISPLAY) || defined(AMOLED_DISPLAY)
      #if (TFT_BL != -1)
      boot_btn.attachClick(tft_bl_active);
      #endif
      boot_btn.attachDoubleClick(ui_switch_next_page_cb);
    #elif defined(OLED_DISPLAY)
      boot_btn.attachDoubleClick(ui_switch_next_page_cb);
    #endif
#elif defined(HAS_TWO_HW_BTN)
    #if (TFT_BL != -1) && defined(TFT_DISPLAY)
    boot_btn.attachClick(tft_bl_active);
    boot_btn.attachDoubleClick(tft_bl_active);
    #endif
    boot_btn.attachLongPressStart(restore_to_factory_setting_cb);

    #if (TFT_BL != -1) && defined(TFT_DISPLAY)
    user_btn.attachClick(tft_bl_active);
    #endif
    #if defined(TFT_DISPLAY)
    user_btn.attachDoubleClick(ui_switch_next_page_cb);
    #endif
    user_btn.attachDuringLongPress(force_cfg_cb);
#elif defined(HAS_ONE_TOUCH_BTN)
    touchAttachInterrupt(TOUCH_PIN, onTouch, TOUCH_THRESHOLD); // 设置触发阈值为 90
#endif
  while (true)
  {
#if defined(HAS_ONE_HW_BTN)
    boot_btn.tick();
#elif defined(HAS_TWO_HW_BTN)
    boot_btn.tick();
    user_btn.tick();
#elif defined(HAS_ONE_TOUCH_BTN)
  TouchEventType event = checkTouchEvent();
  switch (event) {
      case TOUCH_SINGLE:
          // 处理单击事件
          LOG_I("Single touch detected");
          ui_switch_next_page_cb();
          break;
      case TOUCH_LONG:
          // 处理长按事件
          LOG_I("Long touch detected");
          force_cfg_cb();
          break;
      case TOUCH_NONE:
          // 无事件，继续
          break;
  }
#endif
    delay(30);






    // 获取当前任务的堆栈水位线高
    // static uint8_t cnt =0;
    // if(cnt++ % 10 == 0){
    //   UBaseType_t highWaterMark = uxTaskGetStackHighWaterMark(NULL);
    //   LOG_I("%s Stack High Water Mark: %u", name, highWaterMark);
    // }
  }
}