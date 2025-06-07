#include "app_main.h"

//static uint32_t last_light = 0;

app_ctx_t g_appCtx = {
        .bdbFBTimerEvt = NULL,
        .timerFactoryReset = NULL,
        .timerMesurementEvt = NULL,
        .short_poll = POLL_RATE * 3,
        .long_poll = POLL_RATE * LONG_POLL,
        .co2_forced_calibration = 0,
        .co2_factory_reset = 0,
        .net_steer_start = false,
};

//uint32_t count_restart = 0;

#ifdef ZCL_OTA
extern ota_callBack_t app_otaCb;

//running code firmware information
ota_preamble_t app_otaInfo = {
    .fileVer            = FILE_VERSION,
    .imageType          = IMAGE_TYPE,
    .manufacturerCode   = MANUFACTURER_CODE_TELINK,
};
#endif


//Must declare the application call back function which used by ZDO layer
const zdo_appIndCb_t appCbLst = {
    bdb_zdoStartDevCnf,//start device cnf cb
    NULL,//reset cnf cb
    NULL,//device announce indication cb
    app_leaveIndHandler,//leave ind cb
    app_leaveCnfHandler,//leave cnf cb
    app_nwkUpdateIndicateHandler,//nwk update ind cb
    NULL,//permit join ind cb
    NULL,//nlme sync cnf cb
    NULL,//tc join ind cb
    NULL,//tc detects that the frame counter is near limit
};

/**
 *  @brief Definition for BDB finding and binding cluster
 */
uint16_t bdb_findBindClusterList[] =
{
    ZCL_CLUSTER_GEN_ON_OFF,
};

/**
 *  @brief Definition for BDB finding and binding cluster number
 */
#define FIND_AND_BIND_CLUSTER_NUM       (sizeof(bdb_findBindClusterList)/sizeof(bdb_findBindClusterList[0]))

/**
 *  @brief Definition for bdb commissioning setting
 */
bdb_commissionSetting_t g_bdbCommissionSetting = {
    .linkKey.tcLinkKey.keyType = SS_GLOBAL_LINK_KEY,
    .linkKey.tcLinkKey.key = (uint8_t *)tcLinkKeyCentralDefault,             //can use unique link key stored in NV

    .linkKey.distributeLinkKey.keyType = MASTER_KEY,
    .linkKey.distributeLinkKey.key = (uint8_t *)linkKeyDistributedMaster,    //use linkKeyDistributedCertification before testing

    .linkKey.touchLinkKey.keyType = MASTER_KEY,
    .linkKey.touchLinkKey.key = (uint8_t *)touchLinkKeyMaster,               //use touchLinkKeyCertification before testing

#if TOUCHLINK_SUPPORT
    .touchlinkEnable = 1,                                               /* enable touch-link */
#else
    .touchlinkEnable = 0,                                               /* disable touch-link */
#endif
    .touchlinkChannel = DEFAULT_CHANNEL,                                /* touch-link default operation channel for target */
    .touchlinkLqiThreshold = 0xA0,                                      /* threshold for touch-link scan req/resp command */
};

/**********************************************************************
 * LOCAL VARIABLES
 */


/**********************************************************************
 * FUNCTIONS
 */


/*********************************************************************
 * @fn      stack_init
 *
 * @brief   This function initialize the ZigBee stack and related profile. If HA/ZLL profile is
 *          enabled in this application, related cluster should be registered here.
 *
 * @param   None
 *
 * @return  None
 */
void stack_init(void)
{
    /* Initialize ZB stack */
    zb_init();

    /* Register stack CB */
    zb_zdoCbRegister((zdo_appIndCb_t *)&appCbLst);
}

/*********************************************************************
 * @fn      user_app_init
 *
 * @brief   This function initialize the application(Endpoint) information for this node.
 *
 * @param   None
 *
 * @return  None
 */
