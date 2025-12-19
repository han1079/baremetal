#ifndef COMMON_H
#define COMMON_H
#include <stdint.h>
#include <stdbool.h>

typedef volatile uint32_t REGADDR_T;


#define DEBUG 1
#define SET_BIT(val, x) (val |= (1UL << x)) 
#define RESET_BIT(val, x) (val &= ~(1UL << x)) 
#define SET_WORD(val, word) (val = word)
#define GET_BIT(val, x) (val & (1UL << x)) 
#ifdef DEBUG
    // If DEBUG is defined, check the condition
    #define ASSERT(cond) \
        do { \
            if (!(cond)) { \
                __asm("BKPT #0"); \
            } \
        } while(0)
#else
    // If release build, compile it out completely
    #define ASSERT(cond) ((void)0)
#endif
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


#define RCC_BASE 0x40021000UL
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
#define RCC ((RCC_Typedef*) RCC_BASE)

typedef enum {
    WWDG =                  (uint32_t)0,
    PVD_VDDIO2 =            (uint32_t)1,
    RTC =                   (uint32_t)2,
    FLASH =                 (uint32_t)3,
    RCC_CRS =               (uint32_t)4,
    EXTI0_1 =               (uint32_t)5,
    EXTI2_3 =               (uint32_t)6,
    EXTI4_15 =              (uint32_t)7,
    TSC =                   (uint32_t)8,
    DMA1CH1 =               (uint32_t)9,
    DMA1CH23_DMA2CH12 =     (uint32_t)10,
    DMA1CH4TO7_DMA2CH3TO5 = (uint32_t)11,
    ADC_COMP =              (uint32_t)12,
    TIM1_BRK_UP_TRG_COM =   (uint32_t)13,
    TIM1_CC =               (uint32_t)14,
    TIM2_GLOBAL =           (uint32_t)15,
    TIM3_GLOBAL =           (uint32_t)16,
    TIM6_GLOBAL_DACUNDER =  (uint32_t)17,
    TIM7_GLOBAL =           (uint32_t)18,
    TIM14_GLOBAL =          (uint32_t)19,
    TIM15_GLOBAL =          (uint32_t)20,
    TIM16_GLOBAL =          (uint32_t)21,
    TIM17_GLOBAL =          (uint32_t)22,
    I2C1_GLOBAL =           (uint32_t)23,
    I2C2_GLOBAL =           (uint32_t)24,
    SPI1_GLOBAL =           (uint32_t)25,
    SPI2_GLOBAL =           (uint32_t)26,
    USART1_GLOBAL =         (uint32_t)27,
    USART2_GLOBAL =         (uint32_t)28,
    USART3TO8_GLOBAL =      (uint32_t)29,
    CEC_CAN_GLOBAL =        (uint32_t)30,
    USB_GLOBAL =            (uint32_t)31,
} InterruptType;

typedef enum {
    PRIORITY_0 = (uint32_t)(0b00),
    PRIORITY_1 = (uint32_t)(0b01),
    PRIORITY_2 = (uint32_t)(0b10),
    PRIORITY_3 = (uint32_t)(0b11),
} InterruptPriority;


#endif //COMMON_H
