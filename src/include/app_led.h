#ifndef SRC_INCLUDE_APP_LED_H_
#define SRC_INCLUDE_APP_LED_H_

#define LED_ON      0
#define LED_OFF     1

#define ST_LED_RED      0x01   /* bit == 1 - on, bit == 0 - off */
#define ST_LED_GREEN    0x02
#define ST_LED_BLUE     0x04
#define LED_ALL_OFF     0
#define LED_ON_R        ST_LED_RED
#define LED_ON_G        ST_LED_GREEN
#define LED_ON_B        ST_LED_BLUE
#define LED_ON_RG       ST_LED_RED | ST_LED_GREEN
#define LED_ON_RB       ST_LED_RED | ST_LED_BLUE
#define LED_ON_GB       ST_LED_GREEN | ST_LED_BLUE
#define LED_ON_RGB      ST_LED_RED | ST_LED_GREEN | ST_LED_BLUE

//typedef enum {
//    ST_LED_RED =    0x01,   /* bit == 1 - on, bit == 0 - off */
//    ST_LED_GREEN =  0x02,
//    ST_LED_BLUE =   0x04
//} rgb_led_t;

typedef enum {
    BIT_LED_RED =   0,
    BIT_LED_GREEN,
    BIT_LED_BLUE
} bit_led_t;

typedef struct {
    ev_timer_event_t *timerLedEvt;

    uint8_t         led_status;
    uint16_t        ledOnTime;
    uint16_t        ledOffTime;
    uint8_t         oriSta;     //original state before blink
    uint8_t         sta;        //current state in blink
    uint8_t         times;      //blink times
    uint8_t         oriFlashStatus;
    uint8_t         flashStatus;
    bool            startFlash;
} led_config_t;

void led_init();
void led_set_status(uint8_t new_status);
uint8_t led_get_status();
void led_blink_start(uint8_t times, uint16_t ledOnTime, uint16_t ledOffTime, uint8_t new_status);
void led_blink_stop(void);

#endif /* SRC_INCLUDE_APP_LED_H_ */
