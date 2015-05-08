#ifndef __SRAM_H__
#define __SRAM_H__

#define MCU_SRAM_START_ADDR (0x20000000)
#define MCU_SRAM_SIZE       (1024*8)        /*8K*/
#define MCU_SRAM_DATA_END (MCU_SRAM_START_ADDR + MCU_SRAM_SIZE) /*MCU used 8K sram from 0xff8c1000 to 0xff8c3000*/

static inline U32 ddr_save_sp(U32 new_sp)
{
        unsigned long old_sp;

        asm volatile ("mov %0, sp" : "=r" (old_sp));
        asm volatile ("mov sp, %0" :: "r" (new_sp));
        return old_sp;
}

#define DDR_SAVE_SP(save_sp)            do { save_sp = ddr_save_sp(((U32)MCU_SRAM_DATA_END & (~7))); } while (0)
#define DDR_RESTORE_SP(save_sp)         do { ddr_save_sp(save_sp); } while (0)

/* Tag variables with this */
#define __sramdata __attribute__((section(".sram.data")))
/* Tag constants with this */
#define __sramconst __attribute__((section(".sram.rodata")))
/* Tag functions inside SRAM called from outside SRAM with this */
#define __sramfunc __attribute__((long_call)) __attribute__((section(".sram.text"))) __attribute__((noinline))
/* Tag function inside SRAM called from inside SRAM  with this */
#define sramlocalfunc __attribute__((section(".sram.text")))

#endif

