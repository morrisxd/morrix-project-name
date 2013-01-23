#ifndef __BP_H__
#define __BP_H__

#define WT_TRIAL_MASK   (0x04)
#define DEFAULT_WPID    (0)
#define ADDED_BY_MORRIS
#define SINGLE_IMAGE 1
extern WP_U32 dps_ProgramImage[];
extern WP_U16 dps_PC_Table_Init[];
extern WP_U32 dps_ProgramImage_WinFarm_0[], dps_ProgramImage_WinFarm_1[];
extern WP_U16 dps_PC_Table_Init_WinFarm_0[], dps_PC_Table_Init_WinFarm_1[];

extern void App_CLI (void);
void App_perform_freerun (void);

WP_CHAR appname[] = "CBFC test";

#define DEBUG 0
#define PCE_DEBUG 1

#define MAX_LPM_FLOWS 5000
#define WT_IPV4_HEADER_SIZE   20
#define WT_L4_HEADER_SIZE     8
#define WT_MAC_HEADER_SIZE    14
#define WT_IPV4_SA_OFFSET     12
#define WT_IPV4_DA_OFFSET     16
#define WT_IPV4_PROTOCOL_OFFSET 9
#define WT_L4_DST_PORT_OFFSET 2
#define WT_L4_SRC_PORT_OFFSET 0
#define WT_MAC_DA_OFFSET      0
#define WT_MAC_SA_OFFSET      6
#define TEST_LOOPBACK_MODE 1
#define WTI_REGRESSION_MODE 1

#define NUM_DATA_FILTERS 1
#define NUM_FLOW_CTRL_FILTERS 8

#define WT_RX_LOOP_LIMIT 64

#define MAX_IW_POLICERS 50
#define MAX_FLOW_AGG 60
#define POOL_LIMIT 20
#define MAX_IW_PROGRAMMABLE_FILTERS 5

#define NUM_OF_FLOWS 8

#define NUM_OF_CHANNELS NUM_OF_FLOWS
#define NUM_PQ_BLOCK 1

#define MAX_NUM_OF_CHANNELS NUM_OF_CHANNELS
#define NUM_OF_PACKETS_PER_CHANNEL  1
#define MAX_NUM_OF_PACKETS (MAX_NUM_OF_CHANNELS * NUM_OF_PACKETS_PER_CHANNEL)

/* IWF <Src IP, Dest IP, Src Port, Dst Port, IPv4 protocol> -->
       <Src IP, Dst IP, Src Port, Dst Port, IPv4 protocol, MAC Dst last byte> */
static WP_U32 IWF_Table[NUM_OF_FLOWS * 2][11] = {
    /* 0xC0A80101 = 192.168.1.1 */
    {0xC0A80101, 0xC0A8010A, 200, 200, 0x11, 0x0A010101, 0x0A01010A, 100, 100, 0x11, 0x06},
    {0xC0A80102, 0xC0A8010A, 200, 200, 0x11, 0x0A010102, 0x0A01010A, 100, 100, 0x11, 0x06},
    {0xC0A80103, 0xC0A8010A, 200, 200, 0x11, 0x0A010103, 0x0A01010A, 100, 100, 0x11, 0x06},
    {0xC0A80104, 0xC0A8010A, 200, 200, 0x11, 0x0A010104, 0x0A01010A, 100, 100, 0x11, 0x06},
    {0xC0A80105, 0xC0A8010A, 200, 200, 0x11, 0x0A010105, 0x0A01010A, 100, 100, 0x11, 0x06},
    {0xC0A80106, 0xC0A8010A, 200, 200, 0x11, 0x0A010106, 0x0A01010A, 100, 100, 0x11, 0x06},
    {0xC0A80107, 0xC0A8010A, 200, 200, 0x11, 0x0A010107, 0x0A01010A, 100, 100, 0x11, 0x06},
    {0xC0A80108, 0xC0A8010A, 200, 200, 0x11, 0x0A010108, 0x0A01010A, 100, 100, 0x11, 0x06}
#ifndef ADDED_BY_MORRIS
    {0xC0A8010E, 0xC0A8010A, 200, 200, 0x11, 0x0A01010E, 0x0A01010A, 100, 100, 0x11, 0x06},
    {0xC0A8010F, 0xC0A8010A, 200, 200, 0x11, 0x0A01010F, 0x0A01010A, 100, 100, 0x11, 0x06}
#endif
};

#define WT_MAX_FRAME_SIZE         (1536 + 2*512)

#define MAX_DATA_LENGTH   1024
#define APP_HOST_BUFFER_SIZE 1984

// static WP_U8 logTraceLevel = WPI_DEBUG_LEVEL_10;

typedef union StatField
{
    WP_U64 field;
    struct
    {
        WP_U32 high;
        WP_U32 low;
    } part;
} WPT_StatField;

