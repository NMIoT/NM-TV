#include "global.h"

#if defined(TFT_DISPLAY) || defined(AMOLED_DISPLAY)
#include "display.h"
#include "logger.h"
#include "lvgl.h"
#include "monitor.h"
#include "scrsaver.h"
#include <TFT_eSPI.h>
#include "helper.h"
#define SCREEN_WIDTH  TFT_WIDTH
#define SCREEN_HEIGHT TFT_HEIGHT
static TFT_eSPI     tft     = TFT_eSPI();

enum{
  PAGE_LOADING = 0,
  PAGE_CONFIG,
  PAGE_MINER,
  PAGE_CLOCK,
  PAGE_GAUGE,
  PAGE_END
};

/*********************************************************************全局lvgl对象********************************************************************/
static lv_color_t          *lvgl_color_buf = NULL;
static lv_disp_draw_buf_t   draw_buf;
static SemaphoreHandle_t    lvgl_xMutex;
static lv_obj_t *parent_docker = NULL;
static lv_obj_t *g_pages[] = {NULL, NULL, NULL, NULL, NULL, NULL};
static lv_obj_t *saver_scr = NULL;
static lv_obj_t *loading_page = NULL, *config_page = NULL ,*miner_page = NULL, *clock_page = NULL;
static lv_obj_t *lb_lpage_progress_value = NULL, *lb_lpage_details_str = NULL, *lb_lpage_bar = NULL, *lb_lpage_version = NULL, *lb_slogan = NULL, *lb_version_check = NULL, *lb_flash_addr = NULL;
static lv_obj_t *lb_config_page_version = NULL, *lb_config_page_timeout = NULL;
static lv_obj_t *lb_mpage_temp_unit = NULL, *lb_mpage_hr_value = NULL, *lb_mpage_version = NULL, *lb_mpage_blk_hit = NULL, *lb_mpage_temp_or_time_value = NULL,*lb_mpage_ip_value = NULL,*lb_mpage_rssi_value = NULL,*lb_mpage_price_value = NULL;
static lv_obj_t *lb_mpage_job_rec_value = NULL, *lb_mpage_net_diff_value = NULL, *lb_mpage_local_diff_value = NULL,*lb_mpage_shares_value = NULL;
static lv_obj_t *lb_mpage_uptime_day_value = NULL, *lb_mpage_uptime_hms_value = NULL, *lb_mpage_uptime_day_unit_value = NULL;
static lv_obj_t *lb_mpage_rssi_symbol = NULL, *lb_mpage_price_symbol = NULL, *lb_mpage_job_receive_symbol = NULL, *lb_mpage_net_diff_symbol = NULL, *lb_mpage_local_diff_symbol = NULL,*lb_mpage_share_symb = NULL;
static lv_obj_t *lb_clock_page_day_value = NULL, *lb_clock_page_hms_value = NULL, *lb_clock_page_hr_value = NULL, *lb_clock_page_hit_value = NULL, *lb_clock_page_hr_unit = NULL, *lb_clock_page_price = NULL, *lb_clock_page_price_changed = NULL, *lb_clock_page_pool = NULL;
static lv_obj_t *lb_clock_page_version = NULL;

static uint16_t g_page_index = PAGE_MINER;

#include "image_240_240.h"
LV_FONT_DECLARE(ds_digib_font_10)
LV_FONT_DECLARE(ds_digib_font_12)
LV_FONT_DECLARE(ds_digib_font_14)
LV_FONT_DECLARE(ds_digib_font_16)
LV_FONT_DECLARE(ds_digib_font_18)
LV_FONT_DECLARE(ds_digib_font_22)
LV_FONT_DECLARE(ds_digib_font_24)
LV_FONT_DECLARE(ds_digib_font_28)
LV_FONT_DECLARE(ds_digib_font_34)
LV_FONT_DECLARE(ds_digib_font_36)
LV_FONT_DECLARE(ds_digib_font_50)
LV_FONT_DECLARE(ds_digib_font_54)
LV_FONT_DECLARE(ds_digib_font_58)
LV_FONT_DECLARE(symbol_10)
LV_FONT_DECLARE(symbol_14)
LV_FONT_DECLARE(symbol_18)
LV_FONT_DECLARE(symbol_20)
static const lv_font_t *lb_loading_progress_font  = &lv_font_montserrat_14;
static const lv_font_t *lb_loading_slogan_font    = &lv_font_montserrat_18;
static const lv_font_t *lb_loading_version_check_font = &lv_font_montserrat_14;
static const lv_font_t *lb_cfg_timeout_font  = &lv_font_montserrat_18;
static const lv_font_t *lb_version_loading_font  = &lv_font_montserrat_18;
static const lv_font_t *lb_cfg_version_font  = &lv_font_montserrat_18;
static const lv_font_t *lb_loading_font  = &lv_font_montserrat_18;
static const lv_font_t *lb_hashrate_font = &ds_digib_font_36;
static const lv_font_t *lb_blk_hit_font  = &ds_digib_font_58;
static const lv_font_t *lb_temp_or_time_font     = &ds_digib_font_14;
static const lv_font_t *lb_wifi_ip_font  = &ds_digib_font_18;
static const lv_font_t *lb_wifi_rssi_font = &ds_digib_font_18;
static const lv_font_t *lb_mpage_price_font     = &ds_digib_font_16;
static const lv_font_t *lb_uptime_font       = &ds_digib_font_18;
static const lv_font_t *lb_uptime_unit_font  = &lv_font_montserrat_16;
static const lv_font_t *lb_wifi_rssi_symbol_font = &lv_font_montserrat_14;
static const lv_font_t *lb_price_symbol_font     = &symbol_14;

static const lv_font_t *lb_job_received_font = &ds_digib_font_22;
static const lv_font_t *lb_net_diff_font     = &ds_digib_font_22;
static const lv_font_t *lb_local_diff_font   = &ds_digib_font_22;
static const lv_font_t *lb_shares_font       = &ds_digib_font_22;

static const lv_font_t *lb_clock_pool_font       = &lv_font_montserrat_18;
static const lv_font_t *lb_clock_price_font      = &ds_digib_font_36;
static const lv_font_t *lb_miner_version_font    = &ds_digib_font_16;

static const lv_font_t *lb_job_receive_symbol_font        = &symbol_18;
static const lv_font_t *lb_net_diff_symbol_font           = &symbol_18;
static const lv_font_t *lb_local_diff_symbol_font         = &symbol_18;
static const lv_font_t *lb_share_symb_font                = &symbol_18;

static const lv_font_t *lb_clock_hr_font                  = &ds_digib_font_36;
static const lv_font_t *lb_clock_hr_unit_font             = &lv_font_montserrat_14;
static const lv_font_t *lb_clock_version_font             = &ds_digib_font_12;
static const lv_font_t *lb_clock_day_font                 = &ds_digib_font_24;
static const lv_font_t *lb_clock_hms_font                 = &ds_digib_font_54;
static const lv_font_t *lb_clock_price_changed_font       = &ds_digib_font_12;

static const lv_font_t *lb_swarm_total_hr_font            = &ds_digib_font_22;
static const lv_font_t *lb_swarm_best_diff_font           = &ds_digib_font_22;
static const lv_font_t *lb_swarm_workers_font             = &ds_digib_font_22;


static const lv_coord_t lb_version_loading_coord[2] = {(lv_coord_t)(SCREEN_WIDTH - (int16_t)(String(CURRENT_VERSION).length() * 8)), 2};
static const lv_coord_t lb_version_config_coord[2] = {(lv_coord_t)(SCREEN_WIDTH - (int16_t)(String(CURRENT_VERSION).length() * 12)), 10};  
static const lv_coord_t lb_timeout_config_coord[2] = {(lv_coord_t)(SCREEN_WIDTH - (int16_t)(String(CURRENT_VERSION).length() * 11)), SCREEN_HEIGHT - 35};  
static const lv_coord_t lb_mine_page_ver_coord[2]  = {10, 128};
static const lv_coord_t lb_slogan_loading_coord[2] = {0, 10};  
static const lv_coord_t lb_version_check_loading_coord[2] = {0, 40};
static const lv_coord_t lb_flash_addr_loading_coord[2] = {0, 70};
static const lv_coord_t lb_hashrate_coord[2] = {125, 160};
static const lv_coord_t lb_blk_hit_coord[2] = {11, 65};
static const lv_coord_t lb_temp_or_time_coord[2] = {2, 5};
static const lv_coord_t lb_uptime_day_coord[2] = {58, 5};
static const lv_coord_t lb_wifi_ip_coord[2] = {13, 175};
static const lv_coord_t lb_wifi_rssi_coord[2] = {185, 5};

