/*****************************************************************************
 * (C) Copyright Wintegra 2001.  All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: 95100-pce_classification_unit_test.c
 *
 * Purpose: Check WDDI PCE classification functions w/o data
 *
 ****************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#include "morris_config.h"


#include "wp_wddi.h"
#include "wp_host_io.h"

#include "wp_sim.h"
#include "app_convert.h"
#include "wt_util.h"
#include "wt_partition_ecc_util.h"
/*#include "wt_demo2reg.h" */


#include "wpx_enet_phy.h"

/*#define USE_OC12_PHY*/
//#define DEBUG_PRINT

#ifdef DEBUG_PRINT
// #define MY_PRINTF(x, ...) printf(x, ##__VA_ARGS__)
#define MY_PRINTF printf
#else
#define MY_PRINTF(x, ...)
#endif

#ifdef USE_OC12_PHY
#include "wpx_oc12_pmc5357.h"
#else
#include "wpx_oc3_pmc5351.h"
#endif

#include "wpx_app_data.h"
#include "wpx_board_if.h"

#ifdef USE_SOCKET
#include "sockint.h"
#endif
#include "iiw_callback.h"

#ifndef WPI_REGISTRY_H
#include "include/core/registry/wpi_registry.h"
#endif

extern WP_U32 dps_ProgramImage[];
extern WP_U16 dps_PC_Table_Init[];

extern WP_pce_parser_info pce_parser_info;

WP_int_queue_table int_queue_tables = {{{10, 1}, {10, 1}, {10, 1}, {10, 1}}};

#define POOL_LIMIT           10
#define NUM_OF_AAL5_CH       2

#define ETH_RX_TAG1          1001
#define ETH_RX_TAG2          1002
#define ETH_RX_TAG3          1003
#define ETH_TX_TAG1          1101
#define ETH_TX_TAG2          1102
#define ETH_TX_TAG3          1103

#define IW_RX_DUMMY_CH_TAG   2000

#define DATA_LENGTH          (68 + 32)

#define MIN_CH_RATE          5000
#define MAX_CH_RATE          2048000

#define APP_HOST_BUFFER_SIZE 256

#define IW_SYS_MAX_FLOWS     8
#define LEARNING_QUEUE_SIZE  10
#define IW_SYS_MAX_BPORTS    16
#define BRIDGE_PORT_1        11
#define BRIDGE_PORT_2        12
#define BRIDGE_PORT_3        13

#define MAX_PACKET_SIZE      1984
#define IW_BUF_N_BUFFERS     20
#define QN_IW_N_BUFFERS      50

WP_CHAR appname[] = "WP3 PCE Classification Unit Test";

WP_CHAR ascii_buffer[DATA_LENGTH*2+1];
WP_CHAR hex_buffer[DATA_LENGTH*2];
static WP_U32 allow_error_print = 1;

#include "wt_pce_misc.h"
#include "wt_pce_misc_structs.c"

WP_status status;

WP_handle iw_pool, host_pool;
WP_handle iw_pool1, iw_pool2, iw_pool3, iw_pool4, iw_pool5;
WP_handle qnh, qniw, qniw_enet, qniw_enet1, qniw_enet2, qniw_enet3, qniw_enet4, qniw_enet5;

/* Port handles */
WP_handle enetp1, enetp2, enetp3, iw_port;

/* Device handles */
WP_handle enetdev1, enetdev2, enetdev3, iw_dev;

WP_handle iwrx_handle, iwtx_handle;

/* Rx and Tx channel handles */
WP_handle rx_channel_enet_handle1, rx_channel_enet_handle2, rx_channel_enet_handle3, rx_channel_enet_handle4;
WP_handle tx_channel_enet_handle1, tx_channel_enet_handle2, tx_channel_enet_handle3, tx_channel_enet_handle4;

WP_handle rx_host_handle10, tx_host_handle11, rx_host_handle11, rx_host_handle12, rx_host_handle13, rx_host_handle14;
WP_handle iw_rx_ch;
WP_handle iw_system_handle;

/* IW bridge port handles */
WP_handle bport_iw1, bport_iw2, bport_iw3;

/* Flow aggregation handles */
WP_handle flow_aggregation1, flow_aggregation2, flow_aggregation3;

WP_U16 wpe_test_failures = 0;


/* WPE_init */
static void WPE_init(void)
{
   WP_status status;

#ifdef USE_SOCKET
   InitSocketInterface();
#endif

   status = WP_DriverInit();
   WPE_TerminateOnError(status,"WP_DriverInit",__LINE__);

#ifndef USE_SOCKET
   status = WPX_BoardConfigure(WP_WINPATH(DEFAULT_WPID), WPX_CONFIGURE_DEFAULT_CHECKIN);
   WPE_TerminateOnError(status, "WPX_BoardConfigure", __LINE__);
#endif

#ifdef WP_BOARD_WDS3_SL
   int_ram_partition.policer_entries = 0;
#else
   int_ram_partition.policer_entries = 8000;
#endif
   int_ram_partition.pce_hw_rules_num = pce_init.hardware_rules_num;
   WT_SetIntRamPartition(WP_WINPATH(DEFAULT_WPID), &int_ram_partition);

   status = WP_SysInit(WP_WINPATH(DEFAULT_WPID), context);
   WPE_TerminateOnError(status,"WP_SysInit", __LINE__);
   status = WP_ControlRegister(WP_DEBUG_CALLBACK_DEBUG_AREA_LOG, DebugAreaCallback);
}

/* WPE_Create_pools */
static void WPE_Create_pools(void)
{
   /* Create the Buffer pool(s) - interworking*/
#ifdef WINSIM
   iw_buffer_data[0].bus = WP_BUS_HOST;
   iw_buffer_data[0].bank = APP_BANK_HOST;
#endif
   iw_pool = WP_PoolCreate(WP_WINPATH(DEFAULT_WPID), WP_pool_iwbuffer, iw_buffer_data);
   WPE_TerminateOnError(iw_pool, "WP_PoolCreate", __LINE__);

   /* Create an interworking queue node */
   qn_iw->adjunct_pool = iw_pool;

   qniw_enet = WP_QNodeCreate(WP_WINPATH(DEFAULT_WPID), WP_QNODE_IWQ | WP_QNODE_OPT_DEDICATED_RX_HWQ | WP_QNODE_OPT_FMU, qn_iw);
   WPE_TerminateOnError(qniw_enet, "WP_QNodeCreate IW_ENETQ", __LINE__);

}

/* WPE_Create_ports */
static void WPE_Create_ports(void)
{
   WP_status status;
   /* Create an Enet Ports and Devices */
#ifdef USE_SOCKET
   enetp1 = WP_PortCreate(WP_WINPATH(DEFAULT_WPID), WP_PORT_ENET1, enet_s);
   WPE_TerminateOnError(enetp1, "WP_PortCreate Enet1", __LINE__);

   enetp2 = WP_PortCreate(WP_WINPATH(DEFAULT_WPID), WP_PORT_ENET2, enet_s);
   WPE_TerminateOnError(enetp2, "WP_PortCreate Enet2", __LINE__);

   enetp3 = WP_PortCreate(WP_WINPATH(DEFAULT_WPID), WP_PORT_ENET3, enet_s);
   WPE_TerminateOnError(enetp3, "WP_PortCreate Enet3", __LINE__);
#else
   /* Create an Enet Ports and Devices */
#ifdef WP_BOARD_WDS3_SL
   status = WPX_BoardSerdesInit(WP_WINPATH(DEFAULT_WPID), WP_PORT_ENET1, WP_TRUE);
   WPE_TerminateOnError(status, "WPX_BoardSerdesSetLoopback()", __LINE__);

   status = WPX_BoardSerdesInit(WP_WINPATH(DEFAULT_WPID), WP_PORT_ENET2, WP_TRUE);
   WPE_TerminateOnError(status, "WPX_BoardSerdesSetLoopback", __LINE__);

   status = WPX_BoardSerdesInit(WP_WINPATH(DEFAULT_WPID), WP_PORT_ENET9, WP_TRUE);
   WPE_TerminateOnError(status, "WPX_BoardSerdesSetLoopback", __LINE__);

   enetp1 = WP_PortCreate(WP_WINPATH(DEFAULT_WPID), WP_PORT_ENET1, enet_s);
   WPE_TerminateOnError(enetp1, "WP_PortCreate Enet1", __LINE__);

   enetp2 = WP_PortCreate(WP_WINPATH(DEFAULT_WPID), WP_PORT_ENET2, enet_s);
   WPE_TerminateOnError(enetp2, "WP_PortCreate Enet2", __LINE__);

   enetp3 = WP_PortCreate(WP_WINPATH(DEFAULT_WPID), WP_PORT_ENET9, enet_s);
   WPE_TerminateOnError(enetp3, "WP_PortCreate Enet3", __LINE__);
#else
   status = WPX_BoardSerdesInit(WP_WINPATH(DEFAULT_WPID), WP_PORT_ENET13, WP_TRUE);
   WPE_TerminateOnError(status, "WPX_BoardSerdesSetLoopback()", __LINE__);

   status = WPX_BoardSerdesInit(WP_WINPATH(DEFAULT_WPID), WP_PORT_ENET8, WP_TRUE);
   WPE_TerminateOnError(status, "WPX_BoardSerdesSetLoopback", __LINE__);

   status = WPX_BoardSerdesInit(WP_WINPATH(DEFAULT_WPID), WP_PORT_ENET14, WP_TRUE);
   WPE_TerminateOnError(status, "WPX_BoardSerdesSetLoopback", __LINE__);

   enetp1 = WP_PortCreate(WP_WINPATH(DEFAULT_WPID), WP_PORT_ENET13, enet_s);
   WPE_TerminateOnError(enetp1, "WP_PortCreate Enet1", __LINE__);

   enetp2 = WP_PortCreate(WP_WINPATH(DEFAULT_WPID), WP_PORT_ENET8, enet_s);
   WPE_TerminateOnError(enetp2, "WP_PortCreate Enet2", __LINE__);

   enetp3 = WP_PortCreate(WP_WINPATH(DEFAULT_WPID), WP_PORT_ENET14, enet_s);
   WPE_TerminateOnError(enetp3, "WP_PortCreate Enet3", __LINE__);
#endif//SLB
#endif
   iw_port = WP_PortCreate(WP_WINPATH(DEFAULT_WPID), WP_PORT_IW_HOST, NULL);
   WPE_TerminateOnError(iw_port, "PortCreate", __LINE__);

   enetdev1 = WP_DeviceCreate(enetp1, WP_PHY(0), WP_DEVICE_ENET, enet_dev);
   WPE_TerminateOnError(enetdev1, "WP_DeviceCreate Enet1", __LINE__);

   enetdev2 = WP_DeviceCreate(enetp2, WP_PHY(0), WP_DEVICE_ENET, enet_dev);
   WPE_TerminateOnError(enetdev2, "WP_DeviceCreate Enet2", __LINE__);

   enetdev3 = WP_DeviceCreate(enetp3, WP_PHY(0), WP_DEVICE_ENET, enet_dev);
   WPE_TerminateOnError(enetdev3, "WP_DeviceCreate Enet3", __LINE__);

   enet_dev->extended_params = enet_dev_ex;
   status = WP_DeviceModify(enetdev1,
                            WP_DEV_MOD_ENET_PHYSTATMODE |
                            WP_DEV_MOD_ENET_TX_STATMODE |
                            WP_DEV_MOD_ENET_RX_STATMODE,
                            &enet_dev);
   WPE_TerminateOnError(status, "WP_DeviceModify() ENET 1", __LINE__);

   status = WP_DeviceModify(enetdev2,
                            WP_DEV_MOD_ENET_PHYSTATMODE |
                            WP_DEV_MOD_ENET_TX_STATMODE |
                            WP_DEV_MOD_ENET_RX_STATMODE,
                            &enet_dev);
   WPE_TerminateOnError(status, "WP_DeviceModify() ENET 2", __LINE__);

   status = WP_DeviceModify(enetdev3,
                            WP_DEV_MOD_ENET_PHYSTATMODE |
                            WP_DEV_MOD_ENET_TX_STATMODE |
                            WP_DEV_MOD_ENET_RX_STATMODE,
                            &enet_dev);
   WPE_TerminateOnError(status, "WP_DeviceModify() ENET 3", __LINE__);

   iw_dev = WP_DeviceCreate(iw_port, WP_PHY(0), WP_DEVICE_IW_HOST, NULL);
   WPE_TerminateOnError(iw_dev, "DeviceCreate", __LINE__);

   enet_dev->extended_params = enet_dev_ex;
}

static void WPE_CreateChannels(void)
{
   /* create device 1 rx and tx channels */
   ch_config_enet[0].iwmode = WP_PKTCH_IWM_DISABLE;
   tx_channel_enet_handle1 = WP_ChannelCreate(ETH_TX_TAG1, enetdev1, qniw_enet, WP_CH_TX, WP_ENET, &ch_config_enet[0]);
   WPE_TerminateOnError(tx_channel_enet_handle1, "WP_ChannelCreate() Enet TX 1", __LINE__);

   ch_config_enet[0].iwmode = WP_PKTCH_IWM_ENABLE;
   rx_channel_enet_handle1 = WP_ChannelCreate(ETH_RX_TAG1, enetdev1, qniw_enet, WP_CH_RX, WP_ENET, &ch_config_enet[0]);
   WPE_TerminateOnError(rx_channel_enet_handle1, "WP_ChannelCreate() Enet RX", __LINE__);

   /* create device 2 rx and tx channels */
   ch_config_enet[0].iwmode = WP_PKTCH_IWM_ENABLE;
   tx_channel_enet_handle2 = WP_ChannelCreate(ETH_TX_TAG2, enetdev2, qniw_enet, WP_CH_TX, WP_ENET, &ch_config_enet[0]);
   WPE_TerminateOnError(tx_channel_enet_handle2, "WP_ChannelCreate() Enet TX 2", __LINE__);

   ch_config_enet[0].iwmode = WP_PKTCH_IWM_DISABLE;
   rx_channel_enet_handle2 = WP_ChannelCreate(ETH_RX_TAG2, enetdev2, qniw_enet, WP_CH_RX, WP_ENET, &ch_config_enet[0]);
   WPE_TerminateOnError(rx_channel_enet_handle2, "WP_ChannelCreate() Enet RX", __LINE__);

   /* create device 3 rx and tx channels */
   ch_config_enet[0].iwmode = WP_PKTCH_IWM_ENABLE;
   tx_channel_enet_handle3 = WP_ChannelCreate(ETH_TX_TAG3, enetdev3, qniw_enet, WP_CH_TX, WP_ENET, &ch_config_enet[0]);
   WPE_TerminateOnError(tx_channel_enet_handle3, "WP_ChannelCreate() Enet TX 3", __LINE__);

   ch_config_enet[0].iwmode = WP_PKTCH_IWM_DISABLE;
   rx_channel_enet_handle3 = WP_ChannelCreate(ETH_RX_TAG3, enetdev3, qniw_enet, WP_CH_RX, WP_ENET, &ch_config_enet[0]);
   WPE_TerminateOnError(rx_channel_enet_handle3, "WP_ChannelCreate() Enet RX", __LINE__);

   /* Create dummy host channel */
   iw_rx_ch = WP_ChannelCreate(IW_RX_DUMMY_CH_TAG, iw_dev, qniw_enet, WP_CH_RX, WP_IW_HOST, &iw_rx_ch_cfg);
   WPE_TerminateOnError(iw_rx_ch, "ChannelCreate(iw)", __LINE__);
}

static void WPE_CreateFlowAggregationsAndBports(void)
{
   WP_handle status;
   /* create flow aggregations with no iw_port yet.
      this will be later modified by WP_IwFlowAggregationModify.
      this is the way to solve the initialization problem of circular
      dependency between flow aggregation and Iw_Port */
   tx_agg_bridging->vlan_tag_mode = WP_IW_VLAN_TAG_DISABLE;
   tx_agg_bridging->txfunc = iw_rx_ch;
   flow_aggregation1 = WP_IwFlowAggregationCreate(WP_WINPATH(DEFAULT_WPID), WP_IW_TRANS_BRIDGING_MODE, &tx_agg_bridging);
   WPE_TerminateOnError(flow_aggregation1," WP_IwFlowAggregationCreate()", __LINE__);
   flow_aggregation2 = WP_IwFlowAggregationCreate(WP_WINPATH(DEFAULT_WPID), WP_IW_TRANS_BRIDGING_MODE, &tx_agg_bridging);
   WPE_TerminateOnError(flow_aggregation2," WP_IwFlowAggregationCreate()", __LINE__);
   flow_aggregation3 = WP_IwFlowAggregationCreate(WP_WINPATH(DEFAULT_WPID), WP_IW_TRANS_BRIDGING_MODE, &tx_agg_bridging);
   WPE_TerminateOnError(flow_aggregation3," WP_IwFlowAggregationCreate()", __LINE__);

   /* Create the bridging ports */
   bport_conf->flow_agg = flow_aggregation2;/* ### */
   bport_conf->tag = BRIDGE_PORT_1;
   bport_iw1 = WP_IwBridgingPortCreate( iw_system_handle, bport_conf);
   WPE_TerminateOnError(bport_iw1, "WP_IwBridgingPortCreate() 1", __LINE__);

   bport_conf->flow_agg = flow_aggregation2;
   bport_conf->tag = BRIDGE_PORT_2;
   bport_iw2 = WP_IwBridgingPortCreate( iw_system_handle, bport_conf);
   WPE_TerminateOnError(bport_iw2, "WP_IwBridgingPortCreate() 2", __LINE__);

   bport_conf->flow_agg = flow_aggregation3;
   bport_conf->tag = BRIDGE_PORT_3;
   bport_iw3 = WP_IwBridgingPortCreate( iw_system_handle, bport_conf);
   WPE_TerminateOnError(bport_iw3, "WP_IwBridgingPortCreate() 3", __LINE__);

   /* Update the Flow Aggregations*/
   tx_agg_bridging->txfunc = tx_channel_enet_handle1;
   status = WP_IwFlowAggregationModify(flow_aggregation1,
                                       WP_IW_FLOW_AGG_MOD_TXFUNC,
                                       tx_agg_bridging);
   WPE_TerminateOnError(status, "WP_IwFlowAggregationModify() 1", __LINE__);

   tx_agg_bridging->txfunc = tx_channel_enet_handle2;
   status = WP_IwFlowAggregationModify(flow_aggregation2,
                                       WP_IW_FLOW_AGG_MOD_TXFUNC,
                                       tx_agg_bridging);
   WPE_TerminateOnError(status, "WP_IwFlowAggregationModify() 2", __LINE__);

   tx_agg_bridging->txfunc = tx_channel_enet_handle3;
   status = WP_IwFlowAggregationModify(flow_aggregation3,
                                       WP_IW_FLOW_AGG_MOD_TXFUNC,
                                       tx_agg_bridging);
   WPE_TerminateOnError(status, "WP_IwFlowAggregationModify() 3", __LINE__);
}

static void WPE_CreateIwSystemAndItsComponents(void)
{
   WP_status status;

   WPE_CreateChannels();

   /* Create Tx bindings*/
   status = WP_IwTxBindingCreate(tx_channel_enet_handle1, WP_IW_TX_BINDING, tx_binding_cfg);
   WPE_TerminateOnError(status, "WP_IwTxBindingCreate() ENET TX 1", __LINE__);
   status = WP_IwTxBindingCreate(tx_channel_enet_handle2, WP_IW_TX_BINDING, tx_binding_cfg);
   WPE_TerminateOnError(status, "WP_IwTxBindingCreate() ENET TX 2", __LINE__);
   status = WP_IwTxBindingCreate(tx_channel_enet_handle3, WP_IW_TX_BINDING, tx_binding_cfg);
   WPE_TerminateOnError(status, "WP_IwTxBindingCreate() ENET TX 3", __LINE__);

   /* Create the interworking system */
   iw_system_handle = WP_IwSystemCreate(WP_WINPATH(DEFAULT_WPID), WP_IW_TRANS_BRIDGING_MODE, &iw_sys);
   WPE_TerminateOnError(iw_system_handle, "WP_IwSystemCreate()", __LINE__);

   WPE_CreateFlowAggregationsAndBports();
}

static void WPE_Terminate(WP_U32 status)
{
   /* Debug area */
   WP_DebugAreaLog(WP_WINPATH(DEFAULT_WPID), WP_B_DEBUG);

   /* Release WDDI */
   WP_DriverRelease();

   /* For socket based simulators */
#ifdef USE_SOCKET
   CleanUpSocket(5);
#endif
   WT_Reboot();
   exit(status);
}
#define TESTSTRING(x) TESTSTRING_(x)
#define TESTSTRING_(x) #x

/////////////////////////////////////////////////////////////////////////////////////////////
//
// PCE WDDI unit test functions
//
/////////////////////////////////////////////////////////////////////////////////////////////

WP_handle filter, results_special_filter, ext_filter, filter1, ext_filter1, filter2, ext_filter2;
WP_handle filter_set_lvl1_handle, filter_set_lvl0_handle, filter_set_lvl1_handle2;
WP_handle rule_handles[33000];
WP_handle iterator_handles[100];

WP_handle rng_filter1, rng_filter2, rng_filter3;
WP_handle ext_rng_filter1;

WP_handle msk_filter1, msk_filter2, msk_filter3;
WP_handle ext_msk_filter1, ext_msk_filter2, ext_msk_filter3;

WP_handle rng_msk_filter1, rng_msk_filter2, rng_msk_filter3;
WP_handle ext_rng_msk_filter1, ext_rng_msk_filter2, ext_rng_msk_filter3;

static void WPE_PceRulesTest(WP_U32 rules_num, WP_U32 seed)
{
   WP_pce_rule_classification rule_cfg = {0};
   WP_ipv4_addr ipv4_addr;
   WP_U8 mac_addr[] = {0xCA, 0xFE, 0xBB, 0xCC, 0xDD, 0xEE};
   WP_vlan_id vlan_id = 0x0100;
   WP_U32 i, j;
   WP_handle status;
   WP_handle rule_handle;
   WP_pce_result_flow_agg result_flow_agg;

/***************************************************************
 * Single Rule Test
 **************************************************************/
   ipv4_addr = 0xC0A8966B;

   rule_cfg.filter_handle = filter;
   rule_cfg.enabled = WP_ENABLE;
   rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_MAC_DA;
   for(i=0; i<6; i++)
      rule_cfg.rule_fields[0].value.mac_addr[i] = mac_addr[i];

   rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_MAC_SA;
   for(i=0; i<6; i++)
      rule_cfg.rule_fields[1].value.mac_addr[i] = mac_addr[i];

   rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_LAST;

   rule_cfg.match_action = WP_PCE_RULE_MATCH_CONTINUE;
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_LAST;

   MY_PRINTF("\n---- Rule Creating -- 00 -------------------------------------------------\n");
   WPE_TEST_CHECK_HANDLE(rule_handles[0],
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));
   MY_PRINTF("rule_handle[0] = %X\n", rule_handles[0]);

   WPE_TEST_CHECK_RESULT(status,
                     WPI_BASIC_ERROR(WP_ERR_PCE_FILTER_USED_RULES),
                     WP_PceFilterDelete(filter));

   MY_PRINTF("\n---- Creating the same Rule  -- 00 ---------------------------------------\n");
   WPE_TEST_CHECK_RESULT(rule_handle,
                     WPI_BASIC_ERROR(WP_ERR_PCE_RULE_ALREADY_EXISTS),
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));

   MY_PRINTF("\n---- Disabling the Rule  0x%X ---------------------------------------\n", rule_handles[0]);
   WPE_TEST_CHECK_RESULT(status,
                     WP_OK,
                     WP_PceRuleDisable(rule_handles[0]));

   MY_PRINTF("\n---- Creating the disabled Rule  -- 00 -----------------------------------\n");
   WPE_TEST_CHECK_RESULT(rule_handle,
                     WPI_BASIC_ERROR(WP_ERR_PCE_RULE_ALREADY_EXISTS),
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));
   MY_PRINTF("rule_handle = %X\n", rule_handle);

   rule_cfg.rule_fields[0].value.mac_addr[0] = 0x35;
   rule_cfg.rule_fields[1].value.mac_addr[0] = 0x35;
   MY_PRINTF("\n---- Rule Creating -- 35 ------------------------------------------------\n");
   WPE_TEST_CHECK_HANDLE(rule_handles[1],
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));
   MY_PRINTF("rule_handles[1] = %X\n", rule_handles[1]);

   rule_cfg.rule_fields[0].value.mac_addr[0] = 0x45;
   rule_cfg.rule_fields[1].value.mac_addr[0] = 0x45;
   MY_PRINTF("\n---- Rule Creating -- 45 ------------------------------------------------\n");
   WPE_TEST_CHECK_HANDLE(rule_handles[2],
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));
   MY_PRINTF("rule_handles[2] = %X\n", rule_handles[2]);

   MY_PRINTF("\n---- Rule Creating: should be the same ---------------------------------\n");
   WPE_TEST_CHECK_RESULT(rule_handle,
                     WPI_BASIC_ERROR(WP_ERR_PCE_RULE_ALREADY_EXISTS),
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));
   MY_PRINTF("rule_handle = %X\n", rule_handle);

   MY_PRINTF("\n---- Searching Rule Object: WP_ERR_PCE_OBJECT_NOT_FOUND --------------------\n");
   rule_cfg.rule_fields[0].value.mac_addr[0] = 0x46;
   WPE_TEST_CHECK_RESULT(rule_handle,
                     WPI_BASIC_ERROR(WP_ERR_PCE_OBJECT_NOT_FOUND),
                     WP_PceObjectSearch (WP_WINPATH(DEFAULT_WPID),
                                       WP_PCE_OBJECT_TYPE_RULE_CLASSIFICATION,
                                       &rule_cfg));
   rule_cfg.rule_fields[0].value.mac_addr[0] = 0x45;

   MY_PRINTF("\n---- Searching Rule Object: object was found -------------------------------\n");
   WPE_TEST_CHECK_HANDLE(rule_handle,
                     WP_PceObjectSearch (WP_WINPATH(DEFAULT_WPID),
                                       WP_PCE_OBJECT_TYPE_RULE_CLASSIFICATION,
                                       &rule_cfg));
   MY_PRINTF("rule_handle = %X\n", rule_handle);

 /***************************************************************
 * Extended Rule Test
 ***************************************************************/
   rule_cfg.filter_handle = ext_filter;
   rule_cfg.rule_fields[2].value.ipv4_addr = ipv4_addr;
   rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_IPV4_SA;

   rule_cfg.rule_fields[3].value.ipv4_addr = ipv4_addr;
   rule_cfg.rule_fields[3].field_id = WP_PCE_FIELD_ID_IPV4_DA;

   rule_cfg.rule_fields[4].value.vlan_tag = vlan_id;
   rule_cfg.rule_fields[4].field_id = WP_PCE_FIELD_ID_EXT_VLAN_TAG;

   rule_cfg.rule_fields[5].field_id = WP_PCE_FIELD_ID_LAST;

   MY_PRINTF("\n---- Extended Rule Creating --------------------------------------------\n");
   WPE_TEST_CHECK_HANDLE(rule_handles[3],
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));
   MY_PRINTF("rule_handles[3] = %X\n", rule_handles[3]);

   rule_cfg.rule_fields[4].value.vlan_tag = 0x200;
   MY_PRINTF("\n---- Extended Rule Creating: VLAN = 200 --------------------------------\n");
   WPE_TEST_CHECK_HANDLE(rule_handles[4],
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));
   MY_PRINTF("rule_handles[4] = %X\n", rule_handles[4]);

   rule_cfg.rule_fields[0].value.mac_addr[0] = 0x55;
   rule_cfg.rule_fields[1].value.mac_addr[0] = 0x55;
   MY_PRINTF("\n---- Extended Rule Creating: MAC0 = 0x55 -------------------------------\n");
   WPE_TEST_CHECK_HANDLE(rule_handles[5],
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));
   MY_PRINTF("rule_handles[5] = %X\n", rule_handles[5]);

   MY_PRINTF("\n---- Extended Rule Creating: should be the same ------------------------\n");
   WPE_TEST_CHECK_RESULT(rule_handle,
                     WPI_BASIC_ERROR(WP_ERR_PCE_RULE_ALREADY_EXISTS),
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));
   MY_PRINTF("rule_handle = %X\n", rule_handle);

   WPE_TEST_CHECK_RESULT(rule_handle,
                     WPI_BASIC_ERROR(WP_ERR_PCE_RULE_INCOMPATIBLE_FILTER_TYPE),
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_FORWARDING, &rule_cfg));

   WPE_TEST_CHECK_RESULT(rule_handle,
                     WPI_BASIC_ERROR(WP_ERR_PCE_RULE_ILLEGAL_TYPE),
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_FORWARDING+100, &rule_cfg));

   rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_IPV4_DA; /* set wrong field id */
   WPE_TEST_CHECK_RESULT(rule_handle,
                     WPI_BASIC_ERROR(WP_ERR_PCE_RULE_FIELDS_MISMATCH),
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));

   rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_IPV4_SA; /* return the correct field id */

   rule_cfg.filter_handle += 300;/* wrong filter id */
   WPE_TEST_CHECK_RESULT(rule_handle,
                     WPI_BASIC_ERROR(WP_ERR_HANDLE),
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));
   rule_cfg.filter_handle = ext_filter;

   rule_cfg.filter_handle |= 0xFF;/* wrong filter id */
   WPE_TEST_CHECK_RESULT(rule_handle,
                     WPI_BASIC_ERROR(WP_ERR_HANDLE),
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));
   rule_cfg.filter_handle = ext_filter;

   rule_cfg.filter_handle = 0;/* wrong filter id */
   WPE_TEST_CHECK_RESULT(rule_handle,
                     WPI_BASIC_ERROR(WP_ERR_HANDLE),
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));
   rule_cfg.filter_handle = ext_filter;

   WPE_TEST_CHECK_RESULT(status,
                     WPI_BASIC_ERROR(WP_ERR_HANDLE),
                     WP_PceRuleDelete(1));

   WPE_TEST_CHECK_RESULT(status,
                     WPI_BASIC_ERROR(WP_ERR_HANDLE),
                     WP_PceRuleDelete(rule_handles[0] + 40000));

   WPE_TEST_CHECK_RESULT(status,
                     WPI_BASIC_ERROR(WP_ERR_HANDLE),
                     WP_PceRuleDelete(rule_handles[0] + 10000));

   /* Modify */
   WPE_TEST_CHECK_RESULT(status,
                     WPI_BASIC_ERROR(WP_ERR_HANDLE),
                     WP_PceRuleModify(rule_handles[0] + 10000,
                                       WP_PCE_RULE_MODIFY_COMMAND_MATCH_ACTION,
                                       &rule_cfg));

   WPE_TEST_CHECK_RESULT(status,
                     WPI_BASIC_ERROR(WP_ERR_HANDLE),
                     WP_PceRuleModify(ext_filter,
                                       WP_PCE_RULE_MODIFY_COMMAND_MATCH_ACTION,
                                       &rule_cfg));

   rule_cfg.match_action = 100;
   WPE_TEST_CHECK_RESULT(status,
                     WPI_BASIC_ERROR(WP_ERR_PCE_RULE_ILLEGAL_MATCH_ACTION),
                     WP_PceRuleModify(rule_handles[0],
                                       WP_PCE_RULE_MODIFY_COMMAND_MATCH_ACTION,
                                       &rule_cfg));
   rule_cfg.match_action = WP_PCE_RULE_MATCH_CONTINUE;

   WPE_TEST_CHECK_RESULT(status,
                     WPI_BASIC_ERROR(WP_ERR_PCE_RULE_MODIFY_ILLEGAL_COMMAND),
                     WP_PceRuleModify(rule_handles[0],
                                       100,
                                       &rule_cfg));

   WPE_TEST_CHECK_RESULT(status,
                     WP_OK,
                     WP_PceRuleModify(rule_handles[0],
                                       WP_PCE_RULE_MODIFY_COMMAND_MATCH_ACTION,
                                       &rule_cfg));

   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
   result_flow_agg.flow_aggregation = flow_aggregation3;
   rule_cfg.match_result[0].param.flow_agg = result_flow_agg;
   rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;

   WPE_TEST_CHECK_RESULT(status,
                     WP_OK,
                     WP_PceRuleModify(rule_handles[0],
                                       WP_PCE_RULE_MODIFY_COMMAND_RESULT,
                                       &rule_cfg));

   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_LAST;
   WPE_TEST_CHECK_RESULT(status,
                     WP_OK,
                     WP_PceRuleModify(rule_handles[0],
                                       WP_PCE_RULE_MODIFY_COMMAND_RESULT,
                                       &rule_cfg));

   MY_PRINTF("\n---- Searching Extended Rule Object ---------------------------------\n");
   WPE_TEST_CHECK_RESULT(rule_handle,
                     WPI_BASIC_ERROR(WP_ERR_PCE_OBJECT_ILLEGAL_TYPE),
                     WP_PceObjectSearch (WP_WINPATH(DEFAULT_WPID),
                                       100,
                                       &rule_cfg));

   rule_cfg.filter_handle = 200;
   WPE_TEST_CHECK_RESULT(rule_handle,
                     WPI_BASIC_ERROR(WP_ERR_HANDLE),
                     WP_PceObjectSearch (WP_WINPATH(DEFAULT_WPID),
                                       WP_PCE_OBJECT_TYPE_RULE_CLASSIFICATION,
                                       &rule_cfg));
   rule_cfg.filter_handle = ext_filter;

   MY_PRINTF("\n---- extended rule differs in the first rule entry ------------------\n");
   /* extended rule differs in the first rule entry */
   rule_cfg.rule_fields[0].value.mac_addr[0] = 0x54;
   WPE_TEST_CHECK_RESULT(rule_handle,
                     WPI_BASIC_ERROR(WP_ERR_PCE_OBJECT_NOT_FOUND),
                     WP_PceObjectSearch (WP_WINPATH(DEFAULT_WPID),
                                       WP_PCE_OBJECT_TYPE_RULE_CLASSIFICATION,
                                       &rule_cfg));
   rule_cfg.rule_fields[0].value.mac_addr[0] = 0x55;

   MY_PRINTF("\n---- extended rule differs in the second rule entry -----------------\n");
   /* extended rule differs in the second rule entry */
   rule_cfg.rule_fields[3].value.ipv4_addr = 0x12345678;
   WPE_TEST_CHECK_RESULT(rule_handle,
                     WPI_BASIC_ERROR(WP_ERR_PCE_OBJECT_NOT_FOUND),
                     WP_PceObjectSearch (WP_WINPATH(DEFAULT_WPID),
                                       WP_PCE_OBJECT_TYPE_RULE_CLASSIFICATION,
                                       &rule_cfg));
   rule_cfg.rule_fields[3].value.ipv4_addr = ipv4_addr;

   rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_IPV4_DA;
   WPE_TEST_CHECK_RESULT(rule_handle,
                     WPI_BASIC_ERROR(WP_ERR_PCE_RULE_FIELDS_MISMATCH),
                     WP_PceObjectSearch (WP_WINPATH(DEFAULT_WPID),
                                       WP_PCE_OBJECT_TYPE_RULE_CLASSIFICATION,
                                       &rule_cfg));
   rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_IPV4_SA;

   WPE_TEST_CHECK_RESULT(rule_handle,
                     WPI_BASIC_ERROR(WP_ERR_PCE_RULE_INCOMPATIBLE_FILTER_TYPE),
                     WP_PceObjectSearch (WP_WINPATH(DEFAULT_WPID),
                                       WP_PCE_OBJECT_TYPE_RULE_FORWARDING,
                                       &rule_cfg));

   WPE_TEST_CHECK_HANDLE(rule_handle,
                     WP_PceObjectSearch (WP_WINPATH(DEFAULT_WPID),
                                       WP_PCE_OBJECT_TYPE_RULE_CLASSIFICATION,
                                       &rule_cfg));
   MY_PRINTF("rule_handle = %X\n", rule_handle);

   /* Enable & Disable Test */
   WPE_TEST_CHECK_RESULT(status,
                     WPI_BASIC_ERROR(WP_ERR_HANDLE),
                     WP_PceRuleEnable(1));

   WPE_TEST_CHECK_RESULT(status,
                     WPI_BASIC_ERROR(WP_ERR_HANDLE),
                     WP_PceRuleEnable(rule_handles[0] + 40000));

   WPE_TEST_CHECK_RESULT(status,
                     WPI_BASIC_ERROR(WP_ERR_HANDLE),
                     WP_PceRuleEnable(rule_handles[0] + 10000));

   WPE_TEST_CHECK_RESULT(status,
                     WPI_BASIC_ERROR(WP_ERR_PCE_RULE_ALREADY_ENABLED),
                     WP_PceRuleEnable(rule_handles[2]));

   WPE_TEST_CHECK_RESULT(status,
                     WPI_BASIC_ERROR(WP_ERR_HANDLE),
                     WP_PceRuleDisable(1));

   WPE_TEST_CHECK_RESULT(status,
                     WPI_BASIC_ERROR(WP_ERR_HANDLE),
                     WP_PceRuleDisable(rule_handles[0] + 40000));

   WPE_TEST_CHECK_RESULT(status,
                     WPI_BASIC_ERROR(WP_ERR_HANDLE),
                     WP_PceRuleDisable(rule_handles[0] + 10000));

   WPE_TEST_CHECK_RESULT(status,
                     WPI_BASIC_ERROR(WP_ERR_PCE_RULE_ALREADY_DISABLED),
                     WP_PceRuleDisable(rule_handles[0]));

   for(i = 0; i <= 5; i++)
   {
      WPE_TEST_CHECK_RESULT(status,
                        WP_OK,
                        WP_PceRuleDelete(rule_handles[i]));
      MY_PRINTF("Removed rule with handle %X. Status = 0x%X.\n", rule_handles[i], status);
   }

