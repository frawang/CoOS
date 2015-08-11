#ifndef __DDR_RK3368_H
#define __DDR_RK3368_H

#define DDR3_800D   (0)     // 5-5-5
#define DDR3_800E   (1)     // 6-6-6
#define DDR3_1066E  (2)     // 6-6-6
#define DDR3_1066F  (3)     // 7-7-7
#define DDR3_1066G  (4)     // 8-8-8
#define DDR3_1333F  (5)     // 7-7-7
#define DDR3_1333G  (6)     // 8-8-8
#define DDR3_1333H  (7)     // 9-9-9
#define DDR3_1333J  (8)     // 10-10-10
#define DDR3_1600G  (9)     // 8-8-8
#define DDR3_1600H  (10)    // 9-9-9
#define DDR3_1600J  (11)    // 10-10-10
#define DDR3_1600K  (12)    // 11-11-11
#define DDR3_1866J  (13)    // 10-10-10
#define DDR3_1866K  (14)    // 11-11-11
#define DDR3_1866L  (15)    // 12-12-12
#define DDR3_1866M  (16)    // 13-13-13
#define DDR3_2133K  (17)    // 11-11-11
#define DDR3_2133L  (18)    // 12-12-12
#define DDR3_2133M  (19)    // 13-13-13
#define DDR3_2133N  (20)    // 14-14-14
#define DDR3_DEFAULT (21)

#define DDR3_DDR2_DLL_DISABLE_FREQ    (300)	/* 颗粒dll disable的频率*/
#define DDR3_DDR2_ODT_DISABLE_FREQ    (333)	/*颗粒odt disable的频率*/
#define SR_IDLE                       (0x1)	/*unit:32*DDR clk cycle, and 0 for disable auto self-refresh*/
#define PD_IDLE                       (0x40)	/*unit:DDR clk cycle, and 0 for disable auto power-down*/
#define PHY_ODT_DISABLE_FREQ          (333)	/*定义主控端odt disable的频率*/
#define PHY_DLL_DISABLE_FREQ          (400)	/*定义主控端dll bypass的频率*/

#define dsb()                   \
    do {                        \
        __asm volatile          \
           (                    \
               " DSB       \n"  \
           );                   \
    }while(0)

typedef U32     uint32;
typedef U8      uint8;
typedef U16     uint16;
/*CRU*/
/*
typedef enum PLL_ID_Tag
{
    APLLB=0,
    APLLL,
    DPLL,
    CPLL,
    GPLL,
    NPLL,
    PLL_MAX
}PLL_ID;

typedef enum PLL_MODE_Tag
{
    SLOW_MODE=0,
    NORMAL_MODE,
    DEEP_SLOW_MODE,
    MODE_MAX
}PLL_MODE;
*/
#define SET_NR(n)        ((0x3f<<(8+16))|((n-1)<<8))
#define SET_NO(n)        ((0x1f<<(0+16))|((n-1)<<0))
#define SET_NF(n)        ((n-1)&0x1fff)
#define SET_NB(n)        ((n-1)&0xfff)
#define PLLMODE(n)    ((0x3<<(8+16))|(n<<8))
#define PLL_POWER_DOWN  ((0x1<<(1+16))|(1<<1))
#define PLL_POWER_UP    ((0x1<<(1+16))|(0<<1))
#define CLK_GATE_NUM    (24) //gate[24] 在idle port时不需要使能

#define PLL_RESET       ((0x1<<(5+16))|(0x1<<5))
#define PLL_DE_RESET       ((0x1<<(5+16))|(0x0<<5))


typedef volatile struct tagCRU_STRUCT 
{
    uint32 CRU_PLL_CON[PLL_MAX][4]; 
    uint32 reserved0[(0x100-0x60)/4];
    uint32 CRU_CLKSEL_CON[56];
    uint32 reserved1[(0x200-0x1e0)/4];
    uint32 CRU_CLKGATE_CON[25];//0x200
    uint32 reserved2[(0x280-0x264)/4];
    uint32 CRU_GLB_SRST_FST_VALUE;
    uint32 CRU_GLB_SRST_SND_VALUE;    
    uint32 reserved3[(0x300-0x288)/4];
    uint32 CRU_SOFTRST_CON[15];//0x300
    uint32 reserved4[(0x380-0x33c)/4];
    uint32 CRU_MISC_CON;
    uint32 CRU_GLB_CNT_TH;//0x384
    uint32 CRU_GLB_RST_CON;
    uint32 CRU_GLB_RST_ST;//0x38c
    uint32 reserved5[(0x400-0x390)/4];
    uint32 CRU_SDMMC_CON[2];//0x400
    uint32 CRU_SDIO0_CON[2];
    uint32 CRU_SDIO1_CON[2];
    uint32 CRU_EMMC_CON[2];
} CRU_REG, *pCRU_REG;


/*GRF*/
typedef struct tagGPIO_IOMUX
{
    uint32 GPIOA_IOMUX;
    uint32 GPIOB_IOMUX;
    uint32 GPIOC_IOMUX;
    uint32 GPIOD_IOMUX;
}GPIO_IOMUX_T;

typedef struct tagGPIO_P
{
    uint32 GPIOA_P;
    uint32 GPIOB_P;
    uint32 GPIOC_P;
    uint32 GPIOD_P;
}GPIO_P_T;
typedef struct tagGPIO_E
{
    uint32 GPIOA_E;
    uint32 GPIOB_E;
    uint32 GPIOC_E;
    uint32 GPIOD_E;
}GPIO_E_T;

typedef struct tagGPIO_SR
{
    uint32 GPIOL_SR;
    uint32 GPIOH_SR;
}GPIO_SR_T;

/*GRF_DDRC_CON0*/
#define dfi_eff_stat_en      ((3<<(5+16))|(3<<5))
#define dfi_eff_stat_disb    ((3<<(5+16))|(0<<5))

