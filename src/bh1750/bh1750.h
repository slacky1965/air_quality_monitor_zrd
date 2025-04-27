#ifndef SRC_BH1750_BH1750_H_
#define SRC_BH1750_BH1750_H_

#include "app_main.h"

#define BH1750_I2C_ADDR_LOW                                 0x46
#define BH1750_I2C_ADDR_HIGH                                0xB8

#define BH1750_MODE_HIGH_RESOLUTION_MODE                    0x00        /* 1lx   */
#define BH1750_MODE_HIGH_RESOLUTION_MODE2                   0x01        /* 0.5lx */
#define BH1750_MODE_LOW_RESOLUTION_MODE                     0x02        /* 4lx   */

#define BH1750_COMMAND_POWER_DOWN                           0x00        /* power down command */
#define BH1750_COMMAND_POWER_ON                             0x01        /* power on command */
#define BH1750_COMMAND_RESET                                0x07        /* reset command */
#define BH1750_COMMAND_CONTINUOUSLY_H_RESOLUTION_MODE       0x10        /* continuously h-resolution mode command */
#define BH1750_COMMAND_CONTINUOUSLY_H_RESOLUTION_MODE2      0x11        /* continuously h-resolution mode2 command */
#define BH1750_COMMAND_CONTINUOUSLY_L_RESOLUTION_MODE       0x13        /* continuously l-resolution mode command */
#define BH1750_COMMAND_ONE_TIME_H_RESOLUTION_MODE           0x20        /* one time h-resolution mode command */
#define BH1750_COMMAND_ONE_TIME_H_RESOLUTION_MODE2          0x21        /* one time h-resolution mode2 command */
#define BH1750_COMMAND_ONE_TIME_L_RESOLUTION_MODE           0x23        /* one time l-resolution mode command */
#define BH1750_COMMAND_CHANGE_MEASUREMENT_TIME_HIGH         0x40        /* change measurement time high command */
#define BH1750_COMMAND_CHANGE_MEASUREMENT_TIME_LOW          0x60        /* change measurement time low command */

typedef enum {
    BH1750_OK = 0,
    BH1750_ERROR,
    BH1750_ERR_NULL_PTR,
    BH1750_ERR_INIT_FAIL,
    BH1750_ERR_COMM_FAIL,
    BH1750_ERR_MODE_FAIL,
    BH1750_ERR_INVALID_LEN,
    BH1750_ERR_DEV_NOT_FOUND,
    BH1750_ERR_ADDR_NOT_FOUND,
} bh1750_error_t;

struct _bh1750_dev;

typedef struct _bh1750_dev bh1750_dev_t;

typedef int8_t (*bh1750_read_t)(uint8_t *reg_data, uint32_t len, bh1750_dev_t *dev);
//typedef int8_t (*bh1750_write_t)(uint8_t reg_addr, const uint8_t *reg_data, uint32_t len, bh1750_dev_t *dev);
typedef int8_t (*bh1750_write_t)(const uint8_t *reg_data, uint32_t len, bh1750_dev_t *dev);
typedef void (*bh1750_delay_t)(uint32_t period);

struct _bh1750_dev {
    uint8_t         addr;
    bh1750_read_t   read;
    bh1750_write_t  write;
    bh1750_delay_t  delay;
    uint8_t         resolution_mode;
    uint8_t         t;
    uint8_t         inited;
};

uint8_t bh1750_init(bh1750_dev_t *dev);
uint8_t bh1750_single_read(bh1750_dev_t *dev, uint16_t *raw, float *lux);

#endif /* SRC_BH1750_BH1750_H_ */
