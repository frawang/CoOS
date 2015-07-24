/**
 *******************************************************************************
 * @file       cru.c
 * @version    V1.0    
 * @date       2014.12.31
 * @brief      cru source file
 * @details    This file provides all the cru firmware functions.
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

CruReg * pCru = (CruReg *)CRU_BASE;

U32 Cru_GetPllFreq(PLL_ID pll_id)
{
	U32 freq = 0;
	if (((pCru->Pll_Con[pll_id][3] >> 8)&0x3) == NORMAL_MODE)	/* Normal mode*/
		freq = 24 * ((pCru->Pll_Con[pll_id][1] & 0x1fff) + 1)	/* NF */
		    / ((pCru->Pll_Con[pll_id][0] & 0xf)+1)/(((pCru->Pll_Con[pll_id][0] >> 8)&0x3f)+1);
	else if(((pCru->Pll_Con[pll_id][3] >> 8)&0x3) == SLOW_MODE)/* slow mode*/
		freq = 24;
	return (freq * 1000000);
}

U32 Cru_GetMcuFreq(void)
{
	PLL_ID mcu_pll;
	U32 freq;

	if((pCru->Clkgate_Con[13] & (0x03<<11)) != 0) {
		freq = 0; /* mcu fclk or hclk off */
	} else {
		/*MCU source 0:CODEC PLL  1:GENERAL PLL*/
		mcu_pll = ((pCru->Clksel_Con[12] & (0x01<<7)) == 0) ? CPLL : GPLL;

		freq = Cru_GetPllFreq(mcu_pll);
		freq /= ((pCru->Clksel_Con[12] & 0x1f) + 1);
	}

	return freq;
}

U32 Cru_SetMcuFreq(U32 freq)
{
	PLL_ID mcu_pll;
	U32 freq_real, freq_pll;
	U32 div;

	mcu_pll = GPLL;
//    freq_pll = Cru_GetPllFreq(mcu_pll);
#if 1
    do
    {
        freq_pll = Cru_GetPllFreq(mcu_pll);
    }while(freq_pll != 576000000);
#endif

	div = DIV_ROUND_UP(freq_pll, freq);

	pCru->Clksel_Con[12] = (0x1f << (0 + 16)) | (div - 1);
	pCru->Clksel_Con[12] = (0x01 << (7 + 16)) | (0x01 << 7); /* Select gPLL 384Mhz */

	freq_real = freq_pll / ((pCru->Clksel_Con[12] & 0x1f) + 1);
	return freq_real;
}

#if 0
/*MCU freq and release reset singel are set by uboot */
int Cru_Init(void)
{
	U32 freq = Cru_GetPllFreq(GPLL)  / ((pCru->Clksel_Con[12] & 0x1f) + 1);

//	printf("MCU: Cru: Try to set %uMhz, real %uMhz\n\r", CFG_CPU_FREQ / 1000000, freq / 1000000);

	return E_OK;
}
#endif