/***************************************************************
 * Many Rules Test
 **************************************************************/
   MY_PRINTF("\n---- Many rules creating -------------------\n");
   rule_cfg.filter_handle = filter;
   rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_MAC_DA;
   for(i=0; i<6; i++)
      rule_cfg.rule_fields[0].value.mac_addr[i] = mac_addr[i];

   rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_MAC_SA;
   for(i=0; i<6; i++)
      rule_cfg.rule_fields[1].value.mac_addr[i] = mac_addr[i];

   rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_LAST;

   rule_cfg.match_action = WP_PCE_RULE_MATCH_CONTINUE;
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_LAST;

   srand(seed);
   //rules_num = 31900;
   for(i=0; i<rules_num; i++)
   {
      mac_addr[0] = (WP_U8) rand();
      mac_addr[1] = (WP_U8) rand();
      mac_addr[2] = (WP_U8) rand();
      mac_addr[3] = (WP_U8) rand();
      mac_addr[4] = (WP_U8) rand();
      mac_addr[5] = (WP_U8) rand();
      for(j=0; j<6; j++)
      {
         rule_cfg.rule_fields[0].value.mac_addr[j] = mac_addr[j];
         rule_cfg.rule_fields[1].value.mac_addr[j] = mac_addr[j];
      }

      MY_PRINTF("\n---- Rule no %5i Creating -- %02X %02X %02X %02X %02X %02X -------\n",
             i, mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
      if(i==5910)
         rules_num--;
      WPE_TEST_CHECK_HANDLE(rule_handles[i],
                        WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));
      MY_PRINTF("rule_handle[%X] = %X\n", i, rule_handles[i]);
   }
}

static void WPE_PceIteratorsTest(WP_U32 rules_num)
{
   WP_status status;
   WP_handle rule_handle, rule_handle1, rule_handle2;

   WPE_TEST_CHECK_RESULT(status,
                     WPI_BASIC_ERROR(WP_ERR_PCE_ITERATOR_TYPE_ILLEGAL),
                     WP_PceIteratorCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_ITERATOR_TYPE_RULE+100, &filter));

#if DEFAULT_WPID == 0
   WPE_TEST_CHECK_RESULT(status,
                     WPI_BASIC_ERROR(WP_ERR_HANDLE),
                     WP_PceIteratorCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_ITERATOR_TYPE_RULE, &(filter)+100));
#endif


   WPE_TEST_CHECK_HANDLE(iterator_handles[1],
                     WP_PceIteratorCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_ITERATOR_TYPE_AGED_RULE, (void *) WP_UNUSED));
   MY_PRINTF("iterator_handles[%X] = %X\n", 1, iterator_handles[1]);

   WPE_TEST_CHECK_RESULT(status,
                     WPI_BASIC_ERROR(WP_ERR_PCE_ITERATOR_ALREADY_EXISTS),
                     WP_PceIteratorCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_ITERATOR_TYPE_AGED_RULE, (void *) WP_UNUSED));

   WPE_TEST_CHECK_HANDLE(iterator_handles[2],
                     WP_PceIteratorCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_ITERATOR_TYPE_RULE, &filter));
   MY_PRINTF("iterator_handles[%X] = %X\n", 2, iterator_handles[2]);

   WPE_TEST_CHECK_RESULT(status,
                     WPI_BASIC_ERROR(WP_ERR_HANDLE),
                     WP_PceIteratorDelete(filter));

   WPE_TEST_CHECK_RESULT(status,
                     WPI_BASIC_ERROR(WP_ERR_HANDLE),
                     WP_PceIteratorDelete(iterator_handles[2]+1000));

   WPE_TEST_CHECK_RESULT(status,
                     WPI_BASIC_ERROR(WP_ERR_HANDLE),
                     WP_PceIteratorDelete(iterator_handles[2]+100));

   rule_handle1 = rule_handle = WP_PceIteratorGetNext(iterator_handles[2]);
   MY_PRINTF("Iterator 0x%X get next 0x%X\n", iterator_handles[1], rule_handle);
   while(rule_handle != WPI_BASIC_ERROR(WP_ERR_PCE_ITERATOR_NO_ITEMS))
   {
      rule_handle = WP_PceIteratorGetNext(iterator_handles[2]);
      MY_PRINTF("Iterator 0x%X get next 0x%X\n", iterator_handles[2], rule_handle);
   }

   if(rules_num >= 5)
   {
      MY_PRINTF("Deleting (middle) Rule Number %d: 0x%X\n", rules_num/2, rule_handles[rules_num/2]);
      WPE_TEST_CHECK_RESULT(status,
                        WP_OK,
                        WP_PceRuleDelete(rule_handles[rules_num/2]));
   }

   rule_handle2 = rule_handle = WP_PceIteratorGetFirst(iterator_handles[2]);
   if(rule_handle1 != rule_handle2)
      WPE_TEST_ADD_ERROR("Iterator First and Next Handle should be equal");
   MY_PRINTF("Iterator 0x%X get first 0x%X\n", iterator_handles[2], rule_handle);
   if(rules_num >= 5)
   {
      MY_PRINTF("Deleting (current) Rule Number %d: 0x%X\n", 1, rule_handles[1]);
      WPE_TEST_CHECK_RESULT(status,
                        WP_OK,
                        WP_PceRuleDelete(rule_handles[1]));
   }

   while(rule_handle != WPI_BASIC_ERROR(WP_ERR_PCE_ITERATOR_NO_ITEMS))
   {
      rule_handle = WP_PceIteratorGetNext(iterator_handles[2]);
      if(rule_handle2 == rule_handle)
         WPE_TEST_ADD_ERROR("Iterator First and Next Handle should NOT be equal");
      MY_PRINTF("Iterator 0x%X get next 0x%X\n", iterator_handles[2], rule_handle);
   }

   rule_handle1 = WP_PceIteratorGetNext(iterator_handles[0]);
   MY_PRINTF("\nIterator 0x%X get next 0x%X\n", iterator_handles[0], rule_handle);

   rule_handle = WP_PceIteratorGetFirst(iterator_handles[0]);
   MY_PRINTF("Iterator 0x%X get first 0x%X\n", iterator_handles[0], rule_handle);
   if(rule_handle1 != rule_handle)
      WPE_TEST_ADD_ERROR("Iterator First and Next Handle should equal");
   while(rule_handle != WPI_BASIC_ERROR(WP_ERR_PCE_ITERATOR_NO_ITEMS))
   {
      rule_handle = WP_PceIteratorGetNext(iterator_handles[0]);
      MY_PRINTF("Iterator 0x%X get next 0x%X\n", iterator_handles[0], rule_handle);
      if(rules_num >= 5 && rule_handle == rule_handles[rules_num-2])
      {
         MY_PRINTF("Deleting Last (current) Rule %d: 0x%X\n", rules_num-1, rule_handles[rules_num-1]);
         WPE_TEST_CHECK_RESULT(status,
                           WP_OK,
                           WP_PceRuleDelete(rule_handles[rules_num-1]));
      }
   }

   WPE_TEST_CHECK_RESULT(status,
                     WP_OK,
                     WP_PceIteratorDelete(iterator_handles[0]));

   WPE_TEST_CHECK_RESULT(status,
                     WP_OK,
                     WP_PceIteratorDelete(iterator_handles[1]));

   WPE_TEST_CHECK_RESULT(status,
                     WP_OK,
                     WP_PceIteratorDelete(iterator_handles[2]));

   /* filter, ext_filter, filter1, ext_filter1 */
}

static void WPE_PceRulesDeleteTest(WP_U16 rules_num)
{
   WP_status status;
   WP_U16 i;

   for(i=0; i<rules_num; i++)
   {
      if((rules_num >= 5 && i != 1 && i != (rules_num/2) && i != (rules_num-1))
         || (rules_num < 5))
      {
         WPE_TEST_CHECK_RESULT(status,
                        WP_OK,
                        WP_PceRuleDelete(rule_handles[i]));
         MY_PRINTF("Removed rule with handle %X. Status = 0x%X.\n", rule_handles[i], status);
      }
   }
}

static void WPE_PceRemarkedFieldsTest(void)
{
   WP_U16 filter_index = 0, rule_index = 0;
   WP_handle filters[10], rules[10];
   WP_pce_filter_classification filter_clas = {0};
   WP_pce_rule_classification rule_cfg = {0};
   WP_U8 mac_addr[] = {0xCA, 0xFE, 0xBB, 0xCC, 0xDD, 0xEE};
   WP_U32 i;

   filter_clas.no_match_action = WP_PCE_FILTER_NO_MATCH_CONTINUE;
   filter_clas.no_fields_action = WP_PCE_FILTER_NO_FIELDS_CONTINUE;
   filter_clas.no_match_result[0].result_type = WP_PCE_RESULT_LAST;

   MY_PRINTF("\n---- Remarked Fields Test ------------\n");
   MY_PRINTF("\n------------- Filter %d Creating: REM MAC_SA ----------\n", filter_index);
   filter_clas.filter_fields[0].field_id = WP_PCE_FIELD_ID_REMARKED_MAC_SA;
   filter_clas.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_clas.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;
   filter_clas.filter_fields[1].field_id = WP_PCE_FIELD_ID_LAST;

   WPE_TEST_CHECK_HANDLE(filters[filter_index],
                     WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_CLASSIFICATION, &filter_clas));
   MY_PRINTF("\n---- Filter %d Created: 0x%X ----------\n", filter_index, (WP_U32) filters[filter_index]);

   MY_PRINTF("\n------------- Rule %d Creating: REM MAC_SA ----------\n", rule_index);
   rule_cfg.enabled = WP_ENABLE;
   rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_REMARKED_MAC_SA;
   for(i=0; i<6; i++)
      rule_cfg.rule_fields[0].value.mac_addr[i] = mac_addr[i];
   rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_LAST;
   rule_cfg.match_action = WP_PCE_RULE_MATCH_CONTINUE;
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_LAST;
   rule_cfg.filter_handle = filters[rule_index];
   WPE_TEST_CHECK_HANDLE(rules[rule_index],
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));
   MY_PRINTF("\n---- Rule %d Created: 0x%X ----------\n", rule_index, (WP_U32) rules[rule_index]);

   MY_PRINTF("\n------------- Filter %d Creating: REM MAC_SA Mask 00 FF FF FF E0 00 ----------\n", ++filter_index);
   filter_clas.filter_fields[0].field_id = WP_PCE_FIELD_ID_REMARKED_MAC_SA;
   filter_clas.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_clas.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_USED;
   filter_clas.filter_fields[0].mask.mac_addr[0] = 0;
   filter_clas.filter_fields[0].mask.mac_addr[1] = 0xFF;
   filter_clas.filter_fields[0].mask.mac_addr[2] = 0xFF;
   filter_clas.filter_fields[0].mask.mac_addr[3] = 0xFF;
   filter_clas.filter_fields[0].mask.mac_addr[4] = 0xE0;
   filter_clas.filter_fields[0].mask.mac_addr[5] = 0;
   filter_clas.filter_fields[1].field_id = WP_PCE_FIELD_ID_LAST;

   WPE_TEST_CHECK_HANDLE(filters[filter_index],
                     WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_CLASSIFICATION, &filter_clas));
   MY_PRINTF("\n---- Filter %d Created: 0x%X ----------\n", filter_index, (WP_U32) filters[filter_index]);

   MY_PRINTF("\n------------- Rule %d Creating: REM MAC_SA masked ----------\n", ++rule_index);
   rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_REMARKED_MAC_SA;
   rule_cfg.filter_handle = filters[rule_index];
   WPE_TEST_CHECK_HANDLE(rules[rule_index],
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));
   MY_PRINTF("\n---- Rule %d Created: 0x%X ----------\n", rule_index, (WP_U32) rules[rule_index]);

   MY_PRINTF("\n------------- Filter %d Creating: REM MAC_DA ----------\n", ++filter_index);
   filter_clas.filter_fields[0].field_id = WP_PCE_FIELD_ID_REMARKED_MAC_DA;
   filter_clas.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_clas.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;
   filter_clas.filter_fields[1].field_id = WP_PCE_FIELD_ID_LAST;

   WPE_TEST_CHECK_HANDLE(filters[filter_index],
                     WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_CLASSIFICATION, &filter_clas));
   MY_PRINTF("\n---- Filter %d Created: 0x%X ----------\n", filter_index, (WP_U32) filters[filter_index]);

   MY_PRINTF("\n------------- Rule %d Creating: REM MAC_DA ----------\n", ++rule_index);
   rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_REMARKED_MAC_DA;
   rule_cfg.filter_handle = filters[rule_index];
   WPE_TEST_CHECK_HANDLE(rules[rule_index],
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));
   MY_PRINTF("\n---- Rule %d Created: 0x%X ----------\n", rule_index, (WP_U32) rules[rule_index]);

   MY_PRINTF("\n------------- Filter %d Creating: REM MAC_DA Mask 00 00 7F FF 00 00 00 ----------\n", ++filter_index);
   filter_clas.filter_fields[0].field_id = WP_PCE_FIELD_ID_REMARKED_MAC_DA;
   filter_clas.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_clas.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_USED;
   filter_clas.filter_fields[0].mask.mac_addr[0] = 0;
   filter_clas.filter_fields[0].mask.mac_addr[1] = 0;
   filter_clas.filter_fields[0].mask.mac_addr[2] = 0x7F;
   filter_clas.filter_fields[0].mask.mac_addr[3] = 0xFF;
   filter_clas.filter_fields[0].mask.mac_addr[4] = 0;
   filter_clas.filter_fields[0].mask.mac_addr[5] = 0;
   filter_clas.filter_fields[1].field_id = WP_PCE_FIELD_ID_LAST;

   WPE_TEST_CHECK_HANDLE(filters[filter_index],
                     WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_CLASSIFICATION, &filter_clas));
   MY_PRINTF("\n---- Filter %d Created: 0x%X ----------\n", filter_index, (WP_U32) filters[filter_index]);

   MY_PRINTF("\n------------- Rule %d Creating: REM MAC_DA masked ----------\n", ++rule_index);
   rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_REMARKED_MAC_DA;
   rule_cfg.filter_handle = filters[rule_index];
   WPE_TEST_CHECK_HANDLE(rules[rule_index],
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));
   MY_PRINTF("\n---- Rule %d Created: 0x%X ----------\n", rule_index, (WP_U32) rules[rule_index]);

   MY_PRINTF("\n------------- Filter %d Creating: REM MPLS LABEL ----------\n", ++filter_index);
   filter_clas.filter_fields[0].field_id = WP_PCE_FIELD_ID_REMARKED_MPLS_LABEL;
   filter_clas.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_clas.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;
   filter_clas.filter_fields[1].field_id = WP_PCE_FIELD_ID_LAST;

   WPE_TEST_CHECK_HANDLE(filters[filter_index],
                     WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_CLASSIFICATION, &filter_clas));
   MY_PRINTF("\n---- Filter %d Created: 0x%X ----------\n", filter_index, (WP_U32) filters[filter_index]);

   MY_PRINTF("\n------------- Rule %d Creating: REM MPLS LABEL ----------\n", ++rule_index);
   rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_REMARKED_MPLS_LABEL;
   rule_cfg.rule_fields[0].value.mpls_label = 0xFB963;
   rule_cfg.filter_handle = filters[rule_index];
   WPE_TEST_CHECK_HANDLE(rules[rule_index],
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));
   MY_PRINTF("\n---- Rule %d Created: 0x%X ----------\n", rule_index, (WP_U32) rules[rule_index]);

   MY_PRINTF("\n------------- Filter %d Creating: REM MPLS LABEL Mask 0003FFC0 ----------\n", ++filter_index);
   filter_clas.filter_fields[0].field_id = WP_PCE_FIELD_ID_REMARKED_MPLS_LABEL;
   filter_clas.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_clas.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_USED;
   filter_clas.filter_fields[0].mask.mpls_label = 0x0003FFC0;
   filter_clas.filter_fields[1].field_id = WP_PCE_FIELD_ID_LAST;

   WPE_TEST_CHECK_HANDLE(filters[filter_index],
                     WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_CLASSIFICATION, &filter_clas));
   MY_PRINTF("\n---- Filter %d Created: 0x%X ----------\n", filter_index, (WP_U32) filters[filter_index]);

   MY_PRINTF("\n------------- Rule %d Creating: REM MPLS LABEL masked ----------\n", ++rule_index);
   rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_REMARKED_MPLS_LABEL;
   rule_cfg.filter_handle = filters[rule_index];
   WPE_TEST_CHECK_HANDLE(rules[rule_index],
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));
   MY_PRINTF("\n---- Rule %d Created: 0x%X ----------\n", rule_index, (WP_U32) rules[rule_index]);

   MY_PRINTF("\n------------- Filter %d Creating: REM USER SPEC ----------\n", ++filter_index);
   filter_clas.filter_fields[0].field_id = WP_PCE_FIELD_ID_REMARKED_USER_SPECIFIC;
   filter_clas.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_clas.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;
   filter_clas.filter_fields[1].field_id = WP_PCE_FIELD_ID_LAST;

   WPE_TEST_CHECK_HANDLE(filters[filter_index],
                     WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_CLASSIFICATION, &filter_clas));
   MY_PRINTF("\n---- Filter %d Created: 0x%X ----------\n", filter_index, (WP_U32) filters[filter_index]);

   MY_PRINTF("\n------------- Rule %d Creating: REM USER SPEC ----------\n", ++rule_index);
   rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_REMARKED_USER_SPECIFIC;
   for(i=0; i<7; i++)
      rule_cfg.rule_fields[0].value.user_specific[i] = 0x11 * i;
   rule_cfg.filter_handle = filters[rule_index];
   WPE_TEST_CHECK_HANDLE(rules[rule_index],
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));
   MY_PRINTF("\n---- Rule %d Created: 0x%X ----------\n", rule_index, (WP_U32) rules[rule_index]);

   MY_PRINTF("\n------------- Filter %d Creating: REM USER SPEC Mask 00 00 7F FF F8 00 00 ----------\n", ++filter_index);
   filter_clas.filter_fields[0].field_id = WP_PCE_FIELD_ID_REMARKED_USER_SPECIFIC;
   filter_clas.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_clas.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_USED;
   filter_clas.filter_fields[0].mask.mac_addr[0] = 0;
   filter_clas.filter_fields[0].mask.mac_addr[1] = 0;
   filter_clas.filter_fields[0].mask.mac_addr[2] = 0x7F;
   filter_clas.filter_fields[0].mask.mac_addr[3] = 0xFF;
   filter_clas.filter_fields[0].mask.mac_addr[4] = 0xF8;
   filter_clas.filter_fields[0].mask.mac_addr[5] = 0;
   // filter_clas.filter_fields[0].mask.mac_addr[6] = 0;
   filter_clas.filter_fields[1].field_id = WP_PCE_FIELD_ID_LAST;

   WPE_TEST_CHECK_HANDLE(filters[filter_index],
                     WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_CLASSIFICATION, &filter_clas));
   MY_PRINTF("\n---- Filter %d Created: 0x%X ----------\n", filter_index, (WP_U32) filters[filter_index]);

   MY_PRINTF("\n------------- Rule %d Creating: REM USER SPEC masked ----------\n", ++rule_index);
   rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_REMARKED_USER_SPECIFIC;
   rule_cfg.filter_handle = filters[rule_index];
   WPE_TEST_CHECK_HANDLE(rules[rule_index],
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));
   MY_PRINTF("\n---- Rule %d Created: 0x%X ----------\n", rule_index, (WP_U32) rules[rule_index]);
}

