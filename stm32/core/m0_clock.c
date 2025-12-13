#include <m0_clock.h>
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


void get_system_core_clock(void) {
    if (SYS_CLOCK_IS_HSI48()) {
        SystemCoreClock = 48000000UL;
    } else if (SYS_CLOCK_IS_HSI()) {
        SystemCoreClock = 8000000UL;
    } else {
        ASSERT(0); // Unknown clock source
    }
}

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