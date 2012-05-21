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

#include "wpi_gpe_pecs_wddi_shared.h"
#include "brouter_pecs.h"
#include "mpls_pecs.h"


#include "wpx_enet_phy.h"

#include "wpu_debug_sw.h"
#include "api/wp_pce.h"

extern WP_U32 dps_ProgramImage[];
extern WP_U16 dps_PC_Table_Init[];

WP_CHAR appname[] = "Test for SW FDB";

#define DEBUG 0
#define DEFAULT_WPID	(0)

WP_U16 WT_SCH_TIMESTAMP=700;

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
   FILTER_SET_UL,
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
      "FILTER_SET_UL",
};

enum rules
{
   RULES_A_CLASSIFICATION_START = 1,
   RULES_UL_CLASSIFICATION_START = 50,
   RULES_B_CLASSIFICATION_START  = 100,
   RULES_A_FORWARDING_START = 200,
   RULES_B_FORWARDING_START = 60000,
   RULES_C_FORWARDING_START = 120000,
   RULES_D_FORWARDING_START = 180000,
   RULES_E_FORWARDING_START = 240000,
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

WP_U8  start_mac_addr[6] = {0x00, 0x00, 0x01, 0x00, 0x00, 0x00};

/* ***** gpe ******/

#define NUM_OF_PECS      2
#define NUM_OF_PECS_GLOBAL_INFO      10

extern WP_pecs_image_info brouter_pecs_pecs_image;
extern WP_pecs_image_info mpls_pecs_pecs_image;
WP_handle pecs_handles[NUM_OF_PECS];

WP_module_gpe_init gpe_init =
{
   NUM_OF_PECS, /* max_num_of_pecs */
   NUM_OF_PECS_GLOBAL_INFO, /* max_num_of_pecs_global_info */
};

// pecs cfg array
WP_gpe_pecs gpe_pecs_cfg[NUM_OF_PECS] = 
{
   {
      &brouter_pecs_pecs_image, /*pecs_image_info*/
      &WPE_GpeBrouterPecsService /*pecs_service_fp*/
   },
   {
      &mpls_pecs_pecs_image, /*pecs_image_info*/
      &WPE_GpeMplsPecsService /*pecs_service_fp*/
   }

};

WPE_gpe_brouter_pecs_flow_info brouter_pecs_flow_info[] = 
{
   /*  */
   {
      WPE_GPE_BROUTER_PECS_EXTRACT_DISABLE, /* header_extract_mode */
      0, /* extraction_size */
      WPE_GPE_BROUTER_PECS_ADD_DISABLE, /* prefix_add_mode */
      4, /* prefix_add_size */
      14, /* prefix_ip_offset */
      WPE_GPE_BROUTER_PECS_REPLACE_MAC_DISABLE, /* mac_replace_mode */
      {0x11, 0x22, 0x33, 0x44, 0x55, 0x00}, /* mac_da */
      {0x66, 0x77, 0x88, 0x99, 0xaa, 0x00}, /* mac_sa */
      0, /* vlan_edit_options*/
      
      
      {0}, /* prefix_remark_options*/
      WPE_GPE_BROUTER_PECS_TTL_DISABLE, /* ttl_mode */
      WPE_GPE_BROUTER_PECS_TOS_REMARKING_DISABLE, /* tos_remarking_mode */
      {/* prefix */
         0x11, 0x22, 0x33, 0x44, 0x55, 0x00,
         0x11, 0x12, 0x13, 0x14, 0x17, 0x00,
         0x81, 0, 0, 1, 
         0x88,0x47,
         0x11,0x11, 0x10, 0x80,
         0x22,0x22, 0x21, 0x80,
         0,0,0,0,0,0,0,0, 
         0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0, 
         0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0
     }  
   }
};

WP_iw_agg_generic dl_tx_agg_gbe[1] =
{
   {
      /*tag*/ 2,
      /*txfunc*/ 0,
      /*iw_port*/0,
      /*rfcs*/WP_IW_RFCS_ENABLE,
      /*interruptqueue;*/WP_IW_IRQT1,
      /*error_pkt_mode*/WP_IW_ERRPKT_DISCARD,
      /*intmode;*/WP_IW_INT_ENABLE,
      /*statmode;*/WP_IW_STAT_ENABLE,
      /*timestamp_mode;*/WP_IW_TIME_STAMP_DISABLE,
      /*mtu;*/9216,
      /*flow_agg_type;*/WP_IW_FLOW_AGG_PRIMARY,
      /*policer_handle;*/0,
      /*pecs_handle;*/0,
      /*pecs_flow_info;*/0,
      /*pecs_global_info_handle;*/0,
   },
};
   
WP_iw_agg_generic ul_tx_agg_gbe[1] =
{
   {
      /*tag*/ 2,
      /*txfunc*/ 0,
      /*iw_port*/0,
      /*rfcs*/WP_IW_RFCS_ENABLE,
      /*interruptqueue;*/WP_IW_IRQT1,
      /*error_pkt_mode*/WP_IW_ERRPKT_DISCARD,
      /*intmode;*/WP_IW_INT_ENABLE,
      /*statmode;*/WP_IW_STAT_ENABLE,
      /*timestamp_mode;*/WP_IW_TIME_STAMP_DISABLE,
      /*mtu;*/9216,
      /*flow_agg_type;*/WP_IW_FLOW_AGG_PRIMARY,
      /*policer_handle;*/0,
      /*pecs_handle;*/0,
      /*pecs_flow_info;*/0,
      /*pecs_global_info_handle;*/0,
   },
};

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

#define NUM_OF_INPUT_PORT_PER_FILTER_SET 2
WP_U32 num_of_rules_per_filter_set = 50;

enum 
{
   TEST_SW_FDB_ONLY = 0,
   TEST_HYBRID_FDB,
   TEST_HW_FDB_ONLY,
};

enum 
{
   WT_FORWARDING_ONLY = 0,
   WT_LEARNING_ONLY,
   WT_LEARNING_AND_FORWARDING,
};

#if defined(WP_BOARD_WDS3)
#define BUFFER_BUS WP_BUS_PACKET
#define BUFFER_BANK APP_BANK_PACKET
#elif defined(WP_BOARD_WDS3_SL)
#define BUFFER_BUS WP_BUS_HOST
#define BUFFER_BANK APP_BANK_HOST
#endif

#define NUM_OF_REQUIRED_LEARNING_RULES 16000

WP_U32 TestType = TEST_HW_FDB_ONLY;
WP_U32 start_system = 0;
WP_U32 learning_interrupt_counter = 0;
WP_U32 required_learning_rules = 0;
WP_handle aging_iterator_handle = 0;
WP_U32 learnt_rules = 0;
WP_U32 rules_enabled = 0;
WP_U32 rules_created = 0;
WP_U32 filter_set_mode = WT_LEARNING_ONLY; /* 0 - Forwarding only, 1 - learning only, 2 - forwarding and learning */
WP_S32 free_pce_hw_rules;
WP_S32 init_sys_free_pce_hw_rules;
WP_S32 hybrid_evenly = 1; /* 0 - split evenly (50% HW and 50% SW), 1 - create HW until maximum and then SW */
WP_S32 dummy_rules_count = 0;
WP_handle PCE_dummy_rule_handle[32000];

#define WT_MAX_FRAME_SIZE         (1536 + 2*512)

#define MAX_DATA_LENGTH    2500
#define APP_HOST_BUFFER_SIZE 1984

WP_pce_filter_stat filter_stat[100];

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
   0, /* fdb_threshold */
   (256*1024 - 1), /* max_fdb_rules */
};