/*grf updated*/
typedef volatile struct tagREG_FILE {
    GPIO_IOMUX_T GRF_GPIO_IOMUX[3]; // 0x00a8
    uint32 reserved0[(0x100-0x30)/4];
    GPIO_P_T GPIO_P[3];
    uint32 reserved1[(0x200-0x130)/4];
    GPIO_E_T GPIO_E[3];    
    uint32 reserved2[(0x300-0x230)/4];
    GPIO_SR_T GPIO_SR[3];
    uint32 reserved3[(0x380-0x318)/4];
    uint32 GRF_GPIO_SMT;
    uint32 reserved4[(0x400-0x384)/4];
    uint32 GRF_SOC_CON[18];
    uint32 reserved5[(0x480-0x448)/4];    
    uint32 GRF_SOC_STATUS[16];
    uint32 reserved6[(0x500-0x4c0)/4];    
    uint32 GRF_CPU_CON[4];
    uint32 reserved7[(0x520-0x510)/4];    
    uint32 GRF_CPU_STATUS[2];    
    uint32 reserved8[(0x540-0x528)/4]; 
    uint32 GRF_CCI_STATUS[16];
    uint32 reserved9[(0x600-0x580)/4]; 
    uint32 GRF_DDRC0_CON0;
    uint32 reserved10[(0x680-0x604)/4]; 
    uint32 GRF_SIG_DETECT_CON;
    uint32 reserved11[(0x690-0x684)/4]; 
    uint32 GRF_SIG_DETECT_STATUS;
    uint32 reserved12[(0x6a0-0x694)/4]; 
    uint32 GRF_SIG_DETECT_CLR;
    uint32 reserved13[(0x700-0x6a4)/4]; 
    uint32 GRF_UOC0_CON0;
    uint32 reserved14[(0x718-0x704)/4]; 
    uint32 GRF_UOC1_CON1;
    uint32 GRF_UOC1_CON2;    
    uint32 GRF_UOC1_CON3;
    uint32 GRF_UOC1_CON4;
    uint32 GRF_UOC1_CON5;
    uint32 reserved15[(0x738-0x72c)/4]; 
    uint32 GRF_UOC3_CON0;
    uint32 GRF_UOC3_CON1;
    uint32 GRF_UOC4_CON0;
    uint32 GRF_UOC4_CON1;
    uint32 reserved16[(0x780-0x748)/4]; 
    uint32 GRF_USBPHY_CON[16];
    uint32 reserved17[(0x800-0x7c0)/4]; 
    uint32 GRF_PVTM_CON[3];
    uint32 GRF_PVTM_STATUS[3];
    uint32 reserved18[(0x900-0x818)/4]; 
    uint32 GRF_IO_VSEL;
    uint32 GRF_SARADC_TESTBIT;
    uint32 reserved19[(0xf80-0x908)/4]; 
    uint32 GRF_FAST_BOOT_ADDR;
} REG_FILE, *pREG_FILE;


/*PMU GRF*/
typedef struct tagGPIO_IOMUX_PG
{
    uint32 GPIOA_IOMUX;
    uint32 GPIOB_IOMUX;
    uint32 GPIOC_IOMUX;
    uint32 GPIOD_IOMUX;
}PG_GPIO_IOMUX_T;

typedef struct tagGPIO_P_PG
{
    uint32 GPIOA_P;
    uint32 GPIOB_P;
    uint32 GPIOC_P;
    uint32 GPIOD_P;
}PG_GPIO_P_T;
typedef struct tagGPIO_E_PG
{
    uint32 GPIOA_E;
    uint32 GPIOB_E;
    uint32 GPIOC_E;
    uint32 GPIOD_E;
}PG_GPIO_E_T;

typedef struct tagGPIO_SR_PG
{
    uint32 GPIOL_SR;
    uint32 GPIOH_SR;
}PG_GPIO_SR_T;

/*PMUGRF_SOC_CON0*/
#define ddrphy_bufferen_io_en(n)    ((0x1<<(9+16))|(n<<9))
#define ddrphy_bufferen_core_en(n)    ((0x1<<(8+16))|(n<<8))

//REG FILE registers
typedef volatile struct tagPMUGRFREG_FILE
{
    volatile PG_GPIO_IOMUX_T PMUGRF_GPIO0_IOMUX;
    volatile PG_GPIO_P_T PMUGRF_GPIO0_P;
    volatile PG_GPIO_E_T PMUGRF_GPIO0_E;
    volatile PG_GPIO_SR_T PMUGRF_GPIO0_SR;
    uint32 reserved0[(0x100-0x38)/4];
    volatile uint32 PMUGRF_SOC_CON0;
    uint32 reserved1[(0x180-0x104)/4];
    volatile uint32 PMUGRF_PMUPVTM_CON[2];
    uint32 reserved2[(0x190-0x188)/4];
    volatile uint32 PMUGRF_PMUPVTM_STATUS[2];
    uint32 reserved3[(0x200-0x198)/4];
    volatile uint32 PMUGRF_OS_REG[4];
} PMUGRF_FILE, *pPMUREG_FILE;

/*PMU*/
/*PMU_PMU_PWRDN_ST*/
#define pd_gpu_1_pwr_stat   (1<<16)
#define pd_gpu_0_pwr_stat   (1<<15)
#define pd_vio_pwr_stat     (1<<14)
#define pd_video_pwr_stat   (1<<13)
#define pd_peri_pwr_stat    (1<<12)
#define pd_bus_pwr_stat     (1<<11)
#define pd_core_pwr_stat    (1<<10)
#define pd_scu_b_pwr_stat   (1<<9)
/*PMU_PMU_BUS_IDLE_REQ*/
#define idle_req_cci400     (1<<15)
#define idle_req_cci        (1<<14)
#define idle_req_msch_en    (1<<13)
#define idle_req_pmu        (1<<12)
#define idle_req_alive      (1<<11)
#define idle_req_cxcs       (1<<10)
#define idle_req_vio        (1<<8)
#define idle_req_video      (1<<7)
#define idle_req_peri       (1<<6)
#define idle_req_dma        (1<<5)
#define idle_req_bus        (1<<4)
#define idle_req_core       (1<<3)
#define idle_req_gpu        (1<<2)
#define idle_req_cluster_b  (1<<1)
#define idle_req_cluster_l  (1<<0)
/*PMU_PMU_BUS_IDLE_ST*/
#define  idle_pmu   (1<<29)
#define  idle_alive   (1<<28)
#define  idle_msch   (1<<27)
#define  idle_cci     (1<<26)
#define  idle_vio   (1<<24)
#define  idle_video  (1<<23)
#define  idle_peri   (1<<22)
#define  idle_dma   (1<<21)
#define  idle_bus   (1<<20)
#define  idle_core  (1<<19)
#define  idle_gpu   (1<<18)

typedef volatile  struct tagPMU_FILE
{
    volatile uint32 PMU_PMU_WAKEUP_CFG[3];
    volatile uint32 PMU_PMU_PWRDN_CON;
    volatile uint32 PMU_PMU_PWRDN_ST;
    volatile uint32 PMU_PMU_PWRMODE_CORE_CON;
    volatile uint32 PMU_PMU_PWRMODE_COMMON_CON;
    volatile uint32 PMU_PMU_SFT_CON;
    volatile uint32 PMU_PMU_INT_CON;
    volatile uint32 PMU_PMU_INT_ST;
    volatile uint32 PMU_PMU_GPIO_POS_INT_CON;
    volatile uint32 PMU_PMU_GPIO_NEG_INT_CON;
    volatile uint32 PMU_PMU_GPIO_POS_INT_ST;
    volatile uint32 PMU_PMU_GPIO_NEG_INT_ST;
    volatile uint32 PMU_PMU_CORE_PWR_ST;
    volatile uint32 PMU_PMU_BUS_IDLE_REQ;
    volatile uint32 PMU_PMU_BUS_IDLE_ST;
    volatile uint32 PMU_PMU_POWER_ST;
    volatile uint32 PMU_PMU_OSC_CNT;
    volatile uint32 PMU_PMU_PLLLOCK_CNT;
    volatile uint32 PMU_PMU_PLLRST_CNT;
    volatile uint32 PMU_PMU_STABLE_CNT;
    volatile uint32 PMU_PMU_DDRIO_PWRON_CNT;
    volatile uint32 PMU_PMU_WAKEUP_RST_CLR_CNT;
    volatile uint32 PMU_PMU_DDR_SREF_ST;
    volatile uint32 PMU_PMU_SYS_REG[4];
    volatile uint32 PMU_PMU_GPU_APM_CON;
    volatile uint32 PMU_PMU_GPU_APM_ST;
    volatile uint32 PMU_PMU_TIMEOUT_CNT;
}PMU_FILE,*pPMU_FILE;


