#include <peri.h>
#include <task.h>
#include <coocox.h>

OS_STK   task_ddr_stk[TASK_STK_SIZE];	 	/*!< Stack of 'task_ddr' task.	*/
U32 gDdr_freq_MHz = 0;

void Ddr_HandleCmd(MboxMsg *pMsg)
{
	Scpi_DdrCmd cmd = CMD_ID(pMsg->Cmd);
//    U32 i;

	switch (cmd) {
	case SCPI_DDR_INIT: {
		volatile struct __packed1 {
			U32 dram_speed_bin;
			U32 freq;
			U32 lcdc_type;
			U32 addr_mcu_el3;
		} *pBuf_rx;

		volatile struct __packed2 {
			U32 Status;
		} *pBuf_tx;
		U32 dram_speed_bin;
		U32 freq;
        U32 lcdc_type;

		pBuf_rx = (volatile struct __packed1 *)pMsg->A2B_Buf;
		pBuf_tx = (volatile struct __packed2 *)pMsg->B2A_Buf;
		dram_speed_bin = pBuf_rx->dram_speed_bin;
		freq = pBuf_rx->freq;
		lcdc_type = pBuf_rx->lcdc_type;
		//to be defined
		rk3368_ddr_init(dram_speed_bin, freq, lcdc_type, pBuf_rx->addr_mcu_el3);
		
		pBuf_tx->Status = SCPI_SUCCESS;
		Mbox_CmdDone(pMsg);
		break;
	}

	case SCPI_DDR_SET_FREQ: {
		volatile struct __packed1 {
			U32 clk_rate;
			U32 lcdc_type;
		} *pBuf_rx;

		volatile struct __packed2 {
			U32 Status;
		} *pBuf_tx;

		pBuf_rx = (volatile struct __packed1 *)pMsg->A2B_Buf;
		pBuf_tx = (volatile struct __packed2 *)pMsg->B2A_Buf;
//		printf("[%d]MCU:Change to %dMHz->  ",(U32)OSTickCnt,pBuf_rx->clk_rate);
		gDdr_freq_MHz = rk3368_ddr_change_freq(pBuf_rx->clk_rate, pBuf_rx->lcdc_type);
//		printf("[%d]success %dMHz\n\r",(U32)OSTickCnt,gDdr_freq_MHz);
		pBuf_tx->Status = SCPI_SUCCESS;
		Mbox_CmdDone(pMsg);
		break;
	}

	case SCPI_DDR_AUTO_SELF_REFRESH: {
		volatile struct __packed1 {
			U32 enable;
		} *pBuf_rx;

		volatile struct __packed2 {
			U32 Status;
		} *pBuf_tx;
		U32 enable;

		pBuf_rx = (volatile struct __packed1 *)pMsg->A2B_Buf;
		pBuf_tx = (volatile struct __packed2 *)pMsg->B2A_Buf;
		enable = pBuf_rx->enable;
		
		//to be defined
		rk3368_ddr_set_auto_self_refresh(enable);
		pBuf_tx->Status = SCPI_SUCCESS;
		Mbox_CmdDone(pMsg);
		break;		
	}	

	case SCPI_DDR_BANDWIDTH_GET: {
	/*
		volatile struct __packed1 {
			U32 Status;
		} *pBuf_rx;

		volatile struct __packed2 {
			U32 Status;
			struct ddr_bw_info ddr_bw_ch0;
			struct ddr_bw_info ddr_bw_ch1;
		} *pBuf_tx;
		struct ddr_bw_info ddr_bw_ch0;
		struct ddr_bw_info ddr_bw_ch1;

		pBuf_rx = (volatile struct __packed1 *)pMsg->A2B_Buf;
		pBuf_tx = (volatile struct __packed2 *)pMsg->B2A_Buf;
		pBuf_tx->Status = SCPI_SUCCESS;

		//to be defined
		//rk3368_ddr_bandwidth_get(&ddr_bw_ch0,  &ddr_bw_ch1);		
		memcpy(&(pBuf_tx->ddr_bw_ch0), &ddr_bw_ch0, sizeof(pBuf_tx->ddr_bw_ch0));
        memcpy(&(pBuf_tx->ddr_bw_ch1), &ddr_bw_ch1, sizeof(pBuf_tx->ddr_bw_ch1));
		
		Mbox_CmdDone(pMsg);
		*/
		break;		
	}	

	case SCPI_DDR_GET_FREQ: {
		volatile struct __packed2 {
			U32 Status;
			U32 clk_rate;
		} *pBuf_tx;

		pBuf_tx = (volatile struct __packed2 *)pMsg->B2A_Buf;
		pBuf_tx->Status = SCPI_SUCCESS;
		pBuf_tx->clk_rate = ddr_get_dram_freq();
		//printf("[%d]Get:%dM,ADR:%x,CH:%d\n\r",(U32)OSTickCnt,pBuf_tx->clk_rate,pBuf_tx,pMsg->Id);
		Mbox_CmdDone(pMsg);
		break;
	}	

	default:
		break;
	}
}
//#define DDR_CHANGEFREQ_TEST
void task_ddr(void *pdata)
{
#ifdef DDR_CHANGEFREQ_TEST
    uint32 freq,times = 0;
    uint32 max_freq = 800;
    uint32 min_freq = 400;
#endif
    //rk3368_ddr_init(21,0);
	for (;;) {
        #ifdef DDR_CHANGEFREQ_TEST
	    while(1)
	    {
            do
            {
                freq = min_freq + rand();
                freq %= max_freq;
            }while(freq < min_freq);
//            printf("MCU:Change to %d\n\r",freq);
            freq = rk3368_ddr_change_freq(freq);
            times +=1;
//            printf("MCU:Change succ %d,times = %d\n\r",freq,times);
	    }
        #endif
		StatusType err;
		MboxMsg *pMsg = CoPendMail(mboxs[SCPI_CL_DDR], 0, &err);

		if (err == E_OK)
			Ddr_HandleCmd(pMsg);
		else
			printf("[MCU]DDR PendMail Failed\n\r");
	}

    CoExitTask();
}

void Create_DdrTask(void)
{
	/* Create Tasks */
	CoCreateTask(task_ddr, (void *)0, DDR_SET_PRI, &task_ddr_stk[TASK_STK_SIZE-1], TASK_STK_SIZE);
}
