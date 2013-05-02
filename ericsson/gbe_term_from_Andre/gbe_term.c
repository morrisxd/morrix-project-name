/*************************************************************
 * (C) Copyright 2001-2003, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ************************************************************/

/*****************************************************************************
 *
 * Module:
 * Purpose:
 *
 * Change History:
 * +------------+--------------------+----------------------------------------
 * | Date       | By                 | Description
 * +------------+--------------------+----------------------------------------
 * |            |                    |
 * +------------+--------------------+----------------------------------------
 * |            |                    |
 * +------------+--------------------+----------------------------------------
 *
 *****************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>       
#include <ctype.h>
#ifdef __linux__
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <netinet/in.h>
#else
#define htonl(x) x
#define ntohl(x) x
#define fflush(x)
#endif

#include "api/wp_wddi.h"
#include "api/wp_host_io.h"
#include "wpx_gei_phy.h"
#include "wpx_pin_mux_selector.h"
#include "wpx_app_data.h"
#include <wpx_board_data.h>
#include "wpx_enet_phy.h"

#define WUFE_UFE4

#include <wufe_types.h>
#include <wufe_errors.h>
#include <wufei_user_config.h>
#include <wufe_user_config.h>
#include <wufe_ufe.h>
#include <wufe_events.h>
#include <wpx_ufe/wpx_frmr/flexmux/WPX_UFE_FRAMER_API_public.h>

// For mmap driver use
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <netinet/ether.h>
#include "/home/andre/projects/XS/mmap_drv/xs_ioctl.h"

#define ATM
#define HDLCno
#define HDLC_TRANSno
#define HDLC_FRno
#define TRANSno

#define STM1_LOOPBACKno
#define ETH_LOOPBACKno

#define FRAMED

#define LINE_PROTECTIONno

#define TAG_ETH0    0x40
#define TAG_ETH0_L2 0x20
#define TAG_ATM0    0x30
#define TAG_FR0     0x50
#define TAG_HDLC0   0x60
#define TAG_TRANS0  0x70

extern WP_U32 dps_ProgramImage[];
extern WP_U16 dps_PC_Table_Init[];

static void   WPE_StatHsEnet(WP_handle hs_device);
static void   oc3_link_status(int atm_port);

#ifdef ATM
static void   printAtmDeviceStats(WP_handle handle);
static void   atmSend(WP_handle handle, int iter);
#endif

#ifdef HDLC_FR
static void   printFrDeviceStats(WP_handle handle, WP_handle tx_channel_handle, WP_handle rx_channel_handle);
static void   frSend(WP_handle handle, int iter);
#endif

#ifdef HDLC_TRANS
static void   printHdlcDeviceStats(WP_handle handle, WP_handle tx_channel_handle, WP_handle rx_channel_handle);
static void   hdlcSend(WP_handle handle, int iter);
#endif

#ifdef TRANS
static void   printTransDeviceStats(WP_handle handle, WP_handle tx_channel_handle, WP_handle rx_channel_handle);
static void   transSend(WP_handle handle, int iter);
#endif

static void   enet_link_status(int eth_port);
static void   iwport_stats(WP_handle iw_port);
static void   ethSend(WP_handle handle, int iter);

#ifndef __linux__
static int          xs_getchar(unsigned char *c, unsigned int block);
#endif

static WP_CHAR appname[] = "GBE TERM";

static int gserver;

static char lineAlrmStr[54][12] = 
    {
	"SDH_LOS",
	"SDH_LOF",
	"SDH_RS_TIM",
	"SDH_MS_AIS",
	"SDH_MS_RDI",
	"SDH_OOF",
	"SDH_MS_EXC",
	"SDH_MS_DEG",
	"SDH_AU_AIS",
	"SDH_AU_LOP",
	"SDH_HP_LOM",
	"SDH_HP_TIM",
	"SDH_HP_PLM",
	"SDH_HP_UNEQ",
	"SDH_HP_RDI",
	"SDH_HP_EXC",
	"SDH_HP_DEG",
	"SDH_LP_TIM",
	"SDH_LP_PLM",
	"SDH_LP_UNEQ",
	"SDH_LP_RDI",
	"SDH_LP_EXC",
	"SDH_LP_DEG",
	"SDH_TU_AIS",
	"SDH_TU_LOP",
	"SONET_LOS",
	"SONET_LOF",
	"SONET_TIM_S",
	"SONET_AIS_L",
	"SONET_RDI_L",
	"SONET_OOF",
	"SONET_EXC_L",
	"SONET_DEG_L",
	"SONET_LOM_P",
	"SONET_TIM_P",
	"SONET_PLM_P",
	"SONET_UNEQ_P",
	"SONET_RDI_P",
	"SONET_EXC_P",
	"SONET_DEG_P",
	"SONET_TIM_V",
	"SONET_PLM_V",
	"SONET_UNEQ_V",
	"SONET_RDI_V",
	"SONET_EXC_V",
	"SONET_DEG_V",
	"SONET_AIS_V",
	"SONET_LOP_V",
	"SONET_AIS_P",
	"SONET_LOP_P",
	"SDH_RS_EXC",
	"SDH_RS_DEG",
	"SONET_EXC_S",
	"SONET_DEG_S"
    };

static char clientAlrmStr[6][8] = 
    {
	"LOS",
	"LOF",
	"AIS",
	"AIS_CI",
	"RAI",
	"RAI_CI"   
    };

static char socketAlarmStr[3][8] =
    {
	"RFI",
	"RAI",
	"LOS_AIS"
    };

/* Interrupt queue configuration used with WP_SysInit */
static WP_int_queue_table int_queue_tables =
    {
        {
            {24, 1},
            {24, 1},
            {10, 1},
            {10, 1}
        }
    };

static WP_calendar_wheel calendar[2] =
{
   {
#ifdef ATM
      /* function */    WP_CW_ASU,
      /* cwid*/         WP_CW_ID_A,
      /* min_ch_rate */ 150,
      /* max_ch_rate */ 351000,
#else
      /* function */    WP_CW_PSU,
      /* cwid*/         WP_CW_ID_A,
      /* min_ch_rate */ 100000,
      /* max_ch_rate */ 155000000,
#endif
   },
   WP_CALENDAR_WHEEL_TERM
};

WP_atm_global atm_params[1] =
    {
        {
            /* max_pm_tables        */ 0,
            /* max_ubrplus_channels */ 0,
            /* max_upc_tables       */ 0,
            /* max_rx_cids          */ 0,
            /* max_tx_cids          */ 0,
            /* max_cid_range        */ 0,
            /* AAL2 QSR bus         */ WP_BUS_NONE,
            /* AAL2 QSR bank        */ APP_BANK_HOST
        }
    };

static WP_iw_global  max_iw[1] =
    {
        { 
            /* max_iw_aggregations           */ 14,
            /* max_r_iw_systems              */ 2,
            /* max_directmap_enet_systems    */ 1,
            /* max_b_iw_systems              */ 1,
            /* max_m_iw_systems              */ 0,
            /* max_vlan_priority_maps        */ 0,
            /* iw_bkgnd_fifo_size            */ 2000,
            /* cong_pt                       */ NULL,
            /* iw_host_limits                */ {64, 0, 0},
            /* mpls_config                   */ {0,0},
            /* iw_modes */
            {  /*policer_mode                */ WP_IW_POLICER_DISABLE,
               /*statistics_bundle_stat_mode */ WP_IW_STATISTICS_BUNDLE_STAT_DISABLE,
               /*l2_prefix_ext_mode          */ WP_IW_L2_HEADER_EXTENSION_DISABLE,
               /*enhanced_flow_stat_mode     */ WP_IW_ENHANCED_FLOW_STAT_DISABLE,
               /*flow_stat_mode              */ WP_IW_FLOW_STAT_ENABLE,
               /*fr_tx_flow_stat_mode        */ WP_IW_FR_TX_FLOW_STAT_DISABLE,
               /*mfc_alloc_mode              */ WP_IW_MFC_ALLOC_DISABLE,
               /*Learning_queue_mode         */ WP_IW_LEARNING_DISABLE,
               /*port_filtering_mode         */ WP_IW_PORT_FILTERING_DISABLE,
               /*lpm_alloc_mode              */ WP_IW_LPM_ALLOC_DISABLE,
               /* enhanced_mc_member_stat_mode */WP_IW_ENHANCED_MC_MEMBER_STAT_DISABLE
            },
            /* max_iw_stat_bundles           */ 0,
            /* max_fr_sw_systems             */ 0,
            /* max_iw_ports                  */ 14,
            /* max_iw_mc_groups              */ 0,
            /* max_nat_systems               */ 0,
            /* max_iw_second_aggregations    */ 0,
            /* WP_iw_compression_limits      */ NULL,
            /* max_iw_mc_members             */ 0,
            /* max_iw_programmable filters   */ 0,
            /* max_iw_policers               */ 0

        }
    };

static WP_context context[1] =
    {
        {6, /* qnodes */
	 /* the first four assignments are mandatory bus assignments */
	 {
	     /* WP_VB_PARAM = 0, 		      */{WP_BUS_PARAM, APP_BANK_PARAM},
	     /* WP_VB_PACKET, 			      */{WP_BUS_PACKET, APP_BANK_PACKET},
	     /* WP_VB_INTERNAL, 		      */{WP_BUS_INTERNAL, APP_BANK_INTERNAL},
	     /* WP_VB_HOST, 			      */{WP_BUS_HOST, APP_BANK_HOST},
	     /* WP_VB_INTQUEUE_DEBUG, 	              */{WP_BUS_PARAM, APP_BANK_PARAM},
	     /* WP_VB_TIMESTAMP, 		      */{WP_BUS_PARAM, APP_BANK_PARAM},
	     /* WP_VB_CAS, 			      */{WP_BUS_NONE, 0},
	     /* WP_VB_TRANS_TSAT, 		      */{WP_BUS_NONE, 0},
	     /* WP_VB_TPT, 			      */{WP_BUS_INTERNAL, APP_BANK_INTERNAL},
	     /* WP_VB_CW, 			      */{WP_BUS_INTERNAL, APP_BANK_INTERNAL},
	     /* WP_VB_ASU_TXQUEUE, 		      */{WP_BUS_PARAM, APP_BANK_PARAM},
	     /* WP_VB_STATISTICS, 		      */{WP_BUS_PACKET, APP_BANK_PACKET},
	     /* WP_VB_ATM_ADDLOOKUP, 		      */{WP_BUS_PARAM, APP_BANK_PARAM},
	     /* WP_VB_ATM_CPT_EBMT_EXTHEADER,         */{WP_BUS_PARAM, APP_BANK_PARAM},
	     /* WP_VB_ETH_HDLC_CPT, 		      */{WP_BUS_PARAM, APP_BANK_PARAM},
	     /* WP_VB_FBP_BD, 			      */{WP_BUS_PARAM, APP_BANK_PARAM},
	     /* WP_VB_FBP_DATA, 		      */{WP_BUS_PACKET, APP_BANK_PACKET},
	     /* WP_VB_L2_BKGND, 		      */{WP_BUS_PACKET, APP_BANK_PACKET},
	     /* WP_VB_BRIDGE_LEARNING_DFC_PFMT        */{WP_BUS_PACKET, APP_BANK_PACKET},
	     /* WP_VB_IMA_MLPPP_STATE, 		      */{WP_BUS_INTERNAL, APP_BANK_INTERNAL},
	     /* WP_VB_IMA_MLPPP_BUFFER, 	      */{WP_BUS_PACKET, APP_BANK_PACKET},
	     /* WP_VB_HASH_IW_LPM_EMC, 	 	      */{WP_BUS_PACKET, APP_BANK_PACKET},
	     /* WP_VB_IW_MFC_RULES, 		      */{WP_BUS_PACKET, APP_BANK_PACKET},
	     /* WP_VB_IW_FIWT_QAT, 		      */{WP_BUS_PARAM, APP_BANK_PARAM},
	     /* WP_VB_IW_MFCPT_DIFFSERV_UPPT_D        */{WP_BUS_INTERNAL, APP_BANK_INTERNAL},
	     /* WP_VB_STATISTICS2,                    */{WP_BUS_PACKET, APP_BANK_PACKET},
	     /* WP_VB_STATISTICS3,                    */{WP_BUS_PACKET, APP_BANK_PACKET},
	     /* WP_VB_IMA_RX_BUS, 		      */{WP_BUS_NONE, 0},
	     /* WP_VB_PKT_GLOBAL_TCPT, 		      */{WP_BUS_NONE, 0},
	     /* WP_VB_IMA_TX_BUS, 		      */{WP_BUS_NONE, 0},
	     /* WP_VB_UNASSIGNED9, 		      */{WP_BUS_NONE, 0},
	     /* WP_VB_UNASSIGNED10, 		      */{WP_BUS_NONE, 0},
	     /* WP_VB_MCH_DATA,		    	      */{WP_BUS_NONE,0},
	     /* WP_VB_OAM_FM_QUERY,	              */{WP_BUS_NONE,0},
	     /* WP_VB_HASH_ON_HEAP,	  	      */{WP_BUS_NONE,0},
	     /* WP_VB_MFC_INTERNAL,	              */{WP_BUS_PACKET, APP_BANK_PACKET},
	     /* WP_VB_MFR_RAS_TBL,                    */{WP_BUS_NONE,0}, 
	     /* WP_VB_CESOP_DATA,                     */{WP_BUS_PACKET,APP_BANK_PACKET}, 
	     /* WP_VB_SECURITY_ESE,                   */{WP_BUS_NONE,0},
	     /* WP_VB_PDCP_SECURITY_ENTITY,           */{WP_BUS_NONE,0},
	     /* WP_VB_POLICER,                        */{WP_BUS_NONE,0},
	     /* WP_VB_PCE_LRN,                        */{WP_BUS_NONE,0},
	     /* WP_VB_PCEA_PARAM,                     */{WP_BUS_NONE,0},
	     /* WP_VB_CONGESTION_TABLE,               */{WP_BUS_NONE,0},
	     /* WP_VB_PARAMS_PROPAGATION,             */{WP_BUS_NONE,0},  
	 },
         {
             dps_ProgramImage, dps_ProgramImage
         },
         {
             dps_PC_Table_Init, dps_PC_Table_Init
         },
         &int_queue_tables, /* Interrupt queue tables         */
         8,                 /* Mixumum number of pools        */
         atm_params,        /* ATM global parameters          */
         max_iw,            /* Interworking global parameters */
        }
    };

/* Giga Ethernet port1 configuration */
/* ENET port and device params */
static WP_port_enet gbe_s[1] =
    {
        {
            /* pkt_limits      */ {20, 20, 0, 0},
            /* flowmode */        WP_FLOWMODE_FAST,
            /* interface_mode */  WP_ENET_1000_BASE_X_AN,
            /* rx_iw_bkgnd */     WP_IW_BKGND_NOT_USED
        }
    };



static WP_device_enet gbe_dev[1] =
    {
        {
            /* WP_U16 max_tx_channels;             */ 10,
            /* WP_U16 tx_maxsdu;                   */ 1536,
            /* WP_U8  operating_speed;             */ WP_UNUSED,
            /* WP_U8  mac_addr[6];                 */ {0xAA,0xBB,0xCC,0xDD,0xEE,0xFF},
            /* WP_U32 tx_bit_rate;                 */ WP_TX_BIT_RATE_UNLIMITED,
            /* WP_U8  loopbackmode;                */ WP_ENET_NORMAL,
            /* WP_device_enet_ex *extended_params; */ NULL
        }
    };

/* Channel configuration for GigaEthernet */
static WP_ch_enet ch_config_gbe[1] =
    {
        {
            /* intmode           */ WP_PKTCH_INT_ENABLE,
            /* iwmmode           */ WP_PKTCH_IWM_ENABLE,
            /* testmode          */ WP_PKTCH_TEST_DISABLE,
            /* tx_pqblock        */ 0,
            /* tx_pqlevel        */ 0,
            /* tx_shaping_type   */ WP_FMU_SHAPING_TYPE_STRICT,
            /* tx_shaping_params */ NULL,
            /* rx_maxsdu         */ 1536,
            /* tx_cwid           */ 0,
            /* tx_tq             */ 0,
            /* rx_queuedepth     */ 256,
        }
    };

static WP_pool_buffer_data buffer_data[1] =
    {
        {
            /* n_buffers    */ 2048,
            /* offset       */ 64,
            /* size         */ 1984,
            /* pad          */ 0,
            /* databustype  */ WP_BUS_PACKET,
            /* bank         */ APP_BANK_PACKET
        }
    };

static WP_pool_ring_data ring_data[] = 
{
   {
      /* n_rings     */ 16,
      /* ring_length */ 128,
      /* bus         */ WP_BUS_PARAM,
      /* bank        */ APP_BANK_PARAM
   }
};

/* A Host termination queue node for the AAL5 channels */
static WP_qnode_hostq qn_host[1] =
{
   {
      /* pool_buffer    */ 0,
      /* pool_ring      */ 0,
      /* interruptqueue */ WP_IRQT1
   }
};

static WP_qnode_iwq qn_iw[1]=
    {
        {
            /* interruptqueue */ 0,
            /* num_buffers    */ 256,
            /* adjunct_pool   */ 0
        }
    };

static WP_dfc_sys_info dfc_sys_info[1] = {
    {
        WP_IW_128K_HASH_ENTRIES,
        WP_IW_PORT_FILTERING_DISABLE,
        WP_IW_DENY_EQUAL_IP_DISABLE
    }
};

static WP_iw_ipv4_sys_info ipv4_sys_one[1] =
{
   {
      /* classification_mode */ 		WP_IW_POLICY_BASED_ROUTING,
      /* max_lpm_flows */ 			0,
      /* *dfc_info  */ 				dfc_sys_info

   }
};

static WP_iw_sys_routing routing_sys[1]=
    {
        {
            /*  buffer_gap               */ 0x40,
            /*  max_iw_ports             */ 10,
            /*  max_classification_rules */ 10,
            /*  supported_protocols      */ WP_IW_SYS_IPV4,
            /* ipv4_sys_info             */ ipv4_sys_one,
            /* ipv6_sys_info             */ NULL,
            /* mpls_sys_info             */ NULL,
        }
    };

WP_rx_binding_routing rx_binding[1]=
    {
        {
            /*  encap_mode          */ 0,
            /*  l3_support_protocol */ WP_IW_IPV4,
            /*  mru                 */ 1536,
            /*  rx_direction        */ 0,
            /*  input_port          */ 0,
            /* decomp_group         */ 0
        }
    };

static WP_iw_agg_routing iw_agg_routing [1] = {
  
    {
        /* WP_tag tag                    */ TAG_ETH0,
        /* WP_handle txfunc              */ 0,
        /* dfsrv_remarking               */ WP_IW_DSRV_DISABLE,
        /* WP_U8 l2_header_insert_mode   */ WP_IW_L2H_INSERT_DISABLE,
        /* ip_header_replace_mode        */ WP_IW_FAST_FWD_MODE,
        /* WP_U8 interruptqueue          */ WP_IW_IRQT0,
        /* WP_U8 error_pkt_mode          */ WP_IW_ERRPKT_DISCARD,
        /* WP_U8 intmode                 */ WP_IW_INT_ENABLE,
        /* WP_U8 statmode                */ WP_IW_STAT_ENABLE,
        /* WP_U8 timestamp_mode          */ WP_IW_TIME_STAMP_DISABLE,
        /* WP_U8 ov_pool_mode            */ WP_IW_OV_POOL_DISABLE,
        /* WP_U16 fbp_drop_threshold     */ 0,
        /* WP_U16 mtu                    */ 1650,
        /* dfsrv_code_point              */ 0,
        /* WP_U8 prefix_length           */ 0,
        /* multi_cong_prof               */ WP_IW_MULTI_CONGESTION_DISABLE,
        /* WP_U8 prefix_header[48]       */ WP_IW_NULL_L2_ENCAPSULATION,
        /* WP_U8 new_ip_header[20]       */ WP_IW_NULL_L3_ENCAPSULATION ,
        /* WP_U8 policer_enable          */ WP_IW_POLICER_DISABLE,
        /* WP_policer_config   *policer_config;*/ NULL,
        /* WP_U8 cong_mode               */ WP_IW_CONGESTION_DISABLE,
        /* WP_iw_flow_agg_cong_thresh_param *cong_threshold_param */ NULL,
        /* WP_U8 dynamic_length_update   */ WP_IW_LENGTH_UPDATE_DISABLED,
        /* WP_U8 length_offset           */ 0,
        /* tx_direction                  */ 0,
        /* WP_flowaggtype flow_agg_type  */ 0,
        /* iw_port                       */ 0,
        /* WP_handle comp_group_handle   */ 0
    }
};

static WP_tx_binding  tx_qos_iw[1]=
    {
        {
            /* wred_mode;*/ 0,
            /* dci_mode;*/  WP_IW_DYN_CH_INSERT_ENABLE,
            /* maxt;*/1000,
        }
    };

static WP_ch_iw_rx rx_ch [1] = {
    {
        /* pqblock           */ 0,
        /* pqlevel           */ 0,
        /* tx_binding_type   */ WP_IW_TX_BINDING,
        /* tx_binding_config */ tx_qos_iw
    }
};
static WP_iw_route_port route_port[1] =
{
   { 
      /* tag */ 				0x02,
      /* input_filters_mask; */ 		0,
      /* output_filters_mask; */ 		0,
      /* statmode; */ 				WP_IW_PORT_STAT_ENABLE,
      /* forwarding_mode */ 			WP_IW_FORWARDING_DIRECT_MAP,
      /* flow_agg */                            0,
      /* ipv6_source_address_validation */      0,
      /* ipv6_host_term_mode */                 0
   }
};

