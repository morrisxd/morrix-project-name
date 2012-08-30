/*****************************************************************************
 * (C) Copyright PMC - WIN division (Wintegra) 2012.  All rights reserved.
 * PMC-Sierra (WINTEGRA) CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of PMC-Sierra (Wintegra).
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/
 
 /*****************************************************************************
 *  Appplication: MLPPP over IP with QoS
 *  File Name   : mlppp_ppp_tdm.h
 *
 *****************************************************************************/
#define DISABLE_TDM_COMET_CARD   (1)
#define APP_VLAN_ID 0xa
#define APP_MAX_MAC_ADDRESS 10

#define APP_MAX_PKTS_NUM 6

/* Enet Bridge ports */
#define APP_MAX_BPORTS       20
#define APP_MAX_TDM_PORTS    2
#define MP_ENABLE            1
#define APP_NUM_IW_BUFFERS   500
#define APP_IW_BUFFER_SIZE   1984
#define APP_IW_BUFFER_OFFSET 64
#define APP_MASTER_WPID WP_WINPATH(0)

#define WP_MP_BUNDLE_STATE_LINK_ACTIVE   1
#define WP_MP_BUNDLE_STATE_LINK_INACTIVE 0
#define NUM_OF_GROUPS     2
#define NUM_OF_CHANNELS   7
#define PQBLOCK_SIZE      4
#define NUM_OF_LINKS    APP_MAX_TDM_PORTS


typedef enum{
   MLPPP_OVER_IP=1,
   PPP_OVER_IP=2,
}PROTOCOL_SUPPORT;

/* Utility macros */
#define MAC_COPY(d,s)           { int _i;             \
                                  for(_i=0;_i<6;_i++) \
                                *((d)+_i)=*((s)+_i); }

extern WP_U32 dps_ProgramImage[];
extern WP_U16 dps_PC_Table_Init[];

WP_context context_cfg =
{
   4, /* qnodes */
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
      { /* ASU_TPT         */ WP_BUS_PARAM, APP_BANK_PARAM},
      { /* ASU CW          */ WP_BUS_PARAM, APP_BANK_PARAM},
      { /* ASU TX QUEUE    */ WP_BUS_PARAM, APP_BANK_PARAM},
      { /* STATISTICS      */ WP_BUS_PARAM, APP_BANK_PARAM},
      { /* ADDRESS LOOKUP  */ WP_BUS_HOST, APP_BANK_HOST},
      { /* ATM CPT         */ WP_BUS_PARAM, APP_BANK_PARAM},
      { /* ENET HDLC CPT   */ WP_BUS_PARAM, APP_BANK_PARAM},
      { /* IW FBP PARAM    */ WP_BUS_PARAM, APP_BANK_PARAM},
      { /* IW FBP DATA     */ WP_BUS_PACKET, APP_BANK_PACKET},
      { /* EXTERNAL FIFO   */ WP_BUS_PACKET, APP_BANK_PACKET},
      { /* MAC LOOKUP      */ WP_BUS_PARAM, APP_BANK_PARAM},
      { /* IMA DISPATCH    */ WP_BUS_PARAM, APP_BANK_PARAM},
      { /* IMA SHARED      */ WP_BUS_PACKET, APP_BANK_PACKET},
      { /* LPM             */ WP_BUS_HOST, APP_BANK_HOST},
      { /* MFC RULES       */ WP_BUS_HOST, APP_BANK_HOST},
      { /* FIWT, QAT       */ WP_BUS_HOST, APP_BANK_HOST},
      { /* MFCPT, UPPPT, DS */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
      { /* STATISTICS2      */ WP_BUS_PARAM, APP_BANK_PARAM},
      { /* STATISTICS3/IMAPORT */ WP_BUS_PARAM, APP_BANK_PARAM},       /* Port      */
      { /* WP_VB_IMA_RX_BUS    */ WP_BUS_PACKET, APP_BANK_PACKET},     /* Rx buffer */
      { /* PKT_GLOBAL_TCPT     */ WP_BUS_PARAM, APP_BANK_PARAM},
      { /* AAL2_QSR/IMATXBUF   */ WP_BUS_PARAM, APP_BANK_PARAM},       /* Tx buffer */
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
      dps_ProgramImage, dps_ProgramImage
   },
   {
      dps_PC_Table_Init, dps_PC_Table_Init
   },
   NULL,              /* Interrupt queue tables */
   0,                 /* Mixumum number of pools */
   NULL,              /* ATM global parameters */
   NULL,              /* Interworking global parameters */
};

