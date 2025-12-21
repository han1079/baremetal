#include "definitions/uart_defs.h"
#include <drivers/uart.h>
#include <drivers/gpio.h>
#include <core/clock.h>

#define GET_OVERSAMPLE_MODE(uart) \
    GET_BIT((uart)->p_UART_BASE->CONTROL_REG_1, 15)

#define WRITE_BYTE_TO_TRANSMIT_REG(uart, data) \
    ((uart)->p_UART_BASE->TRANSMIT_DATA_REG = (REGADDR_T)(data)) // Write to TDR

#define READ_BYTE_FROM_RECEIVE_REG(uart) \
    ((uint8_t)((uart)->p_UART_BASE->RECEIVE_DATA_REG)) // Read from RDR

#define ACK_TRANSMIT_COMPLETE_AND_CLEAR_FLAG(uart) \
    SET_BIT((uart)->p_UART_BASE->INTERRUPT_CLEAR_REG, 6); // Clear TC flag

#define READ_TRANSMIT_REG_EMPTY_FLAG(uart) \
    GET_BIT((uart)->p_UART_BASE->INTERRUPT_STATUS_REG, 7) // Read TXE flag

#define READ_RECEIVE_DATA_REG_NOT_EMPTY_FLAG(uart) \
    GET_BIT((uart)->p_UART_BASE->INTERRUPT_STATUS_REG, 5) // Read RXNE flag

void __uart_configure_gpio_pins(UartPort_t uart_port){
    gpio_configure_alt_function(*(uart_port.p_rx_pin), \
                                (uart_port.p_rx_pin)->af->UART, \
                                PINSPEED_HIGH, \
                                PIN_PULL_UP);
    gpio_configure_alt_function(*(uart_port.p_tx_pin), \
                                (uart_port.p_tx_pin)->af->UART, \
                                PINSPEED_HIGH, \
                                PIN_PULL_FLOATING);
    set_pin_mode(*(uart_port.p_rx_pin), PINMODE_AF);
    set_pin_mode(*(uart_port.p_tx_pin), PINMODE_AF);
}

void uart_init(UartPort_t uart_port) {
    ring_buffer_init(uart_port.config->rx_ring_buffer, \
        uart_port.config->rx_buffer_storage, \
        uart_port.config->buffer_size);

    ring_buffer_init(uart_port.config->tx_ring_buffer, \
        uart_port.config->tx_buffer_storage, \
        uart_port.config->buffer_size);

    __uart_enable_clock(uart_port.config);
    __uart_configure_gpio_pins(uart_port);
    __uart_configure_baud_rate(uart_port.config, uart_port.config->baud_rate);
    __uart_enable_transmitter(uart_port.config);
    __uart_enable_receiver(uart_port.config);
    __uart_enable_base(uart_port.config);
    __uart_enable_rx_interrupt(uart_port.config);
}

void uart_allow_interrupts(UartPort_t uart_port) {
    __uart_enable_nvic_interrupt(uart_port.config);
}

void uart_disallow_interrupts(UartPort_t uart_port) {
    __uart_disable_nvic_interrupt(uart_port.config);
}

void uart_deinit(UartPort_t uart_port) {
    uart_disallow_interrupts(uart_port);
    __uart_disable_rx_interrupt(uart_port.config);
    __uart_disable_base(uart_port.config);
    __uart_disable_receiver(uart_port.config);
    __uart_disable_transmitter(uart_port.config);
    __uart_disable_clock(uart_port.config);
}



