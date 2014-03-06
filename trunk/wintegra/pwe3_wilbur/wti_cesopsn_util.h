/*****************************************************************************
 * (C) Copyright PMC - WIN division (Wintegra) 2011.  All rights reserved.
 * PMC-Sierra (WINTEGRA) CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of PMC-Sierra (Wintegra).
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/
/*****************************************************************************
 *
 * File: wti_cesopsn_util.h
 *
 * Purpose: provide CESoPSN system utility function.
 *
 ****************************************************************************/
#ifndef WTI_CESOPSN_UTIL_H
#define WTI_CESOPSN_UTIL_H

#include "wp_wddi.h"

#ifndef WTI_CESOP_CLOCK_RECOVERY_ENABLE
#define WTI_CESOP_CLOCK_RECOVERY_ENABLE 0
#endif
#ifndef WTI_CESOP_RX_TIMING_ENABLE
#define WTI_CESOP_RX_TIMING_ENABLE 0
#endif
#ifndef WTI_CLOCK_REC_MODE
#define WTI_CLOCK_REC_MODE 0
#endif
#ifndef WTI_EXT_DIFF_MODE_ACTIVE
#define WTI_EXT_DIFF_MODE_ACTIVE 0
#endif
#ifndef WTI_ACTIVE_ABSOLUTE_MODE
#define WTI_ACTIVE_ABSOLUTE_MODE 0
#endif
#ifndef WTI_MAX_NUM_CLOCK_TRANSLATORS
#define WTI_MAX_NUM_CLOCK_TRANSLATORS 0
#endif

/************************************************************************/



#define WTI_PW_UFE2_PHY_CREATED            0x1
#define WTI_PW_CREATED                     0x2
#define WTI_PW_ACTIVE                      0x4

#define WTI_IW_FLOW_AGGREGATIONS_RESERVED  16
#ifdef WP_BOARD_WDS3_SL
#define WTI_CESOP_BUS                      WP_BUS_PARAM
#define WTI_CESOP_BANK                     APP_BANK_PARAM
#else   /* NOT  WP_BOARD_WDS3_SL */
#define WTI_CESOP_BUS                      WP_BUS_PACKET
#define WTI_CESOP_BANK                     APP_BANK_PACKET
#endif  /* NOT WP_BOARD_WDS3_SL */

#define WTI_TDM_PORTS                      16
#define WTI_E1_SLOTS                       32
#define WTI_T1_SLOTS                       24
#if defined __WT_UFE412__ || defined __WT_UFE448__
#define WTI_CESOP_MAX_CLOCK_REC_INTERFACES 1344
#define WTI_CESOP_MAX_EXT_DIFF_CR_INTERFACES    0
#else
#define WTI_CESOP_MAX_CLOCK_REC_INTERFACES 64
#define WTI_CESOP_MAX_EXT_DIFF_CR_INTERFACES    336
#endif

#define WTI_CESOP_TOTAL_NUM_CLOCK_DOMAINS   WTI_CESOP_MAX_CLOCK_REC_INTERFACES + WTI_CESOP_MAX_EXT_DIFF_CR_INTERFACES
#define WTI_MAX_CONSEC_PKTS_IN_SYNCH_TH         10
#define WTI_MAX_CONSEC_MISS_PKTS_OUT_SYNCH_TH   15
#define WTI_INVALID_HANDLE                 0xFFFFFFFF
 #define WTI_MAX_OCTAL_LINES                N_MAX_UFE_OCT_LINE /* 8 */
#ifndef WTI_MAX_LINES
#if WTI_CESOP_TDI
#define WTI_MAX_LINES                      WTI_TDM_PORTS
#else
#define WTI_MAX_LINES                      N_MAX_UFE_SBI_LINE /* 84 */
#endif
#endif

#ifdef WP_HW_WINPATH3
#define UFE_MEM_PORT_SIZE                           WP_CLOCK_REC_PORT_SIZE_16
#else  /* NOT WP_HW_WINPATH3 */
#define UFE_MEM_PORT_SIZE                           WP_CLOCK_REC_PORT_SIZE_64
#endif /* NOT WP_HW_WINPATH3 */

#define WTI_EMPHY_PORT_OFFSET                       (0x200000 * WTI_EMPHY_PORT)
#if WTI_DUAL_EMPHY
#define WTI_EMPHY_PORT_OFFSET_SECOND_UFE            (0x200000 * WTI_SECOND_EMPHY_PORT)
#endif

#ifdef WP_HW_WINPATH3
#define WTI_UFE_BASE                                ( 0x1f200000 + WTI_EMPHY_PORT_OFFSET )
#else /* NOT WP_HW_WINPATH3 */
/* WP_HW_WINPATH2 */
#define WTI_UFE_BASE                                ( 0x1da00000 + WTI_EMPHY_PORT_OFFSET )
#endif /* Not WP_HW_WINPATH3 */

#define WTI_UFE_VIR_BASE                            (WTI_UFE_BASE + 0xa0000000)

#if (WTI_CESOP_CLOCK_RECOVERY_ENABLE || WTI_CESOP_RX_TIMING_ENABLE)


#ifdef WP_HW_WINPATH2
#define WTI_TS_CLOCKS_RATIO                         WP_IW_CESOP_TS_2_1_RATIO /* WP2 supports 2:1 and 4:1 */
#else
#define WTI_TS_CLOCKS_RATIO                         WP_IW_CESOP_TS_1_1_RATIO /* WP3 supports 1:1, 2:1 and 4:1 */
#endif

#ifdef __WT_UFE3__
#define WTI_CLOCK_REC_UFE_BASE                      WTI_UFE_BASE + (0x8420 << UFE_MEM_PORT_SIZE)
#else  /*  __WT_UFE412__ || __WT_UFE448__ */
#define WTI_CLOCK_REC_UFE_BASE                      WTI_UFE_BASE + (0x9000 << UFE_MEM_PORT_SIZE)
#endif



#define WTI_RX_TS_BASE_ADDRESS                      WTI_UFE_BASE + (0x9800 << UFE_MEM_PORT_SIZE)
#define WTI_TX_TS_BASE_ADDRESS                      WTI_UFE_BASE + (0xa000 << UFE_MEM_PORT_SIZE)

#define WTI_CLOCK_REC_BUSTYPE                       WP_BUS_HOST

#if defined __WT_UFE412__ || defined __WT_UFE448__
#if WTI_CESOP_TDI
#define WT_ClockRecInterfaceId(line_index) line_index
#endif /* WTI_CESOP_TDI */
#endif /* WT_UFE4 */

#endif /* (WTI_CESOP_CLOCK_RECOVERY_ENABLE || WTI_CESOP_RX_TIMING_ENABLE) */

#if WTI_CESOP_TDI
#ifndef MAX_UFE_LINES_USED
#if defined __WT_UFE412__ || defined __WT_UFE_448__
#define MAX_UFE_LINES_USED 1344
#else
#define MAX_UFE_LINES_USED 336
#endif /* WT_UFE4 */
#endif /* MAX_UFE_LINES_USED */
#endif /* WTI_CESOP_TDI */


extern WP_U32 dps_ProgramImage[];
extern WP_U16 dps_PC_Table_Init[];

typedef enum
{
   WTI_TDI_T1_OP_MODE,
   WTI_TDI_E1_OP_MODE
} WTI_tdi_op_mode;

typedef enum
{
   WTI_GMII_OV_FIBER,
   WTI_GMII_OV_COPPER
} WTI_gmii_mode;

typedef enum
{
   WTI_TDI_UNFRAMED_MODE,
   WTI_TDI_FRAMED_MODE
} WTI_tdi_framing_mode;

typedef enum
{
   WTI_TDI_NO_CAS_MODE,
   WTI_TDI_CAS_MODE
} WTI_tdi_cas_mode;

typedef enum
{
   WTI_IW_SYSTEM_ROUTING=0,
   WTI_IW_SYSTEM_BRIDGE=1,
} WTI_iw_system_mode;

#if WTI_ENET_IEEE_802
typedef struct
{
   WP_U8 dest_addr[6];
   WP_U8 source_addr[6];
   WP_U16 tpid;
   WP_U16 tci; /* includes 3 bits for priority, 1 bit CFI and 12 bits vlan id */
   WP_U16 type;
} WTI_enet_header;
#else
typedef struct
{
   WP_U8 dest_addr[6];
   WP_U8 source_addr[6];
#if WTI_NUM_OF_VLAN_TAGS == 2
   WP_U16 outer_type;
   WP_U16 outer_vlan;
#endif
#if WTI_NUM_OF_VLAN_TAGS > 0
   WP_U16 inner_type;
   WP_U16 inner_vlan;
#endif
   WP_U16 type;
} WTI_enet_header;
#endif

typedef struct
{
   WP_U16 source_port;
   WP_U16 dest_port;
   WP_U16 length;
   WP_U16 checksum;
} WTI_udp_header;

typedef struct
{
   WP_U8  ver_ihl;
   WP_U8  tos;
   WP_U16 total_length;
   WP_U16 id;
   WP_U16 frag_offset;
   WP_U8  ttl;
   WP_U8  protocol;
   WP_U16 checksum;
   WP_U32 source_addr;
   WP_U32 dest_addr;
} WTI_ip_header;

typedef struct
{
   WP_U8 control;
   WP_U8 payload_type;
   WP_U16 seq_number;
   WP_U32 timestamp;
   WP_U32 ssrc;
} WTI_rtp_header;

typedef struct
{
   WP_U8 control;
   WP_U8 frg_len;
   WP_U16 seq_number;
} WTI_control_word;