WP_int_queue_table int_queue_table_cfg =
{
   {  /* queue[WP_IRQ_TABLES] */
      {2000, 1},  /* size, rate */
      {2000, 1},
      {2000, 1},
      {2000, 1}
   }
};

WP_pool_ring_data pool_ring_data_cfg = 
{
   /* n_rings */ 50,
   /* ring_length */ 30,
   /* bus */ WP_BUS_HOST,
   /* bank */ APP_BANK_PARAM
};

WP_qnode_hostq qnode_hostq_cfg =
{
   /* pool_buffer */ 0,
   /* pool_ring */ 0,
   /* interruptqueue */ WP_IRQT1
};


WP_pool_buffer_data pool_buffer_data_256_cfg = 
{
   /* n_buffers */ 1000,
   /* offset */ 0,
   /* size */ 256,
   /* pad */ 0,
   /* bus */ WP_BUS_PACKET,
   /* bank */ APP_BANK_PACKET,
};

WP_pool_buffer_data pool_buffer_data_iw_cfg = 
{
   /* n_buffers              */ APP_NUM_IW_BUFFERS,
   /* offset                 */ 64,
   /* size                   */ 1984,
   /* pad                    */ 0,
   /* bus                    */ WP_BUS_PACKET,
   /* bank                   */ APP_BANK_PACKET
};

WP_atm_global atm_global_cfg =
{
   /* max_pm_tables          */ 0,              /* Performance monitoring */
   /* max_ubrplus_channels   */ 0,              /* UBR+ channels configured */
   /* max_upc_tables         */ 0,              /* Max policing tables */
   /* max_rx_cids            */ 0,              /* AAL2 max Rx CIDs/channel */
   /* max_tx_cids            */ 0,              /* AAL2 max Tx CIDs/channel */
   /* max_cid_range          */ WP_AAL2_CIDRANGE_UNUSED,
   /* qsr_bus                */ WP_BUS_NONE,    /* AAL2 QSR bus */
   /* qsr_bank               */ 0,              /* AAL2 QSR bus */
   /* atm_modes              */                 /* Atm modes */
   {
      WP_ATM_RAS_TIMESTAMP_ENABLE,
#if _WP_MODULE_ABR_ENABLE_
      0,
#endif
      WP_AAL2_RX_3FE_DISABLE
   },
   /* max_stats_extensions */   0,
};


WP_tdm_slotgroup tdm_slots[] =
{
   {1, 24},
   {0, 0}
};

/* TDM set up */
WP_port_tdm port_tdm_cfg =
{
   /* atm_limits*/ {0},
   /* pkt_limits*/ {64,64,0,0},
   /* trans_limits */ {{0},{0}},
   /* slot */      tdm_slots,
   /* tdmmode */   WP_TDM_NORMAL,
   /* framemode */ WP_TDM_FRAMED,
   /* pinconfig */ WP_TDM_COMMON,
   /* edge */      WP_TDM_RISING,
   /* delay */     WP_TDM_DELAY_1,
   /* intmode */   WP_TDM_INT_DISABLE,
   /* rx_iw_bkgnd */ WP_IW_BKGND_NOT_USED
};


WP_tdm_binding hdlc_binding[] =
{
   {0, 23}
};


WP_device_tdm_hdlc device_tdm_hdlc_cfg = 
{
   /* n_timeslot_binding */ 1,
   /* timeslot_binding */ hdlc_binding,
   /* tx_statmode */ WP_PKT_STAT_ENABLE,
   /* tx_maxsdu */ 1536,
   /* rx_statmode */ WP_PKT_STAT_ENABLE,
   /* crctype */ WP_HDLC_CRC16,
   /* numflags */ 2,
   /* idlemode */ WP_TDI_IDLEMODE_FLAG,
   /* flowmode */ WP_FLOWMODE_MULTI,
   /* addr_mask */ 0,
   /* address1 */ 0,
   /* address2 */ 0,
   /* tx_tqtypes */ { WP_PKT_SCHED_WFQ,
                      WP_PKT_SCHED_WFQ,
                      WP_PKT_SCHED_WFQ,
                      WP_PKT_SCHED_WFQ
   }
};

WP_port_enet port_enet_cfg =
{
   /* pkt_limits */
   {
      /* max_tx_channels*/ 8,
      /* max_rx_channels*/ 8,
      /* pq_block_size*/   0,
      /* n_pq_blocks*/     0,
      /* emphy_devices*/   0,
   },
   /* flowmode */       WP_FLOWMODE_FAST,
#if 0
   /* interface_mode */ WP_ENET_SGMII_1000,
#else
   /* interface_mode */ WP_ENET_1000_BASE_X_AN,
#endif
   /* rx_iw_bkgnd */    WP_IW_BKGND_NOT_USED,       
};


