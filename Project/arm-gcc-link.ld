OUTPUT_FORMAT ("elf32-littlearm", "elf32-bigarm", "elf32-littlearm")
/* Internal Memory Map*/
MEMORY
{
	rom (rx)  : ORIGIN = 0x00000000, LENGTH = 0x00006000
	ram (rwx) : ORIGIN = 0x00006000, LENGTH = 0x00005000
}

_eram = 0x00006000 + 0x00005000;
SECTIONS
{
	.text :
	{
		KEEP(*(.isr_vector))
		*(.text*)
		
		KEEP(*(.init))
		KEEP(*(.fini))
		
		/* .ctors */
		*crtbegin.o(.ctors)
		*crtbegin?.o(.ctors)
		*(EXCLUDE_FILE(*crtend?.o *crtend.o) .ctors)
		*(SORT(.ctors.*))
		*(.ctors)
		
		/* .dtors */
		*crtbegin.o(.dtors)
		*crtbegin?.o(.dtors)
		*(EXCLUDE_FILE(*crtend?.o *crtend.o) .dtors)
		*(SORT(.dtors.*))
		*(.dtors)
		
		*(.rodata*)
		
		KEEP(*(.eh_fram e*))
	} > rom 
	
	.ARM.extab : 
	{
		*(.ARM.extab* .gnu.linkonce.armextab.*)
	} > rom 
	
	__exidx_start = .;
	.ARM.exidx :
	{
		*(.ARM.exidx* .gnu.linkonce.armexidx.*)
	} > rom 
	__exidx_end = .;
	__etext = .;
	
	/* _sidata is used in coide startup code */
	_sidata = __etext;
	
	.data : AT (__etext)
	{
		__data_start__ = .;
		
		/* _sdata is used in coide startup code */
		_sdata = __data_start__;
		
		*(vtable)
		*(.data*)
		
		. = ALIGN(4);
		/* preinit data */
		PROVIDE_HIDDEN (__preinit_array_start = .);
		KEEP(*(.preinit_array))
		PROVIDE_HIDDEN (__preinit_array_end = .);
		
		. = ALIGN(4);
		/* init data */
		PROVIDE_HIDDEN (__init_array_start = .);
		KEEP(*(SORT(.init_array.*)))
		KEEP(*(.init_array))
		PROVIDE_HIDDEN (__init_array_end = .);
		
		. = ALIGN(4);
		/* finit data */
		PROVIDE_HIDDEN (__fini_array_start = .);
		KEEP(*(SORT(.fini_array.*)))
		KEEP(*(.fini_array))
		PROVIDE_HIDDEN (__fini_array_end = .);
		
		KEEP(*(.jcr*))
		. = ALIGN(4);
		/* All data end */
		__data_end__ = .;
		
		/* _edata is used in coide startup code */
		_edata = __data_end__;
	} > ram 
	
	.bss :
	{
		. = ALIGN(4);
		__bss_start__ = .;
		_sbss = __bss_start__;
		*(.bss*)
		*(COMMON)
		. = ALIGN(4);
		__bss_end__ = .;
		_ebss = __bss_end__;
	} > ram 
		
	.heap (COPY):
	{
		__end__ = .;
		_end = __end__;
		end = __end__;
		*(.heap*)
		__HeapLimit = .;
	} > ram 
	
	/* .stack_dummy section doesn't contains any symbols. It is only
	* used for linker to calculate size of stack sections, and assign
	* values to stack symbols later */
	.co_stack (NOLOAD):
	{
		. = ALIGN(8);
		*(.co_stack .co_stack.*)
	} > ram 
	
	/* Set stack top to end of ram , and stack limit move down by
	* size of stack_dummy section */
	__StackTop = ORIGIN(ram ) + LENGTH(ram );
	__StackLimit = __StackTop - SIZEOF(.co_stack);
	PROVIDE(__stack = __StackTop);
	
	/* Check if data + heap + stack exceeds ram  limit */
	ASSERT(__StackLimit >= __HeapLimit, "region ram  overflowed with stack")

    /*
	 * We align everything to a page boundary so we can
	 * free it after init has commenced and SRAM contents have
	 * been copied to its destination.
	 */
	.sram_start : {
		. = ALIGN(4);
		__sram_start = .;
		__sram_code_start = .;
	}

	.text_sram_code 0x20000000 : AT(__sram_code_start)
	{
		__ssram_code_text = .;
		*(.sram.text)
		*(.sram.rodata)
		. = ALIGN(4);
		__esram_code_text = .;
	}

	/*
	 * Reset the dot pointer, this is needed to create the
	 * relative __sram_data_start below (to be used as extern in code).
	 */
	. = ADDR(.sram_start) + SIZEOF(.sram_start) + SIZEOF(.text_sram_code);

	.sram_data_start : {
		__sram_data_start = .;
	}

	/* TODO: add remainder of ITCM as well, that can be used for data! */
	.data_sram 0x20000000 + SIZEOF(.text_sram_code) : AT(__sram_data_start)
	{
		. = ALIGN(4);
		__ssram_data = .;
		*(.sram.data)
		. = ALIGN(4);
		__esram_data = .;
	}

	/* Reset the dot pointer or the linker gets confused */
	. = ADDR(.sram_data_start) + SIZEOF(.data_sram);

	/* End marker for freeing TCM copy in linked object */
	.sram_end : AT(ADDR(.sram_data_start) + SIZEOF(.data_sram)){
		__sram_end = .;
	}
}
