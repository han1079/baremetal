#ifndef DATA_FRAME_PROTOCOL_H
#define DATA_FRAME_PROTOCOL_H

#include <stdint.h>
#include <stdbool.h>
#include <infrastructure/bytes.h>

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

typedef void (*TwoWildcardIn_VoidOut)(void* wc1, void* wc2);
typedef void (*BytesIn_VoidOut)(ByteSpan_t p_data);
typedef void (*BytesInFailable_VoidOut)(ByteSpan_Failable_t p_data);
typedef void (*WildcardInBytesIn_VoidOut)(void* state, ByteSpan_t p_data);
typedef ByteSpan_Failable_t (*WildcardIn_BytesOut_Failable)(void* state);

typedef struct {
    void* state;
    WildcardInBytesIn_VoidOut ingest_new_data;
    WildcardIn_BytesOut_Failable process_state;
    uint8_t service_routing_table[TOTAL_SERVICE_COUNT];
} DataFramer_t;

typedef struct {
    BytesInFailable_VoidOut callback;
    bool enabled;
} BytesInFailable_VoidOut_VTableEntry_t;
#define FrameCallback BytesIn_VoidOut_VTableEntry_t
#define FrameCallbackFailable BytesInFailable_VoidOut_VTableEntry_t

typedef struct {
    TwoWildcardIn_VoidOut bind;
    void* object1;
    void* object2;
} TwoWildcardIn_VoidOut_VTableEntry_t;


#endif //DATA_FRAME_PROTOCOL_H
