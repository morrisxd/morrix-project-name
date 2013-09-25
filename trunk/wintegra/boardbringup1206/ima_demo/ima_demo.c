/*************************************************************
 * (C) Copyright 2000-2009, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ************************************************************/
#define IMA_DEBUG 0
/*****************************************************************************
 *
 * Unique Test Number: 26210
 *
 * Test File: 26210-ima_demo.c
 *
 * Test Requirement addressed:
 *
 * Test Description:
 * -----------------
 *
 *
 * Test Traffic Setup:
 * ------------------
 *
 * Test Output:
 * ------------
 *
 ****************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef USE_SOCKET
#include <unistd.h>
#include "sockint.h"
#endif
#include "wp_wddi.h"
#include "wp_host_io.h"

#include "wpx_tdm_comet.h"

#include "wp_sim.h"
#include "wpx_app_data.h"

#include "wpx_oc3_pmc5351.h"
#include "wpx_oc12_pmc5357.h"

/* Specify which test features are selected.  */

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
#ifndef _WT_IMA_DEBUG_H_
#define _WT_IMA_DEBUG_H_
typedef enum{
  ICP_ATM_HEADER          =      1,
  ICP_OAM_LABEL           =      5,
  ICP_CELL_LINK_ID        =      6,
  ICP_FRAME_SEQUENCE_NO   =      7,
  ICP_CELL_OFFSET         =      8,
  ICP_LINK_INDICATION     =      9,
  ICP_STATUS_INDICATION   =      10,
  ICP_IMA_ID              =      11,
  ICP_GROUP_STATUS        =      12,
  ICP_TRANSMIT_TIMING     =      13,
  ICP_TX_TEST_CONTROL     =      14,
  ICP_TX_TEST_PATTERN     =      15,
  ICP_RX_TEST_PATTERN     =      16,
  ICP_LINK_INFO           =      17,
  ICP_END_TO_END_CHANNEL  =      50,
  ICP_CRC_ERROR_CONTROL   =      51
}ICP_cell_format;

typedef enum{
  ICP_DECODE_HEADER   = 0,
  ICP_DECODE_OAM_LABEL,
  ICP_DECODE_CELL_LINK_ID,
  ICP_DECODE_FRAME_SEQUENCE_NUMBER,
  ICP_DECODE_ICP_CELL_OFFSET,
  ICP_DECODE_LINK_STUFF_INDICATION,
  ICP_DECODE_STATUS_CHANGE_INDICATION,
  ICP_DECODE_IMA_ID,
  ICP_DECODE_GROUP_STATUS_AND_CONTROL,
  ICP_DECODE_TRANSMIT_TIMING_INFO,
  ICP_DECODE_TX_TEST_CONTROL,
  ICP_DECODE_TX_TEST_PATTERN,
  ICP_DECODE_RX_TEST_PATTERN,
  ICP_DECODE_LINK_INFO,
  ICP_DECODE_END_TO_END_CHANNEL,
  ICP_DECODE_CRC_ERROR_CONTROL,
  ICP_DECODE_ALL
 }ICP_decode_commands;

void CLI_IMA_DecodeAlarmState(WP_U16 state);
void CLI_IMA_DecodeAlarmType(WP_U16 type);
void CLI_IMA_DecodeOAM(WP_U16 oam_label);
void CLI_IMA_DecodeSymmetry(WP_U16 symmetry);
void CLI_IMA_DecodeGroupControl(WP_U16 group_control);
void CLI_IMA_DecodeFrameLength(WP_U16 frame_size);
void CLI_IMA_DecodeClockMode(WP_U16 clock_mode);
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
#define WT_USE_INTERRUPTS 1
#ifdef QUICK
#define WT_USE_TDM_CARD 1
#else
#define WT_USE_TDM_CARD 1
#endif
#define GROUP_OPERATIONAL 1
#define GROUP_NOT_OPERATIONAL 0

#define IRQ_IMA WP_IRQT3
#define IRQ_CH  WP_IRQT2

/* ---------------------------------------------------------------------*/
/* THE FOLLOWING DEFINITIONS MUST BE PRESENT FOR EVERY TEST APPLICATION */
/* ---------------------------------------------------------------------*/

/* define the number of poll cycles for a receive */
#define NUM_POLL_CYCLES 1000

#define RX_OBJ_TBL_SIZE AAL0_NUM_VC
#define TX_OBJ_TBL_SIZE AAL0_NUM_VC

/* ---------------------------------------------------*/
/* APPLICATION SPECIFIC CONFIGURATION AND DEFINITIONS */
/* ---------------------------------------------------*/

#define TEST_NUMBER 1

/* TDM/PHY Clock mode */
#define PHY_TYPE_E1 1

#if PHY_TYPE_E1
#define NUM_BINDINGS 2
#define CELL_TIME WP_IMA_E1_CELL_TIME
#define COMET_MODE WPU_MODE_TDM_E1_MASTER
#define CPLD_MODE  WPU_INIT_COMET_CPLD_E1
#define APP_PORT_TDM_BASE WP_PORT_TDM1
#define TDM_DELAY 3
#else
#define NUM_BINDINGS 1
#define CELL_TIME WP_IMA_T1_CELL_TIME
#define COMET_MODE WPU_MODE_TDM_T1_MASTER
#define CPLD_MODE  WPU_INIT_COMET_CPLD_T1
#define APP_PORT_TDM_BASE WP_PORT_TDM1
#define TDM_DELAY 1
#endif

#define MODIFIERS 0

/* Trial/Test number specifications */
#ifndef WT_TRIAL_MASK
#define WT_TRIAL_MASK 0xffffffff
#endif

/* Since the regression tests allow only for 1 external */
/* TDM port, we limit the number of groups and links to */
/* 1.                                                   */
#define NUM_OF_GROUPS 2
#define NUM_OF_LINKS 4
#define NUM_OF_CONFIGS 2
#define MIN_NUM_OF_LINKS 1
#define LOOKUP_MODE WP_ATM_LOOKUP_HASH

enum WP_IMA_GROUP_CONFIGS
{
  WP_IMA_FRAME_SIZE = 0,
  WP_IMA_SYMMETRY,
  WP_IMA_VERSION,
  WP_IMA_TX_CLOCK_MODE,
  WP_IMA_MIN_TX_LINKS,
  WP_IMA_MIN_RX_LINKS,
  WP_IMA_ALPHA,
  WP_IMA_BETA,
  WP_IMA_GAMMA,
  WP_IMA_CELL_PERIOD,
  WP_IMA_BUNCH_DELAY,
  WP_IMA_MAX_DELAY,
  WP_IMA_MODIFIERS,
  WP_IMA_GSM_TIMEOUT,
  WP_IMA_SCCI_AUDIT_TIMEOUT,
};

extern Y_MenuEntry V_Menu_Debug[];

WP_CHAR *STR_MENU_HDR = {"  IMA Test"};

void CLI_ToggleTTS(void);
void CLI_ToggleTOS(void);
void CLI_ToggleOperationChange(void);
void CLI_ToggleTxQempty(void);
void CLI_ToggleTxQfull(void);
void CLI_ToggleRxGSU_LASR(void);
void CLI_ToggleTxGSU_LASR(void);
void CLI_ToggleFE_ICPchange(void);
void CLI_ToggleNE_ICPchange(void);
void link_del_action(WP_ima_event *task);
void CLI_IMA_PrintGroupState(WP_ima_group_state_info * grp_state);
void CLI_TestTDMChannel(WP_U32 group);
void print_link_statistics(WP_ima_link_statistics * link_statistics);
void menu_interface(void);
void CLI_ProgramQuit(void);
void ICP_Decode(WP_ima_icp_cell * icp_ptr, WP_U32 cmds, WP_U32 extra_options);
void tdm_stats_print(WP_stats_tdi_atm *tdi_atm_stats);
void CLI_IMA_DeleteOneLink(WP_U32 group, WP_U32 link);
struct trial {
       char *name;
       WP_U8 ima_version;
       WP_U8 frame_size;
       WP_U8 tx_clock_mode;
       WP_U8 tx_data;
} trials [] = {
/* 26008 Test 1 */
#if WT_TRIAL_MASK & (1 << 1)
     {
       "IMA Version 1.1, CTC, Frame size 128 trial",
       WP_IMA_VERSION_1_1,
       WP_IMA_FRAME_SIZE_128,
       WP_IMA_TX_CLOCK_CTC,
       0x11
     },
#endif

/* 26008 Test 2 */
#if WT_TRIAL_MASK & (1 << 2)
     {
       "IMA Version 1.1, CTC, Frame size 32 trial",
       WP_IMA_VERSION_1_1,
       WP_IMA_FRAME_SIZE_32,
       WP_IMA_TX_CLOCK_CTC,
       0x22
     },
#endif

/* 26008 Test 3 */
#if WT_TRIAL_MASK & (1 << 3)
     {
       "IMA Version 1.1, CTC, Frame size 256 trial",
       WP_IMA_VERSION_1_1,
       WP_IMA_FRAME_SIZE_256,
       WP_IMA_TX_CLOCK_CTC,
       0x33
     },
#endif

/* 26008 Test 4 */
#if WT_TRIAL_MASK & (1 << 4)
     {
       "IMA Version 1.1, CTC, Frame size 64 trial",
       WP_IMA_VERSION_1_1,
       WP_IMA_FRAME_SIZE_64,
       WP_IMA_TX_CLOCK_CTC,
       0x44
     },
#endif
     {NULL}
};

/*************************************************
 * Function: private debugging
 *************************************************/