WP_pce_parser_info pce_parser_info;

WP_module_pce_init pce_init = {
    &pce_parser_info,           /* parser_info */
    150,                        /* hardware_rules_num */
    15,                         /* filters_num */
    10,                         /* pkt_rx_channel_interfaces_num */
    5,                          /* iw_tx_channel_interfaces_num */
    WP_PCE_DISCARD_MODE_ALL,    /* pce_discard_mode */
    WP_ENABLE,                  /* bridge_loop_prevention_mode */
    WP_ENABLE,                  /* learning_mode */
    {                           /* learning_queue_config */
     30,                        /* learning_queue_size */
     WP_LEARNING_POLLING_MODE,  /* interrupt_mode */
     WP_IW_IRQT0,               /* int_queue_num */
     30,                        /* interrupt_rate */
     }
};

WP_int_queue_table int_queue_tables = { {{24, 1}, {10, 1}, {10, 1}, {10, 1}} };

WP_fmu_shaping_cir_eir l1_group_shaping_params[1];
WP_fmu_shaping_cir_eir l2_group_shaping_params[1];
WP_shaping_group_type_enet packet_group_l1_config[1];
WP_shaping_group_type_enet packet_group_l2_config[1];

WP_iw_global max_iw[1] = {
    {
     /* max_iw_aggregations */ MAX_FLOW_AGG,
     /* max_r_iw_systems; */ 3,
     /* max_directmap_enet_systems; */ 0,
     /* max_b_iw_systems; */ 2,
     /* max_m_iw_systems; */ 0,
     /* max_vlan_priority_maps; */ 0,
     /* iw_bkgnd_fifo_size */ 0,
     /* cong_pt       */ NULL,
     /* iw_host_limits */ {64, 0, 0, 64, 0},
     /* mpls_config */ {0, 0},
     /* iw_modes; */
     {
      /*policer_mode; */ WP_IW_FLOW_POLICER,
      /*statistics_bundle_stat_mode; */ WP_IW_STATISTICS_BUNDLE_STAT_DISABLE,
      /*l2_prefix_ext_mode; */ WP_IW_L2_HEADER_EXTENSION_ENABLE,
      /*enhanced_flow_stat_mode; */ WP_IW_ENHANCED_FLOW_STAT_DISABLE,
      /*flow_stat_mode; */ WP_IW_FLOW_STAT_ENABLE,
      /*fr_tx_flow_stat_mode */ WP_IW_FR_TX_FLOW_STAT_DISABLE,
      /*mfc_alloc_mode; */ WP_IW_MFC_ALLOC_ENABLE,
      /*Learning_queue_mode */ WP_IW_LEARNING_DISABLE,
      /*port_filtering_mode */ WP_IW_PORT_FILTERING_DISABLE,
      /* lpm_alloc_mode */ WP_IW_LPM_ALLOC_DISABLE,
      /*enhanced_mc_member_stat_mode */ WP_DISABLE,
      /*routing_alloc_modes */ WP_DISABLE,
      /*res_mc_addresses_table_mode */ WP_DISABLE,
      /*port_stag_etype_mode */ WP_IW_PORT_STAG_ETYPE_ENABLE
      },
     /* max_ch_agg_stat; */ 0,
     /* max_fr_s_iw_systems; */ 0,
     /*max_iw_ports; */ 100,
     /* max_iw_mc_groups; */ 0,
     /* max_nat_systems; */ 0,
                                            /*max_iw_second_aggregations; */ 0,
                                            /* secondary flow aggregations */
     /*iw_comp_limits; */ 0,
     /* max_iw_mc_members; */ 0,
     /* max_iw_programmable_filters; */ MAX_IW_PROGRAMMABLE_FILTERS,
     /* max_iw_policers; */ MAX_IW_POLICERS,
     /* max_dscpmt */ 0,
     /* max_wred_bindings */ 0,
     /* tcm_policer_limits */
     {
      MAX_IW_POLICERS,
      MAX_IW_POLICERS}

     }
};

