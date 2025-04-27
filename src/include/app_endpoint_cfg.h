#ifndef SRC_INCLUDE_APP_ENDPOINT_CFG_H_
#define SRC_INCLUDE_APP_ENDPOINT_CFG_H_

#define APP_ENDPOINT1 0x01
#define APP_ENDPOINT2 0x02
#define APP_ENDPOINT3 0x03
#define APP_ENDPOINT4 0x04
#define APP_ENDPOINT5 0x05

/**
 *  @brief Defined for basic cluster attributes
 */
typedef struct{
    uint8_t  zclVersion;
    uint8_t  appVersion;
    uint8_t  stackVersion;
    uint8_t  hwVersion;
    uint8_t  manuName[ZCL_BASIC_MAX_LENGTH];
    uint8_t  modelId[ZCL_BASIC_MAX_LENGTH];
    uint8_t  dateCode[ZCL_BASIC_MAX_LENGTH];
    uint8_t  powerSource;
    uint8_t  genDevClass;                        //attr 8
    uint8_t  genDevType;                         //attr 9
    uint8_t  deviceEnable;
    uint8_t  swBuildId[ZCL_BASIC_MAX_LENGTH];    //attr 4000
} zcl_basicAttr_t;

/**
 *  @brief Defined for identify cluster attributes
 */
typedef struct{
    uint16_t identifyTime;
}zcl_identifyAttr_t;

/**
 *  @brief Defined for diagnostics cluster attributes
 */
typedef struct{
    uint8_t lastMessageLQI;
}zcl_diagnosticsAttr_t;

/**
 *  @brief Defined for group cluster attributes
 */
typedef struct{
    u8  nameSupport;
}zcl_groupAttr_t;

/**
 *  @brief Defined for scene cluster attributes
 */
typedef struct{
    u8   sceneCount;
    u8   currentScene;
    u8   nameSupport;
    bool sceneValid;
    u16  currentGroup;
}zcl_sceneAttr_t;

/**
 *  @brief Defined for CO2 clusters attributes
 */
typedef struct {
    float   value;
    float   min;
    float   max;
} zcl_co2Attr_t;

/**
 *  @brief Defined for time cluster attributes
 */
typedef struct {
    uint32_t time_utc;
    uint32_t time_local;
} zcl_timeAttr_t;

/**
 *  @brief Defined for tempearute cluster attributes
 */
typedef struct {
    int16_t value;
    int16_t minValue;
    int16_t maxValue;
} zcl_temperatureAttr_t;

/**
 *  @brief Defined for humidity cluster attributes
 */
typedef struct {
    int16_t value;
    int16_t minValue;
    int16_t maxValue;
} zcl_humidityAttr_t;

/**
 *  @brief Defined for ILLUMINANCE cluster attributes
 */
typedef struct {
    int16_t value;
    int16_t minValue;
    int16_t maxValue;
} zcl_illuminanceAttr_t;

extern uint8_t APP_EP1_CB_CLUSTER_NUM;

/* Attributes */
extern zcl_basicAttr_t g_zcl_basicAttrs;
extern zcl_identifyAttr_t g_zcl_identifyAttrs;
extern zcl_groupAttr_t g_zcl_group1Attrs;
extern zcl_sceneAttr_t g_zcl_scene1Attrs;
extern zcl_co2Attr_t g_zcl_co2Attrs;
extern zcl_temperatureAttr_t g_zcl_temperatureAttrs;
extern zcl_humidityAttr_t g_zcl_humidityAttrs;
extern zcl_diagnosticsAttr_t g_zcl_diagnosticsAttrs;

int32_t app_diagnostics_cmdCb(void *arg);

#endif /* SRC_INCLUDE_APP_ENDPOINT_CFG_H_ */
