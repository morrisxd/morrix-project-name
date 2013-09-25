/*****************************************************************************
 * (C) Copyright Wintegra 2001-2010, All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra, Inc.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 *****************************************************************************/

/*****************************************************************************
 * Example : upi_aal5.c
 *
 * Description:
 *
 *     This application creates two AAL5 channels (one for transmit and one
 *     for received) over a UPI port (UPI port 1). It can be configured in internal
 *     loopback mode or normal mode for use with an OC3 card.
 *     The AAL5 channels are host terminated. Multiple frames with 10 buffers are sent
 *     and received. The application prints out the received data. The received data is
 *     not validated.
 *
 * Output:
 *     After Initializing WDDI objects, the application prints out the
 *     memory available on each of the four winpath buses.
 *
 *     The application prints out the received frame in hexadecimal format.
 *
 *
 *  Test Configuration: This application requires a WDS board. The OC-3 daughter card
 *     is optional. The OC3 variable needs to be defined to 1 for the application
 *     to test normal mode with an OC3 card. A loopback cable must be attached to the
 *     OC3 card
 *
 *  WDDI Version:  This example was tested using WDDI V4.1.

 *****************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "wp_wddi.h"
#include "wp_host_io.h"

#include "wp_sim.h"
#include "wpi_veneer.h"
#include "wpx_app_data.h"
#include "wpx_oc3_pmc5351.h"

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
#define OC3 0 /* 0: external loopback disabled; 1: external loopback enabled */
#define UPI_PORT_INTERFACE WP_PORT_UPI2

WP_CHAR *STR_MENU_HDR = {"  UPI AAL5 Test"};
extern Y_MenuEntry V_Menu_Debug[];
extern Y_MenuEntry V_MainMenu[];

/* Utility function to handle return values from WDDI function calls */
void terminate_on_error(WP_handle, WP_CHAR *);

/*Statistic functions*/
void printChannelStat(WP_handle handle);
void printAtmDeviceStats(WP_handle handle);

/*Debug menu functions*/
void menu_interface(void);

/* interrupt functions */
void WP_AppIndicateEventTx(WP_tag, WP_U32, WP_U32);
void WP_AppIndicateEventRx(WP_tag, WP_U32, WP_U32);
void WPI_SimulateInterrupts(void);

/* scheduling */
void WPE_CreateSchedulingUnit(void);

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

/* global flags to indicate tx and rx interrupts */
WP_U32 tx_happened;
WP_U32 rx_happened;

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
 *      Field Name              Description of field
 *      ----------      --------------------
 *      max_qnodes              Maximum permitted number of queue node objects in the system
 *     virtual_bus         An array of structures providing a mapping of
 *                         each virtual WinPath bus to a physical
 *                              memory bus and bank.
 *      dps_image               Pointer to start of DPS code image for each pair of WinGines
 *  int_queue_data      Pointer to the interrupt queue table array
 *  pool_limit          Specifies the maximum number of buffer pools in the system
 *      atm_data                Pointer to the global atm data structure
 *      iw_data                 Pointer to the global interworking data structure
 *
 *****************************************************************************/
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
       /* WP_VB_STATISTICS,                     */{WP_BUS_HOST, APP_BANK_HOST},
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
       /* WP_VB_IW_FIWT_QAT,                    */{WP_BUS_HOST, APP_BANK_HOST},
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
       /* WP_VB_MCH_DATA,                       */{WP_BUS_NONE, 0},
       /* WP_VB_OAM_FM_QUERY,                   */{WP_BUS_NONE, 0},
       /* WP_VB_HASH_ON_HEAP,                   */{WP_BUS_NONE, 0},
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
    NULL, /* Interrupt queue tables */
    NULL, /* Mixumum number of pools */
    NULL, /* ATM global parameters */
    NULL, /* Interworking global parameters */
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

