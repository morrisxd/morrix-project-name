/*************************************************************
 * (C) Copyright 2001-2010, Wintegra. All rights reserved.
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

/*

        GigE Transmit and Recieve using the host

                This test creates a transparent bridge system and 2 GigE ports and
                devices.

                The test uses WP_HostSend to send a packet out a GigE port using an
                interworking Tx channel is used.

                No learning is done.  No mac addresses are statically inserted
                into the forwarding database.  This means that when a packet is
                received on the GigE ports the packet will always go to the default
                flow aggregation of the bridge port.

                The default aggregation on each bridge port (one for each GigE)
                are set to forward the packets to a host interworking recieve channel.

                When loopback mode is set each GigE port will recieve its own packets.
                When loopback is not set the test simply sends packets and then will
                poll for any packets received on that port.

                The test uses a simple CLI to set loopback mode, transmit and poll for receive
                packets and print statistics.

                The test also starts a scheduling function.  Controlled by START_SCHEDULER.
                The scheduling function is not required for GigE tx or rx.  But it
                does allow the debug function (trace init and trace dump) to show that
                the dps is running, in case no GigE packets are sent or received.

                The interworking host send function sends directly to the flow aggregation.
                By changing the control field of the data unit you can send to the interworking
                 system itself or to a bridge port.

                For a bridge port --

                data_unit.control = WP_HT_IW_B_MODE_FIELD(WP_HT_IW_B_FAST_MODE)|
                         WP_HT_IW_B_VA_DEST_TYPE_FIELD(WP_HT_IW_B_VA_DESTINATION_BPORT)|
                         WP_HT_IW_FSP_VLAN_BPORT_FIELD(iw_bridge_port_handle)|
                         WP_HT_IW_CRC_FIELD(0);

                For sending to the bridge interworking system itself --

                data_unit.control = WP_HT_IW_B_MODE_FIELD
                                      (WP_HT_IW_B_NORMAL_MODE)| WP_HT_IW_CRC_FIELD(0);

*/


/*#define MENU_DEBUG_MODE*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef USE_SOCKET
#include <unistd.h>
#include "sockint.h"
#endif

#include "api/wp_wddi.h"
#include "api/wp_host_io.h"

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
#include "wp_sim.h"

#include "wpi_veneer.h"
#include "wpx_app_data.h"
#include "wpx_gei_phy.h"
#include "wpx_pin_mux_selector.h"


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


void WPE_PrintIWPortStats(WP_handle iw_port_handle, WP_route_port_stats * iwport_stats);
void WPE_GetPortStatistics(WP_handle iw_port_handle, WP_route_port_stats * iwport_stats);
void WPE_PrintFlowStats(WP_handle agg, WP_iw_flow_stats* iw_flow_stats);
void WPE_GetFlowStatistics(WP_handle agg, WP_iw_flow_stats* iw_flow_stats);
void WPE_PrintStatHsEnet(WP_handle hs_device, WP_stats_enet* enet_stats);
void WPE_GetDeviceStatistics(WP_handle hs_device, WP_stats_enet* enet_stats);

#include <wpx_board_data.h>
#include "wpx_enet_phy.h"
void terminate_on_error(WP_handle, WP_CHAR *);
char *STR_MENU_HDR = {"  GigE Loopback Test"};
/*****************************************************************************
 *  Scheduler Creation -    WDDI call to create the Scheduler based on the
 *                          defined earlier in the application
 * THIS IS NOT NEEDED by the application.  It is here to show if the DPS
 * if functioning by showing threads utilized in the debug utilities.
 * This can be used to verify that the dps is actually running in case
 * there is no tx/rx of enet packets.
 *****************************************************************************/
#define START_SCHEDULER 1
/****************************************************************************
 * Miscelaneous defines
 ****************************************************************************/
#define DATA_LENGTH 256

/****************************************************************************
 * ENET Interface defines
 ****************************************************************************/
#define DEBUG_MODE
#ifdef WP_HW_WINPATH3
  #ifndef WP_BOARD_WDS3_SL
  #define WT_ENET_INTERFACE_1 WP_PORT_ENET8
  #define WT_ENET_INTERFACE_2 WP_PORT_ENET13
  #else
  #define WT_ENET_INTERFACE_1 WP_PORT_ENET9
  #define WT_ENET_INTERFACE_2 WP_PORT_ENET10
  #endif
#endif

#ifdef WP_HW_WINPATH2
  #define WT_ENET_INTERFACE_1 WP_PORT_ENET1
  #define WT_ENET_INTERFACE_2 WP_PORT_ENET2
#endif

/*****************************************************************************
 *  dps_ProgramImage[];
 *  dps_PC_Table_Init[];
 *
 *  Program Image is a binary file that contains the program (data path software)
 *  to be loaded into the internal WinComm program memory space
 *
 *  PC Table is a binary file that contains an array of indexes into the dps image.
 *  These indexes show the starting address for different routines
 *  For instance the routine to handle an ATM TDM Receive will be at a particular
 *  offset within the DPS image.  WDDI will use this value when creating an ATM
 *  TDM Receive channel.
 *
 *****************************************************************************/
extern WP_U32 dps_ProgramImage[];
extern WP_U16 dps_PC_Table_Init[];

#define DEV1_TAG 0xAB
#define DEV2_TAG 0xDE
void thread_rx_dev1_init(void* arg);
void thread_rx_dev2_init(void* arg);

void rx_data(WP_handle dev_handle);
void iw_host_tx_data(WP_handle , WP_handle , WP_handle);
void IwHostTxChannel(WP_handle);

void SetDebugUpperMenu(Y_MenuEntry *MenuPtr);
void InitDrivers(void);
void InitBoard(void);
void initMenuStrings(void);
void CLI_Normal_Loopback(void);
void CLI_Internal_Loopback(void);
void CLI_SendPacket_Interface1(void);
void CLI_SendPacket_Interface2(void);
void CLI_PollPacket_Interface1(void);
void CLI_PollPacket_Interface2(void);
void CLI_PrintStatistics(void);
void CLI_ProgramQuit(void);
WP_CHAR appname[] = "Host Send and Receive for GigE port";

#define POOL_LIMIT 8
#define SYSTEM_ONE_MAX_FLOWS 5

static char sendPacket1[40], sendPacket2[40], pollPacket1[40], pollPacket2[40];
extern Y_MenuEntry V_Menu_Debug[];
extern Y_MenuEntry V_MainMenu[];

#ifdef DEBUG_MODE
void F_DisplayFlowStats(WP_handle agg)
{
   WP_iw_flow_stats iw_flow_stats;

   WPE_GetFlowStatistics(agg,&iw_flow_stats);
   WPE_PrintFlowStats(agg,&iw_flow_stats);
}


void WPE_DisplayStatHsEnet(WP_handle hs_device)
{
   WP_stats_enet enet_stats;

   WPE_GetDeviceStatistics(hs_device,&enet_stats);
   WPE_PrintStatHsEnet(hs_device,&enet_stats);
}

void WPE_DisplayPortStats(WP_handle iw_port_handle)
{
  WP_route_port_stats iwport_stats;

  WPE_GetPortStatistics(iw_port_handle,&iwport_stats);
  WPE_PrintIWPortStats(iw_port_handle,&iwport_stats);
}
#endif


WP_iw_agg_hq hq_config[1] =
{
   {
      /* tag */         2,
      /* txfunc */      0,
      /* hq_agg_mode */ WP_IW_HQ_ARP_RARP,
      /* mtu */         1536,
      /* interruptqueue */ WP_IW_IRQT1,
      /* intmode */ WP_IW_INT_DISABLE,
      /* statmode */ WP_IW_STAT_DISABLE,
      /* timestamp_mode */ WP_IW_TIME_STAMP_DISABLE,
      /* ov_pool_mode */ 0,
      /* fbp_drop_threshold */ 0,
      /*tx_direction;*/0,
      /*flow_analyze;*/WP_IW_FLOW_PASS,
      /*force_analyze;*/WP_IW_FORCE_NO,
      /*l4_checksum_recal;*/WP_IW_L4_NO_CHECKSUM,
      /*nexthop;*/0,
      /*flow_agg_type;*/ WP_IW_AGG_PRIMARY
   }
};

   WP_iw_agg_bridging tx_agg_h[1] =
      {
         {
            /*tag*/ 2,
            /*txfunc*/ 0,
            /*input_bport*/0,
            /*rfcs*/WP_IW_RFCS_ENABLE,
            /*l2_header_insert_mode;*/WP_IW_L2H_INSERT_DISABLE,
            /*vlan_tag_mode*/WP_IW_VLAN_TAG_DISABLE,
            /*interruptqueue;*/WP_IW_IRQT1,
            /*error_pkt_mode*/WP_IW_ERRPKT_DISCARD,
            /*intmode;*/WP_IW_INT_ENABLE,
            /*statmode;*/WP_IW_STAT_ENABLE,
            /*timestamp_mode;*/WP_IW_TIME_STAMP_DISABLE,
            /*ov_pool_mode */ WP_IW_OV_POOL_DISABLE,
            /*fbp_drop_threshold;*/0,
            /*replace_vlan_tag*/WP_IW_REPLACE_VTAG_DISABLE,
            /*vlan_tag*/0,
            /*vpmt_handle */0,
            /*mtu;*/1536,
            /*prefix_length */ 0,
            /*prefix_header[28];*/ {0,0,0,0,0,0,0,
                                    0,0,0,0,0,0,0,
                                    0,0,0,0,0,0,0,
                                    0,0,0,0,0,0,0},
            /*policer_enable*/ WP_IW_POLICER_DISABLE,
            /**policer_config;*/NULL,
            /*cong_mode;*/WP_IW_CONGESTION_DISABLE,
            /**cong_threshold_param;*/NULL
         }
      };


