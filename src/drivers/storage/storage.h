#ifndef _STORAGE_H_
#define _STORAGE_H_
#include <Arduino.h>
#include "monitor.h"


#define NMTV_SETTINGS_NAMESPACE	  	"miner_settings" 
// #define MINER_STATUS_NAMESPACE	  	    "miner_status" 
// #define MINER_LICENCE_NAMESPACE	      	"dev_signature"

#define DEFAULT_CFG_AP_SSID				"nmap-2.4g"
// #define DEFAULT_CFG_AP_WIFIPW			"12345678"

#define DEFAULT_SSID					"NMTech-2.4G"
#define DEFAULT_WIFIPW					"NMMiner2048"

#define DEFAULT_TIMEZONE				"8"
#define DEFAULT_SAVESTATS				false
#define DEFAULT_REFRESH_INTERVAL 		2
#define DEFAULT_SCREEN_TIMEOUT			0
#define DEFAULT_SCREEN_BRIGHTNESS       100

// JSON config file SPIFFS (different for backward compatibility with existing devices)
#define JSON_SPIFFS_KEY_WIFISSID		"WifiSSID"
#define JSON_SPIFFS_KEY_WIFIPSWD 		"WiFiPSWD"

#define JSON_SPIFFS_KEY_TIMEZONE		"utcZone"
#define JSON_SPIFFS_KEY_REFRESHINTERV  	"RefreshInterval"
#define JSON_SPIFFS_KEY_SCREENTIMEOUT	"ScreenTimeout"
#define JSON_SPIFFS_KEY_SCREEN_BRIGHTNESS "Brightness"
#define JSON_SPIFFS_KEY_SCREEN_ORIENT  	"ScreenOrient"
#define JSON_SPIFFS_KEY_STATS2NV		"saveStatsToNVS"
#define JSON_SPIFFS_KEY_DEVCIE_CODE		"DeviceCode"
#define JSON_SPIFFS_KEY_LICENCE			"licence"

#define JSON_SPIFFS_KEY_MARKET_ENABLE  	"MarketEnable"//btc price
#define JSON_SPIFFS_KEY_UPTIME      	"Uptime"

#define JSON_SPIFFS_KEY_NEED_CFG      	"NeedConfig"
#define JSON_SPIFFS_KEY_LED_ENABLE    	"LedEnable"
#define JSON_SPIFFS_KEY_AUTO_BRIGHTNESS	"AutoBrightness"


void restore_to_factory_setting_cb();
void save_status_to_nvs();

bool is_license_exist(void);

bool load_g_nm(void);

char *   nvs_config_get_string(const char *ns, const char * key, const char * default_value);
int8_t  nvs_config_get_i8(const char *ns, const char * key,  const int8_t default_value);
uint8_t  nvs_config_get_u8(const char *ns, const char * key,  const uint8_t default_value);
uint16_t nvs_config_get_u16(const char *ns, const char * key, const uint16_t default_value);
uint32_t nvs_config_get_u32(const char *ns, const char * key, const uint32_t default_value);
uint64_t nvs_config_get_u64(const char *ns, const char * key, const uint64_t default_value);

void nvs_config_set_string(const char *ns, const char * key, const char * value);
void nvs_config_set_i8(const char *ns, const char * key, const int8_t value);
void nvs_config_set_u8(const char *ns, const char * key, const uint8_t value);
void nvs_config_set_u16(const char *ns, const char * key, const uint16_t value);
void nvs_config_set_u32(const char *ns, const char * key, const uint32_t value);
void nvs_config_set_u64(const char *ns, const char * key, const uint64_t value);

#endif // _STORAGE_H_