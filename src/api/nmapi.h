#ifndef _MARKET_H_
#define _MARKET_H_
#include <WiFi.h>
#include <ArduinoHttpClient.h> 
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>


class NMIoTAPIClass{
private:
    WiFiClientSecure    _sslwifi;
    HttpClient*         _httpclient;
public:
    NMIoTAPIClass();
    ~NMIoTAPIClass(){
        delete this->_httpclient;
    }
    String get_crypto_rank_price(int start, int limit, const String &convert = "USDT");
    String get_crypto_info(const String &slug = "bitcoin,ethereum,litecoin");
};

void nmapi_thread_entry(void *args);
#endif
