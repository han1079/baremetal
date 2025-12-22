
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
	PROJ_SOURCE_PATH = $(TARGET_DIR)/src
    OPENOCD = $(PROJECT_ROOT)/tools/openocd/bin/openocd
    OPENOCD_PATH = $(PROJECT_ROOT)/tools/openocd/openocd/scripts
    
    CC = $(TOOLCHAIN_PATH)/arm-none-eabi-gcc
    OBJCOPY = $(TOOLCHAIN_PATH)/arm-none-eabi-objcopy
    SIZE = $(TOOLCHAIN_PATH)/arm-none-eabi-size
    
    CCFLAGS = -mcpu=cortex-m0 -mthumb -g -O0 -Wall -I $(STD_INCLUDE_PATH) -I $(PROJ_INCLUDE_PATH)
    LDFLAGS = -mcpu=cortex-m0 -mthumb -nostdlib
    LD = $(PROJ_SOURCE_PATH)/app/app.ld
    
    APP_SRCS = $(PROJ_SOURCE_PATH)/app/app.c

    CORE_SRCS = $(PROJ_SOURCE_PATH)/core/nvic.c \
				$(PROJ_SOURCE_PATH)/core/clock.c \
				$(PROJ_SOURCE_PATH)/core/systick.c \
				$(PROJ_SOURCE_PATH)/core/dispatcher.c

	CONFIG_SRCS = $(PROJ_SOURCE_PATH)/configs/core_setup.c \
				  $(PROJ_SOURCE_PATH)/configs/system_setup.c

    DRIVER_SRCS = $(PROJ_SOURCE_PATH)/drivers/gpio.c \
                  $(PROJ_SOURCE_PATH)/drivers/timer.c \
				  $(PROJ_SOURCE_PATH)/drivers/uart.c 

	DEFINITION_SRCS = $(PROJ_SOURCE_PATH)/definitions/gpio_defs.c \
      	              $(PROJ_SOURCE_PATH)/definitions/timer_defs.c \
				  	  $(PROJ_SOURCE_PATH)/definitions/uart_defs.c

	INFRASTRUCTURE_SRCS = $(PROJ_SOURCE_PATH)/infrastructure/ring_buffer.c \
						  $(PROJ_SOURCE_PATH)/infrastructure/line_framer.c \
						  $(PROJ_SOURCE_PATH)/infrastructure/passthru_framer.c
						
	SERVICE_SRCS = $(PROJ_SOURCE_PATH)/services/uart_console.c

    STARTUP_SRCS = $(PROJ_SOURCE_PATH)/startup/startup.c
    
    FLASH_CMD = $(OPENOCD) -s $(OPENOCD_PATH) -f interface/stlink.cfg -f target/stm32f0x.cfg -c "program $(BUILD_DIR)/app.elf verify reset exit"
endif

# Add more targets here (e.g., avr, esp32, etc.)
# ifeq ($(TARGET),avr)
#     ...
# endif

BUILD_DIR = $(PROJECT_ROOT)/build/$(TARGET)
ELF = $(BUILD_DIR)/app.elf
BIN = $(BUILD_DIR)/app.bin

SRCS = $(APP_SRCS) $(DRIVER_SRCS) $(STARTUP_SRCS) $(CORE_SRCS) $(CONFIG_SRCS) $(DEFINITION_SRCS) $(INFRASTRUCTURE_SRCS) $(SERVICE_SRCS)
OBJS = $(patsubst $(PROJ_SOURCE_PATH)%.c,$(BUILD_DIR)%.o,$(SRCS))

all: $(BIN)
	@echo "Build complete for target: $(TARGET)"
	$(SIZE) $(ELF)

# Universal compile rule
$(BUILD_DIR)/%.o: $(PROJ_SOURCE_PATH)/%.c
	@mkdir -p $(dir $@)
	@echo "Compiling $< ..."
	$(CC) $(CCFLAGS) -c $< -o $@

# Link rule
$(ELF): $(OBJS)
	@mkdir -p $(dir $@)
	@echo "Linking $@ ..."
	$(CC) $(LDFLAGS) -T $(LD) $^ -o $@ -lgcc

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
