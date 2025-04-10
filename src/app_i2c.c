#include "app_main.h"

//static bool i2c_sending = false;

void app_i2c_init() {
    drv_i2c_gpio_set(I2C_PIN_GROUP);
    drv_i2c_master_init(I2C_CLOCK);

    reg_i2c_mode |= FLD_I2C_MASTER_EN; //enable master mode
    reg_i2c_mode &= ~FLD_I2C_HOLD_MASTER; // Disable clock stretching for Sensor

    reg_clk_en0 |= FLD_CLK0_I2C_EN;    //enable i2c clock
    reg_spi_sp  &= ~FLD_SPI_ENABLE;   //force PADs act as I2C; i2c and spi share the hardware of IC

    app_bme280_init();

    app_test_bme280();

//    bme280_init();
//
//    printf("Temperature: %d\r\n", bme280_readTemperature() * 100);

//    uint8_t ret = app_scan_i2c_device(BME280_I2CADDR);
//
//    printf("app_scan_i2c_device ret: %d\r\n", ret);
}

//void drv_i2c_write_series(u8 slaveID, u32 addr, u32 addrLen, u8 *dataBuf, int dataLen);

void app_i2c_write_series(uint8_t device_id, uint8_t *dataBuf, int dataLen) {
//    if(i2c_sending)return;
//    i2c_sending=true;
    i2c_set_id(device_id);
    i2c_write_series(0, 0, dataBuf, dataLen);
//    i2c_sending=false;
}

uint8_t app_scan_i2c_device(uint8_t address) {
    reg_i2c_id = address << 1;
    reg_i2c_ctrl = FLD_I2C_CMD_START | FLD_I2C_CMD_ID;
    while(reg_i2c_status & FLD_I2C_CMD_BUSY);
    reg_i2c_ctrl = FLD_I2C_CMD_STOP;
    while(reg_i2c_status & FLD_I2C_CMD_BUSY );

    return (reg_i2c_status & FLD_I2C_NAK) ? 0 : 1;
}
