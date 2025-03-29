#include "app_main.h"

/* data point for manufacturer id -
 * "ogkdpgy2"
 *
 * id, type, len, divisor, remote_commands_functionCb, local_commands_functionCb
*/

data_point_st_t data_point_model1[DP_IDX_MAXNUM] = {
        {DP_TYPE1_ID_02, DP_VAL,  4,    1,  NULL, local_cmd_co2_1},                                // co2
};


