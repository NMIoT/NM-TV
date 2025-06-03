#ifndef __DEVICE_H__
#define __DEVICE_H__

#if defined(NERDMINERV2)
#include "nerdMinerV2.h"
#elif defined(M5STICK_C)
#include "M5Stick-C.h"
#elif defined(DEVKITV1)
#include "esp32DevKit.h"
#elif defined(LILYGO_T_DISPLAY_S3)
#include "lilygo_t_display_s3.h"
#elif defined(LILYGO_T_DISPLAY_S3_AMOLED)
#include "lilygo_t_display_s3_amoled.h"
#elif defined(NERMINER_S3_AMOLED)
#include "lilygoS3Amoled.h"
#elif defined(NERMINER_S3_DONGLE)
#include "lilygoS3Dongle.h"
#elif defined(LILYGO_S3_T_EMBED)
#include "lilygoS3TEmbed.h"
#elif defined(ESP32_2432S028R_9341) || defined(ESP32_2432S028R_7789)
#include "esp32_2432s028r.h"
#elif defined(ESP32_2432S024)
#include "esp32_2432s024.h"
#elif defined(ESP32_3248S035)
#include "esp32_3248s035.h"
#elif defined(ESP32_2432S028_2USB) // For another type of ESP32_2432S028 version with 2 USB connectors
#include "esp32_2432s028r.h"
#elif defined(NERMINER_T_QT)
#include "lilygoT_QT.h"
#elif defined(NERDMINER_T_DISPLAY_V1)
#include "lilygoV1TDisplay.h"
#elif defined(ESP32_CAM)
#include "esp32CAM.h"
#elif defined(ESP32RGB)
#include "esp32RGB.h"
#elif defined(M5_STAMP_S3)
#include "m5StampS3.h"
#elif defined(DEVKITV1RGB)
#include "esp32DevKitRGB.h"
#elif defined(S3MINIWEMOS)
#include "esp32S3MiniWemos.h"
#elif defined(S3MINIWEACT)
#include "esp32S3MiniWeact.h"
#elif defined(M5STACK_BOARD)
#include "m5stack.h"
#elif defined(WT32_BOARD)
#include "wt32.h"
#elif defined(NERMINER_S3_GEEK)
#include "waveshareS3Geek.h"
#elif defined(NERDMINER_T_HMI)
#include "lilygoT_HMI.h"
#elif defined(HELTEC_VISION_MASTER_T190) 
#include "heltec_vision_master_t190.h"
#elif defined(LILYGO_T_DONGLE_S3) 
#include "lilygo_t_dongle_s3.h"
#elif defined(HELTEC_LORA32_V3)
#include "heltec_wifi_lora32_v3.h"
#elif defined(HELTEC_LORA32_V2)
#include "heltec_wifi_lora32_v2.h"
#elif defined(HELTEC_WIFI_KIT_32_V3)
#include "heltec_wifi_kit_32_v3.h"
#elif defined(NM_BOT_CHAIN_V1)
#include "nm_bot_chain_v1.h"
#elif defined(HELTEC_WIRELESS_STICK_V3)
#include "heltec_wireless_stick_v3.h"
#elif defined(HELTEC_WIRELESS_STICK_LITE_V3)
#include "heltec_wireless_stick_lite_v3.h"
#elif defined(NM_USB_CHAIN_V1)
#include "nm_usb_chain_v1.h"
#elif defined(TTGO_T_DISPLAY)
#include "ttgo_t_display.h"
#elif defined(NM_TV_154)
#include "nm_tv_154.h"
#elif defined(SEEED_XIAO_ESP32_S3)
#include "xiao_esp32_s3.h"
#elif defined(SEEED_XIAO_ESP32_C3)
#include "xiao_esp32_c3.h"
#elif defined(ESP_DEV_KITC_32)
#include "esp32_dev_kitc_32.h"
#elif defined(ESP32_S3_GEEK)
#include "esp32_s3_geek.h"
#elif defined(ESP32_S3_042_OLED)
#include "esp32_s3_042_oled.h"
#elif defined(ESP32_C3_042_OLED)
#include "esp32_c3_042_oled.h"
#elif defined(LILYGO_T_QT)
#include "lilygo_t_qt.h" 
#elif defined(WT32_SC01)
#include "wt32_sc01.h"
#elif defined(ESP32_CAM)
#include "esp32cam.h"







#else
#error "No device defined"
#endif

#endif // __DEVICE_H__
