#include "tl_common.h"
#include "zcl_include.h"

#include "app_main.h"

typedef struct {
    bool        released;
    bool        pressed;
    bool        hold;
    bool        level_up;
    uint8_t     counter;
    uint8_t     debounce;
    uint32_t    pressed_time;
    uint32_t    released_time;
    uint32_t    hold_time;
} app_button_t;

bool factory_reset = false;
static app_button_t app_button = {
    .debounce = 1,
    .hold = false,
    .counter = 0,
    .pressed = false,
    .released = false,
    .level_up = false,
    .pressed_time = 0,
};
static app_button_t *button = &app_button;


static int32_t net_steer_start_offCb(void *args) {

    g_appCtx.net_steer_start = false;
    factory_reset = false;
    led_blink_stop();

    return -1;
}

int32_t button_handlerCb(void *args) {

    if (!drv_gpio_read(BUTTON1)) {
        if (button->pressed && button->counter != 0xFF) {
            if (clock_time_exceed(button->hold_time, TIMEOUT_TICK_500MS)) {
                if (button->hold == false) {
                    button->hold = true;
                } else {
                    if (clock_time_exceed(button->hold_time, TIMEOUT_TICK_5SEC)) {
#if UART_PRINTF_MODE && DEBUG_BUTTON
                        printf("The button was keep pressed for 5 seconds\r\n");
#endif

                        button->counter = 0xFF;
                        bind_outside_clear();

                        zb_factoryReset();
                        zb_deviceFactoryNewSet(true);

                        g_appCtx.net_steer_start = true;
                        led_off();
                        epd_clearZbIcon();
                        TL_ZB_TIMER_SCHEDULE(net_steer_start_offCb, NULL, TIMEOUT_1MIN30SEC);
                        led_effect_start(90, COLOR_RED);
                    }
                }
            }
        }
        if (button->debounce != DEBOUNCE_BUTTON) {
            button->debounce++;
            if (button->debounce == DEBOUNCE_BUTTON) {
                led_blink_start(1, 30, 1, COLOR_RED);
                if (button->counter == 0) {
                    button->counter++;
                    button->pressed = true;
                    if (!zb_isDeviceJoinedNwk() && !zb_isDeviceFactoryNew()) {
                        zb_rejoinReq(zb_apsChannelMaskGet(), g_bdbAttrs.scanDuration);
                    }
                } else if (button->pressed && !clock_time_exceed(button->pressed_time, TIMEOUT_TICK_500MS)) {
                    button->counter++;
                }
                button->hold_time = button->pressed_time = clock_time();
            }
        }
    } else {
        if (button->debounce != 1) {
            button->debounce--;
            if (button->debounce == 1 && button->pressed) {
                button->released = true;
            }
        }
    }

    if (button->released && button->pressed && clock_time_exceed(button->pressed_time, TIMEOUT_TICK_500MS)) {
#if UART_PRINTF_MODE && DEBUG_BUTTON
        printf("Button pressed %d times\r\n", button->counter);
#endif
        switch (button->counter) {
            case 1:
                TL_ZB_TIMER_SCHEDULE(forcedReportCb, NULL, TIMEOUT_100MS);
                break;
            case 2:
#if UART_PRINTF_MODE && DEBUG_BUTTON
                printf("Rotate\r\n");
#endif
                config.rotate = ~config.rotate;
                config.rotate &= 0x01;
                printf("config rotate: %d\r\n", config.rotate);
                config_save();
                TL_SCHEDULE_TASK(epd_forceScreenUpdate, NULL);
                break;
            case 3:
#if UART_PRINTF_MODE && DEBUG_BUTTON
                printf("Inversion\r\n");
#endif
                config.inversion = ~config.inversion;
                config.inversion &= 0x01;
                printf("config inversion: %d\r\n", config.inversion);
                config_save();
                TL_SCHEDULE_TASK(epd_forceScreenUpdate, NULL);
                break;
            case 4:
#if UART_PRINTF_MODE && DEBUG_BUTTON
                printf("Celsius or Fahrenheit\r\n");
#endif
                config.d_mode = ~config.d_mode;
                config.d_mode &= 0x01;
                config_save();
                printf("config d_mode: %d\r\n", config.d_mode);
                zcl_setAttrVal(APP_ENDPOINT1, ZCL_CLUSTER_HAVC_USER_INTERFACE_CONFIG, ZCL_ATTRID_HVAC_TEMPERATURE_DISPLAY_MODE, (uint8_t*)&config.d_mode);
                epd_update_temperature_display_mode();
                break;
            case 5:
#if UART_PRINTF_MODE && DEBUG_BUTTON
                printf("Find and bind on target start\r\n");
#endif
                if (!g_appCtx.bdbFindBindFlg) {
                    g_appCtx.bdbFindBindFlg = TRUE;
                    TL_ZB_TIMER_SCHEDULE(app_bdbFindAndBindStart, NULL, 1000);
                }
                break;
            case 6:
#if UART_PRINTF_MODE && DEBUG_BUTTON
                printf("Reboot monitor\r\n");
#endif
                TL_ZB_TIMER_SCHEDULE(delayedMcuResetCb, NULL, TIMEOUT_100MS);
                break;
            default:
                printf("default button press %d times\r\n", button->counter);
                break;
        }
        button->counter = 0;
        button->pressed = false;
        button->released = false;
        button->hold = false;
    }
    return 0;
}

