#ifndef UART_DEFS_H
#define UART_DEFS_H

#include <core/common.h>
#include <definitions/gpio_defs.h>

#define NVIC_UART1_OFFSET 27
#define NVIC_UART2_OFFSET 28
#define NVIC_UART3TO8_OFFSET 29

#define UART1_BASE 0x40013800UL
#define UART2_BASE 0x40004400UL
#define UART3_BASE 0x40004800UL
#define UART4_BASE 0x40004C00UL
#define UART5_BASE 0x40005000UL
#define UART6_BASE 0x40011400UL
#define UART7_BASE 0x40011800UL
#define UART8_BASE 0x40011C00UL

#define UART_NVIC_SET_ENABLE_REG 0xE000E100UL
#define UART NVIC_CLEAR_ENABLE_REG 0xE000E180UL

#define UART1_CLOCK_SOURCE_OFFSET 0
#define UART2_CLOCK_SOURCE_OFFSET 16
#define UART3_CLOCK_SOURCE_OFFSET 18

#define UART1_APB_ENABLE_OFFSET 14
#define UART2_APB_ENABLE_OFFSET 17
#define UART3_APB_ENABLE_OFFSET 18
#define UART4_APB_ENABLE_OFFSET 19
#define UART5_APB_ENABLE_OFFSET 20
#define UART6_APB_ENABLE_OFFSET 5
#define UART7_APB_ENABLE_OFFSET 6
#define UART8_APB_ENABLE_OFFSET 7

typedef struct {
    REGADDR_T CONTROL_REG_1;
    REGADDR_T CONTROL_REG_2;
    REGADDR_T CONTROL_REG_3;
    REGADDR_T BAUD_RATE_REG;
    REGADDR_T GUARD_TIME_PRESCALER_REG;
    REGADDR_T RECEIVER_TIMEOUT_REG;
    REGADDR_T RQR_REG;
    REGADDR_T INTERRUPT_STATUS_REG;
    REGADDR_T INTERRUPT_CLEAR_REG;
    REGADDR_T RECEIVE_DATA_REG;
    REGADDR_T TRANSMIT_DATA_REG;
} UART_Typedef;



// Create a struct to hold register configuration for each UART port
// We will create a unique instance of this for each UART port
typedef struct {
    UART_Typedef* p_UART_BASE;
    REGADDR_T* p_NVIC_ENABLE_REG;
    uint8_t nvic_enable_offset;
    REGADDR_T* p_UART_CLOCK_SOURCE_REG;
    uint8_t uart_clock_source_offset;
    REGADDR_T* p_APB_CLOCK_ENABLE_REG;
    uint8_t apb_clock_enable_offset;
    uint32_t baud_rate;
} UartPortConfig_t; 


// Create a struct to hold all configuration for a UART port
// This holds a pointer to the register config struct
// as well as TX/RX pin information and baud rate
typedef struct {
    UartPortConfig_t* config;
    gpio_t* p_tx_pin;
    gpio_t* p_rx_pin;
} UartPort_t;

extern const UartPortConfig_t UART1;
extern const UartPortConfig_t UART2;
extern const UartPortConfig_t UART3;
extern const UartPortConfig_t UART4;
extern const UartPortConfig_t UART5;
extern const UartPortConfig_t UART6;
extern const UartPortConfig_t UART7;
extern const UartPortConfig_t UART8;

extern const UartPort_t UART_PORT_TXPA9_RXPA10;



#endif //UART_DEFS_H