/**
 * @file rcc.c
 * @brief Reset and Clock Control - High-Layer driver
 * @author filipembedded
 * 
 */
#include "rcc.h"
#include "stm32f1xx.h"

void HL_SystemInit()
{
    /************************* SWD debugging enable ***************************/
    // Enable AFIO clock
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
    // AF remap and debug I/O register
    AFIO->MAPR   &= ~AFIO_MAPR_SWJ_CFG;
    AFIO->MAPR   |= AFIO_MAPR_SWJ_CFG_1;


}

void HL_ClockConfig()
{
    /************************* HSE Clock Configuration ************************/
    // Counter variable
    volatile uint32_t counter = 0;
    // Clear the HSEBYP bit - not using the clock module, but just Crystal
    RCC->CR &= ~RCC_CR_HSEBYP;
    // Set the HSEON bit to enable External Clock
    RCC->CR |= RCC_CR_HSEON;
    // Check if External clock is ready
    do{
        counter++;
    }
    while ((RCC->CR & RCC_CR_HSERDY) == 0 && (counter < RCC_HSE_CHECK_VALUE));

    if((RCC->CR & RCC_CR_HSERDY)!=0) 
    {
        /*************************** FLASH Setup ******************************/
        // If HSE is ready, prepare FLASH
        FLASH->ACR |= FLASH_ACR_PRFTBE;
        // For 72MHz clock freq - two wait state latency is needed
        // Clear all FLASH_ACR_LATENCY bits
        FLASH->ACR &= ~FLASH_ACR_LATENCY;
        // Set two wait states latency
        FLASH->ACR |= FLASH_ACR_LATENCY_1;

        /************************** AHB Prescaler Setup ***********************/
        // Clear CFGR reg HPRE bits - No Clock division for APB2 bus (72MHz)
        RCC->CFGR &= ~RCC_CFGR_HPRE;

        /************************** APB1 Bus Clock Setup **********************/
        // Clear CFGR reg PPRE1 bits - Clock division is 2 for APB1 bus (max 36MHz)
        RCC->CFGR &= ~RCC_CFGR_PPRE1;
        // Set bit for division 2
        RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;

        /************************** APB2 Bus Clock Setup **********************/
        // Clear CFGR reg PPRE2 bits
        RCC->CFGR &= ~RCC_CFGR_PPRE2;
        // Set bit for division 1
        RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;

        /************************** ADC Prescaler Setup ***********************/
        // Clear ADCPRE bits
        RCC->CFGR &= ~RCC_CFGR_ADCPRE;
        // Set division 6 - APB2/6 = 72MHz/6 = 12MHz
        RCC->CFGR |= RCC_CFGR_ADCPRE_DIV6;

        /************************** USB Prescaler Setup ***********************/
        // Clear USBPRE bits
        RCC->CFGR &= ~RCC_CFGR_USBPRE;
        // Clear bit for no division
        RCC->CFGR &= ~RCC_CFGR_PLLXTPRE;
        // Set PLLX prescaler to 1
        RCC->CFGR |= RCC_CFGR_PLLSRC;

        /********************* PLLmul Setup - Main Clock **********************/
        // Clear PLLMULL bits
        RCC->CFGR &= ~RCC_CFGR_PLLMULL;
        // Set PLLMULL for 9x multiplication to achieve 72MHz clock 
        RCC->CFGR |= RCC_CFGR_PLLMULL9;

        /***************************** Enable PLL *****************************/
        // Set PLL to on
        RCC->CR |= RCC_CR_PLLON;
        // Wait until it is ready
        while ((RCC->CR & RCC_CR_PLLRDY) == 0);

        /******************** Set PLL as a Main Clock Source ******************/
        // Clear Clock Switch register
        RCC->CFGR &= ~RCC_CFGR_SW;
        // Set PLL as a main source 
        RCC->CFGR |= RCC_CFGR_SW_PLL;
        // Wait until PLL is used as a system clock
        while ((RCC->CFGR &RCC_CFGR_SWS) != 0x08);

        
    }
}