static const lv_coord_t lb_price_coord[2] = {40, 145};
static const lv_coord_t lb_price_symbol_coord[2] = {20, 145};

static const lv_coord_t lb_uptime_day_unit_coord[2] = {86, 5};
static const lv_coord_t lb_uptime_hms_coord[2] = {96, 5};
static const lv_coord_t lb_wifi_rssi_symbol_coord[2] = {165, 5};




static const lv_coord_t lb_job_received_coord[2] = {112 + 28, 15 + 14};
static const lv_coord_t lb_net_diff_coord[2]     = {112 + 28, 2*15 + 14*2 + 1};
static const lv_coord_t lb_local_diff_coord[2]   = {112 + 28, 3*15 + 14*3};
static const lv_coord_t lb_shares_coord[2]       = {112 + 28, 4*15 + 14*4 - 1};

static const lv_coord_t lb_job_receive_symbol_coord[2]  = {112, 15 +15};
static const lv_coord_t lb_net_diff_symbol_coord[2]     = {112, 2*15 +15*2 + 1};
static const lv_coord_t lb_local_diff_symbol_coord[2]   = {112, 3*15 +15*3};
static const lv_coord_t lb_share_symb_coord[2]          = {112 - 3, 4*15 + 15*4 - 3};

static const lv_coord_t lb_clock_coord[2] = {0, 5};
static const lv_coord_t lb_clock_hr_coord[2] = {95 + 45 , 2};
static const lv_coord_t lb_clock_hr_unit_coord[2] = {0, 18};
static const lv_coord_t lb_clock_version_coord[2] = {8, 5};

static const lv_coord_t lb_clock_day_coord[2] = {0, 0};
static const lv_coord_t lb_clock_hms_coord[2] = {0, 0};
static const lv_coord_t lb_clock_price_coord[2] = {2, 2};
static const lv_coord_t lb_clock_pool_coord[2] = {1, -6};
static const lv_coord_t lb_clock_price_changed_coord[2] = {1, 25};

static const lv_coord_t lb_swarm_best_diff_coord[2] = {2, 215};
static const lv_coord_t lb_swarm_workers_coord[2]   = {106, 215};
static const lv_coord_t lb_swarm_total_hr_coord[2]  = {170, 215};


const lv_img_dsc_t loading_img = {
    .header = {
        .cf = LV_IMG_CF_TRUE_COLOR,
        .always_zero = 0,
        .reserved = 0,
        .w = SCREEN_WIDTH,  
        .h = SCREEN_HEIGHT,  
    },
    .data_size = SCREEN_WIDTH * SCREEN_HEIGHT * LV_COLOR_SIZE / 8,
    .data = (const uint8_t *)clock_img_array, 
};

const lv_img_dsc_t config_img = {
    .header = {
        .cf = LV_IMG_CF_TRUE_COLOR,
        .always_zero = 0,
        .reserved = 0,
        .w = SCREEN_WIDTH,  
        .h = SCREEN_HEIGHT,  
    },
    .data_size = SCREEN_WIDTH * SCREEN_HEIGHT * LV_COLOR_SIZE / 8,
    .data = (const uint8_t *)clock_img_array, 
};

const lv_img_dsc_t miner_img = {
    .header = {
        .cf = LV_IMG_CF_TRUE_COLOR,
        .always_zero = 0,
        .reserved = 0,
        .w = SCREEN_WIDTH,  
        .h = SCREEN_HEIGHT,  
    },
    .data_size = SCREEN_WIDTH * SCREEN_HEIGHT * LV_COLOR_SIZE / 8,
    .data = (const uint8_t *)clock_img_array, 
};

const lv_img_dsc_t clock_img = {
    .header = {
        .cf = LV_IMG_CF_TRUE_COLOR,
        .always_zero = 0,
        .reserved = 0,
        .w = SCREEN_WIDTH,  
        .h = SCREEN_HEIGHT,  
    },
    .data_size = SCREEN_WIDTH * SCREEN_HEIGHT * LV_COLOR_SIZE / 8,
    .data = (const uint8_t *)clock_img_array, 
};

#if defined(HAS_SCREEN_SAVER_FEATURE)
const lv_img_dsc_t saver_img = {
  .header = {
#if defined (AMOLED_DISPLAY)
      .cf = LV_IMG_CF_TRUE_COLOR_ALPHA , 
#elif defined (TFT_DISPLAY)
      .cf = LV_IMG_CF_TRUE_COLOR,
#endif 
      .always_zero = 0,
      .reserved = 0,
      .w = SCREEN_WIDTH,  
      .h = SCREEN_HEIGHT,  
  },
#if defined (AMOLED_DISPLAY)
  .data_size = SCREEN_WIDTH * SCREEN_HEIGHT * LV_IMG_PX_SIZE_ALPHA_BYTE,
#elif defined (TFT_DISPLAY)
  .data_size = SCREEN_WIDTH * SCREEN_HEIGHT * LV_COLOR_SIZE / 8,
#endif
  .data = (const uint8_t *)saver_img_array, 
};
#endif


static int blpwmChannel = 0;  
static void tft_bl_ctrl(float brightness) {
#ifdef TFT_DISPLAY
    if (TFT_BL != -1) {
      // 确保亮度在 0 到 1 之间
      brightness = brightness <= 0 ? 0 : (brightness >= 1 ? 1 : brightness);
      uint8_t pwm = (TFT_BACKLIGHT_ON == HIGH) ? (uint8_t)(brightness * 255) : (uint8_t)((1 - brightness) * 255);
      ledcWrite(blpwmChannel, pwm);
    }
#elif defined(AMOLED_DISPLAY)
    amoled.setBrightness(brightness * 255);
#endif
}

void tft_bl_active(void){
#ifdef TFT_DISPLAY
  if(TFT_BL!=-1){
    g_nm.screen.active = true;
    g_nm.screen.cnt = 0;
    float brightness = (float)g_nm.screen.brightness/100.0f;
    brightness = brightness <= 0 ? 1 : brightness;//亮度为0时默认为用户设置的起始亮度
    tft_bl_ctrl(brightness);
  }
#endif
  g_nm.screen.last_operaion = millis();
}

static void screen_init(){
  if(TFT_PWER_PIN!=-1){
    pinMode(TFT_PWER_PIN, OUTPUT);
    digitalWrite(TFT_PWER_PIN, LOW);
  }
  
  tft.begin(); 
  tft.setSwapBytes(true);

  if(g_nm.screen.orientation){
    #ifdef NM_TV_154
      tft.setRotation(2);
    #else
      tft.setRotation(1); 
    #endif
  }
  else {
    #ifdef NM_TV_154
      tft.setRotation(0);
    #else
      tft.setRotation(3); 
    #endif
  }

  //等tft初始化完成后再重新设置背光控制PWM
  if(TFT_BL!=-1){
    int freq = 5*1000;    // PWM频率，单位Hz 
    int resolution = 8;   // PWM分辨率，8位
    pinMode(TFT_BL, OUTPUT);
    ledcSetup(blpwmChannel, freq, resolution);
    ledcAttachPin(TFT_BL, blpwmChannel);
    tft_bl_active();
    if(g_nm.screen.sleep_timeout != 0)
      LOG_I("screen sleep timeout: %ds, initial brightness to %d%%", g_nm.screen.sleep_timeout, g_nm.screen.brightness);
    else
      LOG_I("screen never sleep.");
  }
}

