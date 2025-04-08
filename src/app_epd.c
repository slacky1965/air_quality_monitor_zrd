#include "app_main.h"

#include "epaper.h"
#include "bmp.h"
#include "app_epd.h"
#include "app_epd_images.h"
//#include "app_epd_fonts.h"

static uint8_t image_bw[EPD_W_BUFF_SIZE * EPD_H];

static uint16_t real_rotate = EPD_ROTATE_270;

static uint8_t minutes = 61;

static epd_screen_variable_t epd_screen_variable = {
        .minutes = 61,
        .temp_in = 0,
        .temp_out = 0,
        .humidity_in = 0,
        .humidity_out = 0,
        .pressure = 0,
        .level = 6,
        .zbIcon = false,
        .timerZbIcon = NULL,
};

static void epd_screen_var(void *args) {

//    print_time();
//    printf("epd_screen_var. zb_isDeviceJoinedNwk: %d\r\n", zb_isDeviceJoinedNwk());

    ftime_t *ftime;
    uint8_t m;
    uint16_t y;

    uint8_t level;
    uint8_t *pLevel = NULL;

    uint16_t attr_len;
    uint32_t counter;
    uint8_t lqi;
    status_t ret;

    /*                    0123456789012345678901*/
    uint8_t datetime[] = "01-01-2025  Wed  10:00";
    uint8_t idx;

    uint8_t zb_clear[] = "   ";

    uint8_t color;
    uint8_t refresh = 0;

    uint8_t wday_str[][4] = {"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};
    uint8_t *p_wday_str;

    if (config.inversion == APP_EPD_INVERSION_OFF) {
        color = EPD_COLOR_BLACK;
    } else {
        color = EPD_COLOR_WHITE;
    }

    ret = zcl_getAttrVal(APP_ENDPOINT1, ZCL_CLUSTER_GEN_TIME, ZCL_ATTRID_LOCAL_TIME, &attr_len, (uint8_t*)&counter);

    if (ret == ZCL_STA_SUCCESS && counter != 0xFFFFFFFF) {

        ftime = get_ftime(counter);

        if (minutes !=  ftime->minute) {
            minutes =  ftime->minute;
            idx = 0;
            datetime[idx++] = (ftime->day / 10) + 0x30;
            datetime[idx++] = (ftime->day % 10) + 0x30;
            datetime[idx++] = '-';
            datetime[idx++] = (ftime->month / 10) + 0x30;
            datetime[idx++] = (ftime->month % 10) + 0x30;
            datetime[idx++] = '-';
            datetime[idx++] = (ftime->year / 1000) + 0x30;
            y = ftime->year % 1000;
            datetime[idx++] = (y / 100) + 0x30;
            m = y % 100;
            datetime[idx++] = (m / 10) + 0x30;
            datetime[idx++] = (m % 10) + 0x30;
            datetime[idx++] = ' ';
            datetime[idx++] = ' ';
            p_wday_str = wday_str[ftime->wday];
            datetime[idx++] = *p_wday_str++;
            datetime[idx++] = *p_wday_str++;
            datetime[idx++] = *p_wday_str;
            datetime[idx++] = ' ';
            datetime[idx++] = ' ';
            datetime[idx++] = (ftime->hour / 10) + 0x30;
            datetime[idx++] = (ftime->hour % 10) + 0x30;
            datetime[idx++] = ':';
            datetime[idx++] = (ftime->minute / 10) + 0x30;
            datetime[idx++] = (ftime->minute % 10) + 0x30;
            datetime[idx] = 0;

            if (config.rotate == APP_EPD_ROTATE_0) {
                epd_paint_showString(DATETIME_0_X, DATETIME_0_Y, datetime, EPD_FONT_SIZE24x12, color);
            } else {
                epd_paint_showString(DATETIME_90_X, DATETIME_90_Y, datetime, EPD_FONT_SIZE24x12, color);
            }
            refresh |= 0x01;
        }

    }

    uint16_t sh_addr = zb_getLocalShortAddr();

    if (sh_addr != 0xFFFF) {

        led_blink_stop();

//        printf("zb_getLocalShortAddr != ffff: 0x%x\r\n", sh_addr);

        if (!epd_screen_variable.zbIcon) {
//            printf("!epd_screen_variable.zbIcon\r\n");
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
            refresh |= 0x02;
        }

    } else {

//        printf("zb_getLocalShortAddr == ffff: 0x%x\r\n", sh_addr);
        if (epd_screen_variable.zbIcon) {
//            printf("epd_screen_variable.zbIcon\r\n");
            epd_screen_variable.zbIcon = false;
            if (epd_screen_variable.timerZbIcon) {
                TL_ZB_TIMER_CANCEL(&epd_screen_variable.timerZbIcon);
            }
            if (config.rotate == APP_EPD_ROTATE_0) {
                epd_paint_showString(ZB_0_X, ZB_0_Y, zb_clear, EPD_FONT_SIZE24x12, color);
            } else {
                epd_paint_showString(ZB_90_X, ZB_90_Y, zb_clear, EPD_FONT_SIZE24x12, color);
            }
            refresh |= 0x02;
        }


    }

    ret = zcl_getAttrVal(APP_ENDPOINT1, ZCL_CLUSTER_GEN_DIAGNOSTICS, ZCL_DIAGNOSTICS_ATTRID_LAST_MESSAGE_LQI, &attr_len, (uint8_t*)&lqi);

    if (ret == ZCL_STA_SUCCESS) {
        if (lqi == 0) {
            level = 0;
            pLevel = (uint8_t*)level0;
        } else if (lqi <= 51) {
            level = 1;
            pLevel = (uint8_t*)level1;
        } else if (lqi <= 102) {
            level = 2;
            pLevel = (uint8_t*)level2;
        } else if (lqi <= 153){
            level = 3;
            pLevel = (uint8_t*)level3;
        } else if (lqi <= 204){
            level = 4;
            pLevel = (uint8_t*)level4;
        } else {
            level = 5;
            pLevel = (uint8_t*)level5;
        }

        if (epd_screen_variable.level != level) {
            epd_screen_variable.level = level;
            if (config.inversion == APP_EPD_INVERSION_OFF) {
                color = EPD_COLOR_WHITE;
            } else {
                color = EPD_COLOR_BLACK;
            }
            if (config.rotate == APP_EPD_ROTATE_0) {
                epd_paint_showPicture(LEVEL_0_X, LEVEL_0_Y, 32, 24, pLevel, color);
            } else {
                epd_paint_showPicture(LEVEL_90_X, LEVEL_90_Y, 32, 24, pLevel, color);
            }
            refresh |= 0x04;

        }
    }

    if (refresh) {
//        printf("refresh: 0x%x\r\n", refresh);
//        uint8_t ret = epd_init_partial();
//        epd_init();
//        printf("epd_init ret: %d\r\n", ret);
        epd_displayBW_partial(image_bw);
//        epd_enter_deepsleepmode(EPD_DEEPSLEEP_MODE1);
    }

//    uint8_t buf[2048] = {0};
//    zdo_mgmtLqiIndicate(buf);
//
//    printf("lqi buf: ");
//
//    for (uint8_t i = 0; i < 32; i++) {
//        printf("%s%d", buf[i] < 0x10?"0":"", buf[i]);
//    }
//
//    printf("\r\n");
//    uint16_t lqi;
//    int8_t rssi = ZB_RADIO_RSSI_GET();
////    if (rssi < -100) rssi = -90;
//    ZB_RADIO_RSSI_TO_LQI(1, rssi, lqi);
//    printf("LQI: %d\r\n", lqi);;
//    printf("ZB_RADIO_RSSI_GET: %d\r\n", rssi);

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

//    uint16_t x, y;
    uint8_t fwv[]  = ZCL_BASIC_SW_BUILD_ID;
    uint8_t vers[20] = "Firmware ";
    uint8_t name[] = "Air Quality Monitor";
    uint8_t manufacturer[] = "Slacky-DIY";
    uint8_t joined1[] = "To connect to the ZigBee network,";
    uint8_t joined2[] = "press and hold the button for 5 seconds";
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
        epd_paint_showString(STR_MANUF_0_X, STR_MANUF_0_Y, manufacturer, EPD_FONT_SIZE16x8, color);
        epd_paint_showString(STR_MODEL_0_X, STR_MODEL_0_Y, name, EPD_FONT_SIZE24x12, color);
        epd_paint_showString(STR_JOINED_0_X, STR_JOINED_0_Y, joined1, EPD_FONT_SIZE16x8, color);
        epd_paint_showString(STR_JOINED_0_X-22, STR_JOINED_0_Y+20, joined2, EPD_FONT_SIZE16x8, color);
        epd_paint_showString(STR_FW_0_X, STR_FW_0_Y, vers, EPD_FONT_SIZE8x6, color);
    } else {
        epd_paint_showString(STR_MANUF_90_X, STR_MANUF_90_Y, manufacturer, EPD_FONT_SIZE16x8, color);
        epd_paint_showString(STR_MODEL_90_X, STR_MODEL_90_Y, name, EPD_FONT_SIZE24x12, color);
        epd_paint_showString(STR_JOINED_90_X, STR_JOINED_90_Y, joined1, EPD_FONT_SIZE16x8, color);
        epd_paint_showString(STR_JOINED_90_X-22, STR_JOINED_90_Y+20, joined2, EPD_FONT_SIZE16x8, color);
        epd_paint_showString(STR_FW_90_X, STR_FW_90_Y, vers, EPD_FONT_SIZE8x6, color);
    }

//    epd_paint_showString(x+18, y+30, vers, EPD_FONT_SIZE24x12, color);
//    epd_paint_showString(x, y+60, manufacturer, EPD_FONT_SIZE24x12, color);
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

    if (zb_getLocalShortAddr() == 0xFFFF) {
        epd_logo();
        epd_clear();
    }

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

