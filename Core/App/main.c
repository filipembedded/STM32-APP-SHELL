#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <system_stm32f1xx.h>


void main(void)
{
    /************************* Initialize System ******************************/
    // Initialize clocks, PLL's,..
    SystemInit();
    // Update SystemCoreClock variable (always when changing clock source)
    SystemCoreClockUpdate();


    while(1)
    {

    }
}