WP_enet_filters enet_filters_cfg = 
{
   /*rx_ucfilter;*/WP_ENET_FILTER_PROCESS,
   /*rx_bcfilter;*/WP_ENET_FILTER_HOSTTERM,
   /*rx_mcfilter;*/WP_ENET_FILTER_HOSTTERM,
};

WP_device_enet device_enet_cfg = 
{
   /* max_tx_channels */ 8,
   /* tx_maxsdu       */ 1536,
   /* rmii_operating_speed */ WP_UNUSED,
   /* mac_addr[6] */     {0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa},
   /* tx_bit_rate */     1000000000,
   /* loopbackmode  */   WP_ENET_NORMAL,
   /* extended_params */ NULL,
};

WP_pkt_shaping_wfq default_pkt_shaping_wfq_cfg = 
{
   /* weight;  */1,
   /* weight_fraction;*/0
};


WP_ch_enet ch_enet_cfg = 
{
   /* intmode */ WP_PKTCH_INT_ENABLE,
   /* iwmmode */ WP_PKTCH_IWM_ENABLE,
   /* testmode */ WP_PKTCH_TEST_DISABLE,
   /* tx_pqblock */ 0,
   /* tx_pqlevel */ 0,
   /* tx_shaping_type */ WP_FMU_SHAPING_TYPE_STRICT,
   /* tx_shaping_params */ NULL,
   /* rx_maxsdu */ 1536,
   /* tx_cwid */ WP_CW_ID_A,
   /* tx_tq */ 0,
   /* rx_queuedepth */17,
};

WP_pkt_shaping_cir pkt_shaping_cir_cfg = 
{
   /* cir */ 11000,
   /* polling */ WP_SHAPING_POLL_AUTOD,
   /* cbs */ 0,
};

/* Configuration structure for the HDLC channels*/
WP_ch_hdlc ch_hdlc_cfg = 
{
   /* intmode */ WP_PKTCH_INT_DISABLE,
   /* iwmmode */  WP_PKTCH_IWM_ENABLE,
   /* testmode */ WP_PKTCH_TEST_DISABLE,
   /* tx_pqblock */ 0,
   /* tx_pqlevel */ 0,
   /* tx_shaping_type */ WP_PKT_SHAPING_WFQ,
   /* tx_shaping_params */ &default_pkt_shaping_wfq_cfg,
   /* rx_maxsdu */ 1536,
   /* tw_cwid */ WP_CW_ID_A,
   /* tx_tq */ 0
};

WP_calendar_wheel cw_config[2] =
{
   {
      /* function */ WP_CW_PSU,
      /* cwid */ WP_CW_ID_A,
      /* min_ch_speed */ 10000,
      /* max_ch_speed */ 30000
   },
   WP_CALENDAR_WHEEL_TERM
};

WP_iw_global iw_global_cfg =
{
   /* max_iw_aggregations    */ 200,
   /* max_routing_systems    */ 0,
   /* max_directmap_ene...   */ 1,
   /* max_bridging_systems   */ 2,
   /* max_mpls_systems       */ 0,
   /* max_vlan_priority...   */ 0,
   /* iw_bkgnd_fifo_size     */ 1000,
   /* cong_pt                */ NULL,
   /* iw_host_limits         */ {10, 10, 0,0 },
   /* mpls_config            */ {0, 0},
   /* iw_modes               */ {
                                   WP_IW_POLICER_ENABLE,
                                   WP_IW_STATISTICS_BUNDLE_STAT_ENABLE,
                                   WP_IW_L2_HEADER_EXTENSION_ENABLE,
                                   WP_IW_ENHANCED_FLOW_STAT_ENABLE,
                                   WP_IW_FLOW_STAT_ENABLE,
                                   WP_IW_FR_TX_FLOW_STAT_DISABLE,
                                   0,
                                   WP_IW_LEARNING_DISABLE,
   /* port_filtering_mode; */      WP_IW_PORT_FILTERING_ENABLE,
   /* lpm_alloc_mode; */ 0,
   /* enhanced_mc_member_stat_mode; */ 0,
                                   0,
                                   1,
                                 },
   /* max_iw_stat_bundles     */ 0,
   /* max_fr_sw_systems       */ 0,
   /* max_bridging ports      */ APP_MAX_BPORTS,
   /* max_iw_mc_groups        */ 0,
   /* max_nat_systems         */ 0,
   /* max_iw_second_agg       */ 0,
   /* max_iw_compr_limits     */ 0,
   /* WP_U32 max_iw_mc_members */ 0,
   /* WP_U16 max_iw_programmable_filters */ 4, 
   /* WP_U16 max_iw_policers */ 4, 
};

