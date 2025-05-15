#ifndef SRC_ZCL_ZCL_THERMOSTAT_UI_CFG_H_
#define SRC_ZCL_ZCL_THERMOSTAT_UI_CFG_H_

#define ZCL_ATTRID_HVAC_TEMPERATURE_DISPLAY_MODE        0x0000
#define ZCL_ATTRID_HVAC_KEYPAD_LOCKOUT                  0x0001
#define ZCL_ATTRID_HVAC_SCHEDULE_PROG_VISIBILITY        0x0002

#define ZCL_DISPLAY_MODE_CELSIUS                        0x00
#define ZCL_DISPLAY_MODE_FAHRENHEIT                     0x01

#define ZCL_ATTRID_HVAC_CUSTOM_        0xF000

_CODE_ZCL_ status_t zcl_thermostat_ui_cfg_register(u8 endpoint, u16 manuCode, u8 arrtNum, const zclAttrInfo_t attrTbl[], cluster_forAppCb_t cb);

//uint8_t zcl_convert_temperature_fromCtoF(int16_t tempC);
//int16_t zcl_convert_temperature_fromFtoC(int16_t tempF);

#endif /* SRC_ZCL_ZCL_THERMOSTAT_UI_CFG_H_ */
