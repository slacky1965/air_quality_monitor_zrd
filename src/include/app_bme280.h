#ifndef SRC_INCLUDE_APP_BME280_H_
#define SRC_INCLUDE_APP_BME280_H_

#include "bme280.h"

void app_bme280_init();
int8_t bme280_i2c_read(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *intf_ptr);
int8_t bme280_i2c_write(uint8_t reg_addr, const uint8_t *reg_data, uint32_t len, void *intf_ptr);
void bme280_delay_us(uint32_t period, void *intf_ptr);
void app_test_bme280();

#endif /* SRC_INCLUDE_APP_BME280_H_ */