static void WPE_PceRuleResultsTest(void)
{
   WP_U32 temp_rule_index = 0;
   WP_pce_rule_classification rule_cfg = {0};
   WP_handle policer_action_handle, policer_handle, policer_without_action;
   WP_U8 mac_addr[] = {0xCA, 0xFE, 0xBB, 0xCC, 0xDD, 0xEE};
   WP_U32 i;
   WP_handle temp_rule_handles[1000], rule_handle;
   WP_pce_result_flow_agg result_flow_agg;

   MY_PRINTF("\n----------------   PCE Rule Results Test Start   -------------------\n\n");
   rule_cfg.filter_handle = results_special_filter;
   rule_cfg.enabled = WP_ENABLE;
   rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_MAC_DA;
   for(i=0; i<6; i++)
      rule_cfg.rule_fields[0].value.mac_addr[i] = mac_addr[i];

   rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_MAC_SA;
   for(i=0; i<6; i++)
      rule_cfg.rule_fields[1].value.mac_addr[i] = mac_addr[i];

   rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_LAST;

   rule_cfg.match_action = WP_PCE_RULE_MATCH_CONTINUE;

   MY_PRINTF("\n----------------   Results: EXT VLAN - ERRORS  -------------------\n\n");
   rule_cfg.match_result[0].param.ext_vlan.vlan_tag = 0xABCD;
   rule_cfg.match_result[0].param.ext_vlan.mask = 0xEFFF;
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_EXT_VLAN_UPDATE;
   rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;

   rule_cfg.match_result[0].param.ext_vlan.mask = 0;
   WPE_TEST_CHECK_RESULT(rule_handle,
                     WPI_BASIC_ERROR(WP_ERR_PCE_RULE_RESULT_ILLEGAL_MASK),
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));
   rule_cfg.match_result[0].param.ext_vlan.mask = 0xEFFF;

   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_LAST + 1;
   rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;
   WPE_TEST_CHECK_RESULT(rule_handle,
                     WPI_BASIC_ERROR(WP_ERR_PCE_RULE_RESULT_TYPE_ILLEGAL),
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));
/*
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_INGRESS_POLICER;
   rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;
   WPE_TEST_CHECK_RESULT(rule_handle,
                     WPI_BASIC_ERROR(WP_ERR_PCE_RULE_RESULT_TYPE_UNSUPPORTED),
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));
*/
   MY_PRINTF("\n----------------   Results: FLOW AGG - ERROR  -------------------\n\n");

   result_flow_agg.flow_aggregation = 0; /* wrong flow agregation handle */
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
   rule_cfg.match_result[0].param.flow_agg = result_flow_agg;
   rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;
   WPE_TEST_CHECK_RESULT(rule_handle,
                     WPI_BASIC_ERROR(WP_ERR_HANDLE),
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));

   MY_PRINTF("\n----------------   Results: 2 x FLOW AGG - ERROR   -------------------\n\n");
   result_flow_agg.flow_aggregation = flow_aggregation3; /* wrong flow agregation handle */
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
   rule_cfg.match_result[0].param.flow_agg = result_flow_agg;
   rule_cfg.match_result[1].result_type = WP_PCE_RESULT_FLOW_AGG;
   rule_cfg.match_result[1].param.flow_agg = result_flow_agg;
   rule_cfg.match_result[2].result_type = WP_PCE_RESULT_LAST;

   WPE_TEST_CHECK_RESULT(rule_handle,
                     WPI_BASIC_ERROR(WP_ERR_PCE_RULE_RESULT_TYPE_REPEATED),
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));

   MY_PRINTF("\n----------------   Results: CLAS RULE AGING usage - ERROR   -------------------\n\n");
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
   rule_cfg.match_result[0].param.flow_agg = result_flow_agg;
   rule_cfg.match_result[1].result_type = WP_PCE_RESULT_AGING;
   rule_cfg.match_result[1].param.aging.mode = WP_PCE_AGING_ENABLE;
   rule_cfg.match_result[1].param.aging.weight = 1;
   rule_cfg.match_result[2].result_type = WP_PCE_RESULT_LAST;

   WPE_TEST_CHECK_RESULT(rule_handle,
                     WPI_BASIC_ERROR(WP_ERR_PCE_RULE_RESULT_ILLEGAL_AGING_USAGE),
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));

   MY_PRINTF("\n----------------   Results: size exceeeded - ERROR   -------------------\n\n");
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
   rule_cfg.match_result[0].param.flow_agg = result_flow_agg;
   rule_cfg.match_result[1].result_type = WP_PCE_RESULT_OUT_IW_PORT_UPDATE;
   rule_cfg.match_result[1].param.iw_port.iw_port_handle = bport_iw3;
   rule_cfg.match_result[2].result_type = WP_PCE_RESULT_EXT_VLAN_UPDATE;
   rule_cfg.match_result[2].param.ext_vlan.mask = WP_PCE_FIELD_MASK_VLAN_ID;
   rule_cfg.match_result[2].param.ext_vlan.vlan_tag = 0xABCD;
   rule_cfg.match_result[3].result_type = WP_PCE_RESULT_INT_VLAN_UPDATE;
   rule_cfg.match_result[3].param.ext_vlan.mask = WP_PCE_FIELD_MASK_VLAN_ID;
   rule_cfg.match_result[3].param.ext_vlan.vlan_tag = 0xABCD;
   rule_cfg.match_result[4].result_type = WP_PCE_RESULT_AGING;
   rule_cfg.match_result[4].param.aging.mode = WP_PCE_AGING_ENABLE;
   rule_cfg.match_result[4].param.aging.weight = 1;
   rule_cfg.match_result[5].result_type = WP_PCE_RESULT_LAST;

   WPE_TEST_CHECK_RESULT(rule_handle,
                     WPI_BASIC_ERROR(WP_ERR_PCE_RULE_RESULT_SIZE_EXCEEDED),
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));

   MY_PRINTF("\n----------------   Results: two updatings (HFR) - ERROR   -------------------\n\n");
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_IN_IW_PORT_UPDATE;
   rule_cfg.match_result[0].param.iw_port.iw_port_handle = bport_iw3;
   rule_cfg.match_result[1].result_type = WP_PCE_RESULT_OUT_IW_PORT_UPDATE;
   rule_cfg.match_result[1].param.iw_port.iw_port_handle = bport_iw3;
   rule_cfg.match_result[2].result_type = WP_PCE_RESULT_LAST;

   WPE_TEST_CHECK_RESULT(rule_handle,
                     WPI_BASIC_ERROR(WP_ERR_PCE_RULE_RESULT_UPDATES_EXCEEDED),
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));

   MY_PRINTF("\n----------------   Results: two updatings (HFR) - ERROR   -------------------\n\n");
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_REMARK_MPLS_LABEL;
   rule_cfg.match_result[0].param.mpls_label_remarking.mpls_label = 0x31A6D;
   rule_cfg.match_result[1].result_type = WP_PCE_RESULT_OUT_IW_PORT_UPDATE;
   rule_cfg.match_result[1].param.iw_port.iw_port_handle = bport_iw3;
   rule_cfg.match_result[2].result_type = WP_PCE_RESULT_LAST;

   WPE_TEST_CHECK_RESULT(rule_handle,
                     WPI_BASIC_ERROR(WP_ERR_PCE_RULE_RESULT_UPDATES_EXCEEDED),
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));

   MY_PRINTF("\n----------------   Results: Illegal MPLS label - ERROR   -------------------\n\n");
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_REMARK_MPLS_LABEL;
   rule_cfg.match_result[0].param.mpls_label_remarking.mpls_label = 0x1FFFFF;
   rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;

   WPE_TEST_CHECK_RESULT(rule_handle,
                     WPI_BASIC_ERROR(WP_ERR_PCE_RULE_RESULT_ILLEGAL_MPLS_LABEL),
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));

   MY_PRINTF("\n----------------   Results: Create legal MPLS label: (0xAB123) -------------------\n\n");
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_REMARK_MPLS_LABEL;
   rule_cfg.match_result[0].param.mpls_label_remarking.mpls_label = 0xAB123;
   rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;

   WPE_TEST_CHECK_HANDLE(temp_rule_handles[temp_rule_index],
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));

   rule_cfg.match_action = WP_PCE_RULE_MATCH_DENY;
   WPE_TEST_CHECK_RESULT(status,
                     WPI_BASIC_ERROR(WP_ERR_PCE_RULE_DENY_AND_RESULT),
                     WP_PceRuleModify(temp_rule_handles[temp_rule_index], WP_PCE_RULE_MODIFY_COMMAND_MATCH_ACTION, &rule_cfg));
   rule_cfg.match_action = WP_PCE_RULE_MATCH_CONTINUE;

   MY_PRINTF("\n----------------   Results: FAGG & MPLS label: (0xEA031) -------------------\n\n");
   rule_cfg.rule_fields[0].value.mac_addr[5] = temp_rule_index;
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_REMARK_MPLS_LABEL;
   rule_cfg.match_result[0].param.mpls_label_remarking.mpls_label = 0xEA031;
   rule_cfg.match_result[1].result_type = WP_PCE_RESULT_FLOW_AGG;
   rule_cfg.match_result[1].param.flow_agg.flow_aggregation = flow_aggregation3;
   rule_cfg.match_result[2].result_type = WP_PCE_RESULT_LAST;

   temp_rule_index++; WPE_TEST_CHECK_HANDLE(temp_rule_handles[temp_rule_index],
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));

   MY_PRINTF("\n----------------   Results: FAGG & MPLS label & VID : size exceeded error -----------\n\n");
   rule_cfg.rule_fields[0].value.mac_addr[5] = temp_rule_index;
   rule_cfg.match_result[0].param.mpls_label_remarking.mpls_label = 0xEA031;
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_REMARK_MPLS_LABEL;
   rule_cfg.match_result[1].param.flow_agg.flow_aggregation = flow_aggregation3;
   rule_cfg.match_result[1].result_type = WP_PCE_RESULT_FLOW_AGG;
   rule_cfg.match_result[2].param.ext_vlan.vlan_tag = 0xABCD;
   rule_cfg.match_result[2].param.ext_vlan.mask = WP_PCE_FIELD_MASK_VLAN_ID;
   rule_cfg.match_result[2].result_type = WP_PCE_RESULT_EXT_VLAN_UPDATE;
   rule_cfg.match_result[3].result_type = WP_PCE_RESULT_LAST;

   WPE_TEST_CHECK_RESULT(rule_handle,
                     WPI_BASIC_ERROR(WP_ERR_PCE_RULE_RESULT_SIZE_EXCEEDED),
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));

   MY_PRINTF("\n----------------   Results: PREF PRIO: type 0 error -----------\n\n");
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_PREFIX_PRIORITY_REMARKING;
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[0] = WP_PCE_RESULT_PPR_TYPE_DO_NOT_CHANGE;
   rule_cfg.match_result[0].param.prefix_priority_remarking.value[0] = 16;
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[1] = WP_PCE_RESULT_PPR_TYPE_DO_NOT_CHANGE;
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[2] = WP_PCE_RESULT_PPR_TYPE_DO_NOT_CHANGE;
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[3] = WP_PCE_RESULT_PPR_TYPE_DO_NOT_CHANGE;
   rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;

   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[0] = 100;
   WPE_TEST_CHECK_RESULT(rule_handle,
                     WPI_BASIC_ERROR(WP_ERR_PCE_RULE_RESULT_PPR_ILLEGAL_TYPE),
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));

   MY_PRINTF("\n----------------   Results: PREF PRIO: rem val 0 error -----------\n\n");
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[0] = WP_PCE_RESULT_PPR_TYPE_PREFIX_REMARKING_VAL;
   WPE_TEST_CHECK_RESULT(rule_handle,
                     WPI_BASIC_ERROR(WP_ERR_PCE_RULE_RESULT_PPR_ILLEGAL_VALUE),
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));

   MY_PRINTF("\n----------------   Results: PREF PRIO: ext vlan pri val error -----------\n\n");
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[0] = WP_PCE_RESULT_PPR_TYPE_EXT_VLAN_PRIO;
   rule_cfg.match_result[0].param.prefix_priority_remarking.value[0] = 0xA;
   WPE_TEST_CHECK_RESULT(rule_handle,
                     WPI_BASIC_ERROR(WP_ERR_PCE_RULE_RESULT_PPR_ILLEGAL_VALUE),
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));

   MY_PRINTF("\n----------------   Results: PREF PRIO: ext vlan dei val error -----------\n\n");
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[0] = WP_PCE_RESULT_PPR_TYPE_EXT_VLAN_DEI;
   rule_cfg.match_result[0].param.prefix_priority_remarking.value[0] = 0x3;
   WPE_TEST_CHECK_RESULT(rule_handle,
                     WPI_BASIC_ERROR(WP_ERR_PCE_RULE_RESULT_PPR_ILLEGAL_VALUE),
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));

   MY_PRINTF("\n----------------   Results: PREF PRIO: ext vlan prio dei val error -----------\n\n");
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[0] = WP_PCE_RESULT_PPR_TYPE_EXT_VLAN_PRIO_DEI;
   rule_cfg.match_result[0].param.prefix_priority_remarking.value[0] = 0x15;
   WPE_TEST_CHECK_RESULT(rule_handle,
                     WPI_BASIC_ERROR(WP_ERR_PCE_RULE_RESULT_PPR_ILLEGAL_VALUE),
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));

   MY_PRINTF("\n----------------   Results: PREF PRIO: type 1 error -----------\n\n");
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[1] = 45;
   WPE_TEST_CHECK_RESULT(rule_handle,
                     WPI_BASIC_ERROR(WP_ERR_PCE_RULE_RESULT_PPR_ILLEGAL_TYPE),
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));

   MY_PRINTF("\n----------------   Results: PREF PRIO: rem val 1 error -----------\n\n");
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[1] = WP_PCE_RESULT_PPR_TYPE_PREFIX_REMARKING_VAL;
   rule_cfg.match_result[0].param.prefix_priority_remarking.value[1] = 0x71;
   WPE_TEST_CHECK_RESULT(rule_handle,
                     WPI_BASIC_ERROR(WP_ERR_PCE_RULE_RESULT_PPR_ILLEGAL_VALUE),
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));

   MY_PRINTF("\n----------------   Results: PREF PRIO: int vlan pri val error -----------\n\n");
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[1] = WP_PCE_RESULT_PPR_TYPE_INT_VLAN_PRIO;
   rule_cfg.match_result[0].param.prefix_priority_remarking.value[1] = 0xAA;
   WPE_TEST_CHECK_RESULT(rule_handle,
                     WPI_BASIC_ERROR(WP_ERR_PCE_RULE_RESULT_PPR_ILLEGAL_VALUE),
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));

   MY_PRINTF("\n----------------   Results: PREF PRIO: int vlan dei val error -----------\n\n");
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[1] = WP_PCE_RESULT_PPR_TYPE_INT_VLAN_DEI;
   rule_cfg.match_result[0].param.prefix_priority_remarking.value[1] = 0x5;
   WPE_TEST_CHECK_RESULT(rule_handle,
                     WPI_BASIC_ERROR(WP_ERR_PCE_RULE_RESULT_PPR_ILLEGAL_VALUE),
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));

   MY_PRINTF("\n----------------   Results: PREF PRIO: int vlan prio dei val error -----------\n\n");
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[1] = WP_PCE_RESULT_PPR_TYPE_INT_VLAN_PRIO_DEI;
   rule_cfg.match_result[0].param.prefix_priority_remarking.value[1] = 0x17;
   WPE_TEST_CHECK_RESULT(rule_handle,
                     WPI_BASIC_ERROR(WP_ERR_PCE_RULE_RESULT_PPR_ILLEGAL_VALUE),
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));

   MY_PRINTF("\n----------------   Results: PREF PRIO: type 2 error -----------\n\n");
   rule_cfg.match_result[0].param.prefix_priority_remarking.value[1] = 0xA;
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[2] = 59;
   rule_cfg.match_result[0].param.prefix_priority_remarking.value[2] = 0x1;
   WPE_TEST_CHECK_RESULT(rule_handle,
                     WPI_BASIC_ERROR(WP_ERR_PCE_RULE_RESULT_PPR_ILLEGAL_TYPE),
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));

   MY_PRINTF("\n----------------   Results: PREF PRIO: rem val 2 error -----------\n\n");
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[2] = WP_PCE_RESULT_PPR_TYPE_PREFIX_REMARKING_VAL;
   rule_cfg.match_result[0].param.prefix_priority_remarking.value[2] = 0x10;
   WPE_TEST_CHECK_RESULT(rule_handle,
                     WPI_BASIC_ERROR(WP_ERR_PCE_RULE_RESULT_PPR_ILLEGAL_VALUE),
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));

   MY_PRINTF("\n----------------   Results: PREF PRIO: type 3 error -----------\n\n");
   rule_cfg.match_result[0].param.prefix_priority_remarking.value[2] = 0xA;
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[3] = 72;
   rule_cfg.match_result[0].param.prefix_priority_remarking.value[3] = 0x1;
   WPE_TEST_CHECK_RESULT(rule_handle,
                     WPI_BASIC_ERROR(WP_ERR_PCE_RULE_RESULT_PPR_ILLEGAL_TYPE),
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));

   MY_PRINTF("\n----------------   Results: PREF PRIO: rem val 3 error -----------\n\n");
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[3] = WP_PCE_RESULT_PPR_TYPE_PREFIX_REMARKING_VAL;
   rule_cfg.match_result[0].param.prefix_priority_remarking.value[3] = 0x10;
   WPE_TEST_CHECK_RESULT(rule_handle,
                     WPI_BASIC_ERROR(WP_ERR_PCE_RULE_RESULT_PPR_ILLEGAL_VALUE),
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));

   MY_PRINTF("\n----------------   Results: PREF PRIO: tos dscp val error -----------\n\n");
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[3] = WP_PCE_RESULT_PPR_TYPE_TOS_DSCP;
   rule_cfg.match_result[0].param.prefix_priority_remarking.value[3] = 0x45;
   WPE_TEST_CHECK_RESULT(rule_handle,
                     WPI_BASIC_ERROR(WP_ERR_PCE_RULE_RESULT_PPR_ILLEGAL_VALUE),
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));

   MY_PRINTF("\n----------------   Results: PREF PRIO: tos prec val error -----------\n\n");
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[3] = WP_PCE_RESULT_PPR_TYPE_TOS_PRECEDENCE;
   rule_cfg.match_result[0].param.prefix_priority_remarking.value[3] = 0xE;
   WPE_TEST_CHECK_RESULT(rule_handle,
                     WPI_BASIC_ERROR(WP_ERR_PCE_RULE_RESULT_PPR_ILLEGAL_VALUE),
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));

   MY_PRINTF("\n----------------   Results: PREF PRIO: tos dscp val error -----------\n\n");
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[3] = WP_PCE_RESULT_PPR_TYPE_TOS_DSCP_DROP_PRECEDENCE;
   rule_cfg.match_result[0].param.prefix_priority_remarking.value[3] = 0x8;
   WPE_TEST_CHECK_RESULT(rule_handle,
                     WPI_BASIC_ERROR(WP_ERR_PCE_RULE_RESULT_PPR_ILLEGAL_VALUE),
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));

   MY_PRINTF("\n----------------   Results: PREF PRIO: nothing to change error -----------\n\n");
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[0] = WP_PCE_RESULT_PPR_TYPE_DO_NOT_CHANGE;
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[1] = WP_PCE_RESULT_PPR_TYPE_DO_NOT_CHANGE;
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[2] = WP_PCE_RESULT_PPR_TYPE_DO_NOT_CHANGE;
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[3] = WP_PCE_RESULT_PPR_TYPE_DO_NOT_CHANGE;
   WPE_TEST_CHECK_RESULT(rule_handle,
                     WPI_BASIC_ERROR(WP_ERR_PCE_RULE_RESULT_PPR_NOTHING_TO_CHANGE),
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));

   MY_PRINTF("\n----------------   Results: PREF PRIO: pref rem: 0x9 0x7 0x1 0xE ----------------\n\n");
   rule_cfg.rule_fields[0].value.mac_addr[5] = temp_rule_index;
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_PREFIX_PRIORITY_REMARKING;
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[0] = WP_PCE_RESULT_PPR_TYPE_PREFIX_REMARKING_VAL;
   rule_cfg.match_result[0].param.prefix_priority_remarking.value[0] = 0x9;
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[1] = WP_PCE_RESULT_PPR_TYPE_PREFIX_REMARKING_VAL;
   rule_cfg.match_result[0].param.prefix_priority_remarking.value[1] = 0x7;
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[2] = WP_PCE_RESULT_PPR_TYPE_PREFIX_REMARKING_VAL;
   rule_cfg.match_result[0].param.prefix_priority_remarking.value[2] = 0xE;
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[3] = WP_PCE_RESULT_PPR_TYPE_PREFIX_REMARKING_VAL;
   rule_cfg.match_result[0].param.prefix_priority_remarking.value[3] = 0x1;
   rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;
   temp_rule_index++; WPE_TEST_CHECK_HANDLE(temp_rule_handles[temp_rule_index],
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));

   MY_PRINTF("\n----------------   Results: PREF PRIO: pref rem: 0x9 0x7 XXX XXX ----------------\n\n");
   rule_cfg.rule_fields[0].value.mac_addr[5] = temp_rule_index;
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_PREFIX_PRIORITY_REMARKING;
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[0] = WP_PCE_RESULT_PPR_TYPE_PREFIX_REMARKING_VAL;
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[1] = WP_PCE_RESULT_PPR_TYPE_PREFIX_REMARKING_VAL;
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[2] = WP_PCE_RESULT_PPR_TYPE_DO_NOT_CHANGE;
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[3] = WP_PCE_RESULT_PPR_TYPE_DO_NOT_CHANGE;
   rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;
   temp_rule_index++; WPE_TEST_CHECK_HANDLE(temp_rule_handles[temp_rule_index],
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));

   MY_PRINTF("\n----------------   Results: PREF PRIO: pref rem: XXX XXX 0x1 0xE ----------------\n\n");
   rule_cfg.rule_fields[0].value.mac_addr[5] = temp_rule_index;
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_PREFIX_PRIORITY_REMARKING;
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[0] = WP_PCE_RESULT_PPR_TYPE_DO_NOT_CHANGE;
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[1] = WP_PCE_RESULT_PPR_TYPE_DO_NOT_CHANGE;
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[2] = WP_PCE_RESULT_PPR_TYPE_PREFIX_REMARKING_VAL;
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[3] = WP_PCE_RESULT_PPR_TYPE_PREFIX_REMARKING_VAL;
   rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;
   temp_rule_index++; WPE_TEST_CHECK_HANDLE(temp_rule_handles[temp_rule_index],
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));

   MY_PRINTF("\n----------------   Results: PREF PRIO: pref rem: XXX 0x7 XXX 0xE ----------------\n\n");
   rule_cfg.rule_fields[0].value.mac_addr[5] = temp_rule_index;
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_PREFIX_PRIORITY_REMARKING;
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[0] = WP_PCE_RESULT_PPR_TYPE_DO_NOT_CHANGE;
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[1] = WP_PCE_RESULT_PPR_TYPE_PREFIX_REMARKING_VAL;
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[2] = WP_PCE_RESULT_PPR_TYPE_DO_NOT_CHANGE;
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[3] = WP_PCE_RESULT_PPR_TYPE_PREFIX_REMARKING_VAL;
   rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;
   temp_rule_index++; WPE_TEST_CHECK_HANDLE(temp_rule_handles[temp_rule_index],
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));

   MY_PRINTF("\n----------------   Results: PREF PRIO: pref rem: XXX 0x7 XXX XXX ----------------\n\n");
   rule_cfg.rule_fields[0].value.mac_addr[5] = temp_rule_index;
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_PREFIX_PRIORITY_REMARKING;
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[0] = WP_PCE_RESULT_PPR_TYPE_DO_NOT_CHANGE;
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[1] = WP_PCE_RESULT_PPR_TYPE_PREFIX_REMARKING_VAL;
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[2] = WP_PCE_RESULT_PPR_TYPE_DO_NOT_CHANGE;
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[3] = WP_PCE_RESULT_PPR_TYPE_DO_NOT_CHANGE;
   rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;
   temp_rule_index++; WPE_TEST_CHECK_HANDLE(temp_rule_handles[temp_rule_index],
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));

   MY_PRINTF("\n----------------   Results: PREF PRIO: pref rem: XXX XXX XXX 0xE ----------------\n\n");
   rule_cfg.rule_fields[0].value.mac_addr[5] = temp_rule_index;
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_PREFIX_PRIORITY_REMARKING;
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[0] = WP_PCE_RESULT_PPR_TYPE_DO_NOT_CHANGE;
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[1] = WP_PCE_RESULT_PPR_TYPE_DO_NOT_CHANGE;
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[2] = WP_PCE_RESULT_PPR_TYPE_DO_NOT_CHANGE;
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[3] = WP_PCE_RESULT_PPR_TYPE_PREFIX_REMARKING_VAL;
   rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;
   temp_rule_index++; WPE_TEST_CHECK_HANDLE(temp_rule_handles[temp_rule_index],
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));

   MY_PRINTF("\n----------------   Results: PREF PRIO: pref rem: 0x9 0x7 XXX 0xE ----------------\n\n");
   rule_cfg.rule_fields[0].value.mac_addr[5] = temp_rule_index;
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_PREFIX_PRIORITY_REMARKING;
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[0] = WP_PCE_RESULT_PPR_TYPE_PREFIX_REMARKING_VAL;
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[1] = WP_PCE_RESULT_PPR_TYPE_PREFIX_REMARKING_VAL;
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[2] = WP_PCE_RESULT_PPR_TYPE_DO_NOT_CHANGE;
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[3] = WP_PCE_RESULT_PPR_TYPE_PREFIX_REMARKING_VAL;
   rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;
   temp_rule_index++; WPE_TEST_CHECK_HANDLE(temp_rule_handles[temp_rule_index],
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));

   MY_PRINTF("\n----------------   Results: PREF PRIO: pref rem: 0x9 0x7 0x1 XXX ----------------\n\n");
   rule_cfg.rule_fields[0].value.mac_addr[5] = temp_rule_index;
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_PREFIX_PRIORITY_REMARKING;
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[0] = WP_PCE_RESULT_PPR_TYPE_PREFIX_REMARKING_VAL;
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[1] = WP_PCE_RESULT_PPR_TYPE_PREFIX_REMARKING_VAL;
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[2] = WP_PCE_RESULT_PPR_TYPE_PREFIX_REMARKING_VAL;
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[3] = WP_PCE_RESULT_PPR_TYPE_DO_NOT_CHANGE;
   rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;
   temp_rule_index++; WPE_TEST_CHECK_HANDLE(temp_rule_handles[temp_rule_index],
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));

   MY_PRINTF("\n----------------   Results: PREF PRIO: pref rem: XXX 0x7 0x1 0xE ----------------\n\n");
   rule_cfg.rule_fields[0].value.mac_addr[5] = temp_rule_index;
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_PREFIX_PRIORITY_REMARKING;
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[0] = WP_PCE_RESULT_PPR_TYPE_DO_NOT_CHANGE;
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[1] = WP_PCE_RESULT_PPR_TYPE_PREFIX_REMARKING_VAL;
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[2] = WP_PCE_RESULT_PPR_TYPE_PREFIX_REMARKING_VAL;
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[3] = WP_PCE_RESULT_PPR_TYPE_PREFIX_REMARKING_VAL;
   rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;
   temp_rule_index++; WPE_TEST_CHECK_HANDLE(temp_rule_handles[temp_rule_index],
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));

   MY_PRINTF("\n----------------   Results: PREF PRIO: pref rem: 0x9 XXX 0x1 0xE ----------------\n\n");
   rule_cfg.rule_fields[0].value.mac_addr[5] = temp_rule_index;
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_PREFIX_PRIORITY_REMARKING;
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[0] = WP_PCE_RESULT_PPR_TYPE_PREFIX_REMARKING_VAL;
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[1] = WP_PCE_RESULT_PPR_TYPE_DO_NOT_CHANGE;
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[2] = WP_PCE_RESULT_PPR_TYPE_PREFIX_REMARKING_VAL;
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[3] = WP_PCE_RESULT_PPR_TYPE_PREFIX_REMARKING_VAL;
   rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;
   temp_rule_index++; WPE_TEST_CHECK_HANDLE(temp_rule_handles[temp_rule_index],
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));

   MY_PRINTF("\n----------------   Results: PREF PRIO: evpt 3 idei 1 XXX dscp 0x15 ----------------\n\n");
   rule_cfg.rule_fields[0].value.mac_addr[5] = temp_rule_index;
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_PREFIX_PRIORITY_REMARKING;
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[0] = WP_PCE_RESULT_PPR_TYPE_EXT_VLAN_PRIO;
   rule_cfg.match_result[0].param.prefix_priority_remarking.value[0] = 0x3;
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[1] = WP_PCE_RESULT_PPR_TYPE_INT_VLAN_DEI;
   rule_cfg.match_result[0].param.prefix_priority_remarking.value[1] = 0x1;
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[2] = WP_PCE_RESULT_PPR_TYPE_DO_NOT_CHANGE;
   rule_cfg.match_result[0].param.prefix_priority_remarking.value[2] = 0xFF;
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[3] = WP_PCE_RESULT_PPR_TYPE_TOS_DSCP;
   rule_cfg.match_result[0].param.prefix_priority_remarking.value[3] = 0x15;
   rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;
   temp_rule_index++; WPE_TEST_CHECK_HANDLE(temp_rule_handles[temp_rule_index],
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));

   MY_PRINTF("\n----------------   Results: PREF PRIO: edei 1 ivpt 5 rem2 0x9 tos 0xED ----------------\n\n");
   rule_cfg.rule_fields[0].value.mac_addr[5] = temp_rule_index;
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_PREFIX_PRIORITY_REMARKING;
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[0] = WP_PCE_RESULT_PPR_TYPE_EXT_VLAN_DEI;
   rule_cfg.match_result[0].param.prefix_priority_remarking.value[0] = 0x1;
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[1] = WP_PCE_RESULT_PPR_TYPE_INT_VLAN_PRIO;
   rule_cfg.match_result[0].param.prefix_priority_remarking.value[1] = 0x5;
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[2] = WP_PCE_RESULT_PPR_TYPE_PREFIX_REMARKING_VAL;
   rule_cfg.match_result[0].param.prefix_priority_remarking.value[2] = 0x9;
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[3] = WP_PCE_RESULT_PPR_TYPE_TOS;
   rule_cfg.match_result[0].param.prefix_priority_remarking.value[3] = 0xED;
   rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;
   temp_rule_index++; WPE_TEST_CHECK_HANDLE(temp_rule_handles[temp_rule_index],
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));

   MY_PRINTF("\n----------------   Results: PREF PRIO: edpt 0xE idpt 0xB rem2 0x4 precedence 0x5 ----------------\n\n");
   rule_cfg.rule_fields[0].value.mac_addr[5] = temp_rule_index;
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_PREFIX_PRIORITY_REMARKING;
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[0] = WP_PCE_RESULT_PPR_TYPE_EXT_VLAN_PRIO_DEI;
   rule_cfg.match_result[0].param.prefix_priority_remarking.value[0] = 0xE;
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[1] = WP_PCE_RESULT_PPR_TYPE_INT_VLAN_PRIO_DEI;
   rule_cfg.match_result[0].param.prefix_priority_remarking.value[1] = 0xB;
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[2] = WP_PCE_RESULT_PPR_TYPE_PREFIX_REMARKING_VAL;
   rule_cfg.match_result[0].param.prefix_priority_remarking.value[2] = 0x4;
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[3] = WP_PCE_RESULT_PPR_TYPE_TOS_PRECEDENCE;
   rule_cfg.match_result[0].param.prefix_priority_remarking.value[3] = 0x5;
   rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;
   temp_rule_index++; WPE_TEST_CHECK_HANDLE(temp_rule_handles[temp_rule_index],
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));

   MY_PRINTF("\n----------------   Results: PREF PRIO: edpt 0xA XXX XXX dscp/prec 0x3 ----------------\n\n");
   rule_cfg.rule_fields[0].value.mac_addr[5] = temp_rule_index;
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_PREFIX_PRIORITY_REMARKING;
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[0] = WP_PCE_RESULT_PPR_TYPE_EXT_VLAN_PRIO_DEI;
   rule_cfg.match_result[0].param.prefix_priority_remarking.value[0] = 0xA;
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[1] = WP_PCE_RESULT_PPR_TYPE_DO_NOT_CHANGE;
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[2] = WP_PCE_RESULT_PPR_TYPE_DO_NOT_CHANGE;
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[3] = WP_PCE_RESULT_PPR_TYPE_TOS_DSCP_DROP_PRECEDENCE;
   rule_cfg.match_result[0].param.prefix_priority_remarking.value[3] = 0x3;
   rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;
   temp_rule_index++; WPE_TEST_CHECK_HANDLE(temp_rule_handles[temp_rule_index],
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));

   MY_PRINTF("\n----------------   Results: PREF PRIO: edpt 0xA XXX & FLOW AGG ----------------\n\n");
   rule_cfg.rule_fields[0].value.mac_addr[5] = temp_rule_index;
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_PREFIX_PRIORITY_REMARKING;
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[0] = WP_PCE_RESULT_PPR_TYPE_EXT_VLAN_PRIO_DEI;
   rule_cfg.match_result[0].param.prefix_priority_remarking.value[0] = 0xA;
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[1] = WP_PCE_RESULT_PPR_TYPE_DO_NOT_CHANGE;
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[2] = WP_PCE_RESULT_PPR_TYPE_DO_NOT_CHANGE;
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[3] = WP_PCE_RESULT_PPR_TYPE_DO_NOT_CHANGE;
   rule_cfg.match_result[1].result_type = WP_PCE_RESULT_FLOW_AGG;
   rule_cfg.match_result[1].param.flow_agg.flow_aggregation = flow_aggregation3;
   rule_cfg.match_result[2].result_type = WP_PCE_RESULT_LAST;
   temp_rule_index++; WPE_TEST_CHECK_HANDLE(temp_rule_handles[temp_rule_index],
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));

   MY_PRINTF("\n----------------   Results: PREF PRIO: rem0 0xE idpt 0xB & FLOW AGG & LRN FLOW AGG & FS change-------\n\n");

   rule_cfg.rule_fields[0].value.mac_addr[5] = temp_rule_index;
#if MORRIS_UPDATE_VLAN
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_EXT_VLAN_UPDATE;
   rule_cfg.match_result[0].param.ext_vlan.vlan_tag = 0x1234;
   rule_cfg.match_result[0].param.ext_vlan.mask = WP_PCE_FIELD_MASK_VLAN_ID;
#else
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_PREFIX_PRIORITY_REMARKING;
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[0] = WP_PCE_RESULT_PPR_TYPE_PREFIX_REMARKING_VAL;
   rule_cfg.match_result[0].param.prefix_priority_remarking.value[0] = 0xE;
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[1] = WP_PCE_RESULT_PPR_TYPE_INT_VLAN_PRIO_DEI;
   rule_cfg.match_result[0].param.prefix_priority_remarking.value[0] = 0xB;
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[2] = WP_PCE_RESULT_PPR_TYPE_DO_NOT_CHANGE;
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[3] = WP_PCE_RESULT_PPR_TYPE_DO_NOT_CHANGE;
#endif

   rule_cfg.match_result[1].result_type = WP_PCE_RESULT_FLOW_AGG;
   rule_cfg.match_result[1].param.flow_agg.flow_aggregation = flow_aggregation3;
   rule_cfg.match_result[2].result_type = WP_PCE_RESULT_LEARNING_FLOW_AGG;
   rule_cfg.match_result[2].param.learning_flow_agg.flow_aggregation = flow_aggregation3;
   rule_cfg.match_result[3].result_type = WP_PCE_RESULT_FILTER_SET_CHANGE;
   rule_cfg.match_result[3].param.change_fs.filter_set_handle = filter_set_lvl1_handle;
   rule_cfg.match_result[4].result_type = WP_PCE_RESULT_LAST;

#if MORRIS_UPDATE_VLAN
// #warning MORRIS_UPDATE_VLAN_defined
   rule_cfg.match_result[1].result_type = WP_PCE_RESULT_FILTER_SET_CHANGE;
   rule_cfg.match_result[1].param.change_fs.filter_set_handle = filter_set_lvl1_handle;
   rule_cfg.match_result[2].result_type = WP_PCE_RESULT_LAST;
#else
#error MORRIS_UPDATE_VLAN_should_be_defined
#endif 

   temp_rule_index++; WPE_TEST_CHECK_HANDLE(temp_rule_handles[temp_rule_index],
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));

   MY_PRINTF("\n----------------   Results: PREF PRIO masked & MPLS label error ----------------\n\n");
   rule_cfg.rule_fields[0].value.mac_addr[5] = temp_rule_index;
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_PREFIX_PRIORITY_REMARKING;
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[0] = WP_PCE_RESULT_PPR_TYPE_DO_NOT_CHANGE;
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[1] = WP_PCE_RESULT_PPR_TYPE_INT_VLAN_PRIO_DEI;
   rule_cfg.match_result[0].param.prefix_priority_remarking.value[1] = 0xA;
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[2] = WP_PCE_RESULT_PPR_TYPE_DO_NOT_CHANGE;
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[3] = WP_PCE_RESULT_PPR_TYPE_DO_NOT_CHANGE;
   rule_cfg.match_result[1].result_type = WP_PCE_RESULT_REMARK_MPLS_LABEL;
   rule_cfg.match_result[1].param.mpls_label_remarking.mpls_label = 0xABCDE;
   rule_cfg.match_result[2].result_type = WP_PCE_RESULT_LAST;
   WPE_TEST_CHECK_RESULT(rule_handle,
                     WPI_BASIC_ERROR(WP_ERR_PCE_RULE_RESULT_ILLEGAL_COMBINATION),
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));

   MY_PRINTF("\n----------------   Results: IN IW PORT illegal handle ----------------\n\n");
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_IN_IW_PORT_UPDATE;
   rule_cfg.match_result[0].param.iw_port.iw_port_handle = filter_set_lvl1_handle;
   rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;
   WPE_TEST_CHECK_RESULT(rule_handle,
                     WPI_BASIC_ERROR(WP_ERR_HANDLE),
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));

   MY_PRINTF("\n----------------   Results: IN IW PORT ----------------\n\n");
   rule_cfg.rule_fields[0].value.mac_addr[5] = temp_rule_index;
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_IN_IW_PORT_UPDATE;
   rule_cfg.match_result[0].param.iw_port.iw_port_handle = bport_iw3;
   rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;
   temp_rule_index++; WPE_TEST_CHECK_HANDLE(temp_rule_handles[temp_rule_index],
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));

   MY_PRINTF("\n----------------   Results: FS CHANGE IN IW PORT ----------------\n\n");
   rule_cfg.rule_fields[0].value.mac_addr[5] = temp_rule_index;
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FILTER_SET_CHANGE;
   rule_cfg.match_result[0].param.change_fs.filter_set_handle = filter_set_lvl1_handle;
   rule_cfg.match_result[1].result_type = WP_PCE_RESULT_IN_IW_PORT_UPDATE;
   rule_cfg.match_result[1].param.iw_port.iw_port_handle = bport_iw3;
   rule_cfg.match_result[2].result_type = WP_PCE_RESULT_LAST;
   temp_rule_index++; WPE_TEST_CHECK_HANDLE(temp_rule_handles[temp_rule_index],
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));

   MY_PRINTF("\n----------------   Results: UPD EXT VID ----------------\n\n");
   rule_cfg.rule_fields[0].value.mac_addr[5] = temp_rule_index;
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_EXT_VLAN_UPDATE;
   rule_cfg.match_result[0].param.ext_vlan.vlan_tag = 0x1234;
   rule_cfg.match_result[0].param.ext_vlan.mask = WP_PCE_FIELD_MASK_VLAN_ID;
   rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;
   temp_rule_index++; WPE_TEST_CHECK_HANDLE(temp_rule_handles[temp_rule_index],
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));

   MY_PRINTF("\n----------------   Results: UPD INT VID ----------------\n\n");
   rule_cfg.rule_fields[0].value.mac_addr[5] = temp_rule_index;
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_INT_VLAN_UPDATE;
   rule_cfg.match_result[0].param.int_vlan.vlan_tag = 0xABCD;
   rule_cfg.match_result[0].param.int_vlan.mask = WP_PCE_FIELD_MASK_VLAN_ID;
   rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;
   temp_rule_index++; WPE_TEST_CHECK_HANDLE(temp_rule_handles[temp_rule_index],
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));

   MY_PRINTF("\n----------------   Results: PREF PRIO: pref_vprio 0x7 (see 0xE) pref_exp 0x5 XXX XXX ----------------\n\n");
   rule_cfg.rule_fields[0].value.mac_addr[5] = temp_rule_index;
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_PREFIX_PRIORITY_REMARKING;
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[0] = WP_PCE_RESULT_PPR_TYPE_PREFIX_VLAN_PRIO;
   rule_cfg.match_result[0].param.prefix_priority_remarking.value[0] = 0x7;
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[1] = WP_PCE_RESULT_PPR_TYPE_PREFIX_MPLS_EXP;
   rule_cfg.match_result[0].param.prefix_priority_remarking.value[1] = 0x5;
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[2] = WP_PCE_RESULT_PPR_TYPE_DO_NOT_CHANGE;
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[3] = WP_PCE_RESULT_PPR_TYPE_DO_NOT_CHANGE;
   rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;
   temp_rule_index++; WPE_TEST_CHECK_HANDLE(temp_rule_handles[temp_rule_index],
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));

   MY_PRINTF("\n----------------   Results: ING POLICER COLOR RED  ----------------\n\n");
   rule_cfg.rule_fields[0].value.mac_addr[5] = temp_rule_index;
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_INGRESS_POLICER_INPUT_COLOR;
   rule_cfg.match_result[0].param.policer_color.color = WP_IW_TCM_INPUT_COLOR_RED;
   rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;
   temp_rule_index++; WPE_TEST_CHECK_HANDLE(temp_rule_handles[temp_rule_index],
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));

   MY_PRINTF("\n----------------   Results: NUM MPLS POP 5  ----------------\n\n");
   rule_cfg.rule_fields[0].value.mac_addr[5] = temp_rule_index;
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_NUM_OF_MPLS_HEADERS_TO_POP;
   rule_cfg.match_result[0].param.num_mpls_headers_to_pop.number = 5;
   rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;
   temp_rule_index++; WPE_TEST_CHECK_HANDLE(temp_rule_handles[temp_rule_index],
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));

   MY_PRINTF("\n----------------   Results: NUM MPLS POP 3 & ING POLICER COLOR YELLOW  ----------------\n\n");
   rule_cfg.rule_fields[0].value.mac_addr[5] = temp_rule_index;
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_NUM_OF_MPLS_HEADERS_TO_POP;
   rule_cfg.match_result[0].param.num_mpls_headers_to_pop.number = 3;
   rule_cfg.match_result[1].result_type = WP_PCE_RESULT_INGRESS_POLICER_INPUT_COLOR;
   rule_cfg.match_result[1].param.policer_color.color = WP_IW_TCM_INPUT_COLOR_YELLOW;
   rule_cfg.match_result[2].result_type = WP_PCE_RESULT_LAST;
   temp_rule_index++; WPE_TEST_CHECK_HANDLE(temp_rule_handles[temp_rule_index],
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));

   MY_PRINTF("\n----------------   Results: NUM MPLS POP 3 & ING POLICER COLOR YELLOW & IW OUT PORT ----------------\n\n");
   rule_cfg.rule_fields[0].value.mac_addr[5] = temp_rule_index;
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_NUM_OF_MPLS_HEADERS_TO_POP;
   rule_cfg.match_result[0].param.num_mpls_headers_to_pop.number = 3;
   rule_cfg.match_result[1].result_type = WP_PCE_RESULT_INGRESS_POLICER_INPUT_COLOR;
   rule_cfg.match_result[1].param.policer_color.color = WP_IW_TCM_INPUT_COLOR_YELLOW;
   rule_cfg.match_result[2].result_type = WP_PCE_RESULT_IN_IW_PORT_UPDATE;
   rule_cfg.match_result[2].param.iw_port.iw_port_handle = bport_iw3;
   rule_cfg.match_result[3].result_type = WP_PCE_RESULT_LAST;
   WPE_TEST_CHECK_RESULT(rule_handle,
                     WPI_BASIC_ERROR(WP_ERR_PCE_RULE_RESULT_ILLEGAL_COMBINATION),
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));

   rule_cfg.match_result[2].result_type = WP_PCE_RESULT_OUT_IW_PORT_UPDATE;
   temp_rule_index++; WPE_TEST_CHECK_HANDLE(temp_rule_handles[temp_rule_index],
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));
#ifdef DEBUG_PRINT
   WP_Display(WP_WINPATH(DEFAULT_WPID), temp_rule_handles[temp_rule_index], 0, 0);
