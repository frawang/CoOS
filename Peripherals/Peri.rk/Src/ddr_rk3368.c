#include <peri.h>


__sramdata static BACKUP_REG_T ddr_reg;
__sramdata static BACKUP_REG_T *p_ddr_reg;
__sramdata static uint32 ddr_freq;
__sramdata static uint32 loops_per_us;

__sramdata static uint32 clk_gate[CLK_GATE_NUM];
__sramdata static uint8 mmu_status;
__sramdata static uint8 vop_status;
__sramdata static uint8 id_mipi_dis;

static const uint8 ddr3_cl_cwl[22][7]={
/*speed   0~330         331~400       401~533        534~666       667~800        801~933      934~1066
 * tCK    >3            2.5~3         1.875~2.5      1.5~1.875     1.25~1.5       1.07~1.25    0.938~1.07
 *        cl<<4, cwl    cl<<4, cwl    cl<<4, cwl              */
         {((5<<4)|5),   ((5<<4)|5),   0         ,    0,            0,             0,            0}, //DDR3_800D (5-5-5)
         {((5<<4)|5),   ((6<<4)|5),   0         ,    0,            0,             0,            0}, //DDR3_800E (6-6-6)

         {((5<<4)|5),   ((5<<4)|5),   ((6<<4)|6),    0,            0,             0,            0}, //DDR3_1066E (6-6-6)
         {((5<<4)|5),   ((6<<4)|5),   ((7<<4)|6),    0,            0,             0,            0}, //DDR3_1066F (7-7-7)
         {((5<<4)|5),   ((6<<4)|5),   ((8<<4)|6),    0,            0,             0,            0}, //DDR3_1066G (8-8-8)

         {((5<<4)|5),   ((5<<4)|5),   ((6<<4)|6),    ((7<<4)|7),   0,             0,            0}, //DDR3_1333F (7-7-7)
         {((5<<4)|5),   ((5<<4)|5),   ((7<<4)|6),    ((8<<4)|7),   0,             0,            0}, //DDR3_1333G (8-8-8)
         {((5<<4)|5),   ((6<<4)|5),   ((8<<4)|6),    ((9<<4)|7),   0,             0,            0}, //DDR3_1333H (9-9-9)
         {((5<<4)|5),   ((6<<4)|5),   ((8<<4)|6),    ((10<<4)|7),  0,             0,            0}, //DDR3_1333J (10-10-10)

         {((5<<4)|5),   ((5<<4)|5),   ((6<<4)|6),    ((7<<4)|7),   ((8<<4)|8),    0,            0}, //DDR3_1600G (8-8-8)
         {((5<<4)|5),   ((5<<4)|5),   ((6<<4)|6),    ((8<<4)|7),   ((9<<4)|8),    0,            0}, //DDR3_1600H (9-9-9)
         {((5<<4)|5),   ((5<<4)|5),   ((7<<4)|6),    ((9<<4)|7),   ((10<<4)|8),   0,            0}, //DDR3_1600J (10-10-10)
         {((5<<4)|5),   ((6<<4)|5),   ((8<<4)|6),    ((10<<4)|7),  ((11<<4)|8),   0,            0}, //DDR3_1600K (11-11-11)

         {((5<<4)|5),   ((5<<4)|5),   ((6<<4)|6),    ((8<<4)|7),   ((9<<4)|8),    ((11<<4)|9),  0}, //DDR3_1866J (10-10-10)
         {((5<<4)|5),   ((5<<4)|5),   ((7<<4)|6),    ((8<<4)|7),   ((10<<4)|8),   ((11<<4)|9),  0}, //DDR3_1866K (11-11-11)
         {((6<<4)|5),   ((6<<4)|5),   ((7<<4)|6),    ((9<<4)|7),   ((11<<4)|8),   ((12<<4)|9),  0}, //DDR3_1866L (12-12-12)
         {((6<<4)|5),   ((6<<4)|5),   ((8<<4)|6),    ((10<<4)|7),  ((11<<4)|8),   ((13<<4)|9),  0}, //DDR3_1866M (13-13-13)

         {((5<<4)|5),   ((5<<4)|5),   ((6<<4)|6),    ((7<<4)|7),   ((9<<4)|8),    ((10<<4)|9),  ((11<<4)|10)}, //DDR3_2133K (11-11-11)
         {((5<<4)|5),   ((5<<4)|5),   ((6<<4)|6),    ((8<<4)|7),   ((9<<4)|8),    ((11<<4)|9),  ((12<<4)|10)}, //DDR3_2133L (12-12-12)
         {((5<<4)|5),   ((5<<4)|5),   ((7<<4)|6),    ((9<<4)|7),   ((10<<4)|8),   ((12<<4)|9),  ((13<<4)|10)}, //DDR3_2133M (13-13-13)
         {((6<<4)|5),   ((6<<4)|5),   ((7<<4)|6),    ((9<<4)|7),   ((11<<4)|8),   ((13<<4)|9),  ((14<<4)|10)},  //DDR3_2133N (14-14-14)

         {((10<<4)|7),   ((10<<4)|7),   ((10<<4)|7),    ((10<<4)|7),  ((11<<4)|8),   ((13<<4)|9),  ((14<<4)|10)} //DDR3_DEFAULT
};

static const uint8 ddr3_cl_cwl_3328[22][7]={
/*speed   0~330         331~400       401~533        534~666       667~800        801~933      934~1066
 * tCK    >3            2.5~3         1.875~2.5      1.5~1.875     1.25~1.5       1.07~1.25    0.938~1.07
 *        cl<<4, cwl    cl<<4, cwl    cl<<4, cwl              */
         {((5<<4)|5),   ((5<<4)|5),   0         ,    0,            0,             0,            0}, //DDR3_800D (5-5-5)
         {((5<<4)|5),   ((6<<4)|5),   0         ,    0,            0,             0,            0}, //DDR3_800E (6-6-6)

         {((5<<4)|5),   ((5<<4)|5),   ((6<<4)|6),    0,            0,             0,            0}, //DDR3_1066E (6-6-6)
         {((5<<4)|5),   ((6<<4)|5),   ((7<<4)|6),    0,            0,             0,            0}, //DDR3_1066F (7-7-7)
         {((5<<4)|5),   ((6<<4)|5),   ((8<<4)|6),    0,            0,             0,            0}, //DDR3_1066G (8-8-8)

         {((5<<4)|5),   ((5<<4)|5),   ((6<<4)|6),    ((7<<4)|7),   0,             0,            0}, //DDR3_1333F (7-7-7)
         {((5<<4)|5),   ((5<<4)|5),   ((7<<4)|6),    ((8<<4)|7),   0,             0,            0}, //DDR3_1333G (8-8-8)
         {((5<<4)|5),   ((6<<4)|5),   ((8<<4)|6),    ((9<<4)|7),   0,             0,            0}, //DDR3_1333H (9-9-9)
         {((5<<4)|5),   ((6<<4)|5),   ((8<<4)|6),    ((10<<4)|7),  0,             0,            0}, //DDR3_1333J (10-10-10)

         {((5<<4)|5),   ((5<<4)|5),   ((6<<4)|6),    ((7<<4)|7),   ((8<<4)|8),    0,            0}, //DDR3_1600G (8-8-8)
         {((5<<4)|5),   ((5<<4)|5),   ((6<<4)|6),    ((8<<4)|7),   ((9<<4)|8),    0,            0}, //DDR3_1600H (9-9-9)
         {((5<<4)|5),   ((5<<4)|5),   ((7<<4)|6),    ((9<<4)|7),   ((10<<4)|8),   0,            0}, //DDR3_1600J (10-10-10)
         {((5<<4)|5),   ((6<<4)|5),   ((8<<4)|6),    ((10<<4)|7),  ((11<<4)|8),   0,            0}, //DDR3_1600K (11-11-11)

         {((5<<4)|5),   ((5<<4)|5),   ((6<<4)|6),    ((8<<4)|7),   ((9<<4)|8),    ((11<<4)|9),  0}, //DDR3_1866J (10-10-10)
         {((5<<4)|5),   ((5<<4)|5),   ((7<<4)|6),    ((8<<4)|7),   ((10<<4)|8),   ((11<<4)|9),  0}, //DDR3_1866K (11-11-11)
         {((6<<4)|5),   ((6<<4)|5),   ((7<<4)|6),    ((9<<4)|7),   ((11<<4)|8),   ((12<<4)|9),  0}, //DDR3_1866L (12-12-12)
         {((6<<4)|5),   ((6<<4)|5),   ((8<<4)|6),    ((10<<4)|7),  ((11<<4)|8),   ((13<<4)|9),  0}, //DDR3_1866M (13-13-13)

         {((5<<4)|5),   ((5<<4)|5),   ((6<<4)|6),    ((7<<4)|7),   ((9<<4)|8),    ((10<<4)|9),  ((11<<4)|10)}, //DDR3_2133K (11-11-11)
         {((5<<4)|5),   ((5<<4)|5),   ((6<<4)|6),    ((8<<4)|7),   ((9<<4)|8),    ((11<<4)|9),  ((12<<4)|10)}, //DDR3_2133L (12-12-12)
         {((5<<4)|5),   ((5<<4)|5),   ((7<<4)|6),    ((9<<4)|7),   ((10<<4)|8),   ((12<<4)|9),  ((13<<4)|10)}, //DDR3_2133M (13-13-13)
         {((6<<4)|5),   ((6<<4)|5),   ((7<<4)|6),    ((9<<4)|7),   ((11<<4)|8),   ((13<<4)|9),  ((14<<4)|10)},  //DDR3_2133N (14-14-14)

         {((6<<4)|5),   ((6<<4)|5),   ((7<<4)|6),    ((9<<4)|7),  ((11<<4)|8),   ((13<<4)|9),  ((14<<4)|10)} //DDR3_DEFAULT
};

static const uint16 ddr3_tRC_tFAW[22]={
/**    tRC    tFAW   */
    ((50<<8)|50), //DDR3_800D (5-5-5)
    ((53<<8)|50), //DDR3_800E (6-6-6)

    ((49<<8)|50), //DDR3_1066E (6-6-6)
    ((51<<8)|50), //DDR3_1066F (7-7-7)
    ((53<<8)|50), //DDR3_1066G (8-8-8)

    ((47<<8)|45), //DDR3_1333F (7-7-7)
    ((48<<8)|45), //DDR3_1333G (8-8-8)
    ((50<<8)|45), //DDR3_1333H (9-9-9)
    ((51<<8)|45), //DDR3_1333J (10-10-10)

    ((45<<8)|40), //DDR3_1600G (8-8-8)
    ((47<<8)|40), //DDR3_1600H (9-9-9)
    ((48<<8)|40), //DDR3_1600J (10-10-10)
    ((49<<8)|40), //DDR3_1600K (11-11-11)

    ((45<<8)|35), //DDR3_1866J (10-10-10)
    ((46<<8)|35), //DDR3_1866K (11-11-11)
    ((47<<8)|35), //DDR3_1866L (12-12-12)
    ((48<<8)|35), //DDR3_1866M (13-13-13)

    ((44<<8)|35), //DDR3_2133K (11-11-11)
    ((45<<8)|35), //DDR3_2133L (12-12-12)
    ((46<<8)|35), //DDR3_2133M (13-13-13)
    ((47<<8)|35), //DDR3_2133N (14-14-14)

    ((53<<8)|50)  //DDR3_DEFAULT
};

