
#ifndef __SCREEN_SAVER_H__
#define __SCREEN_SAVER_H__
#include "device.h"
#ifdef HAS_SCREEN_SAVER_FEATURE
#include "lvgl.h"


typedef struct {
    lv_obj_t *obj;          // LVGL对象
    int16_t x;              // x坐标
    int16_t y;              // y坐标
    uint8_t size;           // 星星大小
    lv_color_t color;       // 星星颜色
    int16_t alpha;          // 当前透明度
    int8_t alpha_dir;       // 透明度变化方向(1:变亮, -1:变暗)
    uint16_t lifespan;      // 生命周期计数器
    bool active;            // 是否活跃
} star_t;

void reset_star(star_t *star, lv_obj_t *parent);
void update_star(star_t *star, lv_obj_t *parent);
#endif

#endif 


