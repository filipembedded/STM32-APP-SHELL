// Cortex-M startup code using gcc-none-eabi toolchain
#include <stdint.h>
#include <string.h>
 
// symbols provided by the .ld linker script file
extern uint32_t _etext;
extern uint32_t _sdata;
extern uint32_t _edata;
extern uint32_t _sidata;
extern uint32_t _sbss;
extern uint32_t _ebss;
extern uint32_t _estack;
 
extern int main(void);
 
#pragma GCC optimize("O3")
 
__attribute__ ((naked)) 
void Reset_Handler(void)
{
    // copy initialized global variables in .data from flash to SRAM
    memcpy(&_sdata, &_sidata, (size_t)&_edata - (size_t)&_sdata);
 
    // init uninitialized global variables in .bss with zero
    memset(&_sbss, 0, (size_t)&_ebss - (size_t)&_sbss);
    
    // now invoke main()
    main();
 
    // if main() returns:
    for (;;)
        ;
}
 
void* vectors[128] __attribute__((section(".isr_vector"))) = 
{
    &_estack,
    Reset_Handler,
    // more IRQ handlers go here
};