__sramfunc static void ddr_delayus(uint32 us)
{
    uint32 start_value, end_value,delay_loop;
    volatile uint32 *timer_value;
    volatile uint32 timer_ctrl;

    uint32 i;
    if(us ==1)
    {
        i = 1;
        asm volatile (".align 4; 1: subs %0, %0, #1; bne 1b;":"+r" (i));
        return;
    }
    
    start_value = *(uint32*)0xE000E018;
    timer_value = (uint32 *)0xE000E018;
    us -= 2;
    delay_loop = loops_per_us * us;

    if(delay_loop > start_value)
    {
        timer_ctrl = *(volatile uint32 *)0xE000E010;/*read and clear count flag*/
        dsb();
        end_value = *(uint32*)0xE000E014 - (delay_loop - start_value)+1;
        while(((*(volatile uint32*)0xE000E010) & (0x1<<16)) == 0);/*wait timer count to 0*/
        do{
            if((*timer_value <end_value))
                break;
        }while(1);
    }
    else
    {
        end_value = start_value - delay_loop;
        while(*timer_value > end_value);
    }
}

sramlocalfunc static void ddr_move_to_Lowpower_state(void)
{
	volatile uint32 value;

	pGRF_Reg->GRF_DDRC0_CON0 = (1 << 16 | 1);	/*hw_wakeup :disable auto sr*/

	while (1) {
		value = pDDR_Reg->STAT.b.ctl_stat;
		if (value == Low_power) {
			break;
		}
		switch (value) {
		case Init_mem:
			pDDR_Reg->SCTL = CFG_STATE;
			dsb();
			while ((pDDR_Reg->STAT.b.ctl_stat) != Config)
			;
		case Config:
			pDDR_Reg->SCTL = GO_STATE;
			dsb();
			while ((pDDR_Reg->STAT.b.ctl_stat) != Access)
			;
		case Access:
			pDDR_Reg->SCTL = SLEEP_STATE;
			dsb();
			while ((pDDR_Reg->STAT.b.ctl_stat) != Low_power)
			;
			break;
		default:	/*Transitional state*/
			break;
		}
	}
}

sramlocalfunc static  void ddr_move_to_Access_state(void)
{
	volatile uint32 value;

	/*set auto self-refresh idle*/
	pDDR_Reg->MCFG1 =
	    (pDDR_Reg->MCFG1 & 0xffffff00) | ddr_reg.ddr_sr_idle | (1 << 31);
	pDDR_Reg->MCFG = (pDDR_Reg->MCFG & 0xffff00ff) | (PD_IDLE << 8);
	while (1) {
		value = pDDR_Reg->STAT.b.ctl_stat;
		if ((value == Access)
		    || ((pDDR_Reg->STAT.b.lp_trig == 1)
			&& ((pDDR_Reg->STAT.b.ctl_stat) == Low_power))) {
			break;
		}
		switch (value) {
		case Low_power:
			pDDR_Reg->SCTL = WAKEUP_STATE;
			dsb();
			while ((pDDR_Reg->STAT.b.ctl_stat) != Access)
			;
			break;
		case Init_mem:
			pDDR_Reg->SCTL = CFG_STATE;
			dsb();
			while ((pDDR_Reg->STAT.b.ctl_stat) != Config)
			;
		case Config:
			pDDR_Reg->SCTL = GO_STATE;
			dsb();
			while (!(((pDDR_Reg->STAT.b.ctl_stat) == Access)
				 || ((pDDR_Reg->STAT.b.lp_trig == 1)
				     && ((pDDR_Reg->STAT.b.ctl_stat) ==
					 Low_power))))
		    ;
			break;
		default:	/*Transitional state*/
			break;
		}
	}
	pGRF_Reg->GRF_DDRC0_CON0 = (1 << 16 | 0);	/*de_hw_wakeup :enable auto sr if sr_idle != 0*/
}

sramlocalfunc static void ddr_move_to_Config_state(void)
{
	volatile uint32 value;
	pGRF_Reg->GRF_DDRC0_CON0 = (1 << 16 | 1);	/*hw_wakeup :disable auto sr*/
	while (1) {
		value = pDDR_Reg->STAT.b.ctl_stat;
		if (value == Config) {
			break;
		}
		switch (value) {
		case Low_power:
			pDDR_Reg->SCTL = WAKEUP_STATE;
			dsb();
		case Access:
		case Init_mem:
			pDDR_Reg->SCTL = CFG_STATE;
			dsb();
			break;
		default:	/*Transitional state*/
			break;
		}
	}
}

sramlocalfunc static void  ddr_send_command(uint32 rank, uint32 cmd, uint32 arg)
{
    while (pDDR_Reg->MCMD & start_cmd)
	;
	pDDR_Reg->MCMD = (start_cmd | (rank << 20) | arg | cmd);
	dsb();
}

sramlocalfunc static uint32  ddr_data_training(void)
{
	uint32 value, dram_bw;
	value = pDDR_Reg->TREFI;
	pDDR_Reg->TREFI = (0x1<<31);
	dram_bw = (pPHY_Reg->PHY_REG0 >> 4) & 0xf;
	//pPHY_Reg->PHY_REG2 = PHY_DATA_TRAINING_SELECTCS(PHY_SELECT_CS0) | PHY_DTT_EN; /*training cs0*/
    pPHY_Reg->PHY_REG2 = 0x21;
	/*wait echo byte DTDONE*/
	ddr_delayus(1);
	/*stop DTT*/
	while ((pPHY_Reg->PHY_REGff & 0xf) != dram_bw)
	;
//	pPHY_Reg->PHY_REG2 = PHY_DATA_TRAINING_SELECTCS(PHY_SELECT_CS0);
    pPHY_Reg->PHY_REG2 = 0x20;
	/*send some auto refresh to complement the lost while DTT*/
	ddr_send_command(3, PREA_cmd, 0);
	ddr_send_command(3, REF_cmd, 0);
	ddr_send_command(3, REF_cmd, 0);
    ddr_send_command(3, REF_cmd, 0);
    ddr_send_command(3, REF_cmd, 0);

    pDDR_Reg->TREFI = value | (0x1<<31);
	return 0;
}

sramlocalfunc static void  ddr_set_dll_bypass(uint32 freq)
{
	uint32 phase;
	if (freq < 680) {
		phase = 3;
	} else {
		phase = 2;
	}
	pPHY_Reg->PHY_REG28 = phase;	/*rx dll 45°delay*/
	pPHY_Reg->PHY_REG38 = phase;	/*rx dll 45°delay*/
	pPHY_Reg->PHY_REG48 = phase;	/*rx dll 45°delay*/
	pPHY_Reg->PHY_REG58 = phase;	/*rx dll 45°delay*/
	if (freq <= PHY_DLL_DISABLE_FREQ) {
		pPHY_Reg->PHY_REGDLL |= 0x1F;	/*TX DLL bypass */
	} else {
		pPHY_Reg->PHY_REGDLL &= ~0x1F;	/* TX DLL bypass*/
	}

	dsb();
}

static uint32 ddr_get_pll_freq(PLL_ID pll_id)	/*APLL-1;CPLL-2;DPLL-3;GPLL-4*/
{
	uint32 ret = 0;/*deep slow mode 32.768KHz*/

	if (((pCRU_Reg->CRU_PLL_CON[pll_id][3] >> 8)&0x3) == NORMAL_MODE)	/* Normal mode*/
		ret = 24 * ((pCRU_Reg->CRU_PLL_CON[pll_id][1] & 0x1fff) + 1)	/* NF */
		    / ((pCRU_Reg->CRU_PLL_CON[pll_id][0] & 0xf)+1)/(((pCRU_Reg->CRU_PLL_CON[pll_id][0] >> 8)&0x3f)+1);
	else if(((pCRU_Reg->CRU_PLL_CON[pll_id][3] >> 8)&0x3) == SLOW_MODE)/* slow mode*/
		ret = 24;
	return ret;
}

static uint32 ddr_get_phy_pll_freq(void)
{
    uint32 ret = 0;
    uint32 fb_div, pre_div;

    fb_div = (pPHY_Reg->PHY_REGec & 0xff) | ((pPHY_Reg->PHY_REGed & 0x1)<<8);
    pre_div = pPHY_Reg->PHY_REGee & 0xff;
    ret = 2 * 24 * fb_div / (4 * pre_div);
    
    return ret;
}

uint32 ddr_get_dram_freq(void)
{
    uint32 ret;
    
    if((pPHY_Reg->PHY_REGef & 0x1) == SELECT_PHY_PLL)
    {
        ret = ddr_get_phy_pll_freq();
    }
    else
    {
        ret = ddr_get_pll_freq(DPLL) / 2;
    }
    
    return ret;
}

static uint32 ddr_get_MCU_freq(void)
{
    PLL_ID  mcu_pll;
    uint32 mcu_freq;
    if((pCRU_Reg->CRU_CLKGATE_CON[13] & (0x3<<11)) != 0)
    {
        /*mcu fclk or hclk off*/
        mcu_freq = 0;
    }
    else
    {
        /*MCU source 0:CODEC PLL  1:GENERAL PLL*/
        mcu_pll = ((pCRU_Reg->CRU_CLKSEL_CON[12] & (0x1<<7)) == 0) ? CPLL : GPLL;
        mcu_freq = ddr_get_pll_freq(mcu_pll);
        mcu_freq /= ((pCRU_Reg->CRU_CLKSEL_CON[12] & 0x1f)+1);
    }
    return mcu_freq;
}
__sramdata static uint32 NO;
__sramdata static uint32 NR;
__sramdata static uint32 NF;

__sramfunc static uint32  ddr_setSys_pll(uint32 nMHz, uint32 set)
{
	uint32 ret;

    if (!set) {
        if (nMHz <= 150) {   /*实际输出频率<300*/
            NO = 6;
        } else if (nMHz <= 250) {
            NO = 4;
        } else if (nMHz <= 500) {
            NO = 2;
        } else {
            NO = 1;
        }
        NR = 1;
        NF = 2 * nMHz * NR * NO / 24;
        ret = 24*NF/NO/NR/2;
    } else {
        pCRU_Reg->CRU_PLL_CON[DPLL][3] = PLLMODE(SLOW_MODE);/*slow mode*/
        pCRU_Reg->CRU_PLL_CON[DPLL][3] = PLL_POWER_UP;
        pCRU_Reg->CRU_PLL_CON[DPLL][3] = PLL_RESET;
        pCRU_Reg->CRU_PLL_CON[DPLL][0] = SET_NR(NR)|SET_NO(NO);
        pCRU_Reg->CRU_PLL_CON[DPLL][1] = SET_NF(NF);
        pCRU_Reg->CRU_PLL_CON[DPLL][2] = SET_NB(NF/2);//NB recommended NF/2
        ddr_delayus(6);
        pCRU_Reg->CRU_PLL_CON[DPLL][3] = PLL_DE_RESET;
        //dsb;
        ret = nMHz;
        //while((pCRU_Reg->CRU_PLL_CON[DPLL][1] & (0x1<<31))==0); /*wait for pll locked*/

        //pCRU_Reg->CRU_PLL_CON[DPLL][3] = PLLMODE(NORMAL_MODE);
    }
	return ret;
}