enum {
   WTI_QNODE_HOST,
   WTI_QNODE_IW_PSN2TDM,
   WTI_QNODE_IW_TDM2PSN,
   WTI_QNODE_TRANS_TX,
   WTI_QNODE_HDLC,
#if (WTI_PTP_ENABLE == 1)
   N_QNODES = 10
#else
   N_QNODES
#endif
};

enum
{
   WTI_BOARD_CONFIG_NA = 0,
   WTI_DEFAULT_PIN_CONFIG,
   WTI_PTP_TWO_BOARDS_CONFIG,
   WTI_SYNC_ETH_CONFIG
};

typedef struct
{
   WP_handle adjunct_pool;
   WP_handle handle;
} WTI_qnode;

typedef struct
{
   WP_U32 num_buffers;
   WP_U32 buffer_size;
} WTI_qnode_config;

typedef union StatField
{
   WP_U64 field;
   struct {
      WP_U32 high;
      WP_U32 low;
   } part;
} WTI_StatField;

#if WTI_INFO_STRUCTURES_DEFINED
typedef struct
{
   WP_rx_binding_routing rx_binding_gbe;
   WP_rx_binding_bridging  rx_binding_gbe_bridge;
} WTI_iw_info;

typedef struct
{
   WP_iw_agg_cesop cas_tdm2psn_agg_param;
   WP_iw_agg_cesop tdm2psn_agg_param;
   WP_iw_agg_cesop psn2tdm_agg_param;
   WP_tag tx_pwe3_channel_tag;
   WP_ch_trans_pwe3 trans_tx_ch_config;
   WP_tag rx_pwe3_channel_tag;
   WP_ch_trans_pwe3 trans_rx_ch_config;
   WP_device_emphy_trans upi_trans_device;
   WP_rx_binding_directmap trans_rx_binding_config;
   WP_iw_tx_binding_cesop cesop_tx_binding_config;
} WTI_pw_info;

#if WTI_PCE_CLASSIFIER
typedef struct
{
   WP_pce_rule_classification class_cfg;
   WP_pce_rule_classification class_cfg_bridge;
}WTI_pce_info;
#else
typedef struct
{
   WP_flow_class_rule class_cfg;
   WP_flow_class_rule class_cfg_bridge;
}WTI_dfc_info;
#endif
#endif

#if WTI_CESOP_TDI
typedef struct
{
   WP_U8 op_mode;
   WP_U8 cas_mode;
   WP_U8 framing_mode;
} WTI_tdi_config;
#endif

#if defined __WT_UFE412__ || defined __WT_UFE448__
/*
 * This structure is used for gathering information about the test that is running.
 * It is used for the initial setup of the different scenarios
 */
typedef struct
{
	WUFE_line_transf_type transfer_type;/* Transfer type all lines are using */
	WUFE_un_framed framing_mode; /* The framing mode (Framed/Unframed) for all lines */
	WUFE_cas_enable cas_mode; /* CAS enable/disable */
	WP_U32 num_of_lines; /* Number of lines currently in the system */
	WP_U32 num_slots; /* Number of slots in framed mode in each pw */
	WP_U32 first_slot_id; /* The first slot used for each line in framed mode */
	WP_U32 pw_per_line; /* Number of PW per line */
	WP_U32 recovered_pw; /* If multiple PW per line used the recovered PW is set here else should be 0 */
	WP_CHAR test_name[WTI_MAX_STRING_SIZE];
	WP_U32 line_cr_operation_mode[MAX_UFE_LINES_USED]; /* ACR/DCR per line */
        WP_U8 ds3_e3_framing_type;
#ifdef WT_UFE_FRAMER
	WPX_UFE_FRAMER_WUFE_line_transf_type vc_type;
#endif
}WUFE_test_system_setup;
#endif

#if WTI_DUAL_EMPHY
typedef enum
{
   WTI_EMPHY_FIRST = 1,
   WTI_EMPHY_SECOND
}WTI_emphy_number;
#endif

typedef struct
{
   WP_U32 spe_id;
   WP_U32 line_index;
   WP_U32 phy_index;
   WP_U32 n_slots;
   WP_U32 slots[WTI_E1_SLOTS];
   WP_U32 rx_data_unit_size;
   WP_U32 tx_data_unit_size;
   WP_U8  mode;
   WP_U16 rx_buffersize;
   WP_U8  statmode;
   WP_U32 tx_jitter_buffer_size;
   WP_U8  tx_udp_dummy_mode;
   WP_U8  tx_udcas;
   WP_U8  tx_udpattern[WP_TRANS_FRAME_SIZE];
   WP_U8  tx_udp_dummy_pattern[WP_TRANS_FRAME_SIZE];
   WP_U16 window_tx_threshold;
   WP_U8  window_switchover_threshold;
   WP_U16 window_max_threshold;
   WP_U8  lops_detection;
   WP_U8  consec_pkts_in_synch_th;
   WP_U8  consec_miss_pkts_out_synch_th;
   WP_U8  num_of_vlan_tags;
   WP_iw_agg_cesop tdm2psn_flow_agg_config;
   WP_iw_agg_cesop psn2tdm_flow_agg_config;
   WP_U8 *enet_header;
   WP_U8 *udp_header;
   WP_U8 *rtp_header;
   WP_U8 *control_word;
   WTI_ip_header *ip_header;
   WP_U8 rx_clk_rec_mode;
#if WTI_DUAL_EMPHY
   WTI_emphy_number pw_emphy_num;
#endif
} WTI_pw_params;

typedef struct
{
   WP_handle trans_device;
   WP_handle trans_rx;
   WP_handle trans_tx;
   WP_handle tdm2psn_flow_agg;
   WP_handle cas_tdm2psn_flow_agg;
   WP_handle psn2tdm_flow_agg;
#if WTI_PCE_CLASSIFIER
   WP_CHAR pce_rule[WTI_IW_RULE_MAX_LENGTH];
   WP_handle pce_flow_handle;
#else
#if WTI_CESOP_MPLS_IW
   WP_handle mpls_flow_handle;
   WP_dfc_flow_info dfc_flow_info;
   WP_CHAR dfc_rule[WTI_IW_RULE_MAX_LENGTH];
#else
   WP_CHAR dfc_rule[WTI_IW_RULE_MAX_LENGTH];
   WP_handle dfc_flow_handle;
   WP_dfc_flow_info dfc_flow_info;
#endif
#endif
   WP_U32 state;
   WP_U32 clock_rec_tx_enable;
   WP_U32 line_index;

} WTI_pw;

typedef struct
{
      WP_U32  sync_ethernet;
      WP_U32  sync_ethernet_master; /* if set use as SyncE master else as slave */


} WTI_system_features;

typedef struct
{
   WTI_system_features system_features;

   WP_U32 wpid;
   WTI_qnode qnode[N_QNODES];
   WTI_qnode_config qnode_config[N_QNODES];
#if (WTI_CESOP_MPLS_IW && !WTI_CESOP_MPLS_OVER_ENET)
   WP_handle hspi_port;
   WP_handle hspi_device;
#else
   WP_handle enet_port;
   WP_handle enet_device;
   WP_handle rx_enet_channel;
   WP_handle tx_enet_channel;
   WP_handle tx_enet_channel_high_priority;
#if WTI_2_ENET_DEVICES
   WP_handle enet_port1;
   WP_handle enet_device1;
   WP_handle rx_enet_channel1;
   WP_handle tx_enet_channel1;
   WP_handle tx_enet_channel1_high_priority;
#endif
#if WTI_4_ENET_DEVICES
   WP_handle enet_port2;
   WP_handle enet_device2;
   WP_handle rx_enet_channel2;
   WP_handle tx_enet_channel2;
   WP_handle tx_enet_channel2_high_priority;
   WP_handle enet_port3;
   WP_handle enet_device3;
   WP_handle rx_enet_channel3;
   WP_handle tx_enet_channel3;
   WP_handle tx_enet_channel3_high_priority;
#endif
#endif
#if WTI_CESOP_TDI
   WP_handle tdm_port[WTI_TDM_PORTS];
   WP_handle tdm_trans_dev[WTI_TDM_PORTS];
#else
   WT_ufe ufe;
#if WTI_DUAL_EMPHY
   WT_ufe second_ufe;
#endif
   WP_handle trans_port;
#if WTI_DUAL_EMPHY
   WP_handle second_trans_port;
#endif
#endif
   WP_handle iw_host_port;
   WP_handle iw_host_device;
   WP_handle generator;
   WP_handle host_channel;
   WP_handle tdm2psn_iw_system;
   WP_handle psn2tdm_iw_system;
#if WTI_CESOP_MPLS_IW
   WP_handle tdm2psn_mpls_l2_fa_handle[WTI_MAX_NUM_OF_MPLS_FA];
   WP_handle psn2tdm_mpls_l2_fa_handle[WTI_MAX_NUM_OF_MPLS_FA];
   WP_handle psn2tdm_mpls_flow_handle[WTI_MAX_NUM_OF_MPLS_FA];
#endif /* WTI_CESOP_MPLS_IW */
#if WTI_PCE_CLASSIFIER
   WP_handle pce_filter_handle;
   WP_handle pce_filter_set_handle;
   WP_handle pce_interface_handle;
#else
   WP_handle dfc_filter_handle;
#endif

   WP_handle host_term_flow_agg;
   WTI_pw pw[WTI_MAX_PW];
   WP_handle rx_host_channels[N_HOST_RX_CHANNELS];
   WP_handle violation_flow_agg;
   WP_rx_ts_global_params rx_ts_params;
#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
   WP_tx_ts_global_params tx_ts_params;
   WP_clock_rec_params clock_rec[WTI_CESOP_TOTAL_NUM_CLOCK_DOMAINS];
#endif
   WP_U32  iw_system_mode;
   WP_U32  stat_error;
   WP_U16  tx_ch_depth[WTI_MAX_PW];
   WP_handle iw_port;
   WP_handle bridge_iw_port;

   WP_boolean in_termination;


	WP_handle hdlc_dev_handle[WTI_DCC_LINE];
	WP_handle tx_hdlc_ch_handle[WTI_DCC_LINE];
	WP_handle rx_hdlc_ch_handle[WTI_DCC_LINE];
	
	WP_handle tx_dcc_fa_handle[WTI_DCC_LINE];
	WP_handle rx_dcc_fa_handle[WTI_DCC_LINE];

	WP_handle tx_host_hdlc_ch_handle[WTI_DCC_LINE];
	WP_data_unit tx_data_unit[1];
	WP_data_segment tx_segment[1];

    WTI_qnode qnode_hdlc[1];
   
	WP_handle enet_port_DCC;
	WP_handle enet_device_DCC;
	WP_handle rx_enet_channel_DCC;
	WP_handle tx_enet_channel_DCC;

	WP_handle enet_port_SYNCE;
	WP_handle enet_device_SYNCE;
	WP_handle rx_enet_channel_SYNCE;
	WP_handle tx_enet_channel_SYNCE;
	
} WTI_system;

