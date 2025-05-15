#include "zcl_include.h"
#include "zcl_thermostat_ui_cfg.h"

static status_t zcl_thermostat_ui_cfg_cmdHandler(zclIncoming_t *pInMsg);


_CODE_ZCL_ status_t zcl_thermostat_ui_cfg_register(u8 endpoint, u16 manuCode, u8 arrtNum, const zclAttrInfo_t attrTbl[], cluster_forAppCb_t cb)
{
    return zcl_registerCluster(endpoint, ZCL_CLUSTER_HAVC_USER_INTERFACE_CONFIG, manuCode, arrtNum, attrTbl, zcl_thermostat_ui_cfg_cmdHandler, cb);
}

_CODE_ZCL_ static status_t zcl_thermostat_ui_cfg_cmdHandler(zclIncoming_t *pInMsg) {
//    printf("zcl_thermostat_ui_cfg_cmdHandler\r\n");
    if(pInMsg->hdr.frmCtrl.bf.dir == ZCL_FRAME_CLIENT_SERVER_DIR){
//        return zcl_thermostat_clientCmdHandler(pInMsg);
    }else{
//        return zcl_thermostat_serverCmdHandler(pInMsg);
    }

    return ZCL_STA_SUCCESS;
}

//uint8_t zcl_convert_temperature_fromCtoF(int16_t tempC) {
//
//    return (((tempC*18 + 250)/500 + 64)/2);
//}
//
//int16_t zcl_convert_temperature_fromFtoC(int16_t tempF) {
//
//    return (((tempF*2 - 64)*5000 + 90)/180);
//}
//