static void disp_flush( lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p ){
    uint32_t w = ( area->x2 - area->x1 + 1 );
    uint32_t h = ( area->y2 - area->y1 + 1 );

#ifdef TFT_DISPLAY
    tft.startWrite();
    tft.setAddrWindow( area->x1 , area->y1 , w , h  );
    tft.pushColors( ( uint16_t * )&color_p->full, w * h, true );
    tft.endWrite();
#elif defined(AMOLED_DISPLAY)
    static_cast<LilyGo_Display *>(disp_drv->user_data)->pushColors(area->x1, area->y1, w, h, (uint16_t *)color_p);
#endif
    lv_disp_flush_ready(disp_drv);
}

static void lvgl_tick_task(void *args){
  char *name = (char*)malloc(20);
  strcpy(name, (char*)args);
  LOG_I("%s thread started on core %d...", name, xPortGetCoreID());
  free(name);

  lvgl_xMutex = xSemaphoreCreateMutex();
  if(lvgl_xMutex == NULL){
    LOG_E("Failed to create lvgl_xMutex");
    vTaskDelete(NULL);
  }
  static uint16_t tick_interval = 50;
  uint32_t last_tick = millis();
  while(true){
    delay(tick_interval/2);
    if(!g_nm.screen.active)continue;//息屏后不刷新
    if (xSemaphoreTake(lvgl_xMutex, tick_interval/2) == pdTRUE){
      lv_tick_inc(millis() - last_tick);
      lv_timer_handler(); /* let the GUI do its work */
      xSemaphoreGive(lvgl_xMutex); 
      last_tick = millis();
    }
  }
}

static void ui_init(void){
  LOG_I("lvgl version: %s", (String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch()).c_str());
  const uint32_t color_buf_size = SCREEN_WIDTH * SCREEN_HEIGHT / 20;
  lvgl_color_buf = (lv_color_t*)malloc(color_buf_size * sizeof(lv_color_t));

  if(lvgl_color_buf == NULL){
    LOG_E("Failed to allocate memory for lvgl_color_buf");
    return;
  }
  lv_disp_draw_buf_init( &draw_buf, lvgl_color_buf, NULL, color_buf_size);

  /*Initialize the display*/
  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init( &disp_drv );
  /*Change the following line to your display resolution*/
  disp_drv.hor_res = SCREEN_WIDTH;
  disp_drv.ver_res = SCREEN_HEIGHT;
  disp_drv.flush_cb = disp_flush;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register( &disp_drv );
}

