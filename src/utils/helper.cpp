#include "helper.h"
#include <iostream>
#include <cmath>
#include <string>
#include <sstream>
#include <iomanip>
#include "logger.h"
#include <nvs_flash.h>
#include <nvs.h>
#include "global.h"
#include "driver/usb_serial_jtag.h"

void psram_init(){
    if (esp_spiram_init() != ESP_OK) {
        LOG_W("Seems like no PSRAM available.");
    } else {
        // 检查 PSRAM 大小
        size_t psram_size = esp_spiram_get_size();
        LOG_I("PSRAM Size: %d Mbytes", psram_size/1024/1024);
  
        // 检查可用的堆内存
        size_t free_heap = heap_caps_get_free_size(MALLOC_CAP_8BIT);
        LOG_I("HEAP Size: %d Mbytes", free_heap/1024/1024);
  
        // 检查可用的 PSRAM 堆内存
        size_t free_psram_heap = heap_caps_get_free_size(MALLOC_CAP_SPIRAM);
        LOG_I("PSRAM HEAP Size: %d Mbytes", free_psram_heap/1024/1024);
  
        // 检查可用的内部堆内存
        size_t free_internal_heap = heap_caps_get_free_size(MALLOC_CAP_INTERNAL);
        LOG_I("INTERNAL HEAP Size: %d Mbytes", free_internal_heap/1024/1024);
    }
}

void logo_print() {
  log_w("\r\n            ___          ___         ");
  log_w("\r\n           /\\__\\        /\\__\\    ");
  log_w("\r\n          /::|  |      /::|  |       ");
  log_w("\r\n         /:|:|  |     /:|:|  |       ");
  log_w("\r\n        /:/|:|  |__  /:/|:|__|__     ");
  log_w("\r\n       /:/ |:| /\\__\\/:/ |::::\\__\\");
  log_w("\r\n       \\/__|:|/:/  /\\/__/~~/:/  /  ");
  log_w("\r\n           |:/:/  /       /:/  /     ");
  log_w("\r\n           |::/  /       /:/  /      ");
  log_w("\r\n           /:/  /       /:/  /       ");
  log_w("\r\n           \\/__/        \\/__/      \r\n");
  LOG_I("NMMiner version [%s] starting...", CURRENT_VERSION);
}

void disable_usb_uart(){
    Serial.end();
    pinMode(GPIO_NUM_19, INPUT);
    pinMode(GPIO_NUM_20, INPUT);
}

void enable_usb_uart(){
    Serial.setTimeout(20);
    Serial.begin(115200);
}

static uint8_t hex(char ch) {
    uint8_t r = (ch > 57) ? (ch - 55) : (ch - 48);
    return r & 0x0F;
}

int str_to_byte_array(const char *in, size_t in_size, uint8_t *out) {
    int count = 0;
    if (in_size % 2) {
        while (*in && out) {
            *out = hex(*in++);
            if (!*in)
                return count;
            *out = (*out << 4) | hex(*in++);
            *out++;
            count++;
        }
        return count;
    } else {
        while (*in && out) {
            *out++ = (hex(*in++) << 4) | hex(*in++);
            count++;
        }
        return count;
    }
}

void hex_byte_array_to_str(const uint8_t* hexArray, size_t length, char* charArray) {
    for (size_t i = 0; i < length; ++i) {
        sprintf(&charArray[i * 2], "%02x", hexArray[i]);
    }
}

static void reverse_bytes(uint8_t * data, size_t len) {
    for (int i = 0; i < len / 2; ++i) {
        uint8_t temp = data[i];
        data[i] = data[len - 1 - i];
        data[len - 1 - i] = temp;
    }
}

bool checkValid(uint8_t* hash, uint8_t* target) {
    for (int8_t i = 7; i >= 0; i--) {
        if (*(uint32_t*)(hash + 4*i) != *(uint32_t*)(target + 4*i)) {
            return *(uint32_t*)(hash + 4*i) < *(uint32_t*)(target + 4*i); // 从31字节开始比较，如果存在不相等的字节，返回该字节是否小于目标字节,即不相等的字节小于目标字节，返回true 
        }
    }
    return true; // 如果能执行到这里，说明hash和target相等，返回true
}

