/*****************************************************************************
 * (C) Copyright 2000-2009, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: platform/mips_linux_winpath3/sources/wpl_interrupts.c
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

#include <sys/ioctl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>

#include <wpl_platform.h>
#include <wpli_platform.h>

/******************************************************************/
/*                       Typedefs and defines                     */
/******************************************************************/

#define WPL_SIGNAL_RT_SER 35 /*SIGRTMIN*/
#define WPL_SIGNAL_RT_OV (WPL_SIGNAL_RT_SER + 1)
#define WPL_SIGNAL_RT_BUSERR (WPL_SIGNAL_RT_SER + 2)

/******************************************************************/
/*                       Structures + Enumerators                 */
/******************************************************************/

static struct sigaction siginfo;

/******************************************************************/
/*                       Global Database                          */
/******************************************************************/

extern WP_S32 fd[WPL_NUM_BANKS];

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

void WPL_service_callback_handler(WP_S32 signum, siginfo_t* info, void *ptr);
void WPL_overrun_callback_handler(WP_S32 signum, siginfo_t* info, void *ptr);
void WPL_bus_error_callback_handler(WP_S32 signum, siginfo_t* info, void *ptr);


/******************************************************************/
/*                       API Functions                            */
/******************************************************************/

WP_U32 WPL_IntDisable(WP_U32 wpid, WPL_IntVector vector)
{
   switch (vector)
   {
   case WPL_IntServiceQueues:
   {
      break;
   }
   case WPL_IntOverrunQueues:
   {
      sigaction(WPL_SIGNAL_RT_OV, NULL, &siginfo);
      siginfo.sa_handler = SIG_DFL;
      siginfo.sa_flags = 0;
      if(sigaction(WPL_SIGNAL_RT_OV, &siginfo, NULL)==-1) {
         WPLI_LOG(("ERROR!!! WPL_IntDisable for Linux failed\n"));
      }

      break;
   }
   case WPL_BusError:
   {
      WP_U32 value = 0;
      ioctl(fd[WP_BUS_INTERNAL], WPL_IOCTL_ENABLE_BUS_ERR_SIG, &value);

      break;
   }
   case WPL_IntGlobalState:
   {
      sigaction(WPL_SIGNAL_RT_SER, NULL, &siginfo);
      siginfo.sa_handler = SIG_DFL;
      siginfo.sa_flags = 0;
      if(sigaction(WPL_SIGNAL_RT_SER, &siginfo, NULL)==-1) {
         WPLI_LOG(("ERROR!!! WPL_IntDisable for Linux failed\n"));
      }

      break;
   }
   default:
      return WPL_ERR(WPL_ERR_INT_NOT_SUPPORTED);
   }

   return WPL_OK;
}

WP_U32 WPL_IntEnable(WP_U32 wpid, WPL_IntVector vector)
{
   switch (vector)
   {
   case WPL_IntServiceQueues:
   {
      break;
   }
   case WPL_IntOverrunQueues:
   {
      sigaction(WPL_SIGNAL_RT_OV, NULL, &siginfo);
      siginfo.sa_sigaction = WPL_service_callback_handler;
      sigemptyset(&siginfo.sa_mask);
      siginfo.sa_flags = SA_SIGINFO;

      if (sigaction(WPL_SIGNAL_RT_OV, &siginfo, NULL) == -1) {
         WPLI_LOG(("ERROR!!! WPL_IntEnable for Linux failed\n"));
      }

      break;
   }
   case WPL_IntGlobalState:
   {
      sigaction(WPL_SIGNAL_RT_SER, NULL, &siginfo);
      siginfo.sa_sigaction = WPL_service_callback_handler;
      sigemptyset(&siginfo.sa_mask);
      siginfo.sa_flags = SA_SIGINFO;
      if (sigaction(WPL_SIGNAL_RT_SER, &siginfo, NULL) == -1) {
         WPLI_LOG(("ERROR!!! WPL_IntEnable for Linux failed\n"));
      }

      break;
   }
   default:
      return WPL_ERR(WPL_ERR_INT_NOT_SUPPORTED);
   }

   return WPL_OK;
}

void WPL_service_callback_handler(WP_S32 signum, siginfo_t* info, void *ptr)
{
   if (wddi_callback_ptrs[WPL_IntServiceQueues] != NULL)
   {
      wddi_callback_ptrs[WPL_IntServiceQueues](WPL_WINPATH(0), info->si_int);
   }
}

void WPL_overrun_callback_handler(WP_S32 signum, siginfo_t* info, void *ptr)
{
   if (wddi_callback_ptrs[WPL_IntOverrunQueues] != NULL)
   {
      wddi_callback_ptrs[WPL_IntOverrunQueues](WPL_WINPATH(0), info->si_int);
   }
}

void WPL_bus_error_callback_handler(WP_S32 signum, siginfo_t* info, void *ptr)
{
   if (wddi_callback_ptrs[WPL_BusError] != NULL)
   {
      wddi_callback_ptrs[WPL_BusError](WPL_WINPATH(0), info->si_int);
   }
}

void WPL_IntConnect(WP_U32 wpid, WPL_IntVector vector, WPL_IntHandler *function, WPL_IntHandlerCallback *wddi_callback)
{
   switch (vector)
   {
   case WPL_IntServiceQueues:
   {
      wddi_callback_ptrs[vector] = wddi_callback;
      break;
   }
   case WPL_IntOverrunQueues:
   {
      wddi_callback_ptrs[vector] = wddi_callback;
      break;
   }
   case WPL_BusError:
   {
      wddi_callback_ptrs[vector] = wddi_callback;
      break;
   }
   default:
      break;
   }
}

WP_U32 WPLI_IntConnect(void)
{
   /* Registering the signal handlers: this needs to be done before we open the device
    * so that the application is ready when the device starts interrupting us */
   sigaction(WPL_SIGNAL_RT_SER, NULL, &siginfo);
   siginfo.sa_sigaction = WPL_service_callback_handler;
   sigemptyset(&siginfo.sa_mask);
   siginfo.sa_flags = SA_SIGINFO;

   if (sigaction(WPL_SIGNAL_RT_SER, &siginfo, NULL) == -1){
      printf("ERROR\n");
      return WPL_ERR(WPL_ERR_GNU_LINUX_APPLICATION_FAILURE);
   }

   sigaction(WPL_SIGNAL_RT_OV, NULL, &siginfo);
   siginfo.sa_sigaction = WPL_overrun_callback_handler;
   sigemptyset(&siginfo.sa_mask);
   siginfo.sa_flags = SA_SIGINFO;

   if (sigaction(WPL_SIGNAL_RT_OV, &siginfo, NULL) == -1){
      printf("ERROR\n");
      return WPL_ERR(WPL_ERR_GNU_LINUX_APPLICATION_FAILURE);
   }

   sigaction(WPL_SIGNAL_RT_BUSERR, NULL, &siginfo);
   siginfo.sa_sigaction = WPL_bus_error_callback_handler;
   sigemptyset(&siginfo.sa_mask);
   siginfo.sa_flags = SA_SIGINFO;

   if (sigaction(WPL_SIGNAL_RT_BUSERR, &siginfo, NULL) == -1){
      printf("ERROR\n");
      return WPL_ERR(WPL_ERR_GNU_LINUX_APPLICATION_FAILURE);
   }

   return WPL_OK;
}
