
WP_iw_global max_iw[1] =
{
   {
      /* max_iw_aggregations */ 25,
      /* max_r_iw_systems;*/ 1,
      /* max_s_iw_systems;*/ 0,
      /* max_b_iw_systems;*/ 1,
      /* max_m_iw_systems;*/ 0,
      /* max_vlan_priority_maps; */ 2,
      /* iw_bkgnd_fifo_size */0,
      /* cong_pt       */NULL,
      /* iw_host_limits */{10,0,0,10,0},
      /* mpls_config*/ {0,0},
      /* iw_modes;*/
      {  /*policer_mode;*/WP_IW_POLICER_DISABLE,
         /*statistics_bundle_stat_mode;*/WP_IW_STATISTICS_BUNDLE_STAT_DISABLE,
         /*l2_prefix_ext_mode;*/WP_IW_L2_HEADER_EXTENSION_DISABLE,
         /*enhanced_flow_stat_mode;*/WP_IW_ENHANCED_FLOW_STAT_DISABLE,
         /*Flow_stat_modeg;*/WP_IW_FLOW_STAT_ENABLE,
         /*fr_tx_flow_stat_mode*/WP_IW_FR_TX_FLOW_STAT_DISABLE,
         /*mfc_alloc_mode*/WP_IW_MFC_ALLOC_ENABLE,
         /*learning_queue_mode */WP_IW_LEARNING_DISABLE,
         /*port_filtering_mode*/ WP_IW_PORT_FILTERING_DISABLE,
         /*lpm_alloc_mode*/WP_IW_LPM_ALLOC_DISABLE,//WP_IW_LPM_ALLOC_ENABLE,
         /*enhanced_mc_member_stat_mode*/WP_DISABLE,
         /*routing_alloc_modes*/WP_DISABLE,
         /*res_mc_addresses_table_mode*/WP_DISABLE,
         /*port_stag_etype_mode*/WP_IW_PORT_STAG_ETYPE_ENABLE
      },
      /* max_iw_stat_bundles*/ 0,
      /* max_fr_s_iw_systems*/ 0,
      /* max_iw_ports */ 25,
      /* max_iw_mc_groups*/ 5,
      /* max_nat_systems*/ 0,
      /* max_iw_second_aggregations*/ 11,
      /* iw_comp_limits*/ NULL,
      /* max_iw_mc_members */  5,
      /* max_iw_programmable_filters */ 0,
      /* max_iw_policers */ 0,
      /* max_dscpmt */ 1,
      /* num_of_wred_binding */10
   }
};

