#include "global.h"
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
  MENU_PAGE_BEGIN = 0,
  MENU_PAGE_PRICE,
  MENU_PAGE_WEATHER,
  MENU_PAGE_CLOCK,
  MENU_PAGE_IDEA,
  MENU_PAGE_ALBUM,
  MENU_PAGE_SETTINGS,
  MENU_PAGE_END
};

enum{
  SUB_MENU_PAGE_BEGIN = 0,
  SUB_MENU_PAGE_0,
  SUB_MENU_PAGE_1,
  SUB_MENU_PAGE_2,
  SUB_MENU_PAGE_3,
  SUB_MENU_PAGE_4,
  SUB_MENU_PAGE_5,
  SUB_MENU_PAGE_END
};

typedef enum{
  MENU_SCREEN = 0, // menu page
  SUB_MENU_SCREEN, // sub menu page
} screen_type_t;

/*********************************************************************全局lvgl对象********************************************************************/
static lv_color_t          *lvgl_color_buf = NULL;
static lv_disp_draw_buf_t   draw_buf;
static SemaphoreHandle_t    lvgl_xMutex;
static lv_obj_t *parent_docker = NULL;
static lv_obj_t *menu_pages[MENU_PAGE_END + 1] = {NULL,}, *sub_menu_pages[SUB_MENU_PAGE_END + 1] = {NULL,};

static lv_obj_t  *clone_setting_menu_page = NULL, 
                 *price_menu_page = NULL,
                 *weather_menu_page = NULL,
                 *clock_menu_page = NULL, 
                 *idea_menu_page = NULL, 
                 *album_menu_page = NULL,
                 *settings_menu_page = NULL,
                 *clone_price_menu_page = NULL;

static lv_obj_t  *sub_menu_page_5_clone = NULL,
                 *sub_menu_page_0 = NULL, 
                 *sub_menu_page_1 = NULL, 
                 *sub_menu_page_2 = NULL, 
                 *sub_menu_page_3 = NULL, 
                 *sub_menu_page_4 = NULL, 
                 *sub_menu_page_5 = NULL,
                 *sub_menu_page_0_clone = NULL;

typedef struct{
  bool                    sub_page_inited[SUB_MENU_PAGE_END + 1];      // whether the UI layout is initialized
  int                     menu_page_index;      // current menu page index
  int                     sub_menu_page_index;  // current sub menu page index
  screen_type_t           current_screen_type;  // current screen type, MENU_SCREEN or SUB_MENU_SCREEN
}ui_state_t;

ui_state_t ui_state = {
  .menu_page_index          = MENU_PAGE_PRICE,
  .sub_menu_page_index      = SUB_MENU_PAGE_0,
  .current_screen_type      = MENU_SCREEN
};



static void ui_price_rank_summary_refresh(std::map<ccoin_name, ccoin_node> &coin_price_rank, lv_obj_t *page);
static void ui_price_rank_details_refresh(std::map<ccoin_name, ccoin_node> &coin_price_rank, lv_obj_t *page);

static void ui_weather_realtime_refresh(weather_realtime_info_t &realtime, lv_obj_t *page);

typedef void (*page_refresh_func_t)(void);
page_refresh_func_t refresh_table[SUB_MENU_PAGE_END + 1][MENU_PAGE_END + 1] = {
    // clone page
    {
      NULL, 
      NULL,
      NULL,
      NULL,
      NULL,
      NULL,
      NULL,
      NULL
    },
    // SUB_MENU_PAGE_0
    { 
      NULL, 
      [](){ ui_price_rank_summary_refresh(g_nm.coin_price_rank, sub_menu_page_0); }, // MENU_PAGE_PRICE
      [](){ /* weather refresh */ }, // MENU_PAGE_WEATHER
      [](){ /* clock refresh */ }, // MENU_PAGE_CLOCK
      [](){ /* idea refresh */ }, // MENU_PAGE_IDEA
      [](){ /* album refresh */ }, // MENU_PAGE_ALBUM
      [](){ /* settings refresh */ }, // MENU_PAGE_SETTINGS
      NULL, 
    },
    // SUB_MENU_PAGE_1
    { 
      NULL, 
      [](){ ui_price_rank_details_refresh(g_nm.coin_price_rank, sub_menu_page_1); }, // MENU_PAGE_PRICE
      [](){ ui_weather_realtime_refresh(g_nm.weather_realtime, sub_menu_page_1); },  // MENU_PAGE_WEATHER
      [](){ /* clock refresh */ }, // MENU_PAGE_CLOCK
      [](){ /* idea refresh */ }, // MENU_PAGE_IDEA
      [](){ /* album refresh */ }, // MENU_PAGE_ALBUM
      [](){ /* settings refresh */ }, // MENU_PAGE_SETTINGS
      NULL, 
    },
    // SUB_MENU_PAGE_2
    { 
      NULL, 
      [](){ ui_price_rank_details_refresh(g_nm.coin_price_rank, sub_menu_page_2); }, // MENU_PAGE_PRICE
      [](){ /* weather refresh */ }, // MENU_PAGE_WEATHER
      [](){ /* clock refresh */ }, // MENU_PAGE_CLOCK
      [](){ /* idea refresh */ }, // MENU_PAGE_IDEA
      [](){ /* album refresh */ }, // MENU_PAGE_ALBUM
      [](){ /* settings refresh */ }, // MENU_PAGE_SETTINGS
      NULL, 
    },
    // SUB_MENU_PAGE_3
    { 
      NULL, 
      [](){ ui_price_rank_details_refresh(g_nm.coin_price_rank, sub_menu_page_3); }, // MENU_PAGE_PRICE
      [](){ /* weather refresh */ }, // MENU_PAGE_WEATHER
      [](){ /* clock refresh */ }, // MENU_PAGE_CLOCK
      [](){ /* idea refresh */ }, // MENU_PAGE_IDEA
      [](){ /* album refresh */ }, // MENU_PAGE_ALBUM
      [](){ /* settings refresh */ }, // MENU_PAGE_SETTINGS
      NULL, 
    },
    // SUB_MENU_PAGE_4
    { 
      NULL, 
      [](){ ui_price_rank_details_refresh(g_nm.coin_price_rank, sub_menu_page_4); }, // MENU_PAGE_PRICE
      [](){ /* weather refresh */ }, // MENU_PAGE_WEATHER
      [](){ /* clock refresh */ }, // MENU_PAGE_CLOCK
      [](){ /* idea refresh */ }, // MENU_PAGE_IDEA
      [](){ /* album refresh */ }, // MENU_PAGE_ALBUM
      [](){ /* settings refresh */ }, // MENU_PAGE_SETTINGS
      NULL, 
    },
    // SUB_MENU_PAGE_5
    { 
      NULL, 
      [](){ ui_price_rank_details_refresh(g_nm.coin_price_rank, sub_menu_page_5); }, // MENU_PAGE_PRICE
      [](){ /* weather refresh */ }, // MENU_PAGE_WEATHER
      [](){ /* clock refresh */ }, // MENU_PAGE_CLOCK
      [](){ /* idea refresh */ }, // MENU_PAGE_IDEA
      [](){ /* album refresh */ }, // MENU_PAGE_ALBUM
      [](){ /* settings refresh */ }, // MENU_PAGE_SETTINGS
      NULL, 
    },
    // clone page
    {
      NULL, 
      NULL,
      NULL,
      NULL,
      NULL,
      NULL,
      NULL,
      NULL
    },
};



#include "image_240_240.h"
LV_FONT_DECLARE(ds_digib_font_30)
LV_FONT_DECLARE(ds_digib_font_85)
// LV_FONT_DECLARE(symbol_10)
static const lv_font_t *lb_menu_title_font              = &lv_font_montserrat_34;
static const lv_font_t *lb_price_rank_font              = &lv_font_montserrat_34;
static const lv_font_t *lb_coin_name_detail_font        = &lv_font_montserrat_34;
static const lv_font_t *lb_coin_price_detail_font       = &lv_font_montserrat_38;
static const lv_font_t *lb_coin_price_change_1h_font    = &lv_font_montserrat_16;
static const lv_font_t *lb_coin_price_change_24h_font   = &lv_font_montserrat_16;
static const lv_font_t *lb_coin_price_last_updated_font = &lv_font_montserrat_16;
static const lv_font_t *lb_weather_realtime_temp_font   = &ds_digib_font_85;
static const lv_font_t *lb_weather_realtime_temp_feel_font = &ds_digib_font_30;
static const lv_font_t *lb_weather_realtime_desc_font      = &lv_font_montserrat_24;
static const lv_font_t *lb_weather_realtime_temp_unit_font = &lv_font_montserrat_22;
static const lv_font_t *lb_weather_realtime_city_name_font = &lv_font_montserrat_24;



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
  const uint32_t color_buf_size = SCREEN_WIDTH * SCREEN_HEIGHT / 10;
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