// Read and Write Functions
/*HOW UART TX WORKS

If you assume the UART is CONFIGURED and ENABLED already, the actual hardware
functionality is relatively simple.

The PIN is hooked up to a SHIFT REGISTER. Each baud cycle, the SHIFT REGISTER
"presents" the next bit to the PIN. This is quite LITERALLY setting the voltage
on the pin. You can therefore treat "being in the shift register" as "being sent".

To send data, you write a byte to the TRANSMIT DATA REGISTER (TDR). This
register is kind of like a "staging area". The SHIFT REGISTER will "load"
all of this data once it's "SHIFTED OUT" its entire contents:

-----------------------------------
STILL SENDING
-----------------------------------

        SHIFT REGISTER              TRANSMIT DATA REGISTER
|PIN|<--|0|1|0|1|0|1|0|0|           |1|0|1|0|1|1|1|0|
          ^^^^^^^^^^^^                ^^^^^^^^^^^^
        Currently being sent        Waiting to be sent

------------------------------------
DONE SENDING
------------------------------------
        SHIFT REGISTER              TRANSMIT DATA REGISTER
|PIN|<--|EMPTY|   <--------------   |1|0|1|0|1|1|1|0|
        ^^^^^^^^                      ^^^^^^^^^^^^
        Ready for new data          Loads into shift register

-----------------------------------
NEW DATA SENDING
-----------------------------------

        SHIFT REGISTER              TRANSMIT DATA REGISTER
|PIN|<--|1|0|1|0|1|1|1|0|           |EMPTY| ------------------> |TXE|
          ^^^^^^^^^^^^                ^^^^^^^^^^^^              ^^^^^
        Currently being sent        Ready for new data         TXE Flag Set

This TXE flag is can be READ by the software like a normal bit. If it's 
SET, it means that the staging area (TDR) is EMPTY and ready for new data.

You write data - and once the currently sent bits are done, the new ones
are loaded into the SHIFT REGISTER and sent out bit by bit.

CRUCIALLY - when you WRITE data to the staging area (TDR), the TXE flag
is AUTOMATICALLY CLEARED by the hardware. You don't need to do anything
to clear it. It will be set again once the data is fully sent.

Therefore - from the software side - to send data, you need to:
1. READ the TXE flag to see if the TDR is ready for new data
2. If it is ready, WRITE the new byte to the TDR
3. Repeat as necessary for more data

METHOD 1:

This action can be done most "naively by "looping and waiting" 
for the TXE flag to be set, sending the next byte, then waiting again, etc. 

METHOD 2:

You can enable the TXEIE bit. This causes the TXE flag to also write a 
"bit 1" to the NVIC UART Interrupt Pending Register. If interrupts are
ENABLED - this literally causes the hardware to trigger a command
that "overrides" where you are in the normal execution flow and JUMPS to the
UART Interrupt Handler "Lookup". This is HARD BAKED as the 37th entry in
the vector table - which is a chunk of memory at the start of flash memory.

Specifically - (on UART1) it jumps to address 0x000000AC, and reads the
data there. The data there is itself an ADDRESS of a function we define
called "USART1_GLOBAL_Handler". This function is declared in startup.c
and defined here in uart.c.

In this function, we can then do all the TXE reading and TDR writing, since
we are guaranteed that the software will pause whatever it was doing and
jump here when TXE is set. This is fundamentally how interrupts work.

In our function, then, we can "if statement" off of the TXE flag, and
send the next byte as necessary, then we CLEAR the TXE flag and return.

This allows the main program to continue doing whatever it was doing, and
the moment the TDR is empty, it'll ping TXE again, causing another interrupt,
*/

// Single Byte Send. "Naive" Implementation
void uart_send_byte_blocking(UartPort_t uart_port, uint8_t data) {
    while ((READ_TRANSMIT_REG_EMPTY_FLAG(uart_port.config) == 0));
    WRITE_BYTE_TO_TRANSMIT_REG(uart_port.config, data);
}


