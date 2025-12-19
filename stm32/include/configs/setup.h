#ifndef SETUP_H
#define SETUP_H

#include <core/common.h>
#include <core/m0_clock.h>
#include <core/systick.h>
#include <core/interrupts.h>
#include <core/nvic.h>
#include <drivers/gpio.h>
#include <drivers/timer.h>

void high_speed_setup(void);
void standard_speed_setup(void);

void gpio_setup(void);
void timer_setup(void);

#endif //SETUP_H