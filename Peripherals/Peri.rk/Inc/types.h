/*
* Copyright (c) 2015, Fuzhou Rockchip Electronics Co., Ltd
* 
* Author: Frank Wang <frank.wang@rock-chips.com>
* Date  : 05-May-2015
* 
*/

#ifndef _TYPES_H
#define _TYPES_H

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

typedef signed int   s32;
typedef signed short s16;
typedef signed char  s8;

typedef volatile signed long  vs32;
typedef volatile signed short vs16;
typedef volatile signed char  vs8;

typedef unsigned long   ulong;
typedef unsigned int    u32;
typedef unsigned short  u16;
typedef unsigned char   u8;

typedef unsigned int   const  uc32;  /* Read Only */
typedef unsigned short  const uc16;  /* Read Only */
typedef unsigned char   const uc8;   /* Read Only */

typedef volatile unsigned int   vu32;
typedef volatile unsigned short vu16;
typedef volatile unsigned char  vu8;

typedef volatile unsigned int  const  vuc32;  /* Read Only */
typedef volatile unsigned short const vuc16;  /* Read Only */
typedef volatile unsigned char  const vuc8;   /* Read Only */


#endif /* _TYPES_H */
