#ifndef SRC_ZCL_ZCL_PRESSURE_MEASUREMENT_H_
#define SRC_ZCL_ZCL_PRESSURE_MEASUREMENT_H_

#include "app_cfg.h"

/********************* Pressure Measurement ************************/
#if ZCL_PRESSURE_MEASUREMENT_SUPPORT
#define ZCL_PRESSURE_MEASUREMENT
#endif



#define ZCL_ATTRID_PRESSURE_MEASUREDVALUE       0x0000
#define ZCL_ATTRID_PRESSURE_MINMEASUREDVALUE    0x0001
#define ZCL_ATTRID_PRESSURE_MAXMEASUREDVALUE    0x0002

status_t zcl_pressure_measurement_register(u8 endpoint, u16 manuCode, u8 attrNum, const zclAttrInfo_t attrTbl[], cluster_forAppCb_t cb);


#endif /* SRC_ZCL_ZCL_PRESSURE_MEASUREMENT_H_ */