static uint32 ddr_setPHY_pll(uint32 nMHz)
{
    uint32 ret;
    uint16 i;
    uint16 fb_div;
    uint16 pre_div;

    pre_div = 1;
    fb_div = nMHz / 12;
    ret = 2 * 24 * fb_div / (4 * pre_div);

    pPHY_Reg->PHY_REGed |= PHY_PLL_PD;
    pPHY_Reg->PHY_REGec = (fb_div & 0xff);
    pPHY_Reg->PHY_REGed = (pPHY_Reg->PHY_REGed & 0xfe) | ((fb_div >> 8)&0x1); /*fb_div bit 8*/
    pPHY_Reg->PHY_REGee = pre_div;
    pPHY_Reg->PHY_REGed &= (~PHY_PLL_PD);
//    pPHY_Reg->PHY_REGef = SELECT_PHY_PLL;
    while (1) {
        if (((pGRF_Reg->GRF_SOC_STATUS[0] >> 15) & 0x1) == 1) {
            for(i = 0; i<2; i++);
            break;
        }
    }

    return ret;
}


static uint32 ddr_get_parameter(uint32 nMHz)
{
    U32 tmp;
    U32 ret = 0;
    U32 al;
    U32 bl;
    U32 cl;
    U32 cwl;
    U32 bl_tmp;
    PCTL_TIMING_T *p_pctl_timing = &(p_ddr_reg->pctl_timing);
    NOC_TIMING_T *p_noc_timing = &(p_ddr_reg->noc_timing);

    p_pctl_timing->togcnt1u = nMHz / 2;
    p_pctl_timing->togcnt100n = nMHz / 20;
    p_pctl_timing->tinit = 200;

    if (p_ddr_reg->mem_type == DDR3) {
        if (p_ddr_reg->ddr_speed_bin > DDR3_DEFAULT) {
            ret = -1;
            goto out;
        }
        #define DDR3_tREFI_7_8_us    (78)
        #define DDR3_tMRD            (4)
        #define DDR3_tRFC_512Mb      (90)
        #define DDR3_tRFC_1Gb        (110)
        #define DDR3_tRFC_2Gb        (160)
        #define DDR3_tRFC_4Gb        (300)
        #define DDR3_tRFC_8Gb        (350)
        #define DDR3_tRTW            (2)	/*register min valid value*/
        #define DDR3_tRAS            (37)
        #define DDR3_tRRD            (10)
        #define DDR3_tRTP            (7)
        #define DDR3_tWR             (15)
        #define DDR3_tWTR            (7)
        #define DDR3_tXP             (7)
        #define DDR3_tXPDLL          (24)
        #define DDR3_tZQCS           (80)
        #define DDR3_tZQCSI          (10000)
        #define DDR3_tDQS            (1)
        #define DDR3_tCKSRE          (10)
        #define DDR3_tCKE_400MHz     (7)
        #define DDR3_tCKE_533MHz     (6)
        #define DDR3_tMOD            (15)
        #define DDR3_tRSTL           (100)
        #define DDR3_tZQCL           (320)
        #define DDR3_tDLLK           (512)
        p_pctl_timing->trsth = 500;
        al = 0;
        bl = 8;
        if (nMHz <= 330) {
            tmp = 0;
        } else if (nMHz <= 400) {
            tmp = 1;
        } else if (nMHz <= 533) {
            tmp = 2;
        } else if (nMHz <= 666) {
            tmp = 3;
        } else if (nMHz <= 800) {
            tmp = 4;
        } else if (nMHz <= 933) {
            tmp = 5;
        } else {
            tmp = 6;
        }
        /*3368*/
		if(pDDR_Reg->TDPD == 1)
		{
	        if (nMHz <= DDR3_DDR2_DLL_DISABLE_FREQ) {   /*when dll bypss cl = cwl = 6*/
	            cl = 10;
	            cwl = 7;
	        } else {
	            cl = ddr3_cl_cwl[p_ddr_reg->ddr_speed_bin][tmp] >> 4;
	            cwl =
	                ddr3_cl_cwl[p_ddr_reg->ddr_speed_bin][tmp] & 0xf;
	        }
        }
        else
        {
	        if (nMHz <= DDR3_DDR2_DLL_DISABLE_FREQ) {   /*when dll bypss cl = cwl = 6*/
	            cl = 6;
	            cwl = 6;
	        } else {
	            cl = ddr3_cl_cwl_3328[p_ddr_reg->ddr_speed_bin][tmp] >> 4;
	            cwl =
	                ddr3_cl_cwl_3328[p_ddr_reg->ddr_speed_bin][tmp] & 0xf;
	        }
        }
        if (cl == 0) {
            ret = -4;   /*超过颗粒的最大频率*/
        }
        if (nMHz <= DDR3_DDR2_ODT_DISABLE_FREQ) {
            p_ddr_reg->ddrMR[1] = DDR3_DS_40 | DDR3_Rtt_Nom_DIS;
        } else {
            p_ddr_reg->ddrMR[1] = DDR3_DS_40 | DDR3_Rtt_Nom_120;
        }
        p_ddr_reg->ddrMR[2] = DDR3_MR2_CWL(cwl) /* | DDR3_Rtt_WR_60 */ ;
        p_ddr_reg->ddrMR[3] = 0;
        /**************************************************
        * PCTL Timing
        **************************************************/
        /*
         * tREFI, average periodic refresh interval, 7.8us
         */
        p_pctl_timing->trefi =(1<<31) | DDR3_tREFI_7_8_us;
        p_pctl_timing->trefi_mem_ddr3 = DDR3_tREFI_7_8_us * p_pctl_timing->togcnt100n;
        /*
         * tMRD, 4 tCK
         */
        p_pctl_timing->tmrd = DDR3_tMRD & 0x7;
        /*
         * tRFC, 90ns(512Mb),110ns(1Gb),160ns(2Gb),300ns(4Gb),350ns(8Gb)
         */
        if (p_ddr_reg->ddr_capability_per_die <= 0x4000000) {   /*512Mb 90ns*/
            tmp = DDR3_tRFC_512Mb;
        } else if (p_ddr_reg->ddr_capability_per_die <= 0x8000000) {    /*1Gb 110ns*/
            tmp = DDR3_tRFC_1Gb;
        } else if (p_ddr_reg->ddr_capability_per_die <= 0x10000000) {   /*2Gb 160ns*/
            tmp = DDR3_tRFC_2Gb;
        } else if (p_ddr_reg->ddr_capability_per_die <= 0x20000000) {/*4Gb 300ns*/
            tmp = DDR3_tRFC_4Gb;
        } else{     /*8Gb  350ns*/
            tmp = DDR3_tRFC_8Gb;
        }
        p_pctl_timing->trfc = (tmp * nMHz + 999) / 1000;
        /*
         * tXSR, =tDLLK=512 tCK
         */
        p_pctl_timing->texsr = DDR3_tDLLK;
        /*
         * tRP=CL
         */
        p_pctl_timing->trp = cl;
        /*
         * WrToMiss=WL*tCK + tWR + tRP + tRCD
         */
        p_noc_timing->b.WrToMiss =
            ((cwl + ((DDR3_tWR * nMHz + 999) / 1000) + cl + cl) & 0x3F);

        /*
         * tRAS, 37.5ns(400MHz)     37.5ns(533MHz)
         */
        p_pctl_timing->tras =
            (((DDR3_tRAS * nMHz + (nMHz >> 1) + 999) / 1000) & 0x3F);

        /*
         * tRC=tRAS+tRP
         */
        p_pctl_timing->trc = p_pctl_timing->tras + p_pctl_timing->trp;
 //           ((((ddr3_tRC_tFAW[p_ddr_reg->ddr_speed_bin] >> 8) * nMHz +
 //              999) / 1000) & 0x3F);
        p_noc_timing->b.ActToAct =
            ((((ddr3_tRC_tFAW[p_ddr_reg->ddr_speed_bin] >> 8) * nMHz +
               999) / 1000) & 0x3F);

        p_pctl_timing->trtw = (cl + 2 - cwl);   /*DDR3_tRTW*/
        p_noc_timing->b.RdToWr = ((cl + 2 - cwl) & 0x1F);
        p_pctl_timing->tal = al;
        p_pctl_timing->tcl = cl;
        p_pctl_timing->tcwl = cwl;
        /*
         * tRAS, 37.5ns(400MHz)     37.5ns(533MHz)
         */
        p_pctl_timing->tras =
            (((DDR3_tRAS * nMHz + (nMHz >> 1) + 999) / 1000) & 0x3F);
        /*
         * tRCD=CL
         */
        p_pctl_timing->trcd = cl;
        /*
         * tRRD = max(4nCK, 7.5ns), DDR3-1066(1K), DDR3-1333(2K), DDR3-1600(2K)
         *        max(4nCK, 10ns), DDR3-800(1K,2K), DDR3-1066(2K)
         *        max(4nCK, 6ns), DDR3-1333(1K), DDR3-1600(1K)
         *
         */
        tmp = ((DDR3_tRRD * nMHz + 999) / 1000);
        if (tmp < 4) {
            tmp = 4;
        }
        p_pctl_timing->trrd = (tmp & 0xF);
        /*
         * tRTP, max(4 tCK,7.5ns)
         */
        tmp = ((DDR3_tRTP * nMHz + (nMHz >> 1) + 999) / 1000);
        if (tmp < 4) {
            tmp = 4;
        }
        p_pctl_timing->trtp = tmp & 0xF;
        /*
         * RdToMiss=tRTP+tRP + tRCD - (BL/2 * tCK)
         */
        p_noc_timing->b.RdToMiss = ((tmp + cl + cl - (bl >> 1)) & 0x3F);
        /*
         * tWR, 15ns
         */
        tmp = ((DDR3_tWR * nMHz + 999) / 1000);
        p_pctl_timing->twr = tmp & 0x1F;
        if (tmp < 9)
            tmp = tmp - 4;
        else
            tmp = tmp >> 1;
        p_ddr_reg->ddrMR[0] = DDR3_BL8 | DDR3_CL(cl) | DDR3_WR(tmp);

        /*
         * tWTR, max(4 tCK,7.5ns)
         */
        tmp = ((DDR3_tWTR * nMHz + (nMHz >> 1) + 999) / 1000);
        if (tmp < 4) {
            tmp = 4;
        }
        p_pctl_timing->twtr = tmp & 0xF;
        p_noc_timing->b.WrToRd = ((tmp + cwl) & 0x1F);
        /*
         * tXP, max(3 tCK, 7.5ns)(<933MHz)
         */
        tmp = ((DDR3_tXP * nMHz + (nMHz >> 1) + 999) / 1000);
        if (tmp < 3) {
            tmp = 3;
        }
        p_pctl_timing->txp = tmp & 0x7;
        /*
         * tXPDLL, max(10 tCK,24ns)
         */
        tmp = ((DDR3_tXPDLL * nMHz + 999) / 1000);
        if (tmp < 10) {
            tmp = 10;
        }
        p_pctl_timing->txpdll = tmp & 0x3F;
        /*
         * tZQCS, max(64 tCK, 80ns)
         */
        tmp = ((DDR3_tZQCS * nMHz + 999) / 1000);
        if (tmp < 64) {
            tmp = 64;
        }
        p_pctl_timing->tzqcs = tmp & 0x7F;
        /*
         * tZQCSI,
         */
        p_pctl_timing->tzqcsi = DDR3_tZQCSI;
        /*
         * tDQS,
         */
        p_pctl_timing->tdqs = DDR3_tDQS;
        /*
         * tCKSRE, max(5 tCK, 10ns)
         */
        tmp = ((DDR3_tCKSRE * nMHz + 999) / 1000);
        if (tmp < 5) {
            tmp = 5;
        }
        p_pctl_timing->tcksre = tmp & 0x1F;
        /*
         * tCKSRX, max(5 tCK, 10ns)
         */
        p_pctl_timing->tcksrx = tmp & 0x1F;
        /*
         * tCKE, max(3 tCK,7.5ns)(400MHz) max(3 tCK,5.625ns)(533MHz)
         */
        if (nMHz >= 533) {
            tmp = ((DDR3_tCKE_533MHz * nMHz + 999) / 1000);
        } else {
            tmp =
                ((DDR3_tCKE_400MHz * nMHz + (nMHz >> 1) +
                  999) / 1000);
        }
        if (tmp < 3) {
            tmp = 3;
        }
        p_pctl_timing->tcke = tmp & 0x7;
        /*
         * tCKESR, =tCKE + 1tCK
         */
        p_pctl_timing->tckesr = (tmp + 1) & 0xF;
        /*
         * tMOD, max(12 tCK,15ns)
         */
        tmp = ((DDR3_tMOD * nMHz + 999) / 1000);
        if (tmp < 12) {
            tmp = 12;
        }
        p_pctl_timing->tmod = tmp & 0x1F;
        /*
         * tRSTL, 100ns
         */
        p_pctl_timing->trstl =
            ((DDR3_tRSTL * nMHz + 999) / 1000) & 0x7F;
        /*
         * tZQCL, max(256 tCK, 320ns)
         */
        tmp = ((DDR3_tZQCL * nMHz + 999) / 1000);
        if (tmp < 256) {
            tmp = 256;
        }
        p_pctl_timing->tzqcl = tmp & 0x3FF;
        /*
         * tMRR, 0 tCK
         */
        p_pctl_timing->tmrr = 0;
        /*
         * tDPD, 0
         */
        p_pctl_timing->tdpd = pDDR_Reg->TDPD;

        /**************************************************
        *NOC Timing
        **************************************************/
        p_noc_timing->b.BurstLen = ((bl >> 1) & 0x7);
    } else if (p_ddr_reg->mem_type == LPDDR2) {
        #define LPDDR2_tREFI_3_9_us    (38)	/*unit 100ns*/
        #define LPDDR2_tREFI_7_8_us    (78)	/*unit 100ns*/
        #define LPDDR2_tMRD            (5)	/*tCK*/
        #define LPDDR2_tRFC_8Gb        (210)	/*ns*/
        #define LPDDR2_tRFC_4Gb        (130)	/*ns*/
        #define LPDDR2_tRP_4_BANK               (24)	/*ns*/
        #define LPDDR2_tRPab_SUB_tRPpb_4_BANK   (0)
        #define LPDDR2_tRP_8_BANK               (27)	/*ns*/
        #define LPDDR2_tRPab_SUB_tRPpb_8_BANK   (3)
        #define LPDDR2_tRTW          (1)	/*tCK register min valid value*/
        #define LPDDR2_tRAS          (42)	/*ns*/
        #define LPDDR2_tRCD          (24)	/*ns*/
        #define LPDDR2_tRRD          (10)	/*ns*/
        #define LPDDR2_tRTP          (7)	/*ns*/
        #define LPDDR2_tWR           (15)	/*ns*/
        #define LPDDR2_tWTR_GREAT_200MHz         (7)	/*ns*/
        #define LPDDR2_tWTR_LITTLE_200MHz        (10)	/*ns*/
        #define LPDDR2_tXP           (7)	/*ns*/
        #define LPDDR2_tXPDLL        (0)
        #define LPDDR2_tZQCS         (90)	/*ns*/
        #define LPDDR2_tZQCSI        (0)
        #define LPDDR2_tDQS          (2)
        #define LPDDR2_tCKSRE        (1)	/*tCK*/
        #define LPDDR2_tCKSRX        (2)	/*tCK*/
        #define LPDDR2_tCKE          (3)	/*tCK*/
        #define LPDDR2_tMOD          (0)
        #define LPDDR2_tRSTL         (0)
        #define LPDDR2_tZQCL         (360)	/*ns*/
        #define LPDDR2_tMRR          (2)	/*tCK*/
        #define LPDDR2_tCKESR        (15)	/*ns*/
        #define LPDDR2_tDPD_US       (500)
        #define LPDDR2_tFAW_GREAT_200MHz    (50)	/*ns*/
        #define LPDDR2_tFAW_LITTLE_200MHz   (60)	/*ns*/
        #define LPDDR2_tDLLK         (2)	/*tCK*/
        #define LPDDR2_tDQSCK_MAX    (3)	/*tCK*/
        #define LPDDR2_tDQSCK_MIN    (0)	/*tCK*/
        #define LPDDR2_tDQSS         (1)	/*tCK*/

        uint32 trp_tmp;
        uint32 trcd_tmp;
        uint32 tras_tmp;
        uint32 trtp_tmp;
        uint32 twr_tmp;

        al = 0;
        if (nMHz >= 200) {
            bl = 4; /*you can change burst here*/
        } else {
            bl = 8; /* freq < 200MHz, BL fixed 8*/
        }
        /*     1066 933 800 667 533 400 333
         * RL,   8   7   6   5   4   3   3
         * WL,   4   4   3   2   2   1   1
         */
         /*
        if (nMHz <= 200) {
            cl = 3;
            cwl = 1;
            p_ddr_reg->ddrMR[2] = LPDDR2_RL3_WL1;
        } else if (nMHz <= 266) {
            cl = 4;
            cwl = 2;
            p_ddr_reg->ddrMR[2] = LPDDR2_RL4_WL2;
        } else if (nMHz <= 333) {
            cl = 5;
            cwl = 2;
            p_ddr_reg->ddrMR[2] = LPDDR2_RL5_WL2;
        } else if (nMHz <= 400) {
            cl = 6;
            cwl = 3;
            p_ddr_reg->ddrMR[2] = LPDDR2_RL6_WL3;
        } else 
        */
        if (nMHz <= 466) {
            cl = 7;
            cwl = 4;
            p_ddr_reg->ddrMR[2] = LPDDR2_RL7_WL4;
        } else {        /*(nMHz<=1066)*/
            cl = 8;
            cwl = 4;
            p_ddr_reg->ddrMR[2] = LPDDR2_RL8_WL4;
        }
        p_ddr_reg->ddrMR[3] = LPDDR2_DS_34;
        p_ddr_reg->ddrMR[0] = 0;
        /**************************************************
        * PCTL Timing
        **************************************************/
        /*
         * tREFI, average periodic refresh interval, 15.6us(<256Mb) 7.8us(256Mb-1Gb) 3.9us(2Gb-8Gb)
         */
        if (p_ddr_reg->ddr_capability_per_die >= 0x10000000) {  /*2Gb*/
            p_pctl_timing->trefi =(1<<31) | LPDDR2_tREFI_3_9_us;
        } else {
            p_pctl_timing->trefi = (1<<31) | LPDDR2_tREFI_7_8_us;
        }

        /*
         * tMRD, (=tMRW), 5 tCK
         */
        p_pctl_timing->tmrd = LPDDR2_tMRD & 0x7;
        /*
         * tRFC, 90ns(<=512Mb) 130ns(1Gb-4Gb) 210ns(8Gb)
         */
        if (p_ddr_reg->ddr_capability_per_die >= 0x40000000) {  /*8Gb*/
            p_pctl_timing->trfc =
                (LPDDR2_tRFC_8Gb * nMHz + 999) / 1000;
            /*
             * tXSR, max(2tCK,tRFC+10ns)
             */
            tmp = (((LPDDR2_tRFC_8Gb + 10) * nMHz + 999) / 1000);
        } else {
            p_pctl_timing->trfc =
                (LPDDR2_tRFC_4Gb * nMHz + 999) / 1000;
            tmp = (((LPDDR2_tRFC_4Gb + 10) * nMHz + 999) / 1000);
        }
        if (tmp < 2) {
            tmp = 2;
        }
        p_pctl_timing->texsr = tmp & 0x3FF;
        /*
         * tRP, max(3tCK, 4-bank:15ns(Fast) 18ns(Typ) 24ns(Slow), 8-bank:18ns(Fast) 21ns(Typ) 27ns(Slow))
         */
        trp_tmp = ((LPDDR2_tRP_8_BANK * nMHz + 999) / 1000);
        if (trp_tmp < 3) {
            trp_tmp = 3;
        }
        p_pctl_timing->trp =
            ((((LPDDR2_tRPab_SUB_tRPpb_8_BANK * nMHz +
            999) / 1000) & 0x3) << 16) | (trp_tmp & 0xF);

        /*
         * tRAS, max(3tCK,42ns)
         */
        tras_tmp = ((LPDDR2_tRAS * nMHz + 999) / 1000);
        if (tras_tmp < 3) {
            tras_tmp = 3;
        }
        p_pctl_timing->tras = (tras_tmp & 0x3F);

        /*
         * tRCD, max(3tCK, 15ns(Fast) 18ns(Typ) 24ns(Slow))
         */
        trcd_tmp = ((LPDDR2_tRCD * nMHz + 999) / 1000);
        if (trcd_tmp < 3) {
            trcd_tmp = 3;
        }
        p_pctl_timing->trcd = (trcd_tmp & 0xF);
        /*
         * tRTP, max(2tCK, 7.5ns)
         */
        trtp_tmp = ((LPDDR2_tRTP * nMHz + (nMHz >> 1) + 999) / 1000);
        if (trtp_tmp < 2) {
            trtp_tmp = 2;
        }
        p_pctl_timing->trtp = trtp_tmp & 0xF;
        /*
         * tWR, max(3tCK,15ns)
         */
        twr_tmp = ((LPDDR2_tWR * nMHz + 999) / 1000);
        if (twr_tmp < 3) {
            twr_tmp = 3;
        }
        p_pctl_timing->twr = twr_tmp & 0x1F;
        bl_tmp =
            (bl ==
             16) ? LPDDR2_BL16 : ((bl == 8) ? LPDDR2_BL8 : LPDDR2_BL4);
        p_ddr_reg->ddrMR[1] = bl_tmp | LPDDR2_nWR(twr_tmp);

        /*
         * WrToMiss=WL*tCK + tDQSS + tWR + tRP + tRCD
         */
        p_noc_timing->b.WrToMiss =
            ((cwl + LPDDR2_tDQSS + twr_tmp + trp_tmp +
              trcd_tmp) & 0x3F);
        /*
         * RdToMiss=tRTP + tRP + tRCD - (BL/2 * tCK)
         */
        p_noc_timing->b.RdToMiss =
            ((trtp_tmp + trp_tmp + trcd_tmp - (bl >> 1)) & 0x3F);
        /*
         * tRC=tRAS+tRP
         */
        p_pctl_timing->trc = ((tras_tmp + trp_tmp) & 0x3F);
        p_noc_timing->b.ActToAct = ((tras_tmp + trp_tmp) & 0x3F);
        /*
         * RdToWr=RL+tDQSCK-WL
         */
        p_pctl_timing->trtw = (cl + LPDDR2_tDQSCK_MAX + (bl / 2) + 1 - cwl);    /*LPDDR2_tRTW*/
        p_noc_timing->b.RdToWr =
            ((cl + LPDDR2_tDQSCK_MAX + 1 - cwl) & 0x1F);
        p_pctl_timing->tal = al;
        p_pctl_timing->tcl = cl;
        p_pctl_timing->tcwl = cwl;
        /*
         * tRRD, max(2tCK,10ns)
         */
        tmp = ((LPDDR2_tRRD * nMHz + 999) / 1000);
        if (tmp < 2) {
            tmp = 2;
        }
        p_pctl_timing->trrd = (tmp & 0xF);
        /*
         * tWTR, max(2tCK, 7.5ns(533-266MHz)  10ns(200-166MHz))
         */
        if (nMHz > 200) {
            tmp =
                ((LPDDR2_tWTR_GREAT_200MHz * nMHz + (nMHz >> 1) +
                  999) / 1000);
        } else {
            tmp = ((LPDDR2_tWTR_LITTLE_200MHz * nMHz + 999) / 1000);
        }
        if (tmp < 2) {
            tmp = 2;
        }
        p_pctl_timing->twtr = tmp & 0xF;
        /*
         * WrToRd=WL+tDQSS+tWTR
         */
        p_noc_timing->b.WrToRd = ((cwl + LPDDR2_tDQSS + tmp) & 0x1F);
        /*
         * tXP, max(2tCK,7.5ns)
         */
        tmp = ((LPDDR2_tXP * nMHz + (nMHz >> 1) + 999) / 1000);
        if (tmp < 2) {
            tmp = 2;
        }
        p_pctl_timing->txp = tmp & 0x7;
        /*
         * tXPDLL, 0ns
         */
        p_pctl_timing->txpdll = LPDDR2_tXPDLL;
        /*
         * tZQCS, 90ns
         */
        p_pctl_timing->tzqcs =
            ((LPDDR2_tZQCS * nMHz + 999) / 1000) & 0x7F;
        /*
         * tZQCSI,
         */
        /*if (pDDR_Reg->MCFG &= lpddr2_s4) {*/
        if (1) {
            p_pctl_timing->tzqcsi = LPDDR2_tZQCSI;
        } else {
            p_pctl_timing->tzqcsi = 0;
        }
        /*
         * tDQS,
         */
        p_pctl_timing->tdqs = LPDDR2_tDQS;
        /*
         * tCKSRE, 1 tCK
         */
        p_pctl_timing->tcksre = LPDDR2_tCKSRE;
        /*
         * tCKSRX, 2 tCK
         */
        p_pctl_timing->tcksrx = LPDDR2_tCKSRX;
        /*
         * tCKE, 3 tCK
         */
        p_pctl_timing->tcke = LPDDR2_tCKE;
        /*
         * tMOD, 0 tCK
         */
        p_pctl_timing->tmod = LPDDR2_tMOD;
        /*
         * tRSTL, 0 tCK
         */
        p_pctl_timing->trstl = LPDDR2_tRSTL;
        /*
         * tZQCL, 360ns
         */
        p_pctl_timing->tzqcl =
            ((LPDDR2_tZQCL * nMHz + 999) / 1000) & 0x3FF;
        /*
         * tMRR, 2 tCK
         */
        p_pctl_timing->tmrr = LPDDR2_tMRR;
        /*
         * tCKESR, max(3tCK,15ns)
         */
        tmp = ((LPDDR2_tCKESR * nMHz + 999) / 1000);
        if (tmp < 3) {
            tmp = 3;
        }
        p_pctl_timing->tckesr = tmp & 0xF;
        /*
         * tDPD, 500us
         */
        p_pctl_timing->tdpd = pDDR_Reg->TDPD;

        /*************************************************
        * NOC Timing
        **************************************************/
        p_noc_timing->b.BurstLen = ((bl >> 1) & 0x7);
    }else if (p_ddr_reg->mem_type == LPDDR3) { 
        #define LPDDR3_tREFI_3_9_us    (39)  //unit 100ns
        #define LPDDR3_tMRD            (7)   //tCK
        #define LPDDR3_tRFC_8Gb        (210)  //ns
        #define LPDDR3_tRFC_4Gb        (130)  //ns
        #define LPDDR3_tRPpb_8_BANK             (24)  //ns
        #define LPDDR3_tRPab_SUB_tRPpb_8_BANK   (3)   //ns
        #define LPDDR3_tRTW          (1)   //tCK register min valid value
        #define LPDDR3_tRAS          (42)  //ns
        #define LPDDR3_tRCD          (24)  //ns
        #define LPDDR3_tRRD          (10)  //ns
        #define LPDDR3_tRTP          (7)   //ns
        #define LPDDR3_tWR           (15)  //ns
        #define LPDDR3_tWTR          (7)  //ns
        #define LPDDR3_tXP           (7)  //ns
        #define LPDDR3_tXPDLL        (0)
        #define LPDDR3_tZQCS         (90) //ns
        #define LPDDR3_tZQCSI        (0)
        #define LPDDR3_tDQS          (4)
        #define LPDDR3_tCKSRE        (2)  //tCK
        #define LPDDR3_tCKSRX        (2)  //tCK
        #define LPDDR3_tCKE          (3)  //tCK
        #define LPDDR3_tMOD          (0)
        #define LPDDR3_tRSTL         (0)
        #define LPDDR3_tZQCL         (360)  //ns
        #define LPDDR3_tMRR          (4)    //tCK
        #define LPDDR3_tCKESR        (15)   //ns
        #define LPDDR3_tDPD_US       (500)   //us
        #define LPDDR3_tFAW          (50)  //ns
        #define LPDDR3_tDLLK         (2)  //tCK
        #define LPDDR3_tDQSCK_MAX    (3)  //tCK
        #define LPDDR3_tDQSCK_MIN    (0)  //tCK
        #define LPDDR3_tDQSS         (1)  //tCK

        uint32 trp_tmp;
        uint32 trcd_tmp;
        uint32 tras_tmp;
        uint32 trtp_tmp;
        uint32 twr_tmp;
        uint32 twr_LP3;

        al = 0;
        bl = 8;
        /* Only support Write Latency Set A here
         *     1066 933 800 733 667 600 533 400 166
         * RL,   16  14  12  11  10  9   8   6   3
         * WL,   8   8   6   6   6   5   4   3   1
         */
         /*
        if(nMHz<=166)
        {
            cl = 3;
            cwl = 1;
            p_ddr_reg->ddrMR[2] = LPDDR3_RL3_WL1;
        }
        else if(nMHz<=400)
        {
            cl = 6;
            cwl = 3;
            p_ddr_reg->ddrMR[2] = LPDDR3_RL6_WL3;
        }
        else */
        /*if(nMHz<=533)
        {
            cl = 8;
            cwl = 4;
            p_ddr_reg->ddrMR[2] = LPDDR3_RL8_WL4;
        }
        else if(nMHz<=600)
        {
            cl = 9;
            cwl = 5;
            p_ddr_reg->ddrMR[2] = LPDDR3_RL9_WL5;
        }
        else */
        /*3368*/
		if(pDDR_Reg->TDPD == 1)
        {
	        if(nMHz<=667)
	        {
	            cl = 10;
	            cwl = 6;
	            p_ddr_reg->ddrMR[2] = LPDDR3_RL10_WL6;
	        }
	        else if(nMHz<=733)
	        {
	            cl = 11;
	            cwl = 6;
	           p_ddr_reg->ddrMR[2] = LPDDR3_RL11_WL6;
	        }
	        else if(nMHz<=800)
	        {
	            cl = 12;
	            cwl = 6;
	            p_ddr_reg->ddrMR[2] = LPDDR3_RL12_WL6;
	        }
	        else if(nMHz<=933)
	        {
	            cl = 14;
	            cwl = 8;
	            p_ddr_reg->ddrMR[2] = LPDDR3_RL14_WL8;
	        }
	        else //(nMHz<=1066)
	        {
	            cl = 16;
	            cwl = 8;
	            p_ddr_reg->ddrMR[2] = LPDDR3_RL16_WL8;
	        }
		}
		else
		{
			if(nMHz<=533)
			{
				cl = 8;
				cwl = 4;
				p_ddr_reg->ddrMR[2] = LPDDR3_RL8_WL4;
			}
			else
			{
				cl = 9;
				cwl = 5;
				p_ddr_reg->ddrMR[2] = LPDDR3_RL9_WL5;
			}
		}

        p_ddr_reg->ddrMR[3] = LPDDR3_DS_34;
        if(nMHz <= DDR3_DDR2_ODT_DISABLE_FREQ)
        {
            p_ddr_reg->ddrMR11 = LPDDR3_ODT_DIS;
        }
        else
        {
            p_ddr_reg->ddrMR11 = LPDDR3_ODT_240;
        }
        p_ddr_reg->ddrMR[0] = 0;
        /**************************************************
         * PCTL Timing
         **************************************************/
        /*
         * tREFI, average periodic refresh interval, 3.9us(4Gb-16Gb)
         */
        p_pctl_timing->trefi =(1<<31) | LPDDR3_tREFI_3_9_us;

        /*
         * tMRD, (=tMRW), 10 tCK
         */
        p_pctl_timing->tmrd = LPDDR3_tMRD & 0x7;
        /*
         * tRFC, 130ns(4Gb) 210ns(>4Gb)
         */
        if(1)//(p_ddr_reg->ddr_capability_per_die > 0x20000000)   // >4Gb
        {
            p_pctl_timing->trfc = (LPDDR3_tRFC_8Gb*nMHz+999)/1000;
            /*
             * tXSR, max(2tCK,tRFC+10ns)
             */
            tmp=(((LPDDR3_tRFC_8Gb+10)*nMHz+999)/1000);
        }
        else
        {
            p_pctl_timing->trfc = (LPDDR3_tRFC_4Gb*nMHz+999)/1000;
            tmp=(((LPDDR3_tRFC_4Gb+10)*nMHz+999)/1000);
        }
        if(tmp<2)
        {
            tmp=2;
        }
        p_pctl_timing->texsr = tmp&0x3FF;

        /*
         * tRP, max(3tCK, 18ns(Fast) 21ns(Typ) 27ns(Slow))
         */
        //if(pPHY_Reg->DCR.b.DDR8BNK)
        if(1)
        {
            trp_tmp = ((LPDDR3_tRPpb_8_BANK*nMHz+999)/1000);
            if(trp_tmp<3)
            {
                trp_tmp=3;
            }
            p_pctl_timing->trp = ((((LPDDR3_tRPab_SUB_tRPpb_8_BANK*nMHz+999)/1000) & 0x3)<<16) | (trp_tmp&0x1F);
        }
        /*
         * tRAS, max(3tCK,42ns)
         */
        tras_tmp=((LPDDR3_tRAS*nMHz+999)/1000);
        if(tras_tmp<3)
        {
            tras_tmp=3;
        }
        p_pctl_timing->tras = (tras_tmp&0x3F);

        /*
         * tRCD, max(3tCK, 15ns(Fast) 18ns(Typ) 24ns(Slow))
         */
        trcd_tmp = ((LPDDR3_tRCD*nMHz+999)/1000);
        if(trcd_tmp<3)
        {
            trcd_tmp=3;
        }
        p_pctl_timing->trcd = (trcd_tmp&0x1F);

        /*
         * tRTP, max(4tCK, 7.5ns)
         */
        trtp_tmp = ((LPDDR3_tRTP*nMHz+(nMHz>>1)+999)/1000);
        if(trtp_tmp<4)
        {
            trtp_tmp = 4;
        }
        p_pctl_timing->trtp = trtp_tmp&0xF;

        /*
         * tWR, max(4tCK,15ns)
         */
        twr_tmp=((LPDDR3_tWR*nMHz+999)/1000);
        if(twr_tmp<4)
        {
            twr_tmp=4;
        }
        p_pctl_timing->twr = twr_tmp&0x1F;
        if(twr_tmp <=6)
            twr_LP3 = 6;
        else if(twr_tmp <=8)
            twr_LP3 = 8;
        else if(twr_tmp <=12)
            twr_LP3 = twr_tmp;
        else 
            twr_LP3 = 12;

        if(twr_LP3 > 9)
        {
            p_ddr_reg->ddrMR[2] |= (1<<4); /*enable nWR > 9*/
        }
        twr_LP3 = (twr_LP3 > 9) ? (twr_LP3 - 10) : (twr_LP3 - 2);
        p_ddr_reg->ddrMR[1] = LPDDR3_BL8 | LPDDR3_nWR(twr_LP3);

        /*
         * WrToMiss=WL*tCK + tWR + tRP + tRCD
         */
        p_noc_timing->b.WrToMiss = (cwl+twr_tmp+trp_tmp+trcd_tmp);
        /*
         * RdToMiss=tRTP + tRP + tRCD - (BL/2 * tCK)
         */
        p_noc_timing->b.RdToMiss = (trtp_tmp+trp_tmp+trcd_tmp-(bl>>1));
        /*
         * tRC=tRAS+tRP
         */
        p_pctl_timing->trc = ((tras_tmp+trp_tmp)&0x3F);
        p_noc_timing->b.ActToAct = (tras_tmp+trp_tmp);

        /*
         * RdToWr=(cl+2-cwl)
         */
        p_pctl_timing->trtw = (cl+2-cwl);//LPDDR2_tRTW;
        p_noc_timing->b.RdToWr = (cl+2-cwl);
        p_pctl_timing->tal = al;
        p_pctl_timing->tcl = cl;
        p_pctl_timing->tcwl = cwl;
        /*
         * tRRD, max(2tCK,10ns)
         */
        tmp=((LPDDR3_tRRD*nMHz+999)/1000);
        if(tmp<2)
        {
            tmp=2;
        }
        p_pctl_timing->trrd = (tmp&0xF);
        /*
         * tWTR, max(4tCK, 7.5ns)
         */
        tmp=((LPDDR3_tWTR*nMHz+(nMHz>>1)+999)/1000);
        if(tmp<4)
        {
            tmp=4;
        }
        p_pctl_timing->twtr = tmp&0xF;
        /*
         * WrToRd=WL+tWTR
         */
        p_noc_timing->b.WrToRd = (cwl+tmp);
        /*
         * tXP, max(3tCK,7.5ns)
         */
        tmp=((LPDDR3_tXP*nMHz+(nMHz>>1)+999)/1000);
        if(tmp<3)
        {
            tmp=3;
        }
        p_pctl_timing->txp = tmp&0x7;
        /*
         * tXPDLL, 0ns
         */
        p_pctl_timing->txpdll = LPDDR3_tXPDLL;
        /*
         * tZQCS, 90ns
         */
        p_pctl_timing->tzqcs = ((LPDDR3_tZQCS*nMHz+999)/1000)&0x7F;
        /*
         * tZQCSI,
         */
        p_pctl_timing->tzqcsi = LPDDR3_tZQCSI;
        /*
         * tDQS,
         */
        p_pctl_timing->tdqs = LPDDR3_tDQS;
        /*
         * tCKSRE=tCPDED, 2 tCK
         */
        p_pctl_timing->tcksre = LPDDR3_tCKSRE;
        /*
         * tCKSRX, 2 tCK
         */
        p_pctl_timing->tcksrx = LPDDR3_tCKSRX;
        /*
         * tCKE, (max 7.5ns,3 tCK)
         */
        tmp=((7*nMHz+(nMHz>>1)+999)/1000);
        if(tmp<LPDDR3_tCKE)
        {
            tmp=LPDDR3_tCKE;
        }
        p_pctl_timing->tcke = tmp;
        /*
         * tMOD, 0 tCK
         */
        p_pctl_timing->tmod = LPDDR3_tMOD;
        /*
         * tRSTL, 0 tCK
         */
        p_pctl_timing->trstl = LPDDR3_tRSTL;
        /*
         * tZQCL, 360ns
         */
        p_pctl_timing->tzqcl = ((LPDDR3_tZQCL*nMHz+999)/1000)&0x3FF;
        /*
         * tMRR, 4 tCK
         */
        p_pctl_timing->tmrr = LPDDR3_tMRR;
        /*
         * tCKESR, max(3tCK,15ns)
         */
        tmp = ((LPDDR3_tCKESR*nMHz+999)/1000);
        if(tmp < 3)
        {
            tmp = 3;
        }
        p_pctl_timing->tckesr = tmp&0xF;
        /*
         * tDPD, 500us
         */
        p_pctl_timing->tdpd = pDDR_Reg->TDPD;

        /**************************************************
         * NOC Timing
         **************************************************/
        p_noc_timing->b.BurstLen = (bl>>1);
    }

out:
    return ret;
}

