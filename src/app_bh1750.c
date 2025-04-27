#include "app_main.h"
#include "app_bh1750.h"

bh1750_dev_t bh1750_dev;

uint16_t raw;
float lux;

#if UART_PRINTF_MODE && DEBUG_BH1750
static void bh1750_error_codes_print_result(const char api_name[], int8_t rslt) {
    if (rslt != BME280_OK)
    {
        printf("%s\t", api_name);

        switch (rslt)
        {
            case BH1750_ERR_NULL_PTR:
                printf("Error [%d] : Null pointer error.", rslt);
                printf(
                    "It occurs when the user tries to assign value (not address) to a pointer, which has been initialized to NULL.\r\n");
                break;

            case BH1750_ERR_COMM_FAIL:
                printf("Error [%d] : Communication failure error.", rslt);
                printf(
                    "It occurs due to read/write operation failure and also due to power failure during communication\r\n");
                break;

            case BH1750_ERR_DEV_NOT_FOUND:
                printf("Error [%d] : Device not found error. It occurs when the device chip id is incorrectly read\r\n",
                       rslt);
                break;

            case BH1750_ERR_ADDR_NOT_FOUND:
                printf("Error [%d] : Device not found at address. Address %d or %d not correct\r\n",
                       rslt, BH1750_I2C_ADDR_LOW, BH1750_I2C_ADDR_HIGH);
                break;

            case BH1750_ERR_INVALID_LEN:
                printf("Error [%d] : Invalid length error. It occurs when write is done with invalid length\r\n", rslt);
                break;

            default:
                printf("Error [%d] : Unknown error code\r\n", rslt);
                break;
        }
    }
}
#endif



int8_t bh1750_i2c_read(uint8_t *reg_data, uint32_t len, bh1750_dev_t *dev) {
    drv_i2c_read_series(dev->addr, 0, 0, reg_data, len);
    return (reg_i2c_status & FLD_I2C_NAK);
}

int8_t bh1750_i2c_write(const uint8_t *data, uint32_t len, bh1750_dev_t *dev) {
    drv_i2c_write_series(dev->addr, 0, 0, (uint8_t*)data, len);
    return (reg_i2c_status & FLD_I2C_NAK);
}

void bh1750_delay(uint32_t period) {
  sleep_ms(period);
}


uint8_t app_bh1750_init() {

    bh1750_dev.delay = NULL;
    bh1750_dev.read = NULL;
    bh1750_dev.write = NULL;
    bh1750_dev.resolution_mode = BH1750_MODE_LOW_RESOLUTION_MODE;
    bh1750_dev.addr = 0;
    bh1750_dev.inited = 0;

    int8_t ret = scan_i2c_addr(BH1750_I2C_ADDR_LOW);

    if (ret == BH1750_I2C_ADDR_LOW) {
        bh1750_dev.addr = BH1750_I2C_ADDR_LOW;
    } else {
        ret = scan_i2c_addr(BH1750_I2C_ADDR_HIGH);
        if (ret == BH1750_I2C_ADDR_HIGH) {
            bh1750_dev.addr = BH1750_I2C_ADDR_HIGH;
        } else {
            ret = 0;
        }
    }

    if (ret) {
        bh1750_dev.addr = ret;
        bh1750_dev.delay = bh1750_delay;
        bh1750_dev.read = bh1750_i2c_read;
        bh1750_dev.write = bh1750_i2c_write;
    }

    ret = bh1750_init(&bh1750_dev);
#if UART_PRINTF_MODE && DEBUG_BH1750
    bh1750_error_codes_print_result("bh1750_init", ret);
#endif

    return ret;
}

void app_bh1750_measurement() {

    int8_t ret = bh1750_single_read(&bh1750_dev, &raw, &lux);

    if (ret == BH1750_OK) {

        if (lux == 0)
            lux++;

        uint16_t value =  10000 * log10(lux) + 1;

#if UART_PRINTF_MODE && DEBUG_BH1750
        uint32_t ll = lux * 100;
        printf("raw: %d, lux: %d.%d, zcl: %d\r\n", raw, ll / 100, ll % 100, value);
#endif

        zcl_setAttrVal(APP_ENDPOINT1, ZCL_CLUSTER_MS_ILLUMINANCE_MEASUREMENT, ZCL_ATTRID_MEASURED_VALUE, (uint8_t*)&value);

        return;
    }

#if UART_PRINTF_MODE && DEBUG_BH1750
    bh1750_error_codes_print_result("app_bh1750_measurement", ret);
#endif

}
