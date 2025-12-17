#include <peripherals/gpio.h>
#include <configs/setup.h>

void setup(void) {
    high_speed_setup();
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
    }
    return 0;
}
