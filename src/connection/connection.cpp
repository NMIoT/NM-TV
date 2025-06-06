#include <WiFi.h>
#include <WiFiManager.h>
#include "ArduinoJson.h"
#include "logger.h"
#include "display.h"
#include "storage.h"
#include "global.h"

#define CONFIG_TIMEOUT 60*10

static void saveConfigCallback(){
    LOG_W("save Config Callback...");
}

void config_timeout_monitor_thread_entry(void *args){
    char *name = (char*)malloc(20);
    strcpy(name, (char*)args);
    LOG_I("%s thread started on core %d...", name, xPortGetCoreID());
    free(name);
    
    uint16_t timeout = 0;
    g_nm.connection.wifi.status_param.config_timeout = CONFIG_TIMEOUT;
    while(true){
        if (WL_CONNECTED == g_nm.connection.wifi.status_param.status) {
            break;
        }
        g_nm.connection.client_connected = (WiFi.softAPgetStationNum() > 0);
        if(g_nm.connection.client_connected == false){
            LOG_W("WiFi configuration time left: %ds", g_nm.connection.wifi.status_param.config_timeout);
            if(timeout++ >= CONFIG_TIMEOUT){
                LOG_W("WiFi configuration timeout, rebooting...");
                delay(1000);
                ESP.restart();
            }
            g_nm.connection.wifi.status_param.config_timeout = CONFIG_TIMEOUT - timeout;
        }
        else{
            LOG_W("Client connected, waiting for configuration...");
        }
        delay(1000);
    }
    LOG_I("WiFi configuration monitor exit...");
    vTaskDelete(NULL);
}

bool wifi_config(bool blocking){
    static TaskHandle_t configMonitorTaskHandle = NULL;
    if(configMonitorTaskHandle == NULL){
        String taskName = "(config_monitor)";
        xTaskCreatePinnedToCore(config_timeout_monitor_thread_entry, taskName.c_str(), 1024*4, (void*)taskName.c_str(), TASK_PRIORITY_CONFIG_MONITOR, &configMonitorTaskHandle, 1);
    }

    // Define WiFiManager Object
    WiFiManager wm;

    wm.setBreakAfterConfig(true); //Set to detect config edition and save 

    //Set dark theme
    wm.setClass("invert"); // dark theme

    // Set config save notify callback
    wm.setSaveConfigCallback(saveConfigCallback);

    //Advanced MinerSettings
    wm.setConnectTimeout(50); // how long to try to connect for before continuing
    wm.setShowInfoErase(false); // Hide erase info
    wm.setShowInfoUpdate(false); // Hide update info
    wm.setShowPassword(true); // Show password

    WiFiManagerParameter tb_ui_refresh_intv(JSON_SPIFFS_KEY_REFRESHINTERV, "UI refresh interval(second)", String(g_nm.screen.refresh_interval).c_str(), 2);
    WiFiManagerParameter tb_scr_sleep(JSON_SPIFFS_KEY_SCREENTIMEOUT, "Screen sleep time out, second (0s for always on)", String(g_nm.screen.sleep_timeout).c_str(), 4);
    WiFiManagerParameter tb_scr_brightness(JSON_SPIFFS_KEY_SCREEN_BRIGHTNESS, "Initial screen brightnes(0~100)", String(g_nm.screen.brightness).c_str(), 3);

    char checkboxOrient[24] = "type=\"checkbox\"";
    if (g_nm.screen.orientation){
        strcat(checkboxOrient, " checked");
    }
    WiFiManagerParameter ck_screen_orient(JSON_SPIFFS_KEY_SCREEN_ORIENT, "Rotate screen", "T", 2, checkboxOrient, WFM_LABEL_AFTER);



#if defined(TFT_DISPLAY) || defined(OLED_DISPLAY)
    wm.addParameter(&tb_ui_refresh_intv);
#endif

#if ((TFT_BL != -1) && defined(TFT_DISPLAY)) || defined(AMOLED_DISPLAY)
    wm.addParameter(&tb_scr_sleep);
    wm.addParameter(&tb_scr_brightness);
#endif
    wm.addParameter(&ck_screen_orient);
    wm.erase(); // Clear previous settings
    wm.setConfigPortalBlocking(blocking);
    if(wm.startConfigPortal(DEFAULT_CFG_AP_SSID, NULL)){
        //Could be break forced after edditing, so save new config

        LOG_I("Save parameters to NVS");
        LOG_W("Rebooting...");
        delay(2000);
        ESP.restart();
    }
    else{
        return false;
    }
    return true;
}

