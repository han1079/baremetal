#ifndef UART_CONSOLE_H
#define UART_CONSOLE_H

#include <core/common.h>
#include <infrastructure/comms_protocol.h>
#include <infrastructure/string.h>
#include <drivers/uart.h>

static const string_t prefix = CSTRING(">> ");


typedef struct {
    UartDriver_t* uart;
    uint8_t cache[BUFFER_LEN_MAX];
    uint8_t cached_byte_count;
} Console_t;

typedef void (*DoConsoleIngest)(Console_t* console, ByteSpan_t p_data);
typedef void (*DoConsoleProcess)(Console_t* console);

typedef struct {
    Console_t* console;
    DoConsoleIngest v_console_print;
} ConsoleVTable_t;

extern ConsoleVTable_t console_vtable;

void console_init(Console_t* console, UartDriver_t* uart);
DataRoute_t bind_console(ServiceCallback_t* scb, Framer_t* framer, FramerTypeID id);
bool change_driver(UartDriver_t* uart);
void console_on_update();
bool console_rx_callback_newline(void* state);
bool console_rx_callback_passthru(void* state);

void console_write_prefix();
void console_echo_line();

void __console_stage_tx_bytes(Console_t* console, ByteSpan_t p_data);
void __console_stage_one_byte(Console_t* console, uint8_t byte);


#endif //UART_CONSOLE_H
