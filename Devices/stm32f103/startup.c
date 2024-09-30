#include <stdint.h>
#include "rcc.h"
#include "swd.h"

#define SRAM_START (0x20000000U)
#define SRAM_SIZE (20U * 1024U)
#define SRAM_END (SRAM_START + SRAM_SIZE)
#define STACK_POINTER_INIT_ADDRESS (SRAM_END)
#define BootRAM 0xF108F85F


#define ISR_VECTOR_SIZE_WORDS 76
#define RESERVED              0


void Reset_Handler(void);
void Default_Handler(void);
void NMI_Handler(void) __attribute__((weak, alias("Default_Handler")));
void HardFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void MemManage_Handler(void) __attribute__((weak, alias("Default_Handler")));
void BusFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void UsageFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SVCall_Handler(void) __attribute__((weak, alias("Default_Handler")));
void DebugMonitor_Handler(void) __attribute__((weak, alias("Default_Handler")));
void PendSV_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SysTick_Handler(void) __attribute__((weak, alias("Default_Handler")));

/************************* Peripheral Interrupts ******************************/
/* Window Watchdog interrupt */
void WWDG_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
/* PVD Through EXTI Line interrupt */
void PVD_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
/* Tamper interrupt */
void TAMPER_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
/* RTC interrupt */
void RTC_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
/* Flash global interrupt */
void FLASH_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
/* RCC global interrupt */
void RCC_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
/* EXTI Line0 interrupt */
void EXTI0_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
/* EXTI Line1 interrupt */
void EXTI1_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
/* EXTI Line2 interrupt */
void EXTI2_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
/* EXTI Line3 interrupt */
void EXTI3_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
/* EXTI Line4 interrupt */
void EXTI4_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
/* DMA1 Channel 1 interrupt */
void DMA1_Channel1_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
/* DMA1 Channel 2 interrupt */
void DMA1_Channel2_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
/* DMA1 Channel 3 interrupt */
void DMA1_Channel3_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
/* DMA1 Channel 4 interrupt */
void DMA1_Channel4_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
/* DMA1 Channel 5 interrupt */
void DMA1_Channel5_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
/* DMA1 Channel 6 interrupt */
void DMA1_Channel6_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
/* DMA1 Channel 7 interrupt */
void DMA1_Channel7_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
/* ADC1 and ADC2 global interrupt */
void ADC1_2_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
/* USB High Priority or CAN TX interrupts */
void USB_HP_CAN1_TX_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
/* USB Low Priority or CAN RX0 interrupts */
void USB_LP_CAN1_RX0_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
/* CAN RX1 interrupt */
void CAN1_RX1_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
/* CAN SCE interrupt */
void CAN1_SCE_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
/* EXTI Line[9:5] interrupts */
void EXTI9_5_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
/* TIM1 Break interrupt */
void TIM1_BRK_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
/* TIM1 Update interrupt */
void TIM1_UP_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
/* TIM1 Trigger and Commutation interrupts */
void TIM1_TRG_COM_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
/* TIM1 Capture Compare interrupt */
void TIM1_CC_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
/* TIM2 global interrupt */
void TIM2_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
/* TIM3 global interrupt */
void TIM3_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
/* TIM4 global interrupt */
void TIM4_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
/* I2C1 event interrupt */
void I2C1_EV_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
/* I2C1 error interrupt */
void I2C1_ER_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
/* I2C2 event interrupt */
void I2C2_EV_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
/* I2C2 error interrupt */
void I2C2_ER_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
/* SPI1 global interrupt */
void SPI1_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
/* SPI2 global interrupt */
void SPI2_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
/* USART1 global interrupt */
void USART1_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
/* USART2 global interrupt */
void USART2_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
/* USART3 global interrupt */
void USART3_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
/* EXTI Line[15:10] interrupt */
void EXTI15_10_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
/* RTC alarm through EXTI line interrupt */
void RTC_Alarm_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
/* USB wakeup from suspend through EXTI line interrupt */
void USBWakeUp_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));

