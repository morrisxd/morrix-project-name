/*****************************************************************************
 * (C) Copyright 2000-2009, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: platform/mips_linux/sources/wpl_winipc_driver.c
 *
 * Purpose: Winpath's WinIPC related platform dependent functions
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
#include <wpl_mem_access.h>

#include <winmon.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#include <wpli_platform.h>
#include "wpl_winipc_driver.h"

/******************************************************************/
/*                       Typedefs and defines                     */
/******************************************************************/

#define WPL_NUM_SIGNALS     2

#define WPL_INT_RAISE(sw_int)                             \
do{                                                       \
   WP_U32* address = ((WP_U32*)(wpl_rif_base + 0xc2d8));  \
   switch (sw_int)                                        \
   {                                                      \
      case 35:                                            \
         WP_MEM_SET(*address, 1 << 0);                    \
         break;                                           \
      case 36:                                            \
         WP_MEM_SET(*address, 1 << 1);                    \
         break;                                           \
      default:                                            \
         return WPL_ERR(WPL_ERR_INVALID_PARAM);           \
   }                                                      \
}while(0)

#define WPL_INT_WAIT(sw_int)                              \
do{                                                       \
   WP_U32* address = ((WP_U32*)(wpl_rif_base + 0xc284));  \
   WP_U32  intr;                                          \
   WP_U32  comp = 0;                                      \
   switch (sw_int)                                        \
   {                                                      \
      case 35:                                            \
         comp = 1 << 0;                                   \
         break;                                           \
      case 36:                                            \
         comp = 1 << 1;                                   \
         break;                                           \
      default:                                            \
         return WPL_ERR(WPL_ERR_INVALID_PARAM);           \
   }                                                      \
   WP_MEM_GET(intr, *address);                            \
   while (intr & comp)                                    \
   {                                                      \
      WP_MEM_GET(intr, *address);                         \
   }                                                      \
}while(0)

/******************************************************************/
/*                       Structures + Enumerators                 */
/******************************************************************/


/******************************************************************/
/*                       Global Database                          */
/******************************************************************/

extern WP_U32 wpl_rif_base;

/******************************************************************/
/*                       Local Database                           */
/******************************************************************/

static WPL_sig_handler_type signal_handler[WPL_NUM_SIGNALS] = {0};

/******************************************************************/
/*                       Macros + inlines                         */
/******************************************************************/


/******************************************************************/
/*                       Forward Declarations                     */
/******************************************************************/

static void WPLI_WinipcUserIntrHandler(WP_S32);

static void WPLI_WinipcReqIntrHandler(WP_S32);


/******************************************************************/
/*                       API Functions                            */
/******************************************************************/


WP_U32 WPL_WinipcHandlerRegister(WPL_sig_handler_type pi_cb, WP_S32 i_signal)
{
   WP_S32 core;

   /* Register with the interrupt with WPLI_WinipcIntrHandler */
   switch (i_signal)
   {
      case WPL_NTR_SIGNAL_REQ:
         signal_handler[0] = pi_cb;
         WinMon_int_connect(i_signal, WPLI_WinipcReqIntrHandler);
         break;
      case WPL_NTR_SIGNAL_USR:
         signal_handler[1] = pi_cb;
         WinMon_int_connect(i_signal, WPLI_WinipcUserIntrHandler);
         break;
      default:
         printf("PLATFORM: signal value not supported\n");
         return WPL_ERR(WPL_ERR_INVALID_PARAM);
   }

   core = (WinMon_get_core_id() == 0x0A2);

   /* Enable interrupt reception in current core */
   WinMon_enable_int(i_signal, core);
   WinMon_enable_interrupts();
   
   return WPL_OK;
}

WP_U32 WPL_WinipcClientRegister(WP_U32 i_client_id)
{
   /* Client specific registration is not supported by WinMon */

   return WPL_OK;
}

WP_U32 WPL_WinipcSignalRaise(WP_U32 i_to_client, WP_boolean i_user_event)
{
   /* Verify wpl_rif_base in initialized */
   if (wpl_rif_base == NULL)
   {
      WPLI_BusesRifBaseGet(0, &wpl_rif_base);
   }

   if (i_user_event)
   {
      WPL_INT_RAISE(WPL_NTR_SIGNAL_USR);
      WPL_INT_WAIT(WPL_NTR_SIGNAL_USR);
   }
   else
   {
      WPL_INT_RAISE(WPL_NTR_SIGNAL_REQ);
      WPL_INT_WAIT(WPL_NTR_SIGNAL_REQ);
   }

   return WPL_OK;
}

/******************************************************************/
/*                       Static functions                         */
/******************************************************************/

static void WPLI_WinipcUserIntrHandler(WP_S32 unused)
{
   if (signal_handler[1])
   {
      signal_handler[1](WPL_NTR_SIGNAL_USR);
   }
}

static void WPLI_WinipcReqIntrHandler(WP_S32 unused)
{
   if (signal_handler[0])
   {
      signal_handler[0](WPL_NTR_SIGNAL_REQ);
   }
}
