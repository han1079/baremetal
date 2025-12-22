#ifndef COMMS_PROTOCOL_H
#define COMMS_PROTOCOL_H

#include <stdint.h>
#include <stdbool.h>
#include <infrastructure/bytes.h>

#define BUFFER_LEN_MAX 64


typedef enum {
    SERVICE_ID_CONSOLE,
    TOTAL_SERVICE_COUNT,
    NULL_SERVICE_ID,
} ServiceTypeID;

typedef enum {
    FRAMER_PASSTHRU,
    FRAMER_NEWLINE,
    TOTAL_FRAMER_COUNT,
} FramerTypeID;

typedef enum {
    UART_SOURCE_ID,
    TOTAL_SOURCE_COUNT,
    NULL_SOURCE_ID,
} EventSourceTypeID;

/* Protocol for Event Dispatch
    Peripherals and input generates data that goes into buffers.

    These buffers get polled, framed, and dispatched to the 
    relevant services that want them.

    The dispatcher a combination of functions and state variables
    that are lumped together to cause input data to go where it's 
    needed.

    STRUCTURE:

    Peripheral "dumps" data:
    Data Source X -> buffer 

    Dispatcher has access to "periph.get". Uses it to dump into a staging area. 
    periph.get(Buffer, &StagingArea) 

    Dispatcher has access to "data_framer[i].update_state"
    for p in all_data_framers:
        p->update_state(&tmp_buffer, p->state)

    Dispatcher has access to "pairs" of "data_framer" and "service_callback"
    while (data_process.try_to_pop(&(state->buffer))) {
        service_callback(&state);
    }

    DECLARATION:

    At this step - empty structs are declared. These are pointers
    to undefined memory of the appropriate size that need to 
    get filled in with pointers to their appropriate data.

    In framer.h :
    #include <comms_protocol.h> <--- gets the definition
    extern const Framerfcns_t framer_x_fcns;

    In system_init.h
    #include <framer.h>         <-- gets framer specific typedefs
    FramerXState framer_x_state;
    Framer_t framer_x;

    DEFINITION:

    Any "predefinable" stuff should happen now.

    In framer.c :
    const FramerFcns_t framer_x_fcns = {fcn1, fcn2}; <-- framer.c should define its ACCESS SURFACE

    ^^ We only ever need to do this once. This creates the ROUTING TABLE.
    
    INITIALIZATION (RUNTIME ASSIGNMENT / BINDING):

    This is where the functions, its state, and the container get combined:
    
    Defined in framer.c 

    void framer_init(FramerXState* p_s, FramerFcns_t* p_f, Framer_t* p_obj);

    RUN in system_init.c
    framer_init(&framer_x_state, &framer_x_fcns, &framer_x); <--- the ONLY "framer-native" funtion run outside of a vtable
*/

typedef void (*DoIngest)(void* state, ByteSpan_t data);
typedef void (*DoProcess)(void* state);
typedef bool (*TryIngest)(void* state, ByteSpan_t data);
typedef bool (*TryProcess)(void* state);

typedef void (*DoBind)(void* container, void* items);
typedef bool (*TryIngestByteFromDriver)(void* driver, uint8_t* dest);
typedef bool (*TryProcessAndWrite)(void* state, ByteSpan_t* p_data);

typedef struct {
    DoIngest ingest;
    TryProcessAndWrite try_to_process_and_write;
} FramerFcns_t;

typedef struct {
    FramerFcns_t vtbl;
    void* state;
} Framer_t;

typedef struct {
    TryProcess try_to_process;
    bool cb_enabled;
} ServiceCallback_t;

typedef struct {
    ServiceCallback_t service_cb;
    Framer_t framer;
} DataRoute_t;

typedef struct {
    DataRoute_t* routing_table;
    uint8_t rtbl_count;
    TryIngestByteFromDriver driver_ingest;
    ByteSpan_t* cache;
    void* driver;
} Dispatcher_t;



#endif //COMMS_PROTOCOL_H
