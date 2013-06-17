/*******************************************************************
 * (C) Copyright 2011-2012, PMC-Sierra WIN Division. All rights reserved.
 * PMC-SIERRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of PMC-Sierra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 *******************************************************************/

/****************************************************************************
 *
 * Example: 38621-ufe_clear_channel_atm_pos.c
 * This test verifies the UFE ATM/POS Clear channel functionality.
 *
 * The test has various menu scenarios:
 *  1. ATM/HDLC(POS) host test (used for regression testing)
 *  2. ATM/POS interworking configured in the OC12/STM4 mode. This scenario
 *     uses the external tester for traffic generation and verification.
 *  3. ATM/POS interworking configured in the OC3/STM1 mode. This scenario
 *     uses the external tester for traffic generation and verification. It
 *     has two modes: single line (accepts line index) and all 4 lines together.
 *
 *  The Flexmux framer initialization functions are:
 *     TEST_FramerInitialize (set clocks, build_personality, flexmux driver)
 *     TEST_FramerLinePortInitialize (set framer line port facilities and line rate)
 *     TEST_FramerConnectionSet (create client facility and connect between line
 *                               port and the client)
 *     TEST_FramerLoopbackSet (defines various framer loopbacks)
 *
 ****************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef WP_HW_WINPATH2
#error "This test is not supported for WP2"
#endif

#if _WT_MODULE_TEST_COMMENT_ENABLE_
/* This test has been converted to work on Linux.  See
   WDDI_TestCodingGuidlines.doc.  */
#pragma check_tests pool_alloc 1
#endif

#ifdef USE_SOCKET
#include <unistd.h>
#include "sockint.h"
#include "wp_sim.h"
#endif
#include "wp_wddi.h"
#include "wp_host_io.h"

#include "wpx_app_data.h"

#include "wufe_ufe.h"
#include "wpx_board_data.h"

/* Specify which test features are selected.  */
#include "wt_util.h"
#include "wt_new_automation.c"

#if WT_TRIAL_MASK & (1 << 2)
#define TEST_CHECKIN
#endif

enum
{
   TEST_SDH = 0,
   TEST_SONET,
   TEST_OC12_CARD
};

enum
{
   TEST_SDH_STM1 = 0,
   TEST_SDH_STM4,
   TEST_SONET_OC3,
   TEST_SONET_OC12
};

enum
{
   TEST_UFE_412 = 0,
   TEST_UFE_448
};

enum
{
   TEST_NORMAL = 0,
   TEST_LINE_SHALLOW,
   TEST_LINE_DEEP,
   TEST_LINE_SDH,
   TEST_SYSTEM_SHALLOW,
   TEST_SYSTEM_DEEP
};

enum {
   TEST_IWCP_MPLS = 0,
   TEST_IWCP_BRIDGE,
   TEST_IWGP
};

enum {
   TEST_UFE412_REVA = 0,
   TEST_UFE412_REVB,
   TEST_UFE448
};

#define TEST_MAX_POS_FLOWS   4

enum {
   TEST_PKT_UNSHAPED = 0,
   TEST_PKT_PPR_SHAPING,
   TEST_PKT_CIR_SHAPING,
   TEST_PKT_CIR_EIR_SHAPING
};

struct test_config {
   int test_layer1_type;
   int test_line_type;
   int test_ufe4_type;
   int test_use_atm;
   int test_use_pos;
   int test_use_hspos;
   int test_ufe_clear_channel;
   int test_load_firmware;
   int test_use_framer;
   int test_hspos_use_iwgp;
   int test_pkt_scheduler_used;
   int test_atm_chans;
   int test_pos_flows;
   int test_psu_shaping_type;
   int test_use_snake_path;
   int test_iw_port_stats_display;
   int test_dual_emphy;
} test_config =
{
   /* test_layer1_type */         TEST_SDH,
   /* test_line_type */           TEST_SDH_STM4,
#if defined(__WT_UFE4_REVB__)
   /* test_ufe4_type */           TEST_UFE412_REVB,
#else
   /* test_ufe4_type */           TEST_UFE412_REVA,
#endif
   /* test_use_atm */             0,
   /* test_use_pos */             0,
   /* test_use_hspos */           0,
   /* test_ufe_clear_channel */   1, /* when cleared - used mostly for debugging */
   /* test_load_firmware */       1,
   /* test_use_framer */          1,
   /* test_hspos_use_iwgp */      0,
   /* pkt_scheduler_used */       0,
   /* test_atm_chans */           20,
   /* test_pos_flows */           1,
   /* test_psu_shaping_type */    TEST_PKT_UNSHAPED,
   /* test_use_snake_path */      0,
   /* test_iw_port_stats_display*/ 0,
   /* test_dual_emphy */           0
};

#if WT_TRIAL_MASK & (1 << 3)
#define TEST_PORT_IN_USE        WP_PORT_UPI3
#else
#define TEST_PORT_IN_USE        WP_PORT_UPI1
#endif

#if WT_TRIAL_MASK & (1 << 5)
#define TEST_ENET_PORT          WP_PORT_ENET14
#else
#define TEST_ENET_PORT          WP_PORT_ENET13
#endif

#if WT_TRIAL_MASK & (1 << 6)
#define TEST_USING_AAL5         0
#if WT_TRIAL_MASK & (1 << 7)
#define ENCAPSULATION  WP_PWE3_ATM_N_TO_1
#define TEST_USING_AAL0_GROUP 1
#define TEST_WITH_EGRESS_HASH_LOOKUP
#else
#define ENCAPSULATION  WP_PWE3_ATM_1_TO_1_VCC
#define TEST_USING_AAL0_GROUP 0
#endif
#else
#define TEST_USING_AAL5         1
#endif

#if WT_TRIAL_MASK & (1 << 8)
#define TEST_USING_HI_SPEED     1
#else
#define TEST_USING_HI_SPEED     0
#endif

#if defined(__WT_UFE4_DUAL_EMPHY__)
#define TEST_DUAL_EMPHY         1
#define N_UFE_DEVICES           2
#else
#define TEST_DUAL_EMPHY         0
#define N_UFE_DEVICES           1
#endif

#if WT_TRIAL_MASK & (1 << 7)
#if WT_TRIAL_MASK & (1 << 3)
#error "dual enet selection conflict"
#endif
#if WT_TRIAL_MASK & (1 << 5)
#error "dual enet selection conflict"
#endif
#if (N_UFE_DEVICES == 1)
#error "dual enet selection conflict"
#endif
#define N_ENET_DEVICES 2
#define TEST_ENET_PORT_2      WP_PORT_ENET14
#else
#define N_ENET_DEVICES 1
#endif

#define TEST_UFE_MAX_LINE_PORTS 4
#define TEST_MAX_UFE_DEVICES    N_UFE_DEVICES
#define TEST_MAX_ATM_CHANNELS   (TEST_UFE_MAX_LINE_PORTS*20)
#define TEST_MAX_POS_CHANNELS   (TEST_UFE_MAX_LINE_PORTS*4)
#define TEST_ATM_SCHEDULER      TEST_USING_HI_SPEED
#define TEST_SINGLE_IW_QNODE    0
#define TEST_SECOND_FLEXMUX_ID          1
#define TEST_SECOND_CONNECTOR_PORT_ID   2

#define TEST_PWE3ATM_VLAN_ID_START 10
#define TEST_PPP_VLAN_ID_START     100

#ifndef _OMIINO_FRAMER_API_PUBLIC_
#include "WO_FRMR_API_public.h"
#endif

#if (N_UFE_DEVICES >  WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES)
#error "FRAMER not configured for DUAL EMPHY"
#endif

#include "38621_defs.h"

int ufe_has_been_initialized[2];

int test_loopback_type = TEST_NORMAL;

int atm_scheduler_used = TEST_ATM_SCHEDULER;
int drivers_is_up = 0;
int test_iw_type = TEST_IWGP;
int test_host_traffic = 0;
TEST_system the_test;// = NULL;
WP_THREAD_ID mailbox_thread_id = -1;
WP_THREAD_ID alarm_and_pm_thread_id = -1;
int test_use_line = 0;
int test_flexmux_build_personality;

extern WP_CHAR *OMIINO_FRAMER_ErrorCodeToTxt(WP_U8 status);

#include "38621_alarms_and_pm.h"  

extern WP_U32 dps_ProgramImage[];
extern WP_U16 dps_PC_Table_Init[];

/* PCE parameters */
#include "wt_partition_ecc_util.h"
#include "brouter_pecs.h"
extern WP_pce_parser_info pce_parser_info;
extern WP_pecs_image_info brouter_pecs_pecs_image;

WP_S32 TEST_UfeLoadFpga(int);
static void CLI_T_Main(void);
static void CLI_F_Quit(void);
static void CLI_F_QuitNoReset(void);

extern Y_MenuEntry V_MainMenu[];

static void CLI_F_ShallowLineLoopback(WP_CHAR *StrPrm);
static void CLI_F_SDHLineLoopback(WP_CHAR *StrPrm);
static void CLI_F_DeepLineLoopback(WP_CHAR *StrPrm);
static void CLI_F_RunAtmHostTest(WP_CHAR *StrPrm);
static void CLI_F_RunAtmIwTest_1xOC12(WP_CHAR *StrPrm);
static void CLI_F_RunAtmIwTest_1xOC3(WP_CHAR *StrPrm);
static void CLI_F_RunAtmIwTest_4xOC3(WP_CHAR *StrPrm);
static void CLI_F_RunPosHostTest(WP_CHAR *StrPrm);
static void CLI_F_RunPosIwTest_1xOC12(WP_CHAR *StrPrm);
static void CLI_F_RunPosIwTest_1xOC3(WP_CHAR *StrPrm);
static void CLI_F_RunPosIwTest_4xOC3(WP_CHAR *StrPrm);
static void CLI_F_RunHsposHostTest(WP_CHAR *StrPrm);
static void CLI_F_RunHsposIwTest_1xOC12(WP_CHAR *StrPrm);
static void CLI_F_RunHsposIwTest_1xOC3(WP_CHAR *StrPrm);
static void CLI_F_RunHsposIwTest_4xOC3(WP_CHAR *StrPrm);
static void CLI_F_Statistics(WP_CHAR *StrPrm);
static void CLI_F_UfeEvents(WP_CHAR *StrPrm);
static void CLI_F_UfePhyDisplay(WP_CHAR *StrPrm);
static void CLI_F_ConfigureLayer1Type(WP_CHAR *StrPrm);
static void CLI_F_ConfigureUfeRevType(WP_CHAR *StrPrm);
static void CLI_F_ConfigureUseFramer(WP_CHAR *StrPrm);
static void CLI_F_ConfigureLoadFpga(WP_CHAR *StrPrm);
static void CLI_F_ConfigureHsposIwType(WP_CHAR *StrPrm);
static void CLI_F_ConfigurePsu(WP_CHAR *StrPrm);
static void CLI_F_ConfigureIwPortStats(WP_CHAR *StrPrm);
static void CLI_F_RunAtmIwTest_8xOC3(WP_CHAR *StrPrm);
static void CLI_F_RunPosIwTest_8xOC3(WP_CHAR *StrPrm);
static void CLI_F_RunHsposIwTest_8xOC3(WP_CHAR *StrPrm);
static void CLI_F_RunAtmIwTest_2xOC12(WP_CHAR *StrPrm);
static void CLI_F_RunPosIwTest_2xOC12(WP_CHAR *StrPrm);
static void CLI_F_RunHsposIwTest_2xOC12(WP_CHAR *StrPrm);
static void CLI_F_ConfigurePhyScrambling(WP_CHAR *StrPrm);
static void CLI_F_ConfigurePhyCrc(WP_CHAR *StrPrm);
static void CLI_F_ConfigureDeviceCrc(WP_CHAR *StrPrm);
static void CLI_F_EnableAlarms(WP_CHAR *StrPrm);
static void CLI_F_EnablePerformanceMonitoring(WP_CHAR *StrPrm);
static void CLI_F_DisplayAlarmMap(WP_CHAR *StrPrm);

static Y_MenuEntry V_Menu_LineLoopbackTests[] =
{
   {3, TRUE, "Line Loopback Tests",                    {V_MainMenu}},
   {K_Leaf, TRUE, " Shallow line loopback", {CLI_F_ShallowLineLoopback}},
   {K_Leaf, TRUE, " SDH line loopback", {CLI_F_SDHLineLoopback}},
   {K_Leaf, TRUE, " Deep line loopback", {CLI_F_DeepLineLoopback}},
};

static Y_MenuEntry V_Menu_TestConfiguration[] =
{
   {12, TRUE, "Test configuration options",   {V_MainMenu}},
   {K_Leaf, TRUE, "Configure Layer1 type", {CLI_F_ConfigureLayer1Type}},
   {K_Leaf, TRUE, "Configure UFE Rev type", {CLI_F_ConfigureUfeRevType}},
   {K_Leaf, TRUE, "Configure Framer usage", {CLI_F_ConfigureUseFramer}},
   {K_Leaf, TRUE, "Configure load firmware by host", {CLI_F_ConfigureLoadFpga}},
   {K_Leaf, TRUE, "Configure HSPOS Ingress IW type", {CLI_F_ConfigureHsposIwType}},
   {K_Leaf, TRUE, "Configure PSU usage", {CLI_F_ConfigurePsu}},
   {K_Leaf, TRUE, "Configure display of IW port statistics", {CLI_F_ConfigureIwPortStats}},
   {K_Leaf, TRUE, "Configure PHY scrambling", {CLI_F_ConfigurePhyScrambling}},
   {K_Leaf, TRUE, "Configure PHY CRC", {CLI_F_ConfigurePhyCrc}},
   {K_Leaf, TRUE, "Configure device CRC", {CLI_F_ConfigureDeviceCrc}},
   {K_Leaf, TRUE, "Enable alarms", {CLI_F_EnableAlarms}},
   {K_Leaf, TRUE, "Enable performance monitoring", {CLI_F_EnablePerformanceMonitoring}}
};

Y_MenuEntry V_MainMenu[] =
{
   {26, TRUE, "Main Menu",                        {V_MainMenu}},
   {K_Leaf, TRUE, "Stop test", {CLI_F_Quit}},
   {K_Leaf, TRUE, "Stop test (no reset)", {CLI_F_QuitNoReset}},
   {K_Menu, TRUE, "Test Configuration options", {V_Menu_TestConfiguration}},
   {K_Leaf, TRUE, "Display Statistics", {CLI_F_Statistics}},
   {K_Leaf, TRUE, "Read UFE events", {CLI_F_UfeEvents}},
   {K_Leaf, TRUE, "Read UFE data", {CLI_F_UfePhyDisplay}},
   {K_Menu, TRUE, "Run Line loopback tests", {V_Menu_LineLoopbackTests}},
   {K_Leaf, TRUE, "Run ATM Host test", {CLI_F_RunAtmHostTest}},
   {K_Leaf, TRUE, "Run ATM IW test (1xOC12)", {CLI_F_RunAtmIwTest_1xOC12}},
   {K_Leaf, TRUE, "Run ATM IW test (1xOC3)", {CLI_F_RunAtmIwTest_1xOC3}},
   {K_Leaf, TRUE, "Run ATM IW test (4xOC3)", {CLI_F_RunAtmIwTest_4xOC3}},
   {K_Leaf, TRUE, "Run POS Host test", {CLI_F_RunPosHostTest}},
   {K_Leaf, TRUE, "Run POS IW test (1xOC12)", {CLI_F_RunPosIwTest_1xOC12}},
   {K_Leaf, TRUE, "Run POS IW test (1xOC3)", {CLI_F_RunPosIwTest_1xOC3}},
   {K_Leaf, TRUE, "Run POS IW test (4xOC3)", {CLI_F_RunPosIwTest_4xOC3}},
   {K_Leaf, TRUE, "Run HSPOS Host test", {CLI_F_RunHsposHostTest}},
   {K_Leaf, TRUE, "Run HSPOS IW test (1xOC12)", {CLI_F_RunHsposIwTest_1xOC12}},
   {K_Leaf, TRUE, "Run HSPOS IW test (1xOC3)", {CLI_F_RunHsposIwTest_1xOC3}},
   {K_Leaf, TRUE, "Run HSPOS IW test (4xOC3)", {CLI_F_RunHsposIwTest_4xOC3}},
   {K_Leaf, TRUE, "Run ATM IW test (8xOC3)", {CLI_F_RunAtmIwTest_8xOC3}},
   {K_Leaf, TRUE, "Run POS IW test (8xOC3)", {CLI_F_RunPosIwTest_8xOC3}},
   {K_Leaf, TRUE, "Run HSPOS IW test (8xOC3)", {CLI_F_RunHsposIwTest_8xOC3}},
   {K_Leaf, TRUE, "Run ATM IW test (2xOC12)", {CLI_F_RunAtmIwTest_2xOC12}},
   {K_Leaf, TRUE, "Run POS IW test (2xOC12)", {CLI_F_RunPosIwTest_2xOC12}},
   {K_Leaf, TRUE, "Run HSPOS IW test (2xOC12)", {CLI_F_RunHsposIwTest_2xOC12}},
   {K_Leaf, TRUE, "Display Alarm Map", {CLI_F_DisplayAlarmMap}}
};

