#ifndef SRC_ZCL_ZCL_CUSTOM_ATTR_H_
#define SRC_ZCL_ZCL_CUSTOM_ATTR_H_

#define ZCL_ATTRID_TMS_CUSTOM_TEMPERATURE_OFFSET        0xF000          /* Temperature measurement cluster   */
#define ZCL_ATTRID_TMS_CUSTOM_READ_SENSORS_PERIOD       0xF001          /* Temperature measurement cluster   */
#define ZCL_ATTRID_CMS_CUSTOM_CO2_ONOFF                 0xF002          /* Concentration measurement cluster */
#define ZCL_ATTRID_CMS_CUSTOM_CO2_LOW                   0xF003          /* Concentration measurement cluster */
#define ZCL_ATTRID_CMS_CUSTOM_CO2_HIGH                  0xF004          /* Concentration measurement cluster */
#define ZCL_ATTRID_AI_CUSTOM_VOC_ONOFF                  0xF005          /* Analog inptut cluster             */
#define ZCL_ATTRID_AI_CUSTOM_VOC_LOW                    0xF006          /* Analog inptut cluster             */
#define ZCL_ATTRID_AI_CUSTOM_VOC_HIGH                   0xF007          /* Analog inptut cluster             */
#define ZCL_ATTRID_CMS_CUSTOM_CO2_FORCED_CALIBRATION    0xF008          /* Concentration measurement cluster */
#define ZCL_ATTRID_HVAC_CUSTOM_FEATURES_SENSORS         0xF009          /* Thermostat UI Config cluster      */
#define ZCL_ATTRID_HVAC_CUSTOM_DISPLAY_ROTATE           0xF00A          /* Thermostat UI Config cluster      */
#define ZCL_ATTRID_HVAC_CUSTOM_DISPLAY_INVERSION        0xF00B          /* Thermostat UI Config cluster      */
#define ZCL_ATTRID_HVAC_CUSTOM_SOUND                    0xF00C          /* Thermostat UI Config cluster      */

typedef enum {
    FEATURES_NOTHING = 0x00,
    FEATURES_CO2_FORCED_CALIBRATION,
    FEATURES_CO2_FACTORY_RESET,
    FEATURES_BIND_RESET
} features_sensors_t;


#endif /* SRC_ZCL_ZCL_CUSTOM_ATTR_H_ */
