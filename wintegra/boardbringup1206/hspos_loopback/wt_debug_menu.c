#ifndef _WT_DEBUG_MENU_C_
#define _WT_DEBUG_MENU_C_

/*************************************************************
 * (C) Copyright 2001-2005, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ************************************************************/

/*****************************************************************************
 *
 * Module:
 * Purpose:
 *
 * Change History:
 * +------------+--------------------+----------------------------------------
 * | Date       | By                 | Description
 * +------------+--------------------+----------------------------------------
 * |            |                    |
 * +------------+--------------------+----------------------------------------
 * |            |                    |
 * +------------+--------------------+----------------------------------------
 *
 *****************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "api/wp_wddi.h"
#include "api/wp_host_io.h"
#include "wpx_pin_mux_selector.h"
#include "wpx_app_data.h"
#include "wpx_enet_phy.h"
#include "wpi_veneer.h"
#ifdef WP_HW_WINPATH3
#pragma message ("Compiling for Winpath3")
#ifdef WP_BOARD_WDS3_SL
#pragma message ("Compiling for WDS_SL")
#endif
#include "winutil/include/winpath3/wpui_serial.h"
#include "winutil/include/winpath3/memmap.h"
#include "winutil/api/include/winpath3/wpu_util.h"
#include "winutil/include/winpath3/wpui_hw_util.h"
#endif

#ifdef WP_HW_WINPATH2
#pragma message ("Compiling for Winpath2")
#include "winutil/include/winpath2/memmap.h"
#include "winutil/include/winpath2/wpui_serial.h"
#endif

/*************************************************************
 * (C) Copyright 2001-2003, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ************************************************************/

/*****************************************************************************
 *
 * Module:
 * Purpose:
 *
 * Change History:
 * +------------+--------------------+----------------------------------------
 * | Date       | By                 | Description
 * +------------+--------------------+----------------------------------------
 * |            |                    |
 * +------------+--------------------+----------------------------------------
 * |            |                    |
 * +------------+--------------------+----------------------------------------
 *
 *****************************************************************************/

#ifndef _WT_DEBUG_MENU_H_
#define _WT_DEBUG_MENU_H_

#define USE_MULTI_THREADING_FINE_GRAIN_LOCKING_WDDI 1
#define BUFFER_SIZE 50
#define NO_OF_THREADS WINFARM_NUMBER*16

/*Menu Sizes*/
/*Sizes that differ between Winpaths*/
#ifdef WP_HW_WINPATH3 /*Winpath 3*/
  #define WU_ALL_MENU_SIZE 26
  #define PERF_MENU_SIZE 6
#else /*Winpath 2*/
  #define WU_ALL_MENU_SIZE 24
  #define PERF_MENU_SIZE 5
#endif

/*Sizes that differ between Linux and Winmon*/ 
#ifdef linux
  #define FATAL_MENU_SIZE 18
  #define TRACE_MENU_SIZE 7
 #else
  #define FATAL_MENU_SIZE 16
  #define TRACE_MENU_SIZE 6
 #endif


#ifdef WP_HW_WINPATH3
 #ifdef WP_BOARD_WDS3_SL
   #define WINFARM_NUMBER 2
 #else
   #define WINFARM_NUMBER 4
 #endif
#else
  #define WINFARM_NUMBER 2
#endif

#ifdef MENU_DEBUG_MODE
 #define STATUS TRUE
#else
 #define STATUS FALSE
#endif




void CLI_OrderRamDisplay(void);
void CLI_Display_System(void);
void menu_terminate_on_error(WP_handle handle, WP_CHAR *s,WP_U32 line);
void CLI_WinUtilBusErrCheck(WP_CHAR *StrPrm);
void CLI_WinUtilWingineBusUtilization(WP_CHAR *StrPrm);
void CLI_WinUtilThreadUtilization(WP_CHAR *StrPrm);
void CLI_WinUtilThreadArbiterDump(WP_CHAR *StrPrm);
void CLI_WinUtilSerialArbiterDump (WP_CHAR *StrPrm);
void CLI_WinUtilTraceStart (WP_CHAR *StrPrm);
void CLI_WinUtilTraceDump (WP_CHAR *StrPrm);
void CLI_DebugAreDump     (WP_CHAR *StrPrm);
void CLI_DebugVerionInfo  (WP_CHAR *StrPrm);
WP_status WTI_DebugMemoryAlloc(const WP_CHAR *file, WP_U32 line, WP_U32 bus,WP_U32 size, WP_U32 alignment, const WP_CHAR *purpose);
void CLI_WinUtilBmap  (WP_CHAR *StrPrm);
void CLI_WinUtilCoreDump  (WP_CHAR *StrPrm);
void CLI_WinUtilDmap  (WP_CHAR *StrPrm);
void CLI_WinUtilFmum (WP_CHAR *StrPrm);
void CLI_WinUtilFreq (WP_CHAR *StrPrm);
void CLI_WinUtilTraceEnable_DPS (WP_CHAR *StrPrm);
void CLI_WinUtilTraceEnable_Serial (WP_CHAR *StrPrm);
void CLI_WinUtilMemoryMap (WP_CHAR *StrPrm);
void CLI_WinUtilMemoryCheck (WP_CHAR *StrPrm);
void CLI_WinUtilMemoryDisplay (WP_CHAR *StrPrm);
void CLI_WinUtilMemoryFill (WP_CHAR *StrPrm);
void CLI_WinUtilMemoryModify (WP_CHAR *StrPrm);
void CLI_WinUtilMemoryModifyOffsetBased (WP_CHAR *StrPrm);
void CLI_WinUtilSA_TRSLimits (WP_CHAR *StrPrm);
void CLI_WinUtilTA_Bases (WP_CHAR *StrPrm);
void CLI_WinUtilDisplayTA_Table (WP_CHAR *StrPrm);
void CLI_WinUtilOrderMonitorDisplay (WP_CHAR *StrPrm);
void CLI_WinUtilTAOrderRegisters (WP_CHAR *StrPrm);
void CLI_WinUtilCurrentTRSStatus (WP_CHAR *StrPrm);
void CLI_WinUtilTraceStart (WP_CHAR *StrPrm);
void CLI_WinUtilTraceDump (WP_CHAR *StrPrm);
void CLI_WinUtilDisplayAllTraceSerials(void);
void CLI_WinUtilTRS_SerialAllocation (WP_CHAR *StrPrm);
void CLI_WDDI_Allocation(void);
void CLI_WDDI_AllocationEND(void);
void CLI_WinUtilDebugVersionInfo(WP_CHAR *StrPrm);
void CLI_WinUtilWingineUtilization (WP_CHAR *StrPrm);
void CLI_WinUtilFifoCheck (WP_CHAR *StrPrm);
void CLI_WinUtilFreeTxt (WP_CHAR *StrPrm);
void CLI_WinUtilCall(WP_CHAR* strParam, WP_CHAR *StrPrm);
void CLI_WinUtilTrsDump  (WP_CHAR *StrPrm);
void CLI_WinUtilHelp(void);
void CLI_WinUtilTraceOptionsStatus(void);
void CLI_SYSCMD_GetTRS(WP_CHAR *StrPrm);
void CLI_SYSCMD_SetTRS(WP_CHAR *StrPrm);
void CLI_SYSCMD_SmartDropConfig (WP_CHAR *StrPrm);
void CLI_SYSCMD_SetCFUTX( WP_CHAR *StrPrm);
void CLI_SYSCMD_PrintTRSserials(void);
void CLI_DisplayDTCR(void);
void CLI_DisplayTA_PCs(void);
void CLI_DisplaySA_PCT_Table(void);
void CLI_DisplayFifo_StartEnd(WP_CHAR *StrPrm);
void CLI_DisplayFifo_corruption(WP_CHAR *StrPrm);
void CLI_SetFifo_base(WP_CHAR *StrPrm);
void CLI_SetFifo_offset(WP_CHAR *StrPrm);
void CLI_DisplayTRS_AllInfo(void);
void CLI_DPS_RAM_ECCcheck(void);
void CLI_Bus_ECCcheck(void);
void CLI_WF_ReadDPSRAMPCs(void);
void CLI_DisplayFifo_CorruptedNumbers(void);
void CLI_All_VersionBuildTime(void);
WP_status WT_WddiLog(WP_S32 level, const WP_CHAR *string);
void F_menu_TermOnError (WP_handle handle, WP_CHAR *s, WP_U32 LineNum);
void CLI_SetLogLevel(WP_CHAR *StrPrm);
void CLI_APILevel_StartLog(void);
void CLI_APILevel_EndLog(void);
void CLI_InternalLevel_StartLog(void);
void CLI_InternalLevel_EndLog(void);
void CLI_WPILevel_StartLog(void);
void CLI_WPILevel_EndLog(void);
void CLI_FineGrainLocking(void);
void CLI_WDDI_VersionBuildTime(void);
void CLI_FatalError_DisplayAll(WP_CHAR *StrPrm);
void CLI_ShowAllDisplayCmds(void);
void CLI_DisplayCmd (WP_CHAR *StrPrm);
void CLI_PrintMemoryAvailable (WP_CHAR *StrPrm);
void CLI_WP_QNodeStatus(WP_CHAR *StrPrm);
void CLI_ListQnodes(void);
void CLI_TRS_Timeout(void);
void WT_ExampleDebugAreaCallback(WP_U32 wpid, WP_U32 size_allocated, WP_U32 *host_ptr);
void CLI_DebugAreDumpPartial (WP_CHAR *StrPrm);
void CLI_DebugAreDumpFull (WP_CHAR *StrPrm);
#endif

/************************************************************************/
/*                   AUTOMATION INTERFACE                               */
/************************************************************************/

#ifndef _WT_NEW_AUTOMATION_
#define _WT_NEW_AUTOMATION_

/******************************* AUTOMATION *************************/

/* Defined modes of operation */
#define MANUAL                  0
#define AUTO                    1

/* Types of menu systems */
#define REGULAR                 0
#define SPECIAL                 1
#define TTL                     2

/* Automation constants */
#define INST_NUM                256
#define MAX_TTL_FILE_SIZE       10000
#define MAX_CONTROL_STRING_SIZE 1536
#define QUEUESIZE               3
#define SERVER_PORT             9333
#define MSGMAXSIZE              2048
#define WS_PACKET_LENGTH        2049
#define HOST_NAME_LEN           50
#define INVALID_SOCKET          -1
#define SOCKET_ERROR            -1

#define MAP_GS_IC_SYIR          (WEB_WINPATH_BASE+0x10c74)
#define MAP_GS_IC_SEIR          (WEB_WINPATH_BASE+0x10c70)
#define MAP_GS_IC_SEMR0         (WEB_WINPATH_BASE+0x10c60)
#define MAP_GS_IC_SEMR1         (WEB_WINPATH_BASE+0x10c64)
#define MAP_GS_IC_SYMR0         (WEB_WINPATH_BASE+0x10c68)
#define MAP_GS_IC_SYMR1         (WEB_WINPATH_BASE+0x10c6c)

#define MAP_GS_IC_INTMODE       (WEB_WINPATH_BASE+0x10c78)

#ifdef WP_HW_WINPATH1
#define ASIX_ISR                0xbf202818
#define WEB_CPLD_LED_1          0xbf100010
#elif defined WP_HW_WINPATH2
#define WEB_CPLD_LED_1          (WEB_CPLD_SECONDARY_BASE + 0x40)


#endif
/******************************* MENU API *************************/

#ifdef OK
#undef OK
#endif

#define OK 0
#ifndef ERROR
#define ERROR -1
#endif

#ifdef TRUE
#undef TRUE
#endif

#ifdef FALSE
#undef FALSE
#endif

#define TRUE   1
#define FALSE  0
#define TRUE_M 3
#define FALSE_M 2
#define HELP 5

#define STR_STAR           "*************************************************************************"
#define STR_HOT_KEYS       "  Hot keys: !-Main Menu ^-Upper Menu =-Display Cur Menu  d-Debug Menu!    "
#define STR_ENTER_COMMAND  "# Enter your command:"
#define STR_MENU_ERR       "Err> Wrong menu command"

/* Definitions of menus structs */
typedef void (*Y_MnuLeafP) (char *);

typedef enum
{
   K_Menu,
   K_Leaf
} E_MenuNodeType;

typedef struct
{
   E_MenuNodeType    Type;
   char              Visible;
   char              *Desc;
   union
   {
      void           *Menu;
      Y_MnuLeafP     Leaf;
   }  Node;
}  Y_MenuEntry;

typedef void (*sed_int_handler_t)(void);
typedef struct netif my_netif;

extern /*const*/ Y_MenuEntry V_MainMenu[];

extern char *STR_MENU_HDR;
extern void sed_int_handler(void);
extern void sedif_input(my_netif *netif);
extern void tcpdump_init(void);
extern sed_int_handler_t sed_register_handler(sed_int_handler_t fnc);
extern void asixif_int_handler(void);
extern unsigned int imr_shadow;

/*****************************************************************************/
/*****************************************************************************/
/*                         MENU API FUNCTIONS                                */
/*****************************************************************************/
/*****************************************************************************/

void F_MenuEngine     (char * InputBuf);
int  F_EvaluateHotKey (char * InputBuf);
void F_EvaluateCommand(char * InputBuf);
void F_SetMainMenu    (void);
void F_SetDebugMenu   (void);
void F_SetUpperMenu   (void);
void F_SetMenu	      (Y_MenuEntry *Menu);
void F_DisplayMenu    (void);
void F_DisplayHeader  (void);

/*****************************************************************************/
/*****************************************************************************/
/*                         AUTOMATION FUNCTIONS                              */
/*****************************************************************************/
/*****************************************************************************/

/* Gets a string either from the stdin (manual mode) or from the automation
   global array (automation mode) */
WP_CHAR *WT_Gets( char *str);

