#ifndef LINE_FRAMER_H
#define LINE_FRAMER_H
#include "infrastructure/bytes.h"
#include <stdint.h>
#include <stdbool.h>
#include <infrastructure/data_frame_protocol.h>

#define LINE_LEN_MAX 64

typedef struct {
    uint8_t accum_buffer[LINE_LEN_MAX];
    uint8_t accum_idx;

    uint8_t* new_data;
    uint8_t new_data_len;
    uint8_t new_data_idx;
    
    bool overflow;
    uint8_t line_length;
} LineFramerState_t;


void bind_line_framer(void* wc1, void* wc2);
ByteSpan_t line_framer_get_staged_bytes(void* staging_area);
void line_framer_stage_raw_data(void* state, ByteSpan_t p_data);
ByteSpan_Failable_t line_framer_try_to_frame_data(void* state);

#endif //LINE_FRAMER_H
