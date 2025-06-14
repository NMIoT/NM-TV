#include "monitor.h"
#include "logger.h"
#include "connection.h"
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <esp_system.h>
#include "helper.h"
#include "storage.h"
#include <nvs_flash.h>
#include <nvs.h>
#include "global.h"
#include "soc/rtc.h"

static WiFiUDP  udpNtpClient;
static const String   ntpServerUrl= "europe.pool.ntp.org";
static const uint32_t ntpInterval = 1000*60*60*24;//24h update interval
static NTPClient ntpClient(udpNtpClient, ntpServerUrl.c_str());


void monitor_thread_entry(void *args){
    char *name = (char*)malloc(20);
    strcpy(name, (char*)args);
    LOG_I("%s thread started on core %d...", name, xPortGetCoreID());
    free(name);

    uint32_t m_cnt = 0;

    ntpClient.begin();
    ntpClient.setTimeOffset((int)(g_nm.location.tz_offest * 3600)); // Set timezone offset in hours
    ntpClient.setUpdateInterval(ntpInterval);

    while(true){
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

        if(ntpClient.update()){
            struct timeval tv;
            tv.tv_sec = ntpClient.getEpochTime();
            tv.tv_usec = 0;
            settimeofday(&tv, NULL);
            g_nm.location.timestamp = tv.tv_sec;
            String time_local = convert_time_to_local(g_nm.location.timestamp);
            LOG_W("ntp calibrate time %s", time_local.c_str());
        }
        else{
            time_t now;
            time(&now);
            g_nm.location.timestamp = now;
        }



        if(xSemaphoreTake(g_nm.connection.wifi.reconnect_xsem, 0) == pdTRUE){
            WiFi.begin(g_nm.connection.wifi.conn_param.ssid.c_str(), g_nm.connection.wifi.conn_param.pwd.c_str());
            delay(1000);
            continue;
        }

        LOG_W("Free memory: %.3fkB, time : %s" , ESP.getFreeHeap() / 1024.0f, convert_time_to_local(g_nm.location.timestamp).c_str());
    }
    LOG_W("%s thread exit", __FUNCTION__);
    delay(1000);
    vTaskDelete(NULL);
}