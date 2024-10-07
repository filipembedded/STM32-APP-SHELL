/**
 * @file i2c.c
 * @brief High-Layer driver for I2C
 * @author filipembedded
 */
#include <stdlib.h>
#include "i2c.h"
#include "gpio.h"
#include "bus.h"
#include "stm32f1xx_ll_gpio.h"


/*************************** Static function declarations *********************/
static void prvI2C_InitPins(i2c_hl_instance_t instance);
static void prvI2C_InitConfigStruct(i2c_hl_cfg_t *cfg);
static I2C_TypeDef* prvI2C_GetSTMInstance(i2c_hl_instance_t instance);

/********************************** Definitions *******************************/
 
hl_status_t HL_I2C_Init(i2c_hl_instance_t instance, uint32_t clockSpeed)
{
    assert_i2c_instance(instance);

    /* Initialize peripheral clock for I2C & GPIOB */
    if (HL_APB2_IsEnabledClock(BUS_HL_APB2_PERIPH_GPIOB) != HL_SET)
    {
        if (HL_APB2_EnableClock(BUS_HL_APB2_PERIPH_GPIOB) != HL_SUCCESS)
            return HL_ERROR;
    }
    
    if (instance == I2C_HL_INSTANCE_1)
    {
        if (HL_APB1_IsEnabledClock(BUS_HL_APB1_PERIPH_I2C1) != HL_SET)
        {
            if (HL_APB1_EnableClock(BUS_HL_APB1_PERIPH_I2C1) != HL_SUCCESS)
                return HL_ERROR;
        }
    } else {
        if (HL_APB1_IsEnabledClock(BUS_HL_APB1_PERIPH_I2C1) != HL_SET)
        {
            if (HL_APB1_EnableClock(BUS_HL_APB1_PERIPH_I2C2) != HL_SUCCESS)
                return HL_ERROR;
        }
    }

    /* Initialize pins for certain I2C instance */
    prvI2C_InitPins(instance);

    /* Configure and Initialize I2C peripheral */
    i2c_hl_cfg_t hl_cfg;
    prvI2C_InitConfigStruct(&hl_cfg);

    LL_I2C_InitTypeDef cfg = {0};
    cfg.ClockSpeed      = clockSpeed;
    cfg.PeripheralMode  = hl_cfg.periphMode;
    cfg.DutyCycle       = hl_cfg.dutyCycle;
    cfg.OwnAddress1     = hl_cfg.ownAddress;
    cfg.OwnAddrSize     = hl_cfg.ownAddrSize;
    cfg.TypeAcknowledge = hl_cfg.acknowledge;

    I2C_TypeDef *stmInstance;
    stmInstance = prvI2C_GetSTMInstance(instance);

    LL_I2C_Init(stmInstance, &cfg);
    LL_I2C_Enable(stmInstance);

    return HL_SUCCESS;
}

void HL_I2C_DeInit(i2c_hl_instance_t instance)
{
    assert_i2c_instance(instance);

    I2C_TypeDef *stmInstance;
    stmInstance = prvI2C_GetSTMInstance(instance);

    LL_I2C_DeInit(stmInstance);
}

hl_status_t HL_I2C_MasterTransmit(i2c_hl_instance_t instance,
                                  uint32_t slaveAddress,
                                  uint8_t  *pData,
                                  uint32_t size,
                                  uint32_t timeout_ms)
{
    assert_instance(instance);
    /* Variables */
    I2C_TypeDef *stmInstance = NULL;
    uint32_t retransmissionCnt = 0;

    /* Send Start Condition */
    LL_I2C_GenerateStartCondition(stmInstance);

    /* Wait for SB (Start Bit) flag to be set */
    /*...*/


}

hl_status_t HL_I2C_MasterReceive(i2c_hl_instance_t instance,
                                  uint32_t slaveAddress,
                                  uint8_t  *pData,
                                  uint32_t size,
                                  uint32_t timeout_ms)
{

}



/**************************** Static functions ********************************/
static void prvI2C_InitPins(i2c_hl_instance_t instance)
{
    assert_i2c_instance(instance);

    gpio_hl_cfg_t cfg;
    
    switch (instance)
    {
    case I2C_HL_INSTANCE_1:
        cfg.mode       = GPIO_HL_MODE_ALTERNATE;
        cfg.outputType = GPIO_HL_OUTTYPE_OPENDRAIN;
        cfg.speed      = GPIO_HL_SPEED_50MHz;
        HL_GPIO_Init(GPIO_HL_INSTANCE_PORT_B, GPIO_HL_PIN_6, &cfg);
        HL_GPIO_Init(GPIO_HL_INSTANCE_PORT_B, GPIO_HL_PIN_7, &cfg);

        break;
    case I2C_HL_INSTANCE_2:
        cfg.mode       = GPIO_HL_MODE_ALTERNATE;
        cfg.outputType = GPIO_HL_OUTTYPE_OPENDRAIN;
        cfg.speed      = GPIO_HL_SPEED_50MHz;
        HL_GPIO_Init(GPIO_HL_INSTANCE_PORT_B, GPIO_HL_PIN_10, &cfg);
        HL_GPIO_Init(GPIO_HL_INSTANCE_PORT_B, GPIO_HL_PIN_11, &cfg);

        break;
    default:
        break;
    }
}

static void prvI2C_InitConfigStruct(i2c_hl_cfg_t *cfg)
{
    cfg->periphMode  = I2C_HL_MODE_I2C;
    cfg->ownAddress  = I2C_HL_OWN_ADDRESS;
    cfg->acknowledge = I2C_HL_ACK;
    cfg->dutyCycle   = I2C_HL_DUTYCYCLE_2; 
    cfg->ownAddrSize = I2C_HL_OWNADDR_7BIT;
}

static I2C_TypeDef* prvI2C_GetSTMInstance(i2c_hl_instance_t instance)
{
    if (instance == I2C_HL_INSTANCE_1)
        return I2C1;
    
    if (instance == I2C_HL_INSTANCE_2)
        return I2C2;
}