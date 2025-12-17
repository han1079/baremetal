#include <core/systick.h>
#include <core/m0_clock.h>
#include <core/interrupts.h>

#define SYSTICK_VALUE_MASK 0x00FFFFFFUL

static volatile uint32_t ms_counter = 0;


uint32_t get_elapsed_ms() { // Public Getter API for elapsed ms
    return ms_counter;
}

void systick_enable() {
    systick_clear_current_value(); // Clear current value to avoid immediate interrupt
    SET_BIT(SYSTICK->CONTROL_STATUS_REG, 0); // Enable the SysTick timer
    SET_BIT(SYSTICK->CONTROL_STATUS_REG, 1); // Lets SysTick trigger exceptions
}
void systick_disable() {
    RESET_BIT(SYSTICK->CONTROL_STATUS_REG, 0); // Disable the SysTick timer
    RESET_BIT(SYSTICK->CONTROL_STATUS_REG, 1); // Disable SysTick exceptions
}
void systick_set_reload_value(uint32_t value) {
    SET_WORD(SYSTICK->RELOAD_VALUE_REG, value & SYSTICK_VALUE_MASK); // Mask to 24 bits and write value
}
void systick_enable_for_1ms_ticks() {
    uint32_t system_core_clock = get_system_core_clock();
    systick_set_reload_value(system_core_clock / 1000 - 1); // Set for 1ms ticks
    systick_enable();
}

void delay_ms_blocking(uint32_t ms) {
    uint32_t start_ms = ms_counter;
    while ((ms_counter - start_ms) < ms){
        __asm__("nop"); // Super explicit. We are busy doing a "no-op" instruction in a loop.
    }
}

void delay_ms_interruptible(uint32_t ms) {
    uint32_t start_ms = ms_counter;
    while((ms_counter - start_ms) < ms) {
        wait_for_interrupt(); // Sleep until interrupt or SysTick fires (which is itself an interrupt)
    }

}
