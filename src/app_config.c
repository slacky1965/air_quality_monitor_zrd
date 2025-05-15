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
        g_zcl_levelAttrs.currentLevel = config.brightness;

    } else {
        /* default config */
#if UART_PRINTF_MODE
        printf("Default config\r\n");
#endif
        config.rotate = APP_EPD_ROTATE_0;
        config.inversion = APP_EPD_INVERSION_OFF;
        config.mesurement_period = 5; //DEFAULT_MESURE_PERIOD;
        config.brightness = 0xFF;
//        config.unit_pressure = APP_EPD_UNIT_PRESSURE_MM;
//        config.joined = false;

        g_zcl_levelAttrs.currentLevel = config.brightness;
    }

#else
    st = NV_ENABLE_PROTECT_ERROR;
#endif

    return st;
}

