/*****************************************************************************
 * (C) Copyright 2000-2007, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

#ifndef WT_UTIL_C
#define WT_UTIL_C

#if _WT_MODULE_TEST_COMMENT_ENABLE_
/* This test has been converted to work on Linux.  See
   WDDI_TestCodingGuidlines.doc.  */
#pragma check_tests pool_alloc 1
#endif

#include <stdio.h>
#include <string.h>
#include <wp_wddi.h>
#include "wt_util.h"
#ifdef WP_HW_WINPATH1
#include "wpx_tdm_qfalc.h"
#endif

#define WT_DEBUG_CALLBACK 0

WP_status WT_BspMallocDone(void);

#if _WT_MODULE_TEST_ENABLE_
/*****************************************************************************
 * NOTE:
 * The following functions may be used by many test
 * applications. Other examples include routines to add IP checksums
 * for interworking tests that generate network data for a HostSend,
 * or conversion from HEX to ASCII etc.,
 ****************************************************************************/

WP_CHAR winsim_dps_dir[512] = STRINGIFY(WINSIM_DPS_DIR);
WP_CHAR winsim_dps_image[64] = STRINGIFY(DPS_IMAGE) ".cfg.eef";

#if WT_DEBUG_CALLBACK
/* this is a callback for debugging */
static WP_status WT_DebugCallback(const WP_CHAR *file, const WP_CHAR *function,
                                   WP_S32 line, WP_S32 error_id)
{
  if (error_id)
     printf("file = %s , line = %d function = %s\n",file,line,function);
   return WP_OK;
}
#endif

void WT_Start(WP_CHAR *app_name,WP_CHAR *trial_name,WP_CHAR *file_name)
{
#if WT_DEBUG_CALLBACK
#if WP_DEBUG_ENABLE_WDDI_ERROR_LOG
   WP_ControlRegister(WP_DEBUG_CALLBACK_FILE_LINE_ERROR, WT_DebugCallback);
#endif
#endif
   printf("Test Name " STRINGIFY(WDDI_TEST) "\n");
   printf("Test Started %s", app_name);
   if (trial_name)
      printf(" %s", trial_name);
   printf("\nTest Built " __DATE__ " " __TIME__ "\n");
   printf("Test Options " STRINGIFY(TEST_OPTIONS) "\n");
   printf("Test More Options " STRINGIFY(TEST_MORE_OPTIONS) "\n");
   printf("Test Default WinPath ID %d\n\n", DEFAULT_WPID);
}

#endif /* _WT_MODULE_TEST_ENABLE_ */

void WT_TerminateOnError(WP_handle handle, WP_CHAR *s)
{
   WT_LOG(WT_DEBUG_LEVEL_3,(wt_log,"WT_TerminateOnError(%s)\n", s));
#if _WT_MODULE_TEST_ENABLE_
   WPI_SimulateInterrupts();
#endif
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
#if _WT_MODULE_TEST_ENABLE_
   WT_Reboot();
#endif /* _WT_MODULE_TEST_ENABLE_ */
   exit(0);
}

void WT_FailAndTerminate(void)
{
   printf("Test Failed\n");
   WP_DriverRelease();
#if USE_SOCKET
   CleanUpSocket(5);
#endif
#if _WT_MODULE_TEST_ENABLE_
   WT_Reboot();
#endif /* _WT_MODULE_TEST_ENABLE_ */
   exit(1);
}

#if _WT_MODULE_TEST_ENABLE_
/* This function is the same as the generic WP_Delay introduced
   in 1.0.  This should be used only for workarounds.  */
void WT_Delay(WP_U32 interval)
{
   WP_U32 start = WP_TimeRead();
   WP_U32 interim;

   while (1)
   {
       interim = WP_TimeRead();
       if (WP_TimeDelta(interim, start) >= interval)
          break;
   }
}

#endif /* _WT_MODULE_TEST_ENABLE_ */
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
#if _WT_MODULE_TEST_ENABLE_

void WT_Reboot(void)
{
#if !defined(__linux__)
/* Rebooting is specific to board and target.  The test
 * directory should not be concerned how this is done.
 * Instead, it calls a board-specific function. */
   if (WPI_REBOOT_ENABLE) {
      printf("Test Rebooting winmon by WT_Reboot\nWinMon>\n");
      WPX_Reboot();
   }
   else
      printf("Not Rebooting with WT_Reboot\nWinMon>\n");
#else
   exit(0);
#endif
}


/* The WT_BimWinmonInit function should
 * be called before WP_DriverInit to initialize
 * the TA order RAM and bus monitor for
 * BIM. It should be used only for
 * mips_web_winmon_dual target.
 */
#define WT_WINMON_BIM_TA_ORDER_RAM_BASE 0x5f001800
#define WT_WINMON_BIM_TA_ORDER_RAM_SIZE 128