#ifndef WP_BOARD_WDS3_SL
WP_context context[1] = {
    {
     3,                         /* qnodes */
     /* the first four assignments are mandatory bus assignments */
     {
      /*WP_VB_PARAM = 0,           */ {WP_BUS_PARAM, APP_BANK_PARAM},
      /*WP_VB_PACKET,          */ {WP_BUS_PACKET, APP_BANK_PACKET},
      /*WP_VB_INTERNAL,            */ {WP_BUS_INTERNAL, APP_BANK_INTERNAL},
      /*WP_VB_HOST,                */ {WP_BUS_HOST, APP_BANK_HOST},
      /*WP_VB_INTQUEUE_DEBUG,      */ {WP_BUS_HOST, APP_BANK_HOST},
      /*WP_VB_TIMESTAMP,           */ {WP_BUS_HOST, APP_BANK_HOST},
      /*WP_VB_CAS,                 */ {WP_BUS_NONE, 0},
      /*WP_VB_TRANS_TSAT,          */ {WP_BUS_NONE, 0},
      /*WP_VB_TPT,                 */ {WP_BUS_INTERNAL, APP_BANK_INTERNAL},
      /*WP_VB_CW,              */ {WP_BUS_INTERNAL, APP_BANK_INTERNAL},
      /*WP_VB_ASU_TXQUEUE,             */ {WP_BUS_PARAM, APP_BANK_PARAM},
      /*WP_VB_STATISTICS,          */ {WP_BUS_PACKET, APP_BANK_PACKET},
      /*WP_VB_ATM_ADDLOOKUP,       */ {WP_BUS_PARAM, APP_BANK_PARAM},
      /*WP_VB_ATM_CPT_EBMT_EXTHEADER,        */ {WP_BUS_PARAM, APP_BANK_PARAM},
      /*WP_VB_ETH_HDLC_CPT,            */ {WP_BUS_PARAM, APP_BANK_PARAM},
      /*WP_VB_FBP_BD,          */ {WP_BUS_PARAM, APP_BANK_PARAM},
      /*WP_VB_FBP_DATA,            */ {WP_BUS_PACKET, APP_BANK_PACKET},
      /*WP_VB_L2_BKGND,            */ {WP_BUS_PACKET, APP_BANK_PACKET},
      /*WP_VB_BRIDGE_LEARNING_DFC_PFMT       */ {WP_BUS_PACKET, APP_BANK_PACKET},
      /*WP_VB_IMA_MLPPP_STATE,         */ {WP_BUS_INTERNAL, APP_BANK_INTERNAL},
      /*WP_VB_IMA_MLPPP_BUFFER,        */ {WP_BUS_HOST, APP_BANK_HOST},
      /*WP_VB_HASH_IW_LPM_EMC,         */ {WP_BUS_PACKET, APP_BANK_PACKET},
      /*WP_VB_IW_MFC_RULES,            */ {WP_BUS_PACKET, APP_BANK_PACKET},
      /*WP_VB_IW_FIWT_QAT,             */ {WP_BUS_PARAM, APP_BANK_PARAM},
      /*WP_VB_IW_MFCPT_DIFFSERV_UPPT_D       */ {WP_BUS_INTERNAL, APP_BANK_INTERNAL},
      /*WP_VB_STATISTICS2,             */ {WP_BUS_PARAM, APP_BANK_PARAM},
      /*WP_VB_STATISTICS3,             */ {WP_BUS_PARAM, APP_BANK_PARAM},
      /*WP_VB_UNASSIGNED6,             */ {WP_BUS_NONE, 0},
      /*WP_VB_PKT_GLOBAL_TCPT,         */ {WP_BUS_NONE, 0},
      /*WP_VB_UNASSIGNED8,             */ {WP_BUS_NONE, 0},
      /*WP_VB_UNASSIGNED9,             */ {WP_BUS_NONE, 0},
      /*WP_VB_UNASSIGNED10,            */ {WP_BUS_NONE, 0},
      /*WP_VB_MCH_DATA,            */ {WP_BUS_NONE, 0},
      /*WP_VB_OAM_FM_QUERY,                */ {WP_BUS_NONE, 0},
      /*WP_VB_HASH_ON_HEAP,                */ {WP_BUS_NONE, 0},
      /*WP_VB_MFC_INTERNAL,                    */ {WP_BUS_PARAM, APP_BANK_PARAM},
      /*WP_VB_MFR_RAS_TBL,                   */ {WP_BUS_NONE, 0},
      /*WP_VB_CESOP_DATA,                    */ {WP_BUS_NONE, 0},
      /*WP_VB_SECURITY_ESE,                  */ {WP_BUS_NONE, 0},
      /*WP_VB_PDCP_SECURITY_ENTITY,          */ {WP_BUS_NONE, 0},
      /*WP_VB_POLICER,                       */ {WP_BUS_PARAM, APP_BANK_PARAM},
      /*WP_VB_PCE_LRN,                       */ {WP_BUS_INTERNAL, APP_BANK_INTERNAL},
      /*WP_VB_PCEA_PARAM,                    */ {WP_BUS_INTERNAL, APP_BANK_INTERNAL},
      /*WP_VB_CONGESTION_TABLE,              */ {WP_BUS_NONE, 0},
      /*WP_VB_PARAMS_PROPAGATION             */ {WP_BUS_NONE, 0},
      /*WP_VB_STATS_IWPORT_PCE_FILTERS       */ {WP_BUS_INTERNAL, APP_BANK_INTERNAL},
#if _WP_MODULE_CUSTOM_TIGERMILK_ENABLE_
      /*WP_VB_PROFILE_MARKING_STATS          */ {WP_BUS_NONE, 0},
#endif
      /*WP_VB_TIMESTAMP_SERVICES_TABLE       */ {WP_BUS_NONE, 0},
      /*WP_VB_POLICER_ACTION                 */ {WP_BUS_PARAM, APP_BANK_PARAM},
      /*WP_VB_G999_STREAM                    */ {WP_BUS_INTERNAL, APP_BANK_INTERNAL},
      /*WP_VB_G999_STREAM_STATISTICS         */ {WP_BUS_INTERNAL, APP_BANK_INTERNAL},
      },
     {
      dps_ProgramImage, dps_ProgramImage},
     {
      dps_PC_Table_Init, dps_PC_Table_Init},
     &int_queue_tables,         /* Interrupt queue tables */
     2,                         /* Mixumum number of pools */

     NULL,                      /* ATM global parameters */
     max_iw,                    /* Interworking global parameters */
     }
};
#endif //WP_BOARD_WDS3_SL

