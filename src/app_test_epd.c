#include "app_main.h"

#include "epaper.h"
#include "bmp.h"

uint8_t image_bw[EPD_W_BUFF_SIZE * EPD_H];
#ifdef EPD_BWR
uint8_t image_red[EPD_W_BUFF_SIZE * EPD_H];
#endif

const unsigned char zb[] = {
0x00, 0xC0, 0xE0, 0xE8, 0xEC, 0xE4, 0xE6, 0xE6, 0xE7, 0xE7, 0xE7, 0xE7, 0xE7, 0xE7, 0x67, 0x27,
0x06, 0x06, 0x8C, 0x8C, 0xF8, 0xF0, 0xC0, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x3F,
0x1F, 0x0F, 0x07, 0x83, 0xC1, 0xE0, 0xF0, 0xF8, 0xFC, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0x00, 0x03, 0x0F, 0x1F, 0x31, 0x70, 0x60, 0x60, 0xE4, 0xE6, 0xE7, 0xE7, 0xE7, 0xE7, 0xE7, 0xE7,
0xE7, 0x67, 0x67, 0x37, 0x17, 0x07, 0x03, 0x00,
};

static void epd_logo() {

    uint16_t x = 0, y = 0;
    uint8_t fwv[]  = ZCL_BASIC_SW_BUILD_ID;
    uint8_t vers[20] = "Firmware ";
    uint8_t name[] = "Air Quality Monitor";
    uint8_t A[] = "A";

    strcat((char*)vers, (char*)(fwv+1));

    printf("name: %s\r\n", name);

    x = 10;
    y = 118;

//    epd_init_partial();

    epd_paint_selectimage(image_bw);


//    if (rotate == APP_EPD_ROTATE_0) {
//        x = 10;
//        y = 118;
//    } else {
//        real_rotate = EPD_ROTATE_0;
//    }

    epd_paint_showString(x, y, name, EPD_FONT_SIZE24x12, EPD_COLOR_BLACK);
//    epd_displayBW_partial(image_bw);

    epd_paint_showString(x, y+30, vers, EPD_FONT_SIZE24x12, EPD_COLOR_BLACK);

    epd_paint_showString(350, y+60, A, EPD_FONT_SIZE24x12, EPD_COLOR_BLACK);

    epd_displayBW_partial(image_bw);
    epd_enter_deepsleepmode(EPD_DEEPSLEEP_MODE1);

}