/* Interrupt queue configuration used with WP_SysInit */
WP_int_queue_table int_queue_tables =
{
   /* In WP3 the WP_PollEvents is used to retrieve the events */
   {{4000, 0}, {4000, 0}, {4000, 0}, {4000, 0}}
};
#ifdef __WT_UFE3__
WP_port_upi_emphy_ext emphy_config[1] =
{
   {
      /* tx_maxsdu */           8192,
      /* protocols */           WP_EMPHY_TRANS_PWE3,
      /* trans_emphy_devices */ WTI_MAX_PW,
      /* max_ds1 */             336
   }
};
#else /* ufe 4*/
WP_port_upi_emphy_ext emphy_config[1] =
{
   {
      /* tx_maxsdu */           8192,
      /* protocols */           WP_EMPHY_TRANS_PWE3 + WP_EMPHY_HDLC_PPP,
#if 0						
      /* trans_emphy_devices */ WTI_MAX_PW + WTI_DCC_LINE,
      /* max_ds1 */             1344 + WTI_DCC_LINE
#else
      /* trans_emphy_devices */ WTI_MAX_PW,
      /* max_ds1 */             1344
#endif
   }
};
#endif

WP_port_upi_iw_ext emphy_iw_config[1] =
{
   {
      /* iw_characteristics */WP_IW_BKGND_USED,  /* must be set for WTI_CES_SHAPING_MODE */
      /* ces_tdm_wpid */0
   }
};

#if (WTI_CESOP_MPLS_IW && !WTI_CESOP_MPLS_OVER_ENET)
WP_port_pkt_limits hspi_pkt_limits[1] =
{
   {
      /* max_tx_channels */ 64,
      /* max_rx_channels */ 64,
      /* pq_block_size   */ 0,
      /* n_pq_blocks     */ 0,
      /* emphy_devices   */ 0
   }
};

WP_port_upi_packet_ext hspi_pkt_ext[1] =
{
   {
      /* packet_characteristics */ WP_UPI_POS_CRC_DISABLE,
      /* slow_phy_tx_bitrate */ 0,
      /* pkt_limits */ hspi_pkt_limits
   }
};
#endif

WP_iw_global context_iw_global[1] =
{
   {
      /* max_iw_aggregations */        15+N_IW_FLOW_AGGREGATIONS + WTI_IW_FLOW_AGGREGATIONS_RESERVED,
#if WTI_CESOP_MPLS_IW
      /* max_r_iw_systems;*/           1,
#else
      /* max_r_iw_systems;*/           N_IW_ROUTING_SYSTEMS,
#endif
      /* max_s_iw_systems;*/           1,
      /* max_b_iw_systems;*/           1, /* if brdige is used*/
#if WTI_CESOP_MPLS_IW
      /* max_m_iw_systems;*/           N_IW_ROUTING_SYSTEMS,
#else
      /* max_m_iw_systems;*/           0,
#endif
      /* max_vlan_priority_maps; */    0,
      /* iw_bkgnd_fifo_size */         5000,  /* relevant also for  WTI_CES_SHAPING_MODE  */
      /* cong_pt       */              NULL,
      /* iw_host_limits         */
      {
#if (WTI_PTP_ENABLE == 0x1)
         /* max_rx_channels */ N_HOST_RX_CHANNELS+1,
#else
         /* max_rx_channels */ N_HOST_RX_CHANNELS,
#endif
         /* pq_block_size   */ 0,
         /* n_pq_blocks     */ 0,
         /* max_tx_channels */ N_HOST_TX_CHANNELS,
         /* max_tx_groups   */ N_HOST_TX_GROUPS
      },
#if WTI_CESOP_MPLS_IW
      /* mpls_config */
      {
         /* n_llst_tables   */ 2,
         /* llst_table_size */ 2016
      },
#else
      /* mpls_config */
      {
         /* n_llst_tables   */ 0,
         /* llst_table_size */ 0
      },
#endif
      /* iw_modes;*/
      {
         /*policer_mode;*/                WP_IW_POLICER_DISABLE,
         /*statistics_bundle_stat_mode;*/ WP_IW_STATISTICS_BUNDLE_STAT_DISABLE,
         /*l2_prefix_ext_mode;*/          WP_IW_L2_HEADER_EXTENSION_ENABLE,
         /*enhanced_flow_stat_mode;*/     WP_IW_ENHANCED_FLOW_STAT_ENABLE,
         /*flow_stat_mode;*/              WP_IW_FLOW_STAT_ENABLE,
         /*fr_tx_flow_stat_mode*/         WP_IW_FR_TX_FLOW_STAT_DISABLE,
         /*mfc_alloc_mode    */           WP_IW_MFC_ALLOC_ENABLE,
         /*learning_queue_mode */         WP_IW_LEARNING_DISABLE,
         /*port_filtering_mode */         WP_IW_PORT_FILTERING_DISABLE,
         /* lpm_alloc_mode */             WP_IW_LPM_IPV4_ALLOC_ENABLE
      },
      /* max_ch_agg_stat;*/            0,
      /* max_fr_s_iw_systems;*/        0,
      /* max_bridging_ports */         (2*WTI_MAX_PW), /* if bridging is used*/
      /* max_iw_mc_groups   */         0,
      /* max_nat_systems    */         0,
      /* max_iw_second_aggregations */ 0,
      /* max_iw_comp_limits */ 0,
      /* max_iw_mc_members */ 0,
      /* max_iw_programmble_filters */1,
      /* max_iw_policers */ 0,
      /* max_dscpmt */ 0,
      /* num_of_wred_binding */0,
      /* tcm_policer_limits*/ {0,0},
      /* max_flow_rule_stat_tables */ 0,
      /* max_indirect_action_tables */ 0,
      /* lpm_bridging_limits */ {0, 0},
      /* brigding_time_stamp_info */ {0, 0},
      /* iwgp_bkgnd_fifo_size */0,
      /* lpm_max_ipv4_routing_flows */ 2048,
      /* lpm_max_ipv6_routing_flows */ 0
   }
};

