#include "app_main.h"

#include "epaper.h"
#include "bmp.h"
#include "app_epd.h"
#include "app_epd_images.h"

static uint8_t image_bw[EPD_W_BUFF_SIZE * EPD_H];

static uint16_t real_rotate = EPD_ROTATE_270;

static bool show_logo = false;

static epd_screen_variable_t epd_screen_variable = {
        .minutes = 61,
        .temp_in = 0x8001,
        .temp_out = 0x8001,
        .humidity_in = 0xfffe,
        .humidity_out = 0xfffe,
        .battery_percent = 0xfe,
        .pressure = 0,
        .co2 = 0,
        .voc = 0,
        .lux = 0,
        .lqi = 256,
        .level = 6,
        .zbIcon = false,
//        .timerZbIcon = NULL,
};

static void epd_screen_invar();
static void epd_clear();

static void epd_show_temperature(uint16_t x, uint16_t y, int32_t temp, uint16_t color) {

    /* for test */
//    static int16_t temp_c = 2297;
//
//    temp_c = -temp_c;
//
//    temp = temp_c;
//
//    if (temp_c < 0)
//        temp_c--;
//    else
//        temp_c++;

    int16_t temp_int;
    int16_t temp_rem, temp_remt;
    bool negative = false;
    uint8_t str_temp[6] = " --  ";
    uint8_t *ptr;
    uint8_t str_celsius[] = "@C"; // replacement in font30 @ -> °
    uint8_t str_fahren[]  = "@F"; // replacement in font30 @ -> °
    uint8_t *ptr_degree = NULL;
    sFont *font = NULL;
    uint16_t xx, yy;

    button_handler();

    if ((uint16_t)temp != 0x8000) {

        if (config.d_mode == ZCL_DISPLAY_MODE_FAHRENHEIT) {
            temp = temperature_convert_CtoF(temp);
        }

        if (temp < 0) {
            temp = -temp;
            negative = true;
        }

        temp_int = temp / 100;

        temp_remt = temp % 100;

        if (temp < 5)
            temp_rem = 1;
        else
            if((temp_remt % 10) >= 5 && (temp_remt / 10) == 9 ) {
                //temp_int++;
                temp_rem = temp_remt / 10;
            } else
                temp_rem = temp_remt / 10 + (temp_remt % 10 >= 5?1:0);

        ptr = str_temp;

        if (negative)
            *ptr++ = '-';
        else
            *ptr++ = ' ';

        if (itoa(temp_int, ptr)) {

            uint8_t len = strlen((const char*)ptr);

            if (len >= 3) {
                ptr += 3;
                *ptr = 0;
            } else if (len == 2) {
                ptr += 2;
            } else {
                ptr++;
            }

            if (len < 3) {
                if (temp_rem == 0) {
                    *ptr++ = ' ';
                    *ptr++ = ' ';
                    *ptr = 0;
                } else {
                    *ptr++ = '.';

                    if (!itoa(temp_rem, ptr)) {
                        ptr = str_temp;
                        *ptr++ = ' ';
                        *ptr++ = '-';
                        *ptr++ = '-';
                        *ptr++ = ' ';
                        *ptr++ = ' ';
                        *ptr++ = 0;
                    }
                }
            }
        } else {
            ptr = str_temp;
            *ptr++ = ' ';
            *ptr++ = '-';
            *ptr++ = '-';
            *ptr++ = ' ';
            *ptr++ = ' ';
            *ptr++ = 0;
        }
    }

    if (config.rotate == APP_EPD_ROTATE_0) {
        font = &font41;
        xx = 111;
        yy = 10;
    } else {
        xx = 105;
        yy = 8;
        font = &font38;
    }

    if (config.d_mode == ZCL_DISPLAY_MODE_FAHRENHEIT)
        ptr_degree = str_fahren;
    else
        ptr_degree = str_celsius;

    epd_paint_showString(x-5, y, str_temp, font, color, true);
    epd_paint_showString(x+xx, y+yy, ptr_degree, &font30, color, true);

//    printf("str temp: '%s', temp: %d, neg: %d\r\n", str_temp, temp, negative);
}

