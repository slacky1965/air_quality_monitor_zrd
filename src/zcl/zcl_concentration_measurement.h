#ifndef SRC_ZCL_ZCL_CONCENTRATION_MEASUREMENT_H_
#define SRC_ZCL_ZCL_CONCENTRATION_MEASUREMENT_H_

#include "app_cfg.h"

/********************* CO2 Measurement ************************/
#if ZCL_CO2_MEASUREMENT_SUPPORT
#define ZCL_CO2_MEASUREMENT
#endif

#ifdef ZCL_CO2_MEASUREMENT

#define ZCL_CLUSTER_MS_CO2_MEASUREMENT                  0x040D

#define ZCL_CO2_MEASUREMENT_ATTRID_MEASUREDVALUE        0x0000
#define ZCL_CO2_MEASUREMENT_ATTRID_MINMEASUREDVALUE     0x0001
#define ZCL_CO2_MEASUREMENT_ATTRID_MAXMEASUREDVALUE     0x0002

status_t zcl_co2_measurement_register(u8 endpoint, u16 manuCode, u8 attrNum, const zclAttrInfo_t attrTbl[], cluster_forAppCb_t cb);


#endif

#endif /* SRC_ZCL_ZCL_CONCENTRATION_MEASUREMENT_H_ */
