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
    /************************* Peripherals Initialization *********************/
    //GPIO
    gpio_hl_cfg_t cfg;
    cfg.mode       = GPIO_HL_MODE_OUTPUT;
    cfg.outputType = GPIO_HL_OUTTYPE_OPENDRAIN;
    cfg.pull       = GPIO_HL_PULL_UP;
    cfg.speed      = GPIO_HL_SPEED_50MHz;

    HL_GPIO_Init(GPIO_HL_INSTANCE_PORT_C, GPIO_HL_PIN_13, &cfg);

    cfg.mode = GPIO_HL_MODE_INPUT;
    cfg.pull = GPIO_HL_PULL_UP;

    HL_GPIO_Init(GPIO_HL_INSTANCE_PORT_B, GPIO_HL_PIN_12, &cfg);
 
    
    while(1)
    {
        if (HL_GPIO_GetPinValue(GPIO_HL_INSTANCE_PORT_B, GPIO_HL_PIN_12) == HL_SET)
        {
            HL_GPIO_SetPinValue(GPIO_HL_INSTANCE_PORT_C, GPIO_HL_PIN_13, HL_SET);
        } else {
            HL_GPIO_SetPinValue(GPIO_HL_INSTANCE_PORT_C, GPIO_HL_PIN_13, HL_RESET);
        }
    }
}

void SysTick_Handler(void)
{
    device_time_ms++;
}

