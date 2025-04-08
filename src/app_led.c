#include "app_main.h"

led_config_t led_config;

static void led_on(uint32_t led_gpio) {
    switch(led_gpio) {
        case LED_RED:
            led_config.led_status |= 1 << BIT_LED_RED;
            drv_gpio_write(LED_RED, LED_ON);
            break;
        case LED_GREEN:
            led_config.led_status |= 1 << BIT_LED_GREEN;
            drv_gpio_write(LED_GREEN, LED_ON);
            break;
        case LED_BLUE:
            led_config.led_status |= 1 << BIT_LED_BLUE;
            drv_gpio_write(LED_BLUE, LED_ON);
            break;
        default:
            break;
    }
}

static void led_off(uint32_t led_gpio) {
    switch(led_gpio) {
        case LED_RED:
            led_config.led_status &= ~(1 << BIT_LED_RED);
            drv_gpio_write(LED_RED, LED_OFF);
            break;
        case LED_GREEN:
            led_config.led_status &= ~(1 << BIT_LED_GREEN);
            drv_gpio_write(LED_GREEN, LED_OFF);
            break;
        case LED_BLUE:
            led_config.led_status &= ~(1 << BIT_LED_BLUE);
            drv_gpio_write(LED_BLUE, LED_OFF);
            break;
        default:
            break;
    }
}


void led_init() {
    led_config.timerLedEvt = NULL;
    led_config.led_status = 0;
    led_set_status(LED_ALL_OFF);
}

void led_set_status(uint8_t new_status) {
    if (new_status & (1 << BIT_LED_RED))
        led_on(LED_RED);
    else
        led_off(LED_RED);

    if (new_status & (1 << BIT_LED_GREEN))
        led_on(LED_GREEN);
    else
        led_off(LED_GREEN);

    if (new_status & (1 << BIT_LED_BLUE))
        led_on(LED_BLUE);
    else
        led_off(LED_BLUE);
}

uint8_t led_get_status() {
    return led_config.led_status;
}

int32_t led_flashCb(void *args) {
    u32 interval = 0;


    if(led_config.flashStatus == led_config.oriFlashStatus) {
        led_config.times--;
        if(led_config.times <= 0) {
            led_set_status(led_config.oriSta);
            led_config.timerLedEvt = NULL;
            return -1;
        }
    }

    if (led_config.flashStatus == LED_ALL_OFF)
        led_config.flashStatus = led_config.oriFlashStatus;
    else
        led_config.flashStatus = LED_ALL_OFF;

    led_set_status(led_config.flashStatus);
    if(led_config.startFlash) {
        interval = led_config.ledOnTime;
        led_config.startFlash = false;
    }else{
        interval = led_config.ledOffTime;
        led_config.startFlash = true;
    }

    return interval;
}


void led_blink_start(uint8_t times, uint16_t ledOnTime, uint16_t ledOffTime, uint8_t new_status) {

    if(led_config.timerLedEvt) {
        TL_ZB_TIMER_CANCEL(&led_config.timerLedEvt);
    }

    led_config.times = times;
    led_config.oriSta = led_config.led_status;

    led_set_status(LED_ALL_OFF);
    led_config.sta = 0;
    led_config.ledOnTime = ledOnTime;
    led_config.ledOffTime = ledOffTime;
    led_config.oriFlashStatus = new_status;
    led_config.flashStatus = LED_ALL_OFF;
    led_config.startFlash = true;

    led_config.timerLedEvt = TL_ZB_TIMER_SCHEDULE(led_flashCb, NULL, ledOnTime);
}

void led_blink_stop(void) {
    if(led_config.timerLedEvt) {
        TL_ZB_TIMER_CANCEL(&led_config.timerLedEvt);
        led_config.times = 0;
        led_set_status(led_config.oriSta);
    }
}

