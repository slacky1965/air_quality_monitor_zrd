#ifndef SRC_INCLUDE_APP_BH1750_H_
#define SRC_INCLUDE_APP_BH1750_H_

#include "bh1750.h"

uint8_t app_bh1750_init();
void app_bh1750_measurement();
uint16_t app_bh1750_get_lux();

#endif /* SRC_INCLUDE_APP_BH1750_H_ */