WP_tx_binding tx_binding_cfg =
{
   /* res0                      */ 0,
   /* dci_mode                  */ WP_IW_DYN_CH_INSERT_ENABLE,
   /* maxt                      */ 255,//10,
};

WP_qnode_iwq qnode_iwq_cfg =
{
   /* interruptqueue         */ WP_IRQT1,
   /* num_buffers            */ 1000,
   /* adjunct_pool           */ 0,
};

/* INTERWORKING BRIDGING */
WP_ch_iw_rx ch_iw_rx_cfg =
{
   /* pqblock            */ 0,    
                                                           
   /* pqlevel            */ 0,  
                                                     
   /* tx_binding_type    */ WP_IW_TX_BINDING,  
                                                  
   /* tx_binding_config  */ NULL,               
};

WP_rx_binding_bridging rx_binding_bridging_cfg = 
{
   /* encap_mode                */ WP_IW_VCMUX,
   /* mru                       */ 1536,
   /* vcfcs                     */ 0,
   /* input_port                */ 0,
};

WP_iw_agg_bridging default_iw_agg_bridging_cfg = 
{
   /* tag                       */ 0,
   /* txfunc                    */ 0,
   /* bport                     */ 0,
   /* rfcs                      */ WP_IW_RFCS_ENABLE,
   /* l2_header_insert_mode     */ WP_IW_L2H_INSERT_DISABLE,
   /* vlan_tag_mode             */ WP_IW_VLAN_TAG_DISABLE,
   /* interruptqueue            */ WP_IW_IRQT1,
   /* error_pkt_mode            */ WP_IW_ERRPKT_DISCARD,
   /* intmode                   */ WP_IW_INT_ENABLE,
   /* statmode                  */ WP_IW_STAT_ENABLE,
   /* timestamp_mode            */ WP_IW_TIME_STAMP_DISABLE,
   /* ov_pool_mode              */ WP_IW_OV_POOL_DISABLE,
   /* fbp_drop_threshold        */ 0,
   /* replace_vlan_tag          */ WP_IW_REPLACE_VTAG_DISABLE,
   /* vlan_id                   */ 0,
   /* vpmt_handle               */ 0,
   /* mtu                       */ 1536,
   /* prefix_length             */ 0,
   /* prefix_header[28]         */ {0},
   /* policer_enable            */ WP_IW_POLICER_DISABLE,
   /* policer_config            */ NULL,
   /* cong_mode                 */ WP_IW_CONGESTION_DISABLE,
   /* cong_threshold_param      */ NULL ,
   /* flow_agg_type             */ WP_IW_FLOW_AGG_PRIMARY ,
   /* external_vlan */ {
                          WP_IW_EXTERN_VLAN_NONE,
                          0
                       },
   /* bc_mc_priority_mapping */ {
                                   WP_IW_BC_PRIORITY_DISABLE,
                                   0,
                                   WP_IW_MC_PRIORITY_DISABLE,
                                   0
                                },
   /* reserved_agg_mode */ WP_IW_NOT_RESERVED,
   /* next_preserve_inport */ WP_IW_OVERRIDE_INPORT,
   /* next_preserve_inport_params */ 0,
   /* mac_replace_mode */ WP_IW_MAC_REPLACE_DISABLED,
   /* new dst mac */  {0},
   /* new_src_mac */ {0},
   /* multi_cong_prof */ 0,
   /* extraction_length */ 0,
};

WP_dfc_sys_info dfc_sys_info =
{
	 /* classification_hash_size */  WP_IW_32K_HASH_ENTRIES,
	 /* port_filtering */            WP_IW_PORT_FILTERING_INPUT,
	 /* deny_eq_ip */                WP_IW_DENY_EQUAL_IP_DISABLE
};


