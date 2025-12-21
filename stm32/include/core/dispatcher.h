#ifndef DISPATCHER_H
#define DISPATCHER_H

#include <core/common.h>
#include <drivers/uart.h>
#include <infrastructure/data_frame_protocol.h>


void dispatch_uart(UartDriver_t* uart, DataFramer_t** framers, FrameCallbackFailable* routing_table);

#endif //DISPATCHER_H