// Populate isr_vector section with stack pointer and isr handlers
uint32_t isr_vector[ISR_VECTOR_SIZE_WORDS] __attribute__((section(".isr_vector"))) = {
        STACK_POINTER_INIT_ADDRESS,
        (uint32_t)&Reset_Handler,
        (uint32_t)&NMI_Handler,
        (uint32_t)&HardFault_Handler,
        (uint32_t)&MemManage_Handler,
        (uint32_t)&BusFault_Handler,
        (uint32_t)&UsageFault_Handler,
        RESERVED,
        RESERVED,
        RESERVED,
        RESERVED,
        (uint32_t)&SVCall_Handler,
        (uint32_t)&DebugMonitor_Handler,
        RESERVED,
        (uint32_t)&PendSV_Handler,
        (uint32_t)&SysTick_Handler,
        (uint32_t)&WWDG_IRQHandler,
        (uint32_t)&PVD_IRQHandler,
        (uint32_t)&TAMPER_IRQHandler,
        (uint32_t)&RTC_IRQHandler,
        (uint32_t)&FLASH_IRQHandler,
        (uint32_t)&RCC_IRQHandler,
        (uint32_t)&EXTI0_IRQHandler,
        (uint32_t)&EXTI1_IRQHandler,
        (uint32_t)&EXTI2_IRQHandler,
        (uint32_t)&EXTI3_IRQHandler,
        (uint32_t)&EXTI4_IRQHandler,
        (uint32_t)&DMA1_Channel1_IRQHandler,
        (uint32_t)&DMA1_Channel2_IRQHandler,
        (uint32_t)&DMA1_Channel3_IRQHandler,
        (uint32_t)&DMA1_Channel4_IRQHandler,
        (uint32_t)&DMA1_Channel5_IRQHandler,
        (uint32_t)&DMA1_Channel6_IRQHandler,
        (uint32_t)&DMA1_Channel7_IRQHandler,
        (uint32_t)&ADC1_2_IRQHandler,
        (uint32_t)&USB_HP_CAN1_TX_IRQHandler,
        (uint32_t)&USB_LP_CAN1_RX0_IRQHandler,
        (uint32_t)&CAN1_RX1_IRQHandler,
        (uint32_t)&CAN1_SCE_IRQHandler,
        (uint32_t)&EXTI9_5_IRQHandler,
        (uint32_t)&TIM1_BRK_IRQHandler,
        (uint32_t)&TIM1_UP_IRQHandler,
        (uint32_t)&TIM1_TRG_COM_IRQHandler,
        (uint32_t)&TIM1_CC_IRQHandler,
        (uint32_t)&TIM2_IRQHandler,
        (uint32_t)&TIM3_IRQHandler,
        (uint32_t)&TIM4_IRQHandler,
        (uint32_t)&I2C1_EV_IRQHandler,
        (uint32_t)&I2C1_ER_IRQHandler,
        (uint32_t)&I2C2_EV_IRQHandler,
        (uint32_t)&I2C2_ER_IRQHandler,
        (uint32_t)&SPI1_IRQHandler,
        (uint32_t)&SPI2_IRQHandler,
        (uint32_t)&USART1_IRQHandler,
        (uint32_t)&USART2_IRQHandler,
        (uint32_t)&USART3_IRQHandler,
        (uint32_t)&EXTI15_10_IRQHandler,
        (uint32_t)&RTC_Alarm_IRQHandler,
        (uint32_t)&USBWakeUp_IRQHandler,
        RESERVED,
        RESERVED,
        RESERVED,
        RESERVED,
        RESERVED,
        RESERVED,
        RESERVED,
        (uint32_t)BootRAM
};

void Default_Handler(void)
{
        while(1);
}

extern uint32_t _etext, _sdata, _edata, _sbss, _ebss;

void main(void);

void Reset_Handler(void)
{
        // Copy data from flash to ram
        uint32_t data_size = (uint32_t)&_edata - (uint32_t)&_sdata;
        uint8_t *flash_data = (uint8_t*) &_etext;
        uint8_t *sram_data = (uint8_t*) &_sdata;
        for (uint32_t i = 0; i < data_size; i++)
        {
                sram_data[i] = flash_data[i];
        }

        // Clear bss section
        uint32_t bss_size = (uint32_t)&_ebss - (uint32_t)&_sbss;
        uint8_t *bss = (uint8_t*) &_sbss;
        for (uint32_t i = 0; i < bss_size; i++)
        {
                bss[i] = 0;
        }

        // Enable Debug
        HL_SWD_Init();
    
        // initialize Clocks with LL drivers 
        HL_ClockConfigHSE();       

        // Call main
        main();
}