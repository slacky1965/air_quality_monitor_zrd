#include "app_main.h"

static ds3231_dev_t ds3231_dev;

static bool rtc_installed = false;

#if UART_PRINTF_MODE && DEBUG_DS3231
static void ds3231_error_codes_print_result(const char api_name[], int8_t rslt) {
    if (rslt != DS3231_OK)
    {
        printf("%s\t", api_name);

        switch (rslt)
        {
            case DS3231_ERR_NULL_PTR:
                printf("Error [%d] : Null pointer error.", rslt);
                printf(
                    "It occurs when the user tries to assign value (not address) to a pointer, which has been initialized to NULL.\r\n");
                break;

            case DS3231_ERR_COMM_FAIL:
                printf("Error [%d] : Communication failure error.", rslt);
                printf(
                    "It occurs due to read/write operation failure and also due to power failure during communication\r\n");
                break;

            case DS3231_ERR_DEV_NOT_FOUND:
                printf("Error [%d] : Device not found error. It occurs when the device chip id is incorrectly read\r\n",
                       rslt);
                break;

            case DS3231_ERR_ADDR_NOT_FOUND:
                printf("Error [%d] : Device not found at address. Address %d not correct\r\n",
                       rslt, DS3231_I2C_ADDRESS);
                break;

            case DS3231_ERR_INVALID_LEN:
                printf("Error [%d] : Invalid length error. It occurs when write is done with invalid length\r\n", rslt);
                break;

            case DS3231_ERR_CRC:
                printf("Error [%d] : Invalid crc\r\n", rslt);
                break;

            case DS3231_ERR_NOT_READY:
                printf("Error [%d] : Device not ready. Output buffer is empty\r\n", rslt);
                break;

            case DS3231_ERR_MODE_FAIL:
                printf("Error [%d] : The device cannot accept commands. The device is in periodic measurement mode.\r\n", rslt);
                break;

            case DS3231_ERR_SECOND:
                printf("Error [%d] : Second can't be over than 59.\r\n", rslt);
                break;

            case DS3231_ERR_W_SECOND:
                printf("Error [%d] : Write second failed.\r\n", rslt);
                break;

            case DS3231_ERR_MINUTE:
                printf("Error [%d] : Minute can't be over than 59.\r\n", rslt);
                break;

            case DS3231_ERR_W_MINUTE:
                printf("Error [%d] : Write minute failed.\r\n", rslt);
                break;

            case DS3231_ERR_HOUR_12H:
                printf("Error [%d] : Hour can't be over than 12 or less 1.\r\n", rslt);
                break;

            case DS3231_ERR_HOUR_24H:
                printf("Error [%d] : Hour can't be over than 23.\r\n", rslt);
                break;

            case DS3231_ERR_W_HOUR:
                printf("Error [%d] : Write hour failed.\r\n", rslt);
                break;

            case DS3231_ERR_WEEK:
                printf("Error [%d] : Week can't be zero or over than 7.\r\n", rslt);
                break;

            case DS3231_ERR_W_WEEK:
                printf("Error [%d] : Write week failed.\r\n", rslt);
                break;

            case DS3231_ERR_DATE:
                printf("Error [%d] : Date can't be zero or over than 31.\r\n", rslt);
                break;

            case DS3231_ERR_W_DATE:
                printf("Error [%d] : Write date failed.\r\n", rslt);
                break;

            case DS3231_ERR_MONTH:
                printf("Error [%d] : Month can't be zero or over than 12.\r\n", rslt);
                break;

            case DS3231_ERR_W_MONTH:
                printf("Error [%d] : Write month failed.\r\n", rslt);
                break;

            case DS3231_ERR_YEAR:
                printf("Error [%d] : Year can't be over 2200 or less than 2000.\r\n", rslt);
                break;

            case DS3231_ERR_W_YEAR:
                printf("Error [%d] : Write year failed.\r\n", rslt);
                break;

            case DS3231_ERR_FORMAT_FAIL:
                printf("Error [%d] : Format is invalid.\r\n", rslt);
                break;

            default:
                printf("Error [%d] : Unknown error code\r\n", rslt);
                break;
        }
    }
}
#endif

int8_t ds3231_i2c_read(uint16_t reg_addr, uint8_t *reg_data, uint32_t len, ds3231_dev_t *dev) {
    drv_i2c_read_series(dev->addr, reg_addr, 1, reg_data, len);
    return (reg_i2c_status & FLD_I2C_NAK);
}

int8_t ds3231_i2c_write(uint16_t reg_addr, const uint8_t *reg_data, uint32_t len, ds3231_dev_t *dev) {
    drv_i2c_write_series(dev->addr, reg_addr, 1, (uint8_t*)reg_data, len);
    return (reg_i2c_status & FLD_I2C_NAK);
}

void ds3231_delay(uint32_t period) {
  sleep_ms(period);
}



uint8_t app_ds3231_init() {
    ds3231_dev.delay = NULL;
    ds3231_dev.read = NULL;
    ds3231_dev.write = NULL;
    ds3231_dev.addr = 0;
    ds3231_dev.periodic_runnig = true;

    uint8_t addr = DS3231_I2C_ADDRESS;

    uint8_t ret = scan_i2c_addr(addr);

    if (ret == addr) {
        ds3231_dev.addr = DS3231_I2C_ADDRESS;
        ds3231_dev.delay = ds3231_delay;
        ds3231_dev.read = ds3231_i2c_read;
        ds3231_dev.write = ds3231_i2c_write;
    } else {
        ret = 0;
    }

    ret = ds3231_init(&ds3231_dev);
#if UART_PRINTF_MODE && DEBUG_DS3231
    ds3231_error_codes_print_result("ds3231_init", ret);
#endif

    ds3231_time_t t;
    ret = ds3231_get_time(&t);
#if UART_PRINTF_MODE && DEBUG_DS3231
    ds3231_error_codes_print_result("ds3231_get_time", ret);
#endif

    if (ret == DS3231_OK) {
        if (t.format == DS3231_FORMAT_12H) {
            t.format = DS3231_FORMAT_24H;
            ret = ds3231_set_time(&t);
#if UART_PRINTF_MODE && DEBUG_DS3231
            ds3231_error_codes_print_result("ds3231_set_time", ret);
#endif
        }
    }

    if (ret == DS3231_OK)
        rtc_installed = true;

    return ret;
}

uint8_t app_ds3231_set_time(ds3231_time_t *t) {

    uint8_t ret = ds3231_set_time(t);
#if UART_PRINTF_MODE && DEBUG_DS3231
    ds3231_error_codes_print_result("ds3231_set_time", ret);
#endif

    return ret;
}

uint8_t app_ds3231_get_time(ds3231_time_t *t) {

    uint8_t ret = ds3231_get_time(t);
#if UART_PRINTF_MODE && DEBUG_DS3231
    ds3231_error_codes_print_result("ds3231_get_time", ret);
#endif

    return ret;
}