void user_app_init(void)
{

#if ZCL_POLL_CTRL_SUPPORT
    af_powerDescPowerModeUpdate(POWER_MODE_RECEIVER_COMES_PERIODICALLY);
#else
    af_powerDescPowerModeUpdate(POWER_MODE_RECEIVER_COMES_WHEN_STIMULATED);
#endif

    /* Initialize ZCL layer */
    /* Register Incoming ZCL Foundation command/response messages */
    zcl_init(app_zclProcessIncomingMsg);

    /* register endPoint */
    af_endpointRegister(APP_ENDPOINT1, (af_simple_descriptor_t *)&app_ep1Desc, zcl_rx_handler, NULL);

    zcl_reportingTabInit();
    zcl_onOffCfgAttr_restore();
    config_restore();
    led_init();

    /* Register ZCL specific cluster information */
    zcl_register(APP_ENDPOINT1, APP_EP1_CB_CLUSTER_NUM, (zcl_specClusterInfo_t *)g_appEp1ClusterList);

#if ZCL_GP_SUPPORT
    /* Initialize GP */
    gp_init(APP_ENDPOINT1);
#endif

#if ZCL_OTA_SUPPORT
    ota_init(OTA_TYPE_CLIENT, (af_simple_descriptor_t *)&app_ep1Desc, &app_otaInfo, &app_otaCb);
#endif

    app_epd_init();

    app_i2c_init();

    app_ds3231_init();

    TL_ZB_TIMER_SCHEDULE(app_time_cmdCb, NULL, TIMEOUT_10SEC);

    app_first_start_epd();

    app_bme280_init();
    app_bme280_measurement();
    app_bh1750_init();
    app_scd4x_init();
    app_sgp40_init();

    TL_ZB_TIMER_SCHEDULE(app_lqiCb, NULL, TIMEOUT_250MS);
    g_appCtx.timerMesurementEvt = TL_ZB_TIMER_SCHEDULE(app_mesurementCb, NULL, TIMEOUT_100MS);
}

void app_task(void) {

    button_handler();

    if(BDB_STATE_GET() == BDB_STATE_IDLE){
//    if(bdb_isIdle()) {
        report_handler();
    }
}

extern volatile uint16_t T_evtExcept[4];

static void appSysException(void) {

#if UART_PRINTF_MODE
    printf("app_sysException, line: %d, event: %d, reset\r\n", T_evtExcept[0], T_evtExcept[1]);
#endif

#if 1
    SYSTEM_RESET();
#else
    led_on(LED_STATUS);
    while(1);
#endif
}

/*********************************************************************
 * @fn      user_init
 *
 * @brief   User level initialization code.
 *
 * @param   isRetention - if it is waking up with ram retention.
 *
 * @return  None
 */
