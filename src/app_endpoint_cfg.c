#include "tl_common.h"
#include "zcl_include.h"

#include "app_main.h"

#ifndef ZCL_BASIC_MFG_NAME
#define ZCL_BASIC_MFG_NAME          {6,'T','E','L','I','N','K'}
#endif
#ifndef ZCL_BASIC_MODEL_ID
#define ZCL_BASIC_MODEL_ID          {8,'T','L','S','R','8','2','x','x'}
#endif
#ifndef ZCL_BASIC_SW_BUILD_ID
#define ZCL_BASIC_SW_BUILD_ID       {10,'0','1','2','2','0','5','2','0','1','7'}
#endif

#define R               ACCESS_CONTROL_READ
#define RW              ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE
#define RR              ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE
#define RRW             ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE | ACCESS_CONTROL_REPORTABLE

#define ZCL_UINT8       ZCL_DATA_TYPE_UINT8
#define ZCL_INT8        ZCL_DATA_TYPE_INT8
#define ZCL_UINT16      ZCL_DATA_TYPE_UINT16
#define ZCL_INT16       ZCL_DATA_TYPE_INT16
#define ZCL_UINT32      ZCL_DATA_TYPE_UINT32
#define ZCL_ENUM8       ZCL_DATA_TYPE_ENUM8
#define ZCL_BOOLEAN     ZCL_DATA_TYPE_BOOLEAN
#define ZCL_BITMAP8     ZCL_DATA_TYPE_BITMAP8
#define ZCL_BITMAP16    ZCL_DATA_TYPE_BITMAP16
#define ZCL_CHAR_STR    ZCL_DATA_TYPE_CHAR_STR
#define ZCL_UTC         ZCL_DATA_TYPE_UTC
#define ZCL_SINGLE      ZCL_DATA_TYPE_SINGLE_PREC

/**
 *  @brief Definition for Incoming cluster / Sever Cluster
 */
const uint16_t app_ep1_inClusterList[] = {
    ZCL_CLUSTER_GEN_BASIC,
    ZCL_CLUSTER_GEN_IDENTIFY,
#ifdef ZCL_GROUP
    ZCL_CLUSTER_GEN_GROUPS,
#endif
#ifdef ZCL_SCENE
    ZCL_CLUSTER_GEN_SCENES,
#endif
#ifdef ZCL_ON_OFF
    ZCL_CLUSTER_GEN_ON_OFF,
#endif
#ifdef ZCL_ZLL_COMMISSIONING
    ZCL_CLUSTER_TOUCHLINK_COMMISSIONING,
#endif
#ifdef ZCL_LEVEL_CTRL
    ZCL_CLUSTER_GEN_LEVEL_CONTROL,
#endif
#ifdef ZCL_CO2_MEASUREMENT
    ZCL_CLUSTER_MS_CO2_MEASUREMENT,
#endif
#ifdef ZCL_TEMPERATURE_MEASUREMENT
    ZCL_CLUSTER_MS_TEMPERATURE_MEASUREMENT,
#endif
    ZCL_CLUSTER_MS_RELATIVE_HUMIDITY,
#ifdef ZCL_ILLUMINANCE_MEASUREMENT
    ZCL_CLUSTER_MS_ILLUMINANCE_MEASUREMENT,
#endif
    ZCL_CLUSTER_MS_PRESSURE_MEASUREMENT,
};

/**
 *  @brief Definition for Outgoing cluster / Client Cluster
 */
const uint16_t app_ep1_outClusterList[] = {
    ZCL_CLUSTER_GEN_DIAGNOSTICS,
    ZCL_CLUSTER_GEN_TIME,
#ifdef ZCL_OTA
    ZCL_CLUSTER_OTA,
#endif
#ifdef ZCL_TEMPERATURE_MEASUREMENT
    ZCL_CLUSTER_MS_TEMPERATURE_MEASUREMENT,
#endif
    ZCL_CLUSTER_MS_RELATIVE_HUMIDITY,
    ZCL_CLUSTER_GEN_POWER_CFG,
};

/**
 *  @brief Definition for Server cluster number and Client cluster number
 */
#define APP_EP1_IN_CLUSTER_NUM   (sizeof(app_ep1_inClusterList)/sizeof(app_ep1_inClusterList[0]))
#define APP_EP1_OUT_CLUSTER_NUM  (sizeof(app_ep1_outClusterList)/sizeof(app_ep1_outClusterList[0]))