/*****************************************************************************
 *      WP_int_queue_table      Interrupt queue configuration used with WP_SysInit
 *
 *  The WDDI supports 4 interrupt event queues. The following structure has
 *  four pairs of elements: size and ratio.
 *  This structure is used as part of the WinPath context structure.
 *
 *      Size --
 *              Number of entries in interrupt queue table. If the table becomes full before
 *              events have been serviced, subsequent events are not recorded in the table.
 *              Therefore, the size of table should be greater than:
 *              (avg. rate of events / avg. frequency of interrupt servicing)
 *
 *      Ratio --
 *              number of event occurrences to interrupts generated
 *              For each n event occurrences associated with this interrupt queue, an
 *              interrupt is raised. If the ratio is set to 0 then the queue is
 *              established as a polled queue instead of in interrupting queue.
 *
 *****************************************************************************/
WP_int_queue_table int_queue_tables = {{{24, 1}, {10, 1}, {10, 1}, {10, 1}}};


WP_atm_global atm_params[1] =
{
   {
      /* max_pm_tables */ 0,
      /* max_ubrplus_channels */ 0,
      /* max_upc_tables */ 0,
      /* max_rx_cids */ 0,
      /* max_tx_cids */ 10,
      /* max_cid_range */ 10,
      /* AAL2 QSR bus */ WP_BUS_NONE,
      /* AAL2 QSR bank */ APP_BANK_HOST
   }
};


WP_iw_global  max_iw[1] =
{
    {
       /* max_iw_aggregations */ 2 * SYSTEM_ONE_MAX_FLOWS,
       /* max_r_iw_systems;*/ 2,
       /* max_directmap_enet_systems;*/ 0,
       /* max_b_iw_systems;*/ 1,
       /* max_m_iw_systems;*/ 0,
       /* max_vlan_priority_maps; */ 0,
       /* iw_bkgnd_fifo_size */0,
       /* cong_pt       */NULL,
       /* iw_host_limits */ {64,0,0,4,0},
       /* mpls_config*/ {0,0},
    /* iw_modes;*/
      {  /*policer_mode;*/                      WP_IW_POLICER_DISABLE,
         /*statistics_bundle_stat_mode;*/       WP_IW_STATISTICS_BUNDLE_STAT_DISABLE,
         /*l2_prefix_ext_mode;*/                WP_IW_L2_HEADER_EXTENSION_DISABLE,
         /*enhanced_flow_stat_mode;*/           WP_IW_ENHANCED_FLOW_STAT_DISABLE,
#ifdef DEBUG_MODE
         /*flow_stat_mode;*/                    WP_IW_FLOW_STAT_ENABLE,
#else
         /*flow_stat_mode;*/                    WP_IW_FLOW_STAT_DISABLE,
#endif
         /*fr_tx_flow_stat_mode */              WP_IW_FR_TX_FLOW_STAT_DISABLE,
         /*mfc_alloc_mode; */                   WP_IW_MFC_ALLOC_DISABLE,
         /*Learning_queue_mode */               WP_IW_LEARNING_DISABLE
      },
    /* max_ch_agg_stat;*/0,
    /* max_fr_s_iw_systems;*/ 0,
     /* max_bridging_ports */ 20

    }
};


/*****************************************************************************
 *
 *  The context structure is used to initialize certain global resources
 *  and setup memory for each winpath in the system.  See Chapter 4,
 *  section "WinPath Initialization (WP_SysInit)"
 *  in the WinPath Device Driver Interface manual for detailed information.
 *
 *  Each WinPath in the system will have its own unique context structure.
 *
 *
 *              typedef struct {
 *                      WP_U32 max_qnodes;
 *                      WP_bus_bank virtual_bus[WP_VIRTUAL_BUSES];
 *                      WP_U32 * dps_image[2];
 *                      WP_U16 * dps_features[2];
 *                      WP_int_queue_table * int_queue_data;
 *                      WP_pool_global pool_limit;
 *                      WP_atm_global * atm_data;
 *                      WP_iw_global * iw_data;
 *                      WP_U8 schedule_api
 *                      } WP_context;
 *
 *      Field Name      Description of field
 *      ----------      --------------------
 *      max_qnodes      Maximum permitted number of queue node objects in the system
 *      virtual_bus     An array of structures providing a mapping of each virtual
 *                      WinPath bus to a physicalmemory bus and bank.
 *      dps_image       Pointer to start of DPS code image for each pair of WinGines
 *      int_queue_data  Pointer to the interrupt queue table array
 *      pool_limit      Specifies the maximum number of buffer pools in the system
 *      atm_data        Pointer to the global atm data structure
 *      iw_data         Pointer to the global interworking data structure
 *
 *****************************************************************************/


