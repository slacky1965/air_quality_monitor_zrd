#include "app_main.h"

#include "epaper.h"
#include "bmp.h"
#include "app_epd.h"

static uint8_t image_bw[EPD_W_BUFF_SIZE * EPD_H];

static epd_rotate_t rotate = APP_EPD_ROTATE_0;
static uint16_t real_rotate = EPD_ROTATE_270;

static void epd_logo() {

    uint16_t x, y, xrs, yrs, xre, yre;
    uint8_t fwv[]  = ZCL_BASIC_SW_BUILD_ID;
    uint8_t vers[20] = "Firmware ";
    uint8_t name[] = "Air Quality Monitor";
    uint8_t manufacturer[] = "Slacky-DIY & DIY&Zi";

    strcat((char*)vers, (char*)(fwv+1));

    epd_init_partial();

    epd_paint_selectimage(image_bw);


    if (rotate == APP_EPD_ROTATE_0) {
        x = 88;
        y = 100;
        xrs = 1;
        yrs = 1;
        xre = 400;
        yre = 300;
    } else {
        x = 36;
        y = 142;
        xrs = 1;
        yrs = 1;
        xre = 300;
        yre = 400;
    }

    epd_paint_showString(x, y, name, EPD_FONT_SIZE24x12, EPD_COLOR_BLACK);

    epd_paint_showString(x+18, y+30, vers, EPD_FONT_SIZE24x12, EPD_COLOR_BLACK);

    epd_paint_showString(x, y+60, manufacturer, EPD_FONT_SIZE24x12, EPD_COLOR_BLACK);

    epd_paint_drawRectangle(xrs, yrs, xre, yre, EPD_COLOR_BLACK, 0);

    epd_displayBW_partial(image_bw);
    epd_enter_deepsleepmode(EPD_DEEPSLEEP_MODE1);

}

static void epd_clear() {

    epd_paint_selectimage(image_bw);
    epd_paint_clear(EPD_COLOR_WHITE);
    epd_displayBW(image_bw);
//    epd_enter_deepsleepmode(EPD_DEEPSLEEP_MODE1);


}

 static void epd_newimage() {

     if (rotate == APP_EPD_ROTATE_0) {
         real_rotate = EPD_ROTATE_270;
     } else {
         real_rotate = EPD_ROTATE_0;
     }

     epd_paint_newimage(image_bw, EPD_W, EPD_H, real_rotate, EPD_COLOR_WHITE);

 }

void app_epd_init() {

    uint8_t ret = 0;

    epd_io_init();

    ret = epd_init();

    printf("return epd_init: %d\r\n", ret);

    epd_newimage();

    epd_clear();

    epd_logo();

    printf("sleep logo1\r\n");
    sleep_ms(5000);
    printf("sleep5\r\n");
    epd_clear();
    printf("sleep epd_clear\r\n");
//    rotate = APP_EPD_ROTATE_90;
//    epd_reset();
//    printf("sleep epd_reset\r\n");
//    epd_newimage();
//    epd_clear();
//    printf("sleep epd_clear\r\n");
//    epd_logo();
//    printf("sleep logo2\r\n");
}
