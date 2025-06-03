#include "storage.h"
#include <nvs_flash.h>
#include <nvs.h>
#include "global.h"
#include "logger.h"
#include "helper.h"
#include "market.h"

char * nvs_config_get_string(const char *ns, const char * key, const char * default_value){
    nvs_handle handle;
    esp_err_t err;
    err = nvs_open(ns, NVS_READONLY, &handle);
    if (err != ESP_OK) {
        return strdup(default_value);
    }

    size_t size = 0;
    err = nvs_get_str(handle, key, NULL, &size);

    if (err != ESP_OK) {
        return strdup(default_value);
    }

    char * out = (char *)malloc(size);
    err = nvs_get_str(handle, key, out, &size);

    if (err != ESP_OK) {
        free(out);
        return strdup(default_value);
    }

    nvs_close(handle);
    return out;
}

void nvs_config_set_string(const char *ns, const char * key, const char * value){
    nvs_handle handle;
    esp_err_t err;
    err = nvs_open(ns, NVS_READWRITE, &handle);
    if (err != ESP_OK) {
        // LOG_W("Could not open namespace [%s]", ns);
        return;
    }

    err = nvs_set_str(handle, key, value);
    if (err != ESP_OK) {
        // LOG_W("Could not write nvs key: %s, value: %s", key, value);
        return;
    }

    nvs_close(handle);
    return;
}


int8_t nvs_config_get_i8(const char *ns, const char * key, const int8_t default_value){
    nvs_handle handle;
    esp_err_t err;
    err = nvs_open(ns, NVS_READONLY, &handle);
    if (err != ESP_OK) {
        return default_value;
    }

    int8_t out;
    err = nvs_get_i8(handle, key, &out);
    nvs_close(handle);
    
    if (err != ESP_OK) {
        return default_value;
    }
    return out;
}

void nvs_config_set_i8(const char *ns, const char * key, const int8_t value){
    nvs_handle handle;
    esp_err_t err;
    err = nvs_open(ns, NVS_READWRITE, &handle);
    if (err != ESP_OK) {
        return;
    }

    err = nvs_set_i8(handle, key, value);
    if (err != ESP_OK) {
        return;
    }

    nvs_close(handle);
    return;
}



uint8_t nvs_config_get_u8(const char *ns, const char * key, const uint8_t default_value){
    nvs_handle handle;
    esp_err_t err;
    err = nvs_open(ns, NVS_READONLY, &handle);
    if (err != ESP_OK) {
        // LOG_W("Could not open namespace [%s]", ns);
        return default_value;
    }

    uint8_t out;
    err = nvs_get_u8(handle, key, &out);
    nvs_close(handle);
    
    if (err != ESP_OK) {
        return default_value;
    }
    return out;
}

void nvs_config_set_u8(const char *ns, const char * key, const uint8_t value){
    nvs_handle handle;
    esp_err_t err;
    err = nvs_open(ns, NVS_READWRITE, &handle);
    if (err != ESP_OK) {
        // LOG_W("Could not open namespace [%s]", ns);
        return;
    }

    err = nvs_set_u8(handle, key, value);
    if (err != ESP_OK) {
        // LOG_W("Could not write nvs key: %s", key);
        return;
    }

    nvs_close(handle);
    return;
}


uint16_t nvs_config_get_u16(const char *ns, const char * key, const uint16_t default_value){
    nvs_handle handle;
    esp_err_t err;
    err = nvs_open(ns, NVS_READONLY, &handle);
    if (err != ESP_OK) {
        // LOG_W("Could not open namespace [%s]", ns);
        return default_value;
    }

    uint16_t out;
    err = nvs_get_u16(handle, key, &out);
    nvs_close(handle);
    
    if (err != ESP_OK) {
        return default_value;
    }
    return out;
}

void nvs_config_set_u16(const char *ns, const char * key, const uint16_t value){
    nvs_handle handle;
    esp_err_t err;
    err = nvs_open(ns, NVS_READWRITE, &handle);
    if (err != ESP_OK) {
        // LOG_W("Could not open namespace [%s]", ns);
        return;
    }

    err = nvs_set_u16(handle, key, value);
    if (err != ESP_OK) {
        // LOG_W("Could not write nvs key: %s", key);
        return;
    }

    nvs_close(handle);
    return;
}





