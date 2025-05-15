#include "app_main.h"

#include "sensirion_gas_index_algorithm.h"

static sgp40_dev_t sgp40_dev;
static GasIndexAlgorithmParams params;
static int32_t voc_index_value;

#if UART_PRINTF_MODE && DEBUG_SGP40
static void sgp40_error_codes_print_result(const char api_name[], int8_t rslt) {
    if (rslt != SGP40_OK)
    {
        printf("%s\t", api_name);

        switch (rslt)
        {
            case SGP40_ERR_NULL_PTR:
                printf("Error [%d] : Null pointer error.", rslt);
                printf(
                    "It occurs when the user tries to assign value (not address) to a pointer, which has been initialized to NULL.\r\n");
                break;

            case SGP40_ERR_COMM_FAIL:
                printf("Error [%d] : Communication failure error.", rslt);
                printf(
                    "It occurs due to read/write operation failure and also due to power failure during communication\r\n");
                break;

            case SGP40_ERR_DEV_NOT_FOUND:
                printf("Error [%d] : Device not found error. It occurs when the device chip id is incorrectly read\r\n",
                       rslt);
                break;

            case SGP40_ERR_ADDR_NOT_FOUND:
                printf("Error [%d] : Device not found at address. Address %d not correct\r\n",
                       rslt, SGP40_I2C_ADDRESS);
                break;

            case SGP40_ERR_INVALID_LEN:
                printf("Error [%d] : Invalid length error. It occurs when write is done with invalid length\r\n", rslt);
                break;

            case SGP40_ERR_CRC:
                printf("Error [%d] : Invalid crc\r\n", rslt);
                break;

            case SGP40_ERR_NOT_READY:
                printf("Error [%d] : Device not ready. Output buffer is empty\r\n", rslt);
                break;

            case SGP40_ERR_MODE_FAIL:
                printf("Error [%d] : The device cannot accept commands. The device is in periodic measurement mode.\r\n", rslt);
                break;

            default:
                printf("Error [%d] : Unknown error code\r\n", rslt);
                break;
        }
    }
}
#endif



int8_t sgp40_i2c_read(uint8_t *reg_data, uint32_t len, sgp40_dev_t *dev) {
    drv_i2c_read_series(dev->addr << 1, 0, 0, reg_data, len);
    return (reg_i2c_status & FLD_I2C_NAK);
}

//int8_t sgp40_i2c_read(uint16_t reg_addr, uint8_t *reg_data, uint32_t len, sgp40_dev_t *dev) {
//    drv_i2c_read_series(dev->addr << 1, reg_addr, 2, reg_data, len);
//    return (reg_i2c_status & FLD_I2C_NAK);
//}

int8_t sgp40_i2c_write(uint16_t reg_addr, const uint8_t *reg_data, uint32_t len, sgp40_dev_t *dev) {
    drv_i2c_write_series(dev->addr << 1, reg_addr, 2, (uint8_t*)reg_data, len);
    return (reg_i2c_status & FLD_I2C_NAK);
}

void sgp40_delay(uint32_t period) {
  sleep_ms(period);
}

static void app_sgp40_index(void *args) {

    uint16_t attr_len;
    int16_t temp;
    uint16_t rh, raw_temp, raw_rh;
    float f_temp, f_rh, index;

    raw_temp = 0x8000;
    raw_rh = 0x6666;

    status_t ret_temp = zcl_getAttrVal(APP_ENDPOINT1,
                                  ZCL_CLUSTER_MS_TEMPERATURE_MEASUREMENT,
                                  ZCL_TEMPERATURE_MEASUREMENT_ATTRID_MEASUREDVALUE,
                                  &attr_len,
                                  (uint8_t*)&temp);

    status_t ret_rh = zcl_getAttrVal(APP_ENDPOINT1,
                         ZCL_CLUSTER_MS_RELATIVE_HUMIDITY,
                         ZCL_ATTRID_HUMIDITY_MEASUREDVALUE,
                         &attr_len,
                         (uint8_t*)&rh);

    if (ret_temp == ZCL_STA_SUCCESS && ret_rh == ZCL_STA_SUCCESS) {

        if ((uint16_t)temp != 0x8000 && rh != 0xffff) {

           f_temp = (float)temp / 100.0;

           f_rh = (float)rh / 100.0;

           sgp40_temperature_convert(f_temp, &raw_temp);
           sgp40_humidity_convert(f_rh, &raw_rh);

        }
    }

    uint16_t sraw_voc;

    sgp40_error_t ret = sgp40_get_measure_raw(raw_rh, raw_temp, &sraw_voc);
#if UART_PRINTF_MODE && DEBUG_SGP40
    sgp40_error_codes_print_result("sgp40_init", ret);
#endif

    if (ret == SGP40_OK) {
        GasIndexAlgorithm_process(&params, sraw_voc, &voc_index_value);

        index = (float)voc_index_value;

        zcl_setAttrVal(APP_ENDPOINT1, ZCL_CLUSTER_GEN_ANALOG_INPUT_BASIC, ZCL_ANALOG_INPUT_ATTRID_PRESENT_VALUE, (uint8_t*)&index);
    }

//    printf("voc_index_value: %d\r\n", voc_index_value);
}

int32_t app_sgp40_indexTimerCb(void *args) {

    TL_SCHEDULE_TASK(app_sgp40_index, NULL);

    return 0;
}

uint8_t app_sgp40_init() {

    sgp40_dev.delay = NULL;
    sgp40_dev.read = NULL;
    sgp40_dev.write = NULL;
    sgp40_dev.addr = 0;

    uint8_t addr = (SGP40_I2C_ADDRESS << 1);

    uint8_t ret = scan_i2c_addr(addr);

    if (ret == addr) {
        sgp40_dev.addr = SGP40_I2C_ADDRESS;
        sgp40_dev.delay = sgp40_delay;
        sgp40_dev.read = sgp40_i2c_read;
        sgp40_dev.write = sgp40_i2c_write;
    }

    ret = sgp40_init(&sgp40_dev);
#if UART_PRINTF_MODE && DEBUG_SGP40
    sgp40_error_codes_print_result("sgp40_init", ret);
#endif

    GasIndexAlgorithm_init(&params, GasIndexAlgorithm_ALGORITHM_TYPE_VOC);

    TL_ZB_TIMER_SCHEDULE(app_sgp40_indexTimerCb, NULL, TIMEOUT_10SEC);


    return ret;
}

uint16_t app_sgp40_get_voc() {
    return voc_index_value;
}
