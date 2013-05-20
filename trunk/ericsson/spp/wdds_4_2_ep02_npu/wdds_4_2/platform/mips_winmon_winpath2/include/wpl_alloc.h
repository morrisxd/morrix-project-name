/*************************************************************
 * (C) Copyright 2000-2009, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ************************************************************/

/*****************************************************************************
 *
 * Module: wpl_alloc.h
 * Purpose:
 *
 * This module contains platform-specific data types.
 *****************************************************************************/

#ifndef WPL_ALLOC_H
#define WPL_ALLOC_H

#include "wpl_types.h"
#include "wpi_heap_data.h"

/* no threads */
#define THREAD_LOCK()
#define THREAD_UNLOCK()

#define WPL_MALLOC WPI_BspMalloc
#define WPL_FREE   WPI_BspFree

#define WPL_VA_LIST va_list
#define WPL_VA_START(arg1, arg2) va_start(arg1, arg2)
#define WPL_VA_ARG(arg1, arg2)   va_arg(arg1, arg2)
#define WPL_VA_END(arg1)         va_end(arg1)

void WPL_BspMallocInit(WP_U32 heapSize, void **res_private_heap);

void WPL_BspMallocDone(void **res_private_heap);

#endif