/*uPCTL*/
/*SCTL*/
#define INIT_STATE                     (0)
#define CFG_STATE                      (1)
#define GO_STATE                       (2)
#define SLEEP_STATE                    (3)
#define WAKEUP_STATE                   (4)

/*STAT*/
#define Init_mem                       (0)
#define Config                         (1)
#define Config_req                     (2)
#define Access                         (3)
#define Access_req                     (4)
#define Low_power                      (5)
#define Low_power_entry_req            (6)
#define Low_power_exit_req             (7)

/*MCFG*/
#define mddr_lpddr2_clk_stop_idle(n)   ((n)<<24)
#define pd_idle(n)                     ((n)<<8)
#define mddr_en                        (2<<22)
#define lpddr2_en                      (3<<22)
#define ddr2_en                        (0<<5)
#define ddr3_en                        (1<<5)
#define lpddr2_s2                      (0<<6)
#define lpddr2_s4                      (1<<6)
#define mddr_lpddr2_bl_2               (0<<20)
#define mddr_lpddr2_bl_4               (1<<20)
#define mddr_lpddr2_bl_8               (2<<20)
#define mddr_lpddr2_bl_16              (3<<20)
#define ddr2_ddr3_bl_4                 (0)
#define ddr2_ddr3_bl_8                 (1)
#define tfaw_cfg(n)                    (((n)-4)<<18)
#define pd_exit_slow                   (0<<17)
#define pd_exit_fast                   (1<<17)
#define pd_type(n)                     ((n)<<16)
#define two_t_en(n)                    ((n)<<3)
#define bl8int_en(n)                   ((n)<<2)
#define cke_or_en(n)                   ((n)<<1)

/*POWCTL*/
#define power_up_start                 (1<<0)

/*POWSTAT*/
#define power_up_done                  (1<<0)

/*DFISTSTAT0*/
#define dfi_init_complete              (1<<0)

/*CMDTSTAT*/
#define cmd_tstat                      (1<<0)
/*CMDTSTATEN*/
#define cmd_tstat_en                   (1<<1)

/*MCMD*/
#define Deselect_cmd                   (0)
#define PREA_cmd                       (1)
#define REF_cmd                        (2)
#define MRS_cmd                        (3)
#define ZQCS_cmd                       (4)
#define ZQCL_cmd                       (5)
#define RSTL_cmd                       (6)
#define MRR_cmd                        (8)
#define DPDE_cmd                       (9)

#define lpddr2_op(n)                   ((n)<<12)
#define lpddr2_ma(n)                   ((n)<<4)

#define bank_addr(n)                   ((n)<<17)
#define cmd_addr(n)                    ((n)<<4)

#define start_cmd                      (1u<<31)

typedef union STAT_Tag {
	uint32 d32;
	struct {
		unsigned ctl_stat:3;
		unsigned reserved3:1;
		unsigned lp_trig:3;
		unsigned reserved7_31:25;
	} b;
} STAT_T;

typedef union SCFG_Tag {
	uint32 d32;
	struct {
		unsigned hw_low_power_en:1;
		unsigned reserved1_5:5;
		unsigned nfifo_nif1_dis:1;
		unsigned reserved7:1;
		unsigned bbflags_timing:4;
		unsigned reserved12_31:20;
	} b;
} SCFG_T;

