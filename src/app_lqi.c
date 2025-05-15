#include "app_main.h"

static uint8_t lqi = 0;
static bool lqi_true = false;
static uint8_t t_index = 0;

static void start_lqi_scan(uint8_t idx);

static void parse_mqmt_lqi(void *args) {

    zdo_zdpDataInd_t *p = (zdo_zdpDataInd_t *)args;
    zdo_mgmt_lqi_resp_t *rsp = (zdo_mgmt_lqi_resp_t *)p->zpdu;

    uint8_t n_tbl_cnt = rsp->neighbor_tbl_lst_cnt;

    uint16_t src_addr = zb_getLocalShortAddr();

    for (uint8_t i = 0; i < n_tbl_cnt; i++) {
        if (src_addr == rsp->neighbor_tbl_lst[i].network_addr) {
            lqi = rsp->neighbor_tbl_lst[i].lqi;
            lqi_true = true;
            t_index = rsp->start_index;
//            printf("LQI: %d, start_index: %d\r\n", lqi, rsp->start_index);
            return;
        }
    }

    if (n_tbl_cnt) {
        start_lqi_scan(n_tbl_cnt);
    }

    if (!lqi_true)
        t_index = 0;
}

static void start_lqi_scan(uint8_t idx) {

    static uint8_t start_index = 0;
    lqi_true = false;

    if (!zb_isDeviceJoinedNwk()) {
        t_index = 0;
        lqi = 0;
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
    zdo_mgmt_lqi_req_t req;
    u16 dstAddr = 0x0000;
    req.start_index = start_index;
    zb_mgmtLqiReq(dstAddr, &req, &sn, parse_mqmt_lqi);
}

uint8_t app_get_lqi() {
    if (lqi_true) {
        return lqi;
    }

    return 0;
}

int32_t app_lqiCb(void *args) {

//    printf("app_lqiCb()\r\n");

    start_lqi_scan(0);

    if (lqi == 0)
        return TIMEOUT_250MS;

    return TIMEOUT_1MIN;
}
