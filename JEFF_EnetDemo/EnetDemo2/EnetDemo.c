/*******************************************************************************
 * (C) Copyright 2001-2010, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ************************************************************/

/*******************************************************************************
 *
 * Module:
 * Purpose:
 *
 * Change History:
 * +------------+--------------------+------------------------------------------
 * | Date       | By                 | Description
 * +------------+--------------------+------------------------------------------
 * | 25/11/2009 | Alexander Rahlis   | Clean, Modular WDDI 4.0 Test
 * One side is GEnet (FastFlow) the other side is GEnet (HierarchicalFlow)
 * bouth sides use only one Phy.
 * +------------+--------------------+------------------------------------------
 * |            |                    |
 * +------------+--------------------+------------------------------------------
 *
 ******************************************************************************/

/*
  ------- Rules:
  No ifdefs where alternative configuration can be put in comments
  No [1] structures and no double {{}}
  All config structures should end with _config
  use spaces between parameters in functions

  ------- Todo:
// Add policer Stats

------- Requirements:
Bridging/Routing <- later
FlowAgg DirectMapped <-
Learning
Policer FlowAgg
Scheduling & Shaping:
DFC: EMC+MFC
Many DENY/ACCEPT filter and rules combinations
WP_flow_class_priority_action       priority_action;
WP_flow_class_policer_action        policer_action;
WP_flow_class_ext_action            ext_action;
Multicast
PQBlock/VPMT
MQBlock [ASU only] <- later
Full CLI Statistics
*/

/********************************************************************************
 ***                                 INCLUDES                                 ***
 *******************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>


#include "api/wp_wddi.h"
#include "api/wp_host_io.h"
// #include "wpi_veneer.h"
#include "wpx_app_data.h"
#include "wpx_board_if.h"
#include "wpx_board_data.h"
#include "wpl_platform.h"
#include "wp_bus.h"
#include "wpl_locks.h"



#include "AlexR_3.0_Statistics.c"

#include "wufe_errors.h"

#include "config.h"
#include "map.h"
#include "api/wp_wddi.h"
#include "api/wp_host_io.h"
#include "wp_sim.h"
#include "external.h"

/********************************************************************************
 ***                                 DEFINES                                  ***
 *******************************************************************************/

#undef LOCK_AT_START
#define USE_SIMPLE_DEVICE	(0)
#define WPL_THREAD_LOCK_KEY \
   WPL_LOCK_KEY_CREATE(WPL_HW_LOCK, WPL_PRIVATE_LOCK, 7, 0)
#define DELAY_COUNT	(10000 * 100)	// micro seconds
#define ENABLE_TRANSFER          (0)
#define MAX_MACS                 4
#define N_QNODES                 3
#define N_POOLS                  4
#define MAXT_SIZE                255

#if 0
#define MTU_SIZE                 1536
#define SDU_SIZE                 2048  // Must be > MTU_SIZE + 48
#else
#define MTU_SIZE_STD		 1500
#define MTU_SIZE                 (MTU_SIZE_STD + 1024)
#define SDU_SIZE                 (MTU_SIZE + 64)// Must be > MTU_SIZE + 48
#define WEIGHT_SIZE		(64 * 128/ SDU_SIZE  - 1)
#endif

#define PACKET_SIZE              2500
#define CIR_EIR_RATE		(1024 * 100 * 8)

// Main Defines
#define USE_SW_CHANNELS 0

#define NUM_OF_L1_GROUP 1
#define NUM_OF_L2_GROUP 4

#define NUM_OF_HIER_ENET_FAS_PER_L2 8  // Currently only 8 channel per L2, which is maximum only in HW channels mode
#define NUM_OF_HIER_ENET_FAS     (NUM_OF_HIER_ENET_FAS_PER_L2*NUM_OF_L2_GROUP)
#define NUM_OF_FAST_ENET_FAS     1
#define MAX_VLAN_IDS             NUM_OF_HIER_ENET_FAS
#define NUM_OF_IW_BPORTS         (1+1+1+1)   /* 1 MC BP + 1 Host BP + 1 Enet BP + 1 Hierarchical Enet BP */
#define MAX_IW_FLOWS             (MAX_VLAN_IDS*NUM_OF_IW_BPORTS)  /* Max combinations of VLAN+BP = Max Entries in Memberset table = */
        /* Max possible IwFlowAdd WP_FLOW_BRIDGEPATH */
// PQBlock
#if PQBLOCK
#define NUM_OF_CHANNELS_IN_PQ_BLOCK  4
#else /*  */
#define NUM_OF_CHANNELS_IN_PQ_BLOCK  1
#endif /*  */

// Channels
#define NUM_OF_ALL_L2HOST_CHANNELS    2   // 2 Enet
#define NUM_OF_FAST_ENET_TX_CHANNELS  NUM_OF_FAST_ENET_FAS
#define NUM_OF_FAST_ENET_RX_CHANNELS  1   // Must be always 1 per PHY
#define NUM_OF_HIER_ENET_TX_CHANNELS  NUM_OF_HIER_ENET_FAS
#define NUM_OF_HIER_ENET_RX_CHANNELS  1   // Must be always 1 per PHY
#define NUM_OF_ALL_HIER_ENET_CHANNELS (NUM_OF_HIER_ENET_TX_CHANNELS + NUM_OF_HIER_ENET_RX_CHANNELS)
#define NUM_OF_ALL_CHANNELS           (2+NUM_OF_FAST_ENET_TX_CHANNELS+NUM_OF_FAST_ENET_RX_CHANNELS+NUM_OF_ALL_HIER_ENET_CHANNELS)   // 2 IW Host +All Fast + All Hier

// Learning
#define LEARNING_QUEUE_SIZE         100

// DFC
#define NUM_OF_RULES_PER_FILTER     2
#define NUM_OF_EMC_FILTERS          1
#define WP_IW_RULE_MAX_LENGTH       300

// Optional Functionality on/off defines
#define MFC         0           /* 1- with MFC , 0 - without MFC */

// Unimportant Defines
#define BRIDGE_PORT_ENET_TAG        12
#define BRIDGE_PORT_HOST_TAG        13
#define BRIDGE_PORT_HIER_ENET_TAG   14
#define BRIDGE_PORT_MC_TAG          15

#define VLAN_TAG_1                  1
#define VLAN_TAG_2                  2 

// Multicast
#define NUM_OF_MC_GROUPS            1  // Must be ALWAYS 1 for this Test
#define NUM_OF_MC_MEMBERS_PER_GROUP NUM_OF_HIER_ENET_FAS

#define KEY_BACKSPACE           8
#define KEY_ENTER               10

/********************************************************************************
 ***                               Global Variables                           ***
 *******************************************************************************/

   WP_enet_filters my_rx_filters = {

      /* rx_ucfilter; */ WP_ENET_FILTER_PROCESS,
      /* rx_bcfilter; */ WP_ENET_FILTER_PROCESS,
      /* rx_mcfilter; */ WP_ENET_FILTER_PROCESS
   };

   WP_device_enet_ex my_enet_dev_ext_params_config = {

      /* duplexmode; */ WP_ENET_FULLDUPLEX,
      /* rx_maclookup_mode */ WP_DISABLE,
      /* rx_flowcontrol; */ WP_ENET_FLOW_ENABLE,
      /* tx_flowcontrol; */ WP_ENET_FLOW_ENABLE,
      /* rx_addrmode; */ WP_ENET_ADDR_ACCEPTALL,
      /* phystatmode; */ WP_ENET_STAT_ENABLE,
      /* tx_statmode; */ WP_ENET_STAT_ENABLE,
      /* rx_statmode; */ WP_ENET_STAT_ENABLE,
      /* tx_duplicate; */ WP_DISABLE,
      /* rx_filters; */ &my_rx_filters,
      /* rx_timestamp; */ 0
   };

   WP_device_enet my_enet_dev_config = {

      /* max_tx_channels */ 1,
      /* tx_maxsdu */ SDU_SIZE,
      /* operating_speed */ WP_UNUSED,
      // Valid for RMII, RGMII only
      /* mac_addr */
      {
       0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa}, // MAC of this device
      /* tx_bit_rate */ 1000000000,
      /* loopbackmode */ WP_ENET_NORMAL,
#if 0
      /* extended_params */ NULL
#else
	&my_enet_dev_ext_params_config 
#endif
         // Must be NULL at device creation
   };







WP_handle iw_sys;
WP_handle iw_qnode, qnode_host, qnode_mc;
WP_handle adjunct_buffer_pool, pool_host, pool_ring_host;
WP_handle agg_tag_counter = 0;
WP_handle port_host, dev_host, bport_host, agg_host, rx_host_channel,
   tx_host_channel;
WP_handle port_enet, port_enet2, dev_enet, dev_enet2, bport_enet, agg_enet_change_mac,
   agg_enet[NUM_OF_FAST_ENET_FAS], rx_gbe_channel,
   tx_gbe_channel[NUM_OF_FAST_ENET_TX_CHANNELS],
rx_gbe_channel2,
   tx_gbe_channel2[NUM_OF_FAST_ENET_TX_CHANNELS];
WP_handle port_hier_enet, dev_hier_enet, rx_gbe_hier_channel;

WP_handle
   agg_hier_enet[NUM_OF_L1_GROUP][NUM_OF_L2_GROUP]
   [NUM_OF_HIER_ENET_FAS_PER_L2], bport_hier_enet;
WP_handle
   tx_gbe_hier_channel[NUM_OF_L1_GROUP][NUM_OF_L2_GROUP]
   [NUM_OF_HIER_ENET_FAS_PER_L2];
WP_handle mc_group_handle[NUM_OF_MC_GROUPS],
   mc_agg_handle[NUM_OF_MC_GROUPS], mc_dummy_bport;
WP_handle mc_member_handle[NUM_OF_MC_GROUPS * NUM_OF_MC_MEMBERS_PER_GROUP];
WP_handle flow_agg_dedicated[3];
WP_handle emc_filter_handle[NUM_OF_EMC_FILTERS], mfc_filter_handle;
WP_handle flow_rule_handle[NUM_OF_EMC_FILTERS +
                           1][NUM_OF_RULES_PER_FILTER];
WP_handle l1_group_hier[NUM_OF_L1_GROUP];
WP_handle l2_group_hier[NUM_OF_L1_GROUP][NUM_OF_L2_GROUP];
WP_handle l2_shaping_block, l1_shaping_block;
WP_U8 enet_dst_mac[6] = {
   0x00, 0x00, 0x00, 0x0A, 0x00, 0x00
};

#if 1
WP_U8 enet_bidirect_dst_mac[6] = {
   0x00, 0x00, 0x00, 0x05, 0x01, 0x00
};
#endif


WP_U8 enet_change_dst_mac[6] = {
   0x00, 0x00, 0x00, 0x0A, 0x01, 0x00
};

WP_U8 hier_enet_dst_mac[6] = {
   0x00, 0x00, 0x00, 0x0B, 0x00, 0x00
};

WP_U8 hier_enet_change_dst_mac[6] = {
   0x00, 0x00, 0x00, 0x0B, 0x01, 0x00
};

WP_U8 hier_enet_mcast_dst_mac[6] = {
   0x10, 0x00, 0x00, 0x0B, 0x00, 0x00
};

WP_U8 host_dst_mac[6] = {
   0x00, 0x00, 0x00, 0x0C, 0x00, 0x00
};

/********************************************************************************
 ***                          Functions Declarations                          ***
 *******************************************************************************/
int get_line(char *buf);

// General
void WPE_SysInit (void);
void WPE_InitHWCards (void);
void WPE_CreateIWQnode (void);
void WPE_CreateHostQnode (void);
void WPE_CreateMultiCastQnode (void);
void WPE_CreateBufferPools (void);
void WPE_CreateIWBridgeSystem (void);
void WPE_EnableSystem (void);

// Host Side
void WPE_CreateHostPortDevice (void);
void WPE_CreateHostIwRxChannel (void);
void WPE_CreateHostIwTxChannel (void);
void WPE_CreateIwBportHost (void);
void WPE_CreateHostTermFlowAgg (void);

// Enet Side (Fast)
void WPE_CreateFastEnetPortDevice (void);
void WPE_CreateFastEnetIwBport (void);
void WPE_CreateFastEnetFlowAgg (void);
void WPE_CreateFastEnetRxTxChannel (void);
void WPE_CreateFastEnetRxTxBinding (void);

// Enet Side (Hier)
void WPE_CreateHierEnetPortDevice (void);
void WPE_CreateHierEnetIwBport (void);
void WPE_CreateHierEnetFlowAgg (void);
void WPE_CreateHierHWEnetRxTxChannel (void);
void WPE_CreateHierEnetRxTxBinding (void);

//FMU Functionality
void WPE_CreateL1FMUGroups (void);
void WPE_CreateL2FMUGroups (void);

// Optional Functionality
// Multicast
void WPE_MultiCastFlowCreate (void);
void WPE_MultiCastGroupCreate (void);
void WPE_MultiCastMembersFill (void);
void WPE_MultiCastFlowAggCreate (void);
void WPE_CreateDummyBportMultiCast (void);

// Learning & Forwarding
void WPE_SetStaticForwardRules (void);
void WPE_LearningIST (WP_tag tag, WP_U32 data, WP_U32 info);
void WPE_MemeberSetFill (void);
void WPE_ShowForwardingRules (void);
void WPE_DisplayForwardingRule (WP_U32 * l2_forward_rule);

// Classification
void WPE_CreateDedicatedFlowAgg (void);
void WPE_AddBasicFilters (void);
void WPE_AddBasicRules (void);

// Shaping & Scheduling & Policer
void WPE_CreateSchedulingUnit (void);
void WPE_PolicerInit (void);

// Auxillary Functions
void WPE_CLI (void);
void WPE_ShowAvailableBusesMemory (void);
void WPE_RegisterLogCbFunc (void);
void terminate_on_error (WP_handle handle, WP_CHAR * s);

// Host Send/Receive
void WPE_Receive_HostData (WP_handle rx_channel, WP_data_type data_type);
static void WPE_Send_HostData (WP_handle tx_channel, WP_data_type data_type,
                        WP_U8 dst_mac[]);
void WPE_Receive_HostData_IRQ (WP_tag tag, WP_U32 event, WP_U32 info);
void WPE_IWSendReceive (WP_U32 packets, WP_U8 dst_mac[]);
void WPE_L2SendReceiveEnet (WP_U32 packets, WP_U8 dst_mac[]);
void WPE_L2SendReceiveUPI (WP_U32 packets, WP_U8 dst_mac[]);
void WPE_Receive_HostData_IRQ_X (WP_tag tag, WP_U32 event, WP_U32 info);
static WP_tag g_tag;
static WP_U32 g_event;
static WP_U32 g_info;
static WP_U32 g_flag = 0;
static WP_U32 g_threadStop = 1;
static WP_U32 jjj = SECONDS_TO_WAIT;

void app_overrun_callback (WP_U32 wpid, WP_U32 queue_id, WP_U32 overrun_count)
{
   printf ("Overrun happenes: wpid(%2d), queue_id(%2d), overrun_cnt(%6d)", 
      wpid, queue_id, overrun_count);

   return;
}

static WP_U32 iii = 0;
extern void usleep(WP_U32 period);
WP_U32 g_callback = 0;

WP_U32      eoam_lock;
#define WPL_THREAD_LOCK_KEY WPL_LOCK_KEY_CREATE(WPL_HW_LOCK, WPL_PRIVATE_LOCK,         7, 0)

WP_U32 interface_mode = 0;

void *LearningPoll(void *i);

void init_vars (void)
{
	map_address_reg[WP_PORT_ENET1].tcntr = 0;
	map_address_reg[WP_PORT_ENET2].tcntr = 0;
	map_address_reg[WP_PORT_ENET3].tcntr = 0;
	map_address_reg[WP_PORT_ENET4].tcntr = MAP_SERDES3_IO_CTRL_3;
	map_address_reg[WP_PORT_ENET5].tcntr = 0;
	map_address_reg[WP_PORT_ENET6].tcntr = 0;
	map_address_reg[WP_PORT_ENET7].tcntr = 0;
	map_address_reg[WP_PORT_ENET8].tcntr = 0;
	map_address_reg[WP_PORT_ENET9].tcntr = 0;
	map_address_reg[WP_PORT_ENET10].tcntr = 0;
	map_address_reg[WP_PORT_ENET11].tcntr = 0;
	map_address_reg[WP_PORT_ENET12].tcntr = 0;


	map_address_reg[WP_PORT_ENET1].serdes_io_ctrl = 0;
	map_address_reg[WP_PORT_ENET2].serdes_io_ctrl = 0;
	map_address_reg[WP_PORT_ENET3].serdes_io_ctrl = 0;
	map_address_reg[WP_PORT_ENET4].serdes_io_ctrl = MAP_SERDES3_IO_CTRL_3;
	map_address_reg[WP_PORT_ENET5].serdes_io_ctrl = MAP_SERDES3_IO_CTRL_2;
	map_address_reg[WP_PORT_ENET6].serdes_io_ctrl = 0;
	map_address_reg[WP_PORT_ENET7].serdes_io_ctrl = MAP_SERDES1_IO_CTRL_1;
	map_address_reg[WP_PORT_ENET8].serdes_io_ctrl = MAP_SERDES0_IO_CTRL_1;
	map_address_reg[WP_PORT_ENET9].serdes_io_ctrl = 0;
	map_address_reg[WP_PORT_ENET10].serdes_io_ctrl = 0;
	map_address_reg[WP_PORT_ENET11].serdes_io_ctrl = 0;
	map_address_reg[WP_PORT_ENET12].serdes_io_ctrl = 0;
}

/********************************************************************************
 ***                        Functions Implementations: Main                   ***
 *******************************************************************************/
