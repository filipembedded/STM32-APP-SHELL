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


static bus_hl_apb2_peripheral_t prvHL_GPIO_GetBUS(gpio_hl_instance_t instance);
static GPIO_TypeDef* prvHL_GPIO_GetSTMPort(gpio_hl_instance_t instance);

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



/* Test function */