#endif

   MY_PRINTF("\n----------------   Results: NUM MPLS POP 3 & ING POLICER COLOR GREEN & IW OUT PORT & FS_CHANGE ---\n\n");
   rule_cfg.rule_fields[0].value.mac_addr[5] = temp_rule_index;
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_NUM_OF_MPLS_HEADERS_TO_POP;
   rule_cfg.match_result[0].param.num_mpls_headers_to_pop.number = 3;
   rule_cfg.match_result[1].result_type = WP_PCE_RESULT_INGRESS_POLICER_INPUT_COLOR;
   rule_cfg.match_result[1].param.policer_color.color = WP_IW_TCM_INPUT_COLOR_GREEN;
   rule_cfg.match_result[2].result_type = WP_PCE_RESULT_OUT_IW_PORT_UPDATE;
   rule_cfg.match_result[2].param.iw_port.iw_port_handle = bport_iw3;
   rule_cfg.match_result[3].result_type = WP_PCE_RESULT_FILTER_SET_CHANGE;
   rule_cfg.match_result[3].param.change_fs.filter_set_handle = filter_set_lvl1_handle;
   rule_cfg.match_result[4].result_type = WP_PCE_RESULT_LAST;
   temp_rule_index++; WPE_TEST_CHECK_HANDLE(temp_rule_handles[temp_rule_index],
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));

   MY_PRINTF("\n----------------   Results: NUM MPLS POP 4 & ING POLICER COLOR RED & EXT VID 0x0AFE ----------------\n\n");
   rule_cfg.rule_fields[0].value.mac_addr[5] = temp_rule_index;
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_NUM_OF_MPLS_HEADERS_TO_POP;
   rule_cfg.match_result[0].param.num_mpls_headers_to_pop.number = 4;
   rule_cfg.match_result[1].result_type = WP_PCE_RESULT_INGRESS_POLICER_INPUT_COLOR;
   rule_cfg.match_result[1].param.policer_color.color = WP_IW_TCM_INPUT_COLOR_RED;
   rule_cfg.match_result[2].result_type = WP_PCE_RESULT_EXT_VLAN_UPDATE;
   rule_cfg.match_result[2].param.ext_vlan.mask = WP_PCE_MASK_VLAN_ID;
   rule_cfg.match_result[2].param.ext_vlan.vlan_tag = 0xCAFE;
   rule_cfg.match_result[3].result_type = WP_PCE_RESULT_LAST;
   temp_rule_index++; WPE_TEST_CHECK_HANDLE(temp_rule_handles[temp_rule_index],
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));
#ifdef DEBUG_PRINT
   WP_Display(WP_WINPATH(DEFAULT_WPID), temp_rule_handles[temp_rule_index], 0, 0);
#endif

   MY_PRINTF("\n----------------   Results: NUM MPLS POP 4 & ING POLICER COLOR RED & EXT VLAN 0xCAFE ----------------\n\n");
   rule_cfg.rule_fields[0].value.mac_addr[4] = temp_rule_index;
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_NUM_OF_MPLS_HEADERS_TO_POP;
   rule_cfg.match_result[0].param.num_mpls_headers_to_pop.number = 4;
   rule_cfg.match_result[1].result_type = WP_PCE_RESULT_INGRESS_POLICER_INPUT_COLOR;
   rule_cfg.match_result[1].param.policer_color.color = WP_IW_TCM_INPUT_COLOR_RED;
   rule_cfg.match_result[2].result_type = WP_PCE_RESULT_EXT_VLAN_UPDATE;
   rule_cfg.match_result[2].param.ext_vlan.mask = WP_PCE_MASK_VLAN_FULL;
   rule_cfg.match_result[2].param.ext_vlan.vlan_tag = 0xCAFE;
   rule_cfg.match_result[3].result_type = WP_PCE_RESULT_LAST;
   temp_rule_index++; WPE_TEST_CHECK_HANDLE(temp_rule_handles[temp_rule_index],
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));
#ifdef DEBUG_PRINT
   WP_Display(WP_WINPATH(DEFAULT_WPID), temp_rule_handles[temp_rule_index], 0, 0);
#endif

   MY_PRINTF("\n----------------   Results: NUM MPLS POP 4 & ING POLICER COLOR RED & INT VLAN 0xFACE EXT VLAN 0xCAFE ----------------\n\n");
   rule_cfg.rule_fields[0].value.mac_addr[4] = temp_rule_index;
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_NUM_OF_MPLS_HEADERS_TO_POP;
   rule_cfg.match_result[0].param.num_mpls_headers_to_pop.number = 4;
   rule_cfg.match_result[1].result_type = WP_PCE_RESULT_INGRESS_POLICER_INPUT_COLOR;
   rule_cfg.match_result[1].param.policer_color.color = WP_IW_TCM_INPUT_COLOR_RED;
   rule_cfg.match_result[2].result_type = WP_PCE_RESULT_INT_VLAN_UPDATE;
   rule_cfg.match_result[2].param.ext_vlan.mask = WP_PCE_MASK_VLAN_FULL;
   rule_cfg.match_result[2].param.ext_vlan.vlan_tag = 0xFACE;
   rule_cfg.match_result[3].result_type = WP_PCE_RESULT_EXT_VLAN_UPDATE;
   rule_cfg.match_result[3].param.ext_vlan.mask = WP_PCE_MASK_VLAN_FULL;
   rule_cfg.match_result[3].param.ext_vlan.vlan_tag = 0xCAFE;
   rule_cfg.match_result[4].result_type = WP_PCE_RESULT_LAST;
   WPE_TEST_CHECK_RESULT(rule_handle,
                     WPI_BASIC_ERROR(WP_ERR_PCE_RULE_RESULT_ILLEGAL_COMBINATION),
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));

   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
   rule_cfg.match_result[0].param.flow_agg.flow_aggregation = flow_aggregation3;
   rule_cfg.match_result[1].result_type = WP_PCE_RESULT_PREFIX_PRIORITY_REMARKING;
   rule_cfg.match_result[1].param.prefix_priority_remarking.remarking_type[0] = WP_PCE_RESULT_PPR_TYPE_PREFIX_VLAN_PRIO;
   rule_cfg.match_result[1].param.prefix_priority_remarking.value[0] = 0x4;
   rule_cfg.match_result[1].param.prefix_priority_remarking.remarking_type[1] = WP_PCE_RESULT_PPR_TYPE_PREFIX_MPLS_EXP;
   rule_cfg.match_result[1].param.prefix_priority_remarking.value[1] = 0x5;
   rule_cfg.match_result[1].param.prefix_priority_remarking.remarking_type[2] = WP_PCE_RESULT_PPR_TYPE_DO_NOT_CHANGE;
   rule_cfg.match_result[1].param.prefix_priority_remarking.remarking_type[3] = WP_PCE_RESULT_PPR_TYPE_DO_NOT_CHANGE;
   temp_rule_index++; WPE_TEST_CHECK_HANDLE(temp_rule_handles[temp_rule_index],
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));
#ifdef DEBUG_PRINT
   WP_Display(WP_WINPATH(DEFAULT_WPID), temp_rule_handles[temp_rule_index], 0, 0);
#endif

   MY_PRINTF("\n----------------   Results: NUM MPLS POP 4 & ING POLICER COLOR RED & INT VLAN 0xFACE EXT VLAN 0xCAFE ----------------\n\n");
   rule_cfg.rule_fields[0].value.mac_addr[4] = temp_rule_index;
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_INT_VLAN_UPDATE;
   rule_cfg.match_result[0].param.ext_vlan.mask = WP_PCE_MASK_VLAN_FULL;
   rule_cfg.match_result[0].param.ext_vlan.vlan_tag = 0xFACE;
   rule_cfg.match_result[1].result_type = WP_PCE_RESULT_EXT_VLAN_UPDATE;
   rule_cfg.match_result[1].param.ext_vlan.mask = WP_PCE_MASK_VLAN_FULL;
   rule_cfg.match_result[1].param.ext_vlan.vlan_tag = 0xCAFE;
   rule_cfg.match_result[2].result_type = WP_PCE_RESULT_LAST;
   temp_rule_index++; WPE_TEST_CHECK_HANDLE(temp_rule_handles[temp_rule_index],
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));
#ifdef DEBUG_PRINT
   WP_Display(WP_WINPATH(DEFAULT_WPID), temp_rule_handles[temp_rule_index], 0, 0);
#endif

   MY_PRINTF("\n----------------   Results: NUM MPLS POP 4 & ING POLICER COLOR RED & INT VLAN 0xFACE EXT VLAN 0xCAFE ----------------\n\n");
   rule_cfg.rule_fields[0].value.mac_addr[4] = temp_rule_index;
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_INT_VLAN_UPDATE;
   rule_cfg.match_result[0].param.ext_vlan.mask = WP_PCE_MASK_VLAN_ID;
   rule_cfg.match_result[0].param.ext_vlan.vlan_tag = 0xFACE;
   rule_cfg.match_result[1].result_type = WP_PCE_RESULT_EXT_VLAN_UPDATE;
   rule_cfg.match_result[1].param.ext_vlan.mask = WP_PCE_MASK_VLAN_FULL;
   rule_cfg.match_result[1].param.ext_vlan.vlan_tag = 0xCAFE;
   rule_cfg.match_result[2].result_type = WP_PCE_RESULT_LAST;
   temp_rule_index++; WPE_TEST_CHECK_HANDLE(temp_rule_handles[temp_rule_index],
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));
#ifdef DEBUG_PRINT
   WP_Display(WP_WINPATH(DEFAULT_WPID), temp_rule_handles[temp_rule_index], 0, 0);
#endif

   MY_PRINTF("\n----------------   Results: NUM MPLS POP 4 & ING POLICER COLOR RED & INT VLAN 0xFACE EXT VLAN 0xCAFE ----------------\n\n");
   rule_cfg.rule_fields[0].value.mac_addr[4] = temp_rule_index;
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_INT_VLAN_UPDATE;
   rule_cfg.match_result[0].param.ext_vlan.mask = WP_PCE_MASK_VLAN_FULL;
   rule_cfg.match_result[0].param.ext_vlan.vlan_tag = 0xFACE;
   rule_cfg.match_result[1].result_type = WP_PCE_RESULT_EXT_VLAN_UPDATE;
   rule_cfg.match_result[1].param.ext_vlan.mask = WP_PCE_MASK_VLAN_ID;
   rule_cfg.match_result[1].param.ext_vlan.vlan_tag = 0xCAFE;
   rule_cfg.match_result[2].result_type = WP_PCE_RESULT_LAST;
   temp_rule_index++; WPE_TEST_CHECK_HANDLE(temp_rule_handles[temp_rule_index],
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));
#ifdef DEBUG_PRINT
   WP_Display(WP_WINPATH(DEFAULT_WPID), temp_rule_handles[temp_rule_index], 0, 0);
#endif

   MY_PRINTF("\n----------------   Results: NUM MPLS POP 4 & ING POLICER COLOR RED & INT VLAN 0xFACE EXT VLAN 0xCAFE ----------------\n\n");
   rule_cfg.rule_fields[0].value.mac_addr[4] = temp_rule_index;
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_INT_VLAN_UPDATE;
   rule_cfg.match_result[0].param.ext_vlan.mask = WP_PCE_MASK_VLAN_ID;
   rule_cfg.match_result[0].param.ext_vlan.vlan_tag = 0xFACE;
   rule_cfg.match_result[1].result_type = WP_PCE_RESULT_EXT_VLAN_UPDATE;
   rule_cfg.match_result[1].param.ext_vlan.mask = WP_PCE_MASK_VLAN_FULL;
   rule_cfg.match_result[1].param.ext_vlan.vlan_tag = 0xCAFE;
   rule_cfg.match_result[2].result_type = WP_PCE_RESULT_FLOW_AGG;
   rule_cfg.match_result[2].param.flow_agg.flow_aggregation = flow_aggregation3;
   rule_cfg.match_result[3].result_type = WP_PCE_RESULT_LAST;
   temp_rule_index++; WPE_TEST_CHECK_HANDLE(temp_rule_handles[temp_rule_index],
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));
#ifdef DEBUG_PRINT
   WP_Display(WP_WINPATH(DEFAULT_WPID), temp_rule_handles[temp_rule_index], 0, 0);
#endif

   MY_PRINTF("\n----------------   Results: NUM MPLS POP 4 & ING POLICER COLOR RED & INT VLAN 0xFACE EXT VLAN 0xCAFE ----------------\n\n");
   rule_cfg.rule_fields[0].value.mac_addr[4] = temp_rule_index;
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_INT_VLAN_UPDATE;
   rule_cfg.match_result[0].param.ext_vlan.mask = WP_PCE_MASK_VLAN_FULL;
   rule_cfg.match_result[0].param.ext_vlan.vlan_tag = 0xFACE;
   rule_cfg.match_result[1].result_type = WP_PCE_RESULT_EXT_VLAN_UPDATE;
   rule_cfg.match_result[1].param.ext_vlan.mask = WP_PCE_MASK_VLAN_ID;
   rule_cfg.match_result[1].param.ext_vlan.vlan_tag = 0xCAFE;
   rule_cfg.match_result[2].result_type = WP_PCE_RESULT_FLOW_AGG;
   rule_cfg.match_result[2].param.flow_agg.flow_aggregation = flow_aggregation3;
   rule_cfg.match_result[3].result_type = WP_PCE_RESULT_LAST;
   WPE_TEST_CHECK_RESULT(rule_handle,
                     WPI_BASIC_ERROR(WP_ERR_PCE_RULE_RESULT_ILLEGAL_COMBINATION),
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));

   MY_PRINTF("\n----------------   Results: NUM MPLS POP 4 & ING POLICER COLOR RED & INT VLAN 0xFACE EXT VLAN 0xCAFE ----------------\n\n");
   rule_cfg.rule_fields[0].value.mac_addr[4] = temp_rule_index;
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_INT_VLAN_UPDATE;
   rule_cfg.match_result[0].param.ext_vlan.mask = WP_PCE_MASK_VLAN_FULL;
   rule_cfg.match_result[0].param.ext_vlan.vlan_tag = 0xFACE;
   rule_cfg.match_result[1].result_type = WP_PCE_RESULT_EXT_VLAN_UPDATE;
   rule_cfg.match_result[1].param.ext_vlan.mask = WP_PCE_MASK_VLAN_ID;
   rule_cfg.match_result[1].param.ext_vlan.vlan_tag = 0xCAFE;
   rule_cfg.match_result[2].result_type = WP_PCE_RESULT_LAST;
   temp_rule_index++; WPE_TEST_CHECK_HANDLE(temp_rule_handles[temp_rule_index],
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));
#ifdef DEBUG_PRINT
   WP_Display(WP_WINPATH(DEFAULT_WPID), temp_rule_handles[temp_rule_index], 0, 0);
#endif

   MY_PRINTF("\n----------------   Results: MAC SA REMARK: 0A CE CA FE FA CE ---------------\n\n");
   rule_cfg.rule_fields[0].value.mac_addr[5] = temp_rule_index;
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_REMARK_MAC_SA;
   rule_cfg.match_result[0].param.mac_remarking.mac_addr[0] = 0x0A;
   rule_cfg.match_result[0].param.mac_remarking.mac_addr[1] = 0xCE;
   rule_cfg.match_result[0].param.mac_remarking.mac_addr[2] = 0xCA;
   rule_cfg.match_result[0].param.mac_remarking.mac_addr[3] = 0xFE;
   rule_cfg.match_result[0].param.mac_remarking.mac_addr[4] = 0xFA;
   rule_cfg.match_result[0].param.mac_remarking.mac_addr[5] = 0xCE;
   rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;
   temp_rule_index++; WPE_TEST_CHECK_HANDLE(temp_rule_handles[temp_rule_index],
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));

   MY_PRINTF("\n----------------   Results: MAC DA REMARK: 01 02 03 04 05 06 ---------------\n\n");
   rule_cfg.rule_fields[0].value.mac_addr[5] = temp_rule_index;
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_REMARK_MAC_DA;
   for(i=0; i<6; i++)
      rule_cfg.match_result[0].param.mac_remarking.mac_addr[i] = i+1;
   rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;
   temp_rule_index++; WPE_TEST_CHECK_HANDLE(temp_rule_handles[temp_rule_index],
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));

   MY_PRINTF("\n----------------   Results: MAC DA REMARK & FS_CHANGE error ---------------\n\n");
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_REMARK_MAC_DA;
   rule_cfg.match_result[1].result_type = WP_PCE_RESULT_FILTER_SET_CHANGE;
   rule_cfg.match_result[2].result_type = WP_PCE_RESULT_LAST;
   WPE_TEST_CHECK_RESULT(rule_handle,
                     WPI_BASIC_ERROR(WP_ERR_PCE_RULE_RESULT_SIZE_EXCEEDED),
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));

   MY_PRINTF("\n----------------   Results: USER SPECIFIC REMARK: 0x11 0x12 0x13 0x14 0x15 0x16 0x17 ---------------\n\n");
   rule_cfg.rule_fields[0].value.mac_addr[5] = temp_rule_index;
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_REMARK_USER_SPECIFIC;
   for(i=0; i<WP_PCE_USER_SPECIFIC_FIELD_SIZE; i++)
      rule_cfg.match_result[0].param.user_specific.bytes[i] = i+0x11;
   rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;
   temp_rule_index++; WPE_TEST_CHECK_HANDLE(temp_rule_handles[temp_rule_index],
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));

   MY_PRINTF("\n----------------   Results: PRIORITY MAPPING: 0xDB ---------------\n\n");
   rule_cfg.rule_fields[0].value.mac_addr[5] = temp_rule_index;
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_PRIORITY_MAPPING;
   rule_cfg.match_result[0].param.priority_mapping.value = 0xDB;
   rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;
   temp_rule_index++; WPE_TEST_CHECK_HANDLE(temp_rule_handles[temp_rule_index],
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));

   MY_PRINTF("\n----------------   Results: PRIORITY MAPPING: 0xDB & FAGG & LRN_FAGG & FS_CHANGE -----------\n\n");
   rule_cfg.rule_fields[0].value.mac_addr[5] = temp_rule_index;
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_PRIORITY_MAPPING;
   rule_cfg.match_result[0].param.priority_mapping.value = 0xDB;
   rule_cfg.match_result[1].result_type = WP_PCE_RESULT_FLOW_AGG;
   rule_cfg.match_result[1].param.flow_agg.flow_aggregation = flow_aggregation3;
   rule_cfg.match_result[2].result_type = WP_PCE_RESULT_LEARNING_FLOW_AGG;
   rule_cfg.match_result[2].param.learning_flow_agg.flow_aggregation = flow_aggregation3;
   rule_cfg.match_result[3].result_type = WP_PCE_RESULT_FILTER_SET_CHANGE;
   rule_cfg.match_result[3].param.change_fs.filter_set_handle = filter_set_lvl1_handle;
   rule_cfg.match_result[4].result_type = WP_PCE_RESULT_LAST;
   temp_rule_index++; WPE_TEST_CHECK_HANDLE(temp_rule_handles[temp_rule_index],
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));

   MY_PRINTF("\n----------------   Results: policer error handle -----------\n\n");
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_PRIORITY_MAPPING;
   rule_cfg.match_result[0].param.priority_mapping.value = 0xDB;
   rule_cfg.match_result[1].result_type = WP_PCE_RESULT_FLOW_AGG;
   rule_cfg.match_result[1].param.flow_agg.flow_aggregation = flow_aggregation3;
   rule_cfg.match_result[2].result_type = WP_PCE_RESULT_INGRESS_POLICER;
   rule_cfg.match_result[2].param.ingress_policer.policer = flow_aggregation3;
   rule_cfg.match_result[3].result_type = WP_PCE_RESULT_LAST;
   WPE_TEST_CHECK_RESULT(rule_handle,
                     WPI_BASIC_ERROR(WP_ERR_HANDLE),
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));

   MY_PRINTF("\n----------------   Results: policer, policer action, FAGG, PRIO MAP -----------\n\n");
   WPE_TEST_CHECK_HANDLE(policer_action_handle,
      WP_PolicerActionCreate(WP_WINPATH(DEFAULT_WPID), &policer_action));
   WPE_TEST_CHECK_HANDLE(policer_action_handle,
      WP_PolicerActionCreate(WP_WINPATH(DEFAULT_WPID), &policer_action));
   MY_PRINTF("---- policer_action_handle 0x%X ----\n", policer_action_handle);

   pv2.policer_action = policer_action_handle;
   pv2.frame_len_correction = WP_POLICER_LENGTH_CORRECT_4B;
   WPE_TEST_CHECK_HANDLE(policer_handle,
      WP_PolicerCreate(WP_WINPATH(DEFAULT_WPID), WP_POLICER_V2, &pv2));
   WPE_TEST_CHECK_HANDLE(policer_handle,
      WP_PolicerCreate(WP_WINPATH(DEFAULT_WPID), WP_POLICER_V2, &pv2));
   WPE_TEST_CHECK_HANDLE(policer_handle,
      WP_PolicerCreate(WP_WINPATH(DEFAULT_WPID), WP_POLICER_V2, &pv2));
   MY_PRINTF("---- policer_handle 0x%X ----\n", policer_handle);
   pv2.policer_action = 0;
   WPE_TEST_CHECK_HANDLE(policer_without_action,
      WP_PolicerCreate(WP_WINPATH(DEFAULT_WPID), WP_POLICER_V2, &pv2));
   MY_PRINTF("---- policer_without_action 0x%X ----\n", policer_without_action);

   rule_cfg.rule_fields[0].value.mac_addr[5] = temp_rule_index;
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
   rule_cfg.match_result[0].param.flow_agg.flow_aggregation = flow_aggregation3;
   rule_cfg.match_result[1].result_type = WP_PCE_RESULT_INGRESS_POLICER;
   rule_cfg.match_result[1].param.ingress_policer.policer = policer_handle;
   rule_cfg.match_result[2].result_type = WP_PCE_RESULT_PRIORITY_MAPPING;
   rule_cfg.match_result[2].param.priority_mapping.value = 0xDB;
   rule_cfg.match_result[3].result_type = WP_PCE_RESULT_LAST;
   temp_rule_index++; WPE_TEST_CHECK_HANDLE(temp_rule_handles[temp_rule_index],
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));

   MY_PRINTF("\n----------------   Results: cong prof index: 0xFFFF error -----------\n\n");
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_CONGESTION_PROFILE;
   rule_cfg.match_result[0].param.congestion_profile.index = 0xFFFF;
   rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;
   WPE_TEST_CHECK_RESULT(rule_handle,
                     WPI_BASIC_ERROR(WP_ERR_PCE_RULE_RESULT_ILLEGAL_CONG_PROF_INDEX),
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));

   MY_PRINTF("\n----------------   Results: cong prof index: 0x03FF & REDIRECT_TO_COMPATIBLE -----\n\n");
   rule_cfg.rule_fields[0].value.mac_addr[5] = temp_rule_index;
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_CONGESTION_PROFILE;
   rule_cfg.match_result[0].param.congestion_profile.index = 0x3FF;
   rule_cfg.match_result[1].result_type = WP_PCE_RESULT_IWCP_START_REDIRECT;
   rule_cfg.match_result[2].result_type = WP_PCE_RESULT_LAST;
   temp_rule_index++; WPE_TEST_CHECK_HANDLE(temp_rule_handles[temp_rule_index],
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));

   MY_PRINTF("\n----------------   Results: policer w/o action, PPR (1) -----------\n\n");
   rule_cfg.rule_fields[0].value.mac_addr[5] = temp_rule_index;
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_INGRESS_POLICER;
   rule_cfg.match_result[0].param.ingress_policer.policer = policer_without_action;
   rule_cfg.match_result[1].result_type = WP_PCE_RESULT_PREFIX_PRIORITY_REMARKING;
   rule_cfg.match_result[1].param.prefix_priority_remarking.remarking_type[0] = WP_PCE_RESULT_PPR_TYPE_PREFIX_VLAN_PRIO;
   rule_cfg.match_result[1].param.prefix_priority_remarking.value[0] = 0x4;
   rule_cfg.match_result[1].param.prefix_priority_remarking.remarking_type[1] = WP_PCE_RESULT_PPR_TYPE_DO_NOT_CHANGE;
   rule_cfg.match_result[1].param.prefix_priority_remarking.remarking_type[2] = WP_PCE_RESULT_PPR_TYPE_DO_NOT_CHANGE;
   rule_cfg.match_result[1].param.prefix_priority_remarking.remarking_type[3] = WP_PCE_RESULT_PPR_TYPE_DO_NOT_CHANGE;
   rule_cfg.match_result[2].result_type = WP_PCE_RESULT_LAST;
   temp_rule_index++; WPE_TEST_CHECK_HANDLE(temp_rule_handles[temp_rule_index],
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));

   MY_PRINTF("\n----------------   Results: policer w/o action, policer color (1) -----------\n\n");
   rule_cfg.rule_fields[0].value.mac_addr[5] = temp_rule_index;
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_INGRESS_POLICER_INPUT_COLOR;
   rule_cfg.match_result[0].param.policer_color.color = WP_IW_TCM_INPUT_COLOR_RED;
   rule_cfg.match_result[1].result_type = WP_PCE_RESULT_INGRESS_POLICER;
   rule_cfg.match_result[1].param.ingress_policer.policer = policer_without_action;
   rule_cfg.match_result[2].result_type = WP_PCE_RESULT_LAST;
   temp_rule_index++; WPE_TEST_CHECK_HANDLE(temp_rule_handles[temp_rule_index],
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));

   MY_PRINTF("\n----------------   Results: policer, policer action, policer color -----------\n\n");
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_INGRESS_POLICER_INPUT_COLOR;
   rule_cfg.match_result[0].param.policer_color.color = WP_IW_TCM_INPUT_COLOR_RED;
   rule_cfg.match_result[1].result_type = WP_PCE_RESULT_INGRESS_POLICER;
   rule_cfg.match_result[1].param.ingress_policer.policer = policer_handle;
   rule_cfg.match_result[2].result_type = WP_PCE_RESULT_LAST;
   WPE_TEST_CHECK_RESULT(rule_handle,
                     WPI_BASIC_ERROR(WP_ERR_PCE_RULE_RESULT_ILLEGAL_COMBINATION),
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));

   MY_PRINTF("\n----------------   Results: flow agg, prio mapping, NMHP -----------\n\n");
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_NUM_OF_MPLS_HEADERS_TO_POP;
   rule_cfg.match_result[0].param.num_mpls_headers_to_pop.number = 3;
   rule_cfg.match_result[1].result_type = WP_PCE_RESULT_FLOW_AGG;
   rule_cfg.match_result[1].param.flow_agg.flow_aggregation = flow_aggregation3;
   rule_cfg.match_result[2].result_type = WP_PCE_RESULT_PRIORITY_MAPPING;
   rule_cfg.match_result[2].param.priority_mapping.value = 0xDB;
   rule_cfg.match_result[3].result_type = WP_PCE_RESULT_LAST;
   WPE_TEST_CHECK_RESULT(rule_handle,
                     WPI_BASIC_ERROR(WP_ERR_PCE_RULE_RESULT_ILLEGAL_COMBINATION),
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));

   MY_PRINTF("\n----------------   Results: flow agg, prio mapping, PPR (1) -----------\n\n");
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
   rule_cfg.match_result[0].param.flow_agg.flow_aggregation = flow_aggregation3;
   rule_cfg.match_result[1].result_type = WP_PCE_RESULT_PREFIX_PRIORITY_REMARKING;
   rule_cfg.match_result[1].param.prefix_priority_remarking.remarking_type[0] = WP_PCE_RESULT_PPR_TYPE_PREFIX_VLAN_PRIO;
   rule_cfg.match_result[1].param.prefix_priority_remarking.value[0] = 0x4;
   rule_cfg.match_result[1].param.prefix_priority_remarking.remarking_type[1] = WP_PCE_RESULT_PPR_TYPE_DO_NOT_CHANGE;
   rule_cfg.match_result[1].param.prefix_priority_remarking.remarking_type[2] = WP_PCE_RESULT_PPR_TYPE_DO_NOT_CHANGE;
   rule_cfg.match_result[1].param.prefix_priority_remarking.remarking_type[3] = WP_PCE_RESULT_PPR_TYPE_DO_NOT_CHANGE;
   rule_cfg.match_result[2].result_type = WP_PCE_RESULT_PRIORITY_MAPPING;
   rule_cfg.match_result[2].param.priority_mapping.value = 0xDB;
   rule_cfg.match_result[3].result_type = WP_PCE_RESULT_LAST;
   WPE_TEST_CHECK_RESULT(rule_handle,
                     WPI_BASIC_ERROR(WP_ERR_PCE_RULE_RESULT_ILLEGAL_COMBINATION),
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));

   MY_PRINTF("\n----------------   Results: flow agg, learning flow agg, PPR (4) -----------\n\n");
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
   rule_cfg.match_result[0].param.flow_agg.flow_aggregation = flow_aggregation3;
   rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LEARNING_FLOW_AGG;
   rule_cfg.match_result[1].param.flow_agg.flow_aggregation = flow_aggregation3;
   rule_cfg.match_result[2].result_type = WP_PCE_RESULT_PREFIX_PRIORITY_REMARKING;
   rule_cfg.match_result[2].param.prefix_priority_remarking.remarking_type[0] = WP_PCE_RESULT_PPR_TYPE_DO_NOT_CHANGE;
   rule_cfg.match_result[2].param.prefix_priority_remarking.remarking_type[1] = WP_PCE_RESULT_PPR_TYPE_DO_NOT_CHANGE;
   rule_cfg.match_result[2].param.prefix_priority_remarking.remarking_type[2] = WP_PCE_RESULT_PPR_TYPE_DO_NOT_CHANGE;
   rule_cfg.match_result[2].param.prefix_priority_remarking.remarking_type[3] = WP_PCE_RESULT_PPR_TYPE_PREFIX_REMARKING_VAL;
   rule_cfg.match_result[2].param.prefix_priority_remarking.value[1] = 0x7;
   rule_cfg.match_result[3].result_type = WP_PCE_RESULT_LAST;
   WPE_TEST_CHECK_RESULT(rule_handle,
                     WPI_BASIC_ERROR(WP_ERR_PCE_RULE_RESULT_ILLEGAL_COMBINATION),
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));

   MY_PRINTF("\n----------------   Results: PREF PRIO: pref_vprio 0x7 (see 0xE) pref_exp 0x5 XXX XXX & NMHP ----------------\n\n");
   rule_cfg.rule_fields[0].value.mac_addr[5] = temp_rule_index;
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_PREFIX_PRIORITY_REMARKING;
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[0] = WP_PCE_RESULT_PPR_TYPE_PREFIX_VLAN_PRIO;
   rule_cfg.match_result[0].param.prefix_priority_remarking.value[0] = 0x7;
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[1] = WP_PCE_RESULT_PPR_TYPE_PREFIX_MPLS_EXP;
   rule_cfg.match_result[0].param.prefix_priority_remarking.value[1] = 0x5;
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[2] = WP_PCE_RESULT_PPR_TYPE_DO_NOT_CHANGE;
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[3] = WP_PCE_RESULT_PPR_TYPE_DO_NOT_CHANGE;
   rule_cfg.match_result[1].result_type = WP_PCE_RESULT_NUM_OF_MPLS_HEADERS_TO_POP;
   rule_cfg.match_result[1].param.num_mpls_headers_to_pop.number = 3;
   rule_cfg.match_result[2].result_type = WP_PCE_RESULT_LAST;
   temp_rule_index++; WPE_TEST_CHECK_HANDLE(temp_rule_handles[temp_rule_index],
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));

   MY_PRINTF("\n----------------   Results: PREF PRIO: XXX pref_exp 0x5 XXX XXX & NMHP ----------------\n\n");
   rule_cfg.rule_fields[0].value.mac_addr[5] = temp_rule_index;
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_PREFIX_PRIORITY_REMARKING;
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[0] = WP_PCE_RESULT_PPR_TYPE_DO_NOT_CHANGE;
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[1] = WP_PCE_RESULT_PPR_TYPE_PREFIX_MPLS_EXP;
   rule_cfg.match_result[0].param.prefix_priority_remarking.value[1] = 0x5;
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[2] = WP_PCE_RESULT_PPR_TYPE_DO_NOT_CHANGE;
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[3] = WP_PCE_RESULT_PPR_TYPE_DO_NOT_CHANGE;
   rule_cfg.match_result[1].result_type = WP_PCE_RESULT_NUM_OF_MPLS_HEADERS_TO_POP;
   rule_cfg.match_result[1].param.num_mpls_headers_to_pop.number = 3;
   rule_cfg.match_result[2].result_type = WP_PCE_RESULT_LAST;
   temp_rule_index++; WPE_TEST_CHECK_HANDLE(temp_rule_handles[temp_rule_index],
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));

   MY_PRINTF("\n----------------   Results: flow agg, PPR (1, 3 & 4) -----------\n\n");
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
   rule_cfg.match_result[0].param.flow_agg.flow_aggregation = flow_aggregation3;
   rule_cfg.match_result[1].result_type = WP_PCE_RESULT_PREFIX_PRIORITY_REMARKING;
   rule_cfg.match_result[1].param.prefix_priority_remarking.remarking_type[0] = WP_PCE_RESULT_PPR_TYPE_PREFIX_REMARKING_VAL;
   rule_cfg.match_result[1].param.prefix_priority_remarking.value[0] = 0x6;
   rule_cfg.match_result[1].param.prefix_priority_remarking.remarking_type[1] = WP_PCE_RESULT_PPR_TYPE_DO_NOT_CHANGE;
   rule_cfg.match_result[1].param.prefix_priority_remarking.remarking_type[2] = WP_PCE_RESULT_PPR_TYPE_PREFIX_REMARKING_VAL;
   rule_cfg.match_result[1].param.prefix_priority_remarking.value[2] = 0x6;
   rule_cfg.match_result[1].param.prefix_priority_remarking.remarking_type[3] = WP_PCE_RESULT_PPR_TYPE_PREFIX_REMARKING_VAL;
   rule_cfg.match_result[1].param.prefix_priority_remarking.value[3] = 0x7;
   rule_cfg.match_result[2].result_type = WP_PCE_RESULT_LAST;
   WPE_TEST_CHECK_RESULT(rule_handle,
                     WPI_BASIC_ERROR(WP_ERR_PCE_RULE_RESULT_ILLEGAL_COMBINATION),
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));

   MY_PRINTF("\n----------------   Results: priority mapping, PPR (1, 3 & 4) -----------\n\n");
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_PRIORITY_MAPPING;
   rule_cfg.match_result[0].param.priority_mapping.value = 0xDB;
   rule_cfg.match_result[1].result_type = WP_PCE_RESULT_PREFIX_PRIORITY_REMARKING;
   rule_cfg.match_result[1].param.prefix_priority_remarking.remarking_type[0] = WP_PCE_RESULT_PPR_TYPE_PREFIX_REMARKING_VAL;
   rule_cfg.match_result[1].param.prefix_priority_remarking.value[0] = 0x6;
   rule_cfg.match_result[1].param.prefix_priority_remarking.remarking_type[1] = WP_PCE_RESULT_PPR_TYPE_DO_NOT_CHANGE;
   rule_cfg.match_result[1].param.prefix_priority_remarking.remarking_type[2] = WP_PCE_RESULT_PPR_TYPE_PREFIX_REMARKING_VAL;
   rule_cfg.match_result[1].param.prefix_priority_remarking.value[2] = 0x6;
   rule_cfg.match_result[1].param.prefix_priority_remarking.remarking_type[3] = WP_PCE_RESULT_PPR_TYPE_PREFIX_REMARKING_VAL;
   rule_cfg.match_result[1].param.prefix_priority_remarking.value[3] = 0x7;
   rule_cfg.match_result[2].result_type = WP_PCE_RESULT_LAST;
   WPE_TEST_CHECK_RESULT(rule_handle,
                     WPI_BASIC_ERROR(WP_ERR_PCE_RULE_RESULT_ILLEGAL_COMBINATION),
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));

   MY_PRINTF("\n----------------   Results: priority mapping, PPR (1, 2 & 3) -----------\n\n");
   rule_cfg.rule_fields[0].value.mac_addr[5] = temp_rule_index;
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_PRIORITY_MAPPING;
   rule_cfg.match_result[0].param.priority_mapping.value = 0xDB;
   rule_cfg.match_result[1].result_type = WP_PCE_RESULT_PREFIX_PRIORITY_REMARKING;
   rule_cfg.match_result[1].param.prefix_priority_remarking.remarking_type[0] = WP_PCE_RESULT_PPR_TYPE_PREFIX_REMARKING_VAL;
   rule_cfg.match_result[1].param.prefix_priority_remarking.value[0] = 0x6;
   rule_cfg.match_result[1].param.prefix_priority_remarking.remarking_type[1] = WP_PCE_RESULT_PPR_TYPE_PREFIX_REMARKING_VAL;
   rule_cfg.match_result[1].param.prefix_priority_remarking.value[1] = 0x2;
   rule_cfg.match_result[1].param.prefix_priority_remarking.remarking_type[2] = WP_PCE_RESULT_PPR_TYPE_PREFIX_REMARKING_VAL;
   rule_cfg.match_result[1].param.prefix_priority_remarking.value[2] = 0x7;
   rule_cfg.match_result[1].param.prefix_priority_remarking.remarking_type[3] = WP_PCE_RESULT_PPR_TYPE_DO_NOT_CHANGE;
   rule_cfg.match_result[2].result_type = WP_PCE_RESULT_LAST;
   temp_rule_index++; WPE_TEST_CHECK_HANDLE(temp_rule_handles[temp_rule_index],
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));

   MY_PRINTF("\n----------------   Results: flow agg, PPR (1, 2 & 3) -----------\n\n");
   rule_cfg.rule_fields[0].value.mac_addr[5] = temp_rule_index;
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
   rule_cfg.match_result[0].param.flow_agg.flow_aggregation = flow_aggregation3;
   rule_cfg.match_result[1].result_type = WP_PCE_RESULT_PREFIX_PRIORITY_REMARKING;
   rule_cfg.match_result[1].param.prefix_priority_remarking.remarking_type[0] = WP_PCE_RESULT_PPR_TYPE_PREFIX_REMARKING_VAL;
   rule_cfg.match_result[1].param.prefix_priority_remarking.value[0] = 0x6;
   rule_cfg.match_result[1].param.prefix_priority_remarking.remarking_type[1] = WP_PCE_RESULT_PPR_TYPE_PREFIX_REMARKING_VAL;
   rule_cfg.match_result[1].param.prefix_priority_remarking.value[1] = 0x2;
   rule_cfg.match_result[1].param.prefix_priority_remarking.remarking_type[2] = WP_PCE_RESULT_PPR_TYPE_PREFIX_REMARKING_VAL;
   rule_cfg.match_result[1].param.prefix_priority_remarking.value[2] = 0x7;
   rule_cfg.match_result[1].param.prefix_priority_remarking.remarking_type[3] = WP_PCE_RESULT_PPR_TYPE_DO_NOT_CHANGE;
   rule_cfg.match_result[2].result_type = WP_PCE_RESULT_LAST;
   temp_rule_index++; WPE_TEST_CHECK_HANDLE(temp_rule_handles[temp_rule_index],
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));

   MY_PRINTF("\n----------------   Results: Remove the created rules --------------\n\n");
   for(i=0;i<=temp_rule_index; i++)
   {
      WPE_TEST_CHECK_RESULT(status,
                        WP_OK,
                        WP_PceRuleDelete(temp_rule_handles[i]));
   }

   MY_PRINTF("\n----------------   PCE Rule Results Test Finish  -------------------\n\n");
}

