/*****************************************************************************
 * (C) Copyright Wintegra 2001.  All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 *****************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "wp_wddi.h"

#include "wp_sim.h"

#define DEBUG_PRINT

#define DEBUG_PRINT_OUT

#ifdef DEBUG_PRINT
#define APP_PRINTF(x, ...) printf(x, ##__VA_ARGS__)
#else
#define APP_PRINTF(x, ...) 
#endif

#if 1
#ifdef USE_OC12_PHY
#include "wpx_oc12_pmc5357.h"
#else
#include "wpx_oc3_pmc5351.h"
#endif
#endif

#include "wpx_app_data.h"
#include "wpx_board_if.h"

#ifdef USE_SOCKET
#include "sockint.h"
#endif

#include "api/wp_pce.h"

extern WP_U32 dps_ProgramImage[];
extern WP_U16 dps_PC_Table_Init[];

extern WP_pce_parser_info pce_parser_info;

WP_int_queue_table int_queue_tables = {{{10, 1}, {10, 1}, {10, 1}, {10, 1}}};

#define POOL_LIMIT           10

#define ETH_RX_TAG1          1001
#define ETH_RX_TAG2          1002
#define ETH_RX_TAG3          1003
#define ETH_RX_TAG4          1004
#define ETH_TX_TAG1          1101
#define ETH_TX_TAG2          1102
#define ETH_TX_TAG3          1103
#define ETH_TX_TAG4          1104

#define IW_RX_DUMMY_CH_TAG   2000

#define HOST_TX_CH_TAG10     1120
#define HOST_TX_CH_TAG11     1121

#define DATA_LENGTH          (68 + 32)

#define MIN_CH_RATE          5000
#define MAX_CH_RATE          2048000

#define IW_SYS_MAX_FLOWS     8
#define LEARNING_QUEUE_SIZE  10
#define IW_SYS_MAX_BPORTS    16
#define IW_PORT_1            11
#define IW_PORT_2            12
#define IW_PORT_3            13
#define IW_PORT_4            14
#define IW_PORT_5            15
#define IW_PORT_6            16
#define IW_PORT_7            17


#define MAX_PACKET_SIZE      1984
#define IW_BUF_N_BUFFERS     20
#define QN_IW_N_BUFFERS      50

#define PCE_MAX_FILTERS       40
#define PCE_MAX_RULES         PCE_MAX_FILTERS * 3
#define PCE_LVL_0_MAX_SETS    5
#define PCE_LVL_1_MAX_SETS    2
#define PCE_LVL_2_MAX_SETS    3
#define MAX_VLAN_ID           3
WP_CHAR appname[] = "====WP3 FH DEMO===";

static WP_U32 allow_error_print = 1;

typedef enum
{
PCE_FILTER_DMAC = 1,
PCE_FILTER_DSAC,
PCE_FILTER_VLAN
}
PCE_FILTER_LIST;


#include "wt_pce_misc.h"
#include "wt_pce_misc_structs.c"
#include "../wddi/sources/api/wp_prototypes.h"
WP_iw_agg_bridging tx_agg_enet[1] =
{
   {
   /*tag*/ 2,
   /*txfunc*/ 0,
   /*input_bport*/0,
   /*rfcs*/WP_IW_RFCS_ENABLE,
   /*l2_header_insert_mode;*/WP_IW_L2H_INSERT_DISABLE,
   /*vlan_tag_mode*/WP_IW_VLAN_TAG_DISABLE,
   /*interruptqueue;*/WP_IW_IRQT1,
   /*error_pkt_mode*/WP_IW_ERRPKT_DISCARD,
   /*intmode;*/WP_IW_INT_ENABLE,
   /*statmode;*/WP_IW_STAT_ENABLE,
   /*timestamp_mode;*/WP_IW_TIME_STAMP_DISABLE,
   /*ov_pool_mode */ WP_IW_OV_POOL_DISABLE,
   /*fbp_drop_threshold;*/0,
   /*replace_vlan_tag*/WP_IW_REPLACE_VTAG_DISABLE,
   /*vlan_tag*/0,
   /*vpmt_handle */0,
   /*mtu;*/1536,
   /*prefix_length */ 0,
   /*prefix_header[28];*/ {0,0,0,0,0,0,0,
                           0,0,0,0,0,0,0,
                           0,0,0,0,0,0,0,
                           0,0,0,0,0,0,0},
   /*policer_enable*/ WP_IW_POLICER_DISABLE,
   /**policer_config;*/NULL,
   /*cong_mode;*/WP_IW_CONGESTION_DISABLE,
   /**cong_threshold_param;*/NULL
   }
};
WP_U32 iii = 0;

WP_status status;

WP_handle iw_pool1, iw_pool2, iw_pool3, iw_pool4, iw_pool5;
WP_handle host_pool, iw_pool, iw_pool5;
WP_handle qnh, qniw, qniw_enet, qniw_enet4, qniw_enet5;

/* Port handles */
WP_handle enetp1, enetp2, enetp3, enetp4, iw_port;
WP_handle host_port10, host_port11;

/* Device handles */
WP_handle enetdev1, enetdev2, enetdev3, enetdev4, iw_dev;
WP_handle host_device10, host_device11;

WP_handle iwrx_handle, iwtx_handle;

/* Rx and Tx channel handles */
WP_handle rx_channel_enet_handle1, rx_channel_enet_handle2;
WP_handle tx_channel_enet_handle1, tx_channel_enet_handle2;

WP_handle iw_rx_ch;
WP_handle rx_host_handle10, tx_host_handle11, tx_host_bp_handle, tx_host_rp_handle;
WP_handle  iw_system_bridging_handle;
WP_handle gcis_filter, gcis_flow;

/* IW bridge port handles */
WP_handle bport_iw1, bport_iw2, bport_iw3;
WP_handle rport_iw1, rport_iw2, rport_iw3, rport_iw7;
WP_handle bport_iw_host, rport_iw_host;

/*policer info*/
WP_handle policer_action_handle[1];
WP_handle policer_handle_ingress[1];

/* Flow aggregation handles */
WP_handle flow_aggregation1, flow_aggregation2, flow_aggregation3, flow_aggregation4, flow_aggregation7, flow_aggregation_gcsi;

WP_U16 wpe_test_failures = 0;

WP_U32 loopback = WP_TRUE;

/* WPE_init */
static void WPE_init(void)
{
   WP_status status;

   status = WP_DriverInit();
   terminate_on_error(status,"WP_DriverInit");

   status = WP_SysInit(WP_WINPATH(0), context);
   terminate_on_error(status,"WP_SysInit");
}

/* WPE_Create_pools */
static void WPE_Create_pools(void)
{
   /* Create the Buffer pool(s) - interworking*/
   iw_pool1 = WP_PoolCreate(WP_WINPATH(0), WP_pool_iwbuffer, iw_buffer_data);
   terminate_on_error(iw_pool1, "WP_PoolCreate");

   iw_buffer_data->n_buffers = 100;
   iw_pool4 = WP_PoolCreate(WP_WINPATH(0), WP_pool_iwbuffer, iw_buffer_data);
   terminate_on_error(iw_pool4, "WP_PoolCreate 4");

   iw_pool5 = WP_PoolCreate(WP_WINPATH(0), WP_pool_iwbuffer, iw_buffer_data);
   terminate_on_error(iw_pool5, "WP_PoolCreate 5");

   /* Create an interworking queue node */   
   qn_iw->adjunct_pool = iw_pool1;

   qniw_enet = WP_QNodeCreate(WP_WINPATH(0), WP_QNODE_IWQ | WP_QNODE_OPT_FMU, qn_iw);
   terminate_on_error(qniw_enet, "WP_QNodeCreate IW_ENETQ");
   qn_iw->adjunct_pool = iw_pool4;
   qniw_enet4 = WP_QNodeCreate(WP_WINPATH(0), WP_QNODE_IWQ | WP_QNODE_OPT_FMU, qn_iw);
   terminate_on_error(qniw_enet4, "WP_QNodeCreate IW_ENETQ 4");

   qn_iw->adjunct_pool = iw_pool5;
   qniw_enet5 = WP_QNodeCreate(WP_WINPATH(0), WP_QNODE_IWQ | WP_QNODE_OPT_FMU, qn_iw);
   terminate_on_error(qniw_enet5, "WP_QNodeCreate IW_ENETQ 5");
}

/* WPE_Create_ports */
static void WPE_Create_ports(void)
{
   WP_status status;
   /* Create an Enet Ports and Devices */
   WP_CHAR ch;

   printf("\n\n*********Loopback mood *********");
   printf("\nL) Ports in loopback mode");
   printf("\nN) Ports in normal mode\n");

   ch = getchar();
   if(ch == 'n' || ch == 'N')
   {
      loopback = WP_FALSE;
      printf("\n\nPorts in normal mode");
   }
   else
      printf("\n\nPorts in loopback mode");
   

   status = WPX_BoardSerdesInit(0, WP_PORT_ENET7, loopback);
   terminate_on_error(status, "WPX_BoardSerdesInit(Enet1)");

   status = WPX_BoardSerdesInit(0, WP_PORT_ENET8, loopback);
   terminate_on_error(status, "WPX_BoardSerdesInit(Enet2)");      

   status = WPX_BoardSerdesInit(0, WP_PORT_ENET13, WP_TRUE);
   terminate_on_error(status, "WPX_BoardSerdesInit(Enet3)");      

   status = WPX_BoardSerdesInit(0, WP_PORT_ENET14, WP_TRUE);
   terminate_on_error(status, "WPX_BoardSerdesInit(Enet4)");      

   enetp1 = WP_PortCreate(WP_WINPATH(0), WP_PORT_ENET7, enet_s);
   terminate_on_error(enetp1, "WP_PortCreate Enet1");

   enetp2 = WP_PortCreate(WP_WINPATH(0), WP_PORT_ENET8, enet_s);
   terminate_on_error(enetp2, "WP_PortCreate Enet2");

   enetp3 = WP_PortCreate(WP_WINPATH(0), WP_PORT_ENET13, enet_s);
   terminate_on_error(enetp3, "WP_PortCreate Enet3");

   enetp4 = WP_PortCreate(WP_WINPATH(0), WP_PORT_ENET14, enet_s);
   terminate_on_error(enetp4, "WP_PortCreate Enet4");

   iw_port = WP_PortCreate(WP_WINPATH(0), WP_PORT_IW_HOST, NULL);
   terminate_on_error(iw_port, "PortCreate");

   host_port10 = WP_PortCreate(WP_WINPATH(0), WP_PORT_IW_HOST, NULL);
   terminate_on_error(host_port10, "Host Port 10 Create");

   host_port11 = WP_PortCreate(WP_WINPATH(0), WP_PORT_IW_HOST, NULL);
   terminate_on_error(host_port11, "Host Port 11 Create");

   enetdev1 = WP_DeviceCreate(enetp1, WP_PHY(0), WP_DEVICE_ENET, enet_dev);
   terminate_on_error(enetdev1, "WP_DeviceCreate Enet1");

   enetdev2 = WP_DeviceCreate(enetp2, WP_PHY(0), WP_DEVICE_ENET, enet_dev);
   terminate_on_error(enetdev2, "WP_DeviceCreate Enet2");

   enetdev3 = WP_DeviceCreate(enetp3, WP_PHY(0), WP_DEVICE_ENET, enet_dev);
   terminate_on_error(enetdev3, "WP_DeviceCreate Enet3");

   enetdev4 = WP_DeviceCreate(enetp4, WP_PHY(0), WP_DEVICE_ENET, enet_dev);
   terminate_on_error(enetdev4, "WP_DeviceCreate Enet4");

   enet_dev->extended_params = enet_dev_ex;
   status = WP_DeviceModify(enetdev1,
                            WP_DEV_MOD_ENET_PHYSTATMODE |
                            WP_DEV_MOD_ENET_TX_STATMODE |
                            WP_DEV_MOD_ENET_RX_STATMODE,
                            &enet_dev);
   terminate_on_error(status, "WP_DeviceModify() ENET 1");

   status = WP_DeviceModify(enetdev2,
                            WP_DEV_MOD_ENET_PHYSTATMODE |
                            WP_DEV_MOD_ENET_TX_STATMODE |
                            WP_DEV_MOD_ENET_RX_STATMODE,
                            &enet_dev);
   terminate_on_error(status, "WP_DeviceModify() ENET 2");

   status = WP_DeviceModify(enetdev3,
                            WP_DEV_MOD_ENET_PHYSTATMODE |
                            WP_DEV_MOD_ENET_TX_STATMODE |
                            WP_DEV_MOD_ENET_RX_STATMODE,
                            &enet_dev);
   terminate_on_error(status, "WP_DeviceModify() ENET 3");

   status = WP_DeviceModify(enetdev4,
                            WP_DEV_MOD_ENET_PHYSTATMODE |
                            WP_DEV_MOD_ENET_TX_STATMODE |
                            WP_DEV_MOD_ENET_RX_STATMODE,
                            &enet_dev);
   terminate_on_error(status, "WP_DeviceModify() ENET 4");

   iw_dev = WP_DeviceCreate(iw_port, WP_PHY(0), WP_DEVICE_IW_HOST, NULL);
   terminate_on_error(iw_dev, "DeviceCreate");   

   host_device10 = WP_DeviceCreate(host_port10, WP_PHY(0), WP_DEVICE_IW_HOST, NULL);
   terminate_on_error(host_device10, "Host Device 10 Create");   

   host_device11 = WP_DeviceCreate(host_port11, WP_PHY(0), WP_DEVICE_IW_HOST, NULL);
   terminate_on_error(host_device11, "Host Device 11 Create");   

   enet_dev->extended_params = enet_dev_ex;
}