#if WTI_8K_CH_SETUP
#ifdef WP_BOARD_WDS3_SL
WP_context context[1] =
{
   {N_QNODES, /* qnodes */
    /* the first four assignments are mandatory bus assignments */
    {
       /*WP_VB_PARAM = 0,                       */{WP_BUS_PARAM, APP_BANK_PARAM},
       /*WP_VB_PACKET,                          */{WP_BUS_PACKET, APP_BANK_PACKET},
       /*WP_VB_INTERNAL,                        */{WP_BUS_INTERNAL, APP_BANK_INTERNAL},
       /*WP_VB_HOST,                            */{WP_BUS_HOST, APP_BANK_HOST},
       /*WP_VB_INTQUEUE_DEBUG,                  */{WP_BUS_HOST, APP_BANK_HOST},
       /*WP_VB_TIMESTAMP,                       */{WP_BUS_HOST, APP_BANK_HOST},
       /*WP_VB_CAS,                             */{WP_BUS_HOST, APP_BANK_HOST},
       /*WP_VB_TRANS_TSAT,                      */{WP_BUS_HOST, APP_BANK_HOST},
       /*WP_VB_TPT,                             */{WP_BUS_HOST, APP_BANK_HOST},
       /*WP_VB_CW,                              */{WP_BUS_HOST, APP_BANK_HOST},
       /*WP_VB_ASU_TXQUEUE,                     */{WP_BUS_PARAM, APP_BANK_PARAM},
       /*WP_VB_STATISTICS,                      */{WP_BUS_HOST, APP_BANK_HOST},
       /*WP_VB_ATM_ADDLOOKUP,                   */{WP_BUS_HOST, APP_BANK_HOST},
       /*WP_VB_ATM_CPT_EBMT_EXTHEADER,          */{WP_BUS_HOST, APP_BANK_HOST},
       /*WP_VB_ETH_HDLC_CPT,                    */{WP_BUS_HOST, APP_BANK_HOST},
       /*WP_VB_FBP_BD,                          */{WP_BUS_PARAM, APP_BANK_PARAM},
       /*WP_VB_FBP_DATA,                        */{WP_BUS_PARAM, APP_BANK_PARAM},
       /*WP_VB_L2_BKGND,                        */{WP_BUS_PARAM, APP_BANK_PARAM},
       /*WP_VB_BRIDGE_LEARNING_DFC_PFMT         */{WP_BUS_PARAM, APP_BANK_PARAM},
       /*WP_VB_IMA_MLPPP_STATE,                 */{WP_BUS_INTERNAL, APP_BANK_INTERNAL},
       /*WP_VB_IMA_MLPPP_BUFFER,                */{WP_BUS_HOST, APP_BANK_HOST},
       /*WP_VB_HASH_IW_LPM_EMC,                 */{WP_BUS_HOST, APP_BANK_HOST},
       /*WP_VB_IW_MFC_RULES,                    */{WP_BUS_PARAM, APP_BANK_PARAM},
       /*WP_VB_IW_FIWT_QAT,                     */{WP_BUS_PARAM, APP_BANK_PARAM},
       /*WP_VB_IW_MFCPT_DIFFSERV_UPPT_D         */{WP_BUS_INTERNAL, APP_BANK_INTERNAL},
       /*WP_VB_STATISTICS2,                     */{WP_BUS_HOST, APP_BANK_HOST},
       /*WP_VB_STATISTICS3,                     */{WP_BUS_HOST, APP_BANK_HOST},
       /*WP_VB_UNASSIGNED6,                     */{WP_BUS_NONE, 0},
       /*WP_VB_PKT_GLOBAL_TCPT,                 */{WP_BUS_HOST, APP_BANK_HOST},
       /*WP_VB_UNASSIGNED8,                     */{WP_BUS_NONE, 0},
       /*WP_VB_UNASSIGNED9,                     */{WP_BUS_NONE, 0},
       /*WP_VB_UNASSIGNED10,                    */{WP_BUS_NONE, 0},
       /*WP_VB_MCH_DATA,                        */{WP_BUS_NONE, 0},
       /*WP_VB_OAM_FM_QUERY,                    */{WP_BUS_NONE, 0},
       /*WP_VB_HASH_ON_HEAP,                    */{WP_BUS_NONE, 0},
       /*WP_VB_MFC_INTERNAL,                    */{WP_BUS_HOST, APP_BANK_HOST},
       /*WP_VB_MFR_RAS_TBL,                     */{WP_BUS_NONE, 0},
       /*WP_VB_CESOP_DATA,                      */{WTI_CESOP_BUS, WTI_CESOP_BANK},
       /*WP_VB_SECURITY_ESE,                    */{WP_BUS_NONE, 0},
       /*WP_VB_PDCP_SECURITY_ENTITY,            */{WP_BUS_NONE, 0},
       /*WP_VB_POLICER,                         */{WP_BUS_PARAM, APP_BANK_PARAM},
       /*WP_VB_PCE_LRN,                         */{WP_BUS_INTERNAL, APP_BANK_INTERNAL},
       /*WP_VB_PCEA_PARAM,                      */{WP_BUS_INTERNAL, APP_BANK_INTERNAL},
       /*WP_VB_CONGESTION_TABLE,                */{WP_BUS_NONE, 0},
       /*WP_VB_PARAMS_PROPAGATION               */{WP_BUS_NONE, 0},
       /*WP_VB_STATS_IWPORT_PCE_FILTERS         */{WP_BUS_INTERNAL, APP_BANK_INTERNAL},
#if _WP_MODULE_CUSTOM_TIGERMILK_ENABLE_
       /*WP_VB_PROFILE_MARKING_STATS            */{WP_BUS_NONE, 0},
#endif
       /*WP_VB_TIMESTAMP_SERVICES_TABLE         */{WP_BUS_NONE, 0},
       /*WP_VB_POLICER_ACTION                   */{WP_BUS_PARAM, APP_BANK_PARAM},
    },
    {
       dps_ProgramImage, dps_ProgramImage
    },
    {
       dps_PC_Table_Init, dps_PC_Table_Init
    },
    /* int_queue_data */ &int_queue_tables,
    /* pool_limit */     N_POOLS,
    /* atm_data */       0,
    /* iw_data */        &context_iw_global[0],
   }
};
#else
WP_context context[1] =
{
   {N_QNODES, /* qnodes */
    /* the first four assignments are mandatory bus assignments */
    {
       /*WP_VB_PARAM = 0,                       */{WP_BUS_PARAM, APP_BANK_PARAM},
       /*WP_VB_PACKET,                          */{WP_BUS_PACKET, APP_BANK_PACKET},
       /*WP_VB_INTERNAL,                        */{WP_BUS_INTERNAL, APP_BANK_INTERNAL},
       /*WP_VB_HOST,                            */{WP_BUS_HOST, APP_BANK_HOST},
       /*WP_VB_INTQUEUE_DEBUG,                  */{WP_BUS_PACKET, APP_BANK_PACKET},
       /*WP_VB_TIMESTAMP,                       */{WP_BUS_PACKET, APP_BANK_PACKET},
       /*WP_VB_CAS,                             */{WP_BUS_PACKET, APP_BANK_PACKET},
       /*WP_VB_TRANS_TSAT,                      */{WP_BUS_PACKET, APP_BANK_PACKET},
       /*WP_VB_TPT,                             */{WP_BUS_PACKET, APP_BANK_PACKET},
       /*WP_VB_CW,                              */{WP_BUS_PACKET, APP_BANK_PACKET},
       /*WP_VB_ASU_TXQUEUE,                     */{WP_BUS_PARAM, APP_BANK_PARAM},
       /*WP_VB_STATISTICS,                      */{WP_BUS_PACKET, APP_BANK_PACKET},
       /*WP_VB_ATM_ADDLOOKUP,                   */{WP_BUS_PACKET, APP_BANK_PACKET},
       /*WP_VB_ATM_CPT_EBMT_EXTHEADER,          */{WP_BUS_PACKET, APP_BANK_PACKET},
       /*WP_VB_ETH_HDLC_CPT,                    */{WP_BUS_PACKET, APP_BANK_PACKET},
       /*WP_VB_FBP_BD,                          */{WP_BUS_PARAM, APP_BANK_PARAM},
       /*WP_VB_FBP_DATA,                        */{WP_BUS_PARAM, APP_BANK_PARAM},
       /*WP_VB_L2_BKGND,                        */{WP_BUS_PARAM, APP_BANK_PARAM},
       /*WP_VB_BRIDGE_LEARNING_DFC_PFMT         */{WP_BUS_PARAM, APP_BANK_PARAM},
       /*WP_VB_IMA_MLPPP_STATE,                 */{WP_BUS_INTERNAL, APP_BANK_INTERNAL},
       /*WP_VB_IMA_MLPPP_BUFFER,                */{WP_BUS_PACKET, APP_BANK_PACKET},
       /*WP_VB_HASH_IW_LPM_EMC,                 */{WP_BUS_PACKET, APP_BANK_PACKET},
       /*WP_VB_IW_MFC_RULES,                    */{WP_BUS_PARAM, APP_BANK_PARAM},
       /*WP_VB_IW_FIWT_QAT,                     */{WP_BUS_PARAM, APP_BANK_PARAM},
       /*WP_VB_IW_MFCPT_DIFFSERV_UPPT_D         */{WP_BUS_INTERNAL, APP_BANK_INTERNAL},
       /*WP_VB_STATISTICS2,                     */{WP_BUS_PACKET, APP_BANK_PACKET},
       /*WP_VB_STATISTICS3,                     */{WP_BUS_PACKET, APP_BANK_PACKET},
       /*WP_VB_UNASSIGNED6,                     */{WP_BUS_NONE, 0},
       /*WP_VB_PKT_GLOBAL_TCPT,                 */{WP_BUS_PACKET, APP_BANK_PACKET},
       /*WP_VB_UNASSIGNED8,                     */{WP_BUS_NONE, 0},
       /*WP_VB_UNASSIGNED9,                     */{WP_BUS_NONE, 0},
       /*WP_VB_UNASSIGNED10,                    */{WP_BUS_NONE, 0},
       /*WP_VB_MCH_DATA,                        */{WP_BUS_NONE, 0},
       /*WP_VB_OAM_FM_QUERY,                    */{WP_BUS_NONE, 0},
       /*WP_VB_HASH_ON_HEAP,                    */{WP_BUS_NONE, 0},
       /*WP_VB_MFC_INTERNAL,                    */{WP_BUS_PACKET, APP_BANK_PACKET},
       /*WP_VB_MFR_RAS_TBL,                     */{WP_BUS_NONE, 0},
       /*WP_VB_CESOP_DATA,                      */{WTI_CESOP_BUS, WTI_CESOP_BANK},
       /*WP_VB_SECURITY_ESE,                    */{WP_BUS_NONE, 0},
       /*WP_VB_PDCP_SECURITY_ENTITY,            */{WP_BUS_NONE, 0},
       /*WP_VB_POLICER,                         */{WP_BUS_PARAM, APP_BANK_PARAM},
       /*WP_VB_PCE_LRN,                         */{WP_BUS_INTERNAL, APP_BANK_INTERNAL},
       /*WP_VB_PCEA_PARAM,                      */{WP_BUS_INTERNAL, APP_BANK_INTERNAL},
       /*WP_VB_CONGESTION_TABLE,                */{WP_BUS_NONE, 0},
       /*WP_VB_PARAMS_PROPAGATION               */{WP_BUS_NONE, 0},
       /*WP_VB_STATS_IWPORT_PCE_FILTERS         */{WP_BUS_INTERNAL, APP_BANK_INTERNAL},
#if _WP_MODULE_CUSTOM_TIGERMILK_ENABLE_
       /*WP_VB_PROFILE_MARKING_STATS            */{WP_BUS_NONE, 0},
#endif
       /*WP_VB_TIMESTAMP_SERVICES_TABLE         */{WP_BUS_NONE, 0},
       /*WP_VB_POLICER_ACTION                   */{WP_BUS_PARAM, APP_BANK_PARAM},
    },
    {
       dps_ProgramImage, dps_ProgramImage
    },
    {
       dps_PC_Table_Init, dps_PC_Table_Init
    },
    /* int_queue_data */ &int_queue_tables,
    /* pool_limit */     N_POOLS,
    /* atm_data */       0,
    /* iw_data */        &context_iw_global[0],
   }
};
#endif

