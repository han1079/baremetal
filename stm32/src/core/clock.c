#include <core/clock.h>

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



uint32_t get_system_core_clock(void) {
    if (SYS_CLOCK_IS_HSI48()) {
        return 48000000UL;
    } else if (SYS_CLOCK_IS_HSI()) {
        return 8000000UL;
    } else {
        ASSERT(0); // Unknown clock source
    }
    return 8000000UL; // Default to 8 MHz to prevent compiler warnings
}

void sys_clock_set_high_speed(void) {
    if (DEBUG) { GET_CLOCK_DEBUG_VALUES() }

    flash_set_latency_1ws();
    if (SYS_CLOCK_IS_HSI48()) {
        // Already using HSI48, nothing to do
        return;
    }

    ASSERT(!(HSI48_ON_FLAG())); // HSI48 should be off before enabling. Otherwise, something is wrong.

    SET_BIT(RCC->CLOCK_CONTROL_2, 16); // Enable HSI48 (bit 16 of Clock Control Reg 2)

    int attempts = 100;
    while (attempts > 0) {
        attempts--;
    }

    ASSERT(HSI48_RDY_FLAG()); // RDY Flag should be set easily.

    SET_SYS_CLOCK_TO_HSI48();

    attempts = 100;
    while (attempts > 0) {
        attempts--;
    }

    ASSERT((SYS_CLOCK_IS_HSI48())); // Sys clock should set smoothly.

    SystemCoreClock = 48000000UL;
    if (DEBUG) { GET_CLOCK_DEBUG_VALUES() }

}
void sys_clock_set_standard_speed(void) {
    // Unsets HSI48 and returns to default HSI clock 
    if (DEBUG) { GET_CLOCK_DEBUG_VALUES() }
    flash_set_latency_0ws();

    if (SYS_CLOCK_IS_HSI()) {
        // Already using HSI, nothing to do
        return;
    }
    ASSERT((HSI_ON_FLAG())); // HSI should be on already. We don't ever turn it off.

    SET_SYS_CLOCK_TO_HSI();

    int attempts = 100;
    while (attempts > 0) {
        if (SYS_CLOCK_IS_HSI()) { break; }
        attempts--;
    }

    ASSERT((attempts > 0)); // Sys clock should set smoothly.
    SET_HSI48_OFF();

    __asm__("NOP"); // Small delay to allow HSI48 to turn off
    ASSERT(!(HSI48_ON_FLAG())); // HSI48 should be off now.

    SystemCoreClock = 8000000UL;
    if (DEBUG) { GET_CLOCK_DEBUG_VALUES() }
}

uint32_t get_ahb_prescaler_divider(void) {
    // AHB prescaler is bits 4-7 of RCC_CLOCK_CONFIG
    uint8_t prescaler_bits = (RCC->CLOCK_CONFIG >> 4) & 0b1111UL;
    switch (prescaler_bits) {
        case 0b0000: return 1;
        case 0b1000: return 2;
        case 0b1001: return 4;
        case 0b1010: return 8;
        case 0b1011: return 16;
        case 0b1100: return 64;
        case 0b1101: return 128;
        case 0b1110: return 256;
        case 0b1111: return 512;
        default: return 1; // Reserved values treated as no division
    }
}

uint32_t get_apb_prescalar_divider(void) {
    // Grabs default prescalar. In M0 it's APB1.
    return get_apb1_prescaler_divider();
}

uint32_t get_apb1_prescaler_divider(void) {
    // APB1 prescaler is bits 8-10 of RCC_CLOCK_CONFIG
    uint8_t prescaler_bits = (RCC->CLOCK_CONFIG >> 8) & 0b111UL;
    switch (prescaler_bits) {
        case 0b000: return 1;
        case 0b100: return 2;
        case 0b101: return 4;
        case 0b110: return 8;
        case 0b111: return 16;
        default: return 1; // Reserved values treated as no division
    }
}

uint32_t get_apb2_prescaler_divider(void) {
    // Not applicable for M0.
    return 1;
}
