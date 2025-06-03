/*******************************************************************************
 * Size: 8 px
 * Bpp: 4
 * Opts: --bpp 4 --size 8 --no-compress --font fa-solid-900.woff --range 62781,61463,61465,61568,62152,62338,62845,63587 --format lvgl -o symbol_8.c
 ******************************************************************************/

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

#ifndef SYMBOL_8
#define SYMBOL_8 1
#endif

#if SYMBOL_8

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+F017 "" */
    0x1, 0x9d, 0xd8, 0x0, 0x1e, 0xfb, 0xcf, 0xd0,
    0x9f, 0xf7, 0x8f, 0xf8, 0xdf, 0xf7, 0x8f, 0xfc,
    0xdf, 0xfa, 0x1c, 0xfc, 0x9f, 0xff, 0xbe, 0xf8,
    0x1e, 0xff, 0xff, 0xd0, 0x1, 0x9d, 0xd8, 0x0,

    /* U+F019 "" */
    0x0, 0xf, 0xf0, 0x0, 0x0, 0xf, 0xf0, 0x0,
    0x0, 0xf, 0xf0, 0x0, 0x7, 0xff, 0xff, 0x70,
    0x0, 0x9f, 0xf9, 0x0, 0x78, 0x7a, 0xa7, 0x87,
    0xff, 0xfb, 0xbf, 0xff, 0xff, 0xff, 0xfb, 0xbf,

    /* U+F080 "" */
    0xf0, 0x3, 0x30, 0x77, 0xf0, 0x8, 0x87, 0x88,
    0xf0, 0x78, 0x8f, 0x88, 0xf0, 0xf7, 0x7f, 0x77,
    0xf0, 0x0, 0x0, 0x0, 0xef, 0xff, 0xff, 0xff,

    /* U+F2C8 "" */
    0x1c, 0xc4, 0x7, 0x61, 0xb0, 0x7a, 0xab, 0x7,
    0xbb, 0xb0, 0x8b, 0xbc, 0xc, 0xce, 0xc2, 0xcc,
    0xec, 0x14, 0xcc, 0x70,

    /* U+F382 "" */
    0x0, 0x2, 0x30, 0x0, 0x0, 0x0, 0xbf, 0xfa,
    0x22, 0x0, 0x5, 0xff, 0xbf, 0xff, 0x40, 0x1c,
    0xf7, 0x7, 0xff, 0x80, 0xbf, 0xc3, 0x3, 0xdf,
    0xf8, 0xff, 0xfe, 0xe, 0xff, 0xff, 0xbf, 0xfe,
    0x4e, 0xff, 0xfc, 0x18, 0xbb, 0xbb, 0xbb, 0x91,

    /* U+F53D "" */
    0xab, 0xbb, 0xbb, 0xbb, 0xb9, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xfe, 0x4e, 0xec, 0xcc, 0xcf, 0xfa,
    0x6e, 0x94, 0x44, 0x4f, 0xff, 0x8b, 0xdb, 0xec,
    0xbf, 0xff, 0x8f, 0xec, 0xfd, 0xcf, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xab, 0xbb, 0xbb, 0xbb, 0xba,

    /* U+F57D "" */
    0x0, 0x69, 0x94, 0x0, 0x1c, 0x84, 0xef, 0x90,
    0x95, 0x2, 0xff, 0xf5, 0xe7, 0x4c, 0xff, 0x3a,
    0xff, 0x98, 0xbf, 0x9b, 0xbf, 0xf6, 0xc, 0xf7,
    0x2e, 0xfc, 0x3f, 0xc0, 0x2, 0x9d, 0xc8, 0x0,

    /* U+F863 "" */
    0x0, 0x0, 0x0, 0x0, 0x0, 0x1, 0xb7, 0x0,
    0x0, 0x0, 0xbf, 0x60, 0x0, 0x0, 0xf, 0xfe,
    0xfc, 0x20, 0x76, 0xed, 0xdf, 0xfc, 0xb, 0xff,
    0xdd, 0xe6, 0x70, 0x1b, 0xfe, 0xff, 0x0, 0x0,
    0x0, 0x5f, 0xc0, 0x0, 0x0, 0x6, 0xc2, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 128, .box_w = 8, .box_h = 8, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 32, .adv_w = 128, .box_w = 8, .box_h = 8, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 64, .adv_w = 128, .box_w = 8, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 88, .adv_w = 64, .box_w = 5, .box_h = 8, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 108, .adv_w = 160, .box_w = 10, .box_h = 8, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 148, .adv_w = 160, .box_w = 10, .box_h = 8, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 188, .adv_w = 124, .box_w = 8, .box_h = 8, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 220, .adv_w = 128, .box_w = 9, .box_h = 10, .ofs_x = 0, .ofs_y = -2}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/

static const uint16_t unicode_list_0[] = {
    0x0, 0x2, 0x69, 0x2b1, 0x36b, 0x526, 0x566, 0x84c
};

/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 61463, .range_length = 2125, .glyph_id_start = 1,
        .unicode_list = unicode_list_0, .glyph_id_ofs_list = NULL, .list_length = 8, .type = LV_FONT_FMT_TXT_CMAP_SPARSE_TINY
    }
};



/*--------------------
 *  ALL CUSTOM DATA
 *--------------------*/

#if LVGL_VERSION_MAJOR == 8
/*Store all the custom data of the font*/
static  lv_font_fmt_txt_glyph_cache_t cache;
#endif

#if LVGL_VERSION_MAJOR >= 8
static const lv_font_fmt_txt_dsc_t font_dsc = {
#else
static lv_font_fmt_txt_dsc_t font_dsc = {
#endif
    .glyph_bitmap = glyph_bitmap,
    .glyph_dsc = glyph_dsc,
    .cmaps = cmaps,
    .kern_dsc = NULL,
    .kern_scale = 0,
    .cmap_num = 1,
    .bpp = 4,
    .kern_classes = 0,
    .bitmap_format = 0,
#if LVGL_VERSION_MAJOR == 8
    .cache = &cache
#endif
};

extern const lv_font_t symbol_8;


/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
#if LVGL_VERSION_MAJOR >= 8
const lv_font_t symbol_8 = {
#else
lv_font_t symbol_8 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 10,          /*The maximum line height required by the font*/
    .base_line = 2,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = -1,
    .underline_thickness = 0,
#endif
    .dsc = &font_dsc,          /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
#if LV_VERSION_CHECK(8, 2, 0) || LVGL_VERSION_MAJOR >= 9
    .fallback = &symbol_8,
#endif
    .user_data = NULL,
};



#endif /*#if SYMBOL_8*/

