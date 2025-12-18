#ifndef M0_CLOCK_H
#define M0_CLOCK_H

#include <core/common.h>

#define FLASH_ACR (*(volatile uint32_t*)0x40022000u)
#define FLASH_ACR_LATENCY (1u << 0)

static inline void flash_set_latency_1ws(void)
{
    FLASH_ACR |= FLASH_ACR_LATENCY;
}

static inline void flash_set_latency_0ws(void)
{
    FLASH_ACR &= ~FLASH_ACR_LATENCY;
}
void sys_clock_set_high_speed(void);
void sys_clock_set_standard_speed(void);
uint32_t get_system_core_clock(void);

uint32_t get_ahb_prescaler_divider(void);
uint32_t get_apb_prescalar_divider(void);
uint32_t get_apb1_prescaler_divider(void);
uint32_t get_apb2_prescaler_divider(void);

#endif //M0_CLOCK_H