#include "infrastructure/bytes.h"
#include <infrastructure/passthrough_framer.h>
#include <infrastructure/line_framer.h>
#include <infrastructure/data_frame_protocol.h>

void bind_passthru_framer(void* wc1, void* wc2){
    DataFramer_t* df = (DataFramer_t*)wc1;
    PassthruFramerState_t* pass = (PassthruFramerState_t*) wc2;
    df->state = pass;
    df->ingest_new_data = passthru_stage_raw_data;
    df->process_state = passthru_try_to_frame_data;
}

void bind_line_framer(void* wc1, void* wc2) {
    DataFramer_t* df = (DataFramer_t*)wc1;
    LineFramerState_t* line = (LineFramerState_t*)wc2;
    line->accum_idx = 0;
    line->new_data = 0;
    line->new_data_len = 0;
    line->new_data_idx = 0;
    line->overflow = false;
    line->line_length = 0;

    df->state = line;
    df->ingest_new_data = line_framer_stage_raw_data;
    df->process_state = line_framer_try_to_frame_data;
}

ByteSpan_t passthru_get_staged_bytes(void* staging_area) {
    PassthruFramerState_t* pass = (PassthruFramerState_t*) staging_area;
    return pass->bytes_with_count;
}

ByteSpan_t line_framer_get_staged_bytes(void *staging_area) {
    LineFramerState_t* line = (LineFramerState_t*)staging_area;
    return (ByteSpan_t){.bytes = line->accum_buffer, .count = line->accum_idx};
}

void passthru_stage_raw_data(void* staging_area, ByteSpan_t p_data) {
    PassthruFramerState_t* pass = (PassthruFramerState_t*)staging_area;
    pass->bytes_with_count = p_data;
}

ByteSpan_Failable_t passthru_try_to_frame_data(void* staging_area) {
    // Just as passthru
    return (ByteSpan_Failable_t){0,0,true};
}

void line_framer_stage_raw_data(void* staging_area, ByteSpan_t p_data) {
    // This function just "loads" the data in. No actual appending or processing. Keeps OLD state persistent.
    LineFramerState_t* line = (LineFramerState_t*)staging_area;
    line->new_data = p_data.bytes;
    line->new_data_len = p_data.count;
    line->new_data_idx = 0;
}

ByteSpan_Failable_t line_framer_try_to_frame_data(void* staging_area) {
    LineFramerState_t* line = (LineFramerState_t*)staging_area;
    // Mangled data length - don't even attempt.
    if (line->new_data_len == 0) { return (ByteSpan_Failable_t){0,0,false}; }

    // Run out of new data, but found newline last attempt. Stops collecting.
    if (line->new_data_idx == line->new_data_len) { return (ByteSpan_Failable_t){0,0,false}; }

    line->overflow = false; // Reset overflow flag

    while (line->new_data_idx < line->new_data_len) {
        // Index logic happens at the end of the loop, so this is safe.
        line->accum_buffer[line->accum_idx] = line->new_data[line->new_data_idx];

        if (line->accum_idx + 1 >= LINE_LEN_MAX ) {

            line->line_length = LINE_LEN_MAX;
            line->accum_idx = 0;
            
            if (line->new_data_idx + 1 < line->new_data_len) {
                line->overflow = true;
            } 

            line->new_data_idx++;

            break;
        
        } else if (line->accum_buffer[line->accum_idx] == '\n') {
        
            // Found newline. Reset accumulator to "pop" data.
            line->line_length = line->accum_idx + 1;
            line->accum_idx = 0;
            line->new_data_idx++;
            break;

        } else {

            line->accum_idx++;
            line->new_data_idx++;
        
        }
    }

    if (line->accum_idx == 0) {
        return (ByteSpan_Failable_t){(uint8_t*)line->accum_buffer, line->accum_idx, true};
    } else {
        return (ByteSpan_Failable_t){0,0,false};
    }
}