static void ui_boot_page_layout_init(void){
  lv_obj_t *background_img_obj = NULL;
  //create parent object
  lv_obj_t *loading_docker = lv_obj_create(lv_scr_act());
  lv_obj_set_size(loading_docker, SCREEN_WIDTH, SCREEN_HEIGHT );
  lv_obj_set_pos(loading_docker, 0, 0);
  lv_obj_set_scrollbar_mode(lv_scr_act(), LV_SCROLLBAR_MODE_OFF); 
  lv_obj_set_scroll_dir(loading_docker, LV_DIR_ALL); 
  lv_obj_set_scroll_snap_x(loading_docker, LV_SCROLL_SNAP_START); 
  lv_obj_set_scroll_snap_y(loading_docker, LV_SCROLL_SNAP_START); 
  lv_obj_set_style_pad_all(loading_docker, 0, 0);
  lv_obj_set_style_border_width(loading_docker, 0, 0);
  lv_obj_align(loading_docker, LV_ALIGN_TOP_LEFT, 0, 0);

  /*************************************** menu layout******************************************/
  // Create loading menu page
  lv_obj_t *boot_menu_page = lv_obj_create(loading_docker);
  lv_obj_set_size(boot_menu_page, SCREEN_WIDTH, SCREEN_HEIGHT);
  lv_obj_set_pos(boot_menu_page, 0 , 0);
  lv_obj_set_style_pad_all(boot_menu_page, 0, 0);
  lv_obj_set_style_border_width(boot_menu_page, 0, 0);
  lv_obj_set_scrollbar_mode(boot_menu_page, LV_SCROLLBAR_MODE_OFF); 
  background_img_obj = lv_img_create(boot_menu_page);
  lv_img_set_src(background_img_obj, &menu_back_img);
  lv_obj_set_size(background_img_obj, SCREEN_WIDTH, SCREEN_HEIGHT);
  lv_obj_align(background_img_obj, LV_ALIGN_TOP_LEFT, 0, 0);
  // display loading menu page
  lv_obj_scroll_to_view(boot_menu_page, LV_ANIM_OFF);

  String title = "Boot...";
  lv_color_t font_color = lv_color_hex(0xFFFFFF);
  lv_coord_t width = lv_txt_get_width(title.c_str(), strlen(title.c_str()), lb_menu_title_font, 0, LV_TEXT_FLAG_NONE);
  lv_obj_t *lb_menu_title  = lv_label_create(boot_menu_page);
  lv_obj_set_width(lb_menu_title, width);
  lv_label_set_text( lb_menu_title, title.c_str());
  lv_obj_set_style_text_font(lb_menu_title, lb_menu_title_font, LV_PART_MAIN);
  lv_obj_set_style_text_color(lb_menu_title, font_color, LV_PART_MAIN); 
  lv_label_set_long_mode(lb_menu_title, LV_LABEL_LONG_DOT);
  lv_obj_align( lb_menu_title, LV_ALIGN_CENTER, 0,0); 

}

