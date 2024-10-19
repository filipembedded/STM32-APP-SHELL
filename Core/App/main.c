#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <system_stm32f1xx.h>
#include "rcc.h"
#include "swd.h"
#include "gpio.h"
#include "bus.h"
#include "i2c.h"
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

uint32_t tim2_dev_time = 0;

#define SLAVE_ADDRESS 0x05


/****************************** Callbacks *************************************/

void I2C_Init(void) {
    // Step 1: Enable the clocks for GPIOB and I2C1
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOB);  // Enable GPIOB clock
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_I2C1);   // Enable I2C1 clock

    if (LL_APB1_GRP1_IsEnabledClock(LL_APB1_GRP1_PERIPH_I2C1) != 1)
    {
        for (;;);
    }

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



void main(void)
{

    /************************* Initialize device time *************************/
    HL_UTIL_InitDeviceTimer2();

    /************************* Peripherals Initialization *********************/
    //HL_APB1_DisableClock(BUS_HL_APB1_PERIPH_I2C1);
    
    // if (HL_I2C_Init(I2C_HL_INSTANCE_2, 100000) != HL_SUCCESS)
    // {
    //     for (;;);
    // }
    // 
    // I2C_Init();
    




    
    LL_APB1_GRP1_EnableClock(BUS_HL_APB1_PERIPH_I2C1);

    if (LL_AHB1_GRP1_IsEnabledClock(LL_APB1_GRP1_PERIPH_I2C1) == 1)
        for (;;);

    uint32_t status_ll = LL_APB1_GRP1_IsEnabledClock(LL_APB1_GRP1_PERIPH_I2C1);

    
    if (status_ll == 1)
    {
        for (;;);
    }
    


    if (status == HL_ERROR)
        for(;;);

    


    uint8_t baba[] = {0x0b, 0x0a, 0x0b, 0x0a};

    while (1) {
        // Send dummy data to slave device
        HL_I2C_MasterTransmit(I2C_HL_INSTANCE_1, SLAVE_ADDRESS, baba, 
                                                            sizeof(baba), 500);

        // Wait for 500 ms before sending the next byte
        LL_mDelay(500);
    }
}


void SysTick_Handler(void)
{
    device_time_ms++;
}