WP_int_queue_table int_queue_tables =
{
  {
    {24, 1},
    {10, 1},
    {10, 1},
    {10, 1}
  }
};
/*****************************************************************************
 *   WP_port_upi                UPI Port configuration used with WP_PortCreate
 *
 *   This structure defines the port configuration of the UPI. The WDDI manual details
 *   each entry in the structure. In summary, the UTOPIA port is in internal
 * loopback mode (no OC3 card)
 *   or normal mode (using OC3), 16 bit tx and rx data width, round robbin, odd parity,
 *   single (no OC3) or multi phy mode (using OC3). If an OC-3 daughter card is connected,
 *   the OC3 parameter should be enabled and the port will be configured
 *   to run in multi phy and normal mode.  A loopback cable should be attached
 *   to the OC3 card.  Optionally the OC3 card could be put in internal loopback mode.
 *
 *****************************************************************************/

/* Utopia port configuration */
WP_port_atm_limits atm_limits[1] =
{
  {
    64,
    64,
    0,0,0,0,0, 0,0,
    64
  }
};

WP_port_upi_atm_ext upi_atm_ext[1] =
{
   {
      /* atm_characteristics */ WP_UPI_IDLEDISC_ON |  WP_UPI_ATM_USES_ASU |
                                WP_ATM_EXT_PHY_STATS_ENABLE,
      /* utopia_clk */ 50000000,
      /* atm_limits */ atm_limits,
      /* tx_atm_exthdrsz */ 0,
      /* rx_atm_exthdrsz */ 0,
      /* rx_atm_lookupmode */ WP_ATM_LOOKUP_TABLE
   }
};

WP_port_upi_iw_ext iw_ext[1] =
{
  {
    /* iw_characteristics */ WP_IW_BKGND_NOT_USED,
    /* ces_tdm_wpid */ WP_UNUSED
  }
};

WP_port_upi upi_s[1] =
{
   {
      /* upimode */ WP_UPI_UTOPIA,
#if OC3
      /* loopbackmode */          WP_UPI_NORMAL,
#else
      /* loopbackmode */          WP_UPI_LOOPBACK,
#endif
#if OC3
      /* tx_characteristics */ WP_UPI_MULTI        | WP_UPI_MASTER |
                               WP_UPI_PRT_ODD      | WP_UPI_ROBIN  |
                               WP_UPI_BUS_WIDTH_16 | WP_TX_PRIORITY_4,
#else
                               WP_UPI_SINGLE       | WP_UPI_SLAVE |
                               WP_UPI_PRT_ODD      | WP_UPI_ROBIN  |
                               WP_UPI_BUS_WIDTH_16 | WP_TX_PRIORITY_4,
#endif
#if OC3
      /* rx_characteristics */ WP_UPI_MULTI        | WP_UPI_MASTER |
                               WP_UPI_PRT_ODD      | WP_UPI_ROBIN  |
                               WP_UPI_PRTCHK_ON    | WP_UPI_BUS_WIDTH_16,
#else
                               WP_UPI_SINGLE       | WP_UPI_MASTER |
                               WP_UPI_PRT_ODD      | WP_UPI_ROBIN  |
                               WP_UPI_PRTCHK_ON    | WP_UPI_BUS_WIDTH_16,
#endif
      /* atm_ext */ upi_atm_ext,
      /* packet_ext */ NULL,
      /* iw_ext */ iw_ext,
      /* emphy_ext */ NULL
   }
};

/*****************************************************************************
 *      WP_vp_vc_block          ATM VP/VC address lookup range parameters
 *
 *    This structure defines the ATM VP/VC address lookup values that are valid
 *    for the examples. The structure below has one range allowed.
 *    It permits VPI 1 and VPI 4 only with VCI's of 31-36 to be mapped
 *    to specific channels.
 *
 *****************************************************************************/

WP_vp_vc_block vp_vc_range[2] =
{
  {
    /* vpi_1 */ 1,
    /* vpi_2 */ 4,
    /* vci_1 */ 31,
    /* vci_2 */ 36
  },
  WP_VP_VC_BLOCK_TERM
};

/*****************************************************************************
 *      WP_device_upi_atm       UTOPIA ATM device parameters
 *
 *    This structure defines the ATM device specific characterstics of a UTOPIA port
 *    including TX and RX bit rates, traffic class, shaping modes and the VP/VC address
 *    range for this port. In this example TX and RX bit rates can be either 622kbps if
 *    in internal loopback mode or 155kbps if using the OC3 card.
 *
 *****************************************************************************/

