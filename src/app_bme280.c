#include "app_main.h"
#include "app_bme280.h"

#define SAMPLE_COUNT  UINT8_C(3)

static struct bme280_dev dev;
static struct bme280_data comp_data;
static struct bme280_settings settings;
static int8_t rslt;
static uint8_t dev_addr;

static int32_t  temp;
static uint32_t rh, hpa;

#if UART_PRINTF_MODE && DEBUG_BME280
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
#endif

void app_bme280_init() {

    dev_addr = BME280_I2C_ADDR_PRIM; //BME280_I2C_ADDR_SEC; //
    dev.intf_ptr = &dev_addr;
    dev.intf = BME280_I2C_INTF;
    dev.read = bme280_i2c_read;
    dev.write = bme280_i2c_write;
    dev.delay_us = bme280_delay_us;

    rslt = bme280_init(&dev);
#if UART_PRINTF_MODE && DEBUG_BME280
    bme280_error_codes_print_result("bme280_init", rslt);
#endif

    /* Always read the current settings before writing, especially when all the configuration is not modified */
    rslt = bme280_get_sensor_settings(&settings, &dev);
#if UART_PRINTF_MODE && DEBUG_BME280
    bme280_error_codes_print_result("bme280_get_sensor_settings", rslt);
#endif

    /* Over-sampling rate for humidity, temperature and pressure */
    settings.osr_h = BME280_OVERSAMPLING_1X;
    settings.osr_p = BME280_OVERSAMPLING_16X;
    settings.osr_t = BME280_OVERSAMPLING_2X;
    /* Configuring the over-sampling rate, filter coefficient and standby time */
    /* Overwrite the desired settings */
    settings.filter = BME280_FILTER_COEFF_16;

    rslt = bme280_set_sensor_settings(BME280_SEL_ALL_SETTINGS, &settings, &dev);
#if UART_PRINTF_MODE && DEBUG_BME280
    bme280_error_codes_print_result("bme280_set_sensor_settings", rslt);
#endif
}

int8_t bme280_i2c_read(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *intf_ptr) {

    dev_addr = *(uint8_t*)intf_ptr;

    drv_i2c_read_series(dev_addr << 1, reg_addr, 1, reg_data, len);

    return 0;
}

int8_t bme280_i2c_write(uint8_t reg_addr, const uint8_t *reg_data, uint32_t len, void *intf_ptr) {

    dev_addr = *(uint8_t*)intf_ptr;

    drv_i2c_write_series(dev_addr << 1, reg_addr, 1, (uint8_t*)reg_data, len);

    return 0;
}

void bme280_delay_us(uint32_t period, void *intf_ptr) {
  sleep_us(period);
}

void app_bme280_measurement() {

    rslt = bme280_set_sensor_mode(BME280_POWERMODE_FORCED, &dev);
#if UART_PRINTF_MODE && DEBUG_BME280
    bme280_error_codes_print_result("bme280_set_power_mode", rslt);
#endif
    dev.delay_us(40000, dev.intf_ptr);

    rslt = bme280_get_sensor_data(BME280_ALL, &comp_data, &dev);
#if UART_PRINTF_MODE && DEBUG_BME280
    bme280_error_codes_print_result("bme280_get_sensor_data", rslt);
#endif

    if(rslt == BME280_OK) {
        float temperature = comp_data.temperature * 100;
        float humidity = comp_data.humidity * 100;
        float pressure = comp_data.pressure * 1000;

        temp = (int32_t)temperature;
        rh = (uint32_t)humidity;
        hpa = (uint32_t)pressure;

        zcl_setAttrVal(APP_ENDPOINT1, ZCL_CLUSTER_MS_TEMPERATURE_MEASUREMENT, ZCL_TEMPERATURE_MEASUREMENT_ATTRID_MEASUREDVALUE, (uint8_t*)&temp);
        zcl_setAttrVal(APP_ENDPOINT1, ZCL_CLUSTER_MS_RELATIVE_HUMIDITY, ZCL_ATTRID_HUMIDITY_MEASUREDVALUE, (uint8_t*)&rh);

#if UART_PRINTF_MODE && DEBUG_BME280
        printf("temperature: %d.%d C\r\n", temp/100, temp%100);
        printf("humidity: %d.%d% rh\r\n", rh/100, rh%100);
        printf("pressure: %d.%d hpa\r\n", hpa/100, hpa%100);
#endif
    }
}