/* Parses data on the control packet */
void EvaluateControlString(WP_U8 *InputBuf,WP_U32 demo_mode);

/* Fills automation global array of strings */
WP_U32 FillAutomationArray(WP_CHAR *InputBuf, WP_CHAR *params_string,WP_U32 demo_mode);

/* Processes incoming automation packets*/
void WT_processPacketCommand(void);

/* Translates data from ascii to hexadecimal */
void TranslateAsciiToHex(WP_U8 *Hex,WP_CHAR *Ascii,WP_U32 length);

/* Translates data from hexadecimal to ascii*/
void TranslateHexToAscii(WP_CHAR *Hex,WP_CHAR *Ascii,WP_U32 length);

#endif /* _WT_AUTOMATION_ */
#include "api/wp_wddi.h"
#include "winutil/include/wpui_dump.h"
#include "winutil/api/include/wpu_ta.h"

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
#define NO_INPUT "0"

void WPU_WinutilCall(WP_CHAR * input);
wpu_serial_info *WPUI_HwSerialMapTable(void);

extern Y_MenuEntry V_Menu_Debug[];
extern Y_MenuEntry V_Menu_WinUtil[];
extern Y_MenuEntry V_Menu_WinUtilCmd[];
extern Y_MenuEntry V_Menu_MemAndLocks[];

WP_U32 wpid_context = 0;
static WP_U32 max_log_level = 2;


typedef struct
{
      char *name;
      int  index;
} display_cmd;


const display_cmd TRSserials[] =
{
  {"BPRESSURE",WP_SA_BPRESSURE},
  {"EFM_BOND_RX",WP_SA_EFM_BOND_RX},
  {"EFM_BOND_TO",WP_SA_EFM_BOND_TO},
  {"ENET1_RX",WP_SA_ENET1_RX},
  {"ENET1_TX",WP_SA_ENET1_TX},
  {"ENET2_RX",WP_SA_ENET2_RX},
  {"ENET2_TX",WP_SA_ENET2_TX},
  {"ENET3_RX",WP_SA_ENET3_RX},
  {"ENET3_TX",WP_SA_ENET3_TX},
  {"ENET4_RX",WP_SA_ENET4_RX},
  {"ENET4_TX",WP_SA_ENET4_TX},
  {"ENET5_RX",WP_SA_ENET5_RX},
  {"ENET5_TX",WP_SA_ENET5_TX},
  {"ENET6_RX",WP_SA_ENET6_RX},
  {"ENET6_TX",WP_SA_ENET6_TX},
  {"ENET7_RX",WP_SA_ENET7_RX},
  {"ENET7_TX",WP_SA_ENET7_TX},
  {"ENET8_RX",WP_SA_ENET8_RX},
  {"ENET8_TX",WP_SA_ENET8_TX},
  {"ENET9_RX",WP_SA_ENET9_RX},
  {"ENET9_TX",WP_SA_ENET9_TX},
  {"ENET10_RX",WP_SA_ENET10_RX},
  {"ENET10_TX",WP_SA_ENET10_TX},
  {"ENET11_RX",WP_SA_ENET11_RX},
  {"ENET11_TX",WP_SA_ENET11_TX},
  {"ENET12_RX",WP_SA_ENET12_RX},
  {"ENET12_TX",WP_SA_ENET12_TX},
  {"ENET13_RX",WP_SA_ENET13_RX},
  {"ENET13_TX",WP_SA_ENET13_TX},
  {"ENET14_RX",WP_SA_ENET14_RX},
  {"ENET14_TX",WP_SA_ENET14_TX},
  {"ENET15_RX",WP_SA_ENET15_RX},
  {"ENET15_TX",WP_SA_ENET15_TX},
  {"ENET16_RX",WP_SA_ENET16_RX},
  {"ENET16_TX",WP_SA_ENET16_TX},
  {"GBOND_BG",WP_SA_GBOND_BG},
  {"GBOND_TO",WP_SA_GBOND_TO},
  {"HOST_CMD",WP_SA_HOST_CMD},
  {"IMA_RX",WP_SA_IMA_RX},
  {"IMA_TX",WP_SA_IMA_TX},
  {"ITDM_RX",WP_SA_ITDM_RX},
  {"IW_BKGND",WP_SA_IW_BKGND},
  {"IW_TX_HOST",WP_SA_IW_TX_HOST},
  {"L2_BKGND",WP_SA_L2_BKGND},
  {"MCH_RX",WP_SA_MCH_RX},
  {"MCH_TX",WP_SA_MCH_TX},
  {"MFR_RX",WP_SA_MFR_RX},
  {"MFR_TX",WP_SA_MFR_TX},
  {"MILLISECOND_TIMER",WP_SA_MILLISECOND_TIMER},
  {"MLPPP_RX",WP_SA_MLPPP_RX},
  {"OAM_FM",WP_SA_OAM_FM},
  {"POLICER_WRAP_PREV",WP_SA_POLICER_WRAP_PREV},
  {"RAS_TO",WP_SA_RAS_TO},
  {"SCHED_CW_A",WP_SA_SCHED_CW_A},
  {"SCHED_CW_B",WP_SA_SCHED_CW_B},
  {"SCHED_CW_C",WP_SA_SCHED_CW_C},
  {"SCHED_CW_D",WP_SA_SCHED_CW_D},
  {"SCHED_CW_E",WP_SA_SCHED_CW_E},
  {"SCHED_CW_F",WP_SA_SCHED_CW_F},
  {"SCHED_CW_G",WP_SA_SCHED_CW_G},
  {"SCHED_CW_H",WP_SA_SCHED_CW_H},
  {"SERIALS_N",WP_SA_SERIALS_N},
  {"SLOW_PRO_CW",WP_SA_SLOW_PRO_CW},
  {"SW_TIMESTAMP",WP_SA_SW_TIMESTAMP},
  {"TDI1_RX",WP_SA_TDI1_RX},
  {"TDI1_TX",WP_SA_TDI1_TX},
  {"TDI2_RX",WP_SA_TDI2_RX},
  {"TDI2_TX",WP_SA_TDI2_TX},
  {"TDI3_RX",WP_SA_TDI3_RX},
  {"TDI3_TX",WP_SA_TDI3_TX},
  {"TDI4_RX",WP_SA_TDI4_RX},
  {"TDI4_TX",WP_SA_TDI4_TX},
  {"TDI5_RX",WP_SA_TDI5_RX},
  {"TDI5_TX",WP_SA_TDI5_TX},
  {"TDI6_RX",WP_SA_TDI6_RX},
  {"TDI6_TX",WP_SA_TDI6_TX},
  {"TDI7_RX",WP_SA_TDI7_RX},
  {"TDI7_TX",WP_SA_TDI7_TX},
  {"TDI8_RX",WP_SA_TDI8_RX},
  {"TDI8_TX",WP_SA_TDI8_TX},
  {"TDI9_RX",WP_SA_TDI9_RX},
  {"TDI9_TX",WP_SA_TDI9_TX},
  {"TDI10_RX",WP_SA_TDI10_RX},
  {"TDI10_TX",WP_SA_TDI10_TX},
  {"TDI11_RX",WP_SA_TDI11_RX},
  {"TDI11_TX",WP_SA_TDI11_TX},
  {"TDI12_RX",WP_SA_TDI12_RX},
  {"TDI12_TX",WP_SA_TDI12_TX},
  {"TDI13_RX",WP_SA_TDI13_RX},
  {"TDI13_TX",WP_SA_TDI13_TX},
  {"TDI14_RX",WP_SA_TDI14_RX},
  {"TDI14_TX",WP_SA_TDI14_TX},
  {"TDI15_RX",WP_SA_TDI15_RX},
  {"TDI15_TX",WP_SA_TDI15_TX},
  {"TDI16_RX",WP_SA_TDI16_RX},
  {"TDI16_TX",WP_SA_TDI16_TX},
  {"TOKEN_THREAD",WP_SA_TOKEN_THREAD},
  {"TOKEN_THREAD_2",WP_SA_TOKEN_THREAD_2},
  {"UPI1_RX",WP_SA_UPI1_RX},
  {"UPI1_TX",WP_SA_UPI1_TX},
  {"UPI2_RX",WP_SA_UPI2_RX},
  {"UPI2_TX",WP_SA_UPI2_TX},
  {"UPI3_RX",WP_SA_UPI3_RX},
  {"UPI3_TX",WP_SA_UPI3_TX},
  {"UPI4_RX",WP_SA_UPI4_RX},
  {"UPI4_TX",WP_SA_UPI4_TX},
  {"WSE1_RX",WP_SA_WSE1_RX},
  {"WSE1_TX",WP_SA_WSE1_TX},
  {"WSE2_RX",WP_SA_WSE2_RX},
  {"WSE2_TX",WP_SA_WSE2_TX},
  {"XGI1_RX",WP_SA_XGI1_RX},
  {"XGI1_TX",WP_SA_XGI1_TX},
  {"XGI2_RX",WP_SA_XGI2_RX},
  {"XGI2_TX",WP_SA_XGI2_TX},
  {NULL,0},
  };


display_cmd DispCmds[] =
   {
   {"ASU",WP_DISPLAY_ASU},
   {"BIU",WP_DISPLAY_BIU},
   {"BRG",WP_DISPLAY_BRG},
   {"CHANNEL",WP_DISPLAY_CHANNEL},
   {"CID",WP_DISPLAY_CID},
   {"DEVICE",WP_DISPLAY_DEVICE},
   {"DWG",WP_DISPLAY_DWG},
   {"EMPHY_TRANS_REGISTRY",WP_DISPLAY_EMPHY_TRANS_REGISTRY},
   {"ENET",WP_DISPLAY_ENET},
   {"FMU",WP_DISPLAY_FMU},
   {"GIFA",WP_DISPLAY_GIFA},
   {"GROUP",WP_DISPLAY_CID_GROUP},
   {"HANDLE",WP_DISPLAY_HANDLE},
   {"IMA",WP_DISPLAY_IMA},
   {"INT",WP_DISPLAY_INT},
   {"IMA_LINK",WP_DISPLAY_IMA_LINK},
   {"IW_AGGREGATION",WP_DISPLAY_IW_AGGREGATION},
   {"IW_BRIDGE_PORT",WP_DISPLAY_IW_BRIDGE_PORT},
   {"IW_EXT_AGGREGATION",WP_DISPLAY_IW_EXT_AGGREGATION},
   {"IW_FLOW",WP_DISPLAY_IW_FLOW},
   {"IW_MQ",WP_DISPLAY_IW_MQ},
   {"IW_PQ",WP_DISPLAY_IW_PQ},
   {"IW_SYSTEM",WP_DISPLAY_IW_SYSTEM},
   {"IW_WRED",WP_DISPLAY_IW_WRED},
   {"MFR",WP_DISPLAY_MFR},
   {"MFR_REGISTRY",WP_DISPLAY_MFR_REGISTRY},
   {"MLPPP",WP_DISPLAY_MLPPP},
   {"OAM_FM",WP_DISPLAY_OAM_FM},
   {"PMT",WP_DISPLAY_PMT},
   {"PTP",WP_DISPLAY_PTP},
   {"PORT",WP_DISPLAY_PORT},
   {"POOL",WP_DISPLAY_POOL},
   {"PSU",WP_DISPLAY_PSU},
   {"QNODE",WP_DISPLAY_QNODE},
   {"REGISTER",WP_DISPLAY_REGISTER},
   {"REGISTRY",WP_DISPLAY_REGISTRY},
   {"SA",WP_DISPLAY_SA},
   {"SCHEDULER",WP_DISPLAY_SCHEDULER},
   {"SECURITY",WP_DISPLAY_SECURITY},
   {"SYSTEM",WP_DISPLAY_SYSTEM},
   {"TA",WP_DISPLAY_TA},
   {"TDI",WP_DISPLAY_TDI},
   {"TGU",WP_DISPLAY_TGU},
   {"UPCT",WP_DISPLAY_UPCT},
   {"UPI",WP_DISPLAY_UPI},
   {"VIRTUAL_DEVICE",WP_DISPLAY_VIRTUAL_DEVICE},
   {"VPMT",WP_DISPLAY_VPMT},
   {"WMM",WP_DISPLAY_WMM},
   {NULL,0}
   };

WP_CHAR *buildinfo[] =
{
  __DATE__,
  __TIME__
};



void menu_terminate_on_error(WP_handle handle, WP_CHAR *s,WP_U32 line)
{
  WP_CHAR *appname = "appname?";
  if (WP_ERROR_P(handle)) {
    WPU_DUMP("%s: Terminal error (handle 0x%x) %s\n","appname", handle, s);
    WPU_DUMP("%s: Error type - %s\n", appname, WP_ErrorString(handle));
    WPU_DUMP("line = %d\n",line);

    /************************************
     * It is possible to exit here -
     * Howver as it is debug menu actions
     * - it is not recommeded to exit
     *
     *
     *      WP_DriverRelease();
     * #if USE_SOCKET
     *      CleanUpSocket(5);
     *#endif
     *      WT_Reboot();
     *      exit(1);
     ******************************/
    WPU_DUMP("\n\n\n !!! ERROR  !!!  ERROR  !!!  ERROR  !!!  ERROR  !!! \n\n\n");
  }
}


/***************************************************************
 * Func name  : CLI_setWPID
 * Description: Set wpid
 * Input      : wpid #
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_setWPID(WP_CHAR *StrPrm)
{
  WP_S32 res;
  res =  sscanf(StrPrm,"%*d %d\n", &wpid_context);
  if(res < 1)
  {
    WPU_DUMP("Usage: <wpid>: If operating more than one Winpath,\
 select Winpath number for API functions\n");
    return;
  }
  WPU_DUMP("WPID set to: %d\n",wpid_context);
}

/*------------FILE WRITE FUNCTIONS (FROM WPUI_DUMP.C) ----------*/
#ifdef linux

