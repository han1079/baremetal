#include <timer.h>

uint32_t ClockConfigReg1Value;
uint32_t ClockControlReg1Value;
uint32_t ClockControlReg2Value;

uint32_t SystemCoreClock;
#define GET_CLOCK_DEBUG_VALUES() \
    ClockConfigReg1Value = RCC->CLOCK_CONFIG; \
    ClockControlReg1Value = RCC->CLOCK_CONTROL; \
    ClockControlReg2Value = RCC->CLOCK_CONTROL_2;

#define SET_SYS_CLOCK_TO_HSI48() \
    set_register(&RCC->CLOCK_CONFIG, 0, 2, 0b11UL);

#define SET_SYS_CLOCK_TO_HSI() \
    set_register(&RCC->CLOCK_CONFIG, 0, 2, 0b00UL);

#define SET_HSI48_ON() \
    SET_BIT(RCC->CLOCK_CONTROL_2, 16);

#define SET_HSI48_OFF() \
    RESET_BIT(RCC->CLOCK_CONTROL_2, 16);

#define GET_SYS_CLOCK_SOURCE() \
    ((RCC->CLOCK_CONFIG >> 2) & 0b11UL)

#define SYS_CLOCK_IS_HSI48() \
    (GET_SYS_CLOCK_SOURCE() == 0b11UL)

#define SYS_CLOCK_IS_HSI() \
    (GET_SYS_CLOCK_SOURCE() == 0b00UL)

#define HSI_ON_FLAG() \
    GET_BIT(RCC->CLOCK_CONTROL, 0)

#define HSI48_ON_FLAG() \
    GET_BIT(RCC->CLOCK_CONTROL_2, 16)

#define HSI48_RDY_FLAG() \
    GET_BIT(RCC->CLOCK_CONTROL_2, 17)

#define HSI_RDY_FLAG() \
    GET_BIT(RCC->CLOCK_CONTROL, 1)

void sys_clock_set_high_speed(void) {
    if (DEBUG) { GET_CLOCK_DEBUG_VALUES() }

    if (SYS_CLOCK_IS_HSI48()) {
        // Already using HSI48, nothing to do
        return;
    }

    ASSERT(!HSI48_ON_FLAG()); // HSI48 should be off before enabling. Otherwise, something is wrong.

    SET_BIT(RCC->CLOCK_CONTROL_2, 16); // Enable HSI48 (bit 16 of Clock Control Reg 2)

    int attempts = 100;
    while (attempts > 0) {
        if (HSI48_RDY_FLAG()) { break; }
        attempts--;
    }

    ASSERT(attempts > 0); // RDY Flag should be set easily.

    SET_SYS_CLOCK_TO_HSI48();

    attempts = 100;
    while (attempts > 0) {
        if (SYS_CLOCK_IS_HSI48()) { break; }
        attempts--;
    }

    ASSERT(attempts > 0); // Sys clock should set smoothly.

    SystemCoreClock = 48000000UL;
    if (DEBUG) { GET_CLOCK_DEBUG_VALUES() }

}
void sys_clock_set_standard_speed(void) {
    // Unsets HSI48 and returns to default HSI clock 
    if (DEBUG) { GET_CLOCK_DEBUG_VALUES() }

    if (SYS_CLOCK_IS_HSI()) {
        // Already using HSI, nothing to do
        return;
    }
    ASSERT(HSI_ON_FLAG()); // HSI should be on already. We don't ever turn it off.

    SET_SYS_CLOCK_TO_HSI();

    int attempts = 100;
    while (attempts > 0) {
        if (SYS_CLOCK_IS_HSI()) { break; }
        attempts--;
    }

    ASSERT(attempts > 0); // Sys clock should set smoothly.
    SET_HSI48_OFF();

    __asm__("NOP"); // Small delay to allow HSI48 to turn off
    ASSERT(!HSI48_ON_FLAG()); // HSI48 should be off now.

    SystemCoreClock = 8000000UL;
    if (DEBUG) { GET_CLOCK_DEBUG_VALUES() }
}
// typedef struct {
//     REGADDR_T CONTROL_REG_1;
//     REGADDR_T CONTROL_REG_2;
//     REGADDR_T SLAVE_MODE_CONTROL;
//     REGADDR_T DMA_INTERRUPT_ENABLE;
//     REGADDR_T STATUS_REG;
//     REGADDR_T CAPTURE_COMPARE_MODE_1;
//     REGADDR_T CAPTURE_COMPARE_MODE_2;
//     REGADDR_T CAPTURE_COMPARE_ENABLE;
//     REGADDR_T COUNTER;
//     REGADDR_T PRESCALER;
//     REGADDR_T AUTO_RELOAD;
//     REGADDR_T REPEAT_COUNTER;
//     REGADDR_T CAPTURE_COMPARE_REG_1;
//     REGADDR_T CAPTURE_COMPARE_REG_2;
//     REGADDR_T CAPTURE_COMPARE_REG_3;
//     REGADDR_T CAPTURE_COMPARE_REG_4;
//     REGADDR_T BREAK_DEADTIME_REG;
//     REGADDR_T DMA_CONTROL;
//     REGADDR_T DMA_BURST_ADDR;
// } TIMER_Typedef;



void timer_init(TimerControl_t timer_ctrl, uint32_t prescaler, uint16_t counter_threshold) {
    ASSERT(!GET_BIT(timer_ctrl.p_TIMER_BASE->CONTROL_REG_1, 0)); // Timer should be disabled before initialization
    
    SET_BIT(*(timer_ctrl.p_RCC), timer_ctrl.rcc_offset);
    timer_ctrl.p_TIMER_BASE->PRESCALER = prescaler;
    timer_ctrl.p_TIMER_BASE->AUTO_RELOAD_THRESHOLD_REG = counter_threshold;
    SET_BIT(timer_ctrl.p_TIMER_BASE->CONTROL_REG_1, 7); // Enable auto-reload preload
}

void timer_disable(TimerControl_t timer_ctrl) {
    TIMER_DISABLE(timer_ctrl);
    RESET_BIT(*(timer_ctrl.p_RCC), timer_ctrl.rcc_offset);
}

void timer_enable_interrupt(TimerControl_t timer_ctrl) {
    TIMER_ENABLE_INTERRUPTS(timer_ctrl);
}
void timer_disable_interrupt(TimerControl_t timer_ctrl) {
    TIMER_DISABLE_INTERRUPTS(timer_ctrl);
}
void timer_start(TimerControl_t timer_ctrl) {
    TIMER_ENABLE(timer_ctrl);
}
void timer_pause(TimerControl_t timer_ctrl) {
    TIMER_DISABLE(timer_ctrl);
}

void timer_set_counter_threshold(TimerControl_t timer_ctrl, uint16_t threshold) {
    ASSERT(GET_BIT(timer_ctrl.p_TIMER_BASE->CONTROL_REG_1, 7)); // Auto-reload preload must be enabled
    timer_ctrl.p_TIMER_BASE->AUTO_RELOAD_THRESHOLD_REG = threshold;
}