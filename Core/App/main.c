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
#include "stm32f1xx_ll_i2c.h"

#include "gpio.h"

uint32_t device_time_ms = 0;

/****************************** Callbacks *************************************/

void I2C_Init(void) {
    // Step 1: Enable the clocks for GPIOB and I2C1
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOB);  // Enable GPIOB clock
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_I2C1);   // Enable I2C1 clock

    // Step 2: Configure GPIO Pins PB6 (SCL) and PB7 (SDA)
    LL_GPIO_InitTypeDef gpio_initstruct = {0};
    
    gpio_initstruct.Pin = LL_GPIO_PIN_6 | LL_GPIO_PIN_7;  // Configure both pins
    gpio_initstruct.Mode = LL_GPIO_MODE_ALTERNATE;        // Set pins to alternate function
    gpio_initstruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;// Set output as Open-Drain (required for I2C)
    gpio_initstruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;      // Set high speed for I2C communication
    LL_GPIO_Init(GPIOB, &gpio_initstruct);                // Apply configuration

    // Step 3: Configure and Enable I2C1
    LL_I2C_InitTypeDef i2c_initstruct = {0};
    
    LL_I2C_Disable(I2C1); // Disable I2C before configuring

    // Set clock source for I2C peripheral (APB1 clock is default for I2C)
    LL_RCC_ClocksTypeDef rcc_clocks;
    LL_RCC_GetSystemClocksFreq(&rcc_clocks);

    // Set I2C timing (Standard mode - 100 kHz)
    //LL_I2C_SetClockSpeed(I2C1, rcc_clocks.PCLK1_Frequency, LL_I2C_DUTYCYCLE_2, 100000); // 100kHz
    LL_I2C_SetClockSpeedMode(I2C1, LL_I2C_CLOCK_SPEED_STANDARD_MODE);

    // Configure I2C
    i2c_initstruct.PeripheralMode  = LL_I2C_MODE_I2C;           // Standard I2C mode
    //i2c_initstruct.Timing          = 0x2000090E;                // Timing for 100kHz (derived from APB1 clock)
    i2c_initstruct.OwnAddress1     = 0;                         // No slave address (master mode)
    i2c_initstruct.TypeAcknowledge = LL_I2C_ACK;                // Enable ACK
    i2c_initstruct.DutyCycle       = LL_I2C_DUTYCYCLE_2;        // 2:1 duty cycle
    
    LL_I2C_Init(I2C1, &i2c_initstruct);                         // Initialize I2C with the configuration

    // Enable I2C1
    LL_I2C_Enable(I2C1);
}

void I2C_SendByte(uint8_t slave_address, uint8_t data) {
    // Wait until I2C1 is ready for communication
    while (!LL_I2C_IsActiveFlag_SB(I2C1)) {
        LL_I2C_GenerateStartCondition(I2C1);  // Generate Start condition
    }

    // Send Slave address (shifted left by 1 and last bit set to 0 for Write)
    LL_I2C_TransmitData8(I2C1, (slave_address << 1));

    // Wait for address to be acknowledged
    while (!LL_I2C_IsActiveFlag_ADDR(I2C1));
    LL_I2C_ClearFlag_ADDR(I2C1);  // Clear the ADDR flag

    // Wait until the transmit data register is empty
    while (!LL_I2C_IsActiveFlag_TXE(I2C1));

    // Send the data byte
    LL_I2C_TransmitData8(I2C1, data);

    // Wait until byte transfer is finished
    while (!LL_I2C_IsActiveFlag_BTF(I2C1));

    // Generate Stop condition
    LL_I2C_GenerateStopCondition(I2C1);
}

void main(void)
{
    /************************* Peripherals Initialization *********************/

    I2C_Init();
    
    uint8_t dummy_data = 0xAA;  // Dummy data to be sent
    uint8_t slave_address = 0b10101010;  // Replace with actual slave address

    while (1) {
        // Send dummy data to slave device
        I2C_SendByte(slave_address, dummy_data);

        //LL_I2C_TransmitData8(I2C1, dummy_data);

        // Wait for 500 ms before sending the next byte
        LL_mDelay(500);
    }
}


void SysTick_Handler(void)
{
    device_time_ms++;
}


