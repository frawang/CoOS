/**
 *******************************************************************************
 * @file       io-rk3366.h
 * @version    V1.0
 * @date       2016.03.16
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

#ifndef __IO_RK3366_H
#define __IO_RK3366_H

#define MCU_BASE				0x40000000

#define SDMMC_BASE				(MCU_BASE + 0x400000)
#define SDMMC_CDETECT			0x0050

#define MBOX_BASE				(MCU_BASE + 0x6b0000)
#define MBOX_SIZE				SZ_64K

#define UART_DBG_BASE			(MCU_BASE + 0x690000)
#define UART_DBG_SIZE			SZ_64K

#define PMU_BASE				(MCU_BASE + 0x730000)
#define PMU_GRF_BASE			(MCU_BASE + 0x738000)

#define GPIO0_BASE				(MCU_BASE + 0x750000)
#define GPIO0_SIZE				SZ_64K

#define CRU_BASE				(MCU_BASE + 0x760000)
#define CRU_SIZE				SZ_64K

#define GRF_BASE				(MCU_BASE + 0x770000)
#define GRF_SIZE				SZ_64K
#define MCUJTAG_IOMUX_ADDR		(GRF_BASE + 0x0040)
#define MCUJTAG_IOMUX			(0xff << (4 + 16) | 0xaa << 4)

#define GPIO1_BASE				(MCU_BASE + 0x758000)
#define GPIO1_SIZE				SZ_32K

#define GPIO2_BASE				(MCU_BASE + 0x790000)
#define GPIO2_SIZE				SZ_64K

#define GPIO3_BASE				(MCU_BASE + 0x7A0000)
#define GPIO3_SIZE				SZ_64K

#define GPIO4_BASE				(MCU_BASE + 0x7B0000)
#define GPIO4_SIZE				SZ_64K

#define GPIO5_BASE				(MCU_BASE + 0x7C0000)
#define GPIO5_SIZE				(SZ_128K + SZ_64K)

#define SRAM_BASE				(MCU_BASE + 0x8c0000)
#define SRAM_SIZE				SZ_64K

#endif