static void WPE_PceNewFieldsTest(void)
{
   WP_U8 i;
   WP_handle filter2, ext_filter2, ext_filter3, filter3, filter_1bit_mask;
   WP_pce_filter_classification filter_clas = {0};
   WP_pce_rule_classification rule_cfg = {0};
   WP_handle rule2, ext_rule2, ext_rule3, rule3, rule_1bit_mask;

   filter_clas.no_match_action = WP_PCE_FILTER_NO_MATCH_CONTINUE;
   filter_clas.no_fields_action = WP_PCE_FILTER_NO_FIELDS_CONTINUE;
   filter_clas.no_match_result[0].result_type = WP_PCE_RESULT_LAST;

   MY_PRINTF("\n---- Single Filter (filter2) Creating: \n  "
      "ETHER ENCAP TYPE & VLAN TYPE & PPPoE SID & PPPoE PID & IPV4_MORE_FRAGMENTS ----------\n");
   filter_clas.filter_fields[0].field_id = WP_PCE_FIELD_ID_ETHER_ENCAP_TYPE;
   filter_clas.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_clas.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   filter_clas.filter_fields[1].field_id = WP_PCE_FIELD_ID_VLAN_TYPE;
   filter_clas.filter_fields[1].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_clas.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   filter_clas.filter_fields[2].field_id = WP_PCE_FIELD_ID_PPPOE_SESSION_ID;
   filter_clas.filter_fields[2].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_clas.filter_fields[2].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   filter_clas.filter_fields[3].field_id = WP_PCE_FIELD_ID_PPPOE_PROTOCOL_ID;
   filter_clas.filter_fields[3].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_clas.filter_fields[3].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   filter_clas.filter_fields[4].field_id = WP_PCE_FIELD_ID_LAST;
   WPE_TEST_CHECK_HANDLE(filter2,
                     WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_CLASSIFICATION, &filter_clas));
   MY_PRINTF("\n---- Single Filter (filter2) Created: 0x%X ----------\n", (WP_U32) filter2);

#ifdef DEBUG_PRINT
   WP_Display(WP_WINPATH(DEFAULT_WPID), filter2, 0, 0);
#endif

   MY_PRINTF("\n---- Extended Filter (ext_filter2) Creating: \n  "
      "IPV6 FLOW & MPLSH1 & MPLSH5 & Num MPLSH & \n"
      "GRE Prot & GRE key & PARSER_FLAGS & MPLSH2 & MPLSH3 ------\n");

   filter_clas.filter_fields[0].field_id = WP_PCE_FIELD_ID_IPV6_FLOW_LABEL;
   filter_clas.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_clas.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   filter_clas.filter_fields[1].field_id = WP_PCE_FIELD_ID_MPLS_HEADER1;
   filter_clas.filter_fields[1].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_clas.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   filter_clas.filter_fields[2].field_id = WP_PCE_FIELD_ID_MPLS_HEADER5;
   filter_clas.filter_fields[2].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_clas.filter_fields[2].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   filter_clas.filter_fields[3].field_id = WP_PCE_FIELD_ID_NUM_OF_MPLS_HEADERS;
   filter_clas.filter_fields[3].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_clas.filter_fields[3].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   filter_clas.filter_fields[4].field_id = WP_PCE_FIELD_ID_GRE_PROTOCOL;
   filter_clas.filter_fields[4].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_clas.filter_fields[4].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   filter_clas.filter_fields[5].field_id = WP_PCE_FIELD_ID_GRE_KEY;
   filter_clas.filter_fields[5].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_clas.filter_fields[5].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   filter_clas.filter_fields[6].field_id = WP_PCE_FIELD_ID_PARSER_FLAGS;
   filter_clas.filter_fields[6].field_mode = WP_PCE_FIELD_MODE_COMPARE_LOWER_THAN;
   filter_clas.filter_fields[6].mask_mode = WP_PCE_FIELD_MASK_USED;
   filter_clas.filter_fields[6].mask.parser_flags = 0xFFFFFF00;

   filter_clas.filter_fields[7].field_id = WP_PCE_FIELD_ID_MPLS_HEADER2;
   filter_clas.filter_fields[7].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_clas.filter_fields[7].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   filter_clas.filter_fields[8].field_id = WP_PCE_FIELD_ID_MPLS_HEADER3;
   filter_clas.filter_fields[8].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_clas.filter_fields[8].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   filter_clas.filter_fields[9].field_id = WP_PCE_FIELD_ID_LAST;
   WPE_TEST_CHECK_HANDLE(ext_filter2,
                     WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_CLASSIFICATION, &filter_clas));
   MY_PRINTF("\n---- Extended Filter (ext_filter2) Created: 0x%X ----------\n", (WP_U32) ext_filter2);

   filter_clas.filter_fields[9].field_id = WP_PCE_FIELD_ID_MPLS_HEADER4;
   WPE_TEST_CHECK_RESULT(filter3,
                     WPI_BASIC_ERROR(WP_ERR_PCE_FILTER_NO_LAST_FIELD),
                     WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_CLASSIFICATION, &filter_clas));

#ifdef DEBUG_PRINT
   WP_Display(WP_WINPATH(DEFAULT_WPID), ext_filter2, 0, 0);
#endif

   MY_PRINTF("\n---- MPLSHDR 4 error mask check ----------\n");
   filter_clas.filter_fields[0].field_id = WP_PCE_FIELD_ID_MPLS_HEADER4;
   filter_clas.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_clas.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_USED;
   filter_clas.filter_fields[0].mask.mpls_header = 0x1000000;

   filter_clas.filter_fields[1].field_id = WP_PCE_FIELD_ID_LAST;

   WPE_TEST_CHECK_RESULT(filter3,
                     WPI_BASIC_ERROR(WP_ERR_PCE_FILTER_ILLEGAL_MASK),
                     WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_CLASSIFICATION, &filter_clas));

   filter_clas.filter_fields[0].mask.mpls_header = 0xFFF00F;
   WPE_TEST_CHECK_RESULT(filter3,
                     WPI_BASIC_ERROR(WP_ERR_PCE_FILTER_NON_CONTINOUS_MASK),
                     WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_CLASSIFICATION, &filter_clas));

   MY_PRINTF("\n---- IPv6 lflow number error mask check ----------\n");
   filter_clas.filter_fields[0].field_id = WP_PCE_FIELD_ID_IPV6_FLOW_LABEL;
   filter_clas.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_clas.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_USED;
   filter_clas.filter_fields[0].mask.mpls_header = 0x1000000;

   filter_clas.filter_fields[1].field_id = WP_PCE_FIELD_ID_LAST;

   WPE_TEST_CHECK_RESULT(filter3,
                     WPI_BASIC_ERROR(WP_ERR_PCE_FILTER_ILLEGAL_MASK),
                     WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_CLASSIFICATION, &filter_clas));

   MY_PRINTF("\n---- VLAN Type error mask check ----------\n");
   filter_clas.filter_fields[0].field_id = WP_PCE_FIELD_ID_VLAN_TYPE;
   filter_clas.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_clas.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_USED;
   filter_clas.filter_fields[0].mask.mpls_header = 0x10;

   filter_clas.filter_fields[1].field_id = WP_PCE_FIELD_ID_LAST;

   WPE_TEST_CHECK_RESULT(filter3,
                     WPI_BASIC_ERROR(WP_ERR_PCE_FILTER_ILLEGAL_MASK),
                     WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_CLASSIFICATION, &filter_clas));

   MY_PRINTF("\n---- IPv4 Src error mask check ----------\n");
   filter_clas.filter_fields[0].field_id = WP_PCE_FIELD_ID_IPV4_SA;
   filter_clas.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_clas.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_USED;
   filter_clas.filter_fields[0].mask.ipv4_addr = 0x80000000;

   filter_clas.filter_fields[1].field_id = WP_PCE_FIELD_ID_LAST;

   WPE_TEST_CHECK_HANDLE(filter_1bit_mask,
                         WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_CLASSIFICATION, &filter_clas));

#ifdef DEBUG_PRINT
   WP_Display(WP_WINPATH(DEFAULT_WPID), filter_1bit_mask, 0, 0);
#endif

   MY_PRINTF("\n---- Parser Flags error mask check ----------\n");
   filter_clas.filter_fields[0].field_id = WP_PCE_FIELD_ID_PARSER_FLAGS;
   filter_clas.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_clas.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_USED;
   filter_clas.filter_fields[0].mask.parser_flags = 0xABCD00;

   filter_clas.filter_fields[1].field_id = WP_PCE_FIELD_ID_LAST;

   WPE_TEST_CHECK_RESULT(filter3,
                     WPI_BASIC_ERROR(WP_ERR_PCE_FILTER_NON_CONTINOUS_MASK),
                     WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_CLASSIFICATION, &filter_clas));

   filter_clas.filter_fields[0].mask.parser_flags = 0xABCD;
   WPE_TEST_CHECK_RESULT(filter3,
                     WPI_BASIC_ERROR(WP_ERR_PCE_FILTER_MASK_NOT_SUPPORTED),
                     WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_CLASSIFICATION, &filter_clas));

   filter_clas.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;
   WPE_TEST_CHECK_RESULT(filter3,
                     WPI_BASIC_ERROR(WP_ERR_PCE_FILTER_ILLEGAL_MASK),
                     WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_CLASSIFICATION, &filter_clas));

   MY_PRINTF("\n---- Extended Filter (ext_filter3) Creating: \n  "
      "IPv6 SA HIGH & IPv6 SA LOW (mask 01 FF FF FF FF FF FF C0 \n"
      "IPv6 DA HIGH & IPv6 DA LOW (mask 3F FF FF FF FF FF FF FE  ------\n");

   filter_clas.filter_fields[0].field_id = WP_PCE_FIELD_ID_IPV6_SA_HIGH;
   filter_clas.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_clas.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   filter_clas.filter_fields[1].field_id = WP_PCE_FIELD_ID_IPV6_SA_LOW;
   filter_clas.filter_fields[1].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_clas.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_USED;
   for(i=0; i<8; i++)
      filter_clas.filter_fields[1].mask.ipv6_addr_half[i] = 0xFF;
   filter_clas.filter_fields[1].mask.ipv6_addr_half[0] = 0x01;
   filter_clas.filter_fields[1].mask.ipv6_addr_half[7] = 0xC0;

   filter_clas.filter_fields[2].field_id = WP_PCE_FIELD_ID_IPV6_DA_HIGH;
   filter_clas.filter_fields[2].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_clas.filter_fields[2].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   filter_clas.filter_fields[3].field_id = WP_PCE_FIELD_ID_IPV6_DA_LOW;
   filter_clas.filter_fields[3].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_clas.filter_fields[3].mask_mode = WP_PCE_FIELD_MASK_USED;
   for(i=0; i<8; i++)
      filter_clas.filter_fields[3].mask.ipv6_addr_half[i] = 0xFF;
   filter_clas.filter_fields[3].mask.ipv6_addr_half[0] = 0x3F;
   filter_clas.filter_fields[3].mask.ipv6_addr_half[7] = 0xFE;

   filter_clas.filter_fields[4].field_id = WP_PCE_FIELD_ID_LAST;
   WPE_TEST_CHECK_HANDLE(ext_filter3,
                     WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_CLASSIFICATION, &filter_clas));
   MY_PRINTF("\n---- Extended Filter (ext_filter3) Created: 0x%X ----------\n", (WP_U32) ext_filter3);

#ifdef DEBUG_PRINT
   WP_Display(WP_WINPATH(DEFAULT_WPID), ext_filter3, 0, 0);
#endif

   MY_PRINTF("\n---- rule2 (on filter2) creating ----------\n");
   rule_cfg.enabled = WP_ENABLE;
   rule_cfg.match_action = WP_PCE_RULE_MATCH_CONTINUE;
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_LAST;
   rule_cfg.filter_handle = filter2;

   rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_ETHER_ENCAP_TYPE;
   rule_cfg.rule_fields[0].value.ether_encap_type = WP_ENCAPSULATION_TYPE_LLC_SNAP;
   rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_VLAN_TYPE;
   rule_cfg.rule_fields[1].value.vlan_type = WP_VLAN_TYPE_S_PLUS_C_VLAN_TAGGED;
   rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_PPPOE_SESSION_ID;
   rule_cfg.rule_fields[2].value.pppoe_sid = 0xAD67;
   rule_cfg.rule_fields[3].field_id = WP_PCE_FIELD_ID_PPPOE_PROTOCOL_ID;
   rule_cfg.rule_fields[3].value.pppoe_pid = 0x4E55;
   rule_cfg.rule_fields[4].field_id = WP_PCE_FIELD_ID_LAST;

   WPE_TEST_CHECK_HANDLE(rule2,
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));
   MY_PRINTF("\n---- Single Rule (filter2) Created: 0x%X ----------\n", (WP_U32) rule2);

#ifdef DEBUG_PRINT
   WP_Display(WP_WINPATH(DEFAULT_WPID), rule2, 0, 0);
#endif

   rule_cfg.filter_handle = filter_1bit_mask;
   rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_IPV4_SA;
   rule_cfg.rule_fields[0].value.ipv4_addr = 0xFFFFFFFF;
   rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_LAST;
   WPE_TEST_CHECK_HANDLE(rule_1bit_mask,
                         WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));

#ifdef DEBUG_PRINT
   WP_Display(WP_WINPATH(DEFAULT_WPID), rule_1bit_mask, 0, 0);
#endif

   MY_PRINTF("\n---- Extended Rule (ext_filter2) Creating ----------\n");
   rule_cfg.filter_handle = ext_filter2;
   rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_IPV6_FLOW_LABEL;
   rule_cfg.rule_fields[0].value.ipv6_flow_label = 0x7ACE4F;
   rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_MPLS_HEADER1;
   rule_cfg.rule_fields[1].value.mpls_header = 0x111111;
   rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_MPLS_HEADER5;
   rule_cfg.rule_fields[2].value.mpls_header = 0x555555;
   rule_cfg.rule_fields[3].field_id = WP_PCE_FIELD_ID_NUM_OF_MPLS_HEADERS;
   rule_cfg.rule_fields[3].value.num_of_mpls_headers = 3;
   rule_cfg.rule_fields[4].field_id = WP_PCE_FIELD_ID_GRE_PROTOCOL;
   rule_cfg.rule_fields[4].value.gre_protocol = 0xCAFE;
   rule_cfg.rule_fields[5].field_id = WP_PCE_FIELD_ID_GRE_KEY;
   rule_cfg.rule_fields[5].value.gre_key = 0xABBAFACE;
   rule_cfg.rule_fields[6].field_id = WP_PCE_FIELD_ID_PARSER_FLAGS;
   rule_cfg.rule_fields[6].value.parser_flags = 0x56C2;
   rule_cfg.rule_fields[7].field_id = WP_PCE_FIELD_ID_MPLS_HEADER2;
   rule_cfg.rule_fields[7].value.mpls_header = 0x222222;
   rule_cfg.rule_fields[8].field_id = WP_PCE_FIELD_ID_MPLS_HEADER3;
   rule_cfg.rule_fields[8].value.mpls_header = 0x333333;
   rule_cfg.rule_fields[9].field_id = WP_PCE_FIELD_ID_LAST;
   WPE_TEST_CHECK_HANDLE(ext_rule2,
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));
   MY_PRINTF("\n---- Extended Rule (ext_filter2) Created: 0x%X ----------\n", (WP_U32) ext_rule2);

#ifdef DEBUG_PRINT
   WP_Display(WP_WINPATH(DEFAULT_WPID), ext_rule2, 0, 0);
#endif

   MY_PRINTF("\n---- Rule field IPv6 flow label wrong vaue: check error ----------\n");
   rule_cfg.rule_fields[0].value.ipv6_flow_label = 0x22E12345;
   WPE_TEST_CHECK_RESULT(rule3,
                     WPI_BASIC_ERROR(WP_ERR_PCE_RULE_ILLEGAL_IPV6_FLOW_NUMBER),
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));
   rule_cfg.rule_fields[0].value.ipv6_flow_label = 0xEA58D3;

   MY_PRINTF("\n---- Rule field MPLS HEADER wrong vaue: check error ----------\n");
   rule_cfg.rule_fields[2].value.mpls_header = 0x1555555;
   WPE_TEST_CHECK_RESULT(rule3,
                     WPI_BASIC_ERROR(WP_ERR_PCE_RULE_ILLEGAL_MPLS_HEADER),
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));
   rule_cfg.rule_fields[2].value.mpls_header = 0x555555;

   MY_PRINTF("\n---- Rule field number of MPLS headers wrong vaue: check error ----------\n");
   rule_cfg.rule_fields[3].value.num_of_mpls_headers = 17;
   WPE_TEST_CHECK_RESULT(rule3,
                     WPI_BASIC_ERROR(WP_ERR_PCE_RULE_ILLEGAL_NUM_OF_MPLS_HEADERS),
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));
   rule_cfg.rule_fields[3].value.num_of_mpls_headers = 2;

   rule_cfg.filter_handle = filter2;
   MY_PRINTF("\n---- Rule field ENCAPSULATION wrong vaue: check error ----------\n");
   rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_ETHER_ENCAP_TYPE;
   rule_cfg.rule_fields[0].value.ether_encap_type = WP_ENCAPSULATION_TYPE_UNKNOWN+1;
   WPE_TEST_CHECK_RESULT(rule3,
                     WPI_BASIC_ERROR(WP_ERR_PCE_RULE_ILLEGAL_ENCAPSULATION_TYPE),
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));
   rule_cfg.rule_fields[0].value.ether_encap_type = WP_ENCAPSULATION_TYPE_PURE_LLC;

   MY_PRINTF("\n---- Rule field VLAN type wrong vaue: check error ----------\n");
   rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_VLAN_TYPE;
   rule_cfg.rule_fields[1].value.vlan_type = WP_VLAN_TYPE_S_PLUS_C_VLAN_TAGGED+3;
   WPE_TEST_CHECK_RESULT(rule3,
                     WPI_BASIC_ERROR(WP_ERR_PCE_RULE_ILLEGAL_VLAN_TYPE),
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));
   rule_cfg.rule_fields[1].value.vlan_type = WP_VLAN_TYPE_PRIORITY_TAGGED;

   MY_PRINTF("\n---- Extended Rule (ext_rule3) Creating: \n  "
      "IPv6 SA HIGH 01 23 45 66 77 88 99 AA & IPv6 SA LOW FF BB CC DD EE DC BA FF (mask 01 FF FF FF FF FF FF C0 \n"
      "IPv6 DA HIGH 12 23 34 56 78 88 76 54 & IPv6 DA LOW FF AB BA CA FE FA CE FF (mask 3F FF FF FF FF FF FF FE  ------\n");
   rule_cfg.filter_handle = ext_filter3;

   rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_IPV6_SA_HIGH;
   rule_cfg.rule_fields[0].value.ipv6_addr_half[0] = 0x01;
   rule_cfg.rule_fields[0].value.ipv6_addr_half[1] = 0x23;
   rule_cfg.rule_fields[0].value.ipv6_addr_half[2] = 0x45;
   rule_cfg.rule_fields[0].value.ipv6_addr_half[3] = 0x66;
   rule_cfg.rule_fields[0].value.ipv6_addr_half[4] = 0x77;
   rule_cfg.rule_fields[0].value.ipv6_addr_half[5] = 0x88;
   rule_cfg.rule_fields[0].value.ipv6_addr_half[6] = 0x99;
   rule_cfg.rule_fields[0].value.ipv6_addr_half[7] = 0xAA;

   rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_IPV6_SA_LOW;
   rule_cfg.rule_fields[1].value.ipv6_addr_half[0] = 0xFF;
   rule_cfg.rule_fields[1].value.ipv6_addr_half[1] = 0xBB;
   rule_cfg.rule_fields[1].value.ipv6_addr_half[2] = 0xCC;
   rule_cfg.rule_fields[1].value.ipv6_addr_half[3] = 0xDD;
   rule_cfg.rule_fields[1].value.ipv6_addr_half[4] = 0xEE;
   rule_cfg.rule_fields[1].value.ipv6_addr_half[5] = 0xDC;
   rule_cfg.rule_fields[1].value.ipv6_addr_half[6] = 0xBA;
   rule_cfg.rule_fields[1].value.ipv6_addr_half[7] = 0xFF;

   rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_IPV6_DA_HIGH;
   rule_cfg.rule_fields[2].value.ipv6_addr_half[0] = 0x12;
   rule_cfg.rule_fields[2].value.ipv6_addr_half[1] = 0x23;
   rule_cfg.rule_fields[2].value.ipv6_addr_half[2] = 0x34;
   rule_cfg.rule_fields[2].value.ipv6_addr_half[3] = 0x56;
   rule_cfg.rule_fields[2].value.ipv6_addr_half[4] = 0x78;
   rule_cfg.rule_fields[2].value.ipv6_addr_half[5] = 0x88;
   rule_cfg.rule_fields[2].value.ipv6_addr_half[6] = 0x76;
   rule_cfg.rule_fields[2].value.ipv6_addr_half[7] = 0x54;

   rule_cfg.rule_fields[3].field_id = WP_PCE_FIELD_ID_IPV6_DA_LOW;
   rule_cfg.rule_fields[3].value.ipv6_addr_half[0] = 0xFF;
   rule_cfg.rule_fields[3].value.ipv6_addr_half[1] = 0xAB;
   rule_cfg.rule_fields[3].value.ipv6_addr_half[2] = 0xBA;
   rule_cfg.rule_fields[3].value.ipv6_addr_half[3] = 0xCA;
   rule_cfg.rule_fields[3].value.ipv6_addr_half[4] = 0xFE;
   rule_cfg.rule_fields[3].value.ipv6_addr_half[5] = 0xFA;
   rule_cfg.rule_fields[3].value.ipv6_addr_half[6] = 0xCE;
   rule_cfg.rule_fields[3].value.ipv6_addr_half[7] = 0xFF;

   rule_cfg.rule_fields[4].field_id = WP_PCE_FIELD_ID_LAST;
   WPE_TEST_CHECK_HANDLE(ext_rule3,
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));
   MY_PRINTF("\n---- Extended Rule (ext_rule3) Created: 0x%X ----------\n", (WP_U32) ext_rule3);

#ifdef DEBUG_PRINT
   WP_Display(WP_WINPATH(DEFAULT_WPID), ext_rule3, 0, 0);
#endif
}

static void WPE_PceFiltersTest(void)
{
   WP_status status;
   WP_pce_filter_classification filter_clas_cfg = {0};
   WP_pce_filter_classification filter_clas = {0};
   WP_handle filter_temp1;

   filter_clas_cfg.no_match_action = WP_PCE_FILTER_NO_MATCH_DENY;
   filter_clas_cfg.no_fields_action = WP_PCE_FILTER_NO_FIELDS_DENY;

   filter_clas_cfg.filter_fields[0].field_id = WP_PCE_FIELD_ID_INPUT_IW_PORT;
   filter_clas_cfg.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_clas_cfg.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   filter_clas_cfg.filter_fields[1].field_id = WP_PCE_FIELD_ID_LAST;
   filter_clas_cfg.filter_fields[1].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_clas_cfg.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   filter_clas_cfg.no_match_result[0].result_type = WP_PCE_RESULT_LAST;

   WPE_TEST_CHECK_HANDLE(filter,
                     WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_CLASSIFICATION, &filter_clas_cfg));

   filter_clas.no_match_action = WP_PCE_FILTER_NO_MATCH_CONTINUE;
   filter_clas.no_fields_action = WP_PCE_FILTER_NO_FIELDS_CONTINUE;
   filter_clas.no_match_result[0].result_type = WP_PCE_RESULT_LAST;

