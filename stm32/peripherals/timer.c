#include <timer.h>


// typedef struct {
//     REGADDR_T CONTROL_REG_1;
//     REGADDR_T CONTROL_REG_2;
//     REGADDR_T SLAVE_MODE_CONTROL;
//     REGADDR_T DMA_INTERRUPT_ENABLE;
//     REGADDR_T STATUS_REG;
//     REGADDR_T CAPTURE_COMPARE_MODE_1;
//     REGADDR_T CAPTURE_COMPARE_MODE_2;
//     REGADDR_T CAPTURE_COMPARE_ENABLE;
//     REGADDR_T COUNTER;
//     REGADDR_T PRESCALER;
//     REGADDR_T AUTO_RELOAD;
//     REGADDR_T REPEAT_COUNTER;
//     REGADDR_T CAPTURE_COMPARE_REG_1;
//     REGADDR_T CAPTURE_COMPARE_REG_2;
//     REGADDR_T CAPTURE_COMPARE_REG_3;
//     REGADDR_T CAPTURE_COMPARE_REG_4;
//     REGADDR_T BREAK_DEADTIME_REG;
//     REGADDR_T DMA_CONTROL;
//     REGADDR_T DMA_BURST_ADDR;
// } TIMER_Typedef;



void timer_init(TimerControl_t timer_ctrl, uint32_t prescaler, uint16_t counter_threshold) {
    ASSERT(!GET_BIT(timer_ctrl.p_TIMER_BASE->CONTROL_REG_1, 0)); // Timer should be disabled before initialization
    
    SET_BIT(*(timer_ctrl.p_RCC), timer_ctrl.rcc_offset);
    timer_ctrl.p_TIMER_BASE->PRESCALER = prescaler;
    timer_ctrl.p_TIMER_BASE->AUTO_RELOAD_THRESHOLD_REG = counter_threshold;
    SET_BIT(timer_ctrl.p_TIMER_BASE->CONTROL_REG_1, 7); // Enable auto-reload preload
}

void timer_disable(TimerControl_t timer_ctrl) {
    TIMER_DISABLE(timer_ctrl);
    RESET_BIT(*(timer_ctrl.p_RCC), timer_ctrl.rcc_offset);
}

void timer_enable_interrupt(TimerControl_t timer_ctrl) {
    TIMER_ENABLE_INTERRUPTS(timer_ctrl);
}
void timer_disable_interrupt(TimerControl_t timer_ctrl) {
    TIMER_DISABLE_INTERRUPTS(timer_ctrl);
}
void timer_start(TimerControl_t timer_ctrl) {
    TIMER_ENABLE(timer_ctrl);
}
void timer_pause(TimerControl_t timer_ctrl) {
    TIMER_DISABLE(timer_ctrl);
}

void timer_set_counter_threshold(TimerControl_t timer_ctrl, uint16_t threshold) {
    ASSERT(GET_BIT(timer_ctrl.p_TIMER_BASE->CONTROL_REG_1, 7)); // Auto-reload preload must be enabled
    timer_ctrl.p_TIMER_BASE->AUTO_RELOAD_THRESHOLD_REG = threshold;
}