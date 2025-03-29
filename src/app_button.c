#include "tl_common.h"
#include "zcl_include.h"

#include "app_main.h"

static void buttonKeepPressed(uint8_t btNum) {
    g_appCtx.button[btNum-1].state = APP_FACTORY_NEW_DOING;
    g_appCtx.button[btNum-1].ctn = 0;

    if(btNum == VK_SW1) {
#if UART_PRINTF_MODE && DEBUG_BUTTON
        printf("The button was keep pressed for 5 seconds\r\n");
#endif
//        light_blink_stop();
//        light_blink_start(1, 1500, 1);

        if(!zb_isDeviceJoinedNwk()) {
            if (g_appCtx.timerFactoryReset) {
                TL_ZB_TIMER_CANCEL(&g_appCtx.timerFactoryReset);
                zb_resetDevice2FN();
            }
            return;
        }

        zb_resetDevice2FN();
        if (g_appCtx.timerFactoryReset) {
            TL_ZB_TIMER_CANCEL(&g_appCtx.timerFactoryReset);
        }
        g_appCtx.timerFactoryReset = TL_ZB_TIMER_SCHEDULE(delayedFactoryResetCb, NULL, TIMEOUT_5SEC);
    }
}

static void buttonCheckCommand(uint8_t btNum) {
    g_appCtx.button[btNum-1].state = APP_STATE_NORMAL;

    if (g_appCtx.button[btNum-1].ctn == 1) {
#if UART_PRINTF_MODE && DEBUG_BUTTON
        printf("Button push 1 time\r\n");
#endif
    }

    g_appCtx.button[btNum-1].ctn = 0;
}


void keyScan_keyPressedCB(kb_data_t *kbEvt) {

//    uint16_t len;
//    epInfo_t dstEpInfo;
//    zoneStatusChangeNoti_t statusChangeNotification;
    uint8_t keyCode = kbEvt->keycode[0];

    if(keyCode != 0xff) {
        g_appCtx.button[keyCode-1].pressed_time = clock_time();
        g_appCtx.button[keyCode-1].state = APP_FACTORY_NEW_SET_CHECK;
        g_appCtx.button[keyCode-1].ctn++;
//        light_blink_start(1, 100, 1);

//        if (keyCode == VK_SW1) {
//            if(zb_isDeviceJoinedNwk()) {
//                fillIASAddress(&dstEpInfo);
//
//                zcl_getAttrVal(APP_ENDPOINT1, ZCL_CLUSTER_SS_IAS_ZONE, ZCL_ATTRID_ZONE_STATUS, &len, (uint8_t*)&statusChangeNotification.zoneStatus);
//                zcl_getAttrVal(APP_ENDPOINT1, ZCL_CLUSTER_SS_IAS_ZONE, ZCL_ATTRID_ZONE_ID, &len, &statusChangeNotification.zoneId);
//
//                statusChangeNotification.zoneStatus |= ZONE_STATUS_TEST;
//                zcl_setAttrVal(APP_ENDPOINT1, ZCL_CLUSTER_SS_IAS_ZONE, ZCL_ATTRID_ZONE_STATUS, (uint8_t*)&statusChangeNotification.zoneStatus);
//                statusChangeNotification.extStatus = 0;
//                statusChangeNotification.delay = 0;
//
//                zcl_iasZone_statusChangeNotificationCmd(APP_ENDPOINT1, &dstEpInfo, TRUE, &statusChangeNotification);
//
//            }
//        }
    }
}


void keyScan_keyReleasedCB(uint8_t keyCode) {

//    uint16_t len;
//    epInfo_t dstEpInfo;
//    zoneStatusChangeNoti_t statusChangeNotification;

    if (keyCode != 0xff) {
        g_appCtx.button[keyCode-1].released_time = clock_time();
        g_appCtx.button[keyCode-1].state = APP_STATE_RELEASE;

//        if (keyCode == VK_SW1) {
//            if(zb_isDeviceJoinedNwk()) {
//                fillIASAddress(&dstEpInfo);
//
//                zcl_getAttrVal(APP_ENDPOINT1, ZCL_CLUSTER_SS_IAS_ZONE, ZCL_ATTRID_ZONE_STATUS, &len, (uint8_t*)&statusChangeNotification.zoneStatus);
//                zcl_getAttrVal(APP_ENDPOINT1, ZCL_CLUSTER_SS_IAS_ZONE, ZCL_ATTRID_ZONE_ID, &len, &statusChangeNotification.zoneId);
//
//                statusChangeNotification.zoneStatus &= ~ZONE_STATUS_TEST;
//                zcl_setAttrVal(APP_ENDPOINT1, ZCL_CLUSTER_SS_IAS_ZONE, ZCL_ATTRID_ZONE_STATUS, (uint8_t*)&statusChangeNotification.zoneStatus);
//                statusChangeNotification.extStatus = 0;
//                statusChangeNotification.delay = 0;
//
//                zcl_iasZone_statusChangeNotificationCmd(APP_ENDPOINT1, &dstEpInfo, TRUE, &statusChangeNotification);
//
//            }
//        }

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
