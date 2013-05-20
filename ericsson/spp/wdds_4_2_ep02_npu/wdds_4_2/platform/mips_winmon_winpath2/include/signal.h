/*************************************************************
 * (C) Copyright 2004-2006, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ************************************************************/
/* $Id: signal.h,v 1.4 2006/01/07 01:23:04 rogers Exp $ */
/*****************************************************************************
 *
 * Module: $RCSfile: signal.h,v $
 * Purpose: interrupt handling
 *
 * Module Originator: Arnon Mordoh
 * Creation Date: Nov 17th, 2002
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
 *****************************************************************************/

#ifndef _SIGNAL_H_
#define _SIGNAL_H_

#ifndef WINMON_CONTEXT
#include <winmon.h>
#endif

/*******************************************************************
 * EXCEPTION VECTORS
 *******************************************************************/
#ifdef __mips__
#define   SIG_INT        0  /* Interrupt  */
#define   SIG_MOD        1  /* TLB modification */
#define   SIG_TLBL       2  /* TLB (load or instruction fetch) */
#define   SIG_TLBS       3  /* TLB (store) */
#define   SIG_ADEL       4  /* Address error (load or instruction fetch) */
#define   SIG_ADES       5  /* Address error (store) */
#define   SIG_IBE        6  /* Bus error (instruction fetch) */
#define   SIG_DBE        7  /* Bus error (data reference: load or store) */
#define   SIG_SYS        8  /* Syscall */
#define   SIG_BP         9  /* Breakpoint */
#define   SIG_RI         10 /* Reserved instruction */
#define   SIG_CPU        11 /* Coprocessor Unusable */
#define   SIG_OV         12 /* Arithmetic Overflow */
#define   SIG_TR         13 /* Trap */
#define   SIG_14         14 /* Reserved */
#define   SIG_FPE        15 /* Floating point */
#define   SIG_16         16 /* Reserved */
#define   SIG_17         17 /* Reserved */
#define   SIG_18         18 /* Reserved */
#define   SIG_19         19 /* Reserved */
#define   SIG_20         20 /* Reserved */
#define   SIG_21         21 /* Reserved */
#define   SIG_22         22 /* Reserved */
#define   SIG_WATCH      23 /* Reference to WatchHi/WatchLo address */
#define   SIG_MCHECK     24 /* Machine check */
#define   SIG_25         25 /* Reserved */
#define   SIG_26         26 /* Reserved */
#define   SIG_27         27 /* Reserved */
#define   SIG_28         28 /* Reserved */
#define   SIG_29         29 /* Reserved */
#define   SIG_30         30 /* Reserved */
#define   SIG_31         31 /* Reserved */

#define   SIG_LAST       32
#endif

#ifdef __PPC__
#define   SIG_0         0  /* Reserved */
#define   SIG_RESET     1  /* Reset weak */
#define   SIG_CHECK     2  /* Machine check */
#define   SIG_DSI       3  /* Data memory access */
#define   SIG_ISI       4  /* Instruction memory access */
#define   SIG_INT       5  /* External interrupt */
#define   SIG_ALIGN     6  /* Alignment */
#define   SIG_PROG      7  /* Program exception */
#define   SIG_FLOAT     8  /* Floating point unavailable */
#define   SIG_DECR      9  /* Decrement */
#define   SIG_10        10 /* Reserved */
#define   SIG_11        11 /* Reserved */
#define   SIG_SYS       12 /* System call */
#define   SIG_TRACE     13 /* Trace */
#define   SIG_FASSIST   14 /* Floating point assist */
#define   SIG_15        15 /* Reserved */

#define   SIG_LAST      16
#endif

#define SIG_ERR         ((int_handler_t)0)
#define SIG_DFL         ((int_handler_t)4)
#define SIG_IGN         ((int_handler_t)8)

/*******************************************************************
 * INTERRUPT VECTORS
 *******************************************************************/
#define IV_WINPATH_BASE 0x0     /* Vector Base for WinPath devices */

