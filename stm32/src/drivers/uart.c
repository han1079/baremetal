#include <drivers/uart.h>
#include <drivers/gpio.h>
#include <core/clock.h>
#include <core/interrupts.h>

void __uart_configure_gpio_pins(UartPort_t uart_port){
    gpio_configure_alt_function(*(uart_port.p_rx_pin), \
                                (uart_port.p_rx_pin)->af->UART, \
                                PINSPEED_HIGH, \
                                PIN_PULL_UP);
    gpio_configure_alt_function(*(uart_port.p_tx_pin), \
                                (uart_port.p_tx_pin)->af->UART, \
                                PINSPEED_HIGH, \
                                PIN_PULL_FLOATING);
    set_pin_mode(*(uart_port.p_rx_pin), PINMODE_AF);
    set_pin_mode(*(uart_port.p_tx_pin), PINMODE_AF); 
}

void uart_init(UartPort_t uart_port) {

    __uart_enable_clock(uart_port.config);
    __uart_configure_gpio_pins(uart_port);
    __uart_configure_baud_rate(uart_port.config, uart_port.config->baud_rate);
    __uart_enable_transmitter(uart_port.config);
    __uart_enable_receiver(uart_port.config);
    __uart_enable_base(uart_port.config);
    __uart_enable_rx_interrupt(uart_port.config);
    uart_allow_interrupts(uart_port);
}

void uart_allow_interrupts(UartPort_t uart_port) {
    __uart_enable_nvic_interrupt(uart_port.config);
}

void uart_disallow_interrupts(UartPort_t uart_port) {
    __uart_disable_nvic_interrupt(uart_port.config);
}

void uart_deinit(UartPort_t uart_port) {
    uart_disallow_interrupts(uart_port);
    __uart_disable_rx_interrupt(uart_port.config);
    __uart_disable_base(uart_port.config);
    __uart_disable_receiver(uart_port.config);
    __uart_disable_transmitter(uart_port.config);
    __uart_disable_clock(uart_port.config);
}


void USART1_GLOBAL_Handler() {
    // Check RXNE Flag
    if (__get_uart_rxne(&UART1)) {
        uint8_t received_byte;
        __uart_read_byte_from_rdr(&UART1, &received_byte);
        push_to_ring_buffer(UART1.buffer->rx_ring_buffer, received_byte);
    }

    // Check TXE Flag
    if (__get_uart_txe(&UART1)) {
        uint8_t byte_to_tx;
        if (pop_from_ring_buffer(UART1.buffer->tx_ring_buffer, &byte_to_tx)) {
            __uart_write_byte_to_tdr(&UART1, byte_to_tx);
        } else {
            // No more data to send, disable TXE interrupt
            __uart_disable_tx_interrupt(&UART1);
        }
    }

    if (__get_uart_overrun_flag(&UART1)) {
        __uart_clear_overrun_flag(&UART1);
    }
}

void uart_transact_start(UartDriver_t* uart) {
    uint32_t key = lock_interrupts_and_save();
    uart->tx_transactions_in_progress++;
    unlock_interrupts_and_restore(key);
}

uint8_t uart_write(UartDriver_t* uart, ByteSpan_t p_data) {
    uart_transact_start(uart);
    uart_stage_bytes_for_tx(uart, p_data);
    uart_transact_end(uart);
}

void uart_transact_end(UartDriver_t* uart) {
    uint32_t key = lock_interrupts_and_save();
    if (uart->tx_transactions_in_progress > 0) { uart->tx_transactions_in_progress--; }

    if (uart->tx_transactions_in_progress == 0) { __uart_kick_off_tx(uart); } 
    unlock_interrupts_and_restore(key);
}

void uart_stage_bytes_for_tx(UartDriver_t* uart, ByteSpan_t p_data) {
    uint32_t key = lock_interrupts_and_save();
    uint8_t* data = p_data.bytes;
    uint8_t length = p_data.count;
    if (!(__get_uart_txe(uart))) { 
        unlock_interrupts_and_restore(key);
        return; 
    } // UART not ready

    for (int i = 0; i < length; i++) {
        if(!push_to_ring_buffer(uart->buffer->tx_ring_buffer, data[i])) {
            uart->buffer->tx_dropped += (length - i);
            break;
        }
    }
    unlock_interrupts_and_restore(key);
}

bool uart_get_rx_buffer_next_byte(void* driver, uint8_t* dest) {
    uint32_t key = lock_interrupts_and_save();
    UartDriver_t* uart = (UartDriver_t*)driver;
    uint8_t byte;

    // Only grab from ring buffer if there's something there.
    if (pop_from_ring_buffer(uart->buffer->rx_ring_buffer, &byte)) {
        (*dest) = byte;
        unlock_interrupts_and_restore(key);
        return true;
    } else {
        unlock_interrupts_and_restore(key);
        return false;
    }
}

uint8_t get_uart_rx_buffer_count(UartDriver_t *uart) {
    return uart->buffer->rx_ring_buffer->count;
}

uint8_t get_uart_tx_buffer_count(UartDriver_t* uart) {
    return uart->buffer->tx_ring_buffer->count;
}

uint32_t __uart_get_clock_frequency(UartDriver_t* uart) {
    uint8_t clock_source = __uart_get_clock_source(uart);
    switch (clock_source) {
        case 0b00: // PCLK
            uint32_t apb_prescale = get_apb1_prescaler_divider();
            uint32_t ahb_prescale = get_ahb_prescaler_divider();
            ASSERT(ahb_prescale == 1);
            ASSERT(apb_prescale == 1);
            uint32_t pclk_freq = get_system_core_clock() / ahb_prescale / apb_prescale;
            return pclk_freq;
        case 0b01: // SYSCLK
            return get_system_core_clock();
        case 0b10: // HSI
            return 8000000UL;
        case 0b11: // LSE
            ASSERT(0); // Not supported
            return 0;
        default:
            ASSERT(0); // Should not happen
            return 0;
    }
}

void __uart_configure_baud_rate(UartDriver_t* uart, uint32_t baud) {
    uint32_t uart_clock_frequency = __uart_get_clock_frequency(uart);
    uint8_t oversample_by_8 = __get_uart_oversample_mode(uart);

    ASSERT(oversample_by_8 == 0); // Only oversample by 16 supported for now
    
    uint32_t uartdiv = uart_clock_frequency / baud; // Rounded division
    SET_WORD(uart->reg->BASE->BAUD_RATE_REG, uartdiv);
}