WP_iw_sys_bridging iw_sys_bridge = 
{
   /* tag                       */ 0,
   /* max_flows                 */ 15, 
   /* classification mode       */ WP_IW_CLASSIFIER_BASED_BRIDGING,
   /* classifier config         */ {WP_IW_DYNAMIC_CLASS,100,0,NULL},
   /* learning_mode             */ WP_IW_BRIDGE_LEARNING_DISABLE,
   /* learning queue            */ {
                                      0,
                                      0,
                                      0,
                                      1,
                                   },
   /* forwarding_table_size     */ WP_IW_32K_HASH_ENTRIES,
   /* member_set_size           */ WP_IW_32K_HASH_ENTRIES,
   /* buffer_gap                */ 0x40,
   /* max_bridging_ports        */ APP_MAX_BPORTS,
   /* dfc_info                  */ &dfc_sys_info,
   /* svl_mode */ WP_IW_SVL_DISABLED,
 
 	/*stag_ether_type*/                 WP_IW_SYS_BRIDGE_STAG_VLAN_ETYPE_DISABLE,
	/*parsing_mode;*/                   0,
	/*dfc_log_mode;*/                   0
 
};



WP_bridge_port bridge_port_cfg = 
{
   /* tag                       */ 0,
   /* direct_mapping            */ WP_IW_DIRECT_MAP_DISABLE,
   /* flow_agg                  */ 0,
   /* termination_mode          */ WP_IW_HOST_TERM_MODE,
   /* learning_mode             */ WP_IW_LEARNING_DISABLE,
   /* in_filter_mode            */ WP_IW_INGRESS_FILTER_DISABLE,
   /* vlan_param                */ {
                                      WP_IW_ACCEPT_TAGGED_UNTAGGED,
                                      0,
                    /* vlan tunnel */ 0,
      /* vlan_priority_enforcement */ WP_IW_VLAN_PRIORITY_ENFORCE_DISABLED,
                                   },
   /* max_mac_addresses         */ APP_MAX_MAC_ADDRESS,
   /* group_tag                 */ 0,
   /* group_filtering_mode      */ WP_IW_GROUP_FILTER_DISABLE,
   /*unk_mac_sa_filter;*/0,
   /*unk_mc_mode;*/0,
   /*bc_ht_mode;*/0,
   /*input_filters_mask;*/0,
   /*output_filters_mask;*/0,
   /*statmode;*/ WP_IW_PORT_STAT_ENABLE,
   /* unk_uc_mode;*/0,
   /* classification_flag;*/ WP_IW_IWPORT_CLASSIFICATION_ENABLED,
   /* adv_unk_lookup_mode;*/ 0,
   /* cfi_ht_mode;*/ 0,
   /* reserved_mc_ht_mode;*/ WP_IW_RES_MC_HT_ENABLE,
};


WP_iw_sys_directmap iw_sys_pppsw_cfg = 
{
  /*  max_flows */  2*APP_MAX_TDM_PORTS,     
  /*  buffer_gap */ 0x40
};

WP_iw_agg_directmap iw_agg_directmap_cfg  = 
{
   /* tag */                  0,
   /* txfunc */               0,
   /* rfcs */                 WP_IW_RFCS_DISABLE,
   /* l2header_insert_mode */ WP_IW_L2H_INSERT_DISABLE,
   /* interruptqueue */       WP_IW_IRQT1,
   /* error_pkt_mode*/        WP_IW_ERRPKT_DISCARD,
   /* intmode */              WP_IW_INT_DISABLE,
   /* statmode */             WP_IW_STAT_ENABLE,
   /* timestamp_mode */       WP_IW_TIME_STAMP_DISABLE,
   /* ov_pool_mode*/          0,
   /* fbp_drop_threshold */   0,
   /* mtu */                  1536,
   /* prefix_length */        0,
   /* extraction_length */    0,
   /* prefix_header[36] */    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                               0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                               0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                               0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                               0x00, 0x00, 0x00, 0x00},
   /* policer_enable */       0,
   /* policer_config */       NULL,
   /* cong_mode */            0,
   /* cong_threshold_param */ NULL
};


WP_iw_agg_ppp_switching iw_agg_ppp_switching  = 
{
   /* tag */                  0,
   /* txfunc */               0,
   /* l2header_insert_mode */ WP_IW_L2H_INSERT_DISABLE,
   /* interruptqueue */       WP_IW_IRQT1,
   /* error_pkt_mode*/        WP_IW_ERRPKT_DISCARD,
   /* intmode */              WP_IW_INT_DISABLE,
   /* statmode */             WP_IW_STAT_ENABLE,
   /* timestamp_mode */       WP_IW_TIME_STAMP_DISABLE,
   /* ov_pool_mode*/          0,
   /* fbp_drop_threshold */   0,
   /* mtu */                  1536,
   /* prefix_length */        0,
   /* extraction_length */    0,
   /* prefix_header[36] */    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                               0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                               0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                               0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                               0x00, 0x00, 0x00, 0x00},
   /* policer_enable */       0,
   /* policer_config */       NULL,
   /* cong_mode */            0,
   /* cong_threshold_param */ NULL,
   /* flow agg type */ WP_IW_AGG_PRIMARY,
   /* acfc_mode */ WP_IW_ACFC_ENABLE,
   /* pfc_mode */ WP_IW_PFC_ENABLE,
   /* policer handle */ 0,
   /* iwport */ 0,
};

