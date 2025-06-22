#ifndef SRC_INCLUDE_APP_SENSORS_H_
#define SRC_INCLUDE_APP_SENSORS_H_

typedef enum {
    OUTSIDE_S_CLUSTER_OK = 0,
    OUTSIDE_S_NO_CLUSTER,
    OUTSIDE_S_ADDR_FAIL,
    OUTSIDE_S_EMPTY
} outside_sensor_err_t;

typedef struct {
    uint16_t  addr;
    addrExt_t extAddr;
} bind_ext_addr_t;

typedef struct {
    ev_timer_event_t *checkBindTimerEvt;
    uint8_t used;                               /* 0 - not used, 1, 2, 3 - how many cluster+attr */
    bind_ext_addr_t src_extAddr;
    bind_ext_addr_t dst_extAddr;
    uint16_t cluster_id[3];
} bind_outside_sensor_t;



int32_t app_mesurementCb(void *args);
void app_set_outside_temperature(int16_t temp);
int16_t app_get_outside_temperature();
void app_set_outside_humidity(uint16_t hum);
uint16_t app_get_outside_humidity();
void app_set_outside_battery(uint8_t percent);
uint8_t app_get_outside_battery();


void bind_outside_init();
void start_bind_scan(uint16_t bindAddr, uint8_t idx);
uint8_t bind_outside_check(uint16_t addr, uint16_t clId);
void bind_outside_update_timer();
void bind_outside_clear();

#endif /* SRC_INCLUDE_APP_SENSORS_H_ */
