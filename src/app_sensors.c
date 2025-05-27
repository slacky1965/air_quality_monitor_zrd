#include "app_main.h"

#define BIND_TIMEOUT  TIMEOUT_2HOUR //TIMEOUT_10MIN //

static int16_t  outside_temperature = 0x8000;
static uint16_t outside_humidity = 0xffff;
static uint8_t  outside_battery_percent = 0xff;

static uint8_t t_index = 0;
static uint16_t b_addr;

static bool sw_onoff = false;

static bind_outside_sensor_t bind_outside_sensor = {
        .used = 0,
        .checkBindTimerEvt = NULL,
};

static bind_outside_sensor_t bind_outside_sensor_tmp = {
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

static int32_t bind_outside_copy_timerCb(void *args) {

//    print_time();
//    printf("- bind_outside_copy_timerCb\r\n");

    memcpy(&bind_outside_sensor, &bind_outside_sensor_tmp, sizeof(bind_outside_sensor_t));

    bind_outside_sensor.checkBindTimerEvt = NULL;
    return -1;
}

static int32_t bind_outside_timerCb(void *args) {

    uint16_t *addr = (uint16_t*)args;

//    print_time();
//    printf("- bind_outside_timerCb\r\n");

    memset(&bind_outside_sensor_tmp, 0, sizeof(bind_outside_sensor_t));
    start_bind_scan(*addr, 0);

    bind_outside_sensor_tmp.checkBindTimerEvt = NULL;
    return -1;
}

static void parse_mqmt_bind(void *args) {

    zdo_zdpDataInd_t *p = (zdo_zdpDataInd_t *)args;
    zdo_mgmt_bind_resp_t *rsp = (zdo_mgmt_bind_resp_t *)p->zpdu;
    u8 num = rsp->bind_tbl_lst_cnt;
    uint16_t cluster;

    addrExt_t extAddr;
    zb_getLocalExtAddr(extAddr);

    u8 list_ptr = 5;

//    u8 r = irq_disable();

//    printf("local_addr: ");
//    for (uint8_t ii = 0; ii < 8; ii++) {
//        printf("0x%02x:", extAddr[ii]);
//    }
//    printf("\r\n");



    for (uint8_t i = 0; i < num; i++) {


        zdo_bindTabListRec_t *rsp_list = (zdo_bindTabListRec_t *)(p->zpdu + list_ptr);

        cluster = (rsp_list->cid16_h << 8) & 0xff00;
        cluster |= rsp_list->cid16_l;
//        printf("cluster: 0x%04x\r\n", cluster);
        list_ptr += 12;
        if (rsp_list->dst_addr_mode == 0x01) {
            list_ptr += 2;
//            printf("mode 01. dst_addr: 0x%04x", rsp_list->dst_group_addr);
        } else {
            list_ptr += 9;
            if (memcmp(extAddr, rsp_list->dst_ext_addr, 8) == 0) {
                bind_outside_sensor_tmp.cluster_id[bind_outside_sensor_tmp.used++] = cluster;
                memcpy(bind_outside_sensor_tmp.dst_extAddr.extAddr, extAddr, 8);
                memcpy(bind_outside_sensor_tmp.src_extAddr.extAddr, rsp_list->dst_ext_addr, 8);
                bind_outside_sensor_tmp.src_extAddr.addr = p->src_addr;
            }
//            printf("mode 03. dst_addr: ");
//            for (uint8_t ii = 0; ii < 8; ii++) {
//                printf("0x%02x:", rsp_list->dst_ext_addr[ii]);
//            }
//            printf("\r\n");
        }
    }

//    irq_restore(r);

    if (num) {
        start_bind_scan(b_addr, num);
    }

    if (bind_outside_sensor_tmp.used) {
        if (bind_outside_sensor_tmp.checkBindTimerEvt) {
            TL_ZB_TIMER_CANCEL(&bind_outside_sensor_tmp.checkBindTimerEvt);
        }
        bind_outside_sensor_tmp.checkBindTimerEvt = TL_ZB_TIMER_SCHEDULE(bind_outside_timerCb,
                                                                     &bind_outside_sensor_tmp.src_extAddr.addr,
                                                                     BIND_TIMEOUT);
    } else {
        if (bind_outside_sensor_tmp.checkBindTimerEvt) {
            TL_ZB_TIMER_CANCEL(&bind_outside_sensor_tmp.checkBindTimerEvt);
        }
    }

    if (bind_outside_sensor.checkBindTimerEvt) {
        TL_ZB_TIMER_CANCEL(&bind_outside_sensor.checkBindTimerEvt);
    }
    bind_outside_sensor.checkBindTimerEvt = TL_ZB_TIMER_SCHEDULE(bind_outside_copy_timerCb, NULL, TIMEOUT_10SEC);

//    print_time();
//    printf("- bind_outside_sensor_tmp.used: %d\r\n", bind_outside_sensor_tmp.used);
}

void start_bind_scan(uint16_t bindAddr, uint8_t idx) {

    static uint8_t start_index = 0;
//    bind_true = false;
    b_addr = bindAddr;

//    printf("addr: 0x%04x\r\n", bindAddr);

    if (!zb_isDeviceJoinedNwk()) {
        t_index = 0;
//        lqi = 0;
        return;
    }

    if (idx == 0) {
        start_index = t_index;
//        if (t_index == 0)
//            start_index = 0;
//        else
//            start_index = t_index;
    }

//    printf("idx: %d, t_index: %d\r\n", idx, t_index);
    start_index += idx;
    u8 sn = 0;
    zdo_mgmt_bind_req_t req;
    u16 dstAddr = bindAddr;
    req.start_index = start_index;
    zb_mgmtBindReq(dstAddr, &req, &sn, parse_mqmt_bind);
}

uint8_t bind_outside_check(uint16_t addr, uint16_t clId) {

    uint8_t ret = OUTSIDE_S_NO_CLUSTER;

    if (bind_outside_sensor_tmp.used == 0)
        return OUTSIDE_S_EMPTY;

    if (addr != bind_outside_sensor_tmp.src_extAddr.addr)
        return OUTSIDE_S_ADDR_FAIL;

    for (uint8_t i = 0; i < bind_outside_sensor_tmp.used; i++) {
        if (bind_outside_sensor_tmp.cluster_id[i] == clId) {
            ret = OUTSIDE_S_CLUSTER_OK;
        }
    }

    return ret;
}

void bind_outside_update_timer() {
//    print_time();
//    printf("- bind_outside_update_timer\r\n");

    if (bind_outside_sensor_tmp.checkBindTimerEvt) {
        TL_ZB_TIMER_CANCEL(&bind_outside_sensor_tmp.checkBindTimerEvt);
    }
    bind_outside_sensor_tmp.checkBindTimerEvt = TL_ZB_TIMER_SCHEDULE(bind_outside_timerCb,
                                                                 &bind_outside_sensor_tmp.src_extAddr.addr,
                                                                 BIND_TIMEOUT);

}

void bind_outside_init() {
    memset(&bind_outside_sensor_tmp, 0, sizeof(bind_outside_sensor_t));
}
