#include <core/dispatcher.h> 
#include <core/common.h>
#include <services/uart_console.h>
#include <drivers/uart.h>
#include <infrastructure/line_framer.h>
#include <infrastructure/comms_protocol.h>


void dispatcher_init(Dispatcher_t* dispatcher, 
                     ByteSpan_t* cache, 
                     DataRoute_t* routing_table, 
                     void* driver,
                     TryIngestByteFromDriver driver_ingest) {

    dispatcher->routing_table = routing_table;
    dispatcher->driver = driver;
    dispatcher->cache = cache;
    dispatcher->driver_ingest = driver_ingest;
}

void bind_data_router(Dispatcher_t* dispatcher, DataRoute_t router) {
    dispatcher->routing_table[dispatcher->rtbl_count] = router;
    (dispatcher->rtbl_count)++;
}

void dispatch_uart(void* state) {
    Dispatcher_t* dispatcher = (Dispatcher_t*)state;
    UartDriver_t* uart = dispatcher->driver;
    // If empty - return that we grabbed zero bytes.
    if(get_uart_rx_buffer_count(uart) == 0) { return; }

    ByteSpan_t* cache = dispatcher->cache;
    TryIngestByteFromDriver data_ingester = dispatcher->driver_ingest;

    while(!data_ingester(uart, &cache->bytes[cache->count])) {
        (cache->count)++;
    }


    ServiceCallback_t scb;
    Framer_t fr;
    uint8_t route_buf[64];
    ByteSpan_t router_cache = {.bytes = route_buf, .count = 0};

    for (int i = 0; i < dispatcher->rtbl_count; i++) {
        scb = dispatcher->routing_table[i].service_cb;
        fr = dispatcher->routing_table[i].framer;
        fr.vtbl.ingest(fr.state, *cache);
        if(scb.cb_enabled){
            while(!(fr.vtbl.try_to_process_and_write(fr.state, &router_cache))) {
                scb.try_to_process(&router_cache); 
            }
        }
    }
}


