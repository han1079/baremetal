#include <configs/setup.h>

void gpio_setup(void) {
    gpio_initialize_clock();
}

void timer_setup(void) {
    // Placeholder for timer initialization logic
}

void high_speed_setup(void) {
    sys_clock_set_high_speed();
    allow_interrupts_global();
    systick_init_1khz(get_system_core_clock());
    gpio_setup();
    timer_setup();
}

void standard_speed_setup(void) {
    sys_clock_set_standard_speed();
    allow_interrupts_global();
    systick_init_1khz(get_system_core_clock());
    gpio_setup(); 
    timer_setup();
}

