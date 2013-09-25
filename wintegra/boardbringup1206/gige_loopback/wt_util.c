#ifndef WT_UTIL_C
#define WT_UTIL_C
/*****************************************************************************
 * (C) Copyright 2000-2007, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/


#include <stdio.h>
#include <string.h>
#include <wp_wddi.h>

#ifndef _WT_UTIL
#define _WT_UTIL 1


#ifndef WT_CONFIG_H
#define WT_CONFIG_H

#if 0
#pragma commit_check OK 8
#endif
#define _WT_MODULE_TEST_COMMENT_ENABLE_ 0
#define _WT_MODULE_TEST_ENABLE_ 1

#endif /* WT_CONFIG_H */
#include "wp_wddi.h"
#include "wpx_app_data.h"
#include "wpx_board_if.h"

#define STRINGIFY(x) STRINGIFY_(x)
#define STRINGIFY_(x) #x

/* Functions provided by wt_util.c.  */

void WT_Identify(void);
void WT_TranslateAsciiToHex(WP_CHAR *Hex,WP_CHAR *Ascii,WP_U32 length);
void WT_PassAndTerminate(void);
void WT_FailAndTerminate(void);
void WT_PassAndContinue(void);
void WT_FailAndContinue(void);
void WT_TerminateOnError(WP_handle handle, WP_CHAR *s);
WP_U32 WT_TdmBoardFrequency(void);
#define WT_MAX(a,b) ((a) < (b) ? (a) : (b))

#define WTI_MAX(a,b) ((a) > (b) ? (a) : (b))
#define WTI_MIN(a,b) ((a) < (b) ? (a) : (b))

#define WPI_TARGET_MIPS_WEB_VXWORKS      1
#define WPI_TARGET_MIPS_WEB_WINMON       2
#define WPI_TARGET_MIPS_WINMON           2
#define WPI_TARGET_MIPS_WINMON_WINPATH3  2
#define WPI_TARGET_MIPS_WEB_WINMON_GDB   3
#define WPI_TARGET_PPC_WEB_VXWORKS       4
#define WPI_TARGET_PPC_WEB_WINMON        5
#define WPI_TARGET_PPC_WINMON            5
#define WPI_TARGET_VERILOG               6
#define WPI_TARGET_WINSIM                7
#define WPI_TARGET_MIPS_WEB_VXWORKS_DUAL 8
#define WPI_TARGET_MIPS_WEB_WINMON_DUAL  9
#define WPI_TARGET_PPC_WEB_VXWORKS_DUAL  10
#define WPI_TARGET_MIPS_WEB_LINUX        11
#define WPI_TARGET_MIPS_WEB_KMODULE      12
#define WPI_TARGET_MIPS_WEB2_WINMON      13
#define WPI_TARGET_MIPS_WEB2_VXWORKS     14
#define WPI_TARGET_MIPS_LINUX            15
#define WPI_TARGET_PPC_WEB2_VXWORKS      16
#define WPI_TARGET_PPC_LINUX             17
#define WPI_TARGET_MIPS_WINMON_AUTO_MEMMAP  18
#define WPI_TARGET_MIPS_LINUX_AUTO_MEMMAP   19

#define WT_LOG(x,y)

/* protottype for RTDI GPIO configuration */
WP_status WT_GpioCommit(void);
WP_status WT_ItdmGpioCommit(void);

/* prototype for checking port existence */
WP_boolean WTI_CheckForPortPresent(WP_U32 portId);

#define PRINT_STAT_FIELD( field) \
        printf( "%08X%08X", ( sizeof(field)==sizeof(WP_U64))?(WP_U32)(field>>32):0, (WP_U32)field)

/* BRG of each UPI */
#ifdef WP_HW_WINPATH1
#define WT_BRG_UPI1 WP_BRG1
#define WT_BRG_UPI2 WP_BRG3
#endif
#ifdef WP_HW_WINPATH2
#define WT_BRG_UPI1 WP_BRG3
#define WT_BRG_UPI2 WP_BRG1
#endif
#ifdef WP_HW_WINPATH3
#define WT_BRG_UPI1 WP_BRG3
#define WT_BRG_UPI2 WP_BRG1
#endif