WP_device_upi_atm upi_device[1] =
{
  {
#if OC3
    /* tx_bit_rate */ 155000000,
#else
    /* tx_bit_rate */ 622000000,
#endif
    /* tx_slave_addr */ 0,
    /* tx_shaping_mode */ WP_ATM_DEV_SHAPING_VC,
    /* tx_hierarchical_shaping_groups */ 0,
    /* tx_hierarchical_shaping_table */ NULL,
#if OC3
    /* rx_bit_rate */ 155000000,
#else
    /* rx_bit_rate */ 622000000,
#endif
    /* vp_vc_block */ vp_vc_range,
    /* rx_ras_timeout */ 0,
    /* rx_slave_addr */ 0
  }
};

/*****************************************************************************
 *      WP_pool_buffer_data     Buffer Pool parameters
 *
 *    This structure defines a pool used for data buffers including the number, size,
 *    and location of where the buffers will be created
 *    In this example the definition is for 100 buffers, each 144 bytes in length, residing
 *    on the host bus.  For the AAL5 switching application the buffer sizes must be
 *    a multiple of 48 bytes
 *
 *****************************************************************************/
WP_pool_buffer_data buffer_data[] =
{
  {
    /* n_buffers */ 100,
    /* offset */ 0,
    /* size */ 144,
    /* pad */ 0,
    /* bus */ WP_BUS_HOST,
    /* bank */ APP_BANK_HOST
  }
};

/*****************************************************************************
 *      WP_pool_ring_data       Buffer Descriptor ring parameters
 *
 *    This structure defines the buffer descriptor rings controlling the TX and RX of data
 *    for a buffer pool, including the number and length of buffer descriptor rings and the
 *    memory bus where the BD's will be located.
 *    In this example the definition is for 4 rings each with 15 buffer descriptors, residing
 *    residing on the host bus.
 *
 *****************************************************************************/
WP_pool_ring_data ring_data[] =
{
  {
    /* n_rings */ 4,
    /* ring_length */ 15,
    /* bus */ WP_BUS_HOST,
    /* bank */ APP_BANK_HOST
  }
};

/*****************************************************************************
 *      WP_qnode_hostq          AAL5 Host Queue Node parameters
 *
 *    This structure defines QNODE parameters supporting host channels
 *    including the buffer pool and pool ring the Queue Node should use and the interrupt
 *    associated with the QNODE.
 *
 *****************************************************************************/
WP_qnode_hostq qn_host[1] =
{
  {
    /* pool_buffer */ 0,
    /* pool_ring */   0,
    /* interruptqueue */ WP_IRQT0
  }
};


/* Total number of pools that this application will need */
WP_pool_global pool_limit = 8;


/*****************************************************************************
 *  WP_atm_global       Global ATM parameters
 *
 *    This structure defines the global ATM parameters for a particular system and
 *    will determine specific allocation of resources
 *    This structure is used as part of the WinPath context structure.
 *
 *****************************************************************************/
WP_atm_global atm_params[1] =
{
  {
    /* max_pm_tables */ 0,
    /* max_ubrplus_channels */ 0,
    /* max_upc_tables */ 0,
    /* max_rx_cids */ 0,
    /* max_tx_cids */ 10,
    /* max_cid_range */ 10,
    /* qsr_bus */ WP_BUS_NONE,
    /* qsr_bank */ APP_BANK_HOST,
    {  /* modes */
      WP_ATM_RAS_TIMESTAMP_DISABLE,
      WP_AAL2_RX_3FE_DISABLE
    },
    /* max_stats_extensions */ 2,
    /* max_hier_shaping_groups */ 0
  }
};

/*****************************************************************************
 *      WP_atm_shaping_pcr      Peak Cell Rate (PCR) shaping paramters
 *
 *    This structure defines the Peak Cell Rate which will be associated with a
 *    channels shaping parameters. This will be used by the ATM Scheduler to calculate
 *    tranmission rate of cells associated with a specific shaping parameters
 *
 *****************************************************************************/