#else /* Not WTI_8K_CH_SETUP */
#if WP_BOARD_WDS3_SL
WP_context context[1] =
{ /* NOT  WP_BOARD_WDS3_SL */
   {N_QNODES, /* qnodes */
    /* the first four assignments are mandatory bus assignments */
    {
       /*WP_VB_PARAM = 0,                       */{WP_BUS_PARAM, APP_BANK_PARAM},
       /*WP_VB_PACKET,                          */{WP_BUS_PACKET, APP_BANK_PACKET},
       /*WP_VB_INTERNAL,                        */{WP_BUS_INTERNAL, APP_BANK_INTERNAL},
       /*WP_VB_HOST,                            */{WP_BUS_HOST, APP_BANK_HOST},
       /*WP_VB_INTQUEUE_DEBUG,                  */{WP_BUS_HOST, APP_BANK_HOST},
       /*WP_VB_TIMESTAMP,                       */{WP_BUS_HOST, APP_BANK_HOST},
       /*WP_VB_CAS,                             */{WP_BUS_PARAM, APP_BANK_PARAM},
       /*WP_VB_TRANS_TSAT,                      */{WP_BUS_PARAM, APP_BANK_PARAM},
       /*WP_VB_TPT,                             */{WP_BUS_PARAM, APP_BANK_PARAM},
       /*WP_VB_CW,                              */{WP_BUS_PARAM, APP_BANK_PARAM},
#if WTI_CESOP_TDI
       /*WP_VB_ASU_TXQUEUE,                     */{WP_BUS_HOST, APP_BANK_HOST},
#else
       /*WP_VB_ASU_TXQUEUE,                     */{WP_BUS_PARAM, APP_BANK_PARAM},
#endif
       /*WP_VB_STATISTICS,                      */{WP_BUS_HOST, APP_BANK_HOST},
       /*WP_VB_ATM_ADDLOOKUP,                   */{WP_BUS_PARAM, APP_BANK_PARAM},
       /*WP_VB_ATM_CPT_EBMT_EXTHEADER,          */{WP_BUS_PARAM, APP_BANK_PARAM},
       /*WP_VB_ETH_HDLC_CPT,                    */{WP_BUS_PARAM, APP_BANK_PARAM},
       /*WP_VB_FBP_BD,                          */{WP_BUS_PARAM, APP_BANK_PARAM},
       /*WP_VB_FBP_DATA,                        */{WP_BUS_HOST, APP_BANK_HOST},
       /*WP_VB_L2_BKGND,                        */{WP_BUS_HOST, APP_BANK_HOST},
       /*WP_VB_BRIDGE_LEARNING_DFC_PFMT         */{WP_BUS_PARAM, APP_BANK_PARAM},
       /*WP_VB_IMA_MLPPP_STATE,                 */{WP_BUS_INTERNAL, APP_BANK_INTERNAL},
       /*WP_VB_IMA_MLPPP_BUFFER,                */{WP_BUS_HOST, APP_BANK_HOST},
       /*WP_VB_HASH_IW_LPM_EMC,                 */{WP_BUS_HOST, APP_BANK_HOST},
       /*WP_VB_IW_MFC_RULES,                    */{WP_BUS_HOST, APP_BANK_HOST},
       /*WP_VB_IW_FIWT_QAT,                     */{WP_BUS_PARAM, APP_BANK_PARAM},
       /*WP_VB_IW_MFCPT_DIFFSERV_UPPT_D         */{WP_BUS_INTERNAL, APP_BANK_INTERNAL},
       /*WP_VB_STATISTICS2,                     */{WP_BUS_HOST, APP_BANK_HOST},
       /*WP_VB_STATISTICS3,                     */{WP_BUS_HOST, APP_BANK_HOST},
       /*WP_VB_UNASSIGNED6,                     */{WP_BUS_NONE, 0},
       /*WP_VB_PKT_GLOBAL_TCPT,                 */{WP_BUS_PARAM, APP_BANK_PARAM},
       /*WP_VB_UNASSIGNED8,                     */{WP_BUS_NONE, 0},
       /*WP_VB_UNASSIGNED9,                     */{WP_BUS_NONE, 0},
       /*WP_VB_UNASSIGNED10,                    */{WP_BUS_NONE, 0},
       /*WP_VB_MCH_DATA,                        */{WP_BUS_NONE, 0},
       /*WP_VB_OAM_FM_QUERY,                    */{WP_BUS_NONE, 0},
       /*WP_VB_HASH_ON_HEAP,                    */{WP_BUS_NONE, 0},
       /*WP_VB_MFC_INTERNAL,                    */{WP_BUS_HOST, APP_BANK_HOST},
       /*WP_VB_MFR_RAS_TBL,                     */{WP_BUS_NONE, 0},
       /*WP_VB_CESOP_DATA,                      */{WTI_CESOP_BUS, WTI_CESOP_BANK}
    },
    {
       dps_ProgramImage, dps_ProgramImage
    },
    {
       dps_PC_Table_Init, dps_PC_Table_Init
    },
    /* int_queue_data */ &int_queue_tables,
    /* pool_limit */     N_POOLS,
    /* atm_data */       0,
    /* iw_data */        &context_iw_global[0],
   }
};
#else /* NOT  WP_BOARD_WDS3_SL */
#if defined __WT_UFE412__ || __WT_UFE448__
WP_context context[1] =
{ /* For UFE4 */
   {N_QNODES, /* qnodes */
    /* the first four assignments are mandatory bus assignments */
    {
       /*WP_VB_PARAM = 0,                       */{WP_BUS_PARAM, APP_BANK_PARAM},
       /*WP_VB_PACKET,                          */{WP_BUS_PACKET, APP_BANK_PACKET},
       /*WP_VB_INTERNAL,                        */{WP_BUS_INTERNAL, APP_BANK_INTERNAL},
       /*WP_VB_HOST,                            */{WP_BUS_HOST, APP_BANK_HOST},
       /*WP_VB_INTQUEUE_DEBUG,                  */{WP_BUS_HOST, APP_BANK_HOST},
       /*WP_VB_TIMESTAMP,                       */{WP_BUS_HOST, APP_BANK_HOST},
       /*WP_VB_CAS,                             */{WP_BUS_HOST, APP_BANK_HOST},
       /*WP_VB_TRANS_TSAT,                      */{WP_BUS_PARAM, APP_BANK_PARAM},
       /*WP_VB_TPT,                             */{WP_BUS_PARAM, APP_BANK_PARAM},
       /*WP_VB_CW,                              */{WP_BUS_PARAM, APP_BANK_PARAM},
#if WTI_CESOP_TDI
       /*WP_VB_ASU_TXQUEUE,                     */{WP_BUS_PACKET, APP_BANK_PACKET},
#else
       /*WP_VB_ASU_TXQUEUE,                     */{WP_BUS_HOST, APP_BANK_HOST},
#endif
       /*WP_VB_STATISTICS,                      */{WP_BUS_HOST, APP_BANK_HOST},
       /*WP_VB_ATM_ADDLOOKUP,                   */{WP_BUS_PARAM, APP_BANK_PARAM},
       /*WP_VB_ATM_CPT_EBMT_EXTHEADER,          */{WP_BUS_INTERNAL, APP_BANK_INTERNAL},
       /*WP_VB_ETH_HDLC_CPT,                    */{WP_BUS_PARAM, APP_BANK_PARAM},
#if WTI_CESOP_TDI
       /*WP_VB_FBP_BD,                          */{WP_BUS_PACKET, APP_BANK_PACKET},
#else
       /*WP_VB_FBP_BD,                          */{WP_BUS_PARAM, APP_BANK_PARAM},
#endif
       /*WP_VB_FBP_DATA,                        */{WP_BUS_HOST, APP_BANK_HOST},
       /*WP_VB_L2_BKGND,                        */{WP_BUS_HOST, APP_BANK_HOST},
       /*WP_VB_BRIDGE_LEARNING_DFC_PFMT         */{WP_BUS_PARAM, APP_BANK_PARAM},
       /*WP_VB_IMA_MLPPP_STATE,                 */{WP_BUS_INTERNAL, APP_BANK_INTERNAL},
       /*WP_VB_IMA_MLPPP_BUFFER,                */{WP_BUS_HOST, APP_BANK_HOST},
       /*WP_VB_HASH_IW_LPM_EMC,                 */{WP_BUS_HOST, APP_BANK_HOST},
       /*WP_VB_IW_MFC_RULES,                    */{WP_BUS_HOST, APP_BANK_HOST},
       /*WP_VB_IW_FIWT_QAT,                     */{WP_BUS_INTERNAL, APP_BANK_INTERNAL},
       /*WP_VB_IW_MFCPT_DIFFSERV_UPPT_D         */{WP_BUS_INTERNAL, APP_BANK_INTERNAL},
       /*WP_VB_STATISTICS2,                     */{WP_BUS_HOST, APP_BANK_HOST},
       /*WP_VB_STATISTICS3,                     */{WP_BUS_HOST, APP_BANK_HOST},
       /*WP_VB_UNASSIGNED6,                     */{WP_BUS_NONE, 0},
       /*WP_VB_PKT_GLOBAL_TCPT,                 */{WP_BUS_INTERNAL, APP_BANK_INTERNAL},
       /*WP_VB_UNASSIGNED8,                     */{WP_BUS_NONE, 0},
       /*WP_VB_UNASSIGNED9,                     */{WP_BUS_NONE, 0},
       /*WP_VB_UNASSIGNED10,                    */{WP_BUS_NONE, 0},
       /*WP_VB_MCH_DATA,                        */{WP_BUS_NONE, 0},
       /*WP_VB_OAM_FM_QUERY,                    */{WP_BUS_NONE, 0},
       /*WP_VB_HASH_ON_HEAP,                    */{WP_BUS_NONE, 0},
       /*WP_VB_MFC_INTERNAL,                    */{WP_BUS_HOST, APP_BANK_HOST},
       /*WP_VB_MFR_RAS_TBL,                     */{WP_BUS_NONE, 0},
       /*WP_VB_CESOP_DATA,                      */{WTI_CESOP_BUS, WTI_CESOP_BANK}
    },
    {
       dps_ProgramImage, dps_ProgramImage
    },
    {
       dps_PC_Table_Init, dps_PC_Table_Init
    },
    /* int_queue_data */ &int_queue_tables,
    /* pool_limit */     N_POOLS,
    /* atm_data */       0,
    /* iw_data */        &context_iw_global[0],
   }
};
#else  /* Not in UFE4 */
WP_context context[1] =
{ /* For UFE3 */
   {N_QNODES, /* qnodes */
    /* the first four assignments are mandatory bus assignments */
    {
       /*WP_VB_PARAM = 0,                       */{WP_BUS_PARAM, APP_BANK_PARAM},
       /*WP_VB_PACKET,                          */{WP_BUS_PACKET, APP_BANK_PACKET},
       /*WP_VB_INTERNAL,                        */{WP_BUS_INTERNAL, APP_BANK_INTERNAL},
       /*WP_VB_HOST,                            */{WP_BUS_HOST, APP_BANK_HOST},
       /*WP_VB_INTQUEUE_DEBUG,                  */{WP_BUS_PACKET, APP_BANK_PACKET},
       /*WP_VB_TIMESTAMP,                       */{WP_BUS_PACKET, APP_BANK_PACKET},
       /*WP_VB_CAS,                             */{WP_BUS_PACKET, APP_BANK_PACKET},
       /*WP_VB_TRANS_TSAT,                      */{WP_BUS_PARAM, APP_BANK_PARAM},
       /*WP_VB_TPT,                             */{WP_BUS_PARAM, APP_BANK_PARAM},
       /*WP_VB_CW,                              */{WP_BUS_PARAM, APP_BANK_PARAM},
#if WTI_CESOP_TDI
       /*WP_VB_ASU_TXQUEUE,                     */{WP_BUS_PACKET, APP_BANK_PACKET},
#else
       /*WP_VB_ASU_TXQUEUE,                     */{WP_BUS_PARAM, APP_BANK_PARAM},
#endif
       /*WP_VB_STATISTICS,                      */{WP_BUS_HOST, APP_BANK_HOST},
       /*WP_VB_ATM_ADDLOOKUP,                   */{WP_BUS_PARAM, APP_BANK_PARAM},
       /*WP_VB_ATM_CPT_EBMT_EXTHEADER,          */{WP_BUS_PARAM, APP_BANK_PARAM},
       /*WP_VB_ETH_HDLC_CPT,                    */{WP_BUS_PARAM, APP_BANK_PARAM},
#if WTI_CESOP_TDI
       /*WP_VB_FBP_BD,                          */{WP_BUS_PACKET, APP_BANK_PACKET},
#else
       /*WP_VB_FBP_BD,                          */{WP_BUS_PARAM, APP_BANK_PARAM},
#endif
       /*WP_VB_FBP_DATA,                        */{WP_BUS_HOST, APP_BANK_HOST},
       /*WP_VB_L2_BKGND,                        */{WP_BUS_PACKET, APP_BANK_PACKET},
       /*WP_VB_BRIDGE_LEARNING_DFC_PFMT         */{WP_BUS_PARAM, APP_BANK_PARAM},
       /*WP_VB_IMA_MLPPP_STATE,                 */{WP_BUS_INTERNAL, APP_BANK_INTERNAL},
       /*WP_VB_IMA_MLPPP_BUFFER,                */{WP_BUS_PACKET, APP_BANK_PACKET},
       /*WP_VB_HASH_IW_LPM_EMC,                 */{WP_BUS_HOST, APP_BANK_HOST},
       /*WP_VB_IW_MFC_RULES,                    */{WP_BUS_PACKET, APP_BANK_PACKET},
       /*WP_VB_IW_FIWT_QAT,                     */{WP_BUS_PARAM, APP_BANK_PARAM},
       /*WP_VB_IW_MFCPT_DIFFSERV_UPPT_D         */{WP_BUS_INTERNAL, APP_BANK_INTERNAL},
       /*WP_VB_STATISTICS2,                     */{WP_BUS_HOST, APP_BANK_HOST},
       /*WP_VB_STATISTICS3,                     */{WP_BUS_HOST, APP_BANK_HOST},
       /*WP_VB_UNASSIGNED6,                     */{WP_BUS_NONE, 0},
       /*WP_VB_PKT_GLOBAL_TCPT,                 */{WP_BUS_PARAM, APP_BANK_PARAM},
       /*WP_VB_UNASSIGNED8,                     */{WP_BUS_NONE, 0},
       /*WP_VB_UNASSIGNED9,                     */{WP_BUS_NONE, 0},
       /*WP_VB_UNASSIGNED10,                    */{WP_BUS_NONE, 0},
       /*WP_VB_MCH_DATA,                        */{WP_BUS_NONE, 0},
       /*WP_VB_OAM_FM_QUERY,                    */{WP_BUS_NONE, 0},
       /*WP_VB_HASH_ON_HEAP,                    */{WP_BUS_NONE, 0},
       /*WP_VB_MFC_INTERNAL,                    */{WP_BUS_PACKET, APP_BANK_PACKET},
       /*WP_VB_MFR_RAS_TBL,                     */{WP_BUS_NONE, 0},
       /*WP_VB_CESOP_DATA,                      */{WTI_CESOP_BUS, WTI_CESOP_BANK}
    },
    {
       dps_ProgramImage, dps_ProgramImage
    },
    {
       dps_PC_Table_Init, dps_PC_Table_Init
    },
    /* int_queue_data */ &int_queue_tables,
    /* pool_limit */     N_POOLS,
    /* atm_data */       0,
    /* iw_data */        &context_iw_global[0],
   }
};
#endif /* WT_UFE4 */
#endif /* NOT  WP_BOARD_WDS3_SL */
#endif /* NOT WTI_8K_CH_SETUP */

