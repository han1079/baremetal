#ifndef TIMER_H
#define TIMER_H
#include <common.h>
#include <timer_defs.h>

extern uint32_t ClockConfigReg1Value;
extern uint32_t ClockControlReg1Value;
extern uint32_t ClockControlReg2Value;

extern uint32_t SystemCoreClock;


#define TIMER_ENABLE(timer_ctrl) \
    SET_BIT((timer_ctrl.p_TIMER_BASE)->CONTROL_REG_1, 0);
#define TIMER_DISABLE(timer_ctrl) \
    RESET_BIT((timer_ctrl.p_TIMER_BASE)->CONTROL_REG_1, 0);

    
#define TIMER_ENABLE_INTERRUPTS(timer_ctrl) \
    SET_BIT((timer_ctrl.p_TIMER_BASE)->DMA_INTERRUPT_ENABLE, 0);

#define TIMER_DISABLE_INTERRUPTS(timer_ctrl) \
    RESET_BIT((timer_ctrl.p_TIMER_BASE)->DMA_INTERRUPT_ENABLE, 0);

#define TIMER_ENABLE_UEV_GENERATION(timer_ctrl) \
    SET_BIT((timer_ctrl.p_TIMER_BASE)->CONTROL_REG_1, 1);

#define TIMER_DISABLE_UEV_GENERATION(timer_ctrl) \
    RESET_BIT((timer_ctrl.p_TIMER_BASE)->CONTROL_REG_1, 1);

#define TIMER_CLEAR_UPDATE_INTERRUPT_FLAG(timer_ctrl) \
    RESET_BIT((timer_ctrl.p_TIMER_BASE)->STATUS_REG, 0);

#define TIMER_UPDATE_INTERRUPT_FLAG_IS_SET(timer_ctrl) \
    (GET_BIT((timer_ctrl.p_TIMER_BASE)->STATUS_REG, 0))

#define TIMER_FORCE_UPDATE_EVENT(timer_ctrl) \
    SET_BIT((timer_ctrl.p_TIMER_BASE)->EVENT_GENERATION_REG, 0);

void sys_clock_set_high_speed(void);
void sys_clock_set_standard_speed(void);

void timer_init(TimerControl_t timer_ctrl, uint32_t prescaler, uint16_t counter_threshold);
void timer_disable(TimerControl_t timer_ctrl);
void timer_enable_interrupt(TimerControl_t timer_ctrl);
void timer_disable_interrupt(TimerControl_t timer_ctrl);
void timer_start(TimerControl_t timer_ctrl);
void timer_pause(TimerControl_t timer_ctrl);
void timer_set_counter_threshold(TimerControl_t timer_ctrl, uint16_t threshold);



#endif //TIMER_H