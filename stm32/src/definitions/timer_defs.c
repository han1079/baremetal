#include <peripherals/timer.h>

#define TIM1_BASE 0x40012C00UL
#define TIM2_BASE 0x40000000UL
#define TIM3_BASE 0x40000400UL
#define TIM6_BASE 0x40001000UL
#define TIM7_BASE 0x40001400UL
#define TIM14_BASE 0x40002000UL
#define TIM15_BASE 0x40014000UL
#define TIM16_BASE 0x40014400UL
#define TIM17_BASE 0x40014800UL

const TimerControl_t TIMER1 = {
    .p_TIMER_BASE = (TIMER_Typedef*)TIM1_BASE,
    .p_RCC = (REGADDR_T*)&(RCC->PERIPH_APB_2_CLOCK_ENABLE),
    .rcc_offset = 11,
};

const TimerControl_t TIMER2 = {
    .p_TIMER_BASE = (TIMER_Typedef*)TIM2_BASE,
    .p_RCC = (REGADDR_T*)&(RCC->PERIPH_APB_1_CLOCK_ENABLE),
    .rcc_offset = 0,
};

const TimerControl_t TIMER3 = {
    .p_TIMER_BASE = (TIMER_Typedef*)TIM3_BASE,
    .p_RCC = (REGADDR_T*)&(RCC->PERIPH_APB_1_CLOCK_ENABLE),
    .rcc_offset = 1,
};

const TimerControl_t TIMER6 = {
    .p_TIMER_BASE = (TIMER_Typedef*)TIM6_BASE,
    .p_RCC = (REGADDR_T*)&(RCC->PERIPH_APB_1_CLOCK_ENABLE),
    .rcc_offset = 4,
};

const TimerControl_t TIMER7 = {
    .p_TIMER_BASE = (TIMER_Typedef*)TIM7_BASE,
    .p_RCC = (REGADDR_T*)&(RCC->PERIPH_APB_1_CLOCK_ENABLE),
    .rcc_offset = 5,
};

const TimerControl_t TIMER14 = {
    .p_TIMER_BASE = (TIMER_Typedef*)TIM14_BASE,
    .p_RCC = (REGADDR_T*)&(RCC->PERIPH_APB_1_CLOCK_ENABLE),
    .rcc_offset = 8,
};

const TimerControl_t TIMER15 = {
    .p_TIMER_BASE = (TIMER_Typedef*)TIM15_BASE,
    .p_RCC = (REGADDR_T*)&(RCC->PERIPH_APB_2_CLOCK_ENABLE),
    .rcc_offset = 16,
};

const TimerControl_t TIMER16 = {
    .p_TIMER_BASE = (TIMER_Typedef*)TIM16_BASE,
    .p_RCC = (REGADDR_T*)&(RCC->PERIPH_APB_2_CLOCK_ENABLE),
    .rcc_offset = 17,
};

const TimerControl_t TIMER17 = {
    .p_TIMER_BASE = (TIMER_Typedef*)TIM17_BASE,
    .p_RCC = (REGADDR_T*)&(RCC->PERIPH_APB_2_CLOCK_ENABLE),
    .rcc_offset = 18,
};