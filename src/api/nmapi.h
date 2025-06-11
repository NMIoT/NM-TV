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
    String get_crypto_info(const String &slug = "bitcoin");
    static size_t download_coin_icon(int coin_id, uint8_t *buf, size_t buf_size);
};

void nmapi_thread_entry(void *args);
#endif