void user_init(bool isRetention) {

    (void)isRetention;

    start_message();

    /* Initialize Stack */
    stack_init();

    /* Initialize user application */
    user_app_init();

    /* Register except handler for test */
    sys_exceptHandlerRegister(appSysException);


    /* User's Task */
#if ZBHCI_EN
    zbhciInit();
    ev_on_poll(EV_POLL_HCI, zbhciTask);
#endif
    ev_on_poll(EV_POLL_IDLE, app_task);

    /* Read the pre-install code from NV */
    if(bdb_preInstallCodeLoad(&g_appCtx.tcLinkKey.keyType, g_appCtx.tcLinkKey.key) == RET_OK){
        g_bdbCommissionSetting.linkKey.tcLinkKey.keyType = g_appCtx.tcLinkKey.keyType;
        g_bdbCommissionSetting.linkKey.tcLinkKey.key = g_appCtx.tcLinkKey.key;
    }

    bdb_findBindMatchClusterSet(FIND_AND_BIND_CLUSTER_NUM, bdb_findBindClusterList);

    /* Set default reporting configuration */
    bdb_defaultReportingCfg(APP_ENDPOINT1,
                            HA_PROFILE_ID,
                            ZCL_CLUSTER_MS_CO2_MEASUREMENT,
                            ZCL_CO2_MEASUREMENT_ATTRID_MEASUREDVALUE,
                            config.reporting_co2.minInterval,
                            config.reporting_co2.maxInterval,
                            (uint8_t *)&config.reporting_co2.reportableChange.reportableChange_float);
    bdb_defaultReportingCfg(APP_ENDPOINT1,
                            HA_PROFILE_ID,
                            ZCL_CLUSTER_GEN_ANALOG_INPUT_BASIC,
                            ZCL_ANALOG_INPUT_ATTRID_PRESENT_VALUE,
                            config.reporting_voc.minInterval,
                            config.reporting_voc.maxInterval,
                            (uint8_t *)&config.reporting_voc.reportableChange.reportableChange_float);
    bdb_defaultReportingCfg(APP_ENDPOINT1,
                            HA_PROFILE_ID,
                            ZCL_CLUSTER_MS_TEMPERATURE_MEASUREMENT,
                            ZCL_TEMPERATURE_MEASUREMENT_ATTRID_MEASUREDVALUE,
                            config.reporting_temp.minInterval,
                            config.reporting_temp.maxInterval,
                            (uint8_t *)&config.reporting_temp.reportableChange.reportableChange_u16);
    bdb_defaultReportingCfg(APP_ENDPOINT1,
                            HA_PROFILE_ID,
                            ZCL_CLUSTER_MS_RELATIVE_HUMIDITY,
                            ZCL_ATTRID_HUMIDITY_MEASUREDVALUE,
                            config.reporting_hum.minInterval,
                            config.reporting_hum.maxInterval,
                            (uint8_t *)&config.reporting_hum.reportableChange.reportableChange_u16);
    bdb_defaultReportingCfg(APP_ENDPOINT1,
                            HA_PROFILE_ID,
                            ZCL_CLUSTER_MS_PRESSURE_MEASUREMENT,
                            ZCL_ATTRID_PRESSURE_MEASUREDVALUE,
                            config.reporting_press.minInterval,
                            config.reporting_press.maxInterval,
                            (uint8_t *)&config.reporting_press.reportableChange.reportableChange_u16);
    bdb_defaultReportingCfg(APP_ENDPOINT1,
                            HA_PROFILE_ID,
                            ZCL_CLUSTER_MS_ILLUMINANCE_MEASUREMENT,
                            ZCL_ATTRID_MEASURED_VALUE,
                            config.reporting_illum.minInterval,
                            config.reporting_illum.maxInterval,
                            (uint8_t *)&config.reporting_illum.reportableChange.reportableChange_u16);
    bdb_defaultReportingCfg(APP_ENDPOINT1,
                            HA_PROFILE_ID,
                            ZCL_CLUSTER_HAVC_USER_INTERFACE_CONFIG,
                            ZCL_ATTRID_HVAC_CUSTOM_DISPLAY_ROTATE,
                            config.reporting_rotate.minInterval,
                            config.reporting_rotate.maxInterval,
                            (uint8_t *)&config.reporting_rotate.reportableChange.reportableChange_u8);
    bdb_defaultReportingCfg(APP_ENDPOINT1,
                            HA_PROFILE_ID,
                            ZCL_CLUSTER_HAVC_USER_INTERFACE_CONFIG,
                            ZCL_ATTRID_HVAC_CUSTOM_DISPLAY_INVERSION,
                            config.reporting_inversion.minInterval,
                            config.reporting_inversion.maxInterval,
                            (uint8_t *)&config.reporting_inversion.reportableChange.reportableChange_u8);
    bdb_defaultReportingCfg(APP_ENDPOINT1,
                            HA_PROFILE_ID,
                            ZCL_CLUSTER_HAVC_USER_INTERFACE_CONFIG,
                            ZCL_ATTRID_HVAC_TEMPERATURE_DISPLAY_MODE,
                            config.reporting_dMode.minInterval,
                            config.reporting_dMode.maxInterval,
                            (uint8_t *)&config.reporting_dMode.reportableChange.reportableChange_u8);
    bdb_defaultReportingCfg(APP_ENDPOINT1,
                            HA_PROFILE_ID,
                            ZCL_CLUSTER_MS_CO2_MEASUREMENT,
                            ZCL_ATTRID_CMS_CUSTOM_CO2_FORCED_CALIBRATION,
                            config.reporting_co2Frc.minInterval,
                            config.reporting_co2Frc.maxInterval,
                            (uint8_t *)&config.reporting_co2Frc.reportableChange.reportableChange_u16);

    /* Initialize BDB */
    bdb_init((af_simple_descriptor_t *)&app_ep1Desc, &g_bdbCommissionSetting, &g_zbBdbCb, 1);

}
