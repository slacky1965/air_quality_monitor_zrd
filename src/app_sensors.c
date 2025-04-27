#include "app_main.h"

static void mesurement_bme280(void *args) {
    app_bme280_measurement();
}

static void mesurement_bh1750(void *args) {
    app_bh1750_measurement();
}

int32_t app_mesurementCb(void *args) {

    TL_SCHEDULE_TASK(mesurement_bme280, NULL);
    TL_SCHEDULE_TASK(mesurement_bh1750, NULL);
    return config.mesurement_period * 1000;
}