WP_atm_shaping_pcr shaping_param =
{
  /* pcr */     58962, /* 25Mbps in cells/s */
  /* cdvt */    0,
  /* polling */ 58962
};

/*****************************************************************************
 *      WP_ch_aal5              AAL5 Channel specific parameter
 *
 *    This structure defines AAL5 Channel parameters. The WDDI manual details each entry
 *    in the structure. In summary the first AAL5 channel defined here has VPI=1,VCI=31
 *    (which can be changed before WP_ChannelCreate), is shaped using Peak cell Rate,
 *    Real Time Traffic Class and interrupts are enabled.
 *    Interworking mode is disabled.
 *
 *****************************************************************************/

WP_ch_aal5 ch_config_aal5[2] =
{
  {
    /* vpi */               1,
    /* vci */               31,
    /* intmode */           WP_ATMCH_INT_ENABLE,
    /* iwmmode */           WP_ATMCH_IWM_DISABLE,
    /* statmode */          WP_ATMCH_STAT_ENABLE,
    /* testmode */          WP_ATMCH_TEST_DISABLE,
    /* tx_pqblock */        0,
    /* tx_pqlevel */        0,
    /* tx_shaping_type */   WP_ATM_SHAPING_PCR,
    /* tx_shaping_params */ &shaping_param,
    /* tx_shaping_group */  0,
    /* tx_extheader[12] */  {0},
    /* tx_uumode */         WP_ATMCH_UU_DISABLE,
    /* rx_timestamp */      0,
    /* rx_maxsdu */         1500,
    /* tx_mqblock */        NULL,
    /* tx_mqlevel */        0,
    /* tx_mqweight */       0,
    /* tx_cwid */           WP_CW_ID_A,
    /* tx_tq   */           0
  }
};

/* Number of frames the application will send and receive */
#define NUM_OF_FRAMES 5

/* Number of buffers per frame */
#define NUM_OF_BUFS 10

/* keep track of how many frames are to be received */
WP_U32 pt_receive_size[1] =
{
  NUM_OF_FRAMES
};


/* Application specific tags for the channels */
WP_tag tag_r1 = 0;
WP_tag tag_t1 = 1;


