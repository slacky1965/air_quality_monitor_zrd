#include "app_main.h"

#include "zbhci.h"

static int16_t  outside_temperature = 0x8000;
static uint16_t outside_humidity = 0xffff;

static uint8_t t_index = 0;
static uint16_t b_addr;

static bind_outside_sensor_t bind_outside_sensor = {
        .used = 0,
        .checkBindTimerEvt = NULL,
};

static void mesurement_bme280(void *args) {
    app_bme280_measurement();
}

static void mesurement_bh1750(void *args) {
    app_bh1750_measurement();
}

static void mesurement_scd4x(void *args) {
    app_scd4x_measurement();
}

int32_t app_mesurementCb(void *args) {

    TL_SCHEDULE_TASK(mesurement_bme280, NULL);
    TL_SCHEDULE_TASK(mesurement_bh1750, NULL);
    TL_SCHEDULE_TASK(mesurement_scd4x, NULL);

    return config.mesurement_period * 1000;
}

void app_set_outside_temperature(int16_t temp) {
    outside_temperature = temp;
}

int16_t app_get_outside_temperature() {
    return outside_temperature;
}

void app_set_outside_humidity(uint16_t hum) {
    outside_humidity = hum;
}

uint16_t app_get_outside_humidity() {
    return outside_humidity;
}


static int32_t bind_outside_timerCb(void *args) {

    uint16_t *addr = (uint16_t*)args;

    printf("bind_outside_timerCb\r\n");

    memset(&bind_outside_sensor, 0, sizeof(bind_outside_sensor_t));
    start_bind_scan(*addr, 0);

    bind_outside_sensor.checkBindTimerEvt = NULL;
    return -1;
}

static void parse_mqmt_bind(void *args) {

    zdo_zdpDataInd_t *p = (zdo_zdpDataInd_t *)args;
    zdo_mgmt_bind_resp_t *rsp = (zdo_mgmt_bind_resp_t *)p->zpdu;
    u8 num = rsp->bind_tbl_lst_cnt;
    uint16_t cluster;

//    uint16_t local_addr = zb_getLocalShortAddr();

    addrExt_t extAddr;
    zb_getLocalExtAddr(extAddr);

    u8 list_ptr = 5;

    u8 r = irq_disable();

    printf("local_addr: ");
    for (uint8_t ii = 0; ii < 8; ii++) {
        printf("0x%02x:", extAddr[ii]);
    }
    printf("\r\n");



    for (uint8_t i = 0; i < num; i++) {


        zdo_bindTabListRec_t *rsp_list = (zdo_bindTabListRec_t *)(p->zpdu + list_ptr);

        //ZB_LEBESWAP((rsp_list->src_addr), 8);
//        printf("index: %d, cnt: %d, src_addr: ", rsp->start_index, i);
//        for (uint8_t ii = 0; ii < 8; ii++) {
//            printf("0x%02x:", rsp_list->src_addr[ii]);
//        }
//        printf("\r\n");
//        u8 cidH = rsp_list->cid16_h;
//        u8 cidL = rsp_list->cid16_l;
//        rsp_list->cid16_l = cidH;
//        rsp_list->cid16_h = cidL;
        cluster = (rsp_list->cid16_h << 8) & 0xff00;
        cluster |= rsp_list->cid16_l;
        printf("cluster: 0x%04x\r\n", cluster);
        list_ptr += 12;
        if (rsp_list->dst_addr_mode == 0x01) {
//            ZB_LEBESWAP(((u8 *)&rsp_list->dst_group_addr), 2);
            list_ptr += 2;
//            printf("mode 01. dst_addr: 0x%04x", rsp_list->dst_group_addr);
        } else {
//            ZB_LEBESWAP((rsp_list->dst_ext_addr), 8);
            list_ptr += 9;
            if (memcmp(extAddr, rsp_list->dst_ext_addr, 8) == 0) {
                bind_outside_sensor.cluster_id[bind_outside_sensor.used++] = cluster;
                memcpy(bind_outside_sensor.dst_extAddr.extAddr, extAddr, 8);
                memcpy(bind_outside_sensor.src_extAddr.extAddr, rsp_list->dst_ext_addr, 8);
                bind_outside_sensor.src_extAddr.addr = p->src_addr;
            }
            printf("mode 03. dst_addr: ");
            for (uint8_t ii = 0; ii < 8; ii++) {
                printf("0x%02x:", rsp_list->dst_ext_addr[ii]);
            }
            printf("\r\n");
        }
    }

    irq_restore(r);

    if (num) {
        start_bind_scan(b_addr, num);
    }

    if (bind_outside_sensor.used) {
        if (bind_outside_sensor.checkBindTimerEvt) {
            TL_ZB_TIMER_CANCEL(&bind_outside_sensor.checkBindTimerEvt);
        }
        bind_outside_sensor.checkBindTimerEvt = TL_ZB_TIMER_SCHEDULE(bind_outside_timerCb,
                                                                     &bind_outside_sensor.src_extAddr.addr,
                                                                     TIMEOUT_30SEC/*TIMEOUT_2HOUR*/);
    } else {
        if (bind_outside_sensor.checkBindTimerEvt) {
            TL_ZB_TIMER_CANCEL(&bind_outside_sensor.checkBindTimerEvt);
        }
    }

    printf("bind_outside_sensor.used: %d\r\n", bind_outside_sensor.used);
}

void start_bind_scan(uint16_t bindAddr, uint8_t idx) {

    static uint8_t start_index = 0;
//    bind_true = false;
    b_addr = bindAddr;

    printf("addr: 0x%04x\r\n", bindAddr);

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

    if (bind_outside_sensor.used == 0)
        return OUTSIDE_S_EMPTY;

    if (addr != bind_outside_sensor.src_extAddr.addr)
        return OUTSIDE_S_ADDR_FAIL;

    for (uint8_t i = 0; i < bind_outside_sensor.used; i++) {
        if (bind_outside_sensor.cluster_id[i] == clId) {
            ret = OUTSIDE_S_CLUSTER_OK;
        }
    }

    return ret;
}

void bind_outside_update_timer() {

    if (bind_outside_sensor.checkBindTimerEvt) {
        TL_ZB_TIMER_CANCEL(&bind_outside_sensor.checkBindTimerEvt);
    }
    bind_outside_sensor.checkBindTimerEvt = TL_ZB_TIMER_SCHEDULE(bind_outside_timerCb,
                                                                 &bind_outside_sensor.src_extAddr.addr,
                                                                 TIMEOUT_30SEC/*TIMEOUT_2HOUR*/);

}

void bind_outside_init() {
    memset(&bind_outside_sensor, 0, sizeof(bind_outside_sensor_t));
}