/* Sturctures used for MLPPP */
/*  Structure for configuring the MLPPP system  */
WP_mp mp_sys_cfg[1] =
{
      {
         /* n_winpaths */ 1,
         /* n_bundles */  NUM_OF_GROUPS,
         /* n_links */    NUM_OF_LINKS,
         /* tx_buffer_size */         1550,
         /* tx_n_buffers */           512,  
         /* tx_n_bds */               512,  
         /* rx_max_bds_per_bundle */ 256,
         /* rx_n_buffers */          256*2, 
         /* rx_buffer_size */         1536,
         /* rx_iw_bkgnd */ WP_IW_BKGND_USED,
         /* aggregate_data_rate */    2000000,
         /* min_packet_size */       30,
         /* pkt_limits */ { 1024, NUM_OF_GROUPS, PQBLOCK_SIZE, NUM_OF_GROUPS, 0},
      }
};

/*  Structure for configuring the MLPPP bundle  */
 WP_device_mp_bundle mp_config[1] =
   {
      {
         /* tag */                  0,
         /* rx_n_seq_bds */         256,
         /* mrru */                 1500,
         /* long sequence */        WP_MP_LONG_SEQUENCE,
         /* filler frequency*/      0,
         /* tx_maxsdu */            1536,
         /* flowmode */             WP_FLOWMODE_MULTI,
         /* tx_tq_types */          {WP_PKT_SCHED_WFQ,
                                     WP_PKT_SCHED_WFQ,
                                     WP_PKT_SCHED_WFQ,
                                     WP_PKT_SCHED_WFQ},
         /* statmode */ WP_PKT_STAT_ENABLE,
         /* qn_mp */ 0,
         /* buffergap */ 0x40,
         /* tx_frag_disable */ WP_MP_FRAG_DISABLE,
      }
   };


/*  Structure for configuring the MLPPP hdlc channel */
  WP_pkt_shaping_wfq wfq[1]=
   {
      {
         /* weight;  */1,
         /* weight_fraction;*/0
      }
   };

   WP_ch_hdlc ch_config_hdlc[1] =
   {
      {
         /* intmode */ WP_PKTCH_INT_DISABLE,
         /* iwmmode */  WP_PKTCH_IWM_ENABLE,
         /* testmode */ WP_PKTCH_TEST_DISABLE,
         /* tx_pqblock */ 0,
         /* tx_pqlevel */ 0,
         /* tx_shaping_type */ WP_PKT_SHAPING_WFQ,
         /* tx_shaping_params */ wfq,
         /* rx_maxsdu */ 1536
      }
   };

/*  Structure for configuring the MLPPP links */
WP_mp_link mp_link_config[1] =
   {
      {
         /* frag_size */    100,
         /* hdlc_q_depth */ 0,
         /* tx_bd_depth */  2,
         /* tx_acfc */      WP_MP_LINK_TX_NO_ACFC,
         /* tx_pfc */       WP_MP_LINK_TX_NO_PFC,
         /* tx_statmode */  WP_PKT_STAT_ENABLE,
      }
   };

/*  Structures for configuring policer for MLPPP */
WP_policer_slb_config slb_config[1] =
{
 {
    /* leaky_bucket_size */ 100,
 }
};



WP_policer_limits_v1 v1_limit = 
{
	/* max_policers */ 2,
	/* tcm_policer_limits */ {0,0},
};

WP_module_policer policer[1] = 
{
	{
	   /* v2_policer_limits */ 0,
	   /* v1_policer_limits */ &v1_limit,
	   /* atm_policer_limits */ 0,
	   /* mode */ 0,
	}
};

/*-------------------------------------*/
/* V1 policer configuration parameters */
/*-------------------------------------*/
WP_policer_v1 policer_config[1] =
{	
	{
		/* *leaky_bucket*/  &slb_config[0], /*WP_policer_dlb_config
		                          		or
		                 			WP_policer_slb_config*/
		/*committed_info_rate*/ 30000000,   // for different data rate this field need to be changed
		/* policer_mode*/	WP_IW_DISCARD_NONCONFORMING_PKT,
		/* tx_handle*/		 0,
		/* leaky_bucket_mode*/ WP_IW_SINGLE_LEAKY_BUCKET,
		/* frame_len_correction*/	WP_IW_PLC_FRM_LEN_CORRECT_DISABLE,
		/* *tcm_config*/		NULL,
	}
} ;