WP_data_segment bufs[NUM_OF_BUFS];
WP_data_segment *curr_buff_ptr;
WP_data_unit data_unit;
WP_handle tx_handle;
WP_handle pool_buffers;
WP_handle rx_handle;
WP_U32 rx_counter = 0;
WP_handle upid1, upidev1, qn1, qn2;
WP_S32 main(WP_S32 argc, WP_CHAR **argv)
{
  WP_handle status;
  WP_handle pool_ring;

  WP_U32 i;

  WP_U32 bytes;

  printf ("UPI AAL5 App:  Running Protocol AAL5 test...\n");

  /*****************************************************************************
   *  Driver Initialisation - WDDI call registers driver with lower level
   *
   *
   *****************************************************************************/
  status = WP_DriverInit();
  terminate_on_error(status, "WP_DriverInit()");

  /*****************************************************************************
   * Initialize the final parts of the WinPath context structure before calling
   * the WP_SysInit function
   *****************************************************************************/
  context->int_queue_data = &int_queue_tables;
  context->atm_data = atm_params;
  context->iw_data = 0;
  context->pool_limit = pool_limit;


#ifdef WP_HW_WINPATH3
   status = WPX_BoardConfigure(0, WPX_CONFIGURE_2UPI_1XGI_10SGMII);
   terminate_on_error(status, "WPX_BoardConfigure()");
#endif


  /*****************************************************************************
   *  WinPath Initialisation - WDDI call uses the context structure to initialise
   *                           WinPath DPS and general WDDI constraints
   *
   *****************************************************************************/
  status = WP_SysInit(WP_WINPATH(0), context);
  terminate_on_error(status, "WP_SysInit()");

  /*****************************************************************************
   *  BRG Clock Initialisation - WDDI call to create output clocks to be used as
   *                             as UPI serial clock source for this application
   *                             Create a 50Mhz output clock on BRG1 using
   *                             the external 100MHz clock as a source for division
   *
   *****************************************************************************/
#ifdef WP_HW_WINPATH2
  status = WP_SysClockCreate(WP_WINPATH(0), WP_BRG3, WP_BRG_SRC_BRGIN2, 2);
  terminate_on_error(status, "WP_SysClockCreate()");
#endif



 /*****************************************************************************
   *  Buffer and Ring Pool Creation - WDDI call to create the buffer and ring pools for
   *                                  this application. The functions use the structures
   *                                  defined earlier in this application
   *
   *****************************************************************************/
  pool_buffers = WP_PoolCreate(WP_WINPATH(0), WP_pool_buffer, buffer_data);
  terminate_on_error(pool_buffers, "WP_PoolCreate() pool_buffers");

  /* Create the Ring pool(s) */
  pool_ring = WP_PoolCreate(WP_WINPATH(0), WP_pool_ring, ring_data);
  terminate_on_error(pool_ring, "WP_PoolCreate() pool_ring");

  /* Attach the Buffer and Ring pools to the qnode */
  qn_host->pool_buffer = pool_buffers;
  qn_host->pool_ring = pool_ring;

  /*****************************************************************************
   *  AAL5 Queue Node Creation -  WDDI call to create the host Queue Node
   *                              The function uses the qn_host_cfg structure defined
   *                              earlier in this application. This application
   *                              creates two qnodes simply to show that multiple
   *                              qnodes can be used.  The TX and RX channels could
   *                              share a single qnode.
   *
   *****************************************************************************/
  /* assign the interrupt queue */
  qn_host->interruptqueue = WP_IRQT0;

  /* Create the Host termination queue node for tx channel */
  qn1 = WP_QNodeCreate(0, WP_QNODE_HOSTQ, qn_host);
  terminate_on_error(qn1, "WP_QNodeCreate() QNODE_HOSTQ");

  /* Create the Host termination queue node for rx channel */
  qn2 = WP_QNodeCreate(0, WP_QNODE_HOSTQ, qn_host);
  terminate_on_error(qn2, "WP_QNodeCreate() QNODE_HOSTQ");

   /*****************************************************************************
    *  UPI Port Creation   - WDDI call to create a UPI port using the structure upi_port_cfg
    *                        defining defines the characterstics of the port. In this
    *                        example the port is created with UPI1
    *****************************************************************************/
  upid1 = WP_PortCreate(WP_WINPATH(0), UPI_PORT_INTERFACE, upi_s);
  terminate_on_error(upid1, "WP_PortCreate() UPI");

  /*****************************************************************************
   *  UPI ATM Device Creation  - WDDI call to create a UPI ATM device using the
   *                             structure upi_device_cfg which defines the particular
   *                             device characteristics. In this example only one ATM
   *                             device is required in Single Phy mode therefore only
   *                             PHY0 is created
   *
   *****************************************************************************/
  upidev1 = WP_DeviceCreate(upid1, WP_PHY(0), WP_DEVICE_ATM, upi_device);
  terminate_on_error(upidev1, "WP_DeviceCreate() UPIATM");

   /*****************************************************************************
   * Register Interrupt Handlers - The following functions attach a callback function
   *                               to events which may be found in the interrupt queues
   *
   *****************************************************************************/
  status = WP_ControlRegister(WP_EVENT_TX_INDICATE, &WP_AppIndicateEventTx);
  terminate_on_error(status, "WP_Control Register()");

  status = WP_ControlRegister(WP_EVENT_RX_INDICATE, &WP_AppIndicateEventRx);
  terminate_on_error(status, "WP_Control Register()");


  WPE_CreateSchedulingUnit();

  /*****************************************************************************
   *  System Commit - WDDI call to commit the WinPath setup
   *
   *****************************************************************************/
  printf("Executing System Commit\n");
  status = WP_SysCommit();
  terminate_on_error(status, "WP_SysCommit()");

  #ifdef PSU_MODE
      status = WP_SysPsuEnable(WP_WINPATH(0));
      terminate_on_error(status, "WP_SysPsuEnable()");
  #else
      status = WP_SysAsuEnable(WP_WINPATH(0));
      terminate_on_error(status, "WP_SysAsuEnable()");
  #endif



  /* initialize the oc3 card. */
#if OC3
  WPU_Oc3Pmc5351CardInit (UPI_PORT_INTERFACE, WPU_MODE_OC3_ATM | WPU_MODE_OC3_PHY0_LOOPBACK_OFF);
#endif
  /*****************************************************************************
   *  UPI Port Enable - WDDI call to enable the created UPI device. In this example
   *                    the port operates in full duplex mode
   *
   *****************************************************************************/
  status = WP_PortEnable(upid1, WP_DIRECTION_DUPLEX);
  terminate_on_error(status, "WP_PortEnable() UPI");

  status = WP_DeviceEnable(upidev1, WP_DIRECTION_DUPLEX);
  terminate_on_error(status, "WP_DeviceEnable() (UPI)");
  /*****************************************************************************
   * WP Memory Stats - Calls to the API to display memory available on the buses.
   *
   ****************************************************************************/
  status = WP_MemoryAvailable(WP_WINPATH(0), WP_BUS_PACKET, APP_BANK_PACKET, &bytes);
  printf("UPI AAL5 App:  Packet bus bytes left = %d\n", bytes);

  status = WP_MemoryAvailable(WP_WINPATH(0), WP_BUS_PARAM, APP_BANK_PARAM, &bytes);
  printf("UPI AAL5 App:  Param bus bytes left = %d\n", bytes);

  status = WP_MemoryAvailable(WP_WINPATH(0), WP_BUS_INTERNAL, APP_BANK_INTERNAL, &bytes);
  printf("UPI AAL5 App:  Internal bus bytes left = %d\n", bytes);

  status = WP_MemoryAvailable(WP_WINPATH(0), WP_BUS_HOST, APP_BANK_HOST, &bytes);
  printf("UPI AAL5 App:  Host bus bytes left = %d\n", bytes);

  /*****************************************************************************
   *  ATM Scheduler Enable - WDDI call to enable the previously created ATM Scheduling Unit
   *
   *****************************************************************************/
  status = WP_SysAsuEnable(WP_WINPATH(0));
  terminate_on_error(status, "WP_SysAsuEnable()");

  /*****************************************************************************
   * AAL5 Channel Creation - WDDI call to create Tx and Rx AAL5 channels
   *
   *
   *
   *****************************************************************************/
  tx_handle = WP_ChannelCreate(tag_t1, upidev1, qn1, WP_CH_TX, WP_ATM_AAL5, &ch_config_aal5[0]);
  terminate_on_error(tx_handle, "WP_ChannelCreate() AAL5 Tx");

  rx_handle = WP_ChannelCreate(tag_r1, upidev1, qn2, WP_CH_RX, WP_ATM_AAL5, &ch_config_aal5[0]);
  terminate_on_error(rx_handle, "WP_ChannelCreate() AAL5 Rx");

  /*****************************************************************************
   *  Channel enable  -      These functions enable the Tx and Rx channels
   *                         The Rx channel is enabled first to ensure that transmitted
   *                         traffic can be received
   *
   *****************************************************************************/
  status = WP_ChannelEnable(rx_handle);
  terminate_on_error(status, "WP_ChannelEnable() AAL5 Rx");

  status = WP_ChannelEnable(tx_handle);
  terminate_on_error(status, "WP_ChannelEnable() AAL5 Tx");

  /*****************************************************************************
   * Prepare Tx and Rx buffer pointers - This section of code initialises the TX
   *                                     and RX buffer pointers for each of the packets
   *                                     in this application
   *
   *
   *****************************************************************************/
    for (i = 0; i < NUM_OF_BUFS; ++i) /* cycle through all buffers */
    {
      bufs[i].next = &bufs[i+1];
      bufs[i].displacement = 0;
    }
    bufs[NUM_OF_BUFS-1].next = NULL;


  menu_interface();

  return 0;
}

