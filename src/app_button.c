#include "tl_common.h"
#include "zcl_include.h"

#include "app_main.h"

bool factory_reset = false;

static int32_t net_steer_start_offCb(void *args) {

    g_appCtx.net_steer_start = false;
    factory_reset = false;
    led_blink_stop();

    return -1;
}

//static int32_t factory_resetCb(void *args) {
//
//    g_appCtx.net_steer_start = true;
//    TL_ZB_TIMER_SCHEDULE(net_steer_start_offCb, NULL, TIMEOUT_1MIN30SEC);
//    led_effect_start(55, COLOR_RED);
//
//    return -1;
//}


static void buttonKeepPressed(uint8_t btNum) {
    g_appCtx.button[btNum-1].state = APP_FACTORY_NEW_DOING;
    g_appCtx.button[btNum-1].ctn = 0;

    if(btNum == VK_SW1) {
#if UART_PRINTF_MODE && DEBUG_BUTTON
        printf("The button was keep pressed for 5 seconds\r\n");
#endif

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

static void buttonCheckCommand(uint8_t btNum) {
    g_appCtx.button[btNum-1].state = APP_STATE_NORMAL;

    if (g_appCtx.button[btNum-1].ctn == 1) {
#if UART_PRINTF_MODE && DEBUG_BUTTON
        printf("Button push 1 time\r\n");
#endif
        TL_ZB_TIMER_SCHEDULE(forcedReportCb, NULL, TIMEOUT_100MS);
    } else if (g_appCtx.button[btNum-1].ctn == 2) {
#if UART_PRINTF_MODE && DEBUG_BUTTON
        printf("Button push 2 times. Rotate\r\n");
#endif
        config.rotate = ~config.rotate;
        config.rotate &= 0x01;
        printf("config rotate: %d\r\n", config.rotate);
        config_save();
        TL_SCHEDULE_TASK(epd_forceScreenUpdate, NULL);
    } else if (g_appCtx.button[btNum-1].ctn == 3) {
#if UART_PRINTF_MODE && DEBUG_BUTTON
        printf("Button push 3 times. Inversion\r\n");
#endif
        config.inversion = ~config.inversion;
        config.inversion &= 0x01;
        printf("config inversion: %d\r\n", config.inversion);
        config_save();
        TL_SCHEDULE_TASK(epd_forceScreenUpdate, NULL);
    } else if (g_appCtx.button[btNum-1].ctn == 4) {
#if UART_PRINTF_MODE && DEBUG_BUTTON
        printf("Button push 4 times. Celsius or Fahrenheit\r\n");
#endif
        config.d_mode = ~config.d_mode;
        config.d_mode &= 0x01;
        config_save();
        printf("config d_mode: %d\r\n", config.d_mode);
        zcl_setAttrVal(APP_ENDPOINT1, ZCL_CLUSTER_HAVC_USER_INTERFACE_CONFIG, ZCL_ATTRID_HVAC_TEMPERATURE_DISPLAY_MODE, (uint8_t*)&config.d_mode);
        epd_update_temperature_display_mode();
    } else if (g_appCtx.button[btNum-1].ctn == 5) {
#if UART_PRINTF_MODE && DEBUG_BUTTON
        printf("Button push 5 times. Reboot monitor\r\n");
#endif
        TL_ZB_TIMER_SCHEDULE(delayedMcuResetCb, NULL, TIMEOUT_100MS);
     }

    g_appCtx.button[btNum-1].ctn = 0;
}


void keyScan_keyPressedCB(kb_data_t *kbEvt) {

    uint8_t keyCode = kbEvt->keycode[0];

    if(keyCode != 0xff) {
        g_appCtx.button[keyCode-1].pressed_time = clock_time();
        g_appCtx.button[keyCode-1].state = APP_FACTORY_NEW_SET_CHECK;
        g_appCtx.button[keyCode-1].ctn++;
        led_blink_start(1, 30, 1, COLOR_RED);
    }
}


void keyScan_keyReleasedCB(uint8_t keyCode) {

    if (keyCode != 0xff) {
        g_appCtx.button[keyCode-1].released_time = clock_time();
        g_appCtx.button[keyCode-1].state = APP_STATE_RELEASE;
    }
}

void button_handler(void) {
    static uint8_t valid_keyCode = 0xff;

    for (uint8_t i = 0; i < MAX_BUTTON_NUM; i++) {
        if (g_appCtx.button[i].state == APP_FACTORY_NEW_SET_CHECK) {
            if(clock_time_exceed(g_appCtx.button[i].pressed_time, TIMEOUT_TICK_5SEC)) {
                buttonKeepPressed(i+1);
            }
        }

        if (g_appCtx.button[i].state == APP_STATE_RELEASE) {
            if(clock_time_exceed(g_appCtx.button[i].released_time, TIMEOUT_TICK_250MS)) {
                buttonCheckCommand(i+1);
            }

        }
    }

    if(kb_scan_key(0, 1)){
        if(kb_event.cnt){
            g_appCtx.keyPressed = 1;
            keyScan_keyPressedCB(&kb_event);
            if(kb_event.cnt == 1){
                valid_keyCode = kb_event.keycode[0];
            }
        }else{
            keyScan_keyReleasedCB(valid_keyCode);
            valid_keyCode = 0xff;
            g_appCtx.keyPressed = 0;
        }
    }
}

uint8_t button_idle() {

    if (g_appCtx.keyPressed) {
        return true;
    }

    for (uint8_t i = 0; i < MAX_BUTTON_NUM; i++) {
        if (g_appCtx.button[i].ctn) return true;
    }

    return false;
}
