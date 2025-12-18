#include <peripherals/uart.h>
#include <peripherals/gpio.h>
#include <core/m0_clock.h>

#define GET_OVERSAMPLE_MODE(uart) \
    GET_BIT((uart)->p_UART_BASE->CONTROL_REG_1, 15)

#define WRITE_BYTE_TO_TRANSMIT_REG(uart, data) \
    ((uart)->p_UART_BASE->TRANSMIT_DATA_REG = (REGADDR_T)(data)) // Write to TDR

#define ACK_TRANSMIT_COMPLETE_AND_CLEAR_FLAG(uart) \
    SET_BIT((uart)->p_UART_BASE->INTERRUPT_CLEAR_REG, 6); // Clear TC flag

#define READ_TRANSMIT_REG_EMPTY_FLAG(uart) \
    GET_BIT((uart)->p_UART_BASE->INTERRUPT_STATUS_REG, 7) // Read TXE flag

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
}

void uart_enable_interrupts(UartPort_t uart_port) {
    __uart_enable_nvic_interrupt(uart_port.config);
    __uart_enable_rx_interrupt(uart_port.config);
    __uart_enable_tx_interrupt(uart_port.config);
}

void uart_disable_interrupts(UartPort_t uart_port) {
    __uart_disable_rx_interrupt(uart_port.config);
    __uart_disable_tx_interrupt(uart_port.config);
    __uart_disable_nvic_interrupt(uart_port.config);
}

void uart_deinit(UartPort_t uart_port) {
    uart_disable_interrupts(uart_port);
    __uart_disable_base(uart_port.config);
    __uart_disable_receiver(uart_port.config);
    __uart_disable_transmitter(uart_port.config);
    __uart_disable_clock(uart_port.config);
}

void uart_send_byte(UartPort_t uart_port, uint8_t data) {
    while (!(READ_TRANSMIT_REG_EMPTY_FLAG(uart_port.config)));
    WRITE_BYTE_TO_TRANSMIT_REG(uart_port.config, data);
}

uint32_t __uart_get_clock_frequency(UartPortConfig_t* uart) {
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

void __uart_configure_baud_rate(UartPortConfig_t* uart, uint32_t baud) {
    uint32_t uart_clock_frequency = __uart_get_clock_frequency(uart);
    uint8_t oversample_by_8 = GET_OVERSAMPLE_MODE(uart);

    ASSERT(oversample_by_8 == 0); // Only oversample by 16 supported for now
    
    uint32_t uartdiv = uart_clock_frequency / baud; // Rounded division
    SET_WORD(uart->p_UART_BASE->BAUD_RATE_REG, uartdiv);
}