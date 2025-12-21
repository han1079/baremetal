#include <definitions/uart_defs.h>

#define UART1_BUFFER_SIZE 64 
static uint8_t uart1_rx_rbuf_storage[UART1_BUFFER_SIZE];
static uint8_t uart1_tx_rbuf_storage[UART1_BUFFER_SIZE];
static RingBuffer_t uart1_rx_rbuf;
static RingBuffer_t uart1_tx_rbuf;

// UART Port Configurations
// Includes base addresses, NVIC settings, clock settings, and buffer configs
// Bundles them alongside a buffer for RX and TX, and ring buffer structs.
// TODO: distinguish between mutable and immutable data here.

UartDriver_t UART1 = {
    .p_UART_BASE = (UART_Typedef*)UART1_BASE,
    .p_NVIC_ENABLE_REG = (REGADDR_T*)UART_NVIC_SET_ENABLE_REG,
    .nvic_enable_offset = NVIC_UART1_OFFSET,
    .p_UART_CLOCK_SOURCE_REG = (REGADDR_T*)&(RCC->CLOCK_CONFIG_3),
    .uart_clock_source_offset = UART1_CLOCK_SOURCE_OFFSET, 
    .p_APB_CLOCK_ENABLE_REG = (REGADDR_T*)&(RCC->PERIPH_APB_2_CLOCK_ENABLE),
    .apb_clock_enable_offset = UART1_APB_ENABLE_OFFSET,
    .baud_rate = 115200,
    .rx_buffer_storage = uart1_rx_rbuf_storage,
    .tx_buffer_storage = uart1_tx_rbuf_storage,
    .rx_ring_buffer = &uart1_rx_rbuf,
    .tx_ring_buffer = &uart1_tx_rbuf,
    .buffer_size = UART1_BUFFER_SIZE,
};
UartDriver_t UART2 = {
    .p_UART_BASE = (UART_Typedef*)UART2_BASE,
    .p_NVIC_ENABLE_REG = (REGADDR_T*)UART_NVIC_SET_ENABLE_REG,
    .nvic_enable_offset = NVIC_UART2_OFFSET,
    .p_UART_CLOCK_SOURCE_REG = (REGADDR_T*)&(RCC->CLOCK_CONFIG_3),
    .uart_clock_source_offset = UART2_CLOCK_SOURCE_OFFSET, 
    .p_APB_CLOCK_ENABLE_REG = (REGADDR_T*)&(RCC->PERIPH_APB_1_CLOCK_ENABLE),
    .apb_clock_enable_offset = UART2_APB_ENABLE_OFFSET,
    .baud_rate = 115200,
    .rx_buffer_storage = 0, // Not implementing for UART2+
    .tx_buffer_storage = 0,
    .rx_ring_buffer = 0,
    .tx_ring_buffer = 0,
    .buffer_size = 0,
};
UartDriver_t UART3 = {
    .p_UART_BASE = (UART_Typedef*)UART3_BASE,
    .p_NVIC_ENABLE_REG = (REGADDR_T*)UART_NVIC_SET_ENABLE_REG,
    .nvic_enable_offset = NVIC_UART3TO8_OFFSET,
    .p_UART_CLOCK_SOURCE_REG = (REGADDR_T*)&(RCC->CLOCK_CONFIG_3),
    .uart_clock_source_offset = UART3_CLOCK_SOURCE_OFFSET, 
    .p_APB_CLOCK_ENABLE_REG = (REGADDR_T*)&(RCC->PERIPH_APB_1_CLOCK_ENABLE),
    .apb_clock_enable_offset = UART3_APB_ENABLE_OFFSET,
    .baud_rate = 115200,
    .rx_buffer_storage = 0, // Not implementing for UART2+
    .tx_buffer_storage = 0,
    .rx_ring_buffer = 0,
    .tx_ring_buffer = 0,
    .buffer_size = 0,
};

const UartPort_t UART_PORT_TXPA9_RXPA10 = {
    .config = &UART1,
    .p_tx_pin = &PA9,
    .p_rx_pin = &PA10,
};
