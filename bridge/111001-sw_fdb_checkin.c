/*************************************************************
 * (C) Copyright 2001-2011, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ************************************************************/
#if _WT_MODULE_TEST_COMMENT_ENABLE_
/* This test has been converted to work on Linux.  See
   WDDI_TestCodingGuidlines.doc.  */
#pragma check_tests pool_alloc 1
#endif
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
#include "wt_util.h"
#include "wp_sim.h"

#include "wt_sw_fdb_misc.h"
#include "wpx_app_data.h"
#include "wpx_oc12_pmc5357.h"
#include "wpx_gei_phy.h"
#include "wpx_pin_mux_selector.h"
#include "app_convert.h"
#include "wt_partition_ecc_util.h"


#include "wpx_enet_phy.h"

#include "wpu_debug_sw.h"
#include "api/wp_pce.h"

extern WP_U32 dps_ProgramImage[];
extern WP_U16 dps_PC_Table_Init[];

WP_CHAR appname[] = "Test for SW FDB";

#define DEBUG 1
#define DEFAULT_WPID	(0)
#define USE_SHIFT_ENUM	(1)

enum filter_sets
{
   FILTER_SET_A = 0,
   FILTER_SET_B,
   FILTER_SET_C,
   FILTER_SET_D,
   FILTER_SET_E,
   FILTER_SET_COUNT,
};

   
enum filters
{
   FILTER_SET_B_CLASSIFICATION = 0,
   FILTER_SET_B_FORWARDING,
   FILTER_SET_B_LEARNING,
   FILTER_SET_A_CLASSIFICATION,
   FILTER_SET_A_FORWARDING,
   FILTER_SET_A_LEARNING,
   FILTER_SET_C_FORWARDING,
   FILTER_SET_C_LEARNING,
   FILTER_SET_D_FORWARDING,
   FILTER_SET_D_LEARNING,
   FILTER_SET_E_FORWARDING,
   FILTER_SET_E_LEARNING,
   FILTER_SET_LAST,
};

WP_CHAR *filters_string[] =
{
      "FILTER_SET_B_CLASSIFICATION",
      "FILTER_SET_B_FORWARDING",
      "FILTER_SET_B_LEARNING",
      "FILTER_SET_A_CLASSIFICATION",
      "FILTER_SET_A_FORWARDING",
      "FILTER_SET_A_LEARNING",
      "FILTER_SET_C_FORWARDING",
      "FILTER_SET_C_LEARNING",
      "FILTER_SET_D_FORWARDING",
      "FILTER_SET_D_LEARNING",
      "FILTER_SET_E_FORWARDING",
      "FILTER_SET_E_LEARNING",
};

enum rules
{
   RULES_A_CLASSIFICATION_START = 1,
   RULES_A_FORWARDING_START = 100,
   RULES_B_CLASSIFICATION_START  = 200,
   RULES_B_FORWARDING_START = 300,
   RULES_C_FORWARDING_START = 400,
   RULES_D_FORWARDING_START = 500,
   RULES_E_FORWARDING_START = 600,
};
   
enum flow_aggs
{
   FILTER_SET_B_CLASSIFICATION_FLOW_AGG = 0,
   FILTER_SET_B_FORWARDING_FLOW_AGG,
   FILTER_SET_C_FORWARDING_FLOW_AGG,
   FILTER_SET_D_FORWARDING_FLOW_AGG,
   FILTER_SET_E_FORWARDING_FLOW_AGG,
   FLOW_AGG_COUNT,
};

WP_CHAR *flow_agg_string[] =
{
      "FILTER_SET_B_CLASSIFICATION",
      "FILTER_SET_B_FORWARDING",
      "FILTER_SET_C_FORWARDING",
      "FILTER_SET_D_FORWARDING",
      "FILTER_SET_E_FORWARDING",
};

WP_U8  start_mac_addr[6] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x00};

#define NUM_OF_PCE_FILTERS (FILTER_SET_LAST)

#define LEARNING_QUEUE_SIZE  30
#define IW_SYS_MAX_BPORTS 30

#define WT_IPV4_HEADER_SIZE   20
#define WT_L4_HEADER_SIZE     8

#define WT_RX_LOOP_LIMIT 5000

#define MAX_IW_POLICERS 50
#define MAX_FLOW_AGG 60
#define POOL_LIMIT 20
#define MAX_IW_PROGRAMMABLE_FILTERS 5


#define MAX_NUM_OF_CHANNELS 1

#define NUM_OF_RULES_PER_FILTER_SET 12
#define NUM_OF_INPUT_PORT_PER_FILTER_SET 2
#define NUM_OF_FLOWS NUM_OF_RULES_PER_FILTER_SET

/* IWF <Mac SA High, Mac SA Low, Mac DA High, Mac DA Low, Cvlan Tag> */
static WP_U32 Traffic_Table[FILTER_SET_COUNT][5] ={
   { 0x00112233, 0x4400, 0x00112233, 0x4400, 0x100 },
   { 0x00112233, 0x4400, 0x00112233, 0x4400, 0x110 },
   { 0x00112233, 0x4400, 0x00112233, 0x4400, 0x120 },
   { 0x00112233, 0x4400, 0x00112233, 0x4400, 0x130 },
   { 0x00112233, 0x4400, 0x00112233, 0x4400, 0x140 },
};


enum 
{
   TEST_SW_FDB_ONLY = 0,
   TEST_HYBRID_FDB,
   TEST_HW_FDB_ONLY,
};

#if defined(WP_BOARD_WDS3)
#define BUFFER_BUS WP_BUS_PACKET
#define BUFFER_BANK APP_BANK_PACKET
#define ENET_PORT WP_PORT_ENET13
#elif defined(WP_BOARD_WDS3_SL)
#define BUFFER_BUS WP_BUS_PARAM
#define BUFFER_BANK APP_BANK_PARAM
#define ENET_PORT WP_PORT_ENET11
#endif

WP_U32 TestType = TEST_SW_FDB_ONLY;
WP_U32 Checked = WP_FALSE;

WP_U32 free_pce_hw_rules;
WP_U32 dummy_rules_count = 0;
WP_handle PCE_dummy_rule_handle[1000];

#define WT_MAX_FRAME_SIZE         (1536 + 2*512)

#define MAX_DATA_LENGTH    2500
#define APP_HOST_BUFFER_SIZE 1984

WP_pce_filter_stat filter_stat[100];
WP_pce_filter_stat previous_filter_stat[100];
WP_handle aging_iterator_handle = NULL;
WP_U32 aged_rules = 0;
WP_U32 iter_rules = 0;

typedef union StatField
{
      WP_U64 field;
      struct {
            WP_U32 high;
            WP_U32 low;
      } part;
} WPT_StatField;


WP_pce_parser_info pce_parser_info;

WP_sw_fdb_info sw_fdb_info =
{
   WP_BUS_PARAM, /* sw_fdb_bus */ 
   12, /* fdb_threshold */
#if 1
   0, /* max_fdb_rules */
#else
   10, /* max_fdb_rules */
#endif
};

WP_module_pce_init pce_init =
{
   &pce_parser_info, /* parser_info */
   200, /* hardware_rules_num */
   15, /* filters_num */
   10, /* pkt_rx_channel_interfaces_num */
   5, /* iw_tx_channel_interfaces_num */
   WP_PCE_DISCARD_MODE_SMART_DISCARD, /* pce_discard_mode */
   WP_ENABLE, /* bridge_loop_prevention_mode */
//   WP_PCE_LEARNING_PROTECTION_ENABLE | 
   WP_ENABLE, /* learning_mode */
   {/* learning_queue_config */
      30, /* learning_queue_size */
      WP_LEARNING_INTENABLE, /* interrupt_mode */
      WP_IW_IRQT0, /* int_queue_num */
      1, /* interrupt_rate */
      4, /* protection_table_size */
   },
   2, /* iw_next_round_pce_interfaces_num*/
   0, /* interfaces_on_atm_rx_mode */
   0,/* atm_rx_channel_interfaces_num */
   0,/* device_interfaces_num */
   WP_SW_FDB_ENABLE_MODE,/* enhanced_mode */
   &sw_fdb_info, /* sw_fdb_info  */
};

WP_int_queue_table int_queue_tables = {{{24, 1}, {10, 1}, {10, 1}, {10, 1}}};

WP_iw_global  max_iw[1] = {
   {
      /* max_iw_aggregations */ MAX_FLOW_AGG,
      /* max_r_iw_systems;*/ 3,
      /* max_directmap_enet_systems;*/ 0,
      /* max_b_iw_systems;*/ 6,
      /* max_m_iw_systems;*/ 0,
      /* max_vlan_priority_maps; */ 0,
      /* iw_bkgnd_fifo_size */0,
      /* cong_pt       */NULL,
      /* iw_host_limits */{64,0,0,64,0},
      /* mpls_config*/ {0,0},
      /* iw_modes;*/
      {
         /*policer_mode;*/                      WP_IW_FLOW_POLICER,
         /*statistics_bundle_stat_mode;*/       WP_IW_STATISTICS_BUNDLE_STAT_DISABLE,
         /*l2_prefix_ext_mode;*/                WP_IW_L2_HEADER_EXTENSION_ENABLE,
         /*enhanced_flow_stat_mode;*/           WP_IW_ENHANCED_FLOW_STAT_DISABLE,
         /*flow_stat_mode;*/                    WP_IW_FLOW_STAT_ENABLE,
         /*fr_tx_flow_stat_mode */              WP_IW_FR_TX_FLOW_STAT_DISABLE,
         /*mfc_alloc_mode; */                   WP_IW_MFC_ALLOC_ENABLE,
         /*Learning_queue_mode */               WP_IW_LEARNING_DISABLE,
         /*port_filtering_mode */               WP_IW_PORT_FILTERING_DISABLE,
         /* lpm_alloc_mode */                   WP_IW_LPM_ALLOC_DISABLE,
         /*enhanced_mc_member_stat_mode*/       WP_DISABLE,
         /*routing_alloc_modes*/                WP_DISABLE,
         /*res_mc_addresses_table_mode*/        WP_DISABLE,
         /*port_stag_etype_mode*/               WP_IW_PORT_STAG_ETYPE_ENABLE
      },
      /* max_ch_agg_stat;*/ 0,
      /* max_fr_s_iw_systems;*/ 0,
      /*max_iw_ports;*/ 100,
      /* max_iw_mc_groups;*/0,
      /* max_nat_systems;*/0,
      /*max_iw_second_aggregations;*/0, /* secondary flow aggregations */
      /*iw_comp_limits;*/0,
      /* max_iw_mc_members;*/0,
      /* max_iw_programmable_filters;*/MAX_IW_PROGRAMMABLE_FILTERS,
      /* max_iw_policers;*/MAX_IW_POLICERS,
      /* max_dscpmt */ 0,
      /* max_wred_bindings */ 0,
      /* tcm_policer_limits */
      {
         MAX_IW_POLICERS,
         MAX_IW_POLICERS
      }

   }
};

