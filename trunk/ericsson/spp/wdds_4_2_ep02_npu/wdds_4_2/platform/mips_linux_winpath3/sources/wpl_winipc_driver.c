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

#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <wpl_platform.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <sys/ioctl.h>

#include "wpl_winipc_driver.h"


/******************************************************************/
/*                       Typedefs and defines                     */
/******************************************************************/

#define WPL_NUM_SIGNALS     2

#define WPL_IOCTL_CLNT_REGISTER    0
#define WPL_IOCTL_RAISE_REQ_EVENT  1
#define WPL_IOCTL_RAISE_USER_EVENT 2

#define WPL_NTR_CLNT_INFO(clnt_id) ((clnt_id << 16) + (WP_U16) getpid())

#define WPL_WINIPC_ACCEPT_INTERRUPTS 1

#if (WPL_WINIPC_ACCEPT_INTERRUPTS == 1)
#define WPL_WINIPC_OPEN_FLAG O_RDWR
#else
#define WPL_WINIPC_OPEN_FLAG O_RDWR | O_NOFOLLOW
#endif

/******************************************************************/
/*                       Structures + Enumerators                 */
/******************************************************************/


/******************************************************************/
/*                       Global Database                          */
/******************************************************************/


/******************************************************************/
/*                       Local Database                           */
/******************************************************************/

static WP_S32 wpl_ntr_device = 0;

static WP_S32 signal_number = 0;
static WPL_sig_handler_type signal_handler[WPL_NUM_SIGNALS] = {0};

/******************************************************************/
/*                       Macros + inlines                         */
/******************************************************************/


/******************************************************************/
/*                       Forward Declarations                     */
/******************************************************************/

static WP_U32 WPLI_WinipcDeviceOpen(void);

static WP_S32 WPLI_WinipcUserIntrHandler(void);

static WP_S32 WPLI_WinipcReqIntrHandler(void);


/******************************************************************/
/*                       API Functions                            */
/******************************************************************/

WP_U32 WPL_WinipcHandlerRegister(WPL_sig_handler_type pi_cb, WP_S32 i_signal)
{
   struct sigaction siginfo;

   signal_number = i_signal;
   sigaction(i_signal, NULL, &siginfo);

   /* Register with the interrupt with WPLI_WinipcXXXXIntrHandler */
   switch (i_signal)
   {
      case WPL_NTR_SIGNAL_REQ:
         signal_handler[0] = pi_cb;
         siginfo.sa_sigaction = (void (*)(int, siginfo_t *, void *)) WPLI_WinipcReqIntrHandler;
         break;
      case WPL_NTR_SIGNAL_USR:
         signal_handler[1] = pi_cb;
         siginfo.sa_sigaction = (void (*)(int, siginfo_t *, void *)) WPLI_WinipcUserIntrHandler;
         break;
      default:
         printf("PLATFORM: signal value not supported\n");
         return WPL_ERR(WPL_ERR_INVALID_PARAM);
   }

   sigemptyset(&siginfo.sa_mask);
   siginfo.sa_flags = SA_SIGINFO;
   if (sigaction(i_signal, &siginfo, NULL) == -1)
   {
      perror("PLATFORM: could not register to a signal");
      return WPL_ERR(WPL_ERR_PLATFORM);
   }

   return WPL_OK;
}




WP_U32 WPL_WinipcClientRegister(WP_U32 i_client_id)
{
   WP_U32 info;

   if (wpl_ntr_device <= 0)
   {
      if (WPL_IS_ERROR(WPLI_WinipcDeviceOpen()))
      {
         return WPL_ERR(WPL_ERR_PLATFORM);
      }
   }

   info = WPL_NTR_CLNT_INFO(i_client_id);
   if (ioctl(wpl_ntr_device, WPL_IOCTL_CLNT_REGISTER, info) < 0)
   {
      perror("PLATFORM: ioctl to winipc failed");
      return WPL_ERR(WPL_ERR_PLATFORM);
   }

   return WPL_OK;
}

WP_U32 WPL_WinipcSignalRaise(WP_U32 i_to_client, WP_boolean i_user_event)
{
   WP_U32 info;

   if (wpl_ntr_device <= 0)
   {
      if (WPL_IS_ERROR(WPLI_WinipcDeviceOpen()))
      {
         return WPL_ERR(WPL_ERR_PLATFORM);
      }
   }

   info = WPL_NTR_CLNT_INFO(i_to_client);
   if (ioctl(wpl_ntr_device, (i_user_event ? WPL_IOCTL_RAISE_USER_EVENT : WPL_IOCTL_RAISE_REQ_EVENT), info) < 0)
   {
      perror("PLATFORM: ioctl to winipc failed");
      return WPL_ERR(WPL_ERR_PLATFORM);
   }
 
   return WPL_OK;
}

/******************************************************************/
/*                       Static functions                         */
/******************************************************************/

static WP_U32 WPLI_WinipcDeviceOpen(void)
{
   if ((wpl_ntr_device = open("/dev/wp_winipc", WPL_WINIPC_OPEN_FLAG)) < 0)
   {
      perror("PLATFORM: could not open the winipc driver");
      return WPL_ERR(WPL_ERR_PLATFORM);
   }

   return WPL_OK;
}

static WP_S32 WPLI_WinipcUserIntrHandler(void)
{
   if (signal_handler[1])
   {
      signal_handler[1](WPL_NTR_SIGNAL_USR);
   }

   return WPL_OK;
}

static WP_S32 WPLI_WinipcReqIntrHandler(void)
{
   if (signal_handler[0])
   {
      signal_handler[0](WPL_NTR_SIGNAL_REQ);
   }

   return WPL_OK;
}
