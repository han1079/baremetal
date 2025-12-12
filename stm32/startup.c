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
void TIM2_Global_Handler(void)      __attribute__((weak, alias("No_Op_Handler")));
void TIM3_Global_Handler(void)      __attribute__((weak, alias("No_Op_Handler")));
void TIM6_Global_DACUnder_Handler(void)      __attribute__((weak, alias("No_Op_Handler")));
void TIM7_Global_Handler(void)      __attribute__((weak, alias("No_Op_Handler")));
void TIM14_Global_Handler(void)      __attribute__((weak, alias("No_Op_Handler")));
void TIM15_Global_Handler(void)      __attribute__((weak, alias("No_Op_Handler")));
void TIM16_Global_Handler(void)      __attribute__((weak, alias("No_Op_Handler")));
void TIM17_Global_Handler(void)      __attribute__((weak, alias("No_Op_Handler")));
void I2C1_Global_Handler(void)      __attribute__((weak, alias("No_Op_Handler")));
void I2C2_Global_Handler(void)      __attribute__((weak, alias("No_Op_Handler")));
void SPI1_Global_Handler(void)      __attribute__((weak, alias("No_Op_Handler")));
void SPI2_Global_Handler(void)      __attribute__((weak, alias("No_Op_Handler")));
void USART1_Global_Handler(void)      __attribute__((weak, alias("No_Op_Handler")));
void USART2_Global_Handler(void)      __attribute__((weak, alias("No_Op_Handler")));
void USART3TO8_Global_Handler(void)      __attribute__((weak, alias("No_Op_Handler")));
void CEC_CAN_Global_Handler(void)      __attribute__((weak, alias("No_Op_Handler")));
void USB_Global_Handler(void)      __attribute__((weak, alias("No_Op_Handler")));


MEMADDR_T vector_table[32] __attribute__((section(".vector_segment"))) = {
    (MEMADDR_T)&_estack,
    (MEMADDR_T)&Reset_Handler,
    (MEMADDR_T)&NMI_Handler,
    (MEMADDR_T)&HardFault_Handler,
    (MEMADDR_T)&SVCall_Handler,
    (MEMADDR_T)&PendSV_Handler,
    (MEMADDR_T)&SysTick_Handler,
    (MEMADDR_T)&WWDG_Handler,
    (MEMADDR_T)&PVD_VDDIO2_Handler,
    (MEMADDR_T)&RTC_Handler,
    (MEMADDR_T)&Flash_Handler,
    (MEMADDR_T)&RCC_CRS_Handler,
    (MEMADDR_T)&EXTI0_1_Handler,
    (MEMADDR_T)&EXTI2_3_Handler,
    (MEMADDR_T)&EXTI4_15_Handler,
    (MEMADDR_T)&TSC_Handler,
    (MEMADDR_T)&DMA1CH1_Handler,
    (MEMADDR_T)&DMA1CH23_DMA2CH12_Handler,
    (MEMADDR_T)&DMA1CH4TO7_DMA2CH3TO5_Handler,
    (MEMADDR_T)&ADC_COMP_Handler,
    (MEMADDR_T)&TIM1_BRK_UP_TRG_COM_Handler,
    (MEMADDR_T)&TIM1_CC_Handler,
    (MEMADDR_T)&TIM2_Global_Handler,
    (MEMADDR_T)&TIM3_Global_Handler,
    (MEMADDR_T)&TIM6_Global_DACUnder_Handler,
    (MEMADDR_T)&TIM7_Global_Handler,
    (MEMADDR_T)&TIM14_Global_Handler,
    (MEMADDR_T)&TIM15_Global_Handler,
    (MEMADDR_T)&TIM16_Global_Handler,
    (MEMADDR_T)&TIM17_Global_Handler,
    (MEMADDR_T)&I2C1_Global_Handler,
    (MEMADDR_T)&I2C2_Global_Handler,
    (MEMADDR_T)&SPI1_Global_Handler,
    (MEMADDR_T)&SPI2_Global_Handler,
    (MEMADDR_T)&USART1_Global_Handler,
    (MEMADDR_T)&USART2_Global_Handler,
    (MEMADDR_T)&USART3TO8_Global_Handler,
    (MEMADDR_T)&CEC_CAN_Global_Handler,
    (MEMADDR_T)&USB_Global_Handler
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