/* An array of one WP_context structure for each WinPath */
WP_context context[1] =
{
   {7, /* qnodes */
    /* the first four assignments are mandatory bus assignments */
    {
       /* WP_VB_PARAM = 0,                      */{WP_BUS_PARAM, APP_BANK_PARAM},
       /* WP_VB_PACKET,                         */{WP_BUS_PACKET, APP_BANK_PACKET},
       /* WP_VB_INTERNAL,                       */{WP_BUS_INTERNAL, APP_BANK_INTERNAL},
       /* WP_VB_HOST,                           */{WP_BUS_HOST, APP_BANK_HOST},
       /* WP_VB_INTQUEUE_DEBUG,                 */{WP_BUS_HOST, APP_BANK_HOST},
       /* WP_VB_TIMESTAMP,                      */{WP_BUS_HOST, APP_BANK_HOST},
       /* WP_VB_CAS,                            */{WP_BUS_NONE, 0},
       /* WP_VB_TRANS_TSAT,                     */{WP_BUS_NONE, 0},
       /* WP_VB_TPT,                            */{WP_BUS_INTERNAL, APP_BANK_INTERNAL},
       /* WP_VB_CW,                             */{WP_BUS_INTERNAL, APP_BANK_INTERNAL},
       /* WP_VB_ASU_TXQUEUE,                    */{WP_BUS_PARAM, APP_BANK_PARAM},
#ifndef WP_BOARD_WDS3_SL
       /* WP_VB_STATISTICS,                     */{WP_BUS_PACKET, APP_BANK_PACKET},
#else
       /* WP_VB_STATISTICS,                     */{WP_BUS_PARAM, APP_BANK_PARAM},
#endif
       /* WP_VB_ATM_ADDLOOKUP,                  */{WP_BUS_PARAM, APP_BANK_PARAM},
       /* WP_VB_ATM_CPT_EBMT_EXTHEADER,         */{WP_BUS_PARAM, APP_BANK_PARAM},
       /* WP_VB_ETH_HDLC_CPT,                   */{WP_BUS_PARAM, APP_BANK_PARAM},
       /* WP_VB_FBP_BD,                         */{WP_BUS_PARAM, APP_BANK_PARAM},
#ifndef WP_BOARD_WDS3_SL
       /* WP_VB_FBP_DATA,                       */{WP_BUS_PACKET, APP_BANK_PACKET},
       /* WP_VB_L2_BKGND,                       */{WP_BUS_PACKET, APP_BANK_PACKET},
       /* WP_VB_BRIDGE_LEARNING_DFC_PFMT        */{WP_BUS_PACKET, APP_BANK_PACKET},
#else
       /* WP_VB_FBP_DATA,                       */{WP_BUS_PARAM, APP_BANK_PARAM},
       /* WP_VB_L2_BKGND,                       */{WP_BUS_PARAM, APP_BANK_PARAM},
       /* WP_VB_BRIDGE_LEARNING_DFC_PFMT        */{WP_BUS_PARAM, APP_BANK_PARAM},
#endif
       /* WP_VB_IMA_MLPPP_STATE,                */{WP_BUS_INTERNAL, APP_BANK_INTERNAL},
       /* WP_VB_IMA_MLPPP_BUFFER,               */{WP_BUS_HOST, APP_BANK_HOST},
#ifndef WP_BOARD_WDS3_SL
       /* WP_VB_HASH_IW_LPM_EMC,                */{WP_BUS_PACKET, APP_BANK_PACKET},
       /* WP_VB_IW_MFC_RULES,                   */{WP_BUS_PACKET, APP_BANK_PACKET},
#else
       /* WP_VB_HASH_IW_LPM_EMC,                */{WP_BUS_PARAM, APP_BANK_PARAM},
       /* WP_VB_IW_MFC_RULES,                   */{WP_BUS_PARAM, APP_BANK_PARAM},
#endif
       /* WP_VB_IW_FIWT_QAT,                    */{WP_BUS_PARAM, APP_BANK_PARAM},
       /* WP_VB_IW_MFCPT_DIFFSERV_UPPT_D        */{WP_BUS_INTERNAL, APP_BANK_INTERNAL},
#ifndef WP_BOARD_WDS3_SL
       /* WP_VB_STATISTICS2,                    */{WP_BUS_PACKET, APP_BANK_PACKET},
       /* WP_VB_STATISTICS3,                    */{WP_BUS_PACKET, APP_BANK_PACKET},
#else
       /* WP_VB_STATISTICS2,                    */{WP_BUS_PARAM, APP_BANK_PARAM},
       /* WP_VB_STATISTICS3,                    */{WP_BUS_PARAM, APP_BANK_PARAM},
#endif
       /* WP_VB_IMA_RX_BUS,                     */{WP_BUS_NONE, 0},
       /* WP_VB_PKT_GLOBAL_TCPT,                */{WP_BUS_NONE, 0},
       /* WP_VB_IMA_TX_BUS,                     */{WP_BUS_NONE, 0},
       /* WP_VB_UNASSIGNED9,                    */{WP_BUS_NONE, 0},
       /* WP_VB_UNASSIGNED10,                   */{WP_BUS_NONE, 0},
       /* WP_VB_MCH_DATA,                       */{WP_BUS_NONE,0},
       /* WP_VB_OAM_FM_QUERY,                   */{WP_BUS_NONE,0},
       /* WP_VB_HASH_ON_HEAP,                   */{WP_BUS_NONE,0},
#ifndef WP_BOARD_WDS3_SL
       /* WP_VB_MFC_INTERNAL,                   */{WP_BUS_PACKET, APP_BANK_PACKET},
#else
       /* WP_VB_MFC_INTERNAL,                   */{WP_BUS_PARAM, APP_BANK_PARAM},
#endif
       /* WP_VB_MFR_RAS_TBL,                    */{WP_BUS_NONE,0},
       /* WP_VB_CESOP_DATA,                     */{WP_BUS_NONE,0},
       /* WP_VB_SECURITY_ESE,                   */{WP_BUS_NONE,0},
       /* WP_VB_PDCP_SECURITY_ENTITY,           */{WP_BUS_NONE,0},
       /* WP_VB_POLICER,                        */{WP_BUS_NONE,0},
       /* WP_VB_PCE_LRN,                        */{WP_BUS_NONE,0},
       /* WP_VB_PCEA_PARAM,                     */{WP_BUS_NONE,0},
       /* WP_VB_CONGESTION_TABLE,               */{WP_BUS_NONE,0},
       /* WP_VB_PARAMS_PROPAGATION,             */{WP_BUS_NONE,0},

    },
    {
       dps_ProgramImage, dps_ProgramImage
    },
    {
       dps_PC_Table_Init, dps_PC_Table_Init
    },
    &int_queue_tables, /* Interrupt queue tables */
    POOL_LIMIT,        /* Mixumum number of pools */
    atm_params,        /* ATM global parameters */
    max_iw,            /* Interworking global parameters */
   }
};


#if 0
/* Callback function for a queuenode - Need to define the correct type later*/
WP_S32 QNCallback(WP_S32 status) {
   printf("App: QNCallback: Status returned is %d\n",status);
   return 0;
}
#endif

/* Giga Ethernet port1 configuration */
/* ENET port and device params */
WP_mii_mgmt mii_mgmt_cfg[1] =
{
   {
      1000000000  /* Interface speed, currently unused */
   }
};




WP_enet_filters enet_filters[1] =

{
        {
         /* rx_ucfilter */  WP_ENET_FILTER_DISCARD,
         /* rx_bcfilter */  WP_ENET_FILTER_HOSTTERM,
         /* rx_mcfilter */  WP_ENET_FILTER_HOSTTERM
        }
};



/* Device configuration for Hs_Enet*/
WP_device_enet_ex enet_ex[1] =
{
      {
      /* WP_U8 duplexmode;              */  WP_ENET_FULLDUPLEX,
      /* WP_U8 rx_maclookup_mode;       */  WP_ENET_MACLOOKUP_TABLE,
      /* WP_U8 rx_flowcontrol;          */  WP_ENET_FLOW_ENABLE,
      /* WP_U8 tx_flowcontrol;          */  WP_ENET_FLOW_ENABLE,
      /* WP_U8 rx_addrmode;             */  WP_ENET_ADDR_ACCEPTALL,

#ifdef DEBUG_MODE
      /* phystatmode */ WP_ENET_STAT_ENABLE,
#else
      /* phystatmode */ WP_ENET_STAT_DISABLE,
#endif

#ifdef DEBUG_MODE
      /*tx_statmode;*/ WP_PKT_STAT_ENABLE,
#else
      /*tx_statmode;*/ WP_PKT_STAT_DISABLE,
#endif

#ifdef DEBUG_MODE
      /*rx_statmode;*/ WP_PKT_STAT_ENABLE,
#else
      /*rx_statmode;*/ WP_PKT_STAT_DISABLE,
#endif

      /* WP_U8 tx_duplicate;  */ WP_DISABLE,
      /* WP_enet_filters *rx_filters; */ &enet_filters[0]
      }
};






WP_port_enet gbe_s[1] =
{
      {
      /* pkt_limits */                     {64, 64, 0, 0},
      /* flowmode   */                     WP_FLOWMODE_FAST,
#ifdef WP_HW_WINPATH3
      /* interface_mode */                 WP_ENET_SGMII_1000,
#endif
#ifdef WP_HW_WINPATH2
      /* interface_mode */                 WP_ENET_RGMII_1000,
#endif
      /* rx_iw_bkgnd    */                 0
     }
};




WP_device_enet gbe_dev[1] =

{
       {
      /* WP_U16 max_tx_channels; */         10,
      /* WP_U16 tx_maxsdu;       */         1536,
      /* WP_U8  operating_speed; */         WP_UNUSED,
      /* mac_addr[6];            */         {0xAA,0xBB,0xCC,0xDD,0xEE,0xFF},
#ifdef WP_HW_WINPATH3
      /* WP_U32 tx_bit_rate;     */         1000000000,
#endif
#ifdef WP_HW_WINPATH2
      /* WP_U32 tx_bit_rate;     */         50000000,
#endif
      /* WP_U8  loopbackmode;    */         WP_ENET_NORMAL,
      /* WP_device_enet_ex *extended_params; */ NULL
      }
};


WP_pkt_shaping_wfq wfq[1]=
{{
   /* weight;  */1,
   /* weight_fraction;*/0
}};

WP_ch_enet ch_enet[1] =
{
          {
           /* WP_U8 intmode; */                 WP_PKTCH_INT_DISABLE,
           /* WP_U8 iwmode;  */                 WP_PKTCH_IWM_ENABLE,
           /* WP_U8 testmode;*/                 WP_PKTCH_TEST_DISABLE,
           /* WP_handle tx_pqblock;     */      0,
           /* WP_U16 tx_pqlevel;        */      0,
/* JLA  */ /* WP_pkt_shaping_type tx_shaping_type; */ /* WP_PKT_SHAPING_STRICT,*/
           /* WP_pkt_shaping_type tx_shaping_type; */ WP_FMU_SHAPING_TYPE_STRICT,
           /* void *tx_shaping_params;          Pointer to structure of type
                                                WP_pkt_shaping_cir,
                                                WP_pkt_shaping_cir_eir,
                                                WP_pkt_shaping_min_max,
                                                WP_pkt_shaping_wrr,
                                                WP_pkt_shaping_wfq  &wfq[0],
                                                WP_pkt_shaping_wfq  */  NULL,
           /* WP_U16 rx_maxsdu; */              1536,
           /* WP_U8 tx_cwid;    */              0,
           /* WP_U8 tx_tq;      */              0,
           /* WP_U32 rx_queuedepth;*/           16,
          }
};






