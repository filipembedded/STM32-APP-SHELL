/**
 * @file i2c.h
 * @brief High-Layer I2C driver
 * @author filipembedded
 */

#ifndef I2C_H
#define I2C_H

#include "stm32f1xx_ll_i2c.h"
#include "util.h"
#include "assert.h"

/********************************** Defines ***********************************/
#define assert_i2c_instance(instance) assert((instance == I2C_HL_INSTANCE_1)|| \
                                            (instance == I2C_HL_INSTANCE_2))

#define I2C_HL_OWN_ADDRESS 0x00
/********************************** Typedefs **********************************/
typedef enum {
    I2C_HL_INSTANCE_1,
    I2C_HL_INSTANCE_2
} i2c_hl_instance_t;

typedef enum {
    I2C_HL_MODE_I2C              = LL_I2C_MODE_I2C,
    I2C_HL_MODE_SMBUS_HOST       = LL_I2C_MODE_SMBUS_HOST,
    I2C_HL_MODE_SMBUS_DEVICE     = LL_I2C_MODE_SMBUS_DEVICE,
    I2C_HL_MODE_SMBUS_DEVICE_ARP = LL_I2C_MODE_SMBUS_DEVICE_ARP,
} i2c_hl_peripheral_mode_t;

typedef enum {
    I2C_HL_DUTYCYCLE_2    = LL_I2C_DUTYCYCLE_2,
    I2C_HL_DUTYCYCLE_16_9 = LL_I2C_DUTYCYCLE_16_9,
} i2c_hl_duty_cycle_t;

typedef enum {
    I2C_HL_ACK  = LL_I2C_ACK,
    I2C_HL_NACK = LL_I2C_NACK,
} i2c_hl_type_acknowledge_t;

typedef enum {
    I2C_HL_OWNADDR_7BIT  = LL_I2C_OWNADDRESS1_7BIT,
    I2C_HL_OWNADDR_10BIT = LL_I2C_OWNADDRESS1_10BIT,
} i2c_hl_own_addr_size_t;

typedef struct {
    i2c_hl_peripheral_mode_t  periphMode;
    uint32_t                  clockSpeed;
    i2c_hl_duty_cycle_t       dutyCycle;
    uint32_t                  ownAddress;
    i2c_hl_type_acknowledge_t acknowledge;
    i2c_hl_own_addr_size_t    ownAddrSize;
} i2c_hl_cfg_t;


/******************************** Declarations ********************************/

hl_status_t HL_I2C_Init(i2c_hl_instance_t instance, uint32_t clockSpeed);
void HL_I2C_DeInit(i2c_hl_instance_t instance);
hl_status_t HL_I2C_MasterTransmit(i2c_hl_instance_t instance,
                                  uint32_t slaveAddress,
                                  uint8_t  *pData,
                                  uint32_t size,
                                  uint32_t timeout_ms);

hl_status_t HL_I2C_MasterReceive(i2c_hl_instance_t instance,
                                  uint32_t slaveAddress,
                                  uint8_t  *pData,
                                  uint32_t size,
                                  uint32_t timeout_ms);






#endif
