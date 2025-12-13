#include <stdint.h>
#define MEMADDR_T uint32_t

extern MEMADDR_T _estack;
extern MEMADDR_T _data_start;
extern MEMADDR_T  _data_stop;
extern MEMADDR_T  _bss_start;
extern MEMADDR_T  _bss_stop;

extern MEMADDR_T _data_start_pos_flash;

extern void main(void);

void Reset_Handler(void); 
void NMI_Handler(void)          __attribute__((weak, alias("Crash_Handler")));
void HardFault_Handler(void)    __attribute__((weak, alias("Crash_Handler")));
void SVCall_Handler(void)       __attribute__((weak, alias("No_Op_Handler")));
void PendSV_Handler(void)       __attribute__((weak, alias("No_Op_Handler")));
void SysTick_Handler(void)      __attribute__((weak, alias("No_Op_Handler")));
void WWDG_Handler(void)      __attribute__((weak, alias("No_Op_Handler")));
void PVD_VDDIO2_Handler(void)      __attribute__((weak, alias("No_Op_Handler")));
void RTC_Handler(void)      __attribute__((weak, alias("No_Op_Handler")));
void Flash_Handler(void)      __attribute__((weak, alias("No_Op_Handler")));
void RCC_CRS_Handler(void)      __attribute__((weak, alias("No_Op_Handler")));
void EXTI0_1_Handler(void)      __attribute__((weak, alias("No_Op_Handler")));
void EXTI2_3_Handler(void)      __attribute__((weak, alias("No_Op_Handler")));
void EXTI4_15_Handler(void)      __attribute__((weak, alias("No_Op_Handler")));
void TSC_Handler(void)      __attribute__((weak, alias("No_Op_Handler")));
void DMA1CH1_Handler(void)      __attribute__((weak, alias("No_Op_Handler")));
void DMA1CH23_DMA2CH12_Handler(void)      __attribute__((weak, alias("No_Op_Handler")));
void DMA1CH4TO7_DMA2CH3TO5_Handler(void)      __attribute__((weak, alias("No_Op_Handler")));
void ADC_COMP_Handler(void)      __attribute__((weak, alias("No_Op_Handler")));
void TIM1_BRK_UP_TRG_COM_Handler(void)      __attribute__((weak, alias("No_Op_Handler")));
void TIM1_CC_Handler(void)      __attribute__((weak, alias("No_Op_Handler")));
void TIM2_GLOBAL_Handler(void)      __attribute__((weak, alias("No_Op_Handler")));
void TIM3_GLOBAL_Handler(void)      __attribute__((weak, alias("No_Op_Handler")));
void TIM6_GLOBAL_DACUnder_Handler(void)      __attribute__((weak, alias("No_Op_Handler")));
void TIM7_GLOBAL_Handler(void)      __attribute__((weak, alias("No_Op_Handler")));
void TIM14_GLOBAL_Handler(void)      __attribute__((weak, alias("No_Op_Handler")));
void TIM15_GLOBAL_Handler(void)      __attribute__((weak, alias("No_Op_Handler")));
void TIM16_GLOBAL_Handler(void)      __attribute__((weak, alias("No_Op_Handler")));
void TIM17_GLOBAL_Handler(void)      __attribute__((weak, alias("No_Op_Handler")));
void I2C1_GLOBAL_Handler(void)      __attribute__((weak, alias("No_Op_Handler")));
void I2C2_GLOBAL_Handler(void)      __attribute__((weak, alias("No_Op_Handler")));
void SPI1_GLOBAL_Handler(void)      __attribute__((weak, alias("No_Op_Handler")));
void SPI2_GLOBAL_Handler(void)      __attribute__((weak, alias("No_Op_Handler")));
void USART1_GLOBAL_Handler(void)      __attribute__((weak, alias("No_Op_Handler")));
void USART2_GLOBAL_Handler(void)      __attribute__((weak, alias("No_Op_Handler")));
void USART3TO8_GLOBAL_Handler(void)      __attribute__((weak, alias("No_Op_Handler")));
void CEC_CAN_GLOBAL_Handler(void)      __attribute__((weak, alias("No_Op_Handler")));
void USB_GLOBAL_Handler(void)      __attribute__((weak, alias("No_Op_Handler")));


