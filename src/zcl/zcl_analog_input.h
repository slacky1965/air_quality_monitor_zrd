#ifndef SRC_ZCL_ZCL_ANALOG_INPUT_H_
#define SRC_ZCL_ZCL_ANALOG_INPUT_H_

#if ZCL_ANALOG_INPUT_SUPPORT
#define ZCL_ANALOG_INPUT
#endif


#define ZCL_ANALOG_INPUT_ATTRID_DESCRIPTION             0x001C
#define ZCL_ANALOG_INPUT_ATTRID_MAX_PRESENT_VALUE       0x0041
#define ZCL_ANALOG_INPUT_ATTRID_MIN_PRESENT_VALUE       0x0045
#define ZCL_ANALOG_INPUT_ATTRID_OUT_OF_SERVICE          0x0051      // M false
#define ZCL_ANALOG_INPUT_ATTRID_PRESENT_VALUE           0x0055      // M value
#define ZCL_ANALOG_INPUT_ATTRID_RELIABILITY             0x0067

#define ZCL_ANALOG_INPUT_ATTRID_RESOLUTION              0x006A
#define ZCL_ANALOG_INPUT_ATTRID_STATUS_FLAG             0x006F      // M 0x00
#define ZCL_ANALOG_INPUT_ATTRID_ENGINEERING_UNITS       0x0075
#define ZCL_ANALOG_INPUT_ATTRID_APPLICATION_TYPE        0x0100      // O 0x0C Count

#define ZCL_ANALOG_INPUT_ATTRID_TYPE_IDX_VOC            0x0200

status_t zcl_analog_input_register(u8 endpoint, u16 manuCode, u8 attrNum, const zclAttrInfo_t attrTbl[], cluster_forAppCb_t cb);

#endif /* SRC_ZCL_ZCL_ANALOG_INPUT_H_ */
