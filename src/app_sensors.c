#include "app_main.h"

#define BIND_TIMEOUT  TIMEOUT_2HOUR //TIMEOUT_10MIN //

static int16_t  outside_temperature = 0x8000;
static uint16_t outside_humidity = 0xffff;
static uint8_t  outside_battery_percent = 0xff;

//static uint8_t t_index = 0;
//static uint16_t b_addr;

static bool sw_onoff = false;

static bind_outside_sensor_t bind_outside_sensor = {
        .used = 0,
        .checkBindTimerEvt = NULL,
};

static void cmdOnOff(uint8_t command) {
    epInfo_t dstEpInfo;
    TL_SETSTRUCTCONTENT(dstEpInfo, 0);

    dstEpInfo.profileId = HA_PROFILE_ID;

    dstEpInfo.dstAddrMode = APS_DSTADDR_EP_NOTPRESETNT;

    /* command 0x00 - off, 0x01 - on, 0x02 - toggle */

    switch(command) {
        case ZCL_CMD_ONOFF_OFF:
#if UART_PRINTF_MODE && DEBUG_ONOFF
            printf("OnOff command: off\r\n");
#endif /* UART_PRINTF_MODE */
            zcl_onOff_offCmd(APP_ENDPOINT1, &dstEpInfo, FALSE);
            break;
        case ZCL_CMD_ONOFF_ON:
#if UART_PRINTF_MODE && DEBUG_ONOFF
            printf("OnOff command: on\r\n");
#endif /* UART_PRINTF_MODE */
            zcl_onOff_onCmd(APP_ENDPOINT1, &dstEpInfo, FALSE);
            break;
        case ZCL_CMD_ONOFF_TOGGLE:
#if UART_PRINTF_MODE && DEBUG_ONOFF
            printf("OnOff command: toggle\r\n");
#endif /* UART_PRINTF_MODE */
            zcl_onOff_toggleCmd(APP_ENDPOINT1, &dstEpInfo, FALSE);
            break;
        default:
            break;
    }
}

static void proc_co2_voc_onoff(void *args) {

    bool co2_ctrl_off = false;
    bool co2_ctrl_on = false;
    bool voc_ctrl_off = false;
    bool voc_ctrl_on = false;

    zcl_onOffSwitchCfgAttr_t *onoffCfgAttrs = zcl_onOffSwitchCfgAttrGet();

    if(zb_isDeviceJoinedNwk()) {

        uint16_t voc = app_sgp40_get_voc();
        uint16_t co2 = app_scd4x_get_co2();

//        printf("co2: %d, voc: %d\r\n", co2, voc);

//        if (co2 < CO2_ONOFF_MIN || voc < VOC_ONOFF_MIN)
//            return;

        if (config.co2_onoff) {
            if (co2 >= config.co2_onoff_high)
                co2_ctrl_on = true;
            if (co2 <= config.co2_onoff_low)
                co2_ctrl_off = true;
            if (co2_ctrl_off && co2_ctrl_on)
                co2_ctrl_on = false;
        }

        if (config.voc_onoff) {
            if (voc >= config.voc_onoff_high)
                voc_ctrl_on = true;
            if (voc <= config.voc_onoff_low)
                voc_ctrl_off = true;
            if (voc_ctrl_off && voc_ctrl_on)
                voc_ctrl_on = false;
        }

        if (sw_onoff && !config.co2_onoff && !config.voc_onoff) {
            sw_onoff = false;
            switch(onoffCfgAttrs->switchActions) {
                case ZCL_SWITCH_ACTION_ON_OFF:
                    cmdOnOff(ZCL_CMD_ONOFF_ON);
                    break;
                case ZCL_SWITCH_ACTION_OFF_ON:
                    cmdOnOff(ZCL_CMD_ONOFF_OFF);
                    break;
//                    case ZCL_SWITCH_ACTION_TOGGLE:
//                        cmdOnOff(ZCL_CMD_ONOFF_TOGGLE);
//                        break;
                default:
                    break;
            }
        }


        if ((co2_ctrl_on && config.co2_onoff) || (voc_ctrl_on && config.voc_onoff)) {
            if (!sw_onoff) {
                sw_onoff = true;
                if (config.sound)
                    sound_start(3, 20, 100);
#if UART_PRINTF_MODE && DEBUG_ONOFF
                printf("Switch action: 0x0%x\r\n", onoffCfgAttrs->switchActions);
#endif /* UART_PRINTF_MODE */
                switch(onoffCfgAttrs->switchActions) {
                    case ZCL_SWITCH_ACTION_ON_OFF:
                        cmdOnOff(ZCL_CMD_ONOFF_OFF);
                        break;
                    case ZCL_SWITCH_ACTION_OFF_ON:
                        cmdOnOff(ZCL_CMD_ONOFF_ON);
                        break;
//                    case ZCL_SWITCH_ACTION_TOGGLE:
//                        cmdOnOff(ZCL_CMD_ONOFF_TOGGLE);
//                        break;
                    default:
                        break;
                }
            }
        }

        if ((co2_ctrl_off && config.co2_onoff && !voc_ctrl_on) || (voc_ctrl_off && config.voc_onoff && !co2_ctrl_on)) {
            if (sw_onoff) {
                sw_onoff = false;
#if UART_PRINTF_MODE && DEBUG_ONOFF
                printf("Switch action: 0x0%x\r\n", onoffCfgAttrs->switchActions);
#endif /* UART_PRINTF_MODE */
                switch(onoffCfgAttrs->switchActions) {
                    case ZCL_SWITCH_ACTION_ON_OFF:
                        cmdOnOff(ZCL_CMD_ONOFF_ON);
                        break;
                    case ZCL_SWITCH_ACTION_OFF_ON:
                        cmdOnOff(ZCL_CMD_ONOFF_OFF);
                        break;
//                    case ZCL_SWITCH_ACTION_TOGGLE:
//                        cmdOnOff(ZCL_CMD_ONOFF_TOGGLE);
//                        break;
                    default:
                        break;
                }
            }
        }
    }

//    printf("sw_onoff: %d, co2_ctrl_off: %d, co2_ctrl_on: %d, voc_ctrl_off: %d, voc_ctrl_on: %d\r\n", sw_onoff, co2_ctrl_off, co2_ctrl_on, voc_ctrl_off, voc_ctrl_on);
}