MEMADDR_T vector_table[48] __attribute__((section(".vector_segment"))) = {
    (MEMADDR_T)&_estack,                        // 0x00000000 - Initial Stack Pointer
    (MEMADDR_T)&Reset_Handler,                  // 0x00000004 - Reset
    (MEMADDR_T)&NMI_Handler,                    // 0x00000008 - NMI
    (MEMADDR_T)&HardFault_Handler,              // 0x0000000C - Hard Fault
    0,                                          // 0x00000010 - Reserved
    0,                                          // 0x00000014 - Reserved
    0,                                          // 0x00000018 - Reserved
    0,                                          // 0x0000001C - Reserved
    0,                                          // 0x00000020 - Reserved
    0,                                          // 0x00000024 - Reserved
    0,                                          // 0x00000028 - Reserved
    (MEMADDR_T)&SVCall_Handler,                 // 0x0000002C - SVCall
    0,                                          // 0x00000030 - Reserved
    0,                                          // 0x00000034 - Reserved
    (MEMADDR_T)&PendSV_Handler,                 // 0x00000038 - PendSV
    (MEMADDR_T)&SysTick_Handler,                // 0x0000003C - SysTick
    (MEMADDR_T)&WWDG_Handler,                   // 0x00000040 - Window Watchdog
    (MEMADDR_T)&PVD_VDDIO2_Handler,             // 0x00000044 - PVD and VDDIO2
    (MEMADDR_T)&RTC_Handler,                    // 0x00000048 - RTC
    (MEMADDR_T)&Flash_Handler,                  // 0x0000004C - Flash
    (MEMADDR_T)&RCC_CRS_Handler,                // 0x00000050 - RCC and CRS
    (MEMADDR_T)&EXTI0_1_Handler,                // 0x00000054 - EXTI Line 0 and 1
    (MEMADDR_T)&EXTI2_3_Handler,                // 0x00000058 - EXTI Line 2 and 3
    (MEMADDR_T)&EXTI4_15_Handler,               // 0x0000005C - EXTI Line 4 to 15
    (MEMADDR_T)&TSC_Handler,                    // 0x00000060 - TSC
    (MEMADDR_T)&DMA1CH1_Handler,                // 0x00000064 - DMA1 Channel 1
    (MEMADDR_T)&DMA1CH23_DMA2CH12_Handler,      // 0x00000068 - DMA1 Channel 2 and 3, DMA2 Channel 1 and 2
    (MEMADDR_T)&DMA1CH4TO7_DMA2CH3TO5_Handler,  // 0x0000006C - DMA1 Channel 4 to 7, DMA2 Channel 3 to 5
    (MEMADDR_T)&ADC_COMP_Handler,               // 0x00000070 - ADC and COMP
    (MEMADDR_T)&TIM1_BRK_UP_TRG_COM_Handler,    // 0x00000074 - TIM1 Break, Update, Trigger and Commutation
    (MEMADDR_T)&TIM1_CC_Handler,                // 0x00000078 - TIM1 Capture Compare
    (MEMADDR_T)&TIM2_GLOBAL_Handler,            // 0x0000007C - TIM2
    (MEMADDR_T)&TIM3_GLOBAL_Handler,            // 0x00000080 - TIM3
    (MEMADDR_T)&TIM6_GLOBAL_DACUnder_Handler,   // 0x00000084 - TIM6 and DAC Underrun
    (MEMADDR_T)&TIM7_GLOBAL_Handler,            // 0x00000088 - TIM7
    (MEMADDR_T)&TIM14_GLOBAL_Handler,           // 0x0000008C - TIM14
    (MEMADDR_T)&TIM15_GLOBAL_Handler,           // 0x00000090 - TIM15
    (MEMADDR_T)&TIM16_GLOBAL_Handler,           // 0x00000094 - TIM16
    (MEMADDR_T)&TIM17_GLOBAL_Handler,           // 0x00000098 - TIM17
    (MEMADDR_T)&I2C1_GLOBAL_Handler,            // 0x0000009C - I2C1
    (MEMADDR_T)&I2C2_GLOBAL_Handler,            // 0x000000A0 - I2C2
    (MEMADDR_T)&SPI1_GLOBAL_Handler,            // 0x000000A4 - SPI1
    (MEMADDR_T)&SPI2_GLOBAL_Handler,            // 0x000000A8 - SPI2
    (MEMADDR_T)&USART1_GLOBAL_Handler,          // 0x000000AC - USART1
    (MEMADDR_T)&USART2_GLOBAL_Handler,          // 0x000000B0 - USART2
    (MEMADDR_T)&USART3TO8_GLOBAL_Handler,       // 0x000000B4 - USART3 to USART8
    (MEMADDR_T)&CEC_CAN_GLOBAL_Handler,         // 0x000000B8 - CEC and CAN
    (MEMADDR_T)&USB_GLOBAL_Handler              // 0x000000BC - USB
};

void Crash_Handler(void) {
    while(1);
}

void No_Op_Handler(void) {
    return;
}

void Reset_Handler(void) {
    /* This is the entry point for the program.
    * The RESET vector points to the function pointer for this guy
    * As a result, this function needs to do 2 things:
    * 1. It needs to load all of the data and .bss variables from flash into
    *       sram based on the linker extern variables 
    * 2. It needs to jump into main after variable loading is complete.
    */

    // Variable loading

    uint32_t size = (uint32_t)&_data_stop - (uint32_t)&_data_start;

    
    /* Since we control where in flash the start of data occurs - we just use the
     * linker symbol directly. This means .ld needs to explicitly assign _data_start
     * when defining the MEMORY segment. This is usually just 0x20000000 for STM32s.
     *
     * The data_start_pos_flash (_sidata in convention) is set when CONSTRUCTING
     * the .data section in the SECTIONS part of the linker script.
     * */
    uint8_t* p_dest = (uint8_t*)&_data_start;
    uint8_t* p_src = (uint8_t*)&_data_start_pos_flash;

    for (int i = 0; i < size; i++) {
        // Since the first iteration is already lined up - assign it first.
        // Increment in an organized fashion afterward.
        *p_dest = *p_src;

        // Separate out the increment from assignment to prevent ambiguity.
        ++p_dest;
        ++p_src;
    }

    size = (uint32_t)&_bss_stop - (uint32_t)&_bss_start;

    /*The .bss section need not be directly contiguous with .data. This is why we want
     * to set the location explicitly in the linker file and jump to writing to that 
     * part of memory. 
     *
     * The source, however, IS continuous, since the flash loading is just a bunch
     * of appends. This way, we can use the old p_dest, since it just finished with .data.
     * */
    p_dest = (uint8_t*)&_bss_start;

    for (int i = 0; i < size; i++) {
        // p_dest picks up from where it left off.
        *p_dest = 0;
        ++p_dest;
        ++p_src;
    }


    // Now we jump into main. This is where the main app comes in.
    main();
}

