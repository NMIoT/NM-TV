#ifndef GLOBAL_H
#define GLOBAL_H
#include <Arduino.h>
#include "connection.h"
#include "device.h"
#include "nmapi.h"
#include <map>
#include <list>

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
#define MonitorTaskCore   1

enum{
    // lowest priority
    TASK_PRIORITY_BTN = 1, 
    TASK_PRIORITY_MONITOR, 
    TASK_PRIORITY_MARKET,
    TASK_PRIORITY_CONNECT, 
    TASK_PRIORITY_LED, 
    TASK_PRIORITY_CONFIG_MONITOR,
    TASK_PRIORITY_DISPLAY, 
    TASK_PRIORITY_UI_REFRESH,
    TASK_PRIORITY_LVGL_DRV, 
    //highest priority
};



typedef struct{
    double      lat;
    double      lon; 
}coord_info_t;

typedef struct{
    String      description;
    String      icon;
    String      main;
    int         id;
}weather_info_t;

typedef struct{
    struct{
        int         all;
    }clouds;
    uint32_t       dt;
    String         dt_txt;
    struct{
        float       feels_like;
        float       grnd_level;
        float       humidity;
        float       pressure;
        float       sea_level;
        float       temp;
        float       temp_kf; // 3h step
        float       temp_max;
        float       temp_min;
    }main;
    uint32_t       pop; // Probability of precipitation
    uint32_t       visibility;
    weather_info_t weather;
    struct{
        int         deg;
        int         gust;
        int         speed;
    }wind;
    struct{
        String      pod;
    }sys;
}forecast_node_t;

/*************************** weather forecast ***************************/
typedef struct{
    struct{
        coord_info_t  coord;
        String         country;
        uint32_t       id;
        String         name;
        uint32_t       population;
        uint32_t       sunrise;
        uint32_t       sunset;
        uint32_t       timezone; // Timezone offset in seconds
    }city;
    uint32_t       cnt; // Number of forecast nodes
    String         cod; // Response code
    uint32_t       message; // Response message
    // forecast_node_t *list; // Array of forecast nodes
    std::list<forecast_node_t> list; // List of forecast nodes
}weather_forecast_info_t;

/*************************** weather realtime ***************************/
typedef struct{
    String         base;
    struct{
        int         all;
    }clouds;
    uint32_t       cod;// Response code
    coord_info_t   coord;
    uint32_t       dt;
    uint32_t       id;
    struct{
        float       feels_like;
        float       grnd_level;
        float       humidity;
        float       pressure;
        float       sea_level;
        float       temp;
        float       temp_kf; // 3h step
        float       temp_max;
        float       temp_min;
    }main;
    String         name;
    struct {
        String country;
        uint32_t id;
        String sunrise;
        String sunset;
        uint32_t type;
    }sys;
    uint32_t timezone;// Timezone offset in seconds
    uint32_t visibility;
    weather_info_t weather;
    struct{
        int         deg;
        int         speed;
    }wind;
}weather_realtime_info_t;



/*************************** Crypto Price ***************************/
typedef struct{
    double       realtime;
    double       market_cap;
    double       percent_change_1h;
    double       percent_change_24h;
    double       percent_change_7d;
    double       percent_change_30d;
    double       percent_change_60d;
    double       percent_change_90d;
    double       volume_24h;
    double       volume_change_24h;
    String      last_updated;
}crypto_price_info_t;


typedef struct{
    String                  name;
    int                     id; 
    String                  symbol;
    String                  slug;
    crypto_price_info_t     price;
    uint16_t                rank;
}crypto_coin_info_t;

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

typedef String             coin_name;
typedef crypto_coin_info_t ccoin_info;

typedef struct{
    board_info_t        board;
    screen_info_t       screen;
    connect_info_t      connection;
    float               timezone;
    bool                tz_updated;
    std::map<coin_name, ccoin_info> coin_map;

    weather_realtime_info_t weather_realtime;
    weather_forecast_info_t weather_forecast;

    bool                need_cfg;
    uint8_t             *coin_icon;
    bool                coin_icon_updated;
}nm_sal_t;

extern nm_sal_t g_nm;
extern TaskHandle_t task_btn, task_ui, task_market, task_monitor, task_lvgl_tick, task_ui_refresh;

#endif // GLOBAL_H