/**
 *  @brief Definition for simple description for HA profile
 */
const af_simple_descriptor_t app_ep1Desc = {
    HA_PROFILE_ID,                          /* Application profile identifier */
    HA_DEV_SIMPLE_SENSOR,                   /* Application device identifier */
    APP_ENDPOINT1,                          /* Endpoint */
    2,                                      /* Application device version */
    0,                                      /* Reserved */
    APP_EP1_IN_CLUSTER_NUM,                 /* Application input cluster count */
    APP_EP1_OUT_CLUSTER_NUM,                /* Application output cluster count */
    (uint16_t *)app_ep1_inClusterList,      /* Application input cluster list */
    (uint16_t *)app_ep1_outClusterList,     /* Application output cluster list */
};



/* Basic */
zcl_basicAttr_t g_zcl_basicAttrs =
{
    .zclVersion     = 0x03,
    .appVersion     = APP_RELEASE,
    .stackVersion   = (STACK_RELEASE|STACK_BUILD),
    .hwVersion      = HW_VERSION,
    .manuName       = ZCL_BASIC_MFG_NAME,
    .modelId        = ZCL_BASIC_MODEL_ID,
    .dateCode       = ZCL_BASIC_DATE_CODE,
    .powerSource    = POWER_SOURCE_MAINS_1_PHASE,
    .swBuildId      = ZCL_BASIC_SW_BUILD_ID,
    .deviceEnable   = TRUE,
};

const zclAttrInfo_t basic_attrTbl[] =
{
    { ZCL_ATTRID_BASIC_ZCL_VER,             ZCL_UINT8,      R,  (uint8_t*)&g_zcl_basicAttrs.zclVersion  },
    { ZCL_ATTRID_BASIC_APP_VER,             ZCL_UINT8,      R,  (uint8_t*)&g_zcl_basicAttrs.appVersion  },
    { ZCL_ATTRID_BASIC_STACK_VER,           ZCL_UINT8,      R,  (uint8_t*)&g_zcl_basicAttrs.stackVersion},
    { ZCL_ATTRID_BASIC_HW_VER,              ZCL_UINT8,      R,  (uint8_t*)&g_zcl_basicAttrs.hwVersion   },
    { ZCL_ATTRID_BASIC_MFR_NAME,            ZCL_CHAR_STR,   R,  (uint8_t*)g_zcl_basicAttrs.manuName     },
    { ZCL_ATTRID_BASIC_MODEL_ID,            ZCL_CHAR_STR,   R,  (uint8_t*)g_zcl_basicAttrs.modelId      },
    { ZCL_ATTRID_BASIC_DATE_CODE,           ZCL_CHAR_STR,   R,  (uint8_t*)g_zcl_basicAttrs.dateCode     },
    { ZCL_ATTRID_BASIC_POWER_SOURCE,        ZCL_ENUM8,      R,  (uint8_t*)&g_zcl_basicAttrs.powerSource },
    { ZCL_ATTRID_BASIC_DEV_ENABLED,         ZCL_BOOLEAN,    RW, (uint8_t*)&g_zcl_basicAttrs.deviceEnable},
    { ZCL_ATTRID_BASIC_SW_BUILD_ID,         ZCL_CHAR_STR,   R,  (uint8_t*)&g_zcl_basicAttrs.swBuildId   },

    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,   ZCL_UINT16,     R,  (uint8_t*)&zcl_attr_global_clusterRevision},

};

#define ZCL_BASIC_ATTR_NUM       sizeof(basic_attrTbl) / sizeof(zclAttrInfo_t)


/* Identify */
zcl_identifyAttr_t g_zcl_identifyAttrs =
{
    .identifyTime   = 0x0000,
};

const zclAttrInfo_t identify_attrTbl[] =
{
    { ZCL_ATTRID_IDENTIFY_TIME,             ZCL_UINT16,     RW, (uint8_t*)&g_zcl_identifyAttrs.identifyTime},

    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,   ZCL_UINT16,     R,  (uint8_t*)&zcl_attr_global_clusterRevision},
};

#define ZCL_IDENTIFY_ATTR_NUM           sizeof(identify_attrTbl) / sizeof(zclAttrInfo_t)

