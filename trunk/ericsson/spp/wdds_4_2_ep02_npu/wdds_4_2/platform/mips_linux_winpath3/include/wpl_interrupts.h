/*************************************************************
 * (C) Copyright 2000-2009, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ************************************************************/

/*****************************************************************************
 *
 * Module: wpl_interrupts.h
 * Purpose:
 *
 * This module contains platform-specific data types.
 *****************************************************************************/

#ifndef WPL_INTERRUPTS_H
#define WPL_INTERRUPTS_H

#include "wpl_interrupts_interface.h"

#define WPL_CORE0 0x0A0 /* The core with L1 cache */
#define WPL_CORE1 0x0A2 /* The core with L1+L2 cache */

#if WP_RESERVE_IRQ3_FOR_WINK
#define WPL_INT_IC_IR_OVERRUN_ALL_QS  0x00038000 /* Overflow event bits */
#define WPL_INT_IC_IR_SERVICE_ALL_QS  0x00700000 /* Service event bits */
#else
#define WPL_INT_IC_IR_OVERRUN_ALL_QS  0x0003C000 /* Overflow event bits */
#define WPL_INT_IC_IR_SERVICE_ALL_QS  0x00F00000 /* Service event bits */
#endif

#define WPL_INT_IC_IR_OVERRUN     (WPL_INT_IC_IR_OVERRUN_ALL_QS & ~WPL_BspOverrunMask())
#define WPL_INT_IC_IR_SERVICE     (WPL_INT_IC_IR_SERVICE_ALL_QS & ~WPL_BspServiceMask())

#define WPL_INT_IC_IR_BUSERROR        0x0000001f
#define WPL_INT_IC_IR_SGMIIAN         0x00002000

WP_U32 WPLI_IntConnect(void);

#define WPLI_WPID_INDICATOR_INTERRUPT_MASK   0xC0000000
#define WPL_INT_IC_IR_SERDES                 0x00001000
#define WPLI_WPID_INDICATOR_INTERRUPT_OFFSET 30

#endif
