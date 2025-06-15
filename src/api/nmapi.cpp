#include "nmapi.h"
#include "logger.h"
#include <ArduinoJson.h>
#include "global.h"
#include "helper.h"
// #include "city_map.h"

#define NMAPI_HOST "api.nmiot.net" // The host for the NMIoT API
#define NMAPI_PORT 443             // The port for the NMIoT API, usually 443 for HTTPS

// {"data":[{"circulating_supply":19876831,"cmc_rank":1,"date_added":"2010-07-13T00:00:00.000Z","id":1,"infinite_supply":false,"last_updated":"2025-06-10T07:59:00.000Z","max_supply":21000000,"name":"Bitcoin","num_market_pairs":12211,"platform":null,"quote":{"USDT":{"fully_diluted_market_cap":2293873198252.393,"last_updated":"2025-06-10T07:58:00.000Z","market_cap":2171187137956.7773,"market_cap_dominance":63.6417,"percent_change_1h":-0.25424626,"percent_change_24h":3.43496009,"percent_change_30d":5.68097093,"percent_change_60d":34.5334089,"percent_change_7d":3.83807176,"percent_change_90d":32.56131259,"price":109232.0570596378,"tvl":null,"volume_24h":60217736958.08709,"volume_change_24h":50.4439}},"self_reported_circulating_supply":null,"self_reported_market_cap":null,"slug":"bitcoin","symbol":"BTC","tags":["mineable","pow","sha-256","store-of-value","state-channel","coinbase-ventures-portfolio","three-arrows-capital-portfolio","polychain-capital-portfolio","binance-labs-portfolio","blockchain-capital-portfolio","boostvc-portfolio","cms-holdings-portfolio","dcg-portfolio","dragonfly-capital-portfolio","electric-capital-portfolio","fabric-ventures-portfolio","framework-ventures-portfolio","galaxy-digital-portfolio","huobi-capital-portfolio","alameda-research-portfolio","a16z-portfolio","1confirmation-portfolio","winklevoss-capital-portfolio","usv-portfolio","placeholder-ventures-portfolio","pantera-capital-portfolio","multicoin-capital-portfolio","paradigm-portfolio","bitcoin-ecosystem","layer-1","ftx-bankruptcy-estate","2017-2018-alt-season","us-strategic-crypto-reserve","binance-ecosystem"],"total_supply":19876831,"tvl_ratio":null},{"circulating_supply":120722642.63995315,"cmc_rank":2,"date_added":"2015-08-07T00:00:00.000Z","id":1027,"infinite_supply":true,"last_updated":"2025-06-10T07:59:00.000Z","max_supply":null,"name":"Ethereum","num_market_pairs":10336,"platform":null,"quote":{"USDT":{"fully_diluted_market_cap":323173614738.81384,"last_updated":"2025-06-10T07:58:00.000Z","market_cap":323173614738.8187,"market_cap_dominance":9.4728,"percent_change_1h":0.0619422,"percent_change_24h":7.4125775,"percent_change_30d":8.25512507,"percent_change_60d":72.52976721,"percent_change_7d":2.55821184,"percent_change_90d":40.07112962,"price":2676.9925481391374,"tvl":null,"volume_24h":26891976777.97495,"volume_change_24h":98.3791}},"self_reported_circulating_supply":null,"self_reported_market_cap":null,"slug":"ethereum","symbol":"ETH","tags":["pos","smart-contracts","ethereum-ecosystem","coinbase-ventures-portfolio","three-arrows-capital-portfolio","polychain-capital-portfolio","binance-labs-portfolio","blockchain-capital-portfolio","boostvc-portfolio","cms-holdings-portfolio","dcg-portfolio","dragonfly-capital-portfolio","electric-capital-portfolio","fabric-ventures-portfolio","framework-ventures-portfolio","hashkey-capital-portfolio","kenetic-capital-portfolio","huobi-capital-portfolio","alameda-research-portfolio","a16z-portfolio","1confirmation-portfolio","winklevoss-capital-portfolio","usv-portfolio","placeholder-ventures-portfolio","pantera-capital-portfolio","multicoin-capital-portfolio","paradigm-portfolio","ethereum-pow-ecosystem","layer-1","ftx-bankruptcy-estate","sora-ecosystem","rsk-rbtc-ecosystem","world-liberty-financial-portfolio","us-strategic-crypto-reserve","binance-ecosystem"],"total_supply":120722642.63995315,"tvl_ratio":null}],"status":{"credit_count":1,"elapsed":51,"error_code":0,"error_message":null,"notice":null,"timestamp":"2025-06-10T08:00:36.782Z","total_count":9695}}
String NMIoTAPIClass::get_crypto_rank_price(int start, int limit, const String &convert){
    String url = "/v1/crypto/price_rank?start=" + String(start) + "&limit=" + String(limit) + "&convert=" + convert;
    LOG_D("Requesting: %s", url.c_str());
    WiFiClientSecure    sslwifi;
    sslwifi.setInsecure(); // Disable SSL certificate verification for testing purposes
    HttpClient httpclient(sslwifi, NMAPI_HOST, NMAPI_PORT);

    int httpCode = httpclient.get(url);
    if (httpCode != 0) {
        LOG_E("HTTP GET failed, code: %d", httpCode);
        return "";
    }

    int status = httpclient.responseStatusCode();
    if (status != 200) {
        LOG_E("HTTP response status: %d", status);
        return "";
    }

    String response = httpclient.responseBody();
    return response;
}

