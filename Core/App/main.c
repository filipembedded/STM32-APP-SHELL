#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <system_stm32f1xx.h>
#include "rcc.h"
#include "swd.h"
#include "gpio.h"
#include "stm32f1xx_ll_gpio.h"
#include "stm32f1xx_ll_rcc.h"
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_system.h"
#include "stm32f1xx_ll_exti.h"
#include "stm32f1xx_ll_cortex.h"
#include "stm32f1xx_ll_utils.h"
#include "stm32f1xx_ll_pwr.h"
#include "stm32f1xx_ll_dma.h"

#include "gpio.h"

uint32_t device_time_ms = 0;

void main(void)
{
    /************************* APB Buses Initialization  **********************/
    //LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOC);
    //LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
    /************************* Peripherals Initialization *********************/
    //GPIO
    /*
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
    */
    gpio_hl_cfg_t cfg;
    cfg.mode       = GPIO_HL_MODE_OUTPUT;
    cfg.outputType = GPIO_HL_OUTTYPE_OPENDRAIN;
    cfg.pull       = GPIO_HL_PULL_UP;
    cfg.speed      = GPIO_HL_SPEED_50MHz;

    HL_GPIO_Init_experimental(GPIO_HL_INSTANCE_PORT_C, GPIO_HL_PIN_13, &cfg);

    
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

