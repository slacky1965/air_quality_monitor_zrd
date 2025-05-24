#include "tl_common.h"
#include "scd4x.h"

#define SCD4X_CRC8_POLYNOMIAL       0x31
#define SCD4X_CRC8_INIT             0xFF

#define SCD4X_COMMAND_START_PERIODIC                                0x21B1      /* start periodic measurement command */
#define SCD4X_COMMAND_READ                                          0xEC05      /* read measurement command */
#define SCD4X_COMMAND_STOP_PERIODIC                                 0x3F86      /* stop periodic measurement command */
#define SCD4X_COMMAND_SET_TEMPERATURE_OFFSET                        0x241D      /* set temperature offset command */
#define SCD4X_COMMAND_GET_TEMPERAT RE_OFFSET                        0x2318      /* get temperature offset command */
#define SCD4X_COMMAND_SET_SENSOR_ALTITUDE                           0x2427      /* set sensor altitude command */
#define SCD4X_COMMAND_GET_SENSOR_ALTITUDE                           0x2322      /* get sensor altitude command */
#define SCD4X_COMMAND_SET_AMBIENT_PRESSURE                          0xE000      /* set ambient pressure command */
#define SCD4X_COMMAND_GET_AMBIENT_PRESSURE                          0xE000      /* get ambient pressure command */
#define SCD4X_COMMAND_PERFORM_FORCED_RECALIBRATION                  0x362F      /* perform forced recalibration command */
#define SCD4X_COMMAND_SET_AUTO_SELF_CALIBRATION                     0x2416      /* set automatic self calibration enabled command */
#define SCD4X_COMMAND_GET_AUTO_SELF_CALIBRATION                     0x2313      /* get automatic self calibration enabled command */
#define SCD4X_COMMAND_START_LOW_POWER_PERIODIC                      0x21AC      /* start low power periodic measurement command */
#define SCD4X_COMMAND_GET_DATA_READY_STATUS                         0xE4B8      /* get data ready status command */
#define SCD4X_COMMAND_PERSIST_SETTINGS                              0x3615      /* persist settings command */
#define SCD4X_COMMAND_GET_SERIAL_NUMBER                             0x3682      /* get serial number command */
#define SCD4X_COMMAND_PERFORM_SELF_TEST                             0x3639      /* perform self test command */
#define SCD4X_COMMAND_PERFORM_FACTORY_RESET                         0x3632      /* perform factory reset command */
#define SCD4X_COMMAND_REINIT                                        0x3646      /* reinit command */
#define SCD4X_COMMAND_MEASURE_SINGLE_SHOT                           0x219D      /* measure single shot command */
#define SCD4X_COMMAND_MEASURE_SINGLE_SHOT_RHT_ONLY                  0x2196      /* measure single shot rht only command */
#define SCD4X_COMMAND_POWER_DOWN                                    0x36E0      /* power down command */
#define SCD4X_COMMAND_WAKE_UP                                       0x36F6      /* wake up command */
#define SCD4X_COMMAND_SET_AUTO_SELF_CALIBRATION_INIT_PERIOD         0x2445      /* set automatic self calibration initial period command */
#define SCD4X_COMMAND_GET_AUTO_SELF_CALIBRATION_INIT_PERIOD         0x2340      /* get automatic self calibration initial period command */
#define SCD4X_COMMAND_SET_AUTO_SELF_CALIBRATION_STANDARD_PERIOD     0x244E      /* set automatic self calibration standard period command */
#define SCD4X_COMMAND_GET_AUTO_SELF_CALIBRATION_STANDARD_PERIOD     0x234B      /* get automatic self calibration standard period command */

static scd4x_dev_t *dev = NULL;

