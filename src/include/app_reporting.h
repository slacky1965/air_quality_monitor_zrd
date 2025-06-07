#ifndef SRC_INCLUDE_APP_REPORTING_H_
#define SRC_INCLUDE_APP_REPORTING_H_

#define DEF_MIN_INTERVAL_CO2        10
#define DEF_MAX_INTERVAL_CO2        3600
#define DEF_CHANGE_CO2              0.00001
#define DEF_MIN_INTERVAL_VOC        10
#define DEF_MAX_INTERVAL_VOC        3600
#define DEF_CHANGE_VOC              30
#define DEF_MIN_INTERVAL_TEMP       10
#define DEF_MAX_INTERVAL_TEMP       3600
#define DEF_CHANGE_TEMP             10
#define DEF_MIN_INTERVAL_HUM        10
#define DEF_MAX_INTERVAL_HUM        3600
#define DEF_CHANGE_HUM              10
#define DEF_MIN_INTERVAL_ILL        10
#define DEF_MAX_INTERVAL_ILL        3600
#define DEF_CHANGE_ILL              5
#define DEF_MIN_INTERVAL_PRESS      10
#define DEF_MAX_INTERVAL_PRESS      3600
#define DEF_CHANGE_PRESS            50
#define DEF_MIN_INTERVAL_ROTATE     0
#define DEF_MAX_INTERVAL_ROTATE     65000
#define DEF_CHANGE_ROTATE           0
#define DEF_MIN_INTERVAL_INVERS     0
#define DEF_MAX_INTERVAL_INVERS     65000
#define DEF_CHANGE_INVERS           0
#define DEF_MIN_INTERVAL_DMODE      0
#define DEF_MAX_INTERVAL_DMODE      65000
#define DEF_CHANGE_DMODE            0
#define DEF_MIN_INTERVAL_FRC_CO2    0
#define DEF_MAX_INTERVAL_FRC_CO2    65000
#define DEF_CHANGE_FRC_CO2          0


typedef union {
    uint8_t     reportableChange[REPORTABLE_CHANGE_MAX_ANALOG_SIZE];
    uint8_t     reportableChange_u8;
    uint16_t    reportableChange_u16;
    uint32_t    reportableChange_u32;
    float       reportableChange_float;
}  app_reportableChange_t;

typedef struct {
    uint16_t                minInterval;
    uint16_t                maxInterval;
    app_reportableChange_t  reportableChange;
} app_reporting_t;

int32_t forcedReportCb(void *arg);

#endif /* SRC_INCLUDE_APP_REPORTING_H_ */
