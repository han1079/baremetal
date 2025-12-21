#include <definitions/gpio_defs.h>

gpio_af_lut_t GPIO_AF_DEFAULT =    {.TIMER = NO_AF, .UART = NO_AF, .I2C = NO_AF,   .SPI = NO_AF, .CAN = NO_AF};
gpio_af_lut_t GPIO_AF_PA9 =        {.TIMER = AF2,   .UART = AF1,   .I2C = AF4,     .SPI = NO_AF, .CAN = NO_AF};
#define GPIO_AF_PA10 GPIO_AF_PA9

gpio_t PA1 = {.base = GPIO_A, .offset = 1U , .af = &GPIO_AF_DEFAULT};
gpio_t PA2 = {.base = GPIO_A, .offset = 2U , .af = &GPIO_AF_DEFAULT};
gpio_t PA3 = {.base = GPIO_A, .offset = 3U , .af = &GPIO_AF_DEFAULT};
gpio_t PA4 = {.base = GPIO_A, .offset = 4U , .af = &GPIO_AF_DEFAULT};
gpio_t PA5 = {.base = GPIO_A, .offset = 5U , .af = &GPIO_AF_DEFAULT};
gpio_t PA6 = {.base = GPIO_A, .offset = 6U , .af = &GPIO_AF_DEFAULT};
gpio_t PA7 = {.base = GPIO_A, .offset = 7U , .af = &GPIO_AF_DEFAULT};
gpio_t PA8 = {.base = GPIO_A, .offset = 8U , .af = &GPIO_AF_DEFAULT};
gpio_t PA9 = {.base = GPIO_A, .offset = 9U , .af = &GPIO_AF_PA9};
gpio_t PA10 = {.base = GPIO_A, .offset = 10U, .af = &GPIO_AF_PA10};
gpio_t PA11 = {.base = GPIO_A, .offset = 11U, .af = &GPIO_AF_DEFAULT};
gpio_t PA12 = {.base = GPIO_A, .offset = 12U, .af = &GPIO_AF_DEFAULT};
gpio_t PA13 = {.base = GPIO_A, .offset = 13U, .af = &GPIO_AF_DEFAULT};
