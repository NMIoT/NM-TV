/*******************************************************************************
 * Size: 12 px
 * Bpp: 4
 * Opts: --bpp 4 --size 12 --no-compress --font DS_DIGIB.TTF --range 32-127 --format lvgl -o ds_digib_font_12.c
 ******************************************************************************/

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

#ifndef DS_DIGIB_FONT_12
#define DS_DIGIB_FONT_12 1
#endif

#if DS_DIGIB_FONT_12

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+0020 " " */

    /* U+0021 "!" */
    0x1, 0x94, 0xd4, 0xc4, 0x20, 0xb4, 0xd4, 0xa4,
    0x1, 0xc4, 0x10,

    /* U+0022 "\"" */
    0x7a, 0xf3, 0x69, 0xd2, 0x0, 0x0,

    /* U+0023 "#" */
    0x0, 0x90, 0x81, 0x0, 0xf, 0x2e, 0x30, 0x2f,
    0x5e, 0x6d, 0x50, 0x2e, 0x3d, 0x40, 0x2f, 0x5e,
    0x6d, 0x50, 0x2d, 0x3d, 0x40, 0x0, 0xa0, 0x91,
    0x0,

    /* U+0024 "$" */
    0x0, 0x0, 0x0, 0x0, 0x75, 0x0, 0x0, 0x53,
    0x0, 0x5, 0xff, 0xf3, 0x4b, 0x33, 0x30, 0x5b,
    0x0, 0x0, 0x9, 0xff, 0x90, 0x0, 0x33, 0x86,
    0x0, 0x0, 0xa7, 0x2, 0x33, 0x95, 0x2f, 0xff,
    0x70, 0x0, 0x53, 0x0, 0x0, 0x85, 0x0, 0x0,
    0x0, 0x0,

    /* U+0025 "%" */
    0x0, 0x0, 0x0, 0x51, 0x4, 0xe0, 0x8, 0xc0,
    0x4a, 0x4c, 0x1f, 0x40, 0x5b, 0x2e, 0x8b, 0x0,
    0x3, 0xd1, 0x63, 0x0, 0x0, 0xa, 0x68, 0xb0,
    0x0, 0x4e, 0x66, 0x6a, 0x0, 0xd7, 0x76, 0x6a,
    0x2, 0xd0, 0x8, 0xb0, 0x0, 0x40, 0x0, 0x0,

    /* U+0026 "&" */
    0x5, 0xff, 0x70, 0x4, 0xb3, 0x31, 0x0, 0x5b,
    0x0, 0x96, 0x0, 0x9f, 0xfa, 0x92, 0x5a, 0x33,
    0x87, 0x6, 0xc0, 0xa, 0x70, 0x4b, 0x33, 0x95,
    0x0, 0x5f, 0xf7, 0x0,

    /* U+0027 "'" */
    0x7a, 0x69, 0x0,

    /* U+0028 "(" */
    0x7, 0xe2, 0x69, 0x10, 0x7a, 0x0, 0x43, 0x0,
    0x43, 0x0, 0x7a, 0x0, 0x68, 0x10, 0x7, 0xe2,

    /* U+0029 ")" */
    0x4f, 0x40, 0x2, 0xc3, 0x0, 0xe4, 0x0, 0x62,
    0x0, 0x61, 0x0, 0xe4, 0x2, 0xc3, 0x4f, 0x40,

    /* U+002A "*" */
    0x0, 0x0, 0x0, 0x0, 0x96, 0x0, 0x5d, 0xa9,
    0xe3, 0x6, 0x55, 0x50, 0x3e, 0xa6, 0xd1, 0x25,
    0xa7, 0x51, 0x0, 0x43, 0x0,

    /* U+002B "+" */
    0x0, 0x62, 0x0, 0x0, 0xb5, 0x0, 0x3f, 0x8b,
    0xd0, 0x2, 0xb6, 0x10, 0x0, 0x93, 0x0, 0x0,
    0x0, 0x0,

    /* U+002C "," */
    0x7a, 0x68, 0x0,

    /* U+002D "-" */
    0x3f, 0xff, 0xd0, 0x2, 0x33, 0x10,

    /* U+002E "." */
    0x11, 0x7a,

    /* U+002F "/" */
    0x0, 0x0, 0x12, 0x0, 0x3, 0xf2, 0x0, 0xc,
    0x90, 0x0, 0x4e, 0x10, 0x0, 0x14, 0x0, 0x5,
    0xa0, 0x0, 0xe, 0x50, 0x0, 0x8c, 0x0, 0x0,
    0x93, 0x0, 0x0, 0x10, 0x0, 0x0,

    /* U+0030 "0" */
    0x3b, 0xff, 0xc4, 0x6b, 0x33, 0xa7, 0x6c, 0x0,
    0xa7, 0x35, 0x0, 0x44, 0x35, 0x0, 0x44, 0x6c,
    0x0, 0xa7, 0x6b, 0x33, 0xa7, 0x3b, 0xff, 0xc4,

    /* U+0031 "1" */
    0x12, 0xc4, 0xd4, 0x62, 0x62, 0xd4, 0xc4, 0x12,

    /* U+0032 "2" */
    0xa, 0xff, 0xc4, 0x0, 0x33, 0xa7, 0x0, 0x0,
    0x97, 0x7, 0xff, 0xa1, 0x5a, 0x33, 0x10, 0x6c,
    0x0, 0x0, 0x6b, 0x33, 0x0, 0x3b, 0xff, 0xb0,

    /* U+0033 "3" */
    0xa, 0xff, 0xc4, 0x0, 0x33, 0xa7, 0x0, 0x0,
    0x97, 0x7, 0xff, 0xa1, 0x0, 0x33, 0x86, 0x0,
    0x0, 0xa7, 0x0, 0x33, 0x97, 0xa, 0xff, 0xc4,

    /* U+0034 "4" */
    0x0, 0x0, 0x0, 0x54, 0x0, 0x36, 0x6c, 0x0,
    0xa7, 0x5b, 0x0, 0x97, 0x8, 0xff, 0xa1, 0x0,
    0x33, 0x96, 0x0, 0x0, 0xa7, 0x0, 0x0, 0x57,
    0x0, 0x0, 0x0,

    /* U+0035 "5" */
    0x3b, 0xff, 0xc0, 0x6b, 0x33, 0x0, 0x5b, 0x0,
    0x0, 0x9, 0xff, 0x90, 0x0, 0x33, 0x86, 0x0,
    0x0, 0xa7, 0x0, 0x33, 0x97, 0xa, 0xff, 0xc4,

    /* U+0036 "6" */
    0x3b, 0xff, 0xc0, 0x6b, 0x33, 0x0, 0x5b, 0x0,
    0x0, 0x9, 0xff, 0x90, 0x5a, 0x33, 0x86, 0x6c,
    0x0, 0xa7, 0x6b, 0x33, 0x97, 0x3b, 0xff, 0xc4,

    /* U+0037 "7" */
    0xa, 0xff, 0xc4, 0x0, 0x33, 0xa7, 0x0, 0x0,
    0xa7, 0x0, 0x0, 0x44, 0x0, 0x0, 0x44, 0x0,
    0x0, 0xa7, 0x0, 0x0, 0x97, 0x0, 0x0, 0x3,

    /* U+0038 "8" */
    0x3b, 0xff, 0xc4, 0x6b, 0x33, 0xa7, 0x5b, 0x0,
    0x97, 0x9, 0xff, 0xa1, 0x5a, 0x33, 0x86, 0x6c,
    0x0, 0xa7, 0x6b, 0x33, 0x97, 0x3b, 0xff, 0xc4,

    /* U+0039 "9" */
    0x3b, 0xff, 0xc4, 0x6b, 0x33, 0xa7, 0x5b, 0x0,
    0x97, 0x9, 0xff, 0xa1, 0x0, 0x33, 0x86, 0x0,
    0x0, 0xa7, 0x0, 0x33, 0x97, 0xa, 0xff, 0xc4,

    /* U+003A ":" */
    0x3e, 0x2, 0x0, 0x0, 0x0, 0x2, 0x3e,

    /* U+003B ";" */
    0x3e, 0x2, 0x0, 0x0, 0x0, 0x14, 0x3e, 0x19,

    /* U+003C "<" */
    0x0, 0x14, 0x0, 0xca, 0xc, 0xb0, 0x3b, 0x30,
    0x8, 0xe2, 0x0, 0x8c,

    /* U+003D "=" */
    0x3f, 0xff, 0xd0, 0x3, 0x33, 0x20, 0x3f, 0xff,
    0xd0, 0x2, 0x33, 0x10,

    /* U+003E ">" */
    0x22, 0x0, 0x5e, 0x30, 0x8, 0xe2, 0x0, 0xa6,
    0xc, 0xb0, 0x7c, 0x0,

    /* U+003F "?" */
    0x1d, 0xff, 0x70, 0x1, 0x33, 0xa6, 0x0, 0x0,
    0x97, 0x7, 0xff, 0xa1, 0x5a, 0x33, 0x10, 0x6c,
    0x0, 0x0, 0x6b, 0x0, 0x0, 0x41, 0x0, 0x0,
    0x37, 0x0, 0x0, 0x48, 0x0, 0x0,

    /* U+0040 "@" */
    0x5, 0xff, 0xfe, 0x64, 0xb3, 0x33, 0x6b, 0x6c,
    0xb, 0xfd, 0x22, 0x48, 0x73, 0x5a, 0x24, 0x98,
    0x6, 0xb6, 0xc1, 0x9f, 0xd7, 0x4b, 0x33, 0x66,
    0x20, 0x5f, 0xff, 0xfb,

    /* U+0041 "A" */
    0x3b, 0xff, 0xc4, 0x6b, 0x33, 0xa7, 0x5b, 0x0,
    0x97, 0x9, 0xff, 0xa1, 0x5a, 0x33, 0x96, 0x6c,
    0x0, 0xa7, 0x69, 0x0, 0x87, 0x20, 0x0, 0x2,

    /* U+0042 "B" */
    0x3b, 0xff, 0x70, 0x6b, 0x33, 0xa6, 0x5b, 0x0,
    0x97, 0x9, 0xff, 0xa1, 0x5a, 0x33, 0x86, 0x6c,
    0x0, 0xa7, 0x6b, 0x33, 0x95, 0x3b, 0xff, 0x70,

    /* U+0043 "C" */
    0x5, 0xff, 0xe1, 0x4b, 0x33, 0x10, 0x6c, 0x0,
    0x0, 0x35, 0x0, 0x0, 0x35, 0x0, 0x0, 0x5c,
    0x0, 0x0, 0x4a, 0x33, 0x10, 0x4, 0xff, 0xd1,

    /* U+0044 "D" */
    0x3b, 0xff, 0x70, 0x6b, 0x33, 0xa6, 0x6c, 0x0,
    0xa7, 0x35, 0x0, 0x44, 0x35, 0x0, 0x44, 0x6c,
    0x0, 0xa7, 0x6b, 0x33, 0xa6, 0x3b, 0xff, 0x70,

    /* U+0045 "E" */
    0x3b, 0xff, 0xe1, 0x6b, 0x33, 0x10, 0x5b, 0x0,
    0x0, 0x9, 0xff, 0x90, 0x5a, 0x33, 0x10, 0x6c,
    0x0, 0x0, 0x6b, 0x33, 0x10, 0x3b, 0xff, 0xd1,

    /* U+0046 "F" */
    0x3b, 0xff, 0xe1, 0x6b, 0x33, 0x10, 0x5b, 0x0,
    0x0, 0x9, 0xff, 0x90, 0x5a, 0x33, 0x10, 0x6c,
    0x0, 0x0, 0x57, 0x0, 0x0, 0x0, 0x0, 0x0,

    /* U+0047 "G" */
    0x5, 0xff, 0xe1, 0x4b, 0x33, 0x10, 0x5b, 0x0,
    0x0, 0x1, 0x5f, 0x90, 0x59, 0x3, 0x96, 0x6c,
    0x0, 0xa7, 0x4b, 0x22, 0x96, 0x4, 0xff, 0x60,
    0x0, 0x33, 0x0,

    /* U+0048 "H" */
    0x10, 0x0, 0x1, 0x67, 0x0, 0x57, 0x6c, 0x0,
    0xa7, 0x5b, 0x0, 0x97, 0x9, 0xff, 0xa1, 0x5a,
    0x33, 0x96, 0x6c, 0x0, 0xa7, 0x69, 0x0, 0x87,
    0x20, 0x0, 0x2,

    /* U+0049 "I" */
    0x33, 0xd4, 0xd4, 0x62, 0x62, 0xd4, 0xd4, 0x33,

    /* U+004A "J" */
    0x0, 0x0, 0x15, 0x0, 0x0, 0xa7, 0x0, 0x0,
    0xa7, 0x0, 0x0, 0x44, 0x0, 0x0, 0x44, 0x5a,
    0x0, 0xa7, 0x4a, 0x33, 0xa6, 0x4, 0xff, 0x70,

    /* U+004B "K" */
    0x43, 0x5, 0xf0, 0x6c, 0x7e, 0x30, 0x5b, 0xa3,
    0x0, 0x9, 0xff, 0x90, 0x5a, 0x33, 0x96, 0x6c,
    0x0, 0xa7, 0x69, 0x0, 0x87, 0x20, 0x0, 0x2,

    /* U+004C "L" */
    0x42, 0x0, 0x0, 0x6b, 0x0, 0x0, 0x6c, 0x0,
    0x0, 0x35, 0x0, 0x0, 0x35, 0x0, 0x0, 0x6c,
    0x0, 0x0, 0x6b, 0x33, 0x10, 0x3b, 0xff, 0xd1,

    /* U+004D "M" */
    0x3b, 0xff, 0xc4, 0x6b, 0x46, 0xa7, 0x6c, 0x7a,
    0xa7, 0x35, 0x7a, 0x44, 0x35, 0x58, 0x44, 0x6c,
    0x0, 0xa7, 0x6b, 0x0, 0xa7, 0x42, 0x0, 0x25,

    /* U+004E "N" */
    0x4c, 0xe2, 0x15, 0x6c, 0xad, 0xa7, 0x6c, 0x7,
    0xb7, 0x35, 0x0, 0x44, 0x35, 0x0, 0x44, 0x6c,
    0x0, 0xa7, 0x6b, 0x0, 0xa7, 0x42, 0x0, 0x25,

    /* U+004F "O" */
    0x5, 0xff, 0x70, 0x4b, 0x33, 0xa6, 0x6c, 0x0,
    0xa7, 0x35, 0x0, 0x44, 0x35, 0x0, 0x44, 0x5c,
    0x0, 0xa7, 0x4a, 0x33, 0xa6, 0x4, 0xff, 0x70,

    /* U+0050 "P" */
    0x3b, 0xff, 0x70, 0x6b, 0x33, 0xa6, 0x5b, 0x0,
    0x97, 0x9, 0xff, 0xa1, 0x5a, 0x33, 0x10, 0x6c,
    0x0, 0x0, 0x69, 0x0, 0x0, 0x20, 0x0, 0x0,

    /* U+0051 "Q" */
    0x5, 0xff, 0x70, 0x4b, 0x33, 0xa6, 0x6c, 0x0,
    0xa7, 0x23, 0x0, 0x23, 0x47, 0x0, 0x55, 0x5c,
    0x2, 0xa5, 0x4a, 0x3a, 0xc0, 0x4, 0xf7, 0xc7,

    /* U+0052 "R" */
    0x3b, 0xff, 0x70, 0x6b, 0x33, 0xa6, 0x5b, 0x0,
    0x97, 0x9, 0xff, 0xa1, 0x5a, 0x85, 0x10, 0x6c,
    0x9d, 0x10, 0x6b, 0xc, 0xc0, 0x42, 0x1, 0xd4,

    /* U+0053 "S" */
    0x5, 0xff, 0xf3, 0x4b, 0x33, 0x30, 0x5b, 0x0,
    0x0, 0x9, 0xff, 0x90, 0x0, 0x33, 0x86, 0x0,
    0x0, 0xa7, 0x2, 0x33, 0x95, 0x2f, 0xff, 0x70,

    /* U+0054 "T" */
    0x6f, 0xff, 0xf7, 0x3, 0x45, 0x30, 0x0, 0x89,
    0x0, 0x0, 0x25, 0x0, 0x0, 0x25, 0x0, 0x0,
    0x8a, 0x0, 0x0, 0x8a, 0x0, 0x0, 0x57, 0x0,
    0x0, 0x0, 0x0,

    /* U+0055 "U" */
    0x42, 0x0, 0x15, 0x6b, 0x0, 0xa7, 0x6c, 0x0,
    0xa7, 0x35, 0x0, 0x44, 0x35, 0x0, 0x44, 0x5c,
    0x0, 0xa7, 0x4a, 0x33, 0xa6, 0x4, 0xff, 0x70,

    /* U+0056 "V" */
    0x10, 0x0, 0x1, 0x67, 0x0, 0x67, 0x6c, 0x0,
    0xa7, 0x5a, 0x0, 0x87, 0x1, 0x0, 0x30, 0x5a,
    0x6, 0xf0, 0x6c, 0x4f, 0x40, 0x6c, 0xd6, 0x0,
    0x14, 0x0, 0x0,

    /* U+0057 "W" */
    0x42, 0x0, 0x15, 0x6b, 0x0, 0xa7, 0x6c, 0x0,
    0xa7, 0x35, 0x58, 0x44, 0x35, 0x7a, 0x44, 0x6c,
    0x7a, 0xa7, 0x6b, 0x45, 0xa7, 0x3b, 0xff, 0xc4,

    /* U+0058 "X" */
    0x69, 0x0, 0x87, 0x2f, 0x32, 0xf4, 0x8, 0xca,
    0xa0, 0x0, 0x98, 0x10, 0x0, 0x98, 0x10, 0x8,
    0xca, 0xa0, 0x2f, 0x42, 0xf3, 0x69, 0x0, 0x87,

    /* U+0059 "Y" */
    0x10, 0x0, 0x1, 0x57, 0x0, 0x57, 0x5c, 0x0,
    0xa7, 0x5b, 0x0, 0x97, 0x6, 0xff, 0xa1, 0x0,
    0x33, 0x86, 0x0, 0x0, 0xa7, 0x1, 0x33, 0x95,
    0xb, 0xff, 0x70,

    /* U+005A "Z" */
    0x1d, 0xff, 0xf3, 0x1, 0x33, 0xa0, 0x0, 0x9,
    0xc0, 0x0, 0xd, 0x30, 0x1, 0xc0, 0x0, 0xa,
    0xb0, 0x0, 0x9, 0x43, 0x0, 0x2f, 0xff, 0xc0,

    /* U+005B "[" */
    0x4c, 0xe2, 0x79, 0x10, 0x7a, 0x0, 0x43, 0x0,
    0x43, 0x0, 0x7a, 0x0, 0x79, 0x10, 0x4c, 0xe2,

    /* U+005C "\\" */
    0x31, 0x0, 0x0, 0x3f, 0x10, 0x0, 0xa, 0xa0,
    0x0, 0x2, 0xf2, 0x0, 0x0, 0x50, 0x0, 0x0,
    0xc, 0x40, 0x0, 0x7, 0xd0, 0x0, 0x0, 0xe6,
    0x0, 0x0, 0x57, 0x0, 0x0, 0x1,

    /* U+005D "]" */
    0x4f, 0xb2, 0x2, 0xc4, 0x0, 0xe4, 0x0, 0x62,
    0x0, 0x61, 0x0, 0xe4, 0x2, 0xc4, 0x4f, 0xa2,

    /* U+005E "^" */
    0x0, 0x0, 0x0, 0x1, 0x87, 0x0, 0xc, 0x9f,
    0x60, 0x7c, 0x4, 0xf0,

    /* U+005F "_" */
    0xcf, 0xff, 0xf6, 0x13, 0x33, 0x30,

    /* U+0060 "`" */
    0x1, 0x4, 0xe0, 0xd, 0x40, 0x0,

    /* U+0061 "a" */
    0x3b, 0xff, 0xc4, 0x6b, 0x33, 0xa7, 0x5b, 0x0,
    0x97, 0x9, 0xff, 0xa1, 0x5a, 0x33, 0x96, 0x6c,
    0x0, 0xa7, 0x69, 0x0, 0x87, 0x20, 0x0, 0x2,

    /* U+0062 "b" */
    0x3b, 0xff, 0x70, 0x6b, 0x33, 0xa6, 0x5b, 0x0,
    0x97, 0x9, 0xff, 0xa1, 0x5a, 0x33, 0x86, 0x6c,
    0x0, 0xa7, 0x6b, 0x33, 0x95, 0x3b, 0xff, 0x70,

    /* U+0063 "c" */
    0x5, 0xff, 0xe1, 0x4b, 0x33, 0x10, 0x6c, 0x0,
    0x0, 0x35, 0x0, 0x0, 0x35, 0x0, 0x0, 0x5c,
    0x0, 0x0, 0x4a, 0x33, 0x10, 0x4, 0xff, 0xd1,

    /* U+0064 "d" */
    0x3b, 0xff, 0x70, 0x6b, 0x33, 0xa6, 0x6c, 0x0,
    0xa7, 0x35, 0x0, 0x44, 0x35, 0x0, 0x44, 0x6c,
    0x0, 0xa7, 0x6b, 0x33, 0xa6, 0x3b, 0xff, 0x70,

    /* U+0065 "e" */
    0x3b, 0xff, 0xe1, 0x6b, 0x33, 0x10, 0x5b, 0x0,
    0x0, 0x9, 0xff, 0x90, 0x5a, 0x33, 0x10, 0x6c,
    0x0, 0x0, 0x6b, 0x33, 0x10, 0x3b, 0xff, 0xd1,

    /* U+0066 "f" */
    0x3b, 0xff, 0xe1, 0x6b, 0x33, 0x10, 0x5b, 0x0,
    0x0, 0x9, 0xff, 0x90, 0x5a, 0x33, 0x10, 0x6c,
    0x0, 0x0, 0x57, 0x0, 0x0, 0x0, 0x0, 0x0,

    /* U+0067 "g" */
    0x5, 0xff, 0xe1, 0x4b, 0x33, 0x10, 0x5b, 0x0,
    0x0, 0x1, 0x5f, 0x90, 0x59, 0x3, 0x96, 0x6c,
    0x0, 0xa7, 0x4b, 0x22, 0x96, 0x4, 0xff, 0x60,
    0x0, 0x33, 0x0,

    /* U+0068 "h" */
    0x10, 0x0, 0x1, 0x67, 0x0, 0x57, 0x6c, 0x0,
    0xa7, 0x5b, 0x0, 0x97, 0x9, 0xff, 0xa1, 0x5a,
    0x33, 0x96, 0x6c, 0x0, 0xa7, 0x69, 0x0, 0x87,
    0x20, 0x0, 0x2,

    /* U+0069 "i" */
    0x33, 0xd4, 0xd4, 0x62, 0x62, 0xd4, 0xd4, 0x33,

    /* U+006A "j" */
    0x0, 0x0, 0x15, 0x0, 0x0, 0xa7, 0x0, 0x0,
    0xa7, 0x0, 0x0, 0x44, 0x0, 0x0, 0x44, 0x5a,
    0x0, 0xa7, 0x4a, 0x33, 0xa6, 0x4, 0xff, 0x70,

    /* U+006B "k" */
    0x43, 0x5, 0xf0, 0x6c, 0x7e, 0x30, 0x5b, 0xa3,
    0x0, 0x9, 0xff, 0x90, 0x5a, 0x33, 0x96, 0x6c,
    0x0, 0xa7, 0x69, 0x0, 0x87, 0x20, 0x0, 0x2,

    /* U+006C "l" */
    0x42, 0x0, 0x0, 0x6b, 0x0, 0x0, 0x6c, 0x0,
    0x0, 0x35, 0x0, 0x0, 0x35, 0x0, 0x0, 0x6c,
    0x0, 0x0, 0x6b, 0x33, 0x10, 0x3b, 0xff, 0xd1,

    /* U+006D "m" */
    0x3b, 0xff, 0xc4, 0x6b, 0x46, 0xa7, 0x6c, 0x7a,
    0xa7, 0x35, 0x7a, 0x44, 0x35, 0x58, 0x44, 0x6c,
    0x0, 0xa7, 0x6b, 0x0, 0xa7, 0x42, 0x0, 0x25,

    /* U+006E "n" */
    0x4c, 0xe2, 0x15, 0x6c, 0xad, 0xa7, 0x6c, 0x7,
    0xb7, 0x35, 0x0, 0x44, 0x35, 0x0, 0x44, 0x6c,
    0x0, 0xa7, 0x6b, 0x0, 0xa7, 0x42, 0x0, 0x25,

    /* U+006F "o" */
    0x5, 0xff, 0x70, 0x4b, 0x33, 0xa6, 0x6c, 0x0,
    0xa7, 0x35, 0x0, 0x44, 0x35, 0x0, 0x44, 0x5c,
    0x0, 0xa7, 0x4a, 0x33, 0xa6, 0x4, 0xff, 0x70,

    /* U+0070 "p" */
    0x3b, 0xff, 0x70, 0x6b, 0x33, 0xa6, 0x5b, 0x0,
    0x97, 0x9, 0xff, 0xa1, 0x5a, 0x33, 0x10, 0x6c,
    0x0, 0x0, 0x69, 0x0, 0x0, 0x20, 0x0, 0x0,

    /* U+0071 "q" */
    0x5, 0xff, 0x70, 0x4b, 0x33, 0xa6, 0x6c, 0x0,
    0xa7, 0x23, 0x0, 0x23, 0x47, 0x0, 0x55, 0x5c,
    0x2, 0xa5, 0x4a, 0x3a, 0xc0, 0x4, 0xf7, 0xc7,

    /* U+0072 "r" */
    0x3b, 0xff, 0x70, 0x6b, 0x33, 0xa6, 0x5b, 0x0,
    0x97, 0x9, 0xff, 0xa1, 0x5a, 0x85, 0x10, 0x6c,
    0x9d, 0x10, 0x6b, 0xc, 0xc0, 0x42, 0x1, 0xd4,

    /* U+0073 "s" */
    0x5, 0xff, 0xf3, 0x4b, 0x33, 0x30, 0x5b, 0x0,
    0x0, 0x9, 0xff, 0x90, 0x0, 0x33, 0x86, 0x0,
    0x0, 0xa7, 0x2, 0x33, 0x95, 0x2f, 0xff, 0x70,

    /* U+0074 "t" */
    0x6f, 0xff, 0xf7, 0x3, 0x45, 0x30, 0x0, 0x89,
    0x0, 0x0, 0x25, 0x0, 0x0, 0x25, 0x0, 0x0,
    0x8a, 0x0, 0x0, 0x8a, 0x0, 0x0, 0x57, 0x0,
    0x0, 0x0, 0x0,

    /* U+0075 "u" */
    0x42, 0x0, 0x15, 0x6b, 0x0, 0xa7, 0x6c, 0x0,
    0xa7, 0x35, 0x0, 0x44, 0x35, 0x0, 0x44, 0x5c,
    0x0, 0xa7, 0x4a, 0x33, 0xa6, 0x4, 0xff, 0x70,

    /* U+0076 "v" */
    0x10, 0x0, 0x1, 0x67, 0x0, 0x67, 0x6c, 0x0,
    0xa7, 0x5a, 0x0, 0x87, 0x1, 0x0, 0x30, 0x5a,
    0x6, 0xf0, 0x6c, 0x4f, 0x40, 0x6c, 0xd6, 0x0,
    0x14, 0x0, 0x0,

    /* U+0077 "w" */
    0x42, 0x0, 0x15, 0x6b, 0x0, 0xa7, 0x6c, 0x0,
    0xa7, 0x35, 0x58, 0x44, 0x35, 0x7a, 0x44, 0x6c,
    0x7a, 0xa7, 0x6b, 0x45, 0xa7, 0x3b, 0xff, 0xc4,

    /* U+0078 "x" */
    0x69, 0x0, 0x87, 0x2f, 0x32, 0xf4, 0x8, 0xca,
    0xa0, 0x0, 0x98, 0x10, 0x0, 0x98, 0x10, 0x8,
    0xca, 0xa0, 0x2f, 0x42, 0xf3, 0x69, 0x0, 0x87,

    /* U+0079 "y" */
    0x10, 0x0, 0x1, 0x57, 0x0, 0x57, 0x5c, 0x0,
    0xa7, 0x5b, 0x0, 0x97, 0x6, 0xff, 0xa1, 0x0,
    0x33, 0x86, 0x0, 0x0, 0xa7, 0x1, 0x33, 0x95,
    0xb, 0xff, 0x70,

    /* U+007A "z" */
    0x1d, 0xff, 0xf3, 0x1, 0x33, 0xa0, 0x0, 0x9,
    0xc0, 0x0, 0xd, 0x30, 0x1, 0xc0, 0x0, 0xa,
    0xb0, 0x0, 0x9, 0x43, 0x0, 0x2f, 0xff, 0xc0,

    /* U+007B "{" */
    0x0, 0x8d, 0x10, 0x79, 0x10, 0x7, 0x90, 0x7,
    0xb2, 0x0, 0x7, 0x80, 0x0, 0x89, 0x0, 0x7,
    0x81, 0x0, 0x8, 0xd1,

    /* U+007C "|" */
    0xa7, 0xa7, 0xa7, 0xa7, 0xa7, 0xa7, 0xa7, 0xa7,
    0xa7, 0xa7, 0x11,

    /* U+007D "}" */
    0x4f, 0x40, 0x0, 0x2d, 0x30, 0x0, 0xe3, 0x0,
    0x3, 0xe3, 0x0, 0xc4, 0x0, 0xe, 0x40, 0x2,
    0xc2, 0x4, 0xf4, 0x0,

    /* U+007E "~" */
    0x0, 0x0, 0x3, 0xe7, 0x16, 0x51, 0x9e, 0x30,
    0x0, 0x0
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 45, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 0, .adv_w = 55, .box_w = 2, .box_h = 11, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 11, .adv_w = 59, .box_w = 4, .box_h = 3, .ofs_x = 0, .ofs_y = 5},
    {.bitmap_index = 17, .adv_w = 116, .box_w = 7, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 42, .adv_w = 98, .box_w = 6, .box_h = 14, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 84, .adv_w = 134, .box_w = 8, .box_h = 10, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 124, .adv_w = 104, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 152, .adv_w = 35, .box_w = 2, .box_h = 3, .ofs_x = 0, .ofs_y = 5},
    {.bitmap_index = 155, .adv_w = 60, .box_w = 4, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 171, .adv_w = 60, .box_w = 4, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 187, .adv_w = 93, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 2},
    {.bitmap_index = 208, .adv_w = 90, .box_w = 6, .box_h = 6, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 226, .adv_w = 35, .box_w = 2, .box_h = 3, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 229, .adv_w = 90, .box_w = 6, .box_h = 2, .ofs_x = 0, .ofs_y = 3},
    {.bitmap_index = 235, .adv_w = 35, .box_w = 2, .box_h = 2, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 237, .adv_w = 94, .box_w = 6, .box_h = 10, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 267, .adv_w = 98, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 291, .adv_w = 55, .box_w = 2, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 299, .adv_w = 98, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 323, .adv_w = 98, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 347, .adv_w = 98, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 374, .adv_w = 98, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 398, .adv_w = 98, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 422, .adv_w = 98, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 446, .adv_w = 98, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 470, .adv_w = 98, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 494, .adv_w = 43, .box_w = 2, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 501, .adv_w = 43, .box_w = 2, .box_h = 8, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 509, .adv_w = 68, .box_w = 4, .box_h = 6, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 521, .adv_w = 90, .box_w = 6, .box_h = 4, .ofs_x = 0, .ofs_y = 2},
    {.bitmap_index = 533, .adv_w = 68, .box_w = 4, .box_h = 6, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 545, .adv_w = 98, .box_w = 6, .box_h = 10, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 575, .adv_w = 118, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 603, .adv_w = 98, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 627, .adv_w = 98, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 651, .adv_w = 98, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 675, .adv_w = 98, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 699, .adv_w = 98, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 723, .adv_w = 98, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 747, .adv_w = 98, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 774, .adv_w = 98, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 801, .adv_w = 55, .box_w = 2, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 809, .adv_w = 98, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 833, .adv_w = 98, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 857, .adv_w = 98, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 881, .adv_w = 98, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 905, .adv_w = 98, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 929, .adv_w = 98, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 953, .adv_w = 98, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 977, .adv_w = 98, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1001, .adv_w = 98, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1025, .adv_w = 98, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1049, .adv_w = 98, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 1076, .adv_w = 98, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1100, .adv_w = 94, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1127, .adv_w = 98, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1151, .adv_w = 94, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1175, .adv_w = 98, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1202, .adv_w = 98, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1226, .adv_w = 60, .box_w = 4, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1242, .adv_w = 94, .box_w = 6, .box_h = 10, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 1272, .adv_w = 60, .box_w = 4, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1288, .adv_w = 89, .box_w = 6, .box_h = 4, .ofs_x = 0, .ofs_y = 5},
    {.bitmap_index = 1300, .adv_w = 90, .box_w = 6, .box_h = 2, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 1306, .adv_w = 46, .box_w = 3, .box_h = 4, .ofs_x = 0, .ofs_y = 5},
    {.bitmap_index = 1312, .adv_w = 98, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1336, .adv_w = 98, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1360, .adv_w = 98, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1384, .adv_w = 98, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1408, .adv_w = 98, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1432, .adv_w = 98, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1456, .adv_w = 98, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 1483, .adv_w = 98, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1510, .adv_w = 55, .box_w = 2, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1518, .adv_w = 98, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1542, .adv_w = 98, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1566, .adv_w = 98, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1590, .adv_w = 98, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1614, .adv_w = 98, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1638, .adv_w = 98, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1662, .adv_w = 98, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1686, .adv_w = 98, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1710, .adv_w = 98, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1734, .adv_w = 98, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1758, .adv_w = 98, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 1785, .adv_w = 98, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1809, .adv_w = 94, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1836, .adv_w = 98, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1860, .adv_w = 94, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1884, .adv_w = 98, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1911, .adv_w = 98, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1935, .adv_w = 75, .box_w = 5, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1955, .adv_w = 61, .box_w = 2, .box_h = 11, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 1966, .adv_w = 75, .box_w = 5, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1986, .adv_w = 81, .box_w = 5, .box_h = 4, .ofs_x = 0, .ofs_y = 2}
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
    .bpp = 4,
    .kern_classes = 0,
    .bitmap_format = 0,
#if LVGL_VERSION_MAJOR == 8
    .cache = &cache
#endif
};

extern const lv_font_t ds_digib_font_12;


/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
#if LVGL_VERSION_MAJOR >= 8
const lv_font_t ds_digib_font_12 = {
#else
lv_font_t ds_digib_font_12 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 14,          /*The maximum line height required by the font*/
    .base_line = 3,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = -1,
    .underline_thickness = 1,
#endif
    .dsc = &font_dsc,          /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
#if LV_VERSION_CHECK(8, 2, 0) || LVGL_VERSION_MAJOR >= 9
    .fallback = &ds_digib_font_12,
#endif
    .user_data = NULL,
};



#endif /*#if DS_DIGIB_FONT_12*/

