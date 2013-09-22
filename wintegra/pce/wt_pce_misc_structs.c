
WP_iw_global max_iw[1] =
{
   {
      /* max_iw_aggregations */ 5000,
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
#ifndef WINSIM
         /*lpm_alloc_mode*/WP_IW_LPM_IPV4_ALLOC_ENABLE |WP_IW_LPM_IPV6_ALLOC_ENABLE,
#else
         /*lpm_alloc_mode*/WP_IW_LPM_ALLOC_DISABLE,
#endif
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
      /* num_of_wred_binding */10,
      /* tcm_policer_limits*/ {0,0},
      /* max_flow_rule_stat_tables */ 0,
      /* max_indirect_action_tables */ 0,
      /* lpm_bridging_limits */ {0, 0},
      /* brigding_time_stamp_info */ {0, 0},
      /* iwgp_bkgnd_fifo_size */0,
      /* lpm_max_ipv4_routing_flows */ 2048,
      /* lpm_max_ipv6_routing_flows */ 2048
   }
};

/* An array of one WP_context structure for each WinPath */
#ifdef WP_BOARD_WDS3_SL
WP_context context[1] =
{
   {
      10, /* qnodes */
      /* the first four assignments are mandatory bus assignments */
      {
         /*WP_VB_PARAM = 0,         */{WP_BUS_PARAM, APP_BANK_PARAM},
         /*WP_VB_PACKET,         */{WP_BUS_PACKET, APP_BANK_PACKET},
         /*WP_VB_INTERNAL,          */{WP_BUS_INTERNAL, APP_BANK_INTERNAL},
         /*WP_VB_HOST,           */{WP_BUS_HOST, APP_BANK_HOST},
         /*WP_VB_INTQUEUE_DEBUG,       */{WP_BUS_HOST, APP_BANK_HOST},
         /*WP_VB_TIMESTAMP,         */{WP_BUS_HOST, APP_BANK_HOST},
         /*WP_VB_CAS,            */{WP_BUS_NONE, 0},
         /*WP_VB_TRANS_TSAT,        */{WP_BUS_NONE, 0},
         /*WP_VB_TPT,            */{WP_BUS_PARAM, APP_BANK_PARAM},
         /*WP_VB_CW,             */{WP_BUS_PARAM, APP_BANK_PARAM},
         /*WP_VB_ASU_TXQUEUE,          */{WP_BUS_PARAM, APP_BANK_PARAM},
         /*WP_VB_STATISTICS,        */{WP_BUS_PARAM, APP_BANK_PARAM},
         /*WP_VB_ATM_ADDLOOKUP,     */{WP_BUS_PARAM, APP_BANK_PARAM},
         /*WP_VB_ATM_CPT_EBMT_EXTHEADER,        */{WP_BUS_PARAM, APP_BANK_PARAM},
         /*WP_VB_ETH_HDLC_CPT,         */{WP_BUS_PARAM, APP_BANK_PARAM},
         /*WP_VB_FBP_BD,         */{WP_BUS_PARAM, APP_BANK_PARAM},
         /*WP_VB_FBP_DATA,          */{WP_BUS_PARAM, APP_BANK_PARAM},
         /*WP_VB_L2_BKGND,          */{WP_BUS_PARAM, APP_BANK_PARAM},
         /*WP_VB_BRIDGE_LEARNING_DFC_PFMT       */{WP_BUS_PARAM, APP_BANK_PARAM},
         /*WP_VB_IMA_MLPPP_STATE,      */{WP_BUS_PARAM, APP_BANK_PARAM},
         /*WP_VB_IMA_MLPPP_BUFFER,     */{WP_BUS_HOST, APP_BANK_HOST},
         /*WP_VB_HASH_IW_LPM_EMC,      */{WP_BUS_PARAM, APP_BANK_PARAM},
         /*WP_VB_IW_MFC_RULES,         */{WP_BUS_PARAM, APP_BANK_PARAM},
         /*WP_VB_IW_FIWT_QAT,          */{WP_BUS_PARAM, APP_BANK_PARAM},
         /*WP_VB_IW_MFCPT_DIFFSERV_UPPT_D       */{WP_BUS_PARAM, APP_BANK_PARAM},
         /*WP_VB_STATISTICS2,          */{WP_BUS_PARAM, APP_BANK_PARAM},
         /*WP_VB_STATISTICS3,          */{WP_BUS_PARAM, APP_BANK_PARAM},
         /*WP_VB_UNASSIGNED6,          */{WP_BUS_NONE, 0},
         /*WP_VB_PKT_GLOBAL_TCPT,      */{WP_BUS_NONE, 0},
         /*WP_VB_UNASSIGNED8,          */{WP_BUS_NONE, 0},
         /*WP_VB_UNASSIGNED9,          */{WP_BUS_NONE, 0},
         /*WP_VB_UNASSIGNED10,         */{WP_BUS_NONE, 0},
         /*WP_VB_MCH_DATA,                      */{WP_BUS_NONE,0},
         /*WP_VB_OAM_FM_QUERY,                   */{WP_BUS_NONE,0},
         /*WP_VB_HASH_ON_HEAP,                   */{WP_BUS_NONE,0},
         /*WP_VB_MFC_INTERNAL,                   */{WP_BUS_HOST, APP_BANK_HOST},
         /*WP_VB_MFR_RAS_TBL,                   */{WP_BUS_NONE, 0},
         /*WP_VB_CESOP_DATA,                    */{WP_BUS_NONE, 0},
         /*WP_VB_SECURITY_ESE,                  */{WP_BUS_NONE, 0},
         /*WP_VB_PDCP_SECURITY_ENTITY,          */{WP_BUS_NONE, 0},
         /*WP_VB_POLICER,                       */{WP_BUS_PARAM, APP_BANK_PARAM},
         /*WP_VB_PCE_LRN,                       */{WP_BUS_PARAM, APP_BANK_PARAM},
         /*WP_VB_PCEA_PARAM,                    */{WP_BUS_PARAM, APP_BANK_PARAM},
         /*WP_VB_CONGESTION_TABLE,              */{WP_BUS_NONE, 0},
         /*WP_VB_PARAMS_PROPAGATION             */{WP_BUS_NONE, 0},
         /*WP_VB_STATS_IWPORT_PCE_FILTERS       */{WP_BUS_PARAM, APP_BANK_PARAM},
#if _WP_MODULE_CUSTOM_TIGERMILK_ENABLE_
         /*WP_VB_PROFILE_MARKING_STATS          */{WP_BUS_NONE, 0},
#endif
         /*WP_VB_TIMESTAMP_SERVICES_TABLE       */{WP_BUS_NONE, 0},
         /*WP_VB_POLICER_ACTION                 */{WP_BUS_PARAM, APP_BANK_PARAM},
         {/* WP_VB_PECS_GLOBAL*/ WP_BUS_NONE, 0},
         {/* WP_VB_ENET_10G_LONG_PKT*/ WP_BUS_NONE, 0},
         {/* WP_VB_G999_STREAM*/ WP_BUS_NONE, 0},
         {/* WP_VB_G999_STREAM_STATISTICS*/ WP_BUS_NONE, 0},
         {/* WP_VB_FMU_FIFO_TO_CHANNEL_NUMBER*/ WP_BUS_NONE, 0},
         {/* WP_VB_IPWORT_EXT*/ WP_BUS_PARAM, APP_BANK_PARAM}

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
#else
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
#if _WP_MODULE_CUSTOM_TIGERMILK_ENABLE_
         /*WP_VB_PROFILE_MARKING_STATS          */{WP_BUS_NONE, 0},
#endif
         /*WP_VB_TIMESTAMP_SERVICES_TABLE       */{WP_BUS_NONE, 0},
         /*WP_VB_POLICER_ACTION                 */{WP_BUS_PARAM, APP_BANK_PARAM},
         {/* WP_VB_PECS_GLOBAL*/ WP_BUS_NONE, 0},
         {/* WP_VB_ENET_10G_LONG_PKT*/ WP_BUS_NONE, 0},
         {/* WP_VB_G999_STREAM*/ WP_BUS_NONE, 0},
         {/* WP_VB_G999_STREAM_STATISTICS*/ WP_BUS_NONE, 0},
         {/* WP_VB_FMU_FIFO_TO_CHANNEL_NUMBER*/ WP_BUS_NONE, 0},
         {/* WP_VB_IPWORT_EXT*/ WP_BUS_PARAM, APP_BANK_PARAM}

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
#endif

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
#ifdef WP_BOARD_WDS3_SL
      /* bus */ WP_BUS_PARAM,
      /* bank */ APP_BANK_PARAM
#else
      /* bus */ WP_BUS_PACKET,
      /* bank */ APP_BANK_PACKET
#endif
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
      /* maxt;     */ 255,
   }
};

WP_tx_binding_bytes  tx_binding_bytes_cfg[1]=
{
   {
      /* maxt_bytes;*/100000,
   }
};

WP_rx_binding_bridging  rx_binding_cfg[1]=
{
   {
       /*  encap_mode */0,
       /*  mru;*/1512,
       /*  vcfcs;*/0,
       /*  input_port;*/0,
       /*  pce_if_handle; */ 0
   }
};

WP_rx_binding_routing rx_binding_routing[1]=
{
   {
      /*  encap_mode*/ 0,
      /*  l3_support_protocol;*/WP_IW_IPV4,
      /*  mru;*/1488,
      /*  rx_direction; */ 0,
      /*  input_port; */ 0,
      /*  decomp_group; */ 0,
      /*  pce_if_handle; */ 0
   }
};

WP_rx_binding_routing rx_binding_routing_cfg[1]=
{
   {
      /* WP_U8     encap_mode */ WP_IW_VCMUX /*WP_IW_ENET_TYPE_1*/,
      /* WP_U32    l3_support_protocol */ WP_IW_IPV4 | WP_IW_IPV6 | WP_IW_ARP,
      /* WP_U16    mru */1512,
      /* WP_U8     rx_direction */0,
      /* WP_handle input_port */0,
      /* WP_handle decomp_group */0,
      /*  pce_if_handle; */ 0
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
      /* in_filter_mode */        WP_IW_INGRESS_FILTER_DISABLE,
      /* vlan_param */
      {
         /* vlan_acceptance_mode */WP_IW_ACCEPT_TAGGED_UNTAGGED,
         /* vlan_tag */0xA,
         /*vlan_tunnel*/WP_IW_VLAN_TUNNEL_1Q_IN_1Q_DISABLE,
         /*vlan_priority_enforce_mode*/WP_IW_VLAN_PRIORITY_ENFORCE_DISABLED,
         /*stag_vlan_etype*/ 0x9100
      },
      /* max_mac_addresses */ 20,
      /* group_tag */ 0,
      /*group_filtering_mode*/     WP_IW_GROUP_FILTER_DISABLE,
//#if 0
      /*unk_mac_sa_filter*/        WP_IW_UNK_MACSA_FILTER_DISABLE,
      /*unk_mc_mode*/              WP_IW_UNK_MC_DROP,
      /*bc_ht_mode*/               WP_IW_BC_HT_DISABLE,
      /*unk_input_filters_mask*/   0,
      /*unk_output_filters_mask*/  0,
      /*statmode*/                 WP_IW_PORT_STAT_ENABLE,
      /*unk_uc_mode*/              WP_IW_UNK_UC_SR_ENABLE,
      /*classification_flag*/      WP_IW_BPORT_CLASSIFICATION_ENABLED,
      /*adv_unk_lookup_mode*/      WP_IW_ADV_UNK_LOOKUP_ENABLED,
      /*cfi_ht_mode*/              WP_IW_CFI_HT_DISABLE,
      /*reserved_mc_ht_mode*/      WP_IW_RES_MC_HT_DISABLE,
      /*predefined_ht_mode*/       WP_DISABLE, /*WP_IW_ARP_HT_ENABLE | WP_IW_IGMP_HT_ENABLE*/
      /*res_mc_action_table 0,*/
//#endif
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
   /* input_filters_mask; */ 2,
   /* output_filters_mask; */ 2,
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
      /* flowmode */ WP_FLOWMODE_FAST  | WP_FLOWMODE_ENET_WRED_ENABLE,
#if defined(VERILOG)
      /* interface_mode */ WP_ENET_GMII,
#elif defined(WINSIM)
      /* interface_mode */ WP_ENET_SGMII_1000,
#else
      /* interface_mode */ WP_ENET_SGMII_1000,
#endif
      /* rx_iw_bkgnd */WP_IW_BKGND_NOT_USED,
   },
};

WP_port_enet enet_x[1] =
{
   {
      /* pkt_limits */  { 5, 5, 0, 0, 0},
      /* flowmode */ WP_FLOWMODE_FAST,
      /* interface_mode */ WP_ENET_XAUI,
      /* rx_iw_bkgnd */WP_IW_BKGND_NOT_USED,
   },
};

/* Configure Utopia port */
WP_port_pkt_limits pkt_limits = {
   /* max_tx_channels */ 1,
   /* max_rx_channels */ 1,
   /* pq_block_size   */ 1,
   /* n_pq_blocks     */ 1,
   /* emphy_devices   */
};

WP_port_upi_packet_ext pkt_ext = {
   /* packet_characteristics */
   /*WP_UPI_POS_CRC_ENABLE,*/ 0,
   /* slow_phy_tx_bitrates */ 0,
   /* packet_limits */ &pkt_limits
};

WP_port_upi upi_pos_s[1] = 
{
   {
      /* upimode */ WP_UPI_POS_HIGHSPEED,
      /* loopback */ WP_UPI_LOOPBACK,
      /* tx_characteristics */ 0,
      /* rx_characteristics */ 0,
      /* atm_ext */ NULL,
      /* packet_ext */ &pkt_ext,
      /* iw_ext */ NULL,
      /* emphy_ext */ 0
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
#ifdef USE_SOCKET
      /* loopbackmode */ WP_ENET_LOOPBACK,
#else
      /* loopbackmode */ WP_ENET_NORMAL,
#endif
      /* extended_params */ NULL,
   }
};

/* Configuration for the HSPOS Device */
WP_device_upi_hspos upi_hspos_device[] =
{
   {
      /* tx_bit_rate;*/ 622000000,
      /* tx_statmode;*/ WP_PKT_STAT_ENABLE,
      /* tx_intmode;*/ WP_PKTCH_INT_DISABLE,
      /* tx_slave_addr;*/ 0,
      /* tx_queuedepth[8];*/ {0},
      /* rx_bit_rate;*/ 622000000,
      /* rx_statmode;*/ WP_PKT_STAT_ENABLE,
      /* rx_intmode;*/ WP_PKTCH_INT_DISABLE,
      /* rx_queuedepth;*/ 2000,
      /* rx_slave_addr;*/ 0,
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

WP_ch_hdlc ch_config_hdlc[1] =
{
   {
      /* intmode */ WP_PKTCH_INT_DISABLE,
      /* iwmode */  WP_PKTCH_IWM_ENABLE,
      /* testmode */ WP_PKTCH_TEST_DISABLE,
      /* tx_pqblock */ 0,
      /* tx_pqlevel */ 0,
      /* tx_shaping_type */ WP_FMU_SHAPING_TYPE_STRICT,
      /* tx_shaping_params */ NULL,
      /* rx_maxsdu */ 1536,
      /* tx_cwid */ WP_CW_ID_A,
      /* tx_tq */ 0
   }
};

#define WT_DEFAULT_CBS (10*1536*8)
#define WT_DEFAULT_EBS (10*1536*8)
WP_fmu_shaping_cir_eir cir_eir_shaping_params[] =
{
   {
      /* cir   */ 900000000,
      /* cbs   */ WT_DEFAULT_CBS,
      /* eir   */ 100000000,
      /* ebs   */ WT_DEFAULT_EBS,
      /* flags */ 0
   }
};

WP_shaping_group_type_packet l1_group_config[] =
{
   {
         /* group_level */     WP_L1_GROUP,
         /* tx_shaping_type */ WP_FMU_SHAPING_TYPE_STRICT,
         /* shaping_params */  NULL,
         /* num_fifos */       WP_NUM_FIFOS_8,
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
#ifdef WP_BOARD_WDS3_SL
   /* type */ WP_POLICER_V2_TYPE_EXTERNAL,
#else
   /* type */ WP_POLICER_V2_TYPE_INTERNAL,
#endif
   /* color_mode */ WP_POLICER_COLOR_AWARE, 
   /* red_packets_action */ WP_POLICER_DROP_RED_PACKETS,
   /* cir */ 1,
   /* cbs */ 2,
   /* eir */ 3,
   /* ebs */ 4,
   /* policer_action */ 0,
   /* frame_len_correction */ WP_POLICER_LENGTH_CORRECT_DISABLE,
};

WP_policer_limits_v2 v2_policer_limits[1]=
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
   WP_ENABLE, /* learning_mode */
   {/* learning_queue_config */
      30, /* learning_queue_size */
      WP_LEARNING_POLLING_MODE, /* interrupt_mode */
      WP_IW_IRQT0, /* int_queue_num */
      30, /* interrupt_rate */
   }
};

WP_pce_filter_classification filter_clas_cfg =
{
   /* tag */ 0,
   /* no_match_action */ WP_PCE_FILTER_NO_MATCH_CONTINUE,
   /* no_fields_action */ WP_PCE_FILTER_NO_FIELDS_CONTINUE,
   /* filter_fields[WP_PCE_MAX_NUM_OF_FIELDS] */ {{WP_PCE_FIELD_ID_LAST}},
   /* no_match_result[WP_PCE_MAX_NUM_OF_FILTER_RESULTS] */ {{WP_PCE_RESULT_LAST}},
};

WP_pce_rule_classification rule_clas_cfg =
{
   /* tag */ 0,
   /* filter_handle */ 0,
   /* rule_fields[WP_PCE_MAX_NUM_OF_FIELDS] */ {{WP_PCE_FIELD_ID_LAST}},
   /* match_action */ WP_PCE_RULE_MATCH_CONTINUE,
   /* enabled */ WP_ENABLE,
   /* match_result[WP_PCE_MAX_NUM_OF_FILTER_RESULTS] */ {{WP_PCE_RESULT_LAST}},
};

WP_pce_filter_set filter_set_cfg =
{
   /* tag */ 0,
   /* filter_set_level */ 0,
   /* filters[WP_PCE_MAX_NUM_OF_FILTERS_IN_SET] */ {WP_UNUSED},
   /* next_filter_set */ WP_UNUSED,
};

WP_pce_if_params_pkt_rx_channel pce_if_params =
{
   /* filter_set_handle */ 0,
   /* parser_start_type */ WP_PCE_PARSER_START_TYPE_HDLC_PPP,
   /* mode */ WP_PCE_IW_PORT_CONNECTION_ENABLED,
   /* ip_header_validation */ WP_DISABLE,
};

WP_pce_filter_stat pce_filter_stats[256];
WP_pce_global_stat pce_global_stats;

WT_Interface global_interfaces[WT_MAX_INTERFACES+1];
WT_TestGlobalInfo global_info;
WP_U32 ClocksPerMsec;
