/*****************************************************************************
 * (C) Copyright Wintegra 2001.  All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: 103011-swap_g999_to_enet.c
 *
 * Purpose: Test the G999 to Enet swap feature
 *
 * This test can be activated in two modes:
 * SWAP_TO_ENET_DEVICE = 0 --> Activates the system in G999 mode
 * SWAP_TO_ENET_DEVICE = 1 --> Activates the system in Swapped Enet mode (103012-swap_g999_to_enet_2.c)
 *
 * Before the WP_SysCommit() command, both devices are created:
 * First G999 device, and afterwards the Enet device is created OVER the same port.
 *
 * The choice of which device to activate should be done after WP_SysCommit(),
 * using the WP_DeviceEnable(), as demonstrated in this test.
 *
 * To start this test, go to main() function.
 *
 * Note: There are some functions and variables which are not in use for this test,
 *       but only for debugging purposes.
 *
 *   TEST CONFIGURATION:
 *   ------------------
 * (*)In G999 mode:
 *    ------------
 *     send and receive 8 frames (200 bytes long), one frame on each stream:
 *
 *     ENET streams:
 *     -----------
 *      iw_tx_ch[ii] --> flow_agg_compatible_stream[ii] --> aal5_tx_ch_last_stream ->L2 -> L1 ->ENET stream ->G999 device -->
 *      loopback --> aal5_rx_ch_last_stream --> F.A host --> Iw_rx_ch
 *
 *     ATM stream:
 *     -----------
 *      iw_tx_ch[ii] --> flow_agg_compatible_stream[ii] --> tx_ch_stream[ii] ->L2 -> L1 ->ATM stream ->G999 device -->
 *      loopback --> rx_ch_stream[ii] --> F.A host --> Iw_rx_ch
 *
 *
 * (*)In ENET mode:
 *    ------------
 *    send and receive 10 frames (200 bytes long).
 *
 ****************************************************************************/

enum
{
   WT_SWAP_MODE_G999 = 0,
   WT_SWAP_MODE_ENET,

} WT_SWAP_MODE;

enum
{
   WT_WINNET_GROUP_A = 0,
   WT_WINNET_GROUP_B,
   WT_DUMMY_PORT_1,
   WT_DUMMY_PORT_2,
   WT_MAX_NUM_OF_GROUPS
} WT_WINNET_GROUP;

/****************************/
/* Prints debug information */
/****************************/
#define DEFAULT_WPID 0
#define DEBUG_PRINT 0
#define DEBUG_PRINT_WDDI_FUNCTION_CALL 0

#define DEBUG_SMAP 0

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "wp_wddi.h"
#include "wp_host_io.h"

#include "wp_sim.h"
#include "app_convert.h"
#include "wt_util.h"
#include "wt_partition_ecc_util.h"


#include "wpx_enet_phy.h"

#if DEBUG_PRINT
   #define MY_PRINTF(x, ...) printf(x, ##__VA_ARGS__)
#else
   #define MY_PRINTF(x, ...)
#endif

#define TESTSTRING(x) TESTSTRING_(x)
#define TESTSTRING_(x) #x


#ifdef USE_OC12_PHY
#include "wpx_oc12_pmc5357.h"
#else
#include "wpx_oc3_pmc5351.h"
#endif

#include "wpx_app_data.h"
#include "wpx_board_if.h"

extern WP_U32 dps_ProgramImage[];
extern WP_U16 dps_PC_Table_Init[];
// extern WP_pce_parser_info pce_parser_info;
WP_pce_parser_info pce_parser_info;

#define POOL_LIMIT           10

#define ETH_RX_TAG_START           0x1100
#define ETH_TX_TAG_START           0x1200
#define ETH_TX_SWAP_TAG            0x1280
#define ETH_RX_PHYSICAL_DEV_CH_TAG 0x1300
#define ETH_RX_PHYSICAL_DEV_SWAP_CH_TAG 0x1350
#define IW_RX_CH_TAG               0x1400
#define IW_TX_CH_TAG_START         0x1450

#define FLOW_CONTROL_FILTERS_TAG_START 0x1500
#define FLOW_CONTROL_RULES_TAG_START   0x1600
#define DEVICE_FILTER_TAG              0x1700
#define DEVICE_RULES_TAG_START         0x1800
#define FILTER_SET_FLOW_CONTROL_TAG    0x1900
#define FILTER_SET_DEVICE_TAG          0x2000
#define RX_CHANNEL_FILTER_SET_TAG      0x2100
#define TX_CHANNEL_FILTER_SET_TAG      0x2101
#define RX_CHANNEL_FILTER_TAG          0x2200
#define TX_CHANNEL_FILTER_TAG          0x2201
#define TX_CHANNEL_RULES_HOST_TAG_START              0x2202
#define RX_CHANNEL_RULES_HOST_TAG_START              0x2300
#define RX_CHANNEL_RULES_HOST_MC_TAG_START           0x2350
#define RX_CHANNEL_RULES_STREAM_COMPATIBLE_TAG_START 0x2400
#define RX_CHANNEL_RULES_STREAM_GENERIC_TAG_START    0x2500
#define RX_CHANNEL_RULES_ENET_COMPATIBLE_TAG_START   0x2600
#define RX_CHANNEL_RULES_ENET_GENERIC_TAG_START      0x2700
#define BRIDGE_PORT_TAG_START                        0x2800
#define BRIDGE_PORT_HOST_TAG                         0x2900
#define BRIDGE_PORT_HOST_MC_TAG                      0x2950
#define IW_TX_CHANNEL_RULES_TAG_START                0x2a00
#define IW_FLOW_AGG_TAG_HOST                         0x2b00
#define IW_FLOW_AGG_TAG_COMPATIBLE                   0x2b20
#define IW_FLOW_AGG_TAG_GENERIC                      0x2b50
#define IW_FLOW_AGG_TAG_MULTICAST                    0x2b70
#define ATM_TX_TAG1                                  0x3101
#define ATM_RX_TAG1                                  0x3111

#define MAX_DATA_LENGTH      (2048)
#define NUM_OF_FRAMES 10
WP_U32 sent_frame_length[NUM_OF_FRAMES] = {30,   64,  128, 250,
                                           500, 750, 1000, 1250,
                                           1500, 1500};
   
#define MAX_G999_STREAMS      48
#define MIN_CH_RATE          5000
#define MAX_CH_RATE          2048000

#define VPI_INIT 20
#define VCI_INIT 40
#define AAL5_VCMUX_PRFIX_LENGTH 2

#define APP_HOST_BUFFER_SIZE 256

#define IW_SYS_MAX_FLOWS     8
#define LEARNING_QUEUE_SIZE  10
#define IW_SYS_MAX_BPORTS    16

#define MAX_PACKET_SIZE      1984
#define IW_BUF_N_BUFFERS     20
#define QN_IW_N_BUFFERS      50

#define WT_RECEIVE_DELAY     1000
#define ONE_SEC             1000000
#define WT_WIMSIM_DELAY_SEC (2*ONE_SEC)
#define WT_RECEIVE_DELAY_WINSIM  (100*ONE_SEC)

/*PCE Flow control*/
#define NUM_OF_PCE_FILTERS (MAX_G999_STREAMS)
#define NUM_OF_PCE_RULES  ((MAX_G999_STREAMS * 2))

#define WT_DEFAULT_NUM_OF_STREAMS 24
WP_CHAR appname[] = "WP3 basic_g999";
WP_boolean wddi_trace_debug = WP_FALSE;

WP_int_queue_table int_queue_table_cfg[1];
WP_atm_global atm_global_cfg[1];
WP_iw_global  iw_global_cfg[1];
WP_context context_cfg[1];

WP_pool_buffer_data pool_buffer_data_768_cfg[1];
WP_pool_buffer_data pool_buffer_data_2048_cfg[1];
WP_qnode_iwq qnode_iwq_cfg[1];
WP_qnode_iwq qnode_iw_host_cfg[1];

WP_port_enet port_enet_cfg[1];
WP_device_enet_g999 device_enet_g999_cfg[1];
WP_device_enet_g999_ex device_enet_g999_ex_cfg[1];
WP_device_enet_g999_atm device_enet_g999_atm[1];
WP_device_enet_ex device_enet_ex_cfg[1];
WP_device_enet device_enet_cfg[1];
WP_g999_stream g999_stream_cfg[MAX_G999_STREAMS];
WP_rx_binding_bridging rx_binding_bridging_cfg[1];

WP_module_pce_init module_pce_init_cfg[1];
WP_ch_enet ch_enet_cfg[1];
WP_tx_binding  tx_binding_cfg[1];
WP_ch_iw_rx    ch_iw_rx_cfg[1];
WP_ch_iw_tx    ch_iw_tx_cfg[1];
WP_iw_agg_bridging tx_agg_bridging[1];
WP_iw_agg_hq iw_agg_hq_cfg[1];
WP_iw_agg_generic iw_agg_generic_cfg[1];
/* Multicast */
WP_iw_mc_member iw_mc_member_cfg[1];
WP_iw_mc_group iw_mc_group_cfg[1];
WP_iw_agg_multicast iw_agg_multicast_cfg[1];
WP_qnode_iw_mcq qnode_iw_mcq_cfg[1];

WP_U8 mac_address_arr[6] = {0xAA,0xBB,0xCC,0xDD,0xEE,0x00};

WP_U8 vcmux_prefix_header[32] = {0,0,0,0,0,0,0,
                                 0,0,0,0,0,0,0,
                                 0,0,0,0,0,0,0,
                                 0,0,0,0,0,0,0};

WP_U8 prefix_header[32] = {0,0,0,0,0,0,0,
                           0,0,0,0,0,0,0,
                           0,0,0,0,0,0,0,
                           0,0,0,0,0,0,0};
WP_bridge_port bridge_port_cfg[1];
WP_iw_sys_bridging iw_sys_bridging_cfg[1];
WP_iw_ipv4_sys_info ipv4_sys_info_cfg[1];
WP_dfc_sys_info dfc_sys_info_cfg[1];
WP_fmu_shaping_cir_eir l1_group_shaping_params[1];
WP_fmu_shaping_cir_eir l2_group_shaping_params[1];
WP_shaping_group_type_packet shaping_group_type_packet_l1_cfg[1];
WP_shaping_group_type_packet shaping_group_type_packet_l2_cfg[1];
WP_data_unit generator_data_unit[1];
WP_data_unit data_unit[1];
WP_data_segment km_tx_buf[1];

WP_handle g999_stream_handle[MAX_G999_STREAMS];
WP_handle shaping_group_l1[MAX_G999_STREAMS], shaping_group_l2[MAX_G999_STREAMS];
WP_handle shaping_group_swap_l1[WT_MAX_NUM_OF_GROUPS], shaping_group_swap_l2[WT_MAX_NUM_OF_GROUPS];
WP_handle pce_device_filter, pce_device_rules[MAX_G999_STREAMS], device_filterset_handle, device_pce_if_handle;
WP_handle pce_rx_channel_filter, rx_channel_filterset_handle;

/* Swapped Enet PCE handles*/
WP_handle pce_rx_channel_rules_enet_to_host[WT_MAX_NUM_OF_GROUPS];
WP_handle pce_rx_channel_rules_swapped_enet_to_enet[2];
WP_handle pce_rx_channel_rules_enet_to_swapped_enet[WT_MAX_NUM_OF_GROUPS][2];
WP_handle rx_channel_enet_pce_if_handle;

WP_handle pce_rx_channel_rules_to_host[MAX_G999_STREAMS];
WP_handle pce_rx_channel_rules_to_enet_from_generic[MAX_G999_STREAMS];
WP_handle pce_rx_channel_rules_to_enet_from_compatible[MAX_G999_STREAMS];
WP_handle pce_rx_channel_rules_to_stream_from_generic[MAX_G999_STREAMS];
WP_handle rx_channel_stream_pce_if_handle[MAX_G999_STREAMS];
WP_handle iw_tx_channel_pce_if_handle;
WP_handle pce_iw_tx_channel_filter, iw_tx_channel_filterset_handle;
WP_handle pce_iw_tx_channel_rule_to_host;

WP_handle iw_pool1, iw_pool_host;
WP_handle qniw_enet, qniw_host;

/* ATM handles*/
WP_handle pool_1536, pool_ring_host, qniw_aal, iw_pool_aal5, qniw_aal5, qnode_host;

/* ATM pools data structures */
WP_qnode_iwq qnode_iwq_aal5_cfg[1];
WP_pool_ring_data pool_ring_data_cfg[1];
WP_pool_buffer_data pool_buffer_data_cfg_1536[1];
WP_qnode_hostq qnode_hostq_cfg[1];
WP_ch_aal5 ch_aal5_cfg[1];

/* Port handles */
WP_handle enetp1, enetp2, host_port;

/* Device handles */
WP_handle group_a_g999_dev, group_a_enet_dev, group_b_g999_dev, group_b_enet_dev, host_dev;
WP_handle dummy_port1, dummy_device1;
WP_handle dummy_port2, dummy_device2;

/* Rx and Tx channel handles */
WP_handle rx_channel_g999_dev[WT_MAX_NUM_OF_GROUPS], rx_channel_stream[MAX_G999_STREAMS];
WP_handle tx_channel_stream[MAX_G999_STREAMS];

WP_handle tx_channel_enet_dev[WT_MAX_NUM_OF_GROUPS], rx_channel_enet_dev[WT_MAX_NUM_OF_GROUPS];

WP_handle iw_rx_ch, iw_tx_ch[MAX_G999_STREAMS], iw_tx_ch_enet[WT_MAX_NUM_OF_GROUPS];
WP_handle iw_bridging_system_handle;

/**************************/
/* IW bridge port handles */
/**************************/
WP_handle bport_host, bport_iw_enet[WT_MAX_NUM_OF_GROUPS];
/* G999 system */
WP_handle bport_iw_stream[MAX_G999_STREAMS], bport_host_tx[MAX_G999_STREAMS];
/* Swapped enet system */
WP_handle bport_iw_swapped_enet[WT_MAX_NUM_OF_GROUPS], bport_host_tx_swapped_enet[WT_MAX_NUM_OF_GROUPS];

/****************************/
/* Flow aggregation handles */
/****************************/
WP_handle flow_agg_host[WT_MAX_NUM_OF_GROUPS];
/* G999 system */
WP_handle flow_agg_compatible_stream[MAX_G999_STREAMS];
/* Swapped enet system */
WP_handle flow_agg_compatible_enet[WT_MAX_NUM_OF_GROUPS];
WP_U32 max_g999_streams_in_use[WT_MAX_NUM_OF_GROUPS];

WP_boolean scenario_created = WP_FALSE;
WP_U16 wpe_test_failures = 0;
typedef union StatField
{
      WP_U64 field;
      struct {
            WP_U32 high;
            WP_U32 low;
      } part;
} WPT_StatField;

#define WTE_PRINT_STAT_HIGH_LOW(printf_string, field) \
   printf (printf_string, ((WPT_StatField*)&field)->part.high, ((WPT_StatField*)&field)->part.low)

#define WTE_PRINT_STAT_IF_NOT_ZERO(printf_string, field) \
   do                                                    \
   {                                                     \
      if (field != 0)                                    \
         WTE_PRINT_STAT_HIGH_LOW(printf_string, field);  \
   }while(0)

#define WTE_PRINT_STAT_IF_NOT_ZERO_WITH_INDEX(printf_string, index, field) \
   do                                                                   \
   {                                                                    \
      if (field != 0)                                                   \
         printf (printf_string, index, ((WPT_StatField*)&field)->part.high, ((WPT_StatField*)&field)->part.low); \
   }while(0)

void WTE_DefaultConfigs(void);
void WTE_init(void);
void WTE_Create_pools(void);
void WTE_Create_ports(void);
void WTE_CreateStreamsGroupA(void);
void WTE_CreateStreamsGroupB(void);
void WTE_CreatePceRulesDeviceGroupA(void);
void WTE_CreatePceRulesDeviceGroupB(void);
void WTE_CreatePceRules(WP_U8 swap_modem,
                        WP_S32 ii,
                        WP_U8 group_num);
void WTE_GroupsCreateGroupA(WP_U8 swap_mode);
void WTE_CreateChannelsGroupA(WP_U8 swap_mode);
void WTE_CreateHostFlowAggregationsAndIwPorts(void);
void WTE_CreateEnetRxChannelPceFiltersFilterSetsIntrefaces(void);
void WTE_CreateG999DevicePceFiltersFilterSetsIntrefaces(void);
void WTE_CreateIwSystemAndItsComponentsGroupA(WP_U8 swap_mode);
void WTE_CreateIwSystemAndItsComponentsGroupB(WP_U8 swap_mode);
void WTE_EnablePortsDevicesChannelsGroupA(WP_U8 swap_mode);
void WTE_EnablePortsDevicesChannelsGroupB(WP_U8 swap_mode);
void WTE_SystemDeleteGroupA(WP_U8 swap_mode);
void WTE_SystemDeleteGroupB(WP_U8 swap_mode);
void WTE_CreateDummyFlowsPort1(void);
void WTE_CreateDummyFlowsPort2(void);
void WTE_ReceiveOnG999DevChannel(WP_handle rx_channel_g999);
void WTE_ReceiveOnG999StreamChannel(void);
WP_boolean WTE_ReceiveOnIwHostChannel(void);
WP_boolean WTE_CheckFrame(WP_status status);
WP_boolean WTE_Test_System(WP_U8 swap_mode,
                           WP_U32 start_index,
                           WP_S32 frame_index,
                           WP_U8 group_num);
void WTE_HostSendPauseFrame(void);
void WTE_HostSendDirect(void);
void WTE_WinutilCommand(void);
void WTE_WinutilSmap(void);

void WTE_ResetStats(void);
void WTE_ResetIwPortStats(WP_handle iw_port_handle, WP_boolean router_port);
void WTE_ResetFlowStats(WP_handle agg);
void WTE_ResetGenericFlowStats(WP_handle agg);
void WTE_ResetG999StreamStats(WP_handle stream_handle);
void WTE_ResetEnetDeviceStats(WP_handle device_handle);
void WTE_ResetPCEStats(WP_handle filter_handle);
void WTE_ResetPCEGlobalStats(void);
void WTE_ResetStatisticsGroupA(WP_U8 swap_mode);
void WTE_ResetStatisticsGroupB(WP_U8 swap_mode);
void WTE_ResetStatisticsDummyPort1(void);
void WTE_ResetStatisticsDummyPort2(void);

void WTE_Terminate(WP_U32 status);
void WTE_DisplayIwPortStats(WP_handle iw_port_handle, WP_boolean router_port, WP_CHAR* type);
void WTE_DisplayFlowStats(WP_handle agg, WP_CHAR *type);
void WTE_DisplayMultiClassDeviceStats(WP_handle device_handle, WP_CHAR* type);
void WTE_DisplayEnetDeviceStats(WP_handle device_handle, WP_CHAR* type);
void WTE_DisplayFMU(void);
void WTE_DisplayStats(void);
void WTE_DisplayQnodeStatus(void);
void WTE_DisplayPceFilters(void);
void WTE_DisplayPceRules(void);
void WTE_DisplayPceFilterSets(void);
void WTE_DisplayPCEStats(WP_handle filter_handle,
                         WP_CHAR* type);
void WTE_DisplayPCEGlobalStats(void);
void WTE_DisplayG999StreamStats(WP_handle stream_handle, WP_CHAR* type);
void WTE_DisplayDevices(void);
void WTE_DisplayStreams(void);
void WTE_PceDataTest(void);
void WTI_WddiAttachCallback(const WP_CHAR *file, const WP_CHAR *function, WP_S32 line);
WP_status WTI_LogCallBack(WP_S32 level, const WP_CHAR *message);
void terminate_on_error(WP_handle handle, WP_CHAR *s,WP_U32 line);

void WTE_SwapGroupAToEnetMode(void);
void WTE_SwapGroupAToG999Mode(WP_U32 max_g999_streams);
void WTE_CreateGroupAG999Mode(void);
void WTE_CreateGroupAEnetMode(void);
void WTE_DeleteGroupAG999Mode(void);
void WTE_DeleteGroupAEnetMode(void);

void WTE_SwapGroupBToEnetMode(void);
void WTE_SwapGroupBToG999Mode(WP_U32 max_g999_streams);
void WTE_CreateGroupBG999Mode(void);
void WTE_CreateGroupBEnetMode(void);
void WTE_DeleteGroupBG999Mode(void);
void WTE_DeleteGroupBEnetMode(void);

WP_boolean WTE_Scenario_1(void);
WP_boolean WTE_Scenario_2(void);
WP_boolean WTE_Scenario_3(void);
WP_boolean WTE_Scenario_4(void);
WP_boolean WTE_Scenario_5(void);

#ifdef BSP
WP_S32 Main(WP_S32 argc, WP_CHAR **argv)
#else
WP_S32 main(WP_S32 argc, WP_CHAR **argv)
#endif
{
   WP_status status;
   WP_boolean scenario_passed = WP_TRUE;
   WP_boolean test_passed = WP_TRUE;

#if _WT_MODULE_TEST_ENABLE_
   WT_Start(TESTSTRING(WDDI_TEST), NULL, __FILE__);
#endif
   WTE_DefaultConfigs();

   WTE_init();

   max_g999_streams_in_use[WT_WINNET_GROUP_A] = WT_DEFAULT_NUM_OF_STREAMS;
   max_g999_streams_in_use[WT_WINNET_GROUP_B] = WT_DEFAULT_NUM_OF_STREAMS;
   max_g999_streams_in_use[WT_DUMMY_PORT_1] = 1; /* used for host send */
   max_g999_streams_in_use[WT_DUMMY_PORT_2] = 1; /* used for host send */
   WTE_Create_pools();
   WTE_Create_ports();

   status = WP_SysCommit();
   terminate_on_error(status, "WP_SysCommit()", __LINE__);

   WT_Identify();

   WTE_CreateHostFlowAggregationsAndIwPorts();
   WTE_CreateEnetRxChannelPceFiltersFilterSetsIntrefaces();
   WTE_CreateG999DevicePceFiltersFilterSetsIntrefaces();
   WTE_CreateDummyFlowsPort1();
   WTE_CreateDummyFlowsPort2();
printf ("step1\n");
   /* Group A in Enet mode, Group B in Enet mode */
   scenario_passed = WTE_Scenario_4();
   if (! scenario_passed ) test_passed = WP_FALSE;

   /* Group A and Group B in G999 mode */
   scenario_passed = WTE_Scenario_2();
   if (! scenario_passed ) test_passed = WP_FALSE;

printf ("step2\n");
   /* Group A in G999 mode, Group B in Enet mode */
   scenario_passed = WTE_Scenario_1();
   if (! scenario_passed ) test_passed = WP_FALSE;

printf ("step 3\n");
   /* Group A and Group B in G999 mode */
   scenario_passed = WTE_Scenario_2();
   if (! scenario_passed ) test_passed = WP_FALSE;

printf ("step 4\n");
   /* Group A in Enet mode, Group B in G999 mode */
   scenario_passed = WTE_Scenario_3();
   if (! scenario_passed ) test_passed = WP_FALSE;

printf ("step 5\n");
   printf ("QNodes initial state\n");
   WTE_DisplayQnodeStatus ();

printf ("step 6\n");
   if (! test_passed)
      printf ("Test Failed\n");
   else
      printf ("Test Passed\n");

   do {
      WPL_Delay (10000);
      printf ("Press q to quit\n");
   } while ('q' != getchar ()); 
   WTE_Terminate(0);

  WTE_Terminate(0);

/***************************************************************/
   return 0;
} /* main */

