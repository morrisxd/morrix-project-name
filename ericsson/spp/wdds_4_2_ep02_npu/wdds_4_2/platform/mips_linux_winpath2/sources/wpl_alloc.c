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

#define WPL_PRIVATE_HEAP_ALLOCATION 0

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
   *res_private_heap = NULL;
   return;
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

#if WPL_PRIVATE_HEAP_ALLOCATION
void *my_malloc( WP_U32 size );
void my_free( void *ptr );
#else
#define my_malloc malloc
#define my_free free
#endif

void *WPLI_VeneerBspMalloc(WP_U32 size)
{
   void *ptr;

   ptr = (void *) my_malloc(size);
   return (ptr);
}

void WPLI_VeneerBspFree(void* ptr)
{
   if (ptr)
      my_free(ptr);
}

#if WPL_PRIVATE_HEAP_ALLOCATION

#define WPL_USED 1

typedef struct
{
   WP_U32 size;
}WPL_mem_unit;

typedef struct
{
   WPL_mem_unit* free;
   WPL_mem_unit* heap;
}WPL_mem_system;

#define WPL_UNINITIALIZED -1
static WPL_mem_system msys =
{  (WPL_mem_unit*)WPL_UNINITIALIZED, (WPL_mem_unit*)WPL_UNINITIALIZED};
static WP_U32 malloc_num_of_calls = 0;

extern WP_U32 _heap_base;
static WP_U32 heapStart(void)
{
   return (WP_U32)&(_heap_base);
}

extern WP_U32 _sp;
static WP_U32 spStart(void)
{
   return (WP_U32)&(_sp);
}

static WPL_mem_unit* compact( WPL_mem_unit *p, WP_U32 nsize );

static void alloc_compact( void )
{
   msys.free = compact( msys.heap, 0x7fffffff );
}

static WPL_mem_unit* compact( WPL_mem_unit *p, WP_U32 nsize )
{
   WP_U32 bsize, psize;
   WPL_mem_unit *best;

   best = p;
   bsize = 0;

   while( psize = p->size, psize )
   {
      if( psize & WPL_USED )
      {
         if( bsize != 0 )
         {
            best->size = bsize;
            if( bsize >= nsize )
            {
               return best;
            }
         }
         bsize = 0;
         best = p = (WPL_mem_unit *)( (WP_U32)p + (psize & ~WPL_USED) );
      }
      else
      {
         bsize += psize;
         p = (WPL_mem_unit *)( (WP_U32)p + psize );
      }

   }

   if( bsize != 0 )
   {
      best->size = bsize;
      if( bsize >= nsize )
      {
         return best;
      }
   }

   return 0;
}

void alloc_init( void *heap, WP_U32 len )
{
   malloc_num_of_calls = 0;
   len += 3;
   len >>= 2;
   len <<= 2;
   msys.free = msys.heap = (WPL_mem_unit *) heap;
   msys.free->size = msys.heap->size = len - sizeof(WPL_mem_unit);
   *(WP_U32 *)((WP_U8 *)heap + len - 4) = 0;
}

void *my_malloc( WP_U32 size )
{
   WP_U32 fsize;
   WPL_mem_unit *p;

   if( malloc_num_of_calls == 0 && (WP_U32)msys.free == WPL_UNINITIALIZED && (WP_U32)msys.heap == WPL_UNINITIALIZED )
   {
      WP_U32 hp_start = heapStart();
      WP_U32 sp_start = spStart();
      alloc_init( (void*)hp_start, sp_start - hp_start );
   }

   if( size == 0 ) return 0;

   size += 3 + sizeof(WPL_mem_unit);
   size >>= 2;
   size <<= 2;

   if( msys.free == 0 || size > msys.free->size )
   {
      msys.free = compact( msys.heap, size );
      if( msys.free == 0 ) return 0;
   }

   p = msys.free;
   fsize = msys.free->size;

   if( fsize >= size + sizeof(WPL_mem_unit) )
   {
      msys.free = (WPL_mem_unit *)( (WP_U32)p + size );
      msys.free->size = fsize - size;
   }
   else
   {
      msys.free = 0;
      size = fsize;
   }

   p->size = size | WPL_USED;

   if( !(++malloc_num_of_calls % 15) )
   alloc_compact();

   return (void *)( (WP_U32)p + sizeof(WPL_mem_unit) );
}

void my_free( void *ptr )
{
   if( ptr )
   {
      WPL_mem_unit *p;

      p = (WPL_mem_unit *)( (WP_U32)ptr - sizeof(WPL_mem_unit) );
      p->size &= ~WPL_USED;
   }
}

#endif
