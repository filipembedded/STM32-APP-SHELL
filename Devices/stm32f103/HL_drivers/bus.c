/**
 * @file   bus.c
 * @brief  Advanced Microcontroller Bus Architecture - AMBA
 * @author filipembedded
 */

#include "bus.h"
#include "util.h"
#include "stm32f1xx_ll_bus.h"

/******************************* Functions ************************************/
hl_status_t HL_APB1_EnableClock(bus_hl_apb1_peripheral_t peripheral)
{
    /* TODO: Add asserts */
    LL_APB1_GRP1_EnableClock(peripheral);
    if (HL_APB1_IsEnabledClock(peripheral) == HL_SET)
        return HL_SUCCESS;
    else 
        return HL_ERROR;
}

hl_status_t HL_APB1_DisableClock(bus_hl_apb1_peripheral_t peripheral)
{
    /* TODO: Add asserts */
    LL_APB1_GRP1_DisableClock(peripheral);
    if (HL_APB1_IsEnabledClock(peripheral) == HL_SET)
        return HL_ERROR;
    else 
        return HL_SUCCESS;
}

hl_state_t HL_APB1_IsEnabledClock(bus_hl_apb1_peripheral_t peripheral)
{
    /* TODO: Add asserts */
    if (LL_AHB1_GRP1_IsEnabledClock(peripheral))
        return HL_SET;
    else 
        return HL_RESET;
}

hl_status_t HL_APB2_EnableClock(bus_hl_apb2_peripheral_t peripheral)
{
    /* TODO: Add asserts */
    LL_APB2_GRP1_EnableClock(peripheral);
    if (HL_APB2_IsEnabledClock(peripheral) == HL_SET)
        return HL_SUCCESS;
    else 
        return HL_ERROR;
}

hl_status_t HL_APB2_DisableClock(bus_hl_apb1_peripheral_t peripheral)
{
    /* TODO: Add asserts */
    LL_APB2_GRP1_DisableClock(peripheral);
    if (HL_APB2_IsEnabledClock(peripheral) == HL_SET)
        return HL_ERROR;
    else 
        return HL_SUCCESS;
}

hl_state_t HL_APB2_IsEnabledClock(bus_hl_apb2_peripheral_t peripheral)
{
    /* TODO: Add asserts */
    if (LL_APB2_GRP1_IsEnabledClock(peripheral))
        return HL_SET;
    else 
        return HL_RESET;
}


