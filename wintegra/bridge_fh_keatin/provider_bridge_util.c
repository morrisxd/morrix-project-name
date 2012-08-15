/*****************************************************************************
 * (C) Copyright PMC - WIN division (Wintegra) 2012.  All rights reserved.
 * PMC-Sierra (WINTEGRA) CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of PMC-Sierra (Wintegra).
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

#include "provider_bridge_util.h"

void WPE_Pecs_Init (WP_gpe_pecs gpe_pecs_config[], WP_handle pecs_handle[])
{
   WP_U32 ii;

   for (ii = 0; ii < NUM_OF_PECS; ii++)
   {
      /* It loads and relocates the PECS DPS image from the predefined PECS library
       * to the DPS program RAM. It also registers the PECS Service Function.
       */
      pecs_handle[ii] =
         WP_GpePecsCreate (WP_WINPATH (DEFAULT_WPID), WP_GPE_PECS,
                           (void *) &gpe_pecs_config[ii]);
      printf ("CREATING PECS: handle returned: %x\n", pecs_handle[ii]);
      App_TerminateOnError (pecs_handle[ii], "WP_GpePecsCreate", __LINE__);
   }
}

void F_MyAppIndicateEventIwgpLearning (WP_tag tag, WP_U32 data,
                                       WP_U32 info)
{
#if LEARNING_MODE_INTERRUPT
   /* Tell the Learning thread there are rules need to learn */
   WPL_SemIncrement (&learning_sem, 1);
#endif

   return;
}

void WPE_PortsCreate (WP_handle iw_sys, WP_handle * iwport,
                      WP_U32 max_mac_address)
{
   WP_bridge_port bport_conf = {
      /* tag */ 0,
      /* direct_mapping */ WP_IW_DIRECT_MAP_ENABLE,
      /* flow_agg */ 0,
      /* termination_mode */ WP_IW_HOST_TERM_MODE,
      /* learning_mode */ WP_IW_LEARNING_DISABLE,
      /* in_filter_mode */ WP_IW_INGRESS_FILTER_DISABLE,
      /* vlan_param */
      {
       /* vlan_acceptance_mode */ WP_IW_ACCEPT_TAGGED_ONLY,
       /* vlan_tag */ 0,
       /*vlan_tunnel */ WP_IW_VLAN_TUNNEL_1Q_IN_1Q_DISABLE,
       /*vlan_priority_enforce_mode */
       WP_IW_VLAN_PRIORITY_ENFORCE_DISABLED,
       /*stag_vlan_etype */ WP_IW_BPORT_STAG_VLAN_ETYPE_DISABLE
       },
      /* max_mac_addresses */ 300,
      /* group_tag */ 0,
      /*group_filtering_mode */ WP_IW_GROUP_FILTER_DISABLE,
      /*unk_mac_sa_filter */ WP_IW_UNK_MACSA_FILTER_DISABLE,
      /*unk_mc_mode */ WP_IW_UNK_MC_DROP,
      /*bc_ht_mode */ WP_IW_BC_HT_DISABLE,
      /*unk_input_filters_mask */ 0,
      /*unk_output_filters_mask */ 0,
      /*statmode */ WP_IW_PORT_STAT_ENABLE,
      /*unk_uc_mode */ WP_IW_UNK_UC_SR_ENABLE,
      /*classification_flag */ WP_IW_BPORT_CLASSIFICATION_ENABLED,
      /*adv_unk_lookup_mode */ WP_IW_ADV_UNK_LOOKUP_ENABLED,
      /*cfi_ht_mode */ WP_IW_CFI_HT_DISABLE,
      /*reserved_mc_ht_mode */ WP_IW_RES_MC_HT_DISABLE,
      /*predefined_ht_mode */ WP_DISABLE,
      /*res_mc_action_table */ {0},
      /*WP_iw_bport_enet_oam_params */ NULL
   };

   bport_conf.flow_agg = default_agg_host;
   bport_conf.tag = 0;
   bport_conf.max_mac_addresses = max_mac_address;

   *iwport = WP_IwPortCreate (iw_sys, &bport_conf);   // direct map
   App_TerminateOnError (*iwport, "WP_IwPortCreate ", __LINE__);
}

void WPE_PortsVlanTagMdf (WP_handle iwport, WP_U16 vlan_tag)
{
   WP_status status = 0;

   WP_bridge_port bport_conf = {
      /* tag */ 0,
      /* direct_mapping */ WP_IW_DIRECT_MAP_ENABLE,
      /* flow_agg */ 0,
      /* termination_mode */ WP_IW_HOST_TERM_MODE,
      /* learning_mode */ WP_IW_LEARNING_DISABLE,
      /* in_filter_mode */ WP_IW_INGRESS_FILTER_DISABLE,
      /* vlan_param */
      {
       /* vlan_acceptance_mode */ WP_IW_ACCEPT_TAGGED_ONLY,
       /* vlan_tag */ 0,
       /*vlan_tunnel */ WP_IW_VLAN_TUNNEL_1Q_IN_1Q_DISABLE,
       /*vlan_priority_enforce_mode */
       WP_IW_VLAN_PRIORITY_ENFORCE_DISABLED,
       /*stag_vlan_etype */ WP_IW_BPORT_STAG_VLAN_ETYPE_DISABLE
       },
      /* max_mac_addresses */ 300,
      /* group_tag */ 0,
      /*group_filtering_mode */ WP_IW_GROUP_FILTER_DISABLE,
      /*unk_mac_sa_filter */ WP_IW_UNK_MACSA_FILTER_DISABLE,
      /*unk_mc_mode */ WP_IW_UNK_MC_DROP,
      /*bc_ht_mode */ WP_IW_BC_HT_DISABLE,
      /*unk_input_filters_mask */ 0,
      /*unk_output_filters_mask */ 0,
      /*statmode */ WP_IW_PORT_STAT_ENABLE,
      /*unk_uc_mode */ WP_IW_UNK_UC_SR_ENABLE,
      /*classification_flag */ WP_IW_BPORT_CLASSIFICATION_ENABLED,
      /*adv_unk_lookup_mode */ WP_IW_ADV_UNK_LOOKUP_ENABLED,
      /*cfi_ht_mode */ WP_IW_CFI_HT_DISABLE,
      /*reserved_mc_ht_mode */ WP_IW_RES_MC_HT_DISABLE,
      /*predefined_ht_mode */ WP_DISABLE,
      /*res_mc_action_table */ {0},
      /*WP_iw_bport_enet_oam_params */ NULL
   };

   bport_conf.flow_agg = default_agg_host;
   bport_conf.vlan_param.vlan_tag = vlan_tag;
   bport_conf.vlan_param.vlan_acceptance_mode =
      WP_IW_ACCEPT_TAGGED_UNTAGGED;
   bport_conf.max_mac_addresses = max_mac_addresses;

   status =
      WP_IwPortModify (iwport,
                       WP_IW_BPORT_MOD_VLAN_DEFAULT_TAG |
                       WP_IW_BPORT_MOD_VLAN_ACCEPT_MODE, &bport_conf);
   App_TerminateOnError (status, "WP_IwPortModify ", __LINE__);

}

void WPE_HostCreate (void)
{
   WP_tx_binding tx_binding_iw[1] = {
      {
       /* res0; */ 0,
       /* dci_mode; */ WP_IW_DYN_CH_INSERT_DISABLE,
       /* maxt; */ 255,
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
                                       //WP_IW_INT_DISABLE,
       /*statmode; */ WP_IW_STAT_ENABLE,
       /*timestamp_mode; */ WP_IW_TIME_STAMP_DISABLE,
       /*ov_pool_mode */ WP_IW_OV_POOL_DISABLE,
       /*fbp_drop_threshold; */ 0,
       /*replace_vlan_id */ WP_IW_REPLACE_VTAG_DISABLE,
       /*vlan_id */ 0x0,
       /*vpmt_handle */ 0,
       /*mtu; */ WT_MAX_FRAME_SIZE,
       /*prefix_length */ 0,
       /*prefix_header[28]; */ {0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7,
                                0x8, 0x9, 0x10, 0x11, 0x12, 0x13, 0x14,
                                0x15, 0x16, 0x17, 0x18, 0x19, 0x20, 0x21,
                                0x22, 0x23, 0x24, 0, 0, 0, 0},
       /*policer_enable */ WP_IW_POLICER_DISABLE,
                                /**policer_config;*/ NULL,
       /*cong_mode; */ WP_IW_CONGESTION_DISABLE,
                                /**cong_threshold_param;*/ NULL
       }
   };
   WP_bridge_port iwhost_bport_config[1] = {
      {
       /* tag */ 0x2000,
       /* direct_mapping *//*WP_IW_DIRECT_MAP_DISABLE,// */
       WP_IW_DIRECT_MAP_ENABLE,
       /* flow_agg */ 0,
       /* flooding_term_mode */ WP_IW_HOST_TERM_MODE,
       /* learning_mode */ WP_IW_LEARNING_DISABLE,
                                                         /* in_filter_mode */ WP_IW_INGRESS_FILTER_ENABLE,
                                                         //WP_IW_INGRESS_FILTER_DISABLE,
       /* vlan_param */
       {
                                                                  /* vlan_acceptance_mode */ WP_IW_ACCEPT_TAGGED_UNTAGGED,
                                                                  // WP_IW_ACCEPT_TAGGED_ONLY,
        /* vlan_tag */ 0x5,
        /*vlan_tunnel */ WP_IW_VLAN_TUNNEL_1Q_IN_1Q_DISABLE,
        /*vlan_priority_enforce_mode */
        WP_IW_VLAN_PRIORITY_ENFORCE_DISABLED,
        /*stag_vlan_etype */ WP_IWPORT_STAG_DISABLED
        },
       /*max_mac_addresses */ 34,
       /*group_tag */ 0,
       /*group_filtering_mode */ WP_IW_GROUP_FILTER_DISABLE,
       /*unk_mac_sa_filter */ 0,
       /*unk_mc_mode; */ 0,
       /*bc_ht_mode; */ 0,
       /*input_filters_mask */ 0,
       /*output_filters_mask; */ 0,
       /*statmode */ WP_IW_PORT_STAT_ENABLE,
       /*unk_uc_mode; */ WP_IW_UNK_UC_SR_ENABLE,
       /*classification_flag */ WP_IW_BPORT_CLASSIFICATION_ENABLED,
       }
   };

   /* create host rx channel */
   rx_host_handle = WP_ChannelCreate (0x1111,
                                      devhost,
                                      qniw,
                                      WP_CH_RX,
                                      WP_IW_HOST, &ch_config_iw[0]);
   App_TerminateOnError (rx_host_handle, "WP_ChannelCreate() Host sys",
                         __LINE__);

   /* create host flow aggregation */
   br_agg_gbe->txfunc = rx_host_handle;
   default_agg_host =
      WP_IwFlowAggregationCreate (WP_WINPATH (DEFAULT_WPID),
                                  WP_IW_VLAN_AWARE_BRIDGING_MODE,
                                  &br_agg_gbe[0]);
   App_TerminateOnError (default_agg_host, "WP_IwFlowAggregationCreate()",
                         __LINE__);

   /* create bridging port */
   iwhost_bport_config->flow_agg = default_agg_host;
   h_iw_port_general_host =
      WP_IwPortCreate (dl_general_iwsys_bridge, &iwhost_bport_config[0]);
   App_TerminateOnError (h_iw_port_general_host, " WP_IwPortCreate()",
                         __LINE__);
}

void WPE_BridgePortsCreate (WP_handle iw_sys)
{
   WP_U32 ii;

   // gbe bports
   for (ii = 0; ii < NR_GBE; ii++)
   {
      WPE_PortsCreate (iw_sys, &gbe[ii].bport_enet, max_mac_addresses);
   }
}

