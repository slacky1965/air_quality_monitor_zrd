#include "app_main.h"
#include "app_bme280.h"

/**************************************************************************************************
 *                                          CONSTANTS
 **************************************************************************************************/


static void bme280_waitUs(uint32_t us);
static void bme280_waitMs(uint32_t ms);

static void bme280_write8(uint8_t reg, uint8_t data);
static uint8_t bme280_read8(uint8_t reg);
static bool bme280_isReadingCalibration(void);
static void bme280_readCoefficients(void);
static uint16_t bme280_read16(uint8_t reg);
static uint16_t bme280_read16_LE(uint8_t reg);
static int16_t bme280_readS16(uint8_t reg);
static int16_t bme280_readS16_LE(uint8_t reg);
static uint32_t bme280_read24(uint8_t reg);
static void bme280_setSampling(sensor_mode mode, sensor_sampling tempSampling, sensor_sampling pressSampling, sensor_sampling humSampling, sensor_filter filter, standby_duration duration);
float bme280_readTemperature(void);
float bme280_readHumidity(void);
float bme280_readPressure(void);
void bme280_takeForcedMeasurement(void);
bool bme280_checkRegisterStatus(void);

int32_t t_fine;

/**************************************************************************/
/*!
    @brief  calibration data
*/
/**************************************************************************/
  uint16_t bme280_dig_T1; ///< temperature compensation value
  int16_t bme280_dig_T2;  ///< temperature compensation value
  int16_t bme280_dig_T3;  ///< temperature compensation value

  uint16_t bme280_dig_P1; ///< pressure compensation value
  int16_t bme280_dig_P2;  ///< pressure compensation value
  int16_t bme280_dig_P3;  ///< pressure compensation value
  int16_t bme280_dig_P4;  ///< pressure compensation value
  int16_t bme280_dig_P5;  ///< pressure compensation value
  int16_t bme280_dig_P6;  ///< pressure compensation value
  int16_t bme280_dig_P7;  ///< pressure compensation value
  int16_t bme280_dig_P8;  ///< pressure compensation value
  int16_t bme280_dig_P9;  ///< pressure compensation value

  uint8_t bme280_dig_H1; ///< humidity compensation value
  int16_t bme280_dig_H2; ///< humidity compensation value
  uint8_t bme280_dig_H3; ///< humidity compensation value
  int16_t bme280_dig_H4; ///< humidity compensation value
  int16_t bme280_dig_H5; ///< humidity compensation value
  int8_t bme280_dig_H6;  ///< humidity compensation value

static bool status = false;

bool bme280_init(void) {

//    int ret = scan_i2c_addr(0x99);
//    printf("bme280_init. ret: 0x%x\r\n", ret);
  // check if sensor, i.e. the chip ID is correct
  uint8_t chip = bme280_read8(BME280_REGISTER_CHIPID);
  printf("bme280_init. chip: 0x%x\r\n", chip);
  if (chip != 0x60) {
    return 0;
  }

  // reset the device using soft-reset
  // this makes sure the IIR is off, etc.
  bme280_write8(BME280_REGISTER_SOFTRESET, 0xB6);

  // wait for chip to wake up.
  bme280_waitUs(10);

  // if chip is still reading calibration, delay
  while (bme280_isReadingCalibration())
  bme280_waitUs(10);

  bme280_readCoefficients(); // read trimming parameters, see DS 4.2.2

//  bme280_setSampling(MODE_NORMAL, SAMPLING_X16, SAMPLING_X16, SAMPLING_X16, FILTER_OFF, STANDBY_MS_0_5); // use defaults
  bme280_setSampling(MODE_FORCED, SAMPLING_X2, SAMPLING_X16, SAMPLING_X1, FILTER_X16, STANDBY_MS_62_5); // air

  bme280_waitUs(100);

  return 1;
}

static void bme280_waitUs(uint32_t us) {
    sleep_us(us);
}

static void bme280_waitMs(uint32_t ms) {
    sleep_ms(ms);
}