int main (int argc, WP_CHAR ** argv)
{
   WP_THREAD_ID learning_thread_id;
   WP_handle status;
   WP_U32 i;

   WPE_RegisterLogCbFunc ();

//----- Basic Functionality POS+Enet+Host ----//
   status = WP_DriverInit ();
   terminate_on_error (status, "WP_DriverInit()");
   WPE_SysInit ();
   WPE_CreateIWBridgeSystem ();
   WPE_InitHWCards ();
   WPE_CreateBufferPools ();

   //WPE_PolicerInit();
   //WPE_CreateSchedulingUnit();
   WPE_CreateFastEnetPortDevice (); 
   WPE_CreateHierEnetPortDevice ();
   WPE_CreateHostPortDevice (); 
   status = WP_SysCommit ();   
   terminate_on_error (status, "WP_SysCommit");

   /*
    * the following 2 lines of creating shaping group is written according to the code above, 
    * because in WPE_CreateFast/HierEnetPortDevice () using paramter 'WP_ENET_FMU_HIERARCHICAL_SHAPING_MODE', 
    * we must create shaping group here now !!! it is required.
    */
   WPE_CreateL1FMUGroups ();  
   WPE_CreateL2FMUGroups ();   
   WPE_CreateHostIwRxChannel ();
   WPE_CreateHostTermFlowAgg (); 
   WPE_CreateIwBportHost ();    
   WPE_CreateHostIwTxChannel ();
   WPE_CreateFastEnetIwBport (); 
   WPE_CreateFastEnetRxTxChannel (); 
   WPE_CreateFastEnetRxTxBinding (); 
   WPE_CreateFastEnetFlowAgg (); 
   WPE_CreateHierEnetIwBport ();
   WPE_CreateHierHWEnetRxTxChannel (); 
   WPE_CreateHierEnetRxTxBinding ();  
   WPE_CreateHierEnetFlowAgg ();

//-------- Optional Functionality ---------//
   // Register All Interrrupts (use WPI_SimulateInterrupts under Winmon)
   status =
      WP_ControlRegister (WP_FORWARD_RULE_FIND_ALL,
                          &WPE_DisplayForwardingRule);
   terminate_on_error (status,
                       "WP_ControlRegister WP_FORWARD_RULE_FIND_ALL ");

   //status = WP_ControlRegister(WP_EVENT_IW_LEARNING_QUEUE, &WPE_LearningIST);
   //terminate_on_error(status, "WP_ControlRegister WP_EVENT_IW_LEARNING_QUEUE");



      /*
       * receive signal
       */
#if 1
      // callback
      status = WP_ControlRegister(WP_EVENT_RX_INDICATE, 
                                       &WPE_Receive_HostData_IRQ_X);
      terminate_on_error(status, "WP_ControlRegister WP_EVENT_RX_INDICATE ");

      status = WP_ControlRegister(WP_EVENT_QUEUE_OVERRUN, 
                                       &app_overrun_callback);
      terminate_on_error(status, "WP_ControlRegister WP_EVENT_QUEUE_OVERRUN");

#endif

   //WPE_CreateDedicatedFlowAgg();

   //WPE_MultiCastFlowCreate();

   /*---------------------------------------------------*\
		IMPORTANT !
   \*---------------------------------------------------*/
#if 1
   WPE_SetStaticForwardRules ();
#endif

   //WPE_AddBasicFilters();
   //WPE_AddBasicRules();
//--------------------------------------//
   status = WP_IwSystemBuild (iw_sys);
   terminate_on_error (status, "WP_IwSystemBuild");
   WPE_EnableSystem ();


#if 1
	printf ("before lock init\n");

	WPL_LockKeyInit (WPL_THREAD_LOCK_KEY, &eoam_lock);
	printf ("after lock init\n");


#ifdef LOCK_AT_START
	WPL_Lock(WPL_THREAD_LOCK_KEY, &eoam_lock);
#endif


	learning_thread_id = 0;
#if 1
	status = WPL_ThreadInit(&learning_thread_id, LearningPoll, 0);
	terminate_on_error (status , "WPL_ThreadInit() learning");
	printf ("after Threadinit\n");
#endif

#endif
	i = 0;

#if 0
   for (i = 0; i < 10; i++)

   {

      WPE_Send_HostData(tx_host_channel, WP_DATA_IW, 
                              /*enet_change_dst_mac*/enet_dst_mac);
      //WPE_IWSendReceive (1, enet_change_dst_mac);
   }
#endif
   WPE_CLI ();
   printf ("Test Passed \n");
   return 0;
}

/********************************************************************************
 ***                      Functions Implementations: General                  ***
 *******************************************************************************/
void WPE_SysInit (void)
{
   WP_handle status;
   extern WP_U32 dps_ProgramImage[];
   extern WP_U16 dps_PC_Table_Init[];

   WP_int_queue_table int_queue_tables = {
      {                         /*size, rate */

       { 10, 1},                 /* WP_IRQT0  highest */
       { 10, 1},                 /* WP_IRQT1 */
       { 10, 1},                 /* WP_IRQT2 */
       { 10, 1}                  /* WP_IRQT3  lowest */
       }
   };
   WP_iw_global max_iw = {

      /* max_iw_aggregations */ MAX_IW_FLOWS,
      // MAX_IW_AGG is always < MAX_IW_FLOWS since hierarchicalple flows can point to same FA
      /* max_routing_systems; */ 0,
      /* max_directmap_enet_systems */ 0,
      /* max_bridging_systems; */ 1,
      /* max_mpls_systems; */ 0,
      /* max_vlan_priority_maps; */ 1,
      /* iw_bkgnd_fifo_size */ 1000,
      /* cong_pt       */ NULL,
      /* iw_host_limits */
      {

       /* max_ht_rx_channels */ 1,
       /* pq_block_size */ 0,
       /* n_pq_blocks */ 0,
       /* max_ht_tx_channels */ 1,
       /* max_tx_groups */ 0
       },
      /* mpls_config */
      { 0, 0},
      /* iw_modes */
      {
	/* policer_mode */ WP_IW_POLICER_ENABLE,
  /* statistics_bundle_stat_mode */ WP_IW_STATISTICS_BUNDLE_STAT_ENABLE,
       /* l2_prefix_ext_mode; */ WP_IW_L2_HEADER_EXTENSION_DISABLE,
       /* enhanced_flow_stat_mode; */ WP_IW_ENHANCED_FLOW_STAT_ENABLE,
       /* flow_stat_mode; */ WP_IW_FLOW_STAT_ENABLE,
       /* fr_tx_flow_stat_mode */ WP_IW_FR_TX_FLOW_STAT_DISABLE,
       /* mfc_alloc_mode; */ WP_IW_MFC_ALLOC_ENABLE,
       /* learning_queue_mode */ WP_IW_LEARNING_ENABLE,
       /* port_filtering_mode */ WP_IW_PORT_FILTERING_ENABLE,
       /* lpm_alloc_mode */ WP_IW_LPM_ALLOC_DISABLE,
       /* enhanced_mc_member_stat_mode */ WP_DISABLE,
       /* routing_alloc_modes */ 0,
       /* res_mc_addresses_table_mode */ 0,
       /* port_stag_etype_mode */ 0,
       /* iw_characteristics */ 0
       },
      /* max_iw_stat_bundles */ 0,
      /* max_fr_sw_systems */ 0,
      /* max_iw_ports */ NUM_OF_IW_BPORTS,
      /* max_iw_mc_groups */ NUM_OF_MC_GROUPS,
      /* max_nat_systems */ 0,
      /* max_iw_second_aggregations */ 0,
      /* iw_comp_limits */ NULL,
      /* max_iw_mc_members */ NUM_OF_MC_GROUPS * NUM_OF_MC_MEMBERS_PER_GROUP,
      /* max_iw_programmable_filters */ 32,
      /* max_iw_policers */ 1,
      /* max_dscpmt; */ 0,
      /* max_wred_bindings; */ 0,
      /* tcm_policer_limits; */
      { 0, 0},
      /* max_flow_rule_stat_tables; */ 0,
      /* max_indirect_action_tables; */ 0,
      /* lpm_dfc_limits; */
      { 0, 0},
      /* brigding_time_stamp_info; */
      { 0, 0}
   };

// To Do: put NONE everywhere for non crucial masters
   WP_context context = {
      N_QNODES,
      /* the first four assignments are mandatory bus assignments */
      {

       {                        /* BUS_PARAM                */
        WP_BUS_PARAM, APP_BANK_PARAM},
       {                        /* BUS_PACKET               */
        WP_BUS_PACKET, APP_BANK_PACKET},
       {                        /* BUS_INTERNAL             */
        WP_BUS_INTERNAL, APP_BANK_INTERNAL},
       {                        /* BUS_HOST                 */
        WP_BUS_HOST, APP_BANK_HOST},
       {                        /* INTQUEUE_DEBUG           */
        WP_BUS_HOST, APP_BANK_HOST},
       {                        /* TIMESTAMP                */
        WP_BUS_HOST, APP_BANK_HOST},
       {                        /* CAS                      */
        WP_BUS_NONE, 0},
       {                        /* TRANS_TSAT               */
        WP_BUS_NONE, 0},
       {                        /* TPT                      */
        WP_BUS_INTERNAL, APP_BANK_INTERNAL},
       {                        /* CW                       */
        WP_BUS_INTERNAL, APP_BANK_INTERNAL},
       {                        /* ASU_TXQUEUE              */
        WP_BUS_PARAM, APP_BANK_PARAM},
       {                        /* STATISTICS               */
        WP_BUS_HOST, APP_BANK_HOST},
       {                        /* ATM_ADDLOOKUP            */
        WP_BUS_PARAM, APP_BANK_PARAM},
       {                        /* ATM_CPT_EBMT_EXTHEADER   */
        WP_BUS_PARAM, APP_BANK_PARAM},
       {                        /* ETH HDLC CPT             */
        WP_BUS_PARAM, APP_BANK_PARAM},
       {                        /* FBP BD                   */
        WP_BUS_PARAM, APP_BANK_PARAM},
       {                        /* FBP DATA                 */
        WP_BUS_PACKET, APP_BANK_PACKET},
       {                        /* L2_BKGND                 */
        WP_BUS_PACKET, APP_BANK_PACKET},
       {                        /* BRIDGE_LEARNING_DFC_PFMT */
        WP_BUS_PARAM, APP_BANK_PARAM},
       {                        /* IMA_MLPPP_STATE          */
        WP_BUS_INTERNAL, APP_BANK_INTERNAL},
       {                        /* IMA_MLPPP_BUFFER         */
        WP_BUS_PARAM, APP_BANK_PARAM},
       {                        /* HASH_IW_LPM_EMC          */
        WP_BUS_PACKET, APP_BANK_PACKET},
       {                        /* IW_MFC_RULES             */
        WP_BUS_PARAM, APP_BANK_PARAM},
       {                        /* FIWT_QAT                 */
        WP_BUS_PARAM, APP_BANK_PARAM},
       {                        /* MFCPT_DIFFSERV_UPPT_DFCT */
        WP_BUS_INTERNAL, APP_BANK_INTERNAL},
       {                        /* STATISTICS2              */
        WP_BUS_PACKET, APP_BANK_PACKET},
       {                        /* STATISTICS3              */
        WP_BUS_PACKET, APP_BANK_PACKET},
       {                        /* UNASSIGNED6              */
        WP_BUS_NONE, 0},
       {                        /* PKT_GLOBAL_TCPT          */
        WP_BUS_INTERNAL, APP_BANK_INTERNAL},
       {                        /* AAL2_QSR                 */
        WP_BUS_NONE, 0},
       {                        /* UNASSIGNED9              */
        WP_BUS_NONE, 0},
       {                        /* UNASSIGNED10             */
        WP_BUS_NONE, 0},
       /* ********************* VIRTUAL BUS ASSIGNMENT ******************** */
       {                        /* MCH_DATA                 */
        WP_BUS_PACKET, APP_BANK_PACKET},
       {                        /* OAM_FM_QUERY             */
        WP_BUS_NONE, 0},
       {                        /* HASH_ON_HEAP             */
        WP_BUS_NONE, 0},
       {                        /* MFC_INTERNAL             */
        WP_BUS_PACKET, APP_BANK_PACKET},
       {                        /* MFR_RAS_TBL              */
        WP_BUS_NONE, 0},
       {                        /* CESOP_DATA               */
        WP_BUS_NONE, 0},
       {                        /* WP_VB_SECURITY_ESE, */
        WP_BUS_NONE, 0},
       {                        /* WP_VB_PDCP_SECURITY_ENTITY, */
        WP_BUS_NONE, 0},
       {                        /* WP_VB_POLICER, */
        WP_BUS_NONE, 0},
       {                        /* WP_VB_PCE_LRN, */
        WP_BUS_NONE, 0},
       {                        /* WP_VB_PCEA_PARAM, */
        WP_BUS_NONE, 0},
       {                        /* WP_VB_CONGESTION_TABLE, */
        WP_BUS_NONE, 0},},
      {
       dps_ProgramImage, dps_ProgramImage},
      {
       dps_PC_Table_Init, dps_PC_Table_Init}, 
      &int_queue_tables,  /* Interrupt queue tables */
      N_POOLS,                  /* Maxumum number of pools */
      NULL,                     /* ATM global parameters */
      &max_iw,                  /* Interworking global parameters */
   };
   status = WP_SysInit (WP_WINPATH (0), &context);
   terminate_on_error (status, "WP_SysInit");
} 

void WPE_InitHWCards ()
{
   WP_handle status;

   status = WPX_BoardConfigure (0, WPX_CONFIGURE_2UPI_1XGI_10SGMII);
   terminate_on_error (status, "WPX_CONFIGURE_2UPI_1XGI_10SGMII()");

   status = WPX_BoardSerdesInit (0, EGRESS_PORT, WPX_SERDES_LOOPBACK);
   terminate_on_error (status, "WPX_BoardSerdesInit EGRESS()");

   status = WPX_BoardSerdesInit (0, EGRESS_PORT2, WPX_SERDES_NORMAL);
   terminate_on_error (status, "WPX_BoardSerdesInit EGRESS2()");

   status = WPX_BoardSerdesInit (0, INGRESS_PORT, WPX_SERDES_NORMAL);
   terminate_on_error (status, "WPX_BoardSerdesInit INGRESS()");
}

/*
 * we must create pool firstly, then create Qnode secondly.
 */
// Create IW (Non Ring) Pools 
void WPE_CreateIWQnode ()
{

   /* Host (Adjunct) pool part of IW QNode */
   WP_pool_buffer_data adjunct_buffer_config = {
      /* n_buffers */ 200,
      // Number of host threads that do Host Receive (1 BD per thread)
      /* offset */ 64,
#if 0
      /* size */ 2048,
#else
      /* size */ 2048,
#endif
      /* pad */ 0,
      /* databustype */ WP_BUS_HOST,
      /* bank */ APP_BANK_HOST
   };

   /* IW QNode (Free Buffer Pool part of IW QNode) */
   WP_qnode_iwq qn_iw_config = {
      // for IW TX channels only
      /* interruptqueue */ 0, 
      /* num_buffers */ MAXT_SIZE * (NUM_OF_HIER_ENET_TX_CHANNELS + 1 +
                   1) + (NUM_OF_HIER_ENET_RX_CHANNELS + 1 + 1),
      /* adjunct_pool */ 0
   };
   adjunct_buffer_pool =
      WP_PoolCreate (WP_WINPATH (0), WP_pool_iwbuffer,
                     &adjunct_buffer_config);
   terminate_on_error (adjunct_buffer_pool, "WP_PoolCreate()");
   qn_iw_config.adjunct_pool = adjunct_buffer_pool;
   iw_qnode =
#if 1
      WP_QNodeCreate (0,
                      WP_QNODE_IWQ |
                      WP_QNODE_OPT_DEDICATED_RX_HWQ |
                      WP_QNODE_OPT_FMU, &qn_iw_config);
#else
      WP_QNodeCreate (0, WP_QNODE_IWQ_JUMBO , &qn_iw_config);
#endif

#ifdef WP_HW_WINPATH2
#error fuck!!!
#endif

#ifdef WP_HW_WINPATH3
// #error !!!fuck!!!
#endif


   terminate_on_error (iw_qnode, "WP_QNodeCreate()");
} 


void WPE_CreateHostQnode ()
{
   WP_handle pool_host, pool_ring_host;
   WP_pool_buffer_data buffers_host_config = {  /* n_buffers */
      10 * NUM_OF_ALL_L2HOST_CHANNELS, // n_rings * ring_length
      /* offset */ 64,
      /* size */ 2048,
      /* pad */ 0,
      /* bus */ WP_BUS_HOST,
      /* bank */ APP_BANK_HOST
   };
   WP_pool_ring_data ring_host_config = {

      /* n_rings */ NUM_OF_ALL_L2HOST_CHANNELS,
      // Host Termination part of All RX channels + Non IW Host TX channels
      /* ring_length */ 10,
      // Max Numuber of BDs in each channel
      /* bus */ WP_BUS_HOST,
      /* bank */ APP_BANK_HOST
   };
   WP_qnode_hostq qn_host_config = {

      /* pool_buffer */ 0,
      /* pool_ring */ 0,
      /* interruptqueue */ WP_IRQT1,
   };

   // Create the buffers
   pool_host =
      WP_PoolCreate (WP_WINPATH (0), WP_pool_buffer, &buffers_host_config);
   terminate_on_error (pool_host, "WP_PoolCreate() pool_host");

   // Create the pointers to buffers - rings
   pool_ring_host =
      WP_PoolCreate (WP_WINPATH (0), WP_pool_ring, &ring_host_config);
   terminate_on_error (pool_ring_host, "WP_PoolCreate() pool_ring_host");
   qn_host_config.pool_buffer = pool_host;
   qn_host_config.pool_ring = pool_ring_host;
   qnode_host = WP_QNodeCreate (0, WP_QNODE_HOSTQ, &qn_host_config);
   terminate_on_error (qnode_host, "WP_QNodeCreate() qnode_host");
} 

void WPE_CreateMultiCastQnode ()
{
   WP_qnode_iw_mcq iw_mc_qn_config = {

      /* interruptqueue */ 0,
      /* num_buffers */
      2 * NUM_OF_MC_GROUPS * NUM_OF_MC_MEMBERS_PER_GROUP,
      /* offset */ 64,
      /* size */ 2048,
      /* pad */ 0
   };
   qnode_mc = WP_QNodeCreate (WP_WINPATH (0), WP_QNODE_IW_MCQ, /* WP_QNDE_IW_MCQ is the key point */
                              &iw_mc_qn_config);
   terminate_on_error (qnode_mc, "WP_QNodeCreate MCQ()");
} 


void WPE_CreateBufferPools ()
{
   WPE_CreateIWQnode ();
   WPE_CreateHostQnode ();
   WPE_CreateMultiCastQnode ();
} 

void WPE_CreateIWBridgeSystem ()
{
   WP_dfc_sys_info dfc_sys_info = {

      /* classification_hash_size */ WP_IW_2K_HASH_ENTRIES,
      /* port_filtering */ WP_IW_PORT_FILTERING_INPUT,
      /* deny_eq_ip */ WP_IW_DENY_EQUAL_IP_DISABLE
   };
   WP_iw_sys_bridging iw_sys_bridge = {

      /* tag */ 0,
      /* max_flows; */ MAX_IW_FLOWS,
      /* classification_mode */ WP_IW_CLASSIFIER_BASED_BRIDGING,
      /* classifier_config */
      {

       /*classification_parsing_mode */ WP_IW_DYNAMIC_CLASS,
       /*max_classification_rules; */ (NUM_OF_EMC_FILTERS + 1) * NUM_OF_RULES_PER_FILTER,
       /*number_parse_fields */ 0,
       /**parse_keys*/ NULL
       },
      /* learning_mode; */ WP_IW_BRIDGE_LEARNING_ENABLE,
      /* learning_queue */
      {

       /*int_queue_num */ WP_IW_IRQT1,
       /*learning_queue_size */ LEARNING_QUEUE_SIZE,
       /*interrupt_enbale */ WP_LEARNING_INTENABLE,
       /*interrupt_rate */ 1
       },
      /*forwarding_table_size */ WP_IW_32K_HASH_ENTRIES,
      /*member_set_size */ WP_IW_32K_HASH_ENTRIES,
      /*buffer_gap; */ 64,
      /*max_iw_ports */ NUM_OF_IW_BPORTS,
      /*dfc info */ &dfc_sys_info,
      /*svl_mode */ WP_IW_SVL_DISABLED,
      /*stag_ether_type */ WP_IW_SYS_BRIDGE_STAG_VLAN_ETYPE_DISABLE,
      /*parsing_mode; */ 0,
      /*dfc_log_mode; */ 0
   };
   iw_sys =
      WP_IwSystemCreate (WP_WINPATH (0),
                         WP_IW_VLAN_AWARE_BRIDGING_MODE, &iw_sys_bridge);
   terminate_on_error (iw_sys, " WP_IwSystemCreate - Bridging");
} 


