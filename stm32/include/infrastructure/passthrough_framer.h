#ifndef PASSTHRU_FRAMER_H
#define PASSTHRU_FRAMER_H
#include "infrastructure/bytes.h"
#define LINE_LEN_MAX 64

#include <infrastructure/data_frame_protocol.h>
typedef struct {
    ByteSpan_t bytes_with_count; 
} PassthruFramerState_t;

void bind_passthru_framer(void* wc1, void* wc2);
ByteSpan_t passthru_get_staged_bytes(void* state);
void passthru_stage_raw_data(void* state, ByteSpan_t p_data);
ByteSpan_Failable_t passthru_try_to_frame_data(void* state);
#endif //PASSTHRU_FRAMER_H