#endif
#ifdef WP_HW_WINPATH1
#include "wpx_tdm_qfalc.h"
#endif

#define WT_DEBUG_CALLBACK 0

WP_status WT_BspMallocDone(void);


void WT_TerminateOnError(WP_handle handle, WP_CHAR *s)
{
   WT_LOG(WT_DEBUG_LEVEL_3,(wt_log,"WT_TerminateOnError(%s)\n", s));
   if (WP_ERROR_P(handle)) {
      printf("Test Abort %s %s 0x%x\n", s, WP_ErrorString(handle), handle);
      WT_FailAndTerminate();
   }
}

void WT_PassAndContinue(void)
{
   printf("Test Passed\n");
}

void WT_FailAndContinue(void)
{
   printf("Test Failed\n");
}

void WT_PassAndTerminate(void)
{
   printf("Test Passed\n");
   WP_DriverRelease();
#if USE_SOCKET
   CleanUpSocket(5);
#endif
   exit(0);
}

void WT_FailAndTerminate(void)
{
   printf("Test Failed\n");
   WP_DriverRelease();
#if USE_SOCKET
   CleanUpSocket(5);
#endif
   exit(1);
}

void WT_TranslateAsciiToHex(WP_CHAR *Hex, WP_CHAR *Ascii,
                            WP_U32 length)
{
   WP_S32 i, first = 0, second = 0;

   for (i=0;i<length;i++) {
      if (Ascii[2*i] >= '0' && Ascii[2*i] <= '9')
         first = Ascii[2*i] - '0';
      if (Ascii[2*i] >= 'a' && Ascii[2*i] <= 'f')
         first = Ascii[2*i] - 'a' + 10;
      if (Ascii[2*i+1] >= '0' && Ascii[2*i+1] <= '9')
         second = Ascii[2*i+1] - '0';
      if (Ascii[2*i+1] >= 'a' && Ascii[2*i+1] <= 'f')
         second = Ascii[2*i+1] - 'a' + 10;

      Hex[i] = (WP_CHAR)(first * 16 + second);
   }
   return;
}

void WT_Identify(void)
{
   WP_U32 p, w;
   WP_CHAR *first_dps_found = NULL;
   WP_U32 found_error = 0;
   printf("Test Identity Version %s\n", WP_Identity.wddi_version);
   printf("Test Identity Build %s\n", WP_Identity.wddi_build);
   for (p = 0; p < WP_MAX_WINPATHS; p++) {
      for (w = 0; w < WP_WINFARMS; w++) {
         if (!WP_Identity.dps_in_wddi[p][w] ||
             strlen(WP_Identity.dps_in_wddi[p][w]) == 0)
            continue;
         printf("Test Identity DPS WP%d WF%d in wddi %s in use %s\n", p, w,
                WP_Identity.dps_in_wddi[p][w],
                WP_Identity.dps_in_use[p][w]);
         if (!first_dps_found)
            first_dps_found = WP_Identity.dps_in_wddi[p][w];
         if (strcmp(WP_Identity.dps_in_wddi[p][w], first_dps_found) ||
            strcmp(WP_Identity.dps_in_use[p][w], first_dps_found))
         found_error = 1;
      }
   }
   if (found_error) {
      printf("Test Abort DPS not uniform\n");
      WT_FailAndTerminate();
   }
}



/* Return TDM board frequency value    */
WP_U32 WT_TdmBoardFrequency(void)
{
   return 0;
}

