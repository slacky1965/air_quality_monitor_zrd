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

}

int8_t scan_i2c_addr(int address) {
    if ((reg_clk_en0 & FLD_CLK0_I2C_EN)==0)
        app_i2c_init();
    u8 r = reg_i2c_speed;
    reg_i2c_speed = (u8)(CLOCK_SYS_CLOCK_HZ/(4*100000)); // 100 kHz
    reg_i2c_id = (u8) address;
    reg_i2c_ctrl = FLD_I2C_CMD_START | FLD_I2C_CMD_ID | FLD_I2C_CMD_STOP;
    while (reg_i2c_status & FLD_I2C_CMD_BUSY);
    reg_i2c_speed = r;
    return ((reg_i2c_status & FLD_I2C_NAK)? 0 : address);
}

//int8_t app_i2c_read(uint8_t dev_addr, uint8_t reg_addr, uint8_t *reg_data, uint32_t len) {
//
//    drv_i2c_read_series(dev_addr << 1, reg_addr, 1, reg_data, len);
//
//    return (reg_i2c_status & FLD_I2C_NAK);
//}
//
//int8_t app_i2c_write(uint8_t dev_addr, uint8_t reg_addr, const uint8_t *reg_data, uint32_t len) {
//
//    drv_i2c_write_series(dev_addr << 1, reg_addr, 1, (uint8_t*)reg_data, len);
//
//    return (reg_i2c_status & FLD_I2C_NAK);
//}
//
