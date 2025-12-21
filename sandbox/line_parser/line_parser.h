#include <stdint.h>
#include <stdbool.h>

#define LINE_LEN_MAX 64

typedef struct {
    uint8_t current_line[LINE_LEN_MAX];
    uint8_t current_idx;
    bool overflow;
    bool newline_found;
    uint8_t line_length;
} ASCIILine_t;

bool init_line(ASCIILine_t* line);
bool build_line(ASCIILine_t* line, uint8_t* p_src, uint8_t src_size);

