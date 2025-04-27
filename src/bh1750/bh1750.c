#include "bh1750.h"

uint8_t bh1750_init(bh1750_dev_t *dev) {

    if (dev == NULL)
        return BH1750_ERR_DEV_NOT_FOUND;

    if (!dev->addr) return BH1750_ERR_ADDR_NOT_FOUND;

    if (!dev->delay) return BH1750_ERR_NULL_PTR;

    if (!dev->read) return BH1750_ERR_NULL_PTR;

    if (!dev->write) return BH1750_ERR_NULL_PTR;

    uint8_t data = BH1750_COMMAND_POWER_ON;
    uint8_t ret = dev->write(&data, 1, dev);

    if (ret != BH1750_OK) {
        return BH1750_ERR_COMM_FAIL;
    }

    dev->delay(5);

    data = BH1750_COMMAND_RESET;
    ret = dev->write(&data, 1, dev);
    if (ret != BH1750_OK) {
        return BH1750_ERR_COMM_FAIL;
    }

    dev->delay(5);

    data = BH1750_COMMAND_CHANGE_MEASUREMENT_TIME_HIGH | ((69 >> 5) & 0x07);
    ret = dev->write(&data, 1, dev);
    if (ret != BH1750_OK) {
        return BH1750_ERR_COMM_FAIL;
    }

    data = BH1750_COMMAND_CHANGE_MEASUREMENT_TIME_LOW | ((69 >> 0) & 0x1F);
    ret = dev->write(&data, 1, dev);
    if (ret != BH1750_OK) {
        return BH1750_ERR_COMM_FAIL;
    }

    dev->delay(5);
    dev->resolution_mode = BH1750_MODE_HIGH_RESOLUTION_MODE;
    dev->t = 69;
    dev->inited = 1;


    return BH1750_OK;
}

uint8_t bh1750_single_read(bh1750_dev_t *dev, uint16_t *raw, float *lux) {
    uint8_t res;
    uint8_t data;
    uint8_t buf[2];

    if (dev == NULL) return BH1750_ERR_DEV_NOT_FOUND;

    if (dev->inited != 1) return BH1750_ERR_INIT_FAIL;

    if (!dev->addr) return BH1750_ERR_ADDR_NOT_FOUND;

    if (!dev->delay) return BH1750_ERR_NULL_PTR;

    if (!dev->read) return BH1750_ERR_NULL_PTR;

    if (!dev->write) return BH1750_ERR_NULL_PTR;

    if (dev->resolution_mode == BH1750_MODE_HIGH_RESOLUTION_MODE)
        data = BH1750_COMMAND_ONE_TIME_H_RESOLUTION_MODE;
    else if (dev->resolution_mode == BH1750_MODE_HIGH_RESOLUTION_MODE2)
        data = BH1750_COMMAND_ONE_TIME_H_RESOLUTION_MODE2;
    else if (dev->resolution_mode == BH1750_MODE_LOW_RESOLUTION_MODE)
        data = BH1750_COMMAND_ONE_TIME_L_RESOLUTION_MODE;
    else
        return BH1750_ERR_MODE_FAIL;

    res = dev->write(&data, 1, dev);
    if (res != BH1750_OK)
        return BH1750_ERR_COMM_FAIL;

    if (dev->resolution_mode == BH1750_MODE_HIGH_RESOLUTION_MODE) {
        dev->delay((uint32_t)(180.0f * (float)(dev->t) / 69.0f));
    } else if (dev->resolution_mode == BH1750_MODE_HIGH_RESOLUTION_MODE2) {
        dev->delay((uint32_t)(180.0f * (float)(dev->t) / 69.0f));
    } else {
        dev->delay((uint32_t)(24.0f * (float)(dev->t) / 69.0f));
    }

    res = dev->read(buf, 2, dev);
    if (res != BH1750_OK)
        return BH1750_ERR_COMM_FAIL;

    *raw = (((uint16_t)buf[0]) << 8) | buf[1];
    if (dev->resolution_mode == BH1750_MODE_HIGH_RESOLUTION_MODE) {
        *lux = (float)(*raw) / 1.2f * (69.0f / ((float)(dev->t)));
    } else if (dev->resolution_mode == BH1750_MODE_HIGH_RESOLUTION_MODE2) {
        *lux = (float)(*raw) / 1.2f * (69.0f / ((float)(dev->t))) / 2.0f;
    } else {
        *lux = (float)(*raw) / 1.2f * (69.0f / ((float)(dev->t)));
    }

    return BH1750_OK;
}


