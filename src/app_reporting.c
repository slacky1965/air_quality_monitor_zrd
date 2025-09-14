#include "app_main.h"

int32_t forcedReportCb(void *arg) {

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

        pAttrEntry = zcl_findAttribute(APP_ENDPOINT1,
                                       ZCL_CLUSTER_GEN_TIME,
                                       ZCL_ATTRID_TIME);
        if (pAttrEntry)
            zcl_sendReportCmd(APP_ENDPOINT1, &dstEpInfo,  TRUE, ZCL_FRAME_SERVER_CLIENT_DIR,
                               ZCL_CLUSTER_GEN_TIME, pAttrEntry->id, pAttrEntry->type, pAttrEntry->data);
    }

    return -1;
}