void WPE_RxBindingCreate (WP_handle iw_sys)
{
   WP_status status;
   WP_U16 ii;

   WP_rx_binding_bridging rx_binding_cfg[1] = {
      {
       /*  encap_mode */ 0,
       /*  mru; */ 512,
       /*  vcfcs; */ 0,
       /*  input_port; */ 0
       }
   };

   /* GE bindging */
   for (ii = 0; ii < NR_GBE; ii++)
   {
      rx_binding_cfg[0].input_port = gbe[ii].bport_enet;
      rx_binding_cfg[0].pce_if_handle = 0;

      status = WP_IwRxBindingCreate (gbe[ii].rx_chan_enet, iw_sys,
                                     qniw, &rx_binding_cfg[0]);
      App_TerminateOnError (status, "WP_IwRxBindingCreate Enet", __LINE__);
   }
}

void WPE_RxBindingMdfVlan (WP_handle iw_sys, WP_U32 port_num)
{
   WP_status status;

   WP_rx_binding_bridging rx_binding_cfg[1] = {
      {
       /*  encap_mode */ 0,
       /*  mru; */ 512,
       /*  vcfcs; */ 0,
       /*  input_port; */ 0
       }
   };

   if ( /*(port_num >= 0) && */ (port_num < NR_GBE))
   {
      rx_binding_cfg[0].input_port = gbe[port_num].bport_enet;
      status = WP_IwRxBindingModify (gbe[port_num].rx_chan_enet, iw_sys,
                                     qniw,
                                     WP_IW_RX_BIND_B_MOD_BPORT,
                                     &rx_binding_cfg[0]);
   }
   else
   {
      printf
         ("port number %d : Wrong port number!Valid value is from %d to %d\n",
          port_num, 0, NR_GBE);
      return;
   }

   App_TerminateOnError (status, "WPE_RxBindingMdfVlan", __LINE__);

}

void WPE_BridgePortsVlanTagSet (WP_U32 port_num, WP_U16 vlan_tag)
{
   if ( /*(port_num >= 0)&& */ (port_num < NR_GBE))
      WPE_PortsVlanTagMdf (gbe[port_num].bport_enet, vlan_tag);
   else
      printf
         ("port number %d : Wrong port number!Valid value is from %d to %d\n",
          port_num, 0, NR_GBE);

   //WPE_RxBindingMdfVlan(dl_general_iwsys_bridge, port_num);
}

void WPE_TxAggCreate (void)
{
   WP_U32 i;

   WP_iw_agg_generic dl_tx_agg_gbe[1] = {       // outging port flowAgg
      {
       /*tag */ 2,
       /*txfunc */ 0,
       /*iw_port */ 0,
       /*rfcs */ WP_IW_RFCS_ENABLE,
       /*interruptqueue; */ WP_IW_IRQT1,
       /*error_pkt_mode */ WP_IW_ERRPKT_DISCARD,
       /*intmode; */ WP_IW_INT_DISABLE,
       /*statmode; */ WP_IW_STAT_ENABLE,
       /*timestamp_mode; */ WP_IW_TIME_STAMP_DISABLE,
       /*mtu; */ 9216,
       /*flow_agg_type; */ WP_IW_FLOW_AGG_PRIMARY,
       /*policer_handle; */ 0,
       /*pecs_handle; */ 0,
       /*pecs_flow_info; */ 0,
       /*pecs_global_info_handle; */ 0,
       },
   };
   WPE_gpe_brouter_pecs_flow_info brouter_pecs_flow_info[] = {
      /*  */
      {
       WPE_GPE_BROUTER_PECS_EXTRACT_DISABLE, /* header_extract_mode */
       0,                       /* extraction_size */
       WPE_GPE_BROUTER_PECS_ADD_DISABLE,  /* prefix_add_mode */
       4,                       /* prefix_add_size */
       14,                      /* prefix_ip_offset */
       WPE_GPE_BROUTER_PECS_REPLACE_MAC_DISABLE,   /* mac_replace_mode */
       {0x11, 0x22, 0x33, 0x44, 0x55, 0x00}, /* mac_da */
       {0x66, 0x77, 0x88, 0x99, 0xaa, 0x00}, /* mac_sa */
       0,                       /* vlan_edit_options */

       {0},                     /* prefix_remark_options */
       WPE_GPE_BROUTER_PECS_TTL_DISABLE,  /* ttl_mode */
       WPE_GPE_BROUTER_PECS_TOS_REMARKING_DISABLE, /* tos_remarking_mode */
       {                        /* prefix */
        0x11, 0x22, 0x33, 0x44, 0x55, 0x00,
        0x11, 0x12, 0x13, 0x14, 0x17, 0x00,
        0x81, 0, 0, 1,
        0x88, 0x47,
        0x11, 0x11, 0x10, 0x80,
        0x22, 0x22, 0x21, 0x80,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0}
       }
   };

   /* DL GE TX flow agg */
   for (i = 0; i < NR_GBE; i++)
   {
      dl_tx_agg_gbe->txfunc = gbe[i].tx_chan_enet;
      dl_tx_agg_gbe->iw_port = gbe[i].bport_enet;
      dl_tx_agg_gbe->pecs_handle = pecs_handles[0];
      dl_tx_agg_gbe->pecs_flow_info = (void *) &brouter_pecs_flow_info[0];
      gbe[i].agg_enet =
         WP_IwFlowAggregationCreate (WP_WINPATH (DEFAULT_WPID),
                                     WP_IW_GENERIC_MODE, &dl_tx_agg_gbe);
      App_TerminateOnError (gbe[i].agg_enet,
                            "WP_IwFlowAggregationCreate()", __LINE__);
   }
}

void WTE_CreateUDFSet (void)
{
   memset (&pce_user_programmable_fields_set_config, 0,
           sizeof (WP_pce_user_programmable_fields_set_config));

   pce_user_programmable_fields_set_config.
      pce_user_programmable_field_config[0].
      pce_user_programmable_field_ref_point =
      WP_PCE_UPF_REF_POINT_LAYER4_START;
   pce_user_programmable_fields_set_config.
      pce_user_programmable_field_config[0].
      user_programmable_field_collected_field_id =
      WP_PCE_FIELD_ID_USER_PROGRAMMABLE_FIELDS;
   pce_user_programmable_fields_set_config.
      pce_user_programmable_field_config[0].field_offset = 10;
   pce_user_programmable_fields_set_config.
      pce_user_programmable_field_config[0].field_size = 1;

   pce_upf_set =
      WP_PceUserProgrammableFieldsSetCreate (WP_SYSHANDLE (DEFAULT_WPID),
                                             &pce_user_programmable_fields_set_config);
   App_TerminateOnError (pce_upf_set,
                         "WP_PceUserProgrammableFieldsSetCreate",
                         __LINE__);

   return;
}

