#ifndef GPIO_H
#define GPIO_H
#include <common.h>
#include <gpio_defs.h>

#define GPIO_A_OFFSET 0x00000000UL
#define GPIO_B_OFFSET 0x00000400UL
#define GPIO_C_OFFSET 0x00000800UL
#define GPIO_D_OFFSET 0x00000C00UL
#define GPIO_F_OFFSET 0x00001400UL

#define GPIO_A_BASE GPIO_CTRL_BASE + GPIO_A_OFFSET
#define GPIO_B_BASE GPIO_CTRL_BASE + GPIO_B_OFFSET
#define GPIO_C_BASE GPIO_CTRL_BASE + GPIO_C_OFFSET
#define GPIO_D_BASE GPIO_CTRL_BASE + GPIO_D_OFFSET
#define GPIO_F_BASE GPIO_CTRL_BASE + GPIO_F_OFFSET

#define GPIO_A ((GPIO_Typedef*) GPIO_A_BASE)
#define GPIO_B ((GPIO_Typedef*) GPIO_B_BASE)
#define GPIO_C ((GPIO_Typedef*) GPIO_C_BASE)
#define GPIO_D ((GPIO_Typedef*) GPIO_D_BASE)
#define GPIO_F ((GPIO_Typedef*) GPIO_F_BASE)

typedef enum {
    PINMODE_INPUT = 0b00,
    PINMODE_OUTPUT = 0b01,
    PINMODE_AF = 0b10,
    PINMODE_ANALOG = 0b11,
} PinMode;

typedef enum PinSpeed {
    PINSPEED_LOW = 0b00,
    PINSPEED_MED = 0b01,
    PINSPEED_HIGH = 0b11,
} PinSpeed;

typedef enum PinPullMode {
    PIN_PULL_FLOATING = 0b00,
    PIN_PULL_UP = 0b01,
    PIN_PULL_DOWN = 0b10,
} PinPullMode;

void gpio_initialize_clock(void);
void set_pin(gpio_t pin, int mode);
void set_pin_mode(gpio_t pin, PinMode mode);
void set_pin_pull(gpio_t pin, PinPullMode mode);
int get_pin(gpio_t pin);


#endif //GPIO_H