static void epd_screen_var(void *args) {

    if (g_appCtx.net_steer_start || led_flashing())
        return;


//    print_time();
//    printf("epd_screen_var. zb_isDeviceJoinedNwk: %d\r\n", zb_isDeviceJoinedNwk());

    ftime_t *ftime;
    uint8_t m;
    uint16_t x, y;

    uint8_t level, bat;
    uint8_t *pLevel = NULL;

    uint16_t attr_len;
    uint32_t counter;
    int16_t temp;
    uint16_t rh, co2, voc, lux, kpa;
//    uint8_t lqi;
    status_t ret;

    /*                    01234567890123456789*/
    uint8_t datetime[] = "01-01-2025 Wed 10:00";
    uint8_t idx;

    uint8_t zb_clear[] = "   ";

    uint8_t color;
    uint32_t refresh = 0;

    uint8_t wday_str[][4] = {"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};
    uint8_t *p_wday_str;
    bool update_time = false;
    uint8_t idx_date_stop, idx_time_start;

    led_color_t led_color = led_get_color();

    epd_io_init();
    button_handler();
    epd_reset();

    if (config.inversion == APP_EPD_INVERSION_OFF) {
        color = EPD_COLOR_BLACK;
    } else {
        color = EPD_COLOR_WHITE;
    }

    ds3231_time_t t;
    if (app_ds3231_get_time(&t) == DS3231_OK) {
//        printf("time from ds3231\r\n");
        if (epd_screen_variable.minutes != t.minute) {
            epd_screen_variable.minutes = t.minute;
            update_time = true;

            idx = 0;
            datetime[idx++] = (t.date / 10) + 0x30;
            datetime[idx++] = (t.date % 10) + 0x30;
            datetime[idx++] = '-';
            datetime[idx++] = (t.month / 10) + 0x30;
            datetime[idx++] = (t.month % 10) + 0x30;
            datetime[idx++] = '-';
            datetime[idx++] = (t.year / 1000) + 0x30;
            y = t.year % 1000;
            datetime[idx++] = (y / 100) + 0x30;
            m = y % 100;
            datetime[idx++] = (m / 10) + 0x30;
            datetime[idx++] = (m % 10) + 0x30;
            datetime[idx++] = ' ';
            p_wday_str = wday_str[t.week-1];
            datetime[idx++] = *p_wday_str++;
            datetime[idx++] = *p_wday_str++;
            datetime[idx++] = *p_wday_str;
            idx_date_stop = idx;
            datetime[idx++] = ' ';
            idx_time_start = idx;
            datetime[idx++] = (t.hour / 10) + 0x30;
            datetime[idx++] = (t.hour % 10) + 0x30;
            datetime[idx++] = ':';
            datetime[idx++] = (t.minute / 10) + 0x30;
            datetime[idx++] = (t.minute % 10) + 0x30;
            datetime[idx] = 0;
        }
    } else {

        ret = zcl_getAttrVal(APP_ENDPOINT1, ZCL_CLUSTER_GEN_TIME, ZCL_ATTRID_LOCAL_TIME, &attr_len, (uint8_t*)&counter);

        if (ret == ZCL_STA_SUCCESS && counter != 0xFFFFFFFF) {

//            printf("time from tlsr8258\r\n");

            ftime = get_ftime(counter);

            if (epd_screen_variable.minutes !=  ftime->minute) {
                epd_screen_variable.minutes =  ftime->minute;
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
                p_wday_str = wday_str[ftime->wday];
                datetime[idx++] = *p_wday_str++;
                datetime[idx++] = *p_wday_str++;
                datetime[idx++] = *p_wday_str;
                idx_date_stop = idx;
                datetime[idx++] = ' ';
                idx_time_start = idx;
                datetime[idx++] = (ftime->hour / 10) + 0x30;
                datetime[idx++] = (ftime->hour % 10) + 0x30;
                datetime[idx++] = ':';
                datetime[idx++] = (ftime->minute / 10) + 0x30;
                datetime[idx++] = (ftime->minute % 10) + 0x30;
                datetime[idx] = 0;

                update_time = true;
            }
        }
    }

    if (update_time) {
        if (config.rotate == APP_EPD_ROTATE_0) {
            epd_paint_showString(56, 0, datetime, &font26, color, true);
        } else {
            datetime[idx_date_stop] = 0;
            epd_paint_showString(50, 373, datetime, &font26, color, true);
            epd_paint_showString(180, 50, datetime+idx_time_start, &font38, color, false);
        }
        refresh |= 0x01;

    }

    button_handler();

    uint16_t sh_addr = zb_getLocalShortAddr();

    if (sh_addr < 0xFFF8) {

//        led_blink_stop();

        if (!epd_screen_variable.zbIcon) {
//            printf("!epd_screen_variable.zbIcon\r\n");
            epd_screen_variable.zbIcon = true;
//            if (epd_screen_variable.timerZbIcon) {
//                TL_ZB_TIMER_CANCEL(&epd_screen_variable.timerZbIcon);
//            }
            if (config.rotate == APP_EPD_ROTATE_0) {
                epd_paint_showPicture(6, 0, 24, 24, image_zbLogo, color);
            } else {
                epd_paint_showPicture(6, 0, 24, 24, image_zbLogo, color);
            }
            refresh |= 0x02;
        }

    } else {

        if (epd_screen_variable.zbIcon) {
//            printf("epd_screen_variable.zbIcon\r\n");
            epd_screen_variable.zbIcon = false;
//            if (epd_screen_variable.timerZbIcon) {
//                TL_ZB_TIMER_CANCEL(&epd_screen_variable.timerZbIcon);
//            }
            if (config.rotate == APP_EPD_ROTATE_0) {
                epd_paint_showString(6, 0, zb_clear, &font24, color, true);
            } else {
                epd_paint_showString(6, 0, zb_clear, &font24, color, true);
            }
            refresh |= 0x02;

            app_lqiCb(NULL);
        }


    }

    button_handler();

    uint8_t lqi = app_get_lqi();

    if (lqi == 0) {
        level = 0;
        pLevel = (uint8_t*)image_level0;
    } else if (lqi <= 51) {
        level = 1;
        pLevel = (uint8_t*)image_level1;
    } else if (lqi <= 102) {
        level = 2;
        pLevel = (uint8_t*)image_level2;
    } else if (lqi <= 153){
        level = 3;
        pLevel = (uint8_t*)image_level3;
    } else if (lqi <= 204){
        level = 4;
        pLevel = (uint8_t*)image_level4;
    } else {
        level = 5;
        pLevel = (uint8_t*)image_level5;
    }

    if (epd_screen_variable.level != level) {
        epd_screen_variable.level = level;
        if (config.rotate == APP_EPD_ROTATE_0) {
            epd_paint_showPicture(361, 0, 32, 24, pLevel, color);
        } else {
            epd_paint_showPicture(261, 0, 32, 24, pLevel, color);
        }
        refresh |= 0x04;
    }

    button_handler();

    ret = zcl_getAttrVal(APP_ENDPOINT1, ZCL_CLUSTER_MS_TEMPERATURE_MEASUREMENT, ZCL_TEMPERATURE_MEASUREMENT_ATTRID_MEASUREDVALUE, &attr_len, (uint8_t*)&temp);

    if (ret == ZCL_STA_SUCCESS && epd_screen_variable.temp_in != temp) {
        epd_screen_variable.temp_in = temp;

        if (config.rotate == APP_EPD_ROTATE_0) {
            epd_show_temperature(36, 210, temp, color);
        } else {
            epd_show_temperature(36, 277, temp, color);
        }

        refresh |= 0x08;
    }

    button_handler();

    ret = zcl_getAttrVal(APP_ENDPOINT1, ZCL_CLUSTER_MS_RELATIVE_HUMIDITY, ZCL_ATTRID_HUMIDITY_MEASUREDVALUE, &attr_len, (uint8_t*)&rh);

    if (ret == ZCL_STA_SUCCESS && epd_screen_variable.humidity_in != rh) {
        epd_screen_variable.humidity_in = rh;

        uint8_t str_hum[5] = "-- ";
        uint8_t str_rh[] = "%Rh";

        if (rh != 0xffff) {

            rh /= 100;

            if (epd_screen_variable.humidity_out % 100 >= 50)
                rh++;

            if (rh > 100)
                rh = 100;


            if (!itoa(rh, str_hum)) {
                uint8_t *ptr = str_hum;
                *ptr++ = '-';
                *ptr++ = '-';
                *ptr++ = ' ';
            }

            if (strlen((const char*)str_hum) == 2) {
                str_hum[3] = ' ';
                str_hum[4] = 0;
            }
        }

        if (config.rotate == APP_EPD_ROTATE_0) {
            epd_paint_showString(56, 255, str_hum, &font41, color, true);
            epd_paint_showString(146, 263, str_rh, &font30, color, true);
        } else {
            epd_paint_showString(203, 277, str_hum, &font41, color, true);
            epd_paint_showChar(273, 285, '%', &font30, color);
        }

        refresh |= 0x10;

    }

    button_handler();

    /* outsize temperature */
    temp = app_get_outside_temperature();
    if (epd_screen_variable.temp_out != temp) {
        epd_screen_variable.temp_out = temp;

        if (config.rotate == APP_EPD_ROTATE_0) {
            epd_show_temperature(236, 210, temp, color);
        } else {
            epd_show_temperature(36, 330, temp, color);
        }

        refresh |= 0x20;

    }

    button_handler();

    /* outside humidity */
    rh = app_get_outside_humidity();
    if (epd_screen_variable.humidity_out != rh) {
        epd_screen_variable.humidity_out = rh;

        uint8_t str_hum[5] = "-- ";
        uint8_t str_rh[] = "%Rh";

        if (rh != 0xffff) {

            rh /= 100;

            if (epd_screen_variable.humidity_out % 100 >= 50)
                rh++;

            if (rh > 100)
                rh = 100;


            if (!itoa(rh, str_hum)) {
                uint8_t *ptr = str_hum;
                *ptr++ = '-';
                *ptr++ = '-';
                *ptr++ = ' ';
            }

            if (strlen((const char*)str_hum) == 2) {
                str_hum[3] = ' ';
                str_hum[4] = 0;
            }
        }


        if (config.rotate == APP_EPD_ROTATE_0) {
            epd_paint_showString(261-5, 255, str_hum, &font41, color, true);
            epd_paint_showString(346, 263, str_rh, &font30, color, true);
        } else {
            epd_paint_showString(203, 330, str_hum, &font41, color, true);
            epd_paint_showChar(273, 338, '%', &font30, color);
        }

        refresh |= 0x40;

    }

    button_handler();

    /* outside battery */
    bat = app_get_outside_battery();

    if (epd_screen_variable.battery_percent != bat) {
        epd_screen_variable.battery_percent = bat;

        uint8_t *p_image_bat = NULL;

        if (bat == 0xFF)
            bat = 0;

        bat /= 2;

        if (bat < 10)
            p_image_bat = (uint8_t*)image_battery_0;
        else if (bat < 30)
            p_image_bat = (uint8_t*)image_battery_25;
        else if (bat < 60)
            p_image_bat = (uint8_t*)image_battery_50;
        else if (bat < 80)
            p_image_bat = (uint8_t*)image_battery_75;
        else
            p_image_bat = (uint8_t*)image_battery_100;

        if (config.rotate == APP_EPD_ROTATE_0) {
            epd_paint_showPicture(374, 208, 22, 10, p_image_bat, color);
        } else {
            epd_paint_showPicture(274, 323, 22, 10, p_image_bat, color);
        }
        refresh |= 0x80;
    }

    button_handler();

    co2 = app_scd4x_get_co2();

    if (co2 != epd_screen_variable.co2) {

//        printf("epd_co2: %d\r\n", co2);

        epd_screen_variable.co2 = co2;

        uint8_t str_co2[8] = {0};
        uint8_t str_co[] = "CO";
        uint8_t str_ppm[] = "PPM";

        if (itoa(co2, str_co2)) {

            sFont *fontCO2 = NULL;
            uint16_t xCO2;

            if (config.rotate == APP_EPD_ROTATE_0) {
                fontCO2 = &font54;
                xCO2 = 60 + fontCO2->width / 2;
                epd_paint_showString(xCO2 - (4 / 2 * fontCO2->width), 60, (uint8_t*)"    ", fontCO2, color, false);
                xCO2 = xCO2 - (strlen((char*)str_co2) * fontCO2->width / 2);
                /* print value CO2 */
                epd_paint_showString(xCO2, 60, str_co2, fontCO2, color, false);
                /* print string "CO" */
                epd_paint_showString(23, 140, str_co, &font30, color, true);
                /* print number 2 */
                epd_paint_showNum(60, 150, 2, 1, &font19, color);
                /* print string "PPM" */
                epd_paint_showString(76, 140, str_ppm, &font30, color, true);
            } else {
                fontCO2 = &font58;
                xCO2 = 60 + fontCO2->width / 2;
                epd_paint_showString(xCO2 - (4 / 2 * fontCO2->width), 40, (uint8_t*)"    ", fontCO2, color, false);
                xCO2 = xCO2 - (strlen((char*)str_co2) * fontCO2->width / 2);
                /* print value CO2 */
                epd_paint_showString(xCO2, 40, str_co2, fontCO2, color, false);
                /* print string "CO" */
                epd_paint_showString(23, 100, str_co, &font30, color, true);
                /* print number 2 */
                epd_paint_showNum(60, 110, 2, 1, &font19, color);
                /* print string "PPM" */
                epd_paint_showString(78, 100, str_ppm, &font30, color, true);
            }

            refresh |= 0x100;
        }
    }

    button_handler();

    voc = app_sgp40_get_voc();

    if (epd_screen_variable.voc != voc) {

        epd_screen_variable.voc = voc;

        uint8_t str_voc_val[8] = {0};
        uint8_t str_voc[] = "VOC";
        uint8_t str_ppm[] = "InP";

        if (itoa(voc, str_voc_val)) {

            sFont *fontVOC = NULL;
            uint16_t xVOC;

//            strcpy((char*)str_voc_val, "0000");

            if (config.rotate == APP_EPD_ROTATE_0) {
                fontVOC = &font54;
                xVOC = 210 + fontVOC->width / 2;
                epd_paint_showString(xVOC - (4 / 2 * fontVOC->width), 60, (uint8_t*)"    ", fontVOC, color, false);
                xVOC = xVOC - (strlen((char*)str_voc_val) * fontVOC->width / 2);
                /* print value VOC */
                epd_paint_showString(xVOC, 60, str_voc_val, fontVOC, color, false);
                /* print string "VOC" */
                epd_paint_showString(168, 140, str_voc, &font30, color, true);
                /* print string "PPM" */
                epd_paint_showString(230, 140, str_ppm, &font30, color, true);
            } else {
                fontVOC = &font58;
                xVOC = 60 + fontVOC->width / 2;
                epd_paint_showString(xVOC - (4 / 2 * fontVOC->width), 160, (uint8_t*)"    ", fontVOC, color, false);
                xVOC = xVOC - (strlen((char*)str_voc_val) * fontVOC->width / 2);
                /* print value VOC */
                epd_paint_showString(xVOC, 160, str_voc_val, fontVOC, color, false);
                /* print string "VOC" */
                epd_paint_showString(22, 220, str_voc, &font30, color, true);
                /* print string "PPM" */
                epd_paint_showString(78, 220, str_ppm, &font30, color, true);
            }

            refresh |= 0x200;
        }
    }

    button_handler();

    lux = app_bh1750_get_lux();

    uint8_t clear_buf[] = "       ";
    uint8_t clear_cnt = 0;

    sFont *font = &font24;

    if (config.rotate == APP_EPD_ROTATE_0) {
        x = 288;
        y = 60;
    } else {
        x = 188;
        y = 160;
    }

    if (epd_screen_variable.lux != lux) {
        epd_screen_variable.lux = lux;

        uint8_t str_lux_val[5] = {0};
        uint8_t str_lux[] = "Lx";

        if (itoa(lux, str_lux_val)) {

            clear_cnt = strlen((const char*)str_lux_val);

            /* clear print space */
            epd_paint_showString(x, y, clear_buf, font, color, false);
            /* print value LUX */
            epd_paint_showString(x + (font24.width * (5 - clear_cnt)), y, str_lux_val, font, color, false);
            /* print string "Lux" */
            epd_paint_showString(x+89, y+3, str_lux, &font19, color, true);

            refresh |= 0x400;
        }
    }

    button_handler();

    ret = zcl_getAttrVal(APP_ENDPOINT1, ZCL_CLUSTER_MS_PRESSURE_MEASUREMENT, ZCL_ATTRID_PRESSURE_MEASUREDVALUE, &attr_len, (uint8_t*)&kpa);
    y += 26;

    if (ret == ZCL_STA_SUCCESS && kpa != 0x8000) {
        if (epd_screen_variable.pressure != kpa) {
            epd_screen_variable.pressure = kpa;

            uint8_t str_kpa[5] = {0};
            uint8_t str_hpa[] = "hP";
            uint8_t str_mm[] = "mm";

            if (itoa((kpa * 1000 / 1333), str_kpa)) {

//                strcpy((char*)str_kpa, "0000");

                uint8_t clear_cnt = strlen((const char*)str_kpa);

                /* clear print space */
                epd_paint_showString(x, y, clear_buf, &font24, color, false);
                /* print value pressure */
                epd_paint_showString(x + (font24.width * (5 - clear_cnt)), y, str_kpa, &font24, color, false);
                epd_paint_showString(x+89, y+3, str_mm, &font19, color, true);

                refresh |= 0x800;
            }

            y += 26;

            if (itoa(kpa, str_kpa)) {

                uint8_t clear_cnt = strlen((const char*)str_kpa);

                /* clear print space */
                epd_paint_showString(x, y, clear_buf, &font24, color, false);
                /* print value pressure */
                epd_paint_showString(x + (font24.width * (5 - clear_cnt)), y, str_kpa, &font24, color, false);
                epd_paint_showString(x+89, y+3, str_hpa, &font19, color, true);

                refresh |= 0x800;
            }
        } else {
            y += 26;
        }
    } else {
        y += 26;
    }

    button_handler();

    y += 26;

    if (epd_screen_variable.lqi != lqi) {
        epd_screen_variable.lqi = lqi;

        uint8_t str_lqi_val[5] = {0};
        uint8_t str_lqi[] = "Lq";

        if (itoa(lqi, str_lqi_val)) {

            uint8_t clear_cnt = strlen((const char*)str_lqi_val);

            /* clear print space */
            epd_paint_showString(x, y, clear_buf, &font24, color, false);
            /* print value lqi */
            epd_paint_showString(x + (font24.width * (5 - clear_cnt)), y, str_lqi_val, &font24, color, false);
            epd_paint_showString(x+89, y+3, str_lqi, &font19, color, true);

            refresh |= 0x1000;
        }
    }


    button_handler();

    if (refresh) {
        epd_displayBW_partial(image_bw);
        epd_enter_deepsleepmode(EPD_DEEPSLEEP_MODE1);

        if (co2 <= 600 && voc <= 100) {
            if (led_color != COLOR_GREEN) {
                led_color = COLOR_GREEN;
//                led_on(led_color);
            }
        } else if (co2 <= 900 && voc <= 250) {
            if (led_color != COLOR_YELLOW) {
                led_color = COLOR_YELLOW;
//                led_on(led_color);
            }
        } else if (co2 <= 1200 && voc <= 400) {
            if (led_color != COLOR_PURPLE) {
                led_color = COLOR_PURPLE;
//                led_on(led_color);
            }
        } else {
            if (led_color != COLOR_RED) {
                led_color = COLOR_RED;
//                led_on(led_color);
            }
        }

        led_on(led_color);
    }
}

int32_t epd_screen_varCb(void *args) {

    TL_SCHEDULE_TASK(epd_screen_var, NULL);
    return 0;
}

static void epd_screen_invar() {

    uint8_t color, clear;
    uint8_t external[] = "OUTSIDE";
    uint8_t internal[] = "INSIDE";

    epd_io_init();
    epd_reset();

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
        epd_paint_drawRectangle(1, 27, 400, 300, color, 0);
        epd_paint_drawLine(1, 200, 400, 200, color);
        epd_paint_drawLine(200, 200, 200, 300, color);
        epd_paint_showString(1, 192, internal, &font16, color, true);
        epd_paint_showString(328, 192, external, &font16, color, true);
        epd_paint_showPicture(2, 217, 24, 24, image_tempLogo, color);
        epd_paint_showPicture(202, 217, 24, 24, image_tempLogo, color);
        epd_paint_showPicture(2, 261, 24, 24, image_humidityLogo, color);
        epd_paint_showPicture(202, 261, 24, 24, image_humidityLogo, color);


    } else {
        epd_paint_drawRectangle(1, 27, 300, 400, color, 0);
        epd_paint_drawLine(1, 267, 300, 267, color);
        epd_paint_drawLine(1, 320, 300, 320, color);
        epd_paint_drawLine(1, 373, 300, 373, color);
        epd_paint_showString(1, 261, internal, &font13, color, true);
        epd_paint_showString(1, 314, external, &font13, color, true);
        epd_paint_showPicture(2, 282, 24, 24, image_tempLogo, color);
        epd_paint_showPicture(2, 334, 24, 24, image_tempLogo, color);
        epd_paint_showPicture(178, 282, 24, 24, image_humidityLogo, color);
        epd_paint_showPicture(178, 334, 24, 24, image_humidityLogo, color);
    }

    epd_displayBW_partial(image_bw);
    epd_enter_deepsleepmode(EPD_DEEPSLEEP_MODE1);

}

