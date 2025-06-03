#include "scrsaver.h"
#include "logger.h"


#ifdef HAS_SCREEN_SAVER_FEATURE

#define STAR_COUNT 20       // 星星数量
#define MAX_DEPTH 10        // 最大深度层级
#define MIN_STAR_SIZE 2     // 最小星星尺寸
#define MAX_STAR_SIZE 5     // 最大星星尺寸

// 各参数对效果的影响
// 参数	作用域	调大效果	调小效果
// alpha_dir * n	单次透明度变化	渐变更急促	渐变更平滑
// lifespan	星星总生存时间	星星存在更久	星星快速消失
// 定时器间隔	全局刷新频率	动画变卡顿但省资源	动画更流畅但耗资源


// // 生成随机星空颜色
// static lv_color_t random_star_color() {
//     uint8_t r = 0, g = 0, b = 0;
//     uint8_t type = rand() % 6;
    
//     switch(type) {
//         case 0: // 蓝色调
//             b = 200 + rand() % 55;
//             g = 100 + rand() % 100;
//             break;
//         case 1: // 黄色调
//             r = 200 + rand() % 55;
//             g = 180 + rand() % 75;
//             break;
//         case 2: // 红色调
//             r = 200 + rand() % 55;
//             break;
//         case 3: // 白色
//             r = g = b = 200 + rand() % 55;
//             break;
//         default: // 蓝白色
//             r = g = 150 + rand() % 105;
//             b = 200 + rand() % 55;
//     }
    
//     return lv_color_make(r, g, b);
// }



// 生成随机星空颜色（柔和版）
static lv_color_t random_star_color() {
    uint8_t r = 0, g = 0, b = 0;
    uint8_t type = rand() % 10;  // 扩大范围以便更精细控制概率
    
    switch(type) {
        case 0: // 冷蓝色（概率10%）
            b = 180 + rand() % 75;
            g = 80 + rand() % 100;
            break;
            
        case 1: // 暖黄色（概率10%）
            r = 180 + rand() % 50;
            g = 160 + rand() % 60;
            b = 50 + rand() % 50;
            break;
            
        case 2: // 极低饱和度红色（概率10%，比原版更柔和）
            r = 150 + rand() % 50;
            g = b = 100 + rand() % 80;
            break;
            
        case 3: // 纯白色（概率30%）
        case 4:
        case 5:
            r = g = b = 200 + rand() % 55;
            break;
            
        default: // 蓝白色（概率40%，星空主色调）
            r = g = 170 + rand() % 85;
            b = 200 + rand() % 55;
    }
    
    // 可选：添加微弱的随机亮度波动
    if(rand() % 3 == 0) {
        uint8_t dim = 20 + rand() % 30;
        r = LV_MAX(r - dim, 0);
        g = LV_MAX(g - dim, 0);
        b = LV_MAX(b - dim, 0);
    }
    
    return lv_color_make(r, g, b);
}


// 创建/重置星星
void reset_star(star_t *star, lv_obj_t *parent) {
    if(!star->active) {
        // 随机位置
        star->x = rand() % lv_obj_get_width(parent);
        star->y = rand() % lv_obj_get_height(parent);
        
        // 随机大小
        star->size = MIN_STAR_SIZE + rand() % (MAX_STAR_SIZE - MIN_STAR_SIZE + 1);
        
        // 随机颜色
        star->color = random_star_color();
        
        // 初始透明度为0
        star->alpha = 0;
        star->alpha_dir = 1; // 开始变亮
        star->lifespan = 10 + rand() % 50; // 随机生命周期
        star->active = true;
        
        // 创建或更新LVGL对象
        if(star->obj == NULL) {
            star->obj = lv_obj_create(parent);
            lv_obj_remove_style_all(star->obj);
            lv_obj_set_style_radius(star->obj, LV_RADIUS_CIRCLE, 0);
        }
        
        lv_obj_set_size(star->obj, star->size, star->size);
        lv_obj_set_pos(star->obj, star->x, star->y);
        lv_obj_set_style_bg_color(star->obj, star->color, 0);
        lv_obj_set_style_bg_opa(star->obj, star->alpha, 0);
    }
}

// 更新星星状态
void update_star(star_t *star, lv_obj_t *parent) {
    if(!star->active) {
        // 随机决定是否生成新星星
        if(rand() % 100 < 80) { // 80%的几率生成新星星
            reset_star(star, parent);
        }
        return;
    }
    
    // 更新透明度 渐变更急促 或者 渐变更平滑
    star->alpha += star->alpha_dir * 6;
    
    // 检查透明度边界
    if(star->alpha >= 255) {
        star->alpha = 255;
        star->alpha_dir = -1; // 开始变暗
    } else if(star->alpha <= 0) {
        star->alpha = 0;
        star->active = false; // 星星消失
        return;
    }
    
    // 更新LVGL对象
    lv_obj_set_style_bg_opa(star->obj, star->alpha, 0);
    
    // 减少生命周期
    if(--star->lifespan <= 0) {
        star->alpha_dir = -1; // 开始消失
    }
}
#endif // HAS_SCREEN_SAVER_FEATURE