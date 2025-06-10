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

// {"data":[{"circulating_supply":19876831,"cmc_rank":1,"date_added":"2010-07-13T00:00:00.000Z","id":1,"infinite_supply":false,"last_updated":"2025-06-10T07:59:00.000Z","max_supply":21000000,"name":"Bitcoin","num_market_pairs":12211,"platform":null,"quote":{"USDT":{"fully_diluted_market_cap":2293873198252.393,"last_updated":"2025-06-10T07:58:00.000Z","market_cap":2171187137956.7773,"market_cap_dominance":63.6417,"percent_change_1h":-0.25424626,"percent_change_24h":3.43496009,"percent_change_30d":5.68097093,"percent_change_60d":34.5334089,"percent_change_7d":3.83807176,"percent_change_90d":32.56131259,"price":109232.0570596378,"tvl":null,"volume_24h":60217736958.08709,"volume_change_24h":50.4439}},"self_reported_circulating_supply":null,"self_reported_market_cap":null,"slug":"bitcoin","symbol":"BTC","tags":["mineable","pow","sha-256","store-of-value","state-channel","coinbase-ventures-portfolio","three-arrows-capital-portfolio","polychain-capital-portfolio","binance-labs-portfolio","blockchain-capital-portfolio","boostvc-portfolio","cms-holdings-portfolio","dcg-portfolio","dragonfly-capital-portfolio","electric-capital-portfolio","fabric-ventures-portfolio","framework-ventures-portfolio","galaxy-digital-portfolio","huobi-capital-portfolio","alameda-research-portfolio","a16z-portfolio","1confirmation-portfolio","winklevoss-capital-portfolio","usv-portfolio","placeholder-ventures-portfolio","pantera-capital-portfolio","multicoin-capital-portfolio","paradigm-portfolio","bitcoin-ecosystem","layer-1","ftx-bankruptcy-estate","2017-2018-alt-season","us-strategic-crypto-reserve","binance-ecosystem"],"total_supply":19876831,"tvl_ratio":null},{"circulating_supply":120722642.63995315,"cmc_rank":2,"date_added":"2015-08-07T00:00:00.000Z","id":1027,"infinite_supply":true,"last_updated":"2025-06-10T07:59:00.000Z","max_supply":null,"name":"Ethereum","num_market_pairs":10336,"platform":null,"quote":{"USDT":{"fully_diluted_market_cap":323173614738.81384,"last_updated":"2025-06-10T07:58:00.000Z","market_cap":323173614738.8187,"market_cap_dominance":9.4728,"percent_change_1h":0.0619422,"percent_change_24h":7.4125775,"percent_change_30d":8.25512507,"percent_change_60d":72.52976721,"percent_change_7d":2.55821184,"percent_change_90d":40.07112962,"price":2676.9925481391374,"tvl":null,"volume_24h":26891976777.97495,"volume_change_24h":98.3791}},"self_reported_circulating_supply":null,"self_reported_market_cap":null,"slug":"ethereum","symbol":"ETH","tags":["pos","smart-contracts","ethereum-ecosystem","coinbase-ventures-portfolio","three-arrows-capital-portfolio","polychain-capital-portfolio","binance-labs-portfolio","blockchain-capital-portfolio","boostvc-portfolio","cms-holdings-portfolio","dcg-portfolio","dragonfly-capital-portfolio","electric-capital-portfolio","fabric-ventures-portfolio","framework-ventures-portfolio","hashkey-capital-portfolio","kenetic-capital-portfolio","huobi-capital-portfolio","alameda-research-portfolio","a16z-portfolio","1confirmation-portfolio","winklevoss-capital-portfolio","usv-portfolio","placeholder-ventures-portfolio","pantera-capital-portfolio","multicoin-capital-portfolio","paradigm-portfolio","ethereum-pow-ecosystem","layer-1","ftx-bankruptcy-estate","sora-ecosystem","rsk-rbtc-ecosystem","world-liberty-financial-portfolio","us-strategic-crypto-reserve","binance-ecosystem"],"total_supply":120722642.63995315,"tvl_ratio":null}],"status":{"credit_count":1,"elapsed":51,"error_code":0,"error_message":null,"notice":null,"timestamp":"2025-06-10T08:00:36.782Z","total_count":9695}}
String NMIoTAPIClass::get_crypto_rank_price(int start, int limit, const String &convert){
    String url = "/crypto/price?start=" + String(start) + "&limit=" + String(limit) + "&convert=" + convert;
    LOG_D("Requesting: %s", url.c_str());

    int httpCode = this->_httpclient->get(url);
    if (httpCode != 0) {
        LOG_E("HTTP GET failed, code: %d", httpCode);
        return "";
    }

    int status = this->_httpclient->responseStatusCode();
    if (status != 200) {
        LOG_E("HTTP response status: %d", status);
        return "";
    }

    String response = this->_httpclient->responseBody();
    return response;
}


