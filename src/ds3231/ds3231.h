#ifndef SRC_DS3231_DS3231_H_
#define SRC_DS3231_DS3231_H_

#define DS3231_I2C_ADDRESS        0xD0

typedef enum {
    DS3231_OK = 0,
    DS3231_ERROR,
    DS3231_ERR_NULL_PTR,
    DS3231_ERR_INIT_FAIL,
    DS3231_ERR_COMM_FAIL,
    DS3231_ERR_MODE_FAIL,
    DS3231_ERR_INVALID_LEN,
    DS3231_ERR_DEV_NOT_FOUND,
    DS3231_ERR_ADDR_NOT_FOUND,
    DS3231_ERR_CRC,
    DS3231_ERR_NOT_READY,
    DS3231_ERR_SECOND,
    DS3231_ERR_W_SECOND,
    DS3231_ERR_MINUTE,
    DS3231_ERR_W_MINUTE,
    DS3231_ERR_HOUR_12H,
    DS3231_ERR_HOUR_24H,
    DS3231_ERR_W_HOUR,
    DS3231_ERR_WEEK,
    DS3231_ERR_W_WEEK,
    DS3231_ERR_DATE,
    DS3231_ERR_W_DATE,
    DS3231_ERR_MONTH,
    DS3231_ERR_W_MONTH,
    DS3231_ERR_YEAR,
    DS3231_ERR_W_YEAR,
    DS3231_ERR_FORMAT_FAIL,
} ds3231_error_t;

struct _ds3231_dev;

typedef struct _ds3231_dev ds3231_dev_t;

typedef int8_t (*ds3231_read_t)(uint16_t reg_addr, uint8_t *reg_data, uint32_t len, ds3231_dev_t *dev);
typedef int8_t (*ds3231_write_t)(uint16_t reg_addr, const uint8_t *reg_data, uint32_t len, ds3231_dev_t *dev);
typedef void   (*ds3231_delay_t)(uint32_t period);


struct _ds3231_dev {
    uint8_t         addr;
    ds3231_read_t   read;
    ds3231_write_t  write;
    ds3231_delay_t  delay;
    uint8_t         type;
    bool            periodic_runnig;
};

typedef enum
{
    DS3231_AM = 0x00,           /* am */
    DS3231_PM = 0x01,           /* pm */
} ds3231_am_pm_t;

typedef enum
{
    DS3231_FORMAT_12H = 0x01,   /* 12h format */
    DS3231_FORMAT_24H = 0x00,   /* 24h format */
} ds3231_format_t;

typedef struct {
    uint16_t year;              /* year */
    uint8_t  month;             /* month */
    uint8_t  week;              /* week */
    uint8_t  date;              /* date */
    uint8_t  hour;              /* hour */
    uint8_t  minute;            /* minute */
    uint8_t  second;            /* second */
    ds3231_format_t format;     /* data format */
    ds3231_am_pm_t am_pm;       /* am pm */
} ds3231_time_t;

uint8_t ds3231_init(ds3231_dev_t *pdev);
uint8_t ds3231_get_time(ds3231_time_t *t);
uint8_t ds3231_set_time(ds3231_time_t *t);


#endif /* SRC_DS3231_DS3231_H_ */
