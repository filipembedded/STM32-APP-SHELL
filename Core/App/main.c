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



void main(void)
{
    /************************* Initialize System ******************************/
    // NOTE: This should be moved into Startup.c
    // Initialize clocks, PLL's,..
    HL_SystemInit();
    HL_ClockConfig();

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
        // LL_mDelay(1000);
        for (uint32_t i = 0; i < 7200000; i++);
    }
}



