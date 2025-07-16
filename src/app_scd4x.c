#include "app_main.h"

static scd4x_dev_t scd4x_dev;
static uint16_t pressure_pa = 0x8000;
static uint16_t co2, temperature, humidity;
static uint8_t err_cnt = 0;

#if UART_PRINTF_MODE && DEBUG_SCD4X
static void scd4x_error_codes_print_result(const char api_name[], int8_t rslt) {
    if (rslt != SCD4X_OK)
    {
        printf("%s\t", api_name);

        switch (rslt)
        {
            case SCD4X_ERR_NULL_PTR:
                printf("Error [%d] : Null pointer error.", rslt);
                printf(
                    "It occurs when the user tries to assign value (not address) to a pointer, which has been initialized to NULL.\r\n");
                break;

            case SCD4X_ERR_COMM_FAIL:
                printf("Error [%d] : Communication failure error.", rslt);
                printf(
                    "It occurs due to read/write operation failure and also due to power failure during communication\r\n");
                break;

            case SCD4X_ERR_DEV_NOT_FOUND:
                printf("Error [%d] : Device not found error. It occurs when the device chip id is incorrectly read\r\n",
                       rslt);
                break;

            case SCD4X_ERR_ADDR_NOT_FOUND:
                printf("Error [%d] : Device not found at address. Address %d not correct\r\n",
                       rslt, SCD4X_I2C_ADDRESS);
                break;

            case SCD4X_ERR_INVALID_LEN:
                printf("Error [%d] : Invalid length error. It occurs when write is done with invalid length\r\n", rslt);
                break;

            case SCD4X_ERR_CRC:
                printf("Error [%d] : Invalid crc\r\n", rslt);
                break;

            case SCD4X_ERR_NOT_READY:
                printf("Error [%d] : Device not ready. Output buffer is empty\r\n", rslt);
                break;

            case SCD4X_ERR_MODE_FAIL:
                printf("Error [%d] : The device cannot accept commands. The device is in periodic measurement mode.\r\n", rslt);
                break;

            default:
                printf("Error [%d] : Unknown error code\r\n", rslt);
                break;
        }
    }
}
#endif



int8_t scd4x_i2c_read(uint16_t reg_addr, uint32_t reg_len, uint8_t *reg_data, uint32_t len, scd4x_dev_t *dev) {
    drv_i2c_read_series(dev->addr << 1, reg_addr, reg_len, reg_data, len);
    return (reg_i2c_status & FLD_I2C_NAK);
}

int8_t scd4x_i2c_write(uint16_t reg_addr, const uint8_t *reg_data, uint32_t len, scd4x_dev_t *dev) {
    drv_i2c_write_series(dev->addr << 1, reg_addr, 2, (uint8_t*)reg_data, len);
    return (reg_i2c_status & FLD_I2C_NAK);
}

void scd4x_delay(uint32_t period) {
  sleep_ms(period);
}



uint8_t app_scd4x_init() {

    err_cnt = 0;

    scd4x_dev.delay = NULL;
    scd4x_dev.read = NULL;
    scd4x_dev.write = NULL;
    scd4x_dev.addr = 0;
    scd4x_dev.periodic_runnig = true;

    uint8_t addr = (SCD4X_I2C_ADDRESS << 1);

    uint8_t ret = scan_i2c_addr(addr);

    if (ret == addr) {
        scd4x_dev.addr = SCD4X_I2C_ADDRESS;
        scd4x_dev.delay = scd4x_delay;
        scd4x_dev.read = scd4x_i2c_read;
        scd4x_dev.write = scd4x_i2c_write;
    } else {
        ret = 0;
    }

    ret = scd4x_init(&scd4x_dev);
#if UART_PRINTF_MODE && DEBUG_SCD4X
    scd4x_error_codes_print_result("scd4x_init", ret);
#endif

//    ret = scd4x_cmd_wake_up(&scd4x_dev);
//#if UART_PRINTF_MODE && DEBUG_SCD4X
//    scd4x_error_codes_print_result("scd4x_wake_up", ret);
//#endif

    ret = scd4x_cmd_stop_periodic(&scd4x_dev);
#if UART_PRINTF_MODE && DEBUG_SCD4X
    scd4x_error_codes_print_result("scd4x_stop_periodic", ret);
#endif
    if (ret == SCD4X_OK)
        scd4x_dev.periodic_runnig = false;

    ret = scd4x_cmd_reinit(&scd4x_dev);
#if UART_PRINTF_MODE && DEBUG_SCD4X
    scd4x_error_codes_print_result("scd4x_reinit", ret);
#endif

    app_scd4x_set_pressure();

    uint16_t pressure = 0;
    ret = scd4x_cmd_get_ambient_pressure(&pressure);
#if UART_PRINTF_MODE && DEBUG_SCD4X
    scd4x_error_codes_print_result("scd4x_cmd_get_ambient_pressure", ret);
#endif

#if UART_PRINTF_MODE && DEBUG_SCD4X
    if (ret == SCD4X_OK) {
        printf("[scd4x] pressure: %d\r\n", pressure);
    }
#endif

    ret = scd4x_cmd_start_periodic();
#if UART_PRINTF_MODE && DEBUG_SCD4X
    scd4x_error_codes_print_result("scd4x_start_periodic", ret);
#endif
    if (ret == SCD4X_OK)
        scd4x_dev.periodic_runnig = true;

    return ret;
}

