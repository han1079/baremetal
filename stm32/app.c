#include <peripherals/gpio.h>

void delay(int cycles) {
    int i = cycles;
    while(i > 0){
        i--;   
    }
}

int main(void) {
    gpio_initialize_clock();
    set_pin_mode(PA5, PINMODE_OUTPUT);
    set_pin_pull(PA5, PIN_PULL_DOWN);
    set_pin(PA5, 1);
    while(1) {
        delay(60000);
        set_pin(PA5, 0);
        delay(60000);
        set_pin(PA5, 1);
    }
    return 0;
}
