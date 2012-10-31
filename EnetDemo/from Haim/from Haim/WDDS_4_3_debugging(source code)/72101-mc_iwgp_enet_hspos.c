
/*****************************************************************************
 * (C) Copyright 2000-2009, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/
#if _WT_MODULE_TEST_COMMENT_ENABLE_
/* This test has been converted to work on Linux.  See
   WDDI_TestCodingGuidlines.doc.  */
#pragma check_tests pool_alloc 1
#endif

/****************************************************************************
 *
 * Unique Test Number: 72101
 * Test Name: 72101-mc_iwgp_enet_hspos.c
 * Test Setup:
 *       1.
 *       2.
 *
 * Test Common configurated system:
 *
 * Test Description:
 *         This application tests Multicast functionality with the new IWGP
 *         on Winpath-3. It covers most of the MC API functions and modifications.
 *         In addition it checks memory usage and maximum mmebers creation.
 * Remarks:
 *
 *
 ***************************************************************************/
#define TEST_DEBUG 0
#define UPI_MIXED_SHAPING 1
#define CHECK_FIWT 0              /*Always set to 0 in check-in mode*/
#define CHECK_DATA_COPY_SCENARIO 1
#define CHECK_SCENARIO_6 1        /*This is host termination scenario - compatible interface is needed*/
#define VARIOUS_PACKET_LENGTHS 1 /*Check different packet lengths*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "wt_util.h"
#include "wt_partition_ecc_util.h"

#include "wp_wddi.h"
#include "wpx_app_data.h"
#include "app_convert.h"
#include "wpx_enet_phy.h"
#include "api/wp_pce.h"
#include "wpu_debug_sw.h" //phil


#ifdef USE_SOCKET
#include <unistd.h>
#include "sockint.h"
#include "wp_sim.h"
#else
#include "wpx_board_data.h"
#endif
#include "iiw_callback.h"


#define NUMBER_OF_ENET_PORTS 4
#define MAX_ENET_TX          8
#define NUMBER_OF_UPI_PORTS  1
#define MAX_UPI_TX           8
#define MAX_MC_GROUPS        4100
#define MAX_MC_MEMBERS       1048577

#define SYSTEM_MAX_FLOWS     ((NUMBER_OF_ENET_PORTS*MAX_ENET_TX)+(NUMBER_OF_UPI_PORTS*MAX_UPI_TX)+MAX_MC_GROUPS+5)
#define MAX_BPORTS_SYS       (NUMBER_OF_ENET_PORTS+MAX_UPI_TX+1)
#define MAX_MEMBERS_PER_GROUP 40
#define POOL_LIMIT           8
#define PQ_BLOCK_SIZE        8
#define LEARNING_QUEUE_SIZE  20
#define DATA_LENGTH          1500
#define APP_HOST_BUFFER_SIZE 2048
#define MAX_NUM_OF_FILTERS 5
#define MAX_NUMBER_OF_RULES 5000
#define SHAPING_BLOCK_SIZE 8

#define WPE_ERR 1
#define WPE_GOOD 0


extern WP_U32 dps_ProgramImage[];
extern WP_U16 dps_PC_Table_Init[];

extern WP_pce_parser_info pce_parser_info;
#define INT_QUEUE_SIZE  (100)
#define RATIO           (1)
static WP_int_queue_table int_queue_tables = {{{INT_QUEUE_SIZE, RATIO           }, {INT_QUEUE_SIZE, RATIO}, {INT_QUEUE_SIZE, RATIO}, {INT_QUEUE_SIZE, RATIO}}};

static WP_atm_global atm_params[1] =
{
   {
      /* max_pm_tables */ 0,
      /* max_ubrplus_channels */ 0,
      /* max_upc_tables */ 0,
      /* max_rx_cids */ 0,
      /* max_tx_cids */ 10,
      /* max_cid_range */ 10,
      /* AAL2 QSR bus */ WP_BUS_NONE,
      /* AAL2 QSR bank */ APP_BANK_HOST,
      {
         WP_ATM_RAS_TIMESTAMP_DISABLE
      },
   }
};


static WP_iw_global  max_iw[1] ={
   {
      /* max_iw_aggregations */ SYSTEM_MAX_FLOWS,
      /* max_r_iw_systems;*/ 0,
      /* max_s_iw_systems;*/ 0,
      /* max_b_iw_systems;*/ 1,
      /* max_m_iw_systems;*/ 0,
      /* max_vlan_priority_maps */0,
      /* iw_bkgnd_fifo_size */0,
      /* cong_pt       */NULL,
      /* iw_host_limits */ {64,8,4,1,0},
      /* mpls_config*/ {0,0},
      /* iw_modes;*/
      {  /*policer_mode;*/WP_IW_POLICER_DISABLE,
         /*statistics_bundle_stat_mode;*/WP_IW_STATISTICS_BUNDLE_STAT_ENABLE,
         /*l2_prefix_ext_mode;*/WP_IW_L2_HEADER_EXTENSION_DISABLE,
         /*enhanced_flow_stat_mode;*/WP_IW_ENHANCED_FLOW_STAT_DISABLE,
         /*flow_stat_mode;*/WP_IW_FLOW_STAT_ENABLE,
         /*fr_tx_flow_stat_mode*/WP_IW_FR_TX_FLOW_STAT_DISABLE,
         /*mfc_alloc_mode*/  WP_IW_MFC_ALLOC_DISABLE,
         /*learning_queue_mode */WP_IW_LEARNING_DISABLE,
         /*port_filtering_mode */WP_IW_PORT_FILTERING_ENABLE,
         /*lpm_alloc_mode;*/0,
         /*enhanced_mc_member_stat_mode;*/0,
         /*routing_alloc_modes;*/0,
         /*res_mc_addresses_table_mode;*/0,
         /*portag_etype_mode;*/0,
         /*iw_characteristics*/WP_IW_MODES_IWGP_BACKGROUND_ENABLE
      },
      /* max_iw_stat_bundles*/ 0,
      /* max_fr_s_iw_systems*/ 0,
      /* max_bridging_ports */ MAX_BPORTS_SYS,
      /* max_iw_mc_groups   */ MAX_MC_GROUPS,
      /* max_nat_systems   */0,
      /* max_iw_second_aggregations */0,
      /*iw_comp_limits  */NULL,
      /* max_iw_mc_members */MAX_MC_MEMBERS,
      /* max_iw_programmable_filters*/0,
      /* max_iw_policers*/0,
      /* max_dscpmt*/0,
      /*max_wred_bindings*/0,
      /*tcm_policer_limits*/{0},
      /*max_flow_rule_stat_tables*/0,
      /*max_indirect_action_tables*/0,
      /*lpm_dfc_limits*/{0},
      /*brigding_time_stamp_info*/{0},
      /*background_fifo_size*/200
   }
};

/* The WPE_System data structure holds parameters
 * of the required system for this test. It holds handles
 * of ports, devices, qnodes, channels, etc.
 */

typedef struct
{
      WP_U32    wpid;
      /*ports*/
      WP_handle phost_handle;
      WP_handle enet_port_handle[NUMBER_OF_ENET_PORTS];
      WP_handle upi_port_handle[NUMBER_OF_UPI_PORTS];

      /*Devices*/
      WP_handle enetdev[NUMBER_OF_ENET_PORTS];
      WP_handle upidev[NUMBER_OF_UPI_PORTS];
      WP_handle devhost;

      /*Pool, buffer and Qnodes handles*/
      WP_handle data_pool_handle;
      WP_U32    buffer_data_size;
      WP_handle qniw;
      WP_handle qnh_atm;
      WP_handle qnh;
      WP_handle qn_mc;


      /*Iw System*/
      WP_handle iw_system;

      /*Channels*/
      WP_handle host_tx;
      WP_handle host_rx;
      WP_handle rx_enet_handle[NUMBER_OF_ENET_PORTS];
      WP_handle tx_enet_handle[NUMBER_OF_ENET_PORTS][MAX_ENET_TX];
      WP_handle rx_upi_handle[NUMBER_OF_UPI_PORTS];
      WP_handle tx_upi_handle[NUMBER_OF_UPI_PORTS][MAX_UPI_TX];


      /*Flow Aggregations*/
      WP_handle flow_host;
      WP_handle flow_enet[NUMBER_OF_ENET_PORTS][MAX_ENET_TX];
      WP_handle flow_upi[NUMBER_OF_UPI_PORTS][MAX_UPI_TX];
      WP_handle flow_mc[MAX_MC_GROUPS];

      /*pqblock*/
      WP_handle pqblock;

      /*vpmt handle*/
      WP_handle vpmt_handle;

      /*Iw Ports*/
      WP_handle iw_port_host;
      WP_handle iw_port_enet[NUMBER_OF_ENET_PORTS];
      WP_handle iw_port_upi[NUMBER_OF_UPI_PORTS];

      /*MC*/
      WP_handle mc_group_handle[MAX_MC_GROUPS];
      WP_handle mc_member_handle[MAX_MC_GROUPS][MAX_MEMBERS_PER_GROUP];


      /*System info*/
      WP_U32 num_tx_on_enet;
      WP_U32 num_tx_on_upi;
      WP_U32 num_of_groups;
      WP_U32 last_tag;
      WP_U32 num_of_members[MAX_MC_GROUPS];
      WP_U32 vlan_mc_start;
      WP_U32 vlan_upi_start;

      /*System PCE*/
      WP_handle filter[MAX_NUM_OF_FILTERS];
      WP_handle rule[MAX_NUMBER_OF_RULES];
      WP_handle filter_set_handle;
      WP_handle interfaces[NUMBER_OF_ENET_PORTS+NUMBER_OF_UPI_PORTS];
      WP_U32 n_rules;

#if UPI_MIXED_SHAPING
      WP_handle l1_group[NUMBER_OF_UPI_PORTS];
      WP_handle l1_block[NUMBER_OF_UPI_PORTS];
      WP_handle l2_group[NUMBER_OF_UPI_PORTS][SHAPING_BLOCK_SIZE];
      WP_handle l2_block[NUMBER_OF_UPI_PORTS];
#endif
}WPE_system;

WP_pce_filter_stat pce_filter_stats[256];


/*Function Prototypes*/
static void WPE_SystemInitialize(WPE_system *sys);
static void WPE_PoolCreate(WPE_system *l2_struct);
static void WPE_TerminateOnError(WP_handle handle, WP_CHAR *s,WP_U32 line);
static void WPE_Terminate(WP_U32 status);
static void WPE_PortDeviceSetup(WPE_system *sys);
static void WPE_IWcreate(WPE_system *sys);
static void WPE_CreateBridgingPorts(WPE_system *sys);
static void WPE_SystemEnable(WPE_system *sys);
static void WPE_CreateMCgroups(WPE_system *sys);
static void WPE_AddMCmembers(WPE_system *sys);
WP_U16 WTE_IpChecksumCalc(WP_U8 *buff, unsigned short len);
static void APP_DataUnitsSetup(WPE_system *sys,WP_handle *send_handle,WP_U32 length,WP_CHAR* src,
                               WP_CHAR* dest,WP_CHAR* sa_ip,WP_CHAR* da_ip,
                               WP_CHAR* tos,WP_CHAR* vlan_tag);
static void WPE_CreateRxChannels(WPE_system *sys);
static void WPE_CreateHostTxAndIWport(WPE_system *sys);
static void WPE_RxBindings(WPE_system *sys);
static void WPE_DefaultSetup(WPE_system *sys);
static void WPE_CreateTxChannels(WPE_system *sys);
static void WPE_FlowAggsCreate(WPE_system *system);
static void WPE_DeleteBetweenScenarios(WPE_system *sys);
static void WPE_BuildScenario(WPE_system *sys,WP_U32 scenario);
static void WPE_DeleteAllMC(WPE_system *sys);
static WP_U32 WPE_RunScenario(WPE_system *sys,WP_U32 scenario);

static WP_U32 WPE_RunS_1(WPE_system *sys);
static WP_U32 WPE_RunS_2(WPE_system *sys);
static WP_U32 WPE_RunS_3(WPE_system *sys);
static WP_U32 WPE_RunS_4(WPE_system *sys);
static WP_U32 WPE_RunS_5(WPE_system *sys);
static WP_U32 WPE_RunS_6(WPE_system *sys);
static void WPE_S3_MemAdd(WPE_system *sys);
static void WPE_PceFiltersCreate(WPE_system *sys);
static void WPE_CreateFilterSets(WPE_system *sys);
static void WPE_PceInterfaces(WPE_system *sys);
static void WPE_BuildDefaultPce(WPE_system *sys);
static void WPE_DeletePceRules(WPE_system *sys);
static void WPE_SendToAllVlans(WPE_system *sys,WP_U32 n);
static void WPE_SendToAllMc(WPE_system *sys,WP_U32 n);
static WP_U32 WPE_VerifyScenario(WPE_system *sys,WP_U32 s,WP_U32 phase,WP_U32 n);
static WP_U32 WPE_CheckStatistics(WPE_system *sys,WP_U32 expected1,WP_U32 expected2,WP_S32 diff1,WP_S32 diff2);
static void WPE_ResetAllStatistics(WPE_system *sys);
static WP_U32 WPE_HostRecive(WPE_system *sys,WP_U8 *sent_buffer);
#if UPI_MIXED_SHAPING
static void WPE_CreateShapingGroups(WPE_system *sys);
#endif
static WP_U32 WPE_CheckQnodes(WPE_system *sys,WP_U32 mc_qnode_expected,WP_U32 iw_qnode_expected);

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

/***************************************************************
 * Func name  : CLI_DebugSWLogDisable
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_DebugEventLogRead(WP_CHAR *StrPrm)//phil
{
   WP_U32 num_of_entries_actually_read = 0;
   WPU_DebugSwLogRead(WP_SYSHANDLE(0),
                      100,
                      &num_of_entries_actually_read,
                      WPE_PrintLogEntry);
   printf("num_of_entries_actually_read: %d\n",num_of_entries_actually_read);
};

#if TEST_DEBUG
static void WPE_PrintPceFilterStats(WP_handle filter)
{
   WP_U32 index;
   WP_U32 temp1, temp2;

   index = filter & 0xFF;

   WP_PceFilterStatistics (filter, &(pce_filter_stats[index]));

   printf("PCE Filter %d Statistics\n", index);

   temp1 = pce_filter_stats[index].denied_pkts;
   temp2 = pce_filter_stats[index].denied_pkts >> 32;
   printf("denied_pkts            %08X %08X\n", temp1, temp2);

   temp1 = pce_filter_stats[index].matched_pkts;
   temp2 = pce_filter_stats[index].matched_pkts >> 32;
   printf("matched_pkts           %08X %08X\n", temp1, temp2);

   temp1 = pce_filter_stats[index].no_matched_pkts;
   temp2 = pce_filter_stats[index].no_matched_pkts >> 32;
   printf("no_matched_pkts        %08X %08X\n", temp1, temp2);

   printf("\n\n\n");
}


static void WPE_CheckPceFilterStats(WP_handle filter, WP_U16 diff_denied, WP_U16 diff_matched, WP_U16 diff_no_matched)
{
   WP_pce_global_stat pce_global_stats;

   WP_CHAR log_text[200];
   WP_U32 temp1, temp2, diff;
   WP_pce_filter_stat filter_stat_old;
   WP_U8 index = (WP_U8)(filter & 0xFF);

   WP_PceGlobalStatistics(WP_WINPATH(0), &pce_global_stats);

/*   WP_PceGlobalStatisticsReset (WP_WINPATH(0), &pce_global_stats);
     WP_PceFilterStatisticsReset (filter, &(pce_filter_stats[0]));*/

   memcpy(&filter_stat_old, &(pce_filter_stats[index]), sizeof(WP_pce_filter_stat));

   WP_PceFilterStatistics (filter, &(pce_filter_stats[index]));

   printf("\n\nComparing PCE Filter %d Statistic Changes\n", index);

   temp1 = pce_filter_stats[index].denied_pkts;
   temp2 = pce_filter_stats[index].denied_pkts >> 32;
   diff = pce_filter_stats[index].denied_pkts - filter_stat_old.denied_pkts;
   printf("denied_pkts            %08X %08X       diff %04X\n", temp2, temp1, diff);
   if(diff != diff_denied)
   {
      sprintf(log_text, "Fliter Statistics Error: denied packets number should be %04X\n", diff_denied);
      //    WPI_TEST_ADD_ERROR(log_text);
   }

   temp1 = pce_filter_stats[index].matched_pkts;
   temp2 = pce_filter_stats[index].matched_pkts >> 32;
   diff = pce_filter_stats[index].matched_pkts - filter_stat_old.matched_pkts;
   printf("matched_pkts           %08X %08X       diff %04X\n", temp2, temp1, diff);
   if(diff != diff_matched)
   {
      sprintf(log_text, "Fliter Statistics Error: matched packets number should be %04X\n", diff_matched);
   }

   temp1 = pce_filter_stats[index].no_matched_pkts;
   temp2 = pce_filter_stats[index].no_matched_pkts >> 32;
   diff = pce_filter_stats[index].no_matched_pkts - filter_stat_old.no_matched_pkts;
   printf("no_matched_pkts        %08X %08X       diff %04X\n", temp2, temp1, diff);
   if(diff != diff_no_matched)
   {
      sprintf(log_text, "Fliter Statistics Error: not matched packets number should be %04X\n", diff_no_matched);
   }

   printf("\n\n");
}

static void WPE_PrintPceGlobalStats(void)
{
   WP_U32 temp1, temp2;
   WP_pce_global_stat pce_global_stats;

   WP_PceGlobalStatistics(WP_WINPATH(0), &pce_global_stats);

   printf("\n\nPCE Global Statistics\n");

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

   printf("\n\n");
}

#endif

WP_U32 WPT_DisplayBFlowStats(WP_handle agg, WP_CHAR *type)
{
   WP_iw_flow_stats iw_flow_stats;
   WP_U32 forward_packets = 0;

   memset(&iw_flow_stats,0,sizeof(iw_flow_stats));
   WP_IwFlowStatistics(agg,
                       WP_IW_FLOW_STAT,
                       &iw_flow_stats);
   printf ("\n%s flow statistics of flow %d (%x)\n",
           type,(agg & 0x000000ff),agg);
   printf ("==========================================================\n");
   if (iw_flow_stats.forward_packet)
   {

      printf ("Forward Packet:          ");
      PRINT_STAT_FIELD(iw_flow_stats.forward_packet);
   }
   if (iw_flow_stats.fbp_drop_packets)
   {

      printf( "\nFBP Drop Packets:        ");
      PRINT_STAT_FIELD(iw_flow_stats.fbp_drop_packets);
   }
   if (iw_flow_stats.mtu_drop_packets)
   {
      printf( "\nMTU Drop Packets:        ");
      PRINT_STAT_FIELD(iw_flow_stats.mtu_drop_packets);
   }
   if (iw_flow_stats.ttl_drop_packets)
   {
      printf( "\nTTL Drop Packets:        ");
      PRINT_STAT_FIELD(iw_flow_stats.ttl_drop_packets);
   }
   if (iw_flow_stats.tx_queue_drop_packets)
   {
      printf( "\nTX Queue Drop Packets:   ");
      PRINT_STAT_FIELD(iw_flow_stats.tx_queue_drop_packets);
   }
   if (iw_flow_stats.mpls_drop)
   {

      printf( "\nMPLS Drop:               ");
      PRINT_STAT_FIELD(iw_flow_stats.mpls_drop);
   }
   if (iw_flow_stats.denied_packets)
   {
      printf( "\nDenied Packets:          ");
      PRINT_STAT_FIELD(iw_flow_stats.denied_packets);
   }
   if (iw_flow_stats.group_filtered_packets)
   {
      printf( "\ngroup_filtered_packets:  ");
      PRINT_STAT_FIELD(iw_flow_stats.group_filtered_packets);
   }
   if (iw_flow_stats.forwarded_bytes)
   {
      printf( "\nforwarded_bytes:         ");
      PRINT_STAT_FIELD(iw_flow_stats.forwarded_bytes);
   }
   if (iw_flow_stats.gtp_bad_headers)
   {
      printf( "\ngtp_bad_headers:         ");
      PRINT_STAT_FIELD(iw_flow_stats.gtp_bad_headers);
   }
   if (iw_flow_stats.policer_non_conforming_packets)
   {
      printf( "\npolicer_non_conforming_packets: ");
      PRINT_STAT_FIELD(iw_flow_stats.policer_non_conforming_packets);
   }
   if (iw_flow_stats.cfm_mac_in_mac_drop)
   {
      printf( "\ncfm_mac_in_mac_drop:     ");
      PRINT_STAT_FIELD(iw_flow_stats.cfm_mac_in_mac_drop);
   }

   printf("\n\n");
   forward_packets = (WP_U32)iw_flow_stats.forward_packet;
   return forward_packets;
}/*WPT_DisplayBFlowStats*/