static WP_port_upi_iw_ext emphy_iw_config[1] =
{
   {
       1,
       0
   }
};

/* Interrupt related functions */
static void My_AppIndicateEventRx(WP_tag tag, WP_U32 data, WP_U32 info);
static void My_AppIndicateEventTx(WP_tag tag, WP_U32 data, WP_U32 info);
static void My_AppIndicateErrors(WP_tag tag, WP_U32 data, WP_U32 info);
static void terminate_on_error(WP_handle, WP_CHAR *);
static void flexmux_terminate_on_error(WP_handle handle, WP_CHAR *s);
static void wufe_terminate_on_error(WP_handle handle, WP_CHAR *s);

static void fileLineError(const WP_CHAR *file, const WP_CHAR *function, WP_S32 line, WP_S32 error_id) {
    printf("Error %x - file %s, function %s, line %d\n", error_id, file, function, line);
    return;
}

#ifdef USE_WDDI_LOG
static void wddiLog(int level, char *msg) {
    
    printf("WDDI LOG: %s\n", msg);

    return;
}
#endif

static WP_handle        qniw;
static WP_handle        qniw_enet;
static WP_handle        tx_enet_handle[10], rx_enet_handle[10];
static WP_handle        tx_tdm_handle[4], rx_tdm_handle[4];
static WP_handle        iwbuffer_pool, buffer_pool;
static WP_handle        gbep[10], gbedev[10], tdm_device[4];
static WP_handle        iwp,iwdev;
static WP_handle        iw_sys,iw_agg_g[10], rx_host_term;

static int eth_tx_no;
static int eth_rx_no;
static int atm_tx_no;
static int atm_rx_no;

#define	MAP_WN0_TSTAT_0	 0xbe014204
#define	MAP_WN0_TSTAT_1	 0xbe014244
#define	MAP_WN1_TSTAT_0	 0xbe016204
#define	MAP_WN1_TSTAT_1	 0xbe016244
#define	MAP_WN2_TSTAT_0	 0xbe018204
#define	MAP_WN2_TSTAT_1	 0xbe018244
#define	MAP_WN3_TSTAT_0	 0xbe01a204
#define	MAP_WN3_TSTAT_1	 0xbe01a244
#define	MAP_WN4_TSTAT_0	 0xbe01c204
#define	MAP_WN4_TSTAT_1	 0xbe01c244
#define	MAP_WN5_TSTAT_0	 0xbe01e204
#define	MAP_WN5_TSTAT_1	 0xbe01e244
#define	MAP_WN6_TSTAT_0	 0xbe020204
#define	MAP_WN6_TSTAT_1	 0xbe020244
#define	MAP_WN7_TSTAT_0	 0xbe022204
#define	MAP_WN7_TSTAT_1	 0xbe022244

#ifdef __linux__
static sem_t sema;
static sem_t mbox_sema;

static WP_tag g_tag;
static WP_U32 g_data;
static WP_U32 g_info;

static void process_enet_l2(WP_tag tag, WP_U32 data, WP_U32 info);
static void process_enet(WP_tag tag, WP_U32 data, WP_U32 info);
static void process_atm(WP_tag tag, WP_U32 data, WP_U32 info);
static void process_fr(WP_tag tag, WP_U32 data, WP_U32 info);
static void process_trans(WP_tag tag, WP_U32 data, WP_U32 info);
static void process_hdlc(WP_tag tag, WP_U32 data, WP_U32 info);

#define LINK_STATUS_OK 0
#define LINK_STATUS_KO 1

static int stm1_link_status(int port)
{
    WP_status link_status = LINK_STATUS_KO;

#ifdef ATM
    WP_status status;
    WP_stats_upi_atm atm_upi_stats;
    
    status = WP_DeviceStatistics(tdm_device[port], &atm_upi_stats);
    if(status) {
	terminate_on_error(status, "WP_DeviceStatistics()");
    }

    if(atm_upi_stats.emphy_stats.rx_sync_state == WP_DEV_EMPHY_IN_SYNC) {
	link_status = LINK_STATUS_OK;
    } else {
	link_status = LINK_STATUS_KO;
    }
#endif
    
    return link_status;
}

static int eth_link_status(int port)
{
    unsigned int tstat, tmp, reg_val;
    int status;

    tstat = 0;

    switch(port) {
    case 0 :
	tstat = MAP_WN1_TSTAT_0;
	break;
    case 1 :
	tstat = MAP_WN2_TSTAT_0;
	break;
    case 2 :
	tstat = MAP_WN1_TSTAT_1;
	break;
    default :
	tstat = MAP_WN1_TSTAT_0;
    }

    tstat &= 0x1fffffff;

    tmp = *(volatile unsigned int *)(unsigned long)(tstat + WPL_RIF_VIRTUAL(0, 0));
    reg_val = ntohl(tmp);

    if(reg_val & 0x4) {
	status = LINK_STATUS_OK;
    } else {
	status = LINK_STATUS_KO;
	
    }

    return status;
}

static void poll_eth_status(void) 
{
    int i, iter;
    unsigned int status;
    unsigned int last_status[3];

    for(i=0; i<3; i++) {
	last_status[i] = LINK_STATUS_KO;
    }

    iter = 0;

    while(1) {
	for(i=0; i<3; i++) {
	    status = eth_link_status(i);
	    if(last_status[i] != status) {
		if(status == LINK_STATUS_KO) {
		    printf("port %d went DOWN\n", i);
		} else {
		    printf("port %d went UP\n", i);
		}
	    }
	    last_status[i] = status;

	    if(gserver == 0) {
		while(sleep(1) != 0);
		
		printf(".");fflush(stdout);
		
		if(iter++ == 5) {
		    if((eth_link_status(1) == LINK_STATUS_OK) && (eth_link_status(2) == LINK_STATUS_OK)) {
			printf("Port 1 and Port 2 OK\n");
			exit(0);
		    } else {
			printf("Port 1 and/or Port 2 KO, sleeping for debug\n");
			sleep(10000);
		    }
		}
	    }
	}
    }

    return;
}

/*
  SFP PORT          ETH PORT          STM-1 PORT
  ----------------------------------------------
  0                 -                 0
  1                 2                 -
  2                 1                 -
  3                 0                 1
*/
static int link_status(int sfp_port)
{
    int status = LINK_STATUS_KO;
    unsigned char mode;

    switch(sfp_port) {
    case 0 :
	status = stm1_link_status(0);
	break;

    case 1 :
	status = eth_link_status(2);
	break;

    case 2 :
	status = eth_link_status(1);
	break;

    case 3 :
	status = WPX_PortModeGet(0, &mode, 3);
	//terminate_on_error(status, "WPX_PortModeGet");

	if(mode == WPX_PORT_MODE_ETH) {
	    status = eth_link_status(0);
	} else {
	    status = stm1_link_status(1);
	}
	break;
    default :
	printf("Invalid sfp port (%d)\n", sfp_port);
    }

    //printf("port %d status = %d\n", sfp_port, status);

    return status;
}

static int kill_all_threads = 0;

static void *mailbox_handler(void *dummy)
{
    WP_U32                   thread_params[2];

    // This thread will server the framer

    thread_params[0] = 0;
    thread_params[1] = 0;

    while(1) {

	if(kill_all_threads) pthread_exit(NULL);

	// Wait on semaphore
	sem_wait(&mbox_sema);

	printf("$");fflush(stdout);

	// Serve the mailbox
       WPX_UFE_FRAMER_FlexmuxServiceMailbox(thread_params);
    }

    return NULL;
}

static void *led_handler(void *dummy)
{
    int i;

    // This thread will check all 4 SFP ports every second and will update the LEDs status accordingly

    while(1) {

	if(kill_all_threads) pthread_exit(NULL);

	for(i=0; i<4; i++) {
	    if(link_status(i) == LINK_STATUS_OK) {
		WPX_PortLedSet(0, WPX_PORT_LED_ON, i);
	    } else {
		WPX_PortLedSet(0, WPX_PORT_LED_OFF, i);
	    }
	}

	sleep(1);
    }

    return NULL;
}

static void *event_handler( void *dummy)
{
    WP_tag tag;
    WP_U32 data;
    WP_U32 info;

    while(1) {

	if(kill_all_threads) pthread_exit(NULL);

	sem_wait(&sema);

	tag  = g_tag;
	data = g_data;
	info = g_info;
	
	//printf("event_handler: tag=%x data=%x info=%x\n", tag, data, info); fflush(stdout);
	
	if((tag >= TAG_ETH0) && (tag < TAG_ETH0+4)) {
	    process_enet(tag, data, info);
	} else if((tag >= TAG_ETH0_L2) && (tag < TAG_ETH0_L2+10)) {
	    process_enet_l2(tag, data, info);
	} else if((tag >= TAG_ATM0) && (tag < TAG_ATM0 + 4)) {
	    process_atm(tag, data, info);
	} else if((tag >= TAG_FR0) && (tag < TAG_FR0 + 4)) {
	    process_fr(tag, data, info);
	} else if((tag >= TAG_TRANS0) && (tag < TAG_TRANS0 + 4)) {
	    process_trans(tag, data, info);
	} else if((tag >= TAG_HDLC0) && (tag < TAG_HDLC0 + 4)) {
	    process_hdlc(tag, data, info);
	} else {
	    printf("Packet Received with unknown tag %x data %x info %x\n", tag, data, info);
	}
    }

    return NULL;
}
#endif

void displayUfeEvents(void) 
{
    WUFE_events events;
    WP_status   status;
    WP_U16      event;
    //    WP_U32      i;

    status = WUFE_UfeReadEvents(0, &events);
    wufe_terminate_on_error(status, "WUFE_UfeReadEvents()");  

    //printf("\nUFE Events:\n");
    //printf("-----------\n");

    // CORE
    event = events.core_events[0].event_reg;
    if (event & WUFE_EVENT_CORE) {
	if (event & WUFE_EVENT_CORE_INGRESS_FBP_OV)
	    printf("\tCORE : WUFE_EVENT_CORE_INGRESS_FBP_OV\n");
	if (event & WUFE_EVENT_CORE_EGRESS_FBP_OV)
	    printf("\tCORE : WUFE_EVENT_CORE_EGRESS_FBP_OV\n");
	if (event & WUFE_EVENT_CORE_EGRESS_PHY_UNDERRUN) {
	    printf("\tCORE : WUFE_EVENT_CORE_EGRESS_PHY_UNDERRUN\n");
	    printf("\t\ttx_phy_underrun_handle = 0x%x\n",
		   events.core_events->tx_phy_underrun_handle);
	}
    }

    // EMPHY
    event = events.emphy_events[0].event_reg; 
    if (event & WUFE_EVENT_EMPHY) {
      if (event & WUFE_EVENT_EMPHY_INGRESS_BUS)
         printf("\tEMPHY: WUFE_EVENT_EMPHY_INGRESS_BUS\n");
      if (event & WUFE_EVENT_EMPHY_INGRESS_MAX_BUFF_SIZE)
         printf("\tEMPHY: WUFE_EVENT_EMPHY_INGRESS_MAX_BUFF_SIZE\n");
      if (event & WUFE_EVENT_EMPHY_INGRESS_IF)
         printf("\tEMPHY: WUFE_EVENT_EMPHY_INGRESS_IF\n");
      if (event & WUFE_EVENT_EMPHY_EGRESS_PHY_NOT_VALID)
      {
         printf("\tEMPHY: WUFE_EVENT_EMPHY_EGRESS_PHY_NOT_VALID\n");
         printf("\t\tphy_not_valid_handle = 0x%x\n",
                events.emphy_events->phy_not_valid_handle);
      }
      if (event & WUFE_EVENT_EMPHY_EGRESS_BUS)
         printf("\tEMPHY: WUFE_EVENT_EMPHY_EGRESS_BUS\n");
      if (event & WUFE_EVENT_EMPHY_EGRESS_IF )
         printf("\tEMPHY: WUFE_EVENT_EMPHY_EGRESS_IF\n");
      if (event &WUFE_EVENT_EMPHY_EGRESS_PARITY )
         printf("\tEMPHY: WUFE_EVENT_EMPHY_EGRESS_PARITY\n");
    }

    // TDM
    event = events.tdm_events.event_reg;
    if (event & WUFE_EVENT_TDM)	{
	if (event & WUFE_EVENT_TDM_RX_PARITY)
	    printf("\tTDM  : WUFE_EVENT_TDM_RX_PARITY\n");
	if (event & WUFE_EVENT_TDM_TX_PARITY)
	    printf("\tTDM  : WUFE_EVENT_TDM_TX_PARITY\n");
    } 
    
    // FM
    event = events.fm_events.event_reg;
    if (event & WUFE_EVENT_FM) {
	if (event & WUFE_EVENT_FM_RX_DDR_OVERRUN)
	    printf("\tFM   : WUFE_EVENT_FM_RX_DDR_OVERRUN\n");
	if (event & WUFE_EVENT_FM_RX_BURST_OVERRUN)
	    printf("\tFM   : WUFE_EVENT_FM_RX_BURST_OVERRUN\n");
	if (event & WUFE_EVENT_FM_TX_DDR_UNDERRUN)
	    printf("\tFM   : WUFE_EVENT_FM_TX_DDR_UNDERRUN\n");
	if (event & WUFE_EVENT_FM_TX_BURST_UNDERRUN)
	    printf("\tFM   : WUFE_EVENT_FM_TX_BURST_UNDERRUN\n");
	if (event & WUFE_EVENT_FM_RX_ECC_ONE_BIT)
	    printf("\tFM   : WUFE_EVENT_FM_RX_ECC_ONE_BIT\n");
	if (event & WUFE_EVENT_FM_TX_ECC_ONE_BIT)
	    printf("\tFM   : WUFE_EVENT_FM_TX_ECC_ONE_BIT\n");
	if (event & WUFE_EVENT_FM_TX_READ_FIFO_OVERRUN)
	    printf("\tFM   : WUFE_EVENT_FM_TX_READ_FIFO_OVERRUN\n");
    } 

    // MACHINE
    event = events.machine_events.rx_event_reg;
    if (event & WUFE_EVENT_MACHINE_RX) {
	if (event & WUFE_EVENT_MACHINE_CES_RX_LOSS)
	    printf("\tRX MACHINE : WUFE_EVENT_MACHINE_CES_RX_LOSS\n");
	if (event & WUFE_EVENT_MACHINE_CES_RX_MF_LOSS)
	    printf("\tRX MACHINE : WUFE_EVENT_MACHINE_CES_RX_MF_LOSS\n");
	if (event & WUFE_EVENT_MACHINE_ATM_RX_LCD)
	    printf("\tRX MACHINE : WUFE_EVENT_MACHINE_ATM_RX_LCD\n");
	if (event & WUFE_EVENT_MACHINE_HDLC_RX_ABORT)
	    printf("\tRX MACHINE : WUFE_EVENT_MACHINE_HDLC_RX_ABORT\n");
	if (event & WUFE_EVENT_MACHINE_HDLC_RX_CRC_ERR)
	    printf("\tRX MACHINE : WUFE_EVENT_MACHINE_HDLC_RX_CRC_ERR\n");
	if (event & WUFE_EVENT_MACHINE_HDLC_RX_NON_OCTET)
	    printf("\tRX MACHINE : WUFE_EVENT_MACHINE_HDLC_RX_NON_OCTET\n");
	if (event & WUFE_EVENT_MACHINE_POS_RX_ABORT)
	    printf("\tRX MACHINE : WUFE_EVENT_MACHINE_POS_RX_ABORT\n");
	if (event & WUFE_EVENT_MACHINE_POS_CRC_ERR)
	    printf("\tRX MACHINE : WUFE_EVENT_MACHINE_POS_CRC_ERR\n");
	if (event & WUFE_EVENT_MACHINE_RX_OVERRUN_BURST)
	    printf("\tRX MACHINE : WUFE_EVENT_MACHINE_RX_OVERRUN_BURST\n");
	if (event & WUFE_EVENT_MACHINE_RX_OVERRUN_DDR)
	    printf("\tRX MACHINE : WUFE_EVENT_MACHINE_RX_OVERRUN_DDR\n");
    } 

    event = events.machine_events.tx_event_reg;

    if (event & WUFE_EVENT_MACHINE_TX) {
	if (event & WUFE_EVENT_MACHINE_CES_TX_LOSS)
	    printf("\tTX MACHINE : WUFE_EVENT_MACHINE_CES_TX_LOSS\n");
	if (event & WUFE_EVENT_MACHINE_TX_UNDERRUN_BURST)
	    printf("\tTX MACHINE : WUFE_EVENT_MACHINE_TX_UNDERRUN_BURST\n");
	if (event & WUFE_EVENT_MACHINE_TX_UNDERRUN_DDR)
	    printf("\tTX MACHINE : WUFE_EVENT_MACHINE_TX_UNDERRUN_DDR\n");
    } 

    // BS
    event = events.bs_events.pll_lost_lock_event_reg;
    if(event & 0x400) printf("\tBS LOCK : TX EMPHY PLL LOST CLOCK\n");
    if(event & 0x200) printf("\tBS LOCK : RX EMPHY PLL LOST CLOCK\n");
    if(event & 0x080) printf("\tBS LOCK : DDR1 PLL LOST CLOCK\n");
    if(event & 0x040) printf("\tBS LOCK : DDR0 PLL LOST CLOCK\n");
    if(event & 0x020) printf("\tBS LOCK : DIFF PLL LOST CLOCK\n");
    if(event & 0x010) printf("\tBS LOCK : ADAPT PLL LOST CLOCK\n");
    if(event & 0x008) printf("\tBS LOCK : EXT PLL LOST CLOCK\n");
    if(event & 0x004) printf("\tBS LOCK : FRMR PLL LOST CLOCK\n");
    if(event & 0x003) printf("\tBS LOCK : DDR PLL LOST CLOCK\n");
    if(event & 0x002) printf("\tBS LOCK : SYSCLK PLL LOST CLOCK\n");

#if 0
    event = events.bs_events.absent_event_reg;
    printf("absent reg = %x\n", event);
    for(i=0; i<4; i++) {
	if(event & (1 << i)) {
	    printf("\tBS SFP : SFP absent\n");
	} else {
	    printf("\tBS SFP : SFP OK\n");
	}
    }

    event = events.bs_events.los_event_reg;
    printf("los reg = %x\n", event);
    for(i=0; i<4; i++) {
	if(event & (1 << i)) {
	    printf("\tBS LOS : Loss of signal\n");
	} else {
	    printf("\tBS LOS : Signal OK\n");
	}
    }

    // Display TX_DISABLE
    for(i=0; i<4; i++) {
	unsigned short txd;

	txd = *(volatile unsigned short *)(unsigned long)(0x1f000000 + WPL_PHY_VIRTUAL(0, 0) + 0x330000 + 0x88);
	printf("\tBS TXD : %d\n", txd & (1<<i));
    }
#endif

    return;
}

WP_U32 ufe_e1_slots[] = {
   1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15,
   17, 18, 19, 20, 21,22, 23, 24, 25, 26, 27, 28, 29, 30, 31
};

#ifndef O_NOFOLLOW
#define O_NOFOLLOW     00400000        /* don't follow links */
#endif

int print_winmon_env (void)
{
#if 0
    int               fd;
    XS_FILE_DESC      desc;
    unsigned char     buf[2048], offset;
    char              *str, *ptr, *saveptr;
    struct ether_addr *addr;

    fd = open("/dev/wp_bank24", O_RDWR|O_NOFOLLOW, 0644);
    if(fd == -1) {
	perror("open");
	exit(1);
    }
	
    desc.buf = buf;

    if(ioctl(fd, XS_IOCTL_ENV_GET, &desc) != 0) {
	close(fd);
	printf("XS_IOCTL_ENV_GET failed\n");
	perror("ioctl");
	exit(1);
    }

    close(fd);

    printf("\n\n==================================================================\n\n");
    printf("               WinMon environment variables:\n\n");
	   

    // Print all environment variables
    printf("%s\n", buf);

    // Get Serial and MACs
    ptr = (char *)buf;
    while((str = strtok_r(ptr, "\n", &saveptr)) != NULL) {
	if(!strncmp(str, "BOARD_MAC_ETH0", strlen("BOARD_MAC_ETH0"))) {
	    offset = strlen("BOARD_MAC_ETH0") + 1;
	    addr = ether_aton(&str[offset]);
	    printf("MAC for port 0 is %02x:%02x:%02x:%02x:%02x:%02x\n", 
		   addr->ether_addr_octet[0],
		   addr->ether_addr_octet[1],
		   addr->ether_addr_octet[2],
		   addr->ether_addr_octet[3],
		   addr->ether_addr_octet[4],
		   addr->ether_addr_octet[5]);
	}
	if(!strncmp(str, "BOARD_MAC_ETH1", strlen("BOARD_MAC_ETH1"))) {
	    offset = strlen("BOARD_MAC_ETH1") + 1;
	    addr = ether_aton(&str[offset]);
	    printf("MAC for port 1 is %02x:%02x:%02x:%02x:%02x:%02x\n", 
		   addr->ether_addr_octet[0],
		   addr->ether_addr_octet[1],
		   addr->ether_addr_octet[2],
		   addr->ether_addr_octet[3],
		   addr->ether_addr_octet[4],
		   addr->ether_addr_octet[5]);
	}
	if(!strncmp(str, "BOARD_MAC_ETH2", strlen("BOARD_MAC_ETH2"))) {
	    offset = strlen("BOARD_MAC_ETH2") + 1;
	    addr = ether_aton(&str[offset]);
	    printf("MAC for port 2 is %02x:%02x:%02x:%02x:%02x:%02x\n", 
		   addr->ether_addr_octet[0],
		   addr->ether_addr_octet[1],
		   addr->ether_addr_octet[2],
		   addr->ether_addr_octet[3],
		   addr->ether_addr_octet[4],
		   addr->ether_addr_octet[5]);
	}

	if(!strncmp(str, "BOARD_SERIAL", strlen("BOARD_SERIAL"))) {
	    offset = strlen("BOARD_SERIAL") + 1;
	    printf("Serial is %d\n", atoi(&str[offset]));
	}

	ptr = NULL;
    }

    printf("==================================================================\n\n");
#endif
    return 0;
}