#if defined(WP_BOARD_WDS3)
WP_context context[1] =
{
   {8, /* qnodes */
    /* the first four assignments are mandatory bus assignments */
    {
       {/* WP_BUS_PARAM    */ WP_BUS_PARAM, APP_BANK_PARAM},
       {/* WP_BUS_PACKET   */ WP_BUS_PACKET, APP_BANK_PACKET},
       {/* WP_BUS_INTERNAL */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
       {/* WP_BUS_HOST     */ WP_BUS_HOST, APP_BANK_HOST},
       {/* WP_BUS_IRQ      */ WP_BUS_HOST, APP_BANK_HOST},
       {/* WP_BUS_TMSTMP   */ WP_BUS_HOST, APP_BANK_HOST},
       {/* UNASSIGNED      */ WP_BUS_NONE, 0},
       {/* UNASSIGNED      */ WP_BUS_NONE, 0},
       {/* ASU_TPT         */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
       {/* ASU CW          */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
       {/* ASU TX QUEUE    */ WP_BUS_PACKET, APP_BANK_PACKET},
       {/* STATISTICS      */ WP_BUS_PARAM  , APP_BANK_PARAM},
       {/* ADDRESS LOOKUP  */ WP_BUS_PARAM, APP_BANK_PARAM},
       {/* ATM CPT         */ WP_BUS_PARAM, APP_BANK_PARAM},
       {/* ENET HDLC CPT   */ WP_BUS_PACKET, APP_BANK_PACKET},
       {/* IW FBP PARAM    */ WP_BUS_PARAM, APP_BANK_PARAM},
       {/* IW FBP DATA     */ WP_BUS_PACKET, APP_BANK_PACKET},
       {/* EXTERNAL FIFO   */ WP_BUS_PACKET, APP_BANK_PACKET},
       {/* MAC LOOKUP      */ WP_BUS_PARAM, APP_BANK_PARAM},
       {/* IMA SMALL       */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
       {/* IMA LARGE       */ WP_BUS_PARAM, APP_BANK_PARAM},
       {/* LPM             */ WP_BUS_PACKET, APP_BANK_PACKET},
       {/* MFC RULES       */ WP_BUS_PARAM, APP_BANK_PARAM},
       {/* FIWT, QAT       */ WP_BUS_PARAM, APP_BANK_PARAM},
       {/* MFCPT, UPPPT, DS*/ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
       {/* STATISTICS2     */ WP_BUS_PACKET, APP_BANK_PACKET},
       {/* STATISTICS3     */ WP_BUS_HOST, APP_BANK_HOST},
       {/* UNASSIGNED      */ WP_BUS_NONE, 0},
       {/* PKT_GLOBAL_TCPT */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
       {/* UNASSIGNED      */ WP_BUS_NONE, 0},
       {/* MANDATORY       */ WP_BUS_NONE, 0},
       {/* MANDATORY       */ WP_BUS_NONE, 0},
       /* ************* VIRTUAL BUS ASSIGNMENT ***************/
       {/* MCH_DATA        */ WP_BUS_PACKET, APP_BANK_PACKET}
       ,
       {/* WP_VB_OAM_FM_QUERY*/ WP_BUS_NONE, 0},
       {/* WP_VB_HASH_ON_HEAP*/ WP_BUS_NONE, 0},
       {/* WP_VB_MFC_INTERNAL*/ WP_BUS_PACKET, APP_BANK_PACKET},
       {/* WP_VB_MFR_RAS_TBL,           */ WP_BUS_NONE, 0},
       {/* WP_VB_CESOP_DATA,            */ WP_BUS_NONE, 0},
       {/* WP_VB_SECURITY_ESE,          */ WP_BUS_NONE, 0},
       {/* WP_VB_PDCP_SECURITY_ENTITY,  */ WP_BUS_NONE, 0},
       {/* WP_VB_POLICER,               */ WP_BUS_PARAM, APP_BANK_PARAM},
       {/* WP_VB_PCE_LRN,               */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
       {/* WP_VB_PCEA_PARAM,            */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
       {/* WP_VB_CONGESTION_TABLE,      */ WP_BUS_NONE, 0},
         /*WP_VB_PARAMS_PROPAGATION             */{WP_BUS_NONE, 0},
         /*WP_VB_STATS_IWPORT_PCE_FILTERS       */{WP_BUS_INTERNAL, APP_BANK_INTERNAL},
#if _WP_MODULE_CUSTOM_TIGERMILK_ENABLE_
         /*WP_VB_PROFILE_MARKING_STATS          */{WP_BUS_NONE, 0},
#endif
         /*WP_VB_TIMESTAMP_SERVICES_TABLE       */{WP_BUS_NONE, 0},
         /*WP_VB_POLICER_ACTION                 */{WP_BUS_PARAM, APP_BANK_PARAM},
         {/* WP_VB_PECS_GLOBAL*/ WP_BUS_NONE, 0},
         {/* WP_VB_ENET_10G_LONG_PKT*/ WP_BUS_NONE, 0},
         {/* WP_VB_G999_STREAM*/ WP_BUS_NONE, 0},
         {/* WP_VB_G999_STREAM_STATISTICS*/ WP_BUS_NONE, 0},
         {/* WP_VB_FMU_FIFO_TO_CHANNEL_NUMBER*/ WP_BUS_NONE, 0},
         {/* WP_VB_IPWORT_EXT*/ WP_BUS_PARAM, APP_BANK_PARAM}
    },
    {
       dps_ProgramImage, dps_ProgramImage
    },
    {
       dps_PC_Table_Init, dps_PC_Table_Init
    },
    &int_queue_tables, /* Interrupt queue tables */
    POOL_LIMIT,        /* Mixumum number of pools */
    NULL,           /* ATM global parameters */
    max_iw          /* Interworking global parameters */
   }
};
#elif defined(WP_BOARD_WDS3_SL)
WP_context context[1] =
{
   {8, /* qnodes */
    /* the first four assignments are mandatory bus assignments */
    {
       {/* WP_BUS_PARAM    */ WP_BUS_PARAM, APP_BANK_PARAM},
       {/* WP_BUS_PACKET   */ WP_BUS_PACKET, APP_BANK_PARAM},
       {/* WP_BUS_INTERNAL */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
       {/* WP_BUS_HOST     */ WP_BUS_HOST, APP_BANK_HOST},
       {/* WP_BUS_IRQ      */ WP_BUS_HOST, APP_BANK_HOST},
       {/* WP_BUS_TMSTMP   */ WP_BUS_HOST, APP_BANK_HOST},
       {/* UNASSIGNED      */ WP_BUS_NONE, 0},
       {/* UNASSIGNED      */ WP_BUS_NONE, 0},
       {/* ASU_TPT         */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
       {/* ASU CW          */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
       {/* ASU TX QUEUE    */ WP_BUS_PARAM, APP_BANK_PARAM},
       {/* STATISTICS      */ WP_BUS_PARAM  , APP_BANK_PARAM},
       {/* ADDRESS LOOKUP  */ WP_BUS_PARAM, APP_BANK_PARAM},
       {/* ATM CPT         */ WP_BUS_PARAM, APP_BANK_PARAM},
       {/* ENET HDLC CPT   */ WP_BUS_PARAM, APP_BANK_PARAM},
       {/* IW FBP PARAM    */ WP_BUS_PARAM, APP_BANK_PARAM},
       {/* IW FBP DATA     */ WP_BUS_PARAM, APP_BANK_PARAM},
       {/* EXTERNAL FIFO   */ WP_BUS_PARAM, APP_BANK_PARAM},
       {/* MAC LOOKUP      */ WP_BUS_PARAM, APP_BANK_PARAM},
       {/* IMA SMALL       */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
       {/* IMA LARGE       */ WP_BUS_PARAM, APP_BANK_PARAM},
       {/* LPM             */ WP_BUS_PARAM, APP_BANK_PARAM},
       {/* MFC RULES       */ WP_BUS_PARAM, APP_BANK_PARAM},
       {/* FIWT, QAT       */ WP_BUS_PARAM, APP_BANK_PARAM},
       {/* MFCPT, UPPPT, DS*/ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
       {/* STATISTICS2     */ WP_BUS_PARAM, APP_BANK_PARAM},
       {/* STATISTICS3     */ WP_BUS_HOST, APP_BANK_HOST},
       {/* UNASSIGNED      */ WP_BUS_NONE, 0},
       {/* PKT_GLOBAL_TCPT */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
       {/* UNASSIGNED      */ WP_BUS_NONE, 0},
       {/* MANDATORY       */ WP_BUS_NONE, 0},
       {/* MANDATORY       */ WP_BUS_NONE, 0},
       /* ************* VIRTUAL BUS ASSIGNMENT ***************/
       {/* MCH_DATA        */ WP_BUS_PARAM, APP_BANK_PARAM}
       ,
       {/* WP_VB_OAM_FM_QUERY*/ WP_BUS_NONE, 0},
       {/* WP_VB_HASH_ON_HEAP*/ WP_BUS_NONE, 0},
       {/* WP_VB_MFC_INTERNAL*/ WP_BUS_PARAM, APP_BANK_PARAM},
       {/* WP_VB_MFR_RAS_TBL,           */ WP_BUS_NONE, 0},
       {/* WP_VB_CESOP_DATA,            */ WP_BUS_NONE, 0},
       {/* WP_VB_SECURITY_ESE,          */ WP_BUS_NONE, 0},
       {/* WP_VB_PDCP_SECURITY_ENTITY,  */ WP_BUS_NONE, 0},
       {/* WP_VB_POLICER,               */ WP_BUS_PARAM, APP_BANK_PARAM},
       {/* WP_VB_PCE_LRN,               */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
       {/* WP_VB_PCEA_PARAM,            */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
       {/* WP_VB_CONGESTION_TABLE,      */ WP_BUS_NONE, 0},
         /*WP_VB_PARAMS_PROPAGATION             */{WP_BUS_NONE, 0},
         /*WP_VB_STATS_IWPORT_PCE_FILTERS       */{WP_BUS_INTERNAL, APP_BANK_INTERNAL},
#if _WP_MODULE_CUSTOM_TIGERMILK_ENABLE_
         /*WP_VB_PROFILE_MARKING_STATS          */{WP_BUS_NONE, 0},
#endif
         /*WP_VB_TIMESTAMP_SERVICES_TABLE       */{WP_BUS_NONE, 0},
         /*WP_VB_POLICER_ACTION                 */{WP_BUS_PARAM, APP_BANK_PARAM},
         {/* WP_VB_PECS_GLOBAL*/ WP_BUS_NONE, 0},
         {/* WP_VB_ENET_10G_LONG_PKT*/ WP_BUS_NONE, 0},
         {/* WP_VB_G999_STREAM*/ WP_BUS_NONE, 0},
         {/* WP_VB_G999_STREAM_STATISTICS*/ WP_BUS_NONE, 0},
         {/* WP_VB_FMU_FIFO_TO_CHANNEL_NUMBER*/ WP_BUS_NONE, 0},
         {/* WP_VB_IPWORT_EXT*/ WP_BUS_PARAM, APP_BANK_PARAM}
    },
    {
       dps_ProgramImage, dps_ProgramImage
    },
    {
       dps_PC_Table_Init, dps_PC_Table_Init
    },
    &int_queue_tables, /* Interrupt queue tables */
    POOL_LIMIT,        /* Mixumum number of pools */
    NULL,           /* ATM global parameters */
    max_iw          /* Interworking global parameters */
   }
};
#endif

/* Callback function for a queuenode - Need to define the correct type later*/
WP_S32 QNCallback(WP_S32 status) {
   printf("App: QNCallback: Status returned is %d\n",status);
   return 0;
}

WP_pool_buffer_data buffer_data_2048[] = {
   {
      /* n_buffers */ 192,
      /* offset */ 64,
      /* size */ 1984, /* RBS */
      /* pad */ 0,
      /* bus */ BUFFER_BUS,
      /* bank */ BUFFER_BANK
   }
};

WP_pool_buffer_data buffer_data_144[1] =
{
   {
      /* n_buffers */ 1000,
      /* offset */ 0,
      /* size */ (WT_MAX_FRAME_SIZE+8),
      /* pad */ 0,
      /* bus */ BUFFER_BUS,
      /* bank */ BUFFER_BANK
   }
};

WP_ch_enet gbe_ch[2] =
{
   {
      /* intmode */           WP_PKTCH_INT_DISABLE,
      /* iwmode */            WP_PKTCH_IWM_ENABLE,
      /* testmode */          WP_PKTCH_TEST_DISABLE,
      /* tx_pqblock */        0,
      /* tx_pqlevel */        0,
      /* tx_shaping_type */   WP_FMU_SHAPING_TYPE_STRICT,
      /* tx_shaping_params */ NULL,
      /* rx_maxsdu */         WT_MAX_FRAME_SIZE,
      /* tx_cwid */           WP_CW_ID_A,
      /* tx_tq */             0,
      /* rx_queuedepth[4] */  17
   },
   {
      /* intmode */           WP_PKTCH_INT_ENABLE,
      /* iwmode */            WP_PKTCH_IWM_DISABLE,
      /* testmode */          WP_PKTCH_TEST_DISABLE,
      /* tx_pqblock */        0,
      /* tx_pqlevel */        0,
      /* tx_shaping_type */   WP_FMU_SHAPING_TYPE_STRICT,
      /* tx_shaping_params */ NULL,
      /* rx_maxsdu */         WT_MAX_FRAME_SIZE,
      /* tx_cwid */           WP_CW_ID_A,
      /* tx_tq */             0,
      /* rx_queuedepth[4] */  17
   }
};

WP_tx_binding  tx_binding_iw[1]=
{
   {
      /* res0;*/ 0,
      /* dci_mode;*/  WP_IW_DYN_CH_INSERT_DISABLE,
      /* maxt;*/55,
   }
};

WP_ch_iw_rx ch_config_iw[1]=
{
   {
      /* pqblock */ 0,
      /* pqlevel */ 0,
      /* tx_binding_type */ WP_IW_TX_BINDING,
      /* tx_binding_config */ tx_binding_iw
   }
};


WP_tx_binding tx_binding_cfg[1] =
{
   {
      /* res0 */0,
      /* dci_mode */WP_IW_DYN_CH_INSERT_DISABLE,
      /* maxt */WT_MAX_FRAME_SIZE
   }
};

WP_qnode_iwq qn_iw[1]=
{
   {
      /* interruptqueue */ 0,
      /* num_buffers */ 192,
      /* adjunct_pool */ 0
   }
};

/* <<<<<<<<<<<<<<<<<<<<<<<<<<< GMII START >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */

/* Giga Ethernet port1 & 2 configuration */
WP_port_enet enet_s_fast[1] =
{
   {
      /* pkt_limits */  {64, 64, 0 ,0 },
      /* flowmode */       WP_FLOWMODE_FAST,
      /* interface_mode */ WP_ENET_SGMII_1000,
      /* rx_iw_bkgnd */    0
   }
};

/* Enet device configuration */
WP_device_enet enet_dev[1] =
{
   {
      /* max_tx_channels */     MAX_NUM_OF_CHANNELS,
      /* tx_maxsdu */           WT_MAX_FRAME_SIZE,
      /* operating_speed */     WP_UNUSED,
      /* mac_addr */ {0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa},
      /* tx_bit_rate */        WP_TX_BIT_RATE_UNLIMITED,
      /* loopbackmode */ WP_ENET_NORMAL,
      /* extended_params */ NULL,
   }
};

WP_device_enet_ex enet_device_extended_config[1] =
{
   {
      /*duplexmode*/       WP_ENET_FULLDUPLEX,
      /*rx_maclookup_mode*/       WP_DISABLE,
      /*rx_flowcontrol*/       WP_ENET_FLOW_ENABLE,
      /*tx_flowcontrol*/       WP_ENET_FLOW_ENABLE,
      /*rx_addrmode*/       WP_ENET_ADDR_ACCEPTALL,
      /*phystatmode*/       WP_ENET_STAT_ENABLE,
      /*tx_statmode*/       WP_ENABLE,
      /*rx_statmode*/       WP_ENABLE,
      /*tx_duplicate*/       WP_DISABLE,
      /*rx_filters*/       0,
      /*rx_timestamp*/       WP_DISABLE,
      /*timestamp_type*/       0,
      /*max_tx_slow_channels*/  MAX_NUM_OF_CHANNELS,
   },
};

/* <<<<<<<<<<<<<<<<<<<<<<<<<<< GMII END >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */

WP_iw_agg_bridging br_agg_gbe[1] =
{
   {
      /*tag*/ 2,
      /*txfunc*/ 0,
#if 0
      // wp_iw.h
      // line: 03001
#define input_bport	iw_port
#endif
      /*input_bport*/0,
      /*rfcs*/WP_IW_RFCS_ENABLE,
      /*l2_header_insert_mode;*/WP_IW_L2H_INSERT_DISABLE,
      /*vlan_tag_mode*/WP_IW_VLAN_TAG_ENABLE,
      /*interruptqueue;*/WP_IW_IRQT1,
      /*error_pkt_mode*/WP_IW_ERRPKT_DISCARD,
      /*intmode;*/WP_IW_INT_ENABLE,
      /*statmode;*/WP_IW_STAT_ENABLE,
      /*timestamp_mode;*/WP_IW_TIME_STAMP_DISABLE,
      /*ov_pool_mode */ WP_IW_OV_POOL_DISABLE,
      /*fbp_drop_threshold;*/0,
      /*replace_vlan_id*/WP_IW_REPLACE_VTAG_DISABLE,
      /*vlan_id*/0x05,
      /*vpmt_handle */0,
      /*mtu;*/WT_MAX_FRAME_SIZE,
      /*prefix_length */ 0,
      /*prefix_header[28];*/ {0x1,0x2,0x3,0x4,0x5,0x6,0x7,
                              0x8,0x9,0x10,0x11,0x12,0x13,0x14,
                              0x15,0x16,0x17,0x18,0x19,0x20,0x21,
                              0x22,0x23,0x24,0,0,0,0},
      /*policer_enable*/ WP_IW_POLICER_DISABLE,
      /**policer_config;*/NULL,
      /*cong_mode;*/WP_IW_CONGESTION_DISABLE,
      /**cong_threshold_param;*/NULL
   }
};

WP_iw_sys_bridging iw_sys_bridging[1]=
{
   {
      /* tag */                   0,
      /* max_flows; */            8,
      /* classification_mode */   WP_IW_NO_CLASSIFIER,
      /* classifier_config */     {0,0,0,NULL},
      /* learning_mode;*/         WP_IW_BRIDGE_LEARNING_DISABLE,
      /* learning_queue */
      {
         /*int_queue_num*/        WP_IW_IRQT1,
         /*learning_queue_size*/  LEARNING_QUEUE_SIZE,
         /*interrupt_enbale*/     WP_LEARNING_INTENABLE,
         /*interrupt_rate*/       1
      },
      /*forwarding_table_size*/   WP_IW_HASH_UNUSED,
      /*member_set_size*/         WP_IW_HASH_UNUSED,
      /*buffer_gap;*/             0x80,
      /*max_bridging_ports*/      IW_SYS_MAX_BPORTS,
      /*dfc_info:*/               NULL,
      /*svl_mode*/                WP_IW_SVL_DISABLED, 
      /*stag_ether_type*/         0/*DEFAULT VALUE*/,
   }
};

WP_tx_binding  tx_binding_l2pi[1]=
{
   {
      /* wred_mode;*/ 0,
      /* dci_mode;*/  1,
      /* maxt;*/128,
   }
};

WP_rx_binding_bridging  rx_binding_cfg[1]=
{
   {
       /*  encap_mode */0,
       /*  mru;*/512,
       /*  vcfcs;*/0,
       /*  input_port;*/0
   }
};

WP_bridge_port iwhost_bport_config[1] =
{
   {
      /* tag */ 0x2000,
      /* direct_mapping */ WP_IW_DIRECT_MAP_ENABLE,
      /* flow_agg */ 0,
      /* flooding_term_mode */ WP_IW_HOST_TERM_MODE,
      /* learning_mode */ WP_IW_LEARNING_DISABLE,
      /* in_filter_mode */ WP_IW_INGRESS_FILTER_DISABLE,
      /* vlan_param */
      {
         /* vlan_acceptance_mode */ WP_IW_ACCEPT_TAGGED_ONLY,
         /* vlan_tag */ 0x5,
         /*vlan_tunnel*/WP_IW_VLAN_TUNNEL_1Q_IN_1Q_DISABLE,
         /*vlan_priority_enforce_mode*/WP_IW_VLAN_PRIORITY_ENFORCE_DISABLED,
         /*stag_vlan_etype*/ WP_IWPORT_STAG_DISABLED
      },
      /*max_mac_addresses */ 34,
      /*group_tag */ 0,
      /*group_filtering_mode */ WP_IW_GROUP_FILTER_DISABLE,
      /*unk_mac_sa_filter*/ 0,
      /*unk_mc_mode;*/0,
      /*bc_ht_mode;*/ 0,
      /*input_filters_mask*/ 0,
      /*output_filters_mask;*/ 0,
      /*statmode*/ WP_IW_PORT_STAT_ENABLE
   }
};

WP_bridge_port bport_conf =
{
      /* tag */                   0,
      /* direct_mapping*/         WP_IW_DIRECT_MAP_ENABLE,
      /* flow_agg */              0,
      /* termination_mode*/       WP_IW_HOST_TERM_MODE,
      /* learning_mode */         WP_IW_LEARNING_DISABLE,
      /* in_filter_mode */        WP_IW_INGRESS_FILTER_DISABLE,
      /* vlan_param */
      {
         /* vlan_acceptance_mode */WP_IW_ACCEPT_TAGGED_ONLY,
         /* vlan_tag */0,
         /*vlan_tunnel*/WP_IW_VLAN_TUNNEL_1Q_IN_1Q_DISABLE,
         /*vlan_priority_enforce_mode*/WP_IW_VLAN_PRIORITY_ENFORCE_DISABLED,
         /*stag_vlan_etype*/ WP_IW_BPORT_STAG_VLAN_ETYPE_DISABLE
      },
      /* max_mac_addresses */ 300,
      /* group_tag */ 0,
      /*group_filtering_mode*/     WP_IW_GROUP_FILTER_DISABLE,
      /*unk_mac_sa_filter*/        WP_IW_UNK_MACSA_FILTER_ENABLE,
      /*unk_mc_mode*/              WP_IW_UNK_MC_DROP,
      /*bc_ht_mode*/               WP_IW_BC_HT_DISABLE,
      /*unk_input_filters_mask*/   0,
      /*unk_output_filters_mask*/  0,
      /*statmode*/                 WP_IW_PORT_STAT_ENABLE,
      /*unk_uc_mode*/              WP_IW_UNK_UC_SR_ENABLE,
      /*classification_flag*/      WP_IW_BPORT_CLASSIFICATION_ENABLED,
      /*adv_unk_lookup_mode*/      WP_IW_ADV_UNK_LOOKUP_ENABLED,
      /*cfi_ht_mode*/              WP_IW_CFI_HT_DISABLE,
      /*reserved_mc_ht_mode*/      WP_IW_RES_MC_HT_DISABLE,
      /*predefined_ht_mode*/       WP_DISABLE,
      /*res_mc_action_table */     {0},
      /*WP_iw_bport_enet_oam_params */ NULL
};

void WP_AppLog (WP_CHAR *s)
{
}

void WT_Start(WP_CHAR *app_name, WP_CHAR *trial_name, WP_CHAR *file_name);
void WT_Reboot(void);
static void WP_CheckPceModuleInitScenarios(void);
static void check_error(WP_handle handle, WP_CHAR *s, WP_U32 line, WP_U32 excpected_error);
static void terminate_on_error(WP_handle, WP_CHAR *,WP_U32);
static void terminate(WP_CHAR *s, WP_U32 line);
static void WPE_LayerTwoSystemCreate(void);
static void WPE_SystemEnable(void);
static void WPE_ChannelsDelete(void);
static void WPE_ChannelsCreate(void);
static void WPE_PortsCreate(WP_handle iw_sys, WP_handle *iwport, WP_U32 max_mac_addresses);
void WPT_CompatibleFlowStatistics( WP_handle agg);
WP_U32 WPT_CompatibleFaGetFwPacketsCount(WP_handle agg);
static void WPE_RxBindingDelete(WP_handle ch, WP_handle iw_sys);
static void WPE_DlRxBindingCreate(WP_handle iw_sys);
void WPT_DisplayBridgingPortStats( WP_handle port_handle, WP_CHAR* type);
void WPT_DisplayRoutingPortStats( WP_handle port_handle, WP_CHAR* type);
static void WPE_HostSendPacket(WP_U32 filter_set_index, WP_U32 packet_index);
static void WPE_CheckReceivedTraffic(WP_U32 packet_index);
static void WPE_CheckStatistics(void);
WP_U16 WTI_IpChecksumCalc(WP_U8 *buff, WP_U16 len);
void WTI_TranslateHexToAscii(WP_CHAR *Ascii, WP_CHAR *Hex, WP_U32 length);

void WPU_WinutilCall(WP_CHAR * input);
void CLI_RunTest(void);
void CLI_CreateTest(void);
void CLI_DeleteTest(void);
void CLI_RunScenarios(void);
void WPE_StatisticsReset(void);
void WPE_CheckPceFilterStatsReset(void);
void WT_UtilitiesMenu(void);
void CLI_Menu(void);
void CLI_StatMenu(void);

void WPE_CreateDummyPceHwRules(WP_U32 rules_needed_count);
void WPE_DeleteDummyPceHwRules(void);

static void WPE_DeleteDlPceInterface(void);
static void WPE_CreateDlPceInterface(WP_handle iw_sys);
static void WPE_CreateDlPceFilters(void);
static void WPE_CreateDlPceRules(void);
static void WPE_DeleteDlPceRules(void);
static void WPE_CreateDlPceFilterSets(void);
static void WPE_DeleteDlSecondRoundPceInterface(void);
static void WPE_CreateDlSecondRoundPceInterface(void);
static void WPE_CreateDlSecondRoundPceFilters(void);
static void WPE_CreateDlSecondRoundPceFilterSets(void);
static void WPE_CreateDlSecondRoundPceRules(void);
static void WPE_DeleteDlSecondRoundPceRules(void);
static void WPE_PrintPceGlobalStats(void);
static void WPE_CheckForwardingFilterCreateErrors(void);
static void WPE_CheckForwardingFilterModifyErrors(void);
static void WPE_CheckLearningFilterCreateErrors(void);
static void WPE_CheckLearningFilterModifyErrors(void);
static void WT_CheckFilterSetModify(void);

WP_U32 WPT_HostChannelQueueSize(WP_handle rx_host_handle);
WP_U32 WPT_HostChannelQueueSizeX(WP_handle rx_host_handle);
WP_U32 WPT_ShowChannelQueueSize(WP_handle rx_host_handle);
WP_U32 WPT_LocalDisplayDeviceStats(WP_handle device_handle);
void WPE_CheckPceFilterStats(WP_handle filter);

WP_handle devhost;
WP_handle default_agg, flow_agg[FLOW_AGG_COUNT];
WP_handle pool_2048, pool_144;

WP_handle gbe_port1, gbe_dev1;

WP_handle qniw;
WP_handle h_iw_port_general, h_iw_port_gbe[FILTER_SET_COUNT * NUM_OF_INPUT_PORT_PER_FILTER_SET], h_iw_port_next_round,
   h_iw_output_port, h_iw_output_port_b;
WP_handle rx_host_handle, pce_dl_second_round_if_handle;
WP_handle bport_host;
WP_handle gbe1_rx_ch_handle, gbe1_tx_ch_handle;

WP_handle dl_general_iwsys_bridge, dl_iwsys_bridge[NUM_OF_INPUT_PORT_PER_FILTER_SET];

WP_U32 payload_size[2*NUM_OF_FLOWS];
WP_U32 l2_l3_size[2*NUM_OF_FLOWS];

WP_U8 expected_data[2*NUM_OF_FLOWS][3536];

WP_boolean scenario_failed;

/* PCE */
WP_pce_global_stat pce_global_stats;
WP_handle PCE_filter[100];
WP_handle dl_filter_set[FILTER_SET_COUNT];
WP_handle PCE_rule_handle[256*1024];

WP_handle rule_iterator_handle = NULL;

WP_U16 wpe_test_failures = 0;

WP_pce_learned_forwarding_rule learned_rules_list[100];

/******************************/

WP_status logCallBack(WP_S32 level, const WP_CHAR *message)
{
   if(level <= 10)
      printf("%s", message);
   return WP_OK;
}

WP_U32 debug_error = 1;

WP_status WPE_DebugCallback(const WP_CHAR *file, const WP_CHAR *function,
                            WP_S32 line, WP_S32 error_id)
{
   if (debug_error)
   {
      if (error_id != WP_ERR_HST_NO_DATA_TO_GET)
         printf("\n\nfile = %s , line = %d\nfunction = %s\n",file,line,function);
   }
   return WP_OK;
}

void WTI_GenerateRandomData(WP_CHAR *Hex,
                            WP_U8 pattern1,
                            WP_U8 rand_flag,
                            WP_U32 max_size,
                            WP_U32 min_size)
{
   WP_U8 tmprand;
   WP_U32 i,datarandlength;

   if (max_size == min_size)
      datarandlength = max_size;
   else
      if (min_size < max_size)
         datarandlength= min_size + (rand()%(max_size - min_size));
      else datarandlength = max_size + (rand()%(min_size - max_size));

   for (i=0;i<datarandlength;i++)
   {

      if (rand_flag)
         tmprand = rand() % 256; /* random numer 0-256 */
      else
      {
         tmprand = pattern1;
      }

      Hex[i] = tmprand;
   }
}

void PrepareExpectedData(WP_U8 *expected_data,
                         WP_U32 payload_size,
                         WP_U32 filter_set_index, 
                         WP_U8 packet_index)
{
   WP_U32 header_size;
   WP_CHAR ascii_buffer[MAX_DATA_LENGTH*2+1];
   WP_CHAR hexa_buffer[MAX_DATA_LENGTH*2];
   WP_CHAR mac_sa_high[9];
   WP_CHAR mac_sa_low[5];
   WP_CHAR mac_da_high[9];
   WP_CHAR mac_da_low[5];
   WP_CHAR cvlan[7];

   /* set the ip header the packet */
   WP_U8 temp_ip_header_hex[100];
   WP_U8 temp_ip_header_char[100];
   WP_U16 checksum;
   WP_U16 ip_length;
   WP_CHAR ip_length_char[10];

   /* set the src and dst mac address at the packet */
   sprintf (mac_da_high, "%08x", Traffic_Table[filter_set_index][2]);
   sprintf (mac_da_low, "%04x", (Traffic_Table[filter_set_index][3] + packet_index));
   sprintf (mac_sa_high, "%08x", Traffic_Table[filter_set_index][0]);
   sprintf (mac_sa_low, "%04x", (Traffic_Table[filter_set_index][1] + packet_index));
   
   strcpy (ascii_buffer, mac_da_high);
   strcat (ascii_buffer, mac_da_low);
   strcat (ascii_buffer, mac_sa_high);
   strcat (ascii_buffer, mac_sa_low);

   /* Add SVlan */
   strcat (ascii_buffer, "81000");
   sprintf (cvlan, "%03x", (Traffic_Table[filter_set_index][4] + (packet_index%2)));
   strcat (ascii_buffer, cvlan);

   /* Add CVlan */
   strcat (ascii_buffer, "81000");
   sprintf (cvlan, "%03x", (Traffic_Table[filter_set_index][4] + (packet_index%2)));
   strcat (ascii_buffer, cvlan);
   
   /* set the ethernet type at the packet */
   strcat (ascii_buffer, "0800");


   ip_length = WT_IPV4_HEADER_SIZE + WT_L4_HEADER_SIZE + payload_size;
   sprintf(ip_length_char, "%04x", ip_length);     /* Total Length */

   sprintf ((WP_CHAR *)temp_ip_header_char, "45");       /* Version = Ipv4,  IHL = 5 */
   strcat ((WP_CHAR *)temp_ip_header_char, "80");        /* Type Of Service - 8 bit */
   strcat ((WP_CHAR *)temp_ip_header_char, ip_length_char);     /* Total Length */
   strcat ((WP_CHAR *)temp_ip_header_char, "00000000"); /* Id, Flags, frag */
   strcat ((WP_CHAR *)temp_ip_header_char, "0A");       /* TTL = 0x09, */
   strcat ((WP_CHAR *)temp_ip_header_char, "11");/* ip_protocol - 8 bits*/
   strcat ((WP_CHAR *)temp_ip_header_char, "0000"); /* IP header Checksum - updated later*/

   strcat ((WP_CHAR *)temp_ip_header_char, "c0a80101");/* IP src address */
   strcat ((WP_CHAR *)temp_ip_header_char, "c0a8010b");/* IP dst address */

   App_TranslateAsciiToHex((WP_CHAR *)temp_ip_header_hex,
                           (WP_CHAR *)temp_ip_header_char, WT_IPV4_HEADER_SIZE);
   checksum = WTI_IpChecksumCalc(temp_ip_header_hex, WT_IPV4_HEADER_SIZE);
   temp_ip_header_hex[10] = checksum >> 8;
   temp_ip_header_hex[11] = checksum & 0xff;
   WTI_TranslateHexToAscii((WP_CHAR *)temp_ip_header_char,
                           (WP_CHAR *)temp_ip_header_hex, WT_IPV4_HEADER_SIZE);

   strcat ((WP_CHAR *)ascii_buffer, (WP_CHAR *)temp_ip_header_char);

   header_size = strlen(ascii_buffer)/2;

   /* printf("\nEXPECTED DATA HEADER NUMBER %d: %s \n",channel_index, ascii_buffer); */

   App_TranslateAsciiToHex(hexa_buffer, ascii_buffer, header_size);

   /* Src and Dst ports */
   hexa_buffer[header_size + 0] = 0;
   hexa_buffer[header_size + 1] = (WP_CHAR)0xc8;
   hexa_buffer[header_size + 2] = 0;
   hexa_buffer[header_size + 3] = (WP_CHAR)0xc8;
   hexa_buffer[header_size + 4] = 0;
   hexa_buffer[header_size + 5] = payload_size + WT_L4_HEADER_SIZE;
   hexa_buffer[header_size + 6] = 0;
   hexa_buffer[header_size + 7] = 0;
   header_size += WT_L4_HEADER_SIZE;
   l2_l3_size[packet_index] = header_size;

   WTI_GenerateRandomData(&hexa_buffer[header_size],
                          packet_index, 0, payload_size, payload_size);

   memcpy(expected_data, (WP_U8 *)hexa_buffer, header_size + payload_size);
}

void PacketDataUnitSetup(WP_data_unit *data_unit,
                         WP_data_segment *km_tx_buf,
                         WP_U8 *km_data_ptr,
                         WP_U32 payload_size,
                         WP_U32 filter_set_index, 
                         WP_U8 packet_index)
{
   WP_U32 header_size;
   WP_CHAR ascii_buffer[MAX_DATA_LENGTH*2+1];
   WP_CHAR hexa_buffer[MAX_DATA_LENGTH*2];
   WP_CHAR mac_sa_high[9];
   WP_CHAR mac_sa_low[5];
   WP_CHAR mac_da_high[9];
   WP_CHAR mac_da_low[5];
   WP_CHAR cvlan[7];
#if DEBUG
   WP_U32 ii = 0;
#endif

   /* set the ip header the packet */
   WP_U8 temp_ip_header_hex[100];
   WP_U8 temp_ip_header_char[100];
   WP_U16 checksum;
   WP_U16 ip_length;
   WP_CHAR ip_length_char[10];

   /* Prepare packet for Host Send  */
   data_unit->data_size = MAX_DATA_LENGTH;
   data_unit->n_active = 1;

   km_tx_buf->data_size = data_unit->data_size;
   km_data_ptr = WP_PoolAlloc(pool_144);
   if (!km_data_ptr)
   {
      terminate("Pool Empty",__LINE__);
   }
   km_tx_buf->pool_handle = pool_144;
   km_tx_buf->data = km_data_ptr;

   km_tx_buf->next = NULL;
   km_tx_buf->displacement = 0;

   data_unit->segment = (km_tx_buf);
   data_unit->n_segments = 1;
   data_unit->type = WP_DATA_ENET;
   data_unit->control = 0;

   /* set the src and dst mac address at the packet */
   sprintf (mac_da_high, "%08x", Traffic_Table[filter_set_index][2]);
   sprintf (mac_da_low, "%04x", (Traffic_Table[filter_set_index][3]  + packet_index));
   sprintf (mac_sa_high, "%08x", Traffic_Table[filter_set_index][0]);
   sprintf (mac_sa_low, "%04x", (Traffic_Table[filter_set_index][1] + packet_index));
   strcpy (ascii_buffer, mac_da_high);
   strcat (ascii_buffer, mac_da_low);
   strcat (ascii_buffer, mac_sa_high);
   strcat (ascii_buffer, mac_sa_low);

   /* Add SVlan */
   strcat (ascii_buffer, "81000");
   sprintf (cvlan, "%03x", (Traffic_Table[filter_set_index][4] + (packet_index%2)));
   strcat (ascii_buffer, cvlan);

   /* Add CVlan */
   strcat (ascii_buffer, "81000");
   sprintf (cvlan, "%03x", (Traffic_Table[filter_set_index][4] + (packet_index%2)));
   strcat (ascii_buffer, cvlan);

   /* set the ethernet type at the packet */
   strcat (ascii_buffer, "0800");


   ip_length = WT_IPV4_HEADER_SIZE + WT_L4_HEADER_SIZE + payload_size;
   sprintf(ip_length_char, "%04x", ip_length);     /* Total Length */

   sprintf ((WP_CHAR *)temp_ip_header_char, "45");       /* Version = Ipv4,  IHL = 5 */
   strcat ((WP_CHAR *)temp_ip_header_char, "80");        /* Type Of Service - 8 bit */
   strcat ((WP_CHAR *)temp_ip_header_char, ip_length_char);     /* Total Length */
   strcat ((WP_CHAR *)temp_ip_header_char, "00000000"); /* Id, Flags, frag */
   strcat ((WP_CHAR *)temp_ip_header_char, "0A");       /* TTL = 0x0a, */
   strcat ((WP_CHAR *)temp_ip_header_char, "11");/* ip_protocol - 8 bits*/
   strcat ((WP_CHAR *)temp_ip_header_char, "0000"); /* IP header Checksum - updated later*/

   strcat ((WP_CHAR *)temp_ip_header_char, "c0a80101");/* IP src address */
   strcat ((WP_CHAR *)temp_ip_header_char, "c0a8010b");/* IP dst address */

   App_TranslateAsciiToHex((WP_CHAR *)temp_ip_header_hex,
                           (WP_CHAR *)temp_ip_header_char, WT_IPV4_HEADER_SIZE);
   checksum = WTI_IpChecksumCalc(temp_ip_header_hex, WT_IPV4_HEADER_SIZE);
   temp_ip_header_hex[10] = checksum >> 8;
   temp_ip_header_hex[11] = checksum & 0xff;
   WTI_TranslateHexToAscii((WP_CHAR *)temp_ip_header_char,
                           (WP_CHAR *)temp_ip_header_hex, WT_IPV4_HEADER_SIZE);

   strcat (ascii_buffer, (WP_CHAR *)temp_ip_header_char);

   header_size = strlen(ascii_buffer)/2;
   l2_l3_size[packet_index] = header_size;

#if DEBUG
   printf("\nHOST SEND DATA: l2_l3_size=%d, payload_size=%d \n",
          l2_l3_size[packet_index], payload_size);
#endif

   App_TranslateAsciiToHex(hexa_buffer, ascii_buffer, header_size);

   /* Src and Dst ports */
   hexa_buffer[header_size + 0] = 0;
   hexa_buffer[header_size + 1] = (WP_CHAR)0xc8;
   hexa_buffer[header_size + 2] = 0;
   hexa_buffer[header_size + 3] = (WP_CHAR)0xc8;
   hexa_buffer[header_size + 4] = 0;
   hexa_buffer[header_size + 5] = payload_size + WT_L4_HEADER_SIZE;
   hexa_buffer[header_size + 6] = 0;
   hexa_buffer[header_size + 7] = 0;
   header_size += WT_L4_HEADER_SIZE;

   WTI_GenerateRandomData(&hexa_buffer[header_size], 
                          packet_index, 0, payload_size, payload_size);

   WP_MEM_BYTES_FILL(km_data_ptr, '0', buffer_data_144[0].size);
   WP_MEM_BYTES_SET(km_data_ptr, (WP_U8 *)hexa_buffer, MAX_DATA_LENGTH);

   data_unit->data_size = header_size + payload_size;
   km_tx_buf->data_size = data_unit->data_size;

#if DEBUG
   printf ("packet : %x \n ", (WP_U32) km_data_ptr);

   for( ii=0; ii < data_unit->segment->data_size; ii++)
   {
      if((ii)%4 == 0) printf("\n");
      printf("%2.2x", data_unit->segment->data[ii]);
   }
   printf("( %d bytes )\n", data_unit->segment->data_size);

#endif
}


WP_S32 main(WP_S32 argc, WP_CHAR **argv)
{
   WT_Start("111001", NULL, __FILE__);
   /* run the scenarios */

   WT_Identify();

   printf( "\n\nTest SW FDB Only Mode\n");
   printf( "----------------------\n\n");

   TestType = TEST_SW_FDB_ONLY;
   CLI_RunTest();
   
   printf( "\n\nTest Hybrid Mode\n");
   printf( "----------------------\n\n");
   
   TestType = TEST_HYBRID_FDB;
   CLI_RunTest();
   
   printf( "\n\nTest HW FDB Mode\n");
   printf( "----------------------\n\n");
    
   TestType = TEST_HW_FDB_ONLY;
   CLI_RunTest();

   
   printf( "\n\nTest Passed\n");

   /* Reboot */
   WT_Reboot();

   exit(0);
   return 1;
}

void CLI_RunTest(void)
{
  CLI_CreateTest();

  CLI_RunScenarios();
    
  CLI_DeleteTest();
}

void CLI_DeleteTest(void)
{
   /* Delete the DL PCE filters and filter sets */
   WPE_DeleteDlPceInterface();

   /* Delete the DL Second round filters and filter sets */
   WPE_DeleteDlSecondRoundPceInterface();

   WPE_RxBindingDelete(gbe1_rx_ch_handle, dl_general_iwsys_bridge);
   
   WPE_ChannelsDelete();
   
   /* Release WDDI */
   WP_DriverRelease();
}

/* Send traffic to all filter sets */
void CLI_RunAllFiltersScenario(void)
{
   WP_U32 ii, filter_set_index;
   
   memcpy(previous_filter_stat, filter_stat, sizeof(WP_pce_filter_stat)*100);

   /* send data */
   for(filter_set_index=0; filter_set_index < FILTER_SET_COUNT; filter_set_index++)
   {
      for(ii=0; ii < NUM_OF_FLOWS/* 12 */; ii++)
      {
         WPE_HostSendPacket(filter_set_index, ii);
         WPT_ShowChannelQueueSize (rx_host_handle);
      }
      
      for(ii=0; ii < NUM_OF_FLOWS; ii++)
      {
         if (1)
         {
            WPE_CheckReceivedTraffic(ii);         
            printf ("CLI_RunAllFiltersScenario: WPE_CheckReceivedTraffic(%2d)(%2d)\n", filter_set_index, ii);
         }
      }
   }
   printf ("CLI_RunAllFiltersScenario: before WPE_CheckStatistics()\n"); 
   WPE_CheckStatistics();

   printf("\nFilter Scenarios Pass\n");
}

void CLI_CheckFwdFilterStatistics(WP_U32 expected_default_fa_count,
                                  WP_U32 filter_num,
                                  WP_U32 deny,
                                  WP_U32 err_line)
{
   WP_handle status;

  /* Get filter statistics */
   status = WP_PceFilterStatistics (PCE_filter[filter_num], &filter_stat[filter_num]);
   terminate_on_error(status, "WP_PceFilterStatistics()",err_line);


   if (expected_default_fa_count != WPT_CompatibleFaGetFwPacketsCount(default_agg))
   {
      printf("Class FA Expected default %d,  Got feault %d \n", 
             expected_default_fa_count,
             WPT_CompatibleFaGetFwPacketsCount(default_agg));
      terminate("FA Statistics Error",err_line);
   }
  
   if (filter_stat[filter_num].no_matched_pkts != 
       (previous_filter_stat[filter_num].no_matched_pkts + NUM_OF_FLOWS))
   {
      terminate("PCE Filter no_match statistics are wrong",err_line);
   }

   if (filter_stat[filter_num].matched_pkts != 
       (previous_filter_stat[filter_num].matched_pkts))
   {
      terminate("PCE Filter match statistics are wrong",err_line);
   }

   if (filter_stat[filter_num].denied_pkts != 
       (previous_filter_stat[filter_num].denied_pkts + (deny*NUM_OF_FLOWS)))
   {
      terminate("PCE Filter deny statistics are wrong",err_line);
   }

}


void CLI_CheckStatisticsMatched(WP_U32 excpected_class_fa_count, 
                                WP_U32 expected_default_fa_count,
                                WP_U32 expected_b_class_match_count,
                                WP_U32 expected_a_forwarding_match_count,
                                WP_U32 expected_a_forwarding_no_match_count,
                                WP_U32 err_line)
{
   WP_handle status;

   /* Get filter statistics */
   status = WP_PceFilterStatistics (PCE_filter[FILTER_SET_B_CLASSIFICATION], &filter_stat[FILTER_SET_B_CLASSIFICATION]);
   terminate_on_error(status, "WP_PceFilterStatistics()",err_line);

   status = WP_PceFilterStatistics (PCE_filter[FILTER_SET_A_FORWARDING], &filter_stat[FILTER_SET_A_FORWARDING]);
   terminate_on_error(status, "WP_PceFilterStatistics()",err_line);

  if (excpected_class_fa_count != WPT_CompatibleFaGetFwPacketsCount(flow_agg[FILTER_SET_B_CLASSIFICATION_FLOW_AGG]) ||
      expected_default_fa_count != WPT_CompatibleFaGetFwPacketsCount(default_agg))
  {
     printf("Class FA Expected %d, default %d,  Got Class FA %d, Got feault %d \n", 
            excpected_class_fa_count, 
            expected_default_fa_count,
            WPT_CompatibleFaGetFwPacketsCount(flow_agg[FILTER_SET_B_CLASSIFICATION_FLOW_AGG]),
            WPT_CompatibleFaGetFwPacketsCount(default_agg));
     terminate("FA Statistics Error",err_line);
  }
  
  if (filter_stat[FILTER_SET_B_CLASSIFICATION].matched_pkts != expected_b_class_match_count ||
      filter_stat[FILTER_SET_A_FORWARDING].matched_pkts != expected_a_forwarding_match_count || 
      filter_stat[FILTER_SET_A_FORWARDING].no_matched_pkts != expected_a_forwarding_no_match_count)
   {
      printf("classification Got %8x Exp %8x, fw match Got %8x Exp %8x, Fwd no match Got %8x Exp %8x\n",
             (WP_U32)filter_stat[FILTER_SET_B_CLASSIFICATION].matched_pkts, 
             expected_b_class_match_count,
             (WP_U32)filter_stat[FILTER_SET_A_FORWARDING].matched_pkts,
             expected_a_forwarding_match_count,
             (WP_U32)filter_stat[FILTER_SET_A_FORWARDING].no_matched_pkts, 
             expected_a_forwarding_no_match_count);

      terminate("PCE Filter statistics are not matched",err_line);
   }
}

void CLI_SendAndRecievePacket(void)
{
   WP_U32 ii;

   /* send data */
   for(ii=0; ii < NUM_OF_FLOWS; ii++)
   {
      WPE_HostSendPacket(FILTER_SET_A, ii);
   }
   
   /* Check recieved data */
   for(ii=0; ii < NUM_OF_FLOWS; ii++)
   {
      WPE_CheckReceivedTraffic(ii);         
   }
}

void CLI_CheckLearningEvents(WP_U32 expected_edp, WP_U32 expected_learnt_count, WP_U32 err_line)
{
   WP_pce_rule_forwarding rule_fwd = {0};
   WP_handle status;
   WP_U32 rules_returned = 0;
   WP_U32 ii;


   status = WP_PceLearningQueueService(WP_WINPATH(DEFAULT_WPID), &(learned_rules_list[0]), 100, &rules_returned);
   terminate_on_error(status, "WP_PceLearningQueueService",err_line);

//   printf("learnt %d rules\n", rules_returned);

   if (rules_returned != expected_learnt_count)
      terminate("Error in number of Learnt rules",err_line);
   
   status = WP_PceFilterStatistics (PCE_filter[FILTER_SET_A_LEARNING], &filter_stat[FILTER_SET_A_LEARNING]);
   terminate_on_error(status, "WP_PceFilterStatistics()",err_line);

//    printf("no match %d %d\n", (WP_U32)filter_stat[FILTER_SET_A_LEARNING].no_matched_pkts, 
//           (WP_U32)previous_filter_stat[FILTER_SET_A_LEARNING].no_matched_pkts);


   if ((WP_U32)filter_stat[FILTER_SET_A_LEARNING].no_matched_pkts != 
       ((WP_U32)previous_filter_stat[FILTER_SET_A_LEARNING].no_matched_pkts + expected_learnt_count))
      terminate("PCE Learning filter Stats error",err_line);

   for (ii =0; ii<rules_returned; ii++)
   {
      memcpy(&rule_fwd, &(learned_rules_list[ii].fwd_rule), sizeof(WP_pce_rule_forwarding));
    
      if (rule_fwd.filter_handle != PCE_filter[FILTER_SET_A_FORWARDING])
         terminate("Error - IW port not matched",err_line);
      
      if (expected_edp && 
          learned_rules_list[ii].params.edp != 1)
         terminate("Error - Not EDP",err_line);
   }
}

void CLI_RunCreateFilterSetAForwardingRules(void)
{
   WP_pce_rule_forwarding rule_fwd = {0};
   WP_U32 rule_index, ii, i;
   
   /* Fill the PCE with HW rules until left free only NUM_OF_RULES_PER_FILTER_SET/2 */
   if (TestType == TEST_HYBRID_FDB)
      WPE_CreateDummyPceHwRules(NUM_OF_RULES_PER_FILTER_SET/2);

    /* Create forwarding rules for filter set A */
   for(i=0; i<NUM_OF_RULES_PER_FILTER_SET; i++)
   {
      rule_fwd.enabled = WP_ENABLE;
      rule_fwd.filter_handle = PCE_filter[FILTER_SET_A_FORWARDING];
      rule_index = RULES_A_FORWARDING_START + i;
      
      rule_fwd.rule_fields[0].field_id = WP_PCE_FIELD_ID_MAC_DA;
      for(ii=0; ii<5; ii++)
         rule_fwd.rule_fields[0].value.mac_addr[ii] = start_mac_addr[ii]; 
      rule_fwd.rule_fields[0].value.mac_addr[5] = i; 

      rule_fwd.rule_fields[1].field_id = WP_PCE_FIELD_ID_LAST;
      
      rule_fwd.match_result[0].result_type = WP_PCE_RESULT_OUT_IW_PORT_UPDATE;
      rule_fwd.match_result[0].param.iw_port.iw_port_handle = h_iw_port_gbe[0]; 
      
      rule_fwd.match_result[1].result_type = WP_PCE_RESULT_AGING;
      if (i < (NUM_OF_RULES_PER_FILTER_SET/2) + 2 &&
          i > (NUM_OF_RULES_PER_FILTER_SET/2) - 2)
      {
         rule_fwd.match_result[1].param.aging.mode = WP_PCE_AGING_DISABLE;
      }
      else
      {
         rule_fwd.match_result[1].param.aging.mode = WP_PCE_AGING_ENABLE;
         rule_fwd.match_result[1].param.aging.weight = 2;      
      }

      rule_fwd.match_result[2].result_type = WP_PCE_RESULT_PCE_INTERFACE;
      rule_fwd.match_result[2].param.pce_if_iw_next_round.pce_interface = pce_dl_second_round_if_handle;

      rule_fwd.match_result[3].result_type = WP_PCE_RESULT_LAST;
      
      rule_fwd.match_action = WP_PCE_RULE_MATCH_CONTINUE;
      
      PCE_rule_handle[rule_index] = WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID),
                                                     WP_PCE_RULE_FORWARDING,
                                                     &rule_fwd);
      terminate_on_error(PCE_rule_handle[rule_index], "WP_PceRuleCreate",__LINE__);
   }  

   if (TestType == TEST_HYBRID_FDB)
   {
      for(i=0; i<NUM_OF_RULES_PER_FILTER_SET/2; i++)
         free_pce_hw_rules--;

      WPE_DeleteDummyPceHwRules();
   }
}

void CLI_RunRulesearch(void)
{
   WP_pce_rule_forwarding rule_fwd = {0};
   WP_U32 ii, i;
   WP_handle rule_handle;
   
    /* Create forwarding rules for filter set A */
   for(i=0; i<NUM_OF_RULES_PER_FILTER_SET; i++)
   {
      rule_fwd.filter_handle = PCE_filter[FILTER_SET_A_FORWARDING];
      rule_fwd.rule_fields[0].field_id = WP_PCE_FIELD_ID_MAC_DA;
      for(ii=0; ii<5; ii++)
         rule_fwd.rule_fields[0].value.mac_addr[ii] = start_mac_addr[ii]; 
      rule_fwd.rule_fields[0].value.mac_addr[5] = i; 
      
      rule_fwd.rule_fields[1].field_id = WP_PCE_FIELD_ID_LAST;
      
      rule_handle = WP_PceObjectSearch (WP_WINPATH(DEFAULT_WPID),
                                        WP_PCE_OBJECT_TYPE_RULE_FORWARDING,
                                        &rule_fwd);
      terminate_on_error(rule_handle,"WP_PceObjectSearch",__LINE__);        

      if (rule_handle != PCE_rule_handle[RULES_A_FORWARDING_START + i])
         terminate("Wrong Pce Rule returned",__LINE__);
   }

   printf("\nTest Rule Search Pass\n");
}


void CLI_PceIteratorGetNextRule(void)
{
   WP_handle rule_handle;

   while(1)
   {
      rule_handle = WP_PceIteratorGetNext(rule_iterator_handle);
      if (WP_ERROR(rule_handle) == WP_ERR_PCE_ITERATOR_NO_ITEMS)
      {
         break;
      }
      
//      WP_Display(WP_WINPATH(DEFAULT_WPID), rule_handle, 0, 0);

      iter_rules++;
   }
}

void CLI_PceIteratorGetFirstRule(void)
{
   WP_handle rule_handle;
   
   rule_handle = WP_PceIteratorGetFirst(rule_iterator_handle);
   if (WP_ERROR(rule_handle) != WP_ERR_PCE_ITERATOR_NO_ITEMS)
   {
//      WP_Display(WP_WINPATH(DEFAULT_WPID), rule_handle, 0, 0);

      iter_rules++;
   }
}

void CLI_RunRuleIertationScenarios(void)
{
   WP_handle status;
   WP_U32 ii, i;
   WP_pce_rule_forwarding rule_fwd = {0};
   WP_U32 rule_index;
   

   rule_iterator_handle = WP_PceIteratorCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_ITERATOR_TYPE_RULE, (void *)&PCE_filter[FILTER_SET_A_FORWARDING]);
   terminate_on_error(rule_iterator_handle,"WP_PceIteratorCreate",__LINE__);        
   
   iter_rules = 0;

   CLI_PceIteratorGetFirstRule();

   CLI_PceIteratorGetNextRule();

   if (iter_rules != (NUM_OF_RULES_PER_FILTER_SET))
      terminate("Wrong number of forwarding rules",__LINE__);


/*----------rule iterator after rule deletion--------------------*/

   iter_rules = 0;
   
   CLI_PceIteratorGetFirstRule();

   status = WP_PceRuleDelete(PCE_rule_handle[RULES_A_FORWARDING_START+4]);
   terminate_on_error(status, "WP_PceRuleDelete",__LINE__);

   status = WP_PceRuleDelete(PCE_rule_handle[RULES_A_FORWARDING_START+5]);
   terminate_on_error(status, "WP_PceRuleDelete",__LINE__);

   status = WP_PceRuleDelete(PCE_rule_handle[RULES_A_FORWARDING_START+6]);
   terminate_on_error(status, "WP_PceRuleDelete",__LINE__);

   CLI_PceIteratorGetNextRule();

   if (iter_rules != (NUM_OF_RULES_PER_FILTER_SET-3))
      terminate("Wrong number of forwarding rules",__LINE__);

/*----------rule iterator after rule deletion--------------------*/

   iter_rules = 0;

   CLI_PceIteratorGetFirstRule();

   /* Delete forwarding rules */
   for(ii=7; ii<NUM_OF_RULES_PER_FILTER_SET; ii++)
   {
      status = WP_PceRuleDelete(PCE_rule_handle[RULES_A_FORWARDING_START+ii]);
      terminate_on_error(status, "WP_PceRuleDelete",__LINE__);
   }

   CLI_PceIteratorGetNextRule();

   if (iter_rules != (4))
      terminate("Wrong number of forwarding rules",__LINE__);

/*----------rule iterator after rule deletion--------------------*/
  
   iter_rules = 0;

   /* Delete forwarding rules */
   for(ii=0; ii<4; ii++)
   {
      status = WP_PceRuleDelete(PCE_rule_handle[RULES_A_FORWARDING_START+ii]);
      terminate_on_error(status, "WP_PceRuleDelete",__LINE__);
   }

   CLI_PceIteratorGetFirstRule();

   if (iter_rules != 0)
      terminate("Wrong number of forwarding rules",__LINE__);


   status = WP_PceIteratorDelete(rule_iterator_handle);
   terminate_on_error(status, "WP_PceIteratorDelete",__LINE__);

   /* Restore the forwarding rules */
   /* Create forwarding rules for filter set A */
   for(i=0; i<NUM_OF_RULES_PER_FILTER_SET; i++)
   {
      rule_fwd.enabled = WP_ENABLE;
      rule_fwd.filter_handle = PCE_filter[FILTER_SET_A_FORWARDING];
      rule_index = RULES_A_FORWARDING_START + i;
      
      rule_fwd.rule_fields[0].field_id = WP_PCE_FIELD_ID_MAC_DA;
      for(ii=0; ii<5; ii++)
         rule_fwd.rule_fields[0].value.mac_addr[ii] = start_mac_addr[ii]; 
      rule_fwd.rule_fields[0].value.mac_addr[5] = i; 

      rule_fwd.rule_fields[1].field_id = WP_PCE_FIELD_ID_LAST;
      
      rule_fwd.match_result[0].result_type = WP_PCE_RESULT_OUT_IW_PORT_UPDATE;
      rule_fwd.match_result[0].param.iw_port.iw_port_handle = h_iw_output_port_b; 
      
      rule_fwd.match_result[1].result_type = WP_PCE_RESULT_AGING;
      rule_fwd.match_result[1].param.aging.mode = WP_PCE_AGING_DISABLE;
      
      rule_fwd.match_result[2].result_type = WP_PCE_RESULT_PCE_INTERFACE;
      rule_fwd.match_result[2].param.pce_if_iw_next_round.pce_interface = pce_dl_second_round_if_handle;

      rule_fwd.match_result[3].result_type = WP_PCE_RESULT_LAST;
      
      rule_fwd.match_action = WP_PCE_RULE_MATCH_CONTINUE;
      
      PCE_rule_handle[rule_index] = WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID),
                                                     WP_PCE_RULE_FORWARDING,
                                                     &rule_fwd);
      terminate_on_error(PCE_rule_handle[rule_index], "WP_PceRuleCreate",__LINE__);
   }

   printf("\nRule Iterator Scenarios Pass\n");
}

void CLI_PceIteratorGetNextAgedRule(void)
{
   WP_handle rule_handle;

   while(1)
   {
      rule_handle = WP_PceIteratorGetNext(aging_iterator_handle);
      if (WP_ERROR(rule_handle) == WP_ERR_PCE_ITERATOR_NO_ITEMS)
      {
         break;
      }
      
      aged_rules++;
   }
}

void CLI_PceIteratorGetFirstAgedRule(void)
{
   WP_handle rule_handle;
   
   rule_handle = WP_PceIteratorGetFirst(aging_iterator_handle);
   if (WP_ERROR(rule_handle) != WP_ERR_PCE_ITERATOR_NO_ITEMS)
   {
      aged_rules++;
   }
}

void CLI_RunAgingScenarios(void)
{
   WP_U32 ii;
   WP_handle status;

#if 0
   WP_U32 rule_index;
   
   /* Delete forwarding rules */
   for(ii=0; ii<NUM_OF_RULES_PER_FILTER_SET; ii++)
   {
      rule_index = RULES_A_FORWARDING_START + ii;
      
      status = WP_PceRuleDelete(PCE_rule_handle[rule_index]);
      terminate_on_error(status, "WP_PceRuleDelete",__LINE__);
    }
#endif

   /* Create Forwarding rules with Aging */
   CLI_RunCreateFilterSetAForwardingRules();

   aging_iterator_handle = WP_PceIteratorCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_ITERATOR_TYPE_AGED_RULE, (void *)&PCE_filter[FILTER_SET_A_FORWARDING]);
   terminate_on_error(aging_iterator_handle,"WP_PceIteratorCreate",__LINE__);

/*------------Aged all rules (with aging enable)----------*/

   aged_rules = 0;
   
   /* Get first aged rule */
   CLI_PceIteratorGetFirstAgedRule();

   /* Get all the rest of aged rule */
   CLI_PceIteratorGetNextAgedRule();

   /* Get first aged rule */
   CLI_PceIteratorGetFirstAgedRule();

   /* Get all the rest of aged rule */
   CLI_PceIteratorGetNextAgedRule();

   /* Get first aged rule */
   CLI_PceIteratorGetFirstAgedRule();
   
   /* Get all the rest of aged rule */
   CLI_PceIteratorGetNextAgedRule();

   if (aged_rules != (NUM_OF_RULES_PER_FILTER_SET - 3))
      terminate("Wrong number of aged rules",__LINE__);
       

/*--------------After aging update only part of the rules are aged-----*/
   aged_rules = 0;
   
   /* Send packets */
   for(ii=(NUM_OF_FLOWS/2); ii < NUM_OF_FLOWS; ii++)
   {
      WPE_HostSendPacket(FILTER_SET_A, ii);
   }

   /* Get first aged rule */
   CLI_PceIteratorGetFirstAgedRule();
   
   /* Get all the rest of aged rule */
   CLI_PceIteratorGetNextAgedRule();

  if (aged_rules != (NUM_OF_RULES_PER_FILTER_SET/2 - 1))
      terminate("Wrong number of aged rules",__LINE__);
  
  
/*------------Aged all rules (with aging enable)----------*/
  
   /* Get first aged rule */
   CLI_PceIteratorGetFirstAgedRule();

   /* Get all the rest of aged rule */
   CLI_PceIteratorGetNextAgedRule();

   aged_rules = 0;

   /* Get first aged rule */
   /* At this point all the rules should be aged */
   CLI_PceIteratorGetFirstAgedRule();
   
   /* Get all the rest of aged rule */
   CLI_PceIteratorGetNextAgedRule();

  if (aged_rules != (NUM_OF_RULES_PER_FILTER_SET - 3))
     terminate("Wrong number of aged rules",__LINE__);


  status = WP_PceIteratorDelete(aging_iterator_handle);
  terminate_on_error(status, "WP_PceIteratorDelete",__LINE__);

  printf("\nAging Scenarios Pass\n");
}

void CLI_RunLearningScenarios(void)
{
   WP_U32 i, ii, rule_index;
   WP_pce_rule_forwarding rule_fwd = {0};
   WP_handle status;
   WP_U32 rules_returned = 0;
   
   /* Clean learning queue */
   status = WP_PceLearningQueueService(WP_WINPATH(DEFAULT_WPID), &(learned_rules_list[0]), 100, &rules_returned);
   terminate_on_error(status, "WP_PceLearningQueueService",__LINE__);

   status = WP_PceFilterStatistics (PCE_filter[FILTER_SET_A_LEARNING], &filter_stat[FILTER_SET_A_LEARNING]);
   terminate_on_error(status, "WP_PceFilterStatistics()",__LINE__);

   memcpy(previous_filter_stat, filter_stat, sizeof(WP_pce_filter_stat)*100);

   /* Modify Rule Result */
   rule_fwd.enabled = WP_ENABLE;
   rule_fwd.match_result[0].result_type = WP_PCE_RESULT_OUT_IW_PORT_UPDATE;
   rule_fwd.match_result[0].param.iw_port.iw_port_handle = h_iw_port_gbe[1]; 
   
   rule_fwd.match_result[1].result_type = WP_PCE_RESULT_AGING;
   rule_fwd.match_result[1].param.aging.mode = WP_PCE_AGING_DISABLE;
   
   rule_fwd.match_result[2].result_type = WP_PCE_RESULT_LAST;
   
   for(i=0; i<NUM_OF_FLOWS; i++)
   {
      status = WP_PceRuleModify(PCE_rule_handle[RULES_A_FORWARDING_START+i],
                                WP_PCE_RULE_MODIFY_COMMAND_RESULT,
                                &rule_fwd);
      terminate_on_error(status, "WP_PceRuleModify",__LINE__);
   }

   /* Send packets */
   for(ii=0; ii < NUM_OF_FLOWS; ii++)
   {
      WPE_HostSendPacket(FILTER_SET_A, ii);
   }

   /* Check learning EDP */
   CLI_CheckLearningEvents(WP_TRUE, NUM_OF_FLOWS, __LINE__);   

/*---------------------------------------------------*/   

   memcpy(previous_filter_stat, filter_stat, sizeof(WP_pce_filter_stat)*100);
   
   /* Modify Rule Result */
   rule_fwd.match_result[0].result_type = WP_PCE_RESULT_OUT_IW_PORT_UPDATE;
   rule_fwd.match_result[0].param.iw_port.iw_port_handle = h_iw_port_gbe[0]; 
   
   rule_fwd.match_result[1].result_type = WP_PCE_RESULT_AGING;
   rule_fwd.match_result[1].param.aging.mode = WP_PCE_AGING_DISABLE;
   
   rule_fwd.match_result[2].result_type = WP_PCE_RESULT_LAST;
   
   for(i=0; i<NUM_OF_FLOWS; i++)
   {
      status = WP_PceRuleModify(PCE_rule_handle[RULES_A_FORWARDING_START+i],
                                WP_PCE_RULE_MODIFY_COMMAND_RESULT,
                                &rule_fwd);
      terminate_on_error(status, "WP_PceRuleModify",__LINE__);
   }


   /* Send packets */
   for(ii=0; ii < NUM_OF_FLOWS; ii++)
   {
      WPE_HostSendPacket(FILTER_SET_A, ii);
   }

   /* learning match */
   CLI_CheckLearningEvents(WP_FALSE, 0, __LINE__);   
      
/*---------------------------------------------------*/   

   memcpy(previous_filter_stat, filter_stat, sizeof(WP_pce_filter_stat)*100);
     
   /* Send packets */
   for(ii=NUM_OF_FLOWS; ii < 2*NUM_OF_FLOWS; ii++)
   {
      WPE_HostSendPacket(FILTER_SET_A, ii);
   }

   status = WP_PceLearningQueueService(WP_WINPATH(DEFAULT_WPID), &(learned_rules_list[0]), 100, &rules_returned);
   terminate_on_error(status, "WP_PceLearningQueueService",__LINE__);

   //printf("learnt %d rules\n", rules_returned);

   status = WP_PceFilterStatistics (PCE_filter[FILTER_SET_A_LEARNING], &filter_stat[FILTER_SET_A_LEARNING]);
   terminate_on_error(status, "WP_PceFilterStatistics()",__LINE__);

//    printf("no match %d %d\n", (WP_U32)filter_stat[FILTER_SET_A_LEARNING].no_matched_pkts, 
//           (WP_U32)previous_filter_stat[FILTER_SET_A_LEARNING].no_matched_pkts + NUM_OF_FLOWS);

   if ((WP_U32)filter_stat[FILTER_SET_A_LEARNING].no_matched_pkts != 
       (((WP_U32)previous_filter_stat[FILTER_SET_A_LEARNING].no_matched_pkts) + NUM_OF_FLOWS))
      terminate("PCE Learning filter Stats error",__LINE__);

   /* Max Max - no learning */
   if (rules_returned)
      terminate("Error in Max Mac - no learning event should be send",__LINE__);

/*---------------------------------------------------*/   
   
   rule_fwd.enabled = WP_ENABLE;
   rule_fwd.filter_handle = PCE_filter[FILTER_SET_A_FORWARDING];
   rule_index = RULES_A_FORWARDING_START + 0x20;
   
   rule_fwd.rule_fields[0].field_id = WP_PCE_FIELD_ID_MAC_DA;
   for(ii=0; ii<5; ii++)
      rule_fwd.rule_fields[0].value.mac_addr[ii] = start_mac_addr[ii]; 
   rule_fwd.rule_fields[0].value.mac_addr[5] = i; 
   
   rule_fwd.rule_fields[1].field_id = WP_PCE_FIELD_ID_LAST;
   
   rule_fwd.match_result[0].result_type = WP_PCE_RESULT_OUT_IW_PORT_UPDATE;
   rule_fwd.match_result[0].param.iw_port.iw_port_handle = h_iw_port_gbe[0]; 
   
   rule_fwd.match_result[1].result_type = WP_PCE_RESULT_AGING;
   rule_fwd.match_result[1].param.aging.mode = WP_PCE_AGING_DISABLE;
   
   rule_fwd.match_result[2].result_type = WP_PCE_RESULT_LAST;
   
   rule_fwd.match_action = WP_PCE_RULE_MATCH_CONTINUE;
   
   PCE_rule_handle[rule_index] = WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID),
                                                  WP_PCE_RULE_FORWARDING,
                                                  &rule_fwd);
   check_error(PCE_rule_handle[rule_index], "WP_PceRuleCreate()" ,__LINE__, WP_ERR_BPORT_MAX_MAC);

   printf("\nTest Max Mac Pass\n");

/*---------------------------------------------------*/   

   memcpy(previous_filter_stat, filter_stat, sizeof(WP_pce_filter_stat)*100);
   
   /* Delete the rule */
   for(i=0; i<NUM_OF_FLOWS; i++)
   {
      status = WP_PceRuleDelete(PCE_rule_handle[RULES_A_FORWARDING_START+i]);
      terminate_on_error(status, "WP_PceRuleDelete()",__LINE__);
   }

   if (TestType == TEST_HYBRID_FDB)
   {
      for(i=0; i<NUM_OF_RULES_PER_FILTER_SET/2; i++)
         free_pce_hw_rules++;
   }

   /* Send packets */
   for(ii=0; ii < NUM_OF_FLOWS; ii++)
   {
      WPE_HostSendPacket(FILTER_SET_A, ii);
   }

   /* Check learning no EDP */
   CLI_CheckLearningEvents(WP_FALSE, NUM_OF_FLOWS, __LINE__);   

/*---------------------------------------------------*/   

   printf("\nLearning Scenarios Pass\n");
}

void CLI_RunRuleScenarios(void)
{
   WP_pce_rule_forwarding rule_fwd = {0};
   WP_pce_rule_classification rule_cfg = {0};
   WP_handle status;
   WP_U32 i, ii, excpected_class_fa_count, expected_default_fa_count;
   WP_pce_filter_forwarding fwd_filter_cfg = {0};
   
   memcpy(previous_filter_stat, filter_stat, sizeof(WP_pce_filter_stat)*100);

   rule_fwd.enabled = WP_ENABLE;
   rule_fwd.filter_handle = PCE_filter[FILTER_SET_A_FORWARDING];

   excpected_class_fa_count = WPT_CompatibleFaGetFwPacketsCount(flow_agg[FILTER_SET_B_CLASSIFICATION_FLOW_AGG]);
   expected_default_fa_count = WPT_CompatibleFaGetFwPacketsCount(default_agg);

   /* Send and recieve packet */
   CLI_SendAndRecievePacket();

   /* check statistics */
   CLI_CheckStatisticsMatched(excpected_class_fa_count+NUM_OF_FLOWS,
                              expected_default_fa_count, 
                              previous_filter_stat[FILTER_SET_B_CLASSIFICATION].matched_pkts + NUM_OF_FLOWS,
                              previous_filter_stat[FILTER_SET_A_FORWARDING].matched_pkts +  NUM_OF_FLOWS,
                              previous_filter_stat[FILTER_SET_A_FORWARDING].no_matched_pkts, __LINE__);

/*---------------------------------------------------*/   

   memcpy(previous_filter_stat, filter_stat, sizeof(WP_pce_filter_stat)*100);

   /* Disable the rule */
   for(i=0; i<NUM_OF_FLOWS; i++)
   {
      status = WP_PceRuleDisable(PCE_rule_handle[RULES_A_FORWARDING_START+i]);
      terminate_on_error(status, "WP_PceRuleDisable()",__LINE__);
   }
   
   /* Send and recieve packet */
   CLI_SendAndRecievePacket();
   
   /* check statistics */
   CLI_CheckStatisticsMatched(excpected_class_fa_count+NUM_OF_FLOWS,
                              expected_default_fa_count+NUM_OF_FLOWS, 
                              previous_filter_stat[FILTER_SET_B_CLASSIFICATION].matched_pkts,
                              previous_filter_stat[FILTER_SET_A_FORWARDING].matched_pkts,
                              previous_filter_stat[FILTER_SET_A_FORWARDING].no_matched_pkts +  NUM_OF_FLOWS,
                              __LINE__);

/*---------------------------------------------------*/   

   memcpy(previous_filter_stat, filter_stat, sizeof(WP_pce_filter_stat)*100);

   /* Enable the rule */
   for(i=0; i<NUM_OF_FLOWS; i++)
   {
      status = WP_PceRuleEnable(PCE_rule_handle[RULES_A_FORWARDING_START+i]);
      terminate_on_error(status, "WP_PceRuleEnable()",__LINE__);
   }
   
   /* Send and recieve packet */
   CLI_SendAndRecievePacket();
   
   /* check statistics */
   CLI_CheckStatisticsMatched(excpected_class_fa_count+(2*NUM_OF_FLOWS),
                              expected_default_fa_count+NUM_OF_FLOWS, 
                              previous_filter_stat[FILTER_SET_B_CLASSIFICATION].matched_pkts + NUM_OF_FLOWS,
                              previous_filter_stat[FILTER_SET_A_FORWARDING].matched_pkts +  NUM_OF_FLOWS,
                              previous_filter_stat[FILTER_SET_A_FORWARDING].no_matched_pkts,
                              __LINE__);

/*---------------------------------------------------*/   

   memcpy(previous_filter_stat, filter_stat, sizeof(WP_pce_filter_stat)*100);

   /* Modify Rule Result */
   rule_fwd.match_result[0].result_type = WP_PCE_RESULT_OUT_IW_PORT_UPDATE;
   rule_fwd.match_result[0].param.iw_port.iw_port_handle = h_iw_output_port_b; 
   
   rule_fwd.match_result[1].result_type = WP_PCE_RESULT_AGING;
   rule_fwd.match_result[1].param.aging.mode = WP_PCE_AGING_DISABLE;
   
   rule_fwd.match_result[2].result_type = WP_PCE_RESULT_FLOW_AGG;
   rule_fwd.match_result[2].param.flow_agg.flow_aggregation = flow_agg[FILTER_SET_B_FORWARDING_FLOW_AGG];
   
   rule_fwd.match_result[3].result_type = WP_PCE_RESULT_LAST;
   
   for(i=0; i<NUM_OF_FLOWS; i++)
   {
      status = WP_PceRuleModify(PCE_rule_handle[RULES_A_FORWARDING_START+i],
                                WP_PCE_RULE_MODIFY_COMMAND_RESULT,
                                &rule_fwd);
      terminate_on_error(status, "WP_PceRuleModify",__LINE__);
   }

   /* Send and recieve packet */
   CLI_SendAndRecievePacket();
   
   /* check statistics */
   CLI_CheckStatisticsMatched(excpected_class_fa_count+(2*NUM_OF_FLOWS),
                              expected_default_fa_count+NUM_OF_FLOWS, 
                              previous_filter_stat[FILTER_SET_B_CLASSIFICATION].matched_pkts,
                              previous_filter_stat[FILTER_SET_A_FORWARDING].matched_pkts +  NUM_OF_FLOWS,
                              previous_filter_stat[FILTER_SET_A_FORWARDING].no_matched_pkts,
                              __LINE__);

/*---------------------------------------------------*/   

   memcpy(previous_filter_stat, filter_stat, sizeof(WP_pce_filter_stat)*100);

   /* Modify Rule Result */
   rule_fwd.match_result[0].result_type = WP_PCE_RESULT_OUT_IW_PORT_UPDATE;
   rule_fwd.match_result[0].param.iw_port.iw_port_handle = h_iw_output_port; 
   
   rule_fwd.match_result[1].result_type = WP_PCE_RESULT_AGING;
   rule_fwd.match_result[1].param.aging.mode = WP_PCE_AGING_DISABLE;
   
   rule_fwd.match_result[2].result_type = WP_PCE_RESULT_PCE_INTERFACE;
   rule_fwd.match_result[2].param.pce_if_iw_next_round.pce_interface = pce_dl_second_round_if_handle;

   rule_fwd.match_result[3].result_type = WP_PCE_RESULT_LAST;
   
   for(i=0; i<NUM_OF_FLOWS; i++)
   {
      status = WP_PceRuleModify(PCE_rule_handle[RULES_A_FORWARDING_START+i],
                                WP_PCE_RULE_MODIFY_COMMAND_RESULT,
                                &rule_fwd);
      terminate_on_error(status, "WP_PceRuleModify",__LINE__);
   }
   
   /* Send and recieve packet */
   CLI_SendAndRecievePacket();
   
   /* check statistics */
   CLI_CheckStatisticsMatched(excpected_class_fa_count+(2*NUM_OF_FLOWS),
                              expected_default_fa_count+NUM_OF_FLOWS, 
                              previous_filter_stat[FILTER_SET_B_CLASSIFICATION].matched_pkts,
                              previous_filter_stat[FILTER_SET_A_FORWARDING].matched_pkts +  NUM_OF_FLOWS,
                              previous_filter_stat[FILTER_SET_A_FORWARDING].no_matched_pkts,
                              __LINE__);

/*---------------------------------------------------*/   

   memcpy(previous_filter_stat, filter_stat, sizeof(WP_pce_filter_stat)*100);

   /* Modify Rule Result */
   rule_fwd.match_result[0].result_type = WP_PCE_RESULT_OUT_IW_PORT_UPDATE;
   rule_fwd.match_result[0].param.iw_port.iw_port_handle = h_iw_output_port_b; 
   
   rule_fwd.match_result[1].result_type = WP_PCE_RESULT_AGING;
   rule_fwd.match_result[1].param.aging.mode = WP_PCE_AGING_DISABLE;
   
   rule_fwd.match_result[2].result_type = WP_PCE_RESULT_PCE_INTERFACE;
   rule_fwd.match_result[2].param.pce_if_iw_next_round.pce_interface = pce_dl_second_round_if_handle;

   rule_fwd.match_result[3].result_type = WP_PCE_RESULT_LAST;
   
   for(i=0; i<NUM_OF_FLOWS; i++)
   {
      status = WP_PceRuleModify(PCE_rule_handle[RULES_A_FORWARDING_START+i],
                                WP_PCE_RULE_MODIFY_COMMAND_RESULT,
                                &rule_fwd);
      terminate_on_error(status, "WP_PceRuleModify",__LINE__);
   }
   
   /* Send and recieve packet */
   CLI_SendAndRecievePacket();
   
   /* check statistics */
   CLI_CheckStatisticsMatched(excpected_class_fa_count+(3*NUM_OF_FLOWS),
                              expected_default_fa_count+NUM_OF_FLOWS, 
                              previous_filter_stat[FILTER_SET_B_CLASSIFICATION].matched_pkts + NUM_OF_FLOWS,
                              previous_filter_stat[FILTER_SET_A_FORWARDING].matched_pkts +  NUM_OF_FLOWS,
                              previous_filter_stat[FILTER_SET_A_FORWARDING].no_matched_pkts,
                              __LINE__);

/*---------------------------------------------------*/   

   memcpy(previous_filter_stat, filter_stat, sizeof(WP_pce_filter_stat)*100);

   /* change rule to match deny */
   rule_fwd.match_action = WP_PCE_RULE_MATCH_DENY;
   for(i=0; i<NUM_OF_FLOWS; i++)
   {
      status = WP_PceRuleModify(PCE_rule_handle[RULES_A_FORWARDING_START+i],
                                WP_PCE_RULE_MODIFY_COMMAND_MATCH_ACTION,
                                &rule_fwd);
      terminate_on_error(status, "WP_PceRuleModify",__LINE__);
   }

   /* send data */
   for(ii=0; ii < NUM_OF_FLOWS; ii++)
   {
      WPE_HostSendPacket(FILTER_SET_A, ii);
   }
   
   /* check for packet drop */
   if(WPT_HostChannelQueueSizeX(rx_host_handle))
      terminate("Rx Host Channel wrong Size",__LINE__);

   /* Check recieved data */
   for(ii=0; ii < NUM_OF_FLOWS; ii++)
   {
      WPE_CheckReceivedTraffic(ii);         
   }

   /* check statistics */
   CLI_CheckStatisticsMatched(excpected_class_fa_count+(3*NUM_OF_FLOWS),
                              expected_default_fa_count+NUM_OF_FLOWS, 
                              previous_filter_stat[FILTER_SET_B_CLASSIFICATION].matched_pkts,
                              previous_filter_stat[FILTER_SET_A_FORWARDING].matched_pkts +  NUM_OF_FLOWS,
                              previous_filter_stat[FILTER_SET_A_FORWARDING].no_matched_pkts,
                              __LINE__);

/*---------------------------------------------------*/   

   memcpy(previous_filter_stat, filter_stat, sizeof(WP_pce_filter_stat)*100);

  /* change rule to match accept */
   rule_fwd.match_result[0].result_type = WP_PCE_RESULT_OUT_IW_PORT_UPDATE;
   rule_fwd.match_result[0].param.iw_port.iw_port_handle = h_iw_output_port; 
   
   rule_fwd.match_result[1].result_type = WP_PCE_RESULT_AGING;
   rule_fwd.match_result[1].param.aging.mode = WP_PCE_AGING_DISABLE;
   
   rule_fwd.match_result[2].result_type = WP_PCE_RESULT_PCE_INTERFACE;
   rule_fwd.match_result[2].param.pce_if_iw_next_round.pce_interface = pce_dl_second_round_if_handle;

   rule_fwd.match_result[3].result_type = WP_PCE_RESULT_LAST;
   
   rule_fwd.match_action = WP_PCE_RULE_MATCH_ACCEPT;
   for(i=0; i<NUM_OF_FLOWS; i++)
   {
      status = WP_PceRuleModify(PCE_rule_handle[RULES_A_FORWARDING_START+i],
                                WP_PCE_RULE_MODIFY_COMMAND_MATCH_ACTION |
                                WP_PCE_RULE_MODIFY_COMMAND_RESULT,
                                &rule_fwd);
      terminate_on_error(status, "WP_PceRuleModify",__LINE__);
   }
   
   /* send data */
   for(ii=0; ii < NUM_OF_FLOWS; ii++)
   {
      WPE_HostSendPacket(FILTER_SET_A, ii);
   }

   /* check for packet drop */
   if(WPT_HostChannelQueueSizeX(rx_host_handle) != NUM_OF_FLOWS)
      terminate("Rx Host Channel wrong Size",__LINE__);

   /* check statistics */
   CLI_CheckStatisticsMatched(excpected_class_fa_count+(3*NUM_OF_FLOWS),
                              expected_default_fa_count+NUM_OF_FLOWS, 
                              previous_filter_stat[FILTER_SET_B_CLASSIFICATION].matched_pkts,
                              previous_filter_stat[FILTER_SET_A_FORWARDING].matched_pkts + NUM_OF_FLOWS,
                              previous_filter_stat[FILTER_SET_A_FORWARDING].no_matched_pkts,
                              __LINE__);

/*---------------------------------------------------*/   

   memcpy(previous_filter_stat, filter_stat, sizeof(WP_pce_filter_stat)*100);

   /* Delete the rule */
   for(i=0; i<NUM_OF_FLOWS; i++)
   {
      status = WP_PceRuleDelete(PCE_rule_handle[RULES_A_FORWARDING_START+i]);
      terminate_on_error(status, "WP_PceRuleDelete()",__LINE__);
   }
   
   if (TestType == TEST_HYBRID_FDB)
   {
      for(i=0; i<NUM_OF_FLOWS/2; i++)
         free_pce_hw_rules++;

      /* Fill the PCE with HW rules until left free only NUM_OF_RULES_PER_FILTER_SET/2 */
      WPE_CreateDummyPceHwRules(NUM_OF_FLOWS/2);
   }

   /* Send and recieve packet */
   CLI_SendAndRecievePacket();
   
   /* check statistics */
   CLI_CheckStatisticsMatched(excpected_class_fa_count+(3*NUM_OF_FLOWS),
                              expected_default_fa_count+(2*NUM_OF_FLOWS), 
                              previous_filter_stat[FILTER_SET_B_CLASSIFICATION].matched_pkts,
                              previous_filter_stat[FILTER_SET_A_FORWARDING].matched_pkts,
                              previous_filter_stat[FILTER_SET_A_FORWARDING].no_matched_pkts +  NUM_OF_FLOWS,
                              __LINE__);

   
   /* Create the rule again with bridge loop */
   rule_fwd.rule_fields[0].field_id = WP_PCE_FIELD_ID_MAC_DA;
   for(ii=0; ii<5; ii++)
      rule_fwd.rule_fields[0].value.mac_addr[ii] = start_mac_addr[ii]; 
   
   rule_fwd.rule_fields[1].field_id = WP_PCE_FIELD_ID_LAST;
   
   rule_fwd.match_result[0].result_type = WP_PCE_RESULT_OUT_IW_PORT_UPDATE;
   
   rule_fwd.match_result[1].result_type = WP_PCE_RESULT_AGING;
   rule_fwd.match_result[1].param.aging.mode = WP_PCE_AGING_DISABLE;
   
   rule_fwd.match_result[2].result_type = WP_PCE_RESULT_PCE_INTERFACE;
   rule_fwd.match_result[2].param.pce_if_iw_next_round.pce_interface = pce_dl_second_round_if_handle;
   
   rule_fwd.match_result[3].result_type = WP_PCE_RESULT_LAST;
   
   rule_fwd.match_action = WP_PCE_RULE_MATCH_CONTINUE;
   
   for(i=0; i<NUM_OF_FLOWS; i++)
   {
      rule_fwd.match_result[0].param.iw_port.iw_port_handle = h_iw_port_gbe[0]; 
      rule_fwd.rule_fields[0].value.mac_addr[5] = i; 
      PCE_rule_handle[RULES_A_FORWARDING_START+i] = WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID),
                                                                   WP_PCE_RULE_FORWARDING,
                                                                   &rule_fwd);
      terminate_on_error(PCE_rule_handle[RULES_A_FORWARDING_START+i], "WP_PceRuleCreate",__LINE__);
   }


   /* Send and recieve packet */
   CLI_SendAndRecievePacket();
   
   /* check statistics */
   CLI_CheckStatisticsMatched(excpected_class_fa_count+(3*NUM_OF_FLOWS),
                              expected_default_fa_count+(2*NUM_OF_FLOWS), 
                              previous_filter_stat[FILTER_SET_B_CLASSIFICATION].matched_pkts,
                              previous_filter_stat[FILTER_SET_A_FORWARDING].matched_pkts,
                              previous_filter_stat[FILTER_SET_A_FORWARDING].no_matched_pkts + (2*NUM_OF_FLOWS),
                              __LINE__);

   if (TestType == TEST_HYBRID_FDB)
   {
      for(i=0; i<NUM_OF_RULES_PER_FILTER_SET/2; i++)
         free_pce_hw_rules--;

      WPE_DeleteDummyPceHwRules();
   }

/*---------------------------------------------------*/   

   memcpy(previous_filter_stat, filter_stat, sizeof(WP_pce_filter_stat)*100);

   /* Modify Filter Set A classification */
   for(i=0; i<NUM_OF_INPUT_PORT_PER_FILTER_SET; i++)
   {
      rule_cfg.enabled = WP_ENABLE;
      rule_cfg.filter_handle = PCE_filter[FILTER_SET_A_CLASSIFICATION];
      rule_cfg.match_result[0].result_type = WP_PCE_RESULT_IN_IW_PORT_UPDATE;
      rule_cfg.match_result[0].param.iw_port.iw_port_handle = h_iw_port_gbe[i];

      rule_cfg.match_result[1].result_type = WP_PCE_RESULT_PCE_INTERFACE;
      rule_cfg.match_result[1].param.pce_if_iw_next_round.pce_interface = pce_dl_second_round_if_handle;

      rule_cfg.match_result[2].result_type = WP_PCE_RESULT_LAST;

      status = WP_PceRuleModify(PCE_rule_handle[RULES_A_CLASSIFICATION_START+i],
                                WP_PCE_RULE_MODIFY_COMMAND_RESULT,
                                &rule_cfg);
      terminate_on_error(status, "WP_PceRuleModify",__LINE__);
   }

   /* Modify Rule Result */
   rule_fwd.match_result[0].result_type = WP_PCE_RESULT_OUT_IW_PORT_UPDATE;
   rule_fwd.match_result[0].param.iw_port.iw_port_handle = h_iw_output_port_b; 
   
   rule_fwd.match_result[1].result_type = WP_PCE_RESULT_AGING;
   rule_fwd.match_result[1].param.aging.mode = WP_PCE_AGING_DISABLE;
   
   rule_fwd.match_result[2].result_type = WP_PCE_RESULT_LAST;
   
   for(i=0; i<NUM_OF_FLOWS; i++)
   {
      status = WP_PceRuleModify(PCE_rule_handle[RULES_A_FORWARDING_START+i],
                                WP_PCE_RULE_MODIFY_COMMAND_RESULT,
                                &rule_fwd);
      terminate_on_error(status, "WP_PceRuleModify",__LINE__);
   }

   /* Send and recieve packet */
   CLI_SendAndRecievePacket();

   /* check statistics */
   CLI_CheckStatisticsMatched(excpected_class_fa_count+(4*NUM_OF_FLOWS),
                              expected_default_fa_count+(2*NUM_OF_FLOWS), 
                              previous_filter_stat[FILTER_SET_B_CLASSIFICATION].matched_pkts + NUM_OF_FLOWS,
                              previous_filter_stat[FILTER_SET_A_FORWARDING].matched_pkts +  NUM_OF_FLOWS,
                              previous_filter_stat[FILTER_SET_A_FORWARDING].no_matched_pkts,
                              __LINE__);

/*---------------------------------------------------*/   

   memcpy(previous_filter_stat, filter_stat, sizeof(WP_pce_filter_stat)*100);

   /* Modify Rule Result with next round cancelation */
   rule_fwd.match_result[0].result_type = WP_PCE_RESULT_OUT_IW_PORT_UPDATE;
   rule_fwd.match_result[0].param.iw_port.iw_port_handle = h_iw_output_port_b; 
   
   rule_fwd.match_result[1].result_type = WP_PCE_RESULT_AGING;
   rule_fwd.match_result[1].param.aging.mode = WP_PCE_AGING_DISABLE;
   
   rule_fwd.match_result[2].result_type = WP_PCE_RESULT_PCE_INTERFACE;
   rule_fwd.match_result[2].param.pce_if_iw_next_round.pce_interface = 0;

   rule_fwd.match_result[3].result_type = WP_PCE_RESULT_LAST;
   
   for(i=0; i<NUM_OF_FLOWS; i++)
   {
      status = WP_PceRuleModify(PCE_rule_handle[RULES_A_FORWARDING_START+i],
                                WP_PCE_RULE_MODIFY_COMMAND_RESULT,
                                &rule_fwd);
      terminate_on_error(status, "WP_PceRuleModify",__LINE__);
   }

   /* Send and recieve packet */
   CLI_SendAndRecievePacket();

   /* check statistics */
   CLI_CheckStatisticsMatched(excpected_class_fa_count+(4*NUM_OF_FLOWS),
                              expected_default_fa_count+(3*NUM_OF_FLOWS), 
                              previous_filter_stat[FILTER_SET_B_CLASSIFICATION].matched_pkts,
                              previous_filter_stat[FILTER_SET_A_FORWARDING].matched_pkts + NUM_OF_FLOWS,
                              previous_filter_stat[FILTER_SET_A_FORWARDING].no_matched_pkts,
                              __LINE__);

/*---------------------------------------------------*/   

   memcpy(previous_filter_stat, filter_stat, sizeof(WP_pce_filter_stat)*100);

   /* Restore Filter Set A classification */
   for(i=0; i<NUM_OF_INPUT_PORT_PER_FILTER_SET; i++)
   {
      rule_cfg.enabled = WP_ENABLE;
      rule_cfg.filter_handle = PCE_filter[FILTER_SET_A_CLASSIFICATION];
      rule_cfg.match_result[0].result_type = WP_PCE_RESULT_IN_IW_PORT_UPDATE;
      rule_cfg.match_result[0].param.iw_port.iw_port_handle = h_iw_port_gbe[i];

      rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;

      status = WP_PceRuleModify(PCE_rule_handle[RULES_A_CLASSIFICATION_START+i],
                                WP_PCE_RULE_MODIFY_COMMAND_RESULT,
                                &rule_cfg);
      terminate_on_error(status, "WP_PceRuleModify",__LINE__);
   }


   /* Restore Filter Set A forwarding Rules */
   rule_fwd.match_result[0].result_type = WP_PCE_RESULT_OUT_IW_PORT_UPDATE;
   rule_fwd.match_result[0].param.iw_port.iw_port_handle = h_iw_output_port_b; 
   
   rule_fwd.match_result[1].result_type = WP_PCE_RESULT_AGING;
   rule_fwd.match_result[1].param.aging.mode = WP_PCE_AGING_DISABLE;
   
   rule_fwd.match_result[2].result_type = WP_PCE_RESULT_PCE_INTERFACE;
   rule_fwd.match_result[2].param.pce_if_iw_next_round.pce_interface = pce_dl_second_round_if_handle;

   rule_fwd.match_result[3].result_type = WP_PCE_RESULT_LAST;
   
   for(i=0; i<NUM_OF_FLOWS; i++)
   {
      status = WP_PceRuleModify(PCE_rule_handle[RULES_A_FORWARDING_START+i],
                                WP_PCE_RULE_MODIFY_COMMAND_RESULT,
                                &rule_fwd);
      terminate_on_error(status, "WP_PceRuleModify",__LINE__);
   }

   /* Send and recieve packet */
   CLI_SendAndRecievePacket();

   /* check statistics */
   CLI_CheckStatisticsMatched(excpected_class_fa_count+(5*NUM_OF_FLOWS),
                              expected_default_fa_count+(3*NUM_OF_FLOWS), 
                              previous_filter_stat[FILTER_SET_B_CLASSIFICATION].matched_pkts + NUM_OF_FLOWS,
                              previous_filter_stat[FILTER_SET_A_FORWARDING].matched_pkts + NUM_OF_FLOWS,
                              previous_filter_stat[FILTER_SET_A_FORWARDING].no_matched_pkts,
                              __LINE__);

/*-------check forwarding no match action = accept-------------------------------*/   
   
   if (TestType != TEST_SW_FDB_ONLY)
   {
      memcpy(previous_filter_stat, filter_stat, sizeof(WP_pce_filter_stat)*100);
      
      expected_default_fa_count = WPT_CompatibleFaGetFwPacketsCount(default_agg);
      
      for(ii=NUM_OF_FLOWS; ii < 2*NUM_OF_FLOWS; ii++)
      {
         WPE_HostSendPacket(FILTER_SET_D, ii);
      }
      
      CLI_CheckFwdFilterStatistics(expected_default_fa_count+ NUM_OF_FLOWS,
                                   FILTER_SET_D_FORWARDING,
                                   0,
                                   __LINE__);
      
      
/*-------check forwarding no match action = deny-------------------------------*/   
      
      memcpy(previous_filter_stat, filter_stat, sizeof(WP_pce_filter_stat)*100);

      expected_default_fa_count = WPT_CompatibleFaGetFwPacketsCount(default_agg);
      
      for(ii=NUM_OF_FLOWS; ii < 2*NUM_OF_FLOWS; ii++)
      {
         WPE_HostSendPacket(FILTER_SET_E, ii);
      }
      
      CLI_CheckFwdFilterStatistics(expected_default_fa_count,
                                   FILTER_SET_E_FORWARDING,
                                   1,
                                   __LINE__);

/*-------check forwarding modify no match action = accept-------------------------------*/   
      
      
      memcpy(previous_filter_stat, filter_stat, sizeof(WP_pce_filter_stat)*100);
      
      expected_default_fa_count = WPT_CompatibleFaGetFwPacketsCount(default_agg);
      
      
      fwd_filter_cfg.no_match_action = WP_PCE_FILTER_NO_MATCH_ACCEPT;
      status = WP_PceFilterModify(PCE_filter[FILTER_SET_E_FORWARDING], 
                                  WP_PCE_FILTER_MODIFY_COMMAND_NO_MATCH, 
                                  &fwd_filter_cfg);
      terminate_on_error(status, "WP_PceFilterModify",__LINE__);
      
      
      for(ii=NUM_OF_FLOWS; ii < 2*NUM_OF_FLOWS; ii++)
      {
         WPE_HostSendPacket(FILTER_SET_E, ii);
      }
      
      CLI_CheckFwdFilterStatistics(expected_default_fa_count + NUM_OF_FLOWS,
                                   FILTER_SET_E_FORWARDING,
                                   0,
                                   __LINE__);
   
/*-------check forwarding modify no match action = deny-------------------------------*/   
   
 
      memcpy(previous_filter_stat, filter_stat, sizeof(WP_pce_filter_stat)*100);

      expected_default_fa_count = WPT_CompatibleFaGetFwPacketsCount(default_agg);
      

      fwd_filter_cfg.no_match_action = WP_PCE_FILTER_NO_MATCH_DENY;
      status = WP_PceFilterModify(PCE_filter[FILTER_SET_E_FORWARDING], 
                                  WP_PCE_FILTER_MODIFY_COMMAND_NO_MATCH, 
                                  &fwd_filter_cfg);
      terminate_on_error(status, "WP_PceFilterModify",__LINE__);
   
   
      for(ii=NUM_OF_FLOWS; ii < 2*NUM_OF_FLOWS; ii++)
      {
         WPE_HostSendPacket(FILTER_SET_E, ii);
      }
      
      CLI_CheckFwdFilterStatistics(expected_default_fa_count,
                                   FILTER_SET_E_FORWARDING,
                                   1,
                                   __LINE__);
   }
   
   printf("\nRule Scenarios Pass\n");
}

void WPE_CheckRulesRestrictions(void)
{
   WP_pce_rule_forwarding rule_fwd = {0};
   WP_U32 rule_index, ii;
   WP_handle status;
   

   rule_fwd.enabled = WP_ENABLE;
   rule_fwd.filter_handle = PCE_filter[FILTER_SET_A_FORWARDING];
   rule_index = RULES_A_FORWARDING_START + 50;
   
   rule_fwd.rule_fields[0].field_id = WP_PCE_FIELD_ID_MAC_DA;
   for(ii=0; ii<5; ii++)
      rule_fwd.rule_fields[0].value.mac_addr[ii] = start_mac_addr[ii]; 
   rule_fwd.rule_fields[0].value.mac_addr[5] = 0x0; 
   
   rule_fwd.rule_fields[1].field_id = WP_PCE_FIELD_ID_LAST;
/////////////////////////////////////////////////////////////
   rule_fwd.match_result[0].result_type = WP_PCE_RESULT_OUT_IW_PORT_UPDATE;
   rule_fwd.match_result[0].param.iw_port.iw_port_handle = h_iw_output_port; 
   
   rule_fwd.match_result[1].result_type = WP_PCE_RESULT_AGING;
   rule_fwd.match_result[1].param.aging.mode = WP_PCE_AGING_DISABLE;
   
   rule_fwd.match_result[2].result_type = WP_PCE_RESULT_LAST;
   
   rule_fwd.match_action = WP_PCE_RULE_MATCH_CONTINUE;
   
   /* Check Rules duplication error */
   PCE_rule_handle[rule_index] = WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID),
                                                  WP_PCE_RULE_FORWARDING,
                                                  &rule_fwd);
#if USE_SHIFT_ENUM	
   check_error(PCE_rule_handle[rule_index], "WP_PceRuleCreate()" ,__LINE__, WP_ERR_PCE_RULE_ALREADY_EXISTS);
#else
   check_error(PCE_rule_handle[rule_index], "WP_PceRuleCreate()" ,__LINE__, WP_ERR_PCE_RULE_ALREADY_EXISTS - 1);
#endif
   printf("\nTest Duplication Rules check -- PASSED\n");

   /* Check Rule illigal result error */
   rule_fwd.rule_fields[0].value.mac_addr[5] = 0x23; 
   rule_fwd.match_result[2].result_type = WP_PCE_RESULT_FILTER_SET_CHANGE;
   rule_fwd.match_result[2].param.change_fs.filter_set_handle = dl_filter_set[FILTER_SET_B];

   rule_fwd.match_result[3].result_type = WP_PCE_RESULT_LAST;

   PCE_rule_handle[rule_index] = WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID),
                                                  WP_PCE_RULE_FORWARDING,
                                                  &rule_fwd);
   if (pce_init.enhanced_mode == WP_SW_FDB_ENABLE_MODE)
   {
#if USE_SHIFT_ENUM	
      check_error(PCE_rule_handle[rule_index], "WP_PceRuleCreate()" ,__LINE__, WP_ERR_SW_FDB_RULE_RESULT_ILLEGAL_COMBINATION);
#else
      check_error(PCE_rule_handle[rule_index], "WP_PceRuleCreate()" ,__LINE__, WP_ERR_SW_FDB_RULE_RESULT_ILLEGAL_COMBINATION - 1);
#endif

   }
   else
   {
      check_error(PCE_rule_handle[rule_index], "WP_PceRuleCreate()" ,__LINE__, WP_OK);
      status = WP_PceRuleDelete(PCE_rule_handle[rule_index]);
      check_error(status, "WP_PceRuleDelete()" ,__LINE__, WP_OK);
   }
   
   printf("\nTest Illigal Rule result check -- PASSED\n");

   /* Used Flow agg can't be deleted */
   for (ii=0; ii<FLOW_AGG_COUNT; ii++)
   {
      status =  WP_IwFlowAggregationDelete(flow_agg[ii]);
      check_error(status, "WP_IwFlowAggregationDelete()" ,__LINE__, WP_ERR_IW_FLOW_AGG_USED_BY_PCE_RESULTS);
   }
   printf("\nTest Can't Delete FA after connected to rule -- PASSED\n");

   /* Used Output port can't be deleted */
   status =  WP_IwPortDelete(h_iw_output_port);
   check_error(status, "WP_IwPortDelete()" ,__LINE__, WP_ERR_IW_PORT_USED_BY_PCE_RESULTS);

   status =  WP_IwPortDelete(h_iw_output_port_b);
   check_error(status, "WP_IwPortDelete()" ,__LINE__, WP_ERR_IW_PORT_USED_BY_PCE_RESULTS);

   printf("\nTest Can't Delete Output port after connected to rule -- PASSED\n");
}

void WPE_CheckMaxRulesRestriction(void)
{
   WP_pce_rule_forwarding rule_fwd = {0};
   WP_U32 rule_index, ii, i, num_of_rules_left;
   WP_handle status;
   
   
   num_of_rules_left = (pce_init.sw_fdb_info->max_fdb_rules - (5*NUM_OF_RULES_PER_FILTER_SET));

   /* Create up to maximum FDB Rules */
   for(i=0; i<num_of_rules_left; i++)
   {
      rule_fwd.enabled = WP_ENABLE;
      rule_fwd.filter_handle = PCE_filter[FILTER_SET_A_FORWARDING];
      rule_index = 800 + i;
      
      rule_fwd.rule_fields[0].field_id = WP_PCE_FIELD_ID_MAC_DA;
      for(ii=0; ii<5; ii++)
         rule_fwd.rule_fields[0].value.mac_addr[ii] = start_mac_addr[ii]; 
      rule_fwd.rule_fields[0].value.mac_addr[5] = 0x20+i; 
      
      rule_fwd.rule_fields[1].field_id = WP_PCE_FIELD_ID_LAST;
      
      rule_fwd.match_result[0].result_type = WP_PCE_RESULT_OUT_IW_PORT_UPDATE;
      rule_fwd.match_result[0].param.iw_port.iw_port_handle = h_iw_output_port; 
      
      rule_fwd.match_result[1].result_type = WP_PCE_RESULT_AGING;
      rule_fwd.match_result[1].param.aging.mode = WP_PCE_AGING_DISABLE;
      
      rule_fwd.match_result[2].result_type = WP_PCE_RESULT_LAST;
      
      rule_fwd.match_action = WP_PCE_RULE_MATCH_CONTINUE;
      
      PCE_rule_handle[rule_index] = WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID),
                                                     WP_PCE_RULE_FORWARDING,
                                                     &rule_fwd);
      terminate_on_error(PCE_rule_handle[rule_index], "WP_PceRuleCreate",__LINE__);
   }

   /* Check reaching maximum FDB rules */
   rule_index = 800 + num_of_rules_left;
   rule_fwd.rule_fields[0].value.mac_addr[5] = 0x20+num_of_rules_left; 
   PCE_rule_handle[rule_index] = WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID),
                                                  WP_PCE_RULE_FORWARDING,
                                                  &rule_fwd);