String NMIoTAPIClass::get_weather_realtime(const double &lat, const double &lon){
    String url = "/v1/weather/realtime?lat=" + String(lat, 6) + "&lon=" + String(lon, 6);
    LOG_D("Requesting: %s", url.c_str());
    WiFiClientSecure    sslwifi;
    sslwifi.setInsecure(); // Disable SSL certificate verification for testing purposes
    HttpClient httpclient(sslwifi, NMAPI_HOST, NMAPI_PORT);

    int httpCode = httpclient.get(url);
    if (httpCode != 0) {
        LOG_E("HTTP GET failed, code: %d", httpCode);
        return "";
    }

    int status = httpclient.responseStatusCode();
    if (status != 200) {
        LOG_E("HTTP response status: %d", status);
        return "";
    }

    String response = httpclient.responseBody();
    return response;
}

String NMIoTAPIClass::get_weather_forecast(const double &lat, const double &lon, const size_t &cnt){
    String url = "/v1/weather/forecast?lat=" + String(lat, 6) + "&lon=" + String(lon, 6) + "&cnt=" + String(cnt);
    LOG_D("Requesting: %s", url.c_str());
    WiFiClientSecure    sslwifi;
    sslwifi.setInsecure(); // Disable SSL certificate verification for testing purposes
    HttpClient httpclient(sslwifi, NMAPI_HOST, NMAPI_PORT);

    int httpCode = httpclient.get(url);
    if (httpCode != 0) {
        LOG_E("HTTP GET failed, code: %d", httpCode);
        return "";
    }

    int status = httpclient.responseStatusCode();
    if (status != 200) {
        LOG_E("HTTP response status: %d", status);
        return "";
    }

    String response = httpclient.responseBody();
    return response;
}

String NMIoTAPIClass::get_air_pollution(const double &lat, const double &lon){
    String url = "/v1/weather/air_pollution?lat=" + String(lat, 6) + "&lon=" + String(lon, 6);
    LOG_D("Requesting: %s", url.c_str());
    WiFiClientSecure    sslwifi;
    sslwifi.setInsecure(); // Disable SSL certificate verification for testing purposes
    HttpClient httpclient(sslwifi, NMAPI_HOST, NMAPI_PORT);

    int httpCode = httpclient.get(url);
    if (httpCode != 0) {
        LOG_E("HTTP GET failed, code: %d", httpCode);
        return "";
    }

    int status = httpclient.responseStatusCode();
    if (status != 200) {
        LOG_E("HTTP response status: %d", status);
        return "";
    }

    String response = httpclient.responseBody();
    return response;
}