#if !WTI_PCE_CLASSIFIER
WP_dfc_sys_info dfc_sys_info =
{
   /* classification_hash_size */ WP_IW_2K_HASH_ENTRIES,
   /* port_filtering */           WP_IW_PORT_FILTERING_DISABLED,
   /* deny_eq_ip */               WP_IW_DENY_EQUAL_IP_DISABLE
};
#endif

WP_iw_agg_hq  host_term_agg[1] =
{

   {
      /*tag;*/40,
      /*txfunc;*/0,
      /*hq_agg_mode;*/WP_IW_HQ_ARP_RARP,
      /*mtu;*/318,
      /*interruptqueue;*/WP_IW_IRQT0,
      /*intmode;*/WP_IW_INT_ENABLE,
      /*statmode;*/WP_IW_STAT_ENABLE,
      /*timestamp_mode;*/WP_IW_TIME_STAMP_DISABLE,
      /*ov_pool_mode;*/WP_IW_OV_POOL_DISABLE,
      /*fbp_drop_threshold;*/0,
      /*tx_direction;*/0,
      /*flow_analyze;*/WP_IW_FLOW_PASS,
      /*force_analyze;*/WP_IW_FORCE_NO,
      /*l4_checksum_recal;*/WP_IW_L4_NO_CHECKSUM,
      /*nexthop;*/0,
      /*flow_agg_type;*/ WP_IW_AGG_PRIMARY
   }
};

/* set CR values for 300MHz (active in WP2) */
WP_clock_rec_freq clock_rec_default_t1_freq_for_300MHz =
   {0x60, 0x267760D4, 0x027CBF63 /* for 300MHz/100ppm dco reference clock */};
WP_clock_rec_freq clock_rec_default_e1_freq_for_300MHz =
   {0x48, 0x3E000000, 0x01E00C4A /* for 300MHz/100ppm dco reference clock */};
/* set CR values for extended differential mode */
WP_clock_rec_freq clock_rec_default_ext_diff_t1_freq_for_300MHz =
   {0x60, 0x267760D4, 0x027CBF63 /* for 300MHz/100ppm dco reference clock */};
WP_clock_rec_freq clock_rec_default_ext_diff_e1_freq_for_300MHz =
   {0x48, 0x3E000000, 0x01E00C4A /* for 300MHz/100ppm dco reference clock */};

