/**
 * @file i2c.c
 * @brief High-Layer driver for I2C
 * @author filipembedded
 */

#include "i2c.h"
#include "stm32f1xx_ll_gpio.h"


/*************************** Static function declarations *********************/
static void prvI2C_InitPins(i2c_hl_instance_t instance);

/********************************** Definitions *******************************/

hl_status_t HL_I2C_Init(i2c_hl_instance_t instance, uint32_t clockSpeed)
{
    assert_i2c_instance(instance);

    /* Initialize peripheral clock for I2C */


}

void HL_I2C_DeInit(i2c_hl_instance_t instance)
{

}

hl_status_t HL_I2C_MasterTransmit(i2c_hl_instance_t instance,
                                  uint32_t slaveAddress,
                                  uint8_t  *pData,
                                  uint32_t size,
                                  uint32_t timeout_ms)
{

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
    switch (instance)
    {
    case I2C_HL_INSTANCE_1:
        LL_GPIO_InitTypeDef cfg = {0};

        break;
    case I2C_HL_INSTANCE_2:


        break;
    default:
        break;
    }
}