WP_CHAR  file_name[512];
FILE* file_ptr1 = 0;

/***************************************************************
 * Func name  : SetFileName
 * Description: Set file to edit
 * Input      : filename
 * OutPut     :
 * Return Val :
 ***************************************************************/
void SetFileName(WP_CHAR * str)
{
        strcpy(file_name, str);
}

/***************************************************************
 * Func name  : WriteToFile
 * Description: Print string to file
 * Input      : Input string
 * OutPut     :
 * Return Val :
 ***************************************************************/
void WriteToFile(WP_CHAR * str)
{
        if  (file_ptr1)
        {
                fprintf(file_ptr1,"%s ",str);
        }
        else
        {
            file_ptr1 = fopen( file_name, "wb" );
            fprintf(file_ptr1,"%s ",str);
        }
}

/***************************************************************
 * Func name  : CLI_RetrieveStringDump
 * Description: Print to File
 * Input      : Input String
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_RetrieveStringDump(WP_CHAR *str)
{
  WriteToFile(str);
  return;
}


/***************************************************************
 * Func name  : CloseFile
 * Description: CloseFile
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CloseFile(void)
{
        if (file_ptr1)
        {
          fclose(file_ptr1);
          file_ptr1 = 0;
        }
}
#endif
/*------------FILE WRITE FUNCTIONS (FROM WPUI_DUMP.C) ----------*/



/*------------WINUTIL FUNCTIONS---------------------------------*/
/***************************************************************
 * Func name  : CLI_WinUtilBusErrCheck
 * Description: Calls Hardware Bus Monitor
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_WinUtilBusErrCheck(WP_CHAR *StrPrm)
{
  WP_CHAR strParam[BUFFER_SIZE] = "hbm";
  CLI_WinUtilCall(strParam, StrPrm);
}

/***************************************************************
 * Func name  : CLI_WinUtilWingineBusUtilization
 * Description: Print System Utilization
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_WinUtilWingineBusUtilization(WP_CHAR *StrPrm)
{
  WP_CHAR strParam[BUFFER_SIZE] = "sysu";
  CLI_WinUtilCall(strParam, StrPrm);
}


/***************************************************************
 * Func name  : CLI_WinUtilThreadUtilization
 * Description: PRint TRS utilization
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_WinUtilThreadUtilization(WP_CHAR *StrPrm)
{
 WP_CHAR strParam[BUFFER_SIZE] = "trsu";
  CLI_WinUtilCall(strParam, StrPrm);
}


/***************************************************************
 * Func name  : CLI_WinUtilBmap
 * Description: Prints Bus Mapping
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_WinUtilBmap  (WP_CHAR *StrPrm)
{
 WP_CHAR strParam[BUFFER_SIZE] = "bmap";
 CLI_WinUtilCall(strParam, StrPrm);
}

/***************************************************************
 * Func name  : CLI_WinUtilCoreDump
 * Description: Dump memory partition to log
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_WinUtilCoreDump  (WP_CHAR  *StrPrm)
{
  WP_CHAR strParam[BUFFER_SIZE] = "core";
  CLI_WinUtilCall(strParam, StrPrm);
}

/***************************************************************
 * Func name  : CLI_WinUtilDmap
 * Description: Prints device mapping
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_WinUtilDmap  (WP_CHAR  *StrPrm)
{
  WP_CHAR strParam[BUFFER_SIZE] = "dmap";
  CLI_WinUtilCall(strParam, StrPrm);
}

/***************************************************************
 * Func name  : CLI_WinUtilFmum
 * Description: Check for FMU put errors
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_WinUtilFmum (WP_CHAR *StrPrm)
{
  WP_CHAR strParam[BUFFER_SIZE] = "fmum";
  CLI_WinUtilCall(strParam, StrPrm);
}

/***************************************************************
 * Func name  : CLI_WinUtilFreq
 * Description: Change bus freq
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_WinUtilFreq (WP_CHAR *StrPrm)
{
 WP_CHAR strParam[BUFFER_SIZE] = "freq";
 WP_CHAR help[BUFFER_SIZE] = "freq -help";
  if(strlen(StrPrm) < 3)
  {
    WPU_WinutilCall(help);
    return;
  }
  CLI_WinUtilCall(strParam, StrPrm);
}

/***************************************************************
 * Func name  : CLI_WinUtilTraceEnable_DPS
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_WinUtilTraceEnable_DPS (WP_CHAR *StrPrm)
{
  WP_CHAR strParam[BUFFER_SIZE] = "tb init";
  CLI_WinUtilCall(strParam, StrPrm);
}

/***************************************************************
 * Func name  : CLI_WinUtilTraceEnable_Serial
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_WinUtilTraceEnable_Serial (WP_CHAR *StrPrm)
{
  WP_CHAR strParam[BUFFER_SIZE] = "tb init -serial";
  WP_CHAR serialName[10] = "";
  WP_S32 res;
  res = sscanf(StrPrm,"%*d %s",&serialName[0]);
  if(res != 1)
  {
    WPU_DUMP("Invalid number of arguments\n\n");
    WPU_DUMP("Usage: <serial_name>\n\n");
    WPU_DUMP("Parameters: \n");
    WPU_DUMP("<serial_name>: Serial name (note: not index),\
 See \"Trace Specific menu\" option 3, \"List Trace Serials\"\n");
    return;
  }
  CLI_WinUtilCall(strParam, StrPrm);
}

/***************************************************************
 * Func name  : CLI_WinUtilMemoryMap
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_WinUtilMemoryMap (WP_CHAR *StrPrm)
{
 /*TODO swap to direct api call, read memory value as well*/
  WP_CHAR strParam[BUFFER_SIZE] = "map";
  WPU_DUMP("Part of the map defines are zero based (e.g.winNwet)\n");
  CLI_WinUtilCall(strParam, StrPrm);
}

/***************************************************************
 * Func name  : CLI_WinUtilMemoryCheck
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_WinUtilMemoryCheck (WP_CHAR *StrPrm)
{
  WP_CHAR strParam[BUFFER_SIZE] = "mc";
  WP_CHAR help[BUFFER_SIZE] = "mc -help";
  if(strlen(StrPrm) < 3)
  {
    WPU_WinutilCall(help);
    return;
  }
  CLI_WinUtilCall(strParam, StrPrm);
}

/***************************************************************
 * Func name  : CLI_WinUtilMemoryDisplay
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_WinUtilMemoryDisplay (WP_CHAR *StrPrm)
{
  WP_CHAR strParam[BUFFER_SIZE] = "md";
  WP_CHAR help[BUFFER_SIZE] = "md -help";
  if(strlen(StrPrm) < 3)
  {
     WPU_WinutilCall(help);
    return;
  }
  CLI_WinUtilCall(strParam, StrPrm);
}

/***************************************************************
 * Func name  : CLI_WinUtilMemoryFill
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_WinUtilMemoryFill (WP_CHAR *StrPrm)
{
  WP_CHAR strParam[BUFFER_SIZE] = "mf";
  WP_CHAR help[BUFFER_SIZE] = "mf -help";
  if(strlen(StrPrm) < 3)
  {
    WPU_WinutilCall(help);
    return;
  }
  CLI_WinUtilCall(strParam, StrPrm);
}

/***************************************************************
 * Func name  : CLI_WinUtilMemoryModify
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_WinUtilMemoryModify (WP_CHAR *StrPrm)
{
  WP_CHAR strParam[BUFFER_SIZE] = "mm";
  WP_CHAR help[BUFFER_SIZE] = "mm -help";
  #ifdef MENU_DEBUG_MODE
   WPU_DUMP("Input StrPrm length: %d\n", strlen(StrPrm));
  #endif
  if(strlen(StrPrm) < 3)
  {
    WPU_WinutilCall(help);
    return;
  }
  CLI_WinUtilCall(strParam, StrPrm);
}

/***************************************************************
 * Func name  : CLI_WinUtilModifyTA_Table
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_WinUtilModifyTA_Table (WP_CHAR *StrPrm)
{
   WP_U32 baseIndex, tableIndex, tableSize, entryIndex, newVal;
   WP_S32 res;
   WP_status status;
   WPU_ta_info ta;

   WP_CHAR strParam[BUFFER_SIZE];
   res = sscanf(StrPrm, "%*d %d %d %d %d %x",\
     &baseIndex, &tableIndex, &entryIndex, &tableSize, &newVal);
   if(res != 3)
   {
     WPU_DUMP("Invalid number of arguments\n");
     WPU_DUMP("Usage: <baseIndex> <entryIndex> <entrySize> <newVal> \n");
     return;
   }
  status =  WPU_TA(0, (void*)baseIndex ,WPU_TA_CMD_GET_ONE_BASE, &ta);
  F_menu_TermOnError(status, "WPU_TA(0, baseIndex, WPU_TA_CMD_GET_ONE_BASE,ta)", __LINE__);
  sprintf(strParam, "mm %X %X", \
          ta.bases[baseIndex] + tableIndex*tableSize + entryIndex*(4), newVal);
  WPU_WinutilCall(strParam);
}

/***************************************************************
 * Func name  : CLI_WinUtilSA_TRSLimits
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_WinUtilSA_TRSLimits (WP_CHAR *StrPrm)
{
  WP_CHAR strParam[BUFFER_SIZE] = "smap";
  CLI_WinUtilCall(strParam, StrPrm);
}

/***************************************************************
 * Func name  : CLI_WinUtilTA_Bases
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_WinUtilTA_Bases (WP_CHAR *StrPrm)
{
  WP_CHAR strParam[BUFFER_SIZE] = "tab";
  CLI_WinUtilCall(strParam, StrPrm);
}

/***************************************************************
 * Func name  : CLI_WinUtilDisplayTA_Table
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_WinUtilDisplayTA_Table (WP_CHAR *StrPrm)
{
  WP_U32 baseIndex, entrySize, entryIndex;
  WP_S32 res;
  WP_status status;
  WP_CHAR strParam[BUFFER_SIZE];
  WPU_ta_info ta;
  res =  sscanf(StrPrm,"%*d %d %d %d",&baseIndex, &entrySize, &entryIndex);
  if( res < 3)
  {
    WPU_DUMP("Invalid number of arguments\n\n");
    WPU_DUMP("Usage: <baseIndex> <entrySize> <entryIndex>\n\n");
    WPU_DUMP("Parameters:\n");
    WPU_DUMP("<baseIndex>: Base Register Index,\
 See \"Winutil Cmd: tab\" for list of registers\n\n");
    WPU_DUMP("<entrySize>: Size of each Table Entry\n\n");
    WPU_DUMP("<entryIndex>: Table Entry in Register\n\n");
    return;
  }
 status =  WPU_TA(0, (void*)baseIndex ,WPU_TA_CMD_GET_ONE_BASE, &ta);
 F_menu_TermOnError(status, "WPU_TA(0, baseIndex, WPU_TA_CMD_GET_ONE_BASE,ta)", __LINE__);
 if((ta.bases[baseIndex]&0xF0000000) == 0xF0000000)
 {
   WPU_DUMP("Register is uninitialized\n");
   return;
 }
 sprintf(strParam,"md %X :%d",(ta.bases[baseIndex] & 0xFFFFFFF0)+entryIndex*entrySize,entrySize);
 WPU_DUMP("Printing Base Index: %d, Entry: %d\n",baseIndex, entryIndex);
 WPU_DUMP("-----------------------------------------------------\n");
 WPU_WinutilCall(strParam);
}


/***************************************************************
 * Func name  : CLI_WinUtilOrderMonitorDisplay
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_WinUtilOrderMonitorDisplay (WP_CHAR *StrPrm)
{
  WP_CHAR strParam[BUFFER_SIZE] = "taom";
  CLI_WinUtilCall(strParam, StrPrm);
}



/***************************************************************
 * Func name  : CLI_WinUtilTAOrderRegisters
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_WinUtilTAOrderRegisters (WP_CHAR *StrPrm)
{
  WP_CHAR strParam[BUFFER_SIZE] = "taot";
  CLI_WinUtilCall(strParam, StrPrm);
}

/***************************************************************
 * Func name  : CLI_WinUtilCurrentTRSStatus
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_WinUtilCurrentTRSStatus (WP_CHAR *StrPrm)
{
  WP_CHAR strParam[BUFFER_SIZE] = "tast";
  CLI_WinUtilCall(strParam, StrPrm);
}

/***************************************************************
 * Func name  : CLI_WinUtilTraceStart
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_WinUtilTraceStart (WP_CHAR *StrPrm)
{
  WP_CHAR strParam[BUFFER_SIZE] = "tb init";
  CLI_WinUtilCall(strParam, StrPrm);
}

/***************************************************************
 * Func name  : CLI_WinUtilTraceDump
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_WinUtilTraceDump (WP_CHAR *StrPrm)
{
  WP_CHAR strParam[BUFFER_SIZE] = "tb dump";
  WPU_WinutilCall(strParam);
}

#ifdef linux
/***************************************************************
 * Func name  : CLI_WinUtilTraceDumpFile
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_WinUtilTraceDumpFile (WP_CHAR *StrPrm)
{
  WP_CHAR strParam[BUFFER_SIZE] = "tb dump";
  WP_CHAR filename[512] = "";
  WP_S32 res;
  res=sscanf(StrPrm, "%*d %s", &filename[0]);
  if(res < 1)
  {
    WPU_DUMP("You must provide a filename\n");
    return;
  }
  CloseFile();
  SetFileName((WP_CHAR*)&filename);
  ChangeDumpFunction(&CLI_RetrieveStringDump);
  WPU_WinutilCall(strParam);
  CloseFile();
  ChangeDumpFunction(NULL);
}
#endif

/***************************************************************
 * Func name  : CLI_WinUtilDisplayAllTraceSerials
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_WinUtilDisplayAllTraceSerials()
{
 WP_U8 serial_index;
 /* Initalize the serial map table */
 wpu_serial_info *serial_map_table = (wpu_serial_info *)WPUI_HwSerialMapTable();

 WPU_DUMP("Listing Serial Names:\n");
 for(serial_index = 0; \
 (serial_index < WPU_SERIAL_LAST_ID) && (serial_map_table[serial_index].serial_name != NULL);\
  serial_index++)
   {
     WPU_DUMP("%s \t", serial_map_table[serial_index].serial_name);
     if((serial_index % 4) == 3)
       WPU_DUMP("\n");
   }
 WPU_DUMP("\n");
}

