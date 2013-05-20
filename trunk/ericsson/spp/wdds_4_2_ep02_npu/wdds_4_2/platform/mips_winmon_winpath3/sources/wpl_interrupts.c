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
#include <wpl_mem_access.h>
#include <winmon.h>
#include <mt_api.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/******************************************************************/
/*                       Typedefs and defines                     */
/******************************************************************/

/******************************************************************/
/*                       Structures + Enumerators                 */
/******************************************************************/

/******************************************************************/
/*                       Global Database                          */
/******************************************************************/
extern WP_U32 wds_bsp_ver;

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

typedef void WPL_callback_handler(WP_S32 event_vector);

static void WPL_service_callback_handler(WP_S32 event_vector);
static void WPL_overrun_callback_handler(WP_S32 event_vector);
static void WPL_bus_error_callback_handler(WP_S32 event_vector);
static void WPL_sgmii_an_callback_handler(WP_S32 event_vector);
static void WPL_eint3_callback_handler(WP_S32 event_vector);

static void WPL_service_callback_handler_wpid(WP_U32 wpid, WP_S32 event_vector);
static void WPL_overrun_callback_handler_wpid(WP_U32 wpid, WP_S32 event_vector);
static void WPL_bus_error_callback_handler_wpid(WP_U32 wpid, WP_S32 event_vector);
static void WPL_sgmii_an_callback_handler_wpid(WP_U32 wpid, WP_S32 event_vector);
static void WPL_eint3_callback_handler_wpid(WP_U32 wpid, WP_S32 event_vector);
/******************************************************************/
/*                       API Functions                            */
/******************************************************************/

