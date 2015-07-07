#include <peri.h>
#include <task.h>

//#define MCU_SW_VERSION   "V2.04 2015052604"

/*---------------------------------- Variable Define -------------------------*/
OS_STK   task_init_stk[TASK_STK_SIZE];	 	/*!< Stack of 'task_init' task.	*/

/* This function should only be called before task_init exit. */
static void uart_printf (char *ptr)
{
#if 0
	while (*ptr != 0){
		Uart_SendChar(*ptr++);
	}
#endif
}

void task_init(void *pdata)
{
	/* Peripherals initialize */
	if (Uart_Init() != E_OK)
		printf("[MCU]:Init uart failed\n");

	if (Cru_Init() != E_OK)
		printf("[MCU]:Init cru failed\n");

	if (Mbox_Init() != E_OK)
		printf("[MCU]:Init mailbox failed\n");

	/* Applications initialize */
	Create_DdrTask();
	Create_JtagMux_Task();
	creat_suspend_task();
#ifdef CFG_TSADC
    create_tsadc_task();
#endif
	CoExitTask();	 /*!< Delete 'task_init' task. 	*/
}

int main(void)
{
//	printf("\nMCU Version %s\n", MCU_SW_VERSION);
	*(volatile U32*)0x0 = 2015070707;/*addr 0 is M3 MSP value*/
	/*v1.04 20150401:training used cs 0*/
	/*V1.05 20150402:mailbox used global msg*/
	/*V2.00 20150403:*/
	/*V2.01 20150413:fix jtag iomux and reset stack size*/
	/*V2.01 20150422:fix idle iommu result in mmu bus error*/
	/*V2.02 20150425:support mcu coos code at dram and DCF code at sram*/
	/*V2.02 20150430 addr 0 used to store mcu code version*/
	/*V2.03 2015050703 for ddr3 dll bypass cl 10 cwl 7*/
	/*V2.03 2015050803 set idle task to sram*/
	/*V2.04 2015052604 add 3328 supported*/
	/*V2.05 2015062605 Fixed suspend&resume issue and added tsadc function */
	/*V2.06 2015070106 Modified tsadc function priority */	
	/*V2.07 2015070707 Fixed tsadc bug and added idle wfi */
	/*hy v1.01 set (mmu_status == 0)*/
	/*hy v1.02 :"MCU: Initialize MCU V2.01 20150420_MMUstall_hy_V1.02" ÐÞ¸Ädisable stall Ìõ¼þ¶ª£»*/
	/*cru must be inital first if not coos may can't run task*/
	CoInitOS();							/*!< Initial CooCox RTOS 			  */
//	uart_printf ("...       [OK]. \n\r");
//	uart_printf ("MCU: Create a initial task...        ");	
	CoCreateTask(task_init, (void *)0, TASK_INIT_PRI, &task_init_stk[TASK_STK_SIZE-1], TASK_STK_SIZE);
//	uart_printf (" [OK]. \n\r");
//	uart_printf("MCU: Start MCU...\n\r");
	CoStartOS();
    
	printf("[MCU]:CoOS start failed\n");
    while(1);

    return 0;
}