void WPE_EnableSystem ()
{
   WP_handle status;
   WP_U32 i, j, k;

   status = WP_SysPsuEnable (WP_WINPATH (0));
   terminate_on_error (status, "WP_SysPsuEnable()");

   /* Enable the Enet Channels First TX than RX */
   for (i = 0; i < NUM_OF_FAST_ENET_TX_CHANNELS; i++)
   {
      status = WP_ChannelEnable (tx_gbe_channel[i]);
      terminate_on_error (status, "WP_ChannelEnable() tx_gbe");
   }
   status = WP_ChannelEnable (rx_gbe_channel);
   terminate_on_error (status, "WP_ChannelEnable() rx_gbe");

   for (i = 0; i < NUM_OF_FAST_ENET_TX_CHANNELS; i++)
   {
      status = WP_ChannelEnable (tx_gbe_channel2[i]);
      terminate_on_error (status, "WP_ChannelEnable() tx_gbe 2");
   }

   status = WP_ChannelEnable (rx_gbe_channel2);
   terminate_on_error (status, "WP_ChannelEnable() rx_gbe 2");

   // Hierarchical Enet
   for (i = 0; i < NUM_OF_L1_GROUP; i++)
   {
      for (j = 0; j < NUM_OF_L2_GROUP; j++)
      {
         for (k = 0; k < NUM_OF_HIER_ENET_FAS_PER_L2; k++)
         {
            status = WP_ChannelEnable (tx_gbe_hier_channel[i][j][k]);
            terminate_on_error (status, "WP_ChannelEnable() tx_gbe");
         }
      }
   }
   status = WP_ChannelEnable (rx_gbe_hier_channel);
   terminate_on_error (status, "WP_ChannelEnable() rx_gbe");

   /* Enable the Enet Ports */
   status = WP_PortEnable (port_enet, WP_DIRECTION_DUPLEX);
   terminate_on_error (status, "WP_PortEnable Enet");

   status = WP_PortEnable (port_enet2, WP_DIRECTION_DUPLEX);
   terminate_on_error (status, "WP_PortEnable Enet2");

   /* Enable the Enet Device */
   status = WP_DeviceEnable (dev_enet, WP_DIRECTION_DUPLEX);
   terminate_on_error (status, "WP_DeviceEnable Enet");
   status = WP_DeviceEnable (dev_enet2, WP_DIRECTION_DUPLEX);
   terminate_on_error (status, "WP_DeviceEnable Enet2");
   status = WP_PortEnable (port_hier_enet, WP_DIRECTION_DUPLEX);
   terminate_on_error (status, "WP_PortEnable Enet");
   status = WP_DeviceEnable (dev_hier_enet, WP_DIRECTION_DUPLEX);
   terminate_on_error (status, "WP_DeviceEnable Enet");
}

void WPE_CreateHostPortDevice ()
{
   port_host = WP_PortCreate (WP_WINPATH (0), WP_PORT_IW_HOST, NULL);
   terminate_on_error (port_host, "WP_PortCreate() HOST");
   dev_host = WP_DeviceCreate (port_host, 0, WP_DEVICE_IW_HOST, NULL);
   terminate_on_error (dev_host, "WP_DeviceCreate() Host");
} 

void WPE_CreateHostIwRxChannel ()
{

   // This is TX from DPS point of view but RX from Host point of view
   WP_tx_binding tx_host_binding_config = {

      /* res0 */ 0,
      // Unused
      /* dci_mode */ WP_IW_DYN_CH_INSERT_DISABLE,
      /* maxt */ MAXT_SIZE
   };
   WP_ch_iw_rx iw_host_ch_config = {

      /* pqblock */ 0,
      /* pqlevel */ 0,
      /* tx_binding_type */ WP_IW_TX_BINDING,
      /* tx_binding_config */ &tx_host_binding_config
   };
   rx_host_channel =
      WP_ChannelCreate (0, dev_host, iw_qnode, WP_CH_RX,
                        WP_IW_HOST, &iw_host_ch_config);
   terminate_on_error (rx_host_channel,
                       "WP_ChannelCreate RX Host channel");
} 

void WPE_CreateHostIwTxChannel ()
{
   WP_ch_iw_tx ch_iw_tx_config = {

      /* iw_system */ 0,
      /* reserved */ 0,
      /* group_id */ WP_IW_CH_GROUP_ID_NULL,
      /* reserved2 */ 0,
      /* input_port */ 0
   };
   ch_iw_tx_config.iw_system = iw_sys;
   ch_iw_tx_config.input_port = bport_host;
   tx_host_channel =
      WP_ChannelCreate (0, dev_host, iw_qnode, WP_CH_TX,
                        WP_IW_HOST, &ch_iw_tx_config);
   terminate_on_error (tx_host_channel,
                       "WP_ChannelCreate TX Host channel");
}