void app_scd4x_set_pressure() {

    uint16_t attr_len;
    uint16_t pressure;

    status_t ret = zcl_getAttrVal(APP_ENDPOINT1, ZCL_CLUSTER_MS_PRESSURE_MEASUREMENT, ZCL_ATTRID_PRESSURE_MEASUREDVALUE, &attr_len, (uint8_t*)&pressure);

    if (ret == ZCL_STA_SUCCESS && pressure != pressure_pa) {
        pressure_pa = pressure;

        ret = scd4x_cmd_set_ambient_pressure(pressure);
#if UART_PRINTF_MODE && DEBUG_SCD4X
        scd4x_error_codes_print_result("scd4x_cmd_set_ambient_pressure", ret);
#endif
    }
}

void app_scd4x_measurement() {

    app_scd4x_set_pressure();

    scd4x_error_t ret = scd4x_cmd_read(&co2, &temperature, &humidity);
#if UART_PRINTF_MODE && DEBUG_SCD4X
    scd4x_error_codes_print_result("scd4x_cmd_read", ret);
#endif

    if (ret == SCD4X_OK) {

#if 0 //UART_PRINTF_MODE && DEBUG_SCD4X
        printf("[scd4x] co2: %d, temperature: %d, humidity: %d\r\n", co2, temperature, humidity);
#endif

        if (co2 != 0 && co2 < 10000) {
            float attrCo2 = (float)co2/1000000;
            zcl_setAttrVal(APP_ENDPOINT1, ZCL_CLUSTER_MS_CO2_MEASUREMENT, ZCL_CO2_MEASUREMENT_ATTRID_MEASUREDVALUE, (uint8_t*)&attrCo2);
        }

    } else {
        err_cnt++;
    }

    if (err_cnt == 2) {
        app_scd4x_init();
    }
}

uint16_t app_scd4x_get_co2() {
    return co2;
}

uint8_t app_scd4x_forced_calibration() {

    scd4x_error_t ret = scd4x_cmd_stop_periodic();
    int16_t frc;

    if (ret == SCD4X_OK) {
        ret = scd4x_cmd_forced_calibration(&frc);
        scd4x_cmd_start_periodic();
    }

    g_appCtx.co2_forced_calibration = 0;

//    printf("app_scd4x_forced_calibration(). ret: %d\r\n");

    if (frc != config.co2_frc) {
        zcl_setAttrVal(APP_ENDPOINT1, ZCL_CLUSTER_MS_CO2_MEASUREMENT, ZCL_ATTRID_CMS_CUSTOM_CO2_FORCED_CALIBRATION, (uint8_t*)&frc);
        config.co2_frc = frc;
        config_save();
    }


    return ret;
}

uint8_t app_scd4x_factory_reset() {

    scd4x_error_t ret = scd4x_cmd_stop_periodic();

    if (ret == SCD4X_OK) {
        ret = scd4x_factory_reset();
        if (ret == SCD4X_OK) {
            config.co2_frc = 0;
            config_save();
            zcl_setAttrVal(APP_ENDPOINT1, ZCL_CLUSTER_MS_CO2_MEASUREMENT, ZCL_ATTRID_CMS_CUSTOM_CO2_FORCED_CALIBRATION, (uint8_t*)&config.co2_frc);
        }
        scd4x_cmd_start_periodic();
    }

    g_appCtx.co2_factory_reset = 0;

    return ret;
}