static void epd_logo() {

    epd_io_init();
    epd_reset();

//    uint16_t x, y;
    uint8_t fwv[]  = ZCL_BASIC_SW_BUILD_ID;
    uint8_t vers[20] = "Firmware ";
    uint8_t name[] = "Air Quality Monitor";
    uint8_t manufacturer[] = "Slacky-DIY";
    uint8_t joined1[] = "To connect to the ZigBee network,";
    uint8_t joined2[] = "press and hold the button for 5 seconds";
    uint8_t joined3[] = "press and hold the button";
    uint8_t joined4[] = "for 5 seconds";
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
        epd_paint_showString(6, 0, manufacturer, &font16, color, true);
        epd_paint_showString(70, 100, name, &font26, color, true);
        epd_paint_showString(60, 160, joined1, &font13, color, true);
        epd_paint_showString(26, 180, joined2, &font13, color, true);
        epd_paint_showString(266, 285, vers, &font12, color, true);
    } else {
        epd_paint_showString(6, 0, manufacturer, &font16, color, true);
        epd_paint_showString(16, 142, name, &font26, color, true);
        epd_paint_showString(4, 202, joined1, &font13, color, true);
        epd_paint_showString(42, 222, joined3, &font13, color, true);
        epd_paint_showString(92, 242, joined4, &font13, color, true);
        epd_paint_showString(186, 385, vers, &font12, color, true);
    }

    epd_displayBW_partial(image_bw);
    epd_enter_deepsleepmode(EPD_DEEPSLEEP_MODE1);

    sleep_ms(TIMEOUT_5SEC);
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

     epd_io_init();
     epd_reset();

     epd_paint_selectimage(image_bw);
     epd_paint_clear(EPD_COLOR_WHITE);
     epd_displayBW(image_bw);
     sleep_ms(TIMEOUT_100MS);
 }

