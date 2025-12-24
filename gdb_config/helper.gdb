set pagination off
set print pretty on
set print elements 0

define h
    printf "delete breakpoints              - Deletes all breakpoints\n"
    printf "info breakpoints                - Shows all breakpoints\n"
    printf "info locals                     - Shows local vars in current frame\n"
    printf "info args                       - Shows args in current frame\n"
    printf "where                           - Custom cmd - shows source line + PC\n"
    printf "bt<x>                           - Backtrace 8 frames\n"
    printf "x/<int><w,b>x                   - Inspects <int> <words, bytes\n"
    printf "x/s <addr>                      - C-string at addr\n"
    printf "list *$pc-<x>, +<y>             - List source x back and y forward\n"
end

define where
    frame
    info line *$pc 
    printf "PC = 0x%08x\n, $pc"
end

define get_bit
    set $b = ((uint32_t)$arg0 >> $arg1) & 1
    printf "bit[%d] = %d\n", $arg1, $b
end

define reset
    monitor reset halt
end