/* Callback function for a queuenode - Need to define the correct type later*/
WP_S32 QNCallback (WP_S32 status)
{
    printf ("App: QNCallback: Status returned is %d\n", status);
    return 0;
}

WP_pool_buffer_data buffer_data_2048[] = {
    {
     /* n_buffers */ 1024,
     /* offset */ 64,
                                /* size */ 1984,
                                /* RBS */
     /* pad */ 0,
#ifndef WP_BOARD_WDS3_SL
     /* bus */ WP_BUS_PACKET,
     /* bank */ APP_BANK_PACKET
#else
     /* bus */ WP_BUS_HOST,
     /* bank */ APP_BANK_HOST
#endif //WP_BOARD_WDS3_SL
     }
};

WP_pool_buffer_data buffer_data_144[1] = {
    {
     /* n_buffers */ 32,
     /* offset */ 0,
     /* size */ 1984,
     /* pad */ 0,
#ifndef WP_BOARD_WDS3_SL
     /* bus */ WP_BUS_PACKET,
     /* bank */ APP_BANK_PACKET
#else
     /* bus */ WP_BUS_PARAM,
     /* bank */ APP_BANK_PARAM
#endif //WP_BOARD_WDS3_SL
     }
};

WP_ch_enet gbe_ch[2] = {
    {
     /* intmode */ WP_PKTCH_INT_DISABLE,
     /* iwmode */ WP_PKTCH_IWM_ENABLE,
     /* testmode */ WP_PKTCH_TEST_DISABLE,
     /* tx_pqblock */ 0,
     /* tx_pqlevel */ 0,
     /* tx_shaping_type */ WP_FMU_SHAPING_TYPE_STRICT,
     /* tx_shaping_params */ NULL,
     /* rx_maxsdu */ WT_MAX_FRAME_SIZE,
     /* tx_cwid */ WP_CW_ID_A,
     /* tx_tq */ 0,
     /* rx_queuedepth[4] */ 17
     }
    ,
    {
     /* intmode */ WP_PKTCH_INT_DISABLE,
     /* iwmode */ WP_PKTCH_IWM_DISABLE,
     /* testmode */ WP_PKTCH_TEST_DISABLE,
     /* tx_pqblock */ 0,
     /* tx_pqlevel */ 0,
     /* tx_shaping_type */ WP_FMU_SHAPING_TYPE_STRICT,
     /* tx_shaping_params */ NULL,
     /* rx_maxsdu */ WT_MAX_FRAME_SIZE,
     /* tx_cwid */ WP_CW_ID_A,
     /* tx_tq */ 0,
     /* rx_queuedepth[4] */ 17
     }
};

