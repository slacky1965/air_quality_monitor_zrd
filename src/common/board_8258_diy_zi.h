#ifndef SRC_INCLUDE_BOARD_8258_DIY_H_
#define SRC_INCLUDE_BOARD_8258_DIY_H_

/************************* Configure KEY GPIO ***************************************/
#define MAX_BUTTON_NUM  1

#define BUTTON1                 GPIO_PC1
#define PC1_FUNC                AS_GPIO
#define PC1_OUTPUT_ENABLE       OFF
#define PC1_INPUT_ENABLE        ON
#define PULL_WAKEUP_SRC_PC1     PM_PIN_PULLUP_1M //PM_PIN_PULLUP_10K //

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
#define EPD_RST                 GPIO_PC0
#define PC0_FUNC                AS_GPIO
#define PC0_OUTPUT_ENABLE       ON
#define PC0_INPUT_ENABLE        OFF

/* Busy pin */
#define EPD_BUSY                GPIO_PB1
#define PB1_INPUT_ENABLE        ON
#define PB1_FUNC                AS_GPIO
#define PULL_WAKEUP_SRC_PB1     PM_PIN_PULLUP_1M

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



/********************* Configure fake LED for bootloader ***************************/

#define LED_STATUS              GPIO_PB4
#define PB4_FUNC                AS_GPIO
#define PB4_OUTPUT_ENABLE       ON
#define PB4_INPUT_ENABLE        OFF

#define LED_POWER               GPIO_PA0
#define PA0_FUNC                AS_GPIO
#define PA0_OUTPUT_ENABLE       ON
#define PA0_INPUT_ENABLE        OFF

#define LED_PERMIT              LED_STATUS


///**************************** Configure LED ******************************************/
//
//#define LED_STATUS              GPIO_PB6
//#define PB6_FUNC                AS_GPIO
//#define PB6_OUTPUT_ENABLE       ON
//#define PB6_INPUT_ENABLE        OFF
//
//#define LED_POWER               GPIO_PB7
//#define PB7_FUNC                AS_GPIO
//#define PB7_OUTPUT_ENABLE       ON
//#define PB7_INPUT_ENABLE        OFF
//
//#define LED_PERMIT              LED_STATUS

///************************* Configure Temperature ***********************************/
//#define GPIO_TEMP               GPIO_PC3
//#define PC3_FUNC                AS_GPIO
////#define PULL_WAKEUP_SRC_PC3     PM_PIN_PULLUP_1M

/**************************** Configure UART ***************************************
*    UART_TX_PA2 = GPIO_PA2,
*    UART_TX_PB1 = GPIO_PB1,
*    UART_TX_PC2 = GPIO_PC2,
*    UART_TX_PD0 = GPIO_PD0,
*    UART_TX_PD3 = GPIO_PD3,
*    UART_TX_PD7 = GPIO_PD7,
*    UART_RX_PA0 = GPIO_PA0,
*    UART_RX_PB0 = GPIO_PB0,
*    UART_RX_PB7 = GPIO_PB7,
*    UART_RX_PC3 = GPIO_PC3,
*    UART_RX_PC5 = GPIO_PC5,
*    UART_RX_PD6 = GPIO_PD6,
*/

///**************************** UART for optoport ***********************************/
//#define BAUDRATE_UART           9600
//#define GPIO_UART_TX            UART_TX_PD7
//#define GPIO_UART_RX            UART_RX_PA0

#if UART_PRINTF_MODE
#define DEBUG_INFO_TX_PIN       GPIO_PD3    //printf
#define DEBUG_BAUDRATE          115200

#endif /* UART_PRINTF_MODE */

#endif /* SRC_INCLUDE_BOARD_8258_DIY_H_ */
