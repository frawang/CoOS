/**
 *******************************************************************************
 * @file       io-rk3399.h
 * @version    V1.0
 * @date       2016.03.30
 * @brief      Io header file
 * @details    This file including some defines and declares related to base
 * 	           address.
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
 * COPYRIGHT 2016 Fuzhou Rockchip Electronics Co., Ltd
 *******************************************************************************
 */

#ifndef __IO_RK3399_H
#define __IO_RK3399_H

#define MCU_BASE			0x40000000

#define SDMMC_BASE			(MCU_BASE + 0x06320000)
#define SDMMC_CDETECT			0x0050

#define PMU_GRF_BASE			(MCU_BASE + 0x07320000)

#define SGRF_BASE			(MCU_BASE + 0x07330000)
#define SECURE_GRF_PERILP_CON(n)	(0x08100 + (n) * 4)
#define SECURE_GRF_PMU_CON(n)		(0x0c100 + (n) * 4)

#define MBOX_SIZE			SZ_64K

#define UART_DBG_BASE			(MCU_BASE + 0x071a0000)
#define UART_DBG_SIZE			SZ_64K

#define CRU_BASE			(MCU_BASE + 0x07760000)
#define CRU_SIZE			SZ_64K

#define GRF_BASE			(MCU_BASE + 0x07770000)
#define GRF_SIZE			SZ_64K

#define SRAM_BASE			(MCU_BASE + 0x078c0000)
#define SRAM_SIZE			(SZ_128K + SZ_64K)

#ifdef PERIL_M0
#define MBOX_BASE			(MCU_BASE + 0x076b0000)

#define MCUJTAG_IOMUX_ADDR		(GRF_BASE + 0x0e024)
#define MCUJTAG_IOMUX			(0xf << (8 + 16) | 0xa << 8)

#define M0_INT_ARB_MASKN(n)		(MCU_BASE + 0x07798000 + (n) * 4)
#define M0_INT_ARB_FLAGN(n)		(MCU_BASE + 0x07798080 + (n) * 4)

#elif  PMU_M0
#define MBOX_BASE			(MCU_BASE + 0x07390000)

#define MCUJTAG_IOMUX_ADDR		(PMU_GRF_BASE + 0x00014)
#define MCUJTAG_IOMUX			(0xf << (2 + 16) | 0x5 << 2)

#define M0_INT_ARB_MASKN(n)		(MCU_BASE + 0x0779c000 + (n) * 4)
#define M0_INT_ARB_FLAGN(n)		(MCU_BASE + 0x0779c080 + (n) * 4)
#endif

#endif
