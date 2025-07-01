#ifndef SRC_INCLUDE_APP_SENSORS_H_
#define SRC_INCLUDE_APP_SENSORS_H_

#define BIND_CLUSTER_NUM    3

typedef enum {
    OUTSIDE_SRC_CLUSTER_OK = 0,
    OUTSIDE_SRC_NO_CLUSTER,
    OUTSIDE_SRC_ADDR_FAIL,
    OUTSIDE_SRC_TABLE_FULL,
    OUTSIDE_SRC_EMPTY
} outside_sensor_err_t;

typedef struct {
    uint16_t  addr;
    addrExt_t extAddr;
} bind_ext_addr_t;

typedef struct {
    ev_timer_event_t *checkBindTimerEvt;
    uint8_t used;                               /* 0 - not used, 1, 2, 3 - how many cluster */
    uint16_t src_addr;
    bind_ext_addr_t src_extAddr;
    bind_ext_addr_t dst_extAddr;
    uint16_t cluster_id[BIND_CLUSTER_NUM];
} bind_outside_sensor_t;



int32_t app_mesurementCb(void *args);
void app_set_outside_temperature(int16_t temp);
int16_t app_get_outside_temperature();
void app_set_outside_humidity(uint16_t hum);
uint16_t app_get_outside_humidity();
void app_set_outside_battery(uint8_t percent);
uint8_t app_get_outside_battery();


void bind_outside_init();
outside_sensor_err_t bind_outsise_proc(uint16_t addr, uint16_t clusterId);
void bind_outside_update_timer();
#define bind_outside_clear bind_outside_init

#endif /* SRC_INCLUDE_APP_SENSORS_H_ */