static void ui_working_page_layout_init(void){
  lv_obj_t *background_img_obj = NULL;
  //create parent object
  parent_docker = lv_obj_create(lv_scr_act());
  lv_obj_set_size(parent_docker, SCREEN_WIDTH * 8, SCREEN_HEIGHT * 2); 
  lv_obj_set_pos(parent_docker, 0, 0);
  lv_obj_set_scrollbar_mode(lv_scr_act(), LV_SCROLLBAR_MODE_OFF); 
  lv_obj_set_scroll_dir(parent_docker, LV_DIR_ALL); 
  lv_obj_set_scroll_snap_x(parent_docker, LV_SCROLL_SNAP_START); 
  lv_obj_set_scroll_snap_y(parent_docker, LV_SCROLL_SNAP_START); 
  lv_obj_set_style_pad_all(parent_docker, 0, 0);
  lv_obj_set_style_border_width(parent_docker, 0, 0);
  lv_obj_align(parent_docker, LV_ALIGN_TOP_LEFT, 0, 0);

  /*************************************** menu layout******************************************/
  // Create clone setting menu page
  clone_setting_menu_page = lv_obj_create(parent_docker);
  lv_obj_set_size(clone_setting_menu_page, SCREEN_WIDTH, SCREEN_HEIGHT);
  lv_obj_set_pos(clone_setting_menu_page, 0 , 0);
  lv_obj_set_style_pad_all(clone_setting_menu_page, 0, 0);
  lv_obj_set_style_border_width(clone_setting_menu_page, 0, 0);
  lv_obj_set_scrollbar_mode(clone_setting_menu_page, LV_SCROLLBAR_MODE_OFF); 
  background_img_obj = lv_img_create(clone_setting_menu_page);
  lv_img_set_src(background_img_obj, &menu_back_img);
  lv_obj_set_size(background_img_obj, SCREEN_WIDTH, SCREEN_HEIGHT);
  lv_obj_align(background_img_obj, LV_ALIGN_TOP_LEFT, 0, 0);

  // Create price menu page  
  price_menu_page = lv_obj_create(parent_docker);
  lv_obj_set_size(price_menu_page, SCREEN_WIDTH, SCREEN_HEIGHT);
  lv_obj_set_pos(price_menu_page, 1 * SCREEN_WIDTH, 0);
  lv_obj_set_style_pad_all(price_menu_page, 0, 0);
  lv_obj_set_style_border_width(price_menu_page, 0, 0);
  lv_obj_set_scrollbar_mode(price_menu_page, LV_SCROLLBAR_MODE_OFF); 
  background_img_obj = lv_img_create(price_menu_page);
  lv_img_set_src(background_img_obj, &menu_back_img);
  lv_obj_set_size(background_img_obj, SCREEN_WIDTH, SCREEN_HEIGHT);
  lv_obj_align(background_img_obj, LV_ALIGN_TOP_LEFT, 0, 0);

  // Create weather menu page  
  weather_menu_page = lv_obj_create(parent_docker);
  lv_obj_set_size(weather_menu_page, SCREEN_WIDTH, SCREEN_HEIGHT);
  lv_obj_set_pos(weather_menu_page, 2 * SCREEN_WIDTH, 0);
  lv_obj_set_style_pad_all(weather_menu_page, 0, 0);
  lv_obj_set_style_border_width(weather_menu_page, 0, 0);
  lv_obj_set_scrollbar_mode(weather_menu_page, LV_SCROLLBAR_MODE_OFF); 
  background_img_obj = lv_img_create(weather_menu_page);
  lv_img_set_src(background_img_obj, &menu_back_img);
  lv_obj_set_size(background_img_obj, SCREEN_WIDTH, SCREEN_HEIGHT);
  lv_obj_align(background_img_obj, LV_ALIGN_TOP_LEFT, 0, 0);

  // Create clock menu page  
  clock_menu_page = lv_obj_create(parent_docker);
  lv_obj_set_size(clock_menu_page, SCREEN_WIDTH, SCREEN_HEIGHT);
  lv_obj_set_pos(clock_menu_page, 3 * SCREEN_WIDTH, 0);
  lv_obj_set_style_pad_all(clock_menu_page, 0, 0);
  lv_obj_set_style_border_width(clock_menu_page, 0, 0);
  lv_obj_set_scrollbar_mode(clock_menu_page, LV_SCROLLBAR_MODE_OFF); 
  background_img_obj = lv_img_create(clock_menu_page);
  lv_img_set_src(background_img_obj, &menu_back_img);
  lv_obj_set_size(background_img_obj, SCREEN_WIDTH, SCREEN_HEIGHT);
  lv_obj_align(background_img_obj, LV_ALIGN_TOP_LEFT, 0, 0);

  // Create idea menu page  
  idea_menu_page = lv_obj_create(parent_docker);
  lv_obj_set_size(idea_menu_page, SCREEN_WIDTH, SCREEN_HEIGHT);
  lv_obj_set_pos(idea_menu_page, 4 * SCREEN_WIDTH, 0);
  lv_obj_set_style_pad_all(idea_menu_page, 0, 0);
  lv_obj_set_style_border_width(idea_menu_page, 0, 0);
  lv_obj_set_scrollbar_mode(idea_menu_page, LV_SCROLLBAR_MODE_OFF); 
  background_img_obj = lv_img_create(idea_menu_page);
  lv_img_set_src(background_img_obj, &menu_back_img);
  lv_obj_set_size(background_img_obj, SCREEN_WIDTH, SCREEN_HEIGHT);
  lv_obj_align(background_img_obj, LV_ALIGN_TOP_LEFT, 0, 0);

  // Create album menu page  
  album_menu_page = lv_obj_create(parent_docker);
  lv_obj_set_size(album_menu_page, SCREEN_WIDTH, SCREEN_HEIGHT);
  lv_obj_set_pos(album_menu_page, 5 * SCREEN_WIDTH, 0);
  lv_obj_set_style_pad_all(album_menu_page, 0, 0);
  lv_obj_set_style_border_width(album_menu_page, 0, 0);
  lv_obj_set_scrollbar_mode(album_menu_page, LV_SCROLLBAR_MODE_OFF); 
  background_img_obj = lv_img_create(album_menu_page);
  lv_img_set_src(background_img_obj, &menu_back_img);
  lv_obj_set_size(background_img_obj, SCREEN_WIDTH, SCREEN_HEIGHT);
  lv_obj_align(background_img_obj, LV_ALIGN_TOP_LEFT, 0, 0);

  // Create setting menu page
  settings_menu_page = lv_obj_create(parent_docker);
  lv_obj_set_size(settings_menu_page, SCREEN_WIDTH, SCREEN_HEIGHT);
  lv_obj_set_pos(settings_menu_page, 6 * SCREEN_WIDTH, 0);
  lv_obj_set_style_pad_all(settings_menu_page, 0, 0);
  lv_obj_set_style_border_width(settings_menu_page, 0, 0);
  lv_obj_set_scrollbar_mode(settings_menu_page, LV_SCROLLBAR_MODE_OFF); 
  background_img_obj = lv_img_create(settings_menu_page);
  lv_img_set_src(background_img_obj, &menu_back_img);
  lv_obj_set_size(background_img_obj, SCREEN_WIDTH, SCREEN_HEIGHT);
  lv_obj_align(background_img_obj, LV_ALIGN_TOP_LEFT, 0, 0);

  // Create clone price menu page
  clone_price_menu_page = lv_obj_create(parent_docker);
  lv_obj_set_size(clone_price_menu_page, SCREEN_WIDTH, SCREEN_HEIGHT);
  lv_obj_set_pos(clone_price_menu_page, 7 * SCREEN_WIDTH, 0);
  lv_obj_set_style_pad_all(clone_price_menu_page, 0, 0);
  lv_obj_set_style_border_width(clone_price_menu_page, 0, 0);
  lv_obj_set_scrollbar_mode(clone_price_menu_page, LV_SCROLLBAR_MODE_OFF); 
  background_img_obj = lv_img_create(clone_price_menu_page);
  lv_img_set_src(background_img_obj, &menu_back_img);
  lv_obj_set_size(background_img_obj, SCREEN_WIDTH, SCREEN_HEIGHT);
  lv_obj_align(background_img_obj, LV_ALIGN_TOP_LEFT, 0, 0);

/*************************************** sub menu layout******************************************/
  // Create sub menu page5 clone
  sub_menu_page_5_clone = lv_obj_create(parent_docker);
  lv_obj_set_size(sub_menu_page_5_clone, SCREEN_WIDTH, SCREEN_HEIGHT);
  lv_obj_set_pos(sub_menu_page_5_clone, 0, SCREEN_HEIGHT);
  lv_obj_set_style_pad_all(sub_menu_page_5_clone, 0, 0);
  lv_obj_set_style_border_width(sub_menu_page_5_clone, 0, 0);
  lv_obj_set_scrollbar_mode(sub_menu_page_5_clone, LV_SCROLLBAR_MODE_OFF); 
  background_img_obj = lv_img_create(sub_menu_page_5_clone);
  lv_img_set_src(background_img_obj, &lv_sub_menu_back_img);
  lv_obj_set_size(background_img_obj, SCREEN_WIDTH, SCREEN_HEIGHT);
  lv_obj_align(background_img_obj, LV_ALIGN_TOP_LEFT, 0, 0);

  // Create sub menu page0  
  sub_menu_page_0 = lv_obj_create(parent_docker);
  lv_obj_set_size(sub_menu_page_0, SCREEN_WIDTH, SCREEN_HEIGHT);
  lv_obj_set_pos(sub_menu_page_0, SCREEN_WIDTH, SCREEN_HEIGHT);
  lv_obj_set_style_pad_all(sub_menu_page_0, 0, 0);
  lv_obj_set_style_border_width(sub_menu_page_0, 0, 0);
  lv_obj_set_scrollbar_mode(sub_menu_page_0, LV_SCROLLBAR_MODE_OFF); 
  background_img_obj = lv_img_create(sub_menu_page_0);
  lv_img_set_src(background_img_obj, &lv_sub_menu_back_img);
  lv_obj_set_size(background_img_obj, SCREEN_WIDTH, SCREEN_HEIGHT);
  lv_obj_align(background_img_obj, LV_ALIGN_TOP_LEFT, 0, 0);

  // Create sub menu page1
  sub_menu_page_1 = lv_obj_create(parent_docker);
  lv_obj_set_size(sub_menu_page_1, SCREEN_WIDTH, SCREEN_HEIGHT);
  lv_obj_set_pos(sub_menu_page_1, 2 * SCREEN_WIDTH, SCREEN_HEIGHT);
  lv_obj_set_style_pad_all(sub_menu_page_1, 0, 0);
  lv_obj_set_style_border_width(sub_menu_page_1, 0, 0);
  lv_obj_set_scrollbar_mode(sub_menu_page_1, LV_SCROLLBAR_MODE_OFF); 
  background_img_obj = lv_img_create(sub_menu_page_1);
  lv_img_set_src(background_img_obj, &lv_sub_menu_back_img);
  lv_obj_set_size(background_img_obj, SCREEN_WIDTH, SCREEN_HEIGHT);
  lv_obj_align(background_img_obj, LV_ALIGN_TOP_LEFT, 0, 0);

  // Create sub menu page2
  sub_menu_page_2 = lv_obj_create(parent_docker);
  lv_obj_set_size(sub_menu_page_2, SCREEN_WIDTH, SCREEN_HEIGHT);
  lv_obj_set_pos(sub_menu_page_2, 3 * SCREEN_WIDTH, SCREEN_HEIGHT);
  lv_obj_set_style_pad_all(sub_menu_page_2, 0, 0);
  lv_obj_set_style_border_width(sub_menu_page_2, 0, 0);
  lv_obj_set_scrollbar_mode(sub_menu_page_2, LV_SCROLLBAR_MODE_OFF); 
  background_img_obj = lv_img_create(sub_menu_page_2);
  lv_img_set_src(background_img_obj, &lv_sub_menu_back_img);
  lv_obj_set_size(background_img_obj, SCREEN_WIDTH, SCREEN_HEIGHT);
  lv_obj_align(background_img_obj, LV_ALIGN_TOP_LEFT, 0, 0);

  // Create sub menu page3
  sub_menu_page_3 = lv_obj_create(parent_docker);
  lv_obj_set_size(sub_menu_page_3, SCREEN_WIDTH, SCREEN_HEIGHT);
  lv_obj_set_pos(sub_menu_page_3, 4 * SCREEN_WIDTH, SCREEN_HEIGHT);
  lv_obj_set_style_pad_all(sub_menu_page_3, 0, 0);
  lv_obj_set_style_border_width(sub_menu_page_3, 0, 0);
  lv_obj_set_scrollbar_mode(sub_menu_page_3, LV_SCROLLBAR_MODE_OFF); 
  background_img_obj = lv_img_create(sub_menu_page_3);
  lv_img_set_src(background_img_obj, &lv_sub_menu_back_img);
  lv_obj_set_size(background_img_obj, SCREEN_WIDTH, SCREEN_HEIGHT);
  lv_obj_align(background_img_obj, LV_ALIGN_TOP_LEFT, 0, 0);

  // Create sub menu page4
  sub_menu_page_4 = lv_obj_create(parent_docker);
  lv_obj_set_size(sub_menu_page_4, SCREEN_WIDTH, SCREEN_HEIGHT);
  lv_obj_set_pos(sub_menu_page_4, 5 * SCREEN_WIDTH, SCREEN_HEIGHT);
  lv_obj_set_style_pad_all(sub_menu_page_4, 0, 0);
  lv_obj_set_style_border_width(sub_menu_page_4, 0, 0);
  lv_obj_set_scrollbar_mode(sub_menu_page_4, LV_SCROLLBAR_MODE_OFF); 
  background_img_obj = lv_img_create(sub_menu_page_4);
  lv_img_set_src(background_img_obj, &lv_sub_menu_back_img);
  lv_obj_set_size(background_img_obj, SCREEN_WIDTH, SCREEN_HEIGHT);
  lv_obj_align(background_img_obj, LV_ALIGN_TOP_LEFT, 0, 0);

  // Create sub menu page5
  sub_menu_page_5 = lv_obj_create(parent_docker);
  lv_obj_set_size(sub_menu_page_5, SCREEN_WIDTH, SCREEN_HEIGHT);
  lv_obj_set_pos(sub_menu_page_5, 6 * SCREEN_WIDTH, SCREEN_HEIGHT);
  lv_obj_set_style_pad_all(sub_menu_page_5, 0, 0);
  lv_obj_set_style_border_width(sub_menu_page_5, 0, 0);
  lv_obj_set_scrollbar_mode(sub_menu_page_5, LV_SCROLLBAR_MODE_OFF); 
  background_img_obj = lv_img_create(sub_menu_page_5);
  lv_img_set_src(background_img_obj, &lv_sub_menu_back_img);
  lv_obj_set_size(background_img_obj, SCREEN_WIDTH, SCREEN_HEIGHT);
  lv_obj_align(background_img_obj, LV_ALIGN_TOP_LEFT, 0, 0);

  // Create sub menu page0 clone
  sub_menu_page_0_clone = lv_obj_create(parent_docker);
  lv_obj_set_size(sub_menu_page_0_clone, SCREEN_WIDTH, SCREEN_HEIGHT);
  lv_obj_set_pos(sub_menu_page_0_clone, 7 * SCREEN_WIDTH, SCREEN_HEIGHT);
  lv_obj_set_style_pad_all(sub_menu_page_0_clone, 0, 0);
  lv_obj_set_style_border_width(sub_menu_page_0_clone, 0, 0);
  lv_obj_set_scrollbar_mode(sub_menu_page_0_clone, LV_SCROLLBAR_MODE_OFF); 
  background_img_obj = lv_img_create(sub_menu_page_0_clone);
  lv_img_set_src(background_img_obj, &lv_sub_menu_back_img);
  lv_obj_set_size(background_img_obj, SCREEN_WIDTH, SCREEN_HEIGHT);
  lv_obj_align(background_img_obj, LV_ALIGN_TOP_LEFT, 0, 0);

  // Create menu_pages array
  menu_pages[MENU_PAGE_BEGIN]        = clone_setting_menu_page;
  menu_pages[MENU_PAGE_PRICE]        = price_menu_page;
  menu_pages[MENU_PAGE_WEATHER]      = weather_menu_page; 
  menu_pages[MENU_PAGE_CLOCK]        = clock_menu_page; 
  menu_pages[MENU_PAGE_IDEA]         = idea_menu_page;
  menu_pages[MENU_PAGE_ALBUM]        = album_menu_page;
  menu_pages[MENU_PAGE_SETTINGS]     = settings_menu_page;
  menu_pages[MENU_PAGE_END]          = clone_price_menu_page;   // clone first page to the end for easy access

  // Create sub_menu_pages array
  sub_menu_pages[SUB_MENU_PAGE_BEGIN] = sub_menu_page_5_clone;
  sub_menu_pages[SUB_MENU_PAGE_0]     = sub_menu_page_0;
  sub_menu_pages[SUB_MENU_PAGE_1]     = sub_menu_page_1;
  sub_menu_pages[SUB_MENU_PAGE_2]     = sub_menu_page_2;
  sub_menu_pages[SUB_MENU_PAGE_3]     = sub_menu_page_3;
  sub_menu_pages[SUB_MENU_PAGE_4]     = sub_menu_page_4;
  sub_menu_pages[SUB_MENU_PAGE_5]     = sub_menu_page_5;
  sub_menu_pages[SUB_MENU_PAGE_END]   = sub_menu_page_0_clone;

  ////////////////////////////////////// clone setting menu page layout /////////////////////////////////////////////
  String title = "Settings";
  lv_color_t font_color = lv_color_hex(0xFFFFFF);
  lv_coord_t width = lv_txt_get_width(title.c_str(), strlen(title.c_str()), lb_menu_title_font, 0, LV_TEXT_FLAG_NONE);
  lv_obj_t *lb_menu_title  = lv_label_create(clone_setting_menu_page);
  lv_obj_set_width(lb_menu_title, width);
  lv_label_set_text( lb_menu_title, title.c_str());
  lv_obj_set_style_text_font(lb_menu_title, lb_menu_title_font, LV_PART_MAIN);
  lv_obj_set_style_text_color(lb_menu_title, font_color, LV_PART_MAIN); 
  lv_label_set_long_mode(lb_menu_title, LV_LABEL_LONG_DOT);
  lv_obj_align( lb_menu_title, LV_ALIGN_CENTER, 0,0); 
  ////////////////////////////////////// price menu page layout ///////////////////////////////////////////////
  title = "Price";
  font_color = lv_color_hex(0xFFFFFF);
  width = lv_txt_get_width(title.c_str(), strlen(title.c_str()), lb_menu_title_font, 0, LV_TEXT_FLAG_NONE);
  lb_menu_title  = NULL;
  lb_menu_title = lv_label_create(price_menu_page);
  lv_obj_set_width(lb_menu_title, width);
  lv_label_set_text( lb_menu_title, title.c_str());
  lv_obj_set_style_text_font(lb_menu_title, lb_menu_title_font, LV_PART_MAIN);
  lv_obj_set_style_text_color(lb_menu_title, font_color, LV_PART_MAIN); 
  lv_label_set_long_mode(lb_menu_title, LV_LABEL_LONG_DOT);
  lv_obj_align( lb_menu_title, LV_ALIGN_CENTER, 0,0); 
  ////////////////////////////////////// weather menu page layout /////////////////////////////////////////////
  title = "Weather";
  font_color = lv_color_hex(0xFFFFFF);
  width = lv_txt_get_width(title.c_str(), strlen(title.c_str()), lb_menu_title_font, 0, LV_TEXT_FLAG_NONE);
  lb_menu_title  = lv_label_create(weather_menu_page);
  lv_obj_set_width(lb_menu_title, width);
  lv_label_set_text( lb_menu_title, title.c_str());
  lv_obj_set_style_text_font(lb_menu_title, lb_menu_title_font, LV_PART_MAIN);
  lv_obj_set_style_text_color(lb_menu_title, font_color, LV_PART_MAIN); 
  lv_label_set_long_mode(lb_menu_title, LV_LABEL_LONG_DOT);
  lv_obj_align( lb_menu_title, LV_ALIGN_CENTER, 0,0); 
  ////////////////////////////////////// clock menu page layout ///////////////////////////////////////////////
  title = "Clock";
  font_color = lv_color_hex(0xFFFFFF);
  width = lv_txt_get_width(title.c_str(), strlen(title.c_str()), lb_menu_title_font, 0, LV_TEXT_FLAG_NONE);
  lb_menu_title  = lv_label_create(clock_menu_page);
  lv_obj_set_width(lb_menu_title, width);
  lv_label_set_text( lb_menu_title, title.c_str());
  lv_obj_set_style_text_font(lb_menu_title, lb_menu_title_font, LV_PART_MAIN);
  lv_obj_set_style_text_color(lb_menu_title, font_color, LV_PART_MAIN); 
  lv_label_set_long_mode(lb_menu_title, LV_LABEL_LONG_DOT);
  lv_obj_align( lb_menu_title, LV_ALIGN_CENTER, 0,0); 
  ////////////////////////////////////// idea menu page layout ////////////////////////////////////////////////
  title = "Idea";
  font_color = lv_color_hex(0xFFFFFF);
  width = lv_txt_get_width(title.c_str(), strlen(title.c_str()), lb_menu_title_font, 0, LV_TEXT_FLAG_NONE);
  lb_menu_title  = lv_label_create(idea_menu_page);
  lv_obj_set_width(lb_menu_title, width);
  lv_label_set_text( lb_menu_title, title.c_str());
  lv_obj_set_style_text_font(lb_menu_title, lb_menu_title_font, LV_PART_MAIN);
  lv_obj_set_style_text_color(lb_menu_title, font_color, LV_PART_MAIN); 
  lv_label_set_long_mode(lb_menu_title, LV_LABEL_LONG_DOT);
  lv_obj_align( lb_menu_title, LV_ALIGN_CENTER, 0,0); 
  ////////////////////////////////////// album menu page layout ///////////////////////////////////////////////
  title = "Album";
  font_color = lv_color_hex(0xFFFFFF);
  width = lv_txt_get_width(title.c_str(), strlen(title.c_str()), lb_menu_title_font, 0, LV_TEXT_FLAG_NONE);
  lb_menu_title  = lv_label_create(album_menu_page);
  lv_obj_set_width(lb_menu_title, width);
  lv_label_set_text( lb_menu_title, title.c_str());
  lv_obj_set_style_text_font(lb_menu_title, lb_menu_title_font, LV_PART_MAIN);
  lv_obj_set_style_text_color(lb_menu_title, font_color, LV_PART_MAIN); 
  lv_label_set_long_mode(lb_menu_title, LV_LABEL_LONG_DOT);
  lv_obj_align( lb_menu_title, LV_ALIGN_CENTER, 0,0); 
  ////////////////////////////////////// setting menu page layout /////////////////////////////////////////////
  title = "Settings";
  font_color = lv_color_hex(0xFFFFFF);
  width = lv_txt_get_width(title.c_str(), strlen(title.c_str()), lb_menu_title_font, 0, LV_TEXT_FLAG_NONE);
  lb_menu_title  = lv_label_create(settings_menu_page);
  lv_obj_set_width(lb_menu_title, width);
  lv_label_set_text( lb_menu_title, title.c_str());
  lv_obj_set_style_text_font(lb_menu_title, lb_menu_title_font, LV_PART_MAIN);
  lv_obj_set_style_text_color(lb_menu_title, font_color, LV_PART_MAIN); 
  lv_label_set_long_mode(lb_menu_title, LV_LABEL_LONG_DOT);
  lv_obj_align( lb_menu_title, LV_ALIGN_CENTER, 0,0); 
  ////////////////////////////////////// clone price menu page layout /////////////////////////////////////////////
  title = "Price";
  font_color = lv_color_hex(0xFFFFFF);
  width = lv_txt_get_width(title.c_str(), strlen(title.c_str()), lb_menu_title_font, 0, LV_TEXT_FLAG_NONE);
  lb_menu_title  = lv_label_create(clone_price_menu_page);
  lv_obj_set_width(lb_menu_title, width);
  lv_label_set_text( lb_menu_title, title.c_str());
  lv_obj_set_style_text_font(lb_menu_title, lb_menu_title_font, LV_PART_MAIN);
  lv_obj_set_style_text_color(lb_menu_title, font_color, LV_PART_MAIN); 
  lv_label_set_long_mode(lb_menu_title, LV_LABEL_LONG_DOT);
  lv_obj_align( lb_menu_title, LV_ALIGN_CENTER, 0,0); 
}