/***************************************************************
 * Func name  : CLI_WinUtilTRS_SerialAllocation
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_WinUtilTRS_SerialAllocation (WP_CHAR *StrPrm)
{
  WP_CHAR strParam[BUFFER_SIZE] = "trsa";
  CLI_WinUtilCall(strParam, StrPrm);
}

/***************************************************************
 * Func name  : CLI_WinUtilDebugVersionInfo
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_WinUtilDebugVersionInfo  (WP_CHAR *StrPrm)
{
  /*WP_CHAR strParam[BUFFER_SIZE] = "ver";
  CLI_WinUtilCall(strParam, StrPrm);*/
  WPU_WinutilCall("ver");
}

/***************************************************************
 * Func name  : CLI_WinUtilWingineUtilization
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_WinUtilWingineUtilization (WP_CHAR *StrPrm)
{
  WP_CHAR strParam[BUFFER_SIZE] = "wgu";
  CLI_WinUtilCall(strParam, StrPrm);
}

/***************************************************************
 * Func name  : CLI_WinUtilFifoCheck
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_WinUtilFifoCheck (WP_CHAR *StrPrm)
{
 WP_CHAR strParam[BUFFER_SIZE] = "wfc";
 WP_CHAR help[BUFFER_SIZE] = "wfc -help";
  if(strlen(StrPrm) < 3)
  {
    WPU_WinutilCall(help);
    return;
  }
  CLI_WinUtilCall(strParam, StrPrm);
}

/***************************************************************
 * Func name  : CLI_WinUtilFreeTxt
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_WinUtilFreeTxt (WP_CHAR *StrPrm)
{
 WP_CHAR strParam[BUFFER_SIZE] = "";
  CLI_WinUtilCall(strParam, StrPrm);
}

/***************************************************************
 * Func name  : CLI_WinUtilCall
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_WinUtilCall(WP_CHAR * strParam, WP_CHAR *StrPrm){
  WP_CHAR *tok;
  WP_S32 size = 0;
  size = strlen(strParam);
  tok = strtok(StrPrm," ");
  tok = strtok(NULL, " ");
  while (tok != NULL)
  {
    if (tok !=NULL)
      strcat(strParam, " ");
    size = size + strlen(tok) + 1;
    if(size > BUFFER_SIZE)
    {
      WPU_DUMP("Error: Too many arguments\n");
      return;
    }
    strcat(strParam, tok);
    tok = strtok(NULL, " ");
  }
#ifdef MENU_DEBUG_MODE
  WPU_DUMP("\nCommand Echo: '%s'\n",strParam);
#endif
  WPU_WinutilCall(strParam);
}


/***************************************************************
 * Func name  : CLI_WinUtilTRSDump
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_WinUtilTRSDump  (WP_CHAR *StrPrm)
{
  WP_U32 cmd, trs_num;
  WP_S32 res;
  WP_CHAR winutil_cmd[100];

  res = sscanf(StrPrm, "%d %d ", &cmd, &trs_num);

  if (res != 2)
    {
      WPU_DUMP("ERROR:invalid number of parameters\n");
      return;
    }

#ifdef WP_HW_WINPATH3
  if (trs_num > 64)
    {
      WPU_DUMP("ERROR: WP3 invalid TRS number\n");
      return;
    }
#endif
#ifdef WP_HW_WINPATH2
  if (trs_num > 32)
    {
      WPU_DUMP("ERROR: WP2 invalid TRS number\n");
      return;
    }
#endif
  sprintf( winutil_cmd,"trs %d",trs_num);

  WPU_WinutilCall(winutil_cmd);
}

/***************************************************************
 * Func name  : CLI_WinUtilHelp
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_WinUtilHelp()
{
  WPU_WinutilCall("-help");
}

/***************************************************************
 * Func name  : CLI_WinUtilTraceOptionsStatus
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_WinUtilTraceOptionsStatus()
{
  WPU_WinutilCall("tb status");
}

/*------------WINUTIL FUNCTIONS-END-----------------------------*/


/***************************************************************
 * Func name  : CLI_SYSCMD_GetTRS
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_SYSCMD_GetTRS(WP_CHAR *StrPrm)
{
   WP_S32 res, i;
   WP_U32 input;
   WP_syscmd_trs_info params;
   WP_U32 index;
   res = sscanf(StrPrm,"%*d %d", &input);
   params.serial_name = input;
   if (res != 1)
   {
    WPU_DUMP("Invalid number of arguments\n\n");
    WPU_DUMP("Usage: <trace_serial_index>\n\n");
    WPU_DUMP("Parameters: \n");
    WPU_DUMP("<trace_serial_index>: \n");
    CLI_SYSCMD_PrintTRSserials();
    return;
   }
   for (index = 0; TRSserials[index].index != params.serial_name; index++);
   if(WP_SysCommand(wpid_context, WP_SYSCMD_READ_TRS_INFO ,&params) == WP_OK)
   {
    WPU_DUMP("Serial name: %s, max TRS #: %d\n",TRSserials[index].name,params.entry.max_trs);
    WPU_DUMP("TRS Masks: ");
    for(i = 0; i < WINFARM_NUMBER;i++)
    {
      WPU_DUMP("WF%d: %04X  ",i,params.entry.trs_mask[i]);
      if(i%2 == 1)
        WPU_DUMP(" ");
    }
    WPU_DUMP("\n");
   }
}

/***************************************************************
 * Func name  : CLI_SYSCMD_SetTRS
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_SYSCMD_SetTRS(WP_CHAR *StrPrm)
{
   WP_S32 max_trs, res, i;
   WP_U32 trs_mask_pattern[4],input;
   WP_syscmd_trs_info params;
   WP_status status;
   WP_CHAR strParam[BUFFER_SIZE];
#ifdef WP_HW_WINPATH3
   res = sscanf(StrPrm, "%*d %d %d %X %X %X %X",
    &input, &max_trs, &trs_mask_pattern[0],
    &trs_mask_pattern[1], &trs_mask_pattern[2],&trs_mask_pattern[3]);
   params.serial_name=input;
   if(res < 6 )
#endif
#ifdef WP_HW_WINPATH2
   res = sscanf(StrPrm, "%*d %d %d %X %X",
                &input, &max_trs,
                &trs_mask_pattern[0],&trs_mask_pattern[1]);
   params.serial_name=input;
   if(res < 4 )
#endif
   {
    WPU_DUMP("Invalid number of arguments\n\n");
    WPU_DUMP("Usage:<trace_serial_index> <max_trs> <trs_mask_pattern>\n\n");
    WPU_DUMP("Parameters: \n");
    WPU_DUMP("<max_trs>: Max TRS value to set\n\n");
    WPU_DUMP("<trs_mask_pattern>: TRS Mask pattern of hexadecimal inputs,\
ie: AAAA BBBB CCCC DDDD\n");
    WPU_DUMP("<trace_serial_index>: \n");
    CLI_SYSCMD_PrintTRSserials();
    WPU_DUMP("\n\n");
    return;
   }
   params.entry.max_trs = max_trs;
   for(i = 0; i < WINFARM_NUMBER; i++)
   {
     params.entry.trs_mask[i] = trs_mask_pattern[i];
   }
   status = WP_SysCommand(wpid_context, WP_SYSCMD_WRITE_TRS_INFO, &params);
   F_menu_TermOnError(status, "WP_SysCommand(wpid, WP_SYSCMD_WRITE_TRS_INFO, &params)", __LINE__);
   if(status == WP_OK)
   {
     sprintf(strParam,"0 %d",params.serial_name);
     CLI_SYSCMD_GetTRS(strParam);
   }
}

/***************************************************************
 * Func name  : CLI_SYSCMD_PrintTRSserials
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_SYSCMD_PrintTRSserials()
{
 /*printf from WP_sa_serial enum*/
 WP_U32 i;
 WP_U32 cmds;
 WP_U32 rows;
 WPU_DUMP("Listing TRS Serials: \n");
 for(cmds = 0; TRSserials[cmds].name != NULL ; cmds++);
 rows = cmds/3;
 if (cmds%3 == 1)
 {
   for(i = 0; i < rows ; i++)
   {
     WPU_DUMP("(%03d) %s\t",TRSserials[i].index,TRSserials[i].name );
     if(strlen(TRSserials[i].name) < 10)
          WPU_DUMP("\t");
     WPU_DUMP("(%03d) %s\t",TRSserials[i+rows+1].index,TRSserials[i+rows+1].name );
     if(strlen(TRSserials[i+rows+1].name) < 10)
         WPU_DUMP("\t");
     WPU_DUMP("(%03d) %s\n",TRSserials[i+2*rows+1].index,TRSserials[i+2*rows+1].name );
   }
   WPU_DUMP("(%03d) %s/t",TRSserials[i].index,TRSserials[i].name );
 }else if(cmds%3 == 2)
 {
   for(i = 0; i < rows ; i++)
   {
     WPU_DUMP("(%03d) %s\t",TRSserials[i].index,TRSserials[i].name );
     if(strlen(TRSserials[i].name ) < 10)
         WPU_DUMP("\t");
     WPU_DUMP("(%03d) %s\t",TRSserials[i+rows+1].index,TRSserials[i+rows+1].name );
     if(strlen(TRSserials[i+rows+1].name ) < 10)
        WPU_DUMP("\t");
     WPU_DUMP("(%03d) %s\n",TRSserials[i+2*rows+2].index,TRSserials[i+2*rows+2].name );
   }
     WPU_DUMP("(%03d) %s\t",TRSserials[i].index,TRSserials[i].name );
     if(strlen(TRSserials[i].name ) < 10)
         WPU_DUMP("\t");
     WPU_DUMP("(%03d) %s\t",TRSserials[i+rows+1].index,TRSserials[i+rows+1].name );
 }else
 {
   for(i = 0; i < rows ; i++)
   {
     WPU_DUMP("(%03d) %s\t",TRSserials[i].index,TRSserials[i].name );
     if(strlen(TRSserials[i].name ) < 10)
        WPU_DUMP("\t");
     WPU_DUMP("(%03d) %s\t",TRSserials[i+rows].index,TRSserials[i+rows].name );
     if(strlen(TRSserials[i+rows].name ) < 10)
        WPU_DUMP("\t");
     WPU_DUMP("(%03d) %s\n",TRSserials[i+2*rows].index,TRSserials[i+2*rows].name );
   }
 }
   WPU_DUMP("\n\n");
}


