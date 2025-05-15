#ifndef SRC_SGP40_SGP40_H_
#define SRC_SGP40_SGP40_H_

#define SGP40_I2C_ADDRESS       0x59

typedef enum {
    SGP40_OK = 0,
    SGP40_ERROR,
    SGP40_ERR_NULL_PTR,
    SGP40_ERR_INIT_FAIL,
    SGP40_ERR_COMM_FAIL,
    SGP40_ERR_READ_FAIL,
    SGP40_ERR_WRITE_FAIL,
    SGP40_ERR_MODE_FAIL,
    SGP40_ERR_INVALID_LEN,
    SGP40_ERR_DEV_NOT_FOUND,
    SGP40_ERR_ADDR_NOT_FOUND,
    SGP40_ERR_CRC,
    SGP40_ERR_NOT_READY,
} sgp40_error_t;

struct _sgp40_dev;

typedef struct _sgp40_dev sgp40_dev_t;

typedef int8_t (*sgp40_read_t)(uint8_t *reg_data, uint32_t len, sgp40_dev_t *dev);
typedef int8_t (*sgp40_write_t)(uint16_t reg_addr, const uint8_t *reg_data, uint32_t len, sgp40_dev_t *dev);
typedef void   (*sgp40_delay_t)(uint32_t period);


struct _sgp40_dev {
    uint8_t         addr;
    sgp40_read_t    read;
    sgp40_write_t   write;
    sgp40_delay_t   delay;
};

sgp40_error_t sgp40_init(sgp40_dev_t *pdev);
sgp40_error_t sgp40_temperature_convert(float temp, uint16_t *raw_temperature);
sgp40_error_t sgp40_humidity_convert(float rh, uint16_t *raw_humidity);
sgp40_error_t sgp40_get_measure_raw(uint16_t raw_humidity, uint16_t raw_temperature, uint16_t *sraw_voc);
sgp40_error_t sgp40_soft_reset();

#endif /* SRC_SGP40_SGP40_H_ */
