#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <system_stm32f1xx.h>
#include "rcc.h"
#include "swd.h"
#include "gpio.h"
#include "bus.h"
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

uint32_t tim2_dev_time = 0;

/****************************** Callbacks *************************************/
void gpio_cb_0(void);
void gpio_cb_6(void);
void gpio_cb_12(void);
void gpio_cb_15(void);


void GPIO_Config(void)
{
    // Enable clock for GPIOB
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOB);

    // Configure PB12 as input
    LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_12, LL_GPIO_MODE_INPUT);
    LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_12, LL_GPIO_PULL_UP);  // Configure pull-up resistor
}

void EXTI_ConfigHL()
{
    //HL_GPIO_EnableFuckingInterrupt(GPIO_HL_INSTANCE_PORT_B, GPIO_HL_PIN_12, GPIO_HL_EDGESTATE_FALLING);
    //enableFuckinInterrupt();
    HL_GPIO_EnableInterrupt(GPIO_HL_INSTANCE_PORT_B, GPIO_HL_PIN_12, GPIO_HL_EDGESTATE_FALLING);    
    HL_GPIO_ConfigureCallback(GPIO_HL_INSTANCE_PORT_B, GPIO_HL_PIN_12, gpio_cb_12);
}

void main(void)
{

    /************************* Initialize device time *************************/
    HL_UTIL_InitDeviceTimer2();

    /************************* Peripherals Initialization *********************/
    //GPIO
    gpio_hl_cfg_t cfg;
    cfg.mode       = GPIO_HL_MODE_OUTPUT;
    cfg.outputType = GPIO_HL_OUTTYPE_OPENDRAIN;
    cfg.pull       = GPIO_HL_PULL_UP;
    cfg.speed      = GPIO_HL_SPEED_50MHz;
    HL_GPIO_Init(GPIO_HL_INSTANCE_PORT_C, GPIO_HL_PIN_13, &cfg);
    HL_GPIO_SetPinValue(GPIO_HL_INSTANCE_PORT_C, GPIO_HL_PIN_13, GPIO_HL_SET);
// 
    cfg.outputType = GPIO_HL_OUTTYPE_PUSHPULL;
    cfg.mode = GPIO_HL_MODE_INPUT;
    cfg.pull = GPIO_HL_PULL_UP;
// 
    HL_GPIO_Init(GPIO_HL_INSTANCE_PORT_B, GPIO_HL_PIN_12, &cfg);
    HL_GPIO_Init(GPIO_HL_INSTANCE_PORT_A, GPIO_HL_PIN_0, &cfg);
    HL_GPIO_Init(GPIO_HL_INSTANCE_PORT_A, GPIO_HL_PIN_7, &cfg);
    HL_GPIO_Init(GPIO_HL_INSTANCE_PORT_B, GPIO_HL_PIN_1, &cfg);
    HL_GPIO_Init(GPIO_HL_INSTANCE_PORT_C, GPIO_HL_PIN_15, &cfg);


    HL_GPIO_EnableInterrupt(GPIO_HL_INSTANCE_PORT_B, GPIO_HL_PIN_12, 
                                                    GPIO_HL_EDGESTATE_FALLING);
    HL_GPIO_ConfigureCallback(GPIO_HL_INSTANCE_PORT_B, GPIO_HL_PIN_12, gpio_cb_12);
    
    HL_GPIO_EnableInterrupt(GPIO_HL_INSTANCE_PORT_A, GPIO_HL_PIN_0, 
                                                    GPIO_HL_EDGESTATE_FALLING);
    HL_GPIO_ConfigureCallback(GPIO_HL_INSTANCE_PORT_A, GPIO_HL_PIN_0, gpio_cb_0);

    HL_GPIO_EnableInterrupt(GPIO_HL_INSTANCE_PORT_A, GPIO_HL_PIN_7, 
                                                    GPIO_HL_EDGESTATE_FALLING);
    HL_GPIO_ConfigureCallback(GPIO_HL_INSTANCE_PORT_A, GPIO_HL_PIN_7, gpio_cb_6);

    HL_GPIO_EnableInterrupt(GPIO_HL_INSTANCE_PORT_B, GPIO_HL_PIN_1, 
                                                    GPIO_HL_EDGESTATE_FALLING);
    HL_GPIO_ConfigureCallback(GPIO_HL_INSTANCE_PORT_B, GPIO_HL_PIN_1, gpio_cb_6);

    HL_GPIO_EnableInterrupt(GPIO_HL_INSTANCE_PORT_C, GPIO_HL_PIN_15, 
                                                    GPIO_HL_EDGESTATE_FALLING);
    HL_GPIO_ConfigureCallback(GPIO_HL_INSTANCE_PORT_C, GPIO_HL_PIN_15, gpio_cb_15);
    
    
    
    /* Last wake time */
    tim2_dev_time = HL_GetDeviceTime();

    while(1)
    {
        if ((HL_GetDeviceTime()-tim2_dev_time) >= 500)
        {
            HL_GPIO_TogglePin(GPIO_HL_INSTANCE_PORT_C, GPIO_HL_PIN_13);
            tim2_dev_time = HL_GetDeviceTime();
        }
    }
}


void SysTick_Handler(void)
{
    device_time_ms++;
}


void gpio_cb_0(void)
{
    for (int i = 0; i < 10; i++)
    {
        HL_GPIO_TogglePin(GPIO_HL_INSTANCE_PORT_C, GPIO_HL_PIN_13);
        LL_mDelay(50);
    }
}

void gpio_cb_6(void)
{
    for (int i = 0; i < 10; i++)
    {
        HL_GPIO_TogglePin(GPIO_HL_INSTANCE_PORT_C, GPIO_HL_PIN_13);
        LL_mDelay(50);
    }   
}

void gpio_cb_12(void)
{
    for (int i = 0; i < 10; i++)
    {
        HL_GPIO_TogglePin(GPIO_HL_INSTANCE_PORT_C, GPIO_HL_PIN_13);
        LL_mDelay(50);
    }
}

void gpio_cb_15(void)
{
    for (int i = 0; i < 20; i++)
    {
        HL_GPIO_TogglePin(GPIO_HL_INSTANCE_PORT_C, GPIO_HL_PIN_13);
        LL_mDelay(25);
    }
}