void WTE_DefaultConfigs(void)
{
   WP_bus_bank vb_param ,vb_packet ,vb_internal ,vb_host ,vb_none;
   WP_U8 enet_encap_header[12] = {0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc,
                                  0xbc, 0x9a, 0x78, 0x56, 0x34, 0x12};
   WP_U8 *mac_addr = mac_address_arr;
   WP_S32 ii;

   memset (int_queue_table_cfg, 0, sizeof (WP_int_queue_table));
   int_queue_table_cfg[0].queue[0].size = 10;
   int_queue_table_cfg[0].queue[0].rate = 1;
   int_queue_table_cfg[0].queue[1].size = 10;
   int_queue_table_cfg[0].queue[1].rate = 1;
   int_queue_table_cfg[0].queue[2].size = 10;
   int_queue_table_cfg[0].queue[2].rate = 1;
   int_queue_table_cfg[0].queue[3].size = 10;
   int_queue_table_cfg[0].queue[3].rate = 1;

   memset (atm_global_cfg, 0, sizeof (WP_atm_global));
   atm_global_cfg[0].qsr_bus = WP_BUS_NONE;
   atm_global_cfg[0].qsr_bank = WP_BUS_NONE;

   memset (iw_global_cfg, 0, sizeof (WP_iw_global));
   iw_global_cfg[0].max_iw_aggregations = 110;
   iw_global_cfg[0].max_routing_systems = 0;
   iw_global_cfg[0].max_directmap_enet_systems = 0;
   iw_global_cfg[0].max_bridging_systems = 1;
   iw_global_cfg[0].max_mpls_systems = 0;
   iw_global_cfg[0].max_vlan_priority_maps = 1;
   iw_global_cfg[0].iw_bkgnd_fifo_size = 0;
   iw_global_cfg[0].cong_pt = NULL;
   iw_global_cfg[0].iw_host_limits.max_rx_channels = 2;
   iw_global_cfg[0].iw_host_limits.pq_block_size = 0;
   iw_global_cfg[0].iw_host_limits.n_pq_blocks = 0;
   iw_global_cfg[0].iw_host_limits.max_tx_channels = MAX_G999_STREAMS*2;
   iw_global_cfg[0].iw_host_limits.max_tx_groups = 0;
   iw_global_cfg[0].mpls_config.n_llst_tables = 0;
   iw_global_cfg[0].mpls_config.llst_table_size = 0;

   iw_global_cfg[0].iw_modes.policer_mode = WP_IW_NO_POLICER;
   iw_global_cfg[0].iw_modes.statistics_bundle_stat_mode = WP_IW_STATISTICS_BUNDLE_STAT_DISABLE;
   iw_global_cfg[0].iw_modes.l2_prefix_ext_mode = WP_IW_L2_HEADER_EXTENSION_DISABLE;
   iw_global_cfg[0].iw_modes.enhanced_flow_stat_mode = WP_IW_ENHANCED_FLOW_STAT_DISABLE;
   iw_global_cfg[0].iw_modes.flow_stat_mode = WP_IW_FLOW_STAT_ENABLE;
   iw_global_cfg[0].iw_modes.fr_tx_flow_stat_mode = WP_IW_FR_TX_FLOW_STAT_DISABLE;
   iw_global_cfg[0].iw_modes.mfc_alloc_mode = WP_IW_MFC_ALLOC_DISABLE;
   iw_global_cfg[0].iw_modes.learning_queue_mode = WP_IW_LEARNING_DISABLE;
   iw_global_cfg[0].iw_modes.port_filtering_mode = WP_IW_PORT_FILTERING_DISABLE;
   iw_global_cfg[0].iw_modes.lpm_alloc_mode = WP_IW_LPM_ALLOC_DISABLE;
   iw_global_cfg[0].iw_modes.enhanced_mc_member_stat_mode = WP_IW_ENHANCED_MC_MEMBER_STAT_ENABLE;
   iw_global_cfg[0].iw_modes.routing_alloc_modes = WP_DISABLE;
   iw_global_cfg[0].iw_modes.res_mc_addresses_table_mode = WP_IW_BPORT_RES_MC_TABLE_ALLOC_DISABLE;
   iw_global_cfg[0].iw_modes.port_stag_etype_mode = WP_IW_PORT_STAG_ETYPE_DISABLE;
   iw_global_cfg[0].iw_modes.iw_characteristics = (WP_IW_MODES_DFC_INDIRECT_ACTION_DISABLE |
                                            WP_IW_MODES_FLOW_CLASS_RULE_STAT_DISABLE |
                                            WP_IW_MODES_BRIDGING_PREFIX_TIME_STAMP_DISABLE);

   iw_global_cfg[0].max_iw_stat_bundles = 0;
   iw_global_cfg[0].max_fr_sw_systems = 0;
   iw_global_cfg[0].max_iw_ports = MAX_G999_STREAMS*3 + 10;
   iw_global_cfg[0].max_iw_mc_groups = 0;
   iw_global_cfg[0].max_nat_systems = 0;
   iw_global_cfg[0].max_iw_second_aggregations = 0;
   iw_global_cfg[0].iw_comp_limits = 0;
   iw_global_cfg[0].max_iw_mc_members = 0;
   iw_global_cfg[0].max_iw_programmable_filters = 0;
   iw_global_cfg[0].max_iw_policers = 0;
   iw_global_cfg[0].max_dscpmt = 0;
   iw_global_cfg[0].max_wred_bindings = 0;
   iw_global_cfg[0].tcm_policer_limits.max_vlan_priority_tcm_policers = 0;
   iw_global_cfg[0].tcm_policer_limits.max_dscp_tcm_policers = 0;
   iw_global_cfg[0].max_flow_rule_stat_tables = 0;
   iw_global_cfg[0].max_indirect_action_tables = 0;
   iw_global_cfg[0].brigding_time_stamp_info.time_stamp_offset = 0;
   iw_global_cfg[0].brigding_time_stamp_info.time_stamp_resolution = 0;
   iw_global_cfg[0].iwgp_bkgnd_fifo_size = 0;

   memset (&vb_param, 0, sizeof (vb_param));
   vb_param.bus = WP_BUS_PARAM;
   vb_param.bank = APP_BANK_PARAM;
   memset (&vb_packet, 0, sizeof (vb_packet));
   vb_packet.bus = WP_BUS_PACKET;
   vb_packet.bank = APP_BANK_PACKET;
   memset (&vb_internal, 0, sizeof (vb_internal));
   vb_internal.bus = WP_BUS_INTERNAL;
   vb_internal.bank = APP_BANK_INTERNAL;
   memset (&vb_host, 0, sizeof (vb_host));
   vb_host.bus = WP_BUS_HOST;
   vb_host.bank = APP_BANK_HOST;
   memset (&vb_none, 0, sizeof (vb_none));
   vb_none.bus = WP_BUS_NONE;
   vb_none.bank = 0;

   memset (context_cfg, 0, sizeof (WP_context));
   context_cfg[0].max_qnodes = 3;
   for (ii=0 ; ii<WP_VIRTUAL_BUSES ; ii++)
      memcpy (&(context_cfg[0].virtual_bus[ii]), &vb_none, sizeof(WP_bus_bank));
   memcpy(&(context_cfg[0].virtual_bus[WP_VB_PARAM]),                    &vb_param, sizeof (WP_bus_bank));
   memcpy(&(context_cfg[0].virtual_bus[WP_VB_PACKET]),                   &vb_packet, sizeof (WP_bus_bank));
   memcpy(&(context_cfg[0].virtual_bus[WP_VB_INTERNAL]),                 &vb_internal, sizeof (WP_bus_bank));
   memcpy(&(context_cfg[0].virtual_bus[WP_VB_HOST]),                     &vb_host, sizeof (WP_bus_bank));
   memcpy(&(context_cfg[0].virtual_bus[WP_VB_INTQUEUE_DEBUG]),           &vb_host, sizeof (WP_bus_bank));
   memcpy(&(context_cfg[0].virtual_bus[WP_VB_TIMESTAMP]),                &vb_host, sizeof (WP_bus_bank));
   memcpy(&(context_cfg[0].virtual_bus[WP_VB_TPT]),                      &vb_internal, sizeof (WP_bus_bank));
   memcpy(&(context_cfg[0].virtual_bus[WP_VB_CW]),                       &vb_internal, sizeof (WP_bus_bank));
   memcpy(&(context_cfg[0].virtual_bus[WP_VB_ASU_TXQUEUE]),              &vb_param, sizeof (WP_bus_bank));
   memcpy(&(context_cfg[0].virtual_bus[WP_VB_STATISTICS]),               &vb_param, sizeof (WP_bus_bank));
   memcpy(&(context_cfg[0].virtual_bus[WP_VB_ATM_ADDLOOKUP]),            &vb_param, sizeof (WP_bus_bank));
   memcpy(&(context_cfg[0].virtual_bus[WP_VB_ATM_CPT_EBMT_EXTHEADER]),   &vb_param, sizeof (WP_bus_bank));
   memcpy(&(context_cfg[0].virtual_bus[WP_VB_ETH_HDLC_CPT]),             &vb_param, sizeof (WP_bus_bank));
   memcpy(&(context_cfg[0].virtual_bus[WP_VB_FBP_BD]),                   &vb_param, sizeof (WP_bus_bank));
   memcpy(&(context_cfg[0].virtual_bus[WP_VB_FBP_DATA]),                 &vb_host, sizeof (WP_bus_bank));
   memcpy(&(context_cfg[0].virtual_bus[WP_VB_L2_BKGND]),                 &vb_host, sizeof (WP_bus_bank));
   memcpy(&(context_cfg[0].virtual_bus[WP_VB_BRIDGE_LEARNING_DFC_PFMT]), &vb_param, sizeof (WP_bus_bank));
   memcpy(&(context_cfg[0].virtual_bus[WP_VB_IMA_MLPPP_STATE]),          &vb_internal, sizeof (WP_bus_bank));
   memcpy(&(context_cfg[0].virtual_bus[WP_VB_IMA_MLPPP_BUFFER]),         &vb_param, sizeof (WP_bus_bank));
   memcpy(&(context_cfg[0].virtual_bus[WP_VB_HASH_IW_LPM_EMC]),          &vb_host, sizeof (WP_bus_bank));
   memcpy(&(context_cfg[0].virtual_bus[WP_VB_IW_MFC_RULES]),             &vb_host, sizeof (WP_bus_bank));
   memcpy(&(context_cfg[0].virtual_bus[WP_VB_IW_FIWT_QAT]),              &vb_param, sizeof (WP_bus_bank));
   memcpy(&(context_cfg[0].virtual_bus[WP_VB_IW_MFCPT_DIFFSERV_UPPT_DFCT]), &vb_internal, sizeof (WP_bus_bank));
   memcpy(&(context_cfg[0].virtual_bus[WP_VB_STATISTICS2]),              &vb_host, sizeof (WP_bus_bank));
   memcpy(&(context_cfg[0].virtual_bus[WP_VB_STATISTICS3]),              &vb_host, sizeof (WP_bus_bank));
   memcpy(&(context_cfg[0].virtual_bus[WP_VB_PKT_GLOBAL_TCPT]),          &vb_internal, sizeof (WP_bus_bank));
   memcpy(&(context_cfg[0].virtual_bus[WP_VB_MCH_DATA]),                 &vb_host, sizeof (WP_bus_bank));
   memcpy(&(context_cfg[0].virtual_bus[WP_VB_MFC_INTERNAL]),             &vb_host, sizeof (WP_bus_bank));
   memcpy(&(context_cfg[0].virtual_bus[WP_VB_POLICER]),                  &vb_param, sizeof (WP_bus_bank));
   memcpy(&(context_cfg[0].virtual_bus[WP_VB_PCE_LRN]),                  &vb_internal, sizeof (WP_bus_bank));
   memcpy(&(context_cfg[0].virtual_bus[WP_VB_PCEA_PARAM]),               &vb_internal, sizeof (WP_bus_bank));
   memcpy(&(context_cfg[0].virtual_bus[WP_VB_STATS_IWPORT_PCE_FILTERS]), &vb_internal, sizeof (WP_bus_bank));
   memcpy(&(context_cfg[0].virtual_bus[WP_VB_POLICER_ACTION]),           &vb_param, sizeof (WP_bus_bank));
   memcpy(&(context_cfg[0].virtual_bus[WP_VB_G999_STREAM]),              &vb_internal, sizeof (WP_bus_bank));
   memcpy(&(context_cfg[0].virtual_bus[WP_VB_G999_STREAM_STATISTICS]),   &vb_internal, sizeof (WP_bus_bank));
   memcpy(&(context_cfg[0].virtual_bus[WP_VB_FMU_FIFO_TO_CHANNEL_NUMBER]), &vb_internal, sizeof (WP_bus_bank));
   memcpy(&(context_cfg[0].virtual_bus[WP_VB_IPWORT_EXT]), &vb_param, sizeof (WP_bus_bank));
   context_cfg[0].dps_image[0] = dps_ProgramImage;
   context_cfg[0].dps_image[1] = dps_ProgramImage;
   context_cfg[0].dps_features[0] = dps_PC_Table_Init;
   context_cfg[0].dps_features[1] = dps_PC_Table_Init;
   context_cfg[0].int_queue_data = int_queue_table_cfg;
   context_cfg[0].pool_limit = POOL_LIMIT;
   context_cfg[0].atm_data = atm_global_cfg;
   context_cfg[0].iw_data = iw_global_cfg;
   context_cfg[0].dps_winfarm_mask = 0;

   memset (pool_buffer_data_768_cfg, 0, sizeof (WP_pool_buffer_data));
   pool_buffer_data_768_cfg[0].n_buffers = 100;
   pool_buffer_data_768_cfg[0].offset = 0x80;
   pool_buffer_data_768_cfg[0].size = 512;
   pool_buffer_data_768_cfg[0].pad = 0;
#ifdef WP_BOARD_WDS3_SL
   pool_buffer_data_768_cfg[0].bus = WP_BUS_PARAM;
   pool_buffer_data_768_cfg[0].bank = APP_BANK_PARAM;
#else
   pool_buffer_data_768_cfg[0].bus = WP_BUS_PACKET;
   pool_buffer_data_768_cfg[0].bank = APP_BANK_PACKET;
#endif
   memset (pool_buffer_data_cfg_1536, 0, sizeof (WP_pool_buffer_data));
   pool_buffer_data_cfg_1536[0].n_buffers = 30;
   pool_buffer_data_cfg_1536[0].offset = 0x80;
   pool_buffer_data_cfg_1536[0].size = MAX_DATA_LENGTH;
   pool_buffer_data_cfg_1536[0].pad = 0;
#ifdef WP_BOARD_WDS3_SL
   pool_buffer_data_cfg_1536[0].bus = WP_BUS_PARAM;
   pool_buffer_data_cfg_1536[0].bank = APP_BANK_PARAM;
#else
   pool_buffer_data_cfg_1536[0].bus = WP_BUS_PACKET;
   pool_buffer_data_cfg_1536[0].bank = APP_BANK_PACKET;
#endif// WP_BOARD_WDS3_SL

   memset (pool_ring_data_cfg, 0, sizeof (WP_pool_ring_data));
   pool_ring_data_cfg[0].n_rings =6;
   pool_ring_data_cfg[0].ring_length =4;
#ifdef WP_BOARD_WDS3_SL
   pool_ring_data_cfg[0].bus = WP_BUS_PARAM;
   pool_ring_data_cfg[0].bank = APP_BANK_PARAM;
#else
   pool_ring_data_cfg[0].bus = WP_BUS_PACKET;
   pool_ring_data_cfg[0].bank = APP_BANK_PACKET;
#endif// WP_BOARD_WDS3_SL

   memset (pool_buffer_data_2048_cfg, 0, sizeof (WP_pool_buffer_data));
   pool_buffer_data_2048_cfg[0].n_buffers = 100;
   pool_buffer_data_2048_cfg[0].offset = 0x80;
   pool_buffer_data_2048_cfg[0].size = 2048;
   pool_buffer_data_2048_cfg[0].pad = 0;

#ifdef WP_BOARD_WDS3_SL
   pool_buffer_data_2048_cfg[0].bus = WP_BUS_PARAM;
   pool_buffer_data_2048_cfg[0].bank = APP_BANK_PARAM;
#else
   pool_buffer_data_2048_cfg[0].bus = WP_BUS_PACKET;
   pool_buffer_data_2048_cfg[0].bank = APP_BANK_PACKET;
#endif

   memset (qnode_hostq_cfg, 0, sizeof (WP_qnode_hostq));
   qnode_hostq_cfg[0].pool_buffer = 0;
   qnode_hostq_cfg[0].pool_ring = 0;
   qnode_hostq_cfg[0].interruptqueue = WP_IRQT0;

   memset(qnode_iwq_aal5_cfg, 0 , sizeof(WP_qnode_iwq));
   qnode_iwq_aal5_cfg[0].interruptqueue = 0;
   qnode_iwq_aal5_cfg[0].num_buffers = 50;
   qnode_iwq_aal5_cfg[0].adjunct_pool = 0;

   memset (qnode_iwq_cfg, 0, sizeof (WP_qnode_iwq));
   qnode_iwq_cfg[0].interruptqueue = 0;
   qnode_iwq_cfg[0].num_buffers = 100;
   qnode_iwq_cfg[0].adjunct_pool = 0;

   memset (qnode_iw_host_cfg, 0, sizeof (WP_qnode_iwq));
   qnode_iw_host_cfg[0].interruptqueue = 0;
   qnode_iw_host_cfg[0].num_buffers = 20;
   qnode_iw_host_cfg[0].adjunct_pool = 0;

   memset(module_pce_init_cfg, 0, sizeof (WP_module_pce_init));
   module_pce_init_cfg[0].parser_info = &pce_parser_info;
   module_pce_init_cfg[0].hardware_rules_num = 4000;
   module_pce_init_cfg[0].filters_num = MAX_G999_STREAMS + 10;
   module_pce_init_cfg[0].pkt_rx_channel_interfaces_num = MAX_G999_STREAMS + 10;
   module_pce_init_cfg[0].iw_tx_channel_interfaces_num = MAX_G999_STREAMS;
   module_pce_init_cfg[0].pce_discard_mode = WP_PCE_DISCARD_MODE_ALL;
   module_pce_init_cfg[0].bridge_loop_prevention_mode = WP_ENABLE;
   module_pce_init_cfg[0].learning_mode = WP_ENABLE;
   module_pce_init_cfg[0].learning_queue_config.learning_queue_size = 30;
   module_pce_init_cfg[0].learning_queue_config.interrupt_mode = WP_LEARNING_POLLING_MODE;
   module_pce_init_cfg[0].learning_queue_config.int_queue_num = WP_IW_IRQT0;
   module_pce_init_cfg[0].learning_queue_config.interrupt_rate = 30;
   module_pce_init_cfg[0].device_interfaces_num = 1;
   /*AAL5 to IWGP*/
   module_pce_init_cfg[0].interfaces_on_atm_rx_mode = WP_ENABLE;
   module_pce_init_cfg[0].atm_rx_channel_interfaces_num = 1;

   memset(port_enet_cfg, 0, sizeof (WP_port_enet));
   port_enet_cfg[0].pkt_limits.max_tx_channels = 200;
   port_enet_cfg[0].pkt_limits.max_rx_channels = 100;
   port_enet_cfg[0].pkt_limits.pq_block_size = 0;
   port_enet_cfg[0].pkt_limits.n_pq_blocks = 0;
   port_enet_cfg[0].pkt_limits.emphy_devices = 0;
   port_enet_cfg[0].flowmode = WP_ENET_FMU_HIERARCHICAL_SHAPING_MODE;
   port_enet_cfg[0].interface_mode = WP_ENET_SGMII_1000;
   port_enet_cfg[0].rx_iw_bkgnd = WP_IW_BKGND_NOT_USED;

   memset(device_enet_g999_cfg, 0, sizeof (WP_device_enet_g999));
   device_enet_g999_cfg[0].g999_modes = WP_G999_MODE_ATM;
   device_enet_g999_cfg[0].max_g999_streams = 0;
   device_enet_g999_cfg[0].max_fragment_length = 256;
   device_enet_g999_cfg[0].pce_if_handle = 0;
   device_enet_g999_cfg[0].loopbackmode = WP_ENET_NORMAL; /* Currently works in serdes loopback */
   device_enet_g999_cfg[0].extended_params = NULL;

   device_enet_g999_atm[0].max_rx_channels = 100;
   device_enet_g999_atm[0].max_tx_channels = 200;
   device_enet_g999_atm[0].discard_atm_oam_crc10_error_cells = WP_ENABLE;
   device_enet_g999_atm[0].six_bytes_header_mode = WP_ENABLE;

   device_enet_g999_cfg[0].g999_atm = device_enet_g999_atm;

   memset (device_enet_g999_ex_cfg, 0, sizeof (WP_device_enet_g999_ex));
   device_enet_g999_ex_cfg[0].statistics_mode = WP_ENABLE;
   device_enet_g999_ex_cfg[0].enet_encap_mode = WP_DISABLE;
   device_enet_g999_ex_cfg[0].length_mode = WP_ENABLE;
   device_enet_g999_ex_cfg[0].preamble_size = 7;
   device_enet_g999_ex_cfg[0].inter_packet_gap = 12;
   device_enet_g999_ex_cfg[0].max_tx_slow_channels = 0;
   device_enet_g999_ex_cfg[0].rx_timestamp_type = 0;
   device_enet_g999_ex_cfg[0].tx_timestamp_type = 0;
   device_enet_g999_ex_cfg[0].rx_timestamp = WP_DISABLE;
   device_enet_g999_ex_cfg[0].tx_timestamp = WP_DISABLE;

   memset(device_enet_ex_cfg, 0, sizeof (WP_device_enet_ex));
   device_enet_ex_cfg[0].duplexmode = WP_ENET_FULLDUPLEX;
   device_enet_ex_cfg[0].rx_maclookup_mode = WP_DISABLE;
   device_enet_ex_cfg[0].rx_flowcontrol = WP_ENET_FLOW_ENABLE;
   device_enet_ex_cfg[0].tx_flowcontrol = WP_ENET_FLOW_ENABLE;
   device_enet_ex_cfg[0].rx_addrmode = WP_ENET_ADDR_ACCEPTALL;
   device_enet_ex_cfg[0].phystatmode = WP_ENET_STAT_ENABLE;
   device_enet_ex_cfg[0].tx_statmode = WP_ENET_STAT_ENABLE;
   device_enet_ex_cfg[0].rx_statmode = WP_ENET_STAT_ENABLE;
   device_enet_ex_cfg[0].tx_duplicate = WP_DISABLE;
   device_enet_ex_cfg[0].rx_filters = NULL;
   device_enet_ex_cfg[0].rx_timestamp = WP_DISABLE;
   device_enet_ex_cfg[0].tx_timestamp = WP_DISABLE;
   device_enet_ex_cfg[0].tx_timestamp_type = 0;
   device_enet_ex_cfg[0].rx_timestamp_type = 0;
   device_enet_ex_cfg[0].max_tx_slow_channels = 0;
   device_enet_ex_cfg[0].multi_class_config = NULL;

   memset(device_enet_cfg, 0, sizeof (WP_device_enet));
   device_enet_cfg[0].max_tx_channels = 10;
   device_enet_cfg[0].tx_maxsdu = MAX_DATA_LENGTH;
   device_enet_cfg[0].operating_speed = WP_UNUSED;
   memcpy (device_enet_cfg[0].mac_addr, mac_addr, 6);
   device_enet_cfg[0].tx_bit_rate = WP_TX_BIT_RATE_UNLIMITED;
   device_enet_cfg[0].loopbackmode = WP_ENET_NORMAL;
   device_enet_cfg[0].extended_params = NULL;

   memset(g999_stream_handle, 0, sizeof (WP_handle)*MAX_G999_STREAMS);
   memset(g999_stream_cfg, 0, sizeof (WP_g999_stream)*MAX_G999_STREAMS);
   g999_stream_cfg[0].stream_id = 0;
   memcpy (g999_stream_cfg[0].enet_encap_header, enet_encap_header, sizeof (WP_U8) * 12);
   g999_stream_cfg[0].tx_bit_rate = WP_TX_BIT_RATE_UNLIMITED;
   g999_stream_cfg[0].remove_enet_fcs = WP_DISABLE;

   memset(ch_enet_cfg, 0, sizeof (WP_ch_enet));
   ch_enet_cfg[0].intmode = WP_PKTCH_INT_DISABLE;
   ch_enet_cfg[0].iwmode = WP_PKTCH_IWM_ENABLE;
   ch_enet_cfg[0].testmode = WP_PKTCH_TEST_DISABLE;
   ch_enet_cfg[0].tx_pqblock = 0;
   ch_enet_cfg[0].tx_pqlevel = 0;
   ch_enet_cfg[0].tx_shaping_type = WP_FMU_SHAPING_TYPE_STRICT;
   ch_enet_cfg[0].tx_shaping_params = NULL;
   ch_enet_cfg[0].rx_maxsdu = 2500;
   ch_enet_cfg[0].tx_cwid = WP_CW_ID_A;
   ch_enet_cfg[0].tx_tq = 0;
   ch_enet_cfg[0].rx_queuedepth = 20;
   ch_enet_cfg[0].rx_timestamp = WP_DISABLE;

   memset(ch_aal5_cfg, 0, sizeof(ch_aal5_cfg));
   ch_aal5_cfg[0].vpi = VPI_INIT;
   ch_aal5_cfg[0].vci = VCI_INIT;
   ch_aal5_cfg[0].intmode = WP_ATMCH_INT_DISABLE;
   ch_aal5_cfg[0].iwmode = WP_ATMCH_IWM_ENABLE;
   ch_aal5_cfg[0].statmode = WP_ATMCH_STAT_ENABLE;
   ch_aal5_cfg[0].testmode = WP_ATMCH_TEST_DISABLE;
   ch_aal5_cfg[0].tx_shaping_type = WP_FMU_SHAPING_TYPE_STRICT;
   ch_aal5_cfg[0].tx_shaping_params = NULL;
   ch_aal5_cfg[0].tx_uumode = WP_ATMCH_UU_DISABLE;
   ch_aal5_cfg[0].rx_timestamp = WP_ATMCH_TIME_DISABLE;
   ch_aal5_cfg[0].rx_maxsdu = MAX_DATA_LENGTH;
   ch_aal5_cfg[0].tx_tq = 0;

   memset(rx_binding_bridging_cfg, 0, sizeof (WP_rx_binding_bridging));
   rx_binding_bridging_cfg[0].encap_mode = 0;
   rx_binding_bridging_cfg[0].mru = 512;
   rx_binding_bridging_cfg[0].vcfcs = 0;
   rx_binding_bridging_cfg[0].input_port = 0;
   rx_binding_bridging_cfg[0].pce_if_handle = 0;

   memset (tx_binding_cfg, 0, sizeof (tx_binding_cfg));
   tx_binding_cfg[0].dci_mode = WP_IW_DYN_CH_INSERT_DISABLE;
   tx_binding_cfg[0].maxt = 1000;

   memset (ch_iw_rx_cfg, 0, sizeof (WP_ch_iw_rx));
   ch_iw_rx_cfg[0].pqblock = 0;
   ch_iw_rx_cfg[0].pqlevel = 0;
   ch_iw_rx_cfg[0].tx_binding_type = WP_IW_TX_BINDING;
   ch_iw_rx_cfg[0].tx_binding_config = tx_binding_cfg;

   memset (ch_iw_tx_cfg, 0, sizeof (WP_ch_iw_tx));
   ch_iw_tx_cfg[0].iw_system = 0;
   ch_iw_tx_cfg[0].group_id = WP_IW_CH_GROUP_ID_NULL;
   ch_iw_tx_cfg[0].input_port = 0;
   ch_iw_tx_cfg[0].pce_if_handle = 0;

   memset (tx_agg_bridging, 0, sizeof (WP_iw_agg_bridging));
   tx_agg_bridging[0].tag = 2;
   tx_agg_bridging[0].txfunc = 0;
   tx_agg_bridging[0].iw_port = 0;
   tx_agg_bridging[0].rfcs = WP_IW_RFCS_ENABLE;
   tx_agg_bridging[0].l2_header_insert_mode = WP_IW_L2H_INSERT_DISABLE;
   tx_agg_bridging[0].vlan_tag_mode = WP_IW_VLAN_TAG_UNCHANGED;
   tx_agg_bridging[0].interruptqueue = WP_IW_IRQT1;
   tx_agg_bridging[0].error_pkt_mode = WP_IW_ERRPKT_DISCARD;
   tx_agg_bridging[0].intmode = WP_IW_INT_DISABLE;
   tx_agg_bridging[0].statmode = WP_IW_STAT_ENABLE;
   tx_agg_bridging[0].timestamp_mode = WP_IW_TIME_STAMP_DISABLE;
   tx_agg_bridging[0].ov_pool_mode = WP_IW_OV_POOL_DISABLE;
   tx_agg_bridging[0].fbp_drop_threshold = 0;
   tx_agg_bridging[0].replace_vlan_id = WP_IW_REPLACE_VTAG_DISABLE;
   tx_agg_bridging[0].vlan_id = 0x5;
   tx_agg_bridging[0].vpmt_handle = 0;
   tx_agg_bridging[0].mtu = MAX_DATA_LENGTH;
   tx_agg_bridging[0].prefix_length = 0;
   memcpy (tx_agg_bridging[0].prefix_header, prefix_header, 32);
   tx_agg_bridging[0].policer_enable = WP_IW_POLICER_DISABLE;
   tx_agg_bridging[0].policer_config = NULL;
   tx_agg_bridging[0].cong_mode = WP_IW_CONGESTION_DISABLE;
   tx_agg_bridging[0].cong_threshold_param = NULL;
   tx_agg_bridging[0].flow_agg_type = WP_IW_AGG_PRIMARY;
   tx_agg_bridging[0].external_vlan.mode = WP_IW_EXTERN_VLAN_NONE;
   tx_agg_bridging[0].bc_mc_priority.bc_priority_mode = 0;
   tx_agg_bridging[0].bc_mc_priority.bc_priority_offset = 0;
   tx_agg_bridging[0].bc_mc_priority.mc_priority_mode = 0;
   tx_agg_bridging[0].bc_mc_priority.mc_priority_offset = 0;
   tx_agg_bridging[0].reserved_agg_mode = WP_IW_NOT_RESERVED;
   tx_agg_bridging[0].next_sys_preserve_inport = 0;
   tx_agg_bridging[0].next_sys_preserve_inport_params = 0;
   tx_agg_bridging[0].mac_replace_mode = WP_IW_MAC_REPLACE_DISABLED;
   memcpy (tx_agg_bridging[0].new_dst_mac, mac_addr, 6);
   memcpy (tx_agg_bridging[0].new_src_mac, mac_addr, 6);
   tx_agg_bridging[0].multi_cong_prof = 0;
   tx_agg_bridging[0].extraction_length = 0;
   tx_agg_bridging[0].policer_handle = 0;
   tx_agg_bridging[0].vpmt_accessing_mode = WP_IW_VPMT_ACCESSING_EGRESS_EXTERNAL;
   tx_agg_bridging[0].n_prefix_remark_fields = 0;
/*    tx_agg_bridging[0].prefix_remark_offset = 0; */
   tx_agg_bridging[0].forwarding_dscp_remark = 0;

   memset (iw_agg_hq_cfg, 0, sizeof (WP_iw_agg_hq));
   iw_agg_hq_cfg[0].tag = 3;
   iw_agg_hq_cfg[0].txfunc = 0;
   iw_agg_hq_cfg[0].hq_agg_mode = WP_IW_HQ_NOT_RESERVED;
   iw_agg_hq_cfg[0].mtu = MAX_DATA_LENGTH;
   iw_agg_hq_cfg[0].interruptqueue = WP_IW_IRQT0;
   iw_agg_hq_cfg[0].intmode = WP_IW_INT_DISABLE;
   iw_agg_hq_cfg[0].statmode = WP_IW_STAT_ENABLE;
   iw_agg_hq_cfg[0].timestamp_mode = WP_IW_TIME_STAMP_DISABLE;
   iw_agg_hq_cfg[0].ov_pool_mode = WP_IW_OV_POOL_DISABLE;
   iw_agg_hq_cfg[0].fbp_drop_threshold = 0;
   iw_agg_hq_cfg[0].tx_direction = WP_IW_NETWORK_GLOBAL;
   iw_agg_hq_cfg[0].flow_analyze = WP_IW_FLOW_PASS;
   iw_agg_hq_cfg[0].force_analyze = WP_IW_FORCE_NO;
   iw_agg_hq_cfg[0].l4_checksum_recal = WP_IW_L4_NO_CHECKSUM;
   iw_agg_hq_cfg[0].nexthop = 0;
   iw_agg_hq_cfg[0].flow_agg_type = WP_IW_AGG_PRIMARY;
   iw_agg_hq_cfg[0].monitor_queue_size = 0;
   iw_agg_hq_cfg[0].iw_port = 0;
   iw_agg_hq_cfg[0].host_term_control = WP_IW_HQ_CONTROL_NONE;
   iw_agg_hq_cfg[0].policer_mode = WP_IW_POLICER_DISABLE;
   iw_agg_hq_cfg[0].policer_handle = 0;

   memset (iw_agg_generic_cfg, 0, sizeof (WP_iw_agg_generic));
   iw_agg_generic_cfg[0].tag = 4;
   iw_agg_generic_cfg[0].txfunc = 0;
   iw_agg_generic_cfg[0].iw_port = 0;
   iw_agg_generic_cfg[0].rfcs = WP_IW_RFCS_ENABLE;
   iw_agg_generic_cfg[0].interruptqueue = WP_IW_IRQT1;
   iw_agg_generic_cfg[0].error_pkt_mode = WP_IW_ERRPKT_DISCARD;
   iw_agg_generic_cfg[0].intmode = WP_IW_INT_ENABLE;
   iw_agg_generic_cfg[0].statmode = WP_IW_STAT_ENABLE;
   iw_agg_generic_cfg[0].timestamp_mode = WP_IW_TIME_STAMP_DISABLE;
   iw_agg_generic_cfg[0].mtu = 9216;
   iw_agg_generic_cfg[0].flow_agg_type = WP_IW_FLOW_AGG_PRIMARY;
   iw_agg_generic_cfg[0].policer_handle = 0;
   iw_agg_generic_cfg[0].pecs_handle = 0;
   iw_agg_generic_cfg[0].pecs_flow_info = 0;
   iw_agg_generic_cfg[0].pecs_global_info_handle = 0;

   memset (bridge_port_cfg, 0, sizeof (WP_bridge_port));
   bridge_port_cfg[0].tag = 0x2000;
   bridge_port_cfg[0].direct_mapping = WP_IW_DIRECT_MAP_ENABLE;
   bridge_port_cfg[0].flow_agg = 0;
   bridge_port_cfg[0].flooding_term_mode = WP_IW_HOST_TERM_MODE;
   bridge_port_cfg[0].learning_mode = WP_IW_LEARNING_DISABLE;
   bridge_port_cfg[0].in_filter_mode = WP_IW_INGRESS_FILTER_DISABLE;
   bridge_port_cfg[0].vlan_param.vlan_acceptance_mode =
      WP_IW_ACCEPT_TAGGED_UNTAGGED;
   bridge_port_cfg[0].vlan_param.vlan_tag = 0x5;
   bridge_port_cfg[0].vlan_param.vlan_tunnel =
      WP_IW_VLAN_TUNNEL_1Q_IN_1Q_DISABLE;
   bridge_port_cfg[0].vlan_param.vlan_priority_enforce_mode =
      WP_IW_VLAN_PRIORITY_ENFORCE_DISABLED;
   bridge_port_cfg[0].vlan_param.stag_vlan_etype = 0;
   bridge_port_cfg[0].max_mac_addresses = 100;
   bridge_port_cfg[0].group_tag = 0;
   bridge_port_cfg[0].group_filtering_mode = 0;
   bridge_port_cfg[0].unk_mac_sa_filter = WP_IW_UNK_MACSA_FILTER_DISABLE;
   bridge_port_cfg[0].unk_mc_mode = WP_IW_UNK_MC_DROP;
   bridge_port_cfg[0].bc_ht_mode = WP_IW_BC_HT_DISABLE;
   bridge_port_cfg[0].input_filters_mask = 0xFFFFFFFF;
   bridge_port_cfg[0].output_filters_mask = 0xFFFFFFFF;
   bridge_port_cfg[0].statmode = WP_IW_PORT_STAT_ENABLE;
   bridge_port_cfg[0].unk_uc_mode = WP_IW_UNK_UC_SR_DISABLE;
   bridge_port_cfg[0].classification_flag =
      WP_IW_BPORT_CLASSIFICATION_DISABLED;
   bridge_port_cfg[0].adv_unk_lookup_mode =
      WP_IW_ADV_UNK_LOOKUP_DISABLED;
   bridge_port_cfg[0].cfi_ht_mode = WP_IW_CFI_HT_DISABLE;
   bridge_port_cfg[0].reserved_mc_ht_mode = WP_IW_RES_MC_HT_DISABLE;
   bridge_port_cfg[0].predefined_ht_mode = WP_DISABLE;
   bridge_port_cfg[0].res_mc_action_table.bridge_group_mc_address = WP_IW_BPORT_RES_MC_DENY;
   bridge_port_cfg[0].res_mc_action_table.pause_mc_address = WP_IW_BPORT_RES_MC_DENY;
   bridge_port_cfg[0].res_mc_action_table.slow_protocol_mc_address = WP_IW_BPORT_RES_MC_DENY;
   bridge_port_cfg[0].res_mc_action_table.eap_over_lan_mc_address = WP_IW_BPORT_RES_MC_DENY;
   bridge_port_cfg[0].res_mc_action_table.link_constrained_protocols_addresses = WP_IW_BPORT_RES_MC_DENY;
   bridge_port_cfg[0].res_mc_action_table.bridge_management_group_mc_address = WP_IW_BPORT_RES_MC_DENY;
   bridge_port_cfg[0].res_mc_action_table.gmrp_mc_address = WP_IW_BPORT_RES_MC_DENY;
   bridge_port_cfg[0].res_mc_action_table.gvrp_mc_address = WP_IW_BPORT_RES_MC_DENY;
   bridge_port_cfg[0].res_mc_action_table.reserved_garp_mc_addresses = WP_IW_BPORT_RES_MC_DENY;
   bridge_port_cfg[0].res_mc_action_table.slow_protocol_efm_oam_mc_address = WP_IW_BPORT_RES_MC_DENY;
   bridge_port_cfg[0].enet_oam_params = NULL;
   bridge_port_cfg[0].dfc_log_mode = WP_IW_PORT_DFC_LOG_DISABLE;

   memset (dfc_sys_info_cfg, 0, sizeof (WP_dfc_sys_info));
   dfc_sys_info_cfg[0].classification_hash_size = WP_IW_HASH_UNUSED;
   dfc_sys_info_cfg[0].port_filtering = WP_IW_PORT_FILTERING_DISABLED;
   dfc_sys_info_cfg[0].deny_eq_ip = WP_IW_DENY_EQUAL_IP_DISABLE;
   dfc_sys_info_cfg[0].dfc_options = 0;

   memset (iw_sys_bridging_cfg, 0, sizeof (WP_iw_sys_bridging));
   iw_sys_bridging_cfg[0].tag = 0;
   iw_sys_bridging_cfg[0].max_flows = 0;
   iw_sys_bridging_cfg[0].classification_mode = WP_IW_NULL_CLASSIFIER;
   iw_sys_bridging_cfg[0].classifier_config.classification_parsing_mode = 
      WP_IW_NULL_CLASSIFIER;
   iw_sys_bridging_cfg[0].classifier_config.max_classification_rules = 0;
   iw_sys_bridging_cfg[0].classifier_config.number_parse_fields = 0;
   iw_sys_bridging_cfg[0].classifier_config.parse_keys = NULL;
   iw_sys_bridging_cfg[0].learning_mode = WP_IW_BRIDGE_LEARNING_DISABLE;
   iw_sys_bridging_cfg[0].learning_queue.int_queue_num = WP_IW_IRQT1;
   iw_sys_bridging_cfg[0].learning_queue.learning_queue_size = 0;
   iw_sys_bridging_cfg[0].learning_queue.interrupt_enable = WP_DISABLE;
   iw_sys_bridging_cfg[0].learning_queue.interrupt_rate = 0;
   iw_sys_bridging_cfg[0].forwarding_table_size = WP_IW_HASH_UNUSED;
   iw_sys_bridging_cfg[0].member_set_size = WP_IW_HASH_UNUSED;
   iw_sys_bridging_cfg[0].buffer_gap = 0x80;
   iw_sys_bridging_cfg[0].max_iw_ports = MAX_G999_STREAMS*2 + 10;
   iw_sys_bridging_cfg[0].dfc_info = dfc_sys_info_cfg;
   iw_sys_bridging_cfg[0].svl_mode = WP_IW_SVL_DISABLED;
   iw_sys_bridging_cfg[0].stag_ether_type = WP_IW_SYS_BRIDGE_STAG_VLAN_ETYPE_DISABLE;
   iw_sys_bridging_cfg[0].parsing_mode = WP_IW_BRIDGE_PARSE_IDENTIFY_PRI_TAG;
   iw_sys_bridging_cfg[0].dfc_log_mode = WP_IW_BRIDGE_DFC_LOG_DISABLE;
   iw_sys_bridging_cfg[0].iw_params_load_mode = WP_IW_PARAMS_LOAD_DFC_PARAMS_DISABLE;

   memset (ipv4_sys_info_cfg, 0, sizeof (WP_iw_ipv4_sys_info));
   ipv4_sys_info_cfg[0].classification_mode = WP_IW_NULL_CLASSIFIER;
   ipv4_sys_info_cfg[0].max_lpm_flows = 0;
   ipv4_sys_info_cfg[0].dfc_info = dfc_sys_info_cfg;

   memset (l1_group_shaping_params, 0, sizeof (WP_fmu_shaping_cir_eir));
   l1_group_shaping_params[0].cir = 1000000000;
   l1_group_shaping_params[0].cbs = 80000;
   l1_group_shaping_params[0].eir = 1000000000;
   l1_group_shaping_params[0].ebs = 80000;
   l1_group_shaping_params[0].flags = 0;

   memset (shaping_group_type_packet_l1_cfg, 0, sizeof (WP_shaping_group_type_packet));
   shaping_group_type_packet_l1_cfg[0].group_level       = WP_L1_GROUP;
   shaping_group_type_packet_l1_cfg[0].tx_shaping_type   = WP_FMU_SHAPING_TYPE_CIR_EIR;
   shaping_group_type_packet_l1_cfg[0].tx_shaping_params = &l1_group_shaping_params[0];
   shaping_group_type_packet_l1_cfg[0].num_fifos         = WP_NUM_FIFOS_8;
   shaping_group_type_packet_l1_cfg[0].block_level       = 0;
   shaping_group_type_packet_l1_cfg[0].group_mode        = WP_MODE_HW;

   memset (l2_group_shaping_params, 0, sizeof (WP_fmu_shaping_cir_eir));
   l2_group_shaping_params[0].cir = 1000000000;
   l2_group_shaping_params[0].cbs = 80000;
   l2_group_shaping_params[0].eir = 1000000000;
   l2_group_shaping_params[0].ebs = 80000;
   l2_group_shaping_params[0].flags = 0;

   memset (shaping_group_type_packet_l2_cfg, 0, sizeof (WP_shaping_group_type_packet));
   shaping_group_type_packet_l2_cfg[0].group_level       = WP_L2_GROUP;
   shaping_group_type_packet_l2_cfg[0].tx_shaping_type   = WP_FMU_SHAPING_TYPE_CIR_EIR;
   shaping_group_type_packet_l2_cfg[0].tx_shaping_params = &l2_group_shaping_params[0];
   shaping_group_type_packet_l2_cfg[0].num_fifos         = WP_NUM_FIFOS_8;
   shaping_group_type_packet_l2_cfg[0].block_level       = 0;
   shaping_group_type_packet_l2_cfg[0].group_mode        = WP_MODE_HW;

   memset (qnode_iw_mcq_cfg, 0, sizeof (WP_qnode_iw_mcq));
   qnode_iw_mcq_cfg[0].interruptqueue = 0;
   qnode_iw_mcq_cfg[0].num_buffers = 100;
   qnode_iw_mcq_cfg[0].offset = 0x40;
   qnode_iw_mcq_cfg[0].size = 128;
   qnode_iw_mcq_cfg[0].pad = 9;

   memset (iw_mc_member_cfg, 0, sizeof (WP_iw_mc_member));
   iw_mc_member_cfg[0].txfunc = 0;
   iw_mc_member_cfg[0].service_class_index = 0;
   iw_mc_member_cfg[0].buffers_threshold = 0;
   iw_mc_member_cfg[0].iw_port = 0;
   iw_mc_member_cfg[0].edit_size = 0x10;
   iw_mc_member_cfg[0].data_copy_mode = WP_IW_MC_MEMBER_DATACOPY_DISABLE;
   iw_mc_member_cfg[0].iw_port_check_mode = WP_IW_MC_MEMBER_IWPORT_CHECK_ENABLE;
   iw_mc_member_cfg[0].src_ip_filtering_mode = WP_IW_SRC_IP_FILTERING_DISABLE;

   memset (iw_mc_group_cfg, 0, sizeof (WP_iw_mc_group));
   iw_mc_group_cfg[0].type = WP_IW_MC_UNRESTRICTED;
   iw_mc_group_cfg[0].mode = (WP_IW_MC_MODE_STAT_ENABLE |
                              WP_IW_MC_MODE_MEMBERS_STAT_ENABLE);
   iw_mc_group_cfg[0].max_iw_mc_members = 300;
   iw_mc_group_cfg[0].qnode = 0;
   iw_mc_group_cfg[0].encaps_gap = 0x40;
   iw_mc_group_cfg[0].classes_config = NULL;
   iw_mc_group_cfg[0].identifier = WP_UNUSED;

   memset (iw_agg_multicast_cfg, 0, sizeof (WP_iw_agg_multicast));
   iw_agg_multicast_cfg[0].tag = 0;
   iw_agg_multicast_cfg[0].type = WP_IW_FLOW_AGG_PRIMARY;
   iw_agg_multicast_cfg[0].mc_group = 0;
   iw_agg_multicast_cfg[0].mtu = MAX_DATA_LENGTH;
   iw_agg_multicast_cfg[0].intmode = WP_IW_INT_DISABLE;
   iw_agg_multicast_cfg[0].interruptqueue = WP_IW_IRQT1;
   iw_agg_multicast_cfg[0].statmode = WP_IW_STAT_ENABLE;
   iw_agg_multicast_cfg[0].timestamp_mode = WP_IW_TIME_STAMP_DISABLE;
   iw_agg_multicast_cfg[0].ov_pool_mode = WP_IW_OV_POOL_ENABLE;
   iw_agg_multicast_cfg[0].fbp_drop_threshold = 0;
   iw_agg_multicast_cfg[0].policer_enable = WP_IW_POLICER_DISABLE;
   iw_agg_multicast_cfg[0].policer_config = NULL;

} /* WTE_DefaultConfigs */

void WTE_init(void)
{
   WP_status status;

   status = WP_DriverInit();
   terminate_on_error(status, "WP_DriverInit", __LINE__);

#if WP_DEBUG_ENABLE_WDDI_LOG
   status = WP_ControlRegister(WP_DEBUG_CALLBACK_WDDI_LOG, WTI_LogCallBack);
#endif
   terminate_on_error(status, "WP_ControlRegister WTI_LogCallBack", __LINE__);
#if !defined(WINSIM)  && !defined(VERILOG)
   status = WPX_BoardConfigure(WP_WINPATH(DEFAULT_WPID), WPX_CONFIGURE_DEFAULT_CHECKIN);
   terminate_on_error(status, "WPX_BoardConfigure()", __LINE__);
#endif

   int_ram_partition.policer_entries = 0;
   int_ram_partition.pce_hw_rules_num = module_pce_init_cfg[0].hardware_rules_num;
   WT_SetIntRamPartition(WP_WINPATH(DEFAULT_WPID), &int_ram_partition);

   status = WP_SysInit(WP_WINPATH(DEFAULT_WPID), context_cfg);
   terminate_on_error(status,"WP_SysInit", __LINE__);

   /*Init PCE module*/
   status = WP_ModuleInit(WP_SYSHANDLE(DEFAULT_WPID), WP_WDDI_MODULE_PCE, module_pce_init_cfg);
   terminate_on_error(status, "WPI_PceModuleInit()", __LINE__);

} /* WTE_init */

