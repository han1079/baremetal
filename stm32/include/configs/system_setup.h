#ifndef SYSTEM_SETUP_H
#define SYSTEM_SETUP_H

#include <infrastructure/comms_protocol.h> 
#include <infrastructure/line_framer.h> 
#include <infrastructure/passthru_framer.h> 

#include <core/dispatcher.h> 
#include <core/common.h>

#include <services/uart_console.h>

extern Dispatcher_t uart1_dispatcher;
extern DataRoute_t uart1_routing_table[TOTAL_FRAMER_COUNT * TOTAL_SERVICE_COUNT];

extern FramerFcns_t LineFramerFcns;
extern FramerFcns_t PassthruFramersFcns;

extern Framer_t LineFramer;
extern Framer_t PassthruFramer;

extern ServiceCallback_t ConsoleLineCallback;
extern ServiceCallback_t ConsolePassthruCallback;

extern LineFramerState_t LineFramerState;
extern PassthruFramerState_t PassthruFramerState;

extern Console_t uart1_console;
extern ByteSpan_t uart1_console_cache;
extern uint8_t uart1_console_buffer[BUFFER_LEN_MAX];

void system_setup();
void __setup_framers();
void __setup_services();

#endif //SYSTEM_SETUP_H
