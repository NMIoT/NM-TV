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
  PAGE_PRICE_RANK,
  PAGE_PRICE_DETAIL,
  PAGE_CLOCK,
  PAGE_WEATHER,
  PAGE_END
};

/*********************************************************************全局lvgl对象********************************************************************/
static lv_color_t          *lvgl_color_buf = NULL;
static lv_disp_draw_buf_t   draw_buf;
static SemaphoreHandle_t    lvgl_xMutex;
static lv_obj_t *parent_docker = NULL;
static lv_obj_t *g_pages[] = {NULL, NULL, NULL, NULL, NULL, NULL};
static lv_obj_t *loading_page = NULL, *config_page = NULL, *price_page = NULL, *weather_page = NULL, *clock_page = NULL;
static uint16_t g_page_index = PAGE_PRICE_RANK;

#include "image_240_240.h"
// LV_FONT_DECLARE(ds_digib_font_10)
// LV_FONT_DECLARE(ds_digib_font_12)
// LV_FONT_DECLARE(ds_digib_font_14)
// LV_FONT_DECLARE(ds_digib_font_16)
// LV_FONT_DECLARE(ds_digib_font_18)
// LV_FONT_DECLARE(ds_digib_font_22)
// LV_FONT_DECLARE(ds_digib_font_24)
// LV_FONT_DECLARE(ds_digib_font_28)
// LV_FONT_DECLARE(ds_digib_font_34)
// LV_FONT_DECLARE(ds_digib_font_36)
// LV_FONT_DECLARE(ds_digib_font_50)
// LV_FONT_DECLARE(ds_digib_font_54)
// LV_FONT_DECLARE(ds_digib_font_58)
// LV_FONT_DECLARE(symbol_10)
// LV_FONT_DECLARE(symbol_14)
// LV_FONT_DECLARE(symbol_18)
// LV_FONT_DECLARE(symbol_20)

static const lv_font_t *lb_price_rank_font = &lv_font_montserrat_34;
// static const lv_font_t *lb_loading_slogan_font    = &lv_font_montserrat_18;
// static const lv_font_t *lb_loading_version_check_font = &lv_font_montserrat_14;
// static const lv_font_t *lb_cfg_timeout_font  = &lv_font_montserrat_18;
// static const lv_font_t *lb_version_loading_font  = &lv_font_montserrat_18;
// static const lv_font_t *lb_cfg_version_font  = &lv_font_montserrat_18;
// static const lv_font_t *lb_loading_font  = &lv_font_montserrat_18;
// static const lv_font_t *lb_hashrate_font = &ds_digib_font_36;
// static const lv_font_t *lb_blk_hit_font  = &ds_digib_font_58;
// static const lv_font_t *lb_temp_or_time_font     = &ds_digib_font_14;
// static const lv_font_t *lb_wifi_ip_font  = &ds_digib_font_18;
// static const lv_font_t *lb_wifi_rssi_font = &ds_digib_font_18;
// static const lv_font_t *lb_mPAGE_PRICE_RANK_font     = &ds_digib_font_16;
// static const lv_font_t *lb_uptime_font       = &ds_digib_font_18;
// static const lv_font_t *lb_uptime_unit_font  = &lv_font_montserrat_16;
// static const lv_font_t *lb_wifi_rssi_symbol_font = &lv_font_montserrat_14;
// static const lv_font_t *lb_price_symbol_font     = &symbol_14;

// static const lv_font_t *lb_job_received_font = &ds_digib_font_22;
// static const lv_font_t *lb_net_diff_font     = &ds_digib_font_22;
// static const lv_font_t *lb_local_diff_font   = &ds_digib_font_22;
// static const lv_font_t *lb_shares_font       = &ds_digib_font_22;

// static const lv_font_t *lb_clock_pool_font       = &lv_font_montserrat_18;
// static const lv_font_t *lb_clock_price_font      = &ds_digib_font_36;
// static const lv_font_t *lb_miner_version_font    = &ds_digib_font_16;

// static const lv_font_t *lb_job_receive_symbol_font        = &symbol_18;
// static const lv_font_t *lb_net_diff_symbol_font           = &symbol_18;
// static const lv_font_t *lb_local_diff_symbol_font         = &symbol_18;
// static const lv_font_t *lb_share_symb_font                = &symbol_18;

// static const lv_font_t *lb_clock_hr_font                  = &ds_digib_font_36;
// static const lv_font_t *lb_clock_hr_unit_font             = &lv_font_montserrat_14;
// static const lv_font_t *lb_clock_version_font             = &ds_digib_font_12;
// static const lv_font_t *lb_clock_day_font                 = &ds_digib_font_24;
// static const lv_font_t *lb_clock_hms_font                 = &ds_digib_font_54;
// static const lv_font_t *lb_clock_price_changed_font       = &ds_digib_font_12;