/* HOW UART RX WORKS

Receiving data is somewhat similar to transmitting data. There is
a "staging area" called the RECEIVE DATA REGISTER (RDR) that holds the
"latest" received byte.

What is a received byte? It's when the SHIFT REGISTER (that is constantly
"listening" on the PIN) has "shifted in" a full byte of data. Once
it has done so, it "loads" this byte into the RDR.

The "indicator" is simliar to TXE - but it's called the RXNE (RX NOT EMPTY) flag. This
flag is SET by the hardware once a new byte has been loaded into the RDR.

However - we DO NOT CONTROL input data stream! That means while the RDR is sitting,
it's very much possible that the NEW data is coming in via the SHIFT REGISTER.

However - RDR is only EMPTIED when we READ from it. Furthermore, on the M0,
there is ONLY A SINGLE BYTE RDR. That means every time RXNE is set, we MUST
READ the RDR to get the data out - or else the data filling the SHIFT REGISTER
has NO WHERE TO GO once it shifts in a new byte.

If this happens, the NEW data is LOST. This is called an OVERRUN error. This
will set the ORE flag in the INTERRUPT STATUS REGISTER.

Therefore, a "polling" based RX implementation is quite inefficient. Because
you don't know when data is coming in, you have to "constantly" check the RXNE flag,
READ the RDR, and hope that you are "fast enough" to read it before the next byte
comes in.

This is probably FINE if your main loop "goes through all of its logic" quickly,
since you can just do a "check RXNE, read RDR if set" every loop. However, this does NOT
scale very well to more complicated systems, and is generally wasteful.

A much better way is to use INTERRUPTS. Similar to TXEIE, there is an RXNEIE bit
that causes the RXNE flag to "ping" the NVIC UART Interrupt Pending Register
when it is set. This causes the same interrupt mechanism to occur as above, except now,
we read RXNE and grab the data from RDR.

REMEMBER - all we need to do is CLEAR the RDR by READING from it. This automatically
clears the RXNE flag and allows new data to come in. We can then leverage
the larger amount of storage in RAM to hold onto the data until the main program
is ready to process it.

Thus, we still have 2 methods. Polling and Interrupt Based:

Method 1: Polling Based RX

This is similar to the TXE polling based TX. We just "loop and wait" for RXNE to be set,
then READ the RDR. This is what Arduino does when you write:

    while (!Serial.available());
    byte data = Serial.read();

I personally despise this approach, since you are putting essentially a while 1 in another
while 1. This means that if no data is coming in, your entire program is stuck in this loop,
unable to do anything else. AWFUL!

Method 2: Interrupt Based RX

This is how most serious systems do RX. You enable RXNEIE, and in the UART Interrupt Handler,
you check RXNE, READ the RDR, and STORE the data somewhere safe (like a circular buffer in RAM).
This way, the main program can do whatever it wants, and the moment data comes in, the interrupt
will "fire", allowing you to grab the data.

Since this is "context rich" - the main program can then process the data at its leisure, or the
ring buffer can intelligent PARSE the data into meaningful packets, and then dispatch those
packets to the appropriate handlers.

We can do this by essentially recreating the "Interrupt Table", but for more complex "packet"
routing events.

For example - if a packet is received, and it says "TURN ON LED", we can have the RX interrupt handler
parse the packet, and store a "1" in the "LED Control Variable".

Put this way - all of the main loop functionality turns into "checking control variables", 
"checking current state", and "dispatch to the correct functions".

This way - UART becomes essentially an "event source" that triggers state changes in the main program,
which then we can treat as the top level "scheduler" of actions that our system performs.

Basically - a very small event buffer for a very small operating system.
*/

// Single Byte Receive. "Naive" Implementation
uint8_t uart_receive_byte_blocking(UartPort_t uart_port) {
    while ((READ_RECEIVE_DATA_REG_NOT_EMPTY_FLAG(uart_port.config) == 0)); 
    return READ_BYTE_FROM_RECEIVE_REG(uart_port.config); 
}


void USART1_GLOBAL_Handler() {
    // Check RXNE Flag
    if (READ_RECEIVE_DATA_REG_NOT_EMPTY_FLAG(&UART1)) {
        uint8_t received_byte = READ_BYTE_FROM_RECEIVE_REG(&UART1);
        push_to_ring_buffer(UART1.rx_ring_buffer, received_byte);
    }

    // Check TXE Flag
    if (READ_TRANSMIT_REG_EMPTY_FLAG(&UART1)) {
        uint8_t byte_to_tx;
        if (pop_from_ring_buffer(UART1.tx_ring_buffer, &byte_to_tx)) {
            WRITE_BYTE_TO_TRANSMIT_REG(&UART1, byte_to_tx);
        } else {
            // No more data to send, disable TXE interrupt
            __uart_disable_tx_interrupt(&UART1);
        }
    }
}

void uart_write_byte_array(UartDriver_t* uart, ByteSpan_t p_data) {
    uint8_t* data = p_data.bytes;
    uint8_t length = p_data.count;
    if ((READ_TRANSMIT_REG_EMPTY_FLAG(uart) != 0)) { return; } // UART not ready

    ASSERT(__get_uart_nvic_interrupt_enable_status(uart) != 0); // NVIC Interrupts must be enabled
    ASSERT(__get_uart_tx_interrupt_enable_status(uart) == 0); // TXE Interrupt must be disabled

    for (uint32_t i = 0; i < length; i++) {
        push_to_ring_buffer(uart->tx_ring_buffer, data[i]);
    }

    // By here, we KNOW that interrupts are enabled globally, and TXE shows the shift register is empty
    // We enable TXE interrupts to kick off the first send.
    __uart_enable_tx_interrupt(uart);
}

