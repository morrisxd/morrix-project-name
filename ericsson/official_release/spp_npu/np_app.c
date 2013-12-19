/* COPYRIGHT-ENEA-SRC-R2 *
**************************************************************************
* Copyright (C) 2009 by Enea AB.
* All rights reserved.
*
* This Software is furnished under a software license agreement and
* may be used only in accordance with the terms of such agreement.
* Any other use or reproduction is prohibited. No title to and
* ownership of the Software is hereby transferred.
*
* PROPRIETARY NOTICE
* This Software consists of confidential information.
* Trade secret law and copyright law protect this Software.
* The above notice of copyright on this Software does not indicate
* any actual or intended publication of such Software.
**************************************************************************
* COPYRIGHT-END */
/*----------------------------------------------------------------------------
 * File Name	:	  np_app.c
 * File Mark	 :	  none
 * Description	:	  npu application file
 * Others		 :	  none
 * Version       :       v1.00
 * Author		 :
 * Date 		 :
 *
 * Modify History		:
 *	   Date              Version         Author           Modification
 *-----------------------------------------------------------------------------
 2011-3-6              initial version   Richard zhou
 ----------------------------------------------------------------------------*/

#include "np_common.h"
#include "spp_npu_link.h"
#include "spp_wddi_server.h"
#include <search.h>  //hash table API
#include <semaphore.h>

#define ETH_ALEN	6		/* Octets in one ethernet addr	 */
/*
 *	This is an Ethernet frame header.
 */

struct ethhdr {
	WP_U8	h_dest[ETH_ALEN];	/* destination eth addr	*/
	WP_U8	h_source[ETH_ALEN];	/* source ether addr	*/
	WP_U16		h_proto;		/* packet type ID field	*/
} ;

struct vlan_ethhdr {
	WP_U8	h_dest[ETH_ALEN];
	WP_U8	h_source[ETH_ALEN];
	WP_U16		h_vlan_proto;
	WP_U16		h_vlan_TCI;
	WP_U16		h_vlan_encapsulated_proto;
};


struct udphdr {
	WP_U16	source;
	WP_U16	dest;
	WP_U16	len;
	WP_U16	check;
};
struct iphdr {
	WP_U8	ihl:4,
		version:4;
	WP_U8	tos;
	WP_U16	tot_len;
	WP_U16	id;
	WP_U16	frag_off;
	WP_U8	ttl;
	WP_U8	protocol;
	WP_U16	check;
	WP_U32	saddr;
	WP_U32	daddr;
	/*The options start here. */
};

extern int pipefd[2];
extern int pipefd_RxFrHprio[2];
extern int pipefdRxEth[2];
extern int pipefdRxEthHprio[2];
extern WP_U32 dps_ProgramImage[];
extern WP_U16 dps_PC_Table_Init[];
extern struct NODE* fr_dl_bvci_nodehead[];
extern struct NODE* fr_dl_dport_nodehead[];
extern struct NODE* update_bvci_nodehead;
extern WUFE_handle ufe_device_handle[N_UFE_MAX_LINES_T1][32];
extern int g_logLevel;

WP_CHAR appname[] = "spp npu";
#define WPI_MASK_BITS(n) ((1 << (n)) - 1)
#define WPI_INDEX_MASK WPI_MASK_BITS(18)
#define WPI_SHIFT_MASK(x,s,m) ((((WP_U32)(x)) >> (s)) & (m))
#define WPI_HANDLE_INDEX(x) WPI_SHIFT_MASK(x, 0, WPI_INDEX_MASK)


//static void AppTxEvent(WP_tag tag, WP_U32 data, WP_U32 info);
static void AppRxEvent(WP_tag tag, WP_U32 data, WP_U32 info);
#if NPU_HOST_OLP
void *pcimem_copy(void * b,const void * a,int n);
#endif
extern void fr_system_init(WP_U32 ufe_port);
extern void np_initial_fr_qnode(WP_U32 ufe_port);

WP_handle np_flow_host[10];

#define N_FLOWS							20000
#define N_IW_PORTS						10
#define N_IW_AGGS						40000

#define POOL_LIMIT 			8
#define LEARNING_QUEUE_SIZE 		200

#define STATISTICS_ENABLE    1


#define NPU_FILTER_NUM 6
#if BFD_OVERETH
#define NPU_FILTER_MASK_ALL 0xfc000000    /*6 filters 1111 11000*/
#define NPU_FILTER_MASK_DEFRAG 0x0c000000 /*0000 1100*/
#define NPU_FILTER_MASK_MAC 0x7c000000 /*0111 1100*/
#else

#define NPU_FILTER_MASK_ALL 0xf8000000    /*5 filters 1111 1000*/
#define NPU_FILTER_MASK_DEFRAG 0x18000000 /*0001 1000*/
#define NPU_FILTER_MASK_MAC 0x78000000 /*0111 1000*/
#endif

#define NPU_FILTER_DST_MAC  0
#define NPU_FILTER_DPORT_UDP_BFD 1
#define NPU_FILTER_SIP_UDP_PEER 2
#define NPU_FILTER_UDP_DEFRAG   3
#define NPU_FILTER_DPORT_UDP_IP_UL 4
#define NPU_FILTER_DPORT_UDP_FMC  5

/*add for BFD*/
#define MAX_BFD_TRANSMISSION_RATE 10
#define MIN_BFD_TRANSMISSION_RATE 1
#define MAX_BFD_DETECTION_TIME 10
#define MIN_BFD_DETECTION_TIME 1
#define MAX_BFD_RX_SESSIONS 1
#define MAX_BFD_TX_SESSIONS 1


volatile WP_U32 rx_event = 0;
volatile WP_U32 tx_event = 0;


WP_handle default_host_aggregation;
WP_handle agg_enet_1, agg_enet_2;
WP_handle agg_enet_dfc_1, agg_enet_dfc_2;

WP_handle qnode_iw,qniw_frag,qniw_defrag;
WP_handle iw_sys_bridging;

WP_handle gbep1, gbep2, gbedev1, gbedev2;
WP_handle pool_ring_host;
WP_handle pool_256,pool_2048,pool_defrag,pool_frag;

WP_handle iwp_host, iw_host_dev;
WP_handle iw_tx_host_handle;
WP_handle iw_rx_host_handle;

WP_handle tx_enet_handle_1, rx_enet_handle_1, tx_enet_handle_2, rx_enet_handle_2 ;

WP_flow_class_rule rule_cfg;
WP_flow_class_forwarding_action forwarding_action;
//WP_l2_forward forward_rule;

WP_handle ip_frag_system_handle;
WP_handle ip_defrag_context_pool;
WP_handle ip_defrag_hash_pool;


WP_int_queue_table int_queue_tables = {{{30000, 1}, {3000, 1}, {10, 1}, {10, 1}}};


WP_iw_global  max_iw[1] = {
    {
        /* max_iw_aggregations */ N_IW_AGGS,//N_FLOWS+1,
        /* max_r_iw_systems;*/ 2,
        /* max_directmap_enet_systems;*/ 0,
        /* max_b_iw_systems;*/ 5,
        /* max_m_iw_systems;*/ 0,
        /* max_vlan_priority_maps; */ 0,
        /* iw_bkgnd_fifo_size */4000,
        /* cong_pt       */NULL,
        /* iw_host_limits */ {N_UFE_MAX_PHYS*4+20, 0, 0,N_UFE_MAX_PHYS*4+20,0},/*configure tx rx channel number*/
        /* mpls_config*/ {0,0},
        /* iw_modes;*/
        {  /*policer_mode;*/                      WP_IW_POLICER_DISABLE,
           /*statistics_bundle_stat_mode;*/       WP_IW_STATISTICS_BUNDLE_STAT_DISABLE,
           /*l2_prefix_ext_mode;*/                WP_IW_L2_HEADER_EXTENSION_ENABLE,
           /*enhanced_flow_stat_mode;*/           WP_IW_ENHANCED_FLOW_STAT_DISABLE,
           /*flow_stat_mode;*/                    WP_IW_FLOW_STAT_ENABLE,
           /*fr_tx_flow_stat_mode */              WP_IW_FR_TX_FLOW_STAT_ENABLE,
           /*mfc_alloc_mode; */                   WP_IW_MFC_ALLOC_ENABLE,
           /*Learning_queue_mode */               WP_IW_LEARNING_DISABLE,
           /*port_filtering_mode */               WP_IW_PORT_FILTERING_ENABLE,
           /*lpm_alloc_mode */                    WP_IW_LPM_ALLOC_DISABLE,
           /* enhanced_mc_member_stat_mode; */    0,
        },

        /* max_ch_agg_stat;*/0,
        /* max_fr_s_iw_systems;*/ 2,
        /* max_iw_ports */ N_IW_PORTS,
        /* max_iw_mc_groups   */ 0,
        /* max_nat_systems */    0,
        /* max_iw_second_aggregations */ 0,
        /*  *iw_comp_limits */   NULL,
        /* max_iw_mc_members */  0,
        /* max_iw_programmable_filters */ 5,
        /* max_iw_policers */ 0
    }
};

