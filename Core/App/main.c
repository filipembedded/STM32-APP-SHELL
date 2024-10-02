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

/****************************** Callbacks *************************************/
void gpio_cb_0(void);
void gpio_cb_6(void);
void gpio_cb_12(void);



uint32_t prvHL_GPIO_GetEXTI_PORT_FromInstanceTest(gpio_hl_instance_t instance)
{
    switch (instance)
    {
        case GPIO_HL_INSTANCE_PORT_A:
            return LL_GPIO_AF_EXTI_PORTA;
        case GPIO_HL_INSTANCE_PORT_B:
            return LL_GPIO_AF_EXTI_PORTB;
        case GPIO_HL_INSTANCE_PORT_C:
            return LL_GPIO_AF_EXTI_PORTC;
        case GPIO_HL_INSTANCE_PORT_D:
            return LL_GPIO_AF_EXTI_PORTD;
        case GPIO_HL_INSTANCE_PORT_E:
            return LL_GPIO_AF_EXTI_PORTE;
    }
}

uint32_t prvHL_GPIO_GetEXTI_LINE_FromPinTest(gpio_hl_pin_t pin)
{
    switch (pin)
    {
        case GPIO_HL_PIN_0:
            return LL_GPIO_AF_EXTI_LINE0;
        case GPIO_HL_PIN_1:
            return LL_GPIO_AF_EXTI_LINE1;
        case GPIO_HL_PIN_2:
            return LL_GPIO_AF_EXTI_LINE2;
        case GPIO_HL_PIN_3:
            return LL_GPIO_AF_EXTI_LINE3;
        case GPIO_HL_PIN_4:
            return LL_GPIO_AF_EXTI_LINE4;
        case GPIO_HL_PIN_5:
            return LL_GPIO_AF_EXTI_LINE5;
        case GPIO_HL_PIN_6:
            return LL_GPIO_AF_EXTI_LINE6;
        case GPIO_HL_PIN_7:
            return LL_GPIO_AF_EXTI_LINE7;
        case GPIO_HL_PIN_8:
            return LL_GPIO_AF_EXTI_LINE8;
        case GPIO_HL_PIN_9:
            return LL_GPIO_AF_EXTI_LINE9;
        case GPIO_HL_PIN_10:
            return LL_GPIO_AF_EXTI_LINE10;
        case GPIO_HL_PIN_11:
            return LL_GPIO_AF_EXTI_LINE11;
        case GPIO_HL_PIN_12:
            return LL_GPIO_AF_EXTI_LINE12;
        case GPIO_HL_PIN_13:
            return LL_GPIO_AF_EXTI_LINE13;
        case GPIO_HL_PIN_14:
            return LL_GPIO_AF_EXTI_LINE14;
        case GPIO_HL_PIN_15:
            return LL_GPIO_AF_EXTI_LINE15;

    }
}


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
    HL_GPIO_SetPinValue(GPIO_HL_INSTANCE_PORT_C, GPIO_HL_PIN_13, GPIO_HL_SET);

    cfg.mode = GPIO_HL_MODE_INPUT;
    cfg.pull = GPIO_HL_PULL_UP;

    HL_GPIO_Init(GPIO_HL_INSTANCE_PORT_B, GPIO_HL_PIN_12, &cfg);
    HL_GPIO_Init(GPIO_HL_INSTANCE_PORT_A, GPIO_HL_PIN_0, &cfg);
    HL_GPIO_Init(GPIO_HL_INSTANCE_PORT_A, GPIO_HL_PIN_6, &cfg);
    HL_GPIO_Init(GPIO_HL_INSTANCE_PORT_B, GPIO_HL_PIN_1, &cfg);


    HL_GPIO_EnableInterrupt(GPIO_HL_INSTANCE_PORT_B, GPIO_HL_PIN_12, 
                                                    GPIO_HL_EDGESTATE_FALLING);
    HL_GPIO_ConfigureCallback(GPIO_HL_INSTANCE_PORT_B, GPIO_HL_PIN_12, gpio_cb_12);
    
    HL_GPIO_EnableInterrupt(GPIO_HL_INSTANCE_PORT_A, GPIO_HL_PIN_0, 
                                                    GPIO_HL_EDGESTATE_FALLING);
    HL_GPIO_ConfigureCallback(GPIO_HL_INSTANCE_PORT_A, GPIO_HL_PIN_0, gpio_cb_0);

    HL_GPIO_EnableInterrupt(GPIO_HL_INSTANCE_PORT_A, GPIO_HL_PIN_6, 
                                                    GPIO_HL_EDGESTATE_FALLING);
    HL_GPIO_ConfigureCallback(GPIO_HL_INSTANCE_PORT_A, GPIO_HL_PIN_6, gpio_cb_6);

    HL_GPIO_EnableInterrupt(GPIO_HL_INSTANCE_PORT_B, GPIO_HL_PIN_1, 
                                                    GPIO_HL_EDGESTATE_FALLING);
    HL_GPIO_ConfigureCallback(GPIO_HL_INSTANCE_PORT_B, GPIO_HL_PIN_1, gpio_cb_6);


    /*************************** DEBUG SECTION ********************************/
  //  if (HL_APB2_EnableClock(BUS_HL_APB2_PERIPH_AFIO) != HL_SUCCESS)
  //  {
  //      for (;;);
  //  }
//
  //  uint32_t exti_port = prvHL_GPIO_GetEXTI_PORT_FromInstanceTest(GPIO_HL_INSTANCE_PORT_B);
  //  uint32_t exti_line = prvHL_GPIO_GetEXTI_LINE_FromPinTest(GPIO_HL_PIN_12);
//
  //  /* Remap GPIO pins to EXTI pins using AFIO */
  //  LL_GPIO_AF_SetEXTISource(exti_port, exti_line); 
//
  //  LL_EXTI_InitTypeDef exti_init;
  //  exti_init.Line_0_31   = exti_line;
  //  exti_init.LineCommand = ENABLE;
  //  exti_init.Mode        = LL_EXTI_MODE_IT;
  //  exti_init.Trigger     = GPIO_HL_EDGESTATE_FALLING;
//
  //  if (LL_EXTI_Init(&exti_init) != SUCCESS)
  //  {
  //      for (;;);
  //  }




    /**************************************************************************/

    while(1)
    {

    
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

