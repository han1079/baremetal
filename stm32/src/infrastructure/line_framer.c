#include <infrastructure/line_framer.h>


void line_framer_init(Framer_t* framer, FramerFcns_t* vtable, void* state){
    LineFramerState_t* line = (LineFramerState_t*)state;
    line->accum_idx = 0;
    line->new_data = 0;
    line->new_data_len = 0;
    line->new_data_idx = 0;
    line->overflow = false;
    line->output.bytes = line->accum_buffer;
    line->output.count = 0;

    framer->state = line;

    vtable->ingest = line_framer_ingest;
    vtable->try_to_process_and_write = line_framer_try_to_process_and_write;
}

void line_framer_ingest(void* state, ByteSpan_t data) {
    // This function just "loads" the data in. No actual appending or processing. Keeps OLD state persistent.
    LineFramerState_t* line = (LineFramerState_t*)state;
    line->new_data = data.bytes;
    line->new_data_len = data.count;
    line->new_data_idx = 0;
    line->output.count = 0;
}

bool line_framer_try_to_process_and_write(void* state, ByteSpan_t* p_data) {
    LineFramerState_t* line = (LineFramerState_t*)state;
    // Mangled data length - don't even attempt.
    if (line->new_data_len == 0) { return false; }

    // Run out of new data, but found newline last attempt. Stops collecting.
    if (line->new_data_idx == line->new_data_len) { return false; }

    line->overflow = false; // Reset overflow flag

    while (line->new_data_idx < line->new_data_len) {
        // Index logic happens at the end of the loop, so this is safe.
        line->accum_buffer[line->accum_idx] = line->new_data[line->new_data_idx];

        if (line->accum_idx + 1 >= BUFFER_LEN_MAX ) {

            line->output.count = BUFFER_LEN_MAX;
            line->accum_idx = 0;
            
            if (line->new_data_idx + 1 < line->new_data_len) {
                line->overflow = true;
            } 

            line->new_data_idx++;

            break;
        
        } else if (line->accum_buffer[line->accum_idx] == '\n') {
        
            // Found newline. Reset accumulator to "pop" data.
            line->output.count = line->accum_idx + 1;
            line->accum_idx = 0;
            line->new_data_idx++;
            break;

        } else {

            line->accum_idx++;
            line->new_data_idx++;
        
        }
    }

    if (line->accum_idx == 0) {
        p_data = &(line->output);
        return true;
    } else {
        return false;
    }
}
