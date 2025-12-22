
#include <infrastructure/passthru_framer.h>


void passthru_framer_init(Framer_t* framer, FramerFcns_t* vtable, void* state) {
    PassthruFramerState_t* pass = (PassthruFramerState_t*) state;
    pass->ingested = 0;

    framer->state = pass;

    vtable->ingest = passthru_framer_ingest;
    vtable->try_to_process_and_write = passthru_framer_try_to_process_and_write;
}

void passthru_framer_ingest(void* state, ByteSpan_t data) {
    PassthruFramerState_t* pass = (PassthruFramerState_t*)state;
    pass->bytes = data;
    pass->ingested = true;
}

bool passthru_framer_try_to_process_and_write(void* state, ByteSpan_t* p_data) {
    PassthruFramerState_t* pass = (PassthruFramerState_t*)state;
    pass->ingested = false;
    p_data = &(pass->bytes);
    return true;
}