#if USE_SHIFT_ENUM	
   check_error(PCE_rule_handle[rule_index], "WP_PceRuleCreate()" ,__LINE__, WP_ERR_PCE_NUM_OF_RULES_EXCEEDED);
#else
   check_error(PCE_rule_handle[rule_index], "WP_PceRuleCreate()" ,__LINE__, WP_ERR_PCE_FILTER_MODIFY_ILLEGAL_COMMAND);
#endif
   printf("\nTest Reached Max FDB Rules -- PASSED\n");
   
   for(i=0; i<num_of_rules_left; i++)
   {
      rule_index = 800 + i;
      
      status = WP_PceRuleDelete(PCE_rule_handle[rule_index]);
      terminate_on_error(status, "WP_PceRuleDelete",__LINE__);
   }
}

void CLI_RunScenarios(void)
{

   /* rules mean where to goes, the action */
   WPE_CreateDlSecondRoundPceRules();

   WPE_CreateDlPceRules();


   if (pce_init.enhanced_mode == WP_SW_FDB_ENABLE_MODE)
   {
      /* Check Max FDB rules restriction */
      WPE_CheckMaxRulesRestriction();
   }
   
   /* Check forwarding rules restrictions */
   WPE_CheckRulesRestrictions();

   /* Check forwarding filter 'no match action' */
   WPE_CheckForwardingFilterCreateErrors();

   /* Check learning filter 'no match action' */
   WPE_CheckLearningFilterCreateErrors();

   /* Filter scenarios */
   CLI_RunAllFiltersScenario();
   
   /* Rule scenarios */
   CLI_RunRuleScenarios();

   /* Learning scenarios */
   CLI_RunLearningScenarios();
  
   /* Aging scenarios */
   CLI_RunAgingScenarios();
 
   /* PCE Interface on Rule scenarios */
   CLI_RunRuleIertationScenarios();
   
   /* Rule Search scenarios */
   CLI_RunRulesearch();

   /* Check FilterSet modify restrictions */
   WT_CheckFilterSetModify();

   /* Check forwarding filter 'no match action' */
   WPE_CheckForwardingFilterModifyErrors();

   /* Check learning filter 'no match action' */
   WPE_CheckLearningFilterModifyErrors();

#if DEBUG
   CLI_Menu();
#endif

   WPE_DeleteDlPceRules();

   WPE_DeleteDlSecondRoundPceRules();
}

