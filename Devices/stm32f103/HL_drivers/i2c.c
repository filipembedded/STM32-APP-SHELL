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
        if (HL_APB1_IsEnabledClock(BUS_HL_APB1_PERIPH_I2C2) != HL_SET)
        {
            if (HL_APB1_EnableClock(BUS_HL_APB1_PERIPH_I2C2) != HL_SUCCESS)
                return HL_ERROR;
        }
    }

    /* Initialize pins for certain I2C instance */
    //prvI2C_InitPins(instance);

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


    /* Wait for the bus to be free before start - Is SR2 register busy? */
    if (prvI2C_WaitFlagSet(stmInstance, LL_I2C_IsActiveFlag_BUSY, RESET,
                                                    timeout_ms) != HL_SUCCESS)
    {
        return HL_ERROR;
    }

    /* Generate start condition - Set SB bit in SR1 register */
    LL_I2C_GenerateStartCondition(stmInstance);

    /* Wait for the start condition to be generated - Is SB bit set in SR1? */
    if (prvI2C_WaitFlagSet(stmInstance, LL_I2C_IsActiveFlag_SB, SET,
                                            timeout_ms) != HL_SUCCESS)
    {
        return HL_ERROR;
    }

    /* Send slave address (write mode) - Write data to the DR register with
        write bit - 0 */
    LL_I2C_TransmitData8(stmInstance, (slaveAddress << 1) & ~0x01);

    /* Wait for address to be sent (ADDR flag) - Is ADDR flag set in SR1? */
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
    assert_instance(instance);

    uint32_t tmpSize = size;

    /* Variables */
    I2C_TypeDef *stmInstance = prvI2C_GetSTMInstance(instance);

    /* Wait until the bus is free - Check BUSY flag in SR2 */
    if (prvI2C_WaitFlagSet(stmInstance, LL_I2C_IsActiveFlag_BUSY, RESET,
                                        timeout_ms) != HL_SUCCESS)
    {
        return HL_ERROR;
    }

    /* Generate START condition - Set SB bit in SR1 register */
    LL_I2C_GenerateStartCondition(stmInstance);

    /* Wait until start condition is being generated - Is SB bit set in SR1 */
    if (prvI2C_WaitFlagSet(stmInstance, LL_I2C_IsActiveFlag_SB, SET, 
                                                    timeout_ms) != HL_SUCCESS)
    {
        return HL_ERROR;
    }

    /* Send slave address (read mode) - Write address to the DR register with 
    read bit set */
    LL_I2C_TransmitData8(stmInstance, (slaveAddress << 1) | 0x01);

    /* Receive data on the way appropriate to the number of bytes received */
    switch (size)
    {
        case 1:
            /* Special case for receiving exactly one byte */
            LL_I2C_AcknowledgeNextData(stmInstance, LL_I2C_NACK);

            /* Wait for address to be completely sent - Is ADDR bit set in SR1*/
            if (prvI2C_WaitFlagSet(stmInstance, LL_I2C_IsActiveFlag_ADDR, SET,
                                                    timeout_ms) != HL_SUCCESS)
            {
                return HL_ERROR;
            }

            LL_I2C_ClearFlag_ADDR(stmInstance);
            LL_I2C_GenerateStopCondition(stmInstance);

            /* Wait for RXNE flag - Data is ready */
            if (prvI2C_WaitFlagSet(stmInstance, LL_I2C_IsActiveFlag_RXNE, SET,
                                                        timeout_ms) != HL_SUCCESS)
            {
                return HL_ERROR;
            }

            /* Read the data after the RXNE is set */
            *pData = LL_I2C_ReceiveData8(stmInstance);
            break;
        case 2:
            /* Special case for receiving exactly two bytes */
            LL_I2C_EnableBitPOS(stmInstance);
            LL_I2C_AcknowledgeNextData(stmInstance, LL_I2C_ACK);

            /* Wait for address to be completely sent - Is ADDR bit set in SR1*/
            if (prvI2C_WaitFlagSet(stmInstance, LL_I2C_IsActiveFlag_ADDR, SET,
                                                    timeout_ms) != HL_SUCCESS)
            {
                return HL_ERROR;
            }

            LL_I2C_ClearFlag_ADDR(stmInstance);
            LL_I2C_AcknowledgeNextData(stmInstance, LL_I2C_NACK);

            /* Wait for BTF to be set */
            if (prvI2C_WaitFlagSet(stmInstance, LL_I2C_IsActiveFlag_BTF, SET,
                                                    timeout_ms) != HL_SUCCESS)
            {
                return HL_ERROR;
            }

            /* Generate STOP condition */
            LL_I2C_GenerateStopCondition(stmInstance);

            pData[1] = LL_I2C_ReceiveData8(stmInstance);
            pData[0] = LL_I2C_ReceiveData8(stmInstance);
            
            break;
        default:
            /* General case of receiving more than 2 bytes of data */
            LL_I2C_AcknowledgeNextData(stmInstance, LL_I2C_ACK);

            /* Wait for the ADDR flag to be set */
            if (prvI2C_WaitFlagSet(stmInstance, LL_I2C_IsActiveFlag_ADDR, SET,
                                                    timeout_ms) != HL_SUCCESS)
            {
                return HL_ERROR;
            }

            /* Clear ADDR flag */
            LL_I2C_ClearFlag_ADDR(stmInstance);

            /* Read bytes before last 3 bytes */
            while (tmpSize > 3)
            {
                if (prvI2C_WaitFlagSet(stmInstance, LL_I2C_IsActiveFlag_RXNE,
                                        SET, timeout_ms) != HL_SUCCESS)
                {
                    return HL_ERROR;
                }
                *pData++ = LL_I2C_ReceiveData8(stmInstance);
                tmpSize--;
            }

            /* Handle the last 3 bytes receive sequence */

            /* RXNE bit set */
            if (prvI2C_WaitFlagSet(stmInstance, LL_I2C_IsActiveFlag_RXNE, 
                                        SET, timeout_ms) != HL_SUCCESS)
            {
                return HL_ERROR;
            }

            /* BTF bit set - data N-2 in DR and N-1 in shift register, SCL low*/
            if (prvI2C_WaitFlagSet(stmInstance, LL_I2C_IsActiveFlag_BTF, 
                                        SET, timeout_ms) != HL_SUCCESS)
            {
                return HL_ERROR;
            }

            LL_I2C_AcknowledgeNextData(stmInstance, LL_I2C_ACK);

            /* Read data N-2 */
            *pData++ = LL_I2C_ReceiveData8(stmInstance);

            LL_I2C_AcknowledgeNextData(stmInstance, LL_I2C_ACK);

            /* Read data N-1 */
            *pData++ = LL_I2C_ReceiveData8(stmInstance);

            LL_I2C_AcknowledgeNextData(stmInstance, LL_I2C_NACK);
             
            /* RXNE bit set */
            if (prvI2C_WaitFlagSet(stmInstance, LL_I2C_IsActiveFlag_RXNE, 
                                        SET, timeout_ms) != HL_SUCCESS)
            {
                return HL_ERROR;
            } 


            /* Read data N */
            *pData++ = LL_I2C_ReceiveData8(stmInstance);          

            break;
    }

    /* Generate STOP condition */
    LL_I2C_GenerateStopCondition(stmInstance);
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