void WTE_Create_pools(void)
{
   /* Create the Buffer pool(s) - atm <-> enet  system 1 */
   pool_1536 = WP_PoolCreate(WP_WINPATH(DEFAULT_WPID), WP_pool_buffer, pool_buffer_data_cfg_1536);
   terminate_on_error(pool_1536, "WP_PoolCreate() pool_1536", __LINE__);

   /* Create the Ring pool(s) - Host (ATM) */
   pool_ring_host = WP_PoolCreate(WP_WINPATH(DEFAULT_WPID), WP_pool_ring, pool_ring_data_cfg);
   terminate_on_error(pool_ring_host, "WP_PoolCreate() pool_ring_host", __LINE__);

   /* Create the Buffer pool(s) - interworking*/
   iw_pool1 = WP_PoolCreate(WP_WINPATH(DEFAULT_WPID), WP_pool_iwbuffer, pool_buffer_data_768_cfg);
   terminate_on_error(iw_pool1, "WP_PoolCreate 1", __LINE__);

   /* Create the Buffer pool(s) - interworking*/
   iw_pool_host = WP_PoolCreate(WP_WINPATH(DEFAULT_WPID), WP_pool_iwbuffer, pool_buffer_data_2048_cfg);
   terminate_on_error(iw_pool_host, "WP_PoolCreate iw_pool_host", __LINE__);

   qnode_hostq_cfg->pool_buffer = pool_1536;
   qnode_hostq_cfg->pool_ring = pool_ring_host;
   qnode_host = WP_QNodeCreate(WP_WINPATH(DEFAULT_WPID), WP_QNODE_HOSTQ, qnode_hostq_cfg);
   terminate_on_error(qnode_host, "WP_QNodeCreate() qnode_host", __LINE__);

   /* Create an interworking queue node */
   qnode_iwq_cfg->adjunct_pool = iw_pool1;
   qniw_enet = WP_QNodeCreate(WP_WINPATH(DEFAULT_WPID), WP_QNODE_IWQ | WP_QNODE_OPT_FMU, qnode_iwq_cfg);
   terminate_on_error(qniw_enet, "WP_QNodeCreate IW_ENETQ", __LINE__);

   /* Create an interworking queue node */
   qnode_iw_host_cfg->adjunct_pool = iw_pool_host;
   qniw_host = WP_QNodeCreate(WP_WINPATH(DEFAULT_WPID), WP_QNODE_IWQ | WP_QNODE_OPT_FMU, qnode_iw_host_cfg);
   terminate_on_error(qniw_host, "WP_QNodeCreate IW_HOST", __LINE__);

}/* WTE_Create_pools */

void WTE_G999_Stream_Configuration(WP_U32 stream_number)
{
   WP_U8 enet_encap_header[12] = {0x12, 0x34, 0x56, 0x78, 0x9a, 0x00,
                                  0xbc, 0x9a, 0x78, 0x56, 0x34, 0x12};

   g999_stream_cfg[stream_number].stream_id = stream_number;
   g999_stream_cfg[stream_number].tx_bit_rate = WP_TX_BIT_RATE_UNLIMITED;
   g999_stream_cfg[stream_number].remove_enet_fcs = WP_DISABLE;

   enet_encap_header[5] = stream_number;
   enet_encap_header[11] = stream_number;

   memcpy (g999_stream_cfg[stream_number].enet_encap_header, enet_encap_header, sizeof (WP_U8) * 12);
} /* WTE_G999_Stream_Configuration */

void WTE_Create_ports(void)
{
   /*********************************/
   /* Create Enet Ports and Devices */
   /*********************************/

   /* Init serdes */
   WP_status status;
#ifdef WP_BOARD_WDS3_SL
      status = WPX_BoardSerdesInit(WP_WINPATH(DEFAULT_WPID), WP_PORT_ENET1, WPX_SERDES_LOOPBACK);
      terminate_on_error(status, "WPX_BoardSerdesSetLoopback()", __LINE__);
      status = WPX_BoardSerdesInit(WP_WINPATH(DEFAULT_WPID), WP_PORT_ENET2, WPX_SERDES_LOOPBACK);
      terminate_on_error(status, "WPX_BoardSerdesSetLoopback()", __LINE__);

      status = WPX_BoardSerdesInit(WP_WINPATH(DEFAULT_WPID), WP_PORT_ENET9, WPX_SERDES_LOOPBACK);
      terminate_on_error(status, "WPX_BoardSerdesSetLoopback()", __LINE__);
      status = WPX_BoardSerdesInit(WP_WINPATH(DEFAULT_WPID), WP_PORT_ENET10, WPX_SERDES_LOOPBACK);
      terminate_on_error(status, "WPX_BoardSerdesSetLoopback()", __LINE__);

      enetp1 = WP_PortCreate(WP_WINPATH(DEFAULT_WPID), WP_PORT_ENET1, port_enet_cfg);
      terminate_on_error(enetp1, "WP_PortCreate enetp1 WP_PORT_ENET1", __LINE__);

      enetp2 = WP_PortCreate(WP_WINPATH(DEFAULT_WPID), WP_PORT_ENET2, port_enet_cfg);
      terminate_on_error(enetp2, "WP_PortCreate enetp2 WP_PORT_ENET2", __LINE__);

      dummy_port1 = WP_PortCreate(WP_WINPATH(DEFAULT_WPID), WP_PORT_ENET9, port_enet_cfg);
      terminate_on_error(dummy_port1, "WP_PortCreate dummy_port WP_PORT_ENET9", __LINE__);
      dummy_device1 = WP_DeviceCreate(dummy_port1, WP_PHY(0), WP_DEVICE_ENET, device_enet_cfg);
      terminate_on_error(dummy_device1, "WP_DeviceCreate Enet over dummy_port1", __LINE__);

      dummy_port2 = WP_PortCreate(WP_WINPATH(DEFAULT_WPID), WP_PORT_ENET10, port_enet_cfg);
      terminate_on_error(dummy_port2, "WP_PortCreate dummy_port2 WP_PORT_ENET10", __LINE__);
      dummy_device2 = WP_DeviceCreate(dummy_port2, WP_PHY(0), WP_DEVICE_ENET, device_enet_cfg);
      terminate_on_error(dummy_device2, "WP_DeviceCreate Enet over dummy_port2", __LINE__);
#else
   status = WPX_BoardSerdesInit(WP_WINPATH(DEFAULT_WPID), WP_PORT_ENET13, WPX_SERDES_LOOPBACK);
   terminate_on_error(status, "WPX_BoardSerdesSetLoopback()", __LINE__);
   status = WPX_BoardSerdesInit(WP_WINPATH(DEFAULT_WPID), WP_PORT_ENET14, WPX_SERDES_LOOPBACK);
   terminate_on_error(status, "WPX_BoardSerdesSetLoopback()", __LINE__);

   status = WPX_BoardSerdesInit(WP_WINPATH(DEFAULT_WPID), WP_PORT_ENET3, WPX_SERDES_LOOPBACK);
   terminate_on_error(status, "WPX_BoardSerdesSetLoopback()", __LINE__);
   status = WPX_BoardSerdesInit(WP_WINPATH(DEFAULT_WPID), WP_PORT_ENET15, WPX_SERDES_LOOPBACK);
   terminate_on_error(status, "WPX_BoardSerdesSetLoopback()", __LINE__);

   enetp1 = WP_PortCreate(WP_WINPATH(DEFAULT_WPID), WP_PORT_ENET13, port_enet_cfg);
   terminate_on_error(enetp1, "WP_PortCreate enetp1 WP_PORT_ENET13", __LINE__);

   enetp2 = WP_PortCreate(WP_WINPATH(DEFAULT_WPID), WP_PORT_ENET14, port_enet_cfg);
   terminate_on_error(enetp2, "WP_PortCreate enetp2 WP_PORT_ENET14", __LINE__);

   dummy_port1 = WP_PortCreate(WP_WINPATH(DEFAULT_WPID), WP_PORT_ENET3, port_enet_cfg);
   terminate_on_error(dummy_port1, "WP_PortCreate dummy_port WP_PORT_ENET3", __LINE__);
   dummy_device1 = WP_DeviceCreate(dummy_port1, WP_PHY(0), WP_DEVICE_ENET, device_enet_cfg);
   terminate_on_error(dummy_device1, "WP_DeviceCreate Enet over dummy_port1", __LINE__);

   dummy_port2 = WP_PortCreate(WP_WINPATH(DEFAULT_WPID), WP_PORT_ENET15, port_enet_cfg);
   terminate_on_error(dummy_port2, "WP_PortCreate dummy_port2 WP_PORT_ENET15", __LINE__);
   dummy_device2 = WP_DeviceCreate(dummy_port2, WP_PHY(0), WP_DEVICE_ENET, device_enet_cfg);
   terminate_on_error(dummy_device2, "WP_DeviceCreate Enet over dummy_port2", __LINE__);
#endif

   /* Create a host port */
   host_port = WP_PortCreate(WP_WINPATH(DEFAULT_WPID), WP_PORT_IW_HOST, NULL);
   terminate_on_error(host_port, "PortCreate", __LINE__);

   /**********************************************************/
   /* Create an Enet and G999 devices over Group A port      */
   /**********************************************************/
   device_enet_g999_cfg[0].max_g999_streams = max_g999_streams_in_use[WT_WINNET_GROUP_A];
   group_a_g999_dev = WP_DeviceCreate(enetp1, WP_PHY(0), WP_DEVICE_ENET_G999, device_enet_g999_cfg);
   terminate_on_error(group_a_g999_dev, "WP_DeviceCreate G999 group_a_g999_dev", __LINE__);

   device_enet_g999_cfg[0].extended_params = device_enet_g999_ex_cfg;
   status = WP_DeviceModify(group_a_g999_dev, WP_DEV_MOD_G999_EXTENDED_MODES | WP_DEV_MOD_ENET_RX_STATMODE | WP_DEV_MOD_ENET_TX_STATMODE, device_enet_g999_cfg);
   terminate_on_error(status, "WP_DeviceModify group_a_g999_dev WP_DEV_MOD_G999_EXTENDED_MODES", __LINE__);

   memset(device_enet_cfg, 0, sizeof (WP_device_enet));
   device_enet_cfg[0].max_tx_channels = port_enet_cfg[0].pkt_limits.max_tx_channels;
   device_enet_cfg[0].operating_speed = WP_UNUSED;
   memcpy (device_enet_cfg[0].mac_addr, mac_address_arr, 6);
   device_enet_cfg[0].loopbackmode = WP_ENET_NORMAL;
   device_enet_cfg[0].extended_params = NULL;
   device_enet_cfg[0].tx_maxsdu = MAX_DATA_LENGTH;
   device_enet_cfg[0].tx_bit_rate = WP_TX_BIT_RATE_UNLIMITED;

   group_a_enet_dev = WP_DeviceCreate(enetp1, WP_PHY(0), WP_DEVICE_ENET, device_enet_cfg);
   terminate_on_error(group_b_enet_dev, "WP_DeviceCreate Enet over enetp1", __LINE__);

   /**********************************************************/
   /* Create an Enet and G999 devices over Group B port      */
   /**********************************************************/
   device_enet_g999_cfg[0].extended_params = NULL;
   device_enet_g999_cfg[0].max_g999_streams = max_g999_streams_in_use[WT_WINNET_GROUP_B];
   group_b_g999_dev = WP_DeviceCreate(enetp2, WP_PHY(0), WP_DEVICE_ENET_G999, device_enet_g999_cfg);
   terminate_on_error(group_b_g999_dev, "WP_DeviceCreate G999 group_b_g999_dev", __LINE__);

   device_enet_g999_cfg[0].extended_params = device_enet_g999_ex_cfg;
   status = WP_DeviceModify(group_b_g999_dev, WP_DEV_MOD_G999_EXTENDED_MODES | WP_DEV_MOD_ENET_RX_STATMODE | WP_DEV_MOD_ENET_TX_STATMODE, device_enet_g999_cfg);
   terminate_on_error(status, "WP_DeviceModify group_b_g999_dev WP_DEV_MOD_G999_EXTENDED_MODES", __LINE__);

   memset(device_enet_cfg, 0, sizeof (WP_device_enet));
   device_enet_cfg[0].max_tx_channels = port_enet_cfg[0].pkt_limits.max_tx_channels;
   device_enet_cfg[0].operating_speed = WP_UNUSED;
   memcpy (device_enet_cfg[0].mac_addr, mac_address_arr, 6);
   device_enet_cfg[0].loopbackmode = WP_ENET_NORMAL;
   device_enet_cfg[0].extended_params = NULL;
   device_enet_cfg[0].tx_maxsdu = MAX_DATA_LENGTH;
   device_enet_cfg[0].tx_bit_rate = WP_TX_BIT_RATE_UNLIMITED;

   group_b_enet_dev = WP_DeviceCreate(enetp2, WP_PHY(0), WP_DEVICE_ENET, device_enet_cfg);
   terminate_on_error(group_b_enet_dev, "WP_DeviceCreate Enet over enetp2", __LINE__);
   /*******************************************************/

   /* Create a host device */
   host_dev = WP_DeviceCreate(host_port, WP_PHY(0), WP_DEVICE_IW_HOST, NULL);
   terminate_on_error(host_dev, "WP_DeviceCreate host_dev", __LINE__);

} /* WTE_Create_ports */


void WTE_CreateStreamsGroupA(void)
{
   WP_U32 ii = 0;
   WP_CHAR temp_string[256];
   WP_U8 max_streams = max_g999_streams_in_use[WT_WINNET_GROUP_A];

   /********************************************/
   /* All but the last stream are Enet streams */
   /********************************************/
   for (ii=0 ; ii<max_streams ; ii++)
   {
      WTE_G999_Stream_Configuration(ii);
      g999_stream_handle[ii] = WP_G999StreamCreate(group_a_g999_dev, WP_G999_STREAM_ENET, &g999_stream_cfg[ii]);
      sprintf (temp_string, "WP_G999StreamCreate ENET g999_stream_handle[%d]", ii);
      terminate_on_error(g999_stream_handle[ii], temp_string, __LINE__);
   }
} /* WTE_CreateStreamsGroupA */

void WTE_CreateStreamsGroupB(void)
{
   WP_U32 ii = 0, jj;
   WP_CHAR temp_string[256];
   WP_U8 max_streams = max_g999_streams_in_use[WT_WINNET_GROUP_B];

   /********************************************/
   /* All but the last stream are Enet streams */
   /********************************************/
   for (ii=0 ; ii<max_streams ; ii++)
   {
      jj = ii + max_g999_streams_in_use[WT_WINNET_GROUP_A];
      WTE_G999_Stream_Configuration(jj);
      g999_stream_handle[jj] = WP_G999StreamCreate(group_b_g999_dev, WP_G999_STREAM_ENET, &g999_stream_cfg[jj]);
      sprintf (temp_string, "WP_G999StreamCreate ENET g999_stream_handle[%d]", jj);
      terminate_on_error(g999_stream_handle[jj], temp_string, __LINE__);
   }
} /* WTE_CreateStreamsGroupB */

void WTE_CreatePceRulesDeviceGroupA(void)
{
   WP_pce_rule_classification rule_cfg = {0};
   WP_S32 ii;
   WP_device_enet_g999 temp_device_enet_g999_cfg[1];
   WP_status status;

   /*******************************************/
   /* PCE Device filter rules, one per stream */
   /*******************************************/
   for (ii=0 ; ii<max_g999_streams_in_use[WT_WINNET_GROUP_A] ; ii++)
   {
      memset (&rule_cfg, 0, sizeof (WP_pce_rule_classification));
      rule_cfg.tag = DEVICE_RULES_TAG_START + ii;
      rule_cfg.filter_handle = pce_device_filter;
      rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_DEVICE;
      rule_cfg.rule_fields[0].value.device_handle = group_a_g999_dev;
      rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_G999_STREAM_ID;
      rule_cfg.rule_fields[1].value.g999_stream_id = g999_stream_cfg[ii].stream_id;
      rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_LAST;

      rule_cfg.match_action = WP_PCE_RULE_MATCH_ACCEPT;
      rule_cfg.enabled = WP_ENABLE;
      rule_cfg.match_result[0].result_type = WP_PCE_RESULT_G999_STREAM;
      rule_cfg.match_result[0].param.g999_stream.g999_stream_handle = g999_stream_handle[ii];/*Half Eent, and half ATM*/
      rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;
      pce_device_rules[ii] = WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg);
      terminate_on_error(pce_device_rules[ii], "WP_PceRuleCreate pce_device_rules[ii]", __LINE__);
   }

   memset(temp_device_enet_g999_cfg, 0, sizeof (WP_device_enet_g999));
   temp_device_enet_g999_cfg[0].pce_if_handle = device_pce_if_handle;
   device_enet_g999_cfg[0].pce_if_handle = device_pce_if_handle;

   status = WP_DeviceModify(group_a_g999_dev, WP_DEV_MOD_G999_PCE_INTERFACE, temp_device_enet_g999_cfg);
   terminate_on_error(status, "WP_DeviceModify WP_DEV_MOD_G999_PCE_INTERFACE", __LINE__);
} /* WTE_CreatePceRulesDeviceGroupA */

void WTE_CreatePceRulesDeviceGroupB(void)
{
   WP_pce_rule_classification rule_cfg = {0};
   WP_S32 ii, jj;
   WP_device_enet_g999 temp_device_enet_g999_cfg[1];
   WP_U32 max_streams = max_g999_streams_in_use[WT_WINNET_GROUP_B];
   WP_status status;

   /*******************************************/
   /* PCE Device filter rules, one per stream */
   /*******************************************/
   for (ii=0 ; ii<max_streams ; ii++)
   {
      jj = ii + max_g999_streams_in_use[WT_WINNET_GROUP_A];
      memset (&rule_cfg, 0, sizeof (WP_pce_rule_classification));
      rule_cfg.tag = DEVICE_RULES_TAG_START + jj;
      rule_cfg.filter_handle = pce_device_filter;
      rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_DEVICE;
      rule_cfg.rule_fields[0].value.device_handle = group_b_g999_dev;
      rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_G999_STREAM_ID;
      rule_cfg.rule_fields[1].value.g999_stream_id = g999_stream_cfg[jj].stream_id;
      rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_LAST;

      rule_cfg.match_action = WP_PCE_RULE_MATCH_ACCEPT;
      rule_cfg.enabled = WP_ENABLE;
      rule_cfg.match_result[0].result_type = WP_PCE_RESULT_G999_STREAM;
      rule_cfg.match_result[0].param.g999_stream.g999_stream_handle = g999_stream_handle[jj];/*Half Eent, and half ATM*/
      rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;
      pce_device_rules[jj] = WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg);
      terminate_on_error(pce_device_rules[jj], "WP_PceRuleCreate pce_device_rules[jj]", __LINE__);
   }

   memset(temp_device_enet_g999_cfg, 0, sizeof (WP_device_enet_g999));
   temp_device_enet_g999_cfg[0].pce_if_handle = device_pce_if_handle;
   device_enet_g999_cfg[0].pce_if_handle = device_pce_if_handle;

   status = WP_DeviceModify(group_b_g999_dev, WP_DEV_MOD_G999_PCE_INTERFACE, temp_device_enet_g999_cfg);
   terminate_on_error(status, "WP_DeviceModify WP_DEV_MOD_G999_PCE_INTERFACE", __LINE__);
} /* WTE_CreatePceRulesDeviceGroupB */

void WTE_GroupsCreateGroupA(WP_U8 swap_mode)
{
   if (swap_mode == WT_SWAP_MODE_G999)
   {
      WP_S32 ii;
      WP_CHAR temp_string[256];
      WP_U32 max_streams = max_g999_streams_in_use[WT_WINNET_GROUP_A];

      for (ii=0 ; ii<max_streams ; ii++)
      {
         shaping_group_l1[ii] = WP_ShapingGroupCreate(
            g999_stream_handle[ii],
            WP_SHAPING_GROUP_TYPE_PACKET,
            shaping_group_type_packet_l1_cfg);
         sprintf (temp_string, "WP_ShapingGroupCreate shaping_group_l1[%d]", ii);
         terminate_on_error(shaping_group_l1[ii], temp_string, __LINE__);

         shaping_group_l2[ii] = WP_ShapingGroupCreate(
            shaping_group_l1[ii],
            WP_SHAPING_GROUP_TYPE_PACKET,
            shaping_group_type_packet_l2_cfg);
         sprintf (temp_string, "WP_ShapingGroupCreate shaping_group_l2[%d]", ii);
         terminate_on_error(shaping_group_l2[ii], temp_string, __LINE__);
      }
   }
   else /* (swap_mode == WT_SWAP_MODE_ENET) */
   {
      /******************************************/
      /* shaping groups for G999<-->Enet device */
      /******************************************/
      shaping_group_swap_l1[WT_WINNET_GROUP_A] = WP_ShapingGroupCreate(
         group_a_enet_dev,
         WP_SHAPING_GROUP_TYPE_PACKET,
         shaping_group_type_packet_l1_cfg);

      terminate_on_error(shaping_group_swap_l1[WT_WINNET_GROUP_A],
                         "WP_ShapingGroupCreate shaping_group_swap_l1[WT_WINNET_GROUP_A]",
                         __LINE__);

      shaping_group_swap_l2[WT_WINNET_GROUP_A] = WP_ShapingGroupCreate(
         shaping_group_swap_l1[WT_WINNET_GROUP_A],
         WP_SHAPING_GROUP_TYPE_PACKET,
         shaping_group_type_packet_l2_cfg);

      terminate_on_error(shaping_group_swap_l2[WT_WINNET_GROUP_A],
                         "WP_ShapingGroupCreate shaping_group_swap_l2[WT_WINNET_GROUP_A]",
                         __LINE__);
   }
} /* WTE_GroupsCreateGroupA */

void WTE_GroupsCreateGroupB(WP_U8 swap_mode)
{
   if (swap_mode == WT_SWAP_MODE_G999)
   {
      WP_S32 ii, jj;
      WP_CHAR temp_string[256];
      WP_U32 max_streams = max_g999_streams_in_use[WT_WINNET_GROUP_B];

      for (ii=0 ; ii<max_streams ; ii++)
      {
         jj = ii+max_g999_streams_in_use[WT_WINNET_GROUP_A];
         shaping_group_l1[jj] = WP_ShapingGroupCreate(
            g999_stream_handle[jj],
            WP_SHAPING_GROUP_TYPE_PACKET,
            shaping_group_type_packet_l1_cfg);
         sprintf (temp_string, "WP_ShapingGroupCreate shaping_group_l1[%d]", jj);
         terminate_on_error(shaping_group_l1[jj], temp_string, __LINE__);

         shaping_group_l2[jj] = WP_ShapingGroupCreate(
            shaping_group_l1[jj],
            WP_SHAPING_GROUP_TYPE_PACKET,
            shaping_group_type_packet_l2_cfg);
         sprintf (temp_string, "WP_ShapingGroupCreate shaping_group_l2[%d]", jj);
         terminate_on_error(shaping_group_l2[jj], temp_string, __LINE__);
      }
   }
   else /* (swap_mode == WT_SWAP_MODE_ENET) */
   {
      /******************************************/
      /* shaping groups for G999<-->Enet device */
      /******************************************/
      shaping_group_swap_l1[WT_WINNET_GROUP_B] = WP_ShapingGroupCreate(
         group_b_enet_dev,
         WP_SHAPING_GROUP_TYPE_PACKET,
         shaping_group_type_packet_l1_cfg);

      terminate_on_error(shaping_group_swap_l1[WT_WINNET_GROUP_B],
                         "WP_ShapingGroupCreate shaping_group_swap_l1[WT_WINNET_GROUP_B]",
                         __LINE__);

      shaping_group_swap_l2[WT_WINNET_GROUP_B] = WP_ShapingGroupCreate(
         shaping_group_swap_l1[WT_WINNET_GROUP_B],
         WP_SHAPING_GROUP_TYPE_PACKET,
         shaping_group_type_packet_l2_cfg);

      terminate_on_error(shaping_group_swap_l1[WT_WINNET_GROUP_B],
                         "WP_ShapingGroupCreate shaping_group_swap_l2[WT_WINNET_GROUP_B]",
                         __LINE__);
   }
} /* WTE_GroupsCreateGroupB */

void WTE_CreateChannelsGroupA(WP_U8 swap_mode)
{
   WP_S32 ii=0;

   /**********************************************************************/
   /* Since a interworking RX channel is not allowed to be created over  */
   /* a G999 device, in case of swapping this device into Enet, we will  */
   /* need to delete and recreate this channel in an interworking mode   */
   /**********************************************************************/
   if (swap_mode == WT_SWAP_MODE_G999)
   {
      WP_U8 max_streams = max_g999_streams_in_use[WT_WINNET_GROUP_A];
      WP_CHAR temp_string[256];

      /******************************************/
      /* Create RX channel over the g999 Device */
      /******************************************/
      ch_enet_cfg[0].iwmode = WP_PKTCH_IWM_DISABLE;
      rx_channel_g999_dev[WT_WINNET_GROUP_A] = WP_ChannelCreate(ETH_RX_PHYSICAL_DEV_CH_TAG,
                                                                group_a_g999_dev,
                                                                qniw_enet,
                                                                WP_CH_RX,
                                                                WP_ENET,
                                                                ch_enet_cfg);
      terminate_on_error(rx_channel_g999_dev[WT_WINNET_GROUP_A], "WP_ChannelCreate() rx_channel_g999_dev[WT_WINNET_GROUP_A]", __LINE__);

      ch_enet_cfg[0].iwmode = WP_PKTCH_IWM_ENABLE;

      for (ii=0 ; ii<max_streams ; ii++)
      {
         /* create 1 rx and tx channel per stream */
         tx_channel_stream[ii] = WP_ChannelCreate(ETH_TX_TAG_START + ii,
                                                  shaping_group_l2[ii],
                                                  qniw_enet,
                                                  WP_CH_TX,
                                                  WP_ENET,
                                                  ch_enet_cfg);
         sprintf (temp_string, "WP_ChannelCreate tx_channel_stream[%d]", ii);
         terminate_on_error(tx_channel_stream[ii], temp_string, __LINE__);

         rx_channel_stream[ii] = WP_ChannelCreate(ETH_RX_TAG_START + ii,
                                                  g999_stream_handle[ii],
                                                  qniw_enet,
                                                  WP_CH_RX,
                                                  WP_ENET,
                                                  ch_enet_cfg);
         sprintf (temp_string, "WP_ChannelCreate rx_channel_stream[%d]", ii);
         terminate_on_error(rx_channel_stream[ii], temp_string, __LINE__);
      }
   }
   else /* (swap_mode == WT_SWAP_MODE_ENET) */
   {
      /*****************************************************/
      /* Create Rx channel for the swapped Enet Rx Channel */
      /*****************************************************/
      ch_enet_cfg[0].iwmode = WP_PKTCH_IWM_ENABLE;
      rx_channel_enet_dev[WT_WINNET_GROUP_A] =
         WP_ChannelCreate(ETH_RX_PHYSICAL_DEV_SWAP_CH_TAG,
                          group_a_enet_dev,
                          qniw_enet,
                          WP_CH_RX,
                          WP_ENET,
                          ch_enet_cfg);
      terminate_on_error(rx_channel_enet_dev[WT_WINNET_GROUP_A], "WP_ChannelCreate() rx_channel_enet_dev[WT_WINNET_GROUP_A]", __LINE__);

      ch_enet_cfg[0].iwmode = WP_PKTCH_IWM_ENABLE;
      tx_channel_enet_dev[WT_WINNET_GROUP_A] =
         WP_ChannelCreate(ETH_TX_SWAP_TAG,
                          shaping_group_swap_l2[WT_WINNET_GROUP_A],
                          qniw_enet,
                          WP_CH_TX,
                          WP_ENET,
                          ch_enet_cfg);
      terminate_on_error(tx_channel_enet_dev[WT_WINNET_GROUP_A], "WP_ChannelCreate tx_channel_enet_dev[WT_WINNET_GROUP_A]", __LINE__);

   }
} /* WTE_CreateChannelsGroupA */

void WTE_CreateChannelsGroupB(WP_U8 swap_mode)
{
   /**********************************************************************/
   /* Since a interworking RX channel is not allowed to be created over  */
   /* a G999 device, in case of swapping this device into Enet, we will  */
   /* need to delete and recreate this channel in an interworking mode   */
   /**********************************************************************/
   if (swap_mode == WT_SWAP_MODE_G999)
   {
      WP_U8 max_streams = max_g999_streams_in_use[WT_WINNET_GROUP_B];
      WP_CHAR temp_string[256];
      WP_S32 ii, jj;
      /******************************************/
      /* Create RX channel over the g999 Device */
      /******************************************/
      ch_enet_cfg[0].iwmode = WP_PKTCH_IWM_DISABLE;
      rx_channel_g999_dev[WT_WINNET_GROUP_B] = WP_ChannelCreate(ETH_RX_PHYSICAL_DEV_CH_TAG,
                                                                group_b_g999_dev,
                                                                qniw_enet,
                                                                WP_CH_RX,
                                                                WP_ENET,
                                                                ch_enet_cfg);
      terminate_on_error(rx_channel_g999_dev[WT_WINNET_GROUP_B], "WP_ChannelCreate() rx_channel_g999_dev[WT_WINNET_GROUP_B]", __LINE__);

      ch_enet_cfg[0].iwmode = WP_PKTCH_IWM_ENABLE;

      for (ii=0 ; ii<max_streams ; ii++)
      {
         jj = ii+max_g999_streams_in_use[WT_WINNET_GROUP_A];
         /* create 1 rx and tx channel per stream */
         tx_channel_stream[jj] = WP_ChannelCreate(ETH_TX_TAG_START + jj,
                                                  shaping_group_l2[jj],
                                                  qniw_enet,
                                                  WP_CH_TX,
                                                  WP_ENET,
                                                  ch_enet_cfg);
         sprintf (temp_string, "WP_ChannelCreate tx_channel_stream[%d]", jj);
         terminate_on_error(tx_channel_stream[jj], temp_string, __LINE__);

         rx_channel_stream[jj] = WP_ChannelCreate(ETH_RX_TAG_START + jj,
                                                  g999_stream_handle[jj],
                                                  qniw_enet,
                                                  WP_CH_RX,
                                                  WP_ENET,
                                                  ch_enet_cfg);
         sprintf (temp_string, "WP_ChannelCreate rx_channel_stream[%d]", jj);
         terminate_on_error(rx_channel_stream[jj], temp_string, __LINE__);
      }
   }
   else /* (swap_mode == WT_SWAP_MODE_ENET) */
   {
      /*****************************************************/
      /* Create Rx channel for the swapped Enet Rx Channel */
      /*****************************************************/
      ch_enet_cfg[0].iwmode = WP_PKTCH_IWM_ENABLE;
      rx_channel_enet_dev[WT_WINNET_GROUP_B] =
         WP_ChannelCreate(ETH_RX_PHYSICAL_DEV_SWAP_CH_TAG,
                          group_b_enet_dev,
                          qniw_enet,
                          WP_CH_RX,
                          WP_ENET,
                          ch_enet_cfg);
      terminate_on_error(rx_channel_enet_dev[WT_WINNET_GROUP_B], "WP_ChannelCreate() rx_channel_enet_dev[WT_WINNET_GROUP_B]", __LINE__);

      ch_enet_cfg[0].iwmode = WP_PKTCH_IWM_ENABLE;
      tx_channel_enet_dev[WT_WINNET_GROUP_B] =
         WP_ChannelCreate(ETH_TX_SWAP_TAG,
                          shaping_group_swap_l2[WT_WINNET_GROUP_B],
                          qniw_enet,
                          WP_CH_TX,
                          WP_ENET,
                          ch_enet_cfg);
      terminate_on_error(tx_channel_enet_dev[WT_WINNET_GROUP_B], "WP_ChannelCreate tx_channel_enet_dev[WT_WINNET_GROUP_B]", __LINE__);

   }
} /* WTE_CreateChannelsGroupB */

void WTE_CreateHostFlowAggregationsAndIwPorts(void)
{
   WP_S32 ii = 0;

   /* Create the interworking system */
   iw_bridging_system_handle = WP_IwSystemCreate(WP_WINPATH(DEFAULT_WPID), WP_IW_VLAN_AWARE_BRIDGING_MODE, iw_sys_bridging_cfg);
   terminate_on_error(iw_bridging_system_handle, "WP_IwSystemCreate() iw_bridging_system_handle", __LINE__);

   /* Create host RX channel */
   ch_enet_cfg[0].iwmode = WP_PKTCH_IWM_ENABLE;
   iw_rx_ch = WP_ChannelCreate(IW_RX_CH_TAG,
                               host_dev,
                               qniw_host,
                               WP_CH_RX,
                               WP_IW_HOST,
                               ch_iw_rx_cfg);
   terminate_on_error(iw_rx_ch, "WP_ChannelCreate iw_rx_ch", __LINE__);

   /* WDDI bug in host queue flow aggregation, it does not allow to modify. */
   iw_agg_hq_cfg[0].txfunc = iw_rx_ch;
   iw_agg_hq_cfg[0].tag = IW_FLOW_AGG_TAG_HOST;
   flow_agg_host[0] = WP_IwFlowAggregationCreate(WP_WINPATH(DEFAULT_WPID),
                                              WP_IW_HOST_QUEUE_MODE,
                                              iw_agg_hq_cfg);
   terminate_on_error(flow_agg_host[0],"WP_IwFlowAggregationCreate() flow_agg_host[0]", __LINE__);

   /* Create a host bridge port and a host flow aggregation */
   bridge_port_cfg[0].flow_agg = flow_agg_host[0];
   bridge_port_cfg[0].tag = BRIDGE_PORT_HOST_TAG;
   bport_host = WP_IwPortCreate( iw_bridging_system_handle, bridge_port_cfg);
   terminate_on_error(bport_host, "WP_IwPortCreate bport_host", __LINE__);

   /* WDDI bug in host queue flow aggregation, it does not allow to modify. */
   iw_agg_hq_cfg[0].txfunc = iw_rx_ch;
   iw_agg_hq_cfg[0].tag = IW_FLOW_AGG_TAG_HOST + 1;
   iw_agg_hq_cfg[0].iw_port = bport_host;
   flow_agg_host[WT_WINNET_GROUP_A] =
      WP_IwFlowAggregationCreate(WP_WINPATH(DEFAULT_WPID),
                                 WP_IW_HOST_QUEUE_MODE,
                                 iw_agg_hq_cfg);
   terminate_on_error(flow_agg_host[WT_WINNET_GROUP_A],"WP_IwFlowAggregationCreate() flow_agg_host[WT_WINNET_GROUP_A] with IW-port", __LINE__);

   /* WDDI bug in host queue flow aggregation, it does not allow to modify. */
   iw_agg_hq_cfg[0].txfunc = iw_rx_ch;
   iw_agg_hq_cfg[0].tag = IW_FLOW_AGG_TAG_HOST + 2;
   iw_agg_hq_cfg[0].iw_port = bport_host;
   flow_agg_host[WT_WINNET_GROUP_B] =
      WP_IwFlowAggregationCreate(WP_WINPATH(DEFAULT_WPID),
                                 WP_IW_HOST_QUEUE_MODE,
                                 iw_agg_hq_cfg);
   terminate_on_error(flow_agg_host[WT_WINNET_GROUP_B],"WP_IwFlowAggregationCreate() flow_agg_host[WT_WINNET_GROUP_B] with IW-port", __LINE__);

   /* Create a host bridge port and
    * a host flow aggregation
    * for the regular Enet device */
   bridge_port_cfg[0].flow_agg = flow_agg_host[WT_WINNET_GROUP_A];
   bridge_port_cfg[0].tag = BRIDGE_PORT_TAG_START + ii;
   bport_iw_enet[WT_WINNET_GROUP_A] = WP_IwPortCreate( iw_bridging_system_handle, bridge_port_cfg);
   terminate_on_error(bport_iw_enet[WT_WINNET_GROUP_A], "WP_IwPortCreate bport_iw_enet[WT_WINNET_GROUP_A]", __LINE__);

   bridge_port_cfg[0].flow_agg = flow_agg_host[WT_WINNET_GROUP_B];
   bridge_port_cfg[0].tag = BRIDGE_PORT_TAG_START + ii;
   bport_iw_enet[WT_WINNET_GROUP_B] = WP_IwPortCreate( iw_bridging_system_handle, bridge_port_cfg);
   terminate_on_error(bport_iw_enet[WT_WINNET_GROUP_B], "WP_IwPortCreate bport_iw_enet[WT_WINNET_GROUP_B]", __LINE__);

} /* WTE_CreateHostFlowAggregationsAndIwPorts */