void CLI_CreateTest(void)
{
  WP_handle status;
  WP_U32 i, max_mac_addresses;  

#if DEBUG
   WP_ControlRegister(WP_DEBUG_CALLBACK_WDDI_LOG,logCallBack);
   WP_ControlRegister(WP_DEBUG_CALLBACK_FILE_LINE_ERROR,WPE_DebugCallback);
#endif

   WPE_LayerTwoSystemCreate();

   status = WP_SysCommit();
   terminate_on_error(status, "WP_SysCommit",__LINE__);
printf ("CLI_CreateTest: WP_SysCommit() OK\n");
   
   status = WPU_DebugEventGroupLoad(WP_WINPATH(DEFAULT_WPID),-1,"IWGP_MODULE_EXITS");
   terminate_on_error(status, "WPU_DebugEventGroupLoad",__LINE__);

   status = WPU_DebugEventGroupLoad(WP_WINPATH(DEFAULT_WPID),-1,"IWGP_SUBMODULE_EXITS");
   terminate_on_error(status, "WPU_DebugEventGroupLoad",__LINE__);

   status = WPU_DebugEventGroupLoad(WP_WINPATH(DEFAULT_WPID),-1,"IWGP_SUBMODULE_ENTRIES");
   terminate_on_error(status, "WPU_DebugEventGroupLoad",__LINE__);

   status = WPU_DebugEventGroupLoad(WP_WINPATH(DEFAULT_WPID),-1,"IWGP_LOCKS");
   terminate_on_error(status, "WPU_DebugEventGroupLoad",__LINE__);

printf ("CLI_CreateTest: WPU_DebugEventGroupLoad() OK\n");


/*---------------------------------------*\
	morris
		: gbe1_rx_ch_handle/gbe1_tx_ch_handle
		: rx_host_handle
\*---------------------------------------*/
   WPE_ChannelsCreate();





/*---------------------------------------*\
	morris
		: default_agg	==> rx_host_handle
\*---------------------------------------*/
   br_agg_gbe->txfunc = rx_host_handle;
   default_agg =  WP_IwFlowAggregationCreate(WP_WINPATH(DEFAULT_WPID),
                                             WP_IW_VLAN_AWARE_BRIDGING_MODE,
                                             &br_agg_gbe[0]);
   terminate_on_error(default_agg, "WP_IwFlowAggregationCreate()",__LINE__);
printf ("CLI_CreateTest: WP_IwFlowAggregationCreate(default_agg ) OK\n");

/*---------------------------------------*\
 * 	morris
 * 		:flow_agg[0]	==>
 * 		:flow_agg[1]	==>
 * 		:flow_agg[2]	==> rx_host_handle
 * 		:flow_agg[3]	==>
 * 		:flow_agg[4]	==>
\*---------------------------------------*/
   br_agg_gbe->txfunc = rx_host_handle;
   for (i=0; i<FLOW_AGG_COUNT/*5*/; i++)
   {
      flow_agg[i] =  WP_IwFlowAggregationCreate(WP_WINPATH(DEFAULT_WPID),
                                                WP_IW_VLAN_AWARE_BRIDGING_MODE,
                                                &br_agg_gbe[0]);
      terminate_on_error(flow_agg[i], "WP_IwFlowAggregationCreate()",__LINE__);
printf ("CLI_CreateTest: WP_IwFlowAggregationCreate(flow_agg[%d])OK\n", i);
   }
   
/*---------------------------------------*\
	morris
		: IW system
		dl_general_iwsys_bridge
\*---------------------------------------*/
   /* Create Generic IW system */
   dl_general_iwsys_bridge = WP_IwSystemCreate(WP_WINPATH(DEFAULT_WPID), WP_IW_VLAN_AWARE_BRIDGING_MODE, &iw_sys_bridging[0]);
   terminate_on_error(dl_general_iwsys_bridge, "WP_IwSystemCreate",__LINE__);
printf ("CLI_CreateTest: WP_IwSystemCreate(dl_general_iwsys_bridge ) OK\n");





/*---------------------------------------*\
	morris
	IW port create
		: h_iw_output_port	<==>
		: h_iw_output_port_b	<==> dl_general_iwsys_bridge
		: h_iw_port_next_round	<==>
\*---------------------------------------*/
   /* create output IW Ports */
   WPE_PortsCreate(dl_general_iwsys_bridge, &h_iw_output_port, 300);
printf ("CLI_CreateTest: WPE_PortsCreate(h_iw_output_port) OK\n");
   WPE_PortsCreate(dl_general_iwsys_bridge, &h_iw_output_port_b, 300);
printf ("CLI_CreateTest: WPE_PortsCreate(h_iw_output_port_b) OK\n");

   /* Create next round IW port */
   WPE_PortsCreate(dl_general_iwsys_bridge, &h_iw_port_next_round, 300);
printf ("CLI_CreateTest: WPE_PortsCreate(h_iw_port_next_round) OK\n");





/*---------------------------------------*\
	total 2 IW systems
		dl_iwsys_bridge[0]
		dl_iwsys_bridge[1]
\*---------------------------------------*/
   /* Create IW systems */
   for(i=0; i<NUM_OF_INPUT_PORT_PER_FILTER_SET/*2*/; i++)
   {
      dl_iwsys_bridge[i] = WP_IwSystemCreate(WP_WINPATH(DEFAULT_WPID), WP_IW_VLAN_AWARE_BRIDGING_MODE, &iw_sys_bridging[0]);
      terminate_on_error(dl_iwsys_bridge[i], "WP_IwSystemCreate",__LINE__);
printf ("CLI_CreateTest: WP_IwSystemCreate(dl_iwsys_bridge[%d]) OK\n", i);
   }
   




/*---------------------------------------*\
	total = 5 * 2 = 10
		: h_iw_port_gbe[0]	===> 
		: h_iw_port_gbe[2]	===> 
		: h_iw_port_gbe[4] 	===> dl_iwsys_bridge[0]
		: h_iw_port_gbe[6]	===> 
		: h_iw_port_gbe[8]	===> 


		: h_iw_port_gbe[1]	===> 
		: h_iw_port_gbe[3]	===> 
		: h_iw_port_gbe[5]	===> dl_iwsys_bridge[1]
		: h_iw_port_gbe[7]	===> 
		: h_iw_port_gbe[9]	===> 
\*---------------------------------------*/
   /* create Input IW Ports */
   for(i=0; i<(FILTER_SET_COUNT/*5*/ * NUM_OF_INPUT_PORT_PER_FILTER_SET/*2*/); i++)
   {
      max_mac_addresses = NUM_OF_RULES_PER_FILTER_SET;
      WPE_PortsCreate(dl_iwsys_bridge[(i%2)], &h_iw_port_gbe[i], max_mac_addresses);
printf ("CLI_CreateTest: WPE_PortsCreate(h_iw_port_gbe[%d]) OK\n", i);
   }




/*---------------------------------------*\
	h_iw_port_general <==> dl_general_iwsys_bridge
\*---------------------------------------*/
   /* Create General IW port */
   WPE_PortsCreate(dl_general_iwsys_bridge, &h_iw_port_general, 300);
printf ("CLI_CreateTest: WPE_PortsCreate(h_iw_port_general OK\n");
   WPE_DlRxBindingCreate(dl_general_iwsys_bridge);
printf ("CLI_CreateTest: WPE_DlRxBindingCreate(dl_general_iwsys_bridge) OK\n");

   WPE_SystemEnable();
printf ("CLI_CreateTest: WPE_SystemEnable() OK\n");

   status = WP_IwSystemBuild(dl_general_iwsys_bridge);
   terminate_on_error(status, "WP_IwSystemBuild()",__LINE__);
printf ("CLI_CreateTest: WP_IwSystemBuild(dl_general_iwsys_bridge) OK\n");


/*----------------------------------------------------------------*\
	we create PCE module here
\*----------------------------------------------------------------*/

   /* Creates the DL Second round PCE Interface, filters and filter sets (FilterSet B) */
   WPE_CreateDlSecondRoundPceInterface();
printf ("CLI_CreateTest: WPE_CreateDlSecondRoundPceInterface() OK\n");

   /* Creates the DL PCE filters and rules (and filter sets A,C,D,E) */
   WPE_CreateDlPceInterface(dl_general_iwsys_bridge);
printf ("CLI_CreateTest: WPE_CreateDlPceInterface(dl_general_iwsys_bridge) OK\n");

   /* PCE Statistics reset */
   WPE_CheckPceFilterStatsReset();
printf ("CLI_CreateTest: WPE_CheckPceFilterStatsReset() OK\n");
}

void WPE_CheckPceFilterStatsReset(void)
{
   WP_U32 i;  

   memset(filter_stat, 0, sizeof(WP_pce_filter_stat) * 100);

   for (i=0; i<NUM_OF_PCE_FILTERS; i++)
      WP_PceFilterStatisticsReset (PCE_filter[i], &filter_stat[i]);

//   printf("\nPCE statistics has been cleared\n");
}

void CLI_StatMenu(void)
{
   WP_U32 i;
   WP_U8 cmd;
   WP_status status;
   WP_CHAR temp[100];
   
   while(1)
   {
      printf("\n\nStatistics Menu\n");
      printf("--------------------\n");
      printf("1) Device Stats\n");
      printf("2) Iw Port Stats\n");
      printf("3) Flow Agg Stats\n");
      printf("4) PCE Stats\n");
      printf("5) Memory available\n");
      printf("6) Clear Statistics\n");
      printf("w) Winutil \n");
      printf("m) Main Menu \n");

      cmd = getchar();

     if (cmd == '1') {

        /* Device Statistics */
        printf("\nDevice GE1 statistics\n");
        WPT_LocalDisplayDeviceStats(gbe_dev1);

        printf("\nRx Host queue size %d\n", WPT_HostChannelQueueSize(rx_host_handle));
     }
     else if (cmd == '2') {
        WPT_DisplayBridgingPortStats(h_iw_port_general,"General IW port");
        for (i=0; i<(FILTER_SET_COUNT * NUM_OF_INPUT_PORT_PER_FILTER_SET); i++)
        {
           sprintf (temp, "input port index %d filter set %d", i%2, i/2);
           WPT_DisplayBridgingPortStats(h_iw_port_gbe[i], temp);
        }
        WPT_DisplayBridgingPortStats(h_iw_port_next_round, "next round input port");

        WPT_DisplayBridgingPortStats(h_iw_output_port, "output port a");
        WPT_DisplayBridgingPortStats(h_iw_output_port_b, "output port b");
      }
      else if (cmd == '3') {
         printf("\nDefault FA statistics\n");
         WPT_CompatibleFlowStatistics(default_agg);

         for (i=0; i<FLOW_AGG_COUNT; i++)
         {
            printf("\nFlow agg %d statistics\n",i);
            WPT_CompatibleFlowStatistics(flow_agg[i]);
         }

         printf("\nFA Total statistics\n");
         for (i=0; i<FLOW_AGG_COUNT; i++)
         {
            printf ("FA %s Total packets %d\n",flow_agg_string[i],WPT_CompatibleFaGetFwPacketsCount(flow_agg[i]));
         }
      }
      else if (cmd == '4') {
         printf("\nPCE filter statistics\n");
         for (i=0; i<NUM_OF_PCE_FILTERS; i++)
            WPE_CheckPceFilterStats(PCE_filter[i]);
         
         WPE_PrintPceGlobalStats();
      }
      else if (cmd == '5')
      {
         WP_U32 bytes = 0;

         status = WP_MemoryAvailable(WP_WINPATH(DEFAULT_WPID), WP_BUS_PACKET, APP_BANK_PACKET, &bytes);
         printf("Memory status: Packet bus bytes left   = 0x%08x\n", bytes);
         
         status = WP_MemoryAvailable(WP_WINPATH(DEFAULT_WPID), WP_BUS_PARAM, APP_BANK_PARAM, &bytes);
         printf("Memory status: Param bus bytes left    = 0x%08x\n", bytes);
         
         status = WP_MemoryAvailable(WP_WINPATH(DEFAULT_WPID), WP_BUS_INTERNAL, APP_BANK_INTERNAL, &bytes);
         printf("Memory status: Internal bus bytes left = 0x%08x\n", bytes);

         status = WP_MemoryAvailable(WP_WINPATH(DEFAULT_WPID), WP_BUS_HOST, APP_BANK_HOST, &bytes);
         printf("Memory status: Host bus bytes left     = 0x%08x\n", bytes);
      }
      else if (cmd == '6')
      {
         WPE_StatisticsReset();
      }
      else if (cmd == 'w')
      {
         WT_UtilitiesMenu();
      }
      if (cmd == 'm')
      {
         printf("\n\n");
         break;
      }
   }
}

void WPT_LocalDeviceStatsReset(WP_handle enet_handle)
{
   WP_stats_enet enet_stats;
   WP_status status;

   status = WP_DeviceStatisticsReset(enet_handle, &enet_stats);
   terminate_on_error(status, "WP_DeviceStatisticsReset()",__LINE__);
}

void WPE_StatisticsReset(void)
{
   /* Device Statistics */
   WPT_LocalDeviceStatsReset(gbe_dev1);

   /* PCE Statistics reset */
   WPE_CheckPceFilterStatsReset();
}

WP_status WPE_PrintLogEntry(wps_lm_log_read_entry * log_entry)
{
   WP_U32 ii,jj;

   printf("TRS number: %d Identity: %d\n",log_entry->trs_number,log_entry->event_id);
   printf("Group: %s\nEvent: %s\nEvent num: %d\n",log_entry->group_name,log_entry->event_name,log_entry->event_id);
   printf("timestamp = 0x%08x,%08x\n",
          (WP_U32)(log_entry->timestamp >> 32),
          (WP_U32)(log_entry->timestamp & 0xffffffff));
   printf("File: %s Line: %d\n",log_entry->file,log_entry->line);

   for(ii = 0; ii < log_entry->num_of_entries; ii++)
   {
      printf("entry_size: %d\n",log_entry->entry_size[ii]);
      printf("%s\n",log_entry->entry_message[ii]);
      for(jj = 0; jj < log_entry->entry_size[ii]; jj+=4)
      {
         printf("0x%02x",log_entry->entry_info[ii][jj]);
         printf("%02x",log_entry->entry_info[ii][jj+1]);
         printf("%02x",log_entry->entry_info[ii][jj+2]);
         printf("%02x\n",log_entry->entry_info[ii][jj+3]);
      }
   }
   printf("\n\n");
   return WP_OK;
}

void WT_UtilitiesMenu(void)
{

   WP_CHAR ch1='c';

   do
   {    
      printf("\n\n1) Check for Bus Error");
      printf("\n2) Wingine and Bus Utilization");
      printf("\n3) Thread Utilization");
      printf("\n4) Thread Arbiter Dump");
      printf("\n5) Serial Arbiter Dump");
      printf("\n6) DPS Hardware Trace Start");
      printf("\n7) DPS Hardware Trace Dump");
      printf("\n8) Version Info");
      printf("\n9) DPS log");
      printf("\nM) Main Menu\n\n");

      ch1 = getchar();

      switch (ch1)
      {
         case '1' :
            WPU_WinutilCall("hbm");
            break;
         case '2' :
            WPU_WinutilCall("sysu");
            break;
         case '3' :
            WPU_WinutilCall("trsu");
            break;
         case '4' :
            WPU_WinutilCall("taot");
            printf("\n");
            WPU_WinutilCall("tast");
            printf("\n");
            WPU_WinutilCall("tab");
            printf("\n");
            WPU_WinutilCall("taom");
            printf("\n");
            WPU_WinutilCall("dmap");
            printf("\n");
            WPU_WinutilCall("bmap");
            break;
         case '5' :
            WPU_WinutilCall("trsa");
            printf("\n");
            WPU_WinutilCall("smap");
            printf("\n");
            break;
         case '6' :
            WPU_WinutilCall("tb init");
            break;
         case '7' :
            WPU_WinutilCall("tb dump");
            break;
         case '8' :
            WPU_WinutilCall("ver");
         case '9':
         {
            WP_U32 num_of_entries_actually_read = 0;
            WP_status status = WPU_DebugSwLogRead(WP_SYSHANDLE(DEFAULT_WPID),
                                                  100,
                                                  &num_of_entries_actually_read,
                                                  WPE_PrintLogEntry);
            terminate_on_error(status, "WPU_DebugSwLogRead()",__LINE__);
            printf("num_of_entries_actually_read: %d\n",num_of_entries_actually_read);
            break;
         }
         
      }

    } while (ch1 != 'M' && ch1 != 'm');
}

