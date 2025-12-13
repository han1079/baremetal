#ifndef NVIC_H
#define NVIC_H
#include <common.h>

#define SYSCTRL_BASE 0xE000ED00UL
typedef struct{
    REGADDR_T CPUID;
    REGADDR_T INTERRUPT_CTRL_STATE;
    REGADDR_T VECTOR_TABLE_OFFSET; 
    REGADDR_T APPLICATION_INTERRUPT_RESET;
    REGADDR_T SYSTEM_CONTROL;
    REGADDR_T CONFIGURATION_CONTROL;
    REGADDR_T SYSTEM_HANDLER_PRIORITY_2;
    REGADDR_T SYSTEM_HANDLER_PRIORITY_3;
} SYSCTRL_Typedef;
#define SYSCTRL ((SYSCTRL_Typedef*) SYSCTRL_BASE)

#define SYSTICK_BASE 0xE000E010UL
typedef struct {
    REGADDR_T CONTROL_STATUS_REG;
    REGADDR_T RELOAD_VALUE_REG;
    REGADDR_T CURRENT_VALUE_REG;
    REGADDR_T CALIBRATION_VALUE_REG;
} SYSTICK_Typedef;
#define SYSTICK ((SYSTICK_Typedef*) SYSTICK_BASE)

#define NVIC_SET_ENABLE_REG (REGADDR_T*)0xE000E100UL
#define NVIC_CLEAR_ENABLE_REG (REGADDR_T*)0xE000E180UL
#define NVIC_SET_PENDING_REG (REGADDR_T*)0xE000E200UL
#define NVIC_CLEAR_PENDING_REG (REGADDR_T*)0xE000E280UL


#define NVIC_PRIORITY_BASE 0xE000E400UL
typedef struct {
    REGADDR_T WORD[8];
} NVIC_PRIORITY_Typedef;
#define NVIC_PRIORITY ((NVIC_PRIORITY_Typedef*) NVIC_PRIORITY_BASE)


static inline void system_enable_interrupts(void) {
    __asm__ volatile ("cpsie i" : : : "memory");
}

static inline void system_disable_interrupts(void) {
    __asm__ volatile ("cpsid i" : : : "memory");
}

static inline void force_system_reset(void) {
    __asm__ volatile ("dsb" : : : "memory");
    SYSCTRL->APPLICATION_INTERRUPT_RESET = (0x5FAUL << 16) | (1UL << 2);
    __asm__ volatile ("dsb" : : : "memory");
    while(1);
}

static inline void enable_interrupt_type(InterruptType type) {
    SET_BIT(*NVIC_SET_ENABLE_REG, type);
}
void disable_interrupt_type(InterruptType type) {
    SET_BIT(*NVIC_CLEAR_ENABLE_REG, type);
}

void force_interrupt_type(InterruptType type) {
    SET_BIT(*NVIC_SET_PENDING_REG, type);
}
void clear_interrupt_type(InterruptType type) {
    SET_BIT(*NVIC_CLEAR_PENDING_REG, type);
}

void set_interrupt_type_priority(InterruptType type, InterruptPriority priority);
uint32_t get_interrupt_type_priority(InterruptType type);


#endif //NVIC_H