void terminate_on_error(WP_handle handle, WP_CHAR *s)
{
  if (WP_ERROR_P(handle)) {
    printf("UPI AAL5 App: Terminal error (handle %#8.8x) %s\n", handle, s);
    printf("UPI AAL5 App: Error type - %s\n", WP_error_name[WP_ERROR(handle)]);
    WP_DriverRelease();
    while(1);
    return;
  } else {
    printf("UPI AAL5 App: Handle/Status returned from %s is %#8.8x \n", s, handle);
  }
}

/*****************************************************************************
 *   The following two functions need to be defined to handle interrupt
 *   events from Winpath. This application makes use of rx interrupts to receive
 *   frames only when they are present on the rx channel.
 ******************************************************************************/
void WP_AppIndicateEventTx(WP_tag tag, WP_U32 data, WP_U32 info) {
  tx_happened = 1;
  return;
}

void WP_AppIndicateEventRx(WP_tag tag, WP_U32 data, WP_U32 info) {
  rx_happened = 1;
  return;
}


void WPE_CreateSchedulingUnit(void)
{
    WP_status   status;
    WP_calendar_wheel calendar[2] =
    {
        {
#ifdef PSU_MODE
            /* function */    WP_CW_PSU,
#else
            /* function */    WP_CW_ASU,
#endif
            /* cwid*/         WP_CW_ID_A,
            /* min_ch_rate */ 10000,    /* kbps / 424 */
            /* max_ch_rate */ 117925, /* 50Mbps / 424 */
        },
        WP_CALENDAR_WHEEL_TERM
    };
    status  = WP_SysSchedulerCreate(WP_WINPATH(0), calendar);
    terminate_on_error(status, "WP_SysSchedulerCreate()");
}

