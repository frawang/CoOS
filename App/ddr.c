#include <peri.h>
#include <task.h>

OS_STK   task_ddr_stk[TASK_STK_SIZE];	 	/*!< Stack of 'task_ddr' task.	*/

void Ddr_HandleCmd(MboxMsg *pMsg)
{
	Scpi_DdrCmd cmd = CMD_ID(pMsg->Cmd);

	switch (cmd) {
	case SCPI_DDR_CHANGE_FREQ: {
		volatile struct __packed {
			U32 Status;
		} *pBuf;

		pBuf = (volatile struct __packed *)pMsg->B2A_Buf;
		pBuf->Status = SCPI_SUCCESS;
		Mbox_CmdDone(pMsg);
		break;
	}
	default:
		break;
	}
}

void task_ddr(void *pdata)
{
	for (;;) {
		StatusType err;
		MboxMsg *pMsg = CoPendMail(mboxs[SCPI_CL_DDR], 0, &err);

		if (err == E_OK)
			Ddr_HandleCmd(pMsg);
		else
			printf("CoOS: CoPendMail...        [Fail]\n\r");
	}
}

void Create_DdrTask(void)
{
	/* Create Tasks */
	CoCreateTask(task_ddr, (void *)0, DDR_SET_PRI, &task_ddr_stk[TASK_STK_SIZE-1], TASK_STK_SIZE);
}
