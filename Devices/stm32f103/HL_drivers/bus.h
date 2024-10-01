/**
 * @file   bus.h
 * @brief  Advanced Microcontroller Bus Architecture - AMBA
 * @author filipembedded
 */

#ifndef BUS_H
#define BUS_H


/***************************** Includes ***************************************/
#include "stm32f103xb.h"
#include "stm32f1xx_ll_bus.h"
#include "util.h"



/***************************** Typedefs ***************************************/
typedef enum {
    BUS_HL_APB1_PERIPH_BKP     = LL_APB1_GRP1_PERIPH_BKP,
    BUS_HL_APB1_PERIPH_TIM2    = LL_APB1_GRP1_PERIPH_TIM2,
    BUS_HL_APB1_PERIPH_TIM3    = LL_APB1_GRP1_PERIPH_TIM3,
    BUS_HL_APB1_PERIPH_TIM4    = LL_APB1_GRP1_PERIPH_TIM4,
    BUS_HL_APB1_PERIPH_USART2  = LL_APB1_GRP1_PERIPH_USART2,
    BUS_HL_APB1_PERIPH_USART3  = LL_APB1_GRP1_PERIPH_USART3,
    BUS_HL_APB1_PERIPH_SPI2    = LL_APB1_GRP1_PERIPH_SPI2,
    BUS_HL_APB1_PERIPH_I2C1    = LL_APB1_GRP1_PERIPH_I2C1,
    BUS_HL_APB1_PERIPH_I2C2    = LL_APB1_GRP1_PERIPH_I2C2,
    BUS_HL_APB1_PERIPH_CAN1    = LL_APB1_GRP1_PERIPH_CAN1,
    BUS_HL_APB1_PERIPH_USB     = LL_APB1_GRP1_PERIPH_USB, 
    BUS_HL_APB1_PERIPH_WWDG    = LL_APB1_GRP1_PERIPH_WWDG,
} bus_hl_apb1_peripheral_t;

typedef enum {
    BUS_HL_APB2_PERIPH_GPIOA  = LL_APB2_GRP1_PERIPH_GPIOA,
    BUS_HL_APB2_PERIPH_GPIOB  = LL_APB2_GRP1_PERIPH_GPIOB,
    BUS_HL_APB2_PERIPH_GPIOC  = LL_APB2_GRP1_PERIPH_GPIOC,
    BUS_HL_APB2_PERIPH_GPIOD  = LL_APB2_GRP1_PERIPH_GPIOD,
    BUS_HL_APB2_PERIPH_GPIOE  = LL_APB2_GRP1_PERIPH_GPIOE,
    BUS_HL_APB2_PERIPH_TIM1   = LL_APB2_GRP1_PERIPH_TIM1,
    BUS_HL_APB2_PERIPH_SPI1   = LL_APB2_GRP1_PERIPH_SPI1,
    BUS_HL_APB2_PERIPH_USART1 = LL_APB2_GRP1_PERIPH_USART1,
    BUS_HL_APB1_PERIPH_ADC1   = LL_APB2_GRP1_PERIPH_ADC1,
    BUS_HL_APB1_PERIPH_ADC2   = LL_APB2_GRP1_PERIPH_ADC2,
} bus_hl_apb2_peripheral_t;


/**************************** Functions ***************************************/
hl_status_t HL_APB1_EnableClock(bus_hl_apb1_peripheral_t peripheral);

hl_status_t HL_APB1_DisableClock(bus_hl_apb1_peripheral_t peripheral);

hl_state_t HL_APB1_IsEnabledClock(bus_hl_apb1_peripheral_t peripheral);

hl_status_t HL_APB2_EnableClock(bus_hl_apb2_peripheral_t peripheral);

hl_status_t HL_APB2_DisableClock(bus_hl_apb2_peripheral_t peripheral);

hl_state_t HL_APB2_IsEnabledClock(bus_hl_apb2_peripheral_t peripheral);

#endif