/* set CR values for 320MHz (active in WP3 - slow) */

WP_clock_rec_freq clock_rec_default_t1_freq_for_320MHz =
   {0x66, 0xA07F5637, 0x02A7328C /* for 320MHz/100ppm dco reference clock */};
WP_clock_rec_freq clock_rec_default_e1_freq_for_320MHz =
   {0x4d, 0x20000000, 0x02000D1B /* for 320MHz/100ppm dco reference clock */};
/* set CR values for extended differential mode */
WP_clock_rec_freq clock_rec_default_ext_diff_t1_freq_for_320MHz =
   {0x66, 0xA07F5637, 0x02A7328C /* for 320MHz/100ppm dco reference clock */};
WP_clock_rec_freq clock_rec_default_ext_diff_e1_freq_for_320MHz =
   {0x4d, 0x20000000, 0x02000D1B /* for 320MHz/100ppm dco reference clock */};


/* set CR values for 450MHz (active in WP3) */
WP_clock_rec_freq clock_rec_default_t1_freq_for_450MHz =
   {0x90, 0xB9B3113E, 0x03BB1F15 /* for 450MHz/100ppm dco reference clock */};
WP_clock_rec_freq clock_rec_default_e1_freq_for_450MHz =
   {0x6C, 0xDD000000, 0x02D0126F /* for 450MHz/100ppm dco reference clock */};
/* set CR values for extended differential mode */
WP_clock_rec_freq clock_rec_default_ext_diff_t1_freq_for_450MHz =
   {0x90, 0xB9B3113E, 0x03BB1F15 /* for 450MHz/100ppm dco reference clock */};
WP_clock_rec_freq clock_rec_default_ext_diff_e1_freq_for_450MHz =
   {0x6C, 0xDD000000, 0x02D0126F /* for 450MHz/100ppm dco reference clock */};

/* set CR values for 200MHz */
WP_clock_rec_freq clock_rec_default_t1_freq_for_200MHz =
   {0x3F, 0xC44F95E2, 0x01A87F97 /* for 200MHz/100ppm dco reference clock */};
WP_clock_rec_freq clock_rec_default_e1_freq_for_200MHz =
   {0x2F, 0xD4000000, 0x01400831 /* for 200MHz/100ppm dco reference clock */};
/* set CR values for extended differential mode */
WP_clock_rec_freq clock_rec_default_ext_diff_t1_freq_for_200MHz =
   {0x3F, 0xC44F95E2, 0x01A87F97 /* for 200MHz/100ppm dco reference clock */};
WP_clock_rec_freq clock_rec_default_ext_diff_e1_freq_for_200MHz =
   {0x2F, 0xD4000000, 0x01400831 /* for 200MHz/100ppm dco reference clock */};

/* structures for self calculated of DCO values */
WP_clock_rec_freq clock_rec_dco_e1_params, clock_rec_dco_t1_params;

/*************************************************************************/

/* configuration structures */
WP_ch_iw_rx iw_host_ch_config;
WP_tx_binding tx_binding_config;
WP_rx_binding_routing rx_binding_routing_config;
#if WTI_CESOP_MPLS_IW

#if (!WTI_CESOP_MPLS_OVER_ENET)
WP_port_upi hspi_port_config;
WP_device_upi_hspos hspi_device_config;
#else
WP_port_enet enet_port_config;
WP_device_enet enet_device_config;
WP_device_enet_ex enet_device_ex_config;
WP_ch_enet enet_rx_ch_config;
WP_ch_enet enet_tx_ch_config;
WP_tx_binding tx_binding_gbe;
#endif

WP_mpls_label_range mpls_label_ranges[2];
#if WTI_PCE_CLASSIFIER
WP_rx_binding_routing rx_binding_gbe;
WP_rx_binding_bridging rx_binding_gbe_bridge;
WP_iw_sys_routing iw_routing_system_config;
WP_iw_ipv4_sys_info ipv4_sys_info;
WP_pce_filter_set level1_set_cfg;
WP_pce_if_params_pkt_rx_channel pce_interface_cfg;
WP_pce_filter_stat pce_filter_stats[5];
#else
WP_iw_sys_mpls iw_mpls_sys_config;
WP_rx_binding_mpls rx_binding_mpls;
WP_rx_binding_bridging rx_binding_gbe_bridge;
#endif
#else
WP_port_enet enet_port_config;
WP_device_enet enet_device_config;
WP_device_enet_ex enet_device_ex_config;
WP_ch_enet enet_rx_ch_config;
WP_ch_enet enet_tx_ch_config;
WP_rx_binding_routing rx_binding_gbe;
WP_tx_binding tx_binding_gbe;
WP_rx_binding_bridging rx_binding_gbe_bridge;
WP_iw_sys_routing iw_routing_system_config;
WP_iw_ipv4_sys_info ipv4_sys_info;
#endif
WP_iw_sys_directmap iw_cesop_system_config;
WP_ch_trans_pwe3 trans_ch_config;
WP_iw_agg_cesop tdm2psn_flow_agg_config;
WP_iw_agg_cesop psn2tdm_flow_agg_config;
WP_rx_binding_directmap trans_rx_binding_config;
WP_iw_tx_binding_cesop cesop_tx_binding_config;
WP_iw_agg_cesop deny_flow_agg_config;
WP_iw_agg_hq violation_config;
WTI_pw_params pw_config;
#if WTI_CESOP_TDI
WP_tdm_slotgroup tdm_slots[WTI_TDM_PORTS][2];
WP_port_tdm tdm_port_config;
WP_tdm_binding trans_binding[WTI_TDM_PORTS];
WP_device_tdm_trans trans_device_config[WTI_TDM_PORTS];
WP_device_tdm_trans_ces tdm_ces_specific[WTI_TDM_PORTS];
WP_device_tdm_trans_ces_pwe3 pwe3_ext_config[WTI_TDM_PORTS];
WTI_tdi_config tdm_config[WTI_TDM_PORTS];
#else /* EMPHY */
WP_port_upi upi_emphy_port_config;
WP_device_emphy_trans upi_trans_device;

WP_port_upi_packet_ext packet_ext;
WP_port_pkt_limits pkt_limits;

#endif

#if WTI_PCE_CLASSIFIER
WP_module_pce_init pce_init;
WP_pce_rule_classification class_cfg;
WP_pce_rule_classification class_cfg_bridge;
WP_pce_filter_classification filter_param;
#if WTI_INFO_STRUCTURES_DEFINED
/* info structures */
WTI_pce_info pce_info_base[WTI_MAX_PW];
#endif
#else
#if WTI_CESOP_MPLS_IW
WP_flow_mpls mpls_flow_cfg;
WP_flow_class_rule class_cfg_bridge;
wp_classification_filter filter_param;
#else
WP_flow_class_rule class_cfg;
WP_flow_class_rule class_cfg_bridge;
wp_classification_filter filter_param;
#if WTI_INFO_STRUCTURES_DEFINED
/* info structures */
WTI_dfc_info dfc_info_base[WTI_MAX_PW];
#endif
#endif
#endif

#ifdef WTI_CESOP_MPLS_IW
WP_iw_agg_mpls_l2_router mpls_layer2_agg_cfg;
#endif /* WTI_CESOP_TO_MPLS_FA_CASCADING */

#if WTI_INFO_STRUCTURES_DEFINED
/* info structures */
WTI_iw_info iw_info_base;
WTI_pw_info pw_info_base[WTI_MAX_PW];
#endif

WP_sync_eth_config sync_eth_config;

/********* Projects Handles **********************/

/*****************************************************************************/
/*****************************************************************************/
/*                         MENU API FUNCTIONS                                */
/*****************************************************************************/
/*****************************************************************************/

void F_MenuEngine     (char * InputBuf);
int  F_EvaluateHotKey (char * InputBuf);
void F_EvaluateCommand(char * InputBuf);
void F_SetMainMenu    (void);
void F_SetUpperMenu   (void);
void F_DisplayMenu    (void);
void F_DisplayHeader  (void);

/*****************************************************************************/
/*****************************************************************************/
/*                         DEMO API FUNCTIONS                                */
/*****************************************************************************/
/*****************************************************************************/

/* BRG of each UPI */
#ifdef WP_HW_WINPATH1
#define WT_BRG_UPI1 WP_BRG1
#define WT_BRG_UPI2 WP_BRG3
#endif
#ifdef WP_HW_WINPATH2
#define WT_BRG_UPI1 WP_BRG3
#define WT_BRG_UPI2 WP_BRG1
#endif
#ifdef WP_HW_WINPATH3
#define WT_BRG_UPI1 WP_BRG3
#define WT_BRG_UPI2 WP_BRG1
#endif


WTI_system *the_system = 0;
#if defined __WT_UFE412__ || defined __WT_UFE448__
WUFE_test_system_setup test_setup;
#endif

/************************************************************************/

#if WTI_CESOP_TDI
void WTI_TdiTransDeviceDelete(WP_U32 index);
#endif

void WTI_CesopSystemStart(void);
void WTI_SystemAlloc(void);
void WTI_SystemInit(void);
void WTI_SetTimestampPrescaler(void);
void WTI_SystemSetup(void);
void WTI_IwSystemSetup(void);
void WTI_PwCreate(int pw_index, int oobc_mode, int tx_clock_master_flag, int rx_clock_master_flag);
void WTI_PwCreateNoPhyEnable(int pw_index, int oobc_mode, int tx_clock_master_flag, int rx_clock_master_flag);
#if WTI_DUAL_EMPHY
void WTI_PwCreateOnSecondUfe(int pw_index,
                             int oobc_mode,
                             int tx_clock_master_flag,
                             int rx_clock_master_flag);
