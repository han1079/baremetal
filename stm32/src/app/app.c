#include <drivers/gpio.h>
#include <configs/core_setup.h>
#include <configs/system_setup.h>
#include <services/uart_console.h>
#include <drivers/uart.h>
#include <core/dispatcher.h>

void setup(void) {
    high_speed_setup();
    uart_init(UART_PORT_TXPA9_RXPA10);
    set_pin_mode(PA5, PINMODE_OUTPUT);
    set_pin_pull(PA5, PIN_PULL_DOWN);
    system_setup();
}


int main(void) {
    setup();
    while(1) {
        dispatch_uart(&uart1_dispatcher); 
        delay_ms_interruptible(10);
        set_pin(PA5, 1);
        console_on_update();
        delay_ms_interruptible(5);
        set_pin(PA5, 0);
    }
    return 0;
}