// static const lv_font_t *lb_swarm_total_hr_font            = &ds_digib_font_22;
// static const lv_font_t *lb_swarm_best_diff_font           = &ds_digib_font_22;
// static const lv_font_t *lb_swarm_workers_font             = &ds_digib_font_22;


// static const lv_coord_t lb_version_loading_coord[2] = {(lv_coord_t)(SCREEN_WIDTH - (int16_t)(String(CURRENT_VERSION).length() * 8)), 2};
// static const lv_coord_t lb_version_config_coord[2] = {(lv_coord_t)(SCREEN_WIDTH - (int16_t)(String(CURRENT_VERSION).length() * 12)), 10};  
// static const lv_coord_t lb_timeout_config_coord[2] = {(lv_coord_t)(SCREEN_WIDTH - (int16_t)(String(CURRENT_VERSION).length() * 11)), SCREEN_HEIGHT - 35};  
// static const lv_coord_t lb_mine_page_ver_coord[2]  = {10, 128};
// static const lv_coord_t lb_slogan_loading_coord[2] = {0, 10};  
// static const lv_coord_t lb_version_check_loading_coord[2] = {0, 40};
// static const lv_coord_t lb_flash_addr_loading_coord[2] = {0, 70};
// static const lv_coord_t lb_hashrate_coord[2] = {125, 160};
// static const lv_coord_t lb_blk_hit_coord[2] = {11, 65};
// static const lv_coord_t lb_temp_or_time_coord[2] = {2, 5};
// static const lv_coord_t lb_uptime_day_coord[2] = {58, 5};
// static const lv_coord_t lb_wifi_ip_coord[2] = {13, 175};
// static const lv_coord_t lb_wifi_rssi_coord[2] = {185, 5};

// static const lv_coord_t lb_price_coord[2] = {40, 145};
// static const lv_coord_t lb_price_symbol_coord[2] = {20, 145};

// static const lv_coord_t lb_uptime_day_unit_coord[2] = {86, 5};
// static const lv_coord_t lb_uptime_hms_coord[2] = {96, 5};
// static const lv_coord_t lb_wifi_rssi_symbol_coord[2] = {165, 5};




// static const lv_coord_t lb_job_received_coord[2] = {112 + 28, 15 + 14};
// static const lv_coord_t lb_net_diff_coord[2]     = {112 + 28, 2*15 + 14*2 + 1};
// static const lv_coord_t lb_local_diff_coord[2]   = {112 + 28, 3*15 + 14*3};
// static const lv_coord_t lb_shares_coord[2]       = {112 + 28, 4*15 + 14*4 - 1};

// static const lv_coord_t lb_job_receive_symbol_coord[2]  = {112, 15 +15};
// static const lv_coord_t lb_net_diff_symbol_coord[2]     = {112, 2*15 +15*2 + 1};
// static const lv_coord_t lb_local_diff_symbol_coord[2]   = {112, 3*15 +15*3};
// static const lv_coord_t lb_share_symb_coord[2]          = {112 - 3, 4*15 + 15*4 - 3};

// static const lv_coord_t lb_clock_coord[2] = {0, 5};
// static const lv_coord_t lb_clock_hr_coord[2] = {95 + 45 , 2};
// static const lv_coord_t lb_clock_hr_unit_coord[2] = {0, 18};
// static const lv_coord_t lb_clock_version_coord[2] = {8, 5};

// static const lv_coord_t lb_clock_day_coord[2] = {0, 0};
// static const lv_coord_t lb_clock_hms_coord[2] = {0, 0};
// static const lv_coord_t lb_clock_price_coord[2] = {2, 2};
// static const lv_coord_t lb_clock_pool_coord[2] = {1, -6};
// static const lv_coord_t lb_clock_price_changed_coord[2] = {1, 25};

// static const lv_coord_t lb_swarm_best_diff_coord[2] = {2, 215};
// static const lv_coord_t lb_swarm_workers_coord[2]   = {106, 215};
// static const lv_coord_t lb_swarm_total_hr_coord[2]  = {170, 215};


const lv_img_dsc_t loading_img = {
    .header = {
        .cf = LV_IMG_CF_TRUE_COLOR,
        .always_zero = 0,
        .reserved = 0,
        .w = SCREEN_WIDTH,  
        .h = SCREEN_HEIGHT,  
    },
    .data_size = SCREEN_WIDTH * SCREEN_HEIGHT * LV_COLOR_SIZE / 8,
    .data = (const uint8_t *)global_img_array, 
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
    .data = (const uint8_t *)global_img_array, 
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
    .data = (const uint8_t *)global_img_array, 
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
    .data = (const uint8_t *)global_img_array, 
};

