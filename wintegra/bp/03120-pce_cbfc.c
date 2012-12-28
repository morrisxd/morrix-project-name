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
// #include "wt_util.h"
#include "wt_partition_ecc_util.h"
#include "wp_sim.h"

// #include "wpi_veneer.h"
#include "wpx_app_data.h"
#include "wpx_oc12_pmc5357.h"
#include "wpx_gei_phy.h"
#include "wpx_pin_mux_selector.h"
// #include "app_convert.h"

#include "wpx_enet_phy.h"

#ifndef WPI_REGISTRY_H
#include "include/core/registry/wpi_registry.h"
#else
#error REGISTRY_should_not_defined
#endif

#include "api/wp_pce.h"
//#include "wt_gcfi_misc.h"

#define WT_TRIAL_MASK	0x4
#define DEFAULT_WPID	(0)
#define ADDED_BY_MORRIS
#define SINGLE_IMAGE 1
extern WP_U32 dps_ProgramImage[];
extern WP_U16 dps_PC_Table_Init[];
extern WP_U32 dps_ProgramImage_WinFarm_0[], dps_ProgramImage_WinFarm_1[];
extern WP_U16 dps_PC_Table_Init_WinFarm_0[], dps_PC_Table_Init_WinFarm_1[];

WP_CHAR appname[] = "CBFC test";
static WP_U32 num_of_packets = 0;

#define DEBUG 0
#define PCE_DEBUG 1

#define MAX_LPM_FLOWS 5000
#define WT_IPV4_HEADER_SIZE   20
#define WT_L4_HEADER_SIZE     8
#define WT_MAC_HEADER_SIZE    14
#define WT_IPV4_SA_OFFSET     12
#define WT_IPV4_DA_OFFSET     16
#define WT_IPV4_PROTOCOL_OFFSET 9
#define WT_L4_DST_PORT_OFFSET 2
#define WT_L4_SRC_PORT_OFFSET 0
#define WT_MAC_DA_OFFSET      0
#define WT_MAC_SA_OFFSET      6

/*-----------------------------------------------------------*\
\*-----------------------------------------------------------*/
#define TEST_LOOPBACK_MODE 1
#define WTI_REGRESSION_MODE 1

#define NUM_DATA_FILTERS 1
#define NUM_FLOW_CTRL_FILTERS 8

#define WT_RX_LOOP_LIMIT 64

#define MAX_IW_POLICERS 50
#define MAX_FLOW_AGG 60
#define POOL_LIMIT 20
#define MAX_IW_PROGRAMMABLE_FILTERS 5

#define NUM_OF_FLOWS 8

#define NUM_OF_CHANNELS NUM_OF_FLOWS
#define NUM_PQ_BLOCK 1

#define MAX_NUM_OF_CHANNELS NUM_OF_CHANNELS
#define NUM_OF_PACKETS_PER_CHANNEL  1
#define MAX_NUM_OF_PACKETS (MAX_NUM_OF_CHANNELS/*8*/ * NUM_OF_PACKETS_PER_CHANNEL/*1*/)

/* IWF <Src IP, Dest IP, Src Port, Dst Port, IPv4 protocol> -->
       <Src IP, Dst IP, Src Port, Dst Port, IPv4 protocol, MAC Dst last byte> */
static WP_U32 IWF_Table[NUM_OF_FLOWS][11] = {
   {0xC0A80101, 0xC0A8010A, 200, 200, 0x11, 0x0A010101, 0x0A01010A,
    100,
    100, 0x11, 0x06},
   {0xC0A80102, 0xC0A8010A, 200, 200, 0x11, 0x0A010102, 0x0A01010A,
    100,
    100, 0x11, 0x06},
   {0xC0A80103, 0xC0A8010A, 200, 200, 0x11, 0x0A010103, 0x0A01010A,
    100,
    100, 0x11, 0x06},
   {0xC0A80104, 0xC0A8010A, 200, 200, 0x11, 0x0A010104, 0x0A01010A,
    100,
    100, 0x11, 0x06},
   {0xC0A80105, 0xC0A8010A, 200, 200, 0x11, 0x0A010105, 0x0A01010A,
    100,
    100, 0x11, 0x06},
   {0xC0A80106, 0xC0A8010A, 200, 200, 0x11, 0x0A010106, 0x0A01010A,
    100,
    100, 0x11, 0x06},
   {0xC0A80107, 0xC0A8010A, 200, 200, 0x11, 0x0A010107, 0x0A01010A,
    100,
    100, 0x11, 0x06},
   {0xC0A80108, 0xC0A8010A, 200, 200, 0x11, 0x0A010108, 0x0A01010A,
    100,
    100, 0x11, 0x06}
#ifndef ADDED_BY_MORRIS
   {0xC0A8010E, 0xC0A8010A, 200, 200, 0x11, 0x0A01010E, 0x0A01010A,
    100,
    100, 0x11, 0x06},
   {0xC0A8010F, 0xC0A8010A, 200, 200, 0x11, 0x0A01010F, 0x0A01010A,
    100,
    100, 0x11, 0x06}
#endif
};

#define WT_MAX_FRAME_SIZE         (1536 + 2*512)

#define MAX_DATA_LENGTH   1024
#define APP_HOST_BUFFER_SIZE 1984

static WP_U8 logTraceLevel = WPI_DEBUG_LEVEL_10;
void App_TranslateAsciiToHex(WP_CHAR *Hex,WP_CHAR *Ascii,WP_U32 length);



/****************************************************************************************************************************
* Function name: App_Quit()
* Description  :      
 *
*
* Input  params: none
* Output params: 
 * Return val   : none
*****************************************************************************************************************************/
#define STRINGIFY(x) STRINGIFY_(x)
#define STRINGIFY_(x) #x


void WT_Start(WP_CHAR *app_name,WP_CHAR *trial_name,WP_CHAR *file_name)
{
#if WT_DEBUG_CALLBACK
   WP_ControlRegister(WP_DEBUG_CALLBACK_FILE_LINE_ERROR, WT_DebugCallback);
#endif
   printf("Test Name " STRINGIFY(WDDI_TEST) "\n");
   printf("Test Started %s", app_name);
   if (trial_name)
      printf(" %s", trial_name);
   printf("\nTest Built " __DATE__ " " __TIME__ "\n");
   printf("Test Options " STRINGIFY(TEST_OPTIONS) "\n");
   printf("Test More Options " STRINGIFY(TEST_MORE_OPTIONS) "\n");
}

void WT_Reboot(void)
{
#if !defined(__linux__)
/* 
 * Rebooting is specific to board and target.  The test
 * directory should not be concerned how this is done.
 * Instead, it calls a board-specific function. 
 */
   if (WPI_REBOOT_ENABLE) {
      printf("Test Rebooting winmon by WT_Reboot\nWinMon>\n");
      WPX_Reboot();
   }
   else
      printf("Not Rebooting with WT_Reboot\nWinMon>\n");
#endif
}





typedef union StatField
{
   WP_U64 field;
   struct
   {
      WP_U32 high;
      WP_U32 low;
   } part;
} WPT_StatField;

WP_pce_parser_info pce_parser_info;

WP_module_pce_init pce_init = {
   &pce_parser_info,            /* parser_info */
   150,                         /* hardware_rules_num */
   15,                          /* filters_num */
   10,                          /* pkt_rx_channel_interfaces_num */
   5,                           /* iw_tx_channel_interfaces_num */
   WP_PCE_DISCARD_MODE_ALL,     /* pce_discard_mode */
   WP_ENABLE,                   /* bridge_loop_prevention_mode */
   WP_ENABLE,                   /* learning_mode */
   {                            /* learning_queue_config */
    30,                         /* learning_queue_size */
    WP_LEARNING_POLLING_MODE,   /* interrupt_mode */
    WP_IW_IRQT0,                /* int_queue_num */
    30,                         /* interrupt_rate */
    }
};

WP_int_queue_table int_queue_tables =
   { {{24, 1}, {10, 1}, {10, 1}, {10, 1}} };

WP_fmu_shaping_cir_eir l1_group_shaping_params[1];
WP_fmu_shaping_cir_eir l2_group_shaping_params[1];
WP_shaping_group_type_enet packet_group_l1_config[1];
WP_shaping_group_type_enet packet_group_l2_config[1];

WP_iw_global max_iw[1] = {
   {
    /* max_iw_aggregations */ MAX_FLOW_AGG,
    /* max_r_iw_systems; */ 3,
    /* max_directmap_enet_systems; */ 0,
    /* max_b_iw_systems; */ 2,
    /* max_m_iw_systems; */ 0,
    /* max_vlan_priority_maps; */ 0,
    /* iw_bkgnd_fifo_size */ 0,
    /* cong_pt       */ NULL,
    /* iw_host_limits */ {64, 0, 0, 64, 0},
    /* mpls_config */ {0, 0},
    /* iw_modes; */
    {
     /*policer_mode; */ WP_IW_FLOW_POLICER,
     /*statistics_bundle_stat_mode; */
     WP_IW_STATISTICS_BUNDLE_STAT_DISABLE,
     /*l2_prefix_ext_mode; */ WP_IW_L2_HEADER_EXTENSION_ENABLE,
     /*enhanced_flow_stat_mode; */ WP_IW_ENHANCED_FLOW_STAT_DISABLE,
     /*flow_stat_mode; */ WP_IW_FLOW_STAT_ENABLE,
     /*fr_tx_flow_stat_mode */ WP_IW_FR_TX_FLOW_STAT_DISABLE,
     /*mfc_alloc_mode; */ WP_IW_MFC_ALLOC_ENABLE,
     /*Learning_queue_mode */ WP_IW_LEARNING_DISABLE,
     /*port_filtering_mode */ WP_IW_PORT_FILTERING_DISABLE,
     /* lpm_alloc_mode */ WP_IW_LPM_ALLOC_DISABLE,
     /*enhanced_mc_member_stat_mode */ WP_DISABLE,
     /*routing_alloc_modes */ WP_DISABLE,
     /*res_mc_addresses_table_mode */ WP_DISABLE,
     /*port_stag_etype_mode */ WP_IW_PORT_STAG_ETYPE_ENABLE
     },
    /* max_ch_agg_stat; */ 0,
    /* max_fr_s_iw_systems; */ 0,
    /*max_iw_ports; */ 100,
    /* max_iw_mc_groups; */ 0,
    /* max_nat_systems; */ 0,
    /*max_iw_second_aggregations; */ 0,
    /* secondary flow aggregations */
    /*iw_comp_limits; */ 0,
    /* max_iw_mc_members; */ 0,
    /* max_iw_programmable_filters; */ MAX_IW_PROGRAMMABLE_FILTERS,
    /* max_iw_policers; */ MAX_IW_POLICERS,
    /* max_dscpmt */ 0,
    /* max_wred_bindings */ 0,
    /* tcm_policer_limits */
    {
     MAX_IW_POLICERS,
     MAX_IW_POLICERS}

    }
};

