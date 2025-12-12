#!/bin/bash
openocd -s ./tools/openocd/openocd/scripts -f interface/stlink-v2.cfg -f target/stm32f0x.cfg
