#ifndef LINE_FRAMER_H
#define LINE_FRAMER_H
#include <stdint.h>
#include <stdbool.h>
#include <infrastructure/comms_protocol.h>


typedef struct {
    uint8_t accum_buffer[BUFFER_LEN_MAX];
    uint8_t accum_idx;

    uint8_t* new_data;
    uint8_t new_data_len;
    uint8_t new_data_idx;
    
    bool overflow;

    ByteSpan_t output;
} LineFramerState_t;


void line_framer_init(Framer_t* framer, FramerFcns_t* vtable, void* state);

void line_framer_ingest(void* state, ByteSpan_t data);
bool line_framer_try_to_process_and_write(void* state, ByteSpan_t* p_data);

#endif //LINE_FRAMER_H