/*****************************************************************************
 *      WP_pool_buffer_data     Buffer Pool parameters
 *
 *    This structure defines a pool used for data buffers including the number, size,
 *    and location of where the buffers will be created
 *
 *****************************************************************************/

WP_pool_buffer_data buffer_data[1] =
{
    {
        /* n_buffers */   1000,
/* offset */ 0x40,/**/
        /* size */ 1984,
        /* pad */         0,
#ifndef WP_BOARD_WDS3_SL
        /* databustype */ WP_BUS_PACKET,
        /* bank */        APP_BANK_PACKET
#else
        /* databustype */ WP_BUS_PARAM,
        /* bank */        APP_BANK_PARAM
#endif
    }
};

/*****************************************************************************
 *      WP_qnode_iwq     Interworking Buffer Pool parameters
 *
 *    Interruptqueue specifies the event queue used by objects that reference this queue node
 *    num_buffers specifies the number of interworking buffers to create for the queue node
 *    adjunct_pool specifies a pool of buffers to be used when sending/recieving interworking
 *    data to/from the host.  Note that the size and characteristics of the adjunct pool is used
 *    in the creation of the buffers created for this iw queue node.
 *
 *****************************************************************************/

WP_qnode_iwq qn_iw[1]=
{
   {
      /* interruptqueue */  0,
      /* num_buffers */     1000,
      /* adjunct_pool */ 0
   }
};


WP_iw_agg_routing tx_binding[2] =
{
    {
       /* tag */                0x12345678,
       /* txfunc */      0,
       /* dfsrv_remarking */    WP_IW_DSRV_DISABLE,
       /* l2hie_mode */         WP_IW_L2H_INSERT_ENABLE,
       /* shiphrm */            WP_IW_FAST_FWD_MODE,
       /* interruptqueue;*/     WP_IW_IRQT0,
       /* error_pkt_mode*/      0,
       /* intm_mode */          WP_IW_INT_DISABLE /*  TO CHECK */,
       /* ste_mode */           WP_IW_STAT_DISABLE,
       /* tspe_mode */          WP_IW_TIME_STAMP_DISABLE,
       /* ov_pool_mode */       0,
       /* fbp_drop_threshold */ 0,
       /* mtu */                1536,
       /* dfsrv_code_point */   0,
       /* prefix_length */      14,
       /* multi_cong_prof  */   0,
       /* prefix_header[16] */   {0x12,0x34,0x56,0x78,0x99,0x99,0x12,0x34,
                                  0x56,0x78,0x77,0x77,0x08,0x00,0x00,0x00},
       /* new_ip_header[20]; */  {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                  0x00,0x00,0x00,0x00},
      /*policer_enable*/              WP_IW_POLICER_DISABLE,
      /**policer_config;*/            NULL,
      /*cong_mode;*/                  WP_IW_CONGESTION_DISABLE,
      /**cong_threshold_param;*/      NULL

    },
   {
       /* tag */                0x12345678,
       /* txfunc */      0,
       /* dfsrv_remarking */    WP_IW_DSRV_DISABLE,
       /* l2hie_mode */         WP_IW_L2H_INSERT_ENABLE,
       /* shiphrm */            WP_IW_FAST_FWD_MODE,
       /* interruptqueue;*/     WP_IW_IRQT0,
       /* error_pkt_mode*/      0,
       /* intm_mode */          WP_IW_INT_DISABLE /*  TO CHECK */,
       /* ste_mode */           WP_IW_STAT_DISABLE,
       /* tspe_mode */          WP_IW_TIME_STAMP_DISABLE,
       /* ov_pool_mode */       0,
       /* fbp_drop_threshold */ 0,
       /* mtu */                1536,
       /* dfsrv_code_point */   0,
       /* prefix_length */      22,
       /* multi_cong_prof  */   0,
       /* prefix_header[24] */   {0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,
                                  0x00,0x00,0x00,0x02,0x00,0x00,0xaa,0xaa,
                                  0x03,0x00,0x00,0x00,0x08,0x00,0x00,0x00},

       /* new_ip_header[20]; */  {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                  0x00,0x00,0x00,0x00},
      /*policer_enable*/              WP_IW_POLICER_DISABLE,
      /**policer_config;*/            NULL,
      /*cong_mode;*/                  WP_IW_CONGESTION_DISABLE,
      /**cong_threshold_param;*/      NULL

    }
};



WP_iw_sys_bridging dm_sys[1]=
{
   {
      /* tag */        0,
      /* max_flows; */ 21,
      /* classification_mode */WP_IW_NO_CLASSIFIER,
      /* classifier_config */ {WP_IW_NULL_CLASSIFIER,0,0,NULL},
      /* learning_mode;*/WP_IW_BRIDGE_LEARNING_DISABLE,
      /* learning_queue */
           {
            /*int_queue_num*/WP_IW_IRQT0,
            /*learning_queue_size*/ 10,
            /*interrupt_enbale*/WP_LEARNING_POLLING_MODE,
            /*interrupt_rate*/1
           },
      /* forwarding_table_size*/WP_IW_32K_HASH_ENTRIES,
      /* member_set_size*/WP_IW_32K_HASH_ENTRIES,
      /* buffer_gap;*/0x40,
      /* max_bridging_ports*/ 21,
      /* *dfc_info */         NULL
   }
};

WP_bridge_port bport_conf[1] =
{
  {
    /* tag */ 0x5555,
    /* direct_mapping*/WP_IW_DIRECT_MAP_ENABLE,
    /* flow_agg */0,
    /* termination_mode*/ /*WP_IW_L2_TERM_MODE*/WP_IW_HOST_TERM_MODE,
    /* learning_mode */WP_IW_LEARNING_DISABLE,
    /* in_filter_mode */WP_IW_INGRESS_FILTER_DISABLE,
    /* vlan_param */
    {
       /* vlan_acceptance_mode */WP_IW_ACCEPT_TAGGED_UNTAGGED,
       /* vlan_tag */0xa,
    },
    /* max_mac_addresses */ 21,
    /* group_tag */ 0,
    /* group_filtering_mode*/WP_IW_GROUP_FILTER_DISABLE,
    /* unk_mac_sa_filter*/0,
    /* unk_mc_mode*/WP_IW_UNK_MC_HT,
    /* bc_ht_mode*/WP_IW_BC_HT_DISABLE
  }
};


WP_rx_binding_bridging  rx_bind_dm[1]=
{
   {
      /*  encap_mode */0,
      /*  mru;*/1536,
      /*  vcfcs;*/0,
      /*  input_port;*/0
   }
};


WP_calendar_wheel calendar_cfg[2] =
{
   {
      /* function */    WP_CW_ASU,
      /* cwid*/         WP_CW_ID_A,
      /* min_ch_rate */ 150, /* 64Kbps / 424 , MIN_CH_RATE */
      /* max_ch_rate */ 117925 /* 50Mbps / 424 , MAX_CH_RATE */
   },
   WP_CALENDAR_WHEEL_TERM
};




WP_iw_agg_directmap iw_agg_dm [1] = {

  {
   /*WP_tag tag;*/ 0,
   /*WP_handle txfunc;*/   0,
   /*WP_U8 rfcs;*/ WP_IW_RFCS_DISABLE,
   /*WP_U8 l2_header_insert_mode;*/ /* 10*/ WP_IW_L2H_INSERT_DISABLE,
   /*WP_U8 interruptqueue;*/ WP_IW_L2H_INSERT_DISABLE,
   /*WP_U8 error_pkt_mode;*/ WP_IW_ERRPKT_DISCARD,
   /*WP_U8 intmode;*/ WP_IW_INT_DISABLE,
   /*WP_U8 statmode;*/ WP_IW_STAT_ENABLE,
   /*WP_U8 timestamp_mode;*/ WP_IW_TIME_STAMP_DISABLE,
   /*WP_U8 ov_pool_mode;*/ WP_IW_OV_POOL_DISABLE,
   /*WP_U16 fbp_drop_threshold;*/ 0,
   /*WP_U16 mtu; */ /*20 */ 1536,
   /*WP_U8 prefix_length;*/ 0,
   /*WP_U8 extraction_length;*/ 0,
   /*WP_U8 prefix_header[68];*/ WP_IW_NULL_L2_ENCAPSULATION,
   /*WP_U8 policer_enable;*/ WP_IW_POLICER_DISABLE,
   /*WP_policer_config   *policer_config  */ /*96*/ NULL,
   /*WP_U8 cong_mode;*/ WP_IW_CONGESTION_DISABLE,
   /*WP_iw_flow_agg_cong_thresh_param *cong_threshold_param;*/ NULL,
   /*WP_U8 dynamic_length_update;*/ WP_IW_LENGTH_UPDATE_DISABLED,
   /*WP_U8 length_offset;*/ 0,
   /*WP_flowaggtype    flow_agg_type;*/ WP_IW_FLOW_AGG_PRIMARY,
   /*WP_handle comp_group_handle;*/ 0
  }
};