/*-------------------------------------------------*\
   PCE_filter[FILTER_SET_UNKNOWNUNICAST_CLASSIFICATION] =
   PCE_filter[FILTER_SET_IPV6_IP_CLASSIFICATION] =
   PCE_filter[FILTER_SET_L4_PORT_CLASSIFICATION] =
   PCE_filter[FILTER_SET_L4_SUBTYPE_CLASSIFICATION] =
   PCE_filter[FILTER_SET_RESERVED_MAC_CLASSIFICATION] =
   PCE_filter[FILTER_SET_CLASSIFICATION] =   // for learning flowAgg
   PCE_filter[FILTER_SET_FORWARDING] =
   PCE_filter[FILTER_SET_LEARNING] =
\*-------------------------------------------------*/
void WPE_CreatePceFilters (void)
{
   WP_pce_filter_classification unicast_filter_class = { 0 };
   WP_pce_filter_forwarding fwd_filter_cfg = { 0 };
   WP_pce_filter_learning lrn_filter_cfg = { 0 };
   WP_pce_filter_classification filter_class = { 0 };

   /* create unknown unicast classification filter */
   unicast_filter_class.no_match_action = WP_PCE_FILTER_NO_MATCH_CONTINUE;
   unicast_filter_class.no_fields_action = WP_PCE_FILTER_NO_FIELDS_DENY;

   unicast_filter_class.no_match_result[0].result_type =
      WP_PCE_RESULT_LAST;

   unicast_filter_class.filter_fields[0].field_mode =
      WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   unicast_filter_class.filter_fields[0].mask_mode =
      WP_PCE_FIELD_MASK_NOT_USED;
   unicast_filter_class.filter_fields[0].field_id =
      WP_PCE_FIELD_ID_VLAN_TAG;
   unicast_filter_class.filter_fields[1].field_id = WP_PCE_FIELD_ID_LAST;
   PCE_filter[FILTER_SET_UNKNOWNUNICAST_CLASSIFICATION] =
      WP_PceFilterCreate (WP_WINPATH (DEFAULT_WPID),
                          WP_PCE_FILTER_CLASSIFICATION,
                          &unicast_filter_class);
   App_TerminateOnError (PCE_filter
                         [FILTER_SET_UNKNOWNUNICAST_CLASSIFICATION],
                         "WP_PceFilterCreate", __LINE__);

   /* ipv6 match */
   memset (&filter_class, 0, sizeof (filter_class));

   filter_class.no_match_action = WP_PCE_FILTER_NO_MATCH_CONTINUE;
   filter_class.no_fields_action = WP_PCE_FILTER_NO_FIELDS_CONTINUE; //WP_PCE_FILTER_NO_FIELDS_DENY;

   filter_class.no_match_result[0].result_type = WP_PCE_RESULT_LAST;

   filter_class.filter_fields[0].field_id = WP_PCE_FIELD_ID_INPUT_IW_PORT;
   filter_class.filter_fields[0].field_mode =
      WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_class.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   filter_class.filter_fields[1].field_id = WP_PCE_FIELD_ID_IPV6_DA_HIGH;
   filter_class.filter_fields[1].field_mode =
      WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_class.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   filter_class.filter_fields[2].field_id = WP_PCE_FIELD_ID_IPV6_DA_LOW;
   filter_class.filter_fields[2].field_mode =
      WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_class.filter_fields[2].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   filter_class.filter_fields[3].field_id = WP_PCE_FIELD_ID_LAST;

   PCE_filter[FILTER_SET_IPV6_IP_CLASSIFICATION] =
      WP_PceFilterCreate (WP_WINPATH (DEFAULT_WPID),
                          WP_PCE_FILTER_CLASSIFICATION, &filter_class);
   App_TerminateOnError (PCE_filter[FILTER_SET_IPV6_IP_CLASSIFICATION],
                         "WP_PceFilterCreate", __LINE__);

   /* L4 port */
   memset (&filter_class, 0, sizeof (filter_class));

   filter_class.no_match_action = WP_PCE_FILTER_NO_MATCH_CONTINUE;
   filter_class.no_fields_action = WP_PCE_FILTER_NO_FIELDS_CONTINUE; //WP_PCE_FILTER_NO_FIELDS_DENY;

   filter_class.no_match_result[0].result_type = WP_PCE_RESULT_LAST;

   filter_class.filter_fields[0].field_id = WP_PCE_FIELD_ID_INPUT_IW_PORT;
   filter_class.filter_fields[0].field_mode =
      WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_class.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   filter_class.filter_fields[1].field_mode =
      WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_class.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;
   filter_class.filter_fields[1].field_id = WP_PCE_FIELD_ID_L4_DST_PORT;

   filter_class.filter_fields[2].field_id = WP_PCE_FIELD_ID_LAST;

   PCE_filter[FILTER_SET_L4_PORT_CLASSIFICATION] =
      WP_PceFilterCreate (WP_WINPATH (DEFAULT_WPID),
                          WP_PCE_FILTER_CLASSIFICATION, &filter_class);
   App_TerminateOnError (PCE_filter[FILTER_SET_L4_PORT_CLASSIFICATION],
                         "WP_PceFilterCreate", __LINE__);

   /* L4 port  & subtype */
   memset (&filter_class, 0, sizeof (filter_class));

#define RX_CHANNEL_FILTER_TAG          0x2200

   filter_class.tag = RX_CHANNEL_FILTER_TAG;

   filter_class.no_match_action = WP_PCE_FILTER_NO_MATCH_CONTINUE;
   filter_class.no_fields_action = WP_PCE_FILTER_NO_FIELDS_CONTINUE; //WP_PCE_FILTER_NO_FIELDS_DENY;

   filter_class.no_match_result[0].result_type = WP_PCE_RESULT_LAST;

   filter_class.filter_fields[0].field_id = WP_PCE_FIELD_ID_INPUT_IW_PORT;
   filter_class.filter_fields[0].field_mode =
      WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_class.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   filter_class.filter_fields[1].field_id =
      WP_PCE_FIELD_ID_USER_PROGRAMMABLE_FIELDS;
   filter_class.filter_fields[1].field_mode =
      WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_class.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_USED;
   filter_class.filter_fields[1].mask.pce_user_programmable_fields =
      0xff000000;

   filter_class.filter_fields[2].field_id = WP_PCE_FIELD_ID_LAST;

   PCE_filter[FILTER_SET_L4_SUBTYPE_CLASSIFICATION] =
      WP_PceFilterCreate (WP_WINPATH (DEFAULT_WPID),
                          WP_PCE_FILTER_CLASSIFICATION, &filter_class);
   App_TerminateOnError (PCE_filter[FILTER_SET_L4_SUBTYPE_CLASSIFICATION],
                         "WP_PceFilterCreate", __LINE__);

   /* Reserved MAC */
   memset (&filter_class, 0, sizeof (filter_class));

   filter_class.no_match_action = WP_PCE_FILTER_NO_MATCH_CONTINUE;
   filter_class.no_fields_action = WP_PCE_FILTER_NO_FIELDS_CONTINUE; //WP_PCE_FILTER_NO_FIELDS_DENY;

   filter_class.no_match_result[0].result_type = WP_PCE_RESULT_LAST;

   filter_class.filter_fields[0].field_id = WP_PCE_FIELD_ID_INPUT_IW_PORT;
   filter_class.filter_fields[0].field_mode =
      WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_class.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   filter_class.filter_fields[1].field_id = WP_PCE_FIELD_ID_MAC_DA;
   filter_class.filter_fields[1].field_mode =
      WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_class.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   filter_class.filter_fields[2].field_id = WP_PCE_FIELD_ID_LAST;

   PCE_filter[FILTER_SET_RESERVED_MAC_CLASSIFICATION] =
      WP_PceFilterCreate (WP_WINPATH (DEFAULT_WPID),
                          WP_PCE_FILTER_CLASSIFICATION, &filter_class);
   App_TerminateOnError (PCE_filter
                         [FILTER_SET_RESERVED_MAC_CLASSIFICATION],
                         "WP_PceFilterCreate", __LINE__);

   /* create classification for learning flow agg filter */
   memset (&filter_class, 0, sizeof (filter_class));

   filter_class.no_match_action = WP_PCE_FILTER_NO_MATCH_CONTINUE;
   filter_class.no_fields_action = WP_PCE_FILTER_NO_FIELDS_DENY;

   filter_class.no_match_result[0].result_type = WP_PCE_RESULT_LAST;

   filter_class.filter_fields[0].field_id = WP_PCE_FIELD_ID_INPUT_IW_PORT;
   filter_class.filter_fields[0].field_mode =
      WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_class.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   filter_class.filter_fields[1].field_mode =
      WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_class.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_USED;
   filter_class.filter_fields[1].mask.vlan_tag = WP_PCE_FIELD_MASK_VLAN_ID;
   filter_class.filter_fields[1].field_id = WP_PCE_FIELD_ID_VLAN_TAG;

   filter_class.filter_fields[2].field_id = WP_PCE_FIELD_ID_LAST;

   PCE_filter[FILTER_SET_CLASSIFICATION] =
      WP_PceFilterCreate (WP_WINPATH (DEFAULT_WPID),
                          WP_PCE_FILTER_CLASSIFICATION, &filter_class);
   App_TerminateOnError (PCE_filter[FILTER_SET_CLASSIFICATION],
                         "WP_PceFilterCreate", __LINE__);

   /* forward filter */
   fwd_filter_cfg.no_match_action = WP_PCE_FILTER_NO_MATCH_CONTINUE;
   fwd_filter_cfg.no_fields_action = WP_PCE_FILTER_NO_FIELDS_DENY;

   fwd_filter_cfg.no_match_result[0].result_type = WP_PCE_RESULT_LAST;

   fwd_filter_cfg.filter_fields[0].field_mode =
      WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   fwd_filter_cfg.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   fwd_filter_cfg.filter_fields[1].field_mode =
      WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   fwd_filter_cfg.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_USED;
   fwd_filter_cfg.filter_fields[1].mask.vlan_tag =
      WP_PCE_FIELD_MASK_VLAN_ID;

   fwd_filter_cfg.filter_fields[2].field_mode =
      WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   fwd_filter_cfg.filter_fields[2].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   fwd_filter_cfg.filter_fields[0].field_id = WP_PCE_FIELD_ID_MAC_DA;
   fwd_filter_cfg.filter_fields[1].field_id = WP_PCE_FIELD_ID_VLAN_TAG;
   fwd_filter_cfg.filter_fields[2].field_id = WP_PCE_FIELD_ID_LAST;

   PCE_filter[FILTER_SET_FORWARDING] =
      WP_PceFilterCreate (WP_WINPATH (DEFAULT_WPID),
                          WP_PCE_FILTER_FORWARDING, &fwd_filter_cfg);
   App_TerminateOnError (PCE_filter[FILTER_SET_FORWARDING],
                         "WP_PceFilterCreate", __LINE__);

   /* learning fitler */
   lrn_filter_cfg.forwarding_filter = PCE_filter[FILTER_SET_FORWARDING];
   lrn_filter_cfg.no_match_action = WP_PCE_FILTER_NO_MATCH_CONTINUE;
   lrn_filter_cfg.no_fields_action = WP_PCE_FILTER_NO_FIELDS_DENY;

   lrn_filter_cfg.no_match_result[0].result_type = WP_PCE_RESULT_LAST;

   lrn_filter_cfg.filter_fields[0].field_mode =
      WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   lrn_filter_cfg.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   lrn_filter_cfg.filter_fields[1].field_mode =
      WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   lrn_filter_cfg.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_USED;
   lrn_filter_cfg.filter_fields[1].mask.vlan_tag =
      WP_PCE_FIELD_MASK_VLAN_ID;

   lrn_filter_cfg.filter_fields[2].field_mode =
      WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   lrn_filter_cfg.filter_fields[2].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   lrn_filter_cfg.filter_fields[0].field_id = WP_PCE_FIELD_ID_MAC_SA;
   lrn_filter_cfg.filter_fields[1].field_id = WP_PCE_FIELD_ID_VLAN_TAG;
   lrn_filter_cfg.filter_fields[2].field_id = WP_PCE_FIELD_ID_LAST;

   PCE_filter[FILTER_SET_LEARNING] =
      WP_PceFilterCreate (WP_WINPATH (DEFAULT_WPID),
                          WP_PCE_FILTER_LEARNING, &lrn_filter_cfg);
   App_TerminateOnError (PCE_filter[FILTER_SET_LEARNING],
                         "WP_PceFilterCreate", __LINE__);
}

void WPE_CreatePceFilterSets (void)
{
   WP_pce_filter_set fs_level;

   /* filter set with learning */
   fs_level.filter_set_level = 0;
   fs_level.next_filter_set = WP_UNUSED;
   fs_level.filters[0] = PCE_filter[FILTER_SET_IPV6_IP_CLASSIFICATION];
   fs_level.filters[1] = PCE_filter[FILTER_SET_L4_SUBTYPE_CLASSIFICATION];
   fs_level.filters[2] = PCE_filter[FILTER_SET_L4_PORT_CLASSIFICATION];
   fs_level.filters[3] =
      PCE_filter[FILTER_SET_RESERVED_MAC_CLASSIFICATION];
   fs_level.filters[4] =
      PCE_filter[FILTER_SET_UNKNOWNUNICAST_CLASSIFICATION];
   fs_level.filters[5] = PCE_filter[FILTER_SET_CLASSIFICATION];
   fs_level.filters[6] = PCE_filter[FILTER_SET_LEARNING];
   fs_level.filters[7] = PCE_filter[FILTER_SET_FORWARDING];
   fs_level.filters[8] = WP_UNUSED;

   filter_set_lrn_en =
      WP_PceFilterSetCreate (WP_WINPATH (DEFAULT_WPID), &fs_level);
   App_TerminateOnError (filter_set_lrn_en, "WP_PceFilterSetCreate",
                         __LINE__);

   /* filter set without learning */
   fs_level.filter_set_level = 0;
   fs_level.next_filter_set = WP_UNUSED;
   fs_level.filters[0] = PCE_filter[FILTER_SET_IPV6_IP_CLASSIFICATION];
   fs_level.filters[1] = PCE_filter[FILTER_SET_L4_SUBTYPE_CLASSIFICATION];
   fs_level.filters[2] = PCE_filter[FILTER_SET_L4_PORT_CLASSIFICATION];
   fs_level.filters[3] =
      PCE_filter[FILTER_SET_RESERVED_MAC_CLASSIFICATION];
   fs_level.filters[4] =
      PCE_filter[FILTER_SET_UNKNOWNUNICAST_CLASSIFICATION];
   fs_level.filters[5] = PCE_filter[FILTER_SET_CLASSIFICATION];
   /*------------------------------------------------------------*\
               we remove learning filter here for disabling learning function.
   \*------------------------------------------------------------*/
   fs_level.filters[6] = PCE_filter[FILTER_SET_FORWARDING];
   fs_level.filters[7] = WP_UNUSED;

   filter_set_lrn_dis =
      WP_PceFilterSetCreate (WP_WINPATH (DEFAULT_WPID), &fs_level);
   App_TerminateOnError (filter_set_lrn_dis, "WP_PceFilterSetCreate",
                         __LINE__);
}

void WPE_CreatePceInterface (WP_handle iw_sys)
{
   WP_pce_if_params_pkt_rx_channel pce_if_params = { 0 };
   WP_handle pce_if_handle;
   WP_status status;
   WP_U32 ii;

   WP_rx_binding_bridging rx_binding_cfg[1] = {
      {
       /*  encap_mode */ 0,
       /*  mru; */ 512,
       /*  vcfcs; */ 0,
       /*  input_port; */ 0
       }
   };

   pce_if_params.mode = WP_PCE_IW_PORT_CONNECTION_ENABLED;
   pce_if_params.parser_start_type = WP_PCE_PARSER_START_TYPE_ETHERNET;
   pce_if_params.filter_set_handle = filter_set_lrn_en;  //filter_set_lrn_dis;
   pce_if_params.ip_header_validation = WP_DISABLE;
   pce_if_params.user_programmable_fields_handle = pce_upf_set;

   pce_if_handle =
      WP_PceInterfaceCreate (WP_PCE_IF_TYPE_PKT_RX_CHANNEL,
                             &pce_if_params);
   App_TerminateOnError (pce_if_handle, "WP_PceInterfaceCreate()",
                         __LINE__);

   /* Modify the PCE interface for GE RX channel handle */
   for (ii = 0; ii < NR_GBE; ii++)
   {
      rx_binding_cfg[0].pce_if_handle = pce_if_handle;
      status = WP_IwRxBindingModify (gbe[ii].rx_chan_enet,
                                     iw_sys,
                                     qniw,
                                     WP_IW_RX_BIND_MOD_PCE_INTERFACE,
                                     &rx_binding_cfg[0]);
      App_TerminateOnError (status, "WP_IwRxBindingModify", __LINE__);
   }
}