/* DDR Controller register struct */
typedef volatile struct DDR_REG_Tag {
	/*Operational State, Control, and Status Registers*/
	SCFG_T SCFG;		/*State Configuration Register*/
	volatile uint32 SCTL;	/*State Control Register*/
	STAT_T STAT;		/*State Status Register*/
	volatile uint32 INTRSTAT;	/*Interrupt Status Register*/
	uint32 reserved0[(0x40 - 0x10) / 4];
	/*Initailization Control and Status Registers*/
	volatile uint32 MCMD;	/*Memory Command Register*/
	volatile uint32 POWCTL;	/*Power Up Control Registers*/
	volatile uint32 POWSTAT;	/*Power Up Status Register*/
	volatile uint32 CMDTSTAT;	/*Command Timing Status Register*/
	volatile uint32 CMDTSTATEN;	/*Command Timing Status Enable Register*/
	uint32 reserved1[(0x60 - 0x54) / 4];
	volatile uint32 MRRCFG0;	/*MRR Configuration 0 Register*/
	volatile uint32 MRRSTAT0;	/*MRR Status 0 Register*/
	volatile uint32 MRRSTAT1;	/*MRR Status 1 Register*/
	uint32 reserved2[(0x7c - 0x6c) / 4];
	/*Memory Control and Status Registers*/
	volatile uint32 MCFG1;	/*Memory Configuration 1 Register*/
	volatile uint32 MCFG;	/*Memory Configuration Register*/
	volatile uint32 PPCFG;	/*Partially Populated Memories Configuration Register*/
	volatile uint32 MSTAT;	/*Memory Status Register*/
	volatile uint32 LPDDR2ZQCFG;	/*LPDDR2 ZQ Configuration Register*/
	uint32 reserved3;
	/*DTU Control and Status Registers*/
	volatile uint32 DTUPDES;	/*DTU Status Register*/
	volatile uint32 DTUNA;	/*DTU Number of Random Addresses Created Register*/
	volatile uint32 DTUNE;	/*DTU Number of Errors Register*/
	volatile uint32 DTUPRD0;	/*DTU Parallel Read 0*/
	volatile uint32 DTUPRD1;	/*DTU Parallel Read 1*/
	volatile uint32 DTUPRD2;	/*DTU Parallel Read 2*/
	volatile uint32 DTUPRD3;	/*DTU Parallel Read 3*/
	volatile uint32 DTUAWDT;	/*DTU Address Width*/
	uint32 reserved4[(0xc0 - 0xb4) / 4];
	/*Memory Timing Registers*/
	volatile uint32 TOGCNT1U;	/*Toggle Counter 1U Register*/
	volatile uint32 TINIT;	/*t_init Timing Register*/
	volatile uint32 TRSTH;	/*Reset High Time Register*/
	volatile uint32 TOGCNT100N;	/*Toggle Counter 100N Register*/
	volatile uint32 TREFI;	/*t_refi Timing Register*/
	volatile uint32 TMRD;	/*t_mrd Timing Register*/
	volatile uint32 TRFC;	/*t_rfc Timing Register*/
	volatile uint32 TRP;	/*t_rp Timing Register*/
	volatile uint32 TRTW;	/*t_rtw Timing Register*/
	volatile uint32 TAL;	/*AL Latency Register*/
	volatile uint32 TCL;	/*CL Timing Register*/
	volatile uint32 TCWL;	/*CWL Register*/
	volatile uint32 TRAS;	/*t_ras Timing Register*/
	volatile uint32 TRC;	/*t_rc Timing Register*/
	volatile uint32 TRCD;	/*t_rcd Timing Register*/
	volatile uint32 TRRD;	/*t_rrd Timing Register*/
	volatile uint32 TRTP;	/*t_rtp Timing Register*/
	volatile uint32 TWR;	/*t_wr Timing Register*/
	volatile uint32 TWTR;	/*t_wtr Timing Register*/
	volatile uint32 TEXSR;	/*t_exsr Timing Register*/
	volatile uint32 TXP;	/*t_xp Timing Register*/
	volatile uint32 TXPDLL;	/*t_xpdll Timing Register*/
	volatile uint32 TZQCS;	/*t_zqcs Timing Register*/
	volatile uint32 TZQCSI;	/*t_zqcsi Timing Register*/
	volatile uint32 TDQS;	/*t_dqs Timing Register*/
	volatile uint32 TCKSRE;	/*t_cksre Timing Register*/
	volatile uint32 TCKSRX;	/*t_cksrx Timing Register*/
	volatile uint32 TCKE;	/*t_cke Timing Register*/
	volatile uint32 TMOD;	/*t_mod Timing Register*/
	volatile uint32 TRSTL;	/*Reset Low Timing Register*/
	volatile uint32 TZQCL;	/*t_zqcl Timing Register*/
	volatile uint32 TMRR;	/*t_mrr Timing Register*/
	volatile uint32 TCKESR;	/*t_ckesr Timing Register*/
	volatile uint32 TDPD;	/*t_dpd Timing Register*/
    volatile uint32 TREFI_MEM_DDR3 ;        //new register
    uint32 reserved5[(0x180-0x14c)/4];
	/*ECC Configuration, Control, and Status Registers*/
	volatile uint32 ECCCFG;	/*ECC Configuration Register*/
	volatile uint32 ECCTST;	/*ECC Test Register*/
	volatile uint32 ECCCLR;	/*ECC Clear Register*/
	volatile uint32 ECCLOG;	/*ECC Log Register*/
	uint32 reserved6[(0x200 - 0x190) / 4];
	/*DTU Control and Status Registers*/
	volatile uint32 DTUWACTL;	/*DTU Write Address Control Register*/
	volatile uint32 DTURACTL;	/*DTU Read Address Control Register*/
	volatile uint32 DTUCFG;	/*DTU Configuration Control Register*/
	volatile uint32 DTUECTL;	/*DTU Execute Control Register*/
	volatile uint32 DTUWD0;	/*DTU Write Data 0*/
	volatile uint32 DTUWD1;	/*DTU Write Data 1*/
	volatile uint32 DTUWD2;	/*DTU Write Data 2*/
	volatile uint32 DTUWD3;	/*DTU Write Data 3*/
	volatile uint32 DTUWDM;	/*DTU Write Data Mask*/
	volatile uint32 DTURD0;	/*DTU Read Data 0*/
	volatile uint32 DTURD1;	/*DTU Read Data 1*/
	volatile uint32 DTURD2;	/*DTU Read Data 2*/
	volatile uint32 DTURD3;	/*DTU Read Data 3*/
	volatile uint32 DTULFSRWD;	/*DTU LFSR Seed for Write Data Generation*/
	volatile uint32 DTULFSRRD;	/*DTU LFSR Seed for Read Data Generation*/
	volatile uint32 DTUEAF;	/*DTU Error Address FIFO*/
	/*DFI Control Registers*/
	volatile uint32 DFITCTRLDELAY;	/*DFI tctrl_delay Register*/
	volatile uint32 DFIODTCFG;	/*DFI ODT Configuration Register*/
	volatile uint32 DFIODTCFG1;	/*DFI ODT Configuration 1 Register*/
	volatile uint32 DFIODTRANKMAP;	/*DFI ODT Rank Mapping Register*/
	/*DFI Write Data Registers*/
	volatile uint32 DFITPHYWRDATA;	/*DFI tphy_wrdata Register*/
	volatile uint32 DFITPHYWRLAT;	/*DFI tphy_wrlat Register*/
	uint32 reserved7[(0x260 - 0x258) / 4];
	volatile uint32 DFITRDDATAEN;	/*DFI trddata_en Register*/
	volatile uint32 DFITPHYRDLAT;	/*DFI tphy_rddata Register*/
	uint32 reserved8[(0x270 - 0x268) / 4];
	/*DFI Update Registers*/
	volatile uint32 DFITPHYUPDTYPE0;	/*DFI tphyupd_type0 Register*/
	volatile uint32 DFITPHYUPDTYPE1;	/*DFI tphyupd_type1 Register*/
	volatile uint32 DFITPHYUPDTYPE2;	/*DFI tphyupd_type2 Register*/
	volatile uint32 DFITPHYUPDTYPE3;	/*DFI tphyupd_type3 Register*/
	volatile uint32 DFITCTRLUPDMIN;	/*DFI tctrlupd_min Register*/
	volatile uint32 DFITCTRLUPDMAX;	/*DFI tctrlupd_max Register*/
	volatile uint32 DFITCTRLUPDDLY;	/*DFI tctrlupd_dly Register*/
	uint32 reserved9;
	volatile uint32 DFIUPDCFG;	/*DFI Update Configuration Register*/
	volatile uint32 DFITREFMSKI;	/*DFI Masked Refresh Interval Register*/
	volatile uint32 DFITCTRLUPDI;	/*DFI tctrlupd_interval Register*/
	uint32 reserved10[(0x2ac - 0x29c) / 4];
	volatile uint32 DFITRCFG0;	/*DFI Training Configuration 0 Register*/
	volatile uint32 DFITRSTAT0;	/*DFI Training Status 0 Register*/
	volatile uint32 DFITRWRLVLEN;	/*DFI Training dfi_wrlvl_en Register*/
	volatile uint32 DFITRRDLVLEN;	/*DFI Training dfi_rdlvl_en Register*/
	volatile uint32 DFITRRDLVLGATEEN;	/*DFI Training dfi_rdlvl_gate_en Register*/
	/*DFI Status Registers*/
	volatile uint32 DFISTSTAT0;	/*DFI Status Status 0 Register*/
	volatile uint32 DFISTCFG0;	/*DFI Status Configuration 0 Register*/
	volatile uint32 DFISTCFG1;	/*DFI Status configuration 1 Register*/
	uint32 reserved11;
	volatile uint32 DFITDRAMCLKEN;	/*DFI tdram_clk_enalbe Register*/
	volatile uint32 DFITDRAMCLKDIS;	/*DFI tdram_clk_disalbe Register*/
	volatile uint32 DFISTCFG2;	/*DFI Status configuration 2 Register*/
	volatile uint32 DFISTPARCLR;	/*DFI Status Parity Clear Register*/
	volatile uint32 DFISTPARLOG;	/*DFI Status Parity Log Register*/
	uint32 reserved12[(0x2f0 - 0x2e4) / 4];
	/*DFI Low Power Registers*/
	volatile uint32 DFILPCFG0;	/*DFI Low Power Configuration 0 Register*/
	uint32 reserved13[(0x300 - 0x2f4) / 4];
	/*DFI Training 2 Registers*/
	volatile uint32 DFITRWRLVLRESP0;	/*DFI Training dif_wrlvl_resp Status 0 Register*/
	volatile uint32 DFITRWRLVLRESP1;	/*DFI Training dif_wrlvl_resp Status 1 Register*/
	volatile uint32 DFITRWRLVLRESP2;	/*DFI Training dif_wrlvl_resp Status 2 Register*/
	volatile uint32 DFITRRDLVLRESP0;	/*DFI Training dif_rdlvl_resp Status 0 Register*/
	volatile uint32 DFITRRDLVLRESP1;	/*DFI Training dif_rdlvl_resp Status 1 Register*/
	volatile uint32 DFITRRDLVLRESP2;	/*DFI Training dif_rdlvl_resp Status 2 Register*/
	volatile uint32 DFITRWRLVLDELAY0;	/*DFI Training dif_wrlvl_delay Configuration 0 Register*/
	volatile uint32 DFITRWRLVLDELAY1;	/*DFI Training dif_wrlvl_delay Configuration 1 Register*/
	volatile uint32 DFITRWRLVLDELAY2;	/*DFI Training dif_wrlvl_delay Configuration 2 Register*/
	volatile uint32 DFITRRDLVLDELAY0;	/*DFI Training dif_rdlvl_delay Configuration 0 Register*/
	volatile uint32 DFITRRDLVLDELAY1;	/*DFI Training dif_rdlvl_delay Configuration 1 Register*/
	volatile uint32 DFITRRDLVLDELAY2;	/*DFI Training dif_rdlvl_delay Configuration 2 Register*/
	volatile uint32 DFITRRDLVLGATEDELAY0;	/*DFI Training dif_rdlvl_gate_delay Configuration 0 Register*/
	volatile uint32 DFITRRDLVLGATEDELAY1;	/*DFI Training dif_rdlvl_gate_delay Configuration 1 Register*/
	volatile uint32 DFITRRDLVLGATEDELAY2;	/*DFI Training dif_rdlvl_gate_delay Configuration 2 Register*/
	volatile uint32 DFITRCMD;	/*DFI Training Command Register*/
	uint32 reserved14[(0x3f8 - 0x340) / 4];
	/*IP Status Registers*/
	volatile uint32 IPVR;	/*IP Version Register*/
	volatile uint32 IPTR;	/*IP Type Register*/
} DDR_REG_T, *pDDR_REG_T;

