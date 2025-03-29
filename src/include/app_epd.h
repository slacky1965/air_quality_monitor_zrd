#ifndef SRC_INCLUDE_APP_EPD_H_
#define SRC_INCLUDE_APP_EPD_H_

typedef enum {
    APP_EPD_ROTATE_0 = 0,
    APP_EPD_ROTATE_90
} epd_rotate_t;

typedef enum {
    APP_EPD_INVERSION_OFF = 0,
    APP_EPD_INVERSION_ON
} epd_inversion_t;

void app_epd_init();

#endif /* SRC_INCLUDE_APP_EPD_H_ */