void ui_switch_next_page_cb(){
  xSemaphoreGive(g_nm.global_xsem.next_page_xsem);
}

void ui_switch_prev_page_cb(){
  xSemaphoreGive(g_nm.global_xsem.prev_page_xsem);
}

void ui_enter_or_exit_current_page_cb(){
  xSemaphoreGive(g_nm.global_xsem.ok_cancel_xsem);
}

static void ui_sub_menu_page_obj_clear(lv_obj_t *page){
  if(page == NULL) return;
  // Clear all objects in the page
  lv_obj_clean(page);
  // Recreate the background image
  lv_obj_t *background_img_obj = lv_img_create(page);
  lv_img_set_src(background_img_obj, &lv_sub_menu_back_img);
  lv_obj_set_size(background_img_obj, SCREEN_WIDTH, SCREEN_HEIGHT);
  lv_obj_align(background_img_obj, LV_ALIGN_TOP_LEFT, 0, 0);
}

static void ui_price_rank_summary_refresh(std::map<ccoin_name, ccoin_node> &coin_price_rank, lv_obj_t *page){
  // https://s2.coinmarketcap.com/static/img/coins/32x32/1.png
  if(coin_price_rank.empty()) return;
  if(page == NULL) return;
  static const uint8_t rank_max = 7; // Maximum number of coins to display
  static lv_img_dsc_t coin_icon_img_dsc[rank_max];
  static lv_obj_t* icon_png_list[rank_max] = {NULL,};
  static lv_obj_t* lb_crypto_coin_price[rank_max] = {NULL,};

  // Initialize the icon png list and price label list
  if(!ui_state.sub_page_inited[SUB_MENU_PAGE_0]){
    for(uint8_t i = 0; i < rank_max; i++) {
        //create or update the icon image
        ui_state.sub_page_inited[SUB_MENU_PAGE_0] = true; // assume all sub pages are inited
        bool init = false;

        icon_png_list[i] = lv_img_create(page);
        init = (icon_png_list[i] != NULL);
        ui_state.sub_page_inited[SUB_MENU_PAGE_0] = ui_state.sub_page_inited[SUB_MENU_PAGE_0] && init;//set init inactive if any icon image is not created successfully
        if(!init) {
            LOG_E("Failed to create icon_png_list[%d]", i);
            continue;
        }
        lv_obj_align(icon_png_list[i], LV_ALIGN_TOP_LEFT, 0, i * 33 + 2);

        //create the price label
        lb_crypto_coin_price[i] = lv_label_create(page);
        init = (lb_crypto_coin_price[i] != NULL);
        ui_state.sub_page_inited[SUB_MENU_PAGE_0] = ui_state.sub_page_inited[SUB_MENU_PAGE_0] && init;//set init inactive if any label is not created successfully
        if(!init) {
            LOG_E("Failed to create lb_crypto_coin_price[%d]", i);
            continue;
        }
        lv_color_t font_color = lv_color_hex(0xFFFFFF);
        lv_obj_set_width(lb_crypto_coin_price[i], SCREEN_WIDTH);
        lv_label_set_text(lb_crypto_coin_price[i], "");
        lv_obj_set_style_text_color(lb_crypto_coin_price[i], font_color, LV_PART_MAIN); 
        lv_obj_set_style_text_font(lb_crypto_coin_price[i], lb_price_rank_font, 0);
        lv_obj_align(lb_crypto_coin_price[i], LV_ALIGN_TOP_LEFT, 40, i * 33);
    }
    LOG_I("Sub page %d initialized", SUB_MENU_PAGE_0);
  }

  if(!ui_state.sub_page_inited[SUB_MENU_PAGE_0]) {
      LOG_E("Sub page %d is not initialized", SUB_MENU_PAGE_0);
      return;
  }
  

  // Sort the map by price in descending order
  std::vector<std::pair<ccoin_name, ccoin_node>> sorted_vec(coin_price_rank.begin(), coin_price_rank.end());
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


      int float_bits = 1;
      if(coin.second.price.realtime > 0 && coin.second.price.realtime < 10) float_bits = 4;
      else if(coin.second.price.realtime >= 10 && coin.second.price.realtime < 100) float_bits = 3;
      else if(coin.second.price.realtime >= 100 && coin.second.price.realtime < 1000) float_bits = 2;
      else if(coin.second.price.realtime >= 1000 && coin.second.price.realtime < 10000) float_bits = 1;
      else if(coin.second.price.realtime >= 10000 && coin.second.price.realtime < 100000) float_bits = 0;
      else float_bits = 1;


      //update coin price label
      String price_str = "$" + String(coin.second.price.realtime, float_bits);
      lv_coord_t width = lv_txt_get_width(price_str.c_str(), strlen(price_str.c_str()), lb_price_rank_font, 0, LV_TEXT_FLAG_NONE);
      lv_obj_set_width(lb_crypto_coin_price[index], width);
      lv_label_set_text_fmt(lb_crypto_coin_price[index], "%s", price_str);
      index++;
  }
}