#define PRIVATE_START 0x0e000000
#define PRIVATE_HSTART (0xa0000000 + PRIVATE_START)
#define PRIVATE_END   0x0efffff8
#define PRIVATE_HEND   (0xa0000000 + PRIVATE_END)
#define PRIVATE_INT_BASE  0xbd022000
static void private_int_queue(void)
{
#if IMA_DEBUG
#define PRIVATE_BUS 1
#define PRIVATE_QUEUE 2
   WP_U32 *base = (WP_U32 *) (PRIVATE_INT_BASE +
                              PRIVATE_BUS * 0x1000 + 0x800 +
                              4 * PRIVATE_QUEUE);
   WP_U32 *b;

   printf("writing %x to %p\n", PRIVATE_START, base);
   base[0] = PRIVATE_START;
   base[8] = PRIVATE_START;
   base[16] = PRIVATE_END;
   for (b = (WP_U32 *) PRIVATE_HSTART; b <= (WP_U32 *) PRIVATE_HEND; b += 2)
   {
      b[0] = 0x40000000;
      b[1] = 0;
   }
#endif
}

void print_icp(WP_ima_icp_cell *icp);

/* Number of IMA DPS generated ticks in 1 second */
#define TICKS_PER_SECOND 2
#define SECONDS(n) (n * TICKS_PER_SECOND)

/* Number of of channels to switch. */
#define NUM_OF_SWCH 2

/* Peak cell rate used for each link */
#define PCR 4000
#define SHAPED 0
#define CLP_CELL_THRESHOLD 100

extern WP_U32 dps_ProgramImage[];
extern WP_U16 dps_PC_Table_Init[];