void WTE_CreateEnetRxChannelPceFiltersFilterSetsIntrefaces(void)
{
   WP_pce_filter_classification filter_class = {0};
   WP_pce_filter_set filterset = {0};
   WP_pce_filter_stat pce_stats;
   WP_CHAR temp_string[256];
   WP_S32 ii;
   WP_pce_if_params_pkt_rx_channel pce_if_params_pkt_rx_channel_cfg;
   WP_status status;

   memset (&filter_class, 0, sizeof (WP_pce_filter_classification));
   filter_class.tag = RX_CHANNEL_FILTER_TAG;
   filter_class.no_match_action = WP_PCE_FILTER_NO_MATCH_CONTINUE;
   filter_class.no_fields_action = WP_PCE_FILTER_NO_FIELDS_CONTINUE;
   filter_class.no_match_result[0].result_type = WP_PCE_RESULT_LAST;

   filter_class.filter_fields[0].field_id = WP_PCE_FIELD_ID_MAC_DA;
   filter_class.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_class.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;
   filter_class.filter_fields[1].field_id = WP_PCE_FIELD_ID_INPUT_IW_PORT;
   filter_class.filter_fields[1].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_class.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;
   filter_class.filter_fields[2].field_id = WP_PCE_FIELD_ID_LAST;

   pce_rx_channel_filter = WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_CLASSIFICATION, &filter_class);
   terminate_on_error(pce_rx_channel_filter, "WP_PceFilterCreate pce_rx_channel_filter", __LINE__);
   status = WP_PceFilterStatisticsReset (pce_rx_channel_filter, &pce_stats);
   terminate_on_error(status, "WP_PceFilterStatisticsReset pce_rx_channel_filter", __LINE__);

   /*************************/
   /* Create PCE Filter-Set */
   /*************************/
   memset (&filterset, 0, sizeof (WP_pce_filter_set));
   filterset.tag = RX_CHANNEL_FILTER_SET_TAG;
   filterset.filter_set_level = 2;
   filterset.filters[0] = pce_rx_channel_filter;
   filterset.filters[1] = WP_UNUSED;
   filterset.next_filter_set = WP_UNUSED;
   rx_channel_filterset_handle = WP_PceFilterSetCreate(WP_WINPATH(DEFAULT_WPID), &filterset);
   terminate_on_error(rx_channel_filterset_handle, "WP_PceFilterSetCreate", __LINE__);

   /*************************/
   /* Create PCE interfaces */
   /*************************/
   memset (&pce_if_params_pkt_rx_channel_cfg, 0, sizeof (WP_pce_if_params_pkt_rx_channel));
   pce_if_params_pkt_rx_channel_cfg.filter_set_handle = rx_channel_filterset_handle;
   pce_if_params_pkt_rx_channel_cfg.parser_start_type = WP_PCE_PARSER_START_TYPE_ETHERNET;
   pce_if_params_pkt_rx_channel_cfg.mode = WP_PCE_CONNECTION_ENABLED;
   pce_if_params_pkt_rx_channel_cfg.ip_header_validation = WP_DISABLE;

   /* PCE interfaces for the G999 device (for each stream) */
   for (ii=0 ; ii<MAX_G999_STREAMS ; ii++)
   {
      rx_channel_stream_pce_if_handle[ii] = WP_PceInterfaceCreate(WP_PCE_IF_TYPE_PKT_RX_CHANNEL, &pce_if_params_pkt_rx_channel_cfg);
      sprintf (temp_string, "WP_PceInterfaceCreate rx_channel_stream_pce_if_handle[%d]", ii);
      terminate_on_error(rx_channel_stream_pce_if_handle[ii], temp_string, __LINE__);
   }
   /* PCE interface for the swapped ENET device */
   rx_channel_enet_pce_if_handle = WP_PceInterfaceCreate(WP_PCE_IF_TYPE_PKT_RX_CHANNEL, &pce_if_params_pkt_rx_channel_cfg);
   terminate_on_error(rx_channel_enet_pce_if_handle, "WP_PceInterfaceCreate rx_channel_enet_pce_if_handle", __LINE__);
} /* WTE_CreateEnetRxChannelPceFiltersFilterSetsIntrefaces */

void WTE_CreateG999DevicePceFiltersFilterSetsIntrefaces(void)
{
   WP_pce_filter_classification filter_device = {0};
   WP_pce_filter_set filterset = {0};
   WP_pce_if_params_device pce_if_params_device_cfg = {0};
   WP_pce_filter_stat pce_stats;
   WP_status status;

   /*****************************************/
   /* PCE Device filter one for all streams */
   /*****************************************/
   memset (&filter_device, 0, sizeof (WP_pce_filter_classification));
   filter_device.tag = DEVICE_FILTER_TAG;
   filter_device.no_match_action = WP_PCE_FILTER_NO_MATCH_ACCEPT;
   filter_device.no_fields_action = WP_PCE_FILTER_NO_FIELDS_ACCEPT;

   filter_device.filter_fields[0].field_id = WP_PCE_FIELD_ID_DEVICE;
   filter_device.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_device.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;
   filter_device.filter_fields[1].field_id = WP_PCE_FIELD_ID_G999_STREAM_ID;
   filter_device.filter_fields[1].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_device.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_USED;
   filter_device.filter_fields[1].mask.g999_stream_id = 0x3FF;
   filter_device.filter_fields[2].field_id = WP_PCE_FIELD_ID_LAST;

   filter_device.no_match_result[0].result_type = WP_PCE_RESULT_LAST;

   pce_device_filter = WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_CLASSIFICATION, &filter_device);
   terminate_on_error(pce_device_filter, "WP_PceFilterCreate pce_device_filter", __LINE__);
   status = WP_PceFilterStatisticsReset (pce_device_filter, &pce_stats);
   terminate_on_error(status, "WP_PceFilterStatisticsReset pce_device_filter", __LINE__);

   memset (&filterset, 0, sizeof (WP_pce_filter_set));
   filterset.tag = FILTER_SET_DEVICE_TAG;
   filterset.filter_set_level = 1;
   filterset.filters[0] = pce_device_filter;
   filterset.filters[1] = WP_UNUSED;
   filterset.next_filter_set = WP_UNUSED;
   device_filterset_handle = WP_PceFilterSetCreate(WP_WINPATH(DEFAULT_WPID), &filterset);
   terminate_on_error(device_filterset_handle, "WP_PceFilterSetCreate device_filterset_handle", __LINE__);

   /********************************************************************************************* */
   /********************************************************************************************* */
   /********************************************************************************************* */

   memset (&pce_if_params_device_cfg, 0, sizeof (WP_pce_if_params_device));
   pce_if_params_device_cfg.mode = WP_PCE_CONNECTION_ENABLED;
   if (device_enet_g999_ex_cfg[0].enet_encap_mode == WP_DISABLE)
   {
      pce_if_params_device_cfg.parser_start_type = WP_PCE_PARSER_START_TYPE_G999_TCI_ONLY;
   }
   else
   {
      pce_if_params_device_cfg.parser_start_type = WP_PCE_PARSER_START_TYPE_G999_ENET_ENCAP;
   }
   pce_if_params_device_cfg.filter_set_handle = device_filterset_handle;
   device_pce_if_handle = WP_PceInterfaceCreate(WP_PCE_IF_TYPE_DEVICE, &pce_if_params_device_cfg);
   terminate_on_error(device_pce_if_handle, "WP_PceInterfaceCreate device_pce_if_handle", __LINE__);
}

void WTE_CreatePceRules(WP_U8 swap_mode,
                        WP_S32 ii,
                        WP_U8 group_num)
{
   WP_pce_rule_classification rule_cfg = {0};

   if (swap_mode == WT_SWAP_MODE_G999)
   {
      WP_CHAR temp_string[256];
      /**********************************************/
      /* PCE rules forward from STREAM to HOST      */
      /**********************************************/
      memset (&rule_cfg, 0, sizeof (WP_pce_rule_classification));
      rule_cfg.tag = RX_CHANNEL_RULES_HOST_TAG_START + ii;
      rule_cfg.filter_handle = pce_rx_channel_filter;
      rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_MAC_DA;
      rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_INPUT_IW_PORT;
      rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_LAST;
      rule_cfg.match_action = WP_PCE_RULE_MATCH_ACCEPT;
      rule_cfg.enabled = WP_ENABLE;
      rule_cfg.rule_fields[0].value.mac_addr[0] = 0x12;
      rule_cfg.rule_fields[0].value.mac_addr[1] = 0x34;
      rule_cfg.rule_fields[0].value.mac_addr[2] = 0x56;
      rule_cfg.rule_fields[0].value.mac_addr[3] = 0x78;
      rule_cfg.rule_fields[0].value.mac_addr[4] = 0x00;
      rule_cfg.rule_fields[0].value.mac_addr[5] = ii;
      rule_cfg.rule_fields[1].value.iw_port_handle = bport_iw_stream[ii];
      rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
      rule_cfg.match_result[0].param.flow_agg.flow_aggregation = flow_agg_host[group_num];
      rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;
      pce_rx_channel_rules_to_host[ii] = WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg);
      sprintf (temp_string, "WP_PceRuleCreate pce_rx_channel_rules_to_host[%d]", ii);
      terminate_on_error(pce_rx_channel_rules_to_host[ii], temp_string, __LINE__);

      /**********************************************/
      /* PCE rules forward from ENET to STREAM      */
      /*  Through Compatible Flow Aggregation       */
      /**********************************************/
      memset (&rule_cfg, 0, sizeof (WP_pce_rule_classification));
      rule_cfg.tag = RX_CHANNEL_RULES_ENET_COMPATIBLE_TAG_START + ii;
      rule_cfg.filter_handle = pce_rx_channel_filter;
      rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_MAC_DA;
      rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_INPUT_IW_PORT;
      rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_LAST;
      rule_cfg.match_action = WP_PCE_RULE_MATCH_ACCEPT;
      rule_cfg.enabled = WP_ENABLE;
      rule_cfg.rule_fields[0].value.mac_addr[0] = 0x12;
      rule_cfg.rule_fields[0].value.mac_addr[1] = 0x34;
      rule_cfg.rule_fields[0].value.mac_addr[2] = 0x56;
      rule_cfg.rule_fields[0].value.mac_addr[3] = 0x78;
      rule_cfg.rule_fields[0].value.mac_addr[4] = 0x01;
      rule_cfg.rule_fields[0].value.mac_addr[5] = ii;
      rule_cfg.rule_fields[1].value.iw_port_handle = bport_iw_enet[group_num];
      rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
      rule_cfg.match_result[0].param.flow_agg.flow_aggregation = flow_agg_compatible_stream[ii];
      rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;
      pce_rx_channel_rules_to_stream_from_generic[ii] = WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg);
      sprintf (temp_string, "WP_PceRuleCreate pce_rx_channel_rules_to_stream_from_generic[%d]", ii);
      terminate_on_error(pce_rx_channel_rules_to_stream_from_generic[ii], temp_string, __LINE__);
   }
   else /* (swap_mode == WT_SWAP_MODE_ENET) */
   {
      /***********************************************/
      /* PCE rules forward from swapped Enet to HOST */
      /***********************************************/
      memset (&rule_cfg, 0, sizeof (WP_pce_rule_classification));
      rule_cfg.tag = RX_CHANNEL_RULES_HOST_TAG_START;
      rule_cfg.filter_handle = pce_rx_channel_filter;
      rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_MAC_DA;
      rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_INPUT_IW_PORT;
      rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_LAST;
      rule_cfg.match_action = WP_PCE_RULE_MATCH_ACCEPT;
      rule_cfg.enabled = WP_ENABLE;
      rule_cfg.rule_fields[0].value.mac_addr[0] = 0x12;
      rule_cfg.rule_fields[0].value.mac_addr[1] = 0x34;
      rule_cfg.rule_fields[0].value.mac_addr[2] = 0x56;
      rule_cfg.rule_fields[0].value.mac_addr[3] = 0x78;
      rule_cfg.rule_fields[0].value.mac_addr[4] = 0x00;
      rule_cfg.rule_fields[0].value.mac_addr[5] = 0x00;
      rule_cfg.rule_fields[1].value.iw_port_handle = bport_iw_swapped_enet[group_num];
      rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
      rule_cfg.match_result[0].param.flow_agg.flow_aggregation = flow_agg_host[group_num];
      rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;
      pce_rx_channel_rules_enet_to_host[group_num] = WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg);
      terminate_on_error(pce_rx_channel_rules_enet_to_host[group_num], "WP_PceRuleCreate pce_rx_channel_rules_enet_to_host[group_num]", __LINE__);
      /****************************************************/
      /*     PCE rules forward from regular ENET to       */
      /* swapped ENET Through Compatible Flow Aggregation */
      /****************************************************/
      memset (&rule_cfg, 0, sizeof (WP_pce_rule_classification));
      rule_cfg.tag = RX_CHANNEL_RULES_ENET_COMPATIBLE_TAG_START;
      rule_cfg.filter_handle = pce_rx_channel_filter;
      rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_MAC_DA;
      rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_INPUT_IW_PORT;
      rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_LAST;
      rule_cfg.match_action = WP_PCE_RULE_MATCH_ACCEPT;
      rule_cfg.enabled = WP_ENABLE;
      rule_cfg.rule_fields[0].value.mac_addr[0] = 0x12;
      rule_cfg.rule_fields[0].value.mac_addr[1] = 0x34;
      rule_cfg.rule_fields[0].value.mac_addr[2] = 0x56;
      rule_cfg.rule_fields[0].value.mac_addr[3] = 0x78;
      rule_cfg.rule_fields[0].value.mac_addr[4] = 0x01;
      rule_cfg.rule_fields[0].value.mac_addr[5] = 0x00;
      rule_cfg.rule_fields[1].value.iw_port_handle = bport_iw_enet[group_num];
      rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
      rule_cfg.match_result[0].param.flow_agg.flow_aggregation = flow_agg_compatible_enet[group_num];
      rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;
      pce_rx_channel_rules_enet_to_swapped_enet[group_num][0] = WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg);
      terminate_on_error(pce_rx_channel_rules_enet_to_swapped_enet[group_num][0], "WP_PceRuleCreate pce_rx_channel_rules_enet_to_swapped_enet[group_num][0]", __LINE__);
   }
} /* WTE_CreatePceRules */

void WTE_CreateIwSystemAndItsComponentsGroupA(WP_U8 swap_mode)
{
   WP_status status;

   if (swap_mode == WT_SWAP_MODE_G999)
   {
      WP_CHAR temp_string[256];
      WP_U32 max_streams = max_g999_streams_in_use[WT_WINNET_GROUP_A];
      WP_U8 ii=0;
      for (ii=0 ; ii<max_streams ; ii++)
      {
         /**************************/
         /* Create host TX channel */
         /**************************/

         bridge_port_cfg[0].flow_agg = flow_agg_host[WT_WINNET_GROUP_A];
         bridge_port_cfg[0].tag = BRIDGE_PORT_TAG_START + ii;
         bport_iw_stream[ii] = WP_IwPortCreate( iw_bridging_system_handle, bridge_port_cfg);
         sprintf (temp_string, "WP_IwPortCreate bport_iw_stream[%d]", ii);
         terminate_on_error(bport_iw_stream[ii], temp_string, __LINE__);

         tx_agg_bridging[0].txfunc = tx_channel_stream[ii];
         tx_agg_bridging[0].iw_port = bport_iw_stream[ii];
         tx_agg_bridging[0].tag = IW_FLOW_AGG_TAG_COMPATIBLE+ii;
         flow_agg_compatible_stream[ii] = WP_IwFlowAggregationCreate(WP_WINPATH(DEFAULT_WPID),
                                                                     WP_IW_VLAN_AWARE_BRIDGING_MODE,
                                                                     tx_agg_bridging);
         sprintf (temp_string, "WP_IwFlowAggregationCreate flow_agg_compatible_stream[%d] bport_iw_stream[%d]", ii, ii);
         terminate_on_error(flow_agg_compatible_stream[ii], temp_string, __LINE__);

         /* Create the bridging ports */
         bridge_port_cfg[0].flow_agg = flow_agg_compatible_stream[ii];
         bridge_port_cfg[0].tag = BRIDGE_PORT_HOST_TAG+ii;
         bport_host_tx[ii] = WP_IwPortCreate( iw_bridging_system_handle, bridge_port_cfg);
         sprintf (temp_string, "WP_IwPortCreate bport_host_tx[%d]", ii);
         terminate_on_error(bport_host_tx[ii], temp_string, __LINE__);

         /* creating the IW host tx channel here because
          * the bport_host needs to be created first.   */
         ch_iw_tx_cfg[0].iw_system = iw_bridging_system_handle;
         ch_iw_tx_cfg[0].input_port = bport_host_tx[ii];
         iw_tx_ch[ii] = WP_ChannelCreate(IW_TX_CH_TAG_START +ii,
                                         host_dev,
                                         qniw_host,
                                         WP_CH_TX,
                                         WP_IW_HOST,
                                         ch_iw_tx_cfg);
         sprintf (temp_string, "WP_ChannelCreate to iw_tx_ch[%d]", ii);
         terminate_on_error(iw_tx_ch[ii], temp_string, __LINE__);

         WTE_CreatePceRules(swap_mode, ii, WT_WINNET_GROUP_A);

         /***********************************************/
         /* Create Rx binding to the Rx Channel Streams */
         /***********************************************/
         rx_binding_bridging_cfg[0].input_port = bport_iw_stream[ii];
         rx_binding_bridging_cfg[0].pce_if_handle = rx_channel_stream_pce_if_handle[ii];
         status = WP_IwRxBindingCreate(rx_channel_stream[ii],
                                       iw_bridging_system_handle,
                                       qniw_enet,
                                       rx_binding_bridging_cfg);
         sprintf (temp_string, "WP_IwRxBindingCreate rx_channel_stream[%d] to bport_iw_stream[%d]", ii, ii);
         terminate_on_error(status, temp_string, __LINE__);

         /*****************************************/
         /* Create Tx bindings to the Tx Channel 1*/
         /*****************************************/
         status = WP_IwTxBindingCreate(tx_channel_stream[ii],
                                       WP_IW_TX_BINDING,
                                       tx_binding_cfg);
         sprintf (temp_string, "WP_IwTxBindingCreate tx_channel_stream[%d]", ii);
         terminate_on_error(status, temp_string, __LINE__);
      }
   }
   else /* (swap_mode == WT_SWAP_MODE_ENET) */
   {
      /************************************/
      /* Enet Configuration               */
      /************************************/
      bridge_port_cfg[0].flow_agg = flow_agg_host[WT_WINNET_GROUP_A];
      bridge_port_cfg[0].tag = BRIDGE_PORT_TAG_START;
      bport_iw_swapped_enet[WT_WINNET_GROUP_A] = WP_IwPortCreate( iw_bridging_system_handle, bridge_port_cfg);
      terminate_on_error(bport_iw_swapped_enet[WT_WINNET_GROUP_A], "WP_IwPortCreate bport_iw_swapped_enet[WT_WINNET_GROUP_A]", __LINE__);

      tx_agg_bridging[0].txfunc = tx_channel_enet_dev[WT_WINNET_GROUP_A];
      tx_agg_bridging[0].iw_port = bport_iw_swapped_enet[WT_WINNET_GROUP_A];
      tx_agg_bridging[0].tag = IW_FLOW_AGG_TAG_COMPATIBLE;
      flow_agg_compatible_enet[WT_WINNET_GROUP_A] = WP_IwFlowAggregationCreate(WP_WINPATH(DEFAULT_WPID),
                                                                               WP_IW_VLAN_AWARE_BRIDGING_MODE,
                                                                               tx_agg_bridging);
      terminate_on_error(flow_agg_compatible_enet[WT_WINNET_GROUP_A], "WP_IwFlowAggregationCreate flow_agg_compatible_enet[WT_WINNET_GROUP_A] bport_iw_swapped_enet[WT_WINNET_GROUP_A]", __LINE__);

      /* Create a bridge port */
      bridge_port_cfg[0].flow_agg = flow_agg_compatible_enet[WT_WINNET_GROUP_A];
      bridge_port_cfg[0].tag = BRIDGE_PORT_HOST_TAG;
      bport_host_tx_swapped_enet[WT_WINNET_GROUP_A] = WP_IwPortCreate( iw_bridging_system_handle, bridge_port_cfg);
      terminate_on_error(bport_host_tx_swapped_enet[WT_WINNET_GROUP_A], "WP_IwPortCreate bport_host_tx_swapped_enet[WT_WINNET_GROUP_A]", __LINE__);

      /**************************/
      /* Create host TX channel */
      /**************************/
      /* creating the IW host tx channel here because
       * the bport_host needs to be created first.   */
      ch_iw_tx_cfg[0].iw_system = iw_bridging_system_handle;
      ch_iw_tx_cfg[0].input_port = bport_host_tx_swapped_enet[WT_WINNET_GROUP_A];
      iw_tx_ch_enet[WT_WINNET_GROUP_A] = WP_ChannelCreate(IW_TX_CH_TAG_START,
                                                          host_dev,
                                                          qniw_host,
                                                          WP_CH_TX,
                                                          WP_IW_HOST,
                                                          ch_iw_tx_cfg);
      terminate_on_error(iw_tx_ch_enet[WT_WINNET_GROUP_A], "WP_ChannelCreate to iw_tx_ch_enet[WT_WINNET_GROUP_A]", __LINE__);

      WTE_CreatePceRules(swap_mode, 0, WT_WINNET_GROUP_A);
      /*****************************************************/
      /* Create Rx binding for the swapped Enet Rx Channel */
      /*****************************************************/

      /* The RX channel over the swapped ENET device will be created only after
       * enabling it ("WP_DeviceEnable"), since this is an interworking channel
       * (an iw-channel cannot be created over a G999 device).
       * Only after the creation of this channel we could bind it */

      rx_binding_bridging_cfg[0].input_port = bport_iw_swapped_enet[WT_WINNET_GROUP_A];
      rx_binding_bridging_cfg[0].pce_if_handle = rx_channel_enet_pce_if_handle;
      status = WP_IwRxBindingCreate(rx_channel_enet_dev[WT_WINNET_GROUP_A],
                                    iw_bridging_system_handle,
                                    qniw_enet,
                                    rx_binding_bridging_cfg);
      terminate_on_error(status, "WP_IwRxBindingCreate rx_channel_enet_dev[WT_WINNET_GROUP_A] to bport_iw_swapped_enet[WT_WINNET_GROUP_A]", __LINE__);

      /*****************************************************/
      /* Create Tx binding for the swapped Enet Tx Channel */
      /*****************************************************/
      status = WP_IwTxBindingCreate(tx_channel_enet_dev[WT_WINNET_GROUP_A],
                                    WP_IW_TX_BINDING,
                                    tx_binding_cfg);
      terminate_on_error(status, "WP_IwTxBindingCreate tx_channel_enet_dev[WT_WINNET_GROUP_A]", __LINE__);
   }
} /* WTE_CreateIwSystemAndItsComponentsGroupA */

void WTE_CreateIwSystemAndItsComponentsGroupB(WP_U8 swap_mode)
{
   WP_status status;

   if (swap_mode == WT_SWAP_MODE_G999)
   {
      WP_CHAR temp_string[256];
      WP_U32 max_streams = max_g999_streams_in_use[WT_WINNET_GROUP_B];
      WP_S32 ii, jj;
      for (ii=0 ; ii<max_streams ; ii++)
      {
         jj = ii+max_g999_streams_in_use[WT_WINNET_GROUP_A];
         /**************************/
         /* Create host TX channel */
         /**************************/

         bridge_port_cfg[0].flow_agg = flow_agg_host[WT_WINNET_GROUP_A];
         bridge_port_cfg[0].tag = BRIDGE_PORT_TAG_START + jj;
         bport_iw_stream[jj] = WP_IwPortCreate( iw_bridging_system_handle, bridge_port_cfg);
         sprintf (temp_string, "WP_IwPortCreate bport_iw_stream[%d]", jj);
         terminate_on_error(bport_iw_stream[jj], temp_string, __LINE__);

         tx_agg_bridging[0].txfunc = tx_channel_stream[jj];
         tx_agg_bridging[0].iw_port = bport_iw_stream[jj];
         tx_agg_bridging[0].tag = IW_FLOW_AGG_TAG_COMPATIBLE+jj;
         flow_agg_compatible_stream[jj] = WP_IwFlowAggregationCreate(WP_WINPATH(DEFAULT_WPID),
                                                                     WP_IW_VLAN_AWARE_BRIDGING_MODE,
                                                                     tx_agg_bridging);
         sprintf (temp_string, "WP_IwFlowAggregationCreate flow_agg_compatible_stream[%d] bport_iw_stream[%d]", jj, jj);
         terminate_on_error(flow_agg_compatible_stream[jj], temp_string, __LINE__);

         /* Create the bridging ports */
         bridge_port_cfg[0].flow_agg = flow_agg_compatible_stream[jj];
         bridge_port_cfg[0].tag = BRIDGE_PORT_HOST_TAG+jj;
         bport_host_tx[jj] = WP_IwPortCreate( iw_bridging_system_handle, bridge_port_cfg);
         sprintf (temp_string, "WP_IwPortCreate bport_host_tx[%d]", jj);
         terminate_on_error(bport_host_tx[jj], temp_string, __LINE__);

         /* creating the IW host tx channel here because
          * the bport_host needs to be created first.   */
         ch_iw_tx_cfg[0].iw_system = iw_bridging_system_handle;
         ch_iw_tx_cfg[0].input_port = bport_host_tx[jj];
         iw_tx_ch[jj] = WP_ChannelCreate(IW_TX_CH_TAG_START +jj,
                                         host_dev,
                                         qniw_host,
                                         WP_CH_TX,
                                         WP_IW_HOST,
                                         ch_iw_tx_cfg);
         sprintf (temp_string, "WP_ChannelCreate to iw_tx_ch[%d]", jj);
         terminate_on_error(iw_tx_ch[jj], temp_string, __LINE__);

         WTE_CreatePceRules(swap_mode, jj, WT_WINNET_GROUP_B);

         /***********************************************/
         /* Create Rx binding to the Rx Channel Streams */
         /***********************************************/
         rx_binding_bridging_cfg[0].input_port = bport_iw_stream[jj];
         rx_binding_bridging_cfg[0].pce_if_handle = rx_channel_stream_pce_if_handle[jj];
         status = WP_IwRxBindingCreate(rx_channel_stream[jj],
                                       iw_bridging_system_handle,
                                       qniw_enet,
                                       rx_binding_bridging_cfg);
         sprintf (temp_string, "WP_IwRxBindingCreate rx_channel_stream[%d] to bport_iw_stream[%d]", jj, jj);
         terminate_on_error(status, temp_string, __LINE__);

         /*****************************************/
         /* Create Tx bindings to the Tx Channel 1*/
         /*****************************************/
         status = WP_IwTxBindingCreate(tx_channel_stream[jj],
                                       WP_IW_TX_BINDING,
                                       tx_binding_cfg);
         sprintf (temp_string, "WP_IwTxBindingCreate tx_channel_stream[%d]", jj);
         terminate_on_error(status, temp_string, __LINE__);
      }
   }
   else /* (swap_mode == WT_SWAP_MODE_ENET) */
   {
      /************************************/
      /* Enet Configuration               */
      /************************************/
      bridge_port_cfg[0].flow_agg = flow_agg_host[WT_WINNET_GROUP_B];
      bridge_port_cfg[0].tag = BRIDGE_PORT_TAG_START;
      bport_iw_swapped_enet[WT_WINNET_GROUP_B] = WP_IwPortCreate( iw_bridging_system_handle, bridge_port_cfg);
      terminate_on_error(bport_iw_swapped_enet[WT_WINNET_GROUP_B], "WP_IwPortCreate bport_iw_swapped_enet[WT_WINNET_GROUP_B]", __LINE__);

      tx_agg_bridging[0].txfunc = tx_channel_enet_dev[WT_WINNET_GROUP_B];
      tx_agg_bridging[0].iw_port = bport_iw_swapped_enet[WT_WINNET_GROUP_B];
      tx_agg_bridging[0].tag = IW_FLOW_AGG_TAG_COMPATIBLE;
      flow_agg_compatible_enet[WT_WINNET_GROUP_B] = WP_IwFlowAggregationCreate(WP_WINPATH(DEFAULT_WPID),
                                                                               WP_IW_VLAN_AWARE_BRIDGING_MODE,
                                                                               tx_agg_bridging);
      terminate_on_error(flow_agg_compatible_enet[WT_WINNET_GROUP_B], "WP_IwFlowAggregationCreate flow_agg_compatible_enet[WT_WINNET_GROUP_B] bport_iw_swapped_enet[WT_WINNET_GROUP_B]", __LINE__);

      /* Create a bridge port */
      bridge_port_cfg[0].flow_agg = flow_agg_compatible_enet[WT_WINNET_GROUP_B];
      bridge_port_cfg[0].tag = BRIDGE_PORT_HOST_TAG;
      bport_host_tx_swapped_enet[WT_WINNET_GROUP_B] = WP_IwPortCreate( iw_bridging_system_handle, bridge_port_cfg);
      terminate_on_error(bport_host_tx_swapped_enet[WT_WINNET_GROUP_B], "WP_IwPortCreate bport_host_tx_swapped_enet[WT_WINNET_GROUP_B]", __LINE__);

      /**************************/
      /* Create host TX channel */
      /**************************/
      /* creating the IW host tx channel here because
       * the bport_host needs to be created first.   */
      ch_iw_tx_cfg[0].iw_system = iw_bridging_system_handle;
      ch_iw_tx_cfg[0].input_port = bport_host_tx_swapped_enet[WT_WINNET_GROUP_B];
      iw_tx_ch_enet[WT_WINNET_GROUP_B] = WP_ChannelCreate(IW_TX_CH_TAG_START,
                                               host_dev,
                                               qniw_host,
                                               WP_CH_TX,
                                               WP_IW_HOST,
                                               ch_iw_tx_cfg);
      terminate_on_error(iw_tx_ch_enet[WT_WINNET_GROUP_B], "WP_ChannelCreate to iw_tx_ch_enet[WT_WINNET_GROUP_B]", __LINE__);

      WTE_CreatePceRules(swap_mode, 0, WT_WINNET_GROUP_B);
      /*****************************************************/
      /* Create Rx binding for the swapped Enet Rx Channel */
      /*****************************************************/

      /* The RX channel over the swapped ENET device will be created only after
       * enabling it ("WP_DeviceEnable"), since this is an interworking channel
       * (an iw-channel cannot be created over a G999 device).
       * Only after the creation of this channel we could bind it */

      rx_binding_bridging_cfg[0].input_port = bport_iw_swapped_enet[WT_WINNET_GROUP_B];
      rx_binding_bridging_cfg[0].pce_if_handle = rx_channel_enet_pce_if_handle;
      status = WP_IwRxBindingCreate(rx_channel_enet_dev[WT_WINNET_GROUP_B],
                                    iw_bridging_system_handle,
                                    qniw_enet,
                                    rx_binding_bridging_cfg);
      terminate_on_error(status, "WP_IwRxBindingCreate rx_channel_enet_dev[WT_WINNET_GROUP_B] to bport_iw_swapped_enet[WT_WINNET_GROUP_B]", __LINE__);

      /*****************************************************/
      /* Create Tx binding for the swapped Enet Tx Channel */
      /*****************************************************/
      status = WP_IwTxBindingCreate(tx_channel_enet_dev[WT_WINNET_GROUP_B],
                                    WP_IW_TX_BINDING,
                                    tx_binding_cfg);
      terminate_on_error(status, "WP_IwTxBindingCreate tx_channel_enet_dev[WT_WINNET_GROUP_B]", __LINE__);
   }
} /* WTE_CreateIwSystemAndItsComponentsGroupB */

void WTE_EnablePortsDevicesChannelsGroupA(WP_U8 swap_mode)
{
   WP_status status;

   /*******************************/
   /* Enable the G999 / Enet Port */
   /*******************************/
   status = WP_PortEnable(enetp1, WP_DIRECTION_DUPLEX);
   terminate_on_error(status, "WP_PortEnable 1 G999 port", __LINE__);

   /*******************************************/
   /* by now we've already chosen whether the */
   /* 2nd device will perform as G999 or Enet */
   /*******************************************/
   if (swap_mode == WT_SWAP_MODE_G999)
   {
      WP_CHAR temp_string[256];
      WP_U8 max_streams = max_g999_streams_in_use[WT_WINNET_GROUP_A];
      WP_U16 ii=0;
      /**************************/
      /* G999 components Enable */
      /**************************/
      for (ii=0 ; ii<max_streams ; ii++)
      {
         status = WP_G999StreamTxEnable(g999_stream_handle[ii]);
         sprintf (temp_string, "WP_G999StreamTxEnable g999_stream_handle[%d]", ii);
         terminate_on_error(status, temp_string, __LINE__);
      }

      for (ii=0 ; ii<max_streams ; ii++)
      {
         status = WP_ShapingGroupEnable(shaping_group_l1[ii]);
         sprintf (temp_string, "WP_ShapingGroupEnable shaping_group_l1[%d]", ii);
         terminate_on_error(status, temp_string, __LINE__);

         status = WP_ShapingGroupEnable(shaping_group_l2[ii]);
         sprintf (temp_string, "WP_ShapingGroupEnable shaping_group_l2[%d]", ii);
         terminate_on_error(status, temp_string, __LINE__);
      }

      for (ii=0 ; ii<max_streams ; ii++)
      {
         status = WP_ChannelEnable(rx_channel_stream[ii]);
         sprintf (temp_string, "WP_ChannelEnable rx_channel_stream[%d]", ii);
         terminate_on_error(status, temp_string, __LINE__);
      }

      status = WP_ChannelEnable(rx_channel_g999_dev[WT_WINNET_GROUP_A]);
      terminate_on_error(status, "WP_ChannelEnable rx_channel_g999_dev[WT_WINNET_GROUP_A]", __LINE__);

      for (ii=0 ; ii<max_streams ; ii++)
      {
         status = WP_ChannelEnable(tx_channel_stream[ii]);
         sprintf (temp_string, "WP_ChannelEnable tx_channel_stream[%d]", ii);
         terminate_on_error(status, temp_string, __LINE__);
      }

      for (ii=0 ; ii<max_streams ; ii++)
      {
         status = WP_ChannelEnable(iw_tx_ch[ii]);
         sprintf (temp_string, "WP_ChannelEnable iw_tx_ch[%d]", ii);
         terminate_on_error(status, temp_string, __LINE__);
      }
      status = WP_DeviceEnable(group_a_g999_dev, WP_DIRECTION_DUPLEX);
      terminate_on_error(status, "WP_DeviceEnable G999 device", __LINE__);
   }
   else /* (swap_mode == WT_SWAP_MODE_ENET) */
   {
      /*********************************/
      /* SWAPPED G999 device with ENET */
      /*********************************/

      /* Swapped device shaping groups */
      status = WP_ShapingGroupEnable(shaping_group_swap_l1[WT_WINNET_GROUP_A]);
      terminate_on_error(status, "WP_ShapingGroupEnable shaping_group_swap_l1[WT_WINNET_GROUP_A]", __LINE__);

      status = WP_ShapingGroupEnable(shaping_group_swap_l2[WT_WINNET_GROUP_A]);
      terminate_on_error(status, "WP_ShapingGroupEnable shaping_group_swap_l2[WT_WINNET_GROUP_A]", __LINE__);

      /* Swapped device channels */
      status = WP_ChannelEnable(iw_tx_ch_enet[WT_WINNET_GROUP_A]);
      terminate_on_error(status, "WP_ChannelEnable iw_tx_ch_enet[WT_WINNET_GROUP_A]", __LINE__);

      status = WP_ChannelEnable(tx_channel_enet_dev[WT_WINNET_GROUP_A]);
      terminate_on_error(status, "WP_ChannelEnable tx_channel_enet_dev[WT_WINNET_GROUP_A]", __LINE__);

      status = WP_ChannelEnable(rx_channel_enet_dev[WT_WINNET_GROUP_A]);
      terminate_on_error(status, "WP_ChannelEnable rx_channel_enet_dev[WT_WINNET_GROUP_A]", __LINE__);

      status = WP_DeviceEnable(group_a_enet_dev, WP_DIRECTION_DUPLEX);
      terminate_on_error(status, "WP_DeviceEnable SWAP G999 device with ENET", __LINE__);
   }

   status = WP_ChannelEnable(iw_rx_ch);
   terminate_on_error(status, "WP_ChannelEnable iw_rx_ch", __LINE__);

} /* WTE_EnablePortsDevicesChannelsGroupA */

