#ifndef PASSTHRU_FRAMER_H
#define PASSTHRU_FRAMER_H

#include <infrastructure/comms_protocol.h>

typedef struct {
    ByteSpan_t bytes; 
    bool ingested;
} PassthruFramerState_t;

void passthru_framer_init(Framer_t* framer, FramerFcns_t* vtable, void* state);

void passthru_framer_ingest(void* state, ByteSpan_t data);
bool passthru_framer_try_to_process_and_write(void* state, ByteSpan_t* p_data);

#endif //PASSTHRU_FRAMER_H
