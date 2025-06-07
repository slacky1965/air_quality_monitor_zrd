#include "app_main.h"

config_t config;

static uint8_t checksum(uint8_t *data, uint16_t length) {

    uint8_t crc8 = 0;

    for(uint8_t i = 0; i < length; i++) {
        crc8 += data[i];
    }

    return crc8;
}


nv_sts_t config_save() {
    nv_sts_t st = NV_SUCC;

#if NV_ENABLE

#if UART_PRINTF_MODE
    printf("Saved config\r\n");
#endif

    config.crc = checksum((uint8_t*)&config, sizeof(config_t)-1);
    st = nv_flashWriteNew(1, NV_MODULE_APP,  NV_ITEM_APP_USER_CFG, sizeof(config_t), (uint8_t*)&config);

#else
    st = NV_ENABLE_PROTECT_ERROR;
#endif

    return st;
}

nv_sts_t config_restore() {
    nv_sts_t st = NV_SUCC;

#if NV_ENABLE

    config_t temp_config;

    st = nv_flashReadNew(1, NV_MODULE_APP,  NV_ITEM_APP_USER_CFG, sizeof(config_t), (uint8_t*)&temp_config);

    if (st == NV_SUCC && temp_config.crc == checksum((uint8_t*)&temp_config, sizeof(config_t)-1)) {

#if UART_PRINTF_MODE
        printf("Restored config\r\n");
#endif

        memcpy(&config, &temp_config, (sizeof(config_t)));
    } else {
        /* default config */
#if UART_PRINTF_MODE
        printf("Default config\r\n");
#endif
        config.rotate = DEFAULT_ROTATE;
        config.inversion = DEFAULT_INVERSION;
        config.read_sensors_period = DEFAULT_READ_SENSORS_PERIOD;
        config.brightness = DEFAULT_BRIGHTNESS;
        config.d_mode = DEFAULT_DISPLAY_MODE;
        config.temperature_offset = 0;
        config.co2_onoff = DEFAULT_CO2_ONOFF;
        config.co2_onoff_low = DEFAULT_CO2_ONOFF_MIN;
        config.co2_onoff_high = DEFAULT_CO2_ONOFF_MAX;
        config.voc_onoff = DEFAULT_VOC_ONOFF;
        config.voc_onoff_low = DEFAULT_VOC_ONOFF_MIN;
        config.voc_onoff_high = DEFAULT_VOC_ONOFF_MAX;
        config.co2_frc = 0;
        config.sound = DEFAULT_SOUND;
        config.reporting_co2.minInterval = DEF_MIN_INTERVAL_CO2;
        config.reporting_co2.maxInterval = DEF_MAX_INTERVAL_CO2;
        config.reporting_co2.reportableChange.reportableChange_float = DEF_CHANGE_CO2;
        config.reporting_voc.minInterval = DEF_MIN_INTERVAL_VOC;
        config.reporting_voc.maxInterval = DEF_MAX_INTERVAL_VOC;
        config.reporting_voc.reportableChange.reportableChange_float = DEF_CHANGE_VOC;
        config.reporting_temp.minInterval = DEF_MIN_INTERVAL_TEMP;
        config.reporting_temp.maxInterval = DEF_MAX_INTERVAL_TEMP;
        config.reporting_temp.reportableChange.reportableChange_u16 = DEF_CHANGE_TEMP;
        config.reporting_hum.minInterval = DEF_MIN_INTERVAL_HUM;
        config.reporting_hum.maxInterval = DEF_MAX_INTERVAL_HUM;
        config.reporting_hum.reportableChange.reportableChange_u16 = DEF_CHANGE_HUM;
        config.reporting_press.minInterval = DEF_MIN_INTERVAL_PRESS;
        config.reporting_press.maxInterval = DEF_MAX_INTERVAL_PRESS;
        config.reporting_press.reportableChange.reportableChange_u16 = DEF_CHANGE_PRESS;
        config.reporting_illum.minInterval = DEF_MIN_INTERVAL_ILL;
        config.reporting_illum.maxInterval = DEF_MAX_INTERVAL_ILL;
        config.reporting_illum.reportableChange.reportableChange_u16 = DEF_CHANGE_ILL;
        config.reporting_rotate.minInterval = DEF_MIN_INTERVAL_ROTATE;
        config.reporting_rotate.maxInterval = DEF_MAX_INTERVAL_ROTATE;
        config.reporting_rotate.reportableChange.reportableChange_u8 = DEF_CHANGE_ROTATE;
        config.reporting_inversion.minInterval = DEF_MIN_INTERVAL_INVERS;
        config.reporting_inversion.maxInterval = DEF_MAX_INTERVAL_INVERS;
        config.reporting_inversion.reportableChange.reportableChange_u8 = DEF_CHANGE_INVERS;
        config.reporting_dMode.minInterval = DEF_MIN_INTERVAL_DMODE;
        config.reporting_dMode.maxInterval = DEF_MAX_INTERVAL_DMODE;
        config.reporting_dMode.reportableChange.reportableChange_u8 = DEF_CHANGE_DMODE;
        config.reporting_co2Frc.minInterval = DEF_MIN_INTERVAL_FRC_CO2;
        config.reporting_co2Frc.maxInterval = DEF_MAX_INTERVAL_FRC_CO2;
        config.reporting_co2Frc.reportableChange.reportableChange_u16 = DEF_CHANGE_FRC_CO2;
    }

    g_zcl_levelAttrs.currentLevel = config.brightness;
    g_zcl_thermostatCfgAttrs.temperatureDisplayMode = config.d_mode;
    g_zcl_customAttrs.temperature_offset = config.temperature_offset;
    g_zcl_customAttrs.read_sensors_period = config.read_sensors_period;
    g_zcl_customAttrs.co2_onoff = config.co2_onoff;
    g_zcl_customAttrs.co2_onoff_low = config.co2_onoff_low;
    g_zcl_customAttrs.co2_onoff_high = config.co2_onoff_high;
    g_zcl_customAttrs.voc_onoff = config.voc_onoff;
    g_zcl_customAttrs.voc_onoff_low = config.voc_onoff_low;
    g_zcl_customAttrs.voc_onoff_high = config.voc_onoff_high;
    g_zcl_customAttrs.co2_frc = config.co2_frc;
    g_zcl_customAttrs.display_rotate = config.rotate;
    g_zcl_customAttrs.dispaly_inversion = config.inversion;
    g_zcl_customAttrs.sound = config.sound;

#else
    st = NV_ENABLE_PROTECT_ERROR;
#endif

    return st;
}

