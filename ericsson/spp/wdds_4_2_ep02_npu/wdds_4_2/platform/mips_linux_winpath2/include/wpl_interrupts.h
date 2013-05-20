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
#define WPL_INT_SEIR_OVERRUN_ALL_QS  0x70707070 /* Overflow event bits */
#define WPL_INT_SEIR_SERVICE_ALL_QS  0x07070707 /* Service event bits */
#else
#define WPL_INT_SEIR_OVERRUN_ALL_QS  0xf0f0f0f0 /* Overflow event bits */
#define WPL_INT_SEIR_SERVICE_ALL_QS  0x0f0f0f0f /* Service event bits */
#endif

#define WPL_INT_SEIR_OVERRUN     (WPL_INT_SEIR_OVERRUN_ALL_QS & ~WPL_BspOverrunMask())
#define WPL_INT_SEIR_SERVICE     (WPL_INT_SEIR_SERVICE_ALL_QS & ~WPL_BspServiceMask())

#define WPL_INT_SEIR_SERVICE_HOST_0  0x01010101 /* Service event for
                                                   highest priority
                                                   host queue */
#define WPL_INT_SEIR_OVERRUN_HOST_0  0x10101010

#define WPL_INT_SYIR_BUSMONITORS     0x0f000000

WP_U32 WPLI_IntConnect(void);

#endif
