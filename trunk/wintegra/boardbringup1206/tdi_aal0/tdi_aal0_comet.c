/*****************************************************************************
 * (C) Copyright 2009, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra, Inc.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 *****************************************************************************/
/*****************************************************************************
 *
 * Example : 99006-tdi_aal0_comet.c
 *
 * Description:
 *
 *     This application creates two AAL0 channels over a TDM port (TDM port 8)
 *     The AAL0 channels are host terminated. The test expects data to be looped
 *     back via a loopback connector or internally.
 *     One channel serves as the Rx channel and the other as the Tx channel.
 *     The application prints out the received data.  The test may be
 *     converted to internal loopback by defining LOOPBACK.  A TDM
 *     daughter card is still required in this mode to supply clocks.
 *
 * Input : None
 *
 * Output: The application prints out 2 received cells (52 bytes each) in
 *         hexadecimal format. Each cell has 4 bytes of atm header,
 *         and user data. The first cell contains 10 bytes of user data
 *         set to 0's (30 in hexadecimal) and the 2nd cell contains 10 bytes
 *         of user data set to 1's. (31 in hexadecimal). The remainder
 *         of each cell is uninitialized data (filled with eeee's).
 *
 * Setup : Populate a TDM daughter card that supports TDM8.  Install a loopback
 *         on TDM8 or connect external equipment that will loopback the data.
 *
 *WDDI Version: This application was tested using WDDI 4.0.
 *****************************************************************************/


#define LOOPBACK 0    /*1: internal loopback, 0: external loopback*/
#define PHY_TYPE_E1 0 /*1: E1 mode, 0: T1 mode*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "wp_wddi.h"
#include "wp_host_io.h"
#include "wp_sim.h"
#include "wpx_app_data.h"
#include "wpx_tdm_qfalc.h"
#include "wpx_tdm_comet.h"
#include "wpx_pin_mux_selector.h"
#include "../board/wpx_board_if.h"
/*****************************************************************************
 * (C) Copyright Wintegra 2009 All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra, Inc.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 *****************************************************************************/
#ifndef STATS_H
#define STATS_H

//#include "stats.c"
void printChannelStat(WP_handle);
void printAtmDeviceStats(WP_handle);
void printTdmAtmDeviceStats(WP_handle);
void printPOSDeviceStats(WP_handle);
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

#ifdef WP_BOARD_WDS3_SL
#pragma message("Compiling for WP3 SuperLite")
#endif

void WPE_CreateSchedulingUnit(void);
void WPE_SendReceiveCells(WP_handle , WP_handle , WP_handle);

extern Y_MenuEntry V_MainMenu[];
extern Y_MenuEntry V_Menu_Debug[];
char *STR_MENU_HDR = {"  TDI Test"};

#if PHY_TYPE_E1
#define COMET_MODE WPU_MODE_TDM_E1_MASTER
#define CPLD_MODE  WPU_INIT_COMET_CPLD_E1
#define NUM_BINDINGS 2
#define TDM_DELAY 3
#else
#define COMET_MODE WPU_MODE_TDM_T1_MASTER
#define CPLD_MODE  WPU_INIT_COMET_CPLD_T1
#define NUM_BINDINGS 1
#define TDM_DELAY 1
#endif

#define APP_PORT_TDM_BASE WP_PORT_TDM1

void printChannelStat(WP_handle);
void printAtmDeviceStats(WP_handle);
void printTdmAtmDeviceStats(WP_handle);
void printPOSDeviceStats(WP_handle);
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
 *      typedef struct {
 *          WP_U32 max_qnodes;
 *          WP_bus_bank virtual_bus[WP_VIRTUAL_BUSES];
 *          WP_U32 * dps_image[2];
 *          WP_U16 * dps_features[2];
 *          WP_int_queue_table * int_queue_data;
 *          WP_pool_global pool_limit;
 *          WP_atm_global * atm_data;
 *          WP_iw_global * iw_data;
 *          } WP_context;
 *
 *  Field Name      Description of field
 *  ----------      --------------------
 *  max_qnodes      Maximum permitted number of queue node objects in the system
 *  virtual_bus     An array of structures providing a mapping of each virtual
 *                  WinPath bus to a physical memory bus and bank.
 *  dps_image       Pointer to start of DPS code image for each pair of WinGines
 *  int_queue_data  Pointer to the interrupt queue table array
 *  pool_limit      Specifies the maximum number of buffer pools in the system
 *  atm_data        Pointer to the global atm data structure
 *  iw_data         Pointer to the global interworking data structure
 *
 *****************************************************************************/