WP_context context[1] =
{
    {10, /* qnodes */
     /* the first four assignments are mandatory bus assignments */
     {
         {/* WP_BUS_PARAM    */ WP_BUS_PARAM, APP_BANK_PARAM},
         {/* WP_BUS_PACKET   */ WP_BUS_PACKET, APP_BANK_PACKET},
         {/* WP_BUS_INTERNAL */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
         {/* WP_BUS_HOST     */ WP_BUS_HOST, APP_BANK_HOST},
         {/* WP_BUS_IRQ      */ WP_BUS_HOST, APP_BANK_HOST},
         {/* WP_BUS_TMSTMP   */ WP_BUS_HOST, APP_BANK_HOST},
         {/* UNASSIGNED      */ WP_BUS_NONE, 0},
         {/* UNASSIGNED      */ WP_BUS_NONE, 0},
         {/* ASU_TPT         */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
         {/* ASU CW          */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
         {/* ASU TX QUEUE    */ WP_BUS_PARAM, APP_BANK_PARAM},
         {/* STATISTICS      */ WP_BUS_HOST  , APP_BANK_HOST},
         {/* ADDRESS LOOKUP  */ WP_BUS_PARAM , APP_BANK_PARAM},
         {/* ATM CPT         */ WP_BUS_PARAM, APP_BANK_PARAM},
         {/* ENET HDLC CPT   */ WP_BUS_PARAM, APP_BANK_PARAM},
         {/* IW FBP PARAM    */ WP_BUS_PARAM, APP_BANK_PARAM},
         {/* IW FBP DATA     */ WP_BUS_PACKET, APP_BANK_PACKET},
         {/* EXTERNAL FIFO   */ WP_BUS_PACKET, APP_BANK_PACKET},
         {/* MAC LOOKUP      */ WP_BUS_PARAM, APP_BANK_PARAM},
         {/* IMA SMALL       */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
         {/* IMA LARGE       */ WP_BUS_PACKET, APP_BANK_PARAM},
         //{/* LPM             */ WP_BUS_HOST, APP_BANK_HOST},
         {/* LPM             */ WP_BUS_PARAM, APP_BANK_PARAM},
         {/* MFC RULES       */ WP_BUS_PARAM, APP_BANK_PARAM},
         {/* FIWT, QAT       */ WP_BUS_PARAM, APP_BANK_PARAM},
         {/* MFCPT, UPPPT, DS*/ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
         {/* WP_STATISTICS2  */ WP_BUS_HOST,APP_BANK_HOST},
         {/* WP_STATISTICS3  */ WP_BUS_HOST,APP_BANK_HOST},
         {/* UNASSIGNED      */ WP_BUS_NONE, 0},
         {/* PKT_GLOBAL_TCPT */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
         {/* WP_VB_AAL2_QSR  */ WP_BUS_PARAM, APP_BANK_PARAM},
         {/* MANDATORY       */ WP_BUS_NONE, 0},
         {/* MANDATORY       */ WP_BUS_NONE, 0},
         /* ************* VIRTUAL BUS ASSIGNMENT ***************/
         {/* MCH_DATA        */ WP_BUS_PARAM, APP_BANK_PARAM}
         ,
         {/* WP_VB_OAM_FM_QUERY*/ WP_BUS_NONE, 0},
         {/* WP_VB_HASH_ON_HEAP*/ WP_BUS_NONE, 0},
         {/* WP_VB_MFC_INTERNAL*/ WP_BUS_PARAM, APP_BANK_PARAM}
     },
     {
         dps_ProgramImage, dps_ProgramImage
     },
     {
         dps_PC_Table_Init, dps_PC_Table_Init
     },
     &int_queue_tables, /* Interrupt queue tables */
     POOL_LIMIT,        /* Mixumum number of pools */
     NULL,	        /* ATM global parameters */
     max_iw,          /* Interworking global parameters */
     0xA				/* dps_winfarm_mask for WP3 */
    }
};

/* Callback function for a queuenode - Need to define the correct type later*/
WP_S32 QNCallback(WP_S32 status) {
    Fprintf("App: QNCallback: Status returned is %d\n",status);
    return 0;
}


WP_pool_buffer_data buffer_data_2048[] = {
    {/* n_buffers */ 2000,
     /* offset */ 64,
     /* size */ 1984,
     /* pad */ 0,
     /* bus */ WP_BUS_PACKET,
     /* bank */ APP_BANK_PACKET }
};
WP_pool_buffer_data buffer_data_defrag[] = {
    {/* n_buffers */ 2000,
     /* offset */ 64,
     /* size */ 8096,
     /* pad */ 0,
     /* bus */ WP_BUS_PACKET,
     /* bank */ APP_BANK_PACKET }
};

WP_pool_buffer_data buffer_data_frag[] = {
    {/* n_buffers */ 1000,
     /* offset */ 64,
     /* size */ FRAG_SIZE+256,
     /* pad */ 0,
     /* bus */ WP_BUS_PACKET,
     /* bank */ APP_BANK_PACKET }
};

WP_qnode_iwq qn_iw[1]=
{
    {
        /* interruptqueue */ 0,
        /* num_buffers */ 10000/*10000*/,
        /* adjunct_pool */ 0
    }
};
WP_qnode_iwq qn_iw_defrag[1]=
{
    {
        /* interruptqueue */ 0,
        /* num_buffers */ 1000,
        /* adjunct_pool */ 0
    }
};

WP_qnode_iwq qn_frag_iw[1]=
{
    {
        /* interruptqueue */ 0,
        /* num_buffers */ 2000,
        /* adjunct_pool */ 0
    }
};


/* Giga Ethernet port1 & 2 configuration */
WP_port_enet gbe_s[1] =
{
    {
        /* pkt_limits */ {64, 64, 0, 0},
        /* flowmode */ WP_FLOWMODE_FAST,
        /* interface_mode */ WP_ENET_1000_BASE_X_AN,//WP_ENET_SGMII_1000,
        /* rx_iw_bkgnd */ WP_IW_BKGND_NOT_USED
	}
};

/* Giga Ethernet port1 configuration */
/* ENET port and device params */
WP_mii_mgmt mii_mgmt_cfg[1] =
{
    {
        100000000  /* Interface speed, currently unused */
    }
};

WP_enet_filters gbe_rx_filters[1] =
{
    {
        /* rx_ucfilter WP3   */ WP_ENET_FILTER_PROCESS,
        /* rx_bcfilter       */ WP_ENET_FILTER_HOSTTERM,
        /* rx_mcfilter       */ WP_ENET_FILTER_HOSTTERM
    }
};

WP_device_enet_ex ext_device_enet[1] =
{
    {
	    /* duplexmode */		WP_ENET_FULLDUPLEX,
		/* rx_maclookup_mode */ WP_DISABLE,
		/* rx_flowcontrol */ 	WP_ENET_FLOW_ENABLE,
		/* tx_flowcontrol */ 	WP_ENET_FLOW_ENABLE,
		/* rx_addrmode */ 		WP_ENET_ADDR_ACCEPTALL,
#if STATISTICS_ENABLE
		/* phystatmode */		WP_ENET_STAT_ENABLE,
#else
		/* phystatmode */		WP_ENET_STAT_DISABLE,
#endif
#if STATISTICS_ENABLE
	    /* tx_statmode */		WP_ENET_STAT_ENABLE,
#else
	    /* tx_statmode */		WP_ENET_STAT_DISABLE,
#endif
#if STATISTICS_ENABLE
	    /* rx_statmode */		WP_ENET_STAT_ENABLE,
#else
	    /* rx_statmode */		WP_ENET_STAT_DISABLE,
#endif
		/* tx_duplicate */		WP_DISABLE,
		/* *rx_filters */ 		gbe_rx_filters,
	}
};


/* Giga Ethernet device 1 configuration */
WP_device_enet gbe_dev[1] =
{
    {
        /* max_tx_channels */   10,
        /* tx_maxsdu */         N_MAX_IP_FRAME_SIZE,
        /* operating_speed */   WP_UNUSED,
        /* mac_addr[6];*/       {0xAA,0xBB,0xCC,0xDD,0xEE,0xFF},
        /* tx_bit_rate */       1000000000,
        /* loopbackmode */      WP_ENET_NORMAL,
        /* extended_params */   NULL,
    }
};


/* Channel configuration for GigaEthernet */
WP_ch_enet ch_enet[1] =
{
    {
        /* intmode */ WP_PKTCH_INT_DISABLE,
        /* iwmmode */  WP_PKTCH_IWM_ENABLE,
        /* testmode */ WP_PKTCH_TEST_DISABLE,
        /* tx_pqblock */ 0,
        /* tx_pqlevel */ 0,
        /* tx_shaping_type */ WP_FMU_SHAPING_TYPE_STRICT,
        /* tx_shaping_params */ NULL,
        /* rx_maxsdu */ N_MAX_IP_FRAME_SIZE,
        /* tx_cwid */ 0,
        /* tx_tq */ 0,
        /* rx_queuedepth */ 512,
    }
};


/* DFC Structures */


WP_dfc_sys_info dfc_sys_info[1] =
{
    {
        /* classification_hash_size */ WP_IW_2K_HASH_ENTRIES,
        /* port_filtering */           WP_IW_PORT_FILTERING_ENABLE,
        /* deny_eq_ip */               WP_IW_DENY_EQUAL_IP_DISABLE,
        /* dfc_options */              WP_DISABLE
    }
};


static WP_dfc_flow_info enet_to_enet_dfc_flow_info[NPU_FILTER_NUM] =
{
	{
        /* filter_handle */ 0,
        /* input_port */    0,
        /* output_port */   0,
	},
	{
        /* filter_handle */ 0,
        /* input_port */    0,
        /* output_port */   0,
	},
    {
        /* filter_handle */ 0,
        /* input_port */    0,
        /* output_port */   0,
    },
    {
        /* filter_handle */ 0,
        /* input_port */    0,
        /* output_port */   0,
    }
};




static WP_classification_filter enet_to_enet_classification_filter_cfg[NPU_FILTER_NUM] =
{
	{
		/* fields_mask */	  WP_FIELD_IP_SA | WP_FIELD_PROTOCOL,
		/* no_match_action */ WP_CLASS_FILTER_NO_MATCH_CONTINUE,
		/* filter_type */	  WP_CLASS_FILTER_EMC,
	},

	{
		/* fields_mask */	  WP_FIELD_PROTOCOL ,
		/* no_match_action */ WP_CLASS_FILTER_NO_MATCH_CONTINUE,
		/* filter_type */	  WP_CLASS_FILTER_EMC,
	},
	{
		/* fields_mask */	  WP_FIELD_DST_PORT ,
		/* no_match_action */ WP_CLASS_FILTER_NO_MATCH_CONTINUE,
		/* filter_type */	  WP_CLASS_FILTER_EMC,
	},
	{
        /* fields_mask */     WP_FIELD_DST_PORT | WP_FIELD_PROTOCOL ,
        /* no_match_action */ WP_CLASS_FILTER_NO_MATCH_ACCEPT,
        /* filter_type */     WP_CLASS_FILTER_MFC,
	}

};



static WP_flow_class_rule enet_to_enet_flow_policy_cfg[NPU_FILTER_NUM] =
{
    {
		/*	classifier_string */	   NULL,
		/*	dfc_info */ 			   NULL,
		/*	match_action */ 		   WP_FLOW_CLASS_MODE_FLOW_AGG,
		/*	forwarding_action */	   {{0}},
		/*	wred_entry */			   0,
    },
	{
        /*  classifier_string */ 	  NULL,
        /*  dfc_info */				  NULL,
        /*  match_action */			  WP_FLOW_CLASS_MODE_FLOW_AGG,
        /*  forwarding_action */ 	  {{0}},
        /*  wred_entry */			  0,
	},
	{
        /*  classifier_string */ 	  NULL,
        /*  dfc_info */				  NULL,
        /*  match_action */			  WP_FLOW_CLASS_MODE_FLOW_AGG,
        /*  forwarding_action */ 	  {{0}},
        /*  wred_entry */			  0,
	},
    {
        /*  classifier_string */		 NULL,
        /*  dfc_info */				 NULL,
        /*  match_action */			 WP_FLOW_CLASS_MODE_FLOW_AGG,
        /*  forwarding_action */		 {{0}},
        /*  wred_entry */ 			 0,
    }

};





WP_iw_sys_bridging iw_sys_one[1]=
{
    {
        /* tag */        0,
        /* max_flows; */ N_IW_AGGS+1,
        /* classification_mode */WP_IW_DYNAMIC_CLASS,
        /* classifier_config */ {WP_IW_DYNAMIC_CLASS,N_FLOWS,0,NULL},
        /* learning_mode;*/WP_IW_BRIDGE_LEARNING_DISABLE,
        /* learning_queue */
        {
            /*int_queue_num*/WP_IW_IRQT0,
            /*learning_queue_size*/LEARNING_QUEUE_SIZE,
            /*interrupt_enable*/WP_LEARNING_POLLING_MODE,
            /*interrupt_rate*/1
        },
        /* forwarding_table_size*/WP_IW_32K_HASH_ENTRIES,
        /* member_set_size*/WP_IW_32K_HASH_ENTRIES,
        /* buffer_gap;*/0x40,
        /* max_bridging_ports*/ 6,
        /* *dfc_info */         dfc_sys_info
    }
};


/*** To Do Enable Learning ***/
WP_bridge_port bport_conf[1] =
{
    {
        /* tag */ 0x5555,
        /* direct_mapping*/WP_IW_DIRECT_MAP_DISABLE,
        /* flow_agg */0,
        /* termination_mode*/ WP_IW_HOST_TERM_MODE,
        /* learning_mode */WP_IW_LEARNING_DISABLE,
        /* in_filter_mode */WP_IW_INGRESS_FILTER_DISABLE,
        /* vlan_param */
        {
            /* vlan_acceptance_mode */WP_IW_ACCEPT_TAGGED_UNTAGGED,
            /* vlan_tag */0xa,
            /* vlan tunnel */ WP_IW_VLAN_TUNNEL_1Q_IN_1Q_DISABLE,
            /* vlan_priority_enforce_mode */ WP_IW_VLAN_PRIORITY_ENFORCE_DISABLED,
            /* stag_vlan_etype */ WP_IW_BPORT_STAG_VLAN_ETYPE_DISABLE
        },
        /* max_mac_addresses */ N_FLOWS,
        /* group_tag */ 0,
        /* group_filtering_mode*/WP_IW_GROUP_FILTER_DISABLE,
        /*WP_U8 unk_mac_sa_filter*/ WP_IW_UNK_MACSA_FILTER_DISABLE,
        /*WP_U8 unk_mc_mode;*/      WP_IW_UNK_MC_DROP,
        /*WP_U8 bc_ht_mode;*/       WP_IW_BC_HT_DISABLE,
        /*WP_U32 input_filters_mask;*/ 0x80000000,
        /*WP_U32 output_filters_mask;*/0,
#if STATISTICS_ENABLE
        /*WP_iw_port_statmode statmode;*/WP_IW_PORT_STAT_ENABLE,
#else
        /*WP_iw_port_statmode statmode;*/WP_IW_PORT_STAT_DISABLE,
#endif
        /* unk_uc_mode;    */ WP_IW_UNK_UC_SR_DISABLE,
        /* classification_flag; */ WP_IW_BPORT_CLASSIFICATION_ENABLED,
        /* adv_unk_lookup_mode; */ WP_IW_ADV_UNK_LOOKUP_DISABLED,
        /* cfi_ht_mode;              */ WP_IW_CFI_HT_DISABLE,
        /* reserved_mc_ht_mode; */ WP_IW_RES_MC_HT_DISABLE
#if 0
        /* predefined_ht_mode*/ WP_DISABLE,
        /* res_mc_action_table, */ { },
        /* enet_oam_params; */
        {
            /* enet_oam_mode; */ WP_DISABLE,
            /* md_action[WP_IW_ENET_OAM_MAX_MDS] */ NULL,
            /* use_macsa_rem_mep_lookup; */ WP_DISABLE
        },
        /* dfc_log_mode; */ WP_IW_PORT_DFC_LOG_DISABLE
#endif
    }
};

WP_bridge_port update_bvci_bport_conf[1] =
{
    {
        /* tag */ 0x1234,
        /* direct_mapping*/WP_IW_DIRECT_MAP_DISABLE,
        /* flow_agg */0,
        /* termination_mode*/ WP_IW_HOST_TERM_MODE,
        /* learning_mode */WP_IW_LEARNING_DISABLE,
        /* in_filter_mode */WP_IW_INGRESS_FILTER_ENABLE,
        /* vlan_param */
        {
            /* vlan_acceptance_mode */WP_IW_ACCEPT_TAGGED_UNTAGGED,
            /* vlan_tag */0x85,
            /* vlan tunnel */ WP_IW_VLAN_TUNNEL_1Q_IN_1Q_DISABLE,
            /* vlan_priority_enforce_mode */ WP_IW_VLAN_PRIORITY_ENFORCE_DISABLED,
            /* stag_vlan_etype */ WP_IW_BPORT_STAG_VLAN_ETYPE_DISABLE
        },
        /* max_mac_addresses */ N_FLOWS,
        /* group_tag */ 0,
        /* group_filtering_mode*/WP_IW_GROUP_FILTER_DISABLE,
        /*WP_U8 unk_mac_sa_filter*/ WP_IW_UNK_MACSA_FILTER_DISABLE,
        /*WP_U8 unk_mc_mode;*/      WP_IW_UNK_MC_DROP,//WP_IW_UNK_MC_HT,
        /*WP_U8 bc_ht_mode;*/       WP_IW_BC_HT_DISABLE,//WP_IW_BC_HT_ENABLE,
        /*WP_U32 input_filters_mask;*/ 0x40000000,
        /*WP_U32 output_filters_mask;*/0,
#if STATISTICS_ENABLE
        /*WP_iw_port_statmode statmode;*/WP_IW_PORT_STAT_ENABLE,
#else
        /*WP_iw_port_statmode statmode;*/WP_IW_PORT_STAT_DISABLE,
#endif
        /* unk_uc_mode;    */ WP_IW_UNK_UC_SR_ENABLE,
        /* classification_flag; */ WP_IW_BPORT_CLASSIFICATION_ENABLED,
        /* adv_unk_lookup_mode; */ WP_IW_ADV_UNK_LOOKUP_DISABLED,
        /* cfi_ht_mode;              */ WP_IW_CFI_HT_DISABLE,
        /* reserved_mc_ht_mode; */ WP_IW_RES_MC_HT_DISABLE
#if 0
        /* predefined_ht_mode*/ WP_DISABLE,
        /* res_mc_action_table, */ { },
        /* enet_oam_params; */
        {
            /* enet_oam_mode; */ WP_DISABLE,
            /* md_action[WP_IW_ENET_OAM_MAX_MDS] */ NULL,
            /* use_macsa_rem_mep_lookup; */ WP_DISABLE
        },
        /* dfc_log_mode; */ WP_IW_PORT_DFC_LOG_DISABLE
#endif
    }
};


WP_tx_binding  tx_binding[1]=
{
    { // Ethernet Tx binding
        /* wred_mode;*/ 0,
        /* dci_mode;*/  WP_IW_DYN_CH_INSERT_DISABLE,
        /* maxt;*/1000,
    }
};



WP_rx_binding_bridging rx_binding_enet[1]=
{
    {
        /*  encap_mode */0,
        /*  mru;*/ N_MAX_IP_FRAME_SIZE,
        /*  vcfcs;*/0,
        /*  input_port;*/0
    }
};

WP_rx_binding_routing rx_binding[1] =
{
    {
        /* encap_mode*/ 0,
        /* l3_support_protocol */ WP_IW_IPV4,
        /* mru */                 N_MAX_IP_FRAME_SIZE,
        /* rx_direction */0,
        /* input_port */0,
        /* decomp_group */0,
    }
};





WP_iw_agg_directmap tx_agg_enet[1] =
{
    {
        /* tag */                  0,
        /* txfunc */               0,
        /* rfcs */                 WP_IW_RFCS_DISABLE,
        /* l2header_insert_mode;*/ WP_IW_L2H_INSERT_DISABLE,
        /* interruptqueue;*/       WP_IW_IRQT0,
        /* error_pkt_mode*/        WP_IW_ERRPKT_DISCARD,
        /* intmode;*/              WP_IW_INT_DISABLE,
#if STATISTICS_ENABLE
        /* statmode;*/             WP_IW_STAT_ENABLE,
#else
        /* statmode;*/             WP_IW_STAT_DISABLE,
#endif
        /* timestamp_mode;*/       WP_IW_TIME_STAMP_DISABLE,
        /* ov_pool_mode*/          0,
        /* fbp_drop_threshold;*/   0,
        /* mtu;*/                  1536,
        /* prefix_length */        0, /* MAC header with VLAN */
        /* extraction_length */    0,
        /* prefix_header[68];*/    {	0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00,
                                        0x00, 0x00, 0x00, 0x04, 0x81, 0x00, 0x00, 0x00, 0x08, 0x00,
                                        0x45, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
                                        0x40, 0x11, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x02,
                                        0x00, 0x00, 0x00, 0x01, 0xff},
        /* policer_enable */       0,
        /* policer_config */       NULL,
        /* cong_mode */            0,
        /* cong_threshold_param */ NULL,
        /* dynamic_length_update*/ WP_IW_LENGTH_UPDATE_DISABLED,
        /* length_offset*/         0
    }
};

WP_iw_agg_directmap tx_agg_enet_dfc[1] =
{
    {
        /* tag */                  0,
        /* txfunc */               0,
        /* rfcs */                 WP_IW_RFCS_ENABLE,
        /* l2header_insert_mode;*/ WP_IW_L2H_INSERT_ENABLE,
        /* interruptqueue;*/       WP_IW_IRQT0,
        /* error_pkt_mode*/        WP_IW_ERRPKT_DISCARD,
        /* intmode;*/              WP_IW_INT_DISABLE,
#if STATISTICS_ENABLE
        /* statmode;*/             WP_IW_STAT_ENABLE,
#else
        /* statmode;*/             WP_IW_STAT_DISABLE,
#endif
        /* timestamp_mode;*/       WP_IW_TIME_STAMP_DISABLE,
        /* ov_pool_mode*/          0,
        /* fbp_drop_threshold;*/   0,
        /* mtu;*/                  1536,
        /* prefix_length */        46, /* MAC header with VLAN = 18 + 28 for IP/UDP */
        /* extraction_length */    46,
        /* prefix_header[68];*/    {	0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
                                        0x00, 0x00, 0x00, 0x00, 0x00, 0x04,
                                        0x81, 0x00, 0x00, 0x00, 0x08, 0x00,
                                        0x45, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, /*IP */
                                        0x40, 0x11, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x04,
                                        0x00, 0x00, 0x00, 0x01,
                                        0x00, 0x00, 0x00, 0x04,  /* UDP */
                                        0xff, 0xff, 0x00, 0x00},
        /* policer_enable */       0,
        /* policer_config */       NULL,
        /* cong_mode */            0,
        /* cong_threshold_param */ NULL,
        /* dynamic_length_update*/ WP_IW_LENGTH_UPDATE_L3,
        /* length_offset*/         18
    }
};


WP_iw_agg_directmap_enet tx_agg_enet_default[1] =
{
    {
        /* tag */                  0,
        /* txfunc */               0,
        /* rfcs */                 WP_IW_RFCS_ENABLE,
        /* l2header_insert_mode;*/ WP_IW_L2H_INSERT_DISABLE,
        /* interruptqueue;*/       WP_IW_IRQT1,
        /* error_pkt_mode*/        WP_IW_ERRPKT_DISCARD,
        /* intmode;*/              WP_IW_INT_ENABLE,
#if STATISTICS_ENABLE
        /* statmode;*/             WP_IW_STAT_ENABLE,
#else
        /* statmode;*/             WP_IW_STAT_DISABLE,
#endif
        /* timestamp_mode;*/       WP_IW_TIME_STAMP_DISABLE,
        /* ov_pool_mode*/          0,
        /* fbp_drop_threshold;*/   0,
        /* mtu;*/                  1536,
        /* prefix_length */        /*34*/ 0,
        /* extraction_length */    0,
        /* prefix_header[68];*/    {	0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
                                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                        0x08, 0x00,
                                        0x45, 0x00, 0xFF, 0xFF,
                                        0x00, 0x00, 0x00, 0x00,
                                        0x40, 0x11, 0xFF, 0xFF,
                                        0x00, 0x00, 0x00, 0x02,
                                        0x00, 0x00, 0x00, 0x01, 0xff},
        /* policer_enable */       0,
        /* policer_config */       NULL,
        /* cong_mode */            0,
        /* cong_threshold_param */ NULL,
        /* dynamic_length_update*/ /*WP_IW_LENGTH_UPDATE_L3*/WP_IW_LENGTH_UPDATE_DISABLED,
        /* length_offset*/         0
    }
};



WP_ch_iw_rx ch_config_iw[1]=
{
    {
        /* pqblock */ 0,
        /* pqlevel */ 0,
        /* tx_binding_type */ WP_IW_TX_BINDING,
        /* tx_binding_config */ tx_binding
    }
};

/* IP Frag setup */
WP_iw_ip_fragmentation_system ip_frag_sys[1] =
{
    {
        /* n_frag_groups */ 3200,
        /* n_defrag_groups */ 2,
        /* n_defrag_context_pools */ 2,
        /* n_defrag_hash_lookup_tables */ 2,
        WP_IW_FRAG_FORWARDING_ANY
    }
};
WP_iw_ip_defrag_context context_config[1] =
{
    {
        /* n_contexts */ 3200,
    }
};
WP_iw_ip_defrag_hash hash_config[1] =
{
    {
        /* hash_size */ WP_IW_2K_HASH_ENTRIES,
    }
};
WP_iw_ip_frag_group frag_group[1] =
{
    {
        /* qniw */ 0,
        /* frag_size */ 1536,//1496,//256,
        /* buffer_gap */ 64
    }
};
WP_iw_ip_defrag_group defrag_group[1] =
{
    {
        /* defrag_pool */ 0,
        /* defrag_hash */ 0,
        /* qniw */ 0,
        /* defrag_mode */ WP_IW_IP_DEFRAG_NORMAL_MODE,
        /* buffer_gap */ 64,
        /* timeout_in_secs */ 1,
        /* timeout_hash_search_div_factor */
        WP_IW_IP_DEFRAG_TIMEOUT_HASH_ENTRIES_SEARCH_DIV_1,
        /* maxasdu */ N_MAX_IP_FRAME_SIZE
    }
};

WP_iw_agg_routing tx_agg_l3[1] =
{
    {
        /* tag */                  0,
        /* txfunc */               0,
        /* dfsrv_remarking */      WP_IW_DSRV_DISABLE,
        /* l2header_insert_mode */ WP_IW_L2H_INSERT_ENABLE,
        /* siphrm */               WP_IW_FAST_FWD_MODE,
        /* interruptqueue */       WP_IW_IRQT1,
        /* error_pkt_mode */       0,
        /* intmode */              WP_IW_INT_DISABLE,
        /* statmode */             WP_IW_STAT_ENABLE,
        /* timestamp_mode */       WP_IW_TIME_STAMP_DISABLE,
        /* ov_pool_mode */         0,
        /* fbp_drop_threshold */   0,
        /* mtu */                1536,
        /* dfsrv_code_point */     0,
        /* prefix_length */        18,
        /* multi_cong_prof */      0,
        /* prefix_header[18] */    {0x00, 0x23, 0x24, 0x1f, 0x54, 0x15, 0x01, 0x01,
                                    0x02, 0x03, 0x04, 0x05,0x81,0x00,0x00,0x85,0x08,0x00},
        /* new_ip_header[20] */    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                    0x00, 0x00, 0x00, 0x00},
        /* policer_config */        0,
        /* maxt */                 0,
        /* cong_threshold_param */ 0,
    }
};


#define DATA_LENGTH 256
#define WPL_SIGNAL_RT_SER 35
#define WPL_SIGNAL_RT_OV (WPL_SIGNAL_RT_SER + 1)
#define WPL_SIGNAL_RT_BUSERR (WPL_SIGNAL_RT_SER + 2)


WP_calendar_wheel calendar[2] =
{
    {
        /* function */    WP_CW_PSU,
        /* cwid*/         WP_CW_ID_A,
        /* min_ch_rate */ WT_MIN_CH_RATE,
        /* max_ch_rate */ WT_MAX_CH_RATE
    },
    WP_CALENDAR_WHEEL_TERM
};

//static WP_CHAR enet_to_enet_emc_rule_string[512];
WP_handle iw_enet_1, iw_enet_2, iw_enet_3, iw_enet_1_tag, iw_enet_2_tag, iw_enet_3_tag;

WP_CHAR buf[4096];
NP_SYSTEM_INFO np_system = {0};
//extern int g_npinitialFlag;
WP_handle enet_iw_flow_defrag_aggregation;
WP_handle ipdefrag_agg;

#if USE_WDDI_LOG
static void sppwddiLog(int level, char *msg) {
    if(level <4)
    {
		Fprintf("WDDI LOG: %s \n",msg);
	}
    return;
}
#endif
void init_total_meminfo(void)
{

	STRU_MEMORY_STATICS get_memory_statics;
	STRU_SET_INITAIL_CONFIG *initial_cfg = npu_get_initial_cfg_info();

	Np_ShowMemoryAvailable(&get_memory_statics);

	initial_cfg->PacketMemTotalByte = get_memory_statics.PacketMemLeftByte;
	initial_cfg->InterMemTotalByte= get_memory_statics.InterMemLeftByte;
	initial_cfg->ParamMemTotalByte= get_memory_statics.ParamMemLeftByte;

}

/*
 * this will cal fr_system_init ()
 * 	- WPX_PortModeSet ()
 * 	- WPX_BoardConfigure ()
 * 	- WPX_BoardSerdesInit ()
 * 	- WP_PoolCreate ()
 * 	- WP_QNodeCreate ()
 * 	- WP_PortCreate ()
 * 	- WP_DeviceCreate ()
 * 	- WP_DeviceModify ()
 * 	- WP_IwIpFragmentationSystemCreate ()
 * 	- WP_IwIpDefragContextPoolCreate ()
 *	- WP_DeviceModify ()
 * 	- WP_IwIpDefragHashPoolCreate ()
 *	- WP_SysSchedulerCreate ()
 *	- WP_FeatureInit ()
 *	- WP_ControlRegister ()
 *	- np_initial_fr_qnode ()
 *	- npu_BfdModuleInit ()
 *	- 			WP_SysCommit ()
 *	- WP_PortEnable ()
 *	- WP_ChannelCreate ()
 *	- WP_IwSystemCreate ()
 *	- WP_IwFlowAggregationCreate ()
 *	- WP_IwPortCreate ()
 *	- WP_IwTxBindingCreate ()
 *	- WP_IwFlowAggregationCreate ()
 *	- WP_IwClassFilterAdd ()
 *	- np_dst_mac_filter_flowAdd ()
 *	- WP_IwRxBindingCreate ()
 *	- WP_IwSystemBuild ()
 * 	------------------------------------------- fr_system_init ()
 *	- WP_LookupCreate ()
 *	- WP_UserDefHashTableCreate
 *	- np_config_all_phys ()
 *	- npu_BfdInit()
 *	- WP_PriorityMsgFlowSet
 *	- system ()
 *	- npu_enable_ufe_serdes_interrupt ()
 */
WP_S32 np_init(void)
{
    WP_handle status = WP_OK;
    WP_U32 i;
    WP_U32  ufe_port;
    /*add bport handle to global sys info */
    NP_SYSTEM_INFO * the_system = np_get_system_info();
    STRU_SET_INITAIL_CONFIG *initial_cfg = npu_get_initial_cfg_info();
    //initial_cfg->OpterateMode = NPU_MODE_IP;
    Fprintf("spp np WP_DriverInit...\n");
    status = WP_DriverInit();
    terminate_on_error(status, "WP_DriverInit");
    Fprintf("spp np boardconfigure...\n");
    ufe_port = WTUFE_UPI_PORT_1;
	status = WPX_PortModeSet(0, WPX_PORT_MODE_STM1, 3);
    terminate_on_error(status, "WPX_PortModeSet ETH");
	status = WPX_BoardConfigure(0, WPX_CONFIGURE_XS_3200);
	terminate_on_error(status, "WPX_BoardConfigure() UFE4");


    Fprintf("spp np initial ...\n");
    for(i=0;i<5000000;i++);
    //status = WPX_BoardSerdesInit(0, WP_PORT_ENET3, WPX_SERDES_INTERNAL_CLOCK);/*WPX_SERDES_INTERNAL_CLOCK|WPX_SERDES_LOOPBACK*/
    //terminate_on_error(status, "WPX_BoardSerdesInit()");
    status = WPX_BoardSerdesInit(0, WP_PORT_ENET4, WPX_SERDES_INTERNAL_CLOCK);
    terminate_on_error(status, "WPX_BoardSerdesInit()");
#if ENEA_RELEASE
    status = WPX_BoardSerdesInit(0, WP_PORT_ENET5, WPX_SERDES_INTERNAL_CLOCK);
    terminate_on_error(status, "WPX_BoardSerdesInit()");
#else/*for ENEA debug*/
    status = WPX_BoardSerdesInit(0, WP_PORT_ENET5, WPX_SERDES_INTERNAL_CLOCK|WPX_SERDES_LOOPBACK);
    terminate_on_error(status, "WPX_BoardSerdesInit()");
#endif

	Fprintf("spp np sysinit...\n");
	status = WP_SysInit(WP_WINPATH(0), context);
	terminate_on_error(status, "WP_SysInit");
    init_total_meminfo();


    //if(initial_cfg->OpterateMode == NPU_MODE_FR)
    {
		Fprintf("spp np ufe4 initial...\n");
		WUFE_init_config		config;
		memset(&config, 0, sizeof(WUFE_init_config));
		config.ufe_id = 0;

		WPX_Ufe4HwReset(0, 0);

		status = WUFE_UfeInitializeFirmware(&config);
		terminate_on_error(status, "WUFE_UfeInitializeFirmware()");

		status = WPX_FpgaFirmwareInitializationStatus(0);
		terminate_on_error(status, "WPX_FpgaFirmwareInitializationStatus");

	}
    /* Create the Buffer pools - interworking system 2 */
    pool_2048 = WP_PoolCreate(WP_WINPATH(0), WP_pool_iwbuffer, buffer_data_2048);
    terminate_on_error(pool_2048, "WP_PoolCreate() pool_2048");

    /* Create an interworking queue node */
    qn_iw->adjunct_pool = pool_2048;
    qn_iw->num_buffers = 10000;
    qnode_iw = WP_QNodeCreate(0, WP_QNODE_IWQ | WP_QNODE_OPT_DEDICATED_RX_HWQ | WP_QNODE_OPT_FMU, qn_iw);
    terminate_on_error(qnode_iw, "WP_QNodeCreate IWQ with ENET param");

#if 0//IP_FRAG
    qn_iw->adjunct_pool = pool_2048;
    qn_iw->num_buffers = 1000;
    qniw_frag = WP_QNodeCreate(WP_WINPATH(0), WP_QNODE_IWQ, qn_iw);
    terminate_on_error(qniw_frag, "WP_QNodeCreate");
#endif

#if(IP_DEFRAG)

	WP_qnode_iwq_mdb qn_iw_mdb_cfg[] =
        {
            {
                /* interruptqueue; */ 0,
                /* num_of_adj_pools; */ 0,
                /* num_buffers[WP_QNODE_IW_MAX_BUFFER_POOLS]; */ {0},
                /* adjunct_pool[WP_QNODE_IW_MAX_BUFFER_POOLS]; */ {0}
            }
        };
    pool_defrag = WP_PoolCreate(WP_WINPATH(0), WP_pool_iwbuffer, buffer_data_defrag);
    terminate_on_error(pool_defrag, "WP_PoolCreate() pool_defrag");

    qn_iw_mdb_cfg->num_buffers[0] = 1000;
    qn_iw_mdb_cfg->num_of_adj_pools = 1;
    qn_iw_mdb_cfg->adjunct_pool[0] = pool_defrag;


    //qn_iw_defrag->adjunct_pool = pool_defrag;
    qniw_defrag = WP_QNodeCreate(WP_WINPATH(0), WP_QNODE_IW_MDB|WP_QNODE_OPT_FMU, qn_iw_mdb_cfg);
    terminate_on_error(qniw_defrag, "WP_QNodeCreate");
#endif

#if IP_FRAG
    pool_frag = WP_PoolCreate(WP_WINPATH(0), WP_pool_iwbuffer, buffer_data_frag);
    terminate_on_error(pool_frag, "WP_PoolCreate() pool_frag");

    qn_frag_iw->adjunct_pool = pool_frag;
    qniw_frag = WP_QNodeCreate(WP_WINPATH(0), WP_QNODE_IWQ|WP_QNODE_OPT_FMU, qn_frag_iw);
    terminate_on_error(qniw_frag, "WP_QNodeCreate");

#endif

    WP_enet_filters		   enet_filters;
    WP_device_enet_ex       enet_ext;
    enet_filters.rx_ucfilter    = WP_ENET_FILTER_PROCESS;
    enet_filters.rx_bcfilter    = WP_ENET_FILTER_PROCESS;
    enet_filters.rx_mcfilter    = WP_ENET_FILTER_PROCESS;

    enet_ext.duplexmode		   = WP_ENET_FULLDUPLEX;
    enet_ext.rx_maclookup_mode  = WP_DISABLE;
    enet_ext.rx_flowcontrol	   = WP_ENET_FLOW_ENABLE;
    enet_ext.tx_flowcontrol	   = WP_ENET_FLOW_ENABLE;
    enet_ext.rx_addrmode 	   = WP_ENET_ADDR_ACCEPTALL;
    enet_ext.phystatmode 	   = WP_ENET_STAT_ENABLE;
    enet_ext.tx_statmode 	   = WP_ENET_STAT_ENABLE;
    enet_ext.rx_statmode 	   = WP_ENET_STAT_ENABLE;
    enet_ext.tx_duplicate	   = WP_DISABLE;
    enet_ext.rx_filters		   = &enet_filters;

    gbep1 = WP_PortCreate(WP_WINPATH(0),WP_PORT_ENET4,gbe_s);
    terminate_on_error(gbep1, "WP_PortCreate() ENET1");
    gbe_dev[0].extended_params = NULL;
    gbedev1 = WP_DeviceCreate(gbep1, 0, WP_DEVICE_ENET, &gbe_dev);
    terminate_on_error(gbedev1, "WP_DeviceCreate() ENET1 ");
    gbe_dev[0].extended_params = &enet_ext;
	status = WP_DeviceModify(gbedev1,
                             WP_DEV_MOD_ENET_PHYSTATMODE |
                             WP_DEV_MOD_ENET_TX_STATMODE |
                             WP_DEV_MOD_ENET_RX_STATMODE,
                             gbe_dev);
	terminate_on_error(status, "WP_DeviceModify gbedev1");

    gbep2 = WP_PortCreate(WP_WINPATH(0), WP_PORT_ENET5, gbe_s);
    terminate_on_error(gbep2, "WP_PortCreate() ENET2");

    gbe_dev[0].extended_params = NULL;
    gbedev2 = WP_DeviceCreate(gbep2, 0,WP_DEVICE_ENET, &gbe_dev);
    terminate_on_error(gbedev2, "WP_DeviceCreate() ENET2");

    gbe_dev[0].extended_params = &enet_ext;
	status = WP_DeviceModify(gbedev2,
                             WP_DEV_MOD_ENET_PHYSTATMODE |
                             WP_DEV_MOD_ENET_TX_STATMODE |
                             WP_DEV_MOD_ENET_RX_STATMODE,
                             gbe_dev);
	terminate_on_error(status, "WP_DeviceModify gbedev2");


    /* Create an IW Host port */
    iwp_host = WP_PortCreate(WP_WINPATH(0), WP_PORT_IW_HOST, NULL);
    terminate_on_error(iwp_host, "WP_PortCreate() IW Host");

    /* Create an IW Host Device */
    iw_host_dev = WP_DeviceCreate(iwp_host, 0, WP_DEVICE_IW_HOST, NULL);
    terminate_on_error(iw_host_dev, "WP_DeviceCreate() IW Host");

#if (IP_FRAG || IP_DEFRAG)
    /* IP Fragmentation */
    ip_frag_system_handle = WP_IwIpFragmentationSystemCreate(0,ip_frag_sys);
    terminate_on_error(ip_frag_system_handle, "WP_IwIpFragmentationSystemCreate()");
#if IP_DEFRAG
    ip_defrag_context_pool = WP_IwIpDefragContextPoolCreate(ip_frag_system_handle,context_config);
    terminate_on_error(ip_defrag_context_pool, "WP_IwIpDefragContextPoolCreate");
    ip_defrag_hash_pool = WP_IwIpDefragHashPoolCreate(ip_frag_system_handle,hash_config);
    terminate_on_error(ip_defrag_hash_pool, "WP_IwIpDefragHashPoolCreate");
#endif

#endif

//if(initial_cfg->OpterateMode == NPU_MODE_FR)
    {
#if START_SCHEDULER
        /*****************************************************************************
         *  Scheduler Creation -	  WDDI call to create the Scheduler based on the
         *						  defined earlier in the application
         * THIS IS NOT NEEDED by the application.  It is here to show if the DPS
         * if functioning by showing threads utilized in the debug utilities.
         * This can be used to verify that the dps is actually running in case
         * there is no tx/rx of enet packets.
         *****************************************************************************/

        status = WP_SysSchedulerCreate(WP_WINPATH(0), calendar);
        terminate_on_error(status, "WP_SysSchedulerCreate");

        WP_feature_psu feature_psu_config =
            {
                /* max_ppr_plus_channels */ 0,
                /* max_cir_eir_channels */	N_UFE_MAX_PHYS,
                /* atm_psu_tx_maxsdu */    0,
                /* atm_psu_characteristics*/0
            };
        status = WP_FeatureInit(WP_WINPATH(0), WP_FEATURE_PSU, &feature_psu_config);
        terminate_on_error(status, "WP_FeatureInit-PSU");

#endif

    }


    /* Register Tx and Rx event call back */
    //status = WP_ControlRegister(WP_EVENT_TX_INDICATE, &AppTxEvent);
    //terminate_on_error(status, "WP_ControlRegister() TxEvent");

    status = WP_ControlRegister(WP_EVENT_RX_INDICATE, &AppRxEvent);
    terminate_on_error(status, "WP_ControlRegister() RxEvent");
#if USE_WDDI_LOG
    status = WP_ControlRegister(WP_DEBUG_CALLBACK_WDDI_LOG, sppwddiLog);
    terminate_on_error(status, "WP_ControlRegister() wddiLog");
#endif

    //if(initial_cfg->OpterateMode == NPU_MODE_FR)
    {
        /*initial qnode for fr system*/
        np_initial_fr_qnode(ufe_port);

    }
#if BFD_OVERETH
    /*add for BFD*/
    npu_BfdModuleInit();
#endif
	status = WP_SysCommit();
	terminate_on_error(status, "WP_SysCommit");
    //if(initial_cfg->OpterateMode == NPU_MODE_FR)
    {
	    /*Enable the UFE EMPHY Port ,must before ufe system create*/
	    extern WP_handle upi_ufe_port;
	    status = WP_PortEnable(upi_ufe_port, WP_DIRECTION_DUPLEX);
	    terminate_on_error(status, "WP_PortEnable()");
    }

    /* Create Iw Rx channels */
    iw_rx_host_handle = WP_ChannelCreate(RX_ETH_INT_TAG, iw_host_dev, qnode_iw, WP_CH_RX,WP_IW_HOST, &ch_config_iw[0]);
    terminate_on_error(iw_rx_host_handle, "WP_Channel_Create() IW Host");

    /* Create Iw Rx channels */
    the_system->Channel_IP_PrioRx0_host_handle = WP_ChannelCreate(RX_ETH_INT_PRIO_MSG_TAG, iw_host_dev, qnode_iw, WP_CH_RX,WP_IW_HOST, &ch_config_iw[0]);
    terminate_on_error(the_system->Channel_IP_PrioRx0_host_handle, "WP_Channel_Create() IP_PrioRx0 IW Host");

    /* create Enet Rx channel */
    rx_enet_handle_1 = WP_ChannelCreate(2, gbedev1, qnode_iw, WP_CH_RX, WP_ENET, &ch_enet[0]);
    terminate_on_error(rx_enet_handle_1, "WP_ChannelCreate() Enet RX 1");

    /* create Enet Tx channel */
    tx_enet_handle_1 = WP_ChannelCreate(3, gbedev1, qnode_iw, WP_CH_TX, WP_ENET, &ch_enet[0]);
    terminate_on_error(tx_enet_handle_1, "WP_ChannelCreate() Enet TX 1");

    /* create Enet Rx channel */
    rx_enet_handle_2= WP_ChannelCreate(4, gbedev2, qnode_iw, WP_CH_RX, WP_ENET, &ch_enet[0]);
    terminate_on_error(rx_enet_handle_2, "WP_ChannelCreate() Enet RX 2");

    /* create Enet Tx channel */
    tx_enet_handle_2 = WP_ChannelCreate(5, gbedev2, qnode_iw, WP_CH_TX, WP_ENET, &ch_enet[0]);
    terminate_on_error(tx_enet_handle_2, "WP_ChannelCreate() Enet TX 2");


    iw_sys_bridging = WP_IwSystemCreate(0,WP_IW_VLAN_AWARE_BRIDGING_MODE,&iw_sys_one);
    terminate_on_error(iw_sys_bridging, "WP_IwSystemCreate - Bridging");

    tx_agg_enet_default[0].txfunc = iw_rx_host_handle;
    tx_agg_enet_default[0].tag = RX_ETH_INT_TAG;
    the_system->agg_enet_default_host_handle = WP_IwFlowAggregationCreate(WP_WINPATH(0),WP_IW_DIRECTMAP_MODE,&tx_agg_enet_default);
    terminate_on_error (the_system->agg_enet_default_host_handle, "WP_IwFlowAggregationCreate - Default aggregation");

    tx_agg_enet_default[0].txfunc = the_system->Channel_IP_PrioRx0_host_handle;
    tx_agg_enet_default[0].tag = RX_ETH_INT_PRIO_MSG_TAG;
    the_system->agg_enet_PrioRx0_host_handle = WP_IwFlowAggregationCreate(WP_WINPATH(0),WP_IW_DIRECTMAP_MODE,&tx_agg_enet_default);
    terminate_on_error (the_system->agg_enet_PrioRx0_host_handle, "WP_IwFlowAggregationCreate - agg_enet_PrioRx0_host_handle aggregation");

    /*creat iw host tx channel*/
    creat_IwHostTxChannel(the_system->agg_enet_default_host_handle);

    bport_conf[0].flow_agg = the_system->agg_enet_default_host_handle;
    bport_conf[0].input_filters_mask = NPU_FILTER_MASK_ALL;
    bport_conf[0].predefined_ht_mode = WP_IW_ARP_HT_ENABLE;
    iw_enet_1 = WP_IwPortCreate(iw_sys_bridging, bport_conf);
    terminate_on_error(iw_enet_1, "WP_IwPortCreate - Enet1");
    bport_conf[0].predefined_ht_mode = 0;

    the_system->Bport_enet1_handle = iw_enet_1;
    iw_enet_1_tag = bport_conf->tag;
    bport_conf->tag++;

    iw_enet_2 = WP_IwPortCreate(iw_sys_bridging, bport_conf);
    terminate_on_error(iw_enet_2, "WP_IwPortCreate - Enet2");
    iw_enet_2_tag = bport_conf->tag;
    bport_conf->tag++;
#if 1
    bport_conf[0].in_filter_mode = WP_IW_INGRESS_FILTER_ENABLE;
    bport_conf[0].input_filters_mask = NPU_FILTER_MASK_DEFRAG;
    bport_conf[0].vlan_param.vlan_tag = initial_cfg->VlanTag;/*BPORT_DEFAULT_VLAN_TAG*/

#endif

    iw_enet_3 = WP_IwPortCreate(iw_sys_bridging, bport_conf);
    terminate_on_error(iw_enet_3, "WP_IwPortCreate - Enet3");
    bport_conf->tag++;
    /*add for MAC filter*/
    bport_conf[0].in_filter_mode = WP_IW_INGRESS_FILTER_ENABLE;
    bport_conf[0].input_filters_mask = NPU_FILTER_MASK_MAC;
    bport_conf[0].vlan_param.vlan_tag = initial_cfg->VlanTag;

    the_system->Bport_mac_filter_handle= WP_IwPortCreate(iw_sys_bridging, bport_conf);
    terminate_on_error(the_system->Bport_mac_filter_handle, "WP_IwPortCreate - Bport_mac_filter_handle");
    bport_conf->tag++;

    /*add for MAC filter from eth2*/
	bport_conf[0].in_filter_mode = WP_IW_INGRESS_FILTER_ENABLE;
	bport_conf[0].input_filters_mask = NPU_FILTER_MASK_MAC;
	bport_conf[0].vlan_param.vlan_tag = BPORT_DEFAULT_VLAN_TAG;
	the_system->Bport_mac_filter_from_eth2_handle= WP_IwPortCreate(iw_sys_bridging, bport_conf);
	terminate_on_error(the_system->Bport_mac_filter_from_eth2_handle, "WP_IwPortCreate - Bport_mac_filter_from_eth2_handle");
    bport_conf->tag++;

    status = WP_IwTxBindingCreate(tx_enet_handle_1, WP_IW_TX_BINDING, &tx_binding[0]);
    terminate_on_error(status, "WP_IwTxBindingCreate - ENET 1");

    status = WP_IwTxBindingCreate(tx_enet_handle_2, WP_IW_TX_BINDING, &tx_binding[0]);
    terminate_on_error(status, "WP_IwTxBindingCreate - ENET 2");

    /*creat enet tx agg*/
    tx_agg_enet->prefix_length = 0;
    tx_agg_enet->extraction_length =0;

    tx_agg_enet->iw_port = iw_enet_1;
    tx_agg_enet->txfunc = tx_enet_handle_1;
    agg_enet_1 = WP_IwFlowAggregationCreate(0,WP_IW_DIRECTMAP_MODE, tx_agg_enet);
    terminate_on_error(agg_enet_1, "WP_IwFlowAggregationCreate agg_enet_1");

    tx_agg_enet->iw_port = iw_enet_2;
    tx_agg_enet->txfunc = tx_enet_handle_2;
    agg_enet_2 = WP_IwFlowAggregationCreate(0,WP_IW_DIRECTMAP_MODE, tx_agg_enet);
    terminate_on_error(agg_enet_2, "WP_IwFlowAggregationCreate agg_enet_2");

	enet_to_enet_classification_filter_cfg[NPU_FILTER_DST_MAC].filter_type	   = WP_CLASS_FILTER_EMC;
	enet_to_enet_classification_filter_cfg[NPU_FILTER_DST_MAC].no_match_action = WP_CLASS_FILTER_NO_MATCH_DENY;
	enet_to_enet_classification_filter_cfg[NPU_FILTER_DST_MAC].fields_mask	   = WP_FIELD_DST_MAC;

    /* Create the filter */
	enet_to_enet_dfc_flow_info[NPU_FILTER_DST_MAC].filter_handle = WP_IwClassFilterAdd(iw_sys_bridging, &(enet_to_enet_classification_filter_cfg[NPU_FILTER_DST_MAC]));
	terminate_on_error(enet_to_enet_dfc_flow_info[NPU_FILTER_DST_MAC].filter_handle , "WP_IwClassFilterAdd() iw_sys_bridging bfd");
	/*add board mac & boardcast mac*/
	np_dst_mac_filter_flowAdd(the_system);

#if BFD_OVERETH
	enet_to_enet_classification_filter_cfg[NPU_FILTER_DPORT_UDP_BFD].filter_type	   = WP_CLASS_FILTER_EMC;
	enet_to_enet_classification_filter_cfg[NPU_FILTER_DPORT_UDP_BFD].no_match_action = WP_CLASS_FILTER_NO_MATCH_CONTINUE;
	enet_to_enet_classification_filter_cfg[NPU_FILTER_DPORT_UDP_BFD].fields_mask	   = WP_FIELD_DST_PORT | WP_FIELD_PROTOCOL;

	/* Create the filter */
	enet_to_enet_dfc_flow_info[NPU_FILTER_DPORT_UDP_BFD].filter_handle = WP_IwClassFilterAdd(iw_sys_bridging, &(enet_to_enet_classification_filter_cfg[NPU_FILTER_DPORT_UDP_BFD]));
	terminate_on_error(enet_to_enet_dfc_flow_info[NPU_FILTER_DPORT_UDP_BFD].filter_handle , "WP_IwClassFilterAdd() iw_sys_bridging bfd");
#endif


	enet_to_enet_classification_filter_cfg[NPU_FILTER_SIP_UDP_PEER].filter_type	   = WP_CLASS_FILTER_EMC;
	enet_to_enet_classification_filter_cfg[NPU_FILTER_SIP_UDP_PEER].no_match_action = WP_CLASS_FILTER_NO_MATCH_CONTINUE;
	enet_to_enet_classification_filter_cfg[NPU_FILTER_SIP_UDP_PEER].fields_mask	   = WP_FIELD_IP_SA | WP_FIELD_PROTOCOL;

	/* Create the filter */
	enet_to_enet_dfc_flow_info[NPU_FILTER_SIP_UDP_PEER].filter_handle = WP_IwClassFilterAdd(iw_sys_bridging, &(enet_to_enet_classification_filter_cfg[NPU_FILTER_SIP_UDP_PEER]));
	terminate_on_error(enet_to_enet_dfc_flow_info[NPU_FILTER_SIP_UDP_PEER].filter_handle , "WP_IwClassFilterAdd() iw_sys_bridging peer");


    enet_to_enet_classification_filter_cfg[NPU_FILTER_UDP_DEFRAG].filter_type     = WP_CLASS_FILTER_EMC;
    enet_to_enet_classification_filter_cfg[NPU_FILTER_UDP_DEFRAG].no_match_action = WP_CLASS_FILTER_NO_MATCH_CONTINUE;
    enet_to_enet_classification_filter_cfg[NPU_FILTER_UDP_DEFRAG].fields_mask     = WP_FIELD_PROTOCOL;

    /* Create the filter */
    enet_to_enet_dfc_flow_info[NPU_FILTER_UDP_DEFRAG].filter_handle = WP_IwClassFilterAdd(iw_sys_bridging, &(enet_to_enet_classification_filter_cfg[NPU_FILTER_UDP_DEFRAG]));
    terminate_on_error(enet_to_enet_dfc_flow_info[NPU_FILTER_UDP_DEFRAG].filter_handle , "WP_IwClassFilterAdd() iw_sys_bridging ifdefrag");


	/*The filter used for ip-ip upflow */
    enet_to_enet_classification_filter_cfg[NPU_FILTER_DPORT_UDP_IP_UL].filter_type	   = WP_CLASS_FILTER_EMC;
    enet_to_enet_classification_filter_cfg[NPU_FILTER_DPORT_UDP_IP_UL].no_match_action = WP_CLASS_FILTER_NO_MATCH_CONTINUE;
    enet_to_enet_classification_filter_cfg[NPU_FILTER_DPORT_UDP_IP_UL].fields_mask	   = WP_FIELD_DST_PORT | WP_FIELD_PROTOCOL;

    /* Create the filter */
    enet_to_enet_dfc_flow_info[NPU_FILTER_DPORT_UDP_IP_UL].filter_handle = WP_IwClassFilterAdd(iw_sys_bridging, &(enet_to_enet_classification_filter_cfg[NPU_FILTER_DPORT_UDP_IP_UL]));
    terminate_on_error(enet_to_enet_dfc_flow_info[NPU_FILTER_DPORT_UDP_IP_UL].filter_handle , "WP_IwClassFilterAdd() iw_sys_bridging ip ul");

    enet_to_enet_classification_filter_cfg[NPU_FILTER_DPORT_UDP_FMC].filter_type     = WP_CLASS_FILTER_MFC;
    enet_to_enet_classification_filter_cfg[NPU_FILTER_DPORT_UDP_FMC].no_match_action = WP_CLASS_FILTER_NO_MATCH_ACCEPT;
    enet_to_enet_classification_filter_cfg[NPU_FILTER_DPORT_UDP_FMC].fields_mask     =  WP_FIELD_DST_PORT | WP_FIELD_PROTOCOL;

    /* Create the filter */
    enet_to_enet_dfc_flow_info[NPU_FILTER_DPORT_UDP_FMC].filter_handle = WP_IwClassFilterAdd(iw_sys_bridging, &(enet_to_enet_classification_filter_cfg[NPU_FILTER_DPORT_UDP_FMC]));
    terminate_on_error(enet_to_enet_dfc_flow_info[NPU_FILTER_DPORT_UDP_FMC].filter_handle , "WP_IwClassFilterAdd() iw_sys_bridging fmc");

    WP_CHAR mfc_rule_string[30]={0};

    //App_EnetRxBindingCreate();
    rx_binding_enet[0].input_port = iw_enet_1;
    status = WP_IwRxBindingCreate(rx_enet_handle_1,iw_sys_bridging,qnode_iw,&rx_binding_enet[0]);
    terminate_on_error(status, "WP_IwRxBindingCreate - Enet1");

    rx_binding_enet[0].input_port = iw_enet_2;
    status = WP_IwRxBindingCreate(rx_enet_handle_2,iw_sys_bridging,qnode_iw,&rx_binding_enet[0]);
    terminate_on_error(status, "WP_IwRxBindingCreate - Enet2");

    status = WP_IwSystemBuild(iw_sys_bridging);
    terminate_on_error(status, "WP_IwSystemBuild - BasicRules");

    sprintf (mfc_rule_string, "n;n;n;n;n;n;*;*;");
    enet_to_enet_flow_policy_cfg[NPU_FILTER_DPORT_UDP_FMC].classifier_string = mfc_rule_string;
    enet_to_enet_flow_policy_cfg[NPU_FILTER_DPORT_UDP_FMC].action_info.forwarding_action.flow_aggregation = the_system->agg_enet_default_host_handle;
    enet_to_enet_flow_policy_cfg[NPU_FILTER_DPORT_UDP_FMC].dfc_info = &enet_to_enet_dfc_flow_info[NPU_FILTER_DPORT_UDP_FMC];
    enet_to_enet_flow_policy_cfg[NPU_FILTER_DPORT_UDP_FMC].match_action = WP_FLOW_CLASS_MODE_FLOW_AGG;
    the_system->flow_default_mfc = WP_IwFlowAdd(iw_sys_bridging, WP_FLOW_CLASS_RULE,&enet_to_enet_flow_policy_cfg[NPU_FILTER_DPORT_UDP_FMC]);
    terminate_on_error(the_system->flow_default_mfc, "WP_IwFlowAdd() flow_default_mfc");
    status = WP_IwSystemBuild(iw_sys_bridging);
    terminate_on_error(status, "WP_IwSystemBuild - BasicRules");


    //if(initial_cfg->OpterateMode == NPU_MODE_FR)
    {
        Fprintf("np fr_system_init start \n");
        (void)np_frIwSystemCreate(&np_system);
	    fr_system_init(ufe_port);
    }

    /*create lookup table for dps*/
    status = WP_LookupCreate(0, 3200, sizeof(wp_globle_rt));
    terminate_on_error(status, "WP_LookupCreate()");

	status = WP_UserDefHashTableCreate(0,0x10000);/*size must align to 16 byte*/
	//status = WP_UserDefHashTableCreate(0,0x10);/*size must align to 16 byte*/
	if(status != 0)
	{
	    Fprintf("WP_UserDefHashTableCreate fail status = %d\n",status);
        return -1;
	}
#if 0 /*TLLI not found &coredump issue test*/
	WP_U16 result=0xffff;
	int key1,key2;int hsdata= 0x03;
	for(key1 = 0;key1<16;key1++)
		for(key2= 0;key2<3;key2++)
        {
            WPI_UserDefHashEntryInsert (0,
                                        key1,key2,hsdata++);
        }
    Fprintf("hash result\n");
	for(key1 = 0;key1<16;key1++)
		for(key2= 0;key2<3;key2++)
		{
			WPI_UserDefHashEntryDisplay(0,key1,key2,&result);
			Fprintf("hash result = 0x%x\n",result);
		}
	exit(1);
#endif
#if IP_DEFRAG
    WP_iw_mbc_feature_init feature_mbc_config;
    feature_mbc_config.qniw_reassemble = qnode_iw;/*qnode_iw*/
    feature_mbc_config.qniw_mbc = qniw_frag;
    status = WP_FeatureInit(WP_WINPATH(0), WP_FEATURE_MBC, &feature_mbc_config);
    terminate_on_error(status, "WP_FeatureInit-WP_FEATURE_MBC");

    tx_agg_enet->iw_port = iw_enet_3;
    tx_agg_enet->txfunc = iw_sys_bridging;
    the_system->agg_after_defrag = WP_IwFlowAggregationCreate(0,WP_IW_DIRECTMAP_MODE, tx_agg_enet);
    terminate_on_error(the_system->agg_after_defrag, "WP_IwFlowAggregationCreate the_system->agg_bridge_to_route");

    np_ipdefrag_flow_add();
    Fprintf("IP defragment create success!\n");
#endif
    np_send2eth_route_system_create();
    np_enet2_to_enet1_rule_add();

/*create all phys on STM1s*/
    if(initial_cfg->OpterateMode == NPU_MODE_FR)
    {
#if START_SCHEDULER
        status = WP_SysPsuEnable(0);
        terminate_on_error(status, "WP_SysPsuEnable()");
#endif

        np_config_all_phys();
    }
#if BFD_OVERETH
/*ADD for bfd*/
    npu_BfdInit();
#endif

#if NPU_PRIORITY_MSG
    wp_priority_msg flowTable;
    memset(&flowTable,0,sizeof(flowTable));
    flowTable.flow_num[0] = the_system->agg_enet_PrioRx0_host_handle;
    (void)WP_PriorityMsgFlowSet(0,WP_PRIORIYT_MSG_IP_TYPE,&flowTable);
#endif
    np_create_arp_hq();

/*enable Ethernet channel at end */
    status = WP_PortEnable(gbep1, WP_DIRECTION_DUPLEX);
    terminate_on_error(status, "WP_PortEnable ENET 1");

    status = WP_DeviceEnable(gbedev1, WP_DIRECTION_DUPLEX);
    terminate_on_error(status, "WP_DeviceEnable - ENET 1");
/*reset serdes*/
    *(volatile unsigned int *)(unsigned long)(0x1e0082c4 + WPL_RIF_VIRTUAL(0, 0)) = htonl(0x01618000);
    usleep(1);
    *(volatile unsigned int *)(unsigned long)(0x1e0082c4 + WPL_RIF_VIRTUAL(0, 0)) = htonl(0x05618000);


    status = WP_PortEnable(gbep2, WP_DIRECTION_DUPLEX);
    terminate_on_error(status, "WP_PortEnable ENET 2");

    status = WP_DeviceEnable(gbedev2, WP_DIRECTION_DUPLEX);
    terminate_on_error(status, "WP_DeviceEnable - ENET 2");
/*reset serdes*/
    *(volatile unsigned int *)(unsigned long)(0x1e0082c0 + WPL_RIF_VIRTUAL(0, 0)) = htonl(0x01618000);
    usleep(1);
    *(volatile unsigned int *)(unsigned long)(0x1e0082c0 + WPL_RIF_VIRTUAL(0, 0)) = htonl(0x05618000);


    status = WP_ChannelEnable(tx_enet_handle_1);
    terminate_on_error(status, "WP_ChannelEnable - ENET 1 Tx");

    status = WP_ChannelEnable(rx_enet_handle_1);
    terminate_on_error(status, "WP_ChannelEnable - ENET 1 Rx");

    status = WP_ChannelEnable(tx_enet_handle_2);
    terminate_on_error(status, "WP_ChannelEnable - ENET 2 Tx");

    status = WP_ChannelEnable(rx_enet_handle_2);
    terminate_on_error(status, "WP_ChannelEnable - ENET 2 Rx");

    g_npinitialFlag = NPU_INITALIZED;
    char strconfig[200];
    extern char *workingDir;

    sprintf(strconfig,"%s/winutil tb init",workingDir);

    Fprintf("%s\n",strconfig);
    if ((status = system(strconfig)) < 0)
        WDDI_ERR("systemwinutil tb init");

    npu_enable_ufe_serdes_interrupt();
    Fprintf("spp np initial complete\n");
    return 0;

}


void terminate_on_error(WP_handle handle, WP_CHAR *s)
{
    if (WP_ERROR_P(handle)) {
        Fprintf("%s: Terminal error (handle 0x%x) %s\n", appname, handle, s);
        Fprintf("%s: Error type - %s\n", appname, WP_error_name[WP_ERROR(handle)]);
        WP_DriverRelease();
        exit(1);
    }
}


void creat_IwHostTxChannel(WP_handle flow_agg)
{
    WP_handle bridge_port;

    WP_ch_iw_tx ch_config_iw_tx[1] =
        {
            {
                /* iw_system */                 0,
                /* reserved */                  0,
                /* group_id */                  WP_IW_CH_GROUP_ID_NULL,
                /* bridging_group_tag */        0,
                /* input_port */                0
            }
        };

    WP_bridge_port bport_conf[1] =
        {
            {
                /* tag */ 0,
                /* direct_mapping*/WP_IW_DIRECT_MAP_DISABLE,
                /* flow_agg */0,
                /* termination_mode*/ /*WP_IW_L2_TERM_MODE*/WP_IW_HOST_TERM_MODE,
                /* learning_mode */WP_IW_LEARNING_DISABLE,
                /* in_filter_mode */WP_IW_INGRESS_FILTER_DISABLE,
                /* vlan_param */
                {
                    /* vlan_acceptance_mode */WP_IW_ACCEPT_TAGGED_UNTAGGED,
                    /* vlan_tag */0xa,
                },
                /* max_mac_addresses */ 2000,
                /* group_tag */ 0,
                /* group_filtering_mode*/WP_IW_GROUP_FILTER_DISABLE,
                /* unk_mac_sa_filter*/0,
                /* unk_mc_mode*/WP_IW_UNK_MC_SR,
                /* bc_ht_mode*/WP_IW_BC_HT_DISABLE,
                /*input_filters_mask*/ 0,
                /*output_filters_mask*/ 0,
                /*statmode*/ WP_IW_PORT_STAT_ENABLE
            }
        };


    bport_conf->tag = 0xfffe;
    bport_conf->flow_agg = flow_agg;

    bridge_port = WP_IwPortCreate(iw_sys_bridging,bport_conf);
    terminate_on_error( bridge_port,"IwHostTxChannel() WP_IwPortCreate");

    /*add bport handle to global sys info */
    NP_SYSTEM_INFO * the_system = np_get_system_info();
    the_system->Bport_enet2host_handle = bridge_port;

    ch_config_iw_tx->input_port = bridge_port;
    ch_config_iw_tx->iw_system = iw_sys_bridging;
    iw_tx_host_handle = WP_ChannelCreate( 0x123456, iw_host_dev, qnode_iw, WP_CH_TX,
                                          WP_IW_HOST, &ch_config_iw_tx[0]);
    terminate_on_error( iw_tx_host_handle, "WP_ChannelCreate() iw host Tx channel");

}

static WP_data_unit data_unit;
static WP_data_segment tx_buf;
static WP_U8 *data_ptr;

void np_eth_host_send(WP_S32 DataLength, WP_CHAR* pTxData)
{
    WP_status status;
    WP_data_segment* curr_buf;

    if(DataLength > 1536)
    {
        WDDI_WARNING("np_eth_host_send fail	DataLength too big is %d \n",DataLength);
        return;
    }
    //WP_Delay(30);
    data_unit.data_size = DataLength;
    data_unit.n_active = 1;

    tx_buf.data_size = data_unit.data_size;
    data_ptr = WP_PoolAlloc(pool_2048);
    //printf("data_ptr = 0x%x\n",data_ptr);
    if(data_ptr == NULL)
    {
        WDDI_WARNING("np_eth_host_send fail	WP_PoolAlloc is NULL \n");
        return;
    }
    tx_buf.data = data_ptr;
    tx_buf.pool_handle = pool_2048;
    tx_buf.displacement = 0;
    tx_buf.user_info = 0;
    tx_buf.next = NULL;
    tx_buf.buffer_class = 0;

    data_unit.control =
        WP_HT_IW_B_MODE_FIELD(WP_HT_IW_B_FAST_MODE)|
        WP_HT_IW_B_VA_DEST_TYPE_FIELD(WP_HT_IW_B_VA_DESTINATION_FLOW_AGG)|
        WP_HT_IW_FSP_FLOW_AGG_FIELD(agg_enet_1) |
        WP_HT_IW_CRC_FIELD(0);
    data_unit.segment = (&tx_buf);
    data_unit.n_segments = 1;
    data_unit.n_active = 1;
    data_unit.type = WP_DATA_IW;
    data_unit.data_return = 0;
    data_unit.status = 0;

    //WP_MEM_BYTES_FILL(data_ptr, 0, tx_buf.data_size);
#if NPU_HOST_OLP
    pcimem_copy(data_ptr, pTxData, DataLength);
#else
    WP_MEM_BYTES_SET(data_ptr, pTxData, DataLength);
#endif

    WDDI_PRINT("Info: np_eth_host_send, Send packet:0x%x, len:%d\n", (int)pTxData, DataLength);
    curr_buf = data_unit.segment;

    WDDI_STRING(curr_buf->data, curr_buf->data_size);
    status = WP_HostSend(iw_tx_host_handle, &data_unit);
    //terminate_on_error(status, "ETH WP_HostSend()");

    if(WP_ERROR_P(status))
    {
        if (WP_ERROR(status) == WP_ERR_WMM_FIFO_PUT)
        {
            WDDI_WARNING("np_eth_host_send fail \n");
        }
        else
        {
            WP_PoolFree(curr_buf->pool_handle, curr_buf->data);
        }
        WDDI_WARNING("np_eth_host_send fail  error code is %d \n",WP_ERROR(status));
        //WPI_Display(0,0,WP_DISPLAY_POOL);
        //fflush(stdout);
    }
}


int np_eth_host_receive(WP_handle rx_host_handle,WP_S32 *DataLength, WP_CHAR* pTxData)
{
    WP_status status;
    WP_data_unit du={0};
    WP_data_segment seg={0};
    WP_data_segment* curr_buf;
    WP_U32 j;
    du.segment = &seg;
    du.type = WP_DATA_IW;
    du.n_segments = 1;

    if (WP_OK != (status = WP_HostReceive(rx_host_handle, &du)))
    {
        if ( strcmp("WP_ERR_HST_NO_DATA_TO_GET",  WP_error_name[WP_ERROR(status)]) != 0)
            return -1;
        //terminate_on_error(status,"WP_HostReceive error");
    }
    else {
        curr_buf = du.segment;
        for (j = 0; j < du.n_active; j++, curr_buf++)
        {
#if 0
            for (k = 0; k < curr_buf->data_size; k++)
            {
                pTxData[k]=curr_buf->data[k];
            }
#endif
#if NPU_HOST_OLP
            pcimem_copy(pTxData,curr_buf->data,curr_buf->data_size);
#else
            memcpy(pTxData,curr_buf->data,curr_buf->data_size);/**/
#endif

            WDDI_STRING(pTxData, curr_buf->data_size);
            *DataLength = curr_buf->data_size;
            if(*DataLength >1536)
            {
                WDDI_WARNING("np_eth_host_receive a big packet\n");
                *DataLength = 1536;
            }
            WDDI_PRINT("Info: np_eth_host_receive, pTxData:0x%x, DataLength:%d\n", (int)pTxData, DataLength);
            //DEBUG("(%d bytes) \n", curr_buf->data_size);
            WP_PoolFree(curr_buf->pool_handle, curr_buf->data);
        }
        //*DataLength = u16DataIndex;
    }
    return status;
}



WP_iw_agg_directmap_enet enet_iw_agg_cfg[1] =
{
    {
        /* tag */                  0,
        /* txfunc */               0,
        /* rfcs */                 WP_IW_RFCS_ENABLE,/*should be enabled*/
        /* l2header_insert_mode;*/ WP_IW_L2H_INSERT_ENABLE,
        /* interruptqueue;*/       WP_IW_IRQT1,
        /* error_pkt_mode*/        WP_IW_ERRPKT_DISCARD,
        /* intmode;*/              WP_IW_INT_DISABLE,
#if STATISTICS_ENABLE
        /* statmode;*/             WP_IW_STAT_ENABLE,
#else
        /* statmode;*/             WP_IW_STAT_DISABLE,
#endif
        /* timestamp_mode;*/       WP_IW_TIME_STAMP_DISABLE,
        /* ov_pool_mode*/          0,
        /* fbp_drop_threshold;*/   0,
        /* mtu;*/                  1536,
        /* prefix_length */        /*34*/ 6,/*FR header+NSheader*/
        /* extraction_length */    50,
        /* prefix_header[68];*/    {
            0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x01,
            0x02, 0x03, 0x04, 0x05, 0x81, 0x00, 0x00, 0x85,
            0x08, 0x00, 0x45, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x40, 0x00, 0x40, 0x11, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00
        },
        /* policer_enable */       0,
        /* policer_config */       NULL,
        /* cong_mode */            0,
        /* cong_threshold_param */ NULL,
        /*WP_U8 dynamic_length_update;*/ WP_IW_LENGTH_UPDATE_L3,
        /*WP_U8 length_offset;*/ 18,
    }
};


WP_iw_agg_directmap_enet enet_iw_agg_defrag_cfg[1] =
{
    {
        /* tag */                  0,
        /* txfunc */               0,
        /* rfcs */                 WP_IW_RFCS_DISABLE,/*don't remove FCS,the follow fileter agg will remove*/
        /* l2header_insert_mode;*/ WP_IW_L2H_INSERT_DISABLE,
        /* interruptqueue;*/       WP_IW_IRQT1,
        /* error_pkt_mode*/        WP_IW_ERRPKT_DISCARD,
        /* intmode;*/              WP_IW_INT_DISABLE,
#if STATISTICS_ENABLE
        /* statmode;*/             WP_IW_STAT_ENABLE,
#else
        /* statmode;*/             WP_IW_STAT_DISABLE,
#endif
        /* timestamp_mode;*/       WP_IW_TIME_STAMP_DISABLE,
        /* ov_pool_mode*/          0,
        /* fbp_drop_threshold;*/   0,
        /* mtu;*/                  1536,
        /* prefix_length */        /*34*/ 0,/*FR header+NSheader*/
        /* extraction_length */    18/*50*/,
        /* prefix_header[68];*/    {0},
        /* policer_enable */       0,
        /* policer_config */       NULL,
        /* cong_mode */            0,
        /* cong_threshold_param */ NULL,
        /* dynamic_length_update*/WP_IW_LENGTH_UPDATE_DISABLED,
        /* length_offset*/         0
    }
};


DportGrpaggInfo DportGrpagg_handle[65535] = {0};
WP_handle DportGrpflow_handle[65535] = {0};

int np_dl_route_table_add(STRU_SET_DOWNLINK_PATH_RULE * rule)
{
	WP_iw_agg_directmap iw_agg_cfg [1] = {

        {
            /*WP_tag tag;*/ 0,
            /*WP_handle txfunc;*/   0,
            /*WP_U8 rfcs;*/ WP_IW_RFCS_DISABLE,
            /*WP_U8 l2_header_insert_mode;*/ /* 10*/ WP_IW_L2H_INSERT_ENABLE,
            /*WP_U8 interruptqueue;*/ WP_IW_L2H_INSERT_DISABLE,
            /*WP_U8 error_pkt_mode;*/ WP_IW_ERRPKT_DISCARD,
            /*WP_U8 intmode;*/ WP_IW_INT_DISABLE,
            /*WP_U8 statmode;*/ WP_IW_STAT_ENABLE,
            /*WP_U8 timestamp_mode;*/ WP_IW_TIME_STAMP_DISABLE,
            /*WP_U8 ov_pool_mode;*/ WP_IW_OV_POOL_DISABLE,
            /*WP_U16 fbp_drop_threshold;*/ 0,
            /*WP_U16 mtu; */ /*20 */ 1536,
            /*WP_U8 prefix_length;*/ 46,
            /*WP_U8 extraction_length;*/ 46,
            /*WP_U8 prefix_header[68];*/
            {
                /*0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                  0x00, 0x00,*/ 0x45, 0x00, 0x00, 0x00, 0x00, 0x00,
                  0x40, 0x00, 0x40, 0x11, 0x00, 0x00, 0x00, 0x00, /*ip frage flag 0x40*/
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                  0x00, 0x00, 0x00, 0x00
            },
            /*WP_U8 policer_enable;*/ WP_IW_POLICER_DISABLE,
            /*WP_policer_config	 *policer_config  */ /*96*/ NULL,
            /*WP_U8 cong_mode;*/ WP_IW_CONGESTION_DISABLE,
            /*WP_iw_flow_agg_cong_thresh_param *cong_threshold_param;*/ NULL,
            /*WP_U8 dynamic_length_update;*/ WP_IW_LENGTH_UPDATE_L3,
            /*WP_U8 length_offset;*/ 0,
            /*WP_flowaggtype    flow_agg_type;*/ WP_IW_FLOW_AGG_PRIMARY,
            /*WP_handle comp_group_handle;*/ 0
        }
	};
    WP_S32 status;
    WP_handle flow_handle;
	NP_SYSTEM_INFO * the_system = np_get_system_info();
	STRU_SET_INITAIL_CONFIG *initial_cfg = npu_get_initial_cfg_info();
	STRU_VPORT_INFO  *VportInfo;
	STRU_SET_DOWNLINK_PATH_LINK_TABLE * link_table;
    WP_handle enet_iw_flow_aggregation;
    WP_CHAR enet_rule_string[128]={0};
    NP_DL_FLOW_INFO flow_info = {0};
	//WP_handle ip_frag_group;
	//struct vlan_ethhdr * vlan_ethheader;
	struct iphdr *ipheader;
	struct udphdr *udpheader;
	int ret = 0;

	/*Dport has been created*/
	if(DportGrpflow_handle[rule->DportStartValue] == 0)
	{
		/*create AGG for UDP aggregation*/

		enet_iw_agg_cfg->l2_header_insert_mode = WP_IW_L2H_INSERT_DISABLE;
		enet_iw_agg_cfg->txfunc =the_system->agg_enet_default_host_handle;
		if(initial_cfg->OpterateMode == NPU_MODE_IP)
		{
			enet_iw_agg_cfg->mtu = 0xa6a6;

		}
		else
		{
			enet_iw_agg_cfg->mtu = 0xa5a5;

		}
	    enet_iw_agg_cfg->prefix_length = 0;
		enet_iw_agg_cfg->extraction_length = 0;
		enet_iw_flow_aggregation = WP_IwFlowAggregationCreate(0,WP_IW_DIRECTMAP_MODE, enet_iw_agg_cfg);
		terminate_on_error(enet_iw_flow_aggregation, "WP_IwFlowAggregationCreate agg_queue");
	    enet_iw_agg_cfg->l2_header_insert_mode = WP_IW_L2H_INSERT_ENABLE;
		sprintf (enet_rule_string, "n;n;n;n;n;n;[%d-%d];UDP;",htons(rule->DportStartValue),htons(rule->DportStartValue)+rule->PortSize-1);
		enet_to_enet_flow_policy_cfg[NPU_FILTER_DPORT_UDP_FMC].classifier_string = enet_rule_string;
		enet_to_enet_flow_policy_cfg[NPU_FILTER_DPORT_UDP_FMC].action_info.forwarding_action.flow_aggregation = enet_iw_flow_aggregation;
        enet_to_enet_flow_policy_cfg[NPU_FILTER_DPORT_UDP_FMC].dfc_info = &enet_to_enet_dfc_flow_info[NPU_FILTER_DPORT_UDP_FMC];
		enet_to_enet_flow_policy_cfg[NPU_FILTER_DPORT_UDP_FMC].match_action = WP_FLOW_CLASS_MODE_FLOW_AGG;
		flow_handle = WP_IwFlowInsert(iw_sys_bridging,the_system->flow_default_mfc,WP_FLOW_CLASS_RULE,&(enet_to_enet_flow_policy_cfg[NPU_FILTER_DPORT_UDP_FMC]));
		terminate_on_error(flow_handle, "WP_IwFlowInsert() enet_to_enet_iw_sys");
		//status = WP_IwSystemBuild(iw_sys_bridging);
		//terminate_on_error(status, "WP_IwSystemBuild - down link rule");

		DportGrpagg_handle[rule->DportStartValue].handle = enet_iw_flow_aggregation;
		DportGrpagg_handle[rule->DportStartValue].PortSize = rule->PortSize;
		DportGrpflow_handle[rule->DportStartValue] =flow_handle;
		// this indicate we have to build the rules
		ret = 1;
	}
	/*for IP<->IP*/
	if(initial_cfg->OpterateMode == NPU_MODE_IP)
	{

		VportInfo = npu_get_vport_info(rule->nsei_portSrc.portSrc);
		if(VportInfo == NULL)
		{
            WDDI_WARNING("np_dl_route_table_add npu_get_vport_info fail vport= 0x%x",rule->nsei_portSrc.portSrc);
            return ERR_DOWNLINX_FLOW_ADD;
		}
		else if(VportInfo->UsedFlag == UNUSED)
		{
            WDDI_WARNING("np_dl_route_table_add npu_get_vport_info fail ,vport closed vport= 0x%x",rule->nsei_portSrc.portSrc);
            return ERR_DOWNLINX_FLOW_ADD;
		}
		/*find dst MAC according dst ip address*/
		NP_ARP_INFO arp = {0};
		/*get dstip from link table ,key= uplink_rule->PortSrc*/
		link_table = npu_get_ul_link_table_info(rule->nsei_portSrc.portSrc);
		if(link_table == NULL)
		{
			WDDI_WARNING("dl link table is not existed ,key PortSrc = 0x%x",rule->nsei_portSrc.portSrc);
			return ERR_DOWNLINX_FLOW_ADD;
		}
		if(link_table->IpEpAcitveNum == 0)
		{
			WDDI_WARNING("dl link table is not existed ,IpEpAcitveNum = 0\n");
			return ERR_DOWNLINX_FLOW_ADD;
		}
        /*update mac address */
		{
			int ret;
			char ArrayBuf[10];
			WP_U32 IpDst = link_table->IpEp[0].IpDst;
			/*select IpEp[0].IpDst to get arp*/
			ret = tap_socket_getarp(IpDst, ArrayBuf);
			if(ret == 0)/*success*/
			{
				arp.Ip = IpDst;
				memcpy(arp.Mac,ArrayBuf,6);
				/*just for display*/
				(void)npu_set_arp_info(IpDst,&arp);
				np_update_mac_in_route_system(&arp);

			}
			else
			{
				WDDI_WARNING("np_dl_route_table_add npu_get_arp_info fail ipdst= 0x%x",IpDst);
				return ERR_DOWNLINX_FLOW_ADD;
			}
		}

		if(VportInfo->agg_dl_ip2iptx2Enet1 == 0)
		{
			/*IP header*/
			ipheader = (struct iphdr *)&iw_agg_cfg->prefix_header[0];

			ipheader->daddr=0; /*modify by dps according loadbalance*/
			ipheader->saddr=htonl(initial_cfg->ifIpaddr);

			udpheader = (struct udphdr *) (ipheader +1);
			udpheader->source = rule->nsei_portSrc.portSrc;/*modify by dps according loadbalance*/
			udpheader->dest = 0;  /*modify by dps according loadbalance*/

			iw_agg_cfg->iw_port = the_system->Rport_enet1_hanlde;
			iw_agg_cfg->txfunc = the_system->system_route_send2eth1_handle ;//the_system->agg_enet_default_host_handle;//tx_enet_handle_1;
			iw_agg_cfg->prefix_length = 28;	/*add ipv4header+udp header*/
			if(initial_cfg->VlanFlag == NPU_VLAN_ENABLE)
			{
			    iw_agg_cfg->extraction_length = 46;	  /*update ipv4header+udp heade*/
			}
			else
			{
			    iw_agg_cfg->extraction_length = 42;	  /*update ipv4header+udp heade*/
			}
			VportInfo->agg_dl_ip2iptx2Enet1 = WP_IwFlowAggregationCreate(0,WP_IW_DIRECTMAP_MODE,iw_agg_cfg);
			terminate_on_error(VportInfo->agg_dl_ip2iptx2Enet1, "WP_IwFlowAggregationCreate agg_dl_ip2iptx2Enet1");
		}
	}
	flow_info.flowHandle = DportGrpflow_handle[rule->DportStartValue];
	flow_info.agg_dl_dportGrp_handle= DportGrpagg_handle[rule->DportStartValue].handle;
	memcpy(&flow_info.rule,rule,sizeof(STRU_SET_DOWNLINK_PATH_RULE));

	wp_dl_rt dl_rt;
	dl_rt.bvci_n = rule->BVCInew;
	if(initial_cfg->OpterateMode == NPU_MODE_FR)
	{
		dl_rt.nsei_i = rule->nsei_portSrc.NSEI;
		dl_rt.flow_num = WPI_HANDLE_INDEX(flow_info.agg_dl_dportGrp_handle);
	}
	else
	{
	    if(the_system->agg_dl_ip_to_enet2_handle == 0)
	    {
			np_dl_ip_forward_to_enet2_create();
		}
		dl_rt.nsei_i = htons(rule->nsei_portSrc.portSrc);
		//dl_rt.flow_num = WPI_HANDLE_INDEX(VportInfo->agg_dl_ip2iptx2Enet1);
		dl_rt.flow_num = (WPI_HANDLE_INDEX(the_system->agg_dl_ip_to_enet2_handle)<<16) | WPI_HANDLE_INDEX(VportInfo->agg_dl_ip2iptx2Enet1);

	}

	WDDI_INFO("Info: np_dl_route_table_add, key1:0x%x, key2:0x%x, NSEI:0x%x, BVCI_new:0x%x\n",WPI_HANDLE_INDEX(flow_info.agg_dl_dportGrp_handle),rule->BVCI,dl_rt.nsei_i,dl_rt.bvci_n);
	status = WP_DlLookupEntryInsert(iw_sys_bridging, flow_info.agg_dl_dportGrp_handle,
                                    rule->BVCI, &dl_rt);
	//terminate_on_error(status, "WP_DlLookupEntryInsert");
	if(status != 0)
	{
		WDDI_WARNING("np_dl_route_table_add fail status = 0x%x \n",status);
		return status;
	}
	flow_info.UsedFlag = USED;
	npu_set_downlink_info(&flow_info);

	return ret;
}
#define WPI_NSVC_REMOTE_BIT_MASK 0x20000000


int np_dl_link_table_set(STRU_SET_FR_DOWNLINK_PATH_LINK_TABLE * link_table)
{

	WP_iw_agg_directmap_enet enet_iw_agg_cfg[1] =
        {
            {
                /* tag */ 				 0,
                /* txfunc */				 0,
                /* rfcs */				 WP_IW_RFCS_ENABLE,/*should be enabled*/
                /* l2header_insert_mode;*/ WP_IW_L2H_INSERT_ENABLE,
                /* interruptqueue;*/		 WP_IW_IRQT1,
                /* error_pkt_mode*/		 WP_IW_ERRPKT_DISCARD,
                /* intmode;*/ 			 WP_IW_INT_DISABLE,
#if STATISTICS_ENABLE
                /* statmode;*/			 WP_IW_STAT_ENABLE,
#else
                /* statmode;*/			 WP_IW_STAT_DISABLE,
#endif
                /* timestamp_mode;*/		 WP_IW_TIME_STAMP_DISABLE,
                /* ov_pool_mode*/ 		 0,
                /* fbp_drop_threshold;*/	 0,
                /* mtu;*/ 				 1536,
                /* prefix_length */		 /*34*/ 6,/*FR header+NSheader*/
                /* extraction_length */	 50,
                /* prefix_header[68];*/	 {
                    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x01,
                    0x02, 0x03, 0x04, 0x05, 0x81, 0x00, 0x00, 0x85,
                    0x08, 0x00, 0x45, 0x00, 0x00, 0x00, 0x00, 0x00,
                    0x40, 0x00, 0x40, 0x11, 0x00, 0x00, 0x00, 0x00,
                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                    0x00, 0x00, 0x00, 0x00
                },
                /* policer_enable */		 0,
                /* policer_config */		 NULL,
                /* cong_mode */			 0,
                /* cong_threshold_param */ NULL,
                /*WP_U8 dynamic_length_update;*/ WP_IW_LENGTH_UPDATE_L3,
                /*WP_U8 length_offset;*/ 18,
            }
        };


    WP_U32 i=0,j=0;
	WP_U32 vfort;
	WP_S32 status;
	WP_handle enet_iw_flow_aggregation;
	wp_vport_table dl_link_tables;
    STRU_SET_INITAIL_CONFIG *initial_cfg = npu_get_initial_cfg_info();
    for(i =0 ;i< link_table->FrVportAcitveNum;i++)
    {
		if(link_table->NsvcGrp[i].Flag == LOCAL_FLAG)
		{
		    vfort = link_table->NsvcGrp[i].vfort;
		    STRU_VPORT_INFO  *VportInfo = npu_get_vport_info(vfort);
		    FR_CHANNEL_INFO * ch_info;
		    if(VportInfo == NULL)
		    {
		        return ERR_DOWNLINX_FLOW_ADD;
		    }
			else if(VportInfo->UsedFlag == UNUSED)
			{
		        return ERR_DOWNLINX_FLOW_ADD;
			}

		    ch_info = &VportInfo->ch_info;
		    if(ch_info->agg_downlink_tx2fr_device ==0)
		    {
#if ENEA_RELEASE
			    enet_iw_agg_cfg->txfunc =ch_info->tx_iw_handle;// iw_rx_host_handle;//ch_info->rx_host_handle;//ch_info->tx_iw_handle;
#else/*for ENEA debug*/
			    enet_iw_agg_cfg->txfunc = ch_info->agg_fr_rx_host_flow;//ch_info->tx_iw_handle;
#endif
                if(enet_iw_agg_cfg->txfunc == 0)
				{
				    WDDI_WARNING("np_dl_link_table_set ch_info is not correct !\n");
                    return ERR_DOWNLINX_FLOW_ADD;
				}
			    enet_iw_agg_cfg->l2_header_insert_mode = WP_IW_L2H_INSERT_ENABLE;
			    enet_iw_agg_cfg->prefix_length = 2;/*add fr*/
				enet_iw_agg_cfg->dynamic_length_update = WP_IW_LENGTH_UPDATE_DISABLED;
				if(initial_cfg->VlanFlag == NPU_VLAN_ENABLE)
				{
					enet_iw_agg_cfg->extraction_length = 46;/*rm mac+ip+udp header*/
				}
				else
				{
					enet_iw_agg_cfg->extraction_length = 42;/*rm mac+ip+udp header*/
				}
			    WP_FR_HEADER_2OCT_SET(enet_iw_agg_cfg->prefix_header,ch_info->dlci,0,0,0,0);
			    ch_info->agg_downlink_tx2fr_device = WP_IwFlowAggregationCreate(0,WP_IW_DIRECTMAP_MODE, enet_iw_agg_cfg);
			    terminate_on_error(ch_info->agg_downlink_tx2fr_device, "WP_IwFlowAggregationCreate np_dl_link_table_set");
		    }
		    dl_link_tables.flow_num[i] = WPI_HANDLE_INDEX(ch_info->agg_downlink_tx2fr_device);
			j++;
		}
		else
		{
		    WP_U32 peer_vfort;
		    peer_vfort = link_table->NsvcGrp[i].vfort;
		    STRU_PEER_VPORT_INFO  *VportInfo = npu_set_peer_vport_info(peer_vfort);
		    if(VportInfo->agg_downlink_tx2peerfr == 0)
		    {
				struct iphdr * ipheader;
				struct vlan_ethhdr * vlan_ethheader;
				enet_iw_agg_cfg->l2_header_insert_mode = WP_IW_L2H_INSERT_ENABLE;
				enet_iw_agg_cfg->txfunc = agg_enet_2;
				enet_iw_agg_cfg->mtu = 0x600;
				enet_iw_agg_cfg->prefix_length = 46;
				vlan_ethheader = (struct vlan_ethhdr *)&enet_iw_agg_cfg->prefix_header[0];
				memset(&vlan_ethheader->h_dest[0],	0xfe, 6);
				memcpy(&vlan_ethheader->h_source[0], initial_cfg->ifMACsrc, 6);
				vlan_ethheader->h_vlan_proto = htons(0x8100);
				vlan_ethheader->h_vlan_TCI = htons(BPORT_DEFAULT_VLAN_TAG);
				vlan_ethheader->h_vlan_encapsulated_proto = htons(0x0800);
				if(initial_cfg->VlanFlag == NPU_VLAN_ENABLE)
				{
                    enet_iw_agg_cfg->extraction_length = 46;
				}
				else
				{
                    enet_iw_agg_cfg->extraction_length = 42;
				}
				enet_iw_agg_cfg->dynamic_length_update = WP_IW_LENGTH_UPDATE_L3;
				enet_iw_agg_cfg->length_offset = 18;


				ipheader = (struct iphdr *)&enet_iw_agg_cfg->prefix_header[18];
				ipheader->saddr=htonl(link_table->NsvcGrp[i].vfort);//How to get vport information
				/*Fprintf("prefix head source ip %d,%d,%d,%d", enet_iw_agg_cfg->prefix_header[30],
                  enet_iw_agg_cfg->prefix_header[31],
                  enet_iw_agg_cfg->prefix_header[32],
                  enet_iw_agg_cfg->prefix_header[33]);*/

				enet_iw_flow_aggregation = WP_IwFlowAggregationCreate(0,WP_IW_DIRECTMAP_MODE, enet_iw_agg_cfg);
				terminate_on_error(status, "np_dl_link_table_set enet_iw_flow_aggregation");
				VportInfo->agg_downlink_tx2peerfr = enet_iw_flow_aggregation;
				//Fprintf("VportInfo->agg_downlink_tx2peerfr = %d \n",VportInfo->agg_downlink_tx2peerfr);
			}
			dl_link_tables.flow_num[i] = WPI_HANDLE_INDEX(VportInfo->agg_downlink_tx2peerfr);
		}
    }

    dl_link_tables.valid_num =link_table->FrVportAcitveNum;
	/*SPPSP-22  NPU don't redirect traffic to peer NPU when both NPUs have same number available link*/
	if(j == (link_table->FrVportAcitveNum - j))
	{
	    if(j ==0)
	    {
			dl_link_tables.valid_num =0;
		}
		else
		{
			dl_link_tables.valid_num |=WPI_NSVC_REMOTE_BIT_MASK;
		}
	}
	dl_link_tables.valid_num |= (j<<16);

	status = WP_VPortEntryInsert(0, link_table->NSEI, &dl_link_tables);
	terminate_on_error(status, "WP_VPortEntryInsert");
	WDDI_INFO("Info: np_dl_link_table_set, vfort:0x%x, link_table->FrVportAcitveNum:%d, link_table->NSEI:%d\n",vfort,link_table->FrVportAcitveNum,link_table->NSEI);
	npu_set_fr_dl_link_table_info(link_table);
	return 0;
}

int np_ipdefrag_flow_add(void)
{
    WP_S32 status;
    WP_handle flow_handle;
    /*WP_handle enet_iw_flow_defrag_aggregation;*/
	/*WP_handle ipdefrag_agg;*/
	NP_SYSTEM_INFO * the_system = np_get_system_info();
	struct vlan_ethhdr * vlan_ethheader;
	struct ethhdr * ethhdr;
    WP_CHAR enet_rule_string[128]={0};
	STRU_SET_INITAIL_CONFIG *initial_cfg = npu_get_initial_cfg_info();

	if(initial_cfg->VlanFlag == NPU_VLAN_ENABLE)
	{
        vlan_ethheader = (struct vlan_ethhdr *)&tx_agg_l3->prefix_header[0];
    	tx_agg_l3->prefix_length = 18;
		memcpy(&vlan_ethheader->h_dest[0],	initial_cfg->ifMACsrc, 6);
		vlan_ethheader->h_vlan_proto = htons(0x8100);
		vlan_ethheader->h_vlan_TCI = htons(initial_cfg->VlanTag);//htons(BPORT_DEFAULT_VLAN_TAG);
		vlan_ethheader->h_vlan_encapsulated_proto = htons(0x0800);
	}
	else
	{
	    ethhdr = (struct ethhdr *)&tx_agg_l3->prefix_header[0];
		memcpy(&ethhdr->h_dest[0],	initial_cfg->ifMACsrc, 6);
    	tx_agg_l3->prefix_length = 14;
		ethhdr->h_proto = htons(0x0800);
	}
	tx_agg_l3->l2_header_insert_mode = WP_IW_L2H_INSERT_ENABLE;
	tx_agg_l3->txfunc = the_system->agg_after_defrag;//iw_sys_bridging;//default_host_aggregation;//iw_sys_bridging;
	//tx_agg_l3->iw_port = iw_enet_3;
	tx_agg_l3->mtu = N_MAX_IP_FRAME_SIZE;
	ipdefrag_agg = WP_IwFlowAggregationCreate(0, WP_IW_ROUTING_MODE,tx_agg_l3);
	terminate_on_error(ipdefrag_agg, "WP_IwFlowAggregationCreate");

	/* create the ip defrag group */
	defrag_group->defrag_pool = ip_defrag_context_pool;
	defrag_group->defrag_hash = ip_defrag_hash_pool;
	defrag_group->qniw = qniw_defrag;
	the_system->ip_defrag_group = WP_IwIpDefragGroupCreate(ip_frag_system_handle,ipdefrag_agg,defrag_group);
	terminate_on_error(the_system->ip_defrag_group, "WP_IwIpDefragGroupCreate");
    enet_iw_agg_defrag_cfg->txfunc = ipdefrag_agg;
	if(initial_cfg->VlanFlag == NPU_VLAN_ENABLE)
	{
		enet_iw_agg_defrag_cfg->extraction_length = 18;
	}
	else
	{
		enet_iw_agg_defrag_cfg->extraction_length = 14;
	}
	enet_iw_flow_defrag_aggregation = WP_IwFlowAggregationCreate(0,WP_IW_DIRECTMAP_MODE, enet_iw_agg_defrag_cfg);
	terminate_on_error(enet_iw_flow_defrag_aggregation, "WP_IwFlowAggregationCreate enet_iw_flow_defrag_aggregation");

    sprintf(enet_rule_string, "n;n;n;n;n;n;n;UDP;");
	enet_to_enet_flow_policy_cfg[NPU_FILTER_UDP_DEFRAG].classifier_string = enet_rule_string;
	enet_to_enet_flow_policy_cfg[NPU_FILTER_UDP_DEFRAG].action_info.forwarding_action.flow_aggregation = enet_iw_flow_defrag_aggregation;
    enet_to_enet_flow_policy_cfg[NPU_FILTER_UDP_DEFRAG].dfc_info = &enet_to_enet_dfc_flow_info[NPU_FILTER_UDP_DEFRAG];
	enet_to_enet_flow_policy_cfg[NPU_FILTER_UDP_DEFRAG].match_action = WP_FLOW_CLASS_MODE_FLOW_AGG;
	flow_handle = WP_IwFlowAdd(iw_sys_bridging, WP_FLOW_CLASS_RULE,&enet_to_enet_flow_policy_cfg[NPU_FILTER_UDP_DEFRAG]);
	terminate_on_error(flow_handle, "WP_IwFlowAdd() enet_to_enet_iw_sys_defrag");

  	status = WP_IwSystemBuild(iw_sys_bridging);
    terminate_on_error(status, "WP_IwSystemBuild - np_ipdefrag_flow_add");
	return 0;
}


int np_dl_route_table_delete(WP_U16 DportStartValue,WP_U16 bvci,WP_U32 OptMode)
{
    /*OptMode
      DEL_DL_RULE_TALBE_BASED_ON_DPORT
      DEL_DL_RULE_TALBE_BASED_ON_BVCI
      DEL_DL_RULE_TALBE_BASED_ON_DPORT_BVCI

	*/
	//STRU_SET_INITAIL_CONFIG *initial_cfg = npu_get_initial_cfg_info();
    NP_DL_FLOW_INFO *flow_info_ptr;
	char *key;
	NODE * temp;
    if(OptMode == DEL_DL_RULE_TALBE_BASED_ON_DPORT_BVCI)
    {
		flow_info_ptr = npu_get_downlink_info(DportStartValue,bvci);

	    if(flow_info_ptr == NULL)
	    {
	        WDDI_WARNING("can not find folw to be deleted.DportStartValue = %d bvci= %d \n",DportStartValue,bvci);
			return ERR_DOWNLINX_FLOW_DELETE;
	    }
		else if(flow_info_ptr->UsedFlag == UNUSED)
		{
			WDDI_WARNING("this dl route has been been deleted OptMode = DEL_DL_RULE_TALBE_BASED_ON_DPORT_BVCI\n");
			return ERR_DOWNLINX_FLOW_DELETE;
		}

	    WDDI_INFO("Info: np_dl_route_table_delete::1, DportStartValue:0x%x, bvci:%d\n",DportStartValue,bvci);
		WP_DlLookupEntryRemove(0,flow_info_ptr->agg_dl_dportGrp_handle,bvci);
		flow_info_ptr->UsedFlag = UNUSED;
		npu_set_downlink_info(flow_info_ptr);
	}
	else if(OptMode == DEL_DL_RULE_TALBE_BASED_ON_DPORT)
	{
		temp = fr_dl_dport_nodehead[DportStartValue];
	    while(temp!=NULL)
	    {
	        key = temp->ht_key;
	        temp=temp->next;
            flow_info_ptr = npu_get_dl_route_table_info(key);
		    if(flow_info_ptr == NULL)
		    {
				WDDI_WARNING("Warn: can not find folw to be deleted.DportStartValue = %d \n",DportStartValue);
				//return ERR_DOWNLINX_FLOW_DELETE;
		    }
			else if(flow_info_ptr->UsedFlag == UNUSED)
			{
				WDDI_WARNING("Warn: this dl route has been been deleted ,OptMode = DEL_DL_RULE_TALBE_BASED_ON_DPORT\n");
				//return ERR_DOWNLINX_FLOW_DELETE;
			}
			else
			{
				WDDI_INFO("Info: np_dl_route_table_delete::2, DportStartValue:0x%x, bvci:%d\n",DportStartValue,bvci);
				WP_DlLookupEntryRemove(0,flow_info_ptr->agg_dl_dportGrp_handle,flow_info_ptr->rule.BVCI);
				flow_info_ptr->UsedFlag = UNUSED;
				npu_set_downlink_info(flow_info_ptr);
			}
	    }
	}
	else if(OptMode == DEL_DL_RULE_TALBE_BASED_ON_BVCI)
	{
		temp = fr_dl_bvci_nodehead[bvci];
	    while(temp!=NULL)
	    {
	        key = temp->ht_key;
            temp=temp->next;
            flow_info_ptr = npu_get_dl_route_table_info(key);
		    if(flow_info_ptr == NULL)
		    {
				WDDI_WARNING("Warn: can not find folw to be deleted.bvci= %d \n",bvci);
				//return ERR_DOWNLINX_FLOW_DELETE;
		    }
			else if(flow_info_ptr->UsedFlag == UNUSED)
			{
				WDDI_WARNING("Warn: this dl route has been been deleted ,OptMode = DEL_DL_RULE_TALBE_BASED_ON_BVCI\n");
				//return ERR_DOWNLINX_FLOW_DELETE;
			}
			else
			{
				WDDI_INFO("Info: np_dl_route_table_delete::3, DportStartValue:0x%x, bvci:%d\n",DportStartValue,bvci);
				WP_DlLookupEntryRemove(0,flow_info_ptr->agg_dl_dportGrp_handle,bvci);
				flow_info_ptr->UsedFlag = UNUSED;
				npu_set_downlink_info(flow_info_ptr);
			}
	    }
	}
	else
	{
        WDDI_ERR("Error: np_dl_route_table_delete OptMode= %d \n",OptMode);
        return -1;
	}

	return 0;
}

void WT_TranslateAsciiToHexGIGEGIGE(WP_CHAR *Hex, WP_CHAR *Ascii,
                                    WP_U32 length)
{
    WP_S32 i, first = 0, second = 0;

    for (i=0;i<length;i++) {
        if (Ascii[2*i] >= '0' && Ascii[2*i] <= '9')
            first = Ascii[2*i] - '0';
        if (Ascii[2*i] >= 'a' && Ascii[2*i] <= 'f')
            first = Ascii[2*i] - 'a' + 10;
        if (Ascii[2*i+1] >= '0' && Ascii[2*i+1] <= '9')
            second = Ascii[2*i+1] - '0';
        if (Ascii[2*i+1] >= 'a' && Ascii[2*i+1] <= 'f')
            second = Ascii[2*i+1] - 'a' + 10;

        Hex[i] = (WP_CHAR)(first * 16 + second);
    }
    return;
}
/*
  static void AppTxEvent(WP_tag tag, WP_U32 data, WP_U32 info)
  {
  ++tx_event;
  }
*/

int pipefull = 0;
int pipefullFRH = 0;
int pipefullEthH =0;
int pipefullEthL =0;
int pipeWriteEventNum =0;
static void AppRxEvent(WP_tag tag, WP_U32 data, WP_U32 info)
{
    int ret = 0;
    if(g_logLevel ==0 )
    {
        char tmp[100];
        sprintf(tmp,"AppRxEvent tag = %d \n",tag);
        //puts(tmp);
    }
    if(tag == RX_ETH_INT_TAG)/*IP*/
    {
        ret =write(pipefdRxEth[1], (void *)&tag, 4);
        if(ret < 0)
        {
            //puts("write pipefdRxEth error\n");
            pipefullEthL++;
        }
        pipeWriteEventNum++;
    }
    else if(tag == RX_ETH_INT_PRIO_MSG_TAG )
    {
        ret =write(pipefdRxEthHprio[1], (void *)&tag, 4);
        if(ret < 0)
        {
            //puts("write pipefdRxEth error\n");
            pipefullEthH++;
        }
        pipeWriteEventNum++;

    }
    else if(tag == RX_FR_INT_PRIO_MSG_TAG) /*FR high priority*/
    {
        ret =write(pipefd_RxFrHprio[1], (void *)&tag, 4);
        if (ret < 0)
        {
            pipefullFRH++;
        }
    }
    else
    {
        ret =write(pipefd[1], (void *)&tag, 4);
        if(ret < 0)
        {
            pipefull++;
        }
    }

}


NP_SYSTEM_INFO *np_get_system_info(void)
{
    return &np_system;
}

void np_display_iw_port_stat(void)
{
	NP_SYSTEM_INFO * the_system = np_get_system_info();

	Fprintf("\n display Bport_enet1_handle info\n");
	iw_port_stat(the_system->Bport_enet1_handle);
	Fprintf("\n display Bport_enet2host_handle  info\n");
	iw_port_stat(the_system->Bport_enet2host_handle);
	Fprintf("###\n display iw_enet_defrag info###\n");
	iw_port_stat(iw_enet_3);
	Fprintf("###\n display Rport_enet1_hanlde info###\n");
	iw_route_port_stat(the_system->Rport_enet1_hanlde);
	Fprintf("###\n display Bport_mac_filter_handle info###\n");
	iw_port_stat(the_system->Bport_mac_filter_handle);
	Fprintf("###\n display Bport_mac_filter_from_eth2_handle info###\n");
	iw_port_stat(the_system->Bport_mac_filter_from_eth2_handle);
}

WP_bridge_port bridge_bport_conf[1] =
{
    {
        /* tag */ 0x5555,
        /* direct_mapping*/WP_IW_DIRECT_MAP_DISABLE,
        /* flow_agg */0,
        /* termination_mode*/ /*WP_IW_L2_TERM_MODE*/WP_IW_HOST_TERM_MODE,
        /* learning_mode */WP_IW_LEARNING_DISABLE,
        /* in_filter_mode */WP_IW_INGRESS_FILTER_DISABLE,
        /* vlan_param */
        {
            /* vlan_acceptance_mode */WP_IW_ACCEPT_TAGGED_UNTAGGED,
            /* vlan_tag */0xa,
        },
        /* max_mac_addresses */ 21,
        /* group_tag */ 0,
        /* group_filtering_mode*/WP_IW_GROUP_FILTER_DISABLE,
        /* unk_mac_sa_filter*/0,
        /* unk_mc_mode*/WP_IW_UNK_MC_HT,
        /* bc_ht_mode*/WP_IW_BC_HT_DISABLE,
        /*WP_U32 input_filters_mask;*/ 0x80000000,
        /*WP_U32 output_filters_mask;*/0,
#if STATISTICS_ENABLE
        /*WP_iw_port_statmode statmode;*/WP_IW_PORT_STAT_ENABLE,
#else
        /*WP_iw_port_statmode statmode;*/WP_IW_PORT_STAT_DISABLE,
#endif
        /* unk_uc_mode;	  */ WP_IW_UNK_UC_SR_ENABLE,
        /* classification_flag; */ WP_IW_BPORT_CLASSIFICATION_ENABLED,
        /* adv_unk_lookup_mode; */ WP_IW_ADV_UNK_LOOKUP_DISABLED,
        /* cfi_ht_mode;				*/ WP_IW_CFI_HT_DISABLE,
        /* reserved_mc_ht_mode; */ WP_IW_RES_MC_HT_DISABLE

    }
};

WP_iw_agg_directmap bridge_iw_agg_cfg [1] = {

    {
        /*WP_tag tag;*/ 0,
        /*WP_handle txfunc;*/   0,
        /*WP_U8 rfcs;*/ WP_IW_RFCS_DISABLE,
        /*WP_U8 l2_header_insert_mode;*/ /* 10*/ WP_IW_L2H_INSERT_ENABLE,
        /*WP_U8 interruptqueue;*/ WP_IW_L2H_INSERT_DISABLE,
        /*WP_U8 error_pkt_mode;*/ WP_IW_ERRPKT_DISCARD,
        /*WP_U8 intmode;*/ WP_IW_INT_DISABLE,
        /*WP_U8 statmode;*/ WP_IW_STAT_ENABLE,
        /*WP_U8 timestamp_mode;*/ WP_IW_TIME_STAMP_DISABLE,
        /*WP_U8 ov_pool_mode;*/ WP_IW_OV_POOL_DISABLE,
        /*WP_U16 fbp_drop_threshold;*/ 0,
        /*WP_U16 mtu; */ /*20 */ 1536,
        /*WP_U8 prefix_length;*/ 46,
        /*WP_U8 extraction_length;*/ 46,
        /*WP_U8 prefix_header[68];*/
        {
            /*0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
              0x00, 0x00,*/ 0x45, 0x00, 0x00, 0x00, 0x00, 0x00,
              0x40, 0x00, 0x40, 0x11, 0x00, 0x00, 0x00, 0x00, /*ip frage flag 0x40*/
              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
              0x00, 0x00, 0x00, 0x00
        },
        /*WP_U8 policer_enable;*/ WP_IW_POLICER_DISABLE,
        /*WP_policer_config   *policer_config  */ /*96*/ NULL,
        /*WP_U8 cong_mode;*/ WP_IW_CONGESTION_DISABLE,
        /*WP_iw_flow_agg_cong_thresh_param *cong_threshold_param;*/ NULL,
        /*WP_U8 dynamic_length_update;*/ WP_IW_LENGTH_UPDATE_L3,
        /*WP_U8 length_offset;*/ 0,
        /*WP_flowaggtype    flow_agg_type;*/ WP_IW_FLOW_AGG_PRIMARY,
        /*WP_handle comp_group_handle;*/ 0
    }
};

WP_iw_agg_bridging bridge_host_agg_cfg[] =
{
    {
        /*tag*/ 2,
        /*txfunc*/ 0,
        /*input_bport*/0,
        /*rfcs*/WP_IW_RFCS_ENABLE,
        /*l2_header_insert_mode;*/WP_IW_L2H_INSERT_DISABLE,
        /*vlan_tag_mode*/WP_IW_VLAN_TAG_DISABLE,
        /*interruptqueue;*/WP_IW_IRQT1,
        /*error_pkt_mode*/WP_IW_ERRPKT_DISCARD,
        /*intmode;*/WP_IW_INT_ENABLE,
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



int np_updateBVCI_flow_add(STRU_SET_UPDATE_BVCI_PATH_RULE *rule)
{

	WP_iw_agg_directmap bridge_iw_agg_cfg [1] = {

        {
            /*WP_tag tag;*/ 0,
            /*WP_handle txfunc;*/   0,
            /*WP_U8 rfcs;*/ WP_IW_RFCS_DISABLE,
            /*WP_U8 l2_header_insert_mode;*/ /* 10*/ WP_IW_L2H_INSERT_ENABLE,
            /*WP_U8 interruptqueue;*/ WP_IW_L2H_INSERT_DISABLE,
            /*WP_U8 error_pkt_mode;*/ WP_IW_ERRPKT_DISCARD,
            /*WP_U8 intmode;*/ WP_IW_INT_DISABLE,
            /*WP_U8 statmode;*/ WP_IW_STAT_ENABLE,
            /*WP_U8 timestamp_mode;*/ WP_IW_TIME_STAMP_DISABLE,
            /*WP_U8 ov_pool_mode;*/ WP_IW_OV_POOL_DISABLE,
            /*WP_U16 fbp_drop_threshold;*/ 0,
            /*WP_U16 mtu; */ /*20 */ 1536,
            /*WP_U8 prefix_length;*/ 46,
            /*WP_U8 extraction_length;*/ 46,
            /*WP_U8 prefix_header[68];*/
            {
                /*0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                  0x00, 0x00,*/ 0x45, 0x00, 0x00, 0x00, 0x00, 0x00,
                  0x40, 0x00, 0x40, 0x11, 0x00, 0x00, 0x00, 0x00, /*ip frage flag 0x40*/
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                  0x00, 0x00, 0x00, 0x00
            },
            /*WP_U8 policer_enable;*/ WP_IW_POLICER_DISABLE,
            /*WP_policer_config	 *policer_config  */ /*96*/ NULL,
            /*WP_U8 cong_mode;*/ WP_IW_CONGESTION_DISABLE,
            /*WP_iw_flow_agg_cong_thresh_param *cong_threshold_param;*/ NULL,
            /*WP_U8 dynamic_length_update;*/ WP_IW_LENGTH_UPDATE_L3,
            /*WP_U8 length_offset;*/ 0,
            /*WP_flowaggtype    flow_agg_type;*/ WP_IW_FLOW_AGG_PRIMARY,
            /*WP_handle comp_group_handle;*/ 0
        }
	};

    WP_S32 status;
    STRU_SET_INITAIL_CONFIG *initial_cfg = npu_get_initial_cfg_info();
	STRU_VPORT_INFO  *VportInfo;
	u32 card;
	u32 trunk;
	u32 group;
	u32 dlci;
	if(initial_cfg->OpterateMode == NPU_MODE_FR)
	{
		VportInfo = npu_get_vport_info(rule->u_port.vfport);
		np_fraddr_unpack(rule->u_port.vfport,&card,&trunk,&group,&dlci);
	}
	else
	{
		VportInfo = npu_get_vport_info(rule->u_port.portDst);
	}
	if(VportInfo == NULL)
	{
	    if(initial_cfg->OpterateMode == NPU_MODE_FR)
	    {
			WDDI_WARNING("Error: np_updateBVCI_flow_add::npu_get_vport_info failed! vport = %d S%dP%dFR%dD%d\n ",rule->u_port.vfport,card,trunk,group,dlci);
	    }
		else
		{
			WDDI_WARNING("Error: np_updateBVCI_flow_add::npu_get_vport_info failed! vport = %d \n ",rule->u_port.vfport);
		}
		return ERR_BVCI_FLOW_ADD;
	}
    else if(VportInfo->UsedFlag == UNUSED)
    {
		WDDI_WARNING("Error: np_updateBVCI_flow_add::npu_get_vport_info, vport:%d UNUSED!\n ",rule->u_port.vfport);
		return ERR_BVCI_FLOW_ADD;
	}
	if(initial_cfg->OpterateMode == NPU_MODE_FR)
	{
        WDDI_INFO("Info: np_updateBVCI_flow_add::npu_get_vport_info, vport = 0x%x:S%dP%dFR%dD%d\n ",rule->u_port.vfport,card,trunk,group,dlci);
		FR_CHANNEL_INFO *ch_info = &VportInfo->ch_info;
#if 0 /*modify for remove vport related resource*/
		if(ch_info->agg_tx2Enet1 == 0)
		{

			WDDI_WARNING(" np_updateBVCI_flow_add error,the FR ch_info->agg_tx2Enet1 is NULL!\n ",rule->u_port.vfport);
			return ERR_BVCI_FLOW_ADD;
		}
#endif
	    if(ch_info->agg_tx2Enet1 == 0)
	    {
			/*create flow for every vport,this flowagg prefix header will be modified in np_ul_route_table_add()*/
			bridge_iw_agg_cfg->txfunc = agg_enet_1;

			bridge_iw_agg_cfg->prefix_length = 46;
			bridge_iw_agg_cfg->extraction_length = 2;	  /*rm fr header*/
			ch_info->agg_tx2Enet1 = WP_IwFlowAggregationCreate(0,WP_IW_DIRECTMAP_MODE, bridge_iw_agg_cfg);
			terminate_on_error(ch_info->agg_tx2Enet1, "WP_IwFlowAggregationCreate ch_info->agg_tx2Enet1");
		}
		/*add hash search for LOCAL ip_ep*/
		status = WPI_UserDefHashEntryInsert(ch_info->device_handle,WPI_HANDLE_INDEX(ch_info->agg_tx2Enet1),rule->NS_BVCI_old,rule->NS_BVCI);
		terminate_on_error(status, "WPI_UserDefHashEntryInsert LOCAL IPEP");

		if(ch_info->agg_fr_to_enet2 == 0)
		{
            np_ul_fr_to_enet2_create(ch_info);
		}
		/*add hash search for peer ip_ep*/
		status = WPI_UserDefHashEntryInsert(ch_info->device_handle,WPI_HANDLE_INDEX(ch_info->agg_fr_to_enet2),rule->NS_BVCI_old,rule->NS_BVCI);
		terminate_on_error(status, "WPI_UserDefHashEntryInsert PEER IPEP");

#if 0
		WP_U32 result = 0;

		result = 0;
		status = WPI_UserDefHashEntryDisplay(ch_info->device_handle,WPI_HANDLE_INDEX(ch_info->agg_tx2Enet1),rule->NS_BVCI_old,(WPI_PTR)hash_base,&result);
		terminate_on_error(status, "WPI_UserDefHashEntryDisplay");
		Fprintf("key1 = %d key2=0x%x data = 0x%x\n",WPI_HANDLE_INDEX(ch_info->agg_tx2Enet1),rule->NS_BVCI_old,result);
#endif

	}
	else
	{
        WDDI_INFO("Info: np_updateBVCI_flow_add::npu_get_vport_info, vport:%d\n",rule->u_port.vfport);


		if(VportInfo->agg_ul_ip2iptx2Enet1 == 0)
		{
		    NP_SYSTEM_INFO * the_system = np_get_system_info();
			bridge_iw_agg_cfg->txfunc =the_system->agg_enet_default_host_handle;/*make sure create it success*/
			bridge_iw_agg_cfg->prefix_length = 28;		  /*add ipv4header+udp header ,this parameter will be modify when add ul_route_table*/
			bridge_iw_agg_cfg->extraction_length = 46;	  /*remove mac+ipv4header+udp heade,this parameter will be modify when add ul_route_table*/
			VportInfo->agg_ul_ip2iptx2Enet1 = WP_IwFlowAggregationCreate(0,WP_IW_DIRECTMAP_MODE,bridge_iw_agg_cfg);
			terminate_on_error(VportInfo->agg_ul_ip2iptx2Enet1, "np_updateBVCI_flow_add WP_IwFlowAggregationCreate agg_ul_ip2iptx2Enet1");
		}

        status = WPI_UserDefHashEntryInsert(0,WPI_HANDLE_INDEX(VportInfo->agg_ul_ip2iptx2Enet1),rule->NS_BVCI_old,rule->NS_BVCI);
        terminate_on_error(status, "WPI_UserDefHashEntryInsert agg_ul_ip2iptx2Enet1");

        if(VportInfo->agg_ul_ip_to_enet2_handle == 0)
	    {
			np_ul_ip_forward_to_enet2_create(VportInfo);
		}
        status = WPI_UserDefHashEntryInsert(0,WPI_HANDLE_INDEX(VportInfo->agg_ul_ip_to_enet2_handle),rule->NS_BVCI_old,rule->NS_BVCI);
        terminate_on_error(status, "WPI_UserDefHashEntryInsert agg_ul_ip_to_enet2_handle" );
	}



	NP_UPDATE_BVCI_FLOW_INFO flow_info;
    flow_info.agg_updateBVCI_handle = 0;
    flow_info.flowHandle_bvci = 0;
	flow_info.vport = rule->u_port.vfport;
    flow_info.NS_BVCI = rule->NS_BVCI;
	flow_info.NS_BVCI_old= rule->NS_BVCI_old;
	flow_info.UsedFlag = USED;


	if(initial_cfg->OpterateMode == NPU_MODE_FR)
	{
		(void)npu_set_update_bvci_info(rule->u_port.vfport,rule->NS_BVCI_old,&flow_info,VportInfo);

	}
	else
	{
		(void)npu_set_update_bvci_info(rule->u_port.portDst,rule->NS_BVCI_old,&flow_info,VportInfo);
	}

	return 0;
}

int np_updateBVCI_flow_delete(WP_U32 vfport,WP_U16 NS_BVCI_old,WP_U32 OptMode)

{
    int status = WP_OK;
	NP_UPDATE_BVCI_FLOW_INFO * bvci_info = NULL;
    STRU_VPORT_INFO  *VportInfo = npu_get_vport_info(vfport);
	STRU_SET_INITAIL_CONFIG *initial_cfg = npu_get_initial_cfg_info();

	if(VportInfo == NULL)
	{
		WDDI_WARNING("Error: np_updateBVCI_flow_delete::npu_get_vport_info failed! vport:%d\n",vfport);
		return ERR_BVCI_FLOW_DELETE;
	}else if(VportInfo->UsedFlag == UNUSED)
	{
		WDDI_WARNING("Error: np_updateBVCI_flow_delete::npu_get_vport_info, vport:%d unused!\n ",vfport);
		return ERR_BVCI_FLOW_DELETE;
	}
	if(OptMode == DEL_UL_UPDATE_BVCI_TALBE_BASED_ON_PORT_BVCI)
	{
        bvci_info = npu_get_update_bvci_info(vfport,NS_BVCI_old);
        if(NULL == bvci_info)
        {
            WDDI_WARNING(" np_updateBVCI_flow_delete npu_get_update_bvci_info vport = %d \n ",vfport);
            return ERR_BVCI_FLOW_DELETE;
        }
        if(initial_cfg->OpterateMode == NPU_MODE_FR)
        {
            FR_CHANNEL_INFO *ch_info = &VportInfo->ch_info;
            status = WPI_UserDefHashEntryRemove(0,WPI_HANDLE_INDEX(ch_info->agg_tx2Enet1),NS_BVCI_old,bvci_info->NS_BVCI);
            if (status == WP_ERR_NOT_EXIST_IN_HASH)
            {
                WDDI_WARNING("WPI_UserDefHashEntryRemove key not exist in hash \n");
                return ERR_BVCI_FLOW_DELETE;
            }
            status = WPI_UserDefHashEntryRemove(0,WPI_HANDLE_INDEX(ch_info->agg_fr_to_enet2),NS_BVCI_old,bvci_info->NS_BVCI);
            if (status == WP_ERR_NOT_EXIST_IN_HASH)
            {
                WDDI_WARNING("WPI_UserDefHashEntryRemove key not exist in hash \n");
                return ERR_BVCI_FLOW_DELETE;
            }
            WDDI_INFO("Info: np_updateBVCI_flow_delete::1\n");
        }
        else /*IP<->IP*/
        {
            status = WPI_UserDefHashEntryRemove(0,WPI_HANDLE_INDEX(VportInfo->agg_ul_ip2iptx2Enet1),NS_BVCI_old,bvci_info->NS_BVCI);
            if (status == WP_ERR_NOT_EXIST_IN_HASH)
            {
                WDDI_WARNING("WPI_UserDefHashEntryRemove key not exist in hash \n");
                return ERR_BVCI_FLOW_DELETE;
            }
            status = WPI_UserDefHashEntryRemove(0,WPI_HANDLE_INDEX(VportInfo->agg_ul_ip_to_enet2_handle),NS_BVCI_old,bvci_info->NS_BVCI);
            if (status == WP_ERR_NOT_EXIST_IN_HASH)
            {
                WDDI_WARNING("WPI_UserDefHashEntryRemove key not exist in hash \n");
                return ERR_BVCI_FLOW_DELETE;
            }
            WDDI_INFO("Info: np_updateBVCI_flow_delete::2\n");
        }
#if 0
        WP_U32 result = 0;
        status = WPI_UserDefHashEntryDisplay(ch_info->device_handle,WPI_HANDLE_INDEX(ch_info->agg_tx2Enet1),NS_BVCI_old,&result);
        if (status != WP_ERR_NOT_EXIST_IN_HASH)
        {
            Fprintf("key1 = %d key2=0x%x data = 0x%x\n",WPI_HANDLE_INDEX(ch_info->agg_tx2Enet1),NS_BVCI_old,result);
        }
        else
        {
            Fprintf(" WPI_UserDefHashEntryDisplay key not exist in hash \n ");

        }
#endif
        bvci_info->UsedFlag = UNUSED;
        (void)npu_set_update_bvci_info(vfport,NS_BVCI_old,bvci_info,VportInfo);
	}
	else if(OptMode == DEL_UL_UPDATE_BVCI_TALBE_BASED_ON_PORT)
	{

	    NODE *temp;
		char *key;
		NP_UPDATE_BVCI_FLOW_INFO *p_BvciFlowInfo;
		u32 i = 0;
	    temp=VportInfo->bvci_nodehead;
	    while(temp!=NULL)
	    {
	        key = temp->ht_key;
	        temp=temp->next;
			p_BvciFlowInfo = npu_get_ul_bvci_table_info(key);
			if(NULL == p_BvciFlowInfo)
			{
				WDDI_WARNING("When npu_get_ul_bvci_table_info failed\n");
			}
			else if(p_BvciFlowInfo->UsedFlag == UNUSED)
			{
				//WDDI_WARNING("npu_get_ul_bvci_table_info unused\n");
			}
			else
			{
			    /*find bvci talbe info base on vport*/
			    if(vfport == p_BvciFlowInfo->vport)
			    {
					bvci_info = npu_get_update_bvci_info(vfport,p_BvciFlowInfo->NS_BVCI_old);
					if(NULL == bvci_info)
					{
						WDDI_WARNING(" np_updateBVCI_flow_add npu_get_update_bvci_info vport = %d \n ",vfport);
						return ERR_BVCI_FLOW_DELETE;
					}
					if(initial_cfg->OpterateMode == NPU_MODE_FR)
					{
						FR_CHANNEL_INFO *ch_info = &VportInfo->ch_info;
						status = WPI_UserDefHashEntryRemove(0,WPI_HANDLE_INDEX(ch_info->agg_tx2Enet1),p_BvciFlowInfo->NS_BVCI_old,bvci_info->NS_BVCI);
						if (status == WP_ERR_NOT_EXIST_IN_HASH)
						{
							WDDI_WARNING("WPI_UserDefHashEntryRemove key not exist in hash \n");
							//return ERR_BVCI_FLOW_DELETE;
						}
                        WDDI_INFO("Info: np_updateBVCI_flow_delete::3\n");
					}
					else /*IP<->IP*/
					{
						status = WPI_UserDefHashEntryRemove(0,WPI_HANDLE_INDEX(VportInfo->agg_ul_ip2iptx2Enet1),p_BvciFlowInfo->NS_BVCI_old,bvci_info->NS_BVCI);
						if (status == WP_ERR_NOT_EXIST_IN_HASH)
						{
							WDDI_WARNING("WPI_UserDefHashEntryRemove key not exist in hash \n");
							return ERR_BVCI_FLOW_DELETE;
						}
                        WDDI_INFO("Info: np_updateBVCI_flow_delete::4\n");
					}
					p_BvciFlowInfo->UsedFlag = UNUSED;
				}
			}
	    }
	}
    return 0;
}

int np_ul_route_table_add(STRU_SET_UPLINK_PATH_RULE *uplink_rule)
{
    STRU_SET_INITAIL_CONFIG *initial_cfg = npu_get_initial_cfg_info();
	NP_SYSTEM_INFO * the_system = np_get_system_info();
    WP_U32 NRI = 0;
	/*if(NPU_NONPOOL == g_npnonpoolFlag)
      {
      NRI = 256;
      }
      else*/
	{
	    NRI = uplink_rule->NRIValue;
	}
	if(NRI>256)
	{
	    WDDI_WARNING("Error: np_ul_route_table_add, NIR:%d invalid!\n",NRI);
	    return ERR_UPLINX_FLOW_ADD;
	}
	//struct vlan_ethhdr * vlan_ethheader;
	//struct ethhdr * ethhdr;
	struct iphdr *ipheader;
	struct udphdr *udpheader;
	WP_status status;
    NP_UL_FLOW_INFO flow_info;
    FR_CHANNEL_INFO * ch_info;
	STRU_SET_UPLINK_PATH_LINK_TABLE *ul_link_table;
    //WP_handle ip_frag_group,tx_agg_ipfrag;
	WP_handle flow_agg[2]={0};

    STRU_VPORT_INFO  *VportInfo = npu_get_vport_info(uplink_rule->u_port.vfport);
	if(VportInfo ==  NULL)
	{
	    WDDI_WARNING("Error: np_ul_route_table_add::npu_get_vport_info fail! vfport = %d\n",uplink_rule->u_port.vfport);
	    return ERR_UPLINX_FLOW_ADD;
	}
	else if(VportInfo->UsedFlag == UNUSED)
	{
	    WDDI_WARNING("Error: np_ul_route_table_add::npu_get_vport_info, vfport:%d unused!\n",uplink_rule->u_port.vfport);
	    return ERR_UPLINX_FLOW_ADD;
	}
	if(initial_cfg->OpterateMode == NPU_MODE_FR)
	{
		ch_info = &VportInfo->ch_info;

		if(ch_info->agg_tx2Enet1 == 0)
	    {
			/*create flow for every vport,this flowagg prefix header will be modified in np_ul_route_table_add()*/
			bridge_iw_agg_cfg->txfunc = agg_enet_1;


			if(initial_cfg->VlanFlag == NPU_VLAN_ENABLE)
			{
				bridge_iw_agg_cfg->prefix_length = 46;
			}
			else
			{
				bridge_iw_agg_cfg->prefix_length = 42;
			}
			bridge_iw_agg_cfg->extraction_length = 2;	  /*rm fr header*/
			ch_info->agg_tx2Enet1 = WP_IwFlowAggregationCreate(0,WP_IW_DIRECTMAP_MODE, bridge_iw_agg_cfg);
			terminate_on_error(ch_info->agg_tx2Enet1, "WP_IwFlowAggregationCreate ch_info->agg_tx2Enet1");
		}
#if 0
		/*set arp for IPEP dst*/
		{
			/*find dst MAC according dst ip address*/
			NP_ARP_INFO arp={0};
			/*get dstip from link table ,key= uplink_rule->PortSrc*/
			ul_link_table = npu_get_ul_link_table_info(uplink_rule->PortSrc);
			if(ul_link_table == NULL)
			{
				WDDI_WARNING("ul link table is not existed ,key PortSrc = 0x%x",uplink_rule->PortSrc);
				return ERR_UPLINX_FLOW_ADD;
			}
			if(ul_link_table->IpEpAcitveNum == 0)
			{
				WDDI_WARNING("ul link table is not existed ,IpEpAcitveNum = 0\n");
				return ERR_UPLINX_FLOW_ADD;
			}
			int ret;
			char ArrayBuf[10];
			//WP_U32 IPtmp = 0xdc21aa0a;/*10.170.33.220*/;
			WP_U32 IpDst = ul_link_table->IpEp[0].IpDst;
			/*select IpEp[0].IpDst to get arp*/
			ret = tap_socket_getarp(IpDst, ArrayBuf);
			if(ret == 0)/*success*/
			{
				arp.Ip = IpDst;
				memcpy(arp.Mac,ArrayBuf,6);
				/*just for display*/
				(void)npu_set_arp_info(IpDst,&arp);
				np_update_mac_in_route_system(&arp);
			}
			else
			{
				WDDI_WARNING("np_ul_route_table_add npu_get_arp_info fail ipdst= 0x%x",IpDst);
				return ERR_UPLINX_FLOW_ADD;
			}
		}
#endif

		/*make sure fr host flow is created ,then modify the flowagg ,make sure to modified one time*/
		if(ch_info->agg_fr_rx_host_flow != 0 && ch_info->agg_tx2Enet1 != 0 && ch_info->agg_tx2Enet1ModifyFlag == 0)
		{
			/*IP header*/
			ipheader = (struct iphdr *)&bridge_iw_agg_cfg->prefix_header[0];

			ipheader->daddr=0; /*modify by dps according loadbalance*/
			ipheader->saddr=htonl(initial_cfg->ifIpaddr);//0x0a882150;//10.136.33.80

			udpheader = (struct udphdr *) (ipheader +1);
			udpheader->source = 0;
			udpheader->dest = 0;  /*modify by dps according loadbalance*/

			/*TODO should update NS head bvci*/
			bridge_iw_agg_cfg->mtu = 1536;//N_MAX_IP_FRAME_SIZE;
			bridge_iw_agg_cfg->iw_port = the_system->Rport_enet1_hanlde;
			bridge_iw_agg_cfg->txfunc = the_system->system_route_send2eth1_handle;//the_system->agg_enet_default_host_handle;//the_system->agg_enet_default_host_handle;//tx_enet_handle_1;
			bridge_iw_agg_cfg->prefix_length = 28;
			bridge_iw_agg_cfg->extraction_length = 2;	  /*rm fr header*/
			/*modify prefix and length*/
			/*
              brg_iw_flow_aggregation = WP_IwFlowAggregationCreate(0,WP_IW_DIRECTMAP_MODE, bridge_iw_agg_cfg);
              terminate_on_error(brg_iw_flow_aggregation, "WP_IwFlowAggregationCreate agg_queue");

              ch_info->agg_tx2Enet1 = brg_iw_flow_aggregation;
			*/
			status = WP_IwFlowAggregationModify(ch_info->agg_tx2Enet1,WP_IW_FLOW_AGG_MOD_PRX | WP_IW_FLOW_AGG_MOD_TXFUNC|WP_IW_FLOW_AGG_MOD_IWPORT,bridge_iw_agg_cfg);
			terminate_on_error(status, "WP_IwFlowAggregationModify");
			ch_info->agg_tx2Enet1ModifyFlag = 1;
		}

	    if(ch_info->agg_fr_to_enet2 == 0)
	    {
			np_ul_fr_to_enet2_create(ch_info);
		}
		flow_agg[0] = ch_info->agg_tx2Enet1;
		flow_agg[1] = ch_info->agg_fr_to_enet2;
		if(ch_info->agg_fr_rx_host_flow !=0)
		{
            WDDI_INFO("Info: np_ul_route_table_add::1, vfport:0x%x\n",uplink_rule->u_port.vfport);
			status = WP_LookupEntryInsert(ch_info->device_handle, ch_info->agg_fr_rx_host_flow, flow_agg, NRI, htons(uplink_rule->PortSrc)); /*use htons make sure portsrc-1024 is right ,portsrc > 1024*/
			terminate_on_error(status, "WP_LookupEntryInsert");
		}
	}
	else if(initial_cfg->OpterateMode == NPU_MODE_IP)
	{
	    WP_CHAR rule_string[128]={0};
		/*find dst MAC according dst ip address*/
		NP_ARP_INFO arp ={0};
		/*get dstip from link table ,key= uplink_rule->PortSrc*/
		ul_link_table = npu_get_ul_link_table_info(uplink_rule->PortSrc);
		if(ul_link_table == NULL)
		{
			WDDI_WARNING("Error: np_ul_route_table_add::npu_get_ul_link_table_info failed! uplink_rule->PortSrc:0x%x",uplink_rule->PortSrc);
			return ERR_UPLINX_FLOW_ADD;
		}
		if(ul_link_table->IpEpAcitveNum == 0)
		{
			WDDI_WARNING("Error: np_ul_route_table_add,ul_link_table->IpEpAcitveNum:0\n");
			return ERR_UPLINX_FLOW_ADD;
		}
		/*update mac address in route system*/
		{
			int ret;
			char ArrayBuf[10];
			WP_U32 IpDst = ul_link_table->IpEp[0].IpDst;
			/*select IpEp[0].IpDst to get arp*/
			ret = tap_socket_getarp(IpDst, ArrayBuf);
			if(ret == 0)/*success*/
			{
				arp.Ip = IpDst;
				memcpy(arp.Mac,ArrayBuf,6);
				/*just for display*/
				(void)npu_set_arp_info(IpDst,&arp);
				np_update_mac_in_route_system(&arp);
			}
			else
			{
				WDDI_WARNING("Error: np_ul_route_table_add npu_get_arp_info fail ipdst= 0x%x",IpDst);
				return ERR_UPLINX_FLOW_ADD;
			}
		}

        if(VportInfo->agg_ul_ip2iptx2Enet1 == 0)
        {
			/*IP header*/
			ipheader = (struct iphdr *)&bridge_iw_agg_cfg->prefix_header[0];

			ipheader->daddr=0; /*modify by dps according loadbalance*/
			ipheader->saddr=htonl(initial_cfg->ifIpaddr);

			udpheader = (struct udphdr *) (ipheader +1);
			udpheader->source = uplink_rule->PortSrc;/*modify by dps according loadbalance*/
			udpheader->dest = 0;  /*modify by dps according loadbalance*/

			bridge_iw_agg_cfg->iw_port = the_system->Rport_enet1_hanlde;
			bridge_iw_agg_cfg->txfunc = the_system->system_route_send2eth1_handle;
			bridge_iw_agg_cfg->prefix_length = 28;	      /*add ipv4header+udp header*/
			if(initial_cfg->VlanFlag == NPU_VLAN_ENABLE)
			{
			    bridge_iw_agg_cfg->extraction_length = 46;	  /*remove mac+ipv4header+udp heade*/
			}
			else
			{
			    bridge_iw_agg_cfg->extraction_length = 42;	  /*remove mac+ipv4header+udp heade*/
			}
			VportInfo->agg_ul_ip2iptx2Enet1 = WP_IwFlowAggregationCreate(0,WP_IW_DIRECTMAP_MODE,bridge_iw_agg_cfg);
			terminate_on_error(VportInfo->agg_ul_ip2iptx2Enet1, "WP_IwFlowAggregationCreate agg_ul_ip2iptx2Enet1");
		}
		else /*if created in the update bvci function,then just modify it */
		{
		    if(VportInfo->agg_ul_ip2iptx2Enet1ModifyFlag == 0)
		    {
				/*IP header*/
				ipheader = (struct iphdr *)&bridge_iw_agg_cfg->prefix_header[0];

				ipheader->daddr=0xdc21aa0a; /*modify by dps according loadbalance*/
				ipheader->saddr=htonl(initial_cfg->ifIpaddr);

				udpheader = (struct udphdr *) (ipheader +1);
				udpheader->source = uplink_rule->PortSrc;/*modify by dps according loadbalance*/
				udpheader->dest = 0;  /*modify by dps according loadbalance*/

				bridge_iw_agg_cfg->iw_port = the_system->Rport_enet1_hanlde;
				bridge_iw_agg_cfg->txfunc =the_system->system_route_send2eth1_handle;
				bridge_iw_agg_cfg->prefix_length = 28;		  /*add ipv4header+udp header*/
				if(initial_cfg->VlanFlag == NPU_VLAN_ENABLE)
				{
				    bridge_iw_agg_cfg->extraction_length = 46;	  /*remove mac+ipv4header+udp heade*/
				}
				else
				{
				    bridge_iw_agg_cfg->extraction_length = 42;	  /*remove mac+ipv4header+udp heade*/
				}
				status = WP_IwFlowAggregationModify(VportInfo->agg_ul_ip2iptx2Enet1,WP_IW_FLOW_AGG_MOD_PRX | WP_IW_FLOW_AGG_MOD_TXFUNC|WP_IW_FLOW_AGG_MOD_IWPORT,bridge_iw_agg_cfg);
				terminate_on_error(status, "WP_IwFlowAggregationModify");
				VportInfo->agg_ul_ip2iptx2Enet1ModifyFlag = 1;
			}

		}
        if(VportInfo->agg_ip2iptx2host == 0)
        {
			enet_iw_agg_cfg->l2_header_insert_mode = WP_IW_L2H_INSERT_DISABLE;
			enet_iw_agg_cfg->txfunc = the_system->agg_enet_default_host_handle;
			enet_iw_agg_cfg->mtu = 0xa6a6;
			enet_iw_agg_cfg->prefix_length = 0;
			enet_iw_agg_cfg->extraction_length = 0;
			enet_iw_agg_cfg->dynamic_length_update = WP_IW_LENGTH_UPDATE_DISABLED;
			VportInfo->agg_ip2iptx2host = WP_IwFlowAggregationCreate(0,WP_IW_DIRECTMAP_MODE, enet_iw_agg_cfg);
			terminate_on_error(VportInfo->agg_ip2iptx2host, "WP_IwFlowAggregationCreate agg_ip2iptx2host");

			enet_iw_agg_cfg->l2_header_insert_mode = WP_IW_L2H_INSERT_ENABLE;


        }
		if(VportInfo->flow_ul_tx2enet1 == 0)
		{

		    sprintf (rule_string, "n;n;n;n;n;n;%d;UDP;",htons((WP_U16)uplink_rule->u_port.portDst));
			enet_to_enet_flow_policy_cfg[NPU_FILTER_DPORT_UDP_IP_UL].classifier_string = rule_string;
			enet_to_enet_flow_policy_cfg[NPU_FILTER_DPORT_UDP_IP_UL].action_info.forwarding_action.flow_aggregation = VportInfo->agg_ip2iptx2host;
            enet_to_enet_flow_policy_cfg[NPU_FILTER_DPORT_UDP_IP_UL].dfc_info = &enet_to_enet_dfc_flow_info[NPU_FILTER_DPORT_UDP_IP_UL];
			enet_to_enet_flow_policy_cfg[NPU_FILTER_DPORT_UDP_IP_UL].match_action = WP_FLOW_CLASS_MODE_FLOW_AGG;

			VportInfo->flow_ul_tx2enet1 = WP_IwFlowAdd(iw_sys_bridging, WP_FLOW_CLASS_RULE,&(enet_to_enet_flow_policy_cfg[NPU_FILTER_DPORT_UDP_IP_UL]));
			terminate_on_error(VportInfo->flow_ul_tx2enet1, "WP_IwFlowAdd() enet_to_enet_iw_sys");
			status = WP_IwSystemBuild(iw_sys_bridging);
			terminate_on_error(status, "WP_IwSystemBuild - ip up flow rule");
		}


	    if(VportInfo->agg_ul_ip_to_enet2_handle == 0)
	    {
			np_ul_ip_forward_to_enet2_create(VportInfo);
		}
		flow_agg[0] = VportInfo->agg_ul_ip2iptx2Enet1;
		flow_agg[1] = VportInfo->agg_ul_ip_to_enet2_handle;
		if(VportInfo->agg_ip2iptx2host != 0)
		{
            WDDI_INFO("Info: np_ul_route_table_add::2, vfport:%d\n",uplink_rule->u_port.vfport);
			status = WP_LookupEntryInsert(0, VportInfo->agg_ip2iptx2host, flow_agg, NRI, htons(uplink_rule->PortSrc));
			terminate_on_error(status, "WP_LookupEntryInsert");
		}
	}
	else
	{
	    return ERR_UPLINX_FLOW_ADD;
	}
    flow_info.NRI = NRI;
	flow_info.vport = uplink_rule->u_port.vfport;
	flow_info.UsedFlag = USED;
	flow_info.portSrc = uplink_rule->PortSrc;
    /*record the flow infomation, so that it can be delete*/
    npu_set_uplink_info(&flow_info);
	u32 nrinum;
	for(nrinum = 0;nrinum< 257;nrinum++)
	{
	    if(VportInfo->nri_valid_talbe[nrinum]== 0)/*initial status*/
	    {
		    VportInfo->nri_valid_talbe[nrinum]=NRI+1;
		    break;
		}
		else
		{
            if(VportInfo->nri_valid_talbe[nrinum]== NRI+1)
            {
                break;/*NRI alread existed*/
            }
		}
	}

	return 0;

}

int np_ul_route_table_delete(WP_U32 vfport,WP_U32 NRIValue)
{

    WP_status status = WP_OK;
    NP_UL_FLOW_INFO * p_flow_info;
	STRU_SET_INITAIL_CONFIG *initial_cfg = npu_get_initial_cfg_info();
    //NP_SYSTEM_INFO * the_system = np_get_system_info();
    p_flow_info = npu_get_uplink_info(vfport, NRIValue);
    if(p_flow_info == NULL)
    {
        //WDDI_WARNING("can not find flow infomation to be deleted.");
		return ERR_UPLINX_FLOW_DELETE;
    }
	else if(p_flow_info->UsedFlag == UNUSED)
    {
        WDDI_WARNING("Warn: np_ul_route_table_delete::p_flow_info->UsedFlag:UNUSED! vfport:0x%x,NRIValue:%d\n",vfport,NRIValue);
		return ERR_UPLINX_FLOW_DELETE;
	}
    FR_CHANNEL_INFO * ch_info;
    STRU_VPORT_INFO  *VportInfo = npu_get_vport_info(p_flow_info->vport);
	if(VportInfo ==  NULL)
	{
	    WDDI_WARNING("Error: np_ul_route_table_delete::npu_get_vport_info failed! vfport:%d\n",p_flow_info->vport);
	    return ERR_UPLINX_FLOW_ADD;
	}
	else if(VportInfo->UsedFlag == UNUSED)
	{
		WDDI_WARNING("Error: np_ul_route_table_delete npu_get_vport_info, vport:0x%x unused!\n",vfport);
		return ERR_DOWNLINX_FLOW_ADD;
	}
    if(initial_cfg->OpterateMode == NPU_MODE_FR)
    {
		ch_info = &VportInfo->ch_info;
		status = WP_LookupEntryRemove(0, ch_info->agg_fr_rx_host_flow, NRIValue);
		terminate_on_error(status, "WP_LookupEntryRemove");
        /*remove agg flow resource etc.*/
		/*   */
		p_flow_info->UsedFlag = UNUSED;
	    npu_set_uplink_info(p_flow_info);
	    WDDI_INFO("Info: np_ul_route_table_delete::1, vfport:0x%x, NRIValue:%d\n",vfport,NRIValue);
    }
	else /*IP<-IP>*/
	{
		status = WP_LookupEntryRemove(0, VportInfo->agg_ip2iptx2host, NRIValue);
		terminate_on_error(status, "WP_LookupEntryRemove");
		p_flow_info->UsedFlag = UNUSED;
	    npu_set_uplink_info(p_flow_info);
	    WDDI_INFO("Info: np_ul_route_table_delete::2, portSrc:0x%x, NRIValue:%d\n",vfport,NRIValue);

	}
    return status;
}

#define WPI_VALID_BIT_MASK 0x80000000
int np_ul_link_table_set(STRU_SET_UPLINK_PATH_LINK_TABLE *link_table)
{
	u32 i=0,j=0;
    wp_local_rt local_rt = {0};
	local_rt.valid_num = link_table->IpEpAcitveNum;
	u32 LoacalArpfail =0;
    if(local_rt.valid_num == 0)
    {
		WP_IPEndPointEntryInsert(0,htons(link_table->PortSrc), &local_rt);
		WDDI_INFO("np_ul_link_table_set PortSrc = 0x%x linknum=%d\n",link_table->PortSrc,link_table->IpEpAcitveNum);
		npu_set_fr_ul_link_table_info(link_table);
        return 0;
	}

	/*set arp for IPEP dst*/
	{
		/*find dst MAC according dst ip address*/
		NP_ARP_INFO arp={0};
		int ret;
		char ArrayBuf[10];
		//WP_U32 IPtmp = 0xdc21aa0a;/*10.170.33.220*/;
		WP_U32 IpDst = link_table->IpEp[0].IpDst;
		/*select IpEp[0].IpDst to get arp*/
		ret = tap_socket_getarp(IpDst, ArrayBuf);
		if(ret == 0)/*success*/
		{
			arp.Ip = IpDst;
			memcpy(arp.Mac,ArrayBuf,6);
			/*just for display*/
			(void)npu_set_arp_info(IpDst,&arp);
			np_update_mac_in_route_system(&arp);
		}
		else
		{
			WDDI_WARNING("Warn: np_ul_link_table_set::tap_socket_getarp failed! ipdst= 0x%x\n",IpDst);
			//return ERR_UPLINX_FLOW_ADD;
			LoacalArpfail = 1;

		}
	}
	/*SPPSP-22  NPU don't redirect traffic to peer NPU when both NPUs have same number available link*/
	if(LoacalArpfail ==0)
	{
	    /*just add remote */
		for(i = 0;i< local_rt.valid_num ;i++)
		{
			if(link_table->IpEp[i].Flag == LOCAL_FLAG)
			{
				j++;
			}
			local_rt.ip_ep[i].ip_addr  =  htonl(link_table->IpEp[i].IpDst);
			local_rt.ip_ep[i].dst_port = htons(link_table->IpEp[i].PortDst);
			//local_rt.ip_ep[i].src_port = uplinkTable->IpEp[i].Flag;
			//local_rt.ip_ep[i].src_port = htons(link_table->PortSrc);
			local_rt.ip_ep[i].src_port = link_table->IpEp[i].Flag;//(16);//must
		}
		if(j == (local_rt.valid_num - j))
		{
		    if(j==0)
		    {
				local_rt.valid_num =0;

		    }
			else
			{
				local_rt.valid_num |= WPI_VALID_BIT_MASK;

			}
		}
		local_rt.valid_num |= (j<<16);

	}
	else
	{
		STRU_SET_UPLINK_PATH_LINK_TABLE link_table_tmp ;
		memset(&link_table_tmp,0,sizeof(link_table_tmp));
		link_table_tmp.PortSrc = link_table->PortSrc;
		for(i = 0;i< local_rt.valid_num ;i++)
		{
		    if(link_table->IpEp[i].Flag == LOCAL_FLAG)
		    {
                continue;
			}
			/*PEER_FLAG*/
			local_rt.ip_ep[j].ip_addr  =  htonl(link_table->IpEp[i].IpDst);
			local_rt.ip_ep[j].dst_port = htons(link_table->IpEp[i].PortDst);
			//local_rt.ip_ep[i].src_port = uplinkTable->IpEp[i].Flag;
			//local_rt.ip_ep[i].src_port = htons(link_table->PortSrc);
			local_rt.ip_ep[j].src_port = link_table->IpEp[i].Flag;//(16);//must

			link_table_tmp.IpEpAcitveNum = j+1;
			link_table_tmp.IpEp[j].Flag = link_table->IpEp[i].Flag;
			link_table_tmp.IpEp[j].IpDst = link_table->IpEp[i].IpDst;
			link_table_tmp.IpEp[j].PortDst = link_table->IpEp[i].PortDst;
			j++;
		}
		local_rt.valid_num = j;
		if(local_rt.valid_num == 0)
		{
			WDDI_WARNING("Warn: np_ul_link_table_set, no PEER link configured!");
			return ERR_UPLINX_FLOW_ADD;
		}
		WP_IPEndPointEntryInsert(0,htons(link_table_tmp.PortSrc), &local_rt);
		WDDI_INFO("Info: np_ul_link_table_set only peer,PortSrc = 0x%x,linknum=%d\n",link_table_tmp.PortSrc,link_table_tmp.IpEpAcitveNum);
		npu_set_fr_ul_link_table_info(&link_table_tmp);
        return ERR_UPLINX_FLOW_ADD_ARP_FAIL;
	}
    WP_IPEndPointEntryInsert(0,htons(link_table->PortSrc), &local_rt);
	WDDI_INFO("Info: np_ul_link_table_set, PortSrc = 0x%x,IpEpAcitveNum=%d\n",link_table->PortSrc,link_table->IpEpAcitveNum);
	npu_set_fr_ul_link_table_info(link_table);
    return 0;
}


/*called when vport is closed */
int np_ul_release_vport_resource(u32 vport)
{
	WP_S32 status;
	STRU_VPORT_INFO  *VportInfo;
	FR_CHANNEL_INFO * ch_info;
	VportInfo = npu_get_vport_info(vport);

	if(VportInfo == NULL)
	{
		WDDI_WARNING("Error: np_ul_release_resource::npu_get_vport_info fail! vport:0x%x\n",vport);
		return ERR_UL_RELEASE_RESOURCE;
	}
	else if(VportInfo->UsedFlag == UNUSED)
	{
		WDDI_WARNING("Error: np_ul_release_resource::npu_get_vport_info, vport:0x%x unused!\n",vport);
		return ERR_UL_RELEASE_RESOURCE;
	}

	STRU_SET_INITAIL_CONFIG *initial_cfg = npu_get_initial_cfg_info();
	if(initial_cfg->OpterateMode == NPU_MODE_FR)
	{
        ch_info = &VportInfo->ch_info;
        if(ch_info->agg_tx2Enet1 !=0 )
        {
            status = WP_IwFlowAggregationDelete(ch_info->agg_tx2Enet1);
            terminate_on_error(status, "WP_IwFlowAggregationDelete - agg_tx2Enet1 ");
            ch_info->agg_tx2Enet1 = 0;
            ch_info->agg_tx2Enet1ModifyFlag = 0;
        }
        if(ch_info->agg_fr_to_enet2 !=0 )
        {
            status = WP_IwFlowAggregationDelete(ch_info->agg_fr_to_enet2);
            terminate_on_error(status, "WP_IwFlowAggregationDelete - agg_fr_to_enet2 ");
            ch_info->agg_fr_to_enet2 = 0;
        }
        WDDI_INFO("Info: np_ul_release_resource::1, vport:0x%x\n",vport);
	}
	else
	{
	    if(VportInfo->agg_ul_ip2iptx2Enet1 !=0 )
	    {
			status = WP_IwFlowAggregationDelete(VportInfo->agg_ul_ip2iptx2Enet1);
			terminate_on_error(status, "WP_IwFlowAggregationDelete - agg_ul_ip2iptx2Enet1 ");
			VportInfo->agg_ul_ip2iptx2Enet1 = 0;
			VportInfo->agg_ul_ip2iptx2Enet1ModifyFlag = 0;
		}
	    if(VportInfo->agg_ul_ip_to_enet2_handle !=0 )
	    {
			status = WP_IwFlowAggregationDelete(VportInfo->agg_ul_ip_to_enet2_handle);
			terminate_on_error(status, "WP_IwFlowAggregationDelete - agg_ul_ip_to_enet2_handle ");
			VportInfo->agg_ul_ip_to_enet2_handle = 0;
		}

        if(VportInfo->flow_ul_tx2enet1 != 0)
        {
			status = WP_IwFlowDisable(iw_sys_bridging, VportInfo->flow_ul_tx2enet1);
			terminate_on_error(status, "WP_IwFlowDisable() VportInfo->flow_ul_tx2enet1");
			status = WP_IwFlowDelete(iw_sys_bridging, VportInfo->flow_ul_tx2enet1);
			terminate_on_error(status, "WP_IwFlowDelete() flow_ul_tx2enet1");
			status = WP_IwSystemBuild(iw_sys_bridging);
			terminate_on_error(status, "WP_IwSystemBuild - delete rule");
			VportInfo->flow_ul_tx2enet1 = 0;
		}
#if 0 /*agg_ip2iptx2host connect to default host ,don't delete it*/
		if(VportInfo->agg_ip2iptx2host !=0 )
		{
			status = WP_IwFlowAggregationDelete(VportInfo->agg_ip2iptx2host);
			terminate_on_error(status, "WP_IwFlowAggregationDelete - agg_ip2iptx2host ");
			VportInfo->agg_ip2iptx2host = 0;
		}
#endif
        WDDI_INFO("Info: np_ul_release_resource::2, vport:0x%x\n",vport);
	}
	return 0;

}

/*called when vport is closed */
int np_dl_release_vport_resource(u32 vport)
{
	WP_S32 status;
	STRU_VPORT_INFO  *VportInfo;
	FR_CHANNEL_INFO * ch_info;
	VportInfo = npu_get_vport_info(vport);
	//NP_SYSTEM_INFO * the_system = np_get_system_info();
	if(VportInfo == NULL)
	{
		WDDI_WARNING("Error: np_dl_release_resource::npu_get_vport_info fail! vport:0x%x\n",vport);
		return ERR_DL_RELEASE_RESOURCE;
	}
	else if(VportInfo->UsedFlag == UNUSED)
	{
		WDDI_WARNING("Error: np_dl_release_resource::npu_get_vport_info, vport:0x%x unused!\n",vport);
		return ERR_DL_RELEASE_RESOURCE;
	}

	STRU_SET_INITAIL_CONFIG *initial_cfg = npu_get_initial_cfg_info();
	if(initial_cfg->OpterateMode == NPU_MODE_FR)
	{
	    ch_info = &VportInfo->ch_info;
        if(ch_info->flow_enet2_to_fr != 0)
        {
			status = WP_IwFlowDisable(iw_sys_bridging, ch_info->flow_enet2_to_fr);
			terminate_on_error(status, "WP_IwFlowDisable() ch_info->flow_enet2_to_fr");
			status = WP_IwFlowDelete(iw_sys_bridging, ch_info->flow_enet2_to_fr);
			terminate_on_error(status, "WP_IwFlowDelete() ch_info->flow_enet2_to_fr");
			status = WP_IwSystemBuild(iw_sys_bridging);
			terminate_on_error(status, "WP_IwSystemBuild - delete flow_enet2_to_fr");
			ch_info->flow_enet2_to_fr = 0;
		}
		if(ch_info->agg_downlink_tx2fr_device != 0)
		{
			status = WP_IwFlowAggregationDelete(ch_info->agg_downlink_tx2fr_device);
			terminate_on_error(status, "WP_IwFlowAggregationDelete - agg_downlink_tx2fr_device ");
			ch_info->agg_downlink_tx2fr_device = 0;
		}

		if(ch_info->agg_enet2_to_fr != 0)
		{
			status = WP_IwFlowAggregationDelete(ch_info->agg_enet2_to_fr);
			terminate_on_error(status, "WP_IwFlowAggregationDelete - agg_enet2_to_fr ");
			ch_info->agg_enet2_to_fr = 0;
		}
        WDDI_INFO("Info: np_dl_release_resource::1, vport:0x%x\n",vport);
	}
	else
	{
	    if(VportInfo->agg_dl_ip2iptx2Enet1 !=0 )
	    {
			status = WP_IwFlowAggregationDelete(VportInfo->agg_dl_ip2iptx2Enet1);
			terminate_on_error(status, "WP_IwFlowAggregationDelete - agg_dl_ip2iptx2Enet1 ");
			VportInfo->agg_dl_ip2iptx2Enet1 = 0;
		}
        WDDI_INFO("Info: np_dl_release_resource::2, vport:0x%x\n",vport);
	}

	return 0;
}
int np_ul_ip_forward_to_enet2_create(STRU_VPORT_INFO  *VportInfo)
{
	WP_iw_agg_directmap tx_agg_enet[1] =
        {
            {
                /* tag */ 				 0,
                /* txfunc */				 0,
                /* rfcs */				 WP_IW_RFCS_DISABLE,
                /* l2header_insert_mode;*/ WP_IW_L2H_INSERT_DISABLE,
                /* interruptqueue;*/		 WP_IW_IRQT0,
                /* error_pkt_mode*/		 WP_IW_ERRPKT_DISCARD,
                /* intmode;*/ 			 WP_IW_INT_DISABLE,
#if STATISTICS_ENABLE
                /* statmode;*/			 WP_IW_STAT_ENABLE,
#else
                /* statmode;*/			 WP_IW_STAT_DISABLE,
#endif
                /* timestamp_mode;*/		 WP_IW_TIME_STAMP_DISABLE,
                /* ov_pool_mode*/ 		 0,
                /* fbp_drop_threshold;*/	 0,
                /* mtu;*/ 				 1536,
                /* prefix_length */		 0, /* MAC header with VLAN */
                /* extraction_length */	 0,
                /* prefix_header[68];*/	 {	0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00,
                                            0x00, 0x00, 0x00, 0x04, 0x81, 0x00, 0x00, 0x00, 0x08, 0x00,
                                            0x45, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x40, 0x00,
                                            0x40, 0x11, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x02,
                                            0x00, 0x00, 0x00, 0x01, 0xff},
                /* policer_enable */		 0,
                /* policer_config */		 NULL,
                /* cong_mode */			 0,
                /* cong_threshold_param */ NULL,
                /* dynamic_length_update*/ WP_IW_LENGTH_UPDATE_DISABLED,
                /* length_offset*/		 0
            }
        };

    STRU_SET_INITAIL_CONFIG *initial_cfg = npu_get_initial_cfg_info();
    struct vlan_ethhdr * vlan_ethheader;
    struct iphdr *ipheader;
    struct udphdr *udpheader;
    /*MAC header*/
    vlan_ethheader = (struct vlan_ethhdr *)&tx_agg_enet->prefix_header[0];
    //memcpy(&vlan_ethheader->h_dest[0],	arp.Mac, 6);
    memset(&vlan_ethheader->h_dest[0],	0xfe, 6);
    //memcpy(&vlan_ethheader->h_dest[0],	dest_mac, 6);
    memcpy(&vlan_ethheader->h_source[0], initial_cfg->ifMACsrc, 6);

    vlan_ethheader->h_vlan_proto = htons(0x8100);
    vlan_ethheader->h_vlan_TCI = htons(BPORT_DEFAULT_VLAN_TAG);
    vlan_ethheader->h_vlan_encapsulated_proto = htons(0x0800);

    /*IP header*/
    ipheader = (struct iphdr *)(vlan_ethheader + 1);

    ipheader->daddr=0;
    ipheader->saddr=htonl(0x00000001);/*1.1.1.1*/
    udpheader = (struct udphdr *) (ipheader +1);
    udpheader->source = 0;
    udpheader->dest = 0;
    //NP_SYSTEM_INFO * the_system = np_get_system_info();
    tx_agg_enet->txfunc = agg_enet_2;//agg_enet_1;
    tx_agg_enet->prefix_length = 46;
    if(initial_cfg->VlanFlag == NPU_VLAN_ENABLE)
    {
        tx_agg_enet->extraction_length = 46;
    }
    else
    {
        tx_agg_enet->extraction_length = 42;
    }
    tx_agg_enet->dynamic_length_update = WP_IW_LENGTH_UPDATE_L3;
    tx_agg_enet->length_offset = 18;
    tx_agg_enet->l2_header_insert_mode = WP_IW_L2H_INSERT_ENABLE;
    VportInfo->agg_ul_ip_to_enet2_handle = WP_IwFlowAggregationCreate(0,WP_IW_DIRECTMAP_MODE, tx_agg_enet);
    terminate_on_error(VportInfo->agg_ul_ip_to_enet2_handle, "WP_IwFlowAggregationCreate VportInfo->agg_ul_ip_to_enet2_handle");
    WDDI_INFO("Info: np_ul_ip_forward_to_enet2_create, VportInfo->agg_ul_ip_to_enet2_handle:0x%x\n",VportInfo->agg_ul_ip_to_enet2_handle);
    // Fprintf("ch_info->agg_fr_rx_host_flow = %d\n", WPI_HANDLE_INDEX(ch_info->agg_fr_rx_host_flow));

    return 0;

}

int np_dl_ip_forward_to_enet2_create(void)
{
	WP_iw_agg_directmap tx_agg_enet[1] =
        {
            {
                /* tag */ 				 0,
                /* txfunc */				 0,
                /* rfcs */				 WP_IW_RFCS_DISABLE,
                /* l2header_insert_mode;*/ WP_IW_L2H_INSERT_DISABLE,
                /* interruptqueue;*/		 WP_IW_IRQT0,
                /* error_pkt_mode*/		 WP_IW_ERRPKT_DISCARD,
                /* intmode;*/ 			 WP_IW_INT_DISABLE,
#if STATISTICS_ENABLE
                /* statmode;*/			 WP_IW_STAT_ENABLE,
#else
                /* statmode;*/			 WP_IW_STAT_DISABLE,
#endif
                /* timestamp_mode;*/		 WP_IW_TIME_STAMP_DISABLE,
                /* ov_pool_mode*/ 		 0,
                /* fbp_drop_threshold;*/	 0,
                /* mtu;*/ 				 1536,
                /* prefix_length */		 0, /* MAC header with VLAN */
                /* extraction_length */	 0,
                /* prefix_header[68];*/	 {	0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00,
                                            0x00, 0x00, 0x00, 0x04, 0x81, 0x00, 0x00, 0x00, 0x08, 0x00,
                                            0x45, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x40, 0x00,
                                            0x40, 0x11, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x02,
                                            0x00, 0x00, 0x00, 0x01, 0xff},
                /* policer_enable */		 0,
                /* policer_config */		 NULL,
                /* cong_mode */			 0,
                /* cong_threshold_param */ NULL,
                /* dynamic_length_update*/ WP_IW_LENGTH_UPDATE_DISABLED,
                /* length_offset*/		 0
            }
        };


    STRU_SET_INITAIL_CONFIG *initial_cfg = npu_get_initial_cfg_info();
    NP_SYSTEM_INFO * the_system = np_get_system_info();
    struct vlan_ethhdr * vlan_ethheader;
    struct iphdr *ipheader;
    struct udphdr *udpheader;
    /*MAC header*/
    vlan_ethheader = (struct vlan_ethhdr *)&tx_agg_enet->prefix_header[0];
    //memcpy(&vlan_ethheader->h_dest[0],	arp.Mac, 6);
    memset(&vlan_ethheader->h_dest[0],	0xfe, 6);
    //memcpy(&vlan_ethheader->h_dest[0],	dest_mac, 6);
    memcpy(&vlan_ethheader->h_source[0], initial_cfg->ifMACsrc, 6);

    vlan_ethheader->h_vlan_proto = htons(0x8100);
    //vlan_ethheader->h_vlan_TCI = htons(initial_cfg->VlanTag);
    vlan_ethheader->h_vlan_TCI = htons(BPORT_DEFAULT_VLAN_TAG);
    vlan_ethheader->h_vlan_encapsulated_proto = htons(0x0800);

    /*IP header*/
    ipheader = (struct iphdr *)(vlan_ethheader + 1);

    ipheader->daddr=0;
    ipheader->saddr=htonl(0x00000001);/*1.1.1.1*/
    udpheader = (struct udphdr *) (ipheader +1);
    udpheader->source = 0;
    udpheader->dest = 0;

    tx_agg_enet->txfunc = agg_enet_2;//agg_enet_1;
    tx_agg_enet->prefix_length = 46;
    if(initial_cfg->VlanFlag == NPU_VLAN_ENABLE)
    {
        tx_agg_enet->extraction_length = 46;
    }
    else
    {
        tx_agg_enet->extraction_length = 42;
    }
    tx_agg_enet->dynamic_length_update = WP_IW_LENGTH_UPDATE_L3;
    tx_agg_enet->length_offset = 18;
    tx_agg_enet->l2_header_insert_mode = WP_IW_L2H_INSERT_ENABLE;
    the_system->agg_dl_ip_to_enet2_handle = WP_IwFlowAggregationCreate(0,WP_IW_DIRECTMAP_MODE, tx_agg_enet);
    terminate_on_error(the_system->agg_dl_ip_to_enet2_handle, "WP_IwFlowAggregationCreate the_system->agg_dl_ip_to_enet2_handle");
    // Fprintf("ch_info->agg_fr_rx_host_flow = %d\n", WPI_HANDLE_INDEX(ch_info->agg_fr_rx_host_flow));
    WDDI_INFO("Info: np_dl_ip_forward_to_enet2_create, the_system->agg_dl_ip_to_enet2_handle:0x%x\n",the_system->agg_dl_ip_to_enet2_handle);
    return 0;

}

WP_iw_agg_directmap fr_to_enet2_agg_cfg [1] = {

    {
        /*WP_tag tag;*/ 0,
        /*WP_handle txfunc;*/   0,
        /*WP_U8 rfcs;*/ WP_IW_RFCS_DISABLE,
        /*WP_U8 l2_header_insert_mode;*/ /* 10*/ WP_IW_L2H_INSERT_ENABLE,
        /*WP_U8 interruptqueue;*/ WP_IW_L2H_INSERT_DISABLE,
        /*WP_U8 error_pkt_mode;*/ WP_IW_ERRPKT_DISCARD,
        /*WP_U8 intmode;*/ WP_IW_INT_DISABLE,
        /*WP_U8 statmode;*/ WP_IW_STAT_ENABLE,
        /*WP_U8 timestamp_mode;*/ WP_IW_TIME_STAMP_DISABLE,
        /*WP_U8 ov_pool_mode;*/ WP_IW_OV_POOL_DISABLE,
        /*WP_U16 fbp_drop_threshold;*/ 0,
        /*WP_U16 mtu; */ /*20 */ 1536,
        /*WP_U8 prefix_length;*/ 46,
        /*WP_U8 extraction_length;*/ 46,
        /*WP_U8 prefix_header[68];*/
        {
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x45, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x40, 0x00, 0x40, 0x11, 0x00, 0x00, 0x00, 0x00, /*ip frage flag 0x40*/
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00
        },
        /*WP_U8 policer_enable;*/ WP_IW_POLICER_DISABLE,
        /*WP_policer_config   *policer_config  */ /*96*/ NULL,
        /*WP_U8 cong_mode;*/ WP_IW_CONGESTION_DISABLE,
        /*WP_iw_flow_agg_cong_thresh_param *cong_threshold_param;*/ NULL,
        /*WP_U8 dynamic_length_update;*/ WP_IW_LENGTH_UPDATE_L3,
        /*WP_U8 length_offset;*/ 18,
        /*WP_flowaggtype    flow_agg_type;*/ WP_IW_FLOW_AGG_PRIMARY,
        /*WP_handle comp_group_handle;*/ 0
    }
};


int np_ul_fr_to_enet2_create(FR_CHANNEL_INFO * ch_info)
{
    STRU_SET_INITAIL_CONFIG *initial_cfg = npu_get_initial_cfg_info();
    //NP_SYSTEM_INFO * the_system = np_get_system_info();
    struct vlan_ethhdr * vlan_ethheader;
    struct iphdr *ipheader;
    struct udphdr *udpheader;
    //WP_U32 status;
    /*MAC header*/
    vlan_ethheader = (struct vlan_ethhdr *)&fr_to_enet2_agg_cfg->prefix_header[0];
    //memcpy(&vlan_ethheader->h_dest[0],	arp.Mac, 6);
    memset(&vlan_ethheader->h_dest[0],	0xfe, 6);
    //memcpy(&vlan_ethheader->h_dest[0],	dest_mac, 6);
    memcpy(&vlan_ethheader->h_source[0], initial_cfg->ifMACsrc, 6);

    vlan_ethheader->h_vlan_proto = htons(0x8100);
    vlan_ethheader->h_vlan_TCI = htons(initial_cfg->VlanTag);
    vlan_ethheader->h_vlan_TCI = htons(BPORT_DEFAULT_VLAN_TAG);
    vlan_ethheader->h_vlan_encapsulated_proto = htons(0x0800);

    /*IP header*/
    ipheader = (struct iphdr *)(vlan_ethheader + 1);

    ipheader->daddr=0;
    ipheader->saddr=htonl(0x00000001);

    udpheader = (struct udphdr *) (ipheader +1);
    udpheader->source = 0;
    udpheader->dest = 0;

    //enet_iw_agg_cfg->l2_header_insert_mode = WP_IW_L2H_INSERT_ENABLE;
    fr_to_enet2_agg_cfg->mtu = N_MAX_IP_FRAME_SIZE;
    fr_to_enet2_agg_cfg->txfunc = agg_enet_2;//agg_enet_2;
    fr_to_enet2_agg_cfg->prefix_length = 46;
    fr_to_enet2_agg_cfg->extraction_length = 2;
    ch_info->agg_fr_to_enet2= WP_IwFlowAggregationCreate(0,WP_IW_DIRECTMAP_MODE, fr_to_enet2_agg_cfg);
    terminate_on_error(ch_info->agg_fr_to_enet2, "WP_IwFlowAggregationCreate ch_info->agg_fr_to_enet2");
    WDDI_INFO("Info: np_ul_fr_to_enet2_create, ch_info->agg_fr_to_enet2:0x%x\n",ch_info->agg_fr_to_enet2);

    // Fprintf("ch_info->agg_fr_rx_host_flow = %d\n", WPI_HANDLE_INDEX(ch_info->agg_fr_rx_host_flow));
    return 0;

}
int np_enet2_to_enet1_rule_add(void)
{

	WP_iw_agg_directmap_enet enet_to_enetiw_agg_cfg[1] =
        {
            {
                /* tag */ 				 0,
                /* txfunc */				 0,
                /* rfcs */				 WP_IW_RFCS_DISABLE,
                /* l2header_insert_mode;*/ WP_IW_L2H_INSERT_ENABLE,
                /* interruptqueue;*/		 WP_IW_IRQT1,
                /* error_pkt_mode*/		 WP_IW_ERRPKT_DISCARD,
                /* intmode;*/ 			 WP_IW_INT_DISABLE,
#if STATISTICS_ENABLE
                /* statmode;*/			 WP_IW_STAT_ENABLE,
#else
                /* statmode;*/			 WP_IW_STAT_DISABLE,
#endif
                /* timestamp_mode;*/		 WP_IW_TIME_STAMP_DISABLE,
                /* ov_pool_mode*/ 		 0,
                /* fbp_drop_threshold;*/	 0,
                /* mtu;*/ 				 1536,
                /* prefix_length */		 /*34*/ 0,
                /* extraction_length */	 18,
                /* prefix_header[68];*/	 {
                    /*0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x01,
                      0x02, 0x03, 0x04, 0x05, 0x81, 0x00, 0x00, 0x85,
                      0x08, 0x00,*/ 0x45, 0x00, 0x00, 0x28, 0x00, 0x04,
                      0x40, 0x00, 0x40, 0x11, 0x0e, 0x3f, 0x00, 0x00,
                      0x00, 0x00, 0x0a, 0xaa, 0x21, 0xdc, 0x00, 0x00,
                      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                      0x00, 0x00, 0x00, 0x00
                },
                /* policer_enable */		 0,
                /* policer_config */		 NULL,
                /* cong_mode */			 0,
                /* cong_threshold_param */ NULL,
                /*WP_U8 dynamic_length_update;*/ WP_IW_LENGTH_UPDATE_L3,
                /*WP_U8 length_offset;*/ 0,
                WP_IW_FLOW_AGG_PRIMARY
            }
        };

    WP_S32 status;
    WP_handle flow_handle;
    NP_SYSTEM_INFO * the_system = np_get_system_info();
    //STRU_SET_INITAIL_CONFIG *initial_cfg = npu_get_initial_cfg_info();
    WP_handle enet_iw_flow_aggregation/*,tx_agg_ipfrag,ip_frag_group*/;
    WP_CHAR enet_rule_string[256]={0};
#if 0
    struct vlan_ethhdr * enetheader;

    //struct udphdr * udpheader;

    /*MAC header*/
    enetheader = (struct vlan_ethhdr *)&tx_agg_l3->prefix_header[0];
    //memcpy(&enetheader->h_dest[0],	arp.Mac, 6);
    memset(&enetheader->h_dest[0],	0xff, 6);
    //memcpy(&vlan_ethheader->h_dest[0],	dest_mac, 6);
    //memcpy(&enetheader->h_source[0], initial_cfg->ifMACsrc, 6);
    memset(&enetheader->h_source[0],	0xff, 6);
    enetheader->h_vlan_proto = htons(0x8100);
    enetheader->h_vlan_TCI = htons(initial_cfg->VlanTag);
    enetheader->h_vlan_encapsulated_proto = htons(0x0800);
    tx_agg_l3->txfunc = tx_enet_handle_1;
    tx_agg_ipfrag = WP_IwFlowAggregationCreate(0, WP_IW_ROUTING_MODE,tx_agg_l3);
    terminate_on_error(tx_agg_ipfrag, "enet2_to_enet1 WP_IwFlowAggregationCreate");
#endif
	enet_to_enetiw_agg_cfg->rfcs = WP_IW_RFCS_ENABLE;
    enet_to_enetiw_agg_cfg->iw_port = the_system->Rport_enet1_hanlde;
    enet_to_enetiw_agg_cfg->l2_header_insert_mode = WP_IW_L2H_INSERT_DISABLE;/*prefix_length = 0,l2_header_insert_mode must be WP_IW_L2H_INSERT_DISABLE*/
    enet_to_enetiw_agg_cfg->txfunc = the_system->system_route_send2eth1_handle;//the_system->agg_enet_default_host_handle;//agg_enet_1;
    enet_to_enetiw_agg_cfg->mtu = N_MAX_IP_FRAME_SIZE; /*can receive big packet*/
    enet_to_enetiw_agg_cfg->prefix_length = 0;
    enet_to_enetiw_agg_cfg->extraction_length = 18; /*remain ipdst,just replace ipsrc*/
	enet_to_enetiw_agg_cfg->dynamic_length_update = WP_IW_LENGTH_UPDATE_L3;/*disabled when WP_IW_L2H_INSERT_DISABLE*/
	enet_to_enetiw_agg_cfg->length_offset = 0;
	enet_iw_flow_aggregation = WP_IwFlowAggregationCreate(0,WP_IW_DIRECTMAP_MODE, &enet_to_enetiw_agg_cfg[0]);
	terminate_on_error(enet_iw_flow_aggregation, "WP_IwFlowAggregationCreate agg_queue");
    the_system->agg_enet2_to_enet1 = enet_iw_flow_aggregation;
	//sprintf (enet_rule_string, "n;n;n;n;n;n;n;UDP;");
	/*sprintf(enet_rule_string, "%d.%d/255.255;%d.%d/255.255;n;n;n;n;n;n;n",
      ipaddress[3],ipaddress[2],ipaddress[1], ipaddress[0]);*/
	//sprintf(enet_rule_string, "10.170/255.255;33.220/255.255;n;n;n;n;n;n;n");
	sprintf(enet_rule_string, "0.0/255.255;0.1/255.255;n;n;n;n;n;UDP;");

	enet_to_enet_flow_policy_cfg[NPU_FILTER_SIP_UDP_PEER].classifier_string = enet_rule_string;
	enet_to_enet_flow_policy_cfg[NPU_FILTER_SIP_UDP_PEER].action_info.forwarding_action.flow_aggregation = enet_iw_flow_aggregation;//enet_iw_flow_aggregation;
    enet_to_enet_flow_policy_cfg[NPU_FILTER_SIP_UDP_PEER].dfc_info = &enet_to_enet_dfc_flow_info[NPU_FILTER_SIP_UDP_PEER];
    enet_to_enet_flow_policy_cfg[NPU_FILTER_SIP_UDP_PEER].match_action = WP_FLOW_CLASS_MODE_FLOW_AGG;

	flow_handle = WP_IwFlowAdd(iw_sys_bridging, WP_FLOW_CLASS_RULE,&(enet_to_enet_flow_policy_cfg[NPU_FILTER_SIP_UDP_PEER]));
	terminate_on_error(flow_handle, "WP_IwFlowAdd() enet_to_enet_iw_sys");
	status = WP_IwSystemBuild(iw_sys_bridging);
	terminate_on_error(status, "WP_IwSystemBuild - ip up flow rule");

	return 0;
}

//add mac and source ip, then send to directly to interface
int np_dl_enet2_to_fr_rule_add(FR_CHANNEL_INFO * ch_info)
{
    WP_S32 status;
    WP_handle flow_handle;
    //NP_SYSTEM_INFO * the_system = np_get_system_info();
    //STRU_SET_INITAIL_CONFIG *initial_cfg = npu_get_initial_cfg_info();
    WP_handle enet_iw_flow_aggregation;
    WP_CHAR enet_rule_string[256]={0};
    WP_U8 * ipaddress;


#if ENEA_RELEASE
    enet_iw_agg_cfg->txfunc =ch_info->tx_iw_handle;// iw_rx_host_handle;//ch_info->rx_host_handle;//ch_info->tx_iw_handle;
#else/*for ENEA DEBUG*/
    enet_iw_agg_cfg->txfunc = ch_info->agg_fr_rx_host_flow;//ch_info->tx_iw_handle;
#endif
    enet_iw_agg_cfg->l2_header_insert_mode = WP_IW_L2H_INSERT_ENABLE;
    enet_iw_agg_cfg->dynamic_length_update = WP_IW_LENGTH_UPDATE_DISABLED;
    enet_iw_agg_cfg->prefix_length = 2;
    enet_iw_agg_cfg->extraction_length = 46;

    WP_FR_HEADER_2OCT_SET(enet_iw_agg_cfg->prefix_header,ch_info->dlci,0,0,0,0);

    enet_iw_flow_aggregation = WP_IwFlowAggregationCreate(0,WP_IW_DIRECTMAP_MODE, enet_iw_agg_cfg);
    terminate_on_error(enet_iw_flow_aggregation, "WP_IwFlowAggregationCreate agg_downlink_tx2fr_device");
    ch_info->agg_enet2_to_fr= enet_iw_flow_aggregation;

    ipaddress = (WP_U8 *) &ch_info->vfport;
    sprintf(enet_rule_string, "%d.%d/255.255;%d.%d/255.255;n;n;n;n;n;UDP;",
            ipaddress[3],ipaddress[2],ipaddress[1], ipaddress[0]);

	//sprintf(enet_rule_string, "10.170/255.255;33.220/255.255;n;n;n;n;n;n;n");
    //Fprintf(enet_rule_string);

	enet_to_enet_flow_policy_cfg[NPU_FILTER_SIP_UDP_PEER].classifier_string = enet_rule_string;
	enet_to_enet_flow_policy_cfg[NPU_FILTER_SIP_UDP_PEER].action_info.forwarding_action.flow_aggregation = enet_iw_flow_aggregation;
    enet_to_enet_flow_policy_cfg[NPU_FILTER_SIP_UDP_PEER].dfc_info = &enet_to_enet_dfc_flow_info[NPU_FILTER_SIP_UDP_PEER];
    enet_to_enet_flow_policy_cfg[NPU_FILTER_SIP_UDP_PEER].match_action = WP_FLOW_CLASS_MODE_FLOW_AGG;
	flow_handle = WP_IwFlowAdd(iw_sys_bridging, WP_FLOW_CLASS_RULE,&(enet_to_enet_flow_policy_cfg[NPU_FILTER_SIP_UDP_PEER]));
	terminate_on_error(flow_handle, "WP_IwFlowAdd() enet2_to_fr");
	ch_info->flow_enet2_to_fr = flow_handle;
	status = WP_IwSystemBuild(iw_sys_bridging);
	terminate_on_error(status, "WP_IwSystemBuild - enet2_to_fr");
    WDDI_INFO("Info: np_dl_enet2_to_fr_rule_add, ch_info->flow_enet2_to_fr:0x%x\n",flow_handle);
	return 0;
}

extern struct NODE* arp_nodehead;
extern struct hsearch_data *arp_ht;

void np_reconfig_vlan_in_routeSystem(void)
{
	WP_iw_agg_routing tx_route_agg[1] =
        {
            {
                /* tag */ 				 0,
                /* txfunc */				 0,
                /* dfsrv_remarking */ 	 WP_IW_DSRV_DISABLE,
                /* l2header_insert_mode */ WP_IW_L2H_INSERT_ENABLE,
                /* siphrm */				 WP_IW_FAST_FWD_MODE,
                /* interruptqueue */		 WP_IW_IRQT1,
                /* error_pkt_mode */		 0,
                /* intmode */ 			 WP_IW_INT_DISABLE,
                /* statmode */			 WP_IW_STAT_ENABLE,
                /* timestamp_mode */		 WP_IW_TIME_STAMP_DISABLE,
                /* ov_pool_mode */		 0,
                /* fbp_drop_threshold */	 0,
                /* mtu */ 			   1536,//FRAG_SIZE,
                /* dfsrv_code_point */	 0,
                /* prefix_length */		 18,
                /* multi_cong_prof */ 	 0,
                /* prefix_header[18] */	 {0x00, 0x23, 0x24, 0x1f, 0x54, 0x15, 0x00, 0x01,
                                          0x02, 0x03, 0x04, 0x05,0x81,0x00,0x00,0x85,0x08,0x00},
                /* new_ip_header[20] */	 {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                          0x00, 0x00, 0x00, 0x00},
                /* policer_config */		  0,
                /* maxt */				 0,
                /* cong_threshold_param */ 0,
            }
        };

    NODE *temp;
	ENTRY item={0};
	ENTRY *found_item;
	NP_ARP_INFO *p_ArpInfo;
    temp=arp_nodehead;
	WP_S32 status;
	struct vlan_ethhdr *vlan_ethheader;
	struct ethhdr * ethheader;
	//struct iphdr * ipheader;
	STRU_SET_INITAIL_CONFIG *np_reconfig = npu_get_np_reconfig();
    WDDI_INFO("Info: np_reconfig_vlan_in_routeSystem, np_reconfig->VlanFlag:0x%x\n",np_reconfig->VlanFlag);
    while(temp!=NULL)
    {
        item.key = temp->ht_key;
        temp=temp->next;

		if(0 == hsearch_r(item,FIND,&found_item,arp_ht))
		{

		}
		else
		{
			p_ArpInfo = (NP_ARP_INFO *)found_item->data;

			if(np_reconfig->VlanFlag == NPU_VLAN_ENABLE)
			{
				vlan_ethheader = (struct vlan_ethhdr *)&tx_route_agg->prefix_header[0];
				memcpy(&vlan_ethheader->h_dest[0],	p_ArpInfo->Mac, 6);
				memcpy(&vlan_ethheader->h_source[0], np_reconfig->ifMACsrc, 6);
				tx_route_agg->prefix_length = 18;

				vlan_ethheader->h_vlan_proto = htons(0x8100);
				vlan_ethheader->h_vlan_TCI = htons(np_reconfig->VlanTag);
				vlan_ethheader->h_vlan_encapsulated_proto = htons(0x0800);
			}
			else
			{
				ethheader = (struct ethhdr *)&tx_route_agg->prefix_header[0];
				memcpy(&ethheader->h_dest[0],	p_ArpInfo->Mac, 6);
				memcpy(&ethheader->h_source[0], np_reconfig->ifMACsrc, 6);
				ethheader->h_proto =htons(0x0800);
				tx_route_agg->prefix_length = 14;
			}
			if(p_ArpInfo->agg_ipfrag!= 0)
			{
				status = WP_IwFlowAggregationModify(p_ArpInfo->agg_ipfrag, WP_IW_FLOW_AGG_MOD_PRX ,&tx_route_agg[0]);
				terminate_on_error(status, "np_reconfig_vport_in_routeSystem WP_IwFlowAggregationModify");
			}
		}
    }
}
void np_dl_release_FMC_resource(u16 DportStartValue)
{
    int status;
	if(DportGrpflow_handle[DportStartValue] != 0)
	{
		status = WP_IwFlowDisable(iw_sys_bridging, DportGrpflow_handle[DportStartValue]);
		terminate_on_error(status, "WP_IwFlowDisable() DportGrpflow_handle[DportStartValue");
		status = WP_IwFlowDelete(iw_sys_bridging, DportGrpflow_handle[DportStartValue]);
		terminate_on_error(status, "WP_IwFlowDelete() DportGrpflow_handle[DportStartValue]");
		status = WP_IwSystemBuild(iw_sys_bridging);
		terminate_on_error(status, "WP_IwSystemBuild - np_dl_release_FMC_resource");
		DportGrpflow_handle[DportStartValue] = 0;
        WDDI_INFO("Info: np_dl_release_FMC_resource::1, DportStartValue:0x%x\n",DportStartValue);
	}
	if(DportGrpagg_handle[DportStartValue].handle != 0)
	{
		status = WP_IwFlowAggregationDelete(DportGrpagg_handle[DportStartValue].handle);
		terminate_on_error(status, "WP_IwFlowAggregationDelete - DportGrpflow_handle[DportStartValue]");
		DportGrpagg_handle[DportStartValue].handle = 0;
		DportGrpagg_handle[DportStartValue].PortSize = 0;
        WDDI_INFO("Info: np_dl_release_FMC_resource::2, DportStartValue:0x%x\n",DportStartValue);
	}
}

void np_display_defrag_info(WP_handle handle)
{
    WP_status status;
	if(handle == 0)
	{
	    Fprintf("np_display_defrag_info handle = NULL!\n");
		return;
	}
	WP_stats_iw_ip_defrag_group defrag_group_stats;
	memset(&defrag_group_stats, 0, sizeof(defrag_group_stats));
	status = WP_IwIpDefragGroupStatistics(handle, &defrag_group_stats);
	terminate_on_error(status,
                       "Defrag Group Stats.\n");

	Fprintf ("\nIPv4 Defrag Statistics:\n");
	Fprintf ("  rx_fragments    -> %d\n", (WP_U32)defrag_group_stats.rx_fragments);
	Fprintf ("  rx_err_frag_fbp -> %d\n", (WP_U32)defrag_group_stats.rx_err_frag_fbp);
	Fprintf ("  rx_err_timeout  -> %d\n", (WP_U32)defrag_group_stats.rx_err_timeout);
	Fprintf ("  rx_packets	   -> %d\n", (WP_U32)defrag_group_stats.rx_packets);
	Fprintf ("  rx_err_defrag   -> %d\n", (WP_U32)defrag_group_stats.rx_err_defrag);
	Fprintf ("  rx_err_mbc	   -> %d\n", (WP_U32)defrag_group_stats.rx_err_defrag_chain_back);
    return;
}
void np_display_frag_info(WP_handle group_handle)
{

	WP_status status = WP_OK;
	WP_stats_iw_ip_frag_group stats;
	if(group_handle == 0)
	{
	    Fprintf("np_display_frag_info handle = NULL!\n");
		return;
	}
	memset(&stats, 0, sizeof(stats));
	status = WP_IwIpFragGroupStatistics(group_handle,
                                        &stats);
	terminate_on_error(status, "WP_IwIpFragGroupStatistics");

	Fprintf ("\nIP Frag Group statistics:\n");
	Fprintf("\nTx Fragments:		%d ", (WP_U32)stats.tx_fragments);
	Fprintf("\nTx Packets:		%d ", (WP_U32)stats.tx_packets);
	Fprintf("\nTx FBP drops:		%d ", (WP_U32)stats.tx_fbp_drops);
	Fprintf("\nTx Qdepth drops:	%d ", (WP_U32)stats.tx_qdepth_drops);
	Fprintf("\n###############################\n");
	return;
}


static WP_dfc_flow_info send2eth_route_dfc_flow_info[1] =
{
    {
        /* filter_handle */ 	0,	/* filter handle to be used for classification */
        /* input_port */    	0,	/* input port handle to be used for classification */
        /* output_port */   	0	/* output port handle to be used for classification */
    },
};


void np_send2eth_route_system_create(void)
{


	WP_dfc_sys_info dfc_sys_info[1] =
        {
            {
                /* classification_hash_size */  WP_IW_4K_HASH_ENTRIES,
                /* port_filtering */            WP_IW_PORT_FILTERING_DISABLE,
                /* deny_eq_ip */                WP_IW_DENY_EQUAL_IP_DISABLE
            }
        };
	WP_iw_ipv4_sys_info ipv4_sys_info[1] =
        {
            {
                /* classification_mode */ WP_IW_DYNAMIC_CLASS,
                /* max_lpm_flows */ 10, //0,
                /* dfc_info	   */ &dfc_sys_info[0]
            }
        };

	WP_iw_sys_routing  route_sys_info[1] =
        {
            {
                /*  buffer_gap			   */ 0x80,
                /*  max_iw_ports			   */ 2,
                /*  max_classification_rules */ 3200, /*depend on how many ip of SGSN*/
                /*  supported_protocols	   */ WP_IW_SYS_IPV4,
                /*  ipv4_sys_info 		   */ ipv4_sys_info,
                /*  ipv6_sys_info 		   */ NULL,
                /*  mpls_sys_info 		   */ NULL
            }
        };

	WP_iw_route_port route_port_cfg =
        {
            /* tag */ 0,
            /* input_filters_mask; */ 0,
            /* output_filters_mask; */ 0,
            /* statmode; */ WP_IW_PORT_STAT_ENABLE,
            /* forwarding_mode */ WP_IW_FORWARDING_DFC|WP_IW_FORWARDING_DIRECT_MAP,
            /* flow_agg */ 0,
            /* ipv6_source_address_validation */ 0,
            /* ipv6_hop_by_hop_term */ 0
        };

/* Create the filter */
    static WP_classification_filter classification_filter_cfg[1] =
        {


            {
                /* fields_mask */  		WP_FIELD_IPV4_DA,		// Fields used by filter
                /* no_match_action */ 	WP_CLASS_FILTER_NO_MATCH_DENY,	// No match action
                /* filter_type */ 		WP_CLASS_FILTER_EMC,		// Filter type
                /* used_fields_config */	{0,}
            }

        };

    WP_S32 status;


/*add bport handle to global sys info */
    NP_SYSTEM_INFO * the_system = np_get_system_info();

    /* Create the Interworking System */

	the_system->system_route_send2eth1_handle= WP_IwSystemCreate(WP_WINPATH(0), WP_IW_ROUTING_MODE, &route_sys_info[0]);
	terminate_on_error(the_system->system_route_send2eth1_handle, "WP_IwSystemCreate() system_route_send2eth1_handle");
	route_port_cfg.tag = 1;
	route_port_cfg.flow_agg = the_system->agg_enet_default_host_handle;
	the_system->Rport_enet1_hanlde = WP_IwPortCreate(the_system->system_route_send2eth1_handle, &route_port_cfg);
	terminate_on_error(the_system->Rport_enet1_hanlde, "WP_IwPortCreate - Rport_enet1_hanlde");

	/* Create the filter */
	send2eth_route_dfc_flow_info[0].filter_handle = WP_IwClassFilterAdd (the_system->system_route_send2eth1_handle, &classification_filter_cfg[0]);
	terminate_on_error(send2eth_route_dfc_flow_info[0].filter_handle , "WP_IwClassFilterAdd() send2eth_route_dfc_flow_info for eth 1");

	status = WP_IwSystemBuild(the_system->system_route_send2eth1_handle);
	terminate_on_error(status, "WP_IwSystemBuild - system_route_send2eth1_handle");

    return ;
}




void np_send2eth_route_flow_add(NP_ARP_INFO * arpInfo)
{

    WP_iw_agg_directmap_enet enet_to_enetiw_agg_cfg[1] =
		{
            {
                /* tag */ 				 0,
                /* txfunc */				 0,
                /* rfcs */				 WP_IW_RFCS_DISABLE,
                /* l2header_insert_mode;*/ WP_IW_L2H_INSERT_ENABLE,
                /* interruptqueue;*/		 WP_IW_IRQT1,
                /* error_pkt_mode*/		 WP_IW_ERRPKT_DISCARD,
                /* intmode;*/ 			 WP_IW_INT_DISABLE,
#if STATISTICS_ENABLE
                /* statmode;*/			 WP_IW_STAT_ENABLE,
#else
                /* statmode;*/			 WP_IW_STAT_DISABLE,
#endif
                /* timestamp_mode;*/		 WP_IW_TIME_STAMP_DISABLE,
                /* ov_pool_mode*/ 		 0,
                /* fbp_drop_threshold;*/	 0,
                /* mtu;*/ 				 1536,
                /* prefix_length */		 /*34*/ 0,
                /* extraction_length */	 18,
                /* prefix_header[68];*/	 {
                    /*0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x01,
                      0x02, 0x03, 0x04, 0x05, 0x81, 0x00, 0x00, 0x85,
                      0x08, 0x00, */0x45, 0x00, 0x00, 0x28, 0x00, 0x04,
                      0x40, 0x00, 0x40, 0x11, 0x00, 0x00, 0x00, 0x00,
                      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                      0x00, 0x00, 0x00, 0x00
                },
                /* policer_enable */		 0,
                /* policer_config */		 NULL,
                /* cong_mode */			 0,
                /* cong_threshold_param */ NULL,
                /*WP_U8 dynamic_length_update;*/ WP_IW_LENGTH_UPDATE_L3,
                /*WP_U8 length_offset;*/ 0,
                WP_IW_FLOW_AGG_PRIMARY
            }
		};

	WP_iw_agg_routing tx_route_agg[1] =
        {
            {
                /* tag */ 				 0,
                /* txfunc */				 0,
                /* dfsrv_remarking */ 	 WP_IW_DSRV_DISABLE,
                /* l2header_insert_mode */ WP_IW_L2H_INSERT_ENABLE,
                /* siphrm */				 WP_IW_FAST_FWD_MODE,
                /* interruptqueue */		 WP_IW_IRQT1,
                /* error_pkt_mode */		 0,
                /* intmode */ 			 WP_IW_INT_DISABLE,
                /* statmode */			 WP_IW_STAT_ENABLE,
                /* timestamp_mode */		 WP_IW_TIME_STAMP_DISABLE,
                /* ov_pool_mode */		 0,
                /* fbp_drop_threshold */	 0,
                /* mtu */ 			   1536,//FRAG_SIZE,
                /* dfsrv_code_point */	 0,
                /* prefix_length */		 18,
                /* multi_cong_prof */ 	 0,
                /* prefix_header[18] */	 {0x00, 0x23, 0x24, 0x1f, 0x54, 0x15, 0x00, 0x01,
                                          0x02, 0x03, 0x04, 0x05,0x81,0x00,0x00,0x85,0x08,0x00},
                /* new_ip_header[20] */	 {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                          0x00, 0x00, 0x00, 0x00},
                /* policer_config */		  0,
                /* maxt */				 0,
                /* cong_threshold_param */ 0,
            }
        };

    char IpHeader[]={
		0x45, 0x00, 0x00, 0x28, 0x00, 0x04,
        0x40, 0x00, 0x40, 0x11, 0x0e, 0x3f, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00
	};
    WP_S32 status;
	NP_SYSTEM_INFO * the_system = np_get_system_info();
    WP_flow_class_rule flow_policy_cfg;
	WP_CHAR rule_string[128]={0};
	WP_U8        *IPsrc;
	WP_U32 i;
	struct vlan_ethhdr * vlan_ethheader;
	struct ethhdr * ethheader;
	struct iphdr * ipheader;
    STRU_SET_INITAIL_CONFIG *initial_cfg = npu_get_initial_cfg_info();
	//WP_handle enet_iw_flow_aggregation;
	//struct vlan_ethhdr * enetheader;

    if(initial_cfg->VlanFlag == NPU_VLAN_ENABLE)
    {
        vlan_ethheader = (struct vlan_ethhdr *)&tx_route_agg->prefix_header[0];
        memcpy(&vlan_ethheader->h_dest[0],	arpInfo->Mac, 6);
        memcpy(&vlan_ethheader->h_source[0], initial_cfg->ifMACsrc, 6);
        tx_route_agg->prefix_length = 18;

        vlan_ethheader->h_vlan_proto = htons(0x8100);
        vlan_ethheader->h_vlan_TCI = htons(initial_cfg->VlanTag);
        vlan_ethheader->h_vlan_encapsulated_proto = htons(0x0800);
    }
    else
    {
        ethheader = (struct ethhdr *)&tx_route_agg->prefix_header[0];
        memcpy(&ethheader->h_dest[0],	arpInfo->Mac, 6);
        memcpy(&ethheader->h_source[0], initial_cfg->ifMACsrc, 6);
        ethheader->h_proto =htons(0x0800);
        tx_route_agg->prefix_length = 14;
    }

    if(arpInfo->agg_ipfrag == 0)
    {
#if IP_FRAG
        if(initial_cfg->VlanFlag == NPU_VLAN_ENABLE)
        {
            tx_route_agg->mtu = FRAG_SIZE+60+18;
        }
        else
        {
            tx_route_agg->mtu = FRAG_SIZE+60+14;
        }
#endif
        tx_route_agg->txfunc = tx_enet_handle_1;
        tx_route_agg->l2_header_insert_mode = WP_IW_L2H_INSERT_ENABLE;
        arpInfo->agg_ipfrag= WP_IwFlowAggregationCreate(0, WP_IW_ROUTING_MODE,&tx_route_agg);
        terminate_on_error(arpInfo->agg_ipfrag, "WP_IwFlowAggregationCreate arpInfo->agg_ipfrag");
        WDDI_INFO("Info: np_send2eth_route_flow_add, arpInfo->agg_ipfrag:0x%x\n",arpInfo->agg_ipfrag);
        memcpy(arpInfo->MacOld, arpInfo->Mac, 6);
    }
    else
    {
        for(i = 0;i <6;i++)
        {
            if(arpInfo->Mac[i] != arpInfo->MacOld[i])
            {
                break;
            }
        }/*MAC has changed ,we need to modify the flowagg*/
        if(i != 6)
        {

            status = WP_IwFlowAggregationModify(arpInfo->agg_ipfrag,WP_IW_FLOW_AGG_MOD_PRX ,&tx_route_agg[0]);
            terminate_on_error(status, "WP_IwFlowAggregationModify arpInfo->agg_ipfrag");

            memcpy(arpInfo->MacOld, arpInfo->Mac, 6);
        }
    }

#if IP_FRAG
    frag_group->qniw = qniw_frag;
    frag_group->frag_size = FRAG_SIZE;
    if(arpInfo->fragGrpHandle == 0)
    {
        arpInfo->fragGrpHandle= WP_IwIpFragGroupCreate(ip_frag_system_handle,arpInfo->agg_ipfrag,frag_group);
        terminate_on_error(arpInfo->fragGrpHandle, "WP_IwIpFragGroupCreate arpInfo->fragGrpHandle");
    }
#endif


    ipheader = (struct iphdr *)&enet_to_enetiw_agg_cfg->prefix_header[0];
    memcpy(ipheader,IpHeader,16);
    ipheader->saddr=htonl(initial_cfg->ifIpaddr);
    enet_to_enetiw_agg_cfg[0].iw_port = 0;
    enet_to_enetiw_agg_cfg[0].l2_header_insert_mode = WP_IW_L2H_INSERT_ENABLE;
    enet_to_enetiw_agg_cfg[0].txfunc = arpInfo->agg_ipfrag;
    enet_to_enetiw_agg_cfg[0].mtu = N_MAX_IP_FRAME_SIZE;   /*can receive big packet*/
    enet_to_enetiw_agg_cfg[0].prefix_length = 16;
    enet_to_enetiw_agg_cfg[0].extraction_length = 16; /*remain ipdst,just replace ipsrc*/
    enet_to_enetiw_agg_cfg[0].length_offset = 0;

    /*MAC header*/
    if(arpInfo->agg_tx2eth1 == 0)/*create it*/
    {
        arpInfo->agg_tx2eth1 = WP_IwFlowAggregationCreate(0,WP_IW_DIRECTMAP_MODE, &enet_to_enetiw_agg_cfg[0]);
        terminate_on_error(arpInfo->agg_tx2eth1, "WP_IwFlowAggregationCreate np_send2eth_route_flow_add");
        WDDI_INFO("Info: np_send2eth_route_flow_add, arpInfo->agg_tx2eth1:0x%x\n",arpInfo->agg_tx2eth1);
    }

	if( arpInfo->flowHandle == 0)/*just add one time for echo arp*/
	{
		IPsrc = (WP_U8 *)&(arpInfo->Ip);
		sprintf(rule_string, "n;n;n;n;%d.%d/255.255;%d.%d/255.255;",
                IPsrc[0],IPsrc[1],IPsrc[2], IPsrc[3]);
		memset(&flow_policy_cfg,0,sizeof(flow_policy_cfg));
		flow_policy_cfg.classifier_string = rule_string;
		flow_policy_cfg.match_action = WP_FLOW_CLASS_MODE_FLOW_AGG;
		flow_policy_cfg.action_info.forwarding_action.flow_aggregation = arpInfo->agg_tx2eth1;

		flow_policy_cfg.dfc_info = &send2eth_route_dfc_flow_info[0];
		arpInfo->flowHandle= WP_IwFlowAdd(the_system->system_route_send2eth1_handle, WP_FLOW_CLASS_RULE, &flow_policy_cfg);
		terminate_on_error(arpInfo->flowHandle, "WP_IwFlowAdd() arpInfo->flowHandle");
        WDDI_INFO("Info: np_send2eth_route_flow_add, arpInfo->Ip:0x%x, arpInfo->flowHandle:0x%x\n",arpInfo->Ip, arpInfo->flowHandle);

		status = WP_IwSystemBuild(the_system->system_route_send2eth1_handle);
		terminate_on_error(status, "WP_IwSystemBuild - system_route_send2eth1_handle");
	}

	return;
}

void np_update_mac_in_route_system(NP_ARP_INFO *arpInfo )
{
    NP_ARP_INFO * arpInfoHash = NULL;
    arpInfoHash = npu_get_arp_info(arpInfo->Ip);
	if(arpInfoHash != NULL)
	{
        np_send2eth_route_flow_add(arpInfoHash);
	}
	return;
}
int np_ul_link_table_delete(WP_U32 portsrc)
{
    STRU_SET_UPLINK_PATH_LINK_TABLE *link_table;
	int ret;

	link_table = npu_get_ul_link_table_info(portsrc);
	if(link_table == NULL)
	{
		WDDI_WARNING("Error: np_ul_link_table_delete::npu_get_ul_link_table_info fail! PortSrc: 0x%x\n",portsrc);
		return ERR_DOWNLINX_FLOW_ADD;
	}
	if(link_table->IpEpAcitveNum == 0)
	{
		WDDI_WARNING("Error: np_ul_link_table_delete::npu_get_ul_link_table_info,link_table->IpEpAcitveNum is 0\n");
		return ERR_DOWNLINX_FLOW_ADD;
	}
    //memset(link_table,0,sizeof(STRU_SET_UPLINK_PATH_LINK_TABLE));
    link_table->IpEpAcitveNum = 0;
	ret = np_ul_link_table_set(link_table);
    WDDI_INFO("Info: np_ul_link_table_delete,portsrc:0x%x\n", portsrc);
    return 0;
}

/*add for BFD*/

void npu_BfdModuleInit(void)
{
    WP_status status;

    WP_module_iw_bfd bfd_config = {/* max_transmission_rate */  MAX_BFD_TRANSMISSION_RATE,
                                   /* min_transmission_rate */  MIN_BFD_TRANSMISSION_RATE,
                                   /* max_detection_time */ MAX_BFD_DETECTION_TIME,
                                   /* min_detection_time */ MIN_BFD_DETECTION_TIME,
                                   /* hash_size */ WP_IW_32_HASH_ENTRIES,
                                   /* max_rx_sessions */ MAX_BFD_RX_SESSIONS,
                                   /* max_tx_sessions */ MAX_BFD_TX_SESSIONS,
                                   WP_IRQT0,
                                   /* WP_iw_bfd_authentication_config */ 0};


    status = WP_ModuleInit(WP_SYSHANDLE(0),
                           WP_WDDI_MODULE_IW_BFD,
                           &bfd_config);


    terminate_on_error(status, "WP_ModuleInit()");
    Fprintf("initial BFD moulde success!\n");
}

void npu_BfdInit(void)
{

    NP_SYSTEM_INFO * the_system = np_get_system_info();
	static WP_iw_sys_directmap iw_sys_one =
        {
            /*max_flows*/  8,
            /* buffer_gap */ 64
        };

    /* creates the interworking system*/
	the_system->system_bfd_handle= WP_IwSystemCreate(0,
                                                     WP_IW_DIRECTMAP_MODE,
                                                     &iw_sys_one);
	terminate_on_error(the_system->system_bfd_handle," WP_IwSystemCreate() system_bfd_handle");

    /*create flowagg for bfd */
	WP_iw_agg_directmap_enet bfd_tx[1] =
        {
            {
                /* tag */					0,
                /* txfunc */				0,
                /* rfcs */ 				WP_IW_RFCS_DISABLE,
                /* l2header_insert_mode;*/ WP_IW_L2H_INSERT_ENABLE,
                /* interruptqueue;*/		WP_IW_IRQT1,
                /* error_pkt_mode*/		WP_IW_ERRPKT_DISCARD,
                /* intmode;*/				WP_IW_INT_DISABLE,
#if STATISTICS_ENABLE
                /* statmode;*/ 			WP_IW_STAT_ENABLE,
#else
                /* statmode;*/ 			WP_IW_STAT_DISABLE,
#endif
                /* timestamp_mode;*/		WP_IW_TIME_STAMP_DISABLE,
                /* ov_pool_mode*/			0,
                /* fbp_drop_threshold;*/	0,
                /* mtu;*/					1536,
                /* prefix_length */		/*34*/ 0,
                /* extraction_length */	0,
                /* prefix_header[68];*/	{0},
                /* policer_enable */		0,
                /* policer_config */		NULL,
                /* cong_mode */			0,
                /* cong_threshold_param */ NULL,
                /*WP_U8 dynamic_length_update;*/ WP_IW_LENGTH_UPDATE_L3,
                /*WP_U8 length_offset;*/ 14,
                WP_IW_FLOW_AGG_PRIMARY
            }
        };
    char IpHeader[]={
		0x45, 0x00, 0x00, 0x28, 0x00, 0x04,
        0x40, 0x00, 0x40, 0x11, 0x0e, 0x3f, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00
	};

    struct vlan_ethhdr * vlan_ethheader;
    struct iphdr * ipheader;
    struct udphdr *udpheader;
    STRU_SET_INITAIL_CONFIG *initial_cfg = npu_get_initial_cfg_info();
    vlan_ethheader = (struct vlan_ethhdr *)&bfd_tx->prefix_header[0];
    memset(&vlan_ethheader->h_dest[0],0xff, 6);
    memcpy(&vlan_ethheader->h_source[0], initial_cfg->ifMACsrc, 6);

    vlan_ethheader->h_vlan_proto = htons(0x8100);
    vlan_ethheader->h_vlan_TCI = htons(133);//BPORT_DEFAULT_VLAN_TAG
    vlan_ethheader->h_vlan_encapsulated_proto = htons(0x0800);

    ipheader = (struct iphdr *)&bfd_tx->prefix_header[18];
    /*update source ip*/
    memcpy(ipheader,IpHeader,20);
    ipheader->saddr=htonl(initial_cfg->ifIpaddr);
    ipheader->daddr=(0xdc21aa0a);/*10.170.33.220*/
    udpheader = (struct udphdr *) (ipheader +1);
    udpheader->source = htons(UDP_BFD_TMP_SPORT);
    udpheader->dest = htons(UDP_BFD_PORT);
    bfd_tx[0].length_offset = 18;
    bfd_tx[0].dynamic_length_update = WP_IW_LENGTH_UPDATE_L3;
    bfd_tx[0].prefix_length = 46;/*18+20+8*/
    bfd_tx[0].txfunc = tx_enet_handle_1;//agg_enet_2; the_system->agg_enet_default_host_handle;
    the_system->agg_bfd_tx_handle = WP_IwFlowAggregationCreate(0,
                                                               WP_IW_DIRECTMAP_MODE,
                                                               bfd_tx);
    terminate_on_error(the_system->agg_bfd_tx_handle," WP_IwFlowAggregationCreate() the_system->agg_bfd_tx_handle");


    WPE_BfdTxSessionBuild(the_system);
    WPE_BfdRxSessionBuild(the_system);
    np_BFD_flowAdd(the_system);

    Fprintf("initial BFD success!\n");
}

void WPE_BfdTxSessionBuild(NP_SYSTEM_INFO *the_system)
{
	WP_status status;
	//WP_U32 ii = 0;

	WP_U8 packet[24] = {0x20,0x00,0x05,0x18,/* version,multiplier , length */
                        0x00,0x00,0x00,0x00,/* my discriminator */
                        0x00,0x00,0x00,0x00,/* your discriminator */
                        0x00,0x00,0x00,0x02, /*desired min tx interval */
                        0x00,0x00,0x00,0x02,/*required min rx interval */
                        0x00,0x00,0x00,0x00};/* required min echo rx interval */

	WP_iw_oam_switching_system_params switching_params;
	switching_params.flow_agg = the_system->agg_bfd_tx_handle;

#if 0
	WP_iw_bfd_tx_data tx_data =
        {
            /* tx_qnode_handle */ NULL,
            /* tx_iw_system */NULL,
            /* packet */NULL,
            /* flow_agg */ NULL
        };
#endif
	WP_iw_bfd_tx_data tx_data;
	tx_data.tx_iw_system = the_system->system_bfd_handle;
	tx_data.tx_qnode_handle = qnode_iw;
	tx_data.tx_iw_system_params = &switching_params;
	tx_data.packet = packet;

	WP_iw_bfd_tx_table_config  tx_table_config =
        {
            /* transmission_interval */ 4,
            /* meticulous */ WP_IW_BFD_NOT_METICULOUS,
            /* authentication_type */ 0,
            /* packet_length */ 24,
            /* pool_handle */ pool_2048,
            /* data */ tx_data,
            /* tag */  0
        };
	packet[7] = 1;  /* my discriminator */
	packet[11] = 1; /* your discriminator */
	tx_table_config.tag = 0;

	the_system->bfd_tx_table_handle = WP_IwBfdSessionCreate(WP_WINPATH(0),
                                                            WP_BFD_TX_SESSION,
                                                            &tx_table_config);
	terminate_on_error(the_system->bfd_tx_table_handle, "WP_IwBfdSessionCreate()");
	status = WP_IwBfdSessionEnable(the_system->bfd_tx_table_handle);
	terminate_on_error(status, "WP_IwBfdSessionEnable()");
	return ;
}


void WPE_BfdRxSessionBuild(NP_SYSTEM_INFO *the_system)
{
	WP_status status;
	WP_bfd_session_statistics session_statistics[MAX_BFD_RX_SESSIONS] ;

	WP_iw_bfd_rx_table_config rx_table_config =
        {
            /* discriminator*/ 0,
            /* rx_port_handle*/ 0,
            /* timeout_check_rate */ 10,
            /* authentication_type */ 0,
            /* authentication_check */WP_IW_BFD_WITHOUT_AUTHENTICATION_CHECK,
            /* max_range */ 0,
            /*  meticulous */ WP_IW_BFD_NOT_METICULOUS,
            /* timeout_interrupt */WP_IW_BFD_INTERRUPT,
            /* tag */ 0
        };

	rx_table_config.rx_port_handle = iw_enet_2;/*for ethernet2*/
	rx_table_config.discriminator = 1;
	rx_table_config.tag = RX_ETH_BFD_INT_TMO_TAG;
	the_system->bfd_rx_table_handle = WP_IwBfdSessionCreate(WP_WINPATH(0),
                                                            WP_BFD_RX_SESSION,
                                                            &rx_table_config);
	terminate_on_error(the_system->bfd_rx_table_handle, "WP_IwBfdSessionCreate()");

	status = WP_IwBfdSessionEnable(the_system->bfd_rx_table_handle);
	terminate_on_error(status, "WP_IwBfdSessionEnable()");
}


void np_BFD_flowAdd(NP_SYSTEM_INFO* the_system)
{
    WP_status status;
    WP_CHAR rule_string[128]={0};
    WP_flow_class_rule class_cfg;
    WP_flow_class_forwarding_action   forwarding_action;
    WP_iw_agg_bfd flow_agg_config =
        {
            /* WP_tag tag */ 0xff,
            /* WP_handle txfunc */ 0,
            /* rfcs */ WP_IW_RFCS_ENABLE,
            /* WP_U8 intmode */ WP_IW_INT_ENABLE,
            /* interruptqueue */ WP_IW_IRQT1,
            /* statmode */ WP_IW_STAT_ENABLE,
            /* ov_pool_mode */ WP_IW_OV_POOL_ENABLE,
            /* fbp_drop_threshold */ 0,
            /* mtu */ 0x600,
            /* extraction_length */ 42,
            /* flow_agg_type */ WP_IW_AGG_PRIMARY,
            /* keep_whole_packet */ 0
        };

    flow_agg_config.tag = RX_ETH_BFD_INT_TAG;
    flow_agg_config.intmode = WP_IW_INT_ENABLE;
    flow_agg_config.extraction_length = 42; /*remove mac+ip+udp header*/
    flow_agg_config.txfunc = iw_rx_host_handle;/*receive from ethernet 2*/
    the_system->agg_bfd_rx_handle= WP_IwFlowAggregationCreate(WP_WINPATH(0),
                                                              WP_IW_BFD_MODE,
                                                              &flow_agg_config);
    terminate_on_error(the_system->agg_bfd_rx_handle," WP_IwFlowAggregationCreate()");

    sprintf (rule_string, "n;n;n;n;n;n;%d;UDP;",UDP_BFD_PORT);/*one hop 3784*/
    enet_to_enet_flow_policy_cfg[NPU_FILTER_DPORT_UDP_BFD].classifier_string = rule_string;
    enet_to_enet_flow_policy_cfg[NPU_FILTER_DPORT_UDP_BFD].action_info.forwarding_action.flow_aggregation = the_system->agg_bfd_rx_handle;
    enet_to_enet_flow_policy_cfg[NPU_FILTER_DPORT_UDP_BFD].dfc_info = &enet_to_enet_dfc_flow_info[NPU_FILTER_DPORT_UDP_BFD];
    enet_to_enet_flow_policy_cfg[NPU_FILTER_DPORT_UDP_BFD].match_action = WP_FLOW_CLASS_MODE_FLOW_AGG;

    the_system->flow_bdf_handle = WP_IwFlowAdd(iw_sys_bridging, WP_FLOW_CLASS_RULE,&(enet_to_enet_flow_policy_cfg[NPU_FILTER_DPORT_UDP_BFD]));
    terminate_on_error(the_system->flow_bdf_handle, "WP_IwFlowAdd() np_BFD_flowAdd");
    status = WP_IwSystemBuild(iw_sys_bridging);
    terminate_on_error(status, "WP_IwSystemBuild - np_BFD_flowAdd");
}

void np_dst_mac_filter_flowAdd(NP_SYSTEM_INFO* the_system)
{
    WP_status status;
    WP_CHAR rule_string[128]={0};
    //WP_flow_class_rule class_cfg;
    STRU_SET_INITAIL_CONFIG *initial_cfg = npu_get_initial_cfg_info();

    /*create flowagg for bfd */
    WP_iw_agg_directmap_enet mac_filter_agg[1] =
        {
            {
                /* tag */				   0,
                /* txfunc */			   0,
                /* rfcs */				   WP_IW_RFCS_DISABLE,
                /* l2header_insert_mode;*/ WP_IW_L2H_INSERT_DISABLE,
                /* interruptqueue;*/	   WP_IW_IRQT1,
                /* error_pkt_mode*/ 	   WP_IW_ERRPKT_DISCARD,
                /* intmode;*/			   WP_IW_INT_DISABLE,
#if STATISTICS_ENABLE
                /* statmode;*/			   WP_IW_STAT_ENABLE,
#else
                /* statmode;*/			   WP_IW_STAT_DISABLE,
#endif
                /* timestamp_mode;*/	   WP_IW_TIME_STAMP_DISABLE,
                /* ov_pool_mode*/		   0,
                /* fbp_drop_threshold;*/   0,
                /* mtu;*/				   1536,
                /* prefix_length */ 	   /*34*/ 0,
                /* extraction_length */    0,
                /* prefix_header[68];*/    {0},
                /* policer_enable */	   0,
                /* policer_config */	   NULL,
                /* cong_mode */ 		   0,
                /* cong_threshold_param */ NULL,
				/*WP_U8 dynamic_length_update;*/ WP_IW_LENGTH_UPDATE_DISABLED,
				/*WP_U8 length_offset;*/ 0,
				WP_IW_FLOW_AGG_PRIMARY
            }
        };

    mac_filter_agg[0].txfunc = iw_sys_bridging;
    mac_filter_agg[0].iw_port = the_system->Bport_mac_filter_handle;
    the_system->agg_mac_filter_handle = WP_IwFlowAggregationCreate(0,WP_IW_DIRECTMAP_MODE, &mac_filter_agg[0]);
    terminate_on_error(the_system->agg_mac_filter_handle, "WP_IwFlowAggregationCreate the_system->agg_mac_filter_handle");

    mac_filter_agg[0].txfunc = iw_sys_bridging;
    mac_filter_agg[0].iw_port = the_system->Bport_mac_filter_from_eth2_handle;
    mac_filter_agg[0].mtu = N_MAX_IP_FRAME_SIZE;
    the_system->agg_mac_filter_from_eth2_handle = WP_IwFlowAggregationCreate(0,WP_IW_DIRECTMAP_MODE, &mac_filter_agg[0]);
    terminate_on_error(the_system->agg_mac_filter_from_eth2_handle, "WP_IwFlowAggregationCreate the_system->agg_mac_filter_from_eth2_handle");


    sprintf (rule_string, "n;n;n;n;n;n;n;n;n;%d.%d.%d.%d.%d.%d;",0xff,
             0xff,
             0xff,
             0xff,
             0xff,
             0xff);
    enet_to_enet_flow_policy_cfg[NPU_FILTER_DST_MAC].classifier_string = rule_string;
    enet_to_enet_flow_policy_cfg[NPU_FILTER_DST_MAC].action_info.forwarding_action.flow_aggregation = the_system->agg_mac_filter_handle;
    enet_to_enet_flow_policy_cfg[NPU_FILTER_DST_MAC].dfc_info = &enet_to_enet_dfc_flow_info[NPU_FILTER_DST_MAC];
    enet_to_enet_flow_policy_cfg[NPU_FILTER_DST_MAC].match_action = WP_FLOW_CLASS_MODE_FLOW_AGG;

    the_system->flow_mac_filter_handle = WP_IwFlowAdd(iw_sys_bridging, WP_FLOW_CLASS_RULE,&(enet_to_enet_flow_policy_cfg[NPU_FILTER_DST_MAC]));
    terminate_on_error(the_system->flow_mac_filter_handle, "WP_IwFlowAdd() flow_mac_filter_handle 0xff ..");


    sprintf (rule_string, "n;n;n;n;n;n;n;n;n;%d.%d.%d.%d.%d.%d;",initial_cfg->ifMACsrc[0],
             initial_cfg->ifMACsrc[1],
             initial_cfg->ifMACsrc[2],
             initial_cfg->ifMACsrc[3],
             initial_cfg->ifMACsrc[4],
             initial_cfg->ifMACsrc[5]);

    enet_to_enet_flow_policy_cfg[NPU_FILTER_DST_MAC].classifier_string = rule_string;
    the_system->flow_mac_filter_handle = WP_IwFlowAdd(iw_sys_bridging, WP_FLOW_CLASS_RULE,&(enet_to_enet_flow_policy_cfg[NPU_FILTER_DST_MAC]));
    terminate_on_error(the_system->flow_mac_filter_handle, "WP_IwFlowAdd() flow_mac_filter_handle");

    /*add for mac from eth2*/
    sprintf (rule_string, "n;n;n;n;n;n;n;n;n;%d.%d.%d.%d.%d.%d;",0xfe,
             0xfe,
             0xfe,
             0xfe,
             0xfe,
             0xfe);
    enet_to_enet_flow_policy_cfg[NPU_FILTER_DST_MAC].classifier_string = rule_string;
    enet_to_enet_flow_policy_cfg[NPU_FILTER_DST_MAC].action_info.forwarding_action.flow_aggregation = the_system->agg_mac_filter_from_eth2_handle;
    the_system->flow_mac_filter_handle = WP_IwFlowAdd(iw_sys_bridging, WP_FLOW_CLASS_RULE,&(enet_to_enet_flow_policy_cfg[NPU_FILTER_DST_MAC]));
    terminate_on_error(the_system->flow_mac_filter_handle, "WP_IwFlowAdd() flow_mac_filter_handle 0xfe ..");

    status = WP_IwSystemBuild(iw_sys_bridging);
    terminate_on_error(status, "WP_IwSystemBuild - flow_mac_filter_handle");
}

void np_reconfig_vlan_in_iwPort(void)
{
    WP_iw_agg_directmap_enet mac_filter_agg[1] =
        {
            {
                /* tag */				   0,
                /* txfunc */			   0,
                /* rfcs */				   WP_IW_RFCS_DISABLE,
                /* l2header_insert_mode;*/ WP_IW_L2H_INSERT_DISABLE,
                /* interruptqueue;*/	   WP_IW_IRQT1,
                /* error_pkt_mode*/ 	   WP_IW_ERRPKT_DISCARD,
                /* intmode;*/			   WP_IW_INT_DISABLE,
#if STATISTICS_ENABLE
                /* statmode;*/			   WP_IW_STAT_ENABLE,
#else
                /* statmode;*/			   WP_IW_STAT_DISABLE,
#endif
                /* timestamp_mode;*/	   WP_IW_TIME_STAMP_DISABLE,
                /* ov_pool_mode*/		   0,
                /* fbp_drop_threshold;*/   0,
                /* mtu;*/				   1536,
                /* prefix_length */ 	   /*34*/ 0,
                /* extraction_length */    0,
                /* prefix_header[68];*/    {0},
                /* policer_enable */	   0,
                /* policer_config */	   NULL,
                /* cong_mode */ 		   0,
                /* cong_threshold_param */ NULL,
                /*WP_U8 dynamic_length_update;*/ WP_IW_LENGTH_UPDATE_DISABLED,
                /*WP_U8 length_offset;*/ 0,
                WP_IW_FLOW_AGG_PRIMARY
            }
        };
	struct vlan_ethhdr *vlan_ethheader;
	struct ethhdr * ethhdr;
    WP_status status;
	STRU_SET_INITAIL_CONFIG *np_reconfig = npu_get_np_reconfig();
	NP_SYSTEM_INFO * the_system = np_get_system_info();

    Fprintf("Info: np_reconfig_vlan_in_iwPort, np_reconfig->VlanTag:%d\n",np_reconfig->VlanTag);
    bport_conf[0].vlan_param.vlan_tag = np_reconfig->VlanTag;

	if(the_system->Bport_mac_filter_handle != 0)
	{
	    status = WP_IwBridgingPortModify(iw_sys_bridging,the_system->Bport_mac_filter_handle,WP_IW_BPORT_MOD_VLAN_DEFAULT_TAG,bport_conf);
	    if(WP_OK != status)
	    {
            WDDI_WARNING("Warn: np_reconfig_vlan_in_iwPort::WP_IwPortModify::1 fail!\n");
	    }
	}
    if(the_system->agg_mac_filter_handle != 0)
    {
        mac_filter_agg->iw_port = the_system->Bport_mac_filter_handle;
        status = WP_IwFlowAggregationModify(the_system->agg_mac_filter_handle, WP_IW_FLOW_AGG_MOD_IWPORT ,mac_filter_agg);
        if(WP_OK != status)
        {
            WDDI_WARNING("Warn: np_reconfig_vlan_in_iwPort::WP_IwFlowAggregationModify::0 fail!\n");
        }
    }
#if 1
	if(iw_enet_3 != 0)
	{
	    status = WP_IwBridgingPortModify(iw_sys_bridging,iw_enet_3,WP_IW_BPORT_MOD_VLAN_DEFAULT_TAG,bport_conf);
	    if(WP_OK != status)
	    {
            WDDI_WARNING("Warn: np_reconfig_vlan_in_iwPort::WP_IwPortModify::2 fail!\n");
	    }
	}

	if(np_reconfig->VlanFlag == NPU_VLAN_ENABLE)
	{
        vlan_ethheader = (struct vlan_ethhdr *)&tx_agg_l3->prefix_header[0];
    	tx_agg_l3->prefix_length = 18;
		vlan_ethheader->h_vlan_proto = htons(0x8100);
		vlan_ethheader->h_vlan_TCI = htons(np_reconfig->VlanTag);//htons(BPORT_DEFAULT_VLAN_TAG);
		vlan_ethheader->h_vlan_encapsulated_proto = htons(0x0800);
	}
	else
	{
	    ethhdr = (struct ethhdr *)&tx_agg_l3->prefix_header[0];
    	tx_agg_l3->prefix_length = 14;
		ethhdr->h_proto = htons(0x0800);
	}

    if(ipdefrag_agg != 0)
    {
        //tx_agg_l3->iw_port = iw_enet_3;
        status = WP_IwFlowAggregationModify(ipdefrag_agg, /*WP_IW_FLOW_AGG_MOD_IWPORT|*/WP_IW_FLOW_AGG_MOD_PRX ,tx_agg_l3);
        if(WP_OK != status)
        {
            WDDI_WARNING("Warn: np_reconfig_vlan_in_iwPort::WP_IwFlowAggregationModify::1 fail!\n");
        }
    }
#endif
    tx_agg_enet->iw_port = iw_enet_3;
    status = WP_IwFlowAggregationModify(the_system->agg_after_defrag, WP_IW_FLOW_AGG_MOD_IWPORT,tx_agg_enet);
    //WP_Display(0,ipdefrag_agg,WP_DISPLAY_DEFAULT,0);

    if(WP_OK != status)
    {
        WDDI_WARNING("Warn: np_reconfig_vlan_in_iwPort::WP_IwFlowAggregationModify::2 fail!\n");
    }
}

#define WPL_INTRPT_MASK_EXTERNAL_L   (wpl_rif_base + 0xc2b0)
#define WPL_INTRPT_MASK_EXTERNAL_H   (wpl_rif_base + 0xc2b4)

static WP_U32        wpl_rif_base = 0;

#define WPL_INTRPT_REGISTER_GET(val, reg) WP_MEM_GET((val), *((WP_U32*)reg))
#define WPL_INTRPT_REGISTER_SET(reg, val) WP_MEM_SET(*((WP_U32*)reg), (val))

void np_control_interrupt(u8 flag,u8 Irqnum)
{
	WP_U32 intrpt_mask;
	if(wpl_rif_base ==0)
	{
		WPLI_BusesRifBaseGet(0, &wpl_rif_base);
	}
    if(flag)/*enable*/
    {
		WPL_INTRPT_REGISTER_GET(intrpt_mask, WPL_INTRPT_MASK_EXTERNAL_H);
        if(Irqnum ==0)/*FR IRQ0*/
        {
            intrpt_mask |= 0x00100000;
        }
        else if(Irqnum == 1)/*IP IRQ1*/
        {
            intrpt_mask |= 0x00200000;
        }
        else
        {
        }
        WPL_INTRPT_REGISTER_SET(WPL_INTRPT_MASK_EXTERNAL_H, intrpt_mask);

	}
	else
	{
		WPL_INTRPT_REGISTER_GET(intrpt_mask, WPL_INTRPT_MASK_EXTERNAL_H);
        if(Irqnum ==0)/*FR*/
        {
            intrpt_mask &= ~0x00100000;
        }
        else if(Irqnum == 1)/*IP*/
        {
            intrpt_mask &= ~0x00200000;
        }
        else
        {
        }
        WPL_INTRPT_REGISTER_SET(WPL_INTRPT_MASK_EXTERNAL_H, intrpt_mask);
	}
}
void np_set_over_interrupt(void)
{
	WP_U32 intrpt_mask;
	if(wpl_rif_base ==0)
	{
		WPLI_BusesRifBaseGet(0, &wpl_rif_base);
	}
	WPL_INTRPT_REGISTER_GET(intrpt_mask, WPL_INTRPT_MASK_EXTERNAL_H);
	if((intrpt_mask & 0x0003c000) == 0)
	{
		intrpt_mask |= 0x0003c000;
		WPL_INTRPT_REGISTER_SET(WPL_INTRPT_MASK_EXTERNAL_H, intrpt_mask);
		Fprintf("Info: np_set_over_interrupt \n");
	}
}

#if 0
static inline void copy16bytes(void *dst, const void *src)
{
    asm volatile ("movdqu (%[src]), %%xmm0\n\t"
                  "movdqu %%xmm0, (%[dst])\n\t"
                  :
                  : [src] "r" (src), [dst] "r"(dst)
                  : "xmm0", "memory");
}

static inline void copy32bytes(void *dst, const void *src) {
    const char *s =src;
    char *d = dst;
    copy16bytes(d, s);
    copy16bytes(d+16, s+16);
}

static inline void copy64bytes(void *dst, const void *src) {
    const char *s =src;
    char *d = dst;
    copy32bytes(d, s);
    copy32bytes(d+16, s+16);
}

static inline void copy128bytes(void *dst, const void *src) {
    const char *s =src;
    char *d = dst;
    copy64bytes(d, s);
    copy64bytes(d+16, s+16);
}

static inline void copy256bytes(void *dst, const void *src) {
    const char *s =src;
    char *d = dst;
    copy128bytes(d, s);
    copy128bytes(d+16, s+16);
}


#if NPU_HOST_OLP

void *pcimem_copy(void * b,const void * a,size_t n)
{
#if 0
    char *s1 = b;
    const char *s2 = a;
    for(; 0<n; --n)*s1++ = *s2++;
    return b;

#endif
#if 0
    unsigned long long *s1Words2 = b;
    const unsigned long long *s2Words2 = a;
    unsigned int n2Wards  = n >> 4;
    unsigned int ByteLeft = n & 15;
    unsigned int i;

    for(i = 0;i < n2Wards;i++) {
        *s1Words2++ = *s2Words2++;
    }

    char *s1Bytes = (char *)s1Words2;
    const char *s2Bytes = (char *)s2Words2;

    for(i = 0;i < ByteLeft;i++) {
        *s1Bytes++ = *s2Bytes++;
    }
#else
    const unsigned char *src = a;
    unsigned char *dst = b;

    while (n > 256) {
        copy256bytes(dst, src);
        n   -= 256;
        src += 256;
        dst += 256;
    }

    if (n & 128) {
        copy128bytes(dst, src);
        n   -= 128;
        src += 128;
        dst += 128;
    }

    if (n & 64) {
        copy64bytes(dst, src);
        n   -= 64;
        src += 64;
        dst += 64;
    }

    if (n & 32) {
        copy32bytes(dst, src);
        n   -= 32;
        src += 32;
        dst += 32;
    }

    if (n & 16) {
        copy16bytes(dst, src);
        n   -= 16;
        src += 16;
        dst += 16;
    }

    memcpy(dst, src, n);
#endif

#endif
}
#else
static inline void copy16bytes(uint8_t *dst, const uint8_t *src)
{
    asm volatile ("movdqu (%[src]), %%xmm0\n\t"
                  "movdqu %%xmm0, (%[dst])\n\t"
                  :
                  : [src] "r" (src), [dst] "r"(dst)
                  : "xmm0", "memory");
}

static inline void copy32bytes(uint8_t *dst, const uint8_t *src)
{
    asm volatile ("movdqu (%[src]), %%xmm0\n\t"
                  "movdqu 16(%[src]), %%xmm1\n\t"
                  "movdqu %%xmm0, (%[dst])\n\t"
                  "movdqu %%xmm1, 16(%[dst])"
                  :
                  : [src] "r" (src), [dst] "r"(dst)
                  : "xmm0", "xmm1", "memory");
}

static inline void copy64bytes(uint8_t *dst, const uint8_t *src)
{
    asm volatile ("movdqu (%[src]), %%xmm0\n\t"
                  "movdqu 16(%[src]), %%xmm1\n\t"
                  "movdqu 32(%[src]), %%xmm2\n\t"
                  "movdqu 48(%[src]), %%xmm3\n\t"
                  "movdqu %%xmm0, (%[dst])\n\t"
                  "movdqu %%xmm1, 16(%[dst])\n\t"
                  "movdqu %%xmm2, 32(%[dst])\n\t"
                  "movdqu %%xmm3, 48(%[dst])"
                  :
                  : [src] "r" (src), [dst] "r"(dst)
                  : "xmm0", "xmm1", "xmm2", "xmm3","memory");
}

static inline void copy128bytes(uint8_t *dst, const uint8_t *src)
{
    asm volatile ("movdqu (%[src]), %%xmm0\n\t"
                  "movdqu 16(%[src]), %%xmm1\n\t"
                  "movdqu 32(%[src]), %%xmm2\n\t"
                  "movdqu 48(%[src]), %%xmm3\n\t"
                  "movdqu 64(%[src]), %%xmm4\n\t"
                  "movdqu 80(%[src]), %%xmm5\n\t"
                  "movdqu 96(%[src]), %%xmm6\n\t"
                  "movdqu 112(%[src]), %%xmm7\n\t"
                  "movdqu %%xmm0, (%[dst])\n\t"
                  "movdqu %%xmm1, 16(%[dst])\n\t"
                  "movdqu %%xmm2, 32(%[dst])\n\t"
                  "movdqu %%xmm3, 48(%[dst])\n\t"
                  "movdqu %%xmm4, 64(%[dst])\n\t"
                  "movdqu %%xmm5, 80(%[dst])\n\t"
                  "movdqu %%xmm6, 96(%[dst])\n\t"
                  "movdqu %%xmm7, 112(%[dst])"
                  :
                  : [src] "r" (src), [dst] "r"(dst)
                  : "xmm0", "xmm1", "xmm2", "xmm3", "xmm4", "xmm5", "xmm6", "xmm7", "memory");
}

// Try best to use XMM register to improve the memcpy speed
void * pcimem_copy(void * dst, const void * src, int n)
{
    const uint8_t * s = (uint8_t *)src;
    uint8_t * d = (uint8_t *)dst;

    if (n <= 0)
    {
        Fprintf("length error in my cpy function %d\n", n);
        return NULL;
    }

    if (n < 16)
    {
        memcpy(d, s, n);
        return dst;
    }

    // handle the size less than 128 bytes and try best to use 128bits registers
    if ( n < 128 )
    {
        if (n >> 6)
        {
            copy64bytes(d, s);
            n -= 64;
            d += 64;
            s += 64;
        }

        if (n >> 5)
        {
            copy32bytes(d, s);
            n -= 32;
            d += 32;
            s += 32;
        }

        if (n >> 4)
        {
            copy16bytes(d, s);
            n -= 16;
            d += 16;
            s += 16;
        }

	    // handle left less than 16 bytes case
        if (n > 0)
        {
	        copy16bytes(d - 16 + n, s - 16 + n);
        }

        return  dst;
    }

    // Try to use XMM eight 128 bits register to improve the full speed
    while(n >= 128)
    {
        copy128bytes(d, s);
        n -= 128;
        s += 128;
        d += 128;
    }

    // left part is less than 128 bytes, can resue the before part, but in case it
    // can't be built into inline, can keep it redudancy.
    if (n >> 6)
    {
        copy64bytes(d,s);
        n -= 64;
        s += 64;
        d += 64;
    }

    if (n >> 5)
    {
        copy32bytes(d,s);
        n -= 32;
        s += 32;
        d += 32;
    }

    if (n >> 4)
    {
        copy16bytes(d,s);
        n -= 16;
        s += 16;
        d += 16;
    }

    // less than 16 bytes
    if (n > 0)
    {
        copy16bytes(d - 16 + n, s - 16 + n);
    }

    return dst;
}
#endif
void np_create_arp_hq()
{

	NP_SYSTEM_INFO * the_system = np_get_system_info();
	WP_iw_agg_hq hq_config[1] =
        {
            {
                /* tag */ 		2,
                /* txfunc */		0,
                /* hq_agg_mode */ 8,//WP_IW_HQ_ARP_RARP,/wddi has a bug use WP_IW_ARP_TERMINATION.
                /* mtu */ 		1536,
                /* interruptqueue */ WP_IW_IRQT1,
                /* intmode */ WP_IW_INT_ENABLE,
                /* statmode */ WP_IW_STAT_ENABLE,
                /* timestamp_mode */ WP_IW_TIME_STAMP_DISABLE,
                /* ov_pool_mode */ 0,
                /* fbp_drop_threshold */ 0,
                /*tx_direction;*/0,
                /*flow_analyze;*/WP_IW_FLOW_PASS,
                /*force_analyze;*/WP_IW_FORCE_NO,
                /*l4_checksum_recal;*/WP_IW_L4_NO_CHECKSUM,
                /*nexthop;*/0,
                /*flow_agg_type;*/ WP_IW_AGG_PRIMARY
            }
        };
    hq_config[0].tag = RX_ETH_INT_PRIO_MSG_TAG;
    hq_config[0].txfunc = the_system->Channel_IP_PrioRx0_host_handle;
    the_system->agg_host_arp_agg= WP_IwFlowAggregationCreate(WP_WINPATH(0), WP_IW_HOST_QUEUE_MODE,&hq_config[0]);
    terminate_on_error(the_system->agg_host_arp_agg,"WP_IwFlowAggregationCreate() Host ARP");
    return ;
}

npu_printf_debugReg(void)
{
    WP_S32 ret;
    WP_U32 *addr,base_addr,i;
	ret = WPI_DebugBaseAddrGet(0,&base_addr,&addr);
	if(ret != 0)
	{

		Fprintf("WPI_DebugBaseAddrGet failed \n");
		return 0;
	}

	Fprintf(" debug base_addr is 0x%x addr= 0x%x \n",base_addr,addr);
	for(i = 0;i < 16;i++)
	{
        if(i%4 == 0)
        {
		    Fprintf("\n");
	        Fprintf(" 0x%x: ",addr);
        }
        Fprintf("0x%x ",*(WP_U32 *)addr);
        addr++;
	}
	Fprintf("\n");
}
void show_dps_dl_link_item(STRU_SET_FR_DOWNLINK_PATH_LINK_TABLE *p_linkInfo)
{
    wp_vport_table vportItem = {0};
    int i;
    WP_VPortEntryGet(0, p_linkInfo->NSEI, &vportItem);
    Fprintf("DPS DL Link is::valid_num:0x%x\n",vportItem.valid_num);
    for(i = 0; i < 16; i++)
    {
        if(i%2 == 0) Fprintf("\n");
        Fprintf("flow_num[%d]:0x%x, ",i, vportItem.flow_num[i]);
    }
    Fprintf("\n");
}

void show_dps_ul_route_item(NP_UL_FLOW_INFO *p_FrFlowInfo)
{
    int i;
    FR_CHANNEL_INFO * ch_info;
    wp_globle_rt ul_route_item = {0};
    STRU_SET_INITAIL_CONFIG *initial_cfg = npu_get_initial_cfg_info();
    if(NULL == initial_cfg)
    {
        Fprintf("Error: show_dps_ul_route_item::npu_get_initial_cfg_info fail!\n");
        return;
    }

    //Fprintf("Info: show_dps_ul_route_item, vport:%d(0x%x)\n", p_FrFlowInfo->vport);
    STRU_VPORT_INFO  *VportInfo = npu_get_vport_info(p_FrFlowInfo->vport);
    if(NULL == VportInfo)
    {
        Fprintf("Error: show_dps_ul_route_item::npu_get_vport_info fail! vport:%d\n", p_FrFlowInfo->vport);
        return;
    }
    ch_info = &VportInfo->ch_info;

    if(initial_cfg->OpterateMode == NPU_MODE_FR)
    {
        //Fprintf("Info: show_dps_ul_route_item::NPU_MODE_FR, ch_info->agg_fr_rx_host_flow:0x%x\n", ch_info->agg_fr_rx_host_flow);
        WP_lookupEntryGet(0, ch_info->agg_fr_rx_host_flow, &ul_route_item);
    }

    if(initial_cfg->OpterateMode == NPU_MODE_IP)
    {
        Fprintf("Info: show_dps_ul_route_item::NPU_MODE_IP, VportInfo->agg_ip2iptx2host:0x%x\n", VportInfo->agg_ip2iptx2host);
        WP_lookupEntryGet(0, VportInfo->agg_ip2iptx2host, &ul_route_item);
    }

    Fprintf("DPS UL Route is::flow_num:0x%x\n",ul_route_item.flow_num);
    for(i = 0; i < 257; i++)
    {
        if(i%2 == 0) Fprintf("\n");
        Fprintf("port_num[%d]:0x%x, ",i, ul_route_item.port_num[i]);
    }
    Fprintf("\n");
}

void show_dps_dl_route_item(NP_DL_FLOW_INFO *p_EthFlowInfo)
{
    unsigned int ret = 0;
	wp_dl_rt dl_route_item = {0};
    STRU_SET_DOWNLINK_PATH_RULE * rule = &p_EthFlowInfo->rule;
#if 0
	wp_dl_rt dl_rt;
    STRU_VPORT_INFO  *VportInfo;
	NP_SYSTEM_INFO * the_system = np_get_system_info();
	STRU_SET_INITAIL_CONFIG *initial_cfg = npu_get_initial_cfg_info();
	dl_rt.bvci_n = rule->BVCInew;
	if(initial_cfg->OpterateMode == NPU_MODE_FR)
	{
		dl_rt.nsei_i = rule->nsei_portSrc.NSEI;
		dl_rt.flow_num = WPI_HANDLE_INDEX(p_EthFlowInfo->agg_dl_dportGrp_handle);
	}
	else
	{
        VportInfo = npu_get_vport_info(rule->nsei_portSrc.portSrc);
        if(NULL == VportInfo)
        {
            Fprintf("Error: show_dps_dl_route_item::npu_get_vport_info failed! portsrc:%d\n", rule->nsei_portSrc.portSrc);
            return ;
        }
		dl_rt.nsei_i = htons(rule->nsei_portSrc.portSrc);
		dl_rt.flow_num = (WPI_HANDLE_INDEX(the_system->agg_dl_ip_to_enet2_handle)<<16) | WPI_HANDLE_INDEX(VportInfo->agg_dl_ip2iptx2Enet1);
	}
#endif
    ret = WP_DLlookupEntryGet(0, p_EthFlowInfo->agg_dl_dportGrp_handle, rule->BVCI, &dl_route_item);
    if(0 == ret)
    {
        Fprintf("DPS DL Route is::flow_num:0x%x, bvci_n:%d(0x%x), nsei_i:%d(0x%x)\n", \
                dl_route_item.flow_num, dl_route_item.bvci_n, dl_route_item.bvci_n, dl_route_item.nsei_i, dl_route_item.nsei_i);
    }
    else
    {
        Fprintf("DPS DL Route get failed!\n");
    }
}

void show_dps_ul_link_item(STRU_SET_UPLINK_PATH_LINK_TABLE *p_linkInfo)
{
    wp_local_rt ul_link_item = {0};
    int i;
    WP_IPEndPointEntryGet(0, htons(p_linkInfo->PortSrc), &ul_link_item);
    Fprintf("DPS UL Link is::valid_num:0x%x\n",ul_link_item.valid_num);
    for(i = 0; i < 64; i++)
    {
        Fprintf("ip_ep[%d].ip_addr:0x%x, ",i, ul_link_item.ip_ep[i].ip_addr);
        Fprintf("ip_ep[%d].dst_port:0x%x, ",i, ul_link_item.ip_ep[i].dst_port);
        Fprintf("ip_ep[%d].src_port:0x%x\n",i, ul_link_item.ip_ep[i].src_port);
    }

}

void show_dps_bvci_item(NP_UPDATE_BVCI_FLOW_INFO *p_BvciFlowInfo)
{
    WP_U16 hashRet1, hashRet2;
    int rt;
	STRU_VPORT_INFO  *VportInfo;
    FR_CHANNEL_INFO *ch_info;
    STRU_SET_INITAIL_CONFIG *initial_cfg = npu_get_initial_cfg_info();
	VportInfo = npu_get_vport_info(p_BvciFlowInfo->vport);
    if(NULL == VportInfo)
    {
        Fprintf("Error: show_dps_bvci_item::npu_get_vport_info fail! vport:0x%x\n", p_BvciFlowInfo->vport);
        return;
    }

    ch_info = &VportInfo->ch_info;
	if(initial_cfg->OpterateMode == NPU_MODE_FR)
	{
        rt = WPI_UserDefHashEntryDisplay(0,WPI_HANDLE_INDEX(ch_info->agg_tx2Enet1),p_BvciFlowInfo->NS_BVCI_old,&hashRet1);
        if(0 == rt)
        {
            Fprintf("DPS bvci-FR: flowNum_E1:0x%x, bvci_old:%d, bvci_new:%d\n", ch_info->agg_tx2Enet1, p_BvciFlowInfo->NS_BVCI_old, hashRet1);
        }
        rt = WPI_UserDefHashEntryDisplay(0,WPI_HANDLE_INDEX(ch_info->agg_fr_to_enet2),p_BvciFlowInfo->NS_BVCI_old,&hashRet2);
        if(0 == rt)
        {
            Fprintf("DPS bvci-FR: flowNum_E2:0x%x, bvci_old:%d, bvci_new:%d\n", ch_info->agg_fr_to_enet2, p_BvciFlowInfo->NS_BVCI_old, hashRet2);
        }
	}
	if(initial_cfg->OpterateMode == NPU_MODE_IP)
	{
        rt = WPI_UserDefHashEntryDisplay(0,WPI_HANDLE_INDEX(VportInfo->agg_ul_ip2iptx2Enet1),p_BvciFlowInfo->NS_BVCI_old,&hashRet1);
        if(0 == rt)
        {
            Fprintf("DPS bvci-IP: flowNum_E1:0x%x, bvci_old:%d, bvci_new:%d\n", VportInfo->agg_ul_ip2iptx2Enet1, p_BvciFlowInfo->NS_BVCI_old, hashRet1);
        }
        rt = WPI_UserDefHashEntryDisplay(0,WPI_HANDLE_INDEX(VportInfo->agg_ul_ip_to_enet2_handle),p_BvciFlowInfo->NS_BVCI_old,&hashRet2);
        if(0 == rt)
        {
            Fprintf("DPS bvci-IP: flowNum_E2:0x%x, bvci_old:%d, bvci_new:%d\n", VportInfo->agg_ul_ip_to_enet2_handle, p_BvciFlowInfo->NS_BVCI_old, hashRet2);
        }
	}
}
void np_display_dport_flow_info(u16 dport)
{
    wp_ns_bssgp_statis wp_statics;

    memset(&wp_statics,0,sizeof(wp_statics));
    Fprintf("np_display_dport_flow_info(): dport=%d, handle=0x%x\n",dport, DportGrpagg_handle[dport].handle);
    if(DportGrpagg_handle[dport].handle)
    {
        WP_FlowStatisticsGet(0,DportGrpagg_handle[dport].handle,&wp_statics);
        Fprintf("Hi: DportAgg:RxNsBytes=0x%x\n",*(int*)(&wp_statics.RxNsBytes));
        Fprintf("Lo: DportAgg:RxNsBytes=0x%x\n",*((int*)(&wp_statics.RxNsBytes)+1));
        Fprintf("Hi: DportAgg:RxNsDataPackets=0x%x\n",*(int*)(&wp_statics.RxNsDataPackets));
        Fprintf("Lo: DportAgg:RxNsDataPackets=0x%x\n",*((int*)(&wp_statics.RxNsDataPackets)+1));
        Fprintf("Hi: DportAgg:RxNsSignalPackets=0x%x\n",*(int*)(&wp_statics.RxNsSignalPackets));
        Fprintf("Lo: DportAgg:RxNsSignalPackets=0x%x\n",*((int*)(&wp_statics.RxNsSignalPackets)+1));
        Fprintf("Hi: DportAgg:RxBssgpBytes=0x%x\n",*(int*)(&wp_statics.RxBssgpBytes));
        Fprintf("Lo: DportAgg:RxBssgpBytes=0x%x\n",*((int*)(&wp_statics.RxBssgpBytes)+1));
        Fprintf("Hi: DportAgg:RxBssgpDataPackets=0x%x\n",*(int*)(&wp_statics.RxBssgpDataPackets));
        Fprintf("Lo: DportAgg:RxBssgpDataPackets=0x%x\n",*((int*)(&wp_statics.RxBssgpDataPackets)+1));
        Fprintf("Hi: DportAgg:RxBssgpSignalPackets=0x%x\n",*(int*)(&wp_statics.RxBssgpSignalPackets));
        Fprintf("Lo: DportAgg:RxBssgpSignalPackets=0x%x\n",*((int*)(&wp_statics.RxBssgpSignalPackets)+1));
        Fprintf("Hi: DportAgg:TxNsBytes=0x%x\n",*(int*)(&wp_statics.TxNsBytes));
        Fprintf("Lo: DportAgg:TxNsBytes=0x%x\n",*((int*)(&wp_statics.TxNsBytes)+1));
        Fprintf("Hi: DportAgg:TxNsDataPackets=0x%x\n",*(int*)(&wp_statics.TxNsDataPackets));
        Fprintf("Lo: DportAgg:TxNsDataPackets=0x%x\n",*((int*)(&wp_statics.TxNsDataPackets)+1));
        Fprintf("Hi: DportAgg:TxNsSignalPackets=0x%x\n",*(int*)(&wp_statics.TxNsSignalPackets));
        Fprintf("Lo: DportAgg:TxNsSignalPackets=0x%x\n",*((int*)(&wp_statics.TxNsSignalPackets)+1));
        Fprintf("Hi: DportAgg:TxBssgpBytes=0x%x\n",*(int*)(&wp_statics.TxBssgpBytes));
        Fprintf("Lo: DportAgg:TxBssgpBytes=0x%x\n",*((int*)(&wp_statics.TxBssgpBytes)+1));
        Fprintf("Hi: DportAgg:TxBssgpDataPackets=0x%x\n",*(int*)(&wp_statics.TxBssgpDataPackets));
        Fprintf("Lo: DportAgg:TxBssgpDataPackets=0x%x\n",*((int*)(&wp_statics.TxBssgpDataPackets)+1));
        Fprintf("Hi: DportAgg:TxBssgpSignalPackets=0x%x\n",*(int*)(&wp_statics.TxBssgpSignalPackets));
        Fprintf("Lo: DportAgg:TxBssgpSignalPackets=0x%x\n",*((int*)(&wp_statics.TxBssgpSignalPackets)+1));
    }

    return ;
}

void np_display_table_item(STRU_GET_TABLE_ITEM *pstru_get_tblItem)
{
    u32 i;
    u16 tblItem = pstru_get_tblItem->tableLoc[1];
    switch(pstru_get_tblItem->tableLoc[0])
    {
    case UL_ROUTE:
    {
        wp_globle_rt ul_route_item = {0};
        WP_lookupEntryGet(0, tblItem, &ul_route_item);
        Fprintf("DPS UL Route is::flow_num:0x%x\n",ul_route_item.flow_num);
        for(i = 0; i < 257; i++)
        {
            if(i%2 == 0) Fprintf("\n");
            Fprintf("port_num[%d]:0x%x, ",i, ul_route_item.port_num[i]);
        }
        Fprintf("\n");
        break;
    }
    case UL_LINK:
    {
        STRU_SET_UPLINK_PATH_LINK_TABLE ulLinkData = {0};
        ulLinkData.PortSrc = tblItem;
        show_dps_ul_link_item(&ulLinkData);
        break;
    }
    case DL_ROUTE:
    {
        wp_dl_rt dl_route_item = {0};
        WP_DLlookupEntryDirectGet(tblItem, &dl_route_item);
        Fprintf("DPS DL Route is::flow_num:0x%x, bvci_n:%d(0x%x), nsei_i:%d(0x%x)\n", \
                dl_route_item.flow_num, dl_route_item.bvci_n, dl_route_item.bvci_n, dl_route_item.nsei_i, dl_route_item.nsei_i);
        break;
    }
    case DL_LINK:
    {
        STRU_SET_FR_DOWNLINK_PATH_LINK_TABLE dlLinkData = {0};
        dlLinkData.NSEI = tblItem;
        show_dps_dl_link_item(&dlLinkData);
        break;
    }
    case BVCI:
    {
        u16 hashRet;
        WP_status rt;
        rt = WPI_UserDefHashEntryDisplay(0,tblItem,pstru_get_tblItem->tableLoc[2],&hashRet);
        if(0 == rt)
        {
            Fprintf("DPS bvci: flowNum:0x%x, bvci_old:%d, bvci_new:%d\n", tblItem, pstru_get_tblItem->tableLoc[2], hashRet);
        }
        else
        {
            Fprintf("Error: hash failed. flowNum:0x%x, bvci_old:%d\n", tblItem, pstru_get_tblItem->tableLoc[2]);
        }
        break;
    }
    default:
    {
        Fprintf("Error: table number is out of bound!\n");
        break;
    }
    }
}
