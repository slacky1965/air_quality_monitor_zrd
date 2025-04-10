#ifndef SRC_COMMON_BOARD_ZTU_DIY_H_
#define SRC_COMMON_BOARD_ZTU_DIY_H_

#if 1 //VOLTAGE_DETECT_ENABLE
#define VOLTAGE_DETECT_ADC_PIN      GPIO_PC5
#endif


/************************* Configure KEY GPIO ***************************************/
#define MAX_BUTTON_NUM  1

#define BUTTON1                 GPIO_PA0
#define PA0_FUNC                AS_GPIO
#define PA0_OUTPUT_ENABLE       OFF
#define PA0_INPUT_ENABLE        ON
#define PULL_WAKEUP_SRC_PA0     PM_PIN_PULLUP_10K //PM_PIN_PULLUP_1M //

//#define PM_WAKEUP_LEVEL         PM_WAKEUP_LEVEL_LOW

enum {
    VK_SW1 = 0x01,
};

#define KB_MAP_NORMAL   {\
        {VK_SW1,}}

#define KB_MAP_NUM      KB_MAP_NORMAL
#define KB_MAP_FN       KB_MAP_NORMAL

#define KB_DRIVE_PINS  {NULL }
#define KB_SCAN_PINS   {BUTTON1}

/************************ Configure SPI and other for EPD ***************************/

/* Reset pin */
#define EPD_RST                 GPIO_PD4
#define PD4_FUNC                AS_GPIO
#define PD4_OUTPUT_ENABLE       ON
#define PD4_INPUT_ENABLE        OFF

/* Busy pin */
#define EPD_BUSY                GPIO_PB5
#define PB5_FUNC                AS_GPIO
#define PB5_INPUT_ENABLE        ON
#define PB5_OUTPUT_ENABLE       OFF
#define PULL_WAKEUP_SRC_PB5     PM_PIN_PULLDOWN_100K

/* DC pin */
#define EPD_DC                  GPIO_PA1
#define PA1_FUNC                AS_GPIO
#define PA1_OUTPUT_ENABLE       ON
#define PA1_INPUT_ENABLE        OFF

/* CS pin */
#define EPD_CS                  GPIO_PD2
#define PD2_FUNC                AS_GPIO
#define PD2_OUTPUT_ENABLE       ON
#define PD2_INPUT_ENABLE        OFF

/* SPI Clock */
#define EPD_SPI_CLOCK          400000//400K

/*
 *  Define pin for SPI interface
 *          SDO  SDI  SCK  CN
 *          A2   A3   A4   D6
 *          B7   B6   D7   D2
 */
#define EPD_SPI_PIN_GROUP      SPI_GPIO_GROUP_B6B7D2D7
#define EPD_SPI_PIN_CS         EPD_CS

#define PULL_WAKEUP_SRC_PC2     PM_PIN_PULLUP_10K //PM_PIN_PULLUP_1M //
#define PULL_WAKEUP_SRC_PC3     PM_PIN_PULLUP_10K //PM_PIN_PULLUP_1M //


/********************* Configure fake LED for bootloader ***************************/

#if (__PROJECT_TL_BOOT_LOADER__)
#define LED_STATUS              GPIO_PD7
#define PD7_FUNC                AS_GPIO
#define PD7_OUTPUT_ENABLE       ON
#define PD7_INPUT_ENABLE        OFF

#define LED_POWER               GPIO_PC0
#define PC0_FUNC                AS_GPIO
#define PC0_OUTPUT_ENABLE       ON
#define PC0_INPUT_ENABLE        OFF

#define LED_PERMIT              LED_STATUS
#else

#define LED_RED                 GPIO_PC0
#define PC0_FUNC                AS_GPIO
#define PC0_OUTPUT_ENABLE       ON
#define PC0_INPUT_ENABLE        OFF

#define LED_GREEN               GPIO_PC1
#define PC1_FUNC                AS_GPIO
#define PC1_OUTPUT_ENABLE       ON
#define PC1_INPUT_ENABLE        OFF

#define LED_BLUE                GPIO_PD3
#define PD3_FUNC                AS_GPIO
#define PD3_OUTPUT_ENABLE       ON
#define PD3_INPUT_ENABLE        OFF
#endif

///************************* Configure Temperature ***********************************/
//#define GPIO_TEMP               GPIO_PC3
//#define PC3_FUNC                AS_GPIO
////#define PULL_WAKEUP_SRC_PC3     PM_PIN_PULLUP_1M


#if UART_PRINTF_MODE
#define DEBUG_INFO_TX_PIN       UART_TX_PB1    //printf
#define DEBUG_BAUDRATE          115200

#endif /* UART_PRINTF_MODE */

#endif /* SRC_COMMON_BOARD_ZTU_DIY_H_ */