void CLI_Menu(void)
{
   WP_U8 cmd;
   WP_status status;
   WP_pce_rule_forwarding rule_fwd = {0};
   WP_U32 ii=0;
   WP_handle PCE_fwd_handle[20];
   WP_U32 rules_returned = 0;
   WP_handle rule_handle;

   rule_fwd.enabled = WP_ENABLE;
   
   rule_fwd.filter_handle = PCE_filter[FILTER_SET_A_FORWARDING];
   
   rule_fwd.rule_fields[0].field_id = WP_PCE_FIELD_ID_MAC_DA;
   for(ii=0; ii<5; ii++)
      rule_fwd.rule_fields[0].value.mac_addr[ii] = ii; 
   rule_fwd.rule_fields[0].value.mac_addr[5] = 3; 
   
//    rule_fwd.rule_fields[1].field_id = WP_PCE_FIELD_ID_INT_VLAN_TAG;
//    rule_fwd.rule_fields[1].value.vlan_tag = 0x0052;
   
   rule_fwd.rule_fields[0].value.mac_addr[2] = 1; 
   rule_fwd.rule_fields[0].value.mac_addr[3] = (ii & 0xffffff) >> 16; 
   rule_fwd.rule_fields[0].value.mac_addr[4] = (ii & 0xffff) >> 8; 
   rule_fwd.rule_fields[0].value.mac_addr[5] = ii & 0xff; 
   
   rule_fwd.rule_fields[1].field_id = WP_PCE_FIELD_ID_LAST;

   while(1)
   {
      printf("\n\nMenu\n");
      printf("--------\n");
      printf("1) Statistics\n");
      printf("2) Send Data\n");
      printf("3) Check Recieved Data\n");
      printf("c) PCE rule display\n");
      printf("d) Learning queue service \n");
      printf("e) delete Learnt rules \n");
      printf("f) Aging PceIteratorGetFirst \n");
      printf("g) Aging PceIteratorGetNext \n");
      printf("h) Rule PceIteratorGetFirst \n");
      printf("i) Rule PceIteratorGetNext \n");
      printf("j) Rule WP_PceObjectSearch \n");
      printf("r: Exit to WinMon and Reboot\n");
      printf("x: Exit to WinMon\n");
      printf("Enter your command:");

      cmd = getchar();
     
      if (cmd == '1') {
         CLI_StatMenu();
      }
      if (cmd == '2') {
         for(ii=NUM_OF_FLOWS; ii < 2*NUM_OF_FLOWS; ii++)
         {
            WPE_HostSendPacket(FILTER_SET_D, ii);
         }
//         WPE_HostSendPacket(0,0);
      }
      if (cmd == '3') {
         for(ii=NUM_OF_FLOWS; ii < 2*NUM_OF_FLOWS; ii++)
         {
            WPE_HostSendPacket(FILTER_SET_E, ii);
         }
//         WPE_CheckReceivedTraffic(0);
      }
      if (cmd == 'c') {
         WP_Display(WP_WINPATH(DEFAULT_WPID), PCE_rule_handle[0], 0, 0);
      }
      if (cmd == 'd') {

         
         WP_Display(WP_WINPATH(DEFAULT_WPID), WP_DISPLAY_PCE_LEARNING_QUEUE, 0, 0);

         WP_PceLearningQueueService(WP_WINPATH(DEFAULT_WPID), &(learned_rules_list[0]), 10, &rules_returned);

         printf("learnt %d rules\n", rules_returned);
         
         for (ii =0; ii<rules_returned; ii++)
         {
            memcpy(&rule_fwd, &(learned_rules_list[ii].fwd_rule), sizeof(WP_pce_rule_forwarding));
            rule_fwd.enabled = WP_ENABLE;

            rule_fwd.match_result[2].result_type = WP_PCE_RESULT_FLOW_AGG;
            rule_fwd.match_result[2].param.flow_agg.flow_aggregation = flow_agg[0];
            
            rule_fwd.match_result[3].result_type = WP_PCE_RESULT_LAST;
            
            if (learned_rules_list[ii].params.edp == 0)
            {
               PCE_fwd_handle[ii] = WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID),
                                                     WP_PCE_RULE_FORWARDING,
                                                     &rule_fwd);
//               terminate_on_error(PCE_fwd_handle[ii], "WP_PceRuleCreate",__LINE__);
            }
         }
      }
      if (cmd == 'e') {

         for (ii =0; ii<rules_returned; ii++)
         {
            if (PCE_fwd_handle[ii])
            {
               status = WP_PceRuleDelete(PCE_fwd_handle[ii]);
               terminate_on_error(status, "WP_PceRuleDelete",__LINE__);
               PCE_fwd_handle[ii] = 0;
            }
         }
      }
      if (cmd == 'f') {
         
         
         rule_handle = WP_PceIteratorGetFirst(aging_iterator_handle);
         if (WP_ERROR(rule_handle) == WP_ERR_PCE_ITERATOR_NO_ITEMS)
            printf("\nIterator no items\n");
         else
         {
            //      printf("\n\nDeleting aged Rule\n");
            
            status = WP_PceRuleDelete(rule_handle);
            if (WP_ERROR_P(status))
               printf("\n\nError deleting aged Rule\n\n");
         }
      }
      if (cmd == 'g') {
           while(1)
            {
               rule_handle = WP_PceIteratorGetNext(aging_iterator_handle);
               if (WP_ERROR(rule_handle) == WP_ERR_PCE_ITERATOR_NO_ITEMS)
               {
                  printf("\nIterator no items\n");
                  break;
               }

               //        printf("\n\nDeleting aged Rule\n");
               
               status = WP_PceRuleDelete(rule_handle);
               if (WP_ERROR_P(status))
                  printf("\n\nError deleting aged Rule\n\n");
            }
      }
      if (cmd == 'h') {
         if (!rule_iterator_handle)
         {
            rule_iterator_handle = WP_PceIteratorCreate(WP_WINPATH(DEFAULT_WPID), 
                                                        WP_PCE_ITERATOR_TYPE_RULE, (void *)&PCE_filter[FILTER_SET_A_FORWARDING]);
            terminate_on_error(rule_iterator_handle,"WP_PceIteratorCreate",__LINE__);        
         }
         
         rule_handle = WP_PceIteratorGetFirst(rule_iterator_handle);
         if (WP_ERROR(rule_handle) == WP_ERR_PCE_ITERATOR_NO_ITEMS)
            printf("\nIterator no items\n");
         else
         {
            WP_Display(WP_WINPATH(DEFAULT_WPID), rule_handle, 0, 0);

            status = WP_PceRuleDelete(rule_handle);
            terminate_on_error(status, "WP_PceRuleDelete",__LINE__);
         }
      }
      if (cmd == 'i') {
           while(1)
            {
               rule_handle = WP_PceIteratorGetNext(rule_iterator_handle);
               if (WP_ERROR(rule_handle) == WP_ERR_PCE_ITERATOR_NO_ITEMS)
               {
                  printf("\nIterator no items\n");
                  break;
               }

               WP_Display(WP_WINPATH(DEFAULT_WPID), rule_handle, 0, 0);
            }
      }
      if (cmd == 'j') {

         rule_fwd.rule_fields[0].value.mac_addr[5] = 0; 

         rule_handle = WP_PceObjectSearch (WP_WINPATH(DEFAULT_WPID),
                                           WP_PCE_OBJECT_TYPE_RULE_FORWARDING,
                                           &rule_fwd);
         if (WP_ERROR(rule_handle) == WP_ERR_PCE_OBJECT_NOT_FOUND)
         {
            printf("\nError WP_PceObjectSearch no object found \n\n");
         }
         else
         {
            WP_Display(WP_WINPATH(DEFAULT_WPID), rule_handle, 0, 0);
         }

         rule_fwd.rule_fields[0].value.mac_addr[5] = 1; 

         rule_handle = WP_PceObjectSearch (WP_WINPATH(DEFAULT_WPID),
                                           WP_PCE_OBJECT_TYPE_RULE_FORWARDING,
                                           &rule_fwd);
         if (WP_ERROR(rule_handle) == WP_ERR_PCE_OBJECT_NOT_FOUND)
         {
            printf("\nError WP_PceObjectSearch no object found \n\n");
         }
         else
         {
            WP_Display(WP_WINPATH(DEFAULT_WPID), rule_handle, 0, 0);
         }
      }
      else if (cmd == 'x')
      {
         printf("\n\n");
         exit(1);
      }
      if (cmd == 'r')
      {
         printf("\n\n");
         break;
      }
   }
}

void WPE_CheckPceFilterStats(WP_handle filter)
{
   WP_handle status;
   WP_U32 temp1, temp2;
   WP_U8 index = (WP_U8)(filter & 0xFF);

   
   status = WP_PceFilterStatistics (filter, &filter_stat[index]);
   terminate_on_error(status, "WP_PceFilterStatistics()",__LINE__);

   printf("PCE Filter %s Statistic\n", filters_string[index - 1]);

   temp1 = filter_stat[index].denied_pkts;
   temp2 = filter_stat[index].denied_pkts >> 32;
   printf("denied_pkts            %08X %08X\n", temp2, temp1);

   temp1 = filter_stat[index].matched_pkts;
   temp2 = (filter_stat[index].matched_pkts) >> 32;
   printf("matched_pkts           %08X %08X\n", temp2, temp1);

   temp1 = filter_stat[index].no_matched_pkts;
   temp2 = filter_stat[index].no_matched_pkts >> 32;
   printf("no_matched_pkts        %08X %08X\n", temp2, temp1);

   printf("\n\n");
}

static void WPE_LayerTwoSystemCreate(void)
{
   WP_status status;
   WP_handle iwp1;

   /* Call DriverInit, but if it is already initialized, release WDDI and call it again */
   status = WP_DriverInit();
   if (WP_ERROR(status) == WP_ERR_WDDI_ALREADY_INITIALIZED)
   {
      status = WP_DriverRelease();
      terminate_on_error(status, "WP_DriverRelease", __LINE__);

      status = WP_DriverInit();
   }
   terminate_on_error(status, "WP_DriverInit",__LINE__);
printf ("WPE_LayerTwoSystemCreate: WP_DriverRelease() OK\n");

   int_ram_partition.policer_entries = 0;
   int_ram_partition.pce_hw_rules_num = pce_init.hardware_rules_num;
   WT_SetIntRamPartition(WP_WINPATH(DEFAULT_WPID), &int_ram_partition);
printf ("WPE_LayerTwoSystemCreate: WT_SetIntRamPartition() OK\n");

   status = WP_SysInit(WP_WINPATH(DEFAULT_WPID), context);
   terminate_on_error(status, "WP_SysInit",__LINE__);
printf ("WPE_LayerTwoSystemCreate: WP_SysInit() OK\n");

   status = WPX_BoardConfigure(WP_WINPATH(DEFAULT_WPID), WPX_CONFIGURE_DEFAULT_CHECKIN);
   terminate_on_error(status, "WPX_BoardConfigure()", __LINE__);
printf ("WPE_LayerTwoSystemCreate: WPX_BoardConfigure() OK\n");

   status = WPX_BoardSerdesInit(WP_WINPATH(DEFAULT_WPID), ENET_PORT, WP_TRUE);
   terminate_on_error(status, "WPX_BoardSerdesInit()", __LINE__);      
printf ("WPE_LayerTwoSystemCreate: WPX_BoardSerdesInit() OK\n");

   /* Check only once */
   if (!Checked)
   {
      /* Check PCE Module Init scenarios */
      WP_CheckPceModuleInitScenarios();
      Checked = WP_TRUE;
printf ("WPE_LayerTwoSystemCreate: WP_CheckPceModuleInitScenarios() OK\n");
   }
   

   if (TestType == TEST_SW_FDB_ONLY)
   {      
      pce_init.enhanced_mode = WP_SW_FDB_ENABLE_MODE;
      pce_init.sw_fdb_info->sw_fdb_bus = WP_BUS_HOST;
      pce_init.sw_fdb_info->fdb_threshold = 0;
      pce_init.sw_fdb_info->max_fdb_rules = 64;
      pce_init.pce_discard_mode = WP_PCE_DISCARD_MODE_ALL;
printf ("WPE_LayerTwoSystemCreate: TestType == TEST_SW_FDB_ONLY() OK\n");
   }
   else  if (TestType == TEST_HYBRID_FDB)
   {      
      pce_init.enhanced_mode = WP_SW_FDB_ENABLE_MODE;
      pce_init.sw_fdb_info->sw_fdb_bus = WP_BUS_PARAM;
      pce_init.sw_fdb_info->fdb_threshold = 20;
      pce_init.sw_fdb_info->max_fdb_rules = 128;
      pce_init.pce_discard_mode = WP_PCE_DISCARD_MODE_SMART_DISCARD;
printf ("WPE_LayerTwoSystemCreate: TestType == TEST_HYBRID_FDB() OK\n");
   } 
   else 
   {      
      pce_init.enhanced_mode = WP_NO_ENHANCMENT;
      pce_init.pce_discard_mode = WP_PCE_DISCARD_MODE_ALL;
printf ("WPE_LayerTwoSystemCreate: TestType == else () OK\n");
   }
   pce_init.hardware_rules_num = 200;
   pce_init.parser_info = &pce_parser_info;
   status = WP_ModuleInit(WP_SYSHANDLE(DEFAULT_WPID), WP_WDDI_MODULE_PCE, &pce_init);
   terminate_on_error(status, "WPI_PceModuleInit()",__LINE__);
printf ("WPE_LayerTwoSystemCreate: WP_ModuleInit() OK\n");
   
   free_pce_hw_rules = pce_init.hardware_rules_num;

   /*Creates buffer pools*/
   pool_2048 = WP_PoolCreate(WP_WINPATH(DEFAULT_WPID), WP_pool_iwbuffer, buffer_data_2048);
   terminate_on_error(pool_2048, "WP_PoolCreate() pool_2048",__LINE__);

   pool_144 = WP_PoolCreate(WP_WINPATH(DEFAULT_WPID), WP_pool_buffer, buffer_data_144);
   terminate_on_error(pool_144, "WP_PoolCreate() pool_144",__LINE__);
printf ("WPE_LayerTwoSystemCreate: WP_PoolCreate() OK\n");

   /* Create an interworking queue node */
   qn_iw->adjunct_pool = pool_2048;
   qniw = WP_QNodeCreate(WP_WINPATH(DEFAULT_WPID),WP_QNODE_IWQ| WP_QNODE_OPT_FMU, qn_iw);
   terminate_on_error(qniw, "WP_QNodeCreate() qniw",__LINE__);
printf ("WPE_LayerTwoSystemCreate: WP_QNodeCreate() OK\n");


/*---------------------------------------*\
	port & devices create
	port/devie: 		
		iwp1/devhost, gbe_port1/gbe_dev1
\*---------------------------------------*/

   /* Create an IW Host port */
   iwp1 = WP_PortCreate(WP_WINPATH(DEFAULT_WPID), WP_PORT_IW_HOST, NULL);
   terminate_on_error(iwp1, "WP_PortCreate() IW Host",__LINE__);
printf ("WPE_LayerTwoSystemCreate: WP_PortCreate(iwp1/WP_PORT_IW_HOST) OK\n");

   /* Create an IW Host Device */
   devhost = WP_DeviceCreate(iwp1, 0, WP_DEVICE_IW_HOST, NULL);
   terminate_on_error(devhost, "WP_DeviceCreate() IW Host",__LINE__);
printf ("WPE_LayerTwoSystemCreate: WP_DeviceCreate(devhost) OK\n");

   /* Create an GEnet Port (the device that use "regular" mode) */
   enet_s_fast->interface_mode = WP_ENET_SGMII_1000;
   gbe_port1 = WP_PortCreate(WP_WINPATH(DEFAULT_WPID), ENET_PORT, enet_s_fast);
   terminate_on_error(gbe_port1, "WP_PortCreate()",__LINE__);
printf ("WPE_LayerTwoSystemCreate: WP_PortCreate(ENET_PORT/gbe_port1) OK\n");

   /* Create an GEnet Device (the device that use "regular" mode) */
   enet_dev->max_tx_channels = 1;

   enet_dev->extended_params = NULL;
   gbe_dev1 = WP_DeviceCreate(gbe_port1, 0,WP_DEVICE_ENET, enet_dev);
   terminate_on_error(gbe_dev1, "WP_DeviceCreate() GENET13",__LINE__);
printf ("WPE_LayerTwoSystemCreate: WP_DeviceCreate(gbe_dev1) OK\n");

   enet_dev->max_tx_channels = MAX_NUM_OF_CHANNELS;
   enet_dev->extended_params = enet_device_extended_config;
   status = WP_DeviceModify(gbe_dev1,
                            WP_DEV_MOD_ENET_TX_STATMODE |
                            WP_DEV_MOD_ENET_RX_STATMODE ,
                            enet_dev);
   terminate_on_error(status, "WP_DeviceModify() GENET13",__LINE__);
printf ("WPE_LayerTwoSystemCreate: WP_DeviceModify(gbe_dev1) OK\n");
}


static void terminate_on_error(WP_handle handle, WP_CHAR *s,WP_U32 line)
{
   if (WP_ERROR_P(handle)) {
      printf("%s: Terminal error (handle 0x%x) %s\n", appname, handle, s);
      printf("%s: Error type - %s\n", appname, WP_ErrorString(handle));
      printf("line = %d\n",line);
      printf("Test Failed\n");

      WP_DriverRelease();

      WT_Reboot();
      exit(0);
   }
}

static void WP_CheckPceModuleInitScenarios(void)
{
   WP_status status;
	WP_status s1 = 0;

	s1 = WP_ERR_WINFARM_UNAVAILABLE;

   pce_init.enhanced_mode = WP_SW_FDB_ENABLE_MODE;
   pce_init.sw_fdb_info->fdb_threshold = 0;
   pce_init.sw_fdb_info->max_fdb_rules = 0;
   status = WP_ModuleInit(WP_SYSHANDLE(DEFAULT_WPID), WP_WDDI_MODULE_PCE, &pce_init);
#if USE_SHIFT_ENUM	
   check_error(status, "WPI_PceModuleInit()" ,__LINE__, WP_ERR_SW_FDB_INITIALIZATION_ERROR);
#else
   check_error(status, "WPI_PceModuleInit()" ,__LINE__, s1);
#endif
   printf("\nTest SW FDB enabled, Max Fdb Rules = 0 -- PASSED\n");

   pce_init.enhanced_mode = WP_SW_FDB_ENABLE_MODE;
   pce_init.sw_fdb_info->fdb_threshold = 0;
   pce_init.sw_fdb_info->max_fdb_rules = 262144;	// 256k
   status = WP_ModuleInit(WP_SYSHANDLE(DEFAULT_WPID), WP_WDDI_MODULE_PCE, &pce_init);
#if USE_SHIFT_ENUM	
   check_error(status, "WPI_PceModuleInit()" ,__LINE__, WP_ERR_SW_FDB_INITIALIZATION_ERROR);
#else
   check_error(status, "WPI_PceModuleInit()" ,__LINE__, s1);
#endif
   printf("\nTest SW FDB enabled, Max Fdb Rules above 256K -- PASSED\n");

   pce_init.enhanced_mode = WP_SW_FDB_ENABLE_MODE;
   pce_init.hardware_rules_num = 40;
   pce_init.sw_fdb_info->fdb_threshold = 1000;
   pce_init.sw_fdb_info->max_fdb_rules = 100;
   status = WP_ModuleInit(WP_SYSHANDLE(DEFAULT_WPID), WP_WDDI_MODULE_PCE, &pce_init);
#if USE_SHIFT_ENUM	
   check_error(status, "WPI_PceModuleInit()" ,__LINE__, WP_ERR_SW_FDB_INITIALIZATION_ERROR);
#else
   check_error(status, "WPI_PceModuleInit()" ,__LINE__, s1);

#endif
   printf("\nTest SW FDB enabled, num of HW rules are less then the threshold -- PASSED \n");
}

static void check_error(WP_handle handle, WP_CHAR *s, WP_U32 line, WP_U32 excpected_error)
{
   WP_U32 err = 0;
   
   if (WP_ERROR_P(handle)) 
   {
      if (WP_ERR_IW_NAT_ALLOCATION < excpected_error)
      {
         if ((WP_ERROR(handle) != (excpected_error - 1)))
            err = 1;
      } else {
         if ((WP_ERROR(handle) != excpected_error))
            err = 1;
      }
   }
   else
   {
      if (excpected_error != WP_OK)
         err = 1;
   }
   
   if (err)
   {
      printf("%s: Terminal error (handle 0x%x) %s\n", appname, handle, s);
      printf("%s: Error type - %s\n", appname, WP_ErrorString(handle));
      printf("line = %d\n",line);
      printf("Test Failed\n");
      
      WP_DriverRelease();
      
//       WT_Reboot();
      exit(0);
   }
}

static void terminate(WP_CHAR *s, WP_U32 line)
{
   printf("Error %s, line = %d\n",s, line);
   printf("Test Failed\n");

   WP_DriverRelease();

#if 0
   WT_Reboot();
#endif
   exit(0);
}

WP_U32 WPT_ShowChannelQueueSize(WP_handle rx_host_handle)
{
   WP_U32 qdepth = 0;
   WP_status status;

   status = WP_ChannelQDepth(rx_host_handle,
                             &qdepth);
   printf ("WPT_ShowChannelQueueSize: qdepth(%d)\n", qdepth);
   return (qdepth - 1);
}

WP_U32 WPT_HostChannelQueueSizeX (WP_handle rx_host_handle)
{
   WP_U32 qdepth = 0;
   WP_status status;

   status = WP_ChannelQDepth(rx_host_handle,
                             &qdepth);
   terminate_on_error(status, "WP_ChannelQDepth", __LINE__);
   printf ("WPT_HostChannelQueueSize: qdepth(%d)\n", qdepth);
   return (qdepth);
}

WP_U32 WPT_HostChannelQueueSize(WP_handle rx_host_handle)
{
   WP_U32 qdepth = 0;
   WP_status status;

   status = WP_ChannelQDepth(rx_host_handle,
                             &qdepth);
   terminate_on_error(status, "WP_ChannelQDepth", __LINE__);
   printf ("WPT_HostChannelQueueSize: qdepth(%d)\n", qdepth);
   return (qdepth - 1);
}

WP_U32 WPT_LocalDisplayDeviceStats(WP_handle enet_handle)
{
   WP_stats_enet enet_stats;
   WP_status status;

   memset (&enet_stats,0,sizeof(WP_stats_enet));

   status = WP_DeviceStatistics(enet_handle, &enet_stats);
   if (status)
   {
      printf("Error Reading Enet Statistics\n");
      exit(1);
   }
   printf("\n----------------------------------------------------\n");

   printf ("ENET Port Statistics (HW)\n");
#if 0
   printf ("TxRx Frame  64:              %08x%08x\n",
           ((WPT_StatField)enet_stats.txrx_frames_64).part.high,
           ((WPT_StatField)enet_stats.txrx_frames_64).part.low);

   printf ("Tx Bytes:                    %08x%08x\n",
           ((WPT_StatField)enet_stats.tx_bytes).part.high,
           ((WPT_StatField)enet_stats.tx_bytes).part.low);

   printf ("TxRx Frame 127:              %08x%08x\n",
           ((WPT_StatField)enet_stats.txrx_frames_127).part.high,
           ((WPT_StatField)enet_stats.txrx_frames_127).part.low);

   printf ("Tx Packets:                  %08x%08x\n",
           ((WPT_StatField)enet_stats.tx_packets).part.high,
           ((WPT_StatField)enet_stats.tx_packets).part.low);

   printf ("TxRx Frame 255:              %08x%08x\n",
           ((WPT_StatField)enet_stats.txrx_frames_255).part.high,
           ((WPT_StatField)enet_stats.txrx_frames_255).part.low);

   printf ("Tx Multicast:                %08x%08x\n",
           ((WPT_StatField)enet_stats.tx_multicast).part.high,
           ((WPT_StatField)enet_stats.tx_multicast).part.low);

   printf ("TxRx Frame 1023:             %08x%08x\n",
           ((WPT_StatField)enet_stats.txrx_frames_1023).part.high,
           ((WPT_StatField)enet_stats.txrx_frames_1023).part.low);

   printf ("Tx Broadcast:                %08x%08x\n",
           ((WPT_StatField)enet_stats.tx_broadcast).part.high,
           ((WPT_StatField)enet_stats.tx_broadcast).part.low);

   printf ("TxRx Frame 1518:             %08x%08x\n",
           ((WPT_StatField)enet_stats.txrx_frames_1518).part.high,
           ((WPT_StatField)enet_stats.txrx_frames_1518).part.low);

   printf ("Tx Mac Pause:                %08x%08x\n",
           ((WPT_StatField)enet_stats.tx_mac_pause).part.high,
           ((WPT_StatField)enet_stats.tx_mac_pause).part.low);

   printf ("TxRx Frame 1522:             %08x%08x\n",
           ((WPT_StatField)enet_stats.txrx_frames_1522).part.high,
           ((WPT_StatField)enet_stats.txrx_frames_1522).part.low);

   printf ("Tx Defer:                    %08x%08x\n",
           ((WPT_StatField)enet_stats.tx_defer).part.high,
           ((WPT_StatField)enet_stats.tx_defer).part.low);

   printf ("Rx Bytes:                    %08x%08x\n",
           ((WPT_StatField)enet_stats.rx_bytes).part.high,
           ((WPT_StatField)enet_stats.rx_bytes).part.low);

   printf ("Tx Exess Defer:              %08x%08x\n",
           ((WPT_StatField)enet_stats.tx_excess_defer).part.high,
           ((WPT_StatField)enet_stats.tx_excess_defer).part.low);

   printf ("Rx Packets:                  %08x%08x\n",
           ((WPT_StatField)enet_stats.rx_packets).part.high,
           ((WPT_StatField)enet_stats.rx_packets).part.low);

   printf ("Tx Single Collision:         %08x%08x\n",
           ((WPT_StatField)enet_stats.tx_single_collision).part.high,
           ((WPT_StatField)enet_stats.tx_single_collision).part.low);

   printf ("Rx Error FCS:                %08x%08x\n",
           ((WPT_StatField)enet_stats.rx_err_fcs).part.high,
           ((WPT_StatField)enet_stats.rx_err_fcs).part.low);

   printf ("Tx Multi Collision:          %08x%08x\n",
           ((WPT_StatField)enet_stats.tx_multi_collision).part.high,
           ((WPT_StatField)enet_stats.tx_multi_collision).part.low);

   printf ("Rx Multicast:                %08x%08x\n",
           ((WPT_StatField)enet_stats.rx_multicast).part.high,
           ((WPT_StatField)enet_stats.rx_multicast).part.low);

   printf ("Tx Late Collision:           %08x%08x\n",
           ((WPT_StatField)enet_stats.tx_late_collision).part.high,
           ((WPT_StatField)enet_stats.tx_late_collision).part.low);

   printf ("Rx Broadcast:                %08x%08x\n",
           ((WPT_StatField)enet_stats.rx_broadcast).part.high,
           ((WPT_StatField)enet_stats.rx_broadcast).part.low);

   printf ("Tx Excess Collision:         %08x%08x\n",
           ((WPT_StatField)enet_stats.tx_excess_collision).part.high,
           ((WPT_StatField)enet_stats.tx_excess_collision).part.low);

   printf ("Rx Mac Control:              %08x%08x\n",
           ((WPT_StatField)enet_stats.rx_mac_control).part.high,
           ((WPT_StatField)enet_stats.rx_mac_control).part.low);

   printf ("Tx No Collision:             %08x%08x\n",
           ((WPT_StatField)enet_stats.tx_no_collision).part.high,
           ((WPT_StatField)enet_stats.tx_no_collision).part.low);

   printf ("Rx Mac Pause:                %08x%08x\n",
           ((WPT_StatField)enet_stats.rx_mac_pause).part.high,
           ((WPT_StatField)enet_stats.rx_mac_pause).part.low);

   printf ("Tx Mac Pause Honored:        %08x%08x\n",
           ((WPT_StatField)enet_stats.tx_mac_pause_honored).part.high,
           ((WPT_StatField)enet_stats.tx_mac_pause_honored).part.low);

/*---------------------------------------*\
	morris
\*---------------------------------------*/
   printf ("Rx Mac Unknown:              %08x%08x\n",
           ((WPT_StatField)enet_stats.rx_mac_unknown))->part.high,
           ((WPT_StatField)enet_stats.rx_mac_unknown))->part.low);

   printf ("Tx Dropped:                  %08x%08x\n",
           ((WPT_StatField)enet_stats.tx_dropped))->part.high,
           ((WPT_StatField)enet_stats.tx_dropped))->part.low);

   printf ("Rx Error Alignment:          %08x%08x\n",
           ((WPT_StatField)enet_stats.rx_err_alignment))->part.high,
           ((WPT_StatField)enet_stats.rx_err_alignment))->part.low);

   printf ("Tx Jabber:                   %08x%08x\n",
           ((WPT_StatField)enet_stats.tx_jabber))->part.high,
           ((WPT_StatField)enet_stats.tx_jabber))->part.low);

   printf ("Rx Error LEN:                %08x%08x\n",
           ((WPT_StatField)enet_stats.rx_err_length))->part.high,
           ((WPT_StatField)enet_stats.rx_err_length))->part.low);

   printf ("Tx Errors FCS:               %08x%08x\n",
           ((WPT_StatField)enet_stats.tx_err_fcs))->part.high,
           ((WPT_StatField)enet_stats.tx_err_fcs))->part.low);

   printf ("Rx Error Code:               %08x%08x\n",
           ((WPT_StatField)enet_stats.rx_err_code))->part.high,
           ((WPT_StatField)enet_stats.rx_err_code))->part.low);

   printf ("Tx Control:                  %08x%08x\n",
           ((WPT_StatField)enet_stats.tx_control))->part.high,
           ((WPT_StatField)enet_stats.tx_control))->part.low);

   printf ("Rx False Carrier:            %08x%08x\n",
           ((WPT_StatField)enet_stats.rx_false_carrier))->part.high,
           ((WPT_StatField)enet_stats.rx_false_carrier))->part.low);

   printf ("Tx Oversize:                 %08x%08x\n",
           ((WPT_StatField)enet_stats.tx_oversize))->part.high,
           ((WPT_StatField)enet_stats.tx_oversize))->part.low);

   printf ("Rx Undersize:                %08x%08x\n",
           ((WPT_StatField)enet_stats.rx_undersize))->part.high,
           ((WPT_StatField)enet_stats.rx_undersize))->part.low);

   printf ("Tx Undersize:                %08x%08x\n",
           ((WPT_StatField)enet_stats.tx_undersize))->part.high,
           ((WPT_StatField)enet_stats.tx_undersize))->part.low);

   printf ("Rx Oversize:                 %08x%08x\n",
           ((WPT_StatField)enet_stats.rx_oversize))->part.high,
           ((WPT_StatField)enet_stats.rx_oversize))->part.low);

   printf ("Tx Fragments:                %08x%08x\n",
           ((WPT_StatField)enet_stats.tx_fragments))->part.high,
           ((WPT_StatField)enet_stats.tx_fragments))->part.low);

   printf ("Rx Fragments:                %08x%08x\n",
           ((WPT_StatField)enet_stats.rx_fragments))->part.high,
           ((WPT_StatField)enet_stats.rx_fragments))->part.low);
   printf ("Rx Jabber:                   %08x%08x\n",
           ((WPT_StatField)enet_stats.rx_jabber))->part.high,
           ((WPT_StatField)enet_stats.rx_jabber))->part.low);
   printf ("Rx Dropped:                  %08x%08x\n",
           ((WPT_StatField)enet_stats.rx_dropped))->part.high,
           ((WPT_StatField)enet_stats.rx_dropped))->part.low);
#endif
   printf ("HS ENET Device Statistics (DPS)\n");

   printf ("Rx Host Frames:              %08x%08x\n",
           ((WPT_StatField*)&(enet_stats.rx_host_frames))->part.high,
           ((WPT_StatField*)&(enet_stats.rx_host_frames))->part.low);
   printf ("Rx Iw Frames:                %08x%08x\n",
           ((WPT_StatField*)&(enet_stats.rx_iw_frames))->part.high,
           ((WPT_StatField*)&(enet_stats.rx_iw_frames))->part.low);
   printf ("Rx Error Host Full:          %08x%08x\n",
           ((WPT_StatField*)&(enet_stats.rx_err_host_full))->part.high,
           ((WPT_StatField*)&(enet_stats.rx_err_host_full))->part.low);
   printf ("Rx Error Fbp Underrun:       %08x%08x\n",
           ((WPT_StatField*)&(enet_stats.rx_err_fbp_underrun))->part.high,
           ((WPT_StatField*)&(enet_stats.rx_err_fbp_underrun))->part.low);
   printf ("Rx Error Nonvalid Mac:       %08x%08x\n",
           ((WPT_StatField*)&(enet_stats.rx_err_nonvalid_mac))->part.high,
           ((WPT_StatField*)&(enet_stats.rx_err_nonvalid_mac))->part.low);
   printf ("Rx Error Mru:                %08x%08x\n",
           ((WPT_StatField*)&(enet_stats.rx_err_mru))->part.high,
           ((WPT_StatField*)&(enet_stats.rx_err_mru))->part.low);
   printf ("Rx Error Sdu:                %08x%08x\n",
           ((WPT_StatField*)&(enet_stats.rx_err_sdu))->part.high,
           ((WPT_StatField*)&(enet_stats.rx_err_sdu))->part.low);
   printf ("Tx Error Underrun:           %08x%08x\n",
           ((WPT_StatField*)&(enet_stats.tx_err_underrun))->part.high,
           ((WPT_StatField*)&(enet_stats.tx_err_underrun))->part.low);
   printf ("Rx Error Overrun:            %08x%08x\n",
           ((WPT_StatField*)&(enet_stats.rx_err_overrun))->part.high,
           ((WPT_StatField*)&(enet_stats.rx_err_overrun))->part.low);
   printf ("Tx Frames:                   %08x%08x\n",
           ((WPT_StatField*)&(enet_stats.tx_frames))->part.high,
           ((WPT_StatField*)&(enet_stats.tx_frames))->part.low);
   printf("----------------------------------------------------\n");

   return 0;
}