void WTE_EnablePortsDevicesChannelsGroupB(WP_U8 swap_mode)
{
   WP_status status;

   /*******************************/
   /* Enable the G999 / Enet Port */
   /*******************************/
   status = WP_PortEnable(enetp2, WP_DIRECTION_DUPLEX);
   terminate_on_error(status, "WP_PortEnable 2 G999 port", __LINE__);

   /*******************************************/
   /* by now we've already chosen whether the */
   /* 2nd device will perform as G999 or Enet */
   /*******************************************/
   if (swap_mode == WT_SWAP_MODE_G999)
   {
      WP_CHAR temp_string[256];
      WP_U8 max_streams = max_g999_streams_in_use[WT_WINNET_GROUP_B];
      WP_S32 ii, jj;
      /**************************/
      /* G999 components Enable */
      /**************************/
      for (ii=0 ; ii<max_streams ; ii++)
      {
         jj = ii+max_g999_streams_in_use[WT_WINNET_GROUP_A];
         status = WP_G999StreamTxEnable(g999_stream_handle[jj]);
         sprintf (temp_string, "WP_G999StreamTxEnable g999_stream_handle[%d]", jj);
         terminate_on_error(status, temp_string, __LINE__);
      }

      for (ii=0 ; ii<max_streams ; ii++)
      {
         jj = ii+max_g999_streams_in_use[WT_WINNET_GROUP_A];
         status = WP_ShapingGroupEnable(shaping_group_l1[jj]);
         terminate_on_error(status, "WP_ShapingGroupEnable shaping_group_l1_stream1", __LINE__);

         status = WP_ShapingGroupEnable(shaping_group_l2[jj]);
         terminate_on_error(status, "WP_ShapingGroupEnable shaping_group_l2[jj]", __LINE__);
      }

      for (ii=0 ; ii<max_streams ; ii++)
      {
         jj = ii+max_g999_streams_in_use[WT_WINNET_GROUP_A];
         status = WP_ChannelEnable(rx_channel_stream[jj]);
         sprintf (temp_string, "WP_ChannelEnable rx_channel_stream[%d]", jj);
         terminate_on_error(status, temp_string, __LINE__);
      }

      status = WP_ChannelEnable(rx_channel_g999_dev[WT_WINNET_GROUP_B]);
      terminate_on_error(status, "WP_ChannelEnable rx_channel_g999_dev[WT_WINNET_GROUP_B]", __LINE__);

      for (ii=0 ; ii<max_streams ; ii++)
      {
         jj = ii+max_g999_streams_in_use[WT_WINNET_GROUP_A];
         status = WP_ChannelEnable(tx_channel_stream[jj]);
         sprintf (temp_string, "WP_ChannelEnable tx_channel_stream[%d]", jj);
         terminate_on_error(status, temp_string, __LINE__);
      }

      for (ii=0 ; ii<max_streams ; ii++)
      {
         jj = ii+max_g999_streams_in_use[WT_WINNET_GROUP_A];
         status = WP_ChannelEnable(iw_tx_ch[jj]);
         sprintf (temp_string, "WP_ChannelEnable iw_tx_ch[%d]", jj);
         terminate_on_error(status, temp_string, __LINE__);
      }
      status = WP_DeviceEnable(group_b_g999_dev, WP_DIRECTION_DUPLEX);
      terminate_on_error(status, "WP_DeviceEnable G999 device", __LINE__);
   }
   else /* (swap_mode == WT_SWAP_MODE_ENET) */
   {
      /*********************************/
      /* SWAPPED G999 device with ENET */
      /*********************************/

      /* Swapped device shaping groups */
      status = WP_ChannelEnable(iw_tx_ch_enet[WT_WINNET_GROUP_B]);
      terminate_on_error(status, "WP_ChannelEnable iw_tx_ch_enet[WT_WINNET_GROUP_B]", __LINE__);

      status = WP_ShapingGroupEnable(shaping_group_swap_l1[WT_WINNET_GROUP_B]);
      terminate_on_error(status, "WP_ShapingGroupEnable shaping_group_swap_l1[WT_WINNET_GROUP_B]", __LINE__);

      status = WP_ShapingGroupEnable(shaping_group_swap_l2[WT_WINNET_GROUP_B]);
      terminate_on_error(status, "WP_ShapingGroupEnable shaping_group_swap_l2[WT_WINNET_GROUP_B]", __LINE__);

      /* Swapped device channels */
      status = WP_ChannelEnable(tx_channel_enet_dev[WT_WINNET_GROUP_B]);
      terminate_on_error(status, "WP_ChannelEnable tx_channel_enet_dev[WT_WINNET_GROUP_B]", __LINE__);

      status = WP_ChannelEnable(rx_channel_enet_dev[WT_WINNET_GROUP_B]);
      terminate_on_error(status, "WP_ChannelEnable rx_channel_enet_dev[WT_WINNET_GROUP_B]", __LINE__);

      status = WP_DeviceEnable(group_b_enet_dev, WP_DIRECTION_DUPLEX);
      terminate_on_error(status, "WP_DeviceEnable SWAP G999 device with ENET", __LINE__);
   }

   status = WP_ChannelEnable(iw_rx_ch);
   terminate_on_error(status, "WP_ChannelEnable iw_rx_ch", __LINE__);

} /* WTE_EnablePortsDevicesChannelsGroupB */

void WTE_SystemDeleteGroupA(WP_U8 swap_mode)
{
   WP_status status;

   if (swap_mode == WT_SWAP_MODE_G999)
   {
      WP_S32 ii;
      WP_CHAR temp_string[256];
      WP_U32 max_streams = max_g999_streams_in_use[WT_WINNET_GROUP_A];
      status = WP_ChannelDisable(iw_rx_ch);
      terminate_on_error(status, "WP_ChannelDisable iw_rx_ch", __LINE__);
      status = WP_ChannelDisable(rx_channel_g999_dev[WT_WINNET_GROUP_A]);
      terminate_on_error(status, "WP_ChannelDisable rx_channel_g999_dev[WT_WINNET_GROUP_A]", __LINE__);
      for (ii=0 ; ii<max_streams ; ii++)
      {
         status = WP_ChannelDisable(iw_tx_ch[ii]);
         sprintf (temp_string, "WP_ChannelDisable iw_tx_ch[%d]", ii);
         terminate_on_error(status, temp_string, __LINE__);

         status = WP_ChannelDisable(tx_channel_stream[ii]);
         sprintf (temp_string, "WP_ChannelDisable tx_channel_stream[%d]", ii);
         terminate_on_error(status, temp_string, __LINE__);

         rx_binding_bridging_cfg[0].input_port = 0;
         rx_binding_bridging_cfg[0].pce_if_handle = 0;
         status = WP_IwRxBindingModify(rx_channel_stream[ii],
                                       iw_bridging_system_handle,
                                       qniw_enet,
                                       WP_IW_RX_BIND_MOD_DISABLE,
                                       rx_binding_bridging_cfg);
         sprintf (temp_string, "WP_IwRxBindingModify rx_channel_stream[%d] WP_IW_RX_BIND_MOD_DISABLE", ii);
         terminate_on_error(status, temp_string, __LINE__);

         status = WP_ChannelDisable(rx_channel_stream[ii]);
         sprintf (temp_string, "WP_ChannelDisable rx_channel_stream[%d]", ii);
         terminate_on_error(status, temp_string, __LINE__);
      }

      status = WP_ChannelDelete(rx_channel_g999_dev[WT_WINNET_GROUP_A]);
      terminate_on_error(status, "WP_ChannelDelete rx_channel_g999_dev[WT_WINNET_GROUP_A]", __LINE__);
      rx_channel_g999_dev[WT_WINNET_GROUP_A] = 0;
      for (ii=0 ; ii<max_streams ; ii++)
      {
         status = WP_ChannelDelete(iw_tx_ch[ii]);
         sprintf (temp_string, "WP_ChannelDelete iw_tx_ch[%d]", ii);
         terminate_on_error(status, temp_string, __LINE__);
         iw_tx_ch[ii] = 0;

         status = WP_ChannelDelete(tx_channel_stream[ii]);
         sprintf (temp_string, "WP_ChannelDelete tx_channel_stream[%d]", ii);
         terminate_on_error(status, temp_string, __LINE__);
         tx_channel_stream[ii] = 0;

         status = WP_ChannelDelete(rx_channel_stream[ii]);
         sprintf (temp_string, "WP_ChannelDelete rx_channel_stream[%d]", ii);
         terminate_on_error(status, temp_string, __LINE__);
         rx_channel_stream[ii] = 0;
      }

      for (ii=0 ; ii<max_streams ; ii++)
      {
         status = WP_ShapingGroupDisable(shaping_group_l2[ii]);
         sprintf (temp_string, "WP_ShapingGroupDisable shaping_group_l2[%d]", ii);
         terminate_on_error(status, temp_string, __LINE__);

         status = WP_ShapingGroupDelete(shaping_group_l2[ii]);
         sprintf (temp_string, "WP_ShapingGroupDelete shaping_group_l2[%d]", ii);
         terminate_on_error(status, temp_string, __LINE__);
         shaping_group_l2[ii] = 0;
      }
      for (ii=0 ; ii<max_streams ; ii++)
      {
         status = WP_ShapingGroupDisable(shaping_group_l1[ii]);
         sprintf (temp_string, "WP_ShapingGroupDisable shaping_group_l1[%d]", ii);
         terminate_on_error(status, temp_string, __LINE__);

         status = WP_ShapingGroupDelete(shaping_group_l1[ii]);
         sprintf (temp_string, "WP_ShapingGroupDelete shaping_group_l1[%d]", ii);
         terminate_on_error(status, temp_string, __LINE__);
         shaping_group_l1[ii] = 0;
      }
      for (ii=0 ; ii<max_streams ; ii++)
      {
         status = WP_G999StreamTxDisable(g999_stream_handle[ii]);
         sprintf (temp_string, "WP_G999StreamTxDisable g999_stream_handle[%d]", ii);
         terminate_on_error(status, temp_string, __LINE__);

         status = WP_G999StreamDelete(g999_stream_handle[ii]);
         sprintf (temp_string, "WP_G999StreamDelete g999_stream_handle[%d]", ii);
         terminate_on_error(status, temp_string, __LINE__);
         g999_stream_handle[ii] = 0;
      }
      status = WP_DeviceDisable(group_a_g999_dev, WP_DIRECTION_DUPLEX);
      terminate_on_error(status, "WP_DeviceDisable G999 device", __LINE__);
      status = WP_PortDisable(enetp1, WP_DIRECTION_DUPLEX);
      terminate_on_error(status, "WP_PortDisable 1 G999 port", __LINE__);
      /********************************/
      /* Disable and Delete PCE Rules */
      /********************************/
      for (ii=0 ; ii<max_streams ; ii++)
      {
         status = WP_PceRuleDisable(pce_rx_channel_rules_to_stream_from_generic[ii]);
         sprintf (temp_string, "WP_PceRuleDisable pce_rx_channel_rules_to_stream_from_generic[%d]", ii);
         terminate_on_error(status, temp_string, __LINE__);

         status = WP_PceRuleDisable(pce_rx_channel_rules_to_host[ii]);
         sprintf (temp_string, "WP_PceRuleDisable pce_rx_channel_rules_to_host[%d]", ii);
         terminate_on_error(status, temp_string, __LINE__);

         status = WP_PceRuleDelete(pce_rx_channel_rules_to_stream_from_generic[ii]);
         sprintf (temp_string, "WP_PceRuleDelete pce_rx_channel_rules_to_stream_from_generic[%d]", ii);
         terminate_on_error(status, temp_string, __LINE__);
         pce_rx_channel_rules_to_stream_from_generic[ii] = 0;

         status = WP_PceRuleDelete(pce_rx_channel_rules_to_host[ii]);
         sprintf (temp_string, "WP_PceRuleDelete pce_rx_channel_rules_to_host[%d]", ii);
         terminate_on_error(status, temp_string, __LINE__);
         pce_rx_channel_rules_to_host[ii] = 0;

         status = WP_PceRuleDelete(pce_device_rules[ii]);
         sprintf (temp_string, "WP_PceRuleDelete pce_device_rules[%d]", ii);
         terminate_on_error(status, temp_string, __LINE__);
         pce_device_rules[ii] = 0;
      }

      /****************************/
      /* Delete IW-Ports          */
      /****************************/
      for (ii=0 ; ii<max_streams ; ii++)
      {
         status = WP_IwPortDelete(bport_host_tx[ii]);
         sprintf (temp_string, "WP_IwPortDelete bport_host_tx[%d]", ii);
         terminate_on_error(status, temp_string, __LINE__);
         bport_host_tx[ii] = 0;

         status = WP_IwPortDelete(bport_iw_stream[ii]);
         sprintf (temp_string, "WP_IwPortDelete bport_iw_stream[%d]", ii);
         terminate_on_error(status, temp_string, __LINE__);
         bport_iw_stream[ii] = 0;
      }

      /****************************/
      /* Delete Flow Aggregations */
      /****************************/
      for (ii=0 ; ii<max_streams ; ii++)
      {
         status = WP_IwFlowAggregationDelete(flow_agg_compatible_stream[ii]);
         sprintf (temp_string, "WP_IwFlowAggregationDelete flow_agg_compatible_stream[%d]", ii);
         terminate_on_error(status, temp_string, __LINE__);
         flow_agg_compatible_stream[ii] = 0;
      }
   }
   else /* (swap_mode == WT_SWAP_MODE_ENET) */
   {
      status = WP_ChannelDisable(iw_rx_ch);
      terminate_on_error(status, "WP_ChannelDisable iw_rx_ch", __LINE__);

      status = WP_ChannelDisable(iw_tx_ch_enet[WT_WINNET_GROUP_A]);
      terminate_on_error(status, "WP_ChannelDisable iw_tx_ch_enet[WT_WINNET_GROUP_A]", __LINE__);

      status = WP_ChannelDisable(rx_channel_enet_dev[WT_WINNET_GROUP_A]);
      terminate_on_error(status, "WP_ChannelDisable rx_channel_enet_dev[WT_WINNET_GROUP_A]", __LINE__);

      status = WP_ChannelDisable(tx_channel_enet_dev[WT_WINNET_GROUP_A]);
      terminate_on_error(status, "WP_ChannelDisable tx_channel_enet_dev[WT_WINNET_GROUP_A]", __LINE__);

      rx_binding_bridging_cfg[0].input_port = 0;
      rx_binding_bridging_cfg[0].pce_if_handle = 0;
      status = WP_IwRxBindingModify(rx_channel_enet_dev[WT_WINNET_GROUP_A],
                                    iw_bridging_system_handle,
                                    qniw_enet,
                                    WP_IW_RX_BIND_MOD_DISABLE,
                                    rx_binding_bridging_cfg);
      terminate_on_error(status, "WP_IwRxBindingModify rx_channel_stream[%d] WP_IW_RX_BIND_MOD_DISABLE", __LINE__);

      status = WP_ChannelDelete(iw_tx_ch_enet[WT_WINNET_GROUP_A]);
      terminate_on_error(status, "WP_ChannelDelete iw_tx_ch_enet[WT_WINNET_GROUP_A]", __LINE__);
      iw_tx_ch_enet[WT_WINNET_GROUP_A] = 0;

      status = WP_ChannelDelete(rx_channel_enet_dev[WT_WINNET_GROUP_A]);
      terminate_on_error(status, "WP_ChannelDelete rx_channel_enet_dev[WT_WINNET_GROUP_A]", __LINE__);
      rx_channel_enet_dev[WT_WINNET_GROUP_A] = 0;

      status = WP_ChannelDelete(tx_channel_enet_dev[WT_WINNET_GROUP_A]);
      terminate_on_error(status, "WP_ChannelDelete tx_channel_enet_dev[WT_WINNET_GROUP_A]", __LINE__);
      tx_channel_enet_dev[WT_WINNET_GROUP_A] = 0;

      status = WP_ShapingGroupDisable(shaping_group_swap_l2[WT_WINNET_GROUP_A]);
      terminate_on_error(status, "WP_ShapingGroupDisable shaping_group_swap_l2[WT_WINNET_GROUP_A]", __LINE__);

      status = WP_ShapingGroupDelete(shaping_group_swap_l2[WT_WINNET_GROUP_A]);
      terminate_on_error(status, "WP_ShapingGroupDelete shaping_group_swap_l2[WT_WINNET_GROUP_A]", __LINE__);
      shaping_group_swap_l2[WT_WINNET_GROUP_A] = 0;

      status = WP_ShapingGroupDisable(shaping_group_swap_l1[WT_WINNET_GROUP_A]);
      terminate_on_error(status, "WP_ShapingGroupDisable shaping_group_swap_l1[WT_WINNET_GROUP_A]", __LINE__);

      status = WP_ShapingGroupDelete(shaping_group_swap_l1[WT_WINNET_GROUP_A]);
      terminate_on_error(status, "WP_ShapingGroupDelete shaping_group_swap_l1[WT_WINNET_GROUP_A]", __LINE__);
      shaping_group_swap_l1[WT_WINNET_GROUP_A] = 0;

      status = WP_DeviceDisable(group_a_enet_dev, WP_DIRECTION_DUPLEX);
      terminate_on_error(status, "WP_DeviceEnable group_a_enet_dev", __LINE__);

      status = WP_PortDisable(enetp1, WP_DIRECTION_DUPLEX);
      terminate_on_error(status, "WP_PortDisable 1 G999 port", __LINE__);

      /********************************/
      /* Disable and Delete PCE Rules */
      /********************************/
      status = WP_PceRuleDisable(pce_rx_channel_rules_enet_to_host[WT_WINNET_GROUP_A]);
      terminate_on_error(status, "WP_PceRuleDisable pce_rx_channel_rules_enet_to_host[WT_WINNET_GROUP_A]", __LINE__);

      status = WP_PceRuleDisable(pce_rx_channel_rules_enet_to_swapped_enet[WT_WINNET_GROUP_A][0]);
      terminate_on_error(status, "WP_PceRuleDisable pce_rx_channel_rules_enet_to_swapped_enet[WT_WINNET_GROUP_A][0]", __LINE__);

      status = WP_PceRuleDelete(pce_rx_channel_rules_enet_to_host[WT_WINNET_GROUP_A]);
      terminate_on_error(status, "WP_PceRuleDelete pce_rx_channel_rules_enet_to_host[WT_WINNET_GROUP_A]", __LINE__);
      pce_rx_channel_rules_enet_to_host[WT_WINNET_GROUP_A] = 0;

      status = WP_PceRuleDelete(pce_rx_channel_rules_enet_to_swapped_enet[WT_WINNET_GROUP_A][0]);
      terminate_on_error(status, "WP_PceRuleDelete pce_rx_channel_rules_enet_to_swapped_enet[WT_WINNET_GROUP_A][0]", __LINE__);
      pce_rx_channel_rules_enet_to_swapped_enet[WT_WINNET_GROUP_A][0] = 0;

      /****************************/
      /* Delete IW-Ports          */
      /****************************/
      status = WP_IwPortDelete(bport_host_tx_swapped_enet[WT_WINNET_GROUP_A]);
      terminate_on_error(status, "WP_IwPortDelete bport_host_tx_swapped_enet[WT_WINNET_GROUP_A]", __LINE__);
      bport_host_tx_swapped_enet[WT_WINNET_GROUP_A] = 0;

      status = WP_IwPortDelete(bport_iw_swapped_enet[WT_WINNET_GROUP_A]);
      terminate_on_error(status, "WP_IwPortDelete bport_iw_swapped_enet[WT_WINNET_GROUP_A]", __LINE__);
      bport_iw_swapped_enet[WT_WINNET_GROUP_A] = 0;

      /****************************/
      /* Delete Flow Aggregation  */
      /****************************/
      status = WP_IwFlowAggregationDelete(flow_agg_compatible_enet[WT_WINNET_GROUP_A]);
      terminate_on_error(status, "WP_IwFlowAggregationDelete flow_agg_compatible_enet[WT_WINNET_GROUP_A]", __LINE__);
      flow_agg_compatible_enet[WT_WINNET_GROUP_A] = 0;
   }
} /* WTE_SystemDeleteGroupA */

void WTE_SystemDeleteGroupB(WP_U8 swap_mode)
{
   WP_status status;

   if (swap_mode == WT_SWAP_MODE_G999)
   {
      WP_S32 ii, jj;
      WP_CHAR temp_string[256];
      WP_U32 max_streams = max_g999_streams_in_use[WT_WINNET_GROUP_B];
      status = WP_ChannelDisable(iw_rx_ch);
      terminate_on_error(status, "WP_ChannelDisable iw_rx_ch", __LINE__);
      status = WP_ChannelDisable(rx_channel_g999_dev[WT_WINNET_GROUP_B]);
      terminate_on_error(status, "WP_ChannelDisable rx_channel_g999_dev[WT_WINNET_GROUP_B]", __LINE__);
      for (ii=0 ; ii<max_streams ; ii++)
      {
         jj = ii+max_g999_streams_in_use[WT_WINNET_GROUP_A];
         status = WP_ChannelDisable(iw_tx_ch[jj]);
         sprintf (temp_string, "WP_ChannelDisable iw_tx_ch[%d]", jj);
         terminate_on_error(status, temp_string, __LINE__);

         status = WP_ChannelDisable(tx_channel_stream[jj]);
         sprintf (temp_string, "WP_ChannelDisable tx_channel_stream[%d]", jj);
         terminate_on_error(status, temp_string, __LINE__);

         rx_binding_bridging_cfg[0].input_port = 0;
         rx_binding_bridging_cfg[0].pce_if_handle = 0;
         status = WP_IwRxBindingModify(rx_channel_stream[jj],
                                       iw_bridging_system_handle,
                                       qniw_enet,
                                       WP_IW_RX_BIND_MOD_DISABLE,
                                       rx_binding_bridging_cfg);
         sprintf (temp_string, "WP_IwRxBindingModify rx_channel_stream[%d] WP_IW_RX_BIND_MOD_DISABLE", jj);
         terminate_on_error(status, temp_string, __LINE__);

         status = WP_ChannelDisable(rx_channel_stream[jj]);
         sprintf (temp_string, "WP_ChannelDisable rx_channel_stream[%d]", jj);
         terminate_on_error(status, temp_string, __LINE__);
      }

      status = WP_ChannelDelete(rx_channel_g999_dev[WT_WINNET_GROUP_B]);
      terminate_on_error(status, "WP_ChannelDelete rx_channel_g999_dev[WT_WINNET_GROUP_B]", __LINE__);
      rx_channel_g999_dev[WT_WINNET_GROUP_B] = 0;
      for (ii=0 ; ii<max_streams ; ii++)
      {
         jj = ii+max_g999_streams_in_use[WT_WINNET_GROUP_A];
         status = WP_ChannelDelete(iw_tx_ch[jj]);
         sprintf (temp_string, "WP_ChannelDelete iw_tx_ch[%d]", jj);
         terminate_on_error(status, temp_string, __LINE__);
         iw_tx_ch[jj] = 0;

         status = WP_ChannelDelete(tx_channel_stream[jj]);
         sprintf (temp_string, "WP_ChannelDelete tx_channel_stream[%d]", jj);
         terminate_on_error(status, temp_string, __LINE__);
         tx_channel_stream[jj] = 0;

         status = WP_ChannelDelete(rx_channel_stream[jj]);
         sprintf (temp_string, "WP_ChannelDelete rx_channel_stream[%d]", jj);
         terminate_on_error(status, temp_string, __LINE__);
         rx_channel_stream[jj] = 0;
      }

      for (ii=0 ; ii<max_streams ; ii++)
      {
         jj = ii+max_g999_streams_in_use[WT_WINNET_GROUP_A];
         status = WP_ShapingGroupDisable(shaping_group_l2[jj]);
         sprintf (temp_string, "WP_ShapingGroupDisable shaping_group_l2[%d]", jj);
         terminate_on_error(status, temp_string, __LINE__);

         status = WP_ShapingGroupDelete(shaping_group_l2[jj]);
         sprintf (temp_string, "WP_ShapingGroupDelete shaping_group_l2[%d]", jj);
         terminate_on_error(status, temp_string, __LINE__);
         shaping_group_l2[jj] = 0;
      }
      for (ii=0 ; ii<max_streams ; ii++)
      {
         jj = ii+max_g999_streams_in_use[WT_WINNET_GROUP_A];
         status = WP_ShapingGroupDisable(shaping_group_l1[jj]);
         sprintf (temp_string, "WP_ShapingGroupDisable shaping_group_l1[%d]", jj);
         terminate_on_error(status, temp_string, __LINE__);

         status = WP_ShapingGroupDelete(shaping_group_l1[jj]);
         sprintf (temp_string, "WP_ShapingGroupDelete shaping_group_l1[%d]", jj);
         terminate_on_error(status, temp_string, __LINE__);
         shaping_group_l1[jj] = 0;
      }
      for (ii=0 ; ii<max_streams ; ii++)
      {
         jj = ii+max_g999_streams_in_use[WT_WINNET_GROUP_A];
         status = WP_G999StreamTxDisable(g999_stream_handle[jj]);
         sprintf (temp_string, "WP_G999StreamTxDisable g999_stream_handle[%d]", jj);
         terminate_on_error(status, temp_string, __LINE__);

         status = WP_G999StreamDelete(g999_stream_handle[jj]);
         sprintf (temp_string, "WP_G999StreamDelete g999_stream_handle[%d]", jj);
         terminate_on_error(status, temp_string, __LINE__);
         g999_stream_handle[jj] = 0;
      }
      status = WP_DeviceDisable(group_b_g999_dev, WP_DIRECTION_DUPLEX);
      terminate_on_error(status, "WP_DeviceEnable G999 device", __LINE__);
      status = WP_PortDisable(enetp1, WP_DIRECTION_DUPLEX);
      terminate_on_error(status, "WP_PortDisable 1 G999 port", __LINE__);
      /********************************/
      /* Disable and Delete PCE Rules */
      /********************************/
      for (ii=0 ; ii<max_streams ; ii++)
      {
         jj = ii+max_g999_streams_in_use[WT_WINNET_GROUP_A];
         status = WP_PceRuleDisable(pce_rx_channel_rules_to_stream_from_generic[jj]);
         sprintf (temp_string, "WP_PceRuleDisable pce_rx_channel_rules_to_stream_from_generic[%d]", jj);
         terminate_on_error(status, temp_string, __LINE__);

         status = WP_PceRuleDisable(pce_rx_channel_rules_to_host[jj]);
         sprintf (temp_string, "WP_PceRuleDisable pce_rx_channel_rules_to_host[%d]", jj);
         terminate_on_error(status, temp_string, __LINE__);

         status = WP_PceRuleDelete(pce_rx_channel_rules_to_stream_from_generic[jj]);
         sprintf (temp_string, "WP_PceRuleDelete pce_rx_channel_rules_to_stream_from_generic[%d]", jj);
         terminate_on_error(status, temp_string, __LINE__);
         pce_rx_channel_rules_to_stream_from_generic[jj] = 0;

         status = WP_PceRuleDelete(pce_rx_channel_rules_to_host[jj]);
         sprintf (temp_string, "WP_PceRuleDelete pce_rx_channel_rules_to_host[%d]", jj);
         terminate_on_error(status, temp_string, __LINE__);
         pce_rx_channel_rules_to_host[jj] = 0;

         status = WP_PceRuleDelete(pce_device_rules[jj]);
         sprintf (temp_string, "WP_PceRuleDelete pce_device_rules[%d]", jj);
         terminate_on_error(status, temp_string, __LINE__);
         pce_device_rules[jj] = 0;
      }

      /****************************/
      /* Delete IW-Ports          */
      /****************************/
      for (ii=0 ; ii<max_streams ; ii++)
      {
         jj = ii+max_g999_streams_in_use[WT_WINNET_GROUP_A];
         status = WP_IwPortDelete(bport_host_tx[jj]);
         sprintf (temp_string, "WP_IwPortDelete bport_host_tx[%d]", jj);
         terminate_on_error(status, temp_string, __LINE__);
         bport_host_tx[jj] = 0;

         status = WP_IwPortDelete(bport_iw_stream[jj]);
         sprintf (temp_string, "WP_IwPortDelete bport_iw_stream[%d]", jj);
         terminate_on_error(status, temp_string, __LINE__);
         bport_iw_stream[jj] = 0;

      }

      /****************************/
      /* Delete Flow Aggregations */
      /****************************/
      for (ii=0 ; ii<max_streams ; ii++)
      {
         jj = ii+max_g999_streams_in_use[WT_WINNET_GROUP_A];

         status = WP_IwFlowAggregationDelete(flow_agg_compatible_stream[jj]);
         sprintf (temp_string, "WP_IwFlowAggregationDelete flow_agg_compatible_stream[%d]", jj);
         terminate_on_error(status, temp_string, __LINE__);
         flow_agg_compatible_stream[jj] = 0;
      }
   }
   else /* (swap_mode == WT_SWAP_MODE_ENET) */
   {
      status = WP_ChannelDisable(iw_rx_ch);
      terminate_on_error(status, "WP_ChannelDisable iw_rx_ch", __LINE__);

      status = WP_ChannelDisable(iw_tx_ch_enet[WT_WINNET_GROUP_B]);
      terminate_on_error(status, "WP_ChannelDisable iw_tx_ch_enet[WT_WINNET_GROUP_B]", __LINE__);

      status = WP_ChannelDisable(rx_channel_enet_dev[WT_WINNET_GROUP_B]);
      terminate_on_error(status, "WP_ChannelDisable rx_channel_enet_dev[WT_WINNET_GROUP_B]", __LINE__);

      status = WP_ChannelDisable(tx_channel_enet_dev[WT_WINNET_GROUP_B]);
      terminate_on_error(status, "WP_ChannelDisable tx_channel_enet_dev[WT_WINNET_GROUP_B]", __LINE__);

      rx_binding_bridging_cfg[0].input_port = 0;
      rx_binding_bridging_cfg[0].pce_if_handle = 0;
      status = WP_IwRxBindingModify(rx_channel_enet_dev[WT_WINNET_GROUP_B],
                                    iw_bridging_system_handle,
                                    qniw_enet,
                                    WP_IW_RX_BIND_MOD_DISABLE,
                                    rx_binding_bridging_cfg);
      terminate_on_error(status, "WP_IwRxBindingModify rx_channel_stream[%d] WP_IW_RX_BIND_MOD_DISABLE", __LINE__);

      status = WP_ChannelDelete(iw_tx_ch_enet[WT_WINNET_GROUP_B]);
      terminate_on_error(status, "WP_ChannelDelete iw_tx_ch_enet[WT_WINNET_GROUP_B]", __LINE__);
      iw_tx_ch_enet[WT_WINNET_GROUP_B] = 0;

      status = WP_ChannelDelete(rx_channel_enet_dev[WT_WINNET_GROUP_B]);
      terminate_on_error(status, "WP_ChannelDelete rx_channel_enet_dev[WT_WINNET_GROUP_B]", __LINE__);
      rx_channel_enet_dev[WT_WINNET_GROUP_B] = 0;

      status = WP_ChannelDelete(tx_channel_enet_dev[WT_WINNET_GROUP_B]);
      terminate_on_error(status, "WP_ChannelDelete tx_channel_enet_dev[WT_WINNET_GROUP_B]", __LINE__);
      tx_channel_enet_dev[WT_WINNET_GROUP_B] = 0;

      status = WP_ShapingGroupDisable(shaping_group_swap_l2[WT_WINNET_GROUP_B]);
      terminate_on_error(status, "WP_ShapingGroupDisable shaping_group_swap_l2[WT_WINNET_GROUP_B]", __LINE__);

      status = WP_ShapingGroupDelete(shaping_group_swap_l2[WT_WINNET_GROUP_B]);
      terminate_on_error(status, "WP_ShapingGroupDelete shaping_group_swap_l2[WT_WINNET_GROUP_B]", __LINE__);
      shaping_group_swap_l2[WT_WINNET_GROUP_B] = 0;

      status = WP_ShapingGroupDisable(shaping_group_swap_l1[WT_WINNET_GROUP_B]);
      terminate_on_error(status, "WP_ShapingGroupDisable shaping_group_swap_l1[WT_WINNET_GROUP_B]", __LINE__);

      status = WP_ShapingGroupDelete(shaping_group_swap_l1[WT_WINNET_GROUP_B]);
      terminate_on_error(status, "WP_ShapingGroupDelete shaping_group_swap_l1[WT_WINNET_GROUP_B]", __LINE__);
      shaping_group_swap_l1[WT_WINNET_GROUP_B] = 0;

      status = WP_DeviceDisable(group_b_enet_dev, WP_DIRECTION_DUPLEX);
      terminate_on_error(status, "WP_DeviceEnable group_b_enet_dev", __LINE__);

      status = WP_PortDisable(enetp1, WP_DIRECTION_DUPLEX);
      terminate_on_error(status, "WP_PortDisable 1 G999 port", __LINE__);

      /********************************/
      /* Disable and Delete PCE Rules */
      /********************************/
      status = WP_PceRuleDisable(pce_rx_channel_rules_enet_to_host[WT_WINNET_GROUP_B]);
      terminate_on_error(status, "WP_PceRuleDisable pce_rx_channel_rules_enet_to_host[WT_WINNET_GROUP_B]", __LINE__);

      status = WP_PceRuleDisable(pce_rx_channel_rules_enet_to_swapped_enet[WT_WINNET_GROUP_B][0]);
      terminate_on_error(status, "WP_PceRuleDisable pce_rx_channel_rules_enet_to_swapped_enet[WT_WINNET_GROUP_B][0]", __LINE__);

      status = WP_PceRuleDelete(pce_rx_channel_rules_enet_to_host[WT_WINNET_GROUP_B]);
      terminate_on_error(status, "WP_PceRuleDelete pce_rx_channel_rules_enet_to_host[WT_WINNET_GROUP_B]", __LINE__);
      pce_rx_channel_rules_enet_to_host[WT_WINNET_GROUP_B] = 0;

      status = WP_PceRuleDelete(pce_rx_channel_rules_enet_to_swapped_enet[WT_WINNET_GROUP_B][0]);
      terminate_on_error(status, "WP_PceRuleDelete pce_rx_channel_rules_enet_to_swapped_enet[WT_WINNET_GROUP_B][0]", __LINE__);
      pce_rx_channel_rules_enet_to_swapped_enet[WT_WINNET_GROUP_B][0] = 0;

      /****************************/
      /* Delete IW-Ports          */
      /****************************/
      status = WP_IwPortDelete(bport_host_tx_swapped_enet[WT_WINNET_GROUP_B]);
      terminate_on_error(status, "WP_IwPortDelete bport_host_tx_swapped_enet[WT_WINNET_GROUP_B]", __LINE__);
      bport_host_tx_swapped_enet[WT_WINNET_GROUP_B] = 0;

      status = WP_IwPortDelete(bport_iw_swapped_enet[WT_WINNET_GROUP_B]);
      terminate_on_error(status, "WP_IwPortDelete bport_iw_swapped_enet[WT_WINNET_GROUP_B]", __LINE__);
      bport_iw_swapped_enet[WT_WINNET_GROUP_B] = 0;

      /****************************/
      /* Delete Flow Aggregation  */
      /****************************/
      status = WP_IwFlowAggregationDelete(flow_agg_compatible_enet[WT_WINNET_GROUP_B]);
      terminate_on_error(status, "WP_IwFlowAggregationDelete flow_agg_compatible_enet[WT_WINNET_GROUP_B]", __LINE__);
      flow_agg_compatible_enet[WT_WINNET_GROUP_B] = 0;
   }
} /* WTE_SystemDeleteGroupB */