void WPE_CheckPceFilterStatsReset (void)
{
   WP_U32 i;

   memset (filter_stat, 0, sizeof (WP_pce_filter_stat) * 100);

   for (i = 0; i < FILTER_SET_LAST; i++)
      WP_PceFilterStatisticsReset (PCE_filter[i], &filter_stat[i]);
}

void WPE_Reboot (void)
{
#if !defined(__linux__)
   /* Rebooting is specific to board and target.  The test
    * directory should not be concerned how this is done.
    * Instead, it calls a board-specific function. */
   if (WPI_REBOOT_ENABLE)
   {
      printf ("Test Rebooting winmon by WT_Reboot\nWinMon>\n");
      WPX_Reboot ();
   }
   else
      printf ("Not Rebooting with WT_Reboot\nWinMon>\n");
#endif
}

void WPE_Bridge_Quit (WP_U32 status)
{
   if (status)
   {
      WPE_Reboot ();
   }

   demo_running = FALSE;
}

void WPT_DisplayBridgingPortStats (WP_handle port_handle, WP_CHAR * type)
{
   WP_U32 rx_packets = 0;
   WP_bport_stats bport_stats;

   memset (&bport_stats, 0, sizeof (WP_bport_stats));
   WP_IwBridgingStatistics (port_handle, &bport_stats);

   printf ("\n%s Bridge port statistics of port %d (%x)\n",
           type, (port_handle & 0x000000ff), port_handle);
   printf ("============================================\n");
   printf ("Rx Valid Packets:               %08x%08x\n",
           ((WPT_StatField *) & bport_stats.rx_valid_packets)->part.high,
           ((WPT_StatField *) & bport_stats.rx_valid_packets)->part.low);
   rx_packets =
      ((WPT_StatField *) & bport_stats.rx_valid_packets)->part.low;

   printf ("Discard VLAN acceptable filter: %08x%08x\n",
           ((WPT_StatField *) & bport_stats.
            discard_vlan_acceptable_filter)->part.high,
           ((WPT_StatField *) & bport_stats.
            discard_vlan_acceptable_filter)->part.low);
   printf ("Discard ingress filter:         %08x%08x\n",
           ((WPT_StatField *) & bport_stats.discard_ingress_filter)->part.
           high,
           ((WPT_StatField *) & bport_stats.discard_ingress_filter)->part.
           low);
   printf ("Discard bridge classifier:      %08x%08x\n",
           ((WPT_StatField *) & bport_stats.discard_bridge_classifier)->
           part.high,
           ((WPT_StatField *) & bport_stats.discard_bridge_classifier)->
           part.low);
   printf ("Discard unknown MAC SA:         %08x%08x\n",
           ((WPT_StatField *) & bport_stats.discard_unk_macsa)->part.high,
           ((WPT_StatField *) & bport_stats.discard_unk_macsa)->part.low);
   printf ("Deny MAC SA:                    %08x%08x\n",
           ((WPT_StatField *) & bport_stats.deny_mac_sa)->part.high,
           ((WPT_StatField *) & bport_stats.deny_mac_sa)->part.low);
   printf ("Deny MAC DA:                    %08x%08x\n",
           ((WPT_StatField *) & bport_stats.deny_mac_da)->part.high,
           ((WPT_StatField *) & bport_stats.deny_mac_da)->part.low);
   printf ("Rx BC valid packets:            %08x%08x\n",
           ((WPT_StatField *) & bport_stats.rx_bc_valid_packets)->part.
           high,
           ((WPT_StatField *) & bport_stats.rx_bc_valid_packets)->part.
           low);
   printf ("Rx MC valid packets:            %08x%08x\n",
           ((WPT_StatField *) & bport_stats.rx_mc_valid_packets)->part.
           high,
           ((WPT_StatField *) & bport_stats.rx_mc_valid_packets)->part.
           low);
   printf ("Forwarded UC packets:           %08x%08x\n",
           ((WPT_StatField *) & bport_stats.forwarded_uc_packets)->part.
           high,
           ((WPT_StatField *) & bport_stats.forwarded_uc_packets)->part.
           low);
   printf ("Forwarded BC packets:           %08x%08x\n",
           ((WPT_StatField *) & bport_stats.forwarded_bc_packets)->part.
           high,
           ((WPT_StatField *) & bport_stats.forwarded_bc_packets)->part.
           low);
   printf ("Forwarded MC packets:           %08x%08x\n",
           ((WPT_StatField *) & bport_stats.forwarded_mc_packets)->part.
           high,
           ((WPT_StatField *) & bport_stats.forwarded_mc_packets)->part.
           low);

   printf ("\n");
}

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
   printf ("\n----------------------------------------------------\n");
   printf ("Tx Bytes:                    %08x%08x\n",
           ((WPT_StatField *) & enet_stats.tx_bytes)->part.high,
           ((WPT_StatField *) & enet_stats.tx_bytes)->part.low);
   printf ("Tx Packets:                  %08x%08x\n",
           ((WPT_StatField *) & enet_stats.tx_packets)->part.high,
           ((WPT_StatField *) & enet_stats.tx_packets)->part.low);
   printf ("Tx Multicast:                %08x%08x\n",
           ((WPT_StatField *) & enet_stats.tx_multicast)->part.high,
           ((WPT_StatField *) & enet_stats.tx_multicast)->part.low);
   printf ("Tx Broadcast:                %08x%08x\n",
           ((WPT_StatField *) & enet_stats.tx_broadcast)->part.high,
           ((WPT_StatField *) & enet_stats.tx_broadcast)->part.low);
   printf ("Rx Bytes:                    %08x%08x\n",
           ((WPT_StatField *) & enet_stats.rx_bytes)->part.high,
           ((WPT_StatField *) & enet_stats.rx_bytes)->part.low);
   printf ("Rx Packets:                  %08x%08x\n",
           ((WPT_StatField *) & enet_stats.rx_packets)->part.high,
           ((WPT_StatField *) & enet_stats.rx_packets)->part.low);
   printf ("Rx Error FCS:                %08x%08x\n",
           ((WPT_StatField *) & enet_stats.rx_err_fcs)->part.high,
           ((WPT_StatField *) & enet_stats.rx_err_fcs)->part.low);
   printf ("Rx Multicast:                %08x%08x\n",
           ((WPT_StatField *) & enet_stats.rx_multicast)->part.high,
           ((WPT_StatField *) & enet_stats.rx_multicast)->part.low);
   printf ("Rx Broadcast:                %08x%08x\n",
           ((WPT_StatField *) & enet_stats.rx_broadcast)->part.high,
           ((WPT_StatField *) & enet_stats.rx_broadcast)->part.low);
   printf ("Tx Errors FCS:               %08x%08x\n",
           ((WPT_StatField *) & enet_stats.tx_err_fcs)->part.high,
           ((WPT_StatField *) & enet_stats.tx_err_fcs)->part.low);
   printf ("----------------------------------------------------\n");

   return 0;
}

void WPT_GenericFlowStatistics (WP_handle agg)
{
   WP_iw_generic_flow_stats iw_flow_stats;
   WP_U32 forward_packets = 0;
   WP_status status;

   memset (&iw_flow_stats, 0, sizeof (iw_flow_stats));
   status =
      WP_IwFlowStatistics (agg, WP_IW_FLOW_STAT_GENERIC, &iw_flow_stats);
   App_TerminateOnError (status, "WP_IwFlowStatistics", __LINE__);

   printf ("Aggregation flow statistics of flow %d (%x)\n",
           (agg & 0x000000ff), agg);
   printf ("=================================================\n");

   printf ("Forward Packet:          %08x%08x\n",
           ((WPT_StatField *) & iw_flow_stats.forward_packet)->part.high,
           ((WPT_StatField *) & iw_flow_stats.forward_packet)->part.low);
   forward_packets =
      ((WPT_StatField *) & iw_flow_stats.forward_packet)->part.low;

   printf ("Group filtered packets   %08x%08x\n",
           ((WPT_StatField *) & iw_flow_stats.group_filtered_packets)->
           part.high,
           ((WPT_StatField *) & iw_flow_stats.group_filtered_packets)->
           part.low);

   printf ("MTU Drop Packets:        %08x%08x\n",
           ((WPT_StatField *) & iw_flow_stats.mtu_drop_packets)->part.high,
           ((WPT_StatField *) & iw_flow_stats.mtu_drop_packets)->part.low);

   printf ("pecs counter1:           %08x%08x\n",
           ((WPT_StatField *) & iw_flow_stats.pecs_counter_1)->part.high,
           ((WPT_StatField *) & iw_flow_stats.pecs_counter_1)->part.low);

   printf ("TX Queue Drop Packets:   %08x%08x\n",
           ((WPT_StatField *) & iw_flow_stats.tx_queue_drop_packets)->part.
           high,
           ((WPT_StatField *) & iw_flow_stats.tx_queue_drop_packets)->part.
           low);

   printf ("Denied Packets:          %08x%08x\n",
           ((WPT_StatField *) & iw_flow_stats.denied_packets)->part.high,
           ((WPT_StatField *) & iw_flow_stats.denied_packets)->part.low);

   printf ("Group filtered packets:  %08x%08x\n",
           ((WPT_StatField *) & iw_flow_stats.group_filtered_packets)->
           part.high,
           ((WPT_StatField *) & iw_flow_stats.group_filtered_packets)->
           part.low);

   printf ("forwarded_bytes:         %08x%08x\n",
           ((WPT_StatField *) & iw_flow_stats.forwarded_bytes)->part.high,
           ((WPT_StatField *) & iw_flow_stats.forwarded_bytes)->part.low);

   printf ("Policer Non Conforming:  %08x%08x\n",
           ((WPT_StatField *) & iw_flow_stats.
            policer_non_conforming_packets)->part.high,
           ((WPT_StatField *) & iw_flow_stats.
            policer_non_conforming_packets)->part.low);

}

void WPT_IwBridgingPortStatsReset (WP_handle port_handle)
{
   WP_bport_stats bport_stats;
   WP_status status;

   if (!port_handle)
      return;

   status = WP_IwPortStatisticsReset (port_handle, &bport_stats);
   App_TerminateOnError (status, "WP_IwPortStatisticsReset()", __LINE__);
}

void WPT_LocalDeviceStatsReset (WP_handle enet_handle)
{
   WP_stats_enet enet_stats;
   WP_status status;

   status = WP_DeviceStatisticsReset (enet_handle, &enet_stats);
   App_TerminateOnError (status, "WP_DeviceStatisticsReset()", __LINE__);
}

void WPT_GenericFlowStatisticsReset (WP_handle agg)
{
   WP_iw_flow_stats iw_flow_stats;
   WP_status status;

   status =
      WP_IwFlowStatisticsReset (agg, WP_IW_FLOW_STAT_GENERIC,
                                &iw_flow_stats);
   App_TerminateOnError (status, "WP_IwFlowStatisticsReset()", __LINE__);
}

