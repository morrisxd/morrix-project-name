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

#ifndef u_long
#define u_long WP_U32
#endif

#ifndef u_short
#define u_short WP_U16
#endif

#ifndef u_char
#define u_char WP_U8
#endif

#ifndef size_t
#define size_t WP_U32
#endif

/* used only for wddi-related malloc - not used in this platform */
#define THREAD_LOCK()
#define THREAD_UNLOCK()

#ifdef WPL_PRIVATE_HEAP_ALLOCATION 

#ifndef WPL_HEAP_SIZE
#error "WPL_HEAP_SIZE must be defined"
#endif
#ifndef WPL_HEAP_BUS
#error "WPL_HEAP_BUS must be defined"
#endif

#define WPL_MALLOC WPI_BspMalloc
#define WPL_FREE   WPI_BspFree

#else

extern void free (void *);
extern void *malloc(WP_U32);

#define WPL_MALLOC malloc
#define WPL_FREE   free

#endif /* WPL_PRIVATE_HEAP_ALLOCATION */

#define WPL_VA_LIST va_list
#define WPL_VA_START(arg1, arg2) va_start(arg1, arg2)
#define WPL_VA_ARG(arg1, arg2)   va_arg(arg1, arg2)
#define WPL_VA_END(arg1)         va_end(arg1)

void WPL_BspMallocInit(WP_U32 heapSize, void **res_private_heap);

void WPL_BspMallocDone(void **res_private_heap);

void *WPLI_VeneerBspMalloc(WP_U32 size);

void WPLI_VeneerBspFree(void* ptr);

#endif