static scd4x_error_t scd4x_crc8(uint8_t *data, uint8_t len) {
  uint8_t crc = SCD4X_CRC8_INIT;
  for (uint8_t x = 0; x < len; x++) {
    crc ^= data[x];
    for (uint8_t i = 0; i < 8; i++) {
      if ((crc & 0x80) != 0)
        crc = (uint8_t)((crc << 1) ^ SCD4X_CRC8_POLYNOMIAL);
      else
        crc <<= 1;
    }
  }
  return crc;
}

static bool scd4x_cmd_get_data_ready_status() {

    if (!dev)
        return SCD4X_ERR_INIT_FAIL;

    uint8_t buf[3] = {0};
    uint16_t status;

    scd4x_error_t ret = dev->read(SCD4X_COMMAND_GET_DATA_READY_STATUS, 2, buf, 3, dev);

    if (ret == SCD4X_OK) {
        dev->delay(1);
        if (buf[2] != scd4x_crc8(buf, 2)) {
            return false;
        } else {
            status = (uint16_t)buf[0] << 8 | buf[1];
        }

        if ((status & 0x07ff) == 0x0000)
            return false;

        return true;

    } else {
        return false;
    }

    return false;
}

scd4x_error_t scd4x_cmd_read(uint16_t *co2, uint16_t *temperature, uint16_t *humidity) {

    if (!dev)
        return SCD4X_ERR_INIT_FAIL;

    if (!scd4x_cmd_get_data_ready_status()) {
        return SCD4X_ERR_NOT_READY;
    }

    uint8_t buf[9] = {0};
    uint8_t *ptr = buf;
    uint16_t raw, r_co2;
    float r_temp, r_hum;

    scd4x_error_t ret = dev->read(SCD4X_COMMAND_READ, 2, buf, 9, dev);

    if (ret == SCD4X_OK) {
        dev->delay(1);

        raw = *ptr << 8 | *(ptr+1);
        if (*(ptr+2) != scd4x_crc8(ptr, 2)) {
            return SCD4X_ERR_CRC;
        }
        r_co2 = raw;
        ptr += 3;

        raw = *ptr << 8 | *(ptr+1);
        if (*(ptr+2) != scd4x_crc8(ptr, 2)) {
            return SCD4X_ERR_CRC;
        }
        r_temp = (float)raw / 0xFFFF * 175 + (-45);
        ptr += 3;

        raw = *ptr << 8 | *(ptr+1);
        if (*(ptr+2) != scd4x_crc8(ptr, 2)) {
            return SCD4X_ERR_CRC;
        }
        r_hum = (float)raw / 0xFFFF * 100;
        ptr += 3;

        if (co2) *co2 = r_co2;
        if (temperature) *temperature = r_temp * 100;
        if (humidity) *humidity = r_hum * 100;
    }

    return ret;
}

scd4x_error_t scd4x_cmd_stop_periodic() {

    if (!dev)
        return SCD4X_ERR_INIT_FAIL;

    uint8_t buf[1] = {0};

    scd4x_error_t ret = dev->write(SCD4X_COMMAND_STOP_PERIODIC, buf, 0, dev);

    if (ret == SCD4X_OK) {
        dev->delay(500);
    }

    return ret;
}

scd4x_error_t scd4x_cmd_start_periodic() {

    if (!dev)
        return SCD4X_ERR_INIT_FAIL;

    uint8_t buf[1] = {0};

    scd4x_error_t ret = dev->write(SCD4X_COMMAND_START_PERIODIC, buf, 0, dev);

    return ret;
}

scd4x_error_t scd4x_cmd_wake_up() {

    if (!dev)
        return SCD4X_ERR_INIT_FAIL;

    uint8_t buf[1] = {0};

    scd4x_error_t ret = dev->write(SCD4X_COMMAND_WAKE_UP, buf, 0, dev);

    if (ret == SCD4X_OK) {
        dev->delay(30);
    }

    return ret;
}