void WPT_DisplayRoutingPortStats( WP_handle port_handle, WP_CHAR* type)
{
  WP_route_port_stats rport_stats = {0};
  WP_status status = 0;

  memset(&rport_stats, 0, sizeof(WP_route_port_stats));
  status = WP_IwPortStatistics(port_handle, &rport_stats);
  terminate_on_error(status, "WP_IwPortStatistics()",__LINE__);
  printf ("\n%s Route port statistics of port %d (%x)\n",
          type,(port_handle & 0x000000ff),port_handle);
  printf("--------- GBE Port -----------\n");
  printf("rx_valid_packets = %d\n",(WP_U32)rport_stats.rx_valid_packets);
  printf("rx_ipv4_option_packets = %d\n",(WP_U32)rport_stats.rx_ipv4_option_packets);
  printf("rx_non_ip_packets = %d\n",(WP_U32)rport_stats.rx_non_ip_packets);
  printf("rx_compressed_packets = %d\n",(WP_U32)rport_stats.rx_compressed_packets);
  printf("rx_valid_mpls_packets = %d\n",(WP_U32)rport_stats.rx_valid_mpls_packets);
  printf("rx_protocol_error = %d\n",(WP_U32)rport_stats.rx_protocol_error);
  printf("rx_checksum_error = %d\n",(WP_U32)rport_stats.rx_checksum_error);
  printf("rx_discard_classifier = %d\n",(WP_U32)rport_stats.rx_discard_classifier);
  printf("rx_mpls_lookup_drop = %d\n",(WP_U32)rport_stats.rx_mpls_lookup_drop);
  printf("tx_forwarded_packets = %d\n",(WP_U32)rport_stats.tx_forwarded_packets);
  printf("\n");
}

void WPT_DisplayBridgingPortStats( WP_handle port_handle, WP_CHAR* type)
{
   WP_U32 rx_packets = 0;
   WP_U32 i;
   WP_bport_stats bport_stats;
   memset( &bport_stats, 0, sizeof(WP_bport_stats) );
   WP_IwBridgingStatistics( port_handle, &bport_stats );

   printf ("\n%s Bridge port statistics of port %d (%x)\n",
           type,(port_handle & 0x000000ff),port_handle);
   printf ("============================================\n");
   printf ("Rx Valid Packets:               %08x%08x\n",
           ((WPT_StatField*)&bport_stats.rx_valid_packets)->part.high,
           ((WPT_StatField*)&bport_stats.rx_valid_packets)->part.low);
   rx_packets = ((WPT_StatField*)&bport_stats.rx_valid_packets)->part.low;

   printf ("Discard VLAN acceptable filter: %08x%08x\n",
           ((WPT_StatField*)&bport_stats.discard_vlan_acceptable_filter)->part.high,
           ((WPT_StatField*)&bport_stats.discard_vlan_acceptable_filter)->part.low);
   printf ("Discard ingress filter:         %08x%08x\n",
           ((WPT_StatField*)&bport_stats.discard_ingress_filter)->part.high,
           ((WPT_StatField*)&bport_stats.discard_ingress_filter)->part.low);
   printf ("Discard bridge classifier:      %08x%08x\n",
           ((WPT_StatField*)&bport_stats.discard_bridge_classifier)->part.high,
           ((WPT_StatField*)&bport_stats.discard_bridge_classifier)->part.low);
   printf ("Discard unknown MAC SA:         %08x%08x\n",
           ((WPT_StatField*)&bport_stats.discard_unk_macsa)->part.high,
           ((WPT_StatField*)&bport_stats.discard_unk_macsa)->part.low);
   printf ("Deny MAC SA:                    %08x%08x\n",
           ((WPT_StatField*)&bport_stats.deny_mac_sa)->part.high,
           ((WPT_StatField*)&bport_stats.deny_mac_sa)->part.low);
   printf ("Deny MAC DA:                    %08x%08x\n",
           ((WPT_StatField*)&bport_stats.deny_mac_da)->part.high,
           ((WPT_StatField*)&bport_stats.deny_mac_da)->part.low);
   printf ("Rx BC valid packets:            %08x%08x\n",
           ((WPT_StatField*)&bport_stats.rx_bc_valid_packets)->part.high,
           ((WPT_StatField*)&bport_stats.rx_bc_valid_packets)->part.low);
   printf ("Rx MC valid packets:            %08x%08x\n",
           ((WPT_StatField*)&bport_stats.rx_mc_valid_packets)->part.high,
           ((WPT_StatField*)&bport_stats.rx_mc_valid_packets)->part.low);
   printf ("Forwarded UC packets:           %08x%08x\n",
           ((WPT_StatField*)&bport_stats.forwarded_uc_packets)->part.high,
           ((WPT_StatField*)&bport_stats.forwarded_uc_packets)->part.low);
   printf ("Forwarded BC packets:           %08x%08x\n",
           ((WPT_StatField*)&bport_stats.forwarded_bc_packets)->part.high,
           ((WPT_StatField*)&bport_stats.forwarded_bc_packets)->part.low);
   printf ("Forwarded MC packets:           %08x%08x\n",
           ((WPT_StatField*)&bport_stats.forwarded_mc_packets)->part.high,
           ((WPT_StatField*)&bport_stats.forwarded_mc_packets)->part.low);

#if 1
   for (i=0;i<5;i++)
   {
      printf ("DFC Filter %d Discard:           %08x%08x\n",i,
              ((WPT_StatField*)&bport_stats.discard_dfc_filter[i])->part.high,
              ((WPT_StatField*)&bport_stats.discard_dfc_filter[i])->part.low);
   }

   for(i=0; i<NUM_OF_PCE_FILTERS; i++)
   {
      if (i < 10)
         printf ("PCE Filter %d denied packets:    %08x%08x\n", i,
                 ((WPT_StatField*)&bport_stats.discard_pce_filter[i])->part.high,
                 ((WPT_StatField*)&bport_stats.discard_pce_filter[i])->part.low);
      else
         printf ("PCE Filter %d denied packets:   %08x%08x\n", i,
                 ((WPT_StatField*)&bport_stats.discard_pce_filter[i])->part.high,
                 ((WPT_StatField*)&bport_stats.discard_pce_filter[i])->part.low);
   }
#endif

   printf("\n");
}

static void WPE_SystemEnable(void)
{
   WP_status status;

   /* Enable the L2pi Port (for fast "regular") */
   status = WP_PortEnable(gbe_port1, WP_DIRECTION_DUPLEX);
   terminate_on_error(status, "WP_PortEnable gbe_port1",__LINE__);

   /* L2pi-Enet To L2pi-Enet */
   status = WP_DeviceEnable(gbe_dev1, WP_DIRECTION_DUPLEX);
   terminate_on_error(status, "WP_DeviceEnable gbe_dev1", __LINE__);

   status = WP_ChannelEnable(gbe1_rx_ch_handle);
   terminate_on_error(status, "WP_ChannelEnable() rx_enet",__LINE__);

   status = WP_ChannelEnable(gbe1_tx_ch_handle);
   terminate_on_error(status, "WP_ChannelEnable() tx_enet",__LINE__);
}

static void WPE_ChannelsDelete(void)
{
   WP_status status;

   status = WP_ChannelDisable(gbe1_rx_ch_handle);
   terminate_on_error(status, "WP_ChannelDisable() RX_Channel ENET13",__LINE__);

   status = WP_ChannelDelete(gbe1_rx_ch_handle);
   terminate_on_error(status, "WP_ChannelDelete() RX_Channel ENET13",__LINE__);

   status = WP_ChannelDisable(gbe1_tx_ch_handle);
   terminate_on_error(status, "WP_ChannelDisable() TX_Channel ENET13",__LINE__);

   status = WP_ChannelDelete(gbe1_tx_ch_handle);
   terminate_on_error(status, "WP_ChannelDelete() TX_Channel ENET13",__LINE__);

   status = WP_ChannelDelete(rx_host_handle);
   terminate_on_error(status, "WP_ChannelDelete() RX_Channel Host",__LINE__);
}

static void WPE_ChannelsCreate(void)
{
   WP_tag ch_tag = 0;
   WP_status status;

   /* Create Enet1 fast ("reguler") RX channel */
   gbe1_rx_ch_handle = WP_ChannelCreate(ch_tag++,
                                        gbe_dev1,
                                        qniw,
                                        WP_CH_RX,
                                        WP_ENET,
                                        &gbe_ch[0]);
   terminate_on_error(gbe1_rx_ch_handle, "WP_ChannelCreate() RX_Channel ENET13",__LINE__);
printf ("WPE_ChannelsCreate: WP_ChannelCreate(gbe1_rx_ch_handle) OK\n");

   /* Create Enet1 fast ("reguler") TX channel */
   gbe1_tx_ch_handle = WP_ChannelCreate(ch_tag++,
                                        gbe_dev1,
                                        qniw,
                                        WP_CH_TX,
                                        WP_ENET,
                                        &gbe_ch[0]);
   terminate_on_error(gbe1_tx_ch_handle, "WP_ChannelCreate() TX_Channel ENET13",__LINE__);
printf ("WPE_ChannelsCreate: WP_ChannelCreate(gbe1_tx_ch_handle) OK\n");
   
   status =  WP_IwTxBindingCreate(gbe1_tx_ch_handle, WP_IW_TX_BINDING, &tx_binding_l2pi[0]);
   terminate_on_error(status, "WP_IwTxBindingCreate() TX ENET fast device",__LINE__);
printf ("WPE_ChannelsCreate: WP_ChannelCreate(WP_IwTxBindingCreate) OK\n");

   /*create host rx channel*/
   rx_host_handle = WP_ChannelCreate(0x1111,
                                     devhost,
                                     qniw,
                                     WP_CH_RX,
                                     WP_IW_HOST,
                                     &ch_config_iw[0]);
   terminate_on_error(rx_host_handle, "WP_ChannelCreate() Host sys",__LINE__);
printf ("WPE_ChannelsCreate: WP_ChannelCreate(rx_host_handle) OK\n");
}

static void WPE_PortsCreate(WP_handle iw_sys, WP_handle *iwport, WP_U32 max_mac_addresses)
{
   WP_status status;
     
   iwhost_bport_config->flow_agg = default_agg;
   bport_host = WP_IwPortCreate(iw_sys, &iwhost_bport_config[0]);// direct map
   terminate_on_error(bport_host," WP_IwPortCreate()",__LINE__);

   br_agg_gbe->input_bport = bport_host;
   status = WP_IwFlowAggregationModify(default_agg,
                                       WP_IW_FLOW_AGG_B_MOD_BPORT,
                                       &br_agg_gbe[0]);
   terminate_on_error(status," WP_IwFlowAggregationModify()",__LINE__);

   bport_conf.flow_agg = default_agg;
   bport_conf.tag = 0;
   bport_conf.max_mac_addresses = max_mac_addresses;
   
   *iwport = WP_IwPortCreate(iw_sys, &bport_conf);// direct map
   terminate_on_error(*iwport, "WP_IwPortCreate ",__LINE__);
}

static void WPE_RxBindingDelete(WP_handle ch, WP_handle iw_sys)
{
   WP_status status;
   status = WP_IwRxBindingModify(ch,
                                 iw_sys,
                                 qniw,
                                 WP_IW_RX_BIND_MOD_DISABLE,
                                 &rx_binding_cfg[0]);
   terminate_on_error(status, "WP_IwRxBindingModify", __LINE__);
}

static void WPE_DlRxBindingCreate(WP_handle iw_sys)
{
   WP_status status;

   rx_binding_cfg[0].input_port = h_iw_port_general;
   rx_binding_cfg[0].pce_if_handle = 0;
   status = WP_IwRxBindingCreate(gbe1_rx_ch_handle ,iw_sys,
                                 qniw,
                                 &rx_binding_cfg[0]);
   terminate_on_error(status, "WP_IwRxBindingCreate Enet",__LINE__);
}

WP_U32 WPT_CompatibleFaGetFwPacketsCount(WP_handle agg)
{
   WP_iw_flow_stats iw_flow_stats;
   WP_U32 forward_packets = 0;
   WP_status status;

   memset(&iw_flow_stats,0,sizeof(iw_flow_stats));
   status = WP_IwFlowStatistics( agg, WP_IW_FLOW_STAT, &iw_flow_stats);
   terminate_on_error(status , "WP_IwFlowStatistics",__LINE__);
   
   forward_packets = ((WPT_StatField*)&iw_flow_stats.forward_packet)->part.low;

   return forward_packets;
}

void WPT_CompatibleFlowStatistics( WP_handle agg)
{
   WP_iw_flow_stats iw_flow_stats;
   WP_U32 forward_packets = 0;
   WP_status status;

   memset(&iw_flow_stats,0,sizeof(iw_flow_stats));
   status = WP_IwFlowStatistics( agg, WP_IW_FLOW_STAT, &iw_flow_stats);
   terminate_on_error(status , "WP_IwFlowStatistics",__LINE__);

   printf ("Aggregation flow statistics of flow %d (%x)\n",
           (agg & 0x000000ff),agg);
   printf ("=================================================\n");

   printf ("Forward Packet:          %08x%08x\n",
           ((WPT_StatField*)&iw_flow_stats.forward_packet)->part.high,
           ((WPT_StatField*)&iw_flow_stats.forward_packet)->part.low);
   forward_packets = ((WPT_StatField*)&iw_flow_stats.forward_packet)->part.low;

   printf( "FBP Drop Packets:        %08x%08x\n",
           ((WPT_StatField*)&iw_flow_stats.fbp_drop_packets)->part.high,
           ((WPT_StatField*)&iw_flow_stats.fbp_drop_packets)->part.low);

   printf( "MTU Drop Packets:        %08x%08x\n",
           ((WPT_StatField*)&iw_flow_stats.mtu_drop_packets)->part.high,
           ((WPT_StatField*)&iw_flow_stats.mtu_drop_packets)->part.low);

   printf( "TTL Drop Packets:        %08x%08x\n",
           ((WPT_StatField*)&iw_flow_stats.ttl_drop_packets)->part.high,
           ((WPT_StatField*)&iw_flow_stats.ttl_drop_packets)->part.low);

   printf( "TX Queue Drop Packets:   %08x%08x\n",
           ((WPT_StatField*)&iw_flow_stats.tx_queue_drop_packets)->part.high,
           ((WPT_StatField*)&iw_flow_stats.tx_queue_drop_packets)->part.low);

   printf( "MPLS Drop:               %08x%08x\n",
           ((WPT_StatField*)&iw_flow_stats.mpls_drop)->part.high,
           ((WPT_StatField*)&iw_flow_stats.mpls_drop)->part.low);

   printf( "Denied Packets:          %08x%08x\n",
           ((WPT_StatField*)&iw_flow_stats.denied_packets)->part.high,
           ((WPT_StatField*)&iw_flow_stats.denied_packets)->part.low);

   printf( "Group filtered packets:  %08x%08x\n",
           ((WPT_StatField*)&iw_flow_stats.group_filtered_packets)->part.high,
           ((WPT_StatField*)&iw_flow_stats.group_filtered_packets)->part.low);

   printf( "forwarded_bytes:         %08x%08x\n",
           ((WPT_StatField*)&iw_flow_stats.forwarded_bytes)->part.high,
           ((WPT_StatField*)&iw_flow_stats.forwarded_bytes)->part.low);

   printf( "GTP bad headers:         %08x%08x\n",
           ((WPT_StatField*)&iw_flow_stats.gtp_bad_headers)->part.high,
           ((WPT_StatField*)&iw_flow_stats.gtp_bad_headers)->part.low);

   printf( "Policer Non Conforming:  %08x%08x\n",
           ((WPT_StatField*)&iw_flow_stats.policer_non_conforming_packets)->part.high,
           ((WPT_StatField*)&iw_flow_stats.policer_non_conforming_packets)->part.low);

}

void WPT_GenericFlowStatistics( WP_handle agg)
{
   WP_iw_generic_flow_stats iw_flow_stats;
   WP_U32 forward_packets = 0;
   WP_status status;

   memset(&iw_flow_stats,0,sizeof(iw_flow_stats));
   status = WP_IwFlowStatistics( agg, WP_IW_FLOW_STAT_GENERIC, &iw_flow_stats);
   terminate_on_error(status , "WP_IwFlowStatistics",__LINE__);

   printf ("Aggregation flow statistics of flow %d (%x)\n",
           (agg & 0x000000ff),agg);
   printf ("=================================================\n");

   printf ("Forward Packet:          %08x%08x\n",
           ((WPT_StatField*)&iw_flow_stats.forward_packet)->part.high,
           ((WPT_StatField*)&iw_flow_stats.forward_packet)->part.low);
   forward_packets = ((WPT_StatField*)&iw_flow_stats.forward_packet)->part.low;

   printf( "Group filtered packets   %08x%08x\n",
           ((WPT_StatField*)&iw_flow_stats.group_filtered_packets)->part.high,
           ((WPT_StatField*)&iw_flow_stats.group_filtered_packets)->part.low);

   printf( "MTU Drop Packets:        %08x%08x\n",
           ((WPT_StatField*)&iw_flow_stats.mtu_drop_packets)->part.high,
           ((WPT_StatField*)&iw_flow_stats.mtu_drop_packets)->part.low);

   printf( "pecs counter1:           %08x%08x\n",
           ((WPT_StatField*)&iw_flow_stats.pecs_counter_1)->part.high,
           ((WPT_StatField*)&iw_flow_stats.pecs_counter_1)->part.low);

   printf( "TX Queue Drop Packets:   %08x%08x\n",
           ((WPT_StatField*)&iw_flow_stats.tx_queue_drop_packets)->part.high,
           ((WPT_StatField*)&iw_flow_stats.tx_queue_drop_packets)->part.low);

   printf( "Denied Packets:          %08x%08x\n",
           ((WPT_StatField*)&iw_flow_stats.denied_packets)->part.high,
           ((WPT_StatField*)&iw_flow_stats.denied_packets)->part.low);

   printf( "Group filtered packets:  %08x%08x\n",
           ((WPT_StatField*)&iw_flow_stats.group_filtered_packets)->part.high,
           ((WPT_StatField*)&iw_flow_stats.group_filtered_packets)->part.low);

   printf( "forwarded_bytes:         %08x%08x\n",
           ((WPT_StatField*)&iw_flow_stats.forwarded_bytes)->part.high,
           ((WPT_StatField*)&iw_flow_stats.forwarded_bytes)->part.low);

   printf( "Policer Non Conforming:  %08x%08x\n",
           ((WPT_StatField*)&iw_flow_stats.policer_non_conforming_packets)->part.high,
           ((WPT_StatField*)&iw_flow_stats.policer_non_conforming_packets)->part.low);

}


/*************************************************/
/*                                               */
/* PCE WDDI data test items creation             */
/*                                               */
/*************************************************/

static void WPE_PrintPceGlobalStats(void)
{
   WP_U32 temp1, temp2;
   WP_PceGlobalStatistics(WP_WINPATH(DEFAULT_WPID), &pce_global_stats);

   printf("PCE Global Statistics\n");

   temp1 = pce_global_stats.parser_header_access_errors;
   temp2 = pce_global_stats.parser_header_access_errors >> 32;
   printf("parser_header_access_errors %08X %08X\n", temp1, temp2);

   temp1 = pce_global_stats.parser_timeout_err;
   temp2 = pce_global_stats.parser_timeout_err >> 32;
   printf("parser_timeout_err          %08X %08X\n", temp1, temp2);

   temp1 = pce_global_stats.classifier_timeout_err;
   temp2 = pce_global_stats.classifier_timeout_err >> 32;
   printf("classifier_timeout_err      %08X %08X\n", temp1, temp2);

   temp1 = pce_global_stats.parser_accept;
   temp2 = pce_global_stats.parser_accept >> 32;
   printf("parser_accept               %08X %08X\n", temp1, temp2);

   printf("\n\n\n");
}

static void WPE_CheckLearningFilterModifyErrors(void)
{
   WP_pce_filter_learning lrn_filter_cfg = {0};
   WP_status status;

   lrn_filter_cfg.no_match_action = WP_PCE_FILTER_NO_MATCH_DENY;
   status = WP_PceFilterModify(PCE_filter[FILTER_SET_A_LEARNING], WP_PCE_FILTER_MODIFY_COMMAND_NO_MATCH, &lrn_filter_cfg);
   if (pce_init.enhanced_mode != WP_SW_FDB_ENABLE_MODE)
      check_error(status, "WP_PceFilterModify()" ,__LINE__, WP_OK);
   else 
   {
      check_error(status, "WP_PceFilterModify()" ,__LINE__, WP_ERR_SW_FDB_FILTER_ILLIGAL_NO_MATCH_ACTION);
   }
   printf("\nTest Learning WP_PceFilterModify, no match action == deny -- PASSED\n");

   lrn_filter_cfg.no_match_action = WP_PCE_FILTER_NO_MATCH_ACCEPT;
   status = WP_PceFilterModify(PCE_filter[FILTER_SET_A_LEARNING], WP_PCE_FILTER_MODIFY_COMMAND_NO_MATCH, &lrn_filter_cfg);
   if (pce_init.enhanced_mode != WP_SW_FDB_ENABLE_MODE)
      check_error(status, "WP_PceFilterModify()" ,__LINE__, WP_OK);
   else 
   {
      check_error(status, "WP_PceFilterModify()" ,__LINE__, WP_ERR_SW_FDB_FILTER_ILLIGAL_NO_MATCH_ACTION);
   }
   printf("\nTest Learning WP_PceFilterModify, no match action == accept -- PASSED\n");
}

static void WPE_CheckLearningFilterCreateErrors(void)
{
   WP_pce_filter_learning lrn_filter_cfg = {0};
   WP_pce_filter_forwarding fwd_filter_cfg = {0};
   WP_status status;

   fwd_filter_cfg.no_match_action = WP_PCE_FILTER_NO_MATCH_CONTINUE;
   fwd_filter_cfg.no_fields_action = WP_PCE_FILTER_NO_FIELDS_DENY;

   fwd_filter_cfg.no_match_result[0].result_type = WP_PCE_RESULT_LAST;

   fwd_filter_cfg.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   fwd_filter_cfg.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   fwd_filter_cfg.filter_fields[1].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   fwd_filter_cfg.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   fwd_filter_cfg.filter_fields[0].field_id   = WP_PCE_FIELD_ID_MAC_DA;
   fwd_filter_cfg.filter_fields[1].field_id = WP_PCE_FIELD_ID_LAST;
   PCE_filter[FILTER_SET_LAST] = WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_FORWARDING, &fwd_filter_cfg);
   terminate_on_error(PCE_filter[FILTER_SET_LAST], "WP_PceFilterCreate",__LINE__);


   lrn_filter_cfg.forwarding_filter = PCE_filter[FILTER_SET_LAST];
   lrn_filter_cfg.no_fields_action = WP_PCE_FILTER_NO_FIELDS_DENY;

   lrn_filter_cfg.no_match_result[0].result_type = WP_PCE_RESULT_LAST;

   lrn_filter_cfg.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   lrn_filter_cfg.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   lrn_filter_cfg.filter_fields[1].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   lrn_filter_cfg.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   lrn_filter_cfg.filter_fields[0].field_id = WP_PCE_FIELD_ID_MAC_SA;
   lrn_filter_cfg.filter_fields[1].field_id = WP_PCE_FIELD_ID_LAST;

   lrn_filter_cfg.no_match_action = WP_PCE_FILTER_NO_MATCH_DENY;
   status = WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_LEARNING, &lrn_filter_cfg);
   if (pce_init.enhanced_mode != WP_SW_FDB_ENABLE_MODE)
   {
      check_error(status, "WP_PceFilterCreate()" ,__LINE__, WP_OK);
      status = WP_PceFilterDelete(status);
      check_error(status, "WP_PceFilterDelete()" ,__LINE__, WP_OK);
   }
   else 
   {
      check_error(status, "WP_PceFilterCreate()" ,__LINE__, WP_ERR_SW_FDB_FILTER_ILLIGAL_NO_MATCH_ACTION);
   }
   printf("\nTest Learning WP_PceFilterCreate, no match action == deny -- PASSED\n");


   lrn_filter_cfg.forwarding_filter = PCE_filter[FILTER_SET_LAST];
   lrn_filter_cfg.no_fields_action = WP_PCE_FILTER_NO_FIELDS_DENY;

   lrn_filter_cfg.no_match_result[0].result_type = WP_PCE_RESULT_LAST;

   lrn_filter_cfg.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   lrn_filter_cfg.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   lrn_filter_cfg.filter_fields[1].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   lrn_filter_cfg.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   lrn_filter_cfg.filter_fields[0].field_id = WP_PCE_FIELD_ID_MAC_SA;
   lrn_filter_cfg.filter_fields[1].field_id = WP_PCE_FIELD_ID_LAST;

   lrn_filter_cfg.no_match_action = WP_PCE_FILTER_NO_MATCH_ACCEPT;
   status = WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_LEARNING, &lrn_filter_cfg);
   if (pce_init.enhanced_mode != WP_SW_FDB_ENABLE_MODE)
   {
      check_error(status, "WP_PceFilterCreate()" ,__LINE__, WP_OK);
      status = WP_PceFilterDelete(status);
      check_error(status, "WP_PceFilterDelete()" ,__LINE__, WP_OK);
   }
   else 
   {
      check_error(status, "WP_PceFilterCreate()" ,__LINE__, WP_ERR_SW_FDB_FILTER_ILLIGAL_NO_MATCH_ACTION);
   }
   printf("\nTest Learning WP_PceFilterCreate, no match action == accept -- PASSED\n");


   status = WP_PceFilterDelete(PCE_filter[FILTER_SET_LAST]);
   terminate_on_error(status, "WP_PceFilterDelete",__LINE__);



   lrn_filter_cfg.forwarding_filter = PCE_filter[FILTER_SET_A_FORWARDING];
   lrn_filter_cfg.no_fields_action = WP_PCE_FILTER_NO_FIELDS_DENY;

   lrn_filter_cfg.no_match_result[0].result_type = WP_PCE_RESULT_LAST;

   lrn_filter_cfg.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   lrn_filter_cfg.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   lrn_filter_cfg.filter_fields[1].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   lrn_filter_cfg.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   lrn_filter_cfg.filter_fields[0].field_id = WP_PCE_FIELD_ID_MAC_SA;
   lrn_filter_cfg.filter_fields[1].field_id = WP_PCE_FIELD_ID_LAST;

   lrn_filter_cfg.no_match_action = WP_PCE_FILTER_NO_MATCH_ACCEPT;
   status = WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_LEARNING, &lrn_filter_cfg);
   check_error(status, "WP_PceFilterCreate()" ,__LINE__, WP_ERR_PCE_FILTER_LEARNING_ALREADY_EXISTS);
   printf("\nTest Forwarding filter is connected to only one Learning filter -- PASSED\n");
}

static void WPE_CheckForwardingFilterModifyErrors(void)
{
   WP_pce_filter_forwarding fwd_filter_cfg = {0};
   WP_status status;

   fwd_filter_cfg.no_match_action = WP_PCE_FILTER_NO_MATCH_DENY;
   status = WP_PceFilterModify(PCE_filter[FILTER_SET_A_FORWARDING], WP_PCE_FILTER_MODIFY_COMMAND_NO_MATCH, &fwd_filter_cfg);
   if (TestType == TEST_SW_FDB_ONLY)
      check_error(status, "WP_PceFilterModify()" ,__LINE__, WP_ERR_SW_FDB_FILTER_ILLIGAL_NO_MATCH_ACTION);
   else 
   {
      check_error(status, "WP_PceFilterModify()" ,__LINE__, WP_OK);
   }   
   printf("\nTest Fowrarding WP_PceFilterModify, no match action == deny -- PASSED\n");

   fwd_filter_cfg.no_match_action = WP_PCE_FILTER_NO_MATCH_ACCEPT;
   status = WP_PceFilterModify(PCE_filter[FILTER_SET_A_FORWARDING], WP_PCE_FILTER_MODIFY_COMMAND_NO_MATCH, &fwd_filter_cfg);
   if (TestType == TEST_SW_FDB_ONLY)
      check_error(status, "WP_PceFilterModify()" ,__LINE__, WP_ERR_SW_FDB_FILTER_ILLIGAL_NO_MATCH_ACTION);
   else 
   {
      check_error(status, "WP_PceFilterModify()" ,__LINE__, WP_OK);
   }
   printf("\nTest Fowrarding WP_PceFilterModify, no match action == accept -- PASSED\n");
}

static void WPE_CheckForwardingFilterCreateErrors(void)
{
   WP_pce_filter_forwarding fwd_filter_cfg = {0};
   WP_status status;

   fwd_filter_cfg.no_fields_action = WP_PCE_FILTER_NO_FIELDS_DENY;

   fwd_filter_cfg.no_match_result[0].result_type = WP_PCE_RESULT_LAST;

   fwd_filter_cfg.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   fwd_filter_cfg.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   fwd_filter_cfg.filter_fields[1].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   fwd_filter_cfg.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   fwd_filter_cfg.filter_fields[0].field_id   = WP_PCE_FIELD_ID_MAC_DA;
   fwd_filter_cfg.filter_fields[1].field_id = WP_PCE_FIELD_ID_LAST;

   fwd_filter_cfg.no_match_action = WP_PCE_FILTER_NO_MATCH_DENY;
   status = WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_FORWARDING, &fwd_filter_cfg);
   if (TestType == TEST_SW_FDB_ONLY)
   {
      check_error(status, "WP_PceFilterCreate()" ,__LINE__, WP_ERR_SW_FDB_FILTER_ILLIGAL_NO_MATCH_ACTION);
   }
   else 
   {
      check_error(status, "WP_PceFilterCreate()" ,__LINE__, WP_OK);
      status = WP_PceFilterDelete(status);
      check_error(status, "WP_PceFilterDelete()" ,__LINE__, WP_OK);
   }
   
   printf("\nTest Fowrarding WP_PceFilterCreate, no match action == deny -- PASSED\n");



   fwd_filter_cfg.no_fields_action = WP_PCE_FILTER_NO_FIELDS_DENY;

   fwd_filter_cfg.no_match_result[0].result_type = WP_PCE_RESULT_LAST;

   fwd_filter_cfg.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   fwd_filter_cfg.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   fwd_filter_cfg.filter_fields[1].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   fwd_filter_cfg.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   fwd_filter_cfg.filter_fields[0].field_id   = WP_PCE_FIELD_ID_MAC_DA;
   fwd_filter_cfg.filter_fields[1].field_id = WP_PCE_FIELD_ID_LAST;

   fwd_filter_cfg.no_match_action = WP_PCE_FILTER_NO_MATCH_ACCEPT;
   status = WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_FORWARDING, &fwd_filter_cfg);
   if (TestType == TEST_SW_FDB_ONLY)
      check_error(status, "WP_PceFilterCreate()" ,__LINE__, WP_ERR_SW_FDB_FILTER_ILLIGAL_NO_MATCH_ACTION);
   else 
   {
      check_error(status, "WP_PceFilterCreate()" ,__LINE__, WP_OK);
      status = WP_PceFilterDelete(status);
      check_error(status, "WP_PceFilterDelete()" ,__LINE__, WP_OK);
   }
   printf("\nTest Fowrarding WP_PceFilterCreate, no match action == accept -- PASSED\n");
}

/*
 * Create group (A, C, D, E) : (classification, forwarding, learning) filter now
 * FILTER_SET_A_CLASSIFICATION, FILTER_SET_C_CLASSIFICATION
 *
 * group B is used in second round
 * 
 * FILTER_SET_A_CLASSIFICATION 	::= {	WP_PCE_FIELD_ID_INT_VLAN_TAG, \
					WP_PCE_FIELD_ID_INT_VLAN_TAG}
 * FILTER_SET_A_FORWARDING 	::= {	WP_PCE_FIELD_ID_MAC_DA}
 * FILTER_SET_A_LEARNING	::= {	WP_PCE_FIELD_ID_MAC_SA}
 * FILTER_SET_C_FORWARDING 	::= {	WP_PCE_FIELD_ID_INT_VLAN_TAG, \
					WP_PCE_FIELD_ID_INT_VLAN_TAG}
 * FILTER_SET_C_LEARNING	::= {	WP_PCE_FIELD_ID_MAC_SA, \
					WP_PCE_FIELD_ID_INT_VLAN_TAG}
 * FILTER_SET_D_FORWARDING	::= {	WP_PCE_FIELD_ID_MAC_DA, \
					WP_PCE_FIELD_ID_INT_VLAN_TAG, \
					WP_PCE_FIELD_ID_EXT_VLAN_TAG}
 * FILTER_SET_D_LEARNING	::= {	WP_PCE_FIELD_ID_MAC_SA, \
					WP_PCE_FIELD_ID_INT_VLAN_TAG, \
					WP_PCE_FIELD_ID_EXT_VLAN_TAG}}
 * FILTER_SET_E_FORWARDING	::= {	WP_PCE_FIELD_ID_IW_SYSTEM, \
					WP_PCE_FIELD_ID_MAC_DA, \
					WP_PCE_FIELD_ID_INT_VLAN_TAG}
 * FILTER_SET_E_LEARNING	::= {	WP_PCE_FIELD_ID_IW_SYSTEM, \
					WP_PCE_FIELD_ID_MAC_SA, \
					WP_PCE_FIELD_ID_INT_VLAN_TAG}
 */   
static void WPE_CreateDlPceFilters(void)
{
   WP_pce_filter_classification filter_class = {0};
   WP_pce_filter_forwarding fwd_filter_cfg = {0};
   WP_pce_filter_learning lrn_filter_cfg = {0};

   /* Common parameters for all filters*/
   filter_class.no_match_action = WP_PCE_FILTER_NO_MATCH_DENY;
   filter_class.no_fields_action = WP_PCE_FILTER_NO_FIELDS_DENY;

   filter_class.no_match_result[0].result_type = WP_PCE_RESULT_LAST;

   // filter 1
   /*----------------------------*\ 
    * A classification --- VLAN ID 1 
   \*----------------------------*/
   filter_class.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_RANGE_LOW;
   filter_class.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_USED;
   filter_class.filter_fields[0].mask.vlan_tag = WP_PCE_FIELD_MASK_VLAN_ID;
   filter_class.filter_fields[0].field_id = WP_PCE_FIELD_ID_INT_VLAN_TAG;

   /*----------------------------*\ 
    * A classification --- LAN ID 2
   \*----------------------------*/
   filter_class.filter_fields[1].field_mode = WP_PCE_FIELD_MODE_COMPARE_RANGE_HIGH;
   filter_class.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_USED;
   filter_class.filter_fields[1].mask.vlan_tag = WP_PCE_FIELD_MASK_VLAN_ID;
   filter_class.filter_fields[1].field_id = WP_PCE_FIELD_ID_INT_VLAN_TAG;

   /*----------------------------*\ 
    * A classification --- the END
   \*----------------------------*/
   filter_class.filter_fields[2].field_id = WP_PCE_FIELD_ID_LAST;
   PCE_filter[FILTER_SET_A_CLASSIFICATION] = WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_CLASSIFICATION, &filter_class);
   terminate_on_error(PCE_filter[FILTER_SET_A_CLASSIFICATION], "WP_PceFilterCreate",__LINE__);

   
   fwd_filter_cfg.no_match_action = WP_PCE_FILTER_NO_MATCH_CONTINUE;
   fwd_filter_cfg.no_fields_action = WP_PCE_FILTER_NO_FIELDS_DENY;

   // filter 2
   /*----------------------------*\ 
    * A forwarding --- if no match
   \*----------------------------*/
   fwd_filter_cfg.no_match_result[0].result_type = WP_PCE_RESULT_LAST;

   fwd_filter_cfg.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   fwd_filter_cfg.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   /*----------------------------*\ 
    * A forwarding --- dest MAC address
   \*----------------------------*/
   fwd_filter_cfg.filter_fields[1].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   fwd_filter_cfg.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

