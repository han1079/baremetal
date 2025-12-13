#include <nvic.h>

// Interrupt type goes 0-31, so 4 bits to index. Bitshifting right by 2 is equivalent to dividing by 4 
// and discarding the remainder. This gives the correct word index.
#define NVIC_PRIORITY_WORD_IDX(type) ((uint8_t)(type >> 2))

// The priority offset is determined by the remainder of type / 4 (i.e., type % 4),
// multiplied by 8 (since each priority field is 8 bits apart).
// Finally, we want to shift by 6, since the priority looks like 0-3, and we want to
// set it in bits 6 and 7 of the byte.
#define NVIC_PRIORTY_WORD_OFFSET(type) ((uint8_t)((type % 4) * 8 + 6))

void set_interrupt_type_priority(InterruptType type, InterruptPriority priority) {
    // Arrow to get pointer to WORD. Bracket access for value at index.
    // Address to get the register address again.
    REGADDR_T* priority_reg = &NVIC_PRIORITY->WORD[NVIC_PRIORITY_WORD_IDX(type)];

    // Mask out the priority bits to clear them.
    uint32_t clear_mask = ~(0b11 << NVIC_PRIORTY_WORD_OFFSET(type));
    (*priority_reg) &= clear_mask;

    // Set the new priority bits by doing the shift and ORing.
    uint32_t priority_value = (priority << NVIC_PRIORTY_WORD_OFFSET(type));
    (*priority_reg) |= priority_value;
}
uint32_t get_interrupt_type_priority(InterruptType type) {
    uint32_t priority_reg = NVIC_PRIORITY->WORD[NVIC_PRIORITY_WORD_IDX(type)];
    uint32_t priority_value = (priority_reg >> NVIC_PRIORTY_WORD_OFFSET(type)) & 0b11;
    return priority_value;
}