static void WiFiEvent(WiFiEvent_t event){
    static uint8_t retry_cnt = 0, max_retries = 10;
    switch (event) {
        case ARDUINO_EVENT_WIFI_READY: 
            LOG_I("WiFi interface ready");
            break;
        case ARDUINO_EVENT_WIFI_SCAN_DONE:
            LOG_I("Completed scan for access points");
            break;
        case ARDUINO_EVENT_WIFI_STA_START:
            LOG_I("WiFi client started");
            break;
        case ARDUINO_EVENT_WIFI_STA_STOP:
            LOG_W("WiFi clients stopped");
            break;
        case ARDUINO_EVENT_WIFI_STA_CONNECTED:
            LOG_I("Connected to access point, SSID: %s", WiFi.SSID().c_str());
            break;
        case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
            g_nm.connection.wifi.status_param.status = WL_DISCONNECTED;
            retry_cnt++;
            LOG_W("Disconnected from WiFi access point %d times", retry_cnt);
            break;
        case ARDUINO_EVENT_WIFI_STA_AUTHMODE_CHANGE:
            LOG_W("Authentication mode of access point has changed");
            break;
        case ARDUINO_EVENT_WIFI_STA_GOT_IP:
            g_nm.connection.wifi.status_param.ip = WiFi.localIP();
            g_nm.connection.wifi.status_param.status = WL_CONNECTED;
            retry_cnt = 0;
            LOG_I("Got IP address: %s", WiFi.localIP().toString().c_str());
            break;
        case ARDUINO_EVENT_WIFI_STA_LOST_IP:
            LOG_W("Lost IP address and IP address is reset to 0");
            break;
        case ARDUINO_EVENT_WPS_ER_SUCCESS:
            LOG_I("WiFi Protected Setup (WPS): succeeded in enrollee mode");
            break;
        case ARDUINO_EVENT_WPS_ER_FAILED:
            LOG_W("WiFi Protected Setup (WPS): failed in enrollee mode");
            break;
        case ARDUINO_EVENT_WPS_ER_TIMEOUT:
            LOG_W("WiFi Protected Setup (WPS): timeout in enrollee mode");
            break;
        case ARDUINO_EVENT_WPS_ER_PIN:
            LOG_W("WiFi Protected Setup (WPS): pin code in enrollee mode");
            break;
        case ARDUINO_EVENT_WIFI_AP_START:
            LOG_I("WiFi access point started");
            break;
        case ARDUINO_EVENT_WIFI_AP_STOP:
            LOG_W("WiFi access point  stopped");
            break;
        case ARDUINO_EVENT_WIFI_AP_STACONNECTED:
            LOG_I("Client connected");
            break;
        case ARDUINO_EVENT_WIFI_AP_STADISCONNECTED:
            LOG_W("Client disconnected");
            break;
        case ARDUINO_EVENT_WIFI_AP_STAIPASSIGNED:
            LOG_I("Assigned IP address to client");
            break;
        case ARDUINO_EVENT_WIFI_AP_PROBEREQRECVED:
            LOG_I("Received probe request");
            break;
        case ARDUINO_EVENT_WIFI_AP_GOT_IP6:
            LOG_I("AP IPv6 is preferred");
            break;
        case ARDUINO_EVENT_WIFI_STA_GOT_IP6:
            LOG_I("STA IPv6 is preferred");
            break;
        case ARDUINO_EVENT_ETH_GOT_IP6:
            LOG_I("Ethernet IPv6 is preferred");
            break;
        case ARDUINO_EVENT_ETH_START:
            LOG_I("Ethernet started");
            break;
        case ARDUINO_EVENT_ETH_STOP:
            LOG_W("Ethernet stopped");
            break;
        case ARDUINO_EVENT_ETH_CONNECTED:
            LOG_I("Ethernet connected");
            break;
        case ARDUINO_EVENT_ETH_DISCONNECTED:
            LOG_W("Ethernet disconnected");
            break;
        case ARDUINO_EVENT_ETH_GOT_IP:
            LOG_I("Obtained IP address");
            break;
        default: 
            LOG_W("Unhandled wifi event: %d", event);
            break;
    }
    if(retry_cnt >= max_retries){
        LOG_E("WiFi connection error, restart device...");
        ESP.restart();
    }
}


const char* getAuthModeName(wifi_auth_mode_t authmode) {
    switch (authmode) {
        case WIFI_AUTH_OPEN:
            return "Open";
        case WIFI_AUTH_WEP:
            return "WEP";
        case WIFI_AUTH_WPA_PSK:
            return "WPA-PSK";
        case WIFI_AUTH_WPA2_PSK:
            return "WPA2-PSK";
        case WIFI_AUTH_WPA_WPA2_PSK:
            return "WPA/WPA2-PSK";
        case WIFI_AUTH_WPA2_ENTERPRISE:
            return "WPA2-Enterprise";
        case WIFI_AUTH_WPA3_PSK:
            return "WPA3-PSK";
        case WIFI_AUTH_WPA2_WPA3_PSK:
            return "WPA2/WPA3-PSK";
        case WIFI_AUTH_WAPI_PSK:
            return "WAPI-PSK";
        default:
            return "Unknown";
    }
}