/***************************************************************
 * Func name  : CLI_DisplayDTCR
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_DisplayDTCR()
{
  WP_CHAR strParam[BUFFER_SIZE];

 #ifdef WP_HW_WINPATH3
      WPU_DUMP("\n\nWF#0:");
      WPU_DUMP("\n------\n");
      WPU_DUMP("dtcr1:");
      sprintf(strParam,"md %s :%d",STRINGIFY_(MAP_WF0_DTCR1),4);
      WPU_WinutilCall(strParam);
      WPU_DUMP("dtcr2:");
      sprintf(strParam,"md %s :%d",STRINGIFY_(MAP_WF0_DTCR2),4);
      WPU_WinutilCall(strParam);
      WPU_DUMP("dtcr3:");
      sprintf(strParam,"md %s :%d",STRINGIFY_(MAP_WF0_DTCR3),4);
      WPU_WinutilCall(strParam);
      WPU_DUMP("dtcr4:");
      sprintf(strParam,"md %s :%d",STRINGIFY_(MAP_WF0_DTCR4),4);
      WPU_WinutilCall(strParam);


      WPU_DUMP("\n\nWF#1:");
      WPU_DUMP("\n------\n");
      WPU_DUMP("dtcr1:");
      sprintf(strParam,"md %s :%d",STRINGIFY_(MAP_WF1_DTCR1),4);
      WPU_WinutilCall(strParam);
      WPU_DUMP("dtcr2:");
      sprintf(strParam,"md %s :%d",STRINGIFY_(MAP_WF1_DTCR2),4);
      WPU_WinutilCall(strParam);
      WPU_DUMP("dtcr3:");
      sprintf(strParam,"md %s :%d",STRINGIFY_(MAP_WF1_DTCR3),4);
      WPU_WinutilCall(strParam);
      WPU_DUMP("dtcr4:");
      sprintf(strParam,"md %s :%d",STRINGIFY_(MAP_WF1_DTCR4),4);
      WPU_WinutilCall(strParam);

  #ifndef WP_BOARD_WDS3_SL
      WPU_DUMP("\n\nWF#2:");
      WPU_DUMP("\n------\n");
      WPU_DUMP("dtcr1:");
      sprintf(strParam,"md %s :%d",STRINGIFY_(MAP_WF2_DTCR1),4);
      WPU_WinutilCall(strParam);
      WPU_DUMP("dtcr2:");
      sprintf(strParam,"md %s :%d",STRINGIFY_(MAP_WF2_DTCR2),4);
      WPU_WinutilCall(strParam);
      WPU_DUMP("dtcr3:");
      sprintf(strParam,"md %s :%d",STRINGIFY_(MAP_WF2_DTCR3),4);
      WPU_WinutilCall(strParam);
      WPU_DUMP("dtcr4:");
      sprintf(strParam,"md %s :%d",STRINGIFY_(MAP_WF2_DTCR4),4);
      WPU_WinutilCall(strParam);


      WPU_DUMP("\n\nWF#3:");
      WPU_DUMP("\n------\n");
      WPU_DUMP("dtcr1:");
      sprintf(strParam,"md %s :%d",STRINGIFY_(MAP_WF3_DTCR1),4);
      WPU_WinutilCall(strParam);
      WPU_DUMP("dtcr2:");
      sprintf(strParam,"md %s :%d",STRINGIFY_(MAP_WF3_DTCR2),4);
      WPU_WinutilCall(strParam);
      WPU_DUMP("dtcr3:");
      sprintf(strParam,"md %s :%d",STRINGIFY_(MAP_WF3_DTCR3),4);
      WPU_WinutilCall(strParam);
      WPU_DUMP("dtcr4:");
      sprintf(strParam,"md %s :%d",STRINGIFY_(MAP_WF3_DTCR4),4);
      WPU_WinutilCall(strParam);
  #endif
#endif

#ifdef WP_HW_WINPATH2

      WPU_DUMP("\n\nWF#0:");
      WPU_DUMP("\n------\n");
      WPU_DUMP("dtcr1:");
      sprintf(strParam,"md %s :%d",STRINGIFY_(MAP_DWG0_DTCR1),4);
      WPU_WinutilCall(strParam);
      WPU_DUMP("dtcr2:");
      sprintf(strParam,"md %s :%d",STRINGIFY_(MAP_DWG0_DTCR2),4);
      WPU_WinutilCall(strParam);
      WPU_DUMP("dtcr3:");
      sprintf(strParam,"md %s :%d",STRINGIFY_(MAP_DWG0_DTCR3),4);
      WPU_WinutilCall(strParam);
      WPU_DUMP("dtcr4:");
      sprintf(strParam,"md %s :%d",STRINGIFY_(MAP_DWG0_DTCR4),4);
      WPU_WinutilCall(strParam);


      WPU_DUMP("\n\nWF#1:");
      WPU_DUMP("\n------\n");
      WPU_DUMP("dtcr1:");
      sprintf(strParam,"md %s :%d",STRINGIFY_(MAP_DWG1_DTCR1),4);
      WPU_WinutilCall(strParam);
      WPU_DUMP("dtcr2:");
      sprintf(strParam,"md %s :%d",STRINGIFY_(MAP_DWG1_DTCR2),4);
      WPU_WinutilCall(strParam);
      WPU_DUMP("dtcr3:");
      sprintf(strParam,"md %s :%d",STRINGIFY_(MAP_DWG1_DTCR3),4);
      WPU_WinutilCall(strParam);
      WPU_DUMP("dtcr4:");
      sprintf(strParam,"md %s :%d",STRINGIFY_(MAP_DWG1_DTCR4),4);
      WPU_WinutilCall(strParam);
#endif
}

/***************************************************************
 * Func name  : CLI_DisplayTA_PCs
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_DisplayTA_PCs()
{
  WP_U32 i,changed;
  WPU_ta_info ta1,ta2;

  WPU_TA(wpid_context,0,WPU_TA_CMD_GET_PC,&ta1);
  for(i = 0; i< 500000; i++); /*wait*/
  WPU_TA(wpid_context,0,WPU_TA_CMD_GET_PC,&ta2);
  for(i = 0, changed = 0; i< WPU_TRS_SETS; i++)
  {
    if(ta1.pc[i] != ta2.pc[i])
      changed = 1;
  }
  if(changed)
    WPU_DUMP("TA PCs changing\n");
  else
    WPU_DUMP("TA PCs halted\n");

  /*Print captured TA PC*/
  for(i = 0; i< WPU_TRS_SETS; i++)
  {
    WPU_DUMP("0x%08X  ",ta2.pc[i]);
    if(i%4 == 3)
      WPU_DUMP("\n");
  }
}

/***************************************************************
 * Func name  : CLI_DisplaySA_PCT_Table
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_DisplaySA_PCT_Table()
{
  WP_CHAR strParam[BUFFER_SIZE] = "";
#ifdef WP_HW_WINPATH3
  sprintf(strParam, "md %s :%d", STRINGIFY_(MAP_AA_PCT), 64*4);
  WPU_DUMP("AA PCT: ");
#endif
#ifdef WP_HW_WINPATH2
  sprintf(strParam, "md %s :%d", STRINGIFY_(MAP_SA_PCT), 128);
  WPU_DUMP("SA PCT: ");
#endif
  WPU_WinutilCall(strParam);

}

/***************************************************************
 * Func name  : CLI_DisplayFifo_StartEnd
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_DisplayFifo_StartEnd(WP_CHAR *StrPrm)
{
  WP_CHAR strParam[BUFFER_SIZE] = "wfc list ";
  if(strlen(StrPrm) < 3)
  {
   WPU_DUMP("Invalid number of arguments\n");
   WPU_DUMP("Usage: <WMM_no> <FIFO_no>\n");
   return;
  }
  CLI_WinUtilCall(strParam, StrPrm);
}

/***************************************************************
 * Func name  : CLI_DisplayFifo_corruption
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_DisplayFifo_corruption(WP_CHAR *StrPrm)
{
  WP_CHAR strParam[BUFFER_SIZE] = "wfc reg ";
  if(strlen(StrPrm) < 2)
  {
    WPU_DUMP("Invalid number of arguments\n");
    WPU_DUMP("Usage: <WMM_no>\n");
    return;
  }
  CLI_WinUtilCall(strParam, StrPrm);
}

/***************************************************************
 * Func name  : CLI_SetFifo_base
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_SetFifo_base(WP_CHAR *StrPrm)
{
  WP_CHAR strParam[BUFFER_SIZE] = "wfc base ";
  if(strlen(StrPrm) < 3)
  {
    WPU_DUMP("Invalid number of arguments\n");
    WPU_DUMP("Usage: <WMM_no> <FIFO_no>\n");
    return;
  }
  CLI_WinUtilCall(strParam, StrPrm);
}

/***************************************************************
 * Func name  : CLI_SetFifo_offset
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_SetFifo_offset(WP_CHAR *StrPrm)
{
  WP_CHAR strParam[BUFFER_SIZE] = "wfc offset ";
  if(strlen(StrPrm) < 3)
  {
    WPU_DUMP("Invalid number of arguments\n");
    WPU_DUMP("Usage: <WMM_no> <FIFO_no>\n");
    return;
  }
  CLI_WinUtilCall(strParam, StrPrm);
}

/***************************************************************
 * Func name  : CLI_Bus_ECCcheck
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_Bus_ECCcheck()
{
  WP_CHAR strParam[BUFFER_SIZE];
#ifdef WP_HW_WINPATH3
  WPU_DUMP("MAP_SIU1_REG_ECC_DBG: ");
  sprintf(strParam,"md %s :%d",STRINGIFY_(MAP_SIU1_REG_ECC_DBG),4);
  WPU_WinutilCall(strParam);
  WPU_DUMP("MAP_SIU1_REG_ECC_CNTR: ");
  sprintf(strParam,"md %s :%d",STRINGIFY_(MAP_SIU1_REG_ECC_CNTR),4);
  WPU_WinutilCall(strParam);
  WPU_DUMP("MAP_SIU2_REG_ECC_DBG: ");
  sprintf(strParam,"md %s :%d",STRINGIFY_(MAP_SIU2_REG_ECC_DBG),4);
  WPU_WinutilCall(strParam);
  WPU_DUMP("MAP_SIU2_REG_ECC_CNTR: ");
  sprintf(strParam,"md %s :%d",STRINGIFY_(MAP_SIU2_REG_ECC_CNTR),4);
  WPU_WinutilCall(strParam);
  WPU_DUMP("MAP_SIU3_REG_ECC_DEBUG_CONF0: ");
  sprintf(strParam,"md %s :%d",STRINGIFY_(MAP_SIU3_REG_ECC_DEBUG_CONF0),4);
  WPU_WinutilCall(strParam);
  WPU_DUMP("MAP_SIU3_REG_ECC_DEBUG_CONF1: ");
  sprintf(strParam,"md %s :%d",STRINGIFY_(MAP_SIU3_REG_ECC_DEBUG_CONF1),4);
  WPU_WinutilCall(strParam);
  WPU_DUMP("MAP_SIU3_REG_ECC_ERROR_COUNTER: ");
  sprintf(strParam,"md %s :%d",STRINGIFY_(MAP_SIU3_REG_ECC_ERROR_COUNTER),4);
  WPU_WinutilCall(strParam);
  WPU_DUMP("MAP_SIU4X_ECC_CFG");
  sprintf(strParam,"md %s :%d",STRINGIFY_(MAP_SIU4X_ECC_CFG),4);
  WPU_WinutilCall(strParam);
  WPU_DUMP("MAP_SIU4X_ECC_CNT");
  sprintf(strParam,"md %s :%d",STRINGIFY_(MAP_SIU4X_ECC_CNT),4);
  WPU_WinutilCall(strParam);
  WPU_DUMP("MAP_SIU4D_REG_ECC_DBG");
  sprintf(strParam,"md %s :%d",STRINGIFY_(MAP_SIU4D_REG_ECC_DBG),4);
  WPU_WinutilCall(strParam);
  WPU_DUMP("MAP_SIU4D_REG_ECC_CNTR");
  sprintf(strParam,"md %s :%d",STRINGIFY_(MAP_SIU4D_REG_ECC_CNTR),4);
  WPU_WinutilCall(strParam);
#endif
#ifdef WP_HW_WINPATH2
  WPU_DUMP("MAP_SRAM_BIU_ECC_DBG; ");
  sprintf(strParam,"md %s :%d",STRINGIFY_(MAP_SRAM_BIU_ECC_DBG),4);
  WPU_WinutilCall(strParam);
  WPU_DUMP("MAP_SRAM_BIU_ECC_ECNT; ");
  sprintf(strParam,"md %s :%d",STRINGIFY_(MAP_SRAM_BIU_ECC_ECNT),4);
  WPU_WinutilCall(strParam);
  WPU_DUMP("MAP_DDR_BIU_ECC_DBG; ");
  sprintf(strParam,"md %s :%d",STRINGIFY_(MAP_DDR_BIU_ECC_DBG),4);
  WPU_WinutilCall(strParam);
  WPU_DUMP("MAP_DDR_BIU_ECC_ECNT; ");
  sprintf(strParam,"md %s :%d",STRINGIFY_(MAP_DDR_BIU_ECC_ECNT),4);
  WPU_WinutilCall(strParam);
  WPU_DUMP("MAP_Global_BIU_SD_Status: ");
  sprintf(strParam,"md %s :%d",STRINGIFY_(MAP_Global_BIU_SD_Status),8);
  WPU_WinutilCall(strParam);
  WPU_DUMP("MAP_Host_BIU_ECC_DBG; ");
  sprintf(strParam,"md %s :%d",STRINGIFY_(MAP_Host_BIU_ECC_DBG),4);
  WPU_WinutilCall(strParam);
  WPU_DUMP("MAP_Host_BIU_ECC_ECNT; ");
  sprintf(strParam,"md %s :%d",STRINGIFY_(MAP_Host_BIU_ECC_ECNT),4);
  WPU_WinutilCall(strParam);
  WPU_DUMP("MAP_APP_BIU_ECC_DBG");
  sprintf(strParam,"md %s :%d",STRINGIFY_(MAP_APP_BIU_ECC_DBG),4);
  WPU_WinutilCall(strParam);
  WPU_DUMP("MAP_APP_BIU_ECC_ECNT");
  sprintf(strParam,"md %s :%d",STRINGIFY_(MAP_APP_BIU_ECC_ECNT),4);
  WPU_WinutilCall(strParam);
#endif
}

/***************************************************************
 * Func name  : CLI_DisplayFifo_CorruptedNumbers
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_DisplayFifo_CorruptedNumbers()
{
   WP_CHAR strParam[BUFFER_SIZE];
   WP_S32 wmm_no;
   WPU_DUMP("wfc reg: This mechanism is not enabled by default\n");
   for(wmm_no = 0; wmm_no < WP_BUS_WMM5; wmm_no++)
   {
#ifdef WP_BOARD_WDS3_SL
     if(wmm_no == WP_BUS_PACKET)
        continue;
#endif
     sprintf(strParam,"wfc reg %d",wmm_no);
     WPU_WinutilCall(strParam);
   }
}

/***************************************************************
 * Func name  : CLI_All_VersionBuildTime
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_All_VersionBuildTime()
{
  WPU_DUMP("\nDebug Menu Build %s %s\n",buildinfo[0],buildinfo[1]);
  CLI_WinUtilDebugVersionInfo(NO_INPUT);
  CLI_WDDI_VersionBuildTime();
}

/***************************************************************
 * Func name  : F_menu_TermOnError
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void F_menu_TermOnError (WP_handle handle, WP_CHAR *s, WP_U32 LineNum)
{
   if (WP_ERROR_P(handle))
   {
      printf("\nDebug Menu Error Msg %s %s 0x%x Line:%d\n", s,
             WP_error_name[WP_ERROR(handle)], handle, LineNum);
      printf("Debug Command Aborted !!!\n");
   }
}

/***************************************************************
 * Func name  : CLI_SetLogLevel
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_SetLogLevel(WP_CHAR *StrPrm)
{
   WP_S32 level, res;
   res = sscanf(StrPrm,"%*d %d",&level);
   if(res != 1)
   {
     WPU_DUMP("Invalid number of arguments\n");
     WPU_DUMP("Usage: <new_log_level>\n");
     return;
   }
   max_log_level = level;
   WPU_DUMP("Log Level set to = %d\n", max_log_level);
}

WP_status WTI_DebugMemoryAlloc(const WP_CHAR *file, WP_U32 line, WP_U32 bus,WP_U32 size,\
   WP_U32 alignment, const WP_CHAR *purpose)
{
   WPU_DUMP("file = %s , line = %d  bus = %d  size = %d , %s \n",file,line,bus,size,purpose);
   return WP_OK;
}

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
WP_status WT_WddiLogged(WP_S32 level, const WP_CHAR *string)
{
  if (level <= max_log_level)
  {
    WPU_DUMP("%s", string);
    return WP_OK;
  }else
  {
    return WP_ERR_INCORRECT_REGISTRY_STATE;
  }
}

/***************************************************************
 * Func name  : WT_CallTraceCallback
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void WT_CallTraceCallback(WP_CHAR *file, WP_CHAR *function, WP_U32 line) {

  static WP_U32 first_time = 1;

  if (first_time)
  {
    first_time = 0;
    WPU_DUMP("\nWDDI Call Log Enabled\n");
  }
  WPU_DUMP("\nWDDI call from....\nFile = %s\
 \nFunction = %s \nLine = %d \n", file, function, line);

  return;
}

/***************************************************************
 * Func name  : CLI_WDDI_Allocation
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_WDDI_Allocation(void)
{
   WP_status status;
   if(WP_DEBUG_ENABLE_WDDI_ALLOCATION != 1)
   {
     WPU_DUMP("ERROR WDDI ALLOCATION NOT ENABLED\n");
     return;
   }
   status = WP_ControlRegister(WP_DEBUG_CALLBACK_WDDI_ALLOCATION,&WTI_DebugMemoryAlloc);
   if(status == WP_OK)
     WPU_DUMP("\nWDDI Allocation Log enabled\n");
   F_menu_TermOnError(status, "WP_ControlRegister() WP_DEBUG_CALLBACK_WDDI_ALLOCATION",__LINE__);
}

/***************************************************************
 * Func name  : CLI_WDDI_AllocationEND
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_WDDI_AllocationEND(void)
{
   WP_status status;
   if(WP_DEBUG_ENABLE_WDDI_ALLOCATION != 1)
   {
     WPU_DUMP("ERROR WDDI ALLOCATION NOT ENABLED\n");
     return;
   }
   status = WP_ControlRegister(WP_DEBUG_CALLBACK_WDDI_ALLOCATION,NULL);
   if(status == WP_OK)
     WPU_DUMP("\nWDDI Allocation Log disabled\n");
   F_menu_TermOnError(status, "WP_ControlRegister() WP_DEBUG_CALLBACK_WDDI_ALLOCATION",__LINE__);
}

/***************************************************************
 * Func name  : CLI_APILevel_StartLog
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_APILevel_StartLog()
{
   WP_status status;
   if (WP_DEBUG_ENABLE_WDDI_CALL_API  != 1)
   {
     WPU_DUMP("ERROR API LOG NOT ENABLED\n");
     return;
   }
   status = WP_ControlRegister(WP_DEBUG_CALLBACK_WDDI_CALL, &WT_CallTraceCallback);
   if(status == WP_OK)
     WPU_DUMP("\nAPI Level Log enabled\n");
   F_menu_TermOnError(status, "WP_ControlRegister() WP_DEBUG_CALLBACK_WDDI_CALL",__LINE__);
}

/***************************************************************
 * Func name  : CLI_APILevel_EndLog
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_APILevel_EndLog()
{
   WP_status status;
   if (WP_DEBUG_ENABLE_WDDI_CALL_API  != 1)
   {
     WPU_DUMP("ERROR API LOG NOT ENABLED\n");
     return;
   }
   status = WP_ControlRegister(WP_DEBUG_CALLBACK_WDDI_CALL, NULL);
   if(status == WP_OK)
     WPU_DUMP("\nAPI Level Log disabled\n");
   F_menu_TermOnError(status, "WP_ControlRegister() WP_DEBUG_CALLBACK_WDDI_CALL", __LINE__);
}

/***************************************************************
 * Func name  : CLI_InternalLevel_StartLog
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_InternalLevel_StartLog()
{
   WP_status status;
   if (WP_DEBUG_ENABLE_WDDI_CALL_INTERNAL  != 1)
   {
     WPU_DUMP("ERROR WDDI INTERNAL LOG NOT ENABLED\n");
     return;
   }
   status = WP_ControlRegister(WP_DEBUG_CALLBACK_WDDI_ALLOCATION, &WT_WddiLogged);
   F_menu_TermOnError(status, "WP_ControlRegister() WP_DEBUG_CALLBACK_WDDI_LOG", __LINE__);
}

/***************************************************************
 * Func name  : CLI_InternalLevel_EndLog
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_InternalLevel_EndLog()
{
   WP_status status;
   if (WP_DEBUG_ENABLE_WDDI_CALL_INTERNAL  != 1)
   {
     WPU_DUMP("ERROR WDDI INTERNAL LOG NOT ENABLED\n");
     return;
   }
   status = WP_ControlRegister(WP_DEBUG_CALLBACK_WDDI_ALLOCATION, NULL);
   F_menu_TermOnError(status, "WP_ControlRegister() WP_DEBUG_CALLBACK_WDDI_LOG", __LINE__);
}

/***************************************************************
 * Func name  : CLI_WPILevel_StartLog
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_WPILevel_StartLog()
{
   WP_status status;
   if (WP_DEBUG_ENABLE_WDDI_LOG  != 1)
   {
     WPU_DUMP("ERROR WDDI DEBUG LOG NOT ENABLED\n");
     return;
   }
   status = WP_ControlRegister(WP_DEBUG_CALLBACK_WDDI_LOG, WT_WddiLogged);
   if (status == WP_OK)
      WPU_DUMP("\nWPI Level Log enabled\n");
   F_menu_TermOnError(status, "WP_ControlRegister() WP_DEBUG_CALLBACK_WDDI_LOG", __LINE__);
}

/***************************************************************
 * Func name  : CLI_WPILevel_EndLog
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_WPILevel_EndLog()
{
   WP_status status;
   if (WP_DEBUG_ENABLE_WDDI_LOG  != 1)
   {
     WPU_DUMP("ERROR WDDI DEBUG LOG NOT ENABLED\n");
     return;
   }
   status = WP_ControlRegister(WP_DEBUG_CALLBACK_WDDI_LOG, NULL);
   if (status == WP_OK)
      WPU_DUMP("\nWPI Level Log disabled\n");
   F_menu_TermOnError(status, "WP_ControlRegister() WP_DEBUG_CALLBACK_WDDI_LOG", __LINE__);
}

/***************************************************************
 * Func name  : CLI_FineGrainLocking
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_FineGrainLocking()
{
   WP_status status;
   if(USE_MULTI_THREADING_FINE_GRAIN_LOCKING_WDDI != 1)
   {
     WPU_DUMP("Fine grain locking disabled\n");
     return;
   }
   status = WP_ControlRegister(WP_DEBUG_MULTI_THREADING_FINE_GRAIN_LOCKING, NULL);
   F_menu_TermOnError(status, "WP_ControlRegister()\
      WP_DEBUG_MULTI_THREADING_FINE_GRAIN_LOCKING", __LINE__);
}

/***************************************************************
 * Func name  : CLI_WDDI_VersionBuildTime
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_WDDI_VersionBuildTime()
{
   WT_Identify();
   /*WP_U32 p, w;
   WP_CHAR *first_dps_found = NULL;
   WP_U32 found_error = 0;
   WPU_DUMP("Test Identity Version %s\n", WP_Identity.wddi_version);
   WPU_DUMP("Test Identity Build %s\n", WP_Identity.wddi_build);
   for (p = 0; p < WP_MAX_WINPATHS; p++) {
      for (w = 0; w < WP_WINFARMS; w++) {
         if (!WP_Identity.dps_in_wddi[p][w] ||
             strlen(WP_Identity.dps_in_wddi[p][w]) == 0)
            continue;
         WPU_DUMP("Test Identity DPS WP%d WF%d in wddi %s in use %s\n", p, w,
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
      WPU_DUMP("Test Abort DPS not uniform\n");
      WT_FailAndTerminate();
   }*/
}