sramlocalfunc static uint32 ddr_update_timing(void)
{
	uint32 bl_tmp;
	uint32 ret = 0;

	PCTL_TIMING_T *p_pctl_timing = &(ddr_reg.pctl_timing);
	//NOC_TIMING_T *p_noc_timing = &(ddr_reg.noc_timing);

    rk3368_ddr_memCpy((uint32 *)&(pDDR_Reg->TOGCNT1U),
	         (uint32 *)&(p_pctl_timing->togcnt1u), 35);
	pPHY_Reg->PHY_REGb = ((p_pctl_timing->tcl << 4) | (p_pctl_timing->tal));
	pPHY_Reg->PHY_REGc = p_pctl_timing->tcwl;
	//*(volatile uint32 *)SysSrv_DdrTiming = p_noc_timing->d32;
	/*Update PCTL BL*/
	if (ddr_reg.mem_type == DDR3) {
		pDDR_Reg->MCFG = (pDDR_Reg->MCFG &
		     (~(0x1 | (0x3 << 18) | (0x1 << 17) | (0x1 << 16))))
		    | ddr2_ddr3_bl_8 | tfaw_cfg(5) | pd_exit_slow | pd_type(1);
		pDDR_Reg->DFITRDDATAEN = (pDDR_Reg->TAL + pDDR_Reg->TCL -1)/2 - 1;	/*trdata_en = rl-3*/
		pDDR_Reg->DFITPHYWRLAT = (pDDR_Reg->TCWL - 1)/2 - 1;
	} else if (ddr_reg.mem_type == LPDDR2) {
		if ((ddr_reg.ddrMR[1] & 0x7) == LPDDR2_BL8) {
			bl_tmp = mddr_lpddr2_bl_8;
			pPHY_Reg->PHY_REG1 |= PHY_BL_8;
		} else if ((ddr_reg.ddrMR[1] & 0x7) == LPDDR2_BL4) {
			bl_tmp = mddr_lpddr2_bl_4;
			pPHY_Reg->PHY_REG1 &= (~PHY_BL_8);
		} else{		/*if((ddr_reg.ddrMR[1] & 0x7) == LPDDR2_BL16)*/
			bl_tmp = mddr_lpddr2_bl_16;
			ret = -1;
		}
		if (ddr_freq >= 200) {
			pDDR_Reg->MCFG = (pDDR_Reg->MCFG & (~
			      ((0x3 << 20) | (0x3 << 18) | (0x1 << 17) |
			       (0x1 << 16)))) | bl_tmp | tfaw_cfg(5) |
			    pd_exit_fast | pd_type(1);
		} else {
			pDDR_Reg->MCFG =(pDDR_Reg->MCFG &
			     (~ ((0x3 << 20) | (0x3 << 18) | (0x1 << 17) |
			       (0x1 << 16)))) | mddr_lpddr2_bl_8 | tfaw_cfg(6) |
			    pd_exit_fast | pd_type(1);
		}
		pDDR_Reg->DFITRDDATAEN = pDDR_Reg->TCL / 2 - 1;
		pDDR_Reg->DFITPHYWRLAT = pDDR_Reg->TCWL / 2 - 1;

	}else if (ddr_reg.mem_type == LPDDR3) {
		pDDR_Reg->DFITRDDATAEN = pDDR_Reg->TCL / 2 - 1;
		pDDR_Reg->DFITPHYWRLAT = pDDR_Reg->TCWL / 2 - 1;
	}
	return ret;
}