WP_context context[1] =
{
   {6, /* qnodes */
    /* the first four assignments are mandatory bus assignments */
    {
       {/* WP_BUS_PARAM*/                    WP_BUS_PARAM, APP_BANK_PARAM},
       {/* WP_BUS_PACKET*/                   WP_BUS_PACKET, APP_BANK_PACKET},
       {/* WP_BUS_INTERNAL*/                 WP_BUS_INTERNAL, APP_BANK_INTERNAL},
       {/* WP_BUS_HOST */                    WP_BUS_HOST, APP_BANK_HOST},
       {/* WP_BUS_IRQ*/                      WP_BUS_HOST, APP_BANK_HOST},
       {/* WP_BUS_TMSTMP*/                   WP_BUS_HOST, APP_BANK_HOST},
       {/* WP_VB_CAS*/                       WP_BUS_NONE, 0},
       {/* WP_VB_TRANS_TSAT*/                WP_BUS_NONE, 0},
       {/* WP_VB_TPT*/                       WP_BUS_INTERNAL, APP_BANK_INTERNAL},
       {/* WP_VB_CW*/                        WP_BUS_INTERNAL, APP_BANK_INTERNAL},
       {/* WP_VB_ASU_TXQUEUE*/               WP_BUS_PARAM, APP_BANK_PARAM},
       {/* WP_VB_STATISTICS*/                WP_BUS_HOST, APP_BANK_HOST},
       {/* WP_VB_ATM_ADDLOOKUP*/             WP_BUS_PARAM, APP_BANK_PARAM},
       {/* WP_VB_ATM_CPT_EBMT_EXTHEADER*/    WP_BUS_PARAM, APP_BANK_PARAM},
       {/* WP_VB_ETH_HDLC_CPT*/              WP_BUS_PARAM, APP_BANK_PARAM},
       {/* WP_VB_FBP_BD*/                    WP_BUS_PARAM, APP_BANK_PARAM},
#ifdef WP_BOARD_WDS3_SL
       {/* WP_VB_FBP_DATA*/                  WP_BUS_PARAM, APP_BANK_PARAM},
       {/*WP_VB_L2_BKGND*/                   WP_BUS_PARAM, APP_BANK_PARAM},
#else
       {/* WP_VB_FBP_DATA*/                  WP_BUS_PACKET, APP_BANK_PACKET},
       {/*WP_VB_L2_BKGND*/                   WP_BUS_PACKET, APP_BANK_PACKET},
#endif
       {/*WP_VB_BRIDGE_LEARNING_DFC_PFMT*/   WP_BUS_PARAM, APP_BANK_PARAM},
       {/* WP_VB_IMA_MLPPP_STATE*/           WP_BUS_INTERNAL, APP_BANK_INTERNAL},
       {/* WP_VB_IMA_MLPPP_BUFFER*/          WP_BUS_HOST, APP_BANK_HOST},
       {/* WP_VB_HASH_IW_LPM_EMC*/           WP_BUS_HOST, APP_BANK_HOST},
       {/*WP_VB_IW_MFC_RULES*/               WP_BUS_HOST, APP_BANK_HOST},
       {/* WP_VB_IW_FIWT_QAT*/               WP_BUS_PARAM, APP_BANK_PARAM},
       {/* WP_VB_IW_MFCPT_DIFFSERV_UPPT_DFCT*/ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
       {/* WP_VB_STATISTICS2*/               WP_BUS_PARAM, APP_BANK_PARAM},
       {/* WP_VB_STATISTICS3*/               WP_BUS_PARAM, APP_BANK_PARAM},
       {/*WP_VB_UNASSIGNED6*/                WP_BUS_NONE, 0},
       {/* WP_VB_PKT_GLOBAL_TCPT*/           WP_BUS_NONE, 0},
       {/* WP_VB_AAL2_QSR*/                  WP_BUS_NONE, 0},
       {/*WP_VB_UNASSIGNED9*/                WP_BUS_NONE, 0},
       {/*WP_VB_UNASSIGNED10*/               WP_BUS_NONE, 0},
       {/*WP_VB_MCH_DATA*/                   WP_BUS_NONE, 0},
       {/*WP_VB_OAM_FM_QUERY*/               WP_BUS_NONE,0},
       {/*WP_VB_HASH_ON_HEAP*/               WP_BUS_NONE,0},
       {/*WP_VB_MFC_INTERNAL*/               WP_BUS_PARAM, APP_BANK_PARAM},
       {/*WP_VB_MFR_RAS_TBL*/                WP_BUS_NONE,0},
       {/*WP_VB_CESOP_DATA*/                 WP_BUS_NONE,0},
       {/*WP_VB_SECURITY_ESE*/               WP_BUS_NONE,0},
       {/*WP_VB_PDCP_SECURITY_ENTITY*/       WP_BUS_NONE,0},
       {/*WP_VB_POLICER*/                    WP_BUS_NONE,0},
       {/*WP_VB_PCE_LRN*/                    WP_BUS_NONE,0},
       {/*WP_VB_PCEA_PARAM*/                 WP_BUS_NONE,0},
       {/*WP_VB_CONGESTION_TABLE*/           WP_BUS_NONE,0}
    },
    {
       dps_ProgramImage, dps_ProgramImage
    },
    {
       dps_PC_Table_Init, dps_PC_Table_Init
    },

    /* int_queue_table */  NULL,
    /* pool limit      */  NULL,
    /* atm_data        */  NULL,
    /* iw_data         */  NULL,
   }
};


/*****************************************************************************
 *  WP_int_queue_table  Interrupt queue configuration used with WP_SysInit
 *
 *  The WDDI supports 4 interrupt event queues. The following structure has
 *  four pairs of elements: size and ratio.
 *  This structure is used as part of the WinPath context structure.
 *
 *  Size --
 *      Number of entries in interrupt queue table. If the table becomes full before
 *      events have been serviced, subsequent events are not recorded in the table.
 *      Therefore, the size of table should be greater than:
 *      (avg. rate of events / avg. frequency of interrupt servicing)
 *
 *  Ratio --
 *      number of event occurrences to interrupts generated
 *      For each n event occurrences associated with this interrupt queue, an
 *      interrupt is raised.
 *
 *
 *****************************************************************************/
WP_int_queue_table int_queue_tables = {{{24, 1}, {10, 1}, {10, 1}, {10, 1}}};


/*****************************************************************************
 *  WP_pool_buffer_data Buffer Pool parameters
 *
 *    This structure defines a pool used for data buffers including the number, size,
 *    and location of where the buffers will be created
 *    In this example the definition is for 100 buffers, each 144 bytes in length, residing
 *    on the host bus.
 *
 *****************************************************************************/
WP_pool_buffer_data buffer_data[] = {
   {/* n_buffers */ 100,
    /* offset */    0,
    /* size */     144,
    /* pad */      0,
    /* bus */     WP_BUS_HOST,
    /* bank */    APP_BANK_HOST}
};

/*****************************************************************************
 *  WP_pool_ring_data   Buffer Descriptor ring parameters
 *
 *    This structure defines the buffer descriptor rings controlling the TX and RX of data
 *    for a buffer pool, including the number and length of buffer descriptor rings and the
 *    memory bus where the BD's will be located.
 *    In this example the definition is for 4 rings each with 8 buffer descriptors, residing
 *    residing on the host bus.
 *
 *****************************************************************************/
WP_pool_ring_data ring_data[] = {
   {/* n_rings */        4,
    /* ring_length */      8,
    /* bus */           WP_BUS_HOST,
    /* bank */          APP_BANK_HOST}
};

/*****************************************************************************
 *  WP_qnode_hostq      AAL0 Host Queue Node parameters
 *
 *    This structure defines QNODE parameters supporting AAL0 host channels
 *    including the buffer pool and pool ring the Queue Node should use and the interrupt
 *    priority associated with the QNODE.
 *
 *****************************************************************************/
WP_qnode_hostq qn_host[1] =
{
   {
      /* pool_buffer */   0,
      /* pool_ring */     0,
      /* interruptqueue */ WP_IRQT0,
   }
};

/*****************************************************************************
 *  WP_pool_global      Global Pool parameters
 *
 *    This structure specfies the maximum number of buffer pools allowed in the system
 *    This structure is used as part of the WinPath context structure.
 *
 *
 *****************************************************************************/
WP_atm_global atm_params[1] =
{
   {
      /* max_pm_tables */      8,
      /* max_ubrplus_channels */ 8,
      /* max_upc_tables */     0,
      /* max_rx_cids */       0,
      /* max_tx_cids */       0,
      /* max_cid_range */     0,
      /* AAL2 QSR bus */     WP_BUS_NONE,
      /* AAL2 QSR bank */    0
   }
};

/*****************************************************************************
 *  WP_atm_global       Global ATM parameters
 *
 *    This structure defines the global ATM parameters for a particular system and
 *    will determine specific allocation of resources
 *    This structure is used as part of the WinPath context structure.
 *
 *****************************************************************************/
WP_pool_global pool_limit = 8;

/*****************************************************************************
 *  WP_atm_shaping_pcr  Peak Cell Rate (PCR) shaping paramters
 *
 *    This structure defines the Peak Cell Rate which will be associated with a
 *    channels shaping parameters. This will be used by the ATM Scheduler to calculate
 *    tranmission rate of cells associated with a specific shaping parameters
 *
 *****************************************************************************/
WP_atm_shaping_pcr shaping_param =
{
   /* pcr */     58962, /* 25Mbps in cells/second 25*1000000/(8*53) */
   /* cdvt */     0,
   /* polling */   WP_SHAPING_POLL_AUTOD,
};

/*****************************************************************************
 *  WP_ch_aal0      AAL0 Channel specific parameter
 *
 *    This structure defines AAL0 Channel parameters. The WDDI manual details each entry
 *    in the structure. In summary the AAL0 channel defined here has VPI=1,VCI=31,
 *    is shaped using Peak Cell Rate, Real Time Traffic Class and interrupts are enabled,
 *    Interworking mode is disabled.
 *
 *****************************************************************************/
WP_ch_aal0 ch_config_aal0[1] =
{
   {
      /* vpi */                  10,
      /* vci */                  34,
      /* intmode */               WP_ATMCH_INT_DISABLE,
      /* iwmmode */              WP_ATMCH_IWM_DISABLE,
      /* statmode */              WP_ATMCH_STAT_ENABLE,
      /* testmode */              WP_ATMCH_TEST_DISABLE,
      /* tx_pqblock */             0,
      /* tx_pqlevel */             0,
      /* tx_shaping_type */         WP_ATM_SHAPING_PCR,
      /* tx_shaping_params */       &shaping_param,
      /* tx_shaping_group */        0,
      /* tx_ch_insert */           WP_AAL0_CH_INSERT_ENABLE,
      /* tx_extheader[12] */        {0},
      /* rx_oamee */              WP_AAL0_CH_EEMODE_ENABLE,
      /* rx_oamseg */             WP_AAL0_CH_SEGMODE_ENABLE,
      /* WP_U8 crcmode        */      0,
      /* WP_handle tx_mqblock */      0,
      /* WP_U8 tx_mqlevel     */      0,
      /* WP_U8 tx_mqweight    */      0,
      /* WP_U8 rx_iwheaderins */      0,
      /* WP_U16 rx_iwnumberofcells */ 0,
      /* WP_U8 tx_cwid        */     WP_CW_ID_A,
      /* WP_U8 tx_tq          */    0,
      /* epd_mode             */    0,
      /* epd_threshold        */    0,
      /* clp_fwd              */    0
   }
};


#if PHY_TYPE_E1
/* TDM Port configuration */
WP_tdm_slotgroup tdm_slots[] =
{
   {/* repeat */ 1, /* size */ 1},
   {/* repeat */ 1, /* size */ 15},
   {/* repeat */ 1, /* size */ 1},
   {/* repeat */ 1, /* size */ 15},
   {/* repeat */ 0, /* size */ 0}
};
#else
WP_tdm_slotgroup tdm_slots[] = {
   { /* repeat */ 1, /* size */ 24},
   { /* repeat */ 0, /* size */ 0}
};
#endif


WP_port_tdm tdm_setup[1] =
{
   {
      /* atm_limits */      {16,16,0,0},
      /* pkt_limits */      {0},
      /* trans_limits*/     {{0},{0}},
      /* slot */          tdm_slots,
#if LOOPBACK
      /* tdmmode */       WP_TDM_LOOPBACK,
      /* framemode */     WP_TDM_FRAMELESS,
#else
      /* tdmmode */       WP_TDM_NORMAL,
      /* framemode */     WP_TDM_FRAMED,
#endif
      /* pinconfig */      WP_TDM_COMMON,
      /* edge */         WP_TDM_RISING,
      /* delay */         TDM_DELAY,
      /* intmode */       WP_TDM_INT_DISABLE,
      /* rx_iw_bkgnd */    WP_IW_BKGND_NOT_USED
   }
};

/*****************************************************************************
 *  WP_vp_vc_block      ATM VP/VC address lookup range parameters
 *
 *    This structure defines the ATM VP/VC address lookup values that are valid
 *    for the examples. The structure below permits VPI's of 1-4, VCI's of 31-39
 *    to be mapped to specific channels
 *
 *****************************************************************************/
WP_vp_vc_block tdm_vp_vc_range[2] =
{
   {8, 10, 34, 39},
   WP_VP_VC_BLOCK_TERM
};

#if PHY_TYPE_E1
/* TDM ATM device configuration */
WP_tdm_binding atm_tdi_binding[] =
{
   {1, 15}, {17,31}
};
#else
WP_tdm_binding atm_tdi_binding[] = 
{
   {0, 23}
};
#endif



/* TDM ATM Extended device configuration */
WP_device_tdm_atm_ext atm_tdi_ext[] =
{
   {/*alignment mode*/ 0,
   /*sync mode */    0,
   /*dclp*/        0}
};

WP_device_tdm_atm tdmatm_device[1] =
{
   {
      /* n_timeslot_binding */         NUM_BINDINGS,
      /* timeslot_binding */           atm_tdi_binding,
      /* scramblemode */             WP_TDM_SCRAMBLE_ON,
      /* cosetmode */               WP_TDM_COSET_ON ,
      /* idlemode */                WP_TDM_DISCARD,
      /* alpha */                   5,
      /* delta */                   3,
      /* tx_shaping_mode */           WP_ATM_DEV_SHAPING_VC,
      /* tx_hierarchical_shaping_groups */ 0,
      /* tx_hierarchical_shaping_table */  NULL,
      /* lookupmode */               WP_ATM_LOOKUP_TABLE,
      /* vp_vc_block */              tdm_vp_vc_range,
      /* extended stats */             WP_ATM_EXT_PHY_STATS_ENABLE,
      /* RAS timeout    */             10,
      /* TDM ATM Extended Parameters*/  atm_tdi_ext
   }
};

/* Number of cells this application will send and receive */
#define NUM_OF_CELLS 2

/* Array to keep track of how many cells are to be received */
WP_U32 pt_receive_size[1] = {
   NUM_OF_CELLS};


/* Application specific tags for the channels */
WP_tag tag_r1 = 0;
WP_tag tag_t1 = 1;

/* Utility function to handle return values from WDDI function calls */
void terminate_on_error(WP_handle, WP_CHAR *);

   WP_handle qn1;
   WP_handle tdmd1, tdmdev1;
   WP_handle status;
   WP_handle tx_handle;
   WP_handle rx_handle;
   WP_handle pool_144;
   WP_handle pool_ring;

WP_S32 main(WP_S32 argc, WP_CHAR **argv)
{

   printf ("TDI AAL0 App :  Running Protocol AAL0 test...\n");

#if 0
  status = WPX_BoardConfigure(0, WPX_CONFIGURE_DEFAULT_CHECKIN);
  terminate_on_error(status, "WPX_BoardConfigure()");
#endif

 /*******************************************************************************
    *  Driver Initialisation - WDDI call registers driver with lower level
    ********************************************************************************/
   status = WP_DriverInit();
   terminate_on_error(status, "WP_DriverInit()");

  status = WPX_BoardConfigure(0, WPX_CONFIGURE_CHECKIN_TDI_16);
  terminate_on_error(status, "WPX_BoardConfigure()");


 /*****************************************************************************
    * Initialize the final parts of the WinPath context structure before calling
    * the WP_SysInit function
    *****************************************************************************/
   context->int_queue_data = &int_queue_tables;
   context->atm_data = atm_params;
   context->iw_data = 0;
   context->pool_limit = pool_limit;

 /*****************************************************************************
    *  WinPath Initialization - WDDI call uses the context structure to initialize
    *                           WinPath DPS and general WDDI constraints
    *****************************************************************************/
   status = WP_SysInit(WP_WINPATH(0), context);
   terminate_on_error(status, "WP_SysInit()");

 /*****************************************************************************
   *  TDM Port Creation   - WDDI call to create a TDM port using the structure tdm_setup
   *                        defining defines the characterstics of the port.
   *****************************************************************************/
   tdmd1 = WP_PortCreate(WP_WINPATH(0), WP_PORT_TDM8, tdm_setup);
   terminate_on_error(tdmd1, "WP_PortCreate() (TDM8)");

 /*****************************************************************************
   *  TDM Device Creation  -     WDDI call to create a TDM device using the
   *                             structure  tdmatm_device which defines the particular
   *                             device characteristics.
   *****************************************************************************/
   tdmdev1 = WP_DeviceCreate(tdmd1, WP_UNUSED, WP_DEVICE_ATM, tdmatm_device);
   terminate_on_error(tdmdev1, "WP_DeviceCreate() (TDM_ATM)");

 /*****************************************************************************
    *  Buffer and Ring Pool Creation - WDDI call to create the buffer and ring pools for
    *                                  this application. The functions use the structures
    *                                  defined earlier in this application.  Then,
    *                                  attach the Buffer and Ring pools to the qnode
    *                                  and create the Host termination queue node
    *****************************************************************************/
   /* Create the Buffer pool(s) */
   pool_144 = WP_PoolCreate(WP_WINPATH(0), WP_pool_buffer, buffer_data);
   terminate_on_error(pool_144, "WP_PoolCreate() pool_144");

   /* Create the Ring pool(s) */
   pool_ring = WP_PoolCreate(WP_WINPATH(0), WP_pool_ring, ring_data);
   terminate_on_error(pool_ring, "WP_PoolCreate() pool_ring");

   /* Attach the Buffer and Ring pools to the qnode */
   qn_host->pool_buffer = pool_144;
   qn_host->pool_ring = pool_ring;

 /*****************************************************************************
   *  AAL0 Queue Node Creation -  WDDI call to create the host Queue Node
   *                              The function uses the qn_host_cfg structure defined
   *                              earlier in this application. This application
   *                              creates two qnodes simply to show that multiple
   *                              qnodes can be used.  The TX and RX channels could
   *                              share a single qnode.
   *
   *****************************************************************************/
   qn1 = WP_QNodeCreate(0,WP_QNODE_HOSTQ,qn_host);
   terminate_on_error(qn1, "WP_QNodeCreate (QNODE_HOSTQ)");



   WPE_CreateSchedulingUnit();


 /*****************************************************************************
    *  System Commit - WDDI called to commit the WinPath hardware resources
    *                  for this application
    *****************************************************************************/
   status = WP_SysCommit();
   terminate_on_error(status, "WP_SysCommit()");


   status = WP_SysAsuEnable(WP_WINPATH(0));
   terminate_on_error(status, "WP_SysAsuEnable()");

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

 /*****************************************************************************
    *  TDM Port Enable - WDDI call to enable the created TDM device. In this example
    *                    the port operates in full duplex mode. In the case of using
    *                    TDM ports bot the Port and Device need to be enabled.
    *****************************************************************************/
   status = WP_PortEnable(tdmd1, WP_DIRECTION_DUPLEX);
   terminate_on_error(status, "WP_PortEnable() (TDM8)");

   status = WP_DeviceEnable(tdmdev1, WP_DIRECTION_DUPLEX);
   terminate_on_error(status, "WP_DeviceEnable() (TDM_ATM)");



 /*****************************************************************************
   * AAL0 Channel Creation -      WDDI call to create the Tx and Rx channels
   *                              and enable them.  It uses the channel_state
   *                              structure defined earlier to define type, direction
   *                              and VPI/VCI.  The channel handle and operational
   *                              state are then stored in the channel_state structure.
   *****************************************************************************/
   rx_handle = WP_ChannelCreate(tag_r1, tdmdev1, qn1, WP_CH_RX, WP_ATM_AAL0, &ch_config_aal0[0]);
   terminate_on_error(rx_handle, "WP_ChannelCreate() Rx");

   tx_handle = WP_ChannelCreate(tag_t1, tdmdev1, qn1, WP_CH_TX, WP_ATM_AAL0, &ch_config_aal0[0]);
   terminate_on_error(tx_handle, "WP_ChannelCreate() Tx ");

 /*****************************************************************************
   * AAL0 Channel Enable -         Enable RX channel before TX channel so that if
   *                               transmission is quick, RX will not discard data.
   *****************************************************************************/
   status = WP_ChannelEnable(rx_handle);
   terminate_on_error(status, "WP_ChannelEnable() Rx ");

   status = WP_ChannelEnable(tx_handle);
   terminate_on_error(status, "WP_ChannelEnable() Tx ");

   WP_CHAR InputBuf[256];
   F_DisplayMenu();
   while (TRUE)
   {
      printf("%s", STR_ENTER_COMMAND);
      memset(InputBuf,0,256);
      gets(InputBuf);
      printf("\n");
      F_MenuEngine(InputBuf);
   }

   return 0;
}

void CLI_printStats(void)
{
    printChannelStat(rx_handle);
    printChannelStat(tx_handle);
    printTdmAtmDeviceStats(tdmdev1);
}

void CLI_SendReceiveCells(void)
{
     printf("Send/Receive ATM cells on TDI\n");
    WPE_SendReceiveCells(tx_handle, rx_handle, pool_144);
}

void CLI_ProgramQuit(void)
{
  status = WP_DriverRelease();
  return;
}

/**************************************************************************************
 *Terminate_On_Error - Error handling function. This function uses a lookup to output
 *the relevant error message onto the terminal.
 **************************************************************************************/
void terminate_on_error(WP_handle handle, WP_CHAR *s)
{
   if (WP_ERROR_P(handle)) {
      printf("TDI AAL0 App: Terminal error (handle %#8.8x) %s\n", handle, s);
      printf("TDI AAL0 App: Error type - %s\n", WP_error_name[WP_ERROR(handle)]);
      exit(1);
   } else {
      printf("TDI AAL0 App: Handle/Status returned from %s is %#8.8x \n", s, handle);
   }
}

void WPE_CreateSchedulingUnit(void)
{
    WP_status   status;
    WP_calendar_wheel calendar[2] =
    {
        {
            /* function */    WP_CW_ASU,
            /* cwid*/       WP_CW_ID_A,
            /* min_ch_rate */ 150,    /* kbps / 424 */
            /* max_ch_rate */ 117925, /* 50Mbps / 424 */
        },
        WP_CALENDAR_WHEEL_TERM
    };
    status  = WP_SysSchedulerCreate(WP_WINPATH(0), calendar);
    terminate_on_error(status, "WP_SysSchedulerCreate()");
}


void WPE_SendReceiveCells (WP_handle tx_handle, WP_handle rx_handle, WP_handle pool_144)
{

   WP_handle status;
   WP_data_unit data_unit;
   WP_data_segment bufs[NUM_OF_CELLS];
   WP_data_segment *curr_buff_ptr;
   WP_U32 i, j, k;
   WP_U32 rx_counter = 0;

   for(i=0;i<NUM_OF_CELLS;i++) {
      bufs[i].next = &bufs[i+1];
      bufs[i].displacement = 0;
   }

   bufs[NUM_OF_CELLS -1].next = NULL;

   data_unit.data_size = 0;
   data_unit.n_active = 0;

   for (i = 0; i < NUM_OF_CELLS; ++i) {

      /* Each cell is 52 bytes long - 48 bytes of data + 4 bytes of ATM header */
      bufs[i].data_size = 52;
      bufs[i].data = WP_PoolAlloc(pool_144);
      bufs[i].pool_handle = pool_144;

      /* Clear data buffers if running with Verilog */


      memset(bufs[i].data + 4, '0' +i, 10);
      data_unit.data_size += bufs[i].data_size;
      ++data_unit.n_active;
   }

   data_unit.segment = &bufs[0];
   data_unit.n_segments = NUM_OF_CELLS;
   data_unit.type = WP_DATA_AAL0;
   data_unit.control = 0;

   /*****************************************************************************
    * Send Transmit buffers - This section of code performs the function that enables
    *                         the transmission of a particular channel
    *
    *****************************************************************************/
   for(i=0; i<1; ++i) {
      if (WP_OK != WP_HostSend(tx_handle, &data_unit)) {
         printf("UPI AAL0 App: WP_HostSend() returned an error.\n");
         exit(1);
      }
   }
   printf("UPI AAL0 App: Transmitted %d cells \n", NUM_OF_CELLS);


   /* Set up the data unit to receive data in */
   data_unit.segment = &bufs[0];
   data_unit.n_segments = NUM_OF_CELLS;
   data_unit.type = WP_DATA_AAL0;

   /*****************************************************************************
   *  Host Reception of Cells - The following piece of code polls the UPI port for
   *  data cells. WP_HostReceive places the received cells
   *  on the buffers availabe in the data_unit structure.
   *
   *****************************************************************************/
   for(i = 0;i < 5; i++)
   {
      if (WP_OK != WP_HostReceive (rx_handle, &data_unit))
      {
         printf("UPI AAL0 App: WP_HostReceive(): No cell..\n");
         WP_Delay(500000);
      }
      else
      {
         curr_buff_ptr = data_unit.segment;
         printf("UPI AAL0 App: Received cells:\n");
         for(j = 0; j < data_unit.n_active; j++, curr_buff_ptr++)
         {
            for(k = 0; k<curr_buff_ptr->data_size; k++)
            {
               printf("%2.2x", curr_buff_ptr->data[k]);
            }
             /******************************************************************
             *
             * After WP_HostReceive returns the data unit n_active field has
             * the number of buffers returned.
             * ****************************************************************/
            WP_PoolFree(curr_buff_ptr->pool_handle, curr_buff_ptr->data);
            printf(" (%d bytes) \n", curr_buff_ptr->data_size);
         }
         rx_counter += data_unit.n_active;
         if (rx_counter == pt_receive_size[0])
            break;
      }
      printf("\n");
   }

   /*************************************************************
   *
   * Host recover checks the tx channel's ring for buffers that have
   * been transmitted.  These buffers are "attached" to the data unit
   * passed into WP_HostRecover.  The host then frees these buffers
   * back to the pool they came from.
   *
   * It is possible to receive no buffers or many buffers when
   * calling WP_HostRecover.
   ***********************************************************/
   curr_buff_ptr = data_unit.segment;
   status = WP_HostRecover(tx_handle, &data_unit);
   terminate_on_error(status, "WP_HostRecover()");
   for (i = 0; i < data_unit.n_active; i++) {
      WP_PoolFree(curr_buff_ptr->pool_handle, curr_buff_ptr->data);
      curr_buff_ptr = curr_buff_ptr->next;
   }

}

Y_MenuEntry V_MainMenu[]=
{
  {4     ,TRUE,"Main Menu",                              {V_MainMenu}},
  {K_Leaf,TRUE,"Send/Receive cells on TDM Port",         {CLI_SendReceiveCells}},
  {K_Leaf,TRUE,"Print ATM Channel and Device Statistics",{CLI_printStats}},
  {K_Menu,TRUE,"Utilities and Debug",                    {V_Menu_Debug}},
  {K_Leaf,TRUE,"Quit the program",                       {CLI_ProgramQuit}}
};