static void bme280_write8(uint8_t reg, uint8_t data)
{
    uint8_t writeBuffer[2];
    writeBuffer[0] = reg;
    writeBuffer[1] = data;

//    i2cTransaction.targetAddress = BME280_I2CADDR;
//    i2cTransaction.writeBuf = writeBuffer;
//    i2cTransaction.writeCount = 2;
//    i2cTransaction.readBuf = NULL;
//    i2cTransaction.readCount = 0;
//
//    status = I2C_transfer(i2cHandle, &i2cTransaction);
//    if (status == false) {
//        // Unsuccessful I2C transfer
//        if (i2cTransaction.status == I2C_STATUS_ADDR_NACK) {
//            // I2C target address not acknowledged
//        }
//    }

    drv_i2c_write_series(BME280_I2CADDR << 1, 0, 0, writeBuffer, 2);
//    app_i2c_write_series(BME280_I2CADDR, writeBuffer, 2);
//    send_i2c_buf(BME280_I2CADDR, writeBuffer, 2);
}

static uint8_t bme280_read8(uint8_t reg)
{
    uint8_t readBuffer[1];
    uint8_t writeBuffer[1];

    writeBuffer[0] = reg;

//    i2cTransaction.targetAddress = BME280_I2CADDR;
//    i2cTransaction.writeBuf = writeBuffer;
//    i2cTransaction.writeCount = 1;
//    i2cTransaction.readBuf = readBuffer;
//    i2cTransaction.readCount = 1;
//
//    status = I2C_transfer(i2cHandle, &i2cTransaction);
//    if (status == false) {
//      if (i2cTransaction.status == I2C_STATUS_ADDR_NACK) {
//                // I2C target address not acknowledged
//      }
//    }


    drv_i2c_read_series(BME280_I2CADDR << 1, reg, 1, readBuffer, 1);

//    app_i2c_write_series(BME280_I2CADDR, writeBuffer, 1);
//    sleep_ms(40);
//    reg_i2c_mode |= FLD_I2C_HOLD_MASTER;// Enable clock stretching for Sensor
//    i2c_set_id(BME280_I2CADDR << 1);
//    i2c_read_series(0, 0, readBuffer, 1);
//    reg_i2c_mode &= ~FLD_I2C_HOLD_MASTER;// Disable clock stretching for Sensor


//    int ret = send_i2c_byte(BME280_I2CADDR, reg);
//    printf("bme280_read8. send_i2c_byte ret: %d\r\n", ret);
//    sleep_ms(240);
//    ret = read_i2c_buf(BME280_I2CADDR, readBuffer, 1);
//    printf("bme280_read8. read_i2c_buf ret: %d\r\n", ret);


    uint8_t value = readBuffer[0];
  return value;
}

static bool bme280_isReadingCalibration(void) {
  uint8_t const rStatus = bme280_read8(BME280_REGISTER_STATUS);
  return (rStatus & (1 << 0)) != 0;
}

static void bme280_readCoefficients(void) {
  bme280_dig_T1 = bme280_read16_LE(BME280_REGISTER_DIG_T1);
  bme280_dig_T2 = bme280_readS16_LE(BME280_REGISTER_DIG_T2);
  bme280_dig_T3 = bme280_readS16_LE(BME280_REGISTER_DIG_T3);

  bme280_dig_P1 = bme280_read16_LE(BME280_REGISTER_DIG_P1);
  bme280_dig_P2 = bme280_readS16_LE(BME280_REGISTER_DIG_P2);
  bme280_dig_P3 = bme280_readS16_LE(BME280_REGISTER_DIG_P3);
  bme280_dig_P4 = bme280_readS16_LE(BME280_REGISTER_DIG_P4);
  bme280_dig_P5 = bme280_readS16_LE(BME280_REGISTER_DIG_P5);
  bme280_dig_P6 = bme280_readS16_LE(BME280_REGISTER_DIG_P6);
  bme280_dig_P7 = bme280_readS16_LE(BME280_REGISTER_DIG_P7);
  bme280_dig_P8 = bme280_readS16_LE(BME280_REGISTER_DIG_P8);
  bme280_dig_P9 = bme280_readS16_LE(BME280_REGISTER_DIG_P9);

  bme280_dig_H1 = bme280_read8(BME280_REGISTER_DIG_H1);
  bme280_dig_H2 = bme280_readS16_LE(BME280_REGISTER_DIG_H2);
  bme280_dig_H3 = bme280_read8(BME280_REGISTER_DIG_H3);
  bme280_dig_H4 = ((int8_t)bme280_read8(BME280_REGISTER_DIG_H4) << 4) |
                         (bme280_read8(BME280_REGISTER_DIG_H4 + 1) & 0xF);
  bme280_dig_H5 = ((int8_t)bme280_read8(BME280_REGISTER_DIG_H5 + 1) << 4) |
                         (bme280_read8(BME280_REGISTER_DIG_H5) >> 4);
  bme280_dig_H6 = (int8_t)bme280_read8(BME280_REGISTER_DIG_H6);
}


