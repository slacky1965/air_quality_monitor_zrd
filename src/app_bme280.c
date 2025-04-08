#include "app_main.h"
#include "app_bme280.h"

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

    settings.osr_h = BME280_OVERSAMPLING_1X;
    settings.osr_p = BME280_OVERSAMPLING_16X;
    settings.osr_t = BME280_OVERSAMPLING_2X;
    settings.filter = BME280_FILTER_COEFF_16;

//    /* Setting the standby time */
//    settings.standby_time = BME280_STANDBY_TIME_0_5_MS;

    rslt = bme280_set_sensor_settings(BME280_SEL_ALL_SETTINGS, &settings, &dev);
    bme280_error_codes_print_result("bme280_set_sensor_settings", rslt);

}

int8_t bme280_i2c_read(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *intf_ptr) {

    dev_addr = *(uint8_t*)intf_ptr;

    printf("bme280_i2c_read. reg_addr: 0x%x\r\n", reg_addr);

//    drv_i2c_write_byte(dev_addr, I2C_SLAVE_ADDR, I2C_SLAVE_ADDR_LEN, reg_addr);
    drv_i2c_write_series(dev_addr, 0, 0, &reg_addr, 1);

    dev.delay_us(100, intf_ptr);

    drv_i2c_read_series(dev_addr, 0, 0, reg_data, len);
//    *reg_data = drv_i2c_read_byte(dev_addr, reg_addr, 1);

//    drv_i2c_read_series(dev_addr, reg_addr, 1, reg_data, len);
    printf("reg_data: 0x%x\r\n", *reg_data);
//    uint8_t buffer[6];
//    buffer[0] = reg_addr;
//    i2c_set_id(dev_addr);
//    i2c_write_series(0, 0, buffer, 1);
    return 0;
}

int8_t bme280_i2c_write(uint8_t reg_addr, const uint8_t *reg_data, uint32_t len, void *intf_ptr) {

    dev_addr = *(uint8_t*)intf_ptr;

    drv_i2c_write_series(dev_addr, reg_addr, 1, (uint8_t*)reg_data, len);
    return 0;
}

void bme280_delay_us(uint32_t period, void *intf_ptr) {
  sleep_us(period);
}

void app_test_bme280() {

    rslt = bme280_set_sensor_mode(BME280_POWERMODE_FORCED, &dev);
    bme280_error_codes_print_result("bme280_set_power_mode", rslt);

    dev.delay_us(40000, dev.intf_ptr);

    rslt = bme280_get_sensor_data(BME280_ALL, &comp_data, &dev);
    bme280_error_codes_print_result("bme280_get_sensor_data", rslt);

    if(rslt == BME280_OK) {
//        temperature = comp_data.temperature / 100.0; /* °C  */
//        humidity = comp_data.humidity / 1024.0; /* %   */
//        pressure = comp_data.pressure / 10000.0; /* hPa */
    }
}
