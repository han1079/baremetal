#ifndef GPIO_DEFS_H
#define GPIO_DEFS_H
#include <core/common.h>

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

typedef enum AFKeys{
    AF0 = 0b0000,
    AF1 = 0b0001,
    AF2 = 0b0010,
    AF3 = 0b0011,
    AF4 = 0b0100,
    AF5 = 0b0101,
    AF6 = 0b0110,
    AF7 = 0b0111,
    NO_AF = 0b1000,
} AFKey;

typedef enum AlternateFunction {
    AF_TIMER,
    AF_UART,
    AF_I2C,
    AF_SPI,
    AF_CAN,
} AlternateFunction;

typedef struct {
    AFKey TIMER;
    AFKey UART;
    AFKey I2C;
    AFKey SPI;
    AFKey CAN;
} gpio_af_lut_t;

typedef struct {
    GPIO_Typedef* base;
    uint8_t offset;
    gpio_af_lut_t* af;
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