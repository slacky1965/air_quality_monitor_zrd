#ifndef SRC_INCLUDE_APP_SOUND_H_
#define SRC_INCLUDE_APP_SOUND_H_

typedef struct {
    ev_timer_event_t *timerSoundEvt;

    uint16_t soundOnTime;
    uint16_t soundOffTime;
    uint8_t  times;
    uint8_t  sound;
} sound_config_t;

void sound_start(uint8_t times, uint16_t soundOnTime, uint16_t soundOffTime);

#endif /* SRC_INCLUDE_APP_SOUND_H_ */
