/*******************************************************************************
 * Size: 8 px
 * Bpp: 1
 * Opts: --bpp 1 --size 8 --no-compress --font DS_DIGIB.TTF --range 32-127 --format lvgl -o ds_digib_font_8.c
 ******************************************************************************/

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

#ifndef DS_DIGIB_FONT_8
#define DS_DIGIB_FONT_8 1
#endif

#if DS_DIGIB_FONT_8

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+0020 " " */
    0x0,

    /* U+0021 "!" */
    0xf4,

    /* U+0022 "\"" */
    0xc0,

    /* U+0023 "#" */
    0x6d, 0xf6,

    /* U+0024 "$" */
    0x4e, 0x23, 0x90,

    /* U+0025 "%" */
    0x55, 0x94, 0x75, 0x80,

    /* U+0026 "&" */
    0x4a, 0x5a, 0x40,

    /* U+0027 "'" */
    0x80,

    /* U+0028 "(" */
    0x62, 0x40,

    /* U+0029 ")" */
    0x91, 0x80,

    /* U+002A "*" */
    0x5f, 0xa0,

    /* U+002B "+" */
    0x55, 0x0,

    /* U+002C "," */
    0x80,

    /* U+002D "-" */
    0xe0,

    /* U+002E "." */
    0x80,

    /* U+002F "/" */
    0x22, 0x4, 0x80,

    /* U+0030 "0" */
    0x54, 0x54,

    /* U+0031 "1" */
    0x50,

    /* U+0032 "2" */
    0x45, 0x44,

    /* U+0033 "3" */
    0xc7, 0x1c,

    /* U+0034 "4" */
    0xb5, 0x12,

    /* U+0035 "5" */
    0x51, 0x14,

    /* U+0036 "6" */
    0x51, 0x54,

    /* U+0037 "7" */
    0xc4, 0x92,

    /* U+0038 "8" */
    0x55, 0x54,

    /* U+0039 "9" */
    0x55, 0x14,

    /* U+003A ":" */
    0x90,

    /* U+003B ";" */
    0x88,

    /* U+003C "<" */
    0x6c,

    /* U+003D "=" */
    0xfc,

    /* U+003E ">" */
    0x9c,

    /* U+003F "?" */
    0xc5, 0x48, 0x20,

    /* U+0040 "@" */
    0x6b, 0x5a, 0x70,

    /* U+0041 "A" */
    0x55, 0x5a,

    /* U+0042 "B" */
    0x55, 0x54,

    /* U+0043 "C" */
    0x70, 0x46,

    /* U+0044 "D" */
    0x54, 0x54,

    /* U+0045 "E" */
    0x71, 0xc6,

    /* U+0046 "F" */
    0x71, 0xc8,

    /* U+0047 "G" */
    0x71, 0x5e,

    /* U+0048 "H" */
    0xb5, 0x5a,

    /* U+0049 "I" */
    0xd8,

    /* U+004A "J" */
    0x11, 0x19, 0x60,

    /* U+004B "K" */
    0xb9, 0x5a,

    /* U+004C "L" */
    0x92, 0x46,

    /* U+004D "M" */
    0x5f, 0xda,

    /* U+004E "N" */
    0xf4, 0x5a,

    /* U+004F "O" */
    0x54, 0x54,

    /* U+0050 "P" */
    0x55, 0x48,

    /* U+0051 "Q" */
    0x54, 0x56,

    /* U+0052 "R" */
    0x55, 0x6a,

    /* U+0053 "S" */
    0x71, 0x1c,

    /* U+0054 "T" */
    0xe9, 0x24,

    /* U+0055 "U" */
    0xb6, 0xd4,

    /* U+0056 "V" */
    0x16, 0xdc,

    /* U+0057 "W" */
    0xb7, 0xf4,

    /* U+0058 "X" */
    0xb4, 0x5a,

    /* U+0059 "Y" */
    0x99, 0x61, 0x60,

    /* U+005A "Z" */
    0xe0, 0x4c,

    /* U+005B "[" */
    0x62, 0x40,

    /* U+005C "\\" */
    0x44, 0x2, 0x10,

    /* U+005D "]" */
    0x91, 0x80,

    /* U+005E "^" */
    0x14,

    /* U+005F "_" */
    0xf0,

    /* U+0060 "`" */
    0x80,

    /* U+0061 "a" */
    0x55, 0x5a,

    /* U+0062 "b" */
    0x55, 0x54,

    /* U+0063 "c" */
    0x70, 0x46,

    /* U+0064 "d" */
    0x54, 0x54,

    /* U+0065 "e" */
    0x71, 0xc6,

    /* U+0066 "f" */
    0x71, 0xc8,

    /* U+0067 "g" */
    0x71, 0x5e,

    /* U+0068 "h" */
    0xb5, 0x5a,

    /* U+0069 "i" */
    0xd8,

    /* U+006A "j" */
    0x11, 0x19, 0x60,

    /* U+006B "k" */
    0xb9, 0x5a,

    /* U+006C "l" */
    0x92, 0x46,

    /* U+006D "m" */
    0x5f, 0xda,

    /* U+006E "n" */
    0xf4, 0x5a,

    /* U+006F "o" */
    0x54, 0x54,

    /* U+0070 "p" */
    0x55, 0x48,

    /* U+0071 "q" */
    0x54, 0x56,

    /* U+0072 "r" */
    0x55, 0x6a,

    /* U+0073 "s" */
    0x71, 0x1c,

    /* U+0074 "t" */
    0xe9, 0x24,

    /* U+0075 "u" */
    0xb6, 0xd4,

    /* U+0076 "v" */
    0x16, 0xdc,

    /* U+0077 "w" */
    0xb7, 0xf4,

    /* U+0078 "x" */
    0xb4, 0x5a,

    /* U+0079 "y" */
    0x99, 0x61, 0x60,

    /* U+007A "z" */
    0xe0, 0x4c,

    /* U+007B "{" */
    0x2a, 0x22,

    /* U+007C "|" */
    0xfc,

    /* U+007D "}" */
    0x88, 0xa8,

    /* U+007E "~" */
    0xe0
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 30, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1, .adv_w = 37, .box_w = 1, .box_h = 6, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 2, .adv_w = 40, .box_w = 2, .box_h = 1, .ofs_x = 1, .ofs_y = 4},
    {.bitmap_index = 3, .adv_w = 77, .box_w = 4, .box_h = 4, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 5, .adv_w = 65, .box_w = 3, .box_h = 7, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 8, .adv_w = 89, .box_w = 5, .box_h = 6, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 12, .adv_w = 70, .box_w = 4, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 15, .adv_w = 23, .box_w = 1, .box_h = 1, .ofs_x = 1, .ofs_y = 4},
    {.bitmap_index = 16, .adv_w = 40, .box_w = 2, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 18, .adv_w = 40, .box_w = 2, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 20, .adv_w = 62, .box_w = 3, .box_h = 4, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 22, .adv_w = 60, .box_w = 3, .box_h = 3, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 24, .adv_w = 23, .box_w = 1, .box_h = 1, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 25, .adv_w = 60, .box_w = 3, .box_h = 1, .ofs_x = 0, .ofs_y = 2},
    {.bitmap_index = 26, .adv_w = 23, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 27, .adv_w = 63, .box_w = 4, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 30, .adv_w = 65, .box_w = 3, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 32, .adv_w = 37, .box_w = 1, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 33, .adv_w = 65, .box_w = 3, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 35, .adv_w = 65, .box_w = 3, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 37, .adv_w = 65, .box_w = 3, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 39, .adv_w = 65, .box_w = 3, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 41, .adv_w = 65, .box_w = 3, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 43, .adv_w = 65, .box_w = 3, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 45, .adv_w = 65, .box_w = 3, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 47, .adv_w = 65, .box_w = 3, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 49, .adv_w = 28, .box_w = 1, .box_h = 4, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 50, .adv_w = 28, .box_w = 1, .box_h = 5, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 51, .adv_w = 46, .box_w = 2, .box_h = 3, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 52, .adv_w = 60, .box_w = 3, .box_h = 2, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 53, .adv_w = 46, .box_w = 2, .box_h = 3, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 54, .adv_w = 65, .box_w = 3, .box_h = 7, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 57, .adv_w = 79, .box_w = 4, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 60, .adv_w = 65, .box_w = 3, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 62, .adv_w = 65, .box_w = 3, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 64, .adv_w = 65, .box_w = 3, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 66, .adv_w = 65, .box_w = 3, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 68, .adv_w = 65, .box_w = 3, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 70, .adv_w = 65, .box_w = 3, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 72, .adv_w = 65, .box_w = 3, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 74, .adv_w = 65, .box_w = 3, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 76, .adv_w = 37, .box_w = 1, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 77, .adv_w = 65, .box_w = 4, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 80, .adv_w = 65, .box_w = 3, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 82, .adv_w = 65, .box_w = 3, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 84, .adv_w = 65, .box_w = 3, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 86, .adv_w = 65, .box_w = 3, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 88, .adv_w = 65, .box_w = 3, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 90, .adv_w = 65, .box_w = 3, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 92, .adv_w = 65, .box_w = 3, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 94, .adv_w = 65, .box_w = 3, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 96, .adv_w = 65, .box_w = 3, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 98, .adv_w = 65, .box_w = 3, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 100, .adv_w = 65, .box_w = 3, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 102, .adv_w = 62, .box_w = 3, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 104, .adv_w = 65, .box_w = 3, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 106, .adv_w = 62, .box_w = 3, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 108, .adv_w = 65, .box_w = 4, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 111, .adv_w = 65, .box_w = 3, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 113, .adv_w = 40, .box_w = 2, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 115, .adv_w = 63, .box_w = 4, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 118, .adv_w = 40, .box_w = 2, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 120, .adv_w = 59, .box_w = 3, .box_h = 2, .ofs_x = 1, .ofs_y = 3},
    {.bitmap_index = 121, .adv_w = 60, .box_w = 4, .box_h = 1, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 122, .adv_w = 31, .box_w = 2, .box_h = 1, .ofs_x = 0, .ofs_y = 4},
    {.bitmap_index = 123, .adv_w = 65, .box_w = 3, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 125, .adv_w = 65, .box_w = 3, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 127, .adv_w = 65, .box_w = 3, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 129, .adv_w = 65, .box_w = 3, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 131, .adv_w = 65, .box_w = 3, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 133, .adv_w = 65, .box_w = 3, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 135, .adv_w = 65, .box_w = 3, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 137, .adv_w = 65, .box_w = 3, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 139, .adv_w = 37, .box_w = 1, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 140, .adv_w = 65, .box_w = 4, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 143, .adv_w = 65, .box_w = 3, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 145, .adv_w = 65, .box_w = 3, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 147, .adv_w = 65, .box_w = 3, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 149, .adv_w = 65, .box_w = 3, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 151, .adv_w = 65, .box_w = 3, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 153, .adv_w = 65, .box_w = 3, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 155, .adv_w = 65, .box_w = 3, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 157, .adv_w = 65, .box_w = 3, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 159, .adv_w = 65, .box_w = 3, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 161, .adv_w = 65, .box_w = 3, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 163, .adv_w = 65, .box_w = 3, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 165, .adv_w = 62, .box_w = 3, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 167, .adv_w = 65, .box_w = 3, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 169, .adv_w = 62, .box_w = 3, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 171, .adv_w = 65, .box_w = 4, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 174, .adv_w = 65, .box_w = 3, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 176, .adv_w = 50, .box_w = 3, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 178, .adv_w = 40, .box_w = 1, .box_h = 6, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 179, .adv_w = 50, .box_w = 3, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 181, .adv_w = 54, .box_w = 3, .box_h = 1, .ofs_x = 0, .ofs_y = 2}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/



/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 32, .range_length = 95, .glyph_id_start = 1,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
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
    .bpp = 1,
    .kern_classes = 0,
    .bitmap_format = 0,
#if LVGL_VERSION_MAJOR == 8
    .cache = &cache
#endif
};

extern const lv_font_t ds_digib_font_8;


/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
#if LVGL_VERSION_MAJOR >= 8
const lv_font_t ds_digib_font_8 = {
#else
lv_font_t ds_digib_font_8 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 8,          /*The maximum line height required by the font*/
    .base_line = 2,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = -1,
    .underline_thickness = 1,
#endif
    .dsc = &font_dsc,          /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
#if LV_VERSION_CHECK(8, 2, 0) || LVGL_VERSION_MAJOR >= 9
    .fallback = &ds_digib_font_8,
#endif
    .user_data = NULL,
};



#endif /*#if DS_DIGIB_FONT_8*/

