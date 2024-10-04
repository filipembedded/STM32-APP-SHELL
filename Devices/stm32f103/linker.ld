ENTRY(Reset_Handler)

MEMORY
{
	FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 64K
	SRAM (rwx) : ORIGIN = 0x20000000, LENGTH = 20K
}

SECTIONS
{
	.isr_vector :
	{
		KEEP(*(.isr_vector))
	} > FLASH

	.text :
	{
		. = ALIGN(4);

		*(.text)
		*(.rodata)

		. = ALIGN(4);
		_etext = .;
	} > FLASH

	.data :
	{
		. = ALIGN(4);
		_sdata = .;

		*(.data)

		. = ALIGN(4);
		_edata = .;
	} > SRAM AT > FLASH

	.bss :
	{
		. = ALIGN(4);
		_sbss = .;

		*(.bss)

		. = ALIGN(4);
		_ebss = .;
	} > SRAM

	/* Define heap and stack section */
    ._user_heap_stack :
    {
        . = ALIGN(8);
        PROVIDE ( end = . );
        PROVIDE ( _end = . );
        . = . + 0x200;  /* Min heap size (0x200 = 512 bytes) */
        . = . + 0x400;  /* Min stack size (0x400 = 1024 bytes) */
        . = ALIGN(8);
    } > SRAM
}
