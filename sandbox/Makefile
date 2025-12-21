SANDBOX_ROOT = $(CURDIR)
SANDBOX_BUILD = $(SANDBOX_ROOT)/build

CC = gcc
CCFLAGS = -std=c11 -Wall -O0 -g

TARGET ?= line_parser

.PHONY: all
all:
	@if [ ! -d "$(TARGET)" ]; then \
		echo "Unknown target: $(TARGET)"; \
		exit 1; \
	fi
	$(MAKE) -C $(TARGET)


line_parser: line_parser.c line_parser_test.c 
	$(CC) $(CCFLAGS) $^ -o line_parser
