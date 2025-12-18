#include <definitions/uart_defs.h>

const UartPortConfig_t UART1 = {
    .p_UART_BASE = (UART_Typedef*)UART1_BASE,
    .p_NVIC_ENABLE_REG = (REGADDR_T*)UART_NVIC_SET_ENABLE_REG,
    .nvic_enable_offset = NVIC_UART1_OFFSET,
    .p_UART_CLOCK_SOURCE_REG = (REGADDR_T*)&(RCC->CLOCK_CONFIG_3),
    .uart_clock_source_offset = UART1_CLOCK_SOURCE_OFFSET, 
    .p_APB_CLOCK_ENABLE_REG = (REGADDR_T*)&(RCC->PERIPH_APB_2_CLOCK_ENABLE),
    .apb_clock_enable_offset = UART1_APB_ENABLE_OFFSET,
    .baud_rate = 115200,
};
const UartPortConfig_t UART2 = {
    .p_UART_BASE = (UART_Typedef*)UART2_BASE,
    .p_NVIC_ENABLE_REG = (REGADDR_T*)UART_NVIC_SET_ENABLE_REG,
    .nvic_enable_offset = NVIC_UART2_OFFSET,
    .p_UART_CLOCK_SOURCE_REG = (REGADDR_T*)&(RCC->CLOCK_CONFIG_3),
    .uart_clock_source_offset = UART2_CLOCK_SOURCE_OFFSET, 
    .p_APB_CLOCK_ENABLE_REG = (REGADDR_T*)&(RCC->PERIPH_APB_1_CLOCK_ENABLE),
    .apb_clock_enable_offset = UART2_APB_ENABLE_OFFSET,
    .baud_rate = 115200,
};
const UartPortConfig_t UART3 = {
    .p_UART_BASE = (UART_Typedef*)UART3_BASE,
    .p_NVIC_ENABLE_REG = (REGADDR_T*)UART_NVIC_SET_ENABLE_REG,
    .nvic_enable_offset = NVIC_UART3TO8_OFFSET,
    .p_UART_CLOCK_SOURCE_REG = (REGADDR_T*)&(RCC->CLOCK_CONFIG_3),
    .uart_clock_source_offset = UART3_CLOCK_SOURCE_OFFSET, 
    .p_APB_CLOCK_ENABLE_REG = (REGADDR_T*)&(RCC->PERIPH_APB_1_CLOCK_ENABLE),
    .apb_clock_enable_offset = UART3_APB_ENABLE_OFFSET,
    .baud_rate = 115200,
};

const UartPort_t UART_PORT_TXPA9_RXPA10 = {
    .config = &UART1,
    .p_tx_pin = &PA9,
    .p_rx_pin = &PA10,
};