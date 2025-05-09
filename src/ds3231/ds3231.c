#include "tl_common.h"
#include "ds3231.h"

#define DS3231_REG_SECOND               0x00        /* second register */
#define DS3231_REG_MINUTE               0x01        /* minute register */
#define DS3231_REG_HOUR                 0x02        /* hour register */
#define DS3231_REG_WEEK                 0x03        /* week register */
#define DS3231_REG_DATE                 0x04        /* date register */
#define DS3231_REG_MONTH                0x05        /* month register */
#define DS3231_REG_YEAR                 0x06        /* year register */

#define DS1307_REG_CONTROL              0x07        /* control register */
#define DS1307_REG_RAM                  0x08        /* ram start register */

#define DS3231_REG_CONTROL              DS1307_REG_CONTROL
#define DS3231_REG_STATUS               DS1307_REG_RAM

//#define DS3231_REG_ALARM1_SECOND        0x07        /* alarm1 second register */
//#define DS3231_REG_ALARM1_MINUTE        0x08        /* alarm1 minute register */
//#define DS3231_REG_ALARM1_HOUR          0x09        /* alarm1 hour register */
//#define DS3231_REG_ALARM1_WEEK          0x0A        /* alarm1 week register */
//#define DS3231_REG_ALARM2_MINUTE        0x0B        /* alarm2 minute register */
//#define DS3231_REG_ALARM2_HOUR          0x0C        /* alarm2 hour register */
//#define DS3231_REG_ALARM2_WEEK          0x0D        /* alarm2 week register */
//#define DS3231_REG_CONTROL              0x0E        /* control register */
//#define DS3231_REG_STATUS               0x0F        /* status register */
//#define DS3231_REG_XTAL                 0x10        /* xtal register */
//#define DS3231_REG_TEMPERATUREH         0x11        /* temperature high register */
//#define DS3231_REG_TEMPERATUREL         0x12        /* temperature low register */

static ds3231_dev_t *dev = NULL;

static uint8_t ds3231_hex2bcd(uint8_t val) {

    uint8_t i, j, k;

    i = val / 10;
    j = val % 10;
    k = j + (i << 4);

    return k;
}

static uint8_t ds3231_bcd2hex(uint8_t val) {

    uint8_t temp;

    temp = val & 0x0F;
    val = (val >> 4) & 0x0F;
    val = val * 10;
    temp = temp + val;

    return temp;
}


uint8_t ds3231_init(ds3231_dev_t *pdev) {

    if (pdev == NULL)
        return DS3231_ERR_DEV_NOT_FOUND;

    if (!pdev->addr)
        return DS3231_ERR_ADDR_NOT_FOUND;

    if (!pdev->delay)
        return DS3231_ERR_NULL_PTR;

    if (!pdev->read)
        return DS3231_ERR_NULL_PTR;

    if (!pdev->write)
        return DS3231_ERR_NULL_PTR;

    dev = pdev;

    return DS3231_OK;
}

uint8_t ds3231_get_time(ds3231_time_t *t) {

    if (!dev)
        return DS3231_ERR_INIT_FAIL;

    uint8_t buf[7] = {0};

    uint8_t ret = dev->read(DS3231_REG_SECOND, buf, 7, dev);

    if (ret != DS3231_OK)
        return ret;

    t->year = ds3231_bcd2hex(buf[6]) + 2000;                    /* get year */
    t->month = ds3231_bcd2hex(buf[5] & 0x1F);                   /* get month */
    t->week = ds3231_bcd2hex(buf[3] & 0x7);                     /* get week */
    t->date = ds3231_bcd2hex(buf[4] & 0x3F);                    /* get date */
    t->am_pm = (ds3231_am_pm_t)((buf[2] >> 5) & 0x01);          /* get am pm */
    t->format = (ds3231_format_t)((buf[2] >> 6) & 0x01);        /* get format */
    if (t->format == DS3231_FORMAT_12H)                         /* if 12H */
    {
        t->hour = ds3231_bcd2hex(buf[2] & 0x1F);                /* get hour */
    }
    else
    {
        t->hour = ds3231_bcd2hex(buf[2] & 0x3F);                /* get hour */
    }
    t->minute = ds3231_bcd2hex(buf[1]);                         /* get minute */
    t->second = ds3231_bcd2hex(buf[0] & (~(1 << 7)));           /* get second */

    return ret;
}

uint8_t ds3231_set_time(ds3231_time_t *t) {

    if ((t->year < 2000) || (t->year > 2200))
        return DS3231_ERR_YEAR;

    if ((t->month == 0) || (t->month > 12))
        return DS3231_ERR_MONTH;

    if ((t->week == 0) || (t->week > 7))
        return DS3231_ERR_WEEK;

    if ((t->date == 0) || (t->date > 31))
        return DS3231_ERR_DATE;

    if (t->minute > 59)
        return DS3231_ERR_MINUTE;

    if (t->second > 59)
        return DS3231_ERR_SECOND;

    if (t->format == DS3231_FORMAT_12H) {
        if ((t->hour < 1) || (t->hour > 12)) {
            return DS3231_ERR_HOUR_12H;
        }
    }
    else if (t->format == DS3231_FORMAT_24H) {
        if (t->hour > 23)
            return DS3231_ERR_HOUR_24H;
    }
    else
    {
        return DS3231_ERR_FORMAT_FAIL;
    }

    uint8_t data[1] = {0};
    data[0] = ds3231_hex2bcd(t->second);
    uint8_t ret = dev->write(DS3231_REG_SECOND, data, 1, dev);
    if (ret != DS3231_OK)
        return DS3231_ERR_W_SECOND;

    data[0] = ds3231_hex2bcd(t->minute);
    ret = dev->write(DS3231_REG_MINUTE, data, 1, dev);
    if (ret != DS3231_OK)
        return DS3231_ERR_W_MINUTE;


    if (t->format == DS3231_FORMAT_12H) {
        data[0] = (uint8_t)((1 << 6) | (t->am_pm << 5) | ds3231_hex2bcd(t->hour));
    } else {
        data[0] = (0 << 6) | ds3231_hex2bcd(t->hour);
    }

    ret = dev->write(DS3231_REG_HOUR, data, 1, dev);
    if (ret != DS3231_OK)
        return DS3231_ERR_W_HOUR;

    data[0] = ds3231_hex2bcd(t->week);
    ret = dev->write(DS3231_REG_WEEK, data, 1, dev);
    if (ret != DS3231_OK)
        return DS3231_ERR_W_WEEK;

    data[0] = ds3231_hex2bcd(t->date);
    ret = dev->write(DS3231_REG_DATE, data, 1, dev);
    if (ret != DS3231_OK)
        return DS3231_ERR_W_DATE;

    uint16_t year = t->year - 2000;
    uint8_t century;
    if (year >= 100) {
        century = 1;
        year -= 100;
    } else {
        century = 0;
    }

    data[0] = ds3231_hex2bcd(t->month) | (century << 7);
    ret = dev->write(DS3231_REG_MONTH, data, 1, dev);
    if (ret != DS3231_OK)
        return DS3231_ERR_W_MONTH;

    data[0] = ds3231_hex2bcd((uint8_t)year);
    ret = dev->write(DS3231_REG_YEAR, data, 1, dev);
    if (ret != DS3231_OK)
        return DS3231_ERR_W_YEAR;

    return DS3231_OK;
}