//PHY_REG0
#define Soft_dereset_1       (1<<3)
#define Soft_dereset_0       (1<<2)
#define Soft_reset_1         (0<<3)
#define Soft_reset_0         (0<<2)
//PHY_REG1
#define PHY_DDR2             (1)
#define PHY_DDR3             (0)
#define PHY_LPDDR2_3           (2)
//PHY_REG2
#define PHY_DTT_EN   (1<<0)
#define PHY_DTT_DISB   (0<<0)

#define PHY_WRITE_LEVELING_EN   (1<<2)
#define PHY_WRITE_LEVELING_DISB   (0<<2)

#define PHY_SELECT_CS0              (2)
#define PHY_SELECT_CS1              (1)
#define PHY_SELECT_CS0_1            (0)

#define PHY_WRITE_LEVELING_SELECTCS(n)  (n<<6)
#define PHY_DATA_TRAINING_SELECTCS(n)   (n<<4)

#define PHY_BL_4             (0<<2)
#define PHY_BL_8             (1<<2)

#define PHY_CL(n)            (((n)&0xF)<<4)
#define PHY_AL(n)            ((n)&0xF)

//PHY_REG22,25,26,27,28
#define PHY_RON_DISABLE     (0)
#define PHY_RON_272ohm      (1)
#define PHY_RON_135ohm      (2)
#define PHY_RON_91ohm      (3)
//#define PHY_RON_68ohm       (4)
//#define PHY_RON_54ohm       (5)
//#define PHY_RON_45ohm       (6)
#define PHY_RON_38ohm       (7)
#define PHY_RON_68ohm       (8)
#define PHY_RON_54ohm       (9)
#define PHY_RON_45ohm       (10)
#define PHY_RON_39ohm       (11)
#define PHY_RON_34ohm       (12)
#define PHY_RON_30ohm       (13)
#define PHY_RON_27ohm       (14)
#define PHY_RON_25ohm       (15)

#define PHY_RTT_DISABLE     (0)
#define PHY_RTT_1116ohm      (1)
#define PHY_RTT_558ohm      (2)
#define PHY_RTT_372ohm      (3)
#define PHY_RTT_279ohm      (4)
#define PHY_RTT_223ohm      (5)
#define PHY_RTT_186ohm      (6)
#define PHY_RTT_159ohm      (7)
#define PHY_RTT_139ohm      (8)
#define PHY_RTT_124ohm      (9)
#define PHY_RTT_112ohm      (10)
#define PHY_RTT_101ohm      (11)
#define PHY_RTT_93ohm      (12)
#define PHY_RTT_86ohm       (13)
#define PHY_RTT_80ohm       (14)
#define PHY_RTT_74ohm       (15)


#define PHY_DRV_ODT_SET(n) ((n<<4)|n)

#define PHY_PLL_PD      (1<<1)
#define SELECT_PHY_PLL  (1<<0)

