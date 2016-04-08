#include <peri.h>
#include <task.h>
#include <coocox.h>

OS_STK   task_jtagmux_stk[TASK_STK_SIZE];	 	/*!< Stack of 'task_ddr' task.	*/
OS_STK   task_suspend_stk[TASK_STK_SIZE];

#ifdef RK3368
int jtag_mux_en = 0;
#endif

void set_mcujtag_iomux(void)
{
	u32 *grf_iomux = (u32 *)MCUJTAG_IOMUX_ADDR;

	writel(MCUJTAG_IOMUX, grf_iomux);
}

void task_jtagmux(void *pdata)
{
	u32 *p = (u32 *)(SDMMC_BASE + SDMMC_CDETECT);
#ifndef RK3368
	u32 *grf_iomux = (u32 *)MCUJTAG_IOMUX_ADDR;
#endif

	for (;;) {
#if defined(RK3399) && defined(PMU_M0)
		/* for rk3399 pmu m0, just need do set iomux */
		set_mcujtag_iomux();
		goto DELAY;
#endif

#ifdef RK3368
		if (jtag_mux_en == 1) { /* controled by AP */
#endif
			/* does iomux select to sdmmc0_data0? */
#ifdef RK3368
			if ((readl(GRF_BASE + GRF_GPIO2A_IOMUX) & (0x3 << 10)) == 0x1 << 10) {
#else
			if ((readl(grf_iomux) & (0x3 << 0)) == 0x1 << 0) {
#endif
				/* check no sdcard again and fix iomux to jtag*/
				if (*p & 0x1) {
					/* set iomux to mcu jtag */
					set_mcujtag_iomux();
				}
			}
#ifdef RK3368
		}
#endif /* endif if jtag_mux_en ... */

#if defined(RK3399) && defined(PMU_M0)
DELAY:
#endif
		/*delay 3 second*/
		CoTimeDelay(0,0,3,0);
	}

	CoExitTask();
}

void Create_JtagMux_Task(void)
{
	/* Create Tasks */
	CoCreateTask(task_jtagmux, (void *)0, JTAG_MUX_PRI, &task_jtagmux_stk[TASK_STK_SIZE-1], TASK_STK_SIZE);
}

static void SetMcuToWfiState(MboxMsg *pMsg)
{
    U32 systick_ctl = NVIC_ST_CTRL & 0xf;
    volatile struct __packed2 {
        U32 Status;
    } *pBuf_tx;

    pBuf_tx = (volatile struct __packed2 *)pMsg->B2A_Buf;
    pBuf_tx->Status = SCPI_SUCCESS;

    NVIC_ST_CTRL = 0;
    IRQ_DISABLE_SAVE();

    Mbox_CmdDone(pMsg);
    
    __asm volatile ("wfi");

    NVIC_ST_CTRL = systick_ctl;
    IRQ_ENABLE_RESTORE ();
}

void task_suspend(void *pdata)
{
    StatusType err;
    MboxMsg *pMsg = Co_NULL;

    for (; ;) {
        pMsg = CoPendMail(mboxs[SCPI_CL_SYS], 0, &err);
        //CoPendMail(suspend_mail, 0, &err);
        if (err == E_OK) {
            SetMcuToWfiState(pMsg);
        } else {
            printf("[MCU]Suspend PendMail Failed\n\r");
        }
    }
	
    CoExitTask();
}

void creat_suspend_task(void)
{
//	CoCreateTask(task_jtagmux, (void *)0, JTAG_MUX_PRI, &task_jtagmux_stk[TASK_STK_SIZE-1], TASK_STK_SIZE);

	CoCreateTask(task_suspend, (void *)0, TASK_SUSPEND_PRI, &task_suspend_stk[TASK_STK_SIZE-1], TASK_STK_SIZE);
}
