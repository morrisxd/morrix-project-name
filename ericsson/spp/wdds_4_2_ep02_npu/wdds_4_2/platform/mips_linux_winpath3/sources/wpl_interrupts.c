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

#include <wpl_mem_access.h>
#include <wpl_platform.h>
#include <wpli_platform.h>

/******************************************************************/
/*                       Typedefs and defines                     */
/******************************************************************/

#define WPL_SIGNAL_RT_SER               35 /*SIGRTMIN+3*/
#define WPL_SIGNAL_RT_OV       (WPL_SIGNAL_RT_SER + 1)
#define WPL_SIGNAL_RT_BUSERR   (WPL_SIGNAL_RT_SER + 2)
#define WPL_SIGNAL_RT_SGMII_AN          45 /* HAS TO BE 45 */
#define WPL_SIGNAL_RT_SERDES            44
#define WPL_SIGNAL_RT_UFE4_FRAMER       46

#define WPL_EINT3_INTRPT                30

#define WPL_INTERRUPT_HIGH_REG_OFFSET 32
#define WPL_INTRPT_SRC_MIPS0  0
#define WPL_INTRPT_SRC_MIPS1  1
#define WPL_INTRPT_MASK_MIPS0_L  (wpl_rif_base + 0xc288)
#define WPL_INTRPT_MASK_MIPS0_H  (wpl_rif_base + 0xc28c)
#define WPL_INTRPT_MASK_MIPS1_L  (wpl_rif_base + 0xc29c)
#define WPL_INTRPT_MASK_MIPS1_H  (wpl_rif_base + 0xc2a0)
#define WPL_INTRPT_REGISTER_GET(val, reg) WP_MEM_GET((val), *((WP_U32*)reg))
#define WPL_INTRPT_REGISTER_SET(reg, val) WP_MEM_SET(*((WP_U32*)reg), (val))

static WP_U32        wpl_rif_base = 0;

/******************************************************************/
/*                       Structures + Enumerators                 */
/******************************************************************/

static struct sigaction siginfo;

/******************************************************************/
/*                       Global Database                          */
/******************************************************************/

extern WP_S32 fd[WPL_NUM_BANKS];
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

void WPL_service_callback_handler(WP_S32 signum, siginfo_t *info, void *ptr);
void WPL_overrun_callback_handler(WP_S32 signum, siginfo_t *info, void *ptr);
void WPL_bus_error_callback_handler(WP_S32 signum, siginfo_t *info, void *ptr);
void WPL_sgmii_an_callback_handler(WP_S32 signum, siginfo_t *info, void *ptr);
void WPL_eint3_ufe4_callback_handler(WP_S32 signum, siginfo_t *info, void *ptr);
void WPL_serdes_interrupt_callback_handler(WP_S32 signum, siginfo_t* info, void* ptr);

/******************************************************************/
/*                       API Functions                            */
/******************************************************************/

