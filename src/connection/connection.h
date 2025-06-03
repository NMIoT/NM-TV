#ifndef _CONNECTION_H
#define _CONNECTION_H
#include <Arduino.h>
#include "WiFi.h"

typedef struct{
    String ssid;
    String pwd;
    String hostname;
} nm_wifi_conn_param_t;

typedef struct{
    IPAddress ip;
    String    pwd;
    String    ssid;
} nm_ap_conn_param_t;

typedef struct{
    IPAddress   ip;
    IPAddress   gateway;
    IPAddress   subnet;
    IPAddress   dns;
    int         rssi;
    wl_status_t status;
    uint16_t    config_timeout;
} nm_wifi_state_t;


typedef struct{
    nm_ap_conn_param_t      softap_param;
    nm_wifi_conn_param_t    conn_param;
    nm_wifi_state_t         status_param;
    SemaphoreHandle_t       reconnect_xsem;
    SemaphoreHandle_t       force_cfg_xsem;
}wifi_info_t;

bool wifi_config(bool blocking);
void wifi_connect_thread_entry(void *args);

#endif // _CONNECTION_H