void WT_BimWinmonInit(void)
{
#if WPI_TARGET_VALUE == WPI_TARGET_MIPS_WEB_WINMON_DUAL
   WP_U32 i, ta_end;

   /* init ta order */
   ta_end = WT_WINMON_BIM_TA_ORDER_RAM_BASE +
      (WT_WINMON_BIM_TA_ORDER_RAM_SIZE * 4);

   for (i=WT_WINMON_BIM_TA_ORDER_RAM_BASE; i<=ta_end; i+=4)
      *((WP_U32*)i) = 0;

   /* bus monitor enable */
   *((int*)0x5f0102f8) = 0x00010100;
   *((int*)0x5f0104f8) = 0x00010100;
   *((int*)0x5f0106f8) = 0x00010100;
   *((int*)0x5f0108f8) = 0x00010100;
#endif
}
#endif /* _WT_MODULE_TEST_ENABLE_ */

void WT_Identify(void)
{
   WP_U32 p, w;
   WP_CHAR *first_dps_found = NULL;
   WP_U32 found_error = 0;
   printf("Test Default WinPath ID %d\n\n", DEFAULT_WPID);
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

#if _WT_MODULE_TEST_ENABLE_
WP_U8 WT_MemByteGet(WP_U8 *km_data_ptr)
{
   WP_U8 tmp;
   WP_MEM_BYTE_GET(tmp, km_data_ptr[0]);
   return tmp;
}

WP_U8 *km_WT_Memchr(WP_U8 *km_data_ptr, WP_U8 val, WP_U32 len)
{
   WP_U8 tmp;
   WP_U32 i;
   for (i = 0; i < len; i++) {
      WP_MEM_BYTE_GET(tmp, km_data_ptr[i]);
      if (tmp == val)
         return km_data_ptr + i;
   }
   return NULL;
}

/* Wingine utilization Display support*/

void WT_ShowUtilization(void)
{
   WP_U32 wg_util[4];
   WP_U32 total_util;
   static WP_U8 util_buffer[UTIL_BUFFER_SAMPLES * sizeof(WP_util_sample)];
   static WP_boolean first_call = WP_TRUE;

   if (first_call)
   {
      printf("Started Analysis\n\n");
      first_call = WP_FALSE;

   }

   WP_WingineUtilization(WP_WINPATH(DEFAULT_WPID), util_buffer, UTIL_BUFFER_SAMPLES,
                         wg_util, &total_util);
   WT_DisplayWingineUtilization(wg_util, total_util);

   /* Sleep (to slow down display update) */
   WP_Delay(10);

}


void WT_DisplayWingineUtilization ( WP_U32 *wg_util, WP_U32 total_util)
{
   static WP_U32 first_print_screen;

   if (!first_print_screen)
   {
      printf("    *****************************************\n");
      printf("              Average utilization :         \n");
      printf("              ---------------------         \n");
      printf("     WG1:%02d%%  WG2:%02d%%  WG3:%02d%%  WG4:%02d%%     \n",
             wg_util[WP_WINGINE1],
             wg_util[WP_WINGINE2],
             wg_util[WP_WINGINE3],
             wg_util[WP_WINGINE4]);
      printf("                                            \n");
      printf("     Total Utilization : %d%%                \n", total_util);
      printf("                                            \n");
      printf("    ****************************************\n");
      printf("\n");

      first_print_screen = WP_TRUE;
   }
   else
   {
      WT_Vt100CrsrUp(4);
      printf("     Total Utilization : %d%%                \n", total_util);
      WT_Vt100CrsrUp(3);
      printf("     WG1:%02d%%  WG2:%02d%%  WG3:%02d%%  WG4:%02d%%     \n",
             wg_util[WP_WINGINE1],
             wg_util[WP_WINGINE2],
             wg_util[WP_WINGINE3],
             wg_util[WP_WINGINE4]);
      WT_Vt100CrsrDown(6);
   }
}

void WT_Vt100CrsrDown(WP_U32 down)
{
   printf("\033[%dB",down);
}


void WT_Vt100CrsrUp(WP_U32 up)
{
   printf("\033[%dA",up);
}
#endif /* _WT_MODULE_TEST_ENABLE_ */

#if _WT_MODULE_TEST_ENABLE_

#if 0
/*
 * Reset all attributes to default
 */
void WT_Vt100Init(void)
{
    printf("\033c");            /* Device Reset */
    printf("\033[7l");          /* Disable line wrap */
}



/*
 * Reset all attributes, such as bright, reverse, colors to normal
 */
void WT_Vt100Normal(void)
{
   printf("\033[7l");          /* Disable line wrap */
   printf("\033[0m");
}
#endif

#endif  /* _WT_MODULE_TEST_ENABLE_ */

#if _WT_MODULE_TEST_COMMENT_ENABLE_
/*
 * Originally the GpioCommit code remained in TEST mode only and was not
 * being converted to examples.  This needed to be changed for the CACS
 * IMH tests being converted to examples
 */
#endif
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

#if _WT_MODULE_TEST_COMMENT_ENABLE_
#pragma check_tests wt_util.c
#pragma check_tests common
#endif

#endif