/* DDR PHY register struct  updated */
typedef volatile struct DDRPHY_REG_Tag {
    volatile uint32 PHY_REG0;               //PHY soft reset Register
    volatile uint32 PHY_REG1;               //phy working mode, burst length
    volatile uint32 PHY_REG2;               //PHY DQS squelch calibration Register
    volatile uint32 PHY_REG3;               //channel A read odt delay
    volatile uint32 PHY_REG4;               //channel B read odt dleay
    volatile uint32 PHY_REG5;               //write leveling load mode
    volatile uint32 PHY_REG6;               //write leveling 
    uint32 reserved0[(0x2c-0x1c)/4];    
    volatile uint32 PHY_REGb;               //cl,al
    volatile uint32 PHY_REGc;              //CWL set register
    uint32 reserved1[(0x44-0x34)/4];
    volatile uint32 PHY_REG11;              //cmd drv
    volatile uint32 PHY_REG12;              //cmd weak pull up
    volatile uint32 PHY_REG13;              //cmd dll delay
    volatile uint32 PHY_REG14;              //CK dll delay
    uint32 reserved2;                       //0x54
    volatile uint32 PHY_REG16;              //CK drv
    uint32 reserved3[(0x80-0x5c)/4];
    volatile uint32 PHY_REG20;              //left channel a drv
    volatile uint32 PHY_REG21;              //left channel a odt
    uint32 reserved4[(0x98-0x88)/4];
    volatile uint32 PHY_REG26;              //left channel a dq write dll
    volatile uint32 PHY_REG27;              //left channel a dqs write dll
    volatile uint32 PHY_REG28;               //left channel a dqs read dll
    uint32 reserved5[(0xc0-0xa4)/4];
    volatile uint32 PHY_REG30;              //right channel a drv
    volatile uint32 PHY_REG31;              //right channel a odt
    uint32 reserved6[(0xd8-0xc8)/4];
    volatile uint32 PHY_REG36;              //right channel a dq write dll
    volatile uint32 PHY_REG37;              //right channel a dqs write dll
    volatile uint32 PHY_REG38;               //right channel a dqs read dll
    uint32 reserved7[(0x100-0xe4)/4];
    volatile uint32 PHY_REG40;              //left channel b drv
    volatile uint32 PHY_REG41;              //left channel b odt
    uint32 reserved8[(0x118-0x108)/4];
    volatile uint32 PHY_REG46;              //left channel b dq write dll
    volatile uint32 PHY_REG47;              //left channel b dqs write dll
    volatile uint32 PHY_REG48;               //left channel b dqs read dll
    uint32 reserved9[(0x140-0x124)/4];
    volatile uint32 PHY_REG50;              //right channel b drv
    volatile uint32 PHY_REG51;              //right channel b odt
    uint32 reserved10[(0x158-0x148)/4];
    volatile uint32 PHY_REG56;              //right channel b dq write dll
    volatile uint32 PHY_REG57;              //right channel b dqs write dll
    volatile uint32 PHY_REG58;               //right channel b dqs read dll
    uint32 reserved11[(0x1c0-0x164)/4];
    volatile uint32 PHY_REG_skew_cs0data[(0x270-0x1c0)/4];//0x1c0-0x26c
    uint32 reserved12[(0x290-0x270)/4];    
    volatile uint32 PHY_REGDLL;             //dll bypass switch reg
    uint32 reserved13[(0x2c0-0x294)/4];
    volatile uint32 PHY_REG_skew[(0x3b0-0x2c0)/4];//de-skew
    volatile uint32 PHY_REGec;              //0x3b0 pll feedback divide
    volatile uint32 PHY_REGed;              //pll config
    volatile uint32 PHY_REGee; 
    volatile uint32 PHY_REGef; 
    volatile uint32 PHY_REGf0; 
    volatile uint32 PHY_REGf1; 
    volatile uint32 PHY_REGf2;              //0x3c8 
    uint32 reserved14[(0x3e8-0x3cc)/4];
    volatile uint32 PHY_REGfa;              //idqs
    volatile uint32 PHY_REGfb;              // left channel a calibration result
    volatile uint32 PHY_REGfc;              // right channel a calibration result
    volatile uint32 PHY_REGfd;              // left channel b calibration result
    volatile uint32 PHY_REGfe;              // right channel b calibration result
    volatile uint32 PHY_REGff;              //calibrationg done
} DDRPHY_REG_T, *pDDRPHY_REG_T;

#define pCRU_Reg               ((pCRU_REG)CRU_BASE_ADDR)
#define pGRF_Reg               ((pREG_FILE)GRF_BASE_ADDR)
#define pPMUGRF_Reg             ((pPMUREG_FILE)PMU_GRF_BASE_ADDR)
#define pPMU_Reg                ((pPMU_FILE)PMU_BASE_ADDR)
#define pDDR_Reg               ((pDDR_REG_T)DDR_PCTL_BASE_ADDR)
#define pPHY_Reg               ((pDDRPHY_REG_T)(DDR_PHY_BASE_ADDR))
#define SysSrv_DdrTiming       (SERVICE_BUS_ADDR+0xc)
//#define PMU_PWEDN_ST		(RK_PMU_VIRT + 0x8)

#define READ_CS_INFO()   ((((pPMUGRF_Reg->PMUGRF_OS_REG[2])>>11)&0x1)+1)
#define READ_COL_INFO()  (9+(((pPMUGRF_Reg->PMUGRF_OS_REG[2])>>9)&0x3))
#define READ_BK_INFO()   (3-(((pPMUGRF_Reg->PMUGRF_OS_REG[2])>>8)&0x1))
#define READ_CS0_ROW_INFO()  (13+(((pPMUGRF_Reg->PMUGRF_OS_REG[2])>>6)&0x3))
#define READ_CS1_ROW_INFO()  (13+(((pPMUGRF_Reg->PMUGRF_OS_REG[2])>>4)&0x3))
#define READ_BW_INFO()   (2>>(((pPMUGRF_Reg->PMUGRF_OS_REG[2])&0xc)>>2))    //代码中 0->8bit 1->16bit 2->32bit  与grf中定义相反
#define READ_DIE_BW_INFO()   (2>>((pPMUGRF_Reg->PMUGRF_OS_REG[2])&0x3))

/***********************************
 * DDR3 define
 ***********************************/
/*mr0 for ddr3*/
#define DDR3_BL8          (0)
#define DDR3_BC4_8        (1)
#define DDR3_BC4          (2)
#define DDR3_CL(n)        (((((n)-4)&0x7)<<4)|((((n)-4)&0x8)>>1))
#define DDR3_WR(n)        (((n)&0x7)<<9)
#define DDR3_DLL_RESET    (1<<8)
#define DDR3_DLL_DeRESET  (0<<8)

/*mr1 for ddr3*/
#define DDR3_DLL_ENABLE    (0)
#define DDR3_DLL_DISABLE   (1)
#define DDR3_MR1_AL(n)  (((n)&0x7)<<3)

#define DDR3_DS_40            (0)
#define DDR3_DS_34            (1<<1)
#define DDR3_Rtt_Nom_DIS      (0)
#define DDR3_Rtt_Nom_60       (1<<2)
#define DDR3_Rtt_Nom_120      (1<<6)
#define DDR3_Rtt_Nom_40       ((1<<2)|(1<<6))

/*mr2 for ddr3*/
#define DDR3_MR2_CWL(n) ((((n)-5)&0x7)<<3)
#define DDR3_Rtt_WR_DIS       (0)
#define DDR3_Rtt_WR_60        (1<<9)
#define DDR3_Rtt_WR_120       (2<<9)

/***********************************
 * LPDDR2 define
 ***********************************/
/*MR0 (Device Information)*/
#define  LPDDR2_DAI    (0x1)	/* 0:DAI complete, 1:DAI still in progress*/
#define  LPDDR2_DI     (0x1<<1)	/* 0:S2 or S4 SDRAM, 1:NVM*/
#define  LPDDR2_DNVI   (0x1<<2)	/* 0:DNV not supported, 1:DNV supported*/
#define  LPDDR2_RZQI   (0x3<<3)	/*00:RZQ self test not supported, 01:ZQ-pin may connect to VDDCA or float*/
				    /*10:ZQ-pin may short to GND.     11:ZQ-pin self test completed, no error condition detected.*/