void test_epd() {

    uint8_t text[20];
    uint8_t str[10];

    printf("\r\nWeAct Studio Core Board\r\n");
    printf("Epaper Module Test\r\n");

    epd_io_init();

    epd_init();

#ifdef EPD_BWR
    #ifdef EPD_42
  epd_paint_newimage(image_bw, EPD_W, EPD_H, EPD_ROTATE_270, EPD_COLOR_WHITE);
  epd_paint_newimage(image_red, EPD_W, EPD_H, EPD_ROTATE_270, EPD_COLOR_WHITE);
    #else
    epd_paint_newimage(image_bw, EPD_W, EPD_H, EPD_ROTATE_180, EPD_COLOR_WHITE);
    epd_paint_newimage(image_red, EPD_W, EPD_H, EPD_ROTATE_180, EPD_COLOR_WHITE);
    #endif

    epd_paint_selectimage(image_bw);
    epd_paint_clear(EPD_COLOR_WHITE);

    epd_paint_selectimage(image_red);
    epd_paint_clear(EPD_COLOR_WHITE);

    #ifdef EPD_42
        epd_paint_showPicture((EPD_W - 250) / 2, (EPD_H - 122) / 2, 250, 122, gImage_4, EPD_COLOR_WHITE);
        #else
    epd_paint_showPicture((EPD_H - 250) / 2, (EPD_W - 122) / 2, 250, 122, gImage_4, EPD_COLOR_WHITE);
    #endif

    epd_display(image_bw, image_red);
    epd_enter_deepsleepmode(EPD_DEEPSLEEP_MODE1);

    sleep_ms(8000);

    epd_init();

    epd_paint_selectimage(image_bw);
    epd_paint_clear(EPD_COLOR_WHITE);
#ifdef EPD_213
    epd_paint_showString(10, 0, (uint8_t *)&"2.13 Epaper Module", EPD_FONT_SIZE24x12, EPD_COLOR_BLACK);
    epd_paint_showString(10, 50, (uint8_t *)&"with 250 x 122 resolution", EPD_FONT_SIZE16x8, EPD_COLOR_BLACK);
#endif
#ifdef EPD_29
    epd_paint_showString(10, 0, (uint8_t *)&"2.9 Epaper Module", EPD_FONT_SIZE24x12, EPD_COLOR_BLACK);
    epd_paint_showString(10, 50, (uint8_t *)&"with 296 x 128 resolution", EPD_FONT_SIZE16x8, EPD_COLOR_BLACK);
#endif
#ifdef EPD_42
    epd_paint_showString(10, 0, (uint8_t *)&"4.2 Inch Epaper Module", EPD_FONT_SIZE24x12, EPD_COLOR_BLACK);
    epd_paint_showString(10, 50, (uint8_t *)&"with 400 x 300 resolution", EPD_FONT_SIZE16x8, EPD_COLOR_BLACK);
#endif
    epd_paint_showString(10, 29, (uint8_t *)&"Designed By WeAct Studio", EPD_FONT_SIZE16x8, EPD_COLOR_BLACK);

    epd_paint_selectimage(image_red);
    epd_paint_clear(EPD_COLOR_WHITE);

    sprintf((char *)&text, ">> Hello World.");
    epd_paint_showString(10, 71, text, EPD_FONT_SIZE24x12, EPD_COLOR_RED);

#if 1
    epd_paint_showString(10,100,(uint8_t *)&"CH32F103C8T6 Example",EPD_FONT_SIZE16x8,EPD_COLOR_RED);
#else
    epd_paint_drawRectangle(10, 103, EPD_H - 10, 116, EPD_COLOR_RED, 1);
#endif

    epd_display(image_bw, image_red);

#else
#ifdef EPD_42
    epd_paint_newimage(image_bw, EPD_W, EPD_H, EPD_ROTATE_270, EPD_COLOR_WHITE);
//    epd_paint_newimage(image_bw, EPD_W, EPD_H, EPD_ROTATE_0, EPD_COLOR_WHITE);
#else
    epd_paint_newimage(image_bw, EPD_W, EPD_H, EPD_ROTATE_180, EPD_COLOR_WHITE);
#endif

    epd_paint_selectimage(image_bw);

#ifdef EPD_154
    epd_paint_clear(EPD_COLOR_WHITE);
    epd_paint_showPicture((EPD_H - 200) / 2,(EPD_W - 64) / 2,200,64,gImage_5,EPD_COLOR_WHITE);
#else
    epd_paint_clear(EPD_COLOR_WHITE);
#ifdef EPD_42
    epd_paint_showPicture((EPD_W - 250) / 2, (EPD_H - 122) / 2, 250, 122, gImage_4, EPD_COLOR_WHITE);
#else
    epd_paint_showPicture((EPD_H - 250) / 2, (EPD_W - 122) / 2, 250, 122, gImage_4, EPD_COLOR_WHITE);
#endif
#endif

    epd_displayBW(image_bw);
    epd_enter_deepsleepmode(EPD_DEEPSLEEP_MODE1);

    sleep_ms(1000);

    epd_init_partial();

    epd_paint_selectimage(image_bw);
    epd_paint_clear(EPD_COLOR_WHITE);
#ifdef EPD_154
    epd_paint_showString(10, 0, (uint8_t *)&"1.54 Inch", EPD_FONT_SIZE24x12, EPD_COLOR_BLACK);
    epd_paint_showString(10, 23, (uint8_t *)&"Epaper Module", EPD_FONT_SIZE24x12, EPD_COLOR_BLACK);
    epd_paint_showString(10, 48, (uint8_t *)&"Designed By WeAct Studio", EPD_FONT_SIZE12x6, EPD_COLOR_BLACK);
    epd_paint_showString(10, 60, (uint8_t *)&"with 200 x 200 resolution", EPD_FONT_SIZE12x6, EPD_COLOR_BLACK);
    epd_paint_showPicture((EPD_H - 200) / 2,130,200,64,gImage_5,EPD_COLOR_WHITE);
#endif
#ifdef EPD_213
    epd_paint_showString(10, 0, (uint8_t *)&"2.13 Epaper Module", EPD_FONT_SIZE24x12, EPD_COLOR_BLACK);
    epd_paint_showString(10, 50, (uint8_t *)&"with 250 x 122 resolution", EPD_FONT_SIZE16x8, EPD_COLOR_BLACK);
    epd_paint_showString(10, 29, (uint8_t *)&"Designed By WeAct Studio", EPD_FONT_SIZE16x8, EPD_COLOR_BLACK);
#endif
#ifdef EPD_29
    epd_paint_showString(10, 0, (uint8_t *)&"2.9 Epaper Module", EPD_FONT_SIZE24x12, EPD_COLOR_BLACK);
    epd_paint_showString(10, 50, (uint8_t *)&"with 296 x 128 resolution", EPD_FONT_SIZE16x8, EPD_COLOR_BLACK);
    epd_paint_showString(10, 29, (uint8_t *)&"Designed By WeAct Studio", EPD_FONT_SIZE16x8, EPD_COLOR_BLACK);
#endif
#ifdef EPD_42
    epd_paint_showString(10, 0, (uint8_t *)&"4.2 Inch Epaper Module", EPD_FONT_SIZE24x12, EPD_COLOR_BLACK);
    epd_paint_showString(10, 50, (uint8_t *)&"with 400 x 300 resolution", EPD_FONT_SIZE16x8, EPD_COLOR_BLACK);
    epd_paint_showString(10, 29, (uint8_t *)&"Designed By WeAct Studio", EPD_FONT_SIZE16x8, EPD_COLOR_BLACK);
#endif
#if 1
    epd_paint_showString(10,100,(uint8_t *)&"TLSR8258 Example",EPD_FONT_SIZE16x8,EPD_COLOR_BLACK);
#else
    epd_paint_drawRectangle(10, EPD_W-20, EPD_H - 10, EPD_W-6, EPD_COLOR_BLACK, 1);
#endif

    strcpy((char*)text, ">> Partial Mode");
    epd_paint_showString(10, 71, text, EPD_FONT_SIZE24x12, EPD_COLOR_BLACK);

    epd_displayBW_partial(image_bw);

    sleep_ms(1000);

//    uint8_t *test = NULL;
//
//    for (uint32_t i = 123; i < 8 * 123; i += 123)
//    {
//
//        test = itoa(i, str);
//        strcpy((char*)text, ">> Num = ");
//        strcat((char*)text, (char*)test);
//        strcat((char*)text, "     ");
//        //sprintf((char *)&text, ">> Num=%d     ", i);
//        epd_paint_showString(10, 71, text, EPD_FONT_SIZE24x12, EPD_COLOR_BLACK);
//
//        epd_displayBW_partial(image_bw);
//
//        sleep_ms(100);
//    }

    strcpy((char*)text, ">> Hello World.");
    epd_paint_showString(10, 71, text, EPD_FONT_SIZE24x12, EPD_COLOR_BLACK);

    epd_displayBW_partial(image_bw);

    epd_logo();

    sleep_ms(1000);

////    epd_paint_showPicture(200, 71, 16, 16, IMAGE_ONNETWORK, EPD_COLOR_BLACK);
//    epd_paint_showPicture(200, 71, 24, 24, zb, EPD_COLOR_WHITE);
////    epd_paint_showPicture(200, 71, 24, 24, wifi_24dp, EPD_COLOR_BLACK);
////    epd_paint_showPicture(0, 0, 400, 300, test, EPD_COLOR_BLACK);
//    epd_displayBW_partial(image_bw);

//    sleep_ms(1000);

    //    epd_update();
#endif


    epd_enter_deepsleepmode(EPD_DEEPSLEEP_MODE1);

}
