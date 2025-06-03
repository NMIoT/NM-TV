// Setup for the TTGO T Display
#define USER_SETUP_ID 252

// See SetupX_Template.h for all options available
#define ST7789_DRIVER

#define TFT_WIDTH           240
#define TFT_HEIGHT          135

#define CGRAM_OFFSET

#define TFT_MOSI            11
#define TFT_SCLK            12
#define TFT_CS              10
#define TFT_DC              8
#define TFT_RST             9

#define TFT_RGB_ORDER       TFT_BGR // Colour order Blue-Green-Red
#define TFT_INVERSION_ON
#define TFT_BACKLIGHT_ON    HIGH  // HIGH or LOW are options

#define LOAD_GLCD
#define LOAD_FONT2
#define LOAD_FONT4
#define LOAD_FONT6
#define LOAD_FONT7
// #define LOAD_FONT8
#define LOAD_GFXFF

#define SMOOTH_FONT

#define SPI_FREQUENCY  27000000
// #define SPI_FREQUENCY  40000000


#define SPI_READ_FREQUENCY  6000000 // 6 MHz is the maximum SPI read speed for the ST7789V