sramlocalfunc static uint32  ddr_update_mr(void)
{
	/*uint32 cs;
	cs = READ_CS_INFO();
	cs = (1 << cs) - 1;	*/
	if (ddr_reg.mem_type == DDR3) {
		if (ddr_freq > DDR3_DDR2_DLL_DISABLE_FREQ) {
			if ((ddr_reg.ddr_dll_status) == DDR3_DLL_DISABLE) {	/*off -> on*/
				ddr_send_command(3, MRS_cmd, bank_addr(0x1) | cmd_addr((ddr_reg.ddrMR[1])));	/*DLL enable*/
				ddr_send_command(3, MRS_cmd, bank_addr(0x0) | cmd_addr(((ddr_reg.ddrMR[0])) | DDR3_DLL_RESET));	/*DLL reset*/
				ddr_delayus(2);	/*at least 200 DDR cycle*/
				ddr_send_command(3, MRS_cmd, bank_addr(0x0) | cmd_addr((ddr_reg.ddrMR[0])));
				ddr_reg.ddr_dll_status = DDR3_DLL_ENABLE;
			} else{  	/*on -> on*/
				ddr_send_command(3, MRS_cmd, bank_addr(0x1) | cmd_addr((ddr_reg.ddrMR[1])));
				ddr_send_command(3, MRS_cmd,bank_addr(0x0) | cmd_addr((ddr_reg.ddrMR[0])));
			}
		} else {
			ddr_send_command(3, MRS_cmd, bank_addr(0x1) | cmd_addr(((ddr_reg.ddrMR[1])) | DDR3_DLL_DISABLE));	/*DLL disable*/
			ddr_send_command(3, MRS_cmd,bank_addr(0x0) | cmd_addr((ddr_reg.ddrMR[0])));
			ddr_reg.ddr_dll_status = DDR3_DLL_DISABLE;
		}
		ddr_send_command(3, MRS_cmd, bank_addr(0x2) | cmd_addr((ddr_reg.ddrMR[2])));
	} else if (ddr_reg.mem_type == LPDDR2) {
		ddr_send_command(3, MRS_cmd, lpddr2_ma(0x1) | lpddr2_op(ddr_reg.ddrMR[1]));
		ddr_send_command(3, MRS_cmd, lpddr2_ma(0x2) | lpddr2_op(ddr_reg.ddrMR[2]));
		ddr_send_command(3, MRS_cmd, lpddr2_ma(0x3) | lpddr2_op(ddr_reg.ddrMR[3]));
	} else if (ddr_reg.mem_type == LPDDR3) {
		ddr_send_command(3, MRS_cmd, lpddr2_ma(0x1) | lpddr2_op(ddr_reg.ddrMR[1]));
		ddr_send_command(3, MRS_cmd, lpddr2_ma(0x2) | lpddr2_op(ddr_reg.ddrMR[2]));
		ddr_send_command(3, MRS_cmd, lpddr2_ma(0x3) | lpddr2_op(ddr_reg.ddrMR[3]));
		ddr_send_command(3, MRS_cmd, lpddr2_ma(11) | lpddr2_op(ddr_reg.ddrMR11));
	}
	return 0;
}

