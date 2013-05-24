/*****************************************************************************
 * (C) Copyright PMC - WIN division (Wintegra) 2012.  All rights reserved.
 * PMC-Sierra (WINTEGRA) CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of PMC-Sierra (Wintegra).
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *  Appplication: ATM IMA UFE
 *  File Name   : atm_ima_ufe4.h
 *
 *****************************************************************************/
#ifndef WTI_ATM_IMA_H
#define WTI_ATM_IMA_H

/*NOTE: uncomment the macro, compiling for wp3 board... by default compilation will be for WP3 SLB board*/
#define BOARD_WP3

#define IMA_DEBUG 0
#define APP_DEBUG 0

#define APP_VLAN_ID          0xa
#define APP_MAX_MAC_ADDRESS  10

#define APP_MAX_PKTS_NUM 3

//#define _WP_MODULE_IMA_AR_ENABLE_

/* Enet Bridge ports */
#define APP_MAX_BPORTS       10
#define APP_NUM_IW_BUFFERS   500
#define APP_IW_BUFFER_SIZE   1984
#define APP_IW_BUFFER_OFFSET 64
#define APP_MASTER_WPID      WP_WINPATH(0)

/* IMA macros     */
#define NUM_OF_GROUPS      1
#define NUM_OF_LINKS       2    /* 2 ports per group */
#define MIN_NUM_OF_LINKS   1
#define PQBLOCK_SIZE       2
#define NUM_OF_CH_IN_GROUP 2
#define NUM_OF_SWCH        (NUM_OF_CH_IN_GROUP*NUM_OF_GROUPS)
#define MAX_ATM_TX_CHANNEL (NUM_OF_CH_IN_GROUP*NUM_OF_GROUPS)
#define MAX_ATM_RX_CHANNEL (NUM_OF_CH_IN_GROUP*NUM_OF_GROUPS)
#define RATE_TABLE_SLOTS   10
#define VP_PER_IMA_GROUP   1
#define APP_MAX_TDM_PORTS  NUM_OF_LINKS
#define MAX_CPS_SDU        45
#define MAX_SSSAR_SDU      1500
#define MIN_AAL2_CID_VALUE 1
#define MAX_AAL2_CID_VALUE 255
#define APP_MPLS_SIZE      4
#define PCR                4000
#define MAX_CHANNEL_RATE   (NUM_OF_GROUPS*NUM_OF_LINKS*PCR)

#define PHY_TYPE_T1	0
#define PHY_TYPE_E1	1
#undef PHY_TYPE_T1

#define WTI_MAX_LINES                               252  // For E1: should be 252

#define APP_EMPHY_PORT      WP_PORT_UPI1  /* EMPHY port */
#define APP_MAX_IMA_DEVICE  (NUM_OF_LINKS * NUM_OF_GROUPS)

#define WTI_INITIALIZE_FIRMWARE                     1 /* setting to '1' allows programming configuration through the host */
#define WTI_TX_MAX_SDU                              8192
#define WTI_MAX_PW                                  350  //336          /* maximum number of PW's in PWE3 system */  // For E1: should be 260
#define WTI_EMPHY_PORT                              WP_PORT_UPI1  /* EMPHY port */

#define WT_BRG_UPI1                                 WP_BRG3
#define WT_BRG_UPI2                                 WP_BRG1
#define N_MAX_OUT_OF_BAND_CAS_FLOWS                 2
#define WTI_MAX_ABS_PW                              WTI_MAX_PW
#define WTI_MAX_NUM_EMPHY_PHYS                      WTI_MAX_PW /* maximum number of emphy phys, relevant only if working with UFE */
#define WTI_SOCKET_LOOPBACK_MODE                    WUFE_LINE_LOOPBACK_TYPE_NONE
#define WT_FIRST_LINE_INDEX                         0

// UFE Related Defines
#define N_MAX_UFE_SPE_INDEX                         WUFE_MAX_N_CORE*WUFE_MAX_N_STM1*WUFE_MAX_N_SPE
#define MAX_UFE_LINES_USED                          1344
#define N_MAX_EMPHY_PHYS                            4032
#define WT_UFE_HANDLE_INVALID                       0xffffffff

#define WTI_MAX_LINES                               252  // For E1: should be 252

#define MAX_UFE_LINES 								WTI_MAX_LINES
#define OCTETS_PER_LINE 							28

#define WT_MAX_LINE_INDEX                           252

#define MAX_UFE_LINES_USED                          1344
#define MAX_CHANNELS								WTI_MAX_LINES * 2
#define WTI_MAX_DEVICE								WTI_MAX_LINES
#define WTI_MAX_TX_RX_CHANNEL						WTI_MAX_LINES

#define NUM_TU 										3

// FLEXMUX Related Defines
#define WTI_MAX_FLEXMUX_DEVICES_SUPPORTED_BY_DRIVER 4
#define WTI_FLEXMUX_NUM_DEVICES                     1
#define WTI_MAX_LINE_PORTS_PER_FLEXMUX_DEVICE       16

#define MAX_BANDWIDTH 600000000

/* Utility macros */
#define MAC_COPY(d,s)           { int _i; \
	for(_i=0;_i<6;_i++) \
	*((d)+_i)=*((s)+_i); }