WP_tx_binding tx_binding_iw[1] = {
    {
     /* res0; */ 0,
     /* dci_mode; */ WP_IW_DYN_CH_INSERT_DISABLE,
     /* maxt; */ 55,
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

WP_tx_binding tx_binding_cfg[1] = {
    {
     /* res0 */ 0,
     /* dci_mode */ WP_IW_DYN_CH_INSERT_DISABLE,
     /* maxt */ WT_MAX_FRAME_SIZE
     }
};

WP_ch_iw_rx host_ch_cfg[1] = {
    {
     /* pqblock */ 0,
     /* pqlevel */ 0,
     /* tx_binding_type */ WP_IW_TX_BINDING,
     /* tx_binding_config */ 0,
     }
};

WP_pkt_shaping_wfq wfq[1] = { {
                               /* weight;  */ 1,
                               /* weight_fraction; */ 0
                               }
};

WP_pkt_shaping_cir_eir shaping_params_cir_eir[1] = {
    {

     /* cir */ 40000000,
     /* cir_polling */ WP_SHAPING_POLL_AUTOD,
     /* cbs */ 800000,
     /* eir */ 50000000,
     /* eir_polling */ WP_SHAPING_POLL_AUTOD,
     /* ebs */ 800000,
     /* eir_cwid */ WP_CW_ID_B,
     /* eir_tq */ 2,
     }
};

WP_ch_enet ch_config_enet[1] = {
    {
     /* intmode */ WP_PKTCH_INT_ENABLE,
     /* iwmode */ WP_PKTCH_IWM_ENABLE,
     /* testmode */ WP_PKTCH_TEST_DISABLE,
     /* tx_pqblock */ 0,
     /* tx_pqlevel */ 0,
     /* tx_shaping_type */ WP_PKT_SHAPING_CIR_EIR,
     /* tx_shaping_params */ &shaping_params_cir_eir,
     /* rx_maxsdu */ WT_MAX_FRAME_SIZE,
     /* tx_cwid */ WP_CW_ID_B,
     /* tx_tq */ 0,
     /* rx_queuedepth */ MAX_NUM_OF_PACKETS
     }
};

WP_ch_enet ch_config_fast_enet2[1] = {
    {
     /* intmode */ WP_PKTCH_INT_ENABLE,
     /* iwmode */ WP_PKTCH_IWM_ENABLE,
     /* testmode */ WP_PKTCH_TEST_DISABLE,
     /* tx_pqblock */ 0,
     /* tx_pqlevel */ 0,
     /* tx_shaping_type */ WP_FMU_SHAPING_TYPE_STRICT,
     /* tx_shaping_params */ NULL,
     /* rx_maxsdu */ WT_MAX_FRAME_SIZE,
     /* tx_cwid */ WP_CW_ID_B,
     /* tx_tq */ 0,
     /* rx_queuedepth */ 17
     }
};

WP_calendar_wheel calendar[3] = {
    {
     /* function */ WP_CW_ASU,
     /* cwid */ WP_CW_ID_A,
     /* min_ch_rate */ 150,
     /* max_ch_rate */ 600000,
     }
    ,
    {
     /* function */ WP_CW_PSU,
     /* cwid */ WP_CW_ID_B,
     /* min_ch_rate */ 1000000,
     /* max_ch_rate */ 100500000,
     }
    ,
    WP_CALENDAR_WHEEL_TERM
};

WP_qnode_iwq qn_iw[1] = {
    {
     /* interruptqueue */ 0,
     /* num_buffers */ 8192,
     /* adjunct_pool */ 0
     }
};

/* <<<<<<<<<<<<<<<<<<<<<<<<<<< GMII START >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */

/* Giga Ethernet port1 & 2 configuration */
WP_port_enet enet_s_fast[1] = {
    {
     /* pkt_limits */ {64, 64, 8, NUM_PQ_BLOCK}
     ,
     /* flowmode */ WP_FLOWMODE_FAST,
     /* interface_mode */ WP_ENET_SGMII_1000,
     /* rx_iw_bkgnd */ 0
     }
};

/* Giga Ethernet port1 & 2 configuration */
WP_port_enet enet_s[1] = {
    {
     /* pkt_limits */ {64, 64, 8, NUM_PQ_BLOCK}
     ,
     /* flowmode */ WP_ENET_FMU_HIERARCHICAL_SHAPING_MODE,
     /* interface_mode */ WP_ENET_SGMII_1000,
     /* rx_iw_bkgnd */ 0
     }
};

/* Enet device configuration */
WP_device_enet enet_dev[1] = {
    {
     /* max_tx_channels */ MAX_NUM_OF_CHANNELS,
     /* tx_maxsdu */ WT_MAX_FRAME_SIZE,
     /* operating_speed */ WP_UNUSED,
     /* mac_addr */ {0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa}
     ,
     /* tx_bit_rate */ WP_TX_BIT_RATE_UNLIMITED,
     /* loopbackmode */ WP_ENET_NORMAL
     }
};

WP_enet_multi_class_config multi_class_config[1] = {
    {
     /* multi_class_mode           */ WP_ENET_MULTI_CLASS_DISABLED,
     /* max_multi_class_devices    */ 0,
     }
};

WP_device_enet_multi_class enet_multi_class_dev_cfg[1] = {
    {
     /* class_level */ 0,
     /*tx_shaping_type (select_mode) */ WP_ENET_MULTI_CLASS_SHAPING_ROUND_ROBIN,
     /*tx_shaping_params */ NULL,
#ifdef ADDED_BY_MORRIS
     /*tx_bit_rate */ WP_TX_BIT_RATE_UNLIMITED,
#endif
     }
};

WP_device_enet_ex enet_device_extended_config[1] = {
    {
     /*duplexmode */ WP_ENET_FULLDUPLEX,
     /*rx_maclookup_mode */ WP_DISABLE,
     /*rx_flowcontrol */ WP_ENET_FLOW_DISABLE,
     /*tx_flowcontrol */ WP_ENET_FLOW_DISABLE,
     /*rx_addrmode */ WP_ENET_ADDR_ACCEPTALL,
     /*phystatmode */ WP_ENET_STAT_ENABLE,
     /*tx_statmode */ WP_ENABLE,
     /*rx_statmode */ WP_ENABLE,
     /*tx_duplicate */ WP_DISABLE,
     /*rx_filters */ 0,
     /*rx_timestamp */ WP_DISABLE,
     /*timestamp_type */ 0,
     /*max_tx_slow_channels */ NUM_OF_CHANNELS,
     }
    ,
};

/* <<<<<<<<<<<<<<<<<<<<<<<<<<< GMII END >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */

/* <<<<<<<<<<<<<<<<<<<<<<<<<<< HSPOS START >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */

/* Configure Utopia port */
WP_port_pkt_limits pkt_limits = {
    /* max_tx_channels */ MAX_NUM_OF_CHANNELS,
    /* max_rx_channels */ MAX_NUM_OF_CHANNELS,
    /* pq_block_size   */ 1,
    /* n_pq_blocks     */ NUM_OF_CHANNELS,
    /* emphy_devices   */
};

WP_port_upi_packet_ext pkt_ext = {
    /* packet_characteristics */
    /*WP_UPI_POS_CRC_ENABLE, */ 0,
    /* slow_phy_tx_bitrates */ 0,
    /* packet_limits */ &pkt_limits
};

/* Configuration for the HSPOS Device */
WP_device_upi_hspos upi_hspos_device[1] = {
    {
     /* tx_bit_rate; */ WP_TX_BIT_RATE_UNLIMITED,
     /* tx_statmode; */ WP_PKT_STAT_ENABLE,
     /* tx_intmode; */ WP_PKTCH_INT_ENABLE,
     /* tx_slave_addr; */ 0,
     /* tx_queuedepth[8]; */ {0}
     ,
     /* rx_bit_rate; */ 622000000,
     /* rx_statmode; */ WP_PKT_STAT_ENABLE,
     /* rx_intmode; */ WP_PKTCH_INT_ENABLE,
     /* rx_queuedepth; */ 2000,
     /* rx_slave_addr; */ 0,
     /* tx_tag; */ 0,
     /* rx_tag; */ 0,
     /* end_station_mode; */ 0,
#if _WP_MODULE_NEC_GPON_
     /* bridge_port_id_mode; */ 0,
     /* remove_fcs; */ 0,
#endif
     /* tx_maxsdu; */ WT_MAX_FRAME_SIZE,
#if _WP_MODULE_NEC_GPON_
     /* enet_over_pos; */ 0,
#endif
     /*max_tx_slow_channels; */ NUM_OF_CHANNELS,
     }
};

/* <<<<<<<<<<<<<<<<<<<<<<<<<<< HSPOS END >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */

/*    InterWorking - Configurations */

/* Giga -> Giga */

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
     /*statmode; */ WP_IW_STAT_ENABLE,
     /*timestamp_mode; */ WP_IW_TIME_STAMP_DISABLE,
     /*ov_pool_mode */ WP_IW_OV_POOL_DISABLE,
     /*fbp_drop_threshold; */ 0,
     /*replace_vlan_id */ WP_IW_REPLACE_VTAG_DISABLE,
     /*vlan_id */ 0x05,
     /*vpmt_handle */ 0,
     /*mtu; */ WT_MAX_FRAME_SIZE,
     /*prefix_length */ 0,
     /*prefix_header[28]; */ {0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7,
                              0x8, 0x9, 0x10, 0x11, 0x12, 0x13, 0x14,
                              0x15, 0x16, 0x17, 0x18, 0x19, 0x20, 0x21,
                              0x22, 0x23, 0x24, 0, 0, 0, 0}
     ,
     /*policer_enable */ WP_IW_POLICER_DISABLE,
      /**policer_config;*/ NULL,
     /*cong_mode; */ WP_IW_CONGESTION_DISABLE,
      /**cong_threshold_param;*/ NULL
     }
};