void WPE_DumpFwdPCERules (void)
{
   WP_handle rule_handle;
   static WP_handle dump_iterator_handle = 0;

   if (!dump_iterator_handle)
   {
      dump_iterator_handle =
         WP_PceIteratorCreate (WP_WINPATH (DEFAULT_WPID),
                               WP_PCE_ITERATOR_TYPE_RULE,
                               (void *)
                               &PCE_filter[FILTER_SET_FORWARDING]);
      App_TerminateOnError (dump_iterator_handle, "WP_PceIteratorCreate",
                            __LINE__);
   }

   rule_handle = WP_PceIteratorGetFirst (dump_iterator_handle);
   if (!WP_ERROR_P (rule_handle))
   {
      while (1)
      {
         WP_Display (WP_WINPATH (0), rule_handle, 0, 0);
         rule_handle = WP_PceIteratorGetNext (dump_iterator_handle);
         if (WP_ERROR_P (rule_handle))
         {
            if (WP_ERROR (rule_handle) != WP_ERR_PCE_ITERATOR_NO_ITEMS)
               printf ("\n\nerror %s \n", WP_ErrorString (rule_handle));
            break;
         }

      }
   }
   else if (WP_ERROR (rule_handle) != WP_ERR_PCE_ITERATOR_NO_ITEMS)
      printf ("\n\nerror %s \n", WP_ErrorString (rule_handle));

   return;
}

/*************************************unknown & multicast*****************************/

WPE_mc_group *mc_groups = NULL;
WPE_vlan_group *vlan_groups = NULL;

extern APP_gbe_enet_t gbe[NR_GBE];

WP_iw_mc_member bc_member_config[1] = {
   {
    /* txfunc */ 0,
    /* service_class_index */ 0,
    /* buffers_threshold */ 0,
    /* iw_port */ 0,
    /* edit_size */ 0x20,
    /* data_copy_mode */ WP_IW_MC_MEMBER_DATACOPY_DISABLE,
    /*iw_port_check_mode */ WP_IW_MC_MEMBER_IWPORT_CHECK_ENABLE
    }
};

WP_iw_mc_group bc_group_config[1] = {
   {
    /* type */ WP_IW_MC_UNRESTRICTED,
    /* mode */
    WP_IW_MC_MODE_MEMBERS_STAT_ENABLE |
    WP_IW_MC_MODE_MEMBERS_ENHANCED_STAT_ENABLE |
    WP_IW_MC_MODE_GENERIC_GROUP,
    /* max_iw_mc_members */ IW_SYS_MAX_MC_MEMBERS,
    /* qnode */ 0,
    /* encaps_gap */ 64,
    /* classes_config */ NULL,
    /* identifier */ WP_UNUSED
    }
};

WP_iw_agg_multicast bc_agg_config[1] = {
   {
    /* tag */ 0,
    /* type */ WP_IW_FLOW_AGG_PRIMARY,
    /* mc_group */ 0,
    /* mtu */ 1536,
    /* intmode */ WP_IW_INT_DISABLE,
    /* interruptqueue */ WP_IW_IRQT1,
    /* statmode */ WP_IW_STAT_ENABLE,
    /* timestamp_mode */ WP_IW_TIME_STAMP_DISABLE,
    /* ov_pool_mode */ WP_IW_OV_POOL_ENABLE,
    /* fbp_drop_threshold */ 0,
    /* policer_enable */ WP_IW_POLICER_DISABLE,
    /* *policer_config */ NULL
    }
};

WP_bridge_port mc_dummy_bport_config[1] = {
   {
    /* tag */ 0xFFFF,
    /* direct_mapping */ WP_IW_DIRECT_MAP_ENABLE,
    /* flow_agg */ 0,
    /* flooding_term_mode */ WP_IW_HOST_TERM_MODE,
    /* learning_mode */ WP_IW_LEARNING_DISABLE,
    /* in_filter_mode */ WP_IW_INGRESS_FILTER_DISABLE,
    /* vlan_param */
    {
     /* vlan_acceptance_mode */ 0,
     /* vlan_tag */ 0,
     },
    /* max_mac_addresses */ 50,
    /* group_tag */ 0,
    /* group_filtering_mode */ WP_IW_GROUP_FILTER_DISABLE,
    /* unk_mac_sa_filter */ 0,
    /* unk_mc_mode */ WP_IW_UNK_MC_HT,
    /* bc_ht_mode */ WP_IW_BC_HT_DISABLE,
    /* input_filters_mask */ 0,
    /* output_filters_mask */ 0,
    /* statmode */ WP_IW_PORT_STAT_ENABLE
    }
};

extern APP_gbe_enet_t gbe[NR_GBE];
extern WP_handle dl_general_iwsys_bridge;

/******************************* unknown unicast group API *************************/

WP_U32 WPE_VlanInit (void)
{
   vlan_groups = malloc (NUM_OF_VLAN_GROUPS * sizeof (WPE_vlan_group));
   if (vlan_groups == NULL)
   {
      printf ("mc_groups malloc fails\n");
      return WPE_ERROR;
   }
   memset (vlan_groups, 0, NUM_OF_VLAN_GROUPS * sizeof (WPE_vlan_group));
   return 0;
}

static void WPE_CreateUnknownUnicastPceRules (WP_U16 vlan,
                                              WPE_vlan_group * groupInfo)
{
   WP_pce_rule_classification rule_cfg = { 0 };

   WP_handle h_PCE_rule;

   /* Common parameters */
   rule_cfg.enabled = WP_ENABLE;

   rule_cfg.filter_handle =
      PCE_filter[FILTER_SET_UNKNOWNUNICAST_CLASSIFICATION];

   rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_VLAN_TAG;

   rule_cfg.rule_fields[0].value.vlan_tag = vlan;

   rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_LAST;

   rule_cfg.match_action = WP_PCE_RULE_MATCH_CONTINUE;

   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
   rule_cfg.match_result[0].param.flow_agg.flow_aggregation =
      groupInfo->agg_handle;

   rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;

   h_PCE_rule = WP_PceRuleCreate (WP_WINPATH (DEFAULT_WPID),
                                  WP_PCE_RULE_CLASSIFICATION, &rule_cfg);

   if (!WP_ERROR_P (h_PCE_rule))
   {
      groupInfo->PCE_rule_handle = h_PCE_rule;
   }
   else if (WP_ERROR (h_PCE_rule) == WP_ERR_PCE_RULE_ALREADY_EXISTS)
   {
      printf ("PCE rule for vlan %d already exist!\n", vlan);
   }
   else
   {
      App_TerminateOnError (h_PCE_rule, "WP_PceRuleCreate", __LINE__);
   }

   return;
}

void WPE_CreateUnknownUnicastGroup (WP_U16 vlan, WP_U32 port)
{
   WP_U32 i = 0;

   bc_group_config[0].qnode = qniw_mc;
   for (i = 0; i < NUM_OF_VLAN_GROUPS; i++)  //check if vlan flooding group exists
   {
      if ((vlan_groups[i].vlan == vlan) && (vlan_groups[i].valid == 1))
      {
         //printf("Vlan group already been created!\n");
         if ( /*(port >= 0)&& */ (port < NR_GBE))
         {

            bc_member_config[0].txfunc = gbe[port].agg_enet;   //enet tx flow agg
            bc_member_config[0].iw_port = gbe[port].bport_enet;   //output iw port connected to the flow agg 
            vlan_groups[i].member_handle[port] =
               WP_IwMcMemberAdd (vlan_groups[i].group_handle,
                                 bc_member_config);
            App_TerminateOnError (vlan_groups[i].member_handle[port],
                                  "WP_IwMcMemberAdd() bc member",
                                  __LINE__);
            vlan_groups[i].count++;
         }
         else
         {
            printf ("include invalid ports!!!\n");
         }
         return;
      }
   }
   for (i = 0; i < NUM_OF_VLAN_GROUPS; i++)  //find a free entry
   {
      if (vlan_groups[i].valid == 0)
         break;
   }

   if (i == NUM_OF_VLAN_GROUPS)
   {
      printf ("No free vlan group\n");
      return;
   }

   vlan_groups[i].group_handle =
      WP_IwMcGroupCreate (WP_WINPATH (DEFAULT_WPID), bc_group_config);
   App_TerminateOnError (vlan_groups[i].group_handle,
                         "WP_IwMcGroupCreate() bc group", __LINE__);

   /* Create broadcast flow agg using the bc group handle. */
   bc_agg_config[0].mc_group = vlan_groups[i].group_handle;
   vlan_groups[i].agg_handle =
      WP_IwFlowAggregationCreate (WP_WINPATH (DEFAULT_WPID),
                                  WP_IW_GENERIC_MC_MODE, bc_agg_config);
   App_TerminateOnError (vlan_groups[i].agg_handle,
                         "WP_IwFlowAggregationCreate() bc agg", __LINE__);

   /* Create Members for the bc Group (a member per each bport) */

   if ( /*(port >= 0)&& */ (port < NR_GBE))
   {

      bc_member_config[0].txfunc = gbe[port].agg_enet;   //enet tx flow agg
      bc_member_config[0].iw_port = gbe[port].bport_enet;   //output iw port connected to the flow agg 
      vlan_groups[i].member_handle[port] =
         WP_IwMcMemberAdd (vlan_groups[i].group_handle, bc_member_config);
      App_TerminateOnError (vlan_groups[i].member_handle[port],
                            "WP_IwMcMemberAdd() bc member", __LINE__);
      vlan_groups[i].count++;
   }
   else
   {
      printf ("include invalid ports!!!\n");
      return;
   }

   /* Create Dummy Bridge Port for the Multicast Group
      (until pre defined MC Bport will be implemented) */
   mc_dummy_bport_config[0].flow_agg = vlan_groups[i].agg_handle;
   vlan_groups[i].dum_bport_handle =
      WP_IwBridgingPortCreate (dl_general_iwsys_bridge,
                               &mc_dummy_bport_config[0]);
   App_TerminateOnError (vlan_groups[i].dum_bport_handle,
                         "WP_IwBridgingPortCreate() dummy bport",
                         __LINE__);

   WPE_CreateUnknownUnicastPceRules (vlan, &vlan_groups[i]);
   vlan_groups[i].valid = 1;
   vlan_groups[i].vlan = vlan;
   return;
}

void WPE_DeleteUnkonwnUnicastGroup (WP_U16 vlan, WP_U32 port)
{
   WP_U32 i = 0;
   WP_handle status;

   for (i = 0; i < NUM_OF_VLAN_GROUPS; i++)
   {
      if ((vlan_groups[i].vlan == vlan) && (vlan_groups[i].valid == 1))
      {
         if (vlan_groups[i].count == 0)
         {
            status = WP_PceRuleDelete (vlan_groups[i].PCE_rule_handle);
            App_TerminateOnError (status, "WP_PceRuleDelete", __LINE__);
            status =
               WP_IwBridgingPortDelete (dl_general_iwsys_bridge,
                                        vlan_groups[i].dum_bport_handle);
            App_TerminateOnError (status, "WP_IwBridgingPortDelete",
                                  __LINE__);
            status =
               WP_IwFlowAggregationDelete (vlan_groups[i].agg_handle);
            App_TerminateOnError (status, "WP_IwFlowAggregationDelete",
                                  __LINE__);
            status = WP_IwMcGroupDelete (vlan_groups[i].group_handle);
            App_TerminateOnError (status, "WP_IwMcGroupDelete", __LINE__);
            memset (&vlan_groups[i], 0, sizeof (WPE_vlan_group));

         }
         else
         {
            status =
               WP_IwMcMemberDisable (vlan_groups[i].group_handle,
                                     vlan_groups[i].member_handle[port]);
            App_TerminateOnError (status, "WP_IwMcMemberDisable",
                                  __LINE__);
            status =
               WP_IwMcMemberDelete (vlan_groups[i].group_handle,
                                    vlan_groups[i].member_handle[port]);
            App_TerminateOnError (status, "WP_IwMcMemberDelete", __LINE__);
            vlan_groups[i].member_handle[port] = 0;
            vlan_groups[i].count--;
         }

         break;
      }
   }
   return;
}

/******************************* Multicast group API *************************/