static void mesurement_bme280(void *args) {
    app_bme280_measurement();
}

static void mesurement_bh1750(void *args) {
    app_bh1750_measurement();
}

static void mesurement_scd4x(void *args) {
    app_scd4x_measurement();
}

static void mesurement_sgp40(void *args) {
    app_sgp40_measurement();
}

int32_t app_mesurementCb(void *args) {

//    print_time();
//    printf("app_mesurementCb\r\n");

    TL_SCHEDULE_TASK(mesurement_bme280, NULL);
    TL_SCHEDULE_TASK(mesurement_bh1750, NULL);
    TL_SCHEDULE_TASK(mesurement_scd4x, NULL);
    TL_SCHEDULE_TASK(mesurement_sgp40, NULL);
    TL_SCHEDULE_TASK(proc_co2_voc_onoff, NULL);

    return config.read_sensors_period * 1000;
}

void app_set_outside_temperature(int16_t temp) {
    outside_temperature = temp;
}

int16_t app_get_outside_temperature() {

    for(uint8_t i = 0; i < bind_outside_sensor.used; i++) {
        if (bind_outside_sensor.cluster_id[i] == ZCL_CLUSTER_MS_TEMPERATURE_MEASUREMENT) {
            return outside_temperature;
        }
    }
    return 0x8000;
}

void app_set_outside_humidity(uint16_t hum) {
    outside_humidity = hum;
}

uint16_t app_get_outside_humidity() {
    for(uint8_t i = 0; i < bind_outside_sensor.used; i++) {
        if (bind_outside_sensor.cluster_id[i] == ZCL_CLUSTER_MS_RELATIVE_HUMIDITY) {
            return outside_humidity;
        }
    }
    return 0xffff;
}

void app_set_outside_battery(uint8_t percent) {
    outside_battery_percent = percent;
}

uint8_t app_get_outside_battery() {
    for(uint8_t i = 0; i < bind_outside_sensor.used; i++) {
        if (bind_outside_sensor.cluster_id[i] == ZCL_CLUSTER_GEN_POWER_CFG) {
            return outside_battery_percent;
        }
    }
    return 0xff;
}

static int32_t bind_outside_timerCb(void *args) {

    print_time();
    printf("- bind_outside_timerCb\r\n");

    memset(&bind_outside_sensor, 0, sizeof(bind_outside_sensor_t));

    bind_outside_sensor.checkBindTimerEvt = NULL;
    return -1;
}

void bind_outside_update_timer() {
//    print_time();
//    printf("- bind_outside_update_timer\r\n");

    if (bind_outside_sensor.checkBindTimerEvt) {
        TL_ZB_TIMER_CANCEL(&bind_outside_sensor.checkBindTimerEvt);
    }
    bind_outside_sensor.checkBindTimerEvt = TL_ZB_TIMER_SCHEDULE(bind_outside_timerCb, NULL, BIND_TIMEOUT);

}

void bind_outside_init() {
    memset(&bind_outside_sensor, 0, sizeof(bind_outside_sensor_t));
}

outside_sensor_err_t bind_outsise_proc(uint16_t addr, uint16_t clusterId) {

    outside_sensor_err_t ret = OUTSIDE_SRC_CLUSTER_OK;

    if (bind_outside_sensor.used == 0) {
//        printf("New table\r\n");
        bind_outside_sensor.src_addr = addr;
        bind_outside_sensor.cluster_id[bind_outside_sensor.used] = clusterId;
        bind_outside_sensor.used++;
        return ret;
    }

    if (addr != bind_outside_sensor.src_addr) {
//        printf("OUTSIDE_SRC_ADDR_FAIL\r\n");
        return OUTSIDE_SRC_ADDR_FAIL;
    }

    for (uint8_t i = 0; i < BIND_CLUSTER_NUM; i++) {
        if (bind_outside_sensor.cluster_id[i] == clusterId) {
//            printf("OUTSIDE_SRC_CLUSTER_OK\r\n");
            return OUTSIDE_SRC_CLUSTER_OK;
        }
    }

    if ((bind_outside_sensor.used + 1) > BIND_CLUSTER_NUM) {
//        printf("OUTSIDE_SRC_TABLE_FULL\r\n");
        return OUTSIDE_SRC_TABLE_FULL;
    }

    bind_outside_sensor.cluster_id[bind_outside_sensor.used] = clusterId;
    bind_outside_sensor.used++;

//    printf("used: %d\r\n", bind_outside_sensor.used);

    return ret;
}
