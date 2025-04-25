#include "app_main.h"

//static bool i2c_sending = false;

void app_i2c_init() {
    drv_i2c_gpio_set(I2C_PIN_GROUP);
    drv_i2c_master_init(I2C_CLOCK);

//    reg_i2c_mode |= FLD_I2C_MASTER_EN; //enable master mode
//    reg_i2c_mode &= ~FLD_I2C_HOLD_MASTER; // Disable clock stretching for Sensor
//
//    reg_clk_en0 |= FLD_CLK0_I2C_EN;    //enable i2c clock
//    reg_spi_sp  &= ~FLD_SPI_ENABLE;   //force PADs act as I2C; i2c and spi share the hardware of IC

    app_bme280_init();

}

