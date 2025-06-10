#include "nmapi.h"
#include "logger.h"
#include <ArduinoJson.h>
#include "global.h"


#define NMAPI_HOST "api.nmiot.net" // The host for the NMIoT API
#define NMAPI_PORT 443             // The port for the NMIoT API, usually 443 for HTTPS

NMIoTAPIClass::NMIoTAPIClass(){
    this->_sslwifi.setInsecure(); // Disable SSL certificate verification for testing purposes
    this->_httpclient = new HttpClient(this->_sslwifi, NMAPI_HOST, NMAPI_PORT);
}


String NMIoTAPIClass::get_crypto_rank_price(int start, int limit, const String &convert){
    String url = "/crypto/price?start=" + String(start) + "&limit=" + String(limit) + "&convert=" + convert;
    LOG_I("Requesting: %s", url.c_str());

    int httpCode = this->_httpclient->get(url);
    if (httpCode < 0) {
        LOG_E("HTTP GET failed, code: %d", httpCode);
        return "";
    }

    int status = this->_httpclient->responseStatusCode();
    if (status != 200) {
        LOG_E("HTTP response status: %d", status);
        return "";
    }

    String response = this->_httpclient->responseBody();
    LOG_I("Response: %s", response.c_str());
    return response;
}



void nmapi_thread_entry(void *args){
    char *name = (char*)malloc(20);
    strcpy(name, (char*)args);
    LOG_I("%s thread started on core %d...", name, xPortGetCoreID());
    free(name);

    NMIoTAPIClass *api= new NMIoTAPIClass();
    if(!api){
        LOG_E("Failed to create NMIoTAPIClass instance");
        vTaskDelete(NULL);
        return;
    }
    while(true){
       String ret = api->get_crypto_rank_price(1, 2, "USDT");
       LOG_I("Crypto rank price response: %s", ret.c_str());

        delay(1000*10);
    }
    vTaskDelete(NULL);
}