/*MR1 (Device Feature)*/
#define LPDDR2_BL4     (0x2)
#define LPDDR2_BL8     (0x3)
#define LPDDR2_BL16    (0x4)
#define LPDDR2_nWR(n)  (((n)-2)<<5)

/*MR2 (Device Feature 2)*/
#define LPDDR2_RL3_WL1  (0x1)
#define LPDDR2_RL4_WL2  (0x2)
#define LPDDR2_RL5_WL2  (0x3)
#define LPDDR2_RL6_WL3  (0x4)
#define LPDDR2_RL7_WL4  (0x5)
#define LPDDR2_RL8_WL4  (0x6)

/*MR3 (IO Configuration 1)*/
#define LPDDR2_DS_34    (0x1)
#define LPDDR2_DS_40    (0x2)
#define LPDDR2_DS_48    (0x3)
#define LPDDR2_DS_60    (0x4)
#define LPDDR2_DS_80    (0x6)
#define LPDDR2_DS_120   (0x7)	/*optional*/

/*MR4 (Device Temperature)*/
#define LPDDR2_tREF_MASK (0x7)
#define LPDDR2_4_tREF    (0x1)
#define LPDDR2_2_tREF    (0x2)
#define LPDDR2_1_tREF    (0x3)
#define LPDDR2_025_tREF  (0x5)
#define LPDDR2_025_tREF_DERATE    (0x6)

#define LPDDR2_TUF       (0x1<<7)

/*MR8 (Basic configuration 4)*/
#define LPDDR2_S4        (0x0)
#define LPDDR2_S2        (0x1)
#define LPDDR2_N         (0x2)
#define LPDDR2_Density(mr8)  (8<<(((mr8)>>2)&0xf))	/*Unit:MB*/
#define LPDDR2_IO_Width(mr8) (32>>(((mr8)>>6)&0x3))

/*MR10 (Calibration)*/
#define LPDDR2_ZQINIT   (0xFF)
#define LPDDR2_ZQCL     (0xAB)
#define LPDDR2_ZQCS     (0x56)
#define LPDDR2_ZQRESET  (0xC3)

/*MR16 (PASR Bank Mask)*/
/*S2 SDRAM Only*/
#define LPDDR2_PASR_Full (0x0)
#define LPDDR2_PASR_1_2  (0x1)
#define LPDDR2_PASR_1_4  (0x2)
#define LPDDR2_PASR_1_8  (0x3)

/*********************************
**LPDDR3
**********************************/
//MR0 (Device Information)
#define  LPDDR3_DAI    (0x1)        // 0:DAI complete, 1:DAI still in progress
#define  LPDDR3_RZQI   (0x3<<3)     // 00:RZQ self test not supported, 01:ZQ-pin may connect to VDDCA or float
                                        // 10:ZQ-pin may short to GND.     11:ZQ-pin self test completed, no error condition detected.
#define  LPDDR3_WL_SUPOT (1<<6)     // 0:DRAM does not support WL(Set B), 1:DRAM support WL(Set B)
#define  LPDDR3_RL3_SUPOT (1<<7)    // 0:DRAM does not support RL=3,nWR=3,WL=1; 1:DRAM supports RL=3,nWR=3,WL=1 for frequencies <=166
    
    //MR1 (Device Feature)
#define LPDDR3_BL8     (0x3)
#define LPDDR3_nWR(n)  ((n)<<5)
    
    //MR2 (Device Feature 2)
    //WL Set A,default
#define LPDDR3_RL3_WL1   (0x1)       // <=166MHz,optional
#define LPDDR3_RL6_WL3   (0x4)       // <=400MHz
#define LPDDR3_RL8_WL4   (0x6)       // <=533MHz
#define LPDDR3_RL9_WL5   (0x7)       // <=600MHz
#define LPDDR3_RL10_WL6  (0x8)       // <=667MHz,default
#define LPDDR3_RL11_WL6  (0x9)       // <=733MHz
#define LPDDR3_RL12_WL6  (0xa)       // <=800MHz
#define LPDDR3_RL14_WL8  (0xc)       // <=933MHz
#define LPDDR3_RL16_WL8  (0xe)       // <=1066MHz
    //WL Set B, optional
    //#define LPDDR3_RL3_WL1   (0x1)       // <=166MHz,optional
    //#define LPDDR3_RL6_WL3   (0x4)       // <=400MHz
    //#define LPDDR3_RL8_WL4   (0x6)       // <=533MHz
    //#define LPDDR3_RL9_WL5   (0x7)       // <=600MHz
#define LPDDR3_RL10_WL8  (0x8)       // <=667MHz,default
#define LPDDR3_RL11_WL9  (0x9)       // <=733MHz
#define LPDDR3_RL12_WL9  (0xa)       // <=800MHz
#define LPDDR3_RL14_WL11 (0xc)       // <=933MHz
#define LPDDR3_RL16_WL13 (0xe)       // <=1066MHz
#define LPDDR3_nWRE      (1<<4)      // 1:enable nWR programming > 9(defualt)
#define LPDDR3_WL_S      (1<<6)      // 1:Select WL Set B
#define LPDDR3_WR_LEVEL  (1<<7)      // 1:enable
    
    //MR3 (IO Configuration 1)
#define LPDDR3_DS_34    (0x1)
#define LPDDR3_DS_40    (0x2)
#define LPDDR3_DS_48    (0x3)
#define LPDDR3_DS_60    (0x4)        //reserved
#define LPDDR3_DS_80    (0x6)        //reserved
#define LPDDR3_DS_34D_40U   (0x9)  
#define LPDDR3_DS_40D_48U   (0xa)
#define LPDDR3_DS_34D_48U   (0xb)
    
    //MR4 (Device Temperature)
#define LPDDR3_tREF_MASK (0x7)
#define LPDDR3_LT_EXED   (0x0)       // SDRAM Low temperature operating limit exceeded
#define LPDDR3_4_tREF    (0x1)
#define LPDDR3_2_tREF    (0x2)
#define LPDDR3_1_tREF    (0x3)
#define LPDDR3_05_tREF   (0x4)
#define LPDDR3_025_tREF  (0x5)
#define LPDDR3_025_tREF_DERATE    (0x6)
#define LPDDR3_HT_EXED   (0x7)       // SDRAM High temperature operating limit exceeded
    
#define LPDDR3_TUF       (0x1<<7)    // 1:value has changed since last read of MR4
    
    //MR8 (Basic configuration 4)
#define LPDDR3_S8        (0x3)
#define LPDDR3_Density(mr8)  (8<<((mr8>>2)&0xf))   // Unit:MB
#define LPDDR3_IO_Width(mr8) (32>>((mr8>>6)&0x3))
    
    //MR10 (Calibration)
#define LPDDR3_ZQINIT   (0xFF)
#define LPDDR3_ZQCL     (0xAB)
#define LPDDR3_ZQCS     (0x56)
#define LPDDR3_ZQRESET  (0xC3)
    //MR11 (ODT Control)