const char* getCipherTypeName(wifi_cipher_type_t cipher) {
    switch (cipher) {
        case WIFI_CIPHER_TYPE_NONE:
            return "None";
        case WIFI_CIPHER_TYPE_WEP40:
            return "WEP40";
        case WIFI_CIPHER_TYPE_WEP104:
            return "WEP104";
        case WIFI_CIPHER_TYPE_TKIP:
            return "TKIP";
        case WIFI_CIPHER_TYPE_CCMP:
            return "CCMP";
        case WIFI_CIPHER_TYPE_TKIP_CCMP:
            return "TKIP/CCMP";
        case WIFI_CIPHER_TYPE_AES_CMAC128:
            return "AES-CMAC128";
        case WIFI_CIPHER_TYPE_SMS4:
            return "SMS4";
        default:
            return "Unknown";
    }
}

static bool wifi_connecet(){
    const uint8_t connect_timeout = 20;

    WiFi.onEvent(WiFiEvent);
    WiFi.mode(WIFI_STA);
    WiFi.setTxPower(WIFI_POWER_17dBm);
    delay(100);
    LOG_I("Try to connect [%s]...", g_nm.connection.wifi.conn_param.ssid.c_str());
    WiFi.begin(g_nm.connection.wifi.conn_param.ssid.c_str(), g_nm.connection.wifi.conn_param.pwd.c_str());

    while (WL_CONNECTED != g_nm.connection.wifi.status_param.status) {
        static int maxRetries = 0;
        maxRetries++;
        LOG_W("Waiting ip %ds...", maxRetries);
        if(maxRetries >= connect_timeout){
            return false;
        }
        delay(1000);
    }

    // 获取当前连接的AP信息
    wifi_ap_record_t ap_info;
    String authmode = "Unknown";
    if (esp_wifi_sta_get_ap_info(&ap_info) == ESP_OK) {
        switch (ap_info.authmode) {
            case WIFI_AUTH_OPEN:
                authmode = "Open";
                break;
            case WIFI_AUTH_WEP:
                authmode = "WEP";
                break;
            case WIFI_AUTH_WPA_PSK:
                authmode = "WPA-PSK";
                break;
            case WIFI_AUTH_WPA2_PSK:
                authmode = "WPA2-PSK";
                break;
            case WIFI_AUTH_WPA_WPA2_PSK:
                authmode = "WPA/WPA2-PSK";
                break;
            case WIFI_AUTH_WPA2_ENTERPRISE:
                authmode = "WPA2-Enterprise";
                break;
            case WIFI_AUTH_WPA3_PSK:
                authmode = "WPA3-PSK";
                break;
            case WIFI_AUTH_WPA2_WPA3_PSK:
                authmode = "WPA2/WPA3-PSK";
                break;
            case WIFI_AUTH_WAPI_PSK:
                authmode = "WAPI-PSK";
                break;
            default:
                authmode = "Unknown";
                break;
        }
    } 

    LOG_I("--------------------------------------------------");
    LOG_I("IP               : %s ", WiFi.localIP().toString().c_str());
    LOG_I("DNS              : %s, %s", WiFi.dnsIP(0).toString().c_str(), WiFi.dnsIP(1).toString().c_str());
    LOG_I("Gateway          : %s", WiFi.gatewayIP().toString().c_str());
    LOG_I("Subnet           : %s", WiFi.subnetMask().toString().c_str());
    LOG_I("MAC              : %s", WiFi.macAddress().c_str());
    LOG_I("--------------------------------------------------");
    LOG_I("Sleep Mode       : %s", (WiFi.getSleep() ? "Enabled" : "Disabled"));
    LOG_I("SSID             : %s", ap_info.ssid);
    LOG_I("BSSID            : %02X:%02X:%02X:%02X:%02X:%02X", ap_info.bssid[0], ap_info.bssid[1], ap_info.bssid[2], ap_info.bssid[3], ap_info.bssid[4], ap_info.bssid[5]);
    // LOG_I("Primary Chan     : %d", ap_info.primary);
    // LOG_I("Second Chan      : %d", ap_info.second);
    LOG_I("RSSI             : %d dBm", ap_info.rssi);
    LOG_I("Auth Mode        : %s", getAuthModeName(ap_info.authmode));
    LOG_I("Pairwise Cipher  : %s", getCipherTypeName(ap_info.pairwise_cipher));
    LOG_I("Group Cipher     : %s", getCipherTypeName(ap_info.group_cipher));
    // LOG_I("Antenna          : %d", ap_info.ant);
    // LOG_I("PHY 11b          : %s", ap_info.phy_11b ? "y" : "n");
    // LOG_I("PHY 11g          : %s", ap_info.phy_11g ? "y" : "n");
    // LOG_I("PHY 11n          : %s", ap_info.phy_11n ? "y" : "n");
    // LOG_I("PHY LR           : %s", ap_info.phy_lr ? "y" : "n");
    // LOG_I("WPS              : %s", ap_info.wps ? "y" : "n");
    // LOG_I("FTM Responder    : %s", ap_info.ftm_responder ? "y" : "n");
    // LOG_I("FTM Initiator    : %s", ap_info.ftm_initiator ? "y" : "n");
    // LOG_I("Country          : %s", ap_info.country.cc);
    LOG_I("--------------------------------------------------");
    LOG_I("WiFi connected!");//同步头，勿改
    return true;
}