WP_iw_agg_directmap dl_tx_agg_gbe[1] = {
    {
     /* tag                       */ 0,
     /* txfunc                    */ 0,
     /* rfcs                      */ WP_IW_RFCS_DISABLE,
     /* l2_header_insert_mode     */ WP_IW_L2H_INSERT_ENABLE,
     /* interruptqueue            */ WP_IW_IRQT1,
     /* error_pkt_mode            */ 0,
     /* intmode                   */ WP_IW_INT_DISABLE,
     /* statmode                  */ WP_IW_STAT_ENABLE,
     /* timestamp_mode            */ 0,
     /* ov_pool_mode              */ 0,
     /* fbp_drop_threshold        */ 0,
     /* mtu                       */ 1586,

                                                            /* prefix_length             */ WT_MAC_HEADER_SIZE,
                                                            //+ WT_IPV4_HEADER_SIZE + WT_L4_HEADER_SIZE,
                                        /* extraction_length         */ 0,
                                        //WT_IPV4_HEADER_SIZE + WT_L4_HEADER_SIZE,

                                /* prefix_header[42] */ {
                                /* Dest; Src; */

                                                         0x00, 0x01, 0x02, 0x03, 0x04, 0x00,    /* Eth header */
                                                         0x00, 0x01, 0x02, 0x03, 0x04, 0x00,
                                                         0x08, 0x00}
     ,

     /* policer_enable            */ 0,
     /* policer_config            */ 0,
     /* cong_mode                 */ 0,
     /* cong_threshold_param      */ 0,
                                        /* dynamic_length_update;    */ 0,
                                        //WP_IW_LENGTH_UPDATE_L3,
                                        /* length_offset;            */ 0,
                                        //WT_MAC_HEADER_SIZE,
     }

};

