#include <core/dispatcher.h> 
#include <core/common.h>
#include <services/uart_console.h>
#include <drivers/uart.h>
#include <infrastructure/line_framer.h>
#include <infrastructure/data_frame_protocol.h>



void dispatch_uart(UartDriver_t* uart, DataFramer_t** framers, FrameCallbackFailable* routing_table) {
    // If empty - return that we grabbed zero bytes.
    if(get_uart_rx_buffer_count(uart) == 0) { return; }
    uint8_t tmp_buffer[64];
    uint8_t used_buffer_len = 0;

    while(!uart_get_rx_buffer_next_byte(uart, &tmp_buffer[used_buffer_len])) {
        used_buffer_len++;
    }

    ByteSpan_t tmp_bytes = {.bytes = tmp_buffer, .count = used_buffer_len};

    void* tmp_stage;
    for (int i = 0; i < TOTAL_FRAMER_COUNT; i++) {
        tmp_stage = (void*)framers[i]->state;
        framers[i]->ingest_new_data(tmp_stage, tmp_bytes); 
    }

    ByteSpan_Failable_t tmp_frame;
    bool buffer_notempty = true;
    for (int sid = 0; sid < TOTAL_SERVICE_COUNT; sid++) {
        for (int fid = 0; fid < TOTAL_FRAMER_COUNT; fid++) {
            tmp_stage = (void*)framers[fid]->state;
            if (framers[fid]->service_routing_table[sid] == 1) {
                while(buffer_notempty){
                    tmp_frame = framers[fid]->process_state(tmp_stage);
                    buffer_notempty = tmp_frame.result;
                    if (buffer_notempty) {
                        routing_table[GET_FLAT_IDX(fid, sid, TOTAL_SERVICE_COUNT)].callback(tmp_frame);
                    }
                }
            }
        }
    }
}


