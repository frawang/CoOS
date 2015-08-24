#include <peri.h>
#include <task.h>

/*---------------------------------- Variable Define -------------------------*/
OS_STK   task_init_stk[TASK_STK_SIZE];	 	/*!< Stack of 'task_init' task.	*/


void task_init(void *pdata)
{
	/* Peripherals initialize */
	if (Uart_Init() != E_OK)
		printf("[MCU]Init uart failed\n\r");

	if (Mbox_Init() != E_OK)
		printf("[MCU]Init mailbox failed\n\r");

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
	CoInitOS(); /*!< Initial CooCox RTOS */

	CoCreateTask(task_init, (void *)0, TASK_INIT_PRI, &task_init_stk[TASK_STK_SIZE-1], TASK_STK_SIZE);

	CoStartOS();

	printf("\n\r**** MCU CoOS Start Failed ****\n\r");
    while(1);

    return 0;
}