static void WPE_CreateChannels(void)
{
   /* create device 1 rx and tx channels */
   ch_config_enet[0].iwmode = WP_PKTCH_IWM_DISABLE;
   tx_channel_enet_handle1 = WP_ChannelCreate(ETH_TX_TAG1, enetdev1, qniw_enet, WP_CH_TX, WP_ENET, &ch_config_enet[0]);
   terminate_on_error(tx_channel_enet_handle1, "WP_ChannelCreate() Enet TX 1");

   ch_config_enet[0].iwmode = WP_PKTCH_IWM_ENABLE;
   rx_channel_enet_handle1 = WP_ChannelCreate(ETH_RX_TAG1, enetdev1, qniw_enet, WP_CH_RX, WP_ENET, &ch_config_enet[0]);
   terminate_on_error(rx_channel_enet_handle1, "WP_ChannelCreate() Enet RX");

   /* create device 2 rx and tx channels */
   ch_config_enet[0].iwmode = WP_PKTCH_IWM_ENABLE;
   tx_channel_enet_handle2 = WP_ChannelCreate(ETH_TX_TAG2, enetdev2, qniw_enet, WP_CH_TX, WP_ENET, &ch_config_enet[0]);
   terminate_on_error(tx_channel_enet_handle2, "WP_ChannelCreate() Enet TX 2");

   ch_config_enet[0].iwmode = WP_PKTCH_IWM_DISABLE;
   rx_channel_enet_handle2 = WP_ChannelCreate(ETH_RX_TAG2, enetdev2, qniw_enet, WP_CH_RX, WP_ENET, &ch_config_enet[0]);
   terminate_on_error(rx_channel_enet_handle2, "WP_ChannelCreate() Enet RX");
#if 0
   /* Create host channel of bridging port */
   iw_tx_ch_cfg->iw_system = iw_system_bridging_handle;
   iw_tx_ch_cfg->group_id = WP_IW_CH_GROUP_ID_NULL;
   iw_tx_ch_cfg->input_port = bport_iw_host;
   tx_host_bp_handle = WP_ChannelCreate(HOST_TX_CH_TAG10, host_device10, qniw_enet5, WP_CH_TX, WP_IW_HOST, &iw_tx_ch_cfg);
   terminate_on_error(tx_host_bp_handle, "Host TX Channel BP Create(iw)");
#endif

   /* Create dummy host channel */ 
   iw_rx_ch = WP_ChannelCreate(IW_RX_DUMMY_CH_TAG, iw_dev, qniw_enet, WP_CH_RX, WP_IW_HOST, &iw_rx_ch_cfg);
   terminate_on_error(iw_rx_ch, "ChannelCreate(iw)");
}


static void WPE_CreatePolicers(void)
{
   //WP_U32 ingress_pol_index = 0;
   WP_U32 pol_ingress1_ctr = 0;

   policer_action_handle[0] = WP_PolicerActionCreate(WP_WINPATH(0), &policer_action);
   terminate_on_error(policer_action_handle[0], "WP_PolicerActionCreate");    

/*ingress policers on receiver channel 0-ENET7 RX ch*/
   //for(ingress_pol_index=0;ingress_pol_index<MAX_IW_POLICERS_INGRESS;ingress_pol_index++)
   {
      pv2.policer_action = policer_action_handle[0];
      policer_handle_ingress[0] = WP_PolicerCreate(WP_WINPATH(0), 
                                                                                 WP_POLICER_V2,
                                                                                  &pv2);
      terminate_on_error(policer_handle_ingress[0], "WP_PolicerCreate");
      pol_ingress1_ctr++;
#ifdef DEBUG_PRINT
      printf("---- Policer  Created : 0x%X ----------\n",(WP_U32)policer_handle_ingress[0]);
#endif   
   }
}
   
static void WPE_CreateGenericFlowAggregationsAndBports(void)
{
  WP_handle status;   
  /* create flow aggregations with no iw_port yet.      
  this will be later modified by WP_IwFlowAggregationModify.      
  this is the way to solve the initialization problem of circular       
  dependency between flow aggregation and Iw_Port */  

  tx_agg_bridging->txfunc = iw_rx_ch;   
  tx_agg_bridging->vlan_tag_mode = WP_IW_VLAN_TAG_DISABLE;  
  flow_aggregation1 = WP_IwFlowAggregationCreate(WP_WINPATH(0), WP_IW_TRANS_BRIDGING_MODE, &tx_agg_bridging);   
  terminate_on_error(flow_aggregation1," WP_IwFlowAggregationCreate()");   
  flow_aggregation2 = WP_IwFlowAggregationCreate(WP_WINPATH(0), WP_IW_TRANS_BRIDGING_MODE, &tx_agg_bridging);   
  terminate_on_error(flow_aggregation2," WP_IwFlowAggregationCreate()");   

  /* Create the bridging ports */   
  bport_conf->flow_agg = flow_aggregation2;/* ### */   
  bport_conf->tag = IW_PORT_1;   
  bport_iw1 = WP_IwBridgingPortCreate( iw_system_bridging_handle, bport_conf);   
  terminate_on_error(bport_iw1, "WP_IwBridgingPortCreate() 1");   
  
  bport_conf->flow_agg = flow_aggregation2;   
  bport_conf->tag = IW_PORT_2;   
  bport_iw2 = WP_IwBridgingPortCreate( iw_system_bridging_handle, bport_conf);   
  terminate_on_error(bport_iw2, "WP_IwBridgingPortCreate() 2");   

   bport_conf->flow_agg = flow_aggregation2;
   bport_conf->tag = IW_PORT_3;
   bport_iw_host = WP_IwBridgingPortCreate( iw_system_bridging_handle, bport_conf);
   terminate_on_error(bport_iw_host, "WP_IwBridgingPortCreate() host");
   
  /* Update the Flow Aggregations*/   
  tx_agg_bridging->txfunc = tx_channel_enet_handle2;   
  tx_agg_bridging->iw_port = bport_iw1;   
  status = WP_IwFlowAggregationModify(flow_aggregation1, WP_IW_FLOW_AGG_MOD_TXFUNC, tx_agg_bridging);  
  terminate_on_error(status, "WP_IwFlowAggregationModify() 1");   
  tx_agg_bridging->txfunc = tx_channel_enet_handle1;  
  tx_agg_bridging->iw_port = bport_iw2;   
  status = WP_IwFlowAggregationModify(flow_aggregation2, WP_IW_FLOW_AGG_MOD_TXFUNC, tx_agg_bridging);   
  terminate_on_error(status, "WP_IwFlowAggregationModify() 2");

}

