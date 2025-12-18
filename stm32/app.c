#include <peripherals/gpio.h>
#include <configs/setup.h>
#include <peripherals/uart.h>

void setup(void) {
    standard_speed_setup();
    uart_init(UART_PORT_TXPA9_RXPA10);
    set_pin_mode(PA5, PINMODE_OUTPUT);
    set_pin_pull(PA5, PIN_PULL_DOWN);
}

int main(void) {
    setup();
    while(1) {
        delay_ms_interruptible(500); 
        set_pin(PA5, 0);
        delay_ms_interruptible(500);
        set_pin(PA5, 1);
        uart_send_byte(UART_PORT_TXPA9_RXPA10, 'A');
    }
    return 0;
}
