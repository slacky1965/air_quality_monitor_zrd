#ifndef SRC_INCLUDE_APP_SCD4X_H_
#define SRC_INCLUDE_APP_SCD4X_H_

#include "scd4x.h"

uint8_t app_scd4x_init();
void app_scd4x_set_pressure();
void app_scd4x_measurement();
uint16_t app_scd4x_get_co2();
uint8_t app_scd4x_forced_calibration();
uint8_t app_scd4x_factory_reset();

#endif /* SRC_INCLUDE_APP_SCD4X_H_ */