/***************************************************************
 * Single Filter Test
 **************************************************************/
   MY_PRINTF("\n---- Single Filter Test ------------\n");
   MY_PRINTF("\n------------- Single Filter Creating: MAC_DA MAC_SA ----------\n");
   filter_clas.filter_fields[0].field_id = WP_PCE_FIELD_ID_MAC_DA;
   filter_clas.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_clas.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   filter_clas.filter_fields[1].field_id = WP_PCE_FIELD_ID_MAC_SA;
   filter_clas.filter_fields[1].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_clas.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   filter_clas.filter_fields[2].field_id = WP_PCE_FIELD_ID_LAST;
   filter_clas.filter_fields[2].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_clas.filter_fields[2].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   WPE_TEST_CHECK_RESULT(filter,
                     WPI_BASIC_ERROR(WP_ERR_PCE_FILTER_TYPE_ILLEGAL),
                     WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), 100/* wrong filter type */, &filter_clas));

   filter_clas.filter_fields[0].field_id = 100;/* wrong filter field id */
   WPE_TEST_CHECK_RESULT(filter,
                     WPI_BASIC_ERROR(WP_ERR_PCE_FILTER_FIELD_NOT_SUPPORTED),
                     WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_CLASSIFICATION, &filter_clas));
   filter_clas.filter_fields[0].field_id = WP_PCE_FIELD_ID_MAC_DA;

   filter_clas.filter_fields[0].field_id = WP_PCE_FIELD_ID_LAST;/* wrong filter field id */
   WPE_TEST_CHECK_RESULT(filter,
                     WPI_BASIC_ERROR(WP_ERR_PCE_EMPTY_FILTER),
                     WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_CLASSIFICATION, &filter_clas));
   filter_clas.filter_fields[0].field_id = WP_PCE_FIELD_ID_MAC_DA;

   filter_clas.no_match_action = 100;
   WPE_TEST_CHECK_RESULT(filter,
                     WPI_BASIC_ERROR(WP_ERR_PCE_FILTER_ILLEGAL_NO_MATCH_ACTION),
                     WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_CLASSIFICATION, &filter_clas));
   filter_clas.no_match_action = WP_PCE_FILTER_NO_MATCH_CONTINUE;

   filter_clas.no_fields_action = 100;
   WPE_TEST_CHECK_RESULT(filter,
                     WPI_BASIC_ERROR(WP_ERR_PCE_FILTER_ILLEGAL_NO_FIELDS_ACTION),
                     WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_CLASSIFICATION, &filter_clas));
   filter_clas.no_fields_action = WP_PCE_FILTER_NO_FIELDS_CONTINUE;

   WPE_TEST_CHECK_HANDLE(filter,
                     WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_CLASSIFICATION, &filter_clas));
   MY_PRINTF("\n---- Single Filter Created: 0x%X ----------\n", (WP_U32) filter);

   /* Filter Result Create */
   filter_clas.no_match_result[0].result_type = WP_PCE_RESULT_IWCP_START_REDIRECT;
   filter_clas.no_match_result[1].result_type = WP_PCE_RESULT_LAST;
   WPE_TEST_CHECK_HANDLE(filter_temp1,
                     WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_CLASSIFICATION, &filter_clas));
   MY_PRINTF("\n---- Single Filter Created (with no match result): 0x%X ----------\n", (WP_U32) filter_temp1);

   filter_clas.no_match_result[0].result_type = WP_PCE_RESULT_IWCP_START_REDIRECT;
   filter_clas.no_match_result[1].result_type = WP_PCE_RESULT_IWCP_START_REDIRECT;
   WPE_TEST_CHECK_RESULT(status,
                     WPI_BASIC_ERROR(WP_ERR_PCE_FILTER_RESULT_TYPE_REPEATED),
                     WP_PceFilterModify(filter_temp1, WP_PCE_FILTER_MODIFY_COMMAND_RESULT, &filter_clas));

   filter_clas.no_match_result[0].result_type = WP_PCE_RESULT_LAST;
   WPE_TEST_CHECK_RESULT(status,
                     WP_OK,
                     WP_PceFilterModify(filter_temp1, WP_PCE_FILTER_MODIFY_COMMAND_RESULT, &filter_clas));
   MY_PRINTF("\n---- The Filter Modified (with no match result)\n");

   WPE_TEST_CHECK_RESULT(status,
                     WP_OK,
                     WP_PceFilterDelete(filter_temp1));
   MY_PRINTF("\n---- The Filter (with no match result) Deleted \n");

   WPE_TEST_CHECK_HANDLE(results_special_filter,
                     WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_CLASSIFICATION, &filter_clas));
   MY_PRINTF("\n---- Single Filter For Rule Results Created: 0x%X ----------\n", (WP_U32) results_special_filter);

   WPE_TEST_CHECK_HANDLE(iterator_handles[0],
                     WP_PceIteratorCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_ITERATOR_TYPE_RULE, &filter));

   WPE_TEST_CHECK_RESULT(status,
                     WPI_BASIC_ERROR(WP_ERR_PCE_FILTER_USED_ITERATORS),
                     WP_PceFilterDelete(filter));
/*
   WPE_TEST_CHECK_RESULT(status,
                     WP_OK,
                     WP_PceIteratorDelete(iterator_handles[0]));
*/
   WPE_TEST_CHECK_RESULT(status,
                         WPI_BASIC_ERROR(WP_ERR_HANDLE),
                     WP_PceFilterModify(0, WP_PCE_FILTER_MODIFY_COMMAND_NO_MATCH, &filter_clas));

   WPE_TEST_CHECK_RESULT(status,
                     WPI_BASIC_ERROR(WP_ERR_HANDLE),
                     WP_PceFilterModify(filter+300, WP_PCE_FILTER_MODIFY_COMMAND_NO_MATCH, &filter_clas));

   filter_clas.no_match_action = 100;
   WPE_TEST_CHECK_RESULT(status,
                     WPI_BASIC_ERROR(WP_ERR_PCE_FILTER_ILLEGAL_NO_MATCH_ACTION),
                     WP_PceFilterModify(filter, WP_PCE_FILTER_MODIFY_COMMAND_NO_MATCH, &filter_clas));
   filter_clas.no_match_action = WP_PCE_FILTER_NO_MATCH_CONTINUE;

   filter_clas.no_fields_action = 100;
   WPE_TEST_CHECK_RESULT(status,
                     WPI_BASIC_ERROR(WP_ERR_PCE_FILTER_ILLEGAL_NO_FIELDS_ACTION),
                     WP_PceFilterModify(filter, WP_PCE_FILTER_MODIFY_COMMAND_NO_FIELDS, &filter_clas));
   filter_clas.no_fields_action = WP_PCE_FILTER_NO_FIELDS_CONTINUE;

   WPE_TEST_CHECK_RESULT(status,
                     WPI_BASIC_ERROR(WP_ERR_PCE_FILTER_MODIFY_ILLEGAL_COMMAND),
                     WP_PceFilterModify(filter, 100 /* wrong filter modify command */, &filter_clas));

   MY_PRINTF("\n---- One More Single Filter (filter1) Creating: MAC_DA MAC_SA ----------\n");
   WPE_TEST_CHECK_HANDLE(filter1,
                     WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_CLASSIFICATION, &filter_clas));
   MY_PRINTF("\n---- Single Filter (filter1) Created: 0x%X ----------\n", (WP_U32) filter1);

/***************************************************************
 * Extended Filter Test
 **************************************************************/
   MY_PRINTF("\n---- Extended Filter Test ------------\n");
   MY_PRINTF("\n------------- Extended Filter Creating: MAC_DA MAC_SA IPV4_SA IPV4_DA VLAN_ID ----------\n");
   filter_clas.filter_fields[0].field_id = WP_PCE_FIELD_ID_MAC_DA;
   filter_clas.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_clas.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   filter_clas.filter_fields[1].field_id = WP_PCE_FIELD_ID_MAC_SA;
   filter_clas.filter_fields[1].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_clas.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   filter_clas.filter_fields[2].field_id = WP_PCE_FIELD_ID_IPV4_SA;
   filter_clas.filter_fields[2].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_clas.filter_fields[2].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   filter_clas.filter_fields[3].field_id = WP_PCE_FIELD_ID_IPV4_DA;
   filter_clas.filter_fields[3].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_clas.filter_fields[3].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   filter_clas.filter_fields[4].field_id = WP_PCE_FIELD_ID_EXT_VLAN_TAG;
   filter_clas.filter_fields[4].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_clas.filter_fields[4].mask_mode = WP_PCE_FIELD_MASK_USED;
   filter_clas.filter_fields[4].mask.vlan_tag = WP_PCE_FIELD_MASK_VLAN_ID;

   filter_clas.filter_fields[5].field_id = WP_PCE_FIELD_ID_LAST;
   filter_clas.filter_fields[5].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_clas.filter_fields[5].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   WPE_TEST_CHECK_HANDLE(ext_filter,
                     WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_CLASSIFICATION, &filter_clas));
   MY_PRINTF("\n---- Extended Filter Created: 0x%X ----------\n", (WP_U32) ext_filter);

   MY_PRINTF("\n---- One More Extended Filter Creating: MAC_DA MAC_SA IPV4_SA IPV4_DA VLAN_ID ----------\n");
   WPE_TEST_CHECK_HANDLE(ext_filter1,
                     WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_CLASSIFICATION, &filter_clas));
   MY_PRINTF("\n---- Extended Filter Created: 0x%X ----------\n", (WP_U32) ext_filter1);
}

static void WPE_PceDeleteFiltersTest(void)
{
   WP_status status;

   MY_PRINTF("Deleting filters test started.\n");
   MY_PRINTF("Filter delete 0x%X\n", filter);
   WPE_TEST_CHECK_RESULT(status,
                     WP_OK,
                     WP_PceFilterDelete(filter));

   WPE_TEST_CHECK_RESULT(status,
                         WPI_BASIC_ERROR(WP_ERR_HANDLE),
                     WP_PceFilterDelete(filter+100));
   
   WPE_TEST_CHECK_RESULT(status,
                         WPI_BASIC_ERROR(WP_ERR_HANDLE),
                     WP_PceFilterDelete(0));
   
   MY_PRINTF("Already deleted filter delete 0x%X\n", filter);
   WPE_TEST_CHECK_RESULT(status,
                         WPI_BASIC_ERROR(WP_ERR_HANDLE),
                         WP_PceFilterDelete(filter));

   MY_PRINTF("Filter 1 delete\n");
   WPE_TEST_CHECK_RESULT(status,
                     WP_OK,
                     WP_PceFilterDelete(filter1));

   MY_PRINTF("Extended filter delete\n");
   WPE_TEST_CHECK_RESULT(status,
                     WP_OK,
                     WP_PceFilterDelete(ext_filter));

   MY_PRINTF("Extended filter 1 delete\n");
   WPE_TEST_CHECK_RESULT(status,
                     WP_OK,
                     WP_PceFilterDelete(ext_filter1));

   MY_PRINTF("Deleting filters test finished.\n");
}

static void WPE_PceFilterSetsCreateTest(void)
{
   WP_U16 i;
   WP_handle fs_tmp_handle;
   WP_status status;
   WP_pce_filter_set fs_level1_1 = {0};
   WP_pce_filter_set fs_level1_2 = {0};
   WP_pce_filter_set fs_level0 = {0};

   fs_level1_1.filter_set_level = 1;
   fs_level1_1.next_filter_set = WP_UNUSED;
   fs_level1_1.filters[0] = filter1;
   fs_level1_1.filters[1] = filter1;
   fs_level1_1.filters[2] = ext_filter;
   fs_level1_1.filters[3] = ext_filter1;
   fs_level1_1.filters[4] = WP_UNUSED;

   fs_level1_1.filters[2] = 10000;/* wrong filter handle */
   WPE_TEST_CHECK_RESULT(filter_set_lvl1_handle,
                     WPI_BASIC_ERROR(WP_ERR_HANDLE),
                     WP_PceFilterSetCreate(WP_WINPATH(DEFAULT_WPID), &fs_level1_1));
   fs_level1_1.filters[2] = ext_filter;

   fs_level1_1.next_filter_set = 1000;/* wrong filter set handle */
   WPE_TEST_CHECK_RESULT(filter_set_lvl1_handle,
                     WPI_BASIC_ERROR(WP_ERR_HANDLE),
                     WP_PceFilterSetCreate(WP_WINPATH(DEFAULT_WPID), &fs_level1_1));
   fs_level1_1.next_filter_set = WP_UNUSED;

   fs_level1_1.filters[0] = WP_UNUSED;
   WPE_TEST_CHECK_RESULT(filter_set_lvl1_handle,
                     WPI_BASIC_ERROR(WP_ERR_PCE_FILTER_SET_IS_EMPTY),
                     WP_PceFilterSetCreate(WP_WINPATH(DEFAULT_WPID), &fs_level1_1));
   fs_level1_1.filters[0] = filter1;

   MY_PRINTF("\n---- Level 1 Filter Set Creating: -------------------------\n");
   WPE_TEST_CHECK_HANDLE(filter_set_lvl1_handle,
                     WP_PceFilterSetCreate(WP_WINPATH(DEFAULT_WPID), &fs_level1_1));
   MY_PRINTF("\n---- Level 1 Filter Set  Created: 0x%X ----------\n", (WP_U32) filter_set_lvl1_handle);

#ifdef DEBUG_PRINT
   WP_Display(WP_WINPATH(DEFAULT_WPID), filter_set_lvl1_handle, 0, 0);
#endif

   fs_level1_2.filter_set_level = 1;
   fs_level1_2.next_filter_set = WP_UNUSED;
   fs_level1_2.filters[0] = ext_filter;
   fs_level1_2.filters[1] = filter1;
   fs_level1_2.filters[2] = ext_filter1;
   fs_level1_2.filters[3] = filter;
   fs_level1_2.filters[4] = ext_filter1;
   fs_level1_2.filters[5] = filter1;
   fs_level1_2.filters[6] = WP_UNUSED;

   MY_PRINTF("\n---- Level 1 Filter Set 2 Creating: -------------------------\n");
   WPE_TEST_CHECK_HANDLE(filter_set_lvl1_handle2,
                     WP_PceFilterSetCreate(WP_WINPATH(DEFAULT_WPID), &fs_level1_2));
   MY_PRINTF("\n---- Level 1 Filter Set  Created: 0x%X ----------\n", (WP_U32) filter_set_lvl1_handle2);

#ifdef DEBUG_PRINT
   WP_Display(WP_WINPATH(DEFAULT_WPID), filter_set_lvl1_handle2, 0, 0);
#endif

   WPE_TEST_CHECK_RESULT(status,
                     WPI_BASIC_ERROR(WP_ERR_PCE_FILTER_USED_FILTER_SETS),
                     WP_PceFilterDelete(filter));

   fs_level0.filter_set_level = 0;
   fs_level0.next_filter_set = filter_set_lvl1_handle;
   fs_level0.filters[0] = filter;
   fs_level0.filters[1] = ext_filter;
   fs_level0.filters[2] = filter1;
   fs_level0.filters[3] = ext_filter1;
   fs_level0.filters[4] = WP_UNUSED;

   fs_level0.next_filter_set = filter_set_lvl1_handle+10000;/* wrong filter set handle */
   WPE_TEST_CHECK_RESULT(filter_set_lvl0_handle,
                     WPI_BASIC_ERROR(WP_ERR_HANDLE),
                     WP_PceFilterSetCreate(WP_WINPATH(DEFAULT_WPID), &fs_level0));
   fs_level0.next_filter_set = filter_set_lvl1_handle;

   fs_level0.next_filter_set = filter_set_lvl1_handle+100;/* wrong filter set handle */
   WPE_TEST_CHECK_RESULT(filter_set_lvl0_handle,
                     WPI_BASIC_ERROR(WP_ERR_HANDLE),
                     WP_PceFilterSetCreate(WP_WINPATH(DEFAULT_WPID), &fs_level0));
   fs_level0.next_filter_set = filter_set_lvl1_handle;

   MY_PRINTF("\n---- Level 0 Filter Set Creating: -------------------------\n");
   WPE_TEST_CHECK_HANDLE(filter_set_lvl0_handle,
                     WP_PceFilterSetCreate(WP_WINPATH(DEFAULT_WPID), &fs_level0));
   MY_PRINTF("\n---- Level 0 Filter Set  Created: 0x%X ----------\n", (WP_U32) filter_set_lvl0_handle);

   fs_level0.next_filter_set = filter_set_lvl0_handle;
   WPE_TEST_CHECK_RESULT(fs_tmp_handle,
                     WPI_BASIC_ERROR(WP_ERR_PCE_FILTER_SET_LOOP_PREVENTION),
                     WP_PceFilterSetCreate(WP_WINPATH(DEFAULT_WPID), &fs_level0));
   fs_level0.next_filter_set = filter_set_lvl1_handle;

   MY_PRINTF("\n---- Level 0 Filter Set Modifying: Test Error cases -----------\n");
   WPE_TEST_CHECK_RESULT(status,
                     WPI_BASIC_ERROR(WP_ERR_HANDLE),
                     WP_PceFilterSetModify(filter+100, WP_PCE_FILTER_SET_MODIFY_COMMAND_NEXT_FS, &fs_level0));

   WPE_TEST_CHECK_RESULT(status,
                     WPI_BASIC_ERROR(WP_ERR_HANDLE),
                     WP_PceFilterSetModify(filter_set_lvl0_handle+10000, WP_PCE_FILTER_SET_MODIFY_COMMAND_NEXT_FS, &fs_level0));

   WPE_TEST_CHECK_RESULT(status,
                     WPI_BASIC_ERROR(WP_ERR_HANDLE),
                     WP_PceFilterSetModify(filter_set_lvl0_handle+100, WP_PCE_FILTER_SET_MODIFY_COMMAND_NEXT_FS, &fs_level0));

   fs_level0.next_filter_set = filter+100;
   WPE_TEST_CHECK_RESULT(status,
                     WPI_BASIC_ERROR(WP_ERR_HANDLE),
                     WP_PceFilterSetModify(filter_set_lvl0_handle, WP_PCE_FILTER_SET_MODIFY_COMMAND_NEXT_FS, &fs_level0));
   fs_level0.next_filter_set = filter_set_lvl1_handle;

#ifdef DEBUG_PRINT
   WP_Display(WP_WINPATH(DEFAULT_WPID), filter_set_lvl0_handle, 0, 0);
#endif

   fs_level0.next_filter_set = filter_set_lvl0_handle+10000;
   WPE_TEST_CHECK_RESULT(status,
                     WPI_BASIC_ERROR(WP_ERR_HANDLE),
                     WP_PceFilterSetModify(filter_set_lvl0_handle, WP_PCE_FILTER_SET_MODIFY_COMMAND_NEXT_FS, &fs_level0));
   fs_level0.next_filter_set = filter_set_lvl1_handle;

   fs_level0.next_filter_set = filter_set_lvl0_handle+100;
   WPE_TEST_CHECK_RESULT(status,
                     WPI_BASIC_ERROR(WP_ERR_HANDLE),
                     WP_PceFilterSetModify(filter_set_lvl0_handle, WP_PCE_FILTER_SET_MODIFY_COMMAND_NEXT_FS, &fs_level0));
   fs_level0.next_filter_set = filter_set_lvl1_handle;

   fs_level0.next_filter_set = filter_set_lvl0_handle;
   WPE_TEST_CHECK_RESULT(status,
                     WPI_BASIC_ERROR(WP_ERR_PCE_FILTER_SET_LOOP_PREVENTION),
                     WP_PceFilterSetModify(filter_set_lvl0_handle, WP_PCE_FILTER_SET_MODIFY_COMMAND_NEXT_FS, &fs_level0));
   fs_level0.next_filter_set = filter_set_lvl1_handle;

   WPE_TEST_CHECK_RESULT(status,
                     WPI_BASIC_ERROR(WP_ERR_UNSUPPORTED_COMMAND),
                     WP_PceFilterSetModify(filter_set_lvl1_handle, 100 /* wrong command */, &fs_level0));

   MY_PRINTF("\n---- Level 0 Filter Set Modifying: the next fs NULL -------------\n");
   fs_level0.next_filter_set = WP_UNUSED;
   WPE_TEST_CHECK_RESULT(status,
                     WP_OK,
                     WP_PceFilterSetModify(filter_set_lvl0_handle, WP_PCE_FILTER_SET_MODIFY_COMMAND_NEXT_FS, &fs_level0));
   fs_level0.next_filter_set = filter_set_lvl1_handle;

   MY_PRINTF("\n---- Level 0 Filter Set Modifying: the next fs FS2  -------------\n");
   fs_level0.next_filter_set = filter_set_lvl1_handle2;
   WPE_TEST_CHECK_RESULT(status,
                     WP_OK,
                     WP_PceFilterSetModify(filter_set_lvl0_handle, WP_PCE_FILTER_SET_MODIFY_COMMAND_NEXT_FS, &fs_level0));
   fs_level0.next_filter_set = filter_set_lvl1_handle;

   MY_PRINTF("\n---- Level 0 Filter Set Modifying: the filters -------------\n");
   WPE_TEST_CHECK_RESULT(status,
                     WP_OK,
                     WP_PceFilterSetModify(filter_set_lvl0_handle, WP_PCE_FILTER_SET_MODIFY_COMMAND_FILTERS, &fs_level1_2));

   fs_level0.filters[0] = WP_UNUSED;
   WPE_TEST_CHECK_RESULT(status,
                     WPI_BASIC_ERROR(WP_ERR_PCE_FILTER_SET_IS_EMPTY),
                     WP_PceFilterSetModify(filter_set_lvl0_handle, WP_PCE_FILTER_SET_MODIFY_COMMAND_FILTERS, &fs_level0));

   for(i=0; i<WP_PCE_MAX_NUM_OF_FILTERS_IN_SET; i++)
      fs_level0.filters[i] = filter;
   fs_level0.next_filter_set = WP_UNUSED;
   fs_level0.filters[WP_PCE_MAX_NUM_OF_FILTERS_IN_SET-1] = WP_UNUSED;
   WPE_TEST_CHECK_HANDLE(fs_tmp_handle,
                     WP_PceFilterSetCreate(WP_WINPATH(DEFAULT_WPID), &fs_level0));

   WPE_TEST_CHECK_RESULT(status,
                     WP_OK,
                     WP_PceFilterSetDelete(fs_tmp_handle));

   fs_level0.filters[WP_PCE_MAX_NUM_OF_FILTERS_IN_SET-1] = filter;
   WPE_TEST_CHECK_HANDLE(fs_tmp_handle,
                     WP_PceFilterSetCreate(WP_WINPATH(DEFAULT_WPID), &fs_level0));

   WPE_TEST_CHECK_RESULT(status,
                     WP_OK,
                     WP_PceFilterSetDelete(fs_tmp_handle));
}

#define PCE_FS_LEVEL_FILTERS_NUM (WP_PCE_FILTER_SET_MAX_LEVEL*10+10)

static void WPE_PceFilterSetsLevelsTest(void)
{
   WP_status status;
   WP_U16 i, j;
   WP_handle filters[PCE_FS_LEVEL_FILTERS_NUM];
   WP_handle filter_sets[WP_PCE_FILTER_SET_MAX_LEVEL*2+5];
   WP_handle rules[PCE_FS_LEVEL_FILTERS_NUM+40];
   WP_pce_filter_set fs_config = {0};
   WP_pce_filter_classification filter_clas = {0};
   WP_pce_rule_classification rule_cfg = {0};
   WP_U8 mac_addr[] = {0xCA, 0xFE, 0xBB, 0xCC, 0xDD, 0xEE};

   filter_clas.no_match_action = WP_PCE_FILTER_NO_MATCH_CONTINUE;
   filter_clas.no_fields_action = WP_PCE_FILTER_NO_FIELDS_CONTINUE;
   filter_clas.no_match_result[0].result_type = WP_PCE_RESULT_LAST;

   MY_PRINTF("\n---- Creating %d Filters ------------\n", PCE_FS_LEVEL_FILTERS_NUM);
   filter_clas.filter_fields[0].field_id = WP_PCE_FIELD_ID_MAC_DA;
   filter_clas.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_clas.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   filter_clas.filter_fields[1].field_id = WP_PCE_FIELD_ID_LAST;
   filter_clas.filter_fields[1].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_clas.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   for(i=0; i<PCE_FS_LEVEL_FILTERS_NUM; i++)
   {
      WPE_TEST_CHECK_HANDLE(filters[i],
                        WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_CLASSIFICATION, &filter_clas));
      MY_PRINTF("\n---- Filter[%d] Created: 0x%X ----------\n", i, (WP_U32) filters[i]);
   }

   MY_PRINTF("\n---- Creating 22 Filters Sets, 2 for each level ------------\n");
   for(i=0; i<=WP_PCE_FILTER_SET_MAX_LEVEL; i++)
   {
      fs_config.filter_set_level = i;
      fs_config.next_filter_set = WP_UNUSED;
      fs_config.filters[0] = filters[i*10+1];
      fs_config.filters[1] = filters[i*10+2];
      fs_config.filters[2] = filters[i*10+3];
      fs_config.filters[3] = filters[i*10+4];
      fs_config.filters[4] = WP_UNUSED;

      MY_PRINTF("\n---- Level %02d Filter Set Creating: -------------------------\n", i);
      WPE_TEST_CHECK_HANDLE(filter_sets[i*2],
                        WP_PceFilterSetCreate(WP_WINPATH(DEFAULT_WPID), &fs_config));
      MY_PRINTF("\n---- Level %02d Filter Set  Created: 0x%X ----------\n", i, (WP_U32) filter_sets[i*2]);

      fs_config.filter_set_level = i;
      fs_config.next_filter_set = WP_UNUSED;
      fs_config.filters[0] = filters[i*10+0];
      fs_config.filters[1] = filters[i*10+5];
      fs_config.filters[2] = filters[i*10+6];
      fs_config.filters[3] = filters[i*10+7];
      fs_config.filters[4] = filters[i*10+8];
      fs_config.filters[5] = filters[i*10+9];
      fs_config.filters[6] = ext_filter1;
      fs_config.filters[7] = WP_UNUSED;

      MY_PRINTF("\n---- Level %02d Filter Set Creating: -------------------------\n", i);
      WPE_TEST_CHECK_HANDLE(filter_sets[i*2+1],
                        WP_PceFilterSetCreate(WP_WINPATH(DEFAULT_WPID), &fs_config));
      MY_PRINTF("\n---- Level %02d Filter Set  Created: 0x%X ----------\n", i, (WP_U32) filter_sets[i*2+1]);
   }

   MY_PRINTF("\n---- Creating %d Rules ------------\n", PCE_FS_LEVEL_FILTERS_NUM);
   rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_MAC_DA;
   for(i=0; i<6; i++)
      rule_cfg.rule_fields[0].value.mac_addr[i] = mac_addr[i];
   rule_cfg.enabled = WP_ENABLE;
   rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_LAST;
   rule_cfg.match_action = WP_PCE_RULE_MATCH_CONTINUE;

   for(i=0; i<PCE_FS_LEVEL_FILTERS_NUM; i++)
   {
      rule_cfg.filter_handle = filters[i];

      if(i<50 && i%3 == 0)
      {
         rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FILTER_SET_CHANGE;
         rule_cfg.match_result[0].param.change_fs.filter_set_handle = filter_sets[i/5+10];
         rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;
      }
      else
      {
         rule_cfg.match_result[0].result_type = WP_PCE_RESULT_LAST;
      }
      MY_PRINTF("\n---- Rule Creating -- %d ---------------------------------\n", i);
      WPE_TEST_CHECK_HANDLE(rules[i],
                        WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));
      MY_PRINTF("rules[%d] = %X\n", i, rules[i]);
   }

   MY_PRINTF("\n---- Modifying Filters Sets, the NEXT_FS command ------------\n");
   for(i=0; i<=WP_PCE_FILTER_SET_MAX_LEVEL; i++)
   {
      if(i>0)
         fs_config.next_filter_set = filter_sets[i*2-1];
      else
         fs_config.next_filter_set = filter_sets[i*2+1];
      WPE_TEST_CHECK_RESULT(status,
                  WPI_BASIC_ERROR(WP_ERR_PCE_FILTER_SET_LOOP_PREVENTION),
                  WP_PceFilterSetModify(filter_sets[i*2], WP_PCE_FILTER_SET_MODIFY_COMMAND_NEXT_FS, &fs_config));
      if(i<WP_PCE_FILTER_SET_MAX_LEVEL-1)
      {
         fs_config.next_filter_set = filter_sets[i*2+2];
         WPE_TEST_CHECK_RESULT(status,
                     WP_OK,
                     WP_PceFilterSetModify(filter_sets[i*2], WP_PCE_FILTER_SET_MODIFY_COMMAND_NEXT_FS, &fs_config));
         fs_config.next_filter_set = filter_sets[i*2+3];
         WPE_TEST_CHECK_RESULT(status,
                     WP_OK,
                     WP_PceFilterSetModify(filter_sets[i*2+1], WP_PCE_FILTER_SET_MODIFY_COMMAND_NEXT_FS, &fs_config));
      }
   }

   MY_PRINTF("\n---- Modifying Rule Results to Jump ------------\n");
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FILTER_SET_CHANGE;
   rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;
   for(i=0; i<PCE_FS_LEVEL_FILTERS_NUM; i++)
   {
      if(i/10 < WP_PCE_FILTER_SET_MAX_LEVEL-1)
      {
         rule_cfg.match_result[0].param.change_fs.filter_set_handle = filter_sets[i/5+4];
         MY_PRINTF("\n---- Rule[%d] Modifying -----------------------------------\n", i);
         WPE_TEST_CHECK_RESULT(status,
                     WP_OK,
                     WP_PceRuleModify(rules[i], WP_PCE_RULE_MODIFY_COMMAND_RESULT, &rule_cfg));
      }

      if(i/10 > 0)
         rule_cfg.match_result[0].param.change_fs.filter_set_handle = filter_sets[i/5-2];
      else
         rule_cfg.match_result[0].param.change_fs.filter_set_handle = filter_sets[i/5];
      WPE_TEST_CHECK_RESULT(status,
                  WPI_BASIC_ERROR(WP_ERR_PCE_RULE_RESULT_FS_LOOP_PREVENTION),
                  WP_PceRuleModify(rules[i], WP_PCE_RULE_MODIFY_COMMAND_RESULT, &rule_cfg));

      rule_cfg.match_result[0].param.change_fs.filter_set_handle = filters[i];
      WPE_TEST_CHECK_RESULT(status,
                  WPI_BASIC_ERROR(WP_ERR_HANDLE),
                  WP_PceRuleModify(rules[i], WP_PCE_RULE_MODIFY_COMMAND_RESULT, &rule_cfg));

      rule_cfg.match_result[0].param.change_fs.filter_set_handle = filter_sets[0]+500;
      WPE_TEST_CHECK_RESULT(status,
                  WPI_BASIC_ERROR(WP_ERR_HANDLE),
                  WP_PceRuleModify(rules[i], WP_PCE_RULE_MODIFY_COMMAND_RESULT, &rule_cfg));
   }

   MY_PRINTF("\n---- Check Filter Set Modifications ------------\n");
   for(i=3; i<=7; i++)
   {
      j=i-1;
      fs_config.filters[0] = filters[j*10+1];
      fs_config.filters[1] = filters[j*10+2];
      fs_config.filters[2] = filters[j*10+3];
      fs_config.filters[3] = filters[j*10+4];
      fs_config.filters[4] = WP_UNUSED;

      MY_PRINTF("\n---- Level %02d Filter Set Modifying: -------------------------\n", i);
      WPE_TEST_CHECK_RESULT(status,
                  WP_OK,
                  WP_PceFilterSetModify(filter_sets[i*2], WP_PCE_FILTER_SET_MODIFY_COMMAND_FILTERS, &fs_config));

      MY_PRINTF("\n---- Level %02d Filter Set Modifying (filter jumps to the same level FS: ---------\n", i);
      j=i-2;
      fs_config.filters[0] = filters[j*10+1];
      WPE_TEST_CHECK_RESULT(status,
                  WPI_BASIC_ERROR(WP_ERR_PCE_FILTER_SET_LOOP_PREVENTION),
                  WP_PceFilterSetModify(filter_sets[i*2], WP_PCE_FILTER_SET_MODIFY_COMMAND_FILTERS, &fs_config));

      MY_PRINTF("\n---- Level %02d Filter Set Modifying (filter jumps to the lower level FS: --------\n", i);
      j=i-3;
      fs_config.filters[0] = filters[j*10+1];
      WPE_TEST_CHECK_RESULT(status,
                  WPI_BASIC_ERROR(WP_ERR_PCE_FILTER_SET_LOOP_PREVENTION),
                  WP_PceFilterSetModify(filter_sets[i*2], WP_PCE_FILTER_SET_MODIFY_COMMAND_FILTERS, &fs_config));
   }

   i=5;
   MY_PRINTF("\n---- Check Used Filter Set[%d] Deleting: 0x%X ----------\n", i*2, (WP_U32) filter_sets[i*2]);
   WPE_TEST_CHECK_RESULT(status,
                     WPI_BASIC_ERROR(WP_ERR_PCE_FILTER_SET_IN_USE),
                     WP_PceFilterSetDelete(filter_sets[i*2]));
   i=0;
   MY_PRINTF("\n---- Filter Set[%d] Deleting: 0x%X ----------\n", i*2, (WP_U32) filter_sets[i*2]);
   WPE_TEST_CHECK_RESULT(status,
                     WP_OK,
                     WP_PceFilterSetDelete(filter_sets[i*2]));
   MY_PRINTF("\n---- Filter Set[%d] Deleting: 0x%X ----------\n", i*2+1, (WP_U32) filter_sets[i*2+1]);
   WPE_TEST_CHECK_RESULT(status,
                     WP_OK,
                     WP_PceFilterSetDelete(filter_sets[i*2+1]));

   for(i=0; i<PCE_FS_LEVEL_FILTERS_NUM; i++)
   {
      MY_PRINTF("\n---- Rule[%d] Deleting: 0x%X ------------\n", i, (WP_U32) rules[i]);
      WPE_TEST_CHECK_RESULT(status,
                        WP_OK,
                        WP_PceRuleDelete(rules[i]));
   }

   for(i=1; i<=WP_PCE_FILTER_SET_MAX_LEVEL; i++)
   {
      MY_PRINTF("\n---- Filter Set[%d] Deleting: 0x%X ----------\n", i*2, (WP_U32) filter_sets[i*2]);
      WPE_TEST_CHECK_RESULT(status,
                        WP_OK,
                        WP_PceFilterSetDelete(filter_sets[i*2]));
      MY_PRINTF("\n---- Filter Set[%d] Deleting: 0x%X ----------\n", i*2+1, (WP_U32) filter_sets[i*2+1]);
      WPE_TEST_CHECK_RESULT(status,
                        WP_OK,
                        WP_PceFilterSetDelete(filter_sets[i*2+1]));
   }

   for(i=0; i<PCE_FS_LEVEL_FILTERS_NUM; i++)
   {
      MY_PRINTF("\n---- Filter[%d] Deleting: 0x%X ----------\n", i, (WP_U32) filters[i]);
      WPE_TEST_CHECK_RESULT(status,
                        WP_OK,
                        WP_PceFilterDelete(filters[i]));
   }
}

