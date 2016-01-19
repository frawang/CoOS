/**
 *******************************************************************************
 * @file       mailbox.c
 * @version    V1.0    
 * @date       2014.12.31
 * @brief      mailbox source file
 * @details    This file provides all the mailbox firmware functions.
 *******************************************************************************
 * @copy
 *
 *  Redistribution and use in source and binary forms, with or without 
 *  modification, are permitted provided that the following conditions 
 *  are met: 
 *  
 *      * Redistributions of source code must retain the above copyright 
 *  notice, this list of conditions and the following disclaimer. 
 *      * Redistributions in binary form must reproduce the above copyright
 *  notice, this list of conditions and the following disclaimer in the
 *  documentation and/or other materials provided with the distribution. 
 *      * Neither the name of the <ORGANIZATION> nor the names of its 
 *  contributors may be used to endorse or promote products derived 
 *  from this software without specific prior written permission. 
 *  
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF 
 *  THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * <h2><center>&copy; COPYRIGHT 2014 Fuzhou Rockchip Electronics Co., Ltd </center></h2>
 *******************************************************************************
 */ 
#include <peri.h>
#include <coocox.h>
#include "AppConfig.h"

OS_FlagID   flags_chan[NUM_CHANS];  /*!< Flag id,related to mailbox channels.*/
OS_EventID  mboxs[SCPI_MAX];        /*!< Save id of mailbox.        */

MboxReg *pMbox = (MboxReg *)MBOX_BASE;
MboxBuf *pBuf  = (MboxBuf *)(SRAM_BASE + SRAM_SIZE - SZ_4K);

extern OS_EventID suspend_mail;
extern int jtag_mux_en;

extern void *memset(void *s, int ch, size_t n);
extern void *memcpy(void *dest, const void *src, size_t n);

static void Mbox_HandleSysCmd(MboxMsg *pMsg)
{
	Scpi_SysCmd cmd = CMD_ID(pMsg->Cmd);

	switch (cmd) {
	case SCPI_SYS_GET_VERSION: {
		volatile struct __packed {
			U32 Status;
			struct scpi_mcu_ver {
				u32  scpi_ver;
				char mcu_ver[16];
			} version;
		} *pBuf;

		pBuf = (volatile struct __packed *)pMsg->B2A_Buf;
		memset((void *)pBuf, 0, sizeof(*pBuf));
		pBuf->Status = SCPI_SUCCESS;
		pBuf->version.scpi_ver = SCPI_VERSION;
		memcpy((void *)(pBuf->version.mcu_ver), MCU_SW_VERSION, 16);

		Mbox_CmdDone(pMsg);
		break;
	}
	case SCPISYS_REFRESH_MCU_FREQ: {
		volatile struct __packed {
			U32 Status;
			U32 Ver;
		} *pBuf;
//		U32 freq;

		pBuf = (volatile struct __packed *)pMsg->B2A_Buf;
		pBuf->Status = SCPI_SUCCESS;

		Cru_SetMcuFreq(CFG_CPU_FREQ);
//		printf("MCU: Retry to set %uMhz, real %uMhz\n\r", CFG_CPU_FREQ / 1000000, freq / 1000000);

		Mbox_CmdDone(pMsg);
		break;
	}
	case SCPI_SYS_SET_MCU_STATE_SUSPEND: {
		volatile struct __packed {
			U32 Status;
		} *pBuf;
		pBuf = (volatile struct __packed *)pMsg->B2A_Buf;
        if (isr_PostMail(mboxs[SCPI_CL_SYS], pMsg) != E_OK) 
        {
            pBuf->Status = SCPI_ERR_SUPPORT;
            Mbox_CmdDone(pMsg);
        }
        else
            pBuf->Status = SCPI_SUCCESS;
//      Mbox_CmdDone(pMsg);
        /* Firstly, send the mailbox message, and then set M3 to WFI state*/
		//SetMcuToWfiState();
		break;
	}
	case SCPI_SYS_SET_MCU_STATE_RESUME: {
		volatile struct __packed {
			U32 Status;
		} *pBuf;
        /* Resume M3 from suspend state*/
		//ResumeMcuFromSuspendState();
		pBuf = (volatile struct __packed *)pMsg->B2A_Buf;
		pBuf->Status = SCPI_SUCCESS;
		Mbox_CmdDone(pMsg);
		break;
	}

	case SCPI_SYS_SET_JTAGMUX_ON_OFF: {
		volatile struct __packed1 {
			u32 enable;
		} *rx_buf;

		volatile struct __packed2 {
			u32 status;
		} *tx_buf;

		rx_buf = (volatile struct __packed1 *)pMsg->A2B_Buf;
		tx_buf = (volatile struct __packed2 *)pMsg->B2A_Buf;

		jtag_mux_en = rx_buf->enable;
		tx_buf->status = SCPI_SUCCESS;
		Mbox_CmdDone(pMsg);
		break;
	}
	default:
		break;
	}
}

