#include "market.h"
#include "logger.h"
#include <ArduinoJson.h>
#include "global.h"

#define MARKET_TIMEOUT (1000*15)

static bool market_exit = false;

static void onWebSocketEvent(WStype_t type, uint8_t* payload, size_t length) {
    if(g_nm.market == NULL) return;
    static uint8_t err_cnt = 0;
    switch (type) {
        case WStype_DISCONNECTED:
            if(++err_cnt % 3 == 0){
                g_nm.market->updated = false;
                g_nm.market->istimeout = true;
                market_exit = true;
            }
            LOG_W("Market Disconnected");
            break;
        case WStype_CONNECTED:
            LOG_I("Market Connected");
            g_nm.market->subscribe();
            break;
        case WStype_TEXT:{
                // {
                //     "time": 1606291803,
                //     "channel": "spot.tickers",
                //     "event": "update",
                //     "result": {
                //         "currency_pair": "BTC_USDT",
                //         "last": "19106.55",
                //         "lowest_ask": "19108.71",
                //         "highest_bid": "19106.55",
                //         "change_percentage": "3.66",
                //         "base_volume": "2811.3042155865",
                //         "quote_volume": "53441606.52411221454674732293",
                //         "high_24h": "19417.74",
                //         "low_24h": "18434.21"
                //     }
                // }
                StaticJsonDocument<512> doc;
                DeserializationError error = deserializeJson(doc, payload, length);
                if (error == DeserializationError::Ok) {
                    if(doc.containsKey("result")){
                        if(doc["result"].containsKey("last")){
                            g_nm.market->updated = true;
                            g_nm.market->price          = doc["result"]["last"].as<float>();
                            g_nm.market->change_percent = doc["result"]["change_percentage"].as<float>();
                            err_cnt = 0;
                        }
                    }
                    // LOG_I("Market: %s", doc.as<String>().c_str());
                } else {
                    LOG_W("Failed to parse market JSON");
                }
            }
            break;
        case WStype_BIN:
            break;
        case WStype_PING:
            LOG_I("Market PING");
            break;
        case WStype_PONG:
            LOG_I("Market PONG");
            break;
        default:
            break;
    }
}

MarketClass::MarketClass(String host, uint16_t port, String url, String symbol) {
    this->istimeout = false;
    this->updated = false;
    this->_host = host;
    this->_port = port;
    this->_url = url;
    this->timeout = MARKET_TIMEOUT;
    this->_wsclient = new WebSocketsClient();

    StaticJsonDocument<256> doc;
    doc["time"] = millis();
    doc["channel"] = "spot.tickers";
    doc["event"]   = "subscribe";
    JsonArray payload = doc.createNestedArray("payload");
    payload.add(symbol);
    serializeJson(doc, this->_subscribeMessage);
}

MarketClass::~MarketClass(){
    delete this->_wsclient;
}

bool MarketClass::connect(){
    if(this->_wsclient == NULL) return false;
    this->_wsclient->onEvent(onWebSocketEvent);
    this->_wsclient->setReconnectInterval(2000);
    this->_wsclient->beginSSL(this->_host, this->_port, this->_url);
    return true;
}

bool MarketClass::subscribe(){
    return this->_wsclient->sendTXT(this->_subscribeMessage);
}

bool MarketClass::isconnect(){
    return this->_wsclient->isConnected();
}

void MarketClass::loop(){
    this->_wsclient->loop();
}

void market_thread_entry(void *args){
    char *name = (char*)malloc(20);
    strcpy(name, (char*)args);
    LOG_I("%s thread started on core %d...", name, xPortGetCoreID());
    // free(name);

    while (g_nm.market == NULL){
        LOG_W("MarketClass instance is NULL, waiting...");
        delay(1000);
    }

    g_nm.market->connect();
       
    while(true){
        delay(50);

        if(market_exit) break;

        if(g_nm.connection.wifi.status_param.status != WL_CONNECTED) continue;
        
        g_nm.market->loop();

        if(!g_nm.market->updated){
            static uint16_t start = millis();
            g_nm.market->timeout   = MARKET_TIMEOUT - (millis() - start);
            g_nm.market->istimeout = (g_nm.market->timeout <= 0) ? true : false;
        }
    }

    g_nm.market->~MarketClass();
    LOG_W("Market thread exit.");
    vTaskDelete(NULL);
}