sramlocalfunc static void ddr_update_odt(void)
{
	/*adjust DRV and ODT*/
	uint32 phy_odt;
	if ((ddr_reg.mem_type == DDR3)||(ddr_reg.mem_type == LPDDR3)) {
		if (ddr_freq <= PHY_ODT_DISABLE_FREQ) {
			phy_odt = PHY_RTT_DISABLE;
		} else {
			phy_odt = PHY_RTT_279ohm;
		}
	} else {
		phy_odt = PHY_RTT_DISABLE;
	}
	pPHY_Reg->PHY_REG11 = PHY_DRV_ODT_SET(PHY_RON_34ohm);   //cmd drv
    pPHY_Reg->PHY_REG16 = PHY_DRV_ODT_SET(PHY_RON_45ohm);   //clk drv
    
    pPHY_Reg->PHY_REG20 = PHY_DRV_ODT_SET(PHY_RON_34ohm);   //DQS0 drv
    pPHY_Reg->PHY_REG30 = PHY_DRV_ODT_SET(PHY_RON_34ohm);   //DQS1 drv
    pPHY_Reg->PHY_REG40 = PHY_DRV_ODT_SET(PHY_RON_34ohm);   //DQS2 drv
    pPHY_Reg->PHY_REG50 = PHY_DRV_ODT_SET(PHY_RON_34ohm);   //DQS3 drv

	pPHY_Reg->PHY_REG21 = PHY_DRV_ODT_SET(phy_odt);	/*DQS0 odt*/
	pPHY_Reg->PHY_REG31 = PHY_DRV_ODT_SET(phy_odt);	/*DQS1 odt*/
	pPHY_Reg->PHY_REG41 = PHY_DRV_ODT_SET(phy_odt);	/*DQS2 odt*/
	pPHY_Reg->PHY_REG51 = PHY_DRV_ODT_SET(phy_odt);	/*DQS3 odt*/

	dsb();
}