static void WPE_CreateIwSystemAndItsComponents(void)
{
   WP_status status;

   WPE_CreateChannels();

   WPE_CreatePolicers();
   
   status = WP_IwTxBindingCreate(tx_channel_enet_handle1, WP_IW_TX_BINDING, tx_binding_cfg);
   terminate_on_error(status, "WP_IwTxBindingCreate()");

   /* Create the interworking system */
   iw_system_bridging_handle = WP_IwSystemCreate(WP_WINPATH(0), WP_IW_TRANS_BRIDGING_MODE, &iw_sys);
   terminate_on_error(iw_system_bridging_handle, "WP_IwSystemCreate() - Bridging");

   WPE_CreateGenericFlowAggregationsAndBports();


#if 1
   /* Create host channel of bridging port */
   iw_tx_ch_cfg->iw_system = iw_system_bridging_handle;
   iw_tx_ch_cfg->group_id = WP_IW_CH_GROUP_ID_NULL;
   iw_tx_ch_cfg->input_port = bport_iw_host;
   tx_host_bp_handle = WP_ChannelCreate(HOST_TX_CH_TAG10, host_device10, qniw_enet5, WP_CH_TX, WP_IW_HOST, &iw_tx_ch_cfg);
   terminate_on_error(tx_host_bp_handle, "Host TX Channel BP Create(iw)");
#endif




   
   /* Create Rx binding to the Rx Channel 1 */
   rx_binding_cfg->input_port = bport_iw1;
   status = WP_IwRxBindingCreate(rx_channel_enet_handle1, iw_system_bridging_handle, qniw_enet, &(rx_binding_cfg[0]));
   terminate_on_error(status, "WP_RxBindingCreate()");
   
   /* Create Tx binding to the Tx Channel 2 */
   status = WP_IwTxBindingCreate(tx_channel_enet_handle2, WP_IW_TX_BINDING, tx_binding_cfg);
   terminate_on_error(status, "WP_IwTxBindingCreate() ENET TX 2");
}

      
static void WPE_EnablePortsDevicesChannels(void)
{
   WP_status status;

   /* Enable the Enet Port */
   status = WP_PortEnable(enetp1, WP_DIRECTION_DUPLEX);
   terminate_on_error(status, "WP_PortEnable 1");

   status = WP_PortEnable(enetp2, WP_DIRECTION_DUPLEX);
   terminate_on_error(status, "WP_PortEnable 2");

   status = WP_PortEnable(enetp3, WP_DIRECTION_DUPLEX);
   terminate_on_error(status, "WP_PortEnable 3");
  
   status = WP_PortEnable(enetp4, WP_DIRECTION_DUPLEX);
   terminate_on_error(status, "WP_PortEnable 4");
  
   status = WP_DeviceEnable(enetdev1, WP_DIRECTION_DUPLEX);
   terminate_on_error(status, "WP_DeviceEnable 1");

   status = WP_DeviceEnable(enetdev2, WP_DIRECTION_DUPLEX);
   terminate_on_error(status, "WP_DeviceEnable 2");

   status = WP_DeviceEnable(enetdev3, WP_DIRECTION_DUPLEX);
   terminate_on_error(status, "WP_DeviceEnable 3");

   status = WP_DeviceEnable(enetdev4, WP_DIRECTION_DUPLEX);
   terminate_on_error(status, "WP_DeviceEnable 4");

   status = WP_ChannelEnable(rx_channel_enet_handle1);
   terminate_on_error(status, "WP_ChannelEnable Enet Rx 1");

   status = WP_ChannelEnable(rx_channel_enet_handle2);
   terminate_on_error(status, "WP_ChannelEnable Enet Rx 2");
#if 0
   status = WP_ChannelEnable(rx_channel_enet_handle3);
   terminate_on_error(status, "WP_ChannelEnable Enet Rx 3");

   status = WP_ChannelEnable(rx_channel_enet_handle4);
   terminate_on_error(status, "WP_ChannelEnable Enet Rx 4");
 #endif
   status = WP_ChannelEnable(tx_channel_enet_handle1);
   terminate_on_error(status, "WP_ChannelEnable Gen 1");

   status = WP_ChannelEnable(tx_channel_enet_handle2);
   terminate_on_error(status, "WP_ChannelEnable Gen 2");
#if 0
   status = WP_ChannelEnable(tx_channel_enet_handle3);
   terminate_on_error(status, "WP_ChannelEnable Gen 3");

   status = WP_ChannelEnable(tx_channel_enet_handle4);
   terminate_on_error(status, "WP_ChannelEnable Gen 4");
 #endif
}

static void WPE_Terminate(WP_U32 status)
{
   /* Release WDDI */
   WP_DriverRelease();

   /* For socket based simulators */
#ifdef USE_SOCKET
   CleanUpSocket(5);
#endif
   exit(status);
}
#define TESTSTRING(x) TESTSTRING_(x)
#define TESTSTRING_(x) #x


/////////////////////////////////////////////////////////////////////////////////////////////
//
// PCE WDDI data test items creation
//
/////////////////////////////////////////////////////////////////////////////////////////////
WP_handle dfilter[PCE_MAX_FILTERS];
WP_handle dmsk_filter1, drng_filter1, dmsk_rng_filter1;
WP_handle dext_filter;
WP_handle dfilter_set_lvl0_handle[PCE_LVL_0_MAX_SETS];
WP_handle dfilter_set_lvl1_handle[PCE_LVL_1_MAX_SETS];
WP_handle dfilter_set_lvl2_handle[PCE_LVL_2_MAX_SETS];
WP_handle drule_handles[PCE_MAX_RULES];

