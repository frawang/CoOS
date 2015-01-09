/**
 *******************************************************************************
 * @file       mbox.h
 * @version    V1.0    
 * @date       2014.12.31
 * @brief      Mailbox header file
 * @details    This file including some defines and declares related to mailbox.
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

#ifndef __MBOX_H
#define __MBOX_H

#include <scpi.h>

typedef U8                 MboxId;

#define NUM_CHANS			4
extern OS_EventID			mboxs[SCPI_MAX];	/*!< Save id of mailbox.			*/

typedef struct {
	U32 Cmd;
	int A2B_Size;
	volatile void *A2B_Buf;
	int B2A_Size;
	volatile void *B2A_Buf;
	MboxId Id;
} MboxMsg;

typedef struct {
	U32 Cmd;
	U32 Data;
} MboxCmd;

typedef volatile struct {
	U8 A2B_Buf[4][0x200];
	U8 B2A_Buf[4][0x200];
} MboxBuf;

typedef volatile struct {
	U32 A2B_Inten;
	U32 A2B_Status;
	MboxCmd A2B[4];
	U32 B2A_Inten;
	U32 B2A_Status;
	MboxCmd B2A[4];
} MboxReg;

void Mbox_CmdDone(MboxMsg *pMsg);
void Mbox_IRQHandler(void);
int Mbox_Init(void);

#endif
