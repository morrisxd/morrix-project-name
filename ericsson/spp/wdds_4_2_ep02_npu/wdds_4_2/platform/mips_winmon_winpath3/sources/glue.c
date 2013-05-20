/*****************************************************************************
 * (C) Copyright 2000-2006, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: glue.c
 *
 * Purpose: a collection of missing libc functions
 *
 * Functions contained in this file:
 *
 * +---------------------------+----------------------------------------------
 * |  Function name            | Description
 * +---------------------------+----------------------------------------------
 * |
 * |  Internal WDDI Functions
 * |  -------------------------+----------------------------------------------
 * |  usleep                   |
 * +---------------------------+----------------------------------------------
 *
 ****************************************************************************/

#include "winmon.h"

#ifndef NO_AUTO_MEMMAP

WP_S32 get_bus_address(WP_S32 c)
{
   return WinMon_get_bus_address(c);
}

WP_S32 get_bus_size(WP_S32 c)
{
   return WinMon_get_bus_size(c);
}

WP_S32 is_bus_access(WP_S32 c)
{
   return WinMon_is_bus_access(c);
}

#endif

void internal_clock(WP_U32 *i, WP_U32 *f)
{
   WinMon_internal_clock(i, f);
}

WP_U32 get_core_id(void)
{
   return WinMon_get_core_id();
}