void CLI_ReceivePacket_Interface1(void)
{
    WP_status status;
    WP_U32 k = 0, l = 0;

    /*****************************************************************************
     *  Host Reception of Packets - The SimulateInterrupt function will trigger the
     *                              interrupt handler. The handler sets a rx_happened.
     *                              If the flag is set the following piece of code will
     *                              service the event and store the data from the receive
     *                              channel.
     *
     *****************************************************************************/

    data_unit.segment = &bufs[0];
    data_unit.n_segments = NUM_OF_BUFS;
    data_unit.type = WP_DATA_AAL5;
    curr_buff_ptr = data_unit.segment;
    if (rx_happened) {
      status = WP_HostReceive (rx_handle, &data_unit);
      terminate_on_error(status, "WP_HostReceive()");
      printf("UPI AAL5 App: Received frame:\n");
      for (k = 0; k < data_unit.n_active; ++k, ++curr_buff_ptr) {
        for (l = 0; l < curr_buff_ptr->data_size; ++l) {
          printf("%2.2x",curr_buff_ptr->data[l]);
        }
         /******************************************************************
         *
         * After WP_HostReceive returns, the data unit n_active field has
         * the number of buffers returned.WP_PoolFree frees the buffers.
         * ****************************************************************/
         WP_PoolFree(curr_buff_ptr->pool_handle, curr_buff_ptr->data);
         printf("(%d bytes) \n", curr_buff_ptr->data_size);
      }
      rx_happened = 0;  /* reset the interrupt event flag */
      ++rx_counter; /* count frame received */
    } else {
    status = WP_HostReceive (rx_handle, &data_unit);
    if (status != WP_OK) {
      printf("Packet not received\n");
    }
    else {
      for (k = 0; k < data_unit.n_active; ++k, ++curr_buff_ptr) {
         /******************************************************************
         *
         * After WP_HostReceive returns the data unit n_active field has
         * the number of buffers returned.
         * ****************************************************************/
         WP_PoolFree(curr_buff_ptr->pool_handle, curr_buff_ptr->data);
      }
      printf("Cells received, interrupts failed\n");
      return;
    }
  }
}