WP_U32 WPL_IntDisable(WP_U32 wpid, WPL_IntVector vector)
{
   WP_U32 i;
   WP_U32 mask;
   WP_S32 core_id = WinMon_get_core_id();
   WP_U32 core = 0;

   if (core_id == WPL_CORE0)
   {
      core = 0;
   } else if (core_id == WPL_CORE1)
   {
      core = 1;
   }

   switch (vector)
   {
   case WPL_IntServiceQueues:
   {
      mask = WPL_INT_IC_IR_SERVICE;
      for (i = 0; mask > 0; i++, mask >>= 1)
      {
         if (mask & 0x1)
         {
            WinMon_disable_int(IV_SEI_BASE+i, core);
         }
      }
      break;
   }
   case WPL_IntOverrunQueues:
   {
      mask = WPL_INT_IC_IR_OVERRUN;
      for (i = 0; mask > 0; i++, mask >>= 1)
      {
         if (mask & 0x1)
         {
            WinMon_disable_int(IV_SEI_BASE+i, core);
         }
      }
      break;
   }
   case WPL_BusError:
   {
      mask = WPL_INT_IC_IR_BUSERROR;
      for (i = 0; mask > 0; i++, mask >>= 1)
      {
         if (mask & 0x1)
         {
            WinMon_disable_int(IV_SYI_BASE+i, core);
         }
      }
      break;
   }
   case WPL_Eint3Ufe4:
   {
      mask = WPL_INT_IC_IR_EINT3;
      for (i = 0; mask > 0; i++, mask >>= 1)
      {
         if (mask & 0x1)
         {
            WinMon_disable_int(IV_SYI_BASE+i, core);
         }
      }
      break;
   }
   case WPL_SgmiiAn:
   {
      mask = WPL_INT_IC_IR_SGMIIAN;
      for (i = 0; mask > 0; i++, mask >>= 1)
      {
         if (mask & 0x1)
         {
            WinMon_disable_int(IV_SEI_BASE+i, core);
         }
      }
      break;
      
   }
   case WPL_SerdesInterrupt:
   {
      mask = WPL_INT_IC_IR_SERDES;
      for (i = 0; mask > 0; i++, mask >>= 1)
      {
         if (mask & 0x1)
         {
            WinMon_disable_int(IV_SEI_BASE+i, core);
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
   WP_S32 core_id = WinMon_get_core_id();
   WP_U32 core = 0;

   if (core_id == WPL_CORE0)
      core = 0;
   else if (core_id == WPL_CORE1)
      core = 1;

   switch (vector)
   {
   case WPL_IntServiceQueues:
   {
      mask = WPL_INT_IC_IR_SERVICE;
      for (i = 0; mask > 0; i++, mask >>= 1)
      {
         if (mask & 0x1)
         {
            WinMon_enable_int(IV_SEI_BASE+i, core);
         }
      }
      break;
   }
   case WPL_IntOverrunQueues:
   {
      mask = WPL_INT_IC_IR_OVERRUN;
      for (i = 0; mask > 0; i++, mask >>= 1)
      {
         if (mask & 0x1)
         {
            WinMon_enable_int(IV_SEI_BASE+i, core);
         }
      }
      break;
   }
   case WPL_BusError:
   {
      mask = WPL_INT_IC_IR_BUSERROR;
      for (i = 0; mask > 0; i++, mask >>= 1)
      {
         if (mask & 0x1)
         {
            WinMon_enable_int(IV_SYI_BASE+i, core);
         }
      }
      break;
   }
  case WPL_Eint3Ufe4:
   {
      mask = WPL_INT_IC_IR_EINT3;
      for (i = 0; mask > 0; i++, mask >>= 1)
      {
         if (mask & 0x1)
         {
            WinMon_enable_int(IV_SYI_BASE+i, core);
         }
      }

      break;
   }
   case WPL_SgmiiAn:
   {
      mask = WPL_INT_IC_IR_SGMIIAN;
      for (i = 0; mask > 0; i++, mask >>= 1)
      {
         if (mask & 0x1)
         {
            WinMon_enable_int(IV_SEI_BASE+i, core);
         }
      }
      break;
   }
   case WPL_SerdesInterrupt:
   {
      mask = WPL_INT_IC_IR_SERDES;
      for (i = 0; mask > 0; i++, mask >>= 1)
      {
         if (mask & 0x1)
         {
            WinMon_enable_int(IV_SEI_BASE+i, core);
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

void WPL_InterruptConfigureEint3(WP_U32 wpid)
{
   WP_U32 val;
   WP_U32 *IC_IntMode;
   extern WP_U32 wpl_rif_base;

   if (wpl_rif_base == 0)
      WPLI_BusesRifBaseGet(wpid,&wpl_rif_base);

   IC_IntMode = ((WP_U32 *)(wpl_rif_base + 0xc2dc));

   WP_MEM_GET(val, *IC_IntMode);
   val |= 0x8000;
   WP_MEM_SET(*IC_IntMode, val);
}

static void WPL_service_callback_handler(WP_S32 event_vector)
{
   WPL_service_callback_handler_wpid(WPL_WINPATH(0), event_vector);
}

static void WPL_service_callback_handler_wpid(WP_U32 wpid, WP_S32 event_vector)
{
   if (wddi_callback_ptrs[WPL_IntServiceQueues] != NULL)
   {
      wddi_callback_ptrs[WPL_IntServiceQueues](wpid, event_vector);
   }
}

static void WPL_overrun_callback_handler(WP_S32 event_vector)
{
   WPL_overrun_callback_handler_wpid(WPL_WINPATH(0), event_vector);
}

static void WPL_overrun_callback_handler_wpid(WP_U32 wpid, WP_S32 event_vector)
{
   if (wddi_callback_ptrs[WPL_IntOverrunQueues] != NULL)
   {
      wddi_callback_ptrs[WPL_IntOverrunQueues](wpid, event_vector);
   }
}

static void WPL_bus_error_callback_handler(WP_S32 event_vector)
{
   WPL_bus_error_callback_handler_wpid(WPL_WINPATH(0), event_vector);
}

static void WPL_bus_error_callback_handler_wpid(WP_U32 wpid, WP_S32 event_vector)
{
   if (wddi_callback_ptrs[WPL_BusError] != NULL) {
            wddi_callback_ptrs[WPL_BusError](wpid, event_vector);
   }
}

static void WPL_eint3_callback_handler(WP_S32 event_vector)
{
   WPL_eint3_callback_handler_wpid(WPL_WINPATH(0), event_vector);
}

static void WPL_eint3_callback_handler_wpid(WP_U32 wpid, WP_S32 event_vector)
{
   if (wddi_callback_ptrs[WPL_Eint3Ufe4] != NULL) {
            wddi_callback_ptrs[WPL_Eint3Ufe4](wpid, event_vector);
   }
}

static void WPL_sgmii_an_callback_handler_wpid(WP_U32 wpid, WP_S32 event_vector)
{
   if (wddi_callback_ptrs[WPL_SgmiiAn] != NULL) {
            wddi_callback_ptrs[WPL_SgmiiAn](wpid, event_vector);
   }
}

static void WPL_sgmii_an_callback_handler(WP_S32 event_vector)
{
   WPL_sgmii_an_callback_handler_wpid(WPL_WINPATH(0), event_vector);
}

static void WPL_serdes_interrupt_callback_handler_wpid(WP_U32 wpid, WP_S32 event_vector)
{
   if (wddi_callback_ptrs[WPL_SerdesInterrupt] != NULL) {
            wddi_callback_ptrs[WPL_SerdesInterrupt](wpid, event_vector);
   }
}

static void WPL_serdes_interrupt_callback_handler(WP_S32 event_vector)
{
   if (wddi_callback_ptrs[WPL_SerdesInterrupt] != NULL) {
            wddi_callback_ptrs[WPL_SerdesInterrupt](WPL_WINPATH(0), event_vector);
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
      mask = WPL_INT_IC_IR_SERVICE;
      for (i = 0; mask > 0; i++, mask >>= 1)
      {
         if (mask & 0x1)
         {
            if(wds_bsp_ver >= WPL_BSP_MULTI_WINPATH_VERSION)
               int_connect(IV_SEI_BASE + i, (WPL_callback_handler *)WPL_service_callback_handler_wpid);
            else
               int_connect(IV_SEI_BASE + i, WPL_service_callback_handler);
            wddi_callback_ptrs[vector] = wddi_callback;
         }
      }
      break;
   }
   case WPL_IntOverrunQueues:
   {
      mask = WPL_INT_IC_IR_OVERRUN;
      for (i = 0; mask > 0; i++, mask >>= 1)
      {
         if (mask & 0x1)
         {
            if(wds_bsp_ver >= WPL_BSP_MULTI_WINPATH_VERSION)
               int_connect(IV_SEI_BASE + i, (WPL_callback_handler *)WPL_overrun_callback_handler_wpid);
            else
               int_connect(IV_SEI_BASE + i, WPL_overrun_callback_handler);
            wddi_callback_ptrs[vector] = wddi_callback;
         }
      }
      break;
   }
   case WPL_BusError:
   {
      mask = WPL_INT_IC_IR_BUSERROR;
      for (i = 0; mask > 0; i++, mask >>= 1)
      {
         if (mask & 0x1)
         {
            if(wds_bsp_ver >= WPL_BSP_MULTI_WINPATH_VERSION)
               int_connect(IV_SYI_BASE + i, (WPL_callback_handler *)WPL_bus_error_callback_handler_wpid);
            else
               int_connect(IV_SYI_BASE + i, WPL_bus_error_callback_handler);
            wddi_callback_ptrs[vector] = wddi_callback;
         }
      }
      break;
   }
   case WPL_SgmiiAn:
   {
      mask = WPL_INT_IC_IR_SGMIIAN;
      for (i = 0; mask > 0; i++, mask >>= 1)
      {
         if (mask & 0x1)
         {
            if(wds_bsp_ver >= WPL_BSP_MULTI_WINPATH_VERSION)
               int_connect(IV_SEI_BASE + i, (WPL_callback_handler *)WPL_sgmii_an_callback_handler_wpid);
            else
               int_connect(IV_SEI_BASE + i, WPL_sgmii_an_callback_handler);
            wddi_callback_ptrs[vector] = wddi_callback;
         }
      }
      break;
   }
  case WPL_Eint3Ufe4:
   {
      mask = WPL_INT_IC_IR_EINT3;
      for (i = 0; mask > 0; i++, mask >>= 1)
      {
         if (mask & 0x1)
         {
            if(wds_bsp_ver >= WPL_BSP_MULTI_WINPATH_VERSION)
               int_connect(IV_SYI_BASE + i, (WPL_callback_handler *)WPL_eint3_callback_handler_wpid);
            else
               int_connect(IV_SYI_BASE + i, WPL_eint3_callback_handler);
            wddi_callback_ptrs[vector] = wddi_callback;
         }
      }
      break;
   }
   case WPL_SerdesInterrupt:
   {
      mask = WPL_INT_IC_IR_SERDES;
      for (i = 0; mask > 0; i++, mask >>= 1)
      {
         if (mask & 0x1)
         {
            if(wds_bsp_ver >= WPL_BSP_MULTI_WINPATH_VERSION)
               int_connect(IV_SEI_BASE + i, (WPL_callback_handler *)WPL_serdes_interrupt_callback_handler_wpid);
            else
               int_connect(IV_SEI_BASE + i, WPL_serdes_interrupt_callback_handler);
            wddi_callback_ptrs[vector] = wddi_callback;
         }
      }
      break;
   }

   default:
      break;
   }
}