static void ui_price_rank_details_refresh(std::map<ccoin_name, ccoin_node> &coin_price_rank, lv_obj_t *page){
  if(coin_price_rank.empty()) return;
  if(page == NULL) return;


  static const uint8_t rank_max = 5; // Maximum number of coins to display
  static lv_img_dsc_t coin_icon_img_dsc[rank_max];
  static lv_obj_t* icon_png_list[rank_max] = {NULL,};
  static lv_obj_t* lb_crypto_coin_name[rank_max] = {NULL,};
  static lv_obj_t* lb_crypto_coin_price[rank_max] = {NULL,};
  static lv_obj_t* lb_crypto_coin_change_1h[rank_max] = {NULL,};
  static lv_obj_t* lb_crypto_coin_change_24h[rank_max] = {NULL,};
  static lv_obj_t* lb_crypto_coin_price_last_update[rank_max] = {NULL,};
  lv_coord_t width = 0;



  // Sort the map by price in descending order
  std::vector<std::pair<ccoin_name, ccoin_node>> sorted_vec(coin_price_rank.begin(), coin_price_rank.end());
  std::sort(sorted_vec.begin(), sorted_vec.end(),
      [](const std::pair<ccoin_name, ccoin_node>& a, const std::pair<ccoin_name, ccoin_node>& b) {
          return a.second.price.realtime > b.second.price.realtime; 
      }
  );

  ccoin_node coin;
  int index = -1, sub_page_index = -1;
  if(page == sub_menu_page_1)     {index = 0; sub_page_index = 2;}
  else if(page == sub_menu_page_2){index = 1; sub_page_index = 3;}
  else if(page == sub_menu_page_3){index = 2; sub_page_index = 4;}
  else if(page == sub_menu_page_4){index = 3; sub_page_index = 5;}
  else if(page == sub_menu_page_5){index = 4; sub_page_index = 6;}
  else {LOG_E("Unknown page for price rank details refresh"); return;}

  coin = sorted_vec[index].second; // Get the first coin for sub_menu_page_0
  coin_icon_img_dsc[index].header.cf = LV_IMG_CF_RAW_ALPHA;
  coin_icon_img_dsc[index].header.w = 0; //auto width
  coin_icon_img_dsc[index].header.h = 0; //auto height
  coin_icon_img_dsc[index].header.always_zero = 0;
  coin_icon_img_dsc[index].header.reserved = 0;
  coin_icon_img_dsc[index].data_size = coin.icon.size;
  coin_icon_img_dsc[index].data      = (const uint8_t *)(coin.icon.addr);

  if(!ui_state.sub_page_inited[sub_page_index]){
    ui_state.sub_page_inited[sub_page_index] = true; // assume all sub pages are inited
    bool init = false;       

    //create icon image
    icon_png_list[index] = lv_img_create(page);
    init = (icon_png_list[index] != NULL);
    ui_state.sub_page_inited[sub_page_index] = ui_state.sub_page_inited[sub_page_index] && init; //set init inactive if any icon image is not created successfully
    if(!ui_state.sub_page_inited[sub_page_index]) {
      LOG_E("Failed to create icon image for coin: %s", coin.name.c_str());
      return;
    }
    lv_obj_align(icon_png_list[index], LV_ALIGN_TOP_LEFT, 0, 3);
    lv_img_set_src(icon_png_list[index], &coin_icon_img_dsc[index]);

    //create coin name label
    lb_crypto_coin_name[index] = lv_label_create(page);
    init = (lb_crypto_coin_name[index] != NULL);
    ui_state.sub_page_inited[sub_page_index] = ui_state.sub_page_inited[sub_page_index] && init; //set init inactive if any label is not created successfully
    if(!ui_state.sub_page_inited[sub_page_index]) {
      LOG_E("Failed to create coin name label for coin: %s", coin.name.c_str());
      return;
    }
    lv_color_t font_color = lv_color_hex(0xFFFFFF);
    width = lv_txt_get_width(coin.name.c_str(), strlen(coin.name.c_str()), lb_coin_name_detail_font, 0, LV_TEXT_FLAG_NONE);
    lv_obj_set_width(lb_crypto_coin_name[index], width);
    lv_label_set_text(lb_crypto_coin_name[index], coin.name.c_str());
    lv_obj_set_style_text_color(lb_crypto_coin_name[index], font_color, LV_PART_MAIN); 
    lv_obj_set_style_text_font(lb_crypto_coin_name[index], lb_coin_name_detail_font, 0);
    lv_obj_align(lb_crypto_coin_name[index], LV_ALIGN_TOP_MID, 0, 0);

    //create coin price label
    lb_crypto_coin_price[index] = lv_label_create(page);
    init = (lb_crypto_coin_price[index] != NULL);
    ui_state.sub_page_inited[sub_page_index] = ui_state.sub_page_inited[sub_page_index] && init; //set init inactive if any label is not created successfully
    if(!ui_state.sub_page_inited[sub_page_index]) {
      LOG_E("Failed to create coin price label for coin: %s", coin.name.c_str());
      return;
    }
    font_color = lv_color_hex(0x00FF00);
    String price_str = "$" + String(coin.price.realtime, 1);
    width = lv_txt_get_width(price_str.c_str(), strlen(price_str.c_str()), lb_coin_price_detail_font, 0, LV_TEXT_FLAG_NONE);
    lv_obj_set_width(lb_crypto_coin_price[index], width);
    lv_label_set_text(lb_crypto_coin_price[index], price_str.c_str());
    lv_obj_set_style_text_color(lb_crypto_coin_price[index], font_color, LV_PART_MAIN); 
    lv_obj_set_style_text_font(lb_crypto_coin_price[index], lb_coin_price_detail_font, 0);
    lv_obj_align(lb_crypto_coin_price[index], LV_ALIGN_CENTER, 0, -40);

    //create change labels
    lb_crypto_coin_change_1h[index] = lv_label_create(page);
    init = (lb_crypto_coin_change_1h[index] != NULL);
    ui_state.sub_page_inited[sub_page_index] = ui_state.sub_page_inited[sub_page_index] && init; //set init inactive if any label is not created successfully
    if(!ui_state.sub_page_inited[sub_page_index]) {
      LOG_E("Failed to create coin change 1h label for coin: %s", coin.name.c_str());
      return;
    }
    font_color = lv_color_hex(0xFFFFFF);
    String change_1h_str = String(coin.price.percent_change_1h, 2) + "% (1h)";
    width = lv_txt_get_width(change_1h_str.c_str(), strlen(change_1h_str.c_str()), lb_coin_price_change_1h_font, 0, LV_TEXT_FLAG_NONE);
    lv_obj_set_width(lb_crypto_coin_change_1h[index], width);
    lv_label_set_text(lb_crypto_coin_change_1h[index], change_1h_str.c_str());
    lv_obj_set_style_text_color(lb_crypto_coin_change_1h[index], font_color, LV_PART_MAIN); 
    lv_obj_set_style_text_font(lb_crypto_coin_change_1h[index], lb_coin_price_change_1h_font, 0);
    lv_obj_align(lb_crypto_coin_change_1h[index], LV_ALIGN_TOP_LEFT, 0, 105);

    //create change 24h label
    lb_crypto_coin_change_24h[index] = lv_label_create(page);
    init = (lb_crypto_coin_change_24h[index] != NULL);
    ui_state.sub_page_inited[sub_page_index] = ui_state.sub_page_inited[sub_page_index] && init; //set init inactive if any label is not created successfully
    if(!ui_state.sub_page_inited[sub_page_index]) {
      LOG_E("Failed to create coin change 24h label for coin: %s", coin.name.c_str());
      return;
    }
    font_color = lv_color_hex(0xFFFFFF);
    String change_24h_str = String(coin.price.percent_change_24h, 2) + "% (24h)";
    width = lv_txt_get_width(change_24h_str.c_str(), strlen(change_24h_str.c_str()), lb_coin_price_change_24h_font, 0, LV_TEXT_FLAG_NONE);
    lv_obj_set_width(lb_crypto_coin_change_24h[index], width);
    lv_label_set_text(lb_crypto_coin_change_24h[index], change_24h_str.c_str());
    lv_obj_set_style_text_color(lb_crypto_coin_change_24h[index], font_color, LV_PART_MAIN); 
    lv_obj_set_style_text_font(lb_crypto_coin_change_24h[index], lb_coin_price_change_24h_font, 0);
    lv_obj_align(lb_crypto_coin_change_24h[index], LV_ALIGN_TOP_RIGHT, 0, 105);

    //create last updated label
    lb_crypto_coin_price_last_update[index] = lv_label_create(page);
    init = (lb_crypto_coin_price_last_update[index] != NULL);
    ui_state.sub_page_inited[sub_page_index] = ui_state.sub_page_inited[sub_page_index] && init; //set init inactive if any label is not created successfully
    if(!ui_state.sub_page_inited[sub_page_index]) {
      LOG_E("Failed to create coin last updated label for coin: %s", coin.name.c_str());
      return;
    }
    font_color = lv_color_hex(0xFFFFFF);
    String last_update_str = coin.price.last_updated;
    width = lv_txt_get_width(last_update_str.c_str(), strlen(last_update_str.c_str()), lb_coin_price_last_updated_font, 0, LV_TEXT_FLAG_NONE);
    lv_obj_set_width(lb_crypto_coin_price_last_update[index], width);
    lv_label_set_text(lb_crypto_coin_price_last_update[index], last_update_str.c_str());
    lv_obj_set_style_text_color(lb_crypto_coin_price_last_update[index], font_color, LV_PART_MAIN); 
    lv_obj_set_style_text_font(lb_crypto_coin_price_last_update[index], lb_coin_price_last_updated_font, 0);
    lv_obj_align(lb_crypto_coin_price_last_update[index], LV_ALIGN_BOTTOM_RIGHT, 0, 0);

    LOG_I("sub page %d initialized", sub_page_index);
  }




  //update price label
   int float_bits = 1;
  if(coin.price.realtime > 0 && coin.price.realtime < 1) float_bits = 4;
  else if(coin.price.realtime >= 1 && coin.price.realtime < 100) float_bits = 3;
  else if(coin.price.realtime >= 100 && coin.price.realtime < 1000) float_bits = 2;
  else if(coin.price.realtime >= 1000 && coin.price.realtime < 10000) float_bits = 1;
  else if(coin.price.realtime >= 10000 && coin.price.realtime < 100000) float_bits = 0;
  else float_bits = 1;

  String price_str = "$" + String(coin.price.realtime, float_bits);
  width = lv_txt_get_width(price_str.c_str(), strlen(price_str.c_str()), lb_coin_price_detail_font, 0, LV_TEXT_FLAG_NONE);
  lv_obj_set_width(lb_crypto_coin_price[index], width);
  lv_label_set_text(lb_crypto_coin_price[index], price_str.c_str());

  //update change labels
  String change_1h_str = String(coin.price.percent_change_1h, 2) + "% (1h)";
  width = lv_txt_get_width(change_1h_str.c_str(), strlen(change_1h_str.c_str()), lb_coin_price_change_1h_font, 0, LV_TEXT_FLAG_NONE);
  lv_obj_set_width(lb_crypto_coin_change_1h[index], width);
  lv_label_set_text(lb_crypto_coin_change_1h[index], change_1h_str.c_str());

  String change_24h_str = String(coin.price.percent_change_24h, 2) + "% (24h)";
  width = lv_txt_get_width(change_24h_str.c_str(), strlen(change_24h_str.c_str()), lb_coin_price_change_24h_font, 0, LV_TEXT_FLAG_NONE);
  lv_obj_set_width(lb_crypto_coin_change_24h[index], width);
  lv_label_set_text(lb_crypto_coin_change_24h[index], change_24h_str.c_str());


  // update last updated label
  String last_update_str = coin.price.last_updated;
  width = lv_txt_get_width(last_update_str.c_str(), strlen(last_update_str.c_str()), lb_coin_price_last_updated_font, 0, LV_TEXT_FLAG_NONE);
  lv_obj_set_width(lb_crypto_coin_price_last_update[index], width);
  lv_label_set_text(lb_crypto_coin_price_last_update[index], last_update_str.c_str());



  // Set text color based on change 1h values
  if(coin.price.percent_change_1h < 0) {
      lv_obj_set_style_text_color(lb_crypto_coin_change_1h[index], lv_color_hex(0x00FF00), LV_PART_MAIN); // Red for negative change
  } else {
      lv_obj_set_style_text_color(lb_crypto_coin_change_1h[index], lv_color_hex(0xFF0000), LV_PART_MAIN); // Green for positive change
  }
  // Set text color based on change 1h values
  if(coin.price.percent_change_24h < 0) {
      lv_obj_set_style_text_color(lb_crypto_coin_change_24h[index], lv_color_hex(0x00FF00), LV_PART_MAIN); // Red for negative change
  } else {
      lv_obj_set_style_text_color(lb_crypto_coin_change_24h[index], lv_color_hex(0xFF0000), LV_PART_MAIN); // Green for positive change
  }
}

