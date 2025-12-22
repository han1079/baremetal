#include <services/uart_console.h>

ConsoleVTable_t console_vtable = {
    .console = 0,
    .v_console_print = __console_print,
};

void console_init(Console_t* console, UartDriver_t* uart, ByteSpan_t* cache) {
    console_vtable.console = console;
    console->uart = uart;
    console->cache = cache;
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
    router.framer = *framer;
    router.service_cb = *scb;
    return router;
}

bool change_driver(UartDriver_t* uart, ByteSpan_t* cache) {
    if (uart == 0 || cache || 0) { return false; }
    if (uart != 0) {
        console_vtable.console->cache = cache;
    }

    if (cache != 0) {
        console_vtable.console->uart = uart;
    }
    return true;
}

void console_on_update() {
    Console_t* console = console_vtable.console;
    console_println_bytes(*(console->cache));
    console->cache->count = 0; // Reset Cache
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

    console_vtable.console->cache->count = p_data->count;
    uint8_t* cache_bytes = console_vtable.console->cache->bytes;

    for (int i = 0; i < p_data->count; i++){
        cache_bytes[i] = p_data->bytes[i];
    }
    return true;
}

bool console_rx_callback_passthru(void* state) {
    ByteSpan_t* p_data = (ByteSpan_t*)(state);
    console_vtable.console->cache->count = p_data->count;
    ASSERT(p_data->count > 0);
    __console_print(console_vtable.console, *p_data);
    return true;
}

