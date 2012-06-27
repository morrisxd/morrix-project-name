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
 #ifdef WT_HW_WINPATH3_SL
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
WP_status WTI_DebugAlloc(const WP_CHAR *file, WP_U32 line, WP_U32 bus,WP_U32 size, WP_U32 alignment, const WP_CHAR *purpose);
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
