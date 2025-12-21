#ifndef CORE_SETUP_H
#define CORE_SETUP_H

#include <core/common.h>
#include <core/clock.h>
#include <core/systick.h>
#include <core/interrupts.h>
#include <core/nvic.h>
#include <drivers/gpio.h>
#include <drivers/timer.h>

void high_speed_setup(void);
void standard_speed_setup(void);

void gpio_setup(void);
void timer_setup(void);

#endif //CORE_SETUP_H
