#include <peri.h>
#include <coocox.h>
#include <sram.h>

#if 0
/* SRAM section definitions from the linker */
extern char __sram_code_start, __ssram_code_text, __esram_code_text;
extern char __sram_data_start, __ssram_data, __esram_data;

void *mcu_memset(void *s, int c, U32 count)
{
	char *xs = s;

	while (count--)
		*xs++ = c;
	return s;
}

void *mcu_memcpy(void *dest, const void *src, U32 count)
{
	char *tmp = dest;
	const char *s = src;

	while (count--)
		*tmp++ = *s++;
	return dest;
}

#if 0
int __sramdata sram_tst_flag = 0;
void __sramfunc sram_fun_tst(void)
{
    sram_tst_flag = 0x55;

    return;
}
#endif
int mcu_sram_init(void)
{
    char *start;
    char *end;
    char *ram;

	mcu_memset((char *)&__ssram_code_text, 0x0, MCU_SRAM_SIZE);
			
    /* Copy code from RAM to SRAM CODE */
    start = &__ssram_code_text;
    end   = &__esram_code_text;
    ram   = &__sram_code_start;
    mcu_memcpy(start, ram, (end-start));

    //uart_printf("DDR: copied DDR code to MCU RAM\n\r");

    /* Copy data from RAM to SRAM DATA */
    start = &__ssram_data;
    end   = &__esram_data;
    ram   = &__sram_data_start;
    mcu_memcpy(start, ram, (end-start));

    //uart_printf("DDR: copied DDR data to MCU RAM\n\r");

//    sram_fun_tst();

    return 0;
}
#endif

