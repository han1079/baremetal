/*Memory for STM32F09XX - 256K Flash, 32K RAM
 *
 * Starting at 0x08000000 is the standard for STM32 
 * devices due to special boot mode shenanigans.
 *
 * */
MEMORY {
    FLASH(rx) : ORIGIN = 0x08000000, LENGTH = 256K
    SRAM(rwx) : ORIGIN = 0x20000000, LENGTH = 32K
}

_estack = ORIGIN(SRAM) + LENGTH(SRAM);

SECTIONS {
    .vector_segment : {
        *(.vector_segment)
    } >FLASH  AT> FLASH
    /*We only need the vector segment in flash since it's basically
     * a bunch of callable entry points */

    .text : {
        *(.text)
        _text_end = .;
        _data_start_pos_flash = .;
    } >FLASH AT> FLASH
    /*Redundant to do the LMA here, but just being careful*/
    
    .data : {
        _data_start = .;
        *(.data)
        _data_stop = .;
    } >SRAM AT> FLASH 
    /*VMA starts in SRAM, so start assigning addr->symbols at 0x2000000.
     * This iteration process continues until all of .data is assigned.
     * 
     * When LOADING in the .bin file - start shoving in the .data values
     * into the LATEST FLASH location. .text has already accumulated
     * a bunch - and we continue here.
     * */

    .bss : {
        _bss_start = .;
        *(.bss)
        _bss_stop = .;
    } > SRAM 
    /*VMA is simply symbol assignment to the latest available
     * SRAM LOC - incrementing until we're out of .bss. */
}
