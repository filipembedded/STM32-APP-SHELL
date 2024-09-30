#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <system_stm32f1xx.h>
#include "rcc.h"
#include "stm32f1xx_ll_gpio.h"
#include "stm32f1xx_ll_rcc.h"
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_system.h"
#include "stm32f1xx_ll_exti.h"
#include "stm32f1xx_ll_cortex.h"
#include "stm32f1xx_ll_utils.h"
#include "stm32f1xx_ll_pwr.h"
#include "stm32f1xx_ll_dma.h"

uint32_t device_time_ms = 0;

#define RCC_HSE_CHECK_VALUE 0x100000

void LL_ClockConfig(void);


void main(void)
{
    /************************* Initialize System ******************************/
    // NOTE: This should be moved into Startup.c
    // Enable Debug
    HL_SystemInit();

    // initialize Clocks with LL drivers 
    LL_ClockConfig();

    /************************* APB Buses Initialization  **********************/
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOC);
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
    /************************* Peripherals Initialization *********************/
    //GPIO

    LL_GPIO_InitTypeDef cfg;
    cfg.Pin  = LL_GPIO_PIN_13;
    cfg.Mode = LL_GPIO_MODE_OUTPUT;
    cfg.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
    cfg.Pull       = LL_GPIO_PULL_UP;
    cfg.Speed      = LL_GPIO_SPEED_FREQ_HIGH;

    if (LL_GPIO_Init(GPIOC, &cfg) == ERROR)
    {
        for(;;);
    } 
    
    while(1)
    {
        LL_GPIO_TogglePin(GPIOC, LL_GPIO_PIN_13);
        LL_mDelay(100);
        //for (uint32_t i = 0; i < 7200000; i++);
    }
}

void SysTick_Handler(void)
{
    device_time_ms++;
}


void LL_ClockConfig(void)
{
    /************************* HSE Clock Configuration ************************/
    // Clear the HSEBYP bit - not using the clock module, but just Crystal
    LL_RCC_HSE_DisableBypass();
    
    // Set the HSEON bit to enable External Clock
    LL_RCC_HSE_Enable();
    
    // Wait for HSE to be ready
    uint32_t counter = 0;
    while (LL_RCC_HSE_IsReady() != 1 && counter < RCC_HSE_CHECK_VALUE)
    {
        counter++;
    }

    if (LL_RCC_HSE_IsReady() == 1)
    {
        /*************************** FLASH Setup ******************************/
        // If HSE is ready, prepare FLASH
        LL_FLASH_SetLatency(LL_FLASH_LATENCY_2);
        LL_FLASH_EnablePrefetch();

        /************************** AHB Prescaler Setup ***********************/
        // No Clock division for AHB bus (HCLK)
        LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);

        /************************** APB1 Bus Clock Setup **********************/
        // Clock division is 2 for APB1 bus (max 36MHz)
        LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);

        /************************** APB2 Bus Clock Setup **********************/
        // No Clock division for APB2 bus (max 72MHz)
        LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
        
        /********************* PLL Configuration - Main Clock **********************/
        // Configure PLL: PLL source is HSE, multiplier is 9 (8MHz * 9 = 72MHz)
        LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE_DIV_1, LL_RCC_PLL_MUL_9);

        /***************************** Enable PLL *****************************/
        // Enable PLL
        LL_RCC_PLL_Enable();
        // Wait until it is ready
        while (LL_RCC_PLL_IsReady() != 1);

        /******************** Set PLL as a Main Clock Source ******************/
        // Set PLL as a main source
        LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
        // Wait until PLL is used as a system clock
        while (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL);

        // Configure SysTick to generate interrupts every 1ms
        LL_Init1msTick(72000000); // Use the correct HCLK value for your configuration
        LL_SYSTICK_EnableIT(); // Enable SysTick interrupt
    }
}


