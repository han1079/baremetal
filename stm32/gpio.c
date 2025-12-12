#include "gpio.h"

// Do only a single pin definition here. Only need the LED.
const gpio_t PA5 = {
    .base = GPIO_A,
    .offset = 5U
};

void gpio_initialize_clock(void) {
    uint32_t mask = (0x3F << 17);
    (RCC->PERIPH_AHB_CLOCK_ENABLE) |= mask;
}

void set_pin(gpio_t pin, int mode) {
    if (mode == 1) {
        SET_BIT((pin.base)->ATOMIC_SET_RESET, pin.offset);
    } else if (mode == 0) {
        SET_BIT((pin.base)->ATOMIC_SET_RESET, (pin.offset + 16));
    } else {
        return;
    }
}

void set_pin_mode(gpio_t pin, PinMode mode) {
    set_register(&((pin.base)->PIN_MODE), pin.offset*2, 2, mode);
}

void set_pin_pull(gpio_t pin, PinPullMode mode) {
    set_register(&((pin.base)->PULL_UP_DOWN_MODE), pin.offset*2, 2, mode);
}

int get_pin(gpio_t pin) {
    uint32_t reg_val = GET_BIT((pin.base)->INPUT_VALUE, pin.offset);  
    return reg_val ? 1 : 0;
}