static int blpwmChannel = 0;  
static void tft_bl_ctrl(float brightness) {
  if (TFT_BL != -1) {
    // 确保亮度在 0 到 1 之间
    brightness = brightness <= 0 ? 0 : (brightness >= 1 ? 1 : brightness);
    uint8_t pwm = (TFT_BACKLIGHT_ON == HIGH) ? (uint8_t)(brightness * 255) : (uint8_t)((1 - brightness) * 255);
    ledcWrite(blpwmChannel, pwm);
  }
}

void tft_bl_active(void){
  if(TFT_BL!=-1){
    g_nm.screen.active = true;
    g_nm.screen.cnt = 0;
    float brightness = (float)g_nm.screen.brightness/100.0f;
    brightness = brightness <= 0 ? 1 : brightness;//亮度为0时默认为用户设置的起始亮度
    tft_bl_ctrl(brightness);
  }
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
      tft.setRotation(2);
  }
  else {
      tft.setRotation(0);
  }

  if(TFT_BL!=-1){
    int freq = 5*1000;    
    int resolution = 8;  
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

    tft.startWrite();
    tft.setAddrWindow( area->x1 , area->y1 , w , h  );
    tft.pushColors( ( uint16_t * )&color_p->full, w * h, true );
    tft.endWrite();
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
    if(!g_nm.screen.active)continue;
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
  const uint32_t color_buf_size = SCREEN_WIDTH * SCREEN_HEIGHT / 5;
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
  price_page = lv_obj_create(parent_docker);
  lv_obj_set_size(price_page, SCREEN_WIDTH, SCREEN_HEIGHT);
  lv_obj_set_pos(price_page, 1 * SCREEN_WIDTH, 0);
  lv_obj_set_style_pad_all(price_page, 0, 0);
  lv_obj_set_style_border_width(price_page, 0, 0);
  lv_obj_set_scrollbar_mode(price_page, LV_SCROLLBAR_MODE_OFF); 
  lv_obj_t *miner_img_obj = lv_img_create(price_page);
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
  g_pages[PAGE_PRICE_RANK]   = price_page;
  g_pages[PAGE_WEATHER] = weather_page; 
  g_pages[PAGE_CLOCK]   = clock_page; 
  ////////////////////////////////////// loading page layout ///////////////////////////////////////////////
 
}


static void ui_update_loading_string(String str, uint32_t color, bool prgress_update) {


}


static void ui_price_page_rank_refresh(std::map<ccoin_name, ccoin_node> &map){
  // https://s2.coinmarketcap.com/static/img/coins/32x32/1.png
  if(map.empty()) return;
  if(g_pages[PAGE_PRICE_RANK] == NULL) return;

  static const uint8_t rank_max = 7; // Maximum number of coins to display
  static lv_img_dsc_t coin_icon_img_dsc[rank_max];
  static lv_obj_t* icon_png_list[rank_max] = {NULL,};
  static lv_obj_t* lb_crypto_coin_price[rank_max] = {NULL,};

  // Initialize the icon image descriptors and labels if not already done
  for(uint8_t i = 0; i < rank_max; i++) {
      //create or update the icon image
      if(icon_png_list[i] == NULL) {
          icon_png_list[i] = lv_img_create(g_pages[PAGE_PRICE_RANK]);
          lv_obj_align(icon_png_list[i], LV_ALIGN_TOP_LEFT, 0, i * 33 + 2);
      }
      if(lb_crypto_coin_price[i] == NULL) {
          lb_crypto_coin_price[i] = lv_label_create(g_pages[PAGE_PRICE_RANK]);
          lv_color_t font_color = lv_color_hex(0xFFFFFF);
          lv_obj_set_width(lb_crypto_coin_price[i], SCREEN_WIDTH);
          lv_label_set_text(lb_crypto_coin_price[i], "");
          lv_obj_set_style_text_color(lb_crypto_coin_price[i], font_color, LV_PART_MAIN); 
          lv_obj_set_style_text_font(lb_crypto_coin_price[i], lb_price_rank_font, 0);
          lv_obj_align(lb_crypto_coin_price[i], LV_ALIGN_TOP_LEFT, 40, i * 33);
      }
  }

  // Sort the map by price in descending order
  std::vector<std::pair<ccoin_name, ccoin_node>> sorted_vec(map.begin(), map.end());
  std::sort(sorted_vec.begin(), sorted_vec.end(),
      [](const std::pair<ccoin_name, ccoin_node>& a, const std::pair<ccoin_name, ccoin_node>& b) {
          return a.second.price.realtime > b.second.price.realtime; 
      }
  );

  uint8_t index = 0;
  for(const auto& coin : sorted_vec) {
      // item.first 是 ccoin_name，item.second 是 ccoin_node
      if(coin.second.icon.addr == NULL || coin.second.icon.size == 0) continue; //skip if icon data is not available

      //update coin icon image descriptor
      coin_icon_img_dsc[index].header.cf = LV_IMG_CF_RAW_ALPHA;
      coin_icon_img_dsc[index].header.w = 0; //auto width
      coin_icon_img_dsc[index].header.h = 0; //auto height
      coin_icon_img_dsc[index].header.always_zero = 0;
      coin_icon_img_dsc[index].header.reserved = 0;
      coin_icon_img_dsc[index].data_size = coin.second.icon.size;
      coin_icon_img_dsc[index].data      = (const uint8_t *)(coin.second.icon.addr);
      lv_img_set_src(icon_png_list[index], &coin_icon_img_dsc[index]);

      //update coin price label
      String price_str = "$" + String(coin.second.price.realtime, 1);
      lv_coord_t width = lv_txt_get_width(price_str.c_str(), strlen(price_str.c_str()), lb_price_rank_font, 0, LV_TEXT_FLAG_NONE);
      lv_obj_set_width(lb_crypto_coin_price[index], width);
      lv_label_set_text_fmt(lb_crypto_coin_price[index], "%s", price_str);
      index++;
  }
}

