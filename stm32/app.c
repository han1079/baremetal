#include <peripherals/gpio.h>
#include <configs/setup.h>
#include <peripherals/uart.h>

void setup(void) {
    standard_speed_setup();
    uart_init(UART_PORT_TXPA9_RXPA10);
    set_pin_mode(PA5, PINMODE_OUTPUT);
    set_pin_pull(PA5, PIN_PULL_DOWN);
}

void Rapid_Blink(uint32_t count) {
    while (count > 0) {
        set_pin(PA5, 1);
        delay_ms_interruptible(50);
        set_pin(PA5, 0);
        delay_ms_interruptible(50);
        count--;
    }
}

void LED_Loop(uint32_t delay_ms, void (*inserted_function)(void)) {
    while (1) {
        set_pin(PA5, 1);
        delay_ms_interruptible(delay_ms);
        inserted_function();
        set_pin(PA5, 0);
        delay_ms_interruptible(delay_ms);
        inserted_function();
    }
}

void poll_and_blink(void) {
    uint8_t received_byte;
    received_byte = uart_receive_byte(UART_PORT_TXPA9_RXPA10); // Poll to clear any existing data
    if (received_byte) { // If RX pin is high
        uart_send_byte(UART_PORT_TXPA9_RXPA10, 'r'); // Echo back received byte
        uart_send_byte(UART_PORT_TXPA9_RXPA10, 'x'); // Echo back received byte
        uart_send_byte(UART_PORT_TXPA9_RXPA10, ':'); // Echo back received byte
        uart_send_byte(UART_PORT_TXPA9_RXPA10, received_byte); // Echo back received byte
        uart_send_byte(UART_PORT_TXPA9_RXPA10, '\r'); // Echo back received byte
        uart_send_byte(UART_PORT_TXPA9_RXPA10, '\n'); // Echo back received byte
        Rapid_Blink(5);
    }
}

int main(void) {
    setup();
    while(1) {
        poll_and_blink();
    }
    return 0;
}
