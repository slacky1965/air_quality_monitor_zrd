#include "app_main.h"

enum {
    LQI_NONE = 0,
    LQI_PARENT,
    LQI_SIBLING,
};

static uint8_t lqi = 0;
static bool lqi_true = false;
static uint8_t parent;
static uint8_t t_index = 0;

static void start_lqi_scan(void *args);

static int32_t parentTimerCb(void *args) {

    if (parent == LQI_NONE) {
        parent = LQI_SIBLING;
        uint8_t idx = 0;
        TL_SCHEDULE_TASK(start_lqi_scan, (void *)((uint32_t)idx));
    }

    return -1;
}

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
            parent++;
//            u32 r = drv_disable_irq();
//            printf("LQI: %d, start_index: %d, parent: %d\r\n", lqi, rsp->start_index, parent);
//            drv_restore_irq(r);
            return;
        }
    }

    if (n_tbl_cnt) {
        TL_SCHEDULE_TASK(start_lqi_scan, (void *)((uint32_t)n_tbl_cnt));
    }

    if (!lqi_true) {
        t_index = 0;
    }
}

static void start_lqi_scan(void *args) {

    uint8_t idx = (uint8_t)((uint32_t)args);

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

//    tl_zb_normal_neighbor_entry_t *active_head = g_zb_neighborTbl.activeHead;
//    printf("actHead addr: 0x%04x\r\n", g_nwkAddrMap.addrMap[active_head->addrmapIdx].shortAddr);

    for (uint8_t i = 0; i < g_zb_neighborTbl.normalNeighborNum; i++) {
        if (g_zb_neighborTbl.neighborTbl[i].relationship == NEIGHBOR_IS_PARENT && parent == LQI_NONE) {
            dstAddr = g_nwkAddrMap.addrMap[g_zb_neighborTbl.neighborTbl[i].addrmapIdx].shortAddr;
//            printf("addr: 0x%04x\r\n", dstAddr);
        } else if (g_zb_neighborTbl.neighborTbl[i].relationship == NEIGHBOR_IS_SIBLING && parent == LQI_SIBLING) {
            dstAddr = g_nwkAddrMap.addrMap[g_zb_neighborTbl.neighborTbl[i].addrmapIdx].shortAddr;
//            printf("addr: 0x%04x\r\n", dstAddr);
        }
    }

    req.start_index = start_index;
    zb_mgmtLqiReq(dstAddr, &req, &sn, parse_mqmt_lqi);
    TL_ZB_TIMER_SCHEDULE(parentTimerCb, NULL, TIMEOUT_900MS);
}

uint8_t app_get_lqi() {
    if (lqi_true) {
        return lqi;
    }

    return 0;
}

int32_t app_lqiCb(void *args) {

    parent = LQI_NONE;
    uint8_t idx = 0;
    TL_SCHEDULE_TASK(start_lqi_scan, (void *)((uint32_t)idx));

    if (lqi == 0)
        return TIMEOUT_5SEC; //TIMEOUT_250MS;

    return TIMEOUT_1MIN;
}