void button_handler() {

    return;
    if (!drv_gpio_read(BUTTON1)) {
        if (button->pressed && button->counter != 0xFF) {
            if (clock_time_exceed(button->hold_time, TIMEOUT_TICK_500MS)) {
                if (button->hold == false) {
                    button->hold = true;
                } else {
                    if (clock_time_exceed(button->hold_time, TIMEOUT_TICK_5SEC)) {
#if UART_PRINTF_MODE && DEBUG_BUTTON
                        printf("The button was keep pressed for 5 seconds\r\n");
#endif

                        button->counter = 0xFF;
                        bind_outside_clear();

                        zb_factoryReset();
                        zb_deviceFactoryNewSet(true);

                        g_appCtx.net_steer_start = true;
                        led_off();
                        epd_clearZbIcon();
                        TL_ZB_TIMER_SCHEDULE(net_steer_start_offCb, NULL, TIMEOUT_1MIN30SEC);
                        led_effect_start(90, COLOR_RED);
                    }
                }
            }
        }
        if (button->debounce != DEBOUNCE_BUTTON) {
            button->debounce++;
            if (button->debounce == DEBOUNCE_BUTTON) {
                led_blink_start(1, 30, 1, COLOR_RED);
                if (button->counter == 0) {
                    button->counter++;
                    button->pressed = true;
                    if (!zb_isDeviceJoinedNwk() && !zb_isDeviceFactoryNew()) {
                        zb_rejoinReq(zb_apsChannelMaskGet(), g_bdbAttrs.scanDuration);
                    }
                } else if (button->pressed && !clock_time_exceed(button->pressed_time, TIMEOUT_TICK_500MS)) {
                    button->counter++;
                }
                button->hold_time = button->pressed_time = clock_time();
            }
        }
    } else {
        if (button->debounce != 1) {
            button->debounce--;
            if (button->debounce == 1 && button->pressed) {
                button->released = true;
            }
        }
    }

    if (button->released && clock_time_exceed(button->pressed_time, TIMEOUT_TICK_500MS)) {
#if UART_PRINTF_MODE && DEBUG_BUTTON
        printf("Button pressed %d times\r\n", button->counter);
#endif
        switch (button->counter) {
            case 1:
                TL_ZB_TIMER_SCHEDULE(forcedReportCb, NULL, TIMEOUT_100MS);
                break;
            case 2:
#if UART_PRINTF_MODE && DEBUG_BUTTON
                printf("Rotate\r\n");
#endif
                config.rotate = ~config.rotate;
                config.rotate &= 0x01;
                printf("config rotate: %d\r\n", config.rotate);
                config_save();
                TL_SCHEDULE_TASK(epd_forceScreenUpdate, NULL);
                break;
            case 3:
#if UART_PRINTF_MODE && DEBUG_BUTTON
                printf("Inversion\r\n");
#endif
                config.inversion = ~config.inversion;
                config.inversion &= 0x01;
                printf("config inversion: %d\r\n", config.inversion);
                config_save();
                TL_SCHEDULE_TASK(epd_forceScreenUpdate, NULL);
                break;
            case 4:
#if UART_PRINTF_MODE && DEBUG_BUTTON
                printf("Celsius or Fahrenheit\r\n");
#endif
                config.d_mode = ~config.d_mode;
                config.d_mode &= 0x01;
                config_save();
                printf("config d_mode: %d\r\n", config.d_mode);
                zcl_setAttrVal(APP_ENDPOINT1, ZCL_CLUSTER_HAVC_USER_INTERFACE_CONFIG, ZCL_ATTRID_HVAC_TEMPERATURE_DISPLAY_MODE, (uint8_t*)&config.d_mode);
                epd_update_temperature_display_mode();
                break;
            case 5:
#if UART_PRINTF_MODE && DEBUG_BUTTON
                printf("Find and bind on target start\r\n");
#endif
                if (!g_appCtx.bdbFindBindFlg) {
                    g_appCtx.bdbFindBindFlg = TRUE;
                    TL_ZB_TIMER_SCHEDULE(app_bdbFindAndBindStart, NULL, 1000);
                }
                break;
            case 6:
#if UART_PRINTF_MODE && DEBUG_BUTTON
                printf("Reboot monitor\r\n");
#endif
                TL_ZB_TIMER_SCHEDULE(delayedMcuResetCb, NULL, TIMEOUT_100MS);
                break;
            default:
                printf("default button press %d times\r\n", button->counter);
                break;
        }
        button->counter = 0;
        button->pressed = false;
        button->released = false;
        button->hold = false;
    }
}


uint8_t button_idle() {

    if ((button->debounce != 1 && button->debounce != DEBOUNCE_BUTTON) || button->counter || button->pressed) return true;

    return false;
}

