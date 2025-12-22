#include <core/dispatcher.h> 
#include <core/common.h>
#include <services/uart_console.h>
#include <drivers/uart.h>
#include <infrastructure/line_framer.h>
#include <infrastructure/comms_protocol.h>


void dispatcher_init(Dispatcher_t* dispatcher, 
                     DataRoute_t* routing_table, 
                     void* driver,
                     TryIngestByteFromDriver driver_ingest) {

    dispatcher->routing_table = routing_table;
    dispatcher->driver = driver;
    dispatcher->cached_byte_count = 0;
    dispatcher->driver_ingest = driver_ingest;
}

void bind_data_router(Dispatcher_t* dispatcher, DataRoute_t router) {
    dispatcher->routing_table[dispatcher->rtbl_count] = router;
    (dispatcher->rtbl_count)++;
}

void dispatch_uart(Dispatcher_t* dispatcher) {
    UartDriver_t* uart = (UartDriver_t*)dispatcher->driver;
    // If empty - return that we grabbed zero bytes.
    if(get_uart_rx_buffer_count(uart) == 0) { return; }

    dispatcher->cached_byte_count = 0; //Reset the cache to get the next ring upload.
    TryIngestByteFromDriver data_ingester = dispatcher->driver_ingest;

    // Fresh re-population of cache. The len indicator updates after each write so
    // byte count == length instead of being off by 1
    while(data_ingester(uart, &(dispatcher->cache[dispatcher->cached_byte_count]))) {
        (dispatcher->cached_byte_count)++;
    }


    ServiceCallback_t* service;
    Framer_t* framer;

    // Create a "blank" ByteSpan. Used as a tmp location to write ByteSpan to and dump out.
    ByteSpan_t tmp = {0,0};

    for (int i = 0; i < dispatcher->rtbl_count; i++) {
        service = dispatcher->routing_table[i].service_cb;
        framer = dispatcher->routing_table[i].framer;

        framer->vtbl.ingest(framer->state, (ByteSpan_t){dispatcher->cache, dispatcher->cached_byte_count});

        if(service->cb_enabled){
            while((framer->vtbl.try_to_process_and_write(framer->state, &tmp))) {
                service->try_to_process(&tmp); 
            }
        }
    }
}