String formatNumber(float num, uint8_t total_bits) {
    const char *units[] = {" ", "K", "M", "G", "T", "P", "E", "Z", "Y"};
    int unitIndex = 0;

    while (num >= 1000.0 && unitIndex < 8) { // 上限为 8
        num /= 1000.0;
        unitIndex++;
    }

    // 如果达到最大数量级且 num 依然大于等于 1000，则返回 "999.9Y"
    if (num >= 1000.0 && unitIndex == 8) {
        return "999.9Y";
    }

    // 根据 total_bits 确定小数点后的位数
    int integerPartLength = 1;
    if (num >= 10) {
        integerPartLength = 2;
    }
    if (num >= 100) {
        integerPartLength = 3;
    }

    int decimalPlaces = total_bits - integerPartLength;
    if (decimalPlaces < 0) {
        decimalPlaces = 0;
    }

    String formattedNumber = String(num, decimalPlaces) + units[unitIndex];
    return formattedNumber;
}

String get_last_reboot_reason(){
  switch(esp_reset_reason()){
    case ESP_RST_UNKNOWN: return "Unknown";
    case ESP_RST_POWERON: return "Power on";
    case ESP_RST_EXT: return "External reset";
    case ESP_RST_SW: return "Software reset";
    case ESP_RST_PANIC: return "Exception reset";
    case ESP_RST_INT_WDT: return "Watchdog int reset";
    case ESP_RST_TASK_WDT: return "Task watchdog reset";
    case ESP_RST_WDT: return "Watchdog reset";
    case ESP_RST_DEEPSLEEP: return "Deep sleep";
    case ESP_RST_BROWNOUT: return "Brownout";
    case ESP_RST_SDIO: return "SDIO reset";
    default: return "Unknown";
  }
}

String convert_time_to_local(uint32_t timestamp) {
    time_t localTime = timestamp;

    struct tm *timeinfo = localtime(&localTime);
    char timeString[30] = {0,};
    // 使用 %I 表示 12 小时制，%p 表示 AM/PM
    strftime(timeString, sizeof(timeString), "%d-%m-%Y %I:%M:%S %p", timeinfo);

    return String(timeString);
}

String convert_uptime_to_string(uint32_t timecnt){
    char uptime[20] = {0,};

    uint32_t days = timecnt / 86400;
    uint32_t hours = (timecnt % 86400) / 3600;
    uint32_t minutes = (timecnt % 3600) / 60;
    uint32_t seconds = timecnt % 60;
    
    sprintf(uptime, "%03dd %02d:%02d:%02d", days, hours, minutes, seconds);
    return String(uptime);
}

static double le256todouble(const void *target){
	static uint64_t *data64 = NULL;
	static double dcut64;

	data64 = (uint64_t *)((uint8_t *)target + 24);
	dcut64 = *data64 * 6277101735386680763835789423207666416102355444464034512896.0;

	data64 = (uint64_t *)((uint8_t *)target + 16);
	dcut64 += *data64 * 340282366920938463463374607431768211456.0;

	data64 = (uint64_t *)((uint8_t *)target + 8);
	dcut64 += *data64 * 18446744073709551616.0;

	data64 = (uint64_t *)((uint8_t *)target);
	dcut64 += *data64;

	return dcut64;
}

double diff_calculate(void *target){
	static double d64 = 0.0, dcut64 = 0.0;
    static const double truediffone = 26959535291011309493156476344723991336010898738574164086137773096960.0;
	d64 = truediffone;
	dcut64 = le256todouble(target);

    // 检查 dcut64 是否为零或非常接近零
    if (dcut64 == 0.0 || std::abs(dcut64) < std::numeric_limits<double>::epsilon()) {
        // 返回一个特定的错误值，例如 NaN
        return std::numeric_limits<double>::quiet_NaN();
    }

    double result = d64 / dcut64;
    // 检查结果是否为无穷大
    if (std::isinf(result)) {
        LOG_E("diff calculate error infinite result!!!");
        // 处理无穷大的情况，例如返回一个特定的错误值
        return std::numeric_limits<double>::infinity();
    }

    return result;
}

bool isDigit(char ch) {
    return (ch >= '0' && ch <= '9');
}

