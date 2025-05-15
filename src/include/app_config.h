#ifndef SRC_INCLUDE_APP_CONFIG_H_
#define SRC_INCLUDE_APP_CONFIG_H_

typedef struct __attribute__((packed)) {
    uint8_t     rotate;
    uint8_t     inversion;
    uint16_t    mesurement_period;
    uint8_t     brightness;             /* level                */
    uint8_t     d_mode;                 /* Display mode C or F  */
//    uint8_t     unit_pressure;
//    uint8_t     joined;
    uint8_t     crc;
} config_t;

extern config_t config;

nv_sts_t config_save();
nv_sts_t config_restore();

#endif /* SRC_INCLUDE_APP_CONFIG_H_ */
