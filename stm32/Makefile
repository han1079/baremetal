TOOLCHAIN_PATH = $(PROJECT_ROOT)/tools/gcc/bin
STD_INCLUDE_PATH= $(PROJECT_ROOT)/tools/gcc/lib/gcc/arm-none-eabi/10.3.1/include
PROJ_INCLUDE_PATH= $(PROJECT_ROOT)/stm32/include
OPENOCD= $(PROJECT_ROOT)/tools/openocd/bin/openocd
OPENOCD_PATH= $(PROJECT_ROOT)/tools/openocd/openocd/scripts

CC = $(TOOLCHAIN_PATH)/arm-none-eabi-gcc
OBJCOPY = $(TOOLCHAIN_PATH)/arm-none-eabi-objcopy
SIZE = $(TOOLCHAIN_PATH)/arm-none-eabi-size

CCFLAGS = -mcpu=cortex-m0 -mthumb -g -O0 -Wall -I $(STD_INCLUDE_PATH) -I $(PROJ_INCLUDE_PATH)
LDFLAGS = -mcpu=cortex-m0 -mthumb -nostdlib

BUILD_DIR = $(PROJECT_ROOT)/build



ELF = app.elf
BIN = app.bin
LD = app.ld
APP_SRCS = app.c
APP_OBJS = $(APP_SRCS:%.c=$(BUILD_DIR)/%.o)

CORE_SRCS = 	core/nvic.c

PERIPH_SRCS = 	peripherals/gpio.c \
				peripherals/gpio_defs.c \
				peripherals/timer.c \
				peripherals/timer_defs.c

PERIPH_OBJS = $(PERIPH_SRCS:%.c=$(BUILD_DIR)/%.o)

STARTUP_SRCS = startup.c
STARTUP_OBJS = $(STARTUP_SRCS:%.c=$(BUILD_DIR)/%.o)

SRCS = $(APP_SRCS) $(PERIPH_SRCS) $(STARTUP_SRCS) $(CORE_SRCS)
OBJS = $(addprefix $(BUILD_DIR)/, $(notdir $(SRCS:.c=.o)))

SRC_DIRS = $(dir $(SRCS))

vpath %.c $(SRC_DIRS)

all : $(BIN)



# 5. The Universal Rule
$(BUILD_DIR)/%.o : %.c
	@mkdir -p $(dir $@)
	@echo "Compiling $< ..."
	$(CC) $(CCFLAGS) -c $< -o $@

# 6. Link Rule (Updated to use ALL_OBJS)
$(ELF) : $(OBJS)
	@mkdir -p $(dir $@)
	@echo "Linking $@ ..."
	$(CC) $(LDFLAGS) -T $(LD) $^ -o $@

$(BIN) : $(ELF)
	@mkdir -p $(dir $@)
	@echo "Making binary $@ ..."
	$(OBJCOPY) -O binary $^ $@
# --- UTILITIES
clean :
	rm -rf $(BUILD_DIR) *.elf *.bin *.o

flash:
	$(OPENOCD) -s $(OPENOCD_PATH) -f interface/stlink.cfg -f target/stm32f0x.cfg -c "program app.elf verify reset exit"
