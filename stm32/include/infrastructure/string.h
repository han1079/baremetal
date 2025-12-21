#ifndef STRING_H
#define STRING_H

#include <stdint.h>
#include <stdbool.h>
#include <infrastructure/bytes.h>

typedef struct {
    const char* p_str;
    uint8_t len;
} string_t;

#define CSTRING(x) ((string_t){.p_str = (x), .len = (uint8_t)sizeof(x) - 1}) // ONLY USE ON STRING LITERALS

#define BYTES_LIT_NO_TERM(x) ((ByteSpan_t){ \
    .bytes = (uint8_t*)(x), \
    .count  = (uint8_t)(sizeof(x) - 1) \
})

#define BYTES_LIT(x) ((ByteSpan_t){ \
    .bytes = (uint8_t*)(x), \
    .count  = (uint8_t)(sizeof(x) - 1) \
})

#define STRING_TO_BYTES(s) ((ByteSpan_t){ \
    .bytes = (uint8_t*)(s.p_str), \
    .count = (uint8_t) s.len \
})

#endif // STRING_H