void Mbox_CmdDone(MboxMsg *pMsg)
{
	pMbox->B2A[pMsg->Id].Cmd = pMsg->Cmd;
	pMbox->B2A[pMsg->Id].Data = pMsg->B2A_Size;
}

/**
 *******************************************************************************
 * @brief		MBOX_IRQHandler(Vector :138-141, MBOX)
 * @param[in] 	None
 * @param[out] 	None
 * @retval		None
 *
 * @details	   	This is Mailbox interrupt handler,Use to handle Mailbox command.
 *******************************************************************************
 */
 MboxMsg g_msg[NUM_CHANS];
void Mbox_IRQHandler(void)
{
	MboxId id;
	U32 pending;

    CoEnterISR();                 /* Tell CooCox that we are starting an ISR. */
    pending = pMbox->A2B_Status;

    for(id = 0; id < NUM_CHANS; id++) {
    	//MboxMsg msg;
    	Scpi_ClientId sid;
    	Scpi_SysCmd cmd;
    	if (!(pending & (1 << id)))
    		continue;

    	pMbox->A2B_Status = 1 << id;

    	g_msg[id].Cmd = pMbox->A2B[id].Cmd;
    	g_msg[id].A2B_Buf = pBuf->A2B_Buf[id];
    	g_msg[id].A2B_Size = A2B_SIZE(g_msg[id].Cmd);
    	g_msg[id].B2A_Buf = pBuf->B2A_Buf[id];
    	g_msg[id].B2A_Size = pMbox->A2B[id].Data;
    	g_msg[id].Id = id;

    	sid = CMD_SENDER_ID(g_msg[id].Cmd);
    	cmd = CMD_ID(g_msg[id].Cmd);
//    	printf("[%d]MCU: Chan[%d]: A2B message, cmd 0x%08x\n\r",(U32)OSTickCnt, id, g_msg[id].Cmd);
    	if (sid > SCPI_MAX) {
    		* ((U32 *)g_msg[id].B2A_Buf) = SCPI_ERR_SUPPORT;
    		Mbox_CmdDone(&g_msg[id]);
    	}
    	else if (sid == SCPI_CL_SYS) {
    		Mbox_HandleSysCmd(&g_msg[id]);
    	} else {
    		if (isr_PostMail(mboxs[sid], &g_msg[id]) != E_OK) {
                printf("[MCU]MBOX PostMail Failed, SID=%d, CMD=%d\n\r", sid, cmd);
                * ((U32 *)g_msg[id].B2A_Buf) = SCPI_ERR_SUPPORT;
                Mbox_CmdDone(&g_msg[id]);
    		}
    	}
	
        /* clear interrupt pending bit */
        NVIC_ClearPendingIRQ(MBOX0_IRQn + id);
    }

	CoExitISR();
}

int Mbox_Init(void)
{
	MboxId id;

	/* Enable all A2B interrupts */
	pMbox->A2B_Inten = (1 << NUM_CHANS) - 1;

	for (id = 0; id < SCPI_MAX; id++) {

		/* Create Mboxs */
		mboxs[id] = CoCreateMbox(EVENT_SORT_TYPE_FIFO);
	 	if(mboxs[id] == E_CREATE_FAIL)
			return 	E_CREATE_FAIL;
	}
	for (id = 0; id < NUM_CHANS; id++) {
	 	/* Create channel flags */
	 	flags_chan[id] = CoCreateFlag(Co_TRUE, 1);
	 	if(flags_chan[id] == E_CREATE_FAIL)
	 		return 	E_CREATE_FAIL;

	 	/* enable interrupt */
	 	NVIC_EnableIRQ(MBOX0_IRQn + id);
	}

	return E_OK;
}
