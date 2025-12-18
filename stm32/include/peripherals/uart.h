#ifndef UART_H
#define UART_H

#include <core/common.h>
#include <definitions/uart_defs.h>

// API 
void uart_init(UartPort_t* uart_port);
void uart_deinit(UartPort_t* uart_port);
void uart_send_byte(UartPort_t* uart_port, uint8_t data);
uint8_t uart_receive_byte(UartPort_t* uart_port);
void uart_send_data(UartPort_t* uart_port, uint8_t* data, uint32_t length);
void uart_receive_data(UartPort_t* uart_port, uint8_t* buffer, uint32_t length);

// Helper functions
void __uart_configure_gpio_pins(UartPort_t* uart_port);

static inline void __uart_configure_baud_rate(UartPortConfig_t* uart, uint32_t baud) {
    SET_WORD(uart->p_UART_BASE->BAUD_RATE_REG, baud); 
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


#endif //UART_H