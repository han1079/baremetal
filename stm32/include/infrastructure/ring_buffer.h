#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <stdint.h>
#include <stdbool.h>
#include <infrastructure/comms_protocol.h>

typedef struct {
    uint8_t buffer[BUFFER_LEN_MAX];
    uint8_t size;
    volatile uint8_t head;
    volatile uint8_t tail;
    volatile uint8_t count;
} RingBuffer_t;

void ring_buffer_init(RingBuffer_t* rb);
bool ring_buffer_empty(RingBuffer_t* rb);
bool ring_buffer_full(RingBuffer_t* rb);
bool push_to_ring_buffer(RingBuffer_t* rb, uint8_t data); 
bool pop_from_ring_buffer(RingBuffer_t* rb, uint8_t* dest); 




#endif //RING_BUFFER_H
