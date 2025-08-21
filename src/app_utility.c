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
    lifetime_save();
    zb_resetDevice();
    return -1;
}

int32_t delayedFullResetCb(void *arg) {

    //printf("full reset\r\n");
    return -1;
}

int32_t delayedFactoryResetCb(void *arg) {

    zb_resetDevice2FN();
    zb_deviceFactoryNewSet(true);

    factory_reset = true;
    g_appCtx.timerFactoryReset = NULL;

    printf("Cb Factory new: %s\r\n", zb_isDeviceFactoryNew()?"yes":"no");


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

uint8_t *digit64toString(uint64_t value) {
    static uint8_t buff[32] = {0};
    uint8_t *buffer = buff;
    buffer += 17;
    *--buffer = 0;
    do {
        *--buffer = value % 10 + '0';
        value /= 10;
    } while (value != 0);

    return buffer;
}

#define LN10 2.3025850929940456840179914546844

float ln(float x) {
    union {float f; uint32_t i;} u = {x};
    uint32_t bx = u.i;
    int32_t ex = bx >> 23;
    int32_t t = ex - 127;
    uint32_t s = (t < 0) ? (-t) : t;
    bx = 0x3F800000 | (bx & 0x7FFFFF);
    u.i = bx;
    x = u.f;
    return -1.49278 + (2.11263 + (-0.729104 + 0.10969 * x) * x) * x + 0.6931471806 * s;
}

double log10(double x) {
    return ln(x) / LN10;
}

int32_t temperature_convert_CtoF(int16_t temp) {

    float tempF, tempC;

    tempC = (float)temp / 100.0;

    tempF = ((tempC * 9 / 5) + 32) * 100;

    return (int32_t)tempF;
}

////#define log_10_of_2 (0.30102999566)
//#define log_e_of_2 (0.69314718056)
//#define log_10_of_2 (0.30102999566)
//
//unsigned int nbbits(unsigned int n)
//{
//    unsigned int count_bits = 0;
//    while (n != 0)
//    {
//        n = n >> 1;
//        ++count_bits;
//    }
//    return count_bits;
//}
//
//double approx_decimal_log(double x)    /* assume x > 0 */
//{
//    if (x < 1)
//        return -approx_decimal_log(1/x);
//    return (nbbits((unsigned int) x) - 1.0) * log_10_of_2;
//}

//#ifndef INFINITY
//#define INFINITY (1.0f/0.0f)
//#endif
//
//static const float LOG10_2 = 0.30102999f;  // log10(2) константа
//static const int N_SEGMENTS = 32;         // число сегментов на [1,2)
//static const float log10_table[33] = {
//    // Предвычисленные значения log10(1 + i/32) для i = 0..32
//    0.00000f, 0.01334f, 0.02653f, 0.03958f, 0.05249f, 0.06524f, 0.07784f, 0.09027f,
//    0.10255f, 0.11466f, 0.12659f, 0.13836f, 0.14995f, 0.16137f, 0.17261f, 0.18377f,
//    0.19467f, 0.20545f, 0.21584f, 0.22610f, 0.23607f, 0.24583f, 0.25527f, 0.26453f,
//    0.27337f, 0.28199f, 0.29018f, 0.29806f, 0.30546f, 0.31254f, 0.31915f, 0.32549f,
//    0.33148f  // log10(2) ≈ 0.30103, последний элемент ~ log10(2) (чуть больше из-за интервала включения)
//};
//
//float fast_log10(float x) {
//    // Предполагаем x > 0. Можно добавить проверку и обработку x <= 0 при необходимости.
//    union { float f; uint32_t i; } u;
//    u.f = x;
//    uint32_t bits = u.i;
//
//    // Извлекаем порядок и мантиссу
//    int exp2 = (int)((bits >> 23) & 0xFF);
//    if (exp2 == 0) {
//        // x субнормальный (очень маленький): нормализация
//        if (x == 0.0f) return -INFINITY;   // log10(0) -> -бесконечность, если требуется
//        // для субнормальных чисел – привести к нормальному виду
//        // (на практике для диапазона [0.1,10] это не встречается)
//        exp2 = 1;
//    }
//    int E = exp2 - 127;                // действительный экспонент (порядок двоичный)
//    uint32_t mantissa_bits = bits & 0x7FFFFF;
//
//    // Вычисляем индекс сегмента по старшим битам мантиссы
//    // N_SEGMENTS=32 -> используем верхние 5 бит мантиссы (2^5 = 32 сегмента)
//    int idx = mantissa_bits >> (23 - 5);            // верхние 5 бит из 23
//    float log10_low = log10_table[idx];
//    float log10_high = log10_table[idx + 1];
//
//    // Доля внутри сегмента (0.0 до 1.0)
//    // Используем оставшиеся 18 бит мантиссы для дробной части прогресса по сегменту
//    uint32_t frac_bits = mantissa_bits & ((1u << (23 - 5)) - 1);
//    float frac = (float)frac_bits * (1.0f / (1u << (23 - 5)));
//
//    // Линейная интерполяция между log10_table[idx] и log10_table[idx+1]
//    float log10_mant = log10_low + (log10_high - log10_low) * frac;
//
//    // Добавляем вклад порядка: E * log10(2)
//    float result = log10_mant + ((float)E * LOG10_2);
//    return result;
//}


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
//
//
//#define LOG10_2 0.30102999566f  // log10(2)
//#define N_SEGMENTS 32           // Количество сегментов (5 бит)
//
//static const float log10_table[N_SEGMENTS + 1] = {
//    0.00000f, 0.01334f, 0.02653f, 0.03958f, 0.05249f, 0.06524f, 0.07784f, 0.09027f,
//    0.10255f, 0.11466f, 0.12659f, 0.13836f, 0.14995f, 0.16137f, 0.17261f, 0.18377f,
//    0.19467f, 0.20545f, 0.21584f, 0.22610f, 0.23607f, 0.24583f, 0.25527f, 0.26453f,
//    0.27337f, 0.28199f, 0.29018f, 0.29806f, 0.30546f, 0.31254f, 0.31915f, 0.32549f,
//    0.33148f // log10(2) ≈ 0.30103, чуть выше на край
//};
//
//// Подобранная табличка поправок для каждого сегмента
//static const float curvature_table[N_SEGMENTS] = {
//    0.00050f, 0.00048f, 0.00046f, 0.00044f, 0.00042f, 0.00040f, 0.00038f, 0.00037f,
//    0.00036f, 0.00035f, 0.00034f, 0.00033f, 0.00032f, 0.00031f, 0.00030f, 0.00029f,
//    0.00028f, 0.00027f, 0.00026f, 0.00025f, 0.00024f, 0.00023f, 0.00022f, 0.00021f,
//    0.00020f, 0.00019f, 0.00018f, 0.00017f, 0.00016f, 0.00015f, 0.00014f, 0.00013f
//};
//
//float fast_log10_quadratic(float x) {
//    union { float f; uint32_t i; } u;
//    u.f = x;
//    uint32_t bits = u.i;
//
//    int exp2 = (int)((bits >> 23) & 0xFF);
//    int E = exp2 - 127; // Реальный порядок двойки
//    uint32_t mantissa_bits = bits & 0x7FFFFF;
//
//    int idx = mantissa_bits >> (23 - 5); // 5 старших бит — индекс сегмента
//    float log10_low = log10_table[idx];
//    float log10_high = log10_table[idx + 1];
//
//    uint32_t frac_bits = mantissa_bits & ((1u << (23 - 5)) - 1);
//    float frac = (float)frac_bits * (1.0f / (1u << (23 - 5))); // От 0 до 1 в сегменте
//
//    // Линейная интерполяция
//    float base = log10_low + (log10_high - log10_low) * frac;
//
//    // Квадратичная поправка
//    float curvature = curvature_table[idx];
//    base += curvature * frac * (1.0f - frac);
//
//    // Учет степени двойки
//    return base + ((float)E * LOG10_2);
//}
