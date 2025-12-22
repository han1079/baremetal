#include "infrastructure/comms_protocol.h"
#include "infrastructure/passthru_framer.h"
#include "services/uart_console.h"
#include <configs/system_setup.h> 

Dispatcher_t uart1_dispatcher;
DataRoute_t uart1_routing_table[TOTAL_FRAMER_COUNT * TOTAL_SERVICE_COUNT];

FramerFcns_t LineFramerFcns = {
    .ingest = line_framer_ingest,
    .try_to_process_and_write = line_framer_try_to_process_and_write,
};

FramerFcns_t PassthruFramersFcns = {
.ingest = passthru_framer_ingest,
    .try_to_process_and_write = passthru_framer_try_to_process_and_write,
};

Framer_t LineFramer;
Framer_t PassthruFramer;

ServiceCallback_t ConsoleLineCallback = {
    .try_to_process = console_rx_callback_newline,
    .cb_enabled = false,
};

ServiceCallback_t ConsolePassthruCallback = {
    .try_to_process = console_rx_callback_passthru,
    .cb_enabled = false,
};

LineFramerState_t LineFramerState;
PassthruFramerState_t PassthruFramerState;

Console_t uart1_console;

void system_setup() {
    __setup_framers();
    __setup_services();
}

void __setup_framers() {
    line_framer_init(&LineFramer, &LineFramerFcns, (void*)&(LineFramerState));
    passthru_framer_init(&PassthruFramer, &PassthruFramersFcns, (void*)&(PassthruFramerState));
}

void __setup_services() {
    dispatcher_init(&uart1_dispatcher, 
                    &uart1_routing_table[0], 
                    (void*)&UART1, 
                    uart_get_rx_buffer_next_byte);

    console_init(&uart1_console, &UART1);   

    DataRoute_t dr;
    dr = bind_console(&ConsolePassthruCallback, &PassthruFramer, FRAMER_PASSTHRU);
    bind_data_router(&uart1_dispatcher, dr);
    dr = bind_console(&ConsoleLineCallback, &LineFramer, FRAMER_NEWLINE);
    bind_data_router(&uart1_dispatcher, dr);
}