/***************************************************************
 * Func name  : CLI_FatalError_FileDump
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
#ifdef linux
void CLI_FatalError_FileDump(char *StrPrm)
{
  WP_S32 i;
  WP_CHAR strParam[BUFFER_SIZE] = "";
  WP_CHAR filename[512] = "";
  WP_S32 res;
  res=sscanf(StrPrm, "%*d %s", &filename[0]);
  if(res < 1)
  {
    WPU_DUMP("You must provide a filename\n");
    return;
  }
  CloseFile();
  SetFileName((WP_CHAR*)&filename);
  ChangeDumpFunction(&CLI_RetrieveStringDump);
  WPU_DUMP("Dumping DTCR Registers\n");
  CLI_DisplayDTCR();
  WPU_DUMP("Dumping TA Bases\n");
  CLI_WinUtilTA_Bases (NO_INPUT);
  WPU_DUMP("Dumping TA Order Monitor");
  WPU_WinutilCall("taom");
  WPU_DUMP("Dumping TA PC table\n");
  CLI_DisplayTA_PCs();
  WPU_DUMP("Dumping TA status Table\n");
  WPU_WinutilCall("tast");
  WPU_DUMP("Dumping TA Order Table\n");
  WPU_WinutilCall("taot");
  WPU_DUMP("Checking for FMU put errors\n:");
  WPU_WinutilCall("fmum");
  WPU_DUMP("Dumping ECC Registers\n");
  CLI_Bus_ECCcheck();
  WPU_DUMP("Dumping WP_Display SA info\n");
  WP_Display(wpid_context, WP_DISPLAY_SA,0, NULL);
  WPU_DUMP("Checking Fifo for Corruption and Errors\n");
  CLI_DisplayFifo_CorruptedNumbers();
  WPU_DUMP("Checking TRS Serial Allocations");
  CLI_WinUtilTRS_SerialAllocation(NO_INPUT);
  WPU_DUMP("Checking for Bus Monitor Errors\n");
  CLI_WinUtilBusErrCheck(NO_INPUT);
  WPU_DUMP("Dumping Traces\n");
  CLI_WinUtilTraceDump (NO_INPUT);
  WPU_DUMP("Dumping TRS\n");
  for(i = 0; i < NO_OF_THREADS; i++)
  {
    sprintf(strParam,"trs %d",i);
    WPU_WinutilCall(strParam);
   }
  CLI_All_VersionBuildTime();
  CloseFile();
  ChangeDumpFunction(NULL);
 }
#endif

/***************************************************************
 * Func name  : CLI_FatalError_DisplayAll
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_FatalError_DisplayAll(WP_CHAR *StrPrm)
{
  WP_S32 i;
  WP_CHAR strParam[BUFFER_SIZE];
  WPU_DUMP("Dumping DTCR Registers\n");
  CLI_DisplayDTCR();
  WPU_DUMP("Dumping TA Bases\n");
  CLI_WinUtilTA_Bases(NO_INPUT);
  WPU_DUMP("Dumping TA Order Monitor");
  WPU_WinutilCall("taom");
  WPU_DUMP("Dumping TA PC table\n");
  CLI_DisplayTA_PCs();
  WPU_DUMP("Dumping TA status Table\n");
  WPU_WinutilCall("tast");
  WPU_DUMP("Dumping TA Order Table\n");
  WPU_WinutilCall("taot");
  WPU_DUMP("Checking for FMU put errors\n:");
  WPU_WinutilCall("fmum");
  WPU_DUMP("Dumping ECC Registers\n");
  CLI_Bus_ECCcheck();
  WPU_DUMP("Dumping WP_Display SA info\n");
  WP_Display(wpid_context, WP_DISPLAY_SA,0, NULL);
  WPU_DUMP("Checking Fifo for Corruption and Errors\n");
  CLI_DisplayFifo_CorruptedNumbers();
  WPU_DUMP("Checking TRS Serial Allocations");
  CLI_WinUtilTRS_SerialAllocation(NO_INPUT);
  WPU_DUMP("Checking for Bus Monitor Errors\n");
  CLI_WinUtilBusErrCheck(NO_INPUT);
  WPU_DUMP("Dumping Traces\n");
  CLI_WinUtilTraceDump (NO_INPUT);
  WPU_DUMP("Dumping TRS\n");
  for(i = 0; i < NO_OF_THREADS; i++)
  {
    sprintf(strParam,"trs %d",i);
    WPU_WinutilCall(strParam);
   }
  CLI_All_VersionBuildTime();
 }

/***************************************************************
 * Func name  : CLI_ShowAllDisplayCmds
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_ShowAllDisplayCmds()
{
 WP_U32 i;
 WP_U32 cmds;
 WP_U32 rows;
 WPU_DUMP("Listing WP_Display command index: \n");

 for(cmds = 0; DispCmds[cmds].name != NULL ; cmds++);
 rows = cmds/3;
 if (cmds%3 == 1){
  for(i = 0; i < rows ; i++)
  {
    WPU_DUMP("(%03X) %s",DispCmds[i].index,DispCmds[i].name );
    if(strlen(DispCmds[i].name) < 15)
        WPU_DUMP("\t");
    if(strlen(DispCmds[i].name) < 11)
        WPU_DUMP("\t");
    WPU_DUMP("(%03X) %s",DispCmds[i+rows+1].index,DispCmds[i+rows+1].name );
    if(strlen(DispCmds[i+rows+1].name) < 15)
        WPU_DUMP("\t");
    if(strlen(DispCmds[i+rows+1].name) < 11)
        WPU_DUMP("\t");
    WPU_DUMP("(%03X) %s\n",DispCmds[i+2*rows+1].index,DispCmds[i+2*rows+1].name );
  }
  WPU_DUMP("(%03X) %s/t",DispCmds[i].index,DispCmds[i].name );
 }else if(cmds%3 == 2){
  for(i = 0; i < rows ; i++)
  {
    WPU_DUMP("(%03X) %s",DispCmds[i].index,DispCmds[i].name );
    if(strlen(DispCmds[i].name) < 15)
        WPU_DUMP("\t");
    if(strlen(DispCmds[i].name ) < 11)
        WPU_DUMP("\t");
    WPU_DUMP("(%03X) %s",DispCmds[i+rows+1].index,DispCmds[i+rows+1].name );
    if(strlen(DispCmds[i+rows+1].name) < 15)
        WPU_DUMP("\t");
    if(strlen(DispCmds[i+rows+1].name ) < 11)
        WPU_DUMP("\t");
    WPU_DUMP("(%03X) %s\n",DispCmds[i+2*rows+2].index,DispCmds[i+2*rows+2].name );
 }
    WPU_DUMP("(%03X) %s\t",DispCmds[i].index,DispCmds[i].name );
    if(strlen(DispCmds[i].name ) < 11)
        WPU_DUMP("\t");
    WPU_DUMP("(%03X) %s\t",DispCmds[i+rows+1].index,DispCmds[i+rows+1].name );
 }else {
  for(i = 0; i < rows ; i++)
  {
    WPU_DUMP("(%03X) %s",DispCmds[i].index,DispCmds[i].name );
    if(strlen(DispCmds[i].name) < 15)
        WPU_DUMP("\t");
    if(strlen(DispCmds[i].name ) < 11)
        WPU_DUMP("\t");
    WPU_DUMP("(%03X) %s",DispCmds[i+rows+1].index,DispCmds[i+rows+1].name );
    if(strlen(DispCmds[i+rows+1].name) < 15)
        WPU_DUMP("\t");
    if(strlen(DispCmds[i+rows+1].name ) < 11)
        WPU_DUMP("\t");
    WPU_DUMP("(%03X) %s\n",DispCmds[i+2*rows+1].index,DispCmds[i+2*rows+1].name );
  }
 }
 WPU_DUMP("\n\n");
}



/***************************************************************
 * Func name  : CLI_Display_System
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_Display_System()
{
  WP_status status;
  status = WP_Display(wpid_context,WP_DISPLAY_SYSTEM,0,NULL);
}

/***************************************************************
 * Func name  : CLI_DisplayCmd
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_DisplayCmd (WP_CHAR *StrPrm)
{
  WP_U32 cmd, flags = 0, i;
  WP_S32 res;
  WP_U32 status;
  WP_U32 errorlist[] = {29, 30, 31,999};
  WP_U32 handlelist[]= {3,6,999};
  WP_CHAR *disp_flags[] =
  { " (0) DEFAULT",
    " (1) RAW_HANDLES",
    " (2) RAW_DPS_STRUCTURES",
    " (4) RAW_REGISTERS",
    " (8) FMU_ONLY",
    "(16) FMU_L3_ONLY"
  };
  res =sscanf(StrPrm, "%*d 0x%x %d", &cmd, &flags);
  if( res < 1 )
  {
    WPU_DUMP("Invalid number of arguments\n\n");
    WPU_DUMP("Usage: <display_cmd_index> <display_flag_index>\n\n");
    WPU_DUMP("Parameters: \n");
    WPU_DUMP("<display_flag_index>: \n");
    for(i = 0; i < 6; i++)
    {
      WPU_DUMP("%s",disp_flags[i]);
      if(i%3 == 2)
        WPU_DUMP("\n");
    }
    WPU_DUMP("\n");
    WPU_DUMP("<display_cmd_index>: Hexadecimal value of a WDDI handle or a WP_Display command\n");
    CLI_ShowAllDisplayCmds();
    return;
  }

  for(i = 0; errorlist[i] < 999; i++)
  {
    if(cmd == errorlist[i])
    {
      WPU_DUMP("Function Unavailable\n");
      return;
    }
  }

  for(i = 0; handlelist[i] < 999; i++)
  {
    if(cmd == handlelist[i])
    {
       flags = 1;
    }
  }

  status = WP_Display( wpid_context, cmd, flags, NULL);
}

/***************************************************************
 * Func name  : WT_DebugCallback
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
WP_status WT_DebugCallback(const WP_CHAR *file, const WP_CHAR *function,
                            WP_S32 line, WP_S32 error_id)
{
   if (error_id)
      WPU_DUMP("\n\nfile = %s , line = %d\nfunction = %s\n",file,line,function);
   return WP_OK;
}

/***************************************************************
 * Func name  : formatMem2String
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void formatMem2String(WP_U32 input, WP_CHAR *output)
{
  WP_S32 power = 0;
  WP_U32 temp = 0;
  WP_CHAR factor;
  temp = input;
  while((temp/1024) != 0)
  {
    power++;
    temp = temp/1024;
  }
  switch(power)
  {
    case 0: factor = ' ';
      break;
    case 1: factor = 'K';
      break;
    case 2: factor = 'M';
      break;
    case 3: factor = 'G';
      break;
    default: factor = '?';
      break;
  }
 sprintf(output,"%d %cb",temp, factor);
}

/***************************************************************
 * Func name  : CLI_PrintMemoryAvailable
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_PrintMemoryAvailable (WP_CHAR *StrPrm)
{
  WP_U32 bus, bank, left;
  WP_CHAR memleft[12];
  WP_S32 res;
  WP_status status = WP_OK;
  res = sscanf(StrPrm, "%*d %d %d",&bus, &bank);
  WP_ControlRegister(WP_DEBUG_CALLBACK_FILE_LINE_ERROR, NULL);
  switch (res) {
   case 0:
   case -1:
    for(bus = 0; bus < WP_BUS_NONE; bus++)
    {
      bank = 0;
      status = WP_OK;
      while(status == WP_OK)
      {
        status = WP_MemoryAvailable(wpid_context, bus, bank, &left);
        if(status == WP_OK)
        {
          formatMem2String(left,memleft);
          WPU_DUMP("Memory left in Bus: %d, Bank %d: %s bytes\n", bus, bank++, memleft);
        }
      }
    }
    break;
   case 1:
      bank = 0;
      while(status == WP_OK)
      {
        status = WP_MemoryAvailable(wpid_context, bus, bank, &left);
        if(status == WP_OK)
        {
          formatMem2String(left,memleft);
          WPU_DUMP("Memory left in Bus: %d, Bank %d: %d bytes\n", bus, bank++, left);
         }
      }
    break;
   case 2:
    WP_ControlRegister(WP_DEBUG_CALLBACK_FILE_LINE_ERROR, WT_DebugCallback);
    status = WP_MemoryAvailable(wpid_context, bus, bank, &left);
    if(status == WP_OK)
    {
      formatMem2String(left,memleft);
      WPU_DUMP("Memory left in Bus: %d, Bank %d: %d bytes\n", bus, bank, left);
    }
    break;
   default:
    WPU_DUMP("Invalid number of arguments\n\n");
    WPU_DUMP("Usage: <bus_index> <bank>\n\n");
    WPU_DUMP("Parameters: \n");
    WPU_DUMP("<bus_index>: bus index number\n\n");
    WPU_DUMP("<bank>: memory bank number\n");
    break;
  }
  WP_ControlRegister(WP_DEBUG_CALLBACK_FILE_LINE_ERROR, WT_DebugCallback);
}

/***************************************************************
 * Func name  : CLI_ListQnodes
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_ListQnodes()
{
  WP_status status;
  status=WP_Display(wpid_context, WP_DISPLAY_QNODE, WP_DISPLAY_RAW_REGISTERS, NULL);
}

/***************************************************************
 * Func name  : CLI_WP_QNodeStatus
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_WP_QNodeStatus(WP_CHAR *StrPrm)
{
  WP_U32 qnode, result;
  WP_S32 res;
  WP_status status;
  res = sscanf(StrPrm,"%*d 0x%x", &qnode);
  if(res < 1)
  {
    WPU_DUMP("Invalid number of arguments\n\n");
    WPU_DUMP("Usage: <qnode_handle>\n\n");
    WPU_DUMP("Parameters: \n");
    WPU_DUMP("<qnode_handle>: Qnode Handle in Hexadecimal format,\
 See Qnode display menu option 2, \"List all Qnode handles\"\n\n");
    return;
  }
  status = WP_QNodeStatus(qnode,WP_QNODE_STATUS_FIFO,&result);
  F_menu_TermOnError(status, "WP_QnodeStatus", __LINE__);
  WPU_DUMP("Number of buffers available: 0x%08X\n", result);

}

/***************************************************************
 * Func name  : CLI_TRS_Timeout
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_TRS_Timeout()
{
  WPU_DUMP("Function not yet supported\n");
}

/*****************************************************************************
 *
 * Function: WPV_DebugAreaCallback
 *
 * Purpose: Prints the debug area.  Normally used for dps trace information.
 *
 * wpid       winpath ID
 * size       size in bytes of the area
 * host_ptr   pointer to the start of the debug block
 *
 * Outputs:
 *    prints the block
 *
 * Return Value:
 *     None
 *
 * Called by:
 *
 *
 ****************************************************************************/