void WTE_CreateDummyFlowsPort1(void)
{
   WP_status status = WP_OK;
   WP_pce_rule_classification rule_cfg = {0};

   shaping_group_swap_l1[WT_DUMMY_PORT_1] = WP_ShapingGroupCreate(
      dummy_device1,
      WP_SHAPING_GROUP_TYPE_PACKET,
      shaping_group_type_packet_l1_cfg);

   terminate_on_error(shaping_group_swap_l1[WT_DUMMY_PORT_1],
                      "WP_ShapingGroupCreate shaping_group_swap_l1[WT_DUMMY_PORT_1]",
                      __LINE__);

   shaping_group_swap_l2[WT_DUMMY_PORT_1] = WP_ShapingGroupCreate(
      shaping_group_swap_l1[WT_DUMMY_PORT_1],
      WP_SHAPING_GROUP_TYPE_PACKET,
      shaping_group_type_packet_l2_cfg);

   terminate_on_error(shaping_group_swap_l2[WT_DUMMY_PORT_1],
                      "WP_ShapingGroupCreate shaping_group_swap_l2[WT_DUMMY_PORT_1]",
                      __LINE__);

   /*****************************************************/
   /* Create Rx channel for the swapped Enet Rx Channel */
   /*****************************************************/
   ch_enet_cfg[0].iwmode = WP_PKTCH_IWM_ENABLE;
   rx_channel_enet_dev[WT_DUMMY_PORT_1] =
      WP_ChannelCreate(ETH_RX_PHYSICAL_DEV_SWAP_CH_TAG,
                       dummy_device1,
                       qniw_enet,
                       WP_CH_RX,
                       WP_ENET,
                       ch_enet_cfg);
   terminate_on_error(rx_channel_enet_dev[WT_DUMMY_PORT_1], "WP_ChannelCreate() rx_channel_enet_dev[WT_DUMMY_PORT_1]", __LINE__);

   ch_enet_cfg[0].iwmode = WP_PKTCH_IWM_ENABLE;
   tx_channel_enet_dev[WT_DUMMY_PORT_1] =
      WP_ChannelCreate(ETH_TX_SWAP_TAG,
                       shaping_group_swap_l2[WT_DUMMY_PORT_1],
                       qniw_enet,
                       WP_CH_TX,
                       WP_ENET,
                       ch_enet_cfg);
   terminate_on_error(tx_channel_enet_dev[WT_DUMMY_PORT_1], "WP_ChannelCreate tx_channel_enet_dev[WT_DUMMY_PORT_1]", __LINE__);
   /************************************/
   /* Enet Configuration               */
   /************************************/
   iw_agg_hq_cfg[0].txfunc = iw_rx_ch;
   iw_agg_hq_cfg[0].tag = IW_FLOW_AGG_TAG_HOST + 1 + WT_DUMMY_PORT_1;
   iw_agg_hq_cfg[0].iw_port = bport_host;
   flow_agg_host[WT_DUMMY_PORT_1] =
      WP_IwFlowAggregationCreate(WP_WINPATH(DEFAULT_WPID),
                                 WP_IW_HOST_QUEUE_MODE,
                                 iw_agg_hq_cfg);
   terminate_on_error(flow_agg_host[WT_DUMMY_PORT_1],"WP_IwFlowAggregationCreate() flow_agg_host[WT_DUMMY_PORT_1] with IW-port", __LINE__);

   bridge_port_cfg[0].flow_agg = flow_agg_host[WT_DUMMY_PORT_1];
   bridge_port_cfg[0].tag = BRIDGE_PORT_TAG_START;
   bport_iw_swapped_enet[WT_DUMMY_PORT_1] = WP_IwPortCreate( iw_bridging_system_handle, bridge_port_cfg);
   terminate_on_error(bport_iw_swapped_enet[WT_DUMMY_PORT_1], "WP_IwPortCreate bport_iw_swapped_enet[WT_DUMMY_PORT_1]", __LINE__);

   tx_agg_bridging[0].txfunc = tx_channel_enet_dev[WT_DUMMY_PORT_1];
   tx_agg_bridging[0].iw_port = bport_iw_swapped_enet[WT_DUMMY_PORT_1];
   tx_agg_bridging[0].tag = IW_FLOW_AGG_TAG_COMPATIBLE;
   flow_agg_compatible_enet[WT_DUMMY_PORT_1] = WP_IwFlowAggregationCreate(WP_WINPATH(DEFAULT_WPID),
                                                                          WP_IW_VLAN_AWARE_BRIDGING_MODE,
                                                                          tx_agg_bridging);
   terminate_on_error(flow_agg_compatible_enet[WT_DUMMY_PORT_1], "WP_IwFlowAggregationCreate flow_agg_compatible_enet[WT_DUMMY_PORT_1] bport_iw_swapped_enet[WT_DUMMY_PORT_1]", __LINE__);

   /* Create a bridge port */
   bridge_port_cfg[0].flow_agg = flow_agg_compatible_enet[WT_DUMMY_PORT_1];
   bridge_port_cfg[0].tag = BRIDGE_PORT_HOST_TAG;
   bport_host_tx_swapped_enet[WT_DUMMY_PORT_1] = WP_IwPortCreate( iw_bridging_system_handle, bridge_port_cfg);
   terminate_on_error(bport_host_tx_swapped_enet[WT_DUMMY_PORT_1], "WP_IwPortCreate bport_host_tx_swapped_enet[WT_DUMMY_PORT_1]", __LINE__);


   /**************************/
   /* Create host TX channel */
   /**************************/
   /* creating the IW host tx channel here because
    * the bport_host needs to be created first.   */
   ch_iw_tx_cfg[0].iw_system = iw_bridging_system_handle;
   ch_iw_tx_cfg[0].input_port = bport_host_tx_swapped_enet[WT_DUMMY_PORT_1];
   iw_tx_ch_enet[WT_DUMMY_PORT_1] = WP_ChannelCreate(IW_TX_CH_TAG_START,
                                                     host_dev,
                                                     qniw_host,
                                                     WP_CH_TX,
                                                     WP_IW_HOST,
                                                     ch_iw_tx_cfg);
   terminate_on_error(iw_tx_ch_enet[WT_DUMMY_PORT_1], "WP_ChannelCreate to iw_tx_ch_enet[WT_DUMMY_PORT_1]", __LINE__);

   /***********************************************/
   /* PCE rules forward from swapped Enet to HOST */
   /***********************************************/
   memset (&rule_cfg, 0, sizeof (WP_pce_rule_classification));
   rule_cfg.tag = RX_CHANNEL_RULES_HOST_TAG_START;
   rule_cfg.filter_handle = pce_rx_channel_filter;
   rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_MAC_DA;
   rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_INPUT_IW_PORT;
   rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_LAST;
   rule_cfg.match_action = WP_PCE_RULE_MATCH_ACCEPT;
   rule_cfg.enabled = WP_ENABLE;
   rule_cfg.rule_fields[0].value.mac_addr[0] = 0x12;
   rule_cfg.rule_fields[0].value.mac_addr[1] = 0x34;
   rule_cfg.rule_fields[0].value.mac_addr[2] = 0x56;
   rule_cfg.rule_fields[0].value.mac_addr[3] = 0x78;
   rule_cfg.rule_fields[0].value.mac_addr[4] = 0x00;
   rule_cfg.rule_fields[0].value.mac_addr[5] = 0x00;
   rule_cfg.rule_fields[1].value.iw_port_handle = bport_iw_swapped_enet[WT_DUMMY_PORT_1];
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
   rule_cfg.match_result[0].param.flow_agg.flow_aggregation = flow_agg_host[WT_DUMMY_PORT_1];
   rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;
   pce_rx_channel_rules_enet_to_host[WT_DUMMY_PORT_1] = WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg);
   terminate_on_error(pce_rx_channel_rules_enet_to_host[WT_DUMMY_PORT_1], "WP_PceRuleCreate pce_rx_channel_rules_enet_to_host[WT_DUMMY_PORT_1]", __LINE__);

   rx_binding_bridging_cfg[0].input_port = bport_iw_swapped_enet[WT_DUMMY_PORT_1];
   rx_binding_bridging_cfg[0].pce_if_handle = rx_channel_enet_pce_if_handle;
   status = WP_IwRxBindingCreate(rx_channel_enet_dev[WT_DUMMY_PORT_1],
                                 iw_bridging_system_handle,
                                 qniw_enet,
                                 rx_binding_bridging_cfg);
   terminate_on_error(status, "WP_IwRxBindingCreate rx_channel_enet_dev[WT_DUMMY_PORT_1] to bport_iw_swapped_enet[WT_DUMMY_PORT_1]", __LINE__);

   status = WP_IwTxBindingCreate(tx_channel_enet_dev[WT_DUMMY_PORT_1],
                                 WP_IW_TX_BINDING,
                                 tx_binding_cfg);
   terminate_on_error(status, "WP_IwTxBindingCreate tx_channel_enet_dev[WT_DUMMY_PORT_1]", __LINE__);

   status = WP_PortEnable(dummy_port1, WP_DIRECTION_DUPLEX);
   terminate_on_error(status, "WP_PortEnable dummy_port1", __LINE__);

   status = WP_ShapingGroupEnable(shaping_group_swap_l1[WT_DUMMY_PORT_1]);
   terminate_on_error(status, "WP_ShapingGroupEnable shaping_group_swap_l1[WT_DUMMY_PORT_1]", __LINE__);

   status = WP_ShapingGroupEnable(shaping_group_swap_l2[WT_DUMMY_PORT_1]);
   terminate_on_error(status, "WP_ShapingGroupEnable shaping_group_swap_l2[WT_DUMMY_PORT_1]", __LINE__);

   status = WP_ChannelEnable(iw_tx_ch_enet[WT_DUMMY_PORT_1]);
   terminate_on_error(status, "WP_ChannelEnable iw_tx_ch_enet[WT_DUMMY_PORT_1]", __LINE__);

   status = WP_ChannelEnable(tx_channel_enet_dev[WT_DUMMY_PORT_1]);
   terminate_on_error(status, "WP_ChannelEnable tx_channel_enet_dev[WT_DUMMY_PORT_1]", __LINE__);

   status = WP_ChannelEnable(rx_channel_enet_dev[WT_DUMMY_PORT_1]);
   terminate_on_error(status, "WP_ChannelEnable rx_channel_enet_dev[WT_DUMMY_PORT_1]", __LINE__);

   status = WP_DeviceEnable(dummy_device1, WP_DIRECTION_DUPLEX);
   terminate_on_error(status, "WP_DeviceEnable dummy_device1", __LINE__);
} /* WTE_CreateDummyFlowsPort1 */

void WTE_CreateDummyFlowsPort2(void)
{
   WP_status status = WP_OK;
   WP_pce_rule_classification rule_cfg = {0};

   shaping_group_swap_l1[WT_DUMMY_PORT_2] = WP_ShapingGroupCreate(
      dummy_device2,
      WP_SHAPING_GROUP_TYPE_PACKET,
      shaping_group_type_packet_l1_cfg);

   terminate_on_error(shaping_group_swap_l1[WT_DUMMY_PORT_2],
                      "WP_ShapingGroupCreate shaping_group_swap_l1[WT_DUMMY_PORT_2]",
                      __LINE__);

   shaping_group_swap_l2[WT_DUMMY_PORT_2] = WP_ShapingGroupCreate(
      shaping_group_swap_l1[WT_DUMMY_PORT_2],
      WP_SHAPING_GROUP_TYPE_PACKET,
      shaping_group_type_packet_l2_cfg);

   terminate_on_error(shaping_group_swap_l2[WT_DUMMY_PORT_2],
                      "WP_ShapingGroupCreate shaping_group_swap_l2[WT_DUMMY_PORT_2]",
                      __LINE__);

   /*****************************************************/
   /* Create Rx channel for the swapped Enet Rx Channel */
   /*****************************************************/
   ch_enet_cfg[0].iwmode = WP_PKTCH_IWM_ENABLE;
   rx_channel_enet_dev[WT_DUMMY_PORT_2] =
      WP_ChannelCreate(ETH_RX_PHYSICAL_DEV_SWAP_CH_TAG,
                       dummy_device2,
                       qniw_enet,
                       WP_CH_RX,
                       WP_ENET,
                       ch_enet_cfg);
   terminate_on_error(rx_channel_enet_dev[WT_DUMMY_PORT_2], "WP_ChannelCreate() rx_channel_enet_dev[WT_DUMMY_PORT_2]", __LINE__);

   ch_enet_cfg[0].iwmode = WP_PKTCH_IWM_ENABLE;
   tx_channel_enet_dev[WT_DUMMY_PORT_2] =
      WP_ChannelCreate(ETH_TX_SWAP_TAG,
                       shaping_group_swap_l2[WT_DUMMY_PORT_2],
                       qniw_enet,
                       WP_CH_TX,
                       WP_ENET,
                       ch_enet_cfg);
   terminate_on_error(tx_channel_enet_dev[WT_DUMMY_PORT_2], "WP_ChannelCreate tx_channel_enet_dev[WT_DUMMY_PORT_2]", __LINE__);
   /************************************/
   /* Enet Configuration               */
   /************************************/
   iw_agg_hq_cfg[0].txfunc = iw_rx_ch;
   iw_agg_hq_cfg[0].tag = IW_FLOW_AGG_TAG_HOST + 1 + WT_DUMMY_PORT_2;
   iw_agg_hq_cfg[0].iw_port = bport_host;
   flow_agg_host[WT_DUMMY_PORT_2] =
      WP_IwFlowAggregationCreate(WP_WINPATH(DEFAULT_WPID),
                                 WP_IW_HOST_QUEUE_MODE,
                                 iw_agg_hq_cfg);
   terminate_on_error(flow_agg_host[WT_DUMMY_PORT_2],"WP_IwFlowAggregationCreate() flow_agg_host[WT_DUMMY_PORT_2] with IW-port", __LINE__);

   bridge_port_cfg[0].flow_agg = flow_agg_host[WT_DUMMY_PORT_2];
   bridge_port_cfg[0].tag = BRIDGE_PORT_TAG_START;
   bport_iw_swapped_enet[WT_DUMMY_PORT_2] = WP_IwPortCreate( iw_bridging_system_handle, bridge_port_cfg);
   terminate_on_error(bport_iw_swapped_enet[WT_DUMMY_PORT_2], "WP_IwPortCreate bport_iw_swapped_enet[WT_DUMMY_PORT_2]", __LINE__);

   tx_agg_bridging[0].txfunc = tx_channel_enet_dev[WT_DUMMY_PORT_2];
   tx_agg_bridging[0].iw_port = bport_iw_swapped_enet[WT_DUMMY_PORT_2];
   tx_agg_bridging[0].tag = IW_FLOW_AGG_TAG_COMPATIBLE;
   flow_agg_compatible_enet[WT_DUMMY_PORT_2] = WP_IwFlowAggregationCreate(WP_WINPATH(DEFAULT_WPID),
                                                                          WP_IW_VLAN_AWARE_BRIDGING_MODE,
                                                                          tx_agg_bridging);
   terminate_on_error(flow_agg_compatible_enet[WT_DUMMY_PORT_2], "WP_IwFlowAggregationCreate flow_agg_compatible_enet[WT_DUMMY_PORT_2] bport_iw_swapped_enet[WT_DUMMY_PORT_2]", __LINE__);

   /* Create a bridge port */
   bridge_port_cfg[0].flow_agg = flow_agg_compatible_enet[WT_DUMMY_PORT_2];
   bridge_port_cfg[0].tag = BRIDGE_PORT_HOST_TAG;
   bport_host_tx_swapped_enet[WT_DUMMY_PORT_2] = WP_IwPortCreate( iw_bridging_system_handle, bridge_port_cfg);
   terminate_on_error(bport_host_tx_swapped_enet[WT_DUMMY_PORT_2], "WP_IwPortCreate bport_host_tx_swapped_enet[WT_DUMMY_PORT_2]", __LINE__);

   /**************************/
   /* Create host TX channel */
   /**************************/
   /* creating the IW host tx channel here because
    * the bport_host needs to be created first.   */
   ch_iw_tx_cfg[0].iw_system = iw_bridging_system_handle;
   ch_iw_tx_cfg[0].input_port = bport_host_tx_swapped_enet[WT_DUMMY_PORT_2];
   iw_tx_ch_enet[WT_DUMMY_PORT_2] = WP_ChannelCreate(IW_TX_CH_TAG_START,
                                                     host_dev,
                                                     qniw_host,
                                                     WP_CH_TX,
                                                     WP_IW_HOST,
                                                     ch_iw_tx_cfg);
   terminate_on_error(iw_tx_ch_enet[WT_DUMMY_PORT_2], "WP_ChannelCreate to iw_tx_ch_enet[WT_DUMMY_PORT_2]", __LINE__);

   /***********************************************/
   /* PCE rules forward from swapped Enet to HOST */
   /***********************************************/
   memset (&rule_cfg, 0, sizeof (WP_pce_rule_classification));
   rule_cfg.tag = RX_CHANNEL_RULES_HOST_TAG_START;
   rule_cfg.filter_handle = pce_rx_channel_filter;
   rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_MAC_DA;
   rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_INPUT_IW_PORT;
   rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_LAST;
   rule_cfg.match_action = WP_PCE_RULE_MATCH_ACCEPT;
   rule_cfg.enabled = WP_ENABLE;
   rule_cfg.rule_fields[0].value.mac_addr[0] = 0x12;
   rule_cfg.rule_fields[0].value.mac_addr[1] = 0x34;
   rule_cfg.rule_fields[0].value.mac_addr[2] = 0x56;
   rule_cfg.rule_fields[0].value.mac_addr[3] = 0x78;
   rule_cfg.rule_fields[0].value.mac_addr[4] = 0x00;
   rule_cfg.rule_fields[0].value.mac_addr[5] = 0x00;
   rule_cfg.rule_fields[1].value.iw_port_handle = bport_iw_swapped_enet[WT_DUMMY_PORT_2];
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
   rule_cfg.match_result[0].param.flow_agg.flow_aggregation = flow_agg_host[WT_DUMMY_PORT_2];
   rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;
   pce_rx_channel_rules_enet_to_host[WT_DUMMY_PORT_2] = WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg);
   terminate_on_error(pce_rx_channel_rules_enet_to_host[WT_DUMMY_PORT_2], "WP_PceRuleCreate pce_rx_channel_rules_enet_to_host[WT_DUMMY_PORT_2]", __LINE__);

   rx_binding_bridging_cfg[0].input_port = bport_iw_swapped_enet[WT_DUMMY_PORT_2];
   rx_binding_bridging_cfg[0].pce_if_handle = rx_channel_enet_pce_if_handle;
   status = WP_IwRxBindingCreate(rx_channel_enet_dev[WT_DUMMY_PORT_2],
                                 iw_bridging_system_handle,
                                 qniw_enet,
                                 rx_binding_bridging_cfg);
   terminate_on_error(status, "WP_IwRxBindingCreate rx_channel_enet_dev[WT_DUMMY_PORT_2] to bport_iw_swapped_enet[WT_DUMMY_PORT_2]", __LINE__);

   /*****************************************************/
   /* Create Tx binding for the swapped Enet Tx Channel */
   /*****************************************************/
   status = WP_IwTxBindingCreate(tx_channel_enet_dev[WT_DUMMY_PORT_2],
                                 WP_IW_TX_BINDING,
                                 tx_binding_cfg);
   terminate_on_error(status, "WP_IwTxBindingCreate tx_channel_enet_dev[WT_DUMMY_PORT_2]", __LINE__);

   status = WP_PortEnable(dummy_port2, WP_DIRECTION_DUPLEX);
   terminate_on_error(status, "WP_PortEnable dummy_port2", __LINE__);

   status = WP_ShapingGroupEnable(shaping_group_swap_l1[WT_DUMMY_PORT_2]);
   terminate_on_error(status, "WP_ShapingGroupEnable shaping_group_swap_l1[WT_DUMMY_PORT_2]", __LINE__);

   status = WP_ShapingGroupEnable(shaping_group_swap_l2[WT_DUMMY_PORT_2]);
   terminate_on_error(status, "WP_ShapingGroupEnable shaping_group_swap_l2[WT_DUMMY_PORT_2]", __LINE__);

   status = WP_ChannelEnable(iw_tx_ch_enet[WT_DUMMY_PORT_2]);
   terminate_on_error(status, "WP_ChannelEnable iw_tx_ch_enet[WT_DUMMY_PORT_2]", __LINE__);

   status = WP_ChannelEnable(tx_channel_enet_dev[WT_DUMMY_PORT_2]);
   terminate_on_error(status, "WP_ChannelEnable tx_channel_enet_dev[WT_DUMMY_PORT_2]", __LINE__);

   status = WP_ChannelEnable(rx_channel_enet_dev[WT_DUMMY_PORT_2]);
   terminate_on_error(status, "WP_ChannelEnable rx_channel_enet_dev[WT_DUMMY_PORT_2]", __LINE__);

   status = WP_DeviceEnable(dummy_device2, WP_DIRECTION_DUPLEX);
   terminate_on_error(status, "WP_DeviceEnable dummy_device2", __LINE__);
} /* WTE_CreateDummyFlowsPort2 */

void WTE_DisplayIwPortStats(WP_handle iw_port_handle, WP_boolean router_port, WP_CHAR* type)
{
   WP_status status = 0;
   WP_U32 jj;

   if (router_port)
   {
      WP_route_port_stats rout_port_stats;
      memset( &rout_port_stats, 0, sizeof(WP_route_port_stats) );
      printf("%s Iw Router Port Statistics (0x%08x)\n",type, iw_port_handle);
      printf("============================================\n");
      status = WP_IwPortStatistics( iw_port_handle, &rout_port_stats );
      terminate_on_error(status, "WP_IwPortStatistics()",__LINE__);

   printf ("%x%x", ((WPT_StatField*)&rout_port_stats.rx_valid_packets)->part.high, ((WPT_StatField*)&rout_port_stats.rx_valid_packets)->part.low);
      WTE_PRINT_STAT_HIGH_LOW   ("Rx Valid Packets:               %08x%08x\n",
                                 rout_port_stats.rx_valid_packets);
      WTE_PRINT_STAT_IF_NOT_ZERO("rx_ipv4_option_packets:         %08x%08x\n",
                                 rout_port_stats.rx_ipv4_option_packets);
      WTE_PRINT_STAT_IF_NOT_ZERO("rx_non_ip_packets:              %08x%08x\n",
                                 rout_port_stats.rx_non_ip_packets);
      WTE_PRINT_STAT_IF_NOT_ZERO("rx_compressed_packets:          %08x%08x\n",
                                 rout_port_stats.rx_compressed_packets);
      WTE_PRINT_STAT_IF_NOT_ZERO("rx_valid_mpls_packets:          %08x%08x\n",
                                 rout_port_stats.rx_valid_mpls_packets);
      WTE_PRINT_STAT_IF_NOT_ZERO("rx_protocol_error:              %08x%08x\n",
                                 rout_port_stats.rx_protocol_error);
      WTE_PRINT_STAT_IF_NOT_ZERO("rx_checksum_error:              %08x%08x\n",
                                 rout_port_stats.rx_checksum_error);
      WTE_PRINT_STAT_IF_NOT_ZERO("rx_discard_classifier:          %08x%08x\n",
                                 rout_port_stats.rx_discard_classifier);
      WTE_PRINT_STAT_IF_NOT_ZERO("rx_mpls_lookup_drop:            %08x%08x\n",
                                 rout_port_stats.rx_mpls_lookup_drop);
      WTE_PRINT_STAT_IF_NOT_ZERO("tx_forwarded_packets:           %08x%08x\n",
                                 rout_port_stats.tx_forwarded_packets);
      WTE_PRINT_STAT_IF_NOT_ZERO("rx_ipv6_hop_by_hop_host_term:   %08x%08x\n",
                                 rout_port_stats.rx_ipv6_hop_by_hop_host_term);
      WTE_PRINT_STAT_IF_NOT_ZERO("rx_ipv6_link_local_host_term:   %08x%08x\n",
                                 rout_port_stats.rx_ipv6_link_local_host_term);
      WTE_PRINT_STAT_IF_NOT_ZERO("rx_discard_lpm:                 %08x%08x\n",
                                 rout_port_stats.rx_discard_lpm);
      for (jj=0 ; jj<WP_IW_DFC_MAX_NUM_OF_FILTERS ; ++jj)
      {
         WTE_PRINT_STAT_IF_NOT_ZERO_WITH_INDEX("discard_dfc_filter[%d]:        %08x%08x\n",
                                               jj, rout_port_stats.discard_dfc_filter[jj]);
      }
      for (jj=0 ; jj<WP_PCE_MAX_NUM_OF_FILTERS ; ++jj)
      {
         WTE_PRINT_STAT_IF_NOT_ZERO_WITH_INDEX("discard_pce_filter[%d]:        %08x%08x\n",
                                               jj, rout_port_stats.discard_pce_filter[jj]);
      }
      WTE_PRINT_STAT_IF_NOT_ZERO("ingress_policer_non_conforming_packets: %08x%08x\n",
                                 rout_port_stats.ingress_policer_non_conforming_packets);
   }
   else
   {
      WP_bport_stats bport_stats;
      memset( &bport_stats, 0, sizeof(WP_bport_stats) );
      printf("%s Iw Bridge Port Statistics (0x%08x)\n",type, iw_port_handle);
      printf("============================================\n");
      status = WP_IwPortStatistics( iw_port_handle, &bport_stats );
      terminate_on_error(status, "WP_IwPortStatistics()",__LINE__);

      WTE_PRINT_STAT_HIGH_LOW   ("Rx Valid Packets:               %08x%08x\n",
                                 bport_stats.rx_valid_packets);
      WTE_PRINT_STAT_IF_NOT_ZERO("Discard VLAN acceptable filter: %08x%08x\n",
                                 bport_stats.discard_vlan_acceptable_filter);
      WTE_PRINT_STAT_IF_NOT_ZERO("Discard ingress filter:         %08x%08x\n",
                                 bport_stats.discard_ingress_filter);
      WTE_PRINT_STAT_IF_NOT_ZERO("Discard bridge classifier:      %08x%08x\n",
                                 bport_stats.discard_bridge_classifier);
      WTE_PRINT_STAT_IF_NOT_ZERO("Discard unknown MAC SA:         %08x%08x\n",
                                 bport_stats.discard_unk_macsa);
      WTE_PRINT_STAT_IF_NOT_ZERO("Deny MAC SA:                    %08x%08x\n",
                                 bport_stats.deny_mac_sa);
      WTE_PRINT_STAT_IF_NOT_ZERO("Deny MAC DA:                    %08x%08x\n",
                                 bport_stats.deny_mac_da);
      WTE_PRINT_STAT_IF_NOT_ZERO("Rx BC valid packets:            %08x%08x\n",
                                 bport_stats.rx_bc_valid_packets);
      WTE_PRINT_STAT_IF_NOT_ZERO("Rx MC valid packets:            %08x%08x\n",
                                 bport_stats.rx_mc_valid_packets);
      WTE_PRINT_STAT_IF_NOT_ZERO("Forwarded UC packets:           %08x%08x\n",
                                 bport_stats.forwarded_uc_packets);
      WTE_PRINT_STAT_IF_NOT_ZERO("Forwarded BC packets:           %08x%08x\n",
                                 bport_stats.forwarded_bc_packets);
      WTE_PRINT_STAT_IF_NOT_ZERO("Forwarded MC packets:           %08x%08x\n",
                                 bport_stats.forwarded_mc_packets);
      WTE_PRINT_STAT_IF_NOT_ZERO("Protocol Error Packets:         %08x%08x\n",
                                 bport_stats.protocol_error);
      WTE_PRINT_STAT_IF_NOT_ZERO("Res Mc Mac Deny Packets:        %08x%08x\n",
                                 bport_stats.res_mc_mac_deny);
      for (jj=0 ; jj<WP_IW_DFC_MAX_NUM_OF_FILTERS ; ++jj)
      {
         WTE_PRINT_STAT_IF_NOT_ZERO_WITH_INDEX(
            "Discard DFC Filter [%02d]:        %08x%08x\n",
            jj, bport_stats.discard_dfc_filter[jj]);
      }
/*    WTE_PRINT_STAT_IF_NOT_ZERO("Deny CFM Frames:                %08x%08x\n", */
/*                               bport_stats.deny_cfm_frames); */
      WTE_PRINT_STAT_IF_NOT_ZERO("Bad IP Version Packets:         %08x%08x\n",
                                 bport_stats.bad_ip_version);
      WTE_PRINT_STAT_IF_NOT_ZERO("Checksum Error Packets:         %08x%08x\n",
                                 bport_stats.checksum_error);
      WTE_PRINT_STAT_IF_NOT_ZERO("IP Protocol Error Packets:      %08x%08x\n",
                                 bport_stats.ip_protocol_error);
      for (jj=0 ; jj<WP_PCE_MAX_NUM_OF_FILTERS ; ++jj)
      {
         WTE_PRINT_STAT_IF_NOT_ZERO_WITH_INDEX(
            "Discard PCE Filter [%02d]:        %08x%08x\n",
            jj, bport_stats.discard_pce_filter[jj]);
      }
      WTE_PRINT_STAT_IF_NOT_ZERO("Ingress Policer Non Conforming: %08x%08x\n",
                                 bport_stats.ingress_policer_non_conforming_packets);
   }

   printf("\n");
   return;
}

void WTE_ResetIwPortStats(WP_handle iw_port_handle, WP_boolean router_port)
{
   WP_status status = 0;

   if (router_port)
   {
      WP_route_port_stats rout_port_stats;
      memset( &rout_port_stats, 0, sizeof(WP_route_port_stats) );
      status = WP_IwPortStatisticsReset( iw_port_handle, &rout_port_stats );
      terminate_on_error(status, "WP_IwPortStatisticsReset()",__LINE__);
   }
   else
   {
      WP_bport_stats bport_stats;
      memset( &bport_stats, 0, sizeof(WP_bport_stats) );
      status = WP_IwPortStatisticsReset( iw_port_handle, &bport_stats );
      terminate_on_error(status, "WP_IwPortStatisticsReset()",__LINE__);
   }
   return;
}

void WTE_ResetFlowStats(WP_handle agg)
{
   WP_iw_flow_stats iw_flow_stats;
   WP_status status;

   memset(&iw_flow_stats,0,sizeof(WP_iw_flow_stats));
   status = WP_IwFlowStatisticsReset( agg, WP_IW_FLOW_STAT, &iw_flow_stats);
   terminate_on_error(status, "WP_IwFlowStatisticsReset()",__LINE__);

   return;
}


void WTE_ResetGenericFlowStats(WP_handle agg)
{
   WP_iw_generic_flow_stats iw_generic_flow_stats;
   WP_status status;

   memset(&iw_generic_flow_stats,0,sizeof(WP_iw_generic_flow_stats));
   status = WP_IwFlowStatisticsReset( agg, WP_IW_FLOW_STAT, &iw_generic_flow_stats);
   terminate_on_error(status, "WP_IwFlowStatisticsReset()",__LINE__);

   return;
}

void WTE_ResetG999StreamStats(WP_handle stream_handle)
{
   WP_status status;
   WP_stats_g999_stream stats_enet_g999;

   memset( &stats_enet_g999, 0, sizeof(WP_stats_g999_stream) );
   status = WP_G999StreamstatisticsReset(stream_handle, &stats_enet_g999);
   terminate_on_error(status, "WP_G999StreamstatisticsReset()",__LINE__);

   return;
}

void WTE_ResetEnetDeviceStats(WP_handle device_handle)
{

   WP_status status;
   WP_stats_enet stats_enet = {0};

   memset( &stats_enet, 0, sizeof(WP_stats_enet) );
   status = WP_DeviceStatisticsReset(device_handle, &stats_enet);
   terminate_on_error(status, "WP_DeviceStatisticsReset", __LINE__);

   return;
} /* WTE_ResetEnetDeviceStats */


void WTE_ResetPCEStats(WP_handle filter_handle)
{
   WP_status status;
   WP_pce_filter_stat pce_stats;

   memset(&pce_stats, 0, sizeof(WP_pce_filter_stat) );
   if((status = WP_PceFilterStatisticsReset(filter_handle, &pce_stats)) )
   {
      printf("PCE ResetStatistics error\n");
      return;
   }
} /* WTE_ResetPCEStats */


void WTE_ResetPCEGlobalStats(void)
{
   WP_status status = WP_OK;
   WP_pce_global_stat pce_global_stat;
   memset(&pce_global_stat, 0, sizeof(WP_pce_global_stat) );
   status =  WP_PceGlobalStatisticsReset(WP_WINPATH(DEFAULT_WPID),
                                         &pce_global_stat);
} /* WTE_ResetPCEGlobalStats */

void WTE_ResetStatisticsGroupA(WP_U8 swap_mode)
{
   if (swap_mode == WT_SWAP_MODE_G999)
   {
      WTE_ResetEnetDeviceStats(group_a_g999_dev);
      WTE_ResetFlowStats(flow_agg_host[WT_WINNET_GROUP_A]);
      WTE_ResetIwPortStats(bport_host, WP_FALSE);
      WTE_ResetIwPortStats(bport_iw_enet[WT_WINNET_GROUP_A], WP_FALSE);

   }
   else /* (swap_mode == WT_SWAP_MODE_ENET) */
   {
      WTE_ResetEnetDeviceStats(group_a_enet_dev);
      WTE_ResetFlowStats(flow_agg_host[WT_WINNET_GROUP_A]);
      WTE_ResetIwPortStats(bport_host, WP_FALSE);
      WTE_ResetIwPortStats(bport_iw_enet[WT_WINNET_GROUP_A], WP_FALSE);
   }
}

void WTE_ResetStatisticsGroupB(WP_U8 swap_mode)
{
   if (swap_mode == WT_SWAP_MODE_G999)
   {
      WTE_ResetEnetDeviceStats(group_b_g999_dev);
      WTE_ResetFlowStats(flow_agg_host[WT_WINNET_GROUP_B]);
      WTE_ResetIwPortStats(bport_host, WP_FALSE);
      WTE_ResetIwPortStats(bport_iw_enet[WT_WINNET_GROUP_B], WP_FALSE);

   }
   else /* (swap_mode == WT_SWAP_MODE_ENET) */
   {
      WTE_ResetEnetDeviceStats(group_b_enet_dev);
      WTE_ResetFlowStats(flow_agg_host[WT_WINNET_GROUP_B]);
      WTE_ResetIwPortStats(bport_host, WP_FALSE);
      WTE_ResetIwPortStats(bport_iw_enet[WT_WINNET_GROUP_B], WP_FALSE);
   }
}

void WTE_ResetStatisticsDummyPort1(void)
{
   WTE_ResetEnetDeviceStats(dummy_device1);
   WTE_ResetFlowStats(flow_agg_host[WT_DUMMY_PORT_1]);
   WTE_ResetIwPortStats(bport_iw_swapped_enet[WT_DUMMY_PORT_1], WP_FALSE);
}

void WTE_ResetStatisticsDummyPort2(void)
{
   WTE_ResetEnetDeviceStats(dummy_device2);
   WTE_ResetFlowStats(flow_agg_host[WT_DUMMY_PORT_2]);
   WTE_ResetIwPortStats(bport_iw_swapped_enet[WT_DUMMY_PORT_2], WP_FALSE);
}

void WTE_DisplayFlowStats(WP_handle agg, WP_CHAR *type)
{
   WP_iw_flow_stats iw_flow_stats;
   WP_status status;

   memset(&iw_flow_stats,0,sizeof(WP_iw_flow_stats));
   printf ("%s statistics (%x)\n", type,agg);
   printf ("=================================================\n");
   status = WP_IwFlowStatistics( agg, WP_IW_FLOW_STAT, &iw_flow_stats);
   terminate_on_error(status, "WP_IwFlowStatistics()",__LINE__);

   WTE_PRINT_STAT_HIGH_LOW   ("Forward Packet:          %08x%08x\n",
                              iw_flow_stats.forward_packet);
   WTE_PRINT_STAT_IF_NOT_ZERO("FBP Drop Packets:        %08x%08x\n",
                              iw_flow_stats.fbp_drop_packets);
   WTE_PRINT_STAT_IF_NOT_ZERO("MTU Drop Packets:        %08x%08x\n",
                              iw_flow_stats.mtu_drop_packets);
   WTE_PRINT_STAT_IF_NOT_ZERO("TTL Drop Packets:        %08x%08x\n",
                              iw_flow_stats.ttl_drop_packets);
   WTE_PRINT_STAT_IF_NOT_ZERO("TX Queue Drop Packets:   %08x%08x\n",
                              iw_flow_stats.tx_queue_drop_packets);
   WTE_PRINT_STAT_IF_NOT_ZERO("MPLS Drop:               %08x%08x\n",
                              iw_flow_stats.mpls_drop);
   WTE_PRINT_STAT_IF_NOT_ZERO("Denied Packets:          %08x%08x\n",
                              iw_flow_stats.denied_packets);
   WTE_PRINT_STAT_IF_NOT_ZERO("Group filtered packets:  %08x%08x\n",
                              iw_flow_stats.group_filtered_packets);
   WTE_PRINT_STAT_IF_NOT_ZERO("Forwarded Bytes:         %08x%08x\n",
                              iw_flow_stats.forwarded_bytes);
   WTE_PRINT_STAT_IF_NOT_ZERO("GTP bad headers:         %08x%08x\n",
                              iw_flow_stats.gtp_bad_headers);
   WTE_PRINT_STAT_IF_NOT_ZERO("Policer Non Conforming:  %08x%08x\n",
                              iw_flow_stats.policer_non_conforming_packets);
   WTE_PRINT_STAT_IF_NOT_ZERO("Cfm Mac In Mac Drop:     %08x%08x\n",
                              iw_flow_stats.cfm_mac_in_mac_drop);
   printf("\n");
   return;
}

