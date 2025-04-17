#ifndef SRC_ZCL_ZCL_RELATIVE_HUMIDITY_MEASUREMENT_H_
#define SRC_ZCL_ZCL_RELATIVE_HUMIDITY_MEASUREMENT_H_

#include "app_cfg.h"

/********************* Temperature Measurement ************************/
#if ZCL_HUMIDITY_MEASUREMENT_SUPPORT
#define ZCL_HUMIDITY_MEASUREMENT
#endif



#define ZCL_ATTRID_HUMIDITY_MEASUREDVALUE       0x0000
#define ZCL_ATTRID_HUMIDITY_MINMEASUREDVALUE    0x0001
#define ZCL_ATTRID_HUMIDITY_MAXMEASUREDVALUE    0x0002

status_t zcl_humidity_measurement_register(u8 endpoint, u16 manuCode, u8 attrNum, const zclAttrInfo_t attrTbl[], cluster_forAppCb_t cb);


#endif /* SRC_ZCL_ZCL_RELATIVE_HUMIDITY_MEASUREMENT_H_ */
