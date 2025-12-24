#!/bin/bash

if [ -z BAREMETAL_ENV_ACTIVE ]; then
    echo "Environment Not Active!"
else
    echo "Running OpenOCD for STM32"
    openocd -s ./tools/openocd/openocd/scripts -f interface/stlink-v2.cfg -f target/stm32f0x.cfg
fi