sramlocalfunc static void idle_port(void)
{
    uint32 idle_req, idle_stus;

    pPMU_Reg->PMU_PMU_BUS_IDLE_REQ = idle_req_cci400;
    while ((pPMU_Reg->PMU_PMU_BUS_IDLE_ST & (1<<15)) != 0); //bit31 and bit 15 all 0 is idle 
#ifdef SYNC_WITH_LCDC_FRAME_INTR
    /*diable vop DMA and stall vop mmu*/
    vop_status = 0;
    if(((pCRU_Reg->CRU_CLKGATE_CON[16] & (0x1<<6)) == 0)//clk
          && ((pPMU_Reg->PMU_PMU_PWRDN_ST & (1<<14)) == 0))//pd_vio
    {
        if((*(volatile uint32*)VOP_WIN0_CTRL0 & 0x1) | (*(volatile uint32*)VOP_WIN1_CTRL0 & 0x1)
            | (*(volatile uint32*)VOP_WIN2_CTRL0 & 0x1) | (*(volatile uint32*)VOP_WIN3_CTRL0 & 0x1))
        {
            //*(uint32 *)(VOP_BASE_ADDR + 0x8) |= (0x1<<21);//DMA stop enable
            /*get mmu status if 1, mmu is stall*/
           if( *(volatile uint32 *)(VOP_BASE_ADDR + 0x304) & 0x1)
           {
               vop_status = 1;
               mmu_status = ((*(volatile uint32*)(VOP_BASE_ADDR+0x304)) >> 2) & 0x1;
               *(volatile uint32 *)(VOP_BASE_ADDR + 0x308) = 0x2;//stall mmu
               while(((*(volatile uint32*)(VOP_BASE_ADDR+0x304)) & (0x1<<2)) == 0);
           }
           //pCRU_Reg->CRU_CLKSEL_CON[20] = ((pCRU_Reg->CRU_CLKSEL_CON[20] & 0xff)*2+1) | (0xff<<16);
       }
    }
    if(id_mipi_dis == Ddr_FALSE)
        pCRU_Reg->CRU_CLKSEL_CON[20] = ((pCRU_Reg->CRU_CLKSEL_CON[20] & 0xff)*2+1) | (0xff<<16);
    /*dclk_vop0 div * 2*/
    //pCRU_Reg->CRU_CLKSEL_CON[20] = ((pCRU_Reg->CRU_CLKSEL_CON[20] & 0xff)*2+1) | (0xff<<16);
#endif

	rk3368_ddr_memCpy(&(clk_gate[0]),&(pCRU_Reg->CRU_CLKGATE_CON[0]),CLK_GATE_NUM);
    rk3368_ddr_memSet(&(pCRU_Reg->CRU_CLKGATE_CON[0]),CLK_GATE_NUM,0xffff0000);
    idle_req = idle_req_msch_en | idle_req_cci400;
    idle_stus = idle_msch;// | idle_core;
    pPMU_Reg->PMU_PMU_BUS_IDLE_REQ = idle_req;
	dsb();
	while ((pPMU_Reg->PMU_PMU_BUS_IDLE_ST & idle_stus) != idle_stus);
	//rk3368_ddr_memCpy(&(pCRU_Reg->CRU_CLKGATE_CON[0]),&(clk_gate[0]),CLK_GATE_NUM);
}

sramlocalfunc static void deidle_port(void)
{
    uint32 idle_req, idle_stus;
//	rk3368_ddr_memCpy(&(clk_gate[0]),&(pCRU_Reg->CRU_CLKGATE_CON[0]),CLK_GATE_NUM);
//    rk3368_ddr_memSet(&(pCRU_Reg->CRU_CLKGATE_CON[0]),CLK_GATE_NUM,0xffff0000);

    idle_req = idle_req_msch_en;
    idle_stus = idle_msch;
    pPMU_Reg->PMU_PMU_BUS_IDLE_REQ &= ~idle_req;
	dsb();
	while ((pPMU_Reg->PMU_PMU_BUS_IDLE_ST & idle_stus) != 0);
	rk3368_ddr_memCpy_mask(&(pCRU_Reg->CRU_CLKGATE_CON[0]),&(clk_gate[0]),CLK_GATE_NUM);
#ifdef SYNC_WITH_LCDC_FRAME_INTR
    if(id_mipi_dis == Ddr_FALSE)
        pCRU_Reg->CRU_CLKSEL_CON[20] = ((((pCRU_Reg->CRU_CLKSEL_CON[20] & 0xff)+1)/2)-1) | (0xff<<16);

    if(vop_status)
    {
       // *(uint32 *)(VOP_BASE_ADDR + 0x8) &= (~(0x1<<21));
       /*mmu paging enable and mmu is unstall*/
       //pCRU_Reg->CRU_CLKSEL_CON[20] = ((((pCRU_Reg->CRU_CLKSEL_CON[20] & 0xff)+1)/2)-1) | (0xff<<16);
       if(mmu_status == 0)
       {
            *(volatile uint32 *)(VOP_BASE_ADDR + 0x308) = 0x3;//disable stall mmu
            while(((*(volatile uint32*)(VOP_BASE_ADDR+0x304)) & (0x1<<2)) != 0);
       }
       //*(uint32 *)(VOP_BASE_ADDR + 0x8) &= (~(0x1<<21)); //DMA stop disable
    }
#endif
	pPMU_Reg->PMU_PMU_BUS_IDLE_REQ &= ~idle_req_cci400;
	while((pPMU_Reg->PMU_PMU_BUS_IDLE_ST & (1<<15))!=(1<<15)); //cci400 deidle
}

sramlocalfunc static void ddr_selfrefresh_enter(uint32 nMHz)
{
	ddr_move_to_Config_state();
	ddr_move_to_Lowpower_state();
    pPMUGRF_Reg->PMUGRF_SOC_CON0 = ddrphy_bufferen_io_en(0);
	pPHY_Reg->PHY_REG0 = (pPHY_Reg->PHY_REG0 & (~(0x3 << 2)));	/*phy soft reset*/
	dsb();
	pCRU_Reg->CRU_CLKGATE_CON[6] = ((0x3 << 14) << 16) | (3 << 14);	/*disable DDR PHY clock*/
	ddr_delayus(1);
}

