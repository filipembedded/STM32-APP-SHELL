/**
 * @file gpio.c
 * @brief General Purpose Input Output - High Level driver
 * @author filipembedded
 */
#include "gpio.h"
#include "util.h"
#include "bus.h"
#include "stm32f1xx_ll_gpio.h"
#include "assert.h"
#include <stdlib.h>
#include <stddef.h>



void *prvGPIO_LINE0_CALLBACK;
void *prvGPIO_LINE1_CALLBACK;
void *prvGPIO_LINE2_CALLBACK;
void *prvGPIO_LINE3_CALLBACK;
void *prvGPIO_LINE4_CALLBACK;
void *prvGPIO_LINE5_CALLBACK;
void *prvGPIO_LINE6_CALLBACK;
void *prvGPIO_LINE7_CALLBACK;
void *prvGPIO_LINE8_CALLBACK;
void *prvGPIO_LINE9_CALLBACK;
void *prvGPIO_LINE10_CALLBACK;
void *prvGPIO_LINE11_CALLBACK;
void *prvGPIO_LINE12_CALLBACK;
void *prvGPIO_LINE13_CALLBACK;
void *prvGPIO_LINE14_CALLBACK;
void *prvGPIO_LINE15_CALLBACK;

static bus_hl_apb2_peripheral_t prvHL_GPIO_GetBUS(gpio_hl_instance_t instance);
static GPIO_TypeDef* prvHL_GPIO_GetSTMPort(gpio_hl_instance_t instance);

static uint32_t prvHL_GPIO_GetEXTI_LINE_FromPin(gpio_hl_pin_t pin);
static uint32_t prvHL_GPIO_GetEXTI_PORT_FromInstance(gpio_hl_instance_t instance);

static uint32_t prvHL_GPIO_GetAF_EXTI_LINE_FromPin(gpio_hl_pin_t pin);
static uint32_t prvHL_GPIO_GetAF_EXTI_PORT_FromInstance(gpio_hl_instance_t instance);
static IRQn_Type prvHL_GPIO_GetIRQnFromPin(gpio_hl_pin_t pin);



/**************************** Function definitions ****************************/
hl_status_t HL_GPIO_Init(gpio_hl_instance_t instance,
                                      gpio_hl_pin_t pin,
                                      gpio_hl_cfg_t *cfg)
{
    /* Asserts */
    //assert(instance < GPIO_MAX_NUM_INSTANCES);

    /* Typedefs */
    LL_GPIO_InitTypeDef stmCfg;
    stmCfg.Mode       = cfg->mode;
    stmCfg.OutputType = cfg->outputType;
    stmCfg.Pull       = cfg->pull;
    stmCfg.Speed      = cfg->speed;
    stmCfg.Pin        = pin;

    /* Get BUS instance */
    bus_hl_apb2_peripheral_t bus_instance = prvHL_GPIO_GetBUS(instance);

    /* Get STM GPIO_TypeDef * instance */
    GPIO_TypeDef * stmPortInstance = prvHL_GPIO_GetSTMPort(instance);

    /* Check if clock already enabled */
    if (HL_APB2_IsEnabledClock(BUS_HL_APB2_PERIPH_GPIOD) != HL_SET)
    {
        /* Enable if not */
        if (HL_APB2_EnableClock(bus_instance) != HL_SUCCESS)
            return HL_ERROR; 
    }

    if (LL_GPIO_Init(stmPortInstance, &stmCfg) != SUCCESS)
        return HL_ERROR;

    return HL_SUCCESS;
}

void HL_GPIO_TogglePin(gpio_hl_instance_t instance, gpio_hl_pin_t pin)
{
    /* TODO: Add asserts */
    GPIO_TypeDef *stmPort = prvHL_GPIO_GetSTMPort(instance);

    LL_GPIO_TogglePin(stmPort, pin);
}


void HL_GPIO_SetPinValue(gpio_hl_instance_t instance, 
                      gpio_hl_pin_t pin, 
                      gpio_hl_pinvalue_t value)
{
    GPIO_TypeDef *stmPort = prvHL_GPIO_GetSTMPort(instance);
    if (value == GPIO_HL_SET)
        LL_GPIO_SetOutputPin(stmPort, pin);
    else 
        LL_GPIO_ResetOutputPin(stmPort, pin);
}

hl_state_t HL_GPIO_GetPinValue(gpio_hl_instance_t instance, gpio_hl_pin_t pin)
{
    GPIO_TypeDef *stmPort = prvHL_GPIO_GetSTMPort(instance);

    if (LL_GPIO_IsInputPinSet(stmPort, pin) == 1)
        return HL_SET;
    else 
        return HL_RESET;
}

