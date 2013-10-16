/*****************************************************************************
 * (C) Copyright 2000-2003, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.

****************************************************************************/
/***************************************************************************
 *  Description:
 *
 *   Bridging System (bridge1) between ENET ---> HDLC
 *   PPPSwitching + Bridge System (bridge2) between HDLC and ENET
 *
 * Traffic
 *
 *    Bridge1: 
 *    Host sends a packet on ENET1 (port is in internal loopback mode)
 *    The packet is received into Bridge1. A DFC classifier rule based on INPUT_IW_PORT, ETHER_TYPE and VLAN is applied that forwards the PACKET to one of the TDM ports (TDM #1 in this example).
 *    TDM 1 was arbitarily picked, it could have been any TDM. 
 *
 *    Note that the flow aggregation that forwards the packet on TDM1, strips off the Ethernet header and prefixes a 4 byte HDLC header to the packet before sending it out on the TDM (hdlc_prefix_header)
 *    If you want to change the hdlc_prefix_header, you will have to recompile the test.
 *
 *    The TDM port is in loopback mode so the packet is received back into the PPP Switching System.
 *
 *    PPP Switch:
 *    NOTE: Address and Protocol Field compression are enabled on the PPP SWITCHING RX BINDING. So a packet that starts with the PPP header will have AFC/PFC applied to it.
 *    Any packet that does not have a PPP header (Cisco HDLC packet) will assumed to have it compressed and will be accepted as is.
 *    This is the only way to accept CISCO HDLC packets into the PPP Switching since they don't have a PPP header (0xFF03)
 *
 *    So 0xFF03C021 is received back into the PPP Switching system as 0xC021 after applying AFC and PFC
 *    So 0xFF030021 is received back into the PPP Switching system as 0x21 after applying AFC and PFC
 *    So 0x0F008035 is received back into the PPP Switching system as 0x0F008035 since no PPP header is present (0xFF03)
 *    So 0x0F000800 is received back into the PPP Switching system as 0x0F000800 since no PPP header is present (0xFF03)
 *
 *    The PPP SWITCHING default flow aggregation (1 per TDM port) adds a dummy 14 byte ethernet header and forwards the packet to Bridge2
 *
 *    Bridge2:
 *    In Bridge2 we 3 user defined filters, that look for a user defined field starting at the ETHER_TYPE reference point of the packet.
 *
 *    The three filters (Filter0, Filter1, Filter2) are used for recognizing a 4 byte,2 byte and 1 byte header respectively (e.g 0x0F008035, 0xC021, 0x21)
 *
 *    Two DFC rules are added for Filter0: 
 *    (1) For CISCO HDLC LCP packets (0x0F008035). This is sent to a flow agg named h_flow_agg_cisco_control and host terminated
 *    (2) For CISCO HDLC IP DATA packets (0x0F000800). This is sent to a flow agg named h_flow_agg_enet2 and sent out of ENET2 
 *
 *    One DFC rule is added for Filter1: 
 *    (1) For PPP LCP packets (0xc021). This is sent to a flow agg named h_flow_agg_lcp and host terminated
 *
 *    One rule is added for Filter2: 
 *    (1) For IP DATA packets (0x21). This is sent to the flow agg named h_flow_agg_enet2 and sent out on ENET2
 *
 *    Since ENET2 is in loopback mode, all packets sent out will be received back on ENET2 RX channel
 *
 *    Finally look at App_Debug that toggles the debug mode for Bridge1 to ether host terminate packets or forward packets on on the TDM
 *
 ************************************************************************************************************************/    
#define APP_DEBUG 0

#define APP_PPPSW 1

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "wp_wddi.h"
#include "wp_sim.h"
#include "wpx_oc12_pmc5357.h"
#include "wt_util.h"

#include "wpx_pin_mux_selector.h"
#include "wpx_tdm_comet.h"
#include "wpx_enet_phy.h"

#define APP_VLAN_ID 0xa
#define APP_MAX_MAC_ADDRESS 10

/* Enet Bridge ports */
#define APP_MAX_BPORTS 20
#define APP_MAX_TDM_PORTS 8

#define APP_NUM_IW_BUFFERS 500
#define APP_IW_BUFFER_SIZE 1984
#define APP_IW_BUFFER_OFFSET 64
#define APP_MASTER_WPID WP_WINPATH(0)

/* Utility macros */
#define MAC_COPY(d,s)           { int _i; \
                                  for(_i=0;_i<6;_i++) \
                                     *((d)+_i)=*((s)+_i); }

extern WP_U32 dps_ProgramImage[];
extern WP_U16 dps_PC_Table_Init[];