WP_U32 WPT_DisplayBPortStats( WP_handle port_handle, WP_CHAR* type)
{
   WP_U32 rx_packets = 0;
   WP_S32 ii;
   WP_bport_stats bport_stats;
   memset( &bport_stats, 0, sizeof(WP_bport_stats) );
   WP_IwBridgingStatistics( port_handle, &bport_stats );
   printf ("\n%s Bridge port statistics of port %d (0x%x)\n",
           type,(port_handle & 0x000000ff),port_handle);
   printf ("============================================\n");
   if (bport_stats.rx_valid_packets)
   {
      printf ("Rx Valid Packets:               ");
      PRINT_STAT_FIELD(bport_stats.rx_valid_packets);
   }
   if (bport_stats.discard_vlan_acceptable_filter)
   {
      printf ("\nDiscard VLAN acceptable filter: ");
      PRINT_STAT_FIELD(bport_stats.discard_vlan_acceptable_filter);
   }
   if (bport_stats.discard_ingress_filter)
   {
      printf ("\nDiscard ingress filter:         ");
      PRINT_STAT_FIELD(bport_stats.discard_ingress_filter);
   }
   if (bport_stats.discard_bridge_classifier)
   {
      printf ("\nDiscard bridge classifier:      ");
      PRINT_STAT_FIELD(bport_stats.discard_bridge_classifier);
   }
   if (bport_stats.discard_unk_macsa)
   {
      printf ("\nDiscard unknown MAC SA:         ");
      PRINT_STAT_FIELD(bport_stats.discard_unk_macsa);
   }
   if (bport_stats.deny_mac_sa)
   {
      printf ("\nDeny MAC SA:                    ");
      PRINT_STAT_FIELD(bport_stats.deny_mac_sa);
   }
   if (bport_stats.deny_mac_da)
   {
      printf ("\nDeny MAC DA:                    ");
      PRINT_STAT_FIELD(bport_stats.deny_mac_da);
   }
   if (bport_stats.rx_bc_valid_packets)
   {
      printf ("\nRx BC valid packets:            ");
      PRINT_STAT_FIELD(bport_stats.rx_bc_valid_packets);
   }
   if (bport_stats.rx_mc_valid_packets)
   {
      printf ("\nRx MC valid packets:            ");
      PRINT_STAT_FIELD(bport_stats.rx_mc_valid_packets);
   }
   if (bport_stats.forwarded_uc_packets)
   {
      printf ("\nForwarded UC packets:           ");
      PRINT_STAT_FIELD(bport_stats.forwarded_uc_packets);
   }
   if (bport_stats.forwarded_bc_packets)
   {
      printf ("\nForwarded BC packets:           ");
      PRINT_STAT_FIELD(bport_stats.forwarded_bc_packets);
   }
   if (bport_stats.forwarded_mc_packets)
   {
      printf ("\nForwarded MC packets:           ");
      PRINT_STAT_FIELD(bport_stats.forwarded_mc_packets);
   }
   if (bport_stats.protocol_error)
   {
      printf ("\nprotocol_error:                 ");
      PRINT_STAT_FIELD(bport_stats.protocol_error);
   }
   if (bport_stats.res_mc_mac_deny)
   {
      printf ("\nres_mc_mac_deny:                ");
      PRINT_STAT_FIELD(bport_stats.res_mc_mac_deny);
   }
   for (ii=0 ; ii<32 ; ii++)
   {
      if (bport_stats.discard_dfc_filter[ii])
      {
         printf ("\ndiscard_dfc_filter[%d]:         ", ii);
         PRINT_STAT_FIELD(bport_stats.discard_dfc_filter[ii]);
      }
      if (bport_stats.deny_cfm_frames)
      {
         printf ("\ndeny_cfm_frames:             ");
         PRINT_STAT_FIELD(bport_stats.deny_cfm_frames);
      }
      if (bport_stats.bad_ip_version)
      {
         printf ("\nbad_ip_version:             ");
         PRINT_STAT_FIELD(bport_stats.bad_ip_version);
      }
      if (bport_stats.checksum_error)
      {
         printf ("\nchecksum_error:             ");
         PRINT_STAT_FIELD(bport_stats.checksum_error);
      }
      if (bport_stats.ip_protocol_error)
      {
         printf ("\nip_protocol_error:             ");
         PRINT_STAT_FIELD(bport_stats.ip_protocol_error);
      }
   }

   printf("\n\n");

   rx_packets = (WP_U32)bport_stats.rx_valid_packets;
   return rx_packets;
}/*WPT_DisplayBPortStats*/

