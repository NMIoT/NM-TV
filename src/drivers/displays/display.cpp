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
static lv_obj_t *g_menu_pages[MENU_PAGE_END] = {NULL,},
                *g_sub_menu_pages[SUB_MENU_PAGE_END] = {NULL,};
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
  int menu_page_index; // current menu page index
  int sub_menu_page_index; // current sub menu page index
  screen_type_t current_screen_type; // current screen type, MENU_SCREEN or SUB_MENU_SCREEN
}ui_state_t;

ui_state_t g_ui_state = {
  .menu_page_index          = MENU_PAGE_PRICE,
  .sub_menu_page_index      = SUB_MENU_PAGE_0,
  .current_screen_type      = MENU_SCREEN,
};



#include "image_240_240.h"
// LV_FONT_DECLARE(ds_digib_font_10)
// LV_FONT_DECLARE(symbol_10)
static const lv_font_t *lb_price_rank_font = &lv_font_montserrat_34;
static const lv_font_t *lb_menu_title_font = &lv_font_montserrat_34;


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

  // Create g_menu_pages array
  g_menu_pages[MENU_PAGE_BEGIN]        = clone_setting_menu_page;
  g_menu_pages[MENU_PAGE_PRICE]        = price_menu_page;
  g_menu_pages[MENU_PAGE_WEATHER]      = weather_menu_page; 
  g_menu_pages[MENU_PAGE_CLOCK]        = clock_menu_page; 
  g_menu_pages[MENU_PAGE_IDEA]         = idea_menu_page;
  g_menu_pages[MENU_PAGE_ALBUM]        = album_menu_page;
  g_menu_pages[MENU_PAGE_SETTINGS]     = settings_menu_page;
  g_menu_pages[MENU_PAGE_END]          = clone_price_menu_page;   // clone first page to the end for easy access



/*************************************** sub menu layout******************************************/
  // Create sub menu page5 clone
  sub_menu_page_0 = lv_obj_create(parent_docker);
  lv_obj_set_size(sub_menu_page_0, SCREEN_WIDTH, SCREEN_HEIGHT);
  lv_obj_set_pos(sub_menu_page_0, 0, SCREEN_HEIGHT);
  lv_obj_set_style_pad_all(sub_menu_page_0, 0, 0);
  lv_obj_set_style_border_width(sub_menu_page_0, 0, 0);
  lv_obj_set_scrollbar_mode(sub_menu_page_0, LV_SCROLLBAR_MODE_OFF); 
  background_img_obj = lv_img_create(sub_menu_page_0);
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

  // Create g_sub_menu_pages array
  g_sub_menu_pages[SUB_MENU_PAGE_BEGIN] = sub_menu_page_5_clone;
  g_sub_menu_pages[SUB_MENU_PAGE_0]     = sub_menu_page_0;
  g_sub_menu_pages[SUB_MENU_PAGE_1]     = sub_menu_page_1;
  g_sub_menu_pages[SUB_MENU_PAGE_2]     = sub_menu_page_2;
  g_sub_menu_pages[SUB_MENU_PAGE_3]     = sub_menu_page_3;
  g_sub_menu_pages[SUB_MENU_PAGE_4]     = sub_menu_page_4;
  g_sub_menu_pages[SUB_MENU_PAGE_5]     = sub_menu_page_5;
  g_sub_menu_pages[SUB_MENU_PAGE_END]   = sub_menu_page_0_clone;
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

static void ui_update_loading_string(String str, uint32_t color, bool prgress_update) {

}


