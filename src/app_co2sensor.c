#include "app_main.h"

uint8_t zb_modelId_arr[ZB_MODELID_ARR_NUM][ZB_MODELID_FULL_SIZE] = {
        {ZB_MODELID_SIZE, 'T','u','y','a','_','C','O','2','S','e','n','s','o','r','_','r','0','1',0},   // model1
        {ZB_MODELID_SIZE, 'T','u','y','a','_','C','O','2','S','e','n','s','o','r','_','r','0','2',0},   // model2
        {ZB_MODELID_SIZE, 'T','u','y','a','_','C','O','2','S','e','n','s','o','r','_','r','0','3',0},
        {ZB_MODELID_SIZE, 'T','u','y','a','_','C','O','2','S','e','n','s','o','r','_','r','0','4',0},
        {ZB_MODELID_SIZE, 'T','u','y','a','_','C','O','2','S','e','n','s','o','r','_','r','0','5',0},
        {ZB_MODELID_SIZE, 'T','u','y','a','_','C','O','2','S','e','n','s','o','r','_','r','0','6',0},
        {ZB_MODELID_SIZE, 'T','u','y','a','_','C','O','2','S','e','n','s','o','r','_','r','0','7',0},
        {ZB_MODELID_SIZE, 'T','u','y','a','_','C','O','2','S','e','n','s','o','r','_','r','0','8',0},
        {ZB_MODELID_SIZE, 'T','u','y','a','_','C','O','2','S','e','n','s','o','r','_','r','0','9',0},
        {ZB_MODELID_SIZE, 'T','u','y','a','_','C','O','2','S','e','n','s','o','r','_','r','0','A',0},
        {ZB_MODELID_SIZE, 'T','u','y','a','_','C','O','2','S','e','n','s','o','r','_','r','0','B',0},
        {ZB_MODELID_SIZE, 'T','u','y','a','_','C','O','2','S','e','n','s','o','r','_','r','0','C',0},
        {ZB_MODELID_SIZE, 'T','u','y','a','_','C','O','2','S','e','n','s','o','r','_','r','0','D',0},
        {ZB_MODELID_SIZE, 'T','u','y','a','_','C','O','2','S','e','n','s','o','r','_','r','0','E',0},
        {ZB_MODELID_SIZE, 'T','u','y','a','_','C','O','2','S','e','n','s','o','r','_','r','0','F',0},
        {ZB_MODELID_SIZE, 'T','u','y','a','_','C','O','2','S','e','n','s','o','r','_','r','1','0',0},  // etc
};

uint8_t remote_cmd_pkt_buff[DATA_MAX_LEN+12];

void local_cmd_co2(void *args) {

    int32_t *co2 = (int32_t*)args;
    uint16_t divisor = 1;

    if (data_point_model[DP_IDX_CO2].divisor == 10) {
        divisor = 10;
    } else if (data_point_model[DP_IDX_CO2].divisor == 100) {
        divisor = 100;
    }

    *co2 *= divisor;

    float attrCo2 = (float)*co2/1000000;

    zcl_setAttrVal(APP_ENDPOINT1, ZCL_CLUSTER_MS_CO2_MEASUREMENT, ZCL_CO2_MEASUREMENT_ATTRID_MEASUREDVALUE, (uint8_t*)&attrCo2);

#if UART_PRINTF_MODE && DEBUG_CMD
            printf("PPM: %d\r\n", *co2);
#endif

}


