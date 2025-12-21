#ifndef UART_CONSOLE_H
#define UART_CONSOLE_H

#include <core/common.h>
#include <infrastructure/data_frame_protocol.h>
#include <infrastructure/string.h>
#include <drivers/uart.h>

static UartDriver_t* console_uart;
static const string_t prefix = CSTRING(">> ");

static ByteSpan_t buffer_cache;
static uint8_t buffer_cache_data[64];

void console_rx_callback_newline(ByteSpan_Failable_t p_data);
void console_rx_callback_passthru(ByteSpan_Failable_t p_data);

void console_init(UartDriver_t* uart);
void console_on_update();
void console_print(ByteSpan_t p_data);
void console_println(ByteSpan_t p_data);
void __console_print_prefix();

void console_echo_newline();

static inline void console_print_newline() {
    uart_write_byte_array(console_uart, BYTES_LIT_NO_TERM("\n"));
}

static inline void console_print_string(string_t str) { 
    uart_write_byte_array(console_uart, STRING_TO_BYTES(str)); 
}


#endif //UART_CONSOLE_H