static void WPE_PceFilterSetsDeleteTest(void)
{
   WP_handle status;

   MY_PRINTF("Deleting filter sets test started.\n");

   WPE_TEST_CHECK_RESULT(status,
                     WPI_BASIC_ERROR(WP_ERR_HANDLE),
                     WP_PceFilterSetDelete(filter /* wrong handle type: not filter set handle */));

   WPE_TEST_CHECK_RESULT(status,
                     WPI_BASIC_ERROR(WP_ERR_HANDLE),
                     WP_PceFilterSetDelete(filter_set_lvl1_handle+10000));

   WPE_TEST_CHECK_RESULT(status,
                     WPI_BASIC_ERROR(WP_ERR_HANDLE),
                     WP_PceFilterSetDelete(filter_set_lvl1_handle+100));

   WPE_TEST_CHECK_RESULT(status,
                     WPI_BASIC_ERROR(WP_ERR_PCE_FILTER_SET_IN_USE),
                     WP_PceFilterSetDelete(filter_set_lvl1_handle2));

   MY_PRINTF("Level 0 Filter set delete.\n");
   WPE_TEST_CHECK_RESULT(status,
                     WP_OK,
                     WP_PceFilterSetDelete(filter_set_lvl0_handle));

   MY_PRINTF("Level 1 Filter set delete.\n");
   WPE_TEST_CHECK_RESULT(status,
                     WP_OK,
                     WP_PceFilterSetDelete(filter_set_lvl1_handle));

   MY_PRINTF("Level 1 Filter set 2 delete.\n");
   WPE_TEST_CHECK_RESULT(status,
                     WP_OK,
                     WP_PceFilterSetDelete(filter_set_lvl1_handle2));

   MY_PRINTF("Deleting filter sets test finished.\n");
}

/***************************************************************
 * Additional Range Fields
 **************************************************************/
static void WPE_AddedRangesTest(void)
{
   WP_U16 i, j, rng_flt_cnt=0, rng_rule_cnt=0;
   WP_U8 mac_addr1[]  = {0x11, 0x22, 0x11, 0x22, 0x11, 0x22};
   WP_U8 mac_addr2[]  = {0x33, 0x44, 0x55, 0x33, 0x44, 0x55};
   WP_U8 mac_addr3[]  = {0x66, 0x77, 0x88, 0x66, 0x77, 0x88};
   WP_U8 mac_addr4[]  = {0xAA, 0xCC, 0xEE, 0xBB, 0xDD, 0xFF};
   WP_U8 mac_mask1[] = {0x00, 0x00, 0x00, 0xFF, 0x00, 0x00};
   WP_U8 mac_mask2[] = {0x00, 0x00, 0x00, 0x3F, 0xFE, 0x00};
   WP_U8 mac_mask3[] = {0x00, 0x00, 0x00, 0x1F, 0xF8, 0x00};
   WP_U8 mac_mask4[] = {0x00, 0x00, 0x01, 0xFF, 0x80, 0x00};
   WP_U8 mac_mask5[] = {0x00, 0x7F, 0xFF, 0xFF, 0xFC, 0x00};
   WP_pce_filter_classification filter_clas = {0};
   WP_pce_rule_classification rule_cfg = {0};
   WP_handle rule_handle[20], tmp_rng_filters[20];

   rule_handle[0] = 0;

   filter_clas.no_match_action = WP_PCE_FILTER_NO_MATCH_CONTINUE;
   filter_clas.no_fields_action = WP_PCE_FILTER_NO_FIELDS_CONTINUE;
   filter_clas.no_match_result[0].result_type = WP_PCE_RESULT_LAST;

   filter_clas.filter_fields[0].field_id = WP_PCE_FIELD_ID_INT_VLAN_TAG;
   filter_clas.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_LOWER_THAN;
   filter_clas.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   filter_clas.filter_fields[1].field_id = WP_PCE_FIELD_ID_VLAN_TYPE;
   filter_clas.filter_fields[1].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_clas.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   filter_clas.filter_fields[2].field_id = WP_PCE_FIELD_ID_MAC_SA;
   filter_clas.filter_fields[2].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_clas.filter_fields[2].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   filter_clas.filter_fields[3].field_id = WP_PCE_FIELD_ID_LAST;
   WPE_TEST_CHECK_HANDLE(tmp_rng_filters[rng_flt_cnt],
      WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_CLASSIFICATION, &filter_clas));
   MY_PRINTF("\n---- tmp_rng_filters[%d]: 0x%X ----------\n", rng_flt_cnt, (WP_U32) tmp_rng_filters[rng_flt_cnt]);
   rng_flt_cnt++;

   filter_clas.filter_fields[0].field_id = WP_PCE_FIELD_ID_IPV4_TOS;
   filter_clas.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_RANGE_LOW;
   filter_clas.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   filter_clas.filter_fields[1].field_id = WP_PCE_FIELD_ID_IPV4_TOS;
   filter_clas.filter_fields[1].field_mode = WP_PCE_FIELD_MODE_COMPARE_RANGE_HIGH;
   filter_clas.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   WPE_TEST_CHECK_HANDLE(tmp_rng_filters[rng_flt_cnt],
      WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_CLASSIFICATION, &filter_clas));
   MY_PRINTF("\n---- tmp_rng_filters[%d]: 0x%X ----------\n", rng_flt_cnt, (WP_U32) tmp_rng_filters[rng_flt_cnt]);
   rng_flt_cnt++;


   filter_clas.filter_fields[0].field_id = WP_PCE_FIELD_ID_IPV6_TRAFFIC_CLASS;
   filter_clas.filter_fields[1].field_id = WP_PCE_FIELD_ID_IPV6_TRAFFIC_CLASS;
   WPE_TEST_CHECK_HANDLE(tmp_rng_filters[rng_flt_cnt],
      WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_CLASSIFICATION, &filter_clas));
   MY_PRINTF("\n---- tmp_rng_filters[%d]: 0x%X ----------\n", rng_flt_cnt, (WP_U32) tmp_rng_filters[rng_flt_cnt]);
   rng_flt_cnt++;


   filter_clas.filter_fields[0].field_id = WP_PCE_FIELD_ID_IPV6_FLOW_LABEL;
   filter_clas.filter_fields[1].field_id = WP_PCE_FIELD_ID_IPV6_FLOW_LABEL;
   WPE_TEST_CHECK_RESULT(tmp_rng_filters[rng_flt_cnt],
                     WPI_BASIC_ERROR(WP_ERR_PCE_FILTER_RANGE_SIZE_EXCEEDED),
                     WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_CLASSIFICATION, &filter_clas));

   filter_clas.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_USED;
   filter_clas.filter_fields[0].mask.ipv6_flow_label = 0x3FFC;
   filter_clas.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_USED;
   filter_clas.filter_fields[1].mask.ipv6_flow_label = 0xFFF;
   WPE_TEST_CHECK_HANDLE(tmp_rng_filters[rng_flt_cnt],
      WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_CLASSIFICATION, &filter_clas));
   MY_PRINTF("\n---- tmp_rng_filters[%d]: 0x%X ----------\n", rng_flt_cnt, (WP_U32) tmp_rng_filters[rng_flt_cnt]);
   rng_flt_cnt++;


   filter_clas.filter_fields[0].field_id = WP_PCE_FIELD_ID_MAC_DA;
   filter_clas.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;
   filter_clas.filter_fields[1].field_id = WP_PCE_FIELD_ID_MAC_DA;
   filter_clas.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;
   WPE_TEST_CHECK_RESULT(tmp_rng_filters[rng_flt_cnt],
                     WPI_BASIC_ERROR(WP_ERR_PCE_FILTER_RANGE_SIZE_EXCEEDED),
                     WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_CLASSIFICATION, &filter_clas));

   filter_clas.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_USED;
   for(j=0; j<6; j++)
      filter_clas.filter_fields[0].mask.mac_addr[j] = mac_mask4[j];
   filter_clas.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_USED;
   for(j=0; j<6; j++)
      filter_clas.filter_fields[1].mask.mac_addr[j] = mac_mask4[j];
   WPE_TEST_CHECK_RESULT(tmp_rng_filters[rng_flt_cnt],
                     WPI_BASIC_ERROR(WP_ERR_PCE_FILTER_RANGE_SIZE_EXCEEDED),
                     WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_CLASSIFICATION, &filter_clas));

   for(j=0; j<6; j++)
      filter_clas.filter_fields[0].mask.mac_addr[j] = mac_mask1[j];
   for(j=0; j<6; j++)
      filter_clas.filter_fields[1].mask.mac_addr[j] = mac_mask1[j];
   WPE_TEST_CHECK_HANDLE(tmp_rng_filters[rng_flt_cnt],
      WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_CLASSIFICATION, &filter_clas));
   MY_PRINTF("\n---- tmp_rng_filters[%d]: 0x%X ----------\n", rng_flt_cnt, (WP_U32) tmp_rng_filters[rng_flt_cnt]);
   rng_flt_cnt++;

   for(j=0; j<6; j++)
      filter_clas.filter_fields[0].mask.mac_addr[j] = mac_mask2[j];
   for(j=0; j<6; j++)
      filter_clas.filter_fields[1].mask.mac_addr[j] = mac_mask2[j];
   WPE_TEST_CHECK_HANDLE(tmp_rng_filters[rng_flt_cnt],
      WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_CLASSIFICATION, &filter_clas));
   MY_PRINTF("\n---- tmp_rng_filters[%d]: 0x%X ----------\n", rng_flt_cnt, (WP_U32) tmp_rng_filters[rng_flt_cnt]);
   rng_flt_cnt++;

   for(j=0; j<6; j++)
      filter_clas.filter_fields[0].mask.mac_addr[j] = mac_mask3[j];
   for(j=0; j<6; j++)
      filter_clas.filter_fields[1].mask.mac_addr[j] = mac_mask3[j];
   WPE_TEST_CHECK_HANDLE(tmp_rng_filters[rng_flt_cnt],
      WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_CLASSIFICATION, &filter_clas));
   MY_PRINTF("\n---- tmp_rng_filters[%d]: 0x%X ----------\n", rng_flt_cnt, (WP_U32) tmp_rng_filters[rng_flt_cnt]);
   rng_flt_cnt++;


   filter_clas.filter_fields[0].field_id = WP_PCE_FIELD_ID_IPV6_FLOW_LABEL;
   filter_clas.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_HIGHER_THAN;
   filter_clas.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   filter_clas.filter_fields[1].field_id = WP_PCE_FIELD_ID_VLAN_TYPE;
   filter_clas.filter_fields[1].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_clas.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   WPE_TEST_CHECK_HANDLE(tmp_rng_filters[rng_flt_cnt],
      WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_CLASSIFICATION, &filter_clas));
   MY_PRINTF("\n---- tmp_rng_filters[%d]: 0x%X ----------\n", rng_flt_cnt, (WP_U32) tmp_rng_filters[rng_flt_cnt]);
   rng_flt_cnt++;


   filter_clas.filter_fields[0].field_id = WP_PCE_FIELD_ID_MAC_DA;
   filter_clas.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_USED;
   for(j=0; j<6; j++)
      filter_clas.filter_fields[0].mask.mac_addr[j] = mac_mask5[j];

   WPE_TEST_CHECK_HANDLE(tmp_rng_filters[rng_flt_cnt],
      WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_CLASSIFICATION, &filter_clas));
   MY_PRINTF("\n---- tmp_rng_filters[%d]: 0x%X ----------\n", rng_flt_cnt, (WP_U32) tmp_rng_filters[rng_flt_cnt]);
   rng_flt_cnt++;


   filter_clas.filter_fields[0].field_id = WP_PCE_FIELD_ID_MAC_DA;
   filter_clas.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_USED;
   for(j=0; j<6; j++)
      filter_clas.filter_fields[0].mask.mac_addr[j] = mac_mask1[j];

   WPE_TEST_CHECK_HANDLE(tmp_rng_filters[rng_flt_cnt],
      WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_CLASSIFICATION, &filter_clas));
   MY_PRINTF("\n---- tmp_rng_filters[%d]: 0x%X ----------\n", rng_flt_cnt, (WP_U32) tmp_rng_filters[rng_flt_cnt]);
   rng_flt_cnt++;

   MY_PRINTF("\n------------- Rule %d Creating ----------\n", rng_rule_cnt);
   rule_cfg.filter_handle = tmp_rng_filters[rng_rule_cnt];
   rule_cfg.enabled = WP_ENABLE;
   rule_cfg.match_action = WP_PCE_RULE_MATCH_CONTINUE;
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_LAST;

   rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_INT_VLAN_TAG;
   rule_cfg.rule_fields[0].value.vlan_tag = 0x1234;

   rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_VLAN_TYPE;
   rule_cfg.rule_fields[1].value.vlan_type = WP_VLAN_TYPE_C_VLAN_TAGGED;

   rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_MAC_SA;
   for(i=0; i<6; i++)
      rule_cfg.rule_fields[2].value.mac_addr[i] = mac_addr1[i];
   rule_cfg.rule_fields[3].field_id = WP_PCE_FIELD_ID_LAST;

   WPE_TEST_CHECK_HANDLE(rule_handles[rng_rule_cnt],
      WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));
   MY_PRINTF("rule_handle[%d] = %X\n", rng_rule_cnt, rule_handles[rng_rule_cnt]);
   rng_rule_cnt++;

   MY_PRINTF("\n------------- Rule %d Creating ----------\n", rng_rule_cnt);
   rule_cfg.filter_handle = tmp_rng_filters[rng_rule_cnt];
   rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_IPV4_TOS;
   rule_cfg.rule_fields[0].value.ipv4_tos = 0x12;
   rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_IPV4_TOS;
   rule_cfg.rule_fields[1].value.ipv4_tos = 0x54;

   WPE_TEST_CHECK_HANDLE(rule_handles[rng_rule_cnt],
      WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));
   MY_PRINTF("rule_handle[%d] = %X\n", rng_rule_cnt, rule_handles[rng_rule_cnt]);
   rng_rule_cnt++;

   MY_PRINTF("\n------------- Rule %d Creating ----------\n", rng_rule_cnt);
   rule_cfg.filter_handle = tmp_rng_filters[rng_rule_cnt];
   rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_IPV6_TRAFFIC_CLASS;
   rule_cfg.rule_fields[0].value.ipv6_traffic_class = 0x31;
   rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_IPV6_TRAFFIC_CLASS;
   rule_cfg.rule_fields[1].value.ipv6_traffic_class = 0xF7;

   WPE_TEST_CHECK_HANDLE(rule_handles[rng_rule_cnt],
      WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));
   MY_PRINTF("rule_handle[%d] = %X\n", rng_rule_cnt, rule_handles[rng_rule_cnt]);
   rng_rule_cnt++;

   MY_PRINTF("\n------------- Rule %d Creating ----------\n", rng_rule_cnt);
   rule_cfg.filter_handle = tmp_rng_filters[rng_rule_cnt];
   rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_IPV6_FLOW_LABEL;
   rule_cfg.rule_fields[0].value.ipv6_flow_label = 0x123456;
   rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_IPV6_FLOW_LABEL;
   rule_cfg.rule_fields[1].value.ipv6_flow_label = 0xABCDEF;

   WPE_TEST_CHECK_HANDLE(rule_handles[rng_rule_cnt],
      WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));
   MY_PRINTF("rule_handle[%d] = %X\n", rng_rule_cnt, rule_handles[rng_rule_cnt]);
   rng_rule_cnt++;

   MY_PRINTF("\n------------- Rule %d Creating ----------\n", rng_rule_cnt);
   rule_cfg.filter_handle = tmp_rng_filters[rng_rule_cnt];
   rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_MAC_DA;
   for(i=0; i<6; i++)
      rule_cfg.rule_fields[0].value.mac_addr[i] = mac_addr1[i];
   rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_MAC_DA;
   for(i=0; i<6; i++)
      rule_cfg.rule_fields[1].value.mac_addr[i] = mac_addr3[i];
   rule_cfg.rule_fields[1].value.ipv6_flow_label = 0xABCDEF;

   WPE_TEST_CHECK_HANDLE(rule_handles[rng_rule_cnt],
      WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));
   MY_PRINTF("rule_handle[%d] = %X\n", rng_rule_cnt, rule_handles[rng_rule_cnt]);
   rng_rule_cnt++;

   MY_PRINTF("\n------------- Rule %d Creating ----------\n", rng_rule_cnt);
   rule_cfg.filter_handle = tmp_rng_filters[rng_rule_cnt];
   rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_MAC_DA;
   for(i=0; i<6; i++)
      rule_cfg.rule_fields[0].value.mac_addr[i] = mac_addr1[i];
   rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_MAC_DA;
   for(i=0; i<6; i++)
      rule_cfg.rule_fields[1].value.mac_addr[i] = mac_addr2[i];
   rule_cfg.rule_fields[1].value.ipv6_flow_label = 0xABCDEF;

   WPE_TEST_CHECK_HANDLE(rule_handles[rng_rule_cnt],
      WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));
   MY_PRINTF("rule_handle[%d] = %X\n", rng_rule_cnt, rule_handles[rng_rule_cnt]);
   rng_rule_cnt++;

   MY_PRINTF("\n------------- Rule %d Creating ----------\n", rng_rule_cnt);
   rule_cfg.filter_handle = tmp_rng_filters[rng_rule_cnt];
   rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_MAC_DA;
   for(i=0; i<6; i++)
      rule_cfg.rule_fields[0].value.mac_addr[i] = mac_addr3[i];
   rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_MAC_DA;
   for(i=0; i<6; i++)
      rule_cfg.rule_fields[1].value.mac_addr[i] = mac_addr4[i];
   rule_cfg.rule_fields[1].value.ipv6_flow_label = 0xABCDEF;

   WPE_TEST_CHECK_HANDLE(rule_handles[rng_rule_cnt],
      WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));
   MY_PRINTF("rule_handle[%d] = %X\n", rng_rule_cnt, rule_handles[rng_rule_cnt]);
   rng_rule_cnt++;

   MY_PRINTF("\n------------- Rule %d Creating ----------\n", rng_rule_cnt);
   rule_cfg.filter_handle = tmp_rng_filters[rng_rule_cnt];
   rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_IPV6_FLOW_LABEL;
   rule_cfg.rule_fields[0].value.ipv6_flow_label = 0x123456;
   rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_VLAN_TYPE;
   rule_cfg.rule_fields[1].value.vlan_type = WP_VLAN_TYPE_PRIORITY_TAGGED;

   WPE_TEST_CHECK_HANDLE(rule_handles[rng_rule_cnt],
      WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));
   MY_PRINTF("rule_handle[%d] = %X\n", rng_rule_cnt, rule_handles[rng_rule_cnt]);
   rng_rule_cnt++;

   MY_PRINTF("\n------------- Rule %d Creating ----------\n", rng_rule_cnt);
   rule_cfg.filter_handle = tmp_rng_filters[rng_rule_cnt];
   rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_MAC_DA;
   for(i=0; i<6; i++)
      rule_cfg.rule_fields[0].value.mac_addr[i] = mac_addr3[i];
   rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_VLAN_TYPE;
   rule_cfg.rule_fields[1].value.vlan_type = WP_VLAN_TYPE_PRIORITY_TAGGED;

   WPE_TEST_CHECK_HANDLE(rule_handles[rng_rule_cnt],
      WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));
   MY_PRINTF("rule_handle[%d] = %X\n", rng_rule_cnt, rule_handles[rng_rule_cnt]);
   rng_rule_cnt++;

   MY_PRINTF("\n------------- Rule %d Creating ----------\n", rng_rule_cnt);
   rule_cfg.filter_handle = tmp_rng_filters[rng_rule_cnt];
   rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_MAC_DA;
   for(i=0; i<6; i++)
      rule_cfg.rule_fields[0].value.mac_addr[i] = mac_addr2[i];
   rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_VLAN_TYPE;
   rule_cfg.rule_fields[1].value.vlan_type = WP_VLAN_TYPE_PRIORITY_TAGGED;

   WPE_TEST_CHECK_HANDLE(rule_handles[rng_rule_cnt],
      WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));
   MY_PRINTF("rule_handle[%d] = %X\n", rng_rule_cnt, rule_handles[rng_rule_cnt]);
   rng_rule_cnt++;
}

static void WPE_PceRangesTest(void)
{
   WP_handle rule_handle;
   WP_pce_filter_classification filter_clas = {0};
   WP_pce_rule_classification rule_cfg = {0};
   WP_ipv4_addr ipv4_addr;
   WP_U8 mac_addr[] = {0xCA, 0xFE, 0xBB, 0xCC, 0xDD, 0xEE};
   WP_l4_port l4_port1 = 0x1234;
   WP_l4_port l4_port2 = 0x5678;
   WP_U16 i;

   filter_clas.no_match_action = WP_PCE_FILTER_NO_MATCH_CONTINUE;
   filter_clas.no_fields_action = WP_PCE_FILTER_NO_FIELDS_CONTINUE;
   filter_clas.no_match_result[0].result_type = WP_PCE_RESULT_LAST;

/***************************************************************
 * Single Filter Test
 **************************************************************/


   filter_clas.filter_fields[0].field_id = WP_PCE_FIELD_ID_MAC_DA;
   filter_clas.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_clas.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   filter_clas.filter_fields[1].field_id = WP_PCE_FIELD_ID_L4_SRC_PORT;
   filter_clas.filter_fields[1].field_mode = WP_PCE_FIELD_MODE_COMPARE_RANGE_LOW;
   filter_clas.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   filter_clas.filter_fields[2].field_id = WP_PCE_FIELD_ID_L4_SRC_PORT;
   filter_clas.filter_fields[2].field_mode = WP_PCE_FIELD_MODE_COMPARE_RANGE_HIGH;
   filter_clas.filter_fields[2].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   filter_clas.filter_fields[3].field_id = WP_PCE_FIELD_ID_LAST;

   filter_clas.filter_fields[2].field_mode = WP_PCE_FIELD_MODE_COMPARE_RANGE_LOW;
   WPE_TEST_CHECK_RESULT(rng_filter1,
                     WPI_BASIC_ERROR(WP_ERR_PCE_FILTER_RANGE_FIELDS_ILLEGAL_ORDER),
                     WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_CLASSIFICATION, &filter_clas));
   filter_clas.filter_fields[2].field_mode = WP_PCE_FIELD_MODE_COMPARE_RANGE_HIGH;

   filter_clas.filter_fields[2].field_id = WP_PCE_FIELD_ID_L4_DST_PORT;
   WPE_TEST_CHECK_RESULT(rng_filter1,
                     WPI_BASIC_ERROR(WP_ERR_PCE_FILTER_RANGE_FIELDS_INCOMPATIBLE),
                     WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_CLASSIFICATION, &filter_clas));
   filter_clas.filter_fields[2].field_id = WP_PCE_FIELD_ID_L4_SRC_PORT;

   filter_clas.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_RANGE_LOW;
   WPE_TEST_CHECK_RESULT(rng_filter1,
                     WPI_BASIC_ERROR(WP_ERR_PCE_FILTER_RANGE_FIELDS_ILLEGAL_ORDER),
                     WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_CLASSIFICATION, &filter_clas));
   filter_clas.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;

   filter_clas.filter_fields[0].field_id = WP_PCE_FIELD_ID_L4_SRC_PORT;
   filter_clas.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_LOWER_THAN;
   WPE_TEST_CHECK_RESULT(rng_filter1,
                     WPI_BASIC_ERROR(WP_ERR_PCE_FILTER_RANGES_ON_MULTIPLE_FIELDS),
                     WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_CLASSIFICATION, &filter_clas));
   filter_clas.filter_fields[0].field_id = WP_PCE_FIELD_ID_MAC_DA;
   filter_clas.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;

   filter_clas.filter_fields[1].field_id = WP_PCE_FIELD_ID_IPV4_DA;
   filter_clas.filter_fields[1].field_mode = WP_PCE_FIELD_MODE_COMPARE_RANGE_LOW;
   filter_clas.filter_fields[2].field_id = WP_PCE_FIELD_ID_IPV4_DA;
   filter_clas.filter_fields[2].field_mode = WP_PCE_FIELD_MODE_COMPARE_RANGE_HIGH;
   WPE_TEST_CHECK_RESULT(rng_filter1,
                     WPI_BASIC_ERROR(WP_ERR_PCE_FILTER_RANGE_SIZE_EXCEEDED),
                     WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_CLASSIFICATION, &filter_clas));
   filter_clas.filter_fields[1].field_id = WP_PCE_FIELD_ID_L4_SRC_PORT;
   filter_clas.filter_fields[1].field_mode = WP_PCE_FIELD_MODE_COMPARE_RANGE_LOW;
   filter_clas.filter_fields[2].field_id = WP_PCE_FIELD_ID_L4_SRC_PORT;
   filter_clas.filter_fields[2].field_mode = WP_PCE_FIELD_MODE_COMPARE_RANGE_HIGH;

   filter_clas.filter_fields[1].field_id = WP_PCE_FIELD_ID_IPV6_DA_LOW;
   filter_clas.filter_fields[1].field_mode = WP_PCE_FIELD_MODE_COMPARE_RANGE_LOW;
   filter_clas.filter_fields[2].field_id = WP_PCE_FIELD_ID_IPV6_DA_LOW;
   filter_clas.filter_fields[2].field_mode = WP_PCE_FIELD_MODE_COMPARE_RANGE_HIGH;
   WPE_TEST_CHECK_RESULT(rng_filter1,
                     WPI_BASIC_ERROR(WP_ERR_PCE_FILTER_RANGE_SIZE_EXCEEDED),
                     WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_CLASSIFICATION, &filter_clas));
   filter_clas.filter_fields[1].field_id = WP_PCE_FIELD_ID_L4_SRC_PORT;
   filter_clas.filter_fields[1].field_mode = WP_PCE_FIELD_MODE_COMPARE_RANGE_LOW;
   filter_clas.filter_fields[2].field_id = WP_PCE_FIELD_ID_L4_SRC_PORT;
   filter_clas.filter_fields[2].field_mode = WP_PCE_FIELD_MODE_COMPARE_RANGE_HIGH;

   filter_clas.filter_fields[1].field_id = WP_PCE_FIELD_ID_IPV6_SA_HIGH;
   filter_clas.filter_fields[1].field_mode = WP_PCE_FIELD_MODE_COMPARE_HIGHER_THAN;
   WPE_TEST_CHECK_RESULT(rng_filter1,
                     WPI_BASIC_ERROR(WP_ERR_PCE_FILTER_RANGE_SIZE_EXCEEDED),
                     WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_CLASSIFICATION, &filter_clas));
   filter_clas.filter_fields[1].field_id = WP_PCE_FIELD_ID_L4_SRC_PORT;
   filter_clas.filter_fields[1].field_mode = WP_PCE_FIELD_MODE_COMPARE_RANGE_LOW;

   filter_clas.filter_fields[1].field_id = WP_PCE_FIELD_ID_IPV6_SA_LOW;
   filter_clas.filter_fields[1].field_mode = WP_PCE_FIELD_MODE_COMPARE_LOWER_THAN;
   WPE_TEST_CHECK_RESULT(rng_filter1,
                     WPI_BASIC_ERROR(WP_ERR_PCE_FILTER_RANGE_SIZE_EXCEEDED),
                     WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_CLASSIFICATION, &filter_clas));
   filter_clas.filter_fields[1].field_id = WP_PCE_FIELD_ID_L4_SRC_PORT;
   filter_clas.filter_fields[1].field_mode = WP_PCE_FIELD_MODE_COMPARE_RANGE_LOW;

   WPE_TEST_CHECK_HANDLE(rng_filter1,
      WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_CLASSIFICATION, &filter_clas));
   MY_PRINTF("\n---- rng_filter1: 0x%X ----------\n", (WP_U32) rng_filter1);



   filter_clas.filter_fields[1].field_id = WP_PCE_FIELD_ID_L4_DST_PORT;
   filter_clas.filter_fields[1].field_mode = WP_PCE_FIELD_MODE_COMPARE_HIGHER_THAN;
   filter_clas.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   filter_clas.filter_fields[2].field_id = WP_PCE_FIELD_ID_LAST;
   filter_clas.filter_fields[2].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_clas.filter_fields[2].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   WPE_TEST_CHECK_HANDLE(rng_filter2,
      WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_CLASSIFICATION, &filter_clas));
   MY_PRINTF("\n---- rng_filter2: 0x%X ----------\n", (WP_U32) rng_filter2);



   filter_clas.filter_fields[1].field_id = WP_PCE_FIELD_ID_IPV4_SA;
   filter_clas.filter_fields[1].field_mode = WP_PCE_FIELD_MODE_COMPARE_LOWER_THAN;
   filter_clas.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;
   WPE_TEST_CHECK_HANDLE(rng_filter3,
      WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_CLASSIFICATION, &filter_clas));
   MY_PRINTF("\n---- rng_filter3: 0x%X ----------\n", (WP_U32) rng_filter3);

