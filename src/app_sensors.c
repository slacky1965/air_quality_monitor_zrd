#include "app_main.h"

static void mesurement(void *args) {
    app_bme280_measurement();
}

int32_t app_mesurementCb(void *args) {

    TL_SCHEDULE_TASK(mesurement, NULL);
    return config.mesurement_period * 1000;
}
