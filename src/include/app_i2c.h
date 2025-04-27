#ifndef SRC_INCLUDE_APP_I2C_H_
#define SRC_INCLUDE_APP_I2C_H_

#define I2C_PIN_GROUP       I2C_GPIO_GROUP_C2C3
#define I2C_CLOCK           200000//200K

void app_i2c_init();
int8_t scan_i2c_addr(int address);

#endif /* SRC_INCLUDE_APP_I2C_H_ */