/*
 * Global variables used during test execution
 */
WP_U8 wt_mac_enet7[6]    = {0x45, 0x6e, 0x65, 0x74, 0x23, 0x31};
WP_U8 wt_mac_enet8[6]    = {0x45, 0x6e, 0x65, 0x74, 0x23, 0x32};
WP_tag tag_channel_base = 1;

WP_flow_learning_rule_vlan bridge_cfg;
WP_l2_forward forward_rule;

/* Interrupt handling structures */
typedef struct {
   WP_tag event_tag;
   WP_U32 event_type;
   void *event_param;
} app_task;

typedef struct {
   WP_U16 head;
   WP_U16 tail;
   WP_U16 num_elements;
   WP_U16 pad;
   app_task *task;
} app_task_list;

/* Interrupt task list */
#define IRQ_TASK_LIST_SIZE 4096
app_task irq_task[IRQ_TASK_LIST_SIZE];
app_task_list irq_task_list[1] = { { 0, 0, IRQ_TASK_LIST_SIZE, 0, irq_task} };

/* Application prototypes */
void App_Quit(WP_U32 status);
void App_EventRxIndicate(WP_tag tag, WP_U32 info, WP_U32 data);
void App_EventTxIndicate(WP_tag tag, WP_U32 info, WP_U32 data);
void App_EventErrorIndicate(WP_tag tag, WP_U32 info, WP_U32 data);
void App_ShowStats(void);
void App_TestInit(void);
void App_InitHW(void);
void App_SystemSetup(void);
void App_ChannelsCreate(void);
void App_PortsDevicesEnable(void);
void App_ChannelsEnable(void);
void App_ShowMLPPPBundleStats(void);
void App_Debug(WP_boolean debug);
void App_TerminateOnError(WP_handle handle, WP_CHAR *s);
void App_CreateEnetHdlcBridge(void);
void App_CreateHdlcEnetBridge(void);
void App_DuReceive(WP_handle h_rx, WP_U32 data_type);
void App_DataSend(WP_handle h_dev, WP_handle h_pool);
void WPI_SimulateInterrupts(void);

app_task *next_task(app_task_list *task_list, app_task *result);
void add_task(app_task_list *task_list, WP_U32 event_type, WP_tag tag,
              void *event_param);
void app_perform_action(app_task *task);
static void App_perform_freerun(void);
void App_CheckStats(WP_status *test_result);

#define ENET_HEADER_SIZE 14
#define IP_HEADER_SIZE 20
/* Byte offsets from start of IP header */
#define IP_VER_HLEN    0
#define IP_SER_TYPE    1
#define IP_LENGTH      2
#define IP_TTL         8
#define IP_PROTOCOL    9
#define IP_CHECKSUM    10
#define IP_SA          12
#define IP_DA          16

#define UDP_HEADER_SIZE 8
#define UDP_SRC_PORT 0
#define UDP_DST_PORT 2
#define UDP_LENGTH 4
#define UDP_CHECKSUM 6

#define TRAFFIC_DATA '1'
#define CONTROL_DATA '2'

/* Functions used for forming IP header  */
void insert_ip_header(WP_U32 ip_sa, WP_U32 ip_da, WP_U32 payload_size, 
                      WP_U32 protocol, WP_U32 length, WP_U8 *prefix);

/* ETHERNET */
WP_handle               h_port_enet1, h_port_enet2;
WP_handle               h_device_enet1, h_device_enet2;
WP_handle h_port_tdm[APP_MAX_TDM_PORTS];
WP_handle h_dev_tdm[APP_MAX_TDM_PORTS];