uint32_t nvs_config_get_u32(const char *ns, const char * key, const uint32_t default_value){
    nvs_handle handle;
    esp_err_t err;
    err = nvs_open(ns, NVS_READONLY, &handle);
    if (err != ESP_OK) {
        return default_value;
    }

    uint32_t out;
    err = nvs_get_u32(handle, key, &out);
    nvs_close(handle);
    
    if (err != ESP_OK) {
        return default_value;
    }
    return out;
}

void nvs_config_set_u32(const char *ns, const char * key, const uint32_t value){
    nvs_handle handle;
    esp_err_t err;
    err = nvs_open(ns, NVS_READWRITE, &handle);
    if (err != ESP_OK) {
        // LOG_W("Could not open namespace [%s]", ns);
        return;
    }

    err = nvs_set_u32(handle, key, value);
    if (err != ESP_OK) {
        // LOG_W("Could not write nvs key: %s", key);
        return;
    }

    nvs_close(handle);
    return;
}





uint64_t nvs_config_get_u64(const char *ns, const char * key, const uint64_t default_value){
    nvs_handle handle;
    esp_err_t err;
    err = nvs_open(ns, NVS_READONLY, &handle);
    if (err != ESP_OK) {
        return default_value;
    }

    uint64_t out;
    err = nvs_get_u64(handle, key, &out);

    if (err != ESP_OK) {
        return default_value;
    }

    nvs_close(handle);
    return out;
}

void nvs_config_set_u64(const char *ns, const char * key, const uint64_t value){
    nvs_handle handle;
    esp_err_t err;
    err = nvs_open(ns, NVS_READWRITE, &handle);
    if (err != ESP_OK) {
        // LOG_W("Could not open namespace [%s]", ns);
        return;
    }

    err = nvs_set_u64(handle, key, value);
    if (err != ESP_OK) {
        // LOG_W("Could not write nvs key: %s, value: %llu", key, value);
        return;
    }
    nvs_close(handle);
    return;
}

void restore_to_factory_setting_cb(){
    esp_err_t err;
    err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        LOG_W("NVS partition is full or has invalid version, erasing...");
        err = nvs_flash_erase();
        if (err != ESP_OK) {
            LOG_E("Failed to erase NVS partition: %s", esp_err_to_name(err));
            return;
        }
        err = nvs_flash_init();
        if (err != ESP_OK) {
            LOG_E("Failed to initialize NVS after erase: %s", esp_err_to_name(err));
            return;
        }
    } else if (err != ESP_OK) {
        LOG_E("Failed to initialize NVS: %s", esp_err_to_name(err));
        return;
    }
    err = nvs_flash_erase();
    if (err != ESP_OK) {
        LOG_E("Failed to erase NVS partition: %s", esp_err_to_name(err));
        return;
    }

    err = nvs_flash_init();
    if (err != ESP_OK) {
        LOG_E("Failed to initialize NVS after erase: %s", esp_err_to_name(err));
        return;
    }
    LOG_I("NVS partition erased and reinitialized successfully");

    //licence save back
    nvs_config_set_string(MINER_LICENCE_NAMESPACE, JSON_SPIFFS_KEY_LICENCE, g_nm.board.licence.c_str());
    delay(1000);
    ESP.restart();
}


void save_status_to_nvs(){
    
}




bool is_license_exist(void){
    esp_err_t ret = nvs_flash_init();
    g_nm.board.licence  = nvs_config_get_string(MINER_LICENCE_NAMESPACE, JSON_SPIFFS_KEY_LICENCE, "");
    return (g_nm.board.licence != "");
}

