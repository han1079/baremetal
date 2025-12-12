#ifndef REGISTER_MANIP_H
#define REGISTER_MANIP_H
#include <stdint.h>
typedef volatile uint32_t REGADDR_T;

#define SET_BIT(val, x) val |= (1UL << x) 
#define RESET_BIT(val, x) val &= ~(1UL << x) 
#define SET_WORD(val, word) val = word
#define GET_BIT(val, x) val & (1UL << x) 

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

#define AHB1_BASE 0x40021000UL

typedef struct {
    REGADDR_T CLOCK_CONTROL;
    REGADDR_T CLOCK_CONFIG;
    REGADDR_T CLOCK_INTERRUPT;
    REGADDR_T PERIPH_APB_2_RESET;
    REGADDR_T PERIPH_APB_1_RESET;
    REGADDR_T PERIPH_AHB_CLOCK_ENABLE;
    REGADDR_T PERIPH_APB_2_CLOCK_ENABLE;
    REGADDR_T PERIPH_APB_1_CLOCK_ENABLE;
    REGADDR_T RTC_CONTROL;
    REGADDR_T RCC_CONTROL_AND_STATUS;
    REGADDR_T PERIPH_AHB_RESET;
    REGADDR_T CLOCK_CONFIG_2;
    REGADDR_T CLOCK_CONFIG_3;
    REGADDR_T CLOCK_CONTROL_2;
} RCC_Typedef;

#define RCC ((RCC_Typedef*) AHB1_BASE)

static inline void set_register(REGADDR_T* reg, uint8_t offset, uint8_t num_bits, uint32_t data) {
    if (num_bits + offset > 32) {
        return;
    }
    // Bitshift back all 1's until it forms the "erase" mask
    uint32_t dest_mask = ~((0xFFFFFFFF >> (32 - num_bits)) << offset);

    // Bitshift forward all 1's and invert to create a mask 
    // that eliminates the possibility of data corruption
    // if bit length does not match data.
    uint32_t src_mask = ~(0xFFFFFFFF << num_bits);
    uint32_t clean_data = ((data & src_mask) << offset);

    (*reg) &= dest_mask;
    (*reg) |= clean_data;
}
#endif //REGISTER_MANIP_H
