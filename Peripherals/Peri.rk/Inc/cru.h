/**
 *******************************************************************************
 * @file       cru.h
 * @version    V1.0    
 * @date       2014.12.31
 * @brief      Cru header file
 * @details    This file including some defines and declares related to cru.
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
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND ConTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR ConTRIBUTORS BE 
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
 *  ConSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 *  ConTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF 
 *  THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * <h2><center>&copy; COPYRIGHT 2014 Fuzhou Rockchip Electronics Co., Ltd </center></h2>
 *******************************************************************************
 */ 

#ifndef __CRU_H
#define __CRU_H

typedef enum PLL_ID_Tag
{
    APLLB = 0,
    APLLL,
    DPLL,
    CPLL,
    GPLL,
    NPLL,
    PLL_MAX
}PLL_ID;

typedef enum PLL_MODE_Tag
{
    SLOW_MODE = 0,
    NORMAL_MODE,
    DEEP_SLOW_MODE,
    MODE_MAX
}PLL_MODE;

typedef volatile struct {
	U32 Pll_Con[6][4];
	U32 Resv0[40];
	U32 Clksel_Con[56];
	U32 Resv1[8];
	U32 Clkgate_Con[25];
	U32 Resv2[7];
	U32 Glb_Srst_Fst_Value;
	U32 Glb_Srst_Snd_Value;
	U32 Resv3[30];
	U32 Softrst_Con[15];
	U32 Resv4[17];
	U32 Misc_Con;
	U32 Glb_Cnt_Th;
	U32 Glb_Rst_Con;
	U32 Glb_Rst_St;
	U32 Resv5[28];
	U32 Sdmmc_Con0;
	U32 Sdmmc_Con1;
	U32 Sdio0_Con0;
	U32 Sdio0_Con1;
	U32 Sdio1_Con0;
	U32 Sdio1_Con1;
	U32 Emmc_Con0;
	U32 Emmc_Con1;
} CruReg;

int Cru_Init(void);
U32 Cru_GetMcuFreq(void);
U32 Cru_SetMcuFreq(U32 freq);
U32 Cru_GetPllFreq(PLL_ID pll_id);

#endif