WP_U32 WPE_MulticastInit (void)
{
   mc_groups = malloc (NUM_OF_MC_GROUPS * sizeof (WPE_mc_group));
   if (mc_groups == NULL)
   {
      printf ("mc_groups malloc fails\n");
      return WPE_ERROR;
   }
   memset (mc_groups, 0, NUM_OF_MC_GROUPS * sizeof (WPE_mc_group));

   return 0;
}

static void WPE_CreateMulticastPceRules (unsigned char *mc_mac,
                                         WP_U16 vlan,
                                         WPE_mc_group * groupInfo)
{
   WP_pce_rule_forwarding rule_cfg = { 0 };
   WP_U32 ii;
   WP_handle h_PCE_rule;

   /* Common parameters */
   rule_cfg.enabled = WP_ENABLE;

   rule_cfg.filter_handle = PCE_filter[FILTER_SET_FORWARDING];

   rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_MAC_DA;
   for (ii = 0; ii < 6; ii++)
      rule_cfg.rule_fields[0].value.mac_addr[ii] = mc_mac[ii];

   rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_VLAN_TAG;
   rule_cfg.rule_fields[1].value.vlan_tag = vlan;

   rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_LAST;

   rule_cfg.match_action = WP_PCE_RULE_MATCH_CONTINUE;

   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
   rule_cfg.match_result[0].param.flow_agg.flow_aggregation =
      groupInfo->agg_handle;

   rule_cfg.match_result[1].result_type = WP_PCE_RESULT_OUT_IW_PORT_UPDATE;
   rule_cfg.match_result[1].param.iw_port.iw_port_handle =
      groupInfo->dum_bport_handle;

   rule_cfg.match_result[2].result_type = WP_PCE_RESULT_AGING;
   rule_cfg.match_result[2].param.aging.mode = WP_PCE_AGING_DISABLE;

   rule_cfg.match_result[3].result_type = WP_PCE_RESULT_LAST;

   h_PCE_rule = WP_PceRuleCreate (WP_WINPATH (DEFAULT_WPID),
                                  WP_PCE_RULE_FORWARDING, &rule_cfg);
   App_TerminateOnError (h_PCE_rule, "WP_PceRuleCreate", __LINE__);
   groupInfo->PCE_rule_handle = h_PCE_rule;
   memcpy (groupInfo->mc_mac, mc_mac, 6);

   return;
}

void WPE_CreateMulticastGroup (unsigned char *mc_mac, WP_U16 vlan)
{
   WP_U32 i = 0;

   bc_group_config[0].qnode = qniw_mc;
   for (i = 0; i < NUM_OF_MC_GROUPS; i++) //check if mc exists
   {
      if ((mc_groups[i].vlan == vlan) && (mc_groups[i].valid == 1)
          && (!memcmp (mc_mac, mc_groups[i].mc_mac, 6)))
      {
         printf ("MC group already been created!\n");
         return;
      }
   }
   /*mc group is not existed */
   for (i = 0; i < NUM_OF_MC_GROUPS; i++) //find a free entry
   {
      if (mc_groups[i].valid == 0)
         break;
   }
   mc_groups[i].group_handle =
      WP_IwMcGroupCreate (WP_WINPATH (DEFAULT_WPID), bc_group_config);
   App_TerminateOnError (mc_groups[i].group_handle,
                         "WP_IwMcGroupCreate() bc group", __LINE__);

   /* Create broadcast flow agg using the bc group handle. */
   bc_agg_config[0].mc_group = mc_groups[i].group_handle;
   mc_groups[i].agg_handle =
      WP_IwFlowAggregationCreate (WP_WINPATH (DEFAULT_WPID),
                                  WP_IW_GENERIC_MC_MODE, bc_agg_config);
   App_TerminateOnError (mc_groups[i].agg_handle,
                         "WP_IwFlowAggregationCreate() bc agg", __LINE__);

   /* Create Dummy Bridge Port for the Multicast Group
      (until pre defined MC Bport will be implemented) */
   mc_dummy_bport_config[0].flow_agg = mc_groups[i].agg_handle;
   mc_groups[i].dum_bport_handle =
      WP_IwBridgingPortCreate (dl_general_iwsys_bridge,
                               &mc_dummy_bport_config[0]);
   App_TerminateOnError (mc_groups[i].dum_bport_handle,
                         "WP_IwBridgingPortCreate() dummy bport",
                         __LINE__);

   WPE_CreateMulticastPceRules (mc_mac, vlan, &mc_groups[i]);
   mc_groups[i].valid = 1;
   mc_groups[i].vlan = vlan;
   memcpy (mc_groups[i].mc_mac, mc_mac, 6);

   return;
}

void WPE_AddMulticastMember (unsigned char *mc_mac, WP_U16 vlan,
                             WP_U32 port)
{
   WP_U32 i = 0;

   for (i = 0; i < NUM_OF_MC_GROUPS; i++) //check if mc exists
   {
      if ((mc_groups[i].vlan == vlan) && (mc_groups[i].valid == 1)
          && (!memcmp (mc_mac, mc_groups[i].mc_mac, 6)))
      {
         if ( /*(port >= 0)&& */ (port < NR_GBE))
         {

            bc_member_config[0].txfunc = gbe[port].agg_enet;   //enet tx flow agg
            bc_member_config[0].iw_port = gbe[port].bport_enet;   //output iw port connected to the flow agg 
            mc_groups[i].member_handle[port] =
               WP_IwMcMemberAdd (mc_groups[i].group_handle,
                                 bc_member_config);
            App_TerminateOnError (mc_groups[i].member_handle[port],
                                  "WP_IwMcMemberAdd() bc member",
                                  __LINE__);
            mc_groups[i].count++;
         }
         else
         {
            printf ("include invalid ports!!!\n");
         }
         //if multicast group exist, then add member and exist
         return;
      }
   }

   //if come here, it means multicast group does NOT exist
   printf
      ("Multicast group does NOT exist! Please create multicast group first!\n");
   return;

}

void WPE_DeleteMulticastGroup (unsigned char *mc_mac, WP_U16 vlan)
{
   WP_U32 i = 0;
   WP_handle status;

   for (i = 0; i < NUM_OF_MC_GROUPS; i++)
   {
      if ((mc_groups[i].vlan == vlan) && (mc_groups[i].valid == 1)
          && (!memcmp (mc_mac, mc_groups[i].mc_mac, 6)))
      {
         if (mc_groups[i].count == 0)
         {
            status = WP_PceRuleDelete (mc_groups[i].PCE_rule_handle);
            App_TerminateOnError (status, "WP_PceRuleDelete", __LINE__);
            status =
               WP_IwBridgingPortDelete (dl_general_iwsys_bridge,
                                        mc_groups[i].dum_bport_handle);
            App_TerminateOnError (status, "WP_IwBridgingPortDelete",
                                  __LINE__);

            status = WP_IwFlowAggregationDelete (mc_groups[i].agg_handle);
            App_TerminateOnError (status, "WP_IwFlowAggregationDelete",
                                  __LINE__);
            status = WP_IwMcGroupDelete (mc_groups[i].group_handle);
            App_TerminateOnError (status, "WP_IwMcGroupDelete", __LINE__);
            memset (&mc_groups[i], 0, sizeof (WPE_mc_group));

         }
         break;
      }
   }
   return;
}

void WPE_RemoveMulticastMember (unsigned char *mc_mac, WP_U16 vlan,
                                WP_U32 port)
{
   WP_U32 i = 0;
   WP_handle status;

   for (i = 0; i < NUM_OF_MC_GROUPS; i++)
   {
      if ((mc_groups[i].vlan == vlan) && (mc_groups[i].valid == 1)
          && (!memcmp (mc_mac, mc_groups[i].mc_mac, 6)))
      {
         if (mc_groups[i].count != 0)
         {
            status =
               WP_IwMcMemberDisable (mc_groups[i].group_handle,
                                     mc_groups[i].member_handle[port]);
            App_TerminateOnError (status, "WP_IwMcMemberDisable",
                                  __LINE__);
            status =
               WP_IwMcMemberDelete (mc_groups[i].group_handle,
                                    mc_groups[i].member_handle[port]);
            App_TerminateOnError (status, "WP_IwMcMemberDelete", __LINE__);

            mc_groups[i].member_handle[port] = 0;

            mc_groups[i].count--;
         }
         break;
      }
   }
   return;
}

/****************************************fdb function**************************/

/*****mac operate**********/

typedef struct FdbEntry
{
   unsigned char mac[6];
   WP_U16 vlan;
   WP_U16 port;
   WP_handle rule_handle;
   struct FdbEntry *next;
} Node;

Node *Fdbhead = NULL;

void WPE_PrintFDBList (void)
{
   Node *current = NULL;

   current = Fdbhead;

   printf ("\n FDB List are:");
   if (NULL == current)
      printf ("\nLinked list is not exsit!\n");

   while (NULL != current)
   {
      printf
         ("\n  Vlan : %d, MAC : %02x-%02x-%02x-%02x-%02x-%02x, Port : %02d, Handle : %#x",
          current->vlan, current->mac[0], current->mac[1], current->mac[2],
          current->mac[3], current->mac[4], current->mac[5], current->port,
          current->rule_handle);

      current = current->next;
   }
   printf ("\n\n ");

   return;
}

/**create node*/
Node *WPE_CreateFDBList (unsigned char *mac, WP_U16 vlan, WP_U16 port,
                         WP_handle rule_handle)
{
   Node *temp = NULL;

   printf ("come in WPE_CreateFDBList!!\n");
   temp = (Node *) malloc (sizeof (Node));
   if (temp == NULL)
   {
      printf ("malloc failed!\n");
      return Fdbhead;
   }
   printf ("come to WPE_CreateFDBList!!\n");
   memcpy (temp->mac, mac, 6);
   temp->vlan = vlan;
   temp->port = port;
   temp->rule_handle = rule_handle;
   temp->next = NULL;
   Fdbhead = temp;
   printf ("create ok!\n");
   return Fdbhead;
}

/*insert node*/

Node *WPE_InsertFDBNode (unsigned char *mac, WP_U16 vlan, WP_U16 port,
                         WP_handle rule_handle)
{
   Node *current = Fdbhead->next;
   Node *prev = Fdbhead;
   Node *temp = NULL;

   if (NULL == Fdbhead)
      return Fdbhead;

   while (current)
   {
      prev = current;
      current = current->next;
   }

   temp = (Node *) malloc (sizeof (Node));
   if (temp == NULL)
   {
      printf ("malloc failed!\n");
      return Fdbhead;
   }
   memcpy (temp->mac, mac, 6);
   temp->vlan = vlan;
   temp->port = port;
   temp->rule_handle = rule_handle;
   temp->next = NULL;
   prev->next = temp;

   return Fdbhead;
}

Node *WPE_AddFDBNode (WP_pce_rule_forwarding * rule, WP_handle rule_handle)
{
   WP_U16 i, port = 255;
   unsigned char *mac = rule->rule_fields[0].value.mac_addr;
   WP_U16 vlan = rule->rule_fields[1].value.vlan_tag;
   WP_handle handle = rule->match_result[0].param.iw_port.iw_port_handle;

   for (i = 0; i < NR_GBE; i++)
   {
      if (gbe[i].bport_enet == handle)
      {
         port = i;
         break;
      }
   }

   if (Fdbhead == NULL)
      return WPE_CreateFDBList (mac, vlan, port, rule_handle);
   else
      return WPE_InsertFDBNode (mac, vlan, port, rule_handle);
}