extern WP_U32 dps_ProgramImage[];
extern WP_U16 dps_PC_Table_Init[];

/* An array of one WP_context structure for each WinPath */
WP_context context_cfg = { 6,   /* qnodes */
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
    { /* ASU_TPT         */ WP_BUS_HOST, APP_BANK_HOST},
    { /* ASU CW          */ WP_BUS_HOST, APP_BANK_HOST},
    { /* ASU TX QUEUE    */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
    { /* STATISTICS      */ WP_BUS_HOST, APP_BANK_HOST},
    { /* ADDRESS LOOKUP  */ WP_BUS_HOST, APP_BANK_HOST},
    { /* ATM CPT         */ WP_BUS_HOST, APP_BANK_HOST},
    { /* ENET HDLC CPT   */ WP_BUS_HOST, APP_BANK_HOST},
    { /* IW FBP PARAM    */ WP_BUS_HOST, APP_BANK_HOST},
    { /* IW FBP DATA     */ WP_BUS_HOST, APP_BANK_HOST},
    { /* EXTERNAL FIFO   */ WP_BUS_HOST, APP_BANK_HOST},
    { /* MAC LOOKUP      */ WP_BUS_HOST, APP_BANK_HOST},
    { /* IMA DISPATCH    */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},  /* Dispatch */
    { /* IMA SHARED      */ WP_BUS_PACKET, APP_BANK_PACKET},   /* Common   */
    { /* LPM             */ WP_BUS_HOST, APP_BANK_HOST},
    { /* MFC RULES       */ WP_BUS_HOST, APP_BANK_HOST},
    { /* FIWT, QAT       */ WP_BUS_HOST, APP_BANK_HOST},
    { /* MFCPT, UPPPT, DS */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
    { /* STATISTICS2      */ WP_BUS_HOST, APP_BANK_HOST},
    { /* STATISTICS3/IMAPORT */ WP_BUS_PARAM, APP_BANK_PARAM}, /* Port      */
    { /* WP_VB_IMA_RX_BUS    */ WP_BUS_PACKET, APP_BANK_PACKET},  /* Rx buffer */
    { /* PKT_GLOBAL_TCPT     */ WP_BUS_HOST, APP_BANK_HOST},
    { /* AAL2_QSR/IMATXBUF   */ WP_BUS_INTERNAL, APP_BANK_INTERNAL}, /* Tx buffer */
    { /* MANDATORY       */ WP_BUS_NONE, 0},
    { /* MANDATORY       */ WP_BUS_NONE, 0},
    /* ************* VIRTUAL BUS ASSIGNMENT ************** */
    { /* MCH_DATA        */ WP_BUS_PACKET, APP_BANK_PACKET},
    { /* WP_VB_OAM_FM_QUERY */ WP_BUS_HOST, APP_BANK_HOST},
    { /* WP_VB_HASH_ON_HEAP */ WP_BUS_NONE, 0},
    { /* WP_VB_MFC_INTERNAL */ WP_BUS_PACKET, APP_BANK_PACKET},
    { /* WP_VB_MFR_RAS_TBL */ WP_BUS_NONE, 0},
    { /* WP_VB_CESOP_DATA  */ WP_BUS_PACKET, APP_BANK_PACKET}
    },

   {
    dps_ProgramImage, dps_ProgramImage},
   {
    dps_PC_Table_Init, dps_PC_Table_Init},
   /* Int. Queue Table */
   NULL,

   /* Max # of buf pools */
   0,

   /* ATM   - Global info */
   NULL,

   /* IW - Global info */
   NULL
};

WP_int_queue_table int_queue_table_cfg = {
   {                            /* queue[WP_IRQ_TABLES] */
    {200, 1},                   /* size, rate */
    {8192, 1},
    {10, 1},
    {10, 1}
    }
};

WP_qnode_hostq qnode_hostq_cfg = {
   /* pool_buffer */ 0,
   /* pool_ring */ 0,
   /* interruptqueue */ WP_IRQT1
};

WP_pool_ring_data pool_ring_data_cfg = {
   /* n_rings */ 50,
   /* ring_length */ 30,
   /* bus */ WP_BUS_PARAM,
   /* bank */ APP_BANK_PARAM
};

WP_pool_buffer_data pool_buffer_data_256_cfg = {
   /* n_buffers */ 1000,
   /* offset */ 0,
   /* size */ 256,
   /* pad */ 0,
   /* bus */ WP_BUS_PACKET,
   /* bank */ APP_BANK_PACKET,
};

WP_pool_buffer_data pool_buffer_data_iw_cfg = {
   /* n_buffers              */ APP_NUM_IW_BUFFERS,
   /* offset                 */ 64,
   /* size                   */ 1984,
   /* pad                    */ 0,
   /* bus                    */ WP_BUS_PACKET,
   /* bank                   */ APP_BANK_PACKET
};

/* Global atm_data for the atm channels created by this application */
WP_atm_global atm_global_cfg = {
   /* max_pm_tables */ MAX_CHANNELS,
   /* max_ubrplus_channels */ MAX_CHANNELS,
   /* max_upc_tables */ NUM_OF_SWCH,
   /* max_rx_cids */ 0,
   /* max_tx_cids */ 0,
   /* max_cid_range */ 0,
   /* AAL2 QSR bus */ WP_BUS_NONE,
   /* AAL2 QSR bank */ 0,
   /* ATM modes */ {WP_ATM_RAS_TIMESTAMP_DISABLE, WP_AAL2_RX_3FE_DISABLE},
   /* max_stats_extensions */ MAX_CHANNELS,
   /* max_hier_shaping_groups */ 0
};

/* permit vpi's 8-10, each with vci's 34-39 */
WP_vp_vc_block vp_vc_range[2] = {
   {8, 10, 34, 39},
   WP_VP_VC_BLOCK_TERM
};

WP_tdm_binding t1_binding[] = {
   {0, 23}
};

WP_port_enet port_enet_cfg = {
   /* pkt_limits             */
   {
    /* max_tx_channels */ 8,
    /* max_rx_channels */ 8,
    /* pq_block_size */ 0,
    /* n_pq_blocks */ 0,
    /* emphy_devices */ 0,
    },
   /* flowmode               */ WP_FLOWMODE_FAST,
   /* interface_mode */ WP_ENET_SGMII_1000,
   /* rx_iw_bkgnd            */ WP_IW_BKGND_NOT_USED,
};

WP_enet_filters enet_filters_cfg = {
   /*rx_ucfilter; */ WP_ENET_FILTER_PROCESS,
   /*rx_bcfilter; */ WP_ENET_FILTER_HOSTTERM,
   /*rx_mcfilter; */ WP_ENET_FILTER_HOSTTERM,
};

WP_device_enet device_enet_cfg = {
   /* max_tx_channels */ 1,
   /* tx_maxsdu       */ 1536,
   /* rmii_operating_speed */ WP_UNUSED,
   /* mac_addr[6] */ {0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa},
   /* tx_bit_rate */ 1000000000,
   /* loopbackmode  */ WP_ENET_NORMAL,
   /* extended_params */ NULL,
};

WP_pkt_shaping_wfq default_pkt_shaping_wfq_cfg = {
   /* weight;  */ 1,
   /* weight_fraction; */ 0
};

WP_ch_enet ch_enet_cfg = {
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
   /* rx_queuedepth */ 17,
};

WP_calendar_wheel cw_config[2] = {
   {
    /* function */ WP_CW_ASU,
    /* cwid */ WP_CW_ID_A,
    /* min_ch_speed */ 150,
    /* max_ch_speed */ MAX_CHANNEL_RATE
    },
   WP_CALENDAR_WHEEL_TERM
};

WP_iw_global iw_global_cfg = {
   /* max_iw_aggregations    */ 20,
   /* max_routing_systems    */ 0,
   /* max_directmap_ene...   */ 1,
   /* max_bridging_systems   */ 2,
   /* max_mpls_systems       */ 0,
   /* max_vlan_priority...   */ 0,
   /* iw_bkgnd_fifo_size     */ 1000,
   /* cong_pt                */ NULL,
   /* iw_host_limits         */ {10, 10, 0, 0},
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
                                 /* port_filtering_mode; */
                                 WP_IW_PORT_FILTERING_ENABLE,
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

WP_tx_binding tx_binding_cfg = {
   /* res0                      */ 0,
   /* dci_mode                  */ WP_IW_DYN_CH_INSERT_ENABLE,
   /* maxt                      */ 10,
};

WP_qnode_iwq qnode_iwq_cfg = {
   /* interruptqueue         */ WP_IRQT1,
   /* num_buffers            */ 200,
   /* adjunct_pool           */ 0,
};

/* INTERWORKING BRIDGING */
WP_ch_iw_rx ch_iw_rx_cfg = {
   /* pqblock            */ 0,

   /* pqlevel            */ 0,

   /* tx_binding_type    */ WP_IW_TX_BINDING,

   /* tx_binding_config  */ NULL,
};

WP_rx_binding_bridging rx_binding_bridging_cfg = {
   /* encap_mode                */ WP_IW_VCMUX,
   /* mru                       */ 1536,
   /* vcfcs                     */ 0,
   /* input_port                */ 0,
};

WP_iw_agg_bridging default_iw_agg_bridging_cfg = {
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
   /* cong_threshold_param      */ NULL,
   /* flow_agg_type             */ WP_IW_FLOW_AGG_PRIMARY,
   /* external_vlan */ {
                        WP_IW_EXTERN_VLAN_NONE,
                        0},
   /* bc_mc_priority_mapping */ {
                                 WP_IW_BC_PRIORITY_DISABLE,
                                 0,
                                 WP_IW_MC_PRIORITY_DISABLE,
                                 0},
   /* reserved_agg_mode */ WP_IW_NOT_RESERVED,
   /* next_preserve_inport */ WP_IW_OVERRIDE_INPORT,
   /* next_preserve_inport_params */ 0,
   /* mac_replace_mode */ WP_IW_MAC_REPLACE_DISABLED,
   /* new dst mac */ {0},
   /* new_src_mac */ {0},
   /* multi_cong_prof */ 0,
   /* extraction_length */ 0,
};

WP_dfc_sys_info dfc_sys_info = {
   /* classification hash size */ WP_IW_32K_HASH_ENTRIES,
   /* port filtering */ WP_IW_PORT_FILTERING_ENABLE,
   /* deny_eq_ip */ WP_IW_DENY_EQUAL_IP_DISABLE,
};

WP_iw_sys_bridging iw_sys_bridge = {
   /* tag                       */ 0,
   /* max_flows                 */ 15,
   //10, 
   /* classification mode       */ WP_IW_CLASSIFIER_BASED_BRIDGING,
   /* classifier config         */ {WP_IW_DYNAMIC_CLASS, 100, 0, NULL},
   /* learning_mode             */ WP_IW_BRIDGE_LEARNING_DISABLE,
   /* learning queue            */ {
                                    WP_IW_IRQT1,
                                    0,
                                    WP_LEARNING_INTENABLE,
                                    1,
                                    },
   /* forwarding_table_size     */ WP_IW_32K_HASH_ENTRIES,
   /* member_set_size           */ WP_IW_32K_HASH_ENTRIES,
   /* buffer_gap                */ 0x40,
   /* max_bridging_ports        */ APP_MAX_BPORTS,
   /* dfc_info                  */ &dfc_sys_info,
   /* svl_mode */ WP_IW_SVL_DISABLED,
};

WP_bridge_port bridge_port_cfg = {
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
                                    /* vlan_priority_enforcement */
                                    WP_IW_VLAN_PRIORITY_ENFORCE_DISABLED,
                                    },
   /* max_mac_addresses         */ APP_MAX_MAC_ADDRESS,
   /* group_tag                 */ 0,
   /* group_filtering_mode      */ WP_IW_GROUP_FILTER_DISABLE,
   /*unk_mac_sa_filter; */ 0,
   /*unk_mc_mode; */ 0,
   /*bc_ht_mode; */ 0,
   /*input_filters_mask; */ 0,
   /*output_filters_mask; */ 0,
   /*statmode; */ WP_IW_PORT_STAT_ENABLE,
   /* unk_uc_mode; */ 0,
   /* classification_flag; */ WP_IW_IWPORT_CLASSIFICATION_ENABLED,
   /* adv_unk_lookup_mode; */ 0,
   /* cfi_ht_mode; */ 0,
   /* reserved_mc_ht_mode; */ WP_IW_RES_MC_HT_ENABLE,
};

WP_iw_sys_directmap iw_sys_directmap_cfg = {
   /*  max_flows */ 2 * APP_MAX_TDM_PORTS,
   /*  buffer_gap */ 0x40
};

WP_iw_agg_directmap iw_agg_directmap_cfg = {
   /* tag */ 0,
   /* txfunc */ 0,
   /* rfcs */ WP_IW_RFCS_DISABLE,
   /* l2header_insert_mode */ WP_IW_L2H_INSERT_DISABLE,
   /* interruptqueue */ WP_IW_IRQT1,
   /* error_pkt_mode */ WP_IW_ERRPKT_DISCARD,
   /* intmode */ WP_IW_INT_DISABLE,
   /* statmode */ WP_IW_STAT_ENABLE,
   /* timestamp_mode */ WP_IW_TIME_STAMP_DISABLE,
   /* ov_pool_mode */ 0,
   /* fbp_drop_threshold */ 0,
   /* mtu */ 1536,
   /* prefix_length */ 0,
   /* extraction_length */ 0,
   /* prefix_header[36] */ {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                            0x00, 0x00, 0x00, 0x00},
   /* policer_enable */ 0,
   /* policer_config */ NULL,
   /* cong_mode */ 0,
   /* cong_threshold_param */ NULL
};

WP_qnode_iwq qn_iw_ima_cfg = {
   /* interruptqueue */ 0,
   /* num_buffers */ 200,
   /* adjunct_pool */ 0
};

/* Shaping Group Table (Hierarchical shaping) */
WP_U16 shaping_table[RATE_TABLE_SLOTS + 1];

/* Group Resource Allocation parameters, prior to system commit.
 * Only enough structures to cover the potential requirements are required.
 * These are re-used whenever a group is to be started.  These
 * two cover
 * SHAPING_VC
 * SHAPING_HIERARCHICAL
 * LOOKUP_HASH
 * LOOKUP_TABLE                   */
WP_ima_group_alloc group_alloc[] = {
   {
    /* tx_shaping_mode */ WP_ATM_DEV_SHAPING_VC,
    /* tx_hier_shaping_slots_max */ 0,
    /* tx_hierarchical_shaping_table */ NULL,
    /* lookupmode */ WP_ATM_LOOKUP_HASH,
    /* extended phy stats  */ WP_ATM_EXT_PHY_STATS_DISABLE,
    /* Re-assembly timeout */ 0
    },
   {
    /* tx_shaping_mode */ WP_ATM_DEV_SHAPING_VC,
    /* tx_hier_shaping_slots_max */ 0,
    /* tx_hierarchical_shaping_table */ NULL,
    /* lookupmode */ WP_ATM_LOOKUP_HASH,
    /* extended phy stats  */ WP_ATM_EXT_PHY_STATS_DISABLE,
    /* Re-assembly timeout */ 0
    },

   WP_IMA_GROUP_ALLOC_TERM
};

/* Configuration information for the IMA system */
WP_ima_system ima_sys_params = {
   /* atm_limits */ {MAX_CHANNELS, MAX_CHANNELS, 0, 0, 0, 0},
   /* port_in_use                       */ WP_PORT_UPI1,
   /* number of groups in the system    */ NUM_OF_GROUPS,
   /* max number of links in the system */ WTI_MAX_LINES,
   /* Tx queue depth, in # of cells.    */ 8,
   /* max. delay tolerated in ms        */ 40,
   /* Min. Cell length, in micro-secs.  */
   (MAX_BANDWIDTH | WP_IMA_MAX_BANDWIDTH),
   /* Alarm enter persistance time = 2.5 sec */ 2500,
   /* Alarm exit persistance time = 10 sec   */ 10000,
   /* Test Pattern Time out period = 1 sec   */ 1,
   /* Number of lookup Tables                */ 1,
   /* VPI/VCI range used for lookup          */ vp_vc_range,
   /* Allocation requirements                */ group_alloc,
   /* Tick Period = 1/8 second               */ 125,
   /* Interworking bacground mode            */ WP_IW_BKGND_NOT_USED,
   /* Interrupt queue                        */ WP_IRQT1
};

/* Configuration information for the IMA group(s). */
WP_ima_group ima_group =
/* Configuration information for 1st group. */
{
   /* IMA ID (0-255)   */ 10 /*1 */ ,
   /* frame size       */ WP_IMA_FRAME_SIZE_128,
   /* Group Symmetry   */ WP_IMA_SYM_CFG_SYM_OP,
   /* IMA version      */ WP_IMA_VERSION_1_1,
   /* Tx Clock mode    */ WP_IMA_TX_CLOCK_CTC,
   /* Min Tx. Links    */ MIN_NUM_OF_LINKS,
   /* Min Rx. Links    */ MIN_NUM_OF_LINKS,
   /* Alpha            */ 2,
   /* Beta             */ 2,
   /* Gamma            */ 1,
   /* Link Cell length */ WP_IMA_E1_CELL_TIME,
   /* Bunch delay      */ 500000,
   /* Max delay - grp  */ 5,
   /* VPI/VCI range    */ vp_vc_range,
   /* trafic config    */ &group_alloc[0],
#ifdef _WP_MODULE_IMA_AR_ENABLE_
   /* modifiers       */ WP_IMA_AUTO_RESTART_ENABLE | WP_IMA_FE_GID_LEARN,
#else
   /* modifiers        */ WP_IMA_FE_GID_LEARN,
#endif
   /* gsm_timeout      */ 5,
   /* scci_audit_timeout */ 2
};

WP_ima_link ima_link[1] = {
   {
    /* direction         */ WP_IMA_LINK_TX_RX,
    /* link_id           */ WP_IMA_AUTO_LINK_ID,
    /* WP_ima_link_alarm */ {WP_IMA_NO_CHANGE,
                             WP_IMA_NO_CHANGE,
                             WP_IMA_NO_CHANGE,
                             WP_IMA_NO_CHANGE,
                             WP_IMA_NO_CHANGE,
                             WP_IMA_NO_CHANGE},
    },
};

WP_atm_shaping_unshaped unshaped_shaping_param = {
   /* weight */ 1
};

WP_atm_shaping_pcr aal5_shaping_param = {
   /* pcr */ 3000,
   /* cdvt */ 0,
   /* polling */ 3000
};

/* Host Terminated Channel, AAL5 */
WP_ch_aal5 iw_ch_aal5_cfg[1] = {
   {
    /* vpi */ 8,
    /* vci */ 34,
    /* intmode */ WP_ATMCH_INT_ENABLE,
    /* iwmmode */ WP_ATMCH_IWM_DISABLE,
    /* statmode */ WP_ATMCH_STAT_ENABLE,
    /* testmode */ WP_ATMCH_TEST_DISABLE,
    /* tx_pqblock */ 0,
    /* tx_pqlevel */ 0,
    /* tx_shaping_type */ WP_ATM_SHAPING_PCR,
    /* tx_shaping_params */ &aal5_shaping_param,
    /* tx_shaping_group */ 0,
    /* tx_extheader[12] */ {0},
    /* tx_uumode */ WP_ATMCH_UU_DISABLE,
    /* rx_timestamp */ 0,
    /* rx_maxsdu */ 1500,
    /* tx_mqblock */ 0,
    /* tx_mqlevel */ 0,
    /* tx_mqweight */ 0,
    /* tx_cwid */ WP_CW_ID_A,
    /* tx_tq */ 0,
    }
};

WP_iw_agg_pwe3 pwe3_atm_flow_agg_config[1] = {
   {
    /* tag        */ 0,
    /* txfunc     */ 0,
    /* rfcs       */ 0,
    /* intmode    */ WP_DISABLE,
    /* interruptqueue     */ WP_IRQT3,
    /* statmode           */ WP_ENABLE,
    /* timestamp_mode     */ WP_IW_TIME_STAMP_DISABLE,
    /* ov_pool_mode       */ WP_IW_OV_POOL_DISABLE,
    /* fbp_drop_threshold */ 0,
    /* mtu                */ 1536,
    /* flow_agg_type      */ WP_IW_AGG_PRIMARY,

    /* encapsulation_mode  */ WP_PWE3_ATM_AAL5_SDU,
    /* direction           */ WP_PWE3_EGRESS,
    /* sn_mode             */ WP_DISABLE,
    /* switchover_threshold */ 0,
    /* control_word_mode   */ WP_ENABLE,
    /* header_replace_mode */ WP_PWE3_REPLACE_NOTHING,

    /* prefix_length       */ 0,
    /* prefix_header[64]   */ {0},
    /* l3_length_update    */ WP_DISABLE,
    /* l3_header_offset    */ 0,

    /* extraction_length   */ 0,
    /* header              */ (int) NULL,

    /* policer_enable      */ 0,
    /* *policer_config     */ 0,
    /* ingress_cell_relay_mode */ WP_PWE3_INGRESS_NO_CELL_RELAY,
    /* oam_filtering_mode      */ WP_PWE3_OAM_TRANPARENT,
    /* egress_cell_lookup_mode */ WP_PWE3_EGRESS_LOOKUP_DISABLE,
    /* split_qnode             */ 0,
    /* n_prefix_remark_fields  */ 0,
    /* priority_values[3]      */ {0}
    },
};

WP_rx_binding_directmap rx_binding_directmap_cfg = {
   /* aggregation */ 0,
   /* mru */ 1440,
};

/*********************************************************
  UFE configuration
 **********************************************************/
WP_port_upi_emphy_ext emphy_ext = {
   /* tx_maxsdu */ 1536,
   /* protocols */ WP_EMPHY_ATM,
   //WP_EMPHY_IMA,//WP_EMPHY_ATM,
   /* trans_emphy_devices */ WTI_MAX_PW,
   //0,
   /* max_ds1 */ MAX_UFE_LINES_USED,
   //0,
};

WP_port_atm_limits ufe4_atm_limits = {
   /* max_tx_channels  */ MAX_CHANNELS,
   /* max_rx_channels  */ MAX_CHANNELS,
   /* pq_block_size   */ 0,
   /* n_pq_blocks     */ 0,
   /* aal1_tx_channels */ 0,
   /* aal1_rx_channels */ 0,
   /* n_mq_blocks     */ 0,
   /* n_mq_pq_blocks  */ 0,
   /* emphy_devices   */ MAX_UFE_LINES
};

WP_port_upi_atm_ext ufe4_atm_ext = {
   /* atm_characteristics */
   WP_UPI_IDLEDISC_ON | WP_ATM_EXT_PHY_STATS_ENABLE,
   /* utopia_clk */ 50000000,
   /* atm_limits */ &ufe4_atm_limits,
   /* tx_atm_exthdrsz */ 0,
   /* rx_atm_exthdrsz */ 0,
   /* rx_atm_lookupmode */ WP_ATM_LOOKUP_HASH
};

WP_port_upi upi_emphy_port_config = {
   /* upimode */ WP_UPI_UFE4,
   /* loopback */ WP_UPI_NORMAL,
   /* tx_characteristics */
   WP_UPI_MASTER |
      WP_UPI_SINGLE |
      WP_UPI_PRT_EVEN |
      WP_UPI_ROBIN |
      WP_TQ_SLOW_MODE |
      WP_TX_PRIORITY_4 | WP_UPI_EMPHY2_13 | WP_UPI_BUS_WIDTH_16,
   /* rx_characteristics */
   WP_UPI_MASTER |
      WP_UPI_SINGLE |
      WP_UPI_PRT_EVEN |
      WP_UPI_ROBIN | WP_UPI_PRTCHK_OFF | WP_UPI_BUS_WIDTH_16,
   /* atm_ext */ &ufe4_atm_ext,
   /* packet_ext */ 0,
   /* iw_ext */ 0,
   /* emphy_ext */ &emphy_ext
};

WP_device_emphy_atm ufe_device_cfg[1] = {
   {
    /* tx_bit_rate                    */ 155000000,
    /* tx_slave_addr                  */ 0,
    /* tx_shaping_mode                */ WP_ATM_DEV_SHAPING_VC,
    /* tx_shaping_groups_max          */ 0,
    /* tx_hierarchical_shaping_table  */ NULL,
    /* rx_bit_rate                    */ 155000000,
    /* vp_vc_block                    */ 0,
    /* rx_slave_addr                  */ 0,
    /* rx_ras_timeout                 */ 0,
    /* uni_nni_mode                   */ WP_EMPHY_ATM_NNI_MODE,
    /* idle_format                    */ WP_EMPHY_IDLE_REGULAR,
    /* line_id                        */ WP_NO_LINE_ID
    }
};

/* IMA EVENTS */
typedef struct
{
   WP_CHAR *event;
   WP_U32 shift;
} event_table_s;

event_table_s event_table[] = {
   {"BW Change (0=disable, 1=enable): ", WP_IMA_EVENT_BANDWIDTH_CHANGE},
   {"TTS (0=disable, 1=enable): ", WP_IMA_EVENT_TTS},
   {"TOS (0=disable, 1=enable): ", WP_IMA_EVENT_TOS},
   {"OP Change (0=disable, 1=enable): ", WP_IMA_EVENT_OPERATIONAL_CHANGE},
   {"TxQ Empty (0=disable, 1=enable): ", WP_IMA_EVENT_TXQEMPTY},
   {"TxQ Full  (0=disable, 1=enable): ", WP_IMA_EVENT_TXQFULL},
   {"FE ICP Change (0=disable, 1=enable): ", WP_IMA_EVENT_FE_ICP_CHANGE},
   {"NE ICP Change (0=disable, 1=enable): ", WP_IMA_EVENT_NE_ICP_CHANGE},
   {"Rx LASR (0=disable, 1=enable): ", WP_IMA_EVENT_GSU_LASR_RX},
   {"Tx LASR (0=disable, 1=enable): ", WP_IMA_EVENT_GSU_LASR_TX},
   {NULL, (WP_U32) NULL}
};

WP_U32 ticks;                   /* IMA ticks */

/*
 * Global variables used during test execution
 */
WP_U8 wt_mac_enet1[6] = { 0x45, 0x6e, 0x65, 0x74, 0x23, 0x31 };
WP_U8 wt_mac_enet2[6] = { 0x45, 0x6e, 0x65, 0x74, 0x23, 0x32 };
WP_tag tag_channel_base = 1;

WP_flow_learning_rule_vlan bridge_cfg;
WP_l2_forward forward_rule;

/* Interrupt handling structures */
typedef struct
{
   WP_tag event_tag;
   WP_U32 event_type;
   void *event_param;
} app_task;

typedef struct
{
   WP_U16 head;
   WP_U16 tail;
   WP_U16 num_elements;
   WP_U16 pad;
   app_task *task;
} app_task_list;

typedef struct
{
   WP_U16 in;
   WP_U16 out;
   WP_U16 size;
   WP_ima_event *task;
}
ima_app_task_list;

/* Interrupt task list */
#define IRQ_TASK_LIST_SIZE 4096
app_task irq_task[IRQ_TASK_LIST_SIZE];
app_task_list irq_task_list[1] =
   { {0, 0, IRQ_TASK_LIST_SIZE, 0, irq_task} };
ima_app_task_list ima_irq_task_list[1] =
   { {0, 0, IRQ_TASK_LIST_SIZE, (WP_ima_event *) & irq_task} };

/* Application prototypes */
void App_Quit (WP_U32 status);
void App_EventRxIndicate (WP_tag tag, WP_U32 info, WP_U32 data);
void App_EventTxIndicate (WP_tag tag, WP_U32 info, WP_U32 data);
void App_EventErrorIndicate (WP_tag tag, WP_U32 info, WP_U32 data);
static void App_ImaEvent (WP_tag tag, WP_U32 data, WP_U32 info);
void App_Debug (WP_boolean debug);
void App_TerminateOnError (WP_handle handle, WP_CHAR * s);
void App_CreateEnetHdlcBridge (void);
void App_CreateHdlcEnetBridge (void);
void App_DuReceive (WP_handle h_rx, WP_U32 data_type);
void App_DataSend (WP_handle h_dev, WP_handle h_pool);
void WPI_SimulateInterrupts (void);

app_task *next_task (app_task_list * task_list, app_task * result);

WP_ima_event *ima_next_task (ima_app_task_list * task_list,
                             WP_ima_event * result);

void add_task (app_task_list * task_list, WP_U32 event_type, WP_tag tag,
               void *event_param);
void ima_add_task (ima_app_task_list * task_list,
                   WP_U32 data, WP_U32 info);

void app_perform_action (app_task * task);
static void App_perform_freerun (void);

/* Statistics prototypes */
void App_ShowStats (void);
void App_ShowEnetDeviceStats (WP_handle enet_device_handle,
                              WP_stats_enet s_hs_enet);
void App_ShowTdmDeviceStats (WP_handle tdm_device_handle,
                             WP_stats_tdi_hdlc tdi_hdlc_stats);
void App_ShowTdmAtmDeviceStats (WP_handle tdm_device_handle,
                                WP_stats_tdi_atm tdi_atm_stats);
void App_ShowBridgePortStats (WP_handle bport_handle,
                              WP_bport_stats bport_stats);
void App_ShowFlowAggStats (WP_handle flow_agg_handle,
                           WP_iw_flow_stats flow_stats);
void App_AtmTxChannelStats (WP_handle atm_tx_ch_handle,
                            WP_stats_aal5_tx atm_tx_channel_stats);
void App_AtmRxChannelStats (WP_handle atm_rx_ch_handle,
                            WP_stats_aal5_rx atm_rx_channel_stats);
void App_ShowPwe3FlowAggStats (WP_handle flow_agg_handle,
                               WP_iw_pwe3_stats flow_stats);
void App_ShowImaGroupStats (void);
void App_ShowImaLinkStats (void);

/* IMA prototypes */
void App_TestInit (void);
void App_InitHW (void);
void App_SysInit (void);
void App_PortsCreate (void);
void App_DevicesAllocCreate (void);
void App_IMADevicesCreate (void);
void App_BufferPoolsCreate (void);
void App_SystemSetup (void);
void App_ImaSetup (void);
void App_ImaGroupEvent (void);
void App_ChannelsCreate (void);
void App_ImaChannelsCreate (void);
void App_PortsDevicesEnable (void);
void App_ChannelsEnable (void);
void App_CreateUfeEnetIwSystem (void);
void App_CreateEnetUfeIwSystem (void);
void App_enableEmphyPort (void);
void App_ufeSystem (void);
void App_EnableUFESystem (void);
void App_IMADevicesEnable (void);

#define ENET_HEADER_SIZE 14
#define IP_HEADER_SIZE   20
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
#define UDP_SRC_PORT    0
#define UDP_DST_PORT    2
#define UDP_LENGTH      4
#define UDP_CHECKSUM    6

/* Functions used for forming IP header  */
void insert_ip_header (WP_U32 ip_sa, WP_U32 ip_da, WP_U32 payload_size,
                       WP_U32 protocol, WP_U32 length, WP_U8 * prefix);

/* ETHERNET */
WP_handle h_port_enet1, h_port_enet2;
WP_handle h_device_enet1, h_device_enet2;

/* INTERWORKING COMMON */
WP_handle h_pool_buffer_host;
WP_handle h_pool_ring_host;
WP_handle h_pool_buffer_iw, h_pool_256;
WP_handle h_qnode_iwq, h_qnode_host, h_qnode_iwq_tdm;
WP_handle h_bridge1_iwhost, h_bridge2_iwhost;
WP_handle h_bridge1_default_agg, h_bridge2_default_agg;
WP_handle h_flow_agg_tdm[APP_MAX_TDM_PORTS];
WP_handle h_iw_sys_bridge1, h_iw_sys_bridge2;
WP_handle h_learningflow_enet1;
WP_handle h_bridge1_dfcflow1[MAX_ATM_TX_CHANNEL];
WP_handle h_bridge2_dfcflow[4];
WP_handle h_bridge1_filter[2], h_bridge2_filter[4];
WP_handle h_port_iwhost, h_dev_iwhost;
WP_handle h_bridge1_bport_enet1, h_bridge1_bport_tdm[APP_MAX_TDM_PORTS];
WP_handle h_bridge2_bport_enet2, h_bridge2_bport_tdm[APP_MAX_TDM_PORTS];
WP_handle h_flow_agg_enet2;
WP_handle h_rport_tdm[APP_MAX_TDM_PORTS], h_rport_enet2;
WP_handle h_enet1_rx, h_enet1_host_tx, h_enet1_tx;
WP_handle h_enet2_rx, h_enet2_tx;
WP_handle h_tdm_rx[APP_MAX_TDM_PORTS], h_tdm_tx[APP_MAX_TDM_PORTS];

WP_handle emphy_port;
WP_handle h_dev_ima[APP_MAX_IMA_DEVICE];

WP_handle status;

/* IMA related globals */
WP_handle h_dev_tdm_alloc[APP_MAX_TDM_PORTS];
WP_handle ima_sys_handle;
WP_handle ima_group_handle[NUM_OF_GROUPS];
WP_handle ima_link_handle[NUM_OF_LINKS * NUM_OF_GROUPS];
WP_handle ima_tx_ch_handle[MAX_ATM_TX_CHANNEL];
WP_handle ima_rx_ch_handle[MAX_ATM_RX_CHANNEL];
WP_handle ima_group_pqblock_handle[NUM_OF_GROUPS];
WP_handle policer_handle;
WP_handle ima_qn;
WP_handle h_bridge1_bport_ima;
WP_handle h_flow_agg_trans_bridge_egress[MAX_ATM_TX_CHANNEL];
WP_handle h_flow_agg_pwe3_atm_ingress[MAX_ATM_RX_CHANNEL];
WP_handle h_iw_sys_directmap;
WP_tag tag_bridge1_bport_ima;
WP_tag tag_agg_ima1;

/* TAGS */
WP_tag tag_agg_enet;

WP_tag tag_bport_enet1;
WP_tag tag_bport_enet2;
WP_tag tag_bridge1_bport_tdm[APP_MAX_TDM_PORTS];
WP_tag tag_bridge2_bport_tdm[APP_MAX_TDM_PORTS];
WP_tag tag_rport_tdm[APP_MAX_TDM_PORTS];
WP_tag tag_rport_enet2;
WP_tag tag_agg_enet2;

WP_tag tag_iw_sys_bridge1, tag_iw_sys_bridge2;
WP_tag tag_agg_default_bridge1, tag_agg_default_bridge2;

WP_tag tag_enet1_rx, tag_enet2_rx;
WP_tag tag_enet1_tx, tag_enet2_tx;
WP_tag tag_iwhost_rx1, tag_iwhost_rx2;
WP_tag tag_tdmrx = 200;
WP_tag tag_tdmtx = 300;
WP_boolean debug_on = WP_FALSE;

/******************************************************************************/
#endif