hl_status_t HL_GPIO_EnableInterrupt(gpio_hl_instance_t instance,
                             gpio_hl_pin_t pin,
                             gpio_hl_edgestate_t edgeState)
{
    /*Enable the Clock for AFIO (Alternate Function I/O): 
    The EXTI line is connected to the GPIO through the AFIO peripheral, 
    so you need to enable its clock. */
    HL_APB2_EnableClock(BUS_HL_APB2_PERIPH_AFIO);

    
    uint32_t exti_af_port = prvHL_GPIO_GetAF_EXTI_PORT_FromInstance(instance);
    uint32_t exti_af_line = prvHL_GPIO_GetAF_EXTI_LINE_FromPin(pin);

    /* Remap GPIO pins to EXTI pins using AFIO */
    LL_GPIO_AF_SetEXTISource(exti_af_port, exti_af_line);

    uint32_t exti_line = prvHL_GPIO_GetEXTI_LINE_FromPin(pin);
    /*
    LL_EXTI_InitTypeDef exti_init;
    exti_init.Line_0_31   = exti_line;
    exti_init.LineCommand = ENABLE;
    exti_init.Mode        = LL_EXTI_MODE_IT;
    exti_init.Trigger     = edgeState;

    if (LL_EXTI_Init(&exti_init) == SUCCESS)
        return HL_SUCCESS;
    else 
        return HL_ERROR;
    */

    
    LL_EXTI_EnableIT_0_31(exti_line);
    if (edgeState == GPIO_HL_EDGESTATE_FALLING)
    {
        LL_EXTI_EnableFallingTrig_0_31(exti_line);
    } else {
        LL_EXTI_EnableRisingTrig_0_31(exti_line);
    }
    
}

hl_status_t HL_GPIO_ConfigureCallback(gpio_hl_instance_t instance, 
                                      gpio_hl_pin_t      pin,
                                      gpio_hl_callback_t callback)
{
    switch(pin)
    {
        case GPIO_HL_PIN_0:
            prvGPIO_LINE0_CALLBACK = callback;
            break;
        case GPIO_HL_PIN_1:
            prvGPIO_LINE1_CALLBACK = callback;
            break;
        case GPIO_HL_PIN_2:
            prvGPIO_LINE2_CALLBACK = callback;
            break;
        case GPIO_HL_PIN_3:
            prvGPIO_LINE3_CALLBACK = callback;
            break;
        case GPIO_HL_PIN_4:
            prvGPIO_LINE4_CALLBACK = callback;
            break;
        case GPIO_HL_PIN_5:
            prvGPIO_LINE5_CALLBACK = callback;
            break;
        case GPIO_HL_PIN_6:
            prvGPIO_LINE6_CALLBACK = callback;
            break;
        case GPIO_HL_PIN_7:
            prvGPIO_LINE7_CALLBACK = callback;
            break;
        case GPIO_HL_PIN_8:
            prvGPIO_LINE8_CALLBACK = callback;
            break;
        case GPIO_HL_PIN_9:
            prvGPIO_LINE9_CALLBACK = callback;
            break;
        case GPIO_HL_PIN_10:
            prvGPIO_LINE10_CALLBACK = callback;
            break;
        case GPIO_HL_PIN_11:
            prvGPIO_LINE11_CALLBACK = callback;
            break;
        case GPIO_HL_PIN_12:
            prvGPIO_LINE12_CALLBACK = callback;
            break;
        case GPIO_HL_PIN_13:
            prvGPIO_LINE13_CALLBACK = callback;
            break;
        case GPIO_HL_PIN_14:
            prvGPIO_LINE14_CALLBACK = callback;
            break;
        case GPIO_HL_PIN_15:
            prvGPIO_LINE15_CALLBACK = callback;
            break;
    }

    /* TODO: Clear all interrupt before enabling NVIC! */

    IRQn_Type EXTInum;
    EXTInum = prvHL_GPIO_GetIRQnFromPin(pin);
    NVIC_SetPriority(EXTInum, NVIC_PRIORITY_EXTI_GPIO);
    NVIC_EnableIRQ(EXTInum);
}


/************************** Static Functions **********************************/
static bus_hl_apb2_peripheral_t prvHL_GPIO_GetBUS(gpio_hl_instance_t instance)
{
    switch(instance)
    {
        case GPIO_HL_INSTANCE_PORT_A:
            return BUS_HL_APB2_PERIPH_GPIOA;
        case GPIO_HL_INSTANCE_PORT_B:
            return BUS_HL_APB2_PERIPH_GPIOB;
        case GPIO_HL_INSTANCE_PORT_C:
            return BUS_HL_APB2_PERIPH_GPIOC;
        case GPIO_HL_INSTANCE_PORT_D:
            return BUS_HL_APB2_PERIPH_GPIOD;
        case GPIO_HL_INSTANCE_PORT_E:
            return BUS_HL_APB2_PERIPH_GPIOE;
        
    }
    return 0;
}

