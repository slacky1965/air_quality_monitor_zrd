#include "zcl_include.h"
#include "app_main.h"

#ifdef ZCL_ANALOG_INPUT

_CODE_ZCL_ status_t zcl_analog_input_register(u8 endpoint, u16 manuCode, u8 attrNum, const zclAttrInfo_t attrTbl[], cluster_forAppCb_t cb)
{
    return zcl_registerCluster(endpoint, ZCL_CLUSTER_GEN_ANALOG_INPUT_BASIC, manuCode, attrNum, attrTbl, NULL, cb);
}

#endif