size_t NMIoTAPIClass::download_coin_icon(int coin_id, uint8_t *buf, size_t buf_size) {
    if (!buf) {
        LOG_E("icon buffer is null");
        return 0;
    }
    String url = "/static/img/coins/32x32/" + String(coin_id) + ".png";
    WiFiClientSecure sslclient;
    sslclient.setInsecure();
    HttpClient http(sslclient, "s2.coinmarketcap.com", 443);

    http.sendHeader("User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36");

    int httpCode = http.get(url.c_str());
    if (httpCode != HTTP_SUCCESS) {
        LOG_E("HTTP GET failed, code: %d", httpCode);
        memset(buf, 0, buf_size);
        return 0;
    }

    size_t size = http.contentLength();
    if (size <= 0) {
        LOG_E("Invalid content length: %d", size);
        http.endRequest();
        memset(buf, 0, buf_size);
        return 0;
    }

    if (size > buf_size) {
        LOG_E("Buffer size (%d) is smaller than content length (%d)", buf_size, size);
        http.endRequest();
        memset(buf, 0, buf_size);
        return 0;
    }

    int s = http.readBytes(buf, size);
    if (s != size) {
        LOG_W("Downloaded %d/%d bytes from %s", s, size, url.c_str());
        return 0;
    } else {
        // LOG_I("Downloaded %d bytes from %s", s, url.c_str());
    }

    http.endRequest();
    return s; 
}