#define IV_NULL_VEC     -1
#define IV_SYI_BASE     IV_WINPATH_BASE
#define IV_EINT1_VEC    (IV_SYI_BASE +  0)
#define IV_EINT2_VEC    (IV_SYI_BASE +  1)
#define IV_EINT3_VEC    (IV_SYI_BASE +  2)
#define IV_EINT4_VEC    (IV_SYI_BASE +  3)
#define IV_UART_VEC     (IV_SYI_BASE + 16)
#define IV_BRG9_VEC     (IV_SYI_BASE + 17)
#define IV_BRG10_VEC    (IV_SYI_BASE + 18)
#define IV_OCAMF_VEC    (IV_SYI_BASE + 19)
#define IV_BERR1_VEC    (IV_SYI_BASE + 24)        /* SIU1 (Parameter) bus error */
#define IV_BERR2_VEC    (IV_SYI_BASE + 25)        /* SIU2 (Packet) bus error */
#define IV_BERR3_VEC    (IV_SYI_BASE + 26)        /* SIU3 (Global) bus error */
#define IV_BERR4_VEC    (IV_SYI_BASE + 27)        /* SIU4 (Host) bus error */
#define IV_PERR1_VEC    (IV_SYI_BASE + 28)        /* SIU1 parity error */
#define IV_PERR2_VEC    (IV_SYI_BASE + 29)        /* SIU2 parity error */
#define IV_PERR4_VEC    (IV_SYI_BASE + 31)        /* SIU4 parity error */

#define IV_SEI_BASE     (IV_WINPATH_BASE + 32)
#define IV_SEI_00       (IV_SEI_BASE +   0)
#define IV_SEI_01       (IV_SEI_BASE +   1)
#define IV_SEI_02       (IV_SEI_BASE +   2)
#define IV_SEI_03       (IV_SEI_BASE +   3)
#define IV_SEI_04       (IV_SEI_BASE +   4)
#define IV_SEI_05       (IV_SEI_BASE +   5)
#define IV_SEI_06       (IV_SEI_BASE +   6)
#define IV_SEI_07       (IV_SEI_BASE +   7)
#define IV_SEI_08       (IV_SEI_BASE +   8)
#define IV_SEI_09       (IV_SEI_BASE +   9)
#define IV_SEI_10       (IV_SEI_BASE +  10)
#define IV_SEI_11       (IV_SEI_BASE +  11)
#define IV_SEI_12       (IV_SEI_BASE +  12)
#define IV_SEI_13       (IV_SEI_BASE +  13)
#define IV_SEI_14       (IV_SEI_BASE +  14)
#define IV_SEI_15       (IV_SEI_BASE +  15)
#define IV_SEI_16       (IV_SEI_BASE +  16)
#define IV_SEI_17       (IV_SEI_BASE +  17)
#define IV_SEI_18       (IV_SEI_BASE +  18)
#define IV_SEI_19       (IV_SEI_BASE +  19)
#define IV_SEI_20       (IV_SEI_BASE +  20)
#define IV_SEI_21       (IV_SEI_BASE +  21)
#define IV_SEI_22       (IV_SEI_BASE +  22)
#define IV_SEI_23       (IV_SEI_BASE +  23)
#define IV_SEI_24       (IV_SEI_BASE +  24)
#define IV_SEI_25       (IV_SEI_BASE +  25)
#define IV_SEI_26       (IV_SEI_BASE +  26)
#define IV_SEI_27       (IV_SEI_BASE +  27)
#define IV_SEI_28       (IV_SEI_BASE +  28)
#define IV_SEI_29       (IV_SEI_BASE +  29)
#define IV_SEI_30       (IV_SEI_BASE +  30)
#define IV_SEI_31       (IV_SEI_BASE +  31)
#define WINPATH_IV_MAX  IV_SEI_31

/* Routing definitions -- masks used to steer between SEI1 and SEI2, or between SYI1 and SYI2. */
#define SEI0_MASK       0xF0F0F0F0     /*  SEI0 handles QueueOverflows */
#define SEI1_MASK       ~SEI0_MASK     /*  SEI1 handles "normal" Queue events */
#define SYI0_MASK       0xBF08001F     /*  SYI0 handles the catastrophic system events */
#define SYI1_MASK       0x00070000     /*  SYI1 handles the expected system even */

typedef void (* int_handler_t)(WP_U32);

#define signal(sig,handler)  WinMon_signal(sig,handler)

#define int_connect(e,s)     WinMon_int_connect(e,s)
#define enable_interrupts()  WinMon_enable_interrupts()
#define disable_interrupts() WinMon_disable_interrupts()
#define enable_int(d)        WinMon_enable_int(d)
#define disable_int(d)       WinMon_disable_int(d)
#define int_ack(d)           WinMon_int_ack(d)
#define set_timer_int(h,t)   WinMon_set_timer_int(h,t)

#endif