WP_tx_binding tx_bind_rx_ch [1] = {
  {
    /*res0*/ 0,
    /*dci_mode*/ WP_IW_DYN_CH_INSERT_ENABLE,
    /*maxt*/ 25
  }
};


WP_ch_iw_rx rx_ch [1] = {
  {
    /*pqblock*/ 0,
    /*pqlevel*/ 0,
    /*tx_binding_type*/ WP_IW_TX_BINDING,
    /*tx_binding_config*/ &tx_bind_rx_ch[0]
  }
};




WP_data_unit data_unit;
WP_data_segment tx_buf;
WP_U8 *data_ptr;
WP_CHAR ascii_buffer[DATA_LENGTH*2];
WP_CHAR hexa_buffer[DATA_LENGTH];
WP_CHAR ch='c';
WP_U32 loop=0;


WP_handle iw_qnode_1, iwhosttx_handle;
WP_handle buffer_pool_1;
#ifdef WP_HW_WINPATH2
WP_handle buffer_pool_2, iw_qnode_2;
#endif
WP_handle gbep1, gbedev1;
WP_handle gbep2, gbedev2;
WP_handle enet_tx_1, enet_rx_1, enet_tx_2, enet_rx_2;

WP_handle host_port,host_device;
WP_handle iw_sys,iw_agg_g1, iw_agg_g1_tx, iw_agg_g2;
WP_handle iw_agg_g2_tx,iw_ch_g1,iw_ch_g2,bport_g1,bport_g2;