//////////////////////////////////////////////////////////

   fwd_filter_cfg.filter_fields[0].field_id   = WP_PCE_FIELD_ID_MAC_DA;
   fwd_filter_cfg.filter_fields[1].field_id = WP_PCE_FIELD_ID_LAST;
   PCE_filter[FILTER_SET_A_FORWARDING] = WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_FORWARDING, &fwd_filter_cfg);
   terminate_on_error(PCE_filter[FILTER_SET_A_FORWARDING], "WP_PceFilterCreate",__LINE__);

   lrn_filter_cfg.forwarding_filter = PCE_filter[FILTER_SET_A_FORWARDING];
   lrn_filter_cfg.no_match_action = WP_PCE_FILTER_NO_MATCH_CONTINUE;
   lrn_filter_cfg.no_fields_action = WP_PCE_FILTER_NO_FIELDS_DENY;

   // filter 3
   /*----------------------------*\ 
    * A forwarding --- source MAC address
   \*----------------------------*/
   lrn_filter_cfg.no_match_result[0].result_type = WP_PCE_RESULT_LAST;

   lrn_filter_cfg.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   lrn_filter_cfg.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   lrn_filter_cfg.filter_fields[1].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   lrn_filter_cfg.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

//////////////////////////////////////////////////////////
   lrn_filter_cfg.filter_fields[0].field_id = WP_PCE_FIELD_ID_MAC_SA;
   lrn_filter_cfg.filter_fields[1].field_id = WP_PCE_FIELD_ID_LAST;
   PCE_filter[FILTER_SET_A_LEARNING] = WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_LEARNING, &lrn_filter_cfg);
   terminate_on_error(PCE_filter[FILTER_SET_A_LEARNING], "WP_PceFilterCreate",__LINE__);


   fwd_filter_cfg.no_fields_action = WP_PCE_FILTER_NO_FIELDS_DENY;

   // filter 1
   /*----------------------------*\ 
    * C forwarding --- dest MAC address
   \*----------------------------*/
   fwd_filter_cfg.no_match_result[0].result_type = WP_PCE_RESULT_LAST;

   fwd_filter_cfg.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   fwd_filter_cfg.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   /*----------------------------*\ 
    * C forwarding --- VLAN TAG 1
   \*----------------------------*/
   fwd_filter_cfg.filter_fields[1].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   fwd_filter_cfg.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_USED;
   fwd_filter_cfg.filter_fields[1].mask.vlan_tag = WP_PCE_FIELD_MASK_VLAN_ID;

   fwd_filter_cfg.filter_fields[0].field_id   = WP_PCE_FIELD_ID_MAC_DA;
   fwd_filter_cfg.filter_fields[1].field_id = WP_PCE_FIELD_ID_INT_VLAN_TAG;
   fwd_filter_cfg.filter_fields[2].field_id = WP_PCE_FIELD_ID_LAST;

//////////////////////////////////////////////////////////
   fwd_filter_cfg.no_match_action = WP_PCE_FILTER_NO_MATCH_CONTINUE;
   PCE_filter[FILTER_SET_C_FORWARDING] = WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_FORWARDING, &fwd_filter_cfg);
   terminate_on_error(PCE_filter[FILTER_SET_C_FORWARDING], "WP_PceFilterCreate",__LINE__);

   
   // filter 2
   /*----------------------------*\ 
    * C learning --- source MAC address
   \*----------------------------*/
   lrn_filter_cfg.forwarding_filter = PCE_filter[FILTER_SET_C_FORWARDING];
   lrn_filter_cfg.no_fields_action = WP_PCE_FILTER_NO_FIELDS_DENY;

   lrn_filter_cfg.no_match_result[0].result_type = WP_PCE_RESULT_LAST;

   lrn_filter_cfg.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   lrn_filter_cfg.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   /*----------------------------*\ 
    * C learning --- VLAN TAG 1
   \*----------------------------*/
   lrn_filter_cfg.filter_fields[1].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   lrn_filter_cfg.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_USED;
   lrn_filter_cfg.filter_fields[1].mask.vlan_tag = WP_PCE_FIELD_MASK_VLAN_ID;

   lrn_filter_cfg.filter_fields[0].field_id   = WP_PCE_FIELD_ID_MAC_SA;
   lrn_filter_cfg.filter_fields[1].field_id = WP_PCE_FIELD_ID_INT_VLAN_TAG;
   lrn_filter_cfg.filter_fields[2].field_id = WP_PCE_FIELD_ID_LAST;
//////////////////////////////////////////////////////////

   lrn_filter_cfg.no_match_action = WP_PCE_FILTER_NO_MATCH_CONTINUE;
   PCE_filter[FILTER_SET_C_LEARNING] = WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_LEARNING, &lrn_filter_cfg);
   terminate_on_error(PCE_filter[FILTER_SET_C_LEARNING], "WP_PceFilterCreate",__LINE__);
   

   // filter 3
   /*----------------------------*\ 
    * D forwarding --- dest MAC address
   \*----------------------------*/
   fwd_filter_cfg.no_fields_action = WP_PCE_FILTER_NO_FIELDS_DENY;

   fwd_filter_cfg.no_match_result[0].result_type = WP_PCE_RESULT_LAST;

   fwd_filter_cfg.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   fwd_filter_cfg.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   /*----------------------------*\ 
    * D forwarding --- VLAN TAG 1
   \*----------------------------*/
   fwd_filter_cfg.filter_fields[1].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   fwd_filter_cfg.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_USED;
   fwd_filter_cfg.filter_fields[1].mask.vlan_tag = WP_PCE_FIELD_MASK_VLAN_ID;

   /*----------------------------*\ 
    * D forwarding --- VLAN TAG 2
   \*----------------------------*/
   fwd_filter_cfg.filter_fields[2].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   fwd_filter_cfg.filter_fields[2].mask_mode = WP_PCE_FIELD_MASK_USED;
   fwd_filter_cfg.filter_fields[2].mask.vlan_tag = WP_PCE_FIELD_MASK_VLAN_ID;

   fwd_filter_cfg.filter_fields[0].field_id   = WP_PCE_FIELD_ID_MAC_DA;
   fwd_filter_cfg.filter_fields[1].field_id = WP_PCE_FIELD_ID_INT_VLAN_TAG;
   fwd_filter_cfg.filter_fields[2].field_id = WP_PCE_FIELD_ID_EXT_VLAN_TAG;
   fwd_filter_cfg.filter_fields[3].field_id = WP_PCE_FIELD_ID_LAST;
//////////////////////////////////////////////////////////

   if (TestType == TEST_SW_FDB_ONLY)
      fwd_filter_cfg.no_match_action = WP_PCE_FILTER_NO_MATCH_CONTINUE;
   else
      fwd_filter_cfg.no_match_action = WP_PCE_FILTER_NO_MATCH_ACCEPT;
   // D forwarding --- 
   PCE_filter[FILTER_SET_D_FORWARDING] = WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_FORWARDING, &fwd_filter_cfg);
   terminate_on_error(PCE_filter[FILTER_SET_D_FORWARDING], "WP_PceFilterCreate",__LINE__);

   
   lrn_filter_cfg.forwarding_filter = PCE_filter[FILTER_SET_D_FORWARDING];
   lrn_filter_cfg.no_fields_action = WP_PCE_FILTER_NO_FIELDS_DENY;

   /*----------------------------*\ 
    * D learning --- source MAC address
   \*----------------------------*/
   lrn_filter_cfg.no_match_result[0].result_type = WP_PCE_RESULT_LAST;

   lrn_filter_cfg.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   lrn_filter_cfg.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   /*----------------------------*\ 
    * D learning --- VLAN TAG 1
   \*----------------------------*/
   lrn_filter_cfg.filter_fields[1].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   lrn_filter_cfg.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_USED;
   lrn_filter_cfg.filter_fields[1].mask.vlan_tag = WP_PCE_FIELD_MASK_VLAN_ID;

   /*----------------------------*\ 
    * D learning --- VLAN TAG 2
   \*----------------------------*/
   lrn_filter_cfg.filter_fields[2].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   lrn_filter_cfg.filter_fields[2].mask_mode = WP_PCE_FIELD_MASK_USED;
   lrn_filter_cfg.filter_fields[2].mask.vlan_tag = WP_PCE_FIELD_MASK_VLAN_ID;

   lrn_filter_cfg.filter_fields[0].field_id   = WP_PCE_FIELD_ID_MAC_SA;
   lrn_filter_cfg.filter_fields[1].field_id = WP_PCE_FIELD_ID_INT_VLAN_TAG;
   lrn_filter_cfg.filter_fields[2].field_id = WP_PCE_FIELD_ID_EXT_VLAN_TAG;
   lrn_filter_cfg.filter_fields[3].field_id = WP_PCE_FIELD_ID_LAST;
//////////////////////////////////////////////////////////

   lrn_filter_cfg.no_match_action = WP_PCE_FILTER_NO_MATCH_CONTINUE;
   PCE_filter[FILTER_SET_D_LEARNING] = WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_LEARNING, &lrn_filter_cfg);
   terminate_on_error(PCE_filter[FILTER_SET_D_LEARNING], "WP_PceFilterCreate",__LINE__);
 

   fwd_filter_cfg.no_fields_action = WP_PCE_FILTER_NO_FIELDS_DENY;

   /*----------------------------*\ 
    * E forwarding --- dest MAC address 
   \*----------------------------*/
   fwd_filter_cfg.no_match_result[0].result_type = WP_PCE_RESULT_LAST;

   fwd_filter_cfg.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   fwd_filter_cfg.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   /*----------------------------*\ 
    * E forwarding ---  VLAN ID 1
   \*----------------------------*/
   fwd_filter_cfg.filter_fields[1].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   fwd_filter_cfg.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_USED;
   fwd_filter_cfg.filter_fields[1].mask.vlan_tag = WP_PCE_FIELD_MASK_VLAN_ID;

   /*----------------------------*\ 
    * E forwarding ---  VLAN ID 2
   \*----------------------------*/
   fwd_filter_cfg.filter_fields[2].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   fwd_filter_cfg.filter_fields[2].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   fwd_filter_cfg.filter_fields[0].field_id   = WP_PCE_FIELD_ID_MAC_DA;
   fwd_filter_cfg.filter_fields[1].field_id = WP_PCE_FIELD_ID_INT_VLAN_TAG;
   fwd_filter_cfg.filter_fields[2].field_id = WP_PCE_FIELD_ID_IW_SYSTEM;
   fwd_filter_cfg.filter_fields[3].field_id = WP_PCE_FIELD_ID_LAST;
//////////////////////////////////////////////////////////

   if (TestType == TEST_SW_FDB_ONLY)
      fwd_filter_cfg.no_match_action = WP_PCE_FILTER_NO_MATCH_CONTINUE;
   else
      fwd_filter_cfg.no_match_action = WP_PCE_FILTER_NO_MATCH_DENY;
   PCE_filter[FILTER_SET_E_FORWARDING] = WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_FORWARDING, &fwd_filter_cfg);
   terminate_on_error(PCE_filter[FILTER_SET_E_FORWARDING], "WP_PceFilterCreate",__LINE__);

   
   lrn_filter_cfg.forwarding_filter = PCE_filter[FILTER_SET_E_FORWARDING];
   lrn_filter_cfg.no_fields_action = WP_PCE_FILTER_NO_FIELDS_DENY;

   /*----------------------------*\ 
    * E learning --- source MAC address 
   \*----------------------------*/
   lrn_filter_cfg.no_match_result[0].result_type = WP_PCE_RESULT_LAST;

   lrn_filter_cfg.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   lrn_filter_cfg.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   /*----------------------------*\ 
    * E learning --- VLAN TAG 1
   \*----------------------------*/
   lrn_filter_cfg.filter_fields[1].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   lrn_filter_cfg.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_USED;
   lrn_filter_cfg.filter_fields[1].mask.vlan_tag = WP_PCE_FIELD_MASK_VLAN_ID;

   /*----------------------------*\ 
    * E learning --- IW system
   \*----------------------------*/
   lrn_filter_cfg.filter_fields[2].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   lrn_filter_cfg.filter_fields[2].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   lrn_filter_cfg.filter_fields[0].field_id   = WP_PCE_FIELD_ID_MAC_SA;
   lrn_filter_cfg.filter_fields[1].field_id = WP_PCE_FIELD_ID_INT_VLAN_TAG;
   lrn_filter_cfg.filter_fields[2].field_id = WP_PCE_FIELD_ID_IW_SYSTEM;
   lrn_filter_cfg.filter_fields[3].field_id = WP_PCE_FIELD_ID_LAST;
//////////////////////////////////////////////////////////

   lrn_filter_cfg.no_match_action = WP_PCE_FILTER_NO_MATCH_CONTINUE;
   PCE_filter[FILTER_SET_E_LEARNING] = WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_LEARNING, &lrn_filter_cfg);
   terminate_on_error(PCE_filter[FILTER_SET_E_LEARNING], "WP_PceFilterCreate",__LINE__);
 

}

static void WPE_DeleteDlPceRules(void)
{
   WP_handle status;
   WP_U32 ii;
   WP_U32 rule_index;

   for(ii=0; ii < (FILTER_SET_COUNT * NUM_OF_INPUT_PORT_PER_FILTER_SET); ii++)
   {
      rule_index = RULES_A_CLASSIFICATION_START + ii;

      status = WP_PceRuleDelete(PCE_rule_handle[rule_index]);
      terminate_on_error(status, "WP_PceRuleDelete",__LINE__);
   }
    
   for(ii=0; ii<NUM_OF_RULES_PER_FILTER_SET; ii++)
   {
      rule_index = RULES_A_FORWARDING_START + ii;

      status = WP_PceRuleDelete(PCE_rule_handle[rule_index]);
      terminate_on_error(status, "WP_PceRuleDelete",__LINE__);
   }

   for(ii=0; ii<NUM_OF_RULES_PER_FILTER_SET; ii++)
   {
      rule_index = RULES_C_FORWARDING_START + ii;

      status = WP_PceRuleDelete(PCE_rule_handle[rule_index]);
      terminate_on_error(status, "WP_PceRuleDelete",__LINE__);
   }

   for(ii=0; ii<NUM_OF_RULES_PER_FILTER_SET; ii++)
   {
      rule_index = RULES_D_FORWARDING_START + ii;

      status = WP_PceRuleDelete(PCE_rule_handle[rule_index]);
      terminate_on_error(status, "WP_PceRuleDelete",__LINE__);
   }

   for(ii=0; ii<NUM_OF_RULES_PER_FILTER_SET; ii++)
   {
      rule_index = RULES_E_FORWARDING_START + ii;

      status = WP_PceRuleDelete(PCE_rule_handle[rule_index]);
      terminate_on_error(status, "WP_PceRuleDelete",__LINE__);
   }
}

/*
 * Create group (A, C, D, E) : (classification, forwarding, learning) filter now
 * FILTER_SET_A_CLASSIFICATION, FILTER_SET_C_CLASSIFICATION
 *
 * group B is used in second round
 * 
 * FILTER_SET_A_CLASSIFICATION 	::= {	WP_PCE_FIELD_ID_INT_VLAN_TAG, \
					WP_PCE_FIELD_ID_INT_VLAN_TAG}
 * FILTER_SET_A_FORWARDING 	::= {	WP_PCE_FIELD_ID_MAC_DA}
 * FILTER_SET_A_LEARNING	::= {	WP_PCE_FIELD_ID_MAC_SA}
 * FILTER_SET_C_FORWARDING 	::= {	WP_PCE_FIELD_ID_INT_VLAN_TAG, \
					WP_PCE_FIELD_ID_INT_VLAN_TAG}
 * FILTER_SET_C_LEARNING	::= {	WP_PCE_FIELD_ID_MAC_SA, \
					WP_PCE_FIELD_ID_INT_VLAN_TAG}
 * FILTER_SET_D_FORWARDING	::= {	WP_PCE_FIELD_ID_MAC_DA, \
					WP_PCE_FIELD_ID_INT_VLAN_TAG, \
					WP_PCE_FIELD_ID_EXT_VLAN_TAG}
 * FILTER_SET_D_LEARNING	::= {	WP_PCE_FIELD_ID_MAC_SA, \
					WP_PCE_FIELD_ID_INT_VLAN_TAG, \
					WP_PCE_FIELD_ID_EXT_VLAN_TAG}}
 * FILTER_SET_E_FORWARDING	::= {	WP_PCE_FIELD_ID_IW_SYSTEM, \
					WP_PCE_FIELD_ID_MAC_DA, \
					WP_PCE_FIELD_ID_INT_VLAN_TAG}
 * FILTER_SET_E_LEARNING	::= {	WP_PCE_FIELD_ID_IW_SYSTEM, \
					WP_PCE_FIELD_ID_MAC_SA, \
					WP_PCE_FIELD_ID_INT_VLAN_TAG}
 */   
static void WPE_CreateDlPceRules(void)
{
   WP_pce_rule_classification rule_cfg = {0};
   WP_pce_rule_forwarding rule_fwd = {0};
   WP_U32 flow_index, ii, i;
   WP_U32 rule_index;

   /* Common parameters */
   rule_cfg.enabled = WP_ENABLE;

   /* Filter Set A IW input Port change */
   for(flow_index=0; flow_index < (NUM_OF_INPUT_PORT_PER_FILTER_SET); flow_index++)
   {
      rule_index = RULES_A_CLASSIFICATION_START + flow_index;

      rule_cfg.filter_handle = PCE_filter[FILTER_SET_A_CLASSIFICATION];

      /*----------------------------*\
       * we use 2 VLAN tag here only 
      \*----------------------------*/
      rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_INT_VLAN_TAG;
      rule_cfg.rule_fields[0].value.vlan_tag = 0x100 + (0x08 * flow_index);

      rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_INT_VLAN_TAG;
      rule_cfg.rule_fields[1].value.vlan_tag = 0x107 + (0x08 * flow_index);

      rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_LAST;
//////////////////////////////////////////////////////////////////

      rule_cfg.match_action = WP_PCE_RULE_MATCH_CONTINUE;

      rule_cfg.match_result[0].result_type = WP_PCE_RESULT_IN_IW_PORT_UPDATE;
      rule_cfg.match_result[0].param.iw_port.iw_port_handle = h_iw_port_gbe[flow_index];

      rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;
      
      PCE_rule_handle[rule_index] = WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID),
                                                     WP_PCE_RULE_CLASSIFICATION,
                                                     &rule_cfg);
      terminate_on_error(PCE_rule_handle[rule_index], "WP_PceRuleCreate",__LINE__);
      free_pce_hw_rules--;
   }

   /* Filter Set B,C,D,E IW input Port change */
   for(flow_index=0; flow_index < ((FILTER_SET_COUNT - 1) * NUM_OF_INPUT_PORT_PER_FILTER_SET); flow_index++)
   {
      rule_index = RULES_A_CLASSIFICATION_START + NUM_OF_INPUT_PORT_PER_FILTER_SET + flow_index;

      rule_cfg.filter_handle = PCE_filter[FILTER_SET_A_CLASSIFICATION];

      /*----------------------------*\
       * we use 2 VLAN tag here only 
      \*----------------------------*/
      rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_INT_VLAN_TAG;
      rule_cfg.rule_fields[0].value.vlan_tag = 0x110 + (0x10 * (flow_index/NUM_OF_INPUT_PORT_PER_FILTER_SET)) + (flow_index%2);

      rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_INT_VLAN_TAG;
      rule_cfg.rule_fields[1].value.vlan_tag = 0x110 + (0x10 * (flow_index/NUM_OF_INPUT_PORT_PER_FILTER_SET)) + (flow_index%2);

      rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_LAST;
//////////////////////////////////////////////////////////////////

      rule_cfg.match_action = WP_PCE_RULE_MATCH_CONTINUE;

      /* is of no use following */
      rule_cfg.match_result[0].result_type = WP_PCE_RESULT_IN_IW_PORT_UPDATE;
      rule_cfg.match_result[0].param.iw_port.iw_port_handle = h_iw_port_gbe[NUM_OF_INPUT_PORT_PER_FILTER_SET + flow_index];

      rule_cfg.match_result[1].result_type = WP_PCE_RESULT_FILTER_SET_CHANGE;
      rule_cfg.match_result[1].param.change_fs.filter_set_handle = dl_filter_set[FILTER_SET_B + (flow_index/NUM_OF_INPUT_PORT_PER_FILTER_SET)];
      
      rule_cfg.match_result[2].result_type = WP_PCE_RESULT_LAST;
      
      PCE_rule_handle[rule_index] = WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID),
                                                     WP_PCE_RULE_CLASSIFICATION,
                                                     &rule_cfg);
      terminate_on_error(PCE_rule_handle[rule_index], "WP_PceRuleCreate",__LINE__);
      free_pce_hw_rules--;
   }

   /* Fill the PCE with HW rules until left free only NUM_OF_RULES_PER_FILTER_SET/2 */
   if (TestType == TEST_HYBRID_FDB)
      WPE_CreateDummyPceHwRules(NUM_OF_RULES_PER_FILTER_SET/2);

   /* Create forwarding rules for filter set A */
   for(i=0; i<NUM_OF_RULES_PER_FILTER_SET; i++)
   {
      rule_fwd.enabled = WP_ENABLE;
      rule_fwd.filter_handle = PCE_filter[FILTER_SET_A_FORWARDING];
      rule_index = RULES_A_FORWARDING_START + i;
      
      /*----------------------------*\
       * we use only dest MAC address
      \*----------------------------*/
      rule_fwd.rule_fields[0].field_id = WP_PCE_FIELD_ID_MAC_DA;
      for(ii=0; ii<5; ii++)
         rule_fwd.rule_fields[0].value.mac_addr[ii] = start_mac_addr[ii]; 
      rule_fwd.rule_fields[0].value.mac_addr[5] = i; 

      rule_fwd.rule_fields[1].field_id = WP_PCE_FIELD_ID_LAST;
//////////////////////////////////////////////////////////////////
      
      rule_fwd.match_result[0].result_type = WP_PCE_RESULT_OUT_IW_PORT_UPDATE;
      rule_fwd.match_result[0].param.iw_port.iw_port_handle = h_iw_output_port_b; 
      
      rule_fwd.match_result[1].result_type = WP_PCE_RESULT_AGING;
      rule_fwd.match_result[1].param.aging.mode = WP_PCE_AGING_DISABLE;
      
      rule_fwd.match_result[2].result_type = WP_PCE_RESULT_PCE_INTERFACE;
      rule_fwd.match_result[2].param.pce_if_iw_next_round.pce_interface = pce_dl_second_round_if_handle;

      rule_fwd.match_result[3].result_type = WP_PCE_RESULT_LAST;
      
      rule_fwd.match_action = WP_PCE_RULE_MATCH_CONTINUE;
      
      PCE_rule_handle[rule_index] = WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID),
                                                     WP_PCE_RULE_FORWARDING,
                                                     &rule_fwd);
      terminate_on_error(PCE_rule_handle[rule_index], "WP_PceRuleCreate",__LINE__);
   }
   
   if (TestType == TEST_HYBRID_FDB)
   {
      for(i=0; i<NUM_OF_RULES_PER_FILTER_SET/2; i++)
         free_pce_hw_rules--;

      WPE_DeleteDummyPceHwRules();

      /* Fill the PCE with HW rules until left free only NUM_OF_RULES_PER_FILTER_SET/2 */
      WPE_CreateDummyPceHwRules(NUM_OF_RULES_PER_FILTER_SET/2);
   }
   
   /* Create forwarding rules for filter set C */
   for(i=0; i<NUM_OF_RULES_PER_FILTER_SET; i++)
   {
      rule_fwd.enabled = WP_ENABLE;
      rule_fwd.filter_handle = PCE_filter[FILTER_SET_C_FORWARDING];
      rule_index = RULES_C_FORWARDING_START + i;
      
      /*----------------------------*\
       * dest MAC address
      \*----------------------------*/
      rule_fwd.rule_fields[0].field_id = WP_PCE_FIELD_ID_MAC_DA;
      for(ii=0; ii<5; ii++)
         rule_fwd.rule_fields[0].value.mac_addr[ii] = start_mac_addr[ii]; 
      rule_fwd.rule_fields[0].value.mac_addr[5] = i; 
      
      rule_fwd.rule_fields[1].field_id = WP_PCE_FIELD_ID_INT_VLAN_TAG;
      rule_fwd.rule_fields[1].value.vlan_tag = 0x120 + (i%2);

      rule_fwd.rule_fields[2].field_id = WP_PCE_FIELD_ID_LAST;
//////////////////////////////////////////////////////////////////
     /* ------------- */ 
      rule_fwd.match_result[0].result_type = WP_PCE_RESULT_OUT_IW_PORT_UPDATE;
      rule_fwd.match_result[0].param.iw_port.iw_port_handle = h_iw_output_port; 
      
      rule_fwd.match_result[1].result_type = WP_PCE_RESULT_AGING;
      rule_fwd.match_result[1].param.aging.mode = WP_PCE_AGING_DISABLE;
      
      rule_fwd.match_result[2].result_type = WP_PCE_RESULT_FLOW_AGG;
      rule_fwd.match_result[2].param.flow_agg.flow_aggregation = flow_agg[FILTER_SET_C_FORWARDING_FLOW_AGG];
      
      rule_fwd.match_result[3].result_type = WP_PCE_RESULT_LAST;
      
      rule_fwd.match_action = WP_PCE_RULE_MATCH_CONTINUE;
      
      PCE_rule_handle[rule_index] = WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID),
                                                     WP_PCE_RULE_FORWARDING,
                                                     &rule_fwd);
      terminate_on_error(PCE_rule_handle[rule_index], "WP_PceRuleCreate",__LINE__);
   }
   
   if (TestType == TEST_HYBRID_FDB)
   {
      for(i=0; i<NUM_OF_RULES_PER_FILTER_SET/2; i++)
         free_pce_hw_rules--;

      WPE_DeleteDummyPceHwRules();

      /* Fill the PCE with HW rules until left free only NUM_OF_RULES_PER_FILTER_SET/2 */
      WPE_CreateDummyPceHwRules(NUM_OF_RULES_PER_FILTER_SET/2);
   } 

   /* Create forwarding rules for filter set D */
   for(i=0; i<NUM_OF_RULES_PER_FILTER_SET; i++)
   {
      rule_fwd.enabled = WP_ENABLE;
      rule_fwd.filter_handle = PCE_filter[FILTER_SET_D_FORWARDING];
      rule_index = RULES_D_FORWARDING_START + i;
      
      /*----------------------------*\
       * dest MAC address
      \*----------------------------*/
      rule_fwd.rule_fields[0].field_id = WP_PCE_FIELD_ID_MAC_DA;
      for(ii=0; ii<5; ii++)
         rule_fwd.rule_fields[0].value.mac_addr[ii] = start_mac_addr[ii]; 
      rule_fwd.rule_fields[0].value.mac_addr[5] = i; 
      
      /*----------------------------*\
       * and 2 VLAN tag
      \*----------------------------*/
      rule_fwd.rule_fields[1].field_id = WP_PCE_FIELD_ID_INT_VLAN_TAG;
      rule_fwd.rule_fields[1].value.vlan_tag = 0x130 + (i%2);
   
      rule_fwd.rule_fields[2].field_id = WP_PCE_FIELD_ID_EXT_VLAN_TAG;
      rule_fwd.rule_fields[2].value.vlan_tag = 0x130 + (i%2);

      rule_fwd.rule_fields[3].field_id = WP_PCE_FIELD_ID_LAST;
//////////////////////////////////////////////////////////////////
      
      rule_fwd.match_result[0].result_type = WP_PCE_RESULT_OUT_IW_PORT_UPDATE;
      rule_fwd.match_result[0].param.iw_port.iw_port_handle = h_iw_output_port; 
      
      rule_fwd.match_result[1].result_type = WP_PCE_RESULT_AGING;
      rule_fwd.match_result[1].param.aging.mode = WP_PCE_AGING_DISABLE;
      
      rule_fwd.match_result[2].result_type = WP_PCE_RESULT_FLOW_AGG;
      rule_fwd.match_result[2].param.flow_agg.flow_aggregation = flow_agg[FILTER_SET_D_FORWARDING_FLOW_AGG];
      
      rule_fwd.match_result[3].result_type = WP_PCE_RESULT_LAST;
      
      rule_fwd.match_action = WP_PCE_RULE_MATCH_CONTINUE;
      
      PCE_rule_handle[rule_index] = WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID),
                                                     WP_PCE_RULE_FORWARDING,
                                                     &rule_fwd);
      terminate_on_error(PCE_rule_handle[rule_index], "WP_PceRuleCreate",__LINE__);
   }
  
   if (TestType == TEST_HYBRID_FDB)
   {
      for(i=0; i<NUM_OF_RULES_PER_FILTER_SET/2; i++)
         free_pce_hw_rules--;

      WPE_DeleteDummyPceHwRules();

      /* Fill the PCE with HW rules until left free only NUM_OF_RULES_PER_FILTER_SET/2 */
      WPE_CreateDummyPceHwRules(NUM_OF_RULES_PER_FILTER_SET/2);
   } 

   /* Create forwarding rules for filter set E */
   for(i=0; i<NUM_OF_RULES_PER_FILTER_SET; i++)
   {
      rule_fwd.enabled = WP_ENABLE;
      rule_fwd.filter_handle = PCE_filter[FILTER_SET_E_FORWARDING];
      rule_index = RULES_E_FORWARDING_START + i;
      
      /*----------------------------*\
       * dest MAC address
      \*----------------------------*/
      rule_fwd.rule_fields[0].field_id = WP_PCE_FIELD_ID_MAC_DA;
      for(ii=0; ii<5; ii++)
         rule_fwd.rule_fields[0].value.mac_addr[ii] = start_mac_addr[ii]; 
      rule_fwd.rule_fields[0].value.mac_addr[5] = i; 
      
      /*----------------------------*\
       * VLAN
      \*----------------------------*/
      rule_fwd.rule_fields[1].field_id = WP_PCE_FIELD_ID_INT_VLAN_TAG;
      rule_fwd.rule_fields[1].value.vlan_tag = 0x140 + (i%2);
   
      /*----------------------------*\
       * IW port
      \*----------------------------*/
      rule_fwd.rule_fields[2].field_id = WP_PCE_FIELD_ID_IW_SYSTEM;
      rule_fwd.rule_fields[2].value.iw_system_handle = dl_iwsys_bridge[i%2];

      rule_fwd.rule_fields[3].field_id = WP_PCE_FIELD_ID_LAST;
//////////////////////////////////////////////////////////////////
     
     /* --------------- */ 
      rule_fwd.match_result[0].result_type = WP_PCE_RESULT_OUT_IW_PORT_UPDATE;
      rule_fwd.match_result[0].param.iw_port.iw_port_handle = h_iw_output_port; 
      
      rule_fwd.match_result[1].result_type = WP_PCE_RESULT_AGING;
      rule_fwd.match_result[1].param.aging.mode = WP_PCE_AGING_DISABLE;
      
      rule_fwd.match_result[2].result_type = WP_PCE_RESULT_FLOW_AGG;
      rule_fwd.match_result[2].param.flow_agg.flow_aggregation = flow_agg[FILTER_SET_E_FORWARDING_FLOW_AGG];
      
      rule_fwd.match_result[3].result_type = WP_PCE_RESULT_LAST;
      
      rule_fwd.match_action = WP_PCE_RULE_MATCH_CONTINUE;
      
      PCE_rule_handle[rule_index] = WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID),
                                                     WP_PCE_RULE_FORWARDING,
                                                     &rule_fwd);
      terminate_on_error(PCE_rule_handle[rule_index], "WP_PceRuleCreate",__LINE__);
   }

   if (TestType == TEST_HYBRID_FDB)
   {
      for(i=0; i<NUM_OF_RULES_PER_FILTER_SET/2; i++)
         free_pce_hw_rules--;

      WPE_DeleteDummyPceHwRules();
   }
}

