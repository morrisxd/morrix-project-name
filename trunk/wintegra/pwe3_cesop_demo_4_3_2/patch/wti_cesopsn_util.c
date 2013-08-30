/*****************************************************************************
 * (C) Copyright PMC - WIN division (Wintegra) 20181.  All rights reserved.
 * PMC-Sierra (WINTEGRA) CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of PMC-Sierra (Wintegra).
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/
 
/****************************************************************************
 *
 * Example: wti_cesopsn_util.c
 *
 * provide CESoPSN system utility functions.
 *
 ****************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "wp_wddi.h"

#include "wpx_enet_phy.h"
#include "wpx_pin_mux_selector.h"
#include "wt_util.h"

#ifdef WP_HW_WINPATH3
#include "wpx_pinmux.h"
#endif

#if !defined(WTI_UFE_UTIL_OLD_H) && !defined(WTI_CESOP_TDI)
#include "wti_ufe_util.h"
#elif defined(WTI_UFE_UTIL_OLD_H)
#define WT_OCTAL WTI_OCTAL
#define sbi_spe_line_type sbi_spe_transfer
#define WT_UfeSystemCreate WT_UfeSysCreateEnable
#define WT_UfeRelease WT_UfeRelease
#endif

#include "wti_cesopsn_util.h"
#include "wti_cesopsn_defines.h"

#include "wpx_tdm_comet.h"

#ifdef WP_HW_WINPATH2
#undef N_ENET_TX_CHANNELS
#undef N_ENET_RX_CHANNELS
#define N_ENET_TX_CHANNELS 8
#define N_ENET_RX_CHANNELS 8
#endif

#define WTI_TDM2PSN_LEN_MASK        0x003f0000
#define WTI_TDM2PSN_LRM_MASK        0x0f000000
#define WTI_TDM2PSN_CAS_PACKET_LRM  0x03000000
#define WTI_TDM2PSN_DATA_PACKET_LRM 0x00000000

#define  WTI_INITIALIZE_FIRMWARE 1        /* setting to '1' allows programming configuration through the host */

#if WTI_CESOP_TDI
static void WTI_TdmPortConfig(WP_port_tdm *cfg_ptr, WP_U32 port_index);
static void WTI_TransPortOverTdmAlloc(WP_U32 port_index);
static void WTI_TransPortOverTdmCreate(WP_U32 port_index);
static void WTI_TdiTransCesDeviceConfig(WP_tdm_binding *trans_binding,
                                        WP_device_tdm_trans_ces *tdm_ces_specific,
                                        WP_device_tdm_trans *trans_device_config,
                                        WP_U32 device_index);
static void WTI_TdiTransDeviceAlloc(int port_index);
static void WTI_TdiTransDeviceCreate(int port_index);
static void WTI_SystemReleaseTdi(void);
#else
static void WTI_TransDeviceCreate(int pw_index,
                                  int rx_clock_master_flag,
                                  WP_handle emphy_port,
                                  WT_ufe *ufe,
                                  int line_index);
static void WTI_SystemReleaseUfe(void);
static void WTI_TransDeviceDisable(WP_U32 index);
static void WTI_TransDeviceDelete(WP_U32 index);
#endif

static void WTI_Tdm2PsnFlowCreate(int pw_index, int oobc_mode);
static void WTI_Psn2TdmFlowCreate(int pw_index, int oobc_mode, int clock_master);
static void WTI_Tdm2PsnFlowDelete(WP_U32 index);
static void WTI_Psn2TdmFlowDelete(WP_U32 index);

void WTI_SyncEthConfig(WP_sync_eth_config *cfg_ptr);
static void WTI_SyncEthInit(void);
static void WTI_BoardConfigure(void);

#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
static void WTI_ClockRecInitialParamsSet(WP_clock_rec_params *clock_rec_params);
#if !defined(CHECKIN_TEST)
#ifdef WP_HW_WINPATH3
#if defined __WT_UFE448__ || defined __WT_UFE412__
static void WTI_ZarlinkPllSetup(WPX_pll_cfg *pllConfigParam);
#endif //defined __WT_UFE448__ || defined __WT_UFE412__
#endif
#endif//!defined(CHECKIN_TEST)
#if defined __WT_UFE412__ || defined __WT_UFE448__
static void WTI_ClockRecoveryPsn2TdmDefaultParamesConfigUfe4(WP_U32 line_index, WP_U32 cr_operation_mode);
#else
static void WTI_ClockRecoveryPSN2TDMDefultParamesConfig(WP_U32 line_index);
#endif /* __WT_UFE412__ || defined __WT_UFE448__ */
#endif /* WTI_CESOP_CLOCK_RECOVERY_ENABLE */

#if WTI_CES_SHAPING_ENABLED
static void WTI_SchedulerCreate(void);
#endif

#if (WTI_PTP_ENABLE == 1)
void    WPE_PTPConfig(void);
void    WPE_PTPInit(void);
void    WPE_CreateIWBridgeSystem(void);
void    WPE_CreateIWQnode(void);
void    WPE_CreateBufferPools(void);
void    WPE_CreateEnetPortDevice(void);
void    WPE_CreateHostPortDevice(void);
void    WPE_EnetPhyInit(WP_U8 wti_enet_port,WP_U8 enet_mode);
void    WPE_PTPModuleInitWP3(void);
void    WPE_CreatePTPPorts(void);
void    WPE_EnableSystem(void);
void    WPE_PTPTimestampInsertionEnable(void);
void    WPE_CreateEnetRxTxChannel(void);
void    WPE_CreateHostIwRxChannel(void);
void    WPE_CreatePTPPorts(void);
void    WPE_CreateHostTermFlowAgg(void);
WP_status WPE_RECEIVE_PTP_PACKET(WP_U16 * sec_msb,WP_U32 * sec_lsb,WP_U32 * nsec,WP_U8 *temp_buffer);
void    WPE_SEND_PTP_PACKET(WP_handle ptp_port_handle , WP_U8 cd_id ,  WP_U8  ptp_pktid ,WP_U8 loopback_enable);
void    WPE_CreateIwBportHost(void);
void    terminate(WP_CHAR *s,WP_U32 line);
void    App_TranslateAsciiToHex(WP_CHAR *Hex,WP_CHAR *Ascii,WP_U32 length);
void    WPE_CreateIwBportEnet(void);
void    WPE_CreateEnetRxTxBinding(void);
void    WPE_AddBasicFilters(void);
void    WPE_AddBasicRules(void);
static void WPE_RUN_PKTS(void);
void    WTE_DisplayStats(void);
void    WPE_HostTermFlowAggStatistics(WP_handle flow_agg_handle);
void    WPE_IwPortStatistics(WP_handle iw_port);
void    WPE_PtpPortStatistics(WP_handle ptp_port);

#define WPE_MAX_CD             1

#define WTE_STATS                0   /* set to see stats */
#define CHECK_TS                 1    /* remove ts checks for faster test for debug purposes only */
#define WTI_ALLOWED_TS_DIFF     (10000)  /* nanosecs */

#define WPE_PACKETS_PRINT        1
#define WPE_PTP_CHECKSUM_ENABLE_DISABLE WP_PTP_CHECKSUM_DISABLE   /* not supported */
#define WPE_PTP_LOOPBACK_ENABLE_DISABLE WP_PTP_LOOPBACK_ENABLE //ENABLE
#define WPE_PTP_STATS_ENABLE_DISABLE  WP_PTP_STATMODE_ENABLE//ENABLE
#define HOST_DATA_BUFFER_SIZE    144  /* Must be >= hosst dend DATA_LENGTH*/
/* Unimportant Defines*/
#define BRIDGE_PORT_ENET_TAG        10
#define BRIDGE_PORT_HOST_TAG        20
#define VLAN_TAG_1                  1
#define VLAN_TAG_2                  2
#define MTU_SIZE                 1536
#define SDU_SIZE                 2048 /* Must be > MTU_SIZE + 48*/
/* DFC*/
#define NUM_OF_EMC_FILTERS          2
#define NUM_OF_RULES_PER_FILTER     5
#define WP_IW_RULE_MAX_LENGTH       300

#define MAX_IW_FLOWS             (8)
#define NUM_OF_IW_BPORTS         (8) /* */
#define MAXT_SIZE                255
#define SDU_SIZE                 2048 /* Must be > MTU_SIZE + 48*/
#ifdef WP_BOARD_WDS3_SL
#define WTI_ENET_PORT1           WP_PORT_ENET1      /* ENET port */
#else
#define WTI_ENET_PORT1           WP_PORT_ENET14      /* ENET port */
#endif
#define WPE_NUM_ENET_PORTS       1

#define WTI_WINNET_TYPE                             WP_ENET_SGMII_1000
#define WTI_XGI_TYPE                                WP_ENET_XAUI
#define WPE_MAX_PTP_PORTS      8

/*available ports*/
#define WPE_CD0    0

#define PTP_PROTOCOL_OFFSET 42
#define WPE_PTP_CLOCK_SOURCE        WP_PTP_SYSTEM_CLK
#define WPE_PTP_TS_SOURCE           WP_PTP_TS_SOURCE_DCO  /* _DCO or _EXTERNAL - must connect input clock*/

#define PTP_ENABLE  1   /* put 0 to see if problem is with PTP code */
#define NUM_OF_PACKETS 1
#define WPE_TX_TS_OFFSET   (76)  /* ETH 14 + IP 20 + UDP 8 + 34 PTP header */

#define WPE_PTP_CLOCK_FREQ          125000000  /* set if non system clock */


WP_handle  port_enet[1], dev_enet[1], bport_enet[1],rx_enet_channel[1], tx_enet_channel[1];
WP_handle  port_host, dev_host, bport_host, rx_host_channel;
WP_handle ptp_master_1;
WP_handle ptp_master_flow_agg_1;
WP_U8     sent_buffer[2000];
WP_handle  ptp_cds[WPE_MAX_CD] ;
WP_handle  emc_filter_handle[NUM_OF_EMC_FILTERS];
WP_handle  flow_rule_handle[NUM_OF_EMC_FILTERS+1][NUM_OF_RULES_PER_FILTER];
WP_handle  iw_sys;
WP_U32     wpid =  WP_WINPATH(DEFAULT_WPID);
WP_handle  iw_qnode,pool_host ;
WP_U32  clock_frequency =0;
WP_U32 pkt_id = 1;

#include "../93_ptp/wti_ptp_utils_wp3.h"

#endif

#if !WTI_CESOP_TDI
#if defined __WT_UFE412__ || defined __WT_UFE448__
void WTI_SetDefaultE1UnfTestParameters(WP_U32 num_of_lines, WP_U32 interface,
                                       WUFE_line_transf_type *transfer_type,
                                       WUFE_un_framed *framing_mode, WUFE_cas_enable *cas_mode,
                                       WP_U32 *num_slots
#ifdef WT_UFE_FRAMER
                                       ,WP_U8 *flexmux_mode
#endif
   );
void WTI_SetDefaultT1UnfTestParameters(WP_U32 num_of_lines, WP_U32 interface,
                                       WUFE_line_transf_type *transfer_type,
                                       WUFE_un_framed *framing_mode, WUFE_cas_enable *cas_mode,
                                       WP_U32 *num_slots
#ifdef WT_UFE_FRAMER
                                       ,WP_U8 *flexmux_mode
#endif
   );
void WTI_SetDefaultE1FrmTestParameters(WP_U32 num_of_lines, WP_U32 interface,
                                       WUFE_line_transf_type *transfer_type,
                                       WUFE_un_framed *framing_mode, WUFE_cas_enable *cas_mode,
                                       WP_U32 *num_slots
#ifdef WT_UFE_FRAMER
                                       ,WP_U8 *flexmux_mode
#endif
   );
void WTI_SetDefaultT1FrmTestParameters(WP_U32 num_of_lines, WP_U32 interface,
                                       WUFE_line_transf_type *transfer_type,
                                       WUFE_un_framed *framing_mode, WUFE_cas_enable *cas_mode,
                                       WP_U32 *num_slots
#ifdef WT_UFE_FRAMER
                                       ,WP_U8 *flexmux_mode
#endif
   );
#endif /* WT_UFE4 */
#endif /* WTI_CESOP_TDI */

/*global variable - is set in clock recovery reconfiguration*/
WP_U8 clock_recovery_reconfig = 0;

void debug_wddi_log(WP_S32 level, const WP_CHAR *string)
{
   if (level == 0)
      printf("%s\n\n", string);
}

#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
void WTI_ClockRecEvents(WP_tag tag, WP_U32 event, WP_U32 info);
#endif
void WTI_cesop_event_function(WP_tag tag, WP_U32 data, WP_U32 info);

/*****************************************************************************
 * Function name: WTI_CesopSystemStart
 * Description  : create empty CESoPSN system with no PW's
 * Input  params: none
 * Output params:
 * Return val   : none
 *****************************************************************************/
void WTI_CesopSystemStart(void)
{
   WP_status status = WP_OK;

   WT_Start(__FILE__, NULL, __FILE__);

   /* initialize WDDI */
   status = WP_DriverInit();
   WTI_TerminateOnError(status, "WP_DriverInit", __LINE__);
#if defined __WT_UFE412__ || defined __WT_UFE448__
   WPX_Ufe412CpldInterruptMaskSet(0, WPX_FPGA_INTR_ALL_MASKED);
#endif
   /* clear structures and handles */
   WTI_SystemInit();

   /* Configure WDS board and Zarlink PLL */
   WTI_BoardConfigure();

  /* create Qnodes, create enet transparent and host ports, init UFE */
   WTI_SystemSetup();

#if WTI_PCE_CLASSIFIER
   extern WP_pce_parser_info pce_parser_info;

   memset(&pce_init, 0, sizeof(WP_module_pce_init));

   pce_init.parser_info = &pce_parser_info;
   pce_init.hardware_rules_num = WTI_MAX_PW;
   pce_init.filters_num = 2;
   pce_init.pkt_rx_channel_interfaces_num = 2;
   pce_init.iw_tx_channel_interfaces_num = 2;
   pce_init.pce_discard_mode = WP_PCE_DISCARD_MODE_ALL;
   pce_init.bridge_loop_prevention_mode = WP_ENABLE;
   pce_init.learning_mode = WP_DISABLE;

   status = WP_ModuleInit(WP_SYSHANDLE(DEFAULT_WPID), WP_WDDI_MODULE_PCE, &pce_init);
   WTI_TerminateOnError(status, "WPI_PceModuleInit()", __LINE__);
#endif

#if WP_DEBUG_ENABLE_WDDI_LOG
   status = WP_ControlRegister(WP_DEBUG_CALLBACK_WDDI_LOG, debug_wddi_log);
   WTI_TerminateOnError(status, "WP_ControlRegister WPI_LOG", __LINE__);
#endif

#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
   /* Register clock recovery events */
   status = WP_ControlRegister(WP_EVENT_CLOCK_REC, WTI_ClockRecEvents);
   WTI_TerminateOnError(status, "WP_ControlRegister CR EVENTS", __LINE__);
#endif


   status = WP_ControlRegister(WP_EVENT_POLLED_RX_INDICATE, WTI_cesop_event_function);
   WTI_TerminateOnError(status, "WP_ControlRegister", __LINE__);

#if WTI_CI_TESTING_MODE_TDI || WTI_CI_TESTING_MODE_UFE4
   WTI_PTPModuleInit(); // PTP clock for testing different frequencies
#endif

#if WTI_PTP_ENABLE
   WPE_PTPInit();
#endif


   /***********************/
   /* System Commit Stage */
   /***********************/
   status = WP_SysCommit();
   WTI_TerminateOnError(status, "WP_SysCommit", __LINE__);

   WT_Identify();

#if WTI_PTP_ENABLE
   WPE_PTPConfig();
#endif

#if WTI_CES_SHAPING_ENABLED
   /* enable scheduler */
   status = WP_SysPsuEnable(WP_WINPATH(DEFAULT_WPID));
#endif

#if WTI_CI_TESTING_MODE_TDI || WTI_CI_TESTING_MODE_UFE4
   WTI_PTPClockSet(global_clock_rec_diff_ref_clock);
#endif


   /* Init SyncE Clocks */
   if (the_system->system_features.sync_ethernet)
   {
      WTI_SyncEthInit();
   }

#if !WTI_CESOP_TDI
   /* in WDS3 the UFE must be initialized after the UPI port create and enable !!! */
   status = WP_PortEnable(the_system->trans_port, WP_DIRECTION_DUPLEX);
   WTI_TerminateOnError(status, "WP_PortEnable() trans_port",__LINE__);

#if WTI_DUAL_EMPHY
   status = WP_PortEnable(the_system->second_trans_port, WP_DIRECTION_DUPLEX);
   WTI_TerminateOnError(status, "WP_PortEnable() second trans_port",__LINE__);
#endif

#if defined __WT_UFE412__ || defined __WT_UFE448__
#if WTI_DUAL_EMPHY
   WTI_UfeInit(&the_system->ufe, 0, 0);
   WTI_UfeInit(&the_system->second_ufe, 2, 1);
#else
   if (WTI_EMPHY_PORT == WP_PORT_UPI1)
      WTI_UfeInit(&the_system->ufe, 0, 0);
   else if (WTI_EMPHY_PORT == WP_PORT_UPI2)
      WTI_UfeInit(&the_system->ufe, 1, 0);
   else
      WTI_UfeInit(&the_system->ufe, 2, 0);
#endif
#endif
   WT_UfeSystemEnable(&the_system->ufe);
#if WTI_DUAL_EMPHY
   WT_UfeSystemEnable(&the_system->second_ufe);
#endif
#endif /* !WTI_CESOP_TDI */

   /* create IW systems, create deafult classification rule (LPM classification only),
      create Rx and Tx binding for enet channels */
   WTI_IwSystemSetup();

   /* enable transparent and enet ports */
   WTI_PortsEnable();
}

/*****************************************************************************
 * Function name: WTI_SystemAlloc
 * Description  : allocate CESoP system
 * Input  params: none
 * Output params:
 * Return val   : none
 *****************************************************************************/
void WTI_SystemAlloc(void)
{
   if (the_system == 0)
   {
      the_system = (WTI_system *)malloc(sizeof(WTI_system));
      if (!the_system)
      {
         printf("Test Abort. Can't allocate memory for the system\n");
         printf("Test Failed.\n");
         WP_DriverRelease();
         WTI_Reboot();
      }
   }

   memset(the_system, 0, sizeof(WTI_system));
}

#if WTI_CESOP_MPLS_IW
/*****************************************************************************
 * Function name: WTI_MplsLayerAggDefCfg
 * Description  : default configuration of the L2 router MPLS flow aggregation
 * Input  params: none
 * Output params:
 * Return val   : none
 *****************************************************************************/
static void WTI_MplsLayerAggDefCfg(WP_iw_agg_mpls_l2_router *mpls_layer2_agg_cfg)
{
   memset(mpls_layer2_agg_cfg,0,sizeof(WP_iw_agg_mpls_l2_router));

   mpls_layer2_agg_cfg->tag = 0;
   mpls_layer2_agg_cfg->txfunc = WTI_INVALID_HANDLE;
   mpls_layer2_agg_cfg->iw_port = WP_IW_IGNORE_IW_PORT;
   mpls_layer2_agg_cfg->flow_agg_type = WP_IW_AGG_PRIMARY;
   mpls_layer2_agg_cfg->interruptqueue = WP_IW_IRQT1;
   mpls_layer2_agg_cfg->error_pkt_mode = WP_DISABLE;
   mpls_layer2_agg_cfg->intmode = WP_DISABLE;
   mpls_layer2_agg_cfg->statmode = WP_ENABLE;
   mpls_layer2_agg_cfg->timestamp_mode = WP_DISABLE;
   mpls_layer2_agg_cfg->ov_pool_mode = WP_IW_OV_POOL_DISABLE;
   mpls_layer2_agg_cfg->fbp_drop_threshold = 0;
   mpls_layer2_agg_cfg->mtu = 1536;
   mpls_layer2_agg_cfg->multi_cong_prof = WP_DISABLE;
   mpls_layer2_agg_cfg->policer_handle = 0;
   mpls_layer2_agg_cfg->l2_header_insert_mode = 0;
   mpls_layer2_agg_cfg->label_mode = 0;
   mpls_layer2_agg_cfg->num_of_mpls_headers_to_push = 0;
   mpls_layer2_agg_cfg->num_of_mpls_headers_to_pop = 0;
   mpls_layer2_agg_cfg->swap_label = 0;
   mpls_layer2_agg_cfg->ttl_decrement = WP_MPLS_TTL_DECR_DISABLE;
   mpls_layer2_agg_cfg->ttl_mode = WP_DISABLE;

}
#endif /* WTI_CESOP_TO_MPLS_FA_CASCADING */

/*****************************************************************************
 * Function name: WTI_SystemFeaturesSelect
 * Description  : Select relevant system features , new features defines should appear here
 * Input  params: none
 * Output params:
 * Return val   : none
 *****************************************************************************/
void WTI_SystemFeaturesSelect(void)
{

   /*********************************** SyncE *************************************/
   /*  Add option to set the relevant features on run time TBD */
#if WTI_SYNC_ETH_ENABLE
   the_system->system_features.sync_ethernet = WP_ENABLE;
#else /* NOT WTI_SYNC_ETH_ENABLE */
   the_system->system_features.sync_ethernet = WP_DISABLE;
#endif /* NOT WTI_SYNC_ETH_ENABLE */

#if WTI_SYNC_ETH_MASTER_ENABLE
   the_system->system_features.sync_ethernet_master = WP_ENABLE;
#else /* NOT WTI_SYNC_ETH_MASTER_ENABLE */
   the_system->system_features.sync_ethernet_master = WP_DISABLE;
#endif  /* NOT WTI_SYNC_ETH_MASTER_ENABLE */

#ifdef WP_HW_WINPATH2

   if (the_system->system_features.sync_ethernet)
   {
      WTI_TerminateOnError(1, "ERROR - WTI_SystemFeaturesSelect -SyncE is not supported in WP2", __LINE__);
   }
#endif /* WP_HW_WINPATH2 */

   if ((!the_system->system_features.sync_ethernet)&&(the_system->system_features.sync_ethernet_master))
   {
       WTI_TerminateOnError(1, "ERROR - WTI_SystemFeaturesSelect - can't support sync_ethernet_master without sync_ethernet", __LINE__);
   }

  /*********************************** End of SyncE  *************************************/
}


/*****************************************************************************
 * Function name: WTI_SystemInit
 * Description  : clear global structures and init system handles
 * Input  params: none
 * Output params:
 * Return val   : none
 *****************************************************************************/
void WTI_SystemInit(void)
{
   WP_U32 ii;

   if (!the_system)
   {
      printf("Test Abort. The system is not allocated\n");
      exit(1);
   }

   /* Select the relevant system features */
   WTI_SystemFeaturesSelect();

   the_system->wpid = WP_WINPATH(DEFAULT_WPID);

   for (ii=0; ii<N_QNODES; ii++)
   {
      the_system->qnode[ii].adjunct_pool = WTI_INVALID_HANDLE;
      the_system->qnode[ii].handle = WTI_INVALID_HANDLE;
   }

   the_system->qnode_config[WTI_QNODE_HOST].num_buffers = N_HOST_QUEUE_BUFFERS;
   the_system->qnode_config[WTI_QNODE_HOST].buffer_size = N_HOST_QUEUE_BUFFER_SIZE;

   the_system->qnode_config[WTI_QNODE_IW_PSN2TDM].num_buffers = N_IW_PSN2TDM_QUEUE_BUFFERS;
   the_system->qnode_config[WTI_QNODE_IW_PSN2TDM].buffer_size = N_IW_PSN2TDM_QUEUE_BUFFER_SIZE;

   the_system->qnode_config[WTI_QNODE_IW_TDM2PSN].num_buffers = N_IW_TDM2PSN_QUEUE_BUFFERS;
   the_system->qnode_config[WTI_QNODE_IW_TDM2PSN].buffer_size = N_IW_TDM2PSN_QUEUE_BUFFER_SIZE;

   the_system->qnode_config[WTI_QNODE_TRANS_TX].num_buffers = N_TRANS_TX_QUEUE_BUFFERS;
   the_system->qnode_config[WTI_QNODE_TRANS_TX].buffer_size = N_IW_PSN2TDM_QUEUE_BUFFER_SIZE;

#if WTI_CESOP_TDI
   for (ii=0; ii<WTI_TDM_PORTS; ii++)
   {
      the_system->tdm_port[ii] = WTI_INVALID_HANDLE;
      the_system->tdm_trans_dev[ii] = WTI_INVALID_HANDLE;
   }
#else
   the_system->trans_port = WTI_INVALID_HANDLE;
   memset(&upi_trans_device,0,sizeof(WP_device_emphy_trans));
#endif

#if (WTI_CESOP_MPLS_IW && !WTI_CESOP_MPLS_OVER_ENET)
   the_system->hspi_port = WTI_INVALID_HANDLE;
   the_system->hspi_device = WTI_INVALID_HANDLE;
#else
   the_system->enet_port = WTI_INVALID_HANDLE;
   the_system->enet_device = WTI_INVALID_HANDLE;
   the_system->rx_enet_channel = WTI_INVALID_HANDLE;
   the_system->tx_enet_channel = WTI_INVALID_HANDLE;
#if WTI_EXT_DIFF_MODE_ACTIVE
   the_system->tx_enet_channel_high_priority = WTI_INVALID_HANDLE;
#endif
#if (WTI_2_ENET_DEVICES)
   the_system->enet_port1 = WTI_INVALID_HANDLE;
   the_system->enet_device1 = WTI_INVALID_HANDLE;
   the_system->rx_enet_channel1 = WTI_INVALID_HANDLE;
   the_system->tx_enet_channel1 = WTI_INVALID_HANDLE;
#if WTI_EXT_DIFF_MODE_ACTIVE
   the_system->tx_enet_channel1_high_priority = WTI_INVALID_HANDLE;
#endif
#endif
#if (WTI_4_ENET_DEVICES)
   the_system->enet_port2 = WTI_INVALID_HANDLE;
   the_system->enet_device2 = WTI_INVALID_HANDLE;
   the_system->rx_enet_channel2 = WTI_INVALID_HANDLE;
   the_system->tx_enet_channel2 = WTI_INVALID_HANDLE;
#if WTI_EXT_DIFF_MODE_ACTIVE
   the_system->tx_enet_channel2_high_priority = WTI_INVALID_HANDLE;
#endif
   the_system->enet_port3 = WTI_INVALID_HANDLE;
   the_system->enet_device3 = WTI_INVALID_HANDLE;
   the_system->rx_enet_channel3 = WTI_INVALID_HANDLE;
   the_system->tx_enet_channel3 = WTI_INVALID_HANDLE;
#if WTI_EXT_DIFF_MODE_ACTIVE
   the_system->tx_enet_channel3_high_priority = WTI_INVALID_HANDLE;
#endif
#endif
#endif
   the_system->iw_host_port = WTI_INVALID_HANDLE;
   the_system->iw_host_device = WTI_INVALID_HANDLE;
   for (ii=0; ii<N_HOST_RX_CHANNELS; ii++)
   {
      the_system->rx_host_channels[ii] = WTI_INVALID_HANDLE;
   }
   the_system->violation_flow_agg= WTI_INVALID_HANDLE;
   the_system->tdm2psn_iw_system = WTI_INVALID_HANDLE;
   the_system->psn2tdm_iw_system = WTI_INVALID_HANDLE;
#if WTI_PCE_CLASSIFIER
   the_system->pce_filter_handle = WTI_INVALID_HANDLE;
#else
   the_system->dfc_filter_handle = WTI_INVALID_HANDLE;
#endif
   for (ii=0; ii<WTI_MAX_PW; ii++)
   {
      the_system->pw[ii].trans_device = WTI_INVALID_HANDLE;
      the_system->pw[ii].trans_rx = WTI_INVALID_HANDLE;
      the_system->pw[ii].trans_tx = WTI_INVALID_HANDLE;
      the_system->pw[ii].tdm2psn_flow_agg = WTI_INVALID_HANDLE;
      the_system->pw[ii].psn2tdm_flow_agg = WTI_INVALID_HANDLE;
      the_system->pw[ii].state = 0;
   }
   memset(&trans_ch_config,0,sizeof(WP_ch_trans_pwe3));
   memset(&trans_rx_binding_config,0,sizeof(WP_rx_binding_directmap));
   memset(&cesop_tx_binding_config,0,sizeof(WP_iw_tx_binding_cesop));
   memset(&pw_config ,0,sizeof(WTI_pw_params));
   memset(&tdm2psn_flow_agg_config ,0,sizeof(WP_iw_agg_cesop));
   memset(&psn2tdm_flow_agg_config ,0,sizeof(WP_iw_agg_cesop));
   memset(&violation_config,0,sizeof(WP_iw_agg_hq));
#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
   memset(the_system->clock_rec,
          0,
          WTI_CESOP_MAX_CLOCK_REC_INTERFACES * sizeof(WP_clock_rec_params));
   WTI_ClockRecInitialParamsSet(the_system->clock_rec);
#endif

#if WTI_CESOP_MPLS_IW
   WTI_MplsLayerAggDefCfg(&mpls_layer2_agg_cfg);
#endif /* WTI_CESOP_TO_MPLS_FA_CASCADING */

#if WTI_CESOP_MEF8_ENCAP
   WTI_FlowAggregationMef8Tdm2PsnConfigInit(&pw_config.tdm2psn_flow_agg_config);
   WTI_FlowAggregationMef8Psn2TdmConfigInit(&pw_config.psn2tdm_flow_agg_config);
#else /* WTI_CESOP_MEF8_ENCAP */
   WTI_FlowAggregationTdm2PsnConfigInit(&pw_config.tdm2psn_flow_agg_config);
   WTI_FlowAggregationPsn2TdmConfigInit(&pw_config.psn2tdm_flow_agg_config);
#endif

}

/*****************************************************************************
 * Function name: WTI_SystemRelease
 * Description  : releases CESoP and UFE resources
 * Input  params: none
 * Output params:
 * Return val   : none
 *****************************************************************************/
void WTI_SystemRelease(void)
{
#if WTI_CESOP_TDI
   WTI_SystemReleaseTdi();
#else
   WTI_SystemReleaseUfe();
#endif
}

#if WTI_CESOP_TDI
/*****************************************************************************
 * Function name: WTI_TdiTransDeviceDelete
 * Description  : disable and delete transparent device
 * Input  params: index - Tdi Device index
 * Output params:
 * Return val   : none
 *****************************************************************************/
void WTI_TdiTransDeviceDelete(WP_U32 index)
{
   WP_status status;

   status = WP_DeviceDisable(the_system->tdm_trans_dev[index], WP_DIRECTION_DUPLEX);
   WTI_TerminateOnError(status, "WP_DeviceDisable", __LINE__);
   status = WP_DeviceDelete(the_system->tdm_trans_dev[index]);
   WTI_TerminateOnError(status, "WP_DeviceDelete", __LINE__);
}

/*****************************************************************************
 * Function name: WTI_SystemReleaseUfe
 * Description  : releases CESoP and TDI resources
 * Input  params: none
 * Output params:
 * Return val   : none
 *****************************************************************************/
static void WTI_SystemReleaseTdi(void)
{
   WP_U32 i;
   WP_status status;

   for (i=0;i<WTI_MAX_PW;++i)
   {
      /* disable all active PW's */
      if (the_system->pw[i].state & WTI_PW_ACTIVE)
      {
         WTI_PwDisable(i);
      }

      /* delete all PW's */
      if (the_system->pw[i].state & WTI_PW_CREATED)
      {
         WTI_PwDelete(i);
      }
   }

   for (i=0;i<WTI_ACTIVE_TDI_PORTS;++i)
   {
      WTI_TdiTransDeviceDelete(i);
      status = WP_PortDisable(the_system->tdm_port[i], WP_DIRECTION_DUPLEX);
      WTI_TerminateOnError(status, "WP_PortDisable", __LINE__);
   }
}

#else
/*****************************************************************************
 * Function name: WTI_SystemReleaseUfe
 * Description  : releases CESoP and UFE resources
 * Input  params: none
 * Output params:
 * Return val   : none
 *****************************************************************************/
static void WTI_SystemReleaseUfe(void)
{
   WP_U32 i,j;
   WP_U32 e1_lines_per_spe = 21;
   WP_U32 t1_lines_per_spe = 28;
   WP_status status;

#ifdef WT_UFE_FRAMER
   WPX_FRMR_LowLevelCommsDisableAccess(0);
   WPL_ThreadDestroy(&alarm_and_pm_thread_id);
   WPL_ThreadDestroy(&mailbox_thread_id);
#endif

   for (i=0;i<WTI_MAX_PW;++i)
   {
      /* disable all active PW's */
      if (the_system->pw[i].state & WTI_PW_ACTIVE)
      {
         WTI_PwDisable(i);
      }

      /* delete all PW's */
      if (the_system->pw[i].state & WTI_PW_CREATED)
      {
         WTI_PwDelete(i);
      }
      /* PW not created but UFE phy is */
      else if (the_system->pw[i].state & WTI_PW_UFE2_PHY_CREATED)
      {
         WT_UfePhyDelete(&the_system->ufe, i);
      }
   }

   /* delete IW systems */
   if (the_system->tdm2psn_iw_system != WTI_INVALID_HANDLE)
   {
      status = WP_IwSystemDelete(the_system->tdm2psn_iw_system);
      WTI_TerminateOnError(status, "WP_IwSystemDelete tdm2psn system",__LINE__);
   }

   if (the_system->psn2tdm_iw_system != WTI_INVALID_HANDLE)
   {
      if (the_system->rx_enet_channel)
      {
         WP_rx_binding_mpls rx_binding_mpls;
         memset(&rx_binding_mpls, 0, sizeof(WP_rx_binding_mpls));

         status = WP_IwRxBindingModify(the_system->rx_enet_channel, the_system->psn2tdm_iw_system,
                                       the_system->qnode[WTI_QNODE_IW_PSN2TDM].handle,
                                       WP_IW_RX_BIND_MOD_DISABLE, &rx_binding_mpls);
         WTI_TerminateOnError(status, "WP_IwRxBindingModify psn2tdm rx_enet_channel",__LINE__);
      }
      status = WP_IwSystemDelete(the_system->psn2tdm_iw_system);
      WTI_TerminateOnError(status, "WP_IwSystemDelete psn2tdm system",__LINE__);
   }

#if 0
#ifdef WT_UFE_FRAMER
   WPX_FRMR_LowLevelCommsDisableAccess(0);
#endif
#endif

   /* delete UFE lines */
   if (the_system->ufe.fpga_mode == WT_OCTAL)
   {
      for (i=0;i<N_ACTIVE_UFE_OCT_LINES;++i)
      {
         WT_UfeLineDelete(&the_system->ufe, i);
      }
   }
   else
   {
      for (i=0;i<N_ACTIVE_UFE_SBI_SPE;++i)
      {
         if (the_system->ufe.sbi_spe_line_type[i] == WUFE_SBI_TYPE_T1)/* T1 */
         {
            for (j=(i*t1_lines_per_spe);j<((i+1)*t1_lines_per_spe);++j)
            {
               WT_UfeLineDelete(&the_system->ufe, j);
            }
         }
         else /* E1 */
         {
            for (j=(i*e1_lines_per_spe);j<((i+1)*e1_lines_per_spe);++j)
            {
               WT_UfeLineDelete(&the_system->ufe, j);
            }
         }
      }
   }

   /* release UFE */
   WT_UfeRelease(&the_system->ufe);

   /* free CESOP system */
   free(the_system);
}

#endif

/***************************************************************
 * Func name  :
 * Description: Control the TS period
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void WTI_SetTimestampPrescaler(void)
{
   WP_feature_ts_prescaler prescaler;
   WP_status status;

   memset (&prescaler, 0, sizeof (WP_feature_ts_prescaler));
   prescaler.divisor = WTI_TS_DIVISOR_PRESCALER ;

   status = WP_FeatureInit (WP_SYSHANDLE(the_system->wpid), WP_FEATURE_TS_PRESCALER, &prescaler);
   WTI_TerminateOnError(status, "WP_FeatureInit Timestamp prescaler",__LINE__);
}

/***************************************************************
 * Func name  :
 * Description: Set the selected timestamp clock source for WinGine external TS source (timestamp2)
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void WTI_SetExternalWingineTimestampSource(WP_U8 clk_src)
{
   WP_feature_external_wingine_ts_params wg_ts2_source;
   WP_status status;

   if (clk_src > WP_CLK_SRC_INT_SYS)
   {
      printf("WARNING! WinGine timestamp source clock uninitilized. Set to Default system clock!\n");
      clk_src = WP_CLK_SRC_INT_SYS;
   }

   memset (&wg_ts2_source, 0, sizeof (WP_feature_external_wingine_ts_params));
   wg_ts2_source.ts2_clock_source = clk_src; /* internal: default */

   status = WP_FeatureInit (WP_SYSHANDLE(the_system->wpid), WP_FEATURE_EXT_WINGINE_TS, &wg_ts2_source);
   WTI_TerminateOnError(status, "WP_FeatureInit External WG Timestamp source",__LINE__);
}


/*****************************************************************************
 * Function name: WTI_SystemSetup
 * Description  : create Qnodes, ports and initialize UFE
 * Input  params: none
 * Output params:
 * Return val   : none
 *****************************************************************************/
void WTI_SystemSetup(void)
{
   WP_status status;
   WP_feature_iw_cesop cesop_feature_cfg ={0};
#if (WTI_CESOP_CLOCK_RECOVERY_ENABLE || WTI_CESOP_RX_TIMING_ENABLE)
   WP_clock_rec_diff_ext diff_ext_cfg = {0};
#endif
#if WTI_CES_SHAPING_ENABLED
   WP_clock_rec_ces_shaping ces_shaping_cfg={0};
#endif
#if WTI_CESOP_TDI
   WP_U32 i;
#endif

#ifdef WP_HW_WINPATH3
#if (WTI_CESOP_CLOCK_RECOVERY_ENABLE || WTI_CESOP_RX_TIMING_ENABLE)
#if WTI_CESOP_TDI
/* This change is required in WP3 PWE3-CES TDI when CR is in use
   The code decrease the current driven by the WP3 to 4mA on the TX clock and sync of lines 7 & 15
   the current decrease is required because in the WDS3 the TX clock and sync of lines 7 & 15
   are also connected to the FPGA and create a returns which damage the falling edge of the clock
   This change is related to the WDS3 board design                                               */
   status = WPX_PinMuxTdiDriveStrength(the_system->wpid);
   WTI_TerminateOnError(status, "WPX_PinMuxTdiDriveStrength", __LINE__);
#endif  /* WTI_CESOP_TDI */



#if 0 /* workaround */
   WP_U32 * addr;
   WP_U32 temp_val;
/* needed only when working with ENET clock. */

/*
   set SERDES_CLK_OUT_0 0xbe008288
   select a clock  in serdes clk mux
*/
   addr = (WP_U32 *)WPL_PHY_VIRTUAL(WP_WINPATH(DEFAULT_WPID),0x1e008288);
   temp_val = 0x80000011;
   *(WP_U32 *) addr =  temp_val;

#endif /* 0 */
#endif  /* (WTI_CESOP_CLOCK_RECOVERY_ENABLE || WTI_CESOP_RX_TIMING_ENABLE) */
#endif  /* WP_HW_WINPATH3 */

   /* set winpath id */
   the_system->wpid = WP_WINPATH(DEFAULT_WPID);

#if WTI_PCE_CLASSIFIER
   int_ram_partition.policer_entries = 0;
   int_ram_partition.pce_hw_rules_num = 2 * WTI_MAX_PW;
   WT_SetIntRamPartition(WP_WINPATH(DEFAULT_WPID), &int_ram_partition);
#endif
#if (!WTI_CESOP_TDI)
#if defined __WT_UFE412__ || defined __WT_UFE448__
   /* Mask all interrupt sources */
   WPX_Ufe412CpldInterruptSelect(0, WPX_INTERRUPT_NONE);
#endif
#endif
   /* system init */
   status = WP_SysInit(the_system->wpid, context);
   WTI_TerminateOnError(status, "WP_SysInit", __LINE__);

#if (!WTI_CESOP_TDI)
#if defined __WT_UFE412__ || defined __WT_UFE448__

   /* FPGA HW reset */
   WPX_Ufe4HwReset(WP_WINPATH(DEFAULT_WPID), 0);  /* ufe id, connector id as arguments */

   if (WTI_INITIALIZE_FIRMWARE)
   {
      /* Announce that UFE firmware will be initialized through host and not through JTAG */
      printf("\n******************************************************\n");
      printf(" UFE build configuration is loaded through the host!!!\n");
      printf("******************************************************\n\n");
      WT_UfeInitializeFirmware(0);

#if WTI_DUAL_EMPHY

      /* FPGA HW reset */
      WPX_Ufe4HwReset(WP_WINPATH(DEFAULT_WPID), 2);  /* ufe id, connector id as arguments */

      /* This PINMUX is OK for dual EMPHY mode */
      printf("WPX_CONFIGURE_UPI3_UFE412 ************\n\n\n");
      status = WPX_BoardConfigure(the_system->wpid, WPX_CONFIGURE_UPI3_UFE412);
      WTI_TerminateOnError(status, "WPX_BoardConfigure()", __LINE__);

      status = WPX_BoardZarlinkPllConfig(the_system->wpid, WPX_PLL_CONFIG_APLL_125);
      WTI_TerminateOnError(status, "WPX_BoardZarlinkPllConfig()", __LINE__);

      /* Announce that UFE firmware will be initialized through host and not through JTAG */
      printf("\n******************************************************\n");
      printf(" UFE build configuration is loaded through the host (second UFE) !!!\n");
      printf("******************************************************\n\n");
      WT_UfeInitializeFirmware(1);

#endif
   }
   
#if 0
#if defined(WP_BOARD_WDS3)
#if !defined(__linux__)
   /* Configure UFE4 interrupt, connect handler, and enable interrupt */
   /* (1) Connect handler for UFE4 interrupts */
   int_connect(IV_EINT3_VEC, ufe4_interrupt_handler);
   /* (2) Enable the interrupt */
   WinMon_enable_int(IV_EINT3_VEC, 0);
   /* (3) Unmask UFE interrupts in CPLD */
   WPX_Ufe412CpldInterruptUnmask(0);
#endif
#endif
#endif

#endif
#endif /* (!WTI_CESOP_TDI) */

   if (the_system->system_features.sync_ethernet)
   {
      /* in SyncE mode the serdes are configured to use internal reference clock form BRG3 connected to BRGIN1 */
      /* which connected to the SyncE external PLL */
      status = WP_SysClockCreate(the_system->wpid,WP_BRG3,WP_BRG_SRC_BRGIN1, 0);

      WTI_TerminateOnError(status, "WP_SysClockCreate()",__LINE__);

      WP_Delay(1500000);
   }

   /* create queue nodes */
   WTI_QnodesCreate();

#if (WTI_CESOP_MPLS_IW && !WTI_CESOP_MPLS_OVER_ENET)
   /* create HSPI port and device*/
   WTI_HspiPortCreate();
#else
   /* create ENET port and device*/
    WTI_EnetPortCreate();
#endif

   the_system->iw_host_port = WP_PortCreate(the_system->wpid, WP_PORT_IW_HOST, NULL);
   WTI_TerminateOnError(the_system->iw_host_port, "WP_PortCreate() IW Host", __LINE__);

#if WTI_CESOP_TDI
   for (i=0;i<WTI_ACTIVE_TDI_PORTS;++i)
   {
      /* create transparent port over TDM interface */
      WTI_TransPortOverTdmAlloc(i);

      /* create transparent CES device over TDM port */
      WTI_TdiTransDeviceAlloc(i);

   }
#else
   /* init UFE */
#ifdef __WT_UFE3__
   if (WTI_EMPHY_PORT == WP_PORT_UPI1)
      WTI_UfeInit(&the_system->ufe, 0, 0);
   else if (WTI_EMPHY_PORT == WP_PORT_UPI2)
      WTI_UfeInit(&the_system->ufe, 1, 0);
   else
      WTI_UfeInit(&the_system->ufe, 2, 0);
#endif
   /* create transparent port */
   WTI_TransPortCreate();
#endif

   /* feature init */
   WTI_CesopFeatureConfig(&cesop_feature_cfg);

#if (WTI_CESOP_CLOCK_RECOVERY_ENABLE || WTI_CESOP_RX_TIMING_ENABLE)
   cesop_feature_cfg.diff_ext = &diff_ext_cfg;

#if WTI_CES_SHAPING_ENABLED
   cesop_feature_cfg.ces_shaping = &ces_shaping_cfg;
#endif

   diff_ext_cfg.ref_clock = global_clock_rec_diff_ref_clock;
#ifdef __WT_UFE3__
   diff_ext_cfg.ext_dco_clock = WT_CLOCK_REC_EXT_DIFF_DCO_REF_CLOCK;
   diff_ext_cfg.reg_dco_clock = global_clock_rec_reg_dco_ref_clock;
#else
   diff_ext_cfg.ext_dco_clock = 0; /* not in used for UFE4 */
   diff_ext_cfg.reg_dco_clock = global_clock_rec_reg_dco_ref_clock; /* Used as both XO clock and DCO clock */
#endif /* WT_UFE3 */
#endif



   /* timestamp shaping */
#if WTI_CES_SHAPING_ENABLED


#if WTI_CI_TESTING_MODE_UFE4
   ces_shaping_cfg.diff_ref_src_brgin = WP_BRG_SRC_BRGIN4; // differential clock connected , must connect sma5 to sma6
#else
   ces_shaping_cfg.diff_ref_src_brgin = WP_BRG_SRC_BRGIN1; // differential clock connected
#endif
   ces_shaping_cfg.ces_shaping_mode = WP_CES_SHAPING_MODE_PRE_IW_SHAPING;
#endif

   status = WP_FeatureInit(WP_SYSHANDLE(the_system->wpid),
                           WP_FEATURE_IW_CESOP,
                           &cesop_feature_cfg);
   WTI_TerminateOnError(status, "WP_FeatureInit CESOP", __LINE__);

#if WTI_TIMESTAMP_PRESCALER_ENABLE
#if (WTI_DEBUG_LEVEL > 0)
   /* Change TS period */
   printf("set prescaler\n");
   WTI_SetTimestampPrescaler();
#endif
#endif/* WTI_TIMESTAMP_PRESCALER_ENABLE */

#if (WTI_CESOP_CLOCK_RECOVERY_ENABLE || WTI_CESOP_RX_TIMING_ENABLE)
#ifdef WP_HW_WINPATH3
   WTI_SetExternalWingineTimestampSource(WTI_WG_TS2_CLOCK_SOURCE);
#endif /* WP_HW_WINPATH3 */
#endif




#if WTI_CES_SHAPING_ENABLED
 /* Create the PSU scheduling function */
    WTI_SchedulerCreate();
#endif

}

/*****************************************************************************
 * Function name: WTI_PTPModuleInit
 * Description  : Initializes PTP module
 *
 *
 * Input  params: none
 * Output params:
 * Return val   : none
 *****************************************************************************/
void WTI_PTPModuleInit(void)
{

   WP_ptp_system    ptp_system_params = {0};
   WP_status status = WP_OK;

   ptp_system_params.max_ptp_ports = 2 ;

   /* WDDI */
   status = WP_ModuleInit(WP_SYSHANDLE(the_system->wpid) ,WP_WDDI_MODULE_PTP, &ptp_system_params);
   WTI_TerminateOnError(status, "WP_ModuleInit(WP_WDDI_MODULE_PTP)", __LINE__);
}


/*****************************************************************************
 * Function name: WTI_PTPSetup
 * Description  : Setup PTP related objects
 *
 *
 * Input  params: none
 * Output params:
 * Return val   : none
 *****************************************************************************/
void WTI_PTPClockSet(WP_U32 frequency)
{
   WP_ptp_clock_domain      ptp_cd_descriptor={0} ;
   WP_handle cd_handle;

   ptp_cd_descriptor.clock_source_input = WP_PTP_SYSTEM_CLK;  /* WP_PTP_BRGI_0_CLK WP_PTP_SYSTEM_CLK (see WTI_BoardPinConfig)*/
   ptp_cd_descriptor.clock_source_frequency =   WPL_GetProcessorSpeed();
   ptp_cd_descriptor.clock_source_frequency =   1000*(ptp_cd_descriptor.clock_source_frequency/1000);
   ptp_cd_descriptor.clock_domain_hw_id = 2;
   ptp_cd_descriptor.clock_source_mode = WP_PTP_DCO;//WP_PTP_DIRECT
   ptp_cd_descriptor.service_clock_frequency = frequency;
   ptp_cd_descriptor.ts_source_mode = WP_PTP_TS_SOURCE_DCO;
   ptp_cd_descriptor.ts_source_frequency = (2048000*8);

   /* wddi */
   cd_handle = WP_PtpClockDomainCreate( the_system->wpid, &ptp_cd_descriptor);
   WTI_TerminateOnError(cd_handle, "WP_PtpClockDomainCreate ", __LINE__);


   ptp_cd_descriptor.clock_domain_hw_id = 1;
   cd_handle = WP_PtpClockDomainCreate( the_system->wpid, &ptp_cd_descriptor);
   WTI_TerminateOnError(cd_handle, "WP_PtpClockDomainCreate ", __LINE__);
}



/*****************************************************************************
 * Function name: WTI_IwSystemSetup
 * Description  : create IW systems in both directions, create Rx binding
 *                in the PSN-->TDM direction, create a deny flow aggregation,
 *                build IW system in the PSN-->TDM direction.
 * Input  params: none
 * Output params:
 * Return val   : none
 *****************************************************************************/
void WTI_IwSystemSetup(void)
{
   WP_status status;

   /* create TDM to PSN Interworking system */
   WTI_Tdm2PsnIwSystemConfig(&iw_cesop_system_config);
   the_system->tdm2psn_iw_system = WP_IwSystemCreate(the_system->wpid,
                                                     WP_IW_CESOP_MODE,
                                                     &iw_cesop_system_config);
   WTI_TerminateOnError(the_system->tdm2psn_iw_system, "WP_IwSystemCreate TDM-->PSN", __LINE__);

   /* Setting the iw system mode to bridging or routing */
   the_system->iw_system_mode = WTI_IW_SYSTEM_MODE;

   if(the_system->iw_system_mode == WTI_IW_SYSTEM_ROUTING )
   {  /* iw_system_mode is ROUTING */
      printf("***********************************************\n");
      printf("* The interworking system mode is ROUTING !!! *\n");
      printf("***********************************************\n");
   }
   else
   { /* iw_system_mode is BRIDGING */
      printf("************************************************\n");
      printf("* The interworking system mode is BRIDGING !!! *\n");
      printf("************************************************\n");
   }

#if WTI_CESOP_MPLS_IW

#if ((!WTI_MPLS_LOOPBACK) && (!WTI_CESOP_MPLS_OVER_ENET))
   WPU_Oc12Pmc5357CardInit(WP_PORT_UPI2, WPU_MODE_OC12_POS | WPU_MODE_OC12_LOOPBACK_OFF);
#endif

#if WTI_PCE_CLASSIFIER
   /* PCE can work with bridging and routing IW system and that's why these IW systems are used
      instead of MPLS routing */
   if (the_system->iw_system_mode == WTI_IW_SYSTEM_ROUTING)
   {
      WTI_IwSystemRoutingConfig(&iw_routing_system_config,
                                &ipv4_sys_info);
      the_system->psn2tdm_iw_system = WP_IwSystemCreate(the_system->wpid,
                                                        WP_IW_ROUTING_MODE,
                                                        &iw_routing_system_config);
      WTI_TerminateOnError(the_system->psn2tdm_iw_system, "WP_IwSystemCreate PSN-->TDM", __LINE__);
   }
   else if(the_system->iw_system_mode == WTI_IW_SYSTEM_BRIDGE)
   {
      WP_iw_sys_bridging iw_bridge_system_config;

      WTI_IwSystemBridgingConfig(&iw_bridge_system_config);
      the_system->psn2tdm_iw_system = WP_IwSystemCreate(the_system->wpid,
                                                        WP_IW_VLAN_AWARE_BRIDGING_MODE,
                                                        &iw_bridge_system_config);
      WTI_TerminateOnError(the_system->psn2tdm_iw_system, "WP_IwSystemCreate PSN-->TDM", __LINE__);
   }
   else
   {
      printf("INVALID IW config\n");
      exit(1);
   }
#else  /* not WTI_PCE_CLASSIFIER */
    WTI_MplsIwSystemConfig(&iw_mpls_sys_config);
    the_system->psn2tdm_iw_system = WP_IwSystemCreate(the_system->wpid,
                                                      WP_IW_MPLS_ROUTING_MODE,
                                                      &iw_mpls_sys_config);
    WTI_TerminateOnError(the_system->psn2tdm_iw_system, "WP_IwSystemCreate PSN-->TDM", __LINE__);
#endif  /* WTI_PCE_CLASSIFIER */

#if WTI_CESOP_MPLS_OVER_ENET

   /* create Enet channels */
   WTI_EnetChannelsCreate();

   /* Tx binding */
   WTI_TxBindingEnetConfig(&tx_binding_gbe);

   status = WP_IwTxBindingCreate(the_system->tx_enet_channel,
                                 WP_IW_TX_BINDING,
                                 &tx_binding_gbe);
   WTI_TerminateOnError(status, "WP_IwTxBindingCreate TDM-->PSN", __LINE__);

#endif  /* WTI_CESOP_MPLS_OVER_ENET */

#else  /* not WTI_CESOP_MPLS_IW */
   /* create Enet channels */
   WTI_EnetChannelsCreate();

   /* create PSN to TDM Interworking system */
   if (the_system->iw_system_mode == WTI_IW_SYSTEM_ROUTING)
   {
      WTI_IwSystemRoutingConfig(&iw_routing_system_config,
                                &ipv4_sys_info);
      the_system->psn2tdm_iw_system = WP_IwSystemCreate(the_system->wpid,
                                                        WP_IW_ROUTING_MODE,
                                                        &iw_routing_system_config);
      WTI_TerminateOnError(the_system->psn2tdm_iw_system, "WP_IwSystemCreate PSN-->TDM", __LINE__);
   }
   else if(the_system->iw_system_mode == WTI_IW_SYSTEM_BRIDGE)
   {
      WP_iw_sys_bridging iw_bridge_system_config;

      WTI_IwSystemBridgingConfig(&iw_bridge_system_config);
      the_system->psn2tdm_iw_system = WP_IwSystemCreate(the_system->wpid,
                                                        WP_IW_VLAN_AWARE_BRIDGING_MODE,
                                                        &iw_bridge_system_config);
      WTI_TerminateOnError(the_system->psn2tdm_iw_system, "WP_IwSystemCreate PSN-->TDM", __LINE__);
   }
   else
   {
      printf("INVALID IW config\n");
      exit(1);
   }

   WTI_TxBindingEnetConfig(&tx_binding_gbe);
   status = WP_IwTxBindingCreate(the_system->tx_enet_channel,
                                 WP_IW_TX_BINDING,
                                 &tx_binding_gbe);
   WTI_TerminateOnError(status, "WP_IwTxBindingCreate TDM-->PSN", __LINE__);

#if WTI_EXT_DIFF_MODE_ACTIVE
   if (global_active_cr_async_mode > 0)
   {
      /* high priority channel for adaptive mode */
      WTI_TxBindingEnetConfig(&tx_binding_gbe);
      status = WP_IwTxBindingCreate(the_system->tx_enet_channel_high_priority,
                                    WP_IW_TX_BINDING,
                                    &tx_binding_gbe);
      WTI_TerminateOnError(status, "WP_IwTxBindingCreate TDM-->PSN High_Priority ", __LINE__);
   }
#endif
#endif  /* WTI_CESOP_MPLS_IW */

#if WTI_2_ENET_DEVICES
   WTI_TxBindingEnetConfig(&tx_binding_gbe);
   status = WP_IwTxBindingCreate(the_system->tx_enet_channel1,
                                 WP_IW_TX_BINDING,
                                 &tx_binding_gbe);
   WTI_TerminateOnError(status, "WP_IwTxBindingCreate second enet device", __LINE__);

#if WTI_EXT_DIFF_MODE_ACTIVE
   if (global_active_cr_async_mode > 0)
   {
      WTI_TxBindingEnetConfig(&tx_binding_gbe);
      status = WP_IwTxBindingCreate(the_system->tx_enet_channel1_high_priority,
                                    WP_IW_TX_BINDING,
                                    &tx_binding_gbe);
      WTI_TerminateOnError(status, "WP_IwTxBindingCreate second enet device TDM-->PSN High_Priority ", __LINE__);
   }
#endif
#endif

#if WTI_4_ENET_DEVICES
   WTI_TxBindingEnetConfig(&tx_binding_gbe);
   status = WP_IwTxBindingCreate(the_system->tx_enet_channel2,
                                 WP_IW_TX_BINDING,
                                 &tx_binding_gbe);
   WTI_TerminateOnError(status, "WP_IwTxBindingCreate third enet device", __LINE__);

#if WTI_EXT_DIFF_MODE_ACTIVE
   if (global_active_cr_async_mode > 0)
   {
      WTI_TxBindingEnetConfig(&tx_binding_gbe);
      status = WP_IwTxBindingCreate(the_system->tx_enet_channel2_high_priority,
                                    WP_IW_TX_BINDING,
                                    &tx_binding_gbe);
      WTI_TerminateOnError(status, "WP_IwTxBindingCreate third enet device TDM-->PSN High_Priority ", __LINE__);
   }
#endif

   WTI_TxBindingEnetConfig(&tx_binding_gbe);
   status = WP_IwTxBindingCreate(the_system->tx_enet_channel3,
                                 WP_IW_TX_BINDING,
                                 &tx_binding_gbe);
   WTI_TerminateOnError(status, "WP_IwTxBindingCreate fourth enet device", __LINE__);

#if WTI_EXT_DIFF_MODE_ACTIVE
   if (global_active_cr_async_mode > 0)
   {
      WTI_TxBindingEnetConfig(&tx_binding_gbe);
      status = WP_IwTxBindingCreate(the_system->tx_enet_channel3_high_priority,
                                    WP_IW_TX_BINDING,
                                    &tx_binding_gbe);
      WTI_TerminateOnError(status, "WP_IwTxBindingCreate fourth enet device TDM-->PSN High_Priority ", __LINE__);
   }
#endif

#endif

   /* create rx channel for host termaintion */
   WTI_HostRxChannelsCreate();

   /* create host termination flow aggregation */
   host_term_agg[0].txfunc = the_system->rx_host_channels[0];
   the_system->host_term_flow_agg = WP_IwFlowAggregationCreate(the_system->wpid,
                                                               WP_IW_HOST_QUEUE_MODE,
                                                               &host_term_agg);
   WTI_TerminateOnError(the_system->host_term_flow_agg,
                        "WP_IwFlowAggregationCreate host_term_flow aggregation",
                        __LINE__);


#if WTI_PCE_CLASSIFIER

#if ((!TWO_BOARDS_SIMULATION) || (!__linux__))
#else
   rx_chan = the_system->rx_host_channels[0];
   tx_handle = the_system->tx_enet_channel;
   tx_pool = the_system->qnode[WTI_QNODE_IW_PSN2TDM].adjunct_pool;
   qnode_handle = the_system->qnode[WTI_QNODE_IW_PSN2TDM].handle;
#ifdef WTI_BOARD_1
   tun_init("10.0.1.13","10.0.1.10");
#else
   tun_init("10.0.2.13","10.0.2.10");
#endif
   /* Arm RX handler  */
   status = WP_ControlRegister(WP_EVENT_POLLED_RX_INDICATE,tun_rx_event);
   WTI_TerminateOnError(status, "WP_ControlRegister()", __LINE__);
#endif

   /* configure the pce filter */
   WTI_PceFilterConfig(&filter_param);
   the_system->pce_filter_handle = WP_PceFilterCreate(the_system->wpid,
                                                      WP_PCE_FILTER_CLASSIFICATION,
                                                      &filter_param);
   WTI_TerminateOnError(the_system->pce_filter_handle, "WP_PceFilterCreate()", __LINE__);

   /* configure and create the L1 filter set which includes a single filter */
   memset(&level1_set_cfg, 0, sizeof(WP_pce_filter_set));
   level1_set_cfg.tag = 1;
   level1_set_cfg.filter_set_level = 1;
   level1_set_cfg.filters[0] = the_system->pce_filter_handle;
   level1_set_cfg.filters[1] = WP_UNUSED;
   level1_set_cfg.next_filter_set = WP_UNUSED;

   the_system->pce_filter_set_handle = WP_PceFilterSetCreate(the_system->wpid, &level1_set_cfg);
   WTI_TerminateOnError(the_system->pce_filter_set_handle, "WP_PceFilterSetCreate()", __LINE__);

   /* configure and create the PCE interface which includes a single filter set */
   memset(&pce_interface_cfg, 0, sizeof(WP_pce_if_params_pkt_rx_channel));
   pce_interface_cfg.filter_set_handle = the_system->pce_filter_set_handle;
   pce_interface_cfg.parser_start_type = WP_PCE_PARSER_START_TYPE_ETHERNET;
   pce_interface_cfg.mode = WP_PCE_IW_PORT_CONNECTION_ENABLED;
   pce_interface_cfg.ip_header_validation = WP_DISABLE;

   the_system->pce_interface_handle = WP_PceInterfaceCreate(WP_PCE_IF_TYPE_PKT_RX_CHANNEL, &pce_interface_cfg);
   WTI_TerminateOnError(the_system->pce_interface_handle, "WP_PceInterfaceCreate()", __LINE__);
#else /*Not winpath3*/
#if (! WTI_CESOP_MPLS_IW)
#if ((!TWO_BOARDS_SIMULATION) || (!__linux__))
#else
   rx_chan = the_system->rx_host_channels[0];
   tx_handle = the_system->tx_enet_channel;
   tx_pool = the_system->qnode[WTI_QNODE_IW_PSN2TDM].adjunct_pool;
   qnode_handle = the_system->qnode[WTI_QNODE_IW_PSN2TDM].handle;
#ifdef WTI_BOARD_1
   tun_init("10.0.1.13","10.0.1.10");
#else
   tun_init("10.0.2.13","10.0.2.10");
#endif
   /* Arm RX handler  */
   status = WP_ControlRegister(WP_EVENT_POLLED_RX_INDICATE,tun_rx_event);
   WTI_TerminateOnError(status, "WP_ControlRegister()", __LINE__);
#endif
   /* configure the dfc filter */
   WTI_DfcFilterConfig(&filter_param);
   the_system->dfc_filter_handle = WP_IwClassFilterAdd(the_system->psn2tdm_iw_system,
                                                       &filter_param);
   WTI_TerminateOnError(the_system->dfc_filter_handle, "WP_FilterAdd()", __LINE__);
#endif /* !WTI_CESOP_MPLS_IW*/
#endif /*winpath*/


#if (WTI_CESOP_MPLS_IW && !WTI_PCE_CLASSIFIER)
   /* create rx binding PSN ----> TDM */
   WTI_MplsRxBindingRoutingConfig(&rx_binding_mpls);
#if WTI_CESOP_MPLS_OVER_ENET
   status = WP_IwRxBindingCreate(the_system->rx_enet_channel,
#else
   status = WP_IwRxBindingCreate(the_system->hspi_device,
#endif
                                 the_system->psn2tdm_iw_system,
                                 the_system->qnode[WTI_QNODE_IW_PSN2TDM].handle,
                                 &rx_binding_mpls);
   WTI_TerminateOnError(status, "WP_IwRxBindingCreate PSN-->TDM", __LINE__);
#else
   /* create rx binding PSN ----> TDM */
   if(the_system->iw_system_mode == WTI_IW_SYSTEM_ROUTING /*default value of 0*/)
   {
      WP_iw_route_port rout_port_config;

      memset (&rout_port_config,0,sizeof(WP_iw_route_port));

      rout_port_config.tag = 0x1234;
      rout_port_config.input_filters_mask = 0;
      rout_port_config.output_filters_mask = 0;
      rout_port_config.statmode = WP_IW_PORT_STAT_ENABLE;
#if WTI_PCE_CLASSIFIER
      rout_port_config.forwarding_mode = WP_IW_FORWARDING_DIRECT_MAP;
#else
      rout_port_config.forwarding_mode = WP_IW_FORWARDING_DFC | WP_IW_FORWARDING_DIRECT_MAP;
#endif

      WTI_RxBindingRoutingConfig(&rx_binding_gbe);

      rout_port_config.flow_agg = the_system->host_term_flow_agg;
      rx_binding_gbe.input_port = WP_IwPortCreate(the_system->psn2tdm_iw_system,
                                                  &rout_port_config);
      WTI_TerminateOnError(rx_binding_gbe.input_port, "WP_IwPortCreate PSN-->TDM", __LINE__);
      the_system->iw_port = rx_binding_gbe.input_port;

#if WTI_INFO_STRUCTURES_DEFINED
      /* copy configuration structure to internal Demo information base*/
      memcpy(&iw_info_base.rx_binding_gbe, &rx_binding_gbe, sizeof(WP_rx_binding_routing));
#endif
#if WTI_PCE_CLASSIFIER
      rx_binding_gbe.pce_if_handle = the_system->pce_interface_handle;
#endif
      status = WP_IwRxBindingCreate(the_system->rx_enet_channel,
                                    the_system->psn2tdm_iw_system,
                                    the_system->qnode[WTI_QNODE_IW_PSN2TDM].handle,
                                    &rx_binding_gbe);
      WTI_TerminateOnError(status, "WP_IwRxBindingCreate PSN-->TDM", __LINE__);

#if (WTI_2_ENET_DEVICES)
#if WTI_PCE_CLASSIFIER
      rx_binding_gbe.pce_if_handle = the_system->pce_interface_handle;
#endif
      status = WP_IwRxBindingCreate(the_system->rx_enet_channel1,
                                    the_system->psn2tdm_iw_system,
                                    the_system->qnode[WTI_QNODE_IW_PSN2TDM].handle,
                                    &rx_binding_gbe);
      WTI_TerminateOnError(status, "WP_IwRxBindingCreate second enet device", __LINE__);
#endif
#if (WTI_4_ENET_DEVICES)
      status = WP_IwRxBindingCreate(the_system->rx_enet_channel2,
                                    the_system->psn2tdm_iw_system,
                                    the_system->qnode[WTI_QNODE_IW_PSN2TDM].handle,
                                    &rx_binding_gbe);
      WTI_TerminateOnError(status, "WP_IwRxBindingCreate third enet device", __LINE__);

      status = WP_IwRxBindingCreate(the_system->rx_enet_channel3,
                                    the_system->psn2tdm_iw_system,
                                    the_system->qnode[WTI_QNODE_IW_PSN2TDM].handle,
                                    &rx_binding_gbe);
      WTI_TerminateOnError(status, "WP_IwRxBindingCreate fourth enet device", __LINE__);
#endif
   }
   else if(the_system->iw_system_mode == WTI_IW_SYSTEM_BRIDGE)
   {
      WP_bridge_port bridge_port_config;
      memset (&bridge_port_config,0,sizeof(WP_bridge_port));

      WTI_RxBindingBridgeConfig(&rx_binding_gbe_bridge);


      bridge_port_config.tag = 0x123;/*A user-supplied tag unoqueu per bridge port */
      bridge_port_config.direct_mapping = WP_IW_DIRECT_MAP_ENABLE; /*No FDB only DFC and default agg*/
      bridge_port_config.flow_agg = the_system->host_term_flow_agg;
      /*Specify the default flow aggregation (and associated transmit channel) in this case send to the same flow agg ofvaiolations */
      bridge_port_config.flooding_term_mode=WP_IW_HOST_TERM_MODE;
      bridge_port_config.learning_mode=WP_IW_LEARNING_DISABLE;
      bridge_port_config.in_filter_mode=WP_IW_INGRESS_FILTER_DISABLE;
      bridge_port_config.vlan_param.vlan_acceptance_mode =WP_IW_ACCEPT_TAGGED_UNTAGGED;
      bridge_port_config.vlan_param.vlan_tag =0x12;
      bridge_port_config.vlan_param.vlan_tunnel=WP_IW_VLAN_TUNNEL_1Q_IN_1Q_DISABLE;
      bridge_port_config.vlan_param.vlan_priority_enforce_mode=WP_IW_VLAN_PRIORITY_ENFORCE_DISABLED;

      bridge_port_config.max_mac_addresses=WTI_MAX_PW;
      bridge_port_config.group_tag=0;
      bridge_port_config. group_filtering_mode=WP_IW_GROUP_FILTER_DISABLE;
      bridge_port_config. unk_mac_sa_filter=WP_IW_UNK_MACSA_FILTER_ENABLE;
      bridge_port_config. unk_mc_mode=WP_IW_UNK_MC_DROP;
      bridge_port_config. bc_ht_mode = WP_IW_BC_HT_ENABLE;
      bridge_port_config.input_filters_mask=0x80000000; /*WP_IW_PORT_FILTERING_DISABLED*/
      bridge_port_config.output_filters_mask=0x80000000; /*WP_IW_PORT_FILTERING_DISABLED*/
      bridge_port_config.statmode=WP_IW_PORT_STAT_ENABLE;
      bridge_port_config. unk_uc_mode=WP_IW_UNK_UC_SR_DISABLE;
      bridge_port_config. classification_flag=WP_IW_BPORT_CLASSIFICATION_ENABLED;
      bridge_port_config. adv_unk_lookup_mode=WP_IW_ADV_UNK_LOOKUP_DISABLED;
      bridge_port_config. cfi_ht_mode=WP_IW_CFI_HT_DISABLE;
      bridge_port_config. reserved_mc_ht_mode=WP_IW_RES_MC_HT_DISABLE;
      bridge_port_config. predefined_ht_mode=WP_DISABLE;
      rx_binding_gbe_bridge.input_port = WP_IwPortCreate(the_system->psn2tdm_iw_system, &bridge_port_config);
      WTI_TerminateOnError( rx_binding_gbe_bridge.input_port, "WP_IwPortCreate PSN-->TDM", __LINE__);
      the_system->bridge_iw_port = rx_binding_gbe_bridge.input_port;

#if WTI_INFO_STRUCTURES_DEFINED
      /* copy configuration structure to internal Demo information base*/
      memcpy(&iw_info_base.rx_binding_gbe_bridge, &rx_binding_gbe_bridge, sizeof(WP_rx_binding_routing));
#endif

      status = WP_IwRxBindingCreate(the_system->rx_enet_channel,
                                    the_system->psn2tdm_iw_system,
                                    the_system->qnode[WTI_QNODE_IW_PSN2TDM].handle,
                                    &rx_binding_gbe_bridge);
      WTI_TerminateOnError(status, "WP_IwRxBindingCreate PSN-->TDM", __LINE__);

#if (WTI_2_ENET_DEVICES)
      status = WP_IwRxBindingCreate(the_system->rx_enet_channel1,
                                    the_system->psn2tdm_iw_system,
                                    the_system->qnode[WTI_QNODE_IW_PSN2TDM].handle,
                                    &rx_binding_gbe_bridge);
      WTI_TerminateOnError(status, "WP_IwRxBindingCreate second enet device", __LINE__);
#endif
#if (WTI_4_ENET_DEVICES)
      status = WP_IwRxBindingCreate(the_system->rx_enet_channel2,
                                    the_system->psn2tdm_iw_system,
                                    the_system->qnode[WTI_QNODE_IW_PSN2TDM].handle,
                                    &rx_binding_gbe_bridge);
      WTI_TerminateOnError(status, "WP_IwRxBindingCreate third enet device", __LINE__);

      status = WP_IwRxBindingCreate(the_system->rx_enet_channel3,
                                    the_system->psn2tdm_iw_system,
                                    the_system->qnode[WTI_QNODE_IW_PSN2TDM].handle,
                                    &rx_binding_gbe_bridge);
      WTI_TerminateOnError(status, "WP_IwRxBindingCreate fourth enet device", __LINE__);
#endif

   }

#endif /* WTI_CESOP_MPLS_IW */

   /* build IW system in the PSN-->TDM direction */
   status = WP_IwSystemBuild(the_system->psn2tdm_iw_system);
   WTI_TerminateOnError(status, "WP_IwSystemBuild PSN-->TDM",__LINE__);

  return;
}

/*****************************************************************************
 * Function name: WTI_PwCreate
 * Description  : create a single PW.
 * Input  params: pw_index - PW index
 *                oobc_mode - out of band CAS mode (0-disable,1-enable)
 *                rx_clock_master flag - if clock recovery module is enabled, this flag
 *                indicates if this PW is the Rx clock master PW of the port (in UFE2 only).
 *                tx_clock_master flag - if clock recovery module is enabled, this flag
 *                indicates if this PW is the clock master PW of the port.
 * Output params:
 * Return val   : none
 *****************************************************************************/
void WTI_PwCreate(int pw_index,
                  int oobc_mode,
                  int tx_clock_master_flag,
                  int rx_clock_master_flag)
{
#if (!WTI_CESOP_TDI)
   WP_status status;
#endif

   if (pw_index >= WTI_MAX_PW)
   {
      printf("Test Abort: PW index is invalid %d\n", pw_index);
      WTI_Terminate(1);
      exit(1);
   }

   if (oobc_mode != 0 && oobc_mode != 1)
   {
      printf("Test Abort: oobc_mode is invalid %d\n", oobc_mode);
      WTI_Terminate(1);
      exit(1);
   }

#if (!WTI_CESOP_TDI)
   /* create UFE phy and EMPHY transparent device */
   WTI_TransDeviceCreate(pw_index,
                         rx_clock_master_flag,
                         the_system->trans_port,
                         &the_system->ufe,
                         pw_config.line_index);

#endif

   /* create transparent Rx channel, flow aggregation and Rx binding
      in the TDM to PSN direction */
   WTI_Tdm2PsnFlowCreate(pw_index, oobc_mode);

   /* create transparent Tx channel, flow aggregation and Tx binding
      in the PSN to TDM direction */
   WTI_Psn2TdmFlowCreate(pw_index, oobc_mode, tx_clock_master_flag);

#if (!WTI_CESOP_TDI)
   /* enable UFE phy */
   WT_UfePhyEnable(&the_system->ufe, pw_index);

   /* enable transparent device */
   status = WP_DeviceEnable(the_system->pw[pw_index].trans_device, WP_DIRECTION_DUPLEX);
   WTI_TerminateOnError(status, "WP_DeviceEnable Transparent device",__LINE__);
#endif

   /* set PW state to WTI_PW_CREATED */
   the_system->pw[pw_index].state |= WTI_PW_CREATED;

   return;
}

/*****************************************************************************
 * Function name: WTI_PwCreate
 * Description  : create a single PW.
 * Input  params: pw_index - PW index
 *                oobc_mode - out of band CAS mode (0-disable,1-enable)
 *                rx_clock_master flag - if clock recovery module is enabled, this flag
 *                indicates if this PW is the Rx clock master PW of the port (in UFE2 only).
 *                tx_clock_master flag - if clock recovery module is enabled, this flag
 *                indicates if this PW is the clock master PW of the port.
 * Output params:
 * Return val   : none
 *****************************************************************************/
void WTI_PwCreateNoPhyEnable(int pw_index,
                             int oobc_mode,
                             int tx_clock_master_flag,
                             int rx_clock_master_flag)

{
#if (!WTI_CESOP_TDI)
   WP_status status;
#endif

   if (pw_index >= WTI_MAX_PW)
   {
      printf("Test Abort: PW index is invalid %d\n", pw_index);
      WTI_Terminate(1);
      exit(1);
   }

   if (oobc_mode != 0 && oobc_mode != 1)
   {
      printf("Test Abort: oobc_mode is invalid %d\n", oobc_mode);
      WTI_Terminate(1);
      exit(1);
   }

#if (!WTI_CESOP_TDI)
   /* create UFE phy and EMPHY transparent device */
   WTI_TransDeviceCreate(pw_index,
                         rx_clock_master_flag,
                         the_system->trans_port,
                         &the_system->ufe,
                         pw_config.line_index);

#endif

   /* create transparent Rx channel, flow aggregation and Rx binding
      in the TDM to PSN direction */
   WTI_Tdm2PsnFlowCreate(pw_index, oobc_mode);

   /* create transparent Tx channel, flow aggregation and Tx binding
      in the PSN to TDM direction */
   WTI_Psn2TdmFlowCreate(pw_index, oobc_mode, tx_clock_master_flag);

#if (!WTI_CESOP_TDI)
   /* enable UFE phy */
   //WT_UfePhyEnable(&the_system->ufe, pw_index);

   /* enable transparent device */
   status = WP_DeviceEnable(the_system->pw[pw_index].trans_device, WP_DIRECTION_DUPLEX);
   WTI_TerminateOnError(status, "WP_DeviceEnable Transparent device",__LINE__);
#endif

   /* set PW state to WTI_PW_CREATED */
   the_system->pw[pw_index].state |= WTI_PW_CREATED;

   return;
}


#if WTI_DUAL_EMPHY
/*****************************************************************************
 * Function name: WTI_PwCreateOnSecondUfe
 * Description  : create a single PW on the second UFE device.
 * Input  params: pw_index - PW index
 *                oobc_mode - out of band CAS mode (0-disable,1-enable)
 *                rx_clock_master flag - if clock recovery module is enabled, this flag
 *                indicates if this PW is the Rx clock master PW of the port (in UFE2 only).
 *                tx_clock_master flag - if clock recovery module is enabled, this flag
 *                indicates if this PW is the clock master PW of the port.
 * Output params:
 * Return val   : none
 *****************************************************************************/
void WTI_PwCreateOnSecondUfe(int pw_index,
                             int oobc_mode,
                             int tx_clock_master_flag,
                             int rx_clock_master_flag)
{
   WP_status status;

   if (pw_index >= WTI_MAX_PW)
   {
      printf("Test Abort: PW index is invalid %d\n", pw_index);
      WTI_Terminate(1);
      exit(1);
   }

   if (oobc_mode != 0 && oobc_mode != 1)
   {
      printf("Test Abort: oobc_mode is invalid %d\n", oobc_mode);
      WTI_Terminate(1);
      exit(1);
   }

   /* create UFE phy and EMPHY transparent device */
   WTI_TransDeviceCreate(pw_index,
                         rx_clock_master_flag,
                         the_system->second_trans_port,
                         &the_system->second_ufe,
                         pw_config.line_index);

   /* create transparent Rx channel, flow aggregation and Rx binding
      in the TDM to PSN direction */
   WTI_Tdm2PsnFlowCreate(pw_index, oobc_mode);

   /* create transparent Tx channel, flow aggregation and Tx binding
      in the PSN to TDM direction */
   WTI_Psn2TdmFlowCreate(pw_index, oobc_mode, tx_clock_master_flag);

   /* enable UFE phy */
   WT_UfePhyEnable(&the_system->second_ufe, pw_index);

   /* enable transparent device */
   status = WP_DeviceEnable(the_system->pw[pw_index].trans_device, WP_DIRECTION_DUPLEX);
   WTI_TerminateOnError(status, "WP_DeviceEnable Transparent device",__LINE__);

   /* set PW state to WTI_PW_CREATED */
   the_system->pw[pw_index].state |= WTI_PW_CREATED;

   return;
}
#endif

#if WTI_CESOP_TDI

void WTI_TdiTransDeviceClockRecModify(WP_U32 index, WP_U32 val)
{
#if (WTI_CESOP_CLOCK_RECOVERY_ENABLE || WTI_CESOP_RX_TIMING_ENABLE)
   WP_device_tdm_trans_ces_pwe3 *pwe3_ext_ptr_config = &pwe3_ext_config[index];

   if (val == 1)
   {
      pwe3_ext_ptr_config->pwe3_characteristics |= WP_TDI_PWE3_CES_CLOCK_REC;
   }
   else
   {
      pwe3_ext_ptr_config->pwe3_characteristics &= ~WP_TDI_PWE3_CES_CLOCK_REC;
   }
#endif
   return;
}

/*****************************************************************************
 * Function name: WTI_TdiTransCesDeviceConfig
 * Description  : configure transparent CES device over TDI
 * Input  params: trans_binding - pointer to WP_tdm_binding configuration structure
 *                tdm_ces_specific - pointer to WP_device_tdm_trans_ces configuration
 *                                   structure
 *                trans_device_config - pointer to WP_device_tdm_trans configuration
 *                                      structure
 *                device_index - index of TDM transparent CES device
 * Output params:
 * Return val   : none
 *****************************************************************************/
static void WTI_TdiTransCesDeviceConfig(WP_tdm_binding *trans_binding,
                                        WP_device_tdm_trans_ces *tdm_ces_specific,
                                        WP_device_tdm_trans *trans_device_config,
                                        WP_U32 device_index)
{
   WP_device_tdm_trans_ces_pwe3 *pwe3_ext_ptr_config = &pwe3_ext_config[device_index];

   memset(trans_binding, 0, sizeof(WP_tdm_binding));
   memset(tdm_ces_specific, 0, sizeof(WP_device_tdm_trans_ces));
   memset(trans_device_config, 0, sizeof(WP_device_tdm_trans));
   memset(pwe3_ext_ptr_config, 0, sizeof(WP_device_tdm_trans_ces_pwe3));

   trans_binding->startslot = 0;
   trans_binding->endslot = 31;
   tdm_ces_specific->tx_parambustype = WTI_TX_PARAM_BUS_TYPE;
   tdm_ces_specific->tx_parambusbank = WTI_TX_PARAM_BUS_BANK;
   tdm_ces_specific->tx_databustype = WTI_TX_DATA_BUS_TYPE;
   tdm_ces_specific->tx_databusbank = WTI_TX_DATA_BUS_BANK;
   if (tdm_config[device_index].cas_mode == WTI_TDI_CAS_MODE)
      tdm_ces_specific->tx_opmode = WP_TDM_TRANS_CAS_NO_SYNC;
   else
      tdm_ces_specific->tx_opmode = WP_TDM_TRANS_NO_CAS;
   tdm_ces_specific->tx_interruptqueue = WTI_TX_INTERRUPT_QUEUE;
   tdm_ces_specific->rx_parambustype = WTI_RX_PARAM_BUS_TYPE;
   tdm_ces_specific->rx_parambusbank = WTI_RX_PARAM_BUS_BANK;
   tdm_ces_specific->rx_databustype = WTI_RX_DATA_BUS_TYPE;
   if (tdm_config[device_index].cas_mode == WTI_TDI_CAS_MODE)
      tdm_ces_specific->rx_opmode = WP_TDM_TRANS_CAS_NO_SYNC;
   else
      tdm_ces_specific->rx_opmode = WP_TDM_TRANS_NO_CAS;
   tdm_ces_specific->rx_cas_location = WTI_CAS_LOCATION;
   tdm_ces_specific->rx_interruptqueue = WTI_RX_INTERRUPT_QUEUE;
   tdm_ces_specific->pwe3_ext = pwe3_ext_ptr_config;

#if (WTI_CESOP_CLOCK_RECOVERY_ENABLE || WTI_CESOP_RX_TIMING_ENABLE)
   /* if no CR and no RX TS mode - unused struct */
   if (!WTI_CESOP_CLOCK_RECOVERY_ENABLE && !WTI_CESOP_RX_TIMING_ENABLE)
   {
      tdm_ces_specific->pwe3_ext = 0;
   }
   else
   {
      if (WTI_CESOP_CLOCK_RECOVERY_ENABLE)
      {
         pwe3_ext_ptr_config->pwe3_characteristics = 0;
         /* in CR differential mode */
         if (WTI_CLOCK_REC_MODE)
            pwe3_ext_ptr_config->pwe3_characteristics |= WP_TDI_PWE3_CES_TX_TS_DIFFERENTIAL;
         /* need RX TS and Diff TS */
         if (WTI_CESOP_RX_TIMING_ENABLE && !WTI_ACTIVE_ABSOLUTE_MODE)
            pwe3_ext_ptr_config->pwe3_characteristics |= WP_TDI_PWE3_CES_RX_TS_DIFFERENTIAL;
#if WTI_CESOP_TDI
         pwe3_ext_ptr_config->pwe3_characteristics |= WP_TDI_PWE3_CES_CLOCK_REC;
#endif
      }
   }
#else
   tdm_ces_specific->pwe3_ext = 0;
#endif

   trans_device_config->n_timeslot_binding = 1;
   trans_device_config->timeslot_binding =  trans_binding;
   trans_device_config->framemode = WP_TDM_TRANS_SINGLE_FRAME;
   if (tdm_config[device_index].op_mode == WTI_TDI_E1_OP_MODE)
   {
      if (tdm_config[device_index].framing_mode == WTI_TDI_FRAMED_MODE)
      {
         trans_device_config->protocol = (WP_TDM_TRANS_E1_STRUCTURED | WP_TDM_TRANS_PWE3);
         trans_device_config->structure_size = WP_TDM_STRUCTURE_E1_ESF;
      }
      else
      {
         trans_device_config->protocol = (WP_TDM_TRANS_E1 | WP_TDM_TRANS_PWE3);
         trans_device_config->structure_size = WP_TDM_STRUCTURE_BASIC;
      }
   }
   else
   {
      if (tdm_config[device_index].framing_mode == WTI_TDI_FRAMED_MODE)
      {
         trans_device_config->protocol = (WP_TDM_TRANS_T1_STRUCTURED | WP_TDM_TRANS_PWE3);
         trans_device_config->structure_size = WP_TDM_STRUCTURE_T1_ESF;
      }
      else
      {
         trans_device_config->protocol = (WP_TDM_TRANS_T1 | WP_TDM_TRANS_PWE3);
         trans_device_config->structure_size = WP_TDM_STRUCTURE_BASIC;
      }
   }
   trans_device_config->datareverse = WP_TDM_TRANS_REVERSE;
   trans_device_config->sigreverse = WP_TDM_TRANS_REVERSE;
   trans_device_config->protocol_specific = tdm_ces_specific;
}

/*****************************************************************************
 * Function name: WTI_TdiTransDeviceAlloc
 * Description  : Allocate transparent CES device over TDI
 * Input  params: port_index - TDM port index
 * Output params:
 * Return val   : none
 *****************************************************************************/
static void WTI_TdiTransDeviceAlloc(int port_index)
{

   /* Configure a Transparent Device over TDM */
   WTI_TdiTransCesDeviceConfig(&trans_binding[port_index],
                               &tdm_ces_specific[port_index],
                               &trans_device_config[port_index],
                               port_index);

   /* Allocate a Transparent Device over TDM */
   the_system->tdm_trans_dev[port_index] = WP_DeviceAlloc(the_system->tdm_port[port_index],
                                                          WP_UNUSED,
                                                          WP_DEVICE_TRANS,
                                                          &trans_device_config[port_index]);
   WTI_TerminateOnError(the_system->tdm_trans_dev[port_index], "WP_DeviceAlloc() TDI trans",__LINE__);
}

/*****************************************************************************
 * Function name: WTI_TdiTransDeviceCreate
 * Description  : create transparent CES device over TDI
 * Input  params: port_index - TDM port index
 * Output params:
 * Return val   : none
 *****************************************************************************/
static void WTI_TdiTransDeviceCreate(int port_index)
{

   /* Configure a Transparent Device over TDM */
   WTI_TdiTransCesDeviceConfig(&trans_binding[port_index],
                               &tdm_ces_specific[port_index],
                               &trans_device_config[port_index],
                               port_index);

   /* Create a Transparent Device over TDM */
   the_system->tdm_trans_dev[port_index] = WP_DeviceCreate(the_system->tdm_port[port_index],
                                                           WP_UNUSED,
                                                           WP_DEVICE_TRANS,
                                                           &trans_device_config[port_index]);
   WTI_TerminateOnError(the_system->tdm_trans_dev[port_index], "WP_DeviceCreate() TDI trans",__LINE__);
}

#else
/*****************************************************************************
 * Function name: WTI_TransDeviceCreate
 * Description  : create UFE phy and EMPHY transparent device, enable device
 * Input  params:
 * Output params:
 * Return val   : none
 *****************************************************************************/
static void WTI_TransDeviceCreate(int pw_index,
                                  int rx_clock_master_flag,
                                  WP_handle emphy_port,
                                  WT_ufe *ufe,
                                  int line_index)
{
   WP_U32 ufe_phy_index, i, line_id;
   WUFE_status ufe_status;
   WP_handle trans_device;
   WP_U8 slots[WTI_E1_SLOTS];

   /* copy slots from pw_config.slots */
   for (i=0;i<pw_config.n_slots;++i)
      slots[i]=(WP_U8)pw_config.slots[i];

   WT_UfePhyTransCreate(ufe,
                        line_index,
                        pw_config.phy_index,
                        pw_config.n_slots,
                        slots,
                        pw_config.rx_data_unit_size,
                        rx_clock_master_flag);

   /* update PW state */
   the_system->pw[pw_index].state |= WTI_PW_UFE2_PHY_CREATED;

   /* create transparent device */
   upi_trans_device.mode = pw_config.mode;

   if ((upi_trans_device.mode == (WP_EMPHY_TRANS_UNFRAMED | WP_EMPHY_TRANS_E1)) ||
       (upi_trans_device.mode == (WP_EMPHY_TRANS_UNFRAMED | WP_EMPHY_TRANS_T1)) ||
       (upi_trans_device.mode == (WP_EMPHY_TRANS_FRAMED | WP_EMPHY_TRANS_E1)) ||
       (upi_trans_device.mode == (WP_EMPHY_TRANS_FRAMED | WP_EMPHY_TRANS_T1)))
   {
      upi_trans_device.structure_size = WP_TDM_STRUCTURE_BASIC;
   }
   else /* FRAMED CAS */
   {
      if (upi_trans_device.mode == (WP_EMPHY_TRANS_FRAMED_CAS | WP_EMPHY_TRANS_T1))
         upi_trans_device.structure_size = WP_TDM_STRUCTURE_T1_ESF;
      else
         upi_trans_device.structure_size = WP_TDM_STRUCTURE_E1_ESF;
   }

   upi_trans_device.rx_dataunit_size = pw_config.rx_data_unit_size;
   upi_trans_device.tx_dataunit_size = pw_config.tx_data_unit_size;

   ufe_status = WUFE_PhyGetLineId(ufe->phy_handle[pw_index], &line_id);
   if (ufe_status != WUFE_OK)
   {
      printf("ERROR - WUFE_PhyGetLineId failed\n");
      return;
   }
   upi_trans_device.ds1_id = line_id;
#if WTI_INFO_STRUCTURES_DEFINED
   /* copy configuration structure to internal Demo information base*/
   memcpy(&pw_info_base[pw_index].upi_trans_device,
          &upi_trans_device,
          sizeof(WP_device_emphy_trans));
#endif

   ufe_phy_index = WUFE_PHY_INDEX_GET(ufe->phy_handle[pw_index]);
   trans_device = WP_DeviceCreate(emphy_port,
                                  WP_PHY(ufe_phy_index),
                                  WP_DEVICE_TRANS,
                                  &upi_trans_device);
   WTI_TerminateOnError(trans_device, "WP_DeviceCreate EMPHY trans", __LINE__);

   the_system->pw[pw_index].trans_device = trans_device;
}

#endif
/*****************************************************************************
 * Function name: WTI_Tdm2PsnFlowCreate
 * Description  : create TDM --> PSN flow. this function creates transparent Rx channel,
 *                TDM to PSN flow aggregation and Rx binding, builds TDM --> PSN IW system
 * Input  params: pw_index - PW index
 *                oobc_mode - out of band CAS mode (0-disable,1-enable)
 *                clock_rec_mode - in the TDM to PSN direction this flag indicates that the
 *                                 phy associated with this PW is a Rx clock recovery master
 *                                 (only in UFE2 mode)
 * Output params:
 * Return val   : none
 *****************************************************************************/
static void WTI_Tdm2PsnFlowCreate(int pw_index, int oobc_mode)
{
   WP_status status;

   /* create transparent Rx channel */
   WTI_TransChannelRxConfig(&trans_ch_config);

#if WTI_INFO_STRUCTURES_DEFINED
   pw_info_base[pw_index].rx_pwe3_channel_tag = pw_index;
   /* copy configuration structure to internal Demo information base*/
   memcpy(&pw_info_base[pw_index].trans_rx_ch_config,
          &trans_ch_config,
          sizeof(WP_ch_trans_pwe3));
#endif
   the_system->pw[pw_index].trans_rx =
      WP_ChannelCreate(pw_index,
#if WTI_CESOP_TDI
                       the_system->tdm_trans_dev[pw_config.line_index],
#else
                       the_system->pw[pw_index].trans_device,
#endif
                       the_system->qnode[WTI_QNODE_IW_TDM2PSN].handle,
                       WP_CH_RX,
                       WP_TRANSPARENT_PWE3,
                       &trans_ch_config);
   WTI_TerminateOnError(the_system->pw[pw_index].trans_rx,
                   "WP_ChannelCreate trans Rx",
                   __LINE__);

   if (oobc_mode)
   {
      /* create CAS dedicated TDM to PSN flow aggregation */
         /* create TDM to PSN flow aggregation */
      WTI_FlowAggregationTdm2PsnConfig(&tdm2psn_flow_agg_config, pw_index, 2);
#if WTI_INFO_STRUCTURES_DEFINED
      /* copy configuration structure to internal Demo information base*/
      memcpy(&pw_info_base[pw_index].cas_tdm2psn_agg_param,
             &tdm2psn_flow_agg_config,
             sizeof(WP_iw_agg_cesop));
#endif

      the_system->pw[pw_index].cas_tdm2psn_flow_agg =
         WP_IwFlowAggregationCreate(the_system->wpid,
                                    WP_IW_CESOP_MODE,
                                    &tdm2psn_flow_agg_config);
      WTI_TerminateOnError(the_system->pw[pw_index].cas_tdm2psn_flow_agg,
                           "WP_IwFlowAggregationCreate CAS dedicated TDM-->PSN",
                           __LINE__);

      /* create TDM to PSN flow aggregation */
      WTI_FlowAggregationTdm2PsnConfig(&tdm2psn_flow_agg_config, pw_index, 1);
#if WTI_INFO_STRUCTURES_DEFINED
      /* copy configuration structure to internal Demo information base*/
      memcpy(&pw_info_base[pw_index].tdm2psn_agg_param,
             &tdm2psn_flow_agg_config,
             sizeof(WP_iw_agg_cesop));
#endif
      the_system->pw[pw_index].tdm2psn_flow_agg =
         WP_IwFlowAggregationCreate(the_system->wpid,
                                    WP_IW_CESOP_MODE,
                                    &tdm2psn_flow_agg_config);
      WTI_TerminateOnError(the_system->pw[pw_index].tdm2psn_flow_agg,
                           "WP_IwFlowAggregationCreate TDM-->PSN",
                           __LINE__);
   }
   else
   {

      /* create TDM to PSN flow aggregation */
      WTI_FlowAggregationTdm2PsnConfig(&tdm2psn_flow_agg_config, pw_index, 0);
#if WTI_INFO_STRUCTURES_DEFINED
      /* copy configuration structure to internal Demo information base*/
      memcpy(&pw_info_base[pw_index].tdm2psn_agg_param,
             &tdm2psn_flow_agg_config,
             sizeof(WP_iw_agg_cesop));
#endif

      the_system->pw[pw_index].tdm2psn_flow_agg =
         WP_IwFlowAggregationCreate(the_system->wpid,
                                    WP_IW_CESOP_MODE,
                                    &tdm2psn_flow_agg_config);
      WTI_TerminateOnError(the_system->pw[pw_index].tdm2psn_flow_agg,
                           "WP_IwFlowAggregationCreate TDM-->PSN",
                           __LINE__);
   }

   /* create rx binding */
   WTI_RxBindingDirectMapConfig(&trans_rx_binding_config, pw_index);
#if WTI_INFO_STRUCTURES_DEFINED
   /* copy configuration structure to internal Demo information base*/
   memcpy(&pw_info_base[pw_index].trans_rx_binding_config,
          &trans_rx_binding_config,
          sizeof(WP_rx_binding_directmap));
#endif
   status = WP_IwRxBindingCreate(the_system->pw[pw_index].trans_rx,
                                 the_system->tdm2psn_iw_system,
                                 the_system->qnode[WTI_QNODE_IW_TDM2PSN].handle,
                                 &trans_rx_binding_config);
   WTI_TerminateOnError(status, "WP_IwRxBindingCreate TDM --> PSN", __LINE__);

   /* build IW system PSN --> TDM */
   status = WP_IwSystemBuild(the_system->tdm2psn_iw_system);
   WTI_TerminateOnError(status, "WP_IwSystemBuild TDM ----> PSN",__LINE__);
}

/*****************************************************************************
 * Function name: WTI_Psn2TdmUfeClockRecSet
 * Description  : creates PSN --> TDM clock recovery master in UFE; reads the
 *                info and configures the flow aggregation clock recovery
 *                Currently this function is provided in two different versions.
 *
 *                version 1 - is for clock recovery reconfiguration test.
 *                All PW has clock recovery structure in the flow aggregation.
 *                version 2 - is for regular tests.  Only the master PW has
 *                clock recovery structure in the flow aggregation
 *
 *
 * Input  params: pw_index - PW index
 *                clock_master - clock master mode (the clock is recovered according to
 *                               packets arriving to this flow)(0-disable,1-enable)
 *
 * Output params:
 * Return val   : none
 *****************************************************************************/
/*
 * This version is used in reconfiguration clock recovery tests (37500)
*/
#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
static void WTI_Psn2TdmUfeClockRecSet_1(int pw_index, int clock_master)
{
   static WP_U32 ts_const_master, tx_cr_if_master;

   psn2tdm_flow_agg_config.clock_rec_params = &the_system->clock_rec[pw_config.line_index];
   if (clock_master)
   {
#if !(WTI_CESOP_TDI)
      WUFE_clk_rec_info_phy clk_rec_info_phy;

      memset(&clk_rec_info_phy, 0, sizeof(WUFE_clk_rec_info_phy));

      WT_UfeClockRecMasterSet(the_system->ufe.phy_handle[pw_config.phy_index],
                              pw_config.tx_data_unit_size);
      /* get the sub const value from UFE driver */
      WT_UfeClockRecInfoGet(the_system->ufe.ufe_id,
                            the_system->ufe.phy_handle[pw_config.phy_index],
                            WUFE_CLOCK_REC_INFO_PHY,
                            &clk_rec_info_phy);

      ts_const_master = 1;
      /* set clock recovery params structure */
      tx_cr_if_master = clk_rec_info_phy.tx.if_id;
#endif
      /* set clock recovery params structure */
      (psn2tdm_flow_agg_config.clock_rec_params)->clock_rec_master = WP_TRUE;
   }
   else
      (psn2tdm_flow_agg_config.clock_rec_params)->clock_rec_master = WP_FALSE;

   psn2tdm_flow_agg_config.ts_const = ts_const_master;
   (psn2tdm_flow_agg_config.clock_rec_params)->clock_rec_if_id = tx_cr_if_master;

   /*
    * TOFIX
    * All PW from the same clock domain connected has the same clock structure
    * for SATOP with same data_unit the sub const will be the same
    */
}
/*
 * This version is used in regular clock recovery tests (37900)
*/
static void WTI_Psn2TdmUfeClockRecSet_2(int pw_index, int clock_master)
{
#if WTI_CESOP_TDI
   if (clock_master)
   {
      /* set clock recovery params structure */
      the_system->clock_rec[pw_config.line_index].clock_rec_master = WP_TRUE;
      psn2tdm_flow_agg_config.clock_rec_params = &the_system->clock_rec[pw_config.line_index];
   }
#else /* !WTI_CESOP_TDI */
   /* set phy as master */
   if (clock_master)
   {
      WUFE_clk_rec_info_phy clk_rec_info_phy;

      memset(&clk_rec_info_phy, 0, sizeof(WUFE_clk_rec_info_phy));

#ifdef __WT_UFE3__
      WT_UfeClockRecMasterSet(the_system->ufe.phy_handle[pw_config.phy_index],
                               pw_config.tx_data_unit_size);

      /* get the sub const value from UFE driver */
      WT_UfeClockRecInfoGet(the_system->ufe.ufe_id,
                            the_system->ufe.phy_handle[pw_config.phy_index],
                            WUFE_CLOCK_REC_INFO_PHY,
                            &clk_rec_info_phy);
      psn2tdm_flow_agg_config.ts_const = 1;
      the_system->clock_rec[pw_config.line_index].clock_rec_if_id = clk_rec_info_phy.tx.if_id;
#endif /* WT_UFE3 */
      /* set clock recovery params structure */
#if WTI_DUAL_EMPHY
      if(WTI_EMPHY_SECOND == pw_config.pw_emphy_num)
      {
         the_system->clock_rec[pw_config.line_index + WTI_FIRST_UFE_NUM_LINES].clock_rec_master = WP_TRUE;
         psn2tdm_flow_agg_config.clock_rec_params = &the_system->clock_rec[pw_config.line_index + WTI_FIRST_UFE_NUM_LINES];
      }
      else
      {
         the_system->clock_rec[pw_config.line_index].clock_rec_master = WP_TRUE;
         psn2tdm_flow_agg_config.clock_rec_params = &the_system->clock_rec[pw_config.line_index];
      }
#else
      the_system->clock_rec[pw_config.line_index].clock_rec_master = WP_TRUE;
      psn2tdm_flow_agg_config.clock_rec_params = &the_system->clock_rec[pw_config.line_index];
#endif
   }
#endif
}
#endif

/*****************************************************************************
 * Function name: WTI_Psn2TdmFlowCreate
 * Description  : create PSN --> TDM flow. this function creates transparent Tx channel,
 *                ,PSN to TDM flow aggregation and tx binding, adds classification rule
 *                and builds PSN to TDM IW system
 * Input  params: pw_index - PW index
 *                oobc_mode - out of band CAS mode (0-disable,1-enable)
 *                clock_master - clock master mode (the clock is recovered according to
 *                               packets arriving to this flow)(0-disable,1-enable)
 *
 * Output params:
 * Return val   : none
 *****************************************************************************/
static void WTI_Psn2TdmFlowCreate(int pw_index, int oobc_mode, int clock_master)
{
   WP_status status;

   /* create transparent Tx channel */
   WTI_TransChannelTxConfig(&trans_ch_config);
#if WTI_INFO_STRUCTURES_DEFINED
   pw_info_base[pw_index].tx_pwe3_channel_tag = pw_index;
   /* copy configuration structure to internal Demo information base*/
   memcpy(&pw_info_base[pw_index].trans_tx_ch_config,
          &trans_ch_config,
          sizeof(WP_ch_trans_pwe3));
#endif
   the_system->pw[pw_index].trans_tx =
      WP_ChannelCreate(pw_index,
#if WTI_CESOP_TDI
                       the_system->tdm_trans_dev[pw_config.line_index],
#else
                       the_system->pw[pw_index].trans_device,
#endif
                       the_system->qnode[WTI_QNODE_TRANS_TX].handle,
                       WP_CH_TX,
                       WP_TRANSPARENT_PWE3,
                       &trans_ch_config);
   WTI_TerminateOnError(the_system->pw[pw_index].trans_tx,
                        "WP_ChannelCreate trans Tx",
                        __LINE__);

   if(the_system->iw_system_mode == WTI_IW_SYSTEM_ROUTING /*default value of 0*/)
   {
      WTI_FlowAggregationPsn2TdmConfig(&psn2tdm_flow_agg_config, pw_index, oobc_mode);
   }
   else if(the_system->iw_system_mode == WTI_IW_SYSTEM_BRIDGE)
   {
      /* create PSN to TDM flow aggregation */
#if WTI_CESOP_MEF8_ENCAP
      WTI_FlowAggregationPsn2TdmBridgeConfigMef(&psn2tdm_flow_agg_config, pw_index, oobc_mode);
#else
      WTI_FlowAggregationPsn2TdmBridgeConfig(&psn2tdm_flow_agg_config, pw_index, oobc_mode);
#endif
   }

//#ifdef __WT_UFE3__
   /*configure the flow clock recovery and set the UFE CR master*/
#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
   if(clock_recovery_reconfig)
      WTI_Psn2TdmUfeClockRecSet_1(pw_index, clock_master);
   else
      WTI_Psn2TdmUfeClockRecSet_2(pw_index, clock_master);
#endif
//#endif /* WT_UFE4 */

#if WTI_INFO_STRUCTURES_DEFINED
   /* copy configuration structure to internal Demo information base*/
   memcpy(&(pw_info_base[pw_index].psn2tdm_agg_param),
          &psn2tdm_flow_agg_config,
          sizeof(WP_iw_agg_cesop));
#endif
   psn2tdm_flow_agg_config.tag = pw_index;

   the_system->pw[pw_index].psn2tdm_flow_agg =
      WP_IwFlowAggregationCreate(the_system->wpid,
                                 WP_IW_CESOP_MODE,
                                 &psn2tdm_flow_agg_config);
   WTI_TerminateOnError(the_system->pw[pw_index].psn2tdm_flow_agg,
                   "WP_IwFlowAggregationCreate PSN --> TDM",
                   __LINE__);

   /* create tx binding */
   WTI_CesopTxBindingConfig(&cesop_tx_binding_config);
#if WTI_INFO_STRUCTURES_DEFINED
   /* copy configuration structure to internal Demo information base*/
   memcpy(&pw_info_base[pw_index].cesop_tx_binding_config,
          &cesop_tx_binding_config,
          sizeof(WP_iw_tx_binding_cesop));
#endif

   status = WP_IwTxBindingCreate(the_system->pw[pw_index].trans_tx,
                                 WP_IW_TX_BINDING_CESOP,
                                 &cesop_tx_binding_config);
   WTI_TerminateOnError(status, "WP_IwTxBindingCreate PSN --> TDM", __LINE__);


#if WTI_PCE_CLASSIFIER

   if(the_system->iw_system_mode == WTI_IW_SYSTEM_ROUTING)
   {
      /* add PCE classification rule */
      WTI_PceRuleConfig(&class_cfg, pw_index);
#if WTI_INFO_STRUCTURES_DEFINED
      /* copy configuration structure to internal Demo information base*/
      memcpy(&pce_info_base[pw_index].class_cfg,
             &class_cfg,
             sizeof(WP_pce_rule_classification));
#endif

      the_system->pw[pw_index].pce_flow_handle = WP_PceRuleCreate(the_system->wpid,
                                                                  WP_PCE_RULE_CLASSIFICATION,
                                                                  &class_cfg);
      WTI_TerminateOnError(the_system->pw[pw_index].pce_flow_handle,
                           "WP_PceRuleCreate() class flow Route",
                           __LINE__);
   }
   else if(the_system->iw_system_mode == WTI_IW_SYSTEM_BRIDGE)
   {
      WTI_PceRuleConfigBridge(&class_cfg_bridge, pw_index);
#if WTI_INFO_STRUCTURES_DEFINED
      /* copy configuration structure to internal Demo information base*/
      memcpy(&pce_info_base[pw_index].class_cfg_bridge,
             &class_cfg_bridge,
             sizeof(WP_pce_rule_classification));
#endif
      the_system->pw[pw_index].pce_flow_handle = WP_PceRuleCreate(the_system->wpid,
                                                                  WP_PCE_RULE_CLASSIFICATION,
                                                                  &class_cfg_bridge);
      WTI_TerminateOnError(the_system->pw[pw_index].pce_flow_handle,
                           "WP_PceRuleCreate() class flow Bridge",
                           __LINE__);
   }

#else /*winpath*/
#if WTI_CESOP_MPLS_IW
   WTI_MplsFlowConfig(&mpls_flow_cfg, pw_index);
   the_system->pw[pw_index].mpls_flow_handle = WP_IwFlowAdd(the_system->psn2tdm_iw_system,
                                                            WP_FLOW_MPLS,
                                                            &mpls_flow_cfg);
   WTI_TerminateOnError(the_system->pw[pw_index].mpls_flow_handle,
                        "WP_IwFlowAdd() MPLS flow",
                        __LINE__);
#else /*DFC*/
   if(the_system->iw_system_mode == WTI_IW_SYSTEM_ROUTING /*default value of 0*/)
   {
      /* add DFC classification rule */
      WTI_DfcRuleConfig(&class_cfg, pw_index);
#if WTI_INFO_STRUCTURES_DEFINED
      /* copy configuration structure to internal Demo information base*/
      memcpy(&dfc_info_base[pw_index].class_cfg,
             &class_cfg,
             sizeof(WP_flow_class_rule));
#endif

      the_system->pw[pw_index].dfc_flow_handle = WP_IwFlowAdd(the_system->psn2tdm_iw_system,
                                                              WP_FLOW_CLASS_RULE,
                                                              &class_cfg);
      WTI_TerminateOnError(the_system->pw[pw_index].dfc_flow_handle,
                           "WP_IwFlowAdd() class flow Route",
                           __LINE__);
   }

   else if(the_system->iw_system_mode == WTI_IW_SYSTEM_BRIDGE)
   {

      /* add DFC classification rule */
#if WTI_CESOP_MEF8_ENCAP
      WTI_DfcRuleConfigBridgeMef(&class_cfg_bridge, pw_index);
#else
      WTI_DfcRuleConfigBridge(&class_cfg_bridge, pw_index);
#endif

#if WTI_INFO_STRUCTURES_DEFINED
      /* copy configuration structure to internal Demo information base*/
      memcpy(&dfc_info_base[pw_index].class_cfg_bridge,
             &class_cfg_bridge,
             sizeof(WP_flow_class_rule));
#endif
      the_system->pw[pw_index].dfc_flow_handle = WP_IwFlowAdd(the_system->psn2tdm_iw_system,
                                                              WP_FLOW_CLASS_RULE,
                                                              &class_cfg_bridge);
      WTI_TerminateOnError(the_system->pw[pw_index].dfc_flow_handle,
                           "WP_IwFlowAdd() class flow Bridge",
                           __LINE__);
   }
#endif /*MPLS*/
#endif /*Winpath*/


   /* build the IW system PSN --> TDM */
   status = WP_IwSystemBuild(the_system->psn2tdm_iw_system);
   WTI_TerminateOnError(status, "WP_IwSystemBuild PSN ----> TDM",__LINE__);

}

/*****************************************************************************
 * Function name: WTI_PwDelete
 * Description  : Delete a single PW.
 * Input  params: index - PW index
 * Output params:
 * Return val   : none
 *****************************************************************************/
void WTI_PwDelete(WP_U32 index)
{
   /* delete classification rule, disable and delete transparent Tx channel,
      delete flow aggregation in the PSN to TDM direction. */
   WTI_Psn2TdmFlowDelete(index);

   /* disable transparent Rx channel, delete flow aggregation and
      delete transparent Rx channel in the TDM to PSN direction */
   WTI_Tdm2PsnFlowDelete(index);

#if (!WTI_CESOP_TDI)
   /* If PW is created over UFE, disable the transparent device and phy */
   WTI_TransDeviceDisable(index);

   /* delete UFE phy and EMPHY transparent device */
   WTI_TransDeviceDelete(index);
#endif

   /* set PW state to not created */
   the_system->pw[index].state &= ~WTI_PW_CREATED;

   return;
}

/*****************************************************************************
 * Function name: WTI_Psn2TdmIwFlowRuleDisable
 * Description  : disable the classification rule in the PSN to TDM direction.
 * Input  params: index - PW index
 * Output params:
 * Return val   : none
 *****************************************************************************/
void WTI_Psn2TdmIwFlowRuleDisable(WP_U32 index)
{
   WP_status status;

#if WTI_PCE_CLASSIFIER
   status = WP_PceRuleDisable(the_system->pw[index].pce_flow_handle);
   WTI_TerminateOnError(status,
                        "WP_PceRuleDisable PCE flow", __LINE__);
#else
#if WTI_CESOP_MPLS_IW
   /* disable MPLS flow */
   status = WP_IwFlowDisable(the_system->psn2tdm_iw_system,
                             the_system->pw[index].mpls_flow_handle);
   WTI_TerminateOnError(the_system->pw[index].mpls_flow_handle,
                        "WP_IwFlowDisable MPLS flow", __LINE__);

#else
   /* disable DFC classification rule*/
   status = WP_IwFlowDisable(the_system->psn2tdm_iw_system,
                             the_system->pw[index].dfc_flow_handle);
   WTI_TerminateOnError(the_system->pw[index].dfc_flow_handle,
                        "WP_IwFlowDisable DFC flow", __LINE__);
#endif
#endif

   /* build the IW system PSN --> TDM */
   status = WP_IwSystemBuild(the_system->psn2tdm_iw_system);
   WTI_TerminateOnError(status, "WP_IwSystemBuild PSN ----> TDM",__LINE__);
}

/*****************************************************************************
 * Function name: WTI_Psn2TdmIwFlowRuleEnable
 * Description  : enable the classification rule in the PSN to TDM direction.
 * Input  params: index - PW index
 * Output params:
 * Return val   : none
 *****************************************************************************/
void WTI_Psn2TdmIwFlowRuleEnable(WP_U32 index)
{
   WP_status status;


#if WTI_PCE_CLASSIFIER
   status = WP_PceRuleEnable(the_system->pw[index].pce_flow_handle);
   WTI_TerminateOnError(status,
                        "WP_PceRuleEnable PCE flow", __LINE__);
#else
#if WTI_CESOP_MPLS_IW
   /* enable MPLS flow */
   status = WP_IwFlowEnable(the_system->psn2tdm_iw_system,
                             the_system->pw[index].mpls_flow_handle);
   WTI_TerminateOnError(the_system->pw[index].mpls_flow_handle,
                        "WP_IwFlowEnable MPLS flow",
                        __LINE__);

#else
   /* enable DFC classification rule*/
   status = WP_IwFlowEnable(the_system->psn2tdm_iw_system,
                             the_system->pw[index].dfc_flow_handle);
   WTI_TerminateOnError(the_system->pw[index].dfc_flow_handle,
                        "WP_IwFlowEnable DFC flow",
                        __LINE__);
#endif
#endif

   /* build the IW system PSN --> TDM */
   status = WP_IwSystemBuild(the_system->psn2tdm_iw_system);
   WTI_TerminateOnError(status, "WP_IwSystemBuild PSN ----> TDM",__LINE__);

}

/*****************************************************************************
 * Function name: WTI_Psn2TdmTxChannelCreate
 * Description  : Creates transparent tx channel and its tx binding
 * Input  params: index - PW index,
 * Output params:
 * Return val   : none
 *****************************************************************************/
void WTI_Psn2TdmTxChannelCreate(int pw_index, WP_handle emphy_port)
{
   WP_status status;

#if (!WTI_CESOP_TDI)
   WTI_TransDeviceCreate(pw_index, 0, emphy_port, &the_system->ufe, pw_config.line_index);
#endif
   /* create transparent Tx channel */
   WTI_TransChannelTxConfig(&trans_ch_config);
#if WTI_INFO_STRUCTURES_DEFINED
   pw_info_base[pw_index].tx_pwe3_channel_tag = pw_index;
   /* copy configuration structure to internal Demo information base*/
   memcpy(&pw_info_base[pw_index].trans_tx_ch_config,
          &trans_ch_config,
          sizeof(WP_ch_trans_pwe3));
#endif
   the_system->pw[pw_index].trans_tx =
      WP_ChannelCreate(pw_index,
#if WTI_CESOP_TDI
                       the_system->tdm_trans_dev[pw_config.line_index],
#else
                       the_system->pw[pw_index].trans_device,
#endif
                       the_system->qnode[WTI_QNODE_TRANS_TX].handle,
                       WP_CH_TX,
                       WP_TRANSPARENT_PWE3,
                       &trans_ch_config);
   WTI_TerminateOnError(the_system->pw[pw_index].trans_tx,
                        "WP_ChannelCreate trans Tx",
                        __LINE__);

   /* create tx binding */
   WTI_CesopTxBindingConfig(&cesop_tx_binding_config);
#if WTI_INFO_STRUCTURES_DEFINED
   /* copy configuration structure to internal Demo information base*/
   memcpy(&pw_info_base[pw_index].cesop_tx_binding_config,
          &cesop_tx_binding_config,
          sizeof(WP_iw_tx_binding_cesop));
#endif
   status = WP_IwTxBindingCreate(the_system->pw[pw_index].trans_tx,
                                 WP_IW_TX_BINDING_CESOP,
                                 &cesop_tx_binding_config);
   WTI_TerminateOnError(status, "WP_IwTxBindingCreate PSN --> TDM", __LINE__);
}

/*****************************************************************************
 * Function name: WTI_Psn2TdmFlowDelete
 * Description  : delete classification rule, disable and delete transparent Tx channel,
 *                delete flow aggregation in the PSN to TDM direction
 * Input  params: index - PW index
 * Output params:
 * Return val   : none
 *****************************************************************************/
static void WTI_Psn2TdmFlowDelete(WP_U32 index)
{
   WP_status status;


#if WTI_PCE_CLASSIFIER
   /* disable PCE classification rule*/
   status = WP_PceRuleDisable(the_system->pw[index].pce_flow_handle);
   WTI_TerminateOnError(the_system->pw[index].pce_flow_handle,
                        "WP_PceRuleDisable PCE flow", __LINE__);

   /* delete PCE classification rule*/
   status = WP_PceRuleDelete(the_system->pw[index].pce_flow_handle);
   WTI_TerminateOnError(the_system->pw[index].pce_flow_handle,
                        "WP_PceRuleDelete PCE flow", __LINE__);
#else
#if WTI_CESOP_MPLS_IW
   /* disable MPLS flow */
   status = WP_IwFlowDisable(the_system->psn2tdm_iw_system,
                             the_system->pw[index].mpls_flow_handle);
   WTI_TerminateOnError(the_system->pw[index].mpls_flow_handle,
                        "WP_IwFlowDisable MPLS flow",
                        __LINE__);

   /* delete MPLS flow */
   status = WP_IwFlowDelete(the_system->psn2tdm_iw_system,
                             the_system->pw[index].mpls_flow_handle);
   WTI_TerminateOnError(the_system->pw[index].mpls_flow_handle,
                        "WP_IwFlowDelete MPLS flow",
                        __LINE__);
#else
   /* disable DFC classification rule*/
   status = WP_IwFlowDisable(the_system->psn2tdm_iw_system,
                             the_system->pw[index].dfc_flow_handle);
   WTI_TerminateOnError(the_system->pw[index].dfc_flow_handle,
                        "WP_IwFlowDisable DFC flow",
                        __LINE__);

   /* delete DFC classification rule*/
   status = WP_IwFlowDelete(the_system->psn2tdm_iw_system,
                             the_system->pw[index].dfc_flow_handle);
   WTI_TerminateOnError(the_system->pw[index].dfc_flow_handle,
                        "WP_IwFlowDelete DFC flow",
                        __LINE__);
#endif
#endif

   /* build the IW system PSN --> TDM */
   status = WP_IwSystemBuild(the_system->psn2tdm_iw_system);
   WTI_TerminateOnError(status, "WP_IwSystemBuild PSN ----> TDM",__LINE__);

   /* delete PSN-->TDM flow aggregation */
   status = WP_IwFlowAggregationDelete(the_system->pw[index].psn2tdm_flow_agg);
   WTI_TerminateOnError(status, "WP_IwFlowAggregationDelete PSN --> TDM ",__LINE__);

   /* delete the transparent Tx channel */
   status = WP_ChannelDelete(the_system->pw[index].trans_tx);
   WTI_TerminateOnError(status, "WP_ChannelDelete Transparent Tx channel",__LINE__);
}

/*****************************************************************************
 * Function name: WTI_Tdm2PsnFlowDelete
 * Description  : disable transparent Rx channel, delete flow aggregation and
 *                delete transparent Rx channel in the TDM to PSN direction
 * Input  params: index - PW index
 * Output params:
 * Return val   : none
 *****************************************************************************/
static void WTI_Tdm2PsnFlowDelete(WP_U32 index)
{
   WP_status status;

   /* delete TDM-->PSN flow aggregation */
   status = WP_IwFlowAggregationDelete(the_system->pw[index].tdm2psn_flow_agg);
   WTI_TerminateOnError(status, "WP_IwFlowAggregationDelete TDM --> PSN",__LINE__);

   /* delete transparent Rx channel */
   status = WP_ChannelDelete(the_system->pw[index].trans_rx);
   WTI_TerminateOnError(status, "WP_ChannelDelete Transparent Rx channel",__LINE__);
}

#if (!WTI_CESOP_TDI)
/*****************************************************************************
 * Function name: WTI_TransDeviceDelete
 * Description  : disable transparent device, delete transparent device and UFE phy.
 * Input  params: index - PW index
 * Output params:
 * Return val   : none
 *****************************************************************************/
static void WTI_TransDeviceDelete(WP_U32 index)
{
   WP_status status;

   /* delete transparent device */
   status = WP_DeviceDelete(the_system->pw[index].trans_device);
   WTI_TerminateOnError(status , "WP_DeviceDelete Transparent device",__LINE__);

   /* delete UFE phy */
   WT_UfePhyDelete(&the_system->ufe, index);
#if WTI_DUAL_EMPHY
   WT_UfePhyDelete(&the_system->second_ufe, index);
#endif

   /* update PW state */
   the_system->pw[index].state &= ~WTI_PW_UFE2_PHY_CREATED;
}

/*****************************************************************************
 * Function name: WTI_TransDeviceDisable
 * Description  : disable transparent device, delete transparent device and UFE phy.
 * Input  params: index - PW index
 * Output params:
 * Return val   : none
 *****************************************************************************/
static void WTI_TransDeviceDisable(WP_U32 index)
{
   WP_status status;
   /* If PW is created over UFE, disable the transparent device and phy */

   /* disable transparent device */
   status = WP_DeviceDisable(the_system->pw[index].trans_device, WP_DIRECTION_DUPLEX);
   WTI_TerminateOnError(status, "WTI_TransDeviceDisable Transparent device", __LINE__);

   /* disable UFE phy */
   WT_UfePhyDisable(&the_system->ufe, index);
#if WTI_DUAL_EMPHY
   WT_UfePhyDisable(&the_system->second_ufe, index);
#endif

#if 0
   if(index == 0)
   {
      status = WUFE_SystemDisplay(0);
      WTI_TerminateOnError(status, "WUFE_SystemCheckFbp", __LINE__);
   }
#endif

}
#endif

/*****************************************************************************
 * Function name: WTI_PwEnable
 * Description  : Enables transparent Rx and Tx channels
 * Input  params: pw_index - PW index
 * Output params:
 * Return val   : none
 *****************************************************************************/
void WTI_PwEnable(int pw_index)
{
   WP_status status;

   if (pw_index >= WTI_MAX_PW)
   {
      printf("Test Abort: Invalid PW index %d\n", pw_index);
      WTI_Terminate(1);
   }

   /* enable transparent PWE3 Tx channel */
   status = WP_ChannelEnable(the_system->pw[pw_index].trans_tx);
   WTI_TerminateOnError(status , "WP_ChannelEnable Transparent Tx channel",__LINE__);

   /* enable transparent PWE3 Rx channel */
      status = WP_ChannelEnable(the_system->pw[pw_index].trans_rx);
      WTI_TerminateOnError(status , "WP_ChannelEnable Transparent Rx channel",__LINE__);

   /* set PW state to WTI_PW_ACTIVE */
   the_system->pw[pw_index].state |= WTI_PW_ACTIVE;
}

/*****************************************************************************
 * Function name: WTI_PwDisable
 * Description  : Disables transparent Rx and Tx channels
 * Input  params: pw_index - PW index
 * Output params:
 * Return val   : none
 *****************************************************************************/
void WTI_PwDisable(int pw_index)
{
   WP_status status;

   if (pw_index >= WTI_MAX_PW)
   {
      printf("Test Abort: Invalid PW index %d\n", pw_index);
      WTI_Terminate(1);
   }

   /* disable transparent PWE3 Rx channel */
   status = WP_ChannelDisable(the_system->pw[pw_index].trans_rx);
   WTI_TerminateOnError(status , "WP_ChannelDisable Transparent Rx channel",__LINE__);

   /* Added for CES-Shaping - to give the system time to transmit any packets that are still in the calendar wheel */
   WP_Delay(5000);

   /* disable transparent PWE3 Tx channel */
   status = WP_ChannelDisable(the_system->pw[pw_index].trans_tx);
   WTI_TerminateOnError(status , "WP_ChannelDisable Transparent Tx channel",__LINE__);

   /* set PW state to not active */
   the_system->pw[pw_index].state &= ~WTI_PW_ACTIVE;
}

/*****************************************************************************
 * Function name: WTI_PortsEnable
 * Description  : enable upi and enet ports
 * Input  params: none
 * Output params:
 * Return val   : none
 *****************************************************************************/
void WTI_PortsEnable(void)
{
   WP_status status;

#if WTI_CESOP_TDI
   WP_U32 i, mode;

#if WTI_DEBUG_LEVEL > 0
   printf("______ Init COMET ___________\n");
/*
  Note that By PMC naming convention, MASTER mode means that the Comet expects
  external clock to be the master (The backplane is Tx timing MASTER) whereas SLAVE mode actually means that the Comet
  is the MASTER of the clock (The backplane is Tx timing SLAVE).
*/
#endif

#ifdef WP_HW_WINPATH3
   if (WTI_TDM_MODE != WP_TDM_LOOPBACK)
#endif
  {
#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
      if (tdm_config[0].op_mode == WTI_TDI_T1_OP_MODE) /* T1 */
      {
         status = WPU_TdmCometCardInit(WP_PORT_TDM1, WPU_INIT_COMET_CPLD_CR_T1);
         WTI_TerminateOnError(status, "WPU_TdmCometCardInit()", __LINE__);
         if (WTI_ACTIVE_TDI_PORTS > 8)
         {
            status = WPU_TdmCometCardInit(WP_PORT_TDM9, WPU_INIT_COMET_CPLD_CR_T1);
            WTI_TerminateOnError(status, "WPU_TdmCometCardInit()", __LINE__);
         }

      }
      else /* E1 */
      {

        status = WPU_TdmCometCardInit(WP_PORT_TDM1, WPU_INIT_COMET_CPLD_CR_E1);
        WTI_TerminateOnError(status, "WPU_TdmCometCardInit()", __LINE__);

        if (WTI_ACTIVE_TDI_PORTS > 8)
        {
            status = WPU_TdmCometCardInit(WP_PORT_TDM9, WPU_INIT_COMET_CPLD_CR_E1);
            WTI_TerminateOnError(status, "WPU_TdmCometCardInit()", __LINE__);
         }
      }

#else /* not in CR, config to regular mode */
      if (tdm_config[0].op_mode == WTI_TDI_T1_OP_MODE) /* T1 */
      {
         status = WPU_TdmCometCardInit(WP_PORT_TDM1, WPU_INIT_COMET_CPLD_T1);
         WTI_TerminateOnError(status, "WPU_TdmCometCardInit()", __LINE__);
         if (WTI_ACTIVE_TDI_PORTS > 8)
         {
            status = WPU_TdmCometCardInit(WP_PORT_TDM9, WPU_INIT_COMET_CPLD_T1);
            WTI_TerminateOnError(status, "WPU_TdmCometCardInit()", __LINE__);
         }
      }
      else /* E1 */
      {
         status = WPU_TdmCometCardInit(WP_PORT_TDM1, WPU_INIT_COMET_CPLD_E1);
         WTI_TerminateOnError(status, "WPU_TdmCometCardInit()", __LINE__);
         if (WTI_ACTIVE_TDI_PORTS > 8)
         {
            status = WPU_TdmCometCardInit(WP_PORT_TDM9, WPU_INIT_COMET_CPLD_E1);
            WTI_TerminateOnError(status, "WPU_TdmCometCardInit()", __LINE__);
         }
      }
#endif /* WTI_CESOP_CLOCK_RECOVERY_ENABLE */
   }

   /* Initilize the COMET/Quad Card */
   for (i=0;i<WTI_ACTIVE_TDI_PORTS;++i)
   {

      if (tdm_config[i].op_mode == WTI_TDI_T1_OP_MODE) /* T1 */
      {
         if (tdm_config[i].framing_mode == WTI_TDI_UNFRAMED_MODE) /* UNFRAMED */
         {
#if WTI_CESOP_REGRESSION_TEST
            mode = WPU_MODE_TDM_T1_SLAVE_UNFRAMED_PWE3_CES_CR;
#else
            mode = WPU_MODE_TDM_T1_MASTER_UNFRAMED_PWE3_CES_CR;
#endif
         }
         else /* FRAMED */
         {
            if (tdm_config[i].cas_mode == WTI_TDI_CAS_MODE)   /* CAS MODE */
            {
#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
               mode = WPU_MODE_TDM_T1_CR_CAS_MASTER_PWE3_CES_CR;
#elif WTI_CESOP_REGRESSION_TEST
               mode = WPU_MODE_TDM_TRANS_T1_CAS_SLAVE_PWE3_CES_CR;
#else
               mode = WPU_MODE_TDM_TRANS_T1_CAS_MASTER_PWE3_CES_CR;
#endif
            }
            else
            {
#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
               mode = WPU_MODE_TDM_T1_CR_MASTER_PWE3_CES_CR;
#elif WTI_CESOP_REGRESSION_TEST
               mode = WPU_MODE_TDM_T1_SLAVE_PWE3_CES_CR;
#else
               mode = WPU_MODE_TDM_T1_MASTER_PWE3_CES_CR;
#endif
            }
         }
      }
      else /* E1 */
      {
         if (tdm_config[i].framing_mode == WTI_TDI_UNFRAMED_MODE) /* UNFRAMED */
         {
#if WTI_CESOP_REGRESSION_TEST
            mode = WPU_MODE_TDM_E1_SLAVE_UNFRAMED_PWE3_CES_CR;
#else
            mode = WPU_MODE_TDM_E1_MASTER_UNFRAMED_PWE3_CES_CR;
#endif
         }
         else /* FRAMED */
         {
            if (tdm_config[i].cas_mode == WTI_TDI_CAS_MODE)   /* CAS MODE */
            {
#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
               mode = WPU_MODE_TDM_E1_CR_CAS_MASTER_PWE3_CES_CR;
#elif WTI_CESOP_REGRESSION_TEST
               mode = WPU_MODE_TDM_TRANS_E1_CAS_SLAVE_PWE3_CES_CR;
#else
               mode = WPU_MODE_TDM_TRANS_E1_CAS_MASTER_PWE3_CES_CR;
#endif
            }
            else
            {
#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
               mode = WPU_MODE_TDM_E1_CR_MASTER_PWE3_CES_CR;
#elif WTI_CESOP_REGRESSION_TEST
               mode = WPU_MODE_TDM_E1_SLAVE_PWE3_CES_CR;
#else
               mode = WPU_MODE_TDM_E1_MASTER_PWE3_CES_CR;
#endif
            }
         }
      }

#ifdef WP_HW_WINPATH3
       if (WTI_TDM_MODE != WP_TDM_LOOPBACK)
#endif
      {
         status = WPU_TdmCometCardInit(WP_PORT_TDM1+i, mode);
         WTI_TerminateOnError(status, "WPU_TdmCometCardInit()", __LINE__);
      }

      /* create transparent port over TDM interface */
      WTI_TransPortOverTdmCreate(i);

      /* create transparent CES device over TDM port */
      WTI_TdiTransDeviceCreate(i);

      /* Enable the Tdm Port */
      status = WP_PortEnable(the_system->tdm_port[i], WP_DIRECTION_DUPLEX);
      WTI_TerminateOnError(status, "WP_PortEnable() trans_port",__LINE__);

      /* enable transparent device */
      status = WP_DeviceEnable(the_system->tdm_trans_dev[i], WP_DIRECTION_DUPLEX);
      WTI_TerminateOnError(status , "WP_DeviceEnable() trans device",__LINE__);
   }

#ifndef __linux__
#ifdef WP_HW_WINPATH2
   /* set muxing between TDM and Winnet */
   /* mux address on the WDS2 is 0xbd800040. only MSByte is relevant: xxxD-CBAx
      A,C are connected to UPI1 and B,D are connected to UPI2.
      A,B are SEL and C,D are TEST (see HW document for more information).
      To open mux #2 for TDI set: 0x95ff-ffff
      To open mux #1 for TDI set: 0x8bff-ffff
      To open both set: 0x9fff-ffff
      default value: 0x8bff-ffff
      This mux should be open for TDI with CR in differential mode only!
      Otherwise, contention may occur */
#if (WTI_CESOP_RX_TIMING_ENABLE & WTI_CLOCK_REC_MODE)
#if WTI_CLOCK_REC_TDI_LINES_4_7
   *(unsigned int *)0xbd800040 = 0x95ffffff; /* when working with upper 4 lines, open mux #2 */
#else
   *(unsigned int *)0xbd800040 = 0x8bffffff; /* when working with lower 4 lines, open mux #1 */
#endif /* WTI_CLOCK_REC_TDI_LINES_4_7 */
#else
   *(unsigned int *)0xbd800040 = 0x87ffffff;
#endif /* CR RX & diff mode */
#endif
#endif /* __linux__ */

#else /* not in TDI */
#ifdef WP_HW_WINPATH2
   /* In WP3 the UPI port is enabled right after system commit as the UFE must be initialized after UPI port is created AND enabled */
   /* Enable the Upi Port */
    status = WP_PortEnable(the_system->trans_port, WP_DIRECTION_DUPLEX);
    WTI_TerminateOnError(status, "WP_PortEnable() trans_port",__LINE__);
#endif
#endif

#if (WTI_CESOP_MPLS_IW && !WTI_CESOP_MPLS_OVER_ENET)
   /* Enable the HSPOS Port */
   status = WP_PortEnable(the_system->hspi_port, WP_DIRECTION_DUPLEX);
   WTI_TerminateOnError(status, "WP_PortEnable() hspi_port",__LINE__);
#else
   /* Enable the Enet Port */
   status = WP_PortEnable(the_system->enet_port, WP_DIRECTION_DUPLEX);
   WTI_TerminateOnError(status, "WP_PortEnable() enet_port",__LINE__);

   /* Enable the Enet Device */
   status = WP_DeviceEnable(the_system->enet_device, WP_DIRECTION_DUPLEX);
   WTI_TerminateOnError(status, "WP_DeviceEnable() enet_device",__LINE__);

   /* Enable the Enet Rx channel */
   status = WP_ChannelEnable(the_system->rx_enet_channel);
   WTI_TerminateOnError(status, "WP_ChannelEnable() rx_enet_channel",__LINE__);

   /* Enable the Enet Tx channel */
   status = WP_ChannelEnable(the_system->tx_enet_channel);
   WTI_TerminateOnError(status, "WP_ChannelEnable() tx_enet_channel",__LINE__);

#if WTI_EXT_DIFF_MODE_ACTIVE
   if (global_active_cr_async_mode > 0)
   {
      /* Enable the Enet Tx channel - high priority*/
      status = WP_ChannelEnable(the_system->tx_enet_channel_high_priority);
      WTI_TerminateOnError(status, "WP_ChannelEnable() tx_enet_channel_high_priority",__LINE__);
   }
#endif

#endif

#if (WTI_2_ENET_DEVICES)
   /* Enable the second Enet Port */
   status = WP_PortEnable(the_system->enet_port1, WP_DIRECTION_DUPLEX);
   WTI_TerminateOnError(status, "WP_PortEnable() second enet_port",__LINE__);

   /* Enable the Enet Device */
   status = WP_DeviceEnable(the_system->enet_device1, WP_DIRECTION_DUPLEX);
   WTI_TerminateOnError(status, "WP_PortEnable() enet_device1",__LINE__);

   /* Enable the second Enet Rx channel */
   status = WP_ChannelEnable(the_system->rx_enet_channel1);
   WTI_TerminateOnError(status, "WP_ChannelEnable() rx_enet_channel1",__LINE__);

   /* Enable the second Enet Tx channel */
   status = WP_ChannelEnable(the_system->tx_enet_channel1);
   WTI_TerminateOnError(status, "WP_ChannelEnable() tx_enet_channel1",__LINE__);

#if WTI_EXT_DIFF_MODE_ACTIVE
   if (global_active_cr_async_mode > 0)
   {
      /* Enable the second Enet Tx channel - high priority*/
      status = WP_ChannelEnable(the_system->tx_enet_channel1_high_priority);
      WTI_TerminateOnError(status, "WP_ChannelEnable() tx_enet_channel1_high_priority",__LINE__);
   }
#endif

#endif

#if (WTI_4_ENET_DEVICES)
   /* Enable the third Enet Port */
   status = WP_PortEnable(the_system->enet_port2, WP_DIRECTION_DUPLEX);
   WTI_TerminateOnError(status, "WP_PortEnable() third enet_port",__LINE__);

   /* Enable the third Enet Device */
   status = WP_DeviceEnable(the_system->enet_device2, WP_DIRECTION_DUPLEX);
   WTI_TerminateOnError(status, "WP_PortEnable() enet_device2",__LINE__);

   /* Enable the third Enet Rx channel */
   status = WP_ChannelEnable(the_system->rx_enet_channel2);
   WTI_TerminateOnError(status, "WP_ChannelEnable() rx_enet_channel2",__LINE__);

   /* Enable the third Enet Tx channel */
   status = WP_ChannelEnable(the_system->tx_enet_channel2);
   WTI_TerminateOnError(status, "WP_ChannelEnable() tx_enet_channel2",__LINE__);

#if WTI_EXT_DIFF_MODE_ACTIVE
   if (global_active_cr_async_mode > 0)
   {
      /* Enable the third Enet Tx channel - high priority*/
      status = WP_ChannelEnable(the_system->tx_enet_channel2_high_priority);
      WTI_TerminateOnError(status, "WP_ChannelEnable() tx_enet_channel2_high_priority",__LINE__);
   }
#endif

   /* Enable the fourth Enet Port */
   status = WP_PortEnable(the_system->enet_port3, WP_DIRECTION_DUPLEX);
   WTI_TerminateOnError(status, "WP_PortEnable() fourth enet_port",__LINE__);

   /* Enable the fourth Enet Device */
   status = WP_DeviceEnable(the_system->enet_device3, WP_DIRECTION_DUPLEX);
   WTI_TerminateOnError(status, "WP_PortEnable() enet_device3",__LINE__);

   /* Enable the fourth Enet Rx channel */
   status = WP_ChannelEnable(the_system->rx_enet_channel3);
   WTI_TerminateOnError(status, "WP_ChannelEnable() rx_enet_channel3",__LINE__);

   /* Enable the fourth Enet Tx channel */
   status = WP_ChannelEnable(the_system->tx_enet_channel3);
   WTI_TerminateOnError(status, "WP_ChannelEnable() tx_enet_channel3",__LINE__);

#if WTI_EXT_DIFF_MODE_ACTIVE
   if (global_active_cr_async_mode > 0)
   {
      /* Enable the fourth Enet Tx channel - high priority*/
      status = WP_ChannelEnable(the_system->tx_enet_channel3_high_priority);
      WTI_TerminateOnError(status, "WP_ChannelEnable() tx_enet_channel3_high_priority",__LINE__);
   }
#endif

#endif


   /* delay after enet port enable in order to allow auto-negotiation
      to complete */
   WP_Delay(1500000);
}

/*****************************************************************************
 * Function name: WTI_HostRxChannelsCreate
 * Description  : create Host Rx channels for violation traffic
 * Input  params: none
 * Output params:
 * Return val   : none
 *****************************************************************************/
void WTI_HostRxChannelsCreate(void)
{
   WP_status status;
   WP_U16 channel_tag = 0;
   WP_U32 i;

   the_system->iw_host_device = WP_DeviceCreate(the_system->iw_host_port, 0, WP_DEVICE_IW_HOST, NULL);
   WTI_TerminateOnError(the_system->iw_host_device, "WP_DeviceCreate() host device",__LINE__);

   WTI_HostRxChannelConfig(&tx_binding_config, &iw_host_ch_config);

   for (i=0;i<N_HOST_RX_CHANNELS;++i)
   {
      channel_tag = i;

      the_system->rx_host_channels[i] =
         WP_ChannelCreate(channel_tag,
                          the_system->iw_host_device,
                          the_system->qnode[WTI_QNODE_IW_PSN2TDM].handle,
                          WP_CH_RX,
                          WP_IW_HOST,
                          &iw_host_ch_config);
      WTI_TerminateOnError(the_system->rx_host_channels[i],
                           "WP_ChannelCreate host channel",__LINE__);

      status = WP_ChannelEnable(the_system->rx_host_channels[i]);
      WTI_TerminateOnError(status, "WP_ChannelEnable host channel",__LINE__);
   }
}

/*****************************************************************************
 * Function name: WTI_HostQnodeCreate
 * Description  : configure and create Host Qnode
 * Input  params: qnode - pointer to WTI_qnode structure which holds Qnode handles
 * Output params:
 * Return val   : none
 *****************************************************************************/
void WTI_HostQnodeCreate(WTI_qnode *qnode)
{
   WP_pool_buffer_data data_pool_cfg;
   WP_pool_ring_data ring_pool_cfg;
   WP_qnode_hostq hostq_cfg;
   WP_handle handle;

   memset(&data_pool_cfg, 0, sizeof(WP_pool_buffer_data));
   data_pool_cfg.n_buffers = the_system->qnode_config[WTI_QNODE_HOST].num_buffers;
   data_pool_cfg.offset = 0;
   data_pool_cfg.size = the_system->qnode_config[WTI_QNODE_HOST].buffer_size;
   data_pool_cfg.pad = 0;
   data_pool_cfg.bus = WP_BUS_HOST;
   data_pool_cfg.bank = APP_BANK_HOST;

   /* Create the Host Buffer pool(s) */
   handle = WP_PoolCreate(the_system->wpid, WP_pool_buffer, &data_pool_cfg);
   WTI_TerminateOnError(handle, "WP_PoolCreate() Host Buffers",__LINE__);
   qnode->adjunct_pool = handle;

   /* Create rings */
   memset(&ring_pool_cfg, 0, sizeof(WP_pool_ring_data));
   ring_pool_cfg.n_rings = N_HOST_QUEUE_RINGS;
   ring_pool_cfg.ring_length = N_HOST_QUEUE_RING_LENGTH;
#if WTI_8K_CH_SETUP
   ring_pool_cfg.bus = WP_BUS_HOST;
   ring_pool_cfg.bank = APP_BANK_HOST;
#else

#if WTI_CESOP_TDI
#ifdef WP_BOARD_WDS3_SL
  ring_pool_cfg.bus = WP_BUS_HOST;
   ring_pool_cfg.bank = APP_BANK_HOST;
#else   /* NOT  WP_BOARD_WDS3_SL */
   ring_pool_cfg.bus = WP_BUS_PACKET;
   ring_pool_cfg.bank = APP_BANK_PACKET;
#endif  /* NOT  WP_BOARD_WDS3_SL */


#else
   ring_pool_cfg.bus = WP_BUS_PARAM;
   ring_pool_cfg.bank = APP_BANK_PARAM;
#endif
#endif /* WTI_8K_CH_SETUP */

   handle = WP_PoolCreate(the_system->wpid, WP_pool_ring, &ring_pool_cfg);
   WTI_TerminateOnError(handle, "WP_PoolCreate() Host Rings",__LINE__);

   /* Create Qnode for Host queue */
   memset(&hostq_cfg, 0, sizeof(WP_qnode_hostq));
   hostq_cfg.pool_buffer = qnode->adjunct_pool;
   hostq_cfg.pool_ring = handle;
   hostq_cfg.interruptqueue = WTI_PSN2TDM_INTERRUPT_QUEUE;
   handle = WP_QNodeCreate(the_system->wpid, WP_QNODE_HOSTQ, &hostq_cfg);
   WTI_TerminateOnError(handle, "WP_QNodeCreate() HOSTQ",__LINE__);
   qnode->handle = handle;

#if WTI_DEBUG_LEVEL > 0
   printf("HostQ num_buffers = %d size = %d\n",
          the_system->qnode_config[WTI_QNODE_HOST].num_buffers,
          the_system->qnode_config[WTI_QNODE_HOST].buffer_size);
#endif
}

/*****************************************************************************
 * Function name: WTI_IwPsn2TdmQnodeCreate
 * Description  : configure and create PSN to TDM IW Qnode
 * Input  params: qnode - pointer to WTI_qnode structure which holds Qnode handles
 * Output params:
 * Return val   : none
 *****************************************************************************/
void WTI_IwPsn2TdmQnodeCreate(WTI_qnode *qnode)
{
   WP_pool_buffer_data data_pool_cfg;
   WP_qnode_iwq iwq_cfg;
   WP_handle handle;

   memset(&data_pool_cfg, 0, sizeof(WP_pool_buffer_data));

   data_pool_cfg.n_buffers = N_IW_PSN2TDM_QUEUE_ADJUNCT_POOL_BUFFERS;
   data_pool_cfg.offset = 64; /* must be offset > = gap */
   data_pool_cfg.size = the_system->qnode_config[WTI_QNODE_IW_PSN2TDM].buffer_size;
   data_pool_cfg.pad = 0;
   data_pool_cfg.bus = WTI_CESOP_BUS;
   data_pool_cfg.bank = WTI_CESOP_BANK;

   handle = WP_PoolCreate(the_system->wpid, WP_pool_iwbuffer, &data_pool_cfg);
   WTI_TerminateOnError(handle, "WP_PoolCreate() IW PSN2TDM adjunct pool buffers",__LINE__);
   qnode->adjunct_pool = handle;

   memset(&iwq_cfg, 0, sizeof(WP_qnode_iwq));
   iwq_cfg.num_buffers = the_system->qnode_config[WTI_QNODE_IW_PSN2TDM].num_buffers;
   iwq_cfg.adjunct_pool = handle;
   iwq_cfg.interruptqueue = WTI_PSN2TDM_INTERRUPT_QUEUE;

#ifdef WP_HW_WINPATH2
   handle = WP_QNodeCreate(the_system->wpid, WP_QNODE_IWQ | WP_QNODE_OPT_DEDICATED_RX_HWQ| WP_QNODE_OPT_FMU, &iwq_cfg);
#else
   handle = WP_QNodeCreate(the_system->wpid, WP_QNODE_IWQ |  WP_QNODE_OPT_FMU, &iwq_cfg);
#endif
   WTI_TerminateOnError(handle, "WP_QNodeCreate() IW PSN2TDM queue ",__LINE__);
   qnode->handle = handle;

#if WTI_DEBUG_LEVEL > 0
   printf("IW PSN2TDM num_buffers = %d size = %d\n",
          the_system->qnode_config[WTI_QNODE_IW_PSN2TDM].num_buffers,
          the_system->qnode_config[WTI_QNODE_IW_PSN2TDM].buffer_size);
#endif
}

/*****************************************************************************
 * Function name: WTI_IwTdm2PsnQnodeCreate
 * Description  : configure and create TDM to PSN IW Qnode
 * Input  params: qnode - pointer to WTI_qnode structure which holds Qnode handles
 * Output params:
 * Return val   : none
 *****************************************************************************/
void WTI_IwTdm2PsnQnodeCreate(WTI_qnode *qnode)
{
   WP_pool_buffer_data data_pool_cfg;
   WP_qnode_iwq iwq_cfg;
   WP_handle handle;

   memset(&data_pool_cfg, 0, sizeof(WP_pool_buffer_data));

   data_pool_cfg.n_buffers = N_IW_TDM2PSN_QUEUE_ADJUNCT_POOL_BUFFERS;
   data_pool_cfg.offset = 64; /* 64 is the buffer gap */
   data_pool_cfg.size = the_system->qnode_config[WTI_QNODE_IW_TDM2PSN].buffer_size;
   data_pool_cfg.pad = 0;

#ifdef WP_BOARD_WDS3_SL
   data_pool_cfg.bus = WP_BUS_HOST;
   data_pool_cfg.bank = APP_BANK_HOST;
#else  /* NOT  WP_BOARD_WDS3_SL */
   data_pool_cfg.bus = WP_BUS_PACKET;
   data_pool_cfg.bank = APP_BANK_PACKET;
#endif  /* NOT WP_BOARD_WDS3_SL */

   handle = WP_PoolCreate(the_system->wpid, WP_pool_iwbuffer, &data_pool_cfg);
   WTI_TerminateOnError(handle, "WP_PoolCreate() IW TDM2PSN adjunct pool buffers",__LINE__);
   qnode->adjunct_pool = handle;
   memset(&iwq_cfg, 0, sizeof(WP_qnode_iwq));
   iwq_cfg.num_buffers = the_system->qnode_config[WTI_QNODE_IW_TDM2PSN].num_buffers;
   iwq_cfg.adjunct_pool = handle;
   iwq_cfg.interruptqueue = WTI_PSN2TDM_INTERRUPT_QUEUE;

   handle = WP_QNodeCreate(the_system->wpid, WP_QNODE_IWQ | WP_QNODE_OPT_FMU, &iwq_cfg);

   WTI_TerminateOnError(handle, "WP_QNodeCreate() IW TDM2PSN queue",__LINE__);
   qnode->handle = handle;
#if WTI_DEBUG_LEVEL > 0
   printf("IW TDM2PSN num_buffers = %d size = %d\n",
          the_system->qnode_config[WTI_QNODE_IW_TDM2PSN].num_buffers,
          the_system->qnode_config[WTI_QNODE_IW_TDM2PSN].buffer_size);
#endif
}

/*****************************************************************************
 * Function name: WTI_TransTxQnodeCreate
 * Description  : configure and create Host Qnode for transparent Tx
 * Input  params: qnode - pointer to WTI_qnode structure which holds Qnode handles
 * Output params:
 * Return val   : none
 *****************************************************************************/
void WTI_TransTxQnodeCreate(WTI_qnode *qnode)
{
   WP_pool_buffer_data data_pool_cfg;
   WP_pool_ring_data ring_pool_cfg;
   WP_qnode_hostq hostq_cfg;
   WP_handle handle;

   memset(&data_pool_cfg, 0, sizeof(WP_pool_buffer_data));
   data_pool_cfg.n_buffers = the_system->qnode_config[WTI_QNODE_TRANS_TX].num_buffers;
   data_pool_cfg.offset = 0;
   /* buffer size and bus must be the same as for IW Qnode in the PSN2TDM direction */
   data_pool_cfg.size = the_system->qnode_config[WTI_QNODE_TRANS_TX].buffer_size;
   data_pool_cfg.pad = 0;

#if WTI_CESOP_TDI
   data_pool_cfg.bus = WTI_TX_DATA_BUS_TYPE;
   data_pool_cfg.bank = WTI_TX_DATA_BUS_BANK;
#else /* NOT WTI_CESOP_TDI */

#ifdef WP_BOARD_WDS3_SL
   data_pool_cfg.bus = WP_BUS_PARAM;
   data_pool_cfg.bank = WP_BUS_PARAM;
#else   /* NOT  WP_BOARD_WDS3_SL */
   data_pool_cfg.bus = WP_BUS_PACKET;
   data_pool_cfg.bank = APP_BANK_PACKET;
#endif  /* NOT WP_BOARD_WDS3_SL */

#endif /* NOT WTI_CESOP_TDI */

   /* Create the Host Buffer pool(s) */
   handle = WP_PoolCreate(the_system->wpid, WP_pool_iwbuffer, &data_pool_cfg);
   WTI_TerminateOnError(handle, "WP_PoolCreate() Trans Tx Buffers",__LINE__);
   qnode->adjunct_pool = handle;

   /* Create rings */
   memset(&ring_pool_cfg, 0, sizeof(WP_pool_ring_data));
   ring_pool_cfg.n_rings = N_TRANS_TX_QUEUE_RINGS;
   ring_pool_cfg.ring_length = N_TRANS_TX_QUEUE_RING_LENGTH;
#if WTI_CESOP_TDI
   ring_pool_cfg.bus = WTI_TX_DATA_BUS_TYPE;
   ring_pool_cfg.bank = WTI_TX_DATA_BUS_BANK;
#else

#if WTI_8K_CH_SETUP
   ring_pool_cfg.bus = WP_BUS_PARAM;
   ring_pool_cfg.bank = APP_BANK_PARAM;
#else

   ring_pool_cfg.bus = WP_BUS_PARAM;
   ring_pool_cfg.bank = APP_BANK_PARAM;
#endif /* WTI_8K_CH_SETUP */

#endif

   handle = WP_PoolCreate(the_system->wpid, WP_pool_ring, &ring_pool_cfg);
   WTI_TerminateOnError(handle, "WP_PoolCreate() Trans Tx Rings",__LINE__);

   /* Create Qnode for Host queue */
   memset(&hostq_cfg, 0, sizeof(WP_qnode_hostq));
   hostq_cfg.pool_buffer = qnode->adjunct_pool;
   hostq_cfg.pool_ring = handle;
   hostq_cfg.interruptqueue = WTI_PSN2TDM_INTERRUPT_QUEUE;
   handle = WP_QNodeCreate(the_system->wpid, WP_QNODE_HOSTQ, &hostq_cfg);
   WTI_TerminateOnError(handle, "WP_QNodeCreate() HOSTQ",__LINE__);
   qnode->handle = handle;

#if WTI_DEBUG_LEVEL > 0
   printf("Trans Tx queue num_buffers = %d size = %d\n",
          the_system->qnode_config[WTI_QNODE_TRANS_TX].num_buffers,
          the_system->qnode_config[WTI_QNODE_TRANS_TX].buffer_size);
#endif
}

/*****************************************************************************
 * Function name: WTI_QnodesCreate
 * Description  : configure and create Qnodes
 * Input  params: none
 * Output params:
 * Return val   : none
 *****************************************************************************/
void WTI_QnodesCreate(void)
{
   /* Create Queue Node for Ethernet and ATM host queue */
   WTI_HostQnodeCreate(&the_system->qnode[WTI_QNODE_HOST]);

   /* Create Queue Node for IW in the PSN2TDM direction */
   WTI_IwPsn2TdmQnodeCreate(&the_system->qnode[WTI_QNODE_IW_PSN2TDM]);

   /* Create Queue Node for IW in the TDM2PSN direction */
   WTI_IwTdm2PsnQnodeCreate(&the_system->qnode[WTI_QNODE_IW_TDM2PSN]);

   /* Create Queue Node for Transparent Tx queue */
   WTI_TransTxQnodeCreate(&the_system->qnode[WTI_QNODE_TRANS_TX]);
}

#if (!WTI_CESOP_MPLS_IW) || (WTI_CESOP_MPLS_IW && WTI_CESOP_MPLS_OVER_ENET)
/*****************************************************************************
 * Function name: WTI_EnetPortConfig
 * Description  : configure the Enet port
 * Input  params: cfg_ptr - pointer to configuration structure
 * Output params:
 * Return val   : none
 *****************************************************************************/
void WTI_EnetPortConfig(WP_port_enet *cfg_ptr)
{
   memset(cfg_ptr, 0, sizeof(WP_port_enet));

   cfg_ptr->pkt_limits.max_tx_channels = N_ENET_TX_CHANNELS;
   cfg_ptr->pkt_limits.max_rx_channels = N_ENET_RX_CHANNELS;
   cfg_ptr->pkt_limits.pq_block_size = N_ENET_PQ_BLOCK_SIZE;
   cfg_ptr->pkt_limits.n_pq_blocks = N_ENET_PQ_BLOCKS;
   cfg_ptr->flowmode = WP_FLOWMODE_FAST ;
   cfg_ptr->interface_mode = WTI_ENET_TYPE;
   cfg_ptr->rx_iw_bkgnd = WP_IW_BKGND_NOT_USED;

}

/*****************************************************************************
 * Function name: WTI_EnetDeviceConfig
 * Description  : configure the Enet device
 * Input  params: cfg_ptr - pointer to configuration structure
 * Output params:
 * Return val   : none
 *****************************************************************************/
void WTI_EnetDeviceConfig(WP_device_enet *cfg_ptr, WP_device_enet_ex *ex_cfg_ptr)
{
   memset(cfg_ptr, 0, sizeof(WP_device_enet));
   memset(ex_cfg_ptr, 0, sizeof(WP_device_enet_ex));

#ifdef WP_HW_WINPATH2
   if (WTI_ENET_TYPE == WP_ENET_RGMII_1000)
      cfg_ptr->tx_bit_rate = 1000000000;
   else
      cfg_ptr->tx_bit_rate = 100000000;
#else /* WP_HW_WINPATH3 */
#if WTI_UFE_4_1_2
   if(WTI_ENET_TYPE == WP_ENET_XAUI)
   {
      cfg_ptr->tx_bit_rate = WP_TX_BIT_RATE_UNLIMITED;
   }  
   else
      cfg_ptr->tx_bit_rate = 1000000000;
#else
   cfg_ptr->tx_bit_rate = WP_TX_BIT_RATE_UNLIMITED;
#endif
#endif
   cfg_ptr->max_tx_channels = N_ENET_TX_CHANNELS;
   cfg_ptr->tx_maxsdu = 1536;
   cfg_ptr->operating_speed = WP_UNUSED;
   cfg_ptr->mac_addr[0] = 0xAA;
   cfg_ptr->mac_addr[1] = 0xBB;
   cfg_ptr->mac_addr[2] = 0xCC;
   cfg_ptr->mac_addr[3] = 0xDD;
   cfg_ptr->mac_addr[4] = 0xEE;
   cfg_ptr->mac_addr[5] = 0xFF;
#ifdef WP_HW_WINPATH2
   cfg_ptr->loopbackmode = WTI_ENET_MODE;
#else
   /* in WP3 the LOOPBACK is set at the SERDES */
   cfg_ptr->loopbackmode = WP_ENET_NORMAL;
#endif
   ex_cfg_ptr->rx_statmode = WP_ENABLE;
   ex_cfg_ptr->tx_statmode = WP_ENABLE;
   cfg_ptr->extended_params = NULL;
}

/*****************************************************************************
 * Function name: WTI_EnetChannelConfig
 * Description  : configure the Enet channel
 * Input  params: cfg_ptr - pointer to configuration structure
 * Output params:
 * Return val   : none
 *****************************************************************************/
void WTI_EnetChannelConfig(WP_ch_enet *cfg_ptr)
{
   memset(cfg_ptr, 0, sizeof(WP_ch_enet));

   cfg_ptr->intmode = WP_PKTCH_INT_ENABLE;
   cfg_ptr->iwmode = WP_PKTCH_IWM_ENABLE;
   cfg_ptr->testmode = WP_PKTCH_TEST_DISABLE;
   cfg_ptr->tx_pqblock = 0;
   cfg_ptr->tx_pqlevel = 0;
   cfg_ptr->tx_shaping_type = WP_FMU_SHAPING_TYPE_STRICT;
   cfg_ptr->tx_shaping_params = NULL;
   cfg_ptr->rx_maxsdu = 1536;
   cfg_ptr->tx_cwid = WP_UNUSED;
#if WTI_EXT_DIFF_MODE_ACTIVE
   if (global_active_cr_async_mode > 0)
      cfg_ptr->tx_tq = 1;   /* in ext. diff. mode set to low priority (adaptive is the high priority) */
   else
      cfg_ptr->tx_tq = 0;
#else
   cfg_ptr->tx_tq = 0;
#endif
   cfg_ptr->rx_queuedepth = 17;
}
/*****************************************************************************
 * Function name: WTI_EnetChannelConfigWithHighPriority
 * Description  : configure the Enet channel with high priority
 * Input  params: cfg_ptr - pointer to configuration structure
 * Output params:
 * Return val   : none
 *****************************************************************************/
void WTI_EnetChannelConfigWithHighPriority(WP_ch_enet *cfg_ptr)
{
   memset(cfg_ptr, 0, sizeof(WP_ch_enet));

   cfg_ptr->intmode = WP_PKTCH_INT_ENABLE;
   cfg_ptr->iwmode = WP_PKTCH_IWM_ENABLE;
   cfg_ptr->testmode = WP_PKTCH_TEST_DISABLE;
   cfg_ptr->tx_pqblock = 0;
   cfg_ptr->tx_pqlevel = 0;
   cfg_ptr->tx_shaping_type = WP_FMU_SHAPING_TYPE_STRICT;
   cfg_ptr->tx_shaping_params = NULL;
   cfg_ptr->rx_maxsdu = 1536;
   cfg_ptr->tx_cwid = WP_CW_ID_A;
   cfg_ptr->tx_tq = 0;   /* set to high priority */
   cfg_ptr->rx_queuedepth = 17;
}

/*****************************************************************************
 * Function name: WTI_EnetPortCreate
 * Description  : create the Enet port
 * Input  params: none
 * Output params:
 * Return val   : none
 *****************************************************************************/
void WTI_EnetPortCreate(void)
{
   WP_status status;

#ifdef WP_HW_WINPATH2
   if (WTI_ENET_TYPE == WP_ENET_RGMII_1000 && WTI_ENET_MODE != WP_ENET_NORMAL)
   {
      WTI_TerminateOnError(1,
                           "ERROR in WTI_EnetPortCreate. Do not work in internal loopback mode when Enet is Giga",
                           __LINE__);
   }

   if (WTI_ENET_MODE == WP_ENET_NORMAL)
   {
      if (WTI_ENET_TYPE == WP_ENET_RGMII_1000)
      {
         if (WTI_GMII_TYPE == WTI_GMII_OV_FIBER)
         {
            /* init ENET phy */
            status=WPX_EnetPhyInit(WTI_ENET_PORT, WPU_MODE_RGMII | WPU_OV_FIBER | WPU_ANEG_OFF);
            WTI_TerminateOnError(status, "WPX_EnetPhyInit WTI_ENET_PORT FIBER", __LINE__);
         }
         else /*WTI_GMII_TYPE == WTI_GMII_OV_COPPER */
         {
            /* init ENET phy */
            status=WPX_EnetPhyInit(WTI_ENET_PORT, WPU_MODE_RGMII | WPU_OV_COPPER | WPU_ANEG_OFF);
            WTI_TerminateOnError(status, "WPX_EnetPhyInit WTI_ENET_PORT COPPER", __LINE__);
         }
      }
      else /* WTI_SECOND_ENET_TYPE == WP_ENET_RGMII_10_100 */
      {
         if (WTI_GMII_TYPE == WTI_GMII_OV_COPPER)
         {
            /* init ENET phy */
            status=WPX_EnetPhyInit(WTI_ENET_PORT, WPU_MODE_RGMII | WPU_OV_COPPER );
            WTI_TerminateOnError(status, "WPX_EnetPhyInit WTI_ENET_PORT COPPER", __LINE__);
         }
         else /*WTI_GMII_TYPE == WTI_GMII_OV_COPPER */
         {
            WTI_TerminateOnError(1,
                                 "ERROR in WTI_EnetPortCreate. Do not use FIBER mode when 10/100 rate is used",
                                 __LINE__);
         }
      }
   }
#else /* WP_HW_WINPATH3 */
   if(WTI_ENET_TYPE == WP_ENET_XAUI)
   {     
      status = WPX_BoardXgiConfig(0, WTI_ENET_PORT);
      WTI_TerminateOnError(status, "WPX_BoardXgiConfig() WTI_ENET_PORT", __LINE__);
   }
   
#if (WTI_ENET_MODE == WP_ENET_NORMAL)
    status = WPX_BoardSerdesInit(the_system->wpid, WTI_ENET_PORT,WPX_SERDES_NORMAL);
    WTI_TerminateOnError(status, "WPX_BoardSerdesSetLoopback() WTI_ENET_PORT", __LINE__);
#else
   status = WPX_BoardSerdesInit(the_system->wpid, WTI_ENET_PORT,WPX_SERDES_LOOPBACK);
   WTI_TerminateOnError(status, "WPX_BoardSerdesSetLoopback() WTI_ENET_PORT", __LINE__);
#endif
#endif

   /* Create an Enet Port */
   WTI_EnetPortConfig(&enet_port_config);

   the_system->enet_port = WP_PortCreate(the_system->wpid, WTI_ENET_PORT, &enet_port_config);
   WTI_TerminateOnError(the_system->enet_port, "WP_PortCreate() Ethernet",__LINE__);

   /* Create an Enet Device */
   WTI_EnetDeviceConfig(&enet_device_config, &enet_device_ex_config);
   the_system->enet_device = WP_DeviceCreate(the_system->enet_port,
                                             0,
                                             WP_DEVICE_ENET, &enet_device_config);
   WTI_TerminateOnError(the_system->enet_device, "WP_DeviceCreate() enet device",__LINE__);

#ifdef WP_HW_WINPATH3
   enet_device_config.extended_params = &enet_device_ex_config;
   status = WP_DeviceModify(the_system->enet_device,
                            WP_DEV_MOD_ENET_TX_STATMODE |
                            WP_DEV_MOD_ENET_RX_STATMODE,
                            &enet_device_config);
   WTI_TerminateOnError(status, "WP_DeviceModify() enet device",__LINE__);
#endif

#if WTI_2_ENET_DEVICES

#ifdef WP_HW_WINPATH2
   /*config second enet port */
   if (WTI_ENET_MODE == WP_ENET_NORMAL)
   {
      if (WTI_SECOND_ENET_TYPE == WP_ENET_RGMII_1000)
      {
         if (WTI_SECOND_GMII_TYPE == WTI_GMII_OV_FIBER)
         {
            /* init ENET phy */
            status=WPX_EnetPhyInit(WTI_SECOND_ENET_PORT, WPU_MODE_RGMII | WPU_OV_FIBER );
            WTI_TerminateOnError(status, "WPX_EnetPhyInit WTI_SECOND_ENET_PORT FIBER", __LINE__);
         }
         else /*WTI_SECOND_GMII_TYPE == WTI_GMII_OV_COPPER */
         {
            /* init ENET phy */
            status=WPX_EnetPhyInit(WTI_SECOND_ENET_PORT, WPU_MODE_RGMII | WPU_OV_COPPER );
            WTI_TerminateOnError(status, "WPX_EnetPhyInit WTI_SECOND_ENET_PORT COPPER", __LINE__);
         }
      }
      else /* WTI_SECOND_ENET_TYPE == WP_ENET_RGMII_10_100 */
      {
         if (WTI_SECOND_GMII_TYPE == WTI_GMII_OV_COPPER)
         {
            /* init ENET phy */
            status=WPX_EnetPhyInit(WTI_SECOND_ENET_PORT, WPU_MODE_RGMII | WPU_OV_COPPER );
            WTI_TerminateOnError(status, "WPX_EnetPhyInit WTI_SECOND_ENET_PORT COPPER", __LINE__);
         }
         else /*WTI_SECOND_GMII_TYPE == WTI_GMII_OV_COPPER */
         {
            WTI_TerminateOnError(1,
                                 "ERROR in WTI_EnetPortCreate. Do not use FIBER mode when 10/100 rate is used",
                                 __LINE__);
         }
      }
   }
#else /* WP_HW_WINPATH3 */
#if (WTI_SECOND_ENET_MODE == WP_ENET_NORMAL)
   status = WPX_BoardSerdesInit(WP_WINPATH(DEFAULT_WPID), WTI_SECOND_ENET_PORT, WPX_SERDES_NORMAL);
   WTI_TerminateOnError(status, "WPX_BoardSerdesSetLoopback() WTI_SECOND_ENET_PORT", __LINE__);
#else
   status = WPX_BoardSerdesInit(WP_WINPATH(DEFAULT_WPID), WTI_SECOND_ENET_PORT,WPX_SERDES_LOOPBACK);
   WTI_TerminateOnError(status, "WPX_BoardSerdesSetLoopback() WTI_SECOND_ENET_PORT", __LINE__);
#endif
#endif

   /* Create second Enet Port and device for debugging purposes only*/
   WTI_EnetPortConfig(&enet_port_config);
   enet_port_config.interface_mode = WTI_SECOND_ENET_TYPE;
   the_system->enet_port1 = WP_PortCreate(the_system->wpid, WTI_SECOND_ENET_PORT, &enet_port_config);
   WTI_TerminateOnError(the_system->enet_port1, "WP_PortCreate() Ethernet second port",__LINE__);

   enet_device_config.extended_params = NULL;
   the_system->enet_device1 = WP_DeviceCreate(the_system->enet_port1,
                                              0,
                                              WP_DEVICE_ENET,
                                              &enet_device_config);
   WTI_TerminateOnError(the_system->enet_device1, "WP_DeviceCreate() enet second device",__LINE__);

#ifdef WP_HW_WINPATH3
   enet_device_config.extended_params = &enet_device_ex_config;
   status = WP_DeviceModify(the_system->enet_device1,
                            WP_DEV_MOD_ENET_TX_STATMODE |
                            WP_DEV_MOD_ENET_RX_STATMODE,
                            &enet_device_config);
   WTI_TerminateOnError(status, "WP_DeviceModify() enet device",__LINE__);
#endif

#endif /* WTI_2_ENET_DEVICES */

#if WTI_4_ENET_DEVICES

#if (WTI_THIRD_ENET_MODE == WP_ENET_NORMAL)
   status = WPX_BoardSerdesInit(WP_WINPATH(DEFAULT_WPID), WTI_THIRD_ENET_PORT, WPX_SERDES_NORMAL);
   WTI_TerminateOnError(status, "WPX_BoardSerdesSetLoopback() WTI_THIRD_ENET_PORT", __LINE__);
#else
   status = WPX_BoardSerdesInit(WP_WINPATH(DEFAULT_WPID), WTI_THIRD_ENET_PORT, WPX_SERDES_LOOPBACK);
   WTI_TerminateOnError(status, "WPX_BoardSerdesSetLoopback() WTI_THIRD_ENET_PORT", __LINE__);
#endif

   /* Create third Enet Port and device for debugging purposes only*/
   WTI_EnetPortConfig(&enet_port_config);
   enet_port_config.interface_mode = WTI_THIRD_ENET_TYPE;
   the_system->enet_port2 = WP_PortCreate(the_system->wpid, WTI_THIRD_ENET_PORT, &enet_port_config);
   WTI_TerminateOnError(the_system->enet_port2, "WP_PortCreate() Ethernet third port",__LINE__);

   enet_device_config.extended_params = NULL;
   the_system->enet_device2 = WP_DeviceCreate(the_system->enet_port2,
                                              0,
                                              WP_DEVICE_ENET,
                                              &enet_device_config);
   WTI_TerminateOnError(the_system->enet_device2, "WP_DeviceCreate() enet third device",__LINE__);

   enet_device_config.extended_params = &enet_device_ex_config;
   status = WP_DeviceModify(the_system->enet_device2,
                            WP_DEV_MOD_ENET_TX_STATMODE |
                            WP_DEV_MOD_ENET_RX_STATMODE,
                            &enet_device_config);
   WTI_TerminateOnError(status, "WP_DeviceModify() third enet device",__LINE__);

#if (WTI_FOURTH_ENET_MODE == WP_ENET_NORMAL)
   status = WPX_BoardSerdesInit(WP_WINPATH(DEFAULT_WPID), WTI_FOURTH_ENET_PORT, WPX_SERDES_NORMAL);
   WTI_TerminateOnError(status, "WPX_BoardSerdesSetLoopback() WTI_FOURTH_ENET_PORT", __LINE__);
#else
   status = WPX_BoardSerdesInit(WP_WINPATH(DEFAULT_WPID), WTI_FOURTH_ENET_PORT, WPX_SERDES_LOOPBACK);
   WTI_TerminateOnError(status, "WPX_BoardSerdesSetLoopback() WTI_FOURTH_ENET_PORT", __LINE__);
#endif

   /* Create fourth Enet Port and device for debugging purposes only*/
   WTI_EnetPortConfig(&enet_port_config);
   enet_port_config.interface_mode = WTI_FOURTH_ENET_TYPE;
   the_system->enet_port3 = WP_PortCreate(the_system->wpid, WTI_FOURTH_ENET_PORT, &enet_port_config);
   WTI_TerminateOnError(the_system->enet_port3, "WP_PortCreate() Ethernet fourth port",__LINE__);

   enet_device_config.extended_params = NULL;
   the_system->enet_device3 = WP_DeviceCreate(the_system->enet_port3,
                                              0,
                                              WP_DEVICE_ENET,
                                              &enet_device_config);
   WTI_TerminateOnError(the_system->enet_device3, "WP_DeviceCreate() enet fourth device",__LINE__);

   enet_device_config.extended_params = &enet_device_ex_config;
   status = WP_DeviceModify(the_system->enet_device3,
                            WP_DEV_MOD_ENET_TX_STATMODE |
                            WP_DEV_MOD_ENET_RX_STATMODE,
                            &enet_device_config);
   WTI_TerminateOnError(status, "WP_DeviceModify() fourth enet device",__LINE__);

#endif /* WTI_4_ENET_DEVICES */

}



#if WTI_CES_SHAPING_ENABLED
static void WTI_SchedulerCreate(void)
{
WP_status status;

   WP_calendar_wheel calendar[2] =
      {
         {
            /* function */    WP_CW_CES_SHAPING,
            /* cwid*/         WP_CW_ID_A,
            /* min_ch_rate */ 200, /* this is the minimal line rate allowed.
              						  it is reached in case of T1 1xDS0
              						  rate = line_rate * num_slots / (paylod_size * frame_size) */
            /* max_ch_rate */ 800000000,
         },
         WP_CALENDAR_WHEEL_TERM
      };



   status  = WP_SysSchedulerCreate(WP_WINPATH(DEFAULT_WPID), calendar);
   WTI_TerminateOnError(status,"WP_SysSchedulerCreate",__LINE__);
}
#endif  /* WTI_CES_SHAPING_MODE != 0 */

/*****************************************************************************
 * Function name: WTI_EnetChannelsCreate
 * Description  : create Rx and Tx Enet channels
 * Input  params: none
 * Output params:
 * Return val   : none
 *****************************************************************************/
void WTI_EnetChannelsCreate(void)
{

   /* Create Rx enet channel */
   WTI_EnetChannelConfig(&enet_rx_ch_config);
   the_system->rx_enet_channel = WP_ChannelCreate(0,
                                                  the_system->enet_device,
                                                  the_system->qnode[WTI_QNODE_IW_PSN2TDM].handle,
                                                  WP_CH_RX,
                                                  WP_ENET,
                                                  &enet_rx_ch_config);
   WTI_TerminateOnError(the_system->rx_enet_channel, "WP_ChannelCreate() Rx enet channel",__LINE__);

   /* Create Tx enet channel */
   WTI_EnetChannelConfig(&enet_tx_ch_config);

   the_system->tx_enet_channel = WP_ChannelCreate(0,
                                                  the_system->enet_device,
                                                  the_system->qnode[WTI_QNODE_IW_TDM2PSN].handle,
                                                  WP_CH_TX,
                                                  WP_ENET,
                                                  &enet_tx_ch_config);
   WTI_TerminateOnError(the_system->tx_enet_channel, "WP_ChannelCreate() Tx enet channel",__LINE__);


#if WTI_EXT_DIFF_MODE_ACTIVE
   if (global_active_cr_async_mode > 0)
   {
      /* Create high Priority Tx enet channel */
      WTI_EnetChannelConfigWithHighPriority(&enet_tx_ch_config);
      the_system->tx_enet_channel_high_priority = WP_ChannelCreate(0,
                                                                   the_system->enet_device,
                                                                   the_system->qnode[WTI_QNODE_IW_PSN2TDM].handle,
                                                                   WP_CH_TX,
                                                                   WP_ENET,
                                                                   &enet_tx_ch_config);
      WTI_TerminateOnError(the_system->tx_enet_channel_high_priority,
                           "WP_ChannelCreate() Tx enet channel high_priority",__LINE__);
   }
#endif

#if WTI_2_ENET_DEVICES
   /* Create Second Rx enet channel */
   WTI_EnetChannelConfig(&enet_rx_ch_config);
   the_system->rx_enet_channel1 = WP_ChannelCreate(0,
                                                   the_system->enet_device1,
                                                   the_system->qnode[WTI_QNODE_IW_PSN2TDM].handle,
                                                   WP_CH_RX,
                                                   WP_ENET,
                                                   &enet_rx_ch_config);
   WTI_TerminateOnError(the_system->rx_enet_channel1, "WP_ChannelCreate() Rx enet channel1",__LINE__);

   /* Create Tx enet channel */
   WTI_EnetChannelConfig(&enet_tx_ch_config);
   the_system->tx_enet_channel1 = WP_ChannelCreate(0,
                                                   the_system->enet_device1,
                                                   the_system->qnode[WTI_QNODE_IW_TDM2PSN].handle,
                                                   WP_CH_TX,
                                                   WP_ENET,
                                                   &enet_tx_ch_config);
   WTI_TerminateOnError(the_system->tx_enet_channel1, "WP_ChannelCreate() Tx enet channel1",__LINE__);

#if WTI_EXT_DIFF_MODE_ACTIVE
   if (global_active_cr_async_mode > 0)
   {
      /* Create second high Priority Tx enet channel */
      WTI_EnetChannelConfigWithHighPriority(&enet_tx_ch_config);
      the_system->tx_enet_channel1_high_priority = WP_ChannelCreate(0,
                                                                    the_system->enet_device1,
                                                                    the_system->qnode[WTI_QNODE_IW_PSN2TDM].handle,
                                                                    WP_CH_TX,
                                                                    WP_ENET,
                                                                    &enet_tx_ch_config);
      WTI_TerminateOnError(the_system->tx_enet_channel1_high_priority,
                           "WP_ChannelCreate() Tx enet channel1 high_priority",__LINE__);
   }
#endif
#endif

#if WTI_4_ENET_DEVICES
   /* Create Third Rx enet channel */
   WTI_EnetChannelConfig(&enet_rx_ch_config);
   the_system->rx_enet_channel2 = WP_ChannelCreate(0,
                                                   the_system->enet_device2,
                                                   the_system->qnode[WTI_QNODE_IW_PSN2TDM].handle,
                                                   WP_CH_RX,
                                                   WP_ENET,
                                                   &enet_rx_ch_config);
   WTI_TerminateOnError(the_system->rx_enet_channel2, "WP_ChannelCreate() Rx enet channel2",__LINE__);

   /* Create Third Tx enet channel */
   WTI_EnetChannelConfig(&enet_tx_ch_config);
   the_system->tx_enet_channel2 = WP_ChannelCreate(0,
                                                   the_system->enet_device2,
                                                   the_system->qnode[WTI_QNODE_IW_PSN2TDM].handle,
                                                   WP_CH_TX,
                                                   WP_ENET,
                                                   &enet_tx_ch_config);
   WTI_TerminateOnError(the_system->tx_enet_channel2, "WP_ChannelCreate() Tx enet channel2",__LINE__);

#if WTI_EXT_DIFF_MODE_ACTIVE
   if (global_active_cr_async_mode > 0)
   {
      /* Create second high Priority Tx enet channel */
      WTI_EnetChannelConfigWithHighPriority(&enet_tx_ch_config);
      the_system->tx_enet_channel2_high_priority = WP_ChannelCreate(0,
                                                                    the_system->enet_device2,
                                                                    the_system->qnode[WTI_QNODE_IW_PSN2TDM].handle,
                                                                    WP_CH_TX,
                                                                    WP_ENET,
                                                                    &enet_tx_ch_config);
      WTI_TerminateOnError(the_system->tx_enet_channel2_high_priority,
                           "WP_ChannelCreate() Tx enet channel2 high_priority",__LINE__);
   }
#endif
   /* Create Fourth Rx enet channel */
   WTI_EnetChannelConfig(&enet_rx_ch_config);
   the_system->rx_enet_channel3 = WP_ChannelCreate(0,
                                                   the_system->enet_device3,
                                                   the_system->qnode[WTI_QNODE_IW_PSN2TDM].handle,
                                                   WP_CH_RX,
                                                   WP_ENET,
                                                   &enet_rx_ch_config);
   WTI_TerminateOnError(the_system->rx_enet_channel3, "WP_ChannelCreate() Rx enet channel3",__LINE__);

   /* Create Fourth Tx enet channel */
   WTI_EnetChannelConfig(&enet_tx_ch_config);
   the_system->tx_enet_channel3 = WP_ChannelCreate(0,
                                                   the_system->enet_device3,
                                                   the_system->qnode[WTI_QNODE_IW_PSN2TDM].handle,
                                                   WP_CH_TX,
                                                   WP_ENET,
                                                   &enet_tx_ch_config);
   WTI_TerminateOnError(the_system->tx_enet_channel3, "WP_ChannelCreate() Tx enet channel3",__LINE__);

#if WTI_EXT_DIFF_MODE_ACTIVE
   if (global_active_cr_async_mode > 0)
   {
      /* Create second high Priority Tx enet channel */
      WTI_EnetChannelConfigWithHighPriority(&enet_tx_ch_config);
      the_system->tx_enet_channel3_high_priority = WP_ChannelCreate(0,
                                                                    the_system->enet_device3,
                                                                    the_system->qnode[WTI_QNODE_IW_PSN2TDM].handle,
                                                                    WP_CH_TX,
                                                                    WP_ENET,
                                                                    &enet_tx_ch_config);
      WTI_TerminateOnError(the_system->tx_enet_channel3_high_priority,
                           "WP_ChannelCreate() Tx enet channel3 high_priority",__LINE__);
   }
#endif

#endif
}
#endif

#if (WTI_CESOP_MPLS_IW && !WTI_CESOP_MPLS_OVER_ENET)
/*****************************************************************************
 * Function name: WTI_HspiPortCreate
 * Description  : create the HSPI port and device
 * Input  params: none
 * Output params:
 * Return val   : none
 *****************************************************************************/
void WTI_HspiPortCreate(void)
{
   WP_status status;

   /* Create the HSPI Port */
   WTI_HspiPortConfig(&hspi_port_config);

   the_system->hspi_port = WP_PortCreate(the_system->wpid,
                                         WP_PORT_UPI2,
                                         &hspi_port_config);
   WTI_TerminateOnError(the_system->hspi_port,
                        "WP_PortCreate() HSPOS",
                        __LINE__);

   status = WP_SysClockCreate(the_system->wpid, WT_BRG_UPI2, WP_BRG_SRC_BRGIN2, 2);
   WTI_TerminateOnError(status, "WP_SysClockCreate()",__LINE__);

   /* Create the HSPI Device */
   WTI_HspiDeviceConfig(&hspi_device_config);

   the_system->hspi_device = WP_DeviceCreate(the_system->hspi_port,
                                             0,
                                             WP_DEVICE_HSPOS,
                                             &hspi_device_config);
   WTI_TerminateOnError(the_system->hspi_device,
                        "WP_DeviceCreate() HSPI device",
                        __LINE__);
}

/*****************************************************************************
 * Function name: WTI_HspiPortConfig
 * Description  : configure the HSPI port
 * Input  params: cfg_ptr - pointer to configuration structure
 * Output params:
 * Return val   : none
 *****************************************************************************/
void WTI_HspiPortConfig(WP_port_upi *cfg_ptr)
{
   memset(cfg_ptr, 0, sizeof(WP_port_upi));

   cfg_ptr->upimode = WP_UPI_POS_HIGHSPEED;
#if WTI_MPLS_LOOPBACK
   cfg_ptr->loopback = WP_UPI_LOOPBACK;
#else
   cfg_ptr->loopback = WP_UPI_NORMAL;
#endif
   cfg_ptr->rx_characteristics = (WP_UPI_SINGLE  |
#if WTI_MPLS_LOOPBACK
                                  WP_UPI_SLAVE   |
#else
                                  WP_UPI_MASTER  |
#endif
                                  WP_UPI_PRT_ODD |
                                  WP_UPI_ROBIN   |
                                  WP_UPI_PRTCHK_OFF |
                                  WP_UPI_BUS_WIDTH_16);
   cfg_ptr->tx_characteristics = (WP_UPI_SINGLE  |
                                  WP_UPI_MASTER  |
                                  WP_UPI_PRT_ODD |
                                  WP_UPI_ROBIN   |
                                  WP_UPI_FAST_MODE |
                                  WP_UPI_PRTCHK_ON |
                                  WP_UPI_BUS_WIDTH_16);
   cfg_ptr->atm_ext = NULL;
   cfg_ptr->packet_ext = hspi_pkt_ext;
   cfg_ptr->iw_ext = NULL;
   cfg_ptr->emphy_ext = NULL;
}

/*****************************************************************************
 * Function name: WTI_HspiDeviceConfig
 * Description  : configure the HSPI device
 * Input  params: cfg_ptr - pointer to configuration structure
 * Output params:
 * Return val   : none
 *****************************************************************************/
void WTI_HspiDeviceConfig(WP_device_upi_hspos *cfg_ptr)
{
   memset(cfg_ptr, 0, sizeof(WP_device_upi_hspos));

   cfg_ptr->tx_bit_rate = 622000000;
   cfg_ptr->tx_statmode = WP_PKT_STAT_ENABLE;
   cfg_ptr->tx_intmode = WP_PKTCH_INT_DISABLE;
   cfg_ptr->tx_slave_addr = 0;
   cfg_ptr->tx_queuedepth[0] = 20;
   cfg_ptr->tx_queuedepth[1] = 20;
   cfg_ptr->tx_queuedepth[2] = 20;
   cfg_ptr->tx_queuedepth[3] = 20;
   cfg_ptr->tx_queuedepth[4] = 20;
   cfg_ptr->tx_queuedepth[5] = 20;
   cfg_ptr->tx_queuedepth[6] = 20;
   cfg_ptr->tx_queuedepth[7] = 20;
   cfg_ptr->rx_bit_rate = 622000000;
   cfg_ptr->rx_statmode = WP_PKT_STAT_ENABLE;
   cfg_ptr->rx_intmode = WP_PKTCH_INT_DISABLE;
   cfg_ptr->rx_queuedepth = 20;
   cfg_ptr->rx_slave_addr = 0;
   cfg_ptr->tx_tag = 0;
   cfg_ptr->rx_tag = 0;
}
#endif

#if WTI_CESOP_TDI
/*****************************************************************************
 * Function name: WTI_TdmPortConfig
 * Description  : configure TDM port
 * Input  params: cfg_ptr - pointer to configuration structure
 *                port_index - TDM port index
 * Output params:
 * Return val   : none
 *****************************************************************************/
static void WTI_TdmPortConfig(WP_port_tdm *cfg_ptr, WP_U32 port_index)
{
   /* clear TDM slots related to the port */
   memset(&tdm_slots[port_index][0], 0, sizeof(WP_tdm_slotgroup));
   memset(&tdm_slots[port_index][1], 0, sizeof(WP_tdm_slotgroup));
   memset(cfg_ptr, 0, sizeof(tdm_port_config));

   if (tdm_config[port_index].op_mode == WTI_TDI_E1_OP_MODE)
      tdm_slots[port_index][0].repeat = 4;
   else
      tdm_slots[port_index][0].repeat = 3;
   tdm_slots[port_index][0].size = 8;
   tdm_slots[port_index][1].repeat = 0;
   tdm_slots[port_index][1].size = 0;

   cfg_ptr->atm_limits.max_tx_channels = 0;
   cfg_ptr->atm_limits.max_rx_channels = 0;
   cfg_ptr->atm_limits.pq_block_size = 0;
   cfg_ptr->atm_limits.n_pq_blocks = 0;
   cfg_ptr->atm_limits.aal1_tx_channels = 0;
   cfg_ptr->atm_limits.aal1_rx_channels = 0;
   cfg_ptr->atm_limits.n_mq_blocks = 0;
   cfg_ptr->atm_limits.n_mq_pq_blocks = 0;
   cfg_ptr->atm_limits.emphy_devices = 0;
   cfg_ptr->pkt_limits.max_tx_channels = 0;
   cfg_ptr->pkt_limits.max_rx_channels = 0;
   cfg_ptr->pkt_limits.pq_block_size = 0;
   cfg_ptr->pkt_limits.n_pq_blocks = 0;
   cfg_ptr->pkt_limits.emphy_devices = 0;

   cfg_ptr->trans_limits.max_tx_channels[0] = 32;
   cfg_ptr->trans_limits.max_tx_channels[1] = 0;
   cfg_ptr->trans_limits.max_tx_channels[2] = 0;
   cfg_ptr->trans_limits.max_tx_channels[3] = 0;
   cfg_ptr->trans_limits.max_rx_channels[0] = 32;
   cfg_ptr->trans_limits.max_rx_channels[1] = 0;
   cfg_ptr->trans_limits.max_rx_channels[2] = 0;
   cfg_ptr->trans_limits.max_rx_channels[3] = 0;
   cfg_ptr->slot = tdm_slots[port_index];
   cfg_ptr->tdmmode = WTI_TDM_MODE;
#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
   cfg_ptr->pinconfig = WP_TDM_SEPARATE;
#else
   cfg_ptr->pinconfig = WP_TDM_COMMON;
#endif
   cfg_ptr->edge = WP_TDM_RISING;

   cfg_ptr->delay = WP_TDM_DELAY_1;

/*    We use The octal over ufe setups (external clock) ->        */
/* -> need to change the sync bit delay (importent in framed mode) */
   if (tdm_config[port_index].op_mode == WTI_TDI_E1_OP_MODE)
   {
      cfg_ptr->delay = WP_TDM_DELAY_3;
   }

   cfg_ptr->intmode = WTI_TDM_INT_MODE;
   cfg_ptr->rx_iw_bkgnd = WP_IW_BKGND_USED;

   if (tdm_config[port_index].framing_mode == WTI_TDI_FRAMED_MODE) /* FRAMED */
   {
      cfg_ptr->framemode = WP_TDM_FRAMED;
   }
   else /* UNFRAMED */
   {
      cfg_ptr->framemode = WP_TDM_FRAMELESS;
   }
}

/*****************************************************************************
 * Function name: WTI_TransPortOverTdmAlloc
 * Description  : Allocate transparent port over TDM interface
 * Input  params: port_index - TDM port index
 * Output params:
 * Return val   : none
 *****************************************************************************/
static void WTI_TransPortOverTdmAlloc(WP_U32 port_index)
{
   WP_port tdm_port;

   /* configure and allocate TDM port */
   WTI_TdmPortConfig(&tdm_port_config, port_index);
   tdm_port = WP_PORT_TDM1 + port_index;

   the_system->tdm_port[port_index] = WP_PortAlloc(the_system->wpid,
                                                   tdm_port,
                                                   &tdm_port_config);
   WTI_TerminateOnError(the_system->tdm_port[port_index], "WP_PortAlloc TDM Port",__LINE__);
}

/*****************************************************************************
 * Function name: WTI_TransPortOverTdmCreate
 * Description  : create the transparent port over TDM interface
 * Input  params: port_index - TDM port index
 * Output params:
 * Return val   : none
 *****************************************************************************/
static void WTI_TransPortOverTdmCreate(WP_U32 port_index)
{
   WP_port tdm_port;

   tdm_port = WP_PORT_TDM1 + port_index;

   /* configure and create TDM port as UFE */
   WTI_TdmPortConfig(&tdm_port_config, port_index);
   the_system->tdm_port[port_index] = WP_PortCreate(the_system->wpid,
                                                    tdm_port,
                                                    &tdm_port_config);
   WTI_TerminateOnError(the_system->tdm_port[port_index], "WP_PortCreate TDM Port",__LINE__);
}
#else

/*****************************************************************************
 * Function name: WTI_EmphyPortConfig
 * Description  : configure EMPHY port
 * Input  params: cfg_ptr - pointer to configuration structure
 * Output params:
 * Return val   : none
 *****************************************************************************/
void WTI_EmphyPortConfig(WP_port_upi *cfg_ptr)
{
   memset(cfg_ptr, 0, sizeof(WP_port_upi));
#ifdef __WT_UFE3__
   cfg_ptr->upimode = WP_UPI_UFE3;
   cfg_ptr->loopback = WP_UPI_NORMAL;
   cfg_ptr->rx_characteristics = (WP_UPI_SINGLE  |
                                  WP_UPI_MASTER  |
                                  WP_UPI_PRT_ODD |
                                  WP_UPI_FIXED   |
                                  WP_UPI_PRTCHK_OFF |
                                  WP_UPI_BUS_WIDTH_16);
   cfg_ptr->tx_characteristics = (WP_UPI_SINGLE  |
                                  WP_UPI_MASTER  |
                                  WP_UPI_PRT_ODD |
                                  WP_UPI_FIXED   |
                                  WP_UPI_PRTCHK_ON |
                                  WP_UPI_EMPHY2_13 |
                                  WP_UPI_BUS_WIDTH_16);
#else /*UFE4*/
#if WTI_UFE_4_1_2
   cfg_ptr->upimode = WP_UPI_UFE4;
   cfg_ptr->loopback = WP_UPI_NORMAL;
   cfg_ptr->rx_characteristics = (WP_UPI_SINGLE  |
                                  WP_UPI_MASTER  |
                                  WP_UPI_PRT_EVEN |
                                  WP_UPI_FIXED   |
                                  WP_UPI_PRTCHK_OFF |
                                  WP_UPI_BUS_WIDTH_16);
   cfg_ptr->tx_characteristics = (WP_UPI_SINGLE  |
                                  WP_UPI_MASTER  |
                                  WP_UPI_PRT_EVEN |
                                  WP_UPI_FIXED   |
                                  WP_UPI_PRTCHK_ON |
                                  WP_UPI_EMPHY2_13 |
                                  WP_UPI_BUS_WIDTH_16);
#else
   cfg_ptr->upimode = WP_UPI_UFE4;
   cfg_ptr->loopback = WP_UPI_NORMAL;
   cfg_ptr->rx_characteristics = (WP_UPI_SINGLE  |
                                  WP_UPI_MASTER  |
                                  WP_UPI_PRT_EVEN |
                                  WP_UPI_FIXED   |
                                  WP_UPI_PRTCHK_OFF |
                                  WP_UPI_BUS_WIDTH_32);
/*Note: Change in bus width requires change in pinmux (WPX_BoardConfigure)*/
   cfg_ptr->tx_characteristics = (WP_UPI_SINGLE  |
                                  WP_UPI_MASTER  |
                                  WP_UPI_PRT_EVEN |
                                  WP_UPI_FIXED   |
                                  WP_UPI_PRTCHK_ON |
                                  WP_UPI_EMPHY2_13 |
                                  WP_UPI_BUS_WIDTH_32);
#endif
#endif
   cfg_ptr->atm_ext = NULL;
   cfg_ptr->packet_ext = NULL;
   emphy_iw_config[0].ces_tdm_wpid = the_system->wpid;
   cfg_ptr->iw_ext = emphy_iw_config;
   cfg_ptr->emphy_ext = emphy_config;
}


/*****************************************************************************
 * Function name: WTI_TransPortCreate
 * Description  : create the transparent port
 * Input  params: none
 * Output params:
 * Return val   : none
 *****************************************************************************/
void WTI_TransPortCreate(void)
{
   WP_U32 brg;
   WP_status status;

   /* create UPI port as UFE */
   WTI_EmphyPortConfig(&upi_emphy_port_config);
   the_system->trans_port = WP_PortCreate(the_system->wpid,
                                          WTI_EMPHY_PORT,
                                          &upi_emphy_port_config);
   WTI_TerminateOnError(the_system->trans_port, "WP_PortCreate UPI EMPHY Transparent",__LINE__);

   brg = (WTI_EMPHY_PORT == WP_PORT_UPI2) ? WT_BRG_UPI2 : WT_BRG_UPI1;

#if (WP_HW_WINPATH2)
   /* working with system clock and devide it by 4: EMPHY freq = 300/4 = 75MHz */
   status = WP_SysClockCreate(the_system->wpid, brg, WP_BRG_SRC_SYSCLOCK, 2);
#else
   status = WP_SysClockCreate(the_system->wpid, brg, WP_BRG_SRC_BRGIN2, 2);
#endif

   WTI_TerminateOnError(status, "WP_SysClockCreate()",__LINE__);

#if WTI_DUAL_EMPHY
   /* create UPI port as UFE */
   WTI_EmphyPortConfig(&upi_emphy_port_config);
   the_system->second_trans_port = WP_PortCreate(the_system->wpid,
                                                 WTI_SECOND_EMPHY_PORT,
                                                 &upi_emphy_port_config);
   WTI_TerminateOnError(the_system->second_trans_port, "WP_PortCreate second UPI EMPHY Transparent",__LINE__);
#endif
}

/*****************************************************************************
 * Function name: WTI_UfeInit
 * Description  : configure, create and enable UFE system
 * Input  params: ufe - pointer to the WT_ufe structure which includes the UFE global
 *                      parameters
 *                upi_id - identifier of the UPI port in use
 * Output params:
 * Return val   : none
 *****************************************************************************/
void WTI_UfeInit(WT_ufe *ufe, WP_U32 upi_id, WP_U32 ufe_id)
{
   if (ufe->fpga_mode == WT_OCTAL)
   {
      ufe->max_lines = WTI_MAX_OCTAL_LINES;
      ufe->max_phys = WTI_MAX_OCTAL_LINES * 32;
   }
   else
   {
#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
#if defined __WT_UFE412__ || defined __WT_UFE448__
      ufe->system_clock_rec_enable = WUFE_SYSTEM_CLOCK_REC_ENABLE;
      if(WP_DISABLE == global_clock_rec_reg_dco_ref_clock)
      {
    	  the_system_clock_rec_params.xo_ref_clk = WUFE_DISABLE;
      }
      else
      {
    	  the_system_clock_rec_params.xo_ref_clk = global_clock_rec_reg_dco_ref_clock;
      }

      ufe->clk_rec_system_params = &the_system_clock_rec_params;
#else
      ufe->max_sbi_clock_translators = WTI_MAX_NUM_CLOCK_TRANSLATORS;
      ufe->system_clock_rec_enable = WUFE_SYSTEM_CLOCK_REC_ENABLE;
      ufe->clk_rec_system_params = NULL;
#endif /* WT_UFE4 */
#else
#if defined __WT_UFE412__ || defined __WT_UFE448__
#else
      ufe->max_sbi_clock_translators = 0;
#endif  /* WT_UFE4 */
      ufe->system_clock_rec_enable = WUFE_SYSTEM_CLOCK_REC_DISABLE;
      ufe->clk_rec_system_params = NULL;
#endif /* WTI_CESOP_CLOCK_RECOVERY_ENABLE */

      ufe->max_lines = WTI_MAX_LINES;
      ufe->max_phys = WTI_MAX_NUM_EMPHY_PHYS;
      ufe->ufe_id = ufe_id;
      ufe->upi_id = upi_id;
   }

   WT_UfeSystemCreate(ufe);
}

#endif

/*****************************************************************************
 * Function name: WTI_CesopJitterBufferParamsModify
 * Description  : modify Tx jitter buffer parameters: Jitter buffer size,
 *                Tx transmit threshold, Tx switchover Threshold.
 *                modification of these parameters requires disabling the
 *                classification rule in order to avoid traffic from entering
 *                the jitter buffer and disabling the Tx PWE3 transparent channel.
 * Input params:  psn2tdm_iw_system - handle of the PSN --> TDM IW system
 *                classification_flow - handle of  dfc or lpm flow related
 *                                      to the Tx channel in case of IP network
 *                                      mpls flow handle in case of MPLS network.
 *                tx_channel_handle - handle of transparent PWE3 Tx channel
 *                tx_jitter_buffer_size - new Tx jitter buffer size
 *                window_tx_threshold - new Tx transmit threshold
 *                window_switchover_threshold - new switchover threshold
 * Output params:
 * Return val   : none
 *****************************************************************************/
void WTI_CesopJitterBufferParamsModify(WP_handle psn2tdm_iw_system,
                                       WP_handle classification_flow,
                                       WP_handle tx_channel_handle,
                                       WP_U32 tx_jitter_buffer_size,
                                       WP_U16 window_tx_threshold,
                                       WP_U8 window_switchover_threshold,
                                       WP_U8 lops_detection,
                                       WP_U8 consec_pkts_in_synch_th,
                                       WP_U8 consec_miss_pkts_out_synch_th)
{
   WP_status status;
   WP_command command;
   WP_iw_tx_binding_cesop cesop_tx_binding_config;
   WP_ch_trans_pwe3 pwe3_ch_cfg;

   memset(&cesop_tx_binding_config, 0, sizeof(WP_iw_tx_binding_cesop));
   memset(&pwe3_ch_cfg, 0, sizeof(WP_ch_trans_pwe3));

#if WTI_PCE_CLASSIFIER
   status = WP_PceRuleDisable(classification_flow);
   WTI_TerminateOnError(status,
                        "WP_PceRuleDisable PCE flow", __LINE__);
#else
   /* disable classification rule and wait 50 micro seconds to assure that
      no traffic enters the jitter buffer */
   status = WP_IwFlowDisable(psn2tdm_iw_system, classification_flow);
   WTI_TerminateOnError(status, "WP_IwFlowDisable() classification flow", __LINE__);
#endif

   /* build IW system */
   status = WP_IwSystemBuild(psn2tdm_iw_system);
   WTI_TerminateOnError(status, "WP_IwSystemBuild() PSN ----> TDM",__LINE__);

   /* give traffic to pass */
   WP_Delay(50000);

   /* disable transparent Tx channel */
   status = WP_ChannelDisable(tx_channel_handle);
   WTI_TerminateOnError(status, "WP_ChannelDisable() Tx transparent PWE3 channel", __LINE__);

   /* modify jitter buffer size */
   pwe3_ch_cfg.tx_jitter_buffer_size = tx_jitter_buffer_size;

   /* set modification command */
   command = WP_CH_MOD_TRANS_TX_JITTER_BUFFER_SIZE;

   status = WP_ChannelModify(tx_channel_handle,
                             WP_CH_MOD_TRANS_TX_JITTER_BUFFER_SIZE,
                             &pwe3_ch_cfg);
   WTI_TerminateOnError(status, "WP_ChannelModify() Tx transparent PWE3 channel", __LINE__);

   /* modify Tx transmit threshold and switchover threshold */
   cesop_tx_binding_config.window_tx_threshold = window_tx_threshold;
   cesop_tx_binding_config.window_switchover_threshold = window_switchover_threshold;
   cesop_tx_binding_config.window_max_threshold = tx_jitter_buffer_size;
   cesop_tx_binding_config.lops_detection = lops_detection;
   cesop_tx_binding_config.consec_pkts_in_synch_th = consec_pkts_in_synch_th;
   cesop_tx_binding_config.consec_miss_pkts_out_synch_th = consec_miss_pkts_out_synch_th;

   /* set modification command */
   command = (WP_IW_TX_BIND_MOD_CESOP_TX_THRESHOLD |
              WP_IW_TX_BIND_MOD_CESOP_SWITCHOVER_THRESHOLD |
              WP_IW_TX_BIND_MOD_CESOP_JITTER_MAX_THRESHOLD |
              WP_IW_TX_BIND_MOD_CESOP_LOPS_DETECTION );

   status = WP_IwTxBindingModify(tx_channel_handle,
                                 command,
                                 WP_IW_TX_BINDING_CESOP,
                                 &cesop_tx_binding_config);
   WTI_TerminateOnError(status, "WP_IwTxBindingModify()", __LINE__);

   /* enable transparent Tx channel */
   status = WP_ChannelEnable(tx_channel_handle);
   WTI_TerminateOnError(status, "WP_ChannelEnable() Tx transparent PWE3 channel", __LINE__);

#if WTI_PCE_CLASSIFIER
   status = WP_PceRuleEnable(classification_flow);
   WTI_TerminateOnError(status,
                        "WP_PceRuleEnable PCE flow", __LINE__);
#else
   /* enable classification rule */
   status = WP_IwFlowEnable(psn2tdm_iw_system, classification_flow);
   WTI_TerminateOnError(status, "WP_IwFlowEnable() classification flow", __LINE__);
#endif

   /* build IW system */
   status = WP_IwSystemBuild(psn2tdm_iw_system);
   WTI_TerminateOnError(status, "WP_IwSystemBuild() PSN ----> TDM",__LINE__);
}

/*****************************************************************************
 * Function name: WTI_CesopTdm2PsnFlowAggParamsModify
 * Description  : This function modifies TDM to PSN CESoP flow aggregation
 *                parameters that require disabling the TDM to PSN flow.
 * Input  params: rx_channel_handle - handle of transparent PWE3 Rx channel
 *                tdm2psn_flow_agg - handle of TDM to PSN flow aggregation
 *                command - modification command
 *                cfg_ptr - pointer to WP_iw_agg_cesop configuration structure
 * Output params:
 * Return val   : none
 *****************************************************************************/
void WTI_CesopTdm2PsnFlowAggParamsModify(WP_handle rx_channel_handle,
                                         WP_handle tdm2psn_flow_agg,
                                         WP_command command,
                                         WP_iw_agg_cesop *cfg_ptr)
{
   WP_status status;

   /* disable the rx transparent channel */
   status = WP_ChannelDisable(rx_channel_handle);
   WTI_TerminateOnError(status, "WP_ChannelDisable() transparent rx channel", __LINE__);

   /* give traffic to pass */
   WP_Delay(50000);

   /* check that at least one of the bits in the modification command represent
      a modification that requires the disable of the TDM to PSN flow. */
   if (!((command & WP_IW_FLOW_AGG_MOD_TXFUNC) ||
         (command & WP_IW_FLOW_AGG_CESOP_L3_PROTOCOL) ||
         (command & WP_IW_FLOW_AGG_CESOP_CW_OFFSET) ||
         (command & WP_IW_FLOW_AGG_CESOP_RTP_HDR) ||
         (command & WP_IW_FLOW_AGG_CESOP_PAYLOAD_SIZE) ||
         (command & WP_IW_FLOW_AGG_CESOP_PREFIX) ||
         (command & WP_IW_FLOW_AGG_CESOP_L2_LEN_UPDATE) ||
         (command & WP_IW_FLOW_AGG_CESOP_L3_HDR_OFFSET) ||
         (command & WP_IW_FLOW_AGG_CESOP_PRIORITY_REMARK) ||
         (command & WP_IW_FLOW_AGG_CESOP_L4_LEN_UPDATE)))
   {
      printf("ERROR - wrong modification command!");
      return;
   }

   /* modify the flow aggregation */
   status = WP_IwFlowAggregationModify(tdm2psn_flow_agg, command, cfg_ptr);
   WTI_TerminateOnError(status, "WP_IwFlowAggregationModify()",__LINE__);

   /* enable the rx transparent channel */
   status = WP_ChannelEnable(rx_channel_handle);
   WTI_TerminateOnError(status, "WP_ChannelEnable() transparent rx channel", __LINE__);
}

/*****************************************************************************
 * Function name: WTI_CesopPsn2TdmFlowAggParamsModify
 * Description  : This function modifies PSN to TDM CESoP flow aggregation
 *                parameters that require disabling the PSN to TDM flow.
 * Input  params: psn2tdm_iw_system - handle of the PSN --> TDM IW system
 *                classification_flow - handle of  dfc or lpm flow related
 *                                      to the Tx channel
 *                tx_channel_handle - handle of transparent PWE3 Tx channel
 *                psn2tdm_flow_agg - flow aggregation handle in the PSN to
 *                                   TDM direction.
 *                command - flow aggregation modification command.
 *                cfg_ptr - pointer to WP_iw_agg_cesop configuration structure.
 * Output params:
 * Return val   : none
 *****************************************************************************/
void WTI_CesopPsn2TdmFlowAggParamsModify(WP_handle psn2tdm_iw_system,
                                         WP_handle classification_flow,
                                         WP_handle tx_channel_handle,
                                         WP_handle psn2tdm_flow_agg,
                                         WP_command command,
                                         WP_iw_agg_cesop *cfg_ptr)
{
   WP_status status;

   /* check that at least one of the bits in the modification command represent
      a modification that requires the disable of the PSN to TDM flow. */
   if (!((command & WP_IW_FLOW_AGG_MOD_TXFUNC) ||
         (command & WP_IW_FLOW_AGG_CESOP_EXTRACTION_LEN) ||
         (command & WP_IW_FLOW_AGG_CESOP_PAYLOAD_TYPE) ||
         (command & WP_IW_FLOW_AGG_CESOP_SSRC)))
   {
      printf("ERROR - wrong modification command!");
      return;
   }

   /* disable classification rule and wait 50 micro seconds to let traffic pass */
   status = WP_IwFlowDisable(psn2tdm_iw_system, classification_flow);
   WTI_TerminateOnError(status, "WP_IwFlowDisable() classification flow", __LINE__);

   /* build IW system */
   status = WP_IwSystemBuild(psn2tdm_iw_system);
   WTI_TerminateOnError(status, "WP_IwSystemBuild() PSN ----> TDM",__LINE__);

   /* give traffic to pass */
   WP_Delay(50000);

   /* disable transparent Tx channel */
   status = WP_ChannelDisable(tx_channel_handle);
   WTI_TerminateOnError(status, "WP_ChannelDisable() Tx transparent PWE3 channel", __LINE__);

   /* modify the flow aggregation */
   status = WP_IwFlowAggregationModify(psn2tdm_flow_agg, command, cfg_ptr);
   WTI_TerminateOnError(status, "WP_IwFlowAggregationModify()",__LINE__);

   /* enable the tx transparent channel */
   status = WP_ChannelEnable(tx_channel_handle);
   WTI_TerminateOnError(status, "WP_ChannelEnable() Tx transparent PWE3 channel", __LINE__);

   /* enable classification rule */
   status = WP_IwFlowEnable(psn2tdm_iw_system, classification_flow);
   WTI_TerminateOnError(status, "WP_IwFlowEnable() classification flow", __LINE__);

   /* build IW system */
   status = WP_IwSystemBuild(psn2tdm_iw_system);
   WTI_TerminateOnError(status, "WP_IwSystemBuild() PSN ----> TDM",__LINE__);
}


#if WTI_CESOP_MEF8_ENCAP
/*****************************************************************************
 * Function name: WTI_FlowAggregationMef8Tdm2PsnConfigInit
 * Description  : initialize TDM to PSN flow aggregation configuration structure
 *                with default values for MEF encapsulation
 * Input  params: cfg_ptr - pointer to configuration structure
 * Output params:
 * Return val   : none
 *****************************************************************************/
void WTI_FlowAggregationMef8Tdm2PsnConfigInit(WP_iw_agg_cesop *cfg_ptr)
{
   memset(cfg_ptr, 0, sizeof(WP_iw_agg_cesop));

   cfg_ptr->tag = 0;
   cfg_ptr->txfunc = 0;
   cfg_ptr->intmode = WTI_TDM2PSN_INTERRUPT_MODE;
   cfg_ptr->interruptqueue = WTI_TDM2PSN_INTERRUPT_QUEUE;
   cfg_ptr->statmode = WTI_TDM2PSN_STAT_MODE;
   cfg_ptr->timestamp_mode = WTI_TDM2PSN_TIMESTAMP_MODE;
   cfg_ptr->ov_pool_mode = WTI_TDM2PSN_OV_POOL_MODE;
   cfg_ptr->oobc_mode = WP_DISABLE;
   cfg_ptr->fbp_drop_threshold = WTI_TDM2PSN_FBP_DROP_THRESHOLD;
   cfg_ptr->mtu = WTI_TDM2PSN_MTU;
   cfg_ptr->flow_agg_type = WP_IW_AGG_PRIMARY;

   cfg_ptr->l3_protocol = WP_IW_CESOP_L3_MPLS; /* 0 */
   cfg_ptr->control_word_offset = (WTI_ETHERNET_HEADER_SIZE+
                                   WTI_NUM_OF_VLAN_TAGS * WTI_VLAN_TAG_SIZE+
                                   WTI_ECID_LABEL_SIZE);

   cfg_ptr->rtp_header_offset = (cfg_ptr->control_word_offset +
                                 WTI_CONTROL_WORD_LEN);
   cfg_ptr->prefix_length = (cfg_ptr->rtp_header_offset +
                             WTI_RTP_HEADER_LEN);
   cfg_ptr->l3_header_offset = 0;

   cfg_ptr->l4_length_update_mode = 0;
   cfg_ptr->l4_length_offset = 0;

   cfg_ptr->rtp_header_mode = WTI_TDM2PSN_RTP_MODE;

   cfg_ptr->payload_size = 0;
   cfg_ptr->oobc_flow_handle = 0;

   cfg_ptr->l2_length_update_mode = WTI_TDM2PSN_L2_LENGTH_UPDATE_MODE;
   cfg_ptr->l2_length_offset = 0;

   cfg_ptr->l4_checksum_recalc_mode = WTI_TDM2PSN_L4_CHECKSUM_RECALC_MODE;
   cfg_ptr->last_packet_cw_length = 0;
   cfg_ptr->last_packet_ip_checksum = 0;
   cfg_ptr->initial_seq_number = WTI_TDM2PSN_INITIAL_SEQ_NUMBER;
   cfg_ptr->extraction_length = 0;
   cfg_ptr->cas_location = 0;
   cfg_ptr->control_word_mode = 0;
   cfg_ptr->payload_type_check = 0;
   cfg_ptr->payload_type = 0;
   cfg_ptr->oobc_payload_type = 0;
   cfg_ptr->ssrc_check = 0;
   cfg_ptr->ssrc = 0;
   cfg_ptr->oobc_ssrc = 0;
   cfg_ptr->ts_method = 0;
   cfg_ptr->ces_shaping =0;
}
#endif /*#ifdef WTI_CESOP_MEF8_ENCAP*/


/*****************************************************************************
 * Function name: WTI_FlowAggregationTdm2PsnConfigInit
 * Description  : initialize TDM to PSN flow aggregation configuration structure
 *                with default values
 * Input  params: cfg_ptr - pointer to configuration structure
 * Output params:
 * Return val   : none
 *****************************************************************************/
void WTI_FlowAggregationTdm2PsnConfigInit(WP_iw_agg_cesop *cfg_ptr)
{
   memset(cfg_ptr, 0, sizeof(WP_iw_agg_cesop));

   cfg_ptr->tag = 0;
   cfg_ptr->txfunc = 0;
   cfg_ptr->intmode = WTI_TDM2PSN_INTERRUPT_MODE;
   cfg_ptr->interruptqueue = WTI_TDM2PSN_INTERRUPT_QUEUE;
   cfg_ptr->statmode = WTI_TDM2PSN_STAT_MODE;
   cfg_ptr->timestamp_mode = WTI_TDM2PSN_TIMESTAMP_MODE;
   cfg_ptr->ov_pool_mode = WTI_TDM2PSN_OV_POOL_MODE;
   cfg_ptr->oobc_mode = WP_DISABLE;
   cfg_ptr->fbp_drop_threshold = WTI_TDM2PSN_FBP_DROP_THRESHOLD;
   cfg_ptr->mtu = WTI_TDM2PSN_MTU;
   cfg_ptr->flow_agg_type = WP_IW_AGG_PRIMARY;
#if WTI_CESOP_MPLS_IW
   cfg_ptr->l3_protocol = WP_IW_CESOP_L3_IPV4;
#if WTI_CESOP_NATIVE_MPLS_IW
   cfg_ptr->control_word_offset = (WTI_MPLS_LABEL_SIZE +
                                   WTI_L3_HEADER_LEN);
   cfg_ptr->rtp_header_offset = (WTI_MPLS_LABEL_SIZE +
                                 WTI_L3_HEADER_LEN +
                                 WTI_CONTROL_WORD_LEN);
   cfg_ptr->prefix_length = (WTI_MPLS_LABEL_SIZE +
                             WTI_L3_HEADER_LEN +
                             WTI_CONTROL_WORD_LEN +
                             WTI_RTP_HEADER_LEN);
   cfg_ptr->l3_header_offset = WTI_MPLS_LABEL_SIZE;
#else
#if WTI_CESOP_MPLS_OVER_ENET
   cfg_ptr->control_word_offset = (WTI_ETHERNET_HEADER_SIZE +
#else
   cfg_ptr->control_word_offset = (WTI_PPP_HEADER_SIZE +
#endif
                                   WTI_MPLS_LABEL_SIZE +
                                   WTI_L3_HEADER_LEN);
#if WTI_CESOP_MPLS_OVER_ENET
   cfg_ptr->rtp_header_offset = (WTI_ETHERNET_HEADER_SIZE +
#else
   cfg_ptr->rtp_header_offset = (WTI_PPP_HEADER_SIZE +
#endif
                                 WTI_MPLS_LABEL_SIZE +
                                 WTI_L3_HEADER_LEN +
                                 WTI_CONTROL_WORD_LEN);
#if WTI_CESOP_MPLS_OVER_ENET
   cfg_ptr->prefix_length = (WTI_ETHERNET_HEADER_SIZE +
#else
   cfg_ptr->prefix_length = (WTI_PPP_HEADER_SIZE +
#endif
                             WTI_MPLS_LABEL_SIZE +
                             WTI_L3_HEADER_LEN +
                             WTI_CONTROL_WORD_LEN +
                             WTI_RTP_HEADER_LEN);
#if WTI_CESOP_MPLS_OVER_ENET
   cfg_ptr->l3_header_offset = (WTI_ETHERNET_HEADER_SIZE +
#else
   cfg_ptr->l3_header_offset = (WTI_PPP_HEADER_SIZE +
#endif
                                WTI_MPLS_LABEL_SIZE);
#endif
   cfg_ptr->l4_length_update_mode = 0;
   cfg_ptr->l4_length_offset = 0;
#else
   cfg_ptr->l3_protocol = WP_IW_CESOP_L3_IPV4;/* not importnat for PSN2TDM- this control the L2 Length Update*/
   cfg_ptr->control_word_offset = (WTI_L2_HEADER_LEN + WTI_L3_HEADER_LEN +
                                   WTI_L4_HEADER_LEN + WTI_RTP_HEADER_LEN);
   cfg_ptr->rtp_header_offset = (WTI_L2_HEADER_LEN + WTI_L3_HEADER_LEN +
                                 WTI_L4_HEADER_LEN);
   cfg_ptr->prefix_length = (WTI_L2_HEADER_LEN + WTI_L3_HEADER_LEN +
                             WTI_L4_HEADER_LEN + WTI_RTP_HEADER_LEN +
                             WTI_CONTROL_WORD_LEN);
   cfg_ptr->l4_length_update_mode = WTI_TDM2PSN_L4_LENGTH_UPDATE_MODE;
   cfg_ptr->l4_length_offset = (WTI_L2_HEADER_LEN + WTI_L3_HEADER_LEN +
                                WTI_L4_LENGTH_OFFSET);
   cfg_ptr->l3_header_offset = WTI_L2_HEADER_LEN;
#endif
   cfg_ptr->rtp_header_mode = WTI_TDM2PSN_RTP_MODE;

   cfg_ptr->payload_size = 0;
   cfg_ptr->oobc_flow_handle = 0;

   cfg_ptr->l2_length_update_mode = WTI_TDM2PSN_L2_LENGTH_UPDATE_MODE;
   cfg_ptr->l2_length_offset = 0;

   cfg_ptr->l4_checksum_recalc_mode = WTI_TDM2PSN_L4_CHECKSUM_RECALC_MODE;
   cfg_ptr->last_packet_cw_length = 0;
   cfg_ptr->last_packet_ip_checksum = 0;
   cfg_ptr->initial_seq_number = WTI_TDM2PSN_INITIAL_SEQ_NUMBER;
   cfg_ptr->extraction_length = 0;
   cfg_ptr->cas_location = 0;
   cfg_ptr->control_word_mode = 0;
   cfg_ptr->payload_type_check = 0;
   cfg_ptr->payload_type = 0;
   cfg_ptr->oobc_payload_type = 0;
   cfg_ptr->ssrc_check = 0;
   cfg_ptr->ssrc = 0;
   cfg_ptr->oobc_ssrc = 0;
   cfg_ptr->ts_method = 0;
}

#if WTI_CESOP_MEF8_ENCAP
void WTI_FlowAggregationPsn2TdmBridgeConfigMef (WP_iw_agg_cesop *cfg_ptr,
                                             int pw_index,
                                             int oobc_mode)
{
   WP_U32 layer2_offset;

   memcpy(cfg_ptr, &pw_config.psn2tdm_flow_agg_config, sizeof(WP_iw_agg_cesop));

   cfg_ptr->txfunc = the_system->pw[pw_index].trans_tx;
   cfg_ptr->payload_size = pw_config.rx_buffersize;
   cfg_ptr->oobc_mode = oobc_mode;

#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
   cfg_ptr->clock_rec_params = 0;
#endif

   cfg_ptr->timestamp_mode = pw_config.psn2tdm_flow_agg_config.timestamp_mode;

   if (cfg_ptr->timestamp_mode == WP_IW_TIME_STAMP_ENABLE)
   {
      cfg_ptr->ts_method = pw_config.psn2tdm_flow_agg_config.ts_method;
      cfg_ptr->ts_diff_if_id = pw_config.psn2tdm_flow_agg_config.ts_diff_if_id;
      cfg_ptr->ts_const = pw_config.psn2tdm_flow_agg_config.ts_const;
   }
   else
   {
      cfg_ptr->ts_method = 0;
      cfg_ptr->ts_diff_if_id = 0;
      cfg_ptr->ts_const = 0;
   }
   cfg_ptr->buffer_gap = 64; /* should be the same value as in the IW system */
}
#endif

/*****************************************************************************
 * Function name: WTI_FlowAggregationPsn2TdmBridgeConfig
 * Description  : initialize PSN to TDM flow aggregation configuration structure
 *                with actual values after user configuration
 * Input  params: cfg_ptr - pointer to configuration structure
 *                pw_index - PW index
 *                oobc_mode - out of band CAS mode (0-disable,1-enable)
 *                clock_master - clock master mode (the clock is recovered according to
 *                               packets arriving to this flow)(0-disable,1-enable)
 * Output params:
 * Return val   : none
 *****************************************************************************/
void WTI_FlowAggregationPsn2TdmBridgeConfig (WP_iw_agg_cesop *cfg_ptr,
                                             int pw_index,
                                             int oobc_mode)

{
   WP_U32 layer2_offset;

   memcpy(cfg_ptr, &pw_config.psn2tdm_flow_agg_config, sizeof(WP_iw_agg_cesop));

   cfg_ptr->txfunc = the_system->pw[pw_index].trans_tx;
   cfg_ptr->payload_size = pw_config.rx_buffersize;
   cfg_ptr->oobc_mode = oobc_mode;

#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
   cfg_ptr->clock_rec_params = 0;
#endif

   cfg_ptr->timestamp_mode = pw_config.psn2tdm_flow_agg_config.timestamp_mode;

   if (cfg_ptr->timestamp_mode == WP_IW_TIME_STAMP_ENABLE)
   {
      cfg_ptr->ts_method = pw_config.psn2tdm_flow_agg_config.ts_method;
      cfg_ptr->ts_diff_if_id = pw_config.psn2tdm_flow_agg_config.ts_diff_if_id;
      cfg_ptr->ts_const = pw_config.psn2tdm_flow_agg_config.ts_const;
   }
   else
   {
      cfg_ptr->ts_method = 0;
      cfg_ptr->ts_diff_if_id = 0;
      cfg_ptr->ts_const = 0;
   }
   cfg_ptr->buffer_gap = 64; /* should be the same value as in the IW system */

   layer2_offset = 6+6+2+2*pw_config.num_of_vlan_tags;

#if !WTI_CESOP_TO_MPLS_FA_CASCADING
   cfg_ptr->control_word_offset = (layer2_offset+
                                   WTI_L3_HEADER_LEN +
                                   WTI_L4_HEADER_LEN +
                                   WTI_RTP_HEADER_LEN);

   cfg_ptr->rtp_header_offset = (layer2_offset+
                                 WTI_L3_HEADER_LEN +
                                 WTI_L4_HEADER_LEN);

   cfg_ptr->extraction_length = (layer2_offset+
                                 WTI_L3_HEADER_LEN +
                                 WTI_L4_HEADER_LEN +
                                 WTI_RTP_HEADER_LEN +
                                 WTI_CONTROL_WORD_LEN);
#endif

   if(cfg_ptr->extraction_length > 64)
   {
      printf("  the extraction length must be smaller than 64 bytes\n");
      exit(1);

   }


}

/*****************************************************************************
 * Function name: WTI_FlowAggregationTdm2PsnConfig
 * Description  : initialize TDM to PSN flow aggregation configuration structure
 *                with actual values after user configuration
 * Input  params: cfg_ptr - pointer to configuration structure
 *                pw_index - PW index
 *                oobc_mode - out of band CAS mode (0-PW does not support OOBC,
 *                                                  1-PW supports OOBC - DATA flow aggregation,
 *                                                  2-PW supports OOBC - CAS flow aggregation)
 * Output params:
 * Return val   : none
 *****************************************************************************/
void WTI_FlowAggregationTdm2PsnConfig(WP_iw_agg_cesop *cfg_ptr, int pw_index, int oobc_mode)
{
#if WTI_CESOP_MPLS_IW
   WP_U32 mpls_label;
#endif
#if WTI_EXT_DIFF_MODE_ACTIVE
   WP_U32 cr_op_mode = 0;
#endif

   memcpy(cfg_ptr, &pw_config.tdm2psn_flow_agg_config, sizeof(WP_iw_agg_cesop));

#if (WTI_CESOP_MPLS_IW && !WTI_CESOP_MPLS_OVER_ENET)
   cfg_ptr->txfunc = the_system->hspi_device;
#else
#if (WTI_2_ENET_DEVICES)
   if (pw_index > WTI_FIRST_ENET_MAX_PW)
      cfg_ptr->txfunc = the_system->tx_enet_channel1;
   else
   {
#if WTI_EXT_DIFF_MODE_ACTIVE
      cr_op_mode = the_system->clock_rec[pw_config.line_index].operation_mode;
      if (pw_index < global_active_cr_async_mode)
/*      if (cr_op_mode == WP_IW_CLOCK_REC_ADAPTIVE_MODE) */
      {
         /* PW using Adaptive clock recovery method uses a high priority queue */
         cfg_ptr->txfunc = the_system->tx_enet_channel_high_priority;
      }
      else
#endif /* WTI_EXT_DIFF_MODE_ACTIVE */
         cfg_ptr->txfunc = the_system->tx_enet_channel;
   }
#else
#if WTI_EXT_DIFF_MODE_ACTIVE
   cr_op_mode = the_system->clock_rec[pw_config.line_index].operation_mode;
/*   if (pw_index < global_active_cr_async_mode) */
   if (cr_op_mode == WP_IW_CLOCK_REC_ADAPTIVE_MODE)
   {
      /* PW using Adaptive clock recovery method uses a high priority queue */
      cfg_ptr->txfunc = the_system->tx_enet_channel_high_priority;
   }
   else
#endif /* WTI_EXT_DIFF_MODE_ACTIVE */
      cfg_ptr->txfunc = the_system->tx_enet_channel;
#endif
#endif

#if WTI_CESOP_TO_MPLS_FA_CASCADING  /* !!!!!  OVERWRITE !!!!! */
   cfg_ptr->txfunc = pw_config.tdm2psn_flow_agg_config.txfunc;
#endif /* WTI_CESOP_TO_MPLS_FA_CASCADING */

   switch (oobc_mode)
   {
      case (0): /* OOBC disabled */
      {
         WP_U32 cas_size;
         WP_U8 num_slots;

         num_slots = pw_config.n_slots;
         cas_size = (num_slots % 2) ? ((num_slots / 2) + 1) : (num_slots / 2);
         cfg_ptr->payload_size = pw_config.rx_buffersize;
#if WTI_CESOP_TDI
         if (tdm_config[pw_config.line_index].framing_mode == WTI_TDI_FRAMED_MODE &&
             tdm_config[pw_config.line_index].cas_mode == WTI_TDI_CAS_MODE)
         {
            WP_U32 control_word=0;
            WP_U16 structure_size = (tdm_config[pw_config.line_index].op_mode == WTI_TDI_T1_OP_MODE) ?
               WP_TDM_STRUCTURE_T1_ESF :
               WP_TDM_STRUCTURE_E1_ESF;

            /* if UNFRG mode add cas_size to payload_size in order to update headers length */
            if (pw_config.rx_buffersize == pw_config.n_slots * structure_size)
            {
               cfg_ptr->payload_size += cas_size;
               memcpy(&control_word, pw_config.control_word, WTI_CONTROL_WORD_LEN);
               control_word &= ~WTI_TDM2PSN_LEN_MASK;
               if (cfg_ptr->payload_size < 64)
                  control_word |= (cfg_ptr->payload_size << 16);
               memcpy(pw_config.control_word, &control_word, WTI_CONTROL_WORD_LEN);
            }
         }
#else
         if (pw_config.mode == (WP_EMPHY_TRANS_FRAMED_CAS | WP_EMPHY_TRANS_T1) ||
             pw_config.mode == (WP_EMPHY_TRANS_FRAMED_CAS | WP_EMPHY_TRANS_E1))
         {
            WP_U32 control_word=0;
            WP_U16 structure_size;

            if (pw_config.mode == (WP_EMPHY_TRANS_FRAMED_CAS | WP_EMPHY_TRANS_T1))
               structure_size = WP_TDM_STRUCTURE_T1_ESF;
            else
               structure_size = WP_TDM_STRUCTURE_E1_ESF;

            /* if UNFRG mode add cas_size to payload_size in order to update headers length */
            if (pw_config.rx_buffersize == pw_config.n_slots * structure_size)
            {
               cfg_ptr->payload_size += cas_size;
               memcpy(&control_word, pw_config.control_word, WTI_CONTROL_WORD_LEN);
               control_word &= ~WTI_TDM2PSN_LEN_MASK;
               if (cfg_ptr->payload_size < 64)
                  control_word |= (cfg_ptr->payload_size << 16);
               memcpy(pw_config.control_word, &control_word, WTI_CONTROL_WORD_LEN);
            }
         }
         cfg_ptr->intmode = WTI_TDM2PSN_INTERRUPT_MODE;
         cfg_ptr->interruptqueue = WTI_TDM2PSN_INTERRUPT_QUEUE;
#endif
         cfg_ptr->oobc_flow_handle = 0;
         cfg_ptr->oobc_mode = WP_DISABLE;

         if (pw_config.rtp_header)
         {
            WTI_rtp_header rtp;

            memcpy(&rtp, pw_config.rtp_header, sizeof(WTI_rtp_header));

            rtp.payload_type = WTI_PSN2TDM_PAYLOAD_TYPE;
            rtp.ssrc = WTI_PSN2TDM_SSRC_TYPE;

            memcpy(pw_config.rtp_header,&rtp, sizeof(WTI_rtp_header));
         }

         cfg_ptr->initial_seq_number = WTI_TDM2PSN_INITIAL_SEQ_NUMBER;
         cfg_ptr->timestamp_mode = pw_config.tdm2psn_flow_agg_config.timestamp_mode;
         if (cfg_ptr->timestamp_mode == WP_IW_TIME_STAMP_ENABLE)
         {
            cfg_ptr->ts_method = pw_config.tdm2psn_flow_agg_config.ts_method;
            cfg_ptr->ts_diff_if_id = pw_config.tdm2psn_flow_agg_config.ts_diff_if_id;
            cfg_ptr->ts_const = pw_config.tdm2psn_flow_agg_config.ts_const;
         }
         else
         {
            cfg_ptr->ts_method = 0;
            cfg_ptr->ts_diff_if_id = 0;
            cfg_ptr->ts_const = 0;
         }

         break;
      }
      case (1): /* OOBC enabled, DATA dedicated TDM to PSN flow aggregation */
      {
         WP_U32 control_word;

         cfg_ptr->oobc_flow_handle = the_system->pw[pw_index].cas_tdm2psn_flow_agg;
         cfg_ptr->payload_size = pw_config.rx_buffersize;
         cfg_ptr->oobc_mode = WP_ENABLE;

         memcpy(&control_word, pw_config.control_word, WTI_CONTROL_WORD_LEN);
         control_word &= ~WTI_TDM2PSN_LRM_MASK;
         control_word |= WTI_TDM2PSN_DATA_PACKET_LRM;
         memcpy(pw_config.control_word, &control_word, WTI_CONTROL_WORD_LEN);

         if (pw_config.rtp_header)
         {
            WTI_rtp_header rtp;

            memcpy(&rtp, pw_config.rtp_header, sizeof(WTI_rtp_header));

            rtp.payload_type = WTI_PSN2TDM_PAYLOAD_TYPE;
            rtp.ssrc = WTI_PSN2TDM_SSRC_TYPE;

            memcpy(pw_config.rtp_header,&rtp, sizeof(WTI_rtp_header));
         }

         cfg_ptr->intmode = WTI_TDM2PSN_INTERRUPT_MODE;
         cfg_ptr->interruptqueue = WTI_TDM2PSN_INTERRUPT_QUEUE;
         cfg_ptr->initial_seq_number = WTI_TDM2PSN_INITIAL_SEQ_NUMBER;

         break;
      }
      case (2): /* OOBC enabled, CAS dedicated TDM to PSN flow aggregation */
      {
         WP_U32 control_word, cas_size;
         WP_U8 num_slots;

         num_slots = pw_config.n_slots;
         cas_size = (num_slots * 4); /* in OOBC mode, the CAS size is 4*num_slots*/

         cfg_ptr->oobc_flow_handle = 0;
         cfg_ptr->payload_size = cas_size;
         cfg_ptr->oobc_mode = WP_DISABLE;

         memcpy(&control_word, pw_config.control_word, WTI_CONTROL_WORD_LEN);
         control_word &= ~(WTI_TDM2PSN_LRM_MASK | WTI_TDM2PSN_LEN_MASK);
         control_word |= (WTI_TDM2PSN_CAS_PACKET_LRM | (cas_size << 16));
         memcpy(pw_config.control_word, &control_word, WTI_CONTROL_WORD_LEN);

         if (pw_config.rtp_header)
         {
            WTI_rtp_header rtp;

            memcpy(&rtp, pw_config.rtp_header, sizeof(WTI_rtp_header));

            rtp.payload_type = WTI_PSN2TDM_OOBC_PAYLOAD_TYPE;
            rtp.ssrc = WTI_PSN2TDM_OOBC_SSRC_TYPE;

            memcpy(pw_config.rtp_header,&rtp, sizeof(WTI_rtp_header));
         }

         cfg_ptr->intmode = WTI_TDM2PSN_OOBC_INTERRUPT_MODE;
         cfg_ptr->interruptqueue = WTI_TDM2PSN_OOBC_INTERRUPT_QUEUE;
         cfg_ptr->initial_seq_number = WTI_TDM2PSN_OOBC_CAS_INITIAL_SEQ_NUMBER;

         break;
      }
      default:
         printf("error: line: %d\n", __LINE__);
         return;
   }

#if WTI_CESOP_MPLS_IW
/* in case of MPLS FA CASCADING, the L2 is added by the layer2 router MPLS FA */
#if (!WTI_CESOP_NATIVE_MPLS_IW && !WTI_CESOP_TO_MPLS_FA_CASCADING)

#if WTI_CESOP_MPLS_OVER_ENET
      memcpy(cfg_ptr->prefix_header, &mpls_enet_header, WTI_ETHERNET_HEADER_SIZE);
#else
   WP_U32 ppp_header;

   /* set PPP header */
   ppp_header = WTI_MPLSoPPP;
   memcpy(cfg_ptr->prefix_header, &ppp_header, WTI_PPP_HEADER_SIZE);
#endif

#endif /* (!WTI_CESOP_NATIVE_MPLS_IW && !WTI_CESOP_TO_MPLS_FA_CASCADING) */

   /* set mpls label */
   /* if wish to give offset add to pw_index here: */
   mpls_label = WTI_MPLS_LABEL + ((pw_index)<< 12);


#if WTI_CESOP_NATIVE_MPLS_IW
   memcpy(cfg_ptr->prefix_header, &mpls_label, 4);
#else
#if WTI_CESOP_MPLS_OVER_ENET
   memcpy(&cfg_ptr->prefix_header[WTI_ETHERNET_HEADER_SIZE], &mpls_label, 4);
#else
   memcpy(&cfg_ptr->prefix_header[WTI_PPP_HEADER_SIZE], &mpls_label, 4);
#endif
#endif /* WTI_CESOP_NATIVE_MPLS_IW */

   /* set IP header */
   if (!pw_config.ip_header)
   {
      printf("ip_header definition is not set to pw_config\n");
      exit(1);
   }

   pw_config.ip_header->checksum = 0;

   pw_config.ip_header->total_length = (WP_U16) (cfg_ptr->payload_size +
#if WTI_CESOP_MPLS_OVER_ENET
                                                 WTI_ETHERNET_HEADER_SIZE +
#else
                                                 WTI_PPP_HEADER_SIZE +
#endif
                                                 WTI_MPLS_LABEL_SIZE +
                                                 WTI_L3_HEADER_LEN +
                                                 WTI_CONTROL_WORD_LEN +
                                                 WTI_RTP_HEADER_LEN);


   pw_config.ip_header->checksum = WTI_IpChecksumCalc((WP_U8 *)pw_config.ip_header, WTI_L3_HEADER_LEN);

#if WTI_CESOP_NATIVE_MPLS_IW
   memcpy(&cfg_ptr->prefix_header[WTI_MPLS_LABEL_SIZE],
          pw_config.ip_header,
          WTI_L3_HEADER_LEN);
#else
#if WTI_CESOP_MPLS_OVER_ENET
   memcpy(&cfg_ptr->prefix_header[WTI_ETHERNET_HEADER_SIZE + WTI_MPLS_LABEL_SIZE],
#else
   memcpy(&cfg_ptr->prefix_header[WTI_PPP_HEADER_SIZE + WTI_MPLS_LABEL_SIZE],
#endif
          pw_config.ip_header,
          WTI_L3_HEADER_LEN);
#endif /* WTI_CESOP_NATIVE_MPLS_IW */

   /* set PWE3 control word */
   if (pw_config.control_word)
   {
#if WTI_CESOP_NATIVE_MPLS_IW
      memcpy(&cfg_ptr->prefix_header[WTI_MPLS_LABEL_SIZE +
                                     WTI_L3_HEADER_LEN],
             pw_config.control_word,
             WTI_CONTROL_WORD_LEN);
#else
#if WTI_CESOP_MPLS_OVER_ENET
      memcpy(&cfg_ptr->prefix_header[WTI_ETHERNET_HEADER_SIZE +
#else
      memcpy(&cfg_ptr->prefix_header[WTI_PPP_HEADER_SIZE +
#endif
                                     WTI_MPLS_LABEL_SIZE +
                                     WTI_L3_HEADER_LEN],
             pw_config.control_word,
             WTI_CONTROL_WORD_LEN);
#endif /* WTI_CESOP_NATIVE_MPLS_IW */
   }

   /* set RTP header */
   if (pw_config.rtp_header)
   {
#if WTI_CESOP_NATIVE_MPLS_IW
      memcpy(&cfg_ptr->prefix_header[WTI_MPLS_LABEL_SIZE +
                                     WTI_L3_HEADER_LEN +
                                     WTI_CONTROL_WORD_LEN],
             pw_config.rtp_header,
             WTI_RTP_HEADER_LEN);
#else
#if WTI_CESOP_MPLS_OVER_ENET
      memcpy(&cfg_ptr->prefix_header[WTI_ETHERNET_HEADER_SIZE +
#else
      memcpy(&cfg_ptr->prefix_header[WTI_PPP_HEADER_SIZE +
#endif
                                     WTI_MPLS_LABEL_SIZE +
                                     WTI_L3_HEADER_LEN +
                                     WTI_CONTROL_WORD_LEN],
             pw_config.rtp_header,
             WTI_RTP_HEADER_LEN);
#endif /* WTI_CESOP_NATIVE_MPLS_IW */
   }
#else
   if (pw_config.enet_header)
   {
      memcpy(cfg_ptr->prefix_header,
             pw_config.enet_header,
             WTI_L2_HEADER_LEN);
   }
   if (pw_config.udp_header)
   {
      memcpy(&cfg_ptr->prefix_header[WTI_L2_HEADER_LEN+WTI_L3_HEADER_LEN],
             pw_config.udp_header,
             WTI_L4_HEADER_LEN);
   }
   if (pw_config.rtp_header)
   {
      memcpy(&cfg_ptr->prefix_header[WTI_L2_HEADER_LEN+
                                     WTI_L3_HEADER_LEN+
                                     WTI_L4_HEADER_LEN],
             pw_config.rtp_header,
             WTI_RTP_HEADER_LEN);
   }

   if (pw_config.control_word)
   {
      memcpy(&cfg_ptr->prefix_header[WTI_L2_HEADER_LEN+
                                     WTI_L3_HEADER_LEN+
                                     WTI_L4_HEADER_LEN+
                                     WTI_RTP_HEADER_LEN],
             pw_config.control_word,
             WTI_CONTROL_WORD_LEN);
   }

   if (!pw_config.ip_header)
   {
      printf("ip_header definition is not set to pw_config\n");
      exit(1);
   }

   pw_config.ip_header->checksum = 0;
   pw_config.ip_header->total_length = (WP_U16) (cfg_ptr->payload_size +
                                                 WTI_L3_HEADER_LEN+
                                                 WTI_L4_HEADER_LEN+
                                                 WTI_RTP_HEADER_LEN+
                                                 WTI_CONTROL_WORD_LEN);

   pw_config.ip_header->checksum = WTI_IpChecksumCalc((WP_U8 *)pw_config.ip_header, WTI_L3_HEADER_LEN);
   memcpy(&cfg_ptr->prefix_header[WTI_L2_HEADER_LEN],
          pw_config.ip_header,
          WTI_L3_HEADER_LEN);
#endif

#if (!WTI_CESOP_MPLS_IW)
   /* CAS mode */
#if WTI_CESOP_TDI
   if (tdm_config[pw_config.line_index].cas_mode == WTI_TDI_CAS_MODE && oobc_mode == 0)
#else
      if ((pw_config.mode == (WP_EMPHY_TRANS_FRAMED_CAS | WP_EMPHY_TRANS_T1) ||
           pw_config.mode == (WP_EMPHY_TRANS_FRAMED_CAS | WP_EMPHY_TRANS_E1)) && oobc_mode == 0)
#endif
   {
      WP_U16 cas_size;
      WP_U8 structure_size, num_slots;

#if WTI_CESOP_TDI
      structure_size = (tdm_config[pw_config.line_index].op_mode == WTI_TDI_E1_OP_MODE) ?
         WP_TDM_STRUCTURE_E1_ESF :
         WP_TDM_STRUCTURE_T1_ESF;
#else
      if (pw_config.mode == (WP_EMPHY_TRANS_FRAMED_CAS | WP_EMPHY_TRANS_T1))
         structure_size = WP_TDM_STRUCTURE_T1_ESF;
      else
         structure_size = WP_TDM_STRUCTURE_E1_ESF;
#endif
      num_slots = pw_config.n_slots;
      cas_size = (num_slots % 2) ? ((num_slots / 2) + 1) : (num_slots / 2);

      /* if FRG mode - calculate and set last packet ip checksum and CW length*/
      if (cfg_ptr->payload_size != (structure_size * num_slots))
      {

         pw_config.ip_header->checksum = 0;
         pw_config.ip_header->total_length += cas_size;

         cfg_ptr->last_packet_ip_checksum =
            WTI_IpChecksumCalc((WP_U8 *)pw_config.ip_header, WTI_L3_HEADER_LEN);
         cfg_ptr->last_packet_cw_length = cfg_ptr->payload_size + cas_size;
         cfg_ptr->l4_length_update_mode = WP_ENABLE;
      }
   }
#endif
#if WTI_CESOP_MEF8_ENCAP
      memcpy(&cfg_ptr->prefix_header[0], &enet_header, WTI_ETHERNET_HEADER_SIZE +
             WTI_VLAN_TAG_SIZE*WTI_NUM_OF_VLAN_TAGS);
      memcpy(&cfg_ptr->prefix_header[WTI_ETHERNET_HEADER_SIZE +
             WTI_VLAN_TAG_SIZE*WTI_NUM_OF_VLAN_TAGS], &ecid_header, WTI_ECID_LABEL_SIZE);
      memcpy(&cfg_ptr->prefix_header[cfg_ptr->control_word_offset],
             pw_config.control_word, WTI_CONTROL_WORD_LEN);
      memcpy(&cfg_ptr->prefix_header[cfg_ptr->rtp_header_offset],
             pw_config.rtp_header, WTI_RTP_HEADER_LEN);
#endif

   memcpy(&pw_config.tdm2psn_flow_agg_config, cfg_ptr, sizeof(WP_iw_agg_cesop));
}


#if WTI_CESOP_MEF8_ENCAP
/*****************************************************************************
 * Function name: WTI_FlowAggregationMef8Psn2TdmConfigInit
 * Description  : initialize PSN to TDM flow aggregation configuration structure
 *                with default values
 * Input  params: cfg_ptr - pointer to configuration structure
 * Output params:
 * Return val   : none
 *****************************************************************************/
void WTI_FlowAggregationMef8Psn2TdmConfigInit(WP_iw_agg_cesop *cfg_ptr)
{
   memset(cfg_ptr, 0, sizeof(WP_iw_agg_cesop));

   cfg_ptr->tag = 0;
   cfg_ptr->txfunc = WTI_INVALID_HANDLE;
   cfg_ptr->intmode = WTI_PSN2TDM_INTERRUPT_MODE;
   cfg_ptr->interruptqueue = WTI_PSN2TDM_INTERRUPT_QUEUE;
   cfg_ptr->statmode = WTI_PSN2TDM_STAT_MODE;
   cfg_ptr->timestamp_mode = WTI_PSN2TDM_TIMESTAMP_MODE;
   cfg_ptr->ov_pool_mode = WTI_PSN2TDM_OV_POOL_MODE;
   cfg_ptr->oobc_mode = WP_DISABLE;
   cfg_ptr->fbp_drop_threshold = WTI_PSN2TDM_FBP_DROP_THRESHOLD;
   cfg_ptr->mtu = WTI_PSN2TDM_MTU;
   cfg_ptr->flow_agg_type = WP_IW_AGG_PRIMARY;
   cfg_ptr->rtp_header_mode = WTI_TDM2PSN_RTP_MODE;
   cfg_ptr->payload_size = 0;
   cfg_ptr->oobc_flow_handle = 0;
   cfg_ptr->prefix_length = 0;
   cfg_ptr->l2_length_update_mode = 0;
   cfg_ptr->l2_length_offset = 0;
   cfg_ptr->l3_header_offset = 0;
   cfg_ptr->l4_length_update_mode = 0;
   cfg_ptr->l4_length_offset = 0;
   cfg_ptr->l4_checksum_recalc_mode = 0;
   cfg_ptr->last_packet_cw_length = 0;
   cfg_ptr->last_packet_ip_checksum = 0;
   cfg_ptr->initial_seq_number = 0;

   cfg_ptr->l3_protocol = WP_IW_CESOP_L3_MPLS; /* WP_IW_CESOP_L3_ECID;*/
   cfg_ptr->control_word_offset = (WTI_ETHERNET_HEADER_SIZE+
                                   WTI_NUM_OF_VLAN_TAGS * WTI_VLAN_TAG_SIZE+
                                   WTI_ECID_LABEL_SIZE);

   cfg_ptr->rtp_header_offset = (cfg_ptr->control_word_offset +
                                 WTI_CONTROL_WORD_LEN);
   cfg_ptr->extraction_length = (cfg_ptr->rtp_header_offset +
                                 WTI_RTP_HEADER_LEN);
   cfg_ptr->cas_location = WTI_CAS_LOCATION;
   cfg_ptr->control_word_mode = WTI_PSN2TDM_CONTROL_WORD_MODE;
   cfg_ptr->payload_type_check = WTI_PSN2TDM_PAYLOAD_TYPE_CHECK;
   cfg_ptr->payload_type = WTI_PSN2TDM_PAYLOAD_TYPE;
   cfg_ptr->oobc_payload_type = WTI_PSN2TDM_OOBC_PAYLOAD_TYPE;
   cfg_ptr->ssrc_check = WTI_PSN2TDM_SSRC_TYPE_CHECK;
   cfg_ptr->ssrc = WTI_PSN2TDM_SSRC_TYPE;
   cfg_ptr->oobc_ssrc = WTI_PSN2TDM_OOBC_SSRC_TYPE;
}
#endif /*WTI_CESOP_MEF8_ENCAP*/

/*****************************************************************************
 * Function name: WTI_FlowAggregationPsn2TdmConfigInit
 * Description  : initialize PSN to TDM flow aggregation configuration structure
 *                with default values
 * Input  params: cfg_ptr - pointer to configuration structure
 * Output params:
 * Return val   : none
 *****************************************************************************/
void WTI_FlowAggregationPsn2TdmConfigInit(WP_iw_agg_cesop *cfg_ptr)
{
   memset(cfg_ptr, 0, sizeof(WP_iw_agg_cesop));

   cfg_ptr->tag = 0;
   cfg_ptr->txfunc = WTI_INVALID_HANDLE;
   cfg_ptr->intmode = WTI_PSN2TDM_INTERRUPT_MODE;
   cfg_ptr->interruptqueue = WTI_PSN2TDM_INTERRUPT_QUEUE;
   cfg_ptr->statmode = WTI_PSN2TDM_STAT_MODE;
   cfg_ptr->timestamp_mode = WTI_PSN2TDM_TIMESTAMP_MODE;
   cfg_ptr->ov_pool_mode = WTI_PSN2TDM_OV_POOL_MODE;
   cfg_ptr->oobc_mode = WP_DISABLE;
   cfg_ptr->fbp_drop_threshold = WTI_PSN2TDM_FBP_DROP_THRESHOLD;
   cfg_ptr->mtu = WTI_PSN2TDM_MTU;
   cfg_ptr->flow_agg_type = WP_IW_AGG_PRIMARY;
   cfg_ptr->rtp_header_mode = WTI_TDM2PSN_RTP_MODE;
   cfg_ptr->payload_size = 0;
   cfg_ptr->oobc_flow_handle = 0;
   cfg_ptr->prefix_length = 0;
   cfg_ptr->l2_length_update_mode = 0;
   cfg_ptr->l2_length_offset = 0;
   cfg_ptr->l3_header_offset = 0;
   cfg_ptr->l4_length_update_mode = 0;
   cfg_ptr->l4_length_offset = 0;
   cfg_ptr->l4_checksum_recalc_mode = 0;
   cfg_ptr->last_packet_cw_length = 0;
   cfg_ptr->last_packet_ip_checksum = 0;
   cfg_ptr->initial_seq_number = 0;
#if WTI_CESOP_MPLS_IW
   cfg_ptr->l3_protocol = WP_IW_CESOP_L3_MPLS;
   cfg_ptr->control_word_offset = (WTI_MPLS_LABEL_SIZE +
                                   WTI_L3_HEADER_LEN);
   cfg_ptr->rtp_header_offset = (WTI_MPLS_LABEL_SIZE +
                                 WTI_L3_HEADER_LEN +
                                 WTI_CONTROL_WORD_LEN);
   cfg_ptr->extraction_length = (WTI_MPLS_LABEL_SIZE +
                                 WTI_L3_HEADER_LEN +
                                 WTI_CONTROL_WORD_LEN +
                                 WTI_RTP_HEADER_LEN);
#if WTI_PCE_CLASSIFIER
#if WTI_CESOP_TO_MPLS_FA_CASCADING
    /* In case the PCE is used, it classifies the packets based on the second MPLS label
       but the first label and ENET header (added by the L2 router MPLS FA)  still exist
       in the packet and therefore we need to remove them */
   /* In MPLS cascading the Ethernet header has 1 VLAN */
   cfg_ptr->extraction_length += (WTI_L2_HEADER_LEN + WTI_VLAN_TAG_SIZE + WTI_MPLS_LABEL_SIZE);
   cfg_ptr->control_word_offset += (WTI_L2_HEADER_LEN + WTI_VLAN_TAG_SIZE + WTI_MPLS_LABEL_SIZE);
   cfg_ptr->rtp_header_offset += (WTI_L2_HEADER_LEN + WTI_VLAN_TAG_SIZE + WTI_MPLS_LABEL_SIZE);
#endif
#endif

#else
   cfg_ptr->l3_protocol = WP_IW_CESOP_L3_IPV4;
   cfg_ptr->control_word_offset = (WTI_L3_HEADER_LEN + WTI_L4_HEADER_LEN +
                                   WTI_RTP_HEADER_LEN);
   cfg_ptr->rtp_header_offset = (WTI_L3_HEADER_LEN + WTI_L4_HEADER_LEN);
   cfg_ptr->extraction_length = (WTI_L3_HEADER_LEN +
                                 WTI_L4_HEADER_LEN +
                                 WTI_RTP_HEADER_LEN +
                                 WTI_CONTROL_WORD_LEN);
#endif
   cfg_ptr->cas_location = WTI_CAS_LOCATION;
   cfg_ptr->control_word_mode = WTI_PSN2TDM_CONTROL_WORD_MODE;
   cfg_ptr->payload_type_check = WTI_PSN2TDM_PAYLOAD_TYPE_CHECK;
   cfg_ptr->payload_type = WTI_PSN2TDM_PAYLOAD_TYPE;
   cfg_ptr->oobc_payload_type = WTI_PSN2TDM_OOBC_PAYLOAD_TYPE;
   cfg_ptr->ssrc_check = WTI_PSN2TDM_SSRC_TYPE_CHECK;
   cfg_ptr->ssrc = WTI_PSN2TDM_SSRC_TYPE;
   cfg_ptr->oobc_ssrc = WTI_PSN2TDM_OOBC_SSRC_TYPE;
}

/*****************************************************************************
 * Function name: WTI_FlowAggregationPsn2TdmConfig
 * Description  : initialize PSN to TDM flow aggregation configuration structure
 *                with actual values after user configuration
 * Input  params: cfg_ptr - pointer to configuration structure
 *                pw_index - PW index
 *                oobc_mode - out of band CAS mode (0-disable,1-enable)
 *                clock_master - clock master mode (the clock is recovered according to
 *                               packets arriving to this flow)(0-disable,1-enable)
 * Output params:
 * Return val   : none
 *****************************************************************************/
void WTI_FlowAggregationPsn2TdmConfig(WP_iw_agg_cesop *cfg_ptr,
                                      int pw_index,
                                      int oobc_mode)
{
   memcpy(cfg_ptr, &pw_config.psn2tdm_flow_agg_config, sizeof(WP_iw_agg_cesop));

   cfg_ptr->txfunc = the_system->pw[pw_index].trans_tx;
   cfg_ptr->payload_size = pw_config.rx_buffersize;
   cfg_ptr->oobc_mode = oobc_mode;
#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
   cfg_ptr->clock_rec_params = 0;
#endif
   cfg_ptr->timestamp_mode = pw_config.psn2tdm_flow_agg_config.timestamp_mode;

   if (cfg_ptr->timestamp_mode == WP_IW_TIME_STAMP_ENABLE)
   {
      cfg_ptr->ts_method = pw_config.psn2tdm_flow_agg_config.ts_method;
      cfg_ptr->ts_diff_if_id = pw_config.psn2tdm_flow_agg_config.ts_diff_if_id;
      cfg_ptr->ts_const = pw_config.psn2tdm_flow_agg_config.ts_const;
   }
   else
   {
      cfg_ptr->ts_method = 0;
      cfg_ptr->ts_diff_if_id = 0;
      cfg_ptr->ts_const = 0;
   }
   cfg_ptr->buffer_gap = 64; /* should be the same value as in the IW system */
}


#if WTI_CESOP_MEF8_ENCAP
void WTI_DfcFilterConfigMef(wp_classification_filter *cfg_ptr)
{
   cfg_ptr->protocol_type = WP_CLASS_FILTER_IPV4;
   cfg_ptr->fields_mask = WP_FIELD_IDS_ARRAY;
   cfg_ptr->no_match_action = WP_CLASS_FILTER_NO_MATCH_DENY;
   cfg_ptr->field_ids_array[0] = WP_FIELD_ID_ECID;
   cfg_ptr->field_ids_array[1] = WP_FIELD_ID_LAST;
   cfg_ptr->filter_type     = WP_CLASS_FILTER_EXT_ACTION_EMC;

}
#endif /*WTI_CESOP_MEF8_ENCAP*/


#if WTI_PCE_CLASSIFIER

void WTI_CheckPceFilterStats(WP_handle filter, WP_U16 diff_denied, WP_U16 diff_matched, WP_U16 diff_no_matched)
{
   WP_CHAR log_text[200];
   WP_U32 temp1, temp2, diff;
   WP_pce_filter_stat filter_stat_old;
   WP_U8 index = (WP_U8)(filter & 0xFF);

   memcpy(&filter_stat_old, &(pce_filter_stats[index]), sizeof(WP_pce_filter_stat));

   WP_PceFilterStatistics (filter, &(pce_filter_stats[index]));

   MY_PRINTF("\n\nComparing PCE Filter %d Statistic Changes\n", index);

   temp1 = pce_filter_stats[index].denied_pkts;
   temp2 = pce_filter_stats[index].denied_pkts >> 32;
   diff = pce_filter_stats[index].denied_pkts - filter_stat_old.denied_pkts;
   MY_PRINTF("denied_pkts            %08X %08X       diff %04X\n", temp2, temp1, diff);
   if(diff != diff_denied)
   {
      sprintf(log_text, "Fliter Statistics Error: denied packets number should be %04X\n", diff_denied);
      WPE_TEST_ADD_ERROR(log_text);
   }

   temp1 = pce_filter_stats[index].matched_pkts;
   temp2 = pce_filter_stats[index].matched_pkts >> 32;
   diff = pce_filter_stats[index].matched_pkts - filter_stat_old.matched_pkts;
   MY_PRINTF("matched_pkts           %08X %08X       diff %04X\n", temp2, temp1, diff);
   if(diff != diff_matched)
   {
      sprintf(log_text, "Fliter Statistics Error: matched packets number should be %04X\n", diff_matched);
      WPE_TEST_ADD_ERROR(log_text);
   }

   temp1 = pce_filter_stats[index].no_matched_pkts;
   temp2 = pce_filter_stats[index].no_matched_pkts >> 32;
   diff = pce_filter_stats[index].no_matched_pkts - filter_stat_old.no_matched_pkts;
   MY_PRINTF("no_matched_pkts        %08X %08X       diff %04X\n", temp2, temp1, diff);
   if(diff != diff_no_matched)
   {
      sprintf(log_text, "Fliter Statistics Error: not matched packets number should be %04X\n", diff_no_matched);
      WPE_TEST_ADD_ERROR(log_text);
   }

   MY_PRINTF("\n\n");
}

/*****************************************************************************
 * Function name: WTI_PceFilterConfig
 * Description  : configure pce filter
 * Input  params: cfg_ptr - pointer to configuration structure
 * Output params:
 * Return val   : none
 *****************************************************************************/
void WTI_PceFilterConfig(WP_pce_filter_classification *cfg_ptr)
{
   memset(cfg_ptr, 0, sizeof(WP_pce_filter_classification));

   cfg_ptr->tag = 1;
   cfg_ptr->no_match_action = WP_PCE_FILTER_NO_MATCH_DENY;
   cfg_ptr->no_fields_action = WP_PCE_FILTER_NO_FIELDS_DENY;
   cfg_ptr->no_match_result[0].result_type = WP_PCE_RESULT_LAST;
#if (!WTI_CESOP_MPLS_IW)
   cfg_ptr->filter_fields[0].field_id = WP_PCE_FIELD_ID_IPV4_DA;
   cfg_ptr->filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   cfg_ptr->filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;
#if UDP_DES_PORT_PCE_RULE
   cfg_ptr->filter_fields[1].field_id = WP_PCE_FIELD_ID_L4_DST_PORT;
   cfg_ptr->filter_fields[1].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   cfg_ptr->filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   cfg_ptr->filter_fields[2].field_id = WP_PCE_FIELD_ID_LAST;
#else
   cfg_ptr->filter_fields[1].field_id = WP_PCE_FIELD_ID_LAST;
#endif
#else /*WTI_CESOP_MPLS_IW*/
   cfg_ptr->filter_fields[0].field_id = WP_PCE_FIELD_ID_MPLS_HEADER2;
   cfg_ptr->filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   cfg_ptr->filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;
   cfg_ptr->filter_fields[1].field_id = WP_PCE_FIELD_ID_LAST;
   cfg_ptr->no_match_result[0].result_type = WP_PCE_RESULT_LAST;
#endif /*WTI_CESOP_MPLS_IW*/
}
#else /*WP_HW_WINPATH*/
#if (!WTI_CESOP_MPLS_IW)
/*****************************************************************************
 * Function name: WTI_DfcFilterConfig
 * Description  : configure dfc filter
 * Input  params: cfg_ptr - pointer to configuration structure
 * Output params:
 * Return val   : none
 *****************************************************************************/
void WTI_DfcFilterConfig(wp_classification_filter *cfg_ptr)
{
   memset(cfg_ptr, 0, sizeof(wp_classification_filter));

#if WTI_CESOP_MEF8_ENCAP
   WTI_DfcFilterConfigMef(cfg_ptr);
   return;
#endif /*WTI_CESOP_MEF8_ENCAP*/
   cfg_ptr->filter_type     = WP_CLASS_FILTER_EMC;

#if UDP_DES_PORT_DFC_RULE
   cfg_ptr->fields_mask     = WP_FIELD_IP_DA | WP_FIELD_DST_PORT;
   cfg_ptr->no_match_action = WP_CLASS_FILTER_NO_MATCH_DENY;
#else /*UDP_DES_PORT_DFC_RULE*/
   cfg_ptr->fields_mask     = WP_FIELD_IP_DA;
   cfg_ptr->no_match_action = WP_CLASS_FILTER_NO_MATCH_DENY;
#endif /* UDP_DES_PORT_DFC_RULE */
}
#endif /* (!WTI_CESOP_MPLS_IW) */
#endif /* WINPATH */

/*****************************************************************************
 * Function name: WTI_Tdm2PsnIwSystemConfig
 * Description  : configure IW system in the TDM to PSN direction
 * Input  params: cfg_ptr - pointer to configuration structure
 * Output params:
 * Return val   : none
 *****************************************************************************/
void WTI_Tdm2PsnIwSystemConfig(WP_iw_sys_directmap *cfg_ptr)
{
   memset(cfg_ptr, 0, sizeof(WP_iw_sys_directmap));

   cfg_ptr->buffer_gap = 64;
}

#if (WTI_CESOP_CLOCK_RECOVERY_ENABLE)
void WTI_ClockRecDcoParamsCalc(WP_U32 ref_clock,
                               WP_clock_rec_freq *dco_e1_params,
                               WP_clock_rec_freq *dco_t1_params)
{

   WP_status status;
   WP_U32 int_part, frac_part, limit_part;

   /* calculate DCO parameters for E1 */
   status = WP_ClockRecDcoDivisorCalc(WTI_CLOCK_REC_NOMINAL_FREQ_E1,
                                      ref_clock,
                                      WTI_CLOCK_REC_DCO_PPM_OFFSET,
                                      &int_part,
                                      &frac_part,
                                      &limit_part);
   WTI_TerminateOnError(status,
                        "WP_ClockRecDcoDivisorCalc: DCO fractional part is too close to 1 or to 0",
                        __LINE__);

   dco_e1_params->nominal_divisor_int = int_part;
   dco_e1_params->nominal_divisor_rem = frac_part;
   dco_e1_params->max_clock_delta = limit_part;

   /* calculate DCO parameters for T1 */
   status = WP_ClockRecDcoDivisorCalc(WTI_CLOCK_REC_NOMINAL_FREQ_T1,
                                      ref_clock,
                                      WTI_CLOCK_REC_DCO_PPM_OFFSET,
                                      &int_part,
                                      &frac_part,
                                      &limit_part);
   WTI_TerminateOnError(status,
                        "WP_ClockRecDcoDivisorCalc: DCO fractional part is too close to 1 or to 0",
                        __LINE__);

   dco_t1_params->nominal_divisor_int = int_part;
   dco_t1_params->nominal_divisor_rem = frac_part;
   dco_t1_params->max_clock_delta = limit_part;
}
#endif

#if (WTI_CESOP_CLOCK_RECOVERY_ENABLE)
/*****************************************************************************
 * Function name: WTI_ClockRecDcoParamsSet
 * Description  : Calculate and set DCO parameters.
 * Input  params: cfg_ptr - pointer to configuration structure,
 *                external_ref_clk - external reference clock frequency in Hz.
 * Output params:
 * Return val   : none
 *****************************************************************************/
void WTI_ClockRecDcoParamsSet(WP_feature_iw_cesop *cfg_ptr, WP_U32 external_ref_clk)
{
   WP_U32 freq_in_mhz, internal_freq;
#if (WTI_DEBUG_LEVEL > 0)
   WP_clock_rec_freq *dco_e1, *dco_t1;
#endif
   if (cfg_ptr->dco_clock_selection == WP_CLOCK_REC_DCO_CLOCK_INTERNAL)
   {
      /* Internal clock selected. Extract WP internal frequency and calculate DCO params */
      internal_freq = WPL_GetProcessorSpeed();
      freq_in_mhz = WTI_GetMhzValue(internal_freq);

      WTI_ClockRecDcoParamsCalc(freq_in_mhz*1000000,
                                &clock_rec_dco_e1_params, &clock_rec_dco_t1_params);
      cfg_ptr->e1_freq = &clock_rec_dco_e1_params;
      cfg_ptr->t1_freq = &clock_rec_dco_t1_params;

#if (WTI_DEBUG_LEVEL > 0)
      dco_e1 = cfg_ptr->e1_freq;
      dco_t1 = cfg_ptr->t1_freq;

      printf("In WTI_ClockRecDcoParamsSet: internal clock selected:\n");
      printf("e1_freq.int = 0x%x, e1_freq.frac = 0x%x, e1_freq.limit = 0x%x\n",
             dco_e1->nominal_divisor_int, dco_e1->nominal_divisor_rem,
             dco_e1->max_clock_delta);
      printf("t1_freq.int = 0x%x, t1_freq.frac = 0x%x, t1_freq.limit = 0x%x\n",
             dco_t1->nominal_divisor_int, dco_t1->nominal_divisor_rem,
             dco_t1->max_clock_delta);
#endif
   }
   else if (external_ref_clk == 0)
   {
      /* When working with external clock and no reference clock provided,
         assume 100MHz and use default values (internal by the WDDI). */
      cfg_ptr->e1_freq = 0;
      cfg_ptr->t1_freq = 0;
#if (WTI_DEBUG_LEVEL > 0)
      printf("In WTI_ClockRecDcoParamsSet: working with default DCO params\n");
#endif
   }
   else
   {
      /* External clock selected. Calculate DCO params using given reference clock */
      if ((WP_U32)(external_ref_clk/1000)*1000 != external_ref_clk)
      {
         printf("ERROR: Illegal value of external reference clock %d\n", external_ref_clk);
         return;
      }
      WTI_ClockRecDcoParamsCalc(external_ref_clk,
                                &clock_rec_dco_e1_params, &clock_rec_dco_t1_params);
      cfg_ptr->e1_freq = &clock_rec_dco_e1_params;
      cfg_ptr->t1_freq = &clock_rec_dco_t1_params;
#if (WTI_DEBUG_LEVEL > 0)
      dco_e1 = cfg_ptr->e1_freq;
      dco_t1 = cfg_ptr->t1_freq;
      printf("In WTI_ClockRecDcoParamsSet: external clock selected: %dHz\n", external_ref_clk);
      printf("e1_freq.int = 0x%x, e1_freq.frac = 0x%x, e1_freq.limit = 0x%x\n",
             dco_e1->nominal_divisor_int, dco_e1->nominal_divisor_rem,
             dco_e1->max_clock_delta);
      printf("t1_freq.int = 0x%x, t1_freq.frac = 0x%x, t1_freq.limit = 0x%x\n",
             dco_t1->nominal_divisor_int, dco_t1->nominal_divisor_rem,
             dco_t1->max_clock_delta);
#endif
   }
}
#endif

/*****************************************************************************
 * Function name: WTI_CesopFeatureConfig
 * Description  : configure CESoP feature structure
 * Input  params: cfg_ptr - pointer to configuration structure
 * Output params:
 * Return val   : none
 *****************************************************************************/
void WTI_CesopFeatureConfig(WP_feature_iw_cesop *cfg_ptr)
{
#if (!WTI_CESOP_TDI) & (WTI_CESOP_CLOCK_RECOVERY_ENABLE || WTI_CESOP_RX_TIMING_ENABLE)
   WUFE_clk_rec_info_system clk_rec_info_system;
#endif

   memset(cfg_ptr, 0, sizeof(WP_feature_iw_cesop));
   memset(&the_system->rx_ts_params, 0, sizeof(WP_rx_ts_global_params));

#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
   memset(&the_system->tx_ts_params, 0, sizeof(WP_tx_ts_global_params));
#endif

#if (!WTI_CESOP_TDI) & (WTI_CESOP_CLOCK_RECOVERY_ENABLE || WTI_CESOP_RX_TIMING_ENABLE)
   memset(&clk_rec_info_system, 0, sizeof(WUFE_clk_rec_info_system));
#ifdef __WT_UFE3__
   WT_UfeClockRecInfoGet(the_system->ufe.ufe_id,
                         0,
                         WUFE_CLOCK_REC_INFO_SYSTEM,
                         &clk_rec_info_system);
#endif /* WT_UFE3 */
#endif



#if WTI_CESOP_RX_TIMING_ENABLE

#if WTI_CESOP_TDI
#if WTI_ACTIVE_ABSOLUTE_MODE
   the_system->rx_ts_params.ts_base = 0;
   the_system->rx_ts_params.max_ts_interfaces = 0;
   the_system->rx_ts_params.ts_bustype = 0;
   the_system->rx_ts_params.ts_portsize = 0;
   cfg_ptr->rx_ts_params = 0;
#else /* not in ABSOLUTE mode */
   the_system->rx_ts_params.ts_base = WTI_TDI_RX_TS_BASE_ADDRESS;
   the_system->rx_ts_params.max_ts_interfaces = 0;
   the_system->rx_ts_params.ts_bustype = WTI_CLOCK_REC_BUSTYPE;
   the_system->rx_ts_params.ts_portsize = UFE_MEM_PORT_SIZE;
   cfg_ptr->rx_ts_params = &the_system->rx_ts_params;
#endif /* WTI_ACTIVE_ABSOLUTE_MODE */
#else
   /* init the RX TS params */
#ifdef __WT_UFE3__
   the_system->rx_ts_params.ts_base = WTI_RX_TS_BASE_ADDRESS;
   the_system->rx_ts_params.max_ts_interfaces = clk_rec_info_system.max_rx_clk_rec_if;
   the_system->rx_ts_params.max_ext_diff_ts_interfaces = clk_rec_info_system.max_ext_diff_rx_clk_rec_if;
   the_system->rx_ts_params.ts_bustype = WTI_CLOCK_REC_BUSTYPE;
   the_system->rx_ts_params.ts_portsize = UFE_MEM_PORT_SIZE;
   cfg_ptr->rx_ts_params = &the_system->rx_ts_params;
#else
   the_system->rx_ts_params.ts_base = 0;
   the_system->rx_ts_params.max_ts_interfaces = WTI_CESOP_MAX_CLOCK_REC_INTERFACES;
   the_system->rx_ts_params.max_ext_diff_ts_interfaces = 0;
   the_system->rx_ts_params.ts_bustype = 0;
   the_system->rx_ts_params.ts_portsize = 0;
   cfg_ptr->rx_ts_params = &the_system->rx_ts_params;
#endif /* WT_UFE3 */
#endif /* WTI_CESOP_TDI */
#else /* WTI_CESOP_RX_TIMING_ENABLE */
   the_system->rx_ts_params.ts_base = 0;
   the_system->rx_ts_params.max_ts_interfaces = 0;
   the_system->rx_ts_params.max_ext_diff_ts_interfaces = 0;
   the_system->rx_ts_params.ts_bustype = 0;
   the_system->rx_ts_params.ts_portsize = 0;
   cfg_ptr->rx_ts_params = 0;
#endif



   cfg_ptr->max_oobc = N_MAX_OUT_OF_BAND_CAS_FLOWS;
   cfg_ptr->max_abs_ts_pw = WTI_MAX_ABS_PW;
#if WTI_CESOP_TDI
   cfg_ptr->port_interface = WP_IW_CESOP_PORT_TDM;
#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
#if WTI_CLOCK_REC_MODE
   /* work in differential mode. init the TX TS params */
   the_system->tx_ts_params.ts_base = WTI_TDI_TX_TS_BASE_ADDRESS;
   the_system->tx_ts_params.ts_clocks_ratio = 0;
   cfg_ptr->tx_ts_params = &the_system->tx_ts_params;
#else /* work in adaptive mode */
   /* init the TX TS params */
   the_system->tx_ts_params.ts_base = 0;
   the_system->tx_ts_params.ts_clocks_ratio = 0;
   cfg_ptr->tx_ts_params = 0;
#endif /* WTI_CLOCK_REC_MODE */
#endif /* WTI_CESOP_CLOCK_RECOVERY_ENABLE */
#else /* WTI_CESOP_TDI */
   cfg_ptr->port_interface = WP_IW_CESOP_PORT_UFE;
#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
#if __WT_UFE3__
   /* init the TX TS params */
   the_system->tx_ts_params.ts_base = WTI_TX_TS_BASE_ADDRESS;
   the_system->tx_ts_params.ts_clocks_ratio = WTI_TS_CLOCKS_RATIO;
   cfg_ptr->tx_ts_params = &the_system->tx_ts_params;
#else
   cfg_ptr->tx_ts_params = 0;
#endif /* WT_UFE3 */
#endif /* WTI_CESOP_CLOCK_RECOVERY_ENABLE */
#endif /* WTI_CESOP_TDI */

#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
   /* set hardware logic base address */
#if WTI_CESOP_TDI
   cfg_ptr->clock_rec_base = WTI_NONE_ZERO;
   cfg_ptr->clock_rec_bustype = WTI_CLOCK_REC_BUSTYPE;
#else
   cfg_ptr->clock_rec_base = WTI_CLOCK_REC_UFE_BASE;
   cfg_ptr->clock_rec_bustype = WTI_CLOCK_REC_BUSTYPE;
#endif
   cfg_ptr->clock_rec_portsize = UFE_MEM_PORT_SIZE;

#if WTI_CESOP_TDI
   cfg_ptr->max_clock_rec_interfaces = WTI_TDM_PORTS;
#else
#ifdef __WT_UFE3__
   cfg_ptr->max_clock_rec_interfaces = clk_rec_info_system.max_tx_clk_rec_if;
   cfg_ptr->max_clock_rec_ext_diff_interfaces = clk_rec_info_system.max_ext_diff_tx_clk_rec_if;
#else
   cfg_ptr->max_clock_rec_interfaces = WTI_CESOP_MAX_CLOCK_REC_INTERFACES;
   cfg_ptr->max_clock_rec_ext_diff_interfaces = 0;
#endif  /* WT_UFE3 */
#endif /* WTI_CESOP_TDI */

#if WTI_2_ENET_DEVICES
   cfg_ptr->input_devices_num = 2;
   cfg_ptr->clock_rec_input_devices[0] = the_system->enet_device;
   cfg_ptr->clock_rec_input_devices[1] = the_system->enet_device1;
#else
   cfg_ptr->input_devices_num = 1;
   cfg_ptr->clock_rec_input_devices[0] = the_system->enet_device;
#endif

#if WTI_4_ENET_DEVICES
   cfg_ptr->input_devices_num = 4;
   cfg_ptr->clock_rec_input_devices[0] = the_system->enet_device;
   cfg_ptr->clock_rec_input_devices[1] = the_system->enet_device1;
   cfg_ptr->clock_rec_input_devices[2] = the_system->enet_device2;
   cfg_ptr->clock_rec_input_devices[3] = the_system->enet_device3;
#endif

#if (WTI_CESOP_TDI && WTI_CESOP_CLOCK_RECOVERY_ENABLE)
   /* set DCO registers nominal values */
   cfg_ptr->sync_delay = WP_CLOCK_REC_DCO_SYNC_DELAY_1;
   cfg_ptr->dco_clock_selection = WTI_TDI_DCO_CLOCK_SOURCE;
   cfg_ptr->sync_mode = WP_CLOCK_REC_DCO_SYNC_RISING_EGDE;
   cfg_ptr->sync_active_mode = WP_CLOCK_REC_DCO_SYNC_ACTIVE_HIGH;
   cfg_ptr->tdi_ts_clock_selection = WTI_TDI_TS_CLOCK_SOURCE;
  
#if WTI_CESOP_DIFF_DOCSIS_MODE
   cfg_ptr->tdi_ts_mode = WP_TDI_TS_MODE_DOCSIS;
#else
   cfg_ptr->tdi_ts_mode = WP_TDI_TS_MODE_NORMAL;
#endif

#if WTI_CI_TESTING_MODE_TDI 
   /* use PTP clcok source and set frequency */
   cfg_ptr->tdi_ts_clock_selection =WP_CLK_SRC_FCLK2;
#endif

   /* Update DCO parameters (integer, fractional and ppm limit) */
   WTI_ClockRecDcoParamsSet(cfg_ptr, global_clock_rec_reg_dco_ref_clock);

   /* for extended mode use default values. */
   cfg_ptr->ext_diff_t1_freq = 0;
   cfg_ptr->ext_diff_e1_freq = 0;

/* Patch: In WP2 use original method due to problem with reading the clock in WDS-CR boards */
#if WP_HW_WINPATH2
   if (cfg_ptr->dco_clock_selection == WP_CLOCK_REC_DCO_CLOCK_INTERNAL)
   {
      cfg_ptr->t1_freq = &clock_rec_default_t1_freq_for_300MHz;
      cfg_ptr->e1_freq = &clock_rec_default_e1_freq_for_300MHz;
   }
#endif
/* Patch end */

#else
   /* set nominal divisors to 0 which means use defaults values
    * for 100Mhz reference clock */
   cfg_ptr->t1_freq = 0;
   cfg_ptr->e1_freq = 0;
   cfg_ptr->ext_diff_t1_freq = 0;
   cfg_ptr->ext_diff_e1_freq = 0;

#endif /* WP2 & TDI & CR */
   cfg_ptr->ds3_freq = 0;
   cfg_ptr->e3_freq = 0;
   cfg_ptr->ext_diff_ds3_freq = 0;
   cfg_ptr->ext_diff_e3_freq = 0;

#endif /* WTI_CESOP_CLOCK_RECOVERY_ENABLE */
}

#if (WTI_CESOP_CLOCK_RECOVERY_ENABLE)
WP_U32 WTI_GetMhzValue(WP_U32 internal_freq)
{
   WP_U32 freq_in_mhz;

   freq_in_mhz = (WP_U32) (internal_freq / 1000000);
   /* round result */
   if ((internal_freq - (freq_in_mhz * 1000000)) > 500000)
      freq_in_mhz++;
   return (freq_in_mhz);
}
#endif

/*****************************************************************************
 * Function name: WTI_SyncEthInit
 * Description  : Init relevant SyncE clock and blocks
 * Input  params: none
 * Output params:
 * Return val   : none
 *****************************************************************************/
void WTI_SyncEthInit(void)
{
#ifdef WP_HW_WINPATH3
   WP_status status;
#endif

#ifdef WP_HW_WINPATH2
   /* SyncE is not supported */
#else
   WTI_SyncEthConfig(&sync_eth_config);
#if WTI_DEBUG_LEVEL > 1
   printf("Sync Ethernet Init:\n");
#endif /* WTI_DEBUG_LEVEL */

   status = WP_SyncEthSerdesClockOutSelect(the_system->wpid,&sync_eth_config);
   WTI_TerminateOnError(status, "WP_SyncEthSerdesClockOutSelect", __LINE__);
   /* Select the enet_clock mux used CR blocks (DCO,TS) */
   status = WP_SyncEthTimeStampSourceSelect(the_system->wpid,WT_SYNC_ENET_CLOCK_MASTER);
   WTI_TerminateOnError(status, "WP_SyncEthTimeStampSourceSelect", __LINE__);

#if WTI_DEBUG_LEVEL > 1
   printf("Sync Ethernet Config:\n");

   WP_Display(WP_WINPATH(DEFAULT_WPID),
              WP_DISPLAY_SYNC_ENET,
              WP_DISPLAY_DEFAULT,
              NULL);

#endif /* WTI_DEBUG_LEVEL */
#endif  /* WP_HW_WINPATH2 */
}

/*****************************************************************************
 * Function name: WTI_SyncEthConfig
 * Description  : configure SyncE data structures
 * Input  params: cfg_ptr - pointer to configuration structure
 *
 * Output params:
 * Return val   : none
 *****************************************************************************/
void WTI_SyncEthConfig(WP_sync_eth_config *cfg_ptr)
{
   memset(cfg_ptr, 0, sizeof(WP_sync_eth_config));

#ifdef WP_HW_WINPATH2
   /* SyncE is not supported */
#else
   cfg_ptr->clock_master = WT_SYNC_ENET_CLOCK_MASTER;

   if(WTI_ENET_PORT != WP_PORT_ENET13)
   {
      WTI_TerminateOnError(1, "ERROR - WTI_SyncEthConfig -SyncE - current setting suport SyncE over ENET13",
                           __LINE__);
   }


   cfg_ptr->serdes = WP_SYNC_ENET_SERDED4_LANE0;       /* TBD - need to map the winnet to serdes lane */
   cfg_ptr->divider = 15625;   /* 125MHz/15625 = 8KHz */
   cfg_ptr->enable = WP_ENABLE;
#endif  /* WP_HW_WINPATH2 */
}

#if !WTI_CESOP_TDI
#if defined __WT_UFE412__ || defined __WT_UFE448__
void WTI_SetDefaultE1UnfTestParameters(WP_U32 num_of_lines, WP_U32 interface,
                                       WUFE_line_transf_type *transfer_type,
                                       WUFE_un_framed *framing_mode, WUFE_cas_enable *cas_mode,
                                       WP_U32 *num_slots
#ifdef WT_UFE_FRAMER
                                       ,WP_U8 *flexmux_mode
#endif
   )
{
#if defined __WT_UFE448__ || defined __WT_UFE412__
	WP_U32 i;
   global_active_cr_async_mode = num_of_lines;
   *framing_mode = WUFE_UNFRAMED;
   *cas_mode = WUFE_CAS_DISABLE;
   *num_slots = WTI_E1_SLOTS;
   test_setup.pw_per_line = 1;
   test_setup.num_of_lines = num_of_lines;
   test_setup.recovered_pw = 0;

   for(i = 0; i<MAX_UFE_LINES_USED; i++)
   {
	   test_setup.line_cr_operation_mode[i] = global_cr_operation_mode;
   }

   switch(interface)
   {
      case 0:
         *transfer_type = WUFE_SDH_TYPE_E1;
#ifdef WT_UFE_FRAMER
         *flexmux_mode = WPX_UFE_FRAMER_DEVICE_MODE_SDH;
#endif
         break;
      case 1:
         *transfer_type = WUFE_SONET_TYPE_E1;
#ifdef WT_UFE_FRAMER
         *flexmux_mode = WPX_UFE_FRAMER_DEVICE_MODE_SONET;
#endif
         break;
      case 2:
         *transfer_type = WUFE_CAD_TYPE_E1;
         break;
      default:
         WTI_TerminatePrintError("Invalid interface value, Allowed values are: 0-SDH, 1-SONET 2-CAD ", __LINE__);
         return;
   }
#endif
}

void WTI_SetDefaultT1UnfTestParameters(WP_U32 num_of_lines, WP_U32 interface,
                                       WUFE_line_transf_type *transfer_type,
                                       WUFE_un_framed *framing_mode, WUFE_cas_enable *cas_mode,
                                       WP_U32 *num_slots
#ifdef WT_UFE_FRAMER
                                       ,WP_U8 *flexmux_mode
#endif
   )
{
#if defined __WT_UFE448__ || defined __WT_UFE412__
	WP_U32 i;
   global_active_cr_async_mode = num_of_lines;
   *framing_mode = WUFE_UNFRAMED;
   *cas_mode = WUFE_CAS_DISABLE;
   *num_slots = WTI_T1_SLOTS;
   test_setup.pw_per_line = 1;
   test_setup.num_of_lines = num_of_lines;
   test_setup.recovered_pw = 0;

   for(i = 0; i<MAX_UFE_LINES_USED; i++)
   {
	   test_setup.line_cr_operation_mode[i] = global_cr_operation_mode;
   }

   switch(interface)
   {
      case 0:
         *transfer_type = WUFE_SDH_TYPE_T1;
#ifdef WT_UFE_FRAMER
         *flexmux_mode = WPX_UFE_FRAMER_DEVICE_MODE_SDH;
#endif
         break;
      case 1:
         *transfer_type = WUFE_SONET_TYPE_T1;
#ifdef WT_UFE_FRAMER
         *flexmux_mode = WPX_UFE_FRAMER_DEVICE_MODE_SONET;
#endif
         break;
      case 2:
         *transfer_type = WUFE_CAD_TYPE_T1;
         break;
      default:
         WTI_TerminatePrintError("Invalid interface value, Allowed values are: 0-SDH, 1-SONET 2-CAD ", __LINE__);
         return;
   }
#endif

}



void WTI_SetDefaultE1FrmTestParameters(WP_U32 num_of_lines, WP_U32 interface,
                                       WUFE_line_transf_type *transfer_type,
                                       WUFE_un_framed *framing_mode, WUFE_cas_enable *cas_mode,
                                       WP_U32 *num_slots
#ifdef WT_UFE_FRAMER
                                       ,WP_U8 *flexmux_mode
#endif
   )
{
#if defined __WT_UFE448__ || defined __WT_UFE412__
	WP_U32 i;
   global_active_cr_async_mode = num_of_lines;
   *framing_mode = WUFE_FRAMED;
   *cas_mode = WUFE_CAS_DISABLE;
   *num_slots = WTI_E1_SLOTS;
   test_setup.pw_per_line = 1;
   test_setup.num_of_lines = num_of_lines;
   test_setup.first_slot_id = 0;
   test_setup.recovered_pw = 0;

   for(i = 0; i<MAX_UFE_LINES_USED; i++)
   {
	   test_setup.line_cr_operation_mode[i] = global_cr_operation_mode;
   }

   switch(interface)
   {
      case 0:
         *transfer_type = WUFE_SDH_TYPE_E1;
#ifdef WT_UFE_FRAMER
         *flexmux_mode = WPX_UFE_FRAMER_DEVICE_MODE_SDH;
#endif
         break;
      case 1:
         *transfer_type = WUFE_SONET_TYPE_E1;
#ifdef WT_UFE_FRAMER
         *flexmux_mode = WPX_UFE_FRAMER_DEVICE_MODE_SONET;
#endif
         break;
      case 2:
         *transfer_type = WUFE_CAD_TYPE_E1;
         break;
      default:
         WTI_TerminatePrintError("Invalid interface value, Allowed values are: 0-SDH, 1-SONET 2-CAD ", __LINE__);
         return;
   }
#endif
}


void WTI_SetDefaultT1FrmTestParameters(WP_U32 num_of_lines, WP_U32 interface,
                                       WUFE_line_transf_type *transfer_type,
                                       WUFE_un_framed *framing_mode, WUFE_cas_enable *cas_mode,
                                       WP_U32 *num_slots
#ifdef WT_UFE_FRAMER
                                       ,WP_U8 *flexmux_mode
#endif
   )
{
#if defined __WT_UFE448__ || defined __WT_UFE412__
	WP_U32 i;
   global_active_cr_async_mode = num_of_lines;
   *framing_mode = WUFE_FRAMED;
   *cas_mode = WUFE_CAS_DISABLE;
   *num_slots = WTI_T1_SLOTS;
   test_setup.pw_per_line = 1;
   test_setup.num_of_lines = num_of_lines;
   test_setup.first_slot_id = 0;
   test_setup.recovered_pw = 0;

   for(i = 0; i<MAX_UFE_LINES_USED; i++)
   {
	   test_setup.line_cr_operation_mode[i] = global_cr_operation_mode;
   }

   switch(interface)
   {
      case 0:
         *transfer_type = WUFE_SDH_TYPE_T1;
#ifdef WT_UFE_FRAMER
         *flexmux_mode = WPX_UFE_FRAMER_DEVICE_MODE_SDH;
#endif
         break;
      case 1:
         *transfer_type = WUFE_SONET_TYPE_T1;
#ifdef WT_UFE_FRAMER
         *flexmux_mode = WPX_UFE_FRAMER_DEVICE_MODE_SONET;
#endif
         break;
      case 2:
         *transfer_type = WUFE_CAD_TYPE_T1;
         break;
      default:
         WTI_TerminatePrintError("Invalid interface value, Allowed values are: 0-SDH, 1-SONET 2-CAD ", __LINE__);
         return;
   }
#endif
}
#endif /* WT_UFE4 */
#endif /* WTI_CESOP_TDI */

#if !defined(CHECKIN_TEST)
#ifdef WP_HW_WINPATH3
#if defined __WT_UFE448__ || defined __WT_UFE412__
static void WTI_ZarlinkPllSetup(WPX_pll_cfg *pllConfigParam)
{
	pllConfigParam->wpid = the_system->wpid;

	switch(global_clock_rec_diff_ref_clock)
	{
	case 77760000:
		pllConfigParam->apll_rate = wpx_f_77_76;
		pllConfigParam->clock_out = wpx_apll_clk0;
		break;
	case 38880000:
		pllConfigParam->apll_rate = wpx_f_38_88;
		pllConfigParam->clock_out = wpx_apll_clk0;
		break;
	case 19440000:
		pllConfigParam->apll_rate = wpx_f_19_44;
		pllConfigParam->clock_out = wpx_apll_clk0;
		break;
	case 9720000:
		pllConfigParam->apll_rate = wpx_f_9_72;
		pllConfigParam->clock_out = wpx_apll_clk0;
		break;
	case 51840000:
		pllConfigParam->apll_rate = wpx_f_51_84;
		pllConfigParam->clock_out = wpx_apll_clk0;
		break;
	case 25920000:
		pllConfigParam->apll_rate = wpx_f_25_92;
		pllConfigParam->clock_out = wpx_apll_clk0;
		break;
	case 12960000:
		pllConfigParam->apll_rate = wpx_f_12_96;
		pllConfigParam->clock_out = wpx_apll_clk0;
		break;
	case 6480000:
		pllConfigParam->apll_rate = wpx_f_6_48;
		pllConfigParam->clock_out = wpx_apll_clk0;
		break;
	case 125000000:
		pllConfigParam->apll_rate = wpx_f_125;
		pllConfigParam->clock_out = wpx_apll_clk0;
		break;
	case 6250000:
		pllConfigParam->apll_rate = wpx_f_62_5;
		pllConfigParam->clock_out = wpx_apll_clk0;
		break;
	case 50000000:
		pllConfigParam->apll_rate = wpx_f_50;
		pllConfigParam->clock_out = wpx_apll_clk0;
		break;
	case 25000000:
		pllConfigParam->apll_rate = wpx_f_25;
		pllConfigParam->clock_out = wpx_apll_clk0;
		break;
	case 12500000:
		pllConfigParam->apll_rate = wpx_f_12_5;
		pllConfigParam->clock_out = wpx_apll_clk0;
		break;
	default:
		pllConfigParam->clock_rate = global_clock_rec_diff_ref_clock/1000;
		pllConfigParam->clock_out = wpx_p0_clk0;
	}
	printf("********************************************\n");
	printf("SMA1 reference clock set to %d\n", global_clock_rec_diff_ref_clock);
	printf("********************************************\n\n");
}
#endif //defined __WT_UFE448__ || defined __WT_UFE412__
#endif
#endif //!defined(CHECKIN_TEST)

WP_U32 global_jitter_buffer_size = 4;

#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
void WTI_ClockRecInitialParamsSet(WP_clock_rec_params *clock_rec_params)
{
   int i;

   for (i=0; i<WTI_CESOP_TOTAL_NUM_CLOCK_DOMAINS; i++)
   {
      clock_rec_params[i].intmode = WP_ENABLE;
      clock_rec_params[i].intqueue = WP_IRQT1;
      clock_rec_params[i].statmode = WP_ENABLE;
      /* specifies the limit in packets for holdover period expired interrupt */
#if WTI_CI_TESTING_MODE_TDI || WTI_CI_TESTING_MODE_UFE4
      clock_rec_params[i].holdover_counter_limit = 0x0002000; 
#else
      clock_rec_params[i].holdover_counter_limit = 0x0010000;
#endif
      clock_rec_params[i].ts_clock_divider = WP_CLOCK_REC_TS_CLOCK_DIV_INTERNAL;
      clock_rec_params[i].debug_en = WP_ENABLE; /* The default is CR debug clear. */
      /* In UFE4, initialize Physical clock parameters to zero.
         Change the default values in the test itself. Not in used in UFE3 */
      clock_rec_params[i].physical_clock_enable = 0;
      clock_rec_params[i].physical_clock_id = 0;
   }
}

#if defined __WT_UFE412__|| defined __WT_UFE448__
void WTI_ClockRecoveryPsn2TdmDefaultParamesConfigUfe4(WP_U32 line_index, WP_U32 cr_operation_mode)
{

/*
  The default CR parameters are calculated with the following system parameters:

  ACR:
  Update period of 4 seconds.
  Settling time of 500 sec.
  Dumping ration of ~0.707.

  DCR:
  Update period of 4 seconds of 32 bytes

  WP2-TDI:
  Winpath internal clock is 300MHz-> Adaptive reference clock is 150MHz.
  DCO reference is 150MHz (internal clock/2)
  (Differential reference clock is 100MHz)

  WP3-TDI:
  Winpath internal clock is 450MHz-> Adaptive reference clock is 450MHz.
  DCO reference is 450Hz
  (Differential reference clock is 450MHz)

  WP3-UFE3:
  Adaptive reference clock is 100MHz(from 20MHz TCXO on WDS3).
  DCO reference is 100MHz (from 20MHzTCXO on WDS3)
  (Differential reference clock is 100MHz)

  WP2-UFE3:
  Winpath internal clock is 300MHz-> Adaptive reference clock is 150MHz.
  DCO reference is 100MHz
  (Differential reference clock is 100MHz)

  ACR:
  Update period of 4 seconds.
  Settling time of 500 sec.
  Dumping ration of ~0.707.

  DF = 28
  IF =31

  DCR:
  Update period of 32 packets (~32 update/sec).

  DF =1
  IF = 9
*/
   WP_status status = WP_OK;

   if (cr_operation_mode != WP_IW_CLOCK_REC_ADAPTIVE_MODE        && /* WP_IW_CLOCK_REC_ADAPTIVE_TYPE_A_MODE */
       cr_operation_mode != WP_IW_CLOCK_REC_ADAPTIVE_TYPE_B_MODE &&
       cr_operation_mode != WP_IW_CLOCK_REC_DIFFERENTIAL_MODE    &&
       cr_operation_mode != WP_IW_CLOCK_REC_DOCSIS)
   {
         status = !WP_OK;
         WTI_TerminateOnError(status,"Invalid Clock Recovery Operation mode ", __LINE__);
         return;
   }

   if (cr_operation_mode == WP_IW_CLOCK_REC_DIFFERENTIAL_MODE)
   {
      /* Differential Mode */
      /*--- Clock Recovery Paramters ---*/
      /* Set clock recovery mode */
      the_system->clock_rec[line_index].operation_mode = WP_IW_CLOCK_REC_DIFFERENTIAL_MODE;
      /* get the clock_rec_if_id from UFE driver */
      the_system->clock_rec[line_index].clock_rec_if_id = WT_ClockRecInterfaceId(line_index);
      /* Far-end Differential Reference Clock (DRC) frequency in Hz */
      the_system->clock_rec[line_index].remote_diff_ref_clock = global_clock_rec_remote_diff_ref_clock;
      /* First filter factor */
      the_system->clock_rec[line_index].direct_factor = 6;
      /* Second filter factor */
      the_system->clock_rec[line_index].integration_factor =
         the_system->clock_rec[line_index].direct_factor + 8;
      /* The number of PSN packets between divisor updates */
      the_system->clock_rec[line_index].divisor_update_period = 32;
      /* Not in use in differential mode */
      the_system->clock_rec[line_index].phase_error_threshold = 0;
      /* Not in use in differential mode */
      the_system->clock_rec[line_index ].freq_error_rec_period = 0;
      /* This factor control the stability of the clock in holdover */
      /* and use us forggeting factor in the EXMA of the divisor    */
      the_system->clock_rec[line_index ].average_divisor_factor = 10;
      /* Not in use in differential mode */
      the_system->clock_rec[line_index ].allowed_dummy_packets = 0;



  /* Enahnced  mode */
      the_system->clock_rec[line_index].enhanced_acr_mode =  WP_JITTER_BUFFER_LEVELING_EN;
      the_system->clock_rec[line_index].jb_leveling_correction_size = 60;
      the_system->clock_rec[line_index].jb_leveling_threshold_high = (global_jitter_buffer_size/2) + 4;
      the_system->clock_rec[line_index].jb_leveling_threshold_low = (global_jitter_buffer_size/2) - 4;

   }
   else
   {
      /* Adaptive  Mode */
      /*--- Clock Recovery Paramters ---*/
      /* Set adaptive clock recovery mode */
      the_system->clock_rec[line_index].operation_mode = WP_IW_CLOCK_REC_ADAPTIVE_MODE;
      /* get the clock_rec_if_id from UFE driver */
      the_system->clock_rec[line_index].clock_rec_if_id = WT_ClockRecInterfaceId(line_index);
      /* Far-end Differential Reference Clock (DRC) frequency in Hz - not in use in ADAP*/
      the_system->clock_rec[line_index].remote_diff_ref_clock = 0;
      /* First filter factor */
      the_system->clock_rec[line_index].direct_factor = 28; //  27/30   28/33  29/34
      /* Second  filter factor */
      the_system->clock_rec[line_index].integration_factor = 33;
      /* The time in seconds between divisor updates */
      the_system->clock_rec[line_index].divisor_update_period = 4;
      /* The number of host clocks which consider phase error */
      the_system->clock_rec[line_index].phase_error_threshold = 6000;  
      /* The time in sec the system will lock to new freq after trying to fix phase error*/
      /* This time must be N * divisor_update_period*/
      the_system->clock_rec[line_index].freq_error_rec_period = 128; 
      /* This factor control the stability of the clock in holdover */
      /* and use us forggeting factor in the EXMA of the divisor*/
      the_system->clock_rec[line_index].average_divisor_factor = 4;
      /* Allowed dummy packets for 1024 valid packets */
      the_system->clock_rec[line_index].allowed_dummy_packets = 16;
      /* PMSM parameter -  set the number of divisor_update_period in which
         the phase difference was under pm_error_threshold before the PMSM move from hold over to normal state */
      the_system->clock_rec[line_index].pm_threshold = 4;
      /* PMSM parameter - set the number of divisor_update_period in which
         the phase is monotonic  before the PMSM move from hold over to convergence state */
      the_system->clock_rec[line_index].max_pm_threshold = 4;
      /* PMSM parameter - set the value (in adaptive reference clock period) of the allowed difference between
         divisor_update_period windows */
      the_system->clock_rec[line_index].pm_error_threshold = 3000; 

      /* Enahnced ACR mode */
      the_system->clock_rec[line_index].enhanced_acr_mode = WP_TS_DEV_BASED_SM_EN | WP_JITTER_BUFFER_LEVELING_EN;
      the_system->clock_rec[line_index].ts_dev_jump_threshold = 500;
      the_system->clock_rec[line_index].ts_dev_average_factor = 4;

      the_system->clock_rec[line_index].jb_leveling_correction_size = 30;
      the_system->clock_rec[line_index].jb_leveling_threshold_high = (global_jitter_buffer_size/2) + 1;
      the_system->clock_rec[line_index].jb_leveling_threshold_low = (global_jitter_buffer_size/2) - 1;
   }
}

#else /* for UFE3 or TDI */
void WTI_ClockRecoveryPSN2TDMDefultParamesConfig(WP_U32 line_index)
{

/*
  The default CR parameters are calculated with the following system parameters:

  ACR:
  Update period of 4 seconds.
  Settling time of 500 sec.
  Dumping ration of ~0.707.

  DCR:
  Update period of 4 seconds of 32 bytes

  WP2-TDI:
  Winpath internal clock is 300MHz-> Adaptive reference clock is 150MHz.
  DCO reference is 150MHz (internal clock/2)
  (Differential reference clock is 100MHz)

  WP3-TDI:
  Winpath internal clock is 450MHz-> Adaptive reference clock is 450MHz.
  DCO reference is 450Hz
  (Differential reference clock is 450MHz)

  WP3-UFE3:
  Adaptive reference clock is 100MHz(from 20MHz TCXO on WDS3).
  DCO reference is 100MHz (from 20MHzTCXO on WDS3)
  (Differential reference clock is 100MHz)

  WP2-UFE3:
  Winpath internal clock is 300MHz-> Adaptive reference clock is 150MHz.
  DCO reference is 100MHz
  (Differential reference clock is 100MHz)

  ACR:
  Update period of 4 seconds.
  Settling time of 500 sec.
  Dumping ration of ~0.707.

  DF = 28
  IF =31

  DCR:
  Update period of 32 packets (~32 update/sec).

  DF =1
  IF = 9
*/

#if (WTI_CLOCK_REC_MODE || WTI_EXT_DIFF_MODE_ACTIVE)
   /* Differential Mode */
   /*--- Clock Recovery Paramters ---*/
   /* Set clock recovery mode */
   the_system->clock_rec[line_index].operation_mode = WP_IW_CLOCK_REC_DIFFERENTIAL_MODE;
   /* Clock recovery interface index - by defult it's the line index */
   the_system->clock_rec[line_index].clock_rec_if_id = line_index;
   /* Far-end Differential Reference Clock (DRC) frequency in Hz */
   the_system->clock_rec[line_index].remote_diff_ref_clock = global_clock_rec_remote_diff_ref_clock;
   /* First filter factor */
   the_system->clock_rec[line_index].direct_factor = 1;
   /* Second filter factor */
   the_system->clock_rec[line_index].integration_factor =
      the_system->clock_rec[line_index].direct_factor + 8;
   /* The number of PSN packets between divisor updates */
   the_system->clock_rec[line_index].divisor_update_period = 32;
   /* Not in use in differential mode */
   the_system->clock_rec[line_index].phase_error_threshold = 0;
   /* Not in use in differential mode */
   the_system->clock_rec[line_index ].freq_error_rec_period = 0;
   /* This factor control the stability of the clock in holdover */
   /* and use us forggeting factor in the EXMA of the divisor    */
   the_system->clock_rec[line_index ].average_divisor_factor = 10;
   /* Not in use in differential mode */
   the_system->clock_rec[line_index ].allowed_dummy_packets = 0;
#else
   /* Adaptive  Mode */
   /*--- Clock Recovery Paramters ---*/
   /* Set adaptive clock recovery mode */
   the_system->clock_rec[line_index].operation_mode = WP_IW_CLOCK_REC_ADAPTIVE_MODE;
   /* Clock recovery interface index - by defult it's the line index */
   the_system->clock_rec[line_index].clock_rec_if_id = line_index;
   /* Far-end Differential Reference Clock (DRC) frequency in Hz - not in use in ADAP*/
   the_system->clock_rec[line_index].remote_diff_ref_clock = 0;
   /* First filter factor */
   the_system->clock_rec[line_index].direct_factor = 27;
   /* Second  filter factor */
   the_system->clock_rec[line_index].integration_factor = 30;
   /* The time in seconds between divisor updates */
   the_system->clock_rec[line_index].divisor_update_period = 2;
   /* The number of host clocks which consider phase error */
   the_system->clock_rec[line_index].phase_error_threshold = 6000;
   /* The time in sec the system will lock to new freq after trying to fix phase error*/
   /* This time must be N * divisor_update_period*/
   the_system->clock_rec[line_index].freq_error_rec_period = 128;
   /* This factor control the stability of the clock in holdover */
   /* and use us forggeting factor in the EXMA of the divisor*/
   the_system->clock_rec[line_index].average_divisor_factor = 4;
   /* Allowed dummy packets for 1024 valid packets */
   the_system->clock_rec[line_index].allowed_dummy_packets = 16;
   /* PMSM parameter -  set the number of divisor_update_period in which
      the phase difference was under pm_error_threshold before the PMSM move from hold over to normal state */
   the_system->clock_rec[line_index].pm_threshold = 2;
   /* PMSM parameter - set the number of divisor_update_period in which
      the phase is monotonic  before the PMSM move from hold over to convergence state */
   the_system->clock_rec[line_index].max_pm_threshold = 4;
   /* PMSM parameter - set the value (in adaptive reference clock period) of the allowed difference between
      divisor_update_period windows */
   the_system->clock_rec[line_index].pm_error_threshold = 600;

   the_system->clock_rec[line_index].enhanced_acr_mode = WP_JITTER_BUFFER_LEVELING_EN;
   the_system->clock_rec[line_index].jb_leveling_correction_size = 30;
   the_system->clock_rec[line_index].jb_leveling_threshold_high = 17;
   the_system->clock_rec[line_index].jb_leveling_threshold_low = 15;
#endif /* WTI_CLOCK_REC_MODE || WTI_EXT_DIFF_MODE_ACTIVE */

}
#endif /* __WT_UFE412__ || defined __WT_UFE448__ */
#endif

#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
/*****************************************************************************
 * Function name: WTI_ClockRecDcoGlobalDirectInit
 * Description  : configure DCO global registers for WP2 or later HW
 * Input  params:
 * Output params:
 * Return val   : none
 *****************************************************************************/
void WTI_ClockRecDcoGlobalDirectInit(WP_U8 sync_delay,WP_U8 clock_selection)
{
#ifdef WP_HW_WINPATH2
   WP_tdi_dco_global_params dco_global_params;
   WP_status status;

   memset(&dco_global_params,0, sizeof(WP_tdi_dco_global_params));

   switch (sync_delay)
   {
      case 0:
         dco_global_params.sync_delay = WP_CLOCK_REC_DCO_SYNC_DELAY_NA;
         break;
      case 1:
         dco_global_params.sync_delay = WP_CLOCK_REC_DCO_SYNC_DELAY_1;
         break;
      case 2:
         dco_global_params.sync_delay = WP_CLOCK_REC_DCO_SYNC_DELAY_2;
         break;
      case 3:
         dco_global_params.sync_delay = WP_CLOCK_REC_DCO_SYNC_DELAY_3;
         break;
      default:
         status = !WP_OK;
         WTI_TerminateOnError(status,"Invalid dco mode ", __LINE__);
         return;
   }

   switch (clock_selection)
   {
      case 3:
         dco_global_params.clock_selection = WP_CLOCK_REC_DCO_CLOCK_BRGI3;
         break;
      case 5:
         dco_global_params.clock_selection = WP_CLOCK_REC_DCO_CLOCK_INTERNAL;
         break;
      default:
         status = !WP_OK;
         WTI_TerminateOnError(status,"Invalid dco mode ", __LINE__);
         return;
   }

   /* Direct init of global DCO reg */
    status = WP_ClockRecDcoGlobalDirectInit(the_system->wpid,&dco_global_params);
   WTI_TerminateOnError(status,
                        "WP_ClockRecDcoGlobalDirectInit() Direct init of Global TDI DCO reg", __LINE__);
#endif
}
/*****************************************************************************
 * Function name: WTI_ClockRecDcoDirectInit
 * Description  : configure per TDI DCO for WP2 or later HW
 * Input  params: dco_id and all DCO paramters
 * Output params:
 * Return val   : none
 *****************************************************************************/
void WTI_ClockRecDcoDirectInit(WP_U32 dco_id,WP_U8 e1_t1,WP_U8 framing_mode,
                               WP_U8 sync_mode,WP_U8 sync_active_mode,
                               WP_U8 sync_delay,WP_U8 clock_selection)
{

#ifdef WP_HW_WINPATH3
   WP_tdi_dco_global_params dco_global_params;
#endif
   WP_tdi_dco_params dco_params;
   WP_status status;

   memset(&dco_params,0, sizeof(WP_tdi_dco_params));
#ifdef WP_HW_WINPATH3
   memset(&dco_global_params,0, sizeof(WP_tdi_dco_global_params));
#endif

   dco_params.dco_id = dco_id;

   switch (e1_t1)
   {
      case 0:
         dco_params.mode = WP_CLOCK_REC_DCO_T1_MODE;

         /* For 300MHz clk */
         dco_params.nominal_divisor_int = clock_rec_default_t1_freq_for_300MHz.nominal_divisor_int;
         dco_params.nominal_divisor_rem = clock_rec_default_t1_freq_for_300MHz.nominal_divisor_rem;         ;
         break;
      case 1:
         dco_params.mode = WP_CLOCK_REC_DCO_E1_MODE;

         /* For 300MHz clk */
         dco_params.nominal_divisor_int = clock_rec_default_e1_freq_for_300MHz.nominal_divisor_int;
         dco_params.nominal_divisor_rem = clock_rec_default_e1_freq_for_300MHz.nominal_divisor_rem;

         break;
      default:
         status = !WP_OK;
         WTI_TerminateOnError(status,"Invalid dco mode ", __LINE__);
         return;
   }

   switch (framing_mode)
   {
      case 0:
         dco_params.framed = WP_CLOCK_REC_DCO_UNFRAMED;
         break;
      case 1:
         dco_params.framed = WP_CLOCK_REC_DCO_FRAMED;
         break;
      default:
         status = !WP_OK;
         WTI_TerminateOnError(status,"Invalid framing_mode ", __LINE__);
         return;
   }

   switch (sync_mode)
   {
      case 0:
         dco_params.sync_mode = WP_CLOCK_REC_DCO_SYNC_RISING_EGDE;
         break;
      case 1:
         dco_params.sync_mode = WP_CLOCK_REC_DCO_SYNC_FALLING_EGDE;
         break;
      default:
         status = !WP_OK;
         WTI_TerminateOnError(status,"Invalid sync mode_mode ", __LINE__);
         return;
   }

   switch (sync_active_mode)
   {
      case 0:
         dco_params.sync_active_mode = WP_CLOCK_REC_DCO_SYNC_ACTIVE_HIGH;
         break;
      case 1:
         dco_params.sync_active_mode = WP_CLOCK_REC_DCO_SYNC_ACTIVE_LOW;
         break;
      default:
         status = !WP_OK;
         WTI_TerminateOnError(status,"Invalid sync active mode ", __LINE__);
         return;
   }

#ifdef WP_HW_WINPATH3
   switch (sync_delay)
   {
      case 0:
         dco_global_params.sync_delay = WP_CLOCK_REC_DCO_SYNC_DELAY_NA;
         break;
      case 1:
         dco_global_params.sync_delay = WP_CLOCK_REC_DCO_SYNC_DELAY_1;
         break;
      case 2:
         dco_global_params.sync_delay = WP_CLOCK_REC_DCO_SYNC_DELAY_2;
         break;
      case 3:
         dco_global_params.sync_delay = WP_CLOCK_REC_DCO_SYNC_DELAY_3;
         break;
      default:
         status = !WP_OK;
         WTI_TerminateOnError(status,"Invalid dco mode ", __LINE__);
         return;
   }

   switch (clock_selection)
   {
      case 0:
         dco_global_params.clock_selection = WP_CLOCK_REC_DCO_CLOCK_BRGI1;
         break;
      case 1:
         dco_global_params.clock_selection = WP_CLOCK_REC_DCO_CLOCK_BRGI2;
         break;
      case 2:
         dco_global_params.clock_selection = WP_CLOCK_REC_DCO_CLOCK_BRGI3;
         break;
      case 3:
         dco_global_params.clock_selection = WP_CLOCK_REC_DCO_CLOCK_BRGI4;
         break;
      case 4:
         dco_global_params.clock_selection = WP_CLOCK_REC_DCO_CLOCK_ENET;
         break;
      case 5:
         dco_global_params.clock_selection = WP_CLOCK_REC_DCO_CLOCK_INTERNAL;
         break;
      default:
         status = !WP_OK;
         WTI_TerminateOnError(status,"Invalid dco mode ", __LINE__);
         return;
   }

   /* Direct init of tdi DCO */
   status = WP_ClockRecDcoDirectInit(the_system->wpid, &dco_params, &dco_global_params);
#else /* WP2 */
   /* Direct init of tdi DCO */
   status = WP_ClockRecDcoDirectInit(the_system->wpid, &dco_params, NULL);
#endif
   WTI_TerminateOnError(status, "WP_ClockRecDcoDirectInit() Direct init of TDI DCO", __LINE__);
}
/*****************************************************************************
 * Function name: WTI_ClockRecDcoHwRegistersGet
 * Description  : configure per TDI DCO for WP2
 * Input  params: dco_id , expected values;
 * Output params:
 * Return val   : none
 *****************************************************************************/
WP_status WTI_ClockRecDcoHwRegistersGet(WP_U32 dco_id,WP_U32 clk_rec_cfg,
                                        WP_U32 clk_recn_reg0,WP_U32 clk_recn_reg1)
{
   WP_clock_rec_dco_hw_reg wp_clock_rec_dco_hw_reg;
   WP_status status;

   memset(&wp_clock_rec_dco_hw_reg,0, sizeof(wp_clock_rec_dco_hw_reg));

   /*Read and Display the DCO HW registers   */
   status = WP_ClockRecDcoHwRegistersGet(the_system->wpid,dco_id,&wp_clock_rec_dco_hw_reg);
   WTI_TerminateOnError(status, "WP_ClockRecDcoHwRegistersGet", __LINE__);

   printf(" DCO HW registers values of DCO: %d \n",dco_id);
   printf(" clk_rec_cfg %x \n", wp_clock_rec_dco_hw_reg.clk_rec_cfg);
   printf(" clk_rec%d_reg0 %x \n",dco_id ,wp_clock_rec_dco_hw_reg.clk_recn_reg0);
   printf(" clk_rec%d_reg1 %x \n\n",dco_id ,wp_clock_rec_dco_hw_reg.clk_recn_reg1);

   if((wp_clock_rec_dco_hw_reg.clk_rec_cfg == clk_rec_cfg)&&
      (wp_clock_rec_dco_hw_reg.clk_recn_reg0 == clk_recn_reg0)&&
      (wp_clock_rec_dco_hw_reg.clk_recn_reg1 == clk_recn_reg1))
   {
      status = WP_OK;
   }
   else
   {
      status = !WP_OK;
   }
   return status;

}

WP_status WTI_ClockRecTdiDcoRegistersCheck(WP_U32 dco_id, WP_U32 clk_rec_reg0, WP_U32 clk_rec_reg1,
                                           WP_U32 clk_rec_reg2, WP_U32 clk_rec_fifo_reg)
{
   WP_status status = WP_OK;
   WP_U32 reg0, reg1, reg2, fifo_reg;

   status = WTI_ClockRecTdiDcoRegRead(dco_id, &reg0, &reg1, &reg2, &fifo_reg);
   WTI_TerminateOnError(status, "WTI_ClockRecTdiDcoRegRead", __LINE__);

   printf(" DCO HW registers values of DCO: %d \n", dco_id);
   printf(" clock rec cfg reg0  0x%x\n", reg0);
   printf(" clock rec cfg reg1  0x%x\n", reg1);
   printf(" clock rec cfg reg2  0x%x\n", reg2);
   printf(" clock rec fifo reg  0x%x\n", fifo_reg);

   if ((clk_rec_reg0 == reg0) &&
       (clk_rec_reg1 == reg1) &&
       (clk_rec_reg2 == reg2) &&
       (clk_rec_fifo_reg == fifo_reg))
      status = WP_OK;
   else
      status = !WP_OK;

   return status;
}

struct WPT_clock_rec_dco
{
   WP_U32 reg0; /* cfg (16) and div_int (16) */
   WP_U32 reg1; /* div_frac (32) */
   WP_U32 reg2; /* res (16) and clk_divider (16) */
   WP_U32 rx_ts;
   WP_U32 tx_ts;
   WP_U32 fifo_stat;
};

typedef struct WPT_clock_rec_dco WPT_clock_rec_dco;

#define WTI_CLOCK_REC_DCO_BASE 0x25400 // WP3 base
#define WTI_CLOCK_REC_DCO_DELTA 0x18
#define WTI_CLOCK_REC_DCO_REGISTER_OFFSET(t) \
   (WTI_CLOCK_REC_DCO_BASE + WTI_CLOCK_REC_DCO_DELTA * (t))
#define WTI_CLOCK_REC_DCO(reg_base, t) \
   ((WPT_clock_rec_dco *) ((reg_base) + WTI_CLOCK_REC_DCO_REGISTER_OFFSET(t)))

#define WTI_CLOCK_REC_EVENT_MASK 0xF
#define WTI_CLOCK_REC_DIVIDER_MASK 0xFFFF


WP_status WTI_ClockRecTdiDcoHwRegRead(WP_U32 wpid, WP_U32 dco_id, WP_U8 port_interface,
                                     WP_clock_rec_tdi_dco_hw_reg *clock_rec_tdi_dco_hw_reg)
{
   WPT_clock_rec_dco *clock_rec_dco;
   WP_U8 *reg_base;

   /* Verify TDI port ID is in range (dco_id = tdi_interface) */
   if (dco_id > WP_PORT_TDM16 - WP_PORT_TDM1)
   {
      return WP_ERR_CLOCK_REC_DCO_ID;
   }

   reg_base = WPL_RegBaseGet(wpid);

   clock_rec_dco = WTI_CLOCK_REC_DCO(reg_base, dco_id);

   WP_MEM_GET(clock_rec_tdi_dco_hw_reg->reg0, clock_rec_dco->reg0);
   WP_MEM_GET(clock_rec_tdi_dco_hw_reg->reg1, clock_rec_dco->reg1);
   WP_MEM_GET(clock_rec_tdi_dco_hw_reg->reg2, clock_rec_dco->reg2);
   WP_MEM_GET(clock_rec_tdi_dco_hw_reg->fifo_reg, clock_rec_dco->fifo_stat);
   clock_rec_tdi_dco_hw_reg->reg2 =
      clock_rec_tdi_dco_hw_reg->reg2 & WTI_CLOCK_REC_DIVIDER_MASK;
   clock_rec_tdi_dco_hw_reg->fifo_reg =
      clock_rec_tdi_dco_hw_reg->fifo_reg & WTI_CLOCK_REC_EVENT_MASK;

   return WP_OK;
}

WP_status WTI_ClockRecTdiDcoRegRead(WP_U32 dco_id, WP_U32 *reg0, WP_U32 *reg1,
                                    WP_U32 *reg2, WP_U32 *fifo_reg)
{
   WP_status status = WP_OK;
   WP_clock_rec_tdi_dco_hw_reg clock_rec_tdi_dco_hw_reg;

   status = WTI_ClockRecTdiDcoHwRegRead(the_system->wpid, dco_id,
                                       WP_IW_CESOP_PORT_TDM,
                                       &clock_rec_tdi_dco_hw_reg);

   if (status != WP_OK)
   {
      printf("WTI_ClockRecTdiDcoHwRegRead failed: %s\n", WP_error_name[WP_ERROR(status)]);
      return status;
   }

   *reg0 = clock_rec_tdi_dco_hw_reg.reg0;
   *reg1 = clock_rec_tdi_dco_hw_reg.reg1;
   *reg2 = clock_rec_tdi_dco_hw_reg.reg2;
   *fifo_reg = clock_rec_tdi_dco_hw_reg.fifo_reg;

   return status;
}
#endif /* WTI_CESOP_CLOCK_RECOVERY_ENABLE */


/*****************************************************************************
 * Function name: WTI_BoardPinConfig
 * Description  : configure On WDS3 clocks mux using the on-board FPGA
 * Input  params: configuration, board_id;
 * Output params:
 * Return val   : none
 *****************************************************************************/
void WTI_BoardPinConfig(WP_U32 configuration, WP_U32 board_id, WP_U32 pll_clock_out)
{
#ifdef WP_HW_WINPATH3
   WP_status status;
   wpx_wp_cr_pin_io wp_cr_pin_io = {0};

   if (configuration == WTI_DEFAULT_PIN_CONFIG)
   {
      /* Adaptive clock through synt clock,
         differential clock through SMA6, 1 PTP domain, in and out,
         SMA1 extract TCXO as output clock. */
      wp_cr_pin_io.brg_b = WPX_FPGA_NA;//WPX_FPGA_BRG_B_IN_SMA5;
      wp_cr_pin_io.brg_c = WPX_FPGA_BRG_C_IN_SYNT_CLK;
      wp_cr_pin_io.brg_d = WPX_FPGA_BRG_D_IN_SMA6;
      wp_cr_pin_io.clk_rec_fclk1 = WPX_FPGA_CR_FCLK1_OUT_SMA5;
      wp_cr_pin_io.clk_rec_fclk2 = WPX_FPGA_CR_FCLK2_OUT_SMA4;
      wp_cr_pin_io.clk_rec_pps1_output = WPX_FPGA_NA;
      wp_cr_pin_io.clk_rec_pps1_input = WPX_FPGA_CR_PPS1_IN_SMA3;
      wp_cr_pin_io.clk_rec_pps2_output = WPX_FPGA_CR_PPS2_OUT_SMA2;
      wp_cr_pin_io.clk_rec_pps2_input = WPX_FPGA_NA;
      wp_cr_pin_io.sync_eth_clk0_output = WPX_FPGA_NA;
      wp_cr_pin_io.sync_eth_clk1_output = WPX_FPGA_NA;
#if (WTI_CESOP_CLOCK_RECOVERY_ENABLE || WTI_CESOP_RX_TIMING_ENABLE)
      if(wpx_apll_clk0 == pll_clock_out)
      {
    	  wp_cr_pin_io.sma1_out = WPX_FPGA_SMA1_OUT_APLL0_CLK;
      }
      else
      {
    	  wp_cr_pin_io.sma1_out = WPX_FPGA_SMA1_OUT_P0_CLK0;
      }
#else
      wp_cr_pin_io.sma1_out = WPX_FPGA_SMA1_OUT_APLL0_CLK;
#endif
      wp_cr_pin_io.serdes_x_select = WPX_FPGA_SERDES_CLK_APLL0; /* The serdes output is taken from the external APLL output */
      wp_cr_pin_io.serdes1_ref_clk = WPX_FPGA_SERDES_CLK_125;   /* relevant in case serdes_x_select = WPX_FPGA_SERDES_SELECT_CLK */
      wp_cr_pin_io.serdes0_ref_clk = WPX_FPGA_SERDES_CLK_125;   /* relevant in case serdes_x_select = WPX_FPGA_SERDES_SELECT_CLK */

   }
   else if (configuration == WTI_PTP_TWO_BOARDS_CONFIG)
   {
      if (board_id)
      {
         /* BOARD #1: all inputs */
         wp_cr_pin_io.brg_b = WPX_FPGA_BRG_B_IN_SMA5;
         wp_cr_pin_io.brg_c = WPX_FPGA_NA;
         wp_cr_pin_io.brg_d = WPX_FPGA_BRG_D_IN_SMA4;
         wp_cr_pin_io.clk_rec_fclk1 = WPX_FPGA_NA;
         wp_cr_pin_io.clk_rec_fclk2 = WPX_FPGA_NA;
         wp_cr_pin_io.clk_rec_pps1_output = WPX_FPGA_NA;
         wp_cr_pin_io.clk_rec_pps1_input = WPX_FPGA_CR_PPS1_IN_SMA3;
         wp_cr_pin_io.clk_rec_pps2_output = WPX_FPGA_NA;
         wp_cr_pin_io.clk_rec_pps2_input = WPX_FPGA_CR_PPS2_IN_SMA2;
         wp_cr_pin_io.sync_eth_clk0_output = WPX_FPGA_NA;
         wp_cr_pin_io.sync_eth_clk1_output = WPX_FPGA_NA;
         wp_cr_pin_io.sma1_out = WPX_FPGA_SMA1_OUT_TCXO;
         wp_cr_pin_io.serdes_x_select = WPX_FPGA_SERDES_CLK_APLL0 /* The serdes output is taken from the external APLL output */;
         wp_cr_pin_io.serdes1_ref_clk = WPX_FPGA_SERDES_CLK_125;  /* relevant in case serdes_x_select = WPX_FPGA_SERDES_SELECT_CLK */
         wp_cr_pin_io.serdes0_ref_clk = WPX_FPGA_SERDES_CLK_125;  /* relevant in case serdes_x_select = WPX_FPGA_SERDES_SELECT_CLK */
      }
      else
      {
         /* BOARD #2: all outputs */
         wp_cr_pin_io.brg_b = WPX_FPGA_BRG_B_IN_SMA5;
         wp_cr_pin_io.brg_c = WPX_FPGA_NA;
         wp_cr_pin_io.brg_d = WPX_FPGA_NA;
         wp_cr_pin_io.clk_rec_fclk1 = WPX_FPGA_CR_FCLK1_OUT_SMA5;
         wp_cr_pin_io.clk_rec_fclk2 = WPX_FPGA_CR_FCLK2_OUT_SMA4;
         wp_cr_pin_io.clk_rec_pps1_output = WPX_FPGA_CR_PPS1_OUT_SMA3;
         wp_cr_pin_io.clk_rec_pps1_input = WPX_FPGA_NA;
         wp_cr_pin_io.clk_rec_pps2_output = WPX_FPGA_CR_PPS2_OUT_SMA2;
         wp_cr_pin_io.clk_rec_pps2_input = WPX_FPGA_NA;
         wp_cr_pin_io.sync_eth_clk0_output = WPX_FPGA_NA;
         wp_cr_pin_io.sync_eth_clk1_output = WPX_FPGA_NA;
         wp_cr_pin_io.sma1_out = WPX_FPGA_SMA1_OUT_TCXO;
         wp_cr_pin_io.serdes_x_select = WPX_FPGA_SERDES_CLK_APLL0 /* The serdes output is taken from the external APLL output */;
         wp_cr_pin_io.serdes1_ref_clk = WPX_FPGA_SERDES_CLK_125;  /* relevant in case serdes_x_select = WPX_FPGA_SERDES_SELECT_CLK */
         wp_cr_pin_io.serdes0_ref_clk = WPX_FPGA_SERDES_CLK_125;  /* relevant in case serdes_x_select = WPX_FPGA_SERDES_SELECT_CLK */
      }
   }
   else if (configuration == WTI_SYNC_ETH_CONFIG)
   {
      wp_cr_pin_io.brg_b = WPX_FPGA_BRG_B_IN_SMA5;
      wp_cr_pin_io.brg_c = WPX_FPGA_BRG_C_IN_SYNT_CLK;
      wp_cr_pin_io.brg_d = WPX_FPGA_BRG_D_IN_SMA6;
      wp_cr_pin_io.clk_rec_fclk1 = WPX_FPGA_NA;
      wp_cr_pin_io.clk_rec_fclk2 = WPX_FPGA_CR_FCLK2_OUT_SMA4;
      wp_cr_pin_io.clk_rec_pps1_output = WPX_FPGA_NA;
      wp_cr_pin_io.clk_rec_pps1_input = WPX_FPGA_CR_PPS1_IN_SMA3;
      wp_cr_pin_io.clk_rec_pps2_output = WPX_FPGA_NA;
      wp_cr_pin_io.clk_rec_pps2_input = WPX_FPGA_NA;
      wp_cr_pin_io.sma1_out = WPX_FPGA_SMA1_OUT_APLL0_CLK;
      if (board_id)
      {
         wp_cr_pin_io.sync_eth_clk0_output = WPX_FPGA_NA;
         wp_cr_pin_io.sync_eth_clk1_output = WPX_FPGA_SYNC_ETH1_OUT_SMA2;
      }
      else
      {
         wp_cr_pin_io.sync_eth_clk0_output = WPX_FPGA_SYNC_ETH0_OUT_SMA2;
         wp_cr_pin_io.sync_eth_clk1_output = WPX_FPGA_NA;
      }
      wp_cr_pin_io.serdes_x_select = WPX_FPGA_SERDES_CLK_APLL0 /* The serdes output is taken from the external APLL output */;
      wp_cr_pin_io.serdes1_ref_clk = WPX_FPGA_SERDES_CLK_125;  /* relevant in case serdes_x_select = WPX_FPGA_SERDES_SELECT_CLK */
      wp_cr_pin_io.serdes0_ref_clk = WPX_FPGA_SERDES_CLK_125;  /* relevant in case serdes_x_select = WPX_FPGA_SERDES_SELECT_CLK */
   }
   else
   {
      printf("WTI_BoardPinConfig: No confoguration selected\n");
      WTI_Reboot();
   }


   wp_cr_pin_io.sync_eth_clk0_output = WPX_FPGA_NA; /* out through ext. pll. sources muxes */
   wp_cr_pin_io.sync_eth_clk1_output = WPX_FPGA_NA; /* out through ext. pll. sources muxes */
   wp_cr_pin_io.pll_ref_clk1_sel = WPX_FPGA_PLL_REFCLK1_CLK_REC_FCLK_1 ;
   wp_cr_pin_io.pll_ref_clk2_sel = WPX_FPGA_PLL_REFCLK2_CLK_REC_FCLK_2 ;
   /* Set PLL source (entire system clock inc DCO source) to synt clock (board's TCXO/miniOCXO) */
   wp_cr_pin_io.osci_sel = WPX_FPGA_OSCI_SOURCE_SYNT_CLK ;   /*  or WPX_FPGA_OSCI_SOURCE_SMA7*/


   status = WPX_CrBoardPinConfig(the_system->wpid, wp_cr_pin_io);
   WTI_TerminateOnError(status, "WPX_CrBoardPinConfig()", __LINE__);
#endif /* WP_HW_WINPATH3 */
}

/*****************************************************************************
 * Function name: WTI_HostRxChannelsConfig
 * Description  : config Host Rx channel for violation traffic
 * Input  params: tx_binding_config - pointer to WP_tx_binding configuration structure
 *                cfg_ptr - pointer to WP_ch_iw_rx configuration structure
 * Output params:
 * Return val   : none
 *****************************************************************************/
void WTI_HostRxChannelConfig(WP_tx_binding *tx_binding_config, WP_ch_iw_rx *cfg_ptr)
{
   memset(tx_binding_config, 0, sizeof(WP_tx_binding));

   tx_binding_config->res0 = 0;
   tx_binding_config->dci_mode = WP_IW_DYN_CH_INSERT_DISABLE;
   tx_binding_config->maxt = 1536;

   memset(cfg_ptr, 0, sizeof(WP_ch_iw_rx));

   cfg_ptr->pqblock = 0;
   cfg_ptr->pqlevel = 0;
   cfg_ptr->tx_binding_type = WP_IW_TX_BINDING;
   cfg_ptr->tx_binding_config = tx_binding_config;
}

/*****************************************************************************
 * Function name: WTI_IwSystemRoutingConfig
 * Description  : configure IW system in the PSN to TDM direction
 * Input  params: cfg_ptr - pointer to configuration structure
 * Output params:
 * Return val   : none
 *****************************************************************************/
void WTI_IwSystemRoutingConfig(WP_iw_sys_routing *cfg_ptr,
                               WP_iw_ipv4_sys_info *ipv4_sys_info)
{
   memset(ipv4_sys_info, 0, sizeof(WP_iw_ipv4_sys_info));
   memset(cfg_ptr, 0, sizeof(WP_iw_sys_routing));

#if WTI_PCE_CLASSIFIER
   ipv4_sys_info->classification_mode = WP_IW_NO_CLASSIFIER;
   ipv4_sys_info->max_lpm_flows = N_ROUTING_MAX_FLOWS;
   ipv4_sys_info->dfc_info = NULL;
   cfg_ptr->buffer_gap = 64;
   cfg_ptr->max_iw_ports = 2;
   cfg_ptr->max_classification_rules = N_ROUTING_MAX_FLOWS;
   cfg_ptr->supported_protocols = WP_IW_SYS_IPV4;
   cfg_ptr->ipv4_sys_info = ipv4_sys_info;
   cfg_ptr->ipv6_sys_info = NULL;
   cfg_ptr->mpls_sys_info = NULL;
#else
   ipv4_sys_info->classification_mode = WP_IW_POLICY_BASED_ROUTING;
   ipv4_sys_info->max_lpm_flows = N_ROUTING_MAX_FLOWS;
   ipv4_sys_info->dfc_info = &dfc_sys_info;

   cfg_ptr->buffer_gap = 64;
   cfg_ptr->max_iw_ports = 2;
   cfg_ptr->max_classification_rules = N_ROUTING_MAX_FLOWS;
   cfg_ptr->supported_protocols = WP_IW_SYS_IPV4;
   cfg_ptr->ipv4_sys_info = ipv4_sys_info;
   cfg_ptr->ipv6_sys_info = NULL;
   cfg_ptr->mpls_sys_info = NULL;
#endif
}

/*****************************************************************************
 * Function name: WTI_IwSystemBridgingConfig
 * Description  : configure IW system in the PSN to TDM direction
 * Input  params: cfg_ptr - pointer to configuration structure
 * Output params:
 * Return val   : none
 *****************************************************************************/
void WTI_IwSystemBridgingConfig(WP_iw_sys_bridging  *cfg_ptr)
{
   WP_classification classifier_config;

   memset(cfg_ptr, 0, sizeof(WP_iw_sys_bridging ));

   memset (&classifier_config,0,sizeof(WP_classification));

#if WTI_PCE_CLASSIFIER
   classifier_config.classification_parsing_mode = WP_IW_NO_CLASSIFIER;

   classifier_config.max_classification_rules=WTI_MAX_PW;
   classifier_config.number_parse_fields=0;
   classifier_config.parse_keys = NULL;
   cfg_ptr->tag = 0x1234; /*should be unique per bridge system */
   cfg_ptr->max_flows=N_ROUTING_MAX_FLOWS; /*Maximum number of flow for this bridging system*/
   cfg_ptr->classification_mode = WP_IW_NO_CLASSIFIER;
   cfg_ptr->classifier_config=classifier_config;
   cfg_ptr->learning_mode=WP_BRIDGE_LEARN_DISABLE;

   cfg_ptr->learning_queue.int_queue_num=0;
   cfg_ptr->learning_queue.learning_queue_size=0;
   cfg_ptr->learning_queue.interrupt_enable=0;
   cfg_ptr->learning_queue.interrupt_rate=0;

   cfg_ptr->forwarding_table_size=WP_IW_HASH_UNUSED;
   cfg_ptr->member_set_size=WP_IW_HASH_UNUSED;
   cfg_ptr->buffer_gap=64;
   cfg_ptr->max_iw_ports=WTI_MAX_PW;
   cfg_ptr->dfc_info=NULL;
   cfg_ptr->svl_mode=WP_IW_SVL_DISABLED;
#if WTI_NUM_OF_VLAN_TAGS == 2
   cfg_ptr->stag_ether_type=0x8101;
#else
   cfg_ptr->stag_ether_type=0x8100; /*the external Ethernet type supported
                                      by this bridging system for ingress and egress packets. */
#endif
   cfg_ptr->dfc_log_mode = WP_IW_BRIDGE_DFC_LOG_DISABLE;
#else
   classifier_config.classification_parsing_mode =WP_IW_DYNAMIC_CLASS;
   classifier_config.max_classification_rules=WTI_MAX_PW;
   classifier_config.number_parse_fields=0;
   classifier_config.parse_keys = NULL;
   cfg_ptr->tag = 0x1234; /*should be unique per bridge system */
   cfg_ptr->max_flows=N_ROUTING_MAX_FLOWS; /*Maximum number of flow for this bridging system*/
   cfg_ptr->classification_mode =WP_IW_CLASSIFIER_BASED_BRIDGING;
   cfg_ptr->classifier_config=classifier_config;
   cfg_ptr->learning_mode=WP_BRIDGE_LEARN_DISABLE;

   cfg_ptr->learning_queue.int_queue_num=0;
   cfg_ptr->learning_queue.learning_queue_size=0;
   cfg_ptr->learning_queue.interrupt_enable=0;
   cfg_ptr->learning_queue.interrupt_rate=0;

   cfg_ptr->forwarding_table_size=WP_IW_HASH_UNUSED;
   cfg_ptr->member_set_size=WP_IW_HASH_UNUSED;
   cfg_ptr->buffer_gap=64;
   cfg_ptr->max_iw_ports=WTI_MAX_PW;
   cfg_ptr->dfc_info=&dfc_sys_info;
   cfg_ptr->svl_mode=WP_IW_SVL_DISABLED;
#if WTI_NUM_OF_VLAN_TAGS == 2
   cfg_ptr->stag_ether_type=0x8101;
#else
   cfg_ptr->stag_ether_type=0x8100; /*the external Ethernet type supported
                                      by this bridging system for ingress and egress packets. */
#endif
   cfg_ptr->dfc_log_mode = WP_IW_BRIDGE_DFC_LOG_DISABLE;
#endif
}

#if WTI_CESOP_MPLS_IW
/*****************************************************************************
 * Function name: WTI_MplsIwSystemConfig
 * Description  : configure MPLS routing IW system in the PSN to TDM direction
 * Input  params: cfg_ptr - pointer to configuration structure
 * Output params:
 * Return val   : none
 *****************************************************************************/
void WTI_MplsIwSystemConfig(WP_iw_sys_mpls *cfg_ptr)
{
   memset(mpls_label_ranges, 0, (2 * (sizeof(WP_mpls_label_range))));
   memset(cfg_ptr, 0, sizeof(WP_iw_sys_mpls));

   mpls_label_ranges[0].min_label = (WTI_MPLS_LABEL >> 12);
   mpls_label_ranges[0].max_label = (WTI_MPLS_LABEL >> 12) + N_ROUTING_MAX_FLOWS;
   mpls_label_ranges[1].min_label = 0xffffffff;
   mpls_label_ranges[1].max_label = 0xffffffff;

   cfg_ptr->max_flows = N_ROUTING_MAX_FLOWS;
   cfg_ptr->buffer_gap = 64;
   cfg_ptr->edge_out_iw_system = 0;
   cfg_ptr->label_range = mpls_label_ranges;
}

/*****************************************************************************
 * Function name: WTI_MplsFlowConfig
 * Description  : configure MPLS routing flow.
 * Input  params: cfg_ptr - pointer to configuration structure
 * Output params:
 * Return val   : none
 *****************************************************************************/
void WTI_MplsFlowConfig(WP_flow_mpls *cfg_ptr, int pw_index)
{
   memset(cfg_ptr, 0, sizeof(WP_flow_mpls));

   cfg_ptr->aggregation = the_system->pw[pw_index].psn2tdm_flow_agg;
   cfg_ptr->wred_entry = 0;
   cfg_ptr->mpls_label = (WTI_MPLS_LABEL >> 12) + pw_index;
   cfg_ptr->deny_mode = 0;
}

/*****************************************************************************
 * Function name: WTI_MplsRxBindingRoutingConfig
 * Description  : configure Rx binding in the PSN to TDM direction for mpls
 *                routing IW sytem.
 * Input  params: cfg_ptr - pointer to configuration structure
 * Output params:
 * Return val   : none
 *****************************************************************************/
void WTI_MplsRxBindingRoutingConfig(WP_rx_binding_mpls *cfg_ptr)
{
   memset(cfg_ptr, 0, sizeof(WP_rx_binding_mpls));

   cfg_ptr->encap_mode = 0;
   cfg_ptr->l3_support_protocol = WP_IW_MPLS_UNI;
#if WTI_CESOP_NATIVE_MPLS_IW
   cfg_ptr->encap_mode = WP_IW_PPP_POS_NATIVE;
#endif
   cfg_ptr->mru = 318; /* 256 maximum payload size + 58 encapsulation + 4 FCS */
}

#endif

/*****************************************************************************
 * Function name: WTI_RxBindingRoutingConfig
 * Description  : configure Rx binding in the PSN to TDM direction
 * Input  params: cfg_ptr - pointer to configuration structure
 * Output params:
 * Return val   : none
 *****************************************************************************/
void WTI_RxBindingRoutingConfig(WP_rx_binding_routing *cfg_ptr)
{
   memset(cfg_ptr, 0, sizeof(WP_rx_binding_routing));

   cfg_ptr->encap_mode = 0;
#if WTI_CESOP_MPLS_IW
   cfg_ptr->l3_support_protocol = WP_IW_MPLS_UNI;
#if WTI_CESOP_NATIVE_MPLS_IW
   cfg_ptr->encap_mode = WP_IW_PPP_POS_NATIVE;
#endif
#else /* ! WTI_CESOP_MPLS_IW */
#if (TWO_BOARDS_SIMULATION && __linux__)
   cfg_ptr->l3_support_protocol = WP_IW_IPV4 | WP_IW_ARP | WP_IW_RARP;
#else
   cfg_ptr->l3_support_protocol = WP_IW_IPV4;
#endif
#endif /* WTI_CESOP_MPLS_IW */
   cfg_ptr->mru = N_IW_PSN2TDM_QUEUE_BUFFER_SIZE - 64; /* 64 is the buffer gap */
   cfg_ptr->rx_direction = WP_IW_NETWORK_UNKNOWN;
   cfg_ptr->input_port = 0;
   cfg_ptr->decomp_group = WTI_INVALID_HANDLE;
#if WTI_PCE_CLASSIFIER
   cfg_ptr->pce_if_handle = the_system->pce_interface_handle;
#endif
}

/*****************************************************************************
 * Function name: WTI_RxBindingBridgingConfig
 * Description  : configure Rx binding in the PSN to TDM direction
 * Input  params: cfg_ptr - pointer to configuration structure
 * Output params:
 * Return val   : none
 *****************************************************************************/
void WTI_RxBindingBridgeConfig(WP_rx_binding_bridging *cfg_ptr)
{
   memset(cfg_ptr, 0, sizeof(WP_rx_binding_bridging));

   cfg_ptr->encap_mode = 0;
   cfg_ptr->mru = N_IW_PSN2TDM_QUEUE_BUFFER_SIZE - 64; /* 64 is the buffer gap */
   cfg_ptr->vcfcs =0;
   cfg_ptr->input_port = WTI_INVALID_HANDLE;
#if WTI_PCE_CLASSIFIER
   cfg_ptr->pce_if_handle = the_system->pce_interface_handle;
#endif
}

/*****************************************************************************
 * Function name: WTI_RxBindingDirectMapConfig
 * Description  : configure Rx binding in the TDM to PSN direction
 * Input  params: cfg_ptr - pointer to configuration structure
 *                pw_index - PW index
 * Output params:
 * Return val   : none
 *****************************************************************************/
void WTI_RxBindingDirectMapConfig(WP_rx_binding_directmap *cfg_ptr, int pw_index)
{
   memset(cfg_ptr, 0, sizeof(WP_rx_binding_directmap));

   cfg_ptr->default_aggregation = the_system->pw[pw_index].tdm2psn_flow_agg;
}

/*****************************************************************************
 * Function name: WTI_CesopTxBindingConfig
 * Description  : configure Tx binding in the PSN to TDM direction
 * Input  params: cfg_ptr - pointer to configuration structure
 * Output params:
 * Return val   : none
 *****************************************************************************/
void WTI_CesopTxBindingConfig(WP_iw_tx_binding_cesop *cfg_ptr)
{
   memset(cfg_ptr, 0, sizeof(WP_iw_tx_binding_cesop));

   cfg_ptr->window_tx_threshold = pw_config.window_tx_threshold;
   cfg_ptr->window_switchover_threshold = pw_config.window_switchover_threshold;
   cfg_ptr->window_max_threshold = pw_config.window_max_threshold;
   cfg_ptr->lops_detection = pw_config.lops_detection;
   cfg_ptr->consec_pkts_in_synch_th = pw_config.consec_pkts_in_synch_th;
   cfg_ptr->consec_miss_pkts_out_synch_th = pw_config.consec_miss_pkts_out_synch_th;
}

/*****************************************************************************
 * Function name: WTI_TxBindingEnetConfig
 * Description  : configure Tx binding in the TDM to PSN direction
 * Input  params: cfg_ptr - pointer to configuration structure
 * Output params:
 * Return val   : none
 *****************************************************************************/
void WTI_TxBindingEnetConfig(WP_tx_binding *cfg_ptr)
{
    memset(cfg_ptr, 0, sizeof(WP_tx_binding));

   cfg_ptr->res0 = 0;
   cfg_ptr->dci_mode = 1;
   cfg_ptr->maxt = 336;
}

/*****************************************************************************
 * Function name: WTI_TransChannelRxConfig
 * Description  : configure transparent Rx channel
 * Input  params: cfg_ptr - pointer to configuration structure
 * Output params:
 * Return val   : none
 *****************************************************************************/
void WTI_TransChannelRxConfig(WP_ch_trans_pwe3 *cfg_ptr)
{
   memset(cfg_ptr, 0, sizeof(WP_ch_trans_pwe3));

   cfg_ptr->statmode = pw_config.statmode;
   cfg_ptr->num_slots = pw_config.n_slots;
#if WTI_CESOP_TDI
   cfg_ptr->slots = pw_config.slots;
#else
   cfg_ptr->slots = 0;
#endif
   cfg_ptr->rx_buffersize = pw_config.rx_buffersize;

#if WTI_CES_SHAPING_ENABLED
   cfg_ptr->shaping_mode = WP_ENABLE;
   cfg_ptr->tx_cwid = WP_CW_ID_A;
#endif
}

/*****************************************************************************
 * Function name: WTI_TransChannelTxConfig
 * Description  : configure transparent Tx channel
 * Input  params: cfg_ptr - pointer to configuration structure
 * Output params:
 * Return val   : none
 *****************************************************************************/
void WTI_TransChannelTxConfig(WP_ch_trans_pwe3 *cfg_ptr)
{
   memset(cfg_ptr, 0, sizeof(WP_ch_trans_pwe3));
   cfg_ptr->num_slots = pw_config.n_slots;
#if WTI_CESOP_TDI
   cfg_ptr->slots = pw_config.slots;
#else
   cfg_ptr->slots = 0;
#endif
   cfg_ptr->statmode = pw_config.statmode;
   cfg_ptr->tx_jitter_buffer_size = pw_config.tx_jitter_buffer_size;
   cfg_ptr->tx_udp_dummy_mode = pw_config.tx_udp_dummy_mode;
   memcpy(cfg_ptr->tx_udpattern, &pw_config.tx_udpattern, 32);
   memcpy(cfg_ptr->tx_udp_dummy_pattern, &pw_config.tx_udp_dummy_pattern, 32);

   cfg_ptr->tx_udcas = pw_config.tx_udcas;
   cfg_ptr->rx_buffersize = 0;
}

#if WTI_PCE_CLASSIFIER
/*****************************************************************************
 * Function name: WTI_PceRuleConfig
 * Description  : configure PCE Rule
 * Input  params: cfg_ptr - pointer to configuration structure
 *                pw_index - PW index
 * Output params:
 * Return val   : none
 *****************************************************************************/
void WTI_PceRuleConfig(WP_pce_rule_classification *cfg_ptr, WP_U32 pw_index)
{
   WP_pce_result_flow_agg result_flow_agg;
   WP_U8 mpls1,mpls2,mpls3,mpls4;
   WP_U32 mpls, mpls_offset;
#if UDP_DES_PORT_PCE_RULE
   WP_U8 udp_dest_port[2];
   WP_U16 dest_port;
   WP_U32 udp_offset;
#endif/*UDP_DES_PORT_PCE_RULE*/
#if WTI_CLOCK_REC_SNAKE_ENABLED
   WP_U32 pw_index_in,num_of_lines;
#endif


   memset(cfg_ptr, 0, sizeof(WP_pce_rule_classification));

#if (!WTI_CESOP_MPLS_IW)
   /* get destination ip from TDM to PSN flow aggregation config */
#if TWO_BOARDS_SIMULATION
   offset = tdm2psn_flow_agg_config.l3_header_offset + WTI_SRC_IP_OFFSET;
#if UDP_DES_PORT_PCE_RULE
   udp_offset = tdm2psn_flow_agg_config.l3_header_offset + 20;
#endif/*UDP_DES_PORT_PCE_RULE*/
#else /*TWO_BOARDS_SIMULATION*/
   offset = tdm2psn_flow_agg_config.l3_header_offset + WTI_DEST_IP_OFFSET;
#if UDP_DES_PORT_PCE_RULE
   udp_offset = tdm2psn_flow_agg_config.l3_header_offset + 22;
#endif/*UDP_DES_PORT_PCE_RULE*/
#endif/*TWO_BOARDS_SIMULATION*/

   ip1 = (WP_U8)tdm2psn_flow_agg_config.prefix_header[offset];
   ip2 = (WP_U8)tdm2psn_flow_agg_config.prefix_header[offset+1];
   ip3 = (WP_U8)tdm2psn_flow_agg_config.prefix_header[offset+2];
   ip4 = (WP_U8)tdm2psn_flow_agg_config.prefix_header[offset+3];

   ip = (WP_U32)((ip1 << 24) | (ip2<< 16) | (ip3<<8) | ip4);

#if UDP_DES_PORT_PCE_RULE
   udp_dest_port[0] = (WP_U8) tdm2psn_flow_agg_config.prefix_header[udp_offset];
   udp_dest_port[1] = (WP_U8) tdm2psn_flow_agg_config.prefix_header[udp_offset+1];

   dest_port = (WP_U16)((udp_dest_port[0] << 8) | udp_dest_port[1]);
#endif
   cfg_ptr->tag = 1;
   cfg_ptr->match_action = WP_PCE_RULE_MATCH_ACCEPT;
   cfg_ptr->enabled = WP_ENABLE;

   cfg_ptr->filter_handle = the_system->pce_filter_handle;

   cfg_ptr->rule_fields[0].field_id = WP_PCE_FIELD_ID_IPV4_DA;
   cfg_ptr->rule_fields[0].value.ip = ip;
#if UDP_DES_PORT_PCE_RULE
   cfg_ptr->rule_fields[1].field_id = WP_PCE_FIELD_ID_L4_DST_PORT;
   cfg_ptr->rule_fields[1].value.l4_dest = dest_port;

   cfg_ptr->rule_fields[2].field_id = WP_PCE_FIELD_ID_LAST;
#else
   cfg_ptr->rule_fields[1].field_id = WP_PCE_FIELD_ID_LAST;
#endif

#else/*WTI_CESOP_MPLS_IW*/
#if WTI_CESOP_MPLS_OVER_ENET
#if WTI_NUM_OF_VLAN_TAGS == 0
   mpls_offset = WTI_ETHERNET_HEADER_SIZE;
#endif
#if WTI_NUM_OF_VLAN_TAGS == 1
   mpls_offset = WTI_ETHERNET_HEADER_SIZE+WTI_VLAN_TAG_SIZE;
#endif
#if WTI_NUM_OF_VLAN_TAGS == 2
   mpls_offset = WTI_ETHERNET_HEADER_SIZE+(2*WTI_VLAN_TAG_SIZE);
#endif
#else /*WTI_CESOP_MPLS_OVER_ENET*/
#if WTI_CESOP_NATIVE_MPLS_IW
   mpls_offset = 0;
#else
   mpls_offset =WTI_PPP_HEADER_SIZE;
#endif
#endif/*WTI_CESOP_MPLS_OVER_ENET*/

   mpls1 = (WP_U8)tdm2psn_flow_agg_config.prefix_header[mpls_offset];
   mpls2 = (WP_U8)tdm2psn_flow_agg_config.prefix_header[mpls_offset+1];
   mpls3 = (WP_U8)tdm2psn_flow_agg_config.prefix_header[mpls_offset+2];
   mpls4 = (WP_U8)tdm2psn_flow_agg_config.prefix_header[mpls_offset+3];

   mpls = (WP_U32)((mpls1 << 24) | (mpls2<< 16) | (mpls3<<8) | mpls4);

#if WTI_PCE_CLASSIFIER
   mpls &= 0xffffff00; /* Mask upper 24 bits (label + exp + EOS) */
   mpls >>= 8;
#else
   mpls &= 0xfffff000; /* Mask upper 20 bits (label only) */
   mpls >>= 12;
#endif

 

#if WTI_CLOCK_REC_SNAKE_ENABLED
   if(pw_index == 0)
   {
      pw_index_in = 62 + cr_snake_num_of_lines;
      printf("SNAKE MODE: mpls %x ->",mpls);
      mpls &= 0xFFFFF00F;
      mpls |=  (pw_index_in << 4);
      printf("%x  pw %d -> pw %d \n",mpls,pw_index_in,pw_index);
   }
   else if(pw_index == 63)
   {
      pw_index_in = 0;
      printf("SNAKE MODE: mpls %x ->",mpls);
      mpls &= 0xFFFFF00F;
      mpls |=  (pw_index_in << 4);
      printf("%x  pw %d -> pw %d \n",mpls,pw_index_in,pw_index);
   }
   else if(pw_index >= 64 && pw_index < (64+cr_snake_num_of_lines -1))
   {
      pw_index_in = pw_index-1;
      printf("SNAKE MODE: mpls %x ->",mpls);
      mpls &= 0xFFFFF00F;
      mpls |=  (pw_index_in << 4);
      printf("%x  pw %d -> pw %d \n",mpls,pw_index_in,pw_index);
   }
#endif
 
   cfg_ptr->tag = 1;
   cfg_ptr->match_action = WP_PCE_RULE_MATCH_ACCEPT;
   cfg_ptr->enabled = WP_ENABLE;
   cfg_ptr->filter_handle = the_system->pce_filter_handle;
   cfg_ptr->rule_fields[0].field_id = WP_PCE_FIELD_ID_MPLS_HEADER2;
   cfg_ptr->rule_fields[0].value.mpls_header = mpls;
   cfg_ptr->rule_fields[1].field_id = WP_PCE_FIELD_ID_LAST;
#endif /*WTI_CESOP_MPLS_IW*/

   result_flow_agg.flow_aggregation = the_system->pw[pw_index].psn2tdm_flow_agg;

   cfg_ptr->match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
   cfg_ptr->match_result[0].param.flow_agg = result_flow_agg;

   cfg_ptr->match_result[1].result_type = WP_PCE_RESULT_LAST;

}

/*****************************************************************************
 * Function name: WTI_PceRuleConfigBridge
 * Description  : configure PCE Rule
 * Input  params: cfg_ptr - pointer to configuration structure
 *                pw_index - PW index
 * Output params:
 * Return val   : none
 *****************************************************************************/
void WTI_PceRuleConfigBridge(WP_pce_rule_classification *cfg_ptr, WP_U32 pw_index)
{
   WP_pce_result_flow_agg result_flow_agg;
   WP_U32 mpls, mpls_offset;
   WP_U8 mpls1,mpls2,mpls3,mpls4;

   memset(cfg_ptr, 0, sizeof(WP_pce_rule_classification));

#if (!WTI_CESOP_MPLS_IW)
   /* get destination ip from TDM to PSN flow aggregation config */
#if TWO_BOARDS_SIMULATION
   offset = tdm2psn_flow_agg_config.l3_header_offset + WTI_SRC_IP_OFFSET;
#else
   offset = tdm2psn_flow_agg_config.l3_header_offset + WTI_DEST_IP_OFFSET;
#endif

   ip1 = (WP_U8)tdm2psn_flow_agg_config.prefix_header[offset];
   ip2 = (WP_U8)tdm2psn_flow_agg_config.prefix_header[offset+1];
   ip3 = (WP_U8)tdm2psn_flow_agg_config.prefix_header[offset+2];
   ip4 = (WP_U8)tdm2psn_flow_agg_config.prefix_header[offset+3];

   ip = (WP_U32)((ip1 << 24) | (ip2<< 16) | (ip3<<8) | ip4);

   cfg_ptr->tag = 1;
   cfg_ptr->match_action = WP_PCE_RULE_MATCH_ACCEPT;
   cfg_ptr->enabled = WP_ENABLE;

   cfg_ptr->filter_handle = the_system->pce_filter_handle;

   cfg_ptr->rule_fields[0].field_id = WP_PCE_FIELD_ID_IPV4_DA;
   cfg_ptr->rule_fields[0].value. = ip;
   cfg_ptr->rule_fields[1].field_id = WP_PCE_FIELD_ID_LAST;

#else/*WTI_CESOP_MPLS_IW*/
#if WTI_CESOP_MPLS_OVER_ENET
#if WTI_NUM_OF_VLAN_TAGS == 0
   mpls_offset = WTI_ETHERNET_HEADER_SIZE;
#endif
#if WTI_NUM_OF_VLAN_TAGS == 1
   mpls_offset = WTI_ETHERNET_HEADER_SIZE+WTI_VLAN_TAG_SIZE;
#endif
#if WTI_NUM_OF_VLAN_TAGS == 2
   mpls_offset = WTI_ETHERNET_HEADER_SIZE+(2*WTI_VLAN_TAG_SIZE);
#endif
#else /*WTI_CESOP_MPLS_OVER_ENET*/
#if WTI_CESOP_NATIVE_MPLS_IW
   mpls_offset = 0;
#else
   mpls_offset = WTI_PPP_HEADER_SIZE;
#endif
#endif/*WTI_CESOP_MPLS_OVER_ENET*/

   mpls1 = (WP_U8)tdm2psn_flow_agg_config.prefix_header[mpls_offset];
   mpls2 = (WP_U8)tdm2psn_flow_agg_config.prefix_header[mpls_offset+1];
   mpls3 = (WP_U8)tdm2psn_flow_agg_config.prefix_header[mpls_offset+2];
   mpls4 = (WP_U8)tdm2psn_flow_agg_config.prefix_header[mpls_offset+3];

   mpls = (WP_U32)((mpls1 << 24) | (mpls2<< 16) | (mpls3<<8) | mpls4);

#if WTI_PCE_CLASSIFIER
   mpls &= 0xffffff00; /* Mask upper 24 bits (label + exp + EOS) */
   mpls >>= 8;
#else
   mpls &= 0xfffff000; /* Mask upper 20 bits (label only) */
   mpls >>= 12;
#endif

   cfg_ptr->tag = 1;
   cfg_ptr->match_action = WP_PCE_RULE_MATCH_ACCEPT;
   cfg_ptr->enabled = WP_ENABLE;
   cfg_ptr->filter_handle = the_system->pce_filter_handle;
   cfg_ptr->rule_fields[0].field_id = WP_PCE_FIELD_ID_MPLS_HEADER2;
   cfg_ptr->rule_fields[0].value.mpls_header = mpls;
   cfg_ptr->rule_fields[1].field_id = WP_PCE_FIELD_ID_LAST;
#endif

   result_flow_agg.flow_aggregation = the_system->pw[pw_index].psn2tdm_flow_agg;

   cfg_ptr->match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
   cfg_ptr->match_result[0].param.flow_agg = result_flow_agg;
   cfg_ptr->match_result[1].result_type = WP_PCE_RESULT_LAST;
}

#else /*Winpath*/
#if (!WTI_CESOP_MPLS_IW)
/*****************************************************************************
 * Function name: WTI_DfcRuleConfig
 * Description  : configure DFC Rule
 * Input  params: cfg_ptr - pointer to configuration structure
 *                pw_index - PW index
 * Output params:
 * Return val   : none
 *****************************************************************************/
void WTI_DfcRuleConfig(WP_flow_class_rule *cfg_ptr, WP_U32 pw_index)
{
   WP_U8 ip1,ip2,ip3,ip4;
   WP_U32 offset;
#if UDP_DES_PORT_DFC_RULE
   WP_U8 udp_dest_port[2];
   WP_U16 dest_port;
   WP_U32 udp_offset;
#endif/*UDP_DES_PORT_DFC_RULE*/
#if WTI_CESOP_MEF8_ENCAP
   WP_U32 ii;
   WP_CHAR temp_buf[2];
#endif

   memset(cfg_ptr, 0, sizeof(WP_flow_class_rule));
   memset(&the_system->pw[pw_index].dfc_flow_info, 0, sizeof(WP_dfc_flow_info));

   the_system->pw[pw_index].dfc_flow_info.filter_handle = the_system->dfc_filter_handle;
   the_system->pw[pw_index].dfc_flow_info.input_port = 0;
   the_system->pw[pw_index].dfc_flow_info.output_port = 0;

   /* get destination ip from TDM to PSN flow aggregation config */
#if TWO_BOARDS_SIMULATION
   offset = tdm2psn_flow_agg_config.l3_header_offset + WTI_SRC_IP_OFFSET;
#if UDP_DES_PORT_DFC_RULE
   udp_offset = tdm2psn_flow_agg_config.l3_header_offset + 20;
#endif/*UDP_DES_PORT_DFC_RULE*/
#else /*TWO_BOARDS_SIMULATION*/
    offset = tdm2psn_flow_agg_config.l3_header_offset + WTI_DEST_IP_OFFSET;
#if UDP_DES_PORT_DFC_RULE
    udp_offset = tdm2psn_flow_agg_config.l3_header_offset + 22;
#endif/*UDP_DES_PORT_DFC_RULE*/
#endif/*TWO_BOARDS_SIMULATION*/

#if UDP_DES_PORT_DFC_RULE
   ip1 = (WP_U8)tdm2psn_flow_agg_config.prefix_header[offset];
   ip2 = (WP_U8)tdm2psn_flow_agg_config.prefix_header[offset+1];
   ip3 = (WP_U8)tdm2psn_flow_agg_config.prefix_header[offset+2];
   ip4 = (WP_U8)tdm2psn_flow_agg_config.prefix_header[offset+3];

   udp_dest_port[0] = (WP_U8) tdm2psn_flow_agg_config.prefix_header[udp_offset];
   udp_dest_port[1] = (WP_U8) tdm2psn_flow_agg_config.prefix_header[udp_offset+1];

   dest_port = (WP_U16)((udp_dest_port[0] << 8) | udp_dest_port[1]);

   sprintf(the_system->pw[pw_index].dfc_rule,
           "n;n;n;n;%d.%d/0.0;%d.%d/0.0;%d",
           (WP_U32) ip1,
           (WP_U32) ip2,
           (WP_U32) ip3,
           (WP_U32) ip4,
           (WP_U32) dest_port);
#else/*UDP_DES_PORT_DFC_RULE*/
   ip1 = (WP_U8)tdm2psn_flow_agg_config.prefix_header[offset];
   ip2 = (WP_U8)tdm2psn_flow_agg_config.prefix_header[offset+1];
   ip3 = (WP_U8)tdm2psn_flow_agg_config.prefix_header[offset+2];
   ip4 = (WP_U8)tdm2psn_flow_agg_config.prefix_header[offset+3];
   sprintf(the_system->pw[pw_index].dfc_rule,
           "n;n;n;n;%d.%d/0.0;%d.%d/0.0;",
           (WP_U32) ip1,
           (WP_U32) ip2,
           (WP_U32) ip3,
           (WP_U32) ip4);
#endif/*UDP_DES_PORT_DFC_RULE*/

#if WTI_CESOP_MEF8_ENCAP
   strcpy(the_system->pw[pw_index].dfc_rule, "");
   for (ii=0; ii<58; ii++)
      strcat(the_system->pw[pw_index].dfc_rule, "n;");
   sprintf(temp_buf, "%d;", pw_index);

   strcat(the_system->pw[pw_index].dfc_rule, temp_buf);

#endif

   cfg_ptr->classifier_string = the_system->pw[pw_index].dfc_rule;
   cfg_ptr->dfc_info = &the_system->pw[pw_index].dfc_flow_info;
   cfg_ptr->match_action = WP_FLOW_CLASS_MODE_FLOW_AGG;
   cfg_ptr->action_info.forwarding_action.flow_aggregation = the_system->pw[pw_index].psn2tdm_flow_agg;
   cfg_ptr->wred_entry = 0;
}


#if WTI_CESOP_MEF8_ENCAP
/*****************************************************************************
 * Function name: WTI_DfcRuleConfigBridgeMef
 * Description  : configure DFC Rule for ECID encapsulation
 * Input  params: cfg_ptr - pointer to configuration structure
 *                pw_index - PW index
 * Output params:
 * Return val   : none
 *****************************************************************************/
void WTI_DfcRuleConfigBridgeMef(WP_flow_class_rule *cfg_ptr, WP_U32 pw_index)
{
   WP_U32 ii;

   memset(cfg_ptr, 0, sizeof(WP_flow_class_rule));
   memset(&the_system->pw[pw_index].dfc_flow_info, 0, sizeof(WP_dfc_flow_info));

   the_system->pw[pw_index].dfc_flow_info.input_port = 0;
   the_system->pw[pw_index].dfc_flow_info.output_port = 0;
   the_system->pw[pw_index].dfc_flow_info.filter_handle = the_system->dfc_filter_handle;

   strcpy(the_system->pw[pw_index].dfc_rule, "");
   for (ii=0; ii<58; ii++)
      strcat(the_system->pw[pw_index].dfc_rule, "n;");
   strcat(the_system->pw[pw_index].dfc_rule, "0x55555;");


   cfg_ptr->classifier_string = the_system->pw[pw_index].dfc_rule;
   cfg_ptr->dfc_info = &the_system->pw[pw_index].dfc_flow_info;
   cfg_ptr->match_action = WP_FLOW_CLASS_MODE_FLOW_AGG;
   cfg_ptr->action_info.forwarding_action.flow_aggregation = the_system->pw[pw_index].psn2tdm_flow_agg;
   cfg_ptr->wred_entry = 0;
}
#endif

/*****************************************************************************
 * Function name: WTI_DfcRuleConfigBridge
 * Description  : configure DFC Rule
 * Input  params: cfg_ptr - pointer to configuration structure
 *                pw_index - PW index
 * Output params:
 * Return val   : none
 *****************************************************************************/
void WTI_DfcRuleConfigBridge(WP_flow_class_rule *cfg_ptr, WP_U32 pw_index)
{
   WP_U8 ip1,ip2,ip3,ip4;
   WP_U32 offset;

   memset(cfg_ptr, 0, sizeof(WP_flow_class_rule));
   memset(&the_system->pw[pw_index].dfc_flow_info, 0, sizeof(WP_dfc_flow_info));

   the_system->pw[pw_index].dfc_flow_info.input_port = 0;
   the_system->pw[pw_index].dfc_flow_info.output_port = 0;
   the_system->pw[pw_index].dfc_flow_info.filter_handle = the_system->dfc_filter_handle;

   /* get detination ip from TDM to PSN flow aggregation config */
#if TWO_BOARDS_SIMULATION
   offset = tdm2psn_flow_agg_config.l3_header_offset + WTI_SRC_IP_OFFSET;
#else
    offset = tdm2psn_flow_agg_config.l3_header_offset + WTI_DEST_IP_OFFSET;
#endif

   ip1 = (WP_U8)tdm2psn_flow_agg_config.prefix_header[offset];
   ip2 = (WP_U8)tdm2psn_flow_agg_config.prefix_header[offset+1];
   ip3 = (WP_U8)tdm2psn_flow_agg_config.prefix_header[offset+2];
   ip4 = (WP_U8)tdm2psn_flow_agg_config.prefix_header[offset+3];

   sprintf(the_system->pw[pw_index].dfc_rule,
           "n;n;n;n;%d.%d/0.0;%d.%d/0.0;",
           (WP_U32) ip1,
           (WP_U32) ip2,
           (WP_U32) ip3,
           (WP_U32) ip4);

   cfg_ptr->classifier_string = the_system->pw[pw_index].dfc_rule;
   cfg_ptr->dfc_info          = &the_system->pw[pw_index].dfc_flow_info;
   cfg_ptr->match_action =WP_FLOW_CLASS_MODE_FLOW_AGG;
   cfg_ptr->action_info.forwarding_action.flow_aggregation =the_system->pw[pw_index].psn2tdm_flow_agg;
   cfg_ptr->wred_entry =0;

}

#endif /*(!WTI_CESOP_MPLS_IW)*/
#endif /*WINPATH*/


/*****************************************************************************
 * Function name: WTI_IpChecksumCalc
 * Description  : calculate checksum of IP header
 * Input  params: buff - pointer to buffer, len - size of IP header
 * Output params:
 * Return val   : calculated checksum
 *****************************************************************************/
unsigned short WTI_IpChecksumCalc(unsigned char *buff, unsigned short len)
{
   unsigned short *bu16 = (unsigned short*)buff;
   unsigned int sum=0;
   unsigned short i;

   /* make 16 bit words out of every two adjacent
      8 bit words in the packet and add them up */
   len = len/2;
   for (i=0;i<len;i++){
      sum = sum + (WP_U32) bu16[i];
   }

   /* take only 16 bits out of the 32 bit sum
      and add up the carries */
   while (sum>>16)
      sum = (sum & 0xFFFF)+(sum >> 16);

   /* one's complement the result */
   sum = ~sum;

   return ((unsigned short) sum);
}

#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
void WTI_ClockRecEvents(WP_tag tag, WP_U32 event, WP_U32 info)
{
#if 0
   if (!WP_EVENT_FUNCTION_P(event, WP_EVENT_FUNCTION_CLOCK_REC))
      printf("--------------- INVALID EVENT %x %x --------------\n", event, info);

   if (WP_EVENT_P(event, WP_EVENT_CLOCK_REC_HOE))
      printf("--------------- CRT %d: GOT CLOCK_REC_HOE EVENT %x %x -----------\n", tag, event, info);
#endif
}
#endif

void WTI_cesop_event_function(WP_tag tag, WP_U32 data, WP_U32 info)
{
   if (WP_EVENT_FUNCTION_P(data, WP_EVENT_FUNCTION_PWE))
   {
#if 0
      if (data & WP_EVENT_CESOP_IDLE_START) /* receive IDLE start event for a PSN2TDM flow direction */
         printf("WP_EVENT_CESOP_IDLE_START EVENT OCCURED! tag=%d\n",tag);
      if (data & WP_EVENT_CESOP_IDLE_END)   /* receive IDLE end event for a PSN2TDM flow direction */
         printf("WP_EVENT_CESOP_IDLE_END EVENT OCCURED! tag=%d\n",tag);
      if (data & WP_EVENT_CESOP_AIS_START)  /* receive AIS start event for a PSN2TDM flow direction */
         printf("WP_EVENT_CESOP_AIS_START EVENT OCCURED! tag=%d\n",tag);
      if (data & WP_EVENT_CESOP_AIS_END)    /* receive AIS end event for a PSN2TDM flow direction */
         printf("WP_EVENT_CESOP_AIS_END EVENT OCCURED! tag=%d\n",tag);
      if (data & WP_EVENT_CESOP_RDI_START)   /* receive RDI start event for a PSN2TDM flow direction */
         printf("WP_EVENT_CESOP_RDI_START EVENT OCCURED! tag=%d\n",tag);
      if (data & WP_EVENT_CESOP_RDI_END)     /* receive RDI end event for a PSN2TDM flow direction */
         printf("WP_EVENT_CESOP_RDI_END EVENT OCCURED! tag=%d\n",tag);
      if (data & WP_EVENT_CESOP_R_BIT_START)   /* receive R bit start event for a PSN2TDM flow direction */
         printf("WP_EVENT_CESOP_R_BIT_START EVENT OCCURED! tag=%d\n",tag);
      if (data & WP_EVENT_CESOP_R_BIT_END)     /* receive R bit end event for a PSN2TDM flow direction */
         printf("WP_EVENT_CESOP_R_BIT_END EVENT OCCURED! tag=%d\n",tag);
      if (data & WP_EVENT_CESOP_LOPS_START)    /* receive R bit start event for a PSN2TDM flow direction */
         printf("WP_EVENT_CESOP_LOPS_START EVENT OCCURED! tag=%d\n",tag);
      if (data & WP_EVENT_CESOP_LOPS_END)      /* receive R bit end event for a PSN2TDM flow direction */
         printf("WP_EVENT_CESOP_LOPS_END EVENT OCCURED! tag=%d\n",tag);
      else
#endif
         return;
   }
}

/*****************************************************************************/
/*                         MISC UTILS                                        */
/*****************************************************************************/

/*****************************************************************************
 * Function name: WTI_ShowMemStat
 * Description  : display memory on buses
 * Input  params: none
 * Output params:
 * Return val   : none
 *****************************************************************************/
void WTI_ShowMemStat(void)

{
   WP_status status;
   WP_U32 bytes;

   printf("/**********************************************************/\n");

#ifdef WP_BOARD_WDS3_SL
   printf("Memory status: No Packet bus \n");
#else   /* NOT  WP_BOARD_WDS3_SL */
   status = WP_MemoryAvailable(the_system->wpid, WP_BUS_PACKET, APP_BANK_PACKET, &bytes);
   printf("Memory status: Packet bus bytes left   = %d\n", bytes);
#endif  /* NOT WP_BOARD_WDS3_SL */
   status = WP_MemoryAvailable(the_system->wpid, WP_BUS_PARAM, APP_BANK_PARAM, &bytes);
   printf("Memory status: Param bus bytes left    = %d\n", bytes);

   status = WP_MemoryAvailable(the_system->wpid, WP_BUS_INTERNAL, APP_BANK_INTERNAL, &bytes);
   printf("Memory status: Internal bus bytes left = %d\n", bytes);

   status = WP_MemoryAvailable(the_system->wpid, WP_BUS_HOST, APP_BANK_HOST, &bytes);
   printf("Memory status: Host bus bytes left     = %d\n", bytes);

#ifndef __linux__
   status = WP_MemoryAvailable(WP_WINPATH(DEFAULT_WPID), WP_HEAP_BUS, 0, &bytes);
   printf("Memory status: Heap bytes left         = 0x%08x\n",bytes);
#endif
   printf("/**********************************************************/\n");
}

/*****************************************************************************
 * Function name: WTI_Reboot
 * Description  : reboot board and exit
 * Input  params: none
 * Output params:
 * Return val   : none
 *****************************************************************************/
void WTI_Reboot(void)
{
#ifdef QUICK
   WT_Reboot();
#else
   exit(1);
#endif
}

#if (WTI_PTP_ENABLE == 1)

/*****************************************************************************
 * Function name: WPE_PTPSetup
 * Description  : Setup PTP related objects
 *
 *
 * Input  params: none
 * Output params:
 * Return val   : none
 *****************************************************************************/
void WPE_PTPConfig(void)
{
   WP_U32 status;

   /* config board */
#if !defined(CHECKIN_TEST)
   WTI_BoardPinConfig(WTI_DEFAULT_PIN_CONFIG, 0, wpx_apll_clk0);
#endif
   WPE_CreateHostIwRxChannel();  /*create host rx channel*/
   WPE_CreateEnetRxTxChannel();

 /* Create 2 cds with 2 freqs */
 /* includes WP_PtpClockDomainCreate WPTP_PtpCDConfig */
   ptp_cds[0] = WPE_CreatePTPClockDomain(WPE_PTP_1544_Hz,WPE_CD0);
   WTI_TerminateOnError(ptp_cds[0], "WPE_CreatePTPClockDomain",__LINE__);

 /* first create 2 masters  */
   WPE_CreatePTPPorts();

  /* flow agg */
   WPE_CreateHostTermFlowAgg();

    /* iw ports */
   WPE_CreateIwBportHost();   /* after agg_host */
   WPE_CreateIwBportEnet();
   WPE_CreateEnetRxTxBinding();

   /* enable cds */
   WPE_EnablePTPClockDomain(ptp_cds[0]);

/*-------- filters & rules ---------*/
    WPE_AddBasicFilters();
    WPE_AddBasicRules();
/*--------------------------------------*/

   status = WP_IwSystemBuild(iw_sys);
   WTI_TerminateOnError(status, "WP_IwSystemBuild",__LINE__);

   WPE_EnableSystem();

   WPE_RUN_PKTS();
}



/*****************************************************************************
 * Function name: WPE_PTPInit
 * Description  : Initializes PTP module and structs
 *
 *
 * Input  params: none
 * Output params:
 * Return val   : none
 *****************************************************************************/
void WPE_PTPInit(void)
{
   WPE_CreateIWBridgeSystem();

   WPE_CreateBufferPools();

   WPE_CreateEnetPortDevice();
   WPE_CreateHostPortDevice();

   WPE_PTPModuleInitWP3();   /* WP_ModuleInit , WP_DeviceModify(dev_enet[])*/
}


void WPE_CreateIWBridgeSystem()
{

   WP_dfc_sys_info dfc_sys_info =
      {
         /* classification_hash_size */  WP_IW_2K_HASH_ENTRIES,
         /* port_filtering */            WP_IW_PORT_FILTERING_DISABLED,/*WP_IW_PORT_FILTERING_INPUT,*/
         /* deny_eq_ip */                WP_IW_DENY_EQUAL_IP_DISABLE
      };


   WP_iw_sys_bridging iw_sys_bridge =
      {
         /* tag */                           0,
         /* max_flows; */                    MAX_IW_FLOWS,
         /* classification_mode */           WP_IW_CLASSIFIER_BASED_BRIDGING,
         /* classifier_config */
         {
            /*classification_parsing_mode*/ WP_IW_DYNAMIC_CLASS,
            /*max_classification_rules;*/   (NUM_OF_EMC_FILTERS+1)*NUM_OF_RULES_PER_FILTER,
            /*number_parse_fields*/         0,
            /**parse_keys*/                 NULL
         },
         /* learning_mode;*/                 WP_IW_BRIDGE_LEARNING_DISABLE,
         /* learning_queue */
         {
            /*int_queue_num*/               0, /*WP_IW_IRQT1,*/
            /*learning_queue_size*/         0,/*LEARNING_QUEUE_SIZE,*/
            /*interrupt_enbale*/            0,/*WP_LEARNING_INTENABLE,*/
            /*interrupt_rate*/              0/*1*/
         },
         /*forwarding_table_size*/           WP_IW_32K_HASH_ENTRIES,
         /*member_set_size*/                 WP_IW_32K_HASH_ENTRIES,
         /*buffer_gap;*/                     64,
         /*max_iw_ports*/                    NUM_OF_IW_BPORTS,
         /*dfc info*/                        &dfc_sys_info,
         /*svl_mode*/                        WP_IW_SVL_DISABLED,
         /*stag_ether_type*/                 WP_IW_SYS_BRIDGE_STAG_VLAN_ETYPE_DISABLE  /* 0x9100 */
      };


/* iw_sys = WP_IwSystemCreate(wpid, WP_IW_VLAN_AWARE_BRIDGING_MODE, iw_sys_one);*/
   iw_sys = WP_IwSystemCreate(wpid, WP_IW_VLAN_AWARE_BRIDGING_MODE, &iw_sys_bridge);
   WTI_TerminateOnError(iw_sys, " WP_IwSystemCreate - Bridging", __LINE__);
}

void    WPE_CreateIWQnode()
{
   WP_handle adjunct_buffer_pool;

   /* Host (Adjunct) pool part of IW QNode */
   WP_pool_buffer_data adjunct_buffer_config =
      {
         /* n_buffers */     8, /* Number of host threads that do Host Receive (1 BD per thread)*/
         /* offset */        64,
         /* size */          32,
         /* pad */           0,
         /* databustype */   WP_BUS_HOST,
         /* bank */          APP_BANK_HOST
      };


   /* IW QNode (Free Buffer Pool part of IW QNode) */
   WP_qnode_iwq qn_iw_config =
      {
         /* interruptqueue */  0, /* for IW TX channels only*/
         /* num_buffers */     MAXT_SIZE ,/**(NUM_OF_MULTI_ENET_TX_CHANNELS+1+1) + (NUM_OF_MULTI_ENET_RX_CHANNELS+1+1),*/
         /* adjunct_pool */    0
      };

   adjunct_buffer_pool = WP_PoolCreate(wpid, WP_pool_iwbuffer, &adjunct_buffer_config);
   WTI_TerminateOnError(adjunct_buffer_pool, "WP_PoolCreate()",__LINE__);

   qn_iw_config.adjunct_pool = adjunct_buffer_pool;

   iw_qnode = WP_QNodeCreate(WP_WINPATH(DEFAULT_WPID), WP_QNODE_IWQ | WP_QNODE_OPT_FMU, &qn_iw_config);
   printf("WP_QNodeCreate iw_qnode 0x%08x\n",iw_qnode);
   WTI_TerminateOnError(iw_qnode, "WP_QNodeCreate()",__LINE__);


   pool_host = adjunct_buffer_pool;
}




void    WPE_CreateBufferPools()
{
   WPE_CreateIWQnode();
}

void    WPE_CreateEnetPortDevice()
{
   WP_U32 ii;
   WP_port_enet enet_port_config =
      {
         /* pkt_limits */
         {
            /* max_tx_channels*/ 64,
            /* max_rx_channels*/ 64,
            /* pq_block_size*/   0,
            /* n_pq_blocks*/     0,
            /* emphy_devices*/   WP_UNUSED
         },
         /* flowmode */       WP_FLOWMODE_FAST,
         /* interface_mode */ 0, /*WP_ENET_RGMII_10_100,*/
         /* rx_iw_bkgnd */    0,
      };


   WP_device_enet enet_dev_config =
      {
         /* max_tx_channels */ 1,
         /* tx_maxsdu */       SDU_SIZE,
         /* operating_speed */ WP_UNUSED,       /* Valid for RMII, RGMII only*/
         /* mac_addr*/         {0xaa,0xaa,0xaa,0xaa,0xaa,0xaa}, /* MAC of this device*/
         /* tx_bit_rate*/      1000000000,/*50000000,*/
         /* loopbackmode */    WP_ENET_NORMAL, /*WP_ENET_LOOPBACK,WP_ENET_NORMAL,*/
         /* extended_params */ NULL             /* Must be NULL at device creation?*/
      };

   WP_U8 enet_ports[1] =
      {WTI_ENET_PORT1};


   for(ii=0;ii<WPE_NUM_ENET_PORTS ; ii++)
   {


      if(enet_ports[ii] == WP_PORT_XGI1 || enet_ports[ii] == WP_PORT_XGI2)
      {
         enet_port_config.interface_mode = WTI_XGI_TYPE;
      }
      else
      {
         enet_port_config.interface_mode = WTI_WINNET_TYPE;
      }


      if(enet_ports[ii] ==  WTI_ENET_PORT1)
      {
         /* only port1 is controlled by user defines */
         WPE_EnetPhyInit(enet_ports[ii],WTI_ENET_MODE);
      }
      else
      {
         /* rest of ports are serdes loopbacked */
         WPE_EnetPhyInit(enet_ports[ii],WP_ENET_LOOPBACK);
      }

      port_enet[ii] = WP_PortCreate(wpid,enet_ports[ii] , &enet_port_config);
      WTI_TerminateOnError(port_enet[ii], "WP_PortCreate() ENET",__LINE__);

      dev_enet[ii] = WP_DeviceCreate(port_enet[ii], WP_PHY(0), WP_DEVICE_ENET, &enet_dev_config);
      WTI_TerminateOnError(dev_enet[ii], "WP_DeviceCreate() Enet",__LINE__);

   }

}

void    WPE_CreateHostPortDevice()
{

   port_host = WP_PortCreate(wpid, WP_PORT_IW_HOST, NULL);
   WTI_TerminateOnError(port_host, "WP_PortCreate() HOST",__LINE__);

   dev_host = WP_DeviceCreate(port_host, 0, WP_DEVICE_IW_HOST, NULL);
   WTI_TerminateOnError(dev_host, "WP_DeviceCreate() Host",__LINE__);
}

void WPE_EnetPhyInit(WP_U8 wti_enet_port,WP_U8 enet_mode)
{
   WP_status status;

   if(enet_mode == WP_ENET_NORMAL)
   {
      status = WPX_BoardSerdesInit(wpid, wti_enet_port, WPX_SERDES_NORMAL);
      WTI_TerminateOnError(status, "WPX_BoardSerdesInit wti_enet_port", __LINE__);
   }
   else
   {
      status = WPX_BoardSerdesInit(wpid, wti_enet_port, WPX_SERDES_LOOPBACK);
      WTI_TerminateOnError(status, "WPX_BoardSerdesInit wti_enet_port", __LINE__);
   }
}


void  WPE_PTPModuleInitWP3()
{
   WP_ptp_system    ptp_system_params = {0};
   WP_status status = WP_OK;

   ptp_system_params.max_ptp_ports = WPE_MAX_PTP_PORTS ;

   /* WDDI */
   status = WP_ModuleInit(WP_SYSHANDLE(wpid) ,WP_WDDI_MODULE_PTP, &ptp_system_params);
   WTI_TerminateOnError(status, "WP_ModuleInit(WP_WDDI_MODULE_PTP)",__LINE__);

   WPE_PTPTimestampInsertionEnable();
   /* CLOCK MUXING  (no need for now , working with PTP1/2 with system */

   /* get internal clock frequency */
   if(WPE_PTP_CLOCK_SOURCE == WP_PTP_SYSTEM_CLK)
   {
      clock_frequency = WPL_GetProcessorSpeed();
   }
   else
   {
      clock_frequency = WPE_PTP_CLOCK_FREQ;
   }


}

void  WPE_PTPTimestampInsertionEnable()
{

   WP_status status = WP_OK;
   WP_device_enet enet_dev = {0};
   WP_device_enet_ex ex_params ={0};
   WP_U32 ii;


/*Enable the timestamp feature*/

   enet_dev.extended_params = &ex_params;
   ex_params.rx_timestamp = WP_ENABLE;
   ex_params.tx_timestamp = WP_ENABLE;

   for(ii=0;ii<WPE_NUM_ENET_PORTS ; ii++)
   {
      if(ii==0)
      {
         ex_params.tx_timestamp_type = WP_ENET_IEEE_1588_80BIT_TS1;
         ex_params.rx_timestamp_type = WP_ENET_IEEE_1588_80BIT_TS1;
      }
      else
      {
         ex_params.tx_timestamp_type = WP_ENET_IEEE_1588_80BIT_TS2;
         ex_params.rx_timestamp_type = WP_ENET_IEEE_1588_80BIT_TS2;
      }

      status = WP_DeviceModify(dev_enet[ii],
                               WP_DEV_MOD_ENET_RX_TIMESTAMP | WP_DEV_MOD_ENET_TX_TIMESTAMP | WP_DEV_MOD_ENET_TIMESTAMP_TYPE,
                               &enet_dev);
      WTI_TerminateOnError(status, "WP_DeviceModify ", __LINE__);
   }

}

void    WPE_CreateHostIwRxChannel()
{

   /* This is TX from DPS point of view but RX from Host point of view*/
   WP_tx_binding tx_host_binding_config =
      {
         /* res0 */      0, /* Unused*/
         /* dci_mode */  WP_IW_DYN_CH_INSERT_DISABLE,    /* differs from 67000 */
         /* maxt */      MAXT_SIZE
      };

   WP_ch_iw_rx   iw_host_ch_config =
      {
         /* pqblock */           0,
         /* pqlevel */           0,
         /* tx_binding_type */   WP_IW_TX_BINDING,
         /* tx_binding_config */ &tx_host_binding_config
      };

   rx_host_channel = WP_ChannelCreate(0, dev_host, iw_qnode, WP_CH_RX, WP_IW_HOST, &iw_host_ch_config);
   WTI_TerminateOnError(rx_host_channel, "WP_ChannelCreate RX Host channel",__LINE__);
}

void    WPE_CreateEnetRxTxChannel()
{
   WP_U32 ii;

   WP_ch_enet enet_ch_config =
      {
         /* intmode */         WP_PKTCH_INT_DISABLE,
         /* iwmmode */         WP_PKTCH_IWM_ENABLE,
         /* testmode */        WP_PKTCH_TEST_DISABLE,
         /* tx_pqblock */      0,
         /* tx_pqlevel */      0,
         /* tx_shaping_type */ WP_FMU_SHAPING_TYPE_STRICT,
         /* tx_shaping_params*/NULL,
         /* rx_maxsdu */       SDU_SIZE,
         /* tx_cwid */         WP_CW_ID_A,
         /* tx_tq */           0,
         /* rx_queuedepth */   10,
      };


   for(ii=0;ii<WPE_NUM_ENET_PORTS ; ii++)
   {
      WP_U8  tag = 0x11 + ii*0x10 ;


      rx_enet_channel[ii]= WP_ChannelCreate(tag, dev_enet[ii],iw_qnode,WP_CH_RX,WP_ENET,&enet_ch_config);
      WTI_TerminateOnError(rx_enet_channel[ii], "WP_Channel_Create() Gige RX",__LINE__);

      tag+=1;


      tx_enet_channel[ii]= WP_ChannelCreate(tag, dev_enet[ii],iw_qnode,WP_CH_TX,WP_ENET,&enet_ch_config);
      WTI_TerminateOnError(tx_enet_channel[ii], "WP_Channel_Create() Gige TX",__LINE__);


   }


}


void WPE_CreatePTPPorts(void)
{

   ptp_master_1 =  WPE_CreatePTPPort(WP_PTP_MASTER,
                                     tx_enet_channel[0],
                                     rx_enet_channel[0],
                                     rx_host_channel,
                                     rx_host_channel,
                                     NULL,
                                     ptp_cds[0],
                                     WPE_PTP_STATS_ENABLE_DISABLE);

   WTI_TerminateOnError(ptp_master_1, "WPE_CreatePTPPort 1 ",__LINE__);

}


void    WPE_CreateHostTermFlowAgg()
{
   WP_iw_agg_hq flooding1_tx =
      {

         /*tag;*/ 1,
         /*txfunc;*/0,
         /*hq_agg_mode;*/WP_IW_HQ_NOT_RESERVED,
         /*mtu;*/1536,
         /*interruptqueue;*/0,
         /*intmode;*/WP_IW_INT_ENABLE,
         /*statmode;*/WP_IW_STAT_ENABLE,
         /*timestamp_mode;*/WP_IW_TIME_STAMP_DISABLE,
         /*ov_pool_mode;*/WP_IW_OV_POOL_DISABLE,
         /*fbp_drop_threshold;*/0,
         /*tx_direction;*/WP_IW_NETWORK_GLOBAL,
         /*flow_analyze;*/WP_IW_FLOW_PASS,
         /*force_analyze;*/WP_IW_FORCE_NO,
         /*l4_checksum_recal;*/WP_IW_L4_NO_CHECKSUM,
         /*nexthop;*/0,
         /*flow_agg_type;*/WP_IW_AGG_PRIMARY

      };


   flooding1_tx.tag++;
   flooding1_tx.txfunc =  ptp_master_1 ;
   ptp_master_flow_agg_1 = WP_IwFlowAggregationCreate(wpid, WP_IW_HOST_QUEUE_MODE,&flooding1_tx);
   WTI_TerminateOnError(ptp_master_flow_agg_1, "WPE_CreateHostTermIwFlowAgg() Host",__LINE__);



}


void WPE_SEND_PTP_PACKET(WP_handle ptp_port_handle , WP_U8 cd_id ,  WP_U8  ptp_pktid ,WP_U8 loopback_enable)
{
 WP_status status;
 WP_CHAR *ptp_packet_ipv4[SIZE_OF_ptp_ipv4];
 WP_CHAR ascii_buffer[DATA_LENGTH_PTP_IPV4*2+1];
 WP_CHAR hexa_buffer[DATA_LENGTH_PTP_IPV4*2];
 WP_U32 ii;
 WP_data_unit     data_unit;
 WP_data_segment  data_segment;
 WP_U8           *data_ptr = NULL;
 WP_U8  in_packet_offset = WPE_TX_TS_OFFSET ; /* ETH 14 + IP 20 + UDP 8 + 34 PTP header */



 memset(&data_unit,0, sizeof(WP_data_unit));
 memset(&data_segment,  0, sizeof(WP_data_segment));

/* prepare the data ptr */
 data_ptr = WP_PoolAlloc(pool_host);
 if (data_ptr == 0) {
    printf("WPE_SendHostData: host pool is empty\t");
    return;
 }
 /* prepare the data segment */

 data_segment.data_size = DATA_LENGTH_PTP_IPV4 ;
 data_segment.next = NULL;
 data_segment.displacement = 0;
 data_segment.data = data_ptr;
 data_segment.pool_handle = pool_host;

/* prepare the data unit */
 data_unit.data_size =  data_segment.data_size ;
 data_unit.n_active = 1;
 data_unit.n_segments = 1;
 data_unit.control = 0;
 data_unit.segment = &data_segment;
 data_unit.type= WP_DATA_ENET;

// // modify for PTP
#if PTP_ENABLE
 WPE_PtpDataUnitModify(wpid,
                      &data_unit,
                       ptp_pktid,                            /* packet_id also in bd  , */
                       WPE_PTP_CHECKSUM_ENABLE_DISABLE,
                       loopback_enable,
                       in_packet_offset);
#endif

 /* prepare the packet (chars)*/
 WPE_PtpPacketIPV4Default(ptp_packet_ipv4);


 /* convert packet to data */
 ii = 0 ;
 strcpy(ascii_buffer,ptp_packet_ipv4[ii]);

 for(ii = 1 ; ii < SIZE_OF_ptp_ipv4 ; ii++)
 {
    strcat(ascii_buffer,ptp_packet_ipv4[ii]) ;
 }

 App_TranslateAsciiToHex(hexa_buffer,ascii_buffer,DATA_LENGTH_PTP_IPV4);

 WP_MEM_BYTES_FILL(data_ptr, 0 , HOST_DATA_BUFFER_SIZE);
 WP_MEM_BYTES_SET(data_ptr,hexa_buffer,DATA_LENGTH_PTP_IPV4);



/* set different cd id */

 data_ptr[46] = cd_id;



/* update checksum */

 WP_U16 Checksum;
 WP_U16 len_udp = 8+44 ; /* without the extra header */
 Checksum =  udp_sum_calc(data_ptr,len_udp);

 data_ptr[40] = (WP_U8)((Checksum&0xff00)>>8);
 data_ptr[41] = (WP_U8)(Checksum&0x00ff);

#if WPE_PACKETS_PRINT
   printf("\nPacket sent :\n");

   for( ii=0; ii < data_unit.segment->data_size; ii++)
   {
      if((ii)%4 == 0) printf("\n");
      printf("%2.2x", data_unit.segment->data[ii]);
   }
   printf("( %d bytes )\n", data_unit.segment->data_size);
#endif

   memcpy(&sent_buffer,data_ptr,DATA_LENGTH_PTP_IPV4);


   status = WP_HostSend(ptp_port_handle, &data_unit);


   WTI_TerminateOnError(status, "WP_HostSend()",__LINE__);

}


WP_status WPE_RECEIVE_PTP_PACKET(WP_U16 * sec_msb,WP_U32 * sec_lsb,WP_U32 * nsec,WP_U8 *temp_buffer)
{
    WP_data_unit     data_unit;
    WP_data_segment  data_segment,data_segment_mem;
    WP_handle        status;
    WP_iw_data_return date_returned;
    WP_ptp_data_return ptp_data;

    date_returned.exteneded_data = &ptp_data;
    data_unit.data_return = &date_returned;


    memset (&data_unit,0, sizeof(WP_data_unit));
    memset (&data_segment,  0, sizeof(WP_data_segment));

#if PTP_ENABLE
    data_unit.type       = WP_DATA_IW | WP_DATA_RETURN_ENABLE;   /* Type of this data unit.& data return */
#else
    data_unit.type       = WP_DATA_IW;
#endif

    data_unit.segment    = &data_segment;    /* Pointer to first segment.        */
    data_unit.n_segments = 1;           /* Number of available segments.    */
    data_unit.data_return = &date_returned;

    status = WP_HostReceive(rx_host_channel,&data_unit);
    if (status == WP_OK)
    {
       /* copy the data before release */
       memcpy(&data_segment_mem,&data_segment , sizeof(WP_data_segment));
       /* copy buffer to return */
       memcpy(temp_buffer,data_segment.data,data_unit.data_size);

       /* data segment's data needs to be returned to pool */
       WP_PoolFree(data_segment.pool_handle, data_segment.data);

    }
    else
    {
       if ((WP_ERROR(status) == WP_ERR_HST_NO_DATA_TO_GET))
       {
          // printf("ERROR WP_HostReceive -> WP_ERR_HST_NO_DATA_TO_GET\n");

          return status;
       }
       else WTI_TerminateOnError(status, "WP_HostReceive Error()",__LINE__);
    }


#if WPE_PACKETS_PRINT
    WP_U32 ii;
   printf("\nPacket receivd :\n");
   for( ii=0; ii < data_segment_mem.data_size; ii++)
   {
      if((ii)%4 == 0) printf("\n");
      printf("%2.2x", data_segment_mem.data[ii]);
   }
   printf("( %d bytes )\n", data_segment_mem.data_size);
#endif



    if(ptp_data.ptp_rx_type == WP_PTP_RX_INGRESS_PACKET)
    {
       WP_handle ptp_port_handle = ptp_data.ptp_port_handle;
       WP_CHAR *ptp_port_str = 0;

       if(ptp_port_handle == ptp_master_1) ptp_port_str = "ptp_master_1";
       else  ptp_port_str = "unknown";


       printf("\nRx-> %s           Ts:  %u %u.%09u \n",
              ptp_port_str,
              ptp_data.ptp_rx_ts_secs_msb,
              ptp_data.ptp_rx_ts_secs_lsb,
              ptp_data.ptp_rx_ts_nsecs);

       printf("PTP protocol offset = 0x%x\n\n", ptp_data.ptp_rx_protocol_offset);

       if (ptp_data.ptp_rx_protocol_offset != PTP_PROTOCOL_OFFSET)
         terminate( "PTP protocol offset is differ than expected",__LINE__);

       *sec_msb =  ptp_data.ptp_rx_ts_secs_msb;
       *sec_lsb =  ptp_data.ptp_rx_ts_secs_lsb;
       *nsec =  ptp_data.ptp_rx_ts_nsecs;


       return WP_PTP_RX_INGRESS_PACKET;  /* regular rx */
    }
    else if(ptp_data.ptp_rx_type == WP_PTP_RX_LOOPBACK_PACKET)
    {
       //     printf("\nLB-> handle: 0x%08x  Ts: ",ptp_data.ptp_port_handle);
       printf("\nLB-> ");

       printf("  %u %u.%09u \n",
              ptp_data.ptp_rx_ts_secs_msb,
              ptp_data.ptp_rx_ts_secs_lsb,
              ptp_data.ptp_rx_ts_nsecs);

       *sec_msb =  ptp_data.ptp_rx_ts_secs_msb;
       *sec_lsb =  ptp_data.ptp_rx_ts_secs_lsb;
       *nsec =  ptp_data.ptp_rx_ts_nsecs;


       return WP_PTP_RX_LOOPBACK_PACKET;  /* loopback rx */
    }

     printf("\n\n\n !!! data compare fine\n");

    return 0;
}





void    WPE_CreateIwBportHost()
{

   WP_bridge_port iwhost_bport_config =
      {
         /* tag */               BRIDGE_PORT_HOST_TAG,
         /* direct_mapping */    WP_IW_DIRECT_MAP_ENABLE, /*WP_IW_DIRECT_MAP_DISABLE, */
         /* flow_agg */          0,
         /* flooding_term_mode*/ WP_IW_HOST_TERM_MODE,
         /* learning_mode */     WP_IW_LEARNING_DISABLE,
         /* in_filter_mode */    WP_IW_INGRESS_FILTER_DISABLE,
         /* vlan_param */
         {
            /* vlan_acceptance_mode */ WP_IW_ACCEPT_TAGGED_UNTAGGED,
            /* default_vlan_tag */     VLAN_TAG_1,
            /* vlan_tunnel;     */     WP_IW_VLAN_TUNNEL_1Q_IN_1Q_DISABLE,
            /* vlan_pri_enforce_mode*/ WP_IW_VLAN_PRIORITY_ENFORCE_DISABLED
         },
         /*max_mac_addresses */  2000,
         /*group_tag */          WP_IW_BRIDGEALL_GROUP_FILTER,
         /*group_filtering_mode*/WP_IW_GROUP_FILTER_DISABLE,
         /*unk_mac_sa_filter*/   WP_IW_UNK_MACSA_FILTER_DISABLE,
         /*unk_mc_mode;*/        WP_IW_UNK_MC_HT,
         /*bc_ht_mode;*/         WP_IW_BC_HT_DISABLE,
         /*input_filters_mask*/  0,
         /*output_filters_mask*/ 0,
         /*statmode*/            WP_IW_PORT_STAT_ENABLE,
         /*unk_uc_mode;*/        WP_IW_UNK_UC_SR_ENABLE,
         /*classification_flag*/ WP_IW_BPORT_CLASSIFICATION_ENABLED,
         /*adv_unk_lookup_mode*/ WP_IW_ADV_UNK_LOOKUP_DISABLED,
         /*cfi_ht_mode;*/        WP_IW_CFI_HT_DISABLE,
         /*reserved_mc_ht_mode*/ WP_IW_RES_MC_HT_DISABLE,
         /*predefined_ht_mode*/  WP_DISABLE,
         /*res_mc_action_table*/
      };


   iwhost_bport_config.flow_agg =  ptp_master_flow_agg_1 ;/*agg_host[WPE_PTP_PORT0];   default flow agg*/
   bport_host = WP_IwPortCreate(iw_sys, &iwhost_bport_config);
   WTI_TerminateOnError(bport_host," WPE_CreateIwBportHost ",__LINE__);

}
void terminate(WP_CHAR *s,WP_U32 line)
{
      printf(" %s  line %u\n", s,line);
      WP_DriverRelease();
      printf("Test Failed\n");
#if _WT_MODULE_TEST_ENABLE_
   WT_Reboot();
#endif
      exit(0);
 }
void App_TranslateAsciiToHex(WP_CHAR *Hex,WP_CHAR *Ascii,WP_U32 length)
{
    WP_U32 i,first=0,second=0;

   for (i=0;i<length;i++)
     {

          if ( Ascii[2*i] >= '0' &&  Ascii[2*i] <= '9')
            first =  Ascii[2*i] - '0';

          if ( Ascii[2*i] >= 'a' &&  Ascii[2*i] <= 'f')
             first =  Ascii[2*i] - 'a' + 10;

          if ( Ascii[2*i] >= 'A' &&  Ascii[2*i] <= 'F')
             first =  Ascii[2*i] - 'A' + 10;

          if ( Ascii[2*i+1] >= '0' &&  Ascii[2*i+1] <= '9')
            second =  Ascii[2*i+1] - '0';
          if ( Ascii[2*i+1] >= 'a' &&  Ascii[2*i+1] <= 'f')
            second =  Ascii[2*i+1] - 'a' + 10;
          if ( Ascii[2*i+1] >= 'A' &&  Ascii[2*i+1] <= 'F')
            second =  Ascii[2*i+1] - 'A' + 10;

          Hex[i]=(WP_U8 )(first * 16 + second);
     }
    return;
}

void    WPE_CreateIwBportEnet()
{
   WP_U32 ii;
   WP_bridge_port enet_bport_config =
      {
         /* tag */               BRIDGE_PORT_ENET_TAG,
         /* direct_mapping */    WP_IW_DIRECT_MAP_DISABLE,
         /* flow_agg */          0,
         /* flooding_term_mode*/ WP_IW_HOST_TERM_MODE,
         /* learning_mode */     WP_IW_LEARNING_DISABLE,
         /* in_filter_mode */    WP_IW_INGRESS_FILTER_DISABLE,
         /* vlan_param */
         {
            /* vlan_acceptance_mode */ WP_IW_ACCEPT_TAGGED_UNTAGGED,
            /* default_vlan_tag */     VLAN_TAG_1,
            /* vlan_tunnel;     */     WP_IW_VLAN_TUNNEL_1Q_IN_1Q_DISABLE,
            /* vlan_pri_enforce_mode*/ WP_IW_VLAN_PRIORITY_ENFORCE_DISABLED
         },
         /*max_mac_addresses */  2000,
         /*group_tag */          WP_IW_BRIDGEALL_GROUP_FILTER,
         /*group_filtering_mode*/WP_IW_GROUP_FILTER_DISABLE,
         /*unk_mac_sa_filter*/   WP_IW_UNK_MACSA_FILTER_DISABLE,
         /*unk_mc_mode;*/        WP_IW_UNK_MC_HT,
         /*bc_ht_mode;*/         WP_IW_BC_HT_DISABLE,
         /*input_filters_mask*/  0xffffffff,
         /*output_filters_mask*/ 0,
         /*statmode*/            WP_IW_PORT_STAT_ENABLE,
         /*unk_uc_mode;*/        WP_IW_UNK_UC_SR_ENABLE,
         /*classification_flag*/ WP_IW_BPORT_CLASSIFICATION_ENABLED,
         /*adv_unk_lookup_mode*/ WP_IW_ADV_UNK_LOOKUP_DISABLED,
         /*cfi_ht_mode;*/        WP_IW_CFI_HT_DISABLE,
         /*reserved_mc_ht_mode*/ WP_IW_RES_MC_HT_DISABLE,
         /*predefined_ht_mode*/  WP_DISABLE,
         /*res_mc_action_table*/
      };

   enet_bport_config.flow_agg =  ptp_master_flow_agg_1 ;/*agg_host[WPE_PTP_PORT0];  default flow agg should be agg to ho*/

   for(ii=0;ii<WPE_NUM_ENET_PORTS ; ii++)
   {
      enet_bport_config.tag++;

      bport_enet[ii] = WP_IwPortCreate(iw_sys, &enet_bport_config);
      WTI_TerminateOnError(bport_enet[ii]," WPE_CreateIwBportEnet ",__LINE__);
   }



}


void    WPE_CreateEnetRxTxBinding()
{
   WP_handle status;
   WP_U32 ii;

   WP_rx_binding_bridging  rx_binding_enet_config =
      {
         /*  encap_mode */   WP_IW_VCMUX,
         /*  mru;*/          MTU_SIZE,
         /*  vcfcs;*/        0,
         /*  input_port;*/   0
      };

   WP_tx_binding  tx_binding_enet_config =
      {
         /* res0 */      0, /* Unused*/
         /* dci_mode;*/  WP_IW_DYN_CH_INSERT_ENABLE,
         /* maxt;*/      MAXT_SIZE
      };

   /*   printf("WPI_HANDLE_RX_CHANNEL_P( rx_handle) %x , \n",WPI_HANDLE_RX_CHANNEL_P(rx_enet_channel) );*/
/*    printf("WPI_HANDLE_DEVICE_P %x , \n",WPI_HANDLE_DEVICE_P(rx_enet_channel) );*/


 for(ii=0;ii<WPE_NUM_ENET_PORTS ; ii++)
   {
   rx_binding_enet_config.input_port = bport_enet[ii];

   status = WP_IwRxBindingCreate(rx_enet_channel[ii], iw_sys, iw_qnode, &rx_binding_enet_config);
   WTI_TerminateOnError(status, "WPE_CreateIwSysEnetRxBinding() rx_binding_enet",__LINE__);

   status = WP_IwTxBindingCreate(tx_enet_channel[ii], WP_IW_TX_BINDING, &tx_binding_enet_config);
   WTI_TerminateOnError(status, "WP_IwTxBindingCreate 2",__LINE__);
   }



}

void WPE_AddBasicFilters(void)
{
   WP_classification_filter filter_param;

   memset(&filter_param, 0, sizeof(wp_classification_filter));

   /* EMC Filter*/
   filter_param.filter_type     = WP_CLASS_FILTER_EMC;
   filter_param.no_match_action = WP_CLASS_FILTER_NO_MATCH_CONTINUE;/*WP_CLASS_FILTER_NO_MATCH_DENY;*/
   filter_param.fields_mask     = WP_FIELD_IPV4_PROTOCOL | WP_FIELD_DST_PORT; //  |
      //  WP_FIELD_USER_PROGRAMMABLE | WP_FIELD_IN_PORT ;

/* for uppf */
//    filter_param.user_fields_config.num_of_user_fields = 1 ;
//    filter_param.user_fields_config.ref_point = WP_CLASS_REF_POINT_LAYER_4 ;
//    filter_param.user_fields_config.user_fields[0].field_offset = 8 + 4 ; /*bytes: UDP + 4 in ptp header*/
//    filter_param.user_fields_config.user_fields[0].field_size = 1 ; /* 1 byte*/


   emc_filter_handle[0] = WP_IwClassFilterAdd(iw_sys, &filter_param);
   WTI_TerminateOnError(emc_filter_handle[0], "WP_IwClassFilterAdd",__LINE__);


}



void WPE_AddBasicRules(void)
{

   WP_flow_class_rule class_cfg;
   WP_CHAR            dfc_rule[WP_IW_RULE_MAX_LENGTH];
   WP_dfc_flow_info   dfc_flow_info;


   /************************************************************************************************************/



   memset(&class_cfg,     0, sizeof(WP_flow_class_rule));
   memset(&dfc_flow_info, 0, sizeof(WP_dfc_flow_info));
   memset(&dfc_rule,      0, WP_IW_RULE_MAX_LENGTH);

 /* RULE 1 CD id 0 */

   /*sprintf(dfc_rule, "192.168/0.0;0.0/0.0;50;48;1.0/0.0;0.0/0.0;2000;UDP;");*/
   sprintf(dfc_rule,"n;n;n;n;n;n;319;17;");  /* clock domain id == 0 !!!!!!!!!!*/
   dfc_flow_info.filter_handle = emc_filter_handle[0];
   dfc_flow_info.output_port   = 0;
   dfc_flow_info.input_port    = bport_enet[0];  /* ptp_master_1 */

   class_cfg.classifier_string = &dfc_rule[0];
   class_cfg.dfc_info          = &dfc_flow_info;
   class_cfg.match_action      = WP_FLOW_CLASS_MODE_FLOW_AGG;
   class_cfg.action_info.forwarding_action.flow_aggregation =  ptp_master_flow_agg_1 ;
   class_cfg.wred_entry        = 0;


   flow_rule_handle[0][0] = WP_IwFlowAdd(iw_sys, WP_FLOW_CLASS_RULE, &class_cfg);
   WTI_TerminateOnError(flow_rule_handle[0][0], "WP_IwFlowAdd - Basic Rules",__LINE__);



}

static void WPE_RUN_PKTS()
{
   WP_U32  status , ii;
   WP_U16 sec_msb1=0 ,sec_msb2=0;
   WP_U32 sec_lsb1=0,sec_lsb2=0;
   WP_U32 nsec1=0, nsec2=0;
   WP_U8 temp_buffer[2000];
   WP_U8 loopback = WPE_PTP_LOOPBACK_ENABLE_DISABLE;
   WP_U32 allowed_ts_diff = WTI_ALLOWED_TS_DIFF;


   /************* CLOCK DOMAIN 0  *************/

   for(ii=0 ; ii<NUM_OF_PACKETS ; ii++)
   {
      printf("\n****Sending Packet [%u] with ptp_master_1 \n",pkt_id);

#if PTP_ENABLE


      WPE_SEND_PTP_PACKET(ptp_master_1,WPE_CD0,pkt_id++,loopback);
#else
      printf("\n****Sending non ptp packet \n");
      WPE_SEND_PTP_PACKET(tx_enet_channel[0],WPE_CD0,pkt_id++,loopback);
#endif
      WP_Delay(100000);
   }



   for(ii=0 ; ii<NUM_OF_PACKETS ; ii++)
   {

#if PTP_ENABLE

      if(loopback)
      {
         status = WPE_RECEIVE_PTP_PACKET(&sec_msb2,&sec_lsb2,&nsec2,temp_buffer);
         if(status != WP_PTP_RX_LOOPBACK_PACKET)
         {
            WTE_DisplayStats();
         terminate("Expecting Tx loopback",__LINE__);
         }
      }

      WP_Delay(10000);

      status = WPE_RECEIVE_PTP_PACKET(&sec_msb1,&sec_lsb1,&nsec1,temp_buffer);
      if(status != WP_PTP_RX_INGRESS_PACKET)
      {
         WTE_DisplayStats();
         terminate("Expecting Rx ingress",__LINE__);
      }


      WP_Delay(10000);

      /* Check Rx ts ~ Tx loopback */
      if(loopback)
      {
         if(sec_msb1 != sec_msb2 || sec_lsb2 != sec_lsb1 || nsec1 - nsec2 > allowed_ts_diff )
         {
            printf("%u %u.%09u  != %u %u.%09u \n",sec_msb1,sec_lsb1,nsec1,sec_msb2,sec_lsb2,nsec2 );
            terminate("Rx & Tx loopback mismatch (check if serdes internal loopback)",__LINE__);
         }




         //  Check Tx ts == Lb ts
         memcpy(&sec_msb1,&temp_buffer[WPE_TX_TS_OFFSET],2);
         memcpy(&sec_lsb1,&temp_buffer[WPE_TX_TS_OFFSET + 2],4);
         memcpy(&nsec1,&temp_buffer[WPE_TX_TS_OFFSET + 6],4);


         if(sec_msb1 != sec_msb2 || sec_lsb2 != sec_lsb1 || nsec2 - nsec1 > allowed_ts_diff )
         {
            printf("%u %u.%09u  != %u %u.%09u \n",sec_msb1,sec_lsb1,nsec1,sec_msb2,sec_lsb2,nsec2 );
            terminate("Tx & Tx loopback mismatch",__LINE__);
         }
      }

#else  /* ptp enable */
      status = WPE_RECEIVE_PTP_PACKET(&sec_msb1,&sec_lsb1,&nsec1,temp_buffer);
#endif  /* ptp enable */

   }


}

void  WTE_DisplayStats(void)
{

#if WTE_STATS
   printf("\n\nMaster 1 stats:\n");
   WPE_HostTermFlowAggStatistics(ptp_master_flow_agg_1);
   WPE_IwPortStatistics(bport_enet[0]);
   WPE_PtpPortStatistics(ptp_master_1);

#endif

}


void WPE_HostTermFlowAggStatistics(WP_handle flow_agg_handle)
{
   WP_iw_flow_stats host_term_stats = {0};
   WP_status status;
   WP_U32  temp1, temp2, temp3, temp4;


   /* event  */
   status = WP_IwFlowStatistics(flow_agg_handle,
                                WP_IW_FLOW_STAT,
                                &host_term_stats);

   if (status != WP_OK) return;


   printf("\n");
   printf("HOST TERMINATION FLOW AGGREGATION ( FLOW AGGREGATION HANDLE %x ):\n",
          flow_agg_handle);
   printf("--------------------------------------------------------------\n");
   temp1 = host_term_stats.forward_packet;
   temp2 = host_term_stats.forward_packet >> 32;
   temp3 = host_term_stats.fbp_drop_packets;
   temp4 = host_term_stats.fbp_drop_packets >> 32;
   printf("forward_packet:         %.8x%.8x\nfbp_drop_packets:       %.8x%.8x\n", temp2, temp1, temp4, temp3);
   temp1 = host_term_stats.mtu_drop_packets;
   temp2 = host_term_stats.mtu_drop_packets >> 32;
   temp3 = host_term_stats.ttl_drop_packets;
   temp4 = host_term_stats.ttl_drop_packets >> 32;
   printf("mtu_drop_packets:       %.8x%.8x\nttl_drop_packets:       %.8x%.8x\n", temp2, temp1, temp4, temp3);
   temp1 = host_term_stats.tx_queue_drop_packets;
   temp2 = host_term_stats.tx_queue_drop_packets >> 32;
   temp3 = host_term_stats.mpls_drop;
   temp4 = host_term_stats.mpls_drop >> 32;
   printf("tx_queue_drop_packets:  %.8x%.8x\nmpls_drop:              %.8x%.8x\n", temp2, temp1, temp4, temp3);
   temp1 = host_term_stats.denied_packets;
   temp2 = host_term_stats.denied_packets >> 32;
   temp3 = host_term_stats.group_filtered_packets;
   temp4 = host_term_stats.group_filtered_packets >> 32;
   printf("denied_packets:         %.8x%.8x\ngroup_filtered_packets: %.8x%.8x\n", temp2, temp1, temp4, temp3);
   temp1 = host_term_stats.gtp_bad_headers;
   temp2 = host_term_stats.gtp_bad_headers >> 32;
   printf("gtp_bad_headers:        %.8x%.8x\n", temp2,temp1);
}


void WPE_IwPortStatistics(WP_handle iw_port)
{
   WP_bport_stats bport_stats;
   WP_status status;
   WP_U32 i;
   i=0;


   memset( &bport_stats, 0, sizeof(WP_bport_stats) );

   status = WP_IwPortStatistics(iw_port, &bport_stats);

   if (status)
   {
      printf("GetIWStatistics error\n");
      return;
   }
   printf("\n");
   printf("ETHERNET IW BRIDGE PORT STATISTICS\n");
   printf("--------------------------------------------------------------\n");
   printf ("Rx Valid Packets:               %08x%08x\n",
           ((WTI_StatField)bport_stats.rx_valid_packets).part.high,
           ((WTI_StatField)bport_stats.rx_valid_packets).part.low);


   printf ("Discard VLAN acceptable filter: %08x%08x\n",
           ((WTI_StatField)bport_stats.discard_vlan_acceptable_filter).part.high,
           ((WTI_StatField)bport_stats.discard_vlan_acceptable_filter).part.low);

   printf ("Discard ingress filter:         %08x%08x\n",
           ((WTI_StatField)bport_stats.discard_ingress_filter).part.high,
           ((WTI_StatField)bport_stats.discard_ingress_filter).part.low);

   printf ("Discard bridge classifier:      %08x%08x\n",
           ((WTI_StatField)bport_stats.discard_bridge_classifier).part.high,
           ((WTI_StatField)bport_stats.discard_bridge_classifier).part.low);


   printf ("Discard unknown MAC SA:         %08x%08x\n",
           ((WTI_StatField)bport_stats.discard_unk_macsa).part.high,
           ((WTI_StatField)bport_stats.discard_unk_macsa).part.low);

   printf ("Deny MAC SA:                    %08x%08x\n",
           ((WTI_StatField)bport_stats.deny_mac_sa).part.high,
           ((WTI_StatField)bport_stats.deny_mac_sa).part.low);

   printf ("Deny MAC DA:                    %08x%08x\n",
           ((WTI_StatField)bport_stats.deny_mac_da).part.high,
           ((WTI_StatField)bport_stats.deny_mac_da).part.low);

   printf ("Rx BC valid packets:            %08x%08x\n",
           ((WTI_StatField)bport_stats.rx_bc_valid_packets).part.high,
           ((WTI_StatField)bport_stats.rx_bc_valid_packets).part.low);

   printf ("Rx MC valid packets:            %08x%08x\n",
           ((WTI_StatField)bport_stats.rx_mc_valid_packets).part.high,
           ((WTI_StatField)bport_stats.rx_mc_valid_packets).part.low);

   printf ("Forwarded UC packets:           %08x%08x\n",
           ((WTI_StatField)bport_stats.forwarded_uc_packets).part.high,
           ((WTI_StatField)bport_stats.forwarded_uc_packets).part.low);

   printf ("Forwarded BC packets:           %08x%08x\n",
           ((WTI_StatField)bport_stats.forwarded_bc_packets).part.high,
           ((WTI_StatField)bport_stats.forwarded_bc_packets).part.low);

   printf ("Forwarded MC packets:           %08x%08x\n",
           ((WTI_StatField)bport_stats.forwarded_mc_packets).part.high,
           ((WTI_StatField)bport_stats.forwarded_mc_packets).part.low);

   printf ("Protocol_Error :                %08x%08x\n",
           ((WTI_StatField)bport_stats.protocol_error).part.high,
           ((WTI_StatField)bport_stats.protocol_error).part.low);
#if 0

   for (i=0;i<32;i++)
   {
      printf ("DFC Filter %d Discard:           %08x%08x\n",i,
              ((WTI_StatField)bport_stats.discard_dfc_filter[i]).part.high,
              ((WTI_StatField)bport_stats.discard_dfc_filter[i]).part.low);
   }

#endif  /* 0  */

}




void WPE_PtpPortStatistics(WP_handle ptp_port)
{
   WP_ptp_port_stats ptp_port_stats;
   WP_status status;
   WP_U32 i;
   i=0;


   memset( &ptp_port_stats, 0, sizeof(WP_ptp_port_stats) );

   status = WP_PtpStatistics(ptp_port, &ptp_port_stats);

   if (status)
   {
      printf("GetPTPStatistics error\n");
      return;
   }
   printf("\n");
   printf("PTP PORT STATISTICS\n");
   printf("--------------------------------------------------------------\n");
   printf ("Tx Valid TS Packets:               %08x%08x\n",
           ((WTI_StatField)ptp_port_stats.tx_ts_good_packets).part.high,
           ((WTI_StatField)ptp_port_stats.tx_ts_good_packets).part.low);

   printf ("Rx HT Valid TS Packets:            %08x%08x\n",
           ((WTI_StatField)ptp_port_stats.rx_ht_ts_good_packets).part.high,
           ((WTI_StatField)ptp_port_stats.rx_ht_ts_good_packets).part.low);

   printf ("LB Valid TS Packets:               %08x%08x\n",
           ((WTI_StatField)ptp_port_stats.lb_ts_good_packets).part.high,
           ((WTI_StatField)ptp_port_stats.lb_ts_good_packets).part.low);

   printf ("LB only Valid TS Packets:          %08x%08x\n",
           ((WTI_StatField)ptp_port_stats.lb_only_ts_good_packets).part.high,
           ((WTI_StatField)ptp_port_stats.lb_only_ts_good_packets).part.low);

   printf ("Host Fifo Full ind:                %08x%08x\n",
           ((WTI_StatField)ptp_port_stats.host_fifo_full).part.high,
           ((WTI_StatField)ptp_port_stats.host_fifo_full).part.low);

   printf ("FBP underrun:                      %08x%08x\n",
           ((WTI_StatField)ptp_port_stats.lb_bd_list_overrun).part.high,
           ((WTI_StatField)ptp_port_stats.lb_bd_list_overrun).part.low);

   printf ("LB BD list overrun:                %08x%08x\n",
           ((WTI_StatField)ptp_port_stats.lb_bd_list_overrun).part.high,
           ((WTI_StatField)ptp_port_stats.lb_bd_list_overrun).part.low);

   printf ("LB invalid entry:                  %08x%08x\n",
           ((WTI_StatField)ptp_port_stats.lb_invalid_entry).part.high,
           ((WTI_StatField)ptp_port_stats.lb_invalid_entry).part.low);

}

void    WPE_EnableSystem()
{
   WP_handle status;
   WP_U32    ii;



   for(ii=0;ii<WPE_NUM_ENET_PORTS ; ii++)
   {

   /* Enable the Enet Ports */
   status = WP_PortEnable(port_enet[ii], WP_DIRECTION_DUPLEX);
   WTI_TerminateOnError(status, "WP_PortEnable Enet",__LINE__);


   /* Enable the Enet Device */
   status = WP_DeviceEnable(dev_enet[ii], WP_DIRECTION_DUPLEX);
   WTI_TerminateOnError(status, "WP_DeviceEnable Enet",__LINE__);


 /* Enable the Enet Channels */
   status = WP_ChannelEnable(rx_enet_channel[ii]);
   WTI_TerminateOnError(status, "WP_ChannelEnable() rx_enet",__LINE__);

   status = WP_ChannelEnable(tx_enet_channel[ii]);
   WTI_TerminateOnError(status, "WP_ChannelEnable() tx_enet",__LINE__);

   }


}

#endif

static void WTI_BoardConfigure(void)
{
   WP_status status = WP_OK;
#if !defined(CHECKIN_TEST)
#ifdef WP_HW_WINPATH3
   WPX_pll_cfg pllConfigParam = {0};
#endif
#endif


   printf("Differential refference clock set to %d Hz\n",global_clock_rec_diff_ref_clock);
   

#if WTI_CESOP_TDI
   /* when working with the upper 8 TDM ports, must call BoardConfigure */

#ifdef WP_HW_WINPATH3
   status = WPX_BoardConfigure(the_system->wpid, WPX_CONFIGURE_CHECKIN_TDI_16);
   WTI_TerminateOnError(status, "WPX_BoardConfigure()", __LINE__);

#else
   if (WTI_ACTIVE_TDI_PORTS > 8)
   {
      status = WPX_BoardConfigure(the_system->wpid, WPX_CONFIGURE_CHECKIN_TDI_16);
      WTI_TerminateOnError(status, "WPX_BoardConfigure()", __LINE__);

   }
#endif
#else  /* not TDI */
#ifdef WP_HW_WINPATH3
#if (WTI_CESOP_CLOCK_RECOVERY_ENABLE && WTI_CESOP_RX_TIMING_ENABLE &&(!WTI_CAD_0_3_SETUP))
#ifdef __WT_UFE3__
   /* In CR mode the EMPHY clock must be bigger then 75MHz */
   status = WPX_BoardConfigure(the_system->wpid, WPX_CONFIGURE_2UPI_1XGI_10SGMII_UPI_CLK_100M);
   WTI_TerminateOnError(status, "WPX_BoardConfigure()", __LINE__);
#else /* UFE4 */
#if WTI_UFE_4_1_2
   if (WTI_EMPHY_PORT == WP_PORT_UPI3)
   {
      printf("WPX_CONFIGURE_UPI3_UFE412************\n\n\n");
      status = WPX_BoardConfigure(the_system->wpid, WPX_CONFIGURE_UPI3_UFE412);
      WTI_TerminateOnError(status, "WPX_BoardConfigure()", __LINE__);
   }
   else
   {
      printf("WPX_CONFIGURE_2UPI_1XGI_10SGMII_UFE4************\n\n\n");
      status = WPX_BoardConfigure(the_system->wpid, WPX_CONFIGURE_2UPI_1XGI_10SGMII_UFE4);
      WTI_TerminateOnError(status, "WPX_BoardConfigure()", __LINE__);
   }
#else
   printf("WPX_CONFIGURE_2UPI_2XGI_6SGMII_UFE448************\n\n\n");
   status = WPX_BoardConfigure(the_system->wpid, WPX_CONFIGURE_2UPI_2XGI_6SGMII_UFE448);
   WTI_TerminateOnError(status, "WPX_BoardConfigure()", __LINE__);
#endif /*  WTI_UFE_4_1_2*/
#endif /* WT_UFE3 */
#else  /* without CR */
#ifdef __WT_UFE3__
#if (!WTI_CAD_0_3_SETUP)
   status = WPX_BoardConfigure(the_system->wpid, WPX_CONFIGURE_2UPI_1XGI_10SGMII);
#else
   /* In case of CAD on WDS3 (board dependent)  the pinmux must be set to High-Z on UPI16-31
      to avoid contentions withy clocks going from UFE to COMET  */
   status = WPX_BoardConfigure(the_system->wpid, WPX_CONFIGURE_2UPI_16bit_1XGI_10SGMII);
#endif

   WTI_TerminateOnError(status, "WPX_BoardConfigure()", __LINE__);
#else /*UFE4 config for 16/32bit*/
#if WTI_UFE_4_1_2
#if WTI_DUAL_EMPHY
   {
      /* This PINMUX is not good for dual EMPHY but it configures correctly the GPIO pins
         associated with the HOST programming of UPI1 (UFE412 REVB), we replace later the
         PINMUX after programming the UFE412 on UPI1 */
      printf("WPX_CONFIGURE_2UPI_1XGI_10SGMII_UFE4************\n\n\n");
      status = WPX_BoardConfigure(the_system->wpid, WPX_CONFIGURE_2UPI_1XGI_10SGMII_UFE4);
      WTI_TerminateOnError(status, "WPX_BoardConfigure()", __LINE__);
   }
#else
   if (WTI_EMPHY_PORT == WP_PORT_UPI3)
   {
      printf("WPX_CONFIGURE_UPI3_UFE412************\n\n\n");

      status = WPX_BoardConfigure(the_system->wpid, WPX_CONFIGURE_UPI3_UFE412);
      WTI_TerminateOnError(status, "WPX_BoardConfigure()", __LINE__);
   }
   else
   {
      printf("WPX_CONFIGURE_2UPI_1XGI_10SGMII_UFE4************\n\n\n");

      status = WPX_BoardConfigure(the_system->wpid, WPX_CONFIGURE_2UPI_1XGI_10SGMII_UFE4);
      WTI_TerminateOnError(status, "WPX_BoardConfigure()", __LINE__);
   }
#endif
#else
   printf("WPX_CONFIGURE_2UPI_2XGI_6SGMII_UFE448************\n\n\n");

   status = WPX_BoardConfigure(the_system->wpid,WPX_CONFIGURE_2UPI_2XGI_6SGMII_UFE448);
   WTI_TerminateOnError(status, "WPX_BoardConfigure()", __LINE__);

#endif /*  WTI_UFE_4_1_2*/
#endif /* WT_UFE3 */
#endif /* (WTI_CESOP_CLOCK_RECOVERY_ENABLE && WTI_CESOP_RX_TIMING_ENABLE && WTI_EXT_DIFF_MODE_ACTIVE) */
#endif /* WP_HW_WINPATH3 */
#endif /* WTI_CESOP_TDI */


#ifdef WP_HW_WINPATH3
#if !defined(CHECKIN_TEST)

#if defined __WT_UFE448__ || defined __WT_UFE412__
#if !WTI_CI_TESTING_MODE_UFE4
   WTI_ZarlinkPllSetup(&pllConfigParam);
#endif
#else
   pllConfigParam.clock_out = wpx_apll_clk0;
#endif

   /* Update the FPGA over the WDS3 with the clock muxing for PWE3-CR and 1588,
      required suitable version on the FPGA. */
   WTI_BoardPinConfig(WTI_DEFAULT_PIN_CONFIG, 1, pllConfigParam.clock_out);


   /* Set WDS3 PLL to extract 125MHz clock config */
   if ((the_system->system_features.sync_ethernet) && (!the_system->system_features.sync_ethernet_master))
   {
      /* SyncE slave */
      /* The PLL is configured to lock to the input from SyncE clock (normal mode) */
      status =  WPX_BoardZarlinkPllConfig(the_system->wpid, WPX_PLL_CONFIG_APLL_125_MANUAL_NORMAL);
   }
   else
   {
      /* The PLL is configured to work in free run mode */
      status =  WPX_BoardZarlinkPllConfig(the_system->wpid, WPX_PLL_CONFIG_APLL_125);
   }

   WTI_TerminateOnError(status, "WPX_BoardZarlinkPllConfig", __LINE__);

   /* SMA1 is set to APLL_CLK0 or P0_CLK0 - done in WTI_BoardPinConfig
      Here we set the ouput frequency according to WT_CLOCK_REC_DIFF_REF_CLOCK to be used as refference clock
      In UFE3 SMA1 must be configured to 125 MHz (default setting) to support release synthesis */
#if defined __WT_UFE448__ || defined __WT_UFE412__
#if !WTI_CI_TESTING_MODE_UFE4
   status = WPX_ExternalPllClockOutConfig(&pllConfigParam);
   WTI_TerminateOnError(status, "WPX_ExternalPllClockOutConfig", __LINE__);
#endif
#endif

#endif // !defined(CHECKIN_TEST)
#endif /* WP_HW_WINPATH3 */

}