static void ui_weather_realtime_refresh(weather_realtime_info_t &realtime, lv_obj_t *page){
  if(page == NULL) return;
  if(realtime.weather.empty()) return; // no weather data available

  static lv_img_dsc_t weather_icon_img_dsc;
  static lv_obj_t* icon_png = NULL;
  static lv_obj_t* lb_temp = NULL, *lb_temp_feel = NULL, *lb_temp_unit = NULL;
  static lv_obj_t* lb_humidity = NULL, *lb_grnd_level = NULL, *lb_pressure = NULL;
  static lv_obj_t* lb_city_name = NULL, *lb_country_name = NULL;
  static lv_obj_t* lb_sunrise = NULL, *lb_sunset = NULL;
  static lv_obj_t* lb_weather_desc = NULL, *lb_weather_main = NULL, *lb_wind_speed = NULL, *lb_wind_deg = NULL;
  lv_coord_t width = 0;



  // lv_obj_t *spinner = lv_spinner_create(page, 1000, 60); // parent为你的页面对象，1000ms一圈，60px直径
  // lv_obj_set_size(spinner, 60, 60); // 设置大小
  // lv_obj_align(spinner, LV_ALIGN_CENTER, 0, 0); // 居中显示


  if(!ui_state.sub_page_inited[SUB_MENU_PAGE_1]){
    ui_state.sub_page_inited[SUB_MENU_PAGE_1] = true; // assume all sub pages are inited
    bool init = false;
    //create icon image
    icon_png = lv_img_create(page);
    init = (icon_png != NULL);
    ui_state.sub_page_inited[SUB_MENU_PAGE_1] = ui_state.sub_page_inited[SUB_MENU_PAGE_1] && init; //set init inactive if any icon image is not created successfully
    if(!ui_state.sub_page_inited[SUB_MENU_PAGE_1]) {
      LOG_E("Failed to create icon image for weather");
      return;
    }
    lv_obj_align(icon_png, LV_ALIGN_TOP_LEFT, 0, 3);


    //create city name label
    lb_city_name = lv_label_create(page);
    init = (lb_city_name != NULL);
    ui_state.sub_page_inited[SUB_MENU_PAGE_1] = ui_state.sub_page_inited[SUB_MENU_PAGE_1] && init; //set init inactive if any label is not created successfully
    if(!ui_state.sub_page_inited[SUB_MENU_PAGE_1]) {
      LOG_E("Failed to create city name label for weather");
      return;
    }
    lv_obj_set_width(lb_city_name, SCREEN_WIDTH / 2); // set width to half of the screen width
    lv_obj_set_style_text_font(lb_city_name, lb_weather_realtime_city_name_font, LV_PART_MAIN);
    lv_obj_set_style_text_color(lb_city_name, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_label_set_long_mode(lb_city_name, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_obj_align(lb_city_name, LV_ALIGN_TOP_LEFT, 0, 45); // align to the top right corner with some padding
    lv_label_set_text(lb_city_name, realtime.name.c_str());


    //create temperature unit label
    lb_temp_unit = lv_label_create(page);
    init = (lb_temp_unit != NULL);
    ui_state.sub_page_inited[SUB_MENU_PAGE_1] = ui_state.sub_page_inited[SUB_MENU_PAGE_1] && init; //set init inactive if any label is not created successfully
    if(!ui_state.sub_page_inited[SUB_MENU_PAGE_1]) {
      LOG_E("Failed to create temperature unit label for weather");
      return;
    }
    lv_obj_set_style_text_font(lb_temp_unit, lb_weather_realtime_temp_unit_font, LV_PART_MAIN);
    lv_obj_set_style_text_color(lb_temp_unit, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_label_set_text(lb_temp_unit, (String("°") + String("C")).c_str()); // Set temperature unit to Celsius
    lv_obj_align(lb_temp_unit, LV_ALIGN_TOP_RIGHT, -2, 8); // align to the top right corner with some padding

    //create temperature label
    lb_temp = lv_label_create(page);
    init = (lb_temp != NULL);
    ui_state.sub_page_inited[SUB_MENU_PAGE_1] = ui_state.sub_page_inited[SUB_MENU_PAGE_1] && init; //set init inactive if any label is not created successfully
    if(!ui_state.sub_page_inited[SUB_MENU_PAGE_1]) {
      LOG_E("Failed to create temperature label for weather");
      return;
    }
    lv_obj_align(lb_temp, LV_ALIGN_TOP_LEFT, SCREEN_WIDTH / 2, 0); // align to the top right corner with some padding
    lv_obj_set_style_text_font(lb_temp, lb_weather_realtime_temp_font, LV_PART_MAIN);
    lv_obj_set_style_text_color(lb_temp, lv_color_hex(0xFFFFFF), LV_PART_MAIN);

    //create feel temperature label
    lb_temp_feel = lv_label_create(page);
    init = (lb_temp_feel != NULL);
    ui_state.sub_page_inited[SUB_MENU_PAGE_1] = ui_state.sub_page_inited[SUB_MENU_PAGE_1] && init; //set init inactive if any label is not created successfully
    if(!ui_state.sub_page_inited[SUB_MENU_PAGE_1]) {
      LOG_E("Failed to create feel temperature label for weather");
      return;
    }
    lv_obj_set_style_text_font(lb_temp_feel, lb_weather_realtime_temp_feel_font, LV_PART_MAIN);
    lv_obj_set_style_text_color(lb_temp_feel, lv_color_hex(0x00FF00), LV_PART_MAIN);
    lv_obj_align(lb_temp_feel, LV_ALIGN_TOP_RIGHT, 0, 42); // align to the top right corner with some padding

    //create weather description label
    lb_weather_desc = lv_label_create(page);
    init = (lb_weather_desc != NULL);
    ui_state.sub_page_inited[SUB_MENU_PAGE_1] = ui_state.sub_page_inited[SUB_MENU_PAGE_1] && init; //set init inactive if any label is not created successfully
    if(!ui_state.sub_page_inited[SUB_MENU_PAGE_1]) {
      LOG_E("Failed to create weather description label for weather");
      return;
    }
    lv_obj_set_style_text_font(lb_weather_desc, lb_weather_realtime_desc_font, LV_PART_MAIN);
    lv_obj_set_style_text_color(lb_weather_desc, lv_color_hex(0xEE7D30), LV_PART_MAIN);
    lv_label_set_long_mode(lb_weather_desc, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_obj_set_width(lb_weather_desc, SCREEN_WIDTH); // set width to half of the screen width
    lv_obj_align(lb_weather_desc, LV_ALIGN_CENTER, 0, -30); // align to the top left corner with some padding
  }



  //update coin icon image descriptor
  weather_icon_img_dsc.header.cf = LV_IMG_CF_RAW_ALPHA;
  weather_icon_img_dsc.header.w = 0; //auto width
  weather_icon_img_dsc.header.h = 0; //auto height
  weather_icon_img_dsc.header.always_zero = 0;
  weather_icon_img_dsc.header.reserved = 0;
  weather_icon_img_dsc.data_size = realtime.weather[0].icon.size;
  weather_icon_img_dsc.data      = (const uint8_t *)(realtime.weather[0].icon.addr);
  lv_img_set_src(icon_png, &weather_icon_img_dsc);

  //update temperature label
  String temp_str = String(realtime.main.temp, 0);
  width = lv_txt_get_width(temp_str.c_str(), strlen(temp_str.c_str()), lb_weather_realtime_temp_font, 0, LV_TEXT_FLAG_NONE);
  lv_obj_set_width(lb_temp, width);
  lv_label_set_text(lb_temp, temp_str.c_str());

  //update feel temperature label
  String temp_feel_str = String(realtime.main.feels_like, 0);
  width = lv_txt_get_width(temp_feel_str.c_str(), strlen(temp_feel_str.c_str()), lb_weather_realtime_temp_feel_font, 0, LV_TEXT_FLAG_NONE);
  lv_obj_set_width(lb_temp_feel, width);
  lv_label_set_text(lb_temp_feel, temp_feel_str.c_str());

  //update weather description label
  String main_str = realtime.weather[0].main;
  String desc_str = realtime.weather[0].description;
  width = lv_txt_get_width((main_str + "/" + desc_str).c_str(), strlen((main_str + "/" + desc_str).c_str()), lb_weather_realtime_desc_font, 0, LV_TEXT_FLAG_NONE);
  width = (width > SCREEN_WIDTH) ? SCREEN_WIDTH : width; // limit width to screen width
  lv_obj_set_width(lb_weather_desc, width);
  lv_label_set_text(lb_weather_desc, (main_str + "/" + desc_str).c_str());
}


static void ui_refresh_thread(void *args){
  char *name = (char*)malloc(20);
  strcpy(name, (char*)args);
  LOG_I("%s thread started on core %d...", name, xPortGetCoreID());
  free(name);
  uint32_t circle_tick_start = millis(), last_api_tick_start[4] = {millis(), millis(), millis(), millis()};
  while (true){
    delay(100);
    if(xSemaphoreTake(lvgl_xMutex, 0) == pdTRUE){
      // circle menu page scroll
      if((ui_state.current_screen_type == MENU_SCREEN) && (ui_state.menu_page_index == MENU_PAGE_END) && (millis() - circle_tick_start >= 1000)){
        ui_state.menu_page_index = MENU_PAGE_PRICE;
        lv_obj_scroll_to_view(menu_pages[ui_state.menu_page_index], LV_ANIM_OFF);
      }
      if((ui_state.current_screen_type == MENU_SCREEN) && (ui_state.menu_page_index == MENU_PAGE_BEGIN) && (millis() - circle_tick_start >= 1000)){
        ui_state.menu_page_index = MENU_PAGE_SETTINGS;
        lv_obj_scroll_to_view(menu_pages[ui_state.menu_page_index], LV_ANIM_OFF);
      }
      // circle sub menu page scroll
      if((ui_state.current_screen_type == SUB_MENU_SCREEN) && (ui_state.sub_menu_page_index == SUB_MENU_PAGE_END) && (millis() - circle_tick_start >= 1000)){
        ui_state.sub_menu_page_index = SUB_MENU_PAGE_0;
        lv_obj_scroll_to_view(sub_menu_pages[ui_state.sub_menu_page_index], LV_ANIM_OFF);
      }
      if((ui_state.current_screen_type == SUB_MENU_SCREEN) && (ui_state.sub_menu_page_index == SUB_MENU_PAGE_BEGIN) && (millis() - circle_tick_start >= 1000)){
        ui_state.sub_menu_page_index = SUB_MENU_PAGE_5;
        lv_obj_scroll_to_view(sub_menu_pages[ui_state.sub_menu_page_index], LV_ANIM_OFF);
      }
      




      //give the semaphore to fetch coin prices
      if((ui_state.current_screen_type == SUB_MENU_SCREEN) && (ui_state.menu_page_index == MENU_PAGE_PRICE) && 
        (millis() - last_api_tick_start[0] >= PRICE_RANK_UPDATE_INTERVAL)){
          xSemaphoreGive(g_nm.global_xsem.coin_price_xsem); // give the semaphore to fetch coin prices
          last_api_tick_start[0] = millis();
      }
      //give the semaphore to fetch real-time weather data
      if((ui_state.current_screen_type == SUB_MENU_SCREEN) && (ui_state.menu_page_index == MENU_PAGE_WEATHER) && 
        (ui_state.sub_menu_page_index == SUB_MENU_PAGE_1) && //page 1 is the real-time weather page
        (millis() - last_api_tick_start[1] >= WEATHER_REALTIME_UPDATE_INTERVAL)){
          xSemaphoreGive(g_nm.global_xsem.weather_realtime_xsem); // give the semaphore to fetch real-time weather data
          last_api_tick_start[1] = millis();
      }





      // check if the next touch event is triggered
      if(xSemaphoreTake(g_nm.global_xsem.next_page_xsem, 0) == pdTRUE){
        if(ui_state.current_screen_type == MENU_SCREEN){
          lv_obj_scroll_to_view(menu_pages[++ui_state.menu_page_index], LV_ANIM_ON);
          if(ui_state.menu_page_index == MENU_PAGE_END) circle_tick_start = millis();//wait for a second before scrolling back to the first page
        }
        else if(ui_state.current_screen_type == SUB_MENU_SCREEN){
          lv_obj_scroll_to_view(sub_menu_pages[++ui_state.sub_menu_page_index], LV_ANIM_ON);
          if(ui_state.sub_menu_page_index == SUB_MENU_PAGE_END) circle_tick_start = millis();//wait for a second before scrolling back to the first page
        }
      }
      
      // check if the previous touch event is triggered
      if(xSemaphoreTake(g_nm.global_xsem.prev_page_xsem, 0) == pdTRUE){
        if(ui_state.current_screen_type == MENU_SCREEN){
          lv_obj_scroll_to_view(menu_pages[--ui_state.menu_page_index], LV_ANIM_ON);
          if(ui_state.menu_page_index == MENU_PAGE_BEGIN) circle_tick_start = millis();//wait for a second before scrolling back to the first page
        }
        else if(ui_state.current_screen_type == SUB_MENU_SCREEN){
          lv_obj_scroll_to_view(sub_menu_pages[--ui_state.sub_menu_page_index], LV_ANIM_ON);
          if(ui_state.sub_menu_page_index == SUB_MENU_PAGE_BEGIN) circle_tick_start = millis();//wait for a second before scrolling back to the first page
        }
      }
      
      // check if the ok/cancel touch event is triggered
      if(xSemaphoreTake(g_nm.global_xsem.ok_cancel_xsem, 0) == pdTRUE){
        if(ui_state.current_screen_type == MENU_SCREEN){
          ui_state.sub_menu_page_index = ui_state.menu_page_index; 
          lv_obj_scroll_to_view(sub_menu_pages[ui_state.sub_menu_page_index], LV_ANIM_ON);
          ui_state.current_screen_type = SUB_MENU_SCREEN;

          // give the semaphore to fetch coin prices immediately
          if(ui_state.menu_page_index == MENU_PAGE_PRICE){
            xSemaphoreGive(g_nm.global_xsem.coin_price_xsem); // give the semaphore to fetch coin prices
            last_api_tick_start[0] = millis(); // reset the last api tick start time
          }
          // give the semaphore to fetch real-time weather data immediately
          if(ui_state.menu_page_index == MENU_PAGE_WEATHER && ui_state.sub_menu_page_index == SUB_MENU_PAGE_1){
            xSemaphoreGive(g_nm.global_xsem.weather_realtime_xsem); // give the semaphore to fetch real-time weather data
            last_api_tick_start[1] = millis(); // reset the last api tick start time
          }

        }
        else if(ui_state.current_screen_type == SUB_MENU_SCREEN){
          //clear all sub menu pages objects
          for(uint8_t i = 0; i < SUB_MENU_PAGE_END; i++) {
            ui_sub_menu_page_obj_clear(sub_menu_pages[i]);
            ui_state.sub_page_inited[i] = false; // reset sub page inited flag
            LOG_I("Cleared sub menu page %d", i);
          }

          //clear the icon cache
          for(auto &coin : g_nm.coin_price_rank){
            if(coin.second.icon.addr == NULL) continue; //skip if icon data is not available
            free(coin.second.icon.addr); // free icon data
            coin.second.icon.addr = NULL;
            coin.second.icon.size = 0;
            LOG_W("Freed icon data for coin: %s", coin.second.name.c_str());
          }

          //switch back to the menu page
          lv_obj_scroll_to_view(menu_pages[ui_state.menu_page_index], LV_ANIM_ON);
          ui_state.current_screen_type = MENU_SCREEN;

        }
      } 
      
      //refresh current sub menu page
      if(ui_state.current_screen_type == SUB_MENU_SCREEN) {
          auto func = refresh_table[ui_state.sub_menu_page_index][ui_state.menu_page_index];
          if(func) func();
      }

      //release mutex
      xSemaphoreGive(lvgl_xMutex); 
    }

    if(g_nm.screen.sleep_timeout > 0){
      int cycle = (g_nm.screen.sleep_timeout - g_nm.screen.cnt);
      cycle = cycle > 0 ? cycle : 0;
      uint8_t full = (uint8_t)(((float)g_nm.screen.brightness/100.0f) * 255);
      cycle = cycle * full / g_nm.screen.sleep_timeout;
      tft_bl_ctrl((float)cycle / 255.0f);
      g_nm.screen.active = cycle > 0;
    }else {
      tft_bl_ctrl((float)(g_nm.screen.brightness/100.0f));
    }
  }
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

  //lvgl tick task
  String taskName = "(lvgltick)";
  xTaskCreatePinnedToCore(lvgl_tick_task, taskName.c_str(), 1024*5, (void*)taskName.c_str(), TASK_PRIORITY_LVGL_DRV, &task_lvgl_tick, LvglTaskCore);
  delay(500);//wait a bit for lvgl tick task to start, necessary for lvgl to work properly

  taskName = "(uirefresh)";
  xTaskCreatePinnedToCore(ui_refresh_thread, taskName.c_str(), 1024*5, (void*)taskName.c_str(), TASK_PRIORITY_UI_REFRESH, &task_ui_refresh, UiRefreshTaskCore);
  delay(100);


  ui_boot_page_layout_init();
  delay(3000);         
  ui_working_page_layout_init();

  /***************************************switch to main page*************************************/
  // ui_switch_to_page(&ui_state, true);
  lv_obj_scroll_to_view(price_menu_page, LV_ANIM_ON);
  vTaskDelete(NULL);
}
