#ifndef SRC_INCLUDE_APP_DS3231_H_
#define SRC_INCLUDE_APP_DS3231_H_

#include "ds3231.h"

uint8_t app_ds3231_init();
uint8_t app_ds3231_get_time(ds3231_time_t *t);
uint8_t app_ds3231_set_time(ds3231_time_t *t);

#endif /* SRC_INCLUDE_APP_DS3231_H_ */
