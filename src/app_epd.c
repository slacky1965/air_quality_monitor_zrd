#include "app_main.h"

#include "epaper.h"
#include "bmp.h"
#include "app_epd.h"
#include "app_epd_images.h"
#include "app_epd_fonts.h"

static uint8_t image_bw[EPD_W_BUFF_SIZE * EPD_H];

static uint16_t real_rotate = EPD_ROTATE_270;

/* data */
static uint16_t year = 0;
static uint8_t month = 0;
static uint8_t day = 0;
static uint8_t wday = 10;
/* time */
static uint8_t hour = 25;
static uint8_t minutes = 61;
//static uint8_t seconds = 61;

static epd_screen_variable_t epd_screen_variable = {
        .year = 0,
        .month = 0,
        .day = 0,
        .wday = 0,
        .hour = 25,
        .minutes = 61,
        .temp_in = 0,
        .temp_out = 0,
        .humidity_in = 0,
        .humidity_out = 0,
        .pressure = 0,
        .level = 5,
        .zbIcon = false,
        .timerZbIcon = NULL,
};

static void epd_screen_var(void *args) {

//    print_time();
//    printf("epd_screen_var. zb_isDeviceJoinedNwk: %d\r\n", zb_isDeviceJoinedNwk());

    ftime_t *ftime;
    uint8_t m;
    uint16_t y;

    uint16_t attr_len;
    uint32_t counter;

    uint8_t octet2[4];
    uint8_t octet4[5];
    uint8_t color;
    uint8_t refresh = 0;

    uint8_t wday_str[][4] = {"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};


    if (config.inversion == APP_EPD_INVERSION_OFF) {
        color = EPD_COLOR_BLACK;
    } else {
        color = EPD_COLOR_WHITE;
    }


    zcl_getAttrVal(APP_ENDPOINT1, ZCL_CLUSTER_GEN_TIME, ZCL_ATTRID_LOCAL_TIME, &attr_len, (uint8_t*)&counter);

    if (counter != 0xFFFFFFFF) {

        ftime = get_ftime(counter);

        if (day !=  ftime->day) {
            day =  ftime->day;
            octet2[0] = (day / 10) + 0x30;
            octet2[1] = (day % 10) + 0x30;
            octet2[2] = '-';
            octet2[3] = 0;
            if (config.rotate == APP_EPD_ROTATE_0) {
                epd_paint_showString(DATE_0_X, DATE_0_Y, octet2, EPD_FONT_SIZE24x12, color);
            } else {
                epd_paint_showString(DATE_90_X, DATE_90_Y, octet2, EPD_FONT_SIZE24x12, color);
            }
            refresh |= 0x01;
        }

        if (month !=  ftime->month) {
            month =  ftime->month;
            octet2[0] = (month / 10) + 0x30;
            octet2[1] = (month % 10) + 0x30;
            octet2[2] = '-';
            octet2[3] = 0;
            if (config.rotate == APP_EPD_ROTATE_0) {
                epd_paint_showString(DATE_0_X + 3*12, DATE_0_Y, octet2, EPD_FONT_SIZE24x12, color);
            } else {
                epd_paint_showString(DATE_90_X + 3*12, DATE_90_Y, octet2, EPD_FONT_SIZE24x12, color);
            }
            refresh |= 0x02;
        }

        if (year !=  ftime->year) {
            year =  ftime->year;
            octet4[0] = (year / 1000) + 0x30;
            y = year % 1000;
            octet4[1] = (y / 100) + 0x30;
            m = y % 100;
            octet4[2] = (m / 10) + 0x30;
            octet4[3] = (m % 10) + 0x30;
            octet4[4] = 0;
            if (config.rotate == APP_EPD_ROTATE_0) {
                epd_paint_showString(DATE_0_X + 6*12, DATE_0_Y, octet4, EPD_FONT_SIZE24x12, color);
            } else {
                epd_paint_showString(DATE_90_X + 6*12, DATE_90_Y, octet4, EPD_FONT_SIZE24x12, color);
            }
            refresh |= 0x04;
        }


        if (wday != ftime->wday) {
            wday = ftime->wday;
            if (config.rotate == APP_EPD_ROTATE_0) {
                epd_paint_showString(WDAY_0_X, WDAY_0_Y, wday_str[wday], EPD_FONT_SIZE24x12, color);
            } else {
                epd_paint_showString(WDAY_90_X, WDAY_90_Y, wday_str[wday], EPD_FONT_SIZE24x12, color);
            }
            refresh |= 0x08;
        }

        if (hour !=  ftime->hour) {
            hour =  ftime->hour;
            octet2[0] = (hour / 10) + 0x30;
            octet2[1] = (hour % 10) + 0x30;
            octet2[2] = ':';
            octet2[3] = 0;
            if (config.rotate == APP_EPD_ROTATE_0) {
                epd_paint_showString(TIME_0_X, TIME_0_Y, octet2, EPD_FONT_SIZE24x12, color);
            } else {
                epd_paint_showString(TIME_90_X, TIME_90_Y, octet2, EPD_FONT_SIZE24x12, color);
            }
            refresh |= 0x10;
        }

        if (minutes !=  ftime->minute) {
            minutes =  ftime->minute;
            octet2[0] = (minutes / 10) + 0x30;
            octet2[1] = (minutes % 10) + 0x30;
            octet2[2] = 0;
//            octet2[2] = ':';
//            octet2[3] = 0;
            if (config.rotate == APP_EPD_ROTATE_0) {
                epd_paint_showString(TIME_0_X + 3*12, TIME_0_Y, octet2, EPD_FONT_SIZE24x12, color);
            } else {
                epd_paint_showString(TIME_90_X + 3*12, TIME_90_Y, octet2, EPD_FONT_SIZE24x12, color);
            }
            refresh |= 0x20;
        }

//        if (seconds !=  ftime->second) {
//            seconds =  ftime->second;
//            octet2[0] = (seconds / 10) + 0x30;
//            octet2[1] = (seconds % 10) + 0x30;
//            octet2[2] = 0;
//            if (config.rotate == APP_EPD_ROTATE_0) {
//                epd_paint_showString(TIME_0_X + 6*12, TIME_0_Y, octet2, EPD_FONT_SIZE24x12, color);
//            } else {
//                epd_paint_showString(TIME_90_X + 6*12, TIME_90_Y, octet2, EPD_FONT_SIZE24x12, color);
//            }
//            refresh = true;
//        }

    }

    if (zb_isDeviceJoinedNwk()) {

        if (!epd_screen_variable.zbIcon) {
            epd_screen_variable.zbIcon = true;
            if (epd_screen_variable.timerZbIcon) {
                TL_ZB_TIMER_CANCEL(&epd_screen_variable.timerZbIcon);
            }

            if (config.inversion == APP_EPD_INVERSION_OFF) {
                color = EPD_COLOR_WHITE;
            } else {
                color = EPD_COLOR_BLACK;
            }
            if (config.rotate == APP_EPD_ROTATE_0) {
                epd_paint_showPicture(ZB_0_X, ZB_0_Y, 24, 24, zb_logo, color);
            } else {
                epd_paint_showPicture(ZB_90_X, ZB_90_Y, 24, 24, zb_logo, color);
            }
            refresh |= 0x40;
        }

    }

    if (refresh) {
//        uint8_t ret = epd_init_partial(); //epd_init();
//        printf("epd_init ret: %d\r\n", ret);
        epd_displayBW_partial(image_bw);
//        epd_enter_deepsleepmode(EPD_DEEPSLEEP_MODE1);
    }
}

static int32_t epd_screen_varCb(void *args) {

    TL_SCHEDULE_TASK(epd_screen_var, NULL);
    return 0;
}

static void epd_screen_invar() {

    uint8_t color, clear;
    uint8_t external[] = EPD_EXTERNAL;
    uint8_t internal[] = EPD_INTERNAL;
    uint8_t temp[] = EPD_TEMP;
    uint8_t rh[] = EPD_RH;

    if (config.inversion == APP_EPD_INVERSION_OFF) {
        color = EPD_COLOR_BLACK;
        clear = EPD_COLOR_WHITE;
    } else {
        color = EPD_COLOR_WHITE;
        clear = EPD_COLOR_BLACK;
    }

    epd_paint_selectimage(image_bw);

    epd_paint_clear(clear);

    if (config.rotate == APP_EPD_ROTATE_0) {
        epd_paint_drawRectangle(BORDER_0_X_START, BORDER_0_Y_START, BORDER_0_X_END, BORDER_0_Y_END, color, 0);
//        epd_paint_drawLine(LINE1_0_X_START, LINE1_0_Y_START, LINE1_0_X_END, LINE1_0_Y_END, color);
        epd_paint_drawLine(LINE2_0_X_START, LINE2_0_Y_START, LINE2_0_X_END, LINE2_0_Y_END, color);
        epd_paint_drawLine(LINE3_0_X_START, LINE3_0_Y_START, LINE3_0_X_END, LINE3_0_Y_END, color);
        epd_paint_showString(EXTERNAL_0_X, EXTERNAL_0_Y, external, EPD_FONT_SIZE24x12, color);
        epd_paint_showString(INTERNAL_0_X, INTERNAL_0_Y, internal, EPD_FONT_SIZE24x12, color);
//        epd_paint_showString(EXTERNAL_0_X, EXTERNAL_0_Y, external, EPD_FONT_SIZE36x22, EPD_COLOR_WHITE);
//        epd_paint_showString(EXTERNAL_0_X, EXTERNAL_0_Y+10, external, EPD_FONT_SIZE12x6, color);
//        epd_paint_showString(EXTERNAL_0_X, EXTERNAL_0_Y+30, external, EPD_FONT_SIZE16x8, color);
//        epd_paint_showString(EXTERNAL_0_X, EXTERNAL_0_Y+60, external, EPD_FONT_SIZE24x12, color);

    } else {
        epd_paint_drawRectangle(BORDER_90_X_START, BORDER_90_Y_START, BORDER_90_X_END, BORDER_90_Y_END, color, 0);
//        epd_paint_drawLine(LINE1_90_X_START, LINE1_90_Y_START, LINE1_90_X_END, LINE1_90_Y_END, color);
        epd_paint_drawLine(LINE2_90_X_START, LINE2_90_Y_START, LINE2_90_X_END, LINE2_90_Y_END, color);
        epd_paint_drawLine(LINE3_90_X_START, LINE3_90_Y_START, LINE3_90_X_END, LINE3_90_Y_END, color);
        epd_paint_drawLine(LINE4_90_X_START, LINE4_90_Y_START, LINE4_90_X_END, LINE4_90_Y_END, color);
    }

    epd_displayBW_partial(image_bw);
//    epd_enter_deepsleepmode(EPD_DEEPSLEEP_MODE1);

}

static void epd_logo() {

    uint16_t x, y;
    uint8_t fwv[]  = ZCL_BASIC_SW_BUILD_ID;
    uint8_t vers[20] = "Firmware ";
    uint8_t name[] = "Air Quality Monitor";
    uint8_t manufacturer[] = "Slacky-DIY & DIY&Zi";
    uint8_t color, clear;

    if (config.inversion == APP_EPD_INVERSION_OFF) {
        color = EPD_COLOR_BLACK;
        clear = EPD_COLOR_WHITE;
    } else {
        color = EPD_COLOR_WHITE;
        clear = EPD_COLOR_BLACK;
    }

    strcat((char*)vers, (char*)(fwv+1));

    epd_paint_selectimage(image_bw);

    epd_paint_clear(clear);

    if (config.rotate == APP_EPD_ROTATE_0) {
        x = 88;
        y = 100;
    } else {
        x = 36;
        y = 142;
    }

    epd_paint_showString(x, y, name, EPD_FONT_SIZE24x12, color);
    epd_paint_showString(x+18, y+30, vers, EPD_FONT_SIZE24x12, color);
    epd_paint_showString(x, y+60, manufacturer, EPD_FONT_SIZE24x12, color);
    epd_displayBW_partial(image_bw);

    sleep_ms(TIMEOUT_3SEC);
}

 static void epd_newimage() {

     if (config.rotate == APP_EPD_ROTATE_0) {
         real_rotate = EPD_ROTATE_270;
     } else {
         real_rotate = EPD_ROTATE_0;
     }

     epd_paint_newimage(image_bw, EPD_W, EPD_H, real_rotate, EPD_COLOR_WHITE);

 }

 static void epd_clear() {
     epd_paint_selectimage(image_bw);
     epd_paint_clear(EPD_COLOR_WHITE);
     epd_displayBW(image_bw);
     sleep_ms(TIMEOUT_1SEC);
 }

void app_epd_init() {

    epd_io_init();

    epd_init();
    epd_newimage();
    epd_clear();

    epd_init_partial();

    //epd_newimage();

//    epd_logo();
//    epd_clear();
    epd_screen_invar();
    epd_screen_var(NULL);
    TL_ZB_TIMER_SCHEDULE(epd_screen_varCb, NULL, TIMEOUT_10SEC);
    //d_zb();

//    for (uint16_t i = 0; i < sizeof(Font36_Table); i++) {
//        if (i % 108 == 0) {
//            printf("{0x%s%x,", Font36_Table[i]>0x0f?"":"0", Font36_Table[i]);
//        } else if (i % 108 == 107) {
//            printf("0x%s%x},\r\n", Font36_Table[i]>0x0f?"":"0", Font36_Table[i]);
//        } else {
//            printf("0x%s%x,", Font36_Table[i]>0x0f?"":"0", Font36_Table[i]);
//        }
//    }
}

void epd_zb() {

    uint8_t color;

    if (config.inversion == APP_EPD_INVERSION_OFF) {
        color = EPD_COLOR_WHITE;
    } else {
        color = EPD_COLOR_BLACK;
    }
    if (config.rotate == APP_EPD_ROTATE_0) {
        epd_paint_showPicture(ZB_0_X, ZB_0_Y, 24, 24, zb_logo, color);
    } else {
        epd_paint_showPicture(ZB_90_X, ZB_90_Y, 24, 24, zb_logo, color);
    }
    epd_displayBW_partial(image_bw);

//    printf("const uint8_t  zb_logo[] = {\r\n");
//    uint8_t ch;
//    for (uint8_t i = 0; i < sizeof(zb_logo1); i++) {
//        ch = zb_logo1[i];
//        ch = ~ch;
//        if (ch > 9) {
//            printf("\"0x%x\", ", ch);
//        } else {
//            printf("\"0x0%x\", ", ch);
//        }
//    }
//    printf("};\r\n");
}