/* Identify */
zcl_diagnosticsAttr_t g_zcl_diagnosticsAttrs =
{
    .lastMessageLQI = 0,
};

const zclAttrInfo_t diagnostics_attrTbl[] =
{
    { ZCL_DIAGNOSTICS_ATTRID_LAST_MESSAGE_LQI,  ZCL_UINT8,      RW, (uint8_t*)&g_zcl_diagnosticsAttrs.lastMessageLQI},

    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,       ZCL_UINT16,     R,  (uint8_t*)&zcl_attr_global_clusterRevision},
};

#define ZCL_DIAGNOSTICS_ATTR_NUM           sizeof(diagnostics_attrTbl) / sizeof(zclAttrInfo_t)

#ifdef ZCL_GROUP
/* Group */
zcl_groupAttr_t g_zcl_group1Attrs =
{
    .nameSupport    = 0,
};

const zclAttrInfo_t group_attr1Tbl[] =
{
    { ZCL_ATTRID_GROUP_NAME_SUPPORT,        ZCL_BITMAP8,    R,  (uint8_t*)&g_zcl_group1Attrs.nameSupport},

    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,   ZCL_UINT16,     R,  (uint8_t*)&zcl_attr_global_clusterRevision},
};

#define ZCL_GROUP_1ATTR_NUM    sizeof(group_attr1Tbl) / sizeof(zclAttrInfo_t)

#endif

#ifdef ZCL_SCENE
/* Scene */
zcl_sceneAttr_t g_zcl_scene1Attrs = {
    .sceneCount     = 0,
    .currentScene   = 0,
    .currentGroup   = 0x0000,
    .sceneValid     = FALSE,
    .nameSupport    = 0,
};

const zclAttrInfo_t scene_attr1Tbl[] = {
    { ZCL_ATTRID_SCENE_SCENE_COUNT,         ZCL_UINT8,      R,  (uint8_t*)&g_zcl_scene1Attrs.sceneCount   },
    { ZCL_ATTRID_SCENE_CURRENT_SCENE,       ZCL_UINT8,      R,  (uint8_t*)&g_zcl_scene1Attrs.currentScene },
    { ZCL_ATTRID_SCENE_CURRENT_GROUP,       ZCL_UINT16,     R,  (uint8_t*)&g_zcl_scene1Attrs.currentGroup },
    { ZCL_ATTRID_SCENE_SCENE_VALID,         ZCL_BOOLEAN,    R,  (uint8_t*)&g_zcl_scene1Attrs.sceneValid   },
    { ZCL_ATTRID_SCENE_NAME_SUPPORT,        ZCL_BITMAP8,    R,  (uint8_t*)&g_zcl_scene1Attrs.nameSupport  },

    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,   ZCL_UINT16,     R,  (uint8_t*)&zcl_attr_global_clusterRevision},
};

#define ZCL_SCENE_1ATTR_NUM   sizeof(scene_attr1Tbl) / sizeof(zclAttrInfo_t)

#endif

zcl_timeAttr_t g_zcl_timeAttrs = {
    .time_utc   = 0xffffffff,
    .time_local = 0xffffffff,
};

const zclAttrInfo_t time_attrTbl[] =
{
    { ZCL_ATTRID_TIME,                      ZCL_UTC,    RW, (uint8_t*)&g_zcl_timeAttrs.time_utc         },
    { ZCL_ATTRID_LOCAL_TIME,                ZCL_UINT32, R,  (uint8_t*)&g_zcl_timeAttrs.time_local       },

    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,   ZCL_UINT16, R,  (uint8_t*)&zcl_attr_global_clusterRevision  },
};

#define ZCL_TIME_ATTR_NUM    sizeof(time_attrTbl) / sizeof(zclAttrInfo_t)


#ifdef ZCL_CO2_MEASUREMENT

zcl_co2Attr_t g_zcl_co2Attrs = {
        .value = 0.001014,
};