void WTE_DisplayEnetDeviceStats(WP_handle device_handle, WP_CHAR* type)
{

   WP_U32 temp1, temp2;
   WP_stats_enet stats_hs_enet = {0};
   WP_status status;
   printf ("\n%s Device statistics of device %d (%x)\n",
           type,(device_handle & 0x000000ff),device_handle);
   printf ("============================================\n");

   memset( &stats_hs_enet, 0, sizeof(WP_stats_enet) );
   if((status = WP_DeviceStatistics(device_handle, &stats_hs_enet)))
   {
      printf("GetStatistics error\n");
      return;
   }
   printf("\nEnet device statistics:\n");
   printf("Hardware statistics:\n");
   temp1 = stats_hs_enet.rx_packets;
   temp2 = stats_hs_enet.rx_packets >> 32;
   printf("rx_packets:           %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.tx_packets;
   temp2 = stats_hs_enet.tx_packets >> 32;
   printf("tx_packets:           %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.txrx_frames_64;
   temp2 = stats_hs_enet.txrx_frames_64 >> 32;
   printf("txrx_frames_64:           %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.txrx_frames_127;
   temp2 = stats_hs_enet.txrx_frames_127 >> 32;
   printf("txrx_frames_127:           %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.txrx_frames_255;
   temp2 = stats_hs_enet.txrx_frames_255 >> 32;
   printf("txrx_frames_255:           %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.txrx_frames_511;
   temp2 = stats_hs_enet.txrx_frames_511 >> 32;
   printf("txrx_frames_511:           %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.txrx_frames_1023;
   temp2 = stats_hs_enet.txrx_frames_1023 >> 32;
   printf("txrx_frames_1023:           %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.txrx_frames_1518;
   temp2 = stats_hs_enet.txrx_frames_1518 >> 32;
   printf("txrx_frames_1518:           %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.txrx_frames_1522;
   temp2 = stats_hs_enet.txrx_frames_1522 >> 32;
   printf("txrx_frames_1522:           %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.rx_bytes;
   temp2 = stats_hs_enet.rx_bytes >> 32;
   printf("rx_bytes:           %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.rx_err_fcs;
   temp2 = stats_hs_enet.rx_err_fcs >> 32;
   printf("rx_err_fcs:           %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.rx_multicast;
   temp2 = stats_hs_enet.rx_multicast >> 32;
   printf("rx_multicast:           %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.rx_broadcast;
   temp2 = stats_hs_enet.rx_broadcast >> 32;
   printf("rx_broadcast:           %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.rx_mac_control;
   temp2 = stats_hs_enet.rx_mac_control >> 32;
   printf("rx_mac_control:           %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.rx_mac_pause;
   temp2 = stats_hs_enet.rx_mac_pause >> 32;
   printf("rx_mac_pause:           %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.rx_mac_unknown;
   temp2 = stats_hs_enet.rx_mac_unknown >> 32;
   printf("rx_mac_unknown:           %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.rx_err_alignment;
   temp2 = stats_hs_enet.rx_err_alignment >> 32;
   printf("rx_err_alignment:           %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.rx_err_length;
   temp2 = stats_hs_enet.rx_err_length >> 32;
   printf("rx_err_length:           %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.rx_err_code;
   temp2 = stats_hs_enet.rx_err_code >> 32;
   printf("rx_err_code:           %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.rx_false_carrier;
   temp2 = stats_hs_enet.rx_false_carrier >> 32;
   printf("rx_false_carrier:           %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.rx_undersize;
   temp2 = stats_hs_enet.rx_undersize >> 32;
   printf("rx_undersize:           %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.rx_oversize;
   temp2 = stats_hs_enet.rx_oversize >> 32;
   printf("rx_oversize:           %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.rx_fragments;
   temp2 = stats_hs_enet.rx_fragments >> 32;
   printf("rx_fragments:           %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.rx_jabber;
   temp2 = stats_hs_enet.rx_jabber >> 32;
   printf("rx_jabber:           %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.rx_dropped;
   temp2 = stats_hs_enet.rx_dropped >> 32;
   printf("rx_dropped:           %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.tx_bytes;
   temp2 = stats_hs_enet.tx_bytes >> 32;
   printf("tx_bytes:           %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.tx_multicast;
   temp2 = stats_hs_enet.tx_multicast >> 32;
   printf("tx_multicast:           %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.tx_broadcast;
   temp2 = stats_hs_enet.tx_broadcast >> 32;
   printf("tx_broadcast:           %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.tx_mac_pause;
   temp2 = stats_hs_enet.tx_mac_pause >> 32;
   printf("tx_mac_pause:           %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.tx_defer;
   temp2 = stats_hs_enet.tx_defer >> 32;
   printf("tx_defer:           %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.tx_excess_defer;
   temp2 = stats_hs_enet.tx_excess_defer >> 32;
   printf("tx_excess_defer:           %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.tx_single_collision;
   temp2 = stats_hs_enet.tx_single_collision >> 32;
   printf("tx_single_collision:           %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.tx_multi_collision;
   temp2 = stats_hs_enet.tx_multi_collision >> 32;
   printf("tx_multi_collision:           %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.tx_late_collision;
   temp2 = stats_hs_enet.tx_late_collision >> 32;
   printf("tx_late_collision:           %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.tx_excess_collision;
   temp2 = stats_hs_enet.tx_excess_collision >> 32;
   printf("tx_excess_collision:           %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.tx_no_collision;
   temp2 = stats_hs_enet.tx_no_collision >> 32;
   printf("tx_no_collision:           %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.tx_mac_pause_honored;
   temp2 = stats_hs_enet.tx_mac_pause_honored >> 32;
   printf("tx_mac_pause_honored:           %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.tx_dropped;
   temp2 = stats_hs_enet.tx_dropped >> 32;
   printf("tx_dropped:           %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.tx_jabber;
   temp2 = stats_hs_enet.tx_jabber >> 32;
   printf("tx_jabber:           %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.tx_err_fcs;
   temp2 = stats_hs_enet.tx_err_fcs >> 32;
   printf("tx_err_fcs:           %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.tx_control;
   temp2 = stats_hs_enet.tx_control >> 32;
   printf("tx_control:           %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.tx_oversize;
   temp2 = stats_hs_enet.tx_oversize >> 32;
   printf("tx_oversize:           %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.tx_undersize;
   temp2 = stats_hs_enet.tx_undersize >> 32;
   printf("tx_undersize:           %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.tx_fragments;
   temp2 = stats_hs_enet.tx_fragments >> 32;
   printf("tx_fragments:           %.8x%.8x\n", temp2,temp1);

   printf("DPS statistics:\n");
   temp1 = stats_hs_enet.rx_err_overrun;
   temp2 = stats_hs_enet.rx_err_overrun >> 32;
   printf("rx_err_overrun:       %.8x%.8x\n", temp2,temp1);
   temp1 = stats_hs_enet.rx_err_sdu;
   temp2 = stats_hs_enet.rx_err_sdu >> 32;
   printf("rx_err_maxsdu:        %.8x%.8x\n", temp2,temp1);
   temp1 = stats_hs_enet.rx_err_nonvalid_mac;
   temp2 = stats_hs_enet.rx_err_nonvalid_mac >> 32;
   printf("rx_err_mac_address:   %.8x%.8x\n", temp2,temp1);
   temp1 = stats_hs_enet.rx_err_mru;
   temp2 = stats_hs_enet.rx_err_mru >> 32;
   printf("rx_err_mru:           %.8x%.8x\n", temp2,temp1);
   temp1 = stats_hs_enet.tx_err_underrun;
   temp2 = stats_hs_enet.tx_err_underrun >> 32;
   printf("tx_err_underrun:      %.8x%.8x\n", temp2,temp1);
   temp1 = stats_hs_enet.rx_err_host_full;
   temp2 = stats_hs_enet.rx_err_host_full >> 32;
   printf("rx_err_host_full:     %.8x%.8x\n", temp2,temp1);
   temp1 = stats_hs_enet.rx_err_fbp_underrun;
   temp2 = stats_hs_enet.rx_err_fbp_underrun >> 32;
   printf("rx_err_fbp_underrun:  %.8x%.8x\n", temp2,temp1);
   temp1 = stats_hs_enet.rx_iw_frames;
   temp2 = stats_hs_enet.rx_iw_frames >> 32;
   printf("rx_iw_frames:         %.8x%.8x\n", temp2,temp1);
   temp1 = stats_hs_enet.tx_frames;
   temp2 = stats_hs_enet.tx_frames >> 32;
   printf("tx_good_frames:       %.8x%.8x\n", temp2,temp1);
   printf("\n");
   temp1 = stats_hs_enet.rx_host_frames;
   temp2 = stats_hs_enet.rx_host_frames >> 32;
   printf("rx_host_frames:       %.8x%.8x\n", temp2,temp1);
   printf("\n");

   printf("Serial DPS IF statistics:\n");
   temp1 = stats_hs_enet.serial_dps_if_stats.tx_frames;
   temp2 = stats_hs_enet.serial_dps_if_stats.tx_frames >> 32;
   printf("Serial DPS IF tx frames             %.8x%.8x\n", temp2,temp1);
   temp1 = stats_hs_enet.serial_dps_if_stats.tx_bytes;
   temp2 = stats_hs_enet.serial_dps_if_stats.tx_bytes >> 32;
   printf("Serial DPS IF tx bytes              %.8x%.8x\n", temp2,temp1);
   temp1 = stats_hs_enet.serial_dps_if_stats.rx_frames;
   temp2 = stats_hs_enet.serial_dps_if_stats.rx_frames >> 32;
   printf("Serial DPS IF rx frames    %.8x%.8x\n", temp2,temp1);
   temp1 = stats_hs_enet.serial_dps_if_stats.rx_bytes;
   temp2 = stats_hs_enet.serial_dps_if_stats.rx_bytes >> 32;
   printf("Serial DPS IF rx bytes     %.8x%.8x\n", temp2,temp1);
   temp1 = stats_hs_enet.serial_dps_if_stats.rx_pce_denied_frames;
   temp2 = stats_hs_enet.serial_dps_if_stats.rx_pce_denied_frames >> 32;
   printf("Serial DPS IF rx pce denied frames  %.8x%.8x\n", temp2,temp1);
   temp1 = stats_hs_enet.serial_dps_if_stats.rx_dropped_frames;
   temp2 = stats_hs_enet.serial_dps_if_stats.rx_dropped_frames >> 32;
   printf("Serial DPS IF rx dropped frames %.8x%.8x\n", temp2,temp1);

}

WP_U32 WPT_DisplayDeviceStats(WP_handle device_handle, WP_device device, WP_CHAR* type)
{
   WP_U32 temp1, temp2, status;
   WP_stats_enet stats_hs_enet = {0};
   WP_stats_upi_atm stats_upi = {0};
   WP_stats_upi_hspos stats_hspos = {0};

   printf ("\n%s statistics of device %d (%x)\n",
           type,(device_handle & 0x000000ff),device_handle);
   printf ("============================================\n");
   switch (device)
   {
      case WP_DEVICE_ENET:
//          WTE_DisplayEnetDeviceStats(device_handle,type);

      {
         memset(&stats_hs_enet,0,sizeof(WP_stats_enet));
         if( (status = WP_DeviceStatistics(device_handle, &stats_hs_enet)) )
         {
            printf("GetStatistics error\n");
            return 0;
         }
         printf("\nStatistics for ENET device:\n");
         if (stats_hs_enet.rx_packets)
         {

            temp1 = stats_hs_enet.rx_packets;
            temp2 = stats_hs_enet.rx_packets >> 32;
            printf("rx_packets:           %.8x%.8x\n", temp2,temp1);
         }
         if (stats_hs_enet.rx_err_fcs)
         {
            temp1 = stats_hs_enet.rx_err_fcs;
            temp2 = stats_hs_enet.rx_err_fcs >> 32;
            printf("rx_err_fcs:           %.8x%.8x\n", temp2,temp1);
         }
         if (stats_hs_enet.rx_multicast)
         {
            temp1 = stats_hs_enet.rx_multicast;
            temp2 = stats_hs_enet.rx_multicast >> 32;
            printf("rx_multicast:         %.8x%.8x\n", temp2,temp1);
         }
         if (stats_hs_enet.rx_broadcast)
         {
            temp1 = stats_hs_enet.rx_broadcast;
            temp2 = stats_hs_enet.rx_broadcast >> 32;
            printf("rx_broadcast:         %.8x%.8x\n", temp2,temp1);
         }
         if (stats_hs_enet.rx_mac_control)
         {
            temp1 = stats_hs_enet.rx_mac_control;
            temp2 = stats_hs_enet.rx_mac_control >> 32;
            printf("rx_mac_control:         %.8x%.8x\n", temp2,temp1);

         }
         if (stats_hs_enet.rx_mac_pause)
         {
            temp1 = stats_hs_enet.rx_mac_pause;
            temp2 = stats_hs_enet.rx_mac_pause >> 32;
            printf("rx_mac_pause:           %.8x%.8x\n", temp2,temp1);
         }
         if (stats_hs_enet.rx_mac_unknown)
         {
            temp1 = stats_hs_enet.rx_mac_unknown;
            temp2 = stats_hs_enet.rx_mac_unknown >> 32;
            printf("rx_mac_unknown:         %.8x%.8x\n", temp2,temp1);
         }
         if (stats_hs_enet.rx_err_alignment)
         {
            temp1 = stats_hs_enet.rx_err_alignment;
            temp2 = stats_hs_enet.rx_err_alignment >> 32;
            printf("rx_err_alignment:     %.8x%.8x\n", temp2,temp1);
         }
         if (stats_hs_enet.rx_err_length)
         {

            temp1 = stats_hs_enet.rx_err_length;
            temp2 = stats_hs_enet.rx_err_length >> 32;
            printf("rx_err_length:        %.8x%.8x\n", temp2,temp1);
         }
         if (stats_hs_enet.rx_err_code)
         {

            temp1 = stats_hs_enet.rx_err_code;
            temp2 = stats_hs_enet.rx_err_code >> 32;
            printf("rx_err_code:          %.8x%.8x\n", temp2,temp1);
         }
         if (stats_hs_enet.rx_false_carrier)
         {

            temp1 = stats_hs_enet.rx_false_carrier;
            temp2 = stats_hs_enet.rx_false_carrier >> 32;
            printf("rx_false_carrier:     %.8x%.8x\n", temp2,temp1);
         }
         if (stats_hs_enet.rx_undersize)
         {

            temp1 = stats_hs_enet.rx_undersize;
            temp2 = stats_hs_enet.rx_undersize >> 32;
            printf("rx_undersize:         %.8x%.8x\n", temp2,temp1);
         }
         if (stats_hs_enet.rx_oversize)
         {

            temp1 = stats_hs_enet.rx_oversize;
            temp2 = stats_hs_enet.rx_oversize >> 32;
            printf("rx_oversize:          %.8x%.8x\n", temp2,temp1);
         }
         if (stats_hs_enet.rx_fragments)
         {

            temp1 = stats_hs_enet.rx_fragments;
            temp2 = stats_hs_enet.rx_fragments >> 32;
            printf("rx_fragments:         %.8x%.8x\n", temp2,temp1);
         }
         if (stats_hs_enet.rx_jabber)
         {

            temp1 = stats_hs_enet.rx_jabber;
            temp2 = stats_hs_enet.rx_jabber >> 32;
            printf("rx_jabber:            %.8x%.8x\n", temp2,temp1);
         }
         if (stats_hs_enet.rx_dropped)
         {
            temp1 = stats_hs_enet.rx_dropped;
            temp2 = stats_hs_enet.rx_dropped >> 32;
            printf("rx_err_dropped:       %.8x%.8x\n", temp2,temp1);

         }

         if (stats_hs_enet.tx_packets)
         {
            temp1 = stats_hs_enet.tx_packets;
            temp2 = stats_hs_enet.tx_packets >> 32;
            printf("tx_packets:           %.8x%.8x\n", temp2,temp1);

         }
         if (stats_hs_enet.tx_multicast)
         {
            temp1 = stats_hs_enet.tx_multicast;
            temp2 = stats_hs_enet.tx_multicast >> 32;
            printf("tx_multicast:         %.8x%.8x\n", temp2,temp1);

         }
         if (stats_hs_enet.tx_broadcast)
         {
            temp1 = stats_hs_enet.tx_broadcast;
            temp2 = stats_hs_enet.tx_broadcast >> 32;
            printf("tx_broadcast:         %.8x%.8x\n", temp2,temp1);
         }
         if (stats_hs_enet.tx_mac_pause)
         {
            temp1 = stats_hs_enet.tx_mac_pause;
            temp2 = stats_hs_enet.tx_mac_pause >> 32;
            printf("tx_mac_pause:         %.8x%.8x\n", temp2,temp1);
         }
         if (stats_hs_enet.tx_defer)
         {
            temp1 = stats_hs_enet.tx_defer;
            temp2 = stats_hs_enet.tx_defer >> 32;
            printf("tx_defer:             %.8x%.8x\n", temp2,temp1);
         }
         if (stats_hs_enet.tx_excess_defer)
         {
            temp1 = stats_hs_enet.tx_excess_defer;
            temp2 = stats_hs_enet.tx_excess_defer >> 32;
            printf("tx_excess_defer:      %.8x%.8x\n", temp2,temp1);
         }
         if (stats_hs_enet.tx_single_collision)
         {
            temp1 = stats_hs_enet.tx_single_collision;
            temp2 = stats_hs_enet.tx_single_collision >> 32;
            printf("tx_single_collision:  %.8x%.8x\n", temp2,temp1);
         }
         if (stats_hs_enet.tx_multi_collision)
         {
            temp1 = stats_hs_enet.tx_multi_collision;
            temp2 = stats_hs_enet.tx_multi_collision >> 32;
            printf("tx_multi_collision:   %.8x%.8x\n", temp2,temp1);
         }
         if (stats_hs_enet.tx_late_collision)
         {
            temp1 = stats_hs_enet.tx_late_collision;
            temp2 = stats_hs_enet.tx_late_collision >> 32;
            printf("tx_late_collision:    %.8x%.8x\n", temp2,temp1);
         }
         if (stats_hs_enet.tx_excess_collision)
         {
            temp1 = stats_hs_enet.tx_excess_collision;
            temp2 = stats_hs_enet.tx_excess_collision >> 32;
            printf("tx_excess_collision:  %.8x%.8x\n", temp2,temp1);
         }
         if (stats_hs_enet.tx_no_collision)
         {
            temp1 = stats_hs_enet.tx_no_collision;
            temp2 = stats_hs_enet.tx_no_collision >> 32;
            printf("tx_no_collision:      %.8x%.8x\n", temp2,temp1);
         }
         if (stats_hs_enet.tx_mac_pause_honored)
         {
            temp1 = stats_hs_enet.tx_mac_pause_honored;
            temp2 = stats_hs_enet.tx_mac_pause_honored >> 32;
            printf("tx_mac_pause_honored: %.8x%.8x\n", temp2,temp1);
         }
         if (stats_hs_enet.tx_dropped)
         {
            temp1 = stats_hs_enet.tx_dropped;
            temp2 = stats_hs_enet.tx_dropped >> 32;
            printf("tx_dropped:           %.8x%.8x\n", temp2,temp1);
         }
         if (stats_hs_enet.tx_jabber)
         {
            temp1 = stats_hs_enet.tx_jabber;
            temp2 = stats_hs_enet.tx_jabber >> 32;
            printf("tx_jabber:            %.8x%.8x\n", temp2,temp1);
         }
         if (stats_hs_enet.tx_err_fcs)
         {
            temp1 = stats_hs_enet.tx_err_fcs;
            temp2 = stats_hs_enet.tx_err_fcs >> 32;
            printf("tx_err_fcs:           %.8x%.8x\n", temp2,temp1);
         }
         if (stats_hs_enet.tx_control)
         {
            temp1 = stats_hs_enet.tx_control;
            temp2 = stats_hs_enet.tx_control >> 32;
            printf("tx_control:           %.8x%.8x\n", temp2,temp1);
         }
         if (stats_hs_enet.tx_oversize)
         {
            temp1 = stats_hs_enet.tx_oversize;
            temp2 = stats_hs_enet.tx_oversize >> 32;
            printf("tx_oversize:          %.8x%.8x\n", temp2,temp1);
         }
         if (stats_hs_enet.tx_undersize)
         {
            temp1 = stats_hs_enet.tx_undersize;
            temp2 = stats_hs_enet.tx_undersize >> 32;
            printf("tx_undersize:         %.8x%.8x\n", temp2,temp1);
         }
         if (stats_hs_enet.tx_fragments)
         {
            temp1 = stats_hs_enet.tx_fragments;
            temp2 = stats_hs_enet.tx_fragments >> 32;
            printf("tx_fragments:         %.8x%.8x\n", temp2,temp1);
         }
         if (stats_hs_enet.rx_host_frames)
         {
            temp1 = stats_hs_enet.rx_host_frames;
            temp2 = stats_hs_enet.rx_host_frames >> 32;
            printf("rx_host_frames:      %.8x %.8x\n", temp2,temp1);
         }
         if (stats_hs_enet.rx_iw_frames)
         {
            temp1 = stats_hs_enet.rx_iw_frames;
            temp2 = stats_hs_enet.rx_iw_frames >> 32;
            printf("rx_iw_frames:         %.8x%.8x\n", temp2,temp1);
         }
         if (stats_hs_enet.rx_err_host_full)
         {
            temp1 = stats_hs_enet.rx_err_host_full;
            temp2 = stats_hs_enet.rx_err_host_full >> 32;
            printf("rx_err_host_full:     %.8x%.8x\n", temp2,temp1);

         }
         if (stats_hs_enet.rx_err_fbp_underrun)
         {
            temp1 = stats_hs_enet.rx_err_fbp_underrun;
            temp2 = stats_hs_enet.rx_err_fbp_underrun >> 32;
            printf("rx_err_fbp_underrun:  %.8x%.8x\n", temp2,temp1);

         }
         if (stats_hs_enet.rx_err_nonvalid_mac)
         {
            temp1 = stats_hs_enet.rx_err_nonvalid_mac;
            temp2 = stats_hs_enet.rx_err_nonvalid_mac >> 32;
            printf("rx_err_mac_address:   %.8x%.8x\n", temp2,temp1);

         }
         if (stats_hs_enet.rx_err_mru)
         {
            temp1 = stats_hs_enet.rx_err_mru;
            temp2 = stats_hs_enet.rx_err_mru >> 32;
            printf("rx_err_mru:           %.8x%.8x\n", temp2,temp1);

         }
         if (stats_hs_enet.rx_err_sdu)
         {
            temp1 = stats_hs_enet.rx_err_sdu;
            temp2 = stats_hs_enet.rx_err_sdu >> 32;
            printf("rx_err_maxsdu:        %.8x%.8x\n", temp2,temp1);

         }
         if (stats_hs_enet.tx_err_underrun)
         {
            temp1 = stats_hs_enet.tx_err_underrun;
            temp2 = stats_hs_enet.tx_err_underrun >> 32;
            printf("rx_err_underrun:      %.8x%.8x\n", temp2,temp1);

         }
         if (stats_hs_enet.rx_err_overrun)
         {
            temp1 = stats_hs_enet.rx_err_overrun;
            temp2 = stats_hs_enet.rx_err_overrun >> 32;
            printf("rx_err_overrun:       %.8x%.8x\n", temp2,temp1);

         }
         if (stats_hs_enet.tx_frames)
         {
            temp1 = stats_hs_enet.tx_frames;
            temp2 = stats_hs_enet.tx_frames >> 32;
            printf("tx_frames:            %.8x%.8x\n", temp2,temp1);

         }



         printf("\n");

         return stats_hs_enet.rx_packets;
      }
      case WP_DEVICE_ATM:
      {
         memset(&stats_upi,0,sizeof(WP_stats_upi_atm));
         if( (status = WP_DeviceStatistics(device_handle, &stats_upi)) )
         {
            printf("GetStatistics error\n");
            return 0;
         }
         printf("\nStatistics for UPI device:\n");
         if (stats_upi.rx_err_slc)
         {
            printf("rx_err_slc:           ");
            PRINT_STAT_FIELD(stats_upi.rx_err_slc);
         }
         if (stats_upi.rx_err_parity);
         {
            printf("\nrx_err_parity:        ");
            PRINT_STAT_FIELD(stats_upi.rx_err_parity);
         }
         if (stats_upi.rx_err_addr_mismatch)
         {
            printf("\nrx_err_addr_mismatch: ");
            PRINT_STAT_FIELD(stats_upi.rx_err_addr_mismatch);
         }
         if (stats_upi.ext_stats.rx_cells)
         {
            printf("\nrx_cells:             ");
            PRINT_STAT_FIELD(stats_upi.ext_stats.rx_cells);
         }
         if (stats_upi.ext_stats.rx_last_unknown_addr)
         {
            printf("\nrx_last_unknown_addr: ");
            PRINT_STAT_FIELD(stats_upi.ext_stats.rx_last_unknown_addr);
         }
         if (stats_upi.ext_stats.rx_clp1_cells)
         {
            printf("\nrx_clp1_cells:        ");
            PRINT_STAT_FIELD(stats_upi.ext_stats.rx_clp1_cells);
         }
         if (stats_upi.ext_stats.rx_gfc_cells)
         {
            printf("\nrx_gfc_cells:         ");
            PRINT_STAT_FIELD(stats_upi.ext_stats.rx_gfc_cells);
         }
         if (stats_upi.ext_stats.tx_cells)
         {
            printf("\ntx_cells:             ");
            PRINT_STAT_FIELD(stats_upi.ext_stats.tx_cells);
         }
         if (stats_upi.ext_stats.tx_clp1_cells)
         {
            printf("\ntx_clp1_cells:        ");
            PRINT_STAT_FIELD(stats_upi.ext_stats.tx_clp1_cells);
         }
         if (stats_upi.ext_stats.tx_efci_cells)
         {
            printf("\ntx_efci_cells:        ");
            PRINT_STAT_FIELD(stats_upi.ext_stats.tx_efci_cells);
         }
         if (stats_upi.ext_stats.rx_oam_cells)
         {
            printf("\nrx_oam_cells:         ");
            PRINT_STAT_FIELD(stats_upi.ext_stats.rx_oam_cells);
         }
         if (stats_upi.ext_stats.rx_rm_cells)
         {
            printf("\nrx_rm_cells:          ");
            PRINT_STAT_FIELD(stats_upi.ext_stats.rx_rm_cells);
         }
         if (stats_upi.ext_stats.tx_oam_cells)
         {
            printf("\ntx_oam_cells:         ");
            PRINT_STAT_FIELD(stats_upi.ext_stats.tx_oam_cells);
         }
         if (stats_upi.ext_stats.tx_rm_cells)
         {
            printf("\ntx_rm_cells:          ");
            PRINT_STAT_FIELD(stats_upi.ext_stats.tx_rm_cells);
         }
         if (stats_upi.ext_stats.rx_efci_cells)
         {
            printf("\nrx_efci_cells:        ");
            PRINT_STAT_FIELD(stats_upi.ext_stats.rx_efci_cells);
         }
         if (stats_upi.ext_stats.tx_idle_cells)
         {
            printf("\ntx_idle_cells:        ");
            PRINT_STAT_FIELD(stats_upi.ext_stats.tx_idle_cells);
         }
         if (stats_upi.ext_stats.rx_clp0_cells)
         {
            printf("\nrx_clp0_cells:        ");
            PRINT_STAT_FIELD(stats_upi.ext_stats.rx_clp0_cells);
         }
         if (stats_upi.ext_stats.tx_clp0_cells)
         {
            printf("\ntx_clp0_cells:        ");
            PRINT_STAT_FIELD(stats_upi.ext_stats.tx_clp0_cells);
         }
         if (stats_upi.ext_stats.rx_crc_err_cells)
         {
            printf("\nrx_crc_err_cells:     ");
            PRINT_STAT_FIELD(stats_upi.ext_stats.rx_crc_err_cells);
         }
         if (stats_upi.ext_stats.rx_unsupported_cells)
         {
            printf("\nrx_unsupported_cells: ");
            PRINT_STAT_FIELD(stats_upi.ext_stats.rx_unsupported_cells);
         }

         printf("\n\n");
         return (WP_U32)stats_upi.ext_stats.rx_cells;
      }
      case WP_DEVICE_HSPOS:
      {
         memset(&stats_hspos,0,sizeof(WP_stats_upi_hspos));
         if( (status = WP_DeviceStatistics(device_handle, &stats_hspos)) )
         {
            printf("GetStatistics error\n");
            return 0;
         }
         printf("\nStatistics for UPI device:\n");
         if (stats_hspos.rx_host_frames)
         {
            temp1 = stats_hspos.rx_host_frames;
            temp2 = stats_hspos.rx_host_frames >> 32;
            printf("rx_host_frames:      %.8x %.8x\n", temp2,temp1);
         }
         if (stats_hspos.rx_iw_frames)
         {
            temp1 = stats_hspos.rx_iw_frames;
            temp2 = stats_hspos.rx_iw_frames >> 32;
            printf("rx_iw_frames:         %.8x%.8x\n", temp2,temp1);
         }
         if (stats_hspos.rx_err_host_full)
         {
            temp1 = stats_hspos.rx_err_host_full;
            temp2 = stats_hspos.rx_err_host_full >> 32;
            printf("rx_err_host_full:     %.8x%.8x\n", temp2,temp1);
         }
         if (stats_hspos.rx_err_fbp_underrun)
         {
            temp1 = stats_hspos.rx_err_fbp_underrun;
            temp2 = stats_hspos.rx_err_fbp_underrun >> 32;
            printf("rx_err_fbp_underrun:  %.8x%.8x\n", temp2,temp1);
         }
         if (stats_hspos.rx_err_mru)
         {
            temp1 = stats_hspos.rx_err_mru;
            temp2 = stats_hspos.rx_err_mru >> 32;
            printf("rx_err_mru:           %.8x%.8x\n", temp2,temp1);
         }
         if (stats_hspos.rx_err_sdu)
         {
            temp1 = stats_hspos.rx_err_sdu;
            temp2 = stats_hspos.rx_err_sdu >> 32;
            printf("rx_err_maxsdu:        %.8x%.8x\n", temp2,temp1);
         }
         if (stats_hspos.rx_err_sm)
         {
            temp1 = stats_hspos.rx_err_sm;
            temp2 = stats_hspos.rx_err_sm >> 32;
            printf("rx_err_sm:            %.8x%.8x\n", temp2,temp1);
         }
         if (stats_hspos.rx_err_parity)
         {
            temp1 = stats_hspos.rx_err_parity;
            temp2 = stats_hspos.rx_err_parity >> 32;
            printf("rx_err_parity:        %.8x%.8x\n", temp2,temp1);
         }
         if (stats_hspos.rx_err_phy)
         {
            temp1 = stats_hspos.rx_err_phy;
            temp2 = stats_hspos.rx_err_phy >> 32;
            printf("rx_err_phy:           %.8x%.8x\n", temp2,temp1);
         }
         if (stats_hspos.tx_frames)
         {
            temp1 = stats_hspos.tx_frames;
            temp2 = stats_hspos.tx_frames >> 32;
            printf("tx_frames:            %.8x%.8x\n", temp2,temp1);
         }
         return (WP_U32)stats_hspos.rx_iw_frames;;
      }
   }
   return 0;

}/*WPT_DisplayDeviceStats*/

WP_status status;
WP_U32 scenario;  /*Test Scenario Num.*/
WP_U32 total=0;

WP_status WPT_DebugCallback(const WP_CHAR *file, const WP_CHAR *function,
                            WP_S32 line, WP_S32 error_id)
{
   if (error_id)
      printf("\n\nfile = %s , line = %d\nfunction = %s\n",file,line,function);
   return WP_OK;
}

void WPE_DebugInit(void)
{
   WP_module_debug module_debug =
      {
         /* modes */ WP_DEBUG_SW_LOG_CYCLIC | WP_DEBUG_BD_CORRUPTION_ENABLE,
         /* sw_log_bus */ { WP_BUS_PARAM, APP_BANK_PARAM },
         /* sw_log_size */ WP_DEBUG_SW_LOGGING_64K_ENTRIES,
         /* interrupt_queue */ WP_IRQT0,
      };

   status = WP_ModuleInit(WP_SYSHANDLE(0), WP_WDDI_MODULE_DEBUG, &module_debug);
   WPE_TerminateOnError(status, "WPI_DebugModuleInit()", __LINE__);

}

/***********************
 *    MAIN
 **********************/
WP_S32 main(WP_S32 argc, WP_CHAR **argv)
{
   static WPE_system system;
   WP_U32 err=0;
#ifdef USE_SOCKET
   InitSocketInterface();
#endif

#if _WT_MODULE_TEST_ENABLE_
   WT_Start(__FILE__, NULL, __FILE__);
#endif

   system.wpid=WP_WINPATH(0);
   /*PRE-COMMIT*/
   WPE_SystemInitialize(&system);

#ifndef USE_SOCKET
   status = WPX_BoardConfigure(0, WPX_CONFIGURE_DEFAULT_CHECKIN);
   WPE_TerminateOnError(status, "WPX_BoardConfigure()",__LINE__);
#endif

   WPE_PoolCreate(&system);
   WPE_PortDeviceSetup(&system);

#ifndef USE_SOCKET
   {
      WP_U32 port_index;

      for(port_index = 0; port_index<NUMBER_OF_ENET_PORTS; port_index++)
      {
         status = WPX_BoardSerdesInit(0, WP_PORT_ENET13+port_index, WPX_SERDES_LOOPBACK);
         WPE_TerminateOnError(status, "WPX_BoardSerdesSetLoopback()",__LINE__);
      }
   }
#endif

   status=WP_SysCommit();
   WPE_TerminateOnError(status,"WP_SysCommit",__LINE__);

   status = WPU_DebugEventGroupLoad(WP_SYSHANDLE(0), WPU_EVENT_ALL_WF, "L2PI_RX_FLOW_DATA_UNIT");
   WPE_TerminateOnError(status,"WPU_DebugEventGroupLoad",__LINE__);

   status = WPU_DebugEventGroupLoad(WP_SYSHANDLE(0), WPU_EVENT_ALL_WF, "BD_CORRUPTION_L2PI");
   WPE_TerminateOnError(status,"WPU_DebugEventGroupLoad",__LINE__);
   status = WPU_DebugEventGroupLoad(WP_SYSHANDLE(0), WPU_EVENT_ALL_WF, "BD_CORRUPTION_IWGP");
   WPE_TerminateOnError(status,"WPU_DebugEventGroupLoad",__LINE__);
   status = WPU_DebugEventGroupLoad(WP_SYSHANDLE(0), WPU_EVENT_ALL_WF, "BD_CORRUPTION_IWCP");
   WPE_TerminateOnError(status,"WPU_DebugEventGroupLoad",__LINE__);
   status = WPU_DebugEventGroupLoad(WP_SYSHANDLE(0), WPU_EVENT_ALL_WF, "BD_CORRUPTION_GENERAL");
   WPE_TerminateOnError(status,"WPU_DebugEventGroupLoad",__LINE__);

   WT_Identify();

   scenario=1;
   /*POST_COMMIT*/

   WPE_DefaultSetup(&system);

   while (scenario<7)
   {
      WPE_BuildScenario(&system,scenario);

      err = WPE_RunScenario(&system,scenario);
      if (err)
      {
         printf("Scenario number %d\n\n",scenario);
         printf("\nTest Failed\n");
         break;
      }

#ifndef USE_SOCKET


      WPE_DeleteBetweenScenarios(&system);
      WPE_ResetAllStatistics(&system);
#else

      WPE_DeleteBetweenScenarios(&system);

      status =WP_DriverRelease();
      WPE_TerminateOnError(status,"WP_DriverRelease",__LINE__);

      WPE_SystemInitialize(&system);

      WPE_PoolCreate(&system);
      WPE_PortDeviceSetup(&system);

      status=WP_SysCommit();
      WPE_TerminateOnError(status,"WP_SysCommit",__LINE__);

      /*POST_COMMIT*/

      WPE_DefaultSetup(&system);

#endif
      ++scenario;
#if (!CHECK_DATA_COPY_SCENARIO)
      if (scenario == 4)
      {
         printf("\nScenario 4 - Checking copy data + forwarding direct to channel - This feature is currently not checked\n");
         printf("-----------------------------------------------------------\n");
         ++scenario;
      }
#endif
#if (!CHECK_SCENARIO_6)
      if (scenario == 6)
      {
         printf("\nScenario 6 - Checking: host termination as member - This feature is currently not checked\n");
         printf("-----------------------------------------------------------\n");
         ++scenario;
      }
#endif
   }
//   CLI_DebugEventLogRead("asaf");
   if (!err)
      printf("\nTest Passed\n");
   WPE_Terminate(1);
   return 0;
}

#if UPI_MIXED_SHAPING
static void WPE_CreateShapingGroups(WPE_system *sys)
{
   WP_U32 ii,jj;

   WP_shaping_group_type_packet l1_group_config[] =
      {
         {
            /* group_level */     WP_L1_GROUP,
            /* tx_shaping_type */ WP_FMU_SHAPING_TYPE_STRICT,
            /* shaping_params */  NULL,
            /* num_fifos */       WP_NUM_FIFOS_4,
            /* block_handle */    0,
            /* block_level */     0,
            /* group_mode */      0
         }
      };

   WP_shaping_group_type_packet l2_group_config[] =
      {
         {
            /* group_level */     WP_L2_GROUP,
            /* tx_shaping_type */ WP_FMU_SHAPING_TYPE_STRICT,
            /* shaping_params */  NULL,
            /* num_fifos */       WP_NUM_FIFOS_8,
            /* block_handle */    0,
            /* block_level */     0,
            /* group_mode */      WP_MODE_HW
         }
      };
   sys->l1_block[0] = WP_ShapingBlockCreate(sys->iw_system, WP_L1_GROUP, SHAPING_BLOCK_SIZE, WP_NUM_FIFOS_4);
   WPE_TerminateOnError(sys->l1_block[0],"WP_ShapingBlockCreate L1 group",__LINE__);

   /*Create Shaping groups for Level 1 and Shaping blocks for level 2*/
   for (ii=0; ii<NUMBER_OF_UPI_PORTS; ii++)
   {
      l1_group_config->block_handle = sys->l1_block[0];
      l1_group_config->block_level = ii;
      sys->l1_group[ii] = WP_ShapingGroupCreate(sys->upidev[ii],WP_SHAPING_GROUP_TYPE_PACKET, l1_group_config);
      WPE_TerminateOnError(sys->l1_group[ii],"WP_ShapingGroupCreate L1 group",__LINE__);
      sys->l2_block[ii] = WP_ShapingBlockCreate(sys->iw_system, WP_L2_GROUP, SHAPING_BLOCK_SIZE, WP_NUM_FIFOS_8);
      WPE_TerminateOnError(sys->l2_block[ii],"WP_ShapingBlockCreate L2 group",__LINE__);
   }
   /*Now create level 2 groups*/
   for (ii=0; ii<NUMBER_OF_UPI_PORTS; ii++)
   {
      for (jj=0;jj<SHAPING_BLOCK_SIZE;jj++)
      {
         l2_group_config->block_level = jj;
         l2_group_config->block_handle = sys->l2_block[ii];
         sys->l2_group[ii][jj] = WP_ShapingGroupCreate(sys->l1_group[ii],WP_SHAPING_GROUP_TYPE_PACKET, l2_group_config);
         WPE_TerminateOnError(sys->l2_group[ii][jj],"WP_ShapingGroupCreate L2 group",__LINE__);
      }
   }
}
#endif

/*Run a specific scenario*/
static WP_U32 WPE_RunScenario(WPE_system *sys,WP_U32 scenario)
{
   WP_U32 err;
   switch(scenario)
   {
      case 1:
         err = WPE_RunS_1(sys);
         break;
      case 2:
         err = WPE_RunS_2(sys);
         break;
      case 3:
         err = WPE_RunS_3(sys);
         break;
      case 4:
         err = WPE_RunS_4(sys);
         break;
      case 5:
         err = WPE_RunS_5(sys);
         break;
      case 6:
         err = WPE_RunS_6(sys);
         break;
      default:
         err = WPE_ERR;
         break;
   }
   return err;
}

static void WPE_BuildDefaultPce(WPE_system *sys)
{
   WP_pce_rule_classification cls_rule;
   WP_U8 mac_addr2[6] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x01};
   WP_pce_result_flow_agg result_flow_agg;
   WP_U32 ii,jj,index;

   printf("- Creating PCE rules\n");

   cls_rule.filter_handle = sys->filter[0];
   cls_rule.enabled = WP_ENABLE;
   cls_rule.rule_fields[0].field_id = WP_PCE_FIELD_ID_INPUT_IW_PORT;
   cls_rule.rule_fields[0].value.iw_port_handle = sys->iw_port_enet[0];
   cls_rule.rule_fields[1].field_id = WP_PCE_FIELD_ID_MAC_DA;

   for(ii=0; ii<6; ii++)
      cls_rule.rule_fields[1].value.mac_addr[ii] = mac_addr2[ii];

   cls_rule.rule_fields[2].field_id = WP_PCE_FIELD_ID_INT_VLAN_TAG;
   cls_rule.rule_fields[3].field_id = WP_PCE_FIELD_ID_LAST;

   cls_rule.match_action = WP_PCE_RULE_MATCH_ACCEPT;
   index=1;

   /*First - Add Classification rules to PCE directing to normal Enet flows*/
   for (jj=1;jj<NUMBER_OF_ENET_PORTS;jj++)
   {
      for (ii=0;ii<sys->num_tx_on_enet;ii++)
      {
         cls_rule.tag=index;
         cls_rule.rule_fields[2].value.vlan_tag =index;

         cls_rule.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
         result_flow_agg.flow_aggregation =sys->flow_enet[jj][ii];
         cls_rule.match_result[0].param.flow_agg = result_flow_agg;


         cls_rule.match_result[1].result_type = WP_PCE_RESULT_BACKGROUND;
         cls_rule.match_result[1].param.background.action = WP_ENABLE;
         cls_rule.match_result[2].result_type = WP_PCE_RESULT_LAST;

         sys->rule[index-1]=WP_PceRuleCreate(WP_WINPATH(0), WP_PCE_RULE_CLASSIFICATION, &cls_rule);
         WPE_TerminateOnError(sys->rule[index-1], "WP_PceRuleCreate",__LINE__);

         ++(sys->n_rules);
         ++index;
#if TEST_DEBUG
         printf("Line is %d,index = %d\n",__LINE__,index);  //eladj - DEBUG
#endif
      }
   }

   cls_rule.rule_fields[1].value.mac_addr[0] = 0x01;
   sys->vlan_mc_start=index;
   /*Now add Pce Classification rules for the Multicast flows*/
   for (ii=0;ii<sys->num_of_groups;ii++)
   {
      cls_rule.rule_fields[2].value.vlan_tag=index;
      cls_rule.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
      result_flow_agg.flow_aggregation =sys->flow_mc[ii];
      cls_rule.match_result[0].param.flow_agg = result_flow_agg;

      cls_rule.match_result[1].result_type = WP_PCE_RESULT_BACKGROUND;
      cls_rule.match_result[1].param.background.action = WP_ENABLE;
      cls_rule.match_result[2].result_type = WP_PCE_RESULT_LAST;

      sys->rule[index-1]=WP_PceRuleCreate(WP_WINPATH(0), WP_PCE_RULE_CLASSIFICATION, &cls_rule);
      WPE_TerminateOnError(sys->rule[index], "WP_PceRuleCreate",__LINE__);

      ++(sys->n_rules);
      ++index;
#if TEST_DEBUG
      printf("Line is %d,index = %d\n",__LINE__,index);  //eladj - DEBUG
#endif
   }
   sys->vlan_upi_start=index;
   cls_rule.rule_fields[1].value.mac_addr[0] = 0x00;

   /*Add a rule for the Hspos flows*/
   for (ii=0;ii<NUMBER_OF_UPI_PORTS;ii++)
   {
      for (jj=0;jj<sys->num_tx_on_upi;jj++)
      {
         cls_rule.rule_fields[2].value.vlan_tag=index;
         cls_rule.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
         result_flow_agg.flow_aggregation =sys->flow_upi[ii][jj];
         cls_rule.match_result[0].param.flow_agg = result_flow_agg;

         cls_rule.match_result[1].result_type = WP_PCE_RESULT_BACKGROUND;
         cls_rule.match_result[1].param.background.action = WP_ENABLE;
         cls_rule.match_result[2].result_type = WP_PCE_RESULT_LAST;

         sys->rule[index-1]=WP_PceRuleCreate(WP_WINPATH(0), WP_PCE_RULE_CLASSIFICATION, &cls_rule);
         WPE_TerminateOnError(sys->rule[index-1], "WP_PceRuleCreate",__LINE__);

         ++(sys->n_rules);
         ++index;
      }
   }
   /*Apply changes*/
   status=WP_IwSystemBuild(sys->iw_system);
   WPE_TerminateOnError(status,"WP_IwSystemBuild",__LINE__);
}

static void WPE_PceInterfaces(WPE_system *sys)
{
   WP_pce_if_params_pkt_rx_channel interface_params;
   WP_U32 ii=0;
   WP_rx_binding_bridging  rx_binding[1];

   memset (rx_binding, 0, sizeof (WP_rx_binding_bridging));
   interface_params.mode = WP_PCE_IW_PORT_CONNECTION_ENABLED;
   interface_params.parser_start_type = WP_PCE_PARSER_START_TYPE_ETHERNET;
   interface_params.filter_set_handle = sys->filter_set_handle;
   interface_params.ip_header_validation = WP_DISABLE;
   interface_params.sub_port_vlan = WP_DISABLE;
   interface_params.collect_tpids = WP_DISABLE;

   for (ii=0;ii<NUMBER_OF_ENET_PORTS;ii++)
   {
      sys->interfaces[ii]=WP_PceInterfaceCreate(WP_PCE_IF_TYPE_PKT_RX_CHANNEL,&interface_params);
      WPE_TerminateOnError(sys->interfaces[ii], "WP_PceInterfaceCreate",__LINE__);
      rx_binding[0].pce_if_handle = sys->interfaces[ii];
      status = WP_IwRxBindingModify(sys->rx_enet_handle[ii],
                                    sys->iw_system,
                                    sys->qniw,
                                    WP_IW_RX_BIND_MOD_PCE_INTERFACE,
                                    rx_binding);
      WPE_TerminateOnError(status,"WP_IwRxBindingModify",__LINE__);
   }
   for (ii=0;ii<NUMBER_OF_UPI_PORTS;ii++)
   {
      sys->interfaces[ii+NUMBER_OF_ENET_PORTS]=WP_PceInterfaceCreate(WP_PCE_IF_TYPE_PKT_RX_CHANNEL,&interface_params);
      WPE_TerminateOnError(sys->interfaces[ii+NUMBER_OF_ENET_PORTS], "WP_PceInterfaceCreate",__LINE__);
      rx_binding[0].pce_if_handle = sys->interfaces[ii+NUMBER_OF_ENET_PORTS];
      status = WP_IwRxBindingModify(sys->rx_upi_handle[ii],
                                    sys->iw_system,
                                    sys->qniw,
                                    WP_IW_RX_BIND_MOD_PCE_INTERFACE,
                                    rx_binding);
      WPE_TerminateOnError(status,"WP_IwRxBindingModify",__LINE__);
   }
}

static void WPE_CreateFilterSets(WPE_system *sys)
{
   WP_pce_filter_set fs_cfg;

   fs_cfg.filter_set_level = 0;
   fs_cfg.next_filter_set = WP_UNUSED;
   fs_cfg.filters[0] = sys->filter[0];
   fs_cfg.filters[1] = WP_UNUSED;

   sys->filter_set_handle = WP_PceFilterSetCreate(WP_WINPATH(0), &fs_cfg);
   WPE_TerminateOnError(sys->filter_set_handle, "WP_PceFilterSetCreate",__LINE__);
}


static void WPE_PceFiltersCreate(WPE_system *sys)
{
   WP_pce_filter_classification filter_clas_cfg = {0};

   filter_clas_cfg.no_match_action = WP_PCE_FILTER_NO_MATCH_DENY;
   filter_clas_cfg.no_fields_action = WP_PCE_FILTER_NO_FIELDS_DENY;

   filter_clas_cfg.filter_fields[0].field_id = WP_PCE_FIELD_ID_INPUT_IW_PORT;
   filter_clas_cfg.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_clas_cfg.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   filter_clas_cfg.filter_fields[1].field_id = WP_PCE_FIELD_ID_MAC_DA;
   filter_clas_cfg.filter_fields[1].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_clas_cfg.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   filter_clas_cfg.filter_fields[2].field_id = WP_PCE_FIELD_ID_INT_VLAN_TAG;
   filter_clas_cfg.filter_fields[2].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_clas_cfg.filter_fields[2].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   filter_clas_cfg.filter_fields[3].field_id = WP_PCE_FIELD_ID_LAST;
   filter_clas_cfg.filter_fields[3].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_clas_cfg.filter_fields[3].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   filter_clas_cfg.no_match_result[0].result_type = WP_PCE_RESULT_LAST;
   sys->filter[0]=WP_PceFilterCreate(WP_WINPATH(0), WP_PCE_FILTER_CLASSIFICATION, &filter_clas_cfg);
   WPE_TerminateOnError(sys->filter[0], "WP_PceFilterCreate",__LINE__);
}

static void WPE_DeletePceRules(WPE_system *sys)
{
   WP_U32 ii;

   /*Delete PCE rules*/
   for (ii=0;ii<sys->n_rules;ii++)
   {
      status = WP_PceRuleDelete(sys->rule[ii]);
      WPE_TerminateOnError(status,"WP_PceRuleDelete",__LINE__);
   }
   sys->n_rules=0;

   /*Apply changes*/
   status=WP_IwSystemBuild(sys->iw_system);
   WPE_TerminateOnError(status,"WP_IwSystemBuild",__LINE__);
}

static void WPE_SendToAllVlans(WPE_system *sys,WP_U32 n)
{
   WP_U32 ii,jj,v_id=1,length = 86;
   WP_CHAR vtag[5],dest[13] = "000000000001";

   printf("- Sending Packets\n");

   for (ii=0;ii<n;ii++)
   {
      for (jj=0;jj<sys->n_rules;jj++)
      {
//         WPT_DisplayDeviceStats(sys->upidev[0],WP_DEVICE_HSPOS,"HSPOS");
//         WPT_DisplayBFlowStats(sys->flow_upi[0][0],"UPI");

         v_id=1+jj;
         sprintf(vtag,"%04x",v_id);
#if TEST_DEBUG
         printf("Vlan_id=%s\n",vtag);
#endif
         if ((v_id>=sys->vlan_mc_start)&&(v_id <sys->vlan_upi_start))
            dest[1] = '1';
         else
            dest[1] = '0';
         APP_DataUnitsSetup(sys,&(sys->tx_enet_handle[0][0]),length,"00000000000d",dest,"00000002","00000005",
                            "04",vtag);
#if VARIOUS_PACKET_LENGTHS
         length += 120;
#endif
         if (length >= 1500)
            length = 86;
         }
      }

}


static void WPE_SendToAllMc(WPE_system *sys,WP_U32 n)
{
   WP_U32 ii,jj,v_id=1,length =86;
   WP_CHAR vtag[5],dest[13] = "010000000001";

   printf("- Sending MC Packets\n");

   for (ii=0;ii<n;ii++)
   {
      for (jj=0;jj<sys->num_of_groups;jj++)
      {
         v_id=sys->vlan_mc_start+jj;
         sprintf(vtag,"%04x",v_id);
#if TEST_DEBUG
         printf("Vlan_id=%s\n",vtag);
#endif
         APP_DataUnitsSetup(sys,&(sys->tx_enet_handle[0][0]),length,"00000000000d",dest,"00000002","00000005",
                            "04",vtag);
#if VARIOUS_PACKET_LENGTHS
         length += 120;
#endif
         if (length >= 1500)
            length = 86;
      }
   }

}


static WP_U32 WPE_RunS_1(WPE_system *sys)
{
   WP_U32 err;
   WP_U32 mc_qnode_before,iw_qnode_before;

#if CHECK_FIWT
   WP_iw_generic_agg_multicast mc_agg_config[1] =
      {
         {
            /* tag */0,
            /* type */WP_IW_FLOW_AGG_PRIMARY,
            /* mc_group */0,
            /* mtu */1536,
            /* intmode */WP_IW_INT_DISABLE,
            /* interruptqueue */WP_IW_IRQT1,
            /* statmode */WP_IW_STAT_ENABLE,
            /* timestamp_mode */WP_IW_TIME_STAMP_DISABLE,
            /* ov_pool_mode */WP_IW_OV_POOL_ENABLE,
            /* fbp_drop_threshold */0,
            /* policer_enable */WP_IW_POLICER_DISABLE,
            /* *policer_config */NULL,
            /* policer handle*/ 0,
            /* pecs_handle*/0,
            /*pecs_user_info*/ NULL
         }
      };
   WP_iw_mc_member mc_member_config[1] =
      {
         {
            /* txfunc */0,
            /* service_class_index */0,
            /* buffers_threshold */0,
            /* iw_port */0,
            /* edit_size */0x10,
            /* data_copy_mode */WP_IW_MC_MEMBER_DATACOPY_DISABLE,
            /*iw_port_check_mode*/WP_IW_MC_MEMBER_IWPORT_CHECK_ENABLE
         }
      };

   sys->num_of_groups = 1;
#endif
   status = WP_QNodeStatus(sys->qn_mc,WP_QNODE_STATUS_FIFO,&mc_qnode_before);
   WPE_TerminateOnError(status,"WP_QnodeStatus",__LINE__);
   status = WP_QNodeStatus(sys->qniw,WP_QNODE_STATUS_FIFO,&iw_qnode_before);
   WPE_TerminateOnError(status,"WP_QnodeStatus",__LINE__);

   WPE_BuildDefaultPce(sys);
   /*Now add memebers to the Mc group*/
   WPE_AddMCmembers(sys);

   /*Apply changes*/
   status=WP_IwSystemBuild(sys->iw_system);
   WPE_TerminateOnError(status,"WP_IwSystemBuild",__LINE__);
#if CHECK_FIWT
   mc_member_config->txfunc = sys->flow_enet[1][0];
   sys->mc_member_handle[1][0] = WP_IwMcMemberAdd(sys->mc_group_handle[1],
                                                  mc_member_config);
   WPE_TerminateOnError(sys->mc_member_handle[1][0], "WP_IwMcMemberAdd()",__LINE__);
   sys->num_of_members[1]=1;

   /*Check new generic MC Flow for disable and enable*/
   status = WP_IwFlowAggregationDisable(sys->flow_mc[0]);
   WPE_TerminateOnError(status,"WP_IwFlowAggregationDisable",__LINE__);
   WP_Delay(100);

   mc_agg_config->mc_group = sys->mc_group_handle[1];

   status = WP_IwFlowAggregationModify(sys->flow_mc[0],WP_IW_FLOW_AGG_GENERIC_MC_MOD_GROUP_ID,mc_agg_config);
   WPE_TerminateOnError(status,"WP_IwFlowAggregationModify",__LINE__);

   mc_agg_config->mc_group = sys->mc_group_handle[0];

   status = WP_IwFlowAggregationModify(sys->flow_mc[0],WP_IW_FLOW_AGG_GENERIC_MC_MOD_GROUP_ID,mc_agg_config);
   WPE_TerminateOnError(status,"WP_IwFlowAggregationModify",__LINE__);

   status = WP_IwFlowAggregationEnable(sys->flow_mc[0]);
   WPE_TerminateOnError(status,"WP_IwFlowAggregationDisable",__LINE__);
   WP_Delay(1000);
#endif

   /*Start Host send*/
   WPE_SendToAllVlans(sys,10);
#if TEST_DEBUG
   WPT_DisplayBPortStats(sys->iw_port_host,"Host");
   WPT_DisplayBFlowStats(sys->flow_host,"Host");
   WPE_PrintPceFilterStats(sys->filter[0]);
   WPE_PrintPceGlobalStats();
#endif

   /*Verify Results*/
   err = WPE_VerifyScenario(sys,1,1,10);
   if (err)
      return err;
   err = WPE_CheckQnodes(sys,mc_qnode_before,iw_qnode_before);
#if CHECK_FIWT
   sys->num_of_groups = 2;
#endif
   return err;
}

static WP_U32 WPE_CheckQnodes(WPE_system *sys,WP_U32 mc_qnode_expected,WP_U32 iw_qnode_expected)
{
   WP_U32 mc_qnode_after,iw_qnode_after;

   status = WP_QNodeStatus(sys->qn_mc,WP_QNODE_STATUS_FIFO,&mc_qnode_after);
   WPE_TerminateOnError(status,"WP_QnodeStatus",__LINE__);
   status = WP_QNodeStatus(sys->qniw,WP_QNODE_STATUS_FIFO,&iw_qnode_after);
   WPE_TerminateOnError(status,"WP_QnodeStatus",__LINE__);
   if (mc_qnode_expected != mc_qnode_after)
   {
      printf("Error - incorrect BD count in MC Qnode : \n");
      printf("before scenario - %d\n",mc_qnode_expected);
      printf("After scenario - %d\n",mc_qnode_after);
      return WPE_ERR;
   }
   if (iw_qnode_expected != iw_qnode_after)
   {
      printf("Error - incorrect BD count in IW Qnode : \n");
      printf("before scenario - %d\n",iw_qnode_expected);
      printf("After scenario - %d\n",iw_qnode_after);
      return WPE_ERR;
   }
   return WPE_GOOD;
}


static WP_U32 WPE_CheckStatistics(WPE_system *sys,WP_U32 expected1,WP_U32 expected2,WP_S32 diff1,WP_S32 diff2)
{
   WP_stats_enet stats_hs_enet = {0};
   WP_stats_upi_hspos stats_hspos = {0};
   WP_bport_stats bport_stats;
   WP_iw_flow_stats iw_flow_stats;
   WP_U32 ii;
   WP_U32 value1,value2;
#if TEST_DEBUG
   WPT_DisplayDeviceStats(sys->enetdev[0],WP_DEVICE_ENET,"Device");
   WPT_DisplayBFlowStats(sys->flow_mc[0],"MC");
   WPT_DisplayBFlowStats(sys->flow_upi[0][0],"UPI");
#endif

   for (ii=1;ii<NUMBER_OF_ENET_PORTS;ii++)
   {
      memset(&stats_hs_enet,0,sizeof(WP_stats_enet));
      WP_Delay(1000);
      status = WP_DeviceStatistics(sys->enetdev[ii], &stats_hs_enet);
      WPE_TerminateOnError(status,"WP_DeviceStatistics",__LINE__);
      value1 = stats_hs_enet.rx_packets;
      value2 = stats_hs_enet.tx_packets;
#if TEST_DEBUG
      WPT_DisplayDeviceStats(sys->enetdev[ii],WP_DEVICE_ENET,"Device");
      WPT_DisplayBFlowStats(sys->flow_enet[ii][0],"Enet");
      WPE_PrintPceFilterStats(sys->filter[0]);
      WPE_PrintPceGlobalStats();

#endif
      if (value1 != value2)
      {
         WPT_DisplayDeviceStats(sys->enetdev[ii],WP_DEVICE_ENET,"Device");
         printf("Error - Tx packets %d and Rx packets %d don't match on Enet device\n",value1,value2);
         return WPE_ERR;
      }
      else
         if (value1!= expected1)
         {
            WPT_DisplayDeviceStats(sys->enetdev[ii],WP_DEVICE_ENET,"Device");
            printf("Error - only %d packets arrived on Enet device %d instead of %d\n",value1,ii,expected1);
            return WPE_ERR;
         }
      expected1 += diff1;
      memset( &bport_stats, 0, sizeof(WP_bport_stats) );
      status = WP_IwBridgingStatistics(sys->iw_port_enet[ii] , &bport_stats );
      WPE_TerminateOnError(status,"WP_IwBridgingStatistics",__LINE__);
      value1 = bport_stats.forwarded_uc_packets;
      value2 = bport_stats.rx_mc_valid_packets;
#if 0 /*these MC statistics are currently not available*/
      if (value1 != value2)
      {
         WPT_DisplayBPortStats(sys->iw_port_enet[ii],"ii");
         printf("Error - Forwarded Uc packets and Forwarded Mc packets don't match on Iw port\n");
         return WPE_ERR;
      }
      else
         if (value1!= expected2)
         {
            WPT_DisplayBPortStats(sys->iw_port_enet[ii],"ii");
            printf("Error - only %d forwarded packets on Enet Iw port instead of %d\n",value1,expected2);
            return WPE_ERR;
         }
#endif
   }

   /*Check MC flow statistics*/
   for (ii =0; ii< sys->num_of_groups ; ii++)
   {
      memset(&iw_flow_stats,0,sizeof(iw_flow_stats));
      status = WP_IwFlowStatistics(sys->flow_mc[ii],WP_IW_FLOW_STAT,&iw_flow_stats);
      WPE_TerminateOnError(status,"WP_IwFlowStatistics",__LINE__);
      value1 = iw_flow_stats.forward_packet;
      if (value1 != (expected2 + diff2*ii))
      {
         WPT_DisplayBFlowStats(sys->flow_mc[ii],"Mc flow");
         printf("Error - only %d forwarded packets on MC flow %d instead of %d\n",value1,ii,expected2+diff2*ii);
         return WPE_ERR;
      }
   }
   /*Check UPI statistics*/
   if (sys->num_tx_on_upi)
      for (ii=0;ii<NUMBER_OF_UPI_PORTS;ii++)
      {
         memset(&stats_hspos,0,sizeof(WP_stats_upi_hspos));
         status = WP_DeviceStatistics(sys->upidev[ii],&stats_hspos );
         WPE_TerminateOnError(status,"WP_DeviceStatistics",__LINE__);
         value1 = stats_hspos.tx_frames;
         value2 = stats_hspos.rx_iw_frames;
         if (value1!= expected1)
         {
            WPT_DisplayDeviceStats(sys->upidev[ii],WP_DEVICE_HSPOS,"HSPOS");
            printf("Error - only %d packets arrived on UPI device %d instead of %d\n",value1,ii,expected1);
            return WPE_ERR;
         }
         expected1 += diff1;
      }
   return WPE_GOOD;
}

static void WPE_ResetAllStatistics(WPE_system *sys)
{
   WP_U32 ii,jj;
   WP_stats_enet stats_hs_enet = {0};
   WP_bport_stats bport_stats = {0};
   WP_iw_flow_stats iw_flow_stats = {0};
//   WP_iw_mc_class_stats mc_stats =  {0};
   WP_stats_upi_hspos stats_hspos = {0};

   for (ii =0;ii<NUMBER_OF_ENET_PORTS;ii++)
   {
      status = WP_DeviceStatisticsReset(sys->enetdev[ii], &stats_hs_enet);
      WPE_TerminateOnError(status,"WP_DeviceStatisticsReset",__LINE__);
      status = WP_IwPortStatisticsReset(sys->iw_port_enet[ii], &bport_stats);
      WPE_TerminateOnError(status,"WP_IwPortStatisticsReset",__LINE__);
      for (jj =0 ;jj<sys->num_tx_on_enet; jj++)
      {
         status = WP_IwFlowStatisticsReset(sys->flow_enet[ii][jj],WP_IW_FLOW_STAT, &iw_flow_stats);
         WPE_TerminateOnError(status,"WP_IwPortStatisticsReset",__LINE__);
      }
   }

   for (ii =0 ;ii<sys->num_of_groups;ii++)
   {
      status = WP_IwFlowStatisticsReset(sys->flow_mc[ii], WP_IW_FLOW_STAT, &iw_flow_stats);
      WPE_TerminateOnError(status,"WP_IwPortStatisticsReset",__LINE__);
/*      status = WP_IwMcStatisticsReset(sys->mc_group_handle[ii], 0,&mc_stats);
        WPE_TerminateOnError(status,"WP_IwPortStatisticsReset",__LINE__);
*/
   }
   /*Now reset Upi statistics*/
   for (ii =0;ii<NUMBER_OF_UPI_PORTS;ii++)
   {
      status = WP_DeviceStatisticsReset(sys->upidev[ii], &stats_hspos);
      WPE_TerminateOnError(status,"WP_DeviceStatisticsReset",__LINE__);
      status = WP_IwPortStatisticsReset(sys->iw_port_upi[ii], &bport_stats);
      WPE_TerminateOnError(status,"WP_IwPortStatisticsReset",__LINE__);
      for (jj =0 ;jj<sys->num_tx_on_upi; jj++)
      {
         if (sys->flow_upi[ii][jj]!=0)
         {
            status = WP_IwFlowStatisticsReset(sys->flow_upi[ii][jj],WP_IW_FLOW_STAT, &iw_flow_stats);
            WPE_TerminateOnError(status,"WP_IwPortStatisticsReset",__LINE__);
         }
      }
   }
}

static WP_U32 WPE_VerifyScenario(WPE_system *sys,WP_U32 s,WP_U32 phase,WP_U32 n)
{
   WP_U32 check =0;

   printf("- Verifying Packets\n");
   WP_Delay(10000);
   switch (s)
   {
      case 1:
         check = WPE_CheckStatistics(sys,n*2,n,0,0);
         break;
      case 2:
         if (phase == 1)
            check = WPE_CheckStatistics(sys,n*2,n,-10,-n);
         else
            check = WPE_CheckStatistics(sys,n*2,n-10,0,10);
         break;
      case 3:
         check = WPE_CheckStatistics(sys,n*(sys->num_of_groups),n,-n,0);
         break;
      case 4:
         check = WPE_CheckStatistics(sys,n*(sys->num_tx_on_enet+1),n,0,0);
         break;
      case 5:
         check = WPE_CheckStatistics(sys,n*(sys->num_of_groups*(sys->num_tx_on_enet)),n,0,0);
         break;
      case 6:
      {
         WP_iw_flow_stats iw_flow_stats;
         WP_U32 value1;

         memset(&iw_flow_stats,0,sizeof(iw_flow_stats));
         status = WP_IwFlowStatistics(sys->flow_host,WP_IW_FLOW_STAT,&iw_flow_stats);
         WPE_TerminateOnError(status,"WP_IwFlowStatistics",__LINE__);
         value1 = iw_flow_stats.forward_packet;

         if (value1 != n)
         {
            WPT_DisplayBFlowStats(sys->flow_host,"Host");
            printf("Error - Only %d packets arrived to host instead of %d\n",value1,n);
            check = WPE_ERR;
         }
      }
      break;
      default:
         return WPE_ERR;
   }
   return check;
}

static WP_U32 WPE_RunS_2(WPE_system *sys)
{
   WP_U32 err,jj,port,index=0;
   WP_U32 mc_qnode_before,iw_qnode_before;

   WP_iw_mc_member mc_member_config[1] =
      {
         {
            /* txfunc */0,
            /* service_class_index */0,
            /* buffers_threshold */0,
            /* iw_port */0,
            /* edit_size */0x20,
            /* data_copy_mode */WP_IW_MC_MEMBER_DATACOPY_DISABLE,
            /*iw_port_check_mode*/WP_IW_MC_MEMBER_IWPORT_CHECK_ENABLE
         }
      };
   status = WP_QNodeStatus(sys->qn_mc,WP_QNODE_STATUS_FIFO,&mc_qnode_before);
   WPE_TerminateOnError(status,"WP_QnodeStatus",__LINE__);
   status = WP_QNodeStatus(sys->qniw,WP_QNODE_STATUS_FIFO,&iw_qnode_before);
   WPE_TerminateOnError(status,"WP_QnodeStatus",__LINE__);

   WPE_BuildDefaultPce(sys);
   /*Add members of Enet ports*/
   sys->num_of_members[0]=0;
   index=0;
   for (port=1;port<NUMBER_OF_ENET_PORTS;port++)
   {

      mc_member_config->iw_port = sys->iw_port_enet[port];
      for (jj=0;jj<sys->num_tx_on_enet;jj++)
      {
         mc_member_config->txfunc = sys->flow_enet[port][jj];
         sys->mc_member_handle[0][index] = WP_IwMcMemberAdd(sys->mc_group_handle[0],
                                                            mc_member_config);
         WPE_TerminateOnError(sys->mc_member_handle[0][index], "WP_IwMcMemberAdd()",__LINE__);
         ++index;

         /*Host sending*/
         WPE_SendToAllVlans(sys,10);
      }
   }
   sys->num_of_members[0]=index;

   /*add the HSPOS as member*/
   if (sys->num_tx_on_upi)
   {

      mc_member_config->iw_port = sys->iw_port_upi[0];
      mc_member_config->txfunc = sys->flow_upi[0][0];
      sys->mc_member_handle[0][index] = WP_IwMcMemberAdd(sys->mc_group_handle[0],
                                                         mc_member_config);
      WPE_TerminateOnError(sys->mc_member_handle[0][index], "WP_IwMcMemberAdd()",__LINE__);
      ++(sys->num_of_members[0]);

      /*Host sending*/
      WPE_SendToAllVlans(sys,10);
   }

   /*Verify statistics for first phase*/
   err = WPE_VerifyScenario(sys,2,1,10*(((NUMBER_OF_ENET_PORTS-1)*(sys->num_tx_on_enet))+NUMBER_OF_UPI_PORTS*sys->num_tx_on_upi));
   if (err)
      return err;
   err = WPE_CheckQnodes(sys,mc_qnode_before,iw_qnode_before);
   if (err)
      return err;
   /* Reset statistics between phases*/
   WPE_ResetAllStatistics(sys);
   /*Second phase - Move members from the first group to the second group*/
   port=1;
   index=0;

   for (jj=0;jj<(sys->num_of_members[0]);jj++)
   {
      status = WP_IwMcMemberDisable(sys->mc_group_handle[0],sys->mc_member_handle[0][jj]);
      WPE_TerminateOnError(status,"WP_IwMcMemberDisable",__LINE__);
      status = WP_IwMcMemberDelete(sys->mc_group_handle[0],sys->mc_member_handle[0][jj]);
      WPE_TerminateOnError(status,"WP_IwMcMemberDelete",__LINE__);
      if (jj<sys->num_of_members[0]-NUMBER_OF_UPI_PORTS)
      {
    	  mc_member_config->iw_port = sys->iw_port_enet[port];
    	  mc_member_config->txfunc = sys->flow_enet[port][index];
      }
      else
      {
    	  mc_member_config->iw_port = sys->iw_port_upi[0];
    	   mc_member_config->txfunc = sys->flow_upi[0][0];
      }
      sys->mc_member_handle[1][jj] = WP_IwMcMemberAdd(sys->mc_group_handle[1],
                                                      mc_member_config);
      WPE_TerminateOnError(sys->mc_member_handle[1][jj], "WP_IwMcMemberAdd()",__LINE__);

      ++index;
      if (index==sys->num_tx_on_enet)
      {
         index=0;
         ++port;
      }

      /*Host sending*/
      WPE_SendToAllVlans(sys,10);
   }
   sys->num_of_members[0]=0;

   sys->num_of_members[1]=jj;

   /*Verify statistics for Second phase*/
   err = WPE_VerifyScenario(sys,2,2,10*(((NUMBER_OF_ENET_PORTS-1)*(sys->num_tx_on_enet))+NUMBER_OF_UPI_PORTS*sys->num_tx_on_upi));
   if (err)
      return err;
   err = WPE_CheckQnodes(sys,mc_qnode_before,iw_qnode_before);
   return err;
}

static WP_U32 WPE_RunS_3(WPE_system *sys)
{
   WP_U32 temp_num=0,err =0;
   WP_U32 mc_qnode_before,iw_qnode_before;

   if (sys->num_of_groups>NUMBER_OF_ENET_PORTS)
   {
      printf("Number of groups for this scenario must not be more than number of devices!\n");
      printf("Adjusting number of groups to %d\n", NUMBER_OF_ENET_PORTS-1);
      temp_num = sys->num_of_groups;
      sys->num_of_groups=NUMBER_OF_ENET_PORTS-1;

   }
   status = WP_QNodeStatus(sys->qn_mc,WP_QNODE_STATUS_FIFO,&mc_qnode_before);
   WPE_TerminateOnError(status,"WP_QnodeStatus",__LINE__);
   status = WP_QNodeStatus(sys->qniw,WP_QNODE_STATUS_FIFO,&iw_qnode_before);
   WPE_TerminateOnError(status,"WP_QnodeStatus",__LINE__);

   WPE_BuildDefaultPce(sys);

   /*Add members according to scenario 3*/
   WPE_S3_MemAdd(sys);

/*Host sending here*/
   WPE_SendToAllMc(sys,10);

   /*Verify statistics for first phase*/
   err = WPE_VerifyScenario(sys,3,1,10);
   if (err)
      return err;
   err = WPE_CheckQnodes(sys,mc_qnode_before,iw_qnode_before);
   if (err)
      return err;
   printf("- Second scenario phase\n");

   /* Reset statistics between phases*/
   WPE_ResetAllStatistics(sys);

/*Delete all MC members, groups and Flow aggregations*/
   if (temp_num != 0)
      sys->num_of_groups = temp_num;
   WPE_DeletePceRules(sys);
   WPE_DeleteAllMC(sys);

   /*Repeating scenario*/
   if (sys->num_of_groups>NUMBER_OF_ENET_PORTS)
   {
      sys->num_of_groups=NUMBER_OF_ENET_PORTS-1;
   }

   WPE_CreateMCgroups(sys);
   WPE_BuildDefaultPce(sys);
   WPE_S3_MemAdd(sys);

   /* Host sending here*/
   WPE_SendToAllMc(sys,10);
   err = WPE_VerifyScenario(sys,3,2,10);
   if (err)
      return err;
   err = WPE_CheckQnodes(sys,mc_qnode_before,iw_qnode_before);
   return err;

}

static WP_U32 WPE_RunS_4(WPE_system *sys)
{
   WP_U32 jj,port,index=0,err =0;
   WP_U32 ii,v_id=1;
   WP_CHAR vtag[5];
   WP_U32 mc_qnode_before,iw_qnode_before;

   WP_iw_mc_member mc_member_config[1] =
      {
         {
            /* txfunc */0,
            /* service_class_index */0,
            /* buffers_threshold */0,
            /* iw_port */0,
            /* edit_size */0x20,
            /* data_copy_mode */WP_IW_MC_MEMBER_DATACOPY_DISABLE,
            /*iw_port_check_mode*/WP_IW_MC_MEMBER_IWPORT_CHECK_ENABLE
         }
      };

   status = WP_QNodeStatus(sys->qn_mc,WP_QNODE_STATUS_FIFO,&mc_qnode_before);
   WPE_TerminateOnError(status,"WP_QnodeStatus",__LINE__);
   status = WP_QNodeStatus(sys->qniw,WP_QNODE_STATUS_FIFO,&iw_qnode_before);
   WPE_TerminateOnError(status,"WP_QnodeStatus",__LINE__);

   WPE_BuildDefaultPce(sys);

   /*Add flows of Enet ports as members to the first group*/
   sys->num_of_members[0]=0;
   index=0;
   mc_member_config->data_copy_mode=WP_IW_MC_MEMBER_DATACOPY_ENABLE;

   for (port=1;port<NUMBER_OF_ENET_PORTS;port++)
   {
      mc_member_config->iw_port = sys->iw_port_enet[port];
      for (jj=0;jj<sys->num_tx_on_enet;jj++)
      {
         mc_member_config->txfunc = sys->flow_enet[port][jj];

         sys->mc_member_handle[0][index] = WP_IwMcMemberAdd(sys->mc_group_handle[0],
                                                            mc_member_config);
         WPE_TerminateOnError(sys->mc_member_handle[0][index], "WP_IwMcMemberAdd()",__LINE__);
         ++index;
      }
   }
   sys->num_of_members[0]=index;
#if 0
   /*Add Tx Channels of Enet ports as members to the second group*/
   sys->num_of_members[1]=0;
   index=0;
   mc_member_config->edit_size=0x0;
   mc_member_config->data_copy_mode=WP_IW_MC_MEMBER_DATACOPY_DISABLE;

   for (port=1;port<NUMBER_OF_ENET_PORTS;port++)
   {
      mc_member_config->iw_port = sys->iw_port_enet[port];
      for (jj=0;jj<sys->num_tx_on_enet;jj++)
      {
         mc_member_config->txfunc = sys->tx_enet_handle[port][jj];

         sys->mc_member_handle[1][index] = WP_IwMcMemberAdd(sys->mc_group_handle[1],
                                                            mc_member_config);
         WPE_TerminateOnError(sys->mc_member_handle[1][index], "WP_IwMcMemberAdd()",__LINE__);
         ++index;
      }
   }
   sys->num_of_members[1]=index;
#endif
   sys->num_of_groups =1;

   /*Host Sends here*/
   WPE_SendToAllMc(sys,10);

   /*Send another 10 packets directing to the first flow of each device*/
//WPE_SendToAllVlans(sys,10);
   for (ii=0;ii<10;ii++)
   {
      for (jj=0;jj<sys->vlan_mc_start-1;)
      {
         v_id=1+jj;
         sprintf(vtag,"%04x",v_id);
         APP_DataUnitsSetup(sys,&(sys->tx_enet_handle[0][0]),86,"00000000000d","000000000001","00000002","00000005",
                            "04",vtag);
         jj+=sys->num_tx_on_enet;
      }
   }
   err = WPE_VerifyScenario(sys,4,1,10);
   if (err)
      return err;
   err = WPE_CheckQnodes(sys,mc_qnode_before,iw_qnode_before);
   return err;
}

static WP_U32 WPE_RunS_5(WPE_system *sys)
{
   WP_U32 err,ii,N=1;
   WP_U32 mc_qnode_before,iw_qnode_before;

   total = 0;

   status = WP_QNodeStatus(sys->qn_mc,WP_QNODE_STATUS_FIFO,&mc_qnode_before);
   WPE_TerminateOnError(status,"WP_QnodeStatus",__LINE__);
   status = WP_QNodeStatus(sys->qniw,WP_QNODE_STATUS_FIFO,&iw_qnode_before);
   WPE_TerminateOnError(status,"WP_QnodeStatus",__LINE__);

   WPE_BuildDefaultPce(sys);
   printf("- Creating %d members\n",N*131072);
   for (ii=0;ii<N;ii++)
   {
      WPE_AddMCmembers(sys);
#if TEST_DEBUG
      printf("ii= %d\n",ii);
#endif
   }
   /*Host Sends here*/
   WPE_SendToAllMc(sys,1);
   err = WPE_VerifyScenario(sys,5,1,N);
   if (err)
      return err;
   err = WPE_CheckQnodes(sys,mc_qnode_before,iw_qnode_before);
   if (err)
      return err;
   printf("- Second scenario phase\n");
   total = 0;

   WPE_DeletePceRules(sys);
   WPE_DeleteAllMC(sys);
   /*Repeat Scenario*/
   WPE_CreateMCgroups(sys);
   WPE_BuildDefaultPce(sys);
   printf("- Creating %d members\n",N*131072);
   for (ii=0;ii<N;ii++)
      WPE_AddMCmembers(sys);

   /* Reset statistics between phases*/
   WPE_ResetAllStatistics(sys);

   /*Host Sends here*/
   WPE_SendToAllMc(sys,1);
   err = WPE_VerifyScenario(sys,5,2,N);
   if (err)
      return err;
   err = WPE_CheckQnodes(sys,mc_qnode_before,iw_qnode_before);
   return err;
}

static WP_U32 WPE_RunS_6(WPE_system *sys)
{
   WP_U32 err;
   WP_U32 mc_qnode_before,iw_qnode_before;

   WP_iw_mc_member mc_member_config[1] =
      {
         {
            /* txfunc */0,
            /* service_class_index */0,
            /* buffers_threshold */0,
            /* iw_port */0,
            /* edit_size */0x20,
            /* data_copy_mode */WP_IW_MC_MEMBER_DATACOPY_DISABLE,
            /*iw_port_check_mode*/WP_IW_MC_MEMBER_IWPORT_CHECK_ENABLE
         }
      };
   status = WP_QNodeStatus(sys->qn_mc,WP_QNODE_STATUS_FIFO,&mc_qnode_before);
   WPE_TerminateOnError(status,"WP_QnodeStatus",__LINE__);
   status = WP_QNodeStatus(sys->qniw,WP_QNODE_STATUS_FIFO,&iw_qnode_before);
   WPE_TerminateOnError(status,"WP_QnodeStatus",__LINE__);

   WPE_BuildDefaultPce(sys);

   mc_member_config->txfunc = sys->flow_host;;

   sys->mc_member_handle[0][0] = WP_IwMcMemberAdd(sys->mc_group_handle[0],
                                                  mc_member_config);
   WPE_TerminateOnError(sys->mc_member_handle[0][0], "WP_IwMcMemberAdd()",__LINE__);

   sys->num_of_members[0]=1;

   /*TBD - Host Sends here*/
   WPE_SendToAllMc(sys,100);
   err = WPE_VerifyScenario(sys,6,1,100);
   if (err)
      return err;
   err = WPE_CheckQnodes(sys,mc_qnode_before,iw_qnode_before);
   return err;
}



/*Bulid system Tx channels, flows and MC groups and members*/
static void WPE_BuildScenario(WPE_system *sys,WP_U32 scenario)
{
   switch(scenario)
   {
      case 1:
         printf("\nScenario 1 - One MC group\n");

         sys->num_tx_on_enet=1;
#if CHECK_FIWT
         sys->num_of_groups=2;
#else
         sys->num_of_groups=1;
#endif
         sys->num_tx_on_upi=0;

         break;
      case 2:
         printf("\nScenario 2 - Adding\\Removing members from MC group + HSPOS as a member\n");

         sys->num_tx_on_enet=1;
         sys->num_of_groups=2;
         sys->num_tx_on_upi=1;

         break;
      case 3:
         printf("\nScenario 3 - Adding\\Removing MC groups\n");

         sys->num_tx_on_enet=1;
         sys->num_of_groups=3;
         sys->num_tx_on_upi=0;

         break;
      case 4:
         printf("\nScenario 4 - Checking copy data\n");

         sys->num_tx_on_enet=8;
         sys->num_of_groups=1;
         sys->num_tx_on_upi=0;

         break;
      case 5:
         printf("\nScenario 5 - Checking Maximal member creation (This may take a while....)\n");

         sys->num_tx_on_enet=8;
         sys->num_of_groups=4096;
         sys->num_tx_on_upi=8;

         break;
      case 6:
         printf("\nScenario 6 - Checking: host termination as member\n");

         sys->num_tx_on_enet=1;
         sys->num_of_groups=1;
         sys->num_tx_on_upi=0;

         break;
      default:break;
   }
   printf("-----------------------------------------------------------\n");
   sys->n_rules = 0;
   WPE_CreateTxChannels(sys);
   WPE_FlowAggsCreate(sys);
   WPE_CreateMCgroups(sys);
   WPE_SystemEnable(sys);
   return;

}


/*******************************************************
 *
 *Delete all TX Channels,all Flow Aggregations, all MC groups and members
 *
 ******************************************************/
static void WPE_DeleteBetweenScenarios(WPE_system *sys)
{
   WP_U32 ii=0,jj=0,timeout=100;
   WP_status status;

   static WP_tx_binding  tx_bind[1]=
      {
         {
            /* res0*/0,
            /* dci_mode;*/  WP_IW_DYN_CH_INSERT_DISABLE,
            /* maxt;*/0,
         }
      };
   /*Delete PCE rules and sets*/
   WPE_DeletePceRules(sys);

/*Delete Tx channels and Flow aggregations on Enet devices*/
   for (ii=0;ii<NUMBER_OF_ENET_PORTS;ii++)
   {

      for (jj=0;jj<sys->num_tx_on_enet;jj++)
      {
         status=WP_IwFlowAggregationDelete(sys->flow_enet[ii][jj]);
         WPE_TerminateOnError(status,"WP_IwFlowAggregationDelete",__LINE__);
         status = WP_IwTxBindingModify(sys->tx_enet_handle[ii][jj],
                                       WP_IW_TX_BIND_MOD_MAXT,
                                       WP_IW_TX_BINDING,
                                       &tx_bind);
         WPE_TerminateOnError(status, "WP_ITxBindingModify channel", __LINE__);
         timeout=100;

         do
         {
            status=WP_ChannelDisable(sys->tx_enet_handle[ii][jj]);
            WPE_TerminateOnError(status,"WP_ChannelDisable",__LINE__);

            WP_Delay(1000);

            status=WP_ChannelDelete(sys->tx_enet_handle[ii][jj]);
            if (WP_ERROR(status)!=WP_ERR_CH_NOT_DISABLED)
               WPE_TerminateOnError(status,"WP_ChannelDelete",__LINE__);

            --timeout;

         }while ((WP_ERROR(status)!=WP_OK)&&(timeout>0));
      }
   }
/*Delete Tx channels and Flow aggregations on UPI devices*/
   for (ii=0;ii<NUMBER_OF_UPI_PORTS;ii++)
   {

      for (jj=0;jj<sys->num_tx_on_upi;jj++)
      {
         timeout=5000;

         status=WP_IwFlowAggregationDelete(sys->flow_upi[ii][jj]);
         WPE_TerminateOnError(status,"WP_IwFlowAggregationDelete",__LINE__);
         status = WP_IwTxBindingModify(sys->tx_upi_handle[ii][jj],
                                       WP_IW_TX_BIND_MOD_MAXT,
                                       WP_IW_TX_BINDING,
                                       &tx_bind);
         WPE_TerminateOnError(status, "WP_ITxBindingModify channel", __LINE__);
         status=WP_ChannelDisable(sys->tx_upi_handle[ii][jj]);
         WPE_TerminateOnError(status,"WP_ChannelDisable UPI",__LINE__);
         WP_Delay(100);
         do
         {

            status=WP_ChannelDelete(sys->tx_upi_handle[ii][jj]);
            if (WP_ERROR(status)!=WP_ERR_CH_NOT_DISABLED)
               WPE_TerminateOnError(status,"WP_ChannelDelete UPI",__LINE__);
            --timeout;

         }while ((WP_ERROR(status)!=WP_OK)&&(timeout>0));


      }
   }

/*Delete all MC members, groups and Flow aggregations*/
   WPE_DeleteAllMC(sys);

   /*Apply changes*/
   status=WP_IwSystemBuild(sys->iw_system);
   WPE_TerminateOnError(status,"WP_IwSystemBuild",__LINE__);

#if USE_SOCKET
   static WP_rx_binding_bridging  rx_binding[1]=
      {
         {
            /*  encap_mode */0,
            /*  mru;*/1536,
            /*  vcfcs;*/0,
            /*  input_port;*/0
         }
      };

   /*Delete Rx channels on Enet devices, Delete Enet devices*/
   for (ii=0;ii<NUMBER_OF_ENET_PORTS;ii++)
   {
      status = WP_IwRxBindingModify(sys->rx_enet_handle[ii],
                                    sys->iw_system,
                                    sys->qniw,
                                    WP_IW_RX_BIND_MOD_DISABLE,
                                    &rx_binding);
      WPE_TerminateOnError(status, "WP_IRxBindingModify", __LINE__);

      timeout=100;
      do
      {
         status=WP_ChannelDisable(sys->rx_enet_handle[ii]);
         WPE_TerminateOnError(status,"WP_ChannelDisable",__LINE__);

         WP_Delay(1000);

         status=WP_ChannelDelete(sys->rx_enet_handle[ii]);
         if (WP_ERROR(status)!=WP_ERR_CH_NOT_DISABLED)
            WPE_TerminateOnError(status,"WP_ChannelDelete",__LINE__);

         --timeout;

      }while ((WP_ERROR(status)!=WP_OK)&&(timeout>0));

      status=WP_IwPortDelete(sys->iw_port_enet[ii]);
      WPE_TerminateOnError(status,"WP_IwPortDelete",__LINE__);
      /*
        status = WP_DeviceDelete(sys->enetdev[ii]);
        WPE_TerminateOnError(status,"WP_DeviceDelete",__LINE__);
      */

   }

/*Delete Rx channels on UPI devices, Delete UPI devices*/
   for (ii=0;ii<NUMBER_OF_UPI_PORTS;ii++)
   {

      timeout=5000;
      status = WP_IwRxBindingModify(sys->rx_upi_handle[ii],
                                    sys->iw_system,
                                    sys->qniw,
                                    WP_IW_RX_BIND_MOD_DISABLE,
                                    &rx_binding);
      WPE_TerminateOnError(status, "WP_IRxBindingModify", __LINE__);

      WP_Delay(100);
      do
      {

         status=WP_ChannelDelete(sys->rx_upi_handle[ii]);
         if (WP_ERROR(status)!=WP_ERR_CH_NOT_DISABLED)
            WPE_TerminateOnError(status,"WP_ChannelDelete UPI",__LINE__);
         --timeout;

      }while ((WP_ERROR(status)!=WP_OK)&&(timeout>0));

      status=WP_IwPortDelete(sys->iw_port_upi[ii]);
      WPE_TerminateOnError(status,"WP_IwPortDelete",__LINE__);

/*     status = WP_DeviceDelete(sys->upidev[ii]);
       WPE_TerminateOnError(status,"WP_DeviceDelete",__LINE__);
*/
   }

   status=WP_ChannelDisable(sys->host_tx);
   WPE_TerminateOnError(status,"WP_ChannelDisable",__LINE__);
   status=WP_ChannelDelete(sys->host_tx);
   WPE_TerminateOnError(status,"WP_ChannelDelete",__LINE__);

   status=WP_ChannelDisable(sys->host_rx);
   WPE_TerminateOnError(status,"WP_ChannelDisable",__LINE__);
   status=WP_ChannelDelete(sys->host_rx);
   WPE_TerminateOnError(status,"WP_ChannelDelete",__LINE__);

   status=WP_IwFlowAggregationDelete(sys->flow_host);
   WPE_TerminateOnError(status,"WP_IwFlowAggregationDelete",__LINE__);

   status=WP_IwPortDelete(sys->iw_port_host);
   WPE_TerminateOnError(status,"WP_IwPortDelete",__LINE__);

   /*Apply changes*/
   status=WP_IwSystemBuild(sys->iw_system);
   WPE_TerminateOnError(status,"WP_IwSystemBuild",__LINE__);

   status=WP_IwSystemDelete(sys->iw_system);
   WPE_TerminateOnError(status,"WP_IwSystemDelete",__LINE__);

#endif
}

static void WPE_S3_MemAdd(WPE_system *sys)
{
   WP_U32 ii,jj,index;

   WP_iw_mc_member mc_member_config[1] =
      {
         {
            /* txfunc */0,
            /* service_class_index */0,
            /* buffers_threshold */0,
            /* iw_port */0,
            /* edit_size */0x20,
            /* data_copy_mode */WP_IW_MC_MEMBER_DATACOPY_DISABLE,
            /*iw_port_check_mode*/WP_IW_MC_MEMBER_IWPORT_CHECK_ENABLE
         }
      };

   /*Add members of Enet ports - i members to the i'th group*/
   for (ii=0;ii<sys->num_of_groups;ii++)
   {
      sys->num_of_members[ii]=0;
      index=0;

      for (jj=1;jj<(ii+2);jj++)
      {
         mc_member_config->iw_port = sys->iw_port_enet[jj];
         mc_member_config->txfunc = sys->flow_enet[jj][0];
         sys->mc_member_handle[ii][index] = WP_IwMcMemberAdd(sys->mc_group_handle[ii],
                                                             mc_member_config);
         WPE_TerminateOnError(sys->mc_member_handle[ii][index], "WP_IwMcMemberAdd()",__LINE__);
         ++index;
      }
      sys->num_of_members[ii]=index;
   }
}


/*Deletes all MC members, groups and Flow aggregations*/
static void WPE_DeleteAllMC(WPE_system *sys)
{
   WP_U32 ii=0,jj=0;
   WP_status status;

   for (ii=0;ii<sys->num_of_groups;ii++)
   {
      for (jj=0;jj<(sys->num_of_members[ii]);jj++)
      {
         status = WP_IwMcMemberDisable(sys->mc_group_handle[ii],sys->mc_member_handle[ii][jj]);
         WPE_TerminateOnError(status,"WP_IwMcMemberDisable",__LINE__);
         status = WP_IwMcMemberDelete(sys->mc_group_handle[ii],sys->mc_member_handle[ii][jj]);
         WPE_TerminateOnError(status,"WP_IwMcMemberDelete",__LINE__);

      }
      status=WP_IwFlowAggregationDelete(sys->flow_mc[ii]);
      WPE_TerminateOnError(status,"WP_IwFlowAggregationDelete",__LINE__);
      status = WP_IwMcGroupDelete(sys->mc_group_handle[ii]);
      WPE_TerminateOnError(status,"WP_IwMcGroupDelete",__LINE__);
   }
}



/*****************************************************************************
 * Function name: WTE_IpChecksumCalc
 * Description  : calculate checksum of IP header
 * Input  params: buff - pointer to buffer, len - size of IP header
 * Output params:
 * Return val   : calculated checksum
 *****************************************************************************/
WP_U16 WTE_IpChecksumCalc(WP_U8 *buff, unsigned short len)
{
   unsigned short *bu16 = (unsigned short*)buff;
   unsigned int sum=0;
   unsigned short i;

   /* make 16 bit words out of every two adjacent
      8 bit words in the packet and add them up */
   len = len/2;
   for (i=0;i<len;i++){
      sum = sum + (WP_U32) bu16[i];
   }

   /* take only 16 bits out of the 32 bit sum
      and add up the carries */
   while (sum>>16)
      sum = (sum & 0xFFFF)+(sum >> 16);

   /* one's complement the result */
   sum = ~sum;
   return ((WP_U16) sum);
}/*WTE_IpChecksumCalc*/


/*This Function Creates Pakcet and sends it from the host*/
static void APP_DataUnitsSetup(WPE_system *sys,WP_handle *send_handle,WP_U32 length,WP_CHAR* src,WP_CHAR* dest,WP_CHAR* sa_ip,WP_CHAR* da_ip,
                               WP_CHAR* tos,WP_CHAR* vlan_tag)
{
   WP_data_segment km_tx_buf;
   WP_U8 *km_data_ptr=NULL;
   WP_status status;
   WP_CHAR ascii_buffer[DATA_LENGTH*2+1];
   WP_U8 hex_buffer[DATA_LENGTH*2+1];
   WP_data_unit data_unit;
   WP_U16 checksum;

   memset (hex_buffer, 0, DATA_LENGTH*2);
   memset (ascii_buffer, 0, DATA_LENGTH*2+1);
   memset( &data_unit, 0, sizeof( data_unit));

   /* Host Send  */
   data_unit.data_size = length;
   data_unit.n_active = 1;

   km_tx_buf.data_size = data_unit.data_size;
   km_data_ptr = WP_PoolAlloc(sys->data_pool_handle);
   km_tx_buf.data = km_data_ptr;
   km_tx_buf.pool_handle = sys->data_pool_handle;
   km_tx_buf.next = NULL;
   km_tx_buf.displacement = 0;

   data_unit.segment = (&km_tx_buf);
   data_unit.n_segments = 1;

   /*Check if we send_handle is direct to link*/
   if (*send_handle==sys->host_tx)
      data_unit.type = WP_DATA_IW;
   else
      data_unit.type = WP_DATA_ENET;

   data_unit.control =0;/*WP_HT_IW_B_MODE_FIELD(WP_HT_IW_B_NORMAL_MODE)|
                          WP_HT_IW_CRC_FIELD(0);;*/
   /*Only for bridginig Systems :*/
   strcpy (ascii_buffer, dest);
   strcat (ascii_buffer, src);
   if (strlen(vlan_tag))
   {
      strcat (ascii_buffer,"8100");
      strcat (ascii_buffer,vlan_tag);
   }
   strcat (ascii_buffer,"0800");
   strcat (ascii_buffer,"45");
   strcat (ascii_buffer,tos);
   strcat (ascii_buffer,"004c");
   strcat (ascii_buffer,"001ba57d");
   strcat (ascii_buffer,"05110000");/* protocol = 0x11*/
   strcat (ascii_buffer,sa_ip);/* sa ip */
   strcat (ascii_buffer,da_ip);/* Da ip */
   strcat (ascii_buffer,"00080003");
   strcat (ascii_buffer,"001b77c0");
   strcat (ascii_buffer,
           "00a4c5ff001b77c43a528a3f001b77c8eeffeeff0000000000000000000000000000000000000123");
   strcat (ascii_buffer,
           "111111112222222233333333444444445555555566666666777777778888888899999999aaaaaaaa");
   strcat (ascii_buffer,
           "0101010102020202030303030404040405050505060606060707070708080808090909090a0a0a0a");
   strcat (ascii_buffer,
           "111111112222222233333333444444445555555566666666777777778888888899999999aaaaaaaa");
   strcat (ascii_buffer,
           "0101010102020202030303030404040405050505060606060707070708080808090909090a0a0a0a");
   strcat (ascii_buffer,
           "111111112222222233333333444444445555555566666666777777778888888899999999aaaaaaaa");
   strcat (ascii_buffer,
           "0101010102020202030303030404040405050505060606060707070708080808090909090a0a0a0a");
   strcat (ascii_buffer,
           "111111112222222233333333444444445555555566666666777777778888888899999999aaaaaaaa");
   strcat (ascii_buffer,
           "0101010102020202030303030404040405050505060606060707070708080808090909090a0a0a0a");
   strcat (ascii_buffer,
           "111111112222222233333333444444445555555566666666777777778888888899999999aaaaaaaa");
   strcat (ascii_buffer,
           "0101010102020202030303030404040405050505060606060707070708080808090909090a0a0a0a");
   strcat (ascii_buffer,
           "111111112222222233333333444444445555555566666666777777778888888899999999aaaaaaaa");
   strcat (ascii_buffer,
           "0101010102020202030303030404040405050505060606060707070708080808090909090a0a0a0a");

   App_TranslateAsciiToHex((WP_CHAR*)hex_buffer, ascii_buffer, (WP_U32)data_unit.data_size);

   checksum = WTE_IpChecksumCalc(hex_buffer + 18, 20);
   memcpy (hex_buffer + 28, &checksum, 2);

#if DEBUG_DATA

   for (ii  = 0; ii < data_unit.data_size;ii++)
   {
      printf("%02x",hex_buffer[ii]);
      if (ii%4 == 3)
         printf("\n");

   }
   printf("\n");
#endif

   if (km_data_ptr!=NULL)
   {
      WP_MEM_BYTES_FILL(km_data_ptr,0,APP_HOST_BUFFER_SIZE);
      WP_MEM_BYTES_SET(km_data_ptr,hex_buffer,DATA_LENGTH);

      status = WP_HostSend(*send_handle, &data_unit);
      WPE_TerminateOnError(status, "WP_HostSend()",__LINE__);
      //   getchar();
      WP_Delay(1000);
      WPE_HostRecive(sys,km_data_ptr);
   }
   else printf("PoolAlloc=NULL\n");

   return;
}/*APP_DataUnitsSetup*/

static WP_U32 WPE_HostRecive(WPE_system *sys,WP_U8 *sent_buffer)
{
   WP_status status= WP_OK;
   WP_data_unit data_unit;
   WP_data_segment tx_buf;
   WP_data_segment *curr_buff_ptr=NULL;
   WP_U32 err=0,kk,jj;
   WP_U8 hexa_buffer[DATA_LENGTH*2];
   WP_U32 num_of_received_packets = 0;

   memset( &data_unit, 0, sizeof( data_unit));
   memset(&tx_buf, 0, sizeof( tx_buf));
   data_unit.type = WP_DATA_IW;
   data_unit.data_size = DATA_LENGTH;
   data_unit.n_active = 1;
   tx_buf.data_size = data_unit.data_size;
   tx_buf.next = NULL;
   tx_buf.displacement = 0;

   data_unit.segment = (&tx_buf);
   data_unit.n_segments = 1;
   data_unit.n_active = 0;
   tx_buf.data = NULL;

   while (WP_ERROR(status) != WP_ERR_HST_NO_DATA_TO_GET)
   {
      status = WP_HostReceive(sys->host_rx, &data_unit);

      if (WP_ERROR(status) == WP_ERR_HST_NO_DATA_TO_GET) /* didn't get yet? */
      {
         WP_Delay(5000);
         status = WP_HostReceive(sys->host_rx, &data_unit);
      }
      if (WP_ERROR(status) != WP_ERR_HST_NO_DATA_TO_GET)
      {
         WPE_TerminateOnError(status,"WP_HostReceive",__LINE__);
         num_of_received_packets++;
         if (data_unit.status & (WP_HT_HDLC_SOP_EOP_ERROR     |
                                 WP_HT_HDLC_PARITY_ERROR      |
                                 WP_HT_HDLC_RX_ERROR          |
                                 WP_HT_HDLC_NON_ALIGNED       |
                                 WP_HT_HDLC_MAX_LENGTH_ERROR  |
                                 WP_HT_HDLC_CRC_ERROR         |
                                 WP_HT_HDLC_POS_OVERRUN))
         {
            err = WPE_ERR;

            if (data_unit.status & WP_HT_HDLC_SOP_EOP_ERROR) {
               printf("Test Abort: Receive SOP_EOP error\n");

            }
            if (data_unit.status & WP_HT_HDLC_PARITY_ERROR) {
               printf("Test Abort: Receive PARITY error\n");

            }
            if (data_unit.status & WP_HT_HDLC_RX_ERROR) {
               printf ("Test Abort: RECEIVE RX Error\n");

            }
            if (data_unit.status & WP_HT_HDLC_NON_ALIGNED) {
               printf ("Test Abort: Receive Non-Aligned Error\n");

            }
            if (data_unit.status & WP_HT_HDLC_MAX_LENGTH_ERROR) {
               /* only abort if it should work */

               printf ("Test Abort: Receive Max Length error\n");


            }
            if (data_unit.status & WP_HT_HDLC_CRC_ERROR) {
               printf ("Test Abort: Receive CRC error\n");

            }
            if (data_unit.status & WP_HT_HDLC_POS_OVERRUN) {
               printf ("Test Abort: Receive POS overrun\n");

            }
            return err;
         }
         else
         {
            curr_buff_ptr = data_unit.segment;
            WP_MEM_BYTES_GET(((WP_U8 *)hexa_buffer),curr_buff_ptr->data,curr_buff_ptr->data_size);
#if DEBUG_DATA

            printf("curr_buff_ptr->data_size = 0x%x\n",curr_buff_ptr->data_size);
            for (ii  = 0; ii < curr_buff_ptr->data_size;ii++)
            {
               printf("%02x",hexa_buffer[ii]);
               if (ii%4 == 3)
                  printf("\n");

            }
            printf("\n");
#endif



//      printf("\n\nnum of active is %d\n\n",data_unit.n_active);

            for (jj = 0; jj < data_unit.n_active; ++jj)
            {
               for(kk=0;kk<curr_buff_ptr->data_size-4;++kk)
               {
                  if ((WP_U8) sent_buffer[kk] != (WP_U8)(hexa_buffer[kk]))
                  {
                     printf("Error - sent packet ,part %d,is 0x%02x  != recived packet, part %d, is 0x%02x\n",
                            kk,(WP_U8)sent_buffer[kk],kk,(WP_U8)hexa_buffer[kk]);
                     err = WPE_ERR;
                     printf("Test Failed");
                     WPE_Terminate(err);
                  }
               }
               WP_PoolFree(curr_buff_ptr->pool_handle, curr_buff_ptr->data);
               curr_buff_ptr = curr_buff_ptr->next;
            }
         }
      }
   }
   if (num_of_received_packets == 0)
   {
      err = WPE_ERR;
   }
   return err;
}/*WPE_HostRecive*/


/*******************************************************
 *
 * Add MC members to MC groups according to the current scenario
 *
 *******************************************************/
static void WPE_AddMCmembers(WPE_system *sys)
{
   WP_U32 ii,jj,port,index=0;
   WP_U32 print_count = 0;

   WP_iw_mc_member mc_member_config[1] =
      {
         {
            /* txfunc */0,
            /* service_class_index */0,
            /* buffers_threshold */0,
            /* iw_port */0,
            /* edit_size */0x20,
            /* data_copy_mode */WP_IW_MC_MEMBER_DATACOPY_DISABLE,
            /*iw_port_check_mode*/WP_IW_MC_MEMBER_IWPORT_CHECK_ENABLE
         }
      };

   /*Add members of Enet and HSPOS ports*/
   for (ii=0;ii<sys->num_of_groups;ii++)
   {
#if TEST_DEBUG
      printf("Created %d members\n",total);
#endif
      index=sys->num_of_members[ii];
      for (port=1;port<NUMBER_OF_ENET_PORTS;port++)
      {

         mc_member_config->iw_port = sys->iw_port_enet[port];
         for (jj=0;jj<sys->num_tx_on_enet;jj++)
         {
            mc_member_config->txfunc = sys->flow_enet[port][jj];

            sys->mc_member_handle[ii][index] = WP_IwMcMemberAdd(sys->mc_group_handle[ii],
                                                                mc_member_config);
            WPE_TerminateOnError(sys->mc_member_handle[ii][index], "WP_IwMcMemberAdd()",__LINE__);
            ++index;
            ++total;
            ++print_count;
            if (print_count>=12000)
            {
               printf("  - %d members already created\n",total);
               print_count = 0;
            }
         }
      }
      for (port=0;port<NUMBER_OF_UPI_PORTS;port++)
      {

         mc_member_config->iw_port = sys->iw_port_upi[port];
         for (jj=0;jj<sys->num_tx_on_upi;jj++)
         {
            mc_member_config->txfunc = sys->flow_upi[port][jj];

            sys->mc_member_handle[ii][index] = WP_IwMcMemberAdd(sys->mc_group_handle[ii],
                                                                mc_member_config);
            WPE_TerminateOnError(sys->mc_member_handle[ii][index], "WP_IwMcMemberAdd()",__LINE__);
            ++index;
            ++total;
            ++print_count;
            if (print_count>=12000)
            {
               printf("  - %d members already created\n",total);
               print_count = 0;
            }
         }
      }
      sys->num_of_members[ii]=index;

   }
}


/*******************************************************
 *
 * Create MC groups according to the current scenario
 * and add a MC flow aggregation directing to them
 *
 *******************************************************/
static void WPE_CreateMCgroups(WPE_system *sys)
{
   WP_U32 ii;

   WP_iw_generic_agg_multicast mc_agg_config[1] =
      {
         {
            /* tag */0,
            /* type */WP_IW_FLOW_AGG_PRIMARY,
            /* mc_group */0,
            /* mtu */1536,
            /* intmode */WP_IW_INT_DISABLE,
            /* interruptqueue */WP_IW_IRQT1,
            /* statmode */WP_IW_STAT_ENABLE,
            /* timestamp_mode */WP_IW_TIME_STAMP_DISABLE,
            /* ov_pool_mode */WP_IW_OV_POOL_ENABLE,
            /* fbp_drop_threshold */0,
            /* policer_enable */WP_IW_POLICER_DISABLE,
            /* *policer_config */NULL,
         }
      };

   WP_iw_mc_group mc_group_config[1] =
      {
         {
            /* type */WP_IW_MC_UNRESTRICTED,
            /* mode */WP_IW_MC_MODE_STAT_ENABLE | WP_IW_MC_MODE_MEMBERS_STAT_ENABLE | WP_IW_MC_MODE_GENERIC_GROUP,
            /* max_iw_mc_members */300,
            /* qnode */0,
            /* encaps_gap */64,
            /* classes_config */NULL,
            /* identifier */WP_UNUSED
         }
      };

   mc_group_config->qnode = sys->qn_mc;

   for (ii=0;ii<sys->num_of_groups;ii++)
   {

      sys->mc_group_handle[ii] = WP_IwMcGroupCreate(WP_WINPATH(0),
                                                    mc_group_config);
      WPE_TerminateOnError( sys->mc_group_handle[ii], "WP_IwMcGroupCreate()",__LINE__);

      mc_agg_config->mc_group = sys->mc_group_handle[ii];
      sys->flow_mc[ii] = WP_IwFlowAggregationCreate(WP_WINPATH(0),
                                                    WP_IW_GENERIC_MC_MODE,
                                                    mc_agg_config);
//      printf("mc flow agg %x\n", sys->flow_mc[ii]);

      WPE_TerminateOnError(sys->flow_mc[ii], "WP_IwFlowAggregationCreate()",__LINE__);
      sys->num_of_members[ii]=0;
   }
}


/***********************************************
 *Enable System Ports, Channels and Devices
 *
 ***********************************************/
static void WPE_SystemEnable(WPE_system *sys)
{
   WP_status status;
   WP_U32 ii,jj;

   for (ii=0;ii<NUMBER_OF_ENET_PORTS;ii++)
   {
      status = WP_PortEnable(sys->enet_port_handle[ii], WP_DIRECTION_DUPLEX);
      WPE_TerminateOnError(status, "WP_PortEnable() Enet",__LINE__);
      status = WP_DeviceEnable(sys->enetdev[ii], WP_DIRECTION_DUPLEX);
      WPE_TerminateOnError(status, "WP_DeviceEnable() ENET",__LINE__);
   }
   for (ii=0;ii<NUMBER_OF_UPI_PORTS;ii++)
   {
      status = WP_PortEnable(sys->upi_port_handle[ii], WP_DIRECTION_DUPLEX);
      WPE_TerminateOnError(status, "WP_PortEnable() Enet",__LINE__);
      status = WP_DeviceEnable(sys->upidev[ii], WP_DIRECTION_DUPLEX);
      WPE_TerminateOnError(status, "WP_DeviceEnable() ENET",__LINE__);
   }


   for (ii=0;ii<NUMBER_OF_ENET_PORTS;ii++)
   {
      status = WP_ChannelEnable(sys->rx_enet_handle[ii]);
      WPE_TerminateOnError(status, "WP_ChannelEnable() ENET_RX",__LINE__);
      for (jj=0;jj<sys->num_tx_on_enet;jj++)
      {
         status = WP_ChannelEnable(sys->tx_enet_handle[ii][jj]);
         WPE_TerminateOnError(status, "WP_ChannelEnable() ENET_TX",__LINE__);
      }
   }
#if UPI_MIXED_SHAPING
   for (ii=0; ii<NUMBER_OF_UPI_PORTS; ii++)
   {
      status = WP_ShapingGroupEnable(sys->l1_group[ii]);
      WPE_TerminateOnError(status, "WP_ShapingGroupEnable()",__LINE__);
      for (jj=0;jj<SHAPING_BLOCK_SIZE;jj++)
      {
         status = WP_ShapingGroupEnable(sys->l2_group[ii][jj]);
         WPE_TerminateOnError(status, "WP_ShapingGroupEnable()",__LINE__);
      }
      for (jj=0;jj<sys->num_tx_on_upi;jj++)
      {
         status = WP_ChannelEnable(sys->tx_upi_handle[ii][jj]);
         WPE_TerminateOnError(status, "WP_ChannelEnable() UPI_TX",__LINE__);
      }
   }
#endif

   status = WP_ChannelEnable(sys->host_rx);
   WPE_TerminateOnError(status, "WP_ChannelEnable() HOST_RX",__LINE__);
   status = WP_ChannelEnable(sys->host_tx);
   WPE_TerminateOnError(status, "WP_ChannelEnable() HOST_TX",__LINE__);
}


/*****************************************************
 *
 * Create IwPorts
 **************************************************/
static void WPE_CreateBridgingPorts(WPE_system *sys)
{
   WP_U32 ii=0;
   static WP_bridge_port bport_conf[1] =
      {
         {
            /* tag */ 0,
            /* direct_mapping*/WP_IW_DIRECT_MAP_ENABLE,
            /* flow_agg */0,
            /* termination_mode*/WP_IW_HOST_TERM_MODE,
            /* learning_mode */WP_IW_LEARNING_DISABLE,
            /* in_filter_mode */WP_IW_INGRESS_FILTER_DISABLE,
            /* vlan_param */
            {
               /* vlan_acceptance_mode */WP_IW_ACCEPT_TAGGED_UNTAGGED,
               /* vlan_tag */0,
            },
            /* max_mac_addresses */ 20,
            /* group_tag */ 0,
            /*group_filtering_mode*/WP_IW_GROUP_FILTER_DISABLE,
            /*unk_mac_sa_filter; */  WP_IW_UNK_MACSA_FILTER_DISABLE,
            /* unk_mc_mode;*/   WP_IW_UNK_MC_DROP,
            /* bc_ht_mode;*/   WP_IW_BC_HT_DISABLE,
            /* input_filters_mask;*/   0,
            /* output_filters_mask;*/0,
            /* statmode;*/   WP_IW_PORT_STAT_ENABLE,
            /* unk_uc_mode*/  WP_IW_UNK_UC_SR_ENABLE,
            /* Enable/Disable Bridge Classifier per (input) bridge port  */ WP_IW_BPORT_CLASSIFICATION_ENABLED,
            /*adv_unk_lookup_mode*/      WP_IW_ADV_UNK_LOOKUP_ENABLED,
            /*cfi_ht_mode*/              WP_IW_CFI_HT_DISABLE,
            /*reserved_mc_ht_mode*/      WP_IW_RES_MC_HT_DISABLE,
            /*predefined_ht_mode*/       WP_DISABLE, /*WP_IW_ARP_HT_ENABLE | WP_IW_IGMP_HT_ENABLE*/


         }
      };


   bport_conf[0].flow_agg = sys->flow_host;

   /*Create IwPorts for Enet devices */
   for (ii=0;ii<NUMBER_OF_ENET_PORTS;ii++)
   {
      bport_conf[0].tag=ii;
      sys->iw_port_enet[ii] = WP_IwPortCreate(sys->iw_system,  bport_conf);
      WPE_TerminateOnError(sys->iw_port_enet[ii],"WP_IwPortCreate()",__LINE__);
   }

   /*Create IwPorts over UPI's*/
   for (ii=0;ii<NUMBER_OF_UPI_PORTS;ii++)
   {
      bport_conf[0].tag=ii+NUMBER_OF_ENET_PORTS;
      sys->iw_port_upi[ii] = WP_IwPortCreate(sys->iw_system,  bport_conf);
      WPE_TerminateOnError(sys->iw_port_upi[ii],"WP_IwPortCreate()",__LINE__);
   }


}

/**************************************************
 *
 * Creates flow aggregation per Tx channel created
 * for UPI and Enet both.
 *
 *************************************************/
static void WPE_FlowAggsCreate(WPE_system *system)
{
   WP_U32 ii,jj;
   WP_status   status;
   static WP_tx_binding  tx_bind[1]=
      {
         {
            /* res0*/0,
            /* dci_mode;*/  WP_IW_DYN_CH_INSERT_ENABLE,
            /* maxt;*/255,
         }
      };
   static WP_iw_agg_generic tx_agg_gen[1] =
      {
         {
            /*tag*/ 1,
            /*txfunc*/ 0,
            /*iw_port*/0,
            /*rfcs*/WP_IW_RFCS_DISABLE,
            /*interruptqueue;*/WP_IW_IRQT1,
            /*error_pkt_mode*/WP_IW_ERRPKT_DISCARD,
            /*intmode;*/WP_IW_INT_DISABLE,
            /*statmode;*/WP_IW_STAT_ENABLE,
            /*timestamp_mode;*/WP_IW_TIME_STAMP_DISABLE,
            /*mtu;*/1536,
            /*flow_agg_type*/ WP_IW_AGG_PRIMARY,
            /*policer_handle;*/0,
            /*pecs_handle;*/0,
            /*pecs_user_info;*/0

         }
      };

/*Create Enet Flow aggregations*/
   for (ii=0;ii<NUMBER_OF_ENET_PORTS;ii++)
   {

      for (jj=0;jj<system->num_tx_on_enet;jj++)
      {

         tx_agg_gen[0].iw_port = system->iw_port_enet[ii];
         tx_agg_gen[0].txfunc = system->tx_enet_handle[ii][jj];

         system->flow_enet[ii][jj]
            = WP_IwFlowAggregationCreate(system->wpid,
                                         WP_IW_GENERIC_MODE,//WP_IW_VLAN_AWARE_BRIDGING_MODE,
                                         &tx_agg_gen);
         WPE_TerminateOnError(system->flow_enet[ii][jj],"WP_IwFlowAggregation on ENET",__LINE__);
         /*Do the bindings*/
         status=WP_IwTxBindingCreate(system->tx_enet_handle[ii][jj],WP_IW_TX_BINDING,&tx_bind);
         WPE_TerminateOnError(status,"WP_IwTxBindingCreate",__LINE__);
      }
   }
/*Create UPI flow aggregations*/
   /*   tx_agg[0].prefix_length=2;
        tx_agg[0].l2_header_insert_mode=WP_IW_L2H_INSERT_ENABLE;
   */
   for (ii=0;ii<NUMBER_OF_UPI_PORTS;ii++)
   {
      for (jj=0;jj<system->num_tx_on_upi;jj++)
      {
         tx_agg_gen[0].iw_port = system->iw_port_upi[ii];
#if UPI_MIXED_SHAPING
         tx_agg_gen[0].txfunc = system->tx_upi_handle[ii][jj];
#else
         //tx_agg_gen[0].txfunc = system->upidev[ii];
#endif
         system->flow_upi[ii][jj]
            = WP_IwFlowAggregationCreate(system->wpid,
                                         WP_IW_GENERIC_MODE,//WP_IW_VLAN_AWARE_BRIDGING_MODE,
                                         &tx_agg_gen);
         WPE_TerminateOnError(system->flow_upi[ii][jj],"WP_IwFlowAggregationCreate on UPI",__LINE__);
         /*Do the bindings*/
#if UPI_MIXED_SHAPING
         status=WP_IwTxBindingCreate(system->tx_upi_handle[ii][jj],WP_IW_TX_BINDING,&tx_bind);
         WPE_TerminateOnError(status,"WP_IwTxBindingCreate",__LINE__);
#endif
      }
   }
}

/***************************************************************************
 *
 * Create Tx channels Enet & UPI devices, according to the current scenario
 *
 **************************************************************************/
static void WPE_CreateTxChannels(WPE_system *sys)
{
   WP_U32 ii,jj,last_tag;

   /* Channel configuration for Ethernet */
   static WP_ch_enet ch_config_enet[1] =
      {
         {
            /* intmode */ WP_PKTCH_INT_DISABLE,
            /* iwmmode */  WP_PKTCH_IWM_ENABLE,
            /* testmode */ WP_PKTCH_TEST_DISABLE,
            /* tx_pqblock */ 0,
            /* tx_pqlevel */ 0,
            /* tx_shaping_type */ WP_FMU_SHAPING_TYPE_STRICT,
            /* tx_shaping_params */ NULL,
            /* rx_maxsdu */ 1536,
            /* tx_cwid */ WP_CW_ID_A,
            /* tx_tq */ 0,
         }
      };
   static WP_ch_hdlc ch_config_hdlc[1] =
      {
         {
            /* intmode */ WP_PKTCH_INT_DISABLE,
            /* iwmmode */  WP_PKTCH_IWM_ENABLE,
            /* testmode */ WP_PKTCH_TEST_DISABLE,
            /* tx_pqblock */ 0,
            /* tx_pqlevel */ 0,
            /* tx_shaping_type */ WP_FMU_SHAPING_TYPE_STRICT,
            /* tx_shaping_params */ NULL,
            /* rx_maxsdu */ 1536,
            /* tx_cwid */ WP_CW_ID_A,
            /* tx_tq */ 0,
         }
      };


   last_tag=sys->last_tag;

   /* system  ENET Tx channels */

   for (ii=0;ii<NUMBER_OF_ENET_PORTS;ii++)
   {
      for (jj=0;jj<sys->num_tx_on_enet;jj++)
      {
         ch_config_enet[0].tx_tq=jj;
         sys->tx_enet_handle[ii][jj] = WP_ChannelCreate(last_tag+1+ii*MAX_ENET_TX+jj,
                                                        sys->enetdev[ii],
                                                        sys->qniw,
                                                        WP_CH_TX,
                                                        WP_ENET,
                                                        &ch_config_enet[0]);
         WPE_TerminateOnError(sys->tx_enet_handle[ii][jj], "WP_Channel_Create() Enet TX",__LINE__);
      }
   }

   last_tag=last_tag+(NUMBER_OF_ENET_PORTS-1)*MAX_ENET_TX+MAX_ENET_TX;
#if UPI_MIXED_SHAPING
   for (ii=0;ii<NUMBER_OF_UPI_PORTS;ii++)
   {
      for (jj=0;jj<sys->num_tx_on_upi;jj++)
      {
         ch_config_hdlc[0].tx_tq=jj;
         sys->tx_upi_handle[ii][jj] = WP_ChannelCreate(last_tag+1+ii*MAX_UPI_TX+jj,
#if UPI_MIXED_SHAPING
                                                       sys->l2_group[ii][jj],
#else
                                                       sys->upidev[ii],
#endif
                                                       sys->qniw,
                                                       WP_CH_TX,
                                                       WP_HDLC,
                                                       &ch_config_hdlc[0]);
         WPE_TerminateOnError(sys->tx_upi_handle[ii][jj], "WP_Channel_Create() UPI TX",__LINE__);
      }
   }

   last_tag=last_tag+(NUMBER_OF_UPI_PORTS-1)*MAX_UPI_TX+MAX_UPI_TX;
#endif


   sys->last_tag=last_tag;
}

/*Apply defualt setup for all scenarios in this test*/
static void WPE_DefaultSetup(WPE_system *sys)
{
   WPE_CreateRxChannels(sys);
   WPE_IWcreate(sys);
   WPE_CreateHostTxAndIWport(sys);
   WPE_CreateBridgingPorts(sys);
#if UPI_MIXED_SHAPING
   WPE_CreateShapingGroups(sys);
#endif
   WPE_RxBindings(sys);
   WPE_PceFiltersCreate(sys);
   WP_PceFilterStatisticsReset (sys->filter[0], &(pce_filter_stats[0]));
   WPE_CreateFilterSets(sys);
   WPE_PceInterfaces(sys);
}

/************************************************
 * Make Rx bindings
 ***********************************************/
static void WPE_RxBindings(WPE_system *sys)
{
   WP_U32 ii;
   WP_status status;

   static WP_rx_binding_bridging  rx_binding[1]=
      {
         {
            /*  encap_mode */0,
            /*  mru;*/1536,
            /*  vcfcs;*/0,
            /*  input_port;*/0
         }
      };
   for (ii=0;ii<NUMBER_OF_ENET_PORTS;ii++)
   {

      rx_binding[0].input_port = sys->iw_port_enet[ii];

      status=WP_IwRxBindingCreate(sys->rx_enet_handle[ii],sys->iw_system,sys->qniw,&rx_binding[0]);
      WPE_TerminateOnError(status,"WP_IwRxBindingCreate",__LINE__);
   }

   for (ii=0;ii<NUMBER_OF_UPI_PORTS;ii++)
   {

      rx_binding[0].input_port = sys->iw_port_upi[ii];

      status=WP_IwRxBindingCreate(sys->rx_upi_handle[ii],sys->iw_system,sys->qniw,&rx_binding[0]);
      WPE_TerminateOnError(status,"WP_IwRxBindingCreate",__LINE__);
   }


}

/****************************************************
 *
 * Create an Iw port and Tx channel for the host
 *
 ***************************************************/
static void WPE_CreateHostTxAndIWport(WPE_system *sys)
{
   static WP_bridge_port bport_conf[1] =
      {
         {
            /* tag */ 0,
            /* direct_mapping*/  WP_IW_DIRECT_MAP_DISABLE,
            /* flow_agg */0,
            /* termination_mode*/WP_IW_HOST_TERM_MODE,
            /* learning_mode */WP_IW_LEARNING_DISABLE,
            /* in_filter_mode */WP_IW_INGRESS_FILTER_DISABLE,
            /* vlan_param */
            {
               /* vlan_acceptance_mode */WP_IW_ACCEPT_TAGGED_UNTAGGED,
               /* vlan_tag */0,
            },
            /* max_mac_addresses */ 20,
            /* group_tag */ 0,
            /*group_filtering_mode*/WP_IW_GROUP_FILTER_DISABLE,
            /* unk_mac_sa_filter; */  WP_IW_UNK_MACSA_FILTER_DISABLE,
            /* unk_mc_mode;*/   WP_IW_UNK_MC_DROP,
            /* bc_ht_mode;*/   WP_IW_BC_HT_DISABLE,
            /* input_filters_mask;*/   0,
            /* output_filters_mask;*/0,
            /* statmode;*/   WP_IW_PORT_STAT_ENABLE,
         }
      };
   WP_ch_iw_tx ch_iw_tx_config[1] =
      {
         {
            /* iw_system */ 0,
            /* reserved */ 0,
            /* group_id */ WP_IW_CH_GROUP_ID_NULL,
            /* reserved2 */ 0,
            /* input_port */ 0,
         }
      };

   bport_conf[0].flow_agg =  sys->flow_host;
   bport_conf[0].tag=NUMBER_OF_ENET_PORTS+NUMBER_OF_UPI_PORTS;
   sys->iw_port_host = WP_IwPortCreate(sys->iw_system,  bport_conf);
   WPE_TerminateOnError(sys->iw_port_host, "WP_IwPortCreate() HOST ",__LINE__);


   ch_iw_tx_config[0].input_port=sys->iw_port_host;
   ch_iw_tx_config[0].iw_system=sys->iw_system;
   sys->host_tx = WP_ChannelCreate(0x2,
                                   sys->devhost,
                                   sys->qniw,
                                   WP_CH_TX,
                                   WP_IW_HOST,
                                   &ch_iw_tx_config[0]);
   WPE_TerminateOnError(sys->host_tx, "WP_Channel_Create() HOST TX",__LINE__);


}


/*************************************************************
 *
 *  Creats 1 Rx channel per device
 *
 ************************************************************/
static void WPE_CreateRxChannels(WPE_system *sys)
{
   WP_U32 ii,last_tag;

   static WP_tx_binding  tx_binding_iw[1]=
      {
         {
            /* res0;*/ 0,
            /* dci_mode;*/  WP_IW_DYN_CH_INSERT_DISABLE,
            /* maxt;*/20,
         }
      };

   static WP_ch_iw_rx ch_config_iw[1]=
      {
         {
            /* pqblock */ 0,
            /* pqlevel */ 0,
            /* tx_binding_type */ WP_IW_TX_BINDING,
            /* tx_binding_config */ tx_binding_iw
         }
      };

/* Channel configuration for Ethernet */
   static WP_ch_enet ch_config_enet[1] =
      {
         {
            /* intmode */ WP_PKTCH_INT_DISABLE,
            /* iwmmode */  WP_PKTCH_IWM_ENABLE,
            /* testmode */ WP_PKTCH_TEST_DISABLE,
            /* tx_pqblock */ 0,
            /* tx_pqlevel */ 0,
            /* tx_shaping_type */ WP_FMU_SHAPING_TYPE_STRICT,
            /* tx_shaping_params */ NULL,
            /* rx_maxsdu */ 1536,
            /* tx_cwid */ WP_CW_ID_A,
            /* tx_tq */ 0,
         }
      };
   static WP_ch_hdlc ch_config_hdlc[1] =
      {
         {
            /*intmode*/WP_PKTCH_INT_DISABLE,
            /*iwmode*/WP_PKTCH_IWM_ENABLE,
            /*testmode*/WP_PKTCH_TEST_DISABLE,
            /*tx_pqblock*/0,
            /*tx_pqlevel*/0,
            /*tx_shaping_type*/ WP_FMU_SHAPING_TYPE_STRICT,
            /*tx_shaping_params*/NULL,
            /* rx_maxsdu*/1536,
            /*tx_cwid*/WP_CW_ID_A,
            /*tx_tq*/0,
         }
      };


   /* system  ENET Rx channels */
   for(ii = 0; ii <NUMBER_OF_ENET_PORTS; ii++)
   {
      last_tag=ii+NUMBER_OF_ENET_PORTS+NUMBER_OF_UPI_PORTS+1;
      sys->rx_enet_handle[ii] = WP_ChannelCreate(last_tag,
                                                 sys->enetdev[ii],
                                                 sys->qniw,
                                                 WP_CH_RX,
                                                 WP_ENET,
                                                 &ch_config_enet[0]);
      WPE_TerminateOnError(sys->rx_enet_handle[ii], "WP_Channel_Create() Enet RX",__LINE__);
   }
   /* system UPI Rx channels */
   for(ii = 0; ii <NUMBER_OF_UPI_PORTS; ii++)
   {
      sys->rx_upi_handle[ii] = WP_ChannelCreate(last_tag+1+ii,
                                                sys->upidev[ii],
                                                sys->qniw,
                                                WP_CH_RX,
                                                WP_HDLC,
                                                &ch_config_hdlc[0]);
      WPE_TerminateOnError(sys->rx_upi_handle[ii], "WP_Channel_Create() UPI RX",__LINE__);
   }
   last_tag=last_tag+NUMBER_OF_UPI_PORTS;

   sys->last_tag=last_tag;
   /* Create HOST  Rx channel */
   sys->host_rx = WP_ChannelCreate(0x1,
                                   sys->devhost,
                                   sys->qniw,
                                   WP_CH_RX,
                                   WP_IW_HOST,
                                   &ch_config_iw[0]);
   WPE_TerminateOnError(sys->host_rx, "WP_ChannelCreate() Host sys1",__LINE__);

}





/***************************************************
 *
 *  Create IW system
 **************************************************/

static void WPE_IWcreate(WPE_system *sys)
{

   static  WP_dfc_sys_info dfc_sys_info[1] =
      {
         {
            /* classification_hash_size */ WP_IW_32_HASH_ENTRIES,
            /* port_filtering */           WP_IW_PORT_FILTERING_DISABLE,
            /* deny_eq_ip */               WP_IW_DENY_EQUAL_IP_DISABLE
         }
      };
   static WP_iw_sys_bridging iw_sys_one[1]=
      {
         {
            /* tag */                0,
            /* max_flows; */SYSTEM_MAX_FLOWS,
            /* classification_mode */WP_IW_NO_CLASSIFIER,
            /* classifier_config */ {0,0,0,NULL},
            /* learning_mode;*/WP_IW_BRIDGE_LEARNING_DISABLE,
            /* learning_queue */
            {
               /*int_queue_num*/WP_IW_IRQT1,
               /*learning_queue_size*/LEARNING_QUEUE_SIZE,
               /*interrupt_enbale*/WP_LEARNING_INTENABLE,
               /*interrupt_rate*/1
            },
            /*forwarding_table_size*/WP_IW_32_HASH_ENTRIES,
            /*member_set_size*/WP_IW_32_HASH_ENTRIES,
            /*buffer_gap;*/0x40,
            /*max_bridging_ports*/MAX_BPORTS_SYS,
            /*dfc_info*/       &dfc_sys_info[0]
         }
      };

   static WP_iw_agg_bridging flooding_tx[1] =
      {
         {
            /*tag*/ 2,
            /*txfunc*/ 0,
            /*output_bport*/0,
            /*rfcs*/WP_IW_RFCS_ENABLE,
            /*l2_header_insert_mode;*/WP_IW_L2H_INSERT_DISABLE,
            /*vlan_tag_mode*/WP_IW_VLAN_TAG_UNCHANGED,
            /*interruptqueue;*/WP_IW_IRQT1,
            /*error_pkt_mode*/WP_IW_ERRPKT_DISCARD,
            /*intmode;*/WP_IW_INT_DISABLE,
            /*statmode;*/WP_IW_STAT_ENABLE,
            /*timestamp_mode;*/WP_IW_TIME_STAMP_DISABLE,
            /*ov_pool_mode */ WP_IW_OV_POOL_DISABLE,
            /*fbp_drop_threshold;*/0,
            /*replace_vlan_tag*/WP_IW_REPLACE_VTAG_DISABLE,
            /*vlan_tag*/0,
            /*vpmt_handle */0,
            /*mtu;*/1536,
            /*prefix_length */ 0,
            /*prefix_header[28];*/ {0},
            /*policer_enable*/ WP_IW_POLICER_DISABLE,
            /**policer_config;*/NULL,
            /*cong_mode;*/WP_IW_CONGESTION_DISABLE,
            /**cong_threshold_param;*/NULL
         }
      };


   flooding_tx[0].txfunc = sys->host_rx;
   sys->flow_host = WP_IwFlowAggregationCreate(sys->wpid,
                                               WP_IW_VLAN_AWARE_BRIDGING_MODE,
                                               &flooding_tx);
   WPE_TerminateOnError(sys->flow_host," WP_IwFlowAggregationCreate()",__LINE__);

   /* IW System  */
   sys->iw_system=WP_IwSystemCreate(sys->wpid,WP_IW_VLAN_AWARE_BRIDGING_MODE,
                                    &iw_sys_one);
   WPE_TerminateOnError(sys->iw_system,"Wp_IwSystemCreate",__LINE__);

}



/*******************************************************************************
 *
 *Creat The needed Ports and Devices
 *
 *******************************************************************************/
static void WPE_PortDeviceSetup(WPE_system *sys)
{

   WP_U32 ii=0;
   WP_CHAR buff[30];

   static WP_port_enet enet_s[1] =
      {
         {
            /* pkt_limits */  {64, 64, PQ_BLOCK_SIZE, 2},
            /* flowmode */ WP_FLOWMODE_FAST,
            /* interface_mode */ WP_ENET_SGMII_1000,
            /* rx_iw_bkgnd */0,
         }
      };


   /* Enet device configuration */
   static WP_device_enet enet_dev[1] =
      {
         {
            /* max_tx_channels */ 8,
            /* tx_maxsdu */  1536,
            /* rmii_operating_speed */ WP_UNUSED,
            /* mac_addr*/ {0xaa,0xaa,0xaa,0xaa,0xaa,0xaa},
            /*tx_bit_rate;*/ 1000000000,
#ifdef USE_SOCKET
            /* loopbackmode */ WP_ENET_LOOPBACK,
#else
            /* loopbackmode */ WP_ENET_NORMAL,
#endif
            /* extended_params */ NULL,
         }
      };

   WP_enet_filters rx_filters[1] =
      {
         {
            /*rx_ucfilter*/WP_ENET_FILTER_PROCESS,
            /*rx_bcfilter*/WP_ENET_FILTER_PROCESS,
            /*rx_mcfilter*/WP_ENET_FILTER_PROCESS
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
            /*rx_filters*/       rx_filters,
            /*rx_timestamp*/       WP_DISABLE,
            /*timestamp_type*/       0,
         },
      };

   WP_port_pkt_limits pkt_limits = {64, 64, 0,0};
   WP_port_upi_packet_ext packet_ext = {
      /* packet characteristics */ 0,
      /* slow_phy_tx_bitrate */ 0,
      /* pkt_limits */ &pkt_limits
   };

   WP_port_upi upi_s[1] = {{
         /* upimode */ WP_UPI_POS_HIGHSPEED,
         /* loopbackmode */  WP_UPI_LOOPBACK,
         /* tx_characteristics */
         WP_UPI_SLAVE |
         WP_UPI_SINGLE |
         WP_UPI_PRT_ODD |
         WP_UPI_ROBIN |
         WP_TQ_FAST_MODE |
#if UPI_MIXED_SHAPING
         WP_UPI_FMU_HIERARCHICAL_SHAPING_MODE |
#endif
         WP_TX_PRIORITY_4 |
         WP_UPI_BUS_WIDTH_16,
         /* rx_characteristics */
         WP_UPI_MASTER |
         WP_UPI_SINGLE |
         WP_UPI_PRT_ODD |
         WP_UPI_ROBIN |
         WP_UPI_PRTCHK_OFF |
         WP_UPI_BUS_WIDTH_16,
         /* atm_ext */ 0,
         /* packet_ext */ &packet_ext,
         /* iw_ext */ 0,
         /* emphy_ext */ 0
      }};

   /* hspos device configuration */
   WP_device_upi_hspos upi_device[1] =
      {
         {
            /* tx_bit_rate */  WP_TX_BIT_RATE_UNLIMITED,//622000000,
            /* tx stat mode */WP_PKT_STAT_ENABLE,
            /* tx int mode */WP_PKTCH_INT_DISABLE,
            /* tx_slave_addr */ 0,
#if UPI_MIXED_SHAPING
            /* tx queue depth */{0},
#else
            /* tx queue depth */{ 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000},
#endif
            /* rx_bit_rate */ 622000000,
            /* rx stat mode */WP_PKT_STAT_ENABLE,
            /* rx int mode */WP_PKTCH_INT_DISABLE,
            /* rx queue depth */2000,
            /* rx_slave_addr */ 0
         }
      };


   sys->phost_handle=WP_PortCreate(sys->wpid,WP_PORT_IW_HOST,NULL);
   WPE_TerminateOnError(sys->phost_handle,"WP_PortCreate Host",__LINE__);

   sys->devhost = WP_DeviceCreate(sys->phost_handle, 0, WP_DEVICE_IW_HOST, NULL);
   WPE_TerminateOnError(sys->devhost,"WP_DeviceCreate Host",__LINE__);

   for (ii=0;ii<NUMBER_OF_ENET_PORTS;ii++)
   {

      sys->enet_port_handle[ii]=WP_PortCreate(sys->wpid,WP_PORT_ENET13+ii,enet_s);
      /*   if (ii>0)
           enet_dev[0].loopbackmode=WP_ENET_LOOPBACK;*/

      sprintf(buff,"WP_PortCreate ENET %d",ii+1);

      WPE_TerminateOnError(sys->enet_port_handle[ii],buff,__LINE__);
      enet_dev->extended_params = NULL;

      sys->enetdev[ii]=WP_DeviceCreate(sys->enet_port_handle[ii],0,WP_DEVICE_ENET,enet_dev);
      sprintf(buff,"WP_DeviceCreate ENET %d",ii+1);
      WPE_TerminateOnError(sys->enetdev[ii],buff,__LINE__);

      enet_dev->extended_params = enet_device_extended_config;

      status = WP_DeviceModify(sys->enetdev[ii],WP_DEV_MOD_ENET_TX_STATMODE |
                               WP_DEV_MOD_ENET_RX_STATMODE | WP_DEV_MOD_ENET_PHYSTATMODE,enet_dev);
      WPE_TerminateOnError(status,"WP_DeviceModify",__LINE__);

   }

   for (ii=0;ii<NUMBER_OF_UPI_PORTS;ii++)
   {
      sys->upi_port_handle[ii]=WP_PortCreate(sys->wpid,WP_PORT_UPI1+ii,upi_s);
      sprintf(buff,"WP_PortCreate UPI %d",ii+1);
      WPE_TerminateOnError(sys->upi_port_handle[ii],buff,__LINE__);
      sys->upidev[ii]=WP_DeviceCreate(sys->upi_port_handle[ii],0,WP_DEVICE_HSPOS,upi_device);
      sprintf(buff,"WP_DeviceCreate UPI %d",ii+1);
      WPE_TerminateOnError(sys->upidev[ii],buff,__LINE__);
   }
}



/******************************************************
 *
 *Create the buffer pool and Qnodes
 *
 *****************************************************/
static void WPE_PoolCreate(WPE_system *l2_struct)
{
   WP_U32    wpid=0;
   WP_handle pool_ring, pool_ring_host;
   WP_handle pool_256, pool_2048,pool_1536;
   static WP_pool_buffer_data buffer_data_2048[1] =
      {
         {
            /* n_buffers */ 100,
            /* offset */ 64,
            /* size */ APP_HOST_BUFFER_SIZE,
            /* pad */ 0,
#ifdef USE_SOCKET
            /* bus */ WP_BUS_HOST,  /* This should really be WP_BUS_PACKET */
            /* bank */ APP_BANK_HOST
#else
            /* bus */ WP_BUS_PACKET,
            /* bank */ APP_BANK_PACKET
#endif
         }
      };

   WP_pool_buffer_data host_buffer_data[1] =
      {
         {/* n_buffers */ 100,
          /* offset */ 0,
          /* size */ 144,
          /* pad */ 0,
          /* bus */ WP_BUS_PACKET,
          /* bank */ APP_BANK_PACKET
         }
      };



   static WP_pool_ring_data ring_data[1] =
      {
         {
            /* n_rings */ 4,
            /* ring_length */ 8,
#ifdef USE_SOCKET
            /* bus */ WP_BUS_HOST,  /* This should really be WP_BUS_PACKET */
            /* bank */ APP_BANK_HOST
#else
            /* bus */ WP_BUS_PACKET, /*_PARAM,*/
            /* bank */ APP_BANK_PACKET /*_PARAM*/
#endif
         }
      };
   static WP_pool_ring_data ring_data_host[1] =
      {
         {
            /* n_rings */ 50,
            /* ring_length */ 4,
#ifdef USE_SOCKET
            /* bus */ WP_BUS_HOST,  /* This should really be WP_BUS_PACKET */
            /* bank */ APP_BANK_HOST
#else
            /* bus */ WP_BUS_PACKET, /*_PARAM,*/
            /* bank */ APP_BANK_PACKET /*_PARAM*/
#endif
         }
      };
   WP_pool_buffer_data buffer_data_1536[] =
      {
         {/* n_buffers */ 1024,
          /* offset */    0,
          /* size */      1536,
          /* pad */       0,
          /* bus */       WP_BUS_PACKET,
          /* bank */      APP_BANK_PACKET
         }
      };




   static WP_qnode_hostq qn_host[1] =
      {
         {
            /* pool_buffer */ 0,
            /* pool_ring */ 0,
            /* interruptqueue */ WP_IRQT1,
         }
      };
   static WP_qnode_iwq qn_iw[1]=
      {
         {
            /* interruptqueue */ 0,
            /* num_buffers */100,
            /* adjunct_pool */ 0
         }
      };

   WP_qnode_iw_mcq iw_mc_qn_config[1]=
      {
         {
            /* interruptqueue */ 0,
            /* num_buffers */ 100,
            /* offset */ 0x40,
            /* size */ 128,
            /* pad */ 0
         }
      };

   pool_1536 = WP_PoolCreate(WP_WINPATH(0), WP_pool_buffer, buffer_data_1536);
   WPE_TerminateOnError(pool_1536, "WP_PoolCreate() pool_1536", __LINE__);

/* Create the Buffer pool(s) - atm -> atm  system 1 */
   pool_256 = WP_PoolCreate(wpid, WP_pool_buffer, host_buffer_data);
   WPE_TerminateOnError(pool_256, "WP_PoolCreate() pool_256",__LINE__);

   pool_2048 = WP_PoolCreate(wpid, WP_pool_iwbuffer, buffer_data_2048);
   WPE_TerminateOnError(pool_2048, "WP_PoolCreate() pool_2048",__LINE__);
   l2_struct->data_pool_handle = pool_2048;
   l2_struct->buffer_data_size = buffer_data_2048[0].size;
   /* Create the Ring pool(s) */
   pool_ring = WP_PoolCreate(wpid, WP_pool_ring, ring_data);
   WPE_TerminateOnError(pool_ring, "WP_PoolCreate() pool_ring",__LINE__);

   /* Create the Ring pool(s) */
   pool_ring_host = WP_PoolCreate(wpid, WP_pool_ring, ring_data_host);
   WPE_TerminateOnError(pool_ring_host, "WP_PoolCreate() pool_ring_host",__LINE__);

   qn_host->pool_buffer = pool_1536;
   qn_host->pool_ring = pool_ring_host;
   l2_struct->qnh = WP_QNodeCreate(wpid, WP_QNODE_HOSTQ, qn_host);
   WPE_TerminateOnError(l2_struct->qnh, "WP_QNodeCreate() qnh",__LINE__);


   /* Create an interworking queue node */
   qn_iw->adjunct_pool = pool_2048;
   l2_struct->qniw = WP_QNodeCreate(wpid, WP_QNODE_IWQ | WP_QNODE_OPT_DEDICATED_RX_HWQ | WP_QNODE_OPT_FMU, qn_iw);
   WPE_TerminateOnError(l2_struct->qniw, "WP_QNodeCreate() qniw",__LINE__);

   /*Create MC qnode*/
   l2_struct->qn_mc=WP_QNodeCreate(0, WP_QNODE_IW_MCQ | WP_QNODE_OPT_FMU, iw_mc_qn_config);
   WPE_TerminateOnError(l2_struct->qn_mc, "WP_QNodeCreate() qniw_mc", __LINE__);
}

/*********************************************************
 *
 * Initialize the system
 *
 *
 *********************************************************/
static void WPE_SystemInitialize(WPE_system *sys)
{
   WP_module_pce_init pce_init =
      {
         &pce_parser_info, /* parser_info */
         4000, /* hardware_rules_num */
         50, /* filters_num */
         20, /* pkt_rx_channel_interfaces_num */
         20, /* iw_tx_channel_interfaces_num */
         WP_PCE_DISCARD_MODE_ALL, /* pce_discard_mode */
         WP_ENABLE, /* bridge_loop_prevention_mode */
         WP_ENABLE, /* learning_mode */
         {/* learning_queue_config */
            30, /* learning_queue_size */
            WP_LEARNING_POLLING_MODE, /* interrupt_mode */
            WP_IW_IRQT0, /* int_queue_num */
            30, /* interrupt_rate */
         }
      };
   static WP_context context[1]=
      {
         {
            3, /* the first four assignments are mandatory bus assignments */
            {
               {/* WP_BUS_PARAM    */ WP_BUS_PARAM, APP_BANK_PARAM},
               {/* WP_BUS_PACKET   */ WP_BUS_PACKET, APP_BANK_PACKET},
               {/* WP_BUS_INTERNAL */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
               {/* WP_BUS_HOST     */ WP_BUS_HOST, APP_BANK_HOST},
               {/* WP_BUS_IRQ      */ WP_BUS_PACKET, APP_BANK_PACKET},
               {/* WP_BUS_TMSTMP   */ WP_BUS_HOST, APP_BANK_HOST},
               {/* UNASSIGNED      */ WP_BUS_NONE, 0},
               {/* UNASSIGNED      */ WP_BUS_NONE, 0},
               {/* ASU_TPT         */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
               {/* ASU CW          P*/ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
               {/* ASU TX QUEUE    */ WP_BUS_PARAM, APP_BANK_PARAM},/*10*/
               {/* STATISTICS      */ WP_BUS_HOST, APP_BANK_HOST},
               {/* ADDRESS LOOKUP  */ WP_BUS_PARAM, APP_BANK_PARAM},
               {/* ATM CPT         */ WP_BUS_PARAM, APP_BANK_PARAM},
               {/* ENET HDLC CPT   */ WP_BUS_PARAM, APP_BANK_PARAM},
               {/* IW FBP PARAM    */ WP_BUS_PARAM, APP_BANK_PARAM},
               {/* IW FBP DATA     */ WP_BUS_PACKET, APP_BANK_PACKET},
               {/* EXTERNAL FIFO   */ WP_BUS_PACKET, APP_BANK_PACKET},
               {/* MAC LOOKUP      */ WP_BUS_PARAM, APP_BANK_PARAM},
               {/* IMA SMALL       */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
               {/* IMA LARGE       */ WP_BUS_HOST, APP_BANK_HOST},/*20*/
               {/* LPM             */ WP_BUS_HOST, APP_BANK_HOST},
               {/* MFC RULES       */ WP_BUS_HOST, APP_BANK_HOST},
               {/* FIWT, QAT       */ WP_BUS_PARAM, APP_BANK_PARAM},
               {/* MFCPT, UPPPT, DS*/ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
               {/* STATISTICS2      */ WP_BUS_HOST, APP_BANK_HOST},
               {/* STATISTICS3      */ WP_BUS_HOST, APP_BANK_HOST},
               {/* UNASSIGNED      */ WP_BUS_NONE, 0},
               {/* PKT_GLOBAL_TCPT */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
               {/* UNASSIGNED      */ WP_BUS_NONE, 0},
               {/* MANDATORY       */ WP_BUS_NONE, 0},
               {/* MANDATORY       */ WP_BUS_NONE, 0},
               /* ************* VIRTUAL BUS ASSIGNMENT ***************/
               {/* MCH_DATA        */ WP_BUS_PACKET, APP_BANK_PACKET}
#if 1
               ,
               {/* WP_VB_OAM_FM_QUERY*/ WP_BUS_NONE, 0},
               {/* WP_VB_HASH_ON_HEAP*/ WP_BUS_NONE, 0},
               {/* WP_VB_MFC_INTERNAL*/ WP_BUS_PACKET, APP_BANK_PACKET},
               /*WP_VB_MFR_RAS_TBL,           */{WP_BUS_NONE, 0},
               /*WP_VB_CESOP_DATA,            */{WP_BUS_NONE, 0},
               /*WP_VB_SECURITY_ESE,          */{WP_BUS_NONE, 0},
               /*WP_VB_PDCP_SECURITY_ENTITY,  */{WP_BUS_NONE, 0},
               /*WP_VB_POLICER,               */{WP_BUS_PARAM, APP_BANK_PARAM},
               /*WP_VB_PCE_LRN,               */{WP_BUS_INTERNAL, APP_BANK_INTERNAL},
               /*WP_VB_PCEA_PARAM,            */{WP_BUS_INTERNAL, APP_BANK_INTERNAL},
               /*WP_VB_CONGESTION_TABLE,      */{WP_BUS_NONE, 0},
#endif
            },
            {
               dps_ProgramImage, dps_ProgramImage
            },
            {
               dps_PC_Table_Init, dps_PC_Table_Init
            },
            &int_queue_tables, /* Interrupt queue tables */
            POOL_LIMIT,        /* Mixumum number of pools */
            atm_params,        /* ATM global parameters */
            max_iw,            /* Interworking global parameters */
         } };

#define MIN_CH_RATE 150
#define MAX_CH_RATE 600000


   static WP_calendar_wheel calendar[2]=
      {
         {

            /*function*/  WP_CW_ASU,
            /*cwid*/        WP_CW_ID_A,
            /*min_ch_speed*/ MIN_CH_RATE,
            /*max_ch_speed*/ MAX_CH_RATE
         },
         WP_CALENDAR_WHEEL_TERM

      };


   status=WP_DriverInit();
   WPE_TerminateOnError(status,"WP_DriverInit",__LINE__);

   pce_init.hardware_rules_num = MAX_NUMBER_OF_RULES;
   int_ram_partition.policer_entries = 0;
   int_ram_partition.pce_hw_rules_num = pce_init.hardware_rules_num;
   WT_SetIntRamPartition(WP_WINPATH(0), &int_ram_partition);

   status=WP_SysInit(sys->wpid,context);
   WPE_TerminateOnError(status,"WP_SysInit",__LINE__);
   status = WP_ControlRegister(WP_DEBUG_CALLBACK_DEBUG_AREA_LOG, DebugAreaCallback);

   status=WP_SysSchedulerCreate(sys->wpid,calendar);
   WPE_TerminateOnError(status,"WP_SysSchedulerCreate",__LINE__);

   pce_init.parser_info = &pce_parser_info;
   pce_init.pkt_rx_channel_interfaces_num = NUMBER_OF_ENET_PORTS+NUMBER_OF_UPI_PORTS;
   pce_init.learning_queue_config.learning_queue_size = 50;
   pce_init.iw_tx_channel_interfaces_num = 4;
   pce_init.filters_num = 5;
   pce_init.pce_discard_mode = WP_PCE_DISCARD_MODE_ALL;
   pce_init.bridge_loop_prevention_mode = WP_ENABLE;

   WPE_DebugInit();

   status = WP_ModuleInit(WP_SYSHANDLE(0), WP_WDDI_MODULE_PCE, &pce_init);
   WPE_TerminateOnError(status, "WPI_PceModuleInit()",__LINE__);
}

/*********************************************
 *   Terminate if an Error occures
 *********************************************/
static void WPE_TerminateOnError(WP_handle handle, WP_CHAR *s,WP_U32 line)
{
   if (WP_ERROR_P(handle))
   {
      printf("Test Abort %s %s 0x%x\n", s,
             WP_ErrorString(handle), handle);
      printf("Test Failed, Line : %d\n",line);
      WPE_Terminate(1);
      exit(1);
   }
#if TEST_DEBUG
   else
   {
      printf("Bridging App: Handle/Status returned from %s is %#8.8x \n", s, handle);
   }
#endif

}

static void WPE_Terminate(WP_U32 status)
{
   exit(0);
   /* Debug area */
   WP_DebugAreaLog(0, WP_B_DEBUG);
   /* Release WDDI */
   WP_DriverRelease();

/*    For socket based simulators */
#if USE_SOCKET
   CleanUpSocket(5);
#endif

   WT_Reboot();
   exit(status);
}

#if _WT_MODULE_TEST_COMMENT_ENABLE_
#pragma check_tests wt_util.c
#pragma check_tests common
#endif

#include "wt_util.c"
#include "wt_partition_ecc_util.c"


