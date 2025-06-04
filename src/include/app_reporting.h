#ifndef SRC_INCLUDE_APP_REPORTING_H_
#define SRC_INCLUDE_APP_REPORTING_H_

typedef union {
//    uint8_t     reportableChange_array[8];
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