static void alarm_cback_port(U8 iDevice, U8 iLinePort, U8 AlarmCategory, U8 IsAsserted)
{
    printf("PORT    : device = %d, linePort = %d, category = %s, isAsserted = %d\n", 
	   iDevice, iLinePort, lineAlrmStr[AlarmCategory], IsAsserted);

    return;
}

static void alarm_cback_path(U8 iDevice, 
			     WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pLineEndpoint, 
			     U8 AlarmCategory, 
			     U8 IsAsserted)
{
    printf("PATH    : device = %d, stm4=%d, stm1=%d, stm0=%d, tug2=%d,tu=%d, category = %s, isAsserted = %d\n", 
	   iDevice,
	   pLineEndpoint->u.SDH.stm4,
	   pLineEndpoint->u.SDH.stm1,
	   pLineEndpoint->u.SDH.stm0,
	   pLineEndpoint->u.SDH.tug2,
	   pLineEndpoint->u.SDH.tu,
	   lineAlrmStr[AlarmCategory], 
	   IsAsserted);
    
    return;
}

static void alarm_cback_client(U8 iDevice, 
			       U8 iDiscreteClientPort, 
			       U8 AlarmCategory, 
			       U8 IsAsserted)
{
    printf("CLIENT  : device = %d, clientPort = %4d, category = %s, isAsserted = %d\n", 
	   iDevice, iDiscreteClientPort, clientAlrmStr[AlarmCategory], IsAsserted);

    return;
}

static void alarm_cback_discrete(U8  iDevice, 
				 U32 iSocketClientPDH, 
				 U8  AlarmCategory, 
				 U8  IsAsserted)
{
    printf("DISCRETE: device = %d, clientPDH = %4d, category = %s, isAsserted = %d\n", 
	   iDevice, iSocketClientPDH, socketAlarmStr[AlarmCategory], IsAsserted);

    return;
}

static void alarm_cback_k1k2(U8 iDevice, 
			     U8 iLinePort, 
			     WPX_UFE_FRAMER_SONET_SDH_PORT_LINE_K1K2_TYPE *pRX_K1K2)
{
    printf("K1/K2   : device = %d, linePort = %d, k1 = %x, k2 = %x\n", 
		   iDevice, 
		   iLinePort, 
		   pRX_K1K2->K1,
		   pRX_K1K2->K2);

    return;
}

static void alarm_cback_S1(U8 iDevice, U8 iLinePort, U8 S1)
{
    printf("S1      : device = %d, linePort = %d, S1 = %x\n", 
	   iDevice, 
	   iLinePort, 
	   S1);

    return;
}

static void alarm_cback_pm_port(U8 iDevice, U8 iTableIndex, U32 TimeStamp)
{
    printf("PM      : PORT    : %d %4x %8x\n",  iDevice, iTableIndex, TimeStamp);

    return;
}

static void alarm_cback_pm_hpath(U8 iDevice, U8 iTableIndex, U32 TimeStamp)
{
    printf("PM      : HPATH   : %d %4x %8x\n",  iDevice, iTableIndex, TimeStamp);

    return;
}

static void alarm_cback_pm_lpath(U8 iDevice, U8 iTableIndex, U32 TimeStamp)
{
    printf("PM      : LPATH   : %d %4x %8x\n",  iDevice, iTableIndex, TimeStamp);

    return;
}

static void alarm_cback_pm_client(U8 iDevice, U8 iTableIndex, U32 TimeStamp)
{
    printf("PM      : CLIENT  : %d %4x %8x\n",  iDevice, iTableIndex, TimeStamp);

    return;
}

static void alarm_cback_pm_discrete(U8 iDevice, U8 iTableIndex, U32 TimeStamp)
{
    printf("PM      : DISCRETE: %d %4x %8x\n",  iDevice, iTableIndex, TimeStamp);

    return;
}

#ifdef __linux__
void WT_SgmiiAnInterrupt(WP_U32 wpid, WP_U32 signal_info)
{
    unsigned int tadir, tmp;

    printf("an\n"); fflush(stdout);

    tmp = *(volatile unsigned int *)(unsigned long)(0x1e016234 + WPL_RIF_VIRTUAL(0, 0));
    tadir = ntohl(tmp);

    if(tadir & 1) {
	printf("Port 0: AN interrupt: AN failed\n");
    }

    if(tadir & 2) {
	printf("Port 0: AN interrupt: AN complete\n");
    }

    tmp = *(volatile unsigned int *)(unsigned long)(0x1e018234 + WPL_RIF_VIRTUAL(0, 0));
    tadir = ntohl(tmp);

    if(tadir & 1) {
	printf("Port 1: AN interrupt: AN failed\n");
    }

    if(tadir & 2) {
	printf("Port 1: AN interrupt: AN complete\n");
    }

    tmp = *(volatile unsigned int *)(unsigned long)(0x1e016274 + WPL_RIF_VIRTUAL(0, 0));
    tadir = ntohl(tmp);

    if(tadir & 1) {
	printf("Port 2: AN interrupt: AN failed\n");
    }

    if(tadir & 2) {
	printf("Port 2: AN interrupt: AN complete\n");
    }

    return;
}

void WT_SerdesInterrupt(WP_U32 wpid, WP_U32 signal_info)
{
    unsigned int i, int_reg, sta_reg, tmp;

    printf("serdes\n"); fflush(stdout);

    // read and clear
    tmp = *(volatile unsigned int *)(unsigned long)(0x1e0082ec + WPL_RIF_VIRTUAL(0, 0));
    *(volatile unsigned int *)(unsigned long)(0x1e0082ec + WPL_RIF_VIRTUAL(0, 0)) = tmp;
    int_reg = ntohl(tmp);

    tmp = *(volatile unsigned int *)(unsigned long)(0x1e0082cc + WPL_RIF_VIRTUAL(0, 0));
    sta_reg = ntohl(tmp);
    
    for(i=0; i<3; i++) {
	if(int_reg & (1 << (4*i))) {
	    if(sta_reg & (4 << (12-(4*i)))) {
		printf("Port %d: RPLL OK\n", i);
	    } else {
		printf("Port %d: RPLL KO\n", i);
	    }
	}
	if(int_reg & (2 << (4*i))) {
	    if(sta_reg & (2 << (12-(4*i)))) {
		printf("Port %d: Signal OK\n", i);
	    } else {
		printf("Port %d: Signal KO\n", i);
	    }
	}
	if(int_reg & (4 << (4*i))) {
	    if(sta_reg & (1 << (12-(4*i)))) {
		printf("Port %d: Rx Locked\n", i);
	    } else {
		printf("Port %d: Rx Lock failed\n", i);
	    }
	}
    }

    return;
}

void WT_Eint3Interrupt(WP_U32 wpid, WP_U32 signal_info)
{
   WP_boolean is_framer_int, is_core_int;
   WUFE_events active_events;
   extern void WPX_UFE_FRAMER_ISR(WP_U8 iDevice);

   //printf("------------------------------> WT_Eint3Interrupt\n");fflush(stdout);

   memset(&active_events, 0, sizeof(WUFE_events));

   is_framer_int = WUFE_UfeFramerHwInterruptGet(0);
   is_core_int   = WUFE_UfeCoreHwInterruptGet(0);

   if (is_framer_int) {
       //printf("Framer interrupt\n");

       //sem_post(&mbox_sema);

       WPX_UFE_FRAMER_ISR(0);

#if !defined(WPL_MAILBOX_LOCK_KEY)
#define WPL_MAILBOX_LOCK_KEY WPL_LOCK_KEY_CREATE(WPL_HW_LOCK, WPL_PRIVATE_LOCK, 6, 0)
#endif

       WPL_Unlock(WPL_MAILBOX_LOCK_KEY, 0);
   }

   if (is_core_int) {
       //printf("Core interrupt\n");
       displayUfeEvents();
   }

   return;
}
#endif

typedef struct{
        WP_U32 flow_num;//this field can hold 2 FIWTs
        WP_U32 port_num[256];//port_num[64];
}wp_globle_rt;

