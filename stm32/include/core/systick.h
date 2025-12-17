#ifndef SYSTICK_H
#define SYSTICK_H

#include <core/common.h>

#define SYSTICK_BASE 0xE000E010UL
typedef struct {
    REGADDR_T CONTROL_STATUS_REG;
    REGADDR_T RELOAD_VALUE_REG;
    REGADDR_T CURRENT_VALUE_REG;
    REGADDR_T CALIBRATION_VALUE_REG;
} SYSTICK_Typedef;
#define SYSTICK ((SYSTICK_Typedef*) SYSTICK_BASE)

// Inline Helpers

inline static uint32_t systick_get_reload_value() {
    return SYSTICK->RELOAD_VALUE_REG & 0x00FFFFFFUL; // Mask to 24 bits
}

inline static uint32_t systick_get_current_value() {
    return SYSTICK->CURRENT_VALUE_REG & 0x00FFFFFFUL; // Mask to 24 bits
}

inline static void systick_clear_current_value() {
    SYSTICK->CURRENT_VALUE_REG = 0; // Writing any value clears it
}

uint32_t get_elapsed_ms();

void systick_enable();
void systick_disable();
void systick_set_reload_value(uint32_t value);
void systick_init_1khz(uint32_t system_core_clock);

void delay_ms_blocking(uint32_t ms);
void delay_ms_interruptible(uint32_t ms);

#endif //SYSTICK_H