/* An array of one WP_context structure for each WinPath */
WP_context context[1] =
{
   {
      10, /* qnodes */
      /* the first four assignments are mandatory bus assignments */
      {
         /*WP_VB_PARAM = 0, 			*/{WP_BUS_PARAM, APP_BANK_PARAM},
         /*WP_VB_PACKET, 			*/{WP_BUS_PACKET, APP_BANK_PACKET},
         /*WP_VB_INTERNAL, 			*/{WP_BUS_INTERNAL, APP_BANK_INTERNAL},
         /*WP_VB_HOST, 				*/{WP_BUS_HOST, APP_BANK_HOST},
         /*WP_VB_INTQUEUE_DEBUG, 		*/{WP_BUS_HOST, APP_BANK_HOST},
         /*WP_VB_TIMESTAMP, 			*/{WP_BUS_HOST, APP_BANK_HOST},
         /*WP_VB_CAS, 				*/{WP_BUS_NONE, 0},
         /*WP_VB_TRANS_TSAT, 			*/{WP_BUS_NONE, 0},
         /*WP_VB_TPT, 				*/{WP_BUS_INTERNAL, APP_BANK_INTERNAL},
         /*WP_VB_CW, 				*/{WP_BUS_INTERNAL, APP_BANK_INTERNAL},
         /*WP_VB_ASU_TXQUEUE, 			*/{WP_BUS_PARAM, APP_BANK_PARAM},
         /*WP_VB_STATISTICS, 			*/{WP_BUS_INTERNAL, APP_BANK_INTERNAL},
         /*WP_VB_ATM_ADDLOOKUP, 		*/{WP_BUS_PARAM, APP_BANK_PARAM},
         /*WP_VB_ATM_CPT_EBMT_EXTHEADER,        */{WP_BUS_PARAM, APP_BANK_PARAM},
         /*WP_VB_ETH_HDLC_CPT, 			*/{WP_BUS_PARAM, APP_BANK_PARAM},
         /*WP_VB_FBP_BD, 			*/{WP_BUS_PARAM, APP_BANK_PARAM},
         /*WP_VB_FBP_DATA, 			*/{WP_BUS_PACKET, APP_BANK_PACKET},
         /*WP_VB_L2_BKGND, 			*/{WP_BUS_PACKET, APP_BANK_PACKET},
         /*WP_VB_BRIDGE_LEARNING_DFC_PFMT       */{WP_BUS_PACKET, APP_BANK_PACKET},
         /*WP_VB_IMA_MLPPP_STATE, 		*/{WP_BUS_INTERNAL, APP_BANK_INTERNAL},
         /*WP_VB_IMA_MLPPP_BUFFER, 		*/{WP_BUS_HOST, APP_BANK_HOST},
         /*WP_VB_HASH_IW_LPM_EMC, 		*/{WP_BUS_PACKET, APP_BANK_PACKET},
         /*WP_VB_IW_MFC_RULES, 			*/{WP_BUS_PACKET, APP_BANK_PACKET},
         /*WP_VB_IW_FIWT_QAT, 			*/{WP_BUS_INTERNAL, APP_BANK_INTERNAL},
         /*WP_VB_IW_MFCPT_DIFFSERV_UPPT_D       */{WP_BUS_INTERNAL, APP_BANK_INTERNAL},
         /*WP_VB_STATISTICS2, 			*/{WP_BUS_INTERNAL, APP_BANK_INTERNAL},
         /*WP_VB_STATISTICS3, 			*/{WP_BUS_INTERNAL, APP_BANK_INTERNAL},
         /*WP_VB_UNASSIGNED6, 			*/{WP_BUS_NONE, 0},
         /*WP_VB_PKT_GLOBAL_TCPT, 		*/{WP_BUS_NONE, 0},
         /*WP_VB_UNASSIGNED8, 			*/{WP_BUS_NONE, 0},
         /*WP_VB_UNASSIGNED9, 			*/{WP_BUS_NONE, 0},
         /*WP_VB_UNASSIGNED10, 			*/{WP_BUS_NONE, 0},
         /*WP_VB_MCH_DATA,                      */{WP_BUS_NONE,0},
         /*WP_VB_OAM_FM_QUERY,	                */{WP_BUS_NONE,0},
         /*WP_VB_HASH_ON_HEAP,	                */{WP_BUS_NONE,0},
         /*WP_VB_MFC_INTERNAL,	                */{WP_BUS_HOST, APP_BANK_HOST},
         /*WP_VB_MFR_RAS_TBL,                   */{WP_BUS_NONE, 0},
         /*WP_VB_CESOP_DATA,                    */{WP_BUS_NONE, 0},
         /*WP_VB_SECURITY_ESE,                  */{WP_BUS_NONE, 0},
         /*WP_VB_PDCP_SECURITY_ENTITY,          */{WP_BUS_NONE, 0},
         /*WP_VB_POLICER,                       */{WP_BUS_PARAM, APP_BANK_PARAM},
         /*WP_VB_PCE_LRN,                       */{WP_BUS_INTERNAL, APP_BANK_INTERNAL},
         /*WP_VB_PCEA_PARAM,                    */{WP_BUS_INTERNAL, APP_BANK_INTERNAL},
         /*WP_VB_CONGESTION_TABLE,              */{WP_BUS_NONE, 0},
         /*WP_VB_PARAMS_PROPAGATION             */{WP_BUS_NONE, 0},
         /*WP_VB_STATS_IWPORT_PCE_FILTERS       */{WP_BUS_INTERNAL, APP_BANK_INTERNAL},
      },
      {
         dps_ProgramImage, dps_ProgramImage
      },
      {
         dps_PC_Table_Init, dps_PC_Table_Init
      },
      &int_queue_tables, /* Interrupt queue tables */
      POOL_LIMIT,        /* Mixumum number of pools */

      NULL,        /* ATM global parameters */
      max_iw,      /* Interworking global parameters */
   }
};