WP_module_pce_init pce_init =
{
   &pce_parser_info, /* parser_info */
#if defined(WP_BOARD_WDS3)
   32000, /* hardware_rules_num */
#elif defined(WP_BOARD_WDS3_SL)
   8000, /* hardware_rules_num */
#endif
   15, /* filters_num */
   10, /* pkt_rx_channel_interfaces_num */
   5, /* iw_tx_channel_interfaces_num */
   WP_PCE_DISCARD_MODE_SMART_DISCARD, /* pce_discard_mode */
   WP_ENABLE, /* bridge_loop_prevention_mode */
   WP_ENABLE, /* learning_mode */
   {/* learning_queue_config */
      1000, /* learning_queue_size */
      WP_LEARNING_INTENABLE, /* interrupt_mode */
      WP_IW_IRQT0, /* int_queue_num */
      1, /* interrupt_rate */
      4, /* protection_table_size */
   },
   2, /* iw_next_round_pce_interfaces_num*/
   0, /* interfaces_on_atm_rx_mode */
   0,/* atm_rx_channel_interfaces_num */
   0,/* device_interfaces_num */
   WP_NO_ENHANCMENT,/* enhanced_mode */
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
       {/* STATISTICS      */ WP_BUS_INTERNAL  , APP_BANK_INTERNAL},
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
       {/* STATISTICS2     */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
       {/* STATISTICS3     */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
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
       {/* WP_VB_PECS_GLOBAL*/ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
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
       {/* WP_BUS_PACKET   */ WP_BUS_PACKET, APP_BANK_PACKET},
       {/* WP_BUS_INTERNAL */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
       {/* WP_BUS_HOST     */ WP_BUS_HOST, APP_BANK_HOST},
       {/* WP_BUS_IRQ      */ WP_BUS_HOST, APP_BANK_HOST},
       {/* WP_BUS_TMSTMP   */ WP_BUS_HOST, APP_BANK_HOST},
       {/* UNASSIGNED      */ WP_BUS_NONE, 0},
       {/* UNASSIGNED      */ WP_BUS_NONE, 0},
       {/* ASU_TPT         */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
       {/* ASU CW          */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
       {/* ASU TX QUEUE    */ WP_BUS_PARAM, APP_BANK_PARAM},
       {/* STATISTICS      */ WP_BUS_INTERNAL  , APP_BANK_INTERNAL},
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
       {/* STATISTICS2     */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
       {/* STATISTICS3     */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
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
       {/* WP_VB_PECS_GLOBAL*/ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
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
      /* num_buffers */ 2000,
      /* adjunct_pool */ 0
   }
};

WP_pool_iw_bd bd_conf[1] =
{
   {
      /*num_buffer_descriptors*/ 3000,
      /*buffer_pool*/0,
      /*bd_bus*/WP_BUS_INTERNAL,
      /*wmm_num_buffer_descriptors*/ 0
   }
};

WP_fmu_shaping_wrr weight_params[1] =
{
   {
      /*weight*/ 5000,
      /*flags*/0,
   }
   
};


WP_qnode_iw_group group_highp [1] =
{
   {

      /*num_of_bd_pools*/2,
      /*bd_pool*/   {0,0},
      /*fifo_selection_method*/WP_QNODE_IW_SELECTION_WRR,
      /*bd_pool_weight*/ /*{NULL,NULL}*/{&weight_params[0],&weight_params[0]}
   }
};

WP_qnode_iw_group group_lowp [1]=
{
   {
      
      /*num_of_bd_pools*/2,
      /*bd_pool*/   {0,0},
      /*fifo_selection_method*/WP_QNODE_IW_SELECTION_WRR,
      /*bd_pool_weight*//*{NULL,NULL}*/{&weight_params[0],&weight_params[0]}
   }
};

WP_qnode_iw_hier iw_hierq[1] =
{
   {
      /*num_of_groups*/2,
      /*iw_group*/ {{0},{0}},
      /*group_selection_method*/ WP_QNODE_IW_SELECTION_STRICT,
      /*group_weight*/{NULL,NULL}
      
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
      /*unk_mac_sa_filter*/        WP_IW_UNK_MACSA_FILTER_DISABLE,
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

WP_U32 WT_TimeRead(void);
WP_U32 WT_TimeDelta(WP_U32 later,WP_U32 earlier);
static void F_MyAppIndicateEventIwgpLearning(WP_tag tag, WP_U32 data, WP_U32 info);

void WT_Start(WP_CHAR *app_name, WP_CHAR *trial_name, WP_CHAR *file_name);
void WT_Reboot(void);
void terminate_on_error(WP_handle, WP_CHAR *,WP_U32);
void terminate(WP_CHAR *s, WP_U32 line);
static void WPE_LayerTwoSystemCreate(void);
static void WPE_SystemEnable(void);
static void WPE_ChannelsDelete(void);
static void WPE_ChannelsCreate(void);
static void WPE_PortsCreate(WP_handle iw_sys, WP_handle *iwport);
void WPT_CompatibleFlowStatistics( WP_handle agg);
static void WPE_RxBindingDelete(WP_handle ch, WP_handle iw_sys);
static void WPE_DlRxBindingCreate(WP_handle iw_sys);
void WPT_DisplayBridgingPortStats( WP_handle port_handle, WP_CHAR* type);
void WPT_DisplayRoutingPortStats( WP_handle port_handle, WP_CHAR* type);

void CLI_DeleteGenericFA(void);
void WPU_WinutilCall(WP_CHAR * input);
void CLI_CreateTest(void);
void CLI_DeleteTest(void);
void CLI_RunScenarios(void);
void WPT_GenericFlowStatistics( WP_handle agg);
static void WPE_Pecs_Init(WP_gpe_pecs gpe_pecs_cfg[],WP_handle pecs_handles[]);
void WPT_CompatibleFlowStatisticsReset(WP_handle agg);
void WPT_GenericFlowStatisticsReset(WP_handle agg);
void WPT_IwBridgingPortStatsReset(WP_handle port_handle);
void WPE_StatisticsReset(void);
void WPE_CheckPceFilterStatsReset(void);
void WT_UtilitiesMenu(void);
void CLI_Menu(void);
void CLI_StatMenu(void);
void CLI_PreConfigurationDisplay(void);
void CLI_PreConfigurationMenu(void);
void WT_RunLearningScenario(void);


void WPE_CreateDummyPceHwRules(WP_S32 rules_needed_count);
void WPE_DeleteDummyPceHwRules(void);

static void WPE_DeleteUlPceInterface(void);
static void WPE_CreateUlPceInterface(WP_handle iw_sys);
static void WPE_CreateUlPceFilters(void);
static void WPE_CreateUlPceRules(void);
static void WPE_DeleteUlPceRules(void);
static void WPE_CreateUlPceFilterSets(void);
static void WPE_DeleteDlPceInterface(void);
static void WPE_CreateDlPceInterface(WP_handle iw_sys);
static void WPE_CreateDlPceFilters(void);
static void WPE_CreateDlPceRules(void);
static void WPE_DeleteDlPceRules(void);
static void WPE_DisableDlPceRules(void);
static void WPE_EnableDlPceRules(void);
static void WPE_CreateDlPceFilterSets(void);
static void WPE_DeleteDlSecondRoundPceInterface(void);
static void WPE_CreateDlSecondRoundPceInterface(void);
static void WPE_CreateDlSecondRoundPceFilters(void);
static void WPE_CreateDlSecondRoundPceFilterSets(void);
static void WPE_CreateDlSecondRoundPceRules(void);
static void WPE_DeleteDlSecondRoundPceRules(void);
static void WPE_DisableDlSecondRoundPceRules(void);
static void WPE_EnableDlSecondRoundPceRules(void);
static void WPE_PrintPceGlobalStats(void);

WP_U32 WPT_LocalDisplayXgiDeviceStats(WP_handle enet_handle);
WP_U32 WPT_LocalDisplayDeviceStats(WP_handle device_handle);
void WPE_CheckPceFilterStats(WP_handle filter);

WP_handle devhost;
WP_handle default_agg_host, default_agg, flow_agg[FLOW_AGG_COUNT], ul_flow_agg;
WP_handle pool_2048, pool_2048_2, pool_2048_3;
WP_handle bd_pool[4];

WP_handle gbe_port1, gbe_dev1;
WP_handle xgi_port1, xgi_dev1;
WP_handle xgi_port2, xgi_dev2;
WP_handle xgi_rx_ch_handle1, xgi_tx_ch_handle1;
WP_handle xgi_rx_ch_handle2, xgi_tx_ch_handle2;

WP_handle qniw, qniw_hier;
WP_handle h_iw_port_general, h_iw_port_gbe[FILTER_SET_COUNT * NUM_OF_INPUT_PORT_PER_FILTER_SET], h_iw_port_next_round,
   h_iw_output_port, h_iw_output_port_b, h_iw_port_general_host;
WP_handle rx_host_handle, pce_dl_second_round_if_handle;
WP_handle bport_host;
WP_handle gbe1_rx_ch_handle, gbe1_tx_ch_handle;

WP_handle dl_general_iwsys_bridge, dl_iwsys_bridge[NUM_OF_INPUT_PORT_PER_FILTER_SET];

/* PCE */
WP_pce_global_stat pce_global_stats;
WP_handle PCE_filter[100];
WP_handle dl_filter_set[FILTER_SET_COUNT];
WP_handle ul_filter_set;
WP_handle PCE_rule_handle[300*1024];

WP_handle rule_iterator_handle = 0;

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

static void F_MyAppIndicateEventIwgpLearning(WP_tag tag, WP_U32 data, WP_U32 info)
{
   learning_interrupt_counter++;
}

WP_S32 main(WP_S32 argc, WP_CHAR **argv)
{
   WT_Start("111002", NULL, __FILE__);

   CLI_Menu();

   /* Reboot */
   WT_Reboot();

   exit(0);
   return 1;
}

void CLI_DeleteTest(void)
{
   WP_handle status;

   /* Delete UL PCE interface */
   WPE_DeleteUlPceInterface();

   /* Delete UL Flow agg */
   status = WP_IwFlowAggregationDelete(ul_flow_agg);
   terminate_on_error(status, "WP_IwFlowAggregationDelete()",__LINE__);

   /* Delete default Flow agg */
   status = WP_IwFlowAggregationDelete(default_agg);
   terminate_on_error(status, "WP_IwFlowAggregationDelete()",__LINE__);

   CLI_DeleteGenericFA();

   /* Delete the DL PCE filters and filter sets */
   WPE_DeleteDlPceInterface();

   /* Delete the DL Second round filters and filter sets */
   WPE_DeleteDlSecondRoundPceInterface();

#ifdef WP_BOARD_WDS3_SL
   WPE_RxBindingDelete(gbe1_rx_ch_handle, dl_general_iwsys_bridge);
#else
   WPE_RxBindingDelete(xgi_rx_ch_handle1, dl_general_iwsys_bridge);
   WPE_RxBindingDelete(xgi_rx_ch_handle2, dl_general_iwsys_bridge);
#endif
   
   WPE_ChannelsDelete();
   
   if (aging_iterator_handle)
   {
      status = WP_PceIteratorDelete(aging_iterator_handle);
      terminate_on_error(status, "WP_PceIteratorDelete",__LINE__);
      aging_iterator_handle = 0;
   }
   
   /* Release WDDI */
   WP_DriverRelease();
}


void CLI_CreateMultipleFdbRuleScenarios(void)
{
   WPE_CreateDlSecondRoundPceRules();

   WPE_CreateDlPceRules();

   printf("\n\nForwarding Rules created\n");
}

void CLI_DeleteMultipleFdbRuleScenarios(void)
{
   WPE_DeleteDlPceRules();

   WPE_DeleteDlSecondRoundPceRules();

   printf("\n\nForwarding Rules deleted\n");
}

void CLI_DisableMultipleFdbRuleScenarios(void)
{
   WPE_DisableDlPceRules();

   WPE_DisableDlSecondRoundPceRules();

   printf("\n\nForwarding Rules disabled\n");
}

void CLI_EnableMultipleFdbRuleScenarios(void)
{
   WPE_EnableDlPceRules();

   WPE_EnableDlSecondRoundPceRules();

   printf("\n\nForwarding Rules Enabled\n");
}

void CLI_DeleteGenericFA(void)
{
   WP_U32 i;
   WP_handle status;
   
   for (i=0; i<FLOW_AGG_COUNT; i++)
   {
      status = WP_IwFlowAggregationDelete(flow_agg[i]);
      terminate_on_error(status, "WP_IwFlowAggregationDelete()",__LINE__);
   }
}

void CLI_CreateTest(void)
{
  WP_handle status;
  WP_U32 i;  

#if DEBUG
   WP_ControlRegister(WP_DEBUG_CALLBACK_WDDI_LOG,logCallBack);
   WP_ControlRegister(WP_DEBUG_CALLBACK_FILE_LINE_ERROR,WPE_DebugCallback);
#endif

   WPE_LayerTwoSystemCreate();

   status = WP_SysCommit();
   terminate_on_error(status, "WP_SysCommit",__LINE__);
   
   WPE_Pecs_Init(gpe_pecs_cfg,pecs_handles);

#if DEBUG
   status = WPU_DebugEventGroupLoad(WP_WINPATH(DEFAULT_WPID),-1,"IWGP_MODULE_EXITS");
   terminate_on_error(status, "WPU_DebugEventGroupLoad",__LINE__);

   status = WPU_DebugEventGroupLoad(WP_WINPATH(DEFAULT_WPID),-1,"IWGP_SUBMODULE_EXITS");
   terminate_on_error(status, "WPU_DebugEventGroupLoad",__LINE__);

   status = WPU_DebugEventGroupLoad(WP_WINPATH(DEFAULT_WPID),-1,"IWGP_SUBMODULE_ENTRIES");
   terminate_on_error(status, "WPU_DebugEventGroupLoad",__LINE__);

   status = WPU_DebugEventGroupLoad(WP_WINPATH(DEFAULT_WPID),-1,"IWGP_LOCKS");
   terminate_on_error(status, "WPU_DebugEventGroupLoad",__LINE__);
#endif

   WPE_ChannelsCreate();

   br_agg_gbe->txfunc = rx_host_handle;
   default_agg_host =  WP_IwFlowAggregationCreate(WP_WINPATH(DEFAULT_WPID),
                                             WP_IW_VLAN_AWARE_BRIDGING_MODE,
                                             &br_agg_gbe[0]);
   terminate_on_error(default_agg_host, "WP_IwFlowAggregationCreate()",__LINE__);


   /* Create Generic IW system */
   dl_general_iwsys_bridge = WP_IwSystemCreate(WP_WINPATH(DEFAULT_WPID), WP_IW_VLAN_AWARE_BRIDGING_MODE, &iw_sys_bridging[0]);
   terminate_on_error(dl_general_iwsys_bridge, "WP_IwSystemCreate",__LINE__);

   iwhost_bport_config->flow_agg = default_agg_host;
   h_iw_port_general_host = WP_IwPortCreate(dl_general_iwsys_bridge, &iwhost_bport_config[0]);
   terminate_on_error(h_iw_port_general_host," WP_IwPortCreate()",__LINE__);

#ifdef WP_BOARD_WDS3_SL
   dl_tx_agg_gbe->txfunc = gbe1_tx_ch_handle;
#else
   dl_tx_agg_gbe->txfunc = xgi_tx_ch_handle2;
#endif
   dl_tx_agg_gbe->iw_port = h_iw_port_general_host;
   dl_tx_agg_gbe->pecs_handle = pecs_handles[0];
   dl_tx_agg_gbe->pecs_flow_info=(void *) &brouter_pecs_flow_info[0];
   default_agg =  WP_IwFlowAggregationCreate(WP_WINPATH(DEFAULT_WPID),
                                             WP_IW_GENERIC_MODE,
                                             &dl_tx_agg_gbe);
   terminate_on_error(default_agg, "WP_IwFlowAggregationCreate()",__LINE__);


   /* create output IW Ports */
   WPE_PortsCreate(dl_general_iwsys_bridge, &h_iw_output_port);
   WPE_PortsCreate(dl_general_iwsys_bridge, &h_iw_output_port_b);

   /* Create next round IW port */
   WPE_PortsCreate(dl_general_iwsys_bridge, &h_iw_port_next_round);

   /* Create IW systems */
   for(i=0; i<NUM_OF_INPUT_PORT_PER_FILTER_SET; i++)
   {
      dl_iwsys_bridge[i] = WP_IwSystemCreate(WP_WINPATH(DEFAULT_WPID), WP_IW_VLAN_AWARE_BRIDGING_MODE, &iw_sys_bridging[0]);
      terminate_on_error(dl_iwsys_bridge[i], "WP_IwSystemCreate",__LINE__);
   }
   
   /* create Input IW Ports */
   for(i=0; i<(FILTER_SET_COUNT * NUM_OF_INPUT_PORT_PER_FILTER_SET); i++)
   {
      WPE_PortsCreate(dl_iwsys_bridge[(i%2)], &h_iw_port_gbe[i]);
   }

   /* Create General IW port */
   WPE_PortsCreate(dl_general_iwsys_bridge, &h_iw_port_general);
   WPE_DlRxBindingCreate(dl_general_iwsys_bridge);

   for (i=0; i<FLOW_AGG_COUNT; i++)
   {
#ifdef WP_BOARD_WDS3_SL
      dl_tx_agg_gbe->txfunc = gbe1_tx_ch_handle;
#else
      dl_tx_agg_gbe->txfunc = xgi_tx_ch_handle2;
#endif
      dl_tx_agg_gbe->iw_port = h_iw_output_port;
      dl_tx_agg_gbe->pecs_handle = pecs_handles[0];
      dl_tx_agg_gbe->pecs_flow_info=(void *) &brouter_pecs_flow_info[0];
      flow_agg[i] =  WP_IwFlowAggregationCreate(WP_WINPATH(DEFAULT_WPID),
                                                WP_IW_GENERIC_MODE,
                                                &dl_tx_agg_gbe);
      terminate_on_error(flow_agg[i], "WP_IwFlowAggregationCreate()",__LINE__);
   }

#ifdef WP_BOARD_WDS3_SL
   ul_tx_agg_gbe->txfunc = gbe1_tx_ch_handle;
#else
   ul_tx_agg_gbe->txfunc = xgi_tx_ch_handle1;
#endif
   ul_tx_agg_gbe->iw_port = h_iw_output_port;
   ul_tx_agg_gbe->pecs_handle = pecs_handles[0];
   ul_tx_agg_gbe->pecs_flow_info=(void *) &brouter_pecs_flow_info[0];
   ul_flow_agg =  WP_IwFlowAggregationCreate(WP_WINPATH(DEFAULT_WPID),
                                             WP_IW_GENERIC_MODE,
                                             &ul_tx_agg_gbe);
   terminate_on_error(ul_flow_agg, "WP_IwFlowAggregationCreate()",__LINE__);

   WPE_SystemEnable();

   status = WP_IwSystemBuild(dl_general_iwsys_bridge);
   terminate_on_error(status, "WP_IwSystemBuild()",__LINE__);

   /* Creates the DL Second round PCE Interface, filters and filter sets (FilterSet B) */
   WPE_CreateDlSecondRoundPceInterface();

   /* Creates the DL PCE filters and rules (and filter sets A,C,D,E) */
   WPE_CreateDlPceInterface(dl_general_iwsys_bridge);

   /* Creates the UL PCE filters and rules */
   WPE_CreateUlPceInterface(dl_general_iwsys_bridge);

   /* PCE Statistics reset */
   WPE_CheckPceFilterStatsReset();
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
   WP_U32 i, rule_index;
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
      printf("a) Display forwarding rules\n");
      printf("m) Main Menu \n");

      cmd = getchar();

     if (cmd == '1') {

#ifdef WP_BOARD_WDS3_SL
        /* Device Statistics */
        printf("\nDevice GE1 statistics\n");
        WPT_LocalDisplayDeviceStats(gbe_dev1);
#else
         /* Device Statistics */
         printf("\nDevice XGI statistics\n");
         WPT_LocalDisplayXgiDeviceStats(xgi_dev1);

         /* Device Statistics */
         printf("\nDevice XGI statistics\n");
         WPT_LocalDisplayXgiDeviceStats(xgi_dev2);
#endif
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
         WPT_GenericFlowStatistics(default_agg);

         for (i=0; i<FLOW_AGG_COUNT; i++)
         {
            printf("\nFlow agg %s statistics\n",flow_agg_string[i]);
            WPT_GenericFlowStatistics(flow_agg[i]);
         }

         printf("\nFlow agg ul statistics\n");
         WPT_GenericFlowStatistics(ul_flow_agg);
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

#if defined(WP_BOARD_WDS3)
         status = WP_MemoryAvailable(WP_WINPATH(DEFAULT_WPID), WP_BUS_PACKET, APP_BANK_PACKET, &bytes);
         printf("Memory status: Packet bus bytes left   = 0x%08x\n", bytes);
#endif
   
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
      else if (cmd == 'a')
      {
         /* Create forwarding rules for filter set B */
         for(i=0; i<num_of_rules_per_filter_set; i++)
         {
            rule_index = RULES_A_FORWARDING_START + i;
            WP_Display(WP_WINPATH(DEFAULT_WPID), PCE_rule_handle[rule_index], 0, 0);
         }

         /* Create forwarding rules for filter set B */
         for(i=0; i<num_of_rules_per_filter_set; i++)
         {
            rule_index = RULES_B_FORWARDING_START + i;
            WP_Display(WP_WINPATH(DEFAULT_WPID), PCE_rule_handle[rule_index], 0, 0);
         }

         /* Create forwarding rules for filter set B */
         for(i=0; i<num_of_rules_per_filter_set; i++)
         {
            rule_index = RULES_C_FORWARDING_START + i;
            WP_Display(WP_WINPATH(DEFAULT_WPID), PCE_rule_handle[rule_index], 0, 0);
         }

         /* Create forwarding rules for filter set B */
         for(i=0; i<num_of_rules_per_filter_set; i++)
         {
            rule_index = RULES_D_FORWARDING_START + i;
            WP_Display(WP_WINPATH(DEFAULT_WPID), PCE_rule_handle[rule_index], 0, 0);
         }

         /* Create forwarding rules for filter set B */
         for(i=0; i<num_of_rules_per_filter_set; i++)
         {
            rule_index = RULES_E_FORWARDING_START + i;
            WP_Display(WP_WINPATH(DEFAULT_WPID), PCE_rule_handle[rule_index], 0, 0);
         }
      }
      if (cmd == 'm')
      {
         printf("\n\n");
         break;
      }
   }
}

WP_U32 CLI_GetNumber(WP_CHAR *text, WP_U32 low, WP_U32 high)
{
   WP_CHAR val_str[10], *ret_val;
   WP_U32  val;
   
   while(1)
   {
      printf("\n\n%s: \n", text);
      
      /* Get Shaping Parameter */
      ret_val = gets(val_str);
      if((ret_val == NULL) || (strlen(val_str) > 10))
      {
         continue;
      }
      val = atoi(val_str);
      
      if (val >= low && val <= high)
         break;
   }
   
   printf("\nyou choose = %d\n\n", val);
   return val;
}

void CLI_PreConfigurationDisplay(void)
{
   printf("\n\n Init Configuration\n");
   printf("----------------------------\n");
   printf("Num of HW PCE Rules %d\n", pce_init.hardware_rules_num);
   if (TestType == TEST_HW_FDB_ONLY)
      printf("FDB Mode - HW FDB Only\n");
   else if (TestType == TEST_SW_FDB_ONLY)
      printf("FDB Mode - SW FDB Only\n");
   else
      printf("FDB Mode - Hybrid Mode\n");
   printf("SW FDB Max Rules %d\n", pce_init.sw_fdb_info->max_fdb_rules);
   printf("SW FDB Bus %d\n", pce_init.sw_fdb_info->sw_fdb_bus);
   printf("SW FDB Threshold %d\n", pce_init.sw_fdb_info->fdb_threshold);
   if (filter_set_mode == WT_FORWARDING_ONLY)
      printf("Filter Set Structure is Forwarding\n");
   else if (filter_set_mode == WT_LEARNING_ONLY)
      printf("Filter Set Structure is Learning\n");
   else 
      printf("Filter Set Structure is Forwarding and Learning\n");
   printf("FDB Rules per filter set %d\n", num_of_rules_per_filter_set);
   if (hybrid_evenly)
      printf("Hybrid Forwarding Creation method is HW first\n");
   else
      printf("Hybrid Forwarding Creation method is divided to half HW and half SW\n");
}

void CLI_PreConfigurationMenu(void)
{
   WP_U8 cmd;
   WP_CHAR temp[100];
   
   
   while(1)
   {
      printf("\n\nInit Configuration (Pre Commit)\n");
      printf("----------------------------------------\n");
      printf("1) Num of PCE HW Rules\n");
      printf("2) FDB Mode\n");
      printf("3) SW FDB Max FDB Size\n");
      printf("4) SW FDB Bus\n");
      printf("5) SW FDB Threshold\n");
      printf("6) System Set Configuration\n");
      printf("7) Display System Configuration\n");
      printf("8) Filter Set Structure\n");
      printf("9) Forwarding Rule Creation method\n");
      printf("m) Main Menu \n");

      cmd = getchar();

      if (cmd == '1') {
         pce_init.hardware_rules_num = CLI_GetNumber("choose Num of PCE HW Rules (0 - 32K)", 0, 32767);
      }
      else if (cmd == '2') {
         if (rules_created)
         {
            printf("Error - you need to Delete the Scenario\n");
            continue;
         }

         TestType = CLI_GetNumber("choose FDB Mode (0 - SW FDB Only, 1 - SW FDB Hybrid Mode, 2 - HW FDB Only)", 0, 2);
         if (TestType == TEST_HW_FDB_ONLY)
            pce_init.enhanced_mode = WP_NO_ENHANCMENT;
         else
            pce_init.enhanced_mode = WP_SW_FDB_ENABLE_MODE;

         if (TestType == TEST_SW_FDB_ONLY)
         {
            pce_init.sw_fdb_info->fdb_threshold = 0;
         }
      }
      else if (cmd == '3') {
         pce_init.sw_fdb_info->max_fdb_rules = CLI_GetNumber("choose SW FDB Max FDB Rules (0 ..(256K - 1))", 0, (256*1024 - 1));
         num_of_rules_per_filter_set = pce_init.sw_fdb_info->max_fdb_rules/5;
      }
      else if (cmd == '4') {
         pce_init.sw_fdb_info->sw_fdb_bus = CLI_GetNumber("choose SW FDB Bus (0 - Param , 1 - Packet, 2 - Internal, 3 - Host)", 0, 3);
      }
      else if (cmd == '5') {
         if (TestType == TEST_HYBRID_FDB)
         {
            sprintf(temp, "choose SW FDB Threshold (0 - %d)", pce_init.hardware_rules_num);
            pce_init.sw_fdb_info->fdb_threshold = CLI_GetNumber(temp, 0, pce_init.hardware_rules_num);
         }
         else
            printf("Error - can be set only in hybrid Mode\n");
      }
      else if (cmd == '6') {
         if (rules_created)
         {
            printf("Error - you need to Delete the Scenario\n");
            continue;
         }
         if (start_system)
            CLI_DeleteTest();

         if (TestType == TEST_HW_FDB_ONLY)
            num_of_rules_per_filter_set = (pce_init.hardware_rules_num - 100)/5;

         CLI_CreateTest();
         start_system = 1;

         init_sys_free_pce_hw_rules = free_pce_hw_rules;
         
         CLI_PreConfigurationDisplay();         
         
         printf("System initialized\n");
      }
      else if (cmd == '7') {
         CLI_PreConfigurationDisplay();         
      }
      else if (cmd == '8') {
         filter_set_mode = CLI_GetNumber("choose Filter Set Structure (0 - Forwarding Only, 1 - Learning Only, 2 - Forwarding and Learning)", 0, 2);
      }
      else if (cmd == '9') {
         if (TestType == TEST_HYBRID_FDB)
         {
            hybrid_evenly = CLI_GetNumber("choose Hybrid Forwarding Rule Creation method (0 - 50% HW and 50% SW. Note that num of HW rules - rules per filter set can't be over 7000), 1 - As much HW Rules as possible )", 0, 1);

            if (hybrid_evenly == 0)
               free_pce_hw_rules = pce_init.hardware_rules_num;
         }
         else
            printf("Error - can be set only in hybrid Mode\n");
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
   WP_U32 i;
   
#ifdef WP_BOARD_WDS3_SL
   /* Device Statistics */
   WPT_LocalDeviceStatsReset(gbe_dev1);
#else
   WPT_LocalDeviceStatsReset(xgi_dev1);
   WPT_LocalDeviceStatsReset(xgi_dev2);
#endif

   /* PCE Statistics reset */
   WPE_CheckPceFilterStatsReset();

   WPT_IwBridgingPortStatsReset(h_iw_output_port);
   WPT_IwBridgingPortStatsReset(h_iw_output_port_b);
   WPT_IwBridgingPortStatsReset(h_iw_port_next_round);
   WPT_IwBridgingPortStatsReset(h_iw_port_general);

   for (i=0; i<(FILTER_SET_COUNT * NUM_OF_INPUT_PORT_PER_FILTER_SET); i++)
   {
      WPT_IwBridgingPortStatsReset(h_iw_port_gbe[i]);
   }

   for (i = 0; i< FLOW_AGG_COUNT; i++)
      WPT_GenericFlowStatisticsReset(flow_agg[i]);

   WPT_GenericFlowStatisticsReset(ul_flow_agg);

   WPT_GenericFlowStatisticsReset(default_agg);

   printf("\n\nStatistics have been cleared\n");
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
//            WPU_WinutilCall("taot");
            printf("\n");
            WPU_WinutilCall("tast");
            printf("\n");
//            WPU_WinutilCall("tab");
//            printf("\n");
//            WPU_WinutilCall("taom");
//            printf("\n");
//            WPU_WinutilCall("dmap");
//            printf("\n");
//            WPU_WinutilCall("bmap");
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
            WP_status status = WPU_DebugSwLogRead(WP_SYSHANDLE(0),
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

void WT_RunAgingScenario(void)
{
   WP_handle rule_handle;
   WP_U32 aged_rules = 0;
   WP_U32 StartTimeStamp = 0, CurrentTimeStamp, TimeStampDelta;
   
   if (!aging_iterator_handle)
   {
      aging_iterator_handle = WP_PceIteratorCreate(WP_WINPATH(DEFAULT_WPID), 
                                                   WP_PCE_ITERATOR_TYPE_AGED_RULE, 
                                                   (void *)&PCE_filter[FILTER_SET_A_FORWARDING]);
      terminate_on_error(aging_iterator_handle,"WP_PceIteratorCreate",__LINE__);
   }
   

   StartTimeStamp = WT_TimeRead();
   
   rule_handle = WP_PceIteratorGetFirst(aging_iterator_handle);
   if (!WP_ERROR_P(rule_handle))
   {
      aged_rules++;

      while(1)
      {
         rule_handle = WP_PceIteratorGetNext(aging_iterator_handle);
         if (WP_ERROR_P(rule_handle))
         {
            if (WP_ERROR(rule_handle) != WP_ERR_PCE_ITERATOR_NO_ITEMS)
               printf("\n\nerror %s \n", WP_ErrorString(rule_handle));
            break;
         }
         
         aged_rules++;
      }
   }
   else
      if (WP_ERROR(rule_handle) != WP_ERR_PCE_ITERATOR_NO_ITEMS)
         printf("\n\nerror %s \n", WP_ErrorString(rule_handle));
   
   CurrentTimeStamp = WT_TimeRead();

   TimeStampDelta = WT_TimeDelta(CurrentTimeStamp, StartTimeStamp);
   printf("\n\nTime delta (in micro sec) %d\n",TimeStampDelta);

   printf("%d Aged Rules\n",aged_rules);
}

void WT_DeleteLearntRules(void)
{
   WP_U32 ii, rule_index;
   WP_handle status;
   WP_U32 temp,temp2;
   
   WP_U32 StartTimeStamp = 0, CurrentTimeStamp, TimeStampDelta;

   StartTimeStamp = WT_TimeRead();

   for(ii=0; ii < learnt_rules; ii++)
   {
      rule_index = RULES_A_FORWARDING_START + ii;
      
      status = WP_PceRuleDelete(PCE_rule_handle[rule_index]);
      terminate_on_error(status, "WP_PceRuleDelete",__LINE__);
   }

   CurrentTimeStamp = WT_TimeRead();

   TimeStampDelta = WT_TimeDelta(CurrentTimeStamp, StartTimeStamp);
   printf("\n\nTime delta (in micro sec) %d\n",TimeStampDelta);

   temp = (1000000 / TimeStampDelta);
   temp2 = 1000000 - (temp*TimeStampDelta);
   temp = (temp*learnt_rules) + ((temp2*learnt_rules)/TimeStampDelta);

   if (learnt_rules)
   {
      printf("Flushing performance - can delete %d rules per second\n",temp);
      printf("\n\nDeleted learnt rules\n");
   }

   learnt_rules = 0;
}

void WT_RunLearningScenario(void)
{
   WP_U32 rules_returned;
   WP_pce_rule_forwarding rule_fwd = {0};
   WP_pce_learned_forwarding_rule learned_rules_list[20000];

   WP_U32 ii, rule_index;
   WP_handle status;
   WP_U32 StartTimeStamp = 0, CurrentTimeStamp, TimeStampDelta;
   
   WP_U32 temp;
   WP_U32 temp2;

#if 1
   CurrentTimeStamp = WT_TimeRead();
   TimeStampDelta = WT_TimeDelta(CurrentTimeStamp, StartTimeStamp); // morris
   temp = (1000000 / TimeStampDelta);
   temp2 = 1000000 - (temp*TimeStampDelta);
#else
   WP_U32 temp = (1000000 / TimeStampDelta);
   WP_U32 temp2 = 1000000 - (temp*TimeStampDelta);
#endif

   rule_index = RULES_A_FORWARDING_START;
   required_learning_rules = NUM_OF_REQUIRED_LEARNING_RULES;

   printf("learning_interrupt_counter %d\n",learning_interrupt_counter);
   printf("required_learning_rules %d\n",required_learning_rules);
   
   if (learnt_rules)
      WT_DeleteLearntRules();
   
   while(required_learning_rules > 0)
   {
      WP_Delay(5);

   if (!learning_interrupt_counter)
      continue;
      
      if (!StartTimeStamp)
         StartTimeStamp = WT_TimeRead();
   
      status = WP_PceLearningQueueService(WP_WINPATH(DEFAULT_WPID), 
                                          &(learned_rules_list[0]),
                                          200,
                                          &rules_returned);
      terminate_on_error(status, "WP_PceLearningQueueService", __LINE__);
      learning_interrupt_counter = 0;

      for (ii =0; ii<rules_returned; ii++)
      {
         memcpy(&rule_fwd, &(learned_rules_list[ii].fwd_rule), sizeof(WP_pce_rule_forwarding));
         PCE_rule_handle[rule_index] = WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID),
                                                        WP_PCE_RULE_FORWARDING,
                                                        &rule_fwd);
         if(!WP_ERROR_P(PCE_rule_handle[rule_index]))
         {
            rule_index++;
            learnt_rules++;
            required_learning_rules--;
         }

         if (!required_learning_rules)
            break;
      }
   }
   CurrentTimeStamp = WT_TimeRead();

   TimeStampDelta = WT_TimeDelta(CurrentTimeStamp, StartTimeStamp);

   printf("Time delta (in micro sec) %d\n",TimeStampDelta);

   printf("Rules Learnt %d\n",learnt_rules);
   

   temp = (temp*learnt_rules) + ((temp2*learnt_rules)/TimeStampDelta);

   printf("performance Result - Can learn %d rules per second\n", temp);
}

void CLI_Menu(void)
{
   WP_U8 cmd;

   while(1)
   {
      printf("\n\nMenu\n");
      printf("--------\n");
      printf("1) Init configuration menu (pre commit)\n");
      printf("2) Create Forwarding Rules\n");
      printf("3) Delete Forwarding Rules\n");
      printf("4) Disable Forwarding Rules\n");
      printf("5) Enable Forwarding Rules\n");
      printf("6) Start Learning Process\n");
      printf("7) Delete Learnt rules \n");
      printf("8) Aging Scenario\n");
      printf("s) Statistics\n");
      printf("w) winutil\n");
      printf("r) Exit to WinMon and Reboot\n");
      printf("x) Exit to WinMon\n");
      printf("Enter your command:");

      cmd = getchar();
     
      if (cmd == '1') {
         CLI_PreConfigurationMenu();
      }
      if (cmd == '2') {
         if (!start_system)
         {
            printf("Error - you need to Configure the system first\n");
            continue;
         }

         if (rules_created)
         {
            CLI_DeleteMultipleFdbRuleScenarios();
            free_pce_hw_rules = init_sys_free_pce_hw_rules;
         }
         CLI_CreateMultipleFdbRuleScenarios();
         rules_created = 1;
         rules_enabled = 1;
      }
      if (cmd == '3') {
         if (rules_created)
         {           
            CLI_DeleteMultipleFdbRuleScenarios();
            free_pce_hw_rules = init_sys_free_pce_hw_rules;
         }
         rules_created = 0;
      }
      if (cmd == '4') {
         if (rules_created && rules_enabled)
         {       
            CLI_DisableMultipleFdbRuleScenarios();
            rules_enabled = 0;
         }
      }
      if (cmd == '5') {
         if (rules_created && !rules_enabled)
         {           
            CLI_EnableMultipleFdbRuleScenarios();
            rules_enabled = 1;
         }
      }
      if (cmd == '6') {
         if (rules_created && !rules_enabled)
            printf("\n\nLearning scenario should be run with no forwarding rules, delete the forwarding rules\n");
         else
            WT_RunLearningScenario();        
         }
      if (cmd == '7') {
         WT_DeleteLearntRules();         
      }
      if (cmd == '8') {
         if (learnt_rules)
            WT_RunAgingScenario();         
         else
            printf("\n\nNo learning has been done\n");
      }
      if (cmd == 's') {
         CLI_StatMenu();
      }
      else if (cmd == 'w')
      {
         WT_UtilitiesMenu();
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

   int_ram_partition.policer_entries = 0;
   int_ram_partition.pce_hw_rules_num = pce_init.hardware_rules_num;
   WT_SetIntRamPartition(WP_WINPATH(DEFAULT_WPID), &int_ram_partition);

   status = WP_SysInit(WP_WINPATH(DEFAULT_WPID), context);
   terminate_on_error(status, "WP_SysInit",__LINE__);


#ifndef WP_BOARD_WDS3_SL
   status = WPX_BoardConfigure(WP_WINPATH(DEFAULT_WPID), WPX_CONFIGURE_2UPI_L3_2XGI_6SGMII);
   terminate_on_error(status, "WPX_BoardConfigure()", __LINE__);

   status = WPX_BoardXgiConfig(WP_WINPATH(DEFAULT_WPID), WP_PORT_XGI1);
   terminate_on_error(status, "WPX_BoardXgiConfig()", __LINE__);   

   status = WPX_BoardXgiConfig(WP_WINPATH(DEFAULT_WPID), WP_PORT_XGI2);
   terminate_on_error(status, "WPX_BoardXgiConfig()", __LINE__);   

   status = WPX_BoardSerdesInit(WP_WINPATH(DEFAULT_WPID), WP_PORT_XGI1, WP_FALSE); 
   terminate_on_error(status, "WPX_BoardSerdesInit",__LINE__);

   status = WPX_BoardSerdesInit(WP_WINPATH(DEFAULT_WPID), WP_PORT_XGI2, WP_TRUE); 
   terminate_on_error(status, "WPX_BoardSerdesInit",__LINE__);
#else
   status = WPX_BoardConfigure(WP_WINPATH(DEFAULT_WPID), WPX_CONFIGURE_DEFAULT_CHECKIN);
   terminate_on_error(status, "WPX_BoardConfigure()", __LINE__);

   status = WPX_BoardSerdesInit(WP_WINPATH(DEFAULT_WPID), WP_PORT_ENET11, WP_FALSE);
   terminate_on_error(status, "WPX_BoardSerdesInit()", __LINE__);      
#endif

   pce_init.parser_info = &pce_parser_info;
   status = WP_ModuleInit(WP_SYSHANDLE(DEFAULT_WPID), WP_WDDI_MODULE_PCE, &pce_init);
   terminate_on_error(status, "WPI_PceModuleInit()",__LINE__);
   
   status = WP_ControlRegister(WP_EVENT_IWGP_LEARNING_QUEUE, &F_MyAppIndicateEventIwgpLearning);
   terminate_on_error(status, "WP_ControlRegister()", __LINE__);

   free_pce_hw_rules = pce_init.hardware_rules_num;

   status = WP_ModuleInit(WP_SYSHANDLE(DEFAULT_WPID), WP_WDDI_MODULE_GPE, &gpe_init);
   terminate_on_error(status, "WPI_GpeModuleInit()", __LINE__);

   /*Creates buffer pools*/
   buffer_data_2048->n_buffers = 300;
   buffer_data_2048->bus = BUFFER_BUS;
   buffer_data_2048->bank = BUFFER_BANK;
   pool_2048 = WP_PoolCreate(WP_WINPATH(DEFAULT_WPID), WP_pool_iwbuffer, buffer_data_2048);
   terminate_on_error(pool_2048, "WP_PoolCreate() pool_2048",__LINE__);

   /* Create an interworking queue node */
   qn_iw->adjunct_pool = pool_2048;
   qniw = WP_QNodeCreate(WP_WINPATH(DEFAULT_WPID), WP_QNODE_IWQ| WP_QNODE_OPT_FMU, qn_iw);
   terminate_on_error(qniw, "WP_QNodeCreate() qniw",__LINE__);

#ifndef WP_BOARD_WDS3_SL
   /*Creates buffer pools*/
   buffer_data_2048->n_buffers = 3000;
   pool_2048_2 = WP_PoolCreate(WP_WINPATH(DEFAULT_WPID), WP_pool_iwbuffer, buffer_data_2048);
   terminate_on_error(pool_2048_2, "WP_PoolCreate() pool_2048",__LINE__);

   /*Creates buffer pools*/
   buffer_data_2048->bus = WP_BUS_HOST;
   buffer_data_2048->bank = APP_BANK_HOST;
   pool_2048_3 = WP_PoolCreate(WP_WINPATH(DEFAULT_WPID), WP_pool_iwbuffer, buffer_data_2048);
   terminate_on_error(pool_2048_3, "WP_PoolCreate() pool_2048_2",__LINE__);

   bd_conf->buffer_pool = pool_2048_2;
   bd_conf->bd_bus = WP_BUS_INTERNAL;
   bd_pool[0] = WP_PoolCreate(WP_WINPATH(DEFAULT_WPID),WP_POOL_IW_BD, bd_conf);
   terminate_on_error(bd_pool[0], "WP_PoolCreate() bd_pool", __LINE__);

   bd_conf->buffer_pool = pool_2048_3;
   bd_conf->bd_bus = WP_BUS_PARAM;
   bd_pool[1] = WP_PoolCreate(WP_WINPATH(DEFAULT_WPID),WP_POOL_IW_BD, bd_conf);
   terminate_on_error(bd_pool[1], "WP_PoolCreate() bd_pool", __LINE__);


   /*Configure and create Hierarchical Qnode*/
   group_highp[0].bd_pool[0] = bd_pool[0];
   group_highp[0].bd_pool[1] = bd_pool[0];

   group_lowp[0].bd_pool[0] = bd_pool[1];
   group_lowp[0].bd_pool[1] = bd_pool[1];

   iw_hierq->iw_group[0] = group_highp[0];
   iw_hierq->iw_group[1] = group_lowp[0];

   qniw_hier = WP_QNodeCreate(WP_WINPATH(DEFAULT_WPID), WP_QNODE_IW_HIERARCHICAL,iw_hierq);
   terminate_on_error(qniw_hier, "WP_QNodeCreate()",__LINE__);
#endif

   /* Create an IW Host port */
   iwp1 = WP_PortCreate(WP_WINPATH(DEFAULT_WPID), WP_PORT_IW_HOST, NULL);
   terminate_on_error(iwp1, "WP_PortCreate() IW Host",__LINE__);

   /* Create an IW Host Device */
   devhost = WP_DeviceCreate(iwp1, 0, WP_DEVICE_IW_HOST, NULL);
   terminate_on_error(devhost, "WP_DeviceCreate() IW Host",__LINE__);

#ifdef WP_BOARD_WDS3_SL
   /* Create an GEnet Port (the device that use "regular" mode) */
   enet_s_fast->interface_mode = WP_ENET_SGMII_1000;
   gbe_port1 = WP_PortCreate(WP_WINPATH(DEFAULT_WPID), WP_PORT_ENET11, enet_s_fast);
   terminate_on_error(gbe_port1, "WP_PortCreate()",__LINE__);

   /* Create an GEnet Device (the device that use "regular" mode) */
   enet_dev->max_tx_channels = 1;

   enet_dev->extended_params = NULL;
   gbe_dev1 = WP_DeviceCreate(gbe_port1, 0,WP_DEVICE_ENET, enet_dev);
   terminate_on_error(gbe_dev1, "WP_DeviceCreate() GENET13",__LINE__);

   enet_dev->max_tx_channels = MAX_NUM_OF_CHANNELS;
   enet_dev->extended_params = enet_device_extended_config;
   status = WP_DeviceModify(gbe_dev1,
                            WP_DEV_MOD_ENET_TX_STATMODE |
                            WP_DEV_MOD_ENET_PHYSTATMODE |
                            WP_DEV_MOD_ENET_RX_FLOWCONTROL | 
                            WP_DEV_MOD_ENET_TX_FLOWCONTROL |
                            WP_DEV_MOD_ENET_RX_STATMODE ,
                            enet_dev);
   terminate_on_error(status, "WP_DeviceModify() GENET13",__LINE__);
#else
   /* XGI port*/
   enet_s_fast->interface_mode = WP_ENET_XAUI;
   xgi_port1 = WP_PortCreate(WP_WINPATH(DEFAULT_WPID), WP_PORT_XGI1, enet_s_fast);
   terminate_on_error(xgi_port1, "WP_PortCreate XGI1",__LINE__);

   /* Create an XGI Device */
   enet_dev->max_tx_channels = 1;
   
   enet_dev->extended_params = NULL;
   xgi_dev1 = WP_DeviceCreate(xgi_port1, 0,WP_DEVICE_ENET, enet_dev);
   terminate_on_error(xgi_dev1, "WP_DeviceCreate() XGI",__LINE__);
   
   enet_dev->max_tx_channels = MAX_NUM_OF_CHANNELS;
   enet_dev->extended_params = enet_device_extended_config;
   status = WP_DeviceModify(xgi_dev1,
                            WP_DEV_MOD_ENET_TX_STATMODE |
                            WP_DEV_MOD_ENET_PHYSTATMODE |
                            WP_DEV_MOD_ENET_RX_FLOWCONTROL | 
                            WP_DEV_MOD_ENET_TX_FLOWCONTROL |
                            WP_DEV_MOD_ENET_RX_STATMODE ,
                            enet_dev);
   terminate_on_error(status, "WP_DeviceModify() XGI",__LINE__);

   /* XGI port*/
   enet_s_fast->interface_mode = WP_ENET_XAUI;
   xgi_port2 = WP_PortCreate(WP_WINPATH(DEFAULT_WPID), WP_PORT_XGI2, enet_s_fast);
   terminate_on_error(xgi_port2, "WP_PortCreate XGI1",__LINE__);

   /* Create an XGI Device */
   enet_dev->max_tx_channels = 1;
   
   enet_dev->extended_params = NULL;
   xgi_dev2 = WP_DeviceCreate(xgi_port2, 0,WP_DEVICE_ENET, enet_dev);
   terminate_on_error(xgi_dev2, "WP_DeviceCreate() XGI",__LINE__);
   
   enet_dev->max_tx_channels = MAX_NUM_OF_CHANNELS;
   enet_dev->extended_params = enet_device_extended_config;
   status = WP_DeviceModify(xgi_dev2,
                            WP_DEV_MOD_ENET_TX_STATMODE |
                            WP_DEV_MOD_ENET_PHYSTATMODE |
                            WP_DEV_MOD_ENET_RX_FLOWCONTROL | 
                            WP_DEV_MOD_ENET_TX_FLOWCONTROL |
                            WP_DEV_MOD_ENET_RX_STATMODE ,
                            enet_dev);
   terminate_on_error(status, "WP_DeviceModify() XGI",__LINE__);
#endif
}


void terminate_on_error(WP_handle handle, WP_CHAR *s,WP_U32 line)
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

void terminate(WP_CHAR *s, WP_U32 line)
{
   printf("Error %s, line = %d\n",s, line);
   printf("Test Failed\n");

   WP_DriverRelease();

   WT_Reboot();
   exit(0);
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

   printf ("Rx Mac Unknown:              %08x%08x\n",
           ((WPT_StatField)enet_stats.rx_mac_unknown).part.high,
           ((WPT_StatField)enet_stats.rx_mac_unknown).part.low);

   printf ("Tx Dropped:                  %08x%08x\n",
           ((WPT_StatField)enet_stats.tx_dropped).part.high,
           ((WPT_StatField)enet_stats.tx_dropped).part.low);

   printf ("Rx Error Alignment:          %08x%08x\n",
           ((WPT_StatField)enet_stats.rx_err_alignment).part.high,
           ((WPT_StatField)enet_stats.rx_err_alignment).part.low);

   printf ("Tx Jabber:                   %08x%08x\n",
           ((WPT_StatField)enet_stats.tx_jabber).part.high,
           ((WPT_StatField)enet_stats.tx_jabber).part.low);

   printf ("Rx Error LEN:                %08x%08x\n",
           ((WPT_StatField)enet_stats.rx_err_length).part.high,
           ((WPT_StatField)enet_stats.rx_err_length).part.low);

   printf ("Tx Errors FCS:               %08x%08x\n",
           ((WPT_StatField)enet_stats.tx_err_fcs).part.high,
           ((WPT_StatField)enet_stats.tx_err_fcs).part.low);

   printf ("Rx Error Code:               %08x%08x\n",
           ((WPT_StatField)enet_stats.rx_err_code).part.high,
           ((WPT_StatField)enet_stats.rx_err_code).part.low);

   printf ("Tx Control:                  %08x%08x\n",
           ((WPT_StatField)enet_stats.tx_control).part.high,
           ((WPT_StatField)enet_stats.tx_control).part.low);

   printf ("Rx False Carrier:            %08x%08x\n",
           ((WPT_StatField)enet_stats.rx_false_carrier).part.high,
           ((WPT_StatField)enet_stats.rx_false_carrier).part.low);

   printf ("Tx Oversize:                 %08x%08x\n",
           ((WPT_StatField)enet_stats.tx_oversize).part.high,
           ((WPT_StatField)enet_stats.tx_oversize).part.low);

   printf ("Rx Undersize:                %08x%08x\n",
           ((WPT_StatField)enet_stats.rx_undersize).part.high,
           ((WPT_StatField)enet_stats.rx_undersize).part.low);

   printf ("Tx Undersize:                %08x%08x\n",
           ((WPT_StatField)enet_stats.tx_undersize).part.high,
           ((WPT_StatField)enet_stats.tx_undersize).part.low);

   printf ("Rx Oversize:                 %08x%08x\n",
           ((WPT_StatField)enet_stats.rx_oversize).part.high,
           ((WPT_StatField)enet_stats.rx_oversize).part.low);

   printf ("Tx Fragments:                %08x%08x\n",
           ((WPT_StatField)enet_stats.tx_fragments).part.high,
           ((WPT_StatField)enet_stats.tx_fragments).part.low);

   printf ("Rx Fragments:                %08x%08x\n",
           ((WPT_StatField)enet_stats.rx_fragments).part.high,
           ((WPT_StatField)enet_stats.rx_fragments).part.low);
   printf ("Rx Jabber:                   %08x%08x\n",
           ((WPT_StatField)enet_stats.rx_jabber).part.high,
           ((WPT_StatField)enet_stats.rx_jabber).part.low);
   printf ("Rx Dropped:                  %08x%08x\n",
           ((WPT_StatField)enet_stats.rx_dropped).part.high,
           ((WPT_StatField)enet_stats.rx_dropped).part.low);
#endif
   printf ("HS ENET Device Statistics (DPS)\n");

   printf ("Rx Host Frames:              %08x%08x\n",
           ((WPT_StatField*)&enet_stats.rx_host_frames)->part.high,
           ((WPT_StatField*)&enet_stats.rx_host_frames)->part.low);
   printf ("Rx Iw Frames:                %08x%08x\n",
           ((WPT_StatField*)&enet_stats.rx_iw_frames)->part.high,
           ((WPT_StatField*)&enet_stats.rx_iw_frames)->part.low);
   printf ("Rx Error Host Full:          %08x%08x\n",
           ((WPT_StatField*)&enet_stats.rx_err_host_full)->part.high,
           ((WPT_StatField*)&enet_stats.rx_err_host_full)->part.low);
   printf ("Rx Error Fbp Underrun:       %08x%08x\n",
           ((WPT_StatField*)&enet_stats.rx_err_fbp_underrun)->part.high,
           ((WPT_StatField*)&enet_stats.rx_err_fbp_underrun)->part.low);
   printf ("Rx Error Nonvalid Mac:       %08x%08x\n",
           ((WPT_StatField*)&enet_stats.rx_err_nonvalid_mac)->part.high,
           ((WPT_StatField*)&enet_stats.rx_err_nonvalid_mac)->part.low);
   printf ("Rx Error Mru:                %08x%08x\n",
           ((WPT_StatField*)&enet_stats.rx_err_mru)->part.high,
           ((WPT_StatField*)&enet_stats.rx_err_mru)->part.low);
   printf ("Rx Error Sdu:                %08x%08x\n",
           ((WPT_StatField*)&enet_stats.rx_err_sdu)->part.high,
           ((WPT_StatField*)&enet_stats.rx_err_sdu)->part.low);
   printf ("Tx Error Underrun:           %08x%08x\n",
           ((WPT_StatField*)&enet_stats.tx_err_underrun)->part.high,
           ((WPT_StatField*)&enet_stats.tx_err_underrun)->part.low);
   printf ("Rx Error Overrun:            %08x%08x\n",
           ((WPT_StatField*)&enet_stats.rx_err_overrun)->part.high,
           ((WPT_StatField*)&enet_stats.rx_err_overrun)->part.low);
   printf ("Tx Frames:                   %08x%08x\n",
           ((WPT_StatField*)&enet_stats.tx_frames)->part.high,
           ((WPT_StatField*)&enet_stats.tx_frames)->part.low);
   printf("----------------------------------------------------\n");

   return 0;
}

WP_U32 WPT_LocalDisplayXgiDeviceStats(WP_handle enet_handle)
{
   WP_stats_xgi enet_stats;
   WP_status status;

   memset (&enet_stats,0,sizeof(WP_stats_xgi));

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

   printf ("Rx Mac Unknown:              %08x%08x\n",
           ((WPT_StatField)enet_stats.rx_mac_unknown).part.high,
           ((WPT_StatField)enet_stats.rx_mac_unknown).part.low);

   printf ("Tx Dropped:                  %08x%08x\n",
           ((WPT_StatField)enet_stats.tx_dropped).part.high,
           ((WPT_StatField)enet_stats.tx_dropped).part.low);

   printf ("Rx Error Alignment:          %08x%08x\n",
           ((WPT_StatField)enet_stats.rx_err_alignment).part.high,
           ((WPT_StatField)enet_stats.rx_err_alignment).part.low);

   printf ("Tx Jabber:                   %08x%08x\n",
           ((WPT_StatField)enet_stats.tx_jabber).part.high,
           ((WPT_StatField)enet_stats.tx_jabber).part.low);

   printf ("Rx Error LEN:                %08x%08x\n",
           ((WPT_StatField)enet_stats.rx_err_length).part.high,
           ((WPT_StatField)enet_stats.rx_err_length).part.low);

   printf ("Tx Errors FCS:               %08x%08x\n",
           ((WPT_StatField)enet_stats.tx_err_fcs).part.high,
           ((WPT_StatField)enet_stats.tx_err_fcs).part.low);

   printf ("Rx Error Code:               %08x%08x\n",
           ((WPT_StatField)enet_stats.rx_err_code).part.high,
           ((WPT_StatField)enet_stats.rx_err_code).part.low);

   printf ("Tx Control:                  %08x%08x\n",
           ((WPT_StatField)enet_stats.tx_control).part.high,
           ((WPT_StatField)enet_stats.tx_control).part.low);

   printf ("Rx False Carrier:            %08x%08x\n",
           ((WPT_StatField)enet_stats.rx_false_carrier).part.high,
           ((WPT_StatField)enet_stats.rx_false_carrier).part.low);

   printf ("Tx Oversize:                 %08x%08x\n",
           ((WPT_StatField)enet_stats.tx_oversize).part.high,
           ((WPT_StatField)enet_stats.tx_oversize).part.low);

   printf ("Rx Undersize:                %08x%08x\n",
           ((WPT_StatField)enet_stats.rx_undersize).part.high,
           ((WPT_StatField)enet_stats.rx_undersize).part.low);

   printf ("Tx Undersize:                %08x%08x\n",
           ((WPT_StatField)enet_stats.tx_undersize).part.high,
           ((WPT_StatField)enet_stats.tx_undersize).part.low);

   printf ("Rx Oversize:                 %08x%08x\n",
           ((WPT_StatField)enet_stats.rx_oversize).part.high,
           ((WPT_StatField)enet_stats.rx_oversize).part.low);

   printf ("Tx Fragments:                %08x%08x\n",
           ((WPT_StatField)enet_stats.tx_fragments).part.high,
           ((WPT_StatField)enet_stats.tx_fragments).part.low);

   printf ("Rx Fragments:                %08x%08x\n",
           ((WPT_StatField)enet_stats.rx_fragments).part.high,
           ((WPT_StatField)enet_stats.rx_fragments).part.low);
   printf ("Rx Jabber:                   %08x%08x\n",
           ((WPT_StatField)enet_stats.rx_jabber).part.high,
           ((WPT_StatField)enet_stats.rx_jabber).part.low);
   printf ("Rx Dropped:                  %08x%08x\n",
           ((WPT_StatField)enet_stats.rx_dropped).part.high,
           ((WPT_StatField)enet_stats.rx_dropped).part.low);
#endif
   printf ("Device Statistics (DPS)\n");

   printf ("Rx Host Frames:              %08x%08x\n",
           ((WPT_StatField*)&enet_stats.rx_host_frames)->part.high,
           ((WPT_StatField*)&enet_stats.rx_host_frames)->part.low);
   printf ("Rx Iw Frames:                %08x%08x\n",
           ((WPT_StatField*)&enet_stats.rx_iw_frames)->part.high,
           ((WPT_StatField*)&enet_stats.rx_iw_frames)->part.low);
   printf ("Rx Error Host Full:          %08x%08x\n",
           ((WPT_StatField*)&enet_stats.rx_err_host_full)->part.high,
           ((WPT_StatField*)&enet_stats.rx_err_host_full)->part.low);
   printf ("Rx Error Fbp Underrun:       %08x%08x\n",
           ((WPT_StatField*)&enet_stats.rx_err_fbp_underrun)->part.high,
           ((WPT_StatField*)&enet_stats.rx_err_fbp_underrun)->part.low);
   printf ("Rx Error Nonvalid Mac:       %08x%08x\n",
           ((WPT_StatField*)&enet_stats.rx_err_nonvalid_mac)->part.high,
           ((WPT_StatField*)&enet_stats.rx_err_nonvalid_mac)->part.low);
   printf ("Rx Error Mru:                %08x%08x\n",
           ((WPT_StatField*)&enet_stats.rx_err_mru)->part.high,
           ((WPT_StatField*)&enet_stats.rx_err_mru)->part.low);
   printf ("Rx Error Sdu:                %08x%08x\n",
           ((WPT_StatField*)&enet_stats.rx_err_sdu)->part.high,
           ((WPT_StatField*)&enet_stats.rx_err_sdu)->part.low);
   printf ("Tx Error Underrun:           %08x%08x\n",
           ((WPT_StatField*)&enet_stats.tx_err_underrun)->part.high,
           ((WPT_StatField*)&enet_stats.tx_err_underrun)->part.low);
   printf ("Rx Error Overrun:            %08x%08x\n",
           ((WPT_StatField*)&enet_stats.rx_err_overrun)->part.high,
           ((WPT_StatField*)&enet_stats.rx_err_overrun)->part.low);
   printf ("Tx Frames:                   %08x%08x\n",
           ((WPT_StatField*)&enet_stats.tx_frames)->part.high,
           ((WPT_StatField*)&enet_stats.tx_frames)->part.low);
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

#ifdef WP_BOARD_WDS3_SL
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
#else

   /* Enable the XGI Port */
   status = WP_PortEnable(xgi_port1, WP_DIRECTION_DUPLEX);
   terminate_on_error(status, "WP_PortEnable gbe_port",__LINE__);
   
   /* Enable XGI device */
   status = WP_DeviceEnable(xgi_dev1, WP_DIRECTION_DUPLEX);
   terminate_on_error(status, "WP_DeviceEnable gbe_dev", __LINE__);

   status = WP_ChannelEnable(xgi_rx_ch_handle1);
   terminate_on_error(status, "WP_ChannelEnable Rx XGI",__LINE__);

   status = WP_ChannelEnable(xgi_tx_ch_handle1);
   terminate_on_error(status, "WP_ChannelEnable Tx XGI",__LINE__);

   /* Enable the XGI Port */
   status = WP_PortEnable(xgi_port2, WP_DIRECTION_DUPLEX);
   terminate_on_error(status, "WP_PortEnable gbe_port",__LINE__);
   
   /* Enable XGI device */
   status = WP_DeviceEnable(xgi_dev2, WP_DIRECTION_DUPLEX);
   terminate_on_error(status, "WP_DeviceEnable gbe_dev", __LINE__);

   status = WP_ChannelEnable(xgi_rx_ch_handle2);
   terminate_on_error(status, "WP_ChannelEnable Rx XGI",__LINE__);

   status = WP_ChannelEnable(xgi_tx_ch_handle2);
   terminate_on_error(status, "WP_ChannelEnable Tx XGI",__LINE__);
#endif
}

static void WPE_ChannelsDelete(void)
{
   WP_status status;

#ifdef WP_BOARD_WDS3_SL
   status = WP_ChannelDisable(gbe1_rx_ch_handle);
   terminate_on_error(status, "WP_ChannelDisable() RX_Channel ENET13",__LINE__);

   status = WP_ChannelDelete(gbe1_rx_ch_handle);
   terminate_on_error(status, "WP_ChannelDelete() RX_Channel ENET13",__LINE__);

   status = WP_ChannelDisable(gbe1_tx_ch_handle);
   terminate_on_error(status, "WP_ChannelDisable() TX_Channel ENET13",__LINE__);

   status = WP_ChannelDelete(gbe1_tx_ch_handle);
   terminate_on_error(status, "WP_ChannelDelete() TX_Channel ENET13",__LINE__);
#else
   status = WP_ChannelDisable(xgi_rx_ch_handle1);
   terminate_on_error(status, "WP_ChannelDisable() RX_Channel XGI",__LINE__);

   status = WP_ChannelDelete(xgi_rx_ch_handle1);
   terminate_on_error(status, "WP_ChannelDelete() RX_Channel XGI",__LINE__);

   status = WP_ChannelDisable(xgi_tx_ch_handle1);
   terminate_on_error(status, "WP_ChannelDisable() TX_Channel XGI",__LINE__);

   status = WP_ChannelDelete(xgi_tx_ch_handle1);
   terminate_on_error(status, "WP_ChannelDelete() TX_Channel XGI",__LINE__);

   status = WP_ChannelDisable(xgi_rx_ch_handle2);
   terminate_on_error(status, "WP_ChannelDisable() RX_Channel XGI",__LINE__);

   status = WP_ChannelDelete(xgi_rx_ch_handle2);
   terminate_on_error(status, "WP_ChannelDelete() RX_Channel XGI",__LINE__);

   status = WP_ChannelDisable(xgi_tx_ch_handle2);
   terminate_on_error(status, "WP_ChannelDisable() TX_Channel XGI",__LINE__);

   status = WP_ChannelDelete(xgi_tx_ch_handle2);
   terminate_on_error(status, "WP_ChannelDelete() TX_Channel XGI",__LINE__);
#endif

   status = WP_ChannelDelete(rx_host_handle);
   terminate_on_error(status, "WP_ChannelDelete() RX_Channel Host",__LINE__);
}

static void WPE_ChannelsCreate(void)
{
   WP_tag ch_tag = 0;
   WP_status status;

#ifdef WP_BOARD_WDS3_SL
   /* Create Enet1 fast ("reguler") RX channel */
   gbe1_rx_ch_handle = WP_ChannelCreate(ch_tag++,
                                        gbe_dev1,
                                        qniw,
                                        WP_CH_RX,
                                        WP_ENET,
                                        &gbe_ch[0]);
   terminate_on_error(gbe1_rx_ch_handle, "WP_ChannelCreate() RX_Channel ENET13",__LINE__);

   /* Create Enet1 fast ("reguler") TX channel */
   gbe1_tx_ch_handle = WP_ChannelCreate(ch_tag++,
                                        gbe_dev1,
                                        qniw,
                                        WP_CH_TX,
                                        WP_ENET,
                                        &gbe_ch[0]);
   terminate_on_error(gbe1_tx_ch_handle, "WP_ChannelCreate() TX_Channel ENET13",__LINE__);
   
   status =  WP_IwTxBindingCreate(gbe1_tx_ch_handle, WP_IW_TX_BINDING, &tx_binding_l2pi[0]);
   terminate_on_error(status, "WP_IwTxBindingCreate() TX ENET fast device",__LINE__);
#else
   /* create XGI device rx and tx channels */
   xgi_tx_ch_handle1 = WP_ChannelCreate(ch_tag++, 
                                       xgi_dev1, 
                                       qniw, 
                                       WP_CH_TX, 
                                       WP_ENET, 
                                       &gbe_ch[0]);
   terminate_on_error(xgi_tx_ch_handle1, "WP_ChannelCreate() Enet TX XGI",__LINE__);
   
   xgi_rx_ch_handle1 = WP_ChannelCreate(ch_tag++, 
                                       xgi_dev1, 
                                       qniw_hier, 
                                       WP_CH_RX, 
                                       WP_ENET, 
                                       &gbe_ch[0]);
   terminate_on_error(xgi_rx_ch_handle1, "WP_ChannelCreate() Enet RX XGI",__LINE__);

   /* Create Tx binding to the Tx Channel XGI */
   status = WP_IwTxBindingCreate(xgi_tx_ch_handle1, WP_IW_TX_BINDING, &tx_binding_l2pi[0]);
   terminate_on_error(status, "WP_IwTxBindingCreate() ENET TX XGI",__LINE__);

   /* create XGI device rx and tx channels */
   xgi_tx_ch_handle2 = WP_ChannelCreate(ch_tag++, 
                                       xgi_dev2, 
                                       qniw, 
                                       WP_CH_TX, 
                                       WP_ENET, 
                                       &gbe_ch[0]);
   terminate_on_error(xgi_tx_ch_handle2, "WP_ChannelCreate() Enet TX XGI",__LINE__);
   
   xgi_rx_ch_handle2 = WP_ChannelCreate(ch_tag++, 
                                       xgi_dev2, 
                                       qniw_hier, 
                                       WP_CH_RX, 
                                       WP_ENET, 
                                       &gbe_ch[0]);
   terminate_on_error(xgi_rx_ch_handle2, "WP_ChannelCreate() Enet RX XGI",__LINE__);

   /* Create Tx binding to the Tx Channel XGI */
   status = WP_IwTxBindingCreate(xgi_tx_ch_handle2, WP_IW_TX_BINDING, &tx_binding_l2pi[0]);
   terminate_on_error(status, "WP_IwTxBindingCreate() ENET TX XGI",__LINE__);
#endif

   /*create host rx channel*/
   rx_host_handle = WP_ChannelCreate(0x1111,
                                     devhost,
                                     qniw,
                                     WP_CH_RX,
                                     WP_IW_HOST,
                                     &ch_config_iw[0]);
   terminate_on_error(rx_host_handle, "WP_ChannelCreate() Host sys",__LINE__);
}

static void WPE_PortsCreate(WP_handle iw_sys, WP_handle *iwport)
{
   WP_status status;
     
   iwhost_bport_config->flow_agg = default_agg;
   bport_host = WP_IwPortCreate(iw_sys, &iwhost_bport_config[0]);
   terminate_on_error(bport_host," WP_IwPortCreate()",__LINE__);

   br_agg_gbe->input_bport = bport_host;
   status = WP_IwFlowAggregationModify(default_agg,
                                       WP_IW_FLOW_AGG_B_MOD_BPORT,
                                       &br_agg_gbe[0]);
   terminate_on_error(status," WP_IwFlowAggregationModify()",__LINE__);

   bport_conf.flow_agg = default_agg;
   bport_conf.tag = 0;
   
   *iwport = WP_IwPortCreate(iw_sys, &bport_conf);
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

#ifdef WP_BOARD_WDS3_SL
   status = WP_IwRxBindingCreate(gbe1_rx_ch_handle ,iw_sys,
                                 qniw,
                                 &rx_binding_cfg[0]);
   terminate_on_error(status, "WP_IwRxBindingCreate Enet",__LINE__);
#else
   status = WP_IwRxBindingCreate(xgi_rx_ch_handle1 ,iw_sys,
                                 qniw_hier,
                                 &rx_binding_cfg[0]);
   terminate_on_error(status, "WP_IwRxBindingCreate Enet",__LINE__);

   status = WP_IwRxBindingCreate(xgi_rx_ch_handle2 ,iw_sys,
                                 qniw_hier,
                                 &rx_binding_cfg[0]);
   terminate_on_error(status, "WP_IwRxBindingCreate Enet",__LINE__);
#endif
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

static void WPE_CreateDlPceFilters(void)
{
   WP_pce_filter_classification filter_class = {0};
   WP_pce_filter_forwarding fwd_filter_cfg = {0};
   WP_pce_filter_learning lrn_filter_cfg = {0};

   /* Common parameters for all filters*/
   filter_class.no_match_action = WP_PCE_FILTER_NO_MATCH_CONTINUE;
   filter_class.no_fields_action = WP_PCE_FILTER_NO_FIELDS_DENY;

   filter_class.no_match_result[0].result_type = WP_PCE_RESULT_LAST;

   filter_class.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_RANGE_LOW;
   filter_class.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_USED;
   filter_class.filter_fields[0].mask.vlan_tag = WP_PCE_FIELD_MASK_VLAN_ID;
   filter_class.filter_fields[0].field_id = WP_PCE_FIELD_ID_INT_VLAN_TAG;

   filter_class.filter_fields[1].field_mode = WP_PCE_FIELD_MODE_COMPARE_RANGE_HIGH;
   filter_class.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_USED;
   filter_class.filter_fields[1].mask.vlan_tag = WP_PCE_FIELD_MASK_VLAN_ID;
   filter_class.filter_fields[1].field_id = WP_PCE_FIELD_ID_INT_VLAN_TAG;
   filter_class.filter_fields[2].field_id = WP_PCE_FIELD_ID_LAST;
   PCE_filter[FILTER_SET_A_CLASSIFICATION] = WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_CLASSIFICATION, &filter_class);
   terminate_on_error(PCE_filter[FILTER_SET_A_CLASSIFICATION], "WP_PceFilterCreate",__LINE__);

   
   fwd_filter_cfg.no_match_action = WP_PCE_FILTER_NO_MATCH_CONTINUE;
   fwd_filter_cfg.no_fields_action = WP_PCE_FILTER_NO_FIELDS_DENY;

   fwd_filter_cfg.no_match_result[0].result_type = WP_PCE_RESULT_LAST;

   fwd_filter_cfg.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   fwd_filter_cfg.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   fwd_filter_cfg.filter_fields[1].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   fwd_filter_cfg.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   fwd_filter_cfg.filter_fields[0].field_id   = WP_PCE_FIELD_ID_MAC_DA;
   fwd_filter_cfg.filter_fields[1].field_id = WP_PCE_FIELD_ID_LAST;
   PCE_filter[FILTER_SET_A_FORWARDING] = WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_FORWARDING, &fwd_filter_cfg);
   terminate_on_error(PCE_filter[FILTER_SET_A_FORWARDING], "WP_PceFilterCreate",__LINE__);

   lrn_filter_cfg.forwarding_filter = PCE_filter[FILTER_SET_A_FORWARDING];
   lrn_filter_cfg.no_match_action = WP_PCE_FILTER_NO_MATCH_CONTINUE;
   lrn_filter_cfg.no_fields_action = WP_PCE_FILTER_NO_FIELDS_DENY;

   lrn_filter_cfg.no_match_result[0].result_type = WP_PCE_RESULT_LAST;

   lrn_filter_cfg.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   lrn_filter_cfg.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   lrn_filter_cfg.filter_fields[1].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   lrn_filter_cfg.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   lrn_filter_cfg.filter_fields[0].field_id = WP_PCE_FIELD_ID_MAC_SA;
   lrn_filter_cfg.filter_fields[1].field_id = WP_PCE_FIELD_ID_LAST;
   PCE_filter[FILTER_SET_A_LEARNING] = WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_LEARNING, &lrn_filter_cfg);
   terminate_on_error(PCE_filter[FILTER_SET_A_LEARNING], "WP_PceFilterCreate",__LINE__);


   fwd_filter_cfg.no_fields_action = WP_PCE_FILTER_NO_FIELDS_DENY;

   fwd_filter_cfg.no_match_result[0].result_type = WP_PCE_RESULT_LAST;

   fwd_filter_cfg.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   fwd_filter_cfg.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   fwd_filter_cfg.filter_fields[1].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   fwd_filter_cfg.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_USED;
   fwd_filter_cfg.filter_fields[1].mask.vlan_tag = WP_PCE_FIELD_MASK_VLAN_ID;

   fwd_filter_cfg.filter_fields[0].field_id   = WP_PCE_FIELD_ID_MAC_DA;
   fwd_filter_cfg.filter_fields[1].field_id = WP_PCE_FIELD_ID_INT_VLAN_TAG;
   fwd_filter_cfg.filter_fields[2].field_id = WP_PCE_FIELD_ID_LAST;

   fwd_filter_cfg.no_match_action = WP_PCE_FILTER_NO_MATCH_CONTINUE;
   PCE_filter[FILTER_SET_C_FORWARDING] = WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_FORWARDING, &fwd_filter_cfg);
   terminate_on_error(PCE_filter[FILTER_SET_C_FORWARDING], "WP_PceFilterCreate",__LINE__);

   
   lrn_filter_cfg.forwarding_filter = PCE_filter[FILTER_SET_C_FORWARDING];
   lrn_filter_cfg.no_fields_action = WP_PCE_FILTER_NO_FIELDS_DENY;

   lrn_filter_cfg.no_match_result[0].result_type = WP_PCE_RESULT_LAST;

   lrn_filter_cfg.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   lrn_filter_cfg.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   lrn_filter_cfg.filter_fields[1].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   lrn_filter_cfg.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_USED;
   lrn_filter_cfg.filter_fields[1].mask.vlan_tag = WP_PCE_FIELD_MASK_VLAN_ID;

   lrn_filter_cfg.filter_fields[0].field_id   = WP_PCE_FIELD_ID_MAC_SA;
   lrn_filter_cfg.filter_fields[1].field_id = WP_PCE_FIELD_ID_INT_VLAN_TAG;
   lrn_filter_cfg.filter_fields[2].field_id = WP_PCE_FIELD_ID_LAST;

   lrn_filter_cfg.no_match_action = WP_PCE_FILTER_NO_MATCH_CONTINUE;
   PCE_filter[FILTER_SET_C_LEARNING] = WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_LEARNING, &lrn_filter_cfg);
   terminate_on_error(PCE_filter[FILTER_SET_C_LEARNING], "WP_PceFilterCreate",__LINE__);
   

   fwd_filter_cfg.no_fields_action = WP_PCE_FILTER_NO_FIELDS_DENY;

   fwd_filter_cfg.no_match_result[0].result_type = WP_PCE_RESULT_LAST;

   fwd_filter_cfg.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   fwd_filter_cfg.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   fwd_filter_cfg.filter_fields[1].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   fwd_filter_cfg.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_USED;
   fwd_filter_cfg.filter_fields[1].mask.vlan_tag = WP_PCE_FIELD_MASK_VLAN_ID;

   fwd_filter_cfg.filter_fields[2].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   fwd_filter_cfg.filter_fields[2].mask_mode = WP_PCE_FIELD_MASK_USED;
   fwd_filter_cfg.filter_fields[2].mask.vlan_tag = WP_PCE_FIELD_MASK_VLAN_ID;

   fwd_filter_cfg.filter_fields[0].field_id   = WP_PCE_FIELD_ID_MAC_DA;
   fwd_filter_cfg.filter_fields[1].field_id = WP_PCE_FIELD_ID_INT_VLAN_TAG;
   fwd_filter_cfg.filter_fields[2].field_id = WP_PCE_FIELD_ID_EXT_VLAN_TAG;
   fwd_filter_cfg.filter_fields[3].field_id = WP_PCE_FIELD_ID_LAST;

   if (TestType == TEST_SW_FDB_ONLY)
      fwd_filter_cfg.no_match_action = WP_PCE_FILTER_NO_MATCH_CONTINUE;
   else
      fwd_filter_cfg.no_match_action = WP_PCE_FILTER_NO_MATCH_ACCEPT;
   PCE_filter[FILTER_SET_D_FORWARDING] = WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_FORWARDING, &fwd_filter_cfg);
   terminate_on_error(PCE_filter[FILTER_SET_D_FORWARDING], "WP_PceFilterCreate",__LINE__);

   
   lrn_filter_cfg.forwarding_filter = PCE_filter[FILTER_SET_D_FORWARDING];
   lrn_filter_cfg.no_fields_action = WP_PCE_FILTER_NO_FIELDS_DENY;

   lrn_filter_cfg.no_match_result[0].result_type = WP_PCE_RESULT_LAST;

   lrn_filter_cfg.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   lrn_filter_cfg.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   lrn_filter_cfg.filter_fields[1].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   lrn_filter_cfg.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_USED;
   lrn_filter_cfg.filter_fields[1].mask.vlan_tag = WP_PCE_FIELD_MASK_VLAN_ID;

   lrn_filter_cfg.filter_fields[2].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   lrn_filter_cfg.filter_fields[2].mask_mode = WP_PCE_FIELD_MASK_USED;
   lrn_filter_cfg.filter_fields[2].mask.vlan_tag = WP_PCE_FIELD_MASK_VLAN_ID;

   lrn_filter_cfg.filter_fields[0].field_id   = WP_PCE_FIELD_ID_MAC_SA;
   lrn_filter_cfg.filter_fields[1].field_id = WP_PCE_FIELD_ID_INT_VLAN_TAG;
   lrn_filter_cfg.filter_fields[2].field_id = WP_PCE_FIELD_ID_EXT_VLAN_TAG;
   lrn_filter_cfg.filter_fields[3].field_id = WP_PCE_FIELD_ID_LAST;

   lrn_filter_cfg.no_match_action = WP_PCE_FILTER_NO_MATCH_CONTINUE;
   PCE_filter[FILTER_SET_D_LEARNING] = WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_LEARNING, &lrn_filter_cfg);
   terminate_on_error(PCE_filter[FILTER_SET_D_LEARNING], "WP_PceFilterCreate",__LINE__);
 

   fwd_filter_cfg.no_fields_action = WP_PCE_FILTER_NO_FIELDS_DENY;

   fwd_filter_cfg.no_match_result[0].result_type = WP_PCE_RESULT_LAST;

   fwd_filter_cfg.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   fwd_filter_cfg.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   fwd_filter_cfg.filter_fields[1].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   fwd_filter_cfg.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_USED;
   fwd_filter_cfg.filter_fields[1].mask.vlan_tag = WP_PCE_FIELD_MASK_VLAN_ID;

   fwd_filter_cfg.filter_fields[2].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   fwd_filter_cfg.filter_fields[2].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   fwd_filter_cfg.filter_fields[0].field_id   = WP_PCE_FIELD_ID_MAC_DA;
   fwd_filter_cfg.filter_fields[1].field_id = WP_PCE_FIELD_ID_INT_VLAN_TAG;
   fwd_filter_cfg.filter_fields[2].field_id = WP_PCE_FIELD_ID_IW_SYSTEM;
   fwd_filter_cfg.filter_fields[3].field_id = WP_PCE_FIELD_ID_LAST;

   if (TestType == TEST_SW_FDB_ONLY)
      fwd_filter_cfg.no_match_action = WP_PCE_FILTER_NO_MATCH_CONTINUE;
   else
      fwd_filter_cfg.no_match_action = WP_PCE_FILTER_NO_MATCH_DENY;
   PCE_filter[FILTER_SET_E_FORWARDING] = WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_FORWARDING, &fwd_filter_cfg);
   terminate_on_error(PCE_filter[FILTER_SET_E_FORWARDING], "WP_PceFilterCreate",__LINE__);

   
   lrn_filter_cfg.forwarding_filter = PCE_filter[FILTER_SET_E_FORWARDING];
   lrn_filter_cfg.no_fields_action = WP_PCE_FILTER_NO_FIELDS_DENY;

   lrn_filter_cfg.no_match_result[0].result_type = WP_PCE_RESULT_LAST;

   lrn_filter_cfg.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   lrn_filter_cfg.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   lrn_filter_cfg.filter_fields[1].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   lrn_filter_cfg.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_USED;
   lrn_filter_cfg.filter_fields[1].mask.vlan_tag = WP_PCE_FIELD_MASK_VLAN_ID;

   lrn_filter_cfg.filter_fields[2].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   lrn_filter_cfg.filter_fields[2].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   lrn_filter_cfg.filter_fields[0].field_id   = WP_PCE_FIELD_ID_MAC_SA;
   lrn_filter_cfg.filter_fields[1].field_id = WP_PCE_FIELD_ID_INT_VLAN_TAG;
   lrn_filter_cfg.filter_fields[2].field_id = WP_PCE_FIELD_ID_IW_SYSTEM;
   lrn_filter_cfg.filter_fields[3].field_id = WP_PCE_FIELD_ID_LAST;

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
    
   for(ii=0; ii<num_of_rules_per_filter_set; ii++)
   {
      rule_index = RULES_A_FORWARDING_START + ii;

      status = WP_PceRuleDelete(PCE_rule_handle[rule_index]);
      terminate_on_error(status, "WP_PceRuleDelete",__LINE__);
   }

   for(ii=0; ii<num_of_rules_per_filter_set; ii++)
   {
      rule_index = RULES_C_FORWARDING_START + ii;

      status = WP_PceRuleDelete(PCE_rule_handle[rule_index]);
      terminate_on_error(status, "WP_PceRuleDelete",__LINE__);
   }

   for(ii=0; ii<num_of_rules_per_filter_set; ii++)
   {
      rule_index = RULES_D_FORWARDING_START + ii;

      status = WP_PceRuleDelete(PCE_rule_handle[rule_index]);
      terminate_on_error(status, "WP_PceRuleDelete",__LINE__);
   }

   for(ii=0; ii<num_of_rules_per_filter_set; ii++)
   {
      rule_index = RULES_E_FORWARDING_START + ii;

      status = WP_PceRuleDelete(PCE_rule_handle[rule_index]);
      terminate_on_error(status, "WP_PceRuleDelete",__LINE__);
   }
}

static void WPE_DisableDlPceRules(void)
{
   WP_handle status;
   WP_U32 ii;
   WP_U32 rule_index;

    
   for(ii=0; ii<num_of_rules_per_filter_set; ii++)
   {
      rule_index = RULES_A_FORWARDING_START + ii;

      status = WP_PceRuleDisable(PCE_rule_handle[rule_index]);
      terminate_on_error(status, "WP_PceRuleDisable",__LINE__);
   }

   for(ii=0; ii<num_of_rules_per_filter_set; ii++)
   {
      rule_index = RULES_C_FORWARDING_START + ii;

      status = WP_PceRuleDisable(PCE_rule_handle[rule_index]);
      terminate_on_error(status, "WP_PceRuleDisable",__LINE__);
   }

   for(ii=0; ii<num_of_rules_per_filter_set; ii++)
   {
      rule_index = RULES_D_FORWARDING_START + ii;

      status = WP_PceRuleDisable(PCE_rule_handle[rule_index]);
      terminate_on_error(status, "WP_PceRuleDisable",__LINE__);
   }

   for(ii=0; ii<num_of_rules_per_filter_set; ii++)
   {
      rule_index = RULES_E_FORWARDING_START + ii;

      status = WP_PceRuleDisable(PCE_rule_handle[rule_index]);
      terminate_on_error(status, "WP_PceRuleDisable",__LINE__);
   }
}

static void WPE_EnableDlPceRules(void)
{
   WP_handle status;
   WP_U32 ii;
   WP_U32 rule_index;

    
   for(ii=0; ii<num_of_rules_per_filter_set; ii++)
   {
      rule_index = RULES_A_FORWARDING_START + ii;

      status = WP_PceRuleEnable(PCE_rule_handle[rule_index]);
      terminate_on_error(status, "WP_PceRuleEnable",__LINE__);
   }

   for(ii=0; ii<num_of_rules_per_filter_set; ii++)
   {
      rule_index = RULES_C_FORWARDING_START + ii;

      status = WP_PceRuleEnable(PCE_rule_handle[rule_index]);
      terminate_on_error(status, "WP_PceRuleEnable",__LINE__);
   }

   for(ii=0; ii<num_of_rules_per_filter_set; ii++)
   {
      rule_index = RULES_D_FORWARDING_START + ii;

      status = WP_PceRuleEnable(PCE_rule_handle[rule_index]);
      terminate_on_error(status, "WP_PceRuleEnable",__LINE__);
   }

   for(ii=0; ii<num_of_rules_per_filter_set; ii++)
   {
      rule_index = RULES_E_FORWARDING_START + ii;

      status = WP_PceRuleEnable(PCE_rule_handle[rule_index]);
      terminate_on_error(status, "WP_PceRuleEnable",__LINE__);
   }
}

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

      rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_INT_VLAN_TAG;
      rule_cfg.rule_fields[0].value.vlan_tag = 0x100 + (0x08 * flow_index);

      rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_INT_VLAN_TAG;
      rule_cfg.rule_fields[1].value.vlan_tag = 0x107 + (0x08 * flow_index);

      rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_LAST;

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

      rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_INT_VLAN_TAG;
      rule_cfg.rule_fields[0].value.vlan_tag = 0x110 + (0x10 * (flow_index/NUM_OF_INPUT_PORT_PER_FILTER_SET)) + (0x08 * (flow_index%2));

      rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_INT_VLAN_TAG;
      rule_cfg.rule_fields[1].value.vlan_tag = 0x117 + (0x10 * (flow_index/NUM_OF_INPUT_PORT_PER_FILTER_SET)) + (0x08 * (flow_index%2));

      rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_LAST;

      rule_cfg.match_action = WP_PCE_RULE_MATCH_CONTINUE;

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

   /* Fill the PCE with HW rules until left free only num_of_rules_per_filter_set/2 */
   if (TestType == TEST_HYBRID_FDB && !hybrid_evenly)
      WPE_CreateDummyPceHwRules(num_of_rules_per_filter_set/2);

   /* Create forwarding rules for filter set A */
   for(i=0; i<num_of_rules_per_filter_set; i++)
   {
      rule_fwd.enabled = WP_ENABLE;
      rule_fwd.filter_handle = PCE_filter[FILTER_SET_A_FORWARDING];
      rule_index = RULES_A_FORWARDING_START + i;
      
      rule_fwd.rule_fields[0].field_id = WP_PCE_FIELD_ID_MAC_DA;
      for(ii=0; ii<5; ii++)
         rule_fwd.rule_fields[0].value.mac_addr[ii] = start_mac_addr[ii]; 
      rule_fwd.rule_fields[0].value.mac_addr[3] = (i & 0xffffff) >> 16; 
      rule_fwd.rule_fields[0].value.mac_addr[4] = (i & 0xffff) >> 8; 
      rule_fwd.rule_fields[0].value.mac_addr[5] = i & 0xff; 

      rule_fwd.rule_fields[1].field_id = WP_PCE_FIELD_ID_LAST;
      
      rule_fwd.match_result[0].result_type = WP_PCE_RESULT_OUT_IW_PORT_UPDATE;
      if (filter_set_mode == WT_FORWARDING_ONLY)
         rule_fwd.match_result[0].param.iw_port.iw_port_handle = h_iw_output_port_b; 
      else 
         rule_fwd.match_result[0].param.iw_port.iw_port_handle = h_iw_port_gbe[((i%0x10))/8 ]; 
      
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
   
   if (TestType == TEST_HYBRID_FDB && !hybrid_evenly)
   {
      free_pce_hw_rules = 0;

      WPE_DeleteDummyPceHwRules();

      /* Fill the PCE with HW rules until left free only num_of_rules_per_filter_set/2 */
      WPE_CreateDummyPceHwRules(num_of_rules_per_filter_set/2);
   }
   
   /* Create forwarding rules for filter set C */
   for(i=0; i<num_of_rules_per_filter_set; i++)
   {
      rule_fwd.enabled = WP_ENABLE;
      rule_fwd.filter_handle = PCE_filter[FILTER_SET_C_FORWARDING];
      rule_index = RULES_C_FORWARDING_START + i;
      
      rule_fwd.rule_fields[0].field_id = WP_PCE_FIELD_ID_MAC_DA;
      for(ii=0; ii<5; ii++)
         rule_fwd.rule_fields[0].value.mac_addr[ii] = start_mac_addr[ii]; 
      rule_fwd.rule_fields[0].value.mac_addr[3] = (i & 0xffffff) >> 16; 
      rule_fwd.rule_fields[0].value.mac_addr[4] = (i & 0xffff) >> 8; 
      rule_fwd.rule_fields[0].value.mac_addr[5] = i & 0xff; 
      
      rule_fwd.rule_fields[1].field_id = WP_PCE_FIELD_ID_INT_VLAN_TAG;
      rule_fwd.rule_fields[1].value.vlan_tag = 0x120 + (i % 0x10);

      rule_fwd.rule_fields[2].field_id = WP_PCE_FIELD_ID_LAST;
      
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
   
   if (TestType == TEST_HYBRID_FDB && !hybrid_evenly)
   {
      free_pce_hw_rules = 0;

      WPE_DeleteDummyPceHwRules();

      /* Fill the PCE with HW rules until left free only num_of_rules_per_filter_set/2 */
      WPE_CreateDummyPceHwRules(num_of_rules_per_filter_set/2);
   } 

   /* Create forwarding rules for filter set D */
   for(i=0; i<num_of_rules_per_filter_set; i++)
   {
      rule_fwd.enabled = WP_ENABLE;
      rule_fwd.filter_handle = PCE_filter[FILTER_SET_D_FORWARDING];
      rule_index = RULES_D_FORWARDING_START + i;
      
      rule_fwd.rule_fields[0].field_id = WP_PCE_FIELD_ID_MAC_DA;
      for(ii=0; ii<5; ii++)
         rule_fwd.rule_fields[0].value.mac_addr[ii] = start_mac_addr[ii];
      rule_fwd.rule_fields[0].value.mac_addr[3] = (i & 0xffffff) >> 16; 
      rule_fwd.rule_fields[0].value.mac_addr[4] = (i & 0xffff) >> 8; 
      rule_fwd.rule_fields[0].value.mac_addr[5] = i & 0xff; 
      
      rule_fwd.rule_fields[1].field_id = WP_PCE_FIELD_ID_INT_VLAN_TAG;
      rule_fwd.rule_fields[1].value.vlan_tag = 0x130 + (i % 0x10);
   
      rule_fwd.rule_fields[2].field_id = WP_PCE_FIELD_ID_EXT_VLAN_TAG;
      rule_fwd.rule_fields[2].value.vlan_tag = 0x130 + (i % 0x10);

      rule_fwd.rule_fields[3].field_id = WP_PCE_FIELD_ID_LAST;
      
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
  
   if (TestType == TEST_HYBRID_FDB && !hybrid_evenly)
   {
      free_pce_hw_rules = 0;

      WPE_DeleteDummyPceHwRules();

      /* Fill the PCE with HW rules until left free only num_of_rules_per_filter_set/2 */
      WPE_CreateDummyPceHwRules(num_of_rules_per_filter_set/2);
   } 

   /* Create forwarding rules for filter set E */
   for(i=0; i<num_of_rules_per_filter_set; i++)
   {
      rule_fwd.enabled = WP_ENABLE;
      rule_fwd.filter_handle = PCE_filter[FILTER_SET_E_FORWARDING];
      rule_index = RULES_E_FORWARDING_START + i;
      
      rule_fwd.rule_fields[0].field_id = WP_PCE_FIELD_ID_MAC_DA;
      for(ii=0; ii<5; ii++)
         rule_fwd.rule_fields[0].value.mac_addr[ii] = start_mac_addr[ii];
      rule_fwd.rule_fields[0].value.mac_addr[3] = (i & 0xffffff) >> 16; 
      rule_fwd.rule_fields[0].value.mac_addr[4] = (i & 0xffff) >> 8; 
      rule_fwd.rule_fields[0].value.mac_addr[5] = i & 0xff; 
      
      rule_fwd.rule_fields[1].field_id = WP_PCE_FIELD_ID_INT_VLAN_TAG;
      rule_fwd.rule_fields[1].value.vlan_tag = 0x140 + (i % 0x10);
   
      rule_fwd.rule_fields[2].field_id = WP_PCE_FIELD_ID_IW_SYSTEM;
      rule_fwd.rule_fields[2].value.iw_system_handle = dl_iwsys_bridge[ (i % 0x10)/0x08 ];

      rule_fwd.rule_fields[3].field_id = WP_PCE_FIELD_ID_LAST;
      
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

   if (TestType == TEST_HYBRID_FDB && !hybrid_evenly)
   {
      free_pce_hw_rules = 0;

      WPE_DeleteDummyPceHwRules();
   }
}

static void WPE_CreateDlPceFilterSets(void)
{
   WP_pce_filter_set fs_level0;
   
   fs_level0.filter_set_level = 0;
   fs_level0.next_filter_set = WP_UNUSED;
   fs_level0.filters[0] = PCE_filter[FILTER_SET_A_CLASSIFICATION];
   if (filter_set_mode == WT_FORWARDING_ONLY)
   {
      fs_level0.filters[1] = PCE_filter[FILTER_SET_A_FORWARDING];
      fs_level0.filters[2] = WP_UNUSED;
   }
   else if (filter_set_mode == WT_LEARNING_ONLY)
   {
      fs_level0.filters[1] = PCE_filter[FILTER_SET_A_LEARNING];
      fs_level0.filters[2] = WP_UNUSED;
   }
   else
   {
      fs_level0.filters[1] = PCE_filter[FILTER_SET_A_LEARNING];
      fs_level0.filters[2] = PCE_filter[FILTER_SET_A_FORWARDING];
      fs_level0.filters[3] = WP_UNUSED;
   }
   dl_filter_set[FILTER_SET_A] = WP_PceFilterSetCreate(WP_WINPATH(DEFAULT_WPID), &fs_level0);
   terminate_on_error(dl_filter_set[FILTER_SET_A], "WP_PceFilterSetCreate",__LINE__);


   fs_level0.filter_set_level = 1;
   fs_level0.next_filter_set = WP_UNUSED;
   if (filter_set_mode == WT_FORWARDING_ONLY)
   {
      fs_level0.filters[0] = PCE_filter[FILTER_SET_C_FORWARDING];
      fs_level0.filters[1] = WP_UNUSED;
   }
   else if (filter_set_mode == WT_LEARNING_ONLY)
   {
      fs_level0.filters[0] = PCE_filter[FILTER_SET_C_LEARNING];
      fs_level0.filters[1] = WP_UNUSED;
   }
   else
   {
      fs_level0.filters[0] = PCE_filter[FILTER_SET_C_LEARNING];
      fs_level0.filters[1] = PCE_filter[FILTER_SET_C_FORWARDING];
      fs_level0.filters[2] = WP_UNUSED;
   }
   dl_filter_set[FILTER_SET_C] = WP_PceFilterSetCreate(WP_WINPATH(DEFAULT_WPID), &fs_level0);
   terminate_on_error(dl_filter_set[FILTER_SET_C], "WP_PceFilterSetCreate",__LINE__);


   fs_level0.filter_set_level = 1;
   fs_level0.next_filter_set = WP_UNUSED;
   if (filter_set_mode == WT_FORWARDING_ONLY)
   {
      fs_level0.filters[0] = PCE_filter[FILTER_SET_D_FORWARDING];
      fs_level0.filters[1] = WP_UNUSED;
   }
   else if (filter_set_mode == WT_LEARNING_ONLY)
   {
      fs_level0.filters[0] = PCE_filter[FILTER_SET_D_LEARNING];
      fs_level0.filters[1] = WP_UNUSED;
   }
   else
   {
      fs_level0.filters[0] = PCE_filter[FILTER_SET_D_LEARNING];
      fs_level0.filters[1] = PCE_filter[FILTER_SET_D_FORWARDING];
      fs_level0.filters[2] = WP_UNUSED;
   }
   dl_filter_set[FILTER_SET_D] = WP_PceFilterSetCreate(WP_WINPATH(DEFAULT_WPID), &fs_level0);
   terminate_on_error(dl_filter_set[FILTER_SET_D], "WP_PceFilterSetCreate",__LINE__);

   fs_level0.filter_set_level = 1;
   fs_level0.next_filter_set = WP_UNUSED;
   if (filter_set_mode == WT_FORWARDING_ONLY)
   {
      fs_level0.filters[0] = PCE_filter[FILTER_SET_E_FORWARDING];
      fs_level0.filters[1] = WP_UNUSED;
   }
   else if (filter_set_mode == WT_LEARNING_ONLY)
   {
      fs_level0.filters[0] = PCE_filter[FILTER_SET_E_LEARNING];
      fs_level0.filters[1] = WP_UNUSED;
   }
   else
   {
      fs_level0.filters[0] = PCE_filter[FILTER_SET_E_LEARNING];
      fs_level0.filters[1] = PCE_filter[FILTER_SET_E_FORWARDING];
      fs_level0.filters[2] = WP_UNUSED;
   }
   dl_filter_set[FILTER_SET_E] = WP_PceFilterSetCreate(WP_WINPATH(DEFAULT_WPID), &fs_level0);
   terminate_on_error(dl_filter_set[FILTER_SET_E], "WP_PceFilterSetCreate",__LINE__);
}

static void WPE_CreateDlPceInterface(WP_handle iw_sys)
{
   WP_pce_if_params_pkt_rx_channel pce_if_params = {0};
   WP_handle pce_if_handle;
   WP_status status;

   WPE_CreateDlPceFilters();
   WPE_CreateDlPceFilterSets();

   pce_if_params.mode = WP_PCE_IW_PORT_CONNECTION_ENABLED;
   pce_if_params.parser_start_type = WP_PCE_PARSER_START_TYPE_ETHERNET;
   pce_if_params.filter_set_handle = dl_filter_set[FILTER_SET_A];
   pce_if_params.ip_header_validation = WP_DISABLE;

   pce_if_handle = WP_PceInterfaceCreate(WP_PCE_IF_TYPE_PKT_RX_CHANNEL, &pce_if_params);
   terminate_on_error(pce_if_handle,"WP_PceInterfaceCreate()",__LINE__);

   rx_binding_cfg[0].pce_if_handle = pce_if_handle;
#ifdef WP_BOARD_WDS3_SL
   status = WP_IwRxBindingModify(gbe1_rx_ch_handle,
                                 iw_sys,
                                 qniw,
                                 WP_IW_RX_BIND_MOD_PCE_INTERFACE,
                                 &rx_binding_cfg[0]);
   terminate_on_error(status,"WP_IwRxBindingModify",__LINE__);
#else
   status = WP_IwRxBindingModify(xgi_rx_ch_handle1,
                                 iw_sys,
                                 qniw_hier,
                                 WP_IW_RX_BIND_MOD_PCE_INTERFACE,
                                 &rx_binding_cfg[0]);
   terminate_on_error(status,"WP_IwRxBindingModify",__LINE__);
#endif
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

static void WPE_CreateUlPceFilters(void)
{
   WP_pce_filter_classification filter_class = {0};

   /* Common parameters for all filters*/
   filter_class.no_match_action = WP_PCE_FILTER_NO_MATCH_CONTINUE;
   filter_class.no_fields_action = WP_PCE_FILTER_NO_FIELDS_DENY;

   filter_class.no_match_result[0].result_type = WP_PCE_RESULT_LAST;

   filter_class.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_class.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;
   filter_class.filter_fields[0].field_id = WP_PCE_FIELD_ID_MAC_SA;
   filter_class.filter_fields[1].field_id = WP_PCE_FIELD_ID_LAST;
   PCE_filter[FILTER_SET_UL] = WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_CLASSIFICATION, &filter_class);
   terminate_on_error(PCE_filter[FILTER_SET_UL], "WP_PceFilterCreate",__LINE__);
}

static void WPE_CreateUlPceRules(void)
{
   WP_pce_rule_classification rule_cfg = {0};
   WP_U32  ii;
   WP_U32 rule_index;

   /* Common parameters */
   rule_cfg.enabled = WP_ENABLE;
   
   rule_index = RULES_UL_CLASSIFICATION_START;

   rule_cfg.filter_handle = PCE_filter[FILTER_SET_UL];
   
   rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_MAC_SA;
   for(ii=0; ii<5; ii++)
      rule_cfg.rule_fields[0].value.mac_addr[ii] = start_mac_addr[ii];

   rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_LAST;
   
   rule_cfg.match_action = WP_PCE_RULE_MATCH_CONTINUE;
   
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
   rule_cfg.match_result[0].param.flow_agg.flow_aggregation = ul_flow_agg;
   
   rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;
   
   PCE_rule_handle[rule_index] = WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID),
                                                  WP_PCE_RULE_CLASSIFICATION,
                                                  &rule_cfg);
   terminate_on_error(PCE_rule_handle[rule_index], "WP_PceRuleCreate",__LINE__);
   free_pce_hw_rules--;
}

static void WPE_DeleteUlPceRules(void)
{
   WP_U32 rule_index;
   WP_status status;

   rule_index = RULES_UL_CLASSIFICATION_START;
   status = WP_PceRuleDelete(PCE_rule_handle[rule_index]);
   terminate_on_error(status, "WP_PceRuleDelete",__LINE__);
}

static void WPE_CreateUlPceFilterSets(void)
{
   WP_pce_filter_set fs_level0;
   
   fs_level0.filter_set_level = 0;
   fs_level0.next_filter_set = WP_UNUSED;
   fs_level0.filters[0] = PCE_filter[FILTER_SET_UL];
   fs_level0.filters[1] = WP_UNUSED;
   ul_filter_set = WP_PceFilterSetCreate(WP_WINPATH(DEFAULT_WPID), &fs_level0);
   terminate_on_error(ul_filter_set, "WP_PceFilterSetCreate",__LINE__);
}

static void WPE_CreateUlPceInterface(WP_handle iw_sys)
{
   WP_pce_if_params_pkt_rx_channel pce_if_params = {0};
   WP_handle pce_if_handle;
   WP_status status;

   WPE_CreateUlPceFilters();
   WPE_CreateUlPceRules();
   WPE_CreateUlPceFilterSets();

   pce_if_params.mode = WP_PCE_IW_PORT_CONNECTION_ENABLED;
   pce_if_params.parser_start_type = WP_PCE_PARSER_START_TYPE_ETHERNET;
   pce_if_params.filter_set_handle = ul_filter_set;
   pce_if_params.ip_header_validation = WP_DISABLE;

   pce_if_handle = WP_PceInterfaceCreate(WP_PCE_IF_TYPE_PKT_RX_CHANNEL, &pce_if_params);
   terminate_on_error(pce_if_handle,"WP_PceInterfaceCreate()",__LINE__);

   rx_binding_cfg[0].pce_if_handle = pce_if_handle;
#ifndef WP_BOARD_WDS3_SL
   status = WP_IwRxBindingModify(xgi_rx_ch_handle2,
                                 iw_sys,
                                 qniw_hier,
                                 WP_IW_RX_BIND_MOD_PCE_INTERFACE,
                                 &rx_binding_cfg[0]);
   terminate_on_error(status,"WP_IwRxBindingModify",__LINE__);
#endif
}

static void WPE_DeleteUlPceInterface(void)
{
   WP_status status;
   
   WPE_DeleteUlPceRules();
   
   status = WP_PceFilterSetDelete(ul_filter_set);
   terminate_on_error(status ,"WP_PceFilterSetDelete()",__LINE__);  

   status = WP_PceFilterDelete(PCE_filter[FILTER_SET_UL]);
   terminate_on_error(status ,"WP_PceFilterDelete()",__LINE__);  
}

static void WPE_CreateDlSecondRoundPceFilters(void)
{
   WP_pce_filter_classification filter_class = {0};
   WP_pce_filter_forwarding fwd_filter_cfg = {0};
   WP_pce_filter_learning lrn_filter_cfg = {0};

   /* Common parameters for all filters*/
   filter_class.no_match_action = WP_PCE_FILTER_NO_MATCH_CONTINUE;
   filter_class.no_fields_action = WP_PCE_FILTER_NO_FIELDS_DENY;

   filter_class.no_match_result[0].result_type = WP_PCE_RESULT_LAST;

   filter_class.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_class.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   filter_class.filter_fields[1].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_class.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   filter_class.filter_fields[2].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_class.filter_fields[2].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   /* PCE_filter1 */
   filter_class.filter_fields[0].field_id = WP_PCE_FIELD_ID_OUTPUT_IW_PORT;
   filter_class.filter_fields[1].field_id = WP_PCE_FIELD_ID_LAST;
   PCE_filter[FILTER_SET_B_CLASSIFICATION] = WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_CLASSIFICATION, &filter_class);
   terminate_on_error(PCE_filter[FILTER_SET_B_CLASSIFICATION], "WP_PceFilterCreate",__LINE__);


   fwd_filter_cfg.no_fields_action = WP_PCE_FILTER_NO_FIELDS_DENY;

   fwd_filter_cfg.no_match_result[0].result_type = WP_PCE_RESULT_LAST;

   fwd_filter_cfg.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   fwd_filter_cfg.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   fwd_filter_cfg.filter_fields[1].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   fwd_filter_cfg.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_USED;
   fwd_filter_cfg.filter_fields[1].mask.vlan_tag = WP_PCE_FIELD_MASK_VLAN_ID;

   fwd_filter_cfg.filter_fields[2].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   fwd_filter_cfg.filter_fields[2].mask_mode = WP_PCE_FIELD_MASK_USED;
   fwd_filter_cfg.filter_fields[2].mask.vlan_tag = WP_PCE_FIELD_MASK_VLAN_ID;

   fwd_filter_cfg.filter_fields[3].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   fwd_filter_cfg.filter_fields[3].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   fwd_filter_cfg.filter_fields[0].field_id   = WP_PCE_FIELD_ID_MAC_DA;
   fwd_filter_cfg.filter_fields[1].field_id = WP_PCE_FIELD_ID_LAST;

   fwd_filter_cfg.no_match_action = WP_PCE_FILTER_NO_MATCH_CONTINUE;
   PCE_filter[FILTER_SET_B_FORWARDING] = WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_FORWARDING, &fwd_filter_cfg);
   terminate_on_error(PCE_filter[FILTER_SET_B_FORWARDING], "WP_PceFilterCreate",__LINE__);


   lrn_filter_cfg.forwarding_filter = PCE_filter[FILTER_SET_B_FORWARDING];
   lrn_filter_cfg.no_fields_action = WP_PCE_FILTER_NO_FIELDS_DENY;

   lrn_filter_cfg.no_match_result[0].result_type = WP_PCE_RESULT_LAST;

   lrn_filter_cfg.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   lrn_filter_cfg.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   lrn_filter_cfg.filter_fields[1].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   lrn_filter_cfg.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_USED;
   lrn_filter_cfg.filter_fields[1].mask.vlan_tag = WP_PCE_FIELD_MASK_VLAN_ID;

   lrn_filter_cfg.filter_fields[2].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   lrn_filter_cfg.filter_fields[2].mask_mode = WP_PCE_FIELD_MASK_USED;
   lrn_filter_cfg.filter_fields[2].mask.vlan_tag = WP_PCE_FIELD_MASK_VLAN_ID;

   lrn_filter_cfg.filter_fields[3].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   lrn_filter_cfg.filter_fields[3].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   lrn_filter_cfg.filter_fields[0].field_id   = WP_PCE_FIELD_ID_MAC_SA;
   lrn_filter_cfg.filter_fields[1].field_id = WP_PCE_FIELD_ID_LAST;

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

   for(ii=0; ii<num_of_rules_per_filter_set; ii++)
   {
      rule_index = RULES_B_FORWARDING_START + ii;

      status = WP_PceRuleDelete(PCE_rule_handle[rule_index]);
      terminate_on_error(status, "WP_PceRuleDelete",__LINE__);
   }
}

static void WPE_DisableDlSecondRoundPceRules(void)
{
   WP_handle status;
   WP_U32 rule_index, ii;

   for(ii=0; ii<num_of_rules_per_filter_set; ii++)
   {
      rule_index = RULES_B_FORWARDING_START + ii;

      status = WP_PceRuleDisable(PCE_rule_handle[rule_index]);
      terminate_on_error(status, "WP_PceRuleDisable",__LINE__);
   }
}

static void WPE_EnableDlSecondRoundPceRules(void)
{
   WP_handle status;
   WP_U32 rule_index, ii;

   for(ii=0; ii<num_of_rules_per_filter_set; ii++)
   {
      rule_index = RULES_B_FORWARDING_START + ii;

      status = WP_PceRuleEnable(PCE_rule_handle[rule_index]);
      terminate_on_error(status, "WP_PceRuleEnable",__LINE__);
   }
}

static void WPE_CreateDlSecondRoundPceRules(void)
{
   WP_pce_rule_classification rule_cfg = {0};
   WP_pce_rule_forwarding rule_fwd = {0};
   WP_U32 rule_index, ii, i;

   /* Common parameters */
   rule_cfg.enabled = WP_ENABLE;

   rule_index = RULES_B_CLASSIFICATION_START;
   rule_cfg.filter_handle = PCE_filter[FILTER_SET_B_CLASSIFICATION];
   
   rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_OUTPUT_IW_PORT;
   rule_cfg.rule_fields[0].value.iw_port_handle = h_iw_output_port_b;
   
   rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_LAST;
   
   rule_cfg.match_action = WP_PCE_RULE_MATCH_ACCEPT;
   
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
   rule_cfg.match_result[0].param.flow_agg.flow_aggregation = flow_agg[FILTER_SET_B_CLASSIFICATION_FLOW_AGG];
   
   rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;
   
   PCE_rule_handle[rule_index] = WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID),
                                                  WP_PCE_RULE_CLASSIFICATION,
                                                  &rule_cfg);
   terminate_on_error(PCE_rule_handle[rule_index], "WP_PceRuleCreate",__LINE__);

   free_pce_hw_rules--;

   /* Fill the PCE with HW rules until left free only num_of_rules_per_filter_set/2 */
   if (TestType == TEST_HYBRID_FDB && !hybrid_evenly)
      WPE_CreateDummyPceHwRules(num_of_rules_per_filter_set/2);
   
   /* Create forwarding rules for filter set B */
   for(i=0; i<num_of_rules_per_filter_set; i++)
   {
      rule_fwd.enabled = WP_ENABLE;
      rule_fwd.filter_handle = PCE_filter[FILTER_SET_B_FORWARDING];
      rule_index = RULES_B_FORWARDING_START + i;
      
      rule_fwd.rule_fields[0].field_id = WP_PCE_FIELD_ID_MAC_DA;
      for(ii=0; ii<5; ii++)
         rule_fwd.rule_fields[0].value.mac_addr[ii] = start_mac_addr[ii]; 
      rule_fwd.rule_fields[0].value.mac_addr[3] = (i & 0xffffff) >> 16; 
      rule_fwd.rule_fields[0].value.mac_addr[4] = (i & 0xffff) >> 8; 
      rule_fwd.rule_fields[0].value.mac_addr[5] = i & 0xff; 
      
      rule_fwd.rule_fields[1].field_id = WP_PCE_FIELD_ID_LAST;
      
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

   if (TestType == TEST_HYBRID_FDB && !hybrid_evenly)
   {
      free_pce_hw_rules = 0;
      
      WPE_DeleteDummyPceHwRules();
   }
}

static void WPE_CreateDlSecondRoundPceFilterSets(void)
{
   WP_pce_filter_set fs_level0;

   fs_level0.filter_set_level = 1;
   fs_level0.next_filter_set = WP_UNUSED;
   fs_level0.filters[0] = PCE_filter[FILTER_SET_B_CLASSIFICATION];
   if (filter_set_mode == WT_FORWARDING_ONLY)
   {
      fs_level0.filters[1] = PCE_filter[FILTER_SET_B_FORWARDING];
      fs_level0.filters[2] = WP_UNUSED;
   }
   else if (filter_set_mode == WT_LEARNING_ONLY)
   {
      fs_level0.filters[1] = PCE_filter[FILTER_SET_B_LEARNING];
      fs_level0.filters[2] = WP_UNUSED;
   }
   else
   {
      fs_level0.filters[1] = PCE_filter[FILTER_SET_B_LEARNING];
      fs_level0.filters[2] = PCE_filter[FILTER_SET_B_FORWARDING];
      fs_level0.filters[3] = WP_UNUSED;
   }
   dl_filter_set[FILTER_SET_B] = WP_PceFilterSetCreate(WP_WINPATH(DEFAULT_WPID), &fs_level0);
   terminate_on_error(dl_filter_set[FILTER_SET_B], "WP_PceFilterSetCreate",__LINE__);
}

static void WPE_CreateDlSecondRoundPceInterface()
{
   WP_pce_if_params_iw_next_round pce_if_params = {0};

   WPE_CreateDlSecondRoundPceFilters();
   WPE_CreateDlSecondRoundPceFilterSets();

   pce_if_params.mode = WP_PCE_IW_PORT_CONNECTION_ENABLED;
   pce_if_params.parsing_ref_point = WP_PCE_PARSER_REF_POINT_FRAME_START;
   pce_if_params.parser_start_type = WP_PCE_PARSER_START_TYPE_ETHERNET;
   pce_if_params.parsing_ref_offset = 0;
   pce_if_params.filter_set_handle = dl_filter_set[FILTER_SET_B];
   pce_if_params.ip_header_validation = WP_DISABLE;
   pce_if_params.characteristics = WP_PCE_IF_OUTPUT_PORT_PROPAGATION_ENABLE;

   if (TestType == TEST_HYBRID_FDB)
   {
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

void WPE_CreateDummyPceHwRules(WP_S32 rules_needed_count)
{
   WP_U32 rule_index;
   WP_pce_rule_classification rule_cfg = {0};


   if (free_pce_hw_rules > rules_needed_count)
      dummy_rules_count = free_pce_hw_rules - rules_needed_count;
   else
      dummy_rules_count = free_pce_hw_rules / 2;

   if (dummy_rules_count > 0xdff)
      dummy_rules_count = 0xdff;
   
   free_pce_hw_rules = free_pce_hw_rules - dummy_rules_count;

   for(rule_index = 0; rule_index<dummy_rules_count; rule_index++)
   {
      rule_cfg.filter_handle = PCE_filter[FILTER_SET_A_CLASSIFICATION];

      rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_INT_VLAN_TAG;
      rule_cfg.rule_fields[0].value.vlan_tag = 0x200 + rule_index;

      rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_INT_VLAN_TAG;
      rule_cfg.rule_fields[1].value.vlan_tag = 0x200 + rule_index;

      rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_LAST;

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

   free_pce_hw_rules = free_pce_hw_rules + dummy_rules_count;

   dummy_rules_count = 0;
}

void WPT_IwBridgingPortStatsReset(WP_handle port_handle)
{
   WP_bport_stats bport_stats;
   WP_status status;

   if (!port_handle)
      return;
   
   status = WP_IwPortStatisticsReset(port_handle, &bport_stats);
   terminate_on_error(status , "WP_IwPortStatisticsReset()",__LINE__);
}

void WPT_GenericFlowStatisticsReset(WP_handle agg)
{
   WP_iw_flow_stats iw_flow_stats;
   WP_status status;

   status = WP_IwFlowStatisticsReset(agg, WP_IW_FLOW_STAT_GENERIC, &iw_flow_stats);
   terminate_on_error(status , "WP_IwFlowStatisticsReset()",__LINE__);
}

void WPT_CompatibleFlowStatisticsReset(WP_handle agg)
{
   WP_iw_flow_stats iw_flow_stats;
   WP_status status;

   status = WP_IwFlowStatisticsReset(agg, WP_IW_FLOW_STAT, &iw_flow_stats);
   terminate_on_error(status , "WP_IwFlowStatisticsReset()",__LINE__);
}

/* WPE_pecs_init */
static void WPE_Pecs_Init(WP_gpe_pecs gpe_pecs_cfg[],WP_handle pecs_handles[])
{
   WP_U32 ii;
   
   for (ii = 0 ; ii < NUM_OF_PECS ; ii++)
   {
      pecs_handles[ii] = WP_GpePecsCreate(WP_WINPATH(DEFAULT_WPID),WP_GPE_PECS,(void*)&gpe_pecs_cfg[ii]);
      printf("CREATING PECS: handle returned: %x\n", pecs_handles[ii]);
      terminate_on_error(pecs_handles[ii],"WP_GpePecsCreate", __LINE__);  
   }
}

WP_U32 WT_TimeRead(void)
{
#ifdef __linux__
   return WP_TimeRead();
#else   
   WP_U32 i = 0;
   __asm volatile ("mfc0 %0, $9; nop" : "=r" (i));
   return i;
#endif   
}

WP_U32 WT_TimeDelta(WP_U32 later,WP_U32 earlier)
{
#ifdef __linux__
   return WP_TimeDelta(later,earlier);
#else   

   WP_U32 delta = later - earlier;
   /*delta  is in mips frequency clocks divided by 2*/
   return  (delta/(WT_SCH_TIMESTAMP/2));
#endif
}

#include "wt_util.c"
#include "wt_partition_ecc_util.c"
