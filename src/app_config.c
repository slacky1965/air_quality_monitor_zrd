#include "app_main.h"

config_t config;

uint8_t checksum(uint8_t *data, uint16_t length) {

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

