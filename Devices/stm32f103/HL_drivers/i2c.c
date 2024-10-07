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
static hl_status_t prvI2C_WaitFlagSet(I2C_TypeDef *I2C, 
                                      uint32_t (*flagCheckFunc)(I2C_TypeDef *),
                                      uint32_t status,
                                      uint32_t timeout_ms);

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
    I2C_TypeDef *stmInstance = prvI2C_GetSTMInstance(instance);
    uint32_t retransmissionCnt = 0;

    /* Send Start Condition */
    LL_I2C_GenerateStartCondition(stmInstance);

    /* Wait for the bus to be free before start */
    if (prvI2C_WaitFlagSet(stmInstance, LL_I2C_IsActiveFlag_BUSY, RESET,
                                                    timeout_ms) != HL_SUCCESS)
    {
        return HL_ERROR;
    }

    /* Generate start condition */
    LL_I2C_GenerateStartCondition(stmInstance);

    /* Wait for the start condition to be generated */
    if (prvI2C_WaitFlagSet(stmInstance, LL_I2C_IsActiveFlag_SB, SET,
                                            timeout_ms) != HL_SUCCESS)
    {
        return HL_ERROR;
    }

    /* Send slave address (write mode) */
    LL_I2C_TransmitData8(stmInstance, (slaveAddress << 1) & ~0x01);

    /* Wait for address to be sent (ADDR flag) */
    if (prvI2C_WaitFlagSet(stmInstance, LL_I2C_IsActiveFlag_ADDR, SET, 
                                            timeout_ms) != HL_SUCCESS)
    {
        return HL_ERROR;
    }

    /* Clear ADDR flag by reading SR1 and SR2 */
    LL_I2C_ClearFlag_ADDR(stmInstance);

    /* Transmit data */
    for (uint32_t i = 0; i < size; i++)
    {
        /* Wait for transmit buffer to be empty */
        if (prvI2C_WaitFlagSet(stmInstance, LL_I2C_IsActiveFlag_TXE, SET,
                                            timeout_ms) != HL_SUCCESS)
        {
            return HL_ERROR;
        }

        /* Send data byte */
        LL_I2C_TransmitData8(stmInstance, pData[i]);
    }

    /* Wait until the data transfer finishes (TXE and BTF flags) */
    if (prvI2C_WaitFlagSet(stmInstance, LL_I2C_IsActiveFlag_BTF, SET,
                                            timeout_ms) != HL_SUCCESS)
    {
        return HL_ERROR;
    }

    /* Generate STOP condition */
    LL_I2C_GenerateStopCondition(stmInstance);

    return HL_SUCCESS;
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

static hl_status_t prvI2C_WaitFlagSet(I2C_TypeDef *I2Cx, 
                                      uint32_t (*flagCheckFunc)(I2C_TypeDef *),
                                      uint32_t status,
                                      uint32_t timeout_ms)
{
    /* Get current device time */
    uint32_t dev_time = HL_GetDeviceTime();
    while (flagCheckFunc(I2Cx) != status)
    {
        /* Check for timeout */
        if ((HL_GetDeviceTime() - dev_time) >= timeout_ms)
            return HL_ERROR;
    }
    return HL_SUCCESS;
}