#define LPDDR3_ODT_60   (1)           //optional for 1333 and 1600
#define LPDDR3_ODT_120  (2)
#define LPDDR3_ODT_240  (3)
#define LPDDR3_ODT_DIS  (0)


typedef enum DRAM_TYPE_Tag {
	LPDDR = 0,
	DDR,
	DDR2,
	DDR3,
	LPDDR2S2,
	LPDDR2,
    LPDDR3,
	DRAM_MAX
} DRAM_TYPE;

typedef struct PCTRL_TIMING_Tag {
	uint32 ddrFreq;
	/*Memory Timing Registers*/
	uint32 togcnt1u;	/*Toggle Counter 1U Register*/
	uint32 tinit;		/*t_init Timing Register*/
	uint32 trsth;		/*Reset High Time Register*/
	uint32 togcnt100n;	/*Toggle Counter 100N Register*/
	uint32 trefi;		/*t_refi Timing Register*/
	uint32 tmrd;		/*t_mrd Timing Register*/
	uint32 trfc;		/*t_rfc Timing Register*/
	uint32 trp;		    /*t_rp Timing Register*/
	uint32 trtw;		/*t_rtw Timing Register*/
	uint32 tal;		    /*AL Latency Register*/
	uint32 tcl;		    /*CL Timing Register*/
	uint32 tcwl;		/*CWL Register*/
	uint32 tras;		/*t_ras Timing Register*/
	uint32 trc;		    /*t_rc Timing Register*/
	uint32 trcd;		/*t_rcd Timing Register*/
	uint32 trrd;		/*t_rrd Timing Register*/
	uint32 trtp;		/*t_rtp Timing Register*/
	uint32 twr;		    /*t_wr Timing Register*/
	uint32 twtr;		/*t_wtr Timing Register*/
	uint32 texsr;		/*t_exsr Timing Register*/
	uint32 txp;		    /*t_xp Timing Register*/
	uint32 txpdll;		/*t_xpdll Timing Register*/
	uint32 tzqcs;		/*t_zqcs Timing Register*/
	uint32 tzqcsi;		/*t_zqcsi Timing Register*/
	uint32 tdqs;		/*t_dqs Timing Register*/
	uint32 tcksre;		/*t_cksre Timing Register*/
	uint32 tcksrx;		/*t_cksrx Timing Register*/
	uint32 tcke;		/*t_cke Timing Register*/
	uint32 tmod;		/*t_mod Timing Register*/
	uint32 trstl;		/*Reset Low Timing Register*/
	uint32 tzqcl;		/*t_zqcl Timing Register*/
	uint32 tmrr;		/*t_mrr Timing Register*/
	uint32 tckesr;		/*t_ckesr Timing Register*/
	uint32 tdpd;		/*t_dpd Timing Register*/	
    uint32 trefi_mem_ddr3;          //
} PCTL_TIMING_T;

typedef union NOC_TIMING_Tag {
	uint32 d32;
	struct {
		unsigned ActToAct:6;
		unsigned RdToMiss:6;
		unsigned WrToMiss:6;
		unsigned BurstLen:3;
		unsigned RdToWr:5;
		unsigned WrToRd:5;
		unsigned BwRatio:1;
	} b;
} NOC_TIMING_T;

typedef struct BACKUP_REG_Tag {
	PCTL_TIMING_T pctl_timing;
	NOC_TIMING_T noc_timing;
	uint32 ddrMR[4];
	uint32 ddrMR11;
	uint32 mem_type;
	uint32 ddr_speed_bin;
	uint32 ddr_capability_per_die;
	uint32 ddr_dll_status;
	uint32 ddr_sr_idle;
} BACKUP_REG_T;

#ifndef Ddr_FALSE
#define Ddr_FALSE         (0)
#endif

#ifndef Ddr_TRUE
#define Ddr_TRUE          (1)
#endif

#define SCREEN_NULL        0
#define SCREEN_RGB         1
#define SCREEN_LVDS        2
#define SCREEN_DUAL_LVDS   3
#define SCREEN_MCU         4
#define SCREEN_TVOUT       5
#define SCREEN_HDMI        6
#define SCREEN_MIPI        7
#define SCREEN_DUAL_MIPI   8
#define SCREEN_EDP         9
#define SCREEN_TVOUT_TEST  10
#define SCREEN_LVDS_10BIT        11
#define SCREEN_DUAL_LVDS_10BIT   12


#define SYNC_WITH_LCDC_FRAME_INTR
#define DCLK_VOP_DIV		(128 -1)
#define VOP_CLEAR_FLAG0     ((0x1<<(16+3)) | (0x1<<3))
#define VOP_CLEAR_FLAG1     ((0x1<<(16+4)) | (0x1<<4))
#define VOP_FLAG0_STATUS    (1<<19)
#define VOP_FLAG1_STATUS    (1<<20)
#define VOP_LINE_FLAG           (VOP_BASE_ADDR + 0x20)
#define VOP_INTR_CLEAR          (VOP_BASE_ADDR + 0x28)
#define VOP_INTR_STATUS         (VOP_BASE_ADDR + 0x2c)
#define VOP_WIN0_CTRL0          (VOP_BASE_ADDR + 0x30)
#define VOP_WIN1_CTRL0          (VOP_BASE_ADDR + 0x70)
#define VOP_WIN2_CTRL0          (VOP_BASE_ADDR + 0xb0)
#define VOP_WIN3_CTRL0          (VOP_BASE_ADDR + 0x100)

#define LPJ_100MHZ  999456UL
#define GPIO0_D4_H   //do{*(uint32*)GPIO0_BASE_ADDR |= (0x1<<28);}while(0)
#define GPIO0_D4_L   //do{*(uint32*)GPIO0_BASE_ADDR &= ~(0x1<<28);}while(0)

#ifdef DDR_DEBUG
#define _DDR_DEBUG  1
#else
#define _DDR_DEBUG  0
#endif

/*
 * Output a debug text when condition "cond" is met. The "cond" should be
 * computed by a preprocessor in the best case, allowing for the best
 * optimization.
 */
#define ddr_debug_cond(cond, fmt, args...)          \
        do {                                    \
                if (cond)                       \
                        printf(fmt, ##args);    \
        } while (0)

#define ddr_debug(fmt, args...)                     \
        ddr_debug_cond(_DDR_DEBUG, fmt, ##args)


extern void rk3368_ddr_memSet(uint32 *dstAddr, uint32 count, uint32 value);
extern void rk3368_ddr_memCpy(uint32 *dstAddr, uint32 *srcAddr, uint32 count);
extern void rk3368_ddr_memCpy_mask(uint32 *dstAddr, uint32 *srcAddr, uint32 count);

uint32 rk3368_ddr_change_freq(uint32 nMHz, uint32 lcdc_type);
void rk3368_ddr_set_auto_self_refresh(uint8 en);
//int rk3368_ddr_init(U32 dram_speed_bin, uint32 freq);
int rk3368_ddr_init(U32 dram_speed_bin, uint32 freq, uint32 lcdc_type);

uint32 ddr_get_dram_freq(void);

#endif
