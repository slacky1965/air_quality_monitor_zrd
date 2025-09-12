#include "app_main.h"

static sound_config_t sound_config = {
        .timerSoundEvt = NULL,
        .sound = 0,
        .soundOnTime = 0,
        .soundOffTime = 0,
        .times = 0,
};

static void sound_off() {
    drv_pwm_cfg(PWM_SOUND_CHANNEL, 0, sound_config.period);
}

static void sound_on() {
    drv_pwm_cfg(PWM_SOUND_CHANNEL, (sound_config.period * sound_config.volume / 100), sound_config.period);
}

static int32_t sound_effectCb(void *args) {

    int32_t interval = 0;

    if (sound_config.times == 0) {
        sound_config.timerSoundEvt = NULL;
        return -1;
    }

    if (sound_config.times) {
        if (sound_config.sound) {
            sound_config.sound = false;
            if (sound_config.sound_direction == SOUND_INC) sound_config.volume += sound_config.ef_volume;
            if (sound_config.sound_direction == SOUND_DEC) sound_config.volume -= sound_config.ef_volume;
            sound_on();
            interval = sound_config.soundOnTime;
        } else {
            sound_off();
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

void sound_start(uint8_t times, uint16_t soundOnTime, uint16_t soundOffTime, sound_direction_t direction) {

    if (sound_config.timerSoundEvt) {
        TL_ZB_TIMER_CANCEL(&sound_config.timerSoundEvt);
    }

    sound_config.sound = true;
    sound_config.soundOnTime = soundOnTime;
    sound_config.soundOffTime = soundOffTime;
    sound_config.times = times;
    sound_config.sound_direction = direction;

    if (sound_config.sound_direction == SOUND_SAME) {
        sound_config.volume = 100;
    } else if (sound_config.sound_direction == SOUND_INC) {
        sound_config.ef_volume = 100/(times == 0?1:times);
        sound_config.volume = 0;
    } else {
        sound_config.ef_volume = 100/(times == 0?1:times);
        sound_config.volume = 100 + sound_config.ef_volume;
    }

    sound_config.timerSoundEvt = TL_ZB_TIMER_SCHEDULE(sound_effectCb, NULL, TIMEOUT_10MS);
}

void sound_init() {
    memset(&sound_config, 0, sizeof(sound_config_t));

    sound_config.period = (uint16_t)((uint32_t)PWM_CLOCK_SOURCE / 1000);

    PWM_SOUND_CHANNEL_SET();
    drv_pwm_init();
//    drv_pwm_invert(PWM_SOUND_CHANNEL);

    sound_off();

    drv_pwm_start(PWM_SOUND_CHANNEL);

    /* for test */
//    uint8_t dir = 1;
//
//    while(1) {
//        drv_pwm_cfg(PWM_SOUND_CHANNEL, (sound_config.period * sound_config.volume / 100), sound_config.period);
//        dir ? (sound_config.volume++) : (sound_config.volume--);
//        if (sound_config.volume >= 100) {
//            dir = 0;
//        }
//        if (sound_config.volume <= 0) {
//            dir = 1;
//        }
//        WaitMs(50);
//    }


}

bool sound_idle() {
    if (sound_config.timerSoundEvt) {
        return true;
    }

    return false;
}
