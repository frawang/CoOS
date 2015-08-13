/*
 * Copyright (c) 2015, Fuzhou Rockchip Electronics Co., Ltd
 * 
 * Author: Frank Wang <frank.wang@rock-chips.com>
 * Date  : 05-May-2015
 * 
 */

#include <peri.h>
#include <task.h>
#include <coocox.h>
#include "types.h"
#include "io.h"
#include "cru.h"

#ifdef CFG_TSADC

/* TSADC V2 Sensor info define: */
#define TSADCV2_USER_CON            0x00
#define TSADCV2_INT_PD              0x0c
#define TSADCV2_DATA(chn)           (0x20 + (chn) * 0x04)

#define TSADCV3_DATA_MASK           0x3ff
#define TSADC_CLK_GATE_DELAY_TIME   50      /* usec */
#define TSADC_CLK_CYCLE_TIME        32      /* usec */

//#define debug_writel(v, c) {writel(0, c); writel(v, c);}

OS_STK task_tsadc_stk[TASK_STK_SIZE*2];      /*!< Stack of 'task_tsadc' task. */
static int g_adc_data = 122;                 /* default 0 DC */
static u32 g_clk_cycle = TSADC_CLK_CYCLE_TIME;

static int tsadc_get_temp_code(void)
{
    u32 adc_pd;
    u32 i;
    u32 cci_aclk_current;

    /* set A53 pll to 24M slow mode*/   
    writel(0x03000000, 0x4076000c);
    writel(0x03000000, 0x4076001c); 
    CoUdelay(TSADC_CLK_GATE_DELAY_TIME);

    /* set CCI400 */
    cci_aclk_current = readl(0x407600114) & 0x000000ff;
    writel(0x001f001f, 0x407600114);

    /* get temperature from tsadc */
    /* power up, channel 0*/
    writel(0x18, (TSADC_BASE_ADDR + TSADCV2_USER_CON));
    dsb();
    CoUdelay(g_clk_cycle * 2);

    writel(0x38, (TSADC_BASE_ADDR + TSADCV2_USER_CON));
    dsb();
    CoUdelay(g_clk_cycle * 13);

    for (i=0; i<50; i++) { /* try 50 times */
        CoUdelay(g_clk_cycle);
        adc_pd = readl(TSADC_BASE_ADDR + TSADCV2_INT_PD);
        if ((adc_pd & 0x100) == 0x100) {
            CoUdelay(1);
            /*read adc data*/
            g_adc_data = readl(TSADC_BASE_ADDR + TSADCV2_DATA(0));

            /*clear eoc inter*/
            writel(0x100, (TSADC_BASE_ADDR + TSADCV2_INT_PD));
            dsb();
            break;
        }
    }

    /*power down, channel 0*/
    writel(0x0, (TSADC_BASE_ADDR + TSADCV2_USER_CON));
    
    /* recover A53 PLL to normal mode */
    writel(0x03000100, 0x4076000c);
    writel(0x03000100, 0x4076001c);
    
    /* restore CCI400 */
    cci_aclk_current |= 0x00ff0000;
    writel(cci_aclk_current, 0x407600114);
    
    return g_adc_data;
}

void tsadc_cmd_handle(MboxMsg *pmsg)
{
    scpi_tsadc_cmd_t cmd = CMD_ID(pmsg->Cmd);

    switch (cmd) {
        case SCPI_THERMAL_GET_TSADC_DATA: {    
            volatile struct __packed_tx1 {
                u32 status;
                int tsadc_data;
            } *pbuf_tx1;

            pbuf_tx1 = (volatile struct __packed_tx1 *)pmsg->B2A_Buf;
//            printf("MCU [%s][%d]: voltage=%d, temp_adjust=%d\n\r", __FUNCTION__, __LINE__, pbuf_rx->voltage, pbuf_rx->temp_adjust);
            pbuf_tx1->status = SCPI_SUCCESS;
            pbuf_tx1->tsadc_data = tsadc_get_temp_code();    
            Mbox_CmdDone(pmsg);

            break;
        }

        case SCPI_THERMAL_SET_TSADC_CYCLE: {
            volatile struct __packed_rx2 {
                u32 clk_cycle;
            } *pBuf_rx2;

            volatile struct __packed_tx2 {
                U32 status;
            } *pBuf_tx2;

            pBuf_rx2 = (volatile struct __packed_rx2 *)pmsg->A2B_Buf;
            pBuf_tx2 = (volatile struct __packed_tx2 *)pmsg->B2A_Buf;

            g_clk_cycle = pBuf_rx2->clk_cycle;
//          printf("[MCU]clk_cycle=%d\n\r", g_clk_cycle);

            pBuf_tx2->status = SCPI_SUCCESS;
            Mbox_CmdDone(pmsg);
            break;
        }

        default:
            break;
    }

    return;
}

void task_tsadc(void *pdata)
{
    for (;;) {
        StatusType err;
        MboxMsg *pMsg = CoPendMail(mboxs[SCPI_CL_THERMAL], 0, &err);

        if (err == E_OK) {
            tsadc_cmd_handle(pMsg);
        } else {
            printf("[MCU]TSADC PendMail Failed\n\r");
        }
    }

    CoExitTask();
}

void create_tsadc_task(void)
{
    /* Create Tasks */
    CoCreateTask(task_tsadc, (void *)0, TASK_TSADC_PRI,
                &task_tsadc_stk[TASK_STK_SIZE-1], TASK_STK_SIZE);
}

#endif
