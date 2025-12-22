#include <services/uart_console.h>

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
    console_println_bytes(BYTESPAN(console->cache, console->cached_byte_count));
    console->cached_byte_count = 0; // Reset Cache
}

void console_println_bytes(ByteSpan_t p_data) {
    console_vtable.v_console_print(console_vtable.console, p_data);
}

void console_println_string(string_t str) {
    console_vtable.v_console_print(console_vtable.console, STRING_TO_BYTES(str));
}

void __console_print(Console_t* console, ByteSpan_t p_data) {
    uart_write_byte_array(console->uart, p_data);
}

bool console_rx_callback_newline(void* state) {
    ByteSpan_t* p_data = (ByteSpan_t*)(state);

    uint8_t start = console_vtable.console->cached_byte_count;
    uint8_t stop = start + p_data->count;

    // Latest data coming in will overflow?
    if(stop > BUFFER_LEN_MAX) { 
        return false; 
    }

    uint8_t* cache_bytes = console_vtable.console->cache;

    for (int i = start; i < stop; i++){
        cache_bytes[i] = p_data->bytes[i];
    }

    // Increment count so there's less room in the cache.
    console_vtable.console->cached_byte_count += p_data->count;
    return true;
}

bool console_rx_callback_passthru(void* state) {
    ByteSpan_t* p_data = (ByteSpan_t*)(state);
    // Just a direct write
    ASSERT(p_data->count > 0);
    __console_print(console_vtable.console, *p_data);
    return true;
}

