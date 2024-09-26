#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <system_stm32f1xx.h>
#include "rcc.h"



void main(void)
{
    /************************* Initialize System ******************************/
    // NOTE: This should be moved into Startup.c
    // Initialize clocks, PLL's,..
    HL_SystemInit();
    HL_ClockConfig();


    while(1)
    {

    }
}