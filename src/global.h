#ifndef GLOBAL_H
#define GLOBAL_H
#include <Arduino.h>
#include "connection.h"
#include "device.h"
#include "market.h"


#define CURRENT_VERSION     "v1.7.03"
#define MINER_WTDG_TIMEOUT  (1000*60*15) //15分钟看门狗
#define SAVE_INTERVAL       (60*60) //每60分钟保存一次状态到NVS
#define WIFI_RSSI_STRONG    (-60)
#define WIFI_RSSI_GOOD      (-70)

#define StratumTaskCore   0
#define LvglTaskCore      0
#define UiRefreshTaskCore 0
#define UiTaskCore        0
#define BtnTaskCore       0
#define GaugeTaskCore     0
#define MarketTaskCore    0
#define MonitorTaskCore   0
#define SwarmTaskCore     0
#define NminerTaskCore    0
#if (SOC_CPU_CORES_NUM == 2)
    #define TminerTaskCore    1
#elif (SOC_CPU_CORES_NUM == 1) 
    #define TminerTaskCore    0
#endif


enum{
    // lowest priority
    TASK_PRIORITY_TMINER = 1, 
    TASK_PRIORITY_NMINER, 
    TASK_PRIORITY_BTN, 
    TASK_PRIORITY_MARKET,
    TASK_PRIORITY_MONITOR, 
    TASK_PRIORITY_STRATUM, 
    TASK_PRIORITY_SWARM, 
    TASK_PRIORITY_GAUGE, 
    TASK_PRIORITY_LICENCE, 
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
    String      licence;
    float       tempurature;
    float       vbus;
    float       ibus;
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


typedef struct{
  uint64_t normal; // hash cnt of normal mode
  uint64_t turbo;  // hash cnt of turbo mode
  uint64_t total;  // hash cnt of normal + turbo mode 
}miner_hashcnt_data;

typedef struct{
  double normal; // hash rate of normal mode
  double turbo;  // hash rate of turbo mode
  double total;  // hash rate of normal + turbo mode 
}miner_hashrate_data;

typedef struct{
    uint32_t rejected;
    uint32_t accepted;
}miner_shares_data;

typedef struct{
    double   pool;
    double   network;   
    double   last;
    double   best_session;
    double   best_ever;
}miner_diff_data;


typedef struct{
  bool new_job;
  bool stop_mining;
}miner_ctrl_data;

typedef String nm_ip_t;
typedef String nm_status_t;


typedef struct{
    board_info_t        board;
    screen_info_t       screen;
    connect_info_t      connection;
    float               timezone;
    bool                tz_updated;
    MarketClass         *market;
    bool                activate;
    bool                save_stats;
    bool                need_cfg;
    bool                market_enable;
    bool                led_enable;
}nm_sal_t;

extern nm_sal_t g_nm;
extern TaskHandle_t task_btn, task_ui, task_market, task_monitor, task_lvgl_tick, task_ui_refresh;

#endif // GLOBAL_H
