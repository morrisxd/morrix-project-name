/*****************************************************************************
 * (C) Copyright 2001-2006, Wintegra.  All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra, Inc.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/
/*****************************************************************************
 *
 * Module: $RCSfile: sysdefs.h,v $
 * Purpose:
 *
 *
 * Module Originator: Arnon Mordoh
 * Creation Date: Aug 19th, 2001
 *
 * Change History:
 * +------------+--------------------+----------------------------------------
 * | Date       | By                 | Description
 * +------------+--------------------+----------------------------------------
 * |            |                    |
 * +------------+--------------------+----------------------------------------
 * |            |                    |
 * +------------+--------------------+----------------------------------------
 *
 ****************************************************************************/

#ifndef _SYSDEFS_H_
#define _SYSDEFS_H_

#ifdef _ASSEMBLER_

#define BSS      .bss
#define TEXT .text
#define DATA .data

#define LEAF(name)\
                .text;\
                .globl  name;\
                .ent    name;\
name:


#define SLEAF(name)\
                .text;\
                .ent    name;\
name:


#define END(name)\
                .size name,.-name;\
                .end    name


#define STORE(val,addr)\
                li      k1,val;\
                li      k0,KSEG1(addr);\
                sw      k1,0(k0)

#define WAIT(val,mask,addr)\
                li      k0,KSEG1(addr);\
                li      a1,val;\
                li      a0,mask;\
1:              lw      k1,0(k0);\
                and     k1,k1,a0;\
                bne     k1,a1,1b;\
                nop

#else
#include "wpv_types.h"

#define STORE(val,addr)  (*((WP_U32 volatile *)(addr))=val)

#define WAIT(val,mask,addr)\
                while (((*(WP_U32 volatile *)(addr))&mask)!=val);

#define MAXUINT(w)      (\
                ((w) == sizeof(WP_U8))  ? 0xFFU :\
                ((w) == sizeof(WP_U16)) ? 0xFFFFU :\
                ((w) == sizeof(WP_U32)) ? 0xFFFFFFFFU : 0)

#define MAXINT(w)       (\
                ((w) == sizeof(WP_S8))  ? 0x7F :\
                ((w) == sizeof(WP_S16)) ? 0x7FFF :\
                ((w) == sizeof(WP_S32)) ? 0x7FFFFFFF : 0)

#endif

/* Stack size */
#define SYS_STACK_SIZE                  0x2000
#define SYS_APPL_STACK_SIZE             0x5000

/*  RAM vector offsets (ie based at 0x80000000)
 *  for EJTAG and NMI exceptions.
 */
#define SYS_EJTAG_RAM_VECTOR_OFS        0x300
#define SYS_NMI_RAM_VECTOR_OFS          0x380

#define CTRL_A          ('A'-0x40)
#define CTRL_B          ('B'-0x40)
#define CTRL_C          ('C'-0x40)
#define CTRL_D          ('D'-0x40)
#define CTRL_E          ('E'-0x40)
#define CTRL_F          ('F'-0x40)
#define CTRL_H          ('H'-0x40)
#define CTRL_K          ('K'-0x40)
#define CTRL_N          ('N'-0x40)
#define CTRL_P          ('P'-0x40)
#define CTRL_U          ('U'-0x40)
#define DEL             0x7F
#define TAB             0x09
#define CR              0x0D
#define LF              0x0A
#define ESC             0x1B
#define SP              0x20
#define CSI             0x9B
#define EOF_SREC        -2
#define UART_ERROR      -1

#define FALSE          0
#define TRUE              (!FALSE)

#define MIN(x,y)          ((x) < (y) ? (x) : (y))
#define MAX(x,y)      ((x) > (y) ? (x) : (y))

#define INCWRAP(ptr,size)  (ptr)=((ptr)+1)%(size)
#define DECWRAP(ptr,size)  (ptr)=(((ptr)==0)?((size)-1):((ptr)-1))

#define  OK        0x00000000     /* operation completed successfully */
#define  NOT_OK    0xffffffff     /* operation completed not successfully */

#ifndef NULL
#define NULL       ((void *)0)
#endif

#endif