WP_iw_agg_directmap ul_tx_agg_add_eth[1] = {
    {
     /* tag                       */ 0,
     /* txfunc                    */ 0,
     /* rfcs                      */ WP_IW_RFCS_DISABLE,
     /* l2_header_insert_mode     */ WP_IW_L2H_INSERT_ENABLE,
     /* interruptqueue            */ WP_IW_IRQT1,
     /* error_pkt_mode            */ 0,
     /* intmode                   */ WP_IW_INT_DISABLE,
     /* statmode                  */ WP_IW_STAT_ENABLE,
     /* timestamp_mode            */ 0,
     /* ov_pool_mode              */ 0,
     /* fbp_drop_threshold        */ 0,
     /* mtu                       */ 1586,

     /* prefix_length             */ WT_MAC_HEADER_SIZE,
     /* extraction_length         */ 0,

                                /* prefix_header[42] */ {
                                /* Dest; Src; */

                                                         0x00, 0x01, 0x02, 0x03, 0x04, 0x00,    /* Eth header */
                                                         0x00, 0x01, 0x02, 0x03, 0x04, 0x00,
                                                         0x08, 0x00}
     ,

     /* policer_enable            */ 0,
     /* policer_config            */ 0,
     /* cong_mode                 */ 0,
     /* cong_threshold_param      */ 0,
     /* dynamic_length_update;    */ 0,
     /* length_offset;            */ 0,
     }

};

WP_iw_agg_directmap ul_tx_agg_gbe[1] = {
    {
     /* tag                       */ 0,
     /* txfunc                    */ 0,
     /* rfcs                      */ WP_IW_RFCS_DISABLE,
     /* l2_header_insert_mode     */ WP_IW_L2H_INSERT_ENABLE,
     /* interruptqueue            */ WP_IW_IRQT1,
     /* error_pkt_mode            */ 0,
     /* intmode                   */ WP_IW_INT_DISABLE,
     /* statmode                  */ WP_IW_STAT_ENABLE,
     /* timestamp_mode            */ 0,
     /* ov_pool_mode              */ 0,
     /* fbp_drop_threshold        */ 0,
     /* mtu                       */ 1586,

     /* prefix_length             */ WT_IPV4_HEADER_SIZE + WT_L4_HEADER_SIZE,
     /* extraction_length         */ WT_IPV4_HEADER_SIZE + WT_L4_HEADER_SIZE,

                                /* prefix_header[42] */ {
                                /* Dest; Src; */

                                                         0x45, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x11 /* IP header */
                                                         , 0x3d, 0xf1, 0xc0, 0xa8, 0x01, 0x01, 0xc0, 0xa8, 0x96, 0x46, 0x07, 0x7e, 0x13, 0x89, 0x05, 0x2c, 0x00, 0x00   /* Udp header */
                                                         }
     ,

     /* policer_enable            */ 0,
     /* policer_config            */ 0,
     /* cong_mode                 */ 0,
     /* cong_threshold_param      */ 0,
     /* dynamic_length_update;    */ WP_IW_LENGTH_UPDATE_L3,
     /* length_offset;            */ 0,
     }

};

WP_dfc_sys_info dfc_rout_sys_info[1] = {
    {
     /* classification_hash_size */ WP_IW_HASH_UNUSED,
     /* port_filtering */ WP_IW_PORT_FILTERING_DISABLED,
     /* deny_eq_ip */ WP_IW_DENY_EQUAL_IP_DISABLE
     }
};

