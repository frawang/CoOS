/**
 *******************************************************************************
 * @file       io.h	
 * @version    V1.0    
 * @date       2014.12.31
 * @brief      Io header file
 * @details    This file including some defines and declares related to base 
 * 	       address.
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

#ifndef __IO_H
#define __IO_H

#ifdef __cplusplus
 extern "C" {
#endif

#define MCU_BASE	0x40000000

/*!< i2c 				*/
#define I2C0_BASE	(MCU_BASE + 0x650000) /*!< i2c_pmu			*/
#define I2C0_SIZE	SZ_64K

#define I2C1_BASE	(MCU_BASE + 0x660000) /*!< i2c_audio			*/
#define I2C1_SIZE	SZ_64K

#define I2C2_BASE	(MCU_BASE + 0x140000) /*!< i2c_sensor			*/
#define I2C2_SIZE	SZ_64K

#define I2C3_BASE	(MCU_BASE + 0x150000) /*!< i2c_cam			*/
#define I2C3_SIZE	SZ_64K

#define I2C4_BASE	(MCU_BASE + 0x160000) /*!< i2c_tp			*/
#define I2C4_SIZE	SZ_64K

#define I2C5_BASE	(MCU_BASE + 0x170000) /*!< i2c_hdmi			*/
#define I2C5_SIZE	SZ_64K

/*!< uart				*/
#define UART0_BASE	(MCU_BASE + 0x180000) /*!< uart_bt			*/
#define UART0_SIZE	SZ_64K

#define UART1_BASE	(MCU_BASE + 0x190000) /*!< uart_bb			*/
#define UART1_SIZE	SZ_64K

#define UART2_BASE	(MCU_BASE + 0x690000) /*!< uart_dbg			*/
#define UART2_SIZE	SZ_64K

#define UART3_BASE	(MCU_BASE + 0x1b0000) /*!< uart_gps			*/
#define UART3_SIZE	SZ_64K

#define UART4_BASE	(MCU_BASE + 0x1c0000) /*!< uart_ext			*/
#define UART4_SIZE	SZ_64K

/*!< mailbox				*/
#define MBOX_BASE	(MCU_BASE + 0x6b0000)
#define MBOX_SIZE	SZ_64K

/*!< gpio				*/
#define GPIO0_BASE	(MCU_BASE + 0x750000)
#define GPIO0_SIZE	SZ_64K

#define GPIO1_BASE	(MCU_BASE + 0x780000)
#define GPIO1_SIZE	SZ_64K

#define GPIO2_BASE	(MCU_BASE + 0x790000)
#define GPIO2_SIZE	SZ_64K

#define GPIO3_BASE	(MCU_BASE + 0x7A0000)
#define GPIO3_SIZE	SZ_64K

/*!< cru				*/
#define CRU_BASE	(MCU_BASE + 0x760000)
#define CRU_SIZE	SZ_64K

/*!< grf				*/
#define GRF_BASE	(MCU_BASE + 0x770000)
#define GRF_SIZE	SZ_64K

 /*!< sram				*/
 #define SRAM_BASE	(MCU_BASE + 0x8c0000)
 #define SRAM_SIZE	SZ_64K

#define     UART_BT_BASE_ADDR                 0x40180000
#define     UART_BB_BASE_ADDR                 0x40190000
#define     UART_GPS_BASE_ADDR                0x401B0000
#define     UART_EXT_BASE_ADDR                0x401C0000
#define     DMA_PREI_BASE_ADDR                0x40250000
#define     AXI_GPV_BASE_ADDR                 0x40300000
#define     NANDC_BASE_ADDR                   0x40410000
#define     USB_HOST_BASE_ADDR                0x40500000
#define     HSIC_BASE_ADDR                    0x405C0000
#define     DMAC_BUS_BASE_ADDR                0x40600000
#define     DDR_PCTL_BASE_ADDR                0x40610000
#define     DDR_PHY_BASE_ADDR                 0x40620000
#define     I2C_PMU_BASE_ADDR                 0x40650000
#define     UART_DBG_BASE_ADDR                0x40690000
#define     PMU_INTMEM_BASE_ADDR              0x40720000
#define     PMU_BASE_ADDR                     0x40730000
#define     PMU_GRF_BASE_ADDR                 0x40738000
#define     CRU_BASE_ADDR                     0x40760000
#define     GRF_BASE_ADDR                     0x40770000
#define     GPIO0_BASE_ADDR                   0x40750000
#define     GPIO1_BASE_ADDR                   0x40780000
#define     GPIO2_BASE_ADDR                   0x40790000
#define     GPIO3_BASE_ADDR                   0x407A0000
#define     SRAM_BASE_ADDR                    0x408C0000
#define     SERVICE_BUS_ADDR                  0x40AC0000
#define     VOP_BASE_ADDR                     0x40930000
#define     SDRAM_ADDR              0x00000000


#ifdef __cplusplus
}
#endif

#endif
