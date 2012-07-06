/*****************************************************************************
 * (C) Copyright 2000-2002, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/****************************************************************************
 *
 * Example: 33000-mc_hdlc_1ch.c
 *
 * Demonstrates the use of Multi Channel HDLC. The example defines
 * one HDLC transmit and one receive channels over the single multi
 * channel device. The N_FRAMES are transmitted by the HDLC transmit channel
 * must be received by the HDLC receive channel. The data is checked and
 * the test result is printed at the end of the test. The statistics are
 * enabled in this test and also verified.
 * The multi channel device (MCH) is created over TDM transparent device.
 * TDM8 port is used in this test.
 * No interrupts are generated.
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
#include "wpx_app_data.h"
#ifdef WP_HW_WINPATH1
#include "wpx_tdm_qfalc.h"
#else
#include "wpx_tdm_comet.h"
#include "wpx_enet_phy.h"
#endif
//#include "wt_util.h"
#include "wpx_board_if.h"

extern WP_U32 dps_ProgramImage[];
extern WP_U16 dps_PC_Table_Init[];
WP_CHAR ch = 'c';

#define N_TX_HDLC_CH 10
#define WPE_DEBUG_LEVEL /*0*/2

#define WPE_PORT_TDM WP_PORT_TDM1
#define WPE_TDM_E1 0
#define WPE_TDM_T1 1
#define WPE_TDM_MODE WPE_TDM_E1
#define WPE_TDM_PROTOCOL \
    ((WPE_TDM_MODE==WPE_TDM_T1)?WP_TDM_TRANS_MCH_T1:WP_TDM_TRANS_MCH_E1)

#if WPE_TDM_MODE == WPE_TDM_E1
#define NUM_SLOTS 32
#define WPU_INIT_COMET_CPLD_GEN WPU_INIT_COMET_CPLD_E1
#define WPU_MODE_TDM_GEN_MASTER WPU_MODE_TDM_E1_MASTER
#define WP_TDM_DELAY_TYPE WP_TDM_DELAY_3
#else
#define NUM_SLOTS 24
#define WPU_INIT_COMET_CPLD_GEN WPU_INIT_COMET_CPLD_T1
#define WPU_MODE_TDM_GEN_MASTER WPU_MODE_TDM_T1_MASTER
#define WP_TDM_DELAY_TYPE WP_TDM_DELAY_1
#endif

#define HDLC_ADDRESS 0xff03

#define N_QNODES   4
#define N_POOLS    4
#define N_FRAMES   1
#define N_CHANNELS 2
#define N_POLL_CYCLES 100000    /* timeout */

#define WTI_MPLS_LABEL 0x100101f
#define MPLS_SIZE 4

/* Utility macros */
#define MAC_COPY(d,s)           { int _i; \
                                  for(_i=0;_i<6;_i++) \
                                     *((d)+_i)=*((s)+_i); }

WP_U8 wt_mac_enet[6] = { 0x45, 0x6e, 0x65, 0x74, 0x23, 0x32 };

#define APP_MAX_TDM_PORTS 8

#define APP_NUM_IW_BUFFERS 100

#define APP_MAX_BPORTS 200

#define APP_MAX_MAC_ADDRESS 10

/* Frames configuration */
struct WPE_frame_config
{
   WP_U32 length;
   WP_U8 data_char;
} frame_config[N_FRAMES] =
{
   {
   32, 0x12},
      /*{ 6,  0x13 },
         { 62, 0x14 },
         { 14, 0x15 },
         { 80, 0x16 } */
};
static WP_U32 test_failed;

/* TDI1 Port configuration (HDLC Mode) */
/* TDM frame configuration */
WP_tdm_slotgroup tdm_slots[] = {
#if WPE_TDM_MODE == WPE_TDM_E1
   { /* repeat */ 4, /* size */ 8},
#else
   { /* repeat */ 3, /* size */ 8},
#endif
   /* terminate list */
   { /* repeat */ 0, /* size */ 0}
};

#ifndef WP_HW_WINPATH1
/* TDM Port configuration */
WP_port_tdm tdm_config[1] = {
   {
    /* atm_params */ {0, 0, 0, 0, 0, 0},
    /* pkt_limits */ {1, 1, 10, 1},
    /* trans_limits */ {{1, 0, 0, 0}, {1, 0, 0, 0}},
    /* slot */ tdm_slots,
    /* tdmmode */ WP_TDM_LOOPBACK,
    /* framemode */ WP_TDM_FRAMED,
    /* pinconfig */ WP_TDM_SEPARATE,
    /* edge */ WP_TDM_RISING,
    /* delay */ WP_TDM_DELAY_TYPE,
    /* intmode */ WP_TDM_INT_DISABLE,
    /* rx_iw_bkgnd */ WP_IW_BKGND_NOT_USED
    }
};

#else
WP_port_tdm tdm_config[1] = {
   {
    /* atm_params */ {0, 0, 0, 0, 0, 0},
    /* pkt_limits */ {1, 1, 0, 0},
    /* trans_limits */ {{1, 0, 0, 0}, {1, 0, 0, 0}},
    /* slot */ tdm_slots,
    /* tdmmode */ WP_TDM_LOOPBACK,
    /* framemode */ WP_TDM_FRAMED,
    /* pinconfig */ WP_TDM_COMMON,
    /* edge */ WP_TDM_RISING,
    /* delay */ WP_TDM_DELAY_1,
    /* intmode */ WP_TDM_INT_DISABLE,
    /* rx_iw_bkgnd */ WP_IW_BKGND_NOT_USED
    }
};
#endif

/* TDM time slot assignment */
WP_tdm_binding trans_binding[] = {
#if WPE_TDM_MODE == WPE_TDM_E1
   {0, 31}
#else
   {0, 23}
#endif
};

/* MCH configuration for Transparent device */
WP_device_tdm_trans_mch trans_mch_config = {
   /* parambustype  */ WP_BUS_PARAM,
   /* parambusbank  */ APP_BANK_PARAM,
   /* opmode        */ WP_MCH_NX64,
   /* protocols     */ WP_MC_HDLC,
   /* tx_wait_counter */ 10,
   /* tx_numflags   */ 1,
   /* tx_maxsdu     */ 1536,
   /* rx_max_dlci   */ 0
};

/* Configuration information for the TDM Frame Relay device */
WP_device_tdm_trans trans_dev_config[1] = {
   {
    /* n_timeslot_binding */ 1,
    /* timeslot_binding   */ &trans_binding[0],
    /* framemode          */ WP_TDM_TRANS_SINGLE_FRAME,
    /* protocol           */ WPE_TDM_PROTOCOL,
    /* structure_size     */ WP_TDM_STRUCTURE_BASIC,
    /* datareverse        */ WP_TDM_TRANS_REVERSE,
    /* sigreverse         */ WP_TDM_TRANS_REVERSE,
    /* protocol_specific  */ &trans_mch_config
    }
};

/* MC HDLC virtual device configuration */
WP_U32 ch_slots[1] = { 31 };
WP_device_mc_hdlc mc_hdlc_dev_config[1] = {
   {
                                       /* num_slots              */ 32,
                                       //NUM_SLOTS,
                                             /* slots                  */ ch_slots,
                                             /* all slots, no need to specify */
    /* frame_number           */ 0,
    /* opmode                 */ WP_MCH_NX64,
    /* crctype                */ WP_HDLC_CRC16,
    /* tx_idlemode            */ WP_TDI_MCH_IDLEMODE_IDLE,
    /* tx_statmode            */ WP_PKT_STAT_ENABLE,
    /* rx_statmode            */ WP_PKT_STAT_ENABLE,
    /* tx_flowmode            */ WP_FLOWMODE_MULTI,
    /* addr_mask              */ 0x0fff,
    /* address1               */ HDLC_ADDRESS,
    /* address2               */ 0xffff,
    /* tx_tq_types            */ {WP_PKT_SCHED_SHAPING,
                                  WP_PKT_SCHED_SHAPING,
                                  WP_PKT_SCHED_SHAPING,
                                  WP_PKT_SCHED_SHAPING},
    }
};

/* Pool to manage the data buffers required by this application */
/* Note the for Frame Relay channels the buffers sizes must be a
   multiple of 256 bytes */
WP_pool_buffer_data buffer_data[] = {
   {
    /* n_buffers */ 100,
    /* offset */ 0,
    /* size */ 1536,
    /* pad */ 0,
    /* bus */ WP_BUS_HOST,
    /* bank */ APP_BANK_HOST
    }
};

/* Pool of rings */
WP_pool_ring_data ring_data[] = {
   {
    /* n_rings */ 2,
    /* ring_length */ 16,
    /* bus */ WP_BUS_PARAM,
    /* bank */ APP_BANK_PARAM
    }
};

/* Host termination qnode for the HDLC channels */
WP_qnode_hostq host_qnode_config[1] = {
   {
                                /* pool_buffer */ 0,
                                /* set later */
                                /* pool_ring */ 0,
                                /* set later */
    /* interruptqueue */ WP_IRQT1
    }
};

WP_pkt_shaping_ppr hdlc_ch_shaping[1] = {
   {
                                /* ppr */ 2048000,
                                /* bps */
    /* polling */ 2048000
    }
};

/* Configuration structure for the HDLC channels*/
WP_ch_hdlc hdlc_ch_config[1] = {
   {
    /* intmode */ WP_PKTCH_INT_DISABLE,
    /* iwmmode */ WP_PKTCH_IWM_DISABLE,
    /* testmode */ WP_PKTCH_TEST_DISABLE,
    /* tx_pqblock */ 0,
    /* tx_pqlevel */ 0,
    /* tx_shaping_type */ WP_PKT_SHAPING_PPR,
    /* tx_shaping_params */ &hdlc_ch_shaping[0],
    /* rx_maxsdu */ 1536,
    /* tx_cwid */ WP_CW_ID_A,
    /* tx_tq */ 0
    }
};

/* Packet Scheduler Unit (PSU) configuration parameters */
/* A different array should be constructed for each WinPath */
WP_calendar_wheel cw_config[2] = {
   {
    /* function */ WP_CW_PSU,
    /* cwid */ WP_CW_ID_A,
    /* min_ch_speed */ 100000,
    /* max_ch_speed */ 155000000
    },
   WP_CALENDAR_WHEEL_TERM
};

/* Interrupt queue configuration used with WP_SysInit */
WP_int_queue_table int_queue_tables = {
   {
    {24, 1},
    {24, 1},
    {24, 1},
    {24, 1}
    }
};

WP_iw_global iw_global_cfg = {
   /* max_iw_aggregations    */ 200,
   /* max_routing_systems    */ 0,
   /* max_directmap_ene...   */ 1,
   /* max_bridging_systems   */ 2,
   /* max_mpls_systems       */ 0,
   /* max_vlan_priority...   */ 0,
   /* iw_bkgnd_fifo_size     */ 1000,
   /* cong_pt                */ NULL,
   /* iw_host_limits         */ {10, 10, 0, 0},
   /* mpls_config            */ {0, 0},
   /* iw_modes               */ {
                                 WP_IW_POLICER_ENABLE,
                                 WP_IW_STATISTICS_BUNDLE_STAT_ENABLE,
                                 WP_IW_L2_HEADER_EXTENSION_ENABLE,
                                 WP_IW_ENHANCED_FLOW_STAT_ENABLE,
                                 WP_IW_FLOW_STAT_ENABLE,
                                 WP_IW_FR_TX_FLOW_STAT_DISABLE,
                                 0,
                                 WP_IW_LEARNING_DISABLE,
                                 /* port_filtering_mode; */
                                 WP_IW_PORT_FILTERING_ENABLE,
                                 /* lpm_alloc_mode; */ 0,
                                 /* enhanced_mc_member_stat_mode; */ 0,
                                 0,
                                 1,
                                 },
   /* max_iw_stat_bundles     */ 0,
   /* max_fr_sw_systems       */ 0,
   /* max_bridging ports      */ APP_MAX_BPORTS,
   /* max_iw_mc_groups        */ 0,
   /* max_nat_systems         */ 0,
   /* max_iw_second_agg       */ 0,
   /* max_iw_compr_limits     */ 0,
   /* WP_U32 max_iw_mc_members */ 0,
   /* WP_U16 max_iw_programmable_filters */ 4,
   /* WP_U16 max_iw_policers */ 1,
};

WP_atm_global atm_global_cfg = {
                                    /* max_pm_tables          */ 0,
                                    /* Performance monitoring */
                                    /* max_ubrplus_channels   */ 0,
                                    /* UBR+ channels configured */
                                    /* max_upc_tables         */ 0,
                                    /* Max policing tables */
                                    /* max_rx_cids            */ 0,
                                    /* AAL2 max Rx CIDs/channel */
                                    /* max_tx_cids            */ 0,
                                    /* AAL2 max Tx CIDs/channel */
   /* max_cid_range          */ WP_AAL2_CIDRANGE_UNUSED,
                                             /* qsr_bus                */ WP_BUS_NONE,
                                             /* AAL2 QSR bus */
                                    /* qsr_bank               */ 0,
                                    /* AAL2 QSR bus */
   /* atm_modes              *//* Atm modes */
   {
    WP_ATM_RAS_TIMESTAMP_ENABLE,
#if _WP_MODULE_ABR_ENABLE_
    0,
#endif
    WP_AAL2_RX_3FE_DISABLE},
   /* max_stats_extensions */ 0,
};