static void ui_layout_init(void){
  //create parent object
  parent_docker = lv_obj_create(lv_scr_act());
  lv_obj_set_size(parent_docker, SCREEN_WIDTH * 2, SCREEN_HEIGHT * 3); 
  lv_obj_set_pos(parent_docker, 0, 0);
  lv_obj_set_scrollbar_mode(lv_scr_act(), LV_SCROLLBAR_MODE_OFF); 
  lv_obj_set_scroll_dir(parent_docker, LV_DIR_ALL); 
  lv_obj_set_scroll_snap_x(parent_docker, LV_SCROLL_SNAP_START); 
  lv_obj_set_scroll_snap_y(parent_docker, LV_SCROLL_SNAP_START); 
  lv_obj_set_style_pad_all(parent_docker, 0, 0);
  lv_obj_set_style_border_width(parent_docker, 0, 0);
  lv_obj_align(parent_docker, LV_ALIGN_TOP_LEFT, 0, 0);

  // Create loading page
  loading_page = lv_obj_create(parent_docker);
  lv_obj_set_size(loading_page, SCREEN_WIDTH, SCREEN_HEIGHT);
  lv_obj_set_pos(loading_page, 0 , 0);
  lv_obj_set_style_pad_all(loading_page, 0, 0);
  lv_obj_set_style_border_width(loading_page, 0, 0);
  lv_obj_set_scrollbar_mode(loading_page, LV_SCROLLBAR_MODE_OFF); 
  lv_obj_t *loading_img_obj = lv_img_create(loading_page);
  lv_img_set_src(loading_img_obj, &loading_img);
  lv_obj_set_size(loading_img_obj, SCREEN_WIDTH, SCREEN_HEIGHT);
  lv_obj_align(loading_img_obj, LV_ALIGN_TOP_LEFT, 0, 0);
  // Create config page
  config_page = lv_obj_create(parent_docker);
  lv_obj_set_size(config_page, SCREEN_WIDTH, SCREEN_HEIGHT);
  lv_obj_set_pos(config_page, 0 * SCREEN_WIDTH, 1 * SCREEN_HEIGHT); 
  lv_obj_set_style_pad_all(config_page, 0, 0);
  lv_obj_set_style_border_width(config_page, 0, 0);
  lv_obj_set_scrollbar_mode(config_page, LV_SCROLLBAR_MODE_OFF); 
  lv_obj_t *config_img_obj = lv_img_create(config_page);
  lv_img_set_src(config_img_obj, &config_img);
  lv_obj_set_size(config_img_obj, SCREEN_WIDTH, SCREEN_HEIGHT);
  lv_obj_align(config_img_obj, LV_ALIGN_TOP_LEFT, 0, 0);
  // Create miner page  
  miner_page = lv_obj_create(parent_docker);
  lv_obj_set_size(miner_page, SCREEN_WIDTH, SCREEN_HEIGHT);
  lv_obj_set_pos(miner_page, 1 * SCREEN_WIDTH, 0);
  lv_obj_set_style_pad_all(miner_page, 0, 0);
  lv_obj_set_style_border_width(miner_page, 0, 0);
  lv_obj_set_scrollbar_mode(miner_page, LV_SCROLLBAR_MODE_OFF); 
  lv_obj_t *miner_img_obj = lv_img_create(miner_page);
  lv_img_set_src(miner_img_obj, &miner_img);
  lv_obj_set_size(miner_img_obj, SCREEN_WIDTH, SCREEN_HEIGHT);
  lv_obj_align(miner_img_obj, LV_ALIGN_TOP_LEFT, 0, 0);
  
  // Create clock page  
  clock_page = lv_obj_create(parent_docker);
  lv_obj_set_size(clock_page, SCREEN_WIDTH, SCREEN_HEIGHT);
  lv_obj_set_pos(clock_page, SCREEN_WIDTH, SCREEN_HEIGHT);
  lv_obj_set_style_pad_all(clock_page, 0, 0);
  lv_obj_set_style_border_width(clock_page, 0, 0);
  lv_obj_set_scrollbar_mode(clock_page, LV_SCROLLBAR_MODE_OFF); 
  lv_obj_t *clock_img_obj = lv_img_create(clock_page);
  lv_img_set_src(clock_img_obj, &clock_img);
  lv_obj_set_size(clock_img_obj, SCREEN_WIDTH, SCREEN_HEIGHT);
  lv_obj_align(clock_img_obj, LV_ALIGN_TOP_LEFT, 0, 0);

  // Create g_pages array
  g_pages[PAGE_LOADING] = loading_page;
  g_pages[PAGE_CONFIG]  = config_page;
  g_pages[PAGE_MINER]   = miner_page;
  g_pages[PAGE_CLOCK]   = clock_page; 

  //////////////////////////////////////loading page layout///////////////////////////////////////////////
  //Version
  lv_color_t font_color = lv_color_hex(0xFFFFFF);
  lb_lpage_version   = lv_label_create( loading_page );
  lv_obj_set_width(lb_lpage_version, SCREEN_WIDTH);
  lv_label_set_text( lb_lpage_version, CURRENT_VERSION);
  lv_obj_set_style_text_font(lb_lpage_version, lb_version_loading_font, LV_PART_MAIN);
  lv_obj_set_style_text_color(lb_lpage_version, font_color, LV_PART_MAIN); 
  lv_label_set_long_mode(lb_lpage_version, LV_LABEL_LONG_DOT);
  lv_obj_align( lb_lpage_version, LV_ALIGN_TOP_MID, lb_version_loading_coord[0], lb_version_loading_coord[1]);
  //Slogan 
  String slogan = "Make it better";
  font_color = lv_color_hex(0xFFFFFF);
  lb_slogan   = lv_label_create( loading_page );
  lv_coord_t width = lv_txt_get_width(slogan.c_str(), strlen(slogan.c_str()), lb_loading_slogan_font, 0, LV_TEXT_FLAG_NONE);
  lv_obj_set_width(lb_slogan, width);
  lv_label_set_text( lb_slogan, slogan.c_str());
  lv_obj_set_style_text_font(lb_slogan, lb_loading_slogan_font, LV_PART_MAIN);
  lv_obj_set_style_text_color(lb_slogan, font_color, LV_PART_MAIN); 
  lv_label_set_long_mode(lb_slogan, LV_LABEL_LONG_DOT);
  lv_obj_align( lb_slogan, LV_ALIGN_CENTER, lb_slogan_loading_coord[0], lb_slogan_loading_coord[1]);
  //////////////////////////////////////config page layout///////////////////////////////////////////////
  //version
  font_color = lv_color_hex(0xEE7D30);
  lb_config_page_version   = lv_label_create( config_page );
  lv_obj_set_width(lb_config_page_version, SCREEN_WIDTH);
  lv_label_set_text( lb_config_page_version, g_nm.board.fw_version.c_str());
  lv_obj_set_style_text_font(lb_config_page_version, lb_cfg_version_font, LV_PART_MAIN);
  lv_obj_set_style_text_color(lb_config_page_version, font_color, LV_PART_MAIN); 
  lv_label_set_long_mode(lb_config_page_version, LV_LABEL_LONG_DOT);
  lv_obj_align( lb_config_page_version, LV_ALIGN_TOP_MID, lb_version_config_coord[0], lb_version_config_coord[1]);
  //config timeout
  font_color = lv_color_hex(0xFFFFFF);
  lb_config_page_timeout   = lv_label_create( config_page );
  lv_obj_set_width(lb_config_page_timeout, SCREEN_WIDTH);
  lv_label_set_text( lb_config_page_timeout, "");
  lv_obj_set_style_text_font(lb_config_page_timeout, lb_cfg_timeout_font, LV_PART_MAIN);
  lv_obj_set_style_text_color(lb_config_page_timeout, font_color, LV_PART_MAIN); 
  lv_label_set_long_mode(lb_config_page_timeout, LV_LABEL_LONG_DOT);
  lv_obj_align( lb_config_page_timeout, LV_ALIGN_TOP_MID, lb_timeout_config_coord[0], lb_timeout_config_coord[1]);
 //////////////////////////////////////miner page layout///////////////////////////////////////////////
  //Hashrate value
  font_color = lv_color_hex(0xEE7D30);
  lb_mpage_hr_value   = lv_label_create( miner_page );
  lv_obj_set_width(lb_mpage_hr_value, SCREEN_WIDTH);
  lv_label_set_text( lb_mpage_hr_value, " ");
  lv_obj_set_style_text_font(lb_mpage_hr_value, lb_hashrate_font, LV_PART_MAIN);
  lv_obj_set_style_text_color(lb_mpage_hr_value, font_color, LV_PART_MAIN); 
  lv_label_set_long_mode(lb_mpage_hr_value, LV_LABEL_LONG_DOT);
  lv_obj_align( lb_mpage_hr_value, LV_ALIGN_TOP_MID, lb_hashrate_coord[0], lb_hashrate_coord[1]); 
  //Hit value
  font_color = lv_color_hex(0xEE7D30);
  lb_mpage_blk_hit   = lv_label_create( miner_page );
  lv_obj_set_width(lb_mpage_blk_hit, SCREEN_WIDTH);
  lv_label_set_text( lb_mpage_blk_hit, " ");
  lv_obj_set_style_text_font(lb_mpage_blk_hit, lb_blk_hit_font, LV_PART_MAIN);
  lv_obj_set_style_text_color(lb_mpage_blk_hit, font_color, LV_PART_MAIN); 
  lv_label_set_long_mode(lb_mpage_blk_hit, LV_LABEL_LONG_DOT);
  lv_obj_align( lb_mpage_blk_hit, LV_ALIGN_TOP_MID, lb_blk_hit_coord[0], lb_blk_hit_coord[1]); 
  //temp value
  font_color = lv_color_hex(0xFFFFFF);
  lb_mpage_temp_or_time_value   = lv_label_create( miner_page );
  lv_obj_set_width(lb_mpage_temp_or_time_value, SCREEN_WIDTH);
  lv_label_set_text( lb_mpage_temp_or_time_value, " ");
  lv_obj_set_style_text_font(lb_mpage_temp_or_time_value, lb_temp_or_time_font, LV_PART_MAIN);
  lv_obj_set_style_text_color(lb_mpage_temp_or_time_value, font_color, LV_PART_MAIN); 
  lv_label_set_long_mode(lb_mpage_temp_or_time_value, LV_LABEL_LONG_DOT);
  lv_obj_align( lb_mpage_temp_or_time_value, LV_ALIGN_TOP_MID, lb_temp_or_time_coord[0], lb_temp_or_time_coord[1]); 
  //uptime days
  font_color = lv_color_hex(0xFFFFFF);
  lb_mpage_uptime_day_value = lv_label_create( miner_page );
  lv_obj_set_width(lb_mpage_uptime_day_value, SCREEN_WIDTH);
  lv_label_set_text( lb_mpage_uptime_day_value, " ");
  lv_obj_set_style_text_font(lb_mpage_uptime_day_value, lb_uptime_font, LV_PART_MAIN);
  lv_obj_set_style_text_color(lb_mpage_uptime_day_value, font_color, LV_PART_MAIN); 
  lv_label_set_long_mode(lb_mpage_uptime_day_value, LV_LABEL_LONG_DOT);
  lv_obj_align( lb_mpage_uptime_day_value, LV_ALIGN_TOP_LEFT, lb_uptime_day_coord[0], lb_uptime_day_coord[1]); 
  //uptime days unit
  font_color = lv_color_hex(0xEE7D30);
  lb_mpage_uptime_day_unit_value = lv_label_create( miner_page );
  lv_obj_set_width(lb_mpage_uptime_day_unit_value, 20);
  lv_label_set_text( lb_mpage_uptime_day_unit_value, "d");
  lv_obj_set_style_text_font(lb_mpage_uptime_day_unit_value, lb_uptime_unit_font, LV_PART_MAIN);
  lv_label_set_long_mode(lb_mpage_uptime_day_unit_value, LV_LABEL_LONG_DOT);
  lv_obj_set_style_text_color(lb_mpage_uptime_day_unit_value, font_color, LV_PART_MAIN); 
  lv_obj_align( lb_mpage_uptime_day_unit_value, LV_ALIGN_TOP_LEFT, lb_uptime_day_unit_coord[0], lb_uptime_day_unit_coord[1]); 
  //uptime value, hour , minute, second
  font_color = lv_color_hex(0xFFFFFF);
  lb_mpage_uptime_hms_value    = lv_label_create( miner_page );
  lv_obj_set_width(lb_mpage_uptime_hms_value, 150);
  lv_label_set_text( lb_mpage_uptime_hms_value, " ");
  lv_obj_set_style_text_font(lb_mpage_uptime_hms_value, lb_uptime_font, LV_PART_MAIN);
  lv_label_set_long_mode(lb_mpage_uptime_hms_value, LV_LABEL_LONG_DOT);
  lv_obj_set_style_text_color(lb_mpage_uptime_hms_value, font_color, LV_PART_MAIN); 
  lv_obj_align( lb_mpage_uptime_hms_value, LV_ALIGN_TOP_LEFT, lb_uptime_hms_coord[0], lb_uptime_hms_coord[1] );

  //version in miner page 
  font_color = lv_color_hex(0xF0F0F0);
  lb_mpage_version    = lv_label_create( miner_page );
  lv_obj_set_width(lb_mpage_version, 150);
  lv_label_set_text( lb_mpage_version, g_nm.board.fw_version.substring(1, g_nm.board.fw_version.length()).c_str());
  lv_obj_set_style_text_font(lb_mpage_version, lb_miner_version_font, LV_PART_MAIN);
  lv_label_set_long_mode(lb_mpage_version, LV_LABEL_LONG_DOT);
  lv_obj_set_style_text_color(lb_mpage_version, font_color, LV_PART_MAIN); 
  lv_obj_align( lb_mpage_version, LV_ALIGN_TOP_LEFT, lb_mine_page_ver_coord[0], lb_mine_page_ver_coord[1] );

  //wifi ip
  font_color = lv_color_hex(0xFFFFFF);
  lb_mpage_ip_value   = lv_label_create( miner_page );
  lv_obj_set_width(lb_mpage_ip_value, 250);
  lv_label_set_text( lb_mpage_ip_value, " ");
  lv_obj_set_style_text_font(lb_mpage_ip_value, lb_wifi_ip_font, LV_PART_MAIN);
  lv_obj_set_style_text_color(lb_mpage_ip_value, font_color, LV_PART_MAIN); 
  lv_label_set_long_mode(lb_mpage_ip_value, LV_LABEL_LONG_DOT);
  lv_obj_align( lb_mpage_ip_value, LV_ALIGN_TOP_LEFT, lb_wifi_ip_coord[0], lb_wifi_ip_coord[1]);  
  //wifi rssi
  font_color = lv_color_hex(0xFFFFFF);
  lb_mpage_rssi_value   = lv_label_create( miner_page );
  lv_obj_set_width(lb_mpage_rssi_value, SCREEN_WIDTH);
  lv_label_set_text( lb_mpage_rssi_value, " ");
  lv_obj_set_style_text_font(lb_mpage_rssi_value, lb_wifi_rssi_font, LV_PART_MAIN);
  lv_obj_set_style_text_color(lb_mpage_rssi_value, font_color, LV_PART_MAIN); 
  lv_label_set_long_mode(lb_mpage_rssi_value, LV_LABEL_LONG_DOT);
  lv_obj_align( lb_mpage_rssi_value, LV_ALIGN_TOP_LEFT, lb_wifi_rssi_coord[0], lb_wifi_rssi_coord[1]);  

  //btc price
  font_color = lv_color_hex(0xFFFFFF);
  lb_mpage_price_value   = lv_label_create( miner_page );
  lv_obj_set_width(lb_mpage_price_value, SCREEN_WIDTH);
  lv_label_set_text( lb_mpage_price_value, " ");
  lv_obj_set_style_text_font(lb_mpage_price_value, lb_mpage_price_font, LV_PART_MAIN);
  lv_obj_set_style_text_color(lb_mpage_price_value, font_color, LV_PART_MAIN); 
  lv_label_set_long_mode(lb_mpage_price_value, LV_LABEL_LONG_SCROLL_CIRCULAR);
  lv_obj_align( lb_mpage_price_value, LV_ALIGN_TOP_LEFT, lb_price_coord[0], lb_price_coord[1]);

  // symbol wifi rssi
  font_color = lv_color_hex(0xEE7D30);
  lb_mpage_rssi_symbol   = lv_label_create( miner_page );
  lv_obj_set_width(lb_mpage_rssi_symbol, 30);
  lv_label_set_text( lb_mpage_rssi_symbol, LV_SYMBOL_WIFI);
  lv_obj_set_style_text_font(lb_mpage_rssi_symbol, lb_wifi_rssi_symbol_font, LV_PART_MAIN);
  lv_obj_set_style_text_color(lb_mpage_rssi_symbol, font_color, LV_PART_MAIN); 
  lv_label_set_long_mode(lb_mpage_rssi_symbol, LV_LABEL_LONG_DOT);
  lv_obj_align( lb_mpage_rssi_symbol, LV_ALIGN_TOP_LEFT, lb_wifi_rssi_symbol_coord[0], lb_wifi_rssi_symbol_coord[1]); 
  
  //job received value
  font_color = lv_color_hex(0xFFFFFF);
  lb_mpage_job_rec_value   = lv_label_create( miner_page );
  lv_obj_set_width(lb_mpage_job_rec_value, SCREEN_WIDTH);
  lv_label_set_text( lb_mpage_job_rec_value, " ");
  lv_obj_set_style_text_font(lb_mpage_job_rec_value, lb_job_received_font, LV_PART_MAIN);
  lv_obj_set_style_text_color(lb_mpage_job_rec_value, font_color, LV_PART_MAIN); 
  lv_label_set_long_mode(lb_mpage_job_rec_value, LV_LABEL_LONG_DOT);
  lv_obj_align( lb_mpage_job_rec_value, LV_ALIGN_TOP_MID, lb_job_received_coord[0], lb_job_received_coord[1]); 
  //net diff value
  font_color = lv_color_hex(0xFFFFFF);
  lb_mpage_net_diff_value   = lv_label_create( miner_page );
  lv_obj_set_width(lb_mpage_net_diff_value, SCREEN_WIDTH);
  lv_label_set_text( lb_mpage_net_diff_value, " ");
  lv_obj_set_style_text_font(lb_mpage_net_diff_value, lb_net_diff_font, LV_PART_MAIN);
  lv_obj_set_style_text_color(lb_mpage_net_diff_value, font_color, LV_PART_MAIN); 
  lv_label_set_long_mode(lb_mpage_net_diff_value, LV_LABEL_LONG_DOT);
  lv_obj_align( lb_mpage_net_diff_value, LV_ALIGN_TOP_MID, lb_net_diff_coord[0], lb_net_diff_coord[1]); 
  //local diff value
  font_color = lv_color_hex(0xFFFFFF);
  lb_mpage_local_diff_value   = lv_label_create( miner_page );
  lv_obj_set_width(lb_mpage_local_diff_value, SCREEN_WIDTH);
  lv_label_set_text( lb_mpage_local_diff_value, " ");
  lv_obj_set_style_text_font(lb_mpage_local_diff_value, lb_local_diff_font, LV_PART_MAIN);
  lv_obj_set_style_text_color(lb_mpage_local_diff_value, font_color, LV_PART_MAIN); 
  lv_label_set_long_mode(lb_mpage_local_diff_value, LV_LABEL_LONG_DOT);
  lv_obj_align( lb_mpage_local_diff_value, LV_ALIGN_TOP_MID, lb_local_diff_coord[0], lb_local_diff_coord[1]); 
  //shares value  
  font_color = lv_color_hex(0xFFFFFF);
  lb_mpage_shares_value   = lv_label_create( miner_page );
  lv_obj_set_width(lb_mpage_shares_value, SCREEN_WIDTH);
  lv_label_set_text( lb_mpage_shares_value, " ");
  lv_obj_set_style_text_font(lb_mpage_shares_value, lb_shares_font, LV_PART_MAIN);
  lv_obj_set_style_text_color(lb_mpage_shares_value, font_color, LV_PART_MAIN); 
  lv_label_set_long_mode(lb_mpage_shares_value, LV_LABEL_LONG_DOT);
  lv_obj_align( lb_mpage_shares_value, LV_ALIGN_TOP_MID, lb_shares_coord[0], lb_shares_coord[1]); 

  //job received symbol
  font_color = lv_color_hex(0xA9A9A9);
  lb_mpage_job_receive_symbol   = lv_label_create( miner_page );
  lv_obj_set_width(lb_mpage_job_receive_symbol, 30);
  lv_label_set_text( lb_mpage_job_receive_symbol, "\xEF\x80\x99");
  lv_obj_set_style_text_font(lb_mpage_job_receive_symbol, lb_job_receive_symbol_font, LV_PART_MAIN);
  lv_obj_set_style_text_color(lb_mpage_job_receive_symbol, font_color, LV_PART_MAIN); 
  lv_label_set_long_mode(lb_mpage_job_receive_symbol, LV_LABEL_LONG_DOT);
  lv_obj_align( lb_mpage_job_receive_symbol, LV_ALIGN_TOP_LEFT, lb_job_receive_symbol_coord[0], lb_job_receive_symbol_coord[1]); 
  //net diff symbol
  font_color = lv_color_hex(0xA9A9A9);
  lb_mpage_net_diff_symbol   = lv_label_create( miner_page );
  lv_obj_set_width(lb_mpage_net_diff_symbol, 30);
  lv_label_set_text( lb_mpage_net_diff_symbol, "\xEF\x95\xBD");
  lv_obj_set_style_text_font(lb_mpage_net_diff_symbol, lb_net_diff_symbol_font, LV_PART_MAIN);
  lv_obj_set_style_text_color(lb_mpage_net_diff_symbol, font_color, LV_PART_MAIN); 
  lv_label_set_long_mode(lb_mpage_net_diff_symbol, LV_LABEL_LONG_DOT);
  lv_obj_align( lb_mpage_net_diff_symbol, LV_ALIGN_TOP_LEFT, lb_net_diff_symbol_coord[0], lb_net_diff_symbol_coord[1]); 
  //local diff symbol
  font_color = lv_color_hex(0xA9A9A9);
  lb_mpage_local_diff_symbol   = lv_label_create( miner_page );
  lv_obj_set_width(lb_mpage_local_diff_symbol, 30);
  lv_label_set_text( lb_mpage_local_diff_symbol, "\xEF\x82\x80");
  lv_obj_set_style_text_font(lb_mpage_local_diff_symbol, lb_local_diff_symbol_font, LV_PART_MAIN);
  lv_obj_set_style_text_color(lb_mpage_local_diff_symbol, font_color, LV_PART_MAIN); 
  lv_label_set_long_mode(lb_mpage_local_diff_symbol, LV_LABEL_LONG_DOT);
  lv_obj_align( lb_mpage_local_diff_symbol, LV_ALIGN_TOP_LEFT, lb_local_diff_symbol_coord[0], lb_local_diff_symbol_coord[1]); 
  // share symbol
  font_color = lv_color_hex(0xA9A9A9);
  lb_mpage_share_symb   = lv_label_create( miner_page );
  lv_obj_set_width(lb_mpage_share_symb, 30);
  lv_label_set_text( lb_mpage_share_symb, "\xEF\x8E\x82");
  lv_obj_set_style_text_font(lb_mpage_share_symb, lb_share_symb_font, LV_PART_MAIN);
  lv_obj_set_style_text_color(lb_mpage_share_symb, font_color, LV_PART_MAIN); 
  lv_label_set_long_mode(lb_mpage_share_symb, LV_LABEL_LONG_DOT);
  lv_obj_align( lb_mpage_share_symb, LV_ALIGN_TOP_LEFT, lb_share_symb_coord[0], lb_share_symb_coord[1]); 
  //////////////////////////////////////clock page layout///////////////////////////////////////////////
  //version
  font_color = lv_color_hex(0xB0B0B0 );
  lb_clock_page_version  = lv_label_create( clock_page );
  String fw_version = g_nm.board.fw_version.substring(1, g_nm.board.fw_version.length());
  width = lv_txt_get_width(fw_version.c_str(), fw_version.length(), lb_clock_version_font, 0, LV_TEXT_FLAG_NONE);
  lv_obj_set_width(lb_clock_page_version, width);
  lv_label_set_text( lb_clock_page_version, fw_version.c_str());
  lv_obj_set_style_text_font(lb_clock_page_version, lb_clock_version_font, LV_PART_MAIN);
  lv_obj_set_style_text_color(lb_clock_page_version, font_color, LV_PART_MAIN); 
  lv_label_set_long_mode(lb_clock_page_version, LV_LABEL_LONG_SCROLL_CIRCULAR);
  lv_obj_align( lb_clock_page_version, LV_ALIGN_BOTTOM_LEFT, 0,0);
  //clock day
  font_color = lv_color_hex(0xB0B0B0 );
  lb_clock_page_day_value   = lv_label_create( clock_page );
  lv_obj_set_width(lb_clock_page_day_value, SCREEN_WIDTH);
  lv_label_set_text( lb_clock_page_day_value, " ");
  lv_obj_set_style_text_font(lb_clock_page_day_value, lb_clock_day_font, LV_PART_MAIN);
  lv_obj_set_style_text_color(lb_clock_page_day_value, font_color, LV_PART_MAIN); 
  lv_label_set_long_mode(lb_clock_page_day_value, LV_LABEL_LONG_SCROLL_CIRCULAR);
  lv_obj_align( lb_clock_page_day_value, LV_ALIGN_BOTTOM_RIGHT, lb_clock_day_coord[0], lb_clock_day_coord[1]);  
   

  //clock hms
  font_color = lv_color_hex(0xFFFFFF);
  lb_clock_page_hms_value   = lv_label_create( clock_page );
  width  = lv_txt_get_width("00:00:00 AM", strlen("00:00:00 AM"), lb_clock_hms_font, 0, LV_TEXT_FLAG_NONE);
  lv_obj_set_width(lb_clock_page_hms_value, width);
  lv_label_set_text( lb_clock_page_hms_value, " ");
  lv_obj_set_style_text_font(lb_clock_page_hms_value, lb_clock_hms_font, LV_PART_MAIN);
  lv_obj_set_style_text_color(lb_clock_page_hms_value, font_color, LV_PART_MAIN); 
  lv_label_set_long_mode(lb_clock_page_hms_value, LV_LABEL_LONG_SCROLL_CIRCULAR);
  lv_obj_align(lb_clock_page_hms_value, LV_ALIGN_CENTER, lb_clock_hms_coord[0], lb_clock_hms_coord[1]);  

  //hash rate in clock page
  font_color = lv_color_hex(0xEE7D30);
  lb_clock_page_hr_value   = lv_label_create( clock_page );
  lv_obj_set_width(lb_clock_page_hr_value, SCREEN_WIDTH);
  lv_label_set_text( lb_clock_page_hr_value, " ");
  lv_obj_set_style_text_font(lb_clock_page_hr_value, lb_clock_hr_font, LV_PART_MAIN);
  lv_obj_set_style_text_color(lb_clock_page_hr_value, font_color, LV_PART_MAIN); 
  lv_label_set_long_mode(lb_clock_page_hr_value, LV_LABEL_LONG_DOT);
  lv_obj_align( lb_clock_page_hr_value, LV_ALIGN_TOP_MID, lb_clock_hr_coord[0], lb_clock_hr_coord[1]);

  //hash rate unit in clock page
  font_color = lv_color_hex(0xB0B0B0 );
  lb_clock_page_hr_unit   = lv_label_create( clock_page );
  width = lv_txt_get_width(String("kH/s").c_str(), strlen(String("kH/s").c_str()), lb_clock_hr_unit_font, 0, LV_TEXT_FLAG_NONE);
  lv_obj_set_width(lb_clock_page_hr_unit, width);
  lv_label_set_text( lb_clock_page_hr_unit, "kH/s");
  lv_obj_set_style_text_font(lb_clock_page_hr_unit, lb_clock_hr_unit_font, LV_PART_MAIN);
  lv_obj_set_style_text_color(lb_clock_page_hr_unit, font_color, LV_PART_MAIN); 
  lv_label_set_long_mode(lb_clock_page_hr_unit, LV_LABEL_LONG_DOT);
  lv_obj_align( lb_clock_page_hr_unit, LV_ALIGN_TOP_RIGHT, lb_clock_hr_unit_coord[0], lb_clock_hr_unit_coord[1]); 
}

