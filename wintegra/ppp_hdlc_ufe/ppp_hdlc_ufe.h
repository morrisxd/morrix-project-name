/*****************************************************************************
 * (C) Copyright PMC - WIN division (Wintegra) 2011.  All rights reserved.
 * PMC-Sierra (WINTEGRA) CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of PMC-Sierra (Wintegra).
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *  Application : HDLC/PPP over IP/MPLS with QoS
 *  File Name   : ppp_hdlc_ufe.h
 *  Version     : 1
 *  Date        : 28 November 2011
 *  Accompanies : TBD
 *****************************************************************************/

/***************************************************************************
 *  Description:
 *
 *  This file contain all the necessary macros, global structure and variable declaration and definition
 *  And all function declaration.
 *
 ******************************************************************************/


/*NOTE: uncomment the macro, compiling for wp3 board... by default compilation will be for WP3 SLB board*/
#define BOARD_WP3


#define APP_VLAN_ID         0xa
#define APP_MAX_MAC_ADDRESS 10

/* Enet Bridge ports */
#define APP_MAX_BPORTS      20
#define APP_MAX_HDLC_PORTS   2

#define WTI_INITIALIZE_FIRMWARE                     1 /* setting to '1' allows programming configuration through the host */

#define APP_NUM_IW_BUFFERS   500
#define APP_IW_BUFFER_SIZE   1984
#define APP_IW_BUFFER_OFFSET 64
#define APP_MASTER_WPID      WP_WINPATH(0)

#define QOS_ENABLE           1
#define PQBLOCK_SIZE         4
#define APP_MAX_CHANNEL      PQBLOCK_SIZE
#define APP_MAX_DEV_HDLC     PQBLOCK_SIZE
#define APP_MAX_PORT_HDLC     PQBLOCK_SIZE
#define APP_MAX_FLOW_AGG_HDLC PQBLOCK_SIZE


#define WTI_INITIALIZE_FIRMWARE                     1 /* setting to '1' allows programming configuration through the host */
#define WTI_TX_MAX_SDU                              8192
#define WTI_MAX_PW                                  336//336          /* maximum number of PW's in PWE3 system */  // For E1: should be 260
#define WTI_EMPHY_PORT                              WP_PORT_UPI1       /* EMPHY port */
#define WTI_UFE_UPI_PORT                            WTI_EMPHY_PORT
#define WT_BRG_UPI1                                 WP_BRG3
#define WT_BRG_UPI2                                 WP_BRG1
#define N_MAX_OUT_OF_BAND_CAS_FLOWS                 2
#define WTI_MAX_ABS_PW                              WTI_MAX_PW
#define WTI_MAX_NUM_EMPHY_PHYS                      WTI_MAX_PW    /* maximum number of emphy phys, relevant only if working with UFE */
#define WTI_SOCKET_LOOPBACK_MODE                    WUFE_LINE_LOOPBACK_TYPE_NONE
#define WT_FIRST_LINE_INDEX                         0
#define WT_N_DEVICES                                336

// UFE Related Defines
#define N_MAX_UFE_SPE_INDEX                         WUFE_MAX_N_CORE*WUFE_MAX_N_STM1*WUFE_MAX_N_SPE
#define MAX_UFE_LINES_USED                          1344
#define N_MAX_EMPHY_PHYS                            4032
#define WT_UFE_HANDLE_INVALID                       0xffffffff
#define WTI_MAX_LINES                               336  // For E1: should be 252
#define MAX_UFE_LINES                               WTI_MAX_LINES
#define OCTETS_PER_LINE                             28
#define WT_MAX_LINE_INDEX                           336  // For E1: should be 252
#define MAX_UFE_LINES_USED                          1344
#define MAX_CHANNELS                                WTI_MAX_LINES * 2
#define WTI_MAX_DEVICE                              WTI_MAX_LINES
#define WTI_MAX_TX_RX_CHANNEL                       WTI_MAX_LINES
#define NUM_TU                                      4