WP_calendar_wheel calendar[2] =
{
   {
      /* function */    WP_CW_ASU,
      /* cwid*/         WP_CW_ID_A,
      /* min_ch_rate */ MIN_CH_RATE,
      /* max_ch_rate */ MAX_CH_RATE,
   },
   WP_CALENDAR_WHEEL_TERM
};

WP_pool_buffer_data iw_buffer_data[1] =
{
   {
      /* n_buffers */ IW_BUF_N_BUFFERS,
      /* offset */ 64,
      /* size */ MAX_PACKET_SIZE,
      /* pad */ 0,
      /* bus */ WP_BUS_PACKET,
      /* bank */ APP_BANK_PACKET
   }
};

WP_tx_binding_wred_enhanced  tx_binding_wred_cfg[1]=
{
   {
      /* wred_stat_enable */ WP_ENABLE,
      /* dci_mode;*/  WP_IW_DYN_CH_INSERT_DISABLE,
      /* exp_sampling_rate; */    1,
      /* exp_weight; */          2,
   }
};

WP_tx_binding tx_binding_cfg[1] =
{
   {
      /* wred_mode;*/ 0,
      /* dci_mode; */ 1,
      /* maxt;     */ 1255,
   }
};

WP_tx_binding_bytes  tx_binding_bytes_cfg[1]=
{
   {
      /* maxt_bytes;*/10000,
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

WP_rx_binding_routing rx_binding_routing[1]=
{
   {
      /*  encap_mode*/ 0,
      /*  l3_support_protocol;*/WP_IW_IPV4,
      /*  mru;*/1488
   }
};

WP_rx_binding_routing rx_binding_routing_cfg[1]=
{
   {
      /* WP_U8     encap_mode */ WP_IW_VCMUX /*WP_IW_ENET_TYPE_1*/,
      /* WP_U32    l3_support_protocol */ WP_IW_IPV4 | WP_IW_IPV6 | WP_IW_ARP,
      /* WP_U16    mru */512,
      /* WP_U8     rx_direction */0,
      /* WP_handle input_port */0,
      /* WP_handle decomp_group */0
   }
};


WP_qnode_iwq qn_iw[1]=
{
   {
      /* interruptqueue */ 0,
      /* num_buffers */ QN_IW_N_BUFFERS,
      /* adjunct_pool */ 0
   }
};

/*************************************/
/*** InterWorking - Configurations ***/
/*************************************/

WP_dfc_sys_info dfc_sys_info_gcsi =
{
   /* classification_hash_size */ WP_IW_16K_HASH_ENTRIES,
   /* port_filtering */           WP_IW_PORT_FILTERING_DISABLED,
   /* deny_eq_ip */               WP_IW_DENY_EQUAL_IP_DISABLE,
   /* dfc_options*/               WP_DISABLE
};

WP_iw_sys_bridging iw_sys_gcsi[1]=
{
   {
      /* tag */                   0,
      /* max_flows; */            IW_SYS_MAX_FLOWS,
      /* classification_mode */   WP_IW_DYNAMIC_CLASS,
      /* classifier_config: */
      {
         /*classification_parsing_mode*/ WP_IW_DYNAMIC_CLASS,
         /*max_classification_rules;*/1000,
         /*num_parse_fields - unused*/0,
         /*parse_keys*/NULL
      },
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
      /*buffer_gap;*/             0x40,
      /*max_bridging_ports*/      IW_SYS_MAX_BPORTS,
      /*dfc_info:*/               &dfc_sys_info_gcsi,
      /*svl_mode*/                WP_IW_SVL_DISABLED,
      /*stag_ether_type*/         0x8100/*DEFAULT VALUE*/ | WP_IW_BPORT_IWPORT_STAG_VLAN_ETYPE,
   }
};


WP_iw_ipv4_sys_info ipv4_sys_info[1] =
{
   {
      /*WP_U8 classification_mode*/WP_IW_LPM | WP_IW_DYNAMIC_CLASS,
      /*WP_U16 max_lpm_flows*/ 100,
      /*WP_dfc_sys_info *dfc_info*/&dfc_sys_info_gcsi
   }
};

WP_iw_ipv6_sys_info ipv6_sys_info[1] =
{
   {
      /*WP_U8 classification_mode*/WP_IW_LPM | WP_IW_DYNAMIC_CLASS,
      /*WP_U16 max_lpm_flows*/ 100,
      /*WP_iw_hash_entries lpm_hash_size*/WP_IW_32_HASH_ENTRIES,
      /*WP_dfc_sys_info *dfc_info*/&dfc_sys_info_gcsi
   }
};

WP_iw_sys_routing iw_sys_gcsi_routing[1] =
{
   {
      /* WP_U8 buffer_gap */0x40,
      /* WP_U16 max_iw_ports */IW_SYS_MAX_BPORTS,
      /* WP_U16 max_classification_rules */1000,
      /* WP_U32 supported_protocols */WP_IW_SYS_IPV4 | WP_IW_SYS_IPV6,
      /* WP_iw_ipv4_sys_info *ipv4_sys_info */ipv4_sys_info,
      /* WP_iw_ipv6_sys_info *ipv6_sys_info */ipv6_sys_info,
      /* WP_iw_mpls_sys_info *mpls_sys_info */NULL
   }
};

WP_iw_sys_bridging iw_sys[1]=
{
   {
      /* tag */                   0,
      /* max_flows; */            IW_SYS_MAX_FLOWS,
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
      /*buffer_gap;*/             0x40,
      /*max_bridging_ports*/      IW_SYS_MAX_BPORTS
   }
};

WP_bridge_port bport_conf[1] =
{
   {
      /* tag */                   0,
      /* direct_mapping*/         WP_IW_DIRECT_MAP_ENABLE,
      /* flow_agg */              0,
      /* termination_mode*/       WP_IW_HOST_TERM_MODE,
      /* learning_mode */         WP_IW_LEARNING_DISABLE,
      /* in_filter_mode */        WP_BRIDGE_INGRESS_FILTER_DISABLE,
      /* vlan_param */
      {
         /*vlan_acceptance_mode */WP_IW_ACCEPT_TAGGED_UNTAGGED,
         /*vlan_tag */0xA,
         /*vlan_tunnel*/WP_IW_VLAN_TUNNEL_1Q_IN_1Q_DISABLE,
         /*vlan_priority_enforce_mode*/WP_IW_VLAN_PRIORITY_ENFORCE_DISABLED,
         /*stag_vlan_etype*/ 0x9100
      },
      /* max_mac_addresses */ 20,
      /* group_tag */ 0,
      /*group_filtering_mode*/     WP_IW_GROUP_FILTER_DISABLE,
      /*unk_mac_sa_filter*/        WP_IW_UNK_MACSA_FILTER_DISABLE,
      /*unk_mc_mode*/              WP_IW_UNK_MC_DROP,
      /*bc_ht_mode*/               WP_IW_BC_HT_DISABLE,
      /*unk_input_filters_mask*/   0,
      /*unk_output_filters_mask*/  0,
      /*statmode*/                 WP_IW_PORT_STAT_ENABLE,
      /*unk_uc_mode*/              WP_IW_UNK_UC_SR_ENABLE,
      /*classification_flag*/      WP_IW_BPORT_CLASSIFICATION_DISABLED, //WP_IW_BPORT_CLASSIFICATION_ENABLED,
      /*adv_unk_lookup_mode*/      WP_IW_ADV_UNK_LOOKUP_ENABLED,
      /*cfi_ht_mode*/              WP_IW_CFI_HT_DISABLE,
      /*reserved_mc_ht_mode*/      WP_IW_RES_MC_HT_DISABLE,
      /*predefined_ht_mode*/       WP_DISABLE, /*WP_IW_ARP_HT_ENABLE | WP_IW_IGMP_HT_ENABLE*/
      /*res_mc_action_table 0,*/
   }
};

WP_dfc_sys_info dfc_rout_sys_info[1] =
{
   {
      /* classification_hash_size */ WP_IW_HASH_UNUSED,
      /* port_filtering */           WP_IW_PORT_FILTERING_DISABLED,
      /* deny_eq_ip */               WP_IW_DENY_EQUAL_IP_DISABLE
   }
};

WP_iw_ipv4_sys_info ipv4_sys_one[1] =
{
   {
      /* classification_mode */ WP_IW_NULL_CLASSIFIER,
      /* max_lpm_flows */ 0,
      /* *dfc_info  */ &dfc_rout_sys_info[0]
   }
};

WP_iw_sys_routing iw_sys_one_routing[1]=
{
   {
      /*  buffer_gap */ 0x40,
      /*  max_iw_ports */ 30,
      /*  max_classification_rules */  2,
      /* supported_protocols */ WP_IW_SYS_IPV4,
      /* ipv4_sys_info*/ipv4_sys_one,
      /* *ipv6_sys_info*/NULL,
      /* *mpls_sys_info*/NULL,
   }
};

WP_iw_route_port route_port_cfg = 
{
   /* tag */ 0,
   /* input_filters_mask; */ 0,
   /* output_filters_mask; */ 0,
   /* statmode; */ WP_IW_PORT_STAT_ENABLE,
   /* forwarding_mode */ WP_IW_FORWARDING_DIRECT_MAP,
};

WP_iw_route_port iw_port_routing_conf[1] =
{
   {
      /*WP_tag tag*/0,
      /*WP_U32 input_filters_mask*/0,
      /*WP_U32 output_filters_mask*/0,
      /*WP_iw_port_statmode statmode*/WP_IW_PORT_STAT_ENABLE,
      /*WP_U8 forwarding_mode*/WP_IW_FORWARDING_DFC | WP_IW_FORWARDING_LPM,
      /*WP_handle flow_agg*/0,
      /*WP_U8 ipv6_source_address_validation*/WP_DISABLE,
      /*WP_iw_ipv6_host_term_mode ipv6_host_term_mode*/WP_IW_PORT_IPV6_HOST_TERM_DISABLE
   }
};

WP_port_enet enet_s[1] =
{
   {
      /* pkt_limits */  { 5, 5, 0, 0, 0},
      /* flowmode */ WP_FLOWMODE_FAST,  //TODO | WP_FLOWMODE_ENET_WRED_ENABLE,
      /* interface_mode */ WP_ENET_SGMII_1000,
      /* rx_iw_bkgnd */WP_IW_BKGND_NOT_USED,
   },
};


WP_enet_filters rx_filters[1] =
{{
   WP_ENET_FILTER_PROCESS,
   WP_ENET_FILTER_PROCESS,
   WP_ENET_FILTER_PROCESS
}};


WP_device_enet_ex enet_dev_ex[1] =
{
   {
      /* duplexmode;*/WP_ENET_FULLDUPLEX,
      /* rx_maclookup_mode;*/WP_DISABLE,
      /* rx_flowcontrol;*/WP_ENET_FLOW_ENABLE,
      /* tx_flowcontrol;*/WP_ENET_FLOW_ENABLE,
      /* rx_addrmode;*/WP_ENET_ADDR_ACCEPTALL,
      /* phystatmode;*/WP_ENET_STAT_ENABLE,
      /* tx_statmode;*/WP_PKT_STAT_ENABLE,
      /* rx_statmode;*/WP_PKT_STAT_ENABLE,
      WP_DISABLE,
      rx_filters,
      WP_DISABLE,
      /*WP_ENET_32BIT_TS,*/
      0,
   },
};

WP_device_enet enet_dev[1] =
{
   {
      /* max_tx_channels */ 5,
      /* tx_maxsdu */  1536,
      /* operating_speed */ WP_UNUSED,
      /* mac_addr*/ {0xaa,0xbb,0xcc,0xdd,0xee,0xff},
      /* tx_bit_rate */ WP_TX_BIT_RATE_UNLIMITED,
      /* loopbackmode */ WP_ENET_NORMAL,
      /* extended_params */ NULL,
   }
};

WP_ch_iw_tx iw_tx_ch_cfg[] =
{
   {
      /* iw_system */ 0,
      /* reserved */ 0,
      /* group_id */ 0,
      /* reserved2 */ 0,
      /* input_port */ 0
   }
};

WP_ch_enet ch_config_enet[1] =
{
   {
      /* intmode */ WP_PKTCH_INT_DISABLE,
      /* iwmmode */ WP_PKTCH_IWM_ENABLE,
      /* testmode */ WP_PKTCH_TEST_DISABLE,
      /* tx_pqblock */ 0,
      /* tx_pqlevel */ 0,
      /* tx_shaping_type */ WP_FMU_SHAPING_TYPE_STRICT,
      /* tx_shaping_params */ NULL,
      /* rx_maxsdu */ 1536,
      /* tx_cwid */ WP_CW_ID_A,
      /* tx_tq */ 0,
      /* rx_queuedepth */520,
   },
};

WP_policer_action_color_params pac_params =
{
   /* action_mode */ 
   WP_POLICER_ACTION_PRIORITY_MAPPING | WP_POLICER_ACTION_PREFIX_REMARKING,
   {
      {WP_POLICER_ACTION_PREFIX_REMARKING_TYPE_EXT_VLAN_PRIO,
       WP_POLICER_ACTION_PREFIX_REMARKING_TYPE_INT_VLAN_PRIO,
       WP_POLICER_ACTION_PREFIX_REMARKING_TYPE_DO_NOT_CHANGE,
       WP_POLICER_ACTION_PREFIX_REMARKING_TYPE_TOS
      },
      {
         3, 4, 0, 1
      }
   },
   /* priority_mapping_value */ 0x15,
   /* congestion_profile_entry */ 0
};

WP_policer_action policer_action =
{
   {/* green */
   WP_POLICER_ACTION_PRIORITY_MAPPING | WP_POLICER_ACTION_PREFIX_REMARKING,
   {
      {WP_POLICER_ACTION_PREFIX_REMARKING_TYPE_EXT_VLAN_PRIO,
       WP_POLICER_ACTION_PREFIX_REMARKING_TYPE_INT_VLAN_PRIO,
       WP_POLICER_ACTION_PREFIX_REMARKING_TYPE_DO_NOT_CHANGE,
       WP_POLICER_ACTION_PREFIX_REMARKING_TYPE_TOS
      },
      {
         3, 4, 0, 1
      }
   },
   /* priority_mapping_value */ 0x15,
   /* congestion_profile_entry */ 0
   },
   {/* yellow */
   WP_POLICER_ACTION_PRIORITY_MAPPING | WP_POLICER_ACTION_PREFIX_REMARKING,
   {
      {WP_POLICER_ACTION_PREFIX_REMARKING_TYPE_EXT_VLAN_PRIO,
       WP_POLICER_ACTION_PREFIX_REMARKING_TYPE_INT_VLAN_PRIO,
       WP_POLICER_ACTION_PREFIX_REMARKING_TYPE_DO_NOT_CHANGE,
       WP_POLICER_ACTION_PREFIX_REMARKING_TYPE_TOS
      },
      {
         3, 4, 0, 1
      }
   },
   /* priority_mapping_value */ 0x15,
   /* congestion_profile_entry */ 0
   },
   {/* red */
   WP_POLICER_ACTION_PRIORITY_MAPPING | WP_POLICER_ACTION_PREFIX_REMARKING,
   {
      {WP_POLICER_ACTION_PREFIX_REMARKING_TYPE_EXT_VLAN_PRIO,
       WP_POLICER_ACTION_PREFIX_REMARKING_TYPE_INT_VLAN_PRIO,
       WP_POLICER_ACTION_PREFIX_REMARKING_TYPE_DO_NOT_CHANGE,
       WP_POLICER_ACTION_PREFIX_REMARKING_TYPE_TOS
      },
      {
         3, 4, 0, 1
      }
   },
   /* priority_mapping_value */ 0x15,
   /* congestion_profile_entry */ 0
   },
};

WP_policer_v2 pv2 = 
{
   /* method */ WP_POLICER_METHOD_SINGLE_LEAKY_BUCKET,
   /* type */ WP_POLICER_V2_TYPE_INTERNAL, 
   /* color_mode */ WP_POLICER_COLOR_AWARE, 
   /* red_packets_action */ WP_POLICER_DROP_RED_PACKETS,
   /* cir */ 1,
   /* cbs */ 2,
   /* eir */ 3,
   /* ebs */ 4,
   /* policer_action */ 0,
};

WP_policer_limits_v2 v2_policer_limits[1]=
{
   {
      /* max_internal_policers*/5,
      /* max_external_policers*/5,
      /* max_policer_actions*/5
   }
};

WP_module_policer policer_module_config[1]=
{
   {
      /*v2_policer_limits*/v2_policer_limits,
      /*v1_policer_limits*/0,
      /*atm_policer_limits*/0,
      /*mode*/WP_POLICER_STATS_ENABLE
   }
};

WP_iw_agg_generic flow_agg_gen[1] =
{
   {
      /*tag*/ 2,
      /*txfunc*/ 0,
      /*iw_port*/0,
      /*rfcs*/WP_IW_RFCS_DISABLE,
      /*interruptqueue;*/WP_IW_IRQT1,
      /*error_pkt_mode*/WP_IW_ERRPKT_DISCARD,
      /*intmode;*/WP_IW_INT_ENABLE,
      /*statmode;*/WP_IW_STAT_ENABLE,
      /*timestamp_mode;*/WP_IW_TIME_STAMP_DISABLE,
      /*mtu;*/1536,
      /*flow_agg_type;*/WP_IW_FLOW_AGG_PRIMARY,
      /*policer_handle;*/0,
      /*pecs_handle;*/0,
      /*pecs_user_info;*/0
   },
};

WP_fmu_shaping_cir_eir  shaping_params = 
{
   /* cir */    170000000,
   /* cbs */    800000,
   /* eir */    170000000,
   /* ebs */    20000,
   /* flags */ WP_FMU_SHAPING_CIR_EIR_SELECT_MODE_RR
};

WP_shaping_group_type_packet shaping_group_l1[1] = 
{
   {
      WP_L1_GROUP, /* group_level */
      WP_FMU_SHAPING_TYPE_CIR_EIR, /* tx_shaping_type */
      &shaping_params, /* *tx_shaping_params */
      WP_UNUSED, /* num_fifos */
      0, /* block_handle */
      0, /* block_level */
      WP_UNUSED, /* group_mode */
   },
};

WP_shaping_group_type_packet shaping_group_l2[1] = 
{
   {
      WP_L2_GROUP, /* group_level */
      WP_FMU_SHAPING_TYPE_STRICT, /* tx_shaping_type */
      NULL, /* *tx_shaping_params */
      WP_NUM_FIFOS_8, /* num_fifos */
      0, /* block_handle */
      0, /* block_level */
      WP_MODE_HW, /* group_mode */
   },
};

WP_tx_binding tx_binding_gbe[] =
{
   {
      /* res0 */ 0,
      /* dci_mode;*/  WP_IW_DYN_CH_INSERT_DISABLE,
      /* maxt;*/100,
   }
};

WP_ch_iw_rx iw_rx_ch_cfg[] =
{
   {
      /* pqblock */ 0,
      /* pqlevel */ 0,
      /* tx_binding_type */ WP_IW_TX_BINDING,
      /* tx_binding_config */ tx_binding_gbe
   }
};


WP_iw_agg_ipv4_routing tx_agg_routing[1] =
{
   {
      /* tag */ 2,
      /* txfunc */ 0,
      /* dfsrv_remarking */ WP_IW_TOS_REMARK_DISABLE,
      /* l2_header_insert_mode */ WP_IW_L2H_INSERT_DISABLE,
      /* ip_header_replace_mode */ 0,
      /* interruptqueue */ WP_IW_IRQT1,
      /* error_pkt_mode */ WP_IW_ERRPKT_DISCARD,
      /* intmode */ WP_IW_INT_DISABLE,
      /* statmode */ WP_IW_STAT_ENABLE,
      /* timestamp_mode */ WP_IW_TIME_STAMP_DISABLE,
      /* ov_pool_mode */ WP_IW_OV_POOL_DISABLE,
      /* fbp_drop_threshold */ 0,
      /* mtu */ 1536,
      /* dfsrv_code_point */ 0,
      /* prefix_length */ 16,
      /* multi_cong_prof */ WP_DISABLE,
      /* prefix_header[48] */ {0x03,0x01,0x01,0x01,0x01,0x03,
                               0x02,0x01,0x01,0x01,0x01,0x02,
                               0x00,0x0D,0,0,0,0,0,0,
                               0,0,0,0,0,0,0,0,
                               0,0,0,0,0,0,0,0,
                               0,0,0,0,0,0,0,0,},
      /* new_ip_header[20] */ {0,0,0,0,0,0,0,0,
                               0,0,0,0,0,0,0,0,
                               0,0,0,0},
      /* policer_enable */ WP_IW_POLICER_DISABLE,
      /* *policer_config */ NULL,
      /* cong_mode */ WP_IW_CONGESTION_DISABLE,
      /* *cong_threshold_param */ NULL,
      /* dynamic_length_update */ WP_IW_LENGTH_UPDATE_DISABLED,
      /* length_offset */ 0
      /* tx_direction */
      /* flow_agg_type */
      /* iw_port */
      /* comp_group_handle */
      /* ttl_dec_mode */
      /* dscpmt_handle */
      /* next_sys_preserve_inport */
      /* next_sys_preserve_inport_params */
      /* l2_prefix_vlan_mode */
      /* policer_handle */
      /* convert_ipv4_mc_to_ethernet_mc */
      /* n_prefix_remark_fields */
      /* prefix_remark_offset[4] */
      /* mac_replace_mode */
      /* ipv4_encapsulation_mode */
      /* programmable_prefix_mode */
      /* programmable_prefix_offset */
   }
};

WP_iw_agg_bridging tx_agg_bridging[1] =
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

WP_module_pce_init pce_init =
{
   &pce_parser_info, /* parser_info */
   4000, /* hardware_rules_num */
   50, /* filters_num */
   20, /* pkt_rx_channel_interfaces_num */
   20, /* iw_tx_channel_interfaces_num */
   WP_PCE_DISCARD_MODE_ALL, /* pce_discard_mode */
   WP_ENABLE, /* bridge_loop_prevention_mode */
};

WP_pce_filter_stat pce_filter_stats[256];
WP_pce_global_stat pce_global_stats;

WT_Interface global_interfaces[WT_MAX_INTERFACES+1];
WT_TestGlobalInfo global_info;
