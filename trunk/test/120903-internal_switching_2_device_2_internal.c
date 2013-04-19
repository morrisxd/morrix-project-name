/*****************************************************************************
 * (C) Copyright PMC - WIN division (Wintegra) 2011.  All rights reserved.
 * PMC-Sierra (WINTEGRA) CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of PMC-Sierra (Wintegra).
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *  Application : Virtual Bonding with QoS
 *  File Name   : virtual_bonding_demo.c
 *  Version     : 1
 *  Date        : 23 January 2012
 *  Accompanies : TBD
 *****************************************************************************/

/***************************************************************************
 *  Description:
 *
 * This application contains Virtual bonding groups and links creation.
 * There are several Dynamic modifications who can be made
 * There are several scenarios in case the application needed to be tested
 *
 *
 ************************************************************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "wp_wddi.h"
#include "wp_host_io.h"
#include "wp_types.h"
#include "wp_sim.h"
#include "wpx_app_data.h"
#include "wpx_oc3_pmc5351.h"
#include "wp_efm_bonding.h"
#include "wpi_gpe_pecs_wddi_shared.h"
#include "header_ext_add_pecs.h"
//#include "is_brouter_pecs.h"
#include "brouter_pecs.h"
#include "wt_partition_ecc_util.h"
#include "wpx_enet_phy.h"
#include "wpu_debug_sw.h"
#include "wpu_bus_mon.h"
#include "wpu_wgu.h"
#include "wpu_sa_actual.h"
#include "wpu_ta_base.h"
#include "wpu_ta_error.h"
#include "wpu_ta_ord.h"
#include "wpu_ta_stat.h"
#include "wpu_sa_alloc.h"
#include "wpu_sa_limits.h"
#include "wpu_tb.h"

/*****************************************************************************/
/*                       External API                                        */
/*****************************************************************************/
extern WP_U32 dps_ProgramImage[];
extern WP_U16 dps_PC_Table_Init[];
extern WP_pce_parser_info pce_parser_info;
extern WP_pecs_image_info brouter_pecs_pecs_image;
extern WP_pecs_image_info is_brouter_pecs_pecs_image;
/*****************************************************************************/
/*                           Defines                                         */
/*****************************************************************************/
enum
{
   QNODE_IW = 0,
   QNODE_RX_IW,
   NUM_OF_QNODES
};
enum
{
   CLASSIFICATION = 0,
   CLASSIFICATION1,
   FORWARDING,
   LEARNING,
   NUM_OF_FILTERS
};
enum
{
   GENERIC_FA_TYPE = 1,
   INTSW_CASCADING_FA_TYPE,
   INTSW_IW__FA_TYPE
};

enum
{
   IS_BROUTER_PECS = 0,
   BROUTER_PECS,
   NUM_OF_PECS
};

#define MAX_FLOWS                 256
#define MAX_BPORTS                300
#define POOL_LIMIT                100
#define DEFAULT_STAG_TYPE         0x9100
#define PRINT_PACKET              1
#define LEARNING_QUEUE_SIZE       10
#define WPE_MAX_PACKET_SIZE       (9*1024)
#define MAX_CLASSES               8
#define MAX_ENETS                 MAX_CLASSES
#define SCENARIOS                 10

/*****************************************************************************/
/*                       Function declarations                               */
/*****************************************************************************/
static void WPE_Main_Menu(void);
/*************** Dynamic Modifications menu ******************/
static void WPE_Dynamic_Modifications(void);
static void WPE_ChangeSwitching(void);
static void WPE_ChangeToIntsw(void);
static void WPE_ChangeToExtsw(void);
static void WPE_ChangeToCascading(void);
static void WPE_ChangeSwitchingLoop(void);
static void WPE_ModifyMtu(WP_U32 mtu);
static void WPE_ModifyPolicer_Menu(void);
static void WPE_ModifyPecs_Menu(void);
static void WPE_ModifyPecs(WP_boolean enable_disable_pecs,WP_U32 operation);
static void WPE_SmartDiscard(WP_U32 enable,WP_U32 threshold);
static void WPE_DeviceDisable(WP_U32 device,WP_boolean enable);
static void WPE_DeviceDisableLoop(WP_U32 device,WP_U32 loops);
/*************** Statistics menu ******************/
static void WPE_Statistics(void);
static void WPE_DeviceStats(void);
static void WPE_Print64bitVal(WP_CHAR title[], WP_U64 value);
static void WPE_PrintNonZeroStats(WP_CHAR title[], WP_U64 value, WP_boolean non_zero_only);
static void WPE_PrintWnetDevStats(WP_handle dev_handle, WP_boolean non_zero_only);
static void WPE_PrintXauiDevStats(WP_handle dev_handle, WP_boolean non_zero_only);
static void WPE_DisplayDeviceStats(WP_handle device_handle, WP_device device, WP_U32 device_number);
static void WPE_PrintClassStats(WP_handle class_handle, WP_U8 class_type, WP_boolean non_zero_only);
static void WPE_IwPortStats(void);
static void WPE_GetPceStatistics_Menu(WP_CHAR *StrPrm);
static void WPE_GetPceStatistics(void);
static void WPE_ShowFlowStats_Menu(WP_CHAR *StrPrm);
static void WPE_ShowFlowStats(void);
static void WPE_StatShowFlowStats(char* StrPrm, WP_handle flow_agg,WP_read_flow_statmode statmode,WP_boolean non_zero_only);
static void WPE_ShowPolicerStats(void);
static void WPE_Saves_Initial_Qnode_Status(void);
static void WPE_Check_Qnode_Status(void);
static void WPE_StatisticsReset(void);
/*************** Scenarios menu ******************/
static void WPE_Scenarios(void);
static void WPE_Initialize_Pce_Rules(void);
static void WPE_Bridging(void);
static void WPE_CheckForwardingAndLearning(void);
static void WPE_Policing(void);
static void WPE_Create_Policer(void);
static void WPE_Enable_Disable_Policer(WP_U32 enable);
static void WPE_Enable_Disable_Policer_Loop(void);
static void WPE_Policer_Vlan(WP_boolean enable_disable,WP_boolean vlan_change_unchange);
static void WPE_Change_Color(WP_U32 change_to_yellow);
static void WPE_Update_Vlan_Rule(WP_U32 create_rule);
static void WPE_Performance(void);
static void WPE_Performance_w_pecs(void);
static void WPE_Mixed(void);
static void WPE_Override(void);
static void WPE_Mpe_Shaping(void);
static void WPE_Create_Override_Rules_Menu(void);
static void WPE_Modify_Override_Rules_Menu(void);
static void WPE_Create_Override_Rules(WP_U32 first_filter_type,WP_U32 second_filter_type);
static void WPE_Modify_Override_Rules(WP_U32 first_filter_type,WP_U32 second_filter_type);

/*************** General ******************/
static void WPE_Initialize_Pce_Interface(void);
static void WPE_ChannelsCreate(void);
static void WPE_CreateMultiClassDevices(void);
static void WPE_CreateIwPorts(void);
static void WPE_IWSystemCreate(void);
static void WPE_ChannelsEnable(void);
static void WPE_CreateFlowAggregation(void);
//static void WPE_Initialize_ExtSw_Rules(void);
//static void WPE_CreateIngressPolicers(void);
static void WPE_SystemSetup(void);
static void WPE_InitializeContext(WP_context *context, WP_iw_global *max_iw, WP_int_queue_table int_queue_tables);
static void WPE_TerminateOnError(WP_handle handle, WP_CHAR *s, WP_U32 line);
static void WPE_Terminate(WP_U32 status);
static void WPE_HostCreate(void);
static void WPE_Quit(void);
static void WPE_Quit_No_Reboot(void);
static void WPE_SystemEnable(void);
static void WPE_Reboot(void);

typedef union StatField
{
      WP_U64 field;
      struct {
            WP_U32 high;
            WP_U32 low;
      } part;
} WPT_StatField;

/*****************************************************************************/
/*                       Local API                                           */
/*****************************************************************************/

WP_stats_enet stats_enet =
{ 0 };
WP_handle xgi1_port_handle, xgi1_device_handle,xgi2_port_handle,xgi2_device_handle;
WP_handle xgi1_rx_channel, extsw_tx_channel_xgi1[MAX_CLASSES],extsw_tx_channel_xgi2[MAX_CLASSES],mixed_tx_channel[MAX_CLASSES];
WP_handle l2_shaping_group_xgi1[MAX_CLASSES],l1_shaping_group_xgi1[MAX_CLASSES];
WP_handle l2_shaping_group_xgi2[MAX_CLASSES],l1_shaping_group_xgi2[MAX_CLASSES];
WP_handle iw_system, bport_handle[2];
WP_handle pecs_handle[NUM_OF_PECS];
WP_handle host_flow_agg_dummy, host_flow_agg, rx_host_handle, rx_host_handle_dummy, host_device,
      host_bport, host_port;
WP_handle xgi1_pce_if_handle, xgi1_pce_filter[NUM_OF_FILTERS] ={0}, xgi1_filter_set_handle;
WP_pce_if_params_pkt_rx_channel pce_if_params;
WP_U32 starting_qnodes_status[NUM_OF_QNODES];
WP_handle qnodes[NUM_OF_QNODES],pools[NUM_OF_QNODES];
WP_qnode_iw_hier qnodes_hier;
WP_pool_buffer_data iw_buffer_data;
WP_pool_iw_bd iw_bd;
WP_handle buf_pool[2];
WP_handle iw_bd_pool[4];
WP_qnode_hier_status starting_hier_qnodes_status[4];
WP_pce_filter_stat xgi1_filter_stats[NUM_OF_FILTERS],xgi2_filter_stats[NUM_OF_FILTERS];
WP_handle iwport_handle[MAX_ENETS];
WP_handle intsw_class_handle[MAX_CLASSES],intsw_xgi2_class_handle[MAX_CLASSES],mixed_class_handle[MAX_CLASSES];
WP_handle extsw_fa_xgi1[MAX_CLASSES],extsw_fa_xgi2[MAX_CLASSES],mixed_fa[MAX_CLASSES],cascading_fa[MAX_CLASSES],intsw_fa_w_pecs[MAX_CLASSES];
WP_handle papt_handle, policer_handle=0;
WP_handle intsw_rules_handle[SCENARIOS][MAX_CLASSES],extsw_rules_handle[SCENARIOS][MAX_CLASSES],mixed_rules_handle[2][MAX_CLASSES];
WP_handle override_rules[2][MAX_CLASSES],input_color_rules[MAX_CLASSES],vlan_update_rules[MAX_CLASSES];
WP_pce_learned_forwarding_rule learned_rules_list[10];
WP_handle fwd_rules[MAX_CLASSES],fwd_idx = 0;
WP_U32 scenario=0;
//////////
WP_handle xgi2_rx_channel;
WP_handle xgi2_pce_if_handle,xgi2_pce_filter[NUM_OF_FILTERS] ={0},xgi2_filter_set_handle;
WP_handle mpe_channel_shaping_scenario[3],mpe_class_shaping_scenario[3],mpe_fa[3];
WP_U32 policer_method=0,color_mode=0,red_packets_action=0,policer_action = 0;
WP_U32 cir =10500000000, eir = 10500000000;

WP_rx_binding_bridging rx_binding_bridging_cfg[1] =
{
      {
            /*  encap_mode */0,
            /*  mru;*/3000,
            /*  vcfcs;*/0,
            /*  input_port;*/0
      }
};
WP_bridge_port bridge_port[1] =
{
      {
            /* tag */0,
            /* direct_mapping */WP_IW_DIRECT_MAP_DISABLE,
            /* flow_agg */0,
            /* flooding_term_mode */WP_IW_HOST_TERM_MODE,
            /* learning_mode */WP_IW_LEARNING_DISABLE,
            /* in_filter_mode */WP_IW_INGRESS_FILTER_DISABLE,
            /* vlan_param */
            {
                  /* vlan_acceptance_mode */WP_IW_ACCEPT_TAGGED_UNTAGGED,
                  /* vlan_tag */1,
            },
            /* max_mac_addresses */34,
            /* group_tag */0,
            /* group_filtering_mode */WP_IW_GROUP_FILTER_DISABLE,
            /* unk_mac_sa_filter*/0,
            /* unk_mc_mode;*/WP_IW_UNK_MC_HT,
            /* bc_ht_mode;*/0,
            /* input_filters_mask;*/0,
            /* output_filters_mask;*/0,
            /* statmode;*/WP_IW_PORT_STAT_ENABLE,
            /* unk_uc_mode */WP_IW_UNK_UC_SR_DISABLE,
            /* classification_flag */WP_IW_BPORT_CLASSIFICATION_DISABLED
      }
};
#define PRINT_STAT_FIELD(field) printf( "%08x%08x", (WP_U32)(field>>32), (WP_U32)field)

///*120901a*/
#if (WT_TRIAL_MASK == 0x2)
#define XGI 0
#define XGI2 1
#endif
///*120901b*/
#if (WT_TRIAL_MASK == 0x4)
#define XGI 1
#define XGI2 0
#endif
///*120901c*/
#if (WT_TRIAL_MASK == 0x8)
#define XGI 0
#define XGI2 0
#endif
///*120901*/
#ifndef XGI
#define XGI 0
#define XGI2 0
#endif

void WPE_Saves_Initial_Qnode_Status()
{
   WP_status status;
   WP_U32 i;

   for (i = 0; i < NUM_OF_QNODES; i++)
   {
      switch(i)
      {
         case QNODE_IW:
            status = WP_QNodeStatus(qnodes[i], WP_QNODE_STATUS_FIFO, &starting_qnodes_status[i]);
            WPE_TerminateOnError(status, "WP_QNodeStatus", __LINE__);
            break;

         case QNODE_RX_IW:
            status = WP_QNodeStatus(qnodes[i], WP_QNODE_STATUS_FIFO, &starting_hier_qnodes_status[i]);
            WPE_TerminateOnError(status, "WP_QNodeStatus", __LINE__);
            break;
      }
   }

}
static void WPE_ShowFlowStats_Menu(WP_CHAR *StrPrm)
{
   WP_U32 res, cmd;
   WP_U32 param1;
   WP_CHAR tmp[300];

   res = sscanf(StrPrm, "%u %u %s", &cmd, &param1, tmp);
   if (res != 2)
   {
//      printf("Usage: <option> <1(Group Number)> <object number>\n");
//      return;
   }

   WPE_ShowFlowStats();

}
static void WPE_ShowFlowStats()
{
   WP_U8 i;
   for (i = 0; i < MAX_CLASSES; i++)
   {
//      WPE_StatShowFlowStats("intsw_flow_agg", extsw_fa_xgi1[i],WP_IW_FLOW_STAT_INTERNAL_SWITCHING,WP_ENABLE);
   }
   for (i = 0; i < MAX_CLASSES; i++)
   {
      WPE_StatShowFlowStats("extsw_flow_agg", extsw_fa_xgi2[i],WP_IW_FLOW_STAT_GENERIC,WP_ENABLE);
   }
   for (i = 0; i < MAX_CLASSES; i++)
   {
      WPE_StatShowFlowStats("mixed_flow_agg", mixed_fa[i],WP_IW_FLOW_STAT_GENERIC,WP_ENABLE);
   }
   for (i = 0; i < MAX_CLASSES; i++)
   {
      WPE_StatShowFlowStats("cascading_flow_agg", cascading_fa[i],WP_IW_FLOW_STAT_GENERIC,WP_ENABLE);
   }
   for (i = 0; i < 3; i++)
   {
//      WPE_StatShowFlowStats("mpe_flow_agg", mpe_fa[i],WP_IW_FLOW_STAT_INTERNAL_SWITCHING,WP_ENABLE);
   }
   for (i = 0; i < MAX_CLASSES; i++)
   {
//      WPE_StatShowFlowStats("intsw_flow_agg with PECS", intsw_fa_w_pecs[i],WP_IW_FLOW_STAT_INTERNAL_SWITCHING,WP_ENABLE);
   }

   /* host statistics */
   WPE_StatShowFlowStats("Host Flow aggregation", host_flow_agg,WP_IW_FLOW_STAT_GENERIC,WP_ENABLE);
   WPE_StatShowFlowStats("Host Flow aggregation1", host_flow_agg_dummy,WP_IW_FLOW_STAT_GENERIC,WP_ENABLE);

}
static void WPE_ShowPolicerStats()
{
   WP_stats_policer_v2 policer_stats;

   memset(&policer_stats, 0, sizeof(WP_stats_policer_v2));
   WP_PolicerStatistics(policer_handle, &policer_stats);

   printf("\npolicer statistics (%x)\n",policer_handle);
   printf("=================================================\n");

   WPE_PrintNonZeroStats("non_conforming_cbs", policer_stats.non_conforming_cbs, WP_DISABLE);
   WPE_PrintNonZeroStats("non_conforming_ebs", policer_stats.non_conforming_ebs, WP_DISABLE);

   printf("\n");

}
static void WPE_Quit_No_Reboot()
{
   exit(1);
}
static void WPE_Quit()
{
// WP_DriverRelease();
   WPE_Reboot();
   exit(1);
}

static void WPE_Check_Qnode_Status()
{
   WP_status status;
   WP_U32 i, after, total_qnode=0;
   WP_U32 ii, jj;
   WP_qnode_hier_status q_status;

   for (i = 0; i < NUM_OF_QNODES; i++)
   {
      switch (i)
      {
         case QNODE_IW:
         {
            status = WP_QNodeStatus(qnodes[i], WP_QNODE_STATUS_FIFO, &after);
            WPE_TerminateOnError(status, "WP_QNodeStatus", __LINE__);

            if (after == starting_qnodes_status[i])
            {
               printf("Starting qnode status is %08x\n", starting_qnodes_status[i]);
               printf("Current qnode status is  %08x\n", after);
               printf("\nQnode %d status is OK\n", i);
            }
            else
            {
               printf("Starting qnode status is %08x\n", starting_qnodes_status[i]);
               printf("Current qnode status is  %08x\n", after);
               printf("\nError in qnode %d status\n", i);
            }

            break;
         }
         case QNODE_RX_IW:
         {
            status = WP_QNodeStatus(qnodes[i], WP_QNODE_STATUS_FIFO, &q_status);
            WPE_TerminateOnError(status, "WP_QNodeStatus", __LINE__);

            for (ii = 0; ii < 2; ii++)
            {
               for (jj = 0; jj < 2; jj++)
               {
                  if(starting_hier_qnodes_status[i].count[ii][jj] != 0)
                  {
                     printf("fifo %d on group %d has %d available BD's\n", q_status.l3_index[ii][jj],
                            q_status.l2_index[ii], q_status.count[ii][jj]);

                     if (q_status.count[ii][jj] != starting_hier_qnodes_status[i].count[ii][jj])
                        total_qnode = 1;
                  }
               }
            }

            if (total_qnode == 0)
               printf("\nQnode %d status is OK\n",i);
            else
               printf("\nError in qnode %d status\n",i);
         }
      }
   }
}

static void WPE_IwPortStats(void)
{
   WP_U8 i;
   WP_bport_stats iwport_stats = { 0 };
   WP_status status;
//   WP_U32 temp1, temp2;
   WP_boolean non_zero_only = WP_ENABLE;

   for (i = 0; i <= MAX_ENETS+1; i++)
   {
      memset(&iwport_stats, 0, sizeof(WP_bport_stats));
      if (i < MAX_ENETS)
         status = WP_IwPortStatistics(iwport_handle[i], &iwport_stats);
      else if (i == MAX_ENETS)
      {
         status = WP_IwPortStatistics(bport_handle[0], &iwport_stats);
         WPE_TerminateOnError(status, "WP_IwPortStatistics", __LINE__);
      }
      else
      {
         status = WP_IwPortStatistics(bport_handle[1], &iwport_stats);
         WPE_TerminateOnError(status, "WP_IwPortStatistics", __LINE__);
      }


      printf("\nStatistics for Iw Port %d:\n", i);

      WPE_Print64bitVal("Rx Valid Packets", iwport_stats.rx_valid_packets);
      WPE_PrintNonZeroStats("ingress_policer_non_conforming_packets", iwport_stats.ingress_policer_non_conforming_packets, non_zero_only);
      WPE_PrintNonZeroStats("Discard VLAN acceptable filter", iwport_stats.discard_vlan_acceptable_filter, non_zero_only);
      WPE_PrintNonZeroStats("Discard ingress filter", iwport_stats.discard_ingress_filter, non_zero_only);
      WPE_PrintNonZeroStats("Discard bridge classifier", iwport_stats.discard_bridge_classifier, non_zero_only);
      WPE_PrintNonZeroStats("Discard unknown MAC SA", iwport_stats.discard_unk_macsa, non_zero_only);
      WPE_PrintNonZeroStats("Deny MAC SA", iwport_stats.deny_mac_sa, non_zero_only);
      WPE_PrintNonZeroStats("Deny MAC DA", iwport_stats.deny_mac_da, non_zero_only);
      WPE_Print64bitVal("Rx BC valid packets", iwport_stats.rx_bc_valid_packets);
      WPE_Print64bitVal("Rx MC valid packets", iwport_stats.rx_mc_valid_packets);
      WPE_Print64bitVal("Forwarded UC packets", iwport_stats.forwarded_uc_packets);
      WPE_Print64bitVal("Forwarded BC packets", iwport_stats.forwarded_bc_packets);
      WPE_Print64bitVal("Forwarded MC packets", iwport_stats.forwarded_mc_packets);
      WPE_PrintNonZeroStats("protocol_error", iwport_stats.protocol_error, non_zero_only);

//      temp1 = iwport_stats.rx_valid_packets;
//      temp2 = iwport_stats.rx_valid_packets >> 32;
//      if (0 != iwport_stats.rx_valid_packets)
//         printf("rx_valid_packets:                             %.8x%.8x\n", temp2, temp1);
//
//      temp1 = iwport_stats.ingress_policer_non_conforming_packets;
//      temp2 = iwport_stats.ingress_policer_non_conforming_packets >> 32;
//      if (0 != iwport_stats.ingress_policer_non_conforming_packets)
//         printf("ingress_policer_non_conforming_packets:       %.8x%.8x\n", temp2, temp1);
//
//      temp1 = iwport_stats.forwarded_uc_packets;
//      temp2 = iwport_stats.forwarded_uc_packets >> 32;
//      if (0 != iwport_stats.forwarded_uc_packets)
//         printf("forwarded_uc_packets:                         %.8x%.8x\n", temp2, temp1);
//
//      temp1 = iwport_stats.rx_bc_valid_packets;
//      temp2 = iwport_stats.rx_bc_valid_packets >> 32;
//      if (0 != iwport_stats.rx_bc_valid_packets)
//         printf("rx_bc_valid_packets:                          %.8x%.8x\n", temp2, temp1);
//
//      temp1 = iwport_stats.rx_mc_valid_packets;
//      temp2 = iwport_stats.rx_mc_valid_packets >> 32;
//      if (0 != iwport_stats.rx_mc_valid_packets)
//         printf("rx_mc_valid_packets:                          %.8x%.8x\n", temp2, temp1);
//
//      temp1 = iwport_stats.forwarded_bc_packets;
//      temp2 = iwport_stats.forwarded_bc_packets >> 32;
//      if (0 != iwport_stats.forwarded_bc_packets)
//         printf("forwarded_bc_packets:                         %.8x%.8x\n", temp2, temp1);
//
//      temp1 = iwport_stats.forwarded_mc_packets;
//      temp2 = iwport_stats.forwarded_mc_packets >> 32;
//      if (0 != iwport_stats.forwarded_mc_packets)
//         printf("forwarded_mc_packets:                         %.8x%.8x\n", temp2, temp1);
//
//      temp1 = iwport_stats.discard_vlan_acceptable_filter;
//      temp2 = iwport_stats.discard_vlan_acceptable_filter >> 32;
//      if (0 != iwport_stats.discard_vlan_acceptable_filter)
//         printf("discard_vlan_acceptable_filter:               %.8x%.8x\n", temp2, temp1);
//
//      temp1 = iwport_stats.discard_ingress_filter;
//      temp2 = iwport_stats.discard_ingress_filter >> 32;
//      if (0 != iwport_stats.ingress_policer_non_conforming_packets)
//         printf("discard_ingress_filter:                       %.8x%.8x\n", temp2, temp1);
//
//      temp1 = iwport_stats.discard_bridge_classifier;
//      temp2 = iwport_stats.discard_bridge_classifier >> 32;
//      if (0 != iwport_stats.discard_bridge_classifier)
//         printf("discard_bridge_classifier:                    %.8x%.8x\n", temp2, temp1);
//
//      temp1 = iwport_stats.discard_unk_macsa;
//      temp2 = iwport_stats.discard_unk_macsa >> 32;
//      if (0 != iwport_stats.discard_unk_macsa)
//         printf("discard_unk_macsa:                            %.8x%.8x\n", temp2, temp1);
//
//      temp1 = iwport_stats.deny_mac_sa;
//      temp2 = iwport_stats.deny_mac_sa >> 32;
//      if (0 != iwport_stats.deny_mac_sa)
//         printf("deny_mac_sa:                                  %.8x%.8x\n", temp2, temp1);
//
//      temp1 = iwport_stats.deny_mac_da;
//      temp2 = iwport_stats.deny_mac_da >> 32;
//      if (0 != iwport_stats.deny_mac_da)
//         printf("deny_mac_da:                                  %.8x%.8x\n", temp2, temp1);
//
//      temp1 = iwport_stats.protocol_error;
//      temp2 = iwport_stats.protocol_error >> 32;
//      if (0 != iwport_stats.protocol_error)
//         printf("protocol_error:                               %.8x%.8x\n", temp2, temp1);
   }
}
/**********************************************************************************
 *  WPE_Print64bitVal
 *
 *
 **********************************************************************************/
static void WPE_Print64bitVal(WP_CHAR title[], WP_U64 value)
{

   printf("%-24s: 0x%08x %08x\n", title,
          ((WPT_StatField)value).part.high, ((WPT_StatField)value).part.low);

}

/**********************************************************************************
 *  WPE_PrintNonZeroStats
 *
 *
 **********************************************************************************/
static void WPE_PrintNonZeroStats(WP_CHAR title[], WP_U64 value, WP_boolean non_zero_only)
{
   if( (non_zero_only == WP_DISABLE) || (value > 0) )
   {
      WPE_Print64bitVal(title, value);
   }
}
/**********************************************************************************
 *  WPE_PrintWnetDevStats
 *
 *
 **********************************************************************************/
static void WPE_PrintWnetDevStats(WP_handle dev_handle, WP_boolean non_zero_only)
{
   WP_status status;
   WP_CHAR        str_err[50];
   WP_stats_enet enet_stats ={0};

   sprintf(str_err, "WP_DeviceStatistics (0x%08x)", dev_handle);
   status = WP_DeviceStatistics(dev_handle, &enet_stats);
   WPE_TerminateOnError(status, str_err, __LINE__);

   printf("Device statistics (0x%08x)\n", dev_handle);
   printf("==============================\n");
   printf("========= MAC statistics =========\n");
   WPE_PrintNonZeroStats("txrx_frames_64", enet_stats.txrx_frames_64, non_zero_only);
   WPE_PrintNonZeroStats("txrx_frames_127", enet_stats.txrx_frames_127, non_zero_only);
   WPE_PrintNonZeroStats("txrx_frames_255", enet_stats.txrx_frames_255, non_zero_only);
   WPE_PrintNonZeroStats("txrx_frames_511", enet_stats.txrx_frames_511, non_zero_only);
   WPE_PrintNonZeroStats("txrx_frames_1023", enet_stats.txrx_frames_1023, non_zero_only);
   WPE_PrintNonZeroStats("txrx_frames_1518", enet_stats.txrx_frames_1518, non_zero_only);
   WPE_PrintNonZeroStats("txrx_frames_1522", enet_stats.txrx_frames_1522, non_zero_only);
   WPE_Print64bitVal("rx_bytes", enet_stats.rx_bytes);
   WPE_Print64bitVal("rx_packets", enet_stats.rx_packets);
   WPE_PrintNonZeroStats("rx_err_fcs", enet_stats.rx_err_fcs, non_zero_only);
   WPE_PrintNonZeroStats("rx_multicast", enet_stats.rx_multicast, non_zero_only);
   WPE_PrintNonZeroStats("rx_broadcast", enet_stats.rx_broadcast, non_zero_only);
   WPE_PrintNonZeroStats("rx_mac_control", enet_stats.rx_mac_control, non_zero_only);
   WPE_PrintNonZeroStats("rx_mac_pause", enet_stats.rx_mac_pause, non_zero_only);
   WPE_PrintNonZeroStats("rx_mac_unknown", enet_stats.rx_mac_unknown, non_zero_only);
   WPE_PrintNonZeroStats("rx_err_alignment", enet_stats.rx_err_alignment, non_zero_only);
   WPE_PrintNonZeroStats("rx_err_length", enet_stats.rx_err_length, non_zero_only);
   WPE_PrintNonZeroStats("rx_err_code", enet_stats.rx_err_code, non_zero_only);
   WPE_PrintNonZeroStats("rx_false_carrier", enet_stats.rx_false_carrier, non_zero_only);
   WPE_PrintNonZeroStats("rx_undersize", enet_stats.rx_undersize, non_zero_only);
   WPE_PrintNonZeroStats("rx_oversize", enet_stats.rx_oversize, non_zero_only);
   WPE_PrintNonZeroStats("rx_fragments", enet_stats.rx_fragments, non_zero_only);
   WPE_PrintNonZeroStats("rx_jabber", enet_stats.rx_jabber, non_zero_only);
   WPE_PrintNonZeroStats("rx_dropped", enet_stats.rx_dropped, non_zero_only);
   WPE_Print64bitVal("tx_bytes", enet_stats.tx_bytes);
   WPE_Print64bitVal("tx_packets", enet_stats.tx_packets);
   WPE_PrintNonZeroStats("tx_multicast", enet_stats.tx_multicast, non_zero_only);
   WPE_PrintNonZeroStats("tx_broadcast", enet_stats.tx_broadcast, non_zero_only);
   WPE_PrintNonZeroStats("tx_mac_pause", enet_stats.tx_mac_pause, non_zero_only);
   WPE_PrintNonZeroStats("tx_defer", enet_stats.tx_defer, non_zero_only);
   WPE_PrintNonZeroStats("tx_excess_defer", enet_stats.tx_excess_defer, non_zero_only);
   WPE_PrintNonZeroStats("tx_single_collision", enet_stats.tx_single_collision, non_zero_only);
   WPE_PrintNonZeroStats("tx_multi_collision", enet_stats.tx_multi_collision, non_zero_only);
   WPE_PrintNonZeroStats("tx_late_collision", enet_stats.tx_late_collision, non_zero_only);
   WPE_PrintNonZeroStats("tx_excess_collision", enet_stats.tx_excess_collision, non_zero_only);
   WPE_PrintNonZeroStats("tx_no_collision", enet_stats.tx_no_collision, non_zero_only);
   WPE_PrintNonZeroStats("tx_mac_pause_honored", enet_stats.tx_mac_pause_honored, non_zero_only);
   WPE_PrintNonZeroStats("tx_dropped", enet_stats.tx_dropped, non_zero_only);
   WPE_PrintNonZeroStats("tx_jabber", enet_stats.tx_jabber, non_zero_only);
   WPE_PrintNonZeroStats("tx_err_fcs", enet_stats.tx_err_fcs, non_zero_only);
   WPE_PrintNonZeroStats("tx_control", enet_stats.tx_control, non_zero_only);
   WPE_PrintNonZeroStats("tx_oversize", enet_stats.tx_oversize, non_zero_only);
   WPE_PrintNonZeroStats("tx_undersize", enet_stats.tx_undersize, non_zero_only);
   WPE_PrintNonZeroStats("tx_fragments", enet_stats.tx_fragments, non_zero_only);

   /* These statistics are maintained in DPS */
   printf("========= DPS statistics =========\n");
   WPE_Print64bitVal("rx_host_frames", enet_stats.rx_host_frames);
   WPE_Print64bitVal("rx_iw_frames", enet_stats.rx_iw_frames);
   WPE_PrintNonZeroStats("rx_err_host_full", enet_stats.rx_err_host_full, non_zero_only);
   WPE_PrintNonZeroStats("rx_err_fbp_underrun", enet_stats.rx_err_fbp_underrun, non_zero_only);
   WPE_PrintNonZeroStats("rx_err_nonvalid_mac", enet_stats.rx_err_nonvalid_mac, non_zero_only);
   WPE_PrintNonZeroStats("rx_err_mru", enet_stats.rx_err_mru, non_zero_only);
   WPE_PrintNonZeroStats("rx_err_sdu", enet_stats.rx_err_sdu, non_zero_only);
   WPE_PrintNonZeroStats("tx_err_underrun", enet_stats.tx_err_underrun, non_zero_only);
   WPE_PrintNonZeroStats("rx_err_overrun", enet_stats.rx_err_overrun, non_zero_only);
   WPE_PrintNonZeroStats("tx_frames", enet_stats.tx_frames, non_zero_only);

   printf("========= CFU/Serial statistics =========\n");
   WPE_Print64bitVal("tx_frames", enet_stats.serial_dps_if_stats.tx_frames);
   WPE_PrintNonZeroStats("tx_bytes", enet_stats.serial_dps_if_stats.tx_bytes, non_zero_only);
   WPE_Print64bitVal("rx_frames", enet_stats.serial_dps_if_stats.rx_frames);
   WPE_PrintNonZeroStats("rx_bytes", enet_stats.serial_dps_if_stats.rx_bytes, non_zero_only);
   WPE_PrintNonZeroStats("rx_dropped_frames", enet_stats.serial_dps_if_stats.rx_dropped_frames, non_zero_only);
   WPE_PrintNonZeroStats("rx_pce_denied_frames", enet_stats.serial_dps_if_stats.rx_pce_denied_frames, non_zero_only);
}

/**********************************************************************************
 *  WPE_PrintXauiDevStats
 *
 *
 **********************************************************************************/
