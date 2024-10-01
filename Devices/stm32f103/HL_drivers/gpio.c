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


/**************************** Function definitions ****************************/
hl_status_t HL_GPIO_Init(gpio_hl_instance_t instance,
                         gpio_hl_pin_t pin,
                         gpio_hl_cfg_t *cfg)
{
    /* Asserts */
    assert(instance < GPIO_MAX_NUM_INSTANCES);

    /* Statuses */
    hl_status_t status;
    ErrorStatus eStatus;

    /* Typedefs */
    LL_GPIO_InitTypeDef stmCfg;
    stmCfg.Mode       = cfg->mode;
    stmCfg.OutputType = cfg->outputType;
    stmCfg.Pull       = cfg->pull;
    stmCfg.Speed      = cfg->speed;
    
    /* Initialization of Peripheral clock */
    switch(instance)
    {
        case GPIO_HL_INSTANCE_PORT_A:
            if (HL_APB2_IsEnabledClock(BUS_HL_APB2_PERIPH_GPIOA) != HL_SET)
            {
                status = HL_APB2_EnableClock(BUS_HL_APB2_PERIPH_GPIOA);
                eStatus = LL_GPIO_Init(GPIOA, &stmCfg);
                if (status != HL_SUCCESS || eStatus != SUCCESS)
                    return HL_ERROR;
                else 
                    return HL_SUCCESS;
            }
            break;
        case GPIO_HL_INSTANCE_PORT_B:
            if (HL_APB2_IsEnabledClock(BUS_HL_APB2_PERIPH_GPIOB) != HL_SET)
            {
                status = HL_APB2_EnableClock(BUS_HL_APB2_PERIPH_GPIOB);
                eStatus = LL_GPIO_Init(GPIOB, &stmCfg);
                if (status != HL_SUCCESS || eStatus != SUCCESS)
                    return HL_ERROR;
                else 
                    return HL_SUCCESS;
            }
            break;
        case GPIO_HL_INSTANCE_PORT_C:
            if (HL_APB2_IsEnabledClock(BUS_HL_APB2_PERIPH_GPIOC) != HL_SET)
            {
                status = HL_APB2_EnableClock(BUS_HL_APB2_PERIPH_GPIOC);
                eStatus = LL_GPIO_Init(GPIOC, &stmCfg);
                if (status != HL_SUCCESS || eStatus != SUCCESS)
                    return HL_ERROR;
                else 
                    return HL_SUCCESS;
            }
            break;
        case GPIO_HL_INSTANCE_PORT_D:
            if (HL_APB2_IsEnabledClock(BUS_HL_APB2_PERIPH_GPIOD) != HL_SET)
            {
                status = HL_APB2_EnableClock(BUS_HL_APB2_PERIPH_GPIOD);
                eStatus = LL_GPIO_Init(GPIOD, &stmCfg);
                if (status != HL_SUCCESS || eStatus != SUCCESS)
                    return HL_ERROR;
                else 
                    return HL_SUCCESS;
            }
            break;
        case GPIO_HL_INSTANCE_PORT_E:
            if (HL_APB2_IsEnabledClock(BUS_HL_APB2_PERIPH_GPIOE) != HL_SET)
            {
                status = HL_APB2_EnableClock(BUS_HL_APB2_PERIPH_GPIOE);
                eStatus = LL_GPIO_Init(GPIOE, &stmCfg);
                if (status != HL_SUCCESS || eStatus != SUCCESS)
                    return HL_ERROR;
                else 
                    return HL_SUCCESS;
            }
            break;
        default:
            return HL_ERROR;   
    }
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
}



/* Test function */

hl_status_t HL_GPIO_Init_experimental(gpio_hl_instance_t instance,
                                      gpio_hl_pin_t pin,
                                      gpio_hl_cfg_t *cfg)
{
    /* Asserts */
    assert(instance < GPIO_MAX_NUM_INSTANCES);

    /* Typedefs */
    LL_GPIO_InitTypeDef stmCfg;
    stmCfg.Mode       = cfg->mode;
    stmCfg.OutputType = cfg->outputType;
    stmCfg.Pull       = cfg->pull;
    stmCfg.Speed      = cfg->speed;

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