// This is default flow aggregation
// In case you want to drop all the traffic that arrives to Default FlowAgg (or any other FA)
// You can set the MTU_SIZE=0; or set in tx_binding  maxt = 0
void WPE_CreateHostTermFlowAgg ()
{
   WP_iw_agg_bridging agg_host_config = {

      /*tag */ agg_tag_counter++,
      /*txfunc */ 0,
      /*iw_port */ 0,
      /*rfcs */ WP_IW_RFCS_DISABLE,
      /*l2_hdr_insert_mode */ WP_IW_L2H_INSERT_DISABLE,
      /*encap_vlan_tag_mode */ WP_IW_VLAN_TAG_DISABLE,
      //  Enable = Untagged - AddDefaultVLAN, Tagged - Do not Strip VLAN
      /*interruptqueue; */ WP_IW_IRQT1,
      /*error_pkt_mode */ WP_IW_ERRPKT_DISCARD,
      /*intmode; */ WP_IW_INT_ENABLE,
      // To allow receiving packets to host using interrupts
      /*statmode; */ WP_IW_STAT_ENABLE,
      /*timestamp_mode; */ WP_IW_TIME_STAMP_DISABLE,
      /*ov_pool_mode */ WP_IW_OV_POOL_DISABLE,
      /*fbp_drop_threshold */ 0,
      /*replace_vlan_tag */ WP_IW_REPLACE_VTAG_DISABLE,
      /*vlan_id_new */ 0,
      /*vpmt_handle */ 0,
      /*mtu; */ MTU_SIZE,
      /*prefix_length */ 0,
      /*prefix_header[32] */
      {
       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
       0x00, 0x00, 0x00, 0x00, 0x00,
       0x00, 0x00, 0x00, 0x00, 0x00,
       0x00, 0x00, 0x00, 0x00, 0x00,
       0x00, 0x00, 0x00, 0x00, 0x00,
       0x00, 0x00, 0x00, 0x00},
      /*policer_enable */ WP_IW_POLICER_DISABLE,
                /**policer_config;*/ NULL,
      /*cong_mode; */ WP_IW_CONGESTION_DISABLE,
      /*cong_thresh_param */ NULL,
      /*flow_agg_type */ WP_IW_AGG_PRIMARY,
      /*external_vlan */
      {
       WP_IW_EXTERN_VLAN_NONE, 0},
      /*bc_mc_priority */
      {
       0, 0, 0, 0},
      /*reserved_agg_mode */ WP_IW_NOT_RESERVED,
      /*next_prsrv_inport */ WP_IW_OVERRIDE_INPORT,
      /*prsrv_inport_params */ WP_IW_OVERRIDE_INPORT,
      /*mac_replace_mode; */ WP_IW_MAC_REPLACE_DISABLED,
      /*new_dst_mac[6]; */
      {
       0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
      /*new_src_mac[6]; */
      {
       0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
      /*hier_cong_prof; */ WP_DISABLE,
      /*extraction_length; */ 0,
      /*policer_handle; */ 0,
      /*vpmt_accessing_mode */
      WP_IW_VPMT_ACCESSING_EGRESS_EXTERNAL,
      /*prefix_rmrk_fields */ 0,
      /*prefix_rmrk_offst */
      {
       0x00, 0x00, 0x00},
      /*forw_dscp_remark */ WP_DISABLE,
      /*dscp_remark_offset; */ 0
   };
#if 1
   agg_host_config.txfunc = rx_host_channel;
#else
   agg_host_config.txfunc = tx_gbe_channel[0];
#endif
   agg_host =
      WP_IwFlowAggregationCreate (WP_WINPATH (0),
                                  WP_IW_TRANS_BRIDGING_MODE,
                                  &agg_host_config);
   terminate_on_error (agg_host, "WPE_CreateHostTermIwFlowAgg() Host");
}  


void WPE_CreateIwBportHost ()
{
   WP_handle status;
   WP_iw_agg_bridging agg_host_config = {
      0
   };
   WP_bridge_port iwhost_bport_config = {

      /* tag */ BRIDGE_PORT_HOST_TAG,
      /* direct_mapping */ WP_IW_DIRECT_MAP_DISABLE,
      /* flow_agg */ 0,
      /* flooding_term_mode */ WP_IW_HOST_TERM_MODE,
      /* learning_mode */ WP_IW_LEARNING_DISABLE,
      /* in_filter_mode */ WP_IW_INGRESS_FILTER_ENABLE,
      /* vlan_param */
      {

       /* vlan_acceptance_mode */
       WP_IW_ACCEPT_TAGGED_UNTAGGED,
       /* default_vlan_tag */ VLAN_TAG_1,
       /* vlan_tunnel;     */
       WP_IW_VLAN_TUNNEL_1Q_IN_1Q_DISABLE,
       /* vlan_pri_enforce_mode */
       WP_IW_VLAN_PRIORITY_ENFORCE_DISABLED},
      /*max_mac_addresses */ 2000,
      /*group_tag */ WP_IW_BRIDGEALL_GROUP_FILTER,
      /*group_filtering_mode */
      WP_IW_GROUP_FILTER_DISABLE,
      /*unk_mac_sa_filter */
      WP_IW_UNK_MACSA_FILTER_DISABLE,
      /*unk_mc_mode; */ WP_IW_UNK_MC_HT,
      /*bc_ht_mode; */ WP_IW_BC_HT_DISABLE,
      /*input_filters_mask */ 0,
      /*output_filters_mask */ 0,
      /*statmode */ WP_IW_PORT_STAT_ENABLE,
      /*unk_uc_mode; */ WP_IW_UNK_UC_SR_ENABLE,
      /*classification_flag */
      WP_IW_BPORT_CLASSIFICATION_ENABLED,
      /*adv_unk_lookup_mode */
      WP_IW_ADV_UNK_LOOKUP_DISABLED,
      /*cfi_ht_mode; */ WP_IW_CFI_HT_DISABLE,
      /*reserved_mc_ht_mode */ WP_IW_RES_MC_HT_DISABLE,
      /*predefined_ht_mode */ WP_DISABLE,
      /*res_mc_action_table */
      {
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         /**enet_oam_params*/ NULL,
      /* dfc_log_mode */ 0
   };
#if 1
   iwhost_bport_config.flow_agg = agg_host;  // default flow agg
#else
   iwhost_bport_config.flow_agg = agg_enet[0];  // default flow agg
#endif
   bport_host = WP_IwPortCreate (iw_sys, &iwhost_bport_config);
   terminate_on_error (bport_host, " WPE_CreateIwBportHost ");

   /* The flow agg should be modifed, since the host bport was created after the flow aggregation */
   agg_host_config.input_bport = bport_host;
   status =
      WP_IwFlowAggregationModify (agg_host,
                                  WP_IW_FLOW_AGG_B_MOD_BPORT,
                                  &agg_host_config);
   terminate_on_error (status, "WP_IwFlowAggregationModify");
} 

void WPE_CreateFastEnetPortDevice ()
{
   WP_handle status;

   WP_port_enet enet_port_config = {
      /* pkt_limits */
      {
       /* max_tx_channels */ 1,
       /* max_rx_channels */ 1,
       /* pq_block_size */ 0,
       /* n_pq_blocks */ 0,
       /* emphy_devices */ WP_UNUSED
       },
      /* flowmode */ WP_FLOWMODE_FAST,
#if 0
      /* interface_mode */ WP_ENET_SGMII_1000,
#else
      /* interface_mode */ WP_ENET_1000_BASE_X,
#endif
      /* rx_iw_bkgnd */ 0,
   };

#if 1
   WP_enet_filters rx_filters = {

      /* rx_ucfilter; */ WP_ENET_FILTER_PROCESS,
      /* rx_bcfilter; */ WP_ENET_FILTER_PROCESS,
      /* rx_mcfilter; */ WP_ENET_FILTER_PROCESS
   };
   WP_device_enet_ex enet_dev_ext_params_config = {

      /* duplexmode; */ WP_ENET_FULLDUPLEX,
      /* rx_maclookup_mode */ WP_DISABLE,
      /* rx_flowcontrol; */ WP_ENET_FLOW_ENABLE,
      /* tx_flowcontrol; */ WP_ENET_FLOW_ENABLE,
      /* rx_addrmode; */ WP_ENET_ADDR_ACCEPTALL,
      /* phystatmode; */ WP_ENET_STAT_ENABLE,
      /* tx_statmode; */ WP_ENET_STAT_ENABLE,
      /* rx_statmode; */ WP_ENET_STAT_ENABLE,
      /* tx_duplicate; */ WP_DISABLE,
      /* rx_filters; */ &rx_filters,
      /* rx_timestamp; */ 0
   };

#endif /*  */
   WP_device_enet enet_dev_config = {

      /* max_tx_channels */ 1,
      /* tx_maxsdu */ SDU_SIZE,
      /* operating_speed */ WP_UNUSED,
      // Valid for RMII, RGMII only
      /* mac_addr */
      {
       0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa}, // MAC of this device
      /* tx_bit_rate */ 1000000000,
      /* loopbackmode */ WP_ENET_NORMAL,
#if 1
      /* extended_params */ NULL
#else
	&enet_dev_ext_params_config 
#endif
         // Must be NULL at device creation
   };

   status = 0;
   enet_dev_ext_params_config.duplexmode = WP_ENET_FULLDUPLEX;
   //enet_port_config.interface_mode = WP_ENET_SGMII_10_100;
   enet_port_config.pkt_limits.max_tx_channels =
      NUM_OF_FAST_ENET_TX_CHANNELS;
   enet_port_config.pkt_limits.max_rx_channels =
      NUM_OF_FAST_ENET_RX_CHANNELS;
   enet_port_config.flowmode = WP_FLOWMODE_FAST;   

   port_enet =
      WP_PortCreate (WP_WINPATH (0), EGRESS_PORT, &enet_port_config);
   terminate_on_error (port_enet, "WP_PortCreate() Fast ENET");

   port_enet2=
      WP_PortCreate (WP_WINPATH (0), EGRESS_PORT2, &enet_port_config);
   terminate_on_error (port_enet, "WP_PortCreate() Fast ENET2");

   enet_dev_config.max_tx_channels = NUM_OF_FAST_ENET_TX_CHANNELS;


   /* 
    * device loopback was not supported by wdds4.3.1 anymore
    */
#if 0
   enet_dev_config.loopbackmode = WP_ENET_LOOPBACK;   
#else
   enet_dev_config.loopbackmode = WP_ENET_NORMAL;

#endif

   dev_enet =
      WP_DeviceCreate (port_enet, WP_PHY (0), WP_DEVICE_ENET,
                       &enet_dev_config);
   terminate_on_error (dev_enet, "WP_DeviceCreate() Fast Enet");

   dev_enet2 =
      WP_DeviceCreate (port_enet2, WP_PHY (0), WP_DEVICE_ENET,
                       &enet_dev_config);
   terminate_on_error (dev_enet, "WP_DeviceCreate() Fast Enet");

#if  0
   enet_dev_config.extended_params = &enet_dev_ext_params_config;
   status = WP_DeviceModify (dev_enet, WP_DEV_MOD_ENET_DUPLEXMODE, &enet_dev_config);

   terminate_on_error (status, "WP_DeviceModify () Fast Enet");
   printf ("DeviceModify() ok\n");
#endif
} 


void WPE_CreateHierEnetPortDevice ()
{
   WP_port_enet enet_port_config = {

      /* pkt_limits */
      {

       /* max_tx_channels */ 1,
       /* max_rx_channels */ 1,
       /* pq_block_size */ 0,
       /* n_pq_blocks */ 0,
       /* emphy_devices */ WP_UNUSED
       },
      /* flowmode */ WP_FLOWMODE_FAST,
#if 0
      // /* interface_mode */ WP_ENET_SGMII_1000,
	WP_ENET_SGMII_AN,
#else
      /* interface_mode */ WP_ENET_1000_BASE_X,
#endif
      /* rx_iw_bkgnd */ 0,
   };

#if 0
   WP_enet_filters rx_filters = {

      /* rx_ucfilter; */ WP_ENET_FILTER_PROCESS,
      /* rx_bcfilter; */ WP_ENET_FILTER_PROCESS,
      /* rx_mcfilter; */ WP_ENET_FILTER_PROCESS
   };
   WP_device_enet_ex enet_dev_ext_params_config = {

      /* duplexmode; */ WP_ENET_FULLDUPLEX,
      /* rx_maclookup_mode */ WP_DISABLE,
      /* rx_flowcontrol; */ WP_ENET_FLOW_ENABLE,
      /* tx_flowcontrol; */ WP_ENET_FLOW_ENABLE,
      /* rx_addrmode; */ WP_ENET_ADDR_ACCEPTALL,
      /* phystatmode; */ WP_ENET_STAT_ENABLE,
      /* tx_statmode; */ WP_ENET_STAT_ENABLE,
      /* rx_statmode; */ WP_ENET_STAT_ENABLE,
      /* tx_duplicate; */ WP_DISABLE,
      /* rx_filters; */ &rx_filters,
      /* rx_timestamp; */ 0
   };

#endif /*  */
   WP_device_enet hier_enet_dev_config = {

      /* max_tx_channels */ 1,
      /* tx_maxsdu */ SDU_SIZE,
      /* operating_speed */ WP_UNUSED,
      // Valid for RMII, RGMII only
      /* mac_addr */
      {
       0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa},
      // MAC of this device
      /* tx_bit_rate */ 1000000000,
      /* loopbackmode */ WP_ENET_NORMAL,
      /* extended_params */ NULL
         // Must be NULL at device creation
   };
   enet_port_config.pkt_limits.max_tx_channels = NUM_OF_HIER_ENET_TX_CHANNELS;
   enet_port_config.pkt_limits.max_rx_channels = NUM_OF_HIER_ENET_RX_CHANNELS;
   enet_port_config.flowmode = WP_ENET_FMU_HIERARCHICAL_SHAPING_MODE;

   port_hier_enet =
      WP_PortCreate (WP_WINPATH (0), INGRESS_PORT, &enet_port_config);
   terminate_on_error (port_hier_enet,
                       "WP_PortCreate() Hierarchical ENET");

   hier_enet_dev_config.max_tx_channels = NUM_OF_HIER_ENET_TX_CHANNELS;
   dev_hier_enet =
      WP_DeviceCreate (port_hier_enet, WP_PHY (0),
                       WP_DEVICE_ENET, &hier_enet_dev_config);
   terminate_on_error (dev_hier_enet,
                       "WP_DeviceCreate() Hierarchical Enet");
} 

void WPE_CreateFastEnetIwBport ()
{
   WP_bridge_port enet_bport_config = {

      /* tag */ 0,
      /* direct_mapping */ WP_IW_DIRECT_MAP_DISABLE,
      /* flow_agg */ 0,
      /* flooding_term_mode */ WP_IW_HOST_TERM_MODE,
      /* learning_mode */ WP_IW_LEARNING_DISABLE,
      /* in_filter_mode */ WP_IW_INGRESS_FILTER_ENABLE,
      /* vlan_param */
      {

       /* vlan_acceptance_mode */ WP_IW_ACCEPT_TAGGED_UNTAGGED,
       /* default_vlan_tag */ VLAN_TAG_1,
       /* vlan_tunnel;     */ WP_IW_VLAN_TUNNEL_1Q_IN_1Q_DISABLE,
       /* vlan_pri_enforce_mode */ WP_IW_VLAN_PRIORITY_ENFORCE_DISABLED},
      /*max_mac_addresses */ 2000,
      /*group_tag */ WP_IW_BRIDGEALL_GROUP_FILTER,
      /*group_filtering_mode */ WP_IW_GROUP_FILTER_DISABLE,
      /*unk_mac_sa_filter */ WP_IW_UNK_MACSA_FILTER_DISABLE,
      /*unk_mc_mode; */ WP_IW_UNK_MC_HT,
      /*bc_ht_mode; */ WP_IW_BC_HT_DISABLE,
      /*input_filters_mask */ 0,
      /*output_filters_mask */ 0,
      /*statmode */ WP_IW_PORT_STAT_ENABLE,
      /*unk_uc_mode; */ WP_IW_UNK_UC_SR_ENABLE,
      /*classification_flag */ WP_IW_BPORT_CLASSIFICATION_ENABLED,
      /*adv_unk_lookup_mode */ WP_IW_ADV_UNK_LOOKUP_DISABLED,
      /*cfi_ht_mode; */ WP_IW_CFI_HT_DISABLE,
      /*reserved_mc_ht_mode */ WP_IW_RES_MC_HT_DISABLE,
      /*predefined_ht_mode */ WP_DISABLE,
      /*res_mc_action_table */
      {
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         /**enet_oam_params*/ NULL,
      /* dfc_log_mode */ 0
   };

   enet_bport_config.tag = BRIDGE_PORT_ENET_TAG;   
   enet_bport_config.input_filters_mask = 0x00000000;
#if 1
   enet_bport_config.flow_agg = agg_host; // default flow agg
#else
   enet_bport_config.flow_agg = agg_enet[0]; // default flow agg
#endif
   bport_enet = WP_IwPortCreate (iw_sys, &enet_bport_config);
   terminate_on_error (bport_enet, " WPE_CreateIwBportEnet Fast");
}  


void WPE_CreateHierEnetIwBport ()
{
   WP_bridge_port enet_bport_config = {

      /* tag */ 0,
      /* direct_mapping */ WP_IW_DIRECT_MAP_DISABLE,
      /* flow_agg */ 0,
      /* flooding_term_mode */ WP_IW_HOST_TERM_MODE,
      /* learning_mode */ WP_IW_LEARNING_DISABLE,
      /* in_filter_mode */ WP_IW_INGRESS_FILTER_ENABLE,
      /* vlan_param */
      {

       /* vlan_acceptance_mode */ WP_IW_ACCEPT_TAGGED_UNTAGGED,
       /* default_vlan_tag */ VLAN_TAG_1,
       /* vlan_tunnel;     */ WP_IW_VLAN_TUNNEL_1Q_IN_1Q_DISABLE,
       /* vlan_pri_enforce_mode */ WP_IW_VLAN_PRIORITY_ENFORCE_DISABLED},
      /*max_mac_addresses */ 2000,
      /*group_tag */ WP_IW_BRIDGEALL_GROUP_FILTER,
      /*group_filtering_mode */ WP_IW_GROUP_FILTER_DISABLE,
      /*unk_mac_sa_filter */ WP_IW_UNK_MACSA_FILTER_DISABLE,
      /*unk_mc_mode; */ WP_IW_UNK_MC_HT,
      /*bc_ht_mode; */ WP_IW_BC_HT_DISABLE,
      /*input_filters_mask */ 0,
      /*output_filters_mask */ 0,
      /*statmode */ WP_IW_PORT_STAT_ENABLE,
      /*unk_uc_mode; */ WP_IW_UNK_UC_SR_ENABLE,
      /*classification_flag */ WP_IW_BPORT_CLASSIFICATION_ENABLED,
      /*adv_unk_lookup_mode */ WP_IW_ADV_UNK_LOOKUP_DISABLED,
      /*cfi_ht_mode; */ WP_IW_CFI_HT_DISABLE,
      /*reserved_mc_ht_mode */ WP_IW_RES_MC_HT_DISABLE,
      /*predefined_ht_mode */ WP_DISABLE,
      /*res_mc_action_table */
      {
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         /**enet_oam_params*/ NULL,
      /* dfc_log_mode */ 0
   };

   // Only one bPort is possible for one Enet device because only one RX channel is possible
   enet_bport_config.tag = BRIDGE_PORT_HIER_ENET_TAG; 
   enet_bport_config.input_filters_mask = 0x00000000;
   enet_bport_config.flow_agg = agg_host; // default flow agg
   bport_hier_enet = WP_IwPortCreate (iw_sys, &enet_bport_config);
   terminate_on_error (bport_hier_enet,
                       " WPE_CreateIwBportEnet Hierarchical");
}  


void WPE_CreateFastEnetFlowAgg ()
{
   WP_handle status;
   WP_U32 i;
   WP_bridge_port bport_enet_config = {0};

#if 0
   WP_policer_slb_config slb_config = {

      /* leaky_bucket_size */ 1000
         // in milliseconds
   };
   WP_policer_v1 policer_config = {

      /* *leaky_bucket */ &slb_config,
      /* committed_info_rate */ 20000000,
      // in bits per second
      /* policer_mode */ WP_IW_DISCARD_NONCONFORMING_PKT,
      /* tx_handle */ 0,
      // Unused
      /* leaky_bucket_mode */ WP_IW_SINGLE_LEAKY_BUCKET,
      /* frame_len_correction */
      WP_IW_PLC_FRM_LEN_CORRECT_DISABLE,
      /* *tcm_config */
   };

#endif /*  */
   WP_iw_agg_bridging agg_enet_config = {

      /*tag */ agg_tag_counter++,
      /*txfunc */ 0,
      /*iw_port */ 0,
      /*rfcs */ WP_IW_RFCS_DISABLE,
      /*l2_hdr_insert_mode */ WP_IW_L2H_INSERT_DISABLE,
      /*encap_vlan_tag_mode */ WP_IW_VLAN_TAG_ENABLE,
      // Enable = Untagged - AddDefaultVLAN, Tagged - Do not Strip VLAN
      /*interruptqueue; */ WP_IW_IRQT1,
      /*error_pkt_mode */ WP_IW_ERRPKT_DISCARD,
      /*intmode; */ WP_IW_INT_DISABLE,
      /*statmode; */ WP_IW_STAT_ENABLE,
      /*timestamp_mode; */ WP_IW_TIME_STAMP_DISABLE,
      /*ov_pool_mode */ WP_IW_OV_POOL_DISABLE,
      /*fbp_drop_threshold */ 0,
      /*replace_vlan_tag */ WP_IW_REPLACE_VTAG_DISABLE,
      /*vlan_id_new */ 0,
      /*vpmt_handle */ 0,
      /*mtu; */ MTU_SIZE,
      /*prefix_length */ 0,
      /*prefix_header[32] */
      {
       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
       0x00, 0x00, 0x00, 0x00, 0x00,
       0x00, 0x00, 0x00, 0x00, 0x00,
       0x00, 0x00, 0x00, 0x00, 0x00,
       0x00, 0x00, 0x00, 0x00, 0x00,
       0x00, 0x00, 0x00, 0x00},
      /*policer_enable */ WP_IW_POLICER_DISABLE,
         /**policer_config;*/ NULL,
      /*cong_mode; */ WP_IW_CONGESTION_DISABLE,
      /*cong_thresh_param */ NULL,
      /*flow_agg_type */ WP_IW_AGG_PRIMARY,
      /*external_vlan */
      {
       WP_IW_EXTERN_VLAN_NONE, 0},
      /*bc_mc_priority */
      {
       0, 0, 0, 0},
      /*reserved_agg_mode */ WP_IW_NOT_RESERVED,
      /*next_prsrv_inport */ WP_IW_OVERRIDE_INPORT,
      /*prsrv_inport_params */ WP_IW_OVERRIDE_INPORT,
      /*mac_replace_mode; */ WP_IW_MAC_REPLACE_DISABLED,
      /*new_dst_mac[6]; */
      {
       0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
      /*new_src_mac[6]; */
      {
       0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
      /*hier_cong_prof; */ WP_DISABLE,
      /*extraction_length; */ 0,
      /*policer_handle; */ 0,
      /*vpmt_accessing_mode */ WP_IW_VPMT_ACCESSING_EGRESS_EXTERNAL,
      /*prefix_rmrk_fields */ 0,
      /*prefix_rmrk_offst */
      {
       0x00, 0x00, 0x00},
      /*forw_dscp_remark */ WP_DISABLE,
      /*dscp_remark_offset; */ 0
   };
   for (i = 0; i < NUM_OF_FAST_ENET_FAS /* 1 */ ; i++)

   {
      agg_enet_config.txfunc = tx_gbe_channel[i];
#if 1
      agg_enet_config.iw_port = bport_enet;
#else
      agg_enet_config.iw_port = bport_hier_enet;
#endif
      agg_enet_config.l2_header_insert_mode = WP_IW_L2H_INSERT_ENABLE;
      agg_enet_config.mac_replace_mode = WP_IW_REPLACE_DST_MAC;
      memcpy (agg_enet_config.new_dst_mac, hier_enet_dst_mac, 6); 

      //agg_enet_config.policer_enable = WP_IW_POLICER_ENABLE;
      //agg_enet_config.policer_config = &policer_config;
      agg_enet[i] =
         WP_IwFlowAggregationCreate (WP_WINPATH (0),
                                     WP_IW_VLAN_AWARE_BRIDGING_MODE,
                                     &agg_enet_config);
      terminate_on_error (agg_enet[i],
                          "WPE_CreateEnetIwFlowAgg() Fast Enet");
   }

   // Agg with Change DST MAC to HOST Port
   agg_enet_config.txfunc = tx_gbe_channel[0];
   agg_enet_config.iw_port = bport_enet;
   agg_enet_config.mac_replace_mode = WP_IW_REPLACE_DST_MAC;
   agg_enet_config.l2_header_insert_mode = WP_IW_L2H_INSERT_ENABLE;
   memcpy (agg_enet_config.new_dst_mac, host_dst_mac, 6);
   agg_enet_change_mac =
      WP_IwFlowAggregationCreate (WP_WINPATH (0),
                                  WP_IW_VLAN_AWARE_BRIDGING_MODE,
                                  &agg_enet_config);
   terminate_on_error (agg_enet_change_mac,
                       "WP_IwFlowAggregationCreate Replace MAC()");


   bport_enet_config.flow_agg =  agg_enet_change_mac;
   status = WP_IwPortModify(bport_enet, WP_IW_PORT_MODIFY_FLOW_AGG, &bport_enet_config );
   terminate_on_error (status,
                       "WP_IwPortModify Replace MAC()");
}

void WPE_CreateHierEnetFlowAgg ()
{
   WP_U32 i, j, k;

#if 0
   WP_policer_slb_config slb_config = {

      /* leaky_bucket_size */ 1000
         // in milliseconds
   };
   WP_policer_v1 policer_config = {

      /* *leaky_bucket */ &slb_config,
      /* committed_info_rate */ 20000000,
      // in bits per second
      /* policer_mode */ WP_IW_DISCARD_NONCONFORMING_PKT,
      /* tx_handle */ 0,
      // Unused
      /* leaky_bucket_mode */ WP_IW_SINGLE_LEAKY_BUCKET,
      /* frame_len_correction */
      WP_IW_PLC_FRM_LEN_CORRECT_DISABLE,
      /* *tcm_config */
   };

#endif /*  */
   WP_iw_agg_bridging agg_enet_config = {

      /*tag */ agg_tag_counter++,
      /*txfunc */ 0,
      /*iw_port */ 0,
      /*rfcs */ WP_IW_RFCS_DISABLE,
      /*l2_hdr_insert_mode */ WP_IW_L2H_INSERT_DISABLE,
      /*encap_vlan_tag_mode */ WP_IW_VLAN_TAG_ENABLE,
      // Enable = Untagged - AddDefaultVLAN, Tagged - Do not Strip VLAN
      /*interruptqueue; */ WP_IW_IRQT1,
      /*error_pkt_mode */ WP_IW_ERRPKT_DISCARD,
      /*intmode; */ WP_IW_INT_DISABLE,
      /*statmode; */ WP_IW_STAT_ENABLE,
      /*timestamp_mode; */ WP_IW_TIME_STAMP_DISABLE,
      /*ov_pool_mode */ WP_IW_OV_POOL_DISABLE,
      /*fbp_drop_threshold */ 0,
      /*replace_vlan_tag */ WP_IW_REPLACE_VTAG_DISABLE,
      /*vlan_id_new */ 0,
      /*vpmt_handle */ 0,
      /*mtu; */ MTU_SIZE,
      /*prefix_length */ 0,
      /*prefix_header[32] */
      {
       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
       0x00, 0x00, 0x00, 0x00, 0x00,
       0x00, 0x00, 0x00, 0x00, 0x00,
       0x00, 0x00, 0x00, 0x00, 0x00,
       0x00, 0x00, 0x00, 0x00, 0x00,
       0x00, 0x00, 0x00, 0x00},
      /*policer_enable */ WP_IW_POLICER_DISABLE,
         /**policer_config;*/ NULL,
      /*cong_mode; */ WP_IW_CONGESTION_DISABLE,
      /*cong_thresh_param */ NULL,
      /*flow_agg_type */ WP_IW_AGG_PRIMARY,
      /*external_vlan */
      {
       WP_IW_EXTERN_VLAN_NONE, 0},
      /*bc_mc_priority */
      {
       0, 0, 0, 0},
      /*reserved_agg_mode */ WP_IW_NOT_RESERVED,
      /*next_prsrv_inport */ WP_IW_OVERRIDE_INPORT,
      /*prsrv_inport_params */ WP_IW_OVERRIDE_INPORT,
      /*mac_replace_mode; */ WP_IW_MAC_REPLACE_DISABLED,
      /*new_dst_mac[6]; */
      {
       0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
      /*new_src_mac[6]; */
      {
       0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
      /*hier_cong_prof; */ WP_DISABLE,
      /*extraction_length; */ 0,
      /*policer_handle; */ 0,
      /*vpmt_accessing_mode */
      WP_IW_VPMT_ACCESSING_EGRESS_EXTERNAL,
      /*prefix_rmrk_fields */ 0,
      /*prefix_rmrk_offst */
      {
       0x00, 0x00, 0x00},
      /*forw_dscp_remark */ WP_DISABLE,
      /*dscp_remark_offset; */ 0
   };
   for (i = 0; i < NUM_OF_L1_GROUP /* 1 */ ; i++)
   {
      for (j = 0; j < NUM_OF_L2_GROUP /* 4 */ ; j++)
      {
         for (k = 0; k < NUM_OF_HIER_ENET_FAS_PER_L2 /* 8 */ ;
              k++)
         {
#if 1
            agg_enet_config.txfunc = tx_gbe_hier_channel[i][j][k];
#else
            agg_enet_config.txfunc = tx_gbe_hier_channel[0][0][0];
#endif
            agg_enet_config.iw_port = bport_hier_enet;

            //agg_enet_config.policer_enable = WP_IW_POLICER_ENABLE;
            //agg_enet_config.policer_config = &policer_config;
            agg_hier_enet[i][j][k] = WP_IwFlowAggregationCreate (WP_WINPATH (0), WP_IW_VLAN_AWARE_BRIDGING_MODE, &agg_enet_config);
            terminate_on_error (agg_hier_enet[i][j]
                                [k],
                                "WPE_CreateEnetIwFlowAgg Hierarchical()");
         }
      }
   }
}
void WPE_CreateFastEnetRxTxBinding ()
{
   WP_handle status;
   WP_U32 i;
   WP_rx_binding_bridging rx_binding_enet_config = {

      /*  encap_mode */ 0,
      // Not Used in Enet
      /*  mru; */ MTU_SIZE,
      /*  vcfcs; */ 0,
      /*  input_port; */ bport_enet
   };
   WP_tx_binding tx_binding_enet_config = {

      /* res0 */ 0,
      // Unused
      /* dci_mode; */ WP_IW_DYN_CH_INSERT_ENABLE,
      /* maxt; */ MAXT_SIZE
   };

   // Fast Enet
   status =
      WP_IwRxBindingCreate (rx_gbe_channel, iw_sys, iw_qnode,
                            &rx_binding_enet_config);
   terminate_on_error (status,
                       "WPE_CreateIwSysEnetRxBinding() rx_binding_enet Fast");
   for (i = 0; i < NUM_OF_FAST_ENET_TX_CHANNELS; i++)

   {
      status =
         WP_IwTxBindingCreate (tx_gbe_channel[i],
                               WP_IW_TX_BINDING, &tx_binding_enet_config);
      terminate_on_error (status, "WP_IwTxBindingCreate Fast");
   }

   status =
      WP_IwRxBindingCreate (rx_gbe_channel2, iw_sys, iw_qnode,
                            &rx_binding_enet_config);
   terminate_on_error (status,
                       "WPE_CreateIwSysEnetRxBinding() rx_binding_enet Fast 2");
   for (i = 0; i < NUM_OF_FAST_ENET_TX_CHANNELS; i++)

   {
      status =
         WP_IwTxBindingCreate (tx_gbe_channel2[i],
                               WP_IW_TX_BINDING, &tx_binding_enet_config);
      terminate_on_error (status, "WP_IwTxBindingCreate Fast x");
   }


}
void WPE_CreateHierEnetRxTxBinding ()
{
   WP_handle status;
   WP_U32 i, j, k;
   WP_rx_binding_bridging rx_binding_enet_config = {

      /*  encap_mode */ 0,
      // Not Used in Enet
      /*  mru; */ MTU_SIZE,
      /*  vcfcs; */ 0,
      /*  input_port; */ bport_hier_enet
   };
   WP_tx_binding tx_binding_enet_config = {

      /* res0 */ 0,
      // Unused
      /* dci_mode; */ WP_IW_DYN_CH_INSERT_ENABLE,
      /* maxt; */ MAXT_SIZE
   };

   // Hier Enet
   rx_binding_enet_config.input_port = bport_hier_enet;
   status =
      WP_IwRxBindingCreate (rx_gbe_hier_channel, iw_sys,
                            iw_qnode, &rx_binding_enet_config);
   terminate_on_error (status,
                       "WPE_CreateIwSysEnetRxBinding() rx_binding_enet Hier");
   for (i = 0; i < NUM_OF_L1_GROUP; i++)
   {
      for (j = 0; j < NUM_OF_L2_GROUP; j++)
      {
         for (k = 0; k < NUM_OF_HIER_ENET_FAS_PER_L2; k++)
         {
            status = WP_IwTxBindingCreate (tx_gbe_hier_channel[i][j][k], WP_IW_TX_BINDING, &tx_binding_enet_config);
            terminate_on_error (status, "WP_IwTxBindingCreate Hier");
         }
      }
   }
}



void WPE_CreateFastEnetRxTxChannel ()
{
   WP_U32 i;
   WP_ch_enet enet_ch_config = {

      /* intmode */ WP_PKTCH_INT_ENABLE,
      /* iwmode */ WP_PKTCH_IWM_ENABLE,
      /* testmode */ WP_PKTCH_TEST_DISABLE,
      /* tx_pqblock */ 0, // Unused in HW mode
      /* tx_pqlevel */ 0, // Unused in HW mode
      /* tx_shaping_type */ WP_FMU_SHAPING_TYPE_STRICT,
      /* tx_shaping_params */ NULL,
      /* rx_maxsdu */ SDU_SIZE,
      /* tx_cwid */ WP_CW_ID_A, // Unused in HW mode
      /* tx_tq */ 0,
      /* rx_queuedepth */ 10,
   };

   rx_gbe_channel =
      WP_ChannelCreate (0x22, dev_enet, iw_qnode, WP_CH_RX,
                        WP_ENET, &enet_ch_config);
   terminate_on_error (rx_gbe_channel, "WP_Channel_Create() Fast RX");

   for (i = 0; i < NUM_OF_FAST_ENET_TX_CHANNELS /* 1 */ ; i++)

   {
      enet_ch_config.tx_tq = i;
      tx_gbe_channel[i] =
         WP_ChannelCreate (0x11, dev_enet, iw_qnode,
                           WP_CH_TX, WP_ENET, &enet_ch_config);
      terminate_on_error (tx_gbe_channel[i],
                          "WP_Channel_Create() Fast TX");
   }

   rx_gbe_channel2 =
      WP_ChannelCreate (0x66, dev_enet2, iw_qnode, WP_CH_RX,
                        WP_ENET, &enet_ch_config);
   terminate_on_error (rx_gbe_channel2, "WP_Channel_Create() Fast RX2");

   for (i = 0; i < NUM_OF_FAST_ENET_TX_CHANNELS /* 1 */ ; i++)

   {
      enet_ch_config.tx_tq = i;
      tx_gbe_channel2[i] =
         WP_ChannelCreate (0x11, dev_enet2, iw_qnode,
                           WP_CH_TX, WP_ENET, &enet_ch_config);
      terminate_on_error (tx_gbe_channel2[i],
                          "WP_Channel_Create() Fast TX 2");
   }
}
void WPE_CreateHierHWEnetRxTxChannel ()
{
   WP_U32 i, j, k;

#if USE_SIMPLE_DEVICE
   WP_ch_enet simple_enet_ch_config = {

      /* intmode */ WP_PKTCH_INT_ENABLE,
      /* iwmode */ WP_PKTCH_IWM_ENABLE,
      /* testmode */ WP_PKTCH_TEST_DISABLE,
      /* tx_pqblock */ 0, // Unused in HW mode
      /* tx_pqlevel */ 0, // Unused in HW mode
      /* tx_shaping_type */ WP_FMU_SHAPING_TYPE_STRICT,
      /* tx_shaping_params */ NULL,
      /* rx_maxsdu */ SDU_SIZE,
      /* tx_cwid */ WP_CW_ID_A, // Unused in HW mode
      /* tx_tq */ 0,
      /* rx_queuedepth */ 10,
   };
#endif

   WP_fmu_shaping_cir_eir cir_eir_shaping_param = {

      /* cir; */ CIR_EIR_RATE, 	/* bits/second */
      /* cbs; */ 100000, 	/* Committed Burst Size in bits */
      /* eir; */ CIR_EIR_RATE, 	/* bits/second */
      /* ebs; */ 100000, 	/* Committed Burst Size in bits */
      /* flags */ 0,
   };

   WP_ch_enet hier_enet_ch_config = {

      /* intmode */ WP_PKTCH_INT_ENABLE,
      /* iwmode */ WP_PKTCH_IWM_ENABLE,
      /* testmode */ WP_PKTCH_TEST_DISABLE,
      /* tx_pqblock */ 0, // Unused in HW mode
      /* tx_pqlevel */ 0, // Unused in HW mode
      /* tx_shaping_type */ WP_FMU_SHAPING_TYPE_CIR_EIR,
      /* tx_shaping_params */ &cir_eir_shaping_param,
      /* rx_maxsdu */ SDU_SIZE,
      /* tx_cwid */ WP_CW_ID_A, // Unused in HW mode
      /* tx_tq */ 0,
      /* rx_queuedepth */ 10,
   };
   rx_gbe_hier_channel =
      WP_ChannelCreate (0x22, dev_hier_enet, iw_qnode, WP_CH_RX,
                        WP_ENET, &hier_enet_ch_config);
   terminate_on_error (rx_gbe_hier_channel,
                       "WP_Channel_Create() Gige Hierarchical RX");

   for (i = 0; i < NUM_OF_L1_GROUP /* 1 */ ; i++)
   {
      for (j = 0; j < NUM_OF_L2_GROUP /* 4 */ ; j++)
      {
         for (k = 0; k < NUM_OF_HIER_ENET_FAS_PER_L2 /* 8 */ ; k++)
         {
	    printf ("create tx_gbe_hier_channel, i[%d]j[%d]k[%d]\n",i,j,k);
            hier_enet_ch_config.tx_tq = k;
            tx_gbe_hier_channel[i][j][k] = WP_ChannelCreate (0x11+k,
#if USE_SIMPLE_DEVICE
				dev_hier_enet,
#else
                                 l2_group_hier[i][j], 
#endif
                                 iw_qnode,
                                 WP_CH_TX, WP_ENET, 
#if USE_SIMPLE_DEVICE
				&simple_enet_ch_config);
#else
				&hier_enet_ch_config);
#endif
            terminate_on_error (tx_gbe_hier_channel[i][j][k],
                                "WP_Channel_Create() Gige Hierarchical TX");
         }
      }
   }
}



void WPE_CreateL1FMUGroups (void)
{
   WP_U32 entry;
   WP_handle status;

#if 1
   WP_fmu_shaping_cir_eir my_l1_group_shaping_params = {

      /* cir */ CIR_EIR_RATE,
      /* cbs */ 80000,
      /* eir */ 0,
      /* ebs */ 0,
      /* flags */ 0
   };
#endif /*  */



   WP_fmu_shaping_wfq l1_group_shaping_params = {
#if 0
      /* weight */ 1,
#else
      /* weight */ WEIGHT_SIZE,
#endif
      /* flags */ 0,
   };
   WP_shaping_group_type_packet enet_group_l1_config = {

      /* group_level */       WP_L1_GROUP,
#if 0
      /* tx_shaping_type */   WP_FMU_SHAPING_TYPE_WFQ, 
      /* shaping_params */    &l1_group_shaping_params,
#else
      /* tx_shaping_type */   WP_FMU_SHAPING_TYPE_CIR_EIR,   //WP_FMU_SHAPING_TYPE_STRICT,
      /* tx_shaping_params */ &my_l1_group_shaping_params, //NULL, 
#endif
      /* num_fifos */         WP_NUM_FIFOS_8,
      /* block_handle */      0,
      /* block_level */       1,
      /* block_mode */        0,
   };

printf ("weight(%d)\n", WEIGHT_SIZE);
   l1_group_shaping_params.weight = WEIGHT_SIZE;
   my_l1_group_shaping_params.cir = 1000000;

   for (entry = 0; entry < NUM_OF_L1_GROUP /* 1 */ ; entry++)

   {
      enet_group_l1_config.block_level = entry;
      l1_group_hier[entry] =
         WP_ShapingGroupCreate (dev_hier_enet,
                                WP_SHAPING_GROUP_TYPE_PACKET,
                                &enet_group_l1_config);
      terminate_on_error (l1_group_hier[entry],
                          "WP_ShapingGroupCreate l1_hier");
      status = WP_ShapingGroupEnable (l1_group_hier[entry]);
      terminate_on_error (status, "WP_ShapingGroupEnable l1_hier");
   }
}
void WPE_CreateL2FMUGroups (void)
{
   WP_U32 i, entry;
   WP_handle status;
   WP_handle block_handle;

   WP_fmu_shaping_cir_eir l2_group_shaping_params = {
      /* cir */ CIR_EIR_RATE,
      /* cbs */ 80000,
      /* eir */ 0,
      /* ebs */ 0,
      /* flags */ 0
   };
   WP_shaping_group_type_enet enet_group_l2_config = {

      /* group_level */ WP_L2_GROUP,
#if USE_SOCKET                  //// <<<<<, WINSIM BUS: the FMU register does not updated correctly >>>>>>>
#error USE_SOCKET_SHOULD_NOT_BE_DEFINED
      /* tx_shaping_type */ WP_FMU_SHAPING_TYPE_STRICT,
      /* shaping_params */ NULL,
#else /*  */
      /* tx_shaping_type */ WP_FMU_SHAPING_TYPE_CIR_EIR,
      /* shaping_params */ &l2_group_shaping_params,
#endif /*  */
      /* num_fifos */ WP_NUM_FIFOS_8,
      /* block_handle */ 0,
      /* block_level */ 1,
      // not used without ShapingBlock
      /* block_mode */ WP_MODE_HW,
   };

   block_handle = 0;
#if 0
   block_handle = WP_ShapingBlockCreate(WP_SYSHANDLE(WP_WINPATH(0)),
                                     WP_L2_GROUP,
                                     16,
                                     WP_NUM_FIFOS_8);

   terminate_on_error (block_handle, "WP_ShapingBlockCreate error");
   enet_group_l2_config.block_handle = block_handle;
#endif

   i = 0;

#if 1
   for (i = 0; i < NUM_OF_L1_GROUP /* 1 */ ; i++)
#endif
   {
      for (entry = 0; entry < NUM_OF_L2_GROUP /* 4 */ ; entry++)
      {
	printf ("now it is i(%d), entry(%d)\n", i, entry);
#if USE_SW_CHANNELS
         enet_group_l2_config.group_mode = WP_MODE_SW;
#else /*  */
         enet_group_l2_config.group_mode = WP_MODE_HW;
#endif /*  */
	enet_group_l2_config.block_level = entry;
         l2_group_hier[i][entry] =
#if 1
            WP_ShapingGroupCreate (l1_group_hier[i],
#else
            WP_ShapingGroupCreate (dev_hier_enet,
#endif
                                   WP_SHAPING_GROUP_TYPE_PACKET,
                                   &enet_group_l2_config);
         terminate_on_error (l2_group_hier[i][entry],
                             "WP_ShapingGroupCreate l2_hier");
         status = WP_ShapingGroupEnable (l2_group_hier[i][entry]);
         terminate_on_error (status, "WP_ShapingGroupEnable l2_hier");
      }
   }
}

/********************************************************************************
 ***         Functions Implementations: Optional Functionality                ***
 *******************************************************************************/
void WPE_PolicerInit ()
{
   WP_module_policer policer_module_cfg;
   WP_policer_limits_v1 v1_policer_limits;
   WP_policer_limits_atm atm_policer_limits;
   WP_status status;

   memset (&atm_policer_limits, 0, sizeof (WP_policer_limits_atm));
   memset (&v1_policer_limits, 0, sizeof (WP_policer_limits_v1));
   memset (&policer_module_cfg, 0, sizeof (WP_module_policer));
   v1_policer_limits.max_policers = 1;
   policer_module_cfg.v2_policer_limits = 0;
   policer_module_cfg.v1_policer_limits = &v1_policer_limits;
   policer_module_cfg.atm_policer_limits = &atm_policer_limits;
   policer_module_cfg.mode = 0;
   status =
      WP_ModuleInit (WP_SYSHANDLE (0), WP_WDDI_MODULE_POLICER,
                     &policer_module_cfg);
   terminate_on_error (status, "WP_ModuleInit Policer");
}
void WPE_CreateSchedulingUnit (void)
{
   WP_status status;
   WP_calendar_wheel calendar[2] = {

      {

       /* function */ WP_CW_PSU,
       /* cwid */ WP_CW_ID_A,
       /* min_ch_rate */ 10000,
       // bps
       /* max_ch_rate */ 1000000
       // bps
       }, WP_CALENDAR_WHEEL_TERM
   };
   WP_feature_psu feature_psu_config = {

      /* max_ppr_plus_channels */ 0,
      /* max_cir_eir_channels */ 1,
      /* tx_maxsdu */ SDU_SIZE
   };
   status = WP_SysSchedulerCreate (WP_WINPATH (0), calendar);
   terminate_on_error (status, "WP_SysSchedulerCreate()");
   printf ("PSU_MODE On\n");
   status =
      WP_FeatureInit (WP_WINPATH (0), WP_FEATURE_PSU, &feature_psu_config);
   terminate_on_error (status, "WP_FeatureInit-PSU");
} 

void WPE_CreateDedicatedFlowAgg ()
{
   WP_U32 i;
   WP_iw_agg_bridging dedicated_agg_hier_enet_config = {

      /*tag */ agg_tag_counter++,
      /*txfunc */ 0,
      /*iw_port */ 0,
      /*rfcs */ WP_IW_RFCS_DISABLE,
      /*l2_hdr_insert_mode */ WP_IW_L2H_INSERT_ENABLE,
      /*encap_vlan_tag_mode */ WP_IW_VLAN_TAG_ENABLE,
      //  Enable = Untagged - AddDefaultVLAN, Tagged - Do not Strip VLAN
      /*interruptqueue; */ WP_IW_IRQT1,
      /*error_pkt_mode */ WP_IW_ERRPKT_DISCARD,
      /*intmode; */ WP_IW_INT_DISABLE,
      /*statmode; */ WP_IW_STAT_ENABLE,
      /*timestamp_mode; */ WP_IW_TIME_STAMP_DISABLE,
      /*ov_pool_mode */ WP_IW_OV_POOL_DISABLE,
      /*fbp_drop_threshold */ 0,
      /*replace_vlan_tag */ WP_IW_REPLACE_VTAG_DISABLE,
      /*vlan_id_new */ 0,
      /*vpmt_handle */ 0,
      /*mtu; */ MTU_SIZE,
      /*prefix_length */ 0,
      /*prefix_header[32] */
      {
       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
       0x00, 0x00, 0x00, 0x00, 0x00,
       0x00, 0x00, 0x00, 0x00, 0x00,
       0x00, 0x00, 0x00, 0x00, 0x00,
       0x00, 0x00, 0x00, 0x00, 0x00,
       0x00, 0x00, 0x00, 0x00},
      /*policer_enable */ WP_IW_POLICER_DISABLE,
         /**policer_config;*/ NULL,
      /*cong_mode; */ WP_IW_CONGESTION_DISABLE,
      /*cong_thresh_param */ NULL,
      /*flow_agg_type */ WP_IW_AGG_PRIMARY,
      /*external_vlan */
      {
       WP_IW_EXTERN_VLAN_NONE, 0},
      /*bc_mc_priority */
      {
       0, 0, 0, 0},
      /*reserved_agg_mode */ WP_IW_NOT_RESERVED,
      /*next_prsrv_inport */ WP_IW_OVERRIDE_INPORT,
      /*prsrv_inport_params */ WP_IW_OVERRIDE_INPORT,
      /*mac_replace_mode; */ WP_IW_MAC_REPLACE_DISABLED,
      /*new_dst_mac[6]; */
      {
       0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
      /*new_src_mac[6]; */
      {
       0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
      /*hier_cong_prof; */ WP_DISABLE,
      /*extraction_length; */ 0,
      /*policer_handle; */ 0,
      /*vpmt_accessing_mode */
      WP_IW_VPMT_ACCESSING_EGRESS_EXTERNAL,
      /*prefix_rmrk_fields */ 0,
      /*prefix_rmrk_offst */
      {
       0x00, 0x00, 0x00},
      /*forw_dscp_remark */ WP_DISABLE,
      /*dscp_remark_offset; */ 0
   };
   i = 0;
   dedicated_agg_hier_enet_config.txfunc = rx_host_channel;
   dedicated_agg_hier_enet_config.iw_port = bport_host;
   dedicated_agg_hier_enet_config.reserved_agg_mode =
      WP_IW_PPPOE_DISCOVERY_TERMINATION;
   flow_agg_dedicated[i] =
      WP_IwFlowAggregationCreate (0,
                                  WP_IW_VLAN_AWARE_BRIDGING_MODE,
                                  &dedicated_agg_hier_enet_config);
   terminate_on_error (flow_agg_dedicated[i],
                       "WP_IwFlowAggregationCreate");
   i++;
   dedicated_agg_hier_enet_config.txfunc = rx_host_channel;
   dedicated_agg_hier_enet_config.iw_port = bport_host;
   dedicated_agg_hier_enet_config.reserved_agg_mode =
      WP_IW_ARP_TERMINATION;
   flow_agg_dedicated[i] =
      WP_IwFlowAggregationCreate (0,
                                  WP_IW_VLAN_AWARE_BRIDGING_MODE,
                                  &dedicated_agg_hier_enet_config);
   terminate_on_error (flow_agg_dedicated[i],
                       "WP_IwFlowAggregationCreate");
   i++;
   dedicated_agg_hier_enet_config.txfunc = rx_host_channel;
   dedicated_agg_hier_enet_config.iw_port = bport_host;
   dedicated_agg_hier_enet_config.reserved_agg_mode =
      WP_IW_IGMP_TERMINATION;
   flow_agg_dedicated[i] =
      WP_IwFlowAggregationCreate (0,
                                  WP_IW_VLAN_AWARE_BRIDGING_MODE,
                                  &dedicated_agg_hier_enet_config);
   terminate_on_error (flow_agg_dedicated[i],
                       "WP_IwFlowAggregationCreate");
} 

void WPE_SetStaticForwardRules ()
{
   WP_U32 i, j, k;
   WP_handle status;
   WP_l2_forward forward_rule_config = {

      /*dst mac[6]; */
      {
       0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
      /*bport_tag; */ 0,
      /*vlan_id; */ 0,
      /*aggregation; */ 0,
      /*static_mac_mode; */ WP_TRUE,
      /*aging_bit_mode; */ WP_IW_AGING_DISABLE,
      /*process_mode; */ WP_FLOW_BRIDGE_PROCESS_PASS,
      /*deny_mac_sa_mode; */ WP_MAC_SA_PASS,
      /*unknown_mac_mode; */ WP_IW_UNK_MAC_MODE_DISABLED
   };

// -> Enet
   memcpy (forward_rule_config.mac, enet_dst_mac, 6);
   forward_rule_config.bport_tag = BRIDGE_PORT_ENET_TAG;
   forward_rule_config.vlan_id = VLAN_TAG_1;
   forward_rule_config.aggregation = agg_enet[0];
   status = WP_IwMacAddressInsert (iw_sys, &forward_rule_config);
   terminate_on_error (status, "WP_IwMacAddressInsert() to Enet");

// -> Enet (with change of MAC)
   memcpy (forward_rule_config.mac, enet_change_dst_mac, 6);
   forward_rule_config.bport_tag = BRIDGE_PORT_ENET_TAG;
   forward_rule_config.vlan_id = VLAN_TAG_1;
#if 0
   forward_rule_config.aggregation = agg_enet_change_mac;
#else
   forward_rule_config.aggregation = agg_enet[0];
#endif
   status = WP_IwMacAddressInsert (iw_sys, &forward_rule_config);
   terminate_on_error (status,
                       "WP_IwMacAddressInsert() to Enet with change of MAC");

// -> HierarchicalEnet 
   memcpy (forward_rule_config.mac, hier_enet_dst_mac, 6);
   forward_rule_config.bport_tag = BRIDGE_PORT_HIER_ENET_TAG;
   forward_rule_config.vlan_id = VLAN_TAG_1;
   for (i = 0; i < NUM_OF_L1_GROUP /* 1 */ ; i++)
   {
      for (j = 0; j < NUM_OF_L2_GROUP /* 4 */ ; j++)
      {
         for (k = 0; k < NUM_OF_HIER_ENET_FAS_PER_L2/*8*/ ; k++)
         {
            forward_rule_config.aggregation = agg_hier_enet[i][j][k];
            status = WP_IwMacAddressInsert(iw_sys,&forward_rule_config);
            terminate_on_error (status,
                   	"WP_IwMacAddressInsert() -> HierarchicalEnet");
            forward_rule_config.mac[5] ++;
            //forward_rule_config.vlan_id ++;
         }
      }
   }

#if 0
   // -> Multicast to HierarchicalEnet
   memcpy (forward_rule_config.mac, hier_enet_mcast_dst_mac, 6);
   forward_rule_config.bport_tag = BRIDGE_PORT_MC_TAG;
   forward_rule_config.vlan_id = VLAN_TAG_1;

   // Multicast group per VLAN 
   // (no gain if VLAN doesn't have hierarchicalple bports)
   for (i = 0; i < NUM_OF_MC_GROUPS; i++)

   {
      forward_rule_config.aggregation = mc_agg_handle[i];
      status = WP_IwMacAddressInsert (iw_sys, &forward_rule_config);
      terminate_on_error (status,
                          "WP_IwMacAddressInsert() MC to HierarchicalEnet");
      forward_rule_config.vlan_id++;
   }

#endif /*  */
}

// Learning Interrupt Service Thread
// Fills FDB Table entries that look like this:
// MacDA, VLAN -> FlowAgg + (Out bPort)
// The out bPort is used to prevent Loops (when "input bPort" of the packet equals the "Output bPort" found in FDB table)
// and for DFC filtering by outbPort (vs in bPort filtering)
void WPE_LearningIST (WP_tag tag, WP_U32 data, WP_U32 info)
{
   WP_status status;
   WP_U32 return_size, i;
   WP_l2_forward learning_queue[LEARNING_QUEUE_SIZE];

/* Filled values and their default values */
   //learning_queue[i].dst_mac          - OUT - From Packet SA MAC
   //learning_queue[i].bport_tag        - OUT - From Packet In bPort (which becomes out bPort for packets coming back)
   //learning_queue[i].vlan_id          - OUT - VLAN of the Packet or default bPort VLAN tag
   //learning_queue[i].aggregation      - OUT - From memberset table using VLAN, bPort -> FlowAgg
   //learning_queue[i].static_mac_mode  - OUT - WP_FALSE
   //learning_queue[i].aging_bit_mode   - OUT - WP_IW_AGING_ENABLE
   //learning_queue[i].process_mode     - OUT - WP_FLOW_BRIDGE_PROCESS_PASS
   //learning_queue[i].deny_mac_sa_mode - OUT - WP_MAC_SA_PASS
   //learning_queue[i].unknown_mac_mode - OUT - WP_IW_UNK_MAC_MODE_DISABLED

   /* Perform Learning by reading WDDI learning queue */
   status =
      WP_IwLearningQueueService (iw_sys, learning_queue,
                                 LEARNING_QUEUE_SIZE, &return_size);
   terminate_on_error (status, "WPE_LearningEvent()");

   //printf("\n WPE_LearningEvent! Size = %d \n",return_size);
   for (i = 0; i < return_size; i++)

   {

      // set this if you do not want aging 
      learning_queue[i].static_mac_mode = WP_TRUE;
      learning_queue[i].aging_bit_mode = WP_IW_AGING_DISABLE;

      /*printf("MAC WPE_LearningEvent SRC = ");
         for (j=0;j<6;j++) {
         printf("0x%x ",learning_queue[i].mac[j]);
         }
         printf("\n Aggregation = %d, BportTag = %d \n",learning_queue[i].aggregation,learning_queue[i].bport_tag);
       */
      status = WP_IwMacAddressInsert (iw_sys, &learning_queue[i]);
      terminate_on_error (status, "WPE_LearningEvent()");
   }
}

// Table of  VLAN, bPort -> FlowAgg
// the bPort is not passed as a parameter, it is taken from the FlowAgg. and filled automatically.
// This table is used for learning and ingress filtering
// In case the system in not VLAN aware, we still need to do MembersetFill
// for learning propouses, as a database to find FlowAgg by input bPort,
// VLAN filled in this case is always 0
// --- not used now
void WPE_MemeberSetFill (void)
{
   WP_status status;
   WP_flow_learning_rule_vlan bridge_cfg;
   WP_U32 i, j, k;

   //MemeberSetFill Host
   for (i = 0; i < MAX_VLAN_IDS; i++)

   {
      bridge_cfg.flow_agg = agg_host;
      bridge_cfg.vlan_id = VLAN_TAG_1 + i;
      status = WP_IwFlowAdd (iw_sys, WP_FLOW_BRIDGEPATH, &bridge_cfg);
      terminate_on_error (status,
                          "WP_IwFlowAdd() WPE_MemeberSetFill Host");
   }

   //MemeberSetFill Enet
   for (i = 0; i < MAX_VLAN_IDS; i++)

   {
      bridge_cfg.flow_agg = agg_enet[0];
      bridge_cfg.vlan_id = VLAN_TAG_1 + i;
      status = WP_IwFlowAdd (iw_sys, WP_FLOW_BRIDGEPATH, &bridge_cfg);
      terminate_on_error (status,
                          "WP_IwFlowAdd() WPE_MemeberSetFill Enet");
   }

   //MemeberSetFill Hierarchical Enet
   for (i = 0; i < NUM_OF_L1_GROUP; i++)

   {
      for (j = 0; j < NUM_OF_L2_GROUP; j++)

      {
         for (k = 0; k < NUM_OF_HIER_ENET_FAS_PER_L2; k++)

         {
            bridge_cfg.flow_agg = agg_hier_enet[i][j][k];
            bridge_cfg.vlan_id = VLAN_TAG_1 + (i) % MAX_VLAN_IDS;
            status =
               WP_IwFlowAdd (iw_sys, WP_FLOW_BRIDGEPATH, &bridge_cfg);
            terminate_on_error (status,
                                "WP_IwFlowAdd() WPE_MemeberSetFill Hierarchical Enet");
         }
      }
   }
}

// void WPE_MultiCastFlowCreate(void)
// {
//    WPE_MultiCastGroupCreate();
//    WPE_MultiCastFlowAggCreate();
//    WPE_MultiCastMembersFill();
//    WPE_CreateDummyBportMultiCast();
// }

// void WPE_MultiCastGroupCreate(void)
// {
//    WP_U32 i;

//    WP_iw_mc_group mc_group_conf =
//       {
//          /* type */             WP_IW_MC_UNRESTRICTED,
//          /* mode */             0,
//          /* max_iw_mc_members*/ NUM_OF_MC_MEMBERS_PER_GROUP,
//          /* qnode */            0,
//          /* encaps_gap */       0,
//          /* classes_config */   NULL,
//          /* identifier */       WP_UNUSED
//       };

//    for(i=0; i < NUM_OF_MC_GROUPS; i++)
//    {
//       mc_group_conf.qnode = qnode_mc;
//       mc_group_handle[i] = WP_IwMcGroupCreate(WP_WINPATH(0),&mc_group_conf);
//       terminate_on_error(mc_group_handle[i], "WP_IwMcGroupCreate()");
//    }

// }

// void WPE_MultiCastFlowAggCreate(void)
// {
//    WP_U32 i;

//    WP_iw_agg_multicast hier_cast_flow_conf =
//       {
//          /* tag */               agg_tag_counter++,
//          /* type */              WP_IW_FLOW_AGG_PRIMARY,
//          /* mc_group */          0,
//          /* mtu */               MTU_SIZE,
//          /* intmode */           WP_IW_INT_DISABLE,
//          /* interruptqueue */    WP_IW_IRQT1,
//          /* statmode */          WP_IW_STAT_ENABLE,
//          /* timestamp_mode */    WP_IW_TIME_STAMP_DISABLE,
//          /* ov_pool_mode */      WP_IW_OV_POOL_DISABLE,
//          /* fbp_drop_threshold*/ 0,
//          /* policer_enable */    WP_IW_POLICER_DISABLE,
//          /* policer_config */    NULL
//       };

//    for(i=0; i < NUM_OF_MC_GROUPS; i++)
//    {
//       hier_cast_flow_conf.mc_group = mc_group_handle[i];
//       mc_agg_handle[i] = WP_IwFlowAggregationCreate(WP_WINPATH(0), WP_IW_MULTICAST_MODE, &hier_cast_flow_conf);
//       terminate_on_error(mc_agg_handle[i], "WP_IwFlowAggregationCreate() MC");
//    }

// }

// void WPE_MultiCastMembersFill(void)
// {
//    WP_U32 i;

//    WP_iw_mc_member mc_member_config =
//       {
//          /* txfunc */              0,
//          /* service_class_index */ 1,
//          /* buffers_threshold */   0,
//          /* iw_port */             0,
//          /* edit_size */           16,
//          /* data_copy_mode */      WP_IW_MC_MEMBER_DATACOPY_DISABLE,
//          /* iw_port_check_mode */  WP_IW_MC_MEMBER_IWPORT_CHECK_ENABLE
//       };

//    for (i=0; i<NUM_OF_MC_MEMBERS_PER_GROUP; i++)
//    {
//       mc_member_config.txfunc  = agg_hier_enet[i];
//       mc_member_config.iw_port = bport_hier_enet;
//       mc_member_handle[i]      = WP_IwMcMemberAdd(mc_group_handle[0], &mc_member_config);
//       terminate_on_error(mc_member_handle[i], "WP_IwMcMemberAdd() ");
//    }
// }

// void WPE_CreateDummyBportMultiCast(void)
// {

//    WP_bridge_port mc_dummy_bport_config = 
//       { 
//          /* tag */               BRIDGE_PORT_MC_TAG,
//          /* direct_mapping */    WP_IW_DIRECT_MAP_DISABLE, 
//          /* flow_agg */          0,
//          /* flooding_term_mode*/ WP_IW_HOST_TERM_MODE, 
//          /* learning_mode */     WP_IW_LEARNING_ENABLE, 
//          /* in_filter_mode */    WP_IW_INGRESS_FILTER_DISABLE,
//          /* vlan_param */ 
//          { 
//             /* vlan_acceptance_mode */ WP_IW_ACCEPT_TAGGED_UNTAGGED, 
//             /* default_vlan_tag */     VLAN_TAG_1,
//             /* vlan_tunnel;     */     WP_IW_VLAN_TUNNEL_1Q_IN_1Q_DISABLE,
//             /* vlan_pri_enforce_mode*/ WP_IW_VLAN_PRIORITY_ENFORCE_DISABLED
//          },
//          /*max_mac_addresses */  2000,
//          /*group_tag */          WP_IW_BRIDGEALL_GROUP_FILTER,
//          /*group_filtering_mode*/WP_IW_GROUP_FILTER_DISABLE,
//          /*unk_mac_sa_filter*/   WP_IW_UNK_MACSA_FILTER_DISABLE, 
//          /*unk_mc_mode;*/        WP_IW_UNK_MC_HT,
//          /*bc_ht_mode;*/         WP_IW_BC_HT_DISABLE,
//          /*input_filters_mask*/  0, 
//          /*output_filters_mask*/ 0, 
//          /*statmode*/            WP_IW_PORT_STAT_ENABLE,
//          /*unk_uc_mode;*/        WP_IW_UNK_UC_SR_ENABLE,
//          /*classification_flag*/ WP_IW_BPORT_CLASSIFICATION_DISABLED,
//          /*adv_unk_lookup_mode*/ WP_IW_ADV_UNK_LOOKUP_DISABLED,
//          /*cfi_ht_mode;*/        WP_IW_CFI_HT_DISABLE,
//          /*reserved_mc_ht_mode*/ WP_IW_RES_MC_HT_DISABLE,
//          /*predefined_ht_mode*/  WP_DISABLE,
//          /*res_mc_action_table*/ {0,0,0,0,0,0,0,0,0,0},
//          /**enet_oam_params*/    NULL,
//          /* dfc_log_mode*/       0
//       };

//    mc_dummy_bport_config.flow_agg = mc_agg_handle[0];
//    mc_dummy_bport = WP_IwPortCreate(iw_sys, &mc_dummy_bport_config);
//    terminate_on_error(mc_dummy_bport,"WP_IwPortCreate() mc dummy bport");
// }
void WPE_AddBasicFilters (void)
{
   WP_classification_filter filter_param;

   memset (&filter_param, 0, sizeof (wp_classification_filter));

   //filter_param.user_fields_config - Must be set if fields_mask includes WP_FIELD_USER_PROGRAMMABLE
   //filter_param.field_ids_array    - Used when WP_FIELD_IDS_ARRAY is used in the fields_mask field
   //filter_param.protocol_type      - Not used in Bridging Systems
   //filter_param.ip_address_config  - Defines IP address prefix size, when programmable prefix length is used.
   //  ip_option_field_mask;
   //  lpm_dfc_handle;

   // EMC Filter
   filter_param.filter_type = WP_CLASS_FILTER_EMC;
   filter_param.no_match_action = WP_CLASS_FILTER_NO_MATCH_CONTINUE;
   filter_param.fields_mask = WP_FIELD_IDS_ARRAY;
   filter_param.field_ids_array[0] = WP_FIELD_ID_IP_SA_PROGRAMMABLE_PREFIX;   //WP_FIELD_ID_SRC_MAC; //WP_FIELD_ID_IP_SA_PROGRAMMABLE_PREFIX;
   filter_param.field_ids_array[1] = WP_FIELD_ID_LAST;
   filter_param.ip_address_config.ip_sa_prefix_size = 24;
   emc_filter_handle[0] = WP_IwClassFilterAdd (iw_sys, &filter_param);
   terminate_on_error (emc_filter_handle[0], "WP_IwClassFilterAdd");

   // MFC Filter - Must be last
#if MFC
   filter_param.filter_type = WP_CLASS_FILTER_MFC;
   filter_param.no_match_action = WP_CLASS_FILTER_NO_MATCH_ACCEPT;
   filter_param.fields_mask = WP_FIELD_IDS_ARRAY;
   filter_param.field_ids_array[0] = WP_FIELD_ID_IP_SA_PROGRAMMABLE_PREFIX;
   filter_param.field_ids_array[1] = WP_FIELD_ID_LAST;
   filter_param.ip_address_config.ip_sa_prefix_size = 24;
   mfc_filter_handle = WP_IwClassFilterAdd (iw_sys, &filter_param);
   terminate_on_error (mfc_filter_handle, "WP_IwClassFilterAdd");

#endif /*  */
}
void WPE_AddBasicRules (void)
{
   WP_flow_class_rule class_cfg;
   WP_CHAR dfc_rule[WP_IW_RULE_MAX_LENGTH];
   WP_dfc_flow_info dfc_flow_info;

   //***********************************************************************************************************//
   // Rules for EMC filter 0
   /* DENY       rule for the filter */
   memset (&class_cfg, 0, sizeof (WP_flow_class_rule));
   memset (&dfc_flow_info, 0, sizeof (WP_dfc_flow_info));
   memset (&dfc_rule, 0, WP_IW_RULE_MAX_LENGTH);
   sprintf (dfc_rule, "10.0/0.0;0.2/0.0;");

   //sprintf(dfc_rule,  "n;n;n;n;n;n;n;n;0x.0.0.22.22.21;"); // change 21 to 22 to match
   dfc_flow_info.filter_handle = emc_filter_handle[0];
   dfc_flow_info.output_port = 0;
   dfc_flow_info.input_port = 0;
   class_cfg.classifier_string = &dfc_rule[0];
   class_cfg.dfc_info = &dfc_flow_info;
   class_cfg.match_action = WP_FLOW_CLASS_MODE_DENY;
   class_cfg.action_info.forwarding_action.flow_aggregation = 0;
   class_cfg.wred_entry = 0;
   flow_rule_handle[0][0] =
      WP_IwFlowAdd (iw_sys, WP_FLOW_CLASS_RULE, &class_cfg);
   terminate_on_error (flow_rule_handle[0][0],
                       "WP_IwFlowAdd - Basic Rules");

   /* ACCEPT     rule for the filter */
   /* CONTINUE   rule for the filter */
   /* PRIORITY   rule for the filter */
   /* POLICER    rule for the filter */
   /* EXT_ACTION rule for the filter */

#if MFC
   //***********************************************************************************************************//
   // Rules for MFC filter
   /* DENY       rule for the filter */
   memset (&class_cfg, 0, sizeof (WP_flow_class_rule));
   memset (&dfc_flow_info, 0, sizeof (WP_dfc_flow_info));
   memset (&dfc_rule, 0, WP_IW_RULE_MAX_LENGTH);
   sprintf (dfc_rule, "10.0/0.0;0.2/0.0;");

//   sprintf(dfc_rule,  "n;n;n;n;n;n;n;n;0x.0.0.22.22.22;");
   dfc_flow_info.filter_handle = mfc_filter_handle;
   dfc_flow_info.output_port = 0;
   dfc_flow_info.input_port = 0;
   class_cfg.classifier_string = &dfc_rule[0];
   class_cfg.dfc_info = &dfc_flow_info;
   class_cfg.match_action = WP_FLOW_CLASS_MODE_DENY;
   class_cfg.action_info.forwarding_action.flow_aggregation = 0;
   class_cfg.wred_entry = 0;
   flow_rule_handle[1][0] =
      WP_IwFlowAdd (iw_sys, WP_FLOW_CLASS_RULE, &class_cfg);
   terminate_on_error (flow_rule_handle[1][0],
                       "WP_IwFlowAdd - Basic Rules");

   //***********************************************************************************************************//
   /* Prepare Default MFC rule for the filter */
   // FW all to Host FlowAgg
   memset (&class_cfg, 0, sizeof (WP_flow_class_rule));
   memset (&dfc_flow_info, 0, sizeof (WP_dfc_flow_info));
   memset (&dfc_rule, 0, WP_IW_RULE_MAX_LENGTH);
   sprintf (dfc_rule, "0.0/255.255;0.0/255.255;");

//   sprintf(dfc_rule,  "n;n;n;n;n;n;n;n;0x.0.0.22.22.22;");
   dfc_flow_info.filter_handle = mfc_filter_handle;
   dfc_flow_info.output_port = 0;
   dfc_flow_info.input_port = 0;
   class_cfg.classifier_string = &dfc_rule[0];
   class_cfg.dfc_info = &dfc_flow_info;
   class_cfg.match_action = WP_FLOW_CLASS_MODE_FLOW_AGG;
   class_cfg.action_info.forwarding_action.flow_aggregation = agg_host;
   class_cfg.wred_entry = 0;
   flow_rule_handle[1][1] =
      WP_IwFlowAdd (iw_sys, WP_FLOW_CLASS_RULE, &class_cfg);
   terminate_on_error (flow_rule_handle[1][1],
                       "WP_IwFlowAdd - Basic Rules");

#endif /*  */
}

WP_U32 p_got = 0;
WP_U32 p_getnsend = 0;

void WPE_Receive_HostData_IRQ (WP_tag tag, WP_U32 event, WP_U32 info)
{
     WPL_Lock (WPL_THREAD_LOCK_KEY, &eoam_lock);

   g_tag = tag;
   g_event = event;
   g_info = info;
   g_flag = 1;
   g_callback ++;
         if (0 == (iii % 5000))
         {
            printf ("iii(%6d), callback(%6d)\r", iii, g_callback);
         }
     WPL_Unlock(WPL_THREAD_LOCK_KEY, &eoam_lock);
}



void WPE_Receive_HostData_IRQ_X (WP_tag tag, WP_U32 event, WP_U32 info)
{
   WP_data_unit data_unit;
   WP_data_segment segment;
   WP_handle status;

WP_U32 i;
   memset (&data_unit, 0, sizeof (WP_data_unit));
   memset (&segment, 0, sizeof (WP_data_segment));
   data_unit.type = WP_DATA_IW; /* Type of this data unit.          */
   data_unit.segment = &segment; /* Pointer to first segment.        */
   data_unit.n_segments = 1;    /* Number of available segments.    */
   status = WP_HostReceive (rx_host_channel, &data_unit);
   if (status != WP_OK)

   {
      if ((WP_ERROR (status) == WP_ERR_HST_NO_DATA_TO_GET))

      {
         return;
      }

      else
         terminate_on_error (status, "WP_HostReceive Error()");
   }
   p_got ++;
#define DISPLAY_MATRIX  (50)
#if 1
   if (0 == (p_got % DISPLAY_MATRIX))
   {
      printf ("Receive Packet (%10d)\r", p_got);
   }
#endif


   i = 0;

#if 0
   for (i=0;i<data_unit.data_size;i++)
   {
      if(i%16 == 0) printf("\n");
      printf("0x%02x ", data_unit.segment->data[i]);
   }

   printf("\n Data Size = %d \n",data_unit.data_size);
#endif



   status = WP_PoolFree (segment.pool_handle, segment.data);
   terminate_on_error (status, "WP_PoolFree ()");

#if ENABLE_TRANSFER
/*----------------------------------------------------*\
   we are going to make this endlessly !!!
   Transfer packes here.
\*----------------------------------------------------*/
    WPE_Send_HostData(tx_host_channel, WP_DATA_IW, enet_change_dst_mac);
#endif




   return;
}

void WPE_IWSendReceive (WP_U32 packets, WP_U8 dst_mac[])
{
   WP_U32 j;

   //printf("\n ------------ WPE_IWSendReceive -----------------------\n");
   for (j = 0; j < packets; j++)

   {
      WPE_Send_HostData (tx_host_channel, WP_DATA_IW, dst_mac);
   }
   WP_Delay (1000);

   /* status = WP_ChannelQDepth(tx_hdlc_channel_iw[1], &qdepth);
      WPE_TerminateOnError(status, "WP_ChannelQDepth");
      printf("ChannelQDepth = %d\n",qdepth);
    */
   for (j = 0; j < packets; j++)

   {
      WPE_Receive_HostData (rx_host_channel, WP_DATA_IW);
   }
}
void WPE_L2SendReceiveEnet (WP_U32 packets, WP_U8 dst_mac[])
{
   WP_U32 j;

   for (j = 0; j < packets; j++)

   {
      WPE_Send_HostData (tx_gbe_channel[0], WP_DATA_ENET, dst_mac);
   }
   WP_Delay (100000);

   /* status = WP_ChannelQDepth(tx_hdlc_channel_iw[1], &qdepth);
      WPE_TerminateOnError(status, "WP_ChannelQDepth");
      printf("ChannelQDepth = %d\n",qdepth);
    */
   for (j = 0; j < packets; j++)

   {
      WPE_Receive_HostData (rx_gbe_channel, WP_DATA_ENET);  // this rx channel must not have rxbinding!
   }
}
void WPE_Receive_HostData (WP_handle rx_channel, WP_data_type data_type)
{
   WP_data_unit data_unit;
   WP_data_segment segment;
   WP_handle status;

//    WP_U32 i;
   memset (&data_unit, 0, sizeof (WP_data_unit));
   memset (&segment, 0, sizeof (WP_data_segment));
   data_unit.type = data_type;  /* Type of this data unit.          */
   data_unit.segment = &segment; /* Pointer to first segment.        */
   data_unit.n_segments = 1;    /* Number of available segments.    */
   status = WP_HostReceive (rx_channel, &data_unit);
   if (status != WP_OK)

   {
      if ((WP_ERROR (status) == WP_ERR_HST_NO_DATA_TO_GET))

      {
         return;
      }

      else
         terminate_on_error (status, "WP_HostReceive Error()");
   }
   printf ("Receive Packet\n");

//    for (i=0;i<data_unit.data_size;i++)
//    {
//       if(i%16 == 0) printf("\n");
//       printf("0x%02x ", data_unit.segment->data[i]);
//    }
//    printf("\n Data Size = %d \n",data_unit.data_size);
   status = WP_PoolFree (segment.pool_handle, segment.data);
   terminate_on_error (status, "WP_PoolFree ()");
   return;
}

void WPE_Send_HostData (WP_handle tx_channel, WP_data_type data_type,
                        WP_U8 dst_mac[])
{
   WP_handle status;
   WP_data_unit data_unit;
   WP_data_segment segment;
   WP_U32 i;
   WP_U8 hexa_buffer[PACKET_SIZE] =
      //UDP Packet
   {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // MAC DEST
      0x00, 0x00, 0x00, 0x22, 0x22, 0x22, // MAC SRC
      0x08, 0x00, 0x45, 0x00,   // Type, Ver, Len, ToS
      0x00, 0x32, 0x00, 0x00, 0x00, 0x00, 0x40, 0x11, 0xEC, 0x57, // ToT, ID, Frag, TTL, UDPProt, csum
      0x0A, 0x00, 0x00, 0x2C, 0x6E, 0x17, 0x16, 0x21, // SRC IP, DST IP
      0x04, 0x00, 0x96, 0x50, 0x00, 0x1E, 0xD6, 0xFD, // SRC Port, DST Port, Len, csum
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // Payload
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
   };
   memcpy (hexa_buffer, dst_mac, 6);
   memset (&data_unit, 0, sizeof (WP_data_unit));
   memset (&segment, 0, sizeof (WP_data_segment));
   if (data_type == WP_DATA_IW)

   {
      segment.data = WP_PoolAlloc (adjunct_buffer_pool);
      if (segment.data == 0)

      {
         printf ("WPE_SendHostData: adjanct pool is empty\t");
         return;
      }
      segment.pool_handle = adjunct_buffer_pool;
   }

   else                         // L2 Data HSPI enet need DATA_IW only so disable this IF
   {

//       segment.data = WP_PoolAlloc(pool_host);
//       if (segment.data == 0) {
//          printf("WPE_SendHostData: pool_host is empty\t");
//          return;
//       }
//       segment.pool_handle= pool_host;
   }

   segment.data_size       = PACKET_SIZE;
   WP_MEM_BYTES_FILL (segment.data, '0', segment.data_size);
   WP_MEM_BYTES_SET (&segment.data[0], hexa_buffer, PACKET_SIZE);
   data_unit.type          = data_type;  
   data_unit.segment       = &segment; 
   data_unit.n_segments    = 1;    
   data_unit.n_active      = 1;   
   data_unit.data_size     = segment.data_size;  
   data_unit.data_return   = 0;   
   data_unit.control       = WP_HT_CONTROL (0) 
                  | WP_HT_IW_B_MODE_FIELD (WP_HT_IW_B_NORMAL_MODE);   
   data_unit.status        = 0;

   i = 0;


#if 0
   printf ("WP_HostSend \n");
   for (i = 0; i < PACKET_SIZE; i++)
   {
      if (i % 16 == 0)
         printf ("\n");
      printf ("0x%02x ", data_unit.segment->data[i]);
   }
   printf ("\n Data Size = %d \n", data_unit.data_size);

#endif /*  */


   status = WP_HostSend (tx_channel, &data_unit);
   if (WP_ERR_WMM_FIFO_PUT == WP_ERROR (status))

   {
      printf ("WP_HostSend FIFO Put failed\n");
   }

   else if (WP_ERR_WMM_FIFO_GET == WP_ERROR (status))

   {
      status = WP_PoolFree (adjunct_buffer_pool, segment.data);
      terminate_on_error (status, "WP_PoolFree (iw_qnode,segment.data)()");
   }

   else
      terminate_on_error (status, "WP_HostSend ()");
   return;
}




void WPE_ShowForwardingRules (void)
{
   WP_l2_forward forward_rule;
   WP_handle status;

   printf ("\n\nForwarding Rule Entries:\n");
   printf ("------------------------------------\n");
   printf
      ("MAC\t\tVLAN\tBPORT\tFLOW\t\tAGING\tSTATIC\tDENY SA\tPROCESS\tUNK MODE\n");
   status = WP_IwForwardingRuleFindAll (iw_sys, &forward_rule, 0);
   terminate_on_error (status, "WP_IwForwardingRuleFindAll()");
} 

void WPE_DisplayForwardingRule (WP_U32 * l2_forward_rule)
{
   WP_U32 j;
   WP_l2_forward *forward_rule_ptr;

   forward_rule_ptr = (WP_l2_forward *) l2_forward_rule;
   for (j = 0; j < 6; j++)
      printf ("%.2x", forward_rule_ptr->mac[j]);
   printf ("\t");
   printf ("0x%x\t", forward_rule_ptr->vlan_id);
   printf ("0x%x\t", forward_rule_ptr->bport_tag);
   printf ("0x%x\t", forward_rule_ptr->aggregation);
   if (forward_rule_ptr->aging_bit_mode == WP_IW_AGING_ENABLE)
      printf ("On\t");

   else
      printf ("Off\t");
   if (forward_rule_ptr->static_mac_mode == 0x1)
      printf ("On\t");

   else
      printf ("Off\t");
   if (forward_rule_ptr->deny_mac_sa_mode == WP_MAC_SA_DENY)
      printf ("On\t");

   else
      printf ("Off\t");
   switch (forward_rule_ptr->process_mode)

   {
   case WP_FLOW_BRIDGE_PROCESS_PASS:
      printf ("PASS\t");
      break;
   case WP_FLOW_BRIDGE_PROCESS_DENY:
      printf ("DENY\t");
      break;
   case WP_FLOW_BRIDGE_PROCESS_SVL_UC_PASS:
      printf ("SVL\t");
      break;
   case WP_FLOW_BRIDGE_PROCESS_RES_MC_TERM:
      printf ("RES MC\t");
      break;
   case WP_FLOW_BRIDGE_PROCESS_DIRECT_TERM:
      printf ("DIRECT\t");
      break;
   default:
      printf ("(%d)\t", forward_rule_ptr->process_mode);
      break;
   }
   if (forward_rule_ptr->unknown_mac_mode == WP_IW_UNK_MAC_MC)
      printf ("UnkMC");

   else if (forward_rule_ptr->unknown_mac_mode == WP_IW_UNK_MAC_UC)
      printf ("UnkUC");

   else
      printf ("Off");
   printf ("\n");
   return;
}
void WPE_ShowAvailableBusesMemory (void)
{
   WP_U32 bytes;
   WP_handle status;

   status =
      WP_MemoryAvailable (WP_WINPATH (0), WP_BUS_PACKET,
                          APP_BANK_PACKET, &bytes);
   printf ("Packet bus bytes left = %d\n", bytes);
   status =
      WP_MemoryAvailable (WP_WINPATH (0), WP_BUS_PARAM,
                          APP_BANK_PARAM, &bytes);
   printf ("Param bus bytes left = %d\n", bytes);
   status =
      WP_MemoryAvailable (WP_WINPATH (0), WP_BUS_INTERNAL,
                          APP_BANK_INTERNAL, &bytes);
   printf ("Internal bus bytes left = %d\n", bytes);
   status =
      WP_MemoryAvailable (WP_WINPATH (0), WP_BUS_HOST,
                          APP_BANK_HOST, &bytes);
   printf ("Host bus bytes left = %d\n", bytes);
} 


WP_CHAR *WUFE_error_name[] = {

    "WUFE_OK",
    "WUFE_ERR_HANDLE",
    "WUFE_ERR_NULL_POINTER",
    "WUFE_ERR_INVALID_WUFE_ID",
    "WUFE_ERR_INVALID_REGISTRY_STATE",
    "WUFE_ERR_INVALID_REGISTRY_COMMIT_ACTION",
    "WUFE_ERR_INVALID_DIRECTION",
    "WUFE_ERR_INVALID_CONFIG",
    "WUFE_ERR_HW_CONFIG_STATE_MACHINE",
    "WUFE_ERR_HW_CONFIG_CORE_ENABLED",
    "WUFE_ERR_HW_CONFIG_NOT_MATCH",
    "WUFE_ERR_HW_PLL_LOCK",
    "WUFE_ERR_HW_RESET",
    "WUFE_ERR_NOT_IMPLEMENTED",
    "WUFE_ERR_HW_CONFIG_FIFO_MEMORY_NOT_MATCH",
    "WUFE_ERR_HW_CONFIG_FIFO_MEMORY_SIZE",
    "WUFE_ERR_HW_CONFIG_INTERNAL_ZBT_MUST_BE_128K",
    "WUFE_ERR_INVALID_GPIO_CLOCK",
    "WUFE_ERR_UFE_INIT_BEFORE_UPI_ENABLE",
    "WUFE_ERR_EVENT_INVALID_CONFIG",
    "WUFE_ERR_SYSTEM_RECONFIG_CMD",
    "WUFE_ERR_SYSTEM_RECONFIG_SBI_SPE_LINES_EXIST",
    "WUFE_ERR_MAPPING_NOT_INITIALIZED",
    "WUFE_ERR_MAPPING_DUPLICATE",
    "WUFE_ERR_MALLOC",
    "WUFE_NOT_ENOUGH_MEM_FOR_BUFFER",
    "WUFE_NOT_ENOUGH_RX_BUFFERS",
    "WUFE_NOT_ENOUGH_TX_BUFFERS",
    "WUFE_ERR_MEM_INCONSIST",
    "WUFE_ERR_ILLEGAL_EMPHY_PRI_RX_VALUE",
    "WUFE_ERR_ILLEGAL_EMPHY_PRI_TX_VALUE",
    "WUFE_ERR_ILLEGAL_EMPHY_PRI_TX_INFRAME_VALUE",
    "WUFE_ERR_ILLEGAL_EMPHY_PARITY",
    "WUFE_ERR_PARITY_CHECK_NOT_SUPPORTED",
    "WUFE_ERR_SYSTEM_ALREADY_DISABLED",
    "WUFE_ERR_SYSTEM_NOT_DISABLED",
    "WUFE_ERR_EMPHY_CORE1_OFFSET_CONFIG",
    "WUFE_ERR_ILLEGAL_CAS_PATTERN",
    "WUFE_ERR_NO_SYSTEM_CREATED",
    "WUFE_ERR_SYSTEM_NOT_ENABLED",
    "WUFE_ERR_ILLEGAL_TDM_PARITY",
    "WUFE_ERR_ILLEGAL_NUM_LINES",
    "WUFE_ERR_LINE_INVALID_TYPE",
    "WUFE_ERR_CAD_ID_FOR_TRANSFER_TYPE",
    "WUFE_ERR_LINE_ILLEGAL_ID",
    "WUFE_ERR_HMVIP_T1_MUST_BE_FRAMED",
    "WUFE_ERR_DS3_T3_MUST_BE_UNFRAMED",
    "WUFE_ERR_INVALID_TRANSFER_TYPE",
    "WUFE_ERR_CAD_INVALID_CFG",
    "WUFE_ERR_SPE_NOT_EMPTY",
    "WUFE_ERR_CAD_SPE_FULL_OR_WITH_SPACES",
    "WUFE_ERR_INCONSISTENT_SPE_ARRAY",
    "WUFE_ERR_LINES_NOT_SORTED_IN_DECREASING_ORDER",
    "WUFE_ERR_LINE_ALREADY_ACTIVE",
    "WUFE_ERR_LINE_ALREADY_DISABLED",
    "WUFE_ERR_LINE_NOT_DISABLED",
    "WUFE_ERR_LINE_NOT_ENABLED",
    "WUFE_ERR_LINE_EMPTY",
    "WUFE_ERR_LINE_ALREADY_CONFIGURED",
    "WUFE_ERR_LINE_CONTAIN_PHYS",
    "WUFE_ERR_LINE_CONTAIN_ACTIVE_PHYS",
    "WUFE_ERR_LINE_RECONFIG",
    "WUFE_ERR_LINE_MALFUNCTION",
    "WUFE_ERR_LINE_LOOPBACK_TYPE_LINE_NOT_ENABLED_IN_HW",
    "WUFE_ERR_LINE_LOOPBACK_TYPE_SYSTEM_NOT_ENABLED_IN_HW",
    "WUFE_ERR_LINE_INVALID_FPGA",
    "WUFE_ERR_INVALID_TUG2_TRANSFER_TYPE",
    "WUFE_ERR_ENTRY_NOT_EMPTY",
    "WUFE_ERR_VC44C_STM1_STM0_TUG2_TU_MUST_BE_ZERO",
    "WUFE_ERR_VC4_STM0_TUG2_TU_MUST_BE_ZERO",
    "WUFE_ERR_VC3_TUG2_TU_MUST_BE_ZERO",
    "WUFE_ERR_CAS_INVALID_CFG",
    "WUFE_ERR_DS3_SUB_SPE_MUST_BE_ZERO",
    "WUFE_ERR_INDIRECT_DATA_SET_TIMEOUT",
    "WUFE_ERR_INDIRECT_DATA_GET_TIMEOUT",
    "WUFE_SFP_ACCESS_ERROR",
    "WUFE_ERR_MAX_SBI_CT",
    "WUFE_ERR_SBI_CT_NOT_INITIALIZED",
    "WUFE_ERR_SBI_CT_ID_NOT_EXIST",
    "WUFE_ERR_SBI_CT_ID_NOT_MATCH_TRIB_TYPE",
    "WUFE_ERR_SBI_CT_TRIB_TYPE_NOT_MATCH",
    "WUFE_ERR_SBI_CT_LINE_NOT_FOUND",
    "WUFE_ERR_SBI_CT_MORE_THAN_ONE_ACTIVE",
    "WUFE_ERR_CLOCK_REC_NUM_RX_IF_INVALID",
    "WUFE_ERR_CLOCK_REC_NUM_TX_IF_INVALID",
    "WUFE_ERR_CLOCK_REC_INVALID_LINE_IN_EXTENDED_MODE",
    "WUFE_ERR_CLOCK_REC_HW_NOT_SUPPORTED",
    "WUFE_ERR_CLOCK_REC_NOT_ENABLED",
    "WUFE_ERR_CLOCK_REC_SYSTEM_PARAMS",
    "WUFE_ERR_CLOCK_REC_LINE_PARAMS",
    "WUFE_ERR_CLOCK_REC_NO_MASTER_PHY",
    "WUFE_ERR_CLOCK_REC_ACTIVE_SLAVE_PHY",
    "WUFE_ERR_CLOCK_REC_METHOD_INVALID",
    "WUFE_ERR_CLOCK_REC_INVALID_INFO_TYPE",
    "WUFE_ERR_CLOCK_REC_INVALID_CONFIG",
    "WUFE_ERR_CLOCK_REC_INVALID_PROTOCOL_CONFIG",
    "WUFE_ERR_CLOCK_REC_INVALID_TS_WIDTH",
    "WUFE_ERR_CLOCK_REC_INVALID_PHY_ID",
    "WUFE_ERR_CLOCK_REC_INVALID_TRANSFER_TYPE",
    "WUFE_ERR_CLOCK_REC_INVALID_FRAMING_MODE",
    "WUFE_ERR_CLOCK_REC_INVALID_DATA_UNIT_SIZE",
    "WUFE_ERR_CLOCK_REC_NO_FREE_RAM_ENTRY",
    "WUFE_ERR_CLOCK_REC_CGEN_LINE_NOT_FOUND",
    "WUFE_ERR_CLOCK_REC_INVALID_DIRECTION",
    "WUFE_ERR_CLOCK_REC_PHYSICAL_CLOCK_ALREADY_ACTIVE",
    "WUFE_ERR_CLOCK_REC_INVALID_PHYSICAL_CLOCK_ID",
    "WUFE_ERR_ILLEGAL_NUM_PHYS",
    "WUFE_ERR_PHY_INVALID_TYPE",
    "WUFE_ERR_INVALID_FIFO_CFG",
    "WUFE_ERR_SLOT_CONFIG",
    "WUFE_ERR_NO_FREE_PHY",
    "WUFE_ERR_ILLEGAL_PHY_ID",
    "WUFE_ERR_PHY_CONFIG",
    "WUFE_ERR_PHY_NOT_CONNECTED",
    "WUFE_ERR_PHY_ACTIVE",
    "WUFE_ERR_PHY_DISABLED",
    "WUFE_ERR_PHY_NOT_DISABLED",
    "WUFE_ERR_PHY_ALREADY_DISABLED",
    "WUFE_ERR_PHY_FIFO_NOT_EMPTY",
    "WUFE_ERR_PHY_ASSIGNED_ERR",
    "WUFE_ERR_PHY_SUM_MAX_COUNT_TOO_BIG",
    "WUFE_ERR_FIFO_INCONSIST_FBP_COUNT",
    "WUFE_ERR_FIFO_INCONSIST_FBP_PTR",
    "WUFE_ERR_PHY_NOT_ATM",
    "WUFE_ERR_PHY_ACTIVE_MASTER",
    "WUFE_ERR_PHY_INVALID_FPGA",
    "WUFE_ERR_DDR_NOT_READY",
    "WUFE_ERR_MAX_BUFF_SIZE_NOT_IMPLEMENTED",
    "WUFE_ERR_EXT_PLL_WRONG_CONFIG",
    "WUFE_ERR_TEMUX_START",
    "WUFE_ERR_TEMUX_1",
    "WUFE_ERR_TEMUX_2",
    "WUFE_ERR_TEMUX_3",
    "WUFE_ERR_TEMUX_4",
    "WUFE_ERR_TEMUX_5",
    "WUFE_ERR_TEMUX_6",
    "WUFE_ERR_TEMUX_7",
    "WUFE_ERR_TEMUX_8",
    "WUFE_ERR_TEMUX_9",
    "WUFE_ERR_TEMUX_10",
    "WUFE_ERR_TEMUX_11",
    "WUFE_ERR_TEMUX_12",
    "WUFE_ERR_TEMUX_13",
    "WUFE_ERR_TEMUX_14",
    "WUFE_ERR_TEMUX_15",
    "WUFE_ERR_TEMUX_16",
    "WUFE_ERR_TEMUX_17",
    "WUFE_ERR_TEMUX_18",
    "WUFE_ERR_TEMUX_19",
    "WUFE_ERR_TEMUX_20",
    "WUFE_ERR_TEMUX_21",
    "WUFE_ERR_TEMUX_22",
    "WUFE_ERR_TEMUX_23",
    "WUFE_ERR_TEMUX_24",
    "WUFE_ERR_TEMUX_25",
    "WUFE_ERR_TEMUX_26",
    "WUFE_ERR_TEMUX_27",
    "WUFE_ERR_TEMUX_28",
    "WUFE_ERR_TEMUX_29",
    "WUFE_ERR_TEMUX_30",
    "WUFE_ERR_TEMUX_31",
    "WUFE_ERR_TEMUX_32",
    "WUFE_ERR_TEMUX_33",
    "WUFE_ERR_TEMUX_34",
    "WUFE_ERR_TEMUX_35",
    "WUFE_ERR_TEMUX_36",
    "WUFE_ERR_TEMUX_37",
    "WUFE_ERR_TEMUX_38",
    "WUFE_ERR_TEMUX_39",
    "WUFE_ERR_PMC_ARROW622_CONFIG",
    "WUFE_ERR_PMC_ARROW622_RESET",
    "WUFE_ERR_FLEXMUX_FAILURE_COMMUNICATION_WITH_FRAMER",
    "WUFE_ERR_FLEXMUX_FAILURE_DEVICE_NOT_READY",
    "WUFE_ERR_FLEXMUX_FAILURE_BUILD_PERSONALITY",
    "WUFE_ERR_INVALID_PLL_CLOCK_RATE",
    "Undefined message"
}; 




void WPE_CLI (void)
{
   char InputBuf[256];
   int i = 0;
   static int flag = 1;
   unsigned int tmp = 0;

   InputBuf[0] = '\0';
   while ((InputBuf[0] != 'q') && (InputBuf[0] != 'k'))

   {
      printf ("\n");
      printf
         ("Enter: Enet->HierarchicalEnet: \n \t\t\t1-EnetPortDev(ENET4/ENET7/EXGRESS),       \n \t\t\t2-bPortEnet,       \n \t\t\t3-FlowAggHierarchicalEnet,  \n");
      printf
         ("       HierarchicalEnet->Enet: \n \t\t\t4-HierarchicalEnetPortDev(ENET3/ENET8/INGRESS),  \n \t\t\t5-bPoprtHierarchicalEnet, \n \t\t\t6-FlowAggEnet,       \n");
      printf
         ("       HostTermination: \n \t\t\t7-bPortHost,  \n \t\t\t8-FlowAggHost, \n \t\t\tq-Exit, \n \t\t\tr-Reboot, \n \t\t\tk-Kill(noDriverRelease) \n");

      printf
         ("       packet control : \n \t\t\tp-send packets\n");
      printf
         ("       QDepth control : \n \t\t\tb-get WP_ChannelQDepth(tx_gbe_channel[0])\n");
      printf
         ("\t\t\ta-get all statistics)\n");
      printf ("\t\t\t9-disable/enable thread)\n");
      printf ("\t\t\tc-print all error_name)\n");
      printf ("\t\t\td-print all wufe_error_name)\n");
      printf ("\t\t\te-(switch NES on the EGRESS port)\n");
      printf ("\t\t\tg-(adjust the delay(looply)\n");

#if 0
      gets (InputBuf);
#else
	do {
      InputBuf[0] = getchar ();
	}
	while (!('a' <=  InputBuf[0]  && 'z' >= InputBuf[0]) &&  !('0' <= InputBuf[0] && '9' >= InputBuf[0]));
#endif
      switch (InputBuf[0])

      {
      case '1':
         printf
            ("************************* System Stats for Enet Port ********************** \n");
         WPE_DisplayIWSystemStats (port_enet);
         printf
            ("************************* Enet Device Stats ******************************* \n");
         WPE_DisplayEnetDeviceStat (dev_enet);
         break;
      case '2':
         printf
            ("************************* Enet bPort Stats ******************************** \n");
         WPE_DisplayIWPortStats (bport_enet);
         break;
      case '3':
         printf
            ("************************* HierarchicalEnet FlowAgg[0] Stats ********************** \n");
	printf ("agg_enet_change_mac statics\n");
         WPE_DisplayFlowAggStats (agg_enet_change_mac);
	printf ("agg_hier_enet statics\n");
         WPE_DisplayFlowAggStats (agg_hier_enet[0][0][0]);

         break;
      case '4':
         printf
            ("************************* System Stats for HierarchicalEnet Port ***************** \n");
         WPE_DisplayIWSystemStats (port_hier_enet);
         printf
            ("************************** HierarchicalEnet Device Stats ************************* \n");
         WPE_DisplayEnetDeviceStat (dev_hier_enet);
         break;
      case '5':
         printf
            ("************************* HierarchicalEnet bPort Stats *************************** \n");
         WPE_DisplayIWPortStats (bport_hier_enet);
         break;
      case '6':
         printf
            ("************************* Enet FlowAgg Stats ****************************** \n");
         WPE_DisplayFlowAggStats (agg_enet[0]);
         break;
      case '7':
         printf
            ("************************* Host bPort Stats ******************************** \n");
         WPE_DisplayIWPortStats (bport_host);
         break;
      case '8':
         printf
            ("************************* Host FlowAgg Stats ****************************** \n");
         WPE_DisplayFlowAggStats (agg_host);
         break;
      case '9':
	 if (g_threadStop)
		g_threadStop = 0;
	 else
		g_threadStop = 1;
         break;
      case 'a':
         printf
            ("********************* WP_DISPLAY **********************   \n");
         WP_Display (0, WP_DISPLAY_DEVICE, WP_DISPLAY_DEFAULT, 0);
         break;
      case 'b':
	{
	WP_U32 depth = 0;
         WP_ChannelQDepth(tx_gbe_channel[0], &depth);
	printf ("QDepth (%x)\n", depth);
	}
         break;
      case 'c':
	{
		WP_U32 errno = 0;
		for (errno = WP_OK; errno <= WP_TOTAL_ERRORS; errno ++)
		{
			printf ("id(0x%3x)(%4d)=errorname(%s)\n", errno, errno, WP_error_name[(errno)]);
		}
		break;
	}
      case 'd':
	{
		WP_U32 errno = 0;
		for (errno = WUFE_ERR_HANDLE; 
			errno <= WUFE_ERR_INVALID_PLL_CLOCK_RATE; 
			errno ++)
		{
			printf ("id(0x%3x)(%4d)=errorname(%s)\n", 
				errno, errno, 
				WUFE_error_name[WP_ERROR(errno)]);
		}
		break;
	}
      case 'e':

#define ME_FIELD_MASK(S, W)	(((1 << W) - 1) << (S))
#define SERDES_NES_CTRL	MAP_SERDES4_IO_CTRL_4
#define SERDES_IO_CTRL_MASK_ME	(0x003e0000)

	tmp = *(volatile unsigned int *)(unsigned long)(SERDES_NES_CTRL	+ WPL_RIF_VIRTUAL(0, 0));

	printf ("current 0x1e0082c4=(%x)\n", tmp);
	if (flag)
	{
		tmp = tmp & (~(ME_FIELD_MASK(5, 5)));
		tmp = tmp | (0x04 << 5);
		flag = 0;
		printf ("set NES\n");
	} else {
		tmp = tmp & (~(ME_FIELD_MASK(5, 5)));
		flag = 1;
		printf ("clear NES\n");
	}
 	printf ("WPL_RIF_VIRTUAL=(%x)\n", WPL_RIF_VIRTUAL(0,0));
	*(volatile unsigned int *)(unsigned long)(SERDES_NES_CTRL + WPL_RIF_VIRTUAL(0, 0)) = tmp;
	break;
      case 'f':
         printf
            ("********************* SHOW FDB **********************   \n");
         WPE_ShowForwardingRules ();
         break;
      case 's':
         printf
            ("********************* WPE_ShowAvailableBusesMemory **********************   \n");
         WPE_ShowAvailableBusesMemory ();
         break;
      case 'g':
	jjj = (jjj + 1) % 10;
	printf ("current jjj=(%d)\n", jjj);
	break;
      case 'p':
#if 1
   for (i = 0; i < 10; i++)

   {

      WPE_Send_HostData(tx_host_channel, WP_DATA_IW, enet_change_dst_mac);
      //WPE_IWSendReceive (1, enet_change_dst_mac);
   }
#endif
         break;
      case 'q':
         printf
            ("************************* Exiting with Driver Release ********************* \n");
         WP_DriverRelease ();
         break;
      case 'r':
         WPX_Reboot ();
         break;
      case 'k':
         printf
            ("************************* Exiting Driver Still Running! ******************* \n");
         break;
      case 'w':
        {
        WP_CHAR InputBuf[255];
        
        printf ("Enter winutil command >\n");
        get_line(InputBuf);
        printf("\n");
        WPUI_RunWinMon(InputBuf);
	  }
	  break;
      default:
         printf ("Wrong Key(%x).\n\n", InputBuf[0]);
         break;
      }
   }
}
static void WPE_LogErrCb (const WP_CHAR * file, const WP_CHAR * function,
                          WP_S32 line, WP_U32 error_id)
{
   printf ("ERR %s:%s:%d - %s\n", file, function, line,
           WP_error_name[error_id]);
}

#if 0
static void WPE_LogFunctionCb (WP_CHAR * pi_file, WP_CHAR * pi_func,
                               WP_U32 i_line)
{
   printf ("CALL %s:%s:%d\n", pi_file, pi_func, i_line);
} 


static void WPE_LogCb (WP_S32 i_lvl, const WP_CHAR * pi_string)
{
   printf ("LOG [%d]: %s", i_lvl, pi_string);
}
#endif /*  */

// To enable WDDI Log Register these callbacks and:
// in sources/api/wp_debug.h WP_DEBUG_ENABLE_WDDI_LOG 1
// in target/mips_linux/linux_make.inc LOG_LEVEL = WPI_DEBUG_LEVEL_10
void WPE_RegisterLogCbFunc ()
{
   WP_handle status;

   status =
      WP_ControlRegister (WP_DEBUG_CALLBACK_FILE_LINE_ERROR, WPE_LogErrCb);
   terminate_on_error (status, "WP_ControlRegister() ErrorIndicateEvent");

   //status = WP_ControlRegister(WP_DEBUG_CALLBACK_WDDI_CALL, WPE_LogFunctionCb);
   //terminate_on_error(status, "WP_ControlRegister() ErrorIndicateEvent");

   //status = WP_ControlRegister(WP_DEBUG_CALLBACK_WDDI_LOG, WPE_LogCb);
   //terminate_on_error(status, "WP_ControlRegister() ErrorIndicateEvent");
} 



void terminate_on_error (WP_handle handle, WP_CHAR * s)
{
   if (WP_ERROR_P (handle))

   {
      printf (" %s\n", s);

      //printf(" %s\n", WP_error_name[WP_ERROR(handle)]); // need to call this only when WP_DEBUG_CALLBACK_FILE_LINE_ERROR not used
      WP_DriverRelease ();
      exit (1);
   }

   else

   {

#ifdef DEBUG_PRINT
      printf ("Handle/Status returned from %s is 0x%x \n", s, handle);

#endif /*  */
   }
}


extern void WPI_HwWinnetSgmiiAnProceed(WP_U32 wpid, WP_U32 event_bits);

void *LearningPoll(void *i)
{
	WP_U8 iii = 0;
	WP_handle status = 0;
	WP_U8 max_ch_tx = 0;
	WP_U8 max_ch_rx = 0;
	static WP_U32 switch_counter = 0;

   WP_port_enet my_enet_port_config = {
      {
       		/* max_tx_channels */ 1,
       		/* max_rx_channels */ 1,
       		/* pq_block_size */ 0,
       		/* n_pq_blocks */ 0,
       		/* emphy_devices */ WP_UNUSED
       },
      /* flowmode */ WP_FLOWMODE_FAST,
      /* interface_mode */ WP_ENET_1000_BASE_X,
      /* rx_iw_bkgnd */ 0,
   	};


	max_ch_tx = NUM_OF_FAST_ENET_TX_CHANNELS;
	max_ch_rx = NUM_OF_FAST_ENET_RX_CHANNELS;
   	my_enet_port_config.pkt_limits.max_tx_channels = max_ch_tx;
   	my_enet_port_config.pkt_limits.max_rx_channels = max_ch_rx;
   	my_enet_port_config.flowmode = WP_FLOWMODE_FAST;   


	WPL_IntConnect(0, WPL_SgmiiAn, NULL, 
				WPI_HwWinnetSgmiiAnProceed);
	while (1)
   	{
		switch_counter  ++;
		if (WP_ENET_1000_BASE_X == interface_mode)
		{
			interface_mode = WP_ENET_SGMII_AN;
		} else {
		 	interface_mode = WP_ENET_1000_BASE_X;
		}
   		my_enet_port_config.interface_mode = interface_mode;


////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
///////////////////// here is the delay ////////////////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
		for (iii = 0; iii < jjj; iii ++)
		{
			WPL_Delay(DELAY_COUNT);
			if (2 == iii)
			{
			}
		}

		if (g_threadStop)
		{
			continue;
     		}
g_threadStop = 1;
		printf ("change to (%s)count(%10d)\n", (WP_ENET_1000_BASE_X==interface_mode)?"WP_ENET_1000_BASE_X\t":"WP_ENET_SGMII_AN\t", switch_counter);

		if (WP_ENET_SGMII_AN == interface_mode)
		{
			WPL_IntEnable (0, WPL_SgmiiAn);
			printf ("enable SgmiiAn interrupt\n");
		} else {
			WPL_IntDisable (0, WPL_SgmiiAn);
			printf ("disable SgmiiAn interrupt\n");
		}
		WPL_Lock (WPL_THREAD_LOCK_KEY, &eoam_lock);

		printf ("polling again(%d), enter to show the MENU\n", 
				iii++);
		status = WP_DeviceDisable(dev_enet,WP_DIRECTION_DUPLEX);
   		terminate_on_error (status, "WP_DeviceDisable");
		status = WP_PortDisable(port_enet, WP_DIRECTION_DUPLEX);
   		terminate_on_error (status, "WP_PortDisable");

		status = WP_PortModify(port_enet, WP_PORT_MOD_ENET_INTERFACE_MODE, &my_enet_port_config);
   		terminate_on_error (status, "WP_PortModify");

		status = WP_PortEnable(port_enet, WP_DIRECTION_DUPLEX);
   		terminate_on_error (status, "WP_PortEnable Enet");
		status = WP_DeviceEnable(dev_enet, WP_DIRECTION_DUPLEX);
   		terminate_on_error (status, "WP_DeviceEnable Enet");

		WPL_Unlock(WPL_THREAD_LOCK_KEY, &eoam_lock);
   	}
}

#ifdef  __linux__
int get_line(char *buf)
{
        int ch, index, done, i;
        char line[256];
    
        done = 0;
        index = 0;
        memset(line, 0, sizeof(line));
        
        /* Key reading loop */
        while (!done) {
                ch = getchar();
                putchar(ch);
                
                switch (ch)
                {
                case KEY_ENTER:
                        /* skip space */
                        i = 0;
                        while (!(isalpha(line[i]) || isdigit(line[i])) && (i < index)) i++;

                        index -= i;
                        memcpy(buf, line + i, index);
                        buf[index] = '\0';
                        done = 1;
                        break;
                case KEY_BACKSPACE:
                        if (index > 0)
                                index--;
                        printf("\33[K"); 
                        break;
                default:
                        if (index >= 256)
                        {
                                done  = 1;
                                index = 0;
                                break;
                        }
                        
                        line[index++] = ch;
                }
        }

        return index;
}
#else
int get_line(char *buf)
{
#ifdef WP_HW_WINPATH3
    flush();
#endif
    gets(buf);
    
    return strlen(buf);    
}

#endif
