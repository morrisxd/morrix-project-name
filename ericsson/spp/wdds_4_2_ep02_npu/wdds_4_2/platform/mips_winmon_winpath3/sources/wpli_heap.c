/*****************************************************************************
 * (C) Copyright 2000-2006, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: wpli_heap.c
 *
 * Purpose: mips_winmon heap allocator
 *
 * Functions contained in this file:
 *
 * +---------------------------+----------------------------------------------
 * |  Function name            | Description
 * +---------------------------+----------------------------------------------
 * |
 * |  API Functions
 * |  -------------------------+----------------------------------------------
 * |  (None)
 * |  -------------------------+----------------------------------------------
 * |
 * |  Internal WDDI Functions
 * |  -------------------------+----------------------------------------------
 * |  WPL_VeneerMallocInit     | Initialize the veneer memory heap
 * |  WPL_BspMallocInit        | Initialize BSP heap (WDDI only data)
 * |  WPL_VeneerMallocDone     | Free data defining veneer memory heap
 * |  WPL_BspMallocDone        | Free data defining BSP memory heap
 * +---------------------------+----------------------------------------------
 *
 ****************************************************************************/

#include "wpl_platform.h"
#include "wpi_heap_data.h"





