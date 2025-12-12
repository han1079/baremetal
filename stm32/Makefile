TOOLCHAIN_PATH = ../tools/gcc/bin
INCLUDE_PATH= ../tools/gcc/lib/gcc/arm-none-eabi/10.3.1/include
OPENOCD= ../tools/openocd/bin/openocd
OPENOCD_PATH= ../tools/openocd/openocd/scripts

CC = $(TOOLCHAIN_PATH)/arm-none-eabi-gcc
OBJCOPY = $(TOOLCHAIN_PATH)/arm-none-eabi-objcopy
SIZE = $(TOOLCHAIN_PATH)/arm-none-eabi-size

CCFLAGS = -mcpu=cortex-m0 -mthumb -g -O0 -Wall -I $(INCLUDE_PATH)
LDFLAGS = -mcpu=cortex-m0 -mthumb -nostdlib

all : clean app.bin

# --- APP BUILD ---

app.o : app.c
	$(CC) $(CCFLAGS) -c app.c -o app.o

app.elf : app.o startup.o gpio.o
	$(CC) $(LDFLAGS) -T app.ld $^ -o $@

app.bin : app.elf
	$(OBJCOPY) -O binary $^ $@

# --- GPIO BUILD ---

gpio.o : gpio.c 
	$(CC) $(CCFLAGS) -c gpio.c -o gpio.o

# --- STARTUP BUILD ---

startup.o : startup.c
	$(CC) $(CCFLAGS) -c $^ -o $@

# --- UTILITIES
clean :
	rm *.o *.bin *.elf

flash:
	$(OPENOCD) -s $(OPENOCD_PATH) -f interface/stlink.cfg -f target/stm32f0x.cfg -c "program app.elf verify reset exit"