static uint16_t bme280_read16(uint8_t reg) {
//    I2C_Transaction i2cTransaction = {0};
    uint8_t readBuffer[2];
    uint8_t writeBuffer[1];

    writeBuffer[0] = reg;

//    i2cTransaction.targetAddress = BME280_I2CADDR;
//    i2cTransaction.writeBuf = writeBuffer;
//    i2cTransaction.writeCount = 1;
//    i2cTransaction.readBuf = readBuffer;
//    i2cTransaction.readCount = 2;
//
//    status = I2C_transfer(i2cHandle, &i2cTransaction);
//
//    if (status == false) {
//        if (i2cTransaction.status == I2C_STATUS_ADDR_NACK) {
//            // I2C target address not acknowledged
//        }
//    }

    drv_i2c_read_series(BME280_I2CADDR << 1, reg, 1, readBuffer, 2);

    uint16_t value16 = ((readBuffer[0] << 8) | readBuffer[1]);

  return value16;
}

static uint16_t bme280_read16_LE(uint8_t reg) {
  uint16_t temp = bme280_read16(reg);
  return (temp >> 8) | (temp << 8);
}

static int16_t bme280_readS16(uint8_t reg) {
  return (int16_t)bme280_read16(reg);
}

static int16_t bme280_readS16_LE(uint8_t reg) {
  return (int16_t)bme280_read16_LE(reg);
}

static uint32_t bme280_read24(uint8_t reg) {
//    I2C_Transaction i2cTransaction = {0};
    uint8_t readBuffer[3];
    uint8_t writeBuffer[1];

    writeBuffer[0] = reg;

//    i2cTransaction.targetAddress = BME280_I2CADDR;
//    i2cTransaction.writeBuf = writeBuffer;
//    i2cTransaction.writeCount = 1;
//    i2cTransaction.readBuf = readBuffer;
//    i2cTransaction.readCount = 3;
//
//    status = I2C_transfer(i2cHandle, &i2cTransaction);
//
//    if (status == false) {
//        if (i2cTransaction.status == I2C_STATUS_ADDR_NACK) {
//            // I2C target address not acknowledged
//        }
//    }

    drv_i2c_read_series(BME280_I2CADDR << 1, reg, 1, readBuffer, 3);
    uint32_t value32 = ((readBuffer[0] << 12) | readBuffer[1] << 4 | readBuffer[2] >> 4);

  return value32;
}

/*!
 *   @brief  setup sensor with given parameters / settings
 *
 *   This is simply a overload to the normal begin()-function, so SPI users
 *   don't get confused about the library requiring an address.
 *   @param mode the power mode to use for the sensor
 *   @param tempSampling the temp samping rate to use
 *   @param pressSampling the pressure sampling rate to use
 *   @param humSampling the humidity sampling rate to use
 *   @param filter the filter mode to use
 *   @param duration the standby duration to use
 */
static void bme280_setSampling(sensor_mode mode, sensor_sampling tempSampling, sensor_sampling pressSampling, sensor_sampling humSampling, sensor_filter filter, standby_duration duration)
{

  unsigned int configRegspi3w_en = 0; ///< unused - don't set

  // making sure sensor is in sleep mode before setting configuration
  // as it otherwise may be ignored
  bme280_write8(BME280_REGISTER_CONTROL, MODE_SLEEP);

  // you must make sure to also set REGISTER_CONTROL after setting the
  // CONTROLHUMID register, otherwise the values won't be applied (see
  // DS 5.4.3)
  bme280_write8(BME280_REGISTER_CONTROLHUMID, humSampling);

  bme280_write8(BME280_REGISTER_CONFIG, (duration << 5) | (filter << 2) | configRegspi3w_en);

  bme280_write8(BME280_REGISTER_CONTROL, (tempSampling << 5) | (pressSampling << 2) | mode);

}

void bme280_takeForcedMeasurement(void) {
  // If we are in forced mode, the BME sensor goes back to sleep after each
  // measurement and we need to set it to forced mode once at this point, so
  // it will take the next measurement and then return to sleep again.
  // In normal mode simply does new measurements periodically.

    bme280_write8(BME280_REGISTER_CONTROL, (SAMPLING_X16 << 5) | (SAMPLING_X16 << 2) | MODE_FORCED);
}