static void WPE_PrintXauiDevStats(WP_handle dev_handle, WP_boolean non_zero_only)
{
   WP_status status;
   WP_CHAR        str_err[50];
   WP_stats_xgi xgi_stats = {0};

   sprintf(str_err, "WP_DeviceStatistics (0x%08x)", dev_handle);
   status = WP_DeviceStatistics(dev_handle, &xgi_stats);
   WPE_TerminateOnError(status, str_err, __LINE__);

   printf("Device statistics (0x%08x)\n", dev_handle);
   printf("==============================\n");
   printf("========= MAC statistics =========\n");
   WPE_Print64bitVal("tx_packets", xgi_stats.tx_packets);
   WPE_Print64bitVal("tx_bytes", xgi_stats.tx_bytes);
   WPE_PrintNonZeroStats("tx_broadcast", xgi_stats.tx_broadcast, non_zero_only);
   WPE_PrintNonZeroStats("tx_multicast", xgi_stats.tx_multicast, non_zero_only);
   WPE_PrintNonZeroStats("tx_mac_pause", xgi_stats.tx_mac_pause, non_zero_only);
   WPE_PrintNonZeroStats("tx_frames_64", xgi_stats.tx_frames_64, non_zero_only);
   WPE_PrintNonZeroStats("tx_frames_65_127", xgi_stats.tx_frames_65_127, non_zero_only);
   WPE_PrintNonZeroStats("tx_frames_128_255", xgi_stats.tx_frames_128_255, non_zero_only);
   WPE_PrintNonZeroStats("tx_frames_256_511", xgi_stats.tx_frames_256_511, non_zero_only);
   WPE_PrintNonZeroStats("tx_frames_512_1023", xgi_stats.tx_frames_512_1023, non_zero_only);
   WPE_PrintNonZeroStats("tx_frames_1024_1518", xgi_stats.tx_frames_1024_1518, non_zero_only);
   WPE_PrintNonZeroStats("tx_frames_1519_max", xgi_stats.tx_frames_1519_max, non_zero_only);
   WPE_PrintNonZeroStats("tx_err_packets", xgi_stats.tx_err_packets, non_zero_only);
   WPE_Print64bitVal("rx_packets", xgi_stats.rx_packets);
   WPE_Print64bitVal("rx_bytes", xgi_stats.rx_bytes);
   WPE_PrintNonZeroStats("rx_broadcast", xgi_stats.rx_broadcast, non_zero_only);
   WPE_PrintNonZeroStats("rx_multicast", xgi_stats.rx_multicast, non_zero_only);
   WPE_PrintNonZeroStats("rx_mac_pause", xgi_stats.rx_mac_pause, non_zero_only);
   WPE_PrintNonZeroStats("rx_frames_64", xgi_stats.rx_frames_64, non_zero_only);
   WPE_PrintNonZeroStats("rx_frames_65_127", xgi_stats.rx_frames_65_127, non_zero_only);
   WPE_PrintNonZeroStats("rx_frames_128_255", xgi_stats.rx_frames_128_255, non_zero_only);
   WPE_PrintNonZeroStats("rx_frames_256_511", xgi_stats.rx_frames_256_511, non_zero_only);
   WPE_PrintNonZeroStats("rx_frames_512_1023", xgi_stats.rx_frames_512_1023, non_zero_only);
   WPE_PrintNonZeroStats("rx_frames_1024_1518", xgi_stats.rx_frames_1024_1518, non_zero_only);
   WPE_PrintNonZeroStats("rx_frames_1519_max", xgi_stats.rx_frames_1519_max, non_zero_only);
   WPE_PrintNonZeroStats("rx_undersize", xgi_stats.rx_undersize, non_zero_only);
   WPE_PrintNonZeroStats("rx_oversize", xgi_stats.rx_oversize, non_zero_only);
   WPE_PrintNonZeroStats("rx_jabber", xgi_stats.rx_jabber, non_zero_only);
   WPE_PrintNonZeroStats("rx_err_fcs", xgi_stats.rx_err_fcs, non_zero_only);
   WPE_PrintNonZeroStats("rx_err_length", xgi_stats.rx_err_length, non_zero_only);
   WPE_PrintNonZeroStats("rx_err_code", xgi_stats.rx_err_code, non_zero_only);

   /* These statistics are maintained in DPS */
   printf("========= DPS statistics =========\n");
   WPE_Print64bitVal("rx_host_frames", xgi_stats.rx_host_frames);
   WPE_Print64bitVal("rx_iw_frames", xgi_stats.rx_iw_frames);
   WPE_PrintNonZeroStats("rx_err_host_full", xgi_stats.rx_err_host_full, non_zero_only);
   WPE_PrintNonZeroStats("rx_err_fbp_underrun", xgi_stats.rx_err_fbp_underrun, non_zero_only);
   WPE_PrintNonZeroStats("rx_err_nonvalid_mac", xgi_stats.rx_err_nonvalid_mac, non_zero_only);
   WPE_PrintNonZeroStats("rx_err_mru", xgi_stats.rx_err_mru, non_zero_only);
   WPE_PrintNonZeroStats("rx_err_sdu", xgi_stats.rx_err_sdu, non_zero_only);
   WPE_PrintNonZeroStats("tx_err_underrun", xgi_stats.tx_err_underrun, non_zero_only);
   WPE_PrintNonZeroStats("rx_err_overrun", xgi_stats.rx_err_overrun, non_zero_only);
   WPE_PrintNonZeroStats("tx_frames", xgi_stats.tx_frames, non_zero_only);

   printf("========= CFU/Serial statistics =========\n");
   WPE_Print64bitVal("tx_frames", xgi_stats.serial_dps_if_stats.tx_frames);
   WPE_PrintNonZeroStats("tx_bytes", xgi_stats.serial_dps_if_stats.tx_bytes, non_zero_only);
   WPE_Print64bitVal("rx_frames", xgi_stats.serial_dps_if_stats.rx_frames);
   WPE_PrintNonZeroStats("rx_bytes", xgi_stats.serial_dps_if_stats.rx_bytes, non_zero_only);
   WPE_PrintNonZeroStats("rx_dropped_frames", xgi_stats.serial_dps_if_stats.rx_dropped_frames, non_zero_only);
   WPE_PrintNonZeroStats("rx_pce_denied_frames", xgi_stats.serial_dps_if_stats.rx_pce_denied_frames, non_zero_only);

}

static void WPE_DeviceStats(void)
{
   WP_U8 i;

//   printf("\nInternal Switching XGI1 classes\n");
//   for (i = 0; i < MAX_CLASSES; i++)
//   {
////      WPE_DisplayDeviceStats(intsw_class_handle[i], WP_DEVICE_ENET_MULTI_CLASS_INTSW, i);
//      WPE_PrintClassStats(intsw_class_handle[i], WP_DEVICE_ENET_MULTI_CLASS_INTSW, WP_ENABLE);
//   }

//   printf("Internal Switching XGI2 classes\n");
//   for (i = 0; i < MAX_CLASSES; i++)
//   {
//      WPE_PrintClassStats(intsw_xgi2_class_handle[i], WP_DEVICE_ENET_MULTI_CLASS_INTSW, WP_ENABLE);
//   }

//   printf("\nMixed Switching classes\n");
//   for (i = 0; i < MAX_CLASSES; i++)
//   {
////      WPE_DisplayDeviceStats(mixed_class_handle[i], WP_DEVICE_ENET_MULTI_CLASS, i);
//      WPE_PrintClassStats(mixed_class_handle[i], WP_DEVICE_ENET_MULTI_CLASS, WP_ENABLE);
//   }

#if XGI
   printf("\nXGI1 Device\n");
//   WPE_DisplayDeviceStats(xgi1_device_handle, WP_DEVICE_ENET, 0);
   WPE_PrintXauiDevStats(xgi1_device_handle,WP_ENABLE);

   printf("\nXGI2 Device\n");
//   WPE_DisplayDeviceStats(xgi2_device_handle, WP_DEVICE_ENET, 0);
   WPE_PrintXauiDevStats(xgi2_device_handle,WP_ENABLE);
#else
   printf("\ENET14 Device\n");
//   WPE_DisplayDeviceStats(xgi1_device_handle, WP_DEVICE_ENET, 0);
   WPE_PrintWnetDevStats(xgi1_device_handle,WP_ENABLE);

   printf("\nENET15 Device\n");
//   WPE_DisplayDeviceStats(xgi2_device_handle, WP_DEVICE_ENET, 0);
   WPE_PrintWnetDevStats(xgi2_device_handle,WP_ENABLE);
#endif
}
static void WPE_DisplayDeviceStats(WP_handle device_handle, WP_device device, WP_U32 device_number)
{
   WP_U32 temp1, temp2;
   WP_status status;
   WP_stats_enet_multi_class_intsw intsw_mpe_stat;
   WP_stats_enet_multi_class_tx extsw_mpe_stat;
   WP_stats_xgi xgi_stats;

   switch (device)
   {
      case WP_DEVICE_ENET:
         printf("\nDevice statistics of device_handle 0x%x\n", device_handle);
         printf("============================================\n");
         memset(&xgi_stats, 0, sizeof(WP_stats_xgi));
         if((status = WP_DeviceStatistics(device_handle, &xgi_stats)))
         {
            printf("GetStatistics error\n");
            break;
         }

         temp1 = xgi_stats.rx_packets;
         temp2 = xgi_stats.rx_packets >> 32;
         printf("rx_packets:              %.8x%.8x\n", temp2, temp1);
         temp1 = xgi_stats.rx_bytes;
         temp2 = xgi_stats.rx_bytes >> 32;
         printf("rx_bytes:                %.8x%.8x\n", temp2, temp1);
         temp1 = xgi_stats.tx_packets;
         temp2 = xgi_stats.tx_packets >> 32;
         printf("tx_packets:              %.8x%.8x\n", temp2, temp1);
         temp1 = xgi_stats.tx_bytes;
         temp2 = xgi_stats.tx_bytes >> 32;
         printf("tx_bytes:                %.8x%.8x\n", temp2, temp1);

//         ////////////////////////
//         WP_U64 tx_bytes;
//         WP_U64 tx_packets;
//         WP_U64 tx_broadcast;
//         WP_U64 tx_multicast;
//         WP_U64 tx_mac_pause;
//         WP_U64 tx_frames_64;
//         WP_U64 tx_frames_65_127;
//         WP_U64 tx_frames_128_255;
//         WP_U64 tx_frames_256_511;
//         WP_U64 tx_frames_512_1023;
//         WP_U64 tx_frames_1024_1518;
//         WP_U64 tx_frames_1519_max;
//         WP_U64 tx_err_packets;
//         WP_U64 rx_bytes;
//         WP_U64 rx_packets;
//         WP_U64 rx_broadcast;
//         WP_U64 rx_multicast;
//         WP_U64 rx_mac_pause;
//         WP_U64 rx_frames_64;
//         WP_U64 rx_frames_65_127;
//         WP_U64 rx_frames_128_255;
//         WP_U64 rx_frames_256_511;
//         WP_U64 rx_frames_512_1023;
//         WP_U64 rx_frames_1024_1518;
//         WP_U64 rx_frames_1519_max;
//         WP_U64 rx_undersize;
//         WP_U64 rx_oversize;
//         WP_U64 rx_jabber;
//         WP_U64 rx_err_fcs;
//         WP_U64 rx_err_length;
//         WP_U64 rx_err_code;
//
//         /* These statistics are maintained in DPS */
//         WP_U64 rx_host_frames;
//         WP_U64 rx_iw_frames;
//         WP_U64 rx_err_host_full;
//         WP_U64 rx_err_fbp_underrun;
//         WP_U64 rx_err_nonvalid_mac;
//         WP_U64 rx_err_mru;
//         WP_U64 rx_err_sw_state_machine;
//         WP_U64 rx_err_sdu;
//         WP_U64 tx_err_underrun;
//         WP_U64 rx_err_overrun;
//         WP_U64 tx_frames;
//         /////////////////////

         temp1 = xgi_stats.rx_iw_frames;
         temp2 = xgi_stats.rx_iw_frames >> 32;
         if (0 != xgi_stats.rx_iw_frames )
            printf("rx_iw_frames:            %.8X%.8X\n", temp2, temp1);
//         temp1 = xgi_stats.serial_dps_if_stats.rx_frames;
//         temp2 = xgi_stats.serial_dps_if_stats.rx_frames >> 32;
//         if (0 != xgi_stats.serial_dps_if_stats.rx_frames)
//            printf("CFU rx_frames:           %.8x%.8x\n", temp2, temp1);
//         temp1 = xgi_stats.serial_dps_if_stats.rx_dropped_frames;
//         temp2 = xgi_stats.serial_dps_if_stats.rx_dropped_frames >> 32;
//         if (0 != xgi_stats.serial_dps_if_stats.rx_dropped_frames)
//            printf("CFU rx_dropped:          %.8x%.8x\n", temp2, temp1);
//         temp1 = xgi_stats.serial_dps_if_stats.rx_pce_denied_frames;
//         temp2 = xgi_stats.serial_dps_if_stats.rx_pce_denied_frames >> 32;
//         if (0 != xgi_stats.serial_dps_if_stats.rx_pce_denied_frames)
//            printf("CFU rx_pce_denied:       %.8x%.8x\n", temp2, temp1);
         temp1 = xgi_stats.rx_err_overrun;
         temp2 = xgi_stats.rx_err_overrun >> 32;
         if (0 != xgi_stats.rx_err_overrun)
            printf("rx_err_overrun:          %.8x%.8x\n", temp2, temp1);
         temp1 = xgi_stats.rx_err_sdu;
         temp2 = xgi_stats.rx_err_sdu >> 32;
         if (0 != xgi_stats.rx_err_sdu)
            printf("rx_err_sdu:              %.8x%.8x\n", temp2, temp1);
         temp1 = xgi_stats.rx_err_nonvalid_mac;
         temp2 = xgi_stats.rx_err_nonvalid_mac >> 32;
         if (0 != xgi_stats.rx_err_nonvalid_mac)
            printf("rx_err_nonvalid_mac:     %.8x%.8x\n", temp2, temp1);
         temp1 = xgi_stats.tx_err_packets;
         temp2 = xgi_stats.tx_err_packets >> 32;
         if (0 != xgi_stats.tx_err_packets)
            printf("tx_err_packets:          %.8x%.8x\n", temp2, temp1);
         temp1 = xgi_stats.rx_err_mru;
         temp2 = xgi_stats.rx_err_mru >> 32;
         if (0 != xgi_stats.rx_err_mru)
            printf("rx_err_mru:              %.8x%.8x\n", temp2, temp1);
         temp1 = xgi_stats.rx_err_fbp_underrun;
         temp2 = xgi_stats.rx_err_fbp_underrun >> 32;
         if (0 != xgi_stats.rx_err_fbp_underrun)
            printf("rx_err_fbp_underrun:     %.8x%.8x\n", temp2, temp1);
         temp1 = xgi_stats.tx_err_underrun;
         temp2 = xgi_stats.tx_err_underrun >> 32;
         if (0 != xgi_stats.tx_err_underrun)
            printf("tx_err_underrun:         %.8x%.8x\n", temp2, temp1);
//         temp1 = xgi_stats.rx_dropped;
//         temp2 = xgi_stats.rx_dropped >> 32;
//         if (0 != xgi_stats.rx_dropped)
//            printf("rx_dropped:              %.8x%.8x\n", temp2, temp1);
         temp1 = xgi_stats.rx_err_fcs;
         temp2 = xgi_stats.rx_err_fcs >> 32;
         if (0 != xgi_stats.rx_err_fcs)
            printf("rx_err_fcs:              %.8x%.8x\n", temp2, temp1);
//         temp1 = xgi_stats.rx_err_alignment;
//         temp2 = xgi_stats.rx_err_alignment >> 32;
//         if (0 != xgi_stats.rx_err_alignment)
//            printf("rx_err_alignment:        %.8x%.8x\n", temp2, temp1);
         temp1 = xgi_stats.rx_err_length;
         temp2 = xgi_stats.rx_err_length >> 32;
         if (0 != xgi_stats.rx_err_length)
            printf("rx_err_length:           %.8x%.8x\n", temp2, temp1);
         temp1 = xgi_stats.rx_err_code;
         temp2 = xgi_stats.rx_err_code >> 32;
         if (0 != xgi_stats.rx_err_code)
            printf("rx_err_code:             %.8x%.8x\n", temp2, temp1);
         printf("\n");

         break;

      case WP_DEVICE_ENET_MULTI_CLASS:
         printf("\nDevice statistics of External MultiClass %u class_handle 0x%x\n", device_number, device_handle);
         printf("============================================\n");
         memset(&extsw_mpe_stat, 0, sizeof(WP_stats_enet_multi_class_tx));
         if ((status = WP_DeviceStatistics(device_handle, &extsw_mpe_stat)))
         {
            printf("GetStatistics error\n");
            break;
         }

         temp1 = extsw_mpe_stat.tx_frames;
         temp2 = extsw_mpe_stat.tx_frames >> 32;
         printf("tx_frames:               %.8x%.8x\n", temp2, temp1);

         temp1 = extsw_mpe_stat.tx_err_underrun;
         temp2 = extsw_mpe_stat.tx_err_underrun >> 32;
         if (0 != extsw_mpe_stat.tx_err_underrun)
            printf("tx_err_underrun:         %.8x%.8x\n", temp2, temp1);
         temp1 = extsw_mpe_stat.serial_tx_dps_if_stats.tx_frames;
         temp2 = extsw_mpe_stat.serial_tx_dps_if_stats.tx_frames >> 32;
         if (0 != extsw_mpe_stat.serial_tx_dps_if_stats.tx_frames)
            printf("CFU tx_frames:           %.8x%.8x\n", temp2, temp1);
         temp1 = extsw_mpe_stat.serial_tx_dps_if_stats.tx_bytes;
         temp2 = extsw_mpe_stat.serial_tx_dps_if_stats.tx_bytes >> 32;
         if (0 != extsw_mpe_stat.serial_tx_dps_if_stats.tx_bytes)
            printf("CFU tx_bytes:            %.8x%.8x\n", temp2, temp1);

         break;

      case WP_DEVICE_ENET_MULTI_CLASS_INTSW:
         printf("\nDevice statistics of Internal MultiClass %u class_handle 0x%x\n", device_number, device_handle);
         printf("============================================\n");
         memset(&intsw_mpe_stat, 0, sizeof(WP_stats_enet_multi_class_intsw));
         if ((status = WP_DeviceStatistics(device_handle, &intsw_mpe_stat)))
         {
            printf("GetStatistics error\n");
            break;
         }

         temp1 = intsw_mpe_stat.tx_frames;
         temp2 = intsw_mpe_stat.tx_frames >> 32;
         printf("tx_frames:                        %.8x%.8x\n", temp2, temp1);

         temp1 = intsw_mpe_stat.rx_err_sdu;
         temp2 = intsw_mpe_stat.rx_err_sdu >> 32;
         if (0 != intsw_mpe_stat.rx_err_sdu)
            printf("rx_err_sdu :                      %.8x%.8x\n", temp2, temp1);
         /* CFU Stats*/
         temp1 = intsw_mpe_stat.serial_tx_dps_if_stats.tx_frames;
         temp2 = intsw_mpe_stat.serial_tx_dps_if_stats.tx_frames >> 32;
         if (0 != intsw_mpe_stat.serial_tx_dps_if_stats.tx_frames)
            printf("CFU tx_frames:                    %.8x%.8x\n", temp2, temp1);
         temp1 = intsw_mpe_stat.serial_tx_dps_if_stats.tx_bytes;
         temp2 = intsw_mpe_stat.serial_tx_dps_if_stats.tx_bytes >> 32;
         if (0 != intsw_mpe_stat.serial_tx_dps_if_stats.tx_bytes)
            printf("CFU tx_bytes :                    %.8x%.8x\n", temp2, temp1);
         /* Internal Switching CFU Stats*/
         temp1 = intsw_mpe_stat.serial_intsw_dps_if_stats.rx_frames[0];
         temp2 = intsw_mpe_stat.serial_intsw_dps_if_stats.rx_frames[0] >> 32;
         if (0 != intsw_mpe_stat.serial_intsw_dps_if_stats.rx_frames[0])
            printf("Intsw CFU rx_frames:              %.8x%.8x\n", temp2, temp1);
         temp1 = intsw_mpe_stat.serial_intsw_dps_if_stats.rx_bytes[0];
         temp2 = intsw_mpe_stat.serial_intsw_dps_if_stats.rx_bytes[0] >> 32;
         if (0 != intsw_mpe_stat.serial_intsw_dps_if_stats.rx_bytes[0])
            printf("Intsw CFU rx_bytes:               %.8x%.8x\n", temp2, temp1);
         temp1 = intsw_mpe_stat.serial_intsw_dps_if_stats.rx_pce_dropped_frames[0];
         temp2 = intsw_mpe_stat.serial_intsw_dps_if_stats.rx_pce_dropped_frames[0] >> 32;
         if (0 != intsw_mpe_stat.serial_intsw_dps_if_stats.rx_pce_dropped_frames[0])
            printf("Intsw CFU rx_pce_dropped_frames:  %.8x%.8x\n", temp2, temp1);
         temp1 = intsw_mpe_stat.serial_intsw_dps_if_stats.rx_pce_dropped_bytes[0];
         temp2 = intsw_mpe_stat.serial_intsw_dps_if_stats.rx_pce_dropped_bytes[0] >> 32;
         if (0 != intsw_mpe_stat.serial_intsw_dps_if_stats.rx_pce_dropped_bytes[0])
            printf("Intsw CFU rx_pce_dropped_bytes:   %.8x%.8x\n", temp2, temp1);
         temp1 = intsw_mpe_stat.serial_intsw_dps_if_stats.rx_pce_denied_frames;
         temp2 = intsw_mpe_stat.serial_intsw_dps_if_stats.rx_pce_denied_frames >> 32;
         if (0 != intsw_mpe_stat.serial_intsw_dps_if_stats.rx_pce_denied_frames)
            printf("Intsw CFU rx_pce_denied_frames:   %.8x%.8x\n", temp2, temp1);
         temp1 = intsw_mpe_stat.serial_intsw_dps_if_stats.rx_pce_denied_bytes;
         temp2 = intsw_mpe_stat.serial_intsw_dps_if_stats.rx_pce_denied_bytes >> 32;
         if (0 != intsw_mpe_stat.serial_intsw_dps_if_stats.rx_pce_denied_bytes)
            printf("Intsw CFU rx_pce_denied_bytes:    %.8x%.8x\n", temp2, temp1);

         break;
   }
}
static void WPE_StatShowFlowStats(char* StrPrm, WP_handle flow_agg,WP_read_flow_statmode statmode,WP_boolean non_zero_only)
{
   WP_iw_generic_flow_stats extsw_flow_stats;
//   WP_iw_internal_switching_flow_stats intsw_flow_stats;

   switch(statmode)
   {
//      case WP_IW_FLOW_STAT_INTERNAL_SWITCHING:
//
//         memset(&intsw_flow_stats, 0, sizeof(WP_iw_internal_switching_flow_stats));
//         WP_IwFlowStatistics(flow_agg, WP_IW_FLOW_STAT_INTERNAL_SWITCHING, &intsw_flow_stats);
//
//         printf("\nAggregation flow statistics of flow %s (%x)\n", StrPrm, flow_agg);
//         printf("=================================================\n");
//
//         WPE_Print64bitVal("forward_packet", intsw_flow_stats.forward_packet);
//         WPE_PrintNonZeroStats("forwarded_bytes", intsw_flow_stats.forwarded_bytes, non_zero_only);
//         WPE_PrintNonZeroStats("mtu_drop_packets", intsw_flow_stats.mtu_drop_packets, non_zero_only);
//         WPE_PrintNonZeroStats("denied_packets", intsw_flow_stats.denied_packets, non_zero_only);
//         temp_low = intsw_flow_stats.forward_packet;
//         temp_high = (intsw_flow_stats.forward_packet) >> 32;
//         if (0 != intsw_flow_stats.forward_packet)
//            printf("Forward Packet:                   %08x%08x\n", temp_high, temp_low);
//
//         temp_low = intsw_flow_stats.forwarded_bytes;
//         temp_high = (intsw_flow_stats.forwarded_bytes) >> 32;
//         if (0 != intsw_flow_stats.forwarded_bytes)
//            printf("Forwarded Bytes:                  %08x%08x\n", temp_high, temp_low);
//
//         temp_low = intsw_flow_stats.mtu_drop_packets;
//         temp_high = (intsw_flow_stats.mtu_drop_packets) >> 32;
//         if (0 != intsw_flow_stats.mtu_drop_packets)
//            printf("mtu_drop_packets:                 %08x%08x\n", temp_high, temp_low);
//
//         temp_low = intsw_flow_stats.denied_packets;
//         temp_high = (intsw_flow_stats.denied_packets) >> 32;
//         if (0 != intsw_flow_stats.denied_packets)
//            printf("Denied Packets:                   %08x%08x\n", temp_high, temp_low);
//
//         temp_low = intsw_flow_stats.pecs_counter_1;
//         temp_high = (intsw_flow_stats.pecs_counter_1) >> 32;
//         if (0 != intsw_flow_stats.pecs_counter_1)
//            printf("pecs_counter_1:                   %08x%08x\n", temp_high, temp_low);
//
//         temp_low = intsw_flow_stats.pecs_counter_2;
//         temp_high = (intsw_flow_stats.pecs_counter_2) >> 32;
//         if (0 != intsw_flow_stats.pecs_counter_2)
//            printf("pecs_counter_2:                   %08x%08x\n", temp_high, temp_low);
//
//         temp_low = intsw_flow_stats.pecs_counter_3;
//         temp_high = (intsw_flow_stats.pecs_counter_3) >> 32;
//         if (0 != intsw_flow_stats.pecs_counter_3)
//            printf("pecs_counter_3:                   %08x%08x\n", temp_high, temp_low);
//
//         temp_low = intsw_flow_stats.pecs_counter_4;
//         temp_high = (intsw_flow_stats.pecs_counter_4) >> 32;
//         if (0 != intsw_flow_stats.pecs_counter_4)
//            printf("pecs_counter_4:                   %08x%08x\n", temp_high, temp_low);
//
//         temp_low = intsw_flow_stats.pecs_counter_5;
//         temp_high = (intsw_flow_stats.pecs_counter_5) >> 32;
//         if (0 != intsw_flow_stats.pecs_counter_5)
//            printf("pecs_counter_5:                   %08x%08x\n", temp_high, temp_low);
//
//         temp_low = intsw_flow_stats.pecs_counter_6;
//         temp_high = (intsw_flow_stats.pecs_counter_6) >> 32;
//         if (0 != intsw_flow_stats.pecs_counter_6)
//            printf("pecs_counter_6:                   %08x%08x\n", temp_high, temp_low);
//         break;
      case WP_IW_FLOW_STAT_GENERIC:
         memset(&extsw_flow_stats, 0, sizeof(WP_iw_generic_flow_stats));
         WP_IwFlowStatistics(flow_agg, WP_IW_FLOW_STAT_GENERIC, &extsw_flow_stats);

         printf("\nAggregation flow statistics of flow %s (%x)\n", StrPrm, flow_agg);
         printf("=================================================\n");


         WPE_Print64bitVal("forward_packet", extsw_flow_stats.forward_packet);
         WPE_PrintNonZeroStats("mtu_drop_packets", extsw_flow_stats.mtu_drop_packets, non_zero_only);
         WPE_PrintNonZeroStats("tx_queue_drop_packets", extsw_flow_stats.tx_queue_drop_packets, non_zero_only);
         WPE_PrintNonZeroStats("forwarded_bytes", extsw_flow_stats.forwarded_bytes, non_zero_only);
         WPE_PrintNonZeroStats("denied_packets", extsw_flow_stats.denied_packets, non_zero_only);
         WPE_PrintNonZeroStats("policer_non_conforming_packets", extsw_flow_stats.policer_non_conforming_packets, non_zero_only);

//         temp_low = extsw_flow_stats.forward_packet;
//         temp_high = (extsw_flow_stats.forward_packet) >> 32;
//         if (0 != extsw_flow_stats.forward_packet)
//            printf("Forward Packet:                   %08x%08x\n", temp_high, temp_low);
//
//         temp_low = extsw_flow_stats.forwarded_bytes;
//         temp_high = (extsw_flow_stats.forwarded_bytes) >> 32;
//         if (0 != extsw_flow_stats.forwarded_bytes)
//            printf("Forwarded Bytes:                  %08x%08x\n", temp_high, temp_low);
//
//         temp_low = extsw_flow_stats.mtu_drop_packets;
//         temp_high = (extsw_flow_stats.mtu_drop_packets) >> 32;
//         if (0 != extsw_flow_stats.mtu_drop_packets)
//            printf("mtu_drop_packets:                 %08x%08x\n", temp_high, temp_low);
//
//         temp_low = extsw_flow_stats.denied_packets;
//         temp_high = (extsw_flow_stats.denied_packets) >> 32;
//         if (0 != extsw_flow_stats.denied_packets)
//            printf("Denied Packets:                   %08x%08x\n", temp_high, temp_low);
//
//         temp_low = extsw_flow_stats.policer_non_conforming_packets;
//         temp_high = (extsw_flow_stats.policer_non_conforming_packets) >> 32;
//         if (0 != extsw_flow_stats.policer_non_conforming_packets)
//            printf("policer_non_conforming_packets:   %08x%08x\n", temp_high, temp_low);
//
//         temp_low = extsw_flow_stats.pecs_counter_1;
//         temp_high = (extsw_flow_stats.pecs_counter_1) >> 32;
//         if (0 != extsw_flow_stats.pecs_counter_1)
//            printf("pecs_counter_1:                   %08x%08x\n", temp_high, temp_low);
//
//         temp_low = extsw_flow_stats.pecs_counter_2;
//         temp_high = (extsw_flow_stats.pecs_counter_2) >> 32;
//         if (0 != extsw_flow_stats.pecs_counter_2)
//            printf("pecs_counter_2:                   %08x%08x\n", temp_high, temp_low);
//
//         temp_low = extsw_flow_stats.pecs_counter_3;
//         temp_high = (extsw_flow_stats.pecs_counter_3) >> 32;
//         if (0 != extsw_flow_stats.pecs_counter_3)
//            printf("pecs_counter_3:                   %08x%08x\n", temp_high, temp_low);
//
//         temp_low = extsw_flow_stats.pecs_counter_4;
//         temp_high = (extsw_flow_stats.pecs_counter_4) >> 32;
//         if (0 != extsw_flow_stats.pecs_counter_4)
//            printf("pecs_counter_4:                   %08x%08x\n", temp_high, temp_low);
//
//         temp_low = extsw_flow_stats.pecs_counter_5;
//         temp_high = (extsw_flow_stats.pecs_counter_5) >> 32;
//         if (0 != extsw_flow_stats.pecs_counter_5)
//            printf("pecs_counter_5:                   %08x%08x\n", temp_high, temp_low);
//
//         temp_low = extsw_flow_stats.pecs_counter_6;
//         temp_high = (extsw_flow_stats.pecs_counter_6) >> 32;
//         if (0 != extsw_flow_stats.pecs_counter_6)
//            printf("pecs_counter_6:                   %08x%08x\n", temp_high, temp_low);
         break;
      default:
         break;
   }
   printf("\n");
}
/**********************************************************************************
 *  WPE_PrintClassStats
 *
 *
 **********************************************************************************/
