#include <peri.h>
#include <task.h>

/*---------------------------------- Variable Define -------------------------*/
OS_STK   task_init_stk[TASK_STK_SIZE];	 	/*!< Stack of 'task_init' task.	*/

/* This function should only be called before task_init exit. */
static void uart_printf (char *ptr)
{
	while (*ptr != 0){
		Uart_SendChar(*ptr++);
	}
}

void task_init(void *pdata)
{
	/* Peripherals initialize */
	if (Uart_Init() != E_OK)
		uart_printf("CoOS: Peri: Initialize Uart...        [Fail]\n\r");

	if (Cru_Init() != E_OK)
		uart_printf("CoOS: Peri: Initialize Cru...        [Fail]\n\r");

	if (Mbox_Init() != E_OK)
		uart_printf("CoOS: Peri: Initialize Mailbox...        [Fail]\n\r");

	/* Applications initialize */
	Create_DdrTask();

	CoExitTask();	 /*!< Delete 'task_init' task. 	*/
}

int main(void)
{
	uart_printf("CoOS: Initialize CoOS...        ");
	CoInitOS();							/*!< Initial CooCox RTOS 			  */
	uart_printf (" [OK]. \n\r");

	uart_printf ("CoOS: Create a initial task...        ");
	CoCreateTask(task_init, (void *)0, 10,&task_init_stk[TASK_STK_SIZE-1], TASK_STK_SIZE);
	uart_printf (" [OK]. \n\r");

	uart_printf("CoOS: Start CoOS...\n\r");
	CoStartOS();

    while(1);

    return 0;
}