WP_S32 main(WP_S32 argc, WP_CHAR **argv)
{
    WP_handle                eth_port, atm_port, status;
    WP_U32                   i, j, bytes, num_eth_ports;
    WP_handle                iw_port_enet[10];
    WP_handle                pool_ring, qnhost;
    WP_device_enet_ex        enet_ext;
    WP_enet_filters          enet_filters;
    WP_port_upi              upi_emphy_port_config;
    WP_handle                emphy_port;
    WUFE_init_config         config;
    WUFE_system              sys_cfg;
    WP_port_pkt_limits       pkt_limits;
    WP_port_upi_packet_ext   upi_packet_ext;
    WUFE_handle              phy_handle[4];
    WUFE_handle              line_handle[4];
    WUFE_line_sdh            line_config;
    WUFE_phy                 phy_cfg;
    WP_U32                   client_port_id, ufe_phy_index;
    WP_port_upi_atm_ext      upi_atm_ext;
    WP_port_atm_limits       atm_limits;
    WP_U32                   serdes_mode;
    WP_port_upi_emphy_ext    emphy_config;
    WP_THREAD_ID             mailbox_thread_id;
    WP_U32                   thread_params[2];

#ifdef ATM
    WP_device_emphy_atm      upi_emphy_atm_device;
    WP_atm_shaping_unshaped  aal5_shaping_param;
    WP_ch_aal5               aal5_ch_config;
#endif

#ifdef HDLC
    WP_pkt_shaping_wfq       pkt_shaping_param;
#ifdef HDLC_FR
    WP_device_emphy_fr       upi_emphy_fr_device;
    WP_qnode_fr_hostq        qn_fr;
    WP_handle                qnfrtx;
    WP_ch_fr                 fr_ch_config;
#endif
#ifdef HDLC_TRANS
    WP_device_emphy_hdlc_ppp upi_pos_device;
    //WP_device_emphy_trans    upi_trans_device;
    WP_ch_hdlc               hdlc_ch_config;
#endif
#endif

#ifdef TRANS
    WP_device_emphy_trans    upi_trans_device;
    WP_ch_trans_pwe3         trans_ch_config;
    WP_rx_binding_directmap  rx_binding_trans;
    WP_iw_agg_directmap_enet agg_dm;
    WP_handle                iw_agg_trans[4], iw_sys_trans;
    WP_iw_sys_directmap_enet trans_system;
    WP_feature_iw_cesop      cesop_feature_cfg;
#endif

    WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE line_end_point;

#ifdef __linux__
    pthread_t                thread_id_event, thread_id_led, thread_id_mbox;
#endif
    unsigned int             alarmCategory;

    atm_port = 0; // front panel 0
    eth_port = 0; // front panel 2

#if 0
    if(argc > 1) {
	sscanf(argv[1], "%x", &atm_port);
    }
    if(argc > 2) {
	sscanf(argv[2], "%x", &eth_port);
    }
#else
    if(argc > 1) {
	if((argv[1][0] == '-') && (argv[1][1] == 's')) {
	    gserver = 1;
	} else {
	    gserver = 0;
	}
    }
#endif

    printf("%s: atm_port = %d, gbe port = %d\n", appname, atm_port, eth_port);

    printf("%s: running in ", appname);
#ifdef ATM
    printf("ATM mode\n");
#endif
#ifdef HDLC_FR
    printf("FR mode\n");
#endif
#ifdef HDLC_TRANS
    printf("HDLC mode\n");
#endif
#ifdef TRANS
    printf("TRANS mode\n");
#endif

    printf("Loopback: STM-1: ");
#ifdef STM1_LOOPBACK
    printf("LOOP, ETH:  ");
#else
    printf("NONE, ETH:  ");
#endif
#ifdef ETH_LOOPBACK
    printf("LOOP\n");
#else
    printf("NONE\n");
#endif

    if(eth_port > 2) {
	printf("Invalid ETH port, must be 0, 1 or 2\n");
	return -1;
    }

    if(atm_port > 1) {
	printf("Invalid ATM port, must be 0 or 1\n");
	return -1;
    }

    print_winmon_env();

    eth_tx_no = eth_rx_no = 0;
    atm_tx_no = atm_rx_no = 0;

#ifdef __linux__
    sem_init(&sema, 0, 0);
    sem_init(&mbox_sema, 0, 0);
#endif

    WP_ControlRegister(WP_DEBUG_CALLBACK_FILE_LINE_ERROR, fileLineError);
#ifdef USE_WDDI_LOG
    WP_ControlRegister(WP_DEBUG_CALLBACK_WDDI_LOG, wddiLog);
#endif

    // Init driver
    status = WP_DriverInit();
    terminate_on_error(status, "WP_DriverInit()");

    // Select port 3 type
    if(eth_port == 0) {
	status = WPX_PortModeSet(0, WPX_PORT_MODE_ETH, 3);
	terminate_on_error(status, "WPX_PortModeSet ETH");
    } else {
	status = WPX_PortModeSet(0, WPX_PORT_MODE_STM1, 3);
	terminate_on_error(status, "WPX_PortModeSet STM1");
    }

    status = WPX_BoardConfigure(0, WPX_CONFIGURE_XS_3200);
    terminate_on_error(status, "WPX_BoardConfigure(0, WPX_CONFIGURE_XS_3200)");

#ifdef ETH_LOOPBACK
    serdes_mode = WPX_SERDES_INTERNAL_CLOCK | WPX_SERDES_LOOPBACK;
#else
    serdes_mode = WPX_SERDES_INTERNAL_CLOCK;
#endif

    status = WPX_BoardSerdesInit(0, WP_PORT_ENET3, serdes_mode);
    terminate_on_error(status, "WPX_BoardSerdesInit(WP_PORT_ENET3)");      
    status = WPX_BoardSerdesInit(0, WP_PORT_ENET5, serdes_mode);
    terminate_on_error(status, "WPX_BoardSerdesInit(WP_PORT_ENET5)");      
    status = WPX_BoardSerdesInit(0, WP_PORT_ENET4, serdes_mode);
    terminate_on_error(status, "WPX_BoardSerdesInit(WP_PORT_ENET4)");      

    // Init System
    status = WP_SysInit(WP_WINPATH(0), context);
    terminate_on_error(status, "WP_SysInit()");

    /* Display memory statistics */
    status = WP_MemoryAvailable(WP_WINPATH(0), WP_BUS_PACKET, 0, &bytes);
    printf("UPI GENET App:  Packet bus bytes left = %d\n", bytes);
  
    status = WP_MemoryAvailable(WP_WINPATH(0), WP_BUS_PARAM, 0, &bytes);
    printf("UPI GENET App:  Param bus bytes left = %d\n", bytes);
  
    status = WP_MemoryAvailable(WP_WINPATH(0), WP_BUS_INTERNAL, 0, &bytes);
    printf("UPI GENET App:  Internal bus bytes left = %d\n", bytes);

    status = WP_MemoryAvailable(WP_WINPATH(0), WP_BUS_HOST, 0, &bytes);
    printf("UPI GENET App:  Host bus bytes left = %d\n", bytes);

    // Load UFE4 firmware
    printf("Loading UFE4 firmware ... \n"); fflush(stdout);

    memset(&config, 0, sizeof(WUFE_init_config));
    config.ufe_id = 0;
    
    WPX_Ufe4HwReset(0, 0);
    
    status = WUFE_UfeInitializeFirmware(&config);
    wufe_terminate_on_error(status, "WUFE_UfeInitializeFirmware()");  

    status = WPX_FpgaFirmwareInitializationStatus(0);
    terminate_on_error(status, "WPX_FpgaFirmwareInitializationStatus");

    // Create Scheduler
    status = WP_SysSchedulerCreate(WP_WINPATH(0), calendar);
    terminate_on_error(status, "WP_SysSchedulerCreate");

    // ATM buffers
    buffer_pool = WP_PoolCreate(WP_WINPATH(0), WP_pool_buffer, buffer_data);
    terminate_on_error(buffer_pool, "WP_PoolCreate() buffer_pool");

    // Create the ATM ring pool
    pool_ring = WP_PoolCreate(WP_WINPATH(0), WP_pool_ring, ring_data);
    terminate_on_error(pool_ring, "WP_PoolCreate() pool_ring");

    // Attach the ATM buffer and ring pools to the qnode
    qn_host->pool_buffer = buffer_pool;
    qn_host->pool_ring   = pool_ring;

    // Create the Host termination queue node
    qnhost = WP_QNodeCreate(0,WP_QNODE_HOSTQ, qn_host);
    terminate_on_error(qnhost, "WP_QNodeCreate() QNODE_HOSTQ");

    // Ethernet buffers
    iwbuffer_pool = WP_PoolCreate(WP_WINPATH(0), WP_pool_iwbuffer, buffer_data);
    terminate_on_error(iwbuffer_pool, "WP_PoolCreate() iwbuffer_pool");

    qn_iw->adjunct_pool = iwbuffer_pool;
    qniw = WP_QNodeCreate(0,WP_QNODE_IWQ | WP_QNODE_OPT_FMU,qn_iw);
    terminate_on_error(qniw, "WP_QNodeCreate() qniw");

    qniw_enet = WP_QNodeCreate(0, WP_QNODE_IWQ | WP_QNODE_OPT_FMU | WP_QNODE_OPT_DEDICATED_RX_HWQ, qn_iw);
    terminate_on_error(qniw_enet, "WP_QNodeCreate() qniw_enet");

#ifdef HDLC_FR
    // Create FR Qnode
    qn_fr.adjunct_pool   = iwbuffer_pool;
    qn_fr.num_buffers    = 64;
    qn_fr.interruptqueue = WP_IW_IRQT3;
    
    qnfrtx               = WP_QNodeCreate(0, WP_QNODE_FR_HOSTQ, &qn_fr);
    terminate_on_error(qnfrtx, "WP_QNodeCreate qnfrtx");
#endif

    /* Create all GEnet Ports */
    gbep[0] = WP_PortCreate(WP_WINPATH(0), WP_PORT_ENET3, gbe_s); // front port 3
    terminate_on_error(gbep[0], "WP_PortCreate() ENET3");
    
    gbep[1] = WP_PortCreate(WP_WINPATH(0), WP_PORT_ENET5, gbe_s); // front port 2
    terminate_on_error(gbep[1], "WP_PortCreate() ENET5");
    
    gbep[2] = WP_PortCreate(WP_WINPATH(0), WP_PORT_ENET4, gbe_s);  // front port 1
    terminate_on_error(gbep[2], "WP_PortCreate() ENET4");
    
    num_eth_ports = 3;

    /* Create all GEnet Devices */
    for(i=0; i<num_eth_ports; i++) {
	enet_filters.rx_ucfilter    = WP_ENET_FILTER_PROCESS;
	enet_filters.rx_bcfilter    = WP_ENET_FILTER_PROCESS;
	enet_filters.rx_mcfilter    = WP_ENET_FILTER_PROCESS;
	
	enet_ext.duplexmode         = WP_ENET_FULLDUPLEX;
	enet_ext.rx_maclookup_mode  = WP_DISABLE;
	enet_ext.rx_flowcontrol     = WP_ENET_FLOW_ENABLE;
	enet_ext.tx_flowcontrol     = WP_ENET_FLOW_ENABLE;
	enet_ext.rx_addrmode        = WP_ENET_ADDR_ACCEPTALL;
	enet_ext.phystatmode        = WP_ENET_STAT_ENABLE;
	enet_ext.tx_statmode        = WP_ENET_STAT_ENABLE;
	enet_ext.rx_statmode        = WP_ENET_STAT_ENABLE;
	enet_ext.tx_duplicate       = WP_DISABLE;
	enet_ext.rx_filters         = &enet_filters;

	gbe_dev[0].extended_params = NULL;

	gbedev[i] = WP_DeviceCreate(gbep[i], WP_PHY(0), WP_DEVICE_ENET, gbe_dev);
	terminate_on_error(gbedev[0], "WP_DeviceCreate() ENET");

	gbe_dev[0].extended_params = &enet_ext;

	status = WP_DeviceModify(gbedev[i], 
				 WP_DEV_MOD_ENET_PHYSTATMODE | 
				 WP_DEV_MOD_ENET_TX_STATMODE | 
				 WP_DEV_MOD_ENET_RX_STATMODE,
				 gbe_dev);
	terminate_on_error(status, "WP_DeviceModify() ENET");
    }

    // Create the EMPHY port
    memset(&pkt_limits, 0, sizeof(WP_port_pkt_limits));
    
    pkt_limits.max_tx_channels = 4;
    pkt_limits.max_rx_channels = 4;
    pkt_limits.pq_block_size   = 0;
    pkt_limits.n_pq_blocks     = 0;
    pkt_limits.emphy_devices   = 32;
    
    upi_packet_ext.packet_characteristics = WP_UPI_POS_CRC_DISABLE;
    upi_packet_ext.slow_phy_tx_bitrate    = 50000000;
    upi_packet_ext.pkt_limits             = &pkt_limits;
    
    atm_limits.max_tx_channels     = 4;
    atm_limits.max_rx_channels     = 6;
    atm_limits.pq_block_size       = 0;
    atm_limits.n_pq_blocks         = 0;
    atm_limits.aal1_tx_channels    = 0;
    atm_limits.aal1_rx_channels    = 0;
    atm_limits.n_mq_blocks         = 0;
    atm_limits.n_mq_pq_blocks      = 0;
    atm_limits.emphy_devices       = 32;
    atm_limits.max_psu_tx_channels = 0;
    atm_limits.n_shaped_mq         = 0;
    atm_limits.max_aal0_members    = 0;

    upi_atm_ext.atm_characteristics = WP_UPI_IDLEDISC_OFF | WP_ATM_EXT_PHY_STATS_ENABLE;
    upi_atm_ext.utopia_clk          = 62500000;
    upi_atm_ext.atm_limits          = &atm_limits;
    upi_atm_ext.tx_atm_exthdrsz     = 0;
    upi_atm_ext.rx_atm_exthdrsz     = 0;
    upi_atm_ext.rx_atm_lookupmode   = WP_ATM_LOOKUP_HASH;

    emphy_iw_config[0].ces_tdm_wpid          = 0;
    
    memset(&upi_emphy_port_config, 0, sizeof(WP_port_upi));

    emphy_config.tx_maxsdu = 8192;
#ifdef ATM
    emphy_config.protocols           = WP_EMPHY_ATM;
#endif
#ifdef HDLC
    emphy_config.protocols           = WP_EMPHY_FR | WP_EMPHY_HDLC_PPP;
#endif
#ifdef TRANS
    emphy_config.protocols           = WP_EMPHY_TRANS_PWE3;
#endif
    emphy_config.trans_emphy_devices = 1008;
    emphy_config.max_ds1             = 336;

    upi_emphy_port_config.upimode            = WP_UPI_UFE4;
    upi_emphy_port_config.loopback           = 0;
    upi_emphy_port_config.rx_characteristics = (WP_UPI_SINGLE     |
						WP_UPI_MASTER     |    
						WP_UPI_PRT_EVEN   |
						WP_UPI_FIXED      |
						WP_UPI_PRTCHK_OFF |
						WP_UPI_BUS_WIDTH_16);
    upi_emphy_port_config.tx_characteristics = (WP_UPI_SINGLE     |
						WP_UPI_MASTER     |   
						WP_UPI_PRT_EVEN   |
						WP_UPI_FIXED      |
						WP_UPI_PRTCHK_ON  |
						WP_UPI_EMPHY2_13  |
						WP_UPI_BUS_WIDTH_16);
    upi_emphy_port_config.atm_ext            = &upi_atm_ext;
    upi_emphy_port_config.packet_ext         = &upi_packet_ext;
    upi_emphy_port_config.iw_ext             = emphy_iw_config;
    upi_emphy_port_config.emphy_ext          = &emphy_config;

    emphy_port = WP_PortCreate(0, WP_PORT_UPI1, &upi_emphy_port_config);
    terminate_on_error(emphy_port, "WP_PortCreate UPI1 EMPHY");

#ifdef TRANS 

    memset(&cesop_feature_cfg, 0, sizeof(WP_feature_iw_cesop));

    cesop_feature_cfg.port_interface    = WP_IW_CESOP_PORT_UFE;

    status = WP_FeatureInit(WP_SYSHANDLE(0), WP_FEATURE_IW_CESOP, &cesop_feature_cfg);
    terminate_on_error(status, "WP_FeatureInit");
#endif

    /* Create an interworking port */
    iwp =  WP_PortCreate(WP_WINPATH(0), WP_PORT_IW_HOST, NULL);
    terminate_on_error(iwp,"Host Port Create");

    /* Create an interworking device */
    iwdev = WP_DeviceCreate(iwp, 0, WP_DEVICE_IW_HOST, NULL);
    terminate_on_error(iwdev,"Host Device Create");

    /* Register some callback routines */
    status = WP_ControlRegister(WP_EVENT_TX_INDICATE, &My_AppIndicateEventTx);
    terminate_on_error(status,"WP_ControlRegister() TxIndicateEvent");

    status = WP_ControlRegister(WP_EVENT_RX_INDICATE, &My_AppIndicateEventRx);
    terminate_on_error(status,"WP_ControlRegister() RxIndicateEvent");

    status = WP_ControlRegister(WP_EVENT_STATUS_INDICATE, &My_AppIndicateErrors);
    terminate_on_error(status,"WP_ControlRegister() RxIndicateErrors");

    /* Create system */
    status = WP_SysCommit();
    terminate_on_error(status, "WP_SysCommit()");

    /* Display memory statistics */
    status = WP_MemoryAvailable(WP_WINPATH(0), WP_BUS_PACKET, 0, &bytes);
    printf("UPI GENET App:  Packet bus bytes left = %d\n", bytes);
  
    status = WP_MemoryAvailable(WP_WINPATH(0), WP_BUS_PARAM, 0, &bytes);
    printf("UPI GENET App:  Param bus bytes left = %d\n", bytes);
  
    status = WP_MemoryAvailable(WP_WINPATH(0), WP_BUS_INTERNAL, 0, &bytes);
    printf("UPI GENET App:  Internal bus bytes left = %d\n", bytes);

    status = WP_MemoryAvailable(WP_WINPATH(0), WP_BUS_HOST, 0, &bytes);
    printf("UPI GENET App:  Host bus bytes left = %d\n", bytes);

    status = WP_PortEnable(emphy_port, WP_DIRECTION_DUPLEX);
    terminate_on_error(status, "WP_PortEnable() emphy");
    
    status = WP_SysPsuEnable(0);
    terminate_on_error(status, "WP_SysPsuEnable()");
    
    // Init UFE4
    status = WUFE_UfeInit(&config);
    wufe_terminate_on_error(status,"WUFE_UfeInit");
    
    memset(&sys_cfg, 0, sizeof(WUFE_system));
    
    sys_cfg.line_interface                          = WUFE_INTERFACE_SDH;
    sys_cfg.max_line_number                         = 336;
    sys_cfg.max_phy_number                          = 1008;
    sys_cfg.idle_pattern.tx_idle_byte_data_pattern  = 0x81;
    sys_cfg.idle_pattern.tx_idle_nibble_cas_pattern = 0x4;
    sys_cfg.emphy.rx_parity_mode                    = 0;
    sys_cfg.emphy.tx_parity_mode                    = 0;
    sys_cfg.emphy.tx_parity_check                   = 0;
    sys_cfg.tdm_parity.rx_parity_mode               = 0;
    sys_cfg.tdm_parity.tx_parity_mode               = 0;
    sys_cfg.fifo_manager.loopback                   = WUFE_FM_LOOPBACK_DISABLE;
    sys_cfg.max_buff_num                            = WUFE_MAX_BUF_SIZE_64;
    sys_cfg.system_clock_rec_enable                 = WUFE_SYSTEM_CLOCK_REC_DISABLE;
    sys_cfg.clk_rec_system_params                   = NULL;
    
    status = WUFE_SystemConfig(0, &sys_cfg);
    wufe_terminate_on_error(status, "WUFE_SystemConfig");

#ifdef __linux__
    WPL_IntConnect(WP_WINPATH(0), WPL_Eint3Ufe4, 0, WT_Eint3Interrupt);
    WPL_IntEnable (WP_WINPATH(0), WPL_Eint3Ufe4);
#endif


    status = WUFE_SystemEnable(0, WUFE_SYS_EMPHY, WUFE_FULL_DUPLEX);
    wufe_terminate_on_error(status, "WUFE_SystemEnable");

    // New
    status = WUFE_UfeFramerHwInterruptEnable(0);
    wufe_terminate_on_error(status, "WUFE_UfeFramerHwInterruptEnable");
    
    status = WUFE_UfeCoreHwInterruptEnable(0);
    wufe_terminate_on_error(status, "WUFE_UfeCoreHwInterruptEnable");
    
    status = WUFE_SystemInterruptEnable(0);
    wufe_terminate_on_error(status, "WUFE_SystemInterruptEnable ");
    
    status = WUFE_UfeCoreHwInterruptDisable(0);
    wufe_terminate_on_error(status, "WUFE_UfeCoreHwInterruptDisable");

    // stucks the application
    //status = WUFE_UfeSfpHwInterruptEnable(0);
    //wufe_terminate_on_error(status,"WUFE_UfeSfpHwInterruptEnable");

    rx_host_term = WP_ChannelCreate(0x01,
				    iwdev, 
				    qniw, 
				    WP_CH_RX,
				    WP_IW_HOST, 
				    rx_ch);
    terminate_on_error(rx_host_term, "channel create iw rx host");

    /* create Enet Tx channel */
    for(i=0; i<num_eth_ports; i++) {
	tx_enet_handle[i] = WP_ChannelCreate(0x10+i, gbedev[i], qniw_enet, WP_CH_TX, WP_ENET, &ch_config_gbe[0]);
	terminate_on_error(tx_enet_handle[i], "WP_ChannelCreate() Enet TX");
    }
    
    /* create Enet Rx channel */
    for(i=0; i<num_eth_ports; i++) {
	rx_enet_handle[i] = WP_ChannelCreate(TAG_ETH0_L2+i, gbedev[i], qniw_enet, WP_CH_RX, WP_ENET, &ch_config_gbe[0]);
	terminate_on_error(rx_enet_handle[i], "WP_ChannelCreate() Enet RX");
    }

    iw_sys = WP_IwSystemCreate(WP_WINPATH(0), WP_IW_ROUTING_MODE, routing_sys);
    terminate_on_error(iw_sys, "SystemCreate");

    for(i=0; i<num_eth_ports; i++) {
	iw_agg_routing[0].tag    = TAG_ETH0 + i;
	iw_agg_routing[0].txfunc = rx_host_term;
	iw_agg_g[i] = WP_IwFlowAggregationCreate(WP_WINPATH(0),
					      WP_IW_ROUTING_MODE,
					      iw_agg_routing);
	terminate_on_error(iw_agg_g[i],"Aggregation Create");
    }

    for(i=0; i<num_eth_ports; i++) {
	status = WP_IwTxBindingCreate(tx_enet_handle[i], WP_IW_TX_BINDING, &tx_qos_iw[0]);
	terminate_on_error(status, "WP_IwTxBindingCreate");
    }

    for(i=0; i<num_eth_ports; i++) {
	route_port[0].tag = 0x60+i;
	route_port[0].flow_agg = iw_agg_g[i];
	iw_port_enet[i] = WP_IwPortCreate(iw_sys, route_port);
	terminate_on_error(iw_port_enet[i], "WP_IwPortCreate - Enet");
    }

    for(i=0; i<num_eth_ports; i++) {
	rx_binding[0].input_port = iw_port_enet[i];
	status = WP_IwRxBindingCreate(rx_enet_handle[i], iw_sys, qniw_enet, &rx_binding);
	terminate_on_error(status, "RxBindingCreate");
    }

    status = WP_IwSystemBuild(iw_sys);
    terminate_on_error(status,"WP_IwSystemBuild");

   /* Enable Ethernet Ports, Devices and Channels*/
    for(i=0; i<num_eth_ports; i++) {
	status = WP_PortEnable(gbep[i], WP_DIRECTION_DUPLEX);
	terminate_on_error(status, "WP_PortEnable gbep");

	status = WP_DeviceEnable(gbedev[i], WP_DIRECTION_DUPLEX);
	terminate_on_error(status, "WP_DeviceEnable - gbedev");
    }

    status = WPX_UFE_FRAMER_FlexmuxInit(WP_PORT_UPI1, 0);
    flexmux_terminate_on_error(status, "WPX_UFE_FRAMER_FlexmuxInit");

    WUFE_SystemDisplay(0);

    /* service the mailboxes in a separate process */
    if(1) {
	thread_params[0] = 0;
	thread_params[1] = 0;
	
	WPL_ThreadInit(&mailbox_thread_id,
		       (void *(*)(void *)) WPX_UFE_FRAMER_FlexmuxServiceMailbox,
		       (void *) thread_params);
    } else {
	pthread_create( &thread_id_mbox, NULL, &mailbox_handler, NULL);
    }

    printf("\n\nBuilding Framer Personality, please wait ...\n\n");
    status = WPX_UFE_FRAMER_FlexmuxSetBuildPersonality(0,
						       WPX_UFE_FRAMER_BUILD_PERSONALITY_4_PLUS_4_OC3,
						       0);
    flexmux_terminate_on_error(status, "WPX_UFE_FRAMER_FlexmuxSetBuildPersonality");

#ifdef LINE_PROTECTION
    WPX_FRMR_SetDevProtectMode( 0, 0, WPX_UFE_DEVICE_PROTECTION_MODE_LINE);
#else
    WPX_FRMR_SetDevProtectMode( 0, 0, WPX_UFE_DEVICE_PROTECTION_MODE_CARD);
    WPX_FRMR_SetDevCardProtectOperatingMode(0, 0, WPX_UFE_DEVICE_CARD_PROTECTION_OPERATING_MODE_MASTER);
#endif

    status = WPX_FRMR_DEV_SetMode(0, 0, WPX_UFE_FRAMER_DEVICE_MODE_SDH);
    flexmux_terminate_on_error(status, "WPX_FRMR_DEV_SetMode");

    for(i=0; i<4; i++) {
	status = WPX_FRMR_SONET_SDH_PORT_SetRate(0, 0, i, WPX_UFE_FRAMER_LINE_PORT_RATE_STM1);
	flexmux_terminate_on_error(status, "WPX_FRMR_SONET_SDH_PORT_SetRate");

	line_end_point.TransferType        = WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC4;
	line_end_point.u.SDH.stm4          = 0;
	line_end_point.u.SDH.stm1          = i;
	line_end_point.u.SDH.stm0          = 0;
	line_end_point.u.SDH.tug2          = 0;
	line_end_point.u.SDH.tu            = 0;

	status = WPX_FRMR_SONET_SDH_CreateFacility(0, 0, &line_end_point);
	flexmux_terminate_on_error(status, "WPX_FRMR_SONET_SDH_CreateFacility");

	status = WPX_FRMR_SONET_SDH_HO_PATH_C2_SetTX(0, 0, &line_end_point, 0x13);
	flexmux_terminate_on_error(status, "WPX_FRMR_SONET_SDH_HO_PATH_C2_SetTX");
	
	status = WPX_FRMR_SONET_SDH_HO_PATH_C2_SetEX(0, 0, &line_end_point, 0x13);
	flexmux_terminate_on_error(status, "WPX_FRMR_SONET_SDH_HO_PATH_C2_SetEX");
    }

    status = WPX_FRMR_DEV_DIAG_DRV_RecClkSet(0, 0, 0 ,0);
    flexmux_terminate_on_error(status, "WPX_FRMR_DEV_DIAG_DRV_RecClkSet");

#ifdef __linux__
    WPL_IntConnect(WP_WINPATH(0), WPL_SgmiiAn, 0, WT_SgmiiAnInterrupt);
    WPL_IntEnable (WP_WINPATH(0), WPL_SgmiiAn);

    WPL_IntConnect(WP_WINPATH(0), WPL_SerdesInterrupt, 0, WT_SerdesInterrupt);
    WPL_IntEnable (WP_WINPATH(0), WPL_SerdesInterrupt);

    // Enable connected SerDes interrupts
    *(volatile unsigned int *)(unsigned long)(0x1e0082f0 + WPL_RIF_VIRTUAL(0, 0)) = 0;    
    *(volatile unsigned int *)(unsigned long)(0x1e0082f4 + WPL_RIF_VIRTUAL(0, 0)) = htonl(0x80000777);

#endif

#ifdef TRANS
    trans_system.max_flows  = 0;
    trans_system.buffer_gap = 0x40;
    
    iw_sys_trans = WP_IwSystemCreate(WP_WINPATH(0), WP_IW_CESOP_MODE, &trans_system);
    terminate_on_error(iw_sys_trans, "WP_IwSystemCreate() Trans");
#endif

    for(i=0; i<4; i++) {
	line_config.struct_id             = WUFE_STRUCT_LINE_SDH;
	line_config.stm4                  = 0;
	line_config.stm1                  = i;
	line_config.stm0                  = 0;
	line_config.tug2                  = 0;
	line_config.tu                    = 0;
	line_config.transfer_type         = WUFE_SDH_TYPE_E1;
#ifdef FRAMED
	line_config.framed                = WUFE_FRAMED;
#else
	line_config.framed                = WUFE_UNFRAMED;
#endif
	line_config.cas_enable            = WUFE_CAS_DISABLE;
	line_config.clock_rec_line_params = NULL;
	
	status = WUFE_LineCreate(&line_handle[i], 0, WUFE_STRUCT_LINE_SDH, &line_config);
	wufe_terminate_on_error(status, "WUFE_LineCreate");

	status = WUFE_LineEnable(line_handle[i], WUFE_FULL_DUPLEX);
	wufe_terminate_on_error(status, "WUFE_LineEnable");

	client_port_id = i * 84;

	status = WPX_FRMR_SOCKET_PDH_CreateFacility(0, 0, client_port_id, WPX_UFE_FRAMER_CLIENT_E1);
	flexmux_terminate_on_error(status, "WPX_FRMR_SOCKET_PDH_CreateFacility");
	
#ifdef FRAMED
	status = WPX_FRMR_SOCKET_PDH_SetFraming(0, 0, client_port_id, WPX_UFE_FRAMER_CLIENT_PCM31CRC);
#else
	status = WPX_FRMR_SOCKET_PDH_SetFraming(0, 0, client_port_id, WPX_UFE_FRAMER_CLIENT_UNFRAMED);
#endif
	flexmux_terminate_on_error(status, "WPX_FRMR_SOCKET_PDH_SetFraming");
	
	status = WPX_FRMR_SOCKET_PDH_SetMapping(0, 0, client_port_id, WPX_UFE_FRAMER_CLIENT_MAPPING_BIT_ASYNC);
	flexmux_terminate_on_error(status, "WPX_FRMR_SOCKET_PDH_SetMapping");
	
	/* create the line port VC11\VC12 facility */
	line_end_point.TransferType = WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC12;
	line_end_point.u.SDH.stm4          = 0;
	line_end_point.u.SDH.stm1          = i;
	line_end_point.u.SDH.stm0          = 0;
	line_end_point.u.SDH.tug2          = 0;
	line_end_point.u.SDH.tu            = 0;

	status = WPX_FRMR_SONET_SDH_CreateFacility(0, 0, &line_end_point);
	flexmux_terminate_on_error(status, "WPX_FRMR_SONET_SDH_CreateFacility (VC11 or VC12)");
	
	/* set the transmitted and expected V5 values */
	status = WPX_FRMR_SONET_SDH_LO_PATH_V5_SigLabel_SetTX(0, 0, &line_end_point, 0x02);
	flexmux_terminate_on_error(status, "WPX_FRMR_SONET_SDH_LO_PATH_V5_SigLabel_SetTX");
	
	status = WPX_FRMR_SONET_SDH_LO_PATH_V5_SigLabel_SetEX(0, 0, &line_end_point, 0x02);
	flexmux_terminate_on_error(status, "WPX_FRMR_SONET_SDH_LO_PATH_V5_SigLabel_SetEX");
	
	/* create the connection between the client port and the line port */
	status = WPX_FRMR_CONNECTIONS_AddDropLineToSocketCreate(0, 0, &line_end_point, client_port_id);
	flexmux_terminate_on_error(status, "WPX_FRMR_CONNECTIONS_AddDropLineToSocketCreate");
	
	/* Set the PDH Tx line timing  */
	status = WPX_FRMR_SOCKET_PDH_SetClkRec(0, 0, client_port_id, WPX_UFE_FRAMER_TRANSMIT_TIMING_WINPATH);
	flexmux_terminate_on_error(status, "WPX_FRMR_SOCKET_PDH_SetClkRec");

#ifdef STM1_LOOPBACK
	status = WUFE_LineLoopbackConfigure(line_handle[i], WUFE_LINE_LOOPBACK_TYPE_SYSTEM);
	wufe_terminate_on_error(status, "WUFE_LineLoopbackConfigure SYSTEM");
#else
	status = WUFE_LineLoopbackConfigure(line_handle[i],WUFE_LINE_LOOPBACK_TYPE_NONE );
	wufe_terminate_on_error(status, "WUFE_LineLoopbackConfigure NONE");
#endif

	// Tricky !!!
	// UFE port 0 goes to front panel port 0. This is line i=0.
	// UFE port 1 which is the protection of UFE port 0 goes to front panel port 1 (unavailable as STM-1 on Ericsson XS-3200)
	// UFE port 2 goes to front panel port 2. This is line i=1
	// UFE port 3 which is the protection of UFE port 2 goes to front panel port 3 (available as STM-1 on Ericsson XS-3200)
	// UFE ports 4-7 are not connected on XS-3200
	//
	// So if we want STM-1 on front panel port 3, we need to use the protection of UFE port 2, therefore the following call...
	//
	// This mapping may be modified in WDDI flexmux software, but I am not sure.
	//
#ifdef LINE_PROTECTION
	if(i==1) {
	    WPX_FRMR_SONET_SDH_PORT_ForceB(0, 0, 1);
	    flexmux_terminate_on_error(status, "WPX_FRMR_SONET_SDH_PORT_ForceB");
	}
#endif
	memset(&phy_cfg, 0, sizeof(WUFE_phy));

	// --------------------------------------------------------------------------
	//
	//                        PHYs
	//
	// --------------------------------------------------------------------------

#ifdef ATM
	// ATM PHY
	phy_cfg.struct_id                            = WUFE_STRUCT_PHY_ATM;
	phy_cfg.rx_fifo_param.fifo_max_size          = 4;
	phy_cfg.rx_fifo_param.priority_table         = 0;
	phy_cfg.rx_fifo_param.transmit_th            = 0;
	phy_cfg.rx_slot_group.hmvip_id               = 0;
	for(j=0; j<32; j++) {
	    phy_cfg.rx_slot_group.slot_arr[j] = ufe_e1_slots[j]; 
	}
#ifdef FRAMED
	phy_cfg.rx_slot_group.n_slots                = 30;
#else
	phy_cfg.rx_slot_group.n_slots                = 32;
#endif
	phy_cfg.tx_fifo_param.fifo_max_size          = 2;
	phy_cfg.tx_fifo_param.priority_table         = 0;
	phy_cfg.tx_fifo_param.fast_mode              = WUFE_PHY_FIFO_MODE_SLOW;
	phy_cfg.tx_fifo_param.transmit_th            = 0;
	phy_cfg.tx_fifo_param.wait_type              = WUFE_PHY_TX_WAIT_TYPE_D; // 53 bytes wait
	phy_cfg.tx_slot_group.hmvip_id               = 0;
	for(j=0; j<32; j++) {
	    phy_cfg.tx_slot_group.slot_arr[j] = ufe_e1_slots[j]; 
	}
#ifdef FRAMED
	phy_cfg.tx_slot_group.n_slots                = 30;
#else
	phy_cfg.tx_slot_group.n_slots                = 32;
#endif

	phy_cfg.type.atm.rx_descrambler_mode         = WUFE_PHY_ATM_RX_DESCRAMBLER_ENABLE; // EN
	phy_cfg.type.atm.rx_coset_mode               = WUFE_PHY_ATM_RX_ADD_COSET;
	phy_cfg.type.atm.rx_idle_cell_discard        = WUFE_PHY_ATM_RX_IDLE_CELL_DISCARD;
	phy_cfg.type.atm.rx_unassigned_cell_discard  = WUFE_PHY_ATM_RX_UNASSIGNED_CELL_DISCARD;
	phy_cfg.type.atm.rx_hec_error_discard        = WUFE_PHY_ATM_RX_HEC_ERR_DISCARD;
	phy_cfg.type.atm.rx_hec_correction_mode      = WUFE_PHY_ATM_RX_HEC_CORECTION_ENABLE;
	phy_cfg.type.atm.rx_cell_delineation         = WUFE_PHY_ATM_RX_CELL_DELINEATION_NIBBLE_RESOLUTION;
	phy_cfg.type.atm.rx_alpha                    = 5;
	phy_cfg.type.atm.rx_delta                    = 3;
	phy_cfg.type.atm.tx_scrambler_mode           = WUFE_PHY_ATM_TX_SCRAMBLER_ENABLE; // EN
	phy_cfg.type.atm.tx_coset_mode               = WUFE_PHY_ATM_TX_ADD_COSET;
	phy_cfg.type.atm.tx_idle_unassigned_cell_mode= WUFE_PHY_ATM_TX_IDLE_CELL_MODE;
#endif


#ifdef HDLC
	// HDLC PHY
	phy_cfg.struct_id                            = WUFE_STRUCT_PHY_HDLC;
	phy_cfg.rx_fifo_param.fifo_max_size          = 4;
	phy_cfg.rx_fifo_param.priority_table         = 0;
	phy_cfg.rx_fifo_param.transmit_th            = 0;
	phy_cfg.rx_slot_group.hmvip_id               = 0;
	for(j=0; j<32; j++) {
	    phy_cfg.rx_slot_group.slot_arr[j] = ufe_e1_slots[j]; 
	}
#ifdef FRAMED
	phy_cfg.rx_slot_group.n_slots                = 30;
#else
	phy_cfg.rx_slot_group.n_slots                = 32;
#endif
	phy_cfg.tx_fifo_param.fifo_max_size          = 10;
	phy_cfg.tx_fifo_param.priority_table         = 0;
	phy_cfg.tx_fifo_param.fast_mode              = WUFE_PHY_FIFO_MODE_SLOW;
	phy_cfg.tx_fifo_param.transmit_th            = 0;
	phy_cfg.tx_fifo_param.wait_type              = WUFE_PHY_TX_WAIT_TYPE_A;
	phy_cfg.tx_slot_group.hmvip_id               = 0;
	for(j=0; j<32; j++) {
	    phy_cfg.tx_slot_group.slot_arr[j] = ufe_e1_slots[j]; 
	}
#ifdef FRAMED
	phy_cfg.tx_slot_group.n_slots                = 30;
#else
	phy_cfg.tx_slot_group.n_slots                = 32;
#endif
	phy_cfg.type.hdlc.rx_frame_include_crc_mode  = WUFE_PHY_HDLC_CRC_ENABLE;
	phy_cfg.type.hdlc.rx_frame_crc_size          = WUFE_PHY_HDLC_CRC_32;
	phy_cfg.type.hdlc.tx_frame_include_crc_mode  = WUFE_PHY_HDLC_CRC_ENABLE;
	phy_cfg.type.hdlc.tx_frame_crc_size          = WUFE_PHY_HDLC_CRC_32;
	phy_cfg.type.hdlc.tx_no_flag_between_frames  = WUFE_PHY_HDLC_TX_IDLE;
	phy_cfg.type.hdlc.tx_min_flag_num            = 1;
	phy_cfg.type.hdlc.tx_underrun_interrupt_mode = WUFE_UFE_PHY_INT_ENABLE;
#endif

#ifdef TRANS
	// Transparent PHY
	phy_cfg.struct_id                    = WUFE_STRUCT_PHY_TRANS;
	phy_cfg.tx_fifo_param.transmit_th    = 0x1;
	phy_cfg.tx_fifo_param.fast_mode      = WUFE_PHY_FIFO_MODE_SLOW;
	phy_cfg.tx_fifo_param.wait_type      = WUFE_PHY_TX_WAIT_TYPE_A;
	phy_cfg.rx_slot_group.n_slots        = 32;
	phy_cfg.tx_slot_group.n_slots        = 32;
	phy_cfg.type.trans.rx_data_unit_size = 64;
#endif

	status = WUFE_PhyCreate(&phy_handle[i],
				line_handle[i],
				&phy_cfg,
				WUFE_FULL_DUPLEX);
	wufe_terminate_on_error(status, "WUFE_PhyCreate");

	//WUFE_PhyDisplay(phy_handle[i]);

	// --------------------------------------------------------------------------
	//
	//                        Devices
	//
	// --------------------------------------------------------------------------

	// Create Device
	ufe_phy_index = WUFE_PHY_INDEX_GET(phy_handle[i]);

#ifdef ATM
	// ATM Device
	upi_emphy_atm_device.tx_bit_rate                   = 64000;
	upi_emphy_atm_device.tx_slave_addr                 = 0;
	upi_emphy_atm_device.tx_shaping_mode               = WP_ATM_DEV_SHAPING_VC;
	upi_emphy_atm_device.tx_shaping_groups_max         = 0;
	upi_emphy_atm_device.tx_hierarchical_shaping_table = 0;
	upi_emphy_atm_device.rx_bit_rate                   = 64000;
	upi_emphy_atm_device.rx_vp_vc_block                = 0;
	upi_emphy_atm_device.rx_slave_addr                 = 0;
	upi_emphy_atm_device.rx_ras_timeout                = 0;
	upi_emphy_atm_device.uni_nni_mode                  = WP_EMPHY_ATM_UNI_MODE;
	upi_emphy_atm_device.idle_format                   = WP_UNUSED;
	upi_emphy_atm_device.line_id                       = WP_NO_LINE_ID;
	
	tdm_device[i] = WP_DeviceCreate(emphy_port,
					WP_PHY(ufe_phy_index),
					WP_DEVICE_ATM,
					&upi_emphy_atm_device);
	terminate_on_error(tdm_device[i], "WP_DeviceCreate ATM");
#endif

#ifdef TRANS
	// Transparent device
	upi_trans_device.mode             = (WP_EMPHY_TRANS_UNFRAMED | WP_EMPHY_TRANS_E1);
	upi_trans_device.structure_size   = WP_TDM_STRUCTURE_BASIC;
	upi_trans_device.tx_dataunit_size = 32;
	upi_trans_device.rx_dataunit_size = 32;
	upi_trans_device.ds1_id           = 0;
	
	tdm_device[i] = WP_DeviceCreate(emphy_port,
					WP_PHY(ufe_phy_index),
					WP_DEVICE_TRANS,
					&upi_trans_device);
	terminate_on_error(tdm_device[i], "WP_DeviceCreate TRANS");
#endif

#ifdef HDLC_FR
	// FR Device
	upi_emphy_fr_device.tx_bit_rate       = 64000;
	upi_emphy_fr_device.tx_slave_addr     = 0;
	upi_emphy_fr_device.tx_statmode       = WP_PKT_STAT_ENABLE;
	upi_emphy_fr_device.tx_maxsdu         = 1536;
	upi_emphy_fr_device.rx_bit_rate       = 64000;
	upi_emphy_fr_device.rx_slave_addr     = 0;
	upi_emphy_fr_device.rx_intmode        = WP_PKT_INTMODE_ENABLE;
	upi_emphy_fr_device.rx_statmode       = WP_PKT_STAT_ENABLE;
	upi_emphy_fr_device.rx_opmode         = WP_FRCH_OP_NORMAL;
	upi_emphy_fr_device.rx_fr_header_type = WP_FRCH_HEADER_2BYTES;
	upi_emphy_fr_device.rx_lookupmode     = WP_FR_LOOKUP_TABLE;
	upi_emphy_fr_device.rx_n_dlci         = 1024;
	upi_emphy_fr_device.rx_min_dlci       = 0;
	upi_emphy_fr_device.rx_maxsdu         = 1536;
	upi_emphy_fr_device.rx_queue          = qniw;
	upi_emphy_fr_device.tx_tq_types[0]    = WP_PKT_SCHED_WFQ;
	upi_emphy_fr_device.tx_tq_types[1]    = WP_PKT_SCHED_WFQ;
	upi_emphy_fr_device.tx_tq_types[2]    = WP_PKT_SCHED_WFQ;
	upi_emphy_fr_device.tx_tq_types[3]    = WP_PKT_SCHED_WFQ;
	upi_emphy_fr_device.crctype           = WP_HDLC_CRC32;

	tdm_device[i] = WP_DeviceCreate(emphy_port,
					WP_PHY(ufe_phy_index),
					WP_DEVICE_FR,
					&upi_emphy_fr_device);
	terminate_on_error(tdm_device[i], "WP_DeviceCreate FR");
#endif
#ifdef HDLC_TRANS
#if 0
	// Transparent device
	upi_trans_device.mode             = (WP_EMPHY_TRANS_UNFRAMED | WP_EMPHY_TRANS_E1);
	upi_trans_device.structure_size   = WP_TDM_STRUCTURE_BASIC;
	upi_trans_device.tx_dataunit_size = 32;
	upi_trans_device.rx_dataunit_size = 32;
	upi_trans_device.ds1_id           = 0;
	
	tdm_device[i] = WP_DeviceCreate(emphy_port,
					WP_PHY(ufe_phy_index),
					WP_DEVICE_TRANS,
					&upi_trans_device);
	terminate_on_error(tdm_device[i], "WP_DeviceCreate TRANS");
#endif
	upi_pos_device.tx_bit_rate    = 64000;
	upi_pos_device.tx_slave_addr  = 0;
	upi_pos_device.tx_statmode    = WP_PKT_STAT_ENABLE;
	upi_pos_device.tx_maxsdu      = 1536;
	upi_pos_device.rx_bit_rate    = 64000;
	upi_pos_device.rx_slave_addr  = 0;
	upi_pos_device.rx_statmode    = WP_PKT_STAT_ENABLE;
	upi_pos_device.flowmode       = WP_FLOWMODE_SINGLE;
	upi_pos_device.addr_mask      = 0;
	upi_pos_device.address1       = 0;
	upi_pos_device.address2       = 0;
	upi_pos_device.tx_tq_types[0] = WP_PKT_SCHED_WFQ;
	upi_pos_device.tx_tq_types[1] = WP_PKT_SCHED_WFQ;
	upi_pos_device.tx_tq_types[2] = WP_PKT_SCHED_WFQ;
	upi_pos_device.tx_tq_types[3] = WP_PKT_SCHED_WFQ;
	upi_pos_device.crctype        = WP_HDLC_CRC32;
	
	tdm_device[i] = WP_DeviceCreate(emphy_port,
					WP_PHY(ufe_phy_index),
					WP_DEVICE_HDLC,
					&upi_pos_device);
	terminate_on_error(tdm_device[i], "WP_DeviceCreate HDLC");

#endif

	// --------------------------------------------------------------------------
	//
	//                        Channels
	//
	// --------------------------------------------------------------------------

#ifdef ATM

	aal5_shaping_param.weight        = 1;

	// Create Tx and Rx channels
	aal5_ch_config.vpi               = 1;
	aal5_ch_config.vci               = 31;
	aal5_ch_config.intmode           = WP_ATMCH_INT_ENABLE;
	aal5_ch_config.iwmode            = WP_ATMCH_IWM_DISABLE;
	aal5_ch_config.statmode          = WP_ATMCH_STAT_ENABLE;
	aal5_ch_config.testmode          = WP_ATMCH_TEST_DISABLE;
	aal5_ch_config.tx_pqblock        = 0;
	aal5_ch_config.tx_pqlevel        = 0;
	aal5_ch_config.tx_shaping_type   = WP_ATM_SHAPING_UNSHAPED;
	aal5_ch_config.tx_shaping_params = &aal5_shaping_param;
	aal5_ch_config.tx_shaping_group  = 0;
	aal5_ch_config.tx_uumode         = WP_ATMCH_UU_DISABLE;
	aal5_ch_config.rx_timestamp      = 0;
	aal5_ch_config.rx_maxsdu         = 1500;
	aal5_ch_config.tx_mqblock        = 0;
	aal5_ch_config.tx_mqlevel        = 0;
	aal5_ch_config.tx_mqweight       = 0;
	aal5_ch_config.tx_cwid           = WP_CW_ID_A;
	aal5_ch_config.tx_tq             = 0;
	aal5_ch_config.mq_shaping_params = NULL;
	aal5_ch_config.rx_oamee          = 0;
	aal5_ch_config.rx_oamseg         = 0;

	tx_tdm_handle[i] = WP_ChannelCreate(0x400 + i,
					    tdm_device[i],
					    qnhost,
					    WP_CH_TX, 
					    WP_ATM_AAL5,
					    &aal5_ch_config);
	terminate_on_error(tx_tdm_handle[i], "WP_ChannelCreate Tx AAL5");
		
	rx_tdm_handle[i] = WP_ChannelCreate(TAG_ATM0 + i,
					    tdm_device[i],
					    qnhost,
					    WP_CH_RX, 
					    WP_ATM_AAL5,
					    &aal5_ch_config);
	terminate_on_error(rx_tdm_handle[i], "WP_ChannelCreate Rx AAL5");
#endif

#ifdef HDLC
	// Create Tx and Rx channels
	pkt_shaping_param.weight            = 1;
	pkt_shaping_param.weight_fraction   = 1;
	
#ifdef HDLC_FR
	fr_ch_config.intmode               = WP_PKTCH_INT_DISABLE;
	fr_ch_config.statmode              = WP_PKTCH_STAT_ENABLE;
	fr_ch_config.iwmode                = WP_PKTCH_IWM_DISABLE;
	fr_ch_config.tx_opmode             = WP_FRCH_OP_NORMAL;
	fr_ch_config.tx_fr_header_type     = WP_FRCH_HEADER_2BYTES;
	fr_ch_config.tx_fragment_size      = 0; 
	fr_ch_config.tx_pqblock            = 0;
	fr_ch_config.tx_pqlevel            = 0;
	fr_ch_config.tx_shaping_type       = WP_PKT_SHAPING_WFQ;
	fr_ch_config.tx_shaping_params     = &pkt_shaping_param;
	fr_ch_config.tx_cwid               = 0;
	fr_ch_config.tx_tq                 = 0;
	
	tx_tdm_handle[i] = WP_ChannelCreate(0x400 + i,
					    tdm_device[i],
					    qnfrtx,
					    WP_CH_TX, 
					    WP_FRAME_RELAY,
					    &fr_ch_config);
	terminate_on_error(tx_tdm_handle[i], "WP_ChannelCreate Tx FR");
	
	fr_ch_config.intmode               = WP_PKTCH_INT_ENABLE;
	fr_ch_config.statmode              = WP_PKTCH_STAT_ENABLE;
	fr_ch_config.iwmode                = WP_PKTCH_IWM_DISABLE;
	fr_ch_config.rx_queue_threshold    = 200;
	fr_ch_config.rx_dlci               = 111;
	fr_ch_config.rx_fbp_drop_threshold = 0;
	
	rx_tdm_handle[i] = WP_ChannelCreate(TAG_FR0 + i,
					    tdm_device[i],
					    qniw,
					    WP_CH_RX, 
					    WP_FRAME_RELAY,
					    &fr_ch_config);
	terminate_on_error(rx_tdm_handle[i], "WP_ChannelCreate Rx FR");
#endif
#ifdef HDLC_TRANS
	hdlc_ch_config.intmode           = WP_PKTCH_INT_ENABLE;
	hdlc_ch_config.iwmode            = WP_PKTCH_IWM_DISABLE;
	hdlc_ch_config.testmode          = WP_PKTCH_TEST_DISABLE;
	hdlc_ch_config.tx_pqblock        = 0;
	hdlc_ch_config.tx_pqlevel        = 0;
	hdlc_ch_config.tx_shaping_type   = WP_PKT_SHAPING_WFQ;
	hdlc_ch_config.tx_shaping_params = &pkt_shaping_param;
	hdlc_ch_config.rx_maxsdu         = 2048;
	hdlc_ch_config.tx_cwid           = 0;
	hdlc_ch_config.tx_tq             = 0;
	
	tx_tdm_handle[i] = WP_ChannelCreate(0x400 + i,
					    tdm_device[i],
					    qnhost,
					    WP_CH_TX, 
					    WP_HDLC,
					    &hdlc_ch_config);
	terminate_on_error(tx_tdm_handle[i], "WP_ChannelCreate Tx HDLC");
	
	rx_tdm_handle[i] = WP_ChannelCreate(TAG_HDLC0 + i,
					    tdm_device[i],
					    qnhost,
					    WP_CH_RX, 
					    WP_HDLC,
					    &hdlc_ch_config);
	terminate_on_error(rx_tdm_handle[i], "WP_ChannelCreate Rx HDLC");
#endif
#endif
	
#ifdef TRANS
	// Create Tx and Rx channels
	trans_ch_config.num_slots             = 32;
	trans_ch_config.slots                 = NULL;
	trans_ch_config.statmode              = WP_ENABLE;
	trans_ch_config.rx_buffersize         = 64;
	trans_ch_config.tx_jitter_buffer_size = 8;
	trans_ch_config.tx_udp_dummy_mode     = WP_TRANS_TX_LAST_FRAME;
	trans_ch_config.tx_udcas              = 0;
	for(j=0; j<WP_TRANS_FRAME_SIZE; j++) {
	    trans_ch_config.tx_udpattern[j]   = j;
	}
	for(j=0; j<WP_TRANS_FRAME_SIZE; j++) {
	    trans_ch_config.tx_udp_dummy_pattern[j]   = j;
	}
	
	rx_tdm_handle[i] = WP_ChannelCreate(TAG_TRANS0 + i,
					    tdm_device[i],
					    qniw,
					    WP_CH_RX, 
					    WP_TRANSPARENT,//_PWE3,
					    &trans_ch_config);
	terminate_on_error(rx_tdm_handle[i], "WP_ChannelCreate Rx TDM");

	bzero((char *)&agg_dm, sizeof(WP_iw_agg_directmap));
    
	agg_dm.tag                   = TAG_TRANS0 + i;
	agg_dm.txfunc                = rx_host_term;
	agg_dm.rfcs                  = WP_IW_RFCS_DISABLE;
	agg_dm.l2_header_insert_mode = WP_IW_L2H_INSERT_DISABLE;
	agg_dm.interruptqueue        = WP_IW_IRQT0;
	agg_dm.error_pkt_mode        = WP_IW_ERRPKT_TERMINATE;
	agg_dm.intmode               = WP_IW_INT_ENABLE;
	agg_dm.statmode              = WP_IW_STAT_ENABLE;
	agg_dm.timestamp_mode        = WP_IW_TIME_STAMP_DISABLE;
	agg_dm.ov_pool_mode          = WP_IW_OV_POOL_ENABLE;
	agg_dm.fbp_drop_threshold    = 0;
	agg_dm.mtu                   = 1650;
	agg_dm.prefix_length         = 0;
	agg_dm.extraction_length     = 0;
	agg_dm.policer_enable        = WP_IW_POLICER_DISABLE;
	agg_dm.policer_config        = NULL;
	agg_dm.cong_mode             = WP_IW_CONGESTION_DISABLE;
	agg_dm.cong_threshold_param  = NULL;
	agg_dm.dynamic_length_update = WP_IW_LENGTH_UPDATE_DISABLED;
	agg_dm.length_offset         = 0;
	agg_dm.flow_agg_type         = WP_IW_FLOW_AGG_PRIMARY;
	agg_dm.comp_group_handle     = 0;
	agg_dm.iw_port               = 0;
	
	iw_agg_trans[i] = WP_IwFlowAggregationCreate(0, WP_IW_DIRECTMAP_MODE, &agg_dm);
	terminate_on_error(iw_agg_trans[i], "WP_IwFlowAggregationCreate TRANS");

	rx_binding_trans.default_aggregation = iw_agg_trans[i];
	rx_binding_trans.mru                 = 64;

	status = WP_IwRxBindingCreate(rx_tdm_handle[i], iw_sys_trans, qniw, &rx_binding_trans);
	terminate_on_error(status, "RxBindingCreate TRANS"); 
#endif

	status = WUFE_PhyEnable(phy_handle[i], WUFE_FULL_DUPLEX);
	wufe_terminate_on_error(status, "WUFE_PhyEnable");
	
	// Enable device
	status = WP_DeviceEnable(tdm_device[i], WP_DIRECTION_DUPLEX);
	terminate_on_error(status, "WP_DeviceEnable");
    }

#ifdef __linux__
    // Init is done, start the threads
    pthread_create( &thread_id_event, NULL, &event_handler, NULL);
    pthread_create( &thread_id_led, NULL, &led_handler, NULL);
#endif

#if 0

    memset(&ufe_events, 0, sizeof(WUFE_events));
    
    ufe_events.emphy_events[0].event_reg            = WUFE_EVENT_EMPHY;
    ufe_events.emphy_events[0].phy_not_valid_handle = 0;
    
    ufe_events.tdm_events.event_reg                 = WUFE_EVENT_TDM;
    
    ufe_events.fm_events.event_reg                  = WUFE_EVENT_FM;
    
    ufe_events.machine_events.rx_event_reg          = WUFE_EVENT_MACHINE_RX;
    ufe_events.machine_events.tx_event_reg          = WUFE_EVENT_MACHINE_TX;
    
    ufe_events.bs_events.pll_lost_lock_event_reg    = 0x06ff;
    ufe_events.bs_events.absent_event_reg           = 0x000f;
    ufe_events.bs_events.los_event_reg              = 0x000f;
    
    status = WUFE_UfeEventEnable(0, &ufe_events);
    wufe_terminate_on_error(status, "WUFE_UfeEventEnable");

    //status = WUFE_SystemInterruptEnable(0);
    //wufe_terminate_on_error(status,"WUFE_SystsemInterruptEnable");

    //WUFE_UfeSfpHwInterruptEnable(0);
    //wufe_terminate_on_error(status,"WUFE_UfeSfpHwInterruptEnable");

    // We are all set, enable the channels
#endif

    WPX_FRMR_RegisterSonetSdhPortAlarmCallback(alarm_cback_port);
    WPX_FRMR_RegisterSonetSdhPathAlarmCallback(alarm_cback_path);
    WPX_FRMR_RegisterSocketClientPdhAlarmCallback(alarm_cback_client);
    WPX_FRMR_RegisterDiscreteClientPortAlarmCallback(alarm_cback_discrete);
    WPX_FRMR_RegisterSonetSdhSectionK1K2AnnounceCallback(alarm_cback_k1k2);
    WPX_FRMR_RegisterSonetSdhSectionS1AnnounceCallback(alarm_cback_S1);

    WPX_FRMR_PM_RegisterSonetSdhPortAnnounceCallback(alarm_cback_pm_port);
    WPX_FRMR_PM_RegisterSonetSdhHOPathAnnounceCallback(alarm_cback_pm_hpath);
    WPX_FRMR_PM_RegisterSonetSdhLOPathAnnounceCallback(alarm_cback_pm_lpath);
    WPX_FRMR_PM_RegisterSocketClientPdhAnnounceCallback(alarm_cback_pm_client);
    WPX_FRMR_RegisterDiscreteClientPortAnnounceCallback(alarm_cback_pm_discrete);

    for(alarmCategory = WPX_UFE_FRAMER_SDH_LOS;alarmCategory < WPX_UFE_FRAMER_SDH_TU_LOP;alarmCategory++)
	{
	    status = WPX_FRMR_DEV_DRV_SONET_SDH_EnableAlarm(0,alarmCategory);
	    if(status != 0)
		{
		    printf("WPX_FRMR_DEV_DRV_SONET_SDH_EnableAlarm fail alarmCategory = %d \n",alarmCategory);
		    flexmux_terminate_on_error(status, "WPX_FRMR_DEV_DRV_SONET_SDH_EnableAlarm");
		}
	}
    WPX_FRMR_DEV_DRV_SOCKET_EnableAlarm(0,WPX_UFE_FRAMER_DISCRETE_CLIENT_LOS);
    WPX_FRMR_DEV_DRV_SOCKET_EnableAlarm(0,WPX_UFE_FRAMER_DISCRETE_CLIENT_LOF);
    WPX_FRMR_DEV_DRV_SOCKET_EnableAlarm(0,WPX_UFE_FRAMER_DISCRETE_CLIENT_AIS);
    WPX_FRMR_DEV_DRV_SOCKET_EnableAlarm(0,WPX_UFE_FRAMER_DISCRETE_CLIENT_AIS_CI);
    WPX_FRMR_DEV_DRV_SOCKET_EnableAlarm(0,WPX_UFE_FRAMER_DISCRETE_CLIENT_RAI);


   /* Enable Ethernet Ports, Devices and Channels*/
    for(i=0; i<num_eth_ports; i++) {
	status = WP_ChannelEnable(rx_enet_handle[i]);
	terminate_on_error(status, "WP_ChannelEnable Rx ETH");
	
	status = WP_ChannelEnable(tx_enet_handle[i]);
	terminate_on_error(status, "WP_ChannelEnable Tx ETH");
    }

    for(i=0; i<4; i++) {
	// Enable Tx channel
#if !defined ( TRANS ) && !defined ( HDLC_TRANS )
	status = WP_ChannelEnable(tx_tdm_handle[i]);
	terminate_on_error(status, "WP_ChannelEnable TX TDM");
#endif

	// Enable Rx channel
#if !defined ( HDLC_TRANS )
	status = WP_ChannelEnable(rx_tdm_handle[i]);
	terminate_on_error(status, "WP_ChannelEnable RX TDM");
#endif
    }

    print_winmon_env();

    sleep(5);
    printf("Starting polling\n"); fflush(stdout);

    poll_eth_status();

    while(1) {
    	unsigned char c;
	
	printf("\n");
	printf("====================================\n");
	printf("0: quit\n");
	printf("1: ENET link status\n");
	printf("2: ENET stats\n");
	printf("3: IW port stats\n");
	printf("4: OC-3 link status\n");
	printf("5: ATM/FR/TRANS stats\n");
	printf("6: ENET send\n");
	printf("7: ATM/FR/TRANS send\n");
	printf("8: ENET send 256x\n");
	printf("9: ATM/FR/TRANS send 256x\n");
	printf("a: Set port 3 ETH\n");
	printf("b: Set port 3 STM-1\n");
	printf("c: Show all ETH port status\n");
	printf("d: Show all STM-1 port status\n");
	printf("e: Display UFE events\n");
	printf("f: Display PHY\n");
	printf("g: Poll all ETH link status (will not return)\n");
	printf("====================================\n");
	printf("\nEnter your choice: ");

#ifdef __linux__
	do {
	    c = getchar();
	} while(!isalnum(c));
#else
	do {
	    i = xs_getchar(&c, 0);
	} while (i != 0);
#endif
	printf("\n");

	if(c==0x30) {printf("Bye...\n"); break;}
	if(c==0x31) enet_link_status(eth_port);
	if(c==0x32) WPE_StatHsEnet(gbedev[eth_port]);
	if(c==0x33) iwport_stats(iw_port_enet[eth_port]);
	if(c==0x34) oc3_link_status(atm_port);
#ifdef ATM
	if(c==0x35) printAtmDeviceStats(tdm_device[atm_port]);
#endif
#ifdef HDLC_FR
	if(c==0x35) printFrDeviceStats(tdm_device[atm_port], tx_tdm_handle[atm_port], rx_tdm_handle[atm_port]);
#endif
#ifdef HDLC_TRANS
	if(c==0x35) printHdlcDeviceStats(tdm_device[atm_port], tx_tdm_handle[atm_port], rx_tdm_handle[atm_port]);
#endif
#ifdef TRANS
	if(c==0x35) printTransDeviceStats(tdm_device[atm_port], rx_tdm_handle[atm_port], iw_agg_trans[atm_port]);
#endif
	if(c==0x36) ethSend(tx_enet_handle[eth_port], 1);
#ifdef ATM
	if(c==0x37) atmSend(tx_tdm_handle[atm_port], 1);
#endif
#ifdef HDLC_FR
	if(c==0x37) frSend(tx_tdm_handle[atm_port], 1);
#endif
#ifdef HDLC_TRANS
	if(c==0x37) hdlcSend(tx_tdm_handle[atm_port], 1);
#endif
#ifdef TRANS
	if(c==0x37) transSend(tx_tdm_handle[atm_port], 1);
#endif
	if(c==0x38) ethSend(tx_enet_handle[eth_port], 256);
#ifdef ATM
	if(c==0x39) atmSend(tx_tdm_handle[atm_port], 256);
#endif
#ifdef HDLC_FR
	if(c==0x39) frSend(tx_tdm_handle[atm_port], 256);
#endif
#ifdef HDLC_TRANS
	if(c==0x39) hdlcSend(tx_tdm_handle[atm_port], 256);
#endif
#ifdef TRANS
	if(c==0x39) transSend(tx_tdm_handle[atm_port], 256);
#endif
	if(c==0x61) {WPX_PortModeSet(0, WPX_PORT_MODE_ETH, 3); terminate_on_error(status, "WPX_PortModeSet ETH");}
	if(c==0x62) {WPX_PortModeSet(0, WPX_PORT_MODE_ATM, 3); terminate_on_error(status, "WPX_PortModeSet STM1");}
	if(c==0x63) {enet_link_status(0);enet_link_status(1);enet_link_status(2);}
#ifdef LINE_PROTECTION
	if(c==0x64) {oc3_link_status(0);oc3_link_status(1);}
#else
	if(c==0x64) {oc3_link_status(0);oc3_link_status(3);}
#endif
	if(c==0x65) {displayUfeEvents();}
	if(c==0x66) WUFE_PhyDisplay(phy_handle[atm_port]);
	if(c==0x67) poll_eth_status();
    }

#ifdef __linux__
    kill_all_threads = 1;

    sleep(1);
#endif

    WPX_FRMR_LowLevelCommsDisableAccess(0);
    WPL_ThreadDestroy(&mailbox_thread_id);

    WPX_PortLedSet(0, WPX_PORT_LED_OFF, 0);
    WPX_PortLedSet(0, WPX_PORT_LED_OFF, 1);
    WPX_PortLedSet(0, WPX_PORT_LED_OFF, 2);
    WPX_PortLedSet(0, WPX_PORT_LED_OFF, 3);

    // Release WDDI
    WP_DriverRelease();

    return 0;
}

