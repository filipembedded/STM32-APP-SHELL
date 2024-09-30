/**
 * @file swd.c
 * @brief Serial Wire Debug - High-Layer driver
 * @author filipembedded
 * 
 */

#include "swd.h"
#include "stm32f1xx.h"
#include "stm32f1xx_ll_rcc.h"

void HL_SWD_Init()
{
    /************************* SWD debugging enable ***************************/
    // Enable AFIO clock
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
    // AF remap and debug I/O register
    AFIO->MAPR   &= ~AFIO_MAPR_SWJ_CFG;
    AFIO->MAPR   |= AFIO_MAPR_SWJ_CFG_1;
}