static GPIO_TypeDef* prvHL_GPIO_GetSTMPort(gpio_hl_instance_t instance)
{
    switch(instance)
    {
        case GPIO_HL_INSTANCE_PORT_A:
            return GPIOA;
        case GPIO_HL_INSTANCE_PORT_B:
            return GPIOB;
        case GPIO_HL_INSTANCE_PORT_C:
            return GPIOC;
        case GPIO_HL_INSTANCE_PORT_D:
            return GPIOD;
        case GPIO_HL_INSTANCE_PORT_E:
            return GPIOE;
    }
    return 0;
}

static uint32_t prvHL_GPIO_GetEXTI_LINE_FromPin(gpio_hl_pin_t pin)
{
    switch(pin){
        case GPIO_HL_PIN_0:
            return LL_EXTI_LINE_0;
        case GPIO_HL_PIN_1:
            return LL_EXTI_LINE_1;
        case GPIO_HL_PIN_2:
            return LL_EXTI_LINE_2;
        case GPIO_HL_PIN_3:
            return LL_EXTI_LINE_3;
        case GPIO_HL_PIN_4:
            return LL_EXTI_LINE_4;
        case GPIO_HL_PIN_5:
            return LL_EXTI_LINE_5;
        case GPIO_HL_PIN_6:
            return LL_EXTI_LINE_6;
        case GPIO_HL_PIN_7:
            return LL_EXTI_LINE_7;
        case GPIO_HL_PIN_8:
            return LL_EXTI_LINE_8;
        case GPIO_HL_PIN_9:
            return LL_EXTI_LINE_9;
        case GPIO_HL_PIN_10:
            return LL_EXTI_LINE_10;
        case GPIO_HL_PIN_11:
            return LL_EXTI_LINE_11;
        case GPIO_HL_PIN_12:
            return LL_EXTI_LINE_12;
        case GPIO_HL_PIN_13:
            return LL_EXTI_LINE_13;
        case GPIO_HL_PIN_14:
            return LL_EXTI_LINE_14;
        case GPIO_HL_PIN_15:
            return LL_EXTI_LINE_15;
    }
}
static uint32_t prvHL_GPIO_GetEXTI_PORT_FromInstance(gpio_hl_instance_t instance)
{

}

static uint32_t prvHL_GPIO_GetAF_EXTI_LINE_FromPin(gpio_hl_pin_t pin)
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

static uint32_t prvHL_GPIO_GetAF_EXTI_PORT_FromInstance(gpio_hl_instance_t instance)
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

static IRQn_Type prvHL_GPIO_GetIRQnFromPin(gpio_hl_pin_t pin)
{
    switch(pin)
    {
        case GPIO_HL_PIN_0:
            return EXTI0_IRQn;
        case GPIO_HL_PIN_1:
            return EXTI1_IRQn;
        case GPIO_HL_PIN_2:
            return EXTI2_IRQn;
        case GPIO_HL_PIN_3:
            return EXTI3_IRQn;
        case GPIO_HL_PIN_4:
            return EXTI4_IRQn;
        case GPIO_HL_PIN_5:
        case GPIO_HL_PIN_6:
        case GPIO_HL_PIN_7:
        case GPIO_HL_PIN_8:
        case GPIO_HL_PIN_9:
            return EXTI9_5_IRQn;
        case GPIO_HL_PIN_10:
        case GPIO_HL_PIN_11:
        case GPIO_HL_PIN_12:
        case GPIO_HL_PIN_13:
        case GPIO_HL_PIN_14:
        case GPIO_HL_PIN_15:
            return EXTI15_10_IRQn;
        default:
            return NonMaskableInt_IRQn;
    }
}

/******************************** IRQs ****************************************/
void EXTI0_IRQHandler(void)
{
    gpio_hl_callback_t userCallback;
    /* Check if Interrupt is triggered by line 0 */
    if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_0))
    {
        /* Call the user callback */
        if (prvGPIO_LINE0_CALLBACK != NULL)
        {
            userCallback = prvGPIO_LINE0_CALLBACK;
            userCallback();
        }
        LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_0);
    }
}

void EXTI1_IRQHandler(void)
{
    gpio_hl_callback_t userCallback;
    /* Check if Interrupt is triggered by line 0 */
    if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_1))
    {
        /* Call the user callback */
        if (prvGPIO_LINE1_CALLBACK != NULL)
        {
            userCallback = prvGPIO_LINE1_CALLBACK;
            userCallback();
        }
        LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_1);
    } 
}

