#ifndef M0_CLOCK_H
#define M0_CLOCK_H

#include <common.h>

void sys_clock_set_high_speed(void);
void sys_clock_set_standard_speed(void);
uint32_t get_system_core_clock(void);

#endif //M0_CLOCK_H