void WT_ExampleDebugAreaCallback(WP_U32 wpid, WP_U32 size_allocated, WP_U32 *host_ptr)
{

  WP_U32 size = 0x100;
  WP_U32 count;
  WP_U32 line_length =8;

  if(size> size_allocated)
    size = size_allocated;
  WPU_DUMP("\n Winpath %d, start of debug block\
 0x%8.8x,size 0x%x bytes\n", wpid, (WP_U32) host_ptr, size);
  for (count = 0 ; count < (size/line_length) ; count++)
    {
      if (!(count % line_length))
        WPU_DUMP("\n%8.8x (%.3d) : ", (WP_U32) host_ptr,count*line_length);

      WPU_DUMP("0x%8.8x ",*host_ptr++);
    }
  WPU_DUMP("\n");
}

void CLI_DebugAreDumpPartial (WP_CHAR *StrPrm)
{
  WP_status status;

  status = WP_ControlRegister(WP_DEBUG_CALLBACK_DEBUG_AREA_LOG, &WT_ExampleDebugAreaCallback);
  menu_terminate_on_error(status, "WP_ControlRegister()",__LINE__);
  status = WP_DebugAreaLog(0,WP_B_DEBUG);
  menu_terminate_on_error(status, "WP_DebugAreaLog()",__LINE__);
}

void CLI_DebugAreDumpFull (WP_CHAR *StrPrm)
{
  WP_status status;

  status = WP_ControlRegister(WP_DEBUG_CALLBACK_DEBUG_AREA_LOG, &WPL_DebugAreaCallback);
  menu_terminate_on_error(status, "WP_ControlRegister()",__LINE__);
  status = WP_DebugAreaLog(0,WP_B_DEBUG);
  menu_terminate_on_error(status, "WP_DebugAreaLog()",__LINE__);
}



void CLI_gen_helpfile(void)
{
  WPU_DUMP("\n*********************************************************\n");
  WPU_DUMP("   DEBUG MENU GENERAL HELP FILE");
  WPU_DUMP("\n*********************************************************\n");
  WPU_DUMP("I. Parameter input: Parameter inputs to menu options \
are read together with the user's command input.\n\n");
  WPU_DUMP("   Ex: Example Menu \n   1. WinUtil Cmd: md (Memory Display)\n   \
2. WP_SysCmd: Get TRS Allocation\n");
  WPU_DUMP("   \n   Command: \n   \"1 0x10000000 :4\" will execute \n  \
\"md 0x10000000 :4\"\n\n");
  WPU_DUMP("II. WinUtil Cmds can access the WinUtil help files with\
 the \"-help\" parameter\n");
  WPU_DUMP("   Ex: To view \"md\" help file from Example Menu given above, \
enter command: \"1 -help\",\n\n");
  WPU_DUMP("III. Commands that require parameters can display the parameter \
format if no parameters provided\n");
  WPU_DUMP("   Ex: To view the parameters for \"Get TRS Allocation\" \
from Example Menu given above,\n");
  WPU_DUMP("   enter command \"2\" with no parameters\n\n\n");
}

/***************************************************************
 * Func name  : CLI_WinUtilWFChelp
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_WinUtilWFChelp(void)
{
  WPU_WinutilCall("wfc -help");
}

/***************************************************************
 * Func name  : CLI_WinUtilTBhelp
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_WinUtilTBhelp(void)
{
  WPU_WinutilCall("tb -help");
}

/***************************************************************
 * Func name  : SetDebugUpperMenu
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/

void SetDebugUpperMenu(Y_MenuEntry *MenuPtr)
{
  V_Menu_Debug[0].Node.Menu = MenuPtr;
}



/*----------------------ALL MENUS -----------------------------*/

Y_MenuEntry V_Menu_DebugBaseDump[] =
  {
    {3     , TRUE, " Debug Base Dump",          {V_Menu_MemAndLocks}},
    {K_Leaf, TRUE, " Debug Area: only header",  {CLI_DebugAreDumpPartial}},
    {K_Leaf, TRUE, " Debug Area: full dump " ,  {CLI_DebugAreDumpFull}},
    {K_Leaf, HELP, " help",                     {CLI_gen_helpfile}}
  };


Y_MenuEntry V_Menu_Performance[] =
{
   {PERF_MENU_SIZE, TRUE, "Performance menu",      {V_Menu_Debug}},
   {K_Leaf, TRUE,   "trsu (WinUtil Cmd: Display Thread Utilization)",\
      {CLI_WinUtilThreadUtilization}},
#ifdef WP_HW_WINPATH3
   {K_Leaf, TRUE,  "sysu (WinUtil Cmd: Display Bus Utilization)",\
      {CLI_WinUtilWingineBusUtilization}},
#endif
   {K_Leaf, TRUE_M,"wgu  (WinUtil Cmd: Display Wingine Utilization)",\
      {CLI_WinUtilWingineUtilization}},
   {K_Leaf, TRUE, "WP_SysCmd: Get TRS Allocation", {CLI_SYSCMD_GetTRS}},
   {K_Leaf, TRUE, "WP_SysCmd: Set TRS Allocation", {CLI_SYSCMD_SetTRS}},
   {K_Leaf, HELP, "help",                          {CLI_gen_helpfile}}
};