static void ui_clock_page_refresh(){

}



static void ui_switch_to_page(uint8_t page, bool anim){
  g_page_index = page;
  if(anim) lv_obj_scroll_to_view(g_pages[g_page_index], LV_ANIM_ON);
}

static void ui_refresh_thread(void *args){
  char *name = (char*)malloc(20);
  strcpy(name, (char*)args);
  LOG_I("%s thread started on core %d...", name, xPortGetCoreID());
  free(name);

  while (true){
    delay(1000);

    if(xSemaphoreTake(lvgl_xMutex, 0) == pdTRUE){
      if(g_page_index == PAGE_PRICE_RANK) ui_price_page_rank_refresh(g_nm.coin_price_rank);
      else if(g_page_index == PAGE_CLOCK) ui_clock_page_refresh();
      //release mutex
      xSemaphoreGive(lvgl_xMutex); 
    }
    
    if(TFT_BL!=-1){
      if(g_nm.screen.sleep_timeout > 0){
        int cycle = (g_nm.screen.sleep_timeout - g_nm.screen.cnt);
        cycle = cycle > 0 ? cycle : 0;
        uint8_t full = (uint8_t)(((float)g_nm.screen.brightness/100.0f) * 255);//
        cycle = cycle * full / g_nm.screen.sleep_timeout;
        tft_bl_ctrl((float)cycle / 255.0f);
        g_nm.screen.active = cycle > 0;
      }else {
        tft_bl_ctrl((float)(g_nm.screen.brightness/100.0f));
      }
    }
  }
}

void ui_switch_next_page_cb(){
  g_page_index = (g_page_index == PAGE_PRICE_RANK) ? PAGE_CLOCK : PAGE_PRICE_RANK;
  ui_switch_to_page(g_page_index, true);
  g_nm.screen.last_operaion = millis();
}


static void lvgl_log_cb(const char * buf){
    Serial.print(buf);
}


void display_thread(void *args){
  char *name = (char*)malloc(20);
  strcpy(name, (char*)args);
  LOG_I("%s thread started on core %d...", name, xPortGetCoreID());
  free(name);
  uint8_t cnt  = 0;
  screen_init();

  lv_init();

#if LV_USE_LOG
  lv_log_register_print_cb(lvgl_log_cb); 
#endif

  ui_init();

  ui_layout_init();
  
  //lvgl tick task
  String taskName = "(lvgltick)";
  xTaskCreatePinnedToCore(lvgl_tick_task, taskName.c_str(), 1024*4, (void*)taskName.c_str(), TASK_PRIORITY_LVGL_DRV, &task_lvgl_tick, LvglTaskCore);
  delay(500);//wait a bit for lvgl tick task to start, necessary for lvgl to work properly

  taskName = "(uirefresh)";
  xTaskCreatePinnedToCore(ui_refresh_thread, taskName.c_str(), 1024*2.5, (void*)taskName.c_str(), TASK_PRIORITY_UI_REFRESH, &task_ui_refresh, UiRefreshTaskCore);
  delay(500);

  ui_switch_to_page(PAGE_LOADING, true);

  /***************************************switch to miner page*************************************/
  ui_switch_to_page(PAGE_PRICE_RANK, true);
  vTaskDelete(NULL);
}
#endif //TFT_DISPLAY
