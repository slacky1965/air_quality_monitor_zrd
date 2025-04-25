#include "tl_common.h"
#include "zcl_include.h"

#include "app_main.h"

//uint8_t mcuBootAddrGet(void);
uint32_t mcuBootAddrGet(void);

void start_message() {

#ifdef ZCL_OTA
#if UART_PRINTF_MODE
        printf("OTA mode enabled. MCU boot from address: 0x%x\r\n", mcuBootAddrGet());
#endif /* UART_PRINTF_MODE */
#else
#if UART_PRINTF_MODE
    printf("OTA mode desabled. MCU boot from address: 0x%x\r\n", mcuBootAddrGet());
#endif /* UART_PRINTF_MODE */
#endif

#if UART_PRINTF_MODE
    const uint8_t version[] = ZCL_BASIC_SW_BUILD_ID;
    printf("Firmware version: %s\r\n", version+1);
#endif
}


int32_t poll_rateAppCb(void *arg) {

    uint32_t poll_rate = zb_getPollRate();

    if (poll_rate == g_appCtx.long_poll) {
        zb_setPollRate(g_appCtx.short_poll);
        return TIMEOUT_30SEC;
    }

    zb_setPollRate(g_appCtx.long_poll);

    return g_appCtx.long_poll;
}

int32_t delayedMcuResetCb(void *arg) {

    //printf("mcu reset\r\n");
    zb_resetDevice();
    return -1;
}

int32_t delayedFullResetCb(void *arg) {

    //printf("full reset\r\n");
    return -1;
}

int32_t fakeTimer500msCb(void *arg) {

    return 0;
}

char *strcat(char *str1, const char *str2) {
    char *str = str1;
    if (str1 == NULL)
        return NULL;

    if (str2 == NULL || strlen(str2) == 0)
        return str1;

    uint32_t l = strlen(str1);
    uint32_t i = 0;

    for (i = 0; i < strlen(str2); i++) {
        *(str1+l+i) = *(str2+i);
    }
    *(str1+l+i) = '\0';

    return str;
}

/* Function return size of string and convert signed  *
 * integer to ascii value and store them in array of  *
 * character with NULL at the end of the array        */

uint8_t *itoa(int32_t value, uint8_t *ptr) {
    uint32_t count = 0, temp;
    uint8_t *str = ptr;

    if(ptr == NULL)
        return NULL;
        //return 0;
    if(value == 0)
    {
        *ptr++ = '0';
        *ptr = '\0';
        return str;
        //return 1;
    }

    if(value < 0)
    {
        value *= (-1);
        *ptr++ = '-';
        count++;
    }

    for(temp=value; temp>0; temp/=10, ptr++);
    *ptr = '\0';

    for(temp=value; temp>0; temp/=10)
    {
        *--ptr = temp%10 + '0';
        count++;
    }
    return str;
//    return count;
}



uint32_t reverse32(uint32_t in) {
    uint32_t out;
    uint8_t *source = (uint8_t*)&in;
    uint8_t *destination = (uint8_t*)&out;

    destination[3] = source[0];
    destination[2] = source[1];
    destination[1] = source[2];
    destination[0] = source[3];

    return out;
}

uint16_t reverse16(uint16_t in) {
    uint16_t out;
    uint8_t *source = (uint8_t*)&in;
    uint8_t *destination = (uint8_t*)&out;

    destination[1] = source[0];
    destination[0] = source[1];

    return out;
}

int32_t int32_from_str(uint8_t *data) {

    int32_t val = 0;

    val = data[0] << 24;
    val |= data[1] << 16;
    val |= data[2] << 8;
    val |= data[3];

    return val;
}

int16_t int16_from_str(uint8_t *data) {

    int16_t val = 0;

    val |= data[0] << 8;
    val |= data[1];

    return val;
}

uint8_t set_zcl_str(uint8_t *str_in, uint8_t *str_out, uint8_t len) {
    uint8_t *data = str_out;
    uint8_t *str_len = data;
    uint8_t *str_data = data+1;

    for (uint8_t i = 0; *(str_in+i) != 0 && i < (len-1); i++) {
        *(str_data+i) = *(str_in+i);
        (*str_len)++;
    }

    return *str_len;
}

//char * mystrstr(char * mainStr, char * subStr) {
//    char *s1, *s2;
//
//    // Iterate over the main string
//    while( *mainStr != NULL)
//    {
//        // search for the first character in substring
//        if(*mainStr == *subStr)
//        {
//            // The first character of substring is matched.
//            // Check if all characters of substring.
//            s1 = mainStr;
//            s2 = subStr;
//
//            while (*s1 && *s2 )
//            {
//                if(*s1 != *s2)
//                    break;      // not matched
//
//                s1++;
//                s2++;
//            }
//
//            // we reached end of subStr
//            if(*s2 == NULL)
//                return mainStr;
//        }
//
//        // go to next element
//        mainStr++;
//    }
//
//    return NULL;
//}
