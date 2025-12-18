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

static inline uint32_t lock_interrupts_and_save() {
    uint32_t primask_val;
    __asm__ volatile ("mrs %0, primask" : "=r"(primask_val) :: "memory");
    __asm__ volatile ("cpsid i" : : : "memory");
    return primask_val;
}

static inline void unlock_interrupts_and_restore(uint32_t primask_val) {
    __asm__ volatile ("msr primask, %0" :: "r"(primask_val) : "memory");
}


#endif //INTERRUPTS_H