// FLEXMUX Related Defines
#define WTI_MAX_FLEXMUX_DEVICES_SUPPORTED_BY_DRIVER 4
#define WTI_FLEXMUX_NUM_DEVICES                     1
#define WTI_MAX_LINE_PORTS_PER_FLEXMUX_DEVICE       16

/*representing last two bytes of HDLC header and first two bytes of IP header*/
#define APP_TRAFFIC_IP_HEADER     0x08004500 
#define APP_TRAFFIC_IP_HEADER_PPP 0x214500 

#define WTI_EMPHY_PORT                              WP_PORT_UPI1       /* EMPHY port */

#define APP_DEBUG 0
#define APP_PPPSW 1

#define HDLC_OVER_IP        '1'
#define HDLC_OVER_MPLS      '2'
#define PPP_OVER_IP         '3'
#define PPP_OVER_MPLS       '4'

#define TRAFFIC             '1'
#define CONTROL             '2'
#define LCP                 '3'

#define APP_MAX_PKTS_NUM    9

/* Utility macros */
#define MAC_COPY(d,s)           { int _i;             \
                                  for(_i=0;_i<6;_i++) \
                                  *((d)+_i)=*((s)+_i); }

extern WP_U32 dps_ProgramImage[];
extern WP_U16 dps_PC_Table_Init[];

