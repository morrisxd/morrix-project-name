/*****************************************************************************
 * (C) Copyright PMC - WIN division (Wintegra) 2011.  All rights reserved.
 * PMC-Sierra (WINTEGRA) CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of PMC-Sierra (Wintegra).
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *  Application: ENET OAM
 *  File Name   : enet_oam.h
 *
 *****************************************************************************/
/*NOTE: uncomment the macro, compiling for wp3 board... by default compilation will be for WP3 SLB board*/

#include "brouter_pecs.h"
#include "mpls_pecs.h"

#define BOARD_WP3

#define ENET_OAM_DEBUG

//#define APP_VLAN_ID          0xa
//#define APP_MAX_MAC_ADDRESS  10

//#define APP_MAX_PKTS_NUM  3

/* Enet Bridge ports */
#define APP_MAX_VLAN       8
#define APP_MAX_MA         16
#define APP_MAX_MEP        16

#define APP_NUM_IW_BUFFERS   8192
#define APP_IW_BUFFER_SIZE   1984
#define APP_IW_BUFFER_OFFSET 64
#define APP_MASTER_WPID      WP_WINPATH(0)

#define APP_NUM_HW_PCE_RULES   1000

#define SYSTEM_ONE_MAX_FLOWS    64
#define IW_SYS_MAX_DB_DFC_FLOWS 20
#define WPE_MAX_PTP_PORTS      8
/* MA macros     */
#define MSG_Q_SIZE                             (64*256) /* Min size = 64, MAX size = 64*256 = 2^14 */
#define MSG_Q_RATE                             4

/* MEP macros    */

enum WPE_Flow_Aggregation_types
{
   FA_PORT_ENET1 = 0,
   FA_PORT_ENET2,
   FA_PORT_ENET3,
   FA_RES_ENET_OAM,
   FLOW_AGG_HOST,
   MAX_FLOW_AGG,
};

enum WPE_B_Port_types
{
   ENET1_BPORT,
   ENET2_BPORT,
   ENET3_BPORT,
   ENET_OAM_DUMMY_BPORT,
   HOST_BPORT_DL,
   HOST_BPORT_UL,
   ENET2_DUMMY_BPORT,          //for performance test
   MAX_BPORTS
};

enum WPE_IW_System_types
{
   UL_IW_SYS,
   DL_IW_SYS,
   MAX_IW_SYS
};

enum WPE_Filter_types
{
   UL_FILTER,
   DL_FILTER,
   MAX_FILTER
};

enum WPE_ENET_ports
{
   ENET1,
   ENET2,
   ENET3,
   MAX_ENETS
};


/* The WPE_EOAM_DB data structure holds parameters
 * of the system of this example. It holds handles
 * of ports, devices, qnodes, channels, etc.
 */

typedef struct
{
    WP_U32		maId;
    WP_U32      rMepId;
    WP_U8		fCcmEnable;
    WP_U8		fLmm_enable;
    WP_U8		fDm1_enable;
    WP_U8		fDmm_enable;
    WP_U32		ccmRate;
    WP_U32		lmmRate;
    WP_U32		dm1Rate;
    WP_U32		dmmRate;

}WPE_EOAM_CONFIG_T;