#endif
void WTI_PwEnable(int pw_index);
void WTI_PwDisable(int pw_index);
void WTI_PwDelete(WP_U32 index);
void WTI_PortsEnable(void);
void WTI_EnetChannelsCreate(void);
void WTI_HostRxChannelsCreate(void);
void WTI_HostQnodeCreate(WTI_qnode *qnode);
void WTI_IwPsn2TdmQnodeCreate(WTI_qnode *qnode);
void WTI_IwTdm2PsnQnodeCreate(WTI_qnode *qnode);
void WTI_TransTxQnodeCreate(WTI_qnode *qnode);
void WTI_QnodesCreate(void);
void WTI_EnetPortConfig(WP_port_enet *cfg_ptr);
void WTI_EnetDeviceConfig(WP_device_enet *cfg_ptr, WP_device_enet_ex *ex_cfg_ptr);
void WTI_EnetChannelConfig(WP_ch_enet *cfg_ptr);
void WTI_EnetPortCreate(void);
void WTI_EmphyPortConfig(WP_port_upi *cfg_ptr);
void WTI_TransPortCreate(void);
void WTI_HostPortCreate(void);
#if !WTI_CESOP_TDI
void WTI_UfeInit(WT_ufe *ufe, WP_U32 upi_id, WP_U32 ufe_id);
#endif
void WTI_CesopJitterBufferParamsModify(WP_handle psn2tdm_iw_system,
                                       WP_handle classification_flow,
                                       WP_handle tx_channel_handle,
                                       WP_U32 tx_jitter_buffer_size,
                                       WP_U16 window_tx_threshold,
                                       WP_U8 window_switchover_threshold,
                                       WP_U8 lops_detection,
                                       WP_U8 consec_pkts_in_synch_th,
                                       WP_U8 consec_miss_pkts_out_synch_th);
void WTI_CesopTdm2PsnFlowAggParamsModify(WP_handle rx_channel_handle,
                                         WP_handle tdm2psn_flow_agg,
                                         WP_command command,
                                         WP_iw_agg_cesop *cfg_ptr);
void WTI_CesopPsn2TdmFlowAggParamsModify(WP_handle psn2tdm_iw_system,
                                         WP_handle classification_flow,
                                         WP_handle tx_channel_handle,
                                         WP_handle psn2tdm_flow_agg,
                                         WP_command command,
                                         WP_iw_agg_cesop *cfg_ptr);
void WTI_FlowAggregationTdm2PsnConfigInit(WP_iw_agg_cesop *cfg_ptr);
void WTI_FlowAggregationMef8Tdm2PsnConfigInit(WP_iw_agg_cesop *cfg_ptr);
#if WTI_CESOP_MEF8_ENCAP
void WTI_FlowAggregationMef8Tdm2PsnConfigInit(WP_iw_agg_cesop *cfg_ptr);
void WTI_FlowAggregationMef8Psn2TdmConfigInit(WP_iw_agg_cesop *cfg_ptr);
void WTI_FlowAggregationMef8Tdm2PsnConfig(WP_iw_agg_cesop *cfg_ptr, int pw_index, int oobc_mode);
void WTI_FlowAggregationMef8Psn2TdmConfig(WP_iw_agg_cesop *cfg_ptr, int pw_index, int oobc_mode);
void WTI_FlowAggregationMef8Psn2TdmBridgeConfig(WP_iw_agg_cesop *cfg_ptr, int pw_index, int oobc_mode);
#endif /* WTI_CESOP_MEF8_ENCAP */
void WTI_FlowAggregationPsn2TdmConfigInit(WP_iw_agg_cesop *cfg_ptr);
void WTI_FlowAggregationTdm2PsnConfig(WP_iw_agg_cesop *cfg_ptr, int pw_index, int oobc_mode);
void WTI_FlowAggregationPsn2TdmConfig(WP_iw_agg_cesop *cfg_ptr, int pw_index, int oobc_mode);
void WTI_FlowAggregationPsn2TdmBridgeConfigMef(WP_iw_agg_cesop *cfg_ptr, int pw_index, int oobc_mode);
void WTI_FlowAggregationPsn2TdmBridgeConfig(WP_iw_agg_cesop *cfg_ptr, int pw_index, int oobc_mode);
void WTI_CesopFeatureConfig(WP_feature_iw_cesop *cfg_ptr);

#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
void WTI_ClockRecDcoGlobalDirectInit(WP_U8 sync_delay,WP_U8 clock_selection);
void WTI_ClockRecDcoDirectInit(WP_U32 dco_id,WP_U8 e1_t1,WP_U8 framing_mode,
                               WP_U8 sync_mode,WP_U8 sync_active_mode,
                               WP_U8 sync_delay,WP_U8 clock_selection);

WP_status WTI_ClockRecDcoHwRegistersGet(WP_U32 dco_id,WP_U32 clk_rec_cfg,
                                        WP_U32 clk_recn_reg0,WP_U32 clk_recn_reg1);
WP_status WTI_ClockRecTdiDcoRegistersCheck(WP_U32 dco_id, WP_U32 clk_rec_reg0, WP_U32 clk_rec_reg1,
                                           WP_U32 clk_rec_reg2, WP_U32 clk_rec_fifo_reg);
WP_status WTI_ClockRecTdiDcoRegRead(WP_U32 dco_id, WP_U32 *reg0, WP_U32 *reg1,
                                    WP_U32 *reg2, WP_U32 *fifo_reg);
void WTI_ClockRecDcoParamsCalc(WP_U32 ref_clock,
                               WP_clock_rec_freq *dco_e1_params,
                               WP_clock_rec_freq *dco_t1_params);
void WTI_ClockRecDcoParamsSet(WP_feature_iw_cesop *cfg_ptr, WP_U32 external_ref_clk);
WP_U32 WTI_GetMhzValue(WP_U32 internal_freq);
#endif
void WTI_BoardPinConfig(WP_U32 configuration, WP_U32 board_id, WP_U32 pll_clock_out);
#if WTI_PCE_CLASSIFIER
void WTI_PceFilterConfig(WP_pce_filter_classification *cfg_ptr);
void WTI_PceRuleConfig(WP_pce_rule_classification *cfg_ptr, WP_U32 pw_index);
void WTI_PceRuleConfigBridge(WP_pce_rule_classification *cfg_ptr, WP_U32 pw_index);
void WTI_CheckPceFilterStats(WP_handle filter, WP_U16 diff_denied, WP_U16 diff_matched, WP_U16 diff_no_matched);
#else
void WTI_DefaultDenyRuleConfig(WP_flow_class_rule *cfg_ptr);
void WTI_DfcFilterConfig(wp_classification_filter *cfg_ptr);
void WTI_DfcRuleConfig(WP_flow_class_rule *cfg_ptr, WP_U32 pw_index);
void WTI_DfcRuleConfigBridge(WP_flow_class_rule *cfg_ptr, WP_U32 pw_index);
void WTI_DfcRuleConfigBridgeMef(WP_flow_class_rule *cfg_ptr, WP_U32 pw_index);
#endif
void WTI_Tdm2PsnIwSystemConfig(WP_iw_sys_directmap *cfg_ptr);
void WTI_HostRxChannelConfig(WP_tx_binding *tx_binding_config, WP_ch_iw_rx *cfg_ptr);
void WTI_IwSystemRoutingConfig(WP_iw_sys_routing *cfg_ptr,
                               WP_iw_ipv4_sys_info *ipv4_sys_info);
void WTI_IwSystemBridgingConfig(WP_iw_sys_bridging  *cfg_ptr);
void WTI_RxBindingRoutingConfig(WP_rx_binding_routing *cfg_ptr);
void WTI_RxBindingBridgeConfig(WP_rx_binding_bridging *cfg_ptr);
void WTI_RxBindingDirectMapConfig(WP_rx_binding_directmap *cfg_ptr, int pw_index);
void WTI_TxBindingEnetConfig(WP_tx_binding *cfg_ptr);
void WTI_CesopTxBindingConfig(WP_iw_tx_binding_cesop *cfg_ptr);
void WTI_TransChannelRxConfig(WP_ch_trans_pwe3 *cfg_ptr);
void WTI_TransChannelTxConfig(WP_ch_trans_pwe3 *cfg_ptr);
unsigned short WTI_IpChecksumCalc(unsigned char *buff, unsigned short len);
void WTI_ShowMemStat(void);
void WTI_TermOnError(WP_handle handle, WP_CHAR *s, WP_U32 LineNum);
void WTI_TerminatePrintError(WP_CHAR *s, WP_U32 LineNum);
void WTI_Reboot(void);
void WTI_SystemRelease(void);
#if WTI_CESOP_MPLS_IW
#if (!WTI_CESOP_MPLS_OVER_ENET)
void WTI_HspiPortCreate(void);
void WTI_HspiPortConfig(WP_port_upi *cfg_ptr);
void WTI_HspiDeviceConfig(WP_device_upi_hspos *cfg_ptr);
#endif
void WTI_MplsIwSystemConfig(WP_iw_sys_mpls *cfg_ptr);
void WTI_MplsFlowConfig(WP_flow_mpls *cfg_ptr, int pw_index);
void WTI_MplsRxBindingRoutingConfig(WP_rx_binding_mpls *cfg_ptr);
#endif

void WTI_PTPModuleInit(void);
void WTI_PTPClockSet(WP_U32 frequency);


#endif
