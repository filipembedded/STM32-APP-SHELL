/**
 * @file   bus.c
 * @brief  Advanced Microcontroller Bus Architecture - AMBA
 * @author filipembedded
 */

#include "bus.h"
#include "util.h"
#include "stm32f1xx_ll_bus.h"

/******************************* Functions ************************************/
// Advanced Peripheral Bus 1
hl_status_t HL_APB1_EnableClock(bus_hl_apb1_peripheral_t peripheral)
{
    /* TODO: Add asserts */
    LL_APB1_GRP1_EnableClock(peripheral);

    hl_state_t state = HL_RESET;
    state = HL_APB1_IsEnabledClock(peripheral);

    if (state == HL_SET)
        return HL_SUCCESS;
    else 
        return HL_ERROR;
}

hl_status_t HL_APB1_DisableClock(bus_hl_apb1_peripheral_t peripheral)
{
    /* TODO: Add asserts */
    LL_APB1_GRP1_DisableClock(peripheral);

    hl_state_t state = HL_SET;
    state = HL_APB1_IsEnabledClock(peripheral);

    if (state == HL_SET)
        return HL_ERROR;
    else 
        return HL_SUCCESS;
}

hl_state_t HL_APB1_IsEnabledClock(bus_hl_apb1_peripheral_t peripheral)
{
    /* TODO: Add asserts */
    uint32_t state = 0;
    state = LL_APB1_GRP1_IsEnabledClock(peripheral); 
    if (state == 1)
        return HL_SET;
    else 
        return HL_RESET;
}

// Advanced Peripheral Bus 2
hl_status_t HL_APB2_EnableClock(bus_hl_apb2_peripheral_t peripheral)
{
    /* TODO: Add asserts */
    LL_APB2_GRP1_EnableClock(peripheral);

    hl_state_t state = HL_RESET;
    state = HL_APB2_IsEnabledClock(peripheral); 
    
    if (state == HL_SET)
        return HL_SUCCESS;
    else 
        return HL_ERROR;
}

hl_status_t HL_APB2_DisableClock(bus_hl_apb2_peripheral_t peripheral)
{
    /* TODO: Add asserts */
    LL_APB2_GRP1_DisableClock(peripheral);

    hl_state_t state = HL_SET;
    state = HL_APB2_IsEnabledClock(peripheral);

    if (state == HL_SET)
        return HL_ERROR;
    else 
        return HL_SUCCESS;
}

hl_state_t HL_APB2_IsEnabledClock(bus_hl_apb2_peripheral_t peripheral)
{
    /* TODO: Add asserts */
    uint32_t state = 0;
    state = LL_APB2_GRP1_IsEnabledClock(peripheral);
    if (state == 1)
        return HL_SET;
    else 
        return HL_RESET;
}