static void ethSend(WP_handle handle, int iter) 
{
    WP_data_unit    data_unit;
    WP_data_segment seg;
    int             i, j, status;

    for(j=0; j<iter; j++) {
	seg.data_size   = 100;
	seg.pool_handle = iwbuffer_pool;
	seg.next        = NULL;
	seg.data        = WP_PoolAlloc(iwbuffer_pool);
	if(seg.data == NULL) {
	    printf("Could not allocate  buffer\n");
	    return;
	}
	
	// Fill the buffer
	for(i=0; i<seg.data_size; i++) {
	    seg.data[i] = i;
	}
	
	data_unit.data_size  = seg.data_size;
	data_unit.segment    = &seg;
	data_unit.n_segments = 1;
	data_unit.n_active   = 1;
	data_unit.type       = WP_DATA_ENET;
	data_unit.control    = 0;
	
	printf("[ETH %4d]: Sending %d byte ETH packet\n", eth_tx_no++, data_unit.data_size); fflush(stdout);
	
	status = WP_HostSend(handle, &data_unit);
	terminate_on_error(status, "WP_HostSend");
    }

    return;
}

static void atmSend(WP_handle handle, int iter) 
{
    WP_data_unit    data_unit;
    WP_data_segment seg;
    int             i, j, status;

    for(j=0; j<iter; j++) {

	// Recover first
	data_unit.segment    = &seg;
	data_unit.n_segments = 1;
	data_unit.type       = WP_DATA_AAL5;

	status = WP_HostRecover(handle, &data_unit);
	terminate_on_error(status, "WP_HostRecover");

	for(i=0; i<data_unit.n_active; i++) {
	    WP_PoolFree(buffer_pool, seg.data);
	}

	// Send now
	seg.data_size   = 100;
	seg.pool_handle = buffer_pool;
	seg.next        = NULL;
	seg.data        = WP_PoolAlloc(buffer_pool);
	if(seg.data == NULL) {
	    printf("Could not allocate  buffer\n");
	    return;
	}
	
	// Fill the buffer
	for(i=0; i<seg.data_size; i++) {
	    seg.data[i] = i;
	}
	
	data_unit.data_size  = seg.data_size;
	data_unit.segment    = &seg;
	data_unit.n_segments = 1;
	data_unit.n_active   = 1;
	data_unit.type       = WP_DATA_AAL5;
	data_unit.control    = WP_HT_INTERRUPT;
	
	printf("[ATM %4d]: Sending %d byte ATM frame\n", atm_tx_no++, data_unit.data_size); fflush(stdout);
	
	status = WP_HostSend(handle, &data_unit);
	if(status) {
	    printf("WP_HostSend: --------------------------------------> Transmitter full\n");
	    continue;
	}
	//terminate_on_error(status, "WP_HostSend");
    }

    return;
}