static void wait_wifi_config_from_uart_thread_entry(void *args){
    StaticJsonDocument <256> json_config;
    uint8_t config_flg = 0x00;
    while (true) {
        uint16_t len = Serial.available();
        if (len > 0) {
            char *buffer = (char*)malloc(len + 1); // +1 是为了确保字符串以 null 终止
            if (buffer == NULL) {
                LOG_E("Failed to allocate memory");
                continue;
            }

            Serial.readBytes(buffer, len);
            buffer[len] = '\0'; // 确保字符串以 null 终止

            DeserializationError error = deserializeJson(json_config, buffer);
            if (error) {
                LOG_E("deserializeJson failed: %s", error.c_str());
                free(buffer);
                continue;
            }

            if(json_config.containsKey("ssid")){
                g_nm.connection.wifi.conn_param.ssid = json_config["ssid"].as<const char*>();
                nvs_config_set_string(NMTV_SETTINGS_NAMESPACE, JSON_SPIFFS_KEY_WIFISSID, g_nm.connection.wifi.conn_param.ssid.c_str());
                LOG_I("Save Wifi SSID: %s", g_nm.connection.wifi.conn_param.ssid.c_str());
                config_flg = config_flg | 0x01;
            }
            if(json_config.containsKey("password")){
                g_nm.connection.wifi.conn_param.pwd = json_config["password"].as<const char*>();
                nvs_config_set_string(NMTV_SETTINGS_NAMESPACE, JSON_SPIFFS_KEY_WIFIPSWD, g_nm.connection.wifi.conn_param.pwd.c_str());
                LOG_I("Save Wifi Password: %s", g_nm.connection.wifi.conn_param.pwd.c_str());
                config_flg = config_flg | 0x02;
            }
            // if(json_config.containsKey("address")){
            //     g_nm.connection.stratum_primary.user = json_config["address"].as<const char*>();
            //     nvs_config_set_string(NMTV_SETTINGS_NAMESPACE, JSON_SPIFFS_KEY_WALLET_PRI, g_nm.connection.stratum_primary.user.c_str());
            //     LOG_I("Save Wallet Address: %s", g_nm.connection.stratum_primary.user.c_str());
            //     config_flg = config_flg | 0x04;
            // }

            //如果配置完成, 则重启设备
            if(config_flg == 0x07){
                LOG_I("WiFi and user configuration completed, rebooting...");
                delay(1000);
                ESP.restart();
            }
            free(buffer);
        }
    }
    vTaskDelete(NULL);
}

void wifi_connect_thread_entry(void *args){
    uint16_t random_delay = random(0, 1000*10);
    LOG_W("Initializing WiFi, random delay: %d ms", random_delay);
    delay(random_delay);//避免多个设备同时连接路由器 导致路由器拒绝连接

    char *name = (char*)malloc(20);
    strcpy(name, (char*)args);
    LOG_I("%s thread started on core %d...", name, xPortGetCoreID());
    free(name);
    if(!wifi_connecet()){
        //配置请求头，勿改
        LOG_W("WiFi connect timeout, configuration mode triggered...");
        xSemaphoreGive(g_nm.connection.wifi.force_cfg_xsem);
        g_nm.need_cfg = true;
        String taskName = "(wifi_config)";
        xTaskCreatePinnedToCore(wait_wifi_config_from_uart_thread_entry, taskName.c_str(), 1024*3, (void*)taskName.c_str(), 3, NULL, 1);
        while (!wifi_config(true)){
            LOG_E("WiFi configuration failed, retrying in 2s...");
            delay(2000);
        }
    }
    vTaskDelete(NULL);
}