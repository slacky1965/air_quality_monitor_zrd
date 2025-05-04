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

typedef enum {
    APP_EPD_UNIT_PRESSURE_PA = 0,
    APP_EPD_UNIT_PRESSURE_MM
} epd_unit_pressure_t;

typedef struct {
    uint8_t  minutes;
    int16_t  temp_in;
    int16_t  temp_out;
    uint16_t humidity_in;
    uint16_t humidity_out;
    int16_t  pressure;
    uint16_t co2;
    uint16_t voc;
    uint16_t lux;
    uint16_t lqi;
    uint8_t  level;
    bool     zbIcon;
    ev_timer_event_t *timerZbIcon;
} epd_screen_variable_t;

void app_epd_init();
void app_first_start_epd();

#endif /* SRC_INCLUDE_APP_EPD_H_ */