/*delete node by vlan*/
Node *WPE_DeleteFDBNodeByVlan (WP_U16 vlan)
{
   WP_status status;
   Node *prev, *current, *temp;

   if (NULL == Fdbhead)
      return Fdbhead;

   current = Fdbhead;
   prev = Fdbhead;

   while (current)
   {
      if (current->vlan == vlan)
      {
         status = WP_PceRuleDelete (current->rule_handle);
         App_TerminateOnError (status, "WP_PceRuleDelete", __LINE__);
         if (current == Fdbhead)
         {
            Fdbhead = current->next;
            prev = Fdbhead;
         }
         else
         {
            prev->next = current->next;

         }
         temp = current;
         current = current->next;
         free (temp);

         continue;
      }
      else
      {
         prev = current;
         current = current->next;
      }
   }

   return Fdbhead;

}

/*delete node by handle*/
Node *WPE_DeleteFDBNodeByHandle (WP_handle rule_handle)
{
   Node *prev, *current;

   if (NULL == Fdbhead)
      return Fdbhead;

   prev = Fdbhead;
   current = Fdbhead->next;

   if (prev->rule_handle == rule_handle)
   {
      free (Fdbhead);
      Fdbhead = current;
      return Fdbhead;
   }

   while (current)
   {
      if (current->rule_handle == rule_handle)
      {
         prev->next = current->next;
         free (current);
         return Fdbhead;
      }
      prev = current;
      current = current->next;
   }

   return Fdbhead;
}

void WPE_AddFDBEntry (WP_pce_rule_forwarding * rule, WP_handle rule_handle)
{
   WPE_AddFDBNode (rule, rule_handle);
   //WPE_PrintFDBList();
   return;

}

void WPE_DelFDBEntry (WP_handle rule_handle)
{
   WPE_DeleteFDBNodeByHandle (rule_handle);
   //WPE_PrintFDBList();
   return;
}

/*delete all PCE rules related with this vlan*/
void WPE_DeleteVlanPCERules (WP_U16 vlan)
{

   WPE_DeleteFDBNodeByVlan (vlan);

   return;
}

/*************************************dump PCE rules function*********************************/
static void WPE_DumpUnknownUnicastPCERules (void)
{
   WP_handle rule_handle;
   WP_handle dump_iterator_handle;

   dump_iterator_handle = WP_PceIteratorCreate (WP_WINPATH (DEFAULT_WPID),
                                                WP_PCE_ITERATOR_TYPE_RULE,
                                                (void *)
                                                &PCE_filter
                                                [FILTER_SET_UNKNOWNUNICAST_CLASSIFICATION]);
   App_TerminateOnError (dump_iterator_handle, "WP_PceIteratorCreate",
                         __LINE__);

   rule_handle = WP_PceIteratorGetFirst (dump_iterator_handle);
   if (!WP_ERROR_P (rule_handle))
   {
      while (1)
      {
         WP_Display (WP_WINPATH (0), rule_handle, 0, 0);

         rule_handle = WP_PceIteratorGetNext (dump_iterator_handle);
         if (WP_ERROR_P (rule_handle))
         {
            if (WP_ERROR (rule_handle) != WP_ERR_PCE_ITERATOR_NO_ITEMS)
               printf ("\n\nerror %s \n", WP_ErrorString (rule_handle));
            break;
         }

      }
   }
   else if (WP_ERROR (rule_handle) != WP_ERR_PCE_ITERATOR_NO_ITEMS)
      printf ("\n\nerror %s \n", WP_ErrorString (rule_handle));

   WP_PceIteratorDelete (dump_iterator_handle);
   return;
}

static void WPE_DumpMulticastPCERules (void)
{
   WP_U32 i;

   for (i = 0; i < NUM_OF_MC_GROUPS; i++) //check if mc exists
   {
      if (mc_groups[i].valid)
      {
         WP_Display (WP_WINPATH (0), mc_groups[i].PCE_rule_handle, 0, 0);
         return;
      }
   }

   return;
}

static void WPE_DumpForwardingPCERules (void)
{
   WP_handle rule_handle;
   WP_handle dump_iterator_handle;

   dump_iterator_handle = WP_PceIteratorCreate (WP_WINPATH (DEFAULT_WPID),
                                                WP_PCE_ITERATOR_TYPE_RULE,
                                                (void *)
                                                &PCE_filter
                                                [FILTER_SET_FORWARDING]);
   App_TerminateOnError (dump_iterator_handle, "WP_PceIteratorCreate",
                         __LINE__);

   rule_handle = WP_PceIteratorGetFirst (dump_iterator_handle);
   if (!WP_ERROR_P (rule_handle))
   {
      while (1)
      {
         WP_Display (WP_WINPATH (0), rule_handle, 0, 0);
         rule_handle = WP_PceIteratorGetNext (dump_iterator_handle);
         if (WP_ERROR_P (rule_handle))
         {
            if (WP_ERROR (rule_handle) != WP_ERR_PCE_ITERATOR_NO_ITEMS)
               printf ("\n\nerror %s \n", WP_ErrorString (rule_handle));
            break;
         }

      }
   }
   else if (WP_ERROR (rule_handle) != WP_ERR_PCE_ITERATOR_NO_ITEMS)
      printf ("\n\nerror %s \n", WP_ErrorString (rule_handle));

   WP_PceIteratorDelete (dump_iterator_handle);
   return;
}

static void WPE_DumpLearningPCERules (void)
{
   WP_handle rule_handle;
   WP_handle dump_iterator_handle;

   dump_iterator_handle = WP_PceIteratorCreate (WP_WINPATH (DEFAULT_WPID),
                                                WP_PCE_ITERATOR_TYPE_RULE,
                                                (void *)
                                                &PCE_filter
                                                [FILTER_SET_LEARNING]);
   App_TerminateOnError (dump_iterator_handle, "WP_PceIteratorCreate",
                         __LINE__);

   rule_handle = WP_PceIteratorGetFirst (dump_iterator_handle);
   if (!WP_ERROR_P (rule_handle))
   {
      while (1)
      {
         WP_Display (WP_WINPATH (0), rule_handle, 0, 0);
         rule_handle = WP_PceIteratorGetNext (dump_iterator_handle);
         if (WP_ERROR_P (rule_handle))
         {
            if (WP_ERROR (rule_handle) != WP_ERR_PCE_ITERATOR_NO_ITEMS)
               printf ("\n\nerror %s \n", WP_ErrorString (rule_handle));
            break;
         }

      }
   }
   else if (WP_ERROR (rule_handle) != WP_ERR_PCE_ITERATOR_NO_ITEMS)
      printf ("\n\nerror %s \n", WP_ErrorString (rule_handle));

   WP_PceIteratorDelete (dump_iterator_handle);
   return;
}

void WPE_DumpAllPCERules (void)
{
   WPE_DumpUnknownUnicastPCERules ();
   WPE_DumpMulticastPCERules ();
   WPE_DumpLearningPCERules ();
   WPE_DumpForwardingPCERules ();
   return;
}

/********************************************************************************/

WPE_lrn_flwagg_rule lrn_flwagg_rules[MAX_LEARNING_FLOWAGG_RULES];

static int find_first_unused (void)
{
   int cur = 0;

   do
   {
      if (!lrn_flwagg_rules[cur].valid)
         return cur;
   }
   while (++cur < MAX_LEARNING_FLOWAGG_RULES);

   return -1;
}

void WPE_DisableEnablePortLearning (WP_U32 portid, WP_U8 en)
{
   WP_pce_if_params_pkt_rx_channel pce_if_params = { 0 };
   WP_handle pce_if_handle;
   WP_status status;

   WP_rx_binding_bridging rx_binding_cfg[1] = {
      {
       /*  encap_mode */ 0,
       /*  mru; */ 512,
       /*  vcfcs; */ 0,
       /*  input_port; */ 0
       }
   };

   if (en)
      pce_if_params.filter_set_handle = filter_set_lrn_en;  // filter set for learning enable
   else
      pce_if_params.filter_set_handle = filter_set_lrn_dis; // filter set for learning disable

   pce_if_params.mode = WP_PCE_IW_PORT_CONNECTION_ENABLED;
   pce_if_params.parser_start_type = WP_PCE_PARSER_START_TYPE_ETHERNET;
   pce_if_params.ip_header_validation = WP_DISABLE;

   pce_if_handle =
      WP_PceInterfaceCreate (WP_PCE_IF_TYPE_PKT_RX_CHANNEL,
                             &pce_if_params);
   App_TerminateOnError (pce_if_handle, "WP_PceInterfaceCreate()",
                         __LINE__);

   switch (portid)
   {
   case 0:
   case 1:
      rx_binding_cfg[0].pce_if_handle = pce_if_handle;
      status = WP_IwRxBindingModify (gbe[portid].rx_chan_enet,
                                     dl_general_iwsys_bridge,
                                     qniw,
                                     WP_IW_RX_BIND_MOD_PCE_INTERFACE,
                                     &rx_binding_cfg[0]);
      App_TerminateOnError (status, "WP_IwRxBindingModify", __LINE__);
      break;

   default:
      App_TerminateOnError (1, "NO such port", __LINE__);
   }

   return;
}

void WPE_CreateLearningFlowAggPceRule (WP_U32 portid, WP_U32 vid)
{
   WP_pce_rule_classification rule_cfg = { 0 };
   WP_handle port_handle = 0, agg_handle = 0, h_PCE_rule = 0;
   int cur;

   if ((cur = find_first_unused ()) < 0)
   {
      printf ("NO memory for new rule, please check.");
      return;
   }

   switch (portid)
   {
   case 0:
   case 1:
      port_handle = gbe[portid].bport_enet;
      agg_handle = gbe[portid].agg_enet;
      break;
   default:
      printf ("NO such port : %d\n", portid);
      return;
   }

   rule_cfg.enabled = WP_ENABLE;

   rule_cfg.filter_handle = PCE_filter[FILTER_SET_CLASSIFICATION];

   rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_INPUT_IW_PORT;
   rule_cfg.rule_fields[0].value.iw_port_handle = port_handle;

   rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_VLAN_TAG;
   rule_cfg.rule_fields[1].value.vlan_tag = vid;

   rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_LAST;

   rule_cfg.match_action = WP_PCE_RULE_MATCH_CONTINUE;

   /* for learning flow agg */
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_LEARNING_FLOW_AGG;
   rule_cfg.match_result[0].param.learning_flow_agg.flow_aggregation =
      agg_handle;

   rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;

   h_PCE_rule = WP_PceRuleCreate (WP_WINPATH (DEFAULT_WPID),
                                  WP_PCE_RULE_CLASSIFICATION, &rule_cfg);
   if (!WP_ERROR_P (h_PCE_rule))
   {
      lrn_flwagg_rules[cur].handle = h_PCE_rule;
      lrn_flwagg_rules[cur].pid = portid;
      lrn_flwagg_rules[cur].vid = vid;
      lrn_flwagg_rules[cur].valid = 1;
   }
   else if (WP_ERROR (h_PCE_rule) == WP_ERR_PCE_RULE_ALREADY_EXISTS)
   {
      printf
         ("PCE Learning flow agg rule for vlan %d port %d already exist!\n",
          vid, portid);
   }
   else
   {
      App_TerminateOnError (h_PCE_rule, "WP_PceRuleCreate", __LINE__);
   }

   return;
}