WP_iw_ipv4_sys_info ipv4_sys_one[1] = {
    {
     /* classification_mode */ WP_IW_NULL_CLASSIFIER,
     /* max_lpm_flows */ 0,
     /* *dfc_info  */ &dfc_rout_sys_info[0]
     }
};

WP_iw_sys_routing iw_sys_one_routing[1] = {
    {
     /*  buffer_gap */ 0x40,
     /*  max_iw_ports */ 4,
     /*  max_classification_rules */ 2,
     /* supported_protocols */ WP_IW_SYS_IPV4,
     /* ipv4_sys_info */ ipv4_sys_one,
     /* *ipv6_sys_info */ NULL,
     /* *mpls_sys_info */ NULL,
     }
};

WP_tx_binding tx_binding_l2pi[1] = {
    {
     /* wred_mode; */ 0,
     /* dci_mode; */ 1,
     /* maxt; */ 128,
     }
};

WP_rx_binding_routing rx_binding_enet_routing[1] = {
    {
     /*  encap_mode */ 0,
     /*  l3_support_protocol; */ WP_IW_IPV4,
     /*  mru; */ (WT_MAX_FRAME_SIZE - 48)
     }
};

WP_bridge_port iwhost_bport_config[1] = {
    {
     /* tag */ 0x2000,
     /* direct_mapping */ WP_IW_DIRECT_MAP_ENABLE,
     /* flow_agg */ 0,
     /* flooding_term_mode */ WP_IW_HOST_TERM_MODE,
     /* learning_mode */ WP_IW_LEARNING_DISABLE,
     /* in_filter_mode */ WP_IW_INGRESS_FILTER_DISABLE,
     /* vlan_param */
     {
      /* vlan_acceptance_mode */ WP_IW_ACCEPT_TAGGED_UNTAGGED,
      /* vlan_tag */ 0x5,
      }
     ,
     /*max_mac_addresses */ 34,
     /*group_tag */ 0,
     /*group_filtering_mode */ WP_IW_GROUP_FILTER_DISABLE,
     /*unk_mac_sa_filter */ 0,
     /*unk_mc_mode; */ 0,
     /*bc_ht_mode; */ 0,
     /*input_filters_mask */ 0,
     /*output_filters_mask; */ 0,
     /*statmode */ WP_IW_PORT_STAT_ENABLE
     }
};

WP_iw_route_port route_port_cfg = {
    /* tag */ 0,
    /* input_filters_mask; */ 2,
    /* output_filters_mask; */ 2,
    /* statmode; */ WP_IW_PORT_STAT_ENABLE,
    /* forwarding_mode */ WP_IW_FORWARDING_DIRECT_MAP,
};


void WT_Start (WP_CHAR * app_name, WP_CHAR * trial_name, WP_CHAR * file_name);
void WT_Reboot (void);
static void terminate_on_error (WP_handle, WP_CHAR *, WP_U32);
static void App_LayerTwoSystemCreate (void);
static void App_SystemEnable (void);
static void App_DlForwardingTableCreate (void);
static void App_ShapingGroupsCreate (void);
static void App_ChannelsCreate (void);
static void App_HostAggCreate (WP_handle iw_sys);
static void App_DlRxBindingCreate (WP_handle iw_sys);
void WPT_DisplayBridgingPortStats (WP_handle port_handle, WP_CHAR * type);
void WPT_DisplayRoutingPortStats (WP_handle port_handle, WP_CHAR * type);

static void App_HostSendTraffic (void);
static void App_HostSendFlowCtrl (void);
static void App_CheckReceivedTraffic (WP_U32 expected_count);
WP_U16 WTI_IpChecksumCalc (WP_U8 * buff, WP_U16 len);
void WTI_TranslateHexToAscii (WP_CHAR * Ascii, WP_CHAR * Hex, WP_U32 length);

#if 1      
void WPT_FlowStatistics (WP_handle agg);
#endif

void App_Init (void);

static void App_CreateDlPceRulesFiltersSets (WP_handle iw_sys);

#if PCE_DEBUG
void App_PrintPceGlobalStats (void);
#endif
static void App_CreateFlowCtrlPceFilters (void);
static void App_CreateFlowCtrlPceRules (void);
static void App_CreateDataPceFilters (void);

#ifdef ADDED_BY_MORRIS
void App_CreateDlPceRules (void);
#else
static void App_CreateDataPceRules (void);

#endif

WP_U32 WPT_LocalDisplayDeviceStats (WP_handle device_handle);
void App_CheckPceFilterStats (WP_handle filter);
void App_ResetDataPceFilter (void);
void App_ResetFlowCtrlPceFilter (void);


#endif

