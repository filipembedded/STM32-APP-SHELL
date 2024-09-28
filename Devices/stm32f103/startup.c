#include <stdint.h>

#define SRAM_START (0x20000000U)
#define SRAM_SIZE (20U * 1024U)
#define SRAM_END (SRAM_START + SRAM_SIZE)
#define STACK_POINTER_INIT_ADDRESS (SRAM_END)

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

/* Window Watchdog interrupt */
void WWDG_Handler(void) __attribute__((weak, alias("Default_Handler")));
/* PVD Through EXTI Line interrupt */
void PVD_Handler(void) __attribute__((weak, alias("Default_Handler")));
/* Tamper interrupt */
void TAMPER_Interrupt(void) __attribute__((weak, alias("Default_Handler")));
/* RTC interrupt */
void RTC_Handler(void) __attribute__((weak, alias("Default_Handler")));
/* Flash global interrupt */
void FLASH_Handler(void) __attribute__((weak, alias("Default_Handler")));
/* RCC global interrupt */
void RCC_Handler(void) __attribute__((weak, alias("Default_Handler")));
/* EXTI Line0 interrupt */
void EXTI0_Handler(void) __attribute__((weak, alias("Default_Handler")));

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

        // Call main
        main();
}