scd4x_error_t scd4x_cmd_reinit() {

    if (!dev)
        return SCD4X_ERR_INIT_FAIL;

    if (dev->periodic_runnig)
        return SCD4X_ERR_MODE_FAIL;

    uint8_t buf[1] = {0};

    scd4x_error_t ret = dev->write(SCD4X_COMMAND_REINIT, buf, 0, dev);

    if (ret == SCD4X_OK) {
        dev->delay(30);
    }

    return ret;
}

scd4x_error_t scd4x_cmd_get_ambient_pressure(uint16_t *pressure) {

    if (!dev)
        return SCD4X_ERR_INIT_FAIL;

    if (dev->periodic_runnig)
        return SCD4X_ERR_MODE_FAIL;

    uint8_t buf[3] = {0};
    scd4x_error_t ret = dev->read(SCD4X_COMMAND_GET_AMBIENT_PRESSURE, 2, buf, 3, dev);

    if (ret == SCD4X_OK) {
        dev->delay(1);

        if (buf[2] != scd4x_crc8(buf, 2)) {
            ret = SCD4X_ERR_CRC;
        } else {
            *pressure = (uint16_t)buf[0] << 8 | buf[1];
        }
    }

    return ret;
}

scd4x_error_t scd4x_cmd_set_ambient_pressure(uint16_t pressure) {

    if (!dev)
        return SCD4X_ERR_INIT_FAIL;

    uint8_t buf[3] = {0};
    buf[0] = (pressure >> 8) & 0xFF;
    buf[1] = pressure & 0xFF;
    buf[2] = scd4x_crc8(buf, 2);
    scd4x_error_t ret = dev->write(SCD4X_COMMAND_GET_AMBIENT_PRESSURE, buf, 3, dev);

    if (ret == SCD4X_OK) {
        dev->delay(1);
    }

    return ret;
}

scd4x_error_t scd4x_cmd_forced_calibration(int16_t *frc) {

    if (!dev)
        return SCD4X_ERR_INIT_FAIL;

    uint16_t out_ppm, inp_ppm = 0x01e0;
    int16_t frc_t = 0;

    uint8_t buf[3] = {0};
    buf[0] = (inp_ppm >> 8) & 0xFF;
    buf[1] = inp_ppm & 0xFF;
    buf[2] = scd4x_crc8(buf, 2);
    scd4x_error_t ret = dev->write(SCD4X_COMMAND_PERFORM_FORCED_RECALIBRATION, buf, 3, dev);

    if (ret == SCD4X_OK) {
        dev->delay(400);

        ret = dev->read(0, 0, buf, 3, dev);

        if (ret == SCD4X_OK) {
            if (buf[2] != scd4x_crc8(buf, 2)) {
                ret = SCD4X_ERR_CRC;
            } else {
                out_ppm = (uint16_t) buf[0] << 8 | buf[1];
                if (out_ppm == 0xffff)
                    ret = SCD4X_ERROR;
                else
                    frc_t = 0x8000 - out_ppm;
                printf("outppm: %d, %04x\r\n", out_ppm, out_ppm);
            }
        }
    }

    *frc = frc_t;
    return ret;
}

scd4x_error_t scd4x_factory_reset() {

    if (!dev)
        return SCD4X_ERR_INIT_FAIL;

    uint8_t buf[1] = {0};
    scd4x_error_t ret = dev->write(SCD4X_COMMAND_PERFORM_FACTORY_RESET, buf, 0, dev);

    if (ret == SCD4X_OK)
        dev->delay(1200);

    return ret;
}

scd4x_error_t scd4x_init(scd4x_dev_t *pdev) {

    if (pdev == NULL)
        return SCD4X_ERR_DEV_NOT_FOUND;

    if (!pdev->addr)
        return SCD4X_ERR_ADDR_NOT_FOUND;

    if (!pdev->delay)
        return SCD4X_ERR_NULL_PTR;

    if (!pdev->read)
        return SCD4X_ERR_NULL_PTR;

    if (!pdev->write)
        return SCD4X_ERR_NULL_PTR;

    dev = pdev;

    return SCD4X_OK;
}