Y_MenuEntry V_Menu_FatalError[] =
{
   {FATAL_MENU_SIZE , TRUE, "Fatal Error menu: Bus Error Functions",{V_Menu_Debug}},
#ifdef linux
   {K_Leaf, TRUE,   "Display all info",       {CLI_FatalError_DisplayAll}},
   {K_Leaf, TRUE_M, "Display all info <file>", {CLI_FatalError_FileDump}},
#else
   {K_Leaf, TRUE_M, "Display all info",       {CLI_FatalError_DisplayAll}},
#endif
   {K_Leaf, TRUE,   "Print DTCR registers", {CLI_DisplayDTCR}},
   {K_Leaf, TRUE_M, "Print Bus ECC registers",       {CLI_Bus_ECCcheck}},
   {K_Leaf, TRUE,   "Print TA PC current value",     {CLI_DisplayTA_PCs}},
#ifdef WP_HW_WINPATH3
   {K_Leaf, TRUE_M, "Print AA PCT configuration",    {CLI_DisplaySA_PCT_Table}},
#else
   {K_Leaf, TRUE_M, "Print SA PCT configuration",    {CLI_DisplaySA_PCT_Table}},
#endif
   {K_Leaf, TRUE,   "wfc reg <all>  (WinUtil: WMM Check Corrupted FIFO by HW)",\
     {CLI_DisplayFifo_CorruptedNumbers}},
   {K_Leaf, TRUE_M, "fmum           (WinUtil Cmd: Check FMU put error)",\
     {CLI_WinUtilFmum}},
   {K_Leaf, TRUE,   "hbm            (WinUtil: Bus Error Check)",\
     {CLI_WinUtilBusErrCheck}},
   {K_Leaf, TRUE,   "tb dump        (WinUtil: Wingine Trace Dump)",\
     {CLI_WinUtilTraceDump}},
#ifdef linux
   {K_Leaf, TRUE,   "tb dump <file> (WinUtil Cmd: Trace Dump to file)",\
     {CLI_WinUtilTraceDumpFile}},
#endif
   {K_Leaf, TRUE,   "trsa           (WinUtil: TRS Serial Allocation)",\
     {CLI_WinUtilTRS_SerialAllocation}},
   {K_Leaf, TRUE,   "tast           (WinUtil: Print Current TRS status)",\
     {CLI_WinUtilCurrentTRSStatus}},
   {K_Leaf, TRUE_M, "trs            (WinUtil: TRS content Dump)",\
     {CLI_WinUtilTRSDump}},
   {K_Leaf, TRUE,   "taot           (WinUtil: Print TA Order Registers)",\
     {CLI_WinUtilTAOrderRegisters}},
   {K_Leaf, TRUE,   "taom           (WinUtil: Order Monitor Display)",\
     {CLI_WinUtilOrderMonitorDisplay}},
   {K_Leaf, TRUE_M, "Versions and Build times",  {CLI_All_VersionBuildTime}},
   {K_Leaf, HELP,   "help",                      {CLI_gen_helpfile}}
};

Y_MenuEntry V_Menu_Qnode[] =
{
 {3,      TRUE, "Qnode display menu", {V_Menu_MemAndLocks}},
 {K_Leaf, TRUE, "WP_QnodeStatus", {CLI_WP_QNodeStatus}},
 {K_Leaf, TRUE, "List All Qnode handles", {CLI_ListQnodes}},
 {K_Leaf, HELP, "help", {CLI_gen_helpfile}}
};

Y_MenuEntry V_Menu_MemAndLocks[] =
{
   {9    , TRUE, "Memory & Locks menu",                      {V_Menu_Debug}},
   {K_Leaf, TRUE, "Get table from TA base  <baseIndex> <entrySize> <entryIndex>",\
     {CLI_WinUtilDisplayTA_Table}},
   {K_Leaf, TRUE_M, "tab  (WinUtil: Print TA bases)",         {CLI_WinUtilTA_Bases}},
   {K_Menu, TRUE, "Debug base display menu",                  {V_Menu_DebugBaseDump}},
   {K_Menu, TRUE, "Qnode display menu",                       {V_Menu_Qnode}},
   {K_Leaf, TRUE_M, "bmap (WinUtil: Print Bus Mapping Table)",{CLI_WinUtilBmap}},
   {K_Leaf, TRUE, "Print Memory Available",                   {CLI_PrintMemoryAvailable}},
   {K_Leaf, TRUE, "Enable WDDI Memory Allocation Log",        {CLI_WDDI_Allocation}},
   {K_Leaf, TRUE_M, "Disable WDDI Memory Allocation Log",     {CLI_WDDI_AllocationEND}},
   {K_Leaf, HELP, "help",                                     {CLI_gen_helpfile}}
};

Y_MenuEntry V_Menu_DisplayLogAndVersions[] =
{
   {10    , TRUE, "Display, log & versions menu",   {V_Menu_Debug}},
   {K_Leaf, TRUE, "WP_Display Cmd",                 {CLI_DisplayCmd}},
   {K_Leaf, TRUE_M,"WP_Display System Info",         {CLI_Display_System}},
   {K_Leaf, TRUE, "Set log level",                  {CLI_SetLogLevel}},
   {K_Leaf, TRUE, "Enable API level log",           {CLI_APILevel_StartLog}},
   {K_Leaf, TRUE, "Disable API level log",          {CLI_APILevel_EndLog}},
   {K_Leaf, TRUE, "Enable WPI level log",           {CLI_WPILevel_StartLog}},
   {K_Leaf, TRUE_M, "Disable WPI level log",        {CLI_WPILevel_EndLog}},
   {K_Leaf, TRUE, "WDDI version, build time",       {CLI_WDDI_VersionBuildTime}},
   {K_Leaf, TRUE_M, "WinUtil Version, build time",  {CLI_WinUtilDebugVersionInfo}},
   {K_Leaf, HELP, "help",                           {CLI_gen_helpfile}}
};

Y_MenuEntry V_Menu_WMMFIFOSpecific[] =
{
   {5     , TRUE, "WMM FIFO specific menu",{V_Menu_WinUtilCmd}},
   {K_Leaf, TRUE, "wfc list     (WinUtil: Check FIFO for correctness)",\
     {CLI_DisplayFifo_StartEnd}},
   {K_Leaf, TRUE, "wfc reg      (WinUtil: Check FIFO for corruption)",\
     {CLI_DisplayFifo_corruption}},
   {K_Leaf, TRUE, "wfc base     (WinUtil: Set new FIFO base)",\
     {CLI_SetFifo_base}},
   {K_Leaf, TRUE, "wfc offset   (WinUtil: Set new FIFO offset)",\
     {CLI_SetFifo_offset}},
   {K_Leaf, HELP, "help",{CLI_WinUtilWFChelp}}
};

Y_MenuEntry V_Menu_TraceSpecific[] =
{
   {TRACE_MENU_SIZE, TRUE, "Trace specific menu",           {V_Menu_WinUtilCmd}},
   {K_Leaf, TRUE, "tb init         (WinUtil: Enable trace)",{CLI_WinUtilTraceStart}},
   {K_Leaf, TRUE, "tb init -serial (WinUtil: Enable trace on specific serial)",\
     {CLI_WinUtilTraceEnable_Serial}},
   {K_Leaf, TRUE, "List Trace Serials",{CLI_WinUtilDisplayAllTraceSerials}},
   {K_Leaf, TRUE, "tb status       (WinUtil: Print Trace options status)",\
     {CLI_WinUtilTraceOptionsStatus}},
   {K_Leaf, TRUE, "tb dump         (WinUtil: Trace Dump)", {CLI_WinUtilTraceDump}},
#ifdef linux
   {K_Leaf, TRUE, "tb dump <file>  (WinUtil: Trace Dump to file)",\
    {CLI_WinUtilTraceDumpFile}},
#endif
   {K_Leaf, HELP, "help",{CLI_WinUtilTBhelp}}
};

Y_MenuEntry V_Menu_WinUtilCmd[] =
{
   {WU_ALL_MENU_SIZE, TRUE, "All WinUtil Commands",{V_Menu_Debug}},
   {K_Leaf, TRUE,    "WinUtil(free text input)",{CLI_WinUtilFreeTxt}},
   {K_Leaf, TRUE,    "trsu   (WinUtil: Display TRS Utilization)",\
     {CLI_WinUtilThreadUtilization}},
   {K_Leaf, TRUE,    "wgu    (WinUtil: Display Wingine Utilization)",\
    {CLI_WinUtilWingineUtilization}},
#ifdef WP_HW_WINPATH3
   {K_Leaf, TRUE_M,  "sysu   (WinUtil: Display Bus Utilization)",\
    {CLI_WinUtilWingineBusUtilization}},
#endif
   {K_Leaf, TRUE,    "dmap   (WinUtil: Print Device Mapping Table)",{CLI_WinUtilDmap}},
   {K_Leaf, TRUE,    "bmap   (WinUtil: Print Bus Mapping Table)",{CLI_WinUtilBmap}},
   {K_Leaf, TRUE,    "smap   (WinUtil: Print Serial Mapping Table)",{CLI_WinUtilSA_TRSLimits}},
   {K_Leaf, TRUE_M,  "hbm    (WinUtil: Bus Error Check)",{CLI_WinUtilBusErrCheck}},
   {K_Leaf, TRUE,    "map    (WinUtil: Print Register Mappings)",{CLI_WinUtilMemoryMap}},
   {K_Leaf, TRUE,    "mc     (WinUtil: Memory Check)",{CLI_WinUtilMemoryCheck}},
   {K_Leaf, TRUE,    "md     (WinUtil: Memory Display)",{CLI_WinUtilMemoryDisplay}},
   {K_Leaf, TRUE,    "mf     (WinUtil: Memory Fill)",{CLI_WinUtilMemoryFill}},
   {K_Leaf, TRUE_M,  "mm     (WinUtil: Memory Modify)",{CLI_WinUtilMemoryModify}},
   {K_Leaf, TRUE,    "tab    (WinUtil: Print TA base registers)",{CLI_WinUtilTA_Bases}},
   {K_Leaf, TRUE,    "taom   (WinUtil: Print TA order monitors)",\
     {CLI_WinUtilOrderMonitorDisplay}},
   {K_Leaf, TRUE,    "taot   (WinUtil: Print TA order registers)",\
     {CLI_WinUtilTAOrderRegisters}},
   {K_Leaf, TRUE,    "trs    (WinUtil: Print TRS registers)",{CLI_WinUtilTRSDump}},
   {K_Leaf, TRUE,    "tast   (WinUtil: Print Current TRS status)",\
     {CLI_WinUtilCurrentTRSStatus}},
   {K_Leaf, TRUE_M,  "trsa   (WinUtil: Print TRS Serial Allocation)",\
     {CLI_WinUtilTRS_SerialAllocation}},
   {K_Leaf, TRUE,    "fmum   (WinUtil: Check FMU address errors)",{CLI_WinUtilFmum}},
#ifdef WP_HW_WINPATH3
   {K_Leaf, TRUE,    "freq   (WinUtil: Change Bus frequency)",{CLI_WinUtilFreq}},
#endif
   {K_Leaf, TRUE,    "core   (WinUtil: Dump memory partition to file)",{CLI_WinUtilCoreDump}},
   {K_Leaf, TRUE_M,  "ver    (WinUtil: Print Version and Buildtimes)",\
     {CLI_WinUtilDebugVersionInfo}},
   {K_Menu, TRUE,    "wfc menu (WinUtil: WMM FIFO specific menu)",{V_Menu_WMMFIFOSpecific}},
   {K_Menu, TRUE,    "tb menu  (WinUtil: Trace specific menu)",{V_Menu_TraceSpecific}},
   {K_Leaf, HELP,    "help",{CLI_WinUtilHelp}}

};


 Y_MenuEntry V_Menu_Debug[] =
  {
    {14    , TRUE,   "Main Debug menu",{V_MainMenu}},
    {K_Menu, TRUE_M, "Fatal error menu",{V_Menu_FatalError}},
    {K_Leaf, TRUE,   "map  (WinUtil: Memory Map)", {CLI_WinUtilMemoryMap}},
    {K_Leaf, TRUE,   "md   (WinUtil: Memory Display)",{CLI_WinUtilMemoryDisplay}},
    {K_Leaf, TRUE_M, "mm   (WinUtil: Memory Modify)",{CLI_WinUtilMemoryModify}},
    {K_Leaf, TRUE,   "Get table from TA base  <baseIndex> <entrySize> <entryIndex>",\
     {CLI_WinUtilDisplayTA_Table}},
    {K_Leaf, TRUE_M, "tab  (WinUtil: Print TA Bases)",{CLI_WinUtilTA_Bases}},
    {K_Leaf, TRUE,   "WP_Display Cmd",{CLI_DisplayCmd}},
    {K_Leaf, TRUE_M, "WinUtil(free text)", {CLI_WinUtilFreeTxt}},
    {K_Menu, TRUE,   "Performance menu",{V_Menu_Performance}},
    {K_Menu, TRUE,   "Memory & Locks menu",{V_Menu_MemAndLocks}},
    {K_Menu, TRUE,   "Display,Log & Versions menu",{V_Menu_DisplayLogAndVersions}},
    {K_Menu, TRUE_M, "Advanced WinUtil Cmd menu",{V_Menu_WinUtilCmd}},
    {K_Leaf, TRUE_M, "Set Winpath ID 0-4",{CLI_setWPID}},
    {K_Leaf, HELP,   "help",{CLI_gen_helpfile}},
 };
/*---------------------------------------ALL MENUS ----------------------------------*/


#endif
