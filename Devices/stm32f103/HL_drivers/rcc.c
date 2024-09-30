/**
 * @file rcc.c
 * @brief Reset and Clock Control - High-Layer driver
 * @author filipembedded
 * 
 */
#include "rcc.h"
#include "stm32f1xx.h"
#include "stm32f1xx_ll_gpio.h"
#include "stm32f1xx_ll_rcc.h"
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_system.h"
#include "stm32f1xx_ll_exti.h"
#include "stm32f1xx_ll_cortex.h"
#include "stm32f1xx_ll_utils.h"
#include "stm32f1xx_ll_pwr.h"
#include "stm32f1xx_ll_dma.h"

void HL_SystemInit()
{
    /************************* SWD debugging enable ***************************/
    // Enable AFIO clock
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
    // AF remap and debug I/O register
    AFIO->MAPR   &= ~AFIO_MAPR_SWJ_CFG;
    AFIO->MAPR   |= AFIO_MAPR_SWJ_CFG_1;


}

void HL_ClockConfigHSE(void)
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