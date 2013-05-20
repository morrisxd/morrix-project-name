/*****************************************************************************
 * (C) Copyright 2000-2009, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: platform/mips_winmon_winpath3/sources/wpl_interrupts.c
 *
 * Purpose: Winpath's platform dependent functions
 *
 * Functions contained in this file:
 *
 * +---------------------------+----------------------------------------------
 * |  Function name            | Description
 * +---------------------------+----------------------------------------------
 * |
 * |  API Functions
 * |  -------------------------+----------------------------------------------
 * |  -------------------------+----------------------------------------------
 * |  
 * |  Internal Functions
 * |  -------------------------+----------------------------------------------
 * |  -------------------------+----------------------------------------------
 * |  
 * |  Internal Functions (static)
 * |  -------------------------+----------------------------------------------
 * +---------------------------+----------------------------------------------
 *
 ****************************************************************************/

/******************************************************************/
/*                       Includes                                 */
/******************************************************************/

#include <wpl_types.h>
#include <wpl_platform.h>
#include <wpl_interrupts.h>
#include <winmon.h>
#include <signal.h>
#include <stdio.h>

/******************************************************************/
/*                       Typedefs and defines                     */
/******************************************************************/

/******************************************************************/
/*                       Structures + Enumerators                 */
/******************************************************************/

/******************************************************************/
/*                       Global Database                          */
/******************************************************************/

/******************************************************************/
/*                       Local Database                           */
/******************************************************************/

WPL_IntHandlerCallback *wddi_callback_ptrs[WPL_IntVectorNum] = {0};

/******************************************************************/
/*                       Macros + inlines                         */
/******************************************************************/

/******************************************************************/
/*                       Forward Declarations                     */
/******************************************************************/

static void WPL_service_callback_handler(WP_S32 event_vector);
static void WPL_overrun_callback_handler(WP_S32 event_vector);
static void WPL_bus_error_callback_handler(WP_S32 event_vector);

/******************************************************************/
/*                       API Functions                            */
/******************************************************************/

WP_U32 WPL_IntDisable(WP_U32 wpid, WPL_IntVector vector)
{
   WP_U32 i;
   WP_U32 mask;

   switch (vector)
   {
   case WPL_IntServiceQueues:
   {
      mask = WPL_INT_SEIR_SERVICE;
      for (i=0; mask>0; i++, mask>>=1)
      {
         if (mask & 0x1)
         {
            disable_int(IV_SEI_BASE+i);
         }
      }
      break;
   }
   case WPL_IntOverrunQueues:
   {
      mask = WPL_INT_SEIR_OVERRUN;
      for (i=0; mask>0; i++, mask>>=1)
      {
         if (mask & 0x1)
         {
            disable_int(IV_SEI_BASE+i);
         }
      }
      break;
   }
   case WPL_BusError:
   {
      mask = WPL_INT_SYIR_BUSMONITORS;
      for (i=0; mask>0; i++, mask>>=1)
      {
         if (mask & 0x1)
         {
            disable_int(IV_SYI_BASE+i);
         }
      }
      break;
   }
   case WPL_IntGlobalState:
   {
      WinMon_disable_interrupts();
      break;
   }
   default:
      return WPL_ERR(WPL_ERR_INT_NOT_SUPPORTED);
   }

   return WPL_OK;
}

WP_U32 WPL_IntEnable(WP_U32 wpid, WPL_IntVector vector)
{
   WP_U32 i;
   WP_U32 mask;

   switch (vector)
   {
   case WPL_IntServiceQueues:
   {
      mask = WPL_INT_SEIR_SERVICE;
      for (i=0; mask>0; i++, mask>>=1)
      {
         if (mask & 0x1)
         {
            enable_int(IV_SEI_BASE+i);
         }
      }
      break;
   }
   case WPL_IntOverrunQueues:
   {
      mask = WPL_INT_SEIR_OVERRUN;
      for (i=0; mask>0; i++, mask>>=1)
      {
         if (mask & 0x1)
         {
            enable_int(IV_SEI_BASE+i);
         }
      }
      break;
   }
   case WPL_BusError:
   {
      mask = WPL_INT_SYIR_BUSMONITORS;
      for (i=0; mask>0; i++, mask>>=1)
      {
         if (mask & 0x1)
         {
            enable_int(IV_SYI_BASE+i);
         }
      }
      break;
   }
   case WPL_IntGlobalState:
   {
      WinMon_enable_interrupts();
      break;
   }
   default:
      return WPL_ERR(WPL_ERR_INT_NOT_SUPPORTED);
   }

   return WPL_OK;
}

static void WPL_service_callback_handler(WP_S32 event_vector)
{
   if (wddi_callback_ptrs[WPL_IntServiceQueues] != NULL) {
      wddi_callback_ptrs[WPL_IntServiceQueues](WPL_WINPATH(0), event_vector);
   }
}

static void WPL_overrun_callback_handler(WP_S32 event_vector)
{
   if (wddi_callback_ptrs[WPL_IntOverrunQueues] != NULL) {
         wddi_callback_ptrs[WPL_IntOverrunQueues](WPL_WINPATH(0), event_vector);
   }
}
static void WPL_bus_error_callback_handler(WP_S32 event_vector)
{
   if (wddi_callback_ptrs[WPL_BusError] != NULL) {
            wddi_callback_ptrs[WPL_BusError](WPL_WINPATH(0), event_vector);
   }
}

void WPL_IntConnect(WP_U32 wpid, WPL_IntVector vector,
                    WPL_IntHandler *function,
                    WPL_IntHandlerCallback *wddi_callback)
{
   WP_U32 i;
   WP_U32 mask;

   switch (vector)
   {
   case WPL_IntServiceQueues:
   {
      mask = WPL_INT_SEIR_SERVICE;
      for (i=0; mask>0; i++, mask>>=1)
      {
         if (mask & 0x1)
         {
            int_connect(IV_SEI_BASE+i, WPL_service_callback_handler);
            wddi_callback_ptrs[vector] = wddi_callback;
         }
      }
      break;
   }
   case WPL_IntOverrunQueues:
   {
      mask = WPL_INT_SEIR_OVERRUN;
      for (i=0; mask>0; i++, mask>>=1)
      {
         if (mask & 0x1)
         {
            int_connect(IV_SEI_BASE+i, WPL_overrun_callback_handler);
            wddi_callback_ptrs[vector] = wddi_callback;
         }
      }
      break;
   }
   case WPL_BusError:
   {
      mask = WPL_INT_SYIR_BUSMONITORS;
      for (i=0; mask>0; i++, mask>>=1)
      {
         if (mask & 0x1)
         {
            int_connect(IV_SYI_BASE+i, WPL_bus_error_callback_handler);
            wddi_callback_ptrs[vector] = wddi_callback;
         }
      }
      break;
   }
   default:
      break;
   }
}