/* INTERWORKING COMMON */
WP_handle               h_pool_buffer_host;
WP_handle               h_pool_ring_host;
WP_handle               h_pool_buffer_iw,h_pool_256;
WP_handle               h_qnode_iwq, h_qnode_host,h_qnode_iwq_tdm;
WP_handle               h_bridge1_iwhost, h_bridge2_iwhost;
WP_handle               h_bridge1_default_agg,h_bridge2_default_agg;
WP_handle               h_flow_agg_tdm[APP_MAX_TDM_PORTS];
WP_handle               h_iw_sys_bridge1, h_iw_sys_bridge2;
WP_handle               h_iw_sys_pppsw;
WP_handle               h_learningflow_enet1;
WP_handle               h_bridge1_dfcflow1;
WP_handle               h_bridge2_dfcflow[4];
WP_handle               h_bridge1_filter[2],h_bridge2_filter[4];
WP_handle               h_port_iwhost,h_dev_iwhost;
WP_handle               h_bridge1_bport_enet1, h_bridge1_bport_tdm[APP_MAX_TDM_PORTS];
WP_handle               h_bridge2_bport_enet2, h_bridge2_bport_tdm[APP_MAX_TDM_PORTS];
WP_handle               h_flow_agg_pppsw_link[APP_MAX_TDM_PORTS];
WP_handle               h_flow_agg_lcp;
WP_handle               h_rport_tdm[APP_MAX_TDM_PORTS];
WP_handle               h_enet1_rx, h_enet1_tx;
WP_handle               h_enet2_rx, h_enet2_tx;
WP_handle               h_tdm_rx[APP_MAX_TDM_PORTS],h_tdm_tx[APP_MAX_TDM_PORTS];

WP_handle status;

/* MLPPP related globals */ 
WP_handle mlppp_sys_handle;
WP_handle mlppp_group_handle[NUM_OF_GROUPS];
WP_handle mlppp_link_handle[NUM_OF_LINKS];
WP_handle mlppp_bundle_pqblock_handle[NUM_OF_GROUPS];
WP_handle mlppp_qn;
WP_handle mlppp_tx_ch_handle[NUM_OF_CHANNELS];
WP_handle mlppp_rx_ch_handle;
WP_handle h_bridge1_filter2;
WP_handle h_bridge1_mlppp_agg;
WP_handle h_bridge1_bport_mlppp;
WP_handle h_bridge2_bport_mlppp;
WP_handle h_flow_agg_mlppp1,h_flow_agg_mlppp2,h_flow_agg_pppsw_link_mlppp, h_flow_agg_enet2_mlppp;
WP_handle h_flow_agg_mlppp_control;
WP_handle h_bridge1_dfcflow_mlppp[PQBLOCK_SIZE];
WP_handle h_enet2_tx_mlppp[PQBLOCK_SIZE];
WP_handle h_bridge2_dfcflow_mlppp_control;
WP_handle h_bridge2_dfcflow_mlppp_traffic[PQBLOCK_SIZE];
WP_handle policer_handle;
WP_tag    tag_bridge1_bport_mlppp;
WP_tag    tag_bridge2_bport_mlppp;
WP_tag    tag_agg_mlppp, tag_agg_eth2bridge_mlppp,tag_agg_mlppp_ipcp;
WP_tag    tag_agg_pppsw_mlppp;
WP_tag    tag_enet2_tx_mlppp[PQBLOCK_SIZE];
WP_U32    QoS_ENABLE=1;

/* TAGS */
WP_tag tag_agg_enet;

WP_tag tag_bport_enet1;
WP_tag tag_bport_enet2;
WP_tag tag_bridge1_bport_tdm[APP_MAX_TDM_PORTS];
WP_tag tag_bridge2_bport_tdm[APP_MAX_TDM_PORTS];
WP_tag tag_rport_tdm[APP_MAX_TDM_PORTS];
WP_tag tag_agg_enet2;

WP_tag tag_iw_sys_bridge1, tag_iw_sys_bridge2;
WP_tag tag_agg_default_bridge1, tag_agg_default_bridge2;

WP_tag tag_enet1_rx, tag_enet2_rx;
WP_tag tag_enet1_tx, tag_enet2_tx;
WP_tag tag_tdmrx = 200;
WP_tag tag_tdmtx = 300;
WP_tag tag_agg_tdm[APP_MAX_TDM_PORTS];
WP_tag tag_agg_pppsw[APP_MAX_TDM_PORTS];
WP_tag tag_agg_ppp_lcp, tag_agg_ppp_traffic;
WP_tag tag_agg_mlppp_control, tag_agg_mlppp_traffic;
WP_boolean debug_on = WP_FALSE;

WP_CHAR interface_ch = 'X';
WP_U8   freerun = 0;
WP_U8   caseindex;

WP_iw_agg_ppp_switching iw_agg_ppp_switching_tdm[APP_MAX_TDM_PORTS];
WP_iw_agg_ppp_switching iw_agg_ppp_switching_link;
WP_iw_agg_bridging iw_agg_bridging_cfg_tdm[APP_MAX_TDM_PORTS];
WP_iw_agg_bridging iw_agg_mlppp;


/******************************************************************************/