typedef struct
{
      /*handle of interworking system*/
      WP_handle h_iw_sys_bridge[MAX_IW_SYS];
      /*ports*/
      WP_handle h_port_iwhost;
      WP_handle h_port_enet[MAX_ENETS];

      /*devices*/
      WP_handle h_device_iwhost;
      WP_handle h_device_enet[MAX_ENETS];
	  
      /*channels*/
      WP_handle rx_host_handle;
      WP_handle tx_host_handle;
      WP_handle rx_res_enet_oam_host;
      WP_handle tx_enet_handle[MAX_ENETS];
      WP_handle rx_enet_handle[MAX_ENETS];

      /*bport and agg*/
      WP_handle bports[MAX_BPORTS];
      WP_handle flow_agg[MAX_FLOW_AGG];

      /*pool, buffer, qnode*/
      WP_handle h_pool_ring_host;
      WP_handle h_pool_buffer_iw;
      WP_handle h_pool_256;
      WP_handle h_qnode_iwq;
      WP_handle h_qnode_host;

      /* ptp clock domain*/
      WP_handle h_clock_domain;
      WP_THREAD_ID tid;

      /*filter handle*/
      WP_handle h_pce_filter[2];
      WP_handle h_pce_filterset[2];
      WP_handle h_pce_if[2];
      WP_handle h_pce_rule;

      /*pces handle*/
      WP_handle h_pecs[2];

      /*MA and MEP*/
      WPE_EOAM_MA_T eoam_ma_table[EOAM_MAX_MA_NUM];
      WP_U32        ma_num;
	  
      /*user configuration*/
      WPE_EOAM_CONFIG_T eoam_config[16];
	  
      /*performance test resources*/
      //WPE_EOAM_MA_T perf_ma_table[EOAM_PERF_TEST_MAX_MA_NUM];
      WPE_EOAM_MA_PD_T* perf_ma_table;
      WP_handle lm_dummy;
	  
} WPE_EOAM_DB;


/* Utility macros */
#define MAC_COPY(d,s)           { int _i; \
	for(_i=0;_i<6;_i++) \
	*((d)+_i)=*((s)+_i); }

extern WP_U32 dps_ProgramImage[];
extern WP_U16 dps_PC_Table_Init[];

extern WP_pecs_image_info brouter_pecs_pecs_image;
extern WP_pecs_image_info mpls_pecs_pecs_image;
extern WP_pce_parser_info pce_parser_info;


#ifdef BOARD_WP3
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
      { /* ASU TX QUEUE    */ WP_BUS_PACKET, APP_BANK_PACKET},
      { /* STATISTICS      */ WP_BUS_PARAM, APP_BANK_PARAM},
      { /* ADDRESS LOOKUP  */ WP_BUS_HOST, APP_BANK_HOST},
      { /* ATM CPT         */ WP_BUS_PARAM, APP_BANK_PARAM},
      { /* ENET HDLC CPT   */ WP_BUS_PARAM, APP_BANK_PARAM},
      { /* IW FBP PARAM    */ WP_BUS_PACKET, APP_BANK_PACKET},
      { /* IW FBP DATA     */ WP_BUS_PACKET, APP_BANK_PACKET},
      { /* EXTERNAL FIFO   */ WP_BUS_PACKET, APP_BANK_PACKET},
      { /* MAC LOOKUP      */ WP_BUS_PARAM, APP_BANK_PARAM},
      { /* IMA DISPATCH    */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
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

#else /* for WP3 SLB board*/ 

WP_context context_cfg =
{
   4,
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
      //{/* UNASSIGNED6				*/ WP_BUS_NONE, 	0},
      { /* WP_VB_IMA_RX_BUS    */ WP_BUS_PARAM, APP_BANK_PARAM},     /* Rx buffer */
      {/* PKT_GLOBAL_TCPT			*/ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
      //{/* AAL2_QSR 				*/ WP_BUS_NONE, 	0},
      { /* AAL2_QSR/IMATXBUF   */ WP_BUS_PARAM, APP_BANK_PARAM},       /* Tx buffer */
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
   NULL,	  /* Interrupt queue tables */
   0,			  /* Maxumum number of pools */
   NULL,		   /* ATM global parameters */
   NULL,			  /* Interworking global parameters */
};

#endif

static WP_int_queue_table int_queue_tables = {{{1000, 1}, {100, 1}, {10, 1}, {10, 1}}};

static WP_iw_global  iw_global_cfg[1] ={
    {
         /* max_iw_aggregations */ MAX_FLOW_AGG + 2,
         /* max_r_iw_systems;*/ 0,
         /* max_s_iw_systems;*/ 0,
         /* max_b_iw_systems;*/ 3,
         /* max_m_iw_systems;*/ 0,
         /* max_vlan_priority_maps */1,
         /* iw_bkgnd_fifo_size */1000,
         /* cong_pt       */NULL,
         /* iw_host_limits */ {64,0,0,1,0},
         /* mpls_config*/ {0,0},
         /* iw_modes;*/
         {  /*policer_mode;*/WP_IW_POLICER_DISABLE,
            /*statistics_bundle_stat_mode;*/WP_IW_STATISTICS_BUNDLE_STAT_DISABLE,
            /*l2_prefix_ext_mode;*/WP_IW_L2_HEADER_EXTENSION_DISABLE,
            /*enhanced_flow_stat_mode;*/WP_IW_ENHANCED_FLOW_STAT_DISABLE,
            /*flow_stat_mode;*/WP_IW_FLOW_STAT_ENABLE,
            /*fr_tx_flow_stat_mode*/WP_IW_FR_TX_FLOW_STAT_DISABLE,
            /*mfc_alloc_mode*/ WP_IW_MFC_ALLOC_ENABLE,
            /*learning_queue_mode */WP_IW_LEARNING_DISABLE,
            /*port_filtering_mode */WP_IW_PORT_FILTERING_DISABLE,
         /* lpm_alloc_mode */                   WP_IW_LPM_ALLOC_DISABLE,
         /*enhanced_mc_member_stat_mode*/       WP_DISABLE,
         /*routing_alloc_modes*/                WP_DISABLE,
         /*res_mc_addresses_table_mode*/        WP_DISABLE,
         /*port_stag_etype_mode*/               WP_IW_PORT_STAG_ETYPE_ENABLE,
         /* iw_characteristics */  WP_IW_MODES_IWGP_BACKGROUND_ENABLE | WP_IW_MODES_FLOW_CLASS_RULE_STAT_ENABLE,
            /***********************************************************/
         },
         /* max_iw_stat_bundles*/ 0,
         /* max_fr_s_iw_systems*/ 0,
         /* max_bridging_ports */ MAX_BPORTS + 2,
         /* max_iw_mc_groups */ 0,
         /* max_nat_systems */ 0,
         /* max_iw_second_aggregations */ 0,
         /* max_iw_comp_limits */ 0,
         /* max_iw_mc_members */ 0,
         /* max_iw_programmble_filters */0,
         /* max_iw_policers */ 0,
         /* max_dscpmt */ 0,
         /* max_wred_bindings */ 0,
         /* tcm_policer_limits */
         {
            0,
            0
         },
      /* max_flow_rule_stat_tables */  0,
      /* max_indirect_action_tables */ 0,
      /*lpm_dfc_limits*/{0},
      /*brigding_time_stamp_info*/{0},
      /*background_fifo_size*/200
      }
};

WP_calendar_wheel calendar[2] =
{
       {
            /* function */    WP_CW_ASU,
            /* cwid*/         WP_CW_ID_A,
            /* min_ch_rate */ 150,
            /* max_ch_rate */ 600000,
       },

       WP_CALENDAR_WHEEL_TERM
};

/*ENET port configuration*/
WP_port_enet port_enet_cfg =
{
   /* pkt_limits             */ 
   {
      /* max_tx_channels*/ 64,
      /* max_rx_channels*/ 64,
      /* pq_block_size*/   0,
      /* n_pq_blocks*/     0,
      /* emphy_devices*/   0,
   },
   /* flowmode               */ WP_FLOWMODE_FAST,
   /* interface_mode */ WP_ENET_SGMII_1000,
   /* rx_iw_bkgnd            */ WP_IW_BKGND_NOT_USED,
};

/* Enet device configuration */
WP_device_enet device_enet_cfg = 
{
   /* max_tx_channels */ 1,
   /* tx_maxsdu       */ 1536,
   /* rmii_operating_speed */ WP_UNUSED,
   /* mac_addr[6] */     {0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa},
   /* tx_bit_rate */     1000000000,
   /* loopbackmode  */   WP_ENET_NORMAL,//LOOPBACK,
   /* extended_params */ NULL,
};

#ifdef BOARD_WP3
WP_pool_ring_data pool_ring_data_cfg = 
{
   /* n_rings */ 50,
   /* ring_length */ 30,
   /* bus */ WP_BUS_PARAM,
   /* bank */ APP_BANK_PARAM
};

WP_pool_buffer_data pool_buffer_data_256_cfg = 
{
   /* n_buffers */ 8192 * 10,
   /* offset */ 0x40,
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

#else/* for WP3 SLB board*/ 
WP_pool_ring_data pool_ring_data_cfg = 
{
   /* n_rings */ 500,
   /* ring_length */ 10,
   /* bus */ WP_BUS_HOST,
   /* bank */ APP_BANK_HOST
};

WP_pool_buffer_data pool_buffer_data_256_cfg = 
{
   /* n_buffers */ 500,
   /* offset */ 0x40,
   /* size */ 256,
   /* pad */ 0,
   /* bus */ WP_BUS_HOST,
   /* bank */ APP_BANK_HOST,
};

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

WP_qnode_hostq qnode_hostq_cfg =
{
   /* pool_buffer */ 0,
   /* pool_ring */ 0,
   /* interruptqueue */ WP_IRQT1
};

WP_bridge_port bport_conf[1] =
      {
         {
            /* tag */ 0,
            /* direct_mapping*/WP_IW_DIRECT_MAP_ENABLE,
            /* flow_agg */0,
            /* termination_mode*/WP_IW_HOST_TERM_MODE,
            /* learning_mode */WP_IW_LEARNING_DISABLE,
            /* in_filter_mode */WP_IW_INGRESS_FILTER_DISABLE,
            /* vlan_param */
            {
               /* vlan_acceptance_mode */0,
               /* vlan_tag */0  ,
               /* vlan_tunnel*/ 0,
               /* vlan_priority_enforce_mode*/0,
               /* stag_vlan_etype*/DEFAULT_STAG_TYPE
            },
            /* max_mac_addresses */ MAX_FLOW_AGG*4,
            /* group_tag */ 0,
            /*group_filtering_mode*/WP_IW_GROUP_FILTER_DISABLE,
            /*unk_mac_sa_filter;*/ 0,
            /*unk_mc_mode;*/ 0,
            /* bc_ht_mode;*/ 0,
            /*input_filters_mask;*/0,
            /*  output_filters_mask;*/0,
            /*statmode;*/WP_IW_PORT_STAT_ENABLE,
            /*unk_uc_mode*/WP_IW_UNK_UC_SR_DISABLE,
   /* Enable/Disable Bridge Classifier per (input) bridge port  */
            /*classification_flag*/WP_IW_BPORT_CLASSIFICATION_ENABLED,
         }
};

WP_rx_binding_bridging  rx_binding_bri[1]=
{
         {
            /*  encap_mode */0,
            /*  mru;*/1536,
            /*  vcfcs;*/ 0,
            /*  input_port;*/0
         }
};

WP_tx_binding  tx_bind_enet[1]=
{
         {
            /* res0*/0,
            /* dci_mode;*/  WP_IW_DYN_CH_INSERT_ENABLE,
            /* maxt;*/255,
         }
};

static WP_iw_sys_bridging iw_sys_bridge_param[1]=
{
       {
            /* tag */0,
            /* max_flows; */SYSTEM_ONE_MAX_FLOWS,
            /* classification_mode */ WP_IW_NO_CLASSIFIER,
            /* classistatic void  fier_config */
            {
               /*classification_parsing_mode*/ 0,
               /*max_classification_rules;*/0,
               0,
               NULL
            },
            /* learning_mode;*/ WP_IW_BRIDGE_LEARNING_DISABLE,
            /* learning_queue */
            {
             /*int_queue_num*/0,
             /*learning_queue_size*/0,
             /*interrupt_enbale*/0,
             /*interrupt_rate*/0
            },
            /*forwarding_table_size*/WP_IW_2K_HASH_ENTRIES,
            /*member_set_size*/WP_IW_2K_HASH_ENTRIES,
            /*buffer_gap;*/0x40,
            /*max_bridging_ports*/MAX_BPORTS + 2,
            /*dfc info*/NULL,
            /*svl_mode*/0,
            /*stag_ether_type*/DEFAULT_STAG_TYPE
       }
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
   /* rx_queuedepth */50,
};

WP_qnode_iwq qnode_iwq_cfg =
{
   /* interruptqueue         */ WP_IRQT1,
   /* num_buffers            */ 8192,
   /* adjunct_pool           */ 0,
};

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

typedef struct
{
      WP_U16 in;
      WP_U16 out;
      WP_U16 size;
      WP_ima_event *task;
}
ima_app_task_list;

/* Interrupt task list */
#define IRQ_TASK_LIST_SIZE 8192
app_task irq_task[IRQ_TASK_LIST_SIZE];
app_task_list irq_task_list[1] = { { 0, 0, IRQ_TASK_LIST_SIZE, 0, irq_task} };
ima_app_task_list ima_irq_task_list[1] = { {0, 0, IRQ_TASK_LIST_SIZE,(WP_ima_event*) &irq_task} };

/* Application prototypes */
void App_Quit(WP_U32 status);
void App_TerminateOnError(WP_handle handle, WP_CHAR *s);
void App_TestInit(void);
void App_RamInit(void);
void App_SysInit(void);
void App_InitHW(void);
void App_PortsCreate(void);
void App_DevicesAllocCreate(void);
void App_BufferPoolsCreate(void);
void App_SystemSetup(void);
void App_ChannelsCreate(void);
void App_PortsDevicesEnable(void);
void App_ChannelsEnable(void);
void App_EnetOamSystemInit(void);
void app_perform_action(app_task *task);
static void App_perform_freerun (void);
void App_CheckStats (WP_status * test_result);

void add_task(app_task_list *task_list, WP_U32 event_type, WP_tag event_tag,
              void *event_param);
app_task *next_task(app_task_list *task_list, app_task *result);              

/* static functions */
static void WPE_HostCreate(void);
static void WPE_BridgingPortCreate(void);
static void WPE_IwSystemBuild(void);
static void WPE_FlowAggregationCreate(void);
static void WPE_IwPortModify(void);
static void WPE_BindRx(void);
static void WPE_BindTx(void);
static void WPE_FilterDefinition(void);
static void WPE_BasicPceRulesCreate (void);
static void WPE_EthernetOamModuleInit(void);
static void WPE_PceGpeModuleInit(void);

void WPE_PecsCreate (void);
void WPE_PceFilterCreate(void);
void WPE_PceFilterSetCreate (void);
void WPE_PceInterfaceCreate (void);

WP_status WPE_DebugCallback(const WP_CHAR *file, const WP_CHAR *function,
                            WP_S32 line, WP_S32 error_id);
void WPE_WddiCall(const WP_CHAR *file,
                  const WP_CHAR *function,
                  WP_S32 line);                            
void WPE_Reboot(void);
void WPE_SystemRelease(void);
void  WPE_PTPModuleInitWP3(void);
/* TAGS */
WP_tag tag_agg_enet;
WP_tag tag_iw_sys_bridge1;
WP_tag tag_bport_enet1;
WP_tag tag_bport_enet2;
WP_tag tag_bport_enet3;

WP_tag tag_iw_sys_bridge;
WP_tag tag_agg_default_bridge;

WP_tag tag_enet1_rx, tag_enet2_rx, tag_enet3_rx;
WP_tag tag_enet1_tx, tag_enet2_tx, tag_enet3_tx;
WP_tag tag_iwhost_rx1, tag_iwhost_rx_oam; //,tag_iwhost_rx2, tag_iwhost_rx3;

WP_boolean debug_on = WP_FALSE;

/******************************************************************************/
