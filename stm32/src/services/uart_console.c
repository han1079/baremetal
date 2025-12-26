#include "definitions/gpio_defs.h"
#include <services/uart_console.h>

const ByteSpan_t CRLF = {(uint8_t[2]){'\r', '\n'}, 2};
uint8_t a[3] = {'>', '>', ' '};
const ByteSpan_t CONSOLE_PREFIX = {(uint8_t*)&a[0], 3};

ConsoleVTable_t console_vtable = {
    .console = (Console_t*)0,
    .v_console_print = __console_stage_tx_bytes,
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
        uart_transact_start(console->uart);
        console_echo_line();
        console_write_prefix();
        uart_transact_end(console->uart);
    }
    console->cached_byte_count = 0; // Reset Cache
}

void console_write_prefix() {
    Console_t* console = console_vtable.console;
    __console_stage_tx_bytes(console, CONSOLE_PREFIX);
}

void console_echo_line() {
    Console_t* console = console_vtable.console;
    uint8_t line[128];
    uint8_t cache_idx = 0;
    uint8_t line_idx = 0;
    uart_transact_start(console->uart);
    while(cache_idx < console->cached_byte_count) {
        if (console->cache[cache_idx] == '\r') {
            line[line_idx] = '\r';
            line[line_idx + 1] = '\n';
            __console_stage_tx_bytes(console, BYTESPAN(line, (line_idx+2)));
            line_idx = 0;
        } else {
            line[line_idx] = console->cache[cache_idx];
            line_idx++;
        }
        cache_idx++;
    }
    uart_transact_end(console->uart);
}


void __console_stage_tx_bytes(Console_t* console, ByteSpan_t p_data) {
    uart_stage_bytes_for_tx(console->uart, p_data);
}

void __console_stage_one_byte(Console_t* console, uint8_t byte) {
    __uart_push_byte_to_tx_rb((void*)(console->uart), byte);
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
    if(p_data->count == 0) { return false; }
    uint8_t b = 0;
    Console_t* console = console_vtable.console;
    uart_transact_start(console->uart);
    for (int i = 0; i < p_data->count; i++) {
        b = p_data->bytes[i];
        if(b == '\r' || b == '\n') {
            // Directly write full carriage return
            __console_stage_one_byte(console, '\r');
            __console_stage_one_byte(console, '\n');
        } else {
            __console_stage_one_byte(console, b);
        }
    }
    uart_transact_end(console->uart);
    return true;
}

