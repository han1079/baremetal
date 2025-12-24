#include "infrastructure/comms_protocol.h"
#include <definitions/uart_defs.h>


RingBuffer_t uart1_rx_rbuf = {
    .size = BUFFER_LEN_MAX,
    .head = 0,
    .tail = 0,
    .count = 0,
};

RingBuffer_t uart1_tx_rbuf = {
    .size = BUFFER_LEN_MAX,
    .head = 0,
    .tail = 0,
    .count = 0,
};


// UART Port Configurations
// Includes base addresses, NVIC settings, clock settings, and buffer configs
// Bundles them alongside a buffer for RX and TX, and ring buffer structs.
// TODO: distinguish between mutable and immutable data here.

UartHwConfig_t UART1_HW = {
    .BASE = (UART_Typedef*)UART1_BASE,
    .NVIC_INTERRUPT_ENABLE = (REGADDR_T*)UART_NVIC_SET_ENABLE_REG,
    .CLOCK_SRC = (REGADDR_T*)&(RCC->CLOCK_CONFIG_3),
    .APB_ENABLE = (REGADDR_T*)&(RCC->PERIPH_APB_2_CLOCK_ENABLE),
    .clock_src_offset = (uint8_t)UART1_CLOCK_SOURCE_OFFSET, 
    .nvic_iser_offset = (uint8_t)NVIC_UART1_OFFSET,
    .apb_offset = (uint8_t)UART1_APB_ENABLE_OFFSET,
};

UartHwConfig_t UART2_HW = {
    .BASE = (UART_Typedef*)UART2_BASE,
    .NVIC_INTERRUPT_ENABLE = (REGADDR_T*)UART_NVIC_SET_ENABLE_REG,
    .CLOCK_SRC = (REGADDR_T*)&(RCC->CLOCK_CONFIG_3),
    .APB_ENABLE = (REGADDR_T*)&(RCC->PERIPH_APB_1_CLOCK_ENABLE),
    .clock_src_offset = (uint8_t)UART2_CLOCK_SOURCE_OFFSET, 
    .nvic_iser_offset = (uint8_t)NVIC_UART2_OFFSET,
    .apb_offset = (uint8_t)UART2_APB_ENABLE_OFFSET,
};

UartBuffer_t UART1_BUFFER = {
    .rx_ring_buffer = &uart1_rx_rbuf,
    .tx_ring_buffer = &uart1_tx_rbuf,
    .rx_dropped = 0,
    .tx_dropped = 0,
    .rx_max_used = 0,
    .tx_max_used = 0,
};

UartBuffer_t UART2_BUFFER = {
    .rx_ring_buffer = (RingBuffer_t*)0,
    .tx_ring_buffer = (RingBuffer_t*)0,
    .rx_dropped = 0,
    .tx_dropped = 0,
    .rx_max_used = 0,
    .tx_max_used = 0,
};

UartDriver_t UART1 = {
    .reg = (UartHwConfig_t*)&UART1_HW,
    .buffer = (UartBuffer_t*)&UART1_BUFFER,
    .baud_rate = 115200,
};

UartDriver_t UART2 = {
    .reg = (UartHwConfig_t*)&UART2_HW,
    .buffer = (UartBuffer_t*)&UART2_BUFFER,
    .baud_rate = 115200,
};

const UartPort_t UART_PORT_TXPA9_RXPA10 = {
    .config = &UART1,
    .p_tx_pin = &PA9,
    .p_rx_pin = &PA10,
};