void WTE_DisplayGenericFlowStats(WP_handle agg, WP_CHAR *type)
{
   WP_iw_generic_flow_stats iw_generic_flow_stats;
   WP_status status;

   memset(&iw_generic_flow_stats,0,sizeof(WP_iw_generic_flow_stats));
   printf ("%s statistics (%x)\n", type,agg);
   printf ("=================================================\n");
   status = WP_IwFlowStatistics( agg, WP_IW_FLOW_STAT, &iw_generic_flow_stats);
   terminate_on_error(status, "WP_IwFlowStatistics()",__LINE__);

   WTE_PRINT_STAT_HIGH_LOW   ("Forward Packet:          %08x%08x\n",
                              iw_generic_flow_stats.forward_packet);
   WTE_PRINT_STAT_IF_NOT_ZERO("mtu_drop_packets:        %08x%08x\n",
                              iw_generic_flow_stats.mtu_drop_packets);
   WTE_PRINT_STAT_IF_NOT_ZERO("tx_queue_drop_packets:   %08x%08x\n",
                              iw_generic_flow_stats.tx_queue_drop_packets);
   WTE_PRINT_STAT_IF_NOT_ZERO("forwarded_bytes:         %08x%08x\n",
                              iw_generic_flow_stats.forwarded_bytes);
   WTE_PRINT_STAT_IF_NOT_ZERO("denied_packets:          %08x%08x\n",
                              iw_generic_flow_stats.denied_packets);
   WTE_PRINT_STAT_IF_NOT_ZERO("policer_non_conforming_packets: %08x%08x\n",
                              iw_generic_flow_stats.policer_non_conforming_packets);
   WTE_PRINT_STAT_IF_NOT_ZERO("pecs_counter_1:          %08x%08x\n",
                              iw_generic_flow_stats.pecs_counter_1);
   WTE_PRINT_STAT_IF_NOT_ZERO("pecs_counter_2:          %08x%08x\n",
                              iw_generic_flow_stats.pecs_counter_2);
   WTE_PRINT_STAT_IF_NOT_ZERO("pecs_counter_3:          %08x%08x\n",
                              iw_generic_flow_stats.pecs_counter_3);
   WTE_PRINT_STAT_IF_NOT_ZERO("pecs_counter_4:          %08x%08x\n",
                              iw_generic_flow_stats.pecs_counter_4);
   WTE_PRINT_STAT_IF_NOT_ZERO("pecs_counter_5:          %08x%08x\n",
                              iw_generic_flow_stats.pecs_counter_5);
   WTE_PRINT_STAT_IF_NOT_ZERO("pecs_counter_6:          %08x%08x\n",
                              iw_generic_flow_stats.pecs_counter_6);
   printf("\n");
   return;
}

void WTE_DisplayMultiClassDeviceStats(WP_handle device_handle, WP_CHAR* type)
{
   WP_status status;
   WP_stats_enet_multi_class_tx enet_multi_class_stats;
   printf ("%s statistics (%x)\n", type, device_handle);
   printf ("============================================\n");
   memset( &enet_multi_class_stats, 0, sizeof(WP_stats_enet_multi_class_tx) );
   status = WP_DeviceStatistics(device_handle, &enet_multi_class_stats);
   terminate_on_error(status, "WP_DeviceStatistics()", __LINE__);

   WTE_PRINT_STAT_HIGH_LOW   ("DPS tx_frames:              %08x%08x\n",
                              enet_multi_class_stats.tx_frames);

   WTE_PRINT_STAT_IF_NOT_ZERO("DPS tx_err_underrun:        %08x%08x\n",
                              enet_multi_class_stats.tx_err_underrun);

   WTE_PRINT_STAT_IF_NOT_ZERO("DPS_TX_IF tx_frames:        %08x%08x\n",
                              enet_multi_class_stats.serial_tx_dps_if_stats.tx_frames);
   WTE_PRINT_STAT_IF_NOT_ZERO("DPS_TX_IF tx_bytes:         %08x%08x\n",
                              enet_multi_class_stats.serial_tx_dps_if_stats.tx_bytes);
   printf("\n");
   return;
}

void WTE_DisplayEnetDeviceStats(WP_handle device_handle, WP_CHAR* type)
{

   WP_status status;
   WP_stats_enet stats_enet = {0};

   printf ("%s statistics (%x)\n", type, device_handle);
   printf ("============================================\n");
   memset( &stats_enet, 0, sizeof(WP_stats_enet) );
   status = WP_DeviceStatistics(device_handle, &stats_enet);
   terminate_on_error(status, "WP_DeviceStatistics()", __LINE__);

   WTE_PRINT_STAT_IF_NOT_ZERO("HW  rx_bytes:                  %08x%08x\n",
                              stats_enet.rx_bytes);
   WTE_PRINT_STAT_HIGH_LOW   ("HW  rx_packets:                %08x%08x\n",
                              stats_enet.rx_packets);
   WTE_PRINT_STAT_IF_NOT_ZERO("HW  rx_err_fcs:                %08x%08x\n",
                              stats_enet.rx_err_fcs);
   WTE_PRINT_STAT_IF_NOT_ZERO("HW  rx_multicast:              %08x%08x\n",
                              stats_enet.rx_multicast);
   WTE_PRINT_STAT_IF_NOT_ZERO("HW  rx_broadcast:              %08x%08x\n",
                              stats_enet.rx_broadcast);
   WTE_PRINT_STAT_IF_NOT_ZERO("HW  rx_mac_control:            %08x%08x\n",
                              stats_enet.rx_mac_control);
   WTE_PRINT_STAT_IF_NOT_ZERO("HW  rx_mac_pause:              %08x%08x\n",
                              stats_enet.rx_mac_pause);
   WTE_PRINT_STAT_IF_NOT_ZERO("HW  rx_mac_unknown:            %08x%08x\n",
                              stats_enet.rx_mac_unknown);
   WTE_PRINT_STAT_IF_NOT_ZERO("HW  rx_err_alignment:          %08x%08x\n",
                              stats_enet.rx_err_alignment);
   WTE_PRINT_STAT_IF_NOT_ZERO("HW  rx_err_length:             %08x%08x\n",
                              stats_enet.rx_err_length);
   WTE_PRINT_STAT_IF_NOT_ZERO("HW  rx_err_code:               %08x%08x\n",
                              stats_enet.rx_err_code);
   WTE_PRINT_STAT_IF_NOT_ZERO("HW  rx_false_carrier:          %08x%08x\n",
                              stats_enet.rx_false_carrier);
   WTE_PRINT_STAT_IF_NOT_ZERO("HW  rx_undersize:              %08x%08x\n",
                              stats_enet.rx_undersize);
   WTE_PRINT_STAT_IF_NOT_ZERO("HW  rx_oversize:               %08x%08x\n",
                              stats_enet.rx_oversize);
   WTE_PRINT_STAT_IF_NOT_ZERO("HW  rx_fragments:              %08x%08x\n",
                              stats_enet.rx_fragments);
   WTE_PRINT_STAT_IF_NOT_ZERO("HW  rx_jabber:                 %08x%08x\n",
                              stats_enet.rx_jabber);
   WTE_PRINT_STAT_IF_NOT_ZERO("HW  rx_dropped:                %08x%08x\n",
                              stats_enet.rx_dropped);
   WTE_PRINT_STAT_IF_NOT_ZERO("HW  tx_bytes:                  %08x%08x\n",
                              stats_enet.tx_bytes);
   WTE_PRINT_STAT_HIGH_LOW   ("HW  tx_packets:                %08x%08x\n",
                              stats_enet.tx_packets);
   WTE_PRINT_STAT_IF_NOT_ZERO("HW  tx_multicast:              %08x%08x\n",
                              stats_enet.tx_multicast);
   WTE_PRINT_STAT_IF_NOT_ZERO("HW  tx_broadcast:              %08x%08x\n",
                              stats_enet.tx_broadcast);
   WTE_PRINT_STAT_IF_NOT_ZERO("HW  tx_mac_pause:              %08x%08x\n",
                              stats_enet.tx_mac_pause);
   WTE_PRINT_STAT_IF_NOT_ZERO("HW  tx_defer:                  %08x%08x\n",
                              stats_enet.tx_defer);
   WTE_PRINT_STAT_IF_NOT_ZERO("HW  tx_excess_defer:           %08x%08x\n",
                              stats_enet.tx_excess_defer);
   WTE_PRINT_STAT_IF_NOT_ZERO("HW  tx_single_collision:       %08x%08x\n",
                              stats_enet.tx_single_collision);
   WTE_PRINT_STAT_IF_NOT_ZERO("HW  tx_multi_collision:        %08x%08x\n",
                              stats_enet.tx_multi_collision);
   WTE_PRINT_STAT_IF_NOT_ZERO("HW  tx_late_collision:         %08x%08x\n",
                              stats_enet.tx_late_collision);
   WTE_PRINT_STAT_IF_NOT_ZERO("HW  tx_excess_collision:       %08x%08x\n",
                              stats_enet.tx_excess_collision);
   WTE_PRINT_STAT_IF_NOT_ZERO("HW  tx_no_collision:           %08x%08x\n",
                              stats_enet.tx_no_collision);
   WTE_PRINT_STAT_IF_NOT_ZERO("HW  tx_mac_pause_honored:      %08x%08x\n",
                              stats_enet.tx_mac_pause_honored);
   WTE_PRINT_STAT_IF_NOT_ZERO("HW  tx_dropped:                %08x%08x\n",
                              stats_enet.tx_dropped);
   WTE_PRINT_STAT_IF_NOT_ZERO("HW  tx_jabber:                 %08x%08x\n",
                              stats_enet.tx_jabber);
   WTE_PRINT_STAT_IF_NOT_ZERO("HW  tx_err_fcs:                %08x%08x\n",
                              stats_enet.tx_err_fcs);
   WTE_PRINT_STAT_IF_NOT_ZERO("HW  tx_control:                %08x%08x\n",
                              stats_enet.tx_control);
   WTE_PRINT_STAT_IF_NOT_ZERO("HW  tx_oversize:               %08x%08x\n",
                              stats_enet.tx_oversize);
   WTE_PRINT_STAT_IF_NOT_ZERO("HW  tx_undersize:              %08x%08x\n",
                              stats_enet.tx_undersize);
   WTE_PRINT_STAT_IF_NOT_ZERO("HW  tx_fragments:              %08x%08x\n",
                              stats_enet.tx_fragments);


   WTE_PRINT_STAT_IF_NOT_ZERO("DPS rx_host_frames:            %08x%08x\n",
                              stats_enet.rx_host_frames);
   WTE_PRINT_STAT_IF_NOT_ZERO("DPS rx_iw_frames:              %08x%08x\n",
                              stats_enet.rx_iw_frames);
   WTE_PRINT_STAT_IF_NOT_ZERO("DPS rx_err_host_full:          %08x%08x\n",
                              stats_enet.rx_err_host_full);
   WTE_PRINT_STAT_IF_NOT_ZERO("DPS rx_err_fbp_underrun:       %08x%08x\n",
                              stats_enet.rx_err_fbp_underrun);
   WTE_PRINT_STAT_IF_NOT_ZERO("DPS rx_err_nonvalid_mac:       %08x%08x\n",
                              stats_enet.rx_err_nonvalid_mac);
   WTE_PRINT_STAT_IF_NOT_ZERO("DPS rx_err_mru:                %08x%08x\n",
                              stats_enet.rx_err_mru);
   WTE_PRINT_STAT_IF_NOT_ZERO("DPS rx_err_sdu:                %08x%08x\n",
                              stats_enet.rx_err_sdu);
   WTE_PRINT_STAT_IF_NOT_ZERO("DPS tx_err_underrun:           %08x%08x\n",
                              stats_enet.tx_err_underrun);
   WTE_PRINT_STAT_IF_NOT_ZERO("DPS rx_err_overrun:            %08x%08x\n",
                              stats_enet.rx_err_overrun);
   WTE_PRINT_STAT_IF_NOT_ZERO("DPS tx_frames:                 %08x%08x\n",
                              stats_enet.tx_frames);
   WTE_PRINT_STAT_IF_NOT_ZERO("DPS g999_rx_pause_frames:      %08x%08x\n",
                              stats_enet.g999_rx_pause_frames);
   WTE_PRINT_STAT_IF_NOT_ZERO("DPS g999_rx_errored_fragments: %08x%08x\n",
                              stats_enet.g999_rx_errored_fragments);
   WTE_PRINT_STAT_IF_NOT_ZERO("DPS g999_rx_length_error:      %08x%08x\n",
                              stats_enet.g999_rx_length_error);

   WTE_PRINT_STAT_IF_NOT_ZERO("DPS_IF tx_frames:              %08x%08x\n",
                              stats_enet.serial_dps_if_stats.tx_frames);
   WTE_PRINT_STAT_IF_NOT_ZERO("DPS_IF tx_bytes:               %08x%08x\n",
                              stats_enet.serial_dps_if_stats.tx_bytes);
   WTE_PRINT_STAT_IF_NOT_ZERO("DPS_IF rx_frames:              %08x%08x\n",
                              stats_enet.serial_dps_if_stats.rx_frames);
   WTE_PRINT_STAT_IF_NOT_ZERO("DPS_IF rx_bytes:               %08x%08x\n",
                              stats_enet.serial_dps_if_stats.rx_bytes);
   WTE_PRINT_STAT_IF_NOT_ZERO("DPS_IF rx_dropped_frames:      %08x%08x\n",
                              stats_enet.serial_dps_if_stats.rx_dropped_frames);
   WTE_PRINT_STAT_IF_NOT_ZERO("DPS_IF rx_pce_denied_frames:   %08x%08x\n",
                              stats_enet.serial_dps_if_stats.rx_pce_denied_frames);

   printf("\n");
   return;
} /* WTE_DisplayEnetDeviceStats */

void WTE_DisplayG999StreamStats(WP_handle stream_handle, WP_CHAR* type)
{
   WP_status status;
   WP_stats_g999_stream stats_enet_g999;

   printf ("%s statistics (%x)\n", type, stream_handle);
   printf ("============================================\n");
   memset( &stats_enet_g999, 0, sizeof(WP_stats_g999_stream) );
   status = WP_G999StreamStatistics(stream_handle, &stats_enet_g999);
   terminate_on_error(status, "WP_G999StreamStatistics()",__LINE__);

   WTE_PRINT_STAT_HIGH_LOW   ("DPS tx_frames:              %08x%08x\n",
                              stats_enet_g999.enet_multi_class_tx.tx_frames);

   WTE_PRINT_STAT_IF_NOT_ZERO("DPS tx_err_underrun:        %08x%08x\n",
                              stats_enet_g999.enet_multi_class_tx.tx_err_underrun);

   WTE_PRINT_STAT_IF_NOT_ZERO("DPS_TX_IF tx_frames:        %08x%08x\n",
                              stats_enet_g999.enet_multi_class_tx.serial_tx_dps_if_stats.tx_frames);
   WTE_PRINT_STAT_IF_NOT_ZERO("DPS_TX_IF tx_bytes:         %08x%08x\n",
                              stats_enet_g999.enet_multi_class_tx.serial_tx_dps_if_stats.tx_bytes);

   WTE_PRINT_STAT_IF_NOT_ZERO("DPS  rx_host_frames:        %08x%08x\n",
                              stats_enet_g999.rx_host_frames);
   WTE_PRINT_STAT_IF_NOT_ZERO("DPS  rx_iw_frames:          %08x%08x\n",
                              stats_enet_g999.rx_iw_frames);
   WTE_PRINT_STAT_IF_NOT_ZERO("DPS  rx_err_host_full:      %08x%08x\n",
                              stats_enet_g999.rx_err_host_full);
   WTE_PRINT_STAT_IF_NOT_ZERO("DPS  rx_err_fbp_underrun:   %08x%08x\n",
                              stats_enet_g999.rx_err_fbp_underrun);
   WTE_PRINT_STAT_IF_NOT_ZERO("DPS  rx_err_nonvalid_mac:   %08x%08x\n",
                              stats_enet_g999.rx_err_nonvalid_mac);
   WTE_PRINT_STAT_IF_NOT_ZERO("DPS  rx_err_mru:            %08x%08x\n",
                              stats_enet_g999.rx_err_mru);
   WTE_PRINT_STAT_IF_NOT_ZERO("DPS  rx_err_sdu:            %08x%08x\n",
                              stats_enet_g999.rx_err_sdu);
   WTE_PRINT_STAT_IF_NOT_ZERO("DPS  rx_err_overrun:        %08x%08x\n",
                              stats_enet_g999.rx_err_overrun);

   WTE_PRINT_STAT_IF_NOT_ZERO("DPS  rx_fragments:          %08x%08x\n",
                              stats_enet_g999.rx_fragments);
   WTE_PRINT_STAT_IF_NOT_ZERO("DPS  rx_bytes:              %08x%08x\n",
                              stats_enet_g999.rx_bytes);
   WTE_PRINT_STAT_IF_NOT_ZERO("DPS  rx_errored_fragments:  %08x%08x\n",
                              stats_enet_g999.rx_errored_fragments);
   printf("\n");
   return;
}

void WTE_DisplayPCEStats(WP_handle filter_handle,
                         WP_CHAR* type)
{
   WP_status status;
   WP_pce_filter_stat pce_stats;

   printf ("%s PCE statistics of filter (%x)\n",type, filter_handle);
   printf ("============================================\n");

   memset(&pce_stats, 0, sizeof(WP_pce_filter_stat) );

   if((status = WP_PceFilterStatistics(filter_handle, &pce_stats)) )
   {
      printf("PCE GetStatistics error\n");
      return;
   }
   WTE_PRINT_STAT_HIGH_LOW   ("matched_pkts:                 %08x%08x\n",
                              pce_stats.matched_pkts);
   WTE_PRINT_STAT_IF_NOT_ZERO("no_matched_pkts:              %08x%08x\n",
                              pce_stats.no_matched_pkts);
   WTE_PRINT_STAT_IF_NOT_ZERO("denied_pkts:                  %08x%08x\n",
                              pce_stats.denied_pkts);
} /* WTE_DisplayPCEStats */

void WTE_DisplayPCEGlobalStats(void)
{
   WP_status status = WP_OK;
   WP_pce_global_stat pce_global_stat;
   printf ("PCE Global statistics\n");
   printf ("============================================\n");
   memset(&pce_global_stat, 0, sizeof(WP_pce_global_stat) );
   status =  WP_PceGlobalStatistics(WP_WINPATH(DEFAULT_WPID),
                                    &pce_global_stat);

   WTE_PRINT_STAT_IF_NOT_ZERO("parser_header_access_errors:   %08x%08x\n",
                              pce_global_stat.parser_header_access_errors);
   WTE_PRINT_STAT_IF_NOT_ZERO("parser_timeout_err:            %08x%08x\n",
                              pce_global_stat.parser_timeout_err);
   WTE_PRINT_STAT_IF_NOT_ZERO("classifier_timeout_err:        %08x%08x\n",
                              pce_global_stat.classifier_timeout_err);
   WTE_PRINT_STAT_HIGH_LOW   ("parser_accept:                 %08x%08x\n",
                              pce_global_stat.parser_accept);
} /* WTE_DisplayPCEGlobalStats */

void WTE_DisplayFMU(void)
{
   WP_status status = WP_OK;

   /* print global parameters of the FMU. */
   printf ("***********************************************\n");
   printf ("\n\nWP_Display WP_DISPLAY_FMU\n");
   status = WP_Display(WP_WINPATH(DEFAULT_WPID),
                       WP_DISPLAY_FMU,
                       WP_DISPLAY_FMU_ONLY,
                       0);
   terminate_on_error(status, "WP_Display WP_DISPLAY_FMU", __LINE__);

#if SWAP_TO_ENET_DEVICE
   {
      /* print the parameters of the L1 channel. */
      printf ("***********************************************\n");
      printf ("\n\nWP_Display shaping_group_swap_l1[WT_WINNET_GROUP_A]\n");
      status = WP_Display(WP_WINPATH(DEFAULT_WPID),
                          shaping_group_swap_l1[WT_WINNET_GROUP_A],
                          WP_DISPLAY_FMU_ONLY,
                          0);
      terminate_on_error(status, "WP_Display shaping_group_swap_l1[WT_WINNET_GROUP_A]", __LINE__);


      printf ("***********************************************\n");
      printf ("\n\nWP_Display tx_channel_enet_dev[WT_WINNET_GROUP_A]\n");
      /* print the parameters of the L3 channel. */
      status = WP_Display(WP_WINPATH(DEFAULT_WPID),
                          tx_channel_enet_dev[WT_WINNET_GROUP_A],
                          WP_DISPLAY_FMU_ONLY|WP_DISPLAY_FMU_L3_ONLY,
                          0);
      terminate_on_error(status, "WP_Display tx_channel_enet_dev[WT_WINNET_GROUP_A]", __LINE__);
   }
#else
   {
      WP_S32 ii;
      for (ii=0 ; ii<device_enet_g999_cfg[0].max_g999_streams ; ii++)
      {
         /* print the parameters of the L1 channel. */
         printf ("***********************************************\n");
         printf ("\n\nWP_Display shaping_group_l1[%d]\n", ii);
         status = WP_Display(WP_WINPATH(DEFAULT_WPID),
                             shaping_group_l1[ii],
                             WP_DISPLAY_FMU_ONLY,
                             0);
         terminate_on_error(status, "WP_Display shaping_group_l1", __LINE__);
      }

      for (ii=0 ; ii<device_enet_g999_cfg[0].max_g999_streams ; ii++)
      {
         printf ("***********************************************\n");
         printf ("\n\nWP_Display tx_channel_stream[%d]\n", ii);
         /* print the parameters of the L3 channel. */
         status = WP_Display(WP_WINPATH(DEFAULT_WPID),
                             tx_channel_stream[ii],
                             WP_DISPLAY_FMU_ONLY|WP_DISPLAY_FMU_L3_ONLY,
                             0);
         terminate_on_error(status, "WP_Display tx_channel_stream[ii]", __LINE__);
      }
   }
#endif

} /* WTE_DisplayFMU */

void WTE_DisplayPceFilters (void)
{
   WP_status status;
   printf ("***********************************************\n");
   printf("\n\nWP_Display pce_device_filter");
   status = WP_Display(WP_WINPATH(DEFAULT_WPID),
                       pce_device_filter,
                       WP_DISPLAY_DEFAULT,
                       0);
   terminate_on_error(status, "WP_Display pce_filters[0]", __LINE__);

   printf ("***********************************************\n");
   printf("\n\nWP_Display pce_rx_channel_filter");
   status = WP_Display(WP_WINPATH(DEFAULT_WPID),
                       pce_rx_channel_filter,
                       WP_DISPLAY_DEFAULT,
                       0);
   terminate_on_error(status, "WP_Display pce_rx_channel_filter", __LINE__);

} /* WTE_DisplayPceFilters */

void WTE_DisplayPceRules (void)
{
   WP_S32 ii = 0;
   WP_CHAR temp_string[256];
   WP_status status;

   for (ii=0 ; ii<device_enet_g999_cfg[0].max_g999_streams ; ii++)
   {
      printf ("***********************************************\n");
      printf ("\n\nWP_Display pce_device_rules[%d] 0x%x\n", ii, pce_device_rules[ii]);
      status = WP_Display(WP_WINPATH(DEFAULT_WPID),
                          pce_device_rules[ii],
                          WP_DISPLAY_DEFAULT,
                          0);
      sprintf (temp_string, "WP_Display pce_device_rules[%d] 0x%x", ii, pce_device_rules[ii]);
      terminate_on_error(status, temp_string, __LINE__);
   }

   for (ii=0 ; ii<device_enet_g999_cfg[0].max_g999_streams ; ii++)
   {
      printf ("***********************************************\n");
      printf ("\n\nWP_Display pce_rx_channel_rules_to_host[%d] 0x%x\n", ii, pce_rx_channel_rules_to_host[ii]);
      status = WP_Display(WP_WINPATH(DEFAULT_WPID),
                          pce_rx_channel_rules_to_host[ii],
                          WP_DISPLAY_DEFAULT,
                          0);
      sprintf (temp_string, "WP_Display pce_rx_channel_rules_to_host[%d] 0x%x", ii, pce_rx_channel_rules_to_host[ii]);
      terminate_on_error(status, temp_string, __LINE__);
   }

   for (ii=0 ; ii<device_enet_g999_cfg[0].max_g999_streams ; ii++)
   {
      printf ("***********************************************\n");
      printf ("\n\nWP_Display pce_rx_channel_rules_to_enet_from_compatible[%d] 0x%x\n", ii, pce_rx_channel_rules_to_enet_from_compatible[ii]);
      status = WP_Display(WP_WINPATH(DEFAULT_WPID),
                          pce_rx_channel_rules_to_enet_from_compatible[ii],
                          WP_DISPLAY_DEFAULT,
                          0);
      sprintf (temp_string, "WP_Display pce_rx_channel_rules_to_enet_from_compatible[%d] 0x%x", ii, pce_rx_channel_rules_to_enet_from_compatible[ii]);
      terminate_on_error(status, temp_string, __LINE__);
   }

   for (ii=0 ; ii<device_enet_g999_cfg[0].max_g999_streams ; ii++)
   {
      printf ("***********************************************\n");
      printf ("\n\nWP_Display pce_rx_channel_rules_to_enet_from_generic[%d] 0x%x\n", ii, pce_rx_channel_rules_to_enet_from_generic[ii]);
      status = WP_Display(WP_WINPATH(DEFAULT_WPID),
                          pce_rx_channel_rules_to_enet_from_generic[ii],
                          WP_DISPLAY_DEFAULT,
                          0);
      sprintf (temp_string, "WP_Display pce_rx_channel_rules_to_enet_from_generic[%d] 0x%x", ii, pce_rx_channel_rules_to_enet_from_generic[ii]);
      terminate_on_error(status, temp_string, __LINE__);
   }

} /* WTE_DisplayPceRules */

void WTE_DisplayPceFilterSets (void)
{
   WP_status status;
   printf ("***********************************************\n");
   printf ("\n\nWP_Display rx_channel_filterset_handle 0x%x\n", rx_channel_filterset_handle);
   status = WP_Display(WP_WINPATH(DEFAULT_WPID),
                       rx_channel_filterset_handle,
                       WP_DISPLAY_DEFAULT,
                       0);
   terminate_on_error(status, "WP_Display() rx_channel_filterset_handle", __LINE__);

   printf ("***********************************************\n");
   printf ("\n\nWP_Display device_filterset_handle 0x%x\n", device_filterset_handle);
   status = WP_Display(WP_WINPATH(DEFAULT_WPID),
                       device_filterset_handle,
                       WP_DISPLAY_DEFAULT,
                       0);
   terminate_on_error(status, "WP_Display() device_filterset_handle", __LINE__);
} /* WTE_DisplayPceFilterSets */

void WTE_DisplayDevices(void)
{
   WP_status status;
   printf ("***********************************************\n");
   printf ("\n\nWP_Display group_b_enet_dev 0x%x\n", group_b_enet_dev);
   status = WP_Display(WP_WINPATH(DEFAULT_WPID),
                       group_b_enet_dev,
                       WP_DISPLAY_DEFAULT,
                       0);
   terminate_on_error(status, "WP_Display() group_b_enet_dev", __LINE__);

#if SWAP_TO_ENET_DEVICE
   printf ("***********************************************\n");
   printf ("\n\nWP_Display group_a_enet_dev (former group_a_g999_dev) 0x%x\n", group_a_enet_dev);
   status = WP_Display(WP_WINPATH(DEFAULT_WPID),
                       group_a_enet_dev,
                       WP_DISPLAY_DEFAULT,
                       0);
   terminate_on_error(status, "WP_Display() group_a_enet_dev", __LINE__);
#else
   printf ("***********************************************\n");
   printf ("\n\nWP_Display group_a_g999_dev 0x%x\n", group_a_g999_dev);
   status = WP_Display(WP_WINPATH(DEFAULT_WPID),
                       group_a_g999_dev,
                       WP_DISPLAY_DEFAULT,
                       0);
   terminate_on_error(status, "WP_Display() group_a_g999_dev", __LINE__);
#endif
}

void WTE_DisplayStreams(void)
{
   WP_CHAR temp_string[256];
   WP_S32 ii = 1;
   WP_status status;

   for (ii=0 ; ii<2/*device_enet_g999_cfg[0].max_g999_streams*/ ; ii++)
   {
      printf ("***********************************************\n");
      printf ("\n\nWP_Display G999 Stream 0x%x\n", g999_stream_handle[ii]);
      status = WP_Display(WP_WINPATH(DEFAULT_WPID),
                          g999_stream_handle[ii],
                          WP_DISPLAY_DEFAULT | WP_DISPLAY_RAW_DPS_STRUCTURES,
                          0);
      sprintf (temp_string, "WP_Display() g999_stream_handle[%d] 0x%x", ii, g999_stream_handle[ii]);
      terminate_on_error(status, temp_string, __LINE__);
   }
}

void WTE_DisplayQnodeStatus(void)
{
   WP_U32 result;
   WP_status status;

   status = WP_QNodeStatus(qniw_enet, WP_QNODE_STATUS_FIFO, &result);
   terminate_on_error(status, "WP_QNodeStatus()",__LINE__);
   printf ("\nQNode qniw_enet Num Of Buffers (%x)  %d\n", qniw_enet, result);
   printf ("Original num_buffers = %d\n", qnode_iwq_cfg[0].num_buffers);

   status = WP_QNodeStatus(qniw_host, WP_QNODE_STATUS_FIFO, &result);
   terminate_on_error(status, "WP_QNodeStatus()",__LINE__);
   printf ("\nQNode qniw_host Num Of Buffers (%x)  %d\n", qniw_host, result);
   printf ("Original num_buffers = %d\n", qnode_iw_host_cfg[0].num_buffers);
   return;
} /* WTE_DisplayQnodeStatus */

void WTE_DisplayStats(void)
{
#if (!SWAP_TO_ENET_DEVICE)
   WP_CHAR temp_string[256];
   WP_S32 ii;
#endif

   /* Print the statistics */
   WTE_DisplayIwPortStats(bport_host, WP_FALSE, "bport_host");
   WTE_DisplayIwPortStats(bport_iw_enet[WT_WINNET_GROUP_A], WP_FALSE, "bport_iw_enet[WT_WINNET_GROUP_A]");
#if (SWAP_TO_ENET_DEVICE)
   WTE_DisplayIwPortStats(bport_host_tx_swapped_enet[WT_WINNET_GROUP_A], WP_FALSE, "bport_host_tx_swapped_enet[WT_WINNET_GROUP_A]");
#endif
   WTE_DisplayFlowStats(flow_agg_host[WT_WINNET_GROUP_A], "flow_agg_host[WT_WINNET_GROUP_A]");

#if (!SWAP_TO_ENET_DEVICE)
   for (ii=0 ; ii<device_enet_g999_cfg[0].max_g999_streams ; ii++)
   {
      sprintf (temp_string, "flow_agg_compatible_stream[%d]", ii);
      WTE_DisplayFlowStats(flow_agg_compatible_stream[ii], temp_string);
   }
#else
      WTE_DisplayFlowStats(flow_agg_compatible_enet[WT_WINNET_GROUP_A], "flow_agg_compatible_enet[WT_WINNET_GROUP_A]");
#endif
#if (!SWAP_TO_ENET_DEVICE)
   for (ii=0 ; ii<device_enet_g999_cfg[0].max_g999_streams ; ii++)
   {
      sprintf (temp_string, "g999_stream_handle[%d]", ii);
      WTE_DisplayG999StreamStats(g999_stream_handle[ii], temp_string);
   }
#endif


#if SWAP_TO_ENET_DEVICE
   WTE_DisplayEnetDeviceStats(group_a_enet_dev, "group_a_enet_dev");
   WTE_DisplayEnetDeviceStats(group_b_enet_dev, "group_b_enet_dev");
#else
   WTE_DisplayEnetDeviceStats(group_a_g999_dev, "group_a_g999_dev");
   WTE_DisplayEnetDeviceStats(group_b_g999_dev, "group_b_g999_dev");
#endif
   WTE_DisplayPCEStats(pce_device_filter, "pce_device_filter");
   WTE_DisplayPCEStats(pce_rx_channel_filter, "pce_rx_channel_filter");
   WTE_DisplayPCEStats(pce_iw_tx_channel_filter, "pce_iw_tx_channel_filter");

   WTE_DisplayPCEGlobalStats();

} /* WTE_DisplayStats */


void WTE_ResetStats(void)
{
   WP_CHAR temp_string[256];
   WP_S32 ii;

   printf ("\nResetting ALL statistics... \n\n");
   /* Reset the statistics */
   WTE_ResetIwPortStats(bport_host, WP_FALSE);
   WTE_ResetIwPortStats(bport_iw_enet[WT_WINNET_GROUP_A], WP_FALSE);
   for (ii=0 ; ii<device_enet_g999_cfg[0].max_g999_streams ; ii++)
   {
      sprintf (temp_string, "bport_iw_stream[%d]", ii);
      WTE_ResetIwPortStats(bport_iw_stream[ii], WP_FALSE);
   }

   WTE_ResetFlowStats(flow_agg_host[WT_WINNET_GROUP_A]);
   for (ii=0 ; ii<device_enet_g999_cfg[0].max_g999_streams ; ii++)
   {
      sprintf (temp_string, "flow_agg_compatible_stream[%d]", ii);
      WTE_ResetFlowStats(flow_agg_compatible_stream[ii]);
   }

   for (ii=0 ; ii<device_enet_g999_cfg[0].max_g999_streams ; ii++)
   {
      sprintf (temp_string, "g999_stream_handle[%d]", ii);
      WTE_ResetG999StreamStats(g999_stream_handle[ii]);
   }

   WTE_ResetEnetDeviceStats(group_b_enet_dev);
   WTE_ResetEnetDeviceStats(group_a_g999_dev);

   WTE_ResetPCEStats(pce_device_filter);
   WTE_ResetPCEStats(pce_rx_channel_filter);
   WTE_ResetPCEStats(pce_iw_tx_channel_filter);

   WTE_ResetPCEGlobalStats();

   printf ("DONE !!! \n\n");
} /* WTE_ResetStats */



void F_IwDataUnitsSetup(WP_CHAR* dest, WP_U32 data_length)
{
   WP_U8 *km_data_ptr=NULL;
   WP_CHAR ascii_buffer[MAX_DATA_LENGTH*4+1];
   WP_CHAR hexa_buffer[MAX_DATA_LENGTH*2];
   WP_S32 ii;

   /* Host Send  */
   generator_data_unit[0].data_size = data_length;
   generator_data_unit[0].n_active = 1;

   km_tx_buf[0].data_size = generator_data_unit[0].data_size;
   km_data_ptr = WP_PoolAlloc( iw_pool_host);

   km_tx_buf[0].data = km_data_ptr;
   km_tx_buf[0].pool_handle = iw_pool_host;
   km_tx_buf[0].next = NULL;
   km_tx_buf[0].displacement = 0;
   generator_data_unit[0].segment = km_tx_buf;
   generator_data_unit[0].n_segments = 1;
   generator_data_unit[0].type = WP_DATA_IW;
   generator_data_unit[0].control = 0;
   strcpy (ascii_buffer, dest);
   strcat (ascii_buffer,"000000000010"); /* src mac */
   strcat (ascii_buffer,"8100");
   strcat (ascii_buffer,"0005"); /* vlan */
   strcat (ascii_buffer,"0800");
   strcat (ascii_buffer,"45040044");/* tos = 4 */
   strcat (ascii_buffer,"001b0000");
   strcat (ascii_buffer,"0111907d");/* protocol = 0x11*/
   strcat (ascii_buffer,"96780000");/* sa ip */
   strcat (ascii_buffer,"00000000");/* Da ip */
   strcat (ascii_buffer,"00080008");
   strcat (ascii_buffer,"001b77c0");
   for (ii=0 ; ii<13 ; ii++)
   {
      strcat (ascii_buffer,"00010203040506070809101112131415161718192021222324252627282930313233343536373839");
      strcat (ascii_buffer,"40414243444546474849505152535455565758596061626364656667686970717273747576777879");
      strcat (ascii_buffer,"8081828384858687888990919293949596979899a0a1a2a3a4a5a6a7a8a9b0b1b2b3b4b5b6b7b8b9");
      strcat (ascii_buffer,"c0c1c2c3c4c5c6c7c8c9d0d1d2d3d4d5d6d7d8d9e0e1e2e3e4e5e6e7e8e9f0f1f2f3f4f5f6f7f8f9");
   }

   App_TranslateAsciiToHex(hexa_buffer, ascii_buffer, MAX_DATA_LENGTH);

   WP_MEM_BYTES_FILL(km_data_ptr, 0xdb, pool_buffer_data_2048_cfg[0].size+200);

   WP_MEM_BYTES_SET(km_data_ptr, hexa_buffer, MAX_DATA_LENGTH);
   return;
} /* F_IwDataUnitsSetup */