/* An array of one WP_context structure for each WinPath */
WP_context context[1] = {
   {N_QNODES,                   /* qnodes */
    /* the first four assignments are mandatory bus assignments */
    {
     { /* WP_BUS_PARAM    */ WP_BUS_PARAM, APP_BANK_PARAM},
     { /* WP_BUS_PACKET   */ WP_BUS_PACKET, APP_BANK_PACKET},
     { /* WP_BUS_INTERNAL */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
     { /* WP_BUS_HOST     */ WP_BUS_HOST, APP_BANK_HOST},
     { /* WP_BUS_IRQ      */ WP_BUS_PACKET, APP_BANK_PACKET},
     { /* WP_BUS_TMSTMP   */ WP_BUS_HOST, APP_BANK_HOST},
     { /* UNASSIGNED      */ WP_BUS_NONE, 0},
     { /* UNASSIGNED      */ WP_BUS_NONE, 0},
     { /* ASU_TPT         */ WP_BUS_PARAM, APP_BANK_PARAM},
     { /* ASU CW          */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
     { /* ASU TX QUEUE    */ WP_BUS_PARAM, APP_BANK_PARAM},
     { /* STATISTICS      */ WP_BUS_HOST, APP_BANK_HOST},
     { /* ADDRESS LOOKUP  */ WP_BUS_PARAM, APP_BANK_PARAM},
     { /* ATM CPT         */ WP_BUS_PARAM, APP_BANK_PARAM},
     { /* ENET HDLC CPT   */ WP_BUS_PARAM, APP_BANK_PARAM},
     { /* IW FBP PARAM    */ WP_BUS_PARAM, APP_BANK_PARAM},
     { /* IW FBP DATA     */ WP_BUS_PACKET, APP_BANK_PACKET},
     { /* EXTERNAL FIFO   */ WP_BUS_PACKET, APP_BANK_PACKET},
     { /* MAC LOOKUP      */ WP_BUS_PARAM, APP_BANK_PARAM},
     { /* IMA SMALL       */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
     { /* IMA LARGE       */ WP_BUS_HOST, APP_BANK_HOST},
     { /* LPM             */ WP_BUS_HOST, APP_BANK_HOST},
     { /* MFC RULES       */ WP_BUS_HOST, APP_BANK_HOST},
     { /* FIWT, QAT       */ WP_BUS_PARAM, APP_BANK_PARAM},
     { /* MFCPT, UPPPT, DS */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
     { /* STATISTICS2      */ WP_BUS_HOST, APP_BANK_HOST},
     { /* STATISTICS3      */ WP_BUS_HOST, APP_BANK_HOST},
     { /* UNASSIGNED      */ WP_BUS_NONE, 0},
     { /* PKT_GLOBAL_TCPT */ WP_BUS_PARAM, APP_BANK_PARAM},
     { /* UNASSIGNED      */ WP_BUS_NONE, 0},
     { /* MANDATORY       */ WP_BUS_NONE, 0},
     { /* MANDATORY       */ WP_BUS_NONE, 0},
     /* ************* VIRTUAL BUS ASSIGNMENT ************** */
     { /* MCH_DATA        */ WP_BUS_PACKET, APP_BANK_PACKET}
     },
    {
     dps_ProgramImage, dps_ProgramImage},
    {
     dps_PC_Table_Init, dps_PC_Table_Init},
    /* int_queue_data */ &int_queue_tables,
    /* pool_limit */ N_POOLS,
    /* atm_data */ &atm_global_cfg,
    /* iw_data */ &iw_global_cfg
    }
};

/* Dummy logging function */
void WP_AppLog (WP_CHAR * s)
{
}

/* The WPE_System data structure holds parameters
 * of the system of this example. It holds handles
 * of ports, devices, qnodes, channels, etc.
 */
#define WPE_RX_CH_TAG 0
#define WPE_TX_CH_TAG 1
typedef struct
{
   WP_U32 wpid;
   WP_handle port_handle;
   WP_handle trans_dev_handle;
   WP_handle mc_hdlc_dev_handle;
   WP_handle host_qnode_handle;
   WP_handle ch_handle[N_CHANNELS];
   WP_handle data_pool_handle;

   /********************************/
   WP_handle ch_handle_iw[N_TX_HDLC_CH];

   WP_handle pqblock1;
   WP_handle TDM2PSN_port_iwhost;
   WP_handle TDM2PSN_dev_iwhost;
   WP_handle TDM2PSN_chan_iwhost;

   WP_handle Enet_port;
   WP_handle Enet_dev;
   WP_handle Enet_chan_rx;
   WP_handle Enet_chan_tx;

   WP_handle TDM2PSN_bridge_port2ENET;
   WP_handle PSN2TDM_bridge_port2TDM;

   WP_handle h_pool_buffer_iw;
   WP_handle h_qnode_iwq;

   WP_handle h_qnode_iwq_hdlc;

   WP_handle h_iw_sys_pppsw;
   WP_handle bridge_system_TDM2PSN;
   WP_handle bridge_system_PSN2TDM;

   WP_handle TDM2PSN_bridge_filter[3];
   WP_handle TDM2PSN_bridge_dfcflow[4];
   WP_handle PSN2TDM_bridge_filter;
   WP_handle PSN2TDM_bridge_dfcflow;

   WP_handle TDM2PSN_default_agg;
   WP_handle PSN2TDM_default_agg;

   WP_handle h_flow_agg_cisco_control;
   WP_handle h_flow_agg_enet2;
   WP_handle h_flow_agg_lcp;

   WP_handle h_flow_agg_PSN2TDM_Pwe3[APP_MAX_TDM_PORTS];
   WP_handle h_flow_agg_PSN2TDM_Exact[APP_MAX_TDM_PORTS];

   WP_handle TDM2PSN_extract_fa[3];

   WP_handle h_flow_agg_pppsw_link[APP_MAX_TDM_PORTS];

   WP_handle TDM2PSN_bridge_port[APP_MAX_TDM_PORTS];
   WP_handle PSN2TDM_bridge_port[APP_MAX_TDM_PORTS];

   /********************************/

   WP_data_unit tx_data_unit[N_FRAMES];
   WP_data_segment tx_segment[N_FRAMES];
   WP_data_unit rx_data_unit;
   WP_data_segment rx_segment;
} WPE_system;
WPE_system wpe_system[1];

/* Prototypes of internal functions of this example */
static void WPE_SystemSetup (WPE_system * system);
static void WPE_SystemEnable (WPE_system * system);
static void WPE_McDevicesSetup (WPE_system * system);
static void WPE_McDevicesEnable (WPE_system * system);
static void WPE_McDevicesDelete (WPE_system * system);
static void WPE_ChannelsSetup (WPE_system * system);

/*************************************************************/

static void WPE_TDM2PSNSysSetup (WPE_system * system);
static void WPE_PSN2TDMSysSetup (WPE_system * system);

static void WPE_TDM2PSNHostInfoSetup (WPE_system * system);
static void WPE_PPPRxBinding (WPE_system * system);

/**************************************************************/
static void WPE_ChannelsEnable (WPE_system * system);
static void WPE_ChannelsDelete (WPE_system * system);
static void WPE_DataUnitsSetup (WPE_system * system);
static void WPE_PrintStatistics (WPE_system * the_system);

/* Utility function to handle return values from WDDI
   function calls */
static void WPE_TerminateOnError (WP_handle, WP_CHAR *);
static void WPE_Terminate (WP_U32 status);

void WP_ErrCallback (WP_CHAR * file, WP_CHAR * function, WP_U32 line,
                     WP_U32 error_id)
{

   printf
      ("\nWPI_ERROR in...File = %s \nFunction = %s \nLine = %d \nError_Id = %d \n",
       file, function, line, error_id);
   return;
}

void WP_MessageCallback (WP_S32 level, const WP_CHAR * string)
{
   if (8 == level)
      //printf("\nLog level = %d \nlog message = %s \n", level, string);

      return;
}

WP_U32 WP_DeviceCrcModify (WPE_system * system)
{
   WP_U32 status;

   WP_device_mc_hdlc mc_device;

   memset (&mc_device, 0, sizeof (mc_device));

   mc_device.crctype = WP_HDLC_CRC16;
   printf ("Disable Device\n");
   status =
      WP_DeviceDisable (system->mc_hdlc_dev_handle, WP_DIRECTION_DUPLEX);
   if (status)
   {
      WPE_TerminateOnError (status, "WP_DeviceDisable()");
      return status;
   }
   printf ("Modify Device\n");
   status =
      WP_DeviceModify (system->mc_hdlc_dev_handle,
                       WP_DEV_MOD_TDM_MC_HDLC_CRCTYPE, &mc_device);
   if (status)
   {
      WPE_TerminateOnError (status, "WP_DeviceModify()");
      return status;
   }
   printf ("Enable Device\n\n");
   status =
      WP_DeviceEnable (system->mc_hdlc_dev_handle, WP_DIRECTION_DUPLEX);
   if (status)
   {
      WPE_TerminateOnError (status, "WP_DeviceEnable()");
      return status;
   }
   return status;
}

WP_S32 main (WP_S32 argc, WP_CHAR ** argv)
{
   WPE_system *the_system;
   WP_status status;
   WP_U32 i, crc_size;

#if WPE_DEBUG_LEVEL > 0
   WP_U32 j;
#endif
   WP_U32 loop_cycles = 0;
   WP_U32 rx_frames = 0;
   WP_data_unit *data_unit;
   WP_data_segment *segment;

//WP_stats_tdi_hdlc tdi_hdlc[1];
   test_failed = 0;

    /*****************************************************************************
    * WP_ControlRegister - Attaches a callback function to be called whenever WDDI
    * 	   			     internally generates an error handle
    ****************************************************************************/
   status =
      WP_ControlRegister (WP_DEBUG_CALLBACK_FILE_LINE_ERROR,
                          &WP_ErrCallback);
   WPE_TerminateOnError (status, "WP_Control Register()");

   /* Initialize WDDI */
   status = WP_DriverInit ();
   WPE_TerminateOnError (status, "WP_DriverInit()");

   status = WPX_BoardConfigure (0, WPX_CONFIGURE_CHECKIN_TDI_16);
   WPE_TerminateOnError (status, "WPX_BoardConfigure()");

   the_system = wpe_system;

   /* Set up the system */
   WPE_SystemSetup (the_system);

  /*********************************/
   WPE_TDM2PSNHostInfoSetup (the_system); //Phenix
  /*********************************/

   /* System Commit */
   status = WP_SysCommit ();
   WPE_TerminateOnError (status, "WP_SysCommit()");

   /* Create the multi channel HDLC virtual devices */
   WPE_McDevicesSetup (the_system);

   /* Create the HDLC channels */
   WPE_ChannelsSetup (the_system);

/*******************************/

   WPE_TDM2PSNSysSetup (the_system);   //Phenix
   WPE_PSN2TDMSysSetup (the_system);
   WPE_PPPRxBinding (the_system);   //Phenix
/*******************************/

   /* Set up frames (data_units) for transmission */
   WPE_DataUnitsSetup (the_system);
   crc_size = ((mc_hdlc_dev_config[0].crctype == WP_HDLC_CRC16) ? 2 : 4);
   /* Enable the system */
   WPE_SystemEnable (the_system);

   status =
      WP_PortEnable (the_system->TDM2PSN_port_iwhost, WP_DIRECTION_DUPLEX);
   WPE_TerminateOnError (status, "WP_PortEnable() TDM2PSN_port_iwhost");

   status = WP_PortEnable (the_system->Enet_port, WP_DIRECTION_DUPLEX);
   WPE_TerminateOnError (status, "WP_PortEnable() Enet_port");

   /* Enable the multi channel vitual devices */
   WPE_McDevicesEnable (the_system);

   status =
      WP_DeviceEnable (the_system->TDM2PSN_dev_iwhost,
                       WP_DIRECTION_DUPLEX);
   WPE_TerminateOnError (status, "WP_DeviceEnable() (TDM2PSN_dev_iwhost)");

   status = WP_DeviceEnable (the_system->Enet_dev, WP_DIRECTION_DUPLEX);
   WPE_TerminateOnError (status, "WP_DeviceEnable() (Enet_dev)");

   /* Enable the channels */
   WPE_ChannelsEnable (the_system);

   status = WP_ChannelEnable (the_system->Enet_chan_rx);
   WPE_TerminateOnError (status, "WP_ChannelEnable() Enet_chan_rx");

   status = WP_ChannelEnable (the_system->Enet_chan_tx);
   WPE_TerminateOnError (status, "WP_ChannelEnable() Enet_chan_tx");

   status = WP_ChannelEnable (the_system->TDM2PSN_chan_iwhost);
   WPE_TerminateOnError (status, "WP_ChannelEnable() TDM2PSN_chan_iwhost");

   //return 0;
   /* Perform Host Send  */
   for (i = 0; i < N_FRAMES; i++)
   {
      data_unit = &(the_system->tx_data_unit[i]);
      status =
         WP_HostSend (the_system->ch_handle[WPE_TX_CH_TAG], data_unit);
      WPE_TerminateOnError (status, "WP_HostSend()");
   }

   /* Perform Host Receive  */
   rx_frames = 0;
   while (rx_frames < N_FRAMES)
   {
      data_unit = &(the_system->rx_data_unit);
      data_unit->n_active = 0;
      WP_HostReceive (the_system->ch_handle[WPE_RX_CH_TAG], data_unit);
      if (data_unit->n_active > 0)
      {
         segment = &(the_system->rx_segment);
#if WPE_DEBUG_LEVEL > 0
         printf ("Received frame:\n");
         printf ("---------------\n");
         for (j = 0; j < segment->data_size; j++)
         {
            printf ("%2.2x", segment->data[j]);
            if ((j + 1) % 32 == 0)
               printf ("\n");
         }
         printf (" --> ( %d bytes)\n\n", segment->data_size);
#endif
         if ((segment->data_size - crc_size) !=
             frame_config[rx_frames].length
             || segment->data[4] != frame_config[rx_frames].data_char)
            test_failed++;

         rx_frames++;

         /* Release data buffer back to its pool */
         WP_PoolFree (segment->pool_handle, segment->data);
      }
      else if (loop_cycles++ > N_POLL_CYCLES)
      {
         test_failed++;
         break;
      }
   }

   WP_Delay (500);
   WPE_PrintStatistics (the_system);
   do
   {
      //printf("\n1 -- Send a test packet to GigE1");
      //printf("\n2 -- Send a test packet to GigE2");
      //printf("\n3 -- Poll for a packet on GigE1");
      //printf("\n4 -- Poll for a packet on GigE2");
      //printf("\nS -- Print Statistics for both ports");
      //printf("\nD -- Dump phy registers for both ports");

      //printf("\nW -- Write a phy register");
      //printf("\nT -- Dump Debug Registers");
      //printf("\nP -- Control Callback function\n");/*add by Phenix*/

      ch = getchar ();
      if (ch == 's' || ch == 'S')
      {
         WPE_PrintStatistics (the_system);
      }
      else if (ch == 'C' || ch == 'c')
      {
         printf ("try to change CRC type\n");
         WP_DeviceCrcModify (the_system);
      }
   }
   while (ch != 'q');
#if 1
   /* Delete the channels */
   WPE_ChannelsDelete (the_system);

   /* Delete the multi channel vitual devices */
   WPE_McDevicesDelete (the_system);
#endif
   if (test_failed)
      printf ("Test Failed\n");
   else
      printf ("Test Passed\n");

   WPE_Terminate (0);

//   WT_Reboot();

   return 0;
}

static void WPE_SystemSetup (WPE_system * the_system)
{
   WP_status status;
   WP_U32 wpid;
   WP_handle pool_handle;

   WP_U32 bytes;

   the_system->wpid = wpid = WP_WINPATH (0);
   status = WP_SysInit (wpid, context);
   WPE_TerminateOnError (status, "WP_SysInit()");

   /* Create a TDM1 Port */
   the_system->port_handle =
      WP_PortCreate (wpid, WPE_PORT_TDM, tdm_config);
   WPE_TerminateOnError (the_system->port_handle, "WP_PortCreate() TDM1");

   // status = WPU_TdmCometCardInit(WPE_PORT_TDM, WPU_MODE_TDM_E1_MASTER),
   //WPE_TerminateOnError(status, "WPU_TdmCometCardInit()");

   /* Create the Buffer pool(s) */
   pool_handle = WP_PoolCreate (wpid, WP_pool_buffer, buffer_data);
   WPE_TerminateOnError (pool_handle, "WP_PoolCreate() Data Buffers");
   the_system->data_pool_handle = pool_handle;

   /* Create the Ring pool(s) */
   pool_handle = WP_PoolCreate (wpid, WP_pool_ring, ring_data);
   WPE_TerminateOnError (pool_handle, "WP_PoolCreate() Rings");

   /* Attach the Buffer and Ring pools to the qnode */
   host_qnode_config->pool_buffer = the_system->data_pool_handle;
   host_qnode_config->pool_ring = pool_handle;
   /* Create the Host termination Qnode for HDLC channels */
   the_system->host_qnode_handle = WP_QNodeCreate (wpid,
                                                   WP_QNODE_HOSTQ,
                                                   host_qnode_config);
   WPE_TerminateOnError (the_system->host_qnode_handle,
                         "WP_QNodeCreate()");

   /***************************///Phenix
   WP_qnode_iwq qnode_iwq_cfg = {
      /* interruptqueue         */ WP_IRQT1,
      /* num_buffers            */ 1000,
      /* adjunct_pool           */ 0,
   };

   WP_pool_buffer_data pool_buffer_data_iw_cfg = {
      /* n_buffers              */ APP_NUM_IW_BUFFERS,
      /* offset                 */ 64,
      /* size                   */ 1536,
      /* pad                    */ 0,
      /* bus                    */ WP_BUS_PACKET,
      /* bank                   */ APP_BANK_PACKET
   };

/**********************/
   /* Show available memory (for diagnostic purposes only) */
   status =
      WP_MemoryAvailable (WP_WINPATH (0), WP_BUS_PACKET, APP_BANK_PACKET,
                          &bytes);
   WPE_TerminateOnError (status, "WP_MemoryAvailable() PACKET");
   printf ("Packet bus bytes left = %d\n", bytes);
   status =
      WP_MemoryAvailable (WP_WINPATH (0), WP_BUS_PARAM, APP_BANK_PARAM,
                          &bytes);
   WPE_TerminateOnError (status, "WP_MemoryAvailable() PARAM");
   printf ("Param bus bytes left = %d\n", bytes);
   status =
      WP_MemoryAvailable (WP_WINPATH (0), WP_BUS_INTERNAL,
                          APP_BANK_INTERNAL, &bytes);
   WPE_TerminateOnError (status, "WP_MemoryAvailable() INTERNAL");
   printf ("Internal bus bytes left = %d\n", bytes);
   status =
      WP_MemoryAvailable (WP_WINPATH (0), WP_BUS_HOST, APP_BANK_HOST,
                          &bytes);
   WPE_TerminateOnError (status, "WP_MemoryAvailable() HOST");
   printf ("Host bus bytes left = %d\n", bytes);
/************************/
   /* Create Buffer pools */
   the_system->h_pool_buffer_iw =
      WP_PoolCreate (WP_WINPATH (0), WP_pool_iwbuffer,
                     &pool_buffer_data_iw_cfg);
   WPE_TerminateOnError (the_system->h_pool_buffer_iw,
                         "WP_PoolCreate iwbuffer iw");

   /* Create an interworking queue node */
   qnode_iwq_cfg.adjunct_pool = the_system->h_pool_buffer_iw;
   the_system->h_qnode_iwq =
      WP_QNodeCreate (WP_WINPATH (0),
                      WP_QNODE_IWQ | WP_QNODE_OPT_DEDICATED_RX_HWQ |
                      WP_QNODE_OPT_FMU, &qnode_iwq_cfg);
   WPE_TerminateOnError (the_system->h_qnode_iwq,
                         "WP_QNodeCreate() h_qnode_iwq");

   /* Create an interworking queue node */
   qnode_iwq_cfg.adjunct_pool = the_system->h_pool_buffer_iw;
   the_system->h_qnode_iwq_hdlc =
      WP_QNodeCreate (WP_WINPATH (0), WP_QNODE_IWQ, &qnode_iwq_cfg);
   WPE_TerminateOnError (the_system->h_qnode_iwq_hdlc,
                         "WP_QNodeCreate() h_qnode_iwq_tdm");

   WP_device_enet device_enet_cfg = {
      /* max_tx_channels */ 1,
      /* tx_maxsdu       */ 1536,
      /* rmii_operating_speed */ WP_UNUSED,
      /* mac_addr[6] */ {0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa},
      /* tx_bit_rate */ 1000000000,
      /* loopbackmode  */ WP_ENET_LOOPBACK,
      /* extended_params */ NULL,
   };

   WP_port_enet port_enet_cfg = {
      /* pkt_limits             */ {2, 2},
      /* flowmode               */ WP_FLOWMODE_FAST,
      /* miimode                */ WP_ENET_RGMII_1000,
      /* rx_iw_bkgnd            */ WP_IW_BKGND_USED,
   };

   status = WPU_EnetPhyInit (WP_PORT_ENET3, WPU_MODE_RGMII | WPU_OV_FIBER);
   WPE_TerminateOnError (status, " WPU_WinnetPhyInit - WP_PORT_ENET3");

   /* Create BRG1 & BRG3 */
   //status = WP_SysClockCreate(WP_WINPATH(0), WP_BRG1, WP_BRG_SRC_BRGIN2, 2);
   //status = WP_SysClockCreate(WP_WINPATH(0), WP_BRG2, WP_BRG_SRC_BRGIN2, 2);
   //status = WP_SysClockCreate(WP_WINPATH(0), WP_BRG3, WP_BRG_SRC_BRGIN2, 2);
   //status = WP_SysClockCreate(WP_WINPATH(0), WP_BRG4, WP_BRG_SRC_BRGIN2, 2);
   //status = WP_SysClockCreate(WP_WINPATH(0), WP_BRG5, WP_BRG_SRC_BRGIN2, 2);

   /* Create Enet port */
   the_system->Enet_port = WP_PortCreate (WP_WINPATH (0), WP_PORT_ENET3,
                                          &port_enet_cfg);
   WPE_TerminateOnError (the_system->Enet_port, "WP_PortCreate() Enet");

   /* Create device on Enet2 */
   MAC_COPY (device_enet_cfg.mac_addr, wt_mac_enet);

   the_system->Enet_dev =
      WP_DeviceCreate (the_system->Enet_port, WP_PHY (0), WP_DEVICE_ENET,
                       &device_enet_cfg);
   WPE_TerminateOnError (the_system->Enet_dev, "WP_DeviceCreate() Enet");

   /***************************/

   /* Create a Transparent MCH Device over TDM1 port */
   the_system->trans_dev_handle = WP_DeviceCreate (the_system->port_handle,
                                                   WP_UNUSED,
                                                   WP_DEVICE_TRANS,
                                                   trans_dev_config);
   WPE_TerminateOnError (the_system->trans_dev_handle,
                         "WP_DeviceCreate() TDM TRANS");

   status = WP_SysSchedulerCreate (wpid, cw_config);
   WPE_TerminateOnError (status, "WP_SysSchedulerCreate()");

}

static void WPE_McDevicesSetup (WPE_system * the_system)
{
   /* Create Multi Channel HDLC virtual device */
   the_system->mc_hdlc_dev_handle =
      WP_DeviceCreate (the_system->trans_dev_handle, WP_UNUSED,
                       WP_DEVICE_MC_HDLC, &mc_hdlc_dev_config[0]);
   WPE_TerminateOnError (the_system->mc_hdlc_dev_handle,
                         "WP_DeviceCreate() MC_HDLC");
}

static void WPE_ChannelsSetup (WPE_system * the_system)
{
   WP_handle handle;
   WP_status status;
   int ii;
   WP_pkt_shaping_wfq default_pkt_shaping_wfq_cfg = {
      /* weight;  */ 1,
      /* weight_fraction; */ 0
   };

   WP_ch_enet ch_enet_cfg = {
      /* intmode */ WP_PKTCH_INT_ENABLE,
      /* iwmmode */ WP_PKTCH_IWM_ENABLE,
      /* testmode */ WP_PKTCH_TEST_DISABLE,
      /* tx_pqblock */ 0,
      /* tx_pqlevel */ 0,
      /* tx_shaping_type */ WP_PKT_SHAPING_STRICT,
      /* tx_shaping_params */ &default_pkt_shaping_wfq_cfg,
      /* rx_maxsdu */ 1536,
      /* tx_cwid */ WP_CW_ID_A,
      /* tx_tq */ 0,
      /* rx_queuedepth */ 17,
   };

   WP_tx_binding tx_binding_cfg = {
      /* res0                      */ 0,
      /* dci_mode                  */ WP_IW_DYN_CH_INSERT_ENABLE,
      /* maxt                      */ 10,
   };

   WP_tx_binding tx_binding_hdlc[1] = {
      {
       /* res0 */ 0,
       /* dci_mode; */ WP_IW_DYN_CH_INSERT_ENABLE,
       /* maxt; */ 50,
       }
   };

   /* INTERWORKING BRIDGING */
   WP_ch_iw_rx ch_iw_rx_cfg = {
      /* pqblock            */ 0,

      /* pqlevel            */ 0,

      /* tx_binding_type    */ WP_IW_TX_BINDING,

      /* tx_binding_config  */ NULL,
   };

   ch_iw_rx_cfg.tx_binding_config = &tx_binding_cfg;

   the_system->TDM2PSN_chan_iwhost = WP_ChannelCreate (91,
                                                       the_system->
                                                       TDM2PSN_dev_iwhost,
                                                       the_system->
                                                       h_qnode_iwq,
                                                       WP_CH_RX,
                                                       WP_IW_HOST,
                                                       &ch_iw_rx_cfg);
   WPE_TerminateOnError (the_system->TDM2PSN_chan_iwhost,
                         "WP_DeviceCreate() IW Host Channel");

   /* Create Enet  channels */
   the_system->Enet_chan_rx = WP_ChannelCreate (20, the_system->Enet_dev,
                                                the_system->h_qnode_iwq,
                                                WP_CH_RX, WP_ENET,
                                                &ch_enet_cfg);
   WPE_TerminateOnError (the_system->Enet_chan_rx,
                         "WP_ChannelCreate() Enet Rx");

   the_system->Enet_chan_tx = WP_ChannelCreate (21, the_system->Enet_dev,
                                                the_system->h_qnode_iwq,
                                                WP_CH_TX, WP_ENET,
                                                &ch_enet_cfg);
   WPE_TerminateOnError (the_system->Enet_chan_tx,
                         "WP_ChannelCreate() Enet Tx");

   tx_binding_cfg.dci_mode = WP_IW_DYN_CH_INSERT_ENABLE;
   status =
      WP_IwTxBindingCreate (the_system->Enet_chan_tx, WP_IW_TX_BINDING,
                            &tx_binding_cfg);
   WPE_TerminateOnError (status, "WP_IwTxBindingCreate()");

   /* Create the HDLC channels */
   hdlc_ch_config[0].intmode = WP_PKTCH_INT_ENABLE;
   hdlc_ch_config[0].iwmode = WP_PKTCH_IWM_ENABLE;
   handle =
      WP_ChannelCreate (WPE_RX_CH_TAG, the_system->mc_hdlc_dev_handle,
                        the_system->host_qnode_handle, WP_CH_RX, WP_HDLC,
                        &hdlc_ch_config[0]);
   WPE_TerminateOnError (handle, "WP_ChannelCreate() HDLC RX");
   the_system->ch_handle[WPE_RX_CH_TAG] = handle;

   handle = WP_IwPQBlockCreate (the_system->port_handle, WP_HDLC);
   WPE_TerminateOnError (handle, "WP_IwPQBlockCreate()");
   the_system->pqblock1 = handle;

   //hdlc_ch_config[0].tx_pqblock = the_system->pqblock1;
   //hdlc_ch_config[0].tx_pqlevel = 1;////////////////////

   hdlc_ch_config[0].intmode = WP_PKTCH_INT_DISABLE;
   hdlc_ch_config[0].iwmode = WP_PKTCH_IWM_DISABLE;
   handle =
      WP_ChannelCreate (WPE_TX_CH_TAG, the_system->mc_hdlc_dev_handle,
                        the_system->host_qnode_handle, WP_CH_TX, WP_HDLC,
                        &hdlc_ch_config[0]);
   WPE_TerminateOnError (handle, "WP_ChannelCreate() HDLC TX");
   the_system->ch_handle[WPE_TX_CH_TAG] = handle;

   for (ii = 0; ii < N_TX_HDLC_CH; ii++)
   {
      hdlc_ch_config[0].tx_pqblock = the_system->pqblock1;
      hdlc_ch_config[0].tx_pqlevel = ii;  ////////////////////

      hdlc_ch_config[0].intmode = WP_PKTCH_INT_ENABLE;
      hdlc_ch_config[0].iwmode = WP_PKTCH_IWM_ENABLE;
      handle = WP_ChannelCreate (66 + ii, the_system->mc_hdlc_dev_handle,
                                 the_system->h_qnode_iwq_hdlc, WP_CH_TX,
                                 WP_HDLC, &hdlc_ch_config[0]);
      WPE_TerminateOnError (handle, "WP_ChannelCreate() HDLC TX");
      the_system->ch_handle_iw[ii] = handle;

      status =
         WP_IwTxBindingCreate (handle, WP_IW_TX_BINDING,
                               &tx_binding_hdlc[0]);
      WPE_TerminateOnError (status, "WP_IwTxBindingCreate() hdlc");

   }

}

static void WPE_TDM2PSNHostInfoSetup (WPE_system * the_system)
{

   /* Create an IW Host port */
   the_system->TDM2PSN_port_iwhost =
      WP_PortCreate (WP_WINPATH (0), WP_PORT_IW_HOST, NULL);
   WPE_TerminateOnError (the_system->TDM2PSN_port_iwhost,
                         "WP_PortCreate() IW Host Port");

   /* Create an IW Host Device */
   the_system->TDM2PSN_dev_iwhost =
      WP_DeviceCreate (the_system->TDM2PSN_port_iwhost, 0,
                       WP_DEVICE_IW_HOST, NULL);
   WPE_TerminateOnError (the_system->TDM2PSN_dev_iwhost,
                         "WP_DeviceCreate() IW Host Device");

}

static void WPE_PSN2TDMSysSetup (WPE_system * the_system)
{
   WP_classification_filter classification_filter;

   WP_dfc_sys_info dfc_sys_info = {
      /* classification hash size */ WP_IW_32K_HASH_ENTRIES,
      /* port filtering */ WP_IW_PORT_FILTERING_ENABLE,
      /* deny_eq_ip */ WP_IW_DENY_EQUAL_IP_DISABLE,
   };

   WP_iw_sys_bridging iw_sys_bridge = {
      /* tag                       */ 0,
      /* max_flows                 */ 10,
      /* classification mode       */ WP_IW_CLASSIFIER_BASED_BRIDGING,
      /* classifier config         */ {WP_IW_DYNAMIC_CLASS, 100, 0, NULL},
      /* learning_mode             */ WP_IW_BRIDGE_LEARNING_DISABLE,
      /* learning queue            */ {
                                       WP_IW_IRQT1,
                                       0,
                                       WP_LEARNING_INTENABLE,
                                       1,
                                       },
      /* forwarding_table_size     */ WP_IW_32K_HASH_ENTRIES,
      /* member_set_size           */ WP_IW_32K_HASH_ENTRIES,
      /* buffer_gap                */ 0x40,
      /* max_bridging_ports        */ APP_MAX_BPORTS,
      /* dfc_info                  */ &dfc_sys_info,
      /* svl_mode */ WP_IW_SVL_DISABLED,
   };

   /* Bridge1 ENET --> TDM */
   iw_sys_bridge.tag = 24;
   the_system->bridge_system_PSN2TDM = WP_IwSystemCreate (WP_WINPATH (0),
                                                          WP_IW_VLAN_AWARE_BRIDGING_MODE,
                                                          &iw_sys_bridge);
   WPE_TerminateOnError (the_system->bridge_system_PSN2TDM,
                         " WP_IwSystemCreate() Bridge PSN2TDM");

   /* DFC filter using 3 fields */
   classification_filter.fields_mask = WP_FIELD_IDS_ARRAY;
   classification_filter.ip_address_config.ip_sa_prefix_size = 32;
   classification_filter.ip_address_config.ip_da_prefix_size = 32;
   classification_filter.filter_type = WP_CLASS_FILTER_EXT_ACTION_EMC;
   classification_filter.field_ids_array[0] = WP_FIELD_ID_IN_PORT;
   classification_filter.field_ids_array[1] =
      WP_FIELD_ID_USER_PROGRAMMABLE;
   classification_filter.field_ids_array[2] =
      WP_FIELD_ID_USER_PROGRAMMABLE;
   classification_filter.field_ids_array[3] = WP_FIELD_ID_LAST;
   classification_filter.user_fields_config.num_of_user_fields = 1;
   classification_filter.user_fields_config.ref_point =
      WP_CLASS_REF_POINT_FRAME_START;
   classification_filter.user_fields_config.user_fields[0].field_offset =
      22; /*20 */ ;
   //classification_filter.user_fields_config.user_fields[1].field_offset = 6;

   classification_filter.user_fields_config.user_fields[0].field_size = 2;
   //classification_filter.user_fields_config.user_fields[1].field_size = 2;

   classification_filter.no_match_action = WP_CLASS_FILTER_NO_MATCH_DENY;

   the_system->PSN2TDM_bridge_filter =
      WP_IwClassFilterAdd (the_system->bridge_system_PSN2TDM,
                           &classification_filter);
   WPE_TerminateOnError (the_system->PSN2TDM_bridge_filter,
                         " WP_IwClassFilterAdd() Bridge PSN2TDM");

}

static void WPE_TDM2PSNSysSetup (WPE_system * the_system)
{
   WP_classification_filter classification_filter;

   WP_iw_sys_directmap iw_sys_pppsw_cfg = {
      /*  max_flows */ APP_MAX_TDM_PORTS,
      /*  buffer_gap */ 0x40
   };

   WP_dfc_sys_info dfc_sys_info = {
      /* classification hash size */ WP_IW_32K_HASH_ENTRIES,
      /* port filtering */ WP_IW_PORT_FILTERING_ENABLE,
      /* deny_eq_ip */ WP_IW_DENY_EQUAL_IP_DISABLE,
   };
   WP_iw_sys_bridging iw_sys_bridge = {
      /* tag                       */ 0,
      /* max_flows                 */ 10,
      /* classification mode       */ WP_IW_CLASSIFIER_BASED_BRIDGING,
      /* classifier config         */ {WP_IW_DYNAMIC_CLASS, 100, 0, NULL},
      /* learning_mode             */ WP_IW_BRIDGE_LEARNING_DISABLE,
      /* learning queue            */ {
                                       WP_IW_IRQT1,
                                       0,
                                       WP_LEARNING_INTENABLE,
                                       1,
                                       },
      /* forwarding_table_size     */ WP_IW_32K_HASH_ENTRIES,
      /* member_set_size           */ WP_IW_32K_HASH_ENTRIES,
      /* buffer_gap                */ 0x40,
      /* max_bridging_ports        */ APP_MAX_BPORTS,
      /* dfc_info                  */ &dfc_sys_info,
      /* svl_mode */ WP_IW_SVL_DISABLED,
   };

   /* PPP Switching System */
   the_system->h_iw_sys_pppsw =
      WP_IwSystemCreate (WP_WINPATH (0), WP_IW_PPP_SWITCHING_MODE,
                         &iw_sys_pppsw_cfg);
   WPE_TerminateOnError (the_system->h_iw_sys_pppsw,
                         "WP_IwSystemCreate() pppsw");

   /* TDM2PSN */
   iw_sys_bridge.tag = 10;
   the_system->bridge_system_TDM2PSN = WP_IwSystemCreate (WP_WINPATH (0),
                                                          WP_IW_VLAN_AWARE_BRIDGING_MODE,
                                                          &iw_sys_bridge);
   WPE_TerminateOnError (the_system->bridge_system_TDM2PSN,
                         " WP_IwSystemCreate() Bridge TDM2PSN");

   /* Create 3 USER PROGRAMMABLE FILTERS */

   /* 1. FOR CISCO HDLC TRAFFIC CONTROL AND DATA */
   classification_filter.fields_mask = WP_FIELD_IDS_ARRAY;
   classification_filter.ip_address_config.ip_sa_prefix_size = 32;
   classification_filter.ip_address_config.ip_da_prefix_size = 32;
   classification_filter.filter_type = WP_CLASS_FILTER_EMC;
   classification_filter.field_ids_array[0] = WP_FIELD_ID_IN_PORT;
   classification_filter.field_ids_array[1] =
      WP_FIELD_ID_USER_PROGRAMMABLE;
   classification_filter.field_ids_array[2] = WP_FIELD_ID_LAST;
   classification_filter.user_fields_config.num_of_user_fields = 1;
   classification_filter.user_fields_config.ref_point =
      WP_CLASS_REF_POINT_ETHER_TYPE;
   classification_filter.user_fields_config.user_fields[0].field_offset =
      2;

   classification_filter.user_fields_config.user_fields[0].field_size = 4;
   classification_filter.no_match_action =
      WP_CLASS_FILTER_NO_MATCH_CONTINUE;
   the_system->TDM2PSN_bridge_filter[0] =
      WP_IwClassFilterAdd (the_system->bridge_system_TDM2PSN,
                           &classification_filter);
   WPE_TerminateOnError (the_system->TDM2PSN_bridge_filter[0],
                         " WP_IwClassFilterAdd() TDM2PSN Filter 0");

   /* 2. FOR REGULAR PPP CONTROL TRAFFIC */
   classification_filter.user_fields_config.user_fields[0].field_size = 2;
   classification_filter.no_match_action =
      WP_CLASS_FILTER_NO_MATCH_CONTINUE;
   the_system->TDM2PSN_bridge_filter[1] =
      WP_IwClassFilterAdd (the_system->bridge_system_TDM2PSN,
                           &classification_filter);
   WPE_TerminateOnError (the_system->TDM2PSN_bridge_filter[1],
                         " WP_IwClassFilterAdd() TDM2PSN Filter 1");

   /* 3. FOR REGULAR PPP IP TRAFFIC */
   classification_filter.user_fields_config.user_fields[0].field_size = 1;
   classification_filter.no_match_action = WP_CLASS_FILTER_NO_MATCH_DENY;  //WP_CLASS_FILTER_NO_MATCH_DENY
   the_system->TDM2PSN_bridge_filter[2] =
      WP_IwClassFilterAdd (the_system->bridge_system_TDM2PSN,
                           &classification_filter);
   WPE_TerminateOnError (the_system->TDM2PSN_bridge_filter[2],
                         " WP_IwClassFilterAdd() TDM2PSN Filter 2");

}

static void WPE_PPPRxBinding (WPE_system * the_system)
{
   WP_rx_binding_ppp_switching rx_binding_pppsw;
   WP_feature_iw_pppsw_rx_lcp_forwarding pppsw_lcp;
   WP_U8 dummy_prefix_header[APP_MAX_TDM_PORTS][14];

   WP_U8 dummy_prefix_header1[APP_MAX_TDM_PORTS][14];
   WP_flow_class_forwarding_action forwarding_action;
   WP_flow_class_priority_action priority_action;

   //WP_class_filter_user_fields_config filter_user_fields;
   WP_dfc_flow_info dfc_flow_info;
   WP_flow_class_rule flow_class_rule;
   WP_flow_class_ext_action action;
   WP_U8 action_type[3];
   WP_flow_class_action_info action_info[3];
   WP_CHAR rule_string[128];

   WP_status status;
   int ii, jj;

   int loop;

#if 1
   WP_rx_binding_bridging rx_binding_bridging_cfg = {
      /* encap_mode                */ WP_IW_VCMUX,
      /* mru                       */ 1536,
      /* vcfcs                     */ 0,
      /* input_port                */ 0,
   };
#endif
   WP_iw_agg_bridging default_iw_agg_bridging_cfg = {
      /* tag                       */ 0,
      /* txfunc                    */ 0,
      /* bport                     */ 0,
      /* rfcs                      */ WP_IW_RFCS_ENABLE,
      /* l2_header_insert_mode     */ WP_IW_L2H_INSERT_DISABLE,
      /* vlan_tag_mode             */ WP_IW_VLAN_TAG_DISABLE,
      /* interruptqueue            */ WP_IW_IRQT1,
      /* error_pkt_mode            */ WP_IW_ERRPKT_DISCARD,
      /* intmode                   */ WP_IW_INT_ENABLE,
      /* statmode                  */ WP_IW_STAT_ENABLE,
      /* timestamp_mode            */ WP_IW_TIME_STAMP_DISABLE,
      /* ov_pool_mode              */ WP_IW_OV_POOL_DISABLE,
      /* fbp_drop_threshold        */ 0,
      /* replace_vlan_tag          */ WP_IW_REPLACE_VTAG_DISABLE,
      /* vlan_id                   */ 0,
      /* vpmt_handle               */ 0,
      /* mtu                       */ 1536,
      /* prefix_length             */ 0,
      /* prefix_header[28]         */ {0},
      /* policer_enable            */ WP_IW_POLICER_DISABLE,
      /* policer_config            */ NULL,
      /* cong_mode                 */ WP_IW_CONGESTION_DISABLE,
      /* cong_threshold_param      */ NULL,
      /* flow_agg_type             */ WP_IW_FLOW_AGG_PRIMARY,
      /* external_vlan */ {
                           WP_IW_EXTERN_VLAN_NONE,
                           0},
      /* bc_mc_priority_mapping */ {
                                    WP_IW_BC_PRIORITY_DISABLE,
                                    0,
                                    WP_IW_MC_PRIORITY_DISABLE,
                                    0},
      /* reserved_agg_mode */ WP_IW_NOT_RESERVED,
      /* next_preserve_inport */ WP_IW_OVERRIDE_INPORT,
      /* next_preserve_inport_params */ 0,
      /* mac_replace_mode */ WP_IW_MAC_REPLACE_DISABLED,
      /* new dst mac */ {0},
      /* new_src_mac */ {0},
      /* multi_cong_prof */ 0,
      /* extraction_length */ 0,
   };

   WP_iw_agg_ppp_switching iw_agg_ppp_switching = {
      /* tag */ 0,
      /* txfunc */ 0,
      /* l2header_insert_mode */ WP_IW_L2H_INSERT_DISABLE,
      /* interruptqueue */ WP_IW_IRQT1,
      /* error_pkt_mode */ WP_IW_ERRPKT_DISCARD,
      /* intmode */ WP_IW_INT_DISABLE,
      /* statmode */ WP_IW_STAT_ENABLE,
      /* timestamp_mode */ WP_IW_TIME_STAMP_DISABLE,
      /* ov_pool_mode */ 0,
      /* fbp_drop_threshold */ 0,
      /* mtu */ 1536,
      /* prefix_length */ 0,
      /* extraction_length */ 0,
      /* prefix_header[36] */ {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                               0x00,
                               0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                               0x00,
                               0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                               0x00,
                               0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                               0x00,
                               0x00, 0x00, 0x00, 0x00},
      /* policer_enable */ 0,
      /* policer_config */ NULL,
      /* cong_mode */ 0,
      /* cong_threshold_param */ NULL,
      /* flow agg type */ WP_IW_AGG_PRIMARY,
      /* acfc_mode */ WP_IW_ACFC_DISABLE,
      /* pfc_mode */ WP_IW_PFC_DISABLE,
      /* policer handle */ 0,
      /* iwport */ 0,
   };

   WP_iw_agg_bridging iw_agg_bridging_cfg = {
      /* tag                       */ 0,
      /* txfunc                    */ 0,
      /* bport                     */ 0,
      /* rfcs                      */ WP_IW_RFCS_ENABLE,
      /* l2_header_insert_mode     */ WP_IW_L2H_INSERT_DISABLE,
      /* vlan_tag_mode             */ WP_IW_VLAN_TAG_DISABLE,
      /* interruptqueue            */ WP_IW_IRQT1,
      /* error_pkt_mode            */ WP_IW_ERRPKT_DISCARD,
      /* intmode                   */ WP_IW_INT_ENABLE,
      /* statmode                  */ WP_IW_STAT_ENABLE,
      /* timestamp_mode            */ WP_IW_TIME_STAMP_DISABLE,
      /* ov_pool_mode              */ WP_IW_OV_POOL_DISABLE,
      /* fbp_drop_threshold        */ 0,
      /* replace_vlan_tag          */ WP_IW_REPLACE_VTAG_DISABLE,
      /* vlan_id                   */ 0,
      /* vpmt_handle               */ 0,
      /* mtu                       */ 1536,
      /* prefix_length             */ 0,
      /* prefix_header[28]         */ {0},
      /* policer_enable            */ WP_IW_POLICER_DISABLE,
      /* policer_config            */ NULL,
      /* cong_mode                 */ WP_IW_CONGESTION_DISABLE,
      /* cong_threshold_param      */ NULL,
      /* flow_agg_type             */ WP_IW_FLOW_AGG_PRIMARY,
      /* external_vlan */ {
                           WP_IW_EXTERN_VLAN_NONE,
                           0},
      /* bc_mc_priority_mapping */ {
                                    WP_IW_BC_PRIORITY_DISABLE,
                                    0,
                                    WP_IW_MC_PRIORITY_DISABLE,
                                    0},
      /* reserved_agg_mode */ WP_IW_NOT_RESERVED,
      /* next_preserve_inport */ WP_IW_OVERRIDE_INPORT,
      /* next_preserve_inport_params */ 0,
      /* mac_replace_mode */ WP_IW_MAC_REPLACE_DISABLED,
      /* new dst mac */ {0},
      /* new_src_mac */ {0},
      /* multi_cong_prof */ 0,
      /* extraction_length */ 0,
   };

   WP_bridge_port bridge_port_cfg = {
      /* tag                       */ 0,
      /* direct_mapping            */ WP_IW_DIRECT_MAP_DISABLE,
      /* flow_agg                  */ 0,
      /* termination_mode          */ WP_IW_HOST_TERM_MODE,
      /* learning_mode             */ WP_IW_LEARNING_DISABLE,
      /* in_filter_mode            */ WP_IW_INGRESS_FILTER_DISABLE,
      /* vlan_param                */ {
                                       WP_IW_ACCEPT_TAGGED_UNTAGGED,
                                       0,
                                       /* vlan tunnel */ 0,
                                       /* vlan_priority_enforcement */
                                       WP_IW_VLAN_PRIORITY_ENFORCE_DISABLED,
                                       },
      /* max_mac_addresses         */ APP_MAX_MAC_ADDRESS,
      /* group_tag                 */ 0,
      /* group_filtering_mode      */ WP_IW_GROUP_FILTER_DISABLE,
      /*unk_mac_sa_filter; */ 0,
      /*unk_mc_mode; */ 0,
      /*bc_ht_mode; */ 0,
      /*input_filters_mask; */ 0,
      /*output_filters_mask; */ 0,
      /*statmode; */ WP_IW_PORT_STAT_ENABLE,
      /* unk_uc_mode; */ 0,
      /* classification_flag; */ WP_IW_IWPORT_CLASSIFICATION_ENABLED,
      /* adv_unk_lookup_mode; */ 0,
      /* cfi_ht_mode; */ 0,
      /* reserved_mc_ht_mode; */ WP_IW_RES_MC_HT_ENABLE,
   };

#if 1
   WP_iw_agg_directmap iw_agg_directmap_cfg = {
      /* tag */ 0,
      /* txfunc */ 0,
      /* rfcs */ WP_IW_RFCS_ENABLE,
      /* l2header_insert_mode */ WP_IW_L2H_INSERT_DISABLE,
      /* interruptqueue */ WP_IW_IRQT1,
      /* error_pkt_mode */ WP_IW_ERRPKT_DISCARD,
      /* intmode */ WP_IW_INT_DISABLE,
      /* statmode */ WP_IW_STAT_ENABLE,
      /* timestamp_mode */ WP_IW_TIME_STAMP_DISABLE,
      /* ov_pool_mode */ 0,
      /* fbp_drop_threshold */ 0,
      /* mtu */ 1536,
      /* prefix_length */ 0,
      /* extraction_length */ 0,
      /* prefix_header[36] */ {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                               0x00,
                               0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                               0x00,
                               0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                               0x00,
                               0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                               0x00,
                               0x00, 0x00, 0x00, 0x00},
      /* policer_enable */ 0,
      /* policer_config */ NULL,
      /* cong_mode */ 0,
      /* cong_threshold_param */ NULL
   };
#endif

   /* Dummy mac addresses used to send into Bridge2 */
   for (ii = 0; ii < APP_MAX_TDM_PORTS; ii++)
   {
      for (jj = 0; jj < 12; jj++)
      {
         dummy_prefix_header[ii][jj] = jj;
      }

      for (jj = 0; jj < 12; jj++)
      {
         dummy_prefix_header1[ii][jj] = 0xaa;
      }

      dummy_prefix_header[ii][12] = 0x88;
      dummy_prefix_header[ii][13] = 0x47;

      dummy_prefix_header1[ii][12] = 0x00;
      dummy_prefix_header1[ii][13] = 0x2e;
   }

   /* Create Default Aggregation for Bridge */
   iw_agg_bridging_cfg.tag = 11;
   iw_agg_bridging_cfg.txfunc = the_system->TDM2PSN_chan_iwhost;
   iw_agg_bridging_cfg.input_bport = 0;
   iw_agg_bridging_cfg.intmode = WP_IW_INT_ENABLE;

   the_system->TDM2PSN_default_agg =
      WP_IwFlowAggregationCreate (WP_WINPATH (0),
                                  WP_IW_VLAN_AWARE_BRIDGING_MODE,
                                  &iw_agg_bridging_cfg);
   WPE_TerminateOnError (the_system->TDM2PSN_default_agg,
                         " WP_IwFlowAggregationCreate() TDM2PSN Default FA");

   for (ii = 0; ii < 1 /*APP_MAX_TDM_PORTS */ ; ii++)
   {

      /* Create a bridge port for each HDLC channel */
      bridge_port_cfg.tag = 12 + ii;
      bridge_port_cfg.flow_agg = the_system->TDM2PSN_default_agg;
      bridge_port_cfg.input_filters_mask = 0xe0000000;
      bridge_port_cfg.classification_flag =
         WP_IW_IWPORT_CLASSIFICATION_ENABLED;
      the_system->TDM2PSN_bridge_port[ii] =
         WP_IwBridgingPortCreate (the_system->bridge_system_TDM2PSN,
                                  &bridge_port_cfg);
      WPE_TerminateOnError (the_system->TDM2PSN_bridge_port[ii],
                            "WP_IwBridgingPortCreate() Bridge2 TDM");

      /* Create a PPP Switching flow agg for each TDM port */
      iw_agg_ppp_switching.tag = 13 + jj;
      iw_agg_ppp_switching.txfunc = the_system->bridge_system_TDM2PSN;
      iw_agg_ppp_switching.iw_port = the_system->TDM2PSN_bridge_port[ii];
      iw_agg_ppp_switching.prefix_length = 14;
      iw_agg_ppp_switching.l2_header_insert_mode = WP_IW_L2H_INSERT_ENABLE;

      memcpy (&iw_agg_ppp_switching.prefix_header,
              &dummy_prefix_header[ii], 14);

      printf ("ppp sw prefix_header:\n");
      for (loop = 0; loop < 14; loop++)
      {
         printf (" %02x ", iw_agg_ppp_switching.prefix_header[loop]);
      }
      printf ("\n");

      the_system->h_flow_agg_pppsw_link[ii] =
         WP_IwFlowAggregationCreate (WP_WINPATH (0),
                                     WP_IW_PPP_SWITCHING_MODE,
                                     &iw_agg_ppp_switching);

#if 1
      /* Bind the TDM RX channel to the PPP Switching System using this flow agg */
      rx_binding_pppsw.aggregation = the_system->h_flow_agg_pppsw_link[ii];
      rx_binding_pppsw.mru = 1536;
      rx_binding_pppsw.encap_mode = WP_IW_VCMUX;
      rx_binding_pppsw.acfc_mode = WP_IW_ACFC_ENABLE;
      rx_binding_pppsw.pfc_mode = WP_IW_PFC_ENABLE;
      rx_binding_pppsw.pppmux_ncp_filter = WP_IW_PPPMUX_NCP_FILTER_SWITCH;
      rx_binding_pppsw.forward_ppp_to_bundle_mode =
         WP_IW_PPPSW_FORWARD_PPP_TO_LINK;

      status = WP_IwRxBindingCreate (the_system->ch_handle[WPE_RX_CH_TAG],
                                     the_system->h_iw_sys_pppsw,
                                     the_system->h_qnode_iwq,
                                     &rx_binding_pppsw);
      WPE_TerminateOnError (status, " WP_IwRxBindingCreate() PPPSW");
#endif

      /* ENABLE LCP forwarding on the HDLC RX channel, so that LCP packets come into the PPP Switching system instead of being host terminated */
      pppsw_lcp.iw_system = the_system->h_iw_sys_pppsw;
      pppsw_lcp.aggregation = the_system->h_flow_agg_pppsw_link[ii];
      status =
         WP_FeatureInit (the_system->ch_handle[WPE_RX_CH_TAG],
                         WP_FEATURE_IW_PPPSW_RX_LCP_FORWARDING_MODE,
                         &pppsw_lcp);
      WPE_TerminateOnError (status, "WP_FeatureInit LCP");
   }

   /* Create a bridge port to connect ENET */
   bridge_port_cfg.tag = 18;
   bridge_port_cfg.flow_agg = the_system->TDM2PSN_default_agg;
   bridge_port_cfg.input_filters_mask = 0;
   bridge_port_cfg.classification_flag =
      WP_IW_IWPORT_CLASSIFICATION_DISABLED;
   the_system->TDM2PSN_bridge_port2ENET =
      WP_IwBridgingPortCreate (the_system->bridge_system_TDM2PSN,
                               &bridge_port_cfg);
   WPE_TerminateOnError (the_system->TDM2PSN_bridge_port2ENET,
                         "WP_IwBridgingPortCreate() connect to ENET");

   WP_iw_agg_pwe3 pwe3_hdlc_flow_agg_config;

   //WP_U32 mpls_label = WTI_MPLS_LABEL + ((ii +1) << 12);

   memset (&pwe3_hdlc_flow_agg_config, 0, sizeof (WP_iw_agg_pwe3));

   //memcpy(pwe3_hdlc_flow_agg_config.prefix_header, &mpls_label, MPLS_SIZE);     

   memcpy (&pwe3_hdlc_flow_agg_config.prefix_header,
           &dummy_prefix_header1[ii], 14);
   //memcpy(&pwe3_hdlc_flow_agg_config.prefix_header[14], &mpls_label, MPLS_SIZE);

   pwe3_hdlc_flow_agg_config.prefix_header[14] = 0x10;
   pwe3_hdlc_flow_agg_config.prefix_header[15] = 0x01;
   pwe3_hdlc_flow_agg_config.prefix_header[16] = 0x01;
   pwe3_hdlc_flow_agg_config.prefix_header[17] = 0xff;

   pwe3_hdlc_flow_agg_config.prefix_length = 14 + MPLS_SIZE;
   //printf("mpls_label = 0x%x\n",mpls_label);

   printf ("PWe3 prefix_header:\n");
   for (loop = 0; loop < 18; loop++)
   {
      printf (" %02x ", pwe3_hdlc_flow_agg_config.prefix_header[loop]);
   }
   printf ("\n");

   pwe3_hdlc_flow_agg_config.tag = 30;
   pwe3_hdlc_flow_agg_config.txfunc = the_system->Enet_chan_tx;   /* this parameter is set outside this function */
   pwe3_hdlc_flow_agg_config.rfcs = 0;
   pwe3_hdlc_flow_agg_config.intmode = WP_DISABLE;
   pwe3_hdlc_flow_agg_config.interruptqueue = WP_IRQT3;
   pwe3_hdlc_flow_agg_config.statmode = WP_ENABLE;
   pwe3_hdlc_flow_agg_config.timestamp_mode = WP_IW_TIME_STAMP_DISABLE;
   pwe3_hdlc_flow_agg_config.ov_pool_mode = WP_IW_OV_POOL_DISABLE;
   pwe3_hdlc_flow_agg_config.fbp_drop_threshold = 0;

   pwe3_hdlc_flow_agg_config.mtu = 1536;
   pwe3_hdlc_flow_agg_config.flow_agg_type = WP_IW_AGG_PRIMARY;
   pwe3_hdlc_flow_agg_config.encapsulation_mode = WP_PWE3_HDLC;
   pwe3_hdlc_flow_agg_config.direction = WP_PWE3_INGRESS;   /* egress updated later */
   pwe3_hdlc_flow_agg_config.sn_mode = WP_ENABLE;
   pwe3_hdlc_flow_agg_config.control_word_mode = WP_ENABLE;
   pwe3_hdlc_flow_agg_config.header_replace_mode = WP_PWE3_REPLACE_NOTHING;
   pwe3_hdlc_flow_agg_config.header = 0;
   pwe3_hdlc_flow_agg_config.extraction_length = 0;
   pwe3_hdlc_flow_agg_config.switchover_threshold = 10;
   pwe3_hdlc_flow_agg_config.l3_length_update = WP_DISABLE;
   pwe3_hdlc_flow_agg_config.l3_header_offset = 0;

   /* LCP flow agg - PPP LCP packets (0xC021) go here */
#if 0
   iw_agg_directmap_cfg.tag = 15;
   iw_agg_directmap_cfg.txfunc = h_bridge2_iwhost;
   iw_agg_directmap_cfg.iw_port = 0;
   iw_agg_directmap_cfg.extraction_length = 14;
   iw_agg_directmap_cfg.prefix_length = 0;
   iw_agg_directmap_cfg.l2_header_insert_mode = WP_IW_L2H_INSERT_DISABLE;
   iw_agg_directmap_cfg.intmode = WP_IW_INT_ENABLE;
#endif
   the_system->h_flow_agg_lcp =
      WP_IwFlowAggregationCreate (WP_WINPATH (0), WP_IW_PWE3_MODE,
                                  &pwe3_hdlc_flow_agg_config
                                  /*&iw_agg_directmap_cfg */ );
   WPE_TerminateOnError (the_system->h_flow_agg_lcp,
                         " WP_IwFlowAggregationCreate() LCP");

   /* Cisco HDLC Control Packets go here to this flow agg (0x0F008035)  */
#if 0
   iw_agg_directmap_cfg.tag = 16;
   iw_agg_directmap_cfg.txfunc = h_bridge2_iwhost;
   iw_agg_directmap_cfg.iw_port = 0;
   iw_agg_directmap_cfg.extraction_length = 14;
   iw_agg_directmap_cfg.prefix_length = 0;
   iw_agg_directmap_cfg.l2_header_insert_mode = WP_IW_L2H_INSERT_DISABLE;
   iw_agg_directmap_cfg.intmode = WP_IW_INT_ENABLE;
#endif
   the_system->h_flow_agg_cisco_control =
      WP_IwFlowAggregationCreate (WP_WINPATH (0), WP_IW_PWE3_MODE,
                                  &pwe3_hdlc_flow_agg_config
                                  /*&iw_agg_directmap_cfg */ );
   WPE_TerminateOnError (the_system->h_flow_agg_cisco_control,
                         " WP_IwFlowAggregationCreate() CC");

   /* Data packets go here - both PPP IP (0x21) and CISCO HDLC IP (0x0F000800) */
#if 0
   iw_agg_bridging_cfg.tag = 17;
   iw_agg_bridging_cfg.txfunc = the_system->Enet_chan_tx;
   iw_agg_bridging_cfg.iw_port = the_system->TDM2PSN_bridge_port2ENET;
#endif
   the_system->h_flow_agg_enet2 =
      WP_IwFlowAggregationCreate (WP_WINPATH (0), WP_IW_PWE3_MODE,
                                  &pwe3_hdlc_flow_agg_config
                                  /*&iw_agg_bridging_cfg */ );
   WPE_TerminateOnError (the_system->h_flow_agg_enet2,
                         " WP_IwFlowAggregationCreate() ENET2  ");

   iw_agg_directmap_cfg.tag = 16;
   iw_agg_directmap_cfg.txfunc = the_system->h_flow_agg_lcp;
   iw_agg_directmap_cfg.iw_port = 0;
   iw_agg_directmap_cfg.extraction_length = 14;
   iw_agg_directmap_cfg.prefix_length = 0;
   iw_agg_directmap_cfg.l2_header_insert_mode = WP_IW_L2H_INSERT_DISABLE;
   iw_agg_directmap_cfg.intmode = WP_IW_INT_ENABLE;

   the_system->TDM2PSN_extract_fa[0] =
      WP_IwFlowAggregationCreate (WP_WINPATH (0), WP_IW_DIRECTMAP_MODE,
                                  &iw_agg_directmap_cfg);
   WPE_TerminateOnError (the_system->TDM2PSN_extract_fa[0],
                         " WP_IwFlowAggregationCreate() TDM2PSN_extract_fa[0]");

   iw_agg_directmap_cfg.tag = 17;
   //iw_agg_directmap_cfg.txfunc = the_system->h_flow_agg_lcp;
   iw_agg_directmap_cfg.iw_port = 0;
   iw_agg_directmap_cfg.extraction_length = 14;
   iw_agg_directmap_cfg.prefix_length = 0;
   iw_agg_directmap_cfg.l2_header_insert_mode = WP_IW_L2H_INSERT_DISABLE;
   iw_agg_directmap_cfg.intmode = WP_IW_INT_ENABLE;

   iw_agg_directmap_cfg.txfunc = the_system->h_flow_agg_cisco_control;

   the_system->TDM2PSN_extract_fa[1] =
      WP_IwFlowAggregationCreate (WP_WINPATH (0), WP_IW_DIRECTMAP_MODE,
                                  &iw_agg_directmap_cfg);
   WPE_TerminateOnError (the_system->TDM2PSN_extract_fa[1],
                         " WP_IwFlowAggregationCreate() TDM2PSN_extract_fa[1]");

   iw_agg_directmap_cfg.tag = 18;
   //iw_agg_directmap_cfg.txfunc = the_system->h_flow_agg_lcp;
   iw_agg_directmap_cfg.iw_port = 0;
   iw_agg_directmap_cfg.extraction_length = 14;
   iw_agg_directmap_cfg.prefix_length = 0;
   iw_agg_directmap_cfg.l2_header_insert_mode = WP_IW_L2H_INSERT_DISABLE;
   iw_agg_directmap_cfg.intmode = WP_IW_INT_ENABLE;

   iw_agg_directmap_cfg.txfunc = the_system->h_flow_agg_enet2;

   the_system->TDM2PSN_extract_fa[2] =
      WP_IwFlowAggregationCreate (WP_WINPATH (0), WP_IW_DIRECTMAP_MODE,
                                  &iw_agg_directmap_cfg);
   WPE_TerminateOnError (the_system->TDM2PSN_extract_fa[2],
                         " WP_IwFlowAggregationCreate() TDM2PSN_extract_fa[2]");

   /* Add DFC flows */

   /* NOTE for this test we are using TDM#1 to send out and receive the packet, so we use the bridge port for TDM 1 */
   dfc_flow_info.input_port = the_system->TDM2PSN_bridge_port[0]; /* Note that Input_port is one of the fields in the filter */
   dfc_flow_info.output_port = 0;

   /* While adding these flows, make sure to use the correct filter for each flow */

   /* CISCO HDLC Control packets flow */
   dfc_flow_info.filter_handle = the_system->TDM2PSN_bridge_filter[0];
   strcpy (rule_string, "n;n;n;n;n;n;#;0x0f008035");

   forwarding_action.flow_aggregation = the_system->TDM2PSN_extract_fa[1];
   flow_class_rule.classifier_string = rule_string;
   flow_class_rule.dfc_info = &dfc_flow_info;
   flow_class_rule.match_action = WP_FLOW_CLASS_MODE_FLOW_AGG;
   flow_class_rule.action_info.forwarding_action = forwarding_action;
   flow_class_rule.wred_entry = 0;

   the_system->TDM2PSN_bridge_dfcflow[0] =
      WP_IwFlowAdd (the_system->bridge_system_TDM2PSN, WP_FLOW_CLASS_RULE,
                    &flow_class_rule);
   WPE_TerminateOnError (the_system->TDM2PSN_bridge_dfcflow[0],
                         " WP_IwFlowAdd() DFC Bridge2");

   /* CISCO HDLC Data packets flow *//* Same filter as above using 4 byte user fields */
   dfc_flow_info.filter_handle = the_system->TDM2PSN_bridge_filter[0];
   strcpy (rule_string, "n;n;n;n;n;n;#;0x0f000800");

   forwarding_action.flow_aggregation = the_system->TDM2PSN_extract_fa[2];
   flow_class_rule.classifier_string = rule_string;
   flow_class_rule.dfc_info = &dfc_flow_info;
   flow_class_rule.match_action = WP_FLOW_CLASS_MODE_FLOW_AGG;
   flow_class_rule.action_info.forwarding_action = forwarding_action;
   flow_class_rule.wred_entry = 0;

   the_system->TDM2PSN_bridge_dfcflow[1] =
      WP_IwFlowAdd (the_system->bridge_system_TDM2PSN, WP_FLOW_CLASS_RULE,
                    &flow_class_rule);
   WPE_TerminateOnError (the_system->TDM2PSN_bridge_dfcflow[1],
                         " WP_IwFlowAdd() DFC Bridge2");

   /* PPP LCP flow *//* Note the use of FILTER 1 */
   dfc_flow_info.filter_handle = the_system->TDM2PSN_bridge_filter[1];
   strcpy (rule_string, "n;n;n;n;n;n;#;0xc021");

   forwarding_action.flow_aggregation = the_system->TDM2PSN_extract_fa[0];
   flow_class_rule.classifier_string = rule_string;
   flow_class_rule.dfc_info = &dfc_flow_info;
   flow_class_rule.match_action = WP_FLOW_CLASS_MODE_FLOW_AGG;
   flow_class_rule.action_info.forwarding_action = forwarding_action;
   flow_class_rule.wred_entry = 0;

   the_system->TDM2PSN_bridge_dfcflow[2] =
      WP_IwFlowAdd (the_system->bridge_system_TDM2PSN, WP_FLOW_CLASS_RULE,
                    &flow_class_rule);
   WPE_TerminateOnError (the_system->TDM2PSN_bridge_dfcflow[2],
                         " WP_IwFlowAdd() DFC Bridge2");

   /* PPP IP flow *//* Note the use of FILTER 2 */
   dfc_flow_info.filter_handle = the_system->TDM2PSN_bridge_filter[2];
   strcpy (rule_string, "n;n;n;n;n;n;#;0x21");

   forwarding_action.flow_aggregation = the_system->TDM2PSN_extract_fa[2];
   flow_class_rule.classifier_string = rule_string;
   flow_class_rule.dfc_info = &dfc_flow_info;
   flow_class_rule.match_action = WP_FLOW_CLASS_MODE_FLOW_AGG;
   flow_class_rule.action_info.forwarding_action = forwarding_action;
   flow_class_rule.wred_entry = 0;

   the_system->TDM2PSN_bridge_dfcflow[3] =
      WP_IwFlowAdd (the_system->bridge_system_TDM2PSN, WP_FLOW_CLASS_RULE,
                    &flow_class_rule);
   WPE_TerminateOnError (the_system->TDM2PSN_bridge_dfcflow[3],
                         " WP_IwFlowAdd(3) DFC Bridge");

   /* Be sure to build the system */
   status = WP_IwSystemBuild (the_system->bridge_system_TDM2PSN);
   WPE_TerminateOnError (status, " WP_IwSystemBuild() TDM2PSN Bridge SYS");

/*******************engress direction***********************/

   memcpy (&iw_agg_bridging_cfg, &default_iw_agg_bridging_cfg,
           sizeof (WP_iw_agg_bridging));

   /* Create Default Aggregation Bridge1 */
   iw_agg_bridging_cfg.tag = 25;
   iw_agg_bridging_cfg.txfunc = the_system->TDM2PSN_chan_iwhost;
   iw_agg_bridging_cfg.input_bport = 0;
   iw_agg_bridging_cfg.intmode = WP_IW_INT_ENABLE;

   the_system->PSN2TDM_default_agg =
      WP_IwFlowAggregationCreate (WP_WINPATH (0),
                                  WP_IW_VLAN_AWARE_BRIDGING_MODE,
                                  &iw_agg_bridging_cfg);
   WPE_TerminateOnError (the_system->PSN2TDM_default_agg,
                         " WP_IwFlowAggregationCreate() Default Bridge");

   iw_agg_bridging_cfg.intmode = WP_IW_INT_DISABLE;

   bridge_port_cfg.classification_flag =
      WP_IW_IWPORT_CLASSIFICATION_ENABLED;
   bridge_port_cfg.tag = 89;
   bridge_port_cfg.flow_agg = the_system->TDM2PSN_default_agg;
   bridge_port_cfg.input_filters_mask = 0x80000000;

   the_system->PSN2TDM_bridge_port2TDM =
      WP_IwBridgingPortCreate (the_system->bridge_system_PSN2TDM,
                               &bridge_port_cfg);
   WPE_TerminateOnError (the_system->PSN2TDM_bridge_port2TDM,
                         "WP_IwBridgingPortCreate()ENET1");
#if 1
   /* Rx binding ENET 1 */
   rx_binding_bridging_cfg.input_port =
      the_system->PSN2TDM_bridge_port2TDM;
   status =
      WP_IwRxBindingCreate (the_system->Enet_chan_rx,
                            the_system->bridge_system_PSN2TDM,
                            the_system->h_qnode_iwq,
                            &rx_binding_bridging_cfg);
   WPE_TerminateOnError (status, " WP_IwRxBindingCreate() enet1 ");
#endif
#if 0
   /* EACH TDM port is represented as bridge port in Bridge1 too so a packet going from ENET to TDM has the ENET bridge port as input bridge port and TDM
    * bridge port as the output bridge port */
   for (ii = 0; ii < 8; ii++)
   {
      bridge_port_cfg.tag = 60 + ii;
      bridge_port_cfg.flow_agg = the_system->PSN2TDM_default_agg;
      bridge_port_cfg.max_mac_addresses = 10;
      bridge_port_cfg.input_filters_mask = 0;
      bridge_port_cfg.classification_flag =
         WP_IW_IWPORT_CLASSIFICATION_DISABLED;
      the_system->PSN2TDM_bridge_port[ii] =
         WP_IwBridgingPortCreate (the_system->bridge_system_PSN2TDM,
                                  &bridge_port_cfg);
      WPE_TerminateOnError (the_system->PSN2TDM_bridge_port[ii],
                            "WP_IwBridgingPortCreate() PSN2TDM");
   }
#endif

   /* Create flow aggregations that send the packet out on each TDM */
   /* NOTE THE 4 BYTE HDLC header *//* To test different scenarios you need to change the 4 byte header for now */
   /* ALSO NOTE the 18 bytes being extracted */

   for (ii = 0; ii < 1; ii++)
   {

      memset (&pwe3_hdlc_flow_agg_config, 0, sizeof (WP_iw_agg_pwe3));

      pwe3_hdlc_flow_agg_config.tag = 30 + ii;
      pwe3_hdlc_flow_agg_config.txfunc = the_system->ch_handle_iw[ii];  /* this parameter is set outside this function */
      pwe3_hdlc_flow_agg_config.rfcs = 0;
      pwe3_hdlc_flow_agg_config.intmode = WP_DISABLE;
      pwe3_hdlc_flow_agg_config.interruptqueue = WP_IRQT3;
      pwe3_hdlc_flow_agg_config.statmode = WP_ENABLE;
      pwe3_hdlc_flow_agg_config.timestamp_mode = WP_IW_TIME_STAMP_DISABLE;
      pwe3_hdlc_flow_agg_config.ov_pool_mode = WP_IW_OV_POOL_DISABLE;
      pwe3_hdlc_flow_agg_config.fbp_drop_threshold = 0;

      pwe3_hdlc_flow_agg_config.mtu = 1536;
      pwe3_hdlc_flow_agg_config.flow_agg_type = WP_IW_AGG_PRIMARY;
      pwe3_hdlc_flow_agg_config.encapsulation_mode = WP_PWE3_HDLC;
      pwe3_hdlc_flow_agg_config.direction = WP_PWE3_EGRESS; /* egress updated later */
      pwe3_hdlc_flow_agg_config.sn_mode = WP_ENABLE;
      pwe3_hdlc_flow_agg_config.control_word_mode = WP_ENABLE;
      pwe3_hdlc_flow_agg_config.header_replace_mode =
         WP_PWE3_REPLACE_NOTHING;
      pwe3_hdlc_flow_agg_config.header = 0;
      pwe3_hdlc_flow_agg_config.extraction_length = 0;
      pwe3_hdlc_flow_agg_config.switchover_threshold = 10;
      pwe3_hdlc_flow_agg_config.l3_length_update = WP_DISABLE;
      pwe3_hdlc_flow_agg_config.l3_header_offset = 0;

      the_system->h_flow_agg_PSN2TDM_Pwe3[ii] =
         WP_IwFlowAggregationCreate (WP_WINPATH (0), WP_IW_PWE3_MODE,
                                     &pwe3_hdlc_flow_agg_config);
      WPE_TerminateOnError (the_system->h_flow_agg_PSN2TDM_Pwe3[ii],
                            " WP_IwFlowAggregationCreate() CC");

      iw_agg_directmap_cfg.iw_port = 0;
      iw_agg_directmap_cfg.prefix_length = 0;
      iw_agg_directmap_cfg.l2_header_insert_mode =
         WP_IW_L2H_INSERT_DISABLE;
      iw_agg_directmap_cfg.intmode = WP_IW_INT_ENABLE;

      iw_agg_directmap_cfg.tag = 96 + ii;
      iw_agg_directmap_cfg.txfunc = the_system->pqblock1;   //the_system->h_flow_agg_PSN2TDM_Pwe3[ii];
      iw_agg_directmap_cfg.extraction_length = 22;

      the_system->h_flow_agg_PSN2TDM_Exact[ii] =
         WP_IwFlowAggregationCreate (WP_WINPATH (0), WP_IW_DIRECTMAP_MODE,
                                     &iw_agg_directmap_cfg);
      WPE_TerminateOnError (the_system->h_flow_agg_PSN2TDM_Exact[ii],
                            " WP_IwFlowAggregationCreate() TDM ");

      /* Add DFC flow */
      dfc_flow_info.filter_handle = the_system->PSN2TDM_bridge_filter;
      dfc_flow_info.input_port = the_system->PSN2TDM_bridge_port2TDM;   /* Note that Input_port is one of the fields in the filter */
      dfc_flow_info.output_port = 0;

      /* Add VLAN 10 ETHER TYPE = 0x800 */
      /* Note: Input Bridge port not passed in string but passed in above */
      sprintf (rule_string, "n;n;n;n;n;n;#;%d", 0xff03 /*18 */ );
      printf ("loop = %d\n", ii);
      flow_class_rule.classifier_string = rule_string;
      flow_class_rule.dfc_info = &dfc_flow_info;
      flow_class_rule.match_action = WP_FLOW_CLASS_MODE_EXT_ACTION;  //WP_FLOW_CLASS_MODE_FLOW_AGG;

      forwarding_action.flow_aggregation =
         the_system->h_flow_agg_PSN2TDM_Exact[ii];
      priority_action.priority_value = 0;
      priority_action.action_mode = WP_FLOW_PRIORITY_MAPPING;
      priority_action.field_type = 0;

      action_type[1] = WP_FLOW_CLASS_MODE_FLOW_AGG;
      action_type[0] = WP_FLOW_CLASS_MODE_PRIORITY;
      action.num_of_actions = 2;
      action.action_type = action_type;

      action_info[1].forwarding_action = forwarding_action;
      action_info[0].priority_action = priority_action;
      action.action_info = action_info;
      flow_class_rule.action_info.ext_action = action;
      //flow_class_rule.action_info.forwarding_action = forwarding_action;
      flow_class_rule.wred_entry = 0;

      the_system->PSN2TDM_bridge_dfcflow =
         WP_IwFlowAdd (the_system->bridge_system_PSN2TDM,
                       WP_FLOW_CLASS_RULE, &flow_class_rule);
      WPE_TerminateOnError (the_system->PSN2TDM_bridge_dfcflow,
                            " WP_IwFlowAdd() DFC Bridge PSN2TDM");

      sprintf (rule_string, "n;n;n;n;n;n;#;%d", 2);
      printf ("loop = %d\n", ii);
      flow_class_rule.classifier_string = rule_string;
      flow_class_rule.dfc_info = &dfc_flow_info;
      flow_class_rule.match_action = WP_FLOW_CLASS_MODE_EXT_ACTION;  //WP_FLOW_CLASS_MODE_FLOW_AGG;

      forwarding_action.flow_aggregation =
         the_system->h_flow_agg_PSN2TDM_Exact[ii];
      priority_action.priority_value = 1;
      priority_action.action_mode = WP_FLOW_PRIORITY_MAPPING;
      priority_action.field_type = 0;

      action_type[1] = WP_FLOW_CLASS_MODE_FLOW_AGG;
      action_type[0] = WP_FLOW_CLASS_MODE_PRIORITY;
      action.num_of_actions = 2;
      action.action_type = action_type;

      action_info[1].forwarding_action = forwarding_action;
      action_info[0].priority_action = priority_action;
      action.action_info = action_info;
      flow_class_rule.action_info.ext_action = action;
      //flow_class_rule.action_info.forwarding_action = forwarding_action;
      flow_class_rule.wred_entry = 0;

      the_system->PSN2TDM_bridge_dfcflow =
         WP_IwFlowAdd (the_system->bridge_system_PSN2TDM,
                       WP_FLOW_CLASS_RULE, &flow_class_rule);
      WPE_TerminateOnError (the_system->PSN2TDM_bridge_dfcflow,
                            " WP_IwFlowAdd() DFC Bridge PSN2TDM");

#if 1
      sprintf (rule_string, "n;n;n;n;n;n;#;%d", 3);
      printf ("loop = %d\n", ii);
      flow_class_rule.classifier_string = rule_string;
      flow_class_rule.dfc_info = &dfc_flow_info;
      flow_class_rule.match_action = WP_FLOW_CLASS_MODE_EXT_ACTION;  //WP_FLOW_CLASS_MODE_FLOW_AGG;

      forwarding_action.flow_aggregation =
         the_system->h_flow_agg_PSN2TDM_Exact[ii];
      priority_action.priority_value = 2;
      priority_action.action_mode = WP_FLOW_PRIORITY_MAPPING;
      priority_action.field_type = 0;

      action_type[1] = WP_FLOW_CLASS_MODE_FLOW_AGG;
      action_type[0] = WP_FLOW_CLASS_MODE_PRIORITY;
      action.num_of_actions = 2;
      action.action_type = action_type;

      action_info[1].forwarding_action = forwarding_action;
      action_info[0].priority_action = priority_action;
      action.action_info = action_info;
      flow_class_rule.action_info.ext_action = action;
      //flow_class_rule.action_info.forwarding_action = forwarding_action;
      flow_class_rule.wred_entry = 0;

      the_system->PSN2TDM_bridge_dfcflow =
         WP_IwFlowAdd (the_system->bridge_system_PSN2TDM,
                       WP_FLOW_CLASS_RULE, &flow_class_rule);
      WPE_TerminateOnError (the_system->PSN2TDM_bridge_dfcflow,
                            " WP_IwFlowAdd() DFC Bridge PSN2TDM");

      sprintf (rule_string, "n;n;n;n;n;n;#;%d", 4);
      printf ("loop = %d\n", ii);
      flow_class_rule.classifier_string = rule_string;
      flow_class_rule.dfc_info = &dfc_flow_info;
      flow_class_rule.match_action = WP_FLOW_CLASS_MODE_EXT_ACTION;  //WP_FLOW_CLASS_MODE_FLOW_AGG;

      forwarding_action.flow_aggregation =
         the_system->h_flow_agg_PSN2TDM_Exact[ii];
      priority_action.priority_value = 3;
      priority_action.action_mode = WP_FLOW_PRIORITY_MAPPING;
      priority_action.field_type = 0;

      action_type[1] = WP_FLOW_CLASS_MODE_FLOW_AGG;
      action_type[0] = WP_FLOW_CLASS_MODE_PRIORITY;
      action.num_of_actions = 2;
      action.action_type = action_type;

      action_info[1].forwarding_action = forwarding_action;
      action_info[0].priority_action = priority_action;
      action.action_info = action_info;
      flow_class_rule.action_info.ext_action = action;
      //flow_class_rule.action_info.forwarding_action = forwarding_action;
      flow_class_rule.wred_entry = 0;

      the_system->PSN2TDM_bridge_dfcflow =
         WP_IwFlowAdd (the_system->bridge_system_PSN2TDM,
                       WP_FLOW_CLASS_RULE, &flow_class_rule);
      WPE_TerminateOnError (the_system->PSN2TDM_bridge_dfcflow,
                            " WP_IwFlowAdd() DFC Bridge PSN2TDM");

      sprintf (rule_string, "n;n;n;n;n;n;#;%d", 5);
      printf ("loop = %d\n", ii);
      flow_class_rule.classifier_string = rule_string;
      flow_class_rule.dfc_info = &dfc_flow_info;
      flow_class_rule.match_action = WP_FLOW_CLASS_MODE_EXT_ACTION;  //WP_FLOW_CLASS_MODE_FLOW_AGG;

      forwarding_action.flow_aggregation =
         the_system->h_flow_agg_PSN2TDM_Exact[ii];
      priority_action.priority_value = 4;
      priority_action.action_mode = WP_FLOW_PRIORITY_MAPPING;
      priority_action.field_type = 0;

      action_type[1] = WP_FLOW_CLASS_MODE_FLOW_AGG;
      action_type[0] = WP_FLOW_CLASS_MODE_PRIORITY;
      action.num_of_actions = 2;
      action.action_type = action_type;

      action_info[1].forwarding_action = forwarding_action;
      action_info[0].priority_action = priority_action;
      action.action_info = action_info;
      flow_class_rule.action_info.ext_action = action;
      //flow_class_rule.action_info.forwarding_action = forwarding_action;
      flow_class_rule.wred_entry = 0;

      the_system->PSN2TDM_bridge_dfcflow =
         WP_IwFlowAdd (the_system->bridge_system_PSN2TDM,
                       WP_FLOW_CLASS_RULE, &flow_class_rule);
      WPE_TerminateOnError (the_system->PSN2TDM_bridge_dfcflow,
                            " WP_IwFlowAdd() DFC Bridge PSN2TDM");

      sprintf (rule_string, "n;n;n;n;n;n;#;%d", 6);
      printf ("loop = %d\n", ii);
      flow_class_rule.classifier_string = rule_string;
      flow_class_rule.dfc_info = &dfc_flow_info;
      flow_class_rule.match_action = WP_FLOW_CLASS_MODE_EXT_ACTION;  //WP_FLOW_CLASS_MODE_FLOW_AGG;

      forwarding_action.flow_aggregation =
         the_system->h_flow_agg_PSN2TDM_Exact[ii];
      priority_action.priority_value = 5;
      priority_action.action_mode = WP_FLOW_PRIORITY_MAPPING;
      priority_action.field_type = 0;

      action_type[1] = WP_FLOW_CLASS_MODE_FLOW_AGG;
      action_type[0] = WP_FLOW_CLASS_MODE_PRIORITY;
      action.num_of_actions = 2;
      action.action_type = action_type;

      action_info[1].forwarding_action = forwarding_action;
      action_info[0].priority_action = priority_action;
      action.action_info = action_info;
      flow_class_rule.action_info.ext_action = action;
      //flow_class_rule.action_info.forwarding_action = forwarding_action;
      flow_class_rule.wred_entry = 0;

      the_system->PSN2TDM_bridge_dfcflow =
         WP_IwFlowAdd (the_system->bridge_system_PSN2TDM,
                       WP_FLOW_CLASS_RULE, &flow_class_rule);
      WPE_TerminateOnError (the_system->PSN2TDM_bridge_dfcflow,
                            " WP_IwFlowAdd() DFC Bridge PSN2TDM");

      sprintf (rule_string, "n;n;n;n;n;n;#;%d", 7);
      printf ("loop = %d\n", ii);
      flow_class_rule.classifier_string = rule_string;
      flow_class_rule.dfc_info = &dfc_flow_info;
      flow_class_rule.match_action = WP_FLOW_CLASS_MODE_EXT_ACTION;  //WP_FLOW_CLASS_MODE_FLOW_AGG;

      forwarding_action.flow_aggregation =
         the_system->h_flow_agg_PSN2TDM_Exact[ii];
      priority_action.priority_value = 6;
      priority_action.action_mode = WP_FLOW_PRIORITY_MAPPING;
      priority_action.field_type = 0;

      action_type[1] = WP_FLOW_CLASS_MODE_FLOW_AGG;
      action_type[0] = WP_FLOW_CLASS_MODE_PRIORITY;
      action.num_of_actions = 2;
      action.action_type = action_type;

      action_info[1].forwarding_action = forwarding_action;
      action_info[0].priority_action = priority_action;
      action.action_info = action_info;
      flow_class_rule.action_info.ext_action = action;
      //flow_class_rule.action_info.forwarding_action = forwarding_action;
      flow_class_rule.wred_entry = 0;

      the_system->PSN2TDM_bridge_dfcflow =
         WP_IwFlowAdd (the_system->bridge_system_PSN2TDM,
                       WP_FLOW_CLASS_RULE, &flow_class_rule);
      WPE_TerminateOnError (the_system->PSN2TDM_bridge_dfcflow,
                            " WP_IwFlowAdd() DFC Bridge PSN2TDM");

      sprintf (rule_string, "n;n;n;n;n;n;#;%d", 8);
      printf ("loop = %d\n", ii);
      flow_class_rule.classifier_string = rule_string;
      flow_class_rule.dfc_info = &dfc_flow_info;
      flow_class_rule.match_action = WP_FLOW_CLASS_MODE_EXT_ACTION;  //WP_FLOW_CLASS_MODE_FLOW_AGG;

      forwarding_action.flow_aggregation =
         the_system->h_flow_agg_PSN2TDM_Exact[ii];
      priority_action.priority_value = 7;
      priority_action.action_mode = WP_FLOW_PRIORITY_MAPPING;
      priority_action.field_type = 0;

      action_type[1] = WP_FLOW_CLASS_MODE_FLOW_AGG;
      action_type[0] = WP_FLOW_CLASS_MODE_PRIORITY;
      action.num_of_actions = 2;
      action.action_type = action_type;

      action_info[1].forwarding_action = forwarding_action;
      action_info[0].priority_action = priority_action;
      action.action_info = action_info;
      flow_class_rule.action_info.ext_action = action;
      //flow_class_rule.action_info.forwarding_action = forwarding_action;
      flow_class_rule.wred_entry = 0;

      the_system->PSN2TDM_bridge_dfcflow =
         WP_IwFlowAdd (the_system->bridge_system_PSN2TDM,
                       WP_FLOW_CLASS_RULE, &flow_class_rule);
      WPE_TerminateOnError (the_system->PSN2TDM_bridge_dfcflow,
                            " WP_IwFlowAdd() DFC Bridge PSN2TDM");

      sprintf (rule_string, "n;n;n;n;n;n;#;%d", 9);
      printf ("loop = %d\n", ii);
      flow_class_rule.classifier_string = rule_string;
      flow_class_rule.dfc_info = &dfc_flow_info;
      flow_class_rule.match_action = WP_FLOW_CLASS_MODE_EXT_ACTION;  //WP_FLOW_CLASS_MODE_FLOW_AGG;

      forwarding_action.flow_aggregation =
         the_system->h_flow_agg_PSN2TDM_Exact[ii];
      priority_action.priority_value = 8;
      priority_action.action_mode = WP_FLOW_PRIORITY_MAPPING;
      priority_action.field_type = 0;

      action_type[1] = WP_FLOW_CLASS_MODE_FLOW_AGG;
      action_type[0] = WP_FLOW_CLASS_MODE_PRIORITY;
      action.num_of_actions = 2;
      action.action_type = action_type;

      action_info[1].forwarding_action = forwarding_action;
      action_info[0].priority_action = priority_action;
      action.action_info = action_info;
      flow_class_rule.action_info.ext_action = action;
      //flow_class_rule.action_info.forwarding_action = forwarding_action;
      flow_class_rule.wred_entry = 0;

      the_system->PSN2TDM_bridge_dfcflow =
         WP_IwFlowAdd (the_system->bridge_system_PSN2TDM,
                       WP_FLOW_CLASS_RULE, &flow_class_rule);
      WPE_TerminateOnError (the_system->PSN2TDM_bridge_dfcflow,
                            " WP_IwFlowAdd() DFC Bridge PSN2TDM");

      sprintf (rule_string, "n;n;n;n;n;n;#;%d", 10);
      printf ("loop = %d\n", ii);
      flow_class_rule.classifier_string = rule_string;
      flow_class_rule.dfc_info = &dfc_flow_info;
      flow_class_rule.match_action = WP_FLOW_CLASS_MODE_EXT_ACTION;  //WP_FLOW_CLASS_MODE_FLOW_AGG;

      forwarding_action.flow_aggregation =
         the_system->h_flow_agg_PSN2TDM_Exact[ii];
      priority_action.priority_value = 9;
      priority_action.action_mode = WP_FLOW_PRIORITY_MAPPING;
      priority_action.field_type = 0;

      action_type[1] = WP_FLOW_CLASS_MODE_FLOW_AGG;
      action_type[0] = WP_FLOW_CLASS_MODE_PRIORITY;
      action.num_of_actions = 2;
      action.action_type = action_type;

      action_info[1].forwarding_action = forwarding_action;
      action_info[0].priority_action = priority_action;
      action.action_info = action_info;
      flow_class_rule.action_info.ext_action = action;
      //flow_class_rule.action_info.forwarding_action = forwarding_action;
      flow_class_rule.wred_entry = 0;

      the_system->PSN2TDM_bridge_dfcflow =
         WP_IwFlowAdd (the_system->bridge_system_PSN2TDM,
                       WP_FLOW_CLASS_RULE, &flow_class_rule);
      WPE_TerminateOnError (the_system->PSN2TDM_bridge_dfcflow,
                            " WP_IwFlowAdd() DFC Bridge PSN2TDM");
#endif
    /*****************************************************************************
    * WP_ControlRegister - Attaches a callback function to assist in debugging
    *                      for WDDI log messages
    ****************************************************************************/
      status =
         WP_ControlRegister (WP_DEBUG_CALLBACK_WDDI_LOG,
                             &WP_MessageCallback);
      WPE_TerminateOnError (status, "WP_Control Register()");

      status = WP_IwSystemBuild (the_system->bridge_system_PSN2TDM);
      WPE_TerminateOnError (status, " WP_IwSystemBuild() Bridge PSN2TDM");
   }

}

static void WPE_SystemEnable (WPE_system * the_system)
{
   WP_status status;

#ifndef WP_HW_WINPATH1
   printf ("______ Init COMET ___________\n");

   status = WPU_TdmCometCardInit (WP_PORT_TDM1, WPU_INIT_COMET_CPLD_GEN);
   WPE_TerminateOnError (status, "WPU_TdmCometCardInit()");
   status = WPU_TdmCometCardInit (WP_PORT_TDM1, WPU_MODE_TDM_GEN_MASTER);
   WPE_TerminateOnError (status, "WPU_TdmCometCardInit()");
   status = WPU_TdmCometCardInit (WP_PORT_TDM2, WPU_MODE_TDM_GEN_MASTER);
   WPE_TerminateOnError (status, "WPU_TdmCometCardInit()");
   status = WPU_TdmCometCardInit (WP_PORT_TDM3, WPU_MODE_TDM_GEN_MASTER);
   WPE_TerminateOnError (status, "WPU_TdmCometCardInit()");
   status = WPU_TdmCometCardInit (WP_PORT_TDM4, WPU_MODE_TDM_GEN_MASTER);
   WPE_TerminateOnError (status, "WPU_TdmCometCardInit()");
   status = WPU_TdmCometCardInit (WP_PORT_TDM5, WPU_MODE_TDM_GEN_MASTER);
   WPE_TerminateOnError (status, "WPU_TdmCometCardInit()");
   status = WPU_TdmCometCardInit (WP_PORT_TDM6, WPU_MODE_TDM_GEN_MASTER);
   WPE_TerminateOnError (status, "WPU_TdmCometCardInit()");
   status = WPU_TdmCometCardInit (WP_PORT_TDM7, WPU_MODE_TDM_GEN_MASTER);
   WPE_TerminateOnError (status, "WPU_TdmCometCardInit()");
   status = WPU_TdmCometCardInit (WP_PORT_TDM8, WPU_MODE_TDM_GEN_MASTER);
   WPE_TerminateOnError (status, "WPU_TdmCometCardInit()");
#else
   /* intialize QFALC for TDM1 E1 */
   WPU_TdmQfalcCardInit (WPE_PORT_TDM, WPU_MODE_TDM_CLOCK_8MHZ);
#if WPE_TDM_MODE == WPE_TDM_E1
   WPU_TdmQfalcCardInit (WPE_PORT_TDM, WPU_MODE_TDM_TRANS_E1_SLAVE);
#else
   WPU_TdmQfalcCardInit (WPE_PORT_TDM, WPU_MODE_TDM_TRANS_T1_SLAVE);
#endif
#endif

   /* Enable the Packet Scheduling Unit */
   status = WP_SysPsuEnable (the_system->wpid);
   WPE_TerminateOnError (status, "WP_SysPsuEnable()");

   /* In the case of using TDM ports both the Port and Device need to be
      enabled */
   status = WP_PortEnable (the_system->port_handle, WP_DIRECTION_DUPLEX);
   WPE_TerminateOnError (status, "WP_PortEnable() TDM1");

   status =
      WP_DeviceEnable (the_system->trans_dev_handle, WP_DIRECTION_DUPLEX);
   WPE_TerminateOnError (status, "WP_DeviceEnable() (TDM_TRANS)");
   WP_Delay (1000);
}

static void WPE_McDevicesEnable (WPE_system * the_system)
{
   WP_status status;

   /* Enable MC_HDLC virtual device */
   status =
      WP_DeviceEnable (the_system->mc_hdlc_dev_handle,
                       WP_DIRECTION_DUPLEX);
   WPE_TerminateOnError (status, "WP_DeviceEnable() (MC_HDLC)");
   WP_Delay (1000);
}

#if 1
static void WPE_McDevicesDelete (WPE_system * the_system)
{
   WP_status status;

   /* Disable MC_HDLC virtual device */
   status =
      WP_DeviceDisable (the_system->mc_hdlc_dev_handle,
                        WP_DIRECTION_DUPLEX);
   WPE_TerminateOnError (status, "WP_DeviceDisable() (MC_HDLC)");
   /* Delete MC_HDLC virtual device */
   status = WP_DeviceDelete (the_system->mc_hdlc_dev_handle);
   WPE_TerminateOnError (status, "WP_DeviceDelete() (MC_HDLC)");
}
#endif
static void WPE_ChannelsEnable (WPE_system * the_system)
{
   WP_status status;
   WP_U32 i;

   /* Enable HDLC Tx channels. No need to enable HDLC Rx channels */
   status = WP_ChannelEnable (the_system->ch_handle[WPE_TX_CH_TAG]);
   WPE_TerminateOnError (status, "WP_ChannelEnable() HDLC Tx");

   /* Enable HDLC Tx channels. No need to enable HDLC Rx channels */
   for (i = 0; i < N_TX_HDLC_CH; i++)
   {
      status = WP_ChannelEnable (the_system->ch_handle_iw[i]);
      WPE_TerminateOnError (status, "WP_ChannelEnable() HDLC Tx");
   }
}

#if 1
static void WPE_ChannelsDelete (WPE_system * the_system)
{
   WP_status status;
   WP_U32 i, timeout;

   /* Disable Tx HDLC channel. Can't disable Rx HDLC channel */
   status = WP_ChannelDisable (the_system->ch_handle[WPE_TX_CH_TAG]);
   WPE_TerminateOnError (status, "WP_ChannelDisable() HDLC Tx");

   /* Delete HDLC channels */
   for (i = 0; i < N_CHANNELS; i++)
   {
      timeout = 0;
      do
      {
         status = WP_ChannelDelete (the_system->ch_handle[i]);
         if (++timeout > 1000)
            break;
      }
      while (WP_ERROR (status) == WP_ERR_CH_NOT_DISABLED);
      WPE_TerminateOnError (status, "WP_ChannelDelete()");
   }

   for (i = 0; i < N_TX_HDLC_CH; i++)
   {
      status = WP_ChannelDisable (the_system->ch_handle_iw[i]);
      WPE_TerminateOnError (status, "WP_ChannelDisable() HDLC Tx ");
      do
      {
         status = WP_ChannelDelete (the_system->ch_handle_iw[i]);
         if (++timeout > 1000)
            break;
      }
      while (WP_ERROR (status) == WP_ERR_CH_NOT_DISABLED);
   }
}
#endif
static void WPE_DataUnitsSetup (WPE_system * the_system)
{
   WP_U32 i;

#if WPE_DEBUG_LEVEL > 1
   WP_U32 j;
#endif
   WP_data_unit *data_unit;
   WP_data_segment *segment;

   /* Set up Tx data units */
   for (i = 0; i < N_FRAMES; i++)
   {
      data_unit = &(the_system->tx_data_unit[i]);
      segment = &(the_system->tx_segment[i]);

      segment->data_size = frame_config[i].length;
      segment->pool_handle = the_system->data_pool_handle;
      segment->data = WP_PoolAlloc (segment->pool_handle);
      segment->displacement = 0;
      segment->next = NULL;
      /* set HDLC header frist. */
      *(WP_U16 *) segment->data = HDLC_ADDRESS;
      segment->data[2] = 0xc0;
      segment->data[3] = 0x21;
      memset (segment->data + 4, frame_config[i].data_char,
              segment->data_size - 3);
#if WPE_DEBUG_LEVEL > 1
      printf ("Frame %d to be send (%d bytes):\n", i, segment->data_size);
      for (j = 0; j < segment->data_size; j++)
      {
         printf ("%2.2x", segment->data[j]);
         if ((j + 1) % 32 == 0)
            printf ("\n");
      }
      printf ("\n");
#endif

      data_unit->segment = segment;
      data_unit->n_segments = 1;
      data_unit->type = WP_DATA_PPP_HDLC;
      data_unit->data_size = segment->data_size;
      data_unit->n_active = 1;  /* active segments */
      data_unit->control = WP_HT_CONTROL (0);   /* nothing required */
   }

   /* Set up Rx data units */
   data_unit = &(the_system->rx_data_unit);
   segment = &(the_system->rx_segment);

   segment->data_size = 0;
   segment->pool_handle = the_system->data_pool_handle;
   segment->data = 0;
   segment->displacement = 0;
   segment->next = NULL;

   data_unit->segment = segment;
   data_unit->n_segments = 1;
   data_unit->type = WP_DATA_PPP_HDLC;
   data_unit->data_size = segment->data_size;
   data_unit->n_active = 1;     /* active segments */
   data_unit->control = WP_HT_CONTROL (0);   /* nothing required */
}

static void WPE_TerminateOnError (WP_handle handle, WP_CHAR * s)
{
   if (WP_ERROR_P (handle))
   {
      printf ("Test Abort %s %s 0x%x\n", s,
              WP_error_name[WP_ERROR (handle)], handle);
      printf ("Test Failed\n");
      WPE_Terminate (1);
   }
   else
   {
#if WPE_DEBUG_LEVEL > 1
      printf ("Handle/Status returned from %s is %#8.8x \n", s, handle);
#endif
   }
}

static void WPE_Terminate (WP_U32 status)
{
   /* Release WDDI */
   WP_DriverRelease ();

   /* For socket based simulators */
#if USE_SOCKET
   CleanUpSocket (5);
#endif

   if (status)
   {
//      WT_Reboot();
      exit (status);
   }
}

static void WPE_PrintStatistics (WPE_system * the_system)
{
   WP_stats_tdi_hdlc tdi_hdlc[1];
   WP_status status;
   int ii;
   WP_iw_flow_stats flow_stats = { 0 };
   WP_bport_stats bport_stats = { 0 };
   WP_iw_pwe3_stats flow_stats_pwe3 = { 0 };
   WP_stats_enet s_hs_enet = { 0 };

   memset (tdi_hdlc, 0, sizeof (WP_stats_tdi_hdlc));

   /* Get Frame Relay POS device statistics */
   status = WP_DeviceStatistics (the_system->mc_hdlc_dev_handle, tdi_hdlc);
   WPE_TerminateOnError (status, "WP_DeviceStatistics");
#if WPE_DEBUG_LEVEL > 0
   /* print device statistics */
   printf ("HDLC TDI DEVICE STATISTICS:\n");
   printf ("----------------------------\n");
   printf ("Received frames = %#8.8x%8.8x\n",
           (WP_U32) (tdi_hdlc->rx_frames >> 32),
           (WP_U32) (tdi_hdlc->rx_frames & 0xffff));
   printf ("Received valid layer 3 IW frames = %#8.8x%8.8x\n",
           (WP_U32) (tdi_hdlc->rx_iw_frames >> 32),
           (WP_U32) (tdi_hdlc->rx_iw_frames & 0xffff));
   printf ("Transmitted frames = %#8.8x%8.8x\n",
           (WP_U32) (tdi_hdlc->tx_frames >> 32),
           (WP_U32) (tdi_hdlc->tx_frames & 0xffff));
   printf ("\n\n");
#endif

   if (tdi_hdlc->rx_frames != N_FRAMES || tdi_hdlc->tx_frames != N_FRAMES)
   {
      printf
         ("rx_frames = %#8.8x%8.8x tx frames  = %#8.8x%8.8x expected = %d\n",
          (WP_U32) (tdi_hdlc->rx_frames >> 32),
          (WP_U32) (tdi_hdlc->rx_frames & 0xffff),
          (WP_U32) (tdi_hdlc->tx_frames >> 32),
          (WP_U32) (tdi_hdlc->tx_frames & 0xffff), N_FRAMES);
      test_failed++;
   }

   if (tdi_hdlc->rx_err_crc)
   {
      printf ("Received frames with CRC error = %#8.8x%8.8x\n",
              (WP_U32) (tdi_hdlc->rx_err_crc >> 32),
              (WP_U32) (tdi_hdlc->rx_err_crc & 0xffff));
      test_failed++;
   }

   if (tdi_hdlc->rx_err_abort)
   {
      printf ("Received frames with ABORT error = %#8.8x%8.8x\n",
              (WP_U32) (tdi_hdlc->rx_err_abort >> 32),
              (WP_U32) (tdi_hdlc->rx_err_abort & 0xffff));
      test_failed++;
   }

   if (tdi_hdlc->rx_err_addr_mismatch)
   {
      printf ("Discarded frames with address mismatch = %#8.8x%8.8x\n",
              (WP_U32) (tdi_hdlc->rx_err_addr_mismatch >> 32),
              (WP_U32) (tdi_hdlc->rx_err_addr_mismatch & 0xffff));
      test_failed++;
   }

   if (tdi_hdlc->rx_err_buffer_overrun)
   {
      printf ("Discarded frames due to buffer overrun = %#8.8x%8.8x\n",
              (WP_U32) (tdi_hdlc->rx_err_buffer_overrun >> 32),
              (WP_U32) (tdi_hdlc->rx_err_buffer_overrun & 0xffff));
      test_failed++;
   }

   if (tdi_hdlc->rx_err_maxsdu)
   {
      printf ("Discarded frames due to max SDU exceeded = %#8.8x%8.8x\n",
              (WP_U32) (tdi_hdlc->rx_err_maxsdu >> 32),
              (WP_U32) (tdi_hdlc->rx_err_maxsdu & 0xffff));
      test_failed++;
   }

   if (tdi_hdlc->rx_err_iw_buffer_underrun)
   {
      printf ("Discarded frames due to IW buffer overrun = %#8.8x%8.8x\n",
              (WP_U32) (tdi_hdlc->rx_err_iw_buffer_underrun >> 32),
              (WP_U32) (tdi_hdlc->rx_err_iw_buffer_underrun & 0xffff));
      test_failed++;
   }

   if (tdi_hdlc->rx_err_iw_mru)
   {
      printf ("Discarded frames due to IW MRU errors = %#8.8x%8.8x\n",
              (WP_U32) (tdi_hdlc->rx_err_iw_mru >> 32),
              (WP_U32) (tdi_hdlc->rx_err_iw_mru & 0xffff));
      test_failed++;
   }

   if (tdi_hdlc->rx_err_overrun)
   {
      printf ("Discarded frames due to TDI overrun = %#8.8x%8.8x\n",
              (WP_U32) (tdi_hdlc->rx_err_overrun >> 32),
              (WP_U32) (tdi_hdlc->rx_err_overrun & 0xffff));
      test_failed++;
   }

   if (tdi_hdlc->tx_err_underrun)
   {
      printf ("Transmit TDI underruns = %#8.8x%8.8x\n",
              (WP_U32) (tdi_hdlc->tx_err_underrun >> 32),
              (WP_U32) (tdi_hdlc->tx_err_underrun & 0xffff));
      test_failed++;
   }

   for (ii = 0; ii < 1 /*APP_MAX_TDM_PORTS */ ; ii++)
   {
      printf ("-------------------------------\n");
      printf (" Flow statistics PPPSW %d\n", ii);
      printf ("-------------------------------\n");
      memset (&flow_stats, 0, sizeof (flow_stats));
      status =
         WP_IwFlowStatistics (the_system->h_flow_agg_pppsw_link[ii],
                              WP_IW_FLOW_STAT, &flow_stats);
      WPE_TerminateOnError (status, "WP_IwFlowStatistics");

      printf ("forward_packet = %d\n", (WP_U32) flow_stats.forward_packet);
      printf ("fbp_drop_packets = %d\n",
              (WP_U32) flow_stats.fbp_drop_packets);
      printf ("mtu_drop_packets = %d\n",
              (WP_U32) flow_stats.mtu_drop_packets);
      printf ("ttl_drop_packets = %d\n",
              (WP_U32) flow_stats.ttl_drop_packets);
      printf ("tx_queue_drop_packets = %d\n",
              (WP_U32) flow_stats.tx_queue_drop_packets);
      printf ("mpls_drop = %d\n", (WP_U32) flow_stats.mpls_drop);
      printf ("denied_packets = %d\n", (WP_U32) flow_stats.denied_packets);
      printf ("group_filtered_packets = %d\n",
              (WP_U32) flow_stats.group_filtered_packets);
      printf ("forwarded_bytes = %d\n",
              (WP_U32) flow_stats.forwarded_bytes);
      printf ("gtp_bad_headers = %d\n",
              (WP_U32) flow_stats.gtp_bad_headers);
      printf ("policer_non_conforming_packets= %d\n",
              (WP_U32) flow_stats.policer_non_conforming_packets);

      printf ("-------------------------------\n");
      printf ("Bridge Port Statistics TDM %d\n", ii);
      printf ("-------------------------------\n");
      memset (&bport_stats, 0, sizeof (bport_stats));
      status =
         WP_IwPortStatistics (the_system->TDM2PSN_bridge_port[ii],
                              &bport_stats);
      WPE_TerminateOnError (status, "BportStatistics");

      printf ("rx_valid_packets = %d\n",
              (WP_U32) bport_stats.rx_valid_packets);
      printf ("discard_vlan_acceptable_filter = %d\n",
              (WP_U32) bport_stats.discard_vlan_acceptable_filter);
      printf ("discard_ingress_filter = %d\n",
              (WP_U32) bport_stats.discard_ingress_filter);
      printf ("discard_bridge_classifier = %d\n",
              (WP_U32) bport_stats.discard_bridge_classifier);
      printf ("discard_unk_macsa = %d\n",
              (WP_U32) bport_stats.discard_unk_macsa);
      printf ("deny_mac_sa = %d\n", (WP_U32) bport_stats.deny_mac_sa);
      printf ("deny_mac_da = %d\n", (WP_U32) bport_stats.deny_mac_da);
      printf ("rx_bc_valid_packets = %d\n",
              (WP_U32) bport_stats.rx_bc_valid_packets);
      printf ("rx_mc_valid_packets = %d\n",
              (WP_U32) bport_stats.rx_mc_valid_packets);
      printf ("forwarded_uc_packets = %d\n",
              (WP_U32) bport_stats.forwarded_uc_packets);
      printf ("forwarded_bc_packets = %d\n",
              (WP_U32) bport_stats.forwarded_bc_packets);
      printf ("forwarded_mc_packets = %d\n",
              (WP_U32) bport_stats.forwarded_mc_packets);
      printf ("protocol_error = %d\n",
              (WP_U32) bport_stats.protocol_error);

   }

   printf ("-------------------------------\n");
   printf (" Flow statistics TDM2PSN_extract_fa[0]\n");
   printf ("-------------------------------\n");
   memset (&flow_stats, 0, sizeof (flow_stats));
   status =
      WP_IwFlowStatistics (the_system->TDM2PSN_extract_fa[0],
                           WP_IW_FLOW_STAT, &flow_stats);
   WPE_TerminateOnError (status, "WP_IwFlowStatistics");

   printf ("forward_packet = %d\n", (WP_U32) flow_stats.forward_packet);
   printf ("fbp_drop_packets = %d\n",
           (WP_U32) flow_stats.fbp_drop_packets);
   printf ("mtu_drop_packets = %d\n",
           (WP_U32) flow_stats.mtu_drop_packets);
   printf ("ttl_drop_packets = %d\n",
           (WP_U32) flow_stats.ttl_drop_packets);
   printf ("tx_queue_drop_packets = %d\n",
           (WP_U32) flow_stats.tx_queue_drop_packets);
   printf ("mpls_drop = %d\n", (WP_U32) flow_stats.mpls_drop);
   printf ("denied_packets = %d\n", (WP_U32) flow_stats.denied_packets);
   printf ("group_filtered_packets = %d\n",
           (WP_U32) flow_stats.group_filtered_packets);
   printf ("forwarded_bytes = %d\n", (WP_U32) flow_stats.forwarded_bytes);
   printf ("gtp_bad_headers = %d\n", (WP_U32) flow_stats.gtp_bad_headers);
   printf ("policer_non_conforming_packets= %d\n",
           (WP_U32) flow_stats.policer_non_conforming_packets);

   printf ("-------------------------------\n");
   printf (" Flow statistics Bridge FA-->ENET(LCP)\n");
   printf ("-------------------------------\n");
   memset (&flow_stats_pwe3, 0, sizeof (flow_stats_pwe3));
   status =
      WP_IwFlowStatistics (the_system->h_flow_agg_lcp, WP_IW_FLOW_STAT,
                           &flow_stats_pwe3);
   WPE_TerminateOnError (status, "WP_IwFlowStatistics");

   printf ("forward_packet = %d\n",
           (WP_U32) flow_stats_pwe3.forwarded_packets);
   printf ("fbp_drop_packets = %d\n",
           (WP_U32) flow_stats_pwe3.fbp_drop_packets);
   printf ("mtu_drop_packets = %d\n",
           (WP_U32) flow_stats_pwe3.mtu_drop_packets);
   printf ("ttl_drop_packets = %d\n",
           (WP_U32) flow_stats_pwe3.ttl_drop_packets);
   printf ("tx_queue_drop_packets = %d\n",
           (WP_U32) flow_stats_pwe3.tx_queue_drop_packets);
   printf ("mpls_drop = %d\n", (WP_U32) flow_stats_pwe3.mpls_drop_packets);
   printf ("denied_packets = %d\n",
           (WP_U32) flow_stats_pwe3.denied_packets);
   printf ("out_of_seq_packets = %d\n",
           (WP_U32) flow_stats_pwe3.out_of_seq_packets);
   printf ("forwarded_bytes = %d\n",
           (WP_U32) flow_stats_pwe3.forwarded_bytes);
   printf ("non_zero_sn_packets = %d\n",
           (WP_U32) flow_stats_pwe3.non_zero_sn_packets);
   printf ("policer_non_conforming_packets= %d\n",
           (WP_U32) flow_stats_pwe3.policer_non_conforming_packets);
   printf ("switchover_events = %d\n",
           (WP_U32) flow_stats_pwe3.switchover_events);
   printf ("hash_fail_cells= %d\n",
           (WP_U32) flow_stats_pwe3.hash_fail_cells);

   printf ("-------------------------------\n");
   printf (" ENET  statisics \n");
   printf ("-------------------------------\n");
   status = WP_DeviceStatistics (the_system->Enet_dev, &s_hs_enet);
   WPE_TerminateOnError (status, "WP_DeviceStatistics");
   printf ("   rx_bytes            : %u\n", (WP_U32) s_hs_enet.rx_bytes);
   printf ("   rx_packets          : %u\n", (WP_U32) s_hs_enet.rx_packets);
   printf ("   rx_err_fcs          : %u\n", (WP_U32) s_hs_enet.rx_err_fcs);
   printf ("   rx_multicast        : %u\n",
           (WP_U32) s_hs_enet.rx_multicast);
   printf ("   rx_broadcast        : %u\n",
           (WP_U32) s_hs_enet.rx_broadcast);
   printf ("   rx_dropped          : %u\n\n",
           (WP_U32) s_hs_enet.rx_dropped);
   printf ("   tx_bytes            : %u\n", (WP_U32) s_hs_enet.tx_bytes);
   printf ("   tx_packets          : %u\n", (WP_U32) s_hs_enet.tx_packets);
   printf ("   tx_err_fcs          : %u\n", (WP_U32) s_hs_enet.tx_err_fcs);
   printf ("   tx_multicast        : %u\n",
           (WP_U32) s_hs_enet.tx_multicast);
   printf ("   tx_broadcast        : %u\n",
           (WP_U32) s_hs_enet.tx_broadcast);
   printf ("   tx_dropped          : %u\n\n",
           (WP_U32) s_hs_enet.tx_dropped);
   printf ("   rx_err_overrun      : %u\n",
           (WP_U32) s_hs_enet.rx_err_overrun);
   printf ("   rx_err_mru          : %u\n", (WP_U32) s_hs_enet.rx_err_mru);
   printf ("   rx_iw_frames        : %u\n",
           (WP_U32) s_hs_enet.rx_iw_frames);
   printf ("   rx_host_frames      : %u\n",
           (WP_U32) s_hs_enet.rx_host_frames);
   printf ("   tx_frames           : %u\n", (WP_U32) s_hs_enet.tx_frames);

   printf ("-------------------------------\n");
   printf ("Bridge Port Statistics PSN \n");
   printf ("-------------------------------\n");
   memset (&bport_stats, 0, sizeof (bport_stats));
   status =
      WP_IwPortStatistics (the_system->PSN2TDM_bridge_port2TDM,
                           &bport_stats);
   WPE_TerminateOnError (status, "BportStatistics");

   printf ("rx_valid_packets = %d\n",
           (WP_U32) bport_stats.rx_valid_packets);
   printf ("discard_vlan_acceptable_filter = %d\n",
           (WP_U32) bport_stats.discard_vlan_acceptable_filter);
   printf ("discard_ingress_filter = %d\n",
           (WP_U32) bport_stats.discard_ingress_filter);
   printf ("discard_bridge_classifier = %d\n",
           (WP_U32) bport_stats.discard_bridge_classifier);
   printf ("discard_unk_macsa = %d\n",
           (WP_U32) bport_stats.discard_unk_macsa);
   printf ("deny_mac_sa = %d\n", (WP_U32) bport_stats.deny_mac_sa);
   printf ("deny_mac_da = %d\n", (WP_U32) bport_stats.deny_mac_da);
   printf ("rx_bc_valid_packets = %d\n",
           (WP_U32) bport_stats.rx_bc_valid_packets);
   printf ("rx_mc_valid_packets = %d\n",
           (WP_U32) bport_stats.rx_mc_valid_packets);
   printf ("forwarded_uc_packets = %d\n",
           (WP_U32) bport_stats.forwarded_uc_packets);
   printf ("forwarded_bc_packets = %d\n",
           (WP_U32) bport_stats.forwarded_bc_packets);
   printf ("forwarded_mc_packets = %d\n",
           (WP_U32) bport_stats.forwarded_mc_packets);
   printf ("protocol_error = %d\n", (WP_U32) bport_stats.protocol_error);

   printf ("-------------------------------\n");
   printf (" Flow statistics h_flow_agg_PSN2TDM_Exact[0]\n");
   printf ("-------------------------------\n");
   memset (&flow_stats, 0, sizeof (flow_stats));
   status =
      WP_IwFlowStatistics (the_system->h_flow_agg_PSN2TDM_Exact[0],
                           WP_IW_FLOW_STAT, &flow_stats);
   WPE_TerminateOnError (status, "WP_IwFlowStatistics");

   printf ("forward_packet = %d\n", (WP_U32) flow_stats.forward_packet);
   printf ("fbp_drop_packets = %d\n",
           (WP_U32) flow_stats.fbp_drop_packets);
   printf ("mtu_drop_packets = %d\n",
           (WP_U32) flow_stats.mtu_drop_packets);
   printf ("ttl_drop_packets = %d\n",
           (WP_U32) flow_stats.ttl_drop_packets);
   printf ("tx_queue_drop_packets = %d\n",
           (WP_U32) flow_stats.tx_queue_drop_packets);
   printf ("mpls_drop = %d\n", (WP_U32) flow_stats.mpls_drop);
   printf ("denied_packets = %d\n", (WP_U32) flow_stats.denied_packets);
   printf ("group_filtered_packets = %d\n",
           (WP_U32) flow_stats.group_filtered_packets);
   printf ("forwarded_bytes = %d\n", (WP_U32) flow_stats.forwarded_bytes);
   printf ("gtp_bad_headers = %d\n", (WP_U32) flow_stats.gtp_bad_headers);
   printf ("policer_non_conforming_packets= %d\n",
           (WP_U32) flow_stats.policer_non_conforming_packets);

#if 1
   printf ("-------------------------------\n");
   printf (" Flow statistics Bridge FA-->PSN2TDM\n");
   printf ("-------------------------------\n");
   memset (&flow_stats_pwe3, 0, sizeof (flow_stats_pwe3));
   status =
      WP_IwFlowStatistics (the_system->h_flow_agg_PSN2TDM_Pwe3[0],
                           WP_IW_FLOW_STAT, &flow_stats_pwe3);
   WPE_TerminateOnError (status, "WP_IwFlowStatistics");

   printf ("forward_packet = %d\n",
           (WP_U32) flow_stats_pwe3.forwarded_packets);
   printf ("fbp_drop_packets = %d\n",
           (WP_U32) flow_stats_pwe3.fbp_drop_packets);
   printf ("mtu_drop_packets = %d\n",
           (WP_U32) flow_stats_pwe3.mtu_drop_packets);
   printf ("ttl_drop_packets = %d\n",
           (WP_U32) flow_stats_pwe3.ttl_drop_packets);
   printf ("tx_queue_drop_packets = %d\n",
           (WP_U32) flow_stats_pwe3.tx_queue_drop_packets);
   printf ("mpls_drop = %d\n", (WP_U32) flow_stats_pwe3.mpls_drop_packets);
   printf ("denied_packets = %d\n",
           (WP_U32) flow_stats_pwe3.denied_packets);
   printf ("out_of_seq_packets = %d\n",
           (WP_U32) flow_stats_pwe3.out_of_seq_packets);
   printf ("forwarded_bytes = %d\n",
           (WP_U32) flow_stats_pwe3.forwarded_bytes);
   printf ("non_zero_sn_packets = %d\n",
           (WP_U32) flow_stats_pwe3.non_zero_sn_packets);
   printf ("policer_non_conforming_packets= %d\n",
           (WP_U32) flow_stats_pwe3.policer_non_conforming_packets);
   printf ("switchover_events = %d\n",
           (WP_U32) flow_stats_pwe3.switchover_events);
   printf ("hash_fail_cells= %d\n",
           (WP_U32) flow_stats_pwe3.hash_fail_cells);

   printf ("-------------------------------\n");
   printf (" Flow statistics PSN2TDM_default_agg[0]\n");
   printf ("-------------------------------\n");
   memset (&flow_stats, 0, sizeof (flow_stats));
   status =
      WP_IwFlowStatistics (the_system->PSN2TDM_default_agg,
                           WP_IW_FLOW_STAT, &flow_stats);
   WPE_TerminateOnError (status, "WP_IwFlowStatistics");

   printf ("forward_packet = %d\n", (WP_U32) flow_stats.forward_packet);
   printf ("fbp_drop_packets = %d\n",
           (WP_U32) flow_stats.fbp_drop_packets);
   printf ("mtu_drop_packets = %d\n",
           (WP_U32) flow_stats.mtu_drop_packets);
   printf ("ttl_drop_packets = %d\n",
           (WP_U32) flow_stats.ttl_drop_packets);
   printf ("tx_queue_drop_packets = %d\n",
           (WP_U32) flow_stats.tx_queue_drop_packets);
   printf ("mpls_drop = %d\n", (WP_U32) flow_stats.mpls_drop);
   printf ("denied_packets = %d\n", (WP_U32) flow_stats.denied_packets);
   printf ("group_filtered_packets = %d\n",
           (WP_U32) flow_stats.group_filtered_packets);
   printf ("forwarded_bytes = %d\n", (WP_U32) flow_stats.forwarded_bytes);
   printf ("gtp_bad_headers = %d\n", (WP_U32) flow_stats.gtp_bad_headers);
   printf ("policer_non_conforming_packets= %d\n",
           (WP_U32) flow_stats.policer_non_conforming_packets);

   printf ("-------------------------------\n");
   printf (" Flow statistics TDM2PSN_default_agg[0]\n");
   printf ("-------------------------------\n");
   memset (&flow_stats, 0, sizeof (flow_stats));
   status =
      WP_IwFlowStatistics (the_system->TDM2PSN_default_agg,
                           WP_IW_FLOW_STAT, &flow_stats);
   WPE_TerminateOnError (status, "WP_IwFlowStatistics");

   printf ("forward_packet = %d\n", (WP_U32) flow_stats.forward_packet);
   printf ("fbp_drop_packets = %d\n",
           (WP_U32) flow_stats.fbp_drop_packets);
   printf ("mtu_drop_packets = %d\n",
           (WP_U32) flow_stats.mtu_drop_packets);
   printf ("ttl_drop_packets = %d\n",
           (WP_U32) flow_stats.ttl_drop_packets);
   printf ("tx_queue_drop_packets = %d\n",
           (WP_U32) flow_stats.tx_queue_drop_packets);
   printf ("mpls_drop = %d\n", (WP_U32) flow_stats.mpls_drop);
   printf ("denied_packets = %d\n", (WP_U32) flow_stats.denied_packets);
   printf ("group_filtered_packets = %d\n",
           (WP_U32) flow_stats.group_filtered_packets);
   printf ("forwarded_bytes = %d\n", (WP_U32) flow_stats.forwarded_bytes);
   printf ("gtp_bad_headers = %d\n", (WP_U32) flow_stats.gtp_bad_headers);
   printf ("policer_non_conforming_packets= %d\n",
           (WP_U32) flow_stats.policer_non_conforming_packets);

#endif
   /* Get Frame Relay POS device statistics */
   status = WP_DeviceStatistics (the_system->mc_hdlc_dev_handle, tdi_hdlc);
   WPE_TerminateOnError (status, "WP_DeviceStatistics");

#if WPE_DEBUG_LEVEL > 0
   /* print device statistics */
   printf ("HDLC TDI DEVICE STATISTICS:\n");
   printf ("----------------------------\n");
   printf ("Received frames = %#8.8x%8.8x\n",
           (WP_U32) (tdi_hdlc->rx_frames >> 32),
           (WP_U32) (tdi_hdlc->rx_frames & 0xffff));
   printf ("Transmitted frames = %#8.8x%8.8x\n",
           (WP_U32) (tdi_hdlc->tx_frames >> 32),
           (WP_U32) (tdi_hdlc->tx_frames & 0xffff));
   printf ("rx_err_crc   frames = %#8.8x%8.8x\n",
           (WP_U32) (tdi_hdlc->rx_err_crc >> 32),
           (WP_U32) (tdi_hdlc->rx_err_crc & 0xffff));
   printf ("rx_err_abort   frames = %#8.8x%8.8x\n",
           (WP_U32) (tdi_hdlc->rx_err_abort >> 32),
           (WP_U32) (tdi_hdlc->rx_err_abort & 0xffff));

   printf ("rx_err_addr_mismatch   frames = %#8.8x%8.8x\n",
           (WP_U32) (tdi_hdlc->rx_err_addr_mismatch >> 32),
           (WP_U32) (tdi_hdlc->rx_err_addr_mismatch & 0xffff));

   printf ("rx_err_buffer_overrun   frames = %#8.8x%8.8x\n",
           (WP_U32) (tdi_hdlc->rx_err_buffer_overrun >> 32),
           (WP_U32) (tdi_hdlc->rx_err_buffer_overrun & 0xffff));

   printf ("rx_err_overrun   frames = %#8.8x%8.8x\n",
           (WP_U32) (tdi_hdlc->rx_err_overrun >> 32),
           (WP_U32) (tdi_hdlc->rx_err_overrun & 0xffff));

   printf ("rx_err_iw_buffer_underrun   frames = %#8.8x%8.8x\n",
           (WP_U32) (tdi_hdlc->rx_err_iw_buffer_underrun >> 32),
           (WP_U32) (tdi_hdlc->rx_err_iw_buffer_underrun & 0xffff));

   printf ("rx_err_iw_mru   frames = %#8.8x%8.8x\n",
           (WP_U32) (tdi_hdlc->rx_err_iw_mru >> 32),
           (WP_U32) (tdi_hdlc->rx_err_iw_mru & 0xffff));

   printf ("tx_err_underrun   frames = %#8.8x%8.8x\n",
           (WP_U32) (tdi_hdlc->tx_err_underrun >> 32),
           (WP_U32) (tdi_hdlc->tx_err_underrun & 0xffff));

   printf ("rx_err_maxsdu   frames = %#8.8x%8.8x\n",
           (WP_U32) (tdi_hdlc->rx_err_maxsdu >> 32),
           (WP_U32) (tdi_hdlc->rx_err_maxsdu & 0xffff));

   printf ("Received valid layer 3 IW frames = %#8.8x%8.8x\n",
           (WP_U32) (tdi_hdlc->rx_iw_frames >> 32),
           (WP_U32) (tdi_hdlc->rx_iw_frames & 0xffff));

   printf ("rx_err_non_octet   frames = %#8.8x%8.8x\n",
           (WP_U32) (tdi_hdlc->rx_err_non_octet >> 32),
           (WP_U32) (tdi_hdlc->rx_err_non_octet & 0xffff));

   printf ("rx_bytes   frames = %#8.8x%8.8x\n",
           (WP_U32) (tdi_hdlc->rx_bytes >> 32),
           (WP_U32) (tdi_hdlc->rx_bytes & 0xffff));

   printf ("rx_err_bytes   frames = %#8.8x%8.8x\n",
           (WP_U32) (tdi_hdlc->rx_err_bytes >> 32),
           (WP_U32) (tdi_hdlc->rx_err_bytes & 0xffff));

   printf ("tx_bytes   frames = %#8.8x%8.8x\n",
           (WP_U32) (tdi_hdlc->tx_bytes >> 32),
           (WP_U32) (tdi_hdlc->tx_bytes & 0xffff));

   printf ("rx_err_iw_l2_parse   frames = %#8.8x%8.8x\n",
           (WP_U32) (tdi_hdlc->rx_err_iw_l2_parse >> 32),
           (WP_U32) (tdi_hdlc->rx_err_iw_l2_parse & 0xffff));

   printf ("\n\n");
#endif

}

/*****************************************************************************
 * NOTE:
 * This is where the shared routines gets included.  Ideally, this would
 * be done as a separately linked module.
 ****************************************************************************/
//#include "wt_util.c"