bool bme280_checkRegisterStatus(void) {
    // wait until measurement has been completed, otherwise we would read
    // the values from the last measurement
     if (bme280_read8(BME280_REGISTER_STATUS) & 0x08) {
         return false;
     }
     return true;
}

float bme280_readTemperature(void)
{
    int32_t adc_T = bme280_read24(BME280_REGISTER_TEMPDATA);

    double var1;
    double var2;
    double temperature;
    double temperature_min = -40;
    double temperature_max = 85;

    var1 = ((double)adc_T) / 16384.0 - ((double)bme280_dig_T1) / 1024.0;
    var1 = var1 * ((double)bme280_dig_T2);
    var2 = (((double)adc_T) / 131072.0 - ((double)bme280_dig_T1) / 8192.0);
    var2 = (var2 * var2) * ((double)bme280_dig_T3);
    t_fine = (int32_t)(var1 + var2);
    temperature = (var1 + var2) / 5120.0;
    if (temperature < temperature_min)
    {
        temperature = temperature_min;
    }
    else if (temperature > temperature_max)
    {
        temperature = temperature_max;
    }

    return temperature;
}

float bme280_readHumidity(void)
{
//  bme280_readTemperature(); // must be done first to get t_fine

  int32 adc_H = bme280_read16(BME280_REGISTER_HUMIDDATA);

    double humidity;
    double humidity_min = 0.0;
    double humidity_max = 100.0;
    double var1;
    double var2;
    double var3;
    double var4;
    double var5;
    double var6;

    var1 = ((double)t_fine) - 76800.0;
    var2 = (((double)bme280_dig_H4) * 64.0 + (((double)bme280_dig_H5) / 16384.0) * var1);
    var3 = adc_H - var2;
    var4 = ((double)bme280_dig_H2) / 65536.0;
    var5 = (1.0 + (((double)bme280_dig_H3) / 67108864.0) * var1);
    var6 = 1.0 + (((double)bme280_dig_H6) / 67108864.0) * var1 * var5;
    var6 = var3 * var4 * (var5 * var6);
    humidity = var6 * (1.0 - ((double)bme280_dig_H1) * var6 / 524288.0);

    if (humidity > humidity_max)
    {
        humidity = humidity_max;
    }
    else if (humidity < humidity_min)
    {
        humidity = humidity_min;
    }

    return humidity ;
}

//float bme280_readPressure(void)
//{
////  bme280_readTemperature(); // must be done first to get t_fine
//  int32 adc_P = bme280_read24(BME280_REGISTER_PRESSUREDATA);
//    double var1;
//    double var2;
//    double var3;
//    double pressure;
//    double pressure_min = 30000.0;
//    double pressure_max = 110000.0;
//
//    var1 = ((double)t_fine / 2.0) - 64000.0;
//    var2 = var1 * var1 * ((double)bme280_dig_P6) / 32768.0;
//    var2 = var2 + var1 * ((double)bme280_dig_P5) * 2.0;
//    var2 = (var2 / 4.0) + (((double)bme280_dig_P4) * 65536.0);
//    var3 = ((double)bme280_dig_P3) * var1 * var1 / 524288.0;
//    var1 = (var3 + ((double)bme280_dig_P2) * var1) / 524288.0;
//    var1 = (1.0 + var1 / 32768.0) * ((double)bme280_dig_P1);
//
//    /* avoid exception caused by division by zero */
//    if (var1 > (0.0))
//    {
//        pressure = 1048576.0 - (double) adc_P;
//        pressure = (pressure - (var2 / 4096.0)) * 6250.0 / var1;
//        var1 = ((double)bme280_dig_P9) * pressure * pressure / 2147483648.0;
//        var2 = pressure * ((double)bme280_dig_P8) / 32768.0;
//        pressure = pressure + (var1 + var2 + ((double)bme280_dig_P7)) / 16.0;
//        if (pressure < pressure_min)
//        {
//            pressure = pressure_min;
//        }
//        else if (pressure > pressure_max)
//        {
//            pressure = pressure_max;
//        }
//    }
//    else /* Invalid case */
//    {
//        pressure = pressure_min;
//    }
//
//    return pressure / 100;
//}
