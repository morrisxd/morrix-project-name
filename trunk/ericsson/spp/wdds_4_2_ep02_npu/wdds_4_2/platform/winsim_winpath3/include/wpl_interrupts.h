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

#define WPL_INT_IC_IR_OVERRUN_ALL_QS  0x0003C000 /* Overflow event bits */
#define WPL_INT_IC_IR_SERVICE_ALL_QS  0x00F00000 /* Service event bits */

#define WPL_INT_IC_IR_OVERRUN     (WPL_INT_IC_IR_OVERRUN_ALL_QS & ~WPL_BspOverrunMask())
#define WPL_INT_IC_IR_SERVICE     (WPL_INT_IC_IR_SERVICE_ALL_QS & ~WPL_BspServiceMask())

#define WPL_INT_IC_IR_BUSERROR        0x0000001f
#define WPL_INT_IC_IR_SGMIIAN         0x00002000

#endif
