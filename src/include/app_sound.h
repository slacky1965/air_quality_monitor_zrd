#ifndef SRC_INCLUDE_APP_SOUND_H_
#define SRC_INCLUDE_APP_SOUND_H_

typedef enum {
    SOUND_SAME = 0,
    SOUND_INC,
    SOUND_DEC
} sound_direction_t;

typedef struct {
    ev_timer_event_t *timerSoundEvt;

    uint16_t          period;
    uint8_t           volume;
    uint8_t           ef_volume;
    uint16_t          soundOnTime;
    uint16_t          soundOffTime;
    uint8_t           times;
    uint8_t           sound;
    sound_direction_t sound_direction;
} sound_config_t;

void sound_start(uint8_t times, uint16_t soundOnTime, uint16_t soundOffTime, sound_direction_t direction);
bool sound_idle();
void sound_init();

#endif /* SRC_INCLUDE_APP_SOUND_H_ */