const zclAttrInfo_t co2_attrTbl[] = {
        { ZCL_CO2_MEASUREMENT_ATTRID_MEASUREDVALUE,     ZCL_SINGLE, RR, (uint8_t*)&g_zcl_co2Attrs.value             },
        { ZCL_CO2_MEASUREMENT_ATTRID_MINMEASUREDVALUE,  ZCL_SINGLE, R,  (uint8_t*)&g_zcl_co2Attrs.min               },
        { ZCL_CO2_MEASUREMENT_ATTRID_MAXMEASUREDVALUE,  ZCL_SINGLE, R,  (uint8_t*)&g_zcl_co2Attrs.max               },

        { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,           ZCL_UINT16, R,  (uint8_t*)&zcl_attr_global_clusterRevision  },
};

#define ZCL_CO2_ATTR_NUM   sizeof(co2_attrTbl) / sizeof(zclAttrInfo_t)

#endif

#ifdef ZCL_TEMPERATURE_MEASUREMENT
zcl_temperatureAttr_t g_zcl_temperatureAttrs = {
        .value = 0x8000,    /* temperature unknown  */
        .minValue = 0xF060, /* -40.00               */
        .maxValue = 0x2134, /* +85.00               */
};


const zclAttrInfo_t temperature_attrTbl[] = {
        { ZCL_TEMPERATURE_MEASUREMENT_ATTRID_MEASUREDVALUE,     ZCL_INT16, RR, (uint8_t*)&g_zcl_temperatureAttrs.value      },
        { ZCL_TEMPERATURE_MEASUREMENT_ATTRID_MINMEASUREDVALUE,  ZCL_INT16, R,  (uint8_t*)&g_zcl_temperatureAttrs.minValue   },
        { ZCL_TEMPERATURE_MEASUREMENT_ATTRID_MAXMEASUREDVALUE,  ZCL_INT16, R,  (uint8_t*)&g_zcl_temperatureAttrs.maxValue   },

        { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,           ZCL_UINT16, R,  (uint8_t*)&zcl_attr_global_clusterRevision  },
};

#define ZCL_TEMPERATURE_ATTR_NUM   sizeof(temperature_attrTbl) / sizeof(zclAttrInfo_t)
#endif


zcl_humidityAttr_t g_zcl_humidityAttrs = {
        .value = 0xffff,    /* humidity unknown  */
        .minValue = 0x0000,
        .maxValue = 0x2710, /* 100.00              */
};


const zclAttrInfo_t humidity_attrTbl[] = {
        { ZCL_ATTRID_HUMIDITY_MEASUREDVALUE,     ZCL_UINT16, RR, (uint8_t*)&g_zcl_humidityAttrs.value      },
        { ZCL_ATTRID_HUMIDITY_MINMEASUREDVALUE,  ZCL_UINT16, R,  (uint8_t*)&g_zcl_humidityAttrs.minValue   },
        { ZCL_ATTRID_HUMIDITY_MAXMEASUREDVALUE,  ZCL_UINT16, R,  (uint8_t*)&g_zcl_humidityAttrs.maxValue   },

        { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,           ZCL_UINT16, R,  (uint8_t*)&zcl_attr_global_clusterRevision  },
};

#define ZCL_HUMIDITY_ATTR_NUM   sizeof(humidity_attrTbl) / sizeof(zclAttrInfo_t)

zcl_pressureAttr_t g_zcl_pressureAttrs = {
        .value = 0x8000,    /* pressure unknown  */
        .minValue = 0x8001,
        .maxValue = 0x7fff,
};


const zclAttrInfo_t pressure_attrTbl[] = {
        { ZCL_ATTRID_PRESSURE_MEASUREDVALUE,     ZCL_INT16, RR, (uint8_t*)&g_zcl_pressureAttrs.value      },
        { ZCL_ATTRID_PRESSURE_MINMEASUREDVALUE,  ZCL_INT16, R,  (uint8_t*)&g_zcl_pressureAttrs.minValue   },
        { ZCL_ATTRID_PRESSURE_MAXMEASUREDVALUE,  ZCL_INT16, R,  (uint8_t*)&g_zcl_pressureAttrs.maxValue   },

        { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,           ZCL_UINT16, R,  (uint8_t*)&zcl_attr_global_clusterRevision  },
};

#define ZCL_PRESSURE_ATTR_NUM   sizeof(pressure_attrTbl) / sizeof(zclAttrInfo_t)


zcl_illuminanceAttr_t g_zcl_illuminanceAttrs = {
        .value = 0,    /* illuminance low  */
        .minValue = 0x0001,
        .maxValue = 0xfffe,
};