static void ui_price_page_rank_refresh(std::map<ccoin_name, ccoin_node> &map){
  // https://s2.coinmarketcap.com/static/img/coins/32x32/1.png
  if(map.empty()) return;
  if(g_menu_pages[MENU_PAGE_PRICE] == NULL) return;

  static const uint8_t rank_max = 7; // Maximum number of coins to display
  static lv_img_dsc_t coin_icon_img_dsc[rank_max];
  static lv_obj_t* icon_png_list[rank_max] = {NULL,};
  static lv_obj_t* lb_crypto_coin_price[rank_max] = {NULL,};

  // Initialize the icon image descriptors and labels if not already done
  for(uint8_t i = 0; i < rank_max; i++) {
      //create or update the icon image
      if(icon_png_list[i] == NULL) {
          icon_png_list[i] = lv_img_create(price_menu_page);
          lv_obj_align(icon_png_list[i], LV_ALIGN_TOP_LEFT, 0, i * 33 + 2);
      }
      if(lb_crypto_coin_price[i] == NULL) {
          lb_crypto_coin_price[i] = lv_label_create(price_menu_page);
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



static void ui_switch_to_page(ui_state_t *ustate, bool anim){
  if(ustate == NULL) return;
  if(ustate->current_screen_type == MENU_SCREEN){
    lv_obj_scroll_to_view(g_menu_pages[ustate->menu_page_index], anim ? LV_ANIM_ON : LV_ANIM_OFF);
    LOG_W("Switch to menu page: %d", ustate->menu_page_index);
  }
  else if(ustate->current_screen_type == SUB_MENU_SCREEN){
    lv_obj_scroll_to_view(g_sub_menu_pages[ustate->sub_menu_page_index], anim ? LV_ANIM_ON : LV_ANIM_OFF);
    LOG_W("Switch to sub menu page: %d", ustate->sub_menu_page_index);
  }
}

static void ui_refresh_thread(void *args){
  char *name = (char*)malloc(20);
  strcpy(name, (char*)args);
  LOG_I("%s thread started on core %d...", name, xPortGetCoreID());
  free(name);

  while (true){
    delay(1000);

    if(xSemaphoreTake(lvgl_xMutex, 0) == pdTRUE){
        //release mutex
        xSemaphoreGive(lvgl_xMutex); 
    }

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

void ui_switch_next_page_cb(){
  if(g_ui_state.current_screen_type == MENU_SCREEN){
    g_ui_state.menu_page_index++;
    ui_switch_to_page(&g_ui_state, true);
    if(g_ui_state.menu_page_index == MENU_PAGE_END){
      g_ui_state.menu_page_index = MENU_PAGE_PRICE;
      delay(1000);
      ui_switch_to_page(&g_ui_state, false);
    }
  }
  else if(g_ui_state.current_screen_type == SUB_MENU_SCREEN){
    g_ui_state.sub_menu_page_index++;
    ui_switch_to_page(&g_ui_state, true);
    if(g_ui_state.sub_menu_page_index == SUB_MENU_PAGE_END){
      g_ui_state.sub_menu_page_index = SUB_MENU_PAGE_0;
      delay(1000);
      ui_switch_to_page(&g_ui_state, false);
    }
  }
}




void ui_switch_previous_page_cb(){
 if(g_ui_state.current_screen_type == MENU_SCREEN){
    g_ui_state.menu_page_index--;
    ui_switch_to_page(&g_ui_state, true);
    if(g_ui_state.menu_page_index == MENU_PAGE_BEGIN){
      g_ui_state.menu_page_index = MENU_PAGE_SETTINGS;
      delay(1000);
      ui_switch_to_page(&g_ui_state, false);
    }
  }
  else if(g_ui_state.current_screen_type == SUB_MENU_SCREEN){
    g_ui_state.sub_menu_page_index--;
    ui_switch_to_page(&g_ui_state, true);
    if(g_ui_state.sub_menu_page_index == SUB_MENU_PAGE_BEGIN){
      g_ui_state.sub_menu_page_index = SUB_MENU_PAGE_5;
      delay(1000);
      ui_switch_to_page(&g_ui_state, false);
    }
  }
}


void ui_enter_or_exit_current_page_cb(){
  // if(ui_state.current_screen_type == SUB_MENU_SCREEN){
  //   ui_switch_to_page(ui_state.menu_page_index, true);
  // }
  // else if(ui_state.current_screen_type == MENU_SCREEN){
  //   ui_switch_to_page(ui_state.sub_menu_page_index, true);
  // }
  // else {
  //   LOG_E("Unknown screen type: %d", ui_state.current_screen_type);
  // }
  // ui_state.current_screen_type = (ui_state.current_screen_type == MENU_SCREEN) ? SUB_MENU_SCREEN : MENU_SCREEN;

  if(g_ui_state.current_screen_type == MENU_SCREEN){
    g_ui_state.current_screen_type = SUB_MENU_SCREEN;
    g_ui_state.sub_menu_page_index = SUB_MENU_PAGE_0;
    ui_switch_to_page(&g_ui_state, true);
  }
  else if(g_ui_state.current_screen_type == SUB_MENU_SCREEN){
    g_ui_state.current_screen_type = MENU_SCREEN;
    g_ui_state.menu_page_index = MENU_PAGE_PRICE;
    ui_switch_to_page(&g_ui_state, true);
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
  xTaskCreatePinnedToCore(lvgl_tick_task, taskName.c_str(), 1024*4, (void*)taskName.c_str(), TASK_PRIORITY_LVGL_DRV, &task_lvgl_tick, LvglTaskCore);
  delay(500);//wait a bit for lvgl tick task to start, necessary for lvgl to work properly

  taskName = "(uirefresh)";
  xTaskCreatePinnedToCore(ui_refresh_thread, taskName.c_str(), 1024*2.5, (void*)taskName.c_str(), TASK_PRIORITY_UI_REFRESH, &task_ui_refresh, UiRefreshTaskCore);
  delay(100);


  ui_boot_page_layout_init();
  delay(3000);         
  ui_working_page_layout_init();

  /***************************************switch to main page*************************************/
  ui_switch_to_page(&g_ui_state, true);
  vTaskDelete(NULL);
}
#endif //TFT_DISPLAY