bool uart_drain_rx_buffer_until_delimiter(UartDriver_t* uart, uint8_t delimiter, uint8_t* dest, uint8_t dest_length) {
    while((READ_RECEIVE_DATA_REG_NOT_EMPTY_FLAG(uart) == 0)); // Wait for last byte to arrive
    ASSERT(dest_length > 0); // This messes up indexing. Crash out.

    uint8_t index = 0;
    uint8_t byte;
    uint8_t max_index = dest_length - 1; // Leave space for null terminator
    bool delimiter_found = false;

    while (!delimiter_found && index <= max_index) {
        // Case Logic: Pop, and only save if byte has sensible data in it.
        if (pop_from_ring_buffer(uart->rx_ring_buffer, &byte)) 
        { 
            dest[index] = byte; 
        }

        // Logic variables for return.
        if (byte == delimiter) { 
            delimiter_found = true;
        } else {
            index++;
        }
    }

    if(delimiter_found) { return true; }
    return false; 
}
uint8_t uart_drain_rx_buffer_n_bytes(UartDriver_t* uart, uint8_t *dest, uint8_t dest_length, uint8_t num_bytes) {
    while((READ_RECEIVE_DATA_REG_NOT_EMPTY_FLAG(uart) == 0)); // Wait for last byte to arrive
    ASSERT(dest_length > 0); // This messes up indexing. Crash out.
    if (num_bytes > dest_length) { return 0; } // This could be a mistake. Return with error.

    uint8_t index = 0;
    uint8_t byte;
    
    bool rb_empty = false;

    while (!rb_empty && index < num_bytes) {
        // Case Logic : Pop and write only if there's somethere there.
        if (pop_from_ring_buffer(uart->rx_ring_buffer, &byte)) {
            dest[index] = byte;
            index++;
        } else {
            rb_empty = true;
        }
    }

    // Index increments after assignment. Therefore - this tracks how many bytes 
    // were actually written. Guaranteed to be <= num_bytes
    return index;
}

bool uart_get_rx_buffer_next_byte(UartDriver_t* uart, uint8_t *dest) {
    while((READ_RECEIVE_DATA_REG_NOT_EMPTY_FLAG(uart) == 0)); // Wait for last byte to arrive
    uint8_t byte;

    // Only grab from ring buffer if there's something there.
    if (pop_from_ring_buffer(uart->rx_ring_buffer, &byte)) {
        (*dest) = byte;
        return true;
    } else {
        return false;
    }
}

uint8_t get_uart_rx_buffer_count(UartDriver_t *uart) {
    return uart->rx_ring_buffer->count;
}

uint8_t get_uart_tx_buffer_count(UartDriver_t* uart) {
    return uart->tx_ring_buffer->count;
}

uint32_t __uart_get_clock_frequency(UartDriver_t* uart) {
    uint8_t clock_source = __uart_get_clock_source(uart);
    switch (clock_source) {
        case 0b00: // PCLK
            uint32_t apb_prescale = get_apb1_prescaler_divider();
            uint32_t ahb_prescale = get_ahb_prescaler_divider();
            ASSERT(ahb_prescale == 1);
            ASSERT(apb_prescale == 1);
            uint32_t pclk_freq = get_system_core_clock() / ahb_prescale / apb_prescale;
            return pclk_freq;
        case 0b01: // SYSCLK
            return get_system_core_clock();
        case 0b10: // HSI
            return 8000000UL;
        case 0b11: // LSE
            ASSERT(0); // Not supported
            return 0;
        default:
            ASSERT(0); // Should not happen
            return 0;
    }
}

void __uart_configure_baud_rate(UartDriver_t* uart, uint32_t baud) {
    uint32_t uart_clock_frequency = __uart_get_clock_frequency(uart);
    uint8_t oversample_by_8 = GET_OVERSAMPLE_MODE(uart);

    ASSERT(oversample_by_8 == 0); // Only oversample by 16 supported for now
    
    uint32_t uartdiv = uart_clock_frequency / baud; // Rounded division
    SET_WORD(uart->p_UART_BASE->BAUD_RATE_REG, uartdiv);
}
