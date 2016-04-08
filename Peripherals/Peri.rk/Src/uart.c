/**
 *******************************************************************************
 * @file       uart.c
 * @version    V1.0    
 * @date       2014.12.31
 * @brief      Uart source file
 * @details    This file provides all the uart firmware functions.
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

OS_MutexID	mut_uart;
UartReg *pUart = (UartReg *)UART_DBG_BASE;

void Uart_SendChar(unsigned char ch)
{
	U32 tmo = 10 * 1000; // 10 ms

	while(--tmo && ((pUart->Usr & 0x01) || !(pUart->Usr & 0x02)))
		CoUdelay(1);
	
	pUart->Rbr = (U32)ch;
}

int Uart_Init(void)
{
	mut_uart = CoCreateMutex();
	if(mut_uart == E_CREATE_FAIL)
		return E_CREATE_FAIL;
#if 0
	pUart->Lcr = 0x83;
	pUart->Rbr = 0x0d;
	pUart->Lcr = 0x03;
	pUart->Sfe = 0x01;

#endif
	return E_OK;
}

