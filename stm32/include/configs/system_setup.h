#ifndef SYSTEM_SETUP_H
#define SYSTEM_SETUP_H

#include <infrastructure/data_frame_protocol.h> 
#include <infrastructure/line_framer.h> 
#include <infrastructure/passthrough_framer.h> 

#include <core/dispatcher.h> 
#include <core/common.h>

#include <services/uart_console.h>

// Framer State Variable Declaration 
extern LineFramerState_t* p_line_framer_state;
extern PassthruFramerState_t* p_passthru_framer_state;

// Data Framer Objects
extern DataFramer_t* p_line_framer;
extern DataFramer_t* p_passthru_framer;

// These need to get bound to function handles first
extern TwoWildcardIn_VoidOut_VTableEntry_t frame_binding_vtable[TOTAL_FRAMER_COUNT];

// This needs to get populated after proper initialization
extern DataFramer_t* data_framers[TOTAL_FRAMER_COUNT];

// Routing table function handles need to get bound initially
extern BytesInFailable_VoidOut_VTableEntry_t frame_vtable[TOTAL_FRAMER_COUNT*TOTAL_SERVICE_COUNT];

void system_setup();

void __populate_vtables();
void __setup_data_framers();

#endif //SYSTEM_SETUP_H