WP_U32 WPL_IntDisable(WP_U32 wpid, WPL_IntVector vector)
{
   /* printf("WPL_IntDisable wpid %d vector %d\n", wpid, vector); */
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
   case WPL_SgmiiAn:
   {

      sigaction(WPL_SIGNAL_RT_SGMII_AN, NULL, &siginfo);
      siginfo.sa_handler = SIG_DFL;
      siginfo.sa_flags = 0;
      if(sigaction(WPL_SIGNAL_RT_SGMII_AN, &siginfo, NULL)==-1) {
         WPLI_LOG(("ERROR!!! WPL_IntDisable for Linux failed\n"));
      }

      break;
   }
   case WPL_Eint3Ufe4:
   {
      sigaction(WPL_SIGNAL_RT_UFE4_FRAMER, NULL, &siginfo);
      siginfo.sa_handler = SIG_DFL;
      siginfo.sa_flags = 0;
      if(sigaction(WPL_SIGNAL_RT_UFE4_FRAMER, &siginfo, NULL)==-1) {
         WPLI_LOG(("ERROR!!! WPL_IntDisable for Linux failed\n"));
      }

      break;
   }
   case WPL_SerdesInterrupt:
   {
      sigaction(WPL_SIGNAL_RT_SERDES, NULL, &siginfo);
      siginfo.sa_handler = SIG_DFL;
      siginfo.sa_flags = 0;
      if(sigaction(WPL_SIGNAL_RT_SERDES, &siginfo, NULL)==-1) {
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
   case WPL_BusError:
   case WPL_SgmiiAn:
   case WPL_SerdesInterrupt:
   {
      break;
   }
   case WPL_IntOverrunQueues:
   {
      sigaction(WPL_SIGNAL_RT_OV, NULL, &siginfo);
      siginfo.sa_sigaction = (void (*)(int, siginfo_t *, void *))WPL_service_callback_handler;
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
      siginfo.sa_sigaction = (void (*)(int, siginfo_t *, void *))WPL_service_callback_handler;
      sigemptyset(&siginfo.sa_mask);
      siginfo.sa_flags = SA_SIGINFO;
      if (sigaction(WPL_SIGNAL_RT_SER, &siginfo, NULL) == -1) {
         WPLI_LOG(("ERROR!!! WPL_IntEnable for Linux failed\n"));
      }

      break;
   }
   case WPL_Eint3Ufe4:
   {
      sigaction(WPL_SIGNAL_RT_UFE4_FRAMER, NULL, &siginfo);
      siginfo.sa_sigaction = (void (*)(int, siginfo_t *, void *))WPL_eint3_ufe4_callback_handler;
      sigemptyset(&siginfo.sa_mask);
      siginfo.sa_flags = SA_SIGINFO;
      if (sigaction(WPL_SIGNAL_RT_UFE4_FRAMER, &siginfo, NULL) == -1) {
         WPLI_LOG(("ERROR!!! WPL_IntEnable for Linux failed\n"));
      }

      break;
   }
   default:
      return WPL_ERR(WPL_ERR_INT_NOT_SUPPORTED);
   }

   return WPL_OK;
}

void WPL_service_callback_handler(WP_S32 signum, siginfo_t *info, void *ptr)
{
   WP_U32 wpid = WPL_WINPATH(0);
   
   if (wddi_callback_ptrs[WPL_IntServiceQueues] != NULL)
   {
      /*Check the wpid using the */
      if(wds_bsp_ver >= WPL_BSP_MULTI_WINPATH_VERSION)
      {
         wpid = info->si_int & WPLI_WPID_INDICATOR_INTERRUPT_MASK;
         wpid >>= WPLI_WPID_INDICATOR_INTERRUPT_OFFSET;
      }
      /*remove the wpid from info->si_int */
      info->si_int &= ~WPLI_WPID_INDICATOR_INTERRUPT_MASK;
      wddi_callback_ptrs[WPL_IntServiceQueues](wpid, info->si_int);
   }
}

void WPL_overrun_callback_handler(WP_S32 signum, siginfo_t *info, void *ptr)
{
   WP_U32 wpid = WPL_WINPATH(0);

   if (wddi_callback_ptrs[WPL_IntOverrunQueues] != NULL)
   {
      /*Check the wpid using the */
      if(wds_bsp_ver >= WPL_BSP_MULTI_WINPATH_VERSION)
      {
         wpid = info->si_int & WPLI_WPID_INDICATOR_INTERRUPT_MASK;
         wpid >>= WPLI_WPID_INDICATOR_INTERRUPT_OFFSET;
      }
      /*remove the wpid from info->si_int */
      info->si_int &= ~WPLI_WPID_INDICATOR_INTERRUPT_MASK;
      wddi_callback_ptrs[WPL_IntOverrunQueues](wpid, info->si_int);
   }
}

void WPL_bus_error_callback_handler(WP_S32 signum, siginfo_t* info, void* ptr)
{
   WP_U32 wpid = WPL_WINPATH(0);

   if (wddi_callback_ptrs[WPL_BusError] != NULL)
   {
      if(wds_bsp_ver >= WPL_BSP_MULTI_WINPATH_VERSION)
      {
         wpid = info->si_int & WPLI_WPID_INDICATOR_INTERRUPT_MASK;
         wpid >>= WPLI_WPID_INDICATOR_INTERRUPT_OFFSET;
      }
      wddi_callback_ptrs[WPL_BusError](wpid, info->si_int);
   }
}

void WPL_eint3_ufe4_callback_handler(WP_S32 signum, siginfo_t *info, void *ptr)
{
   WP_U16 core;
   WP_U32 intrpt_mask;
   WP_U32 wpid = WPL_WINPATH(0);

   if(wds_bsp_ver >= WPL_BSP_MULTI_WINPATH_VERSION)
   {
      wpid = info->si_int & WPLI_WPID_INDICATOR_INTERRUPT_MASK;
      wpid >>= WPLI_WPID_INDICATOR_INTERRUPT_OFFSET;
   }

   if (wpl_rif_base == 0)
   {
      WPLI_BusesRifBaseGet(wpid,&wpl_rif_base);
   }

   if (wddi_callback_ptrs[WPL_Eint3Ufe4] != NULL)
   {
      wddi_callback_ptrs[WPL_Eint3Ufe4](wpid, info->si_int);
   }

   core = (info->si_int >> 16) & 0xffff;
   
   if (core == WPL_INTRPT_SRC_MIPS0)
   {
      WPL_INTRPT_REGISTER_GET(intrpt_mask, WPL_INTRPT_MASK_MIPS0_L);
      intrpt_mask |= (1 << WPL_EINT3_INTRPT);
      WPL_INTRPT_REGISTER_SET(WPL_INTRPT_MASK_MIPS0_L, intrpt_mask);
   }
   else
   {
      WPL_INTRPT_REGISTER_GET(intrpt_mask, WPL_INTRPT_MASK_MIPS1_L);
      intrpt_mask |= (1 << WPL_EINT3_INTRPT);
      WPL_INTRPT_REGISTER_SET(WPL_INTRPT_MASK_MIPS1_L, intrpt_mask);
   }
}

void WPL_sgmii_an_callback_handler(WP_S32 signum, siginfo_t* info, void* ptr)
{
   WP_U32 intrpt_mask;
   
   if (wddi_callback_ptrs[WPL_SgmiiAn] != NULL)
   {
      wddi_callback_ptrs[WPL_SgmiiAn](WPL_WINPATH(0), info->si_int);
   }
   if (wpl_rif_base == 0)
   {
      WP_U32 wpid = WPL_WINPATH(0);
      if(wds_bsp_ver >= WPL_BSP_MULTI_WINPATH_VERSION)
      {
         /*Check the wpid using the */
         wpid = info->si_int & WPLI_WPID_INDICATOR_INTERRUPT_MASK;
         wpid >>= WPLI_WPID_INDICATOR_INTERRUPT_OFFSET;
      }
      WPLI_BusesRifBaseGet(wpid, &wpl_rif_base);
   }
   if(info->si_int == WPL_INTRPT_SRC_MIPS0)
   {
    
      WPL_INTRPT_REGISTER_GET(intrpt_mask, WPL_INTRPT_MASK_MIPS0_H);
      intrpt_mask |= 1<<(WPL_SIGNAL_RT_SGMII_AN - WPL_INTERRUPT_HIGH_REG_OFFSET);
      WPL_INTRPT_REGISTER_SET(WPL_INTRPT_MASK_MIPS0_H, intrpt_mask);
   }
   else
   {
      WPL_INTRPT_REGISTER_GET(intrpt_mask, WPL_INTRPT_MASK_MIPS1_H);
      intrpt_mask |= 1<<(WPL_SIGNAL_RT_SGMII_AN - WPL_INTERRUPT_HIGH_REG_OFFSET);
      WPL_INTRPT_REGISTER_SET(WPL_INTRPT_MASK_MIPS1_H, intrpt_mask);
    }

}

void WPL_serdes_interrupt_callback_handler(WP_S32 signum, siginfo_t* info, void* ptr)
{
   WP_U32 intrpt_mask;
   
   if (wddi_callback_ptrs[WPL_SerdesInterrupt] != NULL)
   {
      wddi_callback_ptrs[WPL_SerdesInterrupt](WPL_WINPATH(0), info->si_int);
   }
   if (wpl_rif_base == 0)
   {
      WP_U32 wpid = WPL_WINPATH(0);
      if(wds_bsp_ver >= WPL_BSP_MULTI_WINPATH_VERSION)
      {
         /*Check the wpid using the */
         wpid = info->si_int & WPLI_WPID_INDICATOR_INTERRUPT_MASK;
         wpid >>= WPLI_WPID_INDICATOR_INTERRUPT_OFFSET;
      }
      WPLI_BusesRifBaseGet(wpid, &wpl_rif_base);
   }
   if(info->si_int == WPL_INTRPT_SRC_MIPS0)
   {
    
      WPL_INTRPT_REGISTER_GET(intrpt_mask, WPL_INTRPT_MASK_MIPS0_H);
      intrpt_mask |= 1<<(WPL_SIGNAL_RT_SERDES - WPL_INTERRUPT_HIGH_REG_OFFSET);
      WPL_INTRPT_REGISTER_SET(WPL_INTRPT_MASK_MIPS0_H, intrpt_mask);
   }
   else
   {
      WPL_INTRPT_REGISTER_GET(intrpt_mask, WPL_INTRPT_MASK_MIPS1_H);
      intrpt_mask |= 1<<(WPL_SIGNAL_RT_SERDES - WPL_INTERRUPT_HIGH_REG_OFFSET);
      WPL_INTRPT_REGISTER_SET(WPL_INTRPT_MASK_MIPS1_H, intrpt_mask);
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
   case WPL_SgmiiAn:
   {
      wddi_callback_ptrs[vector] = wddi_callback;
      break;
   }
   case WPL_Eint3Ufe4:
   {
      wddi_callback_ptrs[vector] = wddi_callback;
      break;
   }
   case WPL_SerdesInterrupt:
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

   sigaction(WPL_SIGNAL_RT_SGMII_AN, NULL, &siginfo);
   siginfo.sa_sigaction = WPL_sgmii_an_callback_handler;
   sigemptyset(&siginfo.sa_mask);
   siginfo.sa_flags = SA_SIGINFO;

   if (sigaction(WPL_SIGNAL_RT_SGMII_AN, &siginfo, NULL) == -1){
      printf("ERROR\n");
      return WPL_ERR(WPL_ERR_GNU_LINUX_APPLICATION_FAILURE);
   }

   sigaction(WPL_SIGNAL_RT_UFE4_FRAMER, NULL, &siginfo);
   siginfo.sa_sigaction = WPL_eint3_ufe4_callback_handler;
   sigemptyset(&siginfo.sa_mask);
   siginfo.sa_flags = SA_SIGINFO;

   if (sigaction(WPL_SIGNAL_RT_UFE4_FRAMER, &siginfo, NULL) == -1){
      printf("ERROR\n");
      return WPL_ERR(WPL_ERR_GNU_LINUX_APPLICATION_FAILURE);
   }

   sigaction(WPL_SIGNAL_RT_SERDES, NULL, &siginfo);
   siginfo.sa_sigaction = WPL_serdes_interrupt_callback_handler;
   sigemptyset(&siginfo.sa_mask);
   siginfo.sa_flags = SA_SIGINFO;

   if (sigaction(WPL_SIGNAL_RT_SERDES, &siginfo, NULL) == -1){
      printf("ERROR\n");
      return WPL_ERR(WPL_ERR_GNU_LINUX_APPLICATION_FAILURE);
   }

   return WPL_OK;
}