struct wti_gpio {
      WP_U32 gpcr_a;
      WP_U32 gpdr_a;
      WP_U32 gpvr_a;
      WP_U32 pad_a;
      WP_U32 gpcr_b;
      WP_U32 gpdr_b;
      WP_U32 gpvr_b;
      WP_U32 pad_b;
      WP_U32 gpcr_c;
      WP_U32 gpdr_c;
      WP_U32 gpvr_c;
      WP_U32 pad_c;
      WP_U32 gpcr_d;
      WP_U32 gpdr_d;
      WP_U32 gpvr_d;
      WP_U32 pad_d;
      WP_U32 gpcr_e;
      WP_U32 gpdr_e;
      WP_U32 gpvr_e;
      WP_U32 pad_e;
      WP_U32 gpcr_f;
      WP_U32 gpdr_f;
      WP_U32 gpvr_f;
      WP_U32 pad_f;
      WP_U32 gpcr_g;
      WP_U32 gpdr_g;
      WP_U32 gpvr_g;
      WP_U32 pad_g;
      WP_U32 gpcmr_a;
      WP_U32 pad2_a;
      WP_U32 gpcmr_b;
      WP_U32 pad2_b;
      WP_U32 gpcmr_c;
      WP_U32 pad2_c;
      WP_U32 gpcmr_d;
      WP_U32 pad2_d;
      WP_U32 gpcmr_e;
      WP_U32 pad2_e;
      WP_U32 gpcmr_f;
      WP_U32 pad2_f;
      WP_U32 gpcmr_g;
      WP_U32 pad2_g;
}wti_gpio;

#define WPI_GPIO(base) ((struct wti_gpio *) ((base) + 0x10c80))

WP_status WT_GpioCommit(void)
{
   struct wti_gpio *gpreg;

   gpreg = WPI_GPIO(WPL_PHY_VIRTUAL(0, 0x1f000000));
   *((WP_U32 *)&gpreg->gpcr_a)=0xfffc0f03;
   *((WP_U32 *)&gpreg->gpcmr_a)=0x0003f0fc;
   *((WP_U32 *)&gpreg->gpcr_b)=0x00030003;
   *((WP_U32 *)&gpreg->gpcmr_b)=0xfffcfffc;
   *((WP_U32 *)&gpreg->gpcr_c)=0xffffffff;
   *((WP_U32 *)&gpreg->gpcmr_c)=0;
   *((WP_U32 *)&gpreg->gpcr_d)=0xffffffff;
   *((WP_U32 *)&gpreg->gpcmr_d)=0;
   *((WP_U32 *)&gpreg->gpcr_e)=0xffffffff;
   *((WP_U32 *)&gpreg->gpcmr_e)=0;
   *((WP_U32 *)&gpreg->gpcr_f)=0xffffffff;
   *((WP_U32 *)&gpreg->gpcmr_f)=0;
   *((WP_U32 *)&gpreg->gpcr_g)=0xffffff00;
   *((WP_U32 *)&gpreg->gpcmr_g)=0x000000ff;

   return WP_OK;
}

WP_status WT_ItdmGpioCommit(void)
{
   struct wti_gpio *gpreg;

   gpreg = WPI_GPIO(WPL_PHY_VIRTUAL(0, 0x1f000000));
   *((WP_U32 *)&gpreg->gpcr_a)=0xfffc0f03;
   *((WP_U32 *)&gpreg->gpcmr_a)=0x0003f0fc;
   *((WP_U32 *)&gpreg->gpcr_b)=0x00030003;
   *((WP_U32 *)&gpreg->gpcmr_b)=0xfffcfffc;
   *((WP_U32 *)&gpreg->gpcr_c)=0xffffffff;
   *((WP_U32 *)&gpreg->gpcmr_c)=0;
   *((WP_U32 *)&gpreg->gpcr_d)=0xffffffff;
   *((WP_U32 *)&gpreg->gpcmr_d)=0;
   *((WP_U32 *)&gpreg->gpcr_e)=0xffffffff;
   *((WP_U32 *)&gpreg->gpcmr_e)=0;
   *((WP_U32 *)&gpreg->gpcr_f)=0xffffffff;
   *((WP_U32 *)&gpreg->gpcmr_f)=0x00010000;
   *((WP_U32 *)&gpreg->gpcr_g)=0xffffff00;
   *((WP_U32 *)&gpreg->gpcmr_g)=0x400F3CFF;

   return WP_OK;
}


#endif