WP_context context_cfg =
{
   3, /* qnodes */
   /* the first four assignments are mandatory bus assignments */
   {
      {/* WP_BUS_PARAM    */ WP_BUS_PARAM, APP_BANK_PARAM},
      {/* WP_BUS_PACKET   */ WP_BUS_PACKET, APP_BANK_PACKET},
      {/* WP_BUS_INTERNAL */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
      {/* WP_BUS_HOST     */ WP_BUS_HOST, APP_BANK_HOST},
      {/* WP_BUS_IRQ      */ WP_BUS_HOST, APP_BANK_HOST},
      {/* WP_BUS_TMSTMP   */ WP_BUS_HOST, APP_BANK_HOST},
      {/* WP_VB_CAS       */ WP_BUS_HOST,     APP_BANK_HOST},
      {/* UNASSIGNED      */ WP_BUS_NONE, 0},
      {/* ASU_TPT         */ WP_BUS_PARAM, APP_BANK_PARAM},
      {/* ASU CW          */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
      {/* ASU TX QUEUE    */ WP_BUS_PARAM, APP_BANK_PARAM},
      {/* STATISTICS      */ WP_BUS_HOST, APP_BANK_HOST},
      {/* ADDRESS LOOKUP  */ WP_BUS_PARAM, APP_BANK_PARAM},
      {/* ATM CPT         */ WP_BUS_PARAM, APP_BANK_PARAM},
      {/* ENET HDLC CPT   */ WP_BUS_PARAM, APP_BANK_PARAM},
      {/* IW FBP PARAM    */ WP_BUS_PARAM, APP_BANK_PARAM},
      {/* IW FBP DATA     */ WP_BUS_PACKET, APP_BANK_PACKET},
      {/* EXTERNAL FIFO   */ WP_BUS_PACKET, APP_BANK_PACKET},
      {/* MAC LOOKUP      */ WP_BUS_PARAM, APP_BANK_PARAM},
      {/* IMA SMALL       */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
      {/* IMA LARGE       */ WP_BUS_HOST, APP_BANK_HOST},
      {/* LPM             */ WP_BUS_HOST, APP_BANK_HOST},
      {/* MFC RULES       */ WP_BUS_HOST, APP_BANK_HOST},
      {/* FIWT, QAT       */ WP_BUS_PARAM, APP_BANK_PARAM},
      {/* MFCPT, UPPPT, DS*/ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
      {/* STATISTICS2     */ WP_BUS_PACKET, APP_BANK_PACKET},
      {/* STATISTICS3     */ WP_BUS_PACKET, APP_BANK_PACKET},
      {/* UNASSIGNED      */ WP_BUS_NONE, 0},
      {/* WP_VB_PKT_GTCPT */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
      {/* UNASSIGNED      */ WP_BUS_NONE, 0},
      {/* MANDATORY       */ WP_BUS_NONE, 0},
      {/* MANDATORY       */ WP_BUS_NONE, 0},
      {/* WP_VB_MCH_DATA  */ WP_BUS_PACKET, APP_BANK_PACKET},
#if 1
      {/* WP_VB_OAM_FM_QUERY*/ WP_BUS_NONE, 0},
      {/* WP_VB_HASH_ON_HEAP*/ WP_BUS_NONE, 0},
      {/* WP_VB_MFC_INTERNAL*/ WP_BUS_PACKET, APP_BANK_PACKET}
#endif
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
   /* bus */ WP_BUS_PARAM,
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
   /* pkt_limits             */ {2,2},
   /* flowmode               */ WP_FLOWMODE_FAST,
   /* miimode                */ WP_ENET_RGMII_1000,
   /* rx_iw_bkgnd            */ WP_IW_BKGND_USED,
};

WP_enet_filters enet_filters_cfg = 
{
   /*rx_ucfilter;*/WP_ENET_FILTER_PROCESS,
   /*rx_bcfilter;*/WP_ENET_FILTER_HOSTTERM,
   /*rx_mcfilter;*/WP_ENET_FILTER_HOSTTERM,
};

WP_device_enet device_enet_cfg = 
{
   /* max_tx_channels */ 1,
   /* tx_maxsdu       */ 1536,
   /* rmii_operating_speed */ WP_UNUSED,
   /* mac_addr[6] */     {0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa},
   /* tx_bit_rate */     1000000000,
   /* loopbackmode  */   WP_ENET_LOOPBACK,
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
   /* tx_shaping_type */ WP_PKT_SHAPING_STRICT,
   /* tx_shaping_params */ &default_pkt_shaping_wfq_cfg,
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
   /* WP_U16 max_iw_programmable_filters */ 3,
   /* WP_U16 max_iw_policers */ 1,
};

WP_tx_binding tx_binding_cfg =
{
   /* res0                      */ 0,
   /* dci_mode                  */ WP_IW_DYN_CH_INSERT_ENABLE,
   /* maxt                      */ 10,
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
   /* classification hash size */ WP_IW_32K_HASH_ENTRIES,
   /* port filtering */ WP_IW_PORT_FILTERING_ENABLE,
   /* deny_eq_ip */ WP_IW_DENY_EQUAL_IP_DISABLE,
};

WP_iw_sys_bridging iw_sys_bridge = 
{
   /* tag                       */ 0,
   /* max_flows                 */ 10,
   /* classification mode       */ WP_IW_CLASSIFIER_BASED_BRIDGING,
   /* classifier config         */ {WP_IW_DYNAMIC_CLASS,100,0,NULL},
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
  /*  max_flows */  APP_MAX_TDM_PORTS,
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

/*
 * Global variables used during test execution
 */
WP_U8 wt_mac_enet1[6]    = {0x45, 0x6e, 0x65, 0x74, 0x23, 0x31};
WP_U8 wt_mac_enet2[6]    = {0x45, 0x6e, 0x65, 0x74, 0x23, 0x32};
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
void App_Quit(void);
void App_EventRxIndicate(WP_tag tag, WP_U32 info, WP_U32 data);
void App_EventTxIndicate(WP_tag tag, WP_U32 info, WP_U32 data);
void App_EventErrorIndicate(WP_tag tag, WP_U32 info, WP_U32 data);
void App_ShowStats(void);
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

/* Functions provided by wt_util.c.  */

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
WP_handle               h_bridge1_filter1,h_bridge2_filter[3];
WP_handle               h_port_iwhost,h_dev_iwhost;
WP_handle               h_bridge1_bport_enet1, h_bridge1_bport_tdm[APP_MAX_TDM_PORTS];
WP_handle               h_bridge2_bport_enet2, h_bridge2_bport_tdm[APP_MAX_TDM_PORTS];
WP_handle               h_flow_agg_pppsw_link[APP_MAX_TDM_PORTS];
WP_handle               h_flow_agg_lcp, h_flow_agg_cisco_control,h_flow_agg_enet2;
WP_handle               h_rport_tdm[APP_MAX_TDM_PORTS], h_rport_enet2;
WP_handle               h_enet1_rx, h_enet1_host_tx, h_enet1_tx;
WP_handle               h_enet2_rx, h_enet2_tx;
WP_handle               h_tdm_rx[APP_MAX_TDM_PORTS],h_tdm_tx[APP_MAX_TDM_PORTS];

WP_handle status;

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
WP_tag tag_tdmrx = 200;
WP_tag tag_tdmtx = 300;
WP_tag tag_agg_tdm[APP_MAX_TDM_PORTS];
WP_tag tag_agg_pppsw[APP_MAX_TDM_PORTS];
WP_tag tag_agg_lcp, tag_agg_cisco_control;
WP_boolean debug_on = WP_FALSE;

int main(void)
{
   WP_U32 ii;
   WP_status status;
   app_task *task, a_task;
   WP_CHAR comm;
   WP_U32 tag1,tag2;

   tag_agg_enet= 500;

   tag_iw_sys_bridge1 = 600;
   tag_iw_sys_bridge2 = 601;

   tag_bport_enet1 = 700;
   tag_bport_enet2 = 701;
   for(ii = 0;ii < APP_MAX_TDM_PORTS;ii++)
   {
      tag_bridge1_bport_tdm[ii] = 802 + ii;
      tag_bridge2_bport_tdm[ii] = 902 + ii;
   }
   tag_rport_enet2 = 1000;

   for(ii = 0;ii < APP_MAX_TDM_PORTS;ii++)
   {
      tag_agg_tdm[ii] = 1100 + ii;
      tag_agg_pppsw[ii] = 1200 + ii;
   }

   tag_agg_lcp = 1250;
   tag_agg_cisco_control = 1251;

   tag_agg_enet2 = 1300;

   tag_agg_default_bridge1 = 1400;
   tag_agg_default_bridge2 = 1500;

   tag_enet1_rx = 1600;
   tag_enet1_tx = 1700;

   tag_enet2_rx = 1800;
   tag_enet2_tx = 1900;

   /* Context configuration */
   context_cfg.int_queue_data = &int_queue_table_cfg;
   context_cfg.atm_data = &atm_global_cfg;
   context_cfg.iw_data = &iw_global_cfg;
   context_cfg.pool_limit = 8;
   context_cfg.max_qnodes = 4;

   status = WP_DriverInit();
   App_TerminateOnError(status,"WP_DriverInit()");

   status = WP_SysInit(APP_MASTER_WPID, &context_cfg);
   App_TerminateOnError(status, "WP_SysInit()");

   status = WPU_EnetPhyInit(WP_PORT_ENET1, WPU_MODE_RGMII | WPU_OV_FIBER);
   App_TerminateOnError(status, " WPU_WinnetPhyInit - WP_PORT_ENET1");

   status = WPU_EnetPhyInit(WP_PORT_ENET3, WPU_MODE_RGMII | WPU_OV_FIBER);
   App_TerminateOnError(status, " WPU_WinnetPhyInit - WP_PORT_ENET1");

   /* Create BRG1 & BRG3 */
   status = WP_SysClockCreate(WP_WINPATH(0), WP_BRG1, WP_BRG_SRC_BRGIN2, 2);
   status = WP_SysClockCreate(WP_WINPATH(0), WP_BRG2, WP_BRG_SRC_BRGIN2, 2);
   status = WP_SysClockCreate(WP_WINPATH(0), WP_BRG3, WP_BRG_SRC_BRGIN2, 2);
   status = WP_SysClockCreate(WP_WINPATH(0), WP_BRG4, WP_BRG_SRC_BRGIN2, 2);
   status = WP_SysClockCreate(WP_WINPATH(0), WP_BRG5, WP_BRG_SRC_BRGIN2, 2);

   /* Create Enet1 port */
   h_port_enet1 = WP_PortCreate(APP_MASTER_WPID, WP_PORT_ENET1,
                                &port_enet_cfg);
   App_TerminateOnError(h_port_enet1, "WP_PortCreate() Enet1");

   /* Create device on Enet1 */
   MAC_COPY(device_enet_cfg.mac_addr, wt_mac_enet1);
   h_device_enet1 = WP_DeviceCreate(h_port_enet1, WP_PHY(0), WP_DEVICE_ENET,
                                   &device_enet_cfg);
   App_TerminateOnError(h_device_enet1, "WP_DeviceCreate() Enet1");

   status = WPU_TdmCometCardInit(WP_PORT_TDM1, WPU_INIT_COMET_CPLD_T1);
   App_TerminateOnError(status, "WPU_TdmCometCardInit");

   /* Create Enet2 port */
   h_port_enet2 = WP_PortCreate(APP_MASTER_WPID, WP_PORT_ENET3,
                                &port_enet_cfg);
   App_TerminateOnError(h_port_enet2, "WP_PortCreate() Enet2");

   /* Create device on Enet2 */
   MAC_COPY(device_enet_cfg.mac_addr, wt_mac_enet2);
   h_device_enet2 = WP_DeviceCreate(h_port_enet2, WP_PHY(0), WP_DEVICE_ENET,
                                   &device_enet_cfg);
   App_TerminateOnError(h_device_enet2, "WP_DeviceCreate() Enet1");

   /* Create TDM ports and devices */
   for(ii = 0;ii < APP_MAX_TDM_PORTS;ii++)
   {
      h_port_tdm[ii] = WP_PortCreate(WP_WINPATH(0), WP_PORT_TDM1 + ii, &port_tdm_cfg);
      App_TerminateOnError(h_port_tdm[ii],"WP_PortCreate() TDM1");

      h_dev_tdm[ii] = WP_DeviceCreate(h_port_tdm[ii], WP_PHY(0), WP_DEVICE_HDLC, 
                                                        &device_tdm_hdlc_cfg);
      App_TerminateOnError(h_dev_tdm[ii],"WP_DeviceCreate() TDM1");

      status = WPU_TdmCometCardInit(WP_PORT_TDM1 + ii, WPU_MODE_TDM_T1_MASTER),
      App_TerminateOnError(status, "WPU_TdmCometCardInit()");
   }

   /* Create an IW Host port */
   h_port_iwhost = WP_PortCreate(WP_WINPATH(0), WP_PORT_IW_HOST, NULL);
   App_TerminateOnError(h_port_iwhost, "WP_PortCreate() IW Host");

   /* Create an IW Host Device */
   h_dev_iwhost = WP_DeviceCreate(h_port_iwhost, 0, WP_DEVICE_IW_HOST, NULL);
   App_TerminateOnError(h_dev_iwhost, "WP_DeviceCreate() IW Host");

      /* Create Buffer pools */
   h_pool_buffer_iw = WP_PoolCreate(APP_MASTER_WPID, WP_pool_iwbuffer,
                                    &pool_buffer_data_iw_cfg);
   App_TerminateOnError(h_pool_buffer_iw, "WP_PoolCreate iwbuffer iw");


   /* Pool for host termination buffers */
   h_pool_256 = WP_PoolCreate(WP_WINPATH(0), WP_pool_buffer, 
                                  &pool_buffer_data_256_cfg);
   App_TerminateOnError (h_pool_256, "PoolCreate() pool_256");

   h_pool_ring_host = WP_PoolCreate(WP_WINPATH(0), WP_pool_ring, 
                                    &pool_ring_data_cfg);
   App_TerminateOnError(h_pool_ring_host, "PoolCreate() pool_ring_host");

   /* Create an interworking queue node */
   qnode_iwq_cfg.adjunct_pool = h_pool_buffer_iw;
   h_qnode_iwq = WP_QNodeCreate(APP_MASTER_WPID,WP_QNODE_IWQ | WP_QNODE_OPT_DEDICATED_RX_HWQ | WP_QNODE_OPT_FMU,
                                &qnode_iwq_cfg);
   App_TerminateOnError(h_qnode_iwq, "WP_QNodeCreate() h_qnode_iwq");

   /* Create an interworking queue node */
   qnode_iwq_cfg.adjunct_pool = h_pool_buffer_iw;
   h_qnode_iwq_tdm = WP_QNodeCreate(APP_MASTER_WPID,WP_QNODE_IWQ, &qnode_iwq_cfg);
   App_TerminateOnError(h_qnode_iwq_tdm, "WP_QNodeCreate() h_qnode_iwq_tdm");

   /* Host termination qnode */
   qnode_hostq_cfg.pool_buffer = h_pool_256;
   qnode_hostq_cfg.pool_ring = h_pool_ring_host;
   h_qnode_host = WP_QNodeCreate(WP_WINPATH(0), WP_QNODE_HOSTQ, &qnode_hostq_cfg);
   App_TerminateOnError(h_qnode_host, "QnodeCreate() h_qnode_mp");

   status = WP_ControlRegister(WP_EVENT_RX_INDICATE, App_EventRxIndicate);
   App_TerminateOnError(status, "WP_ControlRegister()");

   status = WP_ControlRegister(WP_EVENT_TX_INDICATE, App_EventTxIndicate);
   App_TerminateOnError(status, "WP_ControlRegister()");

   status = WP_ControlRegister(WP_EVENT_STATUS_INDICATE, App_EventErrorIndicate);
   App_TerminateOnError(status, "WP_ControlRegister()");

   status = WP_SysSchedulerCreate(WP_WINPATH(0), cw_config);
   App_TerminateOnError(status, "WP_SysSchedulerCreate()");

   status = WP_SysCommit();     /* Commit system resources */
   App_TerminateOnError(status,"WP_SysCommit()");

  /* Create Enet channels */
   h_enet1_rx = WP_ChannelCreate(tag_enet1_rx, h_device_enet1, 
                                 h_qnode_iwq,WP_CH_RX, WP_ENET,
                                 &ch_enet_cfg);
   App_TerminateOnError(h_enet1_rx, "WP_ChannelCreate() Enet Rx");

   /* NOTE this channel is in NON-IW mode, used to send out a packet from the host out of ENET1 */
   ch_enet_cfg.iwmode = WP_PKTCH_IWM_DISABLE;
   h_enet1_host_tx = WP_ChannelCreate(tag_enet1_tx++, h_device_enet1, 
                                 h_qnode_iwq,WP_CH_TX, WP_ENET,
                                 &ch_enet_cfg);
   App_TerminateOnError(h_enet1_host_tx, "WP_ChannelCreate() Enet Tx");

   tx_binding_cfg.dci_mode = WP_IW_DYN_CH_INSERT_ENABLE;
   status = WP_IwTxBindingCreate(h_enet1_host_tx, WP_IW_TX_BINDING, &tx_binding_cfg);
   App_TerminateOnError(h_enet1_host_tx, "WP_IwTxBindingCreate()");

   /* Create Enet 2 channels */
   ch_enet_cfg.iwmode = WP_PKTCH_IWM_DISABLE;
   h_enet2_rx = WP_ChannelCreate(tag_enet2_rx, h_device_enet2, 
                                 h_qnode_iwq,WP_CH_RX, WP_ENET,
                                 &ch_enet_cfg);
   App_TerminateOnError(h_enet2_rx, "WP_ChannelCreate() Enet Rx");

   ch_enet_cfg.iwmode = WP_PKTCH_IWM_ENABLE;
   h_enet2_tx = WP_ChannelCreate(tag_enet2_tx, h_device_enet2, 
                                 h_qnode_iwq,WP_CH_TX, WP_ENET,
                                 &ch_enet_cfg);
   App_TerminateOnError(h_enet2_tx, "WP_ChannelCreate() Enet Tx");
  
   tx_binding_cfg.dci_mode = WP_IW_DYN_CH_INSERT_ENABLE;
   status = WP_IwTxBindingCreate(h_enet2_tx, WP_IW_TX_BINDING, &tx_binding_cfg);
   App_TerminateOnError(h_enet2_tx, "WP_IwTxBindingCreate()");

   tag1 = tag_tdmrx;
   tag2 = tag_tdmtx;

   /* Create TDM RX and TX Channels */
   for(ii = 0;ii < APP_MAX_TDM_PORTS;ii++)
   {
      ch_hdlc_cfg.intmode = WP_PKTCH_INT_ENABLE;
      h_tdm_rx[ii] = WP_ChannelCreate(tag1++, h_dev_tdm[ii], h_qnode_host, 
                                   WP_CH_RX, WP_HDLC, &ch_hdlc_cfg);
      App_TerminateOnError(h_tdm_rx[ii], "WP_ChannelCreate() h_tdm_rx");

      ch_hdlc_cfg.intmode = WP_PKTCH_INT_DISABLE;
      h_tdm_tx[ii] = WP_ChannelCreate(tag2++, h_dev_tdm[ii], h_qnode_iwq_tdm, 
                                   WP_CH_TX, WP_HDLC, &ch_hdlc_cfg);
      App_TerminateOnError(h_tdm_tx[ii], "WP_ChannelCreate() h_tdm_tx1");

      status = WP_IwTxBindingCreate(h_tdm_tx[ii], WP_IW_TX_BINDING, &tx_binding_cfg);
      App_TerminateOnError(status, "WP_IwTxBindingCreate()");
   }

   WT_Identify(); /* Identify the testcase */

   /* Crate IW HOST termination channels one for each bridge */
   tx_binding_cfg.dci_mode = WP_IW_DYN_CH_INSERT_DISABLE;
   ch_iw_rx_cfg.tx_binding_config = &tx_binding_cfg;
   h_bridge1_iwhost = WP_ChannelCreate(90,
                                     h_dev_iwhost,
                                     h_qnode_iwq,
                                     WP_CH_RX,
                                     WP_IW_HOST,
                                     &ch_iw_rx_cfg);
   App_TerminateOnError(h_bridge1_iwhost, "WP_ChannelCreate() Host atm");

   h_bridge2_iwhost = WP_ChannelCreate(91,
                                     h_dev_iwhost,
                                     h_qnode_iwq,
                                     WP_CH_RX,
                                     WP_IW_HOST,
                                     &ch_iw_rx_cfg);
   App_TerminateOnError(h_bridge2_iwhost, "WP_ChannelCreate() Host atm");

   App_CreateEnetHdlcBridge();  /* Sets up ENET-->HDLC interworking */
   App_CreateHdlcEnetBridge();  /* Sets up HDLC-->ENET interworking */

   status = WP_SysPsuEnable(WP_WINPATH(0));
   App_TerminateOnError(status, "WP_SysPsuEnable()");

   /* Enable all ports and devices */
   status = WP_PortEnable(h_port_enet1, WP_DIRECTION_DUPLEX);
   App_TerminateOnError(status, "WP_PortEnable() Enet1");

   status = WP_DeviceEnable(h_device_enet1, WP_DIRECTION_DUPLEX);
   App_TerminateOnError(status, "WP_DeviceEnable() Enet1");

   status = WP_PortEnable(h_port_enet2, WP_DIRECTION_DUPLEX);
   App_TerminateOnError(status, "WP_PortEnable() Enet2");

   status = WP_DeviceEnable(h_device_enet2, WP_DIRECTION_DUPLEX);
   App_TerminateOnError(status, "WP_DeviceEnable() Enet2");

   for(ii = 0;ii < APP_MAX_TDM_PORTS;ii++)
   {
      status = WP_PortEnable(h_port_tdm[ii], WP_DIRECTION_DUPLEX);
      App_TerminateOnError(status,"WP_PortEnable() TDM ");

      status = WP_DeviceEnable(h_dev_tdm[ii], WP_DIRECTION_DUPLEX);
      App_TerminateOnError(status,"WP_DeviceEnable()");
   }

   status = WP_ChannelEnable(h_enet1_rx);
   App_TerminateOnError(status, "WP_ChannelEnable ENET1 RX");

   status = WP_ChannelEnable(h_enet1_host_tx);
   App_TerminateOnError(status, "WP_ChannelEnable ENET1 RX");

   status = WP_ChannelEnable(h_enet2_rx);
   App_TerminateOnError(status, "WP_ChannelEnable ENET2 RX");

   status = WP_ChannelEnable(h_enet2_tx);
   App_TerminateOnError(status, "WP_ChannelEnable ENET2 RX");

   for(ii = 0;ii < APP_MAX_TDM_PORTS;ii++)
   {
       status = WP_ChannelEnable(h_tdm_tx[ii]);
       App_TerminateOnError(status, "WP_ChannelEnable TDM TX");
   }

   while(1)
   {
      printf("a. Stats\n");
      printf("d. Debug\n");
      printf("p. Send Packet\n");
      printf("s. Simulate Interrupts\n");
      printf("x. Exit\n");
      comm = getchar();
      switch(comm)
      {
         case 'a':
            App_ShowStats();
            break;
         case 'd':
            App_Debug(debug_on);
            break;
         case 'p':
            App_DataSend(h_enet1_host_tx, h_pool_buffer_iw);
            break;
         case 's':
            WPI_SimulateInterrupts();
            break;
         case 'x':
            exit(0);
      }
      WPI_SimulateInterrupts();
      while((task = next_task(irq_task_list, &a_task)) != NULL)
         app_perform_action(task);
   }

   return 0;
}

void App_CreateHdlcEnetBridge(void)
{
   WP_U32 ii=0,jj=0,kk = 0;
   WP_status   status;
   WP_U8 dummy_prefix_header[APP_MAX_TDM_PORTS][14]; 
   WP_dfc_flow_info dfc_flow_info;
   WP_flow_class_rule flow_class_rule;
   WP_CHAR rule_string[128];
   WP_classification_filter classification_filter;
   WP_flow_class_forwarding_action forwarding_action;
   WP_class_filter_user_fields_config filter_user_fields;
   WP_iw_agg_bridging iw_agg_bridging_cfg;

   /* Dummy mac addresses used to send into Bridge2 */
   for(ii = 0;ii < APP_MAX_TDM_PORTS;ii++)
   {
      for(jj = 0;jj < 12;jj++)
      {
         dummy_prefix_header[ii][jj] = jj;
      }

      dummy_prefix_header[ii][12] = 0x08;
      dummy_prefix_header[ii][13] = 0x00;
   }

   /* PPP Switching System */
   h_iw_sys_pppsw = WP_IwSystemCreate(WP_WINPATH(0), WP_IW_PPP_SWITCHING_MODE, 
                                       &iw_sys_pppsw_cfg);
   App_TerminateOnError(h_iw_sys_pppsw, "WP_IwSystemCreate() pppsw");

   /* Bridge2*/
   iw_sys_bridge.tag = tag_iw_sys_bridge2;
   h_iw_sys_bridge2 = WP_IwSystemCreate(APP_MASTER_WPID,
                                   WP_IW_VLAN_AWARE_BRIDGING_MODE,
                                   &iw_sys_bridge);
   App_TerminateOnError(h_iw_sys_bridge2," WP_IwSystemCreate() Bridge2");

   /* Create 3 USER PROGRAMMABLE FILTERS */

   /* 1. FOR CISCO HDLC TRAFFIC CONTROL AND DATA */
   classification_filter.fields_mask = WP_FIELD_IDS_ARRAY;
   classification_filter.ip_address_config.ip_sa_prefix_size = 32;
   classification_filter.ip_address_config.ip_da_prefix_size = 32;
   classification_filter.filter_type = WP_CLASS_FILTER_EMC;
   classification_filter.field_ids_array[0] = WP_FIELD_ID_IN_PORT;
   classification_filter.field_ids_array[1] = WP_FIELD_ID_USER_PROGRAMMABLE;
   classification_filter.field_ids_array[2] = WP_FIELD_ID_LAST;
   classification_filter.user_fields_config.num_of_user_fields = 1;
   classification_filter.user_fields_config.ref_point = WP_CLASS_REF_POINT_ETHER_TYPE;
   classification_filter.user_fields_config.user_fields[0].field_offset = 0;

   classification_filter.user_fields_config.user_fields[0].field_size = 4;
   classification_filter.no_match_action = WP_CLASS_FILTER_NO_MATCH_CONTINUE;
   h_bridge2_filter[0] = WP_IwClassFilterAdd(h_iw_sys_bridge2, &classification_filter);
   App_TerminateOnError(h_bridge2_filter[0]," WP_IwClassFilterAdd() Bridge2");

   /* 2. FOR REGULAR PPP CONTROL TRAFFIC */
   classification_filter.user_fields_config.user_fields[0].field_size = 2;
   classification_filter.no_match_action = WP_CLASS_FILTER_NO_MATCH_CONTINUE;
   h_bridge2_filter[1] = WP_IwClassFilterAdd(h_iw_sys_bridge2, &classification_filter);
   App_TerminateOnError(h_bridge2_filter[1]," WP_IwClassFilterAdd() Bridge2");

   /* 3. FOR REGULAR PPP IP TRAFFIC */
   classification_filter.user_fields_config.user_fields[0].field_size = 1;
   classification_filter.no_match_action = WP_CLASS_FILTER_NO_MATCH_DENY;
   h_bridge2_filter[2] = WP_IwClassFilterAdd(h_iw_sys_bridge2, &classification_filter);
   App_TerminateOnError(h_bridge2_filter[2]," WP_IwClassFilterAdd() Bridge2");

   memcpy(&iw_agg_bridging_cfg, &default_iw_agg_bridging_cfg, sizeof(WP_iw_agg_bridging));

   iw_agg_bridging_cfg.intmode = WP_IW_INT_ENABLE;

   /* Create Default Aggregation for Bridge */
   iw_agg_bridging_cfg.tag = tag_agg_default_bridge2;
   iw_agg_bridging_cfg.txfunc = h_bridge2_iwhost;
   iw_agg_bridging_cfg.input_bport = 0;
   iw_agg_bridging_cfg.intmode = WP_IW_INT_ENABLE;

   h_bridge2_default_agg = WP_IwFlowAggregationCreate(
                                               APP_MASTER_WPID,
                                               WP_IW_VLAN_AWARE_BRIDGING_MODE,
                                               &iw_agg_bridging_cfg);
   App_TerminateOnError(h_bridge2_default_agg," WP_IwFlowAggregationCreate() Bridge2 Default");

   iw_agg_bridging_cfg.intmode = WP_IW_INT_DISABLE;

   for(ii = 0;ii < APP_MAX_TDM_PORTS;ii++)
   {
      WP_rx_binding_ppp_switching rx_binding_pppsw;
      WP_feature_iw_pppsw_rx_lcp_forwarding pppsw_lcp;

      /* Create a bridge port for each TDM port */
      bridge_port_cfg.tag = tag_bridge2_bport_tdm[ii];
      bridge_port_cfg.flow_agg = h_bridge2_default_agg;
      bridge_port_cfg.input_filters_mask = 0xe0000000;
      bridge_port_cfg.classification_flag = WP_IW_IWPORT_CLASSIFICATION_ENABLED;
      h_bridge2_bport_tdm[ii] = WP_IwBridgingPortCreate(h_iw_sys_bridge2,
                                               &bridge_port_cfg);
      App_TerminateOnError(h_bridge2_bport_tdm[ii],"WP_IwBridgingPortCreate() Bridge2 TDM");

      /* Create a PPP Switching flow agg for each TDM port */
      iw_agg_ppp_switching.tag = tag_agg_pppsw[ii];
      iw_agg_ppp_switching.txfunc = h_iw_sys_bridge2;
      iw_agg_ppp_switching.iw_port = h_bridge2_bport_tdm[ii];
      iw_agg_ppp_switching.prefix_length = 14;
      iw_agg_ppp_switching.l2_header_insert_mode = WP_IW_L2H_INSERT_ENABLE;

      memcpy(&iw_agg_ppp_switching.prefix_header, &dummy_prefix_header[ii],14);

      h_flow_agg_pppsw_link[ii] = WP_IwFlowAggregationCreate(
                                               WP_WINPATH(0),
                                               WP_IW_PPP_SWITCHING_MODE,
                                               &iw_agg_ppp_switching);
      App_TerminateOnError(h_flow_agg_pppsw_link[ii]," WP_IwFlowAggregationCreate() PPPSW");
 
      /* Bind the TDM RX channel to the PPP Switching System using this flow agg */
      rx_binding_pppsw.aggregation = h_flow_agg_pppsw_link[ii];
      rx_binding_pppsw.mru = 1536;
      rx_binding_pppsw.encap_mode = WP_IW_VCMUX;
      rx_binding_pppsw.acfc_mode = 1;
      rx_binding_pppsw.pfc_mode = 1;
      rx_binding_pppsw.pppmux_ncp_filter = WP_IW_PPPMUX_NCP_FILTER_SWITCH;
      rx_binding_pppsw.forward_ppp_to_bundle_mode = WP_IW_PPPSW_FORWARD_PPP_TO_LINK;
   
      status = WP_IwRxBindingCreate(h_tdm_rx[ii],
                                    h_iw_sys_pppsw,
                                    h_qnode_iwq,
                                    &rx_binding_pppsw);
      App_TerminateOnError(status," WP_IwRxBindingCreate() PPPSW");

      /* ENABLE LCP forwarding on the TDM RX channel, so that LCP packets come into the PPP Switching system instead of being host terminated */
      pppsw_lcp.iw_system = h_iw_sys_pppsw;
      pppsw_lcp.aggregation = h_flow_agg_pppsw_link[ii];
      status = WP_FeatureInit(h_tdm_rx[ii], WP_FEATURE_IW_PPPSW_RX_LCP_FORWARDING_MODE, &pppsw_lcp);
      App_TerminateOnError(status , "WP_FeatureInit LCP");
   }

   /* Create a bridge port to represent ENET2 */
   bridge_port_cfg.tag = tag_bport_enet2;
   bridge_port_cfg.flow_agg = h_bridge2_default_agg;
   bridge_port_cfg.input_filters_mask = 0;
   bridge_port_cfg.classification_flag = WP_IW_IWPORT_CLASSIFICATION_DISABLED;
   h_bridge2_bport_enet2 = WP_IwBridgingPortCreate(h_iw_sys_bridge2,
                                            &bridge_port_cfg);
   App_TerminateOnError(h_bridge2_bport_enet2,"WP_IwBridgingPortCreate() ENET2");

   /* LCP flow agg - PPP LCP packets (0xC021) go here */
   iw_agg_directmap_cfg.tag = tag_agg_lcp;
   iw_agg_directmap_cfg.txfunc = h_bridge2_iwhost;
   iw_agg_directmap_cfg.iw_port = 0;
   iw_agg_directmap_cfg.extraction_length = 14;
   iw_agg_directmap_cfg.prefix_length = 0;
   iw_agg_directmap_cfg.l2_header_insert_mode = WP_IW_L2H_INSERT_DISABLE;
   iw_agg_directmap_cfg.intmode = WP_IW_INT_ENABLE;

   h_flow_agg_lcp  = WP_IwFlowAggregationCreate(WP_WINPATH(0), WP_IW_DIRECTMAP_MODE,
                                            &iw_agg_directmap_cfg);
   App_TerminateOnError(h_flow_agg_lcp," WP_IwFlowAggregationCreate() LCP");

   /* Cisco HDLC Control Packets go here to this flow agg (0x0F008035)  */
   iw_agg_directmap_cfg.tag = tag_agg_cisco_control;
   iw_agg_directmap_cfg.txfunc = h_bridge2_iwhost;
   iw_agg_directmap_cfg.iw_port = 0;
   iw_agg_directmap_cfg.extraction_length = 14;
   iw_agg_directmap_cfg.prefix_length = 0;
   iw_agg_directmap_cfg.l2_header_insert_mode = WP_IW_L2H_INSERT_DISABLE;
   iw_agg_directmap_cfg.intmode = WP_IW_INT_ENABLE;

   h_flow_agg_cisco_control  = WP_IwFlowAggregationCreate(WP_WINPATH(0), WP_IW_DIRECTMAP_MODE,
                                            &iw_agg_directmap_cfg);
   App_TerminateOnError(h_flow_agg_cisco_control," WP_IwFlowAggregationCreate() CC");

   /* Data packets go here - both PPP IP (0x21) and CISCO HDLC IP (0x0F000800) */
   iw_agg_directmap_cfg.tag = tag_agg_cisco_control;
   iw_agg_bridging_cfg.tag = tag_agg_enet2;
   iw_agg_bridging_cfg.txfunc = h_enet2_tx;
   iw_agg_bridging_cfg.iw_port = h_bridge2_bport_enet2;

   h_flow_agg_enet2  = WP_IwFlowAggregationCreate(
                                    APP_MASTER_WPID,
                                    WP_IW_VLAN_AWARE_BRIDGING_MODE,
                                    &iw_agg_bridging_cfg);
   App_TerminateOnError(h_flow_agg_enet2," WP_IwFlowAggregationCreate() ENET2  ");

   /* Add DFC flows */

   /* NOTE for this test we are using TDM#1 to send out and receive the packet, so we use the bridge port for TDM 1 */
   dfc_flow_info.input_port = h_bridge2_bport_tdm[1];  /* Note that Input_port is one of the fields in the filter */
   dfc_flow_info.output_port = 0;

   /* While adding these flows, make sure to use the correct filter for each flow */

   /* CISCO HDLC Control packets flow */
   dfc_flow_info.filter_handle = h_bridge2_filter[0];
   strcpy(rule_string, "n;n;n;n;n;n;#;0x0f008035");

   forwarding_action.flow_aggregation = h_flow_agg_cisco_control;
   flow_class_rule.classifier_string = rule_string;
   flow_class_rule.dfc_info = &dfc_flow_info;
   flow_class_rule.match_action = WP_FLOW_CLASS_MODE_FLOW_AGG;
   flow_class_rule.action_info.forwarding_action = forwarding_action;
   flow_class_rule.wred_entry = 0;

   h_bridge2_dfcflow[0] = WP_IwFlowAdd(h_iw_sys_bridge2, WP_FLOW_CLASS_RULE, &flow_class_rule);
   App_TerminateOnError(h_bridge2_dfcflow[0]," WP_IwFlowAdd() DFC Bridge2");

   /* CISCO HDLC Data packets flow */ /* Same filter as above using 4 byte user fields */
   dfc_flow_info.filter_handle = h_bridge2_filter[0];
   strcpy(rule_string, "n;n;n;n;n;n;#;0x0f000800");

   forwarding_action.flow_aggregation = h_flow_agg_enet2;
   flow_class_rule.classifier_string = rule_string;
   flow_class_rule.dfc_info = &dfc_flow_info;
   flow_class_rule.match_action = WP_FLOW_CLASS_MODE_FLOW_AGG;
   flow_class_rule.action_info.forwarding_action = forwarding_action;
   flow_class_rule.wred_entry = 0;

   h_bridge2_dfcflow[1] = WP_IwFlowAdd(h_iw_sys_bridge2, WP_FLOW_CLASS_RULE, &flow_class_rule);
   App_TerminateOnError(h_bridge2_dfcflow[1]," WP_IwFlowAdd() DFC Bridge2");

   /* PPP LCP flow */ /* Note the use of FILTER 1 */
   dfc_flow_info.filter_handle = h_bridge2_filter[1];
   strcpy(rule_string, "n;n;n;n;n;n;#;0xc021");

   forwarding_action.flow_aggregation = h_flow_agg_lcp;
   flow_class_rule.classifier_string = rule_string;
   flow_class_rule.dfc_info = &dfc_flow_info;
   flow_class_rule.match_action = WP_FLOW_CLASS_MODE_FLOW_AGG;
   flow_class_rule.action_info.forwarding_action = forwarding_action;
   flow_class_rule.wred_entry = 0;

   h_bridge2_dfcflow[2] = WP_IwFlowAdd(h_iw_sys_bridge2, WP_FLOW_CLASS_RULE, &flow_class_rule);
   App_TerminateOnError(h_bridge2_dfcflow[2]," WP_IwFlowAdd() DFC Bridge2");

   /* PPP IP flow */ /* Note the use of FILTER 2 */
   dfc_flow_info.filter_handle = h_bridge2_filter[2];
   strcpy(rule_string, "n;n;n;n;n;n;#;0x21");

   forwarding_action.flow_aggregation = h_flow_agg_enet2;
   flow_class_rule.classifier_string = rule_string;
   flow_class_rule.dfc_info = &dfc_flow_info;
   flow_class_rule.match_action = WP_FLOW_CLASS_MODE_FLOW_AGG;
   flow_class_rule.action_info.forwarding_action = forwarding_action;
   flow_class_rule.wred_entry = 0;

   h_bridge2_dfcflow[3] = WP_IwFlowAdd(h_iw_sys_bridge2, WP_FLOW_CLASS_RULE, &flow_class_rule);
   App_TerminateOnError(h_bridge2_dfcflow[3]," WP_IwFlowAdd() DFC Bridge2");

   /* Be sure to build the system */
   status = WP_IwSystemBuild(h_iw_sys_bridge2);
   App_TerminateOnError(status," WP_IwSystemBuild() Bridge2");
}

void App_CreateEnetHdlcBridge()
{
   WP_U32 ii=0,jj=0,kk = 0;
   WP_status   status;
   WP_flow_route_ipv4 flow_route_cfg;
   WP_dfc_flow_info dfc_flow_info;
   WP_flow_class_rule flow_class_rule;
   WP_CHAR rule_string[128];
   WP_classification_filter classification_filter;
   WP_flow_class_forwarding_action forwarding_action;
   WP_iw_agg_bridging iw_agg_bridging_cfg;

   /* Bridge1 ENET --> TDM */
   iw_sys_bridge.tag = tag_iw_sys_bridge1;
   h_iw_sys_bridge1 = WP_IwSystemCreate(APP_MASTER_WPID,
                                   WP_IW_VLAN_AWARE_BRIDGING_MODE,
                                   &iw_sys_bridge);
   App_TerminateOnError(h_iw_sys_bridge1," WP_IwSystemCreate() Bridge1");

    /* DFC filter using 3 fields */
   classification_filter.fields_mask = WP_FIELD_IDS_ARRAY;
   classification_filter.no_match_action = WP_CLASS_FILTER_NO_MATCH_DENY;

   /* Using 3 fields, include the input bridge port */
   classification_filter.field_ids_array[0] = WP_FIELD_ID_VLAN_ID;
   classification_filter.field_ids_array[1] = WP_FIELD_ID_ETHER_TYPE;
   classification_filter.field_ids_array[2] = WP_FIELD_ID_IN_PORT;
   classification_filter.field_ids_array[3] = WP_FIELD_ID_LAST;
   classification_filter.ip_address_config.ip_sa_prefix_size = 32;
   classification_filter.ip_address_config.ip_da_prefix_size = 32;
   classification_filter.filter_type = WP_CLASS_FILTER_EMC;

   h_bridge1_filter1 = WP_IwClassFilterAdd(h_iw_sys_bridge1, &classification_filter);
   App_TerminateOnError(h_bridge1_filter1," WP_IwClassFilterAdd() Bridge1");

   memcpy(&iw_agg_bridging_cfg, &default_iw_agg_bridging_cfg, sizeof(WP_iw_agg_bridging));

   /* Create Default Aggregation Bridge1 */
   iw_agg_bridging_cfg.tag = tag_agg_default_bridge1;
   iw_agg_bridging_cfg.txfunc = h_bridge1_iwhost;
   iw_agg_bridging_cfg.input_bport = 0;
   iw_agg_bridging_cfg.intmode = WP_IW_INT_ENABLE;

   h_bridge1_default_agg = WP_IwFlowAggregationCreate(
                                               APP_MASTER_WPID,
                                               WP_IW_VLAN_AWARE_BRIDGING_MODE,
                                               &iw_agg_bridging_cfg);
   App_TerminateOnError(h_bridge1_default_agg," WP_IwFlowAggregationCreate() Default Bridge");
   iw_agg_bridging_cfg.intmode = WP_IW_INT_DISABLE;

   bridge_port_cfg.tag = tag_bport_enet1;
   bridge_port_cfg.flow_agg = h_bridge1_default_agg;
   bridge_port_cfg.input_filters_mask = 0x80000000;
   h_bridge1_bport_enet1 = WP_IwBridgingPortCreate(h_iw_sys_bridge1,
                                            &bridge_port_cfg);
   App_TerminateOnError(h_bridge1_bport_enet1,"WP_IwBridgingPortCreate()ENET1");

   /* Rx binding ENET 1 */
   rx_binding_bridging_cfg.input_port = h_bridge1_bport_enet1;
   status = WP_IwRxBindingCreate(h_enet1_rx,
                                 h_iw_sys_bridge1,
                                 h_qnode_iwq,
                                 &rx_binding_bridging_cfg);
   App_TerminateOnError(status," WP_IwRxBindingCreate() enet1 ");

   /* EACH TDM port is represented as bridge port in Bridge1 too so a packet going from ENET to TDM has the ENET bridge port as input bridge port and TDM
    * bridge port as the output bridge port */
   for(ii = 0;ii < APP_MAX_TDM_PORTS;ii++)
   {
      bridge_port_cfg.tag = tag_bridge1_bport_tdm[ii];
      bridge_port_cfg.flow_agg = h_bridge1_default_agg;
      bridge_port_cfg.max_mac_addresses = APP_MAX_MAC_ADDRESS;
      bridge_port_cfg.input_filters_mask = 0;
      bridge_port_cfg.classification_flag = WP_IW_IWPORT_CLASSIFICATION_DISABLED;
      h_bridge1_bport_tdm[ii] = WP_IwBridgingPortCreate(h_iw_sys_bridge1,
                                               &bridge_port_cfg);
      App_TerminateOnError(h_bridge1_bport_tdm[ii],"WP_IwBridgingPortCreate()TDM");
   }

   /* Create flow aggregations that send the packet out on each TDM */
   /* NOTE THE 4 BYTE HDLC header */ /* To test different scenarios you need to change the 4 byte header for now */
   /* ALSO NOTE the 18 bytes being extracted */

   for(ii = 0;ii < APP_MAX_TDM_PORTS;ii++)
   {
      WP_U8 hdlc_prefix_header[4] = {0x0f,0x00,0x80,0x35};
      iw_agg_bridging_cfg.tag = tag_agg_tdm[ii];
      iw_agg_bridging_cfg.txfunc = h_tdm_tx[ii];
      iw_agg_bridging_cfg.iw_port = h_bridge1_bport_tdm[ii];
      iw_agg_bridging_cfg.extraction_length = 18;
      iw_agg_bridging_cfg.prefix_length = 4;
      iw_agg_bridging_cfg.l2_header_insert_mode = WP_IW_L2H_INSERT_ENABLE;
      memcpy(iw_agg_bridging_cfg.prefix_header, hdlc_prefix_header, 4);

      h_flow_agg_tdm[ii] = WP_IwFlowAggregationCreate(
                                    APP_MASTER_WPID,
                                    WP_IW_VLAN_AWARE_BRIDGING_MODE,
                                    &iw_agg_bridging_cfg);
      App_TerminateOnError(h_flow_agg_tdm[ii]," WP_IwFlowAggregationCreate() TDM ");
   }

   iw_agg_bridging_cfg.prefix_length = 0;
   iw_agg_bridging_cfg.l2_header_insert_mode = WP_IW_L2H_INSERT_DISABLE;
   iw_agg_bridging_cfg.extraction_length = 0;

   /* Add DFC flow */
   dfc_flow_info.filter_handle = h_bridge1_filter1;
   dfc_flow_info.input_port = h_bridge1_bport_enet1;  /* Note that Input_port is one of the fields in the filter */
   dfc_flow_info.output_port = 0;

   forwarding_action.flow_aggregation = h_flow_agg_tdm[1];

   /* Add VLAN 10 ETHER TYPE = 0x800*/
   /* Note: Input Bridge port not passed in string but passed in above */
   sprintf(rule_string, "n;n;n;n;n;n;n;n;n;n;10;n;n;0x800;n;n;n;n;n;n;n;n;n;n;n;n;n;n;n;n;n;n");

   flow_class_rule.classifier_string = rule_string;
   flow_class_rule.dfc_info = &dfc_flow_info;
   flow_class_rule.match_action = WP_FLOW_CLASS_MODE_FLOW_AGG;
   flow_class_rule.action_info.forwarding_action = forwarding_action;
   flow_class_rule.wred_entry = 0;

   h_bridge1_dfcflow1 = WP_IwFlowAdd(h_iw_sys_bridge1, WP_FLOW_CLASS_RULE, &flow_class_rule);
   App_TerminateOnError(h_bridge1_dfcflow1," WP_IwFlowAdd() DFC Bridge1");

   status = WP_IwSystemBuild(h_iw_sys_bridge1);
   App_TerminateOnError(status," WP_IwSystemBuild() Bridge1");
}

/* Send a Ethernet packet on the channel handle passed in */
void App_DataSend(WP_handle h_tx, WP_handle h_pool)
{
#define APP_DATA_LENGTH 80

   WP_data_unit data_unit = {0} ;
   WP_data_segment bufs = {0};
   WP_status status;
   WP_U8 * km_data_ptr;
   WP_U32 ii;
   WP_U8 dst_mac[6] = {0x00,0x00,0x00,0x00,0x00,0x01};
   WP_U8 src_mac[6] = {0x00,0x00,0x00,0x00,0x00,0x02};
   WP_U8 vlan_1[4] = {0x81,0x00,0x00,0xa};
   WP_U8 e_type[2] = {0x08,0x00};
   WP_U32 l2_header_len = sizeof(dst_mac) + sizeof(src_mac) + sizeof(vlan_1) + 
                          sizeof(e_type);
   WP_U32 ip_header_len = 20;
   WP_U32 payload_len;
   WP_U32 offset;
   
   bufs.buffer_class  = 0;
   bufs.displacement = 0;
   bufs.user_info = 0;
   bufs.next =  NULL;
   bufs.pool_handle = h_pool;
   bufs.data_size = APP_DATA_LENGTH;

   data_unit.type = WP_DATA_ENET;
   data_unit.n_active = 1;
   data_unit.data_size = bufs.data_size;
   data_unit.segment = &bufs;
   data_unit.n_segments = 1;

   km_data_ptr = WP_PoolAlloc(h_pool);
   bufs.data = km_data_ptr;
   if(bufs.data == NULL)
   {
      printf("Pool Alloc failure ! \n");
   }

   WP_MEM_BYTES_FILL(km_data_ptr, APP_DATA_LENGTH, pool_buffer_data_iw_cfg.size);
   offset = 0;
   memcpy(km_data_ptr + offset,dst_mac,sizeof(dst_mac));
   offset += sizeof(dst_mac);
   memcpy(km_data_ptr+offset,src_mac,sizeof(src_mac));
   offset += sizeof(src_mac);
   memcpy(km_data_ptr+offset,vlan_1,sizeof(vlan_1));
   offset += sizeof(vlan_1);
   memcpy(km_data_ptr+offset,e_type,sizeof(e_type));
   offset += sizeof(e_type);

   payload_len = APP_DATA_LENGTH - l2_header_len - ip_header_len;

   /* Outer IP header */
   insert_ip_header(0x0a0a0a0a,0xc0a8fe11,payload_len,4,offset,km_data_ptr);
   offset += ip_header_len;

   memset(km_data_ptr+offset,0xa,payload_len);

   printf("sending packet of length %d\n",APP_DATA_LENGTH);
   for(ii = 0;ii < APP_DATA_LENGTH;ii++)
   {
      printf("%.2x",*(km_data_ptr+ii));
      if((ii != 0) && !(ii % 40))
         printf("\n");
   }
   printf("\n");
   /* Use interworking qnode and interworking buffer pool for Gige */
   status = WP_HostSend(h_tx, &data_unit);
   App_TerminateOnError(status, "WP_HostSend()");
   WP_Delay(500000);
   WPI_SimulateInterrupts();
}

/* Event handling */
void add_task(app_task_list *task_list, WP_U32 event_type, WP_tag event_tag,
              void *event_param)
{
   WP_U16 tail = task_list->tail;
   WP_U32 next = tail + 1;

   if (next == task_list->num_elements)
      next = 0;

   if (next != task_list->head) {
      task_list->task[tail].event_tag = event_tag;
      task_list->task[tail].event_type = event_type;
      task_list->task[tail].event_param = event_param;
      task_list->tail = next;
   }
}

/* Event handling */
app_task *
next_task(app_task_list *task_list, app_task *result)
{
   WP_U32 head = task_list->head;
   WP_U32 tail = task_list->tail;

   if (head == tail)
      return NULL;

   *result = task_list->task[head];
   if (++head == task_list->num_elements)
      head = 0;
   task_list->head = head;
   return result;
}

/* Event handling */
void app_perform_action(app_task *task)
{
   WP_U32 tag = task->event_tag;
   switch(task->event_type)
   {
      case WP_EVENT_RX_INDICATE:
      {
         printf("Rx event on tag %d \n",tag);
         if(tag == tag_enet1_rx)
         {
            printf("receiving on enet1\n");
            App_DuReceive(h_enet1_rx, WP_DATA_ENET);
         }
         else if(tag == tag_enet2_rx)
         {
            printf("receiving on enet2\n");
            App_DuReceive(h_enet2_rx, WP_DATA_ENET);
         }
         else if(tag == tag_agg_default_bridge1)
         {
            printf("receiving on default bridge1\n");
            App_DuReceive(h_bridge1_iwhost, WP_DATA_IW);
         }
         else if(tag == tag_agg_default_bridge2)
         {
            printf("receiving on default bridge2\n");
            App_DuReceive(h_bridge2_iwhost, WP_DATA_IW);
         }
         else if(tag == tag_agg_lcp)
         {
            printf("receiving on PPP LCP aggregation\n");
            App_DuReceive(h_bridge2_iwhost, WP_DATA_IW);
         }
         else if(tag == tag_agg_cisco_control)
         {
            printf("receiving on Cisco Control Packet Aggregation\n");
            App_DuReceive(h_bridge2_iwhost, WP_DATA_IW);
         }
         else if((tag >= tag_tdmrx) && (tag <= tag_tdmrx + APP_MAX_TDM_PORTS))
         {
            printf("receiving on tdm %d\n",tag - tag_tdmrx);
            App_DuReceive(h_tdm_rx[tag - tag_tdmrx], WP_DATA_PPP_HDLC);
         }
         else
         {
            printf("RX event on unknown tag  %d\n",tag);
         }
         break;
      }
      case WP_EVENT_TX_INDICATE:
      {
         printf("TX event on tag %d\n",tag);
         break;
      }
      case WP_EVENT_STATUS_INDICATE:
         break;
      default: 
          printf("Not processing unknown event\n");
   }
}

/* Receives a packet on a RX channel */
void App_DuReceive(WP_handle h_rx, WP_U32 data_type)
{
   WP_data_unit data_unit;
   WP_data_segment data_segment, *curr_buff_ptr;
   WP_U32 kk;
   WP_status status;

   data_unit.segment = &data_segment;
   data_unit.n_segments = 1;
   data_unit.type = data_type;
   data_unit.n_active = 0;

   status = WP_HostReceive(h_rx, &data_unit);
   App_TerminateOnError(status, "WP_HostReceive");

   if(data_unit.n_active > 0)
   {
      curr_buff_ptr = data_unit.segment;
      for(kk = 0;kk < curr_buff_ptr->data_size;kk++) 
      {
         printf("%2.2x", curr_buff_ptr->data[kk]);
      }
      printf("( %d bytes )\n", curr_buff_ptr->data_size);
   }
}

void App_EventRxIndicate(WP_tag tag, WP_U32 data, WP_U32 info)
{
   add_task(irq_task_list, WP_EVENT_RX_INDICATE, tag, NULL);
}

void App_EventTxIndicate(WP_tag tag, WP_U32 data, WP_U32 info)
{
   add_task(irq_task_list, WP_EVENT_TX_INDICATE, tag, NULL);
}

void App_EventErrorIndicate(WP_tag tag, WP_U32 event, WP_U32 data)
{
   add_task(irq_task_list, WP_EVENT_STATUS_INDICATE, tag, NULL);
}

void App_Quit(void)
{
   WP_DriverRelease();
#if _WT_MODULE_TEST_ENABLE_
   WT_Reboot();
#endif
   exit(0);
}

void App_TerminateOnError(WP_handle handle, WP_CHAR *s)
{
   if (WPI_SimulateInterrupts(), WP_ERROR_P(handle)) {
      printf("Test Abort %s %s 0x%x\n", s, WP_error_name[WP_ERROR(handle)], handle);
      /* Release WDDI */
      WP_DriverRelease();
#if _WT_MODULE_TEST_ENABLE_
      WT_Reboot();
#endif
      exit(1);
   }
   else
   {
#if 1
      if(handle == WP_OK)
         printf("Status returned from %s : WP_OK\n",s);
      else
         printf("Handle returned from %s is %#8.8x\n", s,handle);
#endif
   }
}

void App_ShowStats(void)
{
   WP_status status;
   WP_stats_enet s_hs_enet = {0};
   WP_iw_global_stats s_iw_global = {0};
   WP_iw_flow_stats flow_stats = {0};
   WP_bport_stats bport_stats = {0};
   WP_route_port_stats rport_stats = {0};
   WP_stats_tdi_hdlc tdi_hdlc_stats = {0};
   WP_U32 ii;

   memset(&s_hs_enet,0,sizeof(s_hs_enet));
   printf("-------------------------------\n");
   printf(" ENET1 statisics \n");
   printf("-------------------------------\n");
   status = WP_DeviceStatistics(h_device_enet1, &s_hs_enet);
   App_TerminateOnError(status, "WP_DeviceStatistics");
   printf("   rx_bytes            : %u\n", (WP_U32)s_hs_enet.rx_bytes);
   printf("   rx_packets          : %u\n", (WP_U32)s_hs_enet.rx_packets);
   printf("   rx_err_fcs          : %u\n", (WP_U32)s_hs_enet.rx_err_fcs);
   printf("   rx_multicast        : %u\n", (WP_U32)s_hs_enet.rx_multicast);
   printf("   rx_broadcast        : %u\n", (WP_U32)s_hs_enet.rx_broadcast);
   printf("   rx_dropped          : %u\n\n", (WP_U32)s_hs_enet.rx_dropped);
   printf("   tx_bytes            : %u\n", (WP_U32)s_hs_enet.tx_bytes);
   printf("   tx_packets          : %u\n", (WP_U32)s_hs_enet.tx_packets);
   printf("   tx_err_fcs          : %u\n", (WP_U32)s_hs_enet.tx_err_fcs);
   printf("   tx_multicast        : %u\n", (WP_U32)s_hs_enet.tx_multicast);
   printf("   tx_broadcast        : %u\n", (WP_U32)s_hs_enet.tx_broadcast);
   printf("   tx_dropped          : %u\n\n", (WP_U32)s_hs_enet.tx_dropped);
   printf("   rx_err_overrun      : %u\n", (WP_U32)s_hs_enet.rx_err_overrun);
   printf("   rx_err_mru          : %u\n", (WP_U32)s_hs_enet.rx_err_mru);
   printf("   rx_iw_frames        : %u\n", (WP_U32)s_hs_enet.rx_iw_frames);
   printf("   rx_host_frames      : %u\n", (WP_U32)s_hs_enet.rx_host_frames);
   printf("   tx_frames           : %u\n", (WP_U32)s_hs_enet.tx_frames);

   memset(&s_hs_enet,0,sizeof(s_hs_enet));
   printf("-------------------------------\n");
   printf(" ENET2 statisics \n");
   printf("-------------------------------\n");
   status = WP_DeviceStatistics(h_device_enet2, &s_hs_enet);
   App_TerminateOnError(status, "WP_DeviceStatistics");
   printf("   rx_bytes            : %u\n", (WP_U32)s_hs_enet.rx_bytes);
   printf("   rx_packets          : %u\n", (WP_U32)s_hs_enet.rx_packets);
   printf("   rx_err_fcs          : %u\n", (WP_U32)s_hs_enet.rx_err_fcs);
   printf("   rx_multicast        : %u\n", (WP_U32)s_hs_enet.rx_multicast);
   printf("   rx_broadcast        : %u\n", (WP_U32)s_hs_enet.rx_broadcast);
   printf("   rx_dropped          : %u\n\n", (WP_U32)s_hs_enet.rx_dropped);
   printf("   tx_bytes            : %u\n", (WP_U32)s_hs_enet.tx_bytes);
   printf("   tx_packets          : %u\n", (WP_U32)s_hs_enet.tx_packets);
   printf("   tx_err_fcs          : %u\n", (WP_U32)s_hs_enet.tx_err_fcs);
   printf("   tx_multicast        : %u\n", (WP_U32)s_hs_enet.tx_multicast);
   printf("   tx_broadcast        : %u\n", (WP_U32)s_hs_enet.tx_broadcast);
   printf("   tx_dropped          : %u\n\n", (WP_U32)s_hs_enet.tx_dropped);
   printf("   rx_err_overrun      : %u\n", (WP_U32)s_hs_enet.rx_err_overrun);
   printf("   rx_err_mru          : %u\n", (WP_U32)s_hs_enet.rx_err_mru);
   printf("   rx_iw_frames        : %u\n", (WP_U32)s_hs_enet.rx_iw_frames);
   printf("   rx_host_frames      : %u\n", (WP_U32)s_hs_enet.rx_host_frames);
   printf("   tx_frames           : %u\n", (WP_U32)s_hs_enet.tx_frames);

   printf("-------------------------------\n");
   printf("Bridge Port Statistics ENET1 \n");
   printf("-------------------------------\n");
   memset(&bport_stats, 0, sizeof(bport_stats));
   status = WP_IwPortStatistics(h_bridge1_bport_enet1, &bport_stats);
   App_TerminateOnError(status, "WP_BportStatistics");

   printf("rx_valid_packets = %d\n",(WP_U32)bport_stats.rx_valid_packets);
   printf("discard_vlan_acceptable_filter = %d\n",(WP_U32)bport_stats.discard_vlan_acceptable_filter);
   printf("discard_ingress_filter = %d\n",(WP_U32)bport_stats.discard_ingress_filter);
   printf("discard_bridge_classifier = %d\n",(WP_U32)bport_stats.discard_bridge_classifier);
   printf("discard_unk_macsa = %d\n",(WP_U32)bport_stats.discard_unk_macsa);
   printf("deny_mac_sa = %d\n",(WP_U32)bport_stats.deny_mac_sa);
   printf("deny_mac_da = %d\n",(WP_U32)bport_stats.deny_mac_da);
   printf("rx_bc_valid_packets = %d\n",(WP_U32)bport_stats.rx_bc_valid_packets);
   printf("rx_mc_valid_packets = %d\n",(WP_U32)bport_stats.rx_mc_valid_packets);
   printf("forwarded_uc_packets = %d\n",(WP_U32)bport_stats.forwarded_uc_packets);
   printf("forwarded_bc_packets = %d\n",(WP_U32)bport_stats.forwarded_bc_packets);
   printf("forwarded_mc_packets = %d\n",(WP_U32)bport_stats.forwarded_mc_packets);
   printf("protocol_error = %d\n",(WP_U32)bport_stats.protocol_error);

   for(ii = 0;ii < APP_MAX_TDM_PORTS;ii++)
   {
       printf("-------------------------------\n");
       printf("Bridge Port Statistics TDM %d\n",ii);
       printf("-------------------------------\n");
       memset(&bport_stats, 0, sizeof(bport_stats));
       status = WP_IwPortStatistics(h_bridge1_bport_tdm[ii], &bport_stats);
       App_TerminateOnError(status, "BportStatistics");

       printf("rx_valid_packets = %d\n",(WP_U32)bport_stats.rx_valid_packets);
       printf("discard_vlan_acceptable_filter = %d\n",(WP_U32)bport_stats.discard_vlan_acceptable_filter);
       printf("discard_ingress_filter = %d\n",(WP_U32)bport_stats.discard_ingress_filter);
       printf("discard_bridge_classifier = %d\n",(WP_U32)bport_stats.discard_bridge_classifier);
       printf("discard_unk_macsa = %d\n",(WP_U32)bport_stats.discard_unk_macsa);
       printf("deny_mac_sa = %d\n",(WP_U32)bport_stats.deny_mac_sa);
       printf("deny_mac_da = %d\n",(WP_U32)bport_stats.deny_mac_da);
       printf("rx_bc_valid_packets = %d\n",(WP_U32)bport_stats.rx_bc_valid_packets);
       printf("rx_mc_valid_packets = %d\n",(WP_U32)bport_stats.rx_mc_valid_packets);
       printf("forwarded_uc_packets = %d\n",(WP_U32)bport_stats.forwarded_uc_packets);
       printf("forwarded_bc_packets = %d\n",(WP_U32)bport_stats.forwarded_bc_packets);
       printf("forwarded_mc_packets = %d\n",(WP_U32)bport_stats.forwarded_mc_packets);
       printf("protocol_error = %d\n",(WP_U32)bport_stats.protocol_error);
   }

   for(ii = 0;ii < APP_MAX_TDM_PORTS;ii++)
   {
      printf("-------------------------------\n");
      printf(" Flow statistics TDM %d\n",ii);
      printf("-------------------------------\n");
      memset(&flow_stats,0,sizeof(flow_stats));
      status = WP_IwFlowStatistics(h_flow_agg_tdm[ii],WP_IW_FLOW_STAT,&flow_stats);
      App_TerminateOnError(status, "WP_IwFlowStatistics");

      printf("forward_packet = %d\n",(WP_U32)flow_stats.forward_packet);
      printf("fbp_drop_packets = %d\n",(WP_U32)flow_stats.fbp_drop_packets);
      printf("mtu_drop_packets = %d\n",(WP_U32)flow_stats.mtu_drop_packets);
      printf("ttl_drop_packets = %d\n",(WP_U32)flow_stats.ttl_drop_packets);
      printf("tx_queue_drop_packets = %d\n",(WP_U32)flow_stats.tx_queue_drop_packets);
      printf("mpls_drop = %d\n",(WP_U32)flow_stats.mpls_drop);
      printf("denied_packets = %d\n",(WP_U32)flow_stats.denied_packets);
      printf("group_filtered_packets = %d\n",(WP_U32)flow_stats.group_filtered_packets);
      printf("forwarded_bytes = %d\n",(WP_U32)flow_stats.forwarded_bytes);
      printf("gtp_bad_headers = %d\n",(WP_U32)flow_stats.gtp_bad_headers);
      printf("policer_non_conforming_packets; = %d\n",(WP_U32)flow_stats.policer_non_conforming_packets);
   }

   for(ii = 0;ii < APP_MAX_TDM_PORTS;ii++)
   {
      printf("-------------------------------\n");
      printf(" Device statisics TDM %d \n",ii);
      printf("-------------------------------\n");
      memset(&tdi_hdlc_stats,0,sizeof(WP_stats_tdi_hdlc));
      status = WP_DeviceStatistics(h_dev_tdm[ii],&tdi_hdlc_stats);
      App_TerminateOnError(status, "WP_Devicestatistics");

      printf("rx_frames = %d\n", (WP_U32)tdi_hdlc_stats.rx_frames);
      printf("tx_frames = %d\n", (WP_U32)tdi_hdlc_stats.tx_frames);
      printf("rx_err_crc = %d\n", (WP_U32)tdi_hdlc_stats.rx_err_crc);
      printf("rx_err_abort = %d\n", (WP_U32)tdi_hdlc_stats.rx_err_abort);
      printf("rx_err_addr_mismatch = %d\n", (WP_U32)tdi_hdlc_stats.rx_err_addr_mismatch);
      printf("rx_err_buffer_overrun = %d\n", (WP_U32)tdi_hdlc_stats.rx_err_buffer_overrun);
      printf("rx_err_overrun = %d\n", (WP_U32)tdi_hdlc_stats.rx_err_overrun);
      printf("rx_err_iw_buffer_underrun = %d\n", (WP_U32)tdi_hdlc_stats.rx_err_iw_buffer_underrun);
      printf("rx_err_iw_mru = %d\n", (WP_U32)tdi_hdlc_stats.rx_err_iw_mru);
      printf("tx_err_underrun = %d\n", (WP_U32)tdi_hdlc_stats.tx_err_underrun);
      printf("rx_err_maxsdu = %d\n", (WP_U32)tdi_hdlc_stats.rx_err_maxsdu);
      printf("rx_iw_frames = %d\n", (WP_U32)tdi_hdlc_stats.rx_iw_frames);
      printf("rx_err_non_octet = %d\n", (WP_U32)tdi_hdlc_stats.rx_err_non_octet);
   }
 
   for(ii = 0;ii < APP_MAX_TDM_PORTS;ii++)
   {
      printf("-------------------------------\n");
      printf(" Flow statistics PPPSW %d\n",ii);
      printf("-------------------------------\n");
      memset(&flow_stats,0,sizeof(flow_stats));
      status = WP_IwFlowStatistics(h_flow_agg_pppsw_link[ii],WP_IW_FLOW_STAT,&flow_stats);
      App_TerminateOnError(status, "WP_IwFlowStatistics");

      printf("forward_packet = %d\n",(WP_U32)flow_stats.forward_packet);
      printf("fbp_drop_packets = %d\n",(WP_U32)flow_stats.fbp_drop_packets);
      printf("mtu_drop_packets = %d\n",(WP_U32)flow_stats.mtu_drop_packets);
      printf("ttl_drop_packets = %d\n",(WP_U32)flow_stats.ttl_drop_packets);
      printf("tx_queue_drop_packets = %d\n",(WP_U32)flow_stats.tx_queue_drop_packets);
      printf("mpls_drop = %d\n",(WP_U32)flow_stats.mpls_drop);
      printf("denied_packets = %d\n",(WP_U32)flow_stats.denied_packets);
      printf("group_filtered_packets = %d\n",(WP_U32)flow_stats.group_filtered_packets);
      printf("forwarded_bytes = %d\n",(WP_U32)flow_stats.forwarded_bytes);
      printf("gtp_bad_headers = %d\n",(WP_U32)flow_stats.gtp_bad_headers);
      printf("policer_non_conforming_packets; = %d\n",(WP_U32)flow_stats.policer_non_conforming_packets);

      printf("-------------------------------\n");
      printf("Bridge2 Port Statistics TDM %d\n",ii);
      printf("-------------------------------\n");
      memset(&bport_stats, 0, sizeof(bport_stats));
      status = WP_IwPortStatistics(h_bridge2_bport_tdm[ii], &bport_stats);
      App_TerminateOnError(status, "BportStatistics");

      printf("rx_valid_packets = %d\n",(WP_U32)bport_stats.rx_valid_packets);
      printf("discard_vlan_acceptable_filter = %d\n",(WP_U32)bport_stats.discard_vlan_acceptable_filter);
      printf("discard_ingress_filter = %d\n",(WP_U32)bport_stats.discard_ingress_filter);
      printf("discard_bridge_classifier = %d\n",(WP_U32)bport_stats.discard_bridge_classifier);
      printf("discard_unk_macsa = %d\n",(WP_U32)bport_stats.discard_unk_macsa);
      printf("deny_mac_sa = %d\n",(WP_U32)bport_stats.deny_mac_sa);
      printf("deny_mac_da = %d\n",(WP_U32)bport_stats.deny_mac_da);
      printf("rx_bc_valid_packets = %d\n",(WP_U32)bport_stats.rx_bc_valid_packets);
      printf("rx_mc_valid_packets = %d\n",(WP_U32)bport_stats.rx_mc_valid_packets);
      printf("forwarded_uc_packets = %d\n",(WP_U32)bport_stats.forwarded_uc_packets);
      printf("forwarded_bc_packets = %d\n",(WP_U32)bport_stats.forwarded_bc_packets);
      printf("forwarded_mc_packets = %d\n",(WP_U32)bport_stats.forwarded_mc_packets);
      printf("protocol_error = %d\n",(WP_U32)bport_stats.protocol_error);
   }

   printf("-------------------------------\n");
   printf(" Flow statistics LCP\n");
   printf("-------------------------------\n");
   memset(&flow_stats,0,sizeof(flow_stats));
   status = WP_IwFlowStatistics(h_flow_agg_lcp,WP_IW_FLOW_STAT,&flow_stats);
   App_TerminateOnError(status, "WP_IwFlowStatistics");

   printf("forward_packet = %d\n",(WP_U32)flow_stats.forward_packet);
   printf("fbp_drop_packets = %d\n",(WP_U32)flow_stats.fbp_drop_packets);
   printf("mtu_drop_packets = %d\n",(WP_U32)flow_stats.mtu_drop_packets);
   printf("ttl_drop_packets = %d\n",(WP_U32)flow_stats.ttl_drop_packets);
   printf("tx_queue_drop_packets = %d\n",(WP_U32)flow_stats.tx_queue_drop_packets);
   printf("mpls_drop = %d\n",(WP_U32)flow_stats.mpls_drop);
   printf("denied_packets = %d\n",(WP_U32)flow_stats.denied_packets);
   printf("group_filtered_packets = %d\n",(WP_U32)flow_stats.group_filtered_packets);
   printf("forwarded_bytes = %d\n",(WP_U32)flow_stats.forwarded_bytes);
   printf("gtp_bad_headers = %d\n",(WP_U32)flow_stats.gtp_bad_headers);
   printf("policer_non_conforming_packets; = %d\n",(WP_U32)flow_stats.policer_non_conforming_packets);

   printf("-------------------------------\n");
   printf(" Flow statistics Cisco Control Packets\n");
   printf("-------------------------------\n");
   memset(&flow_stats,0,sizeof(flow_stats));
   status = WP_IwFlowStatistics(h_flow_agg_cisco_control,WP_IW_FLOW_STAT,&flow_stats);
   App_TerminateOnError(status, "WP_IwFlowStatistics");

   printf("forward_packet = %d\n",(WP_U32)flow_stats.forward_packet);
   printf("fbp_drop_packets = %d\n",(WP_U32)flow_stats.fbp_drop_packets);
   printf("mtu_drop_packets = %d\n",(WP_U32)flow_stats.mtu_drop_packets);
   printf("ttl_drop_packets = %d\n",(WP_U32)flow_stats.ttl_drop_packets);
   printf("tx_queue_drop_packets = %d\n",(WP_U32)flow_stats.tx_queue_drop_packets);
   printf("mpls_drop = %d\n",(WP_U32)flow_stats.mpls_drop);
   printf("denied_packets = %d\n",(WP_U32)flow_stats.denied_packets);
   printf("group_filtered_packets = %d\n",(WP_U32)flow_stats.group_filtered_packets);
   printf("forwarded_bytes = %d\n",(WP_U32)flow_stats.forwarded_bytes);
   printf("gtp_bad_headers = %d\n",(WP_U32)flow_stats.gtp_bad_headers);
   printf("policer_non_conforming_packets; = %d\n",(WP_U32)flow_stats.policer_non_conforming_packets);

   printf("-------------------------------\n");
   printf(" Flow statistics ENET2\n");
   printf("-------------------------------\n");
   memset(&flow_stats,0,sizeof(flow_stats));
   status = WP_IwFlowStatistics(h_flow_agg_enet2,WP_IW_FLOW_STAT,&flow_stats);
   App_TerminateOnError(status, "WP_IwFlowStatistics");

   printf("forward_packet = %d\n",(WP_U32)flow_stats.forward_packet);
   printf("fbp_drop_packets = %d\n",(WP_U32)flow_stats.fbp_drop_packets);
   printf("mtu_drop_packets = %d\n",(WP_U32)flow_stats.mtu_drop_packets);
   printf("ttl_drop_packets = %d\n",(WP_U32)flow_stats.ttl_drop_packets);
   printf("tx_queue_drop_packets = %d\n",(WP_U32)flow_stats.tx_queue_drop_packets);
   printf("mpls_drop = %d\n",(WP_U32)flow_stats.mpls_drop);
   printf("denied_packets = %d\n",(WP_U32)flow_stats.denied_packets);
   printf("group_filtered_packets = %d\n",(WP_U32)flow_stats.group_filtered_packets);
   printf("forwarded_bytes = %d\n",(WP_U32)flow_stats.forwarded_bytes);
   printf("gtp_bad_headers = %d\n",(WP_U32)flow_stats.gtp_bad_headers);
   printf("policer_non_conforming_packets; = %d\n",(WP_U32)flow_stats.policer_non_conforming_packets);

   printf("-------------------------------\n");
   printf(" Flow statisics Default Bridge  \n");
   printf("-------------------------------\n");
   memset(&flow_stats,0,sizeof(flow_stats));
   status = WP_IwFlowStatistics(h_bridge1_default_agg,WP_IW_FLOW_STAT,&flow_stats);
   App_TerminateOnError(status, "WP_IwFlowStatistics");

   printf("forward_packet = %d\n",(WP_U32)flow_stats.forward_packet);
   printf("fbp_drop_packets = %d\n",(WP_U32)flow_stats.fbp_drop_packets);
   printf("mtu_drop_packets = %d\n",(WP_U32)flow_stats.mtu_drop_packets);
   printf("ttl_drop_packets = %d\n",(WP_U32)flow_stats.ttl_drop_packets);
   printf("tx_queue_drop_packets = %d\n",(WP_U32)flow_stats.tx_queue_drop_packets);
   printf("mpls_drop = %d\n",(WP_U32)flow_stats.mpls_drop);
   printf("denied_packets = %d\n",(WP_U32)flow_stats.denied_packets);
   printf("group_filtered_packets = %d\n",(WP_U32)flow_stats.group_filtered_packets);
   printf("forwarded_bytes = %d\n",(WP_U32)flow_stats.forwarded_bytes);
   printf("gtp_bad_headers = %d\n",(WP_U32)flow_stats.gtp_bad_headers);
   printf("policer_non_conforming_packets; = %d\n",(WP_U32)flow_stats.policer_non_conforming_packets);


   printf("-------------------------------\n");
   printf(" Flow statisics Default Router  \n");
   printf("-------------------------------\n");
   memset(&flow_stats,0,sizeof(flow_stats));
   status = WP_IwFlowStatistics(h_bridge2_default_agg,WP_IW_FLOW_STAT,&flow_stats);
   App_TerminateOnError(status, "WP_IwFlowStatistics");

   printf("forward_packet = %d\n",(WP_U32)flow_stats.forward_packet);
   printf("fbp_drop_packets = %d\n",(WP_U32)flow_stats.fbp_drop_packets);
   printf("mtu_drop_packets = %d\n",(WP_U32)flow_stats.mtu_drop_packets);
   printf("ttl_drop_packets = %d\n",(WP_U32)flow_stats.ttl_drop_packets);
   printf("tx_queue_drop_packets = %d\n",(WP_U32)flow_stats.tx_queue_drop_packets);
   printf("mpls_drop = %d\n",(WP_U32)flow_stats.mpls_drop);
   printf("denied_packets = %d\n",(WP_U32)flow_stats.denied_packets);
   printf("group_filtered_packets = %d\n",(WP_U32)flow_stats.group_filtered_packets);
   printf("forwarded_bytes = %d\n",(WP_U32)flow_stats.forwarded_bytes);
   printf("gtp_bad_headers = %d\n",(WP_U32)flow_stats.gtp_bad_headers);
   printf("policer_non_conforming_packets; = %d\n",(WP_U32)flow_stats.policer_non_conforming_packets);
}

void App_Debug(WP_boolean debug)
{
   WP_flow_class_rule flow_class_rule;
   WP_flow_class_forwarding_action forwarding_action;

   if(debug)
   {
      printf("Debug is on. Turning it OFF\n");
      forwarding_action.flow_aggregation = h_flow_agg_tdm[1];
      debug_on = WP_FALSE;
   }
   else
   {
      printf("Debug is off. Turning it ON\n");
      forwarding_action.flow_aggregation = h_bridge1_default_agg;
      debug_on = WP_TRUE;
   }

   flow_class_rule.match_action = WP_FLOW_CLASS_MODE_FLOW_AGG;
   flow_class_rule.action_info.forwarding_action = forwarding_action;
   flow_class_rule.wred_entry = 0;

   status = WP_IwFlowModify(h_iw_sys_bridge1, h_bridge1_dfcflow1, WP_IW_MODIFY_WRITE_THROUGH, 
                                WP_IW_FLOW_MOD_FLOW_AGG, &flow_class_rule);
   App_TerminateOnError(status," WP_IwFlowModify() DFC");

   status = WP_IwSystemBuild(h_iw_sys_bridge1);
   App_TerminateOnError(status," WP_IwSystemBuild()");
}

/****************************************/
/* Common routine to select IP Address  */
/****************************************/
void insert_ip_header(WP_U32 ip_sa, WP_U32 ip_da, WP_U32 payload_size, 
                      WP_U32 protocol, WP_U32 offset, WP_U8 *header)
{
  WP_U32 packet_length;
  WP_U32 temp_sum, i;
  WP_U32 checksum = 0;
  WP_U8  *ptr_ip_data;
  WP_U8  *start_ip_data;

  ptr_ip_data= header + offset;
  start_ip_data = ptr_ip_data;

  /* Reset IP header just in case it has garbage */
  memset(ptr_ip_data,0,IP_HEADER_SIZE);

  /* IP Version 4, header length = 20 bytes */
  ptr_ip_data[IP_VER_HLEN] = 0x45;

  /* Calculate new Packet length */
  packet_length = payload_size + IP_HEADER_SIZE;
  ptr_ip_data[IP_LENGTH] = (WP_U8)(packet_length >> 8);
  ptr_ip_data[IP_LENGTH+1] = (WP_U8)(packet_length & 0xff);

  /* Time to Live */
  ptr_ip_data[IP_TTL] = 64;

  /* Protocol */
  ptr_ip_data[IP_PROTOCOL] = protocol;

  /* Get new ip source address */
  ptr_ip_data[IP_SA] = (WP_U8)(ip_sa >> 24);
  ptr_ip_data[IP_SA+1] = (WP_U8)(ip_sa >> 16);
  ptr_ip_data[IP_SA+2] = (WP_U8)(ip_sa >> 8);
  ptr_ip_data[IP_SA+3] = (WP_U8)(ip_sa & 0xff);

  /* Get new ip destination address */
  ptr_ip_data[IP_DA] = (WP_U8)(ip_da >> 24);
  ptr_ip_data[IP_DA+1] =  (WP_U8)(ip_da >> 16);
  ptr_ip_data[IP_DA+2] = (WP_U8)(ip_da >> 8);
  ptr_ip_data[IP_DA+3] = (WP_U8)(ip_da & 0xff);

  /* Calculate header checksum */
  for(i=0;i < (IP_HEADER_SIZE/2); i++)
  {
      temp_sum = (ptr_ip_data[0] << 8 | ptr_ip_data[1]);
      checksum += temp_sum;
      ptr_ip_data += 2;
  }

  while (checksum & 0xffff0000)
     checksum = (checksum & 0xffff) + ((checksum & 0xffff0000) >> 16);

  checksum = ~checksum;

  /* Save calculated checksum value */
  start_ip_data[IP_CHECKSUM] = checksum >> 8;
  start_ip_data[IP_CHECKSUM+1] = checksum & 0xff;
}

#if _WT_MODULE_TEST_ENABLE_
#include "wt_util.c"    /* WT_Identify */
#endif

#if _APP_MODULE_TEST_COMMENT_ENABLE_
#pragma check_tests common
#endif