void WPE_CreateIPV6MatchPceRule (WP_U8 portid, WP_U8 * ipv6)
{
   WP_pce_rule_classification rule_cfg = { 0 };
   WP_handle port_handle = 0, agg_handle = 0, h_PCE_rule = 0;

   switch (portid)
   {
   case 0:
   case 1:
      port_handle = gbe[portid].bport_enet;
      agg_handle = gbe[1 - portid].agg_enet;
      break;
   default:
      printf ("NO such port : %d\n", portid);
      return;
   }

   rule_cfg.enabled = WP_ENABLE;

   rule_cfg.filter_handle = PCE_filter[FILTER_SET_IPV6_IP_CLASSIFICATION];

   rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_INPUT_IW_PORT;
   rule_cfg.rule_fields[0].value.iw_port_handle = port_handle;

   rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_IPV6_DA_HIGH;
   memcpy (rule_cfg.rule_fields[1].value.ipv6_addr_half, ipv6, 8);

   rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_IPV6_DA_LOW;
   memcpy (rule_cfg.rule_fields[2].value.ipv6_addr_half, ipv6 + 8, 8);

   rule_cfg.rule_fields[3].field_id = WP_PCE_FIELD_ID_LAST;

   rule_cfg.match_action = WP_PCE_RULE_MATCH_ACCEPT;

   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
   rule_cfg.match_result[0].param.flow_agg.flow_aggregation = agg_handle;

   rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;

   h_PCE_rule = WP_PceRuleCreate (WP_WINPATH (DEFAULT_WPID),
                                  WP_PCE_RULE_CLASSIFICATION, &rule_cfg);
   if (WP_ERROR (h_PCE_rule) == WP_ERR_PCE_RULE_ALREADY_EXISTS)
   {
      printf ("PCE rule already exist!\n");
   }
   else
   {
      App_TerminateOnError (h_PCE_rule, "WP_PceRuleCreate", __LINE__);
   }

   return;
}

void WPE_CreateL4PortPceRule (WP_U8 portid, WP_U16 l4_port)
{
   WP_pce_rule_classification rule_cfg = { 0 };
   WP_handle port_handle = 0, agg_handle = 0, h_PCE_rule = 0;

   switch (portid)
   {
   case 0:
   case 1:
      port_handle = gbe[portid].bport_enet;
      agg_handle = gbe[1 - portid].agg_enet;
      break;
   default:
      printf ("NO such port : %d\n", portid);
      return;
   }

   rule_cfg.enabled = WP_ENABLE;

   rule_cfg.filter_handle = PCE_filter[FILTER_SET_L4_PORT_CLASSIFICATION];

   rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_INPUT_IW_PORT;
   rule_cfg.rule_fields[0].value.iw_port_handle = port_handle;

   rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_L4_DST_PORT;
   rule_cfg.rule_fields[1].value.l4_port = l4_port;

   rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_LAST;

   rule_cfg.match_action = WP_PCE_RULE_MATCH_ACCEPT;

   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
   rule_cfg.match_result[0].param.flow_agg.flow_aggregation = agg_handle;

   rule_cfg.match_result[1].result_type = WP_PCE_RESULT_INGRESS_POLICER;
   rule_cfg.match_result[1].param.ingress_policer.policer = policer_handle;   // policer

   rule_cfg.match_result[2].result_type = WP_PCE_RESULT_LAST;

   h_PCE_rule = WP_PceRuleCreate (WP_WINPATH (DEFAULT_WPID),
                                  WP_PCE_RULE_CLASSIFICATION, &rule_cfg);
   if (WP_ERROR (h_PCE_rule) == WP_ERR_PCE_RULE_ALREADY_EXISTS)
   {
      printf ("PCE rule already exist!\n");
   }
   else
   {
      App_TerminateOnError (h_PCE_rule, "WP_PceRuleCreate", __LINE__);
   }

   return;
}

void WPE_CreateL4SubtypePceRule (WP_U8 portid, WP_U32 subtype)
{
   WP_pce_rule_classification rule_cfg = { 0 };
   WP_handle port_handle = 0, agg_handle = 0, h_PCE_rule = 0;

   switch (portid)
   {
   case 0:
   case 1:
      port_handle = gbe[portid].bport_enet;
      agg_handle = gbe[1 - portid].agg_enet;
      break;
   default:
      printf ("NO such port : %d\n", portid);
      return;
   }

   rule_cfg.enabled = WP_ENABLE;

   rule_cfg.filter_handle =
      PCE_filter[FILTER_SET_L4_SUBTYPE_CLASSIFICATION];

   rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_INPUT_IW_PORT;
   rule_cfg.rule_fields[0].value.iw_port_handle = port_handle;

   rule_cfg.rule_fields[1].field_id =
      WP_PCE_FIELD_ID_USER_PROGRAMMABLE_FIELDS;
   rule_cfg.rule_fields[1].value.pce_user_programmable_fields =
      subtype << 24;

   rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_LAST;

   rule_cfg.match_action = WP_PCE_RULE_MATCH_ACCEPT;

   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
   rule_cfg.match_result[0].param.flow_agg.flow_aggregation = agg_handle;

   rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;

   h_PCE_rule = WP_PceRuleCreate (WP_WINPATH (DEFAULT_WPID),
                                  WP_PCE_RULE_CLASSIFICATION, &rule_cfg);
   if (WP_ERROR (h_PCE_rule) == WP_ERR_PCE_RULE_ALREADY_EXISTS)
   {
      printf ("PCE rule - %d already exist!\n", subtype);
   }
   else
   {
      App_TerminateOnError (h_PCE_rule, "WP_PceRuleCreate", __LINE__);
   }

   return;
}

void WPE_CreateReservedMacPceRule (WP_U8 portid, WP_U8 * mac)
{
   WP_pce_rule_classification rule_cfg = { 0 };
   WP_handle port_handle = 0, agg_handle = 0, h_PCE_rule = 0;

   switch (portid)
   {
   case 0:
   case 1:
      port_handle = gbe[portid].bport_enet;
      agg_handle = gbe[1 - portid].agg_enet;
      break;
   default:
      printf ("NO such port : %d\n", portid);
      return;
   }

   rule_cfg.enabled = WP_ENABLE;

   rule_cfg.filter_handle =
      PCE_filter[FILTER_SET_RESERVED_MAC_CLASSIFICATION];

   rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_INPUT_IW_PORT;
   rule_cfg.rule_fields[0].value.iw_port_handle = port_handle;

   rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_MAC_DA;
   memcpy (rule_cfg.rule_fields[1].value.mac_addr, mac, 6);

   rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_LAST;

   rule_cfg.match_action = WP_PCE_RULE_MATCH_ACCEPT;

   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
   rule_cfg.match_result[0].param.flow_agg.flow_aggregation =
      default_agg_host;

   rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;

   h_PCE_rule = WP_PceRuleCreate (WP_WINPATH (DEFAULT_WPID),
                                  WP_PCE_RULE_CLASSIFICATION, &rule_cfg);
   if (WP_ERROR (h_PCE_rule) == WP_ERR_PCE_RULE_ALREADY_EXISTS)
   {
      printf ("PCE rule already exist!\n");
   }
   else
   {
      App_TerminateOnError (h_PCE_rule, "WP_PceRuleCreate", __LINE__);
   }

   return;
}

/* if portid = 0xff, delete all rules belongs to the vid  */
/* if vid = 0xfff, delete all rules belongs to the portid */
void WPE_DeleteLearningFlowAggPceRule (WP_U32 portid, WP_U32 vid)
{
   WP_U32 cur = 0;
   WP_status status;

   while (cur < MAX_LEARNING_FLOWAGG_RULES)
   {
      if (lrn_flwagg_rules[cur].valid)
      {
         if (((lrn_flwagg_rules[cur].pid == portid)
              && (lrn_flwagg_rules[cur].vid == vid)) || ((portid == 0xff)
                                                         &&
                                                         (lrn_flwagg_rules
                                                          [cur].vid ==
                                                          vid))
             || ((lrn_flwagg_rules[cur].pid == portid) && (vid == 0xfff))
             || ((portid == 0xff) && (vid == 0xfff)))

         {
            status = WP_PceRuleDelete (lrn_flwagg_rules[cur].handle);
            App_TerminateOnError (status, "WP_PceRuleDelete", __LINE__);

            lrn_flwagg_rules[cur].valid = 0;
         }
      }

      cur++;
   }
}

/* 
 * only for test, no use 
 */
void WPE_PMTestCreatePceRules (WP_U32 start_vlan)
{
   WP_U32 ii;

   for (ii = 0; ii < NR_GBE; ii++)
   {
      WPE_CreateLearningFlowAggPceRule (ii, start_vlan + ii);
      WPE_CreateLearningFlowAggPceRule (10, start_vlan + ii);
   }

   return;
}

void WPE_PMTestDeletePceRules (WP_U32 start_vlan)
{
   WP_U32 ii;

   for (ii = 0; ii < NR_GBE; ii++)
   {
      WPE_DeleteLearningFlowAggPceRule (ii, start_vlan + ii);
      WPE_DeleteLearningFlowAggPceRule (10, start_vlan + ii);
   }

   return;
}

WP_U32 WT_TimeRead (void)
{
#ifdef __linux__
   return WP_TimeRead ();
#else
   WP_U32 i = 0;
   __asm volatile ("mfc0 %0, $9; nop":"=r" (i));

   return i;
#endif
}

WP_U32 WT_TimeDelta (WP_U32 later, WP_U32 earlier)
{
#ifdef __linux__
   return WP_TimeDelta (later, earlier);
#else

   //keatin WP_U32 delta = later - earlier;

   /*delta  is in mips frequency clocks divided by 2 */
   // keatin return (delta / (WT_SCH_TIMESTAMP / 2));
   return WP_TimeDelta (later, earlier);
#endif
}

int WPE_Receive_HostData (void)
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

   status = WP_HostReceive (rx_host_handle, &data_unit);
   if (status != WP_OK)
   {
      if ((WP_ERROR (status) == WP_ERR_HST_NO_DATA_TO_GET))
      {
         return -1;
      }
      else
         App_TerminateOnError (status, "WP_HostReceive Error()", __LINE__);
   }

   printf ("Receive Packet \n");
   for (i = 0; i < data_unit.data_size; i++)
   {
      if (i % 16 == 0)
         printf ("\n");
      printf ("0x%02x ", data_unit.segment->data[i]);
   }
   printf ("\n Data Size = %d \n", data_unit.data_size);

   status = WP_PoolFree (segment.pool_handle, segment.data);
   App_TerminateOnError (status, "WP_PoolFree ()", __LINE__);

   return 0;
}

void WPE_Receive_HostData_IRQ (WP_tag tag, WP_U32 event, WP_U32 info)
{
   WPL_SemIncrement (&host_rx_sem, 1);

   return;
}

void WPE_Create_Pce_Policer (void)
{
   WP_policer_v2 pv2 = {
      /* method */ WP_POLICER_METHOD_SINGLE_LEAKY_BUCKET,
      /* type */ WP_POLICER_V2_TYPE_INTERNAL,
                                                /* color_mode */ WP_POLICER_COLOR_BLIND,
                                                //WP_POLICER_COLOR_AWARE, //
      /* red_packets_action */ WP_POLICER_DROP_RED_PACKETS,
                                /* cir */ 875000,
                                /*(875000*8/1000)=7,000 Kbps for each stream */
      /* cbs */ 2000,
                                /* eir */ 81250000,
                                /*(81250000*8/1000)=600 Mbps for each stream */
      /* ebs */ 2000,
      /* policer_action */ 0,
      /* frame_len_correction */ WP_POLICER_LENGTH_CORRECT_DISABLE,
   };

   pv2.policer_action = 0;
   policer_handle =
      WP_PolicerCreate (WP_WINPATH (DEFAULT_WPID), WP_POLICER_V2, &pv2);
   App_TerminateOnError (policer_handle, "WP_PolicerActionCreate",
                         __LINE__);
}

void WT_PCERulesAdd (void)
{
   WP_U8 ipv6[16] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
      0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
   };
   WP_U8 mac[6] = { 0x01, 0x80, 0xc2, 0x00, 0x00, 0x00 };

   WPE_Create_Pce_Policer ();

   WPE_CreateIPV6MatchPceRule (0, ipv6);

   WPE_CreateL4PortPceRule (0, 68);
   WPE_CreateL4PortPceRule (0, 67);

   WPE_CreateL4SubtypePceRule (0, 1);

   WPE_CreateReservedMacPceRule (0, mac);

   return;
}
