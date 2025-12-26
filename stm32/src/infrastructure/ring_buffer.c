#include <infrastructure/ring_buffer.h>
#include <core/common.h>
#include <core/interrupts.h>

void ring_buffer_init(RingBuffer_t* rb) {
    rb->size = BUFFER_LEN_MAX;
    rb->head = 0;
    rb->tail = 0;
    rb->count = 0;
}

bool ring_buffer_empty(RingBuffer_t* rb) {
    if ((rb->head) == (rb->tail)) {
        ASSERT(rb->count == 0);
        return true;
    }
    return false;
}

bool ring_buffer_full(RingBuffer_t *rb) {
    if ((rb->head + 1) % rb->size == (rb->tail)) {
        ASSERT(rb->count == (rb->size - 1));
        return true;
    }
    return false;
}

bool push_to_ring_buffer(RingBuffer_t *rb, uint8_t data) {
    uint32_t key = lock_interrupts_and_save();
    if (ring_buffer_full(rb)) {
        unlock_interrupts_and_restore(key);
        return false;
    } // No writes when full.
    
    uint8_t next_head_idx = (rb->head + 1) % (rb->size);
    rb->buffer[rb->head] = data;
    rb->head = next_head_idx;
    rb->count++;

    if (rb->count > rb->max_used) { rb->max_used = rb->count; }

    unlock_interrupts_and_restore(key);
    return true;
}

bool pop_from_ring_buffer(RingBuffer_t *rb, uint8_t *dest) {
    uint32_t key = lock_interrupts_and_save();

    if (ring_buffer_empty(rb)) {
        unlock_interrupts_and_restore(key);
        return false;
    } // No writes when empty 

    uint8_t next_tail_idx = (rb->tail + 1) % (rb->size);
    
    // Move data from buffer to destination memory.
    (*dest) = rb->buffer[rb->tail];
    rb->tail = next_tail_idx;
    rb->count--;

    unlock_interrupts_and_restore(key);

    return true;
}



