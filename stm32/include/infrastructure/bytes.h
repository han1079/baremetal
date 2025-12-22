#ifndef BYTES_H
#define BYTES_H 

#include <stdint.h>
#include <stdbool.h>


typedef struct {
    uint8_t* bytes;
    uint8_t count;
} ByteSpan_t;

typedef struct {
    uint8_t* bytes;
    uint8_t count;
    bool result;
} ByteSpan_Failable_t;

#define BYTESPAN(b,n) (ByteSpan_t){b,n}
#endif //BYTES_H 
