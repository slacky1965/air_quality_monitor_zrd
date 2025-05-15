#include "tl_common.h"
#include "sgp40.h"

#define SGP40_CRC8_POLYNOMIAL                   0x31
#define SGP40_CRC8_INIT                         0xFF

#define SGP40_COMMAND_MEASURE_RAW               0x260F          /* measure raw command */
#define SGP40_COMMAND_EXECUTE_SELF_TEST         0x280E          /* execute self test command */
#define SGP40_COMMAND_TURN_HEATER_OFF           0x3615          /* turn heater off command */
#define SGP40_COMMAND_GET_SERIAL_ID             0x3682          /* get serial id command */
#define SGP40_COMMAND_SOFT_RESET                0x0006          /* soft reset command */

static sgp40_dev_t *dev = NULL;

static uint8_t sgp40_crc8(uint8_t *data, uint8_t len) {
  uint8_t crc = SGP40_CRC8_INIT;
  for (uint8_t x = 0; x < len; x++) {
    crc ^= data[x];
    for (uint8_t i = 0; i < 8; i++) {
      if ((crc & 0x80) != 0)
        crc = (uint8_t)((crc << 1) ^ SGP40_CRC8_POLYNOMIAL);
      else
        crc <<= 1;
    }
  }
  return crc;
}

sgp40_error_t sgp40_init(sgp40_dev_t *pdev) {

    dev = NULL;

    if (pdev == NULL)
        return SGP40_ERR_DEV_NOT_FOUND;

    if (!pdev->addr)
        return SGP40_ERR_ADDR_NOT_FOUND;

    if (!pdev->delay)
        return SGP40_ERR_NULL_PTR;

    if (!pdev->read)
        return SGP40_ERR_NULL_PTR;

    if (!pdev->write)
        return SGP40_ERR_NULL_PTR;

    dev = pdev;

    return SGP40_OK;
}

sgp40_error_t sgp40_temperature_convert(float temp, uint16_t *raw_temperature) {

    if (!dev)
        return SGP40_ERR_INIT_FAIL;

    *raw_temperature = (uint16_t)((temp + 45.0f) / 175.0f * 65535.0f);

    return SGP40_OK;
}

sgp40_error_t sgp40_humidity_convert(float rh, uint16_t *raw_humidity) {

    if (!dev)
        return SGP40_ERR_INIT_FAIL;

    *raw_humidity = (uint16_t)(rh / 100.0f * 65535.0f);

    return SGP40_OK;
}

sgp40_error_t sgp40_soft_reset() {

    uint8_t buf[1];
    buf[0] = 0x00;

    sgp40_error_t ret = dev->write(SGP40_COMMAND_SOFT_RESET, buf, 1, dev);

    dev->delay(5);

    return ret;
}

sgp40_error_t sgp40_get_measure_raw(uint16_t raw_humidity, uint16_t raw_temperature, uint16_t *sraw_voc) {

    uint8_t input[6];
    uint8_t buf[3];

    if (!dev)
        return SGP40_ERR_INIT_FAIL;

    memset(buf, 0, sizeof(buf));
    input[0] = (raw_humidity >> 8) & 0xFF;
    input[1] = (raw_humidity >> 0) & 0xFF;
    input[2] = sgp40_crc8(&input[0], 2);
    input[3] = (raw_temperature >> 8) & 0xFF;
    input[4] = (raw_temperature >> 0) & 0xFF;
    input[5] = sgp40_crc8(&input[3], 2);

    sgp40_error_t ret = dev->write(SGP40_COMMAND_MEASURE_RAW, input, 6, dev);
    if (ret != SGP40_OK)
        return SGP40_ERR_WRITE_FAIL;

    dev->delay(50);

    ret = dev->read(buf, 3, dev);
    if (ret != SGP40_OK)
        return SGP40_ERR_READ_FAIL;

    if (buf[2] != sgp40_crc8(buf, 2))
        return SGP40_ERR_CRC;

    *sraw_voc = (uint16_t)(((uint16_t)buf[0]) << 8 | buf[1]);

    return SGP40_OK;
}