#ifndef WP_BOARD_WDS3_SL
WP_context context[1] = {
   {
    3,                          /* qnodes */
    /* the first four assignments are mandatory bus assignments */
    {
     /*WP_VB_PARAM = 0,           */ {WP_BUS_PARAM, APP_BANK_PARAM},
     /*WP_VB_PACKET,          */ {WP_BUS_PACKET, APP_BANK_PACKET},
     /*WP_VB_INTERNAL,            */ {WP_BUS_INTERNAL,
                                      APP_BANK_INTERNAL},
     /*WP_VB_HOST,                */ {WP_BUS_HOST, APP_BANK_HOST},
     /*WP_VB_INTQUEUE_DEBUG,      */ {WP_BUS_HOST, APP_BANK_HOST},
     /*WP_VB_TIMESTAMP,           */ {WP_BUS_HOST, APP_BANK_HOST},
     /*WP_VB_CAS,                 */ {WP_BUS_NONE, 0},
     /*WP_VB_TRANS_TSAT,          */ {WP_BUS_NONE, 0},
     /*WP_VB_TPT,                 */ {WP_BUS_INTERNAL,
                                      APP_BANK_INTERNAL},
     /*WP_VB_CW,              */ {WP_BUS_INTERNAL, APP_BANK_INTERNAL},
     /*WP_VB_ASU_TXQUEUE,             */ {WP_BUS_PARAM,
                                          APP_BANK_PARAM},
     /*WP_VB_STATISTICS,          */ {WP_BUS_PACKET, APP_BANK_PACKET},
     /*WP_VB_ATM_ADDLOOKUP,       */ {WP_BUS_PARAM, APP_BANK_PARAM},
     /*WP_VB_ATM_CPT_EBMT_EXTHEADER,        */ {WP_BUS_PARAM,
                                                APP_BANK_PARAM},
     /*WP_VB_ETH_HDLC_CPT,            */ {WP_BUS_PARAM,
                                          APP_BANK_PARAM},
     /*WP_VB_FBP_BD,          */ {WP_BUS_PARAM, APP_BANK_PARAM},
     /*WP_VB_FBP_DATA,            */ {WP_BUS_PACKET, APP_BANK_PACKET},
     /*WP_VB_L2_BKGND,            */ {WP_BUS_PACKET, APP_BANK_PACKET},
     /*WP_VB_BRIDGE_LEARNING_DFC_PFMT       */ {WP_BUS_PACKET,
                                                APP_BANK_PACKET},
     /*WP_VB_IMA_MLPPP_STATE,         */ {WP_BUS_INTERNAL,
                                          APP_BANK_INTERNAL},
     /*WP_VB_IMA_MLPPP_BUFFER,        */ {WP_BUS_HOST, APP_BANK_HOST},
     /*WP_VB_HASH_IW_LPM_EMC,         */ {WP_BUS_PACKET,
                                          APP_BANK_PACKET},
     /*WP_VB_IW_MFC_RULES,            */ {WP_BUS_PACKET,
                                          APP_BANK_PACKET},
     /*WP_VB_IW_FIWT_QAT,             */ {WP_BUS_PARAM,
                                          APP_BANK_PARAM},
     /*WP_VB_IW_MFCPT_DIFFSERV_UPPT_D       */ {WP_BUS_INTERNAL,
                                                APP_BANK_INTERNAL},
     /*WP_VB_STATISTICS2,             */ {WP_BUS_PARAM,
                                          APP_BANK_PARAM},
     /*WP_VB_STATISTICS3,             */ {WP_BUS_PARAM,
                                          APP_BANK_PARAM},
     /*WP_VB_UNASSIGNED6,             */ {WP_BUS_NONE, 0},
     /*WP_VB_PKT_GLOBAL_TCPT,         */ {WP_BUS_NONE, 0},
     /*WP_VB_UNASSIGNED8,             */ {WP_BUS_NONE, 0},
     /*WP_VB_UNASSIGNED9,             */ {WP_BUS_NONE, 0},
     /*WP_VB_UNASSIGNED10,            */ {WP_BUS_NONE, 0},
     /*WP_VB_MCH_DATA,            */ {WP_BUS_NONE, 0},
     /*WP_VB_OAM_FM_QUERY,                */ {WP_BUS_NONE, 0},
     /*WP_VB_HASH_ON_HEAP,                */ {WP_BUS_NONE, 0},
     /*WP_VB_MFC_INTERNAL,                    */ {WP_BUS_PARAM,
                                                  APP_BANK_PARAM},
     /*WP_VB_MFR_RAS_TBL,                   */ {WP_BUS_NONE, 0},
     /*WP_VB_CESOP_DATA,                    */ {WP_BUS_NONE, 0},
     /*WP_VB_SECURITY_ESE,                  */ {WP_BUS_NONE, 0},
     /*WP_VB_PDCP_SECURITY_ENTITY,          */ {WP_BUS_NONE, 0},
     /*WP_VB_POLICER,                       */ {WP_BUS_PARAM,
                                                APP_BANK_PARAM},
     /*WP_VB_PCE_LRN,                       */ {WP_BUS_INTERNAL,
                                                APP_BANK_INTERNAL},
     /*WP_VB_PCEA_PARAM,                    */ {WP_BUS_INTERNAL,
                                                APP_BANK_INTERNAL},
     /*WP_VB_CONGESTION_TABLE,              */ {WP_BUS_NONE, 0},
     /*WP_VB_PARAMS_PROPAGATION             */ {WP_BUS_NONE, 0},
     /*WP_VB_STATS_IWPORT_PCE_FILTERS       */ {WP_BUS_INTERNAL,
                                                APP_BANK_INTERNAL},
#if _WP_MODULE_CUSTOM_TIGERMILK_ENABLE_
     /*WP_VB_PROFILE_MARKING_STATS          */ {WP_BUS_NONE, 0},
#endif
     /*WP_VB_TIMESTAMP_SERVICES_TABLE       */ {WP_BUS_NONE, 0},
     /*WP_VB_POLICER_ACTION                 */ {WP_BUS_PARAM,
                                                APP_BANK_PARAM},
     /*WP_VB_G999_STREAM                    */ {WP_BUS_INTERNAL,
                                                APP_BANK_INTERNAL},
     /*WP_VB_G999_STREAM_STATISTICS         */ {WP_BUS_INTERNAL,
                                                APP_BANK_INTERNAL},
     },
    {
     dps_ProgramImage, dps_ProgramImage},
    {
     dps_PC_Table_Init, dps_PC_Table_Init},
    &int_queue_tables,          /* Interrupt queue tables */
    2,                          /* Mixumum number of pools */

    NULL,                       /* ATM global parameters */
    max_iw,                     /* Interworking global parameters */
    }
};
#else
#error SL_should_not_defined
WP_context context[1] = {
   {
    3,                          /* qnodes */
    /* the first four assignments are mandatory bus assignments */
    {
     { /* WP_BUS_PARAM    */ WP_BUS_PARAM, APP_BANK_PARAM},
     { /* WP_BUS_PACKET   */ WP_BUS_PACKET, APP_BANK_PACKET},
     { /* WP_BUS_INTERNAL */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
     { /* WP_BUS_HOST     */ WP_BUS_HOST, APP_BANK_HOST},
     { /* WP_BUS_IRQ      */ WP_BUS_PARAM, APP_BANK_PARAM},
     { /* WP_BUS_TMSTMP   */ WP_BUS_HOST, APP_BANK_HOST},
     { /* UNASSIGNED      */ WP_BUS_NONE, 0},
     { /* UNASSIGNED      */ WP_BUS_NONE, 0},
     { /* ASU_TPT         */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
     { /* ASU CW          */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
     { /* ASU TX QUEUE    */ WP_BUS_PARAM, APP_BANK_PARAM},
     { /* STATISTICS      */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
     { /* ADDRESS LOOKUP  */ WP_BUS_PARAM, APP_BANK_PARAM},
     { /* ATM CPT         */ WP_BUS_PARAM, APP_BANK_PARAM},
     { /* ENET HDLC CPT   */ WP_BUS_PARAM, APP_BANK_PARAM},
     { /* IW FBP PARAM    */ WP_BUS_PARAM, APP_BANK_PARAM},
     { /* IW FBP DATA     */ WP_BUS_HOST, APP_BANK_HOST},
     { /* EXTERNAL FIFO   */ WP_BUS_HOST, APP_BANK_HOST},
     { /* MAC LOOKUP      */ WP_BUS_PARAM, APP_BANK_PARAM},
     { /* IMA SMALL       */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
     { /* IMA LARGE       */ WP_BUS_PARAM, APP_BANK_PARAM},
     { /* LPM             */ WP_BUS_HOST, APP_BANK_HOST},
     { /* MFC RULES       */ WP_BUS_PARAM, APP_BANK_PARAM},
     { /* FIWT, QAT       */ WP_BUS_PARAM, APP_BANK_PARAM},
     { /* MFCPT, UPPPT, DS */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
     { /* STATISTICS2      */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
     { /* STATISTICS3      */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
     { /* UNASSIGNED      */ WP_BUS_NONE, 0},
     { /* PKT_GLOBAL_TCPT */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
     { /* UNASSIGNED      */ WP_BUS_NONE, 0},
     { /* MANDATORY       */ WP_BUS_NONE, 0},
     { /* MANDATORY       */ WP_BUS_NONE, 0},
     /* ************* VIRTUAL BUS ASSIGNMENT ************** */
     { /* MCH_DATA        */ WP_BUS_HOST, APP_BANK_HOST}
     ,
     { /* WP_VB_OAM_FM_QUERY */ WP_BUS_NONE, 0},
     { /* WP_VB_HASH_ON_HEAP */ WP_BUS_NONE, 0},
     { /* WP_VB_MFC_INTERNAL */ WP_BUS_HOST, APP_BANK_HOST},
     { /* WP_VB_MFR_RAS_TBL */ WP_BUS_NONE, 0},
     { /* WP_VB_CESOP_DATA */ WP_BUS_NONE, 0},
     { /* WP_VB_SECURITY_ESE */ WP_BUS_NONE, 0},
     { /* WP_VB_PDCP_SECURITY_ENTITY */ WP_BUS_NONE, 0},
     { /* WP_VB_POLICER */ WP_BUS_PARAM, APP_BANK_PARAM},
     { /* WP_VB_PCE_LRN */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
     { /* WP_VB_PCEA_PARAM */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
     { /* WP_VB_CONGESTION_TABLE */ WP_BUS_NONE, 0},
     { /* WP_VB_PARAMS_PROPAGATION */ WP_BUS_NONE, 0},
     { /* WP_VB_STATS_IWPORT_PCE_FILTERS */ WP_BUS_INTERNAL,
      APP_BANK_INTERNAL},
#if _WP_MODULE_CUSTOM_TIGERMILK_ENABLE_
     { /* WP_VB_PROFILE_MARKING_STATS */ WP_BUS_NONE, 0},
#endif
     { /* WP_VB_TIMESTAMP_SERVICES_TABLE */ WP_BUS_NONE, 0},
     { /* WP_VB_POLICER_ACTION */ WP_BUS_PARAM, APP_BANK_PARAM},
     { /* WP_VB_G999_STREAM */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
     { /* WP_VB_G999_STREAM_STATISTICS */ WP_BUS_INTERNAL,
      APP_BANK_INTERNAL},
     },
    {
     dps_ProgramImage, dps_ProgramImage},
    {
     dps_PC_Table_Init, dps_PC_Table_Init},
    &int_queue_tables,          /* Interrupt queue tables */
    2,                          /* Mixumum number of pools */

    NULL,                       /* ATM global parameters */
    max_iw,                     /* Interworking global parameters */
    }
};
#endif //WP_BOARD_WDS3_SL

/* Callback function for a queuenode - Need to define the correct type later*/
WP_S32 QNCallback (WP_S32 status)
{
   printf ("App: QNCallback: Status returned is %d\n", status);
   return 0;
}

WP_pool_buffer_data buffer_data_2048[] = {
   {
    /* n_buffers */ 1024,
    /* offset */ 64,
    /* size */ 1984,
    /* RBS */
    /* pad */ 0,
#ifndef WP_BOARD_WDS3_SL
    /* bus */ WP_BUS_PACKET,
    /* bank */ APP_BANK_PACKET
#else
    /* bus */ WP_BUS_HOST,
    /* bank */ APP_BANK_HOST
#endif //WP_BOARD_WDS3_SL
    }
};

WP_pool_buffer_data buffer_data_144[1] = {
   {
    /* n_buffers */ 32,
    /* offset */ 0,
    /* size */ 1984,
    /* pad */ 0,
#ifndef WP_BOARD_WDS3_SL
    /* bus */ WP_BUS_PACKET,
    /* bank */ APP_BANK_PACKET
#else
    /* bus */ WP_BUS_PARAM,
    /* bank */ APP_BANK_PARAM
#endif //WP_BOARD_WDS3_SL
    }
};

WP_ch_enet gbe_ch[2] = {
   {
    /* intmode */ WP_PKTCH_INT_DISABLE,
    /* iwmode */ WP_PKTCH_IWM_ENABLE,
    /* testmode */ WP_PKTCH_TEST_DISABLE,
    /* tx_pqblock */ 0,
    /* tx_pqlevel */ 0,
    /* tx_shaping_type */ WP_FMU_SHAPING_TYPE_STRICT,
    /* tx_shaping_params */ NULL,
    /* rx_maxsdu */ WT_MAX_FRAME_SIZE,
    /* tx_cwid */ WP_CW_ID_A,
    /* tx_tq */ 0,
    /* rx_queuedepth[4] */ 17
    }
   ,
   {
    /* intmode */ WP_PKTCH_INT_DISABLE,
    /* iwmode */ WP_PKTCH_IWM_DISABLE,
    /* testmode */ WP_PKTCH_TEST_DISABLE,
    /* tx_pqblock */ 0,
    /* tx_pqlevel */ 0,
    /* tx_shaping_type */ WP_FMU_SHAPING_TYPE_STRICT,
    /* tx_shaping_params */ NULL,
    /* rx_maxsdu */ WT_MAX_FRAME_SIZE,
    /* tx_cwid */ WP_CW_ID_A,
    /* tx_tq */ 0,
    /* rx_queuedepth[4] */ 17
    }
};

WP_tx_binding tx_binding_iw[1] = {
   {
    /* res0; */ 0,
    /* dci_mode; */ WP_IW_DYN_CH_INSERT_DISABLE,
    /* maxt; */ 55,
    }
};

WP_ch_iw_rx ch_config_iw[1] = {
   {
    /* pqblock */ 0,
    /* pqlevel */ 0,
    /* tx_binding_type */ WP_IW_TX_BINDING,
    /* tx_binding_config */ tx_binding_iw
    }
};

WP_tx_binding tx_binding_cfg[1] = {
   {
    /* res0 */ 0,
    /* dci_mode */ WP_IW_DYN_CH_INSERT_DISABLE,
    /* maxt */ WT_MAX_FRAME_SIZE
    }
};

WP_ch_iw_rx host_ch_cfg[1] = {
   {
    /* pqblock */ 0,
    /* pqlevel */ 0,
    /* tx_binding_type */ WP_IW_TX_BINDING,
    /* tx_binding_config */ 0,
    }
};

WP_pkt_shaping_wfq wfq[1] = { {
                               /* weight;  */ 1,
                               /* weight_fraction; */ 0
                               }
};

WP_pkt_shaping_cir_eir shaping_params_cir_eir[1] = {
   {

    /* cir */ 40000000,
    /* cir_polling */ WP_SHAPING_POLL_AUTOD,
    /* cbs */ 800000,
    /* eir */ 50000000,
    /* eir_polling */ WP_SHAPING_POLL_AUTOD,
    /* ebs */ 800000,
    /* eir_cwid */ WP_CW_ID_B,
    /* eir_tq */ 2,
    }
};

WP_ch_enet ch_config_enet[1] = {
   {
    /* intmode */ WP_PKTCH_INT_ENABLE,
    /* iwmode */ WP_PKTCH_IWM_ENABLE,
    /* testmode */ WP_PKTCH_TEST_DISABLE,
    /* tx_pqblock */ 0,
    /* tx_pqlevel */ 0,
    /* tx_shaping_type */ WP_PKT_SHAPING_CIR_EIR,
    /* tx_shaping_params */ &shaping_params_cir_eir,
    /* rx_maxsdu */ WT_MAX_FRAME_SIZE,
    /* tx_cwid */ WP_CW_ID_B,
    /* tx_tq */ 0,
    /* rx_queuedepth */ MAX_NUM_OF_PACKETS
    }
};

WP_ch_enet ch_config_fast_enet2[1] = {
   {
    /* intmode */ WP_PKTCH_INT_ENABLE,
    /* iwmode */ WP_PKTCH_IWM_ENABLE,
    /* testmode */ WP_PKTCH_TEST_DISABLE,
    /* tx_pqblock */ 0,
    /* tx_pqlevel */ 0,
    /* tx_shaping_type */ WP_FMU_SHAPING_TYPE_STRICT,
    /* tx_shaping_params */ NULL,
    /* rx_maxsdu */ WT_MAX_FRAME_SIZE,
    /* tx_cwid */ WP_CW_ID_B,
    /* tx_tq */ 0,
    /* rx_queuedepth */ 17
    }
};

WP_calendar_wheel calendar[3] = {
   {
    /* function */ WP_CW_ASU,
    /* cwid */ WP_CW_ID_A,
    /* min_ch_rate */ 150,
    /* max_ch_rate */ 600000,
    }
   ,
   {
    /* function */ WP_CW_PSU,
    /* cwid */ WP_CW_ID_B,
    /* min_ch_rate */ 1000000,
    /* max_ch_rate */ 100500000,
    }
   ,
   WP_CALENDAR_WHEEL_TERM
};

WP_qnode_iwq qn_iw[1] = {
   {
    /* interruptqueue */ 0,
    /* num_buffers */ 8192,
    /* adjunct_pool */ 0
    }
};

/* <<<<<<<<<<<<<<<<<<<<<<<<<<< GMII START >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */

/* Giga Ethernet port1 & 2 configuration */
WP_port_enet enet_s_fast[1] = {
   {
    /* pkt_limits */ {64, 64, 8, NUM_PQ_BLOCK}
    ,
    /* flowmode */ WP_FLOWMODE_FAST,
    /* interface_mode */ WP_ENET_SGMII_1000,
    /* rx_iw_bkgnd */ 0
    }
};

/* Giga Ethernet port1 & 2 configuration */
WP_port_enet enet_s[1] = {
   {
    /* pkt_limits */ {64, 64, 8, NUM_PQ_BLOCK}
    ,
    /* flowmode */ WP_ENET_FMU_HIERARCHICAL_SHAPING_MODE,
    /* interface_mode */ WP_ENET_SGMII_1000,
    /* rx_iw_bkgnd */ 0
    }
};

/* Enet device configuration */
WP_device_enet enet_dev[1] = {
   {
    /* max_tx_channels */ MAX_NUM_OF_CHANNELS,
    /* tx_maxsdu */ WT_MAX_FRAME_SIZE,
    /* operating_speed */ WP_UNUSED,
    /* mac_addr */ {0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa}
    ,
    /* tx_bit_rate */ WP_TX_BIT_RATE_UNLIMITED,
    /* loopbackmode */ WP_ENET_NORMAL
    }
};

WP_enet_multi_class_config multi_class_config[1] = {
   {
    /* multi_class_mode           */ WP_ENET_MULTI_CLASS_DISABLED,
    /* max_multi_class_devices    */ 0,
    }
};

WP_device_enet_multi_class enet_multi_class_dev_cfg[1] = {
   {
    /* class_level */ 0,
    /*tx_shaping_type (select_mode) */
    WP_ENET_MULTI_CLASS_SHAPING_ROUND_ROBIN,
    /*tx_shaping_params */ NULL,
#ifdef ADDED_BY_MORRIS
    /*tx_bit_rate */ WP_TX_BIT_RATE_UNLIMITED,
#endif
    }
};

WP_device_enet_ex enet_device_extended_config[1] = {
   {
    /*duplexmode */ WP_ENET_FULLDUPLEX,
    /*rx_maclookup_mode */ WP_DISABLE,
    /*rx_flowcontrol */ WP_ENET_FLOW_DISABLE,
    /*tx_flowcontrol */ WP_ENET_FLOW_DISABLE,
    /*rx_addrmode */ WP_ENET_ADDR_ACCEPTALL,
    /*phystatmode */ WP_ENET_STAT_ENABLE,
    /*tx_statmode */ WP_ENABLE,
    /*rx_statmode */ WP_ENABLE,
    /*tx_duplicate */ WP_DISABLE,
    /*rx_filters */ 0,
    /*rx_timestamp */ WP_DISABLE,
    /*timestamp_type */ 0,
    /*max_tx_slow_channels */ NUM_OF_CHANNELS,
    }
   ,
};

/* <<<<<<<<<<<<<<<<<<<<<<<<<<< GMII END >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */

/* <<<<<<<<<<<<<<<<<<<<<<<<<<< HSPOS START >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */

/* Configure Utopia port */
WP_port_pkt_limits pkt_limits = {
   /* max_tx_channels */ MAX_NUM_OF_CHANNELS,
   /* max_rx_channels */ MAX_NUM_OF_CHANNELS,
   /* pq_block_size   */ 1,
   /* n_pq_blocks     */ NUM_OF_CHANNELS,
   /* emphy_devices   */
};

WP_port_upi_packet_ext pkt_ext = {
   /* packet_characteristics */
   /*WP_UPI_POS_CRC_ENABLE, */ 0,
   /* slow_phy_tx_bitrates */ 0,
   /* packet_limits */ &pkt_limits
};

/* Configuration for the HSPOS Device */
WP_device_upi_hspos upi_hspos_device[1] = {
   {
    /* tx_bit_rate; */ WP_TX_BIT_RATE_UNLIMITED,
    /* tx_statmode; */ WP_PKT_STAT_ENABLE,
    /* tx_intmode; */ WP_PKTCH_INT_ENABLE,
    /* tx_slave_addr; */ 0,
    /* tx_queuedepth[8]; */ {0}
    ,
    /* rx_bit_rate; */ 622000000,
    /* rx_statmode; */ WP_PKT_STAT_ENABLE,
    /* rx_intmode; */ WP_PKTCH_INT_ENABLE,
    /* rx_queuedepth; */ 2000,
    /* rx_slave_addr; */ 0,
    /* tx_tag; */ 0,
    /* rx_tag; */ 0,
    /* end_station_mode; */ 0,
#if _WP_MODULE_NEC_GPON_
    /* bridge_port_id_mode; */ 0,
    /* remove_fcs; */ 0,
#error GPON_should_not_defined
#else
#endif
    /* tx_maxsdu; */ WT_MAX_FRAME_SIZE,
#if _WP_MODULE_NEC_GPON_
    /* enet_over_pos; */ 0,
#endif
    /*max_tx_slow_channels; */ NUM_OF_CHANNELS,
    }
};

/* <<<<<<<<<<<<<<<<<<<<<<<<<<< HSPOS END >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */

/*    InterWorking - Configurations */

/* Giga -> Giga */

WP_iw_agg_bridging br_agg_gbe[1] = {
   {
    /*tag */ 2,
    /*txfunc */ 0,
    /*input_bport */ 0,
    /*rfcs */ WP_IW_RFCS_ENABLE,
    /*l2_header_insert_mode; */ WP_IW_L2H_INSERT_DISABLE,
    /*vlan_tag_mode */ WP_IW_VLAN_TAG_ENABLE,
    /*interruptqueue; */ WP_IW_IRQT1,
    /*error_pkt_mode */ WP_IW_ERRPKT_DISCARD,
    /*intmode; */ WP_IW_INT_ENABLE,
    /*statmode; */ WP_IW_STAT_ENABLE,
    /*timestamp_mode; */ WP_IW_TIME_STAMP_DISABLE,
    /*ov_pool_mode */ WP_IW_OV_POOL_DISABLE,
    /*fbp_drop_threshold; */ 0,
    /*replace_vlan_id */ WP_IW_REPLACE_VTAG_DISABLE,
    /*vlan_id */ 0x05,
    /*vpmt_handle */ 0,
    /*mtu; */ WT_MAX_FRAME_SIZE,
    /*prefix_length */ 0,
    /*prefix_header[28]; */ {0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7,
                             0x8, 0x9, 0x10, 0x11, 0x12, 0x13, 0x14,
                             0x15, 0x16, 0x17, 0x18, 0x19, 0x20, 0x21,
                             0x22, 0x23, 0x24, 0, 0, 0, 0}
    ,
    /*policer_enable */ WP_IW_POLICER_DISABLE,
      /**policer_config;*/ NULL,
    /*cong_mode; */ WP_IW_CONGESTION_DISABLE,
      /**cong_threshold_param;*/ NULL
    }
};

WP_iw_agg_directmap dl_tx_agg_gbe[1] = {
   {
    /* tag                       */ 0,
    /* txfunc                    */ 0,
    /* rfcs                      */ WP_IW_RFCS_DISABLE,
    /* l2_header_insert_mode     */ WP_IW_L2H_INSERT_ENABLE,
    /* interruptqueue            */ WP_IW_IRQT1,
    /* error_pkt_mode            */ 0,
    /* intmode                   */ WP_IW_INT_DISABLE,
    /* statmode                  */ WP_IW_STAT_ENABLE,
    /* timestamp_mode            */ 0,
    /* ov_pool_mode              */ 0,
    /* fbp_drop_threshold        */ 0,
    /* mtu                       */ 1586,

    /* prefix_length             */ WT_MAC_HEADER_SIZE,
    //+ WT_IPV4_HEADER_SIZE + WT_L4_HEADER_SIZE,
    /* extraction_length         */ 0,
    //WT_IPV4_HEADER_SIZE + WT_L4_HEADER_SIZE,

    /* prefix_header[42] */ {
                             /* Dest; Src; */

                             0x00, 0x01, 0x02, 0x03, 0x04, 0x00,  /* Eth header */
                             0x00, 0x01, 0x02,
                             0x03, 0x04, 0x00,
                             0x08, 0x00}
    ,

    /* policer_enable            */ 0,
    /* policer_config            */ 0,
    /* cong_mode                 */ 0,
    /* cong_threshold_param      */ 0,
    /* dynamic_length_update;    */ 0,
    //WP_IW_LENGTH_UPDATE_L3,
    /* length_offset;            */ 0,
    //WT_MAC_HEADER_SIZE,
    }

};

WP_iw_agg_directmap ul_tx_agg_add_eth[1] = {
   {
    /* tag                       */ 0,
    /* txfunc                    */ 0,
    /* rfcs                      */ WP_IW_RFCS_DISABLE,
    /* l2_header_insert_mode     */ WP_IW_L2H_INSERT_ENABLE,
    /* interruptqueue            */ WP_IW_IRQT1,
    /* error_pkt_mode            */ 0,
    /* intmode                   */ WP_IW_INT_DISABLE,
    /* statmode                  */ WP_IW_STAT_ENABLE,
    /* timestamp_mode            */ 0,
    /* ov_pool_mode              */ 0,
    /* fbp_drop_threshold        */ 0,
    /* mtu                       */ 1586,

    /* prefix_length             */ WT_MAC_HEADER_SIZE,
    /* extraction_length         */ 0,

    /* prefix_header[42] */ {
                             /* Dest; Src; */

                             0x00, 0x01, 0x02, 0x03, 0x04, 0x00,  /* Eth header */
                             0x00, 0x01, 0x02,
                             0x03, 0x04, 0x00,
                             0x08, 0x00}
    ,

    /* policer_enable            */ 0,
    /* policer_config            */ 0,
    /* cong_mode                 */ 0,
    /* cong_threshold_param      */ 0,
    /* dynamic_length_update;    */ 0,
    /* length_offset;            */ 0,
    }

};

WP_iw_agg_directmap ul_tx_agg_gbe[1] = {
   {
    /* tag                       */ 0,
    /* txfunc                    */ 0,
    /* rfcs                      */ WP_IW_RFCS_DISABLE,
    /* l2_header_insert_mode     */ WP_IW_L2H_INSERT_ENABLE,
    /* interruptqueue            */ WP_IW_IRQT1,
    /* error_pkt_mode            */ 0,
    /* intmode                   */ WP_IW_INT_DISABLE,
    /* statmode                  */ WP_IW_STAT_ENABLE,
    /* timestamp_mode            */ 0,
    /* ov_pool_mode              */ 0,
    /* fbp_drop_threshold        */ 0,
    /* mtu                       */ 1586,

    /* prefix_length             */
    WT_IPV4_HEADER_SIZE + WT_L4_HEADER_SIZE,
    /* extraction_length         */
    WT_IPV4_HEADER_SIZE + WT_L4_HEADER_SIZE,

    /* prefix_header[42] */ {
                             /* Dest; Src; */

                             0x45, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x11   /* IP header */
                             , 0x3d, 0xf1, 0xc0, 0xa8, 0x01, 0x01, 0xc0, 0xa8, 0x96, 0x46, 0x07, 0x7e, 0x13, 0x89, 0x05, 0x2c, 0x00, 0x00 /* Udp header */
                             }
    ,

    /* policer_enable            */ 0,
    /* policer_config            */ 0,
    /* cong_mode                 */ 0,
    /* cong_threshold_param      */ 0,
    /* dynamic_length_update;    */ WP_IW_LENGTH_UPDATE_L3,
    /* length_offset;            */ 0,
    }

};

WP_dfc_sys_info dfc_rout_sys_info[1] = {
   {
    /* classification_hash_size */ WP_IW_HASH_UNUSED,
    /* port_filtering */ WP_IW_PORT_FILTERING_DISABLED,
    /* deny_eq_ip */ WP_IW_DENY_EQUAL_IP_DISABLE
    }
};

WP_iw_ipv4_sys_info ipv4_sys_one[1] = {
   {
    /* classification_mode */ WP_IW_NULL_CLASSIFIER,
    /* max_lpm_flows */ 0,
    /* *dfc_info  */ &dfc_rout_sys_info[0]
    }
};

WP_iw_sys_routing iw_sys_one_routing[1] = {
   {
    /*  buffer_gap */ 0x40,
    /*  max_iw_ports */ 4,
    /*  max_classification_rules */ 2,
    /* supported_protocols */ WP_IW_SYS_IPV4,
    /* ipv4_sys_info */ ipv4_sys_one,
    /* *ipv6_sys_info */ NULL,
    /* *mpls_sys_info */ NULL,
    }
};

WP_tx_binding tx_binding_l2pi[1] = {
   {
    /* wred_mode; */ 0,
    /* dci_mode; */ 1,
    /* maxt; */ 128,
    }
};

WP_rx_binding_routing rx_binding_enet_routing[1] = {
   {
    /*  encap_mode */ 0,
    /*  l3_support_protocol; */ WP_IW_IPV4,
    /*  mru; */ (WT_MAX_FRAME_SIZE - 48)
    }
};

WP_bridge_port iwhost_bport_config[1] = {
   {
    /* tag */ 0x2000,
    /* direct_mapping */ WP_IW_DIRECT_MAP_ENABLE,
    /* flow_agg */ 0,
    /* flooding_term_mode */ WP_IW_HOST_TERM_MODE,
    /* learning_mode */ WP_IW_LEARNING_DISABLE,
    /* in_filter_mode */ WP_IW_INGRESS_FILTER_DISABLE,
    /* vlan_param */
    {
     /* vlan_acceptance_mode */ WP_IW_ACCEPT_TAGGED_UNTAGGED,
     /* vlan_tag */ 0x5,
     }
    ,
    /*max_mac_addresses */ 34,
    /*group_tag */ 0,
    /*group_filtering_mode */ WP_IW_GROUP_FILTER_DISABLE,
    /*unk_mac_sa_filter */ 0,
    /*unk_mc_mode; */ 0,
    /*bc_ht_mode; */ 0,
    /*input_filters_mask */ 0,
    /*output_filters_mask; */ 0,
    /*statmode */ WP_IW_PORT_STAT_ENABLE
    }
};

WP_iw_route_port route_port_cfg = {
   /* tag */ 0,
   /* input_filters_mask; */ 2,
   /* output_filters_mask; */ 2,
   /* statmode; */ WP_IW_PORT_STAT_ENABLE,
   /* forwarding_mode */ WP_IW_FORWARDING_DIRECT_MAP,
};

void WP_AppLog (WP_CHAR * s)
{
}

void WT_Start (WP_CHAR * app_name, WP_CHAR * trial_name,
               WP_CHAR * file_name);
void WT_Reboot (void);
static void terminate_on_error (WP_handle, WP_CHAR *, WP_U32);
static void terminate (WP_U32 line);
static void WPE_LayerTwoSystemCreate (void);
static void WPE_SystemEnable (void);
static void WPE_DlForwardingTableCreate (void);
static void WPE_ShapingGroupsCreate (void);
static void WPE_ChannelsCreate (void);
static void WPE_DlPortsCreate (WP_handle iw_sys);
static void WPE_DlRxBindingCreate (WP_handle iw_sys);
void WPT_DisplayBridgingPortStats (WP_handle port_handle, WP_CHAR * type);
void WPT_DisplayRoutingPortStats (WP_handle port_handle, WP_CHAR * type);

static void WPE_HostSendTraffic (void);
static void WPE_HostSendFlowCtrl (void);

void WPE_CheckReceivedTraffic (WP_U32 expected_count);
WP_U16 WTI_IpChecksumCalc (WP_U8 * buff, WP_U16 len);
void WTI_TranslateHexToAscii (WP_CHAR * Ascii, WP_CHAR * Hex,
                              WP_U32 length);
#if TEST_LOOPBACK_MODE
#else
#endif

#if 1                           //DEBUG && TEST_LOOPBACK_MODE
void WPT_FlowStatistics (WP_handle agg);
#endif

void CLI_RunCommonConfig (void);

static void WPE_CreateDlPceRulesFiltersSets (WP_handle iw_sys);

#if PCE_DEBUG
void WPE_PrintPceGlobalStats (void);
#endif
static void WPE_CreateFlowCtrlPceFilters (void);
static void WPE_CreateFlowCtrlPceRules (void);
static void WPE_CreateDataPceFilters (void);

#ifdef ADDED_BY_MORRIS
void WPE_CreateDlPceRules (void);
#else
static void WPE_CreateDataPceRules (void);

#endif

WP_U32 WPT_LocalDisplayDeviceStats (WP_handle device_handle);
void WPE_CheckPceFilterStats (WP_handle filter);
void WPE_ResetDataPceFilter (void);
void WPE_ResetFlowCtrlPceFilter (void);

WP_handle devhost;
WP_handle filter_handle[NUM_OF_FLOWS];
WP_handle default_agg;
WP_handle pool_2048, pool_144;

WP_handle gbe_port1, gbe_dev1, gbe_port3, gbe_port4;
WP_handle gbe_port2, gbe_dev2, gbe_dev3, gbe_dev4;

/**************************************************************/

WP_handle MultiClass_Device_h[NUM_OF_FLOWS];
WP_handle MultiClass_Tx_Channel_h[NUM_OF_FLOWS];

WP_handle l1_group_h[NUM_OF_FLOWS];
WP_handle l2_group_h[NUM_OF_FLOWS];

/**************************************************************/

WP_handle qniw;
WP_handle h_iw_port_gbe_1;
WP_handle rx_host_handle;
WP_handle bport_host;
WP_handle gbe1_rx_ch_handle, gbe1_tx_ch_handle;
WP_handle gbe2_rx_ch_handle, gbe2_tx_ch_handle;

WP_handle dl_iwsys_routing;

WP_U32 payload_size[MAX_NUM_OF_CHANNELS][NUM_OF_PACKETS_PER_CHANNEL];
WP_U32 l2_l3_size[MAX_NUM_OF_CHANNELS][NUM_OF_PACKETS_PER_CHANNEL];

WP_CHAR ip_protocol[MAX_NUM_OF_CHANNELS][NUM_OF_PACKETS_PER_CHANNEL][100];
WP_CHAR ip_src[MAX_NUM_OF_CHANNELS][NUM_OF_PACKETS_PER_CHANNEL][100];
WP_CHAR ip_dst[MAX_NUM_OF_CHANNELS][NUM_OF_PACKETS_PER_CHANNEL][100];

WP_handle host_channel_psu_2[1];
WP_U8 expected_data[MAX_NUM_OF_CHANNELS][NUM_OF_PACKETS_PER_CHANNEL][3536];

WP_U8 scenario_number = 1;

// WP_U32 loopback_mode;
WP_boolean global_failed = WP_FALSE, scenario_failed;

#if TEST_LOOPBACK_MODE
#endif
WP_U32 loopback_mode;

/* PCE */
WP_pce_global_stat pce_global_stats;
WP_handle PCE_filter_flow_ctrl[32];
WP_handle PCE_filter_data[32];

WP_handle flow_ctrl_filter_set_lvl0_handle;

WP_handle data_filter_set_lvl0_handle;

WP_handle PCE_rule_flow_ctrl_h[64];
WP_handle PCE_rule_data_h[64];

WP_pce_filter_stat filter_stat;

WP_U16 wpe_test_failures = 0;

WP_handle flow_agg_add_header[NUM_OF_FLOWS];

/******************************/

WP_status logCallBack (WP_S32 level, const WP_CHAR * message)
{
   if (level <= logTraceLevel)
      printf ("%s", message);
   return WP_OK;
}

WP_U32 debug_error = 1;

WP_status WPE_DebugCallback (const WP_CHAR * file,
                             const WP_CHAR * function, WP_S32 line,
                             WP_S32 error_id)
{
   if (debug_error)
   {
      if (error_id != WP_ERR_HST_NO_DATA_TO_GET)
         printf ("\n\nfile = %s , line = %d\nfunction = %s\n", file, line,
                 function);
   }
   return WP_OK;
}

void WTI_GenerateRandomData (WP_CHAR * Hex, WP_U8 pattern1, WP_U8 pattern2,
                             WP_U8 rand_flag, WP_U32 max_size,
                             WP_U32 min_size)
{
   WP_U8 tmprand;
   WP_U32 i, datarandlength;

   if (max_size == min_size)
      datarandlength = max_size;
   else if (min_size < max_size)
      datarandlength = min_size + (rand () % (max_size - min_size));
   else
      datarandlength = max_size + (rand () % (min_size - max_size));

   for (i = 0; i < datarandlength; i++)
   {

      if (rand_flag)
         tmprand = rand () % 256;   /* random numer 0-256 */
      else
      {
         if (i % 2 == 0)
            tmprand = pattern1;
         else
            tmprand = pattern2;
      }

      Hex[i] = tmprand;
   }
}

void FlowCtrlDataUnitSetup (WP_data_unit * data_unit,
                            WP_data_segment * km_tx_buf,
                            WP_U8 * km_data_ptr)
{
   // WP_U32 header_size;
   WP_CHAR ascii_buffer[MAX_DATA_LENGTH * 2 + 1];
   WP_CHAR hexa_buffer[MAX_DATA_LENGTH * 2];

   /* Prepare packet for Host Send  */
   data_unit->data_size = MAX_DATA_LENGTH;
   data_unit->n_active = 1;

   km_tx_buf->data_size = data_unit->data_size;
   km_data_ptr = WP_PoolAlloc (pool_144);
   km_tx_buf->pool_handle = pool_144;
   km_tx_buf->data = km_data_ptr;

   km_tx_buf->next = NULL;
   km_tx_buf->displacement = 0;

   data_unit->segment = (km_tx_buf);
   data_unit->n_segments = 1;
   data_unit->type = WP_DATA_ENET;
   data_unit->control = WP_HT_CONTROL (0);

// -------------   CBFC message -----------------
   strcat (ascii_buffer, "0180c2000001");
   strcat (ascii_buffer, "ab12cd34ef56");
   strcat (ascii_buffer, "8808");

   strcat (ascii_buffer, "0101");   // anoter choice is 0x0001

   strcat (ascii_buffer, "00ff");   /* CBFC vector --- all the 8 queues */

   strcat (ascii_buffer, "ffff");   /* CBFC  class 0 */
   strcat (ascii_buffer, "ffff");   /* CBFC  class 1 */
   strcat (ascii_buffer, "ffff");   /* CBFC  class 2 */
   strcat (ascii_buffer, "ffff");   /* CBFC  class 3 */
   strcat (ascii_buffer, "ffff");   /* CBFC  class 4 */
   strcat (ascii_buffer, "ffff");   /* CBFC  class 5 */
   strcat (ascii_buffer, "ffff");   /* CBFC  class 6 */
   strcat (ascii_buffer, "ffff");   /* CBFC  class 7 */

   strcat (ascii_buffer, "00000000");  /* Padding 28 */
   strcat (ascii_buffer, "00000000");
   strcat (ascii_buffer, "00000000");
   strcat (ascii_buffer, "00000000");
   strcat (ascii_buffer, "00000000");
   strcat (ascii_buffer, "00000000");
   strcat (ascii_buffer, "00000000");

   data_unit->data_size = 62;

   km_tx_buf->data_size = data_unit->data_size;

   App_TranslateAsciiToHex ((WP_CHAR *) hexa_buffer,
                            (WP_CHAR *) ascii_buffer,
                            data_unit->data_size);

   WP_MEM_BYTES_FILL (km_data_ptr, '0', buffer_data_144[0].size);
   WP_MEM_BYTES_SET (km_data_ptr, (WP_U8 *) hexa_buffer, MAX_DATA_LENGTH);

#if DEBUG
   {
      WP_U32 ii = 0;

      printf ("packet : %x \n ", (WP_U32) km_data_ptr);

      for (ii = 0; ii < data_unit->segment->data_size; ii++)
      {
         if ((ii) % 4 == 0)
            printf ("\n");
         printf ("%2.2x", data_unit->segment->data[ii]);
      }
      printf ("( %d bytes )\n", data_unit->segment->data_size);
   }
#endif
}

void DataUnitSetup (WP_data_unit * data_unit,
                    WP_data_segment * km_tx_buf,
                    WP_U8 * km_data_ptr, WP_CHAR * dst_mac,
                    WP_CHAR * src_mac, WP_CHAR * ip_protocol,
                    WP_CHAR * ip_src, WP_CHAR * ip_dst,
                    WP_U32 payload_size, WP_U8 channel_index,
                    WP_U8 packet_index)
{
   WP_U32 header_size;
   WP_CHAR ascii_buffer[MAX_DATA_LENGTH * 2 + 1];
   WP_CHAR hexa_buffer[MAX_DATA_LENGTH * 2];

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
   km_data_ptr = WP_PoolAlloc (pool_144);
   km_tx_buf->pool_handle = pool_144;
   km_tx_buf->data = km_data_ptr;

   km_tx_buf->next = NULL;
   km_tx_buf->displacement = 0;

   data_unit->segment = (km_tx_buf);
   data_unit->n_segments = 1;
   data_unit->type = WP_DATA_ENET;
   data_unit->control = WP_HT_CONTROL (0);

   /* set the src and dst mac address at the packet */
   strcpy (ascii_buffer, dst_mac);
   strcat (ascii_buffer, src_mac);

   /* set the ethernet type at the packet */
   strcat (ascii_buffer, "0800");

   ip_length = WT_IPV4_HEADER_SIZE + WT_L4_HEADER_SIZE + payload_size;
   sprintf (ip_length_char, "%04x", ip_length); /* Total Length */

   sprintf ((WP_CHAR *) temp_ip_header_char, "45");   /* Version = Ipv4,  IHL = 5 */
   strcat ((WP_CHAR *) temp_ip_header_char, "80"); /* Type Of Service - 8 bit */
   strcat ((WP_CHAR *) temp_ip_header_char, ip_length_char);   /* Total Length */
   strcat ((WP_CHAR *) temp_ip_header_char, "00000000"); /* Id, Flags, frag */
   strcat ((WP_CHAR *) temp_ip_header_char, "0A"); /* TTL = 0x0a, */
   strcat ((WP_CHAR *) temp_ip_header_char, ip_protocol);   /* ip_protocol - 8 bits */
   strcat ((WP_CHAR *) temp_ip_header_char, "0000");  /* IP header Checksum - updated later */

   strcat ((WP_CHAR *) temp_ip_header_char, ip_src);  /* IP src address */
   strcat ((WP_CHAR *) temp_ip_header_char, ip_dst);  /* IP dst address */

   App_TranslateAsciiToHex ((WP_CHAR *) temp_ip_header_hex,
                            (WP_CHAR *) temp_ip_header_char,
                            WT_IPV4_HEADER_SIZE);
   checksum = WTI_IpChecksumCalc (temp_ip_header_hex, WT_IPV4_HEADER_SIZE);
   temp_ip_header_hex[10] = checksum >> 8;
   temp_ip_header_hex[11] = checksum & 0xff;
   WTI_TranslateHexToAscii ((WP_CHAR *) temp_ip_header_char,
                            (WP_CHAR *) temp_ip_header_hex,
                            WT_IPV4_HEADER_SIZE);

   strcat (ascii_buffer, (WP_CHAR *) temp_ip_header_char);

   header_size = strlen (ascii_buffer) / 2;
   l2_l3_size[channel_index][packet_index] = header_size;
#if DEBUG
   printf
      ("\nHOST SEND DATA: l2_l3_size=%d, payload_size=%d, channel_index=%d ",
       l2_l3_size[channel_index][packet_index], payload_size,
       channel_index);
#endif
   App_TranslateAsciiToHex (hexa_buffer, ascii_buffer, header_size);

   /* Src and Dst ports */
   hexa_buffer[header_size + 0] = IWF_Table[channel_index][2] >> 8;
   hexa_buffer[header_size + 1] = IWF_Table[channel_index][2] % 0xFF;
   hexa_buffer[header_size + 2] = IWF_Table[channel_index][3] >> 8;
   hexa_buffer[header_size + 3] = IWF_Table[channel_index][3] % 0xFF;
   header_size += WT_L4_HEADER_SIZE;

   WTI_GenerateRandomData (&hexa_buffer[header_size], channel_index,
                           packet_index, 0, payload_size, payload_size);

   WP_MEM_BYTES_FILL (km_data_ptr, '0', buffer_data_144[0].size);
   WP_MEM_BYTES_SET (km_data_ptr, (WP_U8 *) hexa_buffer, MAX_DATA_LENGTH);

   data_unit->data_size = header_size + payload_size;
   km_tx_buf->data_size = data_unit->data_size;

#if DEBUG
   {
      WP_U32 ii = 0;

      printf ("packet : %x \n ", (WP_U32) km_data_ptr);

      for (ii = 0; ii < data_unit->segment->data_size; ii++)
      {
         if ((ii) % 4 == 0)
            printf ("\n");
         printf ("%2.2x", data_unit->segment->data[ii]);
      }
      printf ("( %d bytes )\n", data_unit->segment->data_size);
   }
#endif
}

#if TEST_LOOPBACK_MODE
#endif




void App_TranslateAsciiToHex(WP_CHAR *Hex,WP_CHAR *Ascii,WP_U32 length)
{
    WP_S32 i,first=0,second=0;

   for (i=0;i<length;i++)
     {

          if ( Ascii[2*i] >= '0' &&  Ascii[2*i] <= '9')
            first =  Ascii[2*i] - '0';

          if ( Ascii[2*i] >= 'a' &&  Ascii[2*i] <= 'f')
             first =  Ascii[2*i] - 'a' + 10;

          if ( Ascii[2*i] >= 'A' &&  Ascii[2*i] <= 'F')
             first =  Ascii[2*i] - 'A' + 10;

          if ( Ascii[2*i+1] >= '0' &&  Ascii[2*i+1] <= '9')
            second =  Ascii[2*i+1] - '0';
          if ( Ascii[2*i+1] >= 'a' &&  Ascii[2*i+1] <= 'f')
            second =  Ascii[2*i+1] - 'a' + 10;
          if ( Ascii[2*i+1] >= 'A' &&  Ascii[2*i+1] <= 'F')
            second =  Ascii[2*i+1] - 'A' + 10;

          Hex[i]=(WP_CHAR )(first * 16 + second);
     }
    return;
}







WP_flow_class_ext_action action;
WP_U8 action_type[3];
WP_flow_class_action_info action_info[3];
WP_CHAR dfc_classifier_string[300];
WP_U32 temp, offset;
WP_flow_class_forwarding_action forwarding_action;
WP_flow_class_priority_action priority_action;

WP_S32 main (WP_S32 argc, WP_CHAR ** argv)
{
   void *p1 = 0;

   p1 = 0;
//  p1 = (void *)((int*)&WPE_PrintPceGlobalStats);

   WT_Start ("03120", NULL, __FILE__);
   /* run the scenarios */

   CLI_RunCommonConfig ();

#if WTI_REGRESSION_MODE
   /* check if the "total" test fail */
   if (global_failed == WP_FALSE)
      printf ("\nTest Passed\n");
   else
      printf ("\nTest Failed\n");

#endif
   /* Release WDDI */
   terminate (__LINE__);

   exit (0);
   return 1;
}



void WT_FailAndTerminate(void)
{
   printf("Test Failed\n");
   WP_DriverRelease();
#if USE_SOCKET
   CleanUpSocket(5);
#endif
   exit(1);
}


void WT_Identify(void)
{
   WP_U32 p, w;
   WP_CHAR *first_dps_found = NULL;
   WP_U32 found_error = 0;
   printf("Test Identity Version %s\n", WP_Identity.wddi_version);
   printf("Test Identity Build %s\n", WP_Identity.wddi_build);
   for (p = 0; p < WP_MAX_WINPATHS; p++) {
      for (w = 0; w < WP_WINFARMS; w++) {
         if (!WP_Identity.dps_in_wddi[p][w] ||
             strlen(WP_Identity.dps_in_wddi[p][w]) == 0)
            continue;
         printf("Test Identity DPS WP%d WF%d in wddi %s in use %s\n", p, w,
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
      printf("Test Abort DPS not uniform\n");
      WT_FailAndTerminate();
   }
}

void CLI_RunCommonConfig (void)
{
   WP_handle status;
   WP_U32 i;
   WP_U8 cmd;

   i = 0;
   cmd = 0;
#if DEBUG
   WP_ControlRegister (WP_DEBUG_CALLBACK_WDDI_LOG, logCallBack);
   WP_ControlRegister (WP_DEBUG_CALLBACK_FILE_LINE_ERROR,
                       WPE_DebugCallback);
#endif

   WPE_LayerTwoSystemCreate ();

   status = WP_SysCommit ();
   terminate_on_error (status, "WP_SysCommit", __LINE__);
#if DEBUG
   WP_Display (WP_WINPATH (DEFAULT_WPID), gbe_port1, 0, 0);
#endif
   WT_Identify ();

   WPE_ShapingGroupsCreate ();

   WPE_ChannelsCreate ();

   /* Create DL IW system */
   dl_iwsys_routing =
      WP_IwSystemCreate (WP_WINPATH (DEFAULT_WPID),
                         WP_IW_ROUTING_MODE, &iw_sys_one_routing[0]);
   terminate_on_error (dl_iwsys_routing, "WP_IwSystemCreate", __LINE__);

   WPE_DlPortsCreate (dl_iwsys_routing);
   WPE_DlRxBindingCreate (dl_iwsys_routing);

   WPE_SystemEnable ();

   /* Configure DL Flow Aggregations for IWF packet editing */
   WPE_DlForwardingTableCreate ();

   status = WP_IwSystemBuild (dl_iwsys_routing);
   terminate_on_error (status, "WP_IwSystemBuild()", __LINE__);

        /*---------------------------------------------------------*\

          Creates the DL PCE filters and rules (and filter sets) 

        \*---------------------------------------------------------*/
   WPE_CreateDlPceRulesFiltersSets (dl_iwsys_routing);

   WPE_ResetFlowCtrlPceFilter ();
   WPE_ResetDataPceFilter ();

   WP_Delay (1000);
// #if WTI_REGRESSION_MODE
#if 0

/* Sanity check - send data, receive data */
/* Send CBFC , send data, expected to receive no data withing the receive interval */
   WPE_HostSendTraffic ();
   WPE_CheckReceivedTraffic (NUM_OF_FLOWS);

   WPE_HostSendFlowCtrl ();
   WPE_HostSendTraffic ();
   WPE_CheckReceivedTraffic (0); /* No packets should be received within the small interval */

#else /* WTI_REGRESSION_MODE */
   while (1)
   {
      printf ("\n\nMENU (press key without return)\n");
      printf ("_________________________________________(%4d)___\n",
              ++num_of_packets);
      printf ("1: Show Device\t\tStats\n");
      printf ("2: Show Flow\t\tStats\n");
      printf ("3: Show Routing Port\tStats\n");
      printf ("4: Show Pce filter\tStats\n");
#if TEST_LOOPBACK_MODE
      printf ("8: Show Channel\t\tStats\n");
      printf ("...\n");
      printf ("5: Send Pause/CBFC\n");
      printf ("6: Send Traffic\n");
      printf ("7: Received Traffic\n");
#endif
      printf ("...\n");
      printf ("r: Rebot the system\n");
      printf ("a: show flowAgg info\n");
      printf ("b: show WP_ChannelQDepth()\n");
      printf ("c: show WP_QNodeStatus()\n");
      printf ("k: Exit to WinMon without WP_DriverRelease()\n");
      printf ("x: Exit to WinMon\n");
      cmd = getchar ();

      if (cmd == '3')
         WPT_DisplayRoutingPortStats (h_iw_port_gbe_1, "GBE1");

      if (cmd == '1')
      {
         printf ("\nDevice GE1(gbe_dev1) statistics");
         WPT_LocalDisplayDeviceStats (gbe_dev1);
#if 0
         WPT_LocalDisplayDeviceStats (gbe_dev2);
#endif

      }
      if (cmd == '2')
      {
         for (i = 0; i < NUM_OF_FLOWS; i++)
            WPT_FlowStatistics (flow_agg_add_header[i]);
      }

      if (cmd == '4')
      {
         printf ("Flow control filter stats\n");
         for (i = 0; i < NUM_FLOW_CTRL_FILTERS; i++)
         {
            WPE_CheckPceFilterStats (PCE_filter_flow_ctrl[i]);
         }
         printf ("\n\nData filter stats\n");
         WPE_CheckPceFilterStats (PCE_filter_data[0]);
      }
#if TEST_LOOPBACK_MODE
      if (cmd == '5')
         WPE_HostSendFlowCtrl ();
      if (cmd == '6')
         WPE_HostSendTraffic ();

      if (cmd == '7')
      {
         WPE_CheckReceivedTraffic (NUM_OF_FLOWS);
      }

#endif
      if (cmd == '8')
      {
         WP_stats_enet_multi_class_tx mc_tx;

         for (i = 0; i < NUM_FLOW_CTRL_FILTERS; i++)
         {
            status = WP_DeviceStatistics (MultiClass_Device_h[i], &mc_tx);
            if (status)
            {
               printf ("Error Reading Enet Statistics[%d]\n", i);
               // exit (1);
            }
            else
            {
#if 0
               printf ("tx_frames(%6d)\n", mc_tx.tx_frames);
               printf ("tx_err_underrun\t\t\t(%6d)\n", 
                           mc_tx.tx_err_underrun);
//   printf ("serial_tx_dps_if_stats.rx_frames\t(%6d)\n", mc_tx.serial_tx_dps_if_stats.rx_frames);
               printf ("serial_tx_dps_if_stats.tx_frames(%6d)\n",
                       mc_tx.serial_tx_dps_if_stats.tx_frames);
               printf ("serial_tx_dps_if_stats.tx_bytes\t(%6d)\n",
                       mc_tx.serial_tx_dps_if_stats.tx_bytes);
// printf ("serial_tx_dps_if_stats.rx_bytes\t(%6d)\n", mc_tx.serial_tx_dps_if_stats.rx_bytes);
// printf ("serial_tx_dps_if_stats.rx_dropped_frames\t(%6d)\n", mc_tx.serial_tx_dps_if_stats.rx_dropped_frames);
// printf ("serial_tx_dps_if_stats.rx_pce_denied_frames\t(%6d)\n", mc_tx.serial_tx_dps_if_stats.rx_pce_denied_frames);
// WP_ChannelStatisticsReset ();
#endif
            }
         }
      }
      if (cmd == 'r')
      {
         printf ("Please wait while rebooting ...\n");
         WT_Reboot ();
      }
      if (cmd == 'k')
      {
         printf ("Driver still alive\n");
         exit (0);
         break;
      }
      if (cmd == 'a')
      {
         break;
      }
      if (cmd == 'b')
      {
	WP_U32 depth = 0;
	WP_status status = 0;

	if (WP_ERR_HANDLE == (status = WP_ChannelQDepth(gbe2_tx_ch_handle/*gbe1_rx_ch_handle*/, &depth)))
	{
		printf ("WP_ChannelQDepth() returns WP_ERR_HANDLE\n");
	} else {
		printf ("QDepth of  gbe1_rx_ch_handle(%x)\n", depth);
	}
      }
      if (cmd == 'c')
      {
	WP_U32 depth = 0;
	WP_status status = 0;

	if (WP_OK != (status = WP_QNodeStatus(qniw, WP_QNODE_STATUS_FIFO, &depth)))
	{
		printf ("WP_QNodeStatus() returns (%s), handle(%x)\n", WP_ErrorString(status), status);
	} else {
		printf ("QDepth of  qniw(%x)\n", depth);
	}
      }
      if (cmd == 'x')
      {
         WP_DriverRelease ();
         break;
      }
   }
#endif /* WTI_REGRESSION_MODE */

#if WTI_REGRESSION_MODE
   if (scenario_failed == WP_FALSE)
   {
      printf ("\nScenario %d is O.K\n", scenario_number);
   }
   else
   {
      printf ("\nScenario %d Failed\n", scenario_number);
      global_failed = WP_TRUE;
   }

   scenario_number++;
#endif
}

void WPE_ResetFlowCtrlPceFilter (void)
{
   WP_U8 i;

   for (i = 0; i < NUM_FLOW_CTRL_FILTERS /*8 */ ; i++)
   {
      WP_PceFilterStatisticsReset (PCE_filter_flow_ctrl[i], &filter_stat);
   }
}

void WPE_ResetDataPceFilter (void)
{
   WP_U8 i;

   for (i = 0; i < NUM_DATA_FILTERS /*1 */ ; i++)
   {
      WP_PceFilterStatisticsReset (PCE_filter_data[i], &filter_stat);
   }
}

void WPE_CheckPceFilterStats (WP_handle filter)
{
   WP_U32 temp1, temp2;
   WP_U8 index = (WP_U8) (filter & 0xFF);

   memset (&filter_stat, 0, sizeof (WP_pce_filter_stat));

   WP_PceFilterStatistics (filter, &filter_stat);

   printf ("\n\nComparing PCE Filter %d Statistic Changes\n", index);

   temp1 = filter_stat.denied_pkts;
   temp2 = filter_stat.denied_pkts >> 32;
   printf ("denied_pkts            %08X %08X       diff %04X\n",
           temp2, temp1, temp2-temp1);

   temp1 = filter_stat.matched_pkts;
   temp2 = filter_stat.matched_pkts >> 32;
   printf ("matched_pkts           %08X %08X       diff %04X\n",
           temp2, temp1, temp2-temp1);

   temp1 = filter_stat.no_matched_pkts;
   temp2 = filter_stat.no_matched_pkts >> 32;
   printf ("no_matched_pkts        %08X %08X       diff %04X\n",
           temp2, temp1, temp2-temp1);

   printf ("\n\n");
}

#if 1                           // DEBUG & TEST_LOOPBACK_MODE

void WPT_FlowStatistics (WP_handle agg)
{
   WP_iw_flow_stats iw_flow_stats;
   WP_U32 forward_packets = 0;
   WP_status status;

   memset (&iw_flow_stats, 0, sizeof (iw_flow_stats));
   status = WP_IwFlowStatistics (agg, WP_IW_FLOW_STAT, &iw_flow_stats);
   terminate_on_error (status, "WP_IwFlowStatistics", __LINE__);

   printf ("\nAggregation flow statistics of flow %d (%x)\n",
           (agg & 0x000000ff), agg);
   printf ("=================================================\n");

   printf ("Forward Packet:          %08x%08x\n",
           ((WPT_StatField *) & iw_flow_stats.forward_packet)->part.
           high,
           ((WPT_StatField *) & iw_flow_stats.forward_packet)->part.low);
   forward_packets =
      ((WPT_StatField *) & iw_flow_stats.forward_packet)->part.low;

   printf ("FBP Drop Packets:        %08x%08x\n",
           ((WPT_StatField *) & iw_flow_stats.fbp_drop_packets)->
           part.high,
           ((WPT_StatField *) & iw_flow_stats.fbp_drop_packets)->part.low);

   printf ("MTU Drop Packets:        %08x%08x\n",
           ((WPT_StatField *) & iw_flow_stats.mtu_drop_packets)->
           part.high,
           ((WPT_StatField *) & iw_flow_stats.mtu_drop_packets)->part.low);

   printf ("TTL Drop Packets:        %08x%08x\n",
           ((WPT_StatField *) & iw_flow_stats.ttl_drop_packets)->
           part.high,
           ((WPT_StatField *) & iw_flow_stats.ttl_drop_packets)->part.low);

   printf ("TX Queue Drop Packets:   %08x%08x\n",
           ((WPT_StatField *) & iw_flow_stats.
            tx_queue_drop_packets)->part.high,
           ((WPT_StatField *) & iw_flow_stats.
            tx_queue_drop_packets)->part.low);

   printf ("MPLS Drop:               %08x%08x\n",
           ((WPT_StatField *) & iw_flow_stats.mpls_drop)->part.high,
           ((WPT_StatField *) & iw_flow_stats.mpls_drop)->part.low);

   printf ("Denied Packets:          %08x%08x\n",
           ((WPT_StatField *) & iw_flow_stats.denied_packets)->part.
           high,
           ((WPT_StatField *) & iw_flow_stats.denied_packets)->part.low);

   printf ("Group filtered packets:  %08x%08x\n",
           ((WPT_StatField *) &
            iw_flow_stats.group_filtered_packets)->part.high,
           ((WPT_StatField *) &
            iw_flow_stats.group_filtered_packets)->part.low);

   printf ("forwarded_bytes:         %08x%08x\n",
           ((WPT_StatField *) & iw_flow_stats.forwarded_bytes)->part.
           high,
           ((WPT_StatField *) & iw_flow_stats.forwarded_bytes)->part.low);

   printf ("GTP bad headers:         %08x%08x\n",
           ((WPT_StatField *) & iw_flow_stats.gtp_bad_headers)->part.
           high,
           ((WPT_StatField *) & iw_flow_stats.gtp_bad_headers)->part.low);

   printf ("Policer Non Conforming:  %08x%08x\n",
           ((WPT_StatField *) &
            iw_flow_stats.policer_non_conforming_packets)->part.high,
           ((WPT_StatField *) &
            iw_flow_stats.policer_non_conforming_packets)->part.low);

}
#endif

static void WPE_LayerTwoSystemCreate (void)
{
   WP_status status;
   WP_handle iwp1;
   WP_CHAR output_string[256];
   WP_U32 ii;

   /* 
    * Call DriverInit, but if it is already initialized, 
    * release WDDI and call it again 
    *
    * PCE uses the ram partition.
    */
   status = WP_DriverInit ();
   if (WP_ERROR (status) == WP_ERR_WDDI_ALREADY_INITIALIZED)
   {
      status = WP_DriverRelease ();
      terminate_on_error (status, "WP_DriverRelease", __LINE__);

      status = WP_DriverInit ();
   }
   terminate_on_error (status, "WP_DriverInit", __LINE__);

   /* Init PCE hardware rules (4K rules) */
   int_ram_partition.policer_entries = 0;
   int_ram_partition.pce_hw_rules_num = 4095;
   WT_SetIntRamPartition (WP_WINPATH (DEFAULT_WPID), &int_ram_partition);

   status = WP_SysInit (WP_WINPATH (DEFAULT_WPID), context);
   terminate_on_error (status, "WP_SysInit", __LINE__);

   status = WPX_BoardConfigure (WP_WINPATH (DEFAULT_WPID),
                                WPX_CONFIGURE_DEFAULT_CHECKIN);
   terminate_on_error (status, "WPX_BoardConfigure()", __LINE__);

#ifndef WP_BOARD_WDS3_SL
   status = WPX_BoardXgiConfig (WP_WINPATH (DEFAULT_WPID), WP_PORT_XGI1);
   terminate_on_error (status, "WPX_BoardXgiConfig()", __LINE__);
   printf ("WPE_LayerTwoSystemCreate: WPX_BoardXgiConfig() OK\n");
#else
#error XGI_not_defined
#endif

#if WTI_REGRESSION_MODE
   loopback_mode = WP_TRUE;
#else
   loopback_mode = WP_FALSE;
#endif

#ifndef WP_BOARD_WDS3_SL
   status = WPX_BoardSerdesInit (WP_WINPATH (DEFAULT_WPID),
                                 WP_PORT_ENET13, loopback_mode);
   sprintf (output_string,
            "WPX_BoardSerdesInit ENET13 (WPX_BoardSerdesSetLoopback() )");
   terminate_on_error (status, output_string, __LINE__);

   /*
    * ENET14 is of no use in this demo
    */
   status = WPX_BoardSerdesInit (WP_WINPATH (DEFAULT_WPID),
                                 WP_PORT_ENET14, loopback_mode);
   sprintf (output_string,
            "WPX_BoardSerdesInit ENET14 (WPX_BoardSerdesSetLoopback() )");
   terminate_on_error (status, output_string, __LINE__);

   /*
    * XGI is the key port
    */
   status = WPX_BoardSerdesInit (WP_WINPATH (DEFAULT_WPID),
                                 WP_PORT_XGI1, loopback_mode);
   sprintf (output_string,
            "WPX_BoardSerdesInit XGI1 (WPX_BoardSerdesSetLoopback() )");
   terminate_on_error (status, output_string, __LINE__);
   printf ("WPE_LayerTwoSystemCreate: WPX_BoardSerdesInit(%s) OK\n",
           loopback_mode ? "LOOPBACK" : "NORMAL(NON-LOOP)");
#else
#error SL_can_not_be_defined
   status = WPX_BoardSerdesInit (WP_WINPATH (DEFAULT_WPID),
                                 WP_PORT_ENET11, loopback_mode);
   sprintf (output_string,
            "WPX_BoardSerdesInit ENET11 (WPX_BoardSerdesSetLoopback() )");
   terminate_on_error (status, output_string, __LINE__);

   status = WPX_BoardSerdesInit (WP_WINPATH (DEFAULT_WPID),
                                 WP_PORT_ENET12, loopback_mode);
   sprintf (output_string,
            "WPX_BoardSerdesInit ENET12 (WPX_BoardSerdesSetLoopback() )");
   terminate_on_error (status, output_string, __LINE__);
#endif //WP_BOARD_WDS3_SL

   pce_init.parser_info = &pce_parser_info;
   pce_init.pkt_rx_channel_interfaces_num = 20;
   pce_init.iw_tx_channel_interfaces_num = 20;
   pce_init.learning_queue_config.learning_queue_size = 20;

   /*
    * Initializes the PCE module.
    */
   status = WP_ModuleInit (WP_SYSHANDLE (DEFAULT_WPID),
                           WP_WDDI_MODULE_PCE, &pce_init);
   terminate_on_error (status, "WPI_PceModuleInit()", __LINE__);

   /* Creates buffer pools */

   pool_144 =
      WP_PoolCreate (WP_WINPATH (DEFAULT_WPID), WP_pool_buffer,
                     buffer_data_144);
   terminate_on_error (pool_144, "WP_PoolCreate() pool_144", __LINE__);

   pool_2048 =
      WP_PoolCreate (WP_WINPATH (DEFAULT_WPID), WP_pool_iwbuffer,
                     buffer_data_2048);
   terminate_on_error (pool_2048, "WP_PoolCreate() pool_2048", __LINE__);

   /* Create an interworking queue node */
   qn_iw->adjunct_pool = pool_2048;
   qniw = WP_QNodeCreate (WP_WINPATH (DEFAULT_WPID),
                          WP_QNODE_IWQ | WP_QNODE_OPT_FMU, qn_iw);
   terminate_on_error (qniw, "WP_QNodeCreate() qniw", __LINE__);

   /* Create an IW Host port */
   iwp1 = WP_PortCreate (WP_WINPATH (DEFAULT_WPID), WP_PORT_IW_HOST, NULL);
   terminate_on_error (iwp1, "WP_PortCreate() IW Host", __LINE__);

   /* Create an IW Host Device */
   devhost = WP_DeviceCreate (iwp1, 0, WP_DEVICE_IW_HOST, NULL);
   terminate_on_error (devhost, "WP_DeviceCreate() IW Host", __LINE__);

   /* Create an GEnet Port (the device that use "regular" mode) */

   enet_s_fast->flowmode = WP_ENET_FMU_HIERARCHICAL_SHAPING_MODE;
#if (WT_TRIAL_MASK == 0x4)
   /*
    * set up the XGI1
    */
   enet_s_fast->interface_mode = WP_ENET_XAUI;
   gbe_port1 =
      WP_PortCreate (WP_WINPATH (DEFAULT_WPID), WP_PORT_XGI1, enet_s_fast);
   terminate_on_error (gbe_port1, "WP_PortCreate()", __LINE__);

   printf ("_________________\n");
   printf ("XGI-1 created\n");
#else
#error XGI_should_be_defined
   enet_s_fast->interface_mode = WP_ENET_SGMII_1000;
   gbe_port1 =
      WP_PortCreate (WP_WINPATH (DEFAULT_WPID), WP_PORT_ENET13,
                     enet_s_fast);
   terminate_on_error (gbe_port1, "WP_PortCreate()", __LINE__);
   printf ("ENET-13 created!\n");
#endif

   /*
    * ENET14 is of no use
    */
   enet_s_fast->interface_mode = WP_ENET_SGMII_1000;

   enet_s_fast->flowmode = WP_FLOWMODE_FAST;
   gbe_port2 =
      WP_PortCreate (WP_WINPATH (DEFAULT_WPID), WP_PORT_ENET14,
                     enet_s_fast);
   terminate_on_error (gbe_port2, "WP_PortCreate()", __LINE__);
   printf ("_________________\n");
   printf ("ENET-14 created!\n");

   /* Create an GEnet Device (the device that use "regular" mode) */
   enet_dev->max_tx_channels = MAX_NUM_OF_CHANNELS;

   enet_dev->extended_params = NULL;
   gbe_dev1 = WP_DeviceCreate (gbe_port1, 0, WP_DEVICE_ENET, enet_dev);
   terminate_on_error (gbe_dev1, "WP_DeviceCreate() GENET13", __LINE__);

   enet_dev->max_tx_channels = MAX_NUM_OF_CHANNELS /* 8 */ ;
   enet_dev->extended_params = enet_device_extended_config;

/**************************************************************************/
/* Set extended parameters for enabling the Multi-Class mode */
   enet_device_extended_config->tx_flowcontrol =
      WP_ENET_FLOW_MULTI_CLASS_CBFC_TIMERS;

   multi_class_config->multi_class_mode =
      WP_ENET_MULTI_CLASS_EXTERNAL_ONLY;
   multi_class_config->max_multi_class_devices = NUM_OF_FLOWS;
   enet_device_extended_config->multi_class_config = multi_class_config;

   enet_dev->extended_params = enet_device_extended_config;

   status = WP_DeviceModify (gbe_dev1,
                             WP_DEV_MOD_ENET_TX_STATMODE |
                             WP_DEV_MOD_ENET_RX_STATMODE |
                             WP_DEV_MOD_ENET_TX_FLOWCONTROL |
                             WP_DEV_MOD_ENET_MULTI_CLASS_CONFIG, enet_dev);

   terminate_on_error (status, "WP_DeviceModify() GENET13", __LINE__);

   for (ii = 0; ii < NUM_OF_FLOWS /* 8 */ ; ii++)
   {

      MultiClass_Device_h[ii] =
         WP_DeviceCreate (gbe_dev1, WP_PHY (0),
                          WP_DEVICE_ENET_MULTI_CLASS,
                          enet_multi_class_dev_cfg);

      terminate_on_error (MultiClass_Device_h[ii],
                          "WP_DeviceCreate() MultiClass", __LINE__);
   }

/**************************************************************************/

#if 0
   status = WP_DeviceModify (gbe_dev1,
                             WP_DEV_MOD_ENET_TX_STATMODE |
                             WP_DEV_MOD_ENET_RX_STATMODE |
                             WP_DEV_MOD_ENET_RX_FLOWCONTROL |
                             WP_DEV_MOD_ENET_TX_FLOWCONTROL, enet_dev);
   terminate_on_error (status, "WP_DeviceModify() GENET13", __LINE__);
#endif

   enet_dev->extended_params = NULL;
   /*enet_dev->loopbackmode = WP_ENET_LOOPBACK; */
   gbe_dev2 = WP_DeviceCreate (gbe_port2, 0, WP_DEVICE_ENET, enet_dev);
   terminate_on_error (gbe_dev2, "WP_DeviceCreate() GENET14", __LINE__);

   enet_dev->max_tx_channels = MAX_NUM_OF_CHANNELS;

   enet_device_extended_config->tx_flowcontrol = WP_ENET_FLOW_DISABLE;
   enet_device_extended_config->rx_flowcontrol = WP_ENET_FLOW_DISABLE;
   enet_device_extended_config->multi_class_config = NULL;

   enet_dev->extended_params = enet_device_extended_config;

   status = WP_DeviceModify (gbe_dev2,
                             WP_DEV_MOD_ENET_TX_STATMODE |
                             WP_DEV_MOD_ENET_RX_STATMODE |
                             WP_DEV_MOD_ENET_RX_FLOWCONTROL |
                             WP_DEV_MOD_ENET_TX_FLOWCONTROL, enet_dev);
   terminate_on_error (status, "WP_DeviceModify() GENET14", __LINE__);

}

static void terminate_on_error (WP_handle handle, WP_CHAR * s, WP_U32 line)
{
   if (WP_ERROR_P (handle))
   {
      printf ("%s: Terminal error (handle 0x%x) %s\n", appname, handle, s);
      printf ("%s: Error type - %s\n", appname, WP_ErrorString (handle));
      printf ("line = %d\n", line);
      printf ("Test Failed\n");

      WP_DriverRelease ();
#if 0
      WT_Reboot ();
#else
      printf ("terminate_on_error: we stop, instead of reboot\n");
#endif
      exit (0);
   }
}

static void terminate (WP_U32 line)
{
   WP_DriverRelease ();
   // WT_Reboot();
   exit (0);
}

/**
 * Displays Bridge Port Statistics.

*/

WP_U32 WPT_LocalDisplayDeviceStats (WP_handle enet_handle)
{
   WP_stats_enet enet_stats;
   WP_status status;

   memset (&enet_stats, 0, sizeof (WP_stats_enet));

   status = WP_DeviceStatistics (enet_handle, &enet_stats);
   if (status)
   {
      printf ("Error Reading Enet Statistics\n");
      exit (1);
   }
   printf ("HS ENET Device Statistics (DPS)\n");

   printf ("Rx Host Frames:              %08x%08x\n",
           ((WPT_StatField *) & enet_stats.rx_host_frames)->part.high,
           ((WPT_StatField *) & enet_stats.rx_host_frames)->part.low);
   printf ("Rx Iw Frames:                %08x%08x\n",
           ((WPT_StatField *) & enet_stats.rx_iw_frames)->part.high,
           ((WPT_StatField *) & enet_stats.rx_iw_frames)->part.low);
   printf ("Rx Error Host Full:          %08x%08x\n",
           ((WPT_StatField *) & enet_stats.rx_err_host_full)->part.
           high,
           ((WPT_StatField *) & enet_stats.rx_err_host_full)->part.low);
   printf ("Rx Error Fbp Underrun:       %08x%08x\n",
           ((WPT_StatField *) & enet_stats.rx_err_fbp_underrun)->
           part.high,
           ((WPT_StatField *) & enet_stats.rx_err_fbp_underrun)->part.low);
   printf ("Rx Error Nonvalid Mac:       %08x%08x\n",
           ((WPT_StatField *) & enet_stats.rx_err_nonvalid_mac)->
           part.high,
           ((WPT_StatField *) & enet_stats.rx_err_nonvalid_mac)->part.low);
   printf ("Rx Error Mru:                %08x%08x\n",
           ((WPT_StatField *) & enet_stats.rx_err_mru)->part.high,
           ((WPT_StatField *) & enet_stats.rx_err_mru)->part.low);
   printf ("Rx Error Sdu:                %08x%08x\n",
           ((WPT_StatField *) & enet_stats.rx_err_sdu)->part.high,
           ((WPT_StatField *) & enet_stats.rx_err_sdu)->part.low);
   printf ("Tx Error Underrun:           %08x%08x\n",
           ((WPT_StatField *) & enet_stats.tx_err_underrun)->part.
           high,
           ((WPT_StatField *) & enet_stats.tx_err_underrun)->part.low);
   printf ("Rx Error Overrun:            %08x%08x\n",
           ((WPT_StatField *) & enet_stats.rx_err_overrun)->part.high,
           ((WPT_StatField *) & enet_stats.rx_err_overrun)->part.low);
   printf ("Tx Frames:                   %08x%08x\n",
           ((WPT_StatField *) & enet_stats.tx_frames)->part.high,
           ((WPT_StatField *) & enet_stats.tx_frames)->part.low);
   printf ("----------------------------------------------------\n");
   printf ("\n----------------------------------------------------\n");

   printf ("ENET Port Statistics (HW)\n");
#if 1

   printf ("Tx Defer:                    %08x%08x\n",
           ((WPT_StatField *) & enet_stats.tx_defer)->part.high,
           ((WPT_StatField *) & enet_stats.tx_defer)->part.low);

   printf ("Rx Bytes:                    %08x%08x\n",
           ((WPT_StatField *) & enet_stats.rx_bytes)->part.high,
           ((WPT_StatField *) & enet_stats.rx_bytes)->part.low);

   printf ("Tx Exess Defer:              %08x%08x\n",
           ((WPT_StatField *) & enet_stats.tx_excess_defer)->part.
           high,
           ((WPT_StatField *) & enet_stats.tx_excess_defer)->part.low);

   printf ("Rx Packets:                  %08x%08x\n",
           ((WPT_StatField *) & enet_stats.rx_packets)->part.high,
           ((WPT_StatField *) & enet_stats.rx_packets)->part.low);

   printf ("Tx Single Collision:         %08x%08x\n",
           ((WPT_StatField *) & enet_stats.tx_single_collision)->
           part.high,
           ((WPT_StatField *) & enet_stats.tx_single_collision)->part.low);

   printf ("Rx Error FCS:                %08x%08x\n",
           ((WPT_StatField *) & enet_stats.rx_err_fcs)->part.high,
           ((WPT_StatField *) & enet_stats.rx_err_fcs)->part.low);

   printf ("Tx Multi Collision:          %08x%08x\n",
           ((WPT_StatField *) & enet_stats.tx_multi_collision)->part.
           high,
           ((WPT_StatField *) & enet_stats.tx_multi_collision)->part.low);

   printf ("Rx Multicast:                %08x%08x\n",
           ((WPT_StatField *) & enet_stats.rx_multicast)->part.high,
           ((WPT_StatField *) & enet_stats.rx_multicast)->part.low);

   printf ("Tx Late Collision:           %08x%08x\n",
           ((WPT_StatField *) & enet_stats.tx_late_collision)->part.
           high,
           ((WPT_StatField *) & enet_stats.tx_late_collision)->part.low);

   printf ("Rx Broadcast:                %08x%08x\n",
           ((WPT_StatField *) & enet_stats.rx_broadcast)->part.high,
           ((WPT_StatField *) & enet_stats.rx_broadcast)->part.low);

   printf ("Tx Excess Collision:         %08x%08x\n",
           ((WPT_StatField *) & enet_stats.tx_excess_collision)->
           part.high,
           ((WPT_StatField *) & enet_stats.tx_excess_collision)->part.low);

   printf ("Rx Mac Control:              %08x%08x\n",
           ((WPT_StatField *) & enet_stats.rx_mac_control)->part.high,
           ((WPT_StatField *) & enet_stats.rx_mac_control)->part.low);

   printf ("Tx No Collision:             %08x%08x\n",
           ((WPT_StatField *) & enet_stats.tx_no_collision)->part.
           high,
           ((WPT_StatField *) & enet_stats.tx_no_collision)->part.low);

   printf ("Rx Mac Pause:                %08x%08x\n",
           ((WPT_StatField *) & enet_stats.rx_mac_pause)->part.high,
           ((WPT_StatField *) & enet_stats.rx_mac_pause)->part.low);

   printf ("Tx Mac Pause Honored:        %08x%08x\n",
           ((WPT_StatField *) & enet_stats.tx_mac_pause_honored)->
           part.high,
           ((WPT_StatField *) & enet_stats.tx_mac_pause_honored)->
           part.low);

   printf ("Rx Mac Unknown:              %08x%08x\n",
           ((WPT_StatField *) & enet_stats.rx_mac_unknown)->part.high,
           ((WPT_StatField *) & enet_stats.rx_mac_unknown)->part.low);

   printf ("Tx Dropped:                  %08x%08x\n",
           ((WPT_StatField *) & enet_stats.tx_dropped)->part.high,
           ((WPT_StatField *) & enet_stats.tx_dropped)->part.low);

   printf ("Rx Error Alignment:          %08x%08x\n",
           ((WPT_StatField *) & enet_stats.rx_err_alignment)->part.
           high,
           ((WPT_StatField *) & enet_stats.rx_err_alignment)->part.low);

   printf ("Tx Jabber:                   %08x%08x\n",
           ((WPT_StatField *) & enet_stats.tx_jabber)->part.high,
           ((WPT_StatField *) & enet_stats.tx_jabber)->part.low);

   printf ("Rx Error LEN:                %08x%08x\n",
           ((WPT_StatField *) & enet_stats.rx_err_length)->part.high,
           ((WPT_StatField *) & enet_stats.rx_err_length)->part.low);

   printf ("Tx Errors FCS:               %08x%08x\n",
           ((WPT_StatField *) & enet_stats.tx_err_fcs)->part.high,
           ((WPT_StatField *) & enet_stats.tx_err_fcs)->part.low);

   printf ("Rx Error Code:               %08x%08x\n",
           ((WPT_StatField *) & enet_stats.rx_err_code)->part.high,
           ((WPT_StatField *) & enet_stats.rx_err_code)->part.low);

   printf ("Tx Control:                  %08x%08x\n",
           ((WPT_StatField *) & enet_stats.tx_control)->part.high,
           ((WPT_StatField *) & enet_stats.tx_control)->part.low);

   printf ("Rx False Carrier:            %08x%08x\n",
           ((WPT_StatField *) & enet_stats.rx_false_carrier)->part.
           high,
           ((WPT_StatField *) & enet_stats.rx_false_carrier)->part.low);

   printf ("Tx Oversize:                 %08x%08x\n",
           ((WPT_StatField *) & enet_stats.tx_oversize)->part.high,
           ((WPT_StatField *) & enet_stats.tx_oversize)->part.low);

   printf ("Rx Undersize:                %08x%08x\n",
           ((WPT_StatField *) & enet_stats.rx_undersize)->part.high,
           ((WPT_StatField *) & enet_stats.rx_undersize)->part.low);

   printf ("Tx Undersize:                %08x%08x\n",
           ((WPT_StatField *) & enet_stats.tx_undersize)->part.high,
           ((WPT_StatField *) & enet_stats.tx_undersize)->part.low);

   printf ("Rx Oversize:                 %08x%08x\n",
           ((WPT_StatField *) & enet_stats.rx_oversize)->part.high,
           ((WPT_StatField *) & enet_stats.rx_oversize)->part.low);

   printf ("Tx Fragments:                %08x%08x\n",
           ((WPT_StatField *) & enet_stats.tx_fragments)->part.high,
           ((WPT_StatField *) & enet_stats.tx_fragments)->part.low);

   printf ("Rx Fragments:                %08x%08x\n",
           ((WPT_StatField *) & enet_stats.rx_fragments)->part.high,
           ((WPT_StatField *) & enet_stats.rx_fragments)->part.low);
   printf ("Rx Jabber:                   %08x%08x\n",
           ((WPT_StatField *) & enet_stats.rx_jabber)->part.high,
           ((WPT_StatField *) & enet_stats.rx_jabber)->part.low);
   printf ("Rx Dropped:                  %08x%08x\n",
           ((WPT_StatField *) & enet_stats.rx_dropped)->part.high,
           ((WPT_StatField *) & enet_stats.rx_dropped)->part.low);

   printf ("TxRx Frame  64:              %08x%08x\n",
           ((WPT_StatField *) & enet_stats.txrx_frames_64)->part.high,
           ((WPT_StatField *) & enet_stats.txrx_frames_64)->part.low);

   printf ("Tx Bytes:                    %08x%08x\n",
           ((WPT_StatField *) & enet_stats.tx_bytes)->part.high,
           ((WPT_StatField *) & enet_stats.tx_bytes)->part.low);

   printf ("----------------------------------------------------\n");
   printf ("TxRx Frame 127:              %08x%08x\n",
           ((WPT_StatField *) & enet_stats.txrx_frames_127)->part.
           high,
           ((WPT_StatField *) & enet_stats.txrx_frames_127)->part.low);

   printf ("Tx Packets:                  %08x%08x\n",
           ((WPT_StatField *) & enet_stats.tx_packets)->part.high,
           ((WPT_StatField *) & enet_stats.tx_packets)->part.low);

   printf ("TxRx Frame 255:              %08x%08x\n",
           ((WPT_StatField *) & enet_stats.txrx_frames_255)->part.
           high,
           ((WPT_StatField *) & enet_stats.txrx_frames_255)->part.low);

   printf ("Tx Multicast:                %08x%08x\n",
           ((WPT_StatField *) & enet_stats.tx_multicast)->part.high,
           ((WPT_StatField *) & enet_stats.tx_multicast)->part.low);

   printf ("TxRx Frame 1023:             %08x%08x\n",
           ((WPT_StatField *) & enet_stats.txrx_frames_1023)->part.
           high,
           ((WPT_StatField *) & enet_stats.txrx_frames_1023)->part.low);

   printf ("Tx Broadcast:                %08x%08x\n",
           ((WPT_StatField *) & enet_stats.tx_broadcast)->part.high,
           ((WPT_StatField *) & enet_stats.tx_broadcast)->part.low);

   printf ("TxRx Frame 1518:             %08x%08x\n",
           ((WPT_StatField *) & enet_stats.txrx_frames_1518)->part.
           high,
           ((WPT_StatField *) & enet_stats.txrx_frames_1518)->part.low);

   printf ("Tx Mac Pause:                %08x%08x\n",
           ((WPT_StatField *) & enet_stats.tx_mac_pause)->part.high,
           ((WPT_StatField *) & enet_stats.tx_mac_pause)->part.low);

   printf ("TxRx Frame 1522:             %08x%08x\n",
           ((WPT_StatField *) & enet_stats.txrx_frames_1522)->part.
           high,
           ((WPT_StatField *) & enet_stats.txrx_frames_1522)->part.low);

#endif

   return 0;
}

void WPT_DisplayRoutingPortStats (WP_handle port_handle, WP_CHAR * type)
{
   WP_route_port_stats rport_stats = { 0 };
   WP_status status = 0;

   memset (&rport_stats, 0, sizeof (WP_route_port_stats));
   status = WP_IwPortStatistics (port_handle, &rport_stats);
   terminate_on_error (status, "WP_IwPortStatistics()", __LINE__);
   printf ("\n%s Route port statistics of port %d (%x)\n", type,
           (port_handle & 0x000000ff), port_handle);
   printf ("--------- GBE Port -----------\n");
   printf ("rx_valid_packets = %d\n",
           (WP_U32) rport_stats.rx_valid_packets);
   printf ("rx_ipv4_option_packets = %d\n",
           (WP_U32) rport_stats.rx_ipv4_option_packets);
   printf ("rx_non_ip_packets = %d\n",
           (WP_U32) rport_stats.rx_non_ip_packets);
   printf ("rx_compressed_packets = %d\n",
           (WP_U32) rport_stats.rx_compressed_packets);
   printf ("rx_valid_mpls_packets = %d\n",
           (WP_U32) rport_stats.rx_valid_mpls_packets);
   printf ("rx_protocol_error = %d\n",
           (WP_U32) rport_stats.rx_protocol_error);
   printf ("rx_checksum_error = %d\n",
           (WP_U32) rport_stats.rx_checksum_error);
   printf ("rx_discard_classifier = %d\n",
           (WP_U32) rport_stats.rx_discard_classifier);
   printf ("rx_mpls_lookup_drop = %d\n",
           (WP_U32) rport_stats.rx_mpls_lookup_drop);
   printf ("tx_forwarded_packets = %d\n",
           (WP_U32) rport_stats.tx_forwarded_packets);
   printf ("\n");
}

static void WPE_SystemEnable (void)
{
   WP_status status;
   WP_U32 ii;

   /* Enable the L2pi Port (for fast "regular") */
   status = WP_PortEnable (gbe_port1, WP_DIRECTION_DUPLEX);
   terminate_on_error (status, "WP_PortEnable gbe_port1", __LINE__);

   /* Enable the L2pi Port (for fast "regular") */
   status = WP_PortEnable (gbe_port2, WP_DIRECTION_DUPLEX);
   terminate_on_error (status, "WP_PortEnable gbe_port2", __LINE__);

   /* L2pi-Enet To L2pi-Enet */
   status = WP_DeviceEnable (gbe_dev1, WP_DIRECTION_DUPLEX);
   terminate_on_error (status, "WP_DeviceEnable gbe_dev1", __LINE__);

/* enable multiclass devices, only TX direction */

   for (ii = 0; ii < NUM_OF_FLOWS; ii++)
   {
      status = WP_DeviceEnable (MultiClass_Device_h[ii], WP_DIRECTION_TX);
      terminate_on_error (status,
                          "WP_DeviceEnable MultiClass_Device_h", __LINE__);
   }

   status = WP_DeviceEnable (gbe_dev2, WP_DIRECTION_DUPLEX);
   terminate_on_error (status, "WP_DeviceEnable gbe_dev2", __LINE__);

   status = WP_ChannelEnable (gbe1_rx_ch_handle);
   terminate_on_error (status, "WP_ChannelEnable() rx_enet", __LINE__);

   for (ii = 0; ii < NUM_OF_FLOWS; ii++)
   {
      status = WP_ShapingGroupEnable (l1_group_h[ii]);
      terminate_on_error (status, "WP_ShapingGroupEnable l1", __LINE__);

      status = WP_ShapingGroupEnable (l2_group_h[ii]);
      terminate_on_error (status, "WP_ShapingGroupEnable l1", __LINE__);

      status = WP_ChannelEnable (MultiClass_Tx_Channel_h[ii]);
      terminate_on_error (status, "WP_ChannelEnable() tx_enet", __LINE__);
   }

   status = WP_ChannelEnable (gbe2_rx_ch_handle);
   terminate_on_error (status, "WP_ChannelEnable() rx_enet", __LINE__);

   status = WP_ChannelEnable (gbe2_tx_ch_handle);
   terminate_on_error (status, "WP_ChannelEnable() tx_enet", __LINE__);

#if TEST_LOOPBACK_MODE
   status = WP_ChannelEnable (host_channel_psu_2[0]);
   terminate_on_error (status, "WP_ChannelEnable() rx host Enet Psu",
                       __LINE__);
#endif
}

static void WPE_DlForwardingTableCreate (void)
{
   WP_U32 flow_index = 0;

   for (flow_index = 0; flow_index < NUM_OF_FLOWS; flow_index++)
   {
// #if WTI_REGRESSION_MODE
#if 0
      dl_tx_agg_gbe->txfunc = host_channel_psu_2[0];
#else
      dl_tx_agg_gbe->txfunc = MultiClass_Tx_Channel_h[flow_index];
#endif

      /* Update the Eth/IP/Udp Headers fields from IWF Table */
//       dl_tx_agg_gbe->prefix_header[WT_MAC_SA_OFFSET + 5] = 0x03;
//       dl_tx_agg_gbe->prefix_header[WT_MAC_DA_OFFSET + 5] = IWF_Table[flow_index][10];
//       dl_tx_agg_gbe->prefix_header[WT_MAC_HEADER_SIZE + WT_IPV4_SA_OFFSET] =
//         IWF_Table[flow_index][5] >> 24;
//       dl_tx_agg_gbe->prefix_header[WT_MAC_HEADER_SIZE + WT_IPV4_SA_OFFSET + 1] =
//         (IWF_Table[flow_index][5] >> 16) & 0xff;
//       dl_tx_agg_gbe->prefix_header[WT_MAC_HEADER_SIZE + WT_IPV4_SA_OFFSET + 2] =
//         (IWF_Table[flow_index][5] >> 8) & 0xff;
//       dl_tx_agg_gbe->prefix_header[WT_MAC_HEADER_SIZE + WT_IPV4_SA_OFFSET + 3] =
//         IWF_Table[flow_index][5] & 0xff;
//       dl_tx_agg_gbe->prefix_header[WT_MAC_HEADER_SIZE + WT_IPV4_DA_OFFSET] =
//         IWF_Table[flow_index][6] >> 24;
//       dl_tx_agg_gbe->prefix_header[WT_MAC_HEADER_SIZE + WT_IPV4_DA_OFFSET + 1] =
//         (IWF_Table[flow_index][6] >> 16) & 0xff;
//       dl_tx_agg_gbe->prefix_header[WT_MAC_HEADER_SIZE + WT_IPV4_DA_OFFSET + 2] =
//         (IWF_Table[flow_index][6] >> 8) & 0xff;
//       dl_tx_agg_gbe->prefix_header[WT_MAC_HEADER_SIZE + WT_IPV4_DA_OFFSET + 3] =
//         IWF_Table[flow_index][6] & 0xff;
//       dl_tx_agg_gbe->prefix_header[WT_MAC_HEADER_SIZE +
//                                    WT_IPV4_HEADER_SIZE +
//                                    WT_L4_SRC_PORT_OFFSET] = IWF_Table[flow_index][7] >> 8;
//       dl_tx_agg_gbe->prefix_header[WT_MAC_HEADER_SIZE +
//                                    WT_IPV4_HEADER_SIZE +
//                                    WT_L4_SRC_PORT_OFFSET + 1] = IWF_Table[flow_index][7] & 0xff;
//       dl_tx_agg_gbe->prefix_header[WT_MAC_HEADER_SIZE +
//                                    WT_IPV4_HEADER_SIZE +
//                                    WT_L4_DST_PORT_OFFSET] = IWF_Table[flow_index][8] >> 8;
//       dl_tx_agg_gbe->prefix_header[WT_MAC_HEADER_SIZE +
//                                    WT_IPV4_HEADER_SIZE +
//                                    WT_L4_DST_PORT_OFFSET + 1] = IWF_Table[flow_index][8] & 0xff;
//       dl_tx_agg_gbe->prefix_header[WT_MAC_HEADER_SIZE +
//                                    WT_IPV4_PROTOCOL_OFFSET] = IWF_Table[flow_index][9];

      flow_agg_add_header[flow_index] =
         WP_IwFlowAggregationCreate (WP_WINPATH
                                     (DEFAULT_WPID),
                                     WP_IW_DIRECTMAP_MODE, dl_tx_agg_gbe);
      terminate_on_error (flow_agg_add_header[flow_index],
                          "WP_IwFlowAggregationCreate() flow_agg_add_header",
                          __LINE__);
   }
}

static void WPE_ShapingGroupsCreate (void)
{

   WP_U32 ii;

   memset (l1_group_shaping_params, 0, sizeof (l1_group_shaping_params));
   l1_group_shaping_params[0].cir = 200000000;
   l1_group_shaping_params[0].cbs = 800000;
   l1_group_shaping_params[0].eir = 200000000;
   l1_group_shaping_params[0].ebs = 800000;
   l1_group_shaping_params[0].flags = 0;

   memset (packet_group_l1_config, 0, sizeof (packet_group_l1_config));
   packet_group_l1_config[0].group_level = WP_L1_GROUP;
   packet_group_l1_config[0].tx_shaping_type = WP_FMU_SHAPING_TYPE_CIR_EIR;
   packet_group_l1_config[0].tx_shaping_params =
      &l1_group_shaping_params[0];
   packet_group_l1_config[0].num_fifos = WP_NUM_FIFOS_8;
   packet_group_l1_config[0].block_level = 0;
   packet_group_l1_config[0].group_mode = WP_MODE_HW;

   memset (l2_group_shaping_params, 0, sizeof (l2_group_shaping_params));
   l2_group_shaping_params[0].cir = 200000000;
   l2_group_shaping_params[0].cbs = 800000;
   l2_group_shaping_params[0].eir = 200000000;
   l2_group_shaping_params[0].ebs = 800000;
   l2_group_shaping_params[0].flags = 0;

   memset (packet_group_l2_config, 0, sizeof (packet_group_l2_config));
   packet_group_l2_config[0].group_level = WP_L2_GROUP;
   packet_group_l2_config[0].tx_shaping_type = WP_FMU_SHAPING_TYPE_CIR_EIR;
   packet_group_l2_config[0].tx_shaping_params =
      &l2_group_shaping_params[0];
   packet_group_l2_config[0].num_fifos = WP_NUM_FIFOS_8;
   packet_group_l2_config[0].block_level = 0;
   packet_group_l2_config[0].group_mode = WP_MODE_HW;

   for (ii = 0; ii < NUM_OF_FLOWS; ii++)
   {

      l1_group_h[ii] =
         WP_ShapingGroupCreate (MultiClass_Device_h[ii],
                                WP_SHAPING_GROUP_TYPE_ENET,
                                &packet_group_l1_config[0]);

      terminate_on_error (l1_group_h[ii], "l1_group create", __LINE__);

      l2_group_h[ii] =
         WP_ShapingGroupCreate (l1_group_h[ii],
                                WP_SHAPING_GROUP_TYPE_ENET,
                                &packet_group_l2_config[0]);
      terminate_on_error (l2_group_h[ii], "l2_group[0] create", __LINE__);
   }

}

static void WPE_ChannelsCreate (void)
{
   WP_tag ch_tag = 0;
   WP_status status;
   WP_U32 ii;

   /* Create Enet1 fast ("reguler") RX channel */
   gbe1_rx_ch_handle =
      WP_ChannelCreate (ch_tag++, gbe_dev1, qniw, WP_CH_RX,
                        WP_ENET, &gbe_ch[0]);
   terminate_on_error (gbe1_rx_ch_handle,
                       "WP_ChannelCreate() RX_Channel ENET13", __LINE__);

/**************************************************************************/
   for (ii = 0; ii < NUM_OF_FLOWS; ii++)
   {
      MultiClass_Tx_Channel_h[ii] =
         WP_ChannelCreate (ch_tag++, l2_group_h[ii], qniw,
                           WP_CH_TX, WP_ENET, &gbe_ch[1]);
      terminate_on_error (MultiClass_Tx_Channel_h[ii],
                          "WP_ChannelCreate() TX_Channel  - MultiClass  ENET13",
                          __LINE__);
   }

/**************************************************************************/

   /* Create Enet2 fast ("reguler") RX channel */
   gbe2_rx_ch_handle =
      WP_ChannelCreate (ch_tag++, gbe_dev2, qniw, WP_CH_RX,
                        WP_ENET, &gbe_ch[0]);
   terminate_on_error (gbe2_rx_ch_handle,
                       "WP_ChannelCreate() RX_Channel ENET14", __LINE__);

   /* Create Enet2 fast ("reguler") TX channel */
   gbe2_tx_ch_handle =
      WP_ChannelCreate (ch_tag++, gbe_dev2, qniw, WP_CH_TX,
                        WP_ENET, &gbe_ch[0]);
   terminate_on_error (gbe2_tx_ch_handle,
                       "WP_ChannelCreate() TX_Channel ENET14", __LINE__);

   for (ii = 0; ii < NUM_OF_FLOWS; ii++)
   {
      status = WP_IwTxBindingCreate (MultiClass_Tx_Channel_h[ii],
                                     WP_IW_TX_BINDING,
                                     &tx_binding_l2pi[0]);
      terminate_on_error (status,
                          "WP_IwTxBindingCreate() TX ENET fast device",
                          __LINE__);
   }

   status = WP_IwTxBindingCreate (gbe2_tx_ch_handle, WP_IW_TX_BINDING,
                                  &tx_binding_l2pi[0]);
   terminate_on_error (status,
                       "WP_IwTxBindingCreate() TX ENET fast device",
                       __LINE__);

   /*create host rx channel */
   rx_host_handle =
      WP_ChannelCreate (0x1111, devhost, qniw, WP_CH_RX,
                        WP_IW_HOST, &ch_config_iw[0]);
   terminate_on_error (rx_host_handle, "WP_ChannelCreate() Host sys",
                       __LINE__);

   host_ch_cfg[0].tx_binding_config = tx_binding_cfg;

#if TEST_LOOPBACK_MODE
   /* L2pi-Enet To L2pi-Enet */
   host_channel_psu_2[0] =
      WP_ChannelCreate (ch_tag++, devhost, qniw, WP_CH_RX,
                        WP_IW_HOST, host_ch_cfg);
   terminate_on_error (host_channel_psu_2[0],
                       "WP_ChannelCreate() host_channel_psu_2", __LINE__);
#endif
}

static void WPE_DlPortsCreate (WP_handle iw_sys)
{
   WP_status status;

   /*   Build system1 - Rx GBE---> Tx GBE */

   br_agg_gbe->txfunc = rx_host_handle;
   default_agg =
      WP_IwFlowAggregationCreate (WP_WINPATH (DEFAULT_WPID),
                                  WP_IW_VLAN_AWARE_BRIDGING_MODE,
                                  &br_agg_gbe[0]);
   terminate_on_error (default_agg, "WP_IwFlowAggregationCreate()",
                       __LINE__);

   iwhost_bport_config->flow_agg = default_agg;
   bport_host = WP_IwPortCreate (iw_sys, &iwhost_bport_config[0]);
   terminate_on_error (bport_host, " WP_IwPortCreate()", __LINE__);

   br_agg_gbe->input_bport = bport_host;
   status = WP_IwFlowAggregationModify (default_agg,
                                        WP_IW_FLOW_AGG_B_MOD_BPORT,
                                        &br_agg_gbe[0]);
   terminate_on_error (status, " WP_IwFlowAggregationModify()", __LINE__);

   route_port_cfg.flow_agg = default_agg;
   route_port_cfg.tag = 0;
   route_port_cfg.forwarding_mode = WP_IW_FORWARDING_DIRECT_MAP;

   h_iw_port_gbe_1 = WP_IwPortCreate (iw_sys, &route_port_cfg);
   terminate_on_error (h_iw_port_gbe_1, "WP_IwPortCreate", __LINE__);
}

static void WPE_DlRxBindingCreate (WP_handle iw_sys)
{
   WP_status status;

   /*   Build system1 - Rx GBE ---> Tx GBE */

   rx_binding_enet_routing[0].input_port = h_iw_port_gbe_1;
   rx_binding_enet_routing[0].pce_if_handle = 0;
   status = WP_IwRxBindingCreate (gbe1_rx_ch_handle, iw_sys, qniw,
                                  &rx_binding_enet_routing[0]);
   terminate_on_error (status, "WP_IwRxBindingCreate Enet", __LINE__);
}

/*************************************************/
/*                                               */
/* PCE WDDI data test items creation             */
/*                                               */
/*************************************************/

#if PCE_DEBUG
void WPE_PrintPceGlobalStats (void)
{
   WP_U32 temp1, temp2;

   WP_PceGlobalStatistics (WP_WINPATH (DEFAULT_WPID), &pce_global_stats);

   printf ("PCE Global Statistics\n");

   temp1 = pce_global_stats.parser_header_access_errors;
   temp2 = pce_global_stats.parser_header_access_errors >> 32;
   printf ("parser_header_access_errors %08X %08X\n", temp1, temp2);

   temp1 = pce_global_stats.parser_timeout_err;
   temp2 = pce_global_stats.parser_timeout_err >> 32;
   printf ("parser_timeout_err          %08X %08X\n", temp1, temp2);

   temp1 = pce_global_stats.classifier_timeout_err;
   temp2 = pce_global_stats.classifier_timeout_err >> 32;
   printf ("classifier_timeout_err      %08X %08X\n", temp1, temp2);

   temp1 = pce_global_stats.parser_accept;
   temp2 = pce_global_stats.parser_accept >> 32;
   printf ("parser_accept               %08X %08X\n", temp1, temp2);

   printf ("\n\n\n");
}
#else
#error PCE_DEBUG_NOT_DEFINED
#endif

static void WPE_CreateDataPceFilters (void)
{
   WP_pce_filter_classification filter_class = { 0 };

   /* Common parameters for all filters */
   filter_class.no_match_action = WP_PCE_FILTER_NO_MATCH_DENY;
   filter_class.no_fields_action = WP_PCE_FILTER_NO_FIELDS_DENY;
   filter_class.no_match_result[0].result_type = WP_PCE_RESULT_LAST;

   filter_class.filter_fields[0].field_mode =
      WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_class.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;
   filter_class.filter_fields[0].field_id = WP_PCE_FIELD_ID_IPV4_DA;

   filter_class.filter_fields[1].field_mode =
      WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_class.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;
   filter_class.filter_fields[1].field_id = WP_PCE_FIELD_ID_IPV4_SA;

   filter_class.filter_fields[2].field_mode =
      WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_class.filter_fields[2].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;
   filter_class.filter_fields[2].field_id = WP_PCE_FIELD_ID_L4_SRC_PORT;

   filter_class.filter_fields[3].field_mode =
      WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_class.filter_fields[3].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;
   filter_class.filter_fields[3].field_id = WP_PCE_FIELD_ID_L4_DST_PORT;

   filter_class.filter_fields[4].field_mode =
      WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_class.filter_fields[4].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;
   filter_class.filter_fields[4].field_id = WP_PCE_FIELD_ID_IPV4_PROTOCOL;

   filter_class.filter_fields[5].field_mode =
      WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_class.filter_fields[5].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;
   filter_class.filter_fields[5].field_id = WP_PCE_FIELD_ID_LAST;

        /*--------------------------------------------------------*\
                create filters here ...
                each rule is associated with a filter
        \*--------------------------------------------------------*/
   PCE_filter_data[0] =
      WP_PceFilterCreate (WP_WINPATH (DEFAULT_WPID),
                          WP_PCE_FILTER_CLASSIFICATION, &filter_class);
   terminate_on_error (PCE_filter_data[0], "WP_PceFilterCreate", __LINE__);
}

static void WPE_CreateFlowCtrlPceFilters (void)
{
   WP_pce_filter_flow_control filter_flow_ctrl = { 0 };
   WP_pce_filter_cbfc_class_id cbfc_class;
   WP_U32 i;

   /* Common parameters for all filters */
   filter_flow_ctrl.no_match_action = WP_PCE_FILTER_NO_MATCH_CONTINUE;
   filter_flow_ctrl.no_fields_action = WP_PCE_FILTER_NO_FIELDS_DENY;
   filter_flow_ctrl.no_match_result[0].result_type = WP_PCE_RESULT_LAST;

   filter_flow_ctrl.pause_frame_type = WP_PCE_FLOW_CONTROL_CBFC;
   filter_flow_ctrl.flow_control_pause_id = &cbfc_class;

   filter_flow_ctrl.filter_fields[0].field_mode =
      WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_flow_ctrl.filter_fields[0].mask_mode =
      WP_PCE_FIELD_MASK_NOT_USED;
   filter_flow_ctrl.filter_fields[0].field_id = WP_PCE_FIELD_ID_FLOW_CONTROL_BITS;  // only used in flow control

   filter_flow_ctrl.filter_fields[1].field_id = WP_PCE_FIELD_ID_DEVICE; /*Rx CFU Port */
   filter_flow_ctrl.filter_fields[1].field_mode =
      WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_flow_ctrl.filter_fields[1].mask_mode =
      WP_PCE_FIELD_MASK_NOT_USED;

   filter_flow_ctrl.filter_fields[2].field_mode =
      WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_flow_ctrl.filter_fields[2].mask_mode =
      WP_PCE_FIELD_MASK_NOT_USED;
   filter_flow_ctrl.filter_fields[2].field_id = WP_PCE_FIELD_ID_LAST;   // indication of the last id entry

   for (i = 0; i < NUM_OF_FLOWS; i++)
   {

      cbfc_class.class_id = 15 - i;

        /*--------------------------------------------------------*\
                create filters here ...
                each rule is associated with a filter
        \*--------------------------------------------------------*/
      PCE_filter_flow_ctrl[i] =
         WP_PceFilterCreate (WP_WINPATH (DEFAULT_WPID),
                             WP_PCE_FILTER_FLOW_CONTROL,
                             &filter_flow_ctrl);
      terminate_on_error (PCE_filter_flow_ctrl[i],
                          "WP_PceFilterCreate", __LINE__);
   }
}

static void WPE_CreateFlowCtrlPceRules (void)
{
   WP_pce_rule_classification rule_cfg;
   WP_U32 flow_index;
   WP_U32 rule_index;

   /* Common parameters */
   rule_cfg.enabled = WP_ENABLE;

   for (flow_index = 0; flow_index < NUM_OF_FLOWS; flow_index++)
   {
      rule_index = flow_index;
      rule_cfg.filter_handle = PCE_filter_flow_ctrl[flow_index];

      rule_cfg.match_action = WP_PCE_RULE_MATCH_CONTINUE;

      /* 
       * WP_PCE_FIELD_ID_FLOW_CONTROL_BITS
       */
      rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_CONTROL;
      rule_cfg.match_result[0].param.flow_control.
         result_flow_control = WP_PCE_RESULT_FLOW_CONTROL_TYPE_TIMER;
      rule_cfg.match_result[0].param.flow_control.counter_id = flow_index;
      rule_cfg.match_result[0].param.flow_control.pause_handle =
         MultiClass_Device_h[flow_index];

      rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_FLOW_CONTROL_BITS;
      rule_cfg.rule_fields[0].value.flow_control_mode = 1;

      /* 
       * WP_PCE_FIELD_ID_DEVICE
       */
      rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_DEVICE;
      rule_cfg.rule_fields[1].value.device_handle = gbe_dev1;
      rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;

      /* 
       * WP_PCE_FIELD_ID_LAST
       */
      rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_LAST;

        /*--------------------------------------------------------*\
                create rules here ...
                each rule is associated with a filter
        \*--------------------------------------------------------*/
      PCE_rule_flow_ctrl_h[rule_index] =
         WP_PceRuleCreate (WP_WINPATH (DEFAULT_WPID),
                           WP_PCE_RULE_FLOW_CONTROL, &rule_cfg);
      terminate_on_error (PCE_rule_flow_ctrl_h[rule_index],
                          "WP_PceRuleCreate()", __LINE__);

#if DEBUG
      printf ("PCE_rule_flow_ctrl_h[%d]\n", rule_index);
      WP_Display (WP_WINPATH (DEFAULT_WPID),
                  PCE_rule_flow_ctrl_h[rule_index], 0, 0);
#endif
   }
}

static void WPE_CreateDataPceRules (void)
{
   WP_pce_rule_flow_control rule_cfg;
   WP_U32 flow_index;
   WP_U32 rule_index;

   /* Common parameters */
   rule_cfg.enabled = WP_ENABLE;

   for (flow_index = 0; flow_index < NUM_OF_FLOWS; flow_index++)
   {
      rule_index = flow_index;
      rule_cfg.filter_handle = PCE_filter_data[0];

      rule_cfg.match_action = WP_PCE_RULE_MATCH_CONTINUE;

      rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
      rule_cfg.match_result[0].param.flow_agg.flow_aggregation =
         flow_agg_add_header[flow_index];
      rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;

      rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_IPV4_DA;
      rule_cfg.rule_fields[0].value.ipv4_addr = IWF_Table[flow_index][1];

      rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_IPV4_SA;
      rule_cfg.rule_fields[1].value.ipv4_addr = IWF_Table[flow_index][0];

      rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_L4_SRC_PORT;
      rule_cfg.rule_fields[2].value.l4_port = IWF_Table[flow_index][2];

      rule_cfg.rule_fields[3].field_id = WP_PCE_FIELD_ID_L4_DST_PORT;
      rule_cfg.rule_fields[3].value.l4_port = IWF_Table[flow_index][3];

      rule_cfg.rule_fields[4].field_id = WP_PCE_FIELD_ID_IPV4_PROTOCOL;
      rule_cfg.rule_fields[4].value.ipv4_prot = IWF_Table[flow_index][4];

      rule_cfg.rule_fields[5].field_id = WP_PCE_FIELD_ID_LAST;

        /*--------------------------------------------------------*\
                create rules here ...
                each rule is associated with a filter
        \*--------------------------------------------------------*/
      PCE_rule_data_h[rule_index] =
         WP_PceRuleCreate (WP_WINPATH (DEFAULT_WPID),
                           WP_PCE_RULE_CLASSIFICATION, &rule_cfg);
      terminate_on_error (PCE_rule_data_h[rule_index],
                          "WP_PceRuleCreate()", __LINE__);

   }
}

static void WPE_CreateFlowCtrlPceFilterSets (void)
{
   WP_U32 i;
   WP_pce_filter_set fs_level0;

   fs_level0.filter_set_level = 0;
   fs_level0.next_filter_set = WP_UNUSED;
   for (i = 0; i < NUM_FLOW_CTRL_FILTERS /*8 */ ; i++)
   {
      fs_level0.filters[i] = PCE_filter_flow_ctrl[i];
   }

   fs_level0.filters[NUM_FLOW_CTRL_FILTERS] = WP_UNUSED;
   flow_ctrl_filter_set_lvl0_handle =
      WP_PceFilterSetCreate (WP_WINPATH (DEFAULT_WPID), &fs_level0);
#if DEBUG
   printf ("\n\nFlow control filter set:\n");
   WP_Display (WP_WINPATH (DEFAULT_WPID),
               flow_ctrl_filter_set_lvl0_handle, 0, 0);
#endif

}

static void WPE_CreateDataPceFilterSets (void)
{
   WP_U32 i;
   WP_pce_filter_set fs_level0;

   fs_level0.filter_set_level = 0;
   fs_level0.next_filter_set = WP_UNUSED;
   for (i = 0; i < NUM_DATA_FILTERS; i++)
   {
      fs_level0.filters[i] = PCE_filter_data[i];
   }
   fs_level0.filters[NUM_DATA_FILTERS] = WP_UNUSED;
   data_filter_set_lvl0_handle =
      WP_PceFilterSetCreate (WP_WINPATH (DEFAULT_WPID), &fs_level0);

#if DEBUG
   printf ("\n\nData filter set:\n");
   WP_Display (WP_WINPATH (DEFAULT_WPID), data_filter_set_lvl0_handle,
               0, 0);
#endif
}

static void WPE_CreateDlPceRulesFiltersSets (WP_handle iw_sys)
{
   WP_pce_if_params_pkt_rx_channel pce_if_params = { 0 };
   WP_handle pce_if_handle;
   WP_status status;

   /*
    *      create filters here ...
    *      each rule is associated with a filter
    */
   WPE_CreateFlowCtrlPceFilters ();
   WPE_CreateDataPceFilters ();

   /*
    *      create rules here ...
    *      each rule is associated with a filter
    */
   WPE_CreateFlowCtrlPceRules ();
   WPE_CreateDataPceRules ();

   WPE_CreateFlowCtrlPceFilterSets ();
   WPE_CreateDataPceFilterSets ();

   /*
    *      create PCE interfaces
    */
   pce_if_params.mode = WP_PCE_IW_PORT_CONNECTION_ENABLED;
   pce_if_params.parser_start_type = WP_PCE_PARSER_START_TYPE_ETHERNET;
   pce_if_params.filter_set_handle = data_filter_set_lvl0_handle;
   pce_if_params.ip_header_validation = WP_DISABLE;

   pce_if_handle =
      WP_PceInterfaceCreate (WP_PCE_IF_TYPE_PKT_RX_CHANNEL,
                             &pce_if_params);

   terminate_on_error (pce_if_handle, "WP_PceInterfaceCreate()", __LINE__);

   /*
    * create PCE binding
    */
   rx_binding_enet_routing[0].pce_if_handle = pce_if_handle;

   status = WP_IwRxBindingModify (gbe1_rx_ch_handle, iw_sys, qniw,
                                  WP_IW_RX_BIND_MOD_PCE_INTERFACE,
                                  &rx_binding_enet_routing[0]);

   terminate_on_error (status, "WP_IwRxBindingModify", __LINE__);
   num_of_packets = 0;
}

static void WPE_HostSendFlowCtrl ()
{
   WP_data_segment km_tx_buf[3];
   WP_U8 km_data_ptr[3];
   WP_U8 tmp_buffer[2 * MAX_DATA_LENGTH];
   WP_data_unit data_unit;

   WP_data_segment *curr_buff_ptr;
   WP_status status;

   WP_CHAR SRC_MAC[100];
   WP_CHAR DST_MAC[100];

   WP_U32 temp_index;
   WP_U32 packet_index_per_channel[MAX_NUM_OF_CHANNELS];

   memset (&packet_index_per_channel, 0x00,
           sizeof (packet_index_per_channel));

   for (temp_index = 0; temp_index < 8 /*MAX_NUM_OF_PACKETS */ ;
        temp_index++)
   {
      WP_U32 packet_index = 0, channel_index = 0;

      packet_index = temp_index;
      channel_index = 0;

      /* ip protocol */
      sprintf (ip_protocol[channel_index][packet_index], "%02x",
               IWF_Table[temp_index][4]);

      payload_size[channel_index][packet_index] = 26;

#if DEBUG
      printf
         ("\nHost Send to: channel_index=%d, packet_index_per_channel=%d, payload_size=%d",
          channel_index, packet_index,
          payload_size[channel_index][packet_index]);
#endif

      sprintf (SRC_MAC, "000102030402");
      sprintf (DST_MAC, "000102030403");

      /* ipv4 source address */
      sprintf (ip_src[channel_index][packet_index], "%08x",
               IWF_Table[temp_index][0]);

      /* ipv4 destination address */
      sprintf (ip_dst[channel_index][packet_index], "%08x",
               IWF_Table[temp_index][1]);

      FlowCtrlDataUnitSetup (&data_unit, &km_tx_buf[1], &km_data_ptr[1]);

      WP_MEM_BYTES_GET (tmp_buffer, data_unit.segment->data,
                        data_unit.segment->data_size);

        /*-------------------------------------------------------------------*\
				we send here 
		\*-------------------------------------------------------------------*/
      status = WP_HostSend (MultiClass_Tx_Channel_h[0], &data_unit);

      terminate_on_error (status, "WP_HostSend()", __LINE__);
      curr_buff_ptr = data_unit.segment;

      data_unit.n_active = 0;
   }
}

static void WPE_HostSendTraffic ()
{
   WP_data_segment km_tx_buf[3];
   WP_U8 km_data_ptr[3];
   WP_U8 tmp_buffer[2 * MAX_DATA_LENGTH];
   WP_data_unit data_unit;

   WP_data_segment *curr_buff_ptr;
   WP_status status;

   WP_CHAR SRC_MAC[100];
   WP_CHAR DST_MAC[100];

   WP_U32 temp_index;
   WP_U32 packet_index_per_channel[MAX_NUM_OF_CHANNELS];

   memset (&packet_index_per_channel, 0x00,
           sizeof (packet_index_per_channel));

   for (temp_index = 0; temp_index < NUM_OF_FLOWS; temp_index++)
   {
      WP_U32 packet_index = 0, channel_index;

      channel_index = temp_index;

      /* ip protocol */
      sprintf (ip_protocol[channel_index][packet_index], "%02x",
               IWF_Table[temp_index][4]);

      payload_size[channel_index][packet_index] = 64;

#if DEBUG
      printf
         ("\nHost Send to: channel_index=%d, packet_index_per_channel=%d, payload_size=%d",
          channel_index, packet_index,
          payload_size[channel_index][packet_index]);
#endif

      sprintf (SRC_MAC, "000102030402");
      sprintf (DST_MAC, "000102030403");

      /* ipv4 source address */
      sprintf (ip_src[channel_index][packet_index], "%08x",
               IWF_Table[temp_index][0]);

      /* ipv4 destination address */
      sprintf (ip_dst[channel_index][packet_index], "%08x",
               IWF_Table[temp_index][1]);

      DataUnitSetup (&data_unit,
                     &km_tx_buf[1],
                     &km_data_ptr[1],
                     DST_MAC,
                     SRC_MAC,
                     ip_protocol[channel_index][packet_index],
                     ip_src[channel_index][packet_index],
                     ip_dst[channel_index][packet_index],
                     payload_size[channel_index][packet_index],
                     channel_index, packet_index);

      WP_MEM_BYTES_GET (tmp_buffer, data_unit.segment->data,
                        data_unit.segment->data_size);

                /*-------------------------------------------------------------------*\
				we send here 
		\*-------------------------------------------------------------------*/
      status = WP_HostSend (MultiClass_Tx_Channel_h
                            [channel_index], &data_unit);

      terminate_on_error (status, "WP_HostSend()", __LINE__);
      curr_buff_ptr = data_unit.segment;

      data_unit.n_active = 0;
   }
}

void WPE_CheckReceivedTraffic (WP_U32 expected_count)
{
   // WP_U32 index=0;
   WP_data_unit rx_data_unit;

   // WP_data_segment *curr_buff_ptr;
   WP_U32 packet_count = 0 /*, channel_index = 0 */ ;

   // WP_U8 tmp_buffer[2*MAX_DATA_LENGTH];
   WP_U32 ii, i;
   WP_status status;
   WP_U32 number_of_segments = 4;
   WP_data_segment data_segment[number_of_segments];

   scenario_failed = WP_FALSE;

   for (i = 0; i < WT_RX_LOOP_LIMIT; i++)
   {
      /* receive packet from host queue */
      for (ii = 0; ii < number_of_segments; ii++)
      {
         data_segment[ii].displacement = 0;
         data_segment[ii].data_size = 0;
         data_segment[ii].pool_handle = 0;
         data_segment[ii].data = 0;
         data_segment[ii].displacement = 0;
         if (ii == number_of_segments - 1)
            data_segment[ii].next = NULL;
         else
            data_segment[ii].next = &(data_segment[ii + 1]);
      }

      rx_data_unit.segment = (WP_data_segment *) & data_segment;

      rx_data_unit.n_segments = number_of_segments;
      rx_data_unit.type = WP_DATA_IW;
      rx_data_unit.n_active = 0;
      rx_data_unit.control = WP_HT_CONTROL (0);

                /*-------------------------------------------------------------------*\
				we got here 
		\*-------------------------------------------------------------------*/
      status = WP_HostReceive (host_channel_psu_2[0], &rx_data_unit);
      if (status != WP_OK)
      {
         if (WP_ERROR (status) != WP_ERR_HST_NO_DATA_TO_GET)
            terminate_on_error (status,
                                "WP_HostReceive host_channel", __LINE__);
      }
      else
      {
         WP_U32 offset = rx_data_unit.segment[0].data_size;

         for (ii = 1; ii < rx_data_unit.n_active; ii++)
         {
            memcpy (rx_data_unit.segment[0].data +
                    offset,
                    rx_data_unit.segment[ii].data,
                    rx_data_unit.segment[ii].data_size);
            rx_data_unit.segment[0].data_size +=
               rx_data_unit.segment[ii].data_size;
            offset += rx_data_unit.segment[ii].data_size;
         }

         packet_count++;
      }
   }

   if (packet_count != expected_count)
   {
      scenario_failed = WP_TRUE;
   }

}

WP_U16 WTI_IpChecksumCalc (WP_U8 * buff, WP_U16 len)
{
   WP_U16 bu16;
   WP_U32 sum = 0;
   WP_U32 i;

   /* make 16 bit words out of every two adjacent
      8 bit words in the packet and add them up */
   len = len / 2;
   for (i = 0; i < len; i++)
   {
      bu16 = (buff[2 * i] << 8) + buff[2 * i + 1];
      sum = sum + (WP_U32) bu16;
   }

   /* take only 16 bits out of the 32 bit sum
      and add up the carries */
   while (sum >> 16)
      sum = (sum & 0xFFFF) + (sum >> 16);

   /* one's complement the result */
   sum = ~sum;

   return ((WP_U16) sum);
}

void WTI_TranslateHexToAscii (WP_CHAR * Ascii, WP_CHAR * Hex,
                              WP_U32 length)
{
   WP_S32 i;
   WP_CHAR temp_num;

   for (i = 0; i < length; i++)
   {
      temp_num = (Hex[i] & 0xf0) >> 4;
      if (temp_num < 10)
         Ascii[2 * i] = temp_num + '0';
      else
         Ascii[2 * i] = temp_num + 'a' - 10;

      temp_num = Hex[i] & 0x0f;
      if (temp_num < 10)
         Ascii[2 * i + 1] = temp_num + '0';
      else
         Ascii[2 * i + 1] = temp_num + 'a' - 10;

   }

   Ascii[2 * length] = (WP_U32)NULL;

   return;
}

#if TEST_LOOPBACK_MODE
#endif

// #include "wt_util.c"
#include "wt_partition_ecc_util.c"