/***************************************************************
 * Extended Filter Test
 **************************************************************/


   filter_clas.filter_fields[2].field_id = WP_PCE_FIELD_ID_MAC_SA;
   filter_clas.filter_fields[2].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_clas.filter_fields[2].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   filter_clas.filter_fields[3].field_id = WP_PCE_FIELD_ID_IPV4_DA;
   filter_clas.filter_fields[3].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_clas.filter_fields[3].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   filter_clas.filter_fields[4].field_id = WP_PCE_FIELD_ID_LAST;
   filter_clas.filter_fields[4].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_clas.filter_fields[4].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;
   WPE_TEST_CHECK_HANDLE(ext_rng_filter1,
      WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_CLASSIFICATION, &filter_clas));
   MY_PRINTF("\n---- ext_rng_filter1: 0x%X ----------\n", (WP_U32) ext_rng_filter1);


   /*********************************************************************************
   * Single Ranged rules test
   ********************************************************************************/
   ipv4_addr = 0x99999999;

   rule_cfg.filter_handle = rng_filter1;
   rule_cfg.enabled = WP_ENABLE;
   rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_MAC_DA;
   for(i=0; i<6; i++)
      rule_cfg.rule_fields[0].value.mac_addr[i] = mac_addr[i];

   rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_L4_SRC_PORT;
   rule_cfg.rule_fields[1].value.l4_port = l4_port1;

   rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_L4_SRC_PORT;
   rule_cfg.rule_fields[2].value.l4_port = l4_port2;

   rule_cfg.rule_fields[3].field_id = WP_PCE_FIELD_ID_LAST;
   rule_cfg.match_action = WP_PCE_RULE_MATCH_CONTINUE;
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_LAST;

   WPE_TEST_CHECK_HANDLE(rule_handles[0],
      WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));
   MY_PRINTF("rule_handle[0] = %X\n", rule_handles[0]);


   WPE_TEST_CHECK_RESULT(rule_handle,
                     WPI_BASIC_ERROR(WP_ERR_PCE_RULE_ALREADY_EXISTS),
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));


   WPE_TEST_CHECK_HANDLE(rule_handle,
                     WP_PceObjectSearch (WP_WINPATH(DEFAULT_WPID),
                                       WP_PCE_OBJECT_TYPE_RULE_CLASSIFICATION,
                                       &rule_cfg));
   MY_PRINTF("rule_handle = %X\n", rule_handle);


   rule_cfg.rule_fields[1].value.l4_port = l4_port1+1;
   WPE_TEST_CHECK_RESULT(rule_handle,
                     WPI_BASIC_ERROR(WP_ERR_PCE_OBJECT_NOT_FOUND),
                     WP_PceObjectSearch (WP_WINPATH(DEFAULT_WPID),
                                       WP_PCE_OBJECT_TYPE_RULE_CLASSIFICATION,
                                       &rule_cfg));
   rule_cfg.rule_fields[1].value.l4_port = l4_port1;


   rule_cfg.rule_fields[2].value.l4_port = l4_port2+1;
   WPE_TEST_CHECK_RESULT(rule_handle,
                     WPI_BASIC_ERROR(WP_ERR_PCE_OBJECT_NOT_FOUND),
                     WP_PceObjectSearch (WP_WINPATH(DEFAULT_WPID),
                                       WP_PCE_OBJECT_TYPE_RULE_CLASSIFICATION,
                                       &rule_cfg));
   rule_cfg.rule_fields[2].value.l4_port = l4_port2;

   rule_cfg.filter_handle = rng_filter2;
   rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_L4_DST_PORT;
   rule_cfg.rule_fields[1].value.l4_port = l4_port1;

   rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_LAST;


   WPE_TEST_CHECK_HANDLE(rule_handles[1],
      WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));
   MY_PRINTF("rule_handle[1] = %X\n", rule_handles[1]);

   rule_cfg.filter_handle = rng_filter3;
   rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_IPV4_SA;
   rule_cfg.rule_fields[1].value.ipv4_addr = ipv4_addr;

   WPE_TEST_CHECK_HANDLE(rule_handles[2],
      WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));
   MY_PRINTF("rule_handle[2] = %X\n", rule_handles[2]);

/*********************************************************************************
 * Extended Ranged rules test
 ********************************************************************************/
   rule_cfg.filter_handle = ext_rng_filter1;
   rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_MAC_SA;
   for(i=0; i<6; i++)
      rule_cfg.rule_fields[2].value.mac_addr[i] = mac_addr[i];

   rule_cfg.rule_fields[3].field_id = WP_PCE_FIELD_ID_IPV4_DA;
   rule_cfg.rule_fields[3].value.ipv4_addr = ipv4_addr;

   rule_cfg.rule_fields[4].field_id = WP_PCE_FIELD_ID_LAST;



   WPE_TEST_CHECK_HANDLE(rule_handles[3],
      WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));
   MY_PRINTF("rule_handle[3] = %X\n", rule_handles[3]);

   WPE_AddedRangesTest();
}

static void WPE_PceMasksTest(void)
{
   WP_handle rule_handle;
   WP_pce_filter_classification filter_clas = {0};
   WP_pce_rule_classification rule_cfg = {0};
   WP_U8 mac_addr[] = {0xCA, 0xFE, 0xBB, 0xCC, 0xDD, 0xEE};
   WP_l4_port l4_port1 = 0x1234;
   WP_l4_port l4_port_mask1 = 0xffff;
   WP_l4_port l4_port_mask2 = 0x0ffc;
   WP_l4_port l4_port_mask_illegal1 = 0xf00f;
   WP_l4_port l4_port_mask_illegal2 = 0xfefe;
   WP_l4_port l4_port_mask_illegal3 = 0xffee;
   WP_l4_port l4_port_mask_illegal4 = 0xfbfe;
   WP_l4_port l4_port_mask_illegal5 = 0xf7af;
   WP_l4_port l4_port_mask_illegal6 = 0x0;
   WP_ipv4_addr ipv4_addr = 0xFFFFFFFF;
   WP_ipv4_addr ipv4_mask1 = 0x007ffffe;
   WP_ipv4_addr ipv4_mask2 = 0x3ffff800;
   WP_U16 i;

   filter_clas.no_match_action = WP_PCE_FILTER_NO_MATCH_CONTINUE;
   filter_clas.no_fields_action = WP_PCE_FILTER_NO_FIELDS_CONTINUE;
   filter_clas.no_match_result[0].result_type = WP_PCE_RESULT_LAST;

/***************************************************************
 * Single Filter Test
 **************************************************************/


   filter_clas.filter_fields[0].field_id = WP_PCE_FIELD_ID_MAC_DA;
   filter_clas.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_clas.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   filter_clas.filter_fields[1].field_id = WP_PCE_FIELD_ID_L4_SRC_PORT;
   filter_clas.filter_fields[1].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_clas.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_USED;
   filter_clas.filter_fields[1].mask.l4_port = l4_port_mask1;

   filter_clas.filter_fields[2].field_id = WP_PCE_FIELD_ID_LAST;
   filter_clas.filter_fields[2].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_clas.filter_fields[2].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   filter_clas.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_USED;
   filter_clas.filter_fields[1].mask.l4_port = l4_port_mask_illegal1;
   WPE_TEST_CHECK_RESULT(msk_filter1,
                     WPI_BASIC_ERROR(WP_ERR_PCE_FILTER_NON_CONTINOUS_MASK),
                     WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_CLASSIFICATION, &filter_clas));

   filter_clas.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_USED;
   filter_clas.filter_fields[1].mask.l4_port = l4_port_mask_illegal2;
   WPE_TEST_CHECK_RESULT(msk_filter1,
                     WPI_BASIC_ERROR(WP_ERR_PCE_FILTER_NON_CONTINOUS_MASK),
                     WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_CLASSIFICATION, &filter_clas));

   filter_clas.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_USED;
   filter_clas.filter_fields[1].mask.l4_port = l4_port_mask_illegal3;
   WPE_TEST_CHECK_RESULT(msk_filter1,
                     WPI_BASIC_ERROR(WP_ERR_PCE_FILTER_NON_CONTINOUS_MASK),
                     WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_CLASSIFICATION, &filter_clas));

   filter_clas.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_USED;
   filter_clas.filter_fields[1].mask.l4_port = l4_port_mask_illegal4;
   WPE_TEST_CHECK_RESULT(msk_filter1,
                     WPI_BASIC_ERROR(WP_ERR_PCE_FILTER_NON_CONTINOUS_MASK),
                     WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_CLASSIFICATION, &filter_clas));

   filter_clas.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_USED;
   filter_clas.filter_fields[1].mask.l4_port = l4_port_mask_illegal5;
   WPE_TEST_CHECK_RESULT(msk_filter1,
                     WPI_BASIC_ERROR(WP_ERR_PCE_FILTER_NON_CONTINOUS_MASK),
                     WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_CLASSIFICATION, &filter_clas));

   filter_clas.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_USED;
   filter_clas.filter_fields[1].mask.l4_port = l4_port_mask_illegal6;
   WPE_TEST_CHECK_RESULT(msk_filter1,
                     WPI_BASIC_ERROR(WP_ERR_PCE_FILTER_ILLEGAL_MASK),
                     WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_CLASSIFICATION, &filter_clas));

   filter_clas.filter_fields[1].mask.ipv6_addr_half[0] = 0x81;
   filter_clas.filter_fields[1].field_id = WP_PCE_FIELD_ID_IPV6_SA_LOW;
   WPE_TEST_CHECK_RESULT(msk_filter1,
                     WPI_BASIC_ERROR(WP_ERR_PCE_FILTER_NON_CONTINOUS_MASK),
                     WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_CLASSIFICATION, &filter_clas));

   filter_clas.filter_fields[1].field_id = WP_PCE_FIELD_ID_L4_SRC_PORT;
   filter_clas.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_USED;
   filter_clas.filter_fields[1].mask.l4_port = l4_port_mask1;
   WPE_TEST_CHECK_HANDLE(msk_filter1,
      WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_CLASSIFICATION, &filter_clas));
   MY_PRINTF("\n---- msk_filter1: 0x%X ----------\n", (WP_U32) msk_filter1);



   filter_clas.filter_fields[1].field_id = WP_PCE_FIELD_ID_L4_DST_PORT;
   filter_clas.filter_fields[1].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_clas.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_USED;
   filter_clas.filter_fields[1].mask.l4_port = l4_port_mask2;
   WPE_TEST_CHECK_HANDLE(msk_filter2,
      WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_CLASSIFICATION, &filter_clas));
   MY_PRINTF("\n---- msk_filter2: 0x%X ----------\n", (WP_U32) msk_filter2);



   filter_clas.filter_fields[1].field_id = WP_PCE_FIELD_ID_IPV4_SA;
   filter_clas.filter_fields[1].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_clas.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_USED;
   filter_clas.filter_fields[1].mask.ipv4_addr = ipv4_mask1;
   WPE_TEST_CHECK_HANDLE(msk_filter3,
      WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_CLASSIFICATION, &filter_clas));
   MY_PRINTF("\n---- msk_filter3: 0x%X ----------\n", (WP_U32) msk_filter3);

/***************************************************************
 * Extended Filter Test
 **************************************************************/


   filter_clas.filter_fields[2].field_id = WP_PCE_FIELD_ID_MAC_SA;
   filter_clas.filter_fields[2].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_clas.filter_fields[2].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   filter_clas.filter_fields[3].field_id = WP_PCE_FIELD_ID_IPV4_DA;
   filter_clas.filter_fields[3].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_clas.filter_fields[3].mask_mode = WP_PCE_FIELD_MASK_USED;
   filter_clas.filter_fields[3].mask.ipv4_addr = ipv4_mask2;

   filter_clas.filter_fields[4].field_id = WP_PCE_FIELD_ID_LAST;
   filter_clas.filter_fields[4].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_clas.filter_fields[4].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;
   WPE_TEST_CHECK_HANDLE(ext_msk_filter1,
      WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_CLASSIFICATION, &filter_clas));
   MY_PRINTF("\n---- ext_msk_filter1: 0x%X ----------\n", (WP_U32) ext_msk_filter1);

/*********************************************************************************
 * Single Masked rules test
 ********************************************************************************/
   ipv4_addr = 0x99999999;

   rule_cfg.filter_handle = msk_filter1;
   rule_cfg.enabled = WP_ENABLE;
   rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_MAC_DA;
   for(i=0; i<6; i++)
      rule_cfg.rule_fields[0].value.mac_addr[i] = mac_addr[i];

   rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_L4_SRC_PORT;
   rule_cfg.rule_fields[1].value.l4_port = l4_port1;

   rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_LAST;

   rule_cfg.match_action = WP_PCE_RULE_MATCH_CONTINUE;
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_LAST;


   WPE_TEST_CHECK_HANDLE(rule_handles[4],
      WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));
   MY_PRINTF("rule_handle[4] = %X\n", rule_handles[4]);

   rule_cfg.filter_handle = msk_filter2;
   rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_L4_DST_PORT;
   rule_cfg.rule_fields[1].value.l4_port = l4_port1;

   WPE_TEST_CHECK_HANDLE(rule_handles[5],
      WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));
   MY_PRINTF("rule_handle[5] = %X\n", rule_handles[5]);

   rule_cfg.filter_handle = msk_filter3;
   rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_IPV4_SA;
   rule_cfg.rule_fields[1].value.ipv4_addr = ipv4_addr;

   WPE_TEST_CHECK_HANDLE(rule_handles[6],
      WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));
   MY_PRINTF("rule_handle[6] = %X\n", rule_handles[6]);


   WPE_TEST_CHECK_RESULT(rule_handle,
                     WPI_BASIC_ERROR(WP_ERR_PCE_RULE_ALREADY_EXISTS),
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));
   MY_PRINTF("rule_handle = %X\n", rule_handle);


   WPE_TEST_CHECK_HANDLE(rule_handle,
                     WP_PceObjectSearch (WP_WINPATH(DEFAULT_WPID),
                                       WP_PCE_OBJECT_TYPE_RULE_CLASSIFICATION,
                                       &rule_cfg));
   MY_PRINTF("rule_handle = %X\n", rule_handle);


   rule_cfg.rule_fields[1].value.ipv4_addr = ipv4_addr+1;
   WPE_TEST_CHECK_RESULT(rule_handle,
                     WPI_BASIC_ERROR(WP_ERR_PCE_OBJECT_NOT_FOUND),
                     WP_PceObjectSearch (WP_WINPATH(DEFAULT_WPID),
                                       WP_PCE_OBJECT_TYPE_RULE_CLASSIFICATION,
                                       &rule_cfg));
   rule_cfg.rule_fields[1].value.ipv4_addr = ipv4_addr;


/*********************************************************************************
 * Extended Masked rules test
 ********************************************************************************/
   rule_cfg.filter_handle = ext_msk_filter1;
   rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_MAC_SA;
   for(i=0; i<6; i++)
      rule_cfg.rule_fields[2].value.mac_addr[i] = mac_addr[i];

   rule_cfg.rule_fields[3].field_id = WP_PCE_FIELD_ID_IPV4_DA;
   rule_cfg.rule_fields[3].value.ipv4_addr = ipv4_addr;

   rule_cfg.rule_fields[4].field_id = WP_PCE_FIELD_ID_LAST;




   WPE_TEST_CHECK_HANDLE(rule_handles[7],
      WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));
   MY_PRINTF("rule_handle[7] = %X\n", rule_handles[7]);

   return;
}

static void WPE_PceIfTest(void)
{
   WP_pce_if_params_pkt_rx_channel pce_if_params = {0};
   WP_handle pce_if_handle;
   WP_status status;

   pce_if_params.mode = WP_PCE_IW_PORT_CONNECTION_ENABLED;
   pce_if_params.parser_start_type = WP_PCE_PARSER_START_TYPE_ETHERNET;
   pce_if_params.filter_set_handle = filter_set_lvl0_handle;
   pce_if_params.ip_header_validation = WP_DISABLE;



   pce_if_params.parser_start_type = WP_PCE_PARSER_START_TYPE_ETHERNET+100;
   WPE_TEST_CHECK_RESULT(pce_if_handle,
                  WPI_BASIC_ERROR(WP_ERR_PCE_PARSER_START_TYPE_NOT_SUPPORTED),
                  WP_PceInterfaceCreate(WP_PCE_IF_TYPE_PKT_RX_CHANNEL, &pce_if_params));
   pce_if_params.parser_start_type = WP_PCE_PARSER_START_TYPE_ETHERNET;

   pce_if_params.filter_set_handle = filter_set_lvl0_handle+100;
   WPE_TEST_CHECK_RESULT(pce_if_handle,
                  WPI_BASIC_ERROR(WP_ERR_HANDLE),
                  WP_PceInterfaceCreate(WP_PCE_IF_TYPE_PKT_RX_CHANNEL, &pce_if_params));
   pce_if_params.filter_set_handle = filter_set_lvl0_handle;

   pce_if_params.filter_set_handle = bport_iw1;
   WPE_TEST_CHECK_RESULT(pce_if_handle,
                  WPI_BASIC_ERROR(WP_ERR_HANDLE),
                  WP_PceInterfaceCreate(WP_PCE_IF_TYPE_PKT_RX_CHANNEL, &pce_if_params));
   pce_if_params.filter_set_handle = filter_set_lvl0_handle;

   WPE_TEST_CHECK_HANDLE(pce_if_handle,
                  WP_PceInterfaceCreate(WP_PCE_IF_TYPE_PKT_RX_CHANNEL, &pce_if_params));

   rx_binding_cfg->input_port = bport_iw1;
   rx_binding_cfg->pce_if_handle = pce_if_handle + 100;
   WPE_TEST_CHECK_RESULT(status,
                         WPI_BASIC_ERROR(WP_ERR_HANDLE),
                         WP_IwRxBindingCreate(rx_channel_enet_handle1,
                                              iw_system_handle,
                                              qniw_enet,
                                              rx_binding_cfg));

   rx_binding_cfg->input_port = bport_iw1;
   rx_binding_cfg->pce_if_handle = pce_if_handle + 50000;
   WPE_TEST_CHECK_RESULT(status,
                         WPI_BASIC_ERROR(WP_ERR_HANDLE),
                         WP_IwRxBindingCreate(rx_channel_enet_handle1,
                                              iw_system_handle,
                                              qniw_enet,
                                              rx_binding_cfg));
   rx_binding_cfg->input_port = bport_iw1;
   rx_binding_cfg->pce_if_handle = filter_set_lvl0_handle;
   WPE_TEST_CHECK_RESULT(status,
                         WPI_BASIC_ERROR(WP_ERR_HANDLE),
                         WP_IwRxBindingCreate(rx_channel_enet_handle1,
                                              iw_system_handle,
                                              qniw_enet,
                                              rx_binding_cfg));
   rx_binding_cfg->input_port = bport_iw1;
   rx_binding_cfg->pce_if_handle = pce_if_handle;
   WPE_TEST_CHECK_RESULT(status,
                         WP_OK,
                         WP_IwRxBindingCreate(rx_channel_enet_handle1,
                                              iw_system_handle,
                                              qniw_enet,
                                              rx_binding_cfg));

/*    WPE_TEST_CHECK_RESULT(status, */
/*                   WPI_BASIC_ERROR(WP_ERR_PCE_IF_CONNECT_INCOMPATIBLE_OBJECT), */
/*                   WP_PceInterfaceConnect(pce_if_handle, pce_if_handle)); */


}

static void WPE_PceStatisticsTest(void)
{
   WPE_PrintPceGlobalStats();
   WPE_PrintPceFilterStats(filter);
   WP_PceGlobalStatisticsReset(WP_WINPATH(DEFAULT_WPID), &pce_global_stats);
   WP_PceFilterStatisticsReset(filter, &(pce_filter_stats[filter & 0xff]));
   WPE_PrintPceGlobalStats();
   WPE_PrintPceFilterStats(filter);
}

void WPE_PceFiltersTest1(void)
{
   WP_U32 i;
   WP_pce_rule_classification rule_cfg = {0};
   WP_U8 mac_addr1[] = {0x02, 0x01, 0x01, 0x01, 0x01, 0x03};
   WP_pce_filter_classification filter_clas_cfg = {0};
   filter_clas_cfg.no_match_action = WP_PCE_FILTER_NO_MATCH_CONTINUE;
   filter_clas_cfg.no_fields_action = WP_PCE_FILTER_NO_FIELDS_CONTINUE;
   filter_clas_cfg.no_match_result[0].result_type  = WP_PCE_RESULT_IWCP_START_REDIRECT;
   filter_clas_cfg.no_match_result[1].result_type  = WP_PCE_RESULT_LAST;

   filter_clas_cfg.filter_fields[0].field_id = WP_PCE_FIELD_ID_MAC_SA;
   filter_clas_cfg.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_clas_cfg.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   filter_clas_cfg.filter_fields[1].field_id = WP_PCE_FIELD_ID_LAST;
   WPE_TEST_CHECK_HANDLE(filter,
                     WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_CLASSIFICATION, &filter_clas_cfg));

   rule_cfg.filter_handle = filter;
   rule_cfg.enabled = WP_ENABLE;
   rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_MAC_SA;
   for(i=0; i<6; i++)
      rule_cfg.rule_fields[0].value.mac_addr[i] = mac_addr1[i];

   rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_LAST;

   rule_cfg.match_action = WP_PCE_RULE_MATCH_ACCEPT;
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_IWCP_START_REDIRECT;
   rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;


   WPE_TEST_CHECK_HANDLE(rule_handles[0],
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));
   MY_PRINTF("rule_handle[0] = %X\n", rule_handles[0]);

}

static void WPE_Extended(void)
{
   WP_handle filter10, filter11;
   WP_handle rule10, rule11;
   WP_U32 i;
   WP_pce_filter_classification filter_clas_cfg = {0};
   WP_pce_rule_classification rule_cfg = {0};
   filter_clas_cfg.no_match_action = WP_PCE_FILTER_NO_MATCH_CONTINUE;
   filter_clas_cfg.no_fields_action = WP_PCE_FILTER_NO_FIELDS_CONTINUE;
   filter_clas_cfg.no_match_result[0].result_type  = WP_PCE_RESULT_LAST;

   for(i = 0; i<9; i++)
   {
      filter_clas_cfg.filter_fields[i].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
      filter_clas_cfg.filter_fields[i].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;
   }
   
   filter_clas_cfg.filter_fields[0].field_id = WP_PCE_FIELD_ID_MAC_SA;
   filter_clas_cfg.filter_fields[1].field_id = WP_PCE_FIELD_ID_MAC_DA;
   filter_clas_cfg.filter_fields[2].field_id = WP_PCE_FIELD_ID_IPV6_DA_HIGH;
   filter_clas_cfg.filter_fields[3].field_id = WP_PCE_FIELD_ID_IPV6_DA_LOW;
   filter_clas_cfg.filter_fields[4].field_id = WP_PCE_FIELD_ID_IPV6_SA_HIGH;
   filter_clas_cfg.filter_fields[5].field_id = WP_PCE_FIELD_ID_SF_REMARKED_USER_SPECIFIC;
   filter_clas_cfg.filter_fields[5].mask_mode = WP_PCE_FIELD_MASK_USED;
   filter_clas_cfg.filter_fields[5].mask.user_specific[0] = 0;
   filter_clas_cfg.filter_fields[5].mask.user_specific[1] = 0;
   filter_clas_cfg.filter_fields[5].mask.user_specific[2] = 0x07;
   filter_clas_cfg.filter_fields[5].mask.user_specific[3] = 0xFF;
   filter_clas_cfg.filter_fields[5].mask.user_specific[4] = 0xFF;
   filter_clas_cfg.filter_fields[5].mask.user_specific[5] = 0xF8;
   filter_clas_cfg.filter_fields[5].mask.user_specific[6] = 0;
   filter_clas_cfg.filter_fields[6].field_id = WP_PCE_FIELD_ID_LAST;
   WPE_TEST_CHECK_HANDLE(filter10,
                     WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_CLASSIFICATION, &filter_clas_cfg));

   filter_clas_cfg.filter_fields[0].field_id = WP_PCE_FIELD_ID_SF_REMARKED_USER_SPECIFIC;
   filter_clas_cfg.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_USED;
   filter_clas_cfg.filter_fields[0].mask.user_specific[0] = 0;
   filter_clas_cfg.filter_fields[0].mask.user_specific[1] = 0;
   filter_clas_cfg.filter_fields[0].mask.user_specific[2] = 0xFF;
   filter_clas_cfg.filter_fields[0].mask.user_specific[3] = 0xC0;
   filter_clas_cfg.filter_fields[0].mask.user_specific[4] = 0;
   filter_clas_cfg.filter_fields[0].mask.user_specific[5] = 0;
   filter_clas_cfg.filter_fields[0].mask.user_specific[6] = 0;
   filter_clas_cfg.filter_fields[1].field_id = WP_PCE_FIELD_ID_IPV4_DA;
   filter_clas_cfg.filter_fields[2].field_id = WP_PCE_FIELD_ID_IPV4_SA;
   filter_clas_cfg.filter_fields[3].field_id = WP_PCE_FIELD_ID_L4_DST_PORT;
   filter_clas_cfg.filter_fields[3].field_mode = WP_PCE_FIELD_MODE_COMPARE_RANGE_LOW;
   filter_clas_cfg.filter_fields[4].field_id = WP_PCE_FIELD_ID_L4_DST_PORT;
   filter_clas_cfg.filter_fields[4].field_mode = WP_PCE_FIELD_MODE_COMPARE_RANGE_HIGH;
   filter_clas_cfg.filter_fields[5].field_id = WP_PCE_FIELD_ID_LAST;
   WPE_TEST_CHECK_HANDLE(filter11,
                     WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_CLASSIFICATION, &filter_clas_cfg));

   rule_cfg.filter_handle = filter11;
   rule_cfg.enabled = WP_ENABLE;
   rule_cfg.match_action = WP_PCE_RULE_MATCH_CONTINUE;
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_LAST;

   rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_SF_REMARKED_USER_SPECIFIC;
   for(i=0; i<7; i++)
      rule_cfg.rule_fields[0].value.mac_addr[i] = 0xFF;
   rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_IPV4_DA;
   rule_cfg.rule_fields[1].value.ipv4_addr = 0xABCDEF01;
   rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_IPV4_SA;
   rule_cfg.rule_fields[2].value.ipv4_addr = 0x12345678;
   rule_cfg.rule_fields[3].field_id = WP_PCE_FIELD_ID_L4_DST_PORT;
   rule_cfg.rule_fields[3].value.l4_port = 0x1111;
   rule_cfg.rule_fields[4].field_id = WP_PCE_FIELD_ID_L4_DST_PORT;
   rule_cfg.rule_fields[4].value.l4_port = 0x9999;
   rule_cfg.rule_fields[5].field_id = WP_PCE_FIELD_ID_LAST;


   WPE_TEST_CHECK_HANDLE(rule11,
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));
   MY_PRINTF("rule_handle[0] = %X\n", rule11);

   rule_cfg.filter_handle = filter10;
   rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_MAC_SA;
   for(i=0; i<6; i++)
      rule_cfg.rule_fields[0].value.mac_addr[i] = 0x11;
   rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_MAC_DA;
   for(i=0; i<6; i++)
      rule_cfg.rule_fields[1].value.mac_addr[i] = 0x22;
   rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_IPV6_DA_HIGH;
   for(i=0; i<8; i++)
      rule_cfg.rule_fields[2].value.mac_addr[i] = 0x33;
   rule_cfg.rule_fields[3].field_id = WP_PCE_FIELD_ID_IPV6_DA_LOW;
   for(i=0; i<8; i++)
      rule_cfg.rule_fields[3].value.mac_addr[i] = 0x44;
   rule_cfg.rule_fields[4].field_id = WP_PCE_FIELD_ID_IPV6_SA_HIGH;
   for(i=0; i<8; i++)
      rule_cfg.rule_fields[4].value.mac_addr[i] = 0x55;
   rule_cfg.rule_fields[5].field_id = WP_PCE_FIELD_ID_SF_REMARKED_USER_SPECIFIC;
   for(i=0; i<7; i++)
      rule_cfg.rule_fields[5].value.mac_addr[i] = 0xFF;
   rule_cfg.rule_fields[6].field_id = WP_PCE_FIELD_ID_LAST;


   WPE_TEST_CHECK_HANDLE(rule10,
                     WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg));
   MY_PRINTF("rule_handle[0] = %X\n", rule10);

#ifdef DEBUG_PRINT
   WP_Display(WP_WINPATH(DEFAULT_WPID), filter10, 0, 0);
   WP_Display(WP_WINPATH(DEFAULT_WPID), filter11, 0, 0);
   WP_Display(WP_WINPATH(DEFAULT_WPID), rule10, 0, 0);
   WP_Display(WP_WINPATH(DEFAULT_WPID), rule11, 0, 0);
#endif

   printf("rule diff = %d\n", rule10 - rule11);
   
   WPE_TEST_CHECK_RESULT(status, WP_OK, WP_PceRuleDelete(rule10));
   WPE_TEST_CHECK_RESULT(status, WP_OK, WP_PceRuleDelete(rule11));
   WPE_TEST_CHECK_RESULT(status, WP_OK, WP_PceFilterDelete(filter10));
   WPE_TEST_CHECK_RESULT(status, WP_OK, WP_PceFilterDelete(filter11));
}

#define PRINTF printf("line %d\n", __LINE__);

static void WPE_PceClassificationUnitTest(void)
{
   WP_U16 rules_num = 20;

   allow_error_print = 0;

   //WPE_PceFiltersTest1();

   //exit (1);

   WPE_Extended();

   WPE_PceFiltersTest();

   WPE_PceRulesTest(rules_num, 100000);

   WPE_PceIteratorsTest(rules_num);

   WPE_PceRulesDeleteTest(rules_num);

   WPE_PceRangesTest();

   WPE_PceMasksTest();

   WPE_PceFilterSetsCreateTest();

   WPE_PceRemarkedFieldsTest();

   WPE_PceRuleResultsTest();

   WPE_PceNewFieldsTest();

   WPE_PceFilterSetsLevelsTest();

   WPE_PceIfTest();

   WPE_Extended();

   WPE_PceFilterSetsDeleteTest();

   WPE_PceStatisticsTest();

   WPE_PceDeleteFiltersTest();
   allow_error_print = 0;
}

/////////////////////////////////////////////////
//
// WTI_LogCallBack
//
/////////////////////////////////////////////////
#ifdef VERILOG
static WP_U8 logTraceLevel = WPI_DEBUG_LEVEL_10;
#else
#ifdef DEBUG_PRINT
static WP_U8 logTraceLevel = WPI_DEBUG_LEVEL_10;
#else
static WP_U8 logTraceLevel = WPI_DEBUG_LEVEL_0;
#endif
#endif

WP_status WTI_NcmFieldErrorLog(WP_S8 *file_name,  WP_S8 *function_name, WP_U32 line, WP_U32 value)
{
   WPI_LOG(WPI_DEBUG_LEVEL_0,
           (wpi_log, "\nERROR WPI_NCM_FIELD: value 0x%X exceeds the ranges! file: %s, function: %s, line %d\n\n",
            value, file_name, function_name, line));
   return WP_OK;
}

WP_status WTI_LogCallBack(WP_S32 level, const WP_CHAR *message)
{
   if(level <= logTraceLevel)
      printf("%s", message);
   return WP_OK;
}

WP_status WPE_DebugCallback(const WP_CHAR *file, const WP_CHAR *function,
                            WP_S32 line, WP_S32 error_id)
{
   if (error_id && allow_error_print)
      printf("\n\nfile = %s , line = %d\nfunction = %s\n",file,line,function);
   return WP_OK;
}

#include "include/core/hardware/wpi_hw_pce.h"
#include "include/core/pce/wpi_dps_pce.h"
#include "include/core/pce/wpi_pce_private.h"
#include "include/core/pce/wpi_pce_class.h"
#include "../../api/wp_errors.h"

//////////////////////////////////////////////////////////////////////////////
//
// Main Function
//
//////////////////////////////////////////////////////////////////////////////

/* main */
#ifdef BSP
WP_S32 Main(WP_S32 argc, WP_CHAR **argv)
#else
WP_S32 main(WP_S32 argc, WP_CHAR **argv)
#endif
{
   WP_status status;

#if _WT_MODULE_TEST_ENABLE_
   WT_Start(TESTSTRING(WDDI_TEST), NULL, __FILE__);
#endif

   WPE_init();

   pce_init.filters_num = 200;
   pce_init.hardware_rules_num = 500;
   status = WP_ModuleInit(WP_SYSHANDLE(DEFAULT_WPID), WP_WDDI_MODULE_PCE, &pce_init);
   WPE_TerminateOnError(status, "WPI_PceModuleInit()", __LINE__);

   /* Create policer module */
   status = WP_ModuleInit(WP_SYSHANDLE(DEFAULT_WPID), WP_WDDI_MODULE_POLICER, policer_module_config);
   WPE_TerminateOnError(status,"Policer Module Init", __LINE__);


#if WP_DEBUG_ENABLE_WDDI_LOG
   WP_ControlRegister(WP_DEBUG_CALLBACK_WDDI_LOG, WTI_LogCallBack);
#endif
#if WP_DEBUG_ENABLE_WDDI_ERROR_LOG
   WP_ControlRegister(WP_DEBUG_CALLBACK_FILE_LINE_ERROR, WPE_DebugCallback);
#endif
   WP_ControlRegister(WP_DEBUG_CALLBACK_WDDI_NCM_ERROR, WTI_NcmFieldErrorLog);
   WPE_Create_pools();
   WPE_Create_ports();

   status = WP_SysCommit();
   WPE_TerminateOnError(status, "WP_SysCommit()", __LINE__);

   WT_Identify();

   WPE_CreateIwSystemAndItsComponents();

   WPE_PceClassificationUnitTest();

   if (wpe_test_failures == 0)
      printf ("Errors Num = 0.\nTest Passed \n");
   else
      printf ("Errors Num = %d.\nTest Failed \n", wpe_test_failures);

   WPE_Terminate(0);

   return 0;
} /* main */

#include "wt_util.c"
#include "wt_pce_misc.c"
#include "wt_partition_ecc_util.c"