static void WPE_CreatePceFilters(void)
{
   WP_pce_filter_classification filter_clas;
   int i = 0;
/*********** unused addresses commented out ************/
//    WP_ipv4_addr ipv4_mask1 = 0x00FFFFFF;//0x007ffffe;
//    WP_ipv4_addr ipv4_mask2 = 0xFFFFFF00;//0x3ffff800;
//    WP_ipv4_addr ipv4_mask3 = 0x007ffffe;
//    WP_ipv4_addr ipv4_mask4 = 0x3ffff800;
//    WP_ipv4_addr ipv4_mask5 = 0x003c0000;
  // WP_ipv4_addr ipv4_mask6 = 0x00FFFF00;
  WP_U8 mask_mac_addr[6]={0xFE,0xFF,0xFF,0xFF,0xFF,0xFF};
   memset(dfilter, 0, sizeof(dfilter));
      
   /******************************************************************/
   /**************** Start of filters configuration ******************/
   /******************************************************************/
   filter_clas.no_match_action = WP_PCE_FILTER_NO_MATCH_CONTINUE;
   filter_clas.no_fields_action = WP_PCE_FILTER_NO_FIELDS_CONTINUE;
   filter_clas.no_match_result[0].result_type = WP_PCE_RESULT_LAST;

   /***************** Filter 1 configurations *******************/
   filter_clas.no_match_action = WP_PCE_FILTER_NO_MATCH_CONTINUE;
   filter_clas.no_match_result[0].result_type = WP_PCE_RESULT_LAST;

   filter_clas.filter_fields[0].field_id = WP_PCE_FIELD_ID_MAC_DA;
   filter_clas.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_clas.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   filter_clas.filter_fields[1].field_id = WP_PCE_FIELD_ID_LAST;
   filter_clas.filter_fields[1].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_clas.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   dfilter[PCE_FILTER_DMAC] = WP_PceFilterCreate(WP_WINPATH(0), WP_PCE_FILTER_CLASSIFICATION, &filter_clas);
   terminate_on_error(dfilter[PCE_FILTER_DMAC], "WP_PceFilterCreate 1[MAC_DA]");
   
   APP_PRINTF("\n---- Single Filter 1 [MAC_DA] Created: 0x%X ----------\n", (WP_U32) dfilter[PCE_FILTER_DMAC]);

   /* Back to default */
   filter_clas.no_match_result[0].result_type = WP_PCE_RESULT_LAST;
   filter_clas.no_match_action = WP_PCE_FILTER_NO_MATCH_CONTINUE;

   /***************** Filter 2 configurations *******************/
   filter_clas.filter_fields[0].field_id = WP_PCE_FIELD_ID_MAC_DA;
   filter_clas.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_clas.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_USED;

   for(i=0; i<6; i++)
      filter_clas.filter_fields[0].mask.mac_addr[i] = mask_mac_addr[i];

   filter_clas.filter_fields[1].field_id = WP_PCE_FIELD_ID_LAST;
   filter_clas.filter_fields[1].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_clas.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   dfilter[2] = WP_PceFilterCreate(WP_WINPATH(0), WP_PCE_FILTER_CLASSIFICATION, &filter_clas);
   terminate_on_error(dfilter[2], "WP_PceFilterCreate 2");
   APP_PRINTF("\n---- Single Filter 2 [IPV4_DA] Created: 0x%X ----------\n", (WP_U32) dfilter[2]);
#if 0
   /***************** Filter 3 configurations *******************/
   filter_clas.no_match_action = WP_PCE_FILTER_NO_MATCH_DENY;
   filter_clas.no_fields_action = WP_PCE_FILTER_NO_FIELDS_DENY;
   filter_clas.filter_fields[0].field_id = WP_PCE_FIELD_ID_IPV4_DA;
   filter_clas.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_RANGE_LOW;
   filter_clas.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_USED;
   filter_clas.filter_fields[0].mask.ipv4_addr = ipv4_mask6;

   filter_clas.filter_fields[1].field_id = WP_PCE_FIELD_ID_IPV4_DA;
   filter_clas.filter_fields[1].field_mode = WP_PCE_FIELD_MODE_COMPARE_RANGE_HIGH;
   filter_clas.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_USED;
   filter_clas.filter_fields[1].mask.ipv4_addr = ipv4_mask6;

   filter_clas.filter_fields[2].field_id = WP_PCE_FIELD_ID_LAST;
   filter_clas.filter_fields[2].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_clas.filter_fields[2].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   dfilter[3] = WP_PceFilterCreate(WP_WINPATH(0), WP_PCE_FILTER_CLASSIFICATION, &filter_clas);
   terminate_on_error(dfilter[3], "WP_PceFilterCreate 3");
   APP_PRINTF("\n---- Range Filter 3 [IPv4 DA] Created: 0x%X ----------\n", (WP_U32) dfilter[3]);
   /* Back to default */
   filter_clas.no_match_action = WP_PCE_FILTER_NO_MATCH_CONTINUE;
   filter_clas.no_fields_action = WP_PCE_FILTER_NO_FIELDS_CONTINUE;
   /***************** Filter 4 configurations *******************/
   filter_clas.filter_fields[0].field_id = WP_PCE_FIELD_ID_IPV4_SA;
   filter_clas.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_clas.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   filter_clas.filter_fields[1].field_id = WP_PCE_FIELD_ID_LAST;
   filter_clas.filter_fields[1].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_clas.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   dfilter[4] = WP_PceFilterCreate(WP_WINPATH(0), WP_PCE_FILTER_CLASSIFICATION, &filter_clas);
   terminate_on_error(dfilter[4], "WP_PceFilterCreate 4");
   APP_PRINTF("\n---- Single Filter 4 [IPV4_SA] Created: 0x%X ----------\n", (WP_U32) dfilter[4]);

   /***************** Filter 5 configurations *******************/
   /* dummy filter, need to modified TODO */
   filter_clas.filter_fields[0].field_id = WP_PCE_FIELD_ID_VLAN_TYPE;
   filter_clas.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_clas.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   filter_clas.filter_fields[1].field_id = WP_PCE_FIELD_ID_LAST;
   filter_clas.filter_fields[1].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_clas.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   dfilter[5] = WP_PceFilterCreate(WP_WINPATH(0), WP_PCE_FILTER_CLASSIFICATION, &filter_clas);
   terminate_on_error(dfilter[5], "WP_PceFilterCreate 5");
   APP_PRINTF("\n---- Single Filter 5 [VLAN_TYPE] Created: 0x%X ----------\n", (WP_U32) dfilter[5]);

   /***************** Filter 6 configurations *******************/
   /* dummy filter, need to modified TODO */
   filter_clas.filter_fields[0].field_id = WP_PCE_FIELD_ID_VLAN_TYPE;
   filter_clas.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_clas.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   filter_clas.filter_fields[1].field_id = WP_PCE_FIELD_ID_LAST;
   filter_clas.filter_fields[1].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_clas.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   dfilter[6] = WP_PceFilterCreate(WP_WINPATH(0), WP_PCE_FILTER_CLASSIFICATION, &filter_clas);
   terminate_on_error(dfilter[6], "WP_PceFilterCreate 6");
   APP_PRINTF("\n---- Single Filter 6 [VLAN_TYPE] Created: 0x%X ----------\n", (WP_U32) dfilter[6]);

   /***************** Filter 7 configurations *******************/
   /* dummy filter, need to modified TODO */
   filter_clas.filter_fields[0].field_id = WP_PCE_FIELD_ID_VLAN_TYPE;
   filter_clas.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_clas.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   filter_clas.filter_fields[1].field_id = WP_PCE_FIELD_ID_LAST;
   filter_clas.filter_fields[1].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_clas.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   dfilter[7] = WP_PceFilterCreate(WP_WINPATH(0), WP_PCE_FILTER_CLASSIFICATION, &filter_clas);
   terminate_on_error(dfilter[7], "WP_PceFilterCreate 7");
   APP_PRINTF("\n---- Single Filter 7 [VLAN_TYPE] Created: 0x%X ----------\n", (WP_U32) dfilter[7]);

   /***************** Filter 8 configurations *******************/
   filter_clas.filter_fields[0].field_id = WP_PCE_FIELD_ID_VLAN_TAG;
   filter_clas.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_clas.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_USED;
   filter_clas.filter_fields[0].mask.vlan_tag = WP_PCE_FIELD_MASK_VLAN_ID;

   filter_clas.filter_fields[1].field_id = WP_PCE_FIELD_ID_LAST;
   filter_clas.filter_fields[1].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_clas.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   dfilter[8] = WP_PceFilterCreate(WP_WINPATH(0), WP_PCE_FILTER_CLASSIFICATION, &filter_clas);
   terminate_on_error(dfilter[8], "WP_PceFilterCreate 8");
   APP_PRINTF("\n---- Single Filter 8 [] Created: 0x%X ----------\n", (WP_U32) dfilter[8]);

   /***************** Filter 20 configurations *******************/
   filter_clas.filter_fields[0].field_id = WP_PCE_FIELD_ID_MAC_SA;
   filter_clas.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_clas.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   filter_clas.filter_fields[1].field_id = WP_PCE_FIELD_ID_LAST;
   filter_clas.filter_fields[1].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_clas.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   dfilter[20] = WP_PceFilterCreate(WP_WINPATH(0), WP_PCE_FILTER_CLASSIFICATION, &filter_clas);
   terminate_on_error(dfilter[20], "WP_PceFilterCreate 20");
   APP_PRINTF("\n---- Single Filter 20 [IPV4_SA] Created: 0x%X ----------\n", (WP_U32) dfilter[20]);

   /***************** Filter 21 configurations *******************/
   /* dummy filter, need to modified TODO*/
   filter_clas.filter_fields[0].field_id = WP_PCE_FIELD_ID_VLAN_TYPE;
   filter_clas.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_clas.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   filter_clas.filter_fields[1].field_id = WP_PCE_FIELD_ID_LAST;
   filter_clas.filter_fields[1].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_clas.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   dfilter[21] = WP_PceFilterCreate(WP_WINPATH(0), WP_PCE_FILTER_CLASSIFICATION, &filter_clas);
   terminate_on_error(dfilter[21], "WP_PceFilterCreate 21");
   APP_PRINTF("\n---- Single Filter 21 [VLAN_TYPE] Created: 0x%X ----------\n", (WP_U32) dfilter[21]);

   /***************** Filter 22 configurations *******************/
   filter_clas.filter_fields[0].field_id = WP_PCE_FIELD_ID_ETHER_TYPE;
   filter_clas.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_clas.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   filter_clas.filter_fields[1].field_id = WP_PCE_FIELD_ID_LAST;
   filter_clas.filter_fields[1].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_clas.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   dfilter[22] = WP_PceFilterCreate(WP_WINPATH(0), WP_PCE_FILTER_CLASSIFICATION, &filter_clas);
   terminate_on_error(dfilter[22], "WP_PceFilterCreate 22");
   APP_PRINTF("\n---- Single Filter 22 [ETHER_TYPE] Created: 0x%X ----------\n", (WP_U32) dfilter[22]);

   /***************** Filter 23 configurations *******************/
   /* dummy filter, need to modified TODO */
   filter_clas.filter_fields[0].field_id = WP_PCE_FIELD_ID_VLAN_TYPE;
   filter_clas.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_clas.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   filter_clas.filter_fields[1].field_id = WP_PCE_FIELD_ID_LAST;
   filter_clas.filter_fields[1].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_clas.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   dfilter[23] = WP_PceFilterCreate(WP_WINPATH(0), WP_PCE_FILTER_CLASSIFICATION, &filter_clas);
   terminate_on_error(dfilter[23], "WP_PceFilterCreate 23");
   APP_PRINTF("\n---- Single Filter 23 [VLAN_TYPE] Created: 0x%X ----------\n", (WP_U32) dfilter[23]);

   /***************** Filter 24 configurations *******************/
   /* dummy filter, need to modified TODO */
   filter_clas.filter_fields[0].field_id = WP_PCE_FIELD_ID_VLAN_TYPE;
   filter_clas.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_clas.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   filter_clas.filter_fields[1].field_id = WP_PCE_FIELD_ID_LAST;
   filter_clas.filter_fields[1].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_clas.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   dfilter[24] = WP_PceFilterCreate(WP_WINPATH(0), WP_PCE_FILTER_CLASSIFICATION, &filter_clas);
   terminate_on_error(dfilter[24], "WP_PceFilterCreate 24");
   APP_PRINTF("\n---- Single Filter 24 [VLAN_TYPE] Created: 0x%X ----------\n", (WP_U32) dfilter[24]);
 
   /***************** Filter 25 configurations *******************/
   /* dummy filter, need to modified TODO */
   filter_clas.filter_fields[0].field_id = WP_PCE_FIELD_ID_VLAN_TYPE;
   filter_clas.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_clas.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   filter_clas.filter_fields[1].field_id = WP_PCE_FIELD_ID_LAST;
   filter_clas.filter_fields[1].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_clas.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   dfilter[25] = WP_PceFilterCreate(WP_WINPATH(0), WP_PCE_FILTER_CLASSIFICATION, &filter_clas);
   terminate_on_error(dfilter[25], "WP_PceFilterCreate 25");
   APP_PRINTF("\n---- Single Filter 25 [VLAN_TYPE] Created: 0x%X ----------\n", (WP_U32) dfilter[25]);


   /***************** Filter 26 configurations *******************/
   /* dummy filter, need to modified TODO */
   filter_clas.filter_fields[0].field_id = WP_PCE_FIELD_ID_VLAN_TYPE;
   filter_clas.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_clas.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   filter_clas.filter_fields[1].field_id = WP_PCE_FIELD_ID_LAST;
   filter_clas.filter_fields[1].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_clas.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   dfilter[26] = WP_PceFilterCreate(WP_WINPATH(0), WP_PCE_FILTER_CLASSIFICATION, &filter_clas);
   terminate_on_error(dfilter[26], "WP_PceFilterCreate 26");
   APP_PRINTF("\n---- Single Filter 26 [VLAN_TYPE] Created: 0x%X ----------\n", (WP_U32) dfilter[26]);

   /***************** Filter 27 configurations *******************/
   /* dummy filter, need to modified TODO */
   filter_clas.filter_fields[0].field_id = WP_PCE_FIELD_ID_VLAN_TYPE;
   filter_clas.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_clas.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   filter_clas.filter_fields[1].field_id = WP_PCE_FIELD_ID_LAST;
   filter_clas.filter_fields[1].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_clas.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   dfilter[27] = WP_PceFilterCreate(WP_WINPATH(0), WP_PCE_FILTER_CLASSIFICATION, &filter_clas);
   terminate_on_error(dfilter[27], "WP_PceFilterCreate 27");
   APP_PRINTF("\n---- Single Filter 27 [VLAN_TYPE] Created: 0x%X ----------\n", (WP_U32) dfilter[27]);

   /***************** Filter 30 configurations *******************/
   filter_clas.no_match_action = WP_PCE_FILTER_NO_MATCH_DENY;
   filter_clas.no_fields_action = WP_PCE_FILTER_NO_FIELDS_DENY;
   filter_clas.filter_fields[0].field_id = WP_PCE_FIELD_ID_REMARKED_MAC_SA;
   filter_clas.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_clas.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   filter_clas.filter_fields[1].field_id = WP_PCE_FIELD_ID_LAST;
   filter_clas.filter_fields[1].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_clas.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   dfilter[30] = WP_PceFilterCreate(WP_WINPATH(0), WP_PCE_FILTER_CLASSIFICATION, &filter_clas);
   terminate_on_error(dfilter[30], "WP_PceFilterCreate 30");
   APP_PRINTF("\n---- Single Filter 30 [Remarked MAC SA] Created: 0x%X ----------\n", (WP_U32) dfilter[30]);  
   /******************************************************************/
   /***************** End of filters configuration *******************/
   /******************************************************************/
 #endif
}

