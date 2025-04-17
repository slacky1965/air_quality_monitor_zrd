#ifndef SRC_INCLUDE_APP_EPD_H_
#define SRC_INCLUDE_APP_EPD_H_

#define EPD_EXTERNAL        {'E','X','T',0}
#define EPD_INTERNAL        {'I','N',0}
#define EPD_TEMP            {'T','E','M','P',':',0}
#define EPD_RH              {'R','H',':',0}

#define EXTERNAL_0_X        27
#define EXTERNAL_0_Y        27
#define EXTERNAL_90_X       132
#define EXTERNAL_90_Y       348

#define INTERNAL_0_X        335
#define INTERNAL_0_Y        EXTERNAL_0_Y
#define INTERNAL_90_X       (EXTERNAL_90_X + 6)
#define INTERNAL_90_Y       28



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

#define LINE2_0_X_START     100
#define LINE2_0_X_END       100
#define LINE2_0_Y_START     26
#define LINE2_0_Y_END       299
#define LINE2_90_X_START    0
#define LINE2_90_X_END      299
#define LINE2_90_Y_START    126
#define LINE2_90_Y_END      126

#define LINE3_0_X_START     300
#define LINE3_0_X_END       300
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

#define DATETIME_0_X        56
#define DATETIME_0_Y        0
#define DATETIME_90_X       10
#define DATETIME_90_Y       373

#define ZB_0_X              6
#define ZB_0_Y              0
#define ZB_90_X             6
#define ZB_90_Y             0

#define LEVEL_0_X           361
#define LEVEL_0_Y           0
#define LEVEL_90_X          261
#define LEVEL_90_Y          0

#define STR_MANUF_0_X       6
#define STR_MANUF_0_Y       0
#define STR_MANUF_90_X      6
#define STR_MANUF_90_Y      0

#define STR_MODEL_0_X       70
#define STR_MODEL_0_Y       100
#define STR_MODEL_90_X      16
#define STR_MODEL_90_Y      142

#define STR_JOINED_0_X      70
#define STR_JOINED_0_Y      160
#define STR_JOINED_90_X     20
#define STR_JOINED_90_Y     202

#define STR_FW_0_X      286
#define STR_FW_0_Y      285
#define STR_FW_90_X     186
#define STR_FW_90_Y     385

#define STR_TEMP_IN_0_X     301
#define STR_TEMP_IN_0_Y     80
#define STR_TEMP_IN_90_X    24
#define STR_TEMP_IN_90_Y    48

#define STR_DEGREE_IN_0_X   (STR_TEMP_IN_0_X + 65)
#define STR_DEGREE_IN_0_Y   (STR_TEMP_IN_0_Y - 12)
#define STR_DEGREE_IN_90_X  (STR_TEMP_IN_90_X + 70)
#define STR_DEGREE_IN_90_Y  (STR_TEMP_IN_90_Y - 12)

#define STR_HUMIDITY_IN_0_X STR_TEMP_IN_0_X
#define STR_HUMIDITY_IN_0_Y (STR_TEMP_IN_0_Y + 110)
#define STR_HUMIDITY_IN_90_X (STR_TEMP_IN_90_X + 172)
#define STR_HUMIDITY_IN_90_Y STR_TEMP_IN_90_Y

#define STR_RH_IN_0_X       STR_DEGREE_IN_0_X
#define STR_RH_IN_0_Y       (STR_HUMIDITY_IN_0_Y - 12)
#define STR_RH_IN_90_X      (STR_HUMIDITY_IN_90_X + 70)
#define STR_RH_IN_90_Y      STR_DEGREE_IN_90_Y

#define CH_PERCENT_0_X      (STR_RH_IN_0_X + 16)
#define CH_PERCENT_0_Y      (STR_HUMIDITY_IN_0_Y + 24)
#define CH_PERCENT_90_X     (STR_RH_IN_90_X + 16)
#define CH_PERCENT_90_Y     (STR_RH_IN_90_Y + 44)

typedef enum {
    APP_EPD_ROTATE_0 = 0,
    APP_EPD_ROTATE_90
} epd_rotate_t;

typedef enum {
    APP_EPD_INVERSION_OFF = 0,
    APP_EPD_INVERSION_ON
} epd_inversion_t;

typedef struct {
    uint8_t  minutes;
    int16_t  temp_in;
    int16_t  temp_out;
    uint16_t humidity_in;
    uint16_t humidity_out;
    int16_t  pressure;
    uint8_t  level;
    bool     zbIcon;
    ev_timer_event_t *timerZbIcon;
} epd_screen_variable_t;

void app_epd_init();
//void epd_datetime();
//void epd_zb();
void app_first_start_epd();

#endif /* SRC_INCLUDE_APP_EPD_H_ */
