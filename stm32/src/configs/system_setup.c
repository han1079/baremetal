#include "infrastructure/data_frame_protocol.h"
#include <configs/system_setup.h> 

// Framer State Variable Declaration 
LineFramerState_t* p_line_framer_state;
PassthruFramerState_t* p_passthru_framer_state;

// Data Framer Objects
DataFramer_t* p_line_framer;
DataFramer_t* p_passthru_framer;

// These need to get bound to function handles first
TwoWildcardIn_VoidOut_VTableEntry_t frame_binding_vtable[TOTAL_FRAMER_COUNT];

// This needs to get populated after proper initialization
DataFramer_t* data_framers[TOTAL_FRAMER_COUNT];

// Routing table function handles need to get bound initially
BytesInFailable_VoidOut_VTableEntry_t frame_vtable[TOTAL_FRAMER_COUNT*TOTAL_SERVICE_COUNT];

void system_setup() {
    __populate_vtables();
    __setup_data_framers();
}

void __populate_vtables() {
    
    frame_vtable[GET_FLAT_IDX(FRAMER_PASSTHRU, SERVICE_ID_CONSOLE, TOTAL_SERVICE_COUNT)].callback = console_rx_callback_newline;
    frame_vtable[GET_FLAT_IDX(FRAMER_PASSTHRU, SERVICE_ID_CONSOLE, TOTAL_SERVICE_COUNT)].enabled = true;

    frame_vtable[GET_FLAT_IDX(FRAMER_NEWLINE, SERVICE_ID_CONSOLE, TOTAL_SERVICE_COUNT)].callback = console_rx_callback_passthru;
    frame_vtable[GET_FLAT_IDX(FRAMER_NEWLINE, SERVICE_ID_CONSOLE, TOTAL_SERVICE_COUNT)].enabled = true;

    frame_binding_vtable[FRAMER_PASSTHRU].bind = bind_passthru_framer;
    frame_binding_vtable[FRAMER_PASSTHRU].object1 = (void*)(p_passthru_framer);
    frame_binding_vtable[FRAMER_PASSTHRU].object2 = (void*)(p_passthru_framer_state);

    frame_binding_vtable[FRAMER_NEWLINE].bind = bind_line_framer;
    frame_binding_vtable[FRAMER_NEWLINE].object1 = (void*)(p_line_framer);
    frame_binding_vtable[FRAMER_NEWLINE].object2 = (void*)(p_line_framer_state);
}

void __setup_data_framers() {
    data_framers[FRAMER_PASSTHRU] = p_passthru_framer;
    data_framers[FRAMER_NEWLINE] = p_line_framer;

    for (int i = 0; i < TOTAL_FRAMER_COUNT; i++) {
        frame_binding_vtable[i].bind(frame_binding_vtable[i].object1,
                                     frame_binding_vtable[i].object2);
    }
}

