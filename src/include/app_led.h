#ifndef SRC_INCLUDE_APP_LED_H_
#define SRC_INCLUDE_APP_LED_H_

#define LED_RGB_BIT                     24
#define LED_BIT_IN_BYTE                 2
#define LED_RGB_BYTE                    (LED_RGB_BIT / LED_BIT_IN_BYTE)
#define LED_MAX_LEN                     10
#define LED_RESET_LEN                   32
#define LED_LEN                         4   /* Number of LEDs */

enum led_color_t {
    COLOR_NONE =    0x00,
    COLOR_RED =     0x01,
    COLOR_ORANGE =  0x02,
    COLOR_YELLOW =  0x03,
    COLOR_GREEN =   0x04,
    COLOR_CYAN =    0x05,
    COLOR_BLUE =    0x06,
    COLOR_PURPLE =  0x07,
    COLOR_WHITE =   0x08
};

typedef uint8_t led_color_t;

typedef enum {
    LED_COLOR_OK = 0x00,
    LED_COLOR_ERR
} led_color_err_t;

typedef struct __attribute__((packed)) {
    uint8_t reset_buff[LED_RESET_LEN];
    uint8_t brg_buf[LED_MAX_LEN][LED_RGB_BYTE];
} rgb_buffer_t;

typedef struct {
    uint8_t      len;               // how many led
    led_color_t  color;             // color for all led
    uint8_t      brightness;        // brightness for all led
    rgb_buffer_t out_buf;           // output buffer for SPI
} led_dev_t;

typedef struct {
    ev_timer_event_t *timerLedEvt;

    uint16_t        ledOnTime;
    uint16_t        ledOffTime;
    led_color_t     color;
    led_color_t     oriColor;   // original color before blink
    uint8_t         oriBrig;    // original brightness before blink
    uint8_t         times;      //blink times
    led_color_t     oriFlashColor;
    led_color_t     flashColor;
    bool            startFlash;
    int8_t          direction;
    uint8_t         effect_brightness;
    uint32_t        effect_interval;
} led_flash_config_t;

void led_init();
led_color_t led_get_color();
void led_set_brightness(uint8_t brightness);
uint8_t led_get_brightness();
void led_on(led_color_t color);
void led_off();
void led_blink_start(uint8_t times, uint16_t ledOnTime, uint16_t ledOffTime, led_color_t color);
void led_effect_start(uint8_t times, led_color_t color);
#define led_effect_stop led_blink_stop
void led_blink_stop();
bool led_flashing();

#endif /* SRC_INCLUDE_APP_LED_H_ */