static void WPE_PrintClassStats(WP_handle class_handle, WP_U8 class_type, WP_boolean non_zero_only)
{
   WP_status status;
   WP_CHAR        str_err[50];

   printf("Device(class) statistics (0x%08x)\n", class_handle);
   printf("=====================================\n");

   if(class_type == WP_DEVICE_ENET_MULTI_CLASS_INTSW)
   {
      WP_U8 priority;
      WP_stats_enet_multi_class_intsw intsw_stats;

      sprintf(str_err, "WP_DeviceStatistics (0x%08x)", class_handle);
      memset(&intsw_stats, 0, sizeof(WP_stats_enet_multi_class_intsw));
      status = WP_DeviceStatistics(class_handle, &intsw_stats);
      WPE_TerminateOnError(status, str_err, __LINE__);

      printf("========= CFU TX good statistics =========\n");
      WPE_PrintNonZeroStats("tx_frames", intsw_stats.serial_tx_dps_if_stats.tx_frames, non_zero_only);
      WPE_PrintNonZeroStats("tx_bytes", intsw_stats.serial_tx_dps_if_stats.tx_bytes, non_zero_only);
      printf("========= CFU IntSw statistics =========\n");
      WPE_PrintNonZeroStats("pce_denied_frames", intsw_stats.serial_intsw_dps_if_stats.rx_pce_denied_frames, non_zero_only);
      WPE_PrintNonZeroStats("pce_denied_bytes", intsw_stats.serial_intsw_dps_if_stats.rx_pce_denied_bytes, non_zero_only);
      for(priority = 0 ; priority < WP_CFU_RX_AGENT_NUM_OF_PRIORITIES ; priority++)
      {
//         printf("--------- priority %d ---------\n", priority);
         WPE_PrintNonZeroStats("rx_frames", intsw_stats.serial_intsw_dps_if_stats.rx_frames[priority], non_zero_only);
         WPE_PrintNonZeroStats("rx_bytes", intsw_stats.serial_intsw_dps_if_stats.rx_bytes[priority], non_zero_only);
         WPE_PrintNonZeroStats("pce_dropped_frames", intsw_stats.serial_intsw_dps_if_stats.rx_pce_dropped_frames[priority], non_zero_only);
         WPE_PrintNonZeroStats("pce_dropped_bytes", intsw_stats.serial_intsw_dps_if_stats.rx_pce_dropped_bytes[priority], non_zero_only);
      }
      printf("========= DPS statistics =========\n");
      WPE_PrintNonZeroStats("rx_err_sdu", intsw_stats.rx_err_sdu, non_zero_only);
   }
   else
   {
      WP_stats_enet_multi_class_tx  tx_stats;

      sprintf(str_err, "WP_DeviceStatistics (0x%08x)", class_handle);
      memset(&tx_stats, 0, sizeof(WP_stats_enet_multi_class_tx));
      status = WP_DeviceStatistics(class_handle, &tx_stats);
      WPE_TerminateOnError(status, str_err, __LINE__);

      printf("========= CFU TX good statistics =========\n");
      WPE_PrintNonZeroStats("tx_frames", tx_stats.serial_tx_dps_if_stats.tx_frames, non_zero_only);
      WPE_PrintNonZeroStats("tx_bytes", tx_stats.serial_tx_dps_if_stats.tx_bytes, non_zero_only);
      printf("========= DPS statistics =========\n");
      WPE_PrintNonZeroStats("tx_err_underrun", tx_stats.tx_err_underrun, non_zero_only);
      WPE_PrintNonZeroStats("tx_frames", tx_stats.tx_frames, non_zero_only);
   }
}
static void WPE_GetPceStatistics_Menu(WP_CHAR *StrPrm)
{
   WP_U32 res, cmd;
   WP_U32 param1;
   WP_CHAR tmp[300];

   res = sscanf(StrPrm, "%u %u %s", &cmd, &param1, tmp);
   if (res != 1 && res != 2)
   {
      printf("!!!!!!!!!Error with number of parameters!!!!!!!!!!!!!\n");
      return;
   }

   WPE_GetPceStatistics();
}
static void WPE_GetPceStatistics()
{
   WP_status status;

#if XGI
   printf("Printing the XGI1 filter stats\n");
#else
   printf("Printing the Enet14 filter stats\n");
#endif

   status = WP_PceFilterStatistics(xgi1_pce_filter[CLASSIFICATION], &xgi1_filter_stats[CLASSIFICATION]);
   WPE_TerminateOnError(status, "WP_PceFilterStatistics", __LINE__);
   printf("Printing the XGI1 CLASSIFICATION filter stats\n");
   printf("CLASSIFICATION matched:     %.8x \n",(WP_U32) xgi1_filter_stats[CLASSIFICATION].matched_pkts);
   printf("CLASSIFICATION no_matched:  %.8x \n",(WP_U32) xgi1_filter_stats[CLASSIFICATION].no_matched_pkts);
   printf("CLASSIFICATION denied:      %.8x \n",(WP_U32) xgi1_filter_stats[CLASSIFICATION].denied_pkts);

   status = WP_PceFilterStatistics(xgi2_pce_filter[CLASSIFICATION], &xgi2_filter_stats[CLASSIFICATION]);
   WPE_TerminateOnError(status, "WP_PceFilterStatistics", __LINE__);
   printf("Printing the XGI2 CLASSIFICATION filter stats\n");
   printf("CLASSIFICATION matched:     %.8x \n",(WP_U32) xgi2_filter_stats[CLASSIFICATION].matched_pkts);
   printf("CLASSIFICATION no_matched:  %.8x \n",(WP_U32) xgi2_filter_stats[CLASSIFICATION].no_matched_pkts);
   printf("CLASSIFICATION denied:      %.8x \n",(WP_U32) xgi2_filter_stats[CLASSIFICATION].denied_pkts);
   printf("\n");

   status = WP_PceFilterStatistics(xgi2_pce_filter[FORWARDING], &xgi2_filter_stats[FORWARDING]);
   WPE_TerminateOnError(status, "WP_PceFilterStatistics", __LINE__);
   printf("Printing the XGI2 FORWARDING filter stats\n");
   printf("FORWARDING matched:         %.8x \n", (WP_U32) xgi2_filter_stats[FORWARDING].matched_pkts);
   printf("FORWARDING no_matched:      %.8x \n", (WP_U32) xgi2_filter_stats[FORWARDING].no_matched_pkts);
   printf("FORWARDING denied:          %.8x \n", (WP_U32) xgi2_filter_stats[FORWARDING].denied_pkts);
   printf("\n");

   status = WP_PceFilterStatistics(xgi2_pce_filter[LEARNING], &xgi2_filter_stats[LEARNING]);
   WPE_TerminateOnError(status, "WP_PceFilterStatistics", __LINE__);
   printf("Printing the XGI2 LEARNING filter stats\n");
   printf("LEARNING matched:           %.8x \n", (WP_U32) xgi2_filter_stats[LEARNING].matched_pkts);
   printf("LEARNING no_matched:        %.8x \n", (WP_U32) xgi2_filter_stats[LEARNING].no_matched_pkts);
   printf("LEARNING denied:            %.8x \n", (WP_U32) xgi2_filter_stats[LEARNING].denied_pkts);
   printf("\n");
}
static void WPE_StatisticsReset(void)
{
   WP_status status;
   WP_U8 ii;
   WP_iw_generic_flow_stats extsw_flow_stats;
//   WP_iw_internal_switching_flow_stats intsw_flow_stats;
   WP_stats_enet_multi_class_intsw intsw_mpe_stat;
   WP_stats_enet_multi_class_tx tx_stats;
   WP_bport_stats iwport_stats = { 0 };

   /* fa reset*/
   for (ii = 0; ii < MAX_ENETS; ii++)
   {
//      status = WP_IwFlowStatisticsReset(extsw_fa_xgi1[ii], WP_IW_FLOW_STAT_INTERNAL_SWITCHING, &intsw_flow_stats);
//      WPE_TerminateOnError(status, "WP_IwFlowStatisticsReset()", __LINE__);

      status = WP_IwFlowStatisticsReset(extsw_fa_xgi2[ii], WP_IW_FLOW_STAT_GENERIC, &extsw_flow_stats);
      WPE_TerminateOnError(status, "WP_IwFlowStatisticsReset()", __LINE__);

      status = WP_IwFlowStatisticsReset(mixed_fa[ii], WP_IW_FLOW_STAT_GENERIC, &extsw_flow_stats);
      WPE_TerminateOnError(status, "WP_IwFlowStatisticsReset()", __LINE__);

      status = WP_IwFlowStatisticsReset(cascading_fa[ii], WP_IW_FLOW_STAT_GENERIC, &extsw_flow_stats);
      WPE_TerminateOnError(status, "WP_IwFlowStatisticsReset()", __LINE__);

//      status = WP_IwFlowStatisticsReset(intsw_fa_w_pecs[ii], WP_IW_FLOW_STAT_INTERNAL_SWITCHING, &intsw_flow_stats);
//      WPE_TerminateOnError(status, "WP_IwFlowStatisticsReset()", __LINE__);

   }
   for (ii = 0; ii < 3; ii++)
   {
//      status = WP_IwFlowStatisticsReset(mpe_fa[ii], WP_IW_FLOW_STAT_INTERNAL_SWITCHING, &intsw_flow_stats);
//      WPE_TerminateOnError(status, "WP_IwFlowStatisticsReset()", __LINE__);
   }

   /* reset Host flow aggregation statistics */
//   status = WP_IwFlowStatisticsReset(host_flow_agg, WP_IW_FLOW_STAT_GENERIC, &extsw_flow_stats);
//   WPE_TerminateOnError(status, "WP_IwFlowStatisticsReset()", __LINE__);

   status = WP_DeviceStatisticsReset(xgi1_device_handle, &stats_enet);
   WPE_TerminateOnError(status, "WP_DeviceStatisticsReset()", __LINE__);

   status = WP_DeviceStatisticsReset(xgi2_device_handle, &stats_enet);
   WPE_TerminateOnError(status, "WP_DeviceStatisticsReset()", __LINE__);

   for (ii = 0; ii < MAX_CLASSES; ii++)
   {
      status = WP_DeviceStatisticsReset(intsw_class_handle[ii], &intsw_mpe_stat);
      WPE_TerminateOnError(status, "WP_DeviceStatisticsReset()", __LINE__);

//      status = WP_DeviceStatisticsReset(intsw_xgi2_class_handle[ii], &tx_stats);
//      WPE_TerminateOnError(status, "WP_DeviceStatisticsReset()", __LINE__);

      status = WP_DeviceStatisticsReset(mixed_class_handle[ii], &tx_stats);
      WPE_TerminateOnError(status, "WP_DeviceStatisticsReset()", __LINE__);
   }

   for (ii = 0; ii < NUM_OF_FILTERS; ii++)
   {
      status = WP_PceFilterStatisticsReset(xgi2_pce_filter[ii], &(xgi2_filter_stats[ii]));
      WPE_TerminateOnError(status, "WP_PceFilterStatisticsReset()", __LINE__);
   }
   status = WP_PceFilterStatisticsReset(xgi1_pce_filter[CLASSIFICATION], &(xgi1_filter_stats[CLASSIFICATION]));
   WPE_TerminateOnError(status, "WP_PceFilterStatisticsReset()", __LINE__);

   for (ii = 0; ii <= MAX_ENETS; ii++)
   {
      if (ii < MAX_ENETS)
      {
         status = WP_IwPortStatisticsReset(iwport_handle[ii], &iwport_stats);
         WPE_TerminateOnError(status, "WP_IwPortStatisticsReset", __LINE__);
      }
      else
      {
         status = WP_IwPortStatisticsReset(bport_handle[0], &iwport_stats);
         WPE_TerminateOnError(status, "WP_IwPortStatisticsReset", __LINE__);
         status = WP_IwPortStatisticsReset(bport_handle[1], &iwport_stats);
         WPE_TerminateOnError(status, "WP_IwPortStatisticsReset", __LINE__);
      }
   }

   if (policer_handle)
   {
      status = WP_PolicerStatisticsReset(policer_handle);
      WPE_TerminateOnError(status, "WP_PolicerStatisticsReset", __LINE__);
   }
}
/* WPE_TerminateOnError */
static void WPE_TerminateOnError(WP_handle handle, WP_CHAR *s, WP_U32 line)
{
   if (WP_ERROR_P(handle))
   {
      printf("Test Abort %s %s 0x%x\n", s, WP_ErrorString(handle), handle);
      printf("Test Failed in line -  %d. \n", line);
      WPE_Terminate(1);
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
#if REBOOT

   /* Release WDDI */
   WP_DriverRelease();

   /* For socket based simulators */
#ifdef USE_SOCKET
   CleanUpSocket(5);
#endif

   WPE_Reboot();
#else
   exit(1);
#endif
   if (status != 0)
      exit(status);
}
void WPE_Reboot(void)
{
#if !defined(__linux__)
   /* Rebooting is specific to board and target.  The test
    * directory should not be concerned how this is done.
    * Instead, it calls a board-specific function. */
   if (WPI_REBOOT_ENABLE)
   {
      printf("Test Rebooting winmon by WPE_Reboot\nWinMon>\n");
      WPX_Reboot();
   }
   else
      printf("Not Rebooting with WPE_Reboot\nWinMon>\n");
#else
   exit(0);
#endif
}

/****************************************************************************************************************************
 * Function name: main()
 * Description  : This function build the application according to the user choice.
 *        The user can build
 *        Main function will call different functions for initializing group's config and link's config.
 *        The menu will supply dynamic modifications.
 *        For testing, there is also a scenarios sub menu who contains testing scenarios.
 *        There is a Statistics sub menu, who include statistics of devices/groups/links/FA/PCEs/filters and more
 *
 * Input  params: none
 * Output params:
 * Return val   : none
 *****************************************************************************************************************************/
int main(void)
{
   WP_status status;

   WPE_SystemSetup();

//   WPE_CreateMultiClassDevices();

   status = WP_SysCommit();
   WPE_TerminateOnError(status, "WP_SysCommit", __LINE__);

//   WPU_DebugEventGroupLoad(WP_WINPATH(DEFAULT_WPID), -1, "INTSW_MODULE_ENTRIES");
//   WPU_DebugEventGroupLoad(WP_WINPATH(DEFAULT_WPID), -1, "INTSW_MODULE_EXITS");
//   WPU_DebugEventGroupLoad(WP_WINPATH(DEFAULT_WPID), -1, "INTSW_FLOW");
//   WPU_DebugEventGroupLoad(WP_WINPATH(DEFAULT_WPID), -1, "INTSW_SUBMODULE_ENTRIES");
//   WPU_DebugEventGroupLoad(WP_WINPATH(DEFAULT_WPID), -1, "INTSW_SUBMODULE_EXITS");
//   WPU_DebugEventGroupLoad(WP_WINPATH(DEFAULT_WPID), -1, "DEBUG_STATISTICS");

   WPE_ChannelsCreate();

   WPE_IWSystemCreate();

   WPE_Initialize_Pce_Interface();

   WPE_SystemEnable();

   WPE_CreateIwPorts();

   WPE_CreateFlowAggregation();

//   WPE_CreateIngressPolicers();
//   WPE_Initialize_ExtSw_Rules();

   WPE_Saves_Initial_Qnode_Status();

   /* Menu for testing the application*/
   while (1)
   {
      WPE_Main_Menu();
   }

   return 0;
}

static void WPE_Main_Menu()
{
   WP_U32 res, cmd;
   WP_CHAR tmp[300];

   WP_CHAR menu_com[50];

   printf("\nChoose from the Menu below:\n");
   printf("1. Dynamic modifications\n");
   printf("2. Statistics\n");
   printf("3. Scenarios\n");
   printf("4. Quit without reboot\n");
   printf("5. Quit\n");
   printf("d. Display Debug printing (Winpath3 only)\n");
   printf("s. Display winutil sysu (Winpath3 only)\n");
   printf("t. Display winutil trsu (Winpath3 only)\n");
   printf("Enter your command:\n");

   gets(menu_com);
   printf("\n\n");

   res = sscanf(menu_com, "%d %s", &cmd, tmp);

   if (strlen(menu_com) > 1)
   {
      printf("too many numbers were entered\n\n");
      return;
   }
   if (strcmp(menu_com, "\n") == 0)
   {
      return;
   }

   switch (cmd)
   {
      case 1: /* Dynamic modifications */
      {
         WPE_Dynamic_Modifications();
         break;
      }
      case 2: /* Statstics */
      {
         WPE_Statistics();
         break;
      }
      case 3: /* Scenarios */
      {
         WPE_Scenarios();
         break;
      }
      case 4: /* Quit without reebot */
      {
         WPE_Quit_No_Reboot();
         break;
      }
      case 5: /* Quit */
      {
         WPE_Quit();
         break;
      }
      default:
      {
         if (cmd == 0)
         {
            if (strcmp(menu_com, "d") == 0 || strcmp(menu_com, "D") == 0)
            {
#ifdef WP_HW_WINPATH3
               WPU_DebugSwLogDisplay(WP_WINPATH(DEFAULT_WPID),
                     NULL,
                     100);
#endif
            }
            else
               if (strcmp(menu_com, "t") == 0 || strcmp(menu_com, "T") == 0)
               {
                  WPU_WinutilCall("trsu");
               }
               else
                  if (strcmp(menu_com, "s") == 0 || strcmp(menu_com, "S") == 0)
                  {
                     WPU_WinutilCall("sysu");
                  }
                  else
                  {
                     printf("wrong number was entered\n\n");
                  }
         }
         else
         {
            printf("wrong number was entered\n\n");
         }
         return;
      }
   }
}
static void WPE_Dynamic_Modifications(void)
{
   WP_U32 res, cmd;
   WP_U32 param1, param2, param3;
   WP_CHAR tmp[300];
   WP_CHAR StrPrm[100];

   while (1)
   {
      printf("\nChoose from the Menu below:\n");
      printf("1. Change IntSw <==> ExtSw\n");
      printf("2. Modify FAs MTU\n");
      printf("3. Modify Ingress policer\n");
      printf("4. Modify FAs PECS\n");
      printf("5. Smart Discard -Enter:1.Enable,0.Disable and enter the Threshold\n");
      printf("6. Device Disable/Enable:for XGI1 Rx-0,XGI1 Tx-1, for XGI2 Rx-2, XGI2 Tx-3, for Intsw - 4 for Disable - 0, for Enable - 1\n");
      printf("7. Device Disable Enable:for XGI1 Rx-0,XGI1 Tx-1, for XGI2 Rx-2,for Intsw - 3 and num of loops\n");
      printf("!.Upper Menu\n");
      printf("Enter your command:\n");

      gets(StrPrm);
      printf("\n\n");

      res = sscanf(StrPrm, "%d %d %d %d %s", &cmd, &param1, &param2, &param3, tmp);
      switch (cmd)
      {
         case 1:
         {
            WPE_ChangeSwitching();
            break;
         }
         case 2:
         {
            WPE_ModifyMtu(param1);
            break;
         }
         case 3:
         {
            WPE_ModifyPolicer_Menu();
            break;
         }
         case 4:
         {
            WPE_ModifyPecs_Menu();
            break;
         }
         case 5:
         {
            WPE_SmartDiscard(param1,param2);
            break;
         }
         case 6:
         {
            WPE_DeviceDisable(param1,param2);
            break;
         }
         case 7:
         {
            WPE_DeviceDisableLoop(param1,param2);
            break;
         }
         default:
         {
            if (StrPrm[0] != '!')
               printf("\nWrong number was entered\n\n");
         }

      } /* switch */

      if (StrPrm[0] == '!')
         break;

      cmd = 0;
   }
}
static void WPE_ModifyPecs_Menu(void)
{
   WP_U32 res, cmd;
   WP_U32 param1, param2, param3;
   WP_CHAR tmp[300];
   WP_CHAR StrPrm[100];

   while (1)
   {
      printf("\nChoose from the Menu below The PECS Modifications Rates:\n");
      printf("1. Disable PECS\n");
      printf("2. Replace MAC DA\n");
      printf("3. Replace MAC SA\n");
      printf("4. Replace both MACs\n");
      printf("5. Enable PECS with no Vlan Change\n");
      printf("6. Enable PECS with Vlan Edit Untagged -> Double tagged \n");
      printf("7. Enable PECS with Vlan Edit Untagged -> Double tagged + Prefix Insertion\n");
      printf("8. Enable PECS with Sub port Vlan add\n");
      printf("9. Enable PECS with Vlan Edit Untagged -> Double tagged + Subport Vlan\n");
      printf("10.Enable PECS with Vlan Edit Untagged -> Double tagged + Prefix Insertion + Subport Vlan\n");
      printf("!.Upper Menu\n");
      printf("Enter your command:\n");

      gets(StrPrm);
      printf("\n\n");

      res = sscanf(StrPrm, "%d %d %d %d %s", &cmd, &param1, &param2, &param3, tmp);

      switch (cmd)
      {
         case 1:
         {
            WPE_ModifyPecs(WP_DISABLE,0);
            break;
         }
         case 2:
         {
            WPE_ModifyPecs(WP_ENABLE,0);
            break;
         }
         case 3:
         {
            WPE_ModifyPecs(WP_ENABLE,1);
            break;
         }
         case 4:
         {
            WPE_ModifyPecs(WP_ENABLE,2);
            break;
         }
         case 5:
         {
            WPE_ModifyPecs(WP_ENABLE,3);
            break;
         }
         case 6:
         {
            WPE_ModifyPecs(WP_ENABLE,4);
            break;
         }
         case 7:
         {
            WPE_ModifyPecs(WP_ENABLE,5);
            break;
         }
         case 8:
         {
            WPE_ModifyPecs(WP_ENABLE,6);
            break;
         }
         case 9:
         {
            WPE_ModifyPecs(WP_ENABLE,7);
            break;
         }
         case 10:
         {
            WPE_ModifyPecs(WP_ENABLE,8);
            break;
         }
         default:
         {
            if (StrPrm[0] != '!')
               printf("\nWrong number was entered\n\n");
         }

      } /* switch */

      if (StrPrm[0] == '!')
         break;

      cmd = 0;
   }
}
/* operation = 0 MAC DA REPLACE
 * operation = 1 MAC SA REPLACE
 * operation = 2 Both MAC replacement
 * operation = 3 Vlan unchanged
 * operation = 4 Vlan edit */
static void WPE_ModifyPecs(WP_boolean enable_disable_pecs,WP_U32 operation)
{
   WPE_gpe_brouter_pecs_flow_info is_brouter_pecs_user_info[] =
   {
      {
         /* header_extract_mode */        WPE_GPE_BROUTER_PECS_EXTRACT_DISABLE,
         /* extraction_size */            0,
         /* prefix_add_mode */            WPE_GPE_BROUTER_PECS_ADD_DISABLE,
         /* prefix_add_size */            0,
         /* prefix_ip_offset */           0,
         /* mac_replace_mode */           WPE_GPE_BROUTER_PECS_REPLACE_MAC_DISABLE,
         /* mac_da */                     {0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f},
         /* mac_sa */                     {0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
         /* vlan_edit_options */          0,
         /* prefix_remark_options */      {0,0,0,0},
         /* ttl_mode */                   WPE_GPE_BROUTER_PECS_TTL_DISABLE,
         /* tos_remarking_mode */         WPE_GPE_BROUTER_PECS_TOS_REMARKING_DISABLE,
         /* Prefix[84] */                {
                                          0x01,0x02,0x03,0x04,0x05,0x06,
                                          0x00,0x00,0x00,0x00,0x00,0x00,
                                          0,0,0,0,0,0,0,0,0,0,
                                          0,0,0,0,0,0,0,0,0,0,
                                         }
      }
   };
   WPE_vlan_edit_options vlan_edit_options;

   WP_iw_agg_generic tx_agg_gen[1];
   WP_U32 fa_index;
   WP_status status;

   tx_agg_gen[0].pecs_flow_info = is_brouter_pecs_user_info;
   tx_agg_gen[0].pecs_handle = pecs_handle[BROUTER_PECS];

   if (enable_disable_pecs)
   {
      switch(operation)
      {
         case 0:
         {
            is_brouter_pecs_user_info[0].mac_replace_mode = WPE_GPE_BROUTER_PECS_REPLACE_MAC_DA;
            for (fa_index=0; fa_index<MAX_CLASSES; fa_index++)
            {
               /* XGI1 */
               status = WP_IwFlowAggregationModify(extsw_fa_xgi1[fa_index],WP_IW_FLOW_AGG_GENERIC_MOD_PECS,tx_agg_gen);
               WPE_TerminateOnError(status, "WP_IwFlowAggregationModify() ", __LINE__);

               /* XGI2 */
               status = WP_IwFlowAggregationModify(extsw_fa_xgi2[fa_index],WP_IW_FLOW_AGG_GENERIC_MOD_PECS,tx_agg_gen);
               WPE_TerminateOnError(status, "WP_IwFlowAggregationModify() ", __LINE__);
            }
            break;
         }
         case 1:
         {
            is_brouter_pecs_user_info[0].mac_replace_mode = WPE_GPE_BROUTER_PECS_REPLACE_MAC_SA;
            for (fa_index=0; fa_index<MAX_CLASSES; fa_index++)
            {
               /* XGI1 */
               status = WP_IwFlowAggregationModify(extsw_fa_xgi1[fa_index],WP_IW_FLOW_AGG_GENERIC_MOD_PECS,tx_agg_gen);
               WPE_TerminateOnError(status, "WP_IwFlowAggregationModify() ", __LINE__);

               /* XGI2 */
               status = WP_IwFlowAggregationModify(extsw_fa_xgi2[fa_index],WP_IW_FLOW_AGG_GENERIC_MOD_PECS,tx_agg_gen);
               WPE_TerminateOnError(status, "WP_IwFlowAggregationModify() ", __LINE__);

            }
            break;
         }
         case 2:
         {
            is_brouter_pecs_user_info[0].mac_replace_mode = WPE_GPE_BROUTER_PECS_REPLACE_MAC_DA_AND_SA;
            for (fa_index=0; fa_index<MAX_CLASSES; fa_index++)
            {
               /* XGI1 */
               status = WP_IwFlowAggregationModify(extsw_fa_xgi1[fa_index],WP_IW_FLOW_AGG_GENERIC_MOD_PECS,tx_agg_gen);
               WPE_TerminateOnError(status, "WP_IwFlowAggregationModify() ", __LINE__);

               /* XGI2 */
               status = WP_IwFlowAggregationModify(extsw_fa_xgi2[fa_index],WP_IW_FLOW_AGG_GENERIC_MOD_PECS,tx_agg_gen);
               WPE_TerminateOnError(status, "WP_IwFlowAggregationModify() ", __LINE__);
            }
            break;
         }
         case 3:
         {
            memset(&vlan_edit_options,0,sizeof(WPE_vlan_edit_options));

            vlan_edit_options.egress_rule[WPE_GPE_BROUTER_PECS_INPUT_VLAN_S_PLUS_S_TAGGED] = WPE_GPE_BROUTER_PECS_VLAN_EGRESS_RULE_STACKED;
            vlan_edit_options.egress_rule[WPE_GPE_BROUTER_PECS_INPUT_VLAN_S_PLUS_C_TAGGED] = WPE_GPE_BROUTER_PECS_VLAN_EGRESS_RULE_STACKED;
            vlan_edit_options.egress_rule[WPE_GPE_BROUTER_PECS_INPUT_VLAN_STAGGED] = WPE_GPE_BROUTER_PECS_VLAN_EGRESS_RULE_TAGGED;
            vlan_edit_options.egress_rule[WPE_GPE_BROUTER_PECS_INPUT_VLAN_CTAGGED] = WPE_GPE_BROUTER_PECS_VLAN_EGRESS_RULE_TAGGED;
            vlan_edit_options.egress_rule[WPE_GPE_BROUTER_PECS_INPUT_VLAN_PRIORITY_TAGGED] = WPE_GPE_BROUTER_PECS_VLAN_EGRESS_RULE_TAGGED;
            vlan_edit_options.egress_rule[WPE_GPE_BROUTER_PECS_INPUT_VLAN_UNTAGGED] = WPE_GPE_BROUTER_PECS_VLAN_EGRESS_RULE_UNTAGGED;
            vlan_edit_options.int_vlan_etype_source = WPE_GPE_BROUTER_PECS_VLAN_ETYPE_UNCHANGED;
            vlan_edit_options.ext_vlan_etype_source = WPE_GPE_BROUTER_PECS_VLAN_ETYPE_UNCHANGED;
            vlan_edit_options.replace_int_vlan_mode = WPE_GPE_BROUTER_PECS_REPLACE_VLAN_DISABLE;
            vlan_edit_options.replace_ext_vlan_mode = WPE_GPE_BROUTER_PECS_REPLACE_VLAN_DISABLE;
            vlan_edit_options.replace_sub_port_vlan_mode = WPE_GPE_BROUTER_PECS_SUB_PORT_VLAN_DISABLE;

            is_brouter_pecs_user_info[0].vlan_edit_options = &vlan_edit_options;
            is_brouter_pecs_user_info[0].prefix_add_size = 0;
            is_brouter_pecs_user_info[0].header_extract_mode = WPE_GPE_BROUTER_PECS_EXTRACT_DISABLE;
            for (fa_index=0; fa_index<MAX_CLASSES; fa_index++)
            {
               /* XGI1 */
               status = WP_IwFlowAggregationModify(extsw_fa_xgi1[fa_index],WP_IW_FLOW_AGG_GENERIC_MOD_PECS,tx_agg_gen);
               WPE_TerminateOnError(status, "WP_IwFlowAggregationModify() ", __LINE__);

               /* XGI2 */
               status = WP_IwFlowAggregationModify(extsw_fa_xgi2[fa_index],WP_IW_FLOW_AGG_GENERIC_MOD_PECS,tx_agg_gen);
               WPE_TerminateOnError(status, "WP_IwFlowAggregationModify() ", __LINE__);
            }
            break;
         }
         case 4:
         {
            memset(&vlan_edit_options,0,sizeof(WPE_vlan_edit_options));

            vlan_edit_options.egress_rule[WPE_GPE_BROUTER_PECS_INPUT_VLAN_S_PLUS_S_TAGGED] = WPE_GPE_BROUTER_PECS_VLAN_EGRESS_RULE_STACKED;
            vlan_edit_options.egress_rule[WPE_GPE_BROUTER_PECS_INPUT_VLAN_S_PLUS_C_TAGGED] = WPE_GPE_BROUTER_PECS_VLAN_EGRESS_RULE_STACKED;
            vlan_edit_options.egress_rule[WPE_GPE_BROUTER_PECS_INPUT_VLAN_STAGGED] = WPE_GPE_BROUTER_PECS_VLAN_EGRESS_RULE_TAGGED;
            vlan_edit_options.egress_rule[WPE_GPE_BROUTER_PECS_INPUT_VLAN_CTAGGED] = WPE_GPE_BROUTER_PECS_VLAN_EGRESS_RULE_TAGGED;
            vlan_edit_options.egress_rule[WPE_GPE_BROUTER_PECS_INPUT_VLAN_PRIORITY_TAGGED] = WPE_GPE_BROUTER_PECS_VLAN_EGRESS_RULE_TAGGED;
            vlan_edit_options.egress_rule[WPE_GPE_BROUTER_PECS_INPUT_VLAN_UNTAGGED] = WPE_GPE_BROUTER_PECS_VLAN_EGRESS_RULE_STACKED;
            vlan_edit_options.int_vlan_etype_source = WPE_GPE_BROUTER_PECS_VLAN_ETYPE_IW_PORT_STAG;
            vlan_edit_options.ext_vlan_etype_source = WPE_GPE_BROUTER_PECS_VLAN_ETYPE_8100;
            vlan_edit_options.replace_int_vlan_mode = WPE_GPE_BROUTER_PECS_REPLACE_VLAN_ID;
            vlan_edit_options.int_vlan_tag = 222;
            vlan_edit_options.ext_vlan_tag = 111;
            vlan_edit_options.replace_ext_vlan_mode = WPE_GPE_BROUTER_PECS_REPLACE_VLAN_ID;
            vlan_edit_options.replace_sub_port_vlan_mode = WPE_GPE_BROUTER_PECS_SUB_PORT_VLAN_DISABLE;

            is_brouter_pecs_user_info[0].vlan_edit_options = &vlan_edit_options;
            is_brouter_pecs_user_info[0].prefix_add_size = 0;
            is_brouter_pecs_user_info[0].header_extract_mode = WPE_GPE_BROUTER_PECS_EXTRACT_DISABLE;
            for (fa_index=0; fa_index<MAX_CLASSES; fa_index++)
            {
               /* XGI1 */
               status = WP_IwFlowAggregationModify(extsw_fa_xgi1[fa_index],WP_IW_FLOW_AGG_GENERIC_MOD_PECS,tx_agg_gen);
               WPE_TerminateOnError(status, "WP_IwFlowAggregationModify() ", __LINE__);

               /* XGI2 */
               status = WP_IwFlowAggregationModify(extsw_fa_xgi2[fa_index],WP_IW_FLOW_AGG_GENERIC_MOD_PECS,tx_agg_gen);
               WPE_TerminateOnError(status, "WP_IwFlowAggregationModify() ", __LINE__);
            }
            break;
         }
         case 5:
         {
            memset(&vlan_edit_options,0,sizeof(WPE_vlan_edit_options));

            vlan_edit_options.egress_rule[WPE_GPE_BROUTER_PECS_INPUT_VLAN_S_PLUS_S_TAGGED] = WPE_GPE_BROUTER_PECS_VLAN_EGRESS_RULE_STACKED;
            vlan_edit_options.egress_rule[WPE_GPE_BROUTER_PECS_INPUT_VLAN_S_PLUS_C_TAGGED] = WPE_GPE_BROUTER_PECS_VLAN_EGRESS_RULE_STACKED;
            vlan_edit_options.egress_rule[WPE_GPE_BROUTER_PECS_INPUT_VLAN_STAGGED] = WPE_GPE_BROUTER_PECS_VLAN_EGRESS_RULE_TAGGED;
            vlan_edit_options.egress_rule[WPE_GPE_BROUTER_PECS_INPUT_VLAN_CTAGGED] = WPE_GPE_BROUTER_PECS_VLAN_EGRESS_RULE_TAGGED;
            vlan_edit_options.egress_rule[WPE_GPE_BROUTER_PECS_INPUT_VLAN_PRIORITY_TAGGED] = WPE_GPE_BROUTER_PECS_VLAN_EGRESS_RULE_TAGGED;
            vlan_edit_options.egress_rule[WPE_GPE_BROUTER_PECS_INPUT_VLAN_UNTAGGED] = WPE_GPE_BROUTER_PECS_VLAN_EGRESS_RULE_STACKED;
            vlan_edit_options.int_vlan_etype_source = WPE_GPE_BROUTER_PECS_VLAN_ETYPE_IW_PORT_STAG;
            vlan_edit_options.ext_vlan_etype_source = WPE_GPE_BROUTER_PECS_VLAN_ETYPE_8100;
            vlan_edit_options.replace_int_vlan_mode = WPE_GPE_BROUTER_PECS_REPLACE_VLAN_ID;
            vlan_edit_options.int_vlan_tag = 222;
            vlan_edit_options.ext_vlan_tag = 111;
            vlan_edit_options.replace_ext_vlan_mode = WPE_GPE_BROUTER_PECS_REPLACE_VLAN_ID;
            vlan_edit_options.replace_sub_port_vlan_mode = WPE_GPE_BROUTER_PECS_SUB_PORT_VLAN_DISABLE;

            is_brouter_pecs_user_info[0].vlan_edit_options = &vlan_edit_options;
            is_brouter_pecs_user_info[0].prefix_add_mode = WPE_GPE_BROUTER_PECS_ADD_L2_HEADER;
            is_brouter_pecs_user_info[0].prefix_add_size = 12;
            is_brouter_pecs_user_info[0].prefix[10] = scenario;
            for (fa_index=0; fa_index<MAX_CLASSES; fa_index++)
            {
               is_brouter_pecs_user_info[0].prefix[11] = fa_index;
               /* XGI1 */
               status = WP_IwFlowAggregationModify(extsw_fa_xgi1[fa_index],WP_IW_FLOW_AGG_GENERIC_MOD_PECS,tx_agg_gen);
               WPE_TerminateOnError(status, "WP_IwFlowAggregationModify() ", __LINE__);

//               is_brouter_pecs_user_info[0].prefix_add_mode = WPE_GPE_BROUTER_PECS_ADD_DISABLE;
//               is_brouter_pecs_user_info[0].prefix_add_size = 0;
//               is_brouter_pecs_user_info[0].mac_replace_mode = WPE_GPE_BROUTER_PECS_REPLACE_MAC_DA_AND_SA;
//               is_brouter_pecs_user_info[0].mac_sa[4] = scenario;
//               is_brouter_pecs_user_info[0].mac_sa[5] = fa_index;
               /* XGI2 */
               status = WP_IwFlowAggregationModify(extsw_fa_xgi2[fa_index],WP_IW_FLOW_AGG_GENERIC_MOD_PECS,tx_agg_gen);
               WPE_TerminateOnError(status, "WP_IwFlowAggregationModify() ", __LINE__);
            }
            break;
         }
         case 6:
         {
            memset(&vlan_edit_options,0,sizeof(WPE_vlan_edit_options));

            vlan_edit_options.egress_rule[WPE_GPE_BROUTER_PECS_INPUT_VLAN_S_PLUS_S_TAGGED] = WPE_GPE_BROUTER_PECS_VLAN_EGRESS_RULE_STACKED;
            vlan_edit_options.egress_rule[WPE_GPE_BROUTER_PECS_INPUT_VLAN_S_PLUS_C_TAGGED] = WPE_GPE_BROUTER_PECS_VLAN_EGRESS_RULE_STACKED;
            vlan_edit_options.egress_rule[WPE_GPE_BROUTER_PECS_INPUT_VLAN_STAGGED] = WPE_GPE_BROUTER_PECS_VLAN_EGRESS_RULE_TAGGED;
            vlan_edit_options.egress_rule[WPE_GPE_BROUTER_PECS_INPUT_VLAN_CTAGGED] = WPE_GPE_BROUTER_PECS_VLAN_EGRESS_RULE_TAGGED;
            vlan_edit_options.egress_rule[WPE_GPE_BROUTER_PECS_INPUT_VLAN_PRIORITY_TAGGED] = WPE_GPE_BROUTER_PECS_VLAN_EGRESS_RULE_TAGGED;
            vlan_edit_options.egress_rule[WPE_GPE_BROUTER_PECS_INPUT_VLAN_UNTAGGED] = WPE_GPE_BROUTER_PECS_VLAN_EGRESS_RULE_UNTAGGED;
            vlan_edit_options.int_vlan_etype_source = WPE_GPE_BROUTER_PECS_VLAN_ETYPE_UNCHANGED;
            vlan_edit_options.ext_vlan_etype_source = WPE_GPE_BROUTER_PECS_VLAN_ETYPE_UNCHANGED;
            vlan_edit_options.replace_int_vlan_mode = WPE_GPE_BROUTER_PECS_REPLACE_VLAN_DISABLE;
            vlan_edit_options.replace_ext_vlan_mode = WPE_GPE_BROUTER_PECS_REPLACE_VLAN_DISABLE;

            vlan_edit_options.replace_sub_port_vlan_mode = WPE_GPE_BROUTER_PECS_SUB_PORT_VLAN_ADD;
            vlan_edit_options.sub_port_vlan_tag = 0x1234;

            is_brouter_pecs_user_info[0].vlan_edit_options = &vlan_edit_options;
            is_brouter_pecs_user_info[0].prefix_add_size = 0;
            is_brouter_pecs_user_info[0].header_extract_mode = WPE_GPE_BROUTER_PECS_EXTRACT_DISABLE;
            for (fa_index=0; fa_index<MAX_CLASSES; fa_index++)
            {
               /* XGI1 */
               status = WP_IwFlowAggregationModify(extsw_fa_xgi1[fa_index],WP_IW_FLOW_AGG_GENERIC_MOD_PECS,tx_agg_gen);
               WPE_TerminateOnError(status, "WP_IwFlowAggregationModify() ", __LINE__);

               /* XGI2 */
               status = WP_IwFlowAggregationModify(extsw_fa_xgi2[fa_index],WP_IW_FLOW_AGG_GENERIC_MOD_PECS,tx_agg_gen);
               WPE_TerminateOnError(status, "WP_IwFlowAggregationModify() ", __LINE__);
            }
            break;
         }
         case 7:
         {
            memset(&vlan_edit_options,0,sizeof(WPE_vlan_edit_options));

            vlan_edit_options.egress_rule[WPE_GPE_BROUTER_PECS_INPUT_VLAN_S_PLUS_S_TAGGED] = WPE_GPE_BROUTER_PECS_VLAN_EGRESS_RULE_STACKED;
            vlan_edit_options.egress_rule[WPE_GPE_BROUTER_PECS_INPUT_VLAN_S_PLUS_C_TAGGED] = WPE_GPE_BROUTER_PECS_VLAN_EGRESS_RULE_STACKED;
            vlan_edit_options.egress_rule[WPE_GPE_BROUTER_PECS_INPUT_VLAN_STAGGED] = WPE_GPE_BROUTER_PECS_VLAN_EGRESS_RULE_TAGGED;
            vlan_edit_options.egress_rule[WPE_GPE_BROUTER_PECS_INPUT_VLAN_CTAGGED] = WPE_GPE_BROUTER_PECS_VLAN_EGRESS_RULE_TAGGED;
            vlan_edit_options.egress_rule[WPE_GPE_BROUTER_PECS_INPUT_VLAN_PRIORITY_TAGGED] = WPE_GPE_BROUTER_PECS_VLAN_EGRESS_RULE_TAGGED;
            vlan_edit_options.egress_rule[WPE_GPE_BROUTER_PECS_INPUT_VLAN_UNTAGGED] = WPE_GPE_BROUTER_PECS_VLAN_EGRESS_RULE_STACKED;
            vlan_edit_options.int_vlan_etype_source = WPE_GPE_BROUTER_PECS_VLAN_ETYPE_IW_PORT_STAG;
            vlan_edit_options.ext_vlan_etype_source = WPE_GPE_BROUTER_PECS_VLAN_ETYPE_8100;
            vlan_edit_options.replace_int_vlan_mode = WPE_GPE_BROUTER_PECS_REPLACE_VLAN_ID;
            vlan_edit_options.int_vlan_tag = 222;
            vlan_edit_options.ext_vlan_tag = 111;
            vlan_edit_options.replace_ext_vlan_mode = WPE_GPE_BROUTER_PECS_REPLACE_VLAN_ID;

            vlan_edit_options.replace_sub_port_vlan_mode = WPE_GPE_BROUTER_PECS_SUB_PORT_VLAN_ADD;
            vlan_edit_options.sub_port_vlan_tag = 0x1234;

            is_brouter_pecs_user_info[0].vlan_edit_options = &vlan_edit_options;
            is_brouter_pecs_user_info[0].prefix_add_size = 0;
            for (fa_index=0; fa_index<MAX_CLASSES; fa_index++)
            {
               /* XGI1 */
               status = WP_IwFlowAggregationModify(extsw_fa_xgi1[fa_index],WP_IW_FLOW_AGG_GENERIC_MOD_PECS,tx_agg_gen);
               WPE_TerminateOnError(status, "WP_IwFlowAggregationModify() ", __LINE__);

               /* XGI2 */
               status = WP_IwFlowAggregationModify(extsw_fa_xgi2[fa_index],WP_IW_FLOW_AGG_GENERIC_MOD_PECS,tx_agg_gen);
               WPE_TerminateOnError(status, "WP_IwFlowAggregationModify() ", __LINE__);
            }
            break;
         }
         case 8:
         {
            memset(&vlan_edit_options,0,sizeof(WPE_vlan_edit_options));

            vlan_edit_options.egress_rule[WPE_GPE_BROUTER_PECS_INPUT_VLAN_S_PLUS_S_TAGGED] = WPE_GPE_BROUTER_PECS_VLAN_EGRESS_RULE_STACKED;
            vlan_edit_options.egress_rule[WPE_GPE_BROUTER_PECS_INPUT_VLAN_S_PLUS_C_TAGGED] = WPE_GPE_BROUTER_PECS_VLAN_EGRESS_RULE_STACKED;
            vlan_edit_options.egress_rule[WPE_GPE_BROUTER_PECS_INPUT_VLAN_STAGGED] = WPE_GPE_BROUTER_PECS_VLAN_EGRESS_RULE_TAGGED;
            vlan_edit_options.egress_rule[WPE_GPE_BROUTER_PECS_INPUT_VLAN_CTAGGED] = WPE_GPE_BROUTER_PECS_VLAN_EGRESS_RULE_TAGGED;
            vlan_edit_options.egress_rule[WPE_GPE_BROUTER_PECS_INPUT_VLAN_PRIORITY_TAGGED] = WPE_GPE_BROUTER_PECS_VLAN_EGRESS_RULE_TAGGED;
            vlan_edit_options.egress_rule[WPE_GPE_BROUTER_PECS_INPUT_VLAN_UNTAGGED] = WPE_GPE_BROUTER_PECS_VLAN_EGRESS_RULE_STACKED;
            vlan_edit_options.int_vlan_etype_source = WPE_GPE_BROUTER_PECS_VLAN_ETYPE_IW_PORT_STAG;
            vlan_edit_options.ext_vlan_etype_source = WPE_GPE_BROUTER_PECS_VLAN_ETYPE_8100;
            vlan_edit_options.replace_int_vlan_mode = WPE_GPE_BROUTER_PECS_REPLACE_VLAN_ID;
            vlan_edit_options.int_vlan_tag = 222;
            vlan_edit_options.ext_vlan_tag = 111;
            vlan_edit_options.replace_ext_vlan_mode = WPE_GPE_BROUTER_PECS_REPLACE_VLAN_ID;

            vlan_edit_options.replace_sub_port_vlan_mode = WPE_GPE_BROUTER_PECS_SUB_PORT_VLAN_ADD;
            vlan_edit_options.sub_port_vlan_tag = 0x1234;

            is_brouter_pecs_user_info[0].vlan_edit_options = &vlan_edit_options;
            is_brouter_pecs_user_info[0].prefix_add_mode = WPE_GPE_BROUTER_PECS_ADD_L2_HEADER;
            is_brouter_pecs_user_info[0].prefix_add_size = 12;
            is_brouter_pecs_user_info[0].prefix[10] = scenario;
            for (fa_index=0; fa_index<MAX_CLASSES; fa_index++)
            {
               is_brouter_pecs_user_info[0].prefix[11] = fa_index;
               /* XGI1 */
               status = WP_IwFlowAggregationModify(extsw_fa_xgi1[fa_index],WP_IW_FLOW_AGG_GENERIC_MOD_PECS,tx_agg_gen);
               WPE_TerminateOnError(status, "WP_IwFlowAggregationModify() ", __LINE__);

//               is_brouter_pecs_user_info[0].prefix_add_mode = WPE_GPE_BROUTER_PECS_ADD_DISABLE;
//               is_brouter_pecs_user_info[0].prefix_add_size = 0;
//               is_brouter_pecs_user_info[0].mac_replace_mode = WPE_GPE_BROUTER_PECS_REPLACE_MAC_DA_AND_SA;
//               is_brouter_pecs_user_info[0].mac_sa[4] = scenario;
//               is_brouter_pecs_user_info[0].mac_sa[5] = fa_index;
               /* XGI2 */
               status = WP_IwFlowAggregationModify(extsw_fa_xgi2[fa_index],WP_IW_FLOW_AGG_GENERIC_MOD_PECS,tx_agg_gen);
               WPE_TerminateOnError(status, "WP_IwFlowAggregationModify() ", __LINE__);
            }
            break;
         }
         default:
         {

         }
      }
   }
   else
   {
      for (fa_index=0; fa_index<MAX_CLASSES; fa_index++)
      {
         status = WP_IwFlowAggregationModify(extsw_fa_xgi1[fa_index],WP_IW_FLOW_AGG_GENERIC_MOD_PECS,tx_agg_gen);
         WPE_TerminateOnError(status, "WP_IwFlowAggregationModify() ", __LINE__);

         status = WP_IwFlowAggregationModify(extsw_fa_xgi2[fa_index],WP_IW_FLOW_AGG_GENERIC_MOD_PECS,tx_agg_gen);
         WPE_TerminateOnError(status, "WP_IwFlowAggregationModify() ", __LINE__);
      }
   }

}
static void WPE_ModifyPolicer_Menu(void)
{
   WP_U32 res, cmd;
   WP_U32 param1, param2;
   WP_CHAR tmp[300];
   WP_CHAR StrPrm[100];

   WP_policer_v2 pv2 =
   {
   /* method */WP_POLICER_METHOD_MEF_WITH_COUPLING_FLAG/*WP_POLICER_METHOD_SINGLE_LEAKY_BUCKET*/,
#ifdef WP_BOARD_WDS3_SL
         /* type */WP_POLICER_V2_TYPE_EXTERNAL,
#else
         /* type */WP_POLICER_V2_TYPE_INTERNAL,
#endif
         /* color_mode */WP_POLICER_COLOR_AWARE,
         /* red_packets_action */WP_POLICER_DROP_RED_PACKETS,
         /* cir */1,
         /* cbs */2,
         /* eir */3,
         /* ebs */4,
         /* policer_action */0,
         /* frame_len_correction */WP_POLICER_LENGTH_CORRECT_4B
   };
   WP_status status;

   while (1)
   {
      printf("\nChoose from the Menu below:\n");
      printf("1.Policer Method - Enter 0 for Dual bucket and 1 for Single bucket:\n");
      printf("2.Color mode - Enter 0 for Color aware and 1 for Color blind:\n");
      printf("3.Red packets action - Enter 0 for Drop red packets and 1 for Passing red Packets:\n");
      printf("4.Policer action - Enter 0 for Policer action and 1 for Without Policer Action:\n");
      printf("5.CIR/EIR - Enter CIR: Enter EIR (With a space between them) in Units of MEGA:\n");
      printf("6.Modify Policer! :\n");
      printf("!.Upper Menu\n");
      printf("Enter your command:\n");

      gets(StrPrm);
      printf("\n\n");

      res = sscanf(StrPrm, "%d %d %d %s", &cmd, &param1, &param2, tmp);
//      if(res != 3)
//      {
//         printf("Usage: <option> <1(Group Number),2(Link Number)> <object number>\n");
//         return;
//      }

      switch (cmd)
      {
         case 1:
         {
            if (param1)
               policer_method = 1;
            else
               policer_method = 0;
            break;
         }
         case 2:
         {
            if (param1)
               color_mode = 1;
            else
               color_mode = 0;
            break;
         }
         case 3:
         {
            if (param1)
               red_packets_action = 1;
            else
               red_packets_action = 0;
            break;
         }
         case 4:
         {
            if (param1)
               policer_action = 1;
            else
               policer_action = 0;
            break;
         }
         case 5:
         {
            cir = param1*1000000;
            eir = param2*1000000;
         }
         case 6:
         {
            /* this policer would be associated with 10Gbps RX interface */
            pv2.cir = cir /* 20% *//*325853658*/; /* 30% of 10Gbps */
            pv2.eir = eir/* 20% *//*551870912*/; /* 50% of 10Gbps */
            pv2.cbs = 131070; /* 3840 bytes */
            pv2.ebs = 131070;

            if (policer_action)
               pv2.policer_action = 0;
            else
               pv2.policer_action = papt_handle; /* Default */

            if (policer_method)
               pv2.method = WP_POLICER_METHOD_SINGLE_LEAKY_BUCKET;
            else
               pv2.method = WP_POLICER_METHOD_MEF_WITH_COUPLING_FLAG; /* Default */

            if (color_mode)
               pv2.color_mode = WP_POLICER_COLOR_BLIND;
            else
               pv2.color_mode = WP_POLICER_COLOR_AWARE; /* Default */

            if (red_packets_action)
               pv2.red_packets_action = WP_POLICER_PASS_RED_PACKETS;
            else
               pv2.red_packets_action = WP_POLICER_DROP_RED_PACKETS; /* Default */

            status = WP_PolicerModify(policer_handle,WP_POLICER_MOD_ALL,&pv2);
            WPE_TerminateOnError(status, "WP_PolicerModify() ", __LINE__);
            break;
         }
         default:
         {
            if (StrPrm[0] != '!')
               printf("\nWrong number was entered\n\n");
         }

      } /* switch */

      if (StrPrm[0] == '!')
         break;

      cmd = 0;
   }
}
static void WPE_ModifyMtu(WP_U32 mtu)
{
   WP_U32 ii;
   WP_status status;
//   WP_iw_agg_internal_switching tx_agg_intsw[1] =
//   {
//         {
//               /*tag*/0,
//               /*mode*/0,
//               /*txfunc*/0,
//               /*iw_port*/0,
//               /*error_pkt_mode*/0,
//               /*statmode*/WP_IW_STAT_ENABLE,
//               /*mtu*/3000,
//               /*pecs_handle*/0,
//               /*pecs_flow_info*/0
//         }
//   };

//   tx_agg_intsw[0].mtu = mtu;
   for (ii = 0 ; ii< MAX_CLASSES ; ii++)
   {

//       status = WP_IwFlowAggregationModify(extsw_fa_xgi1[ii],WP_IW_FLOW_AGG_MOD_MTU,&tx_agg_intsw[0]);
//       WPE_TerminateOnError(status, "WP_IwFlowAggregationModify() ", __LINE__);
//      WP_IW_FLOW_AGG_MOD_MTU
//      WP_IW_FLOW_AGG_B_MOD_BPORT
//
//      WP_IW_FLOW_AGG_MOD_IWPORT
//      WP_IW_FLOW_AGG_GENERIC_MOD_IW_PORT
//
//      WP_IW_FLOW_AGG_B_MOD_VID
//
//      WP_IW_FLOW_AGG_B_MOD_VPRIO
//
//      WP_IW_FLOW_AGG_B_MOD_VTAG_MODE
//
//      WP_IW_FLOW_AGG_B_MOD_REPLACE_VTAG
//
//      WP_IW_FLOW_AGG_B_MOD_VTAG
//
//      WP_IW_FLOW_AGG_B_MOD_EXT_VLAN
   }

}
static void WPE_SmartDiscard(WP_U32 enable,WP_U32 threshold)
{
   WP_syscmd_smart_drop_params syscmd_smart_drop_params;
   WP_U32 pt;
   WP_status status;
   if (enable)
   {
      /* Set all priorities */
      for (pt = 0; pt < 8 ; pt++)
      {
         syscmd_smart_drop_params.first_pkt_drop_level = 4096;
         syscmd_smart_drop_params.non_first_pkt_drop_level = 4096;
         syscmd_smart_drop_params.first_pkt_agentq_drop_level = 0;
         syscmd_smart_drop_params.non_first_pkt_agentq_drop_level = 0;
         syscmd_smart_drop_params.port = xgi1_port_handle;
         syscmd_smart_drop_params.priority = pt;
         syscmd_smart_drop_params.threshold_type = (WP_SYSCMD_FBP_THRESHOLD | WP_SYSCMD_AGENT_THRESHOLD);

//         status = WP_PortSmartDrop(xgi1_port_handle, WP_TRUE);
//         WPE_TerminateOnError(status,"Set smart drop enable",__LINE__);

         status = WP_SysCommand(WP_WINPATH(DEFAULT_WPID),
                                WP_SYSCMD_SMART_DROP_CONFIG,
                                (void*)&syscmd_smart_drop_params);
         WPE_TerminateOnError(status, "WP_SysCommand WP_SYSCMD_SMART_DROP_CONFIG",__LINE__);
      }
   }
   else
   {
      status = WP_PortSmartDrop(xgi1_port_handle, WP_FALSE);
      WPE_TerminateOnError(status,"Set smart drop enable",__LINE__);
   }

}
static void WPE_DeviceDisable(WP_U32 device,WP_boolean enable)
{

   WP_U32 ii;
   WP_status status;
   WP_port_enet port_cfg[1] =
   {
         {
               /* pkt_limits */
               { 100, 1, 0, 0, 0 },
               /* flowmode */WP_ENET_FMU_HIERARCHICAL_SHAPING_MODE,
               /* interface_mode *//*WP_ENET_SGMII_1000*/WP_ENET_SGMII_AN,
               /* rx_iw_bkgnd */WP_IW_BKGND_NOT_USED
         }
   };

   switch(device)
   {
      case 0:
      {
         if (enable)
         {
             status = WP_PortDisable(xgi1_port_handle, WP_DIRECTION_DUPLEX);
             WPE_TerminateOnError(status, "WP_PortEnable() Enet14", __LINE__);

             status = WP_DeviceDisable(xgi1_device_handle, WP_DIRECTION_DUPLEX);
             WPE_TerminateOnError(status, "WP_DeviceDisable enetdev", __LINE__);

             port_cfg[0].interface_mode = WP_ENET_SGMII_AN;
             status = WP_PortModify(xgi1_port_handle,
                                    WP_PORT_MOD_ENET_INTERFACE_MODE,
                                    &port_cfg);
             WPE_TerminateOnError(status, "WP_PortModify()", __LINE__);

             status = WP_PortEnable(xgi1_port_handle, WP_DIRECTION_DUPLEX);
             WPE_TerminateOnError(status, "WP_PortEnable() Enet14", __LINE__);

             status = WP_DeviceEnable(xgi1_device_handle, WP_DIRECTION_DUPLEX);
             WPE_TerminateOnError(status, "WP_DeviceEnable enetdev", __LINE__);
//            status = WP_DeviceEnable(xgi1_device_handle, WP_DIRECTION_DUPLEX);
//            WPE_TerminateOnError(status, "WP_DeviceEnable enetdev", __LINE__);
         }
         else
         {
             status = WP_PortDisable(xgi1_port_handle, WP_DIRECTION_DUPLEX);
             WPE_TerminateOnError(status, "WP_PortEnable() Enet14", __LINE__);

             status = WP_DeviceDisable(xgi1_device_handle, WP_DIRECTION_DUPLEX);
             WPE_TerminateOnError(status, "WP_DeviceDisable enetdev", __LINE__);

             port_cfg[0].interface_mode = WP_ENET_1000_BASE_X_AN;
             status = WP_PortModify(xgi1_port_handle,
                                    WP_PORT_MOD_ENET_INTERFACE_MODE,
                                    &port_cfg);
             WPE_TerminateOnError(status, "WP_PortModify()", __LINE__);

             status = WP_PortEnable(xgi1_port_handle, WP_DIRECTION_DUPLEX);
             WPE_TerminateOnError(status, "WP_PortEnable() Enet14", __LINE__);

             status = WP_DeviceEnable(xgi1_device_handle, WP_DIRECTION_DUPLEX);
             WPE_TerminateOnError(status, "WP_DeviceEnable enetdev", __LINE__);
//            status = WP_DeviceDisable(xgi1_device_handle, WP_DIRECTION_DUPLEX);
//            WPE_TerminateOnError(status, "WP_DeviceDisable enetdev", __LINE__);
//
         }
         break;
      }
      case 1:
      {
         if (enable)
         {
            status = WP_DeviceEnable(xgi1_device_handle, WP_DIRECTION_TX);
            WPE_TerminateOnError(status, "WP_DeviceEnable enetdev", __LINE__);
         }
         else
         {
            status = WP_DeviceDisable(xgi1_device_handle, WP_DIRECTION_TX);
            WPE_TerminateOnError(status, "WP_DeviceDisable enetdev", __LINE__);
         }
         break;
      }
      case 2:
      {
         if (enable)
         {
            status = WP_DeviceEnable(xgi2_device_handle, WP_DIRECTION_DUPLEX);
            WPE_TerminateOnError(status, "WP_DeviceEnable enetdev", __LINE__);
         }
         else
         {
            status = WP_DeviceDisable(xgi2_device_handle, WP_DIRECTION_DUPLEX);
            WPE_TerminateOnError(status, "WP_DeviceEnable enetdev", __LINE__);
         }
         break;
      }
      case 3:
      {
         if (enable)
         {
            status = WP_DeviceEnable(xgi2_device_handle, WP_DIRECTION_TX);
            WPE_TerminateOnError(status, "WP_DeviceEnable enetdev", __LINE__);
         }
         else
         {
            status = WP_DeviceDisable(xgi2_device_handle, WP_DIRECTION_TX);
            WPE_TerminateOnError(status, "WP_DeviceEnable enetdev", __LINE__);
         }
         break;
      }
      case 4:
      {
         if (enable)
         {
            for (ii = 0; ii < /*MAX_CLASSES*/10; ii++)
            {
               status = WP_DeviceEnable(intsw_class_handle[ii], WP_DIRECTION_TX);
               WPE_TerminateOnError(status, "WP_DeviceEnable enetdev", __LINE__);

               status = WP_DeviceEnable(mixed_class_handle[ii], WP_DIRECTION_TX);
               WPE_TerminateOnError(status, "WP_DeviceEnable enetdev", __LINE__);

            }
            for (ii = 0; ii < 3; ii++)
            {
               status = WP_DeviceEnable(mpe_class_shaping_scenario[ii], WP_DIRECTION_TX);
               WPE_TerminateOnError(status, "WP_DeviceEnable enetdev", __LINE__);
            }

         }
         else
         {
            for (ii = 0; ii < /*MAX_CLASSES*/10; ii++)
            {
               status = WP_DeviceDisable(intsw_class_handle[ii], WP_DIRECTION_TX);
               WPE_TerminateOnError(status, "WP_DeviceEnable enetdev", __LINE__);

               status = WP_DeviceDisable(mixed_class_handle[ii], WP_DIRECTION_TX);
               WPE_TerminateOnError(status, "WP_DeviceEnable enetdev", __LINE__);
            }
            for (ii = 0; ii < 3; ii++)
            {
               status = WP_DeviceDisable(mpe_class_shaping_scenario[ii], WP_DIRECTION_TX);
               WPE_TerminateOnError(status, "WP_DeviceEnable enetdev", __LINE__);
            }
         }

         break;
      }
   }
}
static void WPE_DeviceDisableLoop(WP_U32 device,WP_U32 loops)
{

   WP_U32 ii;
   WP_status status;
   WP_port_enet port_cfg[1] =
   {
         {
               /* pkt_limits */
               { 100, 1, 0, 0, 0 },
               /* flowmode */WP_ENET_FMU_HIERARCHICAL_SHAPING_MODE,
               /* interface_mode *//*WP_ENET_SGMII_1000*/WP_ENET_SGMII_AN,
               /* rx_iw_bkgnd */WP_IW_BKGND_NOT_USED
         }
   };
   switch(device)
   {
      case 0:
      {
         for (ii=0;ii<loops;ii++)
         {
            if (ii%20 == 0)
               printf("ii = %d out of %d with AN\n",ii,loops);

            status = WP_PortDisable(xgi1_port_handle, WP_DIRECTION_DUPLEX);
            WPE_TerminateOnError(status, "WP_PortEnable() Enet14", __LINE__);

            status = WP_DeviceDisable(xgi1_device_handle, WP_DIRECTION_DUPLEX);
            WPE_TerminateOnError(status, "WP_DeviceDisable enetdev", __LINE__);

            if (ii%2)
               port_cfg[0].interface_mode = WP_ENET_SGMII_AN;
            else
               port_cfg[0].interface_mode = WP_ENET_1000_BASE_X_AN;

            status = WP_PortModify(xgi1_port_handle,
                                   WP_PORT_MOD_ENET_INTERFACE_MODE,
                                   &port_cfg);

            WP_Delay(1000);

            status = WP_PortEnable(xgi1_port_handle, WP_DIRECTION_DUPLEX);
            WPE_TerminateOnError(status, "WP_PortEnable() Enet14", __LINE__);

            status = WP_DeviceEnable(xgi1_device_handle, WP_DIRECTION_DUPLEX);
            WPE_TerminateOnError(status, "WP_DeviceEnable enetdev", __LINE__);

         }
         break;
      }
      case 1:
      {
         for (ii=0;ii<loops;ii++)
         {
            if (ii%20 == 0)
               printf("ii = %d out of %d\n",ii,loops);

            status = WP_DeviceDisable(xgi1_device_handle, WP_DIRECTION_TX);
            WPE_TerminateOnError(status, "WP_DeviceDisable enetdev", __LINE__);

            status = WP_DeviceEnable(xgi1_device_handle, WP_DIRECTION_TX);
            WPE_TerminateOnError(status, "WP_DeviceEnable enetdev", __LINE__);
         }
         break;
      }
      case 2:
      {
         for (ii=0;ii<loops;ii++)
         {
            if (ii%20 == 0)
               printf("ii = %d out of %d\n",ii,loops);

            status = WP_PortDisable(xgi2_port_handle, WP_DIRECTION_DUPLEX);
            WPE_TerminateOnError(status, "WP_PortEnable() Enet14", __LINE__);

            status = WP_DeviceDisable(xgi2_device_handle, WP_DIRECTION_DUPLEX);
            WPE_TerminateOnError(status, "WP_DeviceDisable enetdev", __LINE__);

            if (ii%2)
               port_cfg[0].interface_mode = WP_ENET_SGMII_1000/*WP_ENET_SGMII_AN*/;
            else
               port_cfg[0].interface_mode = WP_ENET_1000_BASE_X;

            status = WP_PortModify(xgi1_port_handle,
                                   WP_PORT_MOD_ENET_INTERFACE_MODE,
                                   &port_cfg);

            WP_Delay(50);

            status = WP_DeviceEnable(xgi2_device_handle, WP_DIRECTION_DUPLEX);
            WPE_TerminateOnError(status, "WP_DeviceEnable enetdev", __LINE__);

            status = WP_PortEnable(xgi2_port_handle, WP_DIRECTION_DUPLEX);
            WPE_TerminateOnError(status, "WP_PortEnable() Enet13", __LINE__);
         }
         break;
      }
      case 3:
      {
         for (ii=0;ii<loops;ii++)
         {
            if (ii%20 == 0)
               printf("ii = %d out of %d\n",ii,loops);

            status = WP_DeviceDisable(xgi2_device_handle, WP_DIRECTION_TX);
            WPE_TerminateOnError(status, "WP_DeviceDisable enetdev", __LINE__);

            status = WP_DeviceEnable(xgi2_device_handle, WP_DIRECTION_TX);
            WPE_TerminateOnError(status, "WP_DeviceEnable enetdev", __LINE__);
         }
         break;
      }
      case 4:
      {
         for (ii=0;ii<loops;ii++)
         {
            if (ii%20 == 0)
               printf("ii = %d out of %d\n",ii,loops);

            status = WP_DeviceDisable(intsw_class_handle[0], WP_DIRECTION_TX);
            WPE_TerminateOnError(status, "WP_DeviceDisable enetdev", __LINE__);

            status = WP_DeviceEnable(intsw_class_handle[0], WP_DIRECTION_TX);
            WPE_TerminateOnError(status, "WP_DeviceEnable enetdev", __LINE__);
         }
         break;
      }
   }
}
static void WPE_ChangeSwitching(void)
{
   WP_U32 res, cmd;
   WP_U32 param1, param2, param3;
   WP_CHAR tmp[300];
   WP_CHAR StrPrm[100];

   while (1)
   {
      printf("\nChoose from the Menu below:\n");
      printf("1. Change to ExtSw\n");
      printf("2. Change to IntSw\n");
      printf("3. Change to Cascading\n");
      printf("4. Change Intsw to Extsw in a loop\n");
      printf("!.Upper Menu\n");
      printf("Enter your command:\n");

      gets(StrPrm);
      printf("\n\n");

      res = sscanf(StrPrm, "%d %d %d %d %s", &cmd, &param1, &param2, &param3, tmp);

      switch (cmd)
      {
         case 1:
         {
            WPE_ChangeToExtsw();
            break;
         }
         case 2:
         {
            WPE_ChangeToIntsw();
            break;
         }
         case 3:
         {
            WPE_ChangeToCascading();
            break;
         }
         case 4:
         {
            WPE_ChangeSwitchingLoop();
            break;
         }
         default:
         {
            if (StrPrm[0] != '!')
               printf("\nWrong number was entered\n\n");
         }

      } /* switch */

      if (StrPrm[0] == '!')
         break;

      cmd = 0;
   }
}
static void WPE_ChangeToCascading()
{
   WP_pce_rule_classification rule_cfg = { 0 };
   WP_U8 i = 0;
   WP_status status;

   switch (scenario)
   {
      case 1: /* bridging */
      {
         for (i = 0; i < MAX_CLASSES-1; i++)
         {
            memset(&rule_cfg, 0, sizeof(WP_pce_rule_classification));
            rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
            rule_cfg.match_result[0].param.flow_agg.flow_aggregation = cascading_fa[MAX_CLASSES-1];
            rule_cfg.match_result[1].result_type = WP_PCE_RESULT_IN_IW_PORT_UPDATE;
            rule_cfg.match_result[1].param.iw_port.iw_port_handle = iwport_handle[i];
            rule_cfg.match_result[2].result_type = WP_PCE_RESULT_LAST;

            status = WP_PceRuleModify(intsw_rules_handle[scenario][i], WP_PCE_RULE_MODIFY_COMMAND_RESULT, &rule_cfg);
            WPE_TerminateOnError(status, "WP_PceRuleModify() ", __LINE__);
         }
         break;
      }
      case 2: /* policer */
      {
         for (i = 0; i < MAX_CLASSES; i++)
         {
            memset(&rule_cfg, 0, sizeof(WP_pce_rule_classification));
            rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
            rule_cfg.match_result[0].param.flow_agg.flow_aggregation = cascading_fa[i];
            rule_cfg.match_result[1].result_type = WP_PCE_RESULT_INGRESS_POLICER;
            rule_cfg.match_result[1].param.ingress_policer.policer = policer_handle;
            rule_cfg.match_result[2].result_type = WP_PCE_RESULT_LAST;

            status = WP_PceRuleModify(intsw_rules_handle[scenario][i], WP_PCE_RULE_MODIFY_COMMAND_RESULT, &rule_cfg);
            WPE_TerminateOnError(status, "WP_PceRuleModify() ", __LINE__);
         }
         break;
      }
      case 3: /* performance */
      {
         for (i = 0; i < MAX_CLASSES; i++)
         {
            memset(&rule_cfg, 0, sizeof(WP_pce_rule_classification));
            rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
            rule_cfg.match_result[0].param.flow_agg.flow_aggregation = cascading_fa[i];
            rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;

            status = WP_PceRuleModify(intsw_rules_handle[scenario][i], WP_PCE_RULE_MODIFY_COMMAND_RESULT, &rule_cfg);
            WPE_TerminateOnError(status, "WP_PceRuleModify() ", __LINE__);
         }
         break;
      }
      default:
      {
         printf("\nNo such secnario\n\n");
      }
   }
}
static void WPE_ChangeToIntsw()
{
   WP_pce_rule_classification rule_cfg = { 0 };
   WP_U8 i = 0;
   WP_status status;

   switch (scenario)
   {
      case 1: /* bridging */
      {
         for (i = 0; i < MAX_CLASSES-1; i++)
         {
            memset(&rule_cfg, 0, sizeof(WP_pce_rule_classification));
            rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
            rule_cfg.match_result[0].param.flow_agg.flow_aggregation = extsw_fa_xgi1[MAX_CLASSES-1];
            rule_cfg.match_result[1].result_type = WP_PCE_RESULT_IN_IW_PORT_UPDATE;
            rule_cfg.match_result[1].param.iw_port.iw_port_handle = iwport_handle[i];
            rule_cfg.match_result[2].result_type = WP_PCE_RESULT_LAST;

            status = WP_PceRuleModify(intsw_rules_handle[scenario][i], WP_PCE_RULE_MODIFY_COMMAND_RESULT, &rule_cfg);
            WPE_TerminateOnError(status, "WP_PceRuleModify() ", __LINE__);
         }
         break;
      }
      case 2: /* policer */
      {
         for (i = 0; i < MAX_CLASSES; i++)
         {
            memset(&rule_cfg, 0, sizeof(WP_pce_rule_classification));
            rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
            rule_cfg.match_result[0].param.flow_agg.flow_aggregation = extsw_fa_xgi1[i];
            rule_cfg.match_result[1].result_type = WP_PCE_RESULT_INGRESS_POLICER;
            rule_cfg.match_result[1].param.ingress_policer.policer = policer_handle;
            rule_cfg.match_result[2].result_type = WP_PCE_RESULT_LAST;

            status = WP_PceRuleModify(intsw_rules_handle[scenario][i], WP_PCE_RULE_MODIFY_COMMAND_RESULT, &rule_cfg);
            WPE_TerminateOnError(status, "WP_PceRuleModify() ", __LINE__);
         }
         break;
      }
      case 3: /* performance */
      {
         for (i = 0; i < MAX_CLASSES; i++)
         {
            memset(&rule_cfg, 0, sizeof(WP_pce_rule_classification));
            rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
            rule_cfg.match_result[0].param.flow_agg.flow_aggregation = extsw_fa_xgi1[i];
            rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;

            status = WP_PceRuleModify(intsw_rules_handle[scenario][i], WP_PCE_RULE_MODIFY_COMMAND_RESULT, &rule_cfg);
            WPE_TerminateOnError(status, "WP_PceRuleModify() ", __LINE__);
         }
         break;
      }
      default:
      {
         printf("\nNo such secnario\n\n");
      }
   }
}
static void WPE_ChangeToExtsw()
{
   WP_pce_rule_classification rule_cfg = { 0 };
   WP_U8 i = 0;
   WP_status status;

   switch (scenario)
   {
      case 1: /* bridging */
      {
         for (i = 0; i < MAX_CLASSES-1; i++)
         {
            memset(&rule_cfg, 0, sizeof(WP_pce_rule_classification));
            rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
            rule_cfg.match_result[0].param.flow_agg.flow_aggregation = mixed_fa[MAX_CLASSES-1];
            rule_cfg.match_result[1].result_type = WP_PCE_RESULT_IN_IW_PORT_UPDATE;
            rule_cfg.match_result[1].param.iw_port.iw_port_handle = iwport_handle[i];
            rule_cfg.match_result[2].result_type = WP_PCE_RESULT_LAST;

            status = WP_PceRuleModify(intsw_rules_handle[scenario][i], WP_PCE_RULE_MODIFY_COMMAND_RESULT, &rule_cfg);
            WPE_TerminateOnError(status, "WP_PceRuleModify() ", __LINE__);
         }
         break;
      }
      case 2: /* policer */
      {
         for (i = 0; i < MAX_CLASSES; i++)
         {
            memset(&rule_cfg, 0, sizeof(WP_pce_rule_classification));
            rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
            rule_cfg.match_result[0].param.flow_agg.flow_aggregation = mixed_fa[i];
            rule_cfg.match_result[1].result_type = WP_PCE_RESULT_INGRESS_POLICER;
            rule_cfg.match_result[1].param.ingress_policer.policer = policer_handle;
            rule_cfg.match_result[2].result_type = WP_PCE_RESULT_LAST;

            status = WP_PceRuleModify(intsw_rules_handle[scenario][i], WP_PCE_RULE_MODIFY_COMMAND_RESULT, &rule_cfg);
            WPE_TerminateOnError(status, "WP_PceRuleModify() ", __LINE__);
         }
         break;
      }
      case 3: /* performance */
      {
         for (i = 0; i < MAX_CLASSES; i++)
         {
            memset(&rule_cfg, 0, sizeof(WP_pce_rule_classification));
            rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
            rule_cfg.match_result[0].param.flow_agg.flow_aggregation = mixed_fa[i];
            rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;

            status = WP_PceRuleModify(intsw_rules_handle[scenario][i], WP_PCE_RULE_MODIFY_COMMAND_RESULT, &rule_cfg);
            WPE_TerminateOnError(status, "WP_PceRuleModify() ", __LINE__);
         }
         break;
      }
      default:
      {
         printf("\nNo such secnario\n\n");
      }
   }
}
static void WPE_ChangeSwitchingLoop()
{
   WP_U32 i = 0;

   for (i = 0 ; i < 1000000; i ++)
   {
      if (i%100 == 0)
         printf("i = %d out of 1,000,000\n",i);
      WP_Delay(100);
      WPE_ChangeToExtsw();
      WP_Delay(100);
      WPE_ChangeToIntsw();
   }

}
static void WPE_Statistics(void)
{

   WP_U32 res, cmd;
   WP_U32 param1, param2;
   WP_CHAR tmp[300];
   WP_CHAR StrPrm[100];

   while (1)
   {
      printf("\nChoose from the Menu below:\n");
      printf("1.Device Statistics :\n");
      printf("2.Iw Port Statistics :\n");
      printf("3.PCE Statistics :\n");
      printf("4.FA statistics :\n");
      printf("5.Policer statistics :\n");
      printf("7.Qnode status :\n");
      printf("8.Statistics Reset :\n");
      printf("9.CFU Display :\n");
      printf("!.Upper Menu\n");
      printf("Enter your command:\n");

      gets(StrPrm);
      printf("\n\n");

      res = sscanf(StrPrm, "%d %d %d %s", &cmd, &param1, &param2, tmp);

      switch (cmd)
      {
         case 1: /* Device statistics */
         {
            WPE_DeviceStats();
            break;
         }
         case 2: /* Iw Port statistics */
         {
            WPE_IwPortStats();
            break;
         }
         case 3: /* PCE statistics */
         {
            WPE_GetPceStatistics_Menu(StrPrm);
            break;
         }
         case 4: /* FA statistics */
         {
            WPE_ShowFlowStats_Menu(StrPrm);
            break;
         }
         case 5: /* Policer statistics */
         {
            WPE_ShowPolicerStats();
            break;
         }
         case 7: /* Qnode status */
         {
            WPE_Check_Qnode_Status();
            break;
         }
         case 8: /* Statistics reset*/
         {
            WPE_StatisticsReset();
            break;
         }
         case 9: /* Statistics reset*/
         {
            WP_status status;
            status = WP_Display(WP_WINPATH(DEFAULT_WPID), WP_DISPLAY_SA, WP_DISPLAY_DEFAULT, 0);

            WPE_TerminateOnError(status, "CFU Info Display", __LINE__);

            break;
         }
         default:
         {
            if (StrPrm[0] != '!')
               printf("\nWrong number was entered\n\n");
         }

      } /* switch */

      if (StrPrm[0] == '!')
         break;

      cmd = 0;
   }
}
static void WPE_Scenarios()
{
   WP_U32 res, cmd;
   WP_CHAR tmp[300];
   WP_CHAR StrPrm[100];
   WP_U32 param1;

   while (1)
   {
      cmd = 0;
      printf("\nChoose from the Menu below:\n");
      printf("1. Bridging\n");
      printf("2. Policing\n");
      printf("3. Performance\n");
      printf("4. Mixed device\n");
      printf("5. Override FA rules\n");
      printf("6. Mpe Shaping\n");
      printf("7. Performance with PECS\n");
      printf("!.Upper Menu\n");
      printf("Enter your command:\n");

      gets(StrPrm);
      printf("\n\n");

      res = sscanf(StrPrm, "%d %d %s", &cmd, &param1, tmp);

      switch (cmd)
      {
         case 1:
         {
            WPE_Bridging();
            break;
         }
         case 2:
         {
            WPE_Policing();
            break;
         }
         case 3:
         {
            WPE_Performance();
            break;
         }
         case 4:
         {
            WPE_Mixed();
            break;
         }
         case 5:
         {
            WPE_Override();
            break;
         }
         case 6:
         {
            WPE_Mpe_Shaping();
            break;
         }
         case 7:
         {
            WPE_Performance_w_pecs();
            break;
         }
         default:
         {
            if (StrPrm[0] != '!')
               printf("\nNo such Scenario\n\n");
         }

      } /* switch */

      if (StrPrm[0] == '!')
         break;

      cmd = 0;
   }
}
static void WPE_Bridging()
{
   WP_U32 res, cmd;
   WP_U32 param1, param2;
   WP_CHAR tmp[300];
   WP_CHAR StrPrm[100];

   while (1)
   {
      printf("\nChoose from the Menu below:\n");
      printf("1.Create Bridging rules:\n");
      printf("2.learning service queue function:\n");
      printf("!.Upper Menu\n");
      printf("Enter your command:\n");

      gets(StrPrm);
      printf("\n\n");

      res = sscanf(StrPrm, "%d %d %d %s", &cmd, &param1, &param2, tmp);
//      if(res != 3)
//      {
//         printf("Usage: <option> <1(Group Number),2(Link Number)> <object number>\n");
//         return;
//      }

      switch (cmd)
      {
         case 1: /* learning service queue function */
         {
            scenario = 1;
            WPE_Initialize_Pce_Rules(); /* bridging rules */
            break;
         }
         case 2: /* learning service queue function */
         {
            WPE_CheckForwardingAndLearning();
            break;
         }
         default:
         {
            if (StrPrm[0] != '!')
               printf("\nWrong number was entered\n\n");
         }

      } /* switch */

      if (StrPrm[0] == '!')
         break;

      cmd = 0;
   }
}
static void WPE_CheckForwardingAndLearning()
{
   WP_U32 rules_returned;
   WP_pce_rule_forwarding *rule_fwd;

   WP_PceLearningQueueService(WP_WINPATH(DEFAULT_WPID),
                              &(learned_rules_list[0]),
                              1,
                              &rules_returned);

   rule_fwd = &(learned_rules_list[0].fwd_rule);

   if (rules_returned)
   {
#define WPI_BASIC1_ERROR(x) ((((WP_U32)(x)) | (((WP_U32) (1 << 6) - 1) << 26)))
      rule_fwd->match_result[2].result_type = WP_PCE_RESULT_FLOW_AGG;
      /* mac_addr[5] contains the index of the FA */
      rule_fwd->match_result[2].param.flow_agg.flow_aggregation =
            extsw_fa_xgi1[rule_fwd->rule_fields[0].value.mac_addr[5]];
      rule_fwd->match_result[3].result_type = WP_PCE_RESULT_LAST;

      fwd_rules[fwd_idx] = WP_PceRuleCreate(WP_WINPATH(0), WP_PCE_RULE_FORWARDING, rule_fwd);
      if (fwd_rules[fwd_idx] != WPI_BASIC1_ERROR(WP_ERR_PCE_RULE_ALREADY_EXISTS))
      {
         WPE_TerminateOnError(fwd_rules[fwd_idx], "WP_PceRuleCreate() ", __LINE__);
         printf("\na fwd rule was created\n");
//         WP_Display(WP_WINPATH(DEFAULT_WPID), fwd_rules[fwd_idx] , 0, 0);
      }
      else
         printf("\na similar fwd rule was already created\n");

      fwd_idx++;
   }
   else
      printf("\nNo more fwd rules to create\n");
}
static void WPE_Mpe_Shaping()
{
   WP_U32 res, cmd;
   WP_U32 param1, param2;
   WP_CHAR tmp[300];
   WP_CHAR StrPrm[100];

   while (1)
   {
      printf("\nChoose from the Menu below:\n");
      printf("1.Create Mpe Shaping rules:\n");
      printf("!.Upper Menu\n");
      printf("Enter your command:\n");

      gets(StrPrm);
      printf("\n\n");

      res = sscanf(StrPrm, "%d %d %d %s", &cmd, &param1, &param2, tmp);
//      if(res != 3)
//      {
//         printf("Usage: <option> <1(Group Number),2(Link Number)> <object number>\n");
//         return;
//      }

      switch (cmd)
      {
         case 1: /* learning service queue function */
         {
            scenario = 6;
            WPE_Initialize_Pce_Rules(); /* performance rules */
            break;
         }
         default:
         {
            if (StrPrm[0] != '!')
               printf("\nWrong number was entered\n\n");
         }

      } /* switch */

      if (StrPrm[0] == '!')
         break;

      cmd = 0;
   }
}
static void WPE_Performance()
{
   WP_U32 res, cmd;
   WP_U32 param1, param2;
   WP_CHAR tmp[300];
   WP_CHAR StrPrm[100];

   while (1)
   {
      printf("\nChoose from the Menu below:\n");
      printf("1.Create Performance rules:\n");
      printf("2.Update VLAN rules - 1. Create Vlan update rule 0. Delete Vlan Update rule:\n");
      printf("!.Upper Menu\n");
      printf("Enter your command:\n");

      gets(StrPrm);
      printf("\n\n");

      res = sscanf(StrPrm, "%d %d %d %s", &cmd, &param1, &param2, tmp);
//      if(res != 3)
//      {
//         printf("Usage: <option> <1(Group Number),2(Link Number)> <object number>\n");
//         return;
//      }

      switch (cmd)
      {
         case 1: /* learning service queue function */
         {
            scenario = 3;
            WPE_Initialize_Pce_Rules(); /* performance rules */
            break;
         }
         case 2: /* learning service queue function */
         {
            WPE_Update_Vlan_Rule(param1); /* policer rules */
            break;
         }
         default:
         {
            if (StrPrm[0] != '!')
               printf("\nWrong number was entered\n\n");
         }

      } /* switch */

      if (StrPrm[0] == '!')
         break;

      cmd = 0;
   }
}
static void WPE_Performance_w_pecs()
{
   WP_U32 res, cmd;
   WP_U32 param1, param2;
   WP_CHAR tmp[300];
   WP_CHAR StrPrm[100];

   while (1)
   {
      printf("\nChoose from the Menu below:\n");
      printf("1.Create Performance rules (with PECS):\n");
      printf("!.Upper Menu\n");
      printf("Enter your command:\n");

      gets(StrPrm);
      printf("\n\n");

      res = sscanf(StrPrm, "%d %d %d %s", &cmd, &param1, &param2, tmp);
//      if(res != 3)
//      {
//         printf("Usage: <option> <1(Group Number),2(Link Number)> <object number>\n");
//         return;
//      }

      switch (cmd)
      {
         case 1: /* learning service queue function */
         {
            scenario = 7;
            WPE_Initialize_Pce_Rules(); /* performance rules */
            break;
         }
         default:
         {
            if (StrPrm[0] != '!')
               printf("\nWrong number was entered\n\n");
         }

      } /* switch */

      if (StrPrm[0] == '!')
         break;

      cmd = 0;
   }
}
static void WPE_Mixed()
{
   WP_U32 res, cmd;
   WP_U32 param1, param2;
   WP_CHAR tmp[300];
   WP_CHAR StrPrm[100];

   while (1)
   {
      printf("\nChoose from the Menu below:\n");
      printf("1.Create Mixed Device rules:\n");
      printf("!.Upper Menu\n");
      printf("Enter your command:\n");

      gets(StrPrm);
      printf("\n\n");

      res = sscanf(StrPrm, "%d %d %d %s", &cmd, &param1, &param2, tmp);
//      if(res != 3)
//      {
//         printf("Usage: <option> <1(Group Number),2(Link Number)> <object number>\n");
//         return;
//      }

      switch (cmd)
      {
         case 1: /* learning service queue function */
         {
            scenario = 4;
            WPE_Initialize_Pce_Rules(); /* performance rules */
            break;
         }
         default:
         {
            if (StrPrm[0] != '!')
               printf("\nWrong number was entered\n\n");
         }

      } /* switch */

      if (StrPrm[0] == '!')
         break;

      cmd = 0;
   }
}
static void WPE_Override()
{
   WP_U32 res, cmd;
   WP_U32 param1, param2;
   WP_CHAR tmp[300];
   WP_CHAR StrPrm[100];

   while (1)
   {
      printf("\nChoose from the Menu below:\n");
      printf("1.Create Filters Rules:\n");
      printf("2.Modify Filters Rules:\n");
      printf("!.Upper Menu\n");
      printf("Enter your command:\n");

      gets(StrPrm);
      printf("\n\n");

      res = sscanf(StrPrm, "%d %d %d %s", &cmd, &param1, &param2, tmp);
//      if(res != 3)
//      {
//         printf("Usage: <option> <1(Group Number),2(Link Number)> <object number>\n");
//         return;
//      }

      switch (cmd)
      {
         case 1:
         {
            scenario = 5;
            WPE_Create_Override_Rules_Menu(); /* policer rules */
            break;
         }
         case 2:
         {
            WPE_Modify_Override_Rules_Menu(); /* policer rules */
            break;
         }
         default:
         {
            if (StrPrm[0] != '!')
               printf("\nWrong number was entered\n\n");
         }

      } /* switch */

      if (StrPrm[0] == '!')
         break;

      cmd = 0;
   }
}

static void WPE_Create_Override_Rules_Menu()
{
   WP_U32 res, cmd;
   WP_U32 param1, param2;
   WP_CHAR tmp[300];
   WP_CHAR StrPrm[100];

   printf("Override scenario\n");
   while (1)
   {
      printf("1.First Filter Generic FA second filter with Intsw Cascading FA:\n");
      printf("2.First Filter Generic FA second filter with Intsw IW FA:\n");
      printf("3.First Filter Intsw Cascading FA second filter with Generic FA:\n");
      printf("4.First Filter Intsw Cascading FA second filter with Intsw IW FA:\n");
      printf("5.First Filter Intsw IW FA second filter with Generic FA:\n");
      printf("6.First Filter Intsw IW FA second filter with Intsw Cascading FA:\n");
      printf("!.Upper Menu\n");
      printf("Enter your command:\n");

      gets(StrPrm);
      printf("\n\n");

      res = sscanf(StrPrm, "%d %d %d %s", &cmd, &param1, &param2, tmp);
//      if(res != 3)
//      {
//         printf("Usage: <option> <1(Group Number),2(Link Number)> <object number>\n");
//         return;
//      }

      switch (cmd)
      {
         case 1:
         {
            WPE_Create_Override_Rules(GENERIC_FA_TYPE,INTSW_CASCADING_FA_TYPE);
            break;
         }
         case 2:
         {
            WPE_Create_Override_Rules(GENERIC_FA_TYPE,INTSW_IW__FA_TYPE);
            break;
         }
         case 3:
         {
            WPE_Create_Override_Rules(INTSW_CASCADING_FA_TYPE,GENERIC_FA_TYPE);
            break;
         }
         case 4:
         {
            WPE_Create_Override_Rules(INTSW_CASCADING_FA_TYPE,INTSW_IW__FA_TYPE);
            break;
         }
         case 5:
         {
            WPE_Create_Override_Rules(INTSW_IW__FA_TYPE,GENERIC_FA_TYPE);
            break;
         }
         case 6:
         {
            WPE_Create_Override_Rules(INTSW_IW__FA_TYPE,INTSW_CASCADING_FA_TYPE);
            break;
         }
         default:
         {
            if (StrPrm[0] != '!')
               printf("\nWrong number was entered\n\n");
         }

      } /* switch */

      if (StrPrm[0] == '!')
         break;

      cmd = 0;
   }
}
static void WPE_Modify_Override_Rules_Menu()
{
   WP_U32 res, cmd;
   WP_U32 param1, param2;
   WP_CHAR tmp[300];
   WP_CHAR StrPrm[100];

   while (1)
   {
      printf("1.First Filter Generic FA second filter with Intsw Cascading FA:\n");
      printf("2.First Filter Generic FA second filter with Intsw IW FA:\n");
      printf("3.First Filter Intsw Cascading FA second filter with Generic FA:\n");
      printf("4.First Filter Intsw Cascading FA second filter with Intsw IW FA:\n");
      printf("5.First Filter Intsw IW FA second filter with Generic FA:\n");
      printf("6.First Filter Intsw IW FA second filter with Intsw Cascading FA:\n");
      printf("!.Upper Menu\n");
      printf("Enter your command:\n");

      gets(StrPrm);
      printf("\n\n");

      res = sscanf(StrPrm, "%d %d %d %s", &cmd, &param1, &param2, tmp);
//      if(res != 3)
//      {
//         printf("Usage: <option> <1(Group Number),2(Link Number)> <object number>\n");
//         return;
//      }

      switch (cmd)
      {
         case 1:
         {
            WPE_Modify_Override_Rules(GENERIC_FA_TYPE,INTSW_CASCADING_FA_TYPE);
            break;
         }
         case 2:
         {
            WPE_Modify_Override_Rules(GENERIC_FA_TYPE,INTSW_IW__FA_TYPE);
            break;
         }
         case 3:
         {
            WPE_Modify_Override_Rules(INTSW_CASCADING_FA_TYPE,GENERIC_FA_TYPE);
            break;
         }
         case 4:
         {
            WPE_Modify_Override_Rules(INTSW_CASCADING_FA_TYPE,INTSW_IW__FA_TYPE);
            break;
         }
         case 5:
         {
            WPE_Modify_Override_Rules(INTSW_IW__FA_TYPE,GENERIC_FA_TYPE);
            break;
         }
         case 6:
         {
            WPE_Modify_Override_Rules(INTSW_IW__FA_TYPE,INTSW_CASCADING_FA_TYPE);
            break;
         }
         default:
         {
            if (StrPrm[0] != '!')
               printf("\nWrong number was entered\n\n");
         }

      } /* switch */

      if (StrPrm[0] == '!')
         break;

      cmd = 0;
   }
}
static void WPE_Policing()
{
   WP_U32 res, cmd;
   WP_U32 param1, param2;
   WP_CHAR tmp[300];
   WP_CHAR StrPrm[100];

   while (1)
   {
      printf("\nChoose from the Menu below:\n");
      printf("1.Create policer:\n");
      printf("2.Create policer Rules:\n");
      printf("3.Enable/Disable the policer - Enter 1 to Enable and 0 to Disable:\n");
      printf("4.Enable/Disable the policer in a loop:\n");
      printf("5.Enable Pecs with vlan unchanged:\n");
      printf("6.Enable Pecs with vlan modification:\n");
      printf("7.Change Input Color - Enter 1 to change input color to yellow and 0 to keep it as is:\n");
      printf("!.Upper Menu\n");
      printf("Enter your command:\n");

      gets(StrPrm);
      printf("\n\n");

      res = sscanf(StrPrm, "%d %d %d %s", &cmd, &param1, &param2, tmp);
//      if(res != 3)
//      {
//         printf("Usage: <option> <1(Group Number),2(Link Number)> <object number>\n");
//         return;
//      }

      switch (cmd)
      {
         case 1:
         {
            WPE_Create_Policer();
            break;
         }
         case 2:
         {
            scenario = 2;
            WPE_Initialize_Pce_Rules();
            break;
         }
         case 3:
         {
            WPE_Enable_Disable_Policer(param1); /* policer rules */
            break;
         }
         case 4:
         {
            WPE_Enable_Disable_Policer_Loop(); /* policer rules */
            break;
         }
         case 5:
         {
            WPE_Policer_Vlan(WP_ENABLE,WP_DISABLE); /* policer rules */
            break;
         }
         case 6:
         {
            WPE_Policer_Vlan(WP_ENABLE,WP_ENABLE); /* policer rules */
            break;
         }
         case 7:
         {
            WPE_Change_Color(param1); /* policer rules */
            break;
         }
         default:
         {
            if (StrPrm[0] != '!')
               printf("\nWrong number was entered\n\n");
         }

      } /* switch */

      if (StrPrm[0] == '!')
         break;

      cmd = 0;
   }
}

static void WPE_Create_Policer()
{
   WP_U32 res, cmd;
   WP_U32 param1, param2;
   WP_CHAR tmp[300];
   WP_CHAR StrPrm[100];

   WP_policer_action policer_action_table =
   {
         {/* green */
               /*WP_POLICER_ACTION_PRIORITY_MAPPING |*/ WP_POLICER_ACTION_PREFIX_REMARKING,
               {
                     {
                           WP_POLICER_ACTION_PREFIX_REMARKING_TYPE_EXT_VLAN_PRIO,
                           WP_POLICER_ACTION_PREFIX_REMARKING_TYPE_INT_VLAN_PRIO,
                           WP_POLICER_ACTION_PREFIX_REMARKING_TYPE_DO_NOT_CHANGE,
                           WP_POLICER_ACTION_PREFIX_REMARKING_TYPE_TOS
                     },
                     {
                           1, 2, 3, 3
                     }
               },
               /* priority_mapping_value */ 0x7,
               /* congestion_profile_entry */ 0
         },
         {/* yellow */
               /*WP_POLICER_ACTION_PRIORITY_MAPPING | */WP_POLICER_ACTION_PREFIX_REMARKING,
               {
                     {
                           WP_POLICER_ACTION_PREFIX_REMARKING_TYPE_EXT_VLAN_PRIO,
                           WP_POLICER_ACTION_PREFIX_REMARKING_TYPE_INT_VLAN_PRIO,
                           WP_POLICER_ACTION_PREFIX_REMARKING_TYPE_DO_NOT_CHANGE,
                           WP_POLICER_ACTION_PREFIX_REMARKING_TYPE_TOS
                     },
                     {
                           3, 4, 4, 4
                     }
               },
               /* priority_mapping_value */ 0x8,
               /* congestion_profile_entry */ 0
         },
         {/* red */
               /*WP_POLICER_ACTION_PRIORITY_MAPPING | */WP_POLICER_ACTION_PREFIX_REMARKING,
               {
                     {
                           WP_POLICER_ACTION_PREFIX_REMARKING_TYPE_EXT_VLAN_PRIO,
                           WP_POLICER_ACTION_PREFIX_REMARKING_TYPE_INT_VLAN_PRIO,
                           WP_POLICER_ACTION_PREFIX_REMARKING_TYPE_DO_NOT_CHANGE,
                           WP_POLICER_ACTION_PREFIX_REMARKING_TYPE_TOS
                     },
                     {
                           5, 6, 5, 5
                     }
               },
               /* priority_mapping_value */ 0x9,
               /* congestion_profile_entry */ 0
         },
   };

   WP_policer_v2 pv2 =
   {
   /* method */WP_POLICER_METHOD_MEF_WITH_COUPLING_FLAG/*WP_POLICER_METHOD_SINGLE_LEAKY_BUCKET*/,
#ifdef WP_BOARD_WDS3_SL
         /* type */WP_POLICER_V2_TYPE_EXTERNAL,
#else
         /* type */WP_POLICER_V2_TYPE_INTERNAL,
#endif
         /* color_mode */WP_POLICER_COLOR_AWARE,
         /* red_packets_action */WP_POLICER_DROP_RED_PACKETS,
         /* cir */1,
         /* cbs */2,
         /* eir */3,
         /* ebs */4,
         /* policer_action */0,
         /* frame_len_correction */WP_POLICER_LENGTH_CORRECT_4B
   };

   while (1)
   {
      printf("\nChoose from the Menu below:\n");
      printf("1.Policer Method - Enter 0 for Dual bucket and 1 for Single bucket:\n");
      printf("2.Color mode - Enter 0 for Color aware and 1 for Color blind:\n");
      printf("3.Red packets action - Enter 0 for Drop red packets and 1 for Passing red Packets:\n");
      printf("4.Policer action - Enter 0 for Policer action and 1 for Without Policer Action:\n");
      printf("5.CIR/EIR - Enter CIR: Enter EIR (With a space between them)in Units of MEGA:\n");
      printf("6.Create Policer! (Default CIR/EIR 2Giga):\n");
      printf("!.Upper Menu\n");
      printf("Enter your command:\n");

      gets(StrPrm);
      printf("\n\n");

      res = sscanf(StrPrm, "%d %d %d %s", &cmd, &param1, &param2, tmp);
//      if(res != 3)
//      {
//         printf("Usage: <option> <1(Group Number),2(Link Number)> <object number>\n");
//         return;
//      }

      switch (cmd)
      {
         case 1:
         {
            if (param1)
               policer_method = 1;
            else
               policer_method = 0;
            break;
         }
         case 2:
         {
            if (param1)
               color_mode = 1;
            else
               color_mode = 0;
            break;
         }
         case 3:
         {
            if (param1)
               red_packets_action = 1;
            else
               red_packets_action = 0;
            break;
         }
         case 4:
         {
            if (param1)
               policer_action = 1;
            else
               policer_action = 0;
            break;
         }
         case 5:
         {
            cir = param1*1000000;
            eir = param2*1000000;
            break;
         }
         case 6:
         {
            papt_handle = WP_PolicerActionCreate(WP_WINPATH(0), &policer_action_table);
            WPE_TerminateOnError(papt_handle, "WP_PolicerActionCreate", __LINE__);

            /* this policer would be associated with 10Gbps RX interface */
            pv2.cir = cir /* 20% *//*325853658*/; /* 30% of 10Gbps */
            pv2.eir = eir/* 20% *//*551870912*/; /* 50% of 10Gbps */
            pv2.cbs = 131070; /* 3840 bytes */
            pv2.ebs = 131070;

            if (policer_action)
               pv2.policer_action = 0;
            else
               pv2.policer_action = papt_handle; /* Default */

            if (policer_method)
               pv2.method = WP_POLICER_METHOD_SINGLE_LEAKY_BUCKET;
            else
               pv2.method = WP_POLICER_METHOD_MEF_WITH_COUPLING_FLAG; /* Default */

            if (color_mode)
               pv2.color_mode = WP_POLICER_COLOR_BLIND;
            else
               pv2.color_mode = WP_POLICER_COLOR_AWARE; /* Default */

            if (red_packets_action)
               pv2.red_packets_action = WP_POLICER_PASS_RED_PACKETS;
            else
               pv2.red_packets_action = WP_POLICER_DROP_RED_PACKETS; /* Default */

            policer_handle = WP_PolicerCreate(WP_WINPATH(0), WP_POLICER_V2, &pv2);
            WPE_TerminateOnError(policer_handle, "WP_PolicerCreate", __LINE__);
            break;
         }
         default:
         {
            if (StrPrm[0] != '!')
               printf("\nWrong number was entered\n\n");
         }

      } /* switch */

      if (StrPrm[0] == '!')
         break;

      cmd = 0;
   }
}
static void WPE_Change_Color(WP_U32 change_to_yellow)
{
   WP_U32 i;
   WP_pce_rule_classification rule_cfg = { 0 };
   WP_status status;


   /* first filter rule */
   memset(&rule_cfg, 0, sizeof(WP_pce_rule_classification));

   for (i = 0; i < MAX_CLASSES; i++)
   {
      if (change_to_yellow)
      {
         memset(&rule_cfg, 0, sizeof(WP_pce_rule_classification));

         rule_cfg.match_action = WP_PCE_RULE_MATCH_CONTINUE;
         status = WP_PceRuleModify(intsw_rules_handle[scenario][i], WP_PCE_RULE_MODIFY_COMMAND_MATCH_ACTION, &rule_cfg);
         WPE_TerminateOnError(status, "WP_PceRuleModify() ", __LINE__);

         rule_cfg.enabled = WP_ENABLE;
         rule_cfg.match_action = WP_PCE_RULE_MATCH_ACCEPT;
         rule_cfg.filter_handle = xgi2_pce_filter[CLASSIFICATION1];
         rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_MAC_SA;
         rule_cfg.rule_fields[0].value.mac_addr[0] = 0x00;
         rule_cfg.rule_fields[0].value.mac_addr[1] = 0x00;
         rule_cfg.rule_fields[0].value.mac_addr[2] = 0x00;
         rule_cfg.rule_fields[0].value.mac_addr[3] = 0x00;
         rule_cfg.rule_fields[0].value.mac_addr[4] = scenario;
         rule_cfg.rule_fields[0].value.mac_addr[5] = i;
         rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_LAST;
         rule_cfg.match_result[0].result_type = WP_PCE_RESULT_INGRESS_POLICER_INPUT_COLOR;
         rule_cfg.match_result[0].param.policer_color.color = WP_IW_TCM_INPUT_COLOR_YELLOW;
         rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;

         input_color_rules[i] = WP_PceRuleCreate(WP_WINPATH(0),
                                                 WP_PCE_RULE_CLASSIFICATION,
                                                 &rule_cfg);
         WPE_TerminateOnError(input_color_rules[i], "WP_PceRuleCreate() ", __LINE__);
      }
      else
      {
         memset(&rule_cfg, 0, sizeof(WP_pce_rule_classification));

         rule_cfg.match_action = WP_PCE_RULE_MATCH_ACCEPT;
         status = WP_PceRuleModify(intsw_rules_handle[scenario][i], WP_PCE_RULE_MODIFY_COMMAND_MATCH_ACTION, &rule_cfg);
         WPE_TerminateOnError(status, "WP_PceRuleModify() ", __LINE__);

         status = WP_PceRuleDelete(input_color_rules[i]);
         WPE_TerminateOnError(status, "WP_PceRuleDelete() ", __LINE__);
      }
   }
}
static void WPE_Update_Vlan_Rule(WP_U32 create_rule)
{
   WP_U32 i;
   WP_pce_rule_classification rule_cfg = { 0 };
   WP_status status;


   /* first filter rule */
   memset(&rule_cfg, 0, sizeof(WP_pce_rule_classification));

   for (i = 0; i < MAX_CLASSES; i++)
   {
      if (create_rule)
      {
         memset(&rule_cfg, 0, sizeof(WP_pce_rule_classification));

         rule_cfg.match_action = WP_PCE_RULE_MATCH_CONTINUE;
         status = WP_PceRuleModify(intsw_rules_handle[scenario][i], WP_PCE_RULE_MODIFY_COMMAND_MATCH_ACTION, &rule_cfg);
         WPE_TerminateOnError(status, "WP_PceRuleModify() ", __LINE__);

         rule_cfg.enabled = WP_ENABLE;
         rule_cfg.match_action = WP_PCE_RULE_MATCH_ACCEPT;
         rule_cfg.filter_handle = xgi2_pce_filter[CLASSIFICATION1];
         rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_MAC_SA;
         rule_cfg.rule_fields[0].value.mac_addr[0] = 0x00;
         rule_cfg.rule_fields[0].value.mac_addr[1] = 0x00;
         rule_cfg.rule_fields[0].value.mac_addr[2] = 0x00;
         rule_cfg.rule_fields[0].value.mac_addr[3] = 0x00;
         rule_cfg.rule_fields[0].value.mac_addr[4] = scenario;
         rule_cfg.rule_fields[0].value.mac_addr[5] = i;
         rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_LAST;

         rule_cfg.match_result[0].result_type = WP_PCE_RESULT_EXT_VLAN_UPDATE;
         rule_cfg.match_result[0].param.ext_vlan.vlan_tag = 0xFFFF;
         rule_cfg.match_result[0].param.ext_vlan.mask = WP_PCE_MASK_VLAN_PRIO;
         rule_cfg.match_result[1].result_type = WP_PCE_RESULT_INT_VLAN_UPDATE;
         rule_cfg.match_result[1].param.int_vlan.vlan_tag = 0xFFFF;
         rule_cfg.match_result[1].param.int_vlan.mask = WP_PCE_MASK_VLAN_PRIO;
         rule_cfg.match_result[2].result_type = WP_PCE_RESULT_LAST;

         vlan_update_rules[i] = WP_PceRuleCreate(WP_WINPATH(0),
                                                 WP_PCE_RULE_CLASSIFICATION,
                                                 &rule_cfg);
         WPE_TerminateOnError(vlan_update_rules[i], "WP_PceRuleCreate() ", __LINE__);
      }
      else
      {
         memset(&rule_cfg, 0, sizeof(WP_pce_rule_classification));

         rule_cfg.match_action = WP_PCE_RULE_MATCH_ACCEPT;
         status = WP_PceRuleModify(intsw_rules_handle[scenario][i], WP_PCE_RULE_MODIFY_COMMAND_MATCH_ACTION, &rule_cfg);
         WPE_TerminateOnError(status, "WP_PceRuleModify() ", __LINE__);

         status = WP_PceRuleDelete(vlan_update_rules[i]);
         WPE_TerminateOnError(status, "WP_PceRuleDelete() ", __LINE__);
      }
   }
}
static void WPE_Enable_Disable_Policer(WP_U32 enable)
{
   WP_U32 i,j=0;
   WP_pce_rule_classification rule_cfg = { 0 };
   WP_status status;
   for (i = 0; i < MAX_CLASSES; i++)
   {
      j=0;
      //creating rule for sending the generic packets to the tx channel
      memset(&rule_cfg, 0, sizeof(WP_pce_rule_classification));
      rule_cfg.match_result[j].result_type = WP_PCE_RESULT_FLOW_AGG;
      rule_cfg.match_result[j++].param.flow_agg.flow_aggregation = extsw_fa_xgi1[i];
      if (enable)
      {
         rule_cfg.match_result[j].result_type = WP_PCE_RESULT_INGRESS_POLICER;
         rule_cfg.match_result[j++].param.ingress_policer.policer = policer_handle;
      }
      rule_cfg.match_result[j].result_type = WP_PCE_RESULT_LAST;

      status = WP_PceRuleModify(intsw_rules_handle[scenario][i], WP_PCE_RULE_MODIFY_COMMAND_RESULT, &rule_cfg);
      WPE_TerminateOnError(status, "WP_PceRuleModify() ", __LINE__);
   }
}
static void WPE_Enable_Disable_Policer_Loop()
{
   WP_U32 i;
   for (i=0 ; i<1000000; i++)
   {
      if (i%100 == 0)
         printf("i = %d\n",i);
      WP_Delay(100);
      WPE_Enable_Disable_Policer(0);
      WP_Delay(100);
      WPE_Enable_Disable_Policer(1);
   }
}
static void WPE_Policer_Vlan(WP_boolean enable_disable,WP_boolean vlan_change)
{

   if (enable_disable) /*pecs*/
   {
      if (vlan_change)
      {
         WPE_ModifyPecs(WP_ENABLE,4);
      }
      else
      { /*Vlan unchanged */
         WPE_ModifyPecs(WP_ENABLE,3);
      }
   }
   else
   {
      WPE_ModifyPecs(WP_DISABLE,0);
   }
}
static void WPE_Create_Override_Rules(WP_U32 first_filter_type,WP_U32 second_filter_type)
{
   WP_pce_rule_classification rule_cfg = { 0 };
   WP_U8 i = 0;

   for (i = 0; i < MAX_CLASSES; i++)
   {
      //Rules for first XGI
      memset(&rule_cfg, 0, sizeof(WP_pce_rule_classification));
      rule_cfg.enabled = WP_ENABLE;
      rule_cfg.match_action = WP_PCE_RULE_MATCH_ACCEPT;
      rule_cfg.filter_handle = xgi1_pce_filter[CLASSIFICATION];
      rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_MAC_SA;
      rule_cfg.rule_fields[0].value.mac_addr[0] = 0x00;
      rule_cfg.rule_fields[0].value.mac_addr[1] = 0x00;
      rule_cfg.rule_fields[0].value.mac_addr[2] = 0x00;
      rule_cfg.rule_fields[0].value.mac_addr[3] = 0x00;
      rule_cfg.rule_fields[0].value.mac_addr[4] = scenario;
      rule_cfg.rule_fields[0].value.mac_addr[5] = i; /* class */
      rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_LAST;
      rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
      rule_cfg.match_result[0].param.flow_agg.flow_aggregation = extsw_fa_xgi2[i];
      rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;
      extsw_rules_handle[scenario][i] = WP_PceRuleCreate(WP_WINPATH(0),
                                               WP_PCE_RULE_CLASSIFICATION,
                                               &rule_cfg);
      WPE_TerminateOnError(extsw_rules_handle[scenario][i], "WP_PceRuleCreate() ", __LINE__);

      /* first filter rule */
      memset(&rule_cfg, 0, sizeof(WP_pce_rule_classification));
      rule_cfg.enabled = WP_ENABLE;
      rule_cfg.match_action = WP_PCE_RULE_MATCH_CONTINUE;
      rule_cfg.filter_handle = xgi2_pce_filter[CLASSIFICATION];
      rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_MAC_SA;
      rule_cfg.rule_fields[0].value.mac_addr[0] = 0x00;
      rule_cfg.rule_fields[0].value.mac_addr[1] = 0x00;
      rule_cfg.rule_fields[0].value.mac_addr[2] = 0x00;
      rule_cfg.rule_fields[0].value.mac_addr[3] = 0x00;
      rule_cfg.rule_fields[0].value.mac_addr[4] = scenario;
      rule_cfg.rule_fields[0].value.mac_addr[5] = i;
      rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_LAST;
      rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
      switch(first_filter_type)
      {
         case GENERIC_FA_TYPE:
         {
            rule_cfg.match_result[0].param.flow_agg.flow_aggregation = mixed_fa[i];
            break;
         }
         case INTSW_CASCADING_FA_TYPE:
         {
            rule_cfg.match_result[0].param.flow_agg.flow_aggregation = cascading_fa[i];
            break;
         }
         case INTSW_IW__FA_TYPE:
         {
            rule_cfg.match_result[0].param.flow_agg.flow_aggregation = extsw_fa_xgi1[i];
            break;
         }
      }
      rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;
      override_rules[0][i] = WP_PceRuleCreate(WP_WINPATH(0),
                                              WP_PCE_RULE_CLASSIFICATION,
                                              &rule_cfg);
      WPE_TerminateOnError(override_rules[0][i], "WP_PceRuleCreate() ", __LINE__);

      /* second filter rule */
      rule_cfg.match_action = WP_PCE_RULE_MATCH_ACCEPT;
      rule_cfg.filter_handle = xgi2_pce_filter[CLASSIFICATION1];
      switch(second_filter_type)
      {
         case GENERIC_FA_TYPE:
         {
            rule_cfg.match_result[0].param.flow_agg.flow_aggregation = mixed_fa[i];
            break;
         }
         case INTSW_CASCADING_FA_TYPE:
         {
            rule_cfg.match_result[0].param.flow_agg.flow_aggregation = cascading_fa[i];
            break;
         }
         case INTSW_IW__FA_TYPE:
         {
            rule_cfg.match_result[0].param.flow_agg.flow_aggregation = extsw_fa_xgi1[i];
            break;
         }
      }
      override_rules[1][i] = WP_PceRuleCreate(WP_WINPATH(0),
                                              WP_PCE_RULE_CLASSIFICATION,
                                              &rule_cfg);
      WPE_TerminateOnError(override_rules[1][i], "WP_PceRuleCreate() ", __LINE__);
   }
}
static void WPE_Modify_Override_Rules(WP_U32 first_filter_type,WP_U32 second_filter_type)
{
   WP_pce_rule_classification rule_cfg = { 0 };
   WP_U8 i = 0;
   WP_status status;

   for (i = 0; i < MAX_CLASSES; i++)
   {
      /* first filter rule */
      memset(&rule_cfg, 0, sizeof(WP_pce_rule_classification));
      rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
      rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;
      switch(first_filter_type)
      {
         case GENERIC_FA_TYPE:
         {
            rule_cfg.match_result[0].param.flow_agg.flow_aggregation = mixed_fa[i];
            break;
         }
         case INTSW_CASCADING_FA_TYPE:
         {
            rule_cfg.match_result[0].param.flow_agg.flow_aggregation = cascading_fa[i];
            break;
         }
         case INTSW_IW__FA_TYPE:
         {
            rule_cfg.match_result[0].param.flow_agg.flow_aggregation = extsw_fa_xgi1[i];
            break;
         }
      }

      status = WP_PceRuleModify(override_rules[0][i], WP_PCE_RULE_MODIFY_COMMAND_RESULT, &rule_cfg);
      WPE_TerminateOnError(status, "WP_PceRuleModify() ", __LINE__);

      /* second filter rule */
      switch(second_filter_type)
      {
         case GENERIC_FA_TYPE:
         {
            rule_cfg.match_result[0].param.flow_agg.flow_aggregation = mixed_fa[i];
            break;
         }
         case INTSW_CASCADING_FA_TYPE:
         {
            rule_cfg.match_result[0].param.flow_agg.flow_aggregation = cascading_fa[i];
            break;
         }
         case INTSW_IW__FA_TYPE:
         {
            rule_cfg.match_result[0].param.flow_agg.flow_aggregation = extsw_fa_xgi1[i];
            break;
         }
      }
      status = WP_PceRuleModify(override_rules[1][i], WP_PCE_RULE_MODIFY_COMMAND_RESULT, &rule_cfg);
      WPE_TerminateOnError(status, "WP_PceRuleModify() ", __LINE__);
   }
}
static void WPE_Initialize_Pce_Rules()
{
   WP_pce_rule_classification rule_cfg = { 0 };
   WP_U8 i = 0;

   switch (scenario)
   {
      case 1: /* bridging */
      {
         printf("Bridging scenario\n");
         for (i = 0; i < MAX_CLASSES-1; i++)
         {
            //creating rule for sending the generic packets to the tx channel
            memset(&rule_cfg, 0, sizeof(WP_pce_rule_classification));
            rule_cfg.enabled = WP_ENABLE;
            rule_cfg.match_action = WP_PCE_RULE_MATCH_ACCEPT;
            rule_cfg.filter_handle = xgi1_pce_filter[CLASSIFICATION];
            rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_MAC_SA;
            rule_cfg.rule_fields[0].value.mac_addr[0] = 0x00;
            rule_cfg.rule_fields[0].value.mac_addr[1] = 0x00;
            rule_cfg.rule_fields[0].value.mac_addr[2] = 0x00;
            rule_cfg.rule_fields[0].value.mac_addr[3] = 0x00;
            rule_cfg.rule_fields[0].value.mac_addr[4] = scenario;
            rule_cfg.rule_fields[0].value.mac_addr[5] = i; /* class */
            rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_LAST;
            rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
            rule_cfg.match_result[0].param.flow_agg.flow_aggregation = extsw_fa_xgi2[i];
            rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;
            extsw_rules_handle[scenario][i] = WP_PceRuleCreate(WP_WINPATH(0),
                                                     WP_PCE_RULE_CLASSIFICATION,
                                                     &rule_cfg);
            WPE_TerminateOnError(extsw_rules_handle[scenario][i], "WP_PceRuleCreate() ", __LINE__);

            //creating rule for sending the generic packets to the tx channel
            memset(&rule_cfg, 0, sizeof(WP_pce_rule_classification));
            rule_cfg.enabled = WP_ENABLE;
            rule_cfg.match_action = WP_PCE_RULE_MATCH_CONTINUE;
            rule_cfg.filter_handle = xgi2_pce_filter[CLASSIFICATION];
            rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_MAC_SA;
            rule_cfg.rule_fields[0].value.mac_addr[0] = 0x00;
            rule_cfg.rule_fields[0].value.mac_addr[1] = 0x00;
            rule_cfg.rule_fields[0].value.mac_addr[2] = 0x00;
            rule_cfg.rule_fields[0].value.mac_addr[3] = 0x00;
            rule_cfg.rule_fields[0].value.mac_addr[4] = scenario;
            rule_cfg.rule_fields[0].value.mac_addr[5] = i;
            rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_LAST;
            rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
            rule_cfg.match_result[0].param.flow_agg.flow_aggregation = extsw_fa_xgi1[MAX_CLASSES-1];
            rule_cfg.match_result[1].result_type = WP_PCE_RESULT_IN_IW_PORT_UPDATE;
            rule_cfg.match_result[1].param.iw_port.iw_port_handle = iwport_handle[i];
            rule_cfg.match_result[2].result_type = WP_PCE_RESULT_LAST;
            intsw_rules_handle[scenario][i] = WP_PceRuleCreate(WP_WINPATH(0),
                                               WP_PCE_RULE_CLASSIFICATION,
                                               &rule_cfg);
            WPE_TerminateOnError(intsw_rules_handle[scenario][i], "WP_PceRuleCreate() ", __LINE__);
         }
         break;
      }
      case 2: /* policer */
      {
         printf("Policing scenario\n");
         for (i = 0; i < MAX_CLASSES; i++)
         {
            //creating rule for sending the generic packets to the tx channel
            memset(&rule_cfg, 0, sizeof(WP_pce_rule_classification));
            rule_cfg.enabled = WP_ENABLE;
            rule_cfg.match_action = WP_PCE_RULE_MATCH_ACCEPT;
            rule_cfg.filter_handle = xgi1_pce_filter[CLASSIFICATION];
            rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_MAC_SA;
            rule_cfg.rule_fields[0].value.mac_addr[0] = 0x00;
            rule_cfg.rule_fields[0].value.mac_addr[1] = 0x00;
            rule_cfg.rule_fields[0].value.mac_addr[2] = 0x00;
            rule_cfg.rule_fields[0].value.mac_addr[3] = 0x00;
            rule_cfg.rule_fields[0].value.mac_addr[4] = scenario;
            rule_cfg.rule_fields[0].value.mac_addr[5] = i; /* class */
            rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_LAST;
            rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
            rule_cfg.match_result[0].param.flow_agg.flow_aggregation = extsw_fa_xgi2[i];
            rule_cfg.match_result[1].result_type = WP_PCE_RESULT_INGRESS_POLICER;
            rule_cfg.match_result[1].param.ingress_policer.policer = policer_handle;
            rule_cfg.match_result[2].result_type = WP_PCE_RESULT_LAST;
            extsw_rules_handle[scenario][i] = WP_PceRuleCreate(WP_WINPATH(0),
                                                     WP_PCE_RULE_CLASSIFICATION,
                                                     &rule_cfg);
            WPE_TerminateOnError(extsw_rules_handle[scenario][i], "WP_PceRuleCreate() ", __LINE__);

            //creating rule for sending the generic packets to the tx channel
            memset(&rule_cfg, 0, sizeof(WP_pce_rule_classification));
            rule_cfg.enabled = WP_ENABLE;
            rule_cfg.match_action = WP_PCE_RULE_MATCH_ACCEPT;
            rule_cfg.filter_handle = xgi2_pce_filter[CLASSIFICATION];
            rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_MAC_SA;
            rule_cfg.rule_fields[0].value.mac_addr[0] = 0x00;
            rule_cfg.rule_fields[0].value.mac_addr[1] = 0x00;
            rule_cfg.rule_fields[0].value.mac_addr[2] = 0x00;
            rule_cfg.rule_fields[0].value.mac_addr[3] = 0x00;
            rule_cfg.rule_fields[0].value.mac_addr[4] = scenario;
            rule_cfg.rule_fields[0].value.mac_addr[5] = i;
            rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_LAST;
            rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
            rule_cfg.match_result[0].param.flow_agg.flow_aggregation = extsw_fa_xgi1[i];
            rule_cfg.match_result[1].result_type = WP_PCE_RESULT_INGRESS_POLICER;
            rule_cfg.match_result[1].param.ingress_policer.policer = policer_handle;
            rule_cfg.match_result[2].result_type = WP_PCE_RESULT_LAST;
            intsw_rules_handle[scenario][i] = WP_PceRuleCreate(WP_WINPATH(0),
                                               WP_PCE_RULE_CLASSIFICATION,
                                               &rule_cfg);
            WPE_TerminateOnError(intsw_rules_handle[scenario][i], "WP_PceRuleCreate() ", __LINE__);
         }
         break;
      }
      case 3: /* performance */
      {
         printf("Performance scenario\n");
         for (i = 0; i < MAX_CLASSES; i++)
         {
            //creating rule for sending the generic packets to the tx channel
            memset(&rule_cfg, 0, sizeof(WP_pce_rule_classification));
            rule_cfg.enabled = WP_ENABLE;
            rule_cfg.match_action = WP_PCE_RULE_MATCH_ACCEPT;
            rule_cfg.filter_handle = xgi1_pce_filter[CLASSIFICATION];
            rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_MAC_SA;
            rule_cfg.rule_fields[0].value.mac_addr[0] = 0x00;
            rule_cfg.rule_fields[0].value.mac_addr[1] = 0x00;
            rule_cfg.rule_fields[0].value.mac_addr[2] = 0x00;
            rule_cfg.rule_fields[0].value.mac_addr[3] = 0x00;
            rule_cfg.rule_fields[0].value.mac_addr[4] = scenario;
            rule_cfg.rule_fields[0].value.mac_addr[5] = i; /* class */
            rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_LAST;
            rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
            rule_cfg.match_result[0].param.flow_agg.flow_aggregation = extsw_fa_xgi2[i];
            rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;
            extsw_rules_handle[scenario][i] = WP_PceRuleCreate(WP_WINPATH(0),
                                                     WP_PCE_RULE_CLASSIFICATION,
                                                     &rule_cfg);
            WPE_TerminateOnError(extsw_rules_handle[scenario][i], "WP_PceRuleCreate() ", __LINE__);

            //creating rule for sending the generic packets to the tx channel
            memset(&rule_cfg, 0, sizeof(WP_pce_rule_classification));
            rule_cfg.enabled = WP_ENABLE;
            rule_cfg.match_action = WP_PCE_RULE_MATCH_ACCEPT;
            rule_cfg.filter_handle = xgi2_pce_filter[CLASSIFICATION];
            rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_MAC_SA;
            rule_cfg.rule_fields[0].value.mac_addr[0] = 0x00;
            rule_cfg.rule_fields[0].value.mac_addr[1] = 0x00;
            rule_cfg.rule_fields[0].value.mac_addr[2] = 0x00;
            rule_cfg.rule_fields[0].value.mac_addr[3] = 0x00;
            rule_cfg.rule_fields[0].value.mac_addr[4] = scenario;
            rule_cfg.rule_fields[0].value.mac_addr[5] = i;
            rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_LAST;
            rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
            rule_cfg.match_result[0].param.flow_agg.flow_aggregation = extsw_fa_xgi1[i];
            rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;
            intsw_rules_handle[scenario][i] = WP_PceRuleCreate(WP_WINPATH(0),
                                               WP_PCE_RULE_CLASSIFICATION,
                                               &rule_cfg);
            WPE_TerminateOnError(intsw_rules_handle[scenario][i], "WP_PceRuleCreate() ", __LINE__);

         }
         break;
      }
      case 4: /* Mixed device */
      {
         printf("Mixed device scenario\n");
         for (i = 0; i < MAX_CLASSES; i++)
         {
            //creating rule for sending the generic packets to the tx channel
            memset(&rule_cfg, 0, sizeof(WP_pce_rule_classification));
            rule_cfg.enabled = WP_ENABLE;
            rule_cfg.match_action = WP_PCE_RULE_MATCH_ACCEPT;
            rule_cfg.filter_handle = xgi1_pce_filter[CLASSIFICATION];
            rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_MAC_SA;
            rule_cfg.rule_fields[0].value.mac_addr[0] = 0x00;
            rule_cfg.rule_fields[0].value.mac_addr[1] = 0x00;
            rule_cfg.rule_fields[0].value.mac_addr[2] = 0x00;
            rule_cfg.rule_fields[0].value.mac_addr[3] = 0x00;
            rule_cfg.rule_fields[0].value.mac_addr[4] = scenario;
            rule_cfg.rule_fields[0].value.mac_addr[5] = i; /* class */
            rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_LAST;
            rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
            rule_cfg.match_result[0].param.flow_agg.flow_aggregation = extsw_fa_xgi2[i];
            rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;
            extsw_rules_handle[scenario][i] = WP_PceRuleCreate(WP_WINPATH(0),
                                                        WP_PCE_RULE_CLASSIFICATION,
                                                        &rule_cfg);
            WPE_TerminateOnError(extsw_rules_handle[scenario][i], "WP_PceRuleCreate() ", __LINE__);

            //creating rule for sending the generic packets to the tx channel
            memset(&rule_cfg, 0, sizeof(WP_pce_rule_classification));
            rule_cfg.enabled = WP_ENABLE;
            rule_cfg.match_action = WP_PCE_RULE_MATCH_ACCEPT;
            rule_cfg.filter_handle = xgi2_pce_filter[CLASSIFICATION];
            rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_MAC_SA;
            rule_cfg.rule_fields[0].value.mac_addr[0] = 0x00;
            rule_cfg.rule_fields[0].value.mac_addr[1] = 0x00;
            rule_cfg.rule_fields[0].value.mac_addr[2] = 0x00;
            rule_cfg.rule_fields[0].value.mac_addr[3] = 0x00;
            rule_cfg.rule_fields[0].value.mac_addr[4] = scenario;
            rule_cfg.rule_fields[0].value.mac_addr[5] = i;
            rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_LAST;
            rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
            rule_cfg.match_result[0].param.flow_agg.flow_aggregation = extsw_fa_xgi1[i];
            rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;
            intsw_rules_handle[scenario][i] = WP_PceRuleCreate(WP_WINPATH(0),
                                               WP_PCE_RULE_CLASSIFICATION,
                                               &rule_cfg);
            WPE_TerminateOnError(intsw_rules_handle[scenario][i], "WP_PceRuleCreate() ", __LINE__);

            //creating rule for sending the generic packets to the tx channel
            memset(&rule_cfg, 0, sizeof(WP_pce_rule_classification));
            rule_cfg.enabled = WP_ENABLE;
            rule_cfg.match_action = WP_PCE_RULE_MATCH_ACCEPT;
            rule_cfg.filter_handle = xgi1_pce_filter[CLASSIFICATION];
            rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_MAC_SA;
            rule_cfg.rule_fields[0].value.mac_addr[0] = 0x00;
            rule_cfg.rule_fields[0].value.mac_addr[1] = 0x00;
            rule_cfg.rule_fields[0].value.mac_addr[2] = 0x00;
            rule_cfg.rule_fields[0].value.mac_addr[3] = 0x01;
            rule_cfg.rule_fields[0].value.mac_addr[4] = scenario;
            rule_cfg.rule_fields[0].value.mac_addr[5] = i;
            rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_LAST;
            rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
            rule_cfg.match_result[0].param.flow_agg.flow_aggregation = extsw_fa_xgi2[i];
            rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;
            mixed_rules_handle[0][i] = WP_PceRuleCreate(WP_WINPATH(0),
                                                        WP_PCE_RULE_CLASSIFICATION,
                                                        &rule_cfg);
            WPE_TerminateOnError(mixed_rules_handle[0][i], "WP_PceRuleCreate() ", __LINE__);

            //creating rule for sending the generic packets to the tx channel
            memset(&rule_cfg, 0, sizeof(WP_pce_rule_classification));
            rule_cfg.enabled = WP_ENABLE;
            rule_cfg.match_action = WP_PCE_RULE_MATCH_ACCEPT;
            rule_cfg.filter_handle = xgi2_pce_filter[CLASSIFICATION];
            rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_MAC_SA;
            rule_cfg.rule_fields[0].value.mac_addr[0] = 0x00;
            rule_cfg.rule_fields[0].value.mac_addr[1] = 0x00;
            rule_cfg.rule_fields[0].value.mac_addr[2] = 0x00;
            rule_cfg.rule_fields[0].value.mac_addr[3] = 0x01;
            rule_cfg.rule_fields[0].value.mac_addr[4] = scenario;
            rule_cfg.rule_fields[0].value.mac_addr[5] = i;
            rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_LAST;
            rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
            rule_cfg.match_result[0].param.flow_agg.flow_aggregation = mixed_fa[i];
            rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;
            mixed_rules_handle[1][i] = WP_PceRuleCreate(WP_WINPATH(0),
                                                        WP_PCE_RULE_CLASSIFICATION,
                                                        &rule_cfg);
            WPE_TerminateOnError(mixed_rules_handle[1][i], "WP_PceRuleCreate() ", __LINE__);
         }
         break;
      }
      case 6: /* mpe shaping */
      {
         printf("MPE Shaping scenario\n");
         for (i = 0; i < 3; i++)
         {
            //creating rule for sending the generic packets to the tx channel
            memset(&rule_cfg, 0, sizeof(WP_pce_rule_classification));
            rule_cfg.enabled = WP_ENABLE;
            rule_cfg.match_action = WP_PCE_RULE_MATCH_ACCEPT;
            rule_cfg.filter_handle = xgi1_pce_filter[CLASSIFICATION];
            rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_MAC_SA;
            rule_cfg.rule_fields[0].value.mac_addr[0] = 0x00;
            rule_cfg.rule_fields[0].value.mac_addr[1] = 0x00;
            rule_cfg.rule_fields[0].value.mac_addr[2] = 0x00;
            rule_cfg.rule_fields[0].value.mac_addr[3] = 0x00;
            rule_cfg.rule_fields[0].value.mac_addr[4] = scenario;
            rule_cfg.rule_fields[0].value.mac_addr[5] = i; /* class */
            rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_LAST;
            rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
            rule_cfg.match_result[0].param.flow_agg.flow_aggregation = extsw_fa_xgi2[i];
            rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;
            extsw_rules_handle[scenario][i] = WP_PceRuleCreate(WP_WINPATH(0),
                                                     WP_PCE_RULE_CLASSIFICATION,
                                                     &rule_cfg);
            WPE_TerminateOnError(extsw_rules_handle[scenario][i], "WP_PceRuleCreate() ", __LINE__);

            //creating rule for sending the generic packets to the tx channel
            memset(&rule_cfg, 0, sizeof(WP_pce_rule_classification));
            rule_cfg.enabled = WP_ENABLE;
            rule_cfg.match_action = WP_PCE_RULE_MATCH_ACCEPT;
            rule_cfg.filter_handle = xgi2_pce_filter[CLASSIFICATION];
            rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_MAC_SA;
            rule_cfg.rule_fields[0].value.mac_addr[0] = 0x00;
            rule_cfg.rule_fields[0].value.mac_addr[1] = 0x00;
            rule_cfg.rule_fields[0].value.mac_addr[2] = 0x00;
            rule_cfg.rule_fields[0].value.mac_addr[3] = 0x00;
            rule_cfg.rule_fields[0].value.mac_addr[4] = scenario;
            rule_cfg.rule_fields[0].value.mac_addr[5] = i;
            rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_LAST;
            rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
            rule_cfg.match_result[0].param.flow_agg.flow_aggregation = mpe_fa[i];
            rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;
            intsw_rules_handle[scenario][i] = WP_PceRuleCreate(WP_WINPATH(0),
                                               WP_PCE_RULE_CLASSIFICATION,
                                               &rule_cfg);
            WPE_TerminateOnError(intsw_rules_handle[scenario][i], "WP_PceRuleCreate() ", __LINE__);

         }
         break;
      }
      case 7: /* performance */
      {
         printf("Performance scenario with pecs\n");
         for (i = 0; i < MAX_CLASSES; i++)
         {
            //creating rule for sending the generic packets to the tx channel
            memset(&rule_cfg, 0, sizeof(WP_pce_rule_classification));
            rule_cfg.enabled = WP_ENABLE;
            rule_cfg.match_action = WP_PCE_RULE_MATCH_ACCEPT;
            rule_cfg.filter_handle = xgi1_pce_filter[CLASSIFICATION];
            rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_MAC_SA;
            rule_cfg.rule_fields[0].value.mac_addr[0] = 0x00;
            rule_cfg.rule_fields[0].value.mac_addr[1] = 0x00;
            rule_cfg.rule_fields[0].value.mac_addr[2] = 0x00;
            rule_cfg.rule_fields[0].value.mac_addr[3] = 0x00;
            rule_cfg.rule_fields[0].value.mac_addr[4] = scenario;
            rule_cfg.rule_fields[0].value.mac_addr[5] = i; /* class */
            rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_LAST;
            rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
            rule_cfg.match_result[0].param.flow_agg.flow_aggregation = extsw_fa_xgi2[i];
            rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;
            extsw_rules_handle[scenario][i] = WP_PceRuleCreate(WP_WINPATH(0),
                                                     WP_PCE_RULE_CLASSIFICATION,
                                                     &rule_cfg);
            WPE_TerminateOnError(extsw_rules_handle[scenario][i], "WP_PceRuleCreate() ", __LINE__);

            //creating rule for sending the generic packets to the tx channel
            memset(&rule_cfg, 0, sizeof(WP_pce_rule_classification));
            rule_cfg.enabled = WP_ENABLE;
            rule_cfg.match_action = WP_PCE_RULE_MATCH_ACCEPT;
            rule_cfg.filter_handle = xgi2_pce_filter[CLASSIFICATION];
            rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_MAC_SA;
            rule_cfg.rule_fields[0].value.mac_addr[0] = 0x00;
            rule_cfg.rule_fields[0].value.mac_addr[1] = 0x00;
            rule_cfg.rule_fields[0].value.mac_addr[2] = 0x00;
            rule_cfg.rule_fields[0].value.mac_addr[3] = 0x00;
            rule_cfg.rule_fields[0].value.mac_addr[4] = scenario;
            rule_cfg.rule_fields[0].value.mac_addr[5] = i;
            rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_LAST;
            rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
            rule_cfg.match_result[0].param.flow_agg.flow_aggregation = intsw_fa_w_pecs[i];
            rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;
            intsw_rules_handle[scenario][i] = WP_PceRuleCreate(WP_WINPATH(0),
                                               WP_PCE_RULE_CLASSIFICATION,
                                               &rule_cfg);
            WPE_TerminateOnError(intsw_rules_handle[scenario][i], "WP_PceRuleCreate() ", __LINE__);

         }
         break;
      }
      default:
      {
         printf("\nNo such secnario\n\n");
      }
   }

}
static void WPE_Initialize_Pce_Interface()
{
   WP_status status;
   WP_pce_filter_set fs_level0 = {0};
   WP_pce_filter_classification pce_filter_classification = {0};
   WP_pce_filter_forwarding pce_filter_forwarding = {0};
   WP_pce_filter_learning pce_learning_filter = {0};
   WP_U8 i;

   //create classification filter
   memset(&pce_filter_classification, 0, sizeof(WP_pce_filter_classification));
   pce_filter_classification.no_match_action = WP_PCE_FILTER_NO_MATCH_CONTINUE;
   pce_filter_classification.no_fields_action = WP_PCE_FILTER_NO_FIELDS_DENY;
   pce_filter_classification.no_match_result[0].result_type = WP_PCE_RESULT_LAST;

   pce_filter_classification.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   pce_filter_classification.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;
   pce_filter_classification.filter_fields[0].field_id = WP_PCE_FIELD_ID_MAC_SA;

   pce_filter_classification.filter_fields[1].field_id = WP_PCE_FIELD_ID_LAST;

   /***** XGI1 PCE I/F *********/
   xgi1_pce_filter[CLASSIFICATION] = WP_PceFilterCreate(WP_WINPATH(0),
                                                         WP_PCE_FILTER_CLASSIFICATION,
                                                         &pce_filter_classification);
   WPE_TerminateOnError(xgi1_pce_filter[CLASSIFICATION], "WP_PceFilterCreate() ", __LINE__);

   /***** XGI2 PCE I/F *********/

   xgi2_pce_filter[CLASSIFICATION] = WP_PceFilterCreate(WP_WINPATH(0),
                                                   WP_PCE_FILTER_CLASSIFICATION,
                                                   &pce_filter_classification);
   WPE_TerminateOnError(xgi2_pce_filter[CLASSIFICATION], "WP_PceFilterCreate() ", __LINE__);

   printf("xgi2_pce_filter[CLASSIFICATION] handle =0x%x\n", xgi2_pce_filter[CLASSIFICATION]);

   xgi2_pce_filter[CLASSIFICATION1] = WP_PceFilterCreate(WP_WINPATH(0),
                                                   WP_PCE_FILTER_CLASSIFICATION,
                                                   &pce_filter_classification);
   WPE_TerminateOnError(xgi2_pce_filter[CLASSIFICATION1], "WP_PceFilterCreate() ", __LINE__);

   printf("xgi2_pce_filter[CLASSIFICATION] handle =0x%x\n", xgi2_pce_filter[CLASSIFICATION]);

   //create forwarding filter
   pce_filter_forwarding.no_match_action = WP_PCE_FILTER_NO_MATCH_CONTINUE;
   pce_filter_forwarding.no_fields_action = WP_PCE_FILTER_NO_FIELDS_CONTINUE;
   pce_filter_forwarding.no_match_result[0].result_type = WP_PCE_RESULT_LAST;

   pce_filter_forwarding.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   pce_filter_forwarding.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;
   pce_filter_forwarding.filter_fields[0].field_id = WP_PCE_FIELD_ID_MAC_DA;

   pce_filter_forwarding.filter_fields[1].field_id = WP_PCE_FIELD_ID_LAST;

   xgi2_pce_filter[FORWARDING] = WP_PceFilterCreate(WP_WINPATH(0),
                                               WP_PCE_FILTER_FORWARDING,
                                               &pce_filter_forwarding);
   WPE_TerminateOnError(xgi2_pce_filter[FORWARDING], "WP_PceFilterCreate() ", __LINE__);

   printf("xgi2_pce_filter[FORWARDING] handle =0x%x\n", xgi2_pce_filter[FORWARDING]);

   //create learning filter
   pce_learning_filter.no_match_action = WP_PCE_FILTER_NO_MATCH_CONTINUE;
   pce_learning_filter.no_fields_action = WP_PCE_FILTER_NO_FIELDS_CONTINUE;
   pce_learning_filter.no_match_result[0].result_type = WP_PCE_RESULT_LAST;

   pce_learning_filter.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   pce_learning_filter.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;
   pce_learning_filter.filter_fields[0].field_id = WP_PCE_FIELD_ID_MAC_SA;

   pce_learning_filter.filter_fields[1].field_id = WP_PCE_FIELD_ID_LAST;

   pce_learning_filter.forwarding_filter = xgi2_pce_filter[FORWARDING];

   xgi2_pce_filter[LEARNING] = WP_PceFilterCreate(WP_WINPATH(0),
                                             WP_PCE_FILTER_LEARNING,
                                             &pce_learning_filter);
   WPE_TerminateOnError(xgi2_pce_filter[LEARNING], "WP_PceFilterCreate() ", __LINE__);

   printf("xgi2_pce_filter[LEARNING] handle =0x%x\n", xgi2_pce_filter[LEARNING]);

   //creating first filter sets
   memset(&fs_level0, 0, sizeof(WP_pce_filter_set));
   fs_level0.filter_set_level = 0;
   fs_level0.next_filter_set = WP_UNUSED;
   fs_level0.filters[0] = xgi1_pce_filter[CLASSIFICATION];
   fs_level0.filters[1] = WP_UNUSED;
   xgi1_filter_set_handle = WP_PceFilterSetCreate(WP_WINPATH(0), &fs_level0);
   WPE_TerminateOnError(xgi1_filter_set_handle, "WP_PceFilterSetCreate() ", __LINE__);

   //creating first filter sets
   memset(&fs_level0, 0, sizeof(WP_pce_filter_set));
   fs_level0.filter_set_level = 0;
   fs_level0.next_filter_set = WP_UNUSED;
   fs_level0.filters[0] = xgi2_pce_filter[CLASSIFICATION];
   fs_level0.filters[1] = xgi2_pce_filter[CLASSIFICATION1];
   fs_level0.filters[2] = xgi2_pce_filter[LEARNING];
   fs_level0.filters[3] = xgi2_pce_filter[FORWARDING];
   fs_level0.filters[4] = WP_UNUSED;
   xgi2_filter_set_handle = WP_PceFilterSetCreate(WP_WINPATH(0), &fs_level0);
   WPE_TerminateOnError(xgi2_filter_set_handle, "WP_PceFilterSetCreate() ", __LINE__);

   //creating binding to rx channel
   memset(&pce_if_params, 0, sizeof(WP_pce_if_params_pkt_rx_channel));
   pce_if_params.mode = WP_PCE_IW_PORT_CONNECTION_ENABLED;
   pce_if_params.filter_set_handle = xgi1_filter_set_handle;
   pce_if_params.ip_header_validation = WP_DISABLE;
   pce_if_params.sub_port_vlan = WP_DISABLE;
   pce_if_params.parser_start_type = WP_PCE_PARSER_START_TYPE_ETHERNET;

   xgi1_pce_if_handle = WP_PceInterfaceCreate(WP_PCE_IF_TYPE_PKT_RX_CHANNEL, &pce_if_params);
   WPE_TerminateOnError(xgi1_pce_if_handle, "WP_PceInterfaceCreate", __LINE__);

   rx_binding_bridging_cfg->pce_if_handle = xgi1_pce_if_handle;
   rx_binding_bridging_cfg->input_port = bport_handle[0];
   status = WP_IwRxBindingCreate(xgi1_rx_channel,
                                 iw_system,
                                 qnodes[QNODE_RX_IW],
                                 &rx_binding_bridging_cfg[0]);
   WPE_TerminateOnError(status, "WP_IwRxBindingCreate enet", __LINE__);

   //creating binding to rx channel
   memset(&pce_if_params, 0, sizeof(WP_pce_if_params_pkt_rx_channel));
   pce_if_params.mode = WP_PCE_IW_PORT_CONNECTION_ENABLED;
   pce_if_params.filter_set_handle = xgi2_filter_set_handle;
   pce_if_params.ip_header_validation = WP_DISABLE;
   pce_if_params.sub_port_vlan = WP_DISABLE;
   pce_if_params.parser_start_type = WP_PCE_PARSER_START_TYPE_ETHERNET;

   xgi2_pce_if_handle = WP_PceInterfaceCreate(WP_PCE_IF_TYPE_PKT_RX_CHANNEL, &pce_if_params);
   WPE_TerminateOnError(xgi2_pce_if_handle, "WP_PceInterfaceCreate", __LINE__);

   rx_binding_bridging_cfg->pce_if_handle = xgi2_pce_if_handle;
   rx_binding_bridging_cfg->input_port = bport_handle[1];

   status = WP_IwRxBindingCreate(xgi2_rx_channel,
                                 iw_system,
                                 qnodes[QNODE_RX_IW],
                                 &rx_binding_bridging_cfg[0]);
   WPE_TerminateOnError(status, "WP_IwRxBindingModify rx_enet_handle1", __LINE__);

   for (i = 0; i < NUM_OF_FILTERS; i++)
   {
      status = WP_PceFilterStatisticsReset(xgi2_pce_filter[i], &(xgi2_filter_stats[i]));
      WPE_TerminateOnError(status, "WP_PceFilterStatisticsReset()", __LINE__);
   }
   status = WP_PceFilterStatisticsReset(xgi1_pce_filter[CLASSIFICATION], &(xgi1_filter_stats[CLASSIFICATION]));
   WPE_TerminateOnError(status, "WP_PceFilterStatisticsReset()", __LINE__);
} /* WPE_Initialize_Pce_Interface */
static void WPE_SystemEnable(void)
{
   WP_status status;
   WP_U32 i = 0;

   status = WP_IwSystemBuild(iw_system);
   WPE_TerminateOnError(status, " WP_IwSystemBuild()", __LINE__);

   status = WP_SysAsuEnable(WP_SYSHANDLE(WP_WINPATH(0)));
   WPE_TerminateOnError(status, "WP_SysAsuEnable()", __LINE__);

   status = WP_PortEnable(xgi1_port_handle, WP_DIRECTION_DUPLEX);
   WPE_TerminateOnError(status, "WP_PortEnable() Enet13", __LINE__);

   status = WP_DeviceEnable(xgi1_device_handle, WP_DIRECTION_DUPLEX);
   WPE_TerminateOnError(status, "WP_DeviceEnable enetdev", __LINE__);

   status = WP_PortEnable(xgi2_port_handle, WP_DIRECTION_DUPLEX);
   WPE_TerminateOnError(status, "WP_PortEnable() Enet13", __LINE__);

   status = WP_DeviceEnable(xgi2_device_handle, WP_DIRECTION_DUPLEX);
   WPE_TerminateOnError(status, "WP_DeviceEnable enetdev", __LINE__);

   WPE_ChannelsEnable();
} /* WPE_SystemEnable */
static void WPE_ChannelsEnable()
{
   WP_status status;
   WP_U8 i;

   status = WP_ChannelEnable(xgi1_rx_channel);
   WPE_TerminateOnError(status, "WP_ChannelEnable Rx XGI1", __LINE__);

   status = WP_ChannelEnable(xgi2_rx_channel);
   WPE_TerminateOnError(status, "WP_ChannelEnable Rx XGI2", __LINE__);

   for (i = 0; i < MAX_CLASSES; i++)
   {
      status = WP_ChannelEnable(extsw_tx_channel_xgi1[i]);
      WPE_TerminateOnError(status, "WP_ChannelEnable Tx XGI", __LINE__);

      status = WP_ChannelEnable(extsw_tx_channel_xgi2[i]);
      WPE_TerminateOnError(status, "WP_ChannelEnable Tx XGI", __LINE__);

   }
} /* WPE_ChannelsEnable*/
static void WPE_CreateFlowAggregation()
{
   WP_iw_agg_generic tx_agg_gen[1] =
   {
         {
               /*tag*/1,
               /*txfunc*/0,
               /*iw_port*/0,
               /*rfcs*/WP_IW_RFCS_DISABLE,
               /*interruptqueue;*/WP_IW_IRQT1,
               /*error_pkt_mode*/WP_IW_ERRPKT_DISCARD,
               /*intmode;*/WP_IW_INT_DISABLE,
               /*statmode;*/WP_IW_STAT_ENABLE,
               /*timestamp_mode;*/WP_IW_TIME_STAMP_DISABLE,
               /*mtu;*/9600,
               /*flow_agg_type*/WP_IW_AGG_PRIMARY,
               /*policer_handle;*/0,
               /*pecs_handle;*/0,
               /*pecs_flow_info;*/0,
               /*pecs_global_info_handle*/0,
               /*iw_gpmt_handle*/0
         }
   };

   WP_U8 i;

   /* Internal Switching Flow Aggregations */
   for (i = 0; i < MAX_CLASSES; i++)
   {
      tx_agg_gen[0].tag = i;
      tx_agg_gen[0].txfunc = extsw_tx_channel_xgi1[i];
      tx_agg_gen[0].iw_port = iwport_handle[i];
      tx_agg_gen[0].statmode = WP_IW_STAT_ENABLE;

      extsw_fa_xgi1[i] = WP_IwFlowAggregationCreate(WP_SYSHANDLE(WP_WINPATH(0)),
                                                    WP_IW_GENERIC_MODE,
                                                    &tx_agg_gen[0]);
      WPE_TerminateOnError(extsw_fa_xgi1[i], "WP_IwFlowAggregationCreate", __LINE__);
//      tx_agg_intsw_cascading[0].tag = i;
//      tx_agg_intsw_cascading[0].txfunc = extsw_tx_channel_xgi1[i];
//      intsw_fa[i] = WP_IwFlowAggregationCreate(WP_SYSHANDLE(WP_WINPATH(0)),
//                                               WP_IW_INTERNAL_SWITCHING_PASS_THROUGH_MODE,
//                                               &tx_agg_intsw_cascading[0]);
//      WPE_TerminateOnError(intsw_fa[i], "WP_IwFlowAggregationCreate", __LINE__);
   }

   for (i = 0; i < MAX_CLASSES; i++)
   {
      tx_agg_gen[0].tag = i;
      tx_agg_gen[0].txfunc = extsw_tx_channel_xgi2[i];
      tx_agg_gen[0].iw_port = iwport_handle[i];
      tx_agg_gen[0].statmode = WP_IW_STAT_ENABLE;

      extsw_fa_xgi2[i] = WP_IwFlowAggregationCreate(WP_SYSHANDLE(WP_WINPATH(0)),
                                                    WP_IW_GENERIC_MODE,
                                                    &tx_agg_gen[0]);
      WPE_TerminateOnError(extsw_fa_xgi2[i], "WP_IwFlowAggregationCreate", __LINE__);
   }

} /* WPE_CreateFlowAggregation */
//static void WPE_CreateIngressPolicers()
//{
//   WP_policer_action policer_action =
//   {
//         {/* green */
//               /*WP_POLICER_ACTION_PRIORITY_MAPPING | */WP_POLICER_ACTION_PREFIX_REMARKING,
//               {
//                     {WP_POLICER_ACTION_PREFIX_REMARKING_TYPE_EXT_VLAN_PRIO,
//                           WP_POLICER_ACTION_PREFIX_REMARKING_TYPE_INT_VLAN_PRIO,
//                           WP_POLICER_ACTION_PREFIX_REMARKING_TYPE_DO_NOT_CHANGE,
//                           WP_POLICER_ACTION_PREFIX_REMARKING_TYPE_TOS
//                     },
//                     {
//                           3, 3, 3, 3
//                     }
//               },
//               /* priority_mapping_value */ 0x13,
//               /* congestion_profile_entry */ 0
//         },
//         {/* yellow */
//               /*WP_POLICER_ACTION_PRIORITY_MAPPING | */WP_POLICER_ACTION_PREFIX_REMARKING,
//               {
//                     {WP_POLICER_ACTION_PREFIX_REMARKING_TYPE_EXT_VLAN_PRIO,
//                           WP_POLICER_ACTION_PREFIX_REMARKING_TYPE_INT_VLAN_PRIO,
//                           WP_POLICER_ACTION_PREFIX_REMARKING_TYPE_DO_NOT_CHANGE,
//                           WP_POLICER_ACTION_PREFIX_REMARKING_TYPE_TOS
//                     },
//                     {
//                           4, 4, 4, 4
//                     }
//               },
//               /* priority_mapping_value */ 0x14,
//               /* congestion_profile_entry */ 0
//         },
//         {/* red */
//               /*WP_POLICER_ACTION_PRIORITY_MAPPING | */WP_POLICER_ACTION_PREFIX_REMARKING,
//               {
//                     {WP_POLICER_ACTION_PREFIX_REMARKING_TYPE_EXT_VLAN_PRIO,
//                           WP_POLICER_ACTION_PREFIX_REMARKING_TYPE_INT_VLAN_PRIO,
//                           WP_POLICER_ACTION_PREFIX_REMARKING_TYPE_DO_NOT_CHANGE,
//                           WP_POLICER_ACTION_PREFIX_REMARKING_TYPE_TOS
//                     },
//                     {
//                           5, 5, 5, 5
//                     }
//               },
//               /* priority_mapping_value */ 0x15,
//               /* congestion_profile_entry */ 0
//         },
//   };
//
//   WP_policer_v2 pv2 =
//   {
//   /* method */WP_POLICER_METHOD_MEF_WITH_COUPLING_FLAG/*WP_POLICER_METHOD_SINGLE_LEAKY_BUCKET*/,
//#ifdef WP_BOARD_WDS3_SL
//         /* type */WP_POLICER_V2_TYPE_EXTERNAL,
//#else
//         /* type */WP_POLICER_V2_TYPE_INTERNAL,
//#endif
//         /* color_mode */WP_POLICER_COLOR_AWARE,
//         /* red_packets_action */WP_POLICER_DROP_RED_PACKETS,
//         /* cir */1,
//         /* cbs */2,
//         /* eir */3,
//         /* ebs */4,
//         /* policer_action */0,
//         /* frame_len_correction */WP_POLICER_LENGTH_CORRECT_4B
//   };
//
//   papt_handle = WP_PolicerActionCreate(WP_WINPATH(0), &policer_action);
//   WPE_TerminateOnError(papt_handle, "WP_PolicerActionCreate", __LINE__);
//
//   /* this policer would be associated with 10Gbps RX interface */
//   pv2.cir = 125000000 /* 12.5% *//*325853658*/; /* 30% of 10Gbps */
//   pv2.eir = 200000000/* 20% *//*551870912*/; /* 50% of 10Gbps */
//   pv2.cbs = 131070; /* 3840 bytes */
//   pv2.ebs = 131070;
//
//   if ()
//   {
//      pv2.policer_action = 0;
//      pv2.method = WP_POLICER_METHOD_SINGLE_LEAKY_BUCKET;
//   }
//   else
//   {
//      pv2.policer_action = papt_handle;
//      pv2.method = WP_POLICER_METHOD_MEF_WITH_COUPLING_FLAG;
//   }
//
//   policer_handle = WP_PolicerCreate(WP_WINPATH(0), WP_POLICER_V2, &pv2);
//   WPE_TerminateOnError(policer_handle, "WP_PolicerCreate", __LINE__);
//
//
//}
static void WPE_IWSystemCreate()
{
   WP_gpe_pecs gpe_pecs_cfg[1] =
   {
         {
               &brouter_pecs_pecs_image, /*pecs_image_info*/
               &WPE_GpeBrouterPecsService /*pecs_service_fp*/
         }
   };
   WP_dfc_sys_info dfc_sys_info[1] =
   {
         {
               /* classification_hash_size */WP_IW_2K_HASH_ENTRIES,
               /* port_filtering */WP_IW_PORT_FILTERING_INPUT,
               /* deny_eq_ip */WP_IW_DENY_EQUAL_IP_ENABLE
         }
   };
   WP_iw_sys_bridging iw_sys_one[1] =
   {
         {
               /* tag */0, /* max_flows; */MAX_FLOWS,
               /* classification_mode */WP_IW_NO_CLASSIFIER,
               /* classifier_config */
               { 0, 0, 0, NULL },
               /* learning_mode;*/WP_IW_BRIDGE_LEARNING_DISABLE,
               /* learning_queue */
               {
                     /*int_queue_num*/WP_IW_IRQT0,
                     /*learning_queue_size*/0,
                     /*interrupt_enbale*/WP_LEARNING_POLLING_MODE,
                     /*interrupt_rate*/1 },
                     /*forwarding_table_size*/WP_IW_32K_HASH_ENTRIES,
                     /*member_set_size*/WP_IW_32K_HASH_ENTRIES,
                     /*buffer_gap;*/0x40, /*max_bridging_ports*/MAX_BPORTS,
                     /*dfc_info ptr*/&dfc_sys_info[0]
         }
   };
   /* Create IW Bridge System */
   iw_system = WP_IwSystemCreate(WP_WINPATH(0), WP_IW_VLAN_AWARE_BRIDGING_MODE, &iw_sys_one);
   WPE_TerminateOnError(iw_system, "WP_IwSystemCreate", __LINE__);

   WPE_HostCreate();

   bridge_port[0].flow_agg = host_flow_agg_dummy;
   bport_handle[0] = WP_IwPortCreate(iw_system, bridge_port);
   WPE_TerminateOnError(bport_handle[0], "WP_IwPortCreate()", __LINE__);

   bport_handle[1] = WP_IwPortCreate(iw_system, bridge_port);
   WPE_TerminateOnError(bport_handle[1], "WP_IwPortCreate()", __LINE__);

   pecs_handle[BROUTER_PECS] = WP_GpePecsCreate(WP_WINPATH(0), WP_GPE_PECS, (void*) &gpe_pecs_cfg[0]);
   WPE_TerminateOnError(pecs_handle[BROUTER_PECS], "WP_GpePecsCreate()", __LINE__);

//   gpe_pecs_cfg[0].pecs_image_info = &is_brouter_pecs_pecs_image;  /*intsw pecs_image_info*/
//   gpe_pecs_cfg[0].pecs_service_fp = &WPE_GpeIsBrouterPecsService; /*intsw pecs_service_fp*/

//   pecs_handle[IS_BROUTER_PECS] = WP_GpePecsCreate(WP_WINPATH(0), WP_GPE_PECS, (void*) &gpe_pecs_cfg[0]);
//   WPE_TerminateOnError(pecs_handle[IS_BROUTER_PECS], "WP_GpePecsCreate()", __LINE__);

} /* WPE_IWSystemCreate */
static void WPE_HostCreate(void)
{
   static WP_tx_binding tx_binding_iw[1] =
   {
         {
               /* res0;*/0,
               /* dci_mode;*/WP_IW_DYN_CH_INSERT_ENABLE,
               /* maxt;*/250,
         }
   };
   static WP_ch_iw_rx ch_config_iw[1] =
   {
         {
               /* pqblock */0,
               /* pqlevel */0,
               /* tx_binding_type */WP_IW_TX_BINDING,
               /* tx_binding_config */tx_binding_iw
         }
   };

   WP_iw_agg_hq flooding_tx[1] =
   {
         {
               /* tag */0,
               /* txfunc */0,
               /* hq_agg_mode */WP_IW_HQ_NOT_RESERVED,
               /* mtu */9600,
               /* interruptqueue */WP_IW_IRQT1,
               /* intmode */WP_IW_INT_DISABLE,
               /* statmode */WP_IW_STAT_ENABLE,
               /* timestamp_mode */WP_IW_TIME_STAMP_DISABLE,
               /* ov_pool_mode */WP_IW_OV_POOL_DISABLE,
               /* fbp_drop_threshold */0,
               /* tx_direction */WP_IW_NETWORK_UNKNOWN,
               /* flow_analyze */WP_DISABLE,
               /* force_analyze */WP_IW_FORCE_NO,
               /* l4_checksum_recal */WP_IW_L4_NO_CHECKSUM,
               /* nexthop */0,
               /* flow_agg_type */WP_IW_AGG_PRIMARY,
               /* monitor_queue_size */0,
               /* iw_port */0,
               /* host_term_control */WP_IW_HQ_CONTROL_NONE,
               /* policer_mode */WP_IW_POLICER_DISABLE,
               /* policer_handle */0,
         }

   };

   WP_bridge_port iwhost_bport_config[1] =
   {
         {
               /* tag */0x2000,
               /* direct_mapping */WP_IW_DIRECT_MAP_DISABLE,
               /* flow_agg */0,
               /* flooding_term_mode */WP_IW_HOST_TERM_MODE,
               /* learning_mode */WP_IW_LEARNING_DISABLE,
               /* in_filter_mode */WP_IW_INGRESS_FILTER_DISABLE,
               /* vlan_param */
               {
                     /* vlan_acceptance_mode */WP_IW_ACCEPT_TAGGED_UNTAGGED,
                     /* vlan_tag */0x8,
                     /* vlan_tunnel*/0,
                     /* vlan_priority_enforce_mode*/0, /* stag_vlan_etype*/DEFAULT_STAG_TYPE

               },
               /*max_mac_addresses */100,
               /*group_tag */0,
               /*group_filtering_mode */WP_IW_GROUP_FILTER_DISABLE,
               /*unk_mac_sa_filter*/0,
               /*unk_mc_mode;*/0,
               /*bc_ht_mode;*/0,
               /*input_filters_mask*/0,
               /*output_filters_mask;*/0,
               /*statmode*/WP_IW_PORT_STAT_ENABLE
         }
   };
   /* The following structure is used to configure the Host send (tx) channel.
    For information on the parameters see the Ingress chapter */

   /*create dummy host rx channel will dump all packets */
   rx_host_handle_dummy = WP_ChannelCreate(0,
                                           host_device,
                                           qnodes[QNODE_RX_IW],
                                           WP_CH_RX,
                                           WP_IW_HOST,
                                           &ch_config_iw[0]);
   WPE_TerminateOnError(rx_host_handle_dummy, "WP_ChannelCreate() Host sys1", __LINE__);

   tx_binding_iw[0].maxt = 1000;
   rx_host_handle = WP_ChannelCreate(0,
                                     host_device,
                                     qnodes[QNODE_RX_IW],
                                     WP_CH_RX,
                                     WP_IW_HOST,
                                     &ch_config_iw[0]);
   WPE_TerminateOnError(rx_host_handle, "WP_ChannelCreate() Host sys1", __LINE__);

   /*creates dummy flow Aggregation for the host */
   flooding_tx[0].txfunc = rx_host_handle_dummy;
   host_flow_agg_dummy = WP_IwFlowAggregationCreate(WP_SYSHANDLE(WP_WINPATH(0)),
                                                    WP_IW_HOST_QUEUE_MODE/*WP_IW_VLAN_AWARE_BRIDGING_MODE*/,
                                                    &flooding_tx);
   WPE_TerminateOnError(host_flow_agg_dummy, " WP_IwFlowAggregationCreate()", __LINE__);

   /*creates flow Aggregation for the host */
   flooding_tx[0].txfunc = rx_host_handle;
   host_flow_agg = WP_IwFlowAggregationCreate(WP_SYSHANDLE(WP_WINPATH(0)),
                                              WP_IW_HOST_QUEUE_MODE/*WP_IW_VLAN_AWARE_BRIDGING_MODE*/,
                                              &flooding_tx);
   WPE_TerminateOnError(host_flow_agg, " WP_IwFlowAggregationCreate()", __LINE__);

   /* Create host iw bport for iw tx channel. */
   iwhost_bport_config[0].flow_agg = host_flow_agg_dummy;
   host_bport = WP_IwPortCreate(iw_system, &iwhost_bport_config[0]);
   WPE_TerminateOnError(host_bport, " WP_IwPortCreate()", __LINE__);
} /* WPE_HostCreate */
static void WPE_ChannelsCreate()
{
   WP_U8 i;
   WP_ch_enet ch_config_enet[1] =
   {
         {
               /* intmode */WP_PKTCH_INT_DISABLE,
               /* iwmode */WP_PKTCH_IWM_ENABLE,
               /* testmode */WP_PKTCH_TEST_DISABLE,
               /* tx_pqblock */0,
               /* tx_pqlevel */0,
               /* tx_shaping_type */WP_FMU_SHAPING_TYPE_STRICT,
               /* tx_shaping_params */NULL,
               /* rx_maxsdu *//*1536*/9728,
               /* tx_cwid *//*WP_UNUSED*/WP_CW_ID_A,
               /* tx_tq */0,
               /* rx_queuedepth *//*1024*/20,
         }
   };
   WP_ch_enet_internal_switching ch_config_intsw[1] =
   {
         {
               /* maxsdu */9728
         }
   };
   WP_tx_binding tx_binding[1] =
   {
         {
               /* res0 */0,
               /* dci_mode;*/WP_IW_DYN_CH_INSERT_ENABLE,
               /* maxt;*/255,
         }
   };
   WP_status status;
   WP_shaping_group_type_packet l1_cfg[1];
   WP_fmu_shaping_cir_eir shaping_params[1];
   memset (shaping_params, 0, sizeof (WP_fmu_shaping_cir_eir));
   shaping_params[0].cir = (WP_U64)10000000000;
   shaping_params[0].eir = (WP_U64)10000000000;
   shaping_params[0].cbs = 800000;
   shaping_params[0].ebs = 800000;
   shaping_params[0].flags = 0;

   memset (l1_cfg, 0, sizeof (l1_cfg));
   l1_cfg[0].group_level       = WP_L1_GROUP;
   l1_cfg[0].tx_shaping_type   = WP_FMU_SHAPING_TYPE_CIR_EIR;
   l1_cfg[0].tx_shaping_params = &shaping_params[0];
   l1_cfg[0].num_fifos         = WP_NUM_FIFOS_8;
   l1_cfg[0].block_level       = 0;
   l1_cfg[0].group_mode        = WP_MODE_HW;

   /* Channels */
   xgi1_rx_channel = WP_ChannelCreate(0,
                                      xgi1_device_handle,
                                      qnodes[QNODE_RX_IW],
                                      WP_CH_RX,
                                      WP_ENET,
                                      &ch_config_enet);
   WPE_TerminateOnError(xgi1_rx_channel, "WP_ChannelCreate rx", __LINE__);

   xgi2_rx_channel= WP_ChannelCreate(0,
                                     xgi2_device_handle,
                                     qnodes[QNODE_RX_IW],
                                     WP_CH_RX,
                                     WP_ENET,
                                     &ch_config_enet);
   WPE_TerminateOnError(xgi2_rx_channel, "WP_ChannelCreate rx", __LINE__);

   for (i = 0; i < MAX_CLASSES; i++)
   {
      /* external channel create */
      l1_cfg[0].group_level = WP_L1_GROUP;
      l1_shaping_group_xgi1[i] = WP_ShapingGroupCreate(xgi1_device_handle,
                                                  WP_SHAPING_GROUP_TYPE_PACKET,
                                                  l1_cfg);
      WPE_TerminateOnError(l1_shaping_group_xgi1[i], "l1 WP_ShapingGroupCreate", __LINE__);

      l1_cfg[0].group_level = WP_L2_GROUP;
      l2_shaping_group_xgi1[i] = WP_ShapingGroupCreate(l1_shaping_group_xgi1[i],
                                                  WP_SHAPING_GROUP_TYPE_PACKET,
                                                  l1_cfg);
      WPE_TerminateOnError(l2_shaping_group_xgi1[i], "l2 WP_ShapingGroupCreate", __LINE__);

      status = WP_ShapingGroupEnable(l2_shaping_group_xgi1[i]);
      WPE_TerminateOnError(status, "WP_ShapingGroupEnable", __LINE__);
      status = WP_ShapingGroupEnable(l1_shaping_group_xgi1[i]);
      WPE_TerminateOnError(status, "WP_ShapingGroupEnable", __LINE__);

      /* External Switching tx Channels */
      extsw_tx_channel_xgi1[i] = WP_ChannelCreate(i,
                                             l2_shaping_group_xgi1[i],
                                             qnodes[QNODE_RX_IW],
                                             WP_CH_TX,
                                             WP_ENET,
                                             &ch_config_enet);
      WPE_TerminateOnError(extsw_tx_channel_xgi1[i], "WP_ChannelCreate tx", __LINE__);

      /*Create Tx Binding*/
      status = WP_IwTxBindingCreate(extsw_tx_channel_xgi1[i], WP_IW_TX_BINDING, tx_binding);
      WPE_TerminateOnError(status, "WP_IwTxBindingCreate", __LINE__);
/////////////////////
      /* external channel create */
      l1_cfg[0].group_level = WP_L1_GROUP;
      l1_shaping_group_xgi2[i] = WP_ShapingGroupCreate(xgi2_device_handle,
                                                  WP_SHAPING_GROUP_TYPE_PACKET,
                                                  l1_cfg);
      WPE_TerminateOnError(l1_shaping_group_xgi2[i], "l1 WP_ShapingGroupCreate", __LINE__);

      l1_cfg[0].group_level = WP_L2_GROUP;
      l2_shaping_group_xgi2[i] = WP_ShapingGroupCreate(l1_shaping_group_xgi2[i],
                                                  WP_SHAPING_GROUP_TYPE_PACKET,
                                                  l1_cfg);
      WPE_TerminateOnError(l2_shaping_group_xgi2[i], "l2 WP_ShapingGroupCreate", __LINE__);

      status = WP_ShapingGroupEnable(l2_shaping_group_xgi2[i]);
      WPE_TerminateOnError(status, "WP_ShapingGroupEnable", __LINE__);
      status = WP_ShapingGroupEnable(l1_shaping_group_xgi2[i]);
      WPE_TerminateOnError(status, "WP_ShapingGroupEnable", __LINE__);

      /* External Switching tx Channels */
      extsw_tx_channel_xgi2[i] = WP_ChannelCreate(i,
                                             l2_shaping_group_xgi2[i],
                                             qnodes[QNODE_RX_IW],
                                             WP_CH_TX,
                                             WP_ENET,
                                             &ch_config_enet);
      WPE_TerminateOnError(extsw_tx_channel_xgi2[i], "WP_ChannelCreate tx", __LINE__);

      /*Create Tx Binding*/
      status = WP_IwTxBindingCreate(extsw_tx_channel_xgi2[i], WP_IW_TX_BINDING, tx_binding);
      WPE_TerminateOnError(status, "WP_IwTxBindingCreate", __LINE__);
   }
} /* WPE_ChannelsCreate */
static void WPE_CreateMultiClassDevices()
{
   WP_U8 class_idx;
   WP_device_enet_multi_class extsw;
   WP_device_enet_multi_class_intsw intsw;

   WP_enet_multi_class_shaping_cir_eir shaping_params_ptr[1];

   memset(&intsw, 0, sizeof(WP_device_enet_multi_class_intsw));
   intsw.tx_shaping_type =
         WP_ENET_MULTI_CLASS_SHAPING_STRICT;
   intsw.tx_shaping_params = NULL;

   for (class_idx = 0; class_idx < MAX_CLASSES; class_idx++)
   {
      intsw.class_level = class_idx;
      intsw_class_handle[class_idx] = WP_DeviceCreate(xgi1_device_handle,
                                                      class_idx,
                                                      WP_DEVICE_ENET_MULTI_CLASS_INTSW,
                                                      &intsw);
      WPE_TerminateOnError(intsw_class_handle[class_idx], "WP_DeviceCreate", __LINE__);
   }

} /* WPE_CreateMultiClassDevices */
static void WPE_CreateIwPorts()
{
   WP_U8 port_idx, iw_port_idx;
   memset(bridge_port, 0, sizeof(WP_bridge_port));

   bridge_port[0].tag = 0x0;
   bridge_port[0].direct_mapping = WP_IW_DIRECT_MAP_ENABLE;
   bridge_port[0].flooding_term_mode = WP_IW_HOST_TERM_MODE;
   bridge_port[0].learning_mode = WP_IW_LEARNING_DISABLE;
   bridge_port[0].in_filter_mode = WP_IW_INGRESS_FILTER_DISABLE;
   bridge_port[0].vlan_param.vlan_acceptance_mode = WP_IW_ACCEPT_TAGGED_UNTAGGED;
   bridge_port[0].vlan_param.vlan_tag = 0xA;
   bridge_port[0].max_mac_addresses = 100;
   bridge_port[0].group_tag = 0;
   bridge_port[0].group_filtering_mode = WP_IW_GROUP_FILTER_DISABLE;
   bridge_port[0].unk_mac_sa_filter = WP_IW_UNK_MACSA_FILTER_DISABLE;
   bridge_port[0].unk_mc_mode = WP_IW_UNK_MC_DROP;
   bridge_port[0].bc_ht_mode = WP_IW_BC_HT_DISABLE;
   bridge_port[0].input_filters_mask = 0;
   bridge_port[0].output_filters_mask = 0;
   bridge_port[0].statmode = WP_IW_PORT_STAT_ENABLE;
   bridge_port[0].unk_uc_mode = WP_IW_UNK_UC_SR_ENABLE;
   bridge_port[0].classification_flag = WP_IW_BPORT_CLASSIFICATION_ENABLED;
   bridge_port[0].adv_unk_lookup_mode = WP_IW_ADV_UNK_LOOKUP_ENABLED;
   bridge_port[0].cfi_ht_mode = WP_IW_CFI_HT_DISABLE;
   bridge_port[0].reserved_mc_ht_mode = WP_IW_RES_MC_HT_DISABLE;
   bridge_port[0].predefined_ht_mode = WP_DISABLE;

   for (port_idx = 0, iw_port_idx = 0; port_idx < MAX_CLASSES; port_idx++, iw_port_idx++)
   {
      bridge_port[0].flow_agg = host_flow_agg_dummy;
      bridge_port[0].tag = iw_port_idx;
      iwport_handle[port_idx] = WP_IwPortCreate(iw_system, bridge_port);
      WPE_TerminateOnError(iwport_handle[port_idx], "WP_IwPortCreate", __LINE__);

      printf("iwport_handle[%d] = 0x%x\n", port_idx, iwport_handle[port_idx]);
   }
} /* WPE_CreateIwPorts */
static void WPE_InitializeContext(WP_context *context, WP_iw_global *max_iw, WP_int_queue_table int_queue_tables)
{
   WP_U8 ii;

   memset(max_iw, 0, sizeof(WP_iw_global));
   memset(&int_queue_tables, 0, sizeof(WP_int_queue_table));
   memset(context, 0, sizeof(WP_context));

   for(ii = 0 ; ii < WP_IRQ_TABLES ; ii++)
   {
      int_queue_tables.queue[ii].size = 10;
      int_queue_tables.queue[ii].rate =  1;
   }

   max_iw->max_iw_aggregations = 90;
   max_iw->max_routing_systems = 0;
   max_iw->max_directmap_enet_systems = 0;
   max_iw->max_bridging_systems = 5;
   max_iw->max_mpls_systems = 0;
   max_iw->max_vlan_priority_maps = 5;
   max_iw->iw_bkgnd_fifo_size = 0;
   max_iw->cong_pt = NULL;
   max_iw->iw_host_limits.max_rx_channels = 64;
   max_iw->iw_host_limits.pq_block_size = 0;
   max_iw->iw_host_limits.n_pq_blocks = 0;
   max_iw->iw_host_limits.max_tx_channels = 100;
   max_iw->iw_host_limits.max_tx_groups = 0;
   max_iw->mpls_config.n_llst_tables = 0;
   max_iw->mpls_config.llst_table_size = 0;
   max_iw->iw_modes.policer_mode = WP_IW_POLICER_ENABLE;
   max_iw->iw_modes.statistics_bundle_stat_mode = WP_IW_STATISTICS_BUNDLE_STAT_DISABLE;
   max_iw->iw_modes.l2_prefix_ext_mode = WP_IW_L2_HEADER_EXTENSION_ENABLE;
   max_iw->iw_modes.enhanced_flow_stat_mode = WP_IW_ENHANCED_FLOW_STAT_DISABLE;
   max_iw->iw_modes.flow_stat_mode = WP_IW_FLOW_STAT_ENABLE;
   max_iw->iw_modes.fr_tx_flow_stat_mode = WP_IW_FR_TX_FLOW_STAT_DISABLE;
   max_iw->iw_modes.mfc_alloc_mode = WP_IW_MFC_ALLOC_DISABLE;
   max_iw->iw_modes.learning_queue_mode = WP_IW_LEARNING_DISABLE;
   max_iw->iw_modes.port_filtering_mode = WP_IW_PORT_FILTERING_DISABLE;
   max_iw->iw_modes.lpm_alloc_mode = 0;
   max_iw->iw_modes.enhanced_mc_member_stat_mode = 0;
   max_iw->iw_modes.routing_alloc_modes = 0;
   max_iw->iw_modes.res_mc_addresses_table_mode = 0;
   max_iw->iw_modes.port_stag_etype_mode = WP_IW_PORT_STAG_ETYPE_ENABLE;
   max_iw->iw_modes.iw_characteristics = 0;
   max_iw->max_iw_stat_bundles = 0;
   max_iw->max_fr_sw_systems = 0;
   max_iw->max_iw_ports = 70;
   max_iw->max_iw_mc_groups = 5;
   max_iw->max_nat_systems = 0;
   max_iw->max_iw_second_aggregations = 90;
   max_iw->iw_comp_limits = NULL;
   max_iw->max_iw_mc_members = 5;
   max_iw->max_iw_programmable_filters = 0;
   max_iw->max_iw_policers = 2;
   max_iw->max_dscpmt = 0;
   max_iw->max_wred_bindings = 0;
   max_iw->tcm_policer_limits.max_vlan_priority_tcm_policers = 0;
   max_iw->tcm_policer_limits.max_dscp_tcm_policers= 0;
   max_iw->max_flow_rule_stat_tables = 0;
   max_iw->max_indirect_action_tables = 0;
   max_iw->brigding_time_stamp_info.time_stamp_offset = 0;
   max_iw->brigding_time_stamp_info.time_stamp_resolution = 0;
   max_iw->iwgp_bkgnd_fifo_size = 0;


   context->max_qnodes = POOL_LIMIT;
   context->virtual_bus[WP_VB_PARAM].bus = WP_BUS_PARAM;
   context->virtual_bus[WP_VB_PARAM].bank =  APP_BANK_PARAM;
   context->virtual_bus[WP_VB_PACKET].bus = WP_BUS_PACKET;
   context->virtual_bus[WP_VB_PACKET].bank =  APP_BANK_PACKET;
   context->virtual_bus[WP_VB_INTERNAL].bus = WP_BUS_INTERNAL;
   context->virtual_bus[WP_VB_INTERNAL].bank =  APP_BANK_INTERNAL;
   context->virtual_bus[WP_VB_HOST].bus = WP_BUS_HOST;
   context->virtual_bus[WP_VB_HOST].bank =  APP_BANK_HOST;
   context->virtual_bus[WP_VB_INTQUEUE_DEBUG].bus = WP_BUS_PARAM;
   context->virtual_bus[WP_VB_INTQUEUE_DEBUG].bank =  APP_BANK_PARAM;
   context->virtual_bus[WP_VB_TIMESTAMP].bus = WP_BUS_INTERNAL;
   context->virtual_bus[WP_VB_TIMESTAMP].bank =  APP_BANK_INTERNAL;
   context->virtual_bus[WP_VB_CAS].bus = WP_BUS_NONE;
   context->virtual_bus[WP_VB_CAS].bank =  0;
   context->virtual_bus[WP_VB_TRANS_TSAT].bus = WP_BUS_NONE;
   context->virtual_bus[WP_VB_TRANS_TSAT].bank =  0;
   context->virtual_bus[WP_VB_TPT].bus = WP_BUS_INTERNAL;
   context->virtual_bus[WP_VB_TPT].bank =  APP_BANK_INTERNAL;
   context->virtual_bus[WP_VB_CW].bus = WP_BUS_INTERNAL;
   context->virtual_bus[WP_VB_CW].bank =  APP_BANK_INTERNAL;
   context->virtual_bus[WP_VB_ASU_TXQUEUE].bus = WP_BUS_INTERNAL;
   context->virtual_bus[WP_VB_ASU_TXQUEUE].bank =  APP_BANK_INTERNAL;
   context->virtual_bus[WP_VB_STATISTICS].bus = WP_BUS_INTERNAL;
   context->virtual_bus[WP_VB_STATISTICS].bank =  APP_BANK_INTERNAL;
   context->virtual_bus[WP_VB_ATM_ADDLOOKUP].bus = WP_BUS_PARAM;
   context->virtual_bus[WP_VB_ATM_ADDLOOKUP].bank =  APP_BANK_PARAM;
   context->virtual_bus[WP_VB_ATM_CPT_EBMT_EXTHEADER].bus = WP_BUS_PARAM;
   context->virtual_bus[WP_VB_ATM_CPT_EBMT_EXTHEADER].bank =  APP_BANK_PARAM;
   context->virtual_bus[WP_VB_ETH_HDLC_CPT].bus = WP_BUS_PARAM;
   context->virtual_bus[WP_VB_ETH_HDLC_CPT].bank =  APP_BANK_PARAM;
   context->virtual_bus[WP_VB_FBP_BD].bus = WP_BUS_PARAM;
   context->virtual_bus[WP_VB_FBP_BD].bank =  APP_BANK_PARAM;
   context->virtual_bus[WP_VB_FBP_DATA].bus = WP_BUS_PARAM;
   context->virtual_bus[WP_VB_FBP_DATA].bank =  APP_BANK_PARAM;
   context->virtual_bus[WP_VB_L2_BKGND].bus = WP_BUS_PACKET;
   context->virtual_bus[WP_VB_L2_BKGND].bank =  APP_BANK_PACKET;
   context->virtual_bus[WP_VB_BRIDGE_LEARNING_DFC_PFMT].bus = WP_BUS_PACKET;
   context->virtual_bus[WP_VB_BRIDGE_LEARNING_DFC_PFMT].bank =  APP_BANK_PACKET;
   context->virtual_bus[WP_VB_IMA_MLPPP_STATE].bus = WP_BUS_INTERNAL;
   context->virtual_bus[WP_VB_IMA_MLPPP_STATE].bank =  APP_BANK_INTERNAL;
   context->virtual_bus[WP_VB_IMA_MLPPP_BUFFER].bus = WP_BUS_INTERNAL;
   context->virtual_bus[WP_VB_IMA_MLPPP_BUFFER].bank =  APP_BANK_INTERNAL;
   context->virtual_bus[WP_VB_HASH_IW_LPM_EMC].bus = WP_BUS_PACKET;
   context->virtual_bus[WP_VB_HASH_IW_LPM_EMC].bank =  APP_BANK_PACKET;
   context->virtual_bus[WP_VB_IW_MFC_RULES].bus = WP_BUS_PACKET;
   context->virtual_bus[WP_VB_IW_MFC_RULES].bank =  APP_BANK_PACKET;
   context->virtual_bus[WP_VB_IW_FIWT_QAT].bus = WP_BUS_PARAM;
   context->virtual_bus[WP_VB_IW_FIWT_QAT].bank =  APP_BANK_PARAM;
   context->virtual_bus[WP_VB_IW_MFCPT_DIFFSERV_UPPT_DFCT].bus = WP_BUS_INTERNAL;
   context->virtual_bus[WP_VB_IW_MFCPT_DIFFSERV_UPPT_DFCT].bank =  APP_BANK_INTERNAL;
   context->virtual_bus[WP_VB_STATISTICS2].bus = WP_BUS_INTERNAL;
   context->virtual_bus[WP_VB_STATISTICS2].bank =  APP_BANK_INTERNAL;
   context->virtual_bus[WP_VB_STATISTICS3].bus = WP_BUS_PARAM;
   context->virtual_bus[WP_VB_STATISTICS3].bank =  APP_BANK_PARAM;
   context->virtual_bus[WP_VB_IMA_RX_BUS].bus = WP_BUS_NONE;
   context->virtual_bus[WP_VB_IMA_RX_BUS].bank =  0;
   context->virtual_bus[WP_VB_PKT_GLOBAL_TCPT].bus = WP_BUS_NONE;
   context->virtual_bus[WP_VB_PKT_GLOBAL_TCPT].bank =  0;
   context->virtual_bus[WP_VB_AAL2_QSR].bus = WP_BUS_NONE;
   context->virtual_bus[WP_VB_AAL2_QSR].bank =  0;
   context->virtual_bus[WP_VB_UNASSIGNED9].bus = WP_BUS_NONE;
   context->virtual_bus[WP_VB_UNASSIGNED9].bank =  0;
   context->virtual_bus[WP_VB_UNASSIGNED10].bus = WP_BUS_NONE;
   context->virtual_bus[WP_VB_UNASSIGNED10].bank =  0;
   context->virtual_bus[WP_VB_WDDI_FIRST].bus = WP_BUS_NONE;
   context->virtual_bus[WP_VB_WDDI_FIRST].bank = 0;
   context->virtual_bus[WP_VB_OAM_FM_QUERY].bus = WP_BUS_NONE;
   context->virtual_bus[WP_VB_OAM_FM_QUERY].bank = 0;
   context->virtual_bus[WP_VB_HASH_ON_HEAP].bus = WP_BUS_NONE;
   context->virtual_bus[WP_VB_HASH_ON_HEAP].bank = 0;
   context->virtual_bus[WP_VB_MFC_INTERNAL].bus = WP_BUS_PARAM;
   context->virtual_bus[WP_VB_MFC_INTERNAL].bank =  APP_BANK_PARAM;
   context->virtual_bus[WP_VB_MFR_RAS_TBL].bus = WP_BUS_NONE;
   context->virtual_bus[WP_VB_MFR_RAS_TBL].bank =  0;
   context->virtual_bus[WP_VB_CESOP_DATA].bus = WP_BUS_NONE;
   context->virtual_bus[WP_VB_CESOP_DATA].bank =  0;
   context->virtual_bus[WP_VB_SECURITY_ESE].bus = WP_BUS_NONE;
   context->virtual_bus[WP_VB_SECURITY_ESE].bank =  0;
   context->virtual_bus[WP_VB_PDCP_SECURITY_ENTITY].bus = WP_BUS_NONE;
   context->virtual_bus[WP_VB_PDCP_SECURITY_ENTITY].bank =  0;
   context->virtual_bus[WP_VB_POLICER].bus = WP_BUS_INTERNAL;
   context->virtual_bus[WP_VB_POLICER].bank =  APP_BANK_INTERNAL;
   context->virtual_bus[WP_VB_PCE_LRN].bus = WP_BUS_INTERNAL;
   context->virtual_bus[WP_VB_PCE_LRN].bank =  APP_BANK_INTERNAL;
   context->virtual_bus[WP_VB_PCEA_PARAM].bus = WP_BUS_INTERNAL;
   context->virtual_bus[WP_VB_PCEA_PARAM].bank =  APP_BANK_INTERNAL;
   context->virtual_bus[WP_VB_CONGESTION_TABLE].bus = WP_BUS_NONE;
   context->virtual_bus[WP_VB_CONGESTION_TABLE].bank =  0;
   context->virtual_bus[WP_VB_PARAMS_PROPAGATION].bus = WP_BUS_NONE;
   context->virtual_bus[WP_VB_PARAMS_PROPAGATION].bank =  0;
   context->virtual_bus[WP_VB_STATS_IWPORT_PCE_FILTERS].bus = WP_BUS_INTERNAL;
   context->virtual_bus[WP_VB_STATS_IWPORT_PCE_FILTERS].bank =  APP_BANK_INTERNAL;
   context->virtual_bus[WP_VB_TIMESTAMP_SERVICES_TABLE].bus = WP_BUS_NONE;
   context->virtual_bus[WP_VB_TIMESTAMP_SERVICES_TABLE].bank =  0;
   context->virtual_bus[WP_VB_POLICER_ACTION].bus = WP_BUS_PARAM;
   context->virtual_bus[WP_VB_POLICER_ACTION].bank =  APP_BANK_PARAM;
   context->virtual_bus[WP_VB_PECS_GLOBAL].bus = WP_BUS_PARAM;
   context->virtual_bus[WP_VB_PECS_GLOBAL].bank =  APP_BANK_PARAM;
   context->virtual_bus[WP_VB_ENET_10G_LONG_PKT].bus = WP_BUS_PARAM;
   context->virtual_bus[WP_VB_ENET_10G_LONG_PKT].bank =  APP_BANK_PARAM;
   context->dps_image[0] = context->dps_image[1] = dps_ProgramImage;
   context->dps_features[0] = context->dps_features[1] = dps_PC_Table_Init;
   context->int_queue_data = &int_queue_tables;
   context->pool_limit = POOL_LIMIT;
   context->atm_data = NULL;
   context->iw_data = max_iw;
   context->dps_winfarm_mask = 0;

   int_ram_partition.policer_entries = 4096;
   int_ram_partition.pce_hw_rules_num = WT_PARTITION_UNCHANGED;
   int_ram_partition.cfu_rx_buffers_num = WT_PARTITION_UNCHANGED;
   int_ram_partition.cfu_tx_buffers_num = 4192;
   int_ram_partition.mips_cache_size = WT_PARTITION_UNCHANGED;
   WT_SetIntRamPartition(WP_WINPATH(DEFAULT_WPID), &int_ram_partition);
}
/*****************************************************************************
 * Function name: WPE_SystemSetup
 * Description  :
 * Input  params:
 * Output params:
 * Return val   :
 *****************************************************************************/
static void WPE_SystemSetup(void)
{
   WP_status status;
   WP_U8 ii;

   WP_module_gpe_init gpe_module_init =
   {
         /* max_num_of_pecs */2,
         /* max_num_of_pecs_global_info */10
   } ;
   WP_policer_limits_v2 v2_policer_limits[1] =
   {
         {
#ifdef WP_BOARD_WDS3_SL
               /* max_internal_policers*/0,
#else
               /* max_internal_policers*/5,
#endif
               /* max_external_policers*/5,
               /* max_policer_actions*/5
         }
   };
   WP_module_policer policer_module_init =
   {
         /*v2_policer_limits*/v2_policer_limits,
         /*v1_policer_limits*/0,
         /*atm_policer_limits*/0,
         /*mode*/WP_POLICER_STATS_ENABLE
   };
   WP_qnode_iwq qn_iw[1] =
   {
   {
   /* interruptqueue */0,
   /* num_buffers */3500,
   /* adjunct_pool */0 } };
   /* Enet port configuration */

   WP_port_enet port_cfg[1] =
   {
         {
               /* pkt_limits */
               { 100, 1, 0, 0, 0 },
               /* flowmode */WP_ENET_FMU_HIERARCHICAL_SHAPING_MODE,
               /* interface_mode */WP_ENET_XAUI,
               /* rx_iw_bkgnd */WP_IW_BKGND_NOT_USED
         }
   };
   WP_device_enet device_cfg[1] =
   {
         {
               /* max_tx_channels */90,
               /* tx_maxsdu */(9*1024),
               /* operating speed */WP_UNUSED,
               /* mac_addr */
               { 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa },
               /* tx_bit_rate */WP_TX_BIT_RATE_UNLIMITED,
               /* loopbackmode */WP_ENET_NORMAL,
               /* extended_params */NULL
         }
   };
   WP_enet_filters rx_filters[1] =
   {
   {
   /*rx_ucfilter*/WP_ENET_FILTER_PROCESS,
   /*rx_bcfilter*/WP_ENET_FILTER_PROCESS,
   /*rx_mcfilter*/WP_ENET_FILTER_PROCESS } };
   WP_device_enet_ex enet_device_extended_cfg[1] =
   {
         {
               /*duplexmode*/WP_ENET_FULLDUPLEX,
               /*rx_maclookup_mode*/WP_DISABLE,
               /*rx_flowcontrol*/WP_ENET_FLOW_DISABLE,
               /*tx_flowcontrol*/WP_ENET_FLOW_DISABLE,
               /*rx_addrmode*/WP_ENET_ADDR_ACCEPTALL,
               /*phystatmode*/WP_ENET_STAT_ENABLE,
               /*tx_statmode*/WP_ENET_STAT_ENABLE,
               /*rx_statmode*/WP_ENET_STAT_ENABLE,
               /*tx_duplicate*/WP_DISABLE,
               /*rx_filters*/rx_filters,
               /*rx_timestamp*/WP_DISABLE,
               /*tx_timestamp_type*/0,
               /*max_tx_slow_channels*/0,
               /*tx_timestamp*/WP_DISABLE,
               /*tx_mode_10G*/WP_ENET_TX_MODE_DEFAULT,
               /*rx_timestamp_type*/0,
               /*rx_internal_switching_traffic_mode*/WP_ENABLE,
               /* *multi_class_config*/NULL
         }
   };
   //PCE initialize
   WP_module_pce_init pce_init_cfg =
   {
         &pce_parser_info, /* parser_info */
         4000, /* hardware_rules_num */
         255, /* filters_num */
         300, /* pkt_rx_channel_interfaces_num */
         255, /* iw_tx_channel_interfaces_num */
         /*WP_PCE_DISCARD_MODE_SMART_DISCARD*/WP_PCE_DISCARD_MODE_ALL, /* pce_discard_mode */
         WP_ENABLE, /* bridge_loop_prevention_mode */
         WP_DISABLE, /* learning_mode */
         {/* learning_queue_config */
               /*30*/LEARNING_QUEUE_SIZE, /* learning_queue_size */
               WP_IW_IRQT0, /* int_queue_num */
               30, /* interrupt_rate */
         },
         50 /* iw_next_round_pce_interfaces_num*/
   };
   /*** Pools and QNodes configurations ***/
   WP_pool_buffer_data buffer_data_vb[1] =
   {
   {
   /* n_buffers */3500,
   /* offset */64,
   /* size */1984,
   /* pad */0,
   /* bus *//*WP_BUS_PARAM*/WP_BUS_PACKET,
   /* bank *//*APP_BANK_PARAM*/APP_BANK_PACKET } };
   WP_int_queue_table int_queue_tables =
   {
   {
   { 24, 1 },
   { 10, 1 },
   { 10, 1 },
   { 10, 1 } } };
   WP_atm_global atm_global_cfg[1] =
   {
   {
   /* max_pm_tables */0,
   /* max_ubrplus_channels */0,
   /* max_upc_tables */0,
   /* max_rx_cids */0,
   /* max_tx_cids */10,
   /* max_cid_range */10,
   /* AAL2 QSR bus */WP_BUS_NONE,
   /* AAL2 QSR bank */APP_BANK_HOST,
   { 0, 0 }, 0, 0 } };
   WP_iw_global max_iw[1] =
   {
   {
   /* max_iw_aggregations */5000,
   /* max_r_iw_systems;*/100,
   /* max_s_iw_systems;*/0,
   /* max_b_iw_systems;*/400,
   /* max_m_iw_systems;*/0,
   /* max_vlan_priority_maps */1,
   /* iw_bkgnd_fifo_size */3000,
   /* cong_pt       */NULL,
   /* iw_host_limits */
   { 64, 0, 0, 1, 0 },
   /* mpls_config*/
   { 0, 0 },
   /* iw_modes */
   {
         /* policer_mode */WP_IW_POLICER_ENABLE,
         /* statistics_bundle_stat_mode */WP_IW_STATISTICS_BUNDLE_STAT_DISABLE,
         /* l2_prefix_ext_mode */WP_IW_L2_HEADER_EXTENSION_ENABLE,
         /* enhanced_flow_stat_mode */WP_IW_ENHANCED_FLOW_STAT_DISABLE,
         /* flow_stat_mode */WP_IW_FLOW_STAT_ENABLE,
         /* fr_tx_flow_stat_mode */WP_IW_FR_TX_FLOW_STAT_DISABLE,
         /* mfc_alloc_mode */WP_IW_MFC_ALLOC_ENABLE,
         /* learning_queue_mode */WP_IW_LEARNING_DISABLE,
         /* port_filtering_mode */WP_IW_PORT_FILTERING_ENABLE,
         /* lpm_alloc_mode */WP_IW_LPM_ALLOC_DISABLE,
         /* enhanced_mc_member_stat_mode */WP_DISABLE,
         /* routing_alloc_modes */WP_DISABLE,
         /* res_mc_addresses_table_mode */WP_IW_BPORT_RES_MC_TABLE_ALLOC_DISABLE,
         /* port_stag_etype_mode */WP_IW_PORT_STAG_ETYPE_ENABLE,
         /* iw_characteristics */WP_IW_MODES_IWGP_BACKGROUND_ENABLE
         | WP_IW_MODES_DFC_INDIRECT_ACTION_DISABLE | WP_IW_MODES_FLOW_CLASS_RULE_STAT_ENABLE,
   },
   /* max_iw_stat_bundles*/0,
   /* max_fr_s_iw_systems*/0,
   /* max_bridging_ports */400,
   /* max_iw_mc_groups */100,
   /* max_nat_systems */0,
   /* max_iw_second_aggregations */0,
   /* max_iw_comp_limits */0,
   /* max_iw_mc_members */200,
   /* max_iw_programmble_filters */0,
   /* max_iw_policers */2,
   /* max_dscpmt */0,
   /* max_wred_bindings */0,
   /* tcm_policer_limits */
   { 0, 0 },
   /* max_flow_rule_stat_tables */0,
   /* max_indirect_action_tables */0,
   /*lpm_dfc_limits*/
   { 0 },
   /*brigding_time_stamp_info*/
   { 0 },
   /*background_fifo_size*/300

   } };

   /* An array of one WP_context structure for each WinPath */
   WP_context context[1] =
   {
         { POOL_LIMIT, /* qnodes */
               /* the first four assignments are mandatory bus assignments */
               {
                     {/* WP_BUS_PARAM    */WP_BUS_PARAM, APP_BANK_PARAM },
                     {/* WP_BUS_PACKET   */WP_BUS_PACKET, APP_BANK_PACKET },
                     {/* WP_BUS_INTERNAL */WP_BUS_INTERNAL, APP_BANK_INTERNAL },
                     {/* WP_BUS_HOST     */WP_BUS_HOST, APP_BANK_HOST },
                     {/* WP_BUS_IRQ      */WP_BUS_PACKET, APP_BANK_PACKET },
                     {/* WP_BUS_TMSTMP   */WP_BUS_HOST, APP_BANK_HOST },
                     {/* UNASSIGNED      */WP_BUS_NONE, 0 },
                     {/* UNASSIGNED      */WP_BUS_NONE, 0 },
                     {/* ASU_TPT         */WP_BUS_INTERNAL, APP_BANK_INTERNAL },
                     {/* ASU CW          */WP_BUS_INTERNAL, APP_BANK_INTERNAL },
                     {/* ASU TX QUEUE    */WP_BUS_PARAM, APP_BANK_PARAM },/*#10*/
                     {/* STATISTICS      */WP_BUS_INTERNAL, APP_BANK_INTERNAL },
                     {/* ADDRESS LOOKUP  */WP_BUS_PARAM, APP_BANK_PARAM },
                     {/* ATM CPT         */WP_BUS_PARAM, APP_BANK_PARAM },
                     {/* ENET HDLC CPT   */WP_BUS_PARAM, APP_BANK_PARAM },
                     {/* IW FBP PARAM    *//*WP_BUS_PARAM*/WP_BUS_HOST, /*APP_BANK_PARAM*/APP_BANK_HOST },
                     {/* IW FBP DATA     *//*WP_BUS_PACKET*/WP_BUS_PARAM, /*APP_BANK_PACKET*/WP_BUS_PARAM },
                     {/* EXTERNAL FIFO   */WP_BUS_PACKET, APP_BANK_PACKET },
                     {/* MAC LOOKUP      */WP_BUS_PARAM, APP_BANK_PARAM },
                     {/* IMA SMALL       */WP_BUS_INTERNAL, APP_BANK_INTERNAL },
                     {/* IMA LARGE       */WP_BUS_HOST, APP_BANK_HOST }, /*#20*/
                     {/* LPM             */WP_BUS_PARAM, APP_BANK_PARAM },
                     {/* MFC RULES       */WP_BUS_HOST, APP_BANK_HOST },
                     {/* FIWT, QAT       *//*WP_BUS_PARAM*/WP_BUS_INTERNAL, /*APP_BANK_PARAM */APP_BANK_INTERNAL },
                     {/* MFCPT, UPPPT, DS*/WP_BUS_INTERNAL, APP_BANK_INTERNAL },
                     {/* STATISTICS2      */WP_BUS_HOST, APP_BANK_HOST },
                     {/* STATISTICS3      */WP_BUS_HOST, APP_BANK_HOST },
                     {/* UNASSIGNED      */WP_BUS_NONE, 0 },
                     {/* PKT_GLOBAL_TCPT */WP_BUS_INTERNAL, APP_BANK_INTERNAL },
                     {/* UNASSIGNED      */WP_BUS_NONE, 0 },
                     {/* MANDATORY       */WP_BUS_NONE, 0 },
                     {/* MANDATORY       */WP_BUS_NONE, 0 },
                     /* ************* VIRTUAL BUS ASSIGNMENT ***************/
                     {/* MCH_DATA        */WP_BUS_PACKET, APP_BANK_PACKET },
                     {/* WP_VB_OAM_FM_QUERY*/WP_BUS_NONE, 0 },
                     {/* WP_VB_HASH_ON_HEAP*/WP_BUS_NONE, 0 },
                     {/* WP_VB_MFC_INTERNAL*/WP_BUS_PACKET, APP_BANK_PACKET } },
                     { dps_ProgramImage, dps_ProgramImage },
                     { dps_PC_Table_Init, dps_PC_Table_Init },
                     &int_queue_tables, /* Interrupt queue tables */
                     POOL_LIMIT, /* Mixumum number of pools */
                     atm_global_cfg, /* ATM global parameters */
                     max_iw, /* Interworking global parameters */
         }
   };
   WP_enet_multi_class_config multi_class_config[1] =
   {
         {
               /*multi_class_mode*/WP_ENET_MULTI_CLASS_MIXED_INTSW_EXTERNAL,
               /*max_multi_class_devices*/40,
         }
   };
   WP_fmu_shaping_wrr wrr_shaping[6] =
   {
         {
               /*weight*/ 1600*2,
               /*flags*/0,
         },

         {
               /*weight*/ 1600*1,
               /*flags*/0,
         },

         {
               /*weight*/ 1600*4,
               /*flags*/0,
         },

         {
               /*weight*/ 1600*3,
               /*flags*/0,
         },

         {
               /*weight*/ 1600*6,
               /*flags*/0,
         },

         {
               /*weight*/ 1600*2,
               /*flags*/0,
         }
   };

#ifdef USE_SOCKET
   InitSocketInterface();
#endif

   status = WP_DriverInit();
   WPE_TerminateOnError(status, "WP_DriverInit", __LINE__);

   WPE_InitializeContext(context,max_iw,int_queue_tables);
   status = WP_SysInit(WP_WINPATH(0), context);
   WPE_TerminateOnError(status, "WP_SysInit", __LINE__);

   printf("After sysinit();\n");

   status = WP_ModuleInit(WP_SYSHANDLE(0), WP_WDDI_MODULE_PCE, &pce_init_cfg);
   WPE_TerminateOnError(status, "WP_ModuleInit", __LINE__);

   status = WP_ModuleInit(WP_SYSHANDLE(0), WP_WDDI_MODULE_GPE, &gpe_module_init);
   WPE_TerminateOnError(status, "WP_ModuleInit()", __LINE__);

   status = WP_ModuleInit(WP_SYSHANDLE(DEFAULT_WPID), WP_WDDI_MODULE_POLICER, &policer_module_init);
   WPE_TerminateOnError(status, "WPI_PolicerModuleInit()", __LINE__);

   status = WPX_BoardConfigure(0, WPX_CONFIGURE_2XGI_6SGMII);
   WPE_TerminateOnError(status, "WPX_BoardConfigure()", __LINE__);

   /* Create Qnodes */
   for (ii = 0; ii < NUM_OF_QNODES; ii++)
   {
      switch (ii)
      {
         case QNODE_IW:

            buffer_data_vb->size = 756;

            pools[ii] = WP_PoolCreate(WP_WINPATH(0), WP_pool_iwbuffer, buffer_data_vb);
            WPE_TerminateOnError(pools[ii], "WP_PoolCreate() pools[ii]", __LINE__);

            qn_iw->adjunct_pool = pools[ii];
            qnodes[ii] = WP_QNodeCreate(0,
                                        WP_QNODE_IWQ
                                              | /*WP_QNODE_OPT_DEDICATED_RX_HWQ |*/WP_QNODE_OPT_FMU,
                                        qn_iw);
            WPE_TerminateOnError(qnodes[ii], "WP_QNodeCreate() qnodes[QNODE_IW]", __LINE__);
            break;
         case QNODE_RX_IW:

            memset(&iw_buffer_data, 0, sizeof(iw_buffer_data));
            iw_buffer_data.n_buffers = 51000;
            iw_buffer_data.offset = 64;
            iw_buffer_data.size = 512;
            iw_buffer_data.pad = 0;

         #ifdef WP_BOARD_WDS3_SL
            printf("\tcreate first BUFFER pool (PARAM_BUS)\n");
            iw_buffer_data.bus = WP_BUS_PARAM;
            iw_buffer_data.bank = APP_BANK_PARAM;
         #else
            printf("\tcreate first BUFFER pool (PACKET_BUS)\n");
            iw_buffer_data.bus = WP_BUS_PACKET;
            iw_buffer_data.bank = APP_BANK_PACKET;

         #endif // WP_BOARD_WDS3_SL
            buf_pool[0] = WP_PoolCreate(WP_WINPATH(0), WP_pool_iwbuffer, &iw_buffer_data);
            WPE_TerminateOnError(buf_pool[0], "WP_PoolCreate - Data Buffers (PACKET_BUS) for FMU FBP",__LINE__);

            memset(&iw_buffer_data, 0, sizeof(iw_buffer_data));
            iw_buffer_data.n_buffers = 51000;
            iw_buffer_data.offset = 64;
            iw_buffer_data.size = 512;
            iw_buffer_data.pad = 0;

         #ifdef WP_BOARD_WDS3_SL
            printf("\tcreate second BUFFER pool (PARAM_BUS)\n");
            iw_buffer_data.bus = WP_BUS_PARAM;
            iw_buffer_data.bank = APP_BANK_PARAM;
         #else
            printf("\tcreate second BUFFER pool (PARAM_BUS)\n");
            iw_buffer_data.bus = WP_BUS_PARAM;
            iw_buffer_data.bank = APP_BANK_PARAM;

         #endif // WP_BOARD_WDS3_SL
            buf_pool[1] = WP_PoolCreate(WP_WINPATH(0), WP_pool_iwbuffer, &iw_buffer_data);
            WPE_TerminateOnError(buf_pool[1], "WP_PoolCreate - Data Buffers (PARAM_BUS) for FMU FBP",__LINE__);

            printf("\tcreate first IW BD pool (INTERNAL_BUS)\n");
            memset(&iw_bd, 0, sizeof(WP_pool_iw_bd));
            iw_bd.num_buffer_descriptors = 12500;
            iw_bd.wmm_num_buffer_descriptors = 20;
            iw_bd.bd_bus = WP_BUS_INTERNAL;

            iw_bd.buffer_pool = buf_pool[0];
            iw_bd_pool[0] = WP_PoolCreate(WP_WINPATH(0), WP_POOL_IW_BD, &iw_bd);
            WPE_TerminateOnError(iw_bd_pool[0], "WP_PoolCreate - Buffer Descriptors (INTERNAL_BUS) for FMU FBP", __LINE__);

            printf("\tcreate second IW BD pool (INTERNAL_BUS)\n");
            memset(&iw_bd, 0, sizeof(WP_pool_iw_bd));
            iw_bd.num_buffer_descriptors = 12500;
            iw_bd.wmm_num_buffer_descriptors = 20;
            iw_bd.bd_bus = WP_BUS_INTERNAL;
            iw_bd.buffer_pool = buf_pool[1];
            iw_bd_pool[1] = WP_PoolCreate(WP_WINPATH(0), WP_POOL_IW_BD, &iw_bd);
            WPE_TerminateOnError(iw_bd_pool[1], "WP_PoolCreate - Buffer Descriptors (INTERNAL_BUS) for FMU FBP", __LINE__);

            printf("\tcreate third IW BD pool over (PACKET_BUS)\n");
            memset(&iw_bd, 0, sizeof(WP_pool_iw_bd));
            iw_bd.num_buffer_descriptors = 12500;
            iw_bd.wmm_num_buffer_descriptors = 20;
         #ifdef WP_BOARD_WDS3_SL
            iw_bd.bd_bus = WP_BUS_PARAM;
         #else
            iw_bd.bd_bus = WP_BUS_PACKET;
         #endif
            iw_bd.buffer_pool = buf_pool[0];
            iw_bd_pool[2] = WP_PoolCreate(WP_WINPATH(0), WP_POOL_IW_BD, &iw_bd);
            WPE_TerminateOnError(iw_bd_pool[2], "WP_PoolCreate - Buffer Descriptors (INTERNAL_BUS) for FMU FBP", __LINE__);

            printf("\tcreate fourth BD pool over (PACKET_BUS)\n");
            memset(&iw_bd, 0, sizeof(WP_pool_iw_bd));
            iw_bd.num_buffer_descriptors = 12500;
            iw_bd.wmm_num_buffer_descriptors = 20;
         #ifdef WP_BOARD_WDS3_SL
            iw_bd.bd_bus = WP_BUS_PARAM;
         #else
            iw_bd.bd_bus = WP_BUS_PACKET;
         #endif
            iw_bd.buffer_pool = buf_pool[1];
            iw_bd_pool[3] = WP_PoolCreate(WP_WINPATH(0), WP_POOL_IW_BD, &iw_bd);
            WPE_TerminateOnError(iw_bd_pool[3], "WP_PoolCreate - Buffer Descriptors (INTERNAL_BUS) for FMU FBP", __LINE__);

            memset(&qnodes_hier, 0, sizeof(WP_qnode_iw_hier));

            qnodes_hier.num_of_groups = 2;
            qnodes_hier.group_selection_method = WP_QNODE_IW_SELECTION_STRICT;

            qnodes_hier.iw_group[0].bd_pool[0] = iw_bd_pool[0];
            qnodes_hier.iw_group[0].bd_pool_weight[0] = &wrr_shaping[2];
            qnodes_hier.iw_group[0].bd_pool[1] = iw_bd_pool[1];
            qnodes_hier.iw_group[0].bd_pool_weight[1] = &wrr_shaping[3];
            qnodes_hier.iw_group[0].num_of_bd_pools = 2;
            qnodes_hier.iw_group[0].fifo_selection_method = WP_QNODE_IW_SELECTION_WRR;
            qnodes_hier.group_weight[0] = &wrr_shaping[0];

            qnodes_hier.iw_group[1].bd_pool[0] = iw_bd_pool[2];
            qnodes_hier.iw_group[1].bd_pool_weight[0] = &wrr_shaping[4];
            qnodes_hier.iw_group[1].bd_pool[1] = iw_bd_pool[3];
            qnodes_hier.iw_group[1].bd_pool_weight[1] = &wrr_shaping[5];
            qnodes_hier.iw_group[1].num_of_bd_pools = 2;
            qnodes_hier.iw_group[1].fifo_selection_method = WP_QNODE_IW_SELECTION_WRR;
            qnodes_hier.group_weight[1] = &wrr_shaping[1];

            qnodes[ii] = WP_QNodeCreate(0, WP_QNODE_IW_HIERARCHICAL, &qnodes_hier);
            WPE_TerminateOnError(qnodes[ii], "WP_QNodeCreate - FMU_FBP", __LINE__);

            break;
      }

   }
#if XGI
   status = WPX_BoardXgiConfig(0, WP_PORT_XGI1);
   WPE_TerminateOnError(status, "WPX_BoardXgiConfig()", __LINE__);
   status = WPX_BoardSerdesInit(0, WP_PORT_XGI1, WPX_SERDES_NORMAL);
   WPE_TerminateOnError(status, "WPX_BoardSerdesInit()", __LINE__);

   xgi1_port_handle = WP_PortCreate(WP_WINPATH(0), WP_PORT_XGI1, port_cfg);
   WPE_TerminateOnError(xgi1_port_handle, "WP_PortCreate XGI1", __LINE__);
#else
   status = WPX_BoardSerdesInit(0, WP_PORT_ENET14, WP_FALSE);
   WPE_TerminateOnError(status, "WPX_BoardSerdesInit()", __LINE__);

   /*Create  Port*/
   port_cfg[0].interface_mode = /*WP_ENET_SGMII_1000*/WP_ENET_SGMII_AN;
//   port_cfg[0].flowmode = WP_FLOWMODE_FAST;
   xgi1_port_handle = WP_PortCreate(WP_WINPATH(0), WP_PORT_ENET14, port_cfg);
   WPE_TerminateOnError(xgi1_port_handle, "WP_PortCreate", __LINE__);
#endif

   /* Creating device */
   xgi1_device_handle = WP_DeviceCreate(xgi1_port_handle, WP_PHY(0), WP_DEVICE_ENET, device_cfg);
   WPE_TerminateOnError(xgi1_device_handle, "WP_DeviceCreate XGI1", __LINE__);

#if XGI2
   status = WPX_BoardXgiConfig(0, WP_PORT_XGI2);
   WPE_TerminateOnError(status, "WPX_BoardXgiConfig()", __LINE__);
   status = WPX_BoardSerdesInit(0, WP_PORT_XGI2, WPX_SERDES_LOOPBACK);
   WPE_TerminateOnError(status, "WPX_BoardSerdesInit()", __LINE__);

   xgi2_port_handle = WP_PortCreate(WP_WINPATH(0), WP_PORT_XGI2, port_cfg);
   WPE_TerminateOnError(xgi2_port_handle, "WP_PortCreate XGI1", __LINE__);

#else
   status = WPX_BoardSerdesInit(0, WP_PORT_ENET15, WP_TRUE);
   WPE_TerminateOnError(status, "WPX_BoardSerdesInit()", __LINE__);

   /*Create  Port*/
   port_cfg[0].interface_mode = WP_ENET_SGMII_1000/*WP_ENET_SGMII_AN*/;
//   port_cfg[0].flowmode = WP_FLOWMODE_FAST;
   xgi2_port_handle = WP_PortCreate(WP_WINPATH(0), WP_PORT_ENET15, port_cfg);
   WPE_TerminateOnError(xgi1_port_handle, "WP_PortCreate", __LINE__);

#endif

   xgi2_device_handle = WP_DeviceCreate(xgi2_port_handle, WP_PHY(0), WP_DEVICE_ENET, device_cfg);
   WPE_TerminateOnError(xgi2_device_handle, "WP_DeviceCreate XGI2", __LINE__);

   /* Modifying device */
   memset(enet_device_extended_cfg, 0, sizeof(WP_device_enet_ex));
   memset(rx_filters, 0, sizeof(WP_enet_filters));

   rx_filters->rx_ucfilter = WP_ENET_FILTER_PROCESS;
   rx_filters->rx_bcfilter = WP_ENET_FILTER_PROCESS;
   rx_filters->rx_mcfilter = WP_ENET_FILTER_PROCESS;

   enet_device_extended_cfg[0].duplexmode = WP_ENET_FULLDUPLEX;
   enet_device_extended_cfg->rx_maclookup_mode = WP_DISABLE;
   enet_device_extended_cfg->rx_flowcontrol = WP_ENET_FLOW_ENABLE;
   enet_device_extended_cfg->tx_flowcontrol = WP_ENET_FLOW_DISABLE;
   enet_device_extended_cfg->rx_addrmode = WP_ENET_ADDR_ACCEPTALL;
   enet_device_extended_cfg->phystatmode = WP_ENET_STAT_ENABLE;
   enet_device_extended_cfg->tx_statmode = WP_PKT_STAT_ENABLE;
   enet_device_extended_cfg->rx_statmode = WP_PKT_STAT_ENABLE;
   enet_device_extended_cfg->tx_duplicate = WP_DISABLE;
   enet_device_extended_cfg->rx_filters = rx_filters;
   enet_device_extended_cfg->rx_timestamp = WP_DISABLE;
   enet_device_extended_cfg->max_tx_slow_channels = 0;
   enet_device_extended_cfg->tx_timestamp = 0;
//#if XGI
//   enet_device_extended_cfg->tx_mode_10G = 1;
//#else
//   enet_device_extended_cfg->tx_mode_10G = 0;
//#endif
   ////////////////////////////////////////
   enet_device_extended_cfg->rx_internal_switching_traffic_mode = WP_DISABLE;

   enet_device_extended_cfg->multi_class_config = NULL;
   device_cfg->extended_params = enet_device_extended_cfg;
   ///////////////////////////////
   status = WP_DeviceModify(xgi1_device_handle,
                            WP_DEV_MOD_ENET_PHYSTATMODE
                            | WP_DEV_MOD_ENET_TX_STATMODE
                            | WP_DEV_MOD_ENET_RX_STATMODE
//#if XGI
//                            | WP_DEV_MOD_ENET_10G_LONG_PKT_MODE
//#endif
                            | WP_DEV_MOD_ENET_TX_FLOWCONTROL,
                            device_cfg);
   WPE_TerminateOnError(status, "WP_DeviceModify() XGI1", __LINE__);

#if XGI2

//   enet_device_extended_cfg->tx_mode_10G = 1;
   enet_device_extended_cfg->rx_internal_switching_traffic_mode = WP_DISABLE;
   enet_device_extended_cfg->multi_class_config = NULL;
   device_cfg->extended_params = enet_device_extended_cfg;

   status = WP_DeviceModify(xgi2_device_handle,
                            WP_DEV_MOD_ENET_PHYSTATMODE
                            | WP_DEV_MOD_ENET_TX_STATMODE
                            | WP_DEV_MOD_ENET_RX_STATMODE
//                            | WP_DEV_MOD_ENET_10G_LONG_PKT_MODE
                            | WP_DEV_MOD_ENET_TX_FLOWCONTROL,
                            device_cfg);
   WPE_TerminateOnError(status, "WP_DeviceModify() XGI1", __LINE__);
#else
#endif

   /* Create an IW Host port */
   host_port = WP_PortCreate(WP_WINPATH(0), WP_PORT_IW_HOST, NULL);
   WPE_TerminateOnError(host_port, "WP_PortCreate() IW Host", __LINE__);

   /* Create an IW Host Device */
   host_device = WP_DeviceCreate(host_port, 0, WP_DEVICE_IW_HOST, NULL);
   WPE_TerminateOnError(host_device, "WP_DeviceCreate() IW Host", __LINE__);

} /* WPE_SystemSetup */
#include "../core/host_io/wpi_host_io.c"
#include "wt_partition_ecc_util.c"
