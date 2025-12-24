#!/bin/bash 
if [ -z BAREMETAL_ENV_ACTIVE ]; then
    echo "Environment Not Active!"
else 
    arm-none-eabi-gdb -q -x gdb_config/base.gdb ./build/stm32/app.elf
fi

