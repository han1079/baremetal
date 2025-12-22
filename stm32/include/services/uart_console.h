#ifndef UART_CONSOLE_H
#define UART_CONSOLE_H

#include <core/common.h>
#include <infrastructure/comms_protocol.h>
#include <infrastructure/string.h>
#include <drivers/uart.h>

static const string_t prefix = CSTRING(">> ");


typedef struct {
    UartDriver_t* uart;
    ByteSpan_t* cache;
} Console_t;

typedef void (*DoConsoleIngest)(Console_t* console, ByteSpan_t p_data);
typedef void (*DoConsoleProcess)(Console_t* console);

typedef struct {
    Console_t* console;
    DoConsoleIngest v_console_print;
} ConsoleVTable_t;

extern ConsoleVTable_t console_vtable;

void console_init(Console_t* console, UartDriver_t* uart, ByteSpan_t* cache);
DataRoute_t bind_console(ServiceCallback_t* scb, Framer_t* framer, FramerTypeID id);
bool change_driver(UartDriver_t* uart, ByteSpan_t* cache);
void console_on_update();
void console_println_bytes(ByteSpan_t p_data);
void console_println_string(string_t str);
bool console_rx_callback_newline(void* state);
bool console_rx_callback_passthru(void* state);

void __console_print(Console_t* console, ByteSpan_t p_data);


#endif //UART_CONSOLE_H
