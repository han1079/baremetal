#ifndef GPIO_DEFS_H
#define GPIO_DEFS_H
#include <common.h>

#define GPIO_CTRL_BASE 0x48000000UL
typedef struct {
    REGADDR_T PIN_MODE;
    REGADDR_T OUTPUT_TYPE;
    REGADDR_T OUTPUT_SPEED;
    REGADDR_T PULL_UP_DOWN_MODE;
    REGADDR_T INPUT_VALUE;
    REGADDR_T OUTPUT_VALUE;
    REGADDR_T ATOMIC_SET_RESET;
    REGADDR_T LOCK;
    REGADDR_T ALT_FUNC_CONFIG_BIT_0;
    REGADDR_T ALT_FUNC_CONFIG_BIT_1;
    REGADDR_T ATOMIC_RESET;
} GPIO_Typedef;

typedef struct {
    GPIO_Typedef* base;
    uint8_t offset;
} gpio_t;

extern const gpio_t PA1;
extern const gpio_t PA2;
extern const gpio_t PA3;
extern const gpio_t PA4;
extern const gpio_t PA5;
extern const gpio_t PA6;
extern const gpio_t PA7;
extern const gpio_t PA8;
extern const gpio_t PA9;
extern const gpio_t PA10;
extern const gpio_t PA11;
extern const gpio_t PA12;
extern const gpio_t PA13;

#endif //GPIO_DEFS_H