#ifdef BOARD_WP3
WP_context context_cfg =
{
   3, /* qnodes */
   /* the first four assignments are mandatory bus assignments */
   {
      { /* WP_BUS_PARAM	*/ WP_BUS_PARAM, APP_BANK_PARAM},
      { /* WP_BUS_PACKET	 */ WP_BUS_PACKET, APP_BANK_PACKET},
      { /* WP_BUS_INTERNAL */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
      { /* WP_BUS_HOST	 */ WP_BUS_HOST, APP_BANK_HOST},
      { /* WP_BUS_IRQ 	 */ WP_BUS_HOST, APP_BANK_HOST},
      { /* WP_BUS_TMSTMP	 */ WP_BUS_HOST, APP_BANK_HOST},
      { /* UNASSIGNED 	 */ WP_BUS_NONE, 0},
      { /* UNASSIGNED 	 */ WP_BUS_NONE, 0},
      { /* ASU_TPT		 */ WP_BUS_PARAM, APP_BANK_PARAM},
      { /* ASU CW 		 */ WP_BUS_PARAM, APP_BANK_PARAM},
      { /* ASU TX QUEUE	 */ WP_BUS_PARAM, APP_BANK_PARAM},
      { /* STATISTICS 	 */ WP_BUS_PARAM, APP_BANK_PARAM},
      { /* ADDRESS LOOKUP  */ WP_BUS_HOST, APP_BANK_HOST},
      { /* ATM CPT		 */ WP_BUS_PARAM, APP_BANK_PARAM},
      { /* ENET HDLC CPT	 */ WP_BUS_PARAM, APP_BANK_PARAM},
      { /* IW FBP PARAM	 */ WP_BUS_PARAM, APP_BANK_PARAM},
      { /* IW FBP DATA	 */ WP_BUS_PACKET, APP_BANK_PACKET},
      { /* EXTERNAL FIFO	 */ WP_BUS_PACKET, APP_BANK_PACKET},
      { /* MAC LOOKUP 	 */ WP_BUS_PARAM, APP_BANK_PARAM},
      { /* IMA DISPATCH	 */ WP_BUS_PARAM, APP_BANK_PARAM},
      { /* IMA SHARED 	 */ WP_BUS_PACKET, APP_BANK_PACKET},
      { /* LPM			 */ WP_BUS_HOST, APP_BANK_HOST},
      { /* MFC RULES		 */ WP_BUS_HOST, APP_BANK_HOST},
      { /* FIWT, QAT		 */ WP_BUS_HOST, APP_BANK_HOST},
      { /* MFCPT, UPPPT, DS */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
      { /* STATISTICS2	  */ WP_BUS_PARAM, APP_BANK_PARAM},
      { /* STATISTICS3/IMAPORT */ WP_BUS_PARAM, APP_BANK_PARAM},        /* Port	  */
      { /* WP_VB_IMA_RX_BUS	 */ WP_BUS_PACKET, APP_BANK_PACKET},        /* Rx buffer */
      { /* PKT_GLOBAL_TCPT	 */ WP_BUS_PARAM, APP_BANK_PARAM},
      { /* AAL2_QSR/IMATXBUF	 */ WP_BUS_PARAM, APP_BANK_PARAM},  /* Tx buffer */
      { /* MANDATORY		 */ WP_BUS_NONE, 0},
      { /* MANDATORY		 */ WP_BUS_NONE, 0},
      /* ************* VIRTUAL BUS ASSIGNMENT ***************/
      {/* MCH_DATA		*/ WP_BUS_PACKET, APP_BANK_PACKET},
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
#else /* for WP3 SLB board*/ 

WP_context context_cfg =
{
   3,
   /* the first four assignments are mandatory bus assignments */
   {
      {/* BUS_PARAM				*/ WP_BUS_PARAM,	APP_BANK_PARAM},
      {/* BUS_PACKET				*/ WP_BUS_PACKET,	APP_BANK_PACKET},
      {/* BUS_INTERNAL 			*/ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
      {/* BUS_HOST 				*/ WP_BUS_HOST, 	APP_BANK_HOST},
      {/* INTQUEUE_DEBUG			*/ WP_BUS_HOST, 	APP_BANK_HOST},
      {/* TIMESTAMP				*/ WP_BUS_HOST, 	APP_BANK_HOST},
      {/* CAS						*/ WP_BUS_NONE, 	0},
      {/* TRANS_TSAT				*/ WP_BUS_NONE, 	0},
      {/* TPT						*/ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
      {/* CW						*/ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
      {/* ASU_TXQUEUE				*/ WP_BUS_PARAM,	APP_BANK_PARAM},
      {/* STATISTICS				*/ WP_BUS_HOST, 	APP_BANK_HOST},
      {/* ATM_ADDLOOKUP			*/ WP_BUS_PARAM,	APP_BANK_PARAM},
      {/* ATM_CPT_EBMT_EXTHEADER	*/ WP_BUS_PARAM,	APP_BANK_PARAM},
      {/* ETH HDLC CPT 			*/ WP_BUS_PARAM,	APP_BANK_PARAM},
      {/* FBP BD					*/ WP_BUS_PARAM,	APP_BANK_PARAM},
      {/* FBP DATA 				*/ WP_BUS_PARAM,   APP_BANK_PARAM},
      {/* L2_BKGND 				*/ WP_BUS_PARAM,   APP_BANK_PARAM},
      {/* BRIDGE_LEARNING_DFC_PFMT */ WP_BUS_PARAM,	APP_BANK_PARAM},
      {/* IMA_MLPPP_STATE			*/ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
      {/* IMA_MLPPP_BUFFER 		*/ WP_BUS_PARAM,	APP_BANK_PARAM},
      {/* HASH_IW_LPM_EMC			*/ WP_BUS_PARAM,   APP_BANK_PARAM},
      {/* IW_MFC_RULES 			*/ WP_BUS_PARAM,	APP_BANK_PARAM},
      {/* FIWT_QAT 				*/ WP_BUS_PARAM,	APP_BANK_PARAM},
      {/* MFCPT_DIFFSERV_UPPT_DFCT */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
      {/* STATISTICS2				*/ WP_BUS_PARAM,   APP_BANK_PARAM},
      {/* STATISTICS3				*/ WP_BUS_PARAM,   APP_BANK_PARAM},
      {/* UNASSIGNED6				*/ WP_BUS_NONE, 	0},
      {/* PKT_GLOBAL_TCPT			*/ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
      {/* AAL2_QSR 				*/ WP_BUS_NONE, 	0},
      {/* UNASSIGNED9				*/ WP_BUS_NONE, 	0},
      {/* UNASSIGNED10 			*/ WP_BUS_NONE, 	0},
      /* ********************* VIRTUAL BUS ASSIGNMENT *********************/
      {/* MCH_DATA 				*/ WP_BUS_PARAM,   APP_BANK_PARAM},
      {/* OAM_FM_QUERY 			*/ WP_BUS_NONE, 	0},
      {/* HASH_ON_HEAP 			*/ WP_BUS_NONE, 	0},
      {/* MFC_INTERNAL 			*/ WP_BUS_PARAM,   APP_BANK_PARAM},
      {/* MFR_RAS_TBL				*/ WP_BUS_NONE, 	0},
      {/* CESOP_DATA				*/ WP_BUS_NONE, 	0},
      {/* WP_VB_SECURITY_ESE,*/	   WP_BUS_NONE, 	0},
      {/* WP_VB_PDCP_SECURITY_ENTITY,*/WP_BUS_NONE,	 0},
      {/* WP_VB_POLICER,*/ 		   WP_BUS_NONE, 	0},
      {/* WP_VB_PCE_LRN,*/ 		   WP_BUS_NONE, 	0},
      {/* WP_VB_PCEA_PARAM,*/		   WP_BUS_NONE, 	0},
      {/* WP_VB_CONGESTION_TABLE,*/   WP_BUS_NONE, 	0},

   },
   {
      dps_ProgramImage, dps_ProgramImage
   },
   {
      dps_PC_Table_Init, dps_PC_Table_Init
   },
   NULL,    /* Interrupt queue tables */
   0,       /* Maxumum number of pools */
   NULL,    /* ATM global parameters */
   NULL,    /* Interworking global parameters */
};

#endif

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

#ifdef BOARD_WP3
WP_pool_buffer_data pool_buffer_data_256_cfg = 
{
   /* n_buffers */ 1000,
   /* offset */ 0,
   /* size */ 256,
   /* pad */ 0,
   /* bus */ WP_BUS_PACKET,
   /* bank */ APP_BANK_PACKET,
};
#else/* for WP3 SLB board*/ 
WP_pool_buffer_data pool_buffer_data_256_cfg = 
{
   /* n_buffers */ 1000,
   /* offset */ 0,
   /* size */ 256,
   /* pad */ 0,
   /* bus */ WP_BUS_HOST,
   /* bank */ APP_BANK_HOST,
};
#endif

#ifdef BOARD_WP3
WP_pool_buffer_data pool_buffer_data_iw_cfg = 
{
   /* n_buffers              */ APP_NUM_IW_BUFFERS,
   /* offset                 */ 64,
   /* size                   */ 1984,
   /* pad                    */ 0,
   /* bus                    */ WP_BUS_PACKET,
   /* bank                   */ APP_BANK_PACKET
};
#else/* for WP3 SLB board*/ 
WP_pool_buffer_data pool_buffer_data_iw_cfg = 
{
   /* n_buffers              */ APP_NUM_IW_BUFFERS,
   /* offset                 */ 64,
   /* size                   */ 1984,
   /* pad                    */ 0,
   /* bus                    */ WP_BUS_HOST,
   /* bank                   */ APP_BANK_HOST
};
#endif

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

WP_port_enet port_enet_cfg =
{
   /* pkt_limits             */ 
   {
      /* max_tx_channels*/ 8,
      /* max_rx_channels*/ 8,
      /* pq_block_size*/   0,
      /* n_pq_blocks*/     0,
      /* emphy_devices*/   0,
   },
   /* flowmode               */ WP_FLOWMODE_FAST,
   /* interface_mode */ WP_ENET_SGMII_1000,
   /* rx_iw_bkgnd            */ WP_IW_BKGND_NOT_USED,
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
   /* loopbackmode  */   WP_ENET_NORMAL,//WP_ENET_LOOPBACK, it should be normal if lop back is set from h/w init
   /* extended_params */ NULL,
};


WP_port_upi_iw_ext emphy_iw_config[1] =
{
    {
        /* iw_characteristics */WP_IW_BKGND_USED,
        /* ces_tdm_wpid */0
    }
};

WP_port_upi_emphy_ext emphy_ext =
{
    /* tx_maxsdu */ 1536,
    /* protocols */ WP_EMPHY_HDLC_PPP,
    /* trans_emphy_devices */ 0, //0,
    /* max_ds1 */ MAX_UFE_LINES_USED, //0,
};

WP_port_pkt_limits pkt_limits = {APP_MAX_CHANNEL, APP_MAX_CHANNEL*APP_MAX_HDLC_PORTS, PQBLOCK_SIZE, APP_MAX_PORT_HDLC, APP_MAX_PORT_HDLC};

WP_port_upi_packet_ext emphy_packet_ext = {
        /* packet characteristics */ 0,
        /* slow_phy_tx_bitrate */ 0,
        /* pkt_limits */ &pkt_limits
};

WP_port_upi upi_emphy_port_config =
{

    /* upimode  */ WP_UPI_UFE4,
    /* loopback  */ WP_UPI_NORMAL,
    /* tx_characteristics  */(WP_UPI_SINGLE  |
            WP_UPI_MASTER  |
            WP_UPI_PRT_EVEN |
            WP_UPI_FIXED   |
            WP_UPI_PRTCHK_ON |
            WP_UPI_EMPHY2_13 |
            WP_UPI_BUS_WIDTH_16),
    /* rx_characteristics  */ (WP_UPI_SINGLE  |
            WP_UPI_MASTER  |
            WP_UPI_PRT_EVEN |
            WP_UPI_FIXED   |
            WP_UPI_PRTCHK_OFF |
            WP_UPI_BUS_WIDTH_16),
    /*	 *atm_ext  */ NULL,
    /* *packet_ext  */&emphy_packet_ext,
    /*	 *iw_ext  */ NULL,//emphy_iw_config,
    /*  *emphy_ext;*/ &emphy_ext//emphy_config
};

/* Configuration for the POS HDLC Device */
WP_device_upi_pos device_emphy_hdlc_cfg[] =
{
   {
      /* tx_bit_rate */            155000000,
      /* tx_slave_addr */          0,
      /* tx_statmode */            WP_PKT_STAT_ENABLE,
      /* tx_maxsdu */              1536,
      /* rx_bit_rate */            155000000,
      /* rx_slave_addr */          0,
      /* rx_statmode */            WP_PKT_STAT_ENABLE,
      /* flowmode */               WP_FLOWMODE_MULTI,
      /* addr_mask */              0,
      /* address1 */               0,
      /* address2 */               0,
      /* tx_tq_types */            {WP_PKT_SCHED_WFQ,
                                    WP_PKT_SCHED_WFQ,
                                    WP_PKT_SCHED_WFQ,
                                    WP_PKT_SCHED_WFQ},
      /* crc */                    WP_HDLC_CRC16
   }
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
   /* iw_host_limits         */ {10, 10, 0,0 },//{10, 10, 0,1,0 }//
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
   /* WP_U16 max_iw_programmable_filters */ 25,
   /* WP_U16 max_iw_policers */ 25,
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
   /* classification hash size */ WP_IW_32K_HASH_ENTRIES,
   /* port filtering */ WP_IW_PORT_FILTERING_INPUT,//WP_IW_PORT_FILTERING_ENABLE,
   /* deny_eq_ip */ WP_IW_DENY_EQUAL_IP_DISABLE,
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
  /*  max_flows */  APP_MAX_HDLC_PORTS,
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
      /* tx_handle*/        0,
      /* leaky_bucket_mode*/ WP_IW_SINGLE_LEAKY_BUCKET,
      /* frame_len_correction*/ WP_IW_PLC_FRM_LEN_CORRECT_DISABLE,
      /* *tcm_config*/  NULL,
   }
} ;

/*
 * Global variables used during test execution
 */
WP_U8 wt_mac_enet1[6]    = {0x45, 0x6e, 0x65, 0x74, 0x23, 0x31};
WP_U8 wt_mac_enet2[6]    = {0x45, 0x6e, 0x65, 0x74, 0x23, 0x32};
WP_tag tag_channel_base = 1;

WP_flow_learning_rule_vlan bridge_cfg;
WP_l2_forward forward_rule;
WP_CHAR dp_interface_val = 'X';
WP_CHAR config_choice = 'X';
WP_U32  freerun = 0;

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
void App_ShowDeviceStats(void);
void App_ShowBportStats(void);
void App_ShowFlowAggStats(void);
void App_Debug(WP_boolean debug);
void App_TerminateOnError(WP_handle handle, WP_CHAR *s);
void App_CreateEnetHdlcBridgeForIP(void);
void App_CreateHdlcEnetBridgeForIP(void);
void App_CreateEnetHdlcBridgeForMPLS(void);
void App_CreateHdlcEnetBridgeForMPLS(void);
void App_CreateEnetPPPBridgeForIP(void);
void App_CreatePPPEnetBridgeForIP(void);
void App_CreateEnetPPPBridgeForMPLS(void);
void App_CreatePPPEnetBridgeForMPLS(void);
void App_InitConfig(void);
void App_DeviceAndPortCreate(void);
void App_MemPoolNodeAndSchedCreate(void);
void App_ChannelAndPQBlockCreate(void);
void App_EnableSystem(void);
void App_DuReceive(WP_handle h_rx, WP_U32 data_type);
void App_DataSend(WP_handle h_dev, WP_handle h_pool);
void App_MPLS_DataSend(WP_handle h_dev, WP_handle h_pool);
void App_UfeSystem(void);
void App_EnableUFESystem(void);
void App_EnableEmphyPort(void);
void WPI_SimulateInterrupts(void);
static void App_PerformFreerun(void);
void App_CheckDataStats(WP_status *test_result);
void App_ResetStats(void);

app_task *next_task(app_task_list *task_list, app_task *result);
void add_task(app_task_list *task_list, WP_U32 event_type, WP_tag tag,
              void *event_param);
void app_perform_action(app_task *task);

#define ENET_HEADER_SIZE    14
#define IP_HEADER_SIZE      20
/* Byte offsets from start of IP header */
#define IP_VER_HLEN     0
#define IP_SER_TYPE     1
#define IP_LENGTH       2
#define IP_TTL          8
#define IP_PROTOCOL     9
#define IP_CHECKSUM     10
#define IP_SA           12
#define IP_DA           16

#define UDP_HEADER_SIZE 8
#define UDP_SRC_PORT    0
#define UDP_DST_PORT    2
#define UDP_LENGTH      4
#define UDP_CHECKSUM    6

/* Functions provided by wt_util.c.  */

void insert_ip_header(WP_U32 ip_sa, WP_U32 ip_da, WP_U32 payload_size, 
                      WP_U32 protocol, WP_U32 length, WP_U8 *prefix);

/* ETHERNET */
WP_handle               h_port_enet1, h_port_enet2;
WP_handle               h_device_enet1, h_device_enet2;
//WP_handle               h_port_hdlc[APP_MAX_PORT_HDLC];
WP_handle               h_dev_hdlc[APP_MAX_DEV_HDLC];

/* INTERWORKING COMMON */
WP_handle               h_pool_buffer_host;
WP_handle               h_pool_ring_host;
WP_handle               h_pool_buffer_iw,h_pool_256;
WP_handle               h_qnode_iwq, h_qnode_host,h_qnode_iwq_hdlc;
WP_handle               h_bridge1_iwhost, h_bridge2_iwhost;
WP_handle               h_bridge1_default_agg,h_bridge2_default_agg;
WP_handle               h_flow_agg_hdlc[PQBLOCK_SIZE];
WP_handle               h_iw_sys_bridge1, h_iw_sys_bridge2;
WP_handle               h_iw_sys_pppsw;
WP_handle               h_learningflow_enet1;
WP_handle               h_bridge1_dfcflow1[PQBLOCK_SIZE][APP_MAX_HDLC_PORTS];
WP_handle               h_bridge2_dfcflow[PQBLOCK_SIZE][APP_MAX_HDLC_PORTS],h_bridge2_dfcflow_control,h_bridge2_dfcflow_lcp;
WP_handle               h_bridge1_filter1,h_bridge2_filter[3];
WP_handle               h_port_iwhost,h_dev_iwhost;
WP_handle               h_bridge1_bport_enet1, h_bridge1_bport_hdlc[APP_MAX_PORT_HDLC];
WP_handle               h_bridge2_bport_enet2, h_bridge2_bport_hdlc[APP_MAX_PORT_HDLC];
WP_handle               h_flow_agg_pppsw_link[PQBLOCK_SIZE];
WP_handle               h_flow_agg_lcp, h_flow_agg_control,h_flow_agg_enet2[APP_MAX_FLOW_AGG_HDLC];
WP_handle               h_rport_hdlc[APP_MAX_HDLC_PORTS], h_rport_enet2;
WP_handle               h_enet1_rx, h_enet1_host_tx, h_enet1_tx;
WP_handle               h_enet2_rx, h_enet2_tx;
WP_handle               h_enet2_tx_ch[APP_MAX_CHANNEL];
WP_handle               h_hdlc_rx[APP_MAX_CHANNEL],h_hdlc_tx[PQBLOCK_SIZE][APP_MAX_HDLC_PORTS];// each PQ-block will have n channel
WP_handle               pqblock_handle[PQBLOCK_SIZE];
WP_handle               policer_handle;


WP_handle               status;
WP_handle               emphy_port;

/* TAGS */
WP_tag tag_agg_enet;
WP_tag tag_bport_enet1;
WP_tag tag_bport_enet2;
WP_tag tag_bridge1_bport_hdlc[APP_MAX_PORT_HDLC];
WP_tag tag_bridge2_bport_hdlc[APP_MAX_PORT_HDLC ];
WP_tag tag_rport_hdlc[APP_MAX_HDLC_PORTS];
WP_tag tag_rport_enet2;
WP_tag tag_agg_enet2[APP_MAX_FLOW_AGG_HDLC];
WP_tag tag_iw_sys_bridge1, tag_iw_sys_bridge2;
WP_tag tag_agg_default_bridge1, tag_agg_default_bridge2;
WP_tag tag_enet1_rx, tag_enet2_rx;
WP_tag tag_enet1_tx, tag_enet2_tx;
WP_tag tag_enet2_tx_ch[PQBLOCK_SIZE];
WP_tag tag_hdlcrx = 200;
WP_tag tag_hdlctx = 300;
WP_tag tag_agg_hdlc[APP_MAX_FLOW_AGG_HDLC];
WP_tag tag_agg_pppsw[PQBLOCK_SIZE];
WP_tag tag_agg_lcp, tag_agg_control;
WP_boolean debug_on = WP_FALSE;

WP_iw_agg_bridging iw_agg_hdlc_param[PQBLOCK_SIZE];
WP_iw_agg_ppp_switching iw_agg_ppp_switching_param[APP_MAX_PORT_HDLC];
WP_iw_agg_directmap iw_agg_directmap_cfg_param[APP_MAX_FLOW_AGG_HDLC];