void EXTI2_IRQHandler(void)
{
    gpio_hl_callback_t userCallback;
    /* Check if Interrupt is triggered by line 0 */
    if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_2))
    {
        /* Call the user callback */
        if (prvGPIO_LINE2_CALLBACK != NULL)
        {
            userCallback = prvGPIO_LINE2_CALLBACK;
            userCallback();
        }
        LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_2);
    }
}

void EXTI3_IRQHandler(void)
{
    gpio_hl_callback_t userCallback;
    /* Check if Interrupt is triggered by line 0 */
    if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_3))
    {
        /* Call the user callback */
        if (prvGPIO_LINE3_CALLBACK != NULL)
        {
            userCallback = prvGPIO_LINE3_CALLBACK;
            userCallback();
        }
        LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_3);
    }
}

void EXTI4_IRQHandler(void)
{
    gpio_hl_callback_t userCallback;
    /* Check if Interrupt is triggered by line 0 */
    if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_4))
    {
        /* Call the user callback */
        if (prvGPIO_LINE4_CALLBACK != NULL)
        {
            userCallback = prvGPIO_LINE4_CALLBACK;
            userCallback();
        }
        LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_4);
    }
}

void EXTI9_5_IRQHandler(void)
{
    gpio_hl_callback_t userCallback;

    /* Check if Interrupt is triggered by line 5 */
    if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_5))
    {
        /* Call the user callback */
        if (prvGPIO_LINE5_CALLBACK != NULL)
        {
            userCallback = prvGPIO_LINE5_CALLBACK;
            userCallback();  // Call the function
        }
        LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_5);
    }

    /* Check if Interrupt is triggered by line 6 */
    if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_6))
    {
        /* Call the user callback */
        if (prvGPIO_LINE6_CALLBACK != NULL)
        {
            userCallback = prvGPIO_LINE6_CALLBACK;
            userCallback();  // Call the function
        }
        LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_6);
    }

    /* Check if Interrupt is triggered by line 7 */
    if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_7))
    {
        /* Call the user callback */
        if (prvGPIO_LINE7_CALLBACK != NULL)
        {
            userCallback = prvGPIO_LINE7_CALLBACK;
            userCallback();  // Call the function
        }
        LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_7);
    }

    /* Check if Interrupt is triggered by line 8 */
    if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_8))
    {
        /* Call the user callback */
        if (prvGPIO_LINE8_CALLBACK != NULL)
        {
            userCallback = prvGPIO_LINE8_CALLBACK;
            userCallback();  // Call the function
        }
        LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_8);
    }

    /* Check if Interrupt is triggered by line 9 */
    if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_9))
    {
        /* Call the user callback */
        if (prvGPIO_LINE9_CALLBACK != NULL)
        {
            userCallback = prvGPIO_LINE9_CALLBACK;
            userCallback();  // Call the function
        }
        LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_9);
    }
}


void EXTI15_10_IRQHandler(void)
{
    gpio_hl_callback_t userCallback;

    /* Check if Interrupt is triggered by line 10 */
    if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_10))
    {
        /* Call the user callback */
        if (prvGPIO_LINE10_CALLBACK != NULL)
        {
            userCallback = prvGPIO_LINE10_CALLBACK;
            userCallback();  // Call the function
        }
        LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_10);
    }

    /* Check if Interrupt is triggered by line 11 */
    if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_11))
    {
        /* Call the user callback */
        if (prvGPIO_LINE11_CALLBACK != NULL)
        {
            userCallback = prvGPIO_LINE11_CALLBACK;
            userCallback();  // Call the function
        }
        LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_11);
    }

    /* Check if Interrupt is triggered by line 12 */
    if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_12))
    {
        /* Call the user callback */
        if (prvGPIO_LINE12_CALLBACK != NULL)
        {
            userCallback = prvGPIO_LINE12_CALLBACK;
            userCallback();  // Call the function
        }
        LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_12);
    }

    /* Check if Interrupt is triggered by line 13 */
    if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_13))
    {
        /* Call the user callback */
        if (prvGPIO_LINE13_CALLBACK != NULL)
        {
            userCallback = prvGPIO_LINE13_CALLBACK;
            userCallback();  // Call the function
        }
        LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_13);
    }

    /* Check if Interrupt is triggered by line 14 */
    if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_14))
    {
        /* Call the user callback */
        if (prvGPIO_LINE14_CALLBACK != NULL)
        {
            userCallback = prvGPIO_LINE14_CALLBACK;
            userCallback();  // Call the function
        }
        LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_14);
    }

    /* Check if Interrupt is triggered by line 15 */
    if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_15))
    {
        /* Call the user callback */
        if (prvGPIO_LINE15_CALLBACK != NULL)
        {
            userCallback = prvGPIO_LINE15_CALLBACK;
            userCallback();  // Call the function
        }
        LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_15);
    }
}

