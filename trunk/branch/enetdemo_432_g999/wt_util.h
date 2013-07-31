#if _WT_MODULE_TEST_COMMENT_ENABLE_
/*****************************************************************************
 * (C) Copyright PMC - WIN division (Wintegra) 2011.  All rights reserved.
 * PMC-Sierra (WINTEGRA) CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of PMC-Sierra (Wintegra).
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/
#endif

#ifndef _WT_UTIL
#define _WT_UTIL 1

#include "wt_config.h"
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

#if _WT_MODULE_TEST_ENABLE_
WP_U8 WT_MemByteGet(WP_U8 *km_data_ptr);
WP_U8 *km_WT_Memchr(WP_U8 *km_data_ptr, WP_U8 val, WP_U32 len);
void WPI_SimulateInterrupts(void);
void WT_Start(char *app_name, char *trial_name, char *file_name);
void WT_Delay(WP_U32 msec);
void WT_Reboot(void);
void WT_BimWinmonInit(void);

/* This is an experiment.  A number of tests have common test
   configuration features.  When a feature applies to a test, it is
   often useful to implement it for test development or test
   debugging.  Below is the set of all such features.  */

#undef WT_USE_INTERRUPTS
/* This feature is boolean.  True means use interrupts, false
   means use polling. */

#undef WT_USE_UPI_CARD
#define WT_USE_UPI_CARD_NONE 0
#define WT_USE_UPI_CARD_OC3  1
#define WT_USE_UPI_CARD_OC12 2
#define WT_USE_UPI_CARD_UFE2 3

#undef WT_USE_TDM_CARD
/* This feature is boolean.  True means use a TDM card for external
   loopback, false means use internal loopback.  */

#undef WT_USE_HOST_IO
/* This feature is boolean.  True means use Host I/O to debug the
   test, false means don't.  */

#undef WT_USE_UPI_EXTENDED_HEADERS
/* This feature is boolean.  True means use UPI extended headers,
   false means don't.  */

#undef WT_USE_TRIAL_MENU
/* This feature is boolean.  True means use an interactive menu to
   select trials, false means don't.  */

/* These values are also encoded in wddi/tests/common/wt_util.h,
   wddi/tests/build_tests.pl, and wddi/examples/build_tests.pl. */
#endif /* _WT_MODULE_TEST_ENABLE_ */
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

#if _WT_MODULE_TEST_ENABLE_

/* Logging Functions */

char wt_log[400];
#define WT_DEBUG_LEVEL_0 0 /* no output */
#define WT_DEBUG_LEVEL_1 1 /* fatal error output */
#define WT_DEBUG_LEVEL_2 2 /* severe error output */
#define WT_DEBUG_LEVEL_3 3 /* error */
#define WT_DEBUG_LEVEL_4 4 /* progress output */
#define WT_DEBUG_LEVEL_5 5 /* verbose output */

#ifdef WT_LOG_PRINTF
#define WT_LOG(x,y) do {if ((x) <= WT_DEBUG_LEVEL) {sprintf y; printf("%s",wt_log);}} while(0)
#elif defined(WT_LOG_FILE)
#define WT_LOG(x,y) do {if ((x) <= WT_DEBUG_LEVEL) {sprintf (y); WP_AppLog(wt_log);}} while(0)
#elif defined(WT_LOG_NONE)
#define WT_LOG(x,y)
#endif

/* wingine Utilization display support*/

#define UTIL_BUFFER_SAMPLES 500
#define UTIL_BUFFER_SIZE (UTIL_BUFFER_SAMPLES * 4 * sizeof(WP_U32))

void WT_ShowUtilization(void);
void WT_DisplayWingineUtilization ( WP_U32 *wg_util, WP_U32 total_util);
void WT_Vt100CrsrDown(WP_U32 down);
void WT_Vt100CrsrUp(WP_U32 up);
#if 0
void WT_Vt100Init(void);
void WT_Vt100Normal(void);
#endif
#else
#define WT_LOG(x,y)
#endif

/* protottype for RTDI GPIO configuration */
WP_status WT_GpioCommit(void);
WP_status WT_ItdmGpioCommit(void);

/* prototype for checking port existence */
WP_boolean WTI_CheckForPortPresent(WP_U32 portId);

#define PRINT_STAT_FIELD( field) \
        printf( "%08X%08X", ( sizeof(field)==sizeof(WP_U64))?(WP_U32)(field>>32):0, (WP_U32)field)

#define PRINT_FUNC_LINE \
        printf( "function %s line %d\n", __FUNCTION__, __LINE__)

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
