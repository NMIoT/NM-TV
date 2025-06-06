#ifndef GLOBAL_H
#define GLOBAL_H
#include <Arduino.h>
#include "connection.h"
#include "device.h"
#include "market.h"


#define CURRENT_VERSION     "v0.0.01"
#define MINER_WTDG_TIMEOUT  (1000*60*15) //15分钟看门狗
#define SAVE_INTERVAL       (60*60) //每60分钟保存一次状态到NVS
#define WIFI_RSSI_STRONG    (-60)
#define WIFI_RSSI_GOOD      (-70)


#define LvglTaskCore      1
#define UiRefreshTaskCore 1
#define UiTaskCore        1
#define BtnTaskCore       0
#define MarketTaskCore    0
#define MonitorTaskCore   0

enum{
    // lowest priority
    TASK_PRIORITY_BTN = 1, 
    TASK_PRIORITY_MARKET,
    TASK_PRIORITY_MONITOR, 
    TASK_PRIORITY_CONNECT, 
    TASK_PRIORITY_LED, 
    TASK_PRIORITY_CONFIG_MONITOR,
    TASK_PRIORITY_DISPLAY, 
    TASK_PRIORITY_UI_REFRESH,
    TASK_PRIORITY_LVGL_DRV, 
    //highest priority
};

typedef struct{
    String      fw_version;
    String      hw_version;
    String      fw_latest_release;
    String      model;//different model from different vendor
    uint8_t     devcie_code[32];
    float       tempurature;
}board_info_t;

typedef struct{
    wifi_info_t    wifi; 
    bool           client_connected;
}connect_info_t;

typedef struct{
    bool      orientation;
    uint16_t  refresh_interval;
    int       sleep_timeout;
    bool      auto_brightness_adjust;
    int       reg_adc;
    uint8_t   brightness;
    bool      active;
    bool      saver_enable;//屏保功能使能
    uint32_t  last_operaion;
    uint16_t  cnt;
}screen_info_t;

typedef String nm_ip_t;
typedef String nm_status_t;

typedef struct{
    board_info_t        board;
    screen_info_t       screen;
    connect_info_t      connection;
    float               timezone;
    bool                tz_updated;
    MarketClass         *market;
    bool                need_cfg;
}nm_sal_t;

extern nm_sal_t g_nm;
extern TaskHandle_t task_btn, task_ui, task_market, task_monitor, task_lvgl_tick, task_ui_refresh;

#endif // GLOBAL_H
