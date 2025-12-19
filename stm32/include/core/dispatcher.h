#ifndef DISPATCHER_H
#define DISPATCHER_H

#include <core/common.h>
#include <peripherals/uart.h>

typedef void (*BusDataGetter)(uint8_t* data, uint8_t data_len);

typedef struct {
    BusDataGetter get_data;
    bool ready_to_receive;
    uint32_t service_id;
} MessageRouter_t;

// All functions only callable from main / scheduler! 
bool init_dispatcher();

bool drain_uart_rx_and_dispatch(UartPortConfig_t uart);
bool read_n_bytes_uart_rx_and_dispatch(UartPortConfig_t uart, uint8_t num_bytes); // TODO
bool read_to_delimiter_uart_rx_and_dispatch(UartPortConfig_t uart, uint8_t delimiter); // TODO

bool disallow_service_listen(uint32_t service_id); // TODO - this allows you to "turn off dispatching" for idle
bool allow_service_listen(uint32_t service_id); // TODO

#endif DISPATCHER_H
