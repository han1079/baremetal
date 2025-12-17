
# Top-level Makefile - supports multiple targets
PROJECT_ROOT = $(CURDIR)
export PROJECT_ROOT

# Default target
TARGET ?= stm32

# Target-specific configurations
ifeq ($(TARGET),stm32)
    TARGET_DIR = $(PROJECT_ROOT)/stm32
    TOOLCHAIN_PATH = $(PROJECT_ROOT)/tools/gcc/bin
    STD_INCLUDE_PATH = $(PROJECT_ROOT)/tools/gcc/lib/gcc/arm-none-eabi/10.3.1/include
    PROJ_INCLUDE_PATH = $(TARGET_DIR)/include
    OPENOCD = $(PROJECT_ROOT)/tools/openocd/bin/openocd
    OPENOCD_PATH = $(PROJECT_ROOT)/tools/openocd/openocd/scripts
    
    CC = $(TOOLCHAIN_PATH)/arm-none-eabi-gcc
    OBJCOPY = $(TOOLCHAIN_PATH)/arm-none-eabi-objcopy
    SIZE = $(TOOLCHAIN_PATH)/arm-none-eabi-size
    
    CCFLAGS = -mcpu=cortex-m0 -mthumb -g -O0 -Wall -I $(STD_INCLUDE_PATH) -I $(PROJ_INCLUDE_PATH)
    LDFLAGS = -mcpu=cortex-m0 -mthumb -nostdlib
    LD = $(TARGET_DIR)/app.ld
    
    APP_SRCS = $(TARGET_DIR)/app.c

    CORE_SRCS = $(TARGET_DIR)/core/nvic.c \
				$(TARGET_DIR)/core/m0_clock.c \
				$(TARGET_DIR)/core/systick.c

	CONFIG_SRCS = $(TARGET_DIR)/configs/setup.c

    PERIPH_SRCS = $(TARGET_DIR)/peripherals/gpio.c \
                  $(TARGET_DIR)/peripherals/gpio_defs.c \
                  $(TARGET_DIR)/peripherals/timer.c \
                  $(TARGET_DIR)/peripherals/timer_defs.c
				  
    STARTUP_SRCS = $(TARGET_DIR)/startup.c
    
    FLASH_CMD = $(OPENOCD) -s $(OPENOCD_PATH) -f interface/stlink.cfg -f target/stm32f0x.cfg -c "program $(BUILD_DIR)/app.elf verify reset exit"
endif

# Add more targets here (e.g., avr, esp32, etc.)
# ifeq ($(TARGET),avr)
#     ...
# endif

BUILD_DIR = $(PROJECT_ROOT)/build/$(TARGET)
ELF = $(BUILD_DIR)/app.elf
BIN = $(BUILD_DIR)/app.bin

SRCS = $(APP_SRCS) $(PERIPH_SRCS) $(STARTUP_SRCS) $(CORE_SRCS) $(CONFIG_SRCS)
OBJS = $(patsubst $(TARGET_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))

all: $(BIN)
	@echo "Build complete for target: $(TARGET)"
	$(SIZE) $(ELF)

# Universal compile rule
$(BUILD_DIR)/%.o: $(TARGET_DIR)/%.c
	@mkdir -p $(dir $@)
	@echo "Compiling $< ..."
	$(CC) $(CCFLAGS) -c $< -o $@

# Link rule
$(ELF): $(OBJS)
	@mkdir -p $(dir $@)
	@echo "Linking $@ ..."
	$(CC) $(LDFLAGS) -T $(LD) $^ -o $@

# Binary conversion
$(BIN): $(ELF)
	@mkdir -p $(dir $@)
	@echo "Creating binary $@ ..."
	$(OBJCOPY) -O binary $< $@

clean:
	rm -rf $(BUILD_DIR)

clean-all:
	rm -rf $(PROJECT_ROOT)/build

flash: $(BIN)
	$(FLASH_CMD)

help:
	@echo "Available targets: stm32 (add more as needed)"
	@echo "Usage: make [TARGET=<target>] [goal]"
	@echo "Example: make TARGET=stm32 all"
	@echo "         make TARGET=stm32 flash"
	@echo "         make clean"

.PHONY: all clean clean-all flash help