#define STR_MENU_PROMPT    ">"
WP_CHAR *STR_MENU_HDR = "      UFE Clear Channel test      ";

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
       { /* ASU_TPT         */ WP_BUS_HOST, APP_BANK_HOST},
       { /* ASU CW          */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
       { /* ASU TX QUEUE    */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
       { /* STATISTICS      */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
       { /* ADDRESS LOOKUP  */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
       { /* ATM CPT         */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
       { /* ENET HDLC CPT   */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
       { /* IW FBP PARAM    */ WP_BUS_PACKET, APP_BANK_PACKET},
       { /* IW FBP DATA     */ WP_BUS_HOST, APP_BANK_HOST},
       { /* EXTERNAL FIFO   */ WP_BUS_HOST, APP_BANK_HOST},
       { /* MAC LOOKUP      */ WP_BUS_HOST, APP_BANK_HOST},
       { /* IMA DISPATCH    */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},   /* Dispatch */
       { /* IMA SHARED      */ WP_BUS_PACKET, APP_BANK_PACKET},       /* Common   */
       { /* LPM             */ WP_BUS_HOST, APP_BANK_HOST},
       { /* MFC RULES       */ WP_BUS_HOST, APP_BANK_HOST},
       { /* FIWT, QAT       */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
       { /* MFCPT, UPPPT, DS */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
       { /* STATISTICS2      */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
       { /* STATISTICS3/IMAPORT */ WP_BUS_PARAM, APP_BANK_PARAM},       /* Port      */
       { /* WP_VB_IMA_RX_BUS    */ WP_BUS_PACKET, APP_BANK_PACKET},     /* Rx buffer */
       { /* PKT_GLOBAL_TCPT     */ WP_BUS_PARAM, APP_BANK_PARAM},
       { /* AAL2_QSR/IMATXBUF   */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},       /* Tx buffer */
       { /* MANDATORY       */ WP_BUS_NONE, 0},
       { /* MANDATORY       */ WP_BUS_NONE, 0},
       /* ************* VIRTUAL BUS ASSIGNMENT ***************/
       {/* MCH_DATA        */ WP_BUS_PACKET, APP_BANK_PACKET},
       {/* WP_VB_OAM_FM_QUERY*/ WP_BUS_HOST, APP_BANK_HOST},
       {/* WP_VB_HASH_ON_HEAP*/ WP_BUS_NONE, 0},
       {/* WP_VB_MFC_INTERNAL*/ WP_BUS_PACKET, APP_BANK_PACKET},
       {/* WP_VB_MFR_RAS_TBL */ WP_BUS_NONE, 0},
       {/* WP_VB_CESOP_DATA  */ WP_BUS_PACKET, APP_BANK_PACKET}
    },
    {
       dps_ProgramImage, dps_ProgramImage},
    {
       dps_PC_Table_Init, dps_PC_Table_Init},
   }
};

/* Interrupt queue configuration used with WP_SysInit */
WP_int_queue_table int_queue_tables =
{ {{100, 1}, {100, 1}, {10, 1}, {10, 1}} };


/* Total number of pools that this application will need */
WP_pool_global pool_limit = 8;

/* Global atm_data for the atm channels created by this application */
WP_atm_global atm_params[1] = {
   {
      /* max_pm_tables */ 0,
      /* max_ubrplus_channels */ 0,
      /* max_upc_tables */ 0,
      /* max_rx_cids */ 0,
      /* max_tx_cids */ 0,
      /* max_cid_range */ 0,
      /* AAL2 QSR bus */ WP_BUS_NONE,
      /* AAL2 QSR bank */ 0,
      /* ATM modes */ {WP_ATM_RAS_TIMESTAMP_DISABLE, WP_AAL2_RX_3FE_DISABLE},
      /* max_stats_extensions */ TEST_MAX_ATM_CHANNELS,
      /* max_hier_shaping_groups */ 0
   }
};

WP_iw_global iw_global_params[1] =
{
   {
      /* max_iw_aggregations */        TEST_IW_MAX_FLOWS,
      /* max_r_iw_systems;*/           3,
      /* max_s_iw_systems;*/           2,
      /* max_b_iw_systems;*/           2,
      /* max_m_iw_systems;*/           1,
      /* max_vlan_priority_maps; */    0,
      /* iw_bkgnd_fifo_size */         100,
      /* cong_pt       */              NULL,
      /* iw_host_limits         */
      {
         /* max_rx_channels */ 1,
         /* pq_block_size   */ 0,
         /* n_pq_blocks     */ 0,
         /* max_tx_channels */ 1,
         /* max_tx_groups   */ 0
      },
      /* mpls_config */
      {
         /* n_llst_tables   */ 1,
         /* llst_table_size */ 2016
      },
      /* iw_modes;*/
      {
         /*policer_mode;*/                WP_IW_POLICER_DISABLE,
         /*statistics_bundle_stat_mode;*/ WP_IW_STATISTICS_BUNDLE_STAT_DISABLE,
         /*l2_prefix_ext_mode;*/          WP_IW_L2_HEADER_EXTENSION_ENABLE,
         /*enhanced_flow_stat_mode;*/     WP_IW_ENHANCED_FLOW_STAT_ENABLE,
         /*flow_stat_mode;*/              WP_IW_FLOW_STAT_ENABLE,
         /*fr_tx_flow_stat_mode*/         WP_IW_FR_TX_FLOW_STAT_DISABLE,
         /*mfc_alloc_mode    */           WP_IW_MFC_ALLOC_DISABLE,
         /*learning_queue_mode */         WP_IW_LEARNING_DISABLE,
         /*port_filtering_mode */         WP_IW_PORT_FILTERING_DISABLE,
         /* lpm_alloc_mode */             WP_IW_LPM_ALLOC_DISABLE,
         /* enhanced_mc_member_stat_mode */WP_IW_ENHANCED_MC_MEMBER_STAT_DISABLE,
         /* routing_alloc_modes */        WP_DISABLE,
         /* res_mc_addresses_table_mode */ WP_IW_BPORT_RES_MC_TABLE_ALLOC_DISABLE,
         /* port_stag_etype_mode */       WP_IW_PORT_STAG_ETYPE_DISABLE,
         /* iw_characteristics */         0
      },
      /* max_ch_agg_stat;*/            0,
      /* max_fr_s_iw_systems;*/        0,
      /* max_iw_ports */               (N_UFE_DEVICES * TEST_UFE_MAX_LINE_PORTS + 2),
      /* max_iw_mc_groups   */         0,
      /* max_nat_systems    */         0,
      /* max_iw_second_aggregations */ 0
   }
};

WP_CHAR app_name[] = "38621";

/******************************************
 * UFE4 SDH system configuration
 *****************************************/
WUFE_system ufe_system_cfg = {
   /* max_line_number     */ TEST_UFE_MAX_LINE_PORTS,
   /* max_phy_number      */ TEST_UFE_MAX_LINE_PORTS,
   /* emphy_global        */ {
      /* rx_pri_table          */ {{0, 1, 2, 3}, {0, 1, 2, 3}, {0, 1, 2, 3}, {0, 1, 2, 3}},
      /* tx_pri_table          */ {{0, 0, 1, 2}, {0, 0, 1, 2}, {0, 0, 1, 2}, {0, 0, 1, 2}},
      /* tx_inframe_pri_table  */ {{0, 1, 2, 3}, {0, 1, 2, 3}, {0, 1, 2, 3}, {0, 1, 2, 3}},
      /* rx_parity_mode        */ WUFE_RX_PARITY_EVEN,
      /* tx_parity_mode        */ WUFE_TX_PARITY_EVEN,
      /* tx_parity_check       */ WUFE_TX_PARITY_IGNORE
   },
   /* fifo_manager        */ {
      /* tx_fbp_size           */ TEST_UFE_MAX_LINE_PORTS * 4,
      /* rx_fbp_size           */ TEST_UFE_MAX_LINE_PORTS * 5
   },
   /* idle_pattern        */ {
      /* tx_idle_byte_data_pattern  */ 0xff,
      /* tx_idle_nibble_cas_pattern */ 0x4
   },
   /* line_interface      */ 0,
   /* sbi                 */ {0},
   /* clock_rec_enable */ WUFE_CLOCK_REC_DISABLE
};

WUFE_line_sdh line_sdh_cfg;
WUFE_line_sonet line_sonet_cfg;

/******************************************
 * UFE SDH port configuration
 *****************************************/
WP_port_atm_limits ufe_atm_limits = {
   /*  max_tx_channels  */ TEST_MAX_ATM_CHANNELS,
   /*  max_rx_channels  */ TEST_MAX_ATM_CHANNELS,
   /*  pq_block_size    */ 0,
   /*  n_pq_blocks      */ 0,
   /*  aal1_tx_channels */ 0,
   /*  aal1_rx_channels */ 0,
   /*  n_mq_blocks      */ 0,
   /*  n_mq_pq_blocks   */ 0,
   /*  emphy_devices    */ (TEST_UFE_MAX_LINE_PORTS  * N_UFE_DEVICES)
};

WP_port_pkt_limits pkt_limits = {
    /*  max_tx_channels  */ TEST_MAX_POS_CHANNELS,
    /*  max_rx_channels  */ TEST_MAX_POS_CHANNELS,
    /*  pq_block_size    */ 0,
    /*  n_pq_blocks      */ 0,
    /*  emphy_devices    */ (TEST_UFE_MAX_LINE_PORTS * N_UFE_DEVICES)
};

WP_port_upi_packet_ext pkt_ext =
{
   /* packet_characteristics */ 0,
   /* slow_phy_tx_bitrate */    0,
   /* pkt_limits */ &pkt_limits
};

WP_port_upi_emphy_ext emphy_ext =
{
   /* tx_maxsdu */ 1536,
   /* protocols */ WP_EMPHY_ATM | WP_EMPHY_HDLC_PPP | WP_EMPHY_HSPOS,
   /* trans_emphy_devices */ 0,
   /* max_ds1 */ 0,
   /* hspos_devices_num */ TEST_MAX_UFE_DEVICES * TEST_UFE_MAX_LINE_PORTS
};

WP_port_upi_atm_ext ufe_atm_ext = {
   /* atm_characteristics */
   WP_UPI_IDLEDISC_ON | WP_ATM_EXT_PHY_STATS_ENABLE,
   /* utopia_clk */ 50000000,
   /* atm_limits */ &ufe_atm_limits,
   /* tx_atm_exthdrsz */ 0,
   /* rx_atm_exthdrsz */ 0,
   /* rx_atm_lookupmode */ WP_ATM_LOOKUP_HASH
};

#if TEST_USING_HI_SPEED
WP_port_upi_iw_ext iw_ext = {
   .iw_characteristics = WP_IW_BKGND_USED
};
#endif

WP_port_upi upi_port_config[1] =
{
   {
      /* upimode */ WP_UPI_UFE4,
      /* loopback */ WP_UPI_NORMAL,
      /* tx_characteristics */
      WP_UPI_MASTER |
      WP_UPI_SINGLE |
      WP_UPI_PRT_EVEN |
      WP_UPI_ROBIN |
      WP_TQ_SLOW_MODE |
      WP_TX_PRIORITY_4 |
      WP_UPI_EMPHY2_13,
      /* rx_characteristics */
      WP_UPI_MASTER |
      WP_UPI_SINGLE |
      WP_UPI_PRT_EVEN |
      WP_UPI_ROBIN |
      WP_UPI_PRTCHK_OFF,
      /* atm_ext */ &ufe_atm_ext,
      /* packet_ext */ &pkt_ext,
#if TEST_USING_HI_SPEED
      /* iw_ext */ &iw_ext,
#else
      /* iw_ext */ 0,
#endif
      /* emphy_ext */ &emphy_ext
   }
};


/******************************************
 * UFE4 SDH phy configuration
 *****************************************/
WUFE_phy ufe_phy_atm_cfg = {
   /* struct_id         */ WUFE_STRUCT_PHY_ATM,
   /* rx_fifo_param     */ {
      /* fifo_max_size     */ 2,
      /* priority_table    */ 0
   },
   /* rx_slot_group */ {
      /* hmvip_id          */ 0,
      /* slot_arr          */ { 0 },
      /* n_slots           */ 32
   },
   /* tx_fifo_param; */ {
      /* fifo_max_size     */ 2,
      /* priority_table    */ 0,
      /* fast_mode         */ WUFE_PHY_FIFO_MODE_FAST,
      /* transmit_th       */ 0,
      /* wait_type         */ WUFE_PHY_TX_WAIT_TYPE_D
   },
   /* tx_slot_group */ {
      /* hmvip_id          */ 0,
      /* slot_arr          */ { 0 },
      /* n_slots           */ 32
   },
   /* type          */ {
      /* atm             */ {
         /* rx_descrambler_mode        */ WUFE_PHY_ATM_RX_DESCRAMBLER_ENABLE,
         /* rx_coset_mode              */ WUFE_PHY_ATM_RX_ADD_COSET,
         /* rx_idle_cell_discard       */ WUFE_PHY_ATM_RX_IDLE_CELL_DISCARD,
         /* rx_unsigned_cell_discard   */ WUFE_PHY_ATM_RX_UNASSIGNED_CELL_DISCARD,
         /* rx_hec_error_discard       */ WUFE_PHY_ATM_RX_HEC_ERR_TRANSFER,
         /* rx_hec_correction_mode     */ WUFE_PHY_ATM_RX_HEC_CORECTION_ENABLE,
         /* rx_cell_deliniation        */ WUFE_PHY_ATM_RX_CELL_DELINEATION_BYTE_RESOLUTION,
         /* rx_alpha                   */ 2,
         /* rx_delta                   */ 7,
         /* tx_scrambler_mode          */ WUFE_PHY_ATM_TX_SCRAMBLER_ENABLE,
         /* tx_coset_mode              */ WUFE_PHY_ATM_TX_ADD_COSET,
         /* tx_idle_unsigned_cell_mode */ WUFE_PHY_ATM_TX_UNASSIGNED_CELL_MODE,
         /* tx_underrun_interrupt_mode */ WUFE_PHY_ATM_TX_UNDERRUN_INT_DISABLE
      }
   }
};

WUFE_phy ufe_phy_pos_cfg = {
   /* struct_id         */ WUFE_STRUCT_PHY_POS,
   /* rx_fifo_param     */ {
      /* fifo_max_size     */ 4,
      /* priority_table    */ 0
   },
   /* rx_slot_group */ {
      /* hmvip_id          */ 0,
      /* slot_arr          */ { 0 },
      /* n_slots           */ 32
   },
   /* tx_fifo_param; */ {
      /* fifo_max_size     */ 4,
      /* priority_table    */ 0,
      /* fast_mode         */ WUFE_PHY_FIFO_MODE_FAST,
      /* transmit_th       */ 2,
      /* wait_type         */ WUFE_PHY_TX_WAIT_TYPE_A
   },
   /* tx_slot_group */ {
      /* hmvip_id          */ 0,
      /* slot_arr          */ { 0 },
      /* n_slots           */ 32
   },
   /* type          */ {
      /* POS             */ {
         /* rx_frame_crc_size          */ WUFE_PHY_POS_CRC_32,
         /* rx_descrambler_mode        */ WUFE_PHY_POS_RX_DESCRAMBLER_ENABLE,
         /* tx_frame_crc_size          */ WUFE_PHY_POS_CRC_32,
         /* tx_scrambler_mode          */ WUFE_PHY_POS_TX_SCRAMBLER_ENABLE,
         /* tx_min_flag_num            */ 1
      }
   }
};

/******************************************
 * UFE device configuration
 *****************************************/
WP_device_emphy_atm ufe_atm_device_cfg[1] =
{
   {
      /* tx_bit_rate                    */ 622000000,
      /* tx_slave_addr                  */ 0,
      /* tx_shaping_mode                */ WP_ATM_DEV_SHAPING_VC,
      /* tx_shaping_groups_max          */ 0,
      /* tx_hierarchical_shaping_table  */ NULL,
      /* rx_bit_rate                    */ 622000000,
      /* vp_vc_block                    */ 0,
      /* rx_slave_addr                  */ 0,
      /* rx_ras_timeout                 */ 0,
      /* uni_nni_mode                   */ WP_EMPHY_ATM_NNI_MODE,
      /* idle_format                    */ WP_EMPHY_IDLE_REGULAR,
      /* line_id                        */ WP_NO_LINE_ID
   }
};

WP_device_emphy_hdlc_ppp ufe_hdlc_device_cfg[1] =
{
   {
      /* tx_bit_rate */            1,
      /* tx_slave_addr */          0,
      /* tx_statmode */            WP_PKT_STAT_ENABLE,
      /* tx_maxsdu */              1536,
      /* rx_bit_rate */            1,
      /* rx_slave_addr */          0,
      /* rx_statmode */            WP_PKT_STAT_ENABLE,
      /* flowmode */               WP_FLOWMODE_MULTI,
      /* addr_mask */              0,
      /* address1 */               0,
      /* address2 */               0,
      /* tx_tq_types */            {WP_PKT_SCHED_WFQ,
                                    WP_PKT_SCHED_WFQ,
                                    WP_PKT_SCHED_WFQ,
                                    WP_PKT_SCHED_WFQ},
      /* crctype */                WP_HDLC_CRC32
   }
};

WP_device_upi_hspos ufe_hspos_device_cfg[1] =
{
   {
      /* tx_bit_rate */ WP_TX_BIT_RATE_UNLIMITED,
      /* tx_statmode */ WP_ENABLE,
      /* tx_intmode */  WP_DISABLE,
      /* tx_slave_addr */ 0,
      /* tx_queuedepth */ TX_QUEUEDEPTH_USE_CHANNELS,
      /* rx_bit_rate */ 1, /* for UFE means only enable RX direction */
      /* rx_statmode */ WP_ENABLE,
      /* rx_intmode */ WP_DISABLE,
      /* rx_queuedepth */ 100,
      /* rx_slave_addr */ 0,
      /* tx_tag */ 0,
      /* rx_tag */ 0,
      /* end_station_mode */ WP_END_STATION_MODE_DISABLE, /* use IW only */
#if _WP_MODULE_NEC_GPON_
      /* bridge_port_id_mode */ 0,
      /* remove_fcs */ 0,
#endif
      /* tx_maxsdu */ 1536,
#if _WP_MODULE_NEC_GPON_
      /* enet_over_pos */ 0,
#endif
      /* max_tx_slow_channels */ 0
   }
};

WP_calendar_wheel calendar_wheel_cfg[3] =
{
   {
      /* function */    WP_CW_ASU,
      /* cwid*/         WP_CW_ID_A,
      /* min_ch_rate */ 150,
      /* max_ch_rate */ 1412830
   },
   {
      /* function */    WP_CW_PSU,
      /* cwid*/         WP_CW_ID_B,
      /* min_ch_rate */ 1000000,
      /* max_ch_rate */ 599040000
   },
   WP_CALENDAR_WHEEL_TERM
};

void TEST_DriversRelease(int state);


/*************************************************
 * Function: ufe_terminate_on_error
 *************************************************/
static void ufe_terminate_on_error(WUFE_status status, WP_CHAR *s)
{
   if (status != WUFE_OK)
   {
      printf("UFE Error %s %s\n", WUFE_error_name[(status&0x03ffffff)], s);
#if defined(__linux__)
      if (test_config.test_use_framer)
      {
         if (WPL_ThreadDestroy(&mailbox_thread_id) != 0)
         {
            printf("Test Abort. The WPL_ThreadDestroy failed\n");
         }
      }
#endif
      TEST_DriversRelease(1);
      WT_FailAndTerminate();
   }

}

static void framer_terminate_on_error(WP_CHAR * Message, WP_U8 status, WP_U32 line)
{
   extern WP_CHAR *OMIINO_FRAMER_ErrorCodeToTxt(WP_U8 status);

   if (status)
   {
      printf("Test Abort: function %s in Line:%d returned %s ( Error code %d)\n", Message, line,
            OMIINO_FRAMER_ErrorCodeToTxt(status), (WP_U32) status);

#if defined(__linux__)
      if (test_config.test_use_framer)
      {
         if (WPL_ThreadDestroy(&mailbox_thread_id) != 0)
         {
            printf("Test Abort. The WPL_ThreadDestroy failed\n");
         }
      }
#endif
      TEST_DriversRelease(1);
      WT_FailAndTerminate();
   }
}

void TEST_FailAndTerminate(void)
{
   TEST_DriversRelease(1);
   WT_FailAndTerminate();
}

void TEST_TerminateOnError(WP_handle handle, WP_CHAR *s)
{
   if (WP_ERROR_P(handle)) {
      printf("Test Abort %s %s 0x%x\n", s, WP_ErrorString(handle), handle);
      TEST_FailAndTerminate();
   }
   /*printf("API call status: %s 0x%x\n", s, handle); */
}

static WP_status TEST_DebugCallback(const WP_CHAR *file, const WP_CHAR *function,
                                    WP_S32 line, WP_S32 error_id)
{
   if (error_id)
      printf("file = %s , line = %d function = %s\n",file,line,function);
   return WP_OK;
}

void TEST_UfePortConfigure(WP_U32 wpid)
{
   WP_status status;

   if (test_config.test_ufe4_type == TEST_UFE412_REVA ||
       test_config.test_ufe4_type == TEST_UFE412_REVB)
   {
      upi_port_config->tx_characteristics |= WP_UPI_BUS_WIDTH_16;
      upi_port_config->rx_characteristics |= WP_UPI_BUS_WIDTH_16;

      if (TEST_PORT_IN_USE == WP_PORT_UPI3)
      {
         status = WPX_BoardConfigure(wpid, WPX_CONFIGURE_UPI3_UFE412);
         TEST_TerminateOnError(status, "UPI3/UFE4/Spartan Board configuration");
      }
      else
      {
         status = WPX_BoardConfigure(wpid, WPX_CONFIGURE_2UPI_1XGI_10SGMII_UFE4);
         TEST_TerminateOnError(status, "UFE4/Spartan Board configuration");
      }
   }
   else
   {
      upi_port_config->tx_characteristics |= WP_UPI_BUS_WIDTH_32;
      upi_port_config->rx_characteristics |= WP_UPI_BUS_WIDTH_32;

      if (TEST_PORT_IN_USE == WP_PORT_UPI1)
         status = WPX_BoardConfigure(wpid, WPX_CONFIGURE_2UPI_2XGI_6SGMII2G);
      else
         status = WPX_BoardConfigure(wpid, WPX_CONFIGURE_1UPI_1XGI_6RTBI_6SGMII);
      TEST_TerminateOnError(status, "UFE4/Fpga Board configuration");
   }
}


WP_S32 TEST_WinPathInitialize(void)
{
   WP_handle status;
   WP_U32 wpid = WP_WINPATH(0);

   context->virtual_bus[WP_VB_EMPHY_PORT_LOOKUP].bus = WP_BUS_INTERNAL;
   context->virtual_bus[WP_VB_EMPHY_PORT_LOOKUP].bank = APP_BANK_INTERNAL;
   context->int_queue_data = &int_queue_tables;
   context->atm_data = atm_params;
   context->iw_data = iw_global_params;
   context->pool_limit = pool_limit;

   memcpy(&context->virtual_bus, wp3_bus_assignments, sizeof(wp3_bus_assignments));

   WP_ControlRegister(WP_DEBUG_CALLBACK_FILE_LINE_ERROR, TEST_DebugCallback);

   status = WP_SysInit(WP_WINPATH(0), context);
   TEST_TerminateOnError(status, "WP_SysInit()");

   if (test_config.test_psu_shaping_type == TEST_PKT_CIR_EIR_SHAPING)
   {
      WP_feature_psu psu_config={0, TEST_MAX_POS_CHANNELS, 1536, 0};

      status = WP_FeatureInit(WP_SYSHANDLE(WP_WINPATH(0)), WP_FEATURE_PSU, &psu_config);
      TEST_TerminateOnError(status, "WP_FeatureInit()");
   }

   if (test_iw_type == TEST_IWGP)
   {
      TEST_PceModuleInit();
      TEST_GenericPacketEditingModuleInit();
   }

   if (test_config.test_layer1_type == TEST_OC12_CARD)
   {
      TEST_UpiOc12PortConfigAndCreate(wpid);
   }
   else
   {
      /* UFE mode */
      TEST_UfePortConfigure(wpid);
      TEST_UfeLoadFpga(UFE_PRIMARY);
      if (test_config.test_dual_emphy)
      {
         TEST_SecondUfeBoardConfigure(WP_WINPATH(DEFAULT_WPID));
         TEST_UfeLoadFpga(UFE_SECONDARY);
      }
      TEST_CreatePorts(TEST_PORT_IN_USE, (void*)upi_port_config);
   }

   TEST_QnodesCreate();

   if (atm_scheduler_used || test_config.test_pkt_scheduler_used)
   {
      status = WP_SysSchedulerCreate(wpid, calendar_wheel_cfg);
      TEST_TerminateOnError(status, "WP_SysSchedulerCreate()");
   }

#if TEST_USING_AAL0_GROUP
#ifdef TEST_WITH_EGRESS_HASH_LOOKUP
   {
      WP_feature_pwe3_hash hash_config;

      hash_config.pwe_hash_size = WP_PWE3_ATM_MIN_HASH_SIZE;
      status = WP_FeatureInit(wpid, WP_FEATURE_PWE3_EGRESS_HASH_LOOKUP, &hash_config);
      TEST_TerminateOnError(status, "WP_FeatureInit HASH");
   }
#endif
#endif

   /* Commit system resources */
   status = WP_SysCommit();
   TEST_TerminateOnError(status, "WP_SysCommit()");

   WT_Identify();

   TEST_EgressIwSystemCreate();

   TEST_EnetChannelsCreate();

   if (test_config.test_layer1_type == TEST_OC12_CARD)
   {
      TEST_HdlcRxChannelCreate(0);
   }

   TEST_SystemEnable();

   return 0;
}

WP_S32 TEST_FramerInitialize(int flexmux_id)
{
   WUFE_status status;
   int connector_port_id=0;
   WP_U32 thread_params[2*N_UFE_DEVICES];
   WP_U8 device_mode;
   int test_flexmux_build_personality;

   if (!test_config.test_ufe_clear_channel)
   {
      printf("Test Abort. The framer is not supported for channelized mode\n");
      TEST_FailAndTerminate();
   }

   if (flexmux_id == UFE_PRIMARY)
   {
      connector_port_id =
         (TEST_PORT_IN_USE == WP_PORT_UPI1) ? 0 : (TEST_PORT_IN_USE == WP_PORT_UPI2) ? 1 : 2;
      
      status = WPX_UFE_FRAMER_FlexmuxInit(flexmux_id, connector_port_id);
      framer_terminate_on_error("WPX_UFE_FRAMER_FlexmuxInit", status, __LINE__);

      /* service the mailboxes in a separate process */
      if (mailbox_thread_id == -1)
      {
         thread_params[0] = flexmux_id;
         thread_params[1] = connector_port_id;
         if (test_config.test_dual_emphy)
         {
            thread_params[2] = TEST_SECOND_FLEXMUX_ID;
            thread_params[3] = TEST_SECOND_CONNECTOR_PORT_ID;
         }
         
         WPL_ThreadInit(&mailbox_thread_id,
                        (void *(*)(void *)) WPX_UFE_FRAMER_FlexmuxServiceMailbox,
                        (void *) thread_params);
      }
   }
   else if (test_config.test_dual_emphy)
   {
      connector_port_id = TEST_SECOND_CONNECTOR_PORT_ID;
   }
   else
   {
      printf("Flexmux ID greater than 0 used for single EMPHY mode!\n");
      TEST_FailAndTerminate();
   }

   /* set the Flexmux device build personality */
   if (test_config.test_line_type == TEST_SDH_STM4 ||
       test_config.test_line_type == TEST_SONET_OC12)
   {
      printf("build_personality: WPX_UFE_FRAMER_BUILD_PERSONALITY_1_PLUS_1_OC3_OC12\n");
      test_flexmux_build_personality = WPX_UFE_FRAMER_BUILD_PERSONALITY_1_PLUS_1_OC3_OC12;
   }
   else
   {
      printf("build_personality: WPX_UFE_FRAMER_BUILD_PERSONALITY_4_PLUS_4_OC3\n");
      test_flexmux_build_personality = WPX_UFE_FRAMER_BUILD_PERSONALITY_4_PLUS_4_OC3;
   }

   status = WPX_UFE_FRAMER_FlexmuxSetBuildPersonality(flexmux_id,
                                                      test_flexmux_build_personality,
                                                      connector_port_id);
   framer_terminate_on_error("WPX_UFE_FRAMER_FlexmuxSetBuildPersonality", status, __LINE__);

   if (test_config.test_ufe4_type == TEST_UFE412_REVA)
   {
      printf("-- Set Clock for UFE412 RevA --\n");
      /* set the gpt source clock required with wds-ufe4-s6 board*/
      WPX_FRMR_DEMO_ONLY_Configure_GTP_ClockSource(flexmux_id);
   }

   /* Set line protection according to the build personality in use*/
   WPX_FRMR_SetDevProtectMode(0, flexmux_id, WPX_UFE_DEVICE_PROTECTION_MODE_LINE);

   if ((test_config.test_line_type == TEST_SDH_STM1) ||
       (test_config.test_line_type == TEST_SDH_STM4))
      device_mode = WPX_UFE_FRAMER_DEVICE_MODE_SDH;
   else
      device_mode = WPX_UFE_FRAMER_DEVICE_MODE_SONET;

   /* set device mode (SONET\SDH) */
   status = WPX_FRMR_DEV_SetMode(0, flexmux_id, device_mode);
   framer_terminate_on_error("WPX_UFE_FRAMER_CONFIGURATION_DEVICE_SetMode", status, __LINE__);

   if (flexmux_id == UFE_PRIMARY && test_config.test_layer1_type == TEST_SDH)
   {
      status = WPX_FRMR_DEV_DRV_SONET_SDH_EnableAlarm(0,WPX_UFE_FRAMER_SDH_HP_PLM);
      framer_terminate_on_error("WPX_FRMR_DEV_DRV_SONET_SDH_EnableAlarm HP_PLM", status, __LINE__);

      status = WPX_FRMR_DEV_DRV_SONET_SDH_EnableAlarm(0,WPX_UFE_FRAMER_SDH_HP_UNEQ);
      framer_terminate_on_error("WPX_FRMR_DEV_DRV_SONET_SDH_EnableAlarm HP UNEQ", status, __LINE__);

      status = WPX_FRMR_DEV_DRV_SONET_SDH_EnableAlarm(0,WPX_UFE_FRAMER_SDH_HP_TIM);
      framer_terminate_on_error("WPX_FRMR_DEV_DRV_SONET_SDH_EnableAlarm HP TIM", status, __LINE__);
   }

   return 0;
}

void TEST_SignalOverheadSet(int flexmux_id, int index, int line_port_id)
{
   WUFE_status status;
   U8 TX_C2=19, EX_C2=19;
   WP_CHAR *TX_J1="WG HP-TRACE", *EX_J1="WG HP-TRACE",*pTX_TraceString="WG RS-TRACE",*pEX_TraceString="WG RS-TRACE";
//   U8 TX_V5_SignalLabel=0x2, EX_V5_SignalLabel=0x2;
//   WP_CHAR *TX_J2_TraceString = "WG LP-TRACE",*EX_J2_TraceString = "WG LP-TRACE";

   WPX_FRMR_SONET_SDH_PORT_SEC_J0_SetMode(0, flexmux_id, index, WPX_UFE_FRAMER_PATH_TRACE_MODE_16_BYTE);
   status = WPX_FRMR_SONET_SDH_PORT_SEC_J0_SetTX(0, flexmux_id, index, pTX_TraceString);
   framer_terminate_on_error("WPX_FRMR_SONET_SDH_SOH_J0_SetTX", status, __LINE__);

   status = WPX_FRMR_SONET_SDH_PORT_SEC_J0_SetEX(0, flexmux_id, index, pEX_TraceString);
   framer_terminate_on_error("WPX_FRMR_SONET_SDH_SOH_J0_SetEX", status, __LINE__);

   /* set the transmitted and expected C2 values */
   status = WPX_FRMR_SONET_SDH_HO_PATH_C2_SetTX(0, flexmux_id, &the_test.ufe[flexmux_id].line_end_point[line_port_id], TX_C2);
   framer_terminate_on_error("WPX_FRMR_SONET_SDH_HO_PATH_C2_SetTX", status, __LINE__);

   status = WPX_FRMR_SONET_SDH_HO_PATH_C2_SetEX(0, flexmux_id, &the_test.ufe[flexmux_id].line_end_point[line_port_id], EX_C2);
   framer_terminate_on_error("WPX_FRMR_SONET_SDH_HO_PATH_C2_SetEX", status, __LINE__);

   WPX_FRMR_SONET_SDH_HO_PATH_J1_SetMode(0, flexmux_id, &the_test.ufe[flexmux_id].line_end_point[line_port_id], WPX_UFE_FRAMER_PATH_TRACE_MODE_16_BYTE);
   status = WPX_FRMR_SONET_SDH_HO_PATH_J1_SetTX(0, flexmux_id, &the_test.ufe[flexmux_id].line_end_point[line_port_id], TX_J1);
   framer_terminate_on_error("WPX_FRMR_SONET_SDH_HO_PATH_J1_SetTX", status, __LINE__);

   status = WPX_FRMR_SONET_SDH_HO_PATH_J1_SetEX(0, flexmux_id, &the_test.ufe[flexmux_id].line_end_point[line_port_id], EX_J1);
   framer_terminate_on_error("WPX_FRMR_SONET_SDH_HO_PATH_J1_SetEX", status, __LINE__);

#if 0
   /* set the transmitted and expected V5 values */
   status = WPX_FRMR_SONET_SDH_LO_PATH_V5_SigLabel_SetTX(0, flexmux_id, &the_test.ufe[flexmux_id].line_end_point[line_port_id], TX_V5_SignalLabel);
   framer_terminate_on_error("WPX_FRMR_SONET_SDH_LO_PATH_V5_SigLabel_SetTX", status, __LINE__);

   status = WPX_FRMR_SONET_SDH_LO_PATH_V5_SigLabel_SetEX(0, flexmux_id, &the_test.ufe[flexmux_id].line_end_point[line_port_id], EX_V5_SignalLabel);
   framer_terminate_on_error("WPX_FRMR_SONET_SDH_LO_PATH_V5_SigLabel_SetEX", status, __LINE__);

   status = WPX_FRMR_SONET_SDH_LO_PATH_J2_SetTX(0, flexmux_id, &the_test.ufe[flexmux_id].line_end_point[line_port_id], TX_J2_TraceString);
   framer_terminate_on_error("WPX_FRMR_SONET_SDH_LO_PATH_J2_SetTX", status, __LINE__);

   status = WPX_FRMR_SONET_SDH_LO_PATH_J2_SetEX(0, flexmux_id, &the_test.ufe[flexmux_id].line_end_point[line_port_id], EX_J2_TraceString);
   framer_terminate_on_error("WPX_FRMR_SONET_SDH_LO_PATH_J2_SetEX", status, __LINE__);
#endif
}

WP_S32 TEST_FramerLinePortInitialize(int flexmux_id, int line_port_id)
{
   WUFE_status status;
   int index=0;
   int rate;

   memset(&the_test.ufe[flexmux_id].line_end_point[line_port_id], 0, sizeof(WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE));

   if (test_config.test_line_type == TEST_SONET_OC12)
   {
      the_test.ufe[flexmux_id].line_end_point[line_port_id].TransferType = WPX_UFE_FRAMER_WUFE_SONET_TYPE_STS12;
      index = the_test.ufe[flexmux_id].line_end_point[line_port_id].u.SONET.sts12;
      rate = WPX_UFE_FRAMER_LINE_PORT_RATE_OC12;
   }
   else if (test_config.test_line_type == TEST_SONET_OC3)
   {
      the_test.ufe[flexmux_id].line_end_point[line_port_id].TransferType = WPX_UFE_FRAMER_WUFE_SONET_TYPE_STS3;
      the_test.ufe[flexmux_id].line_end_point[line_port_id].u.SONET.sts3 = line_port_id;
      index = the_test.ufe[flexmux_id].line_end_point[line_port_id].u.SONET.sts3;
      rate = WPX_UFE_FRAMER_LINE_PORT_RATE_OC3;
   }
   else if (test_config.test_line_type == TEST_SDH_STM4)
   {
      the_test.ufe[flexmux_id].line_end_point[line_port_id].TransferType = WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC4_4C;
      index = the_test.ufe[flexmux_id].line_end_point[line_port_id].u.SDH.stm4;
      rate = WPX_UFE_FRAMER_LINE_PORT_RATE_STM4;
   }
   else if (test_config.test_line_type == TEST_SDH_STM1)
   {
      the_test.ufe[flexmux_id].line_end_point[line_port_id].TransferType = WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC4;
      the_test.ufe[flexmux_id].line_end_point[line_port_id].u.SDH.stm1 = line_port_id;
      index = the_test.ufe[flexmux_id].line_end_point[line_port_id].u.SDH.stm1;
      rate = WPX_UFE_FRAMER_LINE_PORT_RATE_STM1;
   }
   else
   {
      printf("Test Abort. TEST_FramerLinePortInitialize -> invalid line type\n");
      rate = 0;
      TEST_FailAndTerminate();
   }

   status = WPX_FRMR_SONET_SDH_PORT_SetRate(0, flexmux_id, line_port_id, rate);
   framer_terminate_on_error("WPX_FRMR_SONET_SDH_PORT_SetRate ", status, __LINE__);

#if CREATE_FACILITY_WORKAROUND
   if (test_config.test_layer1_type == TEST_SDH)
   {
      status = WPX_FRMR_DEV_DRV_SONET_SDH_DisableAlarm(0,WPX_UFE_FRAMER_SDH_HP_PLM);
      framer_terminate_on_error("WPX_FRMR_DEV_DRV_SONET_SDH_DisableAlarm HP_PLM", status, __LINE__);

      status = WPX_FRMR_DEV_DRV_SONET_SDH_DisableAlarm(0,WPX_UFE_FRAMER_SDH_HP_UNEQ);
      framer_terminate_on_error("WPX_FRMR_DEV_DRV_SONET_SDH_DisableAlarm HP UNEQ", status, __LINE__);

      status = WPX_FRMR_DEV_DRV_SONET_SDH_DisableAlarm(0,WPX_UFE_FRAMER_SDH_HP_TIM);
      framer_terminate_on_error("WPX_FRMR_DEV_DRV_SONET_SDH_DisableAlarm HP TIM", status, __LINE__);
   }
#endif

   status = WPX_FRMR_SONET_SDH_CreateFacility(0, flexmux_id, &the_test.ufe[flexmux_id].line_end_point[line_port_id]);
   framer_terminate_on_error("WPX_FRMR_SONET_SDH_CreateFacility (LINE PORT)", status, __LINE__);

#if CREATE_FACILITY_WORKAROUND
   status = WPX_FRMR_SONET_SDH_HO_PATH_SetRdiMode(0, flexmux_id,
                                  &the_test.ufe[flexmux_id].line_end_point[line_port_id],
                                  WPX_UFE_FRAMER_RDI_MODE_ENABLE);
   framer_terminate_on_error("WPX_FRMR_SONET_SDH_HO_PATH_SetRdiMode", status, __LINE__);

   if (test_config.test_layer1_type == TEST_SDH)
   {
      status = WPX_FRMR_DEV_DRV_SONET_SDH_EnableAlarm(0,WPX_UFE_FRAMER_SDH_HP_PLM);
      framer_terminate_on_error("WPX_FRMR_DEV_DRV_SONET_SDH_EnableAlarm HP_PLM", status, __LINE__);

      status = WPX_FRMR_DEV_DRV_SONET_SDH_EnableAlarm(0,WPX_UFE_FRAMER_SDH_HP_UNEQ);
      framer_terminate_on_error("WPX_FRMR_DEV_DRV_SONET_SDH_EnableAlarm HP UNEQ", status, __LINE__);

      status = WPX_FRMR_DEV_DRV_SONET_SDH_EnableAlarm(0,WPX_UFE_FRAMER_SDH_HP_TIM);
      framer_terminate_on_error("WPX_FRMR_DEV_DRV_SONET_SDH_EnableAlarm HP TIM", status, __LINE__);
   }
#endif

   if (!drivers_is_up)
      TEST_SignalOverheadSet(flexmux_id, index, line_port_id);

   return 0;
}

WP_S32 TEST_FramerConnectionSet(int flexmux_id, int line_port_id, int client_port_id)
{
   WUFE_status status;
   int client_type = 0;

   memset(&the_test.ufe[flexmux_id].line_end_point[line_port_id], 0, sizeof(WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE));
   if (test_config.test_line_type == TEST_SDH_STM4 ||
       test_config.test_line_type == TEST_SONET_OC12)
   {
      if (test_config.test_line_type == TEST_SONET_OC12)
         the_test.ufe[flexmux_id].line_end_point[line_port_id].TransferType = WPX_UFE_FRAMER_WUFE_SONET_TYPE_STS12;
      else
         the_test.ufe[flexmux_id].line_end_point[line_port_id].TransferType = WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC4_4C;
      client_type = WPX_UFE_FRAMER_CLIENT_C4_4C;
   }
   else if (test_config.test_line_type == TEST_SDH_STM1)
   {
      the_test.ufe[flexmux_id].line_end_point[line_port_id].TransferType = WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC4;
      the_test.ufe[flexmux_id].line_end_point[line_port_id].u.SDH.stm1 = line_port_id;
      client_type = WPX_UFE_FRAMER_CLIENT_C4;
   }
   else if (test_config.test_line_type == TEST_SONET_OC3)
   {
      the_test.ufe[flexmux_id].line_end_point[line_port_id].TransferType = WPX_UFE_FRAMER_WUFE_SONET_TYPE_STS3;
      the_test.ufe[flexmux_id].line_end_point[line_port_id].u.SONET.sts3 = line_port_id;
      client_type = WPX_UFE_FRAMER_CLIENT_C4;
   }
   else
   {
      printf("Test Abort. TEST_FramerConnectionSet -> invalid line type\n");
      TEST_FailAndTerminate();
   }

   status = WPX_FRMR_SOCKET_PDH_CreateFacility(0, flexmux_id, client_port_id, client_type);
   framer_terminate_on_error("WPX_FRMR_SOCKET_PDH_CreateFacility (CLIENT)", status, __LINE__);

   /* create the connection between the client port and the line port */
   status = WPX_FRMR_CONNECTIONS_AddDropLineToSocketCreate(0, flexmux_id, &the_test.ufe[flexmux_id].line_end_point[line_port_id], client_port_id);
   framer_terminate_on_error("WPX_FRMR_CONNECTIONS_AddDropLineToSocketCreate", status, __LINE__);

   /* Set the PDH Tx line timing  */
   /* line_timing =  WPX_UFE_FRAMER_TRANSMIT_TIMING_SONET_TIMING |
      WPX_UFE_FRAMER_TRANSMIT_TIMING_RX_PATH; */
   if (test_host_traffic)
      status = WPX_FRMR_SOCKET_PDH_SetClkRec(0, flexmux_id, client_port_id, WPX_UFE_FRAMER_TRANSMIT_TIMING_SONET_TIMING);
   else
      status = WPX_FRMR_SOCKET_PDH_SetClkRec(0, flexmux_id, client_port_id, WPX_UFE_FRAMER_TRANSMIT_TIMING_RX_PATH);
   framer_terminate_on_error("WPX_FRMR_SOCKET_PDH_SetClkRec", status, __LINE__);

   return 0;
}

void TEST_FramerSerdesConfigure(int flexmux_id)
{
   if (test_config.test_line_type == TEST_SDH_STM1 ||
       test_config.test_line_type == TEST_SONET_OC3)
   {
      /* for WDS3 the TX DIFF SWING registers should be set to 1 for OC3 SFP optics.
         If the default values (0 ) are OK, then this function can be omitted */
      WPX_FRMR_DEV_DIAG_Poke(0, flexmux_id, 0x88000028, 0x00000001);
      WPX_FRMR_DEV_DIAG_Poke(0, flexmux_id, 0x88000228, 0x00000001);
      WPX_FRMR_DEV_DIAG_Poke(0, flexmux_id, 0x88000428, 0x00000001);
      WPX_FRMR_DEV_DIAG_Poke(0, flexmux_id, 0x88000628, 0x00000001);
      WPX_FRMR_DEV_DIAG_Poke(0, flexmux_id, 0x88000828, 0x00000001);
      WPX_FRMR_DEV_DIAG_Poke(0, flexmux_id, 0x88000a28, 0x00000001);
      WPX_FRMR_DEV_DIAG_Poke(0, flexmux_id, 0x88000c28, 0x00000001);
      WPX_FRMR_DEV_DIAG_Poke(0, flexmux_id, 0x88000e28, 0x00000001);
   }
   else
   {
      /* leave the default values */
      WPX_FRMR_DEV_DIAG_Poke(0, flexmux_id, 0x88000028, 0x00000000);
      WPX_FRMR_DEV_DIAG_Poke(0, flexmux_id, 0x88000228, 0x00000000);
      WPX_FRMR_DEV_DIAG_Poke(0, flexmux_id, 0x88000428, 0x00000000);
      WPX_FRMR_DEV_DIAG_Poke(0, flexmux_id, 0x88000628, 0x00000000);
      WPX_FRMR_DEV_DIAG_Poke(0, flexmux_id, 0x88000828, 0x00000000);
      WPX_FRMR_DEV_DIAG_Poke(0, flexmux_id, 0x88000a28, 0x00000000);
      WPX_FRMR_DEV_DIAG_Poke(0, flexmux_id, 0x88000c28, 0x00000000);
      WPX_FRMR_DEV_DIAG_Poke(0, flexmux_id, 0x88000e28, 0x00000000);
   }
}

WP_S32 TEST_FramerLoopbackSet(int flexmux_id, int port_id)
{
   WUFE_status status;

   if (test_loopback_type == TEST_LINE_SHALLOW)
   {
      printf("--- TEST_LINE_SHALLOW loopback ----\n");
      status = (WP_U8) WPX_FRMR_SONET_SDH_PORT_DIAG_LpbkEnableShallowLine(0, flexmux_id, port_id);
      framer_terminate_on_error("WPX_FRMR_SONET_SDH_PORT_DIAG_LpbkEnableShallowLine", status, __LINE__);
   }
   if (test_loopback_type == TEST_LINE_DEEP)
   {
      printf("--- TEST_LINE_DEEP loopback ----\n");
      status = (WP_U8) WPX_FRMR_SOCKET_PDH_DIAG_LpbkEnableDeepLine(0, flexmux_id, port_id);
      framer_terminate_on_error("WPX_FRMR_SOCKET_PDH_DIAG_LpbkEnableDeepLine", status, __LINE__);
   }
   if (test_loopback_type == TEST_SYSTEM_SHALLOW)
   {
      printf("--- TEST_SYSTEM_SHALLOW loopback ----\n");
      status = (WP_U8) WPX_FRMR_SOCKET_PDH_DIAG_LpbkEnableShallowSystem(0, flexmux_id, port_id);
      framer_terminate_on_error("WPX_FRMR_SOCKET_PDH_DIAG_LpbkEnableShallowSystem", status, __LINE__);
   }
   if (test_loopback_type == TEST_SYSTEM_DEEP)
   {
      printf("--- TEST_SYSTEM_DEEP loopback ----\n");
      status = (WP_U8) WPX_FRMR_SONET_SDH_PORT_DIAG_LpbkEnableDeepSystem(0, flexmux_id, port_id);
      framer_terminate_on_error("WPX_FRMR_SONET_SDH_PORT_DIAG_LpbkEnableDeepSystem", status, __LINE__);
   }
   if (test_loopback_type == TEST_LINE_SDH)
   {
      printf("--- TEST_LINE_SDH loopback ----\n");
      status = (WP_U8) WPX_FRMR_SONET_SDH_DIAG_LpbkEnable(0, flexmux_id, &the_test.ufe[flexmux_id].line_end_point[port_id]);
      framer_terminate_on_error("WPX_FRMR_SONET_SDH_DIAG_LpbkEnable", status, __LINE__);
   }

   return 0;
}

void TEST_SecondUfeBoardConfigure(WP_U32 wpid)
{
   WP_status status;

   status = WPX_BoardConfigure(wpid, WPX_CONFIGURE_UPI3_UFE412);
   TEST_TerminateOnError(status, "UPI3/UFE4/Spartan Board configuration");
}

WP_S32 TEST_UfeLoadFpga(int id)
{
   WUFE_init_config ufe_config;
   WUFE_status status;
   int connector = (TEST_PORT_IN_USE == WP_PORT_UPI1) ? 0 :
      ((TEST_PORT_IN_USE == WP_PORT_UPI2) ? 1 : 2);

   memset(&ufe_config, 0, sizeof(WUFE_init_config));
   ufe_config.ufe_id = 0;

   if (TEST_PORT_IN_USE == WP_PORT_UPI2)
      ufe_config.attributes = WUFE_ATTRIB_CONNECT_OFFSET_F(1);
   else if (TEST_PORT_IN_USE == WP_PORT_UPI3)
      ufe_config.attributes = WUFE_ATTRIB_CONNECT_OFFSET_F(2);

   if(id == UFE_SECONDARY)
   {
      if (test_config.test_dual_emphy)
      {
         ufe_config.attributes = WUFE_ATTRIB_CONNECT_OFFSET_F(2);
         connector = TEST_SECOND_CONNECTOR_PORT_ID;
      }
      else
      {
         printf("UFE ID greater than 0 used for single EMPHY mode!\n");
         WT_FailAndTerminate();
      }
   }

   if (test_config.test_ufe4_type == TEST_UFE412_REVA ||
       test_config.test_ufe4_type == TEST_UFE412_REVB)
   {
      /* FGPA HW reset */
      WPX_Ufe4HwReset(0, connector);
   }

   if(test_config.test_load_firmware)
   {
      /* Announce that UFE firmware will be initialized through host and not through JTAG */
      printf("******************************************************\n");
      printf(" UFE %d build configuration is loaded through the host!!!\n", id);
      printf("******************************************************\n");
      status = WUFE_UfeInitializeFirmware(&ufe_config);
      ufe_terminate_on_error(status, "WUFE_Ufe4InitializeFirmware");
   }

   return 0;
}


WP_S32 TEST_FramerCreateSingleLine(int id, int line)
{
   TEST_FramerLinePortInitialize(id, line);
   TEST_FramerConnectionSet(id, line, line*84);
   if (test_loopback_type != TEST_NORMAL)
   {
      TEST_FramerLoopbackSet(id, line*84);
   }
   return 0;
}

WP_S32 WTI_FlexmuxConnectionDelete(WP_U8 flexmux_id, int line_port_id, int client_port_id)
{
/* This function deletes an existing connection and the associated client and line port facilities */
   WUFE_status status;

   printf("delete: id %d line_port_id %d\n", flexmux_id, line_port_id);
   /* delete the connection between the client port and the line port */
   status = WPX_FRMR_CONNECTIONS_AddDropLineToSocketDelete(0, flexmux_id, &the_test.ufe[flexmux_id].line_end_point[line_port_id], client_port_id);
   framer_terminate_on_error("WPX_FRMR_CONNECTIONS_AddDropLineToSocketDelete", status, __LINE__);

   /* delete the line port facility */
   status = WPX_FRMR_SONET_SDH_DeleteFacility(0, flexmux_id, &the_test.ufe[flexmux_id].line_end_point[line_port_id]);
   framer_terminate_on_error("WPX_FRMR_SONET_SDH_DeleteFacility", status, __LINE__);

   /* delete the client port facility */
   status = WPX_FRMR_SOCKET_PDH_DeleteFacility(0, flexmux_id, client_port_id);
   framer_terminate_on_error("WPX_FRMR_SOCKET_PDH_DeleteFacility", status, __LINE__);

   return 0;
}

void TEST_FramerSetup(int ufe_id, int num_lines)
{
   if (!test_config.test_use_framer)
   {
      printf ("--- No Framer ----------\n");
      return;
   }

   printf("------ Init Framer -----\n");
   TEST_FramerInitialize(ufe_id);

   if (num_lines == 1)
      TEST_FramerCreateSingleLine(ufe_id, test_use_line);
   else
   {
      int i;
      for (i=0; i<num_lines; i++)
      {
         TEST_FramerCreateSingleLine(ufe_id, i);
      }
   }
   TEST_FramerSerdesConfigure(ufe_id);

   return;
}


WP_S32 TEST_UfeInitialize(int ufe_id, int num_lines)
{
   WUFE_init_config ufe_config;
   WUFE_status status;
   int test_device_type;

   if ((test_config.test_line_type == TEST_SDH_STM1) ||
       (test_config.test_line_type == TEST_SDH_STM4))
   {
      ufe_system_cfg.line_interface = WUFE_INTERFACE_SDH;
   }
   else
   {
      ufe_system_cfg.line_interface = WUFE_INTERFACE_SONET;
   }

   memset(&ufe_config, 0, sizeof(WUFE_init_config));
   ufe_config.ufe_id = ufe_id;
   the_test.ufe[ufe_id].ufe_id = ufe_config.ufe_id;

   if (TEST_PORT_IN_USE == WP_PORT_UPI2)
      ufe_config.attributes = WUFE_ATTRIB_CONNECT_OFFSET_F(1);
   else if (TEST_PORT_IN_USE == WP_PORT_UPI3)
      ufe_config.attributes = WUFE_ATTRIB_CONNECT_OFFSET_F(2);

   if (ufe_id > 0)
      ufe_config.attributes = WUFE_ATTRIB_CONNECT_OFFSET_F(2);

   status = WUFE_UfeInit(&ufe_config);
   ufe_terminate_on_error(status, "WUFE_UfeInit");

   test_device_type = WUFE_DEVICE_TYPE_UFE412_M4;
   status = WUFE_DeviceTypeSet(ufe_config.ufe_id, test_device_type);
   ufe_terminate_on_error(status, "WUFE_SystemModeSet  ");

   /* Define UFE System */
   ufe_system_cfg.clock_rec_sys_params = 0;

   /* Initialize the UFE system (system id = 0)  */
   status = WUFE_SystemConfig(ufe_id, &ufe_system_cfg);
   ufe_terminate_on_error(status, "WUFE_SystemConfig");

#if 0
   {
      WP_U32 nbytes = 0;
      WP_MemoryAvailable(WP_WINPATH(0), WP_BUS_INTERNAL, APP_BANK_INTERNAL, &nbytes);
      printf("Available on INTERNAL bus:  0x%x (%d) bytes\n", nbytes, nbytes);
   }
#endif

   TEST_FramerSetup(ufe_id, num_lines);

   status = WUFE_SystemEnable(ufe_id, WUFE_SYS_EMPHY, WUFE_FULL_DUPLEX);
   ufe_terminate_on_error(status, "WUFE_SystemEnable");

   ufe_has_been_initialized[ufe_id] = 1;

   return 0;
};

void TEST_UfeLineSdhConfig(WUFE_line_sdh *line_sdh_cfg, int line_index)
{
   memset(line_sdh_cfg, 0, sizeof(WUFE_line_sdh));
   line_sdh_cfg->struct_id = WUFE_STRUCT_LINE_SDH;
   if (test_config.test_ufe_clear_channel)
   {
      if (test_config.test_line_type == TEST_SDH_STM1)
         line_sdh_cfg->transfer_type = WUFE_SDH_TYPE_VC4;
      else if (test_config.test_line_type == TEST_SDH_STM4)
         line_sdh_cfg->transfer_type = WUFE_SDH_TYPE_VC44C;
      else
      {
         printf("Test Abort. TEST_UfelineSdhConfig -> line type is invalid.\n");
         TEST_FailAndTerminate();
      }
      line_sdh_cfg->framed = WUFE_UNFRAMED;
   }
   else
   {
      line_sdh_cfg->transfer_type = WUFE_SDH_TYPE_T1;
      line_sdh_cfg->framed = WUFE_FRAMED;
   }
   line_sdh_cfg->stm4 = 0;
   line_sdh_cfg->stm1 = line_index;
   line_sdh_cfg->stm0 = 0;
   line_sdh_cfg->tug2 = 0;
   line_sdh_cfg->tu = 0;
   line_sdh_cfg->cas_enable = WUFE_CAS_DISABLE;
}

void TEST_UfeLineSonetConfig(WUFE_line_sonet *line_sonet_cfg, int line_index)
{
   memset(line_sonet_cfg, 0, sizeof(WUFE_line_sonet));
   line_sonet_cfg->struct_id = WUFE_STRUCT_LINE_SONET;
   if (test_config.test_ufe_clear_channel)
   {
      if (test_config.test_line_type == TEST_SONET_OC3)
         line_sonet_cfg->transfer_type = WUFE_SONET_TYPE_STS3;
      else if (test_config.test_line_type == TEST_SONET_OC12)
         line_sonet_cfg->transfer_type = WUFE_SONET_TYPE_STS12;
      else
      {
         printf("Test Abort. TEST_UfelineSonetConfig -> line type is invalid.\n");
         TEST_FailAndTerminate();
      }
      line_sonet_cfg->framed = WUFE_UNFRAMED;
   }
   else
   {
      line_sonet_cfg->transfer_type = WUFE_SONET_TYPE_T1;
      line_sonet_cfg->framed = WUFE_FRAMED;
   }
   line_sonet_cfg->sts12 = 0;
   line_sonet_cfg->sts3 = line_index;
   line_sonet_cfg->sts1 = 0;
   line_sonet_cfg->vt_group = 0;
   line_sonet_cfg->vt = 0;
   line_sonet_cfg->cas_enable = WUFE_CAS_DISABLE;
}

WP_S32 TEST_UfeLinePortCreate(int ufe_id, int line_index, int is_atm)
{
   WUFE_status status;
   int line_type;
   void *line_cfg;
   WUFE_phy phy_cfg;

   if ((test_config.test_line_type == TEST_SDH_STM1) ||
       (test_config.test_line_type == TEST_SDH_STM4))
   {
      line_type = WUFE_STRUCT_LINE_SDH;
      TEST_UfeLineSdhConfig(&line_sdh_cfg, line_index);
      line_cfg = (void*)&line_sdh_cfg;
   }
   else
   {
      line_type = WUFE_STRUCT_LINE_SONET;
      TEST_UfeLineSonetConfig(&line_sonet_cfg, line_index);
      line_cfg = (void*)&line_sonet_cfg;
   }

   /* Initialize the UFE4 lines */
   status = WUFE_LineCreate(&the_test.ufe[ufe_id].line[line_index],
                            the_test.ufe[ufe_id].ufe_id,
                            line_type,
                            line_cfg);

   ufe_terminate_on_error(status, "WUFE_LineCreate");

   if (!test_config.test_use_framer)
   {
      /* Place in Loop-back mode - for debug */
      status = WUFE_LineLoopbackConfigure(the_test.ufe[ufe_id].line[line_index],
                                          WUFE_LINE_LOOPBACK_TYPE_SYSTEM);
      ufe_terminate_on_error(status, "WUFE_LineLoopbackConfigure");
   }

   /* Initialize the UFE4 phys */
   if (is_atm)
      memcpy(&phy_cfg, &ufe_phy_atm_cfg, sizeof(WUFE_phy));
   else
      memcpy(&phy_cfg, &ufe_phy_pos_cfg, sizeof(WUFE_phy));

   if (!test_config.test_ufe_clear_channel)
   {
      int i;

      phy_cfg.rx_slot_group.n_slots = 24;
      phy_cfg.tx_slot_group.n_slots = 24;
      for (i=0; i<24; i++)
      {
         phy_cfg.rx_slot_group.slot_arr[i] = i;
         phy_cfg.tx_slot_group.slot_arr[i] = i;
      }
      phy_cfg.tx_fifo_param.fast_mode = WUFE_PHY_FIFO_MODE_SLOW;
   }

   status = WUFE_PhyCreate(&the_test.ufe[ufe_id].phy[line_index],
                           the_test.ufe[ufe_id].line[line_index],
                           &phy_cfg,
                           WUFE_FULL_DUPLEX);
   ufe_terminate_on_error(status, "WUFE_PhyCreate");

   /* enable line and phy */
   status = WUFE_LineEnable(the_test.ufe[ufe_id].line[line_index], WUFE_FULL_DUPLEX);
   ufe_terminate_on_error(status, "WUFE_LineEnable() UFE4");

   status = WUFE_PhyEnable(the_test.ufe[ufe_id].phy[line_index], WUFE_FULL_DUPLEX);
   ufe_terminate_on_error(status, "WUFE_PhyEnable");

   return 0;
}

static void TEST_ReconfigureAtmPhy(WP_U32 ufe_id, WP_U32 phy, WP_U32 ch_index, WP_U32 n_chan, WUFE_phy *phy_cfg)
{
   WP_status status;
   WUFE_status ufe_status;
   WP_U32 depth;
   WP_tx_binding tx_binding_config;
   WP_boolean is_disabled;
   int j;

   if (test_iw_type == TEST_IWGP)
   {
      for (j = ch_index; j < ch_index + n_chan; ++j)
      {
         status = WP_PceRuleDisable(the_test.atm[j].flow_handle);
         TEST_TerminateOnError(status, "WP_PceRuleDisable");

         do {
            status = WP_ChannelQDepth(the_test.atm[j].tx_ch, &depth);
            TEST_TerminateOnError(status, "WP_ChannelQDepth");
         } while (depth != 1);
      }
   }
   else
   {
      memset(&tx_binding_config, 0, sizeof(WP_tx_binding));

      for (j = ch_index; j < ch_index + n_chan; ++j)
      {
         status = WP_IwTxBindingModify(the_test.atm[j].tx_ch,
                                     WP_IW_TX_BIND_MOD_DCI|WP_IW_TX_BIND_MOD_MAXT,
                                     WP_IW_TX_BINDING,
                                     &tx_binding_config);
         TEST_TerminateOnError(status, "WP_IwTxBindingModify #1");

         status = WP_ChannelDisable(the_test.atm[j].tx_ch);
         if (status != WP_OK && WP_ERROR(status) != WP_ERR_CH_ALREADY_DISABLED)
            TEST_TerminateOnError(status, "WP_ChannelDisable");

         is_disabled = WP_FALSE;
         do {

            status = WP_ChannelStatus(the_test.atm[j].tx_ch, WP_CH_STATUS_DISABLED, &is_disabled);
            TEST_TerminateOnError(status, "WP_ChannelStatus");

         } while (is_disabled == WP_FALSE);
      }
   }

   status = WP_DeviceDisable(the_test.device[phy].handle, WP_DIRECTION_DUPLEX);
   TEST_TerminateOnError(status, "WP_DeviceDisable");
   
   ufe_status = WUFE_PhyDisable(the_test.ufe[ufe_id].phy[phy], WUFE_FULL_DUPLEX);
   ufe_terminate_on_error(ufe_status, "WUFE_PhyDisable");

   WPL_Delay(20000);

   ufe_status = WUFE_PhyReconfigure(the_test.ufe[ufe_id].phy[phy], phy_cfg);
   ufe_terminate_on_error(ufe_status, "WUFE_PhyReconfigure");

   ufe_status = WUFE_PhyEnable(the_test.ufe[ufe_id].phy[phy], WUFE_FULL_DUPLEX);
   ufe_terminate_on_error(ufe_status, "WUFE_PhyEnable");
   
   status = WP_DeviceEnable(the_test.device[phy].handle, WP_DIRECTION_DUPLEX);
   TEST_TerminateOnError(status, "WP_DeviceEnable");
   
   if (test_iw_type == TEST_IWGP)
   {
      for (j = ch_index; j < ch_index + n_chan; ++j)
      {
         status = WP_PceRuleEnable(the_test.atm[j].flow_handle);
         TEST_TerminateOnError(status, "WP_PceRuleEnable");
      }
   }
   else
   {
      for (j = ch_index; j < ch_index + n_chan; ++j)
      {
         tx_binding_config.maxt = 1024;
         tx_binding_config.dci_mode = atm_dci_mode;

         status = WP_IwTxBindingModify(the_test.atm[j].tx_ch,
                                     WP_IW_TX_BIND_MOD_DCI|WP_IW_TX_BIND_MOD_MAXT,
                                     WP_IW_TX_BINDING,
                                     &tx_binding_config);
         TEST_TerminateOnError(status, "WP_IwTxBindingModify #2");

         status = WP_ChannelEnable(the_test.atm[j].tx_ch);
         TEST_TerminateOnError(status, "WP_ChannelEnable");
      }
   }
}

static void TEST_ReconfigurePosPhy(WP_U32 ufe_id, WP_U32 phy, WP_U32 flow, WUFE_phy *phy_cfg)
{
   WP_status status;
   WUFE_status ufe_status;
   WP_U32 depth;
   WP_tx_binding tx_binding_config;
   WP_boolean is_disabled;

   if (test_iw_type == TEST_IWGP)
   {
      status = WP_PceRuleDisable(the_test.pos[flow].egress_flow[flow]);
      TEST_TerminateOnError(status, "WP_PceRuleDisable");

      do {
         status = WP_ChannelQDepth(the_test.pos[flow].tx_ch[flow], &depth);
         TEST_TerminateOnError(status, "WP_ChannelQDepth");
      } while (depth != 1);
   }
   else
   {
      memset(&tx_binding_config, 0, sizeof(WP_tx_binding));

      status = WP_IwTxBindingModify(the_test.pos[flow].tx_ch[flow],
                                  WP_IW_TX_BIND_MOD_DCI|WP_IW_TX_BIND_MOD_MAXT,
                                  WP_IW_TX_BINDING,
                                  &tx_binding_config);
      TEST_TerminateOnError(status, "WP_IwTxBindingModify #1");

      status = WP_ChannelDisable(the_test.pos[flow].tx_ch[flow]);
      if (status != WP_OK && WP_ERROR(status) != WP_ERR_CH_ALREADY_DISABLED)
         TEST_TerminateOnError(status, "WP_ChannelDisable");

      is_disabled = WP_FALSE;
      do {

         status = WP_ChannelStatus(the_test.pos[flow].tx_ch[flow], WP_CH_STATUS_DISABLED, &is_disabled);
         TEST_TerminateOnError(status, "WP_ChannelStatus");

      } while (is_disabled == WP_FALSE);
   }

   status = WP_DeviceDisable(the_test.device[flow].handle, WP_DIRECTION_DUPLEX);
   TEST_TerminateOnError(status, "WP_DeviceDisable");
   
   ufe_status = WUFE_PhyDisable(the_test.ufe[ufe_id].phy[phy], WUFE_FULL_DUPLEX);
   ufe_terminate_on_error(ufe_status, "WUFE_PhyDisable");

   WPL_Delay(20000);

   ufe_status = WUFE_PhyReconfigure(the_test.ufe[ufe_id].phy[phy], phy_cfg);
   ufe_terminate_on_error(ufe_status, "WUFE_PhyReconfigure");

   ufe_status = WUFE_PhyEnable(the_test.ufe[ufe_id].phy[phy], WUFE_FULL_DUPLEX);
   ufe_terminate_on_error(ufe_status, "WUFE_PhyEnable");
   
   status = WP_DeviceEnable(the_test.device[flow].handle, WP_DIRECTION_DUPLEX);
   TEST_TerminateOnError(status, "WP_DeviceEnable");
   
   if (test_iw_type == TEST_IWGP)
   {
      status = WP_PceRuleEnable(the_test.pos[flow].egress_flow[flow]);
      TEST_TerminateOnError(status, "WP_PceRuleEnable");
   }
   else
   {
      tx_binding_config.maxt = 1024;
      tx_binding_config.dci_mode = pos_dci_mode;

      status = WP_IwTxBindingModify(the_test.pos[flow].tx_ch[flow],
                                  WP_IW_TX_BIND_MOD_DCI|WP_IW_TX_BIND_MOD_MAXT,
                                  WP_IW_TX_BINDING,
                                  &tx_binding_config);
      TEST_TerminateOnError(status, "WP_IwTxBindingModify #2");

      status = WP_ChannelEnable(the_test.pos[flow].tx_ch[flow]);
      TEST_TerminateOnError(status, "WP_ChannelEnable");
   }
}

static void TEST_ReconfigureAtmPhy(WP_U32 ufe_id, WP_U32 phy, WP_U32 ch_index, WP_U32 n_chan, WUFE_phy *phy_cfg);

static void CLI_F_ConfigurePhyScrambling(WP_CHAR *strPrm)
{
   WP_U32 res, cmd, phy, rx_on, tx_on;
   WUFE_phy phy_cfg;
   WP_U32 ufe_id;
   int first_atm_path;
   int j;

   res = sscanf(strPrm, "%d %d %d %d", &cmd, &phy, &rx_on, &tx_on);
   if(res != 4)
   {
      printf("Invalid command. Usage: <cmd> <phy> <rx_on> <tx_on> \n");
      return;
   }

#if defined(__WT_UFE4_DUAL_EMPHY__)
   
   if (phy >= 8)
   {
      printf("Invalid phy %d\n", phy);
      return;
   }

#else
   
   if (phy >= 4)
   {
      printf("Invalid phy %d\n", phy);
      return;
   }

#endif

#if defined(__WT_UFE4_DUAL_EMPHY__)

   ufe_id = (phy < 4) ? 0 : 1;

#else

   ufe_id = 0;

#endif

   if (test_config.test_use_atm == 1)
   {
      memset(&phy_cfg, WUFE_ATM_NO_CHANGE, sizeof(WUFE_phy));

      if (rx_on)
         phy_cfg.type.atm.rx_descrambler_mode = WUFE_PHY_ATM_RX_DESCRAMBLER_ENABLE;
      else
         phy_cfg.type.atm.rx_descrambler_mode = WUFE_PHY_ATM_RX_DESCRAMBLER_DISABLE;

      if (tx_on)
         phy_cfg.type.atm.tx_scrambler_mode = WUFE_PHY_ATM_TX_SCRAMBLER_ENABLE;
      else
         phy_cfg.type.atm.tx_scrambler_mode = WUFE_PHY_ATM_TX_SCRAMBLER_DISABLE;

      first_atm_path = phy * test_config.test_atm_chans;

      for (j = first_atm_path; j < first_atm_path + test_config.test_atm_chans; ++j)
      {
         if (the_test.atm[j].state == 0)
         {
            printf("Identified ATM path not configured : %d\n", j);
            return;
         }
      }

      TEST_ReconfigureAtmPhy(ufe_id, (ufe_id == 1) ? phy - 4 : phy, first_atm_path,
                             test_config.test_atm_chans, &phy_cfg);
   }
   else if (test_config.test_use_pos == 1)
   {
      memset(&phy_cfg, WUFE_POS_NO_CHANGE, sizeof(WUFE_phy));


      if (rx_on)
         phy_cfg.type.pos.rx_descrambler_mode = WUFE_PHY_POS_RX_DESCRAMBLER_ENABLE;
      else
         phy_cfg.type.pos.rx_descrambler_mode = WUFE_PHY_POS_RX_DESCRAMBLER_DISABLE;

      if (tx_on)
         phy_cfg.type.pos.tx_scrambler_mode = WUFE_PHY_POS_TX_SCRAMBLER_ENABLE;
      else
         phy_cfg.type.pos.tx_scrambler_mode = WUFE_PHY_POS_TX_SCRAMBLER_DISABLE;

      TEST_ReconfigurePosPhy(ufe_id, (ufe_id == 1) ? phy - 4 : phy, phy, &phy_cfg);
   }
   else
      printf("Feature only available on ATM or POS phy\n");
}

static void CLI_F_ConfigurePhyCrc(WP_CHAR *strPrm)
{
   WP_U32 res, cmd, phy, rx_crc, tx_crc;
   WUFE_phy phy_cfg;
   WP_U32 ufe_id;

   res = sscanf(strPrm, "%d %d %d %d", &cmd, &phy, &rx_crc, &tx_crc);
   if(res != 4)
   {
      printf("Invalid command. Usage: <cmd> <phy> <rx_crc> <tx_crc> \n");
      return;
   }

#if defined(__WT_UFE4_DUAL_EMPHY__)
   
   if (phy >= 8)
   {
      printf("Invalid phy %d\n", phy);
      return;
   }

#else
   
   if (phy >= 4)
   {
      printf("Invalid phy %d\n", phy);
      return;
   }

#endif

   if (rx_crc >= 2 || tx_crc >= 2)
   {
      printf("Invalid CRC:  crc == 0 ==> 16; crc == 1 ==> 32\n");
      return;
   }

   memset(&phy_cfg, WUFE_POS_NO_CHANGE, sizeof(WUFE_phy));

#if defined(__WT_UFE4_DUAL_EMPHY__)

   ufe_id = (phy < 4) ? 0 : 1;

#else

   ufe_id = 0;

#endif

   if (rx_crc == 0)
      phy_cfg.type.pos.rx_frame_crc_size = WUFE_PHY_HDLC_CRC_16;
   else
      phy_cfg.type.pos.rx_frame_crc_size = WUFE_PHY_HDLC_CRC_32;

   if (tx_crc == 0)
      phy_cfg.type.pos.tx_frame_crc_size = WUFE_PHY_HDLC_CRC_16;
   else
      phy_cfg.type.pos.tx_frame_crc_size = WUFE_PHY_HDLC_CRC_32;

   TEST_ReconfigurePosPhy(ufe_id, (ufe_id == 1) ? phy - 4 : phy, phy, &phy_cfg);
}

static void CLI_F_ConfigureDeviceCrc(WP_CHAR *strPrm)
{
   WP_U32 res, cmd, phy, crc;
   WP_device_emphy_hdlc_ppp hdlc_device_cfg;
   WUFE_status status;
   WP_U32 depth;

   res = sscanf(strPrm, "%d %d %d", &cmd, &phy, &crc);
   if(res != 3)
   {
      printf("Invalid command. Usage: <cmd> <phy> <crc>\n");
      return;
   }

#if defined(__WT_UFE4_DUAL_EMPHY__)
   
   if (phy >= 8)
   {
      printf("Invalid phy %d\n", phy);
      return;
   }

#else
   
   if (phy >= 4)
   {
      printf("Invalid phy %d\n", phy);
      return;
   }

#endif

   if (crc >= 2)
   {
      printf("Invalid CRC:  crc == 0 ==> 16; crc == 1 ==> 32\n");
      return;
   }

   memset(&hdlc_device_cfg, 0, sizeof(WP_device_emphy_hdlc_ppp));

   if (crc == 0)
      hdlc_device_cfg.crctype = WP_HDLC_CRC16;
   else
      hdlc_device_cfg.crctype = WP_HDLC_CRC32;

   status = WP_PceRuleDisable(the_test.pos[phy].egress_flow[0]);
   TEST_TerminateOnError(status, "WP_PceRuleDisable");

   do {
      status = WP_ChannelQDepth(the_test.pos[phy].tx_ch[0], &depth);
      TEST_TerminateOnError(status, "WP_ChannelQDepth");
   } while (depth != 1);

   status = WP_DeviceDisable(the_test.device[phy].handle, WP_DIRECTION_DUPLEX);
   TEST_TerminateOnError(status, "WP_DeviceDisable");

   WPL_Delay(20000);

   status = WP_DeviceModify(the_test.device[phy].handle, WP_DEV_MOD_UPI_POS_CRC_TYPE, &hdlc_device_cfg);
   TEST_TerminateOnError(status, "WP_DeviceModify");

   status = WP_DeviceEnable(the_test.device[phy].handle, WP_DIRECTION_DUPLEX);
   TEST_TerminateOnError(status, "WP_DeviceEnable");
   
   status = WP_PceRuleEnable(the_test.pos[phy].egress_flow[0]);
   TEST_TerminateOnError(status, "WP_PceRuleEnable");
}

static void TEST_DeleteUfePorts(void)
{
   WUFE_status status;
   int i, n;

   for (n=0; n<TEST_MAX_UFE_DEVICES; n++)
   {
      printf("Delete UFE%d\n", n);
      for (i=0; i<TEST_UFE_MAX_LINE_PORTS;i++)
      {
         if (the_test.ufe[n].line[i] != 0)
         {
            if (the_test.ufe[n].phy[i] != 0)
            {
               printf("Delete PHY %d\n", i);
               status = WUFE_PhyDisable(the_test.ufe[n].phy[i], WUFE_FULL_DUPLEX);
               ufe_terminate_on_error(status, "WUFE_PhyDisable() UFE4");

               status = WUFE_PhyDelete(the_test.ufe[n].phy[i]);
               ufe_terminate_on_error(status, "WUFE_PhyDelete()");
               the_test.ufe[n].phy[i] = 0;
            }
            printf("Delete Line %d\n", i);
            status = WUFE_LineDisable(the_test.ufe[n].line[i], WUFE_FULL_DUPLEX);
            ufe_terminate_on_error(status, "WUFE_LineDisable() UFE4");

            status = WUFE_LineDelete(the_test.ufe[n].line[i]);
            ufe_terminate_on_error(status, "WUFE_LineDelete()");
            the_test.ufe[n].line[i] = 0;

            if (test_config.test_use_framer)
            {
               printf("Delete flexmux connection %d\n", i);
               WTI_FlexmuxConnectionDelete(n, i, i*84);
            }
         }
         else continue;
      }
   }
}


static void TEST_UfeAnalizeEmphyEvents(WUFE_events_emphy *emphy_events, WP_U32 coreid)
{
   WP_U16 event = emphy_events->event_reg;

   printf("Emphy event register for core id %d: 0x%04x\n", coreid, event);
   if (event & WUFE_EVENT_EMPHY)
   {
      if (event & WUFE_EVENT_EMPHY_INGRESS_BUS)
         printf("WUFE_EVENT_EMPHY_INGRESS_BUS\n");
      if (event & WUFE_EVENT_EMPHY_INGRESS_MAX_BUFF_SIZE)
         printf("WUFE_EVENT_EMPHY_INGRESS_MAX_BUFF_SIZE\n");
      if (event & WUFE_EVENT_EMPHY_INGRESS_IF)
         printf("WUFE_EVENT_EMPHY_INGRESS_IF\n");
      if (event & WUFE_EVENT_EMPHY_EGRESS_PHY_NOT_VALID)
      {
         printf("WUFE_EVENT_EMPHY_EGRESS_PHY_NOT_VALID\n");
         printf("phy_not_valid_handle = 0x%x\n",
                emphy_events->phy_not_valid_handle);
      }
      if (event & WUFE_EVENT_EMPHY_EGRESS_BUS)
         printf("WUFE_EVENT_EMPHY_EGRESS_BUS\n");
      if (event & WUFE_EVENT_EMPHY_EGRESS_IF )
         printf("WUFE_EVENT_EMPHY_EGRESS_IF\n");
      if (event &WUFE_EVENT_EMPHY_EGRESS_PARITY )
         printf("WUFE_EVENT_EMPHY_EGRESS_PARITY\n");
   }
}

static void TEST_UfeAnalizeTdmEvents(WUFE_events_tdm *tdm_events)
{
   WP_U16 event = tdm_events->event_reg;

   printf("TDM event register 0x%04x\n", event);
   if (event & WUFE_EVENT_TDM)
   {
      if (event & WUFE_EVENT_TDM_RX_PARITY)
         printf("WUFE_EVENT_TDM_RX_PARITY\n");
      if (event & WUFE_EVENT_TDM_TX_PARITY)
         printf("WUFE_EVENT_TDM_TX_PARITY\n");
   }
}

static void TEST_UfeAnalizeFmEvents(WUFE_events_fm *fm_events)
{
   WP_U16 event = fm_events->event_reg;

   printf("FM event register 0x%04x\n", event);
   if (event & WUFE_EVENT_FM)
   {
      if (event & WUFE_EVENT_FM_RX_DDR_OVERRUN)
         printf("WUFE_EVENT_FM_RX_DDR_OVERRUN\n");
      if (event & WUFE_EVENT_FM_RX_BURST_OVERRUN)
         printf("WUFE_EVENT_FM_RX_BURST_OVERRUN\n");
      if (event & WUFE_EVENT_FM_TX_DDR_UNDERRUN)
         printf("WUFE_EVENT_FM_TX_DDR_UNDERRUN\n");
      if (event & WUFE_EVENT_FM_TX_BURST_UNDERRUN)
         printf("WUFE_EVENT_FM_TX_BURST_UNDERRUN\n");
      if (event & WUFE_EVENT_FM_RX_ECC_ONE_BIT)
         printf("WUFE_EVENT_FM_RX_ECC_ONE_BIT\n");
      if (event & WUFE_EVENT_FM_TX_ECC_ONE_BIT)
         printf("WUFE_EVENT_FM_TX_ECC_ONE_BIT\n");
      if (event & WUFE_EVENT_FM_TX_READ_FIFO_OVERRUN)
         printf("WUFE_EVENT_FM_TX_READ_FIFO_OVERRUN\n");
      if (event & WUFE_EVENT_FM_RX_VFP_OVERRUN)
         printf("WUFE_EVENT_FM_RX_VFP_OVERRUN\n");
   }
}

static void TEST_UfeAnalizeMachineEvents(WUFE_events_machine *machine_events)
{
   WP_U16 event = machine_events->rx_event_reg;

   printf("Machine event Rx register 0x%04x\n", event);
   if (event & WUFE_EVENT_MACHINE_RX)
   {
      if (event & WUFE_EVENT_MACHINE_CES_RX_LOSS)
         printf("WUFE_EVENT_MACHINE_CES_RX_LOSS\n");
      if (event & WUFE_EVENT_MACHINE_CES_RX_MF_LOSS)
         printf("WUFE_EVENT_MACHINE_CES_RX_MF_LOSS\n");
      if (event & WUFE_EVENT_MACHINE_ATM_RX_LCD)
         printf("WUFE_EVENT_MACHINE_ATM_RX_LCD\n");
      if (event & WUFE_EVENT_MACHINE_HDLC_RX_ABORT)
         printf("WUFE_EVENT_MACHINE_HDLC_RX_ABORT\n");
      if (event & WUFE_EVENT_MACHINE_HDLC_RX_CRC_ERR)
         printf("WUFE_EVENT_MACHINE_HDLC_RX_CRC_ERR\n");
      if (event & WUFE_EVENT_MACHINE_HDLC_RX_NON_OCTET)
         printf("WUFE_EVENT_MACHINE_HDLC_RX_NON_OCTET\n");
      if (event & WUFE_EVENT_MACHINE_POS_RX_ABORT)
         printf("WUFE_EVENT_MACHINE_POS_RX_ABORT\n");
      if (event & WUFE_EVENT_MACHINE_POS_CRC_ERR)
         printf("WUFE_EVENT_MACHINE_POS_CRC_ERR\n");
      if (event & WUFE_EVENT_MACHINE_RX_OVERRUN_BURST)
         printf("WUFE_EVENT_MACHINE_RX_OVERRUN_BURST\n");
      if (event & WUFE_EVENT_MACHINE_RX_OVERRUN_DDR)
         printf("WUFE_EVENT_MACHINE_RX_OVERRUN_DDR\n");
   }

   event = machine_events->tx_event_reg;

   printf("Machine event Tx register 0x%04x\n", event);
   if (event & WUFE_EVENT_MACHINE_TX)
   {
      if (event & WUFE_EVENT_MACHINE_CES_TX_LOSS)
         printf("WUFE_EVENT_MACHINE_CES_TX_LOSS\n");
      if (event & WUFE_EVENT_MACHINE_TX_UNDERRUN_BURST)
         printf("WUFE_EVENT_MACHINE_TX_UNDERRUN_BURST\n");
      if (event & WUFE_EVENT_MACHINE_TX_UNDERRUN_DDR)
         printf("WUFE_EVENT_MACHINE_TX_UNDERRUN_DDR\n");
   }
   printf("\n");
}

static void TEST_UfeAnalizeEvents(WUFE_events *ufe_events)
{
   WP_U32 coreid;

   for(coreid=0; coreid<2; coreid++)
   {
      TEST_UfeAnalizeEmphyEvents(&(ufe_events->emphy_events[coreid]), coreid);
   }

   TEST_UfeAnalizeTdmEvents(&(ufe_events->tdm_events));
   TEST_UfeAnalizeFmEvents(&(ufe_events->fm_events));
   TEST_UfeAnalizeMachineEvents(&(ufe_events->machine_events));
}


void TEST_UfeReadEvents(void)
{
   WUFE_status status;
   WUFE_events ufe_events;

   memset(&ufe_events, 0, sizeof(WUFE_events));

   status = WUFE_UfeReadEvents(0, &ufe_events);
   ufe_terminate_on_error(status, "WUFE_UfeReadEvents ");

   printf("Events for ufe_id %d\n", the_test.ufe[UFE_PRIMARY].ufe_id);
   TEST_UfeAnalizeEvents(&ufe_events);

   if (test_config.test_dual_emphy)
   {
      memset(&ufe_events, 0, sizeof(WUFE_events));
      
      status = WUFE_UfeReadEvents(UFE_SECONDARY, &ufe_events);
      ufe_terminate_on_error(status, "WUFE_UfeReadEvents ");
      
      printf("Events for ufe_id %d\n", the_test.ufe[UFE_SECONDARY].ufe_id);
      TEST_UfeAnalizeEvents(&ufe_events);
   }
}

WP_atm_shaping_unshaped pw_unshaped_params;
TEST_pw_config pw_atm_conn;
TEST_pos_params pos_flow;
TEST_enet_header enet_header;
TEST_ip_header ip_header;
TEST_udp_header udp_header;
WP_atm_shaping_hi_speed hi_speed_param = { 622000000/8/53, 353208/10, 4 };

void TEST_PwAtmConfigure(TEST_pw_config *pw_config, int ingress_index, int egress_index, int enet_id)
{
   memset(pw_config, 0, sizeof(TEST_pw_config));

   if (test_host_traffic)
   {
      pw_config->rx_vpi = 10;
      pw_config->rx_vci = 40+ingress_index;
      pw_config->tx_vpi = 10;
      pw_config->tx_vci = 50+ingress_index;
   }
   else
   {
      pw_config->rx_vpi = 10;
      pw_config->rx_vci = 40+ingress_index;
      pw_config->tx_vpi = 10;
      pw_config->tx_vci = 40+egress_index;
   }

#if TEST_USING_AAL5
   pw_config->encapsulation = WP_PWE3_ATM_AAL5_SDU;
#else
   pw_config->encapsulation = ENCAPSULATION;
   pw_config->cells_per_packet = 28;
#endif
   pw_config->sn_mode = WP_DISABLE;//WP_ENABLE;
#if TEST_USING_HI_SPEED
   pw_config->tx_shaping_type = WP_ATM_SHAPING_HI_SPEED;
   pw_config->tx_shaping_params = &hi_speed_param;
#else
   pw_config->tx_shaping_type = WP_ATM_SHAPING_UNSHAPED;
   pw_config->tx_shaping_params = &pw_unshaped_params;
   pw_unshaped_params.weight = 1;
#endif
   pw_config->tx_tq = 1;
   pw_config->rx_header_replace_mode = WP_PWE3_REPLACE_NOTHING;
   pw_config->tx_header_replace_mode = WP_PWE3_REPLACE_NOTHING;
   TEST_CreateAtmHeader((WP_U8 *)&pw_config->header, pw_config->tx_vpi, pw_config->tx_vci, 0);
   pw_config->iw_host_rx_index = -1; /* use IW host rx channel with index 0 */
   pw_config->enet_header = &enet_header;
   pw_config->ip_header = &ip_header;
   pw_config->udp_header = &udp_header;
   memcpy(&enet_header, &the_test.enet[enet_id].header, sizeof(TEST_enet_header));
   enet_header.vlan_id = TEST_PWE3ATM_VLAN_ID_START + egress_index;

   printf("%s : VLAN_ID %d\n", __FUNCTION__, enet_header.vlan_id);

   if (test_iw_type != TEST_IWCP_MPLS)
   {
      memset(&ip_header, 0, sizeof(TEST_ip_header));
      ip_header.ver_ihl = 0x45;
      ip_header.tos = 0;
      ip_header.total_length = 0;
      ip_header.id = 0;
      ip_header.frag_offset = 0;
      ip_header.ttl = 64;
      ip_header.protocol = 0x11;
      ip_header.checksum = 0;
      ip_header.source_addr = 0xC0000264 + ingress_index; /* 192.0.2.100 */
      ip_header.dest_addr = 0xC0000464 + ingress_index; /* 192.0.4.100 */

      memset(&udp_header, 0, sizeof(TEST_udp_header));
      udp_header.source_port = 100;
      udp_header.dest_port = 200;
      udp_header.length = 40;
      udp_header.checksum = 0x2264;
   }
}

void TEST_HostPwAtmConfigure(TEST_pw_config *pw_config, int index)
{
   memset(pw_config, 0, sizeof(TEST_pw_config));

   pw_config->rx_vpi = 10;
   pw_config->rx_vci = 50+index;
   pw_config->tx_vpi = 10;
   pw_config->tx_vci = 40+index;
   pw_config->tx_shaping_type = WP_ATM_SHAPING_UNSHAPED;
   pw_config->tx_shaping_params = &pw_unshaped_params;
   pw_unshaped_params.weight = 1;
   pw_config->iw_host_rx_index = -1; /* use IW host rx channel with index 0 */
}

void TEST_DriversRelease(int state)
{
   WUFE_status ufe_status;
   WP_status status;
   WP_boolean is_enabled;
   int jj;

   if (test_config.test_use_framer)
   {
      WPX_Ufe412CpldInterruptMaskSet(WP_WINPATH(DEFAULT_WPID), WPX_FPGA_INTR_ALL_MASKED);

      if (ufe_has_been_initialized[UFE_PRIMARY])
         WPX_FRMR_LowLevelCommsDisableAccess(UFE_PRIMARY);

      if (test_config.test_dual_emphy)
         if (ufe_has_been_initialized[UFE_SECONDARY])
            WPX_FRMR_LowLevelCommsDisableAccess(UFE_SECONDARY);

      WPL_IntDisable(DEFAULT_WPID, WPL_Eint3Ufe4);
      WPL_IntConnect(DEFAULT_WPID, WPL_Eint3Ufe4, 0, 0);

      for (jj = 0; jj < N_UFE_DEVICES; ++jj)
      {
         if (ufe_has_been_initialized[jj] == 0)
            continue;

         ufe_status = WUFE_SystemInterruptDisable(jj);
         ufe_terminate_on_error(ufe_status, "WUFE_SystemInterruptDisable");
         is_enabled = WUFE_UfeCoreHwInterruptGet(jj);
         if (is_enabled)
         {
            ufe_status = WUFE_UfeCoreHwInterruptDisable(jj);
            ufe_terminate_on_error(ufe_status, "WUFE_UfeCoreHwInterruptDisable");
         }
      }

#if defined(__linux__)
      if (WPL_ThreadDestroy(&mailbox_thread_id) != 0)
      {
         printf("Test Abort. The WPL_ThreadDestroy failed\n");
      }
#endif
   }

   if (test_config.test_layer1_type != TEST_OC12_CARD)
   {
      if (test_config.test_dual_emphy)
         if (ufe_has_been_initialized[UFE_SECONDARY] == 1)
            WUFE_UfeRelease(UFE_SECONDARY);
      
      if (ufe_has_been_initialized[UFE_PRIMARY] == 1)
         WUFE_UfeRelease(UFE_PRIMARY);
   }
   
   if (state == 0)
   {
      status = WP_DriverRelease();
      TEST_TerminateOnError(status, "WP_DriverRelease");
   }
}

void TEST_UfeDevicesInitialize(int num_lines)
{
   memset(ufe_has_been_initialized, 0, sizeof(ufe_has_been_initialized));

   if (test_config.test_dual_emphy)
   {
      if (num_lines > 2 * TEST_UFE_MAX_LINE_PORTS)
      {     
         /* This indicates 2xOC12 */
         TEST_UfeInitialize(UFE_PRIMARY, 1);
         TEST_UfeInitialize(UFE_SECONDARY, 1);
      }
      else if (num_lines >= TEST_UFE_MAX_LINE_PORTS)
      {
         TEST_UfeInitialize(UFE_PRIMARY, TEST_UFE_MAX_LINE_PORTS);
         TEST_UfeInitialize(UFE_SECONDARY, num_lines - TEST_UFE_MAX_LINE_PORTS);
      }
      else
      {
         TEST_UfeInitialize(UFE_PRIMARY, num_lines);
      }
   }
   else
      TEST_UfeInitialize(UFE_PRIMARY, num_lines);
}

void TEST_UfeDevicesFramerSetup(int num_lines)
{
   if (test_config.test_dual_emphy)
   {
      if (num_lines > 2 * TEST_UFE_MAX_LINE_PORTS)
      {     
         /* This indicates 2xOC12 */
         TEST_FramerSetup(UFE_PRIMARY, 1);
         TEST_FramerSetup(UFE_SECONDARY, 1);
      }
      else if (num_lines >= TEST_UFE_MAX_LINE_PORTS)
      {
         TEST_FramerSetup(UFE_PRIMARY, TEST_UFE_MAX_LINE_PORTS);
         TEST_FramerSetup(UFE_SECONDARY, num_lines - TEST_UFE_MAX_LINE_PORTS);
      }
      else
      {
         TEST_FramerSetup(UFE_PRIMARY, num_lines);
      }
   }
   else
      TEST_FramerSetup(UFE_PRIMARY, num_lines);
}


int TEST_Initialize(int num_lines)
{
   if (test_config.test_layer1_type == TEST_OC12_CARD)
      test_config.test_use_framer = 0;

   if (!drivers_is_up)
   {
      TEST_ResetTheTestParameters();
      TEST_WinPathInitialize();
      TEST_EnetSerdesConfigure();
      if (test_config.test_layer1_type != TEST_OC12_CARD)
      {
         TEST_UfeDevicesInitialize(num_lines);
      }
      drivers_is_up = 1;
   }
   else
   {
      TEST_EnetSerdesConfigure();
      TEST_UfeDevicesFramerSetup(num_lines);
      TEST_SchedulersEnable();
   }

   TEST_IngressIwSystemCreate();

   return 0;
}

void TEST_DeleteOldConfiguration(void)
{
   if (!drivers_is_up)
      return;

   test_loopback_type = TEST_NORMAL;
   printf("------ Delete Configuration ------\n");
   TEST_DeleteWinpathConnections();
   TEST_DeletePorts();
   if (test_config.test_layer1_type != TEST_OC12_CARD)
   {
      TEST_DeleteUfePorts();
   }
   test_config.test_use_snake_path = 0;
   test_config.test_dual_emphy = 0;
}


#define N_FRAMES 5
WP_U16 frame_size[N_FRAMES] = {30, 100, 67, 127, 257};

int test_failures = 0;

WP_S32 TEST_ConfigureAtmPath(int first_line, int last_line, int num_ch)
{
   int i;
   int line_status = 0;
   int is_fast = 0;
   int line, ufe_line, ingress_line, egress_line;
   int ufe_id;
   int enet_id;

   /* Create all ATM devices first */
   if (test_config.test_layer1_type != TEST_OC12_CARD)
   {
      if (test_config.test_ufe_clear_channel)
         is_fast = 1;

      for (line=first_line; line <= last_line; line++)
      {
         ufe_id = TEST_GetUfeId(line);
         if (ufe_id == UFE_PRIMARY)
            ufe_line = line;
         else
            ufe_line = line - TEST_UFE_MAX_LINE_PORTS;
         TEST_UfeLinePortCreate(ufe_id, ufe_line, 1);
         TEST_AtmOverUfeDeviceCreate(line, ufe_line, is_fast);
      }
   }

   /* Configure ATM PW per each line */
   for (line=first_line; line <= last_line; line++)
   {
      printf("--- Run ATM test for line %d ---\n", line);

      enet_id = TEST_GetEnetId(line);

      ingress_line = egress_line = line;
      if (test_config.test_use_snake_path)
         egress_line = ((line+1)%TEST_UFE_MAX_LINE_PORTS);

      for (i=0; i<num_ch; i++)
      {
         TEST_PwAtmConfigure(&pw_atm_conn, ingress_line*num_ch+i, egress_line*num_ch+i, enet_id);
         TEST_PwAtmCreate(&pw_atm_conn, line*num_ch+i, ingress_line, egress_line, enet_id);
      }

      if (test_host_traffic)
      {
         line_status += TEST_AtmHostTraffic(&pw_atm_conn, line, N_FRAMES, frame_size);
      }
      else
      {
         printf("Start Traffic for line %d...\n", line);
      }
   }

   return line_status;
}


WP_S32 TEST_RunAtmScenario(int num_lines, int num_ch)
{
   int test_init_status = 0;
   int run_status = 0;
   int first_line, last_line;

   printf("--- TEST_RunAtmScenario ---\n");
   test_init_status = TEST_Initialize(num_lines);
   if (test_init_status != 0)
      return 0;

   test_config.test_atm_chans = num_ch;

   if (num_lines > 2 * TEST_UFE_MAX_LINE_PORTS)
   {
      printf("--- Configure 1 port on UFE1 and 1 port on UFE2 ---\n");
      run_status = TEST_ConfigureAtmPath(0, 0, num_ch);
      run_status += TEST_ConfigureAtmPath(TEST_UFE_MAX_LINE_PORTS, TEST_UFE_MAX_LINE_PORTS, num_ch);
   }
   else
   {
      if (num_lines == 1)
      {
         if (test_config.test_use_snake_path)
         {
            printf("Snake can't be used for single line configuration\n");
            TEST_FailAndTerminate();
         }
         first_line = test_use_line;
         last_line = test_use_line;
      }
      else
      {
         first_line = 0;
         last_line = num_lines-1;
      }

      run_status = TEST_ConfigureAtmPath(first_line, last_line, num_ch);
   }

   return run_status;
}


void TEST_PsuCirEirShapingConfigure(TEST_pos_params *params)
{
   printf("-------- CIR/EIR PKT shaping ---------\n");
   params->tx_shaping_type = WP_PKT_SHAPING_CIR_EIR;
   params->tx_shaping_params = &pkt_cir_eir_shaping;
   if ((test_config.test_line_type == TEST_SDH_STM1) || (test_config.test_line_type == TEST_SONET_OC3))
      pkt_cir_eir_shaping.cir = 139760000 / test_config.test_pos_flows; /* OC3 /4 */
   else
      pkt_cir_eir_shaping.cir = 599040000 / test_config.test_pos_flows; /* OC12 / 4 */
   pkt_cir_eir_shaping.cir_polling = pkt_cir_eir_shaping.cir / 4;
   pkt_cir_eir_shaping.cbs =  pkt_cir_eir_shaping.cir / 8 / 1400;
   pkt_cir_eir_shaping.eir = pkt_cir_eir_shaping.cir;
   pkt_cir_eir_shaping.ebs = pkt_cir_eir_shaping.cbs;
   pkt_cir_eir_shaping.eir_cwid = WP_CW_ID_B;
}

void TEST_PsuPprShapingConfigure(TEST_pos_params *params)
{
   printf("-------- PPR PKT shaping ---------\n");
   params->tx_shaping_type = WP_PKT_SHAPING_PPR;
   params->tx_shaping_params = &pkt_ppr_shaping;
   if ((test_config.test_line_type == TEST_SDH_STM1) || (test_config.test_line_type == TEST_SONET_OC3))
      pkt_ppr_shaping.ppr = 139760000 / test_config.test_pos_flows; /* OC3 /4 */
   else
      pkt_ppr_shaping.ppr = 599040000 / test_config.test_pos_flows; /* OC12 / 4 */
   pkt_ppr_shaping.polling = WP_SHAPING_POLL_AUTOD;
}

void TEST_PsuCirShapingConfigure(TEST_pos_params *params)
{
   printf("-------- CIR PKT shaping ---------\n");
   params->tx_shaping_type = WP_PKT_SHAPING_CIR;
   params->tx_shaping_params = &pkt_cir_shaping;
   if ((test_config.test_line_type == TEST_SDH_STM1) || (test_config.test_line_type == TEST_SONET_OC3))
      pkt_cir_shaping.cir = 139760000 / test_config.test_pos_flows; /* OC3 /4 */
   else
      pkt_cir_shaping.cir = 599040000 / test_config.test_pos_flows; /* OC12 / 4 */
   pkt_cir_shaping.polling = pkt_cir_shaping.cir / 4;
   pkt_cir_shaping.cbs =  pkt_cir_shaping.cir / 8 / 1400;
}

void TEST_FmuCirEirConfigure(TEST_pos_params *params)
{
   params->tx_shaping_type = WP_FMU_SHAPING_TYPE_CIR_EIR;
   params->tx_shaping_params = &fmu_cir_eir_shaping;
   if ((test_config.test_line_type == TEST_SDH_STM1) || (test_config.test_line_type == TEST_SONET_OC3))
      fmu_cir_eir_shaping.cir = 139760000 / test_config.test_pos_flows; /* OC3 /4 */
   else
      fmu_cir_eir_shaping.cir = 599040000 / test_config.test_pos_flows; /* OC12 / 4 */
   fmu_cir_eir_shaping.cbs = fmu_cir_eir_shaping.cir / 8 / 1400; /* flow rate / 8 = bytes/sec / 1400 = packets/sec */
   fmu_cir_eir_shaping.eir = 0;
   fmu_cir_eir_shaping.ebs = 0;
   fmu_cir_eir_shaping.flags = 0;
}


void TEST_PosFlowConfigure(TEST_pos_params *params, int index, int egress_line_index, int enet_id)
{
   memset(params, 0, sizeof(TEST_pos_params));
   /* the following parameters are not specified as default unshaped type is used
    * in this test case:
    WP_U8 tx_shaping_type;
    void *tx_shaping_params;
    WP_U8 tx_tq;
   */
   memcpy(&enet_header, &the_test.enet[enet_id].header, sizeof(TEST_enet_header));
   enet_header.vlan_id = TEST_PPP_VLAN_ID_START + index;
   params->enet_header = &enet_header;

   printf("%s : VLAN_ID %d\n", __FUNCTION__, enet_header.vlan_id);

   memset(&ip_header, 0, sizeof(TEST_ip_header));
   ip_header.ver_ihl = 0x45;
   ip_header.tos = 0;
   ip_header.total_length = 0;
   ip_header.id = 0;
   ip_header.frag_offset = 0;
   ip_header.ttl = 64;
   ip_header.protocol = 0x11;
   ip_header.checksum = 0;

#if 0
   ip_header.source_addr = 0xC0000164 + index; /* 192.0.1.100 */
   ip_header.dest_addr = 0xC0000364 + index; /* 192.0.3.100 */
#else
   /* always use the same IP address for different UFE devices, but use
    * different IW port and different VLAN_ID on Ethernet side */
   ip_header.source_addr = 0xC0000164;
   ip_header.dest_addr = 0xC0000364;
#endif   
   params->ip_header = &ip_header;

   if (test_config.test_pkt_scheduler_used)
   {
      if (test_config.test_use_pos)
      {
         if (test_config.test_psu_shaping_type == TEST_PKT_CIR_EIR_SHAPING)
         {
            TEST_PsuCirEirShapingConfigure(params);
         }
         else if (test_config.test_psu_shaping_type == TEST_PKT_PPR_SHAPING)
         {
            TEST_PsuPprShapingConfigure(params);
         }
         else if (test_config.test_psu_shaping_type == TEST_PKT_CIR_SHAPING)
         {
            TEST_PsuCirShapingConfigure(params);
         }
      }
      if (test_config.test_use_hspos)
      {
         TEST_FmuCirEirConfigure(params);
      }
      params->tx_tq = 0;
   }
}

WP_U16 pos_frame_size[N_FRAMES] = {48, 64, 67, 250, 1300};
//WP_U16 pos_frame_size[N_FRAMES] = {1300, 64, 64, 64, 64};

WP_S32 TEST_ConfigurePosPath(int first_line, int last_line, int num_flows_per_line)
{
   int is_fast = 0;
   int line_status = 0;
   int i;
   int line, ufe_line, ingress_line, egress_line;
   int ufe_id, enet_id;

printf("%s:  first_line %d, last_line %d, flows_per_line %d\n", __FUNCTION__, first_line, last_line, num_flows_per_line);

   /* Create all POS devices first */
   if (test_config.test_layer1_type != TEST_OC12_CARD)
   {
      if (test_config.test_ufe_clear_channel)
         is_fast = 1;

      for (line=first_line; line <= last_line; line++)
      {
         ufe_id = TEST_GetUfeId(line);
         if (ufe_id == UFE_PRIMARY)
            ufe_line = line;
         else
            ufe_line = line - TEST_UFE_MAX_LINE_PORTS;
         TEST_UfeLinePortCreate(ufe_id, ufe_line, 0);
         TEST_PosOverUfeDeviceCreate(line, ufe_line, is_fast);
      }
   }

   /* Create POS flows now */
   for (line=first_line; line <= last_line; line++)
   {
      printf("--- Run POS test for line %d ---\n", line);
      ingress_line = egress_line = line;
      if (test_config.test_use_snake_path)
      {
         egress_line = ((line+1)%TEST_UFE_MAX_LINE_PORTS);

         if (line >= TEST_UFE_MAX_LINE_PORTS)
            egress_line += TEST_UFE_MAX_LINE_PORTS;
      }

      enet_id = TEST_GetEnetId(line);
      for (i=0; i<num_flows_per_line; i++)
      {
         TEST_PosFlowConfigure(&pos_flow, line*num_flows_per_line + i, egress_line*num_flows_per_line + i, enet_id);

         TEST_PosFlowCreate(&pos_flow, line*num_flows_per_line + i,
                            ingress_line, egress_line, enet_id);
      }

      if (test_host_traffic)
      {
         /* send dummy packet (dropped due to first CRC error */
#if 0
         if (test_config.test_use_hspos)
         {
            TEST_PosHostTraffic(line, 0, NULL, enet_id);
         }
#endif
         line_status += TEST_PosHostTraffic(line, N_FRAMES, pos_frame_size, enet_id);
      }
      else
      {
         printf("Start Traffic for line %d...\n", line);
      }
   }

   return line_status;
}

WP_S32 TEST_RunPosScenario(int num_lines, int num_flows_per_line)
{
   int test_init_status = 0;
   int run_status = 0;
   int first_line, last_line;

   printf("--- TEST_RunPosScenario ---\n");
   test_init_status = TEST_Initialize(num_lines);
   if (test_init_status != 0)
      return 0;

   if (num_lines > 2 * TEST_UFE_MAX_LINE_PORTS)
   {
      printf("--- Configure 1 port on UFE1 and 1 port on UFE2 ---\n");
      run_status = TEST_ConfigurePosPath(0, 0, num_flows_per_line);
      run_status += TEST_ConfigurePosPath(TEST_UFE_MAX_LINE_PORTS, TEST_UFE_MAX_LINE_PORTS, num_flows_per_line);
   }
   else 
   {
      if (num_lines == 1)
      {
         if (test_config.test_use_snake_path)
         {
            printf("Snake can't be used for single line configuration\n");
            TEST_FailAndTerminate();
         }
         first_line = test_use_line;
         last_line = test_use_line;
      }
      else
      {
         first_line = 0;
         last_line = num_lines-1;
      }
      
      run_status = TEST_ConfigurePosPath(first_line, last_line, num_flows_per_line);
   }

   return (run_status);
}


WP_S32 TEST_RunLineLoopbackScenario(void)
{
   TEST_DeleteOldConfiguration();
   test_config.test_use_framer = 1;
   TEST_Initialize(1);

   return 0;
}

static void CLI_F_ShallowLineLoopback(WP_CHAR *StrPrm)
{
   WP_U32 res, cmd;

   res = sscanf(StrPrm, "%d", &cmd);
   if(res != 1)
   {
      printf("Invalid command.\n");
      return;
   }

   if (test_config.test_layer1_type == TEST_OC12_CARD)
   {
      printf("This option is not supported for OC12_CARD layer1 type.\n");
      return;
   }

   test_loopback_type = TEST_LINE_SHALLOW;

   printf("------ RUN SHALLOW LINE LOOPBACK TEST ---------\n");
   TEST_RunLineLoopbackScenario();
}


static void CLI_F_SDHLineLoopback(WP_CHAR *StrPrm)
{
   WP_U32 res, cmd;

   res = sscanf(StrPrm, "%d", &cmd);
   if(res != 1)
   {
      printf("Invalid command.\n");
      return;
   }

   if (test_config.test_layer1_type == TEST_OC12_CARD)
   {
      printf("This option is not supported for OC12_CARD layer1 type.\n");
      return;
   }
   test_loopback_type = TEST_LINE_SDH;

   printf("------ RUN SDH LINE LOOPBACK TEST ---------\n");
   TEST_RunLineLoopbackScenario();
}

static void CLI_F_DeepLineLoopback(WP_CHAR *StrPrm)
{
   WP_U32 res, cmd;

   res = sscanf(StrPrm, "%d", &cmd);
   if(res != 1)
   {
      printf("Invalid command.\n");
      return;
   }

   if (test_config.test_layer1_type == TEST_OC12_CARD)
   {
      printf("This option is not supported for OC12_CARD layer1 type.\n");
      return;
   }
   test_loopback_type = TEST_LINE_DEEP;

   printf("------ RUN DEEP LINE LOOPBACK TEST ---------\n");
   TEST_RunLineLoopbackScenario();
}

static void CLI_F_RunAtmHostTest(WP_CHAR *StrPrm)
{
   WP_U32 res, cmd;
   int n_lines;

   res = sscanf(StrPrm, "%d", &cmd);
   if(res != 1)
   {
      printf("Invalid command.\n");
      return;
   }

   TEST_DeleteOldConfiguration();

   if (test_config.test_layer1_type == TEST_OC12_CARD)
   {
      printf("This option is not supported for OC12_CARD layer1 type.\n");
      return;
   }

   test_host_traffic = 1;
   test_config.test_use_atm = 1;
   test_config.test_use_pos = 0;
   test_config.test_use_hspos = 0;
   if (test_config.test_layer1_type == TEST_SDH)
      test_config.test_line_type = TEST_SDH_STM1;
   else
      test_config.test_line_type = TEST_SONET_OC3;

   if (test_config.test_dual_emphy)
      n_lines = 8;
   else
      n_lines = 4;

   printf("------ Run ATM %dxOC3 host scenario ---------\n", n_lines);
   test_failures = TEST_RunAtmScenario(n_lines, 1);
   if (test_failures)
      printf("ATM Clear Channel Failed.\n");
   else
      printf("ATM Clear Channel Passed.\n");
}

static void CLI_F_RunAtmIwTest_4xOC3(WP_CHAR *StrPrm)
{
   WP_U32 res, cmd;
   char use_snake[10];

   res = sscanf(StrPrm, "%d %s", &cmd, use_snake);
   if (res == 1)
      test_config.test_use_snake_path = 1;
   else if (res == 2)
   {
      if (strcmp(use_snake, "-no-snake") != 0)
      {
         printf("Invalid option %s. Allowed option -no-snake.\n", use_snake);
         return;
      }
      test_config.test_use_snake_path = 0;
   }

   TEST_DeleteOldConfiguration();

   test_host_traffic = 0;
   test_config.test_use_atm = 1;
   test_config.test_use_pos = 0;
   test_config.test_use_hspos = 0;
   if (test_config.test_layer1_type == TEST_SDH)
      test_config.test_line_type = TEST_SDH_STM1;
   else
      test_config.test_line_type = TEST_SONET_OC3;

   printf("------ Run ATM 4xOC3 scenario ---------\n");
   test_use_line = 0;
   TEST_RunAtmScenario(4, 5);
}

static void CLI_F_RunAtmIwTest_1xOC3(WP_CHAR *StrPrm)
{
   WP_U32 res, cmd, line;

   res = sscanf(StrPrm, "%d %d", &cmd, &line);
   if(res != 2)
   {
      printf("Invalid command. Usage: <cmd> <line 0-3>\n");
      return;
   }

   TEST_DeleteOldConfiguration();

   test_host_traffic = 0;
   test_config.test_use_atm = 1;
   test_config.test_use_pos = 0;
   test_config.test_use_hspos = 0;

   if (test_config.test_layer1_type == TEST_SDH)
      test_config.test_line_type = TEST_SDH_STM1;
   else
      test_config.test_line_type = TEST_SONET_OC3;

   printf("------ Run ATM 1xOC3 scenario ---------\n");
   test_use_line = line;
   TEST_RunAtmScenario(1, 20);
}

static void CLI_F_RunAtmIwTest_1xOC12(WP_CHAR *StrPrm)
{
   WP_U32 res, cmd, line;

   res = sscanf(StrPrm, "%d %d", &cmd, &line);
   if(res != 2)
   {
      printf("Invalid command. Usage: <cmd> <line 0-3>\n");
      return;
   }

   TEST_DeleteOldConfiguration();

   test_host_traffic = 0;
   test_config.test_use_atm = 1;
   test_config.test_use_pos = 0;
   test_config.test_use_hspos = 0;

   if (test_config.test_layer1_type == TEST_SDH)
      test_config.test_line_type = TEST_SDH_STM4;
   else
      test_config.test_line_type = TEST_SONET_OC12;

   printf("------ Run ATM 1xOC12 scenario ---------\n");
   test_use_line = line;
#if TEST_USING_HI_SPEED
   TEST_RunAtmScenario(1, 1);
#else
   TEST_RunAtmScenario(1, 20);
#endif
}

static void CLI_F_RunPosHostTest(WP_CHAR *StrPrm)
{
   WP_U32 res, cmd;
   int n_lines;

   res = sscanf(StrPrm, "%d", &cmd);
   if(res != 1)
   {
      printf("Invalid command.\n");
      return;
   }

   if (test_config.test_layer1_type == TEST_OC12_CARD)
   {
      printf("This option is not supported for OC12_CARD layer1 type.\n");
      return;
   }

   TEST_DeleteOldConfiguration();

   test_host_traffic = 1;
   test_config.test_use_pos = 1;
   test_config.test_use_atm = 0;
   test_config.test_use_hspos = 0;

   if (test_config.test_layer1_type == TEST_SDH)
      test_config.test_line_type = TEST_SDH_STM1;
   else
      test_config.test_line_type = TEST_SONET_OC3;

   test_use_line = 0;

   if (test_config.test_dual_emphy)
      n_lines = 8;
   else
      n_lines = 4;

   printf("------ Run POS %dxOC3 host scenario ---------\n", n_lines);
   test_failures = TEST_RunPosScenario(n_lines, 1);
   if (test_failures)
      printf("POS Clear Channel Failed.\n");
   else
      printf("POS Clear Channel Passed.\n");
}

static void CLI_F_RunPosIwTest_1xOC12(WP_CHAR *StrPrm)
{
   WP_U32 res, cmd, line;

   res = sscanf(StrPrm, "%d %d", &cmd, &line);
   if(res != 2)
   {
      printf("Invalid command. Usage: <cmd> <line 0-3>\n");
      return;
   }

   TEST_DeleteOldConfiguration();

   test_host_traffic = 0;
   test_config.test_use_pos = 1;
   test_config.test_use_atm = 0;
   test_config.test_use_hspos = 0;

   if (test_config.test_layer1_type == TEST_SDH)
      test_config.test_line_type = TEST_SDH_STM4;
   else
      test_config.test_line_type = TEST_SONET_OC12;

   printf("------ Run POS 1xOC12 scenario ---------\n");
   test_use_line = line;

   TEST_RunPosScenario(1, test_config.test_pos_flows);
}

static void CLI_F_RunPosIwTest_4xOC3(WP_CHAR *StrPrm)
{
   WP_U32 res, cmd;
   char use_snake[10];
   int snake_path = 0;

   res = sscanf(StrPrm, "%d %s", &cmd, use_snake);
   if (res == 1)
      snake_path = 1;
   else if (res == 2)
   {
      if (strcmp(use_snake, "-no-snake") != 0)
      {
         printf("Invalid option %s. Allowed option -no-snake.\n", use_snake);
         return;
      }
      snake_path = 0;
   }

   TEST_DeleteOldConfiguration();

   test_host_traffic = 0;
   test_config.test_use_pos = 1;
   test_config.test_use_atm = 0;
   test_config.test_use_hspos = 0;
   test_config.test_use_snake_path = snake_path;

   if (test_config.test_layer1_type == TEST_SDH)
      test_config.test_line_type = TEST_SDH_STM1;
   else
      test_config.test_line_type = TEST_SONET_OC3;

   printf("------ Run POS 4xOC3 scenario (%s)---------\n", (snake_path)?"snake path":"no snake");
   test_use_line = 0;
   TEST_RunPosScenario(4, test_config.test_pos_flows);
}

static void CLI_F_RunPosIwTest_1xOC3(WP_CHAR *StrPrm)
{
   WP_U32 res, cmd, line;

   res = sscanf(StrPrm, "%d %d", &cmd, &line);
   if(res != 2)
   {
      printf("Invalid command. Usage: <cmd> <line 0-3>\n");
      return;
   }

   TEST_DeleteOldConfiguration();

   test_host_traffic = 0;
   test_config.test_use_pos = 1;
   test_config.test_use_atm = 0;
   test_config.test_use_hspos = 0;

   if (test_config.test_layer1_type == TEST_SDH)
      test_config.test_line_type = TEST_SDH_STM1;
   else
      test_config.test_line_type = TEST_SONET_OC3;

   printf("------ Run POS 1xOC3 scenario ---------\n");
   test_use_line = line;
   TEST_RunPosScenario(1, test_config.test_pos_flows);
}

static void CLI_F_RunHsposHostTest(WP_CHAR *StrPrm)
{
   WP_U32 res, cmd;
   int n_lines;

   res = sscanf(StrPrm, "%d", &cmd);
   if(res != 1)
   {
      printf("Invalid command. Usage: <cmd> \n");
      return;
   }

   if (test_config.test_layer1_type == TEST_OC12_CARD)
   {
      printf("This option is not supported for OC12_CARD layer1 type.\n");
      return;
   }

   TEST_DeleteOldConfiguration();

   test_host_traffic = 1;
   test_config.test_use_pos = 0;
   test_config.test_use_atm = 0;
   test_config.test_use_hspos = 1;

   if (test_config.test_layer1_type == TEST_SDH)
      test_config.test_line_type = TEST_SDH_STM1;
   else
      test_config.test_line_type = TEST_SONET_OC3;

   test_use_line = 0;

   if (test_config.test_dual_emphy)
      n_lines = 8;
   else
      n_lines = 4;

   printf("------ Run HSPOS %dxOC3 host scenario ---------\n", n_lines);
   test_failures = TEST_RunPosScenario(n_lines, 1);

   if (test_failures)
      printf("HSPOS Clear Channel Failed.\n");
   else
      printf("HSPOS Clear Channel Passed.\n");
}

static void CLI_F_RunHsposIwTest_1xOC12(WP_CHAR *StrPrm)
{
   WP_U32 res, cmd, line;

   res = sscanf(StrPrm, "%d %d", &cmd, &line);
   if(res != 2)
   {
      printf("Invalid command. Usage: <cmd> <line 0-3>\n");
      return;
   }

   TEST_DeleteOldConfiguration();

   test_host_traffic = 0;
   test_config.test_use_pos = 0;
   test_config.test_use_atm = 0;
   test_config.test_use_hspos = 1;
   test_config.test_hspos_use_iwgp = 1;

   if (test_config.test_layer1_type == TEST_SDH)
      test_config.test_line_type = TEST_SDH_STM4;
   else
      test_config.test_line_type = TEST_SONET_OC12;

   printf("------ Run HSPOS 1xOC12 scenario ---------\n");
   test_use_line = line;
   TEST_RunPosScenario(1, test_config.test_pos_flows);
}

static void CLI_F_RunHsposIwTest_4xOC3(WP_CHAR *StrPrm)
{
   WP_U32 res, cmd;
   int snake_path=0;
   char use_snake[10];

   res = sscanf(StrPrm, "%d %s", &cmd, use_snake);
   if (res == 1)
      snake_path = 1;
   else if (res == 2)
   {
      if (strcmp(use_snake, "-no-snake") != 0)
      {
         printf("Invalid option %s. Allowed option -no-snake.\n", use_snake);
         return;
      }
      snake_path = 0;
   }

   TEST_DeleteOldConfiguration();

   test_host_traffic = 0;
   test_config.test_use_pos = 0;
   test_config.test_use_atm = 0;
   test_config.test_use_hspos = 1;
   test_config.test_hspos_use_iwgp = 1;
   test_config.test_use_snake_path = snake_path;

   if (test_config.test_layer1_type == TEST_SDH)
      test_config.test_line_type = TEST_SDH_STM1;
   else
      test_config.test_line_type = TEST_SONET_OC3;

   printf("------ Run HSPOS 4xOC3 scenario (%s)---------\n", (snake_path)?"snake path":"no snake");
   test_use_line = 0;
   TEST_RunPosScenario(4, test_config.test_pos_flows);
}

static void CLI_F_RunHsposIwTest_8xOC3(WP_CHAR *StrPrm)
{
   WP_U32 res, cmd;
   int snake_path=0;
   char use_snake[10];

   if (!test_config.test_dual_emphy)
   {
      printf("This command is not allowed in the current configuration.\n");
      return;
   }

   res = sscanf(StrPrm, "%d %s", &cmd, use_snake);
   if (res == 1)
      snake_path = 1;
   else if (res == 2)
   {
      if (strcmp(use_snake, "-no-snake") != 0)
      {
         printf("Invalid option %s. Allowed option -no-snake.\n", use_snake);
         return;
      }
      snake_path = 0;
   }

   TEST_DeleteOldConfiguration();

   test_host_traffic = 0;
   test_config.test_use_pos = 0;
   test_config.test_use_atm = 0;
   test_config.test_use_hspos = 1;
   test_config.test_hspos_use_iwgp = 1;
   test_config.test_use_snake_path = snake_path;
   if (test_config.test_layer1_type == TEST_SDH)
      test_config.test_line_type = TEST_SDH_STM1;
   else
      test_config.test_line_type = TEST_SONET_OC3;
   test_config.test_dual_emphy = 1;

   printf("------ Run HSPOS 8xOC3 scenario (%s)---------\n", (snake_path)?"snake path":"no snake");
   test_use_line = 0;
   TEST_RunPosScenario(8, test_config.test_pos_flows);
}

static void CLI_F_RunHsposIwTest_1xOC3(WP_CHAR *StrPrm)
{
   WP_U32 res, cmd, line;

   res = sscanf(StrPrm, "%d %d", &cmd, &line);
   if(res != 2)
   {
      printf("Invalid command. Usage: <cmd> <line 0-3>\n");
      return;
   }

   TEST_DeleteOldConfiguration();

   test_host_traffic = 0;
   test_config.test_use_pos = 0;
   test_config.test_use_atm = 0;
   test_config.test_use_hspos = 1;
   test_config.test_hspos_use_iwgp = 1;

   if (test_config.test_layer1_type == TEST_SDH)
      test_config.test_line_type = TEST_SDH_STM1;
   else
      test_config.test_line_type = TEST_SONET_OC3;

   printf("------ Run HSPOS 1xOC3 scenario ---------\n");
   test_use_line = line;
   TEST_RunPosScenario(1, test_config.test_pos_flows);
}


static void CLI_F_Statistics(WP_CHAR *StrPrm)
{
   WP_U32 res, cmd;
   int ch, dev;

   res = sscanf(StrPrm, "%d %d %d", &cmd, &dev, &ch);
   if(res != 3)
   {
      printf("Invalid command. Usage: <cmd> <device_id> <channel_id>\n");
      return;
   }

   if (ch >= TEST_MAX_CHANNELS)
   {
      printf("Test Abort. CLI_F_Statistics -> Invalid channel index\n");
      TEST_FailAndTerminate();
   }
   if (dev >= TEST_MAX_WDDI_DEVICES)
   {
      printf("Test Abort. CLI_F_Statistics -> Invalid device index\n");
      TEST_FailAndTerminate();
   }

   if (!drivers_is_up)
   {
      printf("Invalid command. The test scenario should be run first.\n");
      return;
   }

   TEST_Statistics(dev, ch);
}

static void CLI_F_UfeEvents(WP_CHAR *StrPrm)
{
   WP_U32 res, cmd;

   res = sscanf(StrPrm, "%d", &cmd);
   if(res != 1)
   {
      printf("Invalid command.\n");
      return;
   }

   if (!drivers_is_up)
   {
      printf("Invalid command. The test scenario should be run first.\n");
      return;
   }

   TEST_UfeReadEvents();
}

static void CLI_F_UfePhyDisplay(WP_CHAR *StrPrm)
{
   WP_U32 res, cmd, phy, id;
   
   res = sscanf(StrPrm, "%d %d %d", &cmd, &id, &phy);
   if(res != 3)
   {
      printf("Invalid command. Usage: <cmd> <ufe_id> <phy>\n");
      return;
   }

   if (phy >= TEST_UFE_MAX_LINE_PORTS)
   {
      printf("Invalid PHY number\n");
      return;
   }

   if (id > UFE_SECONDARY)
   {
      printf("Invalid UFE Id %d\n", id);
      return;
   }

   if (!drivers_is_up)
   {
      printf("Invalid command. The test scenario should be run first.\n");
      return;
   }

   if (the_test.ufe[id].ufe_id == id)
      WUFE_PhyDisplay(the_test.ufe[id].phy[phy]);
   else
      printf("Invalid flexmux_id\n");
}

static void CLI_F_ConfigureLayer1Type(WP_CHAR *StrPrm)
{
   WP_U32 res, cmd, type;

   res = sscanf(StrPrm, "%d %d", &cmd, &type);
   if(res != 2)
   {
      printf("Invalid command. Usage: <cmd> <type: 0-SDH, 1-SONET, 2- UPI NORMAL OC12>\n");
      return;
   }

   if (type > 2)
   {
      printf("Invalid type number: %d. Must be 0-SDH, 1-SONET, 2- UPI NORMAL OC12\n", type);
      return;
   }

   test_config.test_layer1_type = type;
}

static void CLI_F_ConfigureUfeRevType(WP_CHAR *StrPrm)
{
   WP_U32 res, cmd, type;

   res = sscanf(StrPrm, "%d %d", &cmd, &type);
   if(res != 2)
   {
      printf("Invalid command. Usage: <cmd> <type: 0-RevA, 1-RevB\n");
      return;
   }

   if (type > 1)
   {
      printf("Invalid type number: %d. Must be  0-RevA, 1-RevB\n", type);
      return;
   }

   test_config.test_ufe4_type = type;
   printf("-- The UFE4 Rev%s type is chosen --\n", (type == 0)? "A" : "B");
}

static void CLI_F_ConfigureUseFramer(WP_CHAR *StrPrm)
{
   WP_U32 res, cmd, type;

   res = sscanf(StrPrm, "%d %d", &cmd, &type);
   if(res != 2)
   {
      printf("Invalid command. Usage: <cmd> <type: 0-no framer, 1-use framer\n");
      return;
   }

   if (type > 1)
   {
      printf("Invalid type number: %d. Must be  0-no framer, 1-use framer\n", type);
      return;
   }

   test_config.test_use_framer = type;
   printf("-- The UFE4 framer is %s used --\n", (type == 0)?"not":"");
}

static void CLI_F_ConfigureLoadFpga(WP_CHAR *StrPrm)
{
   WP_U32 res, cmd, type;

   res = sscanf(StrPrm, "%d %d", &cmd, &type);
   if(res != 2)
   {
      printf("Invalid command. Usage: <cmd> <type: 0-no load, 1-load\n");
      return;
   }

   if (type > 1)
   {
      printf("Invalid type number: %d. Must be  0-no load, 1-load\n", type);
      return;
   }

   test_config.test_load_firmware = type;
   printf("-- The FPGA is %s loaded by the host --\n", (type == 0)?"not":"");
}

static void CLI_F_ConfigureHsposIwType(WP_CHAR *StrPrm)
{
   WP_U32 res, cmd, type;

   res = sscanf(StrPrm, "%d %d", &cmd, &type);
   if(res != 2)
   {
      printf("Invalid command. Usage: <cmd> <type: 0-IWCP, 1-IWGP\n");
      return;
   }

   if (type > 1)
   {
      printf("Invalid type number: %d. Must be  0-IWCP, 1-IWGP\n", type);
      return;
   }

   test_config.test_hspos_use_iwgp = type;
   printf("-- The HSPOS Ingress IW type is set to %s --\n", (type == 0)?"IWCP":"IWGP");
}

static void CLI_F_ConfigureIwPortStats(WP_CHAR *StrPrm)
{
   WP_U32 res, cmd, type;

   res = sscanf(StrPrm, "%d %d", &cmd, &type);
   if(res != 2)
   {
      printf("Invalid command. Usage: <cmd> <type: 0-disable, 1-enable\n");
      return;
   }

   if (type > 1)
   {
      printf("Invalid type number: %d. Must be  0-disable, 1-enable\n", type);
      return;
   }

   test_config.test_iw_port_stats_display = type;
   printf("-- The display of IW port statistics is %s --\n", (type == 0)?"disabled":"enabled");
}

static void CLI_F_ConfigurePsu(WP_CHAR *StrPrm)
{
   WP_U32 res, cmd, type;

   res = sscanf(StrPrm, "%d %d", &cmd, &type);
   if(res != 2)
   {
      printf("Invalid command. Usage: <cmd> <type: 0-Unshaped, 1-Shaped PPR, 2 - Shaped CIR, 3 - Shaped CIR+EIR\n");
      return;
   }

   if (type > 3)
   {
      printf("Invalid type number: %d. Must be  0-Unshaped, 1-Shaped PPR, 2 - Shaped CIR, 3 - Shaped CIR+EIR\n", type);
      return;
   }

   if (drivers_is_up && test_config.test_pkt_scheduler_used == 0)
   {
      printf("This command can be called at the beginning (before WP_SysCommit) in order to configure the PSU.\n");
      return;
   }
   if (type == 0)
   {
      test_config.test_pos_flows = 1;
      test_config.test_psu_shaping_type = TEST_PKT_UNSHAPED;
      printf("-- The PACKET Scheduler (PSU) not used. Packet flows are unshaped --\n");
   }
   else
   {
      test_config.test_pkt_scheduler_used = 1;
      test_config.test_pos_flows = TEST_MAX_POS_FLOWS;
      test_config.test_psu_shaping_type = type;
      printf("-- The PACKET Scheduler (PSU) usage enabled and packet shaping is set to %s --\n",
             (type == 1)?"PPR":(type==2)?"CIR":"CIR+EIR");
   }
}

static WP_boolean alarms_are_enabled = WP_FALSE;

static void CLI_F_EnableAlarms(WP_CHAR *strPrm)
{
   WP_U32 res, cmd;

   res = sscanf(strPrm, "%d", &cmd);
   if (res != 1)
   {
      printf("Invalid command. Usage: <cmd> \n");
      return;
   }

   WTI_enable_alarms(0);  /* 0 ==> SDH */
   alarms_are_enabled = WP_TRUE;
}

static void CLI_F_EnablePerformanceMonitoring(WP_CHAR *strPrm)
{
   WP_U32 res, cmd;

   res = sscanf(strPrm, "%d", &cmd);
   if (res != 1)
   {
      printf("Invalid command. Usage: <cmd> \n");
      return;
   }

   WTI_enable_performance_monitoring();  /* Register the callbacks */
   WTI_enable_performance_monitoring_points(0, line_sdh_cfg.framed);
}


static void CLI_F_DisplayAlarmMap(WP_CHAR *strPrm)
{
   WP_U32 res, cmd;
   void cb_port_alarm_report_only(WP_U8 device, WP_U8 line_port_id, WP_U8 alarm_category, WP_U8 is_asserted);
   void cb_path_alarm_report_only(WP_U8 device_id,
   WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *p_SDH_Sonet_line_endpoint_type,
   WP_U8 alarm_category, WP_U8 is_asserted);
   void cb_pdh_alarm_report_only(WP_U8 device_id, WP_U32 iSocketClientPDH, U8 alarm_category, U8 is_asserted);
   WP_U32 j;

   res = sscanf(strPrm, "%d", &cmd);
   if (res != 1)
   {
      printf("Invalid command. Usage: <cmd> \n");
      return;
   }

   if (!alarms_are_enabled)
      printf("ALARMS ARE NOT ENABLED\n");
   else
   {
      WPX_FRMR_RegisterSonetSdhPortAlarmCallback(&cb_port_alarm_report_only);
      WPX_FRMR_RegisterSonetSdhPathAlarmCallback(&cb_path_alarm_report_only);
      WPX_FRMR_RegisterSocketClientPdhAlarmCallback(&cb_pdh_alarm_report_only);

      for (j = 0; j < WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES; ++j)
         WPX_FRMR_ReportDeviceAlarmMap(j);
   }
}

void TEST_RunCheckin (void)
{
   test_config.test_use_framer = 1;

   CLI_F_RunAtmHostTest("0");
   if (!test_failures)
   {
      CLI_F_RunPosHostTest("0");
   }
   if (!test_failures)
   {
      CLI_F_RunHsposHostTest("0");
   }
   if (test_failures)
      printf("Test Failed.\n");
   else
      printf("Test Passed.\n");

   TEST_DriversRelease(0);
   WT_Reboot();
}

/***************************************************************
 * Func name  : CLI_T_Main
 * Description: "main" for the Menuing System
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_T_Main(void)
{
   WP_status status;
#ifndef TEST_CHECKIN
   WP_CHAR InputBuf[256];
#endif
   
   memset(&the_test, 0, sizeof(TEST_system));

   status = WP_DriverInit();
   TEST_TerminateOnError(status, "WP_DriverInit()");

#ifdef TEST_CHECKIN
   TEST_RunCheckin();
#else
   F_DisplayMenu();
   while (TRUE)
   {
      printf("%s", STR_ENTER_COMMAND);
#ifndef __linux__
      flush();
#endif
      gets(InputBuf);
      printf("\n");
      F_MenuEngine(InputBuf);
   }
#endif
} /* CLI_T_Main */

/***************************************************************
 * Func name  : CLI_F_Quit
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_Quit()
{
   TEST_DriversRelease(0);
   WT_Reboot();
   exit(1);
} /* CLI_F_Quit */

/***************************************************************
 * Func name  : CLI_F_QuitNoReset
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_QuitNoReset()
{
   exit(1);
} /* CLI_F_Quit */


WP_S32 main(WP_S32 argc, WP_CHAR **argv)
{
   if (TEST_DUAL_EMPHY)
      test_config.test_dual_emphy = 1;

#if _WT_MODULE_TEST_ENABLE_

   WT_Start(__FILE__, NULL, __FILE__);

#endif

   CLI_T_Main();
   /* For socket based simulators */
   return 0;
}

static void CLI_F_RunAtmIwTest_8xOC3(WP_CHAR *StrPrm)
{
   WP_U32 res, cmd;

   if ((test_config.test_ufe4_type != TEST_UFE448) && !TEST_DUAL_EMPHY)
   {
      printf("This command is not allowed in the current configuration.\n");
      return;
   }

   res = sscanf(StrPrm, "%d", &cmd);
   if(res != 1)
   {
      printf("Invalid command.\n");
      return;
   }

   TEST_DeleteOldConfiguration();

   test_host_traffic = 0;
   test_config.test_use_atm = 1;
   test_config.test_use_pos = 0;
   test_config.test_use_hspos = 0;
   if (test_config.test_layer1_type == TEST_SDH)
      test_config.test_line_type = TEST_SDH_STM1;
   else
      test_config.test_line_type = TEST_SONET_OC3;
   test_config.test_dual_emphy = 1;

   printf("------ Run ATM 8xOC3 scenario ---------\n");
   test_use_line = 0;
   TEST_RunAtmScenario(8, 5);
}

static void CLI_F_RunPosIwTest_8xOC3(WP_CHAR *StrPrm)
{
   WP_U32 res, cmd;
   
   if (test_config.test_ufe4_type != TEST_UFE448 && !TEST_DUAL_EMPHY)
   {
      printf("This command is not allowed in the current configuration.\n");
      return;
   }

   res = sscanf(StrPrm, "%d", &cmd);
   if(res != 1)
   {
      printf("Invalid command.\n");
      return;
   }

   TEST_DeleteOldConfiguration();

   test_host_traffic = 0;
   test_config.test_use_pos = 1;
   test_config.test_use_atm = 0;
   test_config.test_use_hspos = 0;
   if (test_config.test_layer1_type == TEST_SDH)
      test_config.test_line_type = TEST_SDH_STM1;
   else
      test_config.test_line_type = TEST_SONET_OC3;
   test_config.test_dual_emphy = 1;

   printf("------ Run POS 8xOC3 scenario ---------\n");
   test_use_line = 0;
   TEST_RunPosScenario(8, test_config.test_pos_flows);
}

static void CLI_F_RunAtmIwTest_2xOC12(WP_CHAR *StrPrm)
{
   WP_U32 res, cmd;
   
   if (!TEST_DUAL_EMPHY)
   {
      printf("This command is not allowed in the current configuration.\n");
      return;
   }

   res = sscanf(StrPrm, "%d", &cmd);
   if (res != 1)
   {
      printf("Invalid command. Usage: <cmd>\n");
      return;
   }

   TEST_DeleteOldConfiguration();

   test_host_traffic = 0;
   test_config.test_use_atm = 1;
   test_config.test_use_pos = 0;
   test_config.test_use_hspos = 0;
   if (test_config.test_layer1_type == TEST_SDH)
      test_config.test_line_type = TEST_SDH_STM4;
   else
      test_config.test_line_type = TEST_SONET_OC12;
   test_config.test_dual_emphy = 1;

   printf("------ Run ATM 2xOC12 scenario ---------\n");

   test_use_line = 0;
   TEST_RunAtmScenario(1 + 2 * TEST_UFE_MAX_LINE_PORTS, 10);
}


static void CLI_F_RunPosIwTest_2xOC12(WP_CHAR *StrPrm)
{
   WP_U32 res, cmd;
   
   if (!TEST_DUAL_EMPHY)
   {
      printf("This command is not allowed in the current configuration.\n");
      return;
   }

   res = sscanf(StrPrm, "%d", &cmd);
   if(res != 1)
   {
      printf("Invalid command. Usage: <cmd>\n");
      return;
   }

   TEST_DeleteOldConfiguration();

   test_host_traffic = 0;
   test_config.test_use_pos = 1;
   test_config.test_use_atm = 0;
   test_config.test_use_hspos = 0;
   if (test_config.test_layer1_type == TEST_SDH)
      test_config.test_line_type = TEST_SDH_STM4;
   else
      test_config.test_line_type = TEST_SONET_OC12;
   test_config.test_dual_emphy = 1;

   printf("------ Run POS 2xOC12 scenario ---------\n");
   test_use_line = 0;
   TEST_RunPosScenario(1 + 2 * TEST_UFE_MAX_LINE_PORTS, test_config.test_pos_flows);
}

static void CLI_F_RunHsposIwTest_2xOC12(WP_CHAR *StrPrm)
{
   WP_U32 res, cmd;

   if (!TEST_DUAL_EMPHY)
   {
      printf("This command is not allowed in the current configuration.\n");
      return;
   }

   res = sscanf(StrPrm, "%d", &cmd);
   if(res != 1)
   {
      printf("Invalid command. Usage: <cmd>\n");
      return;
   }

   TEST_DeleteOldConfiguration();

   test_host_traffic = 0;
   test_config.test_use_pos = 0;
   test_config.test_use_atm = 0;
   test_config.test_use_hspos = 1;
   test_config.test_hspos_use_iwgp = 1;

   if (test_config.test_layer1_type == TEST_SDH)
      test_config.test_line_type = TEST_SDH_STM4;
   else
      test_config.test_line_type = TEST_SONET_OC12;
   test_config.test_dual_emphy = 1;

   printf("------ Run HSPOS 2xOC12 scenario ---------\n");
   test_use_line = 0;
   TEST_RunPosScenario(1 + 2 * TEST_UFE_MAX_LINE_PORTS, test_config.test_pos_flows);
}

/*TEST_system the_test;  is defined in the 38621_setup.h */
#include "38621_setup.h"
#include "38621_statistics.h"
#include "38621_upi_normal.h"


/*****************************************************************************
 * NOTE:
 * This is where the shared routines gets included.  Ideally, this would
 * be done as a separately linked module.
 ****************************************************************************/
#include "wt_util.c"
#include "wt_partition_ecc_util.c"