#ifdef HDLC_FR
static void frSend(WP_handle handle, int iter) 
{
    WP_data_unit    data_unit;
    WP_data_segment seg;
    int             i, j, status;

    for(j=0; j<iter; j++) {

	// Send now
	seg.buffer_class = 0;
	seg.displacement = 0;
	seg.user_info    = 0;
	seg.data_size    = 40;
	seg.pool_handle  = iwbuffer_pool;
	seg.next         = NULL;
	seg.data         = WP_PoolAlloc(iwbuffer_pool);
	if(seg.data == NULL) {
	    printf("Could not allocate  buffer\n");
	    return;
	}
	
	// Fill the buffer
	for(i=0; i<seg.data_size; i++) {
	    seg.data[i] = i;
	}
	
	data_unit.data_size  = seg.data_size;
	data_unit.segment    = &seg;
	data_unit.n_segments = 1;
	data_unit.n_active   = 1;
	data_unit.type       = WP_DATA_FRAME_RELAY;
	data_unit.control    = 0;
	
	printf("[FR  %4d]: Sending %d byte FR frame\n", atm_tx_no++, data_unit.data_size); fflush(stdout);
	
	status = WP_HostSend(handle, &data_unit);
	if(status) {
	    printf("WP_HostSend: --------------------------------------> Transmitter full\n");
	    continue;
	}
	//terminate_on_error(status, "WP_HostSend");
    }

    return;
}
#endif

#ifdef HDLC_TRANS
static void hdlcSend(WP_handle handle, int iter) 
{
    WP_data_unit    data_unit;
    WP_data_segment seg;
    int             i, j, status;

    for(j=0; j<iter; j++) {

	// Send now
	seg.buffer_class = 0;
	seg.displacement = 0;
	seg.user_info    = 0;
	seg.data_size    = 40;
	seg.pool_handle  = iwbuffer_pool;
	seg.next         = NULL;
	seg.data         = WP_PoolAlloc(iwbuffer_pool);
	if(seg.data == NULL) {
	    printf("Could not allocate  buffer\n");
	    return;
	}
	
	// Fill the buffer
	for(i=0; i<seg.data_size; i++) {
	    seg.data[i] = i;
	}
	
	data_unit.data_size  = seg.data_size;
	data_unit.segment    = &seg;
	data_unit.n_segments = 1;
	data_unit.n_active   = 1;
	data_unit.type       = WP_DATA_PPP_HDLC;
	data_unit.control    = 0;
	
	printf("[HDLC  %4d]: Sending %d byte\n", atm_tx_no++, data_unit.data_size); fflush(stdout);
	
	status = WP_HostSend(handle, &data_unit);
	if(status) {
	    printf("WP_HostSend: --------------------------------------> Transmitter full\n");
	    continue;
	}
	//terminate_on_error(status, "WP_HostSend");
    }

    return;
}
#endif

#ifdef TRANS
static void transSend(WP_handle handle, int iter) 
{
    WP_data_unit    data_unit;
    WP_data_segment seg;
    int             i, j, status;

    for(j=0; j<iter; j++) {

	// Send now
	seg.buffer_class = 0;
	seg.displacement = 0;
	seg.user_info    = 0;
	seg.data_size    = 40;
	seg.pool_handle  = iwbuffer_pool;
	seg.next         = NULL;
	seg.data         = WP_PoolAlloc(iwbuffer_pool);
	if(seg.data == NULL) {
	    printf("Could not allocate  buffer\n");
	    return;
	}
	
	// Fill the buffer
	for(i=0; i<seg.data_size; i++) {
	    seg.data[i] = i;
	}
	
	data_unit.data_size  = seg.data_size;
	data_unit.segment    = &seg;
	data_unit.n_segments = 1;
	data_unit.n_active   = 1;
	data_unit.type       = WP_DATA_TRANSPARENT;
	data_unit.control    = 0;
	
	printf("[TRANS  %4d]: Sending %d byte\n", atm_tx_no++, data_unit.data_size); fflush(stdout);
	
	status = WP_HostSend(handle, &data_unit);
	if(status) {
	    printf("WP_HostSend: --------------------------------------> Transmitter full\n");
	    continue;
	}
	//terminate_on_error(status, "WP_HostSend");
    }

    return;
}
#endif

static char rbuf[2000];

static void process_enet(WP_tag tag, WP_U32 data, WP_U32 info)
{
    WP_data_unit    rx_data_unit;
    WP_data_segment seg[4];
    unsigned int status, i, j;

    //unsigned int port;
    //port = tag & 0xf;  // ENET
    //printf("Ethernet IP packet received on port %d with tag %x data %x info %x\n", port, tag, data, info);

    seg[0].next = &seg[1];
    seg[1].next = &seg[2];
    seg[2].next = &seg[3];
    seg[3].next = NULL;

    rx_data_unit.type       = WP_DATA_IW;
    rx_data_unit.segment    = &seg[0];
    rx_data_unit.n_segments = 4; 
    rx_data_unit.status     = 0;
    rx_data_unit.data_size  = 0;
    rx_data_unit.n_active   = 0;

    status = WP_HostReceive(rx_host_term, &rx_data_unit);

    // No data
    if(WP_ERROR(status) == WP_ERR_HST_NO_DATA_TO_GET) {
	printf("WP_HostReceive: NO DU\n");
	return;
    }
    terminate_on_error(status, "WP_HostReceive");

    printf("[ETH %4d]: Got %d bytes\n", eth_rx_no++, rx_data_unit.data_size);
    //if((eth_rx_no++ % 1000) == 0) printf("%d\n", eth_rx_no);

    for(j=0; j<rx_data_unit.n_active; j++) {
	//printf("segment %d/%d: data size = %d\n", j+1, rx_data_unit.n_active, seg[j].data_size);
	memcpy(rbuf, seg[j].data, seg[j].data_size);
#if 1
	for (i=0; i<seg[j].data_size; i++) {
	    if(seg[j].data[i] == i) {
		printf ("\33[32m"); printf("%02x", seg[j].data[i]);
	    } else {
		printf ("\33[31m"); printf("%02x", seg[j].data[i]);
	    }
	}
	printf("\33[0m");
	printf("\n-------------------------------------------------\n");
#endif	
	WP_PoolFree(seg[j].pool_handle, seg[j].data);
    }

    //printf("process_enet done\n");

    return;
}

static void process_enet_l2(WP_tag tag, WP_U32 data, WP_U32 info)
{
    WP_data_unit    rx_data_unit;
    WP_data_segment seg[4];
    unsigned int    status, i, j, port;

    seg[0].next = &seg[1];
    seg[1].next = &seg[2];
    seg[2].next = &seg[3];
    seg[3].next = NULL;

    port = tag & 0xf;  // ENET

    printf("Ethernet L2 packet received on port %d with tag %x data %x info %x handle %x\n", 
	   port, tag, data, info, rx_enet_handle[port]);

    rx_data_unit.type       = WP_DATA_ENET;
    rx_data_unit.segment    = &seg[0];
    rx_data_unit.n_segments = 4; 
    rx_data_unit.status     = 0;
    rx_data_unit.data_size  = 0;
    rx_data_unit.n_active   = 0;

    status = WP_HostReceive(rx_enet_handle[port], &rx_data_unit);

    // No data
    if(WP_ERROR(status) == WP_ERR_HST_NO_DATA_TO_GET) {
	printf("WP_HostReceive: NO DU\n");
	return;
    }
    terminate_on_error(status, "WP_HostReceive");

    printf("[ETH %4d]: Got %d bytes\n", eth_rx_no++, rx_data_unit.data_size);

    for(j=0; j<rx_data_unit.n_active; j++) {
	//printf("segment %d/%d: data size = %d\n", j+1, rx_data_unit.n_active, seg[j].data_size);
	for (i=0; i<seg[j].data_size; i++) {
	    if(seg[j].data[i] == i) {
		printf ("\33[32m"); printf("%02x", seg[j].data[i]);
	    } else {
		printf ("\33[31m"); printf("%02x", seg[j].data[i]);
	    }
	}
	printf("\33[0m");
	printf("\n-------------------------------------------------\n");
	
	WP_PoolFree(seg[j].pool_handle, seg[j].data);
    }
    
    return;
}

