#ifndef SRC_INCLUDE_APP_BME280_H_
#define SRC_INCLUDE_APP_BME280_H_

#define BME280_I2CADDR 0x76 // SDO connect GND
//#define BME280_I2CADDR 0x77 // SDO connect VDDIO

/**************************************************************************/
/*!
      @brief  sampling rates
*/
/**************************************************************************/
enum sensor_sampling {
    SAMPLING_NONE = 0x00, //0b000,
    SAMPLING_X1   = 0x01, //0b001,
    SAMPLING_X2   = 0x02, //0b010,
    SAMPLING_X4   = 0x03, //0b011,
    SAMPLING_X8   = 0x04, //0b100,
    SAMPLING_X16  = 0x05 //0b101
};

  /**************************************************************************/
  /*!
      @brief  power modes
  */
  /**************************************************************************/
enum sensor_mode {
    MODE_SLEEP  = 0x00, //0b00,
    MODE_FORCED = 0x01, //0b01,
    MODE_NORMAL = 0x03 //0b11
};

  /**************************************************************************/
  /*!
      @brief  filter values
  */
  /**************************************************************************/
enum sensor_filter {
    FILTER_OFF = 0x00, //0b000,
    FILTER_X2  = 0x01, //0b001,
    FILTER_X4  = 0x02, //0b010,
    FILTER_X8  = 0x03, //0b011,
    FILTER_X16 = 0x04 //0b100
};

  /**************************************************************************/
  /*!
      @brief  standby duration in ms
  */
  /**************************************************************************/
enum standby_duration {
    STANDBY_MS_0_5  = 0x00, //0b000,
    STANDBY_MS_10   = 0x06, //0b110,
    STANDBY_MS_20   = 0x07, //0b111,
    STANDBY_MS_62_5 = 0x01, //0b001,
    STANDBY_MS_125  = 0x02, //0b010,
    STANDBY_MS_250  = 0x03, //0b011,
    STANDBY_MS_500  = 0x04, //0b100,
    STANDBY_MS_1000 = 0x05 //0b101
};

/*!
 *  @brief Register addresses
 */
enum {
  BME280_REGISTER_DIG_T1 = 0x88,
  BME280_REGISTER_DIG_T2 = 0x8A,
  BME280_REGISTER_DIG_T3 = 0x8C,

  BME280_REGISTER_DIG_P1 = 0x8E,
  BME280_REGISTER_DIG_P2 = 0x90,
  BME280_REGISTER_DIG_P3 = 0x92,
  BME280_REGISTER_DIG_P4 = 0x94,
  BME280_REGISTER_DIG_P5 = 0x96,
  BME280_REGISTER_DIG_P6 = 0x98,
  BME280_REGISTER_DIG_P7 = 0x9A,
  BME280_REGISTER_DIG_P8 = 0x9C,
  BME280_REGISTER_DIG_P9 = 0x9E,

  BME280_REGISTER_DIG_H1 = 0xA1,
  BME280_REGISTER_DIG_H2 = 0xE1,
  BME280_REGISTER_DIG_H3 = 0xE3,
  BME280_REGISTER_DIG_H4 = 0xE4,
  BME280_REGISTER_DIG_H5 = 0xE5,
  BME280_REGISTER_DIG_H6 = 0xE7,

  BME280_REGISTER_CHIPID = 0xD0,
  BME280_REGISTER_VERSION = 0xD1,
  BME280_REGISTER_SOFTRESET = 0xE0,

  BME280_REGISTER_CAL26 = 0xE1, // R calibration stored in 0xE1-0xF0

  BME280_REGISTER_CONTROLHUMID = 0xF2,
  BME280_REGISTER_STATUS = 0XF3,
  BME280_REGISTER_CONTROL = 0xF4,
  BME280_REGISTER_CONFIG = 0xF5,
  BME280_REGISTER_PRESSUREDATA = 0xF7,
  BME280_REGISTER_TEMPDATA = 0xFA,
  BME280_REGISTER_HUMIDDATA = 0xFD
};

typedef uint8_t sensor_mode;
typedef uint8_t sensor_sampling;
typedef uint8_t sensor_filter;
typedef uint8_t standby_duration;

extern bool bme280_init(void);
extern float bme280_readTemperature(void);
//extern float bme280_readHumidity(void);
//extern float bme280_readPressure(void);
extern void bme280_takeForcedMeasurement(void);
extern bool bme280_checkRegisterStatus(void);

#endif /* SRC_INCLUDE_APP_BME280_H_ */
