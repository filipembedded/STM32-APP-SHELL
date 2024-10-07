#include "util.h"
#include "bus.h"
#include "stm32f1xx_ll_tim.h"
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_utils.h"



/********************************** Variables *********************************/
volatile uint32_t timer2_tick_count = 0;


/*********************************** Functions ********************************/

void HL_UTIL_InitDeviceTimer2()
{
    // Enable Timer2 clock
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);

    // Set Timer2 to count up
    LL_TIM_SetCounterMode(TIM2, LL_TIM_COUNTERMODE_UP);
    
    // Set prescaler to divide system clock down to 1 MHz (1 tick = 1 µs)
    LL_TIM_SetPrescaler(TIM2, (72-1));

    // Set the auto-reload register to 1000 (so Timer2 overflows every 1 ms)
    LL_TIM_SetAutoReload(TIM2, 1000 - 1);

    // Enable the update interrupt
    LL_TIM_EnableIT_UPDATE(TIM2);
    
    // Enable the timer
    LL_TIM_EnableCounter(TIM2);

    // Set the update request source to counter overflow/underflow only
    LL_TIM_SetUpdateSource(TIM2, LL_TIM_UPDATESOURCE_REGULAR);
    
    // Enable Timer2 interrupt in NVIC
    NVIC_SetPriority(TIM2_IRQn, 0);
    NVIC_EnableIRQ(TIM2_IRQn);
}

uint32_t HL_GetDeviceTime(void)
{
    return timer2_tick_count;
}

void HL_UTIL_DelayMs(uint32_t delay)
{
    LL_mDelay(delay);
}

/********************* Interrupt Service Routines *****************************/
void TIM2_IRQHandler(void)
{
    /* Check if update flag has been set */
    if (LL_TIM_IsActiveFlag_UPDATE(TIM2)) {
        LL_TIM_ClearFlag_UPDATE(TIM2);
        /* Increment the milisecond tick count */
        timer2_tick_count++;
    }
}