static void ui_update_loading_string(String str, uint32_t color, bool prgress_update) {
    static uint8_t progress = 0, progress_total = 8;

    lv_color_t font_color = lv_color_hex(color);
    if (lb_lpage_details_str == NULL){
        //loading status string
        lb_lpage_details_str = lv_label_create(loading_page);
        lv_obj_set_width(lb_lpage_details_str, SCREEN_WIDTH);
        lv_obj_set_style_text_font(lb_lpage_details_str, lb_loading_font, LV_PART_MAIN);
        lv_label_set_long_mode(lb_lpage_details_str, LV_LABEL_LONG_DOT);
        lv_obj_align(lb_lpage_details_str, LV_ALIGN_BOTTOM_LEFT, 0, 0);
        //bar 
        lb_lpage_bar = lv_bar_create(loading_page);
        lv_bar_set_range(lb_lpage_bar, 0, progress_total);
        lv_bar_set_value(lb_lpage_bar, progress, LV_ANIM_ON);
        lv_obj_set_size(lb_lpage_bar, SCREEN_WIDTH * 0.9, 3);
        lv_obj_align(lb_lpage_bar, LV_ALIGN_CENTER, 0, -20);
        lv_obj_set_style_bg_color(lb_lpage_bar, lv_color_hex(0xFFFFFF), LV_PART_INDICATOR);
        lv_obj_set_style_bg_opa(lb_lpage_bar, LV_OPA_COVER, LV_PART_INDICATOR);

        //progress label
        lb_lpage_progress_value = lv_label_create(loading_page);
        lv_label_set_text(lb_lpage_progress_value, "");
        lv_obj_set_style_text_font(lb_lpage_progress_value, lb_loading_progress_font, LV_PART_MAIN); 
        lv_obj_align(lb_lpage_progress_value, LV_ALIGN_LEFT_MID, 30, -10);

        //version check result
        lb_version_check   = lv_label_create( loading_page );
        lv_label_set_text( lb_version_check, "");
        lv_obj_set_style_text_font(lb_version_check, lb_loading_version_check_font, LV_PART_MAIN);
        lv_obj_set_style_text_color(lb_version_check, font_color, LV_PART_MAIN); 
        lv_label_set_long_mode(lb_version_check, LV_LABEL_LONG_SCROLL_CIRCULAR);
        lv_obj_align( lb_version_check, LV_ALIGN_CENTER, lb_version_check_loading_coord[0], lb_version_check_loading_coord[1]);

        //flash address
        lb_flash_addr   = lv_label_create( loading_page );
        lv_label_set_text( lb_flash_addr, "");
        lv_obj_set_style_text_font(lb_flash_addr, lb_loading_version_check_font, LV_PART_MAIN);
        lv_obj_set_style_text_color(lb_flash_addr, font_color, LV_PART_MAIN); 
        lv_label_set_long_mode(lb_flash_addr, LV_LABEL_LONG_SCROLL_CIRCULAR);
        lv_obj_align( lb_flash_addr, LV_ALIGN_CENTER, lb_flash_addr_loading_coord[0], lb_flash_addr_loading_coord[1]);
    }

    //版本检测提示
    int8_t res = compareVersions(g_nm.board.fw_version, g_nm.board.fw_latest_release);
    String reminder = "";
    lv_color_t ck_color = lv_color_hex(0xFFFFFF);
    if(-1 == res){
      ck_color = lv_color_hex(0x00FF00);
      reminder = "New firmware available : " + g_nm.board.fw_latest_release;
      lv_coord_t width = lv_txt_get_width(reminder.c_str(), strlen(reminder.c_str()), lb_loading_version_check_font, 0, LV_TEXT_FLAG_NONE);
      lv_obj_set_width(lb_version_check, (width > SCREEN_WIDTH) ? SCREEN_WIDTH : width);
      lv_label_set_text( lb_version_check, reminder.c_str());
      lv_obj_set_style_text_font(lb_version_check, lb_loading_version_check_font, LV_PART_MAIN);
      lv_obj_set_style_text_color(lb_version_check, ck_color, LV_PART_MAIN); 

      ck_color = lv_color_hex(0xFFFFFF);
      reminder = "https://flash.nmiot.com";
      width = lv_txt_get_width(reminder.c_str(), strlen(reminder.c_str()), lb_loading_version_check_font, 0, LV_TEXT_FLAG_NONE);
      lv_obj_set_width(lb_flash_addr, (width > SCREEN_WIDTH) ? SCREEN_WIDTH : width);
      lv_label_set_text( lb_flash_addr, reminder.c_str());
      lv_obj_set_style_text_font(lb_flash_addr, lb_loading_version_check_font, LV_PART_MAIN);
      lv_obj_set_style_text_color(lb_flash_addr, ck_color, LV_PART_MAIN); 
    }

    //loading进度条
    if(prgress_update){
      lv_coord_t bar_x = lv_obj_get_x(lb_lpage_bar);
      lv_coord_t bar_w = lv_obj_get_width(lb_lpage_bar);
      lv_coord_t label_x = bar_x + (bar_w * progress / progress_total) - lv_obj_get_width(lb_lpage_progress_value) / 2;
      lv_obj_set_pos(lb_lpage_progress_value, label_x, -10);

      lv_obj_set_style_text_color(lb_lpage_progress_value, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
      lv_label_set_text(lb_lpage_progress_value, (String(100 * (float)progress/(float)progress_total, 0) + "%").c_str());
      lv_bar_set_value(lb_lpage_bar, progress, LV_ANIM_ON);
      progress++;
    }
    lv_obj_set_style_text_color(lb_lpage_details_str, font_color, LV_PART_MAIN);
    lv_label_set_text(lb_lpage_details_str, str.c_str());
}

static void ui_saver_page_init(){

}

static void ui_miner_page_refresh(){
  
}

static void ui_clock_page_refresh(){

}


#if defined(HAS_SCREEN_SAVER_FEATURE)
static bool saver_init = false;
static void ui_saver_page_refresh(){
  #define STAR_COUNT   20               // 星星数量
  static uint8_t MAX_DEPTH  = 10;       // 最大深度层级
  static uint8_t MIN_STAR_SIZE = 1;     // 最小星星尺寸
  static uint8_t MAX_STAR_SIZE = 3;     // 最大星星尺寸
  static star_t  stars[STAR_COUNT];     // 星星数组
  static lv_obj_t *bg_img = NULL;       // 背景图
  static lv_style_t style;


  if (saver_scr == NULL) {
    // 1. 创建屏保图层
    saver_scr = lv_obj_create(lv_scr_act());
    lv_obj_set_size(saver_scr, LV_HOR_RES, LV_VER_RES);
    lv_obj_align(saver_scr, LV_ALIGN_CENTER, 0, 0);

    // 2. 禁用滚动条和圆角
    lv_obj_set_scrollbar_mode(saver_scr, LV_SCROLLBAR_MODE_OFF);

    // 3. 设置屏保图层样式（半透明黑背景）
    lv_style_init(&style);
    lv_style_set_radius(&style, 0);              // 禁用圆角
    lv_style_set_bg_color(&style, lv_color_black());
    lv_style_set_bg_opa(&style, LV_OPA_70);
    lv_style_set_border_width(&style, 0);
    lv_style_set_border_opa(&style, LV_OPA_TRANSP);
    lv_obj_add_style(saver_scr, &style, LV_PART_MAIN);

    // 4. 创建背景图片
    bg_img = lv_img_create(saver_scr);  // 图片作为 saver_scr 的子对象
    lv_img_set_src(bg_img, &saver_img);  
    lv_obj_align(bg_img, LV_ALIGN_CENTER, 0, 0);  // 居中显示
    lv_obj_move_background(bg_img);  // 确保图片在星星之下
  }

  lv_coord_t width  = lv_obj_get_width(saver_scr);
  lv_coord_t height = lv_obj_get_height(saver_scr);

  if(width != LV_HOR_RES || height != LV_VER_RES) return;

  if(!saver_init){
    for(int i = 0; i < STAR_COUNT; i++) {
      stars[i].obj = NULL;
      stars[i].active = false;
      reset_star(&stars[i], saver_scr);
    }
    saver_init = true;
  }

  //更新星星
  for(int i = 0; i < STAR_COUNT; i++) {
      update_star(&stars[i], saver_scr);
  }
}
void ui_delete_screensaver() {
  if(saver_scr != NULL) {
      lv_obj_del(saver_scr);  // 删除对象及其所有子对象
      saver_scr = NULL;       // 重置指针避免野指针
      saver_init = false;     // 重置初始化状态
  }
}
#endif

static void ui_switch_to_page(uint8_t page, bool anim){
  g_page_index = page;
  if(anim) lv_obj_scroll_to_view(g_pages[g_page_index], LV_ANIM_ON);
}

static void ui_refresh_thread(void *args){
  char *name = (char*)malloc(20);
  strcpy(name, (char*)args);
  LOG_I("%s thread started on core %d...", name, xPortGetCoreID());
  free(name);


  bool saver_active = false;
  uint32_t freq = 1000 * g_nm.screen.refresh_interval;
 
  while (true){
    uint32_t start = millis();
    
#if defined(HAS_SCREEN_SAVER_FEATURE)
    if((g_nm.minerstatus.job_received > 0) && (g_nm.screen.saver_enable == true)){
      saver_active = (start - g_nm.screen.last_operaion > 1000 * 60*10);
    }
#endif

    freq = (saver_active) ? 50 : 1000 * g_nm.screen.refresh_interval;
    while(millis() - start < freq){
      delay(50);
    }
    

    if(xSemaphoreTake(lvgl_xMutex, 0) == pdTRUE){
#if defined(HAS_SCREEN_SAVER_FEATURE)
      if(saver_active)                    ui_saver_page_refresh();
      else                                ui_delete_screensaver();
#endif
      if(g_page_index == PAGE_MINER)      ui_miner_page_refresh();
      else if(g_page_index == PAGE_CLOCK) ui_clock_page_refresh();
#if defined(HAS_GAUGE_FEATURE)
      else if(g_page_index == PAGE_GAUGE || g_page_index == PAGE_CONFIG) ui_gauge_page_refresh();
#endif
      //release mutex
      xSemaphoreGive(lvgl_xMutex); 
    }


#ifdef TFT_DISPLAY
    if(TFT_BL!=-1){
      #ifdef HAS_PHOTORESISTOR_FEATURE
      //更新背光亮度, 亮度随息屏时间递减, 为零时常亮
      if(g_nm.screen.auto_brightness_adjust){
        static uint16_t adc_max = 256;
        int adc = ((g_nm.screen.reg_adc + 9) / 10) * 10;
        int ladder = adc_max - adc;
        ladder = ladder > 0 ? (ladder): 0;
        ladder = ladder > adc_max ? adc_max: ladder;

        LOG_D("ladder : %d/%d, brightness: %f", ladder, adc_max, (float)ladder / (float)adc_max);
        float brightness = (float)ladder / (float)adc_max;
        brightness = (brightness > 0.01f) ? brightness : 0.01f;
        tft_bl_ctrl(brightness);
      }else{
        if(g_nm.screen.sleep_timeout > 0){
            int cycle = (g_nm.screen.sleep_timeout - g_nm.screen.cnt);
            cycle = cycle > 0 ? cycle : 0;
            uint8_t full = (uint8_t)(((float)g_nm.screen.brightness/100.0f) * 255);//从设定的这个亮度开始逐渐变暗
            cycle = cycle * full / g_nm.screen.sleep_timeout;
            tft_bl_ctrl((float)cycle / 255.0f);
            g_nm.screen.active = cycle > 0;
        }else {
          tft_bl_ctrl((float)(g_nm.screen.brightness/100.0f));
        }
      }
      #else
      if(g_nm.screen.sleep_timeout > 0){
        int cycle = (g_nm.screen.sleep_timeout - g_nm.screen.cnt);
        cycle = cycle > 0 ? cycle : 0;
        uint8_t full = (uint8_t)(((float)g_nm.screen.brightness/100.0f) * 255);//从设定的这个亮度开始逐渐变暗
        cycle = cycle * full / g_nm.screen.sleep_timeout;
        tft_bl_ctrl((float)cycle / 255.0f);
        g_nm.screen.active = cycle > 0;
      }else {
        tft_bl_ctrl((float)(g_nm.screen.brightness/100.0f));
      }
      #endif
    }
#elif defined(AMOLED_DISPLAY)
    if(g_nm.screen.sleep_timeout > 0){
      int cycle = (g_nm.screen.sleep_timeout - g_nm.screen.cnt);
      cycle = cycle > 0 ? cycle : 0;
      uint8_t full = (uint8_t)(((float)g_nm.screen.brightness/100.0f) * 255);//从设定的这个亮度开始逐渐变暗
      cycle = cycle * full / g_nm.screen.sleep_timeout;
      tft_bl_ctrl((float)cycle / 255.0f);
      g_nm.screen.active = cycle > 0;
    }else {
      tft_bl_ctrl(0.99f);
    }
#endif
  }
}

void ui_switch_next_page_cb(){
#if defined(HAS_GAUGE_FEATURE)
  if(g_nm.minerstatus.hashrate.total <= 0) {
    if(g_nm.need_cfg) {
      static bool first_time = true;
      if(first_time) {
        g_page_index = PAGE_CONFIG;
        first_time = false;
      }else {
        g_page_index = (g_page_index == PAGE_GAUGE) ? PAGE_CONFIG : PAGE_GAUGE;
      }
    }else{
      g_page_index = (g_page_index == PAGE_GAUGE) ? PAGE_LOADING : PAGE_GAUGE;
    }
  }
  else {
    //在miner页面和gauge页面clokc页面之间切换
    static bool page_direction = false;
      if (page_direction == false) {
        if (g_page_index == PAGE_GAUGE) {
            g_page_index = PAGE_MINER;
        } else if (g_page_index == PAGE_MINER) {
            g_page_index = PAGE_CLOCK;
        } else if (g_page_index == PAGE_CLOCK) {
            g_page_index = PAGE_MINER;
            page_direction = true;
        }else {
            g_page_index = PAGE_GAUGE;
        }
      } else if (page_direction == true) {
          if (g_page_index == PAGE_MINER) {
              g_page_index = PAGE_GAUGE;
              page_direction = false;
          }
      }
  }
#else
  g_page_index = (g_page_index == PAGE_MINER) ? PAGE_CLOCK : PAGE_MINER;//在miner页面和clock页面之间切换
#endif
  uint8_t default_interval = nvs_config_get_u8(NMTV_SETTINGS_NAMESPACE, JSON_SPIFFS_KEY_REFRESHINTERV, DEFAULT_REFRESH_INTERVAL);

  g_nm.screen.refresh_interval = (g_page_index == PAGE_CLOCK || g_page_index == PAGE_GAUGE) ? 1 : default_interval;

  ui_switch_to_page(g_page_index, true);

  g_nm.screen.last_operaion = millis();
}

void display_thread(void *args){
  char *name = (char*)malloc(20);
  strcpy(name, (char*)args);
  LOG_I("%s thread started on core %d...", name, xPortGetCoreID());
  free(name);
  uint8_t cnt  = 0;
  screen_init();

  lv_init();

  ui_init();

  ui_layout_init();
  
  //lvgl tick task
  String taskName = "(lvgltick)";
  xTaskCreatePinnedToCore(lvgl_tick_task, taskName.c_str(), 1024*3.3, (void*)taskName.c_str(), TASK_PRIORITY_LVGL_DRV, &task_lvgl_tick, LvglTaskCore);
  delay(500);//wait a bit for lvgl tick task to start, necessary for lvgl to work properly

  taskName = "(uirefresh)";
  xTaskCreatePinnedToCore(ui_refresh_thread, taskName.c_str(), 1024*2.5, (void*)taskName.c_str(), TASK_PRIORITY_UI_REFRESH, &task_ui_refresh, UiRefreshTaskCore);
  delay(500);

  ui_switch_to_page(PAGE_LOADING, true);

  /***************************************switch to miner page*************************************/
  ui_switch_to_page(PAGE_MINER, true);
  g_nm.screen.last_operaion = millis();
  vTaskDelete(NULL);
}
#endif //TFT_DISPLAY