sramlocalfunc static void ddr_selfrefresh_exit(void)
{
	pCRU_Reg->CRU_CLKGATE_CON[6] = ((0x3 << 14) << 16) | (0 << 14);	/*enable DDR PHY clock*/
	dsb();
	ddr_delayus(1);
	//pPHY_Reg->PHY_REG0 = (pPHY_Reg->PHY_REG0 | (0x3 << 2));	/*phy soft de-reset*/
	pPHY_Reg->PHY_REG0 |= (1 << 2); /*soft de-reset analogue(dll)*/
	ddr_delayus(5);
	pPHY_Reg->PHY_REG0 |= (1 << 3);/*soft de-reset digital*/
    pPMUGRF_Reg->PMUGRF_SOC_CON0 = ddrphy_bufferen_io_en(1);
	dsb();
	ddr_move_to_Config_state();
	ddr_data_training();
	ddr_move_to_Access_state();
/*    ddr_dtt_check();*/
}


sramlocalfunc void ddr_change_freq_in(uint32 freq_slew)
{
	uint32 value_100n, value_1u;

	if (freq_slew == 1) {
		value_100n = ddr_reg.pctl_timing.togcnt100n;
		value_1u = ddr_reg.pctl_timing.togcnt1u;
		ddr_reg.pctl_timing.togcnt1u = pDDR_Reg->TOGCNT1U;
		ddr_reg.pctl_timing.togcnt100n = pDDR_Reg->TOGCNT100N;
		ddr_update_timing();
		ddr_update_mr();
		ddr_reg.pctl_timing.togcnt100n = value_100n;
		ddr_reg.pctl_timing.togcnt1u = value_1u;
	} else {
		pDDR_Reg->TOGCNT100N = ddr_reg.pctl_timing.togcnt100n;
		pDDR_Reg->TOGCNT1U = ddr_reg.pctl_timing.togcnt1u;
	}

	pDDR_Reg->TZQCSI = 0;

}

sramlocalfunc void ddr_change_freq_out(uint32 freq_slew)
{
	if (freq_slew == 1) {
		pDDR_Reg->TOGCNT100N = ddr_reg.pctl_timing.togcnt100n;
		pDDR_Reg->TOGCNT1U = ddr_reg.pctl_timing.togcnt1u;
		pDDR_Reg->TZQCSI = ddr_reg.pctl_timing.tzqcsi;
	} else {
	    GPIO0_D4_H;
		ddr_update_timing();
		GPIO0_D4_L;
		ddr_update_mr();
	}
	GPIO0_D4_H;
	ddr_data_training();
	GPIO0_D4_L;
}

__sramfunc static void ddr_SRE_2_SRX(uint32 freq)
{
	idle_port();
	ddr_move_to_Lowpower_state();
	ddr_freq = freq;

    pPMUGRF_Reg->PMUGRF_SOC_CON0 = ddrphy_bufferen_core_en(0);
	pPHY_Reg->PHY_REG0 = (pPHY_Reg->PHY_REG0 & (~(0x3 << 2)));	/*phy soft reset*/
	dsb();

    ddr_setSys_pll(freq, 1);

	ddr_set_dll_bypass(freq);	/*set phy dll mode;*/
	ddr_update_timing();

    while((pCRU_Reg->CRU_PLL_CON[DPLL][1] & (0x1<<31))==0); /*wait for pll locked*/
    pCRU_Reg->CRU_PLL_CON[DPLL][3] = PLLMODE(NORMAL_MODE);

	pPHY_Reg->PHY_REG0 |= (1 << 2); /*soft de-reset analogue(dll)*/
	ddr_delayus(5);	
	ddr_update_odt();
	pPHY_Reg->PHY_REG0 |= (1 << 3);/*soft de-reset digital*/
    pPMUGRF_Reg->PMUGRF_SOC_CON0 = ddrphy_bufferen_core_en(1);
	dsb();
	//GPIO0_D4_L;
	ddr_move_to_Config_state();
	ddr_update_mr();
	ddr_data_training();
	ddr_move_to_Access_state();	
	deidle_port();
}

static uint32 save_sp;

uint32 rk3368_ddr_change_freq(uint32 nMHz)
{
    uint32 ret;
    uint32 freq_slew;

	if((pDDR_Reg->TDPD != 1) && (nMHz > 600))
	{
		nMHz = 600;
	}

    ret = ddr_setSys_pll(nMHz, 0);
    if (ret == ddr_freq) {
        goto out;
    } 

    ddr_get_parameter(ret);

    /*wait for lcdc line flag intrrupt*/
#ifdef SYNC_WITH_LCDC_FRAME_INTR
    /*wait interrupt when win0 orr win1 enable*/
    if(((pCRU_Reg->CRU_CLKGATE_CON[16] & (0x1<<6)) == 0)//clk
          && ((pPMU_Reg->PMU_PMU_PWRDN_ST & (1<<14)) == 0))//pd_vio
    {
        if((*(uint32*)VOP_WIN0_CTRL0 & 0x1) | (*(uint32*)VOP_WIN1_CTRL0 & 0x1)
            | (*(uint32*)VOP_WIN2_CTRL0 & 0x1) | (*(uint32*)VOP_WIN3_CTRL0 & 0x1))
        {
            *(uint32 *)VOP_INTR_CLEAR = VOP_CLEAR_FLAG1;
            while(((*(volatile uint32*)VOP_INTR_STATUS) & VOP_FLAG1_STATUS)==0);
        }
    }
#endif
    GPIO0_D4_H;
    /** 1. Make sure there is no host access */
    /*disbale interrupt*/
    __asm volatile 
    (
        " CPSID   I        \n"
        " CPSID   F        \n"
    );
    GPIO0_D4_H;
    DDR_SAVE_SP(save_sp);
    ddr_SRE_2_SRX(ret);
    DDR_RESTORE_SP(save_sp);
    GPIO0_D4_L;
    /*enable intterrupt*/
    __asm volatile 
    (
        " CPSIE   I        \n"
        " CPSIE   F        \n"
    );

/*    clk_set_rate(clk_get(NULL, "ddr_pll"), 0);    */
out:
    return ret;
}

void rk3368_ddr_set_auto_self_refresh(uint8 en)
{
	/*set auto self-refresh idle    */
	ddr_reg.ddr_sr_idle = en ? SR_IDLE : 0;
}

__sramfunc void ddr_suspend(void)
{
	ddr_selfrefresh_enter(0);
    pCRU_Reg->CRU_PLL_CON[DPLL][3] = PLLMODE(SLOW_MODE);
	dsb();
	ddr_delayus(1);
    pCRU_Reg->CRU_PLL_CON[DPLL][3] = PLL_POWER_DOWN;
	dsb();
	ddr_delayus(1);
}

__sramfunc void ddr_resume(void)
{
	uint32 delay = 1000;

    pCRU_Reg->CRU_PLL_CON[DPLL][3] = PLL_POWER_UP;
	dsb();
	while (delay > 0) {
		ddr_delayus(1);
		if (pCRU_Reg->CRU_PLL_CON[DPLL][1] & (0x1 << 31))
			break;
		delay--;
	}

    pCRU_Reg->CRU_PLL_CON[DPLL][3] = PLLMODE(NORMAL_MODE);
	dsb();

	ddr_selfrefresh_exit();
}
/********************
*cs_cap = 1 total cap,
*cs_cap = 0 cs0 cap
********************/
static uint32 ddr_get_cap(uint32 cs_cap)
{
	uint32 cs, bank, row, col, row1, bw;

	bank = READ_BK_INFO();
	row = READ_CS0_ROW_INFO();
	col = READ_COL_INFO();
	cs = READ_CS_INFO();
	bw = READ_BW_INFO();
	if ((cs > 1) && (cs_cap == 1)) {
		row1 = READ_CS1_ROW_INFO();
		return ((1 << (row + col + bank + bw)) +
			(1 << (row1 + col + bank + bw)));
	} else {
		return (1 << (row + col + bank + bw));
	}
}

int rk3368_ddr_init(U32 dram_speed_bin, uint32 freq, uint32 lcdc_type)
{
	uint32 die = 1;
	uint32 nMHz; 

	//ddr_print("RK3368 version 1.00 20141111\n");
	p_ddr_reg = &ddr_reg;
	/*
	*unused tDPD and used this timing to indicate chip type
	*old version this value:D3:0  LP3 500 LP2 500
	*new version this value:3368:1  3328:0
	*/
	if(pDDR_Reg->TDPD == 1)
		ddr_reg.ddr_speed_bin = 21;
	else
		ddr_reg.ddr_speed_bin = dram_speed_bin;

	ddr_freq = 0;
	ddr_reg.ddr_sr_idle = 0;
    ddr_reg.ddr_dll_status = DDR3_DLL_DISABLE;

	ddr_reg.mem_type = ((pPMUGRF_Reg->PMUGRF_OS_REG[2] >>13)&0x7);
	loops_per_us = ddr_get_MCU_freq();

	die = 1 << (READ_BW_INFO() - READ_DIE_BW_INFO());
	ddr_reg.ddr_capability_per_die = ddr_get_cap(0) / die;
	if((lcdc_type == SCREEN_MIPI) || (lcdc_type == SCREEN_DUAL_MIPI))
	    id_mipi_dis = Ddr_TRUE;
	else
        id_mipi_dis = Ddr_FALSE;
    /*
    pPMUGRF_Reg->PMUGRF_GPIO0_IOMUX.GPIOD_IOMUX = (0x3<<(8+16))|(0x0<<8);//GPIO0_D4 IOMUX->GPIO
    *(uint32*)GPIO0_BASE_ADDR |= (0x1<<28);//D4 high
    *(uint32*)(GPIO0_BASE_ADDR +4)|= (0x1<<28);//D4 out
    ddr_delayus(1);
    */
    printf("MCU:ddr init DRAM Type:");
	switch (p_ddr_reg->mem_type) {
    	case DDR3:
    		printf("DDR3");
    		break;
    	case LPDDR2:
    		printf("LPDDR2");
    		break;
    	case LPDDR3:
    	    printf("LPDDR3");
    	    break;
    	default:
    		printf("error type=%d", (p_ddr_reg->mem_type));
	}
	printf("  Cap=%dMB", (ddr_get_cap(1) >> 20));
    if(freq == 0)
    {
         freq = ddr_get_dram_freq();
    }
    nMHz = rk3368_ddr_change_freq(freq);
    printf("  freq:%dMHz\n\r",nMHz);
	/*ddr_print("init success!!! freq=%luMHz\n",
		  nMHz);
    */
	pPHY_Reg->PHY_REG11 = PHY_DRV_ODT_SET(PHY_RON_34ohm);	/*cmd drv*/
	pPHY_Reg->PHY_REG16 = PHY_DRV_ODT_SET(PHY_RON_45ohm);	/*clk drv*/

	pPHY_Reg->PHY_REG20 = PHY_DRV_ODT_SET(PHY_RON_34ohm);	/*DQS0 drv*/
	pPHY_Reg->PHY_REG30 = PHY_DRV_ODT_SET(PHY_RON_34ohm);	/*DQS1 drv*/
	pPHY_Reg->PHY_REG40 = PHY_DRV_ODT_SET(PHY_RON_34ohm);	/*DQS2 drv*/
	pPHY_Reg->PHY_REG50 = PHY_DRV_ODT_SET(PHY_RON_34ohm);	/*DQS3 drv*/

	return 0;
}


