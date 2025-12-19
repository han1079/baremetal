#ifndef UART_H
#define UART_H

#include <core/common.h>
#include <definitions/uart_defs.h>
#include <infra/ring_buffer.h>

// API 
void uart_init(UartPort_t uart_port);
void uart_enable_interrupts(UartPort_t uart_port);
void uart_disable_interrupts(UartPort_t uart_port);
void uart_deinit(UartPort_t uart_port);


void uart_send_byte_blocking(UartPort_t uart_port, uint8_t data);
uint8_t uart_receive_byte_blocking(UartPort_t uart_port);

void uart_write_byte_array(UartPort_t uart_port, uint8_t* data, uint32_t length);
bool uart_drain_rx_buffer_until_delimiter(UartPort_t uart_port, uint8_t delimiter, uint8_t* dest, uint8_t dest_length);
bool uart_get_rx_buffer_next_byte(UartPort_t uart_port, uint8_t* dest);
uint8_t uart_drain_rx_buffer_n_bytes(UartPort_t uart_port, uint8_t* dest, uint8_t dest_length, uint8_t num_bytes);

// Helper functions
void __uart_configure_gpio_pins(UartPort_t uart_port);
void __uart_configure_baud_rate(UartPortConfig_t* uart, uint32_t baud);
uint32_t __uart_get_clock_frequency(UartPortConfig_t* uart);

static inline void __uart_set_clock_source(UartPortConfig_t* uart, uint8_t source) {
    set_register(uart->p_UART_CLOCK_SOURCE_REG, uart->uart_clock_source_offset, 2, source);
}
static inline uint8_t __uart_get_clock_source(UartPortConfig_t* uart) {
    return (uint8_t)((*(uart->p_UART_CLOCK_SOURCE_REG) >> uart->uart_clock_source_offset) & 0b11UL);
}


static inline void __uart_enable_clock(UartPortConfig_t* uart) {
    SET_BIT(*(uart->p_APB_CLOCK_ENABLE_REG), uart->apb_clock_enable_offset);
}
static inline void __uart_disable_clock(UartPortConfig_t* uart) {
    RESET_BIT(*(uart->p_APB_CLOCK_ENABLE_REG), uart->apb_clock_enable_offset);
}
static inline void __uart_enable_nvic_interrupt(UartPortConfig_t* uart) {
    SET_BIT(*(uart->p_NVIC_ENABLE_REG), uart->nvic_enable_offset);
}
static inline void __uart_disable_nvic_interrupt(UartPortConfig_t* uart) {
    RESET_BIT(*(uart->p_NVIC_ENABLE_REG), uart->nvic_enable_offset);
}
static inline void __uart_enable_transmitter(UartPortConfig_t* uart) {
    SET_BIT(uart->p_UART_BASE->CONTROL_REG_1, 3);
}
static inline void __uart_enable_tx_interrupt(UartPortConfig_t* uart) {
    SET_BIT(uart->p_UART_BASE->CONTROL_REG_1, 7);
}
static inline void __uart_enable_receiver(UartPortConfig_t* uart) {
    SET_BIT(uart->p_UART_BASE->CONTROL_REG_1, 2);
}
static inline void __uart_enable_rx_interrupt(UartPortConfig_t* uart) {
    SET_BIT(uart->p_UART_BASE->CONTROL_REG_1, 5);
}
static inline void __uart_enable_base(UartPortConfig_t* uart) {
    SET_BIT(uart->p_UART_BASE->CONTROL_REG_1, 0);
}
static inline void __uart_disable_transmitter(UartPortConfig_t* uart) {
    RESET_BIT(uart->p_UART_BASE->CONTROL_REG_1, 3);
}
static inline void __uart_disable_tx_interrupt(UartPortConfig_t* uart) {
    RESET_BIT(uart->p_UART_BASE->CONTROL_REG_1, 7);
}
static inline void __uart_disable_receiver(UartPortConfig_t* uart) {
    RESET_BIT(uart->p_UART_BASE->CONTROL_REG_1, 2);
}
static inline void __uart_disable_rx_interrupt(UartPortConfig_t* uart) {
    RESET_BIT(uart->p_UART_BASE->CONTROL_REG_1, 5);
}
static inline void __uart_disable_base(UartPortConfig_t* uart) {
    RESET_BIT(uart->p_UART_BASE->CONTROL_REG_1, 0);
}

// Helpers to check status flags
static inline uint8_t __get_uart_word_size(UartPortConfig_t* uart){
    ASSERT(GET_BIT((uart)->p_UART_BASE->CONTROL_REG_1, 28) == 0); // Word length not allowed to be 7
    return (GET_BIT((uart)->p_UART_BASE->CONTROL_REG_1, 12)) ? 9 : 8;
}

static inline uint8_t __get_uart_stop_bits(UartPortConfig_t* uart){
    return (uint8_t)((((uart)->p_UART_BASE->CONTROL_REG_2 >> 12) & 0b11UL));
}

static inline uint8_t __get_uart_nvic_interrupt_enable_status(UartPortConfig_t* uart){
    return (uint8_t)(GET_BIT(*(uart->p_NVIC_ENABLE_REG), uart->nvic_enable_offset) & 0b1UL);
}

static inline uint8_t __get_uart_tx_interrupt_enable_status(UartPortConfig_t* uart){
    return (uint8_t)(GET_BIT((uart)->p_UART_BASE->CONTROL_REG_1, 7) & 0b1UL);
}

static inline uint8_t __get_uart_rx_interrupt_enable_status(UartPortConfig_t* uart){
    return (uint8_t)(GET_BIT((uart)->p_UART_BASE->CONTROL_REG_1, 5) & 0b1UL);
}



#endif //UART_H