void WT_TranslateAsciiToHexGIGEGIGE(WP_CHAR *Hex, WP_CHAR *Ascii,
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

WP_handle status, iw_host_port, iw_host_device;

Y_MenuEntry V_LoopSelect[] =
{
  {2,      TRUE, "GigeLoopback menu",      {V_MainMenu}},
  {K_Leaf, TRUE, "Internal loopback mode", {CLI_Internal_Loopback}},
  {K_Leaf, TRUE, "Normal loopback mode",   {CLI_Normal_Loopback}}
 };


Y_MenuEntry V_MainMenu[] =
{
  {7     , TRUE, "Gigeloopback Functions", {V_MainMenu}},
  {K_Leaf, TRUE, (WP_CHAR *)&sendPacket1, {CLI_SendPacket_Interface1}},
  {K_Leaf, TRUE, (WP_CHAR *)&sendPacket2, {CLI_SendPacket_Interface2}},
  {K_Leaf, TRUE, (WP_CHAR *)&pollPacket1, {CLI_PollPacket_Interface1}},
  {K_Leaf, TRUE, (WP_CHAR *)&pollPacket2, {CLI_PollPacket_Interface2}},
#ifdef DEBUG_MODE
  {K_Leaf, TRUE, "Print Statistics for both ports", {CLI_PrintStatistics}},
#else
  {K_Leaf, FALSE, "Print Statistics for both ports", {CLI_PrintStatistics}},
#endif
  {K_Menu, TRUE, "Utilities and Debug", {V_Menu_Debug}},
  {K_Leaf, TRUE, "Quit the program",{CLI_ProgramQuit}}
};

void CLI_Internal_Loopback()
{
  loop = 1;
  InitBoard();
  SetDebugUpperMenu(&V_MainMenu[0]);
  F_SetMainMenu();
  F_DisplayMenu();
}

void CLI_Normal_Loopback()
{
  loop = 0;
  InitBoard();
  SetDebugUpperMenu(&V_MainMenu[0]);
  F_SetMainMenu();
  F_DisplayMenu();
}

void initMenuStrings()
{
 sprintf(sendPacket1, "Send a test packet to enet %d",WT_ENET_INTERFACE_1 - WP_PORT_ENET1 + 1);
 sprintf(sendPacket2, "Send a test packet to enet %d",WT_ENET_INTERFACE_2 - WP_PORT_ENET1 + 1);
 sprintf(pollPacket1, "Poll for a packet on enet %d",WT_ENET_INTERFACE_1 - WP_PORT_ENET1 + 1);
 sprintf(pollPacket2, "Poll for a packet on enet %d",WT_ENET_INTERFACE_2 - WP_PORT_ENET1 + 1);
}

void InitDrivers()
{
  WP_handle status;
  /*WP_handle iw_system, agg_queue, iw_bundle,default_agg;
    WP_U32 ip_address=0;*/

#if 0 /* internal MAC loopback has an errata.  Set this in the serdes instead */
  if(loop)
  {
    printf("\nLoopback mode set on GigE1 and GigE2");
    gbe_dev[0].loopbackmode = WP_ENET_LOOPBACK;
  }
#endif

   /*****************************************************************************
    *  Driver Initialisation - WDDI call registers driver with lower level
    *
    *
    *****************************************************************************/
  printf("Initialising Driver...");
  status = WP_DriverInit();
  terminate_on_error(status, "WP_DriverInit()");
#ifdef WP_HW_WINPATH3
   status = WPX_BoardConfigure(0, WPX_CONFIGURE_DEFAULT_CHECKIN);
   terminate_on_error(status, "WPX_BoardConfigure()");
   #ifndef WP_BOARD_WDS3_SL
   status = WPX_BoardSerdesInit(0, WP_PORT_ENET3 , WP_FALSE);
   terminate_on_error(status, "WPX_BoardSerdesSetLoopback()");
   status = WPX_BoardSerdesInit(0, WP_PORT_ENET4 , WP_FALSE);
   terminate_on_error(status, "WPX_BoardSerdesSetLoopback()");
   status = WPX_BoardSerdesInit(0, WP_PORT_ENET5 , WP_FALSE);
   terminate_on_error(status, "WPX_BoardSerdesSetLoopback()");
   status = WPX_BoardSerdesInit(0, WP_PORT_ENET6 , WP_FALSE);
   terminate_on_error(status, "WPX_BoardSerdesSetLoopback()");
   status = WPX_BoardSerdesInit(0, WP_PORT_ENET7 , WP_FALSE);
   terminate_on_error(status, "WPX_BoardSerdesSetLoopback()");
   status = WPX_BoardSerdesInit(0, WP_PORT_ENET8 , WP_FALSE);
   terminate_on_error(status, "WPX_BoardSerdesSetLoopback()");
   status = WPX_BoardSerdesInit(0, WP_PORT_ENET13 , WP_FALSE);
   terminate_on_error(status, "WPX_BoardSerdesSetLoopback()");
   status = WPX_BoardSerdesInit(0, WP_PORT_ENET14 , WP_FALSE);
   terminate_on_error(status, "WPX_BoardSerdesSetLoopback()");
   status = WPX_BoardSerdesInit(0, WP_PORT_ENET15 , WP_FALSE);
   terminate_on_error(status, "WPX_BoardSerdesSetLoopback()");
   status = WPX_BoardSerdesInit(0, WP_PORT_ENET16 , WP_FALSE);
   terminate_on_error(status, "WPX_BoardSerdesSetLoopback()");
   #else
   status = WPX_BoardSerdesInit(0, WP_PORT_ENET9 , WP_FALSE);
   terminate_on_error(status, "WPX_BoardSerdesSetLoopback()");
   status = WPX_BoardSerdesInit(0, WP_PORT_ENET10 , WP_FALSE);
   terminate_on_error(status, "WPX_BoardSerdesSetLoopback()");
   status = WPX_BoardSerdesInit(0, WP_PORT_ENET11, WP_FALSE);
   terminate_on_error(status, "WPX_BoardSerdesSetLoopback()");
   status = WPX_BoardSerdesInit(0, WP_PORT_ENET12, WP_FALSE);
   terminate_on_error(status, "WPX_BoardSerdesSetLoopback()");
   status = WPX_BoardSerdesInit(0, WP_PORT_ENET1, WP_FALSE);
   terminate_on_error(status, "WPX_BoardSerdesSetLoopback()");
   status = WPX_BoardSerdesInit(0, WP_PORT_ENET2, WP_FALSE);
   terminate_on_error(status, "WPX_BoardSerdesSetLoopback()");
   #endif
#endif

#ifdef WP_HW_WINPATH2
 #ifdef TARGET_WDS /* only perform PinMux changes on WDS Board */
   status = WPU_PinMuxAllocateToPort(WP_PORT_asdfsdfsdfkENET1,WPU_PIN_MUX_MODE_DEFAULT);
   terminate_on_error(status, "WPU_PinMuxAllocateToPort - Enet1");

   status = WPU_PinMuxAllocateToPort(WP_PORT_ENET3,WPU_PIN_MUX_MODE_DEFAULT);
   terminate_on_error(status, "WPU_PinMuxAllocateToPort - Enet2");

   status=WPU_GeiPhyInit(WP_PORT_ENET3, WPU_MODE_GMII_OV_COPPER );
   terminate_on_error(status, "WPU_GeiPhyInit - Enet2");

   status=WPU_GeiPhyInit(WP_PORT_ENET1, WPU_MODE_GMII_OV_COPPER );
   terminate_on_error(status, "WPU_GeiPhyInit - Enet1");
 #else
/* WPU_EnetPhyInit(WP_PORT_ENET3, WPU_MODE_RGMII | WPU_OV_COPPER);*/
/* WPU_EnetPhyInit(WP_PORT_ENET4, WPU_MODE_RGMII | WPU_OV_COPPER);*/
 #endif
#endif
}

void InitBoard()
{
  volatile WP_U32 i;
  WP_U32 additional_flag;
  WP_handle status;
   /*WP_flow_route route_cfg;*/
  /*****************************************************************************
    *
    *  If loopback mode is required set this in the serdes and not
    *  in the device
    *
    *****************************************************************************/
#ifdef WP_HW_WINPATH3
    if (loop)
    {
         status = WPX_BoardSerdesInit(0,WT_ENET_INTERFACE_1  , WP_TRUE);
         terminate_on_error(status, "WPX_BoardSerdesSetLoopback()");
         status = WPX_BoardSerdesInit(0,WT_ENET_INTERFACE_2  , WP_TRUE);
         terminate_on_error(status, "WPX_BoardSerdesSetLoopback()");
    }
#endif
#ifdef WP_HW_WINPATH2
   if (loop)
   {
      gbe_dev[0].loopbackmode = WP_ENET_LOOPBACK;
   }
#endif
   /*****************************************************************************
    *  WinPath Initialisation - WDDI call uses the context structure to initialise
    *                           WinPath DPS and general WDDI constraints
    *
    *****************************************************************************/

  status = WP_SysInit(WP_WINPATH(0), context);
  terminate_on_error(status, "WP_SysInit()");
  printf("System Initialized\n");

  printf("Waiting for link...\n");
  for(i=0;i<5000000;i++);


  /*****************************************************************************
   *  Buffer Pool Creation - WDDI call to create the adjunct buffer pools
   *****************************************************************************/

  buffer_pool_1 = WP_PoolCreate(WP_WINPATH(0), WP_pool_iwbuffer, buffer_data);
  terminate_on_error(buffer_pool_1, "WP_PoolCreate() GIGE 1");

  qn_iw->adjunct_pool = buffer_pool_1;
#ifdef WP_HW_WINPATH3
  additional_flag = WP_QNODE_OPT_FMU;
#endif
#ifdef WP_HW_WINPATH2
  additional_flag = WP_QNODE_OPT_DEDICATED_RX_HWQ |  WP_QNODE_OPT_FMU;
#endif

  iw_qnode_1 = WP_QNodeCreate(0,WP_QNODE_IWQ | additional_flag,qn_iw);
  terminate_on_error(iw_qnode_1, "WP_QNodeCreate() GIGE 1");

/*  status = WP_SysMiiMgmtCreate(WP_WINPATH(0), mii_mgmt_cfg);*/
/*  terminate_on_error(status, "WP_SysMiiMgmtCreate()");*/

   /*****************************************************************************
    *   WP_port_enet                Ethernet Port configuration used with WP_PortCreate
    *
    *   This structure defines the port configuration of the Enet port. The WDDI manual details
    *   each entry in the structure. In summary, the Enet port is in normal mode, full
    *   duplex with stats enabled.
    *
    *****************************************************************************/



  /* Create an GEnet Port */

  gbep1 = WP_PortCreate(WP_WINPATH(0),WT_ENET_INTERFACE_1,gbe_s);
  terminate_on_error(gbep1, "WP_PortCreate() HSPI-GENET Port 8");



  /*****************************************************************************
 *      WP_device_hs_enet      High Speed Ethernet device parameters
 *
 *    This structure defines the high speed ethernet device specific characterstics of
 *    a gigabit ethernet port.
 *    In this example packet statistics are enabled on both transmission and reception channels.
 *
 *****************************************************************************/

  gbedev1 = WP_DeviceCreate(gbep1, 0, WP_DEVICE_ENET, gbe_dev);
  terminate_on_error(gbedev1, "WP_DeviceCreate() HSPI-GENET Device 1");

  gbe_dev->mac_addr[5] = 0xaa;
  gbe_dev->mac_addr[4] = 0xbb;
  gbe_dev->mac_addr[3] = 0xcc;
  gbe_dev->mac_addr[2] = 0xdd;
  gbe_dev->mac_addr[1] = 0xee;
  gbe_dev->mac_addr[0] = 0x00;

   /*****************************************************************************
    *   WP_port_enet                Ethernet Port configuration used with WP_PortCreate
    *
    *   This structure defines the port configuration of the Enet port. The WDDI manual details
    *   each entry in the structure. In summary, the Enet port is in normal mode, full
    *   duplex with stats enabled.
    *
    *****************************************************************************/


  gbep2 = WP_PortCreate(WP_WINPATH(0),WT_ENET_INTERFACE_2,gbe_s);
  terminate_on_error(gbep2, "WP_PortCreate() HSPI-GENET Port 2");



  /*****************************************************************************
 *      WP_device_hs_enet      High Speed Ethernet device parameters
 *
 *    This structure defines the high speed ethernet device specific characterstics of
 *    a gigabit ethernet port.
 *    In this example packet statistics are enabled on both transmission and reception channels.
 *
 *****************************************************************************/

  gbedev2 = WP_DeviceCreate(gbep2, 0,WP_DEVICE_ENET, gbe_dev);
  terminate_on_error(gbedev2, "WP_DeviceCreate() HSPI-GENET Device 2");


  /* Create virtual host ports and devices. */
  host_port =  WP_PortCreate(WP_WINPATH(0),WP_PORT_IW_HOST,NULL);
  terminate_on_error(host_port,"Host Port Create");
  host_device = WP_DeviceCreate(host_port,0,WP_DEVICE_IW_HOST,NULL);
  terminate_on_error(host_device,"Host Device Create");

  iw_host_port = WP_PortCreate(WP_WINPATH(0), WP_PORT_IW_HOST, NULL);
  terminate_on_error(iw_host_port, "WP_PortCreate() IW Host");

  iw_host_device = WP_DeviceCreate(iw_host_port, 0, WP_DEVICE_IW_HOST, NULL);
  terminate_on_error(iw_host_device, "WP_DeviceCreate() IW Host");

#if START_SCHEDULER
  /*****************************************************************************
   *  Scheduler Creation -    WDDI call to create the Scheduler based on the
   *                          defined earlier in the application
   * THIS IS NOT NEEDED by the application.  It is here to show if the DPS
   * if functioning by showing threads utilized in the debug utilities.
   * This can be used to verify that the dps is actually running in case
   * there is no tx/rx of enet packets.
   *****************************************************************************/

   status = WP_SysSchedulerCreate(WP_WINPATH(0), calendar_cfg);
   terminate_on_error(status, "WP_SysSchedulerCreate");
#endif

  /* Create system */

  status = WP_SysCommit();
  terminate_on_error(status, "WP_SysCommit()");

#if START_SCHEDULER
  /*****************************************************************************
   *  ATM Scheduler Enable - WDDI call to enable the previously created ATM Scheduling Unit
   * THIS IS NOT NEEDED by the application.  It is here to show if the DPS
   * if functioning by showing threads utilized in the debug utilities.
   * This can be used to verify that the dps is actually running in case
   * there is no tx/rx of enet packets.
   *****************************************************************************/

  status = WP_SysAsuEnable(WP_WINPATH(0));
  terminate_on_error(status, "WP_SysAsuEnable()");
#endif


  enet_tx_1 = WP_ChannelCreate(0x19, gbedev1, iw_qnode_1, WP_CH_TX, WP_ENET, &ch_enet[0]);
  terminate_on_error(enet_tx_1, "WP_ChannelCreate() enet tx 1");

  enet_rx_1 = WP_ChannelCreate(0x1a, gbedev1, iw_qnode_1, WP_CH_RX, WP_ENET, &ch_enet[0]);
  terminate_on_error(enet_rx_1, "WP_ChannelCreate() enet rx 1");

  enet_tx_2 = WP_ChannelCreate(0x29, gbedev2, iw_qnode_1, WP_CH_TX, WP_ENET, &ch_enet[0]);
  terminate_on_error(enet_tx_2, "WP_ChannelCreate() enet tx 2");

  enet_rx_2 = WP_ChannelCreate(0x2a, gbedev2, iw_qnode_1, WP_CH_RX, WP_ENET, &ch_enet[0]);
  terminate_on_error(enet_rx_2, "WP_ChannelCreate() enet rx 2");


  status = WP_PortEnable(gbep1,WP_DIRECTION_DUPLEX);
  terminate_on_error(status, "WP_portEnable()");

  status = WP_PortEnable(gbep2,WP_DIRECTION_DUPLEX);
  terminate_on_error(status, "WP_portEnable()");



  status = WP_DeviceEnable(gbedev1,WP_DIRECTION_DUPLEX);
  terminate_on_error(status, "WP_DeviceEnable()");

  status = WP_DeviceEnable(gbedev2,WP_DIRECTION_DUPLEX);
  terminate_on_error(status, "WP_DeviceEnable()");

#if 0 /* Might be useful if there are memory allocation errors */
  status = WP_MemoryAvailable(WP_WINPATH(0), WP_BUS_HOST, 0, &bytes);
  printf("UPI GENET App:  Packet bus bytes left = %d\n", bytes);

  status = WP_MemoryAvailable(WP_WINPATH(0), WP_BUS_PARAM, 0, &bytes);
  printf("UPI GENET App:  Param bus bytes left = %d\n", bytes);

  status = WP_MemoryAvailable(WP_WINPATH(0), WP_BUS_INTERNAL, 0, &bytes);
  printf("UPI GENET App:  Internal bus bytes left = %d\n", bytes);

  status = WP_MemoryAvailable(WP_WINPATH(0), WP_BUS_HOST, 0, &bytes);
  printf("UPI GENET App:  Host bus bytes left = %d\n", bytes);
#endif

  iw_ch_g1 = WP_ChannelCreate(0,host_device,iw_qnode_1,WP_CH_RX,WP_IW_HOST,rx_ch);
  terminate_on_error(iw_ch_g1,"channel create iw rx host");

  iw_ch_g2 = WP_ChannelCreate(0,host_device,iw_qnode_1,WP_CH_RX,WP_IW_HOST,rx_ch);
  terminate_on_error(iw_ch_g2,"channel create iw rx host");

  iw_sys = WP_IwSystemCreate(WP_WINPATH(0),WP_IW_TRANS_BRIDGING_MODE, dm_sys);
  terminate_on_error(iw_sys,"SystemCreate");

  tx_agg_h[0].txfunc = iw_ch_g1;
  iw_agg_g1 = WP_IwFlowAggregationCreate(WP_WINPATH(0),WP_IW_TRANS_BRIDGING_MODE,tx_agg_h);
  terminate_on_error(iw_agg_g1,"Host 1 Aggregation Create");

  tx_agg_h[0].txfunc = iw_ch_g2;
  iw_agg_g2 = WP_IwFlowAggregationCreate(WP_WINPATH(0),WP_IW_TRANS_BRIDGING_MODE,tx_agg_h);
  terminate_on_error(iw_agg_g2,"Host 2 Aggregation Create");

  iw_agg_dm[0].txfunc = enet_tx_1; /*gbedev1;*/
  iw_agg_g1_tx = WP_IwFlowAggregationCreate(WP_WINPATH(0),WP_IW_DIRECTMAP_MODE,iw_agg_dm);
  terminate_on_error(iw_agg_g1_tx,"Aggregation Create");

  IwHostTxChannel( iw_agg_g1);

  iw_agg_dm[0].txfunc = enet_tx_2; /*gbedev2;*/
  iw_agg_g2_tx = WP_IwFlowAggregationCreate(WP_WINPATH(0),WP_IW_DIRECTMAP_MODE,iw_agg_dm);
  terminate_on_error(iw_agg_g1_tx,"Aggregation Create");



  bport_conf[0].flow_agg = iw_agg_g1;
  bport_g1 = WP_IwPortCreate(iw_sys,bport_conf);
  terminate_on_error(bport_g1, "WP_IwPortCreate");

  bport_conf[0].flow_agg = iw_agg_g2;
  bport_g2 = WP_IwPortCreate(iw_sys,bport_conf);
  terminate_on_error(bport_g2, "WP_IwPortCreate");

  rx_bind_dm[0].input_port = bport_g1;
  status = WP_IwRxBindingCreate(enet_rx_1,iw_sys,iw_qnode_1,rx_bind_dm);
  terminate_on_error(status,"RxBindingCreate");

  rx_bind_dm[0].input_port = bport_g2;
  status = WP_IwRxBindingCreate(enet_rx_2,iw_sys,iw_qnode_1,rx_bind_dm);
  terminate_on_error(status,"RxBindingCreate");



  status = WP_IwTxBindingCreate(enet_tx_1, WP_IW_TX_BINDING, &tx_bind_rx_ch[0]);
  terminate_on_error(status, "TxBindingCreate");

  status = WP_IwTxBindingCreate(enet_tx_2, WP_IW_TX_BINDING, &tx_bind_rx_ch[0]);
  terminate_on_error(status, "TxBindingCreate");


  status = WP_ChannelEnable(enet_tx_1);
  terminate_on_error(status, "Enable Tx Channel");

  status = WP_ChannelEnable(enet_rx_1);
  terminate_on_error(status, "Enable Rx Channel");

  status = WP_ChannelEnable(enet_tx_2);
  terminate_on_error(status, "Enable Tx Channel");

  status = WP_ChannelEnable(enet_rx_2);
  terminate_on_error(status, "Enable Rx Channel");




  status = WP_IwSystemBuild(iw_sys);
  terminate_on_error(status,"WP_IwSystemBuild");
}

void CLI_PrintStatistics()
{
      printf("Statistics\nenet %d:\n",WT_ENET_INTERFACE_1 - WP_PORT_ENET1 + 1 );
      WPE_DisplayStatHsEnet(gbedev1);
      printf("enet %d:\n", WT_ENET_INTERFACE_2 - WP_PORT_ENET1 + 1 );
      WPE_DisplayStatHsEnet(gbedev2);
      printf("\n GBE 1 Rx Flow Agg:");
      F_DisplayFlowStats(iw_agg_g1);
      printf("\n GBE 1 Tx Flow Agg:");
      F_DisplayFlowStats(iw_agg_g1_tx);
      printf("\n GBE 2 Rx Flow Agg:");
      F_DisplayFlowStats(iw_agg_g2);
      printf("\n GBE 2 Tx Flow Agg:");
      F_DisplayFlowStats(iw_agg_g2_tx);
}
void CLI_SendPacket_Interface1()
{
      printf("test packet tx to enet %d\n", WT_ENET_INTERFACE_1 - WP_PORT_ENET1 + 1);
      iw_host_tx_data(iw_agg_g1_tx, iwhosttx_handle, buffer_pool_1);
}
void CLI_SendPacket_Interface2()
{
      printf("test packet tx to enet %d\n", WT_ENET_INTERFACE_2 - WP_PORT_ENET1 + 1);
          iw_host_tx_data(iw_agg_g2_tx, iwhosttx_handle, buffer_pool_1);
}
void CLI_PollPacket_Interface1()
{
      WP_U32 a = 0;
      printf("\npoll for packet rx to enet %d\n", WT_ENET_INTERFACE_1- WP_PORT_ENET1 + 1);
      while(a++ < 5)
      {
        rx_data(iw_ch_g1);
	printf ("take a delay 1\n");
	fflush ((FILE *)1);
	WPL_Delay (100000);
      }
}
void CLI_PollPacket_Interface2()
{
      WP_U32 a = 0;
      printf("\npoll for packet rx to enet %d\n", WT_ENET_INTERFACE_2 - WP_PORT_ENET1 + 1);
      while(a++ < 5)
      {
        rx_data(iw_ch_g2);
	fflush ((FILE *)1);
	WPL_Delay (100000);
      }
}

void CLI_ProgramQuit()
{
  status = WP_DriverRelease();
  return;
}

WP_S32 main(WP_S32 argc, WP_CHAR ** argv)
{
   WP_CHAR InputBuf[256];
   initMenuStrings();
   InitDrivers();
   SetDebugUpperMenu(&V_LoopSelect[0]);
   F_SetMenu(&V_LoopSelect[0]);
   F_DisplayMenu();

   while (TRUE)
   {
      printf("%s", STR_ENTER_COMMAND);
      memset(InputBuf,0,256);
      gets(InputBuf);
      printf("\n");
      F_MenuEngine(InputBuf);
   }
}



/************************************************/


void IwHostTxChannel(WP_handle flow_agg)
{
   WP_handle bridge_port;

   WP_ch_iw_tx ch_config_iw_tx[1] =
      {
         {
            /* iw_system */                 0,
            /* reserved */                  0,
            /* group_id */                  WP_IW_CH_GROUP_ID_NULL,
            /* bridging_group_tag */        0,
            /* input_port */                0
         }
      };

   WP_bridge_port bport_conf[1] =
      {
         {
            /* tag */ 0,
            /* direct_mapping*/WP_IW_DIRECT_MAP_DISABLE,
            /* flow_agg */0,
            /* termination_mode*/ /*WP_IW_L2_TERM_MODE*/WP_IW_HOST_TERM_MODE,
            /* learning_mode */WP_IW_LEARNING_DISABLE,
            /* in_filter_mode */WP_IW_INGRESS_FILTER_DISABLE,
            /* vlan_param */
            {
               /* vlan_acceptance_mode */WP_IW_ACCEPT_TAGGED_UNTAGGED,
               /* vlan_tag */0xa,
            },
            /* max_mac_addresses */ 2000,
            /* group_tag */ 0,
            /* group_filtering_mode*/WP_IW_GROUP_FILTER_DISABLE,
            /* unk_mac_sa_filter*/0,
            /* unk_mc_mode*/WP_IW_UNK_MC_SR,
            /* bc_ht_mode*/WP_IW_BC_HT_DISABLE,
            /*input_filters_mask*/ 0,
            /*output_filters_mask*/ 0,
            /*statmode*/ WP_IW_PORT_STAT_ENABLE
         }
      };


   bport_conf->tag = 0xfffe;
   bport_conf->flow_agg = flow_agg;

   bridge_port = WP_IwPortCreate(iw_sys,bport_conf);
   terminate_on_error( bridge_port,"WP_IwPortCreate");

   ch_config_iw_tx->input_port = bridge_port;
   ch_config_iw_tx->iw_system = iw_sys;
   iwhosttx_handle = WP_ChannelCreate( 0x123456, iw_host_device, iw_qnode_1, WP_CH_TX,
                                       WP_IW_HOST, &ch_config_iw_tx[0]);
   terminate_on_error( iwhosttx_handle, "WP_ChannelCreate() iw host Tx channel");

#ifdef WP_HW_WINPATH2
   status = WP_ChannelEnable( iwhosttx_handle);
   terminate_on_error( status,
                 "WP_ChannelEnable() iwhosttx_handle");
#endif
}

void iw_host_tx_data(WP_handle flow_agg_handle, WP_handle iwtx_handle, WP_handle pool)
{
   WP_U32 /*i,j,*/ k;
        WP_status status;
    WP_data_segment* curr_buf;

   data_unit.data_size = DATA_LENGTH;
   data_unit.n_active = 1;

   tx_buf.data_size = data_unit.data_size;
   if (NULL == (data_ptr = WP_PoolAlloc(pool)))
   {
      printf ("WP_PoolAlloc failed\n");
      return ;
   }
   tx_buf.data = data_ptr;
   tx_buf.pool_handle = pool;
   tx_buf.displacement = 0;
   tx_buf.user_info = 0;
   tx_buf.next = NULL;
   tx_buf.buffer_class = 0;

#if 1
   data_unit.control =
         WP_HT_IW_B_MODE_FIELD(WP_HT_IW_B_FAST_MODE) |
         WP_HT_IW_B_VA_DEST_TYPE_FIELD(WP_HT_IW_B_VA_DESTINATION_FLOW_AGG);
#else
   data_unit.control =
         WP_HT_IW_B_MODE_FIELD(WP_HT_IW_B_FAST_MODE)|
         WP_HT_IW_B_VA_DEST_TYPE_FIELD(WP_HT_IW_B_VA_DESTINATION_FLOW_AGG)|
         WP_HT_IW_FSP_FLOW_AGG_FIELD( flow_agg_handle) |
         WP_HT_IW_CRC_FIELD(0);
#endif
   data_unit.segment = (&tx_buf);
   data_unit.n_segments = 1;
   data_unit.n_active = 1;
#if 1
   data_unit.type = WP_DATA_IW;
#else
   data_unit.type = WP_DATA_ENET;
#endif
   data_unit.data_return = 0;
   data_unit.status = 0;
/*
   WP_MEM_BYTES_FILL(data_ptr, 0, tx_buf.data_size);
        for(i = 0;i<DATA_LENGTH;i++)
                WP_MEM_BYTE_SET (data_ptr[i],i);
*/

   /*strcpy (ascii_buffer,"1213141516171001000101010800");*/
   memset (ascii_buffer, 0, DATA_LENGTH*2);
   strcpy (ascii_buffer,"aabbccddeeffaabbccddee000800");
   strcat (ascii_buffer,"45040020");/* tos = 4 */
   strcat (ascii_buffer,"001b0000");
   strcat (ascii_buffer,"0a11e449");/* protocol = 0x11*/
   strcat (ascii_buffer,"00010c20");/* sa ip */
   strcat (ascii_buffer,"c0440000");/* Da ip */
   strcat (ascii_buffer,"00190015");
   strcat (ascii_buffer,"001b77c0");
   strcat (ascii_buffer,"00a4c5ff");

   memset (hexa_buffer, 0, DATA_LENGTH);
   WT_TranslateAsciiToHexGIGEGIGE(hexa_buffer,ascii_buffer,DATA_LENGTH);

   WP_MEM_BYTES_FILL(data_ptr, 0, tx_buf.data_size);
   WP_MEM_BYTES_SET (data_ptr ,hexa_buffer,DATA_LENGTH);

      printf (" Send packet \n ");


    curr_buf = data_unit.segment;
    for (k = 0; k < curr_buf->data_size; k++)
      {
        printf("%2.2x", curr_buf->data[k]);
      }
    printf("\n");

      status = WP_HostSend(iwtx_handle, &data_unit);
      terminate_on_error(status, "WP_HostSend()");




      printf ("take a delay\n");
      fflush ((FILE*)1);
      WPL_Delay (100000);
}


void rx_data(WP_handle dev_handle)
{
  WP_status status;
  WP_data_unit du;
  WP_data_segment seg;
  WP_data_segment* curr_buf;
  WP_U32 j,k;

  du.segment = &seg;
  du.type = WP_DATA_IW;
  du.n_segments = 1;

  if (WP_OK != (status = WP_HostReceive(dev_handle, &du)))
  {
    if ( strcmp("WP_ERR_HST_NO_DATA_TO_GET",  WP_error_name[WP_ERROR(status)]) != 0)
      terminate_on_error(status,"WP_HostReceive error");
  }
  else {

    curr_buf = du.segment;
    for (j = 0; j < du.n_active; j++, curr_buf++)
    {
      for (k = 0; k < curr_buf->data_size; k++)
      {
        printf("%2.2x", curr_buf->data[k]);
      }
      WP_PoolFree(curr_buf->pool_handle, curr_buf->data);
      printf("(%d bytes) \n", curr_buf->data_size);
    }
  }
}


void
terminate_on_error(WP_handle handle, WP_CHAR *s)
{
   if (WP_ERROR_P(handle)) {
      printf("%s: Terminal error (handle 0x%x) %s\n", appname, handle, s);
      printf("%s: Error type - %s\n", appname, WP_error_name[WP_ERROR(handle)]);
      exit(1);
   }
   else
   {
      printf("App: Handle/Status returned from %s is %x \n", s, handle);
   }

}


#include "wt_new_automation.c"
#include "wt_debug_menu.c"
#include "wt_util.c"
#include "wt_statistics_functions.c"