void app_epd_init() {

    epd_io_init();

    epd_init();
    epd_newimage();
    epd_clear();

    epd_init_partial();

#if 1
    epd_logo();
    while(1) {
        button_handler();
    }
#endif

    if (zb_getLocalShortAddr() >= 0xFFF8) {
        epd_logo();
        epd_clear();
        show_logo = true;
    }
}

void app_first_start_epd() {

    epd_screen_invar();
    epd_screen_var(NULL);
    TL_ZB_TIMER_SCHEDULE(epd_screen_varCb, NULL, TIMEOUT_10SEC);
}

void epd_update_temperature_display_mode() {
    epd_screen_variable.temp_in = 0x8001;
    epd_screen_variable.temp_out = 0x8001;
}

void epd_clearZbIcon() {

    uint8_t color;
    uint8_t zb_clear[] = "   ";

    epd_io_init();
    epd_reset();

    if (config.inversion == APP_EPD_INVERSION_OFF) {
        color = EPD_COLOR_BLACK;
    } else {
        color = EPD_COLOR_WHITE;
    }

    epd_screen_variable.zbIcon = false;

    if (config.rotate == APP_EPD_ROTATE_0) {
        epd_paint_showString(6, 0, zb_clear, &font24, color, true);
    } else {
        epd_paint_showString(6, 0, zb_clear, &font24, color, true);
    }

    epd_screen_variable.lqi = 256;
    epd_screen_variable.level = 6;

    if (config.rotate == APP_EPD_ROTATE_0) {
        epd_paint_showPicture(361, 0, 32, 24, image_level0, color);
    } else {
        epd_paint_showPicture(261, 0, 32, 24, image_level0, color);
    }

    epd_displayBW_partial(image_bw);
    epd_enter_deepsleepmode(EPD_DEEPSLEEP_MODE1);
}
