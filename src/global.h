#ifndef GLOBAL_H
#define GLOBAL_H
#include <Arduino.h>
#include "connection.h"
#include "device.h"
#include "nmapi.h"
#include <map>
#include <list>

#define CURRENT_VERSION     "v0.0.02"
#define MINER_WTDG_TIMEOUT  (1000*60*15) //15分钟看门狗
#define SAVE_INTERVAL       (60*60) //每60分钟保存一次状态到NVS
#define WIFI_RSSI_STRONG    (-60)
#define WIFI_RSSI_GOOD      (-70)

#define PRICE_RANK_UPDATE_INTERVAL (1000*60) //update every minute
#define WEATHER_REALTIME_UPDATE_INTERVAL (1000*60*10) //update every 10 minutes
#define WEATHER_FORECAST_UPDATE_INTERVAL (1000*60*15) //update every 15 minutes
#define AIR_POLLUTION_UPDATE_INTERVAL (1000*60*20) //update every 20 minutes


#define LvglTaskCore      1
#define UiRefreshTaskCore 1
#define UiTaskCore        1
#define BtnTaskCore       1
#define ApiTaskCore       0
#define MonitorTaskCore   1

enum{
    // lowest priority
    TASK_PRIORITY_BTN = 1, 
    TASK_PRIORITY_API,
    TASK_PRIORITY_MONITOR, 
    TASK_PRIORITY_CONNECT, 
    TASK_PRIORITY_LED, 
    TASK_PRIORITY_CONFIG_MONITOR,
    TASK_PRIORITY_UI, 
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
        float         deg;
        float         gust;
        float         speed;
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
        float         deg;
        float         speed;
    }wind;
}weather_realtime_info_t;

/*************************** air pollution ***************************/
typedef struct{
    struct{
        float    co;    //ug/m3
        float    nh3;   //ug/m3
        float    no;    //ug/m3
        float    no2;   //ug/m3
        float    o3;    //ug/m3
        float    pm10;  //ug/m3
        float    pm2_5; //ug/m3
        float    so2;   //ug/m3
    }components;
    struct{
        int      aqi; // Air Quality Index
    }main;
    uint32_t      dt;
}air_pollution_node_t;

typedef struct{
    coord_info_t   coord;
    std::list<air_pollution_node_t> list; // List of forecast nodes
}air_pollution_info_t;

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
    String       last_updated;
}crypto_price_info_t;






typedef struct{
    String                  name;
    uint16_t                rank;
    int                     id; 
    String                  symbol;
    String                  slug;
    crypto_price_info_t     price;
    struct{
        uint8_t             *addr; // Pointer to icon data, can be NULL if not available
        uint16_t            size; // Size of the icon in bytes
        bool                updated; // Whether the icon has been updated
    }icon;//png icon data
}crypto_coin_node_t;

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
    // uint16_t  refresh_interval;
    int       sleep_timeout;
    bool      auto_brightness_adjust;
    int       reg_adc;
    uint8_t   brightness;
    bool      active;
    bool      saver_enable;
    uint32_t  last_operaion;
    uint16_t  cnt;
}screen_info_t;

typedef String             ccoin_name;
typedef crypto_coin_node_t ccoin_node;




typedef struct{
    coord_info_t        coord;    // fetch from public IP API
    float               tz_offest; // Timezone offset in hours, e.g., +8 for UTC+8
    bool                tz_updated;
    uint32_t            timestamp; 
}location_info_t;

typedef struct{
    SemaphoreHandle_t   coin_price_xsem; // Semaphore for coin price update
    SemaphoreHandle_t   weather_realtime_xsem; // Semaphore for weather realtime update
    SemaphoreHandle_t   weather_forecast_xsem; // Semaphore for weather forecast update
    SemaphoreHandle_t   air_pollution_xsem; // Semaphore for air pollution update

    SemaphoreHandle_t   next_page_xsem;       // semaphore for next page
    SemaphoreHandle_t   prev_page_xsem;       // semaphore for previous page
    SemaphoreHandle_t   ok_cancel_xsem;       // semaphore for ok or cancel action
}global_evet_drive_xem_t;




typedef struct{
    board_info_t        board;
    screen_info_t       screen;
    connect_info_t      connection;
    location_info_t     location;
    std::map<ccoin_name, ccoin_node> coin_price_rank;
    weather_realtime_info_t weather_realtime;
    weather_forecast_info_t weather_forecast;
    air_pollution_info_t    air_pollution;
    global_evet_drive_xem_t global_xsem;
    bool                need_cfg;
}nm_sal_t;

extern nm_sal_t g_nm;
extern TaskHandle_t task_btn, task_ui, task_nmapi, task_monitor, task_lvgl_tick, task_ui_refresh;

#endif // GLOBAL_H
