/*****************************************************************************
 * (C) Copyright 2000-2006, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: wpl_alloc.c
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wpl_types.h"
#include "wpl_platform.h"


#ifdef WPL_PRIVATE_HEAP_ALLOCATION
#define PRIVATE 0

#define private_default_maxpages (8192*2)    /* enough for 64MB */
#define shared_default_maxpages 8192    /* enough for 32MB */

/*
 * Magic values to put in the page_directory
 */
#define MALLOC_NOT_MINE ((wpl_pginfo*) 0)
#define MALLOC_FREE     ((wpl_pginfo*) 1)
#define MALLOC_FIRST    ((wpl_pginfo*) 2)
#define MALLOC_FOLLOW   ((wpl_pginfo*) 3)
#define MALLOC_MAGIC    ((wpl_pginfo*) 4)

wpl_heapinfo theHeaps[2];

/* Secondary structures for the PRIVATE heap */
wpl_pginfo private_pginfo[private_default_maxpages];
wpl_pgfree private_pgfree[private_default_maxpages];
wpl_pginfo *private_page_dir[malloc_pageshift + private_default_maxpages];

/* Secondary structures for the SHARED heap */
wpl_pginfo shared_pginfo[shared_default_maxpages];
wpl_pgfree shared_pgfree[shared_default_maxpages];
wpl_pginfo *shared_page_dir[malloc_pageshift + shared_default_maxpages];

void WPLI_HeapSizing(wpl_heapinfo *h, void *addr_a, void *addr_b);
void *WPLI_HeapInit(wpl_heapinfo *h);
wpl_pgfree *WPLI_pool_pgfree_alloc(wpl_heapinfo *h);
#endif

/*****************************************************************************
 *
 * Function: WPL_BspMallocInit
 *
 * Purpose: Initialize BSP heap (WDDI only data)
 *
 * Description:
 *
 * Inputs:
 *     heapSize    number of bytes requested in the heap
 *
 * Outputs:
 *     None
 *
 * Return Value:
 *     first_page  if heap initialized successfully,
 *     NULL        otherwise
 *
 * Called by:
 *
 *     WPL_HeapInit  (common/wpi_bsp.c)
 *
 ****************************************************************************/

void WPL_BspMallocInit(WP_U32 heapSize, void **res_private_heap)
{
#ifdef WPL_PRIVATE_HEAP_ALLOCATION
/* Determine the starting address from the heap. */
   wpi_veneer_memory wpl_bus_mappings[WPI_BUS_LAST];
   WP_U32 WPL_heapBaseAddress;
   
   WPL_BusesMapGet(wpl_bus_mappings);
   
   WPL_heapBaseAddress = (WP_U32)
      (wpl_bus_mappings[WP_BUS_PARAM].actual_start +
       wpl_bus_mappings[WP_BUS_PARAM].size + wpl_bus_mappings[WP_BUS_PARAM].uncached_offset);

   memset(&theHeaps[PRIVATE], 0, sizeof(wpl_heapinfo));

   WPLI_HeapSizing(&theHeaps[PRIVATE], (void *) WPL_heapBaseAddress,
                  (void *) (WPL_heapBaseAddress + heapSize));

   theHeaps[PRIVATE].pool_pginfo = &private_pginfo[0];
   theHeaps[PRIVATE].pool_pgfree = &private_pgfree[0];
   theHeaps[PRIVATE].page_dir = &private_page_dir[0];

   *res_private_heap = (void *) &theHeaps[PRIVATE];

   WPLI_HeapInit((wpl_heapinfo *)(*res_private_heap));

   return;

#else

   *res_private_heap = NULL;
   return;

#endif
}

/*****************************************************************************
 *
 * Function: WPL_BspMallocDone
 *
 * Purpose:  Free data defining BSP memory heap allocated on vxWorks heap
 *
 * Description:
 *
 * Inputs:
 *     None
 *
 * Outputs:
 *     None
 *
 * Return Value:
 *     None
 *
 * Called by:
 *
 *     WPL_ReleaseDynamicStorage (wpl_alloc.c)
 *
 ****************************************************************************/

void WPL_BspMallocDone(void **res_private_heap)
{
   *res_private_heap = NULL;
}

