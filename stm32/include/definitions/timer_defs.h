#ifndef TIMER_DEFS_H
#define TIMER_DEFS_H

#include <core/common.h>

typedef struct {
    REGADDR_T CONTROL_REG_1;
    REGADDR_T CONTROL_REG_2;
    REGADDR_T SLAVE_MODE_CONTROL;
    REGADDR_T DMA_INTERRUPT_ENABLE;
    REGADDR_T STATUS_REG;
    REGADDR_T CAPTURE_COMPARE_MODE_1;
    REGADDR_T CAPTURE_COMPARE_MODE_2;
    REGADDR_T CAPTURE_COMPARE_ENABLE;
    REGADDR_T COUNTER;
    REGADDR_T PRESCALER;
    REGADDR_T AUTO_RELOAD_THRESHOLD_REG;
    REGADDR_T REPEAT_COUNTER;
    REGADDR_T CAPTURE_COMPARE_REG_1;
    REGADDR_T CAPTURE_COMPARE_REG_2;
    REGADDR_T CAPTURE_COMPARE_REG_3;
    REGADDR_T CAPTURE_COMPARE_REG_4;
    REGADDR_T BREAK_DEADTIME_REG;
    REGADDR_T DMA_CONTROL;
    REGADDR_T DMA_BURST_ADDR;
} TIMER_Typedef;

typedef struct {
    TIMER_Typedef* p_TIMER_BASE;
    REGADDR_T* p_RCC;
    uint8_t rcc_offset;
} TimerControl_t;
extern const TimerControl_t TIMER1;
extern const TimerControl_t TIMER2;
extern const TimerControl_t TIMER3;
extern const TimerControl_t TIMER6;
extern const TimerControl_t TIMER7;
extern const TimerControl_t TIMER14;
extern const TimerControl_t TIMER15;
extern const TimerControl_t TIMER16;
extern const TimerControl_t TIMER17;

#endif //TIMER_DEFS_H