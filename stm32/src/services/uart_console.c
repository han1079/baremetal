#include <drivers/uart.h>
#include <services/uart_console.h>


void console_init(UartDriver_t *uart) {
    console_uart = uart;
    buffer_cache.count = 0;
    buffer_cache.bytes = buffer_cache_data;
    __console_print_prefix();
}

void console_on_update() {
    console_echo_newline();
}

void console_print(ByteSpan_t p_data) {
    uart_write_byte_array(console_uart, p_data);
}

void console_println(ByteSpan_t p_data) {
    console_print(p_data);
    console_print_newline();
}

void __console_print_prefix() {
    console_print_string(prefix);
}

void console_rx_callback_newline(ByteSpan_Failable_t p_data) {
    buffer_cache.count = p_data.count;
    for (int i = 0; i < p_data.count; i++){
        buffer_cache.bytes[i] = p_data.bytes[i];
    }
}

void console_rx_callback_passthru(ByteSpan_Failable_t p_data) {
    ASSERT(p_data.count > 0);
    if (p_data.bytes[(p_data.count-1)] == '\n') { return; }
    console_print((ByteSpan_t){p_data.bytes, p_data.count});
}

void console_echo_newline() {
    if (buffer_cache.count != 0){
        console_print_newline();
        console_print(buffer_cache);
    }
    __console_print_prefix();
}