size_t NMIoTAPIClass::download_weather_icon(const String &icon_id, uint8_t *buf, size_t buf_size) {
    if (!buf) {
        LOG_E("icon buffer is null");
        return 0;
    }
    String url = "/img/wn/" + icon_id + ".png";
    WiFiClient client;
    HttpClient http(client, "openweathermap.org", 80);

    http.sendHeader("User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36");

    int httpCode = http.get(url.c_str());
    if (httpCode != HTTP_SUCCESS) {
        LOG_E("HTTP GET failed, code: %d", httpCode);
        memset(buf, 0, buf_size);
        return 0;
    }
    size_t size = http.contentLength();
    if (size <= 0) {
        LOG_E("Invalid content length: %d", size);
        http.endRequest();
        memset(buf, 0, buf_size);
        return 0;
    }
    if (size > buf_size) {
        LOG_E("Buffer size (%d) is smaller than content length (%d)", buf_size, size);
        http.endRequest();
        memset(buf, 0, buf_size);
        return 0;
    }
    int s = http.readBytes(buf, size);
    if (s != size) {
        LOG_W("Downloaded %d/%d bytes from %s", s, size, url.c_str());
        return 0;
    } else {
        LOG_D("Downloaded %d bytes from %s", s, url.c_str());
    }
    http.endRequest();
    return s;
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
    uint32_t  max_json_size = 0; // Maximum JSON size recorded
    while(true){
        DynamicJsonDocument doc    = DynamicJsonDocument(1024 * 10);
        DeserializationError error = DeserializationError::Ok;
        String json = "";
        delay(1000);

        // Check if WiFi is connected or not
        if (WiFi.status() != WL_CONNECTED) {
            LOG_E("WiFi still not connected, wait and retry...");
            continue;
        }

        /***********************************update crypto rank price data************************************/
        if(xSemaphoreTake(g_nm.global_xsem.coin_price_xsem, 0) == pdTRUE) {
            json = api->get_crypto_rank_price(1, 7, "USDT");
            max_json_size = max(max_json_size, json.length());
            // LOG_W("%s , size of json: %d", json.c_str(), json.length());
            if(json.isEmpty()) {
                LOG_E("Failed to get crypto rank price data");
                thread_cnt++;
                continue;
            }

            // Deserialize the JSON response
            doc.clear();
            error = deserializeJson(doc, json);
            if (error) {
                LOG_E("deserializeJson() failed: %s", error.c_str());
                thread_cnt++;
                continue;
            }
        
            LOG_W("| %6s | %-12s | %4s | %10s | %15s  | %6s  | %6s  |   %15s        |", "ID", "Slug", "Rank", "Price(USD)", "MarketCap(USD)", "1h ", "1d ", "Last Updated");
            for (JsonObject coin : doc["data"].as<JsonArray>()) {
                const char* name           = coin.containsKey("name") ? coin["name"].as<const char*>() : "unknown";
                JsonObject quote           = coin["quote"].containsKey("USDT") ? coin["quote"]["USDT"] : JsonObject();

                g_nm.coin_price_rank[name].price.realtime = quote.containsKey("price") ? quote["price"].as<double>() : 0.0;
                g_nm.coin_price_rank[name].rank           = coin.containsKey("cmc_rank") ? coin["cmc_rank"].as<int>() : -1;
                g_nm.coin_price_rank[name].name           = name;
                g_nm.coin_price_rank[name].id             = coin.containsKey("id") ? coin["id"].as<int>() : -1;
                g_nm.coin_price_rank[name].symbol         = coin.containsKey("symbol") ? coin["symbol"].as<const char*>() : "unknown";
                g_nm.coin_price_rank[name].slug           = coin.containsKey("slug") ? coin["slug"].as<const char*>() : "unknown";
                g_nm.coin_price_rank[name].price.volume_24h = quote.containsKey("volume_24h") ? quote["volume_24h"].as<double>() : 0.0;
                g_nm.coin_price_rank[name].price.volume_change_24h  = quote.containsKey("volume_change_24h") ? quote["volume_change_24h"].as<double>() : 0.0;
                g_nm.coin_price_rank[name].price.market_cap         = quote.containsKey("market_cap") ? quote["market_cap"].as<double>() : 0.0;
                g_nm.coin_price_rank[name].price.percent_change_1h  = quote.containsKey("percent_change_1h") ? quote["percent_change_1h"].as<double>() : 0.0;
                g_nm.coin_price_rank[name].price.percent_change_24h = quote.containsKey("percent_change_24h") ? quote["percent_change_24h"].as<double>() : 0.0;
                g_nm.coin_price_rank[name].price.percent_change_7d  = quote.containsKey("percent_change_7d") ? quote["percent_change_7d"].as<double>() : 0.0;
                g_nm.coin_price_rank[name].price.percent_change_30d = quote.containsKey("percent_change_30d") ? quote["percent_change_30d"].as<double>() : 0.0;
                g_nm.coin_price_rank[name].price.percent_change_60d = quote.containsKey("percent_change_60d") ? quote["percent_change_60d"].as<double>() : 0.0;
                g_nm.coin_price_rank[name].price.percent_change_90d = quote.containsKey("percent_change_90d") ? quote["percent_change_90d"].as<double>() : 0.0;
                g_nm.coin_price_rank[name].price.last_updated       = coin.containsKey("last_updated") ? coin["last_updated"].as<const char*>() : "unknown";

                LOG_I("| %6d | %-12s | %4d | %10.2f | %16.1f | %6.2f%% | %6.2f%% | %15s |",
                    g_nm.coin_price_rank[name].id,
                    g_nm.coin_price_rank[name].slug,
                    g_nm.coin_price_rank[name].rank,
                    g_nm.coin_price_rank[name].price.realtime,
                    g_nm.coin_price_rank[name].price.market_cap,
                    g_nm.coin_price_rank[name].price.percent_change_1h,
                    g_nm.coin_price_rank[name].price.percent_change_24h,
                    g_nm.coin_price_rank[name].price.last_updated.c_str()
                );
            }
            doc.clear();

            size_t png_max = 1024 * 2;
            for(auto &coin : g_nm.coin_price_rank) {
                if(coin.second.icon.addr != NULL) continue; // Skip if the icon already exists

                uint8_t* buf             = (uint8_t*)malloc(png_max); // Allocate a buffer for the icon
                coin.second.icon.size    = NMIoTAPIClass::download_coin_icon(coin.second.id, buf, png_max); // Download the icon for the coin
                coin.second.icon.updated = (coin.second.icon.size > 0); // Set the flag to true if the icon was downloaded successfully
                coin.second.icon.addr    = (uint8_t*)malloc(coin.second.icon.size); // Allocate memory for the icon address
                memcpy(coin.second.icon.addr, buf, coin.second.icon.size); // Copy the downloaded icon to the address
                free(buf); // Free the temporary buffer after copying
            }
        }
        /************************************update weather realtime data************************************/
        if(xSemaphoreTake(g_nm.global_xsem.weather_realtime_xsem, 0) == pdTRUE) {
            // https://openweathermap.org/img/wn/04d.png
            // https://openweathermap.org/img/wn/03d@2x.png
            // double lat = 30.5728, lon = 104.0668; // Default coordinates for testing
            json = api->get_weather_realtime(g_nm.location.coord.lat, g_nm.location.coord.lon);
            max_json_size = max(max_json_size, json.length());
            LOG_W("%s", json.c_str());
            if(json.isEmpty()) {
                LOG_E("Failed to get weather realtime data");
                thread_cnt++;
                continue;
            }

            // Deserialize the JSON response
            doc.clear();
            error = deserializeJson(doc, json);
            if (error) {
                LOG_E("deserializeJson() failed: %s", error.c_str());
                thread_cnt++;
                continue;
            }

            // Extract the weather data
            JsonObject weather = doc.as<JsonObject>();
            if (!weather.isNull()) {
                g_nm.weather_realtime.base = weather.containsKey("base") ? weather["base"].as<String>() : "";
                g_nm.weather_realtime.coord.lat = weather.containsKey("coord") && weather["coord"].containsKey("lat") ? weather["coord"]["lat"].as<double>() : 0.0;
                g_nm.weather_realtime.coord.lon = weather.containsKey("coord") && weather["coord"].containsKey("lon") ? weather["coord"]["lon"].as<double>() : 0.0;
                g_nm.weather_realtime.dt = weather.containsKey("dt") ? weather["dt"].as<uint32_t>() : 0;
                g_nm.weather_realtime.id = weather.containsKey("id") ? weather["id"].as<uint32_t>() : 0;
                g_nm.weather_realtime.name = weather.containsKey("name") ? weather["name"].as<String>() : "";
                g_nm.weather_realtime.sys.country = weather.containsKey("sys") && weather["sys"].containsKey("country") ? weather["sys"]["country"].as<String>() : "";
                g_nm.weather_realtime.sys.sunrise = weather.containsKey("sys") && weather["sys"].containsKey("sunrise") ? weather["sys"]["sunrise"].as<uint32_t>() : 0;
                g_nm.weather_realtime.sys.sunset = weather.containsKey("sys") && weather["sys"].containsKey("sunset") ? weather["sys"]["sunset"].as<uint32_t>() : 0;
                g_nm.weather_realtime.visibility = weather.containsKey("visibility") ? weather["visibility"].as<uint32_t>() : 0;
                g_nm.weather_realtime.main.feels_like = weather.containsKey("main") && weather["main"].containsKey("feels_like") ? weather["main"]["feels_like"].as<double>() : 0.0;
                g_nm.weather_realtime.main.grnd_level = weather.containsKey("main") && weather["main"].containsKey("grnd_level") ? weather["main"]["grnd_level"].as<double>() : 0.0;
                g_nm.weather_realtime.main.sea_level = weather.containsKey("main") && weather["main"].containsKey("sea_level") ? weather["main"]["sea_level"].as<double>() : 0.0;
                g_nm.weather_realtime.main.temp = weather.containsKey("main") && weather["main"].containsKey("temp") ? weather["main"]["temp"].as<double>() : 0.0;
                g_nm.weather_realtime.main.temp_min = weather.containsKey("main") && weather["main"].containsKey("temp_min") ? weather["main"]["temp_min"].as<double>() : 0.0;
                g_nm.weather_realtime.main.temp_max = weather.containsKey("main") && weather["main"].containsKey("temp_max") ? weather["main"]["temp_max"].as<double>() : 0.0;
                g_nm.weather_realtime.main.pressure = weather.containsKey("main") && weather["main"].containsKey("pressure") ? weather["main"]["pressure"].as<double>() : 0.0;
                g_nm.weather_realtime.main.humidity = weather.containsKey("main") && weather["main"].containsKey("humidity") ? weather["main"]["humidity"].as<double>() : 0.0;
                g_nm.weather_realtime.wind.speed = weather.containsKey("wind") && weather["wind"].containsKey("speed") ? weather["wind"]["speed"].as<float>() : 0;
                g_nm.weather_realtime.wind.deg = weather.containsKey("wind") && weather["wind"].containsKey("deg") ? weather["wind"]["deg"].as<float>() : 0;
                g_nm.weather_realtime.timezone = weather.containsKey("timezone") ? weather["timezone"].as<int>() : 0;
                g_nm.weather_realtime.cod = weather.containsKey("cod") ? weather["cod"].as<int>() : 0;
                if (weather.containsKey("weather") && weather["weather"].is<JsonArray>()) {
                    JsonArray weatherArr = weather["weather"].as<JsonArray>();
                    g_nm.weather_realtime.weather.clear();
                    for (JsonObject w : weatherArr) {
                        weather_info_t info;
                        info.id = w.containsKey("id") ? w["id"].as<uint32_t>() : 0;
                        info.main = w.containsKey("main") ? w["main"].as<String>() : "";
                        info.description = w.containsKey("description") ? w["description"].as<String>() : "";
                        info.icon.id = w.containsKey("icon") ? w["icon"].as<String>() : "";
                        g_nm.weather_realtime.weather.push_back(info);
                    }
                }

 
                LOG_W("Weather in %s (%s): %.2f'C, %s, %s, Humidity: %.1f%%, Wind: %.1f m/s, Icon: %s",
                    g_nm.weather_realtime.name.c_str(),
                    g_nm.weather_realtime.sys.country.c_str(),
                    g_nm.weather_realtime.main.temp,
                    g_nm.weather_realtime.weather[0].description.c_str(),
                    g_nm.weather_realtime.weather[0].main.c_str(),
                    g_nm.weather_realtime.main.humidity,
                    g_nm.weather_realtime.wind.speed,
                    g_nm.weather_realtime.weather[0].icon.id.c_str()
                );



            size_t png_max = 1024 * 2;

            if(g_nm.weather_realtime.weather[0].icon.addr != NULL) continue; // Skip if the icon already exists
            uint8_t* buf   = (uint8_t*)malloc(png_max); // Allocate a buffer for the icon
            String icon_id = g_nm.weather_realtime.weather[0].icon.id; // Get the icon ID

            g_nm.weather_realtime.weather[0].icon.size    = NMIoTAPIClass::download_weather_icon(icon_id, buf, png_max); // Download the icon for the weather
            g_nm.weather_realtime.weather[0].icon.addr    = (uint8_t*)malloc(g_nm.weather_realtime.weather[0].icon.size); // Allocate memory for the icon address
            memcpy(g_nm.weather_realtime.weather[0].icon.addr, buf, g_nm.weather_realtime.weather[0].icon.size); // Copy the downloaded icon to the address
            free(buf); // Free the temporary buffer after copying
            g_nm.weather_realtime.weather[0].icon.updated = (g_nm.weather_realtime.weather[0].icon.size > 0); // Set the flag to true if the icon was downloaded successfully
            } else {
                LOG_E("No weather data found");
            }
        }
        /************************************update weather forecast data ***********************************/
        if(xSemaphoreTake(g_nm.global_xsem.weather_forecast_xsem, 0) == pdTRUE) {
            // double lat = 30.5728, lon = 104.0668; // Default coordinates for testing
            json = api->get_weather_forecast(g_nm.location.coord.lat, g_nm.location.coord.lon, 8);
            max_json_size = max(max_json_size, json.length());
            // LOG_W("%s", json.c_str());
            if(json.isEmpty()) {
                LOG_E("Failed to get weather forecast data");
                thread_cnt++;
                continue;
            }

            // Deserialize the JSON response
            doc.clear();
            error = deserializeJson(doc, json);
            if (error) {
                LOG_E("deserializeJson() failed: %s", error.c_str());
                thread_cnt++;
                continue;
            }

            // Extract the forecast data
            JsonObject forecast = doc.as<JsonObject>();
            if (!forecast.isNull()) {
                g_nm.weather_forecast.city.name = forecast.containsKey("city") && forecast["city"].containsKey("name") ? forecast["city"]["name"].as<String>() : "";
                g_nm.weather_forecast.city.coord.lat = forecast.containsKey("city") && forecast["city"].containsKey("coord") && forecast["city"]["coord"].containsKey("lat") ? forecast["city"]["coord"]["lat"].as<double>() : 0.0;
                g_nm.weather_forecast.city.coord.lon = forecast.containsKey("city") && forecast["city"].containsKey("coord") && forecast["city"]["coord"].containsKey("lon") ? forecast["city"]["coord"]["lon"].as<double>() : 0.0;
                g_nm.weather_forecast.city.country = forecast.containsKey("city") && forecast["city"].containsKey("country") ? forecast["city"]["country"].as<String>() : "";
                g_nm.weather_forecast.city.population = forecast.containsKey("city") && forecast["city"].containsKey("population") ? forecast["city"]["population"].as<uint32_t>() : 0;
                g_nm.weather_forecast.city.timezone = forecast.containsKey("city") && forecast["city"].containsKey("timezone") ? forecast["city"]["timezone"].as<int>() : 0;
                g_nm.weather_forecast.city.sunrise = forecast.containsKey("city") && forecast["city"].containsKey("sunrise") ? forecast["city"]["sunrise"].as<uint32_t>() : 0;
                g_nm.weather_forecast.city.sunset = forecast.containsKey("city") && forecast["city"].containsKey("sunset") ? forecast["city"]["sunset"].as<uint32_t>() : 0;
                g_nm.weather_forecast.cnt = forecast.containsKey("cnt") ? forecast["cnt"].as<uint32_t>() : 0;
                g_nm.weather_forecast.cod = forecast.containsKey("cod") ? forecast["cod"].as<String>() : "";
                g_nm.weather_forecast.message = forecast.containsKey("message") ? forecast["message"].as<double>() : 0.0;
                JsonArray list = forecast.containsKey("list") ? forecast["list"].as<JsonArray>() : JsonArray();
                g_nm.weather_forecast.list.clear(); // Clear the previous forecast data
                for (JsonObject item : list) {
                    forecast_node_t forecast_item;
                    forecast_item.dt = item.containsKey("dt") ? item["dt"].as<uint32_t>() : 0;
                    forecast_item.dt_txt = item.containsKey("dt_txt") ? item["dt_txt"].as<String>() : "";
                    forecast_item.clouds.all = item.containsKey("clouds") && item["clouds"].containsKey("all") ? item["clouds"]["all"].as<int>() : 0;
                    forecast_item.main.temp = item.containsKey("main") && item["main"].containsKey("temp") ? item["main"]["temp"].as<double>() : 0.0;
                    forecast_item.main.feels_like = item.containsKey("main") && item["main"].containsKey("feels_like") ? item["main"]["feels_like"].as<double>() : 0.0;
                    forecast_item.main.temp_min = item.containsKey("main") && item["main"].containsKey("temp_min") ? item["main"]["temp_min"].as<double>() : 0.0;
                    forecast_item.main.temp_max = item.containsKey("main") && item["main"].containsKey("temp_max") ? item["main"]["temp_max"].as<double>() : 0.0;
                    forecast_item.main.pressure = item.containsKey("main") && item["main"].containsKey("pressure") ? item["main"]["pressure"].as<double>() : 0.0;
                    forecast_item.main.humidity = item.containsKey("main") && item["main"].containsKey("humidity") ? item["main"]["humidity"].as<double>() : 0.0;
                    forecast_item.weather.id = item.containsKey("weather") && item["weather"].is<JsonArray>() && item["weather"][0].containsKey("id") ? item["weather"][0]["id"].as<uint32_t>() : 0;
                    forecast_item.weather.main = item.containsKey("weather") && item["weather"].is<JsonArray>() && item["weather"][0].containsKey("main") ? item["weather"][0]["main"].as<String>() : "";
                    forecast_item.weather.description = item.containsKey("weather") && item["weather"].is<JsonArray>() && item["weather"][0].containsKey("description") ? item["weather"][0]["description"].as<String>() : "";
                    forecast_item.weather.icon.id = item.containsKey("weather") && item["weather"].is<JsonArray>() && item["weather"][0].containsKey("icon") ? item["weather"][0]["icon"].as<String>() : "";
                    forecast_item.wind.speed = item.containsKey("wind") && item["wind"].containsKey("speed") ? item["wind"]["speed"].as<float>() : 0;
                    forecast_item.wind.deg = item.containsKey("wind") && item["wind"].containsKey("deg") ? item["wind"]["deg"].as<float>() : 0;
                    forecast_item.wind.gust = item.containsKey("wind") && item["wind"].containsKey("gust") ? item["wind"]["gust"].as<float>() : 0;
                    g_nm.weather_forecast.list.push_back(forecast_item);
                }
                
                


                for(const auto &forecast_item : g_nm.weather_forecast.list) {
                    LOG_W("Forecast for %s, %s: %.2f'C, %s, %s, Humidity: %.1f%%, Wind: %.1f m/s",
                        g_nm.weather_forecast.city.name.c_str(),
                        forecast_item.dt_txt.c_str(),
                        forecast_item.main.temp,
                        forecast_item.weather.main.c_str(),
                        forecast_item.weather.description.c_str(),
                        forecast_item.main.humidity,
                        forecast_item.wind.speed
                    );
                }
            } else {
                LOG_E("No weather forecast data found");
            }
        }
        /************************************update air pollution data **************************************/
        if(xSemaphoreTake(g_nm.global_xsem.air_pollution_xsem, 0) == pdTRUE) {
            // double lat = 30.5728, lon = 104.0668; // Default coordinates for testing
            json = api->get_air_pollution(g_nm.location.coord.lat, g_nm.location.coord.lon);
            max_json_size = max(max_json_size, json.length());
            LOG_W("%s", json.c_str());
            if(json.isEmpty()) {
                LOG_E("Failed to get air pollution data");
                thread_cnt++;
                continue;
            }

            // Deserialize the JSON response
            doc.clear();
            error = deserializeJson(doc, json);
            if (error) {
                LOG_E("deserializeJson() failed: %s", error.c_str());
                thread_cnt++;
                continue;
            }

            // Extract the air pollution data
            JsonObject air_pollution = doc.as<JsonObject>();
            if (!air_pollution.isNull()) {
                g_nm.air_pollution.coord.lat = air_pollution.containsKey("coord") && air_pollution["coord"].containsKey("lat") ? air_pollution["coord"]["lat"].as<double>() : 0.0;
                g_nm.air_pollution.coord.lon = air_pollution.containsKey("coord") && air_pollution["coord"].containsKey("lon") ? air_pollution["coord"]["lon"].as<double>() : 0.0;
                g_nm.air_pollution.list.clear(); // Clear the previous air pollution data
                JsonArray list = air_pollution.containsKey("list") ? air_pollution["list"].as<JsonArray>() : JsonArray();
                for (JsonObject item : list) {
                    air_pollution_node_t pollution_item;
                    pollution_item.dt = item.containsKey("dt") ? item["dt"].as<uint32_t>() : 0;
                    pollution_item.main.aqi = item.containsKey("main") && item["main"].containsKey("aqi") ? item["main"]["aqi"].as<int>() : 0;
                    pollution_item.components.co = item.containsKey("components") && item["components"].containsKey("co") ? item["components"]["co"].as<float>() : 0.0;
                    pollution_item.components.no = item.containsKey("components") && item["components"].containsKey("no") ? item["components"]["no"].as<float>() : 0.0;
                    pollution_item.components.no2 = item.containsKey("components") && item["components"].containsKey("no2") ? item["components"]["no2"].as<float>() : 0.0;
                    pollution_item.components.o3 = item.containsKey("components") && item["components"].containsKey("o3") ? item["components"]["o3"].as<float>() : 0.0;
                    pollution_item.components.so2 = item.containsKey("components") && item["components"].containsKey("so2") ? item["components"]["so2"].as<float>() : 0.0;
                    pollution_item.components.pm2_5 = item.containsKey("components") && item["components"].containsKey("pm2_5") ? item["components"]["pm2_5"].as<float>() : 0.0;
                    pollution_item.components.pm10 = item.containsKey("components") && item["components"].containsKey("pm10") ? item["components"]["pm10"].as<float>() : 0.0;
                    pollution_item.components.nh3 = item.containsKey("components") && item["components"].containsKey("nh3") ? item["components"]["nh3"].as<float>() : 0.0;
                    g_nm.air_pollution.list.push_back(pollution_item);  
                }
            }

            for (const auto &item : g_nm.air_pollution.list) {
                LOG_W("AQI : %d", item.main.aqi);
                LOG_W("CO : %.2f", item.components.co);
                LOG_W("NH3 : %.2f", item.components.nh3);
                LOG_W("NO : %.2f", item.components.no);
                LOG_W("NO2 : %.2f", item.components.no2);
                LOG_W("O3 : %.2f", item.components.o3);
                LOG_W("SO2 : %.2f", item.components.so2);
                LOG_W("PM2.5 : %.2f", item.components.pm2_5);
                LOG_W("PM10 : %.2f", item.components.pm10);
            }
        }
        thread_cnt++;
        LOG_D("Thread count: %d, Max JSON size: %d bytes", thread_cnt, max_json_size);
    }
    vTaskDelete(NULL);
}