/*****************************************************************************
 *
 * Function: WPLI_HeapSizing
 *
 * Purpose: Determine the number of pages to be controlled in the heap
 *
 * Description:
 *
 * Inputs:
 *     h        the heap
 *     addr_a   start address of the heap memory
 *     addr_b   end address of the heap memory (next byte beyond)
 *
 * Outputs:
 *     several fields in the heap structure h are filled in
 *
 * Return Value:
 *     None
 *
 * Called by:
 *
 *     WPL_VeneerMallocInit (target/<platform>/sources/wpl_alloc.c)
 *     WPL_BspMallocInit (target/<platform>/sources/wpl_alloc.c)
 *     WPL_PrivateMemoryInit (target/<platform>/sources/wpl_alloc.c)
 *
 ****************************************************************************/

#ifdef WPL_PRIVATE_HEAP_ALLOCATION

void WPLI_HeapSizing(wpl_heapinfo *h, void *addr_a, void *addr_b)
{
   WP_U32 total_pages, directory_entries;

   h->malloc_a = (u_long) addr_a;
   h->malloc_b = (u_long) addr_b;

   h->first_page = pageround(h->malloc_a);
   h->after_page = h->malloc_b & ~malloc_pagemask;

   total_pages = (h->after_page - h->first_page) >> malloc_pageshift;
   directory_entries = total_pages + malloc_pageshift;
   h->malloc_ninfo = total_pages;
   h->malloc_dinfo = directory_entries;
}

#endif

/*****************************************************************************
 *
 * Function: WPLI_HeapInit
 *
 * Purpose: Initialize the heap defining structure
 *
 * Description:
 *
 * Inputs:
 *     h    the heap
 *
 * Outputs:
 *     fields in the heap structure
 *     pool_pgfree and pool_pginfo lists initialized
 *
 * Return Value:
 *     first_page  first allocatable page in the heap
 *
 * Called by:
 *
 *     WPL_VeneerMallocInit (target/<platform>/sources/wpl_alloc.c)
 *     WPL_BspMallocInit (target/<platform>/sources/wpl_alloc.c)
 *     WPL_PrivateMemoryInit (target/<platform>/sources/wpl_alloc.c)
 *
 ****************************************************************************/

#ifdef WPL_PRIVATE_HEAP_ALLOCATION

void *WPLI_HeapInit(wpl_heapinfo *h)
{
   WP_U32 j;
   wpl_pgfree *px;
   u_long first_page, after_page;

   first_page = h->first_page;
   after_page = h->after_page;

   /*
    * We need a maximum of malloc_pageshift buckets, steal these from the
    * front of the page_directory;
    */
   h->malloc_origo = first_page >> malloc_pageshift;
   h->malloc_origo -= malloc_pageshift;

   h->last_index = ptr2index(h, after_page) - 1;
   for (j = 0; j < malloc_pageshift; j++)
      h->page_dir[j] = NULL;
   for (j = malloc_pageshift; j <= h->last_index; j++)
      h->page_dir[j] = MALLOC_FREE;

   /* initialize pools */
   for (j = 1; j < h->malloc_ninfo; j++)
   {
      h->pool_pgfree[j].next = &h->pool_pgfree[j - 1];
      h->pool_pginfo[j].next = &h->pool_pginfo[j - 1];
   }

   h->pool_pgfree_head = &(h->pool_pgfree[h->malloc_ninfo - 1]);
   h->pool_pginfo_head = &h->pool_pginfo[h->malloc_ninfo - 1];

   /*
    * set up first free-page list
    */

   px = WPLI_pool_pgfree_alloc(h);

   px->page = (void *) first_page;
   px->end = (void *) after_page;
   px->size = after_page - first_page;
   px->next = NULL;
   px->prev = &h->free_list;

   h->free_list.next = px;

   /* Been here, done that */
   ++h->heap_initialized;

   return (void *) first_page;
}

#endif

/*****************************************************************************
 *
 * Function: WPLI_pool_pgfree_alloc
 *
 * Purpose: Allocate page from pool_pgfree list
 *
 * Description:
 *
 * Inputs:
 *     h   the heap
 *
 * Outputs:
 *     None
 *
 * Return Value:
 *     result  the allocated page
 *
 * Called by:
 *     WPLI_HeapInit
 *
 ****************************************************************************/

#ifdef WPL_PRIVATE_HEAP_ALLOCATION

wpl_pgfree *WPLI_pool_pgfree_alloc(wpl_heapinfo *h)
{
   wpl_pgfree *result = h->pool_pgfree_head;
   h->pool_pgfree_head = h->pool_pgfree_head->next;

   return result;
}

#endif


