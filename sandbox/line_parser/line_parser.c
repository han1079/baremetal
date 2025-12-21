#include "line_parser.h"

bool init_line(ASCIILine_t* line) {
    line->current_idx = 0;
    line->overflow = false;
    line->newline_found = false;
    line->line_length = 0;
}


bool build_line(ASCIILine_t* line, uint8_t* p_src, uint8_t src_size) {
    if (line-) 
}
