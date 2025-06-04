#include "app_main.h"

static sound_config_t sound_config = {
        .timerSoundEvt = NULL,
        .sound = 0,
        .soundOnTime = 0,
        .soundOffTime = 0,
        .times = 0,
};

static int32_t sound_effectCb(void *args) {

    int32_t interval = 0;

    if (sound_config.times == 0) {
        sound_config.timerSoundEvt = NULL;
        return -1;
    }

    if (sound_config.times) {
        if (sound_config.sound) {
            sound_config.sound = false;
            drv_gpio_write(SOUND_PIN, ON);
            interval = sound_config.soundOnTime;
        } else {
            drv_gpio_write(SOUND_PIN, OFF);
            sound_config.times--;
            if (sound_config.times == 0) {
                sound_config.timerSoundEvt = NULL;
                return -1;
            }
            sound_config.sound = true;
            interval = sound_config.soundOffTime;
        }
    }

    return interval;
}

void sound_start(uint8_t times, uint16_t soundOnTime, uint16_t soundOffTime) {

    if (sound_config.timerSoundEvt) {
        TL_ZB_TIMER_CANCEL(&sound_config.timerSoundEvt);
    }

    sound_config.sound = true;
    sound_config.soundOnTime = soundOnTime;
    sound_config.soundOffTime = soundOffTime;
    sound_config.times = times;

    sound_config.timerSoundEvt = TL_ZB_TIMER_SCHEDULE(sound_effectCb, NULL, TIMEOUT_10MS);
}
