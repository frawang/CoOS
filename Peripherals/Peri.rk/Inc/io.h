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
 * COPYRIGHT 2014 Fuzhou Rockchip Electronics Co., Ltd
 *******************************************************************************
 */

#ifndef __IO_H
#define __IO_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "types.h"

#ifdef RK3368
#include "io-rk3368.h"
#elif RK3366
#include "io-rk3366.h"
#else

#endif

/*!< The macrs for register */
#define readb(c)	({u8 __v = (*(vu8 *) (c)); __v;})
#define readw(c)	({u16 __v = (*(vu16 *) (c)); __v;})
#define readl(c)	({u32 __v = (*(vu32 *) (c)); __v;})

#define writeb(v, c)	((*(vu8 *) (c)) = (v))
#define writew(v, c)	((*(vu16 *) (c)) = (v))
#define writel(v, c)	((*(vu32 *) (c)) = (v))

#ifdef __cplusplus
}
#endif

#endif
