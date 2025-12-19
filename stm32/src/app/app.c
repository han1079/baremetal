#include <drivers/gpio.h>
#include <configs/setup.h>
#include <drivers/uart.h>

void setup(void) {
    standard_speed_setup();
    uart_init(UART_PORT_TXPA9_RXPA10);
    set_pin_mode(PA5, PINMODE_OUTPUT);
    set_pin_pull(PA5, PIN_PULL_DOWN);
}

void poll_and_blink(void) {
    uint8_t received_byte;
    received_byte = uart_receive_byte_blocking(UART_PORT_TXPA9_RXPA10); // Poll to clear any existing data
    if (received_byte) { // If RX pin is high
        uart_send_byte_blocking(UART_PORT_TXPA9_RXPA10, 'r'); // Echo back received byte
        uart_send_byte_blocking(UART_PORT_TXPA9_RXPA10, 'x'); // Echo back received byte
        uart_send_byte_blocking(UART_PORT_TXPA9_RXPA10, ':'); // Echo back received byte
        uart_send_byte_blocking(UART_PORT_TXPA9_RXPA10, received_byte); // Echo back received byte
        uart_send_byte_blocking(UART_PORT_TXPA9_RXPA10, '\r'); // Echo back received byte
        uart_send_byte_blocking(UART_PORT_TXPA9_RXPA10, '\n'); // Echo back received byte
    }
}

int main(void) {
    setup();
    while(1) {
        poll_and_blink();
    }
    return 0;
}
