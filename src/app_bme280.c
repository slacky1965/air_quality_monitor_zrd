#include "app_main.h"
#include "app_bme280.h"

#define SAMPLE_COUNT  UINT8_C(3)

static struct bme280_dev dev;
static struct bme280_data comp_data;
static struct bme280_settings settings;
static int8_t rslt;
static uint8_t dev_addr;

static void bme280_error_codes_print_result(const char api_name[], int8_t rslt) {
    if (rslt != BME280_OK)
    {
        printf("%s\t", api_name);

        switch (rslt)
        {
            case BME280_E_NULL_PTR:
                printf("Error [%d] : Null pointer error.", rslt);
                printf(
                    "It occurs when the user tries to assign value (not address) to a pointer, which has been initialized to NULL.\r\n");
                break;

            case BME280_E_COMM_FAIL:
                printf("Error [%d] : Communication failure error.", rslt);
                printf(
                    "It occurs due to read/write operation failure and also due to power failure during communication\r\n");
                break;

            case BME280_E_DEV_NOT_FOUND:
                printf("Error [%d] : Device not found error. It occurs when the device chip id is incorrectly read\r\n",
                       rslt);
                break;

            case BME280_E_INVALID_LEN:
                printf("Error [%d] : Invalid length error. It occurs when write is done with invalid length\r\n", rslt);
                break;

            default:
                printf("Error [%d] : Unknown error code\r\n", rslt);
                break;
        }
    }
}


void app_bme280_init() {

    dev_addr = BME280_I2C_ADDR_PRIM; //BME280_I2C_ADDR_SEC; //
    dev.intf_ptr = &dev_addr;
    dev.intf = BME280_I2C_INTF;
    dev.read = bme280_i2c_read;
    dev.write = bme280_i2c_write;
    dev.delay_us = bme280_delay_us;

    rslt = bme280_init(&dev);
    bme280_error_codes_print_result("bme280_init", rslt);

    /* Always read the current settings before writing, especially when all the configuration is not modified */
    rslt = bme280_get_sensor_settings(&settings, &dev);
    bme280_error_codes_print_result("bme280_get_sensor_settings", rslt);

    /* Configuring the over-sampling rate, filter coefficient and standby time */
    /* Overwrite the desired settings */
    settings.filter = BME280_FILTER_COEFF_2;

//    /* Over-sampling rate for humidity, temperature and pressure */
//    settings.osr_h = BME280_OVERSAMPLING_1X;
//    settings.osr_p = BME280_OVERSAMPLING_1X;
//    settings.osr_t = BME280_OVERSAMPLING_1X;

    settings.osr_h = BME280_OVERSAMPLING_1X;
    settings.osr_p = BME280_OVERSAMPLING_16X;
    settings.osr_t = BME280_OVERSAMPLING_2X;
    settings.filter = BME280_FILTER_COEFF_16;
//    rslt = bme280_set_sensor_settings(BME280_OSR_PRESS_SEL | BME280_OSR_TEMP_SEL | BME280_OSR_HUM_SEL | BME280_FILTER_SEL, &dev);


    /* Setting the standby time */
    settings.standby_time = BME280_STANDBY_TIME_0_5_MS;

    rslt = bme280_set_sensor_settings(BME280_SEL_ALL_SETTINGS, &settings, &dev);
    bme280_error_codes_print_result("bme280_set_sensor_settings", rslt);

//    /* Always set the power mode after setting the configuration */
//    rslt = bme280_set_sensor_mode(BME280_POWERMODE_NORMAL, &dev);
//    bme280_error_codes_print_result("bme280_set_power_mode", rslt);

    rslt = bme280_set_sensor_mode(BME280_POWERMODE_FORCED, &dev);
    bme280_error_codes_print_result("bme280_set_power_mode", rslt);

}

int8_t bme280_i2c_read(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *intf_ptr) {

    dev_addr = *(uint8_t*)intf_ptr;

    printf("bme280_i2c_read. reg_addr: 0x%x\r\n", reg_addr);

    drv_i2c_read_series(dev_addr << 1, reg_addr, 1, reg_data, len);
    printf("reg_data: 0x%x\r\n", *reg_data);
    return 0;
}

int8_t bme280_i2c_write(uint8_t reg_addr, const uint8_t *reg_data, uint32_t len, void *intf_ptr) {

    dev_addr = *(uint8_t*)intf_ptr;

    printf("bme280_i2c_write. reg_addr: 0x%x\r\n", reg_addr);
    drv_i2c_write_series(dev_addr << 1, reg_addr, 1, (uint8_t*)reg_data, len);
    printf("reg_data: 0x");
    for (uint8_t i = 0; i < len; i++) {
        if (reg_data[i] < 0x10)
            printf("0");
        printf("%x", reg_data[i]);
    }
    printf("\r\n");
    return 0;
}

void bme280_delay_us(uint32_t period, void *intf_ptr) {
  sleep_us(period);
}

/*!
 *  @brief This internal API is used to get compensated temperature data.
 */
static int8_t get_temperature(uint32_t period, struct bme280_dev *dev)
{
    int8_t rslt = BME280_E_NULL_PTR;
    int8_t idx = 0;
    uint8_t status_reg;

    while (idx < SAMPLE_COUNT)
    {
        rslt = bme280_get_regs(BME280_REG_STATUS, &status_reg, 1, dev);
        bme280_error_codes_print_result("bme280_get_regs", rslt);

        if (status_reg & BME280_STATUS_MEAS_DONE)
        {
            /* Measurement time delay given to read sample */
            dev->delay_us(period, dev->intf_ptr);

            /* Read compensated data */
            rslt = bme280_get_sensor_data(BME280_TEMP, &comp_data, dev);
            bme280_error_codes_print_result("bme280_get_sensor_data", rslt);

#ifndef BME280_DOUBLE_ENABLE
            comp_data.temperature = comp_data.temperature / 100;
#endif

#ifdef BME280_DOUBLE_ENABLE
            printf("Temperature[%d]:   %lf deg C\n", idx, comp_data.temperature);
#else
            printf("Temperature[%d]:   %d deg C\n", idx, (long int)comp_data.temperature);
#endif
            idx++;
        }
    }

    return rslt;
}

void app_test_bme280() {

    dev.delay_us(40000, dev.intf_ptr);

    rslt = bme280_get_sensor_data(BME280_ALL, &comp_data, &dev);
    bme280_error_codes_print_result("bme280_get_sensor_data", rslt);

    if(rslt == BME280_OK) {
        printf("comp_data.temperature: %d\r\n", comp_data.temperature);
//        printf("comp_data.humidity: %d\r\n", comp_data.humidity);
//        printf("comp_data.pressure: %d\r\n", comp_data.pressure);
//        temperature = comp_data.temperature / 100.0; /* °C  */
//        humidity = comp_data.humidity / 1024.0; /* %   */
//        pressure = comp_data.pressure / 10000.0; /* hPa */
    }

//    uint32_t period;
//
//    /* Calculate measurement time in microseconds */
//    rslt = bme280_cal_meas_delay(&period, &settings);
//    bme280_error_codes_print_result("bme280_cal_meas_delay", rslt);
//
//    printf("\nTemperature calculation (Data displayed are compensated values)\n");
//    printf("Measurement time : %d us\r\n\r\n", (long unsigned int)period);
//
//
//    rslt = get_temperature(period, &dev);
//    bme280_error_codes_print_result("get_temperature", rslt);


}