static void process_hdlc(WP_tag tag, WP_U32 data, WP_U32 info)
{
    WP_data_unit    rx_data_unit;
    WP_data_segment seg;
    unsigned int status, i, port;

    port = tag & 0xf;  // ENET

    printf("HDLC bytes received on port %d with tag %x data %x info %x\n", port, tag, data, info);

    rx_data_unit.type       = WP_DATA_TRANSPARENT;//WP_DATA_HDLC;
    rx_data_unit.segment    = &seg;
    rx_data_unit.n_segments = 1; 
    rx_data_unit.status     = 0;
    rx_data_unit.data_size  = 0;

    status = WP_HostReceive(rx_tdm_handle[port], &rx_data_unit);
    terminate_on_error(status, "WP_HostReceive");

    printf("[HDLC  %4d]: Got %d bytes\n", atm_rx_no++, rx_data_unit.data_size);
    for (i=0; i<rx_data_unit.data_size; i++) {
	if(seg.data[i] == i) {
	    printf ("\33[32m"); printf("%02x", seg.data[i]);
	} else {
	    printf ("\33[31m"); printf("%02x", seg.data[i]);
	}
    }
    printf("\33[0m");
    printf("\n-------------------------------------------------\n");

    WP_PoolFree(seg.pool_handle, seg.data);

    return;
}

static void process_trans(WP_tag tag, WP_U32 data, WP_U32 info)
{
    WP_data_unit    rx_data_unit;
    WP_data_segment seg;
    unsigned int status, i, port;

    port = tag & 0xf;  // ENET

    printf("TRANS bytes received on port %d with tag %x data %x info %x\n", port, tag, data, info);

    rx_data_unit.type       = WP_DATA_TRANSPARENT;
    rx_data_unit.segment    = &seg;
    rx_data_unit.n_segments = 1; 
    rx_data_unit.status     = 0;
    rx_data_unit.data_size  = 0;

    status = WP_HostReceive(rx_tdm_handle[port], &rx_data_unit);
    terminate_on_error(status, "WP_HostReceive");

    printf("[TRANS  %4d]: Got %d bytes\n", atm_rx_no++, rx_data_unit.data_size);
    for (i=0; i<rx_data_unit.data_size; i++) {
	if(seg.data[i] == i) {
	    printf ("\33[32m"); printf("%02x", seg.data[i]);
	} else {
	    printf ("\33[31m"); printf("%02x", seg.data[i]);
	}
    }
    printf("\33[0m");
    printf("\n-------------------------------------------------\n");

    WP_PoolFree(seg.pool_handle, seg.data);

    return;
}

static void process_fr(WP_tag tag, WP_U32 data, WP_U32 info)
{
    WP_data_unit    rx_data_unit;
    WP_data_segment seg;
    unsigned int status, i, port;

    port = tag & 0xf;  // ENET

    printf("FR packet received on port %d with tag %x data %x info %x\n", port, tag, data, info);

    rx_data_unit.type       = WP_DATA_FRAME_RELAY;
    rx_data_unit.segment    = &seg;
    rx_data_unit.n_segments = 1; 
    rx_data_unit.status     = 0;
    rx_data_unit.data_size  = 0;

    status = WP_HostReceive(rx_tdm_handle[port], &rx_data_unit);
    terminate_on_error(status, "WP_HostReceive");

    printf("[FR  %4d]: Got %d bytes\n", atm_rx_no++, rx_data_unit.data_size);
    for (i=0; i<rx_data_unit.data_size; i++) {
	if(seg.data[i] == i) {
	    printf ("\33[32m"); printf("%02x", seg.data[i]);
	} else {
	    printf ("\33[31m"); printf("%02x", seg.data[i]);
	}
    }
    printf("\33[0m");
    printf("\n-------------------------------------------------\n");

    WP_PoolFree(seg.pool_handle, seg.data);

    return;
}

static void process_atm(WP_tag tag, WP_U32 data, WP_U32 info)
{
    WP_data_unit    rx_data_unit;
    WP_data_segment seg;
    unsigned int status, i, port;

    port = tag & 0xf;  // ENET

    printf("ATM packet received on port %d with tag %x data %x info %x\n", port, tag, data, info);

    rx_data_unit.type       = WP_DATA_AAL5;
    rx_data_unit.segment    = &seg;
    rx_data_unit.n_segments = 1; 
    rx_data_unit.status     = 0;
    rx_data_unit.data_size  = 0;

    status = WP_HostReceive(rx_tdm_handle[port], &rx_data_unit);
    terminate_on_error(status, "WP_HostReceive");

    printf("[ATM %4d]: Got %d bytes\n", atm_rx_no++, rx_data_unit.data_size);
    for (i=0; i<rx_data_unit.data_size; i++) {
	if(seg.data[i] == i) {
	    printf ("\33[32m"); printf("%02x", seg.data[i]);
	} else {
	    printf ("\33[31m"); printf("%02x", seg.data[i]);
	}
    }
    printf("\33[0m");
    printf("\n-------------------------------------------------\n");

    WP_PoolFree(seg.pool_handle, seg.data);

    return;
}

static void My_AppIndicateEventRx(WP_tag tag, WP_U32 data, WP_U32 info)
{
#ifndef __linux__

    printf("My_AppIndicateEventRx: tag=%x data=%x info=%x\n", tag, data, info); fflush(stdout);

    if((tag >= TAG_ETH0) && (tag < TAG_ETH0 + 4)) {
	process_enet(tag, data, info);
    } else if((tag >= TAG_ETH0_L2) && (tag < TAG_ETH0_L2 + 10)) {
	process_enet_l2(tag, data, info);
    } else if((tag >= TAG_ATM0) && (tag < TAG_ATM0 + 4)) {
	process_atm(tag, data, info);
    } else if((tag >= TAG_TRANS0) && (tag < TAG_TRANS0 + 4)) {
	process_trans(tag, data, info);
    } else if((tag >= TAG_HDLC0) && (tag < TAG_HDLC0 + 4)) {
	process_hdlc(tag, data, info);
    } else {
	printf("Packet Received with unknown tag %x data %x info %x\n", tag, data, info);
    }
#else
    //printf("My_AppIndicateEventRx: tag=%x data=%x info=%x\n", tag, data, info); fflush(stdout);

    g_tag  = tag;
    g_data = data;
    g_info = info;

    sem_post(&sema);
#endif
    return;
}

static void My_AppIndicateEventTx(WP_tag tag, WP_U32 data, WP_U32 info)
{
    printf("My_AppIndicateEventTx %x %x %x\n", tag, data, info); fflush(stdout);

    return;
}

static void My_AppIndicateErrors(WP_tag tag, WP_U32 data, WP_U32 info)
{
    printf("My_AppIndicateErrors %x %x %x\n", tag, data, info); fflush(stdout);

    return;
}


static void
terminate_on_error(WP_handle handle, WP_CHAR *s)
{
    if (WP_ERROR_P(handle)) {
        printf("%s: Terminal error (handle 0x%x) %s\n", appname, handle, s);
        printf("%s: Error type - %s\n", appname, WP_error_name[WP_ERROR(handle)]);
        exit(1);
    } else {
        printf("App: Handle/Status returned from %s is %x \n", s, handle);
    }
    return;
}
static void
flexmux_terminate_on_error(WP_handle handle, WP_CHAR *s)
{
    if (handle) {
        printf("%s: Terminal error (handle 0x%x) %s\n", appname, handle, s);
        exit(1);
    } else {
        printf("App: Handle/Status returned from %s is %x \n", s, handle);
    }
    return;
}
static void
wufe_terminate_on_error(WP_handle handle, WP_CHAR *s)
{
    if (handle != WUFE_OK) {
        printf("%s: Terminal error (handle 0x%x) %s\n", appname, handle, s);
        printf("%s: Error type - %s\n", appname, WUFE_error_name[(handle&0x03ffffff)]);
        exit(1);
    } else {
        printf("App: Handle/Status returned from %s is %x \n", s, handle);
    }
    return;
}

typedef union StatField
{
    WP_U64 field;
    struct {
#if __LITTLE_ENDIAN
        WP_U32 low;
        WP_U32 high;
#else
        WP_U32 high;
        WP_U32 low;
#endif
    } part;
} WPE_StatField;

static void WPE_StatHsEnet(WP_handle hs_device)
{
    WP_stats_enet enet_stats;
    WP_status status;

    memset (&enet_stats, 0, sizeof(enet_stats));
    status = WP_DeviceStatistics(hs_device, &enet_stats);
    if (status)
        {
            printf("Error Reading Hs Enet Stat\n");
            exit(1);
        }
    printf("----------------------------------------------------\n");
      
    printf ("HS ENET Port Statistics (HW)\n");
            
    printf ("TxRx Frame  64:              %08x%08x\n",
            ((WPE_StatField)enet_stats.txrx_frames_64).part.high,
            ((WPE_StatField)enet_stats.txrx_frames_64).part.low);
            
    printf ("TxRx Frame 127:              %08x%08x\n",
            ((WPE_StatField)enet_stats.txrx_frames_127).part.high,
            ((WPE_StatField)enet_stats.txrx_frames_127).part.low);
            
    printf ("TxRx Frame 255:              %08x%08x\n",
            ((WPE_StatField)enet_stats.txrx_frames_255).part.high,
            ((WPE_StatField)enet_stats.txrx_frames_255).part.low);
            
    printf ("TxRx Frame 1023:             %08x%08x\n",
            ((WPE_StatField)enet_stats.txrx_frames_1023).part.high,
            ((WPE_StatField)enet_stats.txrx_frames_1023).part.low);
           
    printf ("TxRx Frame 1518:             %08x%08x\n",
            ((WPE_StatField)enet_stats.txrx_frames_1518).part.high,
            ((WPE_StatField)enet_stats.txrx_frames_1518).part.low);
         
    printf ("TxRx Frame 1522:             %08x%08x\n",
            ((WPE_StatField)enet_stats.txrx_frames_1522).part.high,
            ((WPE_StatField)enet_stats.txrx_frames_1522).part.low);
       
    printf ("Rx Bytes:                    %08x%08x\n",
            ((WPE_StatField)enet_stats.rx_bytes).part.high,
            ((WPE_StatField)enet_stats.rx_bytes).part.low);
         
    printf ("Rx Packets:                  %08x%08x\n",
            ((WPE_StatField)enet_stats.rx_packets).part.high,
            ((WPE_StatField)enet_stats.rx_packets).part.low);
         
    printf ("Rx Error FCS:                %08x%08x\n",
            ((WPE_StatField)enet_stats.rx_err_fcs).part.high,
            ((WPE_StatField)enet_stats.rx_err_fcs).part.low);
           
    printf ("Rx Multicast:                %08x%08x\n",
            ((WPE_StatField)enet_stats.rx_multicast).part.high,
            ((WPE_StatField)enet_stats.rx_multicast).part.low);
           
    printf ("Rx Broadcast:                %08x%08x\n",
            ((WPE_StatField)enet_stats.rx_broadcast).part.high,
            ((WPE_StatField)enet_stats.rx_broadcast).part.low);
           
    printf ("Rx Mac Control:              %08x%08x\n",
            ((WPE_StatField)enet_stats.rx_mac_control).part.high,
            ((WPE_StatField)enet_stats.rx_mac_control).part.low);
           
    printf ("Rx Mac Pause:                %08x%08x\n",
            ((WPE_StatField)enet_stats.rx_mac_pause).part.high,
            ((WPE_StatField)enet_stats.rx_mac_pause).part.low);
           
    printf ("Rx Mac Unknown:              %08x%08x\n",
            ((WPE_StatField)enet_stats.rx_mac_unknown).part.high,
            ((WPE_StatField)enet_stats.rx_mac_unknown).part.low);
           
    printf ("Rx Error Alignment:          %08x%08x\n",
            ((WPE_StatField)enet_stats.rx_err_alignment).part.high,
            ((WPE_StatField)enet_stats.rx_err_alignment).part.low);
           
    printf ("Rx Error LEN:                %08x%08x\n",
            ((WPE_StatField)enet_stats.rx_err_length).part.high,
            ((WPE_StatField)enet_stats.rx_err_length).part.low);
           
    printf ("Rx Error Code:               %08x%08x\n",
            ((WPE_StatField)enet_stats.rx_err_code).part.high,
            ((WPE_StatField)enet_stats.rx_err_code).part.low);
           
    printf ("Rx False Carrier:            %08x%08x\n",
            ((WPE_StatField)enet_stats.rx_false_carrier).part.high,
            ((WPE_StatField)enet_stats.rx_false_carrier).part.low);
           
    printf ("Rx Undersize:                %08x%08x\n",
            ((WPE_StatField)enet_stats.rx_undersize).part.high,
            ((WPE_StatField)enet_stats.rx_undersize).part.low);
           
    printf ("Rx Oversize:                 %08x%08x\n",
            ((WPE_StatField)enet_stats.rx_oversize).part.high,
            ((WPE_StatField)enet_stats.rx_oversize).part.low);
           
    printf ("Rx Fragments:                %08x%08x\n",
            ((WPE_StatField)enet_stats.rx_fragments).part.high,
            ((WPE_StatField)enet_stats.rx_fragments).part.low);
    printf ("Rx Jabber:                   %08x%08x\n",
            ((WPE_StatField)enet_stats.rx_jabber).part.high,
            ((WPE_StatField)enet_stats.rx_jabber).part.low);
    printf ("Rx Dropped:                  %08x%08x\n",
            ((WPE_StatField)enet_stats.rx_dropped).part.high,
            ((WPE_StatField)enet_stats.rx_dropped).part.low);

    printf ("Tx Bytes:                    %08x%08x\n",
            ((WPE_StatField)enet_stats.tx_bytes).part.high,
            ((WPE_StatField)enet_stats.tx_bytes).part.low); 

    printf ("Tx Packets:                  %08x%08x\n",
            ((WPE_StatField)enet_stats.tx_packets).part.high,
            ((WPE_StatField)enet_stats.tx_packets).part.low);

    printf ("Tx Multicast:                %08x%08x\n",
            ((WPE_StatField)enet_stats.tx_multicast).part.high,
            ((WPE_StatField)enet_stats.tx_multicast).part.low);

    printf ("Tx Broadcast:                %08x%08x\n",
            ((WPE_StatField)enet_stats.tx_broadcast).part.high,
            ((WPE_StatField)enet_stats.tx_broadcast).part.low);
            
    printf ("Tx Mac Pause:                %08x%08x\n",
            ((WPE_StatField)enet_stats.tx_mac_pause).part.high,
            ((WPE_StatField)enet_stats.tx_mac_pause).part.low);
            
    printf ("Tx Defer:                    %08x%08x\n",
            ((WPE_StatField)enet_stats.tx_defer).part.high,
            ((WPE_StatField)enet_stats.tx_defer).part.low);
            
    printf ("Tx Exess Defer:              %08x%08x\n",
            ((WPE_StatField)enet_stats.tx_excess_defer).part.high,
            ((WPE_StatField)enet_stats.tx_excess_defer).part.low);

    printf ("Tx Single Collision:         %08x%08x\n",
            ((WPE_StatField)enet_stats.tx_single_collision).part.high,
            ((WPE_StatField)enet_stats.tx_single_collision).part.low);

    printf ("Tx Multi Collision:          %08x%08x\n",
            ((WPE_StatField)enet_stats.tx_multi_collision).part.high,
            ((WPE_StatField)enet_stats.tx_multi_collision).part.low);

    printf ("Tx Late Collision:           %08x%08x\n",
            ((WPE_StatField)enet_stats.tx_late_collision).part.high,
            ((WPE_StatField)enet_stats.tx_late_collision).part.low);
            
    printf ("Tx Excess Collision:         %08x%08x\n",
            ((WPE_StatField)enet_stats.tx_excess_collision).part.high,
            ((WPE_StatField)enet_stats.tx_excess_collision).part.low);
            
    printf ("Tx No Collision:             %08x%08x\n",
            ((WPE_StatField)enet_stats.tx_no_collision).part.high,
            ((WPE_StatField)enet_stats.tx_no_collision).part.low);
            
    printf ("Tx Mac Pause Honored:        %08x%08x\n",
            ((WPE_StatField)enet_stats.tx_mac_pause_honored).part.high,
            ((WPE_StatField)enet_stats.tx_mac_pause_honored).part.low);
 
    printf ("Tx Dropped:                  %08x%08x\n",
            ((WPE_StatField)enet_stats.tx_dropped).part.high,
            ((WPE_StatField)enet_stats.tx_dropped).part.low);

    printf ("Tx Jabber:                   %08x%08x\n",
            ((WPE_StatField)enet_stats.tx_jabber).part.high,
            ((WPE_StatField)enet_stats.tx_jabber).part.low);
            
    printf ("Tx Errors FCS:               %08x%08x\n",
            ((WPE_StatField)enet_stats.tx_err_fcs).part.high,
            ((WPE_StatField)enet_stats.tx_err_fcs).part.low);
            
    printf ("Tx Control:                  %08x%08x\n",
            ((WPE_StatField)enet_stats.tx_control).part.high,
            ((WPE_StatField)enet_stats.tx_control).part.low);
            
    printf ("Tx Oversize:                 %08x%08x\n",
            ((WPE_StatField)enet_stats.tx_oversize).part.high,
            ((WPE_StatField)enet_stats.tx_oversize).part.low);
            
    printf ("Tx Undersize:                %08x%08x\n",
            ((WPE_StatField)enet_stats.tx_undersize).part.high,
            ((WPE_StatField)enet_stats.tx_undersize).part.low);
            
    printf ("Tx Fragments:                %08x%08x\n",
            ((WPE_StatField)enet_stats.tx_fragments).part.high,
            ((WPE_StatField)enet_stats.tx_fragments).part.low);


    printf ("HS ENET Device Statistics (DPS)\n");

    printf ("Rx Host Frames:              %08x%08x\n",
            ((WPE_StatField)enet_stats.rx_host_frames).part.high,
            ((WPE_StatField)enet_stats.rx_host_frames).part.low);
    printf ("Rx Iw Frames:                %08x%08x\n",
            ((WPE_StatField)enet_stats.rx_iw_frames).part.high,
            ((WPE_StatField)enet_stats.rx_iw_frames).part.low);
    printf ("Rx Error Host Full:          %08x%08x\n",
            ((WPE_StatField)enet_stats.rx_err_host_full).part.high,
            ((WPE_StatField)enet_stats.rx_err_host_full).part.low);
    printf ("Rx Error Fbp Underrun:       %08x%08x\n",
            ((WPE_StatField)enet_stats.rx_err_fbp_underrun).part.high,
            ((WPE_StatField)enet_stats.rx_err_fbp_underrun).part.low);
    printf ("Rx Error Nonvalid Mac:       %08x%08x\n",
            ((WPE_StatField)enet_stats.rx_err_nonvalid_mac).part.high,
            ((WPE_StatField)enet_stats.rx_err_nonvalid_mac).part.low);
    printf ("Rx Error Mru:                %08x%08x\n",
            ((WPE_StatField)enet_stats.rx_err_mru).part.high,
            ((WPE_StatField)enet_stats.rx_err_mru).part.low);
    printf ("Rx Error Sdu:                %08x%08x\n",
            ((WPE_StatField)enet_stats.rx_err_sdu).part.high,
            ((WPE_StatField)enet_stats.rx_err_sdu).part.low);
    printf ("Rx Error Underrun:           %08x%08x\n",
            ((WPE_StatField)enet_stats.tx_err_underrun).part.high,
            ((WPE_StatField)enet_stats.tx_err_underrun).part.low);
    printf ("Rx Error Overrun:            %08x%08x\n",
            ((WPE_StatField)enet_stats.rx_err_overrun).part.high,
            ((WPE_StatField)enet_stats.rx_err_overrun).part.low);
    printf ("Tx Frames:                   %08x%08x\n",
            ((WPE_StatField)enet_stats.tx_frames).part.high,
            ((WPE_StatField)enet_stats.tx_frames).part.low);
    printf("----------------------------------------------------\n");

}

static void iwport_stats(WP_handle iw_port)
{
    WP_route_port_stats iw_stats;
    WP_status status;

    printf ("IW Port Statistics \n");
    status = WP_IwPortStatistics(iw_port, &iw_stats);
    if(status) {printf("Error while getting IW port stats\n"); return;}
    printf ("rx_valid_packets             = %d\n", (WP_U32)(iw_stats.rx_valid_packets             & 0xffffffff));
    printf ("rx_ipv4_option_packets       = %d\n", (WP_U32)(iw_stats.rx_ipv4_option_packets       & 0xffff));
    printf ("rx_non_ip_packets            = %d\n", (WP_U32)(iw_stats.rx_non_ip_packets            & 0xffff));
    printf ("rx_compressed_packets        = %d\n", (WP_U32)(iw_stats.rx_compressed_packets        & 0xffff));
    printf ("rx_valid_mpls_packets        = %d\n", (WP_U32)(iw_stats.rx_valid_mpls_packets        & 0xffff));
    printf ("rx_protocol_error            = %d\n", (WP_U32)(iw_stats.rx_protocol_error            & 0xffff));
    printf ("rx_checksum_error            = %d\n", (WP_U32)(iw_stats.rx_checksum_error            & 0xffff));
    printf ("rx_discard_classifier        = %d\n", (WP_U32)(iw_stats.rx_discard_classifier        & 0xffff));
    printf ("rx_mpls_lookup_drop          = %d\n", (WP_U32)(iw_stats.rx_mpls_lookup_drop          & 0xffff));
    printf ("tx_forwarded_packets         = %d\n", (WP_U32)(iw_stats.tx_forwarded_packets         & 0xffffffff));
    printf ("rx_ipv6_hop_by_hop_host_term = %d\n", (WP_U32)(iw_stats.rx_ipv6_hop_by_hop_host_term & 0xffff));
    printf ("rx_ipv6_link_local_host_term = %d\n", (WP_U32)(iw_stats.rx_ipv6_link_local_host_term & 0xffff));

    return;
} 

