#ifndef SRC_INCLUDE_APP_CONFIG_H_
#define SRC_INCLUDE_APP_CONFIG_H_

#define READ_SENSORS_PERIOD_MIN         5
#define READ_SENSORS_PERIOD_MAX         600
#define TEMPERATURE_OFFSET_MIN         -500
#define TEMPERATURE_OFFSET_MAX          500
#define VOC_ONOFF_MIN                   1
#define VOC_ONOFF_MAX                   500
#define VOC_ONOFF_ENABLED               0x01
#define VOC_ONOFF_DISABLED              0x00
#define CO2_ONOFF_MIN                   400
#define CO2_ONOFF_MAX                   2000
#define CO2_ONOFF_ENABLED               0x01
#define CO2_ONOFF_DISABLED              0x00

#define DEFAULT_ROTATE                  APP_EPD_ROTATE_0
#define DEFAULT_INVERSION               APP_EPD_INVERSION_OFF
#define DEFAULT_READ_SENSORS_PERIOD     (READ_SENSORS_PERIOD_MIN * 2)
#define DEFAULT_BRIGHTNESS              0xFF
#define DEFAULT_DISPLAY_MODE            ZCL_DISPLAY_MODE_CELSIUS
#define DEFAULT_TEMPERATURE_OFFSET      0
#define DEFAULT_CO2_ONOFF               CO2_ONOFF_DISABLED
#define DEFAULT_CO2_ONOFF_MIN           CO2_ONOFF_MIN
#define DEFAULT_CO2_ONOFF_MAX           CO2_ONOFF_MAX
#define DEFAULT_VOC_ONOFF               VOC_ONOFF_DISABLED
#define DEFAULT_VOC_ONOFF_MIN           VOC_ONOFF_MIN
#define DEFAULT_VOC_ONOFF_MAX           VOC_ONOFF_MAX


typedef struct __attribute__((packed)) {
    uint8_t     rotate;
    uint8_t     inversion;
    uint16_t    read_sensors_period;
    uint8_t     brightness;             /* level                         */
    uint8_t     d_mode;                 /* Display mode C or F           */
    int16_t     temperature_offset;     /* -5 ... +5 with step 0.1 * 100 */
    uint8_t     voc_onoff;              /* 0 - disabled, 1 - enabled     */
    uint16_t    voc_onoff_low;          /* 50                            */
    uint16_t    voc_onoff_high;         /* 500                           */
    uint8_t     co2_onoff;              /* 0 - disabled, 1 - enabled     */
    uint16_t    co2_onoff_low;          /* 400                           */
    uint16_t    co2_onoff_high;         /* 2000                          */
    int16_t     co2_frc;
    app_reporting_t reporting_co2;
    app_reporting_t reporting_voc;
    app_reporting_t reporting_temp;
    app_reporting_t reporting_hum;
    app_reporting_t reporting_press;
    app_reporting_t reporting_illum;
    app_reporting_t reporting_rotate;
    app_reporting_t reporting_inversion;
    app_reporting_t reporting_dMode;
    app_reporting_t reporting_co2Frc;
    uint8_t     crc;
} config_t;

extern config_t config;

nv_sts_t config_save();
nv_sts_t config_restore();

#endif /* SRC_INCLUDE_APP_CONFIG_H_ */