/**
 *
 */
void WTE_MiscNormalModeHostSend( WP_CHAR *dest,
                                 WP_handle tx_channel_handle,
                                 WP_U32 data_length,
                                 WP_boolean print_frame,
                                 WP_U32 line)
{
   WP_status status;

   memset (generator_data_unit, 0, sizeof (WP_data_unit));
   memset (km_tx_buf, 0, sizeof (WP_data_segment));
   F_IwDataUnitsSetup( dest, data_length);

   generator_data_unit[0].control = (WP_HT_IW_B_MODE_FIELD(WP_HT_IW_B_NORMAL_MODE)|
                                     WP_HT_IW_CRC_FIELD(0));
   if (print_frame == WP_TRUE) /* DEBUG */
   {
      WP_U32 ii;
      for( ii=0; ii < generator_data_unit[0].segment->data_size; ii++)
      {
         if (! (ii % 4) )
            printf ("\n0x%04x 0x%02x%02x%02x%02x", ii,
                    generator_data_unit[0].segment->data[ii+0],
                    generator_data_unit[0].segment->data[ii+1],
                    generator_data_unit[0].segment->data[ii+2],
                    generator_data_unit[0].segment->data[ii+3]);
      }
      printf("( %d bytes )\n", generator_data_unit[0].segment->data_size);
   }
   status = WP_HostSend(tx_channel_handle, generator_data_unit);
   terminate_on_error( status, "WP_HostSend()", line);

   generator_data_unit[0].n_active = 0;

   WP_Delay(WT_RECEIVE_DELAY);

   return;
} /* WTE_MiscNormalModeHostSend */


#define NUM_OF_BUFS 10
WP_data_segment bufs[NUM_OF_BUFS];

void WTE_ResetHandles(void)
{
   WP_U32 ii;

   for(ii=0 ; ii<NUM_OF_BUFS ; ii++)
   {
      bufs[ii].next = &bufs[ii+1];
      bufs[ii].displacement = 0;
   }
   bufs[NUM_OF_BUFS -1].next = NULL;
}

void WTE_ReceiveOnG999DevChannel(WP_handle rx_channel_g999)
{
   WP_status status;
   WP_boolean frame_check_failed;
   WTE_ResetHandles();
   memset (data_unit, 0, sizeof (WP_data_unit));
   data_unit->segment = &bufs[0];
   data_unit->n_segments = NUM_OF_BUFS;
   data_unit->n_active = NUM_OF_BUFS;
   data_unit->type = WP_DATA_ENET;
   status = WP_HostReceive (rx_channel_g999, data_unit);
   if (WP_OK == status )
   {
      printf ("Frame received.\n");
   }
   else
   {
      printf("%s\n", WP_ErrorString(status));
      return;
   }
   frame_check_failed = WTE_CheckFrame(status);
   if (frame_check_failed == WP_TRUE)
      printf ("Scenario Failed\n");
   else
      printf ("Scenario Passed\n");
   return;
}

void WTE_ReceiveOnG999StreamChannel(void)
{
   WP_CHAR InputBuf[256];
   WP_S32 stream_index;
   WP_status status;
   WP_boolean frame_check_failed;
   WTE_ResetHandles();
   memset (data_unit, 0, sizeof (WP_data_unit));
   data_unit->segment = &bufs[0];
   data_unit->n_segments = NUM_OF_BUFS;
   data_unit->n_active = NUM_OF_BUFS;
   data_unit->type = WP_DATA_ENET;
   printf("\t Select Stream index\n");
   gets(InputBuf);
   stream_index = atoi(InputBuf);
   printf("\n");
   if (stream_index >= device_enet_g999_cfg[0].max_g999_streams)
   {
      printf ("BAD STREAM INDEX\n");
      return;
   }
   status = WP_HostReceive (rx_channel_stream[stream_index], data_unit);
   if (WP_OK == status )
   {
      printf ("Frame received.\n");
   }
   else
   {
      printf("%s\n", WP_ErrorString(status));
      return;
   }

   frame_check_failed = WTE_CheckFrame(status);
   if (frame_check_failed == WP_TRUE)
      printf ("Scenario Failed\n");
   else
      printf ("Scenario Passed\n");
   return;
}

WP_boolean WTE_ReceiveOnIwHostChannel(void)
{
   WP_status status;
   WP_boolean frame_check_passed;
   WTE_ResetHandles();

   memset (data_unit, 0, sizeof (WP_data_unit));
   data_unit->segment = &bufs[0];
   data_unit->n_segments = NUM_OF_BUFS;
   data_unit->n_active = NUM_OF_BUFS;
   data_unit->type = WP_DATA_IW;
   status = WP_HostReceive (iw_rx_ch, data_unit);
   if (WP_OK == status)
   {
      MY_PRINTF("Frame received.\n",0);
      frame_check_passed = WTE_CheckFrame(status);
   }
   else
   {
      printf("%s\n", WP_ErrorString(status));
      frame_check_passed = WP_FALSE;
   }

   if (frame_check_passed == WP_FALSE)
   {
      MY_PRINTF ("Scenario Failed\n", 0);
   }

   return frame_check_passed;
}

WP_boolean WTE_Test_System(WP_U8 swap_mode,
                           WP_U32 start_index,
                           WP_S32 frame_index,
                           WP_U8 group_num)
{
   WP_S32 stream_index = 0;
   WP_CHAR dest_mac[100];
   WP_boolean print_frame = 0;
   WP_S32 ii = 0, expected_packets = 0;
   WP_iw_flow_stats iw_flow_stats;
   WP_status status;
   WP_boolean scenario_passed = WP_TRUE;
   WP_CHAR temp_string[100];
   WP_boolean frame_check_passed = WP_TRUE;

#if (DEBUG_PRINT)
   print_frame = WP_TRUE;
#endif
   if (swap_mode == WT_SWAP_MODE_G999)
   {
      expected_packets = max_g999_streams_in_use[group_num];
      for (ii=0 ; ii < max_g999_streams_in_use[group_num]; ii++)
      {
         stream_index = ii+start_index;
         sprintf (dest_mac, "1234567800%02x", stream_index);
         WTE_MiscNormalModeHostSend(dest_mac,
                                    iw_tx_ch[stream_index],
                                    sent_frame_length[frame_index],
                                    print_frame,
                                    __LINE__);
         frame_check_passed = WTE_ReceiveOnIwHostChannel();
         if ( ! frame_check_passed )
         {
            printf ("%s %d: Scenario FAILED frame #%d\n", __FUNCTION__, __LINE__,ii);
            scenario_passed = WP_FALSE;
         }
      }
   }
   else /* (swap_mode == WT_SWAP_MODE_ENET) */
   {
      expected_packets = WT_DEFAULT_NUM_OF_STREAMS;

      for (ii=0 ; ii<WT_DEFAULT_NUM_OF_STREAMS ; ii++)
      {
         /* stream_index is always 0 */
         sprintf (dest_mac, "1234567800%02x", stream_index);
         WTE_MiscNormalModeHostSend( dest_mac,
                                     iw_tx_ch_enet[group_num],
                                     sent_frame_length[frame_index],
                                     print_frame,
                                     __LINE__);

         frame_check_passed = WTE_ReceiveOnIwHostChannel();
         if ( ! frame_check_passed )
         {
            scenario_passed = WP_FALSE;
         }
      }
   }

   /**********************************/
   /* Check host flow agg statistics */
   /**********************************/
   memset(&iw_flow_stats,0,sizeof(WP_iw_flow_stats));
   status = WP_IwFlowStatistics( flow_agg_host[group_num], WP_IW_FLOW_STAT, &iw_flow_stats);
   terminate_on_error(status, "WP_IwFlowStatistics()",__LINE__);

#if (DEBUG_PRINT)
   sprintf (temp_string, "flow_agg_host[%d]", group_num );
   WTE_DisplayFlowStats(flow_agg_host[group_num], temp_string);
#endif

   if (iw_flow_stats.forward_packet == expected_packets)
   {
      scenario_passed = WP_TRUE;
      return scenario_passed;
   }
   else
   {
      printf ("\nEXPECTED %d PACKETS\n", expected_packets);
      printf ("\nRECEIVED %d PACKETS\n", (WP_U32)(iw_flow_stats.forward_packet));
      sprintf (temp_string, "flow_agg_host[%d]", group_num );

      WTE_DisplayFlowStats(flow_agg_host[group_num], temp_string);

      printf ("\nScenario Failed\n");
      scenario_passed = WP_FALSE;
      return scenario_passed;
   }
}

WP_boolean WTE_CheckFrame(WP_status status)
{
   WP_boolean print_frame = WP_FALSE;
   WP_boolean frame_check_passed = WP_TRUE;
   WP_boolean compare_frame = WP_TRUE;

   if (status == WP_OK)
   {
      WP_U16 ii, jj;
      WP_U8   data_buf[MAX_DATA_LENGTH];
      WP_boolean check_failed = WP_FALSE;
      WP_U16 max_size = 0, min_size;
      WP_U16 received_data_size = 0;

      MY_PRINTF("\n", 0);
      memset (data_buf, 0, sizeof (WP_CHAR) * MAX_DATA_LENGTH);

      for (jj=0 ; jj<data_unit->n_active ; jj++)
      {
         WP_MEM_BYTES_GET(&(data_buf[received_data_size]), data_unit->segment[jj].data, data_unit->segment[jj].data_size);
         received_data_size += data_unit->segment[jj].data_size;
      }
      MY_PRINTF ("Received %d (0x%x) Bytes\n", received_data_size, received_data_size);

#if (DEBUG_PRINT)
      print_frame = WP_TRUE;
#endif
      MY_PRINTF("\n", 0);

      if (compare_frame)
      {
         if (print_frame == WP_TRUE) printf ("Offset \t Sent    \t Received\n");
         if (generator_data_unit[0].segment->data_size > received_data_size)
         {
            max_size = generator_data_unit[0].segment->data_size;
            min_size = received_data_size;
         }
         else
         {
            max_size = received_data_size;
            min_size = generator_data_unit[0].segment->data_size;
         }

         for( ii=0; ii <max_size ; ii++)
         {
            if (ii > min_size)
               check_failed = WP_FALSE; /* when passing min_size there is nothing to compare */
            else if (generator_data_unit[0].segment->data[ii] != data_buf[ii])
               check_failed = WP_TRUE;
            if (print_frame == WP_TRUE) /* DEBUG */
            {
               if ( ! (ii % 4) )
               {
                  printf ("0x%04x \t 0x%02x%02x%02x%02x \t 0x%02x%02x%02x%02x \t",
                          ii,
                          generator_data_unit[0].segment->data[ii+0],
                          generator_data_unit[0].segment->data[ii+1],
                          generator_data_unit[0].segment->data[ii+2],
                          generator_data_unit[0].segment->data[ii+3],
                          data_buf[ii+0],
                          data_buf[ii+1],
                          data_buf[ii+2],
                          data_buf[ii+3]);
                  if (check_failed)
                  {
                     printf ("***FAILED\n");
                  }
                  else
                     printf ("PASSED\n");
               }

               if (check_failed)
               {
                  frame_check_passed = WP_FALSE;
               }
               check_failed = WP_FALSE;
            }
         }
         if (print_frame == WP_TRUE)
         {
            printf(" \t %d (0x%x) bytes \t %d (0x%x) bytes\n",
                   generator_data_unit[0].segment->data_size,
                   generator_data_unit[0].segment->data_size,
                   received_data_size,
                   received_data_size);
         }
      }
      else /*  (compare_frame) */
      {
         if (print_frame == WP_TRUE) printf ("Offset \t Received\n");
         max_size = received_data_size;

         for( ii=0; ii <max_size ; ii++)
         {
            if ( ! (ii % 4) )
            {
               if (print_frame == WP_TRUE)
               {
                  printf ("0x%04x \t 0x%02x%02x%02x%02x \n",
                          ii,
                          data_buf[ii+0],
                          data_buf[ii+1],
                          data_buf[ii+2],
                          data_buf[ii+3]);
               }
            }
         }
         if (print_frame == WP_TRUE)
         {
            printf(" \t %d (0x%x) bytes\n",
                   received_data_size,
                   received_data_size);
         }
      } /*  (compare_frame) */

      for (ii=0 ; ii<data_unit->n_active ; ii++)
      {
         status = WP_PoolFree(data_unit->segment[ii].pool_handle, data_unit->segment[ii].data);
         terminate_on_error(status, "WP_PoolFree", __LINE__);
      }
   }
   return frame_check_passed;
}
void WTE_CreateGroupAG999Mode(void)
{
   WTE_CreateStreamsGroupA();
   WTE_CreatePceRulesDeviceGroupA();

   WTE_GroupsCreateGroupA(WT_SWAP_MODE_G999);
   WTE_CreateChannelsGroupA(WT_SWAP_MODE_G999);

   WTE_CreateIwSystemAndItsComponentsGroupA(WT_SWAP_MODE_G999);

   WTE_EnablePortsDevicesChannelsGroupA(WT_SWAP_MODE_G999);
}

void WTE_DeleteGroupAG999Mode(void)
{
   WTE_ResetStatisticsGroupA(WT_SWAP_MODE_G999);
   WTE_SystemDeleteGroupA(WT_SWAP_MODE_G999);
}

void WTE_CreateGroupAEnetMode(void)
{
   WTE_GroupsCreateGroupA(WT_SWAP_MODE_ENET);
   WTE_CreateChannelsGroupA(WT_SWAP_MODE_ENET);
   WTE_CreateIwSystemAndItsComponentsGroupA(WT_SWAP_MODE_ENET);
   WTE_EnablePortsDevicesChannelsGroupA(WT_SWAP_MODE_ENET);
}

void WTE_DeleteGroupAEnetMode(void)
{
   WTE_ResetStatisticsGroupA(WT_SWAP_MODE_ENET);
   WTE_SystemDeleteGroupA(WT_SWAP_MODE_ENET);
}

void WTE_CreateGroupBG999Mode(void)
{
   WTE_CreateStreamsGroupB();
   WTE_CreatePceRulesDeviceGroupB();

   WTE_GroupsCreateGroupB(WT_SWAP_MODE_G999);
   WTE_CreateChannelsGroupB(WT_SWAP_MODE_G999);
   WTE_CreateIwSystemAndItsComponentsGroupB(WT_SWAP_MODE_G999);

   WTE_EnablePortsDevicesChannelsGroupB(WT_SWAP_MODE_G999);
}

void WTE_DeleteGroupBG999Mode(void)
{
   WTE_ResetStatisticsGroupB(WT_SWAP_MODE_G999);
   WTE_SystemDeleteGroupB(WT_SWAP_MODE_G999);
}

void WTE_CreateGroupBEnetMode(void)
{
   WTE_GroupsCreateGroupB(WT_SWAP_MODE_ENET);
   WTE_CreateChannelsGroupB(WT_SWAP_MODE_ENET);
   WTE_CreateIwSystemAndItsComponentsGroupB(WT_SWAP_MODE_ENET);
   WTE_EnablePortsDevicesChannelsGroupB(WT_SWAP_MODE_ENET);
}

void WTE_DeleteGroupBEnetMode(void)
{
   WTE_ResetStatisticsGroupB(WT_SWAP_MODE_ENET);
   WTE_SystemDeleteGroupB(WT_SWAP_MODE_ENET);
}

void WTE_SwapGroupAToEnetMode(void)
{
   WP_status status;
   /* swap to Enet device */
   status = WP_DeviceModify(group_a_enet_dev,
                            WP_DEV_MOD_G999_ENET_SWAP,
                            device_enet_cfg);
   terminate_on_error(status, "WP_DeviceModify group_a_enet_dev", __LINE__);
}

void WTE_SwapGroupBToEnetMode(void)
{
   WP_status status;
   /* swap to Enet device */
   status = WP_DeviceModify(group_b_enet_dev,
                            WP_DEV_MOD_G999_ENET_SWAP,
                            device_enet_cfg);
   terminate_on_error(status, "WP_DeviceModify group_b_enet_dev", __LINE__);
}

void WTE_SwapGroupAToG999Mode(WP_U32 max_g999_streams)
{
   WP_status status;
   /* swap to G999 device */
   device_enet_g999_cfg[0].max_g999_streams = max_g999_streams;
   status = WP_DeviceModify(group_a_g999_dev,
                            WP_DEV_MOD_G999_ENET_SWAP,
                            device_enet_g999_cfg);
   terminate_on_error(status, "WP_DeviceModify group_a_g999_dev", __LINE__);
}

void WTE_SwapGroupBToG999Mode(WP_U32 max_g999_streams)
{
   WP_status status;
   /* swap to G999 device */
   device_enet_g999_cfg[0].max_g999_streams = max_g999_streams;
   status = WP_DeviceModify(group_b_g999_dev,
                            WP_DEV_MOD_G999_ENET_SWAP,
                            device_enet_g999_cfg);
   terminate_on_error(status, "WP_DeviceModify group_b_g999_dev", __LINE__);
}

WP_boolean WTE_Scenario_1(void)
{
   WP_boolean all_scenario_passed = WP_TRUE;
   WP_boolean scenario_passed = WP_TRUE;
   WP_S32 frame_index;

   max_g999_streams_in_use[WT_WINNET_GROUP_A] = 48;
   max_g999_streams_in_use[WT_WINNET_GROUP_B] = 0;
   printf ("\t\t SCENARIO 1: WT_WINNET_GROUP_A G999 %d Streams  WT_WINNET_GROUP_B ENET\n", max_g999_streams_in_use[WT_WINNET_GROUP_A]);

   for (frame_index=0 ; frame_index<NUM_OF_FRAMES ; frame_index++)
   {
      WTE_SwapGroupAToG999Mode(max_g999_streams_in_use[WT_WINNET_GROUP_A]);
      WTE_SwapGroupBToEnetMode();
      WTE_CreateGroupBEnetMode();
      WTE_CreateGroupAG999Mode();

      scenario_passed = WTE_Test_System(WT_SWAP_MODE_G999, 0, frame_index, WT_WINNET_GROUP_A);
      if (!scenario_passed) all_scenario_passed = WP_FALSE;

      scenario_passed = WTE_Test_System(WT_SWAP_MODE_ENET, 0, frame_index, WT_WINNET_GROUP_B);
      if (!scenario_passed) all_scenario_passed = WP_FALSE;

      scenario_passed = WTE_Test_System(WT_SWAP_MODE_ENET, 0, frame_index, WT_DUMMY_PORT_1);
      if (!scenario_passed) all_scenario_passed = WP_FALSE;

      scenario_passed = WTE_Test_System(WT_SWAP_MODE_ENET, 0, frame_index, WT_DUMMY_PORT_2);
      if (!scenario_passed) all_scenario_passed = WP_FALSE;
#if (!DEBUG_SMAP)
      WTE_DeleteGroupAG999Mode();
      WTE_DeleteGroupBEnetMode();
      WTE_ResetStatisticsDummyPort1();
      WTE_ResetStatisticsDummyPort2();
#endif
   }
   if (all_scenario_passed)
      printf ("SCENARIO 1 PASSED\n");
   else
      printf ("SCENARIO 1 FAILED\n");
   return all_scenario_passed;
}

WP_boolean WTE_Scenario_2(void)
{
   WP_boolean all_scenario_passed = WP_TRUE;
   WP_boolean scenario_passed = WP_TRUE;
   WP_S32 frame_index;

   max_g999_streams_in_use[WT_WINNET_GROUP_A] = WT_DEFAULT_NUM_OF_STREAMS;
   max_g999_streams_in_use[WT_WINNET_GROUP_B] = WT_DEFAULT_NUM_OF_STREAMS;
   printf ("\t\t SCENARIO 2: WT_WINNET_GROUP_A G999 %d Streams  WT_WINNET_GROUP_B G999 %d STREAMS\n", max_g999_streams_in_use[WT_WINNET_GROUP_A], max_g999_streams_in_use[WT_WINNET_GROUP_B]);

   for (frame_index=0 ; frame_index<NUM_OF_FRAMES ; frame_index++)
   {
      WTE_SwapGroupAToG999Mode(max_g999_streams_in_use[WT_WINNET_GROUP_A]);
      WTE_SwapGroupBToG999Mode(max_g999_streams_in_use[WT_WINNET_GROUP_B]);
// WTE_DisplayFlowStats(flow_agg_host[0], "TEST");
// WP_Display(0,flow_agg_host[0], WP_DISPLAY_DEFAULT,0);
      WTE_CreateGroupAG999Mode();
      WTE_CreateGroupBG999Mode();

      scenario_passed = WTE_Test_System(WT_SWAP_MODE_G999, 0, frame_index, WT_WINNET_GROUP_A);
      if (!scenario_passed) all_scenario_passed = WP_FALSE;

      scenario_passed = WTE_Test_System(WT_SWAP_MODE_G999, max_g999_streams_in_use[WT_WINNET_GROUP_A], frame_index, WT_WINNET_GROUP_B);
      if (!scenario_passed) all_scenario_passed = WP_FALSE;

      scenario_passed = WTE_Test_System(WT_SWAP_MODE_ENET, 0, frame_index, WT_DUMMY_PORT_1);
      if (!scenario_passed) all_scenario_passed = WP_FALSE;

      scenario_passed = WTE_Test_System(WT_SWAP_MODE_ENET, 0, frame_index, WT_DUMMY_PORT_2);
      if (!scenario_passed) all_scenario_passed = WP_FALSE;
/*      if (!scenario_passed)
      {
         WP_U32 ii, jj;
         WP_CHAR temp_string[200];
         printf ("WTE_Scenario_2 FAILED\n");
//          WTE_DisplayEnetDeviceStats(group_a_g999_dev, "group_a_g999_dev");
         WTE_DisplayEnetDeviceStats(group_b_g999_dev, "group_b_g999_dev");
//       WTE_DisplayIwPortStats(bport_host, WP_FALSE, "bport_host");
//       WTE_DisplayIwPortStats(bport_iw_enet[WT_WINNET_GROUP_A], WP_FALSE, "bport_iw_enet[WT_WINNET_GROUP_A]");
//          for (ii=0 ; ii<max_g999_streams_in_use[WT_WINNET_GROUP_A] ; ii++)
//          {
//             sprintf (temp_string, "Group A g999_stream_handle[%d]", ii);
//             WTE_DisplayG999StreamStats(g999_stream_handle[ii], temp_string);
//          }
         for (ii=0 ; ii<max_g999_streams_in_use[WT_WINNET_GROUP_B] ; ii++)
         {
            jj = ii+max_g999_streams_in_use[WT_WINNET_GROUP_A];
            sprintf (temp_string, "Group B g999_stream_handle[%d]", jj);
            WTE_DisplayG999StreamStats(g999_stream_handle[jj], temp_string);
         }
         WTE_DisplayPCEStats(pce_device_filter, "pce_device_filter");
         WTE_DisplayPCEStats(pce_rx_channel_filter, "pce_rx_channel_filter");
         WTE_DisplayPCEGlobalStats();
         WTE_DisplayFlowStats(flow_agg_host[WT_WINNET_GROUP_A], "flow_agg_host[WT_WINNET_GROUP_A]");
         WTE_DisplayFlowStats(flow_agg_host[WT_WINNET_GROUP_B], "flow_agg_host[WT_WINNET_GROUP_B]");
//       for (ii=0 ; ii<max_g999_streams_in_use[WT_WINNET_GROUP_A] ; ii++)
//       {
//          sprintf (temp_string, "flow_agg_compatible_stream[%d]", ii);
//          WTE_DisplayFlowStats(flow_agg_compatible_stream[ii], temp_string);
//       }
      }
*/
#if (!DEBUG_SMAP)
      WTE_DeleteGroupAG999Mode();
      WTE_DeleteGroupBG999Mode();
      WTE_ResetStatisticsDummyPort1();
      WTE_ResetStatisticsDummyPort2();
#endif
   }
   if (all_scenario_passed)
      printf ("SCENARIO 2 PASSED\n");
   else
      printf ("SCENARIO 2 FAILED\n");

   return all_scenario_passed;
}

WP_boolean WTE_Scenario_3(void)
{
   WP_boolean all_scenario_passed = WP_TRUE;
   WP_boolean scenario_passed = WP_TRUE;
   WP_S32 frame_index;

   max_g999_streams_in_use[WT_WINNET_GROUP_A] = 0;
   max_g999_streams_in_use[WT_WINNET_GROUP_B] = WT_DEFAULT_NUM_OF_STREAMS;
   printf ("\t\t SCENARIO 3: WT_WINNET_GROUP_A ENET  WT_WINNET_GROUP_B G999 %d Streams \n", max_g999_streams_in_use[WT_WINNET_GROUP_B]);

   for (frame_index=0 ; frame_index<NUM_OF_FRAMES ; frame_index++)
   {
      WTE_SwapGroupAToEnetMode();
      WTE_SwapGroupBToG999Mode(max_g999_streams_in_use[WT_WINNET_GROUP_B]);
      WTE_CreateGroupAEnetMode();
      WTE_CreateGroupBG999Mode();

      scenario_passed = WTE_Test_System(WT_SWAP_MODE_ENET, 0, frame_index, WT_WINNET_GROUP_A);
      if (!scenario_passed) {all_scenario_passed = WP_FALSE; printf("%s %d\n", __FUNCTION__, __LINE__);}
      

      scenario_passed = WTE_Test_System(WT_SWAP_MODE_G999, max_g999_streams_in_use[WT_WINNET_GROUP_A], frame_index, WT_WINNET_GROUP_B);
      if (!scenario_passed) {all_scenario_passed = WP_FALSE; printf("%s %d\n", __FUNCTION__, __LINE__);}


      scenario_passed = WTE_Test_System(WT_SWAP_MODE_ENET, 0, frame_index, WT_DUMMY_PORT_1);
      if (!scenario_passed) {all_scenario_passed = WP_FALSE; printf("%s %d\n", __FUNCTION__, __LINE__);}


      scenario_passed = WTE_Test_System(WT_SWAP_MODE_ENET, 0, frame_index, WT_DUMMY_PORT_2);
      if (!scenario_passed) {all_scenario_passed = WP_FALSE; printf("%s %d\n", __FUNCTION__, __LINE__);}


#if (!DEBUG_SMAP)
      WTE_DeleteGroupAEnetMode();
      WTE_DeleteGroupBG999Mode();
      WTE_ResetStatisticsDummyPort1();
      WTE_ResetStatisticsDummyPort2();
#endif
   }
   if (all_scenario_passed)
      printf ("SCENARIO 3 PASSED\n");
   else
      printf ("SCENARIO 3 FAILED\n");
   return all_scenario_passed;
}

WP_boolean WTE_Scenario_4(void)
{
   WP_boolean all_scenario_passed = WP_TRUE;
   WP_boolean scenario_passed = WP_TRUE;
   WP_S32 frame_index;

   max_g999_streams_in_use[WT_WINNET_GROUP_A] = 0;
   max_g999_streams_in_use[WT_WINNET_GROUP_B] = 0;
   printf ("\t\t SCENARIO 4: WT_WINNET_GROUP_A ENET  WT_WINNET_GROUP_B ENET\n");

   for (frame_index=0 ; frame_index<NUM_OF_FRAMES ; frame_index++)
   {
      WTE_SwapGroupAToEnetMode();
      WTE_SwapGroupBToEnetMode();
      WTE_CreateGroupAEnetMode();
      WTE_CreateGroupBEnetMode();

      scenario_passed = WTE_Test_System(WT_SWAP_MODE_ENET, 0, frame_index, WT_WINNET_GROUP_A);
      if (!scenario_passed) all_scenario_passed = WP_FALSE;

      scenario_passed = WTE_Test_System(WT_SWAP_MODE_ENET, 0, frame_index, WT_WINNET_GROUP_B);
      if (!scenario_passed) all_scenario_passed = WP_FALSE;

      scenario_passed = WTE_Test_System(WT_SWAP_MODE_ENET, 0, frame_index, WT_DUMMY_PORT_1);
      if (!scenario_passed) all_scenario_passed = WP_FALSE;

      scenario_passed = WTE_Test_System(WT_SWAP_MODE_ENET, 0, frame_index, WT_DUMMY_PORT_2);
      if (!scenario_passed) all_scenario_passed = WP_FALSE;

/*
      WTE_DisplayEnetDeviceStats(group_a_enet_dev, "group_a_enet_dev");
      WTE_DisplayEnetDeviceStats(dummy_device2, "dummy_device2");
      WTE_DisplayEnetDeviceStats(dummy_device2, "dummy_device2");
      WTE_DisplayFlowStats(flow_agg_host[WT_DUMMY_PORT_1], "flow_agg_host[WT_DUMMY_PORT_1]");
      WTE_DisplayFlowStats(flow_agg_host[WT_DUMMY_PORT_2], "flow_agg_host[WT_DUMMY_PORT_2]");
      WTE_DisplayIwPortStats(bport_iw_swapped_enet[WT_DUMMY_PORT_1], WP_FALSE, "bport_iw_swapped_enet[WT_DUMMY_PORT_1]");
      WTE_DisplayIwPortStats(bport_iw_swapped_enet[WT_DUMMY_PORT_2], WP_FALSE, "bport_iw_swapped_enet[WT_DUMMY_PORT_2]");
      {
         WP_status status;
         printf ("***********************************************\n");
         printf ("\n\nWP_Display group_a_enet_dev (former group_a_g999_dev) 0x%x\n", group_a_enet_dev);
         status = WP_Display(WP_WINPATH(DEFAULT_WPID),
                             group_a_enet_dev,
                             WP_DISPLAY_DEFAULT,
                             0);
         terminate_on_error(status, "WP_Display() group_a_enet_dev", __LINE__);

         printf ("***********************************************\n");
         printf ("\n\nWP_Display dummy_device2 0x%x\n", dummy_device2);
         status = WP_Display(WP_WINPATH(DEFAULT_WPID),
                             dummy_device2,
                             WP_DISPLAY_DEFAULT,
                             0);
         terminate_on_error(status, "WP_Display() dummy_device2", __LINE__);
      }
*/
#if (!DEBUG_SMAP)
      WTE_DeleteGroupAEnetMode();
      WTE_DeleteGroupBEnetMode();
      WTE_ResetStatisticsDummyPort1();
      WTE_ResetStatisticsDummyPort2();
#endif
   }
   if (all_scenario_passed)
      printf ("SCENARIO 4 PASSED\n");
   else
      printf ("SCENARIO 4 FAILED\n");
   return all_scenario_passed;
}

WP_boolean WTE_Scenario_5(void)
{
   WP_boolean all_scenario_passed = WP_TRUE;
   WP_boolean scenario_passed = WP_TRUE;
   WP_S32 frame_index;

   max_g999_streams_in_use[WT_WINNET_GROUP_A] = WT_DEFAULT_NUM_OF_STREAMS;
   max_g999_streams_in_use[WT_WINNET_GROUP_B] = 0;
   printf ("\t\t SCENARIO 5: WT_WINNET_GROUP_A G999 %d Streams  WT_WINNET_GROUP_B ENET\n", max_g999_streams_in_use[WT_WINNET_GROUP_A]);

   for (frame_index=0 ; frame_index<NUM_OF_FRAMES ; frame_index++)
   {
      WTE_SwapGroupAToG999Mode(max_g999_streams_in_use[WT_WINNET_GROUP_A]);
      WTE_SwapGroupBToEnetMode();
      WTE_CreateGroupBEnetMode();
      WTE_CreateGroupAG999Mode();

      scenario_passed = WTE_Test_System(WT_SWAP_MODE_G999, 0, frame_index, WT_WINNET_GROUP_A);
      if (!scenario_passed) all_scenario_passed = WP_FALSE;

      scenario_passed = WTE_Test_System(WT_SWAP_MODE_ENET, 0, frame_index, WT_WINNET_GROUP_B);
      if (!scenario_passed) all_scenario_passed = WP_FALSE;

      scenario_passed = WTE_Test_System(WT_SWAP_MODE_ENET, 0, frame_index, WT_DUMMY_PORT_1);
      if (!scenario_passed) all_scenario_passed = WP_FALSE;

      scenario_passed = WTE_Test_System(WT_SWAP_MODE_ENET, 0, frame_index, WT_DUMMY_PORT_2);
      if (!scenario_passed) all_scenario_passed = WP_FALSE;
#if (!DEBUG_SMAP)
      WTE_DeleteGroupAG999Mode();
      WTE_DeleteGroupBEnetMode();
      WTE_ResetStatisticsDummyPort1();
      WTE_ResetStatisticsDummyPort2();
#endif
   }
   if (all_scenario_passed)
      printf ("SCENARIO 1 PASSED\n");
   else
      printf ("SCENARIO 1 FAILED\n");
   return all_scenario_passed;
}


#define MAX_WINUTIL_HISTORY 10
WP_CHAR WinutilCommands[MAX_WINUTIL_HISTORY][256];
WP_S32 LastWinutilCommand = 0;
extern void WPU_WinutilCall(WP_CHAR * input);

void WTE_WinutilCommand(void)
{
   WP_CHAR InputBuf[256];
   while(1)
   {
      printf("Enter Winutil command, 'h' for history, 'q' to return to main menu\n");
      printf ("bmap, hbm, md, mm, ver, wgu, wfc, dmap, trsu, trsa, smap, tab, taom, taot, tast, tb, trs, fmum, freq, sysu\n");
      gets(InputBuf);
      printf("\n");
      if (! strcmp(InputBuf, "h") )
      {
         WP_S32 ii, WinutilCommandIndex;
         WinutilCommandIndex = LastWinutilCommand;
         for (ii=0 ; ii<MAX_WINUTIL_HISTORY ; ii++)
         {
            printf ("%d \t __ %s __ \n", ii, WinutilCommands[WinutilCommandIndex-1]);
            if (WinutilCommandIndex == 0)
               WinutilCommandIndex = MAX_WINUTIL_HISTORY;
            WinutilCommandIndex--;
         }
      }
      else if (! strcmp(InputBuf,  "q") )
      {
         break;
      }
      else
      {
         strcpy (WinutilCommands[LastWinutilCommand], InputBuf);
         printf ("InputBuf %s WinutilCommands[ %d ] %s\n", InputBuf, LastWinutilCommand, WinutilCommands[LastWinutilCommand]);
         LastWinutilCommand++;
         LastWinutilCommand = LastWinutilCommand % MAX_WINUTIL_HISTORY;
         WPU_WinutilCall(InputBuf);
      }
   }
}

void WTE_WinutilSmap(void)
{
   WPU_WinutilCall("smap");
}

/**************************************************
 *
 * WTI_LogCallBack
 *
 **************************************************/
WP_status WTI_LogCallBack(WP_S32 level, const WP_CHAR *message)
{
   printf("%s", message);
   return WP_OK;
}

void WTI_WddiAttachCallback(const WP_CHAR *file, const WP_CHAR *function, WP_S32 line)
{
   if (wddi_trace_debug)
   {
      printf ("%s %s %d \n", file, function, line);
   }
   return;
}

void WTE_Terminate(WP_U32 status)
{
   /* Release WDDI */
   WP_DriverRelease();
   WT_Reboot();
   exit(status);
}

void terminate_on_error(WP_handle handle, WP_CHAR *s,WP_U32 line)
{
   if (WP_ERROR_P(handle))
   {
      printf("%s: Terminal error (handle 0x%x) %s\n", appname, handle, s);
      printf("%s: Error type - %s\n", appname, WP_ErrorString(handle));
      printf("line = %d\n",line);
      WTE_Terminate(1);
   }
   else
   {
#if (DEBUG_PRINT_WDDI_FUNCTION_CALL)
      printf("%s: Handle/Status returned from %s is 0x%x line %d\n", appname, s, handle, line);
#endif
   }
}


#include "wt_util.c"
#include "wt_partition_ecc_util.c"

