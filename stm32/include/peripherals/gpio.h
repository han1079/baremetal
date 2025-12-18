#ifndef GPIO_H
#define GPIO_H
#include <core/common.h>
#include <definitions/gpio_defs.h>

void gpio_configure_alt_function(gpio_t pin, AFKey af, PinSpeed speed, PinPullMode pull);

void gpio_initialize_clock(void);
void set_pin(gpio_t pin, int mode);
void set_pin_mode(gpio_t pin, PinMode mode);
void set_pin_pull(gpio_t pin, PinPullMode mode);
void set_pin_speed(gpio_t pin, PinSpeed speed);
int get_pin(gpio_t pin);


#endif //GPIO_H
