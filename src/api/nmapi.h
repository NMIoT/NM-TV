#ifndef _MARKET_H_
#define _MARKET_H_
#include <WiFi.h>
#include <ArduinoHttpClient.h> 
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>


class NMIoTAPIClass{
private:

public:
    NMIoTAPIClass(){

    }
    ~NMIoTAPIClass(){

    }
    String get_crypto_rank_price(int start, int limit, const String &convert = "USDT");
    String get_weather_realtime(const double &lat, const double &lon);
    String get_weather_forecast(const double &lat, const double &lon, const size_t &cnt = 16);// 3h every cnt , 3 * 8 = 24
    String get_air_pollution(const double &lat, const double &lon);
    static size_t download_coin_icon(int coin_id, uint8_t *buf, size_t buf_size);
    static size_t download_weather_icon(const String &icon_id, uint8_t *buf, size_t buf_size);
};

void nmapi_thread_entry(void *args);
#endif
