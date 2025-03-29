#ifndef SRC_COMMON_BOARD_TUYA_H_
#define SRC_COMMON_BOARD_TUYA_H_

/**************************** UART for Tuya ZT3L ***********************************
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
#define GPIO_UART_TX            UART_TX_PB1
#define GPIO_UART_RX            UART_RX_PB7

/**************************** Configure UART ***************************************/
#if UART_PRINTF_MODE
#define DEBUG_INFO_TX_PIN       GPIO_PA0
#define DEBUG_BAUDRATE          115200
#endif /* UART_PRINTF_MODE */


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



#endif /* SRC_COMMON_BOARD_TUYA_H_ */
