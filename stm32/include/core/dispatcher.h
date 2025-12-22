#ifndef DISPATCHER_H
#define DISPATCHER_H

#include <core/common.h>
#include <drivers/uart.h>
#include <infrastructure/comms_protocol.h>

void dispatcher_init(Dispatcher_t* dispatcher, 
                     ByteSpan_t* cache, 
                     DataRoute_t* routing_table, 
                     void* driver,
                     TryIngestByteFromDriver driver_ingest);

void bind_data_router(Dispatcher_t* dispatcher, DataRoute_t router);
void dispatch_uart(void* state);

#endif //DISPATCHER_H