float parseHashRateStr(const String& hashRateStr) {
    float hashRate = 0.0;
    int len = hashRateStr.length();

    // 查找单位的位置
    int unitPos = hashRateStr.indexOf("H/s");
    if (unitPos == -1) {
        return hashRate; // 无效的输入格式
    }


    String valueStr = ""; // 数值部分
    String unitStr = ""; // 单位部分
    if(isDigit(hashRateStr.substring(unitPos-1, unitPos).c_str()[0])){
        // 提取数值部分
        valueStr = hashRateStr.substring(0, unitPos);
        valueStr.trim(); // 去除前后的空格
        // 提取单位部分
        unitStr = hashRateStr.substring(unitPos, len);
        unitStr.trim(); // 去除前后的空格
    }else{
        // 提取数值部分
        valueStr = hashRateStr.substring(0, unitPos - 1);
        valueStr.trim(); // 去除前后的空格
        // 提取单位部分
        unitStr = hashRateStr.substring(unitPos-1, len);
        unitStr.trim(); // 去除前后的空格
    }
    // 将数值部分转换为浮点数
    float value = valueStr.toFloat();
    // 根据单位转换为 H/s
    if (unitStr == "TH/s" || unitStr == "tH/s" || unitStr == "th/s" || unitStr == "Th/s") {
        hashRate = value * 1e12;
    }else if (unitStr == "GH/s" || unitStr == "gH/s" || unitStr == "gh/s" || unitStr == "Gh/s") {
        hashRate = value * 1e9;
    } else if (unitStr == "MH/s" || unitStr == "mH/s" || unitStr == "mh/s" || unitStr == "Mh/s") {
        hashRate = value * 1e6;
    } else if (unitStr == "KH/s" || unitStr == "kH/s" || unitStr == "Kh/s" || unitStr == "kh/s") {
        hashRate = value * 1e3;
    } else if (unitStr == "H/s" || unitStr == "h/s") {
        hashRate = value;
    } else{
        return 0.0; // 无效的单位
    }

    return hashRate;
}

float parseDiffStr(const String& diffStr) {
    float diff = 0.0;
    int len = diffStr.length();

    // 查找单位的位置
    String unitStr = String(diffStr.charAt(len-1)); // 单位部分
    if(isdigit(unitStr.c_str()[0])){ // 如果最后一个字符是数字，则没有单位部分
        diff = diffStr.toFloat(); // 直接转换为浮点数
        return diff;
    }

    // 提取数值部分
    String valueStr = diffStr.substring(0, len - 1); // 去除最后一个字符（单位）
    valueStr.trim(); // 去除前后的空格

    // 将数值部分转换为浮点数
    diff = valueStr.toFloat();

    if(unitStr == "K"){ // 单位为K，则乘以1000
        diff *= 1e3;
    }else if(unitStr == "M"){ // 单位为M，则乘以1000000
        diff *= 1e6;
    }else if(unitStr == "G"){ // 单位为G，则乘以1000000000
        diff *= 1e9;
    }else if(unitStr == "T"){ // 单位为T，则乘以1000000000000
        diff *= 1e12;
    }else if(unitStr == "P"){ // 单位为P，则乘以1000000000000000
        diff *= 1e15;
    }else {
        return 0.0; // 无效的单位
    }

    return diff;
}

void* psramAllocator(size_t size) {
    return heap_caps_malloc(size, MALLOC_CAP_SPIRAM);
}

void psramDeallocator(void* pointer) {
    heap_caps_free(pointer);
}


int compareVersions(const String& current, const String& release) {
    if((release == "") || (release == "None")) return -2; // have no release version get yet


    String v1 = current.substring(1);
    String v2 = release.substring(1);
    
    int major1 = 0, minor1 = 0, patch1 = 0;
    int major2 = 0, minor2 = 0, patch2 = 0;
    
    // parse version1
    int dotPos1 = v1.indexOf('.');
    if (dotPos1 != -1) {
        major1 = v1.substring(0, dotPos1).toInt();
        int dotPos2 = v1.indexOf('.', dotPos1 + 1);
        if (dotPos2 != -1) {
            minor1 = v1.substring(dotPos1 + 1, dotPos2).toInt();
            patch1 = v1.substring(dotPos2 + 1).toInt();
        } else {
            minor1 = v1.substring(dotPos1 + 1).toInt();
        }
    } else {
        major1 = v1.toInt();
    }
    
    // parse version2
    int dotPos3 = v2.indexOf('.');
    if (dotPos3 != -1) {
        major2 = v2.substring(0, dotPos3).toInt();
        int dotPos4 = v2.indexOf('.', dotPos3 + 1);
        if (dotPos4 != -1) {
            minor2 = v2.substring(dotPos3 + 1, dotPos4).toInt();
            patch2 = v2.substring(dotPos4 + 1).toInt();
        } else {
            minor2 = v2.substring(dotPos3 + 1).toInt();
        }
    } else {
        major2 = v2.toInt();
    }
    
    if (major1 < major2) return -1;
    if (major1 > major2) return 1;
    

    if (minor1 < minor2) return -1;
    if (minor1 > minor2) return 1;
    

    if (patch1 < patch2) return -1;
    if (patch1 > patch2) return 1;
    

    return 0;
}