static void WPE_CreatePceRules(void)
{
   WP_pce_rule_classification rule_cfg;
   /* addresses not in use are commented out */
   WP_U8 mac_addr1[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
   WP_U8 mac_addr2[] = {0x01, 0x00, 0x00, 0x01, 0x02, 0x03};
 //  WP_U8 mac_addr3[] = {0x02, 0x01, 0x01, 0x01, 0x03, 0x03};
//   WP_U8 mac_addr4[] = {0x06, 0x01, 0x01, 0x01, 0x01, 0x01};
//   WP_U8 mac_addr5[] = {0x06, 0x01, 0x01, 0x01, 0x01, 0x02};
//   WP_U8 mac_addr6[] = {0x06, 0x01, 0x01, 0x01, 0x01, 0x03};
//   WP_U8 mac_addr7[] = {0x06, 0x01, 0x01, 0x01, 0x01, 0x04};
//   WP_U8 mac_addr8[] = {0x0A, 0x01, 0x01, 0x01, 0x01, 0x01};
   WP_vlan_id vlan_id[MAX_VLAN_ID];
   WP_ipv4_addr ipv4_addr_dst[PCE_MAX_RULES];
   WP_ipv4_addr ipv4_addr_src[PCE_MAX_RULES];

   WP_U16 i, rules_created = 0;

   for (i = 0; i < PCE_MAX_RULES; i++)
   {
      ipv4_addr_dst[i] = 0xC0A86464 + i;
      ipv4_addr_src[i] = 0x80A86464 + i;
   }
   for (i = 0; i < MAX_VLAN_ID; i++)
      vlan_id[i] = 0x000A + i;
   
   /************** common rules configurations*******************/
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_INGRESS_POLICER;
   rule_cfg.match_result[0].param.ingress_policer.policer = policer_handle_ingress[0];
   rule_cfg.match_result[1].result_type = WP_PCE_RESULT_FLOW_AGG;
   rule_cfg.match_result[1].param.flow_agg.flow_aggregation = flow_aggregation1;
   rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;
   rule_cfg.enabled = WP_ENABLE;
   rule_cfg.filter_handle = dfilter[PCE_FILTER_DMAC];
   rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_MAC_DA;
   rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_LAST;
   /********************* Filter 1 rule 1 ************************/
   rule_cfg.match_action = WP_PCE_RULE_MATCH_ACCEPT;
   for(i=0; i<6; i++)
      rule_cfg.rule_fields[0].value.mac_addr[i] = mac_addr1[i];
   APP_PRINTF("\n---- Rule %d Creating --[MAC_DA = FF-FF-FF-FF-FF-FF, result ===>policer]----\n", rules_created);
   drule_handles[rules_created] = WP_PceRuleCreate(WP_WINPATH(0), WP_PCE_RULE_CLASSIFICATION, &rule_cfg);
   terminate_on_error(dfilter[rules_created], "WP_PceRuleCreate");
   APP_PRINTF("rule_handle[%d] = %X\n", rules_created, drule_handles[rules_created]);
   rules_created++;
  
   /********************* Filter 2 rule 1 ************************/
   rule_cfg.match_action = WP_PCE_RULE_MATCH_ACCEPT;
   rule_cfg.filter_handle = dfilter[2];
   for(i=0; i<6; i++)
      rule_cfg.rule_fields[0].value.mac_addr[i] = mac_addr2[i];

   APP_PRINTF("\n---- Rule %d Creating --[MAC_DA = 01-00-00-01-02-03, result FLOW_AGG = policer] ----\n", rules_created);
   drule_handles[rules_created] = WP_PceRuleCreate(WP_WINPATH(0), WP_PCE_RULE_CLASSIFICATION, &rule_cfg);
   terminate_on_error(dfilter[rules_created], "WP_PceRuleCreate");
   APP_PRINTF("rule_handle[%d] = %X\n", rules_created, drule_handles[rules_created]);
   rules_created++;
#if 0 
   /********************* Filter 1 rule 3 ************************/
   rule_cfg.match_action = WP_PCE_RULE_MATCH_CONTINUE;
   for(i=0; i<6; i++)
      rule_cfg.rule_fields[0].value.mac_addr[i] = mac_addr3[i];

   APP_PRINTF("\n---- Rule %d Creating --[MAC_DA = 020101010303, result Continue ----\n", rules_created);
   drule_handles[rules_created] = WP_PceRuleCreate(WP_WINPATH(0), WP_PCE_RULE_CLASSIFICATION, &rule_cfg);
   terminate_on_error(dfilter[rules_created], "WP_PceRuleCreate");
   APP_PRINTF("rule_handle[%d] = %X\n", rules_created, drule_handles[rules_created]);
   rules_created++;

   /********************* Filter 1 rule 4 ************************/
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_IWCP_START_REDIRECT;
   
   rule_cfg.match_action = WP_PCE_RULE_MATCH_CONTINUE;
   for(i=0; i<6; i++)
      rule_cfg.rule_fields[0].value.mac_addr[i] = mac_addr8[i];

   APP_PRINTF("\n---- Rule %d Creating --[MAC_DA = 0A0101010101, result Continue ----\n", rules_created);
   drule_handles[rules_created] = WP_PceRuleCreate(WP_WINPATH(0), WP_PCE_RULE_CLASSIFICATION, &rule_cfg);
   terminate_on_error(dfilter[rules_created], "WP_PceRuleCreate");
   APP_PRINTF("rule_handle[%d] = %X\n", rules_created, drule_handles[rules_created]);
   rules_created++;


   /* Backe to default */
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
   rule_cfg.match_result[0].param.flow_agg.flow_aggregation = flow_aggregation3;
   rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;

   /********************* Filter 2 rules ************************/
   rule_cfg.filter_handle = dfilter[2];
   rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_IPV4_DA;
   rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_LAST;
   /********************* Filter 2 rule 1 ************************/
   rule_cfg.match_action = WP_PCE_RULE_MATCH_ACCEPT;
   rule_cfg.rule_fields[0].value.ipv4_addr = ipv4_addr_dst[0];

   APP_PRINTF("\n---- Rule %d Creating --[IP_DA = 0xC0A86464, result FLOW_AGG = flow_agg3] ----\n", rules_created);
   drule_handles[rules_created] = WP_PceRuleCreate(WP_WINPATH(0), WP_PCE_RULE_CLASSIFICATION, &rule_cfg);
   terminate_on_error(dfilter[rules_created], "WP_PceRuleCreate");
   APP_PRINTF("rule_handle[%d] = %X\n", rules_created, drule_handles[rules_created]);
   rules_created++;

   /********************* Filter 2 rule 2 ************************/
   rule_cfg.match_action = WP_PCE_RULE_MATCH_ACCEPT;
   rule_cfg.rule_fields[0].value.ipv4_addr = ipv4_addr_dst[1];

   APP_PRINTF("\n---- Rule %d Creating --[IP_DA = 0xC0A86465, result FLOW_AGG = flow_agg3] ----\n", rules_created);
   drule_handles[rules_created] = WP_PceRuleCreate(WP_WINPATH(0), WP_PCE_RULE_CLASSIFICATION, &rule_cfg);
   terminate_on_error(dfilter[rules_created], "WP_PceRuleCreate");
   APP_PRINTF("rule_handle[%d] = %X\n", rules_created, drule_handles[rules_created]);
   rules_created++;

   /********************* Filter 2 rule 3 ************************/
   rule_cfg.match_action = WP_PCE_RULE_MATCH_CONTINUE;
   rule_cfg.rule_fields[0].value.ipv4_addr = ipv4_addr_dst[2];

   APP_PRINTF("\n---- Rule %d Creating --[IP_DA = 0xC0A86466, result Continue] ----\n", rules_created);
   drule_handles[rules_created] = WP_PceRuleCreate(WP_WINPATH(0), WP_PCE_RULE_CLASSIFICATION, &rule_cfg);
   terminate_on_error(dfilter[rules_created], "WP_PceRuleCreate");
   APP_PRINTF("rule_handle[%d] = %X\n", rules_created, drule_handles[rules_created]);
   rules_created++;

   /********************* Filter 3 rules ************************/
   rule_cfg.filter_handle = dfilter[3];
   rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_IPV4_DA;
   rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_IPV4_DA;
   rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_LAST;
   /********************* Filter 3 rule 1 ************************/
   rule_cfg.match_action = WP_PCE_RULE_MATCH_ACCEPT;
   rule_cfg.rule_fields[0].value.ipv4_addr = ipv4_addr_dst[3];
   rule_cfg.rule_fields[1].value.ipv4_addr = ipv4_addr_dst[7];

   /* VLAN priority modification */
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_PREFIX_PRIORITY_REMARKING;
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[0] = WP_PCE_RESULT_PPR_TYPE_EXT_VLAN_PRIO;
   rule_cfg.match_result[0].param.prefix_priority_remarking.value[0] = 0x3;
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[1] = WP_PCE_RESULT_PPR_TYPE_DO_NOT_CHANGE;
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[2] = WP_PCE_RESULT_PPR_TYPE_DO_NOT_CHANGE;
   rule_cfg.match_result[0].param.prefix_priority_remarking.remarking_type[3] = WP_PCE_RESULT_PPR_TYPE_DO_NOT_CHANGE;
   rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;

   APP_PRINTF("\n---- Rule %d Creating --[Range IP_DA = 0xC0A86467-A, result FLOW_AGG = flow_agg3] ----\n", rules_created);
   drule_handles[rules_created] = WP_PceRuleCreate(WP_WINPATH(0), WP_PCE_RULE_CLASSIFICATION, &rule_cfg);
   terminate_on_error(dfilter[rules_created], "WP_PceRuleCreate");
   APP_PRINTF("rule_handle[%d] = %X\n", rules_created, drule_handles[rules_created]);
   rules_created++;

   /********************* Filter 4 rules ************************/
   rule_cfg.filter_handle = dfilter[4];
   rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_IPV4_SA;
   rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_LAST;
   /********************* Filter 4 rule 1 ************************/
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FILTER_SET_CHANGE;
   rule_cfg.match_result[0].param.change_fs.filter_set_handle = dfilter_set_lvl1_handle[1];
   
   rule_cfg.match_action = WP_PCE_RULE_MATCH_CONTINUE;
   rule_cfg.rule_fields[0].value.ipv4_addr = ipv4_addr_src[0];

   APP_PRINTF("\n---- Rule %d Creating --[IP_SA = 0x80A86464, result Filter set change] ----\n", rules_created);
   drule_handles[rules_created] = WP_PceRuleCreate(WP_WINPATH(0), WP_PCE_RULE_CLASSIFICATION, &rule_cfg);
   terminate_on_error(dfilter[rules_created], "WP_PceRuleCreate");
   APP_PRINTF("rule_handle[%d] = %X\n", rules_created, drule_handles[rules_created]);
   rules_created++;

   /********************* Filter 4 rule 2 ************************/
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FILTER_SET_CHANGE;
   rule_cfg.match_result[0].param.change_fs.filter_set_handle = dfilter_set_lvl2_handle[0];
   rule_cfg.match_action = WP_PCE_RULE_MATCH_CONTINUE;   
   rule_cfg.rule_fields[0].value.ipv4_addr = ipv4_addr_src[1];

   APP_PRINTF("\n---- Rule %d Creating --[IP_SA = 0x80A86465, result Filter set change] ----\n", rules_created);
   drule_handles[rules_created] = WP_PceRuleCreate(WP_WINPATH(0), WP_PCE_RULE_CLASSIFICATION, &rule_cfg);
   terminate_on_error(dfilter[rules_created], "WP_PceRuleCreate");
   APP_PRINTF("rule_handle[%d] = %X\n", rules_created, drule_handles[rules_created]);
   rules_created++;

   /* Back to default */
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
   rule_cfg.match_result[0].param.flow_agg.flow_aggregation = flow_aggregation3;

   /********************* Filter 4 rule 3 ************************/
   rule_cfg.match_action = WP_PCE_RULE_MATCH_CONTINUE;
   rule_cfg.rule_fields[0].value.ipv4_addr = ipv4_addr_src[2];

   APP_PRINTF("\n---- Rule %d Creating --[IP_SA = 0xC0A86466, result Continue] ----\n", rules_created);
   drule_handles[rules_created] = WP_PceRuleCreate(WP_WINPATH(0), WP_PCE_RULE_CLASSIFICATION, &rule_cfg);
   terminate_on_error(dfilter[rules_created], "WP_PceRuleCreate");
   APP_PRINTF("rule_handle[%d] = %X\n", rules_created, drule_handles[rules_created]);
   rules_created++;

   /********************* Filter 8 rules ************************/
   rule_cfg.filter_handle = dfilter[8];
   rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_VLAN_TAG;
   rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_LAST;
   /********************* Filter 8 rule 1 ************************/
   rule_cfg.match_action = WP_PCE_RULE_MATCH_CONTINUE;
   rule_cfg.rule_fields[0].value.vlan_tag = vlan_id[0];

   APP_PRINTF("\n---- Rule %d Creating --[EXT_VLAN = 0x000A, result Continue] ----\n", rules_created);
   drule_handles[rules_created] = WP_PceRuleCreate(WP_WINPATH(0), WP_PCE_RULE_CLASSIFICATION, &rule_cfg);
   terminate_on_error(dfilter[rules_created], "WP_PceRuleCreate");
   APP_PRINTF("rule_handle[%d] = %X\n", rules_created, drule_handles[rules_created]);
   rules_created++;

   /********************* Filter 8 rule 2 ************************/
   rule_cfg.match_action = WP_PCE_RULE_MATCH_CONTINUE;
   rule_cfg.rule_fields[0].value.vlan_tag = vlan_id[1];
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_REMARK_MAC_SA;
   for(i=0; i<6; i++)
      rule_cfg.match_result[0].param.mac_remarking.mac_addr[i] = mac_addr6[i];
   rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;
   APP_PRINTF("\n---- Rule %d Creating --[EXT_VLAN = 0x000B, result jump + Change SA MAC 0x020101010103]----\n", rules_created);
   drule_handles[rules_created] = WP_PceRuleCreate(WP_WINPATH(0), WP_PCE_RULE_CLASSIFICATION, &rule_cfg);
   terminate_on_error(dfilter[rules_created], "WP_PceRuleCreate");
   APP_PRINTF("rule_handle[%d] = %X\n", rules_created, drule_handles[rules_created]);
   rules_created++;

   /* Back to default */
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
   rule_cfg.match_result[0].param.flow_agg.flow_aggregation = flow_aggregation3;
   rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;
   
   /********************* Filter 8 rule 3 ************************/
   rule_cfg.match_action = WP_PCE_RULE_MATCH_ACCEPT;
   rule_cfg.rule_fields[0].value.vlan_tag = vlan_id[2];

   APP_PRINTF("\n---- Rule %d Creating --[EXT_VLAN = 0x000C, result FLOW_AGG = flow_agg3] ----\n", rules_created);
   drule_handles[rules_created] = WP_PceRuleCreate(WP_WINPATH(0), WP_PCE_RULE_CLASSIFICATION, &rule_cfg);
   terminate_on_error(dfilter[rules_created], "WP_PceRuleCreate");
   APP_PRINTF("rule_handle[%d] = %X\n", rules_created, drule_handles[rules_created]);
   rules_created++;

   /********************* Filter 20 rules ************************/
   rule_cfg.filter_handle = dfilter[20];
   rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_MAC_SA;
   rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_LAST;
   /********************* Filter 20 rule 1 ************************/
   rule_cfg.match_action = WP_PCE_RULE_MATCH_CONTINUE;
   for(i=0; i<6; i++)
      rule_cfg.rule_fields[0].value.mac_addr[i] = mac_addr4[i];
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FILTER_SET_CHANGE;
   rule_cfg.match_result[0].param.change_fs.filter_set_handle = dfilter_set_lvl2_handle[1];

   APP_PRINTF("\n---- Rule %d Creating --[MAC_SA = 060101010101, result Filter set change] ----\n", rules_created);
   drule_handles[rules_created] = WP_PceRuleCreate(WP_WINPATH(0), WP_PCE_RULE_CLASSIFICATION, &rule_cfg);
   terminate_on_error(dfilter[rules_created], "WP_PceRuleCreate");
   APP_PRINTF("rule_handle[%d] = %X\n", rules_created, drule_handles[rules_created]);
   rules_created++;

   /* Back to default */
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
   rule_cfg.match_result[0].param.flow_agg.flow_aggregation = flow_aggregation3;

   /********************* Filter 20 rule 2 ************************/
   rule_cfg.match_action = WP_PCE_RULE_MATCH_CONTINUE;
   for(i=0; i<6; i++)
      rule_cfg.rule_fields[0].value.mac_addr[i] = mac_addr5[i];
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FILTER_SET_CHANGE;
   rule_cfg.match_result[0].param.change_fs.filter_set_handle = dfilter_set_lvl2_handle[2];

   APP_PRINTF("\n---- Rule %d Creating --[MAC_SA = 060101010102, result Filter set change] ----\n", rules_created);
   drule_handles[rules_created] = WP_PceRuleCreate(WP_WINPATH(0), WP_PCE_RULE_CLASSIFICATION, &rule_cfg);
   terminate_on_error(dfilter[rules_created], "WP_PceRuleCreate");
   APP_PRINTF("rule_handle[%d] = %X\n", rules_created, drule_handles[rules_created]);
   rules_created++;

   /* Back to default */
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
   rule_cfg.match_result[0].param.flow_agg.flow_aggregation = flow_aggregation3;

   /********************* Filter 20 rule 3 ************************/
   rule_cfg.match_action = WP_PCE_RULE_MATCH_ACCEPT;
   for(i=0; i<6; i++)
      rule_cfg.rule_fields[0].value.mac_addr[i] = mac_addr6[i];

   APP_PRINTF("\n---- Rule %d Creating --[MAC_SA = 060101010103, result Filter set change] ----\n", rules_created);
   drule_handles[rules_created] = WP_PceRuleCreate(WP_WINPATH(0), WP_PCE_RULE_CLASSIFICATION, &rule_cfg);
   terminate_on_error(dfilter[rules_created], "WP_PceRuleCreate");
   APP_PRINTF("rule_handle[%d] = %X\n", rules_created, drule_handles[rules_created]);
   rules_created++;

   /********************* Filter 20 rule 4 ************************/
   rule_cfg.match_action = WP_PCE_RULE_MATCH_CONTINUE;
   for(i=0; i<6; i++)
      rule_cfg.rule_fields[0].value.mac_addr[i] = mac_addr7[i];
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FILTER_SET_CHANGE;
   rule_cfg.match_result[0].param.change_fs.filter_set_handle = dfilter_set_lvl2_handle[2];

   APP_PRINTF("\n---- Rule %d Creating --[MAC_SA = 060101010104, result Filter set change] ----\n", rules_created);
   drule_handles[rules_created] = WP_PceRuleCreate(WP_WINPATH(0), WP_PCE_RULE_CLASSIFICATION, &rule_cfg);
   terminate_on_error(dfilter[rules_created], "WP_PceRuleCreate");
   APP_PRINTF("rule_handle[%d] = %X\n", rules_created, drule_handles[rules_created]);
   rules_created++;

   /* Back to default */
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
   rule_cfg.match_result[0].param.flow_agg.flow_aggregation = flow_aggregation3;

   /********************* Filter 22 rules ************************/
   rule_cfg.filter_handle = dfilter[22];
   rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_ETHER_TYPE;
   rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_LAST;

   /********************* Filter 22 rule 1 ************************/
   rule_cfg.match_action = WP_PCE_RULE_MATCH_ACCEPT;
   rule_cfg.rule_fields[0].value.ethertype = 0x0800;

   APP_PRINTF("\n---- Rule %d Creating --[Ethernet type = 0x0800, result FLOW_AGG = flow_agg3] ----\n", rules_created);
   drule_handles[rules_created] = WP_PceRuleCreate(WP_WINPATH(0), WP_PCE_RULE_CLASSIFICATION, &rule_cfg);
   terminate_on_error(dfilter[rules_created], "WP_PceRuleCreate");
   APP_PRINTF("rule_handle[%d] = %X\n", rules_created, drule_handles[rules_created]);
   rules_created++;

   /********************* Filter 22 rule 2 ************************/
   rule_cfg.match_action = WP_PCE_RULE_MATCH_DENY;
   rule_cfg.rule_fields[0].value.ethertype = 0x86DD;
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_LAST;

   APP_PRINTF("\n---- Rule %d Creating --[Ethernet type, result Deny] ----\n", rules_created);
   drule_handles[rules_created] = WP_PceRuleCreate(WP_WINPATH(0), WP_PCE_RULE_CLASSIFICATION, &rule_cfg);
   terminate_on_error(dfilter[rules_created], "WP_PceRuleCreate");
   APP_PRINTF("rule_handle[%d] = %X\n", rules_created, drule_handles[rules_created]);
   rules_created++;
   /* back to default values */
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;

  /********************* Filter 30 rules ************************/
   rule_cfg.filter_handle = dfilter[30];
   rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_REMARKED_MAC_SA;
   rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_LAST;
   /********************* Filter 30 rule 1 ************************/
   rule_cfg.match_action = WP_PCE_RULE_MATCH_ACCEPT;
   for(i=0; i<6; i++)
      rule_cfg.rule_fields[0].value.mac_addr[i] = mac_addr6[i];

   APP_PRINTF("\n---- Rule %d Creating --[MAC_SA = 060101010103, result accept ] ----\n", rules_created);
   drule_handles[rules_created] = WP_PceRuleCreate(WP_WINPATH(0), WP_PCE_RULE_CLASSIFICATION, &rule_cfg);
   terminate_on_error(dfilter[rules_created], "WP_PceRuleCreate");
   APP_PRINTF("rule_handle[%d] = %X\n", rules_created, drule_handles[rules_created]);
   rules_created++;
 #endif
}


static void WPE_CreatePceFilterSets(void)
{
   WP_pce_filter_set fs_config;
   WP_U32 i;
   
   for (i = 0; i < PCE_MAX_FILTERS; i++)
   {
      if (dfilter[i])
         WP_PceFilterStatisticsReset(dfilter[i],         &(pce_filter_stats[dfilter[i] & 0xFF]));
   }


   fs_config.filter_set_level = 0;
   fs_config.next_filter_set = WP_UNUSED;
   fs_config.filters[0] = dfilter[PCE_FILTER_DMAC];/*Ingress Policer*/
   fs_config.filters[1] = WP_UNUSED;

   dfilter_set_lvl0_handle[0] = WP_PceFilterSetCreate(WP_WINPATH(0), &fs_config);
   terminate_on_error(dfilter_set_lvl0_handle[0], "WP_PceFilterSetCreate");
#ifdef DEBUG_PRINT
   printf("\n---- Level 0 -First Filter Set Created: 0x%X ----------\n", (WP_U32) dfilter_set_lvl0_handle[0]);
#endif
#if 0
   /******************************************************************/
   /****************** Create level 2 filter set ** ******************/
   /******************************************************************/
   /***************** Filter set 8 configurations ********************/
   fs_config.tag = 8;
   fs_config.filter_set_level = 2;
   fs_config.next_filter_set = WP_UNUSED;
   fs_config.filters[0] = dfilter[30];
   fs_config.filters[1] = dfilter[25];
   fs_config.filters[2] = dfilter[26];
   fs_config.filters[3] = dfilter[27];
   fs_config.filters[4] = WP_UNUSED;

   dfilter_set_lvl2_handle[0] = WP_PceFilterSetCreate(WP_WINPATH(0), &fs_config);
   terminate_on_error(dfilter_set_lvl2_handle[0], "WP_PceFilterSetCreate level 2 set 1");
   APP_PRINTF("\n---- Level 2 Filter Set Created: 0x%X ----------\n", (WP_U32) dfilter_set_lvl2_handle[0]);

   /***************** Filter set 9 configurations ********************/
   fs_config.tag = 9;
   fs_config.filter_set_level = 2;
   fs_config.next_filter_set = WP_UNUSED;
   fs_config.filters[0] = dfilter[30];
   fs_config.filters[1] = dfilter[25];
   fs_config.filters[2] = dfilter[26];
   fs_config.filters[3] = dfilter[27];
   fs_config.filters[4] = WP_UNUSED;

   dfilter_set_lvl2_handle[1] = WP_PceFilterSetCreate(WP_WINPATH(0), &fs_config);
   terminate_on_error(dfilter_set_lvl2_handle[1], "WP_PceFilterSetCreate level 2 set 2");
   APP_PRINTF("\n---- Level 2 Filter Set Created: 0x%X ----------\n", (WP_U32) dfilter_set_lvl2_handle[1]);

   /***************** Filter set 10 configurations ********************/
   fs_config.tag = 10;
   fs_config.filter_set_level = 2;
   fs_config.next_filter_set = WP_UNUSED;
   fs_config.filters[0] = dfilter[30];
   fs_config.filters[1] = dfilter[25];
   fs_config.filters[2] = dfilter[26];
   fs_config.filters[3] = dfilter[27];
   fs_config.filters[4] = WP_UNUSED;

   dfilter_set_lvl2_handle[2] = WP_PceFilterSetCreate(WP_WINPATH(0), &fs_config);
   terminate_on_error(dfilter_set_lvl2_handle[2], "WP_PceFilterSetCreate level 2 set 3");
   APP_PRINTF("\n---- Level 2 Filter Set Created: 0x%X ----------\n", (WP_U32) dfilter_set_lvl2_handle[2]);

   /******************************************************************/
   /****************** Create level 1 filter set ** ******************/
   /******************************************************************/
   /***************** Filter set 6 configurations ********************/
   fs_config.tag = 6;
   fs_config.filter_set_level = 1;
   fs_config.next_filter_set = dfilter_set_lvl2_handle[0];
   fs_config.filters[0] = dfilter[20];
   fs_config.filters[1] = dfilter[21];
   fs_config.filters[2] = dfilter[22];
   fs_config.filters[3] = dfilter[23];
   fs_config.filters[4] = WP_UNUSED;

   dfilter_set_lvl1_handle[0] = WP_PceFilterSetCreate(WP_WINPATH(0), &fs_config);
   terminate_on_error(dfilter_set_lvl1_handle[0], "WP_PceFilterSetCreate level 1 set 1");
   APP_PRINTF("\n---- Level 1 Filter Set Created: 0x%X ----------\n", (WP_U32) dfilter_set_lvl1_handle[0]);

   /***************** Filter set 7 configurations ********************/
   fs_config.tag = 7;
   fs_config.filter_set_level = 1;
   fs_config.next_filter_set = dfilter_set_lvl2_handle[0];
   fs_config.filters[0] = dfilter[22];
   fs_config.filters[1] = dfilter[20];
   fs_config.filters[2] = dfilter[24];
   fs_config.filters[3] = dfilter[25];
   fs_config.filters[4] = WP_UNUSED;

   dfilter_set_lvl1_handle[1] = WP_PceFilterSetCreate(WP_WINPATH(0), &fs_config);
   terminate_on_error(dfilter_set_lvl1_handle[1], "WP_PceFilterSetCreate level 1 set 2");
   APP_PRINTF("\n---- Level 1 Filter Set Created: 0x%X ----------\n", (WP_U32) dfilter_set_lvl1_handle[1]);


   /******************************************************************/
   /****************** Create level 0 filter set ** ******************/
   /******************************************************************/
   /***************** Filter set 1 configurations ********************/
   fs_config.tag = 1;
   fs_config.filter_set_level = 0;
   fs_config.next_filter_set = dfilter_set_lvl1_handle[0];
   fs_config.filters[0] = dfilter[1];
   fs_config.filters[1] = dfilter[2];
   fs_config.filters[2] = dfilter[4];
   fs_config.filters[3] = dfilter[8];
   fs_config.filters[4] = WP_UNUSED;

   dfilter_set_lvl0_handle[0] = WP_PceFilterSetCreate(WP_WINPATH(0), &fs_config);
   terminate_on_error(dfilter_set_lvl0_handle[0], "WP_PceFilterSetCreate level 0 set 1");
   APP_PRINTF("\n---- Level 0 Filter Set Created: 0x%X ----------\n", (WP_U32) dfilter_set_lvl0_handle[0]);
   /***************** Filter set 2 configurations ********************/
   fs_config.tag = 2;
   fs_config.filter_set_level = 0;
   fs_config.next_filter_set = dfilter_set_lvl1_handle[0];
   fs_config.filters[0] = dfilter[3];
   fs_config.filters[1] = dfilter[1];
   fs_config.filters[2] = dfilter[2];
   fs_config.filters[3] = dfilter[5];
   fs_config.filters[4] = dfilter[8];
   fs_config.filters[5] = WP_UNUSED;

   dfilter_set_lvl0_handle[1] = WP_PceFilterSetCreate(WP_WINPATH(0), &fs_config);
   terminate_on_error(dfilter_set_lvl0_handle[1], "WP_PceFilterSetCreate  level 0 set 2");
   APP_PRINTF("\n---- Level 0 Filter Set Created: 0x%X ----------\n", (WP_U32) dfilter_set_lvl0_handle[1]);

   /***************** Filter set 3 configurations ********************/
   fs_config.tag = 3;
   fs_config.filter_set_level = 0;
   fs_config.next_filter_set = dfilter_set_lvl1_handle[1];
   fs_config.filters[0] = dfilter[1];
   fs_config.filters[1] = dfilter[3];
   fs_config.filters[2] = dfilter[5];
   fs_config.filters[3] = dfilter[7];
   fs_config.filters[4] = dfilter[8];
   fs_config.filters[5] = WP_UNUSED;

   dfilter_set_lvl0_handle[2] = WP_PceFilterSetCreate(WP_WINPATH(0), &fs_config);
   terminate_on_error(dfilter_set_lvl0_handle[2], "WP_PceFilterSetCreate level 0 set 3");
   APP_PRINTF("\n---- Level 0 Filter Set Created: 0x%X ----------\n", (WP_U32) dfilter_set_lvl0_handle[2]);

   /***************** Filter set 4 configurations ********************/
   fs_config.tag = 4;
   fs_config.filter_set_level = 0;
   fs_config.next_filter_set = dfilter_set_lvl2_handle[1];
   fs_config.filters[0] = dfilter[1];
   fs_config.filters[1] = dfilter[2];
   fs_config.filters[2] = dfilter[4];
   fs_config.filters[3] = WP_UNUSED;

   dfilter_set_lvl0_handle[3] = WP_PceFilterSetCreate(WP_WINPATH(0), &fs_config);
   terminate_on_error(dfilter_set_lvl0_handle[3], "WP_PceFilterSetCreate  level 0 set 4");
   APP_PRINTF("\n---- Level 0 Filter Set Created: 0x%X ----------\n", (WP_U32) dfilter_set_lvl0_handle[3]);

   /***************** Filter set 5 configurations ********************/
   fs_config.tag = 5;
   fs_config.filter_set_level = 0;
   fs_config.next_filter_set = WP_UNUSED;
   fs_config.filters[0] = dfilter[1];
   fs_config.filters[1] = dfilter[5];
   fs_config.filters[2] = dfilter[2];
   fs_config.filters[3] = dfilter[7];
   fs_config.filters[4] = dfilter[8];
   fs_config.filters[5] = WP_UNUSED;

   dfilter_set_lvl0_handle[4] = WP_PceFilterSetCreate(WP_WINPATH(0), &fs_config);
   terminate_on_error(dfilter_set_lvl0_handle[4], "WP_PceFilterSetCreate  level 0 set 5");
   APP_PRINTF("\n---- Level 0 Filter Set Created: 0x%X ----------\n", (WP_U32) dfilter_set_lvl0_handle[4]);
#endif
}

static void WPE_CreatePceDataTestRulesFiltersSets(void)
{
   WP_pce_if_params_pkt_rx_channel pce_if_params;
   //WP_pce_if_params_iw_tx_channel pce_iw_tx_if_params;
   WP_handle pce_if_handle2;//, pce_if_handle_pcea;
   WP_status status;

   WPE_CreatePceFilters();
   WPE_CreatePceFilterSets();
   WPE_CreatePceRules();

   memset(&pce_if_params, 0, sizeof(pce_if_params));
   pce_if_params.mode = WP_PCE_IW_PORT_CONNECTION_ENABLED;
   pce_if_params.parser_start_type = WP_PCE_PARSER_START_TYPE_ETHERNET;
   pce_if_params.filter_set_handle = dfilter_set_lvl0_handle[0];
   pce_if_params.ip_header_validation = WP_DISABLE;
   pce_if_params.sub_port_vlan = WP_DISABLE;
   pce_if_params.collect_tpids = WP_DISABLE;
   pce_if_handle2 = WP_PceInterfaceCreate(WP_PCE_IF_TYPE_PKT_RX_CHANNEL, &pce_if_params);
   terminate_on_error(pce_if_handle2, "WP_PceInterfaceCreate");
   rx_binding_cfg[0].pce_if_handle = pce_if_handle2;

   WPI_TEST_CHECK_RESULT(status,
                         WP_OK,
                         WP_IwRxBindingModify(rx_channel_enet_handle1, 
                                              iw_system_bridging_handle, /*iw_system_routing_handle*/
                                              qniw_enet, 
                                              WP_IW_RX_BIND_MOD_PCE_INTERFACE, 
                                              &(rx_binding_cfg[0])));
   terminate_on_error(status,"WP_IwRxBindingModify");

#if 0
   rx_binding_cfg->pce_if_handle = pce_if_handle2;
   WPI_TEST_CHECK_RESULT(status,
                         WP_OK,
                         WP_IwRxBindingModify(rx_channel_enet_handle4, 
                                              iw_system_bridging_handle, 
                                              qniw_enet, 
                                              WP_IW_RX_BIND_MOD_PCE_INTERFACE, 
                                              rx_binding_cfg));
   terminate_on_error(status,"WP_IwRxBindingModify");

   pce_iw_tx_if_params.mode = WP_PCE_IW_PORT_CONNECTION_ENABLED;
   pce_iw_tx_if_params.parser_start_type = WP_PCE_PARSER_START_TYPE_ETHERNET;
   pce_iw_tx_if_params.filter_set_handle = dfilter_set_lvl0_handle[0];
   pce_iw_tx_if_params.ip_header_validation = WP_DISABLE;

   pce_if_handle_pcea = WP_PceInterfaceCreate(WP_PCE_IF_TYPE_IW_TX_CHANNEL, &pce_iw_tx_if_params);
   terminate_on_error(pce_if_handle_pcea,"WP_PceInterfaceCreate");

   iw_tx_ch_cfg->pce_if_handle = pce_if_handle_pcea;
   
   WPI_TEST_CHECK_RESULT(status,
                         WP_OK,
                         WP_ChannelModify(tx_host_bp_handle, 
                                          WP_HOST_CH_MOD_PCE_INTERFACE,
                                          iw_tx_ch_cfg));
   terminate_on_error(status, "WP_ChannelModify");

   WPI_TEST_CHECK_RESULT(status,
                         WP_OK,
                         WP_ChannelModify(tx_host_rp_handle, 
                                          WP_HOST_CH_MOD_PCE_INTERFACE,
                                          iw_tx_ch_cfg));
   terminate_on_error(status, "WP_ChannelModify");
 #endif
}

static WP_U8 WPE_SendReceivePackets(WP_U32 scenario)
{
   WP_handle tx_channel;
   tx_channel = tx_host_handle11 = tx_host_bp_handle;

   if (scenario & 0x1)
      if(FALSE == WPE_DataUnitSendReceive( "FFFFFFFFFFFF",  // Accept
                                           "040101010101",
                                           "000B",
                                           "", "12121212",
                                           "ABCDEF01",
                                           tx_channel,
                                           rx_channel_enet_handle2)) // PCE filter 1 accept packet to policer
         return  FALSE;

   if (scenario & 0x2)
      if(FALSE == WPE_DataUnitSendReceive( "010000010203", // Accept 
                                           "040101010101",
                                           "000B", "",
                                           "12121212",
                                           "ABCDEF01",
                                           tx_channel,
                                           rx_channel_enet_handle2)) // PCE filter 1 accept packet to policer 
         return  FALSE;
   
   if (scenario & 0x4)
      if(FALSE == WPE_DataUnitSendReceive( "020101010303", // deny
                                           "040101010101",
                                           "000A", "",
                                           "C0A86464",    
                                           "ABCDEF01",
                                           tx_channel, 
                                           rx_channel_enet_handle2)) 
         return  FALSE;
#if 0
   if (scenario & 0x8)
      if(FALSE == WPE_DataUnitSendReceive( "020101010303", // Continue
                                           "040101010101",
                                           "000A", "",
                                           "C0A86465",     // Accept
                                           "ABCDEF01",
                                           tx_channel, 
                                           rx_channel_enet_handle3)) // PCE filter 1 continue- filter 2 accept packet to flow_aggregation3
         return  FALSE;
   
   if (scenario & 0x10)
      if(FALSE == WPE_DataUnitSendReceive( "020101010303", // Continue
                                           "060101010101", 
                                           "000A", "",
                                           "C0A86466",     // Continue
                                           "80A86464",     // Continue
                                           tx_channel, 
                                           rx_channel_enet_handle3)) // PCE filter 1 & 2 continue. 
                                                                     // Filter 4 FS change to FS 7.
                                                                     // Filter 22 accept ethernet type to flow_aggregation3
         return  FALSE;

   if (scenario & 0x20)
      if(FALSE == WPE_DataUnitSendReceive( "020101010303", // Continue F1
                                           "060101010103", // 
                                           "000C", "",     // Accept   F8
                                           "C0A86466",     // Continue F2
                                           "80A86466",     // Continue F4
                                           tx_channel,
                                           rx_channel_enet_handle3)) // PCE filter 1, 2 & 4 continue. 
                                                                     // Filter 8 accept to flow_aggregation3
         return  FALSE;

   if (scenario & 0x40)
      if(FALSE == WPE_DataUnitSendReceive( "020101010303", // Continue F1
                                           "060101010103", // Accept   F20
                                           "000A", "",     // Continue F8
                                           "C0A86466",     // Continue F2
                                           "80A86466",     // Continue F4
                                           tx_channel,
                                           rx_channel_enet_handle3)) // PCE filter 1, 2, 4 & 8 continue. 
                                                                     // Filter 20 accept to flow_aggregation3
         return  FALSE;

   if (scenario & 0x80)
      if(FALSE == WPE_DataUnitSendReceive( "020101010303", // Continue F1
                                           "060101010104", // F20- FS jump to FS10. F30 Accept Remarked mac SA 
                                           "000B", "",     // Continue + change mac SA to 06...03 F8 
                                           "C0A86466",     // Continue F2
                                           "80A86466",     // Continue F4
                                           tx_channel,
                                           rx_channel_enet_handle3)) // PCE filter 1, 2, 4 & 8 continue.
                                                                     // Filter 8 result mac sa remark.
         return  FALSE;                                              // Filter 20 jump to FS10.
                                                                     // Filter 30 accept
   return TRUE;
#endif
   return TRUE;
}

static void WPE_DisplayStats(void)
{
   WP_U32 i;
   WP_CHAR ch;
   
   /* Print the statistics */
   do
   {
      printf("\n\n*********Statistics Menu*********");
      printf("\n1) Enet Port & Device & IwPort1 & FA1 statistics");
      printf("\n2) Enet Port & Device & IwPort2 & FA2 statistics");
      //printf("\n3) Enet Port & Device & FA 3 statistics");
     // printf("\n4) Enet Port & Device 4 statistics");
      printf("\n5) Host bridge port statistics");
      //printf("\n6) Host routing port statistics");
      //printf("\n7) FA 7 & IW Port 7 statistics");
      printf("\n8) Filters statistics");
      printf("\n9) Policer statistics");   
      printf("\na) Clear Policer statistics");     
      printf("\nM) Main Menu\n");
      
      ch = getchar();
      if(ch == '1')
      {
         WPT_DisplayFlowStats(flow_aggregation1, "Bridge Flow Agg 1");
         WTE_DisplayEnetDeviceStats(enetdev1, "ENET device 1");
         WPT_DisplayBPortStats(bport_iw1, "Enet bport 1");
      }
      
      else if(ch == '2')
      {
         WPT_DisplayGenericFlowStats(flow_aggregation2, "Bridge Flow Agg 2");
         WTE_DisplayEnetDeviceStats(enetdev2, "ENET device 2");
         WPT_DisplayBPortStats(bport_iw2, "Enet bport 2");
      }
  #if 0    
      else if(ch == '3')
      {
         WPT_DisplayGenericFlowStats(flow_aggregation3, "Generic Flow Agg 3");
         WTE_DisplayEnetDeviceStats(enetdev3, "ENET device 3");
         WPT_DisplayRoutePortStats(rport_iw3, "Enet rport 3");
      }
      
      else if(ch == '4')
      {
         WTE_DisplayEnetDeviceStats(enetdev4, "ENET device 4");
         WPT_DisplayBPortStats(bport_iw1, "Enet bport 4");
      }
  #endif    
      else if(ch == '5')
      {
         WPT_DisplayBPortStats(bport_iw_host, "iw port bridging host");
      }
   #if 0   
      else if(ch == '6')
      {
         WPT_DisplayRoutePortStats(rport_iw_host, "iw port routing host");
      }

      else if(ch == '7')
      {
         WPT_DisplayGenericFlowStats(flow_aggregation7, "Generic Flow Agg 7");
         WPT_DisplayRoutePortStats(rport_iw7, "Enet rport 7");
         WPT_DisplayFlowStats(flow_aggregation_gcsi, "Routing Flow Agg GCSI");
      }
    #endif   
      else if(ch == '8')
      {
         for (i = 0; i < PCE_MAX_FILTERS; i++)
         {
            if (dfilter[i])
               WPE_PrintPceFilterStats(dfilter[i]);
         }
      }
      else if(ch == '9')
      {
               WPE_PrintPolicerStatistics();
      }
      else if(ch == 'a')
      {
               WPE_ResetPolicerStatistics();
      }
      
   }
   while (ch!='m' && ch!='M');
   
}

static WP_U8 WPE_PceDataTest(void)
{
   WP_U8 result = 0;
   WP_CHAR ch;
   WP_U32 scenario;
   
   WPE_CreatePceDataTestRulesFiltersSets();
   WPE_EnablePortsDevicesChannels();/* Enable the system */

  do
  {
     printf("\n\n*********Main Menu*********");
     printf("\n1) Send test packet 1(DMAC=FF-FF-FF-FF-FF-FF)");
     printf("\n2) Send test packet 2(DMAC=01-00-00-01-02-03)");
     printf("\n3) Send test packet 3(DMAC=02-00-00-01-02-03)");
     //printf("\n4) Send test packet 4");
     //printf("\n5) Send test packet 5");
     //printf("\n6) Send test packet 6");
     //printf("\n7) Send test packet 7");
     //printf("\n8) Send test packet 8");
     printf("\nA) Send all test packets");
     printf("\nS) Switch to Statistics menu");
     printf("\nD) Utilities and Debug");
     printf("\nQ) Quit the program\n");
     
    ch = getchar();
    scenario  = 1;
    
    if(ch == 's' || ch == 'S')
    {
       WPE_DisplayStats();
    }
    else if (ch == 'd' || ch == 'D')
    {
       WT_UtilitiesMenu();
    }
    else if(ch > '0' && ch < '4')
    {
       scenario = scenario << ((WP_U32)ch - 49);
       result = WPE_SendReceivePackets(scenario);
    }
    else if (ch == 'a' || ch == 'A')
    {
       result = WPE_SendReceivePackets(0xFFFF);
    }
  }
  while (ch!='q' && ch!='Q');

  return result;
}

/////////////////////////////////////////////////
// 
// logCallBack
// 
/////////////////////////////////////////////////

#ifdef VERILOG
static WP_U8 logTraceLevel = DEBUG_LEVEL_10;
#else
#ifdef DEBUG_PRINT
static WP_U8 logTraceLevel = DEBUG_LEVEL_6;
#else
static WP_U8 logTraceLevel = DEBUG_LEVEL_0;
#endif
#endif

WP_status logCallBack(WP_S32 level, const WP_CHAR *message)
{
   if(level <= logTraceLevel)//WPI_DEBUG_LEVEL_5
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

//////////////////////////////////////////////////////////////////////////////
// 
// Main Function
// 
//////////////////////////////////////////////////////////////////////////////

WP_S32 main(WP_S32 argc, WP_CHAR **argv)
{
   WP_status status;
   WP_U8 result;

   WPE_init();

   pce_init.hardware_rules_num = PCE_MAX_RULES;
   pce_init.filters_num = PCE_MAX_FILTERS;
   pce_init.pkt_rx_channel_interfaces_num = 10;
   pce_init.iw_tx_channel_interfaces_num = 5;
   pce_init.pce_discard_mode = WP_PCE_DISCARD_MODE_ALL;
   pce_init.bridge_loop_prevention_mode = WP_ENABLE;

   status = WP_ModuleInit(WP_SYSHANDLE(0), WP_WDDI_MODULE_PCE, &pce_init);
   terminate_on_error(status, "WP_PceModuleInit()");

   status = WP_ModuleInit(WP_SYSHANDLE(0), WP_WDDI_MODULE_POLICER, &policer_module_config); 
   terminate_on_error(status, "WPI_PolicerModuleInit()");

   WP_ControlRegister(WP_DEBUG_CALLBACK_WDDI_LOG, logCallBack);
   WP_ControlRegister(WP_DEBUG_CALLBACK_FILE_LINE_ERROR, WPE_DebugCallback);

   status = WPX_BoardConfigure(0, WPX_CONFIGURE_DEFAULT_CHECKIN);
   terminate_on_error(status, "WPX_BoardConfigure()");

   WPE_Create_pools();
   WPE_Create_ports();

   status = WP_SysCommit();
   terminate_on_error(status, "WP_SysCommit()");

   WPE_CreateIwSystemAndItsComponents();
   
   result = WPE_PceDataTest();

   if (wpe_test_failures == 0 && TRUE == result)
   {
      printf ("Test Passed \n");
   }
   else
   {
      if (TRUE != result)
         printf ("Data Test Failure\n");
      if (wpe_test_failures != 0)
         printf ("Errors Num = %d\n", wpe_test_failures);
      printf("Test Failed \n");
      return 0;
   }

   WPE_Terminate(0);

   return 0;
} /* main */


#include "wt_pce_misc.c"


