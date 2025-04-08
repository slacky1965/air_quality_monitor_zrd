#include "app_main.h"

void app_i2c_init() {
    drv_i2c_master_init(I2C_CLOCK);
    drv_i2c_gpio_set(I2C_PIN_GROUP);

    app_bme280_init();

}

