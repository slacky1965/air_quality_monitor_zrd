#ifndef SRC_SCD4X_SCD4X_H_
#define SRC_SCD4X_SCD4X_H_

#define SCD4X_I2C_ADDRESS           0x62
typedef enum {
    SCD4X_OK = 0,
    SCD4X_ERROR,
    SCD4X_ERR_NULL_PTR,
    SCD4X_ERR_INIT_FAIL,
    SCD4X_ERR_COMM_FAIL,
    SCD4X_ERR_MODE_FAIL,
    SCD4X_ERR_INVALID_LEN,
    SCD4X_ERR_DEV_NOT_FOUND,
    SCD4X_ERR_ADDR_NOT_FOUND,
    SCD4X_ERR_CRC,
    SCD4X_ERR_NOT_READY,
} scd4x_error_t;

struct _scd4x_dev;

typedef struct _scd4x_dev scd4x_dev_t;

typedef int8_t (*scd4x_read_t)(uint16_t reg_addr, uint8_t *reg_data, uint32_t len, scd4x_dev_t *dev);
typedef int8_t (*scd4x_write_t)(uint16_t reg_addr, const uint8_t *reg_data, uint32_t len, scd4x_dev_t *dev);
typedef void   (*scd4x_delay_t)(uint32_t period);


struct _scd4x_dev {
    uint8_t         addr;
    scd4x_read_t    read;
    scd4x_write_t   write;
    scd4x_delay_t   delay;
    uint8_t         type;
    bool            periodic_runnig;
};

typedef enum {
    SCD4X_TYPE_SCD40 = 0,
    SCD4X_TYPE_SCD41
} scd4x_type_t;

uint8_t scd4x_init(scd4x_dev_t *dev);
uint8_t scd4x_cmd_stop_periodic();
uint8_t scd4x_cmd_start_periodic();
uint8_t scd4x_cmd_wake_up();
uint8_t scd4x_cmd_reinit();
uint8_t scd4x_cmd_get_ambient_pressure(uint16_t *pressure);
uint8_t scd4x_cmd_set_ambient_pressure(uint16_t pressure);
uint8_t scd4x_cmd_read(uint16_t *co2, uint16_t *temperature, uint16_t *humidity);

#endif /* SRC_SCD4X_SCD4X_H_ */
