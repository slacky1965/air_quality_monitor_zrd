#ifndef SRC_INCLUDE_APP_EPD_H_
#define SRC_INCLUDE_APP_EPD_H_

#define EPD_EXTERNAL        {'E','X','T','.',0}
#define EPD_INTERNAL        {'I','N','.',0}
#define EPD_TEMP            {'T','E','M','P',':',0}
#define EPD_RH              {'R','H',':',0}

#define EXTERNAL_0_X        25
#define EXTERNAL_0_Y        27

#define INTERNAL_0_X        337
#define INTERNAL_0_Y        27



#define BORDER_0_X_START    1
#define BORDER_0_X_END      400
#define BORDER_0_Y_START    27
#define BORDER_0_Y_END      300
#define BORDER_90_X_START   1
#define BORDER_90_X_END     300
#define BORDER_90_Y_START   27
#define BORDER_90_Y_END     400

#define LINE1_0_X_START     0
#define LINE1_0_X_END       399
#define LINE1_0_Y_START     26
#define LINE1_0_Y_END       26
#define LINE1_90_X_START    0
#define LINE1_90_X_END      299
#define LINE1_90_Y_START    26
#define LINE1_90_Y_END      26

#define LINE2_0_X_START     96
#define LINE2_0_X_END       96
#define LINE2_0_Y_START     26
#define LINE2_0_Y_END       299
#define LINE2_90_X_START    0
#define LINE2_90_X_END      299
#define LINE2_90_Y_START    126
#define LINE2_90_Y_END      126

#define LINE3_0_X_START     302
#define LINE3_0_X_END       302
#define LINE3_0_Y_START     26
#define LINE3_0_Y_END       299
#define LINE3_90_X_START    0
#define LINE3_90_X_END      299
#define LINE3_90_Y_START    273
#define LINE3_90_Y_END      273

#define LINE4_90_X_START    0
#define LINE4_90_X_END      299
#define LINE4_90_Y_START    373
#define LINE4_90_Y_END      373

#define DATETIME_0_X        60
#define DATETIME_0_Y        0
#define DATETIME_90_X       18
#define DATETIME_90_Y       373

//#define DATE_0_X            60
//#define DATE_0_Y            0
//#define DATE_90_X           24
//#define DATE_90_Y           373
//
//#define WDAY_0_X            206
//#define WDAY_0_Y            0
//#define WDAY_90_X           162
//#define WDAY_90_Y           373
//
//#define TIME_0_X            270
//#define TIME_0_Y            0
//#define TIME_90_X           216
//#define TIME_90_Y           373

#define ZB_0_X              6
#define ZB_0_Y              0
#define ZB_90_X             6
#define ZB_90_Y             0

typedef enum {
    APP_EPD_ROTATE_0 = 0,
    APP_EPD_ROTATE_90
} epd_rotate_t;

typedef enum {
    APP_EPD_INVERSION_OFF = 0,
    APP_EPD_INVERSION_ON
} epd_inversion_t;

typedef struct {
    uint8_t minutes;
    int16_t temp_in;
    int16_t temp_out;
    int16_t humidity_in;
    int16_t humidity_out;
    int16_t pressure;
    uint8_t level;
    bool    zbIcon;
    ev_timer_event_t *timerZbIcon;
} epd_screen_variable_t;

void app_epd_init();
void epd_datetime();
void epd_zb();

#endif /* SRC_INCLUDE_APP_EPD_H_ */
