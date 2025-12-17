#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include <core/common.h>

static inline void wait_for_interrupt(void) {
    __asm__ volatile ("wfi" : : : "memory");
}
static inline void allow_interrupts_global(void) {
    __asm__ volatile ("cpsie i" : : : "memory");
}

static inline void disallow_interrupts_global(void) {
    __asm__ volatile ("cpsid i" : : : "memory");
}


#endif //INTERRUPTS_H