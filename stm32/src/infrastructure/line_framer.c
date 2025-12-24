#include <infrastructure/line_framer.h>


void line_framer_init(Framer_t* framer, FramerFcns_t* vtable, void* state){
    LineFramerState_t* line = (LineFramerState_t*)state;
    line->accum_idx = 0;

    line->new_data.bytes = (uint8_t*)0;
    line->new_data.count = 0;

    line->new_data_idx = 0;
    line->overflow = false;
    line->output.bytes = line->accum_buffer;
    line->output.count = 0;

    framer->state = line;

    vtable->ingest = line_framer_ingest;
    vtable->try_to_process_and_write = line_framer_try_to_process_and_write;

    framer->vtbl = *vtable;
}

void line_framer_ingest(void* state, ByteSpan_t data) {
    // This function just "loads" the data in. No actual appending or processing. Keeps OLD state persistent.
    LineFramerState_t* line = (LineFramerState_t*)state;
    
    // Since ByteSpan holds only a pointer and a length - 
    // we can pass it a pointer and a length, and it references
    // data that's populated elsewhere. This is the CACHE of the dispatcher usually.
    line->new_data = data;
    
    line->new_data_idx = 0;
    line->output.count = 0;
}

bool line_framer_try_to_process_and_write(void* state, ByteSpan_t* p_data) {
    LineFramerState_t* line = (LineFramerState_t*)state;
    // Mangled data length - don't even attempt.
    if (line->new_data.count == 0) { return false; }

    // Run out of new data, but found newline last attempt. Stops collecting.
    if (line->new_data_idx == line->new_data.count) { return false; }

    line->overflow = false; // Reset overflow flag

    while (line->new_data_idx < line->new_data.count) {
        // Index logic happens at the end of the loop, so this is safe.
        line->accum_buffer[line->accum_idx] = line->new_data.bytes[line->new_data_idx];

        if (line->accum_idx + 1 >= BUFFER_LEN_MAX ) {

            p_data->count = BUFFER_LEN_MAX;
            line->accum_idx = 0;
            
            if (line->new_data_idx + 1 < line->new_data.count) {
                line->overflow = true;
            } 

            line->new_data_idx++;

            break;
        
        } else if ((line->accum_buffer[line->accum_idx] == '\r') ||
                    (line->accum_buffer[line->accum_idx] == '\n')) {
        
            // Found newline. Reset accumulator to "pop" data.
            p_data->count = line->accum_idx + 1;
            line->accum_idx = 0;
            line->new_data_idx++;
            break;

        } else {

            line->accum_idx++;
            line->new_data_idx++;
        
        }
    }

    if (line->accum_idx == 0) {
        // p_data output holder - just write it to the "output" address.
        p_data->bytes = &(line->accum_buffer[0]);
        return true;
    } else {
        // Reset p_data to blank.
        p_data->bytes = (uint8_t*)0;
        p_data->count = 0;
        return false;
    }
}
