#ifndef UART_H
#define UART_H

#include <core/common.h>
#include <core/interrupts.h>
#include <definitions/uart_defs.h>
#include <infrastructure/bytes.h>

// API 
void uart_init(UartPort_t uart_port);
void uart_allow_interrupts(UartPort_t uart_port);
void uart_disallow_interrupts(UartPort_t uart_port);
void uart_deinit(UartPort_t uart_port);

void uart_transact_start(UartDriver_t* uart);
uint8_t uart_write(UartDriver_t* uart, ByteSpan_t p_data);
void uart_transact_end(UartDriver_t* uart);

void uart_stage_bytes_for_tx(UartDriver_t* uart, ByteSpan_t p_data);
bool uart_get_rx_buffer_next_byte(void* driver, uint8_t* dest);

// Buffer Helper functions
uint8_t get_uart_rx_buffer_count(UartDriver_t* uart);
uint8_t get_uart_tx_buffer_count(UartDriver_t* uart);

// Config / Init Helper functions
void __uart_configure_gpio_pins(UartPort_t uart_port);
void __uart_configure_baud_rate(UartDriver_t* uart, uint32_t baud);
uint32_t __uart_get_clock_frequency(UartDriver_t* uart);

static inline void __uart_set_clock_source(UartDriver_t* uart, uint8_t source) {
    set_register(uart->reg->CLOCK_SRC, uart->reg->clock_src_offset, 2, source);
}
static inline uint8_t __uart_get_clock_source(UartDriver_t* uart) {
    return (uint8_t)((*(uart->reg->CLOCK_SRC) >> uart->reg->clock_src_offset) & 0b11UL);
}

static inline void __uart_enable_clock(UartDriver_t* uart) {
    SET_BIT(*(uart->reg->APB_ENABLE), uart->reg->apb_offset);
}
static inline void __uart_disable_clock(UartDriver_t* uart) {
    RESET_BIT(*(uart->reg->APB_ENABLE), uart->reg->apb_offset);
}
static inline void __uart_enable_nvic_interrupt(UartDriver_t* uart) {
    SET_BIT(*(uart->reg->NVIC_INTERRUPT_ENABLE), uart->reg->nvic_iser_offset);
}
static inline void __uart_disable_nvic_interrupt(UartDriver_t* uart) {
    RESET_BIT(*(uart->reg->NVIC_INTERRUPT_ENABLE), uart->reg->nvic_iser_offset);
}
static inline void __uart_enable_transmitter(UartDriver_t* uart) {
    SET_BIT(uart->reg->BASE->CONTROL_REG_1, 3);
}
static inline void __uart_enable_tx_interrupt(UartDriver_t* uart) {
    SET_BIT(uart->reg->BASE->CONTROL_REG_1, 7);
}
static inline void __uart_enable_receiver(UartDriver_t* uart) {
    SET_BIT(uart->reg->BASE->CONTROL_REG_1, 2);
}
static inline void __uart_enable_rx_interrupt(UartDriver_t* uart) {
    SET_BIT(uart->reg->BASE->CONTROL_REG_1, 5);
}
static inline void __uart_enable_base(UartDriver_t* uart) {
    SET_BIT(uart->reg->BASE->CONTROL_REG_1, 0);
}
static inline void __uart_disable_transmitter(UartDriver_t* uart) {
    RESET_BIT(uart->reg->BASE->CONTROL_REG_1, 3);
}
static inline void __uart_disable_tx_interrupt(UartDriver_t* uart) {
    RESET_BIT(uart->reg->BASE->CONTROL_REG_1, 7);
}
static inline void __uart_disable_receiver(UartDriver_t* uart) {
    RESET_BIT(uart->reg->BASE->CONTROL_REG_1, 2);
}
static inline void __uart_disable_rx_interrupt(UartDriver_t* uart) {
    RESET_BIT(uart->reg->BASE->CONTROL_REG_1, 5);
}
static inline void __uart_disable_base(UartDriver_t* uart) {
    RESET_BIT(uart->reg->BASE->CONTROL_REG_1, 0);
}
static inline void __uart_write_byte_to_tdr(UartDriver_t* uart, uint8_t data) {
    uart->reg->BASE->TRANSMIT_DATA_REG = (REGADDR_T)(data);
}
static inline void __uart_read_byte_from_rdr(UartDriver_t* uart, uint8_t* dest) {
    (*dest) = (uint8_t)(uart->reg->BASE->RECEIVE_DATA_REG);
}
static inline void __uart_clear_overrun_flag(UartDriver_t* uart) {
    SET_BIT(uart->reg->BASE->INTERRUPT_CLEAR_REG, 3);
}

// Helpers to check status flags
static inline uint8_t __get_uart_word_size(UartDriver_t* uart){
    ASSERT(GET_BIT((uart)->reg->BASE->CONTROL_REG_1, 28) == 0); // Word length not allowed to be 7
    return (GET_BIT((uart->reg->BASE->CONTROL_REG_1), 12)) ? 9 : 8;
}

static inline uint8_t __get_uart_stop_bits(UartDriver_t* uart){
    return (uint8_t)((((uart->reg->BASE->CONTROL_REG_2) >> 12) & 0b11UL));
}

static inline bool __get_uart_nvic_iser(UartDriver_t* uart){
    return (bool)GET_BIT(*(uart->reg->NVIC_INTERRUPT_ENABLE), uart->reg->nvic_iser_offset);
}

static inline bool __get_uart_txe_enabled(UartDriver_t* uart){
    return (bool)GET_BIT((uart->reg->BASE->CONTROL_REG_1), 7);
}

static inline bool __get_uart_rxne_enabled(UartDriver_t* uart){
    return (bool)GET_BIT((uart->reg->BASE->CONTROL_REG_1), 5);
}

static inline bool __get_uart_txe(UartDriver_t* uart){
    return (bool)GET_BIT((uart->reg->BASE->INTERRUPT_STATUS_REG), 7);
}

static inline bool __get_uart_rxne(UartDriver_t* uart){
    return (bool)GET_BIT((uart->reg->BASE->INTERRUPT_STATUS_REG), 5);
}

static inline bool __get_uart_oversample_mode(UartDriver_t* uart){
    return (bool)GET_BIT((uart->reg->BASE->CONTROL_REG_1), 15);
}

static inline bool __get_uart_overrun_flag(UartDriver_t* uart){
    return (bool)GET_BIT((uart->reg->BASE->INTERRUPT_STATUS_REG), 3);
}

static inline void __uart_push_byte_to_tx_rb(void* driver, uint8_t byte) {
    uint32_t key = lock_interrupts_and_save();
    UartDriver_t* uart = (UartDriver_t*)driver;
    push_to_ring_buffer(uart->buffer->tx_ring_buffer, byte);
    unlock_interrupts_and_restore(key);
}

static inline void __uart_kick_off_tx(UartDriver_t* uart) {
    while(!(__get_uart_txe(uart)));

    uint8_t tmp;
    pop_from_ring_buffer(uart->buffer->tx_ring_buffer, &tmp);
    __uart_write_byte_to_tdr(uart, tmp);

    // By here, we KNOW that interrupts are enabled globally, and TXE shows the shift register is empty
    // We enable TXE interrupts to kick off the first send.
    __uart_enable_tx_interrupt(uart);
}

#endif //UART_H
