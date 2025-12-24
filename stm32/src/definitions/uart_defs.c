#include <definitions/uart_defs.h>

#define UART1_BUFFER_SIZE 64 

RingBuffer_t uart1_rx_rbuf;
RingBuffer_t uart1_tx_rbuf;


// UART Port Configurations
// Includes base addresses, NVIC settings, clock settings, and buffer configs
// Bundles them alongside a buffer for RX and TX, and ring buffer structs.
// TODO: distinguish between mutable and immutable data here.


UartDriver_t UART1 = {
    .p_UART_BASE = (UART_Typedef*)UART1_BASE,
    .p_NVIC_ENABLE_REG = (REGADDR_T*)UART_NVIC_SET_ENABLE_REG,
    .p_UART_CLOCK_SOURCE_REG = (REGADDR_T*)&(RCC->CLOCK_CONFIG_3),
    .p_APB_CLOCK_ENABLE_REG = (REGADDR_T*)&(RCC->PERIPH_APB_2_CLOCK_ENABLE),
    .uart_clock_source_offset = (uint8_t)UART1_CLOCK_SOURCE_OFFSET, 
    .nvic_enable_offset = (uint8_t)NVIC_UART1_OFFSET,
    .apb_clock_enable_offset = (uint8_t)UART1_APB_ENABLE_OFFSET,
    .buffer_size = (uint8_t)UART1_BUFFER_SIZE,
    .rx_ring_buffer = (RingBuffer_t*)&uart1_rx_rbuf,
    .tx_ring_buffer = (RingBuffer_t*)&uart1_tx_rbuf,
    .baud_rate = (uint32_t)115200,
};
UartDriver_t UART2 = {
    .p_UART_BASE = (UART_Typedef*)UART2_BASE,
    .p_NVIC_ENABLE_REG = (REGADDR_T*)UART_NVIC_SET_ENABLE_REG,
    .p_UART_CLOCK_SOURCE_REG = (REGADDR_T*)&(RCC->CLOCK_CONFIG_3),
    .p_APB_CLOCK_ENABLE_REG = (REGADDR_T*)&(RCC->PERIPH_APB_1_CLOCK_ENABLE),
    .uart_clock_source_offset = (uint8_t)UART2_CLOCK_SOURCE_OFFSET, 
    .nvic_enable_offset = (uint8_t)NVIC_UART2_OFFSET,
    .apb_clock_enable_offset = (uint8_t)UART2_APB_ENABLE_OFFSET,
    .buffer_size = (uint8_t)0,
    .rx_ring_buffer = (RingBuffer_t*)0,
    .tx_ring_buffer = (RingBuffer_t*)0,
    .baud_rate = (uint32_t)115200,
};
UartDriver_t UART3 = {
    .p_UART_BASE = (UART_Typedef*)UART3_BASE,
    .p_NVIC_ENABLE_REG = (REGADDR_T*)UART_NVIC_SET_ENABLE_REG,
    .p_UART_CLOCK_SOURCE_REG = (REGADDR_T*)&(RCC->CLOCK_CONFIG_3),
    .p_APB_CLOCK_ENABLE_REG = (REGADDR_T*)&(RCC->PERIPH_APB_1_CLOCK_ENABLE),
    .uart_clock_source_offset = (uint8_t)UART3_CLOCK_SOURCE_OFFSET, 
    .nvic_enable_offset = (uint8_t)NVIC_UART3TO8_OFFSET,
    .apb_clock_enable_offset = (uint8_t)UART3_APB_ENABLE_OFFSET,
    .buffer_size = (uint8_t)0,
    .rx_ring_buffer = (RingBuffer_t*)0,
    .tx_ring_buffer = (RingBuffer_t*)0,
    .baud_rate = (uint32_t)115200,
};

const UartPort_t UART_PORT_TXPA9_RXPA10 = {
    .config = &UART1,
    .p_tx_pin = &PA9,
    .p_rx_pin = &PA10,
};
