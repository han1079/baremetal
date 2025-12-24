#include "definitions/gpio_defs.h"
#include <services/uart_console.h>

const ByteSpan_t CRLF = {(uint8_t[2]){'\r', '\n'}, 2};
uint8_t a[3] = {'>', '>', ' '};
const ByteSpan_t CONSOLE_PREFIX = {(uint8_t*)&a[0], 3};

ConsoleVTable_t console_vtable = {
    .console = (Console_t*)0,
    .v_console_print = __console_print,
};

void console_init(Console_t* console, UartDriver_t* uart) {
    console_vtable.console = console;
    console->uart = uart;
}

DataRoute_t bind_console(ServiceCallback_t* scb, Framer_t* framer, FramerTypeID id) {
    DataRoute_t router;
    if (id == FRAMER_PASSTHRU) {
        scb->try_to_process = console_rx_callback_passthru;
        scb->cb_enabled = true;
    } else if (id == FRAMER_NEWLINE) {
        scb->try_to_process = console_rx_callback_newline;
        scb->cb_enabled = true;
    } else {
        scb->try_to_process = (TryProcess)0;
        scb->cb_enabled = false;
    }
    router.framer = framer;
    router.service_cb = scb;
    return router;
}

bool change_driver(UartDriver_t* uart) {
    if (uart == 0) { return false; }

    console_vtable.console->uart = uart;

    return true;
}

void console_on_update() {
    Console_t* console = console_vtable.console;
    if (console->cached_byte_count > 0) {
        console_echo_line();
        console_write_prefix();
        uart_kick_off_tx(console->uart);
    }
}

void console_write_prefix() {
    Console_t* console = console_vtable.console;
    __console_print(console, CONSOLE_PREFIX);
}

void console_echo_line() {
    Console_t* console = console_vtable.console;
    uint8_t line[128];
    uint8_t cache_idx = 0;
    uint8_t line_idx = 0;
    while(cache_idx < console->cached_byte_count) {
        if (console->cache[cache_idx] == '\r') {
            line[line_idx] = '\r';
            line[line_idx + 1] = '\n';
            __console_print(console, BYTESPAN(line, (line_idx+2)));
            line_idx = 0;
        } else {
            line[line_idx] = console->cache[cache_idx];
            line_idx++;
        }
        cache_idx++;
    }
    console->cached_byte_count = 0; // Reset Cache
}


void __console_print(Console_t* console, ByteSpan_t p_data) {
    uart_write_byte_array(console->uart, p_data);
}

bool console_rx_callback_newline(void* state) {
    ByteSpan_t* p_data = (ByteSpan_t*)(state);

    uint8_t start = console_vtable.console->cached_byte_count;
    uint8_t stop = (uint16_t)(start + p_data->count);

    // Latest data coming in will overflow?
    if(stop > BUFFER_LEN_MAX) { 
        return false; 
    }

    uint8_t* cache_bytes = console_vtable.console->cache;

    for (int i = 0; i < p_data->count; i++){
        cache_bytes[i + start] = p_data->bytes[i];
    }

    // Increment count so there's less room in the cache.
    console_vtable.console->cached_byte_count += p_data->count;
    return true;
}

bool console_rx_callback_passthru(void* state) {
    ByteSpan_t* p_data = (ByteSpan_t*)(state);
    // Just a direct write
    ASSERT(p_data->count > 0);
    if(p_data->bytes[0] != '\r') {
        __console_print(console_vtable.console, *p_data);
    } else {
        __console_print(console_vtable.console, CRLF);
    }
    uart_kick_off_tx(console_vtable.console->uart);
    return true;
}