String NMIoTAPIClass::get_crypto_info(const String &slug){
    String formatted_slug = slug;
    formatted_slug.replace(" ", ""); // Replace spaces with hyphens for slug formatting
    String url = "/crypto/info?slug=" + formatted_slug;
    LOG_D("Requesting: %s", url.c_str());

    int httpCode = this->_httpclient->get(url);
    if (httpCode != 0) {
        LOG_E("HTTP GET failed, code: %d", httpCode);
        return "";
    }

    int status = this->_httpclient->responseStatusCode();
    if (status != 200) {
        LOG_E("HTTP response status: %d", status);
        return "";
    }

    String response = this->_httpclient->responseBody();
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

    uint8_t   fail_count = 0;
    uint32_t  thread_cnt = 0;
    while(true){
        DynamicJsonDocument doc    = DynamicJsonDocument(1024 * 32);
        DeserializationError error = DeserializationError::Ok;
        String json = "";
        delay(1000);

        // Check if WiFi is connected or not
        if (WiFi.status() != WL_CONNECTED) {
            LOG_E("WiFi still not connected, wait and retry...");
            continue;
        }

        // update crypto rank price data every 60s
        if(thread_cnt % 60 == 0) {
            json = api->get_crypto_rank_price(1, 20, "USDT");
            if(json.isEmpty()) {
                delete api;
                api = new NMIoTAPIClass();
                LOG_E("Failed to get crypto rank price data");
                continue;
            }

            // Deserialize the JSON response
            doc.clear();
            error = deserializeJson(doc, json);
            if (error) {
                LOG_E("deserializeJson() failed: %s", error.c_str());
                continue;
            }
        
            LOG_W("| %6s | %-12s | %4s | %10s | %15s  | %6s  | %6s  |   %15s        | %s ", "ID", "Slug", "Rank", "Price(USD)", "MarketCap(USD)", "1h ", "1d ", "Last Updated", "Logo URL");
            for (JsonObject coin : doc["data"].as<JsonArray>()) {
                const char* name           = coin.containsKey("name") ? coin["name"].as<const char*>() : "unknown";
                JsonObject quote           = coin["quote"].containsKey("USDT") ? coin["quote"]["USDT"] : JsonObject();

                g_nm.coin_map[name].price.realtime = quote.containsKey("price") ? quote["price"].as<double>() : 0.0;
                g_nm.coin_map[name].rank           = coin.containsKey("cmc_rank") ? coin["cmc_rank"].as<int>() : -1;
                g_nm.coin_map[name].name           = name;
                g_nm.coin_map[name].id             = coin.containsKey("id") ? coin["id"].as<int>() : -1;
                g_nm.coin_map[name].symbol         = coin.containsKey("symbol") ? coin["symbol"].as<const char*>() : "unknown";
                g_nm.coin_map[name].slug           = coin.containsKey("slug") ? coin["slug"].as<const char*>() : "unknown";
                g_nm.coin_map[name].price.volume_24h = quote.containsKey("volume_24h") ? quote["volume_24h"].as<double>() : 0.0;
                g_nm.coin_map[name].price.volume_change_24h  = quote.containsKey("volume_change_24h") ? quote["volume_change_24h"].as<double>() : 0.0;
                g_nm.coin_map[name].price.market_cap         = quote.containsKey("market_cap") ? quote["market_cap"].as<double>() : 0.0;
                g_nm.coin_map[name].price.percent_change_1h  = quote.containsKey("percent_change_1h") ? quote["percent_change_1h"].as<double>() : 0.0;
                g_nm.coin_map[name].price.percent_change_24h = quote.containsKey("percent_change_24h") ? quote["percent_change_24h"].as<double>() : 0.0;
                g_nm.coin_map[name].price.percent_change_7d  = quote.containsKey("percent_change_7d") ? quote["percent_change_7d"].as<double>() : 0.0;
                g_nm.coin_map[name].price.percent_change_30d = quote.containsKey("percent_change_30d") ? quote["percent_change_30d"].as<double>() : 0.0;
                g_nm.coin_map[name].price.percent_change_60d = quote.containsKey("percent_change_60d") ? quote["percent_change_60d"].as<double>() : 0.0;
                g_nm.coin_map[name].price.percent_change_90d = quote.containsKey("percent_change_90d") ? quote["percent_change_90d"].as<double>() : 0.0;
                g_nm.coin_map[name].price.last_updated       = coin.containsKey("last_updated") ? coin["last_updated"].as<const char*>() : "unknown";

                LOG_I("| %6d | %-12s | %4d | %10.2f | %16.1f | %6.2f%% | %6.2f%% | %15s | %s ", 
                    g_nm.coin_map[name].id,
                    g_nm.coin_map[name].slug,
                    g_nm.coin_map[name].rank,
                    g_nm.coin_map[name].price.realtime,
                    g_nm.coin_map[name].price.market_cap,
                    g_nm.coin_map[name].price.percent_change_1h,
                    g_nm.coin_map[name].price.percent_change_24h,
                    g_nm.coin_map[name].price.last_updated.c_str(),
                    g_nm.coin_map[name].logo_url.c_str()
                );
            }
            doc.clear();

            /********************update crypto logo URLs*********************/
            for(auto &coin : g_nm.coin_map) {
                delay(100); // Add a small delay to avoid overwhelming the API
                if(coin.second.logo_url != "") {
                    LOG_D("Coin: %s already has logo URL: %s", coin.second.name.c_str(), coin.second.logo_url.c_str());
                    continue; // Skip if logo URL is already set
                }

                // Fetch logo URL from the API
                json = api->get_crypto_info(coin.second.slug);
                if(json.isEmpty()) {
                    delete api;
                    api = new NMIoTAPIClass();
                    LOG_E("Failed to get crypto rank price data");
                    continue;
                }
                doc.clear();
                error = deserializeJson(doc, json);
                if (error) {
                    LOG_E("deserializeJson() failed: %s", error.c_str());
                    continue;
                }
                // Process the JSON response to extract logo URLs
                for (JsonPair kv : doc["data"].as<JsonObject>()) {
                    JsonObject info     = kv.value().as<JsonObject>();
                    String    logo_url  = info.containsKey("logo") ? info["logo"].as<const char*>() : "";

                    auto it = g_nm.coin_map.find(coin.second.name);
                    if (it != g_nm.coin_map.end()) {
                        it->second.logo_url = logo_url;
                    } else {
                        LOG_W("coin_map not found for name: %s", coin.second.name.c_str());
                    }
                }
            }
        }
        // update crypto rank info data every 60s
        if((thread_cnt + 10) % (60*1) == 0) {

        }
        //update weather forecast data every 15m
        if((thread_cnt + 20) % (60*15) == 0) {
            LOG_D("Fetching weather forecast data...");
        }
        thread_cnt++;
    }
    vTaskDelete(NULL);
}



