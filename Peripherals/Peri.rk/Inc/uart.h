/**
 *******************************************************************************
 * @file       uart.h
 * @version    V1.0    
 * @date       2014.12.31
 * @brief      Uart header file
 * @details    This file including some defines and declares related to uart.
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

#ifndef __UART_H
#define __UART_H

extern OS_MutexID	mut_uart;

typedef volatile struct {
	U32 Rbr; /*!< rbr, thr, dll			*/
	U32 Ier; /*!< dlh, ier					*/
	U32 Iir; /*!< iir, fcr					*/
	U32 Lcr;
	U32 Mcr;
	U32 Lsr;
	U32 Msr;
	U32 Scr;
	U32 Resv0[4];  /*!< reserved			*/
	U32 Srbr[16]; /*!< srbr, sthr			*/
	U32 Far;
	U32 Tfr;
	U32 Rfw;
	U32 Usr;
	U32 Tfl;
	U32 Rfl;
	U32 Srr;
	U32 Srts;
	U32 Sbcr;
	U32 Sdmam;
	U32 Sfe;
	U32 Srt;
	U32 Stet;
	U32 Htx;
	U32 Dmasa;
	U32 Resv1[18]; /*!< reserved			*/
	U32 Cpr;
	U32 Ucv;
	U32 Ctr;
} UartReg;

int Uart_Init(void);
void Uart_SendChar(unsigned char ch);

#endif
