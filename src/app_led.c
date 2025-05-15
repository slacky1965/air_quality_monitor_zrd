#include "app_main.h"

#define LED_BIT_H0   0b10000000
#define LED_BIT_L0   0b00001000
#define LED_BIT_H1   0b11100000
#define LED_BIT_L1   0b00001110

static led_dev_t led_dev;
static led_flash_config_t led_flash_config;

static void led_spi_init() {
    drv_gpio_write(LED_SPI_PIN_CS, 0);
    drv_spi_master_init(LED_SPI_CLOCK, SPI_MODE_0);
    spi_master_gpio_set(LED_SPI_PIN_GROUP);
}

static void led_set_values(led_color_t color, uint8_t brightness) {

    uint8_t r, g, b, idx;

    led_spi_init();

    switch(color) {
        case COLOR_NONE:
            r = 0;
            g = 0;
            b = 0;
            break;
        case COLOR_RED:
            r = brightness;
            g = 0;
            b = 0;
            break;
        case COLOR_YELLOW:
            r = brightness;
            g = brightness;
            b = 0;
            break;
        case COLOR_GREEN:
            r = 0;
            g = brightness;
            b = 0;
            break;
        case COLOR_BLUE:
            r = 0;
            g = 0;
            b = brightness;
            break;
        case COLOR_PURPLE:
            r = brightness;
            g = 0;
            b = brightness;
            break;
        case COLOR_WHITE:
            r = brightness;
            g = brightness;
            b = brightness;
            break;
        default:
            return;
    }

    led_dev.brightness = brightness;
    led_dev.color = color;

    for (uint8_t len = 0; len < led_dev.len; len++) {
        idx = 7;
        for (uint8_t i = 0; i < 4; i++) {
            if (g >> idx-- & 0x01)
                led_dev.out_buf.brg_buf[len][i] = LED_BIT_H1;
            else
                led_dev.out_buf.brg_buf[len][i] = LED_BIT_H0;
            if (g >> idx-- & 0x01)
                led_dev.out_buf.brg_buf[len][i] |= LED_BIT_L1;
            else
                led_dev.out_buf.brg_buf[len][i] |= LED_BIT_L0;
        }

        idx = 7;
        for (uint8_t i = 4; i < 8; i++) {
            if (r >> idx-- & 0x01)
                led_dev.out_buf.brg_buf[len][i] = LED_BIT_H1;
            else
                led_dev.out_buf.brg_buf[len][i] = LED_BIT_H0;
            if (r >> idx-- & 0x01)
                led_dev.out_buf.brg_buf[len][i] |= LED_BIT_L1;
            else
                led_dev.out_buf.brg_buf[len][i] |= LED_BIT_L0;
        }

        idx = 7;
        for (uint8_t i = 8; i < 12; i++) {
            if (b >> idx-- & 0x01)
                led_dev.out_buf.brg_buf[len][i] = LED_BIT_H1;
            else
                led_dev.out_buf.brg_buf[len][i] = LED_BIT_H0;
            if (b >> idx-- & 0x01)
                led_dev.out_buf.brg_buf[len][i] |= LED_BIT_L1;
            else
                led_dev.out_buf.brg_buf[len][i] |= LED_BIT_L0;
        }
    }

    drv_spi_write(NULL, 0, (uint8_t*)&led_dev.out_buf, led_dev.len * LED_RGB_BYTE + LED_RESET_LEN, LED_SPI_PIN_CS);

}

//static led_color_err_t led_set_color(led_color_t color) {
//
//    switch(color) {
//        case COLOR_NONE:
//        case COLOR_RED:
//        case COLOR_YELLOW:
//        case COLOR_GREEN:
//        case COLOR_BLUE:
//        case COLOR_WHITE:
//            led_dev.color = color;
//            break;
//        default:
//            return LED_COLOR_ERR;
//    }
//
//    return LED_COLOR_OK;
//}

static int32_t led_flashCb(void *args) {
    u32 interval = 0;


    if(led_flash_config.flashColor == led_flash_config.oriFlashColor) {
        led_flash_config.times--;
        if(led_flash_config.times <= 0) {
            led_dev.brightness = led_flash_config.oriBrig;
            led_on(led_flash_config.oriColor);
            led_flash_config.timerLedEvt = NULL;
            return -1;
        }
    }

    if (led_flash_config.flashColor == COLOR_NONE)
        led_flash_config.flashColor = led_flash_config.oriFlashColor;
    else
        led_flash_config.flashColor = COLOR_NONE;

    led_on(led_flash_config.flashColor);
    if(led_flash_config.startFlash) {
        interval = led_flash_config.ledOnTime;
        led_flash_config.startFlash = false;
    }else{
        interval = led_flash_config.ledOffTime;
        led_flash_config.startFlash = true;
    }

    return interval;
}


led_color_t led_get_color() {
    return led_dev.color;
}

void led_set_brightness(uint8_t brightness) {
    led_dev.brightness = brightness;
}

uint8_t led_get_brightness() {
    return led_dev.brightness;
}

void led_on(led_color_t color) {
    led_set_values(color, led_dev.brightness);
}

void led_off() {
//    led_dev.brightness = 0;
    led_dev.color = COLOR_NONE;

    led_set_values(led_dev.color, led_dev.brightness);
}

void led_test(led_color_t color) {

        for (uint16_t i = 1; i < 256; i++) {
            led_set_values(color, i);

            sleep_ms(10);
        }

        for (int16_t i = 255; i > 0; i--) {
            led_set_values(color, i);

            sleep_ms(10);
        }
}

void led_init() {

    led_dev.len = LED_LEN;
    led_dev.brightness = config.brightness;
    led_dev.color = COLOR_NONE;
    memset(led_dev.out_buf.reset_buff, 0, LED_RESET_LEN);

    led_spi_init();
    led_off();

#if 0
loop:
    led_test(COLOR_RED);
    led_test(COLOR_YELLOW);
    led_test(COLOR_GREEN);
    led_test(COLOR_BLUE);
    led_test(COLOR_PURPLE);
    led_test(COLOR_WHITE);

    goto loop;
#endif
}

void led_blink_start(uint8_t times, uint16_t ledOnTime, uint16_t ledOffTime, led_color_t color) {

    if(led_flash_config.timerLedEvt) {
        TL_ZB_TIMER_CANCEL(&led_flash_config.timerLedEvt);
    }

    led_flash_config.times = times;
    led_flash_config.oriColor = led_dev.color;
    led_flash_config.oriBrig = led_dev.brightness;

    led_off();
    led_set_brightness(255);
    led_flash_config.ledOnTime = ledOnTime;
    led_flash_config.ledOffTime = ledOffTime;
    led_flash_config.oriFlashColor = color;
    led_flash_config.flashColor = led_dev.color;
    led_flash_config.startFlash = true;

    led_flash_config.timerLedEvt = TL_ZB_TIMER_SCHEDULE(led_flashCb, NULL, ledOffTime);
}

void led_blink_stop(void) {
    if(led_flash_config.timerLedEvt) {
        TL_ZB_TIMER_CANCEL(&led_flash_config.timerLedEvt);
        led_flash_config.times = 0;
        led_dev.brightness = led_flash_config.oriBrig;
        led_on(led_flash_config.oriColor);
    }
}