void CLI_SendPacket_Interface1(void)
{
  WP_status status;
  WP_S32 i,k,l;

    /*****************************************************************************
     * Prepare Transmit buffers - This section of the code initialises the contents
     *                            of the Tx buffers for each of the packets sent in this
     *                            application. Also initialised is the pointer to the first
     *                            Tx buffer for transmission
     *
     *****************************************************************************/
    data_unit.data_size = 0;
    data_unit.n_active = 0;
    for (i = 0; i < NUM_OF_BUFS; ++i)
    {
      bufs[i].data_size = i + 5;
      bufs[i].data = WP_PoolAlloc(pool_buffers);
      bufs[i].pool_handle = pool_buffers;

      memset(bufs[i].data, '0' + i, bufs[i].data_size);

      data_unit.data_size += bufs[i].data_size;
      ++data_unit.n_active;
    }

    /*****************************************************************************
     * Send Transmit buffers - This section of code performs the function that enables
     *                         the transmission of a particular channel
     *
     *****************************************************************************/
    data_unit.segment = &bufs[0];
    data_unit.n_segments = NUM_OF_BUFS;
    data_unit.type = WP_DATA_AAL5;
    data_unit.control = WP_HT_INTERRUPT | WP_HT_UU_FIELD(0x33);
    printf("Performing HostSend()\n");
    printf("tx_handle is %08x\n",(WP_U32)tx_handle);
    status = WP_HostSend(tx_handle, &data_unit);
    terminate_on_error(status, "WP_HostSend()");
    /*****************************************************************************
     * Simulate Interrupts - WinMon is unable to process interrupts hence the following
     *                       function, WPI_SimulateInterrupts, looks into the interrupt
     *                       queues and calls the callback functions associated with the
     *                       events present on the queue
     *                       If this test is run on an O/S that supports interrupts then the
     *                       call to WPI_SimulateInterrupts should be removed.
     *
     * **************************************************************************/
     WPI_SimulateInterrupts();

    /*************************************************************
     *
     * Host recover checks the tx channel's ring for buffers that have
     * been transmitted.  These buffers are "attached" to the data unit
     * passed into WP_HostRecover.  The host then frees these buffers
     * back to the pool they came from.
     *
     * It is possible to receive no buffers or many buffers when
     * calling WP_HostRecover.
     **************************************************************/
    curr_buff_ptr = data_unit.segment;
    for (k = 0; k < data_unit.n_active; ++k, ++curr_buff_ptr)
    {
        for (l = 0; l < curr_buff_ptr->data_size; ++l)
        {
          printf("%2.2x",curr_buff_ptr->data[l]);
        }
    }

    curr_buff_ptr = data_unit.segment;
    status = WP_HostRecover(tx_handle, &data_unit);
    terminate_on_error(status, "WP_HostRecover()");

    for (i = 0; i < data_unit.n_active; i++)
    {
      WP_PoolFree(curr_buff_ptr->pool_handle, curr_buff_ptr->data);
      curr_buff_ptr = curr_buff_ptr->next;
    }
}

void CLI_ProgramQuit(void)
{
  WP_status status;
  status = WP_DriverRelease();
  return;
}

void CLI_PrintStatistics(void)
{
    printChannelStat(rx_handle);
    printChannelStat(tx_handle);
    printAtmDeviceStats(upidev1);
}

Y_MenuEntry V_MainMenu[] =
{
  {5     , TRUE, "UPI AAL5 Test",       {V_MainMenu}},
  {K_Leaf, TRUE, "Send Packet UPI",    {CLI_SendPacket_Interface1}},
  {K_Leaf, TRUE, "Receive Packet UPI", {CLI_ReceivePacket_Interface1}},
  {K_Menu, TRUE, "Utilities and Debug", {V_Menu_Debug}},
  {K_Leaf, TRUE, "Dump Statistics",     {CLI_PrintStatistics}},
  {K_Leaf, TRUE, "Quit the program",    {CLI_ProgramQuit}}
};

void menu_interface()
{
   static char InputBuf[512];
   F_SetMainMenu();
   F_DisplayMenu();
   while(TRUE)
   {
     printf("%s",STR_ENTER_COMMAND);
     memset(InputBuf,0,512);
     gets(InputBuf);
     printf("\n");
     if(strcmp(InputBuf,"cont") == 0)
      break;
     F_MenuEngine(InputBuf);
   }
}