#ifdef TRANS
static void printTransDeviceStats(WP_handle handle, WP_handle rx_handle, WP_handle agg) {
  WP_status                    status;
  WP_stats_emphy_trans         stats;
  WP_stats_emphy_trans_pwe3_rx rx_stats;
  WP_iw_flow_stats             flowStats;

  status = WP_DeviceStatistics(handle, &stats);
  //terminate_on_error(handle, "WP_DeviceStatistics() FR");

  status = WP_ChannelStatistics(rx_handle, &rx_stats);
  //terminate_on_error(handle, "WP_ChannelStatistics(RX) TRANS");

  if(agg) {
      status = WP_IwFlowStatistics(agg, WP_IW_FLOW_STAT, &flowStats);
      //terminate_on_error(handle, "WP_IwFlowStatistics(RX) TRANS");
  }

  printf("\n");
  printf("TRANS Device Statistics:\n");
  printf("------------------------\n");
  printf("\n");
  printf("phy_buffer_overrun             = %d\n", (WP_U32)(stats.phy_buffer_overrun & 0xffff));
  printf("phy_out_of_sync                = %d\n", (WP_U32)(stats.phy_out_of_sync & 0xffff));

  if(agg) {
      printf("\n");
      printf("TRANS Rx Channel Statistics:\n");
      printf("----------------------------\n");
      printf("\n");
      printf("channel_out_of_sync            = %d\n", (WP_U32)(rx_stats.channel_out_of_sync & 0xffff));
      printf("empty_fbp                      = %d\n", (WP_U32)(rx_stats.empty_fbp & 0xffff));
      printf("valid_packets                  = %d\n", (WP_U32)(rx_stats.valid_packets & 0xffffffff));
      
      printf("\n");
      printf("TRANS Rx Agg Statistics:\n");
      printf("------------------------\n");
      printf("\n");
      printf("forward_packet                 = %d\n", (WP_U32)(flowStats.forward_packet & 0xffffffff));
      printf("fbp_drop_packets               = %d\n", (WP_U32)(flowStats.fbp_drop_packets & 0xffff));
      printf("mtu_drop_packets               = %d\n", (WP_U32)(flowStats.mtu_drop_packets & 0xffff));
      printf("ttl_drop_packets               = %d\n", (WP_U32)(flowStats.ttl_drop_packets & 0xffff));
      printf("tx_queue_drop_packets          = %d\n", (WP_U32)(flowStats.tx_queue_drop_packets & 0xffff));
      printf("mpls_drop                      = %d\n", (WP_U32)(flowStats.mpls_drop & 0xffff));
      printf("denied_packets                 = %d\n", (WP_U32)(flowStats.denied_packets & 0xffff));
      printf("group_filtered_packets         = %d\n", (WP_U32)(flowStats.group_filtered_packets & 0xffff));
      printf("forwarded_bytes                = %d\n", (WP_U32)(flowStats.forwarded_bytes & 0xffffffff));
      printf("gtp_bad_headers                = %d\n", (WP_U32)(flowStats.gtp_bad_headers & 0xffff));
      printf("policer_non_conforming_packets = %d\n", (WP_U32)(flowStats.policer_non_conforming_packets & 0xffff));
  } else {
      printf("\n");
      printf("HDLC Rx Channel Statistics:\n");
      printf("----------------------------\n");
      printf("\n");
  }
  return;
}
#endif

#ifdef HDLC_FR
static void printFrDeviceStats(WP_handle handle, WP_handle tx_handle, WP_handle rx_handle) {
  WP_status       status;
  WP_stats_pos_fr stats;
  WP_stats_fr_tx  tx_stats;
  WP_stats_fr_rx  rx_stats;

  status = WP_DeviceStatistics(handle, &stats);
  //terminate_on_error(handle, "WP_DeviceStatistics() FR");

  printf("\n");
  printf("FR Device Statistics:\n");
  printf("---------------------\n");
  printf("\n");
  printf("rx_frames                     = %d\n", (WP_U32)(stats.rx_frames));
  printf("rx_bytes                      = %d\n", (WP_U32)(stats.rx_bytes));
  printf("rx_frames_de_set              = %d\n", (WP_U32)(stats.rx_frames_de_set));
  printf("rx_bytes_de_set               = %d\n", (WP_U32)(stats.rx_bytes_de_set));
  printf("rx_frames_fecn_set            = %d\n", (WP_U32)(stats.rx_frames_fecn_set));
  printf("rx_frames_becn_set            = %d\n", (WP_U32)(stats.rx_frames_becn_set));
  printf("rx_discarded_err_crc          = %d\n", (WP_U32)(stats.rx_discarded_err_crc));
  printf("rx_discarded_err_alignment    = %d\n", (WP_U32)(stats.rx_discarded_err_alignment));
  printf("rx_discarded_length_violation = %d\n", (WP_U32)(stats.rx_discarded_length_violation));
  printf("rx_discarded_illegal_header   = %d\n", (WP_U32)(stats.rx_discarded_illegal_header));
  printf("rx_discarded_abort            = %d\n", (WP_U32)(stats.rx_discarded_abort));
  printf("rx_discarded_unsupported_dlci = %d\n", (WP_U32)(stats.rx_discarded_unsupported_dlci));
  printf("rx_last_unsupported_dlci      = %d\n", (WP_U32)(stats.rx_last_unsupported_dlci));
  printf("rx_discarded_fbp_empty        = %d\n", (WP_U32)(stats.rx_discarded_fbp_empty));
  printf("rx_discarded_hostq_full       = %d\n", (WP_U32)(stats.rx_discarded_hostq_full));
  printf("rx_discarded_err_parity       = %d\n", (WP_U32)(stats.rx_discarded_err_parity));
  printf("rx_discarded_err_sop_eop      = %d\n", (WP_U32)(stats.rx_discarded_err_sop_eop));
  printf("rx_total_err_bytes            = %d\n", (WP_U32)(stats.rx_total_err_bytes));
  printf("tx_frames                     = %d\n", (WP_U32)(stats.tx_frames));
  printf("tx_bytes                      = %d\n", (WP_U32)(stats.tx_bytes));
  printf("tx_frames_de_set              = %d\n", (WP_U32)(stats.tx_frames_de_set));
  printf("tx_bytes_de_set               = %d\n", (WP_U32)(stats.tx_bytes_de_set));
  printf("tx_frames_fecn_set            = %d\n", (WP_U32)(stats.tx_frames_fecn_set));
  printf("tx_frames_becn_set            = %d\n", (WP_U32)(stats.tx_frames_becn_set));
  printf("tx_underrun                   = %d\n", (WP_U32)(stats.tx_underrun));
  printf("tx_aborted_frames             = %d\n", (WP_U32)(stats.tx_aborted_frames));

  printf("DPS tx_frames                 = %d\n", (WP_U32)(stats.serial_dps_if_stats.tx_frames));
  printf("DPS tx_bytes                  = %d\n", (WP_U32)(stats.serial_dps_if_stats.tx_bytes));
  printf("DPS rx_frames                 = %d\n", (WP_U32)(stats.serial_dps_if_stats.rx_frames));
  printf("DPS rx_bytes                  = %d\n", (WP_U32)(stats.serial_dps_if_stats.rx_bytes));
  printf("DPS rx_dropped_frames         = %d\n", (WP_U32)(stats.serial_dps_if_stats.rx_dropped_frames));
  printf("DPS rx_pce_denied_frames      = %d\n", (WP_U32)(stats.serial_dps_if_stats.rx_pce_denied_frames));

  status = WP_ChannelStatistics(tx_handle, &tx_stats);
  //terminate_on_error(handle, "WP_ChannelStatistics(TX) FR");

  status = WP_ChannelStatistics(rx_handle, &rx_stats);
  //terminate_on_error(handle, "WP_ChannelStatistics(RX) FR");

  printf("\n");
  printf("FR Tx Channel Statistics:\n");
  printf("-------------------------\n");
  printf("\n");
  printf("tx_frames                     = %d\n", (WP_U32)(tx_stats.tx_frames));
  printf("tx_bytes                      = %d\n", (WP_U32)(tx_stats.tx_bytes));
  printf("tx_frames_de_set              = %d\n", (WP_U32)(tx_stats.tx_frames_de_set));
  printf("tx_bytes_de_set               = %d\n", (WP_U32)(tx_stats.tx_bytes_de_set));
  printf("tx_frames_fecn_set            = %d\n", (WP_U32)(tx_stats.tx_frames_fecn_set));
  printf("tx_frames_becn_set            = %d\n", (WP_U32)(tx_stats.tx_frames_becn_set));
  printf("tx_aborted_frames             = %d\n", (WP_U32)(tx_stats.tx_aborted_frames));
  printf("\n");

  printf("FR Rx Channel Statistics:\n");
  printf("-------------------------\n");
  printf("\n");
  printf("rx_frames                     = %d\n", (WP_U32)(rx_stats.rx_frames));
  printf("rx_bytes                      = %d\n", (WP_U32)(rx_stats.rx_bytes));
  printf("rx_frames_de_set              = %d\n", (WP_U32)(rx_stats.rx_frames_de_set));
  printf("rx_bytes_de_set               = %d\n", (WP_U32)(rx_stats.rx_bytes_de_set));
  printf("rx_frames_fecn_set            = %d\n", (WP_U32)(rx_stats.rx_frames_fecn_set));
  printf("rx_frames_becn_set            = %d\n", (WP_U32)(rx_stats.rx_frames_becn_set));
  printf("rx_discarded_err_crc          = %d\n", (WP_U32)(rx_stats.rx_discarded_err_crc) & 0xffff);
  printf("rx_discarded_err_alignment    = %d\n", (WP_U32)(rx_stats.rx_discarded_err_alignment) & 0xffff);
  printf("rx_discarded_length_violation = %d\n", (WP_U32)(rx_stats.rx_discarded_length_violation) & 0xffff);
  printf("rx_discarded_abort            = %d\n", (WP_U32)(rx_stats.rx_discarded_abort) & 0xffff);
  printf("rx_discarded_fbp_empty        = %d\n", (WP_U32)(rx_stats.rx_discarded_fbp_empty) & 0xffff);
  printf("rx_discarded_hostq_full       = %d\n", (WP_U32)(rx_stats.rx_discarded_hostq_full) & 0xffff);
  printf("\n");

  return;
}

#endif

#ifdef HDLC_TRANS
static void printHdlcDeviceStats(WP_handle handle, WP_handle tx_handle, WP_handle rx_handle) {
  WP_status         status;
  WP_stats_upi_hdlc stats;

  status = WP_DeviceStatistics(handle, &stats);
  //terminate_on_error(handle, "WP_DeviceStatistics() HDLC");

  printf("\n");
  printf("HDLC Device Statistics:\n");
  printf("---------------------\n");
  printf("\n");
  printf("rx_frames                     = %d\n", (WP_U32)(stats.rx_frames));
  printf("rx_iw_frames                  = %d\n", (WP_U32)(stats.rx_iw_frames));
  printf("rx_bytes                      = %d\n", (WP_U32)(stats.rx_bytes));
  printf("rx_err_crc                    = %d\n", (WP_U32)(stats.rx_err_crc));
  printf("rx_err_phy                    = %d\n", (WP_U32)(stats.rx_err_phy));
  printf("rx_err_addr_mismatch          = %d\n", (WP_U32)(stats.rx_err_addr_mismatch));
  printf("rx_err_buffer_overrun         = %d\n", (WP_U32)(stats.rx_err_buffer_overrun));
  printf("rx_err_overrun                = %d\n", (WP_U32)(stats.rx_err_overrun));
  printf("rx_err_sop_eop                = %d\n", (WP_U32)(stats.rx_err_sop_eop));
  printf("rx_err_parity                 = %d\n", (WP_U32)(stats.rx_err_parity));
  printf("rx_err_iw_buffer_underrun     = %d\n", (WP_U32)(stats.rx_err_iw_buffer_underrun));
  printf("rx_err_iw_mru                 = %d\n", (WP_U32)(stats.rx_err_iw_mru));
  printf("rx_err_maxsdu                 = %d\n", (WP_U32)(stats.rx_err_maxsdu));
  printf("rx_err_bytes                  = %d\n", (WP_U32)(stats.rx_err_bytes));
  printf("rx_err_abort                  = %d\n", (WP_U32)(stats.rx_err_abort));
  printf("rx_err_iw_l2_parse            = %d\n", (WP_U32)(stats.rx_err_iw_l2_parse));
  printf("tx_frames                     = %d\n", (WP_U32)(stats.tx_frames));
  printf("tx_bytes                      = %d\n", (WP_U32)(stats.tx_bytes));
  printf("DPS tx_frames                 = %d\n", (WP_U32)(stats.serial_dps_if_stats.tx_frames));
  printf("DPS tx_bytes                  = %d\n", (WP_U32)(stats.serial_dps_if_stats.tx_bytes));
  printf("DPS rx_frames                 = %d\n", (WP_U32)(stats.serial_dps_if_stats.rx_frames));
  printf("DPS rx_bytes                  = %d\n", (WP_U32)(stats.serial_dps_if_stats.rx_bytes));
  printf("DPS rx_dropped_frames         = %d\n", (WP_U32)(stats.serial_dps_if_stats.rx_dropped_frames));
  printf("DPS rx_pce_denied_frames      = %d\n", (WP_U32)(stats.serial_dps_if_stats.rx_pce_denied_frames));

  return;
}
#endif

#ifdef ATM
static void printAtmDeviceStats(WP_handle handle) {
  WP_status status;
  WP_stats_upi_atm atm_upi_stats = {0};

  status = WP_DeviceStatistics(handle, &atm_upi_stats);
  if(status) {
      terminate_on_error(handle, "WP_DeviceStatistics() ATM");
  }

  printf("\n");
  printf("Atm Device Statistics:\n");
  printf("----------------------\n");
  printf("\n");
  printf("Rx short/long cells  = %#x%x\n",(WP_U32)(atm_upi_stats.rx_err_slc >> 32), (WP_U32)atm_upi_stats.rx_err_slc);
  printf("Rx Error Parity      = %#x%x\n",(WP_U32)(atm_upi_stats.rx_err_parity >> 32), (WP_U32)atm_upi_stats.rx_err_parity);
  printf("Rx address mismatch  = %#x%x\n",(WP_U32)(atm_upi_stats.rx_err_addr_mismatch >> 32), (WP_U32)atm_upi_stats.rx_err_addr_mismatch);
  printf("Rx cell count        = %#x%x\n",(WP_U32)(atm_upi_stats.ext_stats.rx_cells >> 32), (WP_U32)atm_upi_stats.ext_stats.rx_cells);
  printf("rx_clp0_cells        = %#x%x\n",(WP_U32)(atm_upi_stats.ext_stats.rx_clp0_cells >> 32), (WP_U32)atm_upi_stats.ext_stats.rx_clp0_cells);
  printf("rx_clp1_cells        = %#x%x\n",(WP_U32)(atm_upi_stats.ext_stats.rx_clp1_cells >> 32), (WP_U32)atm_upi_stats.ext_stats.rx_clp1_cells);
  printf("rx_gfc_cells         = %#x%x\n",(WP_U32)(atm_upi_stats.ext_stats.rx_gfc_cells >> 32), (WP_U32)atm_upi_stats.ext_stats.rx_gfc_cells);
  printf("rx_oam_cells         = %#x%x\n",(WP_U32)(atm_upi_stats.ext_stats.rx_oam_cells >> 32), (WP_U32)atm_upi_stats.ext_stats.rx_oam_cells);
  printf("rx_rm_cells          = %#x%x\n",(WP_U32)(atm_upi_stats.ext_stats.rx_rm_cells >> 32), (WP_U32)atm_upi_stats.ext_stats.rx_rm_cells);
  printf("rx_last_unknown_addr = %#x%x\n",(WP_U32)(atm_upi_stats.ext_stats.rx_last_unknown_addr >> 32), (WP_U32)atm_upi_stats.ext_stats.rx_last_unknown_addr);
  printf("tx_cells             = %#x%x\n",(WP_U32)(atm_upi_stats.ext_stats.tx_cells >> 32), (WP_U32)atm_upi_stats.ext_stats.tx_cells);
  printf("tx_clp0_cells        = %#x%x\n",(WP_U32)(atm_upi_stats.ext_stats.tx_clp0_cells >> 32), (WP_U32)atm_upi_stats.ext_stats.tx_clp0_cells);
  printf("tx_clp1_cells        = %#x%x\n",(WP_U32)(atm_upi_stats.ext_stats.tx_clp1_cells >> 32), (WP_U32)atm_upi_stats.ext_stats.tx_clp1_cells);
  printf("tx_efci_cells        = %#x%x\n",(WP_U32)(atm_upi_stats.ext_stats.tx_efci_cells >> 32), (WP_U32)atm_upi_stats.ext_stats.tx_efci_cells);
  printf("tx_rm_cells          = %#x%x\n",(WP_U32)(atm_upi_stats.ext_stats.tx_rm_cells >> 32), (WP_U32)atm_upi_stats.ext_stats.tx_rm_cells);
  printf("tx_oam_cells         = %#x%x\n",(WP_U32)(atm_upi_stats.ext_stats.tx_oam_cells >> 32), (WP_U32)atm_upi_stats.ext_stats.tx_oam_cells);
  printf("tx_idle_cells        = %#x%x\n",(WP_U32)(atm_upi_stats.ext_stats.tx_idle_cells >> 32), (WP_U32)atm_upi_stats.ext_stats.tx_idle_cells);
  printf("rx_err_emphy_overrun = %#x%x\n",(WP_U32)(atm_upi_stats.emphy_stats.rx_err_emphy_overrun >> 32), (WP_U32)atm_upi_stats.emphy_stats.rx_err_emphy_overrun); 
  printf("rx_err_hec           = %#x%x\n",(WP_U32)(atm_upi_stats.emphy_stats.rx_err_hec >> 32), (WP_U32)atm_upi_stats.emphy_stats.rx_err_hec);
  printf("rx__out_of_sync      = %#x%x\n",(WP_U32)(atm_upi_stats.emphy_stats.rx_out_of_sync >> 32), (WP_U32)atm_upi_stats.emphy_stats.rx_out_of_sync);
  printf("rx_enter_sync        = %#x%x\n",(WP_U32)(atm_upi_stats.emphy_stats.rx_enter_sync >> 32), (WP_U32)atm_upi_stats.emphy_stats.rx_enter_sync);
  printf("rx_err_emphy         = %#x%x\n",(WP_U32)(atm_upi_stats.emphy_stats.rx_err_emphy >> 32), (WP_U32)atm_upi_stats.emphy_stats.rx_err_emphy);
  if(atm_upi_stats.emphy_stats.rx_sync_state == WP_DEV_EMPHY_IN_SYNC) {
      printf("rx_sync_state        = IN SYNC\n");
  } else {
      printf("rx_sync_state        = OUT OF SYNC\n");
  }
  printf("\n");
}
#endif

static void enet_link_status(int eth_port) {
    unsigned int tstat, status, tmp;

    tstat = 0;

    switch(eth_port) {
    case 0 :
	tstat = MAP_WN1_TSTAT_0;
	break;
    case 1 :
	tstat = MAP_WN2_TSTAT_0;
	break;
    case 2 :
	tstat = MAP_WN1_TSTAT_1;
	break;
    default :
	tstat = MAP_WN1_TSTAT_0;
    }

    tstat &= 0x1fffffff;

    tmp = *(volatile unsigned int *)(unsigned long)(tstat + WPL_RIF_VIRTUAL(0, 0));
    status = ntohl(tmp);

    //printf("status = %x\n", status);

    if(status & 0x4) {
	printf("ETH  PORT %d: LINK OK\n", eth_port);
    } else {
	printf("ETH  PORT %d: LINK KO !!!!!!!!!!!!\n", eth_port);
    }

    return;
}

static void oc3_link_status(int atm_port) {
    WP_status status;
    WP_stats_upi_atm atm_upi_stats = {0};
    
    status = WP_DeviceStatistics(tdm_device[atm_port], &atm_upi_stats);
    
    if(status) {
	terminate_on_error(status, "WP_DeviceStatistics()");
    }

    if(atm_upi_stats.emphy_stats.rx_sync_state == WP_DEV_EMPHY_IN_SYNC) {
	printf("OC-3 PORT %x: LINK OK\n", atm_port);
    } else {
	printf("OC-3 PORT %x: LINK KO !!!!!!!!!!!!\n", atm_port);
    }

    printf("\n");    
    
    return;
}

#ifndef __linux__
#define UART_CONSOLE
#ifdef UART_CONSOLE
#define WEB_UART_STATUS_DR              0x01
#define MAP_UA_RBR 0xbe00c500
#define MAP_UA_LSR 0xbe00c51c
static int xs_getchar(unsigned char *c, unsigned int block) {
    WP_U32 status;

    do {
	status = *(volatile WP_U32 *)MAP_UA_LSR;   /* Read UART line status register */
	if (status & WEB_UART_STATUS_DR) {
	    *c = (WP_U8)*(volatile WP_U32 *)MAP_UA_RBR;
	    return 0;
	}
	if(block == 0) return -1;
    } while(1);
    
    return 0;
}
#else
// PCI Console
volatile WP_U32 *pciConsoleReadData    = (volatile WP_U32 *)0xbe100468;
volatile WP_U32 *pciConsoleStatus      = (volatile WP_U32 *)0xbe100410;
#define PCI_CONSOLE_READ_STATUS_MASK     0x00000040

static int xs_getchar(unsigned char *c, unsigned int block) {
    unsigned int status;

    do {
	status = *pciConsoleStatus & PCI_CONSOLE_READ_STATUS_MASK;
	if(status) {
	    *c = (WP_U8)*pciConsoleReadData;
	    *pciConsoleStatus |= PCI_CONSOLE_READ_STATUS_MASK;
	    return 0;
	}
	if(block == 0) return -1;
    } while(1);
    
    return 0;
}
#endif
#endif