const zclAttrInfo_t illuminance_attrTbl[] = {
        { ZCL_ATTRID_MEASURED_VALUE,            ZCL_UINT16, RR, (uint8_t*)&g_zcl_illuminanceAttrs.value     },
        { ZCL_ATTRID_MIN_MEASURED_VALUE,        ZCL_UINT16, R,  (uint8_t*)&g_zcl_illuminanceAttrs.minValue  },
        { ZCL_ATTRID_MAX_MEASURED_VALUE,        ZCL_UINT16, R,  (uint8_t*)&g_zcl_illuminanceAttrs.maxValue  },

        { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,   ZCL_UINT16, R,  (uint8_t*)&zcl_attr_global_clusterRevision  },
};

#define ZCL_ILLUMINANCE_ATTR_NUM   sizeof(illuminance_attrTbl) / sizeof(zclAttrInfo_t)



/**
 *  @brief Definition for simple switch ZCL specific cluster
 */
const zcl_specClusterInfo_t g_appEp1ClusterList[] = {
    {ZCL_CLUSTER_GEN_BASIC,         MANUFACTURER_CODE_NONE, ZCL_BASIC_ATTR_NUM,         basic_attrTbl,          zcl_basic_register,         app_basicCb},
    {ZCL_CLUSTER_GEN_IDENTIFY,      MANUFACTURER_CODE_NONE, ZCL_IDENTIFY_ATTR_NUM,      identify_attrTbl,       zcl_identify_register,      app_identifyCb},
    {ZCL_CLUSTER_GEN_DIAGNOSTICS,   MANUFACTURER_CODE_NONE, ZCL_DIAGNOSTICS_ATTR_NUM,   diagnostics_attrTbl,    zcl_diagnostics_register,   app_diagnosticsCb},
    {ZCL_CLUSTER_GEN_TIME,          MANUFACTURER_CODE_NONE, ZCL_TIME_ATTR_NUM,          time_attrTbl,           zcl_time_register,          app_timeCb},
#ifdef ZCL_GROUP
    {ZCL_CLUSTER_GEN_GROUPS,        MANUFACTURER_CODE_NONE, ZCL_GROUP_1ATTR_NUM,        group_attr1Tbl,         zcl_group_register,         NULL},
#endif
#ifdef ZCL_SCENE
    {ZCL_CLUSTER_GEN_SCENES,        MANUFACTURER_CODE_NONE, ZCL_SCENE_1ATTR_NUM,        scene_attr1Tbl,         zcl_scene_register,         app_sceneCb},
#endif
#ifdef ZCL_CO2_MEASUREMENT
    {ZCL_CLUSTER_MS_CO2_MEASUREMENT, MANUFACTURER_CODE_NONE, ZCL_CO2_ATTR_NUM,          co2_attrTbl,        zcl_co2_measurement_register,   app_co2Cb},
#endif
#ifdef ZCL_TEMPERATURE_MEASUREMENT
    {ZCL_CLUSTER_MS_TEMPERATURE_MEASUREMENT, MANUFACTURER_CODE_NONE, ZCL_TEMPERATURE_ATTR_NUM,  temperature_attrTbl,    zcl_temperature_measurement_register,   app_temperatureCb},
#endif
    {ZCL_CLUSTER_MS_RELATIVE_HUMIDITY, MANUFACTURER_CODE_NONE, ZCL_HUMIDITY_ATTR_NUM,  humidity_attrTbl,    zcl_humidity_measurement_register,   app_humidityCb},
    {ZCL_CLUSTER_MS_PRESSURE_MEASUREMENT, MANUFACTURER_CODE_NONE, ZCL_PRESSURE_ATTR_NUM,  pressure_attrTbl,    zcl_pressure_measurement_register,   app_pressureCb},
    {ZCL_CLUSTER_MS_ILLUMINANCE_MEASUREMENT, MANUFACTURER_CODE_NONE, ZCL_ILLUMINANCE_ATTR_NUM,  illuminance_attrTbl,    zcl_illuminanceMeasure_register,   app_illuminanceCb},
};

uint8_t APP_EP1_CB_CLUSTER_NUM = (sizeof(g_appEp1ClusterList)/sizeof(g_appEp1ClusterList[0]));

