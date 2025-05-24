#include "tl_common.h"
#include "zcl_include.h"

#include "app_main.h"

bool factory_reset = false;

static int32_t net_steer_start_offCb(void *args) {

    g_appCtx.net_steer_start = false;
    factory_reset = false;

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

static int32_t forcedReportCb(void *arg) {

    if(zb_isDeviceJoinedNwk()) {
        epInfo_t dstEpInfo;
        TL_SETSTRUCTCONTENT(dstEpInfo, 0);

        dstEpInfo.profileId = HA_PROFILE_ID;
#if FIND_AND_BIND_SUPPORT
        dstEpInfo.dstAddrMode = APS_DSTADDR_EP_NOTPRESETNT;
#else
        dstEpInfo.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
        dstEpInfo.dstEp = APP_ENDPOINT1;
        dstEpInfo.dstAddr.shortAddr = 0xfffc;
#endif

        zclAttrInfo_t *pAttrEntry;

        pAttrEntry = zcl_findAttribute(APP_ENDPOINT1,
                                       ZCL_CLUSTER_MS_CO2_MEASUREMENT,
                                       ZCL_CO2_MEASUREMENT_ATTRID_MEASUREDVALUE);
        if (pAttrEntry)
            zcl_sendReportCmd(APP_ENDPOINT1, &dstEpInfo,  TRUE, ZCL_FRAME_SERVER_CLIENT_DIR,
                              ZCL_CLUSTER_MS_CO2_MEASUREMENT, pAttrEntry->id, pAttrEntry->type, pAttrEntry->data);

        pAttrEntry = zcl_findAttribute(APP_ENDPOINT1,
                                       ZCL_CLUSTER_MS_CO2_MEASUREMENT,
                                       ZCL_ATTRID_CMS_CUSTOM_CO2_ONOFF);
        if (pAttrEntry)
            zcl_sendReportCmd(APP_ENDPOINT1, &dstEpInfo,  TRUE, ZCL_FRAME_SERVER_CLIENT_DIR,
                              ZCL_CLUSTER_MS_CO2_MEASUREMENT, pAttrEntry->id, pAttrEntry->type, pAttrEntry->data);

        pAttrEntry = zcl_findAttribute(APP_ENDPOINT1,
                                       ZCL_CLUSTER_MS_CO2_MEASUREMENT,
                                       ZCL_ATTRID_CMS_CUSTOM_CO2_LOW);
        if (pAttrEntry)
            zcl_sendReportCmd(APP_ENDPOINT1, &dstEpInfo,  TRUE, ZCL_FRAME_SERVER_CLIENT_DIR,
                              ZCL_CLUSTER_MS_CO2_MEASUREMENT, pAttrEntry->id, pAttrEntry->type, pAttrEntry->data);

        pAttrEntry = zcl_findAttribute(APP_ENDPOINT1,
                                       ZCL_CLUSTER_MS_CO2_MEASUREMENT,
                                       ZCL_ATTRID_CMS_CUSTOM_CO2_HIGH);
        if (pAttrEntry)
            zcl_sendReportCmd(APP_ENDPOINT1, &dstEpInfo,  TRUE, ZCL_FRAME_SERVER_CLIENT_DIR,
                              ZCL_CLUSTER_MS_CO2_MEASUREMENT, pAttrEntry->id, pAttrEntry->type, pAttrEntry->data);

        pAttrEntry = zcl_findAttribute(APP_ENDPOINT1,
                                       ZCL_CLUSTER_MS_CO2_MEASUREMENT,
                                       ZCL_ATTRID_CMS_CUSTOM_CO2_FORCED_CALIBRATION);
        if (pAttrEntry)
            zcl_sendReportCmd(APP_ENDPOINT1, &dstEpInfo,  TRUE, ZCL_FRAME_SERVER_CLIENT_DIR,
                              ZCL_CLUSTER_MS_CO2_MEASUREMENT, pAttrEntry->id, pAttrEntry->type, pAttrEntry->data);

        pAttrEntry = zcl_findAttribute(APP_ENDPOINT1,
                                       ZCL_CLUSTER_MS_TEMPERATURE_MEASUREMENT,
                                       ZCL_TEMPERATURE_MEASUREMENT_ATTRID_MEASUREDVALUE);
        if (pAttrEntry)
            zcl_sendReportCmd(APP_ENDPOINT1, &dstEpInfo,  TRUE, ZCL_FRAME_SERVER_CLIENT_DIR,
                              ZCL_CLUSTER_MS_TEMPERATURE_MEASUREMENT, pAttrEntry->id, pAttrEntry->type, pAttrEntry->data);

        pAttrEntry = zcl_findAttribute(APP_ENDPOINT1,
                                       ZCL_CLUSTER_MS_TEMPERATURE_MEASUREMENT,
                                       ZCL_ATTRID_TMS_CUSTOM_TEMPERATURE_OFFSET);
        if (pAttrEntry)
            zcl_sendReportCmd(APP_ENDPOINT1, &dstEpInfo,  TRUE, ZCL_FRAME_SERVER_CLIENT_DIR,
                              ZCL_CLUSTER_MS_TEMPERATURE_MEASUREMENT, pAttrEntry->id, pAttrEntry->type, pAttrEntry->data);

        pAttrEntry = zcl_findAttribute(APP_ENDPOINT1,
                                       ZCL_CLUSTER_MS_TEMPERATURE_MEASUREMENT,
                                       ZCL_ATTRID_TMS_CUSTOM_READ_SENSORS_PERIOD);
        if (pAttrEntry)
            zcl_sendReportCmd(APP_ENDPOINT1, &dstEpInfo,  TRUE, ZCL_FRAME_SERVER_CLIENT_DIR,
                              ZCL_CLUSTER_MS_TEMPERATURE_MEASUREMENT, pAttrEntry->id, pAttrEntry->type, pAttrEntry->data);

        pAttrEntry = zcl_findAttribute(APP_ENDPOINT1,
                                       ZCL_CLUSTER_MS_RELATIVE_HUMIDITY,
                                       ZCL_ATTRID_HUMIDITY_MEASUREDVALUE);
        if (pAttrEntry)
            zcl_sendReportCmd(APP_ENDPOINT1, &dstEpInfo,  TRUE, ZCL_FRAME_SERVER_CLIENT_DIR,
                              ZCL_CLUSTER_MS_RELATIVE_HUMIDITY, pAttrEntry->id, pAttrEntry->type, pAttrEntry->data);

        pAttrEntry = zcl_findAttribute(APP_ENDPOINT1,
                                       ZCL_CLUSTER_MS_PRESSURE_MEASUREMENT,
                                       ZCL_ATTRID_PRESSURE_MEASUREDVALUE);
        if (pAttrEntry)
            zcl_sendReportCmd(APP_ENDPOINT1, &dstEpInfo,  TRUE, ZCL_FRAME_SERVER_CLIENT_DIR,
                              ZCL_CLUSTER_MS_PRESSURE_MEASUREMENT, pAttrEntry->id, pAttrEntry->type, pAttrEntry->data);

        pAttrEntry = zcl_findAttribute(APP_ENDPOINT1,
                                       ZCL_CLUSTER_MS_ILLUMINANCE_MEASUREMENT,
                                       ZCL_ATTRID_MEASURED_VALUE);
        if (pAttrEntry)
            zcl_sendReportCmd(APP_ENDPOINT1, &dstEpInfo,  TRUE, ZCL_FRAME_SERVER_CLIENT_DIR,
                              ZCL_CLUSTER_MS_ILLUMINANCE_MEASUREMENT, pAttrEntry->id, pAttrEntry->type, pAttrEntry->data);

        pAttrEntry = zcl_findAttribute(APP_ENDPOINT1,
                                       ZCL_CLUSTER_GEN_ANALOG_INPUT_BASIC,
                                       ZCL_ANALOG_INPUT_ATTRID_PRESENT_VALUE);
        if (pAttrEntry)
            zcl_sendReportCmd(APP_ENDPOINT1, &dstEpInfo,  TRUE, ZCL_FRAME_SERVER_CLIENT_DIR,
                              ZCL_CLUSTER_GEN_ANALOG_INPUT_BASIC, pAttrEntry->id, pAttrEntry->type, pAttrEntry->data);

        pAttrEntry = zcl_findAttribute(APP_ENDPOINT1,
                                       ZCL_CLUSTER_GEN_ANALOG_INPUT_BASIC,
                                       ZCL_ATTRID_AI_CUSTOM_VOC_ONOFF);
        if (pAttrEntry)
            zcl_sendReportCmd(APP_ENDPOINT1, &dstEpInfo,  TRUE, ZCL_FRAME_SERVER_CLIENT_DIR,
                              ZCL_CLUSTER_GEN_ANALOG_INPUT_BASIC, pAttrEntry->id, pAttrEntry->type, pAttrEntry->data);

        pAttrEntry = zcl_findAttribute(APP_ENDPOINT1,
                                       ZCL_CLUSTER_GEN_ANALOG_INPUT_BASIC,
                                       ZCL_ATTRID_AI_CUSTOM_VOC_LOW);
        if (pAttrEntry)
            zcl_sendReportCmd(APP_ENDPOINT1, &dstEpInfo,  TRUE, ZCL_FRAME_SERVER_CLIENT_DIR,
                              ZCL_CLUSTER_GEN_ANALOG_INPUT_BASIC, pAttrEntry->id, pAttrEntry->type, pAttrEntry->data);

        pAttrEntry = zcl_findAttribute(APP_ENDPOINT1,
                                       ZCL_CLUSTER_GEN_ANALOG_INPUT_BASIC,
                                       ZCL_ATTRID_AI_CUSTOM_VOC_HIGH);
        if (pAttrEntry)
            zcl_sendReportCmd(APP_ENDPOINT1, &dstEpInfo,  TRUE, ZCL_FRAME_SERVER_CLIENT_DIR,
                              ZCL_CLUSTER_GEN_ANALOG_INPUT_BASIC, pAttrEntry->id, pAttrEntry->type, pAttrEntry->data);

        pAttrEntry = zcl_findAttribute(APP_ENDPOINT1,
                                       ZCL_CLUSTER_HAVC_USER_INTERFACE_CONFIG,
                                       ZCL_ATTRID_HVAC_TEMPERATURE_DISPLAY_MODE);
        if (pAttrEntry)
            zcl_sendReportCmd(APP_ENDPOINT1, &dstEpInfo,  TRUE, ZCL_FRAME_SERVER_CLIENT_DIR,
                              ZCL_CLUSTER_HAVC_USER_INTERFACE_CONFIG, pAttrEntry->id, pAttrEntry->type, pAttrEntry->data);

        pAttrEntry = zcl_findAttribute(APP_ENDPOINT1,
                                       ZCL_CLUSTER_HAVC_USER_INTERFACE_CONFIG,
                                       ZCL_ATTRID_HVAC_CUSTOM_DISPLAY_ROTATE);
        if (pAttrEntry)
            zcl_sendReportCmd(APP_ENDPOINT1, &dstEpInfo,  TRUE, ZCL_FRAME_SERVER_CLIENT_DIR,
                              ZCL_CLUSTER_HAVC_USER_INTERFACE_CONFIG, pAttrEntry->id, pAttrEntry->type, pAttrEntry->data);

        pAttrEntry = zcl_findAttribute(APP_ENDPOINT1,
                                       ZCL_CLUSTER_HAVC_USER_INTERFACE_CONFIG,
                                       ZCL_ATTRID_HVAC_CUSTOM_DISPLAY_INVERSION);
        if (pAttrEntry)
            zcl_sendReportCmd(APP_ENDPOINT1, &dstEpInfo,  TRUE, ZCL_FRAME_SERVER_CLIENT_DIR,
                              ZCL_CLUSTER_HAVC_USER_INTERFACE_CONFIG, pAttrEntry->id, pAttrEntry->type, pAttrEntry->data);

        pAttrEntry = zcl_findAttribute(APP_ENDPOINT1,
                                       ZCL_CLUSTER_GEN_ON_OFF_SWITCH_CONFIG,
                                       ZCL_ATTRID_SWITCH_ACTION);
        if (pAttrEntry)
            zcl_sendReportCmd(APP_ENDPOINT1, &dstEpInfo,  TRUE, ZCL_FRAME_SERVER_CLIENT_DIR,
                              ZCL_CLUSTER_GEN_ON_OFF_SWITCH_CONFIG, pAttrEntry->id, pAttrEntry->type, pAttrEntry->data);

        pAttrEntry = zcl_findAttribute(APP_ENDPOINT1,
                                       ZCL_CLUSTER_GEN_LEVEL_CONTROL,
                                       ZCL_ATTRID_LEVEL_CURRENT_LEVEL);
        if (pAttrEntry)
            zcl_sendReportCmd(APP_ENDPOINT1, &dstEpInfo,  TRUE, ZCL_FRAME_SERVER_CLIENT_DIR,
                              ZCL_CLUSTER_GEN_LEVEL_CONTROL, pAttrEntry->id, pAttrEntry->type, pAttrEntry->data);
    }

    return -1;
}

static void buttonKeepPressed(uint8_t btNum) {
    g_appCtx.button[btNum-1].state = APP_FACTORY_NEW_DOING;
    g_appCtx.button[btNum-1].ctn = 0;

    if(btNum == VK_SW1) {
#if UART_PRINTF_MODE && DEBUG_BUTTON
        printf("The button was keep pressed for 5 seconds\r\n");
#endif

        if (zb_getLocalShortAddr() >= 0xFFF8) {
//            zb_deviceFactoryNewSet(true);
            printf("1 Factory new: %s\r\n", zb_isDeviceFactoryNew()?"yes":"no");
            if (!factory_reset) {
                factory_reset = true;
                zb_resetDevice2FN();
                printf("1.2 Factory new: %s\r\n", zb_isDeviceFactoryNew()?"yes":"no");
            }
        } else {
            zb_resetDevice2FN();
            sleep_ms(200);
//            zb_deviceFactoryNewSet(true);
            if (g_appCtx.timerFactoryReset) {
                TL_ZB_TIMER_CANCEL(&g_appCtx.timerFactoryReset);
            }
            g_appCtx.timerFactoryReset = TL_ZB_TIMER_SCHEDULE(delayedFactoryResetCb, NULL, TIMEOUT_5SEC);
            printf("2 Factory new: %s\r\n", zb_isDeviceFactoryNew()?"yes":"no");
        }

        g_appCtx.net_steer_start = true;
        led_off();
        epd_clearZbIcon();
        TL_ZB_TIMER_SCHEDULE(net_steer_start_offCb, NULL, TIMEOUT_1MIN30SEC);
        led_effect_start(55, COLOR_RED);
//        TL_ZB_TIMER_SCHEDULE(factory_resetCb, NULL, TIMEOUT_6SEC);
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
        zb_resetDevice();
    } else if (g_appCtx.button[btNum-1].ctn == 3) {
#if UART_PRINTF_MODE && DEBUG_BUTTON
        printf("Button push 3 times. Inversion\r\n");
#endif
        config.inversion = ~config.inversion;
        config.inversion &= 0x01;
        printf("config inversion: %d\r\n", config.inversion);
        config_save();
        zb_resetDevice();
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
