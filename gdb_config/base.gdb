source ./gdb_config/helper.gdb
target remote :3333
monitor reset halt
load
monitor reset halt

define uart_debug_on
    source ./gdb_config/gdb_uart_debug.gdb 
end