static void WT_CheckFilterSetModify(void)
{
   WP_pce_filter_set fs_level0;
   WP_status status;


   fs_level0.filters[0] = PCE_filter[FILTER_SET_A_FORWARDING];
   fs_level0.filters[1] = PCE_filter[FILTER_SET_A_LEARNING];
   fs_level0.filters[2] = WP_UNUSED;

   status = WP_PceFilterSetModify(dl_filter_set[FILTER_SET_A], WP_PCE_FILTER_SET_MODIFY_COMMAND_FILTERS, &fs_level0);
   if (pce_init.enhanced_mode != WP_SW_FDB_ENABLE_MODE)
   {
      check_error(status, "WP_PceFilterSetModify()" ,__LINE__, WP_OK);
   }
   else 
   {
      check_error(status, "WP_PceFilterSetModify()" ,__LINE__, WP_ERR_SW_FDB_FORWARDING_NOT_LAST);
   }
   printf("\nTest WP_PceFilterSetModify Forwarding filter is no last -- PASSED\n");

   fs_level0.filters[0] = PCE_filter[FILTER_SET_A_LEARNING];
   fs_level0.filters[1] = PCE_filter[FILTER_SET_A_CLASSIFICATION];
   fs_level0.filters[2] = PCE_filter[FILTER_SET_A_FORWARDING];
   fs_level0.filters[3] = WP_UNUSED;

   status = WP_PceFilterSetModify(dl_filter_set[FILTER_SET_A], WP_PCE_FILTER_SET_MODIFY_COMMAND_FILTERS, &fs_level0);
   if (pce_init.enhanced_mode != WP_SW_FDB_ENABLE_MODE)
   {
      check_error(status, "WP_PceFilterSetModify()" ,__LINE__, WP_OK);
   }
   else 
   {
      check_error(status, "WP_PceFilterSetModify()" ,__LINE__, WP_ERR_SW_FDB_CLASSIFICATION_AFTER_LEARNING);
   }
   printf("\nTest WP_PceFilterSetModify Classification after learning filter -- PASSED\n");

   fs_level0.filters[0] = PCE_filter[FILTER_SET_B_LEARNING];
   fs_level0.filters[1] = PCE_filter[FILTER_SET_A_FORWARDING];
   fs_level0.filters[2] = WP_UNUSED;

   status = WP_PceFilterSetModify(dl_filter_set[FILTER_SET_A], WP_PCE_FILTER_SET_MODIFY_COMMAND_FILTERS, &fs_level0);
   if (pce_init.enhanced_mode != WP_SW_FDB_ENABLE_MODE)
   {
      check_error(status, "WP_PceFilterSetModify()" ,__LINE__, WP_OK);
   }
   else 
   {
      check_error(status, "WP_PceFilterSetModify()" ,__LINE__, WP_ERR_SW_FDB_LEARNING_AND_FORWARDING_MISMATCH);
   }
   printf("\nTest WP_PceFilterSetModify Forwarding and learning mismatch -- PASSED\n");

}

static void WPE_CreateDlPceFilterSets(void)
{
   WP_pce_filter_set fs_level0;
   
   fs_level0.filter_set_level = 0;
   fs_level0.next_filter_set = WP_UNUSED;
   fs_level0.filters[0] = PCE_filter[FILTER_SET_A_CLASSIFICATION];
   fs_level0.filters[1] = PCE_filter[FILTER_SET_A_LEARNING];
   fs_level0.filters[2] = PCE_filter[FILTER_SET_A_FORWARDING];
   fs_level0.filters[3] = WP_UNUSED;

   dl_filter_set[FILTER_SET_A] = WP_PceFilterSetCreate(WP_WINPATH(DEFAULT_WPID), &fs_level0);
   terminate_on_error(dl_filter_set[FILTER_SET_A], "WP_PceFilterSetCreate",__LINE__);


   fs_level0.filter_set_level = 1;
   fs_level0.next_filter_set = WP_UNUSED;
   fs_level0.filters[0] = PCE_filter[FILTER_SET_C_LEARNING];
   fs_level0.filters[1] = PCE_filter[FILTER_SET_C_FORWARDING];
   fs_level0.filters[2] = WP_UNUSED;

   dl_filter_set[FILTER_SET_C] = WP_PceFilterSetCreate(WP_WINPATH(DEFAULT_WPID), &fs_level0);
   terminate_on_error(dl_filter_set[FILTER_SET_C], "WP_PceFilterSetCreate",__LINE__);


   fs_level0.filter_set_level = 1;
   fs_level0.next_filter_set = WP_UNUSED;
   fs_level0.filters[0] = PCE_filter[FILTER_SET_D_LEARNING];
   fs_level0.filters[1] = PCE_filter[FILTER_SET_D_FORWARDING];
   fs_level0.filters[2] = WP_UNUSED;

   dl_filter_set[FILTER_SET_D] = WP_PceFilterSetCreate(WP_WINPATH(DEFAULT_WPID), &fs_level0);
   terminate_on_error(dl_filter_set[FILTER_SET_D], "WP_PceFilterSetCreate",__LINE__);

   fs_level0.filter_set_level = 1;
   fs_level0.next_filter_set = WP_UNUSED;
   fs_level0.filters[0] = PCE_filter[FILTER_SET_E_LEARNING];
   fs_level0.filters[1] = PCE_filter[FILTER_SET_E_FORWARDING];
   fs_level0.filters[2] = WP_UNUSED;

   dl_filter_set[FILTER_SET_E] = WP_PceFilterSetCreate(WP_WINPATH(DEFAULT_WPID), &fs_level0);
   terminate_on_error(dl_filter_set[FILTER_SET_E], "WP_PceFilterSetCreate",__LINE__);
}

static void WPE_CreateDlPceInterface(WP_handle iw_sys)
{
   WP_pce_if_params_pkt_rx_channel pce_if_params = {0};
   WP_handle pce_if_handle;
   WP_status status;

   WPE_CreateDlPceFilters();	// create PCE_filter[A,B,C,D,E_forwareding/leaning] here
   WPE_CreateDlPceFilterSets();	// create dl_filter_set[a,c,d,e] here

   pce_if_params.mode = WP_PCE_IW_PORT_CONNECTION_ENABLED;
   pce_if_params.parser_start_type = WP_PCE_PARSER_START_TYPE_ETHERNET;
   pce_if_params.filter_set_handle = dl_filter_set[FILTER_SET_A];
   pce_if_params.ip_header_validation = WP_DISABLE;

   WPI_TEST_CHECK_HANDLE(pce_if_handle,
                         WP_PceInterfaceCreate(WP_PCE_IF_TYPE_PKT_RX_CHANNEL, &pce_if_params));

   rx_binding_cfg[0].pce_if_handle = pce_if_handle;
   WPI_TEST_CHECK_RESULT(status,
                         WP_OK,
                         WP_IwRxBindingModify(gbe1_rx_ch_handle,
                                              iw_sys,
                                              qniw,
                                              WP_IW_RX_BIND_MOD_PCE_INTERFACE,
                                              &rx_binding_cfg[0]));
   terminate_on_error(status,"WP_IwRxBindingModify",__LINE__);
}

static void WPE_DeleteDlPceInterface(void)
{
   WP_status status;
   WP_U32 ii;
   
   for (ii = 0; ii<FILTER_SET_COUNT; ii++)
   {
      if (ii != FILTER_SET_B)
      {
         status = WP_PceFilterSetDelete(dl_filter_set[ii]);
         terminate_on_error(status ,"WP_PceFilterSetDelete()",__LINE__);  
      }
   }

   status = WP_PceFilterDelete(PCE_filter[FILTER_SET_C_LEARNING]);
   terminate_on_error(status ,"WP_PceFilterDelete()",__LINE__);  

   status = WP_PceFilterDelete(PCE_filter[FILTER_SET_C_FORWARDING]);
   terminate_on_error(status ,"WP_PceFilterDelete()",__LINE__);  

   status = WP_PceFilterDelete(PCE_filter[FILTER_SET_D_LEARNING]);
   terminate_on_error(status ,"WP_PceFilterDelete()",__LINE__);  

   status = WP_PceFilterDelete(PCE_filter[FILTER_SET_D_FORWARDING]);
   terminate_on_error(status ,"WP_PceFilterDelete()",__LINE__);  

   status = WP_PceFilterDelete(PCE_filter[FILTER_SET_E_LEARNING]);
   terminate_on_error(status ,"WP_PceFilterDelete()",__LINE__);  

   status = WP_PceFilterDelete(PCE_filter[FILTER_SET_E_FORWARDING]);
   terminate_on_error(status ,"WP_PceFilterDelete()",__LINE__);  

   status = WP_PceFilterDelete(PCE_filter[FILTER_SET_A_LEARNING]);
   terminate_on_error(status ,"WP_PceFilterDelete()",__LINE__);  

   status = WP_PceFilterDelete(PCE_filter[FILTER_SET_A_FORWARDING]);
   terminate_on_error(status ,"WP_PceFilterDelete()",__LINE__);  

   status = WP_PceFilterDelete(PCE_filter[FILTER_SET_A_CLASSIFICATION]);
   terminate_on_error(status ,"WP_PceFilterDelete()",__LINE__);  
}



/*------------------------------------------------------*\
FILTER_SET_B_CLASSIFICATION 	::= { WP_PCE_FIELD_ID_OUTPUT_IW_PORT }
FILTER_SET_B_FORWARDING		::= { WP_PCE_FIELD_ID_MAC_DA }
FILTER_SET_B_LEARNING		::= { WP_PCE_FIELD_ID_MAC_SA }
\*------------------------------------------------------*/

static void WPE_CreateDlSecondRoundPceFilters(void)
{
   WP_pce_filter_classification filter_class = {0};
   WP_pce_filter_forwarding fwd_filter_cfg = {0};
   WP_pce_filter_learning lrn_filter_cfg = {0};

   /* Common parameters for all filters*/
   filter_class.no_match_action = WP_PCE_FILTER_NO_MATCH_CONTINUE;
   filter_class.no_fields_action = WP_PCE_FILTER_NO_FIELDS_DENY;

   /*----------------------------*
    * B classification ---  IW PORT 
   \*----------------------------*/
   filter_class.no_match_result[0].result_type = WP_PCE_RESULT_LAST;

   filter_class.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_class.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   filter_class.filter_fields[1].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_class.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   filter_class.filter_fields[2].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_class.filter_fields[2].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   /* B classification --- PCE_filter1 */
   filter_class.filter_fields[0].field_id = WP_PCE_FIELD_ID_OUTPUT_IW_PORT;
   filter_class.filter_fields[1].field_id = WP_PCE_FIELD_ID_LAST;
//////////////////////////////////////////////////////////////////
   PCE_filter[FILTER_SET_B_CLASSIFICATION] = WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_CLASSIFICATION, &filter_class);
   terminate_on_error(PCE_filter[FILTER_SET_B_CLASSIFICATION], "WP_PceFilterCreate",__LINE__);


   /* PCE_filter2 */
   fwd_filter_cfg.no_fields_action = WP_PCE_FILTER_NO_FIELDS_DENY;

   /*-------------------------------------------*\
    * B forwarding --- VLAN ID 1
   \*-------------------------------------------*/
   fwd_filter_cfg.no_match_result[0].result_type = WP_PCE_RESULT_LAST;

   fwd_filter_cfg.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   fwd_filter_cfg.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   /*-------------------------------------------*\
    * B forwarding --- VLAN ID 1
   \*-------------------------------------------*/
   fwd_filter_cfg.filter_fields[1].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   fwd_filter_cfg.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_USED;
   fwd_filter_cfg.filter_fields[1].mask.vlan_tag = WP_PCE_FIELD_MASK_VLAN_ID;

   /*-------------------------------------------*\
    * B forwarding --- VLAN ID 2
   \*-------------------------------------------*/
   fwd_filter_cfg.filter_fields[2].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   fwd_filter_cfg.filter_fields[2].mask_mode = WP_PCE_FIELD_MASK_USED;
   fwd_filter_cfg.filter_fields[2].mask.vlan_tag = WP_PCE_FIELD_MASK_VLAN_ID;

   fwd_filter_cfg.filter_fields[3].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   fwd_filter_cfg.filter_fields[3].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   /*-------------------------------------------*\
    * B forwarding --- dest MAC address
   \*-------------------------------------------*/
   fwd_filter_cfg.filter_fields[0].field_id   = WP_PCE_FIELD_ID_MAC_DA;
   fwd_filter_cfg.filter_fields[1].field_id = WP_PCE_FIELD_ID_LAST;
//////////////////////////////////////////////////////////////////

   fwd_filter_cfg.no_match_action = WP_PCE_FILTER_NO_MATCH_CONTINUE;
   PCE_filter[FILTER_SET_B_FORWARDING] = WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_FORWARDING, &fwd_filter_cfg);
   terminate_on_error(PCE_filter[FILTER_SET_B_FORWARDING], "WP_PceFilterCreate",__LINE__);


   /* PCE_filter3 */
   lrn_filter_cfg.forwarding_filter = PCE_filter[FILTER_SET_B_FORWARDING];
   lrn_filter_cfg.no_fields_action = WP_PCE_FILTER_NO_FIELDS_DENY;

   /*-------------------------------------------*\
    * B learning --- source MAC address
   \*-------------------------------------------*/
   lrn_filter_cfg.no_match_result[0].result_type = WP_PCE_RESULT_LAST;

   lrn_filter_cfg.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   lrn_filter_cfg.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   /*-------------------------------------------*\
    * B learning --- VLAN ID 1, no use becase ID_LAST at [1]
   \*-------------------------------------------*/
   lrn_filter_cfg.filter_fields[1].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   lrn_filter_cfg.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_USED;
   lrn_filter_cfg.filter_fields[1].mask.vlan_tag = WP_PCE_FIELD_MASK_VLAN_ID;

   /*-------------------------------------------*\
    * B learning --- VLAN ID 2, no use becase ID_LAST at [1]
   \*-------------------------------------------*/
   lrn_filter_cfg.filter_fields[2].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   lrn_filter_cfg.filter_fields[2].mask_mode = WP_PCE_FIELD_MASK_USED;
   lrn_filter_cfg.filter_fields[2].mask.vlan_tag = WP_PCE_FIELD_MASK_VLAN_ID;

   lrn_filter_cfg.filter_fields[3].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   lrn_filter_cfg.filter_fields[3].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   lrn_filter_cfg.filter_fields[0].field_id   = WP_PCE_FIELD_ID_MAC_SA;
   lrn_filter_cfg.filter_fields[1].field_id = WP_PCE_FIELD_ID_LAST;
//////////////////////////////////////////////////////////////////

   lrn_filter_cfg.no_match_action = WP_PCE_FILTER_NO_MATCH_CONTINUE;
   PCE_filter[FILTER_SET_B_LEARNING] = WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_LEARNING, &lrn_filter_cfg);
   terminate_on_error(PCE_filter[FILTER_SET_B_LEARNING], "WP_PceFilterCreate",__LINE__);

}

static void WPE_DeleteDlSecondRoundPceRules(void)
{
   WP_handle status;
   WP_U32 rule_index, ii;

   rule_index = RULES_B_CLASSIFICATION_START;
   status = WP_PceRuleDelete(PCE_rule_handle[rule_index]);
   terminate_on_error(status, "WP_PceRuleDelete",__LINE__);

   for(ii=0; ii<NUM_OF_RULES_PER_FILTER_SET; ii++)
   {
      rule_index = RULES_B_FORWARDING_START + ii;

      status = WP_PceRuleDelete(PCE_rule_handle[rule_index]);
      terminate_on_error(status, "WP_PceRuleDelete",__LINE__);
   }
}

/*------------------------------------------------------*\
FILTER_SET_B_CLASSIFICATION 	::= { WP_PCE_FIELD_ID_OUTPUT_IW_PORT }
FILTER_SET_B_FORWARDING		::= { WP_PCE_FIELD_ID_MAC_DA }
FILTER_SET_B_LEARNING		::= { WP_PCE_FIELD_ID_MAC_SA }
\*------------------------------------------------------*/
static void WPE_CreateDlSecondRoundPceRules(void)
{
   WP_pce_rule_classification rule_cfg = {0};
   WP_pce_rule_forwarding rule_fwd = {0};
   WP_U32 rule_index, ii, i;

   /* Common parameters */
   rule_cfg.enabled = WP_ENABLE;

   rule_index = RULES_B_CLASSIFICATION_START;
   rule_cfg.filter_handle = PCE_filter[FILTER_SET_B_CLASSIFICATION];
   
   /*--------------------------*\
    * B forwarding --- IW port
   \*--------------------------*/
   rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_OUTPUT_IW_PORT;
   rule_cfg.rule_fields[0].value.iw_port_handle = h_iw_output_port_b;
   
   rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_LAST;
//////////////////////////////////////////////////////////////////
   
   /*
    * i think there is no use about the following lines
    */
   rule_cfg.match_action = WP_PCE_RULE_MATCH_ACCEPT;
   
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
   rule_cfg.match_result[0].param.flow_agg.flow_aggregation = flow_agg[FILTER_SET_B_CLASSIFICATION_FLOW_AGG];
   
   rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;
   
   PCE_rule_handle[rule_index] = WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID),
                                                  WP_PCE_RULE_CLASSIFICATION,
                                                  &rule_cfg);
   terminate_on_error(PCE_rule_handle[rule_index], "WP_PceRuleCreate",__LINE__);

   free_pce_hw_rules--;

   /* Fill the PCE with HW rules until left free only NUM_OF_RULES_PER_FILTER_SET/2 */
   if (TestType == TEST_HYBRID_FDB)
      WPE_CreateDummyPceHwRules(NUM_OF_RULES_PER_FILTER_SET/2);
   
   /* Create forwarding rules for filter set B */
   for(i=0; i<NUM_OF_RULES_PER_FILTER_SET; i++)
   {
      rule_fwd.enabled = WP_ENABLE;
      rule_fwd.filter_handle = PCE_filter[FILTER_SET_B_FORWARDING];
      rule_index = RULES_B_FORWARDING_START + i;
      
   /*--------------------------*\
    * B forwarding --- dest MAC address
   \*--------------------------*/
      rule_fwd.rule_fields[0].field_id = WP_PCE_FIELD_ID_MAC_DA;
      for(ii=0; ii<5; ii++)
         rule_fwd.rule_fields[0].value.mac_addr[ii] = start_mac_addr[ii]; 
      rule_fwd.rule_fields[0].value.mac_addr[5] = i; 
      
      rule_fwd.rule_fields[1].field_id = WP_PCE_FIELD_ID_LAST;
//////////////////////////////////////////////////////////////////
      
      // the underlying lines of codes is of no use
      rule_fwd.match_result[0].result_type = WP_PCE_RESULT_OUT_IW_PORT_UPDATE;
      rule_fwd.match_result[0].param.iw_port.iw_port_handle = h_iw_output_port; 
      
      rule_fwd.match_result[1].result_type = WP_PCE_RESULT_AGING;
      rule_fwd.match_result[1].param.aging.mode = WP_PCE_AGING_DISABLE;
      
      rule_fwd.match_result[2].result_type = WP_PCE_RESULT_FLOW_AGG;
      rule_fwd.match_result[2].param.flow_agg.flow_aggregation = flow_agg[FILTER_SET_B_FORWARDING_FLOW_AGG];
      
      rule_fwd.match_result[3].result_type = WP_PCE_RESULT_LAST;
      
      rule_fwd.match_action = WP_PCE_RULE_MATCH_CONTINUE;
      
      PCE_rule_handle[rule_index] = WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID),
                                                     WP_PCE_RULE_FORWARDING,
                                                     &rule_fwd);
      terminate_on_error(PCE_rule_handle[rule_index], "WP_PceRuleCreate",__LINE__);
   }

   if (TestType == TEST_HYBRID_FDB)
   {
      for(i=0; i<NUM_OF_RULES_PER_FILTER_SET/2; i++)
         free_pce_hw_rules--;

      WPE_DeleteDummyPceHwRules();
   }
}

static void WPE_CreateDlSecondRoundPceFilterSets(void)
{
   WP_pce_filter_set fs_level0;

   fs_level0.filter_set_level = 1;
   fs_level0.next_filter_set = WP_UNUSED;
   fs_level0.filters[0] = PCE_filter[FILTER_SET_B_CLASSIFICATION];
   fs_level0.filters[1] = PCE_filter[FILTER_SET_B_LEARNING];
   fs_level0.filters[2] = PCE_filter[FILTER_SET_B_FORWARDING];
   fs_level0.filters[3] = WP_UNUSED;

   dl_filter_set[FILTER_SET_B] = WP_PceFilterSetCreate(WP_WINPATH(DEFAULT_WPID), &fs_level0);
   terminate_on_error(dl_filter_set[FILTER_SET_B], "WP_PceFilterSetCreate",__LINE__);
}


/*------------------------------------------------*\
\*------------------------------------------------*/
static void WPE_CreateDlSecondRoundPceInterface()
{
   WP_pce_if_params_iw_next_round pce_if_params = {0};

   WPE_CreateDlSecondRoundPceFilters();
   WPE_CreateDlSecondRoundPceFilterSets();

   /* WP_PCE_IW_PORT_CONNECTION_ENABLED is the only choice */
   pce_if_params.mode = WP_PCE_IW_PORT_CONNECTION_ENABLED;
   pce_if_params.parsing_ref_point = WP_PCE_PARSER_REF_POINT_FRAME_START;	// where to start the field check, start from where
   pce_if_params.parser_start_type = WP_PCE_PARSER_START_TYPE_ETHERNET;	// start program counter for specific protocol type
   pce_if_params.parsing_ref_offset = 0;	// start from parsing_ref_point
   pce_if_params.filter_set_handle = dl_filter_set[FILTER_SET_B];
   pce_if_params.ip_header_validation = WP_DISABLE;
   pce_if_params.characteristics = WP_PCE_IF_OUTPUT_PORT_PROPAGATION_ENABLE;

   if (TestType == TEST_HYBRID_FDB)
   {
      /* iw port handle should be preserved from previous PCE module */
      pce_if_params.characteristics |= WP_PCE_IF_IW_PORT_PROPAGATION_ENABLE;
      pce_if_params.iw_port = WP_UNUSED;
   }
   else
   {
      pce_if_params.iw_port = h_iw_port_next_round;
   }
   
   pce_dl_second_round_if_handle = WP_PceInterfaceCreate(WP_PCE_IF_TYPE_IW_NEXT_ROUND, 
                                                         &pce_if_params);
   terminate_on_error(pce_dl_second_round_if_handle,"WP_PceInterfaceCreate()",__LINE__);
}

static void WPE_DeleteDlSecondRoundPceInterface()
{
   WP_status status;

   status = WP_PceFilterSetDelete(dl_filter_set[FILTER_SET_B]);
   terminate_on_error(status ,"WP_PceFilterSetDelete()",__LINE__);  

   status = WP_PceFilterDelete(PCE_filter[FILTER_SET_B_LEARNING]);
   terminate_on_error(status ,"WP_PceFilterDelete()",__LINE__);  

   status = WP_PceFilterDelete(PCE_filter[FILTER_SET_B_FORWARDING]);
   terminate_on_error(status ,"WP_PceFilterDelete()",__LINE__);  

   status = WP_PceFilterDelete(PCE_filter[FILTER_SET_B_CLASSIFICATION]);
   terminate_on_error(status ,"WP_PceFilterDelete()",__LINE__);  
}

static void WPE_CheckStatistics()
{
   WP_U32 i=0;
   WP_status status;
   
   if(WPT_HostChannelQueueSizeX(rx_host_handle))
      terminate("Rx Host Channel wrong Size",__LINE__);

   for (i=0; i<FLOW_AGG_COUNT; i++)
   {
      if (WPT_CompatibleFaGetFwPacketsCount(flow_agg[i]) != NUM_OF_FLOWS)
         terminate("FA wrong number of packets", __LINE__);
   }
   
   /* Expected PCE filter results */
   previous_filter_stat[FILTER_SET_A_FORWARDING].matched_pkts +=  NUM_OF_FLOWS;
   previous_filter_stat[FILTER_SET_A_LEARNING].no_matched_pkts +=  NUM_OF_FLOWS;
   previous_filter_stat[FILTER_SET_B_CLASSIFICATION].matched_pkts +=  NUM_OF_FLOWS;
   previous_filter_stat[FILTER_SET_B_CLASSIFICATION].no_matched_pkts +=  NUM_OF_FLOWS;
   previous_filter_stat[FILTER_SET_B_LEARNING].no_matched_pkts +=  NUM_OF_FLOWS;
   previous_filter_stat[FILTER_SET_B_FORWARDING].matched_pkts +=  NUM_OF_FLOWS;
   previous_filter_stat[FILTER_SET_C_LEARNING].no_matched_pkts +=  NUM_OF_FLOWS;
   previous_filter_stat[FILTER_SET_C_FORWARDING].matched_pkts +=  NUM_OF_FLOWS;
   previous_filter_stat[FILTER_SET_D_LEARNING].no_matched_pkts +=  NUM_OF_FLOWS;
   previous_filter_stat[FILTER_SET_D_FORWARDING].matched_pkts +=  NUM_OF_FLOWS;
   previous_filter_stat[FILTER_SET_E_LEARNING].no_matched_pkts +=  NUM_OF_FLOWS;
   previous_filter_stat[FILTER_SET_E_FORWARDING].matched_pkts +=  NUM_OF_FLOWS;

   for (i=0; i<NUM_OF_PCE_FILTERS; i++)
   {
      if (i == FILTER_SET_A_CLASSIFICATION)
         continue;
      status = WP_PceFilterStatistics (PCE_filter[i], &filter_stat[i]);
      terminate_on_error(status, "WP_PceFilterStatistics()",__LINE__);

      if (filter_stat[i].matched_pkts != previous_filter_stat[i].matched_pkts ||
          filter_stat[i].no_matched_pkts != previous_filter_stat[i].no_matched_pkts ||
          filter_stat[i].denied_pkts != previous_filter_stat[i].denied_pkts)
      {
         terminate("PCE Filter statistics are not matched",__LINE__);
      }
   }   
}

void WPE_CreateDummyPceHwRules(WP_U32 rules_needed_count)
{
   WP_U32 rule_index;
   WP_pce_rule_classification rule_cfg = {0};

   dummy_rules_count = free_pce_hw_rules - rules_needed_count;
   
   for(rule_index = 0; rule_index<dummy_rules_count; rule_index++)
   {
      rule_cfg.filter_handle = PCE_filter[FILTER_SET_A_CLASSIFICATION];

      rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_INT_VLAN_TAG;
      rule_cfg.rule_fields[0].value.vlan_tag = 0xF00 + rule_index;

      rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_INT_VLAN_TAG;
      rule_cfg.rule_fields[1].value.vlan_tag = 0xF00 + rule_index;

      rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_LAST;
//////////////////////////////////////////////////////////////////

      rule_cfg.match_action = WP_PCE_RULE_MATCH_CONTINUE;

      rule_cfg.match_result[0].result_type = WP_PCE_RESULT_IN_IW_PORT_UPDATE;
      rule_cfg.match_result[0].param.iw_port.iw_port_handle = h_iw_port_gbe[0];

      rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;
      
      PCE_dummy_rule_handle[rule_index] = WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID),
                                                           WP_PCE_RULE_CLASSIFICATION,
                                                           &rule_cfg);
      terminate_on_error(PCE_dummy_rule_handle[rule_index], "WP_PceRuleCreate",__LINE__);
   }
}

void WPE_DeleteDummyPceHwRules(void)
{
   WP_U32 i;
   WP_status status;
   
   for(i = 0; i<dummy_rules_count; i++)
   {
      status = WP_PceRuleDelete(PCE_dummy_rule_handle[i]);
      terminate_on_error(status, "WP_PceRuleDelete",__LINE__);
   }

   dummy_rules_count = 0;
}

static void WPE_HostSendPacket(WP_U32 filter_set_index, WP_U32 packet_index)
{
   WP_data_segment km_tx_buf[3];
   WP_U8 km_data_ptr[3];
   WP_U8 tmp_buffer[2*MAX_DATA_LENGTH];
   WP_data_unit data_unit;

   WP_data_segment *curr_buff_ptr;
   WP_status status;


   payload_size[packet_index] = 26;
   
#if DEBUG
   printf("\nHost Send to: packaet index %d, payload_size=%d", packet_index, payload_size[packet_index]);
#endif
   
   
   PacketDataUnitSetup(&data_unit,
                       &km_tx_buf[1],
                       &km_data_ptr[1],
                       payload_size[packet_index],
                       filter_set_index,
                       packet_index);
   
   WP_Delay(1000);
   
   WP_MEM_BYTES_GET(tmp_buffer, data_unit.segment->data, data_unit.segment->data_size);
   
   status = WP_HostSend(gbe1_tx_ch_handle, &data_unit);
   terminate_on_error(status, "WP_HostSend()",__LINE__);
   WP_Delay(1000);
   curr_buff_ptr = data_unit.segment;
   
   data_unit.n_active = 0;
   
   /********************** prepare the data for checking **********************/
   
   PrepareExpectedData(expected_data[packet_index],
                       payload_size[packet_index],
                       filter_set_index,
                       packet_index);
}

static void WPE_CheckReceivedTraffic(WP_U32 packet_index)
{
   WP_U32 index=0;
   WP_data_unit rx_data_unit;
   WP_data_segment *curr_buff_ptr;
   WP_U8 tmp_buffer[2*MAX_DATA_LENGTH];
   WP_U32 ii,i;
   WP_status status;
   WP_U32 number_of_segments = 4;
   WP_data_segment data_segment[number_of_segments];

   scenario_failed = WP_FALSE;


   for (i=0; i < WT_RX_LOOP_LIMIT; i++)
   {
      /* receive packet from host queue */
      for(ii = 0; ii<number_of_segments; ii++)
      {
         data_segment[ii].displacement = 0;
         data_segment[ii].data_size = 0;
         data_segment[ii].pool_handle = 0;
         data_segment[ii].data = 0;
         data_segment[ii].displacement = 0;
         if(ii == number_of_segments - 1)
            data_segment[ii].next = NULL;
         else
            data_segment[ii].next = &(data_segment[ii+1]);
      }

      rx_data_unit.segment = (WP_data_segment *)&data_segment;

      rx_data_unit.n_segments = number_of_segments;
      rx_data_unit.type = WP_DATA_IW;
      rx_data_unit.n_active = 0;
      rx_data_unit.control = 0;

      status = WP_HostReceive(rx_host_handle, &rx_data_unit);
      if (status != WP_OK)
      {
         if (WP_ERROR(status) == WP_ERR_HST_NO_DATA_TO_GET)
         {
            WP_Delay(100);
         }
         else
            printf("Error in WP_HostReceive host_channel");
      }
      else
      {
         WP_U32 offset = rx_data_unit.segment[0].data_size;
         for(ii = 1; ii < rx_data_unit.n_active; ii++)
         {
            memcpy(rx_data_unit.segment[0].data + offset,
                   rx_data_unit.segment[ii].data,
                   rx_data_unit.segment[ii].data_size);
            rx_data_unit.segment[0].data_size += rx_data_unit.segment[ii].data_size;
            offset += rx_data_unit.segment[ii].data_size;
         }

         curr_buff_ptr = &rx_data_unit.segment[0];
         WP_MEM_BYTES_GET(tmp_buffer, curr_buff_ptr->data, curr_buff_ptr->data_size);

#if DEBUG
         printf("\ndata size from segment= %d, expected data size = %d \n",
                curr_buff_ptr->data_size,
                l2_l3_size[packet_index] +
                payload_size[packet_index]);

         printf("Host Receive Data: size=%d\n",
                curr_buff_ptr->data_size);
#endif

         if (curr_buff_ptr->data_size !=
             l2_l3_size[packet_index] + payload_size[packet_index])
         {
            printf("\nERROR: packet index = %d \n",
                   packet_index);
            printf("ERROR: data size from segment= %d, expected data size = %d \n\n",
                   curr_buff_ptr->data_size,
                   l2_l3_size[packet_index] +
                   payload_size[packet_index]);
            scenario_failed = WP_TRUE;
            WP_PoolFree(rx_data_unit.segment[0].pool_handle, rx_data_unit.segment[0].data);
            break;
         }

#if 0
         printf("\n\nReceive: Frame-Number = %d\n", packet_index);

         for (index=0;index<curr_buff_ptr->data_size; index++)
         {
            printf(" received data [ %d ] : 0x%02x\n", index, tmp_buffer[index]);
         }
#endif

#if 0
         for (index=0;index<curr_buff_ptr->data_size; index++)
         {
            printf(" excpected [ %d ] : 0x%02x\n", index, expected_data[packet_index][index]);
         }
#endif
         
#if 1
         for (index=0;index<curr_buff_ptr->data_size; index++)
         {
            if (expected_data[packet_index][index] != tmp_buffer[index])
            {
               {
                  WP_U32 iii;

                  printf("\n\n");
                  for (iii=0;iii<50; iii++)
                  {
                     printf("%02x", tmp_buffer[iii]);
                  }
                  printf("\n\n");
               }

               printf("Data differs in index %d: received data[%d]->%x, expected data[%d][%d]->%x\n",
                      index,
                      packet_index,
                      tmp_buffer[index],
                      packet_index,
                      index,
                      expected_data[packet_index][index]);
               scenario_failed = WP_TRUE;
               break;
            }
         }
#endif

         WP_PoolFree(rx_data_unit.segment[0].pool_handle, rx_data_unit.segment[0].data);
         break;
      }
   }

   if (scenario_failed == WP_TRUE)
      terminate("Rx Host Channel data error",__LINE__);
}

WP_U16 WTI_IpChecksumCalc(WP_U8 *buff, WP_U16 len)
{
   WP_U16 bu16;
   WP_U32  sum=0;
   WP_U32 i;

   /* make 16 bit words out of every two adjacent
      8 bit words in the packet and add them up */
   len = len/2;
   for (i=0;i<len;i++){
      bu16 = (buff[2*i] << 8) + buff[2*i+1];
      sum = sum + (WP_U32) bu16;
   }

   /* take only 16 bits out of the 32 bit sum
      and add up the carries */
   while (sum>>16)
      sum = (sum & 0xFFFF)+(sum >> 16);

   /* one's complement the result */
   sum = ~sum;

   return ((WP_U16) sum);
}


void WTI_TranslateHexToAscii(WP_CHAR *Ascii, WP_CHAR *Hex, WP_U32 length)
{
   WP_S32 i;
   WP_CHAR temp_num;

   for (i=0;i<length;i++)
   {
      temp_num = (Hex[i] & 0xf0) >> 4;
      if (temp_num < 10)
         Ascii[2*i] = temp_num + '0';
      else
         Ascii[2*i] =  temp_num + 'a' - 10;

      temp_num = Hex[i] & 0x0f;
      if (temp_num < 10)
         Ascii[2*i+1] = temp_num + '0';
      else
         Ascii[2*i+1] =  temp_num + 'a' - 10;

   }

   Ascii[2*length] = NULL;

   return;
}

#include "wt_util.c"
#include "wt_partition_ecc_util.c"