/* An array of one WP_context structure for each WinPath */
WP_context context[1] = {
   {6,                          /* qnodes */
    /* the first four assignments are mandatory bus assignments */
    {
     { /* WP_BUS_PARAM    */ WP_BUS_PARAM, APP_BANK_PARAM},
     { /* WP_BUS_PACKET   */ WP_BUS_PACKET, APP_BANK_PACKET},
     { /* WP_BUS_INTERNAL */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
     { /* WP_BUS_HOST     */ WP_BUS_HOST, APP_BANK_HOST},
     { /* WP_BUS_IRQ      */ WP_BUS_HOST, APP_BANK_HOST},
     { /* WP_BUS_TMSTMP   */ WP_BUS_HOST, APP_BANK_HOST},
     { /* UNASSIGNED      */ WP_BUS_NONE, 0},
     { /* UNASSIGNED      */ WP_BUS_NONE, 0},
     { /* ASU_TPT         */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
     { /* ASU CW          */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
     { /* ASU TX QUEUE    */ WP_BUS_PARAM, APP_BANK_PARAM},
     { /* STATISTICS      */ WP_BUS_HOST, APP_BANK_HOST},
     { /* ADDRESS LOOKUP  */ WP_BUS_PARAM, APP_BANK_PARAM},
     { /* ATM CPT         */ WP_BUS_PARAM, APP_BANK_PARAM},
     { /* ENET HDLC CPT   */ WP_BUS_PARAM, APP_BANK_PARAM},
     { /* IW FBP PARAM    */ WP_BUS_PARAM, APP_BANK_PARAM},
#ifdef WP_BOARD_WDS3_SL
     { /* IW FBP DATA     */ WP_BUS_PARAM, APP_BANK_PARAM},
     { /* EXTERNAL FIFO   */ WP_BUS_PARAM, APP_BANK_PARAM},
#else
     { /* IW FBP DATA     */ WP_BUS_PACKET, APP_BANK_PACKET},
     { /* EXTERNAL FIFO   */ WP_BUS_PACKET, APP_BANK_PACKET},
#endif
     { /* MAC LOOKUP      */ WP_BUS_PARAM, APP_BANK_PARAM},
     { /* IMA DISPATCH    */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
     { /* IMA SHARED      */ WP_BUS_PARAM, APP_BANK_PARAM},
     { /* LPM             */ WP_BUS_HOST, APP_BANK_HOST},
     { /* MFC RULES       */ WP_BUS_HOST, APP_BANK_HOST},
     { /* FIWT, QAT       */ WP_BUS_PARAM, APP_BANK_PARAM},
     { /* MFCPT, UPPPT, DS */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
     { /* STATISTICS2      */ WP_BUS_HOST, APP_BANK_HOST},
     { /* STATISTICS3/IMA Port */ WP_BUS_HOST, APP_BANK_HOST},
     { /* IMA RX Bus      */ WP_BUS_HOST, APP_BANK_HOST},
     { /* UNASSIGNED      */ WP_BUS_NONE, 0},
     { /* WP_VB_AAL2_QSR/IMA Tx Buf  */ WP_BUS_HOST, APP_BANK_HOST},
     { /* MANDATORY       */ WP_BUS_NONE, 0},
     { /* MANDATORY       */ WP_BUS_NONE, 0}
     },
    {
     dps_ProgramImage, dps_ProgramImage},
    {
     dps_PC_Table_Init, dps_PC_Table_Init},
    }
};

/* Interrupt queue configuration used with WP_SysInit */
WP_int_queue_table int_queue_tables =
   { {{200, 1}, {8192, 1}, {10, 1}, {10, 1}} };

/* Pool to manage the data buffers required by this application */
/* Note the for AAL5 the application buffers sizes must be a
   multiple of 48 bytes */

WP_pool_buffer_data buffer_data[] = {
   {/* n_buffers */ 100,
    /* offset */ 0,
    /* size */ 144,
    /* pad */ 0,
    /* bus */ WP_BUS_HOST,
    /* bank */ APP_BANK_HOST}
};

/* Pool to manage data buffer rings in Winpath from where data is transmitted
   and received */
WP_pool_ring_data ring_data[] = {
   {/* n_rings */ 4,
    /* ring_length */ 15,
    /* bus */ WP_BUS_HOST,
    /* bank */ APP_BANK_HOST}
};

/* A Host termination queue node for the AAL5 channels */
WP_qnode_hostq qn_host[1] =
{
   {
      /* pool_buffer */ 0,
      /* pool_ring */ 0,
      /* interruptqueue */ IRQ_CH,
   }
};

/* Total number of pools that this application will need */
WP_pool_global pool_limit = 8;

/* Global atm_data for the atm channels created by this application */
WP_atm_global atm_params[1] = {
   {
    /* max_pm_tables */ 8,
    /* max_ubrplus_channels */ 8,
    /* max_upc_tables */ NUM_OF_SWCH,
    /* max_rx_cids */ 0,
    /* max_tx_cids */ 0,
    /* max_cid_range */ 0,
    /* AAL2 QSR bus */ WP_BUS_NONE,
    /* AAL2 QSR bank */ 0
    }
};

/* Shaping configuration for the AAL5 channels */
WP_atm_shaping_pcr aal5_shaping_param =
{
   /* pcr */     3000,
   /* cdvt */    0,
   /* polling */ 3000
};

/* Shaping configuration for the AAL5 channels */
#if SHAPED
WP_atm_shaping_pcr shaping_param = {
   /* pcr */ (NUM_OF_GROUPS * MIN_NUM_OF_LINKS * PCR),
   /* x mbps in cells/s */
   /* cdvt */ 0,
   /* polling */ WP_SHAPING_POLL_AUTOD
};
#else
WP_atm_shaping_unshaped shaping_param = {
   /* weight */ 1
#endif
};

/* Host Terminated Channel, AAL5 */
WP_ch_aal5 ch_config_aal5[1] =
{
   {
      /* vpi */ 8,
      /* vci */ 34,
      /* intmode */ WP_ATMCH_INT_ENABLE,
      /* iwmmode */  WP_ATMCH_IWM_DISABLE,
      /* statmode */ WP_ATMCH_STAT_DISABLE,
      /* testmode */ WP_ATMCH_TEST_DISABLE,
      /* tx_pqblock */ 0,
      /* tx_pqlevel */ 0,
      /* tx_shaping_type */ WP_ATM_SHAPING_PCR,
      /* tx_shaping_params */ &aal5_shaping_param,
      /* tx_shaping_group */ 0,
      /* tx_extheader[12] */ {0},
      /* tx_uumode */ WP_ATMCH_UU_DISABLE,
      /* rx_timestamp */ 0,
      /* rx_maxsdu */ 1500
   }
};

#define MAX_CHANNEL_RATE (NUM_OF_GROUPS * NUM_OF_LINKS * PCR)

WP_calendar_wheel calendar_wheel_cfg[2] =
{
   {
      /* function */    WP_CW_ASU,
      /* cwid*/         WP_CW_ID_A,
      /* min_ch_rate */ 150,
      /* max_ch_rate */ MAX_CHANNEL_RATE
   },
   WP_CALENDAR_WHEEL_TERM
};

#if PHY_TYPE_E1
/* TDM Port configuration */
/* 32 slots for E1        */
WP_tdm_slotgroup tdm_slots[] =
{
   {/* repeat */ 1, /* size */ 1},
   {/* repeat */ 1, /* size */ 15},
   {/* repeat */ 1, /* size */ 1},
   {/* repeat */ 1, /* size */ 15},
   {/* repeat */ 0, /* size */ 0}
};
#else
/* TDM Port configuration */
/* 24 slots for T1        */
WP_tdm_slotgroup tdm_slots[] = {
   { /* repeat */ 1, /* size */ 24},
   { /* repeat */ 0, /* size */ 0}
};
#endif

WP_port_tdm tdm_setup[1] = {
   {
    /* atm_limits */ {0, 0, 0, 0, 0, 0},
    /* pkt_limits */ {0, 0, 0, 0},
    /* trans_limits */ {{0}, {0}},
    /* slot */ tdm_slots,
    /* tdmmode */ WP_TDM_NORMAL,
    /* framemode */ WP_TDM_FRAMED,
    /* pinconfig */ WP_TDM_COMMON,
    /* edge */ WP_TDM_RISING,
    /* delay */ TDM_DELAY,
    /* intmode */ WP_ATMCH_INT_ENABLE,
    /* rx_iw_bkgnd */ WP_IW_BKGND_NOT_USED
    }
};

#if PHY_TYPE_E1
/* TDM Atm device configuration */
WP_tdm_binding atm_binding[] = {
   {1, 15}, {17,31}
};
#else
/* TDM Atm device configuration */
WP_tdm_binding atm_binding[] = {
   {0, 23}
};
#endif

/* permit vpi's 8-10, each with vci's 34-39 */
WP_vp_vc_block tdm_vp_vc_range[2] = {
   {8, 10, 34, 39},
   WP_VP_VC_BLOCK_TERM
};

/* Configuration information for the TDM Atm device */
WP_device_tdm_atm tdm_device[1] = {
   {
    /* n_timeslot_binding */ NUM_BINDINGS,
    /* timeslot_binding */ atm_binding,
    /* scramblemode */  WP_TDM_SCRAMBLE_ON,
    /* cosetmode */ 1,
    /* idlemode */ 1,
    /* alpha */ 5,
    /* delta */ 3,
    /* tx_shaping_mode */ 0,
    /* tx_hierarchical_shaping_groups */ 0,
    /* tx_hierarchical_shaping_table */ NULL,
    /* lookupmode */ LOOKUP_MODE,
    /* vp_vc_block */ NULL,
   }
};

/* permit vpi's 8-10, each with vci's 34-39 */
WP_vp_vc_block ima_vp_vc_range[2] = {
   {8, 10, 34, 39},
   WP_VP_VC_BLOCK_TERM
};

/* Group Resource Allocation parameters, prior to system commit */
WP_ima_group_alloc group_alloc[2] = {

   {
    /* tx_shaping_mode */ WP_ATM_DEV_SHAPING_VC,
    /* tx_hierarchical_shaping_groups */ 0,
    /* tx_hierarchical_shaping_table */ NULL,
    /* lookupmode */ LOOKUP_MODE,
    /* extended phy stats  */ WP_ATM_EXT_PHY_STATS_DISABLE,
    /* Re-assembly timeout */ 0
    },
    WP_IMA_GROUP_ALLOC_TERM
};

/* Configuration information for the IMA system */
WP_ima_system system_params = {
   /* atm_limits */                       {NUM_OF_SWCH, NUM_OF_SWCH, 0, 0, 0, 0},
   /* port_in_use                       */ WP_PORT_TDM1,
   /* number of groups in the system    */ 8,
   /* max number of links in the system */ 8,
   /* Tx queue depth, in # of cells.    */ 8,
   /* max. delay tolerated in ms        */ 10,
   /* Min. Cell length, in micro-secs.  */ CELL_TIME,
   /* Alarm enter persistance time = 2.5 sec */ 2500,
   /* Alarm exit persistance time = 10 sec   */ 10000,
   /* Test Pattern Time out period = 1 sec   */ 1,
   /* Number of lookup Tables                */ 1,
   /* VPI/VCI range used for lookup          */ ima_vp_vc_range,
   /* Allocation requirements                */ group_alloc,
   /* Tick Period = 1/2 second               */ 500,
   /* Interworking bacground mode            */ WP_IW_BKGND_NOT_USED,
   /* Interrupt queue                        */ IRQ_IMA
};

/* Configuration information for the IMA group(s). */
WP_ima_group ima_group[NUM_OF_CONFIGS] = {
   /* Configuration information for 1st group. */
   {
    /* IMA ID (0-255)   */ 0,
    /* frame size       */ WP_IMA_FRAME_SIZE_128,
    /* Group Symmetry   */ WP_IMA_SYM_CFG_SYM_OP,
    /* IMA version      */ WP_IMA_VERSION_1_1,
    /* Tx Clock mode    */ WP_IMA_TX_CLOCK_CTC,
    /* Min Tx. Links    */ MIN_NUM_OF_LINKS,
    /* Min Rx. Links    */ MIN_NUM_OF_LINKS,
    /* Alpha            */ 2,
    /* Beta             */ 2,
    /* Gamma            */ 1,
    /* Link Cell length */ CELL_TIME,
    /* Bunch delay      */ 500000,
    /* Max delay - grp  */ 5,
    /* VPI/VCI range    */ ima_vp_vc_range,
    /* trafic config    */ &group_alloc[0],
    /* modifiers        */ MODIFIERS,
    /* gsm_timeout      */ 10,
    /* scci_audit_timeout */ 10
    },
   /* Configuration information for 2nd group. */
   {
    /* IMA ID (0-255)   */ 1,
    /* frame size       */ WP_IMA_FRAME_SIZE_128,
    /* Group Symmetry   */ WP_IMA_SYM_CFG_SYM_OP,
    /* IMA version      */ WP_IMA_VERSION_1_1,
    /* Tx Clock mode    */ WP_IMA_TX_CLOCK_CTC,
    /* Min Tx. Links    */ MIN_NUM_OF_LINKS,
    /* Min Rx. Links    */ MIN_NUM_OF_LINKS,
    /* Alpha            */ 2,
    /* Beta             */ 2,
    /* Gamma            */ 1,
    /* Link Cell length */ CELL_TIME,
    /* Bunch delay      */ 500000,
    /* Max delay - grp  */ 5,
    /* VPI/VCI range    */ ima_vp_vc_range,
    /* trafic config    */ &group_alloc[0],
    /* modifiers        */ MODIFIERS,
    /* gsm_timeout      */ 10,
    /* scci_audit_timeout */ 10
    },
};

WP_U32 ima_port_assignments[NUM_OF_GROUPS][NUM_OF_LINKS] = {
   /* Group 0 ports used */ {0,1,2,3},
   /* Group 1 ports used */ {4,5,6,7}
};

#define NUM_OF_FRAMES 1
#define NUM_OF_BUFS   10

/* keep track of how many frames are to be received */
WP_U32 pt_receive_size[1] = { NUM_OF_FRAMES };

extern void WPI_SimulateInterrupts(void);

/* Dummy logging function */
void WP_AppLog(WP_CHAR *s)
{
}

/* Application specific tags for the channels */
WP_tag tag_r1 = 100;
WP_tag tag_t1 = 200;

/* Utility function to handle return values from WDDI function calls */
static void terminate_on_error(WP_handle,
                               WP_CHAR *);

/* Utility function to print out events */
static void display_events(void);

/* Link State information used for detecting change */
WP_ima_link_state_info link_states[NUM_OF_GROUPS][NUM_OF_LINKS];

/* Group State information used for detecting change */
WP_ima_group_state_info group_state[NUM_OF_GROUPS];

typedef struct
{
   WP_U16 in;
   WP_U16 out;
   WP_U16 size;
   WP_ima_event *task;
}
app_task_list;

static void AppImaEvent(WP_tag tag,
                        WP_U32 data,
                        WP_U32 info);

static void LinkDeleteEvent(WP_tag tag,
                            WP_U32 group_id,
                            WP_U32 link_ids);

static void AppTxEvent(WP_tag tag,
                        WP_U32 data,
                        WP_U32 info);

static void AppRxEvent(WP_tag tag,
                        WP_U32 data,
                        WP_U32 info);

WP_ima_event *next_task(app_task_list *task_list,
                        WP_ima_event * result);

void add_task(app_task_list *task_list,
              WP_U32 data,
              WP_U32 info);

void app_perform_action(WP_ima_event * task);

/* Interrupt task list */

#define IRQ_TASK_LIST_SIZE 4096
WP_ima_event irq_task[IRQ_TASK_LIST_SIZE];
WP_ima_event del_task[IRQ_TASK_LIST_SIZE];
app_task_list irq_task_list[1] = { {0, 0, IRQ_TASK_LIST_SIZE, irq_task} };
app_task_list del_task_list[1] = { {0, 0, IRQ_TASK_LIST_SIZE, del_task} };

WP_ima_event a_task;
WP_ima_event *task;

WP_handle ima_sys_handle;
WP_handle ima_group_handles[NUM_OF_GROUPS];
WP_handle ima_link_handles[NUM_OF_GROUPS][NUM_OF_LINKS];
WP_handle ima_ports[NUM_OF_GROUPS][NUM_OF_LINKS];
WP_handle ima_tdm_devs[NUM_OF_GROUPS][NUM_OF_LINKS];
WP_handle aal5_tx_handle;
WP_handle aal5_rx_handle;
WP_handle qn_aal5;
WP_handle pool_144;
WP_U32 rx_event = 0;
WP_U32 tx_event = 0;
WP_U32 events = 0xFFFFFFFF;
WP_ima_link link_config;
WP_U32 expected_op[NUM_OF_GROUPS];
WP_U32 tx_links[NUM_OF_GROUPS];
WP_U16 bandwidth_flag;
WP_U16 prev_grp_state[NUM_OF_GROUPS];
WP_U16 prev_fe_grp_state[NUM_OF_GROUPS];
WP_U16 deleted_link;


WP_U32 ticks;

WP_CHAR app_name[] = "26008";

WP_S32 main(WP_S32 argc,WP_CHAR **argv)
{
   WP_handle pool_ring;
   WP_handle status;
   WP_U32 i, j;
   WP_port port;
   /* For use with a socket based simulator */

   status = WP_DriverInit();
   terminate_on_error(status, "WP_DriverInit()");

   context->int_queue_data = &int_queue_tables;
   context->atm_data = atm_params;
   context->iw_data = 0;
   context->pool_limit = pool_limit;

   /* Board configuration must be done before SysInit() */
   /* status = WPX_BoardConfigure(0, WPX_CONFIGURE_DEFAULT_CHECKIN); */
   status = WPX_BoardConfigure(0, WPX_CONFIGURE_CHECKIN_TDI_16);
   terminate_on_error(status, "Board configuration failed");

   status = WP_SysInit(WP_WINPATH(0), context);
   terminate_on_error(status, "WP_SysInit()");

   /* Create BRG1 & BRG3 */
   status = WP_SysClockCreate(WP_WINPATH(0), WP_BRG1, WP_BRG_SRC_BRGIN2, 2);
   status = WP_SysClockCreate(WP_WINPATH(0), WP_BRG3, WP_BRG_SRC_BRGIN2, 2);

   /* Create the required number of TDM ports.               */
   /* Number of Ports = Number of groups x Number of links   */
   for (i = 0; i < NUM_OF_GROUPS; i++)
   {
      for (j = 0; j < NUM_OF_LINKS; j++)
      {
         /* Get port selection for group "i" link "j" */
         port = WP_PORT_TDM1 + ima_port_assignments[i][j];
         /* Create port for group "i" link "j" */
         ima_ports[i][j] = WP_PortCreate(WP_WINPATH(0), port, tdm_setup);
         terminate_on_error(ima_ports[i][j], "WP_PortCreate()");
      }
   }

   /* Create the required number of TDM devices. Number */
   /* of Devices = Number of groups x Number of links.  */
   for (i = 0; i < NUM_OF_GROUPS; i++)
   {
      for (j = 0; j < NUM_OF_LINKS; j++)
      {
         /* Create tdm device for group "i" link "j" */
         ima_tdm_devs[i][j] =
            WP_DeviceCreate(ima_ports[i][j], WP_UNUSED, WP_DEVICE_IMA,
                            tdm_device);
         terminate_on_error(ima_tdm_devs[i][j],
                            "WP_DeviceCreate() TDM IMA Dev");
      }
   }

   /* Create the Buffer pool(s) */
   pool_144 = WP_PoolCreate(WP_WINPATH(0), WP_pool_buffer, buffer_data);
   terminate_on_error(pool_144, "WP_PoolCreate() pool_144");

   /* Create the Ring pool(s) */
   pool_ring = WP_PoolCreate(WP_WINPATH(0), WP_pool_ring, ring_data);
   terminate_on_error(pool_ring, "WP_PoolCreate() pool_ring");

   /* Attach the Buffer and Ring pools to the qnode */
   qn_host->pool_buffer = pool_144;
   qn_host->pool_ring = pool_ring;

   /* Create the Host termination queue node */
   qn_aal5 = WP_QNodeCreate(0,WP_QNODE_HOSTQ,qn_host);
   terminate_on_error(qn_aal5, "WP_QNodeCreate() QNODE_HOSTQ");

   /* Initialize the Atm scheduling unit of the Winpath */
   status = WP_SysSchedulerCreate(WP_WINPATH(0), calendar_wheel_cfg);
   terminate_on_error(status, "WP_SysSchedulerCreate()");

   /* Create IMA System */
   ima_sys_handle = WP_ImaSystemCreate(WP_WINPATH(0), &system_params);
   terminate_on_error(ima_sys_handle, "WP_ImaSystemCreate()");

   /* Register IMA callback function */
   /* Event callback initilization */

   status = WP_ControlRegister(WP_EVENT_IMA, &AppImaEvent);
   terminate_on_error(status, "WP_ControlRegister");

   status = WP_ControlRegister(WP_EVENT_IMA_LINK_DELETE, &LinkDeleteEvent);
   terminate_on_error(status, "WP_ControlRegister");

   /* Register Tx and Rx event call back */
   status = WP_ControlRegister(WP_EVENT_TX_INDICATE, &AppTxEvent);
   terminate_on_error(status, "WP_ControlRegister() TxEvent");

   status = WP_ControlRegister(WP_EVENT_RX_INDICATE, &AppRxEvent);
   terminate_on_error(status, "WP_ControlRegister() RxEvent");

   /* Commit system resources */
   status = WP_SysCommit();
   terminate_on_error(status, "WP_SysCommit()");

   /* Debugging enable */
   private_int_queue();
#if 1
   /* Initialize CPLD TDM 1-8 */

   printf("Initializing TDM 1-8...");
   status = WPU_TdmCometCardInit(APP_PORT_TDM_BASE, CPLD_MODE);
   terminate_on_error(status, "WPU_TdmCometCardInit()");
   status = WPU_TdmCometCardInit(APP_PORT_TDM_BASE, COMET_MODE);
   terminate_on_error(status, "WPU_TdmCometCardInit()");
   status = WPU_TdmCometCardInit(APP_PORT_TDM_BASE + 1, COMET_MODE);
   terminate_on_error(status, "WPU_TdmCometCardInit()");
   status = WPU_TdmCometCardInit(APP_PORT_TDM_BASE + 2, COMET_MODE);
   terminate_on_error(status, "WPU_TdmCometCardInit()");
   status = WPU_TdmCometCardInit(APP_PORT_TDM_BASE + 3, COMET_MODE);
   terminate_on_error(status, "WPU_TdmCometCardInit()");
   status = WPU_TdmCometCardInit(APP_PORT_TDM_BASE + 4, COMET_MODE);
   terminate_on_error(status, "WPU_TdmCometCardInit()");
   status = WPU_TdmCometCardInit(APP_PORT_TDM_BASE + 5, COMET_MODE);
   terminate_on_error(status, "WPU_TdmCometCardInit()");
   status = WPU_TdmCometCardInit(APP_PORT_TDM_BASE + 6, COMET_MODE);
   terminate_on_error(status, "WPU_TdmCometCardInit()");
   status = WPU_TdmCometCardInit(APP_PORT_TDM_BASE + 7, COMET_MODE);
   terminate_on_error(status, "WPU_TdmCometCardInit()");
   printf("Completed...\n");
#endif

   /* Enable the Atm scheduling unit */
   status = WP_SysAsuEnable(WP_WINPATH(0));
   terminate_on_error(status, "WP_SysAsuEnable()");

   /* Run the specified tests */
   WT_Identify();

   printf("\n Reseting group handles...\n");
   for (i = 0; i < NUM_OF_GROUPS; i++)
      ima_group_handles[i] = 0;

   printf("\n Static Link Config Parameters...\n");
   /* Static Link configuration parameters */
   link_config.direction = WP_IMA_LINK_TX_RX;
   link_config.link_id = WP_IMA_AUTO_LINK_ID;
   link_config.alarm.lif_persistence_in = WP_IMA_NO_CHANGE;
   link_config.alarm.lif_persistence_out = WP_IMA_NO_CHANGE;
   link_config.alarm.lods_persistence_in = WP_IMA_NO_CHANGE;
   link_config.alarm.lods_persistence_out = WP_IMA_NO_CHANGE;
   link_config.alarm.rfi_persistence_in = WP_IMA_NO_CHANGE;
   link_config.alarm.rfi_persistence_out = WP_IMA_NO_CHANGE;

   menu_interface();
#ifdef USE_SOCKET
   CleanUpSocket(5);
#endif
   WP_DriverRelease();
   return 0;
}

/*****************************************************
 * This function simulates an interrupt and executes
 * all tasks in the task list
 *
 * Inputs
 *   none
 * Output
 *   none
 *
 * Return value
 *   none
 *
 ******************************************************/
static void display_events(void)
{
#ifndef linux
   WPI_SimulateInterrupts();
#endif
   while ((task = next_task(irq_task_list, &a_task)))
      app_perform_action(task);

   while ((task = next_task(del_task_list, &a_task)))
      link_del_action(task);
}

/**************************************************
 * This function adds a task to a task queue.
 *
 * Inputs
 *   task_list:    pointer to the task list
 *   tag:          the task applies to the channel with this tag
 *   action        the action to be applied
 *
 * Output
 *   none
 *
 * Return value
 *   none
 *
 ******************************************************/
void add_task(app_task_list *task_list,
              WP_U32 data,
              WP_U32 info)
{
   WP_U32 in = task_list->in;
   WP_U32 out = task_list->out;
   WP_U32 size = task_list->size;
   WP_U32 inp = in + 1;

   if (inp == size)
      inp = 0;
   if (inp != out)
   {
      task_list->task[in].data = data;
      task_list->task[in].info = info;
      task_list->in = inp;
   }
}

/******************************************************
 * Callback for Link Delete events, adds a delete event
 * to the Task list
 *
 * Inputs
 *   tag:       the task applies to the channel with this tag
 *   groupid:   ima_group handle
 *   linkid :   ima_link handle
 * Output
 *   none
 *
 * Return value
 *   none
 *
 ******************************************************/
void LinkDeleteEvent(WP_tag tag, WP_U32 group_id, WP_U32 link_ids)
{
   add_task(del_task_list, group_id, link_ids);
}

void link_del_action(WP_ima_event *task)
{
   WP_status status;
   WP_U32 results[32];
   WP_U32 n_results,i;
   WP_U32 command;
   deleted_link++;
   command = WP_IMA_TRANSLATE_TO_PORT;

   status = WP_ImaGroupTranslateLinkSet(ima_sys_handle,
                                        task->data, task->info,
                                        command, &n_results, results);

   if (status)
            terminate_on_error(status, "WP_GroupTranslateLinkSet()");

   /* Prints port IDs */
   if (command == WP_IMA_TRANSLATE_TO_PORT) {
     printf("Deleted Links, Port IDs => ");
     for (i=0; i < n_results; i++)
          printf("%d ",results[i]);
   }

   /* Prints Link Handle(s) */
   else {
     printf("Deleted Links, Handles =>\n");
     for (i=0; i < n_results; i++)
          printf("%x\n",results[i]);
   }
   printf("\n");
}


/**************************************************
 * This function gets a task from a task queue.
 *
 * Inputs
 *   task_list:    pointer to the task list
 *   result:       pointer to where to copy task
 *
 * Output
 *   *result:      the task
 *
 * Return value
 *   NULL if no tasks were on the list
 *   result if a task was on the list
 *
 * Note
 * The task is copied from the task queue so that another
 * thread can immediately use the entry in the task queue.
 ******************************************************/
WP_ima_event *next_task(app_task_list *task_list,
                        WP_ima_event * result)
{
   WP_U32 in = task_list->in;
   WP_U32 out = task_list->out;

   if (out != in)
   {
      *result = task_list->task[out];
      out++;
      if (out == task_list->size)
         out = 0;
      task_list->out = out;
      return result;
   }
   return NULL;
}

/**************************************************
 * This function executes a task
 *
 * Inputs
 *   task:         pointer to the task to be executed.
 *
 * Output
 *   none
 *
 * Return value
 *   none
 *
 ******************************************************/
void app_perform_action(WP_ima_event * task)
{
   WP_status status;

   WP_U32 data;
   WP_U32 info;
   WP_U32 action;

   WP_U32 port;
   WP_U32 group;

   data = task->data;
   info = task->info;
   action = data & 0xff;

   group = (data >> 16) & 0xff;
   port = (data >> 8) & 0xff;

   switch (action)
   {
      case WP_IMA_EVENT_BANDWIDTH_CHANGE:
         printf("---Group %d Bandwidth Change:\n", group);
         printf("---Number of Active Rx Links => %d\n",
                (info >> 16 & 0xff));
         printf("---Number of Active Tx Links => %d\n",
                (info & 0xff));
        /*Verify bandwidth is at expected value*/
        if((tx_links[group] == (info &0xff)) )
             bandwidth_flag++;
         break;

      case WP_IMA_EVENT_TXQFULL:
         printf("---Group %d TX queue full port%d\n", (data >> 16 & 0xff),
             (data >> 8) & 0xff);
         break;

      case WP_IMA_EVENT_TXQEMPTY:
         printf("---Group %d TX queue empty port%d\n", (data >> 16 & 0xff),
             (data >> 8) & 0xff);
         break;

      case WP_IMA_EVENT_TTS:
         printf("--Group %d -----Port %d ------->TTS\n",group,port);
         break;

      case WP_IMA_EVENT_TOS:
         printf("--Group %d -----Port %d ------->TOS\n",group,port);
         break;

      case WP_IMA_EVENT_OPERATIONAL_CHANGE:
#if IMA_DEBUG
         printf("---Group %d State %d----------->OP Change\n",group,info);
#endif
         break;

      case WP_IMA_EVENT_FE_ICP_CHANGE:
#if IMA_DEBUG
         printf("--- SCCI %x ------------------------>FE ICP Change\n",
                info);
#endif
         break;

      case WP_IMA_EVENT_NE_ICP_CHANGE:
#if IMA_DEBUG
         printf("--- SCCI %x ------------------------>NE ICP Change\n",
                info);
#endif
         break;

      case WP_IMA_EVENT_PROPRIETARY:
         break;

      case WP_IMA_EVENT_GSU_LASR_TX:
#if IMA_DEBUG
         printf("--Group %d -------------------->Lasr TX: %x\n",group,info);
#endif
         break;
      case WP_IMA_EVENT_GSU_LASR_RX:
#if IMA_DEBUG
         printf("--Group %d -------------------->Lasr RX: %x\n",group,info);
#endif
         break;

      case WP_IMA_EVENT_TICK:

         status = WP_ImaEvent(ima_sys_handle, task);
         if (status)
            terminate_on_error(status, "WP_ImaEvent()");

         if (action == WP_IMA_EVENT_TICK)
            ticks++;
         break;

      default:
         printf("---Unknown IMA event 0x%x 0x%x\n", data, info);
         break;
   }
}

/* Callback functions */
static void AppImaEvent(WP_tag tag,
                        WP_U32 data,
                        WP_U32 info)
{
   add_task(irq_task_list, data, info);
}

static void AppTxEvent(WP_tag tag, WP_U32 data, WP_U32 info)
{
   ++tx_event;
}

static void AppRxEvent(WP_tag tag, WP_U32 data, WP_U32 info)
{
   ++rx_event;
}

static void terminate_on_error(WP_handle handle,
                               WP_CHAR *s)
{
   if (WP_ERROR_P(handle))
   {
      printf("Test Abort %s %s\n", s, WP_error_name[WP_ERROR(handle)]);
      printf("Test Failed\n");
      WP_DriverRelease();
#if USE_SOCKET
      CleanUpSocket(5);
#endif
      exit(1);
   }
   else
   {
      WT_LOG(WT_DEBUG_LEVEL_5, (wt_log,
                                "IMA: Handle/Status returned from %s is %#8.8x \n",
                                s, handle));
   }
}

/**************************************************
 * This function toggles a Maskable event
 *
 * Inputs
 *   event_index:    Event index number
 *
 * Output
 *   none
 *
 * Return value
 *   none
 *
 ******************************************************/

void CLI_ToggleMaskableEvents(WP_U32 event_index)
{
  WP_U32 i;
  WP_status status;

  events ^= (1 << event_index);
  if(events & (1 << event_index))
    printf("Event enabled\n");
  else
    printf("Event disabled\n");

 /*Update all groups*/
 for( i = 0; i < NUM_OF_GROUPS; i++)
  if(ima_group_handles[i])
  {
    status = WP_ImaGroupEventEnable(ima_group_handles[i],events);
    if(status != WP_OK)
      terminate_on_error(status, "WP_ImaGroupEventEnable()");
  }
}

void CLI_ToggleTTS()
{
  CLI_ToggleMaskableEvents(WP_IMA_EVENT_TTS);
}

void CLI_ToggleTOS()
{
  CLI_ToggleMaskableEvents(WP_IMA_EVENT_TOS);
}

void CLI_ToggleOperationChange()
{
  CLI_ToggleMaskableEvents(WP_IMA_EVENT_OPERATIONAL_CHANGE);
}

void CLI_ToggleTxQempty()
{
 CLI_ToggleMaskableEvents(WP_IMA_EVENT_TXQEMPTY);
}

void CLI_ToggleTxQfull()
{
 CLI_ToggleMaskableEvents(WP_IMA_EVENT_TXQFULL);
}

void CLI_ToggleRxGSU_LASR()
{
 CLI_ToggleMaskableEvents(WP_IMA_EVENT_GSU_LASR_RX);
}

void CLI_ToggleTxGSU_LASR()
{
 CLI_ToggleMaskableEvents(WP_IMA_EVENT_GSU_LASR_TX);
}

void CLI_ToggleFE_ICPchange()
{
 CLI_ToggleMaskableEvents(WP_IMA_EVENT_FE_ICP_CHANGE);
}

void CLI_ToggleNE_ICPchange()
{
 CLI_ToggleMaskableEvents(WP_IMA_EVENT_NE_ICP_CHANGE);
}

/**************************************************
 * This function makes a group operational
 *
 * Inputs
 * group:  group number
 *
 * Output
 *   none
 *
 * Return value
 *   none
 *
 ******************************************************/

void CLI_MakeGroupOperational(WP_U32 group)
{
   WP_status status;
   WP_U32 actual_op = GROUP_NOT_OPERATIONAL;
   /*If already operational, skip*/
  if (group_state[group].ne_state == WP_IMA_GSM_OPERATIONAL)
     actual_op  = GROUP_OPERATIONAL;
   /*Clear task list*/
   display_events();
   /*Clear tick count*/
   ticks = 0;
   while (expected_op[group] != actual_op)
   {
      /* Check for IMA related events */
      display_events();

      /* If we've waited more than 20 seconds, something is wrong */
      if(ticks > SECONDS(20))
      {
         WT_LOG(WT_DEBUG_LEVEL_5, (wt_log,"IMA 26008 Test: Group not operational.\n"));
         printf("Make Operational failed: Group Not Operational, check cables!\n");
         return;
      }
      WP_Delay(2000);

     /* Get state of groups */
      status = WP_ImaGroupState(ima_group_handles[group], &group_state[group]);
      if (status)
         terminate_on_error(status, "WP_ImaGroupState()");

      if (prev_grp_state[group] != group_state[group].ne_state)
      {
         prev_grp_state[group] = group_state[group].ne_state;
         if (group_state[group].ne_state == WP_IMA_GSM_OPERATIONAL)
            actual_op = GROUP_OPERATIONAL;
      }
   }
}

/**************************************************
 * This function creates a group
 *
 * Inputs
 * group:  group number
 *
 * Output
 *   none
 *
 * Return value
 *   none
 *
 ******************************************************/

void CLI_GroupCreate(WP_U32 config_no, WP_U32 group)
{
  WP_S32 i;
  WP_status status;

  printf("Creating group...");
  ima_group_handles[group] = WP_ImaGroupCreate(ima_sys_handle, &ima_group[config_no]);
  terminate_on_error(ima_group_handles[group], "WP_ImaGroupCreate()");
  status = WP_ImaGroupEventEnable(ima_group_handles[group],events);
  if (status)
    terminate_on_error(status, "WP_ImaGroupEventEnable()");
  for( i = 0; i < NUM_OF_LINKS; i++)
    ima_link_handles[group][i] = 0;
  printf("Completed\n");
  return;
}

/**************************************************
 * This function creates a group and starts it up
 *
 * Inputs
 * config number
 *
 * Output
 *   none
 *
 * Return value
 *   none
 *
 ******************************************************/
void CLI_IMA_AddGroup(WP_CHAR * StrPrm)
{
  WP_S32 grp_config = 0;
  WP_S32 res,group;
  WP_status status;
  res = sscanf(StrPrm, "%*d %d",&grp_config);
  if(res < 1)
  {
    printf("Usage: <grp_config #>\n");
    printf("%d Group configurations are predefined for use\n",NUM_OF_CONFIGS);
    return;
  }

  if(grp_config < NUM_OF_CONFIGS)
  {
    for(group = 0; group < NUM_OF_GROUPS; group++)
    {
      if(ima_group_handles[group] == NULL)
      {
        expected_op[group] = GROUP_NOT_OPERATIONAL;
        CLI_GroupCreate(grp_config,group);
        status = WP_ImaGroupStartup(ima_group_handles[group]);
        terminate_on_error(status, "WP_ImaGroupStartup()");

        tx_links[group] = 0;

        group_state[group].ne_state = WP_IMA_GSM_START_UP;
        group_state[group].fe_state = WP_IMA_GSM_START_UP;
        expected_op[group] = GROUP_OPERATIONAL;

        prev_grp_state[group] = WP_IMA_GSM_START_UP;
        prev_fe_grp_state[group] = WP_IMA_GSM_START_UP;

        return;
      }
    }
    printf("Group limit reached\n");
    return;
   }
   printf("Invalid Group Config\n");
}

/**************************************************
 * This function deletes a group
 *
 * Inputs
 * group:  group number
 *
 * Output
 *   none
 *
 * Return value
 *   none
 *
 ******************************************************/
void CLI_IMA_DeleteGroup(WP_CHAR * StrPrm)
{
  WP_U32 group;
  WP_S32 res,i;
  WP_status status;
  res = sscanf(StrPrm, "%*d %d",&group);
  if(res < 1)
  {
    printf("Usage: <group #>\n");
    return;
  }
   if(group < NUM_OF_GROUPS)
   {
     printf("Verifying Group %d is used...",group);
     if(ima_group_handles[group])
     {
       printf("Deleting...");
       /*Delete Group*/
       status = WP_ImaGroupDelete(ima_sys_handle, ima_group_handles[group]);
       terminate_on_error(status, "WP_ImaGroupDelete()");
       /*Erase Group and Link Handle record*/
       for(i = 0; i < NUM_OF_LINKS; i++)
         ima_link_handles[group][i] = 0;
       ima_group_handles[group] = 0;
       tx_links[group]=0;
       expected_op[group] = GROUP_NOT_OPERATIONAL;
       /*Execute all IMA events*/
       display_events();
       printf("Group Deleted\n");
       return;
     }
   }
   printf("Group does not exist, or is already deleted\n");
}

void CLI_IMA_AddLink(WP_CHAR * StrPrm)
{
  WP_U32 group;
  WP_U32 link[NUM_OF_LINKS];
  WP_S32 res, i;
  res = sscanf(StrPrm,"%*d %d %d %d %d %d",&group,&link[0],&link[1],&link[2],&link[3]);
  if (res < 2)
  {
    printf("Usage: <group #> <link #> (link #) (link #) (link #)\n ");
    return;
  }

  if (ima_group_handles[group] == 0)
  {
    printf("ERROR Group %d is not allocated\n",group);
    return;
  }

  for(i = 0; i < (res - 1); i++)
  {
    printf("Verifying Link %d is valid...",link[i]);
    if((link[i] < NUM_OF_LINKS))
    {
      if(ima_link_handles[group][link[i]]==0)
      {
         printf("Adding Link..\n");
         /*Clear bandwidth record*/
         bandwidth_flag = 0;
         /*Set expected num of links*/
         tx_links[group]++;
         /*Add Links*/
         ima_link_handles[group][link[i]] = WP_ImaLinkAdd(ima_group_handles[group],
                                                          ima_tdm_devs[group][link[i]],
                                                          ima_tdm_devs[group][link[i]],
                                                          &link_config);
         terminate_on_error(ima_link_handles[group][link[i]], "WP_ImaLinkAdd()");
         /*Check if group is operational*/
         CLI_MakeGroupOperational(group);
         /*Clear all IMA events*/
         display_events();
         /*Clear tick count*/
         ticks = 0;
         /*Wait till the bandwidth has increased to expected value*/
         while(bandwidth_flag == 0)
         {
           display_events();
           if(ticks > SECONDS(5))
           {
              printf("Failed to increase bandwidth, removing link\n");
              tx_links[group]--;
              CLI_IMA_DeleteOneLink(group,link[i]);
              printf("Failed bandwidth decrease normal\n");
              return;
           }
         }
         printf("Completed\n");
      } else
        {
          printf("Link is already allocated\n");
        }
    } else
        printf("Invalid link number\n");
  }
}


void CLI_IMA_DeleteOneLink(WP_U32 group, WP_U32 link)
{
    WP_status status;
    printf("Deleting Link..\n");
    /*Delete Link*/
    status =  WP_ImaLinkDelete(ima_group_handles[group],
                               ima_link_handles[group][link]);
    if (status)
      terminate_on_error(status, "WP_ImaGroupState()");
    /*Clear link handle record*/
    ima_link_handles[group][link] = 0;
    /*Set expected num of links*/
    tx_links[group]--;
    /*Clear bandwidth flag*/
    bandwidth_flag = 0;
    /*Clear deleted flag*/
    deleted_link = 0;
    /*Clear all events*/
    display_events();
    /*Clear tick count*/
    ticks = 0;
    /*Wait till the bandwidth has decreased to expected value*/
    /*Wait till the asynchronous delete event has occured*/
    while((bandwidth_flag == 0)||( deleted_link == 0))
    {
      display_events();
      if(ticks > SECONDS(10))
      {
        printf("Failed to decrease bandwidth\n");
        tx_links[group]++;
        return;
      }
    }
    printf("Completed\n");

}

void CLI_IMA_DeleteLinks(WP_CHAR * StrPrm)
{
  WP_U32 group;
  WP_U32 link[NUM_OF_LINKS];
  WP_S32 res, i;
  res = sscanf(StrPrm,"%*d %d %d %d %d %d",&group,&link[0],&link[1],&link[2],&link[3]);
  if (res < 2)
  {
    printf("Usage: <group #> <link #> (link #) (link #) (link #)\n ");
    return;
  }
  /*Check Group is valid*/
  if (group < NUM_OF_GROUPS)
    if (ima_group_handles[group])
    {
     for(i = 0; i <(res - 1); i++)
     {
       printf("Verifying Link %d is valid...",link[i]);
       /*Check Link is valid*/
       if((link[i] < NUM_OF_LINKS))
       {
         if(ima_link_handles[group][link[i]])
            CLI_IMA_DeleteOneLink(group,link[i]);
         else
           printf("ERROR: Link does not exist or already deleted\n");
       }
     }
     return;
    }
  printf("ERROR Group %d is not allocated\n",group);
}

void CLI_IMA_GroupState(WP_CHAR * StrPrm)
{
  WP_U32 group;
  WP_S32 res;
  WP_status status;

  WP_ima_group_state_info grp_state[NUM_OF_GROUPS];
  res = sscanf(StrPrm,"%*d %d",&group);
  if(res < 1)
  {
    printf("Usage: <group #>\n");
    return;
  }

  if(group < NUM_OF_GROUPS)
  {
    if(ima_group_handles[group] == 0)
      printf("Group %d not allocated", group);
    else
    {
     status = WP_ImaGroupState(ima_group_handles[group], &grp_state[group]);
     if (status)
       terminate_on_error(status, "WP_ImaGroupState()");

     CLI_IMA_PrintGroupState(&grp_state[group]);
    }
  }
}

void CLI_IMA_PrintGroupState(WP_ima_group_state_info * grp_state)
{

  printf("Group %d State Information\n",grp_state->g_id);
  printf("---------------------------------------------\n");

  printf("Near End IMA ID     : %d\n",grp_state->ne_ima_id);
  printf("Near End Symmetry   : ");
  CLI_IMA_DecodeSymmetry(grp_state->ne_symmetry);
  printf("Near End TRL PORT   : %d\n",grp_state->ne_trl_port);
  printf("Near End TRL ID     : %d\n",grp_state->ne_trl_id);
  printf("Near End Frame Size : " );
  CLI_IMA_DecodeFrameLength(grp_state->ne_frame_size);
  printf("Near End Clock Mode : ");
  CLI_IMA_DecodeClockMode(grp_state->ne_clock_mode);
  printf("\n");

  printf("Far End IMA ID      : %d\n",grp_state->fe_ima_id);
  printf("Far End Symmetry    : ");
  CLI_IMA_DecodeSymmetry(grp_state->fe_symmetry);
  printf("Far End TRL ID      : %d\n",grp_state->fe_trl_id);
  printf("Far End Frame Size  : ");
  CLI_IMA_DecodeFrameLength(grp_state->fe_m);
  printf("Far End IMA version : ");
  CLI_IMA_DecodeOAM(grp_state->fe_oam);
  printf("\n");

  printf("Receved Test Pattern    : %d\n",grp_state->rx_test_pat);
  printf("Transmitted Test Pattern: %d\n",grp_state->tx_test_pat);
  printf("Expected Test Pattern   : %d\n\n",grp_state->exp_test_pat);

  printf("Allowed delay : %d\n",grp_state->allowed_delay);
  printf("Delay recorded: %d\n",grp_state->delay);
  printf("Delta between short and long: %d\n",grp_state->link_delay_delta);
  printf("Port with shortest delay    : %d\n",grp_state->short_delay_port);
  printf("Port with longest delay     : %d\n\n",grp_state->long_delay_port);

  printf("Tx Ports    : 0x%08x\n",grp_state->tx_port_set);
  printf("Rx Ports    : 0x%08x\n",grp_state->rx_port_set);
  printf("Rx min links: %d\n",grp_state->rx_min_links);
  printf("Tx min links: %d\n\n",grp_state->tx_min_links);


  printf("Near End state:  ");
  switch(grp_state->ne_state){
  case WP_IMA_GSM_START_UP:
           printf("Start Up\n");
           break;
  case WP_IMA_GSM_START_UP_ACK:
           printf("Start Up ACK\n");
           break;
  case WP_IMA_GSM_CONFIG_ABORTED:
           printf("Config Aborted\n");
           break;
  case WP_IMA_GSM_INSUFFICIENT:
           printf("Insufficient Links\n");
           break;
  case WP_IMA_GSM_BLOCKED:
           printf("Blocked\n");
           break;
  case WP_IMA_GSM_OPERATIONAL:
           printf("Operational\n");
           break;
  case WP_IMA_GSM_NOT_CONFIGURED:
           printf("Not Configured\n");
           break;
  default:
           printf("Unrecognized\n");
           break;
  }
  printf("Far End state :  ");
  CLI_IMA_DecodeGroupControl(grp_state->fe_state);

  printf("Near End Alarm state:   ");
  CLI_IMA_DecodeAlarmState(grp_state->ne_alarm_state);
  printf("Near End Alarm type :   ");
  CLI_IMA_DecodeAlarmType(grp_state->ne_alarm_type);

  printf("Far End Alarm state :   ");
  CLI_IMA_DecodeAlarmState(grp_state->fe_alarm_state);
  printf("Far End Alarm type  :   ");
  CLI_IMA_DecodeAlarmType(grp_state->fe_alarm_type);
  printf("\n");

  printf("RX ports in which LSM state = active: 0x%08x\n",grp_state->rx_lsm_active);
  printf("TX ports in which LSM state = active: 0x%08x\n",grp_state->tx_lsm_active);

}


void CLI_PrintLatestGrpICP(WP_U32 group)
{
   WP_ima_icp_cell tx_icp, rx_icp;
   WP_status status;

   if(group < NUM_OF_GROUPS)
     if(ima_group_handles[group])
     {
       status = WP_ImaGroupIcp(ima_group_handles[group], &rx_icp, &tx_icp,
                               WP_IMA_LATEST_ICP);
       if(status)
          terminate_on_error(status, "WP_ImaGroupIcp()");

        printf("Group %d Latest Tx ICP Cell", group);
        print_icp(&tx_icp);
        printf("\n Decoding ICP Cell:");
        ICP_Decode(&tx_icp, ICP_DECODE_ALL, 0);

        printf("\n----------------------------------------------\n");
        printf("Group %d Latest Rx ICP Cell", group);
        print_icp(&rx_icp);
        ICP_Decode(&rx_icp, ICP_DECODE_ALL, 0);
        return;
     }

   printf("ERROR: Group %d is not allocated\n",group);


}
void CLI_DisplayLatestGrpICP(WP_CHAR * StrPrm)
{
   WP_S32 res;
   WP_U32 group;
   res = sscanf(StrPrm,"%*d %d",&group);
   if(res < 1)
   {
     printf("Usage: <group #>\n");
     return;
   }
   CLI_PrintLatestGrpICP(group);
}

void CLI_PrintLatestLinkICP_Rx(WP_U32 group, WP_U32 link)
{
   WP_status status;
   WP_ima_icp_cell rx_icp;

   if((group < NUM_OF_GROUPS) && (link < NUM_OF_LINKS))
    if(ima_link_handles[group][link])
    {
      status = WP_ImaLinkIcp(ima_link_handles[group][link], &rx_icp);
      if (status)
         terminate_on_error(status, "WP_ImaLinkIcp()");
      printf("Link Latest Rx ICP Cell:\n");
      print_icp(&rx_icp);
      printf("\n Decoding ICP Cell:");
      ICP_Decode(&rx_icp, ICP_DECODE_ALL, 0);
      return;
    }

  printf("ERROR: Group %d Link %d is not allocated\n",group,link);
}

void CLI_DisplayLatestLinkICP_Rx(WP_CHAR * StrPrm)
{
   WP_U32 group, link;
   WP_S32 res;
   res = sscanf(StrPrm,"%*d %d %d",&group, &link);
   if(res < 2)
   {
     printf("Usage: <group #> <link #>\n");
     return;
   }
   CLI_PrintLatestLinkICP_Rx(group,link);
}

void CLI_ActivateTDMChannel(WP_U32 group)
{
  WP_status status;
  aal5_tx_handle = WP_ChannelCreate(tag_t1,ima_group_handles[group],
                   qn_aal5, WP_CH_TX, WP_ATM_AAL5, &ch_config_aal5[0]);
  terminate_on_error(aal5_tx_handle, "WP_ChannelCreate() AAL5 Tx");

  aal5_rx_handle = WP_ChannelCreate(tag_t1,ima_group_handles[group],
                   qn_aal5, WP_CH_RX, WP_ATM_AAL5, &ch_config_aal5[0]);
  terminate_on_error(aal5_rx_handle, "WP_ChannelCreate() AAL5 Rx");

  WP_Delay(700000);

  status = WP_ChannelEnable(aal5_rx_handle);
  terminate_on_error(status, "WP_ChannelEnable() AAL5 Rx");

  status = WP_ChannelEnable(aal5_tx_handle);
  terminate_on_error(status, "WP_ChannelEnable() AAL5 Tx");
}

void CLI_RunTDMChannelTest(WP_CHAR * StrPrm)
{
  WP_S32 res;
  WP_U32 group;
  WP_status status;
  res = sscanf(StrPrm,"%*d %d",&group);
  if(res < 1)
  {
    printf("Usage: <group>\n");
    return;
  }

  if(group < NUM_OF_GROUPS)
  {
    if(ima_group_handles[group] == 0)
    {
       printf("ERROR Group %d is not allocated\n",group);
       return;
    }
    status = WP_ImaGroupState(ima_group_handles[group], &group_state[group]);
    if (status)
       terminate_on_error(status, "WP_ImaGroupState()");

    if (group_state[group].ne_state != WP_IMA_GSM_OPERATIONAL)
    {
      printf("ERROR: Group is not operational\n");
      return;
    }

    CLI_TestTDMChannel(group);
  } else
    printf("ERROR Group %d is not allocated\n",group);
}

void CLI_TestTDMChannel(WP_U32 group)
{
   WP_U32 i, j, x = 0;
   WP_data_segment km_bufs[NUM_OF_BUFS];
   WP_data_segment *km_curr_buff_ptr;
   WP_data_unit data_unit;
   WP_U32 rx_counter= 0;
   WP_status status;
   struct trial *t;
   t = trials;
   ch_config_aal5[0].tx_cwid = WP_CW_ID_A;
   printf("Configuring Test...");
   ima_group[0].version = t->ima_version;
   ima_group[0].frame_size = t->frame_size;
   ima_group[0].tx_clock_mode = t->tx_clock_mode;


  CLI_ActivateTDMChannel(group);
   printf("Initializing Buffers...");
   for(i=0;i<NUM_OF_BUFS;i++)
   {
      km_bufs[i].next = &km_bufs[i+1];
      km_bufs[i].displacement = 0;
   }
   km_bufs[NUM_OF_BUFS -1].next = NULL;

   data_unit.data_size = 0;
   data_unit.n_active = 0;

   for (i = 0; i < NUM_OF_BUFS; ++i)
   {
      km_bufs[i].data_size = i + 5;
      km_bufs[i].data = WP_PoolAlloc(pool_144);
      km_bufs[i].pool_handle = pool_144;

      WP_MEM_BYTES_FILL(km_bufs[i].data, t->tx_data +i, km_bufs[i].data_size);
      data_unit.data_size += km_bufs[i].data_size;
      ++data_unit.n_active;
   }
   printf("Init done\n");
   /* Perform Host Send  */
   printf("Sending Packet..");
   data_unit.segment = &km_bufs[0];
   data_unit.n_segments = NUM_OF_BUFS;
   data_unit.type = WP_DATA_AAL5;
   data_unit.control = WP_HT_CONTROL(WP_HT_INTERRUPT | WP_HT_UU_FIELD(0x33));
   status = WP_HostSend(aal5_tx_handle, &data_unit);
   terminate_on_error(status, "WP_HostSend()");

   WT_LOG(WT_DEBUG_LEVEL_5, (wt_log,"IMA 26008 Test: Transmitted %d frame(s) \n", NUM_OF_FRAMES));

   /* Perform Host Receive  */
   /* Set up the data unit */
   data_unit.segment = &km_bufs[0];
   data_unit.n_segments = NUM_OF_BUFS;
   data_unit.type = WP_DATA_AAL5;


   /* Wait for data to be received */
   /* Times after some time.       */

   printf("Receiving Packet..");
   for(i=0;i<500;i++)
   {
      /* Check for IMA related events */
      display_events();

      if (WP_OK != WP_HostReceive (aal5_rx_handle, &data_unit))
      {
         WT_LOG(WT_DEBUG_LEVEL_5, (wt_log,"IMA 26008 Test: WP_HostReceive(): No frame.\n"));
         WP_Delay(500000/1000);
      }

      else
      {
         km_curr_buff_ptr = data_unit.segment;
         WT_LOG(WT_DEBUG_LEVEL_5, (wt_log,"IMA 26008 Test: Received frame:\n"));
         for(j = 0; j < data_unit.n_active; j++, km_curr_buff_ptr++) {
            for(x = 0; x<km_curr_buff_ptr->data_size; x++)
            {
               WP_U8 tmp;
               if((x%20) == 0)
                 WT_LOG(WT_DEBUG_LEVEL_5, (wt_log,"\n"));

               WP_MEM_BYTE_GET(tmp, km_curr_buff_ptr->data[x]);
               WT_LOG(WT_DEBUG_LEVEL_5, (wt_log,"%2.2x", tmp));
            }
            WP_PoolFree(km_curr_buff_ptr->pool_handle, km_curr_buff_ptr->data);
            WT_LOG(WT_DEBUG_LEVEL_5, (wt_log,"(%d bytes) \n",km_curr_buff_ptr->data_size));
         }
         if (++rx_counter == pt_receive_size[0])
            break;
      }
   }

   /* End Host Send and Receive */
   /* free tx buffers */
   km_curr_buff_ptr = data_unit.segment;
   status = WP_HostRecover(aal5_tx_handle, &data_unit);
   terminate_on_error(status, "WP_HostRecover()");
   for (i = 0; i < data_unit.n_active; i++) {
      WP_PoolFree(km_curr_buff_ptr->pool_handle, km_curr_buff_ptr->data);
      km_curr_buff_ptr = km_curr_buff_ptr->next;
   }

   ticks = 0;
   while(!rx_event || !tx_event)
   {
         display_events();
         if(ticks > SECONDS(5)) {
            WT_LOG(WT_DEBUG_LEVEL_5, (wt_log,"IMA 26008 Test: Event Error.\n"));
            printf("Test Abort: No Tx/Rx Channel Events Generated.\n");
            printf("Test Failed\n");
            return;
         }
   }

   status = WP_ChannelDisable(aal5_tx_handle);
   terminate_on_error(status, "WP_ChannelDisable() AAL5 Tx");
   status = WP_ChannelDisable(aal5_rx_handle);
   terminate_on_error(status, "WP_ChannelDisable() AAL5 Rx");

   /* Give CW time to scan for channel */
   WP_Delay(500000);

   status = WP_ChannelDelete(aal5_tx_handle);
   terminate_on_error(status, "WP_ChannelDelete() AAL5 Tx");
   status = WP_ChannelDelete(aal5_rx_handle);
   terminate_on_error(status, "WP_ChannelDelete() AAL5 Rx");

   if (rx_counter == pt_receive_size[0])
      printf("Test Passed\n");

   else
      printf("Test Failed\n");
}

void CLI_IMA_DebugLogAll()
{
  WP_status status;
  WP_U32 i,j,k;
  WP_ima_link_statistics link_statistics;
  WP_ima_icp_cell rx_icp, tx_icp;
  WP_ima_icp_cell link_rx_icp;
  WP_stats_tdi_atm tdi_atm_stats;
  printf("Debug Log Start\n");
  printf("Versions info: \n");
  WT_Identify();

  for( i=0; i < 10; i++)
  {
     printf("ICP Capture %d\n",i);
     for( j = 0; j < NUM_OF_GROUPS; j++)
     {
       if( ima_group_handles[j])
       {
          status = WP_ImaGroupIcp(ima_group_handles[j], &rx_icp, &tx_icp,
                                  WP_IMA_LATEST_ICP);
          if(status)
             terminate_on_error(status, "WP_ImaGroupIcp()");

          printf("Group %d Tx ICP Cell", j);
          print_icp(&tx_icp);
          printf("Group %d Rx ICP Cell", j);
          print_icp(&rx_icp);

          printf("\n");
          for( k = 0; k < NUM_OF_LINKS; k++)
           if(ima_link_handles[j][k])
           {
               status = WP_ImaLinkIcp(ima_link_handles[j][k], &link_rx_icp);
                if (status)
                   terminate_on_error(status, "WP_ImaLinkIcp()");
                printf("Group %d Link %d Rx ICP Cell:",j,k);
                print_icp(&link_rx_icp);
          }
      }
    }
    WP_Delay(2000000);
    printf("----------------------------------------------\n");
  }

  for( i= 0; (i < NUM_OF_GROUPS); i++)
  {
    if (ima_group_handles[i] == 0)
        continue;
    for (j = 0; j < NUM_OF_LINKS; j++)
    {
      if(ima_link_handles[i][j] == 0)
        continue;
      /* Get Link Statistics */
      status =  WP_ImaLinkStatistics(ima_link_handles[i][j], &link_statistics);
      if (status)
      terminate_on_error(status, "WP_ImaLinkStatistics()");

      printf("\nPort %d Statistics:\n", ima_port_assignments[i][j]);
      printf("=================== \n");
      print_link_statistics(&link_statistics);
    }
  }


  for (i = 0; i < NUM_OF_GROUPS; i++)
  {
      if (ima_group_handles[i] == 0)
         continue;
      for (j = 0; j < NUM_OF_LINKS; j++)
      {
         printf("\nTDI ATM Device Statistics Group %d, Link %d",i,j);
         memset(&tdi_atm_stats,0,sizeof(WP_stats_tdi_atm));
         WP_DeviceStatistics(ima_tdm_devs[i][j], &tdi_atm_stats);
         tdm_stats_print(&tdi_atm_stats);
      }
   }


}


void CLI_ProgramQuit()
{
  WP_status status;
  status = WP_DriverRelease();
  terminate_on_error(status, "WP_DriverRelease");
}

Y_MenuEntry V_Menu_ICP[] =
{
  {2,      TRUE, "ICP Menu",{V_MainMenu}},
  {K_Leaf, TRUE, "Display Latest Group ICP Cells",{CLI_DisplayLatestGrpICP}},
  {K_Leaf, TRUE, "Display Latest Link Rx ICP Cell",{CLI_DisplayLatestLinkICP_Rx}},
};


Y_MenuEntry V_Menu_IMA_Events[] =
{
  {8,      TRUE,"IMA Events Menu",{V_MainMenu}},
  {K_Leaf, TRUE,"Toggle: Indicate TTS events",{CLI_ToggleTTS}},
  {K_Leaf, TRUE_M,"Toggle: Indicate TOS events",{CLI_ToggleTOS}},
  {K_Leaf, TRUE,"Toggle: Indicate Tx queue empty",{CLI_ToggleTxQempty}},
  {K_Leaf, TRUE_M,"Toggle: Indicate Tx queue full", {CLI_ToggleTxQfull}},
  {K_Leaf, TRUE,"Toggle: Indicate Rx GSU/LASR",{CLI_ToggleRxGSU_LASR}},
  {K_Leaf, TRUE_M,"Toggle: Indicate Tx GSU/LASR",{CLI_ToggleTxGSU_LASR}},
  {K_Leaf, TRUE,"Toggle: Indicate Far End ICP Change",{CLI_ToggleFE_ICPchange}},
  {K_Leaf, TRUE,"Toggle: Indicate Near End ICP Change",{CLI_ToggleNE_ICPchange}},
};

Y_MenuEntry V_MainMenu[] =
{
 {11,     TRUE  ,"Main Menu", {V_MainMenu}},
 {K_Leaf, TRUE  ,"Create IMA Group", {CLI_IMA_AddGroup}},
 {K_Leaf, TRUE_M,"Delete IMA Group #",{CLI_IMA_DeleteGroup}},
 {K_Leaf, TRUE  ,"Add Link to Group   0/1/2/3" , {CLI_IMA_AddLink}},
 {K_Leaf, TRUE_M,"Delete Link from Group 0/1/2/3", {CLI_IMA_DeleteLinks}},
 {K_Leaf, TRUE  ,"Test TDM channel on Group",{CLI_RunTDMChannelTest}},
 {K_Leaf, TRUE  ,"IMA Group State", {CLI_IMA_GroupState}},
 {K_Menu, TRUE  ,"ICP Display Menu",{V_Menu_ICP}},
 {K_Menu, TRUE_M,"Events Menu",{V_Menu_IMA_Events}},
 {K_Menu, TRUE  ,"Debug Menu",{V_Menu_Debug}},
 {K_Leaf, TRUE  ,"Debug Information Log",{CLI_IMA_DebugLogAll}},
 {K_Leaf, TRUE  ,"Exit Program",{CLI_ProgramQuit}}
};

void menu_interface()
{
   static char InputBuf[512];
   F_SetMainMenu();
   F_DisplayMenu();
   while (TRUE)
   {
      printf("%s", STR_ENTER_COMMAND);
      memset(InputBuf,0,512);
#ifdef linux
      while(strlen(InputBuf) < 1)
      {
         gets(InputBuf);
      }
#else
      gets(InputBuf);
#endif
      printf("\n");
      F_MenuEngine(InputBuf);
   }

}

/*****************************************************************************
 * NOTE:
 * This is where the shared routines gets included.  Ideally, this would
 * be done as a separately linked module.
 ****************************************************************************/
