#ifndef SRC_INCLUDE_APP_MAIN_H_
#define SRC_INCLUDE_APP_MAIN_H_

#include "tl_common.h"
#include "zcl_include.h"
#include "bdb.h"
#include "ota.h"
#include "gp.h"

#include "app_led.h"
#include "zcl_concentration_measurement.h"
#include "zcl_relative_humidity_measurement.h"
#include "zcl_pressure_measurement.h"
#include "app_lqi.h"
#include "app_button.h"
#include "app_utility.h"
#include "app_endpoint_cfg.h"
#include "app_epd.h"
#include "app_sensors.h"
#include "app_i2c.h"
#include "app_bme280.h"
#include "app_bh1750.h"
#include "app_scd4x.h"
#include "app_ds3231.h"
#include "app_config.h"
#include "app_time.h"

typedef struct {
    uint8_t keyType; /* CERTIFICATION_KEY or MASTER_KEY key for touch-link or distribute network
                        SS_UNIQUE_LINK_KEY or SS_GLOBAL_LINK_KEY for distribute network */
    uint8_t key[16]; /* the key used */
} app_linkKey_info_t;

typedef struct {
    ev_timer_event_t *bdbFBTimerEvt;
    ev_timer_event_t *timerFactoryReset;
    ev_timer_event_t *timerMesurementEvt;
//    ev_timer_event_t *timerForcedReportEvt;
//    ev_timer_event_t *timerStopReportEvt;
//    ev_timer_event_t *timerPollRateEvt;
//    ev_timer_event_t *timerBatteryEvt;
//    ev_timer_event_t *timerLedEvt;
//    ev_timer_event_t *timerNoJoinedEvt;

    uint32_t short_poll;
    uint32_t long_poll;
    uint32_t current_poll;

    uint32_t timeout;

    button_t button[MAX_BUTTON_NUM];
    u8  keyPressed;

    //    u32 keyPressedTime;
//    u8  keyPressed;

//    uint8_t status_onoff1;
//    uint8_t status_onoff2;

//    uint16_t ledOnTime;
//    uint16_t ledOffTime;
//    uint8_t  oriSta;     //original state before blink
//    uint8_t  sta;        //current state in blink
//    uint8_t  times;      //blink times
//    uint8_t  state;

//    uint32_t time_without_joined;

    app_linkKey_info_t tcLinkKey;
} app_ctx_t;

extern app_ctx_t g_appCtx;

extern bdb_appCb_t g_zbBdbCb;

extern bdb_commissionSetting_t g_bdbCommissionSetting;

extern const zcl_specClusterInfo_t g_appEp1ClusterList[];
extern const zcl_specClusterInfo_t g_appEp2ClusterList[];
extern const af_simple_descriptor_t app_ep1Desc;
extern const af_simple_descriptor_t app_ep2Desc;

void app_zclProcessIncomingMsg(zclIncoming_t *pInHdlrMsg);

status_t app_basicCb(zclIncomingAddrInfo_t *pAddrInfo, uint8_t cmdId, void *cmdPayload);
status_t app_identifyCb(zclIncomingAddrInfo_t *pAddrInfo, uint8_t cmdId, void *cmdPayload);
status_t app_powerCfgCb(zclIncomingAddrInfo_t *pAddrInfo, uint8_t cmdId, void *cmdPayload);
status_t app_groupCb(zclIncomingAddrInfo_t *pAddrInfo, uint8_t cmdId, void *cmdPayload);
status_t app_sceneCb(zclIncomingAddrInfo_t *pAddrInfo, uint8_t cmdId, void *cmdPayload);
status_t app_pollCtrlCb(zclIncomingAddrInfo_t *pAddrInfo, uint8_t cmdId, void *cmdPayload);
status_t app_co2Cb(zclIncomingAddrInfo_t *pAddrInfo, uint8_t cmdId, void *cmdPayload);
status_t app_temperatureCb(zclIncomingAddrInfo_t *pAddrInfo, uint8_t cmdId, void *cmdPayload);
status_t app_humidityCb(zclIncomingAddrInfo_t *pAddrInfo, uint8_t cmdId, void *cmdPayload);
status_t app_pressureCb(zclIncomingAddrInfo_t *pAddrInfo, uint8_t cmdId, void *cmdPayload);
status_t app_timeCb(zclIncomingAddrInfo_t *pAddrInfo, uint8_t cmdId, void *cmdPayload);
status_t app_diagnosticsCb(zclIncomingAddrInfo_t *pAddrInfo, uint8_t cmdId, void *cmdPayload);
status_t app_illuminanceCb(zclIncomingAddrInfo_t *pAddrInfo, uint8_t cmdId, void *cmdPayload);

#define zcl_scene1AttrGet()         &g_zcl_scene1Attrs
#define zcl_scene2AttrGet()         &g_zcl_scene2Attrs
#define zcl_co2AttrGet()            &g_zcl_co2Attrs
#define zcl_temperatureAttrGet()    &g_zcl_temperatureAttrs
#define zcl_humidityAttrGet()       &g_zcl_humidityAttrs
#define zcl_pressureAttrGet()       &g_zcl_pressureAttrs
#define zcl_illuminanceAttrGet()    &g_zcl_illuminanceAttrs

void app_leaveCnfHandler(nlme_leave_cnf_t *pLeaveCnf);
void app_leaveIndHandler(nlme_leave_ind_t *pLeaveInd);
void app_otaProcessMsgHandler(uint8_t evt, uint8_t status);
bool app_nwkUpdateIndicateHandler(nwkCmd_nwkUpdate_t *pNwkUpdate);
void app_wakeupPinLevelChange();

extern void test_epd();

#endif /* SRC_INCLUDE_APP_MAIN_H_ */