bool load_g_nm(void){
    static bool nvs_init_flag = false;
    if(!nvs_init_flag){
        LOG_I("Initializing NVS...");
        esp_err_t ret = nvs_flash_init();
        while (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
            LOG_W("NVS partition is full or has invalid version, erasing...");
            if(nvs_flash_erase() != ESP_OK){
                LOG_E("NVS partition erase failed");
            }
            LOG_I("Reinitializing NVS...");
            ret = nvs_flash_init();
            delay(1000);
        }
        g_nm.connection.wifi.reconnect_xsem  = xSemaphoreCreateCounting(1, 0);
        g_nm.connection.wifi.force_cfg_xsem  = xSemaphoreCreateCounting(1, 0);
        g_nm.board.fw_version                = CURRENT_VERSION;
        g_nm.board.fw_latest_release         = "";
        g_nm.board.hw_version                = "1.0.0";
        g_nm.board.model                     = BOARD_MODEL;
        g_nm.activate                        = false;
#if defined(HAS_MARKET_FEATURE)
        g_nm.market_enable                   = nvs_config_get_u8(MINER_SETTINGS_NAMESPACE, JSON_SPIFFS_KEY_MARKET_ENABLE, true);
#else
        g_nm.market_enable                   = false;
#endif
        g_nm.board.licence                   = nvs_config_get_string(MINER_LICENCE_NAMESPACE, JSON_SPIFFS_KEY_LICENCE, "");
        g_nm.screen.orientation              = nvs_config_get_u8(MINER_SETTINGS_NAMESPACE, JSON_SPIFFS_KEY_SCREEN_ORIENT, false);
        g_nm.screen.refresh_interval         = nvs_config_get_u8(MINER_SETTINGS_NAMESPACE, JSON_SPIFFS_KEY_REFRESHINTERV, DEFAULT_REFRESH_INTERVAL);
        g_nm.screen.refresh_interval         = (g_nm.screen.refresh_interval < 1) ? 1 : g_nm.screen.refresh_interval;
        g_nm.screen.sleep_timeout            = nvs_config_get_u16(MINER_SETTINGS_NAMESPACE, JSON_SPIFFS_KEY_SCREENTIMEOUT, DEFAULT_SCREEN_TIMEOUT);
        g_nm.screen.brightness               = nvs_config_get_u8(MINER_SETTINGS_NAMESPACE, JSON_SPIFFS_KEY_SCREEN_BRIGHTNESS, DEFAULT_SCREEN_BRIGHTNESS);
        g_nm.screen.auto_brightness_adjust   = nvs_config_get_u8(MINER_SETTINGS_NAMESPACE, JSON_SPIFFS_KEY_AUTO_BRIGHTNESS, false);
        
        g_nm.screen.active                   = true;
        g_nm.connection.client_connected     = false;

        g_nm.timezone                        = 0.0f;
        g_nm.save_stats                      = nvs_config_get_u8(MINER_SETTINGS_NAMESPACE, JSON_SPIFFS_KEY_STATS2NV, DEFAULT_SAVESTATS);
        g_nm.need_cfg                        = nvs_config_get_u8(MINER_SETTINGS_NAMESPACE, JSON_SPIFFS_KEY_NEED_CFG, false);
        g_nm.led_enable                      = nvs_config_get_u8(MINER_SETTINGS_NAMESPACE, JSON_SPIFFS_KEY_LED_ENABLE, true);
        nvs_config_set_u32(MINER_STATUS_NAMESPACE, JSON_SPIFFS_KEY_BLOCK_HITS, 0);//clear block hits after v0.5.03 for some wrong data
        if(g_nm.save_stats){
           
        }
        else{

        }
        g_nm.market    = (g_nm.market_enable) ? new MarketClass(MARKET_HOST, MARKET_PORT, MARKET_URL, "BTC_USDT") : NULL;
        nvs_init_flag = true;
    }
    
    g_nm.connection.wifi.conn_param.ssid = nvs_config_get_string(MINER_SETTINGS_NAMESPACE, JSON_SPIFFS_KEY_WIFISSID, DEFAULT_SSID);
    g_nm.connection.wifi.conn_param.pwd  = nvs_config_get_string(MINER_SETTINGS_NAMESPACE, JSON_SPIFFS_KEY_WIFIPSWD, DEFAULT_WIFIPW);
    return true;
}