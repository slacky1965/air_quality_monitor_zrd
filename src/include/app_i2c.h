#ifndef SRC_INCLUDE_APP_I2C_H_
#define SRC_INCLUDE_APP_I2C_H_

#define I2C_PIN_GROUP       I2C_GPIO_GROUP_C2C3 //I2C_GPIO_GROUP_C0C1
#define I2C_CLOCK           200000//200K
#define I2C_SLAVE_ADDR      0x48000//0x8800
#define I2C_SLAVE_ADDR_LEN  3//2


void app_i2c_init();

#endif /* SRC_INCLUDE_APP_I2C_H_ */
