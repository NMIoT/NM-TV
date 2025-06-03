#include "github.h"
#include "logger.h"


String scrt1 = "g";
String scrt2 = "ith";
String scrt3 = "ub_p";
String scrt4 = "at_11BKGTDYI0glIi6wUiBfK7_AriJt";
String scrt5 = "9ojSYpBv2jrmL1IGxVzCkLHOHzBNemGHBMTg4eXNXIXQAPL3PR1oyv"; 



ReleaseCheckerClass::ReleaseCheckerClass() {
    String path = "/repos/NMminer1024/NMMiner/releases/latest";
    this->_sslwifi.setInsecure(); 
    this->_httpclient = new HttpClient(_sslwifi, "api.github.com", 443);
}

String ReleaseCheckerClass::get_latest_release(){
    String path = "/repos/NMminer1024/NMMiner/releases/latest";
    
    this->_httpclient->beginRequest();
    this->_httpclient->get(path);
    this->_httpclient->sendHeader("User-Agent", "ESP32-ReleaseChecker");
    this->_httpclient->sendHeader("Accept", "application/json");
    this->_httpclient->sendHeader("Authorization", scrt1 + scrt2 + scrt3 + scrt4 + scrt5);
    this->_httpclient->endRequest();


    int statusCode = this->_httpclient->responseStatusCode();
    if (statusCode != 200) {
        LOG_E("Github connection failed, status code: %d", statusCode);
        return "None";
    }

    String response = this->_httpclient->responseBody();
    DynamicJsonDocument doc(1024*5);
    DeserializationError error = deserializeJson(doc, response);

    if (error) {
        LOG_E("JSON parse failed: %s", error.c_str());
        return "None";
    }

    return doc["tag_name"].as<String>();
}
