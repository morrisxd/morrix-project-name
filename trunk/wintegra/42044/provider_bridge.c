/*****************************************************************************
 * (C) Copyright PMC - WIN division (Wintegra) 2012.  All rights reserved.
 * PMC-Sierra (WINTEGRA) CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of PMC-Sierra (Wintegra).
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

#include "provider_bridge.h"

APP_gbe_enet_t gbe[NR_GBE] = {{WP_PORT_ENET13, }, {WP_PORT_ENET14, },
                              {WP_PORT_ENET15, }, {WP_PORT_ENET16, },
                              {WP_PORT_ENET3, }, {WP_PORT_ENET5, },
                              {WP_PORT_ENET4, }, {WP_PORT_ENET6, },
                              {WP_PORT_ENET8, }, {WP_PORT_ENET7, }};

WP_handle devhost, default_agg_host, h_iw_port_general_host, rx_host_handle;
WP_handle xgi_port, xgi_dev, xgi_bport, xgi_rx_ch_handle, xgi_tx_ch_handle[NR_PRIORITY_CHANNELS], ul_flow_agg;
WP_handle qniw;
WP_handle qniw_mc;
WP_handle pecs_handles[NUM_OF_PECS];
WP_handle shaper_l1_handles[NR_L1_SHAPER_GROUP_ENTRIES];
WP_handle shaper_l2_handles[NR_L2_SHAPER_GROUP_ENTRIES];
WP_handle shaping_blocks[NR_L2_SHAPER_GROUP_ENTRIES];
WP_handle dl_general_iwsys_bridge;

WP_U32 aging_enable = 0;
WP_U32 aging_weight = 1;
WP_U32 aging_period = 60 * 5;  /* 5 Minutes */
WP_THREAD_ID aging_tid;

WP_U32 learning_enable = 0;
WP_U32 learnt_rules = 0;
WP_THREAD_ID learning_tid;

WP_U32 PMTest_enable = 0;
WP_U32 PMTest_vid = 0;
WP_U32 PMTest_delta = 0;

WP_pce_filter_stat filter_stat[100];
WP_handle PCE_filter[100];
WP_handle filter_set_lrn_en, filter_set_lrn_dis;
WP_handle PCE_rule_handle;

WP_pce_learned_forwarding_rule learned_rules_list[WP_PCE_LEARNING_QUEUE_MAX_SIZE];

WP_U32 max_mac_addresses = 100;
WP_U32 start_vid = 1;
WP_U32 bridge_lock;

WP_SEM_ID aging_sem;
WP_SEM_ID learning_sem;

WP_boolean demo_running = WP_TRUE;

WP_handle GPMT_handle;

WP_U8 priority_mapping_remarking_enabled = 0;
WP_U8 priority_mapping[NR_PRIORITY_CHANNELS] = {7, 6, 5, 4, 3, 2, 1, 0};
WP_handle PCE_rules_mapping[NR_PRIORITY_CHANNELS];

WP_int_queue_table int_queue_tables = {{{65500, 1}, {10, 1}, {10, 1}, {10, 1}}};
WP_pce_parser_info pce_parser_info;

WP_sw_fdb_info sw_fdb_info =
{
        WP_BUS_PARAM, /* sw_fdb_bus */ 
        0, /* fdb_threshold */
        (256*1024 - 1), /* max_fdb_rules */
};

WP_module_pce_init pce_init =
{
        &pce_parser_info, /* parser_info */
        32000, /* hardware_rules_num */
        100, /* filters_num */
        10000, /* pkt_rx_channel_interfaces_num */
        10000, /* iw_tx_channel_interfaces_num */
        WP_PCE_DISCARD_MODE_SMART_DISCARD, /* pce_discard_mode */
        WP_ENABLE, /* bridge_loop_prevention_mode */
        WP_ENABLE, /* learning_mode */
        {/* learning_queue_config */
                WP_PCE_LEARNING_QUEUE_MAX_SIZE, /* learning_queue_size */
#if LEARNING_MODE_INTERRUPT
                WP_LEARNING_INTENABLE, /* interrupt_mode */
#else
                WP_LEARNING_POLLING_MODE, /* interrupt_mode */
#endif
                WP_IW_IRQT0, /* int_queue_num */
                1, /* interrupt_rate */
                4, /* protection_table_size */
        },
        2, /* iw_next_round_pce_interfaces_num*/
        0, /* interfaces_on_atm_rx_mode */
        0,/* atm_rx_channel_interfaces_num */
        0,/* device_interfaces_num */
        WP_NO_ENHANCMENT/*WP_SW_FDB_ENABLE_MODE*/,/* enhanced_mode */
        &sw_fdb_info, /* sw_fdb_info  */
};

WP_gpe_pecs gpe_pecs_cfg[NUM_OF_PECS] = 
{
        {
                &brouter_pecs_pecs_image, /*pecs_image_info*/
                &WPE_GpeBrouterPecsService /*pecs_service_fp*/
        },
        {
                &mpls_pecs_pecs_image, /*pecs_image_info*/
                &WPE_GpeMplsPecsService /*pecs_service_fp*/
        }

};

#include "provider_bridge_util.c"
#include "provider_bridge_menu.c"

/****************************************************************************************************************************
 * Function name: App_TerminateOnError()
 * Description  :  This function check for any error status..if error recieved terminate the program
 *
 * Input  params: handle : handle status need to check for failure or success
 *			s : meesage carried in this variable will be displayed on terminal
 * Output params: none
 * Return val   : none
 *****************************************************************************************************************************/
void WPI_SimulateInterrupts(void);
void App_TerminateOnError(WP_handle handle, WP_CHAR *s, WP_U32 line)
{
        if (WP_ERROR_P(handle)) {
                printf("%s: Terminal error (handle 0x%x) %s\n", appname, handle, s);
                printf("%s: Error type - %s\n", appname, WP_ErrorString(handle));
                printf("line = %d\n",line);
                printf("Test Failed\n");

                WP_DriverRelease();

                WPE_Reboot();
                exit(0);
        }
}


/****************************************************************************************************************************
 * Function name: main()
 * Description  : Main function of the application.
 *
 * Input  params: none
 * Output params: 
 * Return val   : none
 *****************************************************************************************************************************/
int main(int argc, char *argv[])
{
        WP_status status;

        /* driver, system context, ram partition init */
        App_SysInit();

        /* hardware configuration */
        App_InitHW();

        /* create physical port and device */
        App_PortsDevicesCreate();

        /* create buffer pool */
        App_BufferPoolsCreate();

        /* PCE and GPE module init */
        App_ModulesInit();

        /* connect the interrupt events */
        App_EventsInit();

        /* commit the system configuration */
        status = WP_SysCommit();
        App_TerminateOnError(status, "WP_SysCommit",__LINE__);

        /* Create and enable shapping groups */
        App_ShapingGroupCreateAndEnable();
        
        /* Pecs init */
        WPE_Pecs_Init(gpe_pecs_cfg,pecs_handles);

        /* create channels for TX & RX */
        App_ChannelsCreate();

        /* Create GPMT */
        WPE_GPMTCreate();
        
        /* init the IW system */
        App_IwSystemInit();

        /* Enable the system */
        App_SystemEnable();

        status = WP_IwSystemBuild(dl_general_iwsys_bridge);
        App_TerminateOnError(status, "WP_IwSystemBuild()",__LINE__);

        /* PCE interface create */
        App_PceInterfaceCreate();

        /* semaphore and thread for learning */
        WPL_SemInit(&learning_sem);
        WPL_ThreadInit(&learning_tid, App_LearningThread, NULL);

        /* semaphore and thread for aging */
        WPL_SemInit(&aging_sem);
        WPL_ThreadInit(&aging_tid, App_AgingThread, NULL);

        /* CLI */
        WPL_LockKeyInit(WPL_THREAD_LOCK_KEY, &bridge_lock);

        if (argc <= 1)
        {
                /* Start CLI*/
                CLI_T_Main(NULL);
   
                WPL_Delay(10);
       
                WPL_LockDestroy(WPL_THREAD_LOCK_KEY, &bridge_lock);
   
                WP_DriverRelease();
       
                exit(0);
        }
        else if ((3 == argc) && (!strcmp (argv[1], "freerun")))
        {
                /* freerun mode */
                printf ("Starting freerun now ...\n");
                App_perform_freerun ();
        }
        else
        {
                printf("Wrong parameter! Only \"go\" or \"go freerun\" supported!");
                /*Quit test */
        }	

        return 0;
}


/*****************************************************************************
 * Function name: App_SysInit
 * Description  : Configures the context and Initializes the System with the 
 *                context 
 * Input  params: None
 * Output params: None
 * Return val   : None
 *****************************************************************************/
void App_SysInit(void)
{
        WP_status status;
        WP_iw_global  max_iw[1] = {
                {
                        /* max_iw_aggregations */ MAX_FLOW_AGG,
                        /* max_r_iw_systems;*/ 0,
                        /* max_directmap_enet_systems;*/ 0,
                        /* max_b_iw_systems;*/ 1,
                        /* max_m_iw_systems;*/ 0,
                        /* max_vlan_priority_maps; */ 2,
                        /* iw_bkgnd_fifo_size */0,
                        /* cong_pt       */NULL,
                        /* iw_host_limits */{10,0,0,10,0},
                        /* mpls_config*/ {0,0},
                        /* iw_modes;*/
                        {
                                /*policer_mode;*/                      WP_IW_POLICER_DISABLE, //WP_IW_FLOW_POLICER,
                                /*statistics_bundle_stat_mode;*/       WP_IW_STATISTICS_BUNDLE_STAT_DISABLE,
                                /*l2_prefix_ext_mode;*/                WP_IW_L2_HEADER_EXTENSION_ENABLE,
                                /*enhanced_flow_stat_mode;*/           WP_IW_ENHANCED_FLOW_STAT_DISABLE,
                                /*flow_stat_mode;*/                    WP_IW_FLOW_STAT_ENABLE,
                                /*fr_tx_flow_stat_mode */              WP_IW_FR_TX_FLOW_STAT_DISABLE,
                                /*mfc_alloc_mode; */                   WP_IW_MFC_ALLOC_DISABLE, //WP_IW_MFC_ALLOC_ENABLE,
                                /*Learning_queue_mode */               WP_IW_LEARNING_DISABLE,
                                /*port_filtering_mode */               WP_IW_PORT_FILTERING_DISABLE,
                                /* lpm_alloc_mode */                   WP_IW_LPM_ALLOC_DISABLE,
                                /*enhanced_mc_member_stat_mode*/       WP_IW_ENHANCED_MC_MEMBER_STAT_ENABLE,
                                /*routing_alloc_modes*/                WP_DISABLE,
                                /*res_mc_addresses_table_mode*/        WP_DISABLE,
                                /*port_stag_etype_mode*/               WP_IW_PORT_STAG_ETYPE_ENABLE
                        },
                        /* max_ch_agg_stat;*/ 0,
                        /* max_fr_s_iw_systems;*/ 0,
                        /*max_iw_ports;*/ 100,
                        /* max_iw_mc_groups;*/NUM_OF_MC_GROUPS+NUM_OF_VLAN_GROUPS,
                        /* max_nat_systems;*/0,
                        /*max_iw_second_aggregations;*/0, /* secondary flow aggregations */
                        /*iw_comp_limits;*/0,
                        /* max_iw_mc_members;*/(NUM_OF_MC_GROUPS+NUM_OF_VLAN_GROUPS)*IW_SYS_MAX_ENET_PORT,
                        /* max_iw_programmable_filters;*/MAX_IW_PROGRAMMABLE_FILTERS,
                        /* max_iw_policers;*/MAX_IW_POLICERS,
                        /* max_dscpmt */ 0,
                        /* max_wred_bindings */ 0,
                        /* tcm_policer_limits */
                        {
                                MAX_IW_POLICERS,
                                MAX_IW_POLICERS
                        }

                }
        };
        WP_context context[1] =
                {
                        {16, /* qnodes */
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
                                 {/* ASU TX QUEUE    */ WP_BUS_PACKET, APP_BANK_PACKET},
                                 {/* STATISTICS      */ WP_BUS_INTERNAL  , APP_BANK_INTERNAL},
                                 {/* ADDRESS LOOKUP  */ WP_BUS_PARAM, APP_BANK_PARAM},
                                 {/* ATM CPT         */ WP_BUS_PARAM, APP_BANK_PARAM},
                                 {/* ENET HDLC CPT   */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
                                 {/* IW FBP PARAM    */ WP_BUS_PARAM, APP_BANK_PARAM},
                                 {/* IW FBP DATA     */ WP_BUS_PACKET, APP_BANK_PACKET},
                                 {/* EXTERNAL FIFO   */ WP_BUS_PACKET, APP_BANK_PACKET},
                                 {/* MAC LOOKUP      */ WP_BUS_PARAM, APP_BANK_PARAM},
                                 {/* IMA SMALL       */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
                                 {/* IMA LARGE       */ WP_BUS_PARAM, APP_BANK_PARAM},
                                 {/* LPM             */ WP_BUS_PACKET, APP_BANK_PACKET},
                                 {/* MFC RULES       */ WP_BUS_PARAM, APP_BANK_PARAM},
                                 {/* FIWT, QAT       */ WP_BUS_PARAM, APP_BANK_PARAM},
                                 {/* MFCPT, UPPPT, DS*/ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
                                 {/* STATISTICS2     */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
                                 {/* STATISTICS3     */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
                                 {/* UNASSIGNED      */ WP_BUS_NONE, 0},
                                 {/* PKT_GLOBAL_TCPT */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
                                 {/* UNASSIGNED      */ WP_BUS_NONE, 0},
                                 {/* MANDATORY       */ WP_BUS_NONE, 0},
                                 {/* MANDATORY       */ WP_BUS_NONE, 0},
                                 /* ************* VIRTUAL BUS ASSIGNMENT ***************/
                                 {/* MCH_DATA        */ WP_BUS_PACKET, APP_BANK_PACKET},
                                 {/* WP_VB_OAM_FM_QUERY*/ WP_BUS_NONE, 0},
                                 {/* WP_VB_HASH_ON_HEAP*/ WP_BUS_NONE, 0},
                                 {/* WP_VB_MFC_INTERNAL*/ WP_BUS_PACKET, APP_BANK_PACKET},
                                 {/* WP_VB_MFR_RAS_TBL,           */ WP_BUS_NONE, 0},
                                 {/* WP_VB_CESOP_DATA,            */ WP_BUS_NONE, 0},
                                 {/* WP_VB_SECURITY_ESE,          */ WP_BUS_NONE, 0},
                                 {/* WP_VB_PDCP_SECURITY_ENTITY,  */ WP_BUS_NONE, 0},
                                 {/* WP_VB_POLICER,               */ WP_BUS_PARAM, APP_BANK_PARAM},
                                 {/* WP_VB_PCE_LRN,               */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
                                 {/* WP_VB_PCEA_PARAM,            */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
                                 {/* WP_VB_CONGESTION_TABLE,      */ WP_BUS_NONE, 0},
                                 /*WP_VB_PARAMS_PROPAGATION             */{WP_BUS_NONE, 0},
                                 /*WP_VB_STATS_IWPORT_PCE_FILTERS       */{WP_BUS_INTERNAL, APP_BANK_INTERNAL},
#if _WP_MODULE_CUSTOM_TIGERMILK_ENABLE_
                                 /*WP_VB_PROFILE_MARKING_STATS          */{WP_BUS_NONE, 0},
#endif
                                 /*WP_VB_TIMESTAMP_SERVICES_TABLE       */{WP_BUS_NONE, 0},
                                 /*WP_VB_POLICER_ACTION                 */{WP_BUS_PARAM, APP_BANK_PARAM},
                                 {/* WP_VB_PECS_GLOBAL*/ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
                                 {/* WP_VB_ENET_10G_LONG_PKT*/ WP_BUS_PARAM, APP_BANK_PARAM},
                                 {/* WP_VB_G999_STREAM*/ WP_BUS_NONE, 0},
                                 {/* WP_VB_G999_STREAM_STATISTICS*/ WP_BUS_NONE, 0},
                                 {/* WP_VB_FMU_FIFO_TO_CHANNEL_NUMBER*/ WP_BUS_NONE, 0},
                                 {/* WP_VB_IPWORT_EXT*/ WP_BUS_PARAM, APP_BANK_PARAM},
                                 {/* WP_VB_IW_GPMT_TABLE*/WP_BUS_PARAM, APP_BANK_PARAM},
                         },
                         {
                                 dps_ProgramImage, dps_ProgramImage
                         },
                         {
                                 dps_PC_Table_Init, dps_PC_Table_Init
                         },
                         &int_queue_tables, /* Interrupt queue tables */
                         POOL_LIMIT,        /* Mixumum number of pools */
                         NULL,           /* ATM global parameters */
                         max_iw          /* Interworking global parameters */
                        }
                };

        status = WP_DriverInit();
        if (WP_ERROR(status) == WP_ERR_WDDI_ALREADY_INITIALIZED)
        {
                status = WP_DriverRelease();
                App_TerminateOnError(status, "WP_DriverRelease", __LINE__);
        }

        /* initialize the ram partition, configure its pce rules's number */
        int_ram_partition.policer_entries = 0;
        int_ram_partition.pce_hw_rules_num = pce_init.hardware_rules_num;
        WT_SetIntRamPartition(WP_WINPATH(DEFAULT_WPID), &int_ram_partition);

        /* initialize GPMT tables */
        context->iw_data->max_iw_gpmt_tables = 159;
        context->iw_data->iw_modes.iw_characteristics |= WP_IW_MODES_GENERIC_PRIORITY_MAPPING_ENABLE;
        
        status = WP_SysInit(WP_WINPATH(DEFAULT_WPID), context);
        App_TerminateOnError(status, "WP_SysInit",__LINE__);

        return ;
}


/****************************************************************************************************************************
 * Function name: App_InitHW()
 * Description  :  
 *
 * Input  params: none
 * Output params: 
 * Return val   : none
 *****************************************************************************************************************************/

void App_InitHW(void)
{
        WP_status status;
        WP_U32    ii;

        /* board configure */
        status = WPX_BoardConfigure(WP_WINPATH(DEFAULT_WPID), WPX_CONFIGURE_DEFAULT_CHECKIN); /* 1xXAUI & 10xSGMII */
        App_TerminateOnError(status, "WPX_BoardConfigure()", __LINE__);

        /* XGI */
        status = WPX_BoardXgiConfig(WP_WINPATH(DEFAULT_WPID), WP_PORT_XGI1);
        App_TerminateOnError(status, "WPX_BoardXgiConfig()", __LINE__);   

        status = WPX_BoardSerdesInit(WP_WINPATH(DEFAULT_WPID), WP_PORT_XGI1, WP_FALSE); 
        App_TerminateOnError(status, "WPX_BoardSerdesInit",__LINE__);

        /* 10 GEs */
        for (ii = 0; ii < NR_GBE; ii++)
        {
                status = WPX_BoardSerdesInit(WP_WINPATH(DEFAULT_WPID), gbe[ii].hw_port, WPX_SERDES_NORMAL);
                App_TerminateOnError(status, "WPX_BoardSerdesInit 3()", __LINE__);
        }

	
        return ;
}


/*****************************************************************************
 * Function name: App_PortsDeviceCreate
 * Description  : 
 * Input  params: None
 * Output params: None
 * Return val   : None
 *****************************************************************************/
void App_PortsDevicesCreate(void)
{
        WP_status status;
        WP_U32 ii;
        WP_handle iwp1;
        WP_port_enet enet_port[1] =
                {
                        {
                                /* pkt_limits */
                                {
                                        /* max_tx_channels*/ MAX_NUM_OF_TX_CHANNELS, 
                                        /* max_rx_channels*/ MAX_NUM_OF_RX_CHANNELS,
                                        /* pq_block_size*/   NR_PQ_BLOCK_SIZE ,
                                        /* n_pq_blocks*/     NR_PQ_BLOCK,
                                        /* emphy_devices*/   WP_UNUSED
                                },
                                /* flowmode */       WP_ENET_FMU_HIERARCHICAL_SHAPING_MODE,
                                /* interface_mode */ WP_ENET_SGMII_1000,
                                /* rx_iw_bkgnd */    0
                        }
                };
        WP_device_enet enet_dev[1] =
                {
                        {
                                /* max_tx_channels */     MAX_NUM_OF_TX_CHANNELS,
                                /* tx_maxsdu */           WT_MAX_FRAME_SIZE,
                                /* operating_speed */     WP_UNUSED,
                                /* mac_addr */            {0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa},
                                /* tx_bit_rate */         WP_TX_BIT_RATE_UNLIMITED,
                                /* loopbackmode */        WP_ENET_NORMAL,
                                /* extended_params */     NULL,
                        }
                };
		WP_enet_filters rx_filters[1] =
                {
                        {
                                /* rx_ucfilter */WP_ENET_FILTER_PROCESS,
                                /* rx_bcfilter */WP_ENET_FILTER_PROCESS,
                                /* rx_mcfilter */WP_ENET_FILTER_PROCESS,
                        }
                };
        WP_device_enet_ex enet_device_extended_config[1] =
                {
                        {
                                /*duplexmode*/           WP_ENET_FULLDUPLEX,
                                /*rx_maclookup_mode*/    WP_DISABLE,
                                /*rx_flowcontrol*/       WP_ENET_FLOW_DISABLE,
                                /*tx_flowcontrol*/       WP_ENET_FLOW_DISABLE,
                                /*rx_addrmode*/          WP_ENET_ADDR_ACCEPTALL,
                                /*phystatmode*/          WP_ENET_STAT_ENABLE,
                                /*tx_statmode*/          WP_DISABLE,
                                /*rx_statmode*/          WP_DISABLE,
                                /*tx_duplicate*/         WP_DISABLE,
                                /*rx_filters*/           rx_filters,
                                /*rx_timestamp*/         WP_DISABLE,
                                /*timestamp_type*/       0,
                                /*max_tx_slow_channels*/ 0,
                        },
                };
        WP_port_enet enet_port_config =
                {
                        /* pkt_limits */
                        {
                                /* max_tx_channels*/ MAX_NUM_OF_TX_CHANNELS, 
                                /* max_rx_channels*/ MAX_NUM_OF_RX_CHANNELS,
                                /* pq_block_size*/   NR_PQ_BLOCK_SIZE ,
                                /* n_pq_blocks*/     NR_PQ_BLOCK,
                                /* emphy_devices*/   WP_UNUSED
                        },
                        /* flowmode */       WP_ENET_FMU_HIERARCHICAL_SHAPING_MODE,
                        /* interface_mode */ WP_ENET_SGMII_1000,
                        /* rx_iw_bkgnd */    0,       
                };

        WP_device_enet enet_dev_config =
                {
                        /* max_tx_channels */ MAX_NUM_OF_TX_CHANNELS,
                        /* tx_maxsdu */       WT_MAX_FRAME_SIZE,
                        /* operating_speed */ WP_UNUSED,
                        /* mac_addr*/         {0xaa,0xaa,0xaa,0xaa,0xaa,0xaa},
                        /* tx_bit_rate*/      WP_TX_BIT_RATE_UNLIMITED,
                        /* loopbackmode */    WP_ENET_NORMAL,
                        /* extended_params */ NULL 
                };
        WP_enet_tmu_compensation enet_device_tmu_compensation[] = 
        {
                {
                        .mode = WP_ENET_TMU_COMPENSATION_ENABLE,
                        .type = WP_ENET_TMU_COMPENSATION_FCS, 
                        .ud_length = 0,
                }, 
        };
	
        /* XGI port*/
        enet_port->interface_mode = WP_ENET_XAUI;
        xgi_port = WP_PortCreate(WP_WINPATH(DEFAULT_WPID), WP_PORT_XGI1, enet_port);
        App_TerminateOnError(xgi_port, "WP_PortCreate XGI1",__LINE__);

        /* Create an XGI Device */
        enet_dev->extended_params = NULL;
        xgi_dev = WP_DeviceCreate(xgi_port, 0,WP_DEVICE_ENET, enet_dev);
        App_TerminateOnError(xgi_dev, "WP_DeviceCreate() XGI",__LINE__);
   
        enet_device_extended_config->tx_force_pb = WP_ENET_TX_USE_PB_FORCE;
        enet_device_extended_config->tx_mode_10G = WP_ENET_TX_MODE_10G_LONG_PKT;
        enet_device_extended_config->tmuc = enet_device_tmu_compensation;
        enet_dev->extended_params = enet_device_extended_config;
        status = WP_DeviceModify(xgi_dev,
                                 WP_DEV_MOD_ENET_TX_STATMODE |
                                 WP_DEV_MOD_ENET_PHYSTATMODE |
                                 WP_DEV_MOD_ENET_RX_FLOWCONTROL | 
                                 WP_DEV_MOD_ENET_TX_FLOWCONTROL |
                                 WP_DEV_MOD_ENET_FORCE_PB_MODE |
                                 WP_DEV_MOD_ENET_10G_LONG_PKT_MODE |
                                 WP_DEV_MOD_ENET_RX_STATMODE |
                                 WP_DEV_MOD_ENET_TMU_COMPENSATION,
                                 enet_dev);
        App_TerminateOnError(status, "WP_DeviceModify() XGI",__LINE__);

        /* Create Giga enet port & device */
        for (ii = 0; ii < NR_GBE; ii++) 
        {
                // create GBE physical port
                enet_port_config.interface_mode = WP_ENET_SGMII_1000;
                gbe[ii].port_enet = WP_PortCreate(WP_WINPATH(0), gbe[ii].hw_port, &enet_port_config);
                App_TerminateOnError(gbe[ii].port_enet, "WP_PortCreate() Fast ENET", __LINE__);

                // create GBE physical device
                enet_dev_config.extended_params = NULL;
                gbe[ii].dev_enet = WP_DeviceCreate(gbe[ii].port_enet, WP_PHY(0), WP_DEVICE_ENET, &enet_dev_config);
                App_TerminateOnError(gbe[ii].dev_enet, "WP_DeviceCreate() Enet", __LINE__);

                // change to packet base mode
                enet_device_extended_config->tx_force_pb = WP_ENET_TX_USE_PB_FORCE;
                enet_device_extended_config->tx_mode_10G = WP_ENET_TX_MODE_DEFAULT;
                enet_device_extended_config->tmuc = enet_device_tmu_compensation;
                enet_dev_config.extended_params = enet_device_extended_config;
                status = WP_DeviceModify(gbe[ii].dev_enet,
                                         WP_DEV_MOD_ENET_TX_STATMODE |
                                         WP_DEV_MOD_ENET_RX_STATMODE |
                                         WP_DEV_MOD_ENET_PHYSTATMODE |
                                         WP_DEV_MOD_ENET_FORCE_PB_MODE |
                                         WP_DEV_MOD_ENET_RX_FLOWCONTROL | 
                                         WP_DEV_MOD_ENET_TX_FLOWCONTROL |
                                         WP_DEV_MOD_ENET_TMU_COMPENSATION,
                                         &enet_dev_config);
                App_TerminateOnError(status, "WP_DeviceModify() GBE",__LINE__);
        }

        /* Create Host ports and device */
        iwp1 = WP_PortCreate(WP_WINPATH(DEFAULT_WPID), WP_PORT_IW_HOST, NULL);
        App_TerminateOnError(iwp1, "WP_PortCreate() IW Host",__LINE__);

        /* Create an IW Host Device */
        devhost = WP_DeviceCreate(iwp1, 0, WP_DEVICE_IW_HOST, NULL);
        App_TerminateOnError(devhost, "WP_DeviceCreate() IW Host",__LINE__);
	
        return ;
}

/*****************************************************************************
 * Function name: App_BufferPoolsCreate
 * Description  : This function creates the buffer pools and Qnodes required for
 *     the application.   
 * Input  params: None
 * Output params: None
 * Return val   : None
 *****************************************************************************/
void App_BufferPoolsCreate(void)
{
        WP_qnode_iw_mcq iw_mc_qn_config[1]=
                {
                        {
                                /* interruptqueue */ 0,
                                /* num_buffers */ 1024,
                                /* offset */ 0x40,
                                /* size */ 128,
                                /* pad */ 0
                        }
                };
#if 0
        WP_handle pool_2048_0, pool_2048_1, pool_2048_2, pool_2048_3;
        WP_handle bd_pool[4];
        WP_pool_iw_bd bd_conf[1] =
                {
                        {
                                /*num_buffer_descriptors*/ 2000,
                                /*buffer_pool*/0,
                                /*bd_bus*/WP_BUS_INTERNAL,
                                /*wmm_num_buffer_descriptors*/ 0
                        }
                };
        WP_pool_buffer_data buffer_data_2048[] =
                {
                        {
                                /* n_buffers */ 192,
                                /* offset */ 128,
                                /* size */ 512, /* RBS */
                                /* pad */ 0,
                                /* bus */ BUFFER_BUS,
                                /* bank */ BUFFER_BANK
                        }
                };
        WP_fmu_shaping_wrr weight_params[1] =
                {
                        {
                                /*weight*/ 5000,
                                /*flags*/0,
                        }
   
                };

        WP_qnode_iw_group group_highp [1] =
                {
                        {

                                /*num_of_bd_pools*/2,
                                /*bd_pool*/ {0,0},
                                /*fifo_selection_method*/WP_QNODE_IW_SELECTION_WRR,
                                /*bd_pool_weight*/ /*{NULL,NULL}*/{&weight_params[0],&weight_params[0]}
                        }
                };
        WP_qnode_iw_group group_lowp [1]=
                {
                        {
      
                                /*num_of_bd_pools*/2,
                                /*bd_pool*/ {0,0},
                                /*fifo_selection_method*/WP_QNODE_IW_SELECTION_WRR,
                                /*bd_pool_weight*//*{NULL,NULL}*/{&weight_params[0],&weight_params[0]}
                        }
                };
        WP_qnode_iw_hier iw_hierq[1] =
                {
                        {
                                /*num_of_groups*/2,
                                /*iw_group*/ {{0},{0}},
                                /*group_selection_method*/ WP_QNODE_IW_SELECTION_STRICT,
                                /*group_weight*/{NULL,NULL}
      
                        }
                };

        /*Creates buffer pools*/
        buffer_data_2048->n_buffers = 2000;

        buffer_data_2048->bus = WP_BUS_PACKET;
        buffer_data_2048->bank = APP_BANK_PACKET;
        pool_2048_0 = WP_PoolCreate(WP_WINPATH(DEFAULT_WPID), WP_pool_iwbuffer, buffer_data_2048);
        App_TerminateOnError(pool_2048_0, "WP_PoolCreate() pool_2048",__LINE__);

        buffer_data_2048->bus = WP_BUS_HOST;
        buffer_data_2048->bank = APP_BANK_HOST;
        pool_2048_1 = WP_PoolCreate(WP_WINPATH(DEFAULT_WPID), WP_pool_iwbuffer, buffer_data_2048);
        App_TerminateOnError(pool_2048_1, "WP_PoolCreate() pool_2048",__LINE__);

        buffer_data_2048->bus = WP_BUS_HOST;
        buffer_data_2048->bank = APP_BANK_HOST;
        pool_2048_2 = WP_PoolCreate(WP_WINPATH(DEFAULT_WPID), WP_pool_iwbuffer, buffer_data_2048);
        App_TerminateOnError(pool_2048_2, "WP_PoolCreate() pool_2048_2",__LINE__);        

        buffer_data_2048->bus = WP_BUS_PACKET;
        buffer_data_2048->bank = APP_BANK_PACKET;
        pool_2048_3 = WP_PoolCreate(WP_WINPATH(DEFAULT_WPID), WP_pool_iwbuffer, buffer_data_2048);
        App_TerminateOnError(pool_2048_3, "WP_PoolCreate() pool_2048_2",__LINE__);

        bd_conf->buffer_pool = pool_2048_0;
        bd_conf->bd_bus = WP_BUS_INTERNAL;
        bd_pool[0] = WP_PoolCreate(WP_WINPATH(DEFAULT_WPID),WP_POOL_IW_BD, bd_conf);
        App_TerminateOnError(bd_pool[0], "WP_PoolCreate() bd_pool", __LINE__);
        
        bd_conf->buffer_pool = pool_2048_1;
        bd_conf->bd_bus = WP_BUS_INTERNAL;
        bd_pool[1] = WP_PoolCreate(WP_WINPATH(DEFAULT_WPID),WP_POOL_IW_BD, bd_conf);
        App_TerminateOnError(bd_pool[0], "WP_PoolCreate() bd_pool", __LINE__);
        

        bd_conf->buffer_pool = pool_2048_2;
        bd_conf->bd_bus = WP_BUS_PARAM;
        bd_pool[2] = WP_PoolCreate(WP_WINPATH(DEFAULT_WPID),WP_POOL_IW_BD, bd_conf);
        App_TerminateOnError(bd_pool[1], "WP_PoolCreate() bd_pool", __LINE__);

        bd_conf->buffer_pool = pool_2048_3;
        bd_conf->bd_bus = WP_BUS_PARAM;
        bd_pool[3] = WP_PoolCreate(WP_WINPATH(DEFAULT_WPID),WP_POOL_IW_BD, bd_conf);
        App_TerminateOnError(bd_pool[1], "WP_PoolCreate() bd_pool", __LINE__);

        /*Configure and create Hierarchical Qnode*/
        group_highp[0].bd_pool[0] = bd_pool[0];
        group_highp[0].bd_pool[1] = bd_pool[1];

        group_lowp[0].bd_pool[0] = bd_pool[2];
        group_lowp[0].bd_pool[1] = bd_pool[3];

        iw_hierq->iw_group[0] = group_highp[0];
        iw_hierq->iw_group[1] = group_lowp[0];

        qniw = WP_QNodeCreate(WP_WINPATH(DEFAULT_WPID), WP_QNODE_IW_HIERARCHICAL,iw_hierq);
        App_TerminateOnError(qniw, "WP_QNodeCreate()",__LINE__);
#else

        WP_handle pool_2048_buffer_chain;
        WP_qnode_iwq qn_iw[1]=
                {
                        {
                                /* interruptqueue */ 0,
                                /* num_buffers */ 2*8192,
                                /* adjunct_pool */ 0
                        }
                };
        WP_pool_buffer_data buffer_data_2048_buffer_chain[] =
                {
                        {
                                /* n_buffers */ 2*8192,
                                /* offset */ 64,
                                /* size */ 512, /* RBS */
                                /* pad */ 0,
                                /* bus */ WP_BUS_HOST,
                                /* bank */ APP_BANK_HOST
                        }
                };
        
        /* create buffer chain pools for RX */ 
        pool_2048_buffer_chain = WP_PoolCreate(WP_WINPATH(DEFAULT_WPID), WP_pool_iwbuffer, buffer_data_2048_buffer_chain);
        App_TerminateOnError(pool_2048_buffer_chain, "WP_PoolCreate() pool_2048_chain",__LINE__);

        qn_iw->adjunct_pool = pool_2048_buffer_chain;
        qniw = WP_QNodeCreate(WP_WINPATH(DEFAULT_WPID),WP_QNODE_IWQ| WP_QNODE_OPT_FMU, qn_iw);
        App_TerminateOnError(qniw, "WP_QNodeCreate() qniw", __LINE__);

#endif
        /* create Qnode */
        qniw_mc = WP_QNodeCreate(WP_WINPATH(DEFAULT_WPID), WP_QNODE_IW_MCQ, iw_mc_qn_config);
        App_TerminateOnError(qniw_mc, "WP_QNodeCreate() qniw_mc", __LINE__);

        return ;
}

/*****************************************************************************
 * Function name: App_PceMoudleInit
 * Description  : This function initialize the modules
 * Input  params: None
 * Output params: None
 * Return val   : None
 *****************************************************************************/
void App_ModulesInit(void)
{
        WP_status status;
        WP_module_gpe_init gpe_init =
                {
                        NUM_OF_PECS, /* max_num_of_pecs */
                        NUM_OF_PECS_GLOBAL_INFO, /* max_num_of_pecs_global_info */
                };

        /* PCE module initialization */
        pce_init.parser_info = &pce_parser_info;
        status = WP_ModuleInit(WP_SYSHANDLE(DEFAULT_WPID), WP_WDDI_MODULE_PCE, &pce_init);
        App_TerminateOnError(status, "WPI_PceModuleInit()",__LINE__);

        /* GPE module initialization */
        status = WP_ModuleInit(WP_SYSHANDLE(DEFAULT_WPID), WP_WDDI_MODULE_GPE, &gpe_init);
        App_TerminateOnError(status, "WPI_GpeModuleInit()", __LINE__);
}


/*****************************************************************************
 * Function name: App_EventsInit
 * Description  : This function initialize the events handler
 * Input  params: None
 * Output params: None
 * Return val   : None
 *****************************************************************************/
void App_EventsInit(void)
{
        WP_status status;

        /* Register Learning interrupt handler */
        status = WP_ControlRegister(WP_EVENT_IWGP_LEARNING_QUEUE, &F_MyAppIndicateEventIwgpLearning);
        App_TerminateOnError(status, "WP_ControlRegister()", __LINE__);

        return ;
}

/*****************************************************************************
 * Function name: App_ShapingGroupCreate
 * Description  : This function create the shapping group for switching
 * Input  params: None
 * Output params: None
 * Return val   : None
 *****************************************************************************/
void App_ShapingGroupCreateAndEnable(void)
{
        WP_U32 ii;
        WP_status status;
        WP_handle l1_block, dev_handle;
        WP_fmu_shaping_cir_eir cir_eir_param =
                {
                        /* cir */       500000,
                        /* cbs */       72000,
                        /* eir */       0,
                        /* ebs */       72000,
                        /* flags */     0,
                };
        WP_shaping_group_type_packet l1_shaping_group =
                {
                        /* group_level */       WP_L1_GROUP,
                        /* tx_shaping_type */   WP_FMU_SHAPING_TYPE_CIR_EIR,   //WP_FMU_SHAPING_TYPE_STRICT,
                        /* tx_shaping_params */ &cir_eir_param, //NULL,
                        /* num_fifos */         WP_NUM_FIFOS_8, //WP_UNUSED,
                        /* block_handle */      0,
                        /* block_level */       1,
                        /* group_mode */        0, //WP_UNUSED,
                };
        WP_shaping_group_type_packet l2_shaping_group =
                {
                        /* group_level */       WP_L2_GROUP,
                        /* tx_shaping_type */   WP_FMU_SHAPING_TYPE_STRICT,
                        /* tx_shaping_params */ NULL,
                        /* num_fifos */         WP_NUM_FIFOS_8, //WP_UNUSED,
                        /* block_handle */      0,
                        /* block_level */       0,
                        /* group_mode */        WP_MODE_HW,
                };
        
        for (ii = 0; ii < NR_L1_SHAPER_GROUP_ENTRIES; ii++)
        {
                if (ii == XGI_L1_SHAPER_GROUP_ENTRY)
                        dev_handle = xgi_dev;
                else
                        dev_handle = gbe[ii].dev_enet;
                
                // create shaping block for strict mode 
                l1_block = WP_ShapingBlockCreate(WP_SYSHANDLE(WP_WINPATH(0)),
                                                 WP_L1_GROUP,
                                                 1,
                                                 WP_NUM_FIFOS_8);
                App_TerminateOnError(l1_block, "WP_ShapingBlockCreate L1", __LINE__);
        
                /* create L1 shaping group */
                l1_shaping_group.block_handle = l1_block;
                l1_shaping_group.block_level = 0;
                shaper_l1_handles[ii] = WP_ShapingGroupCreate(dev_handle,
                                                              WP_SHAPING_GROUP_TYPE_PACKET,
                                                              &l1_shaping_group);
                App_TerminateOnError(shaper_l1_handles[ii], "WP_ShapingGroupCreate L1", __LINE__);

        }

        for (ii = 0; ii < NR_L2_SHAPER_GROUP_ENTRIES; ii++)
        {
                // create L2 shapging group
                shaping_blocks[ii] = WP_ShapingBlockCreate(WP_SYSHANDLE(WP_WINPATH(0)),
                                                           WP_L2_GROUP,
                                                           1, 
                                                           WP_NUM_FIFOS_8);
                App_TerminateOnError(shaping_blocks[ii], "WP_ShapingBlockCreate L2", __LINE__);

                /* create L2 shaping group */
                l2_shaping_group.block_handle = shaping_blocks[ii];
                l2_shaping_group.block_level = 0;
                shaper_l2_handles[ii] = WP_ShapingGroupCreate(shaper_l1_handles[ii],
                                                              WP_SHAPING_GROUP_TYPE_PACKET,
                                                              &l2_shaping_group);
                App_TerminateOnError(shaper_l2_handles[ii],
                                     "WP_ShapingGroupCreate L2", __LINE__);
        }

        for (ii = 0; ii < NR_L1_SHAPER_GROUP_ENTRIES; ii++)
        {
                status = WP_ShapingGroupEnable(shaper_l1_handles[ii]);
                App_TerminateOnError(status , "WP_ShapingGroupEnable l1",__LINE__);
        }
        
        for (ii = 0; ii < NR_L2_SHAPER_GROUP_ENTRIES; ii++)
        {
                status = WP_ShapingGroupEnable(shaper_l2_handles[ii]);
                App_TerminateOnError(status , "WP_ShapingGroupEnable l2",__LINE__);
        }
        
        return ;
}


/*****************************************************************************
 * Function name: App_ChannelsCreate
 * Description  : This function creates the Enet channels
 * Input  params: None
 * Output params: None
 * Return val   : None
 *****************************************************************************/   
void App_ChannelsCreate(void)
{
        WP_tag ch_tag = 0;
        WP_status status;
        WP_U16 ii, jj;
        WP_ch_enet gbe_ch[1] =
                {
                        {
                                /* intmode */           WP_PKTCH_INT_DISABLE,
                                /* iwmode */            WP_PKTCH_IWM_ENABLE,
                                /* testmode */          WP_PKTCH_TEST_DISABLE,
                                /* tx_pqblock */        0,
                                /* tx_pqlevel */        0,
                                /* tx_shaping_type */   WP_FMU_SHAPING_TYPE_STRICT,  //WP_UNUSED, //
                                /* tx_shaping_params */ NULL,
                                /* rx_maxsdu */         WT_MAX_FRAME_SIZE,
                                /* tx_cwid */           WP_CW_ID_A,
                                /* tx_tq */             WP_UNUSED,
                                /* rx_queuedepth[4] */  50
                        },
                };
        WP_tx_binding  tx_binding_l2pi[1]=
                {
                        {
                                /* wred_mode;*/ 0,
                                /* dci_mode;*/  1,
                                /* maxt;*/100,
                        }
                };
        WP_ch_enet enet_ch_config =
                {
                        /* intmode */         WP_PKTCH_INT_DISABLE,
                        /* iwmode */          WP_PKTCH_IWM_ENABLE,
                        /* testmode */        WP_PKTCH_TEST_DISABLE,
                        /* tx_pqblock */      0,
                        /* tx_pqlevel */      0,
                        /* tx_shaping_type*/  WP_FMU_SHAPING_TYPE_STRICT, // WP_UNUSED, //
                        /* tx_shaping_params*/NULL,
                        /* rx_maxsdu */       WT_MAX_FRAME_SIZE,
                        /* tx_cwid */         WP_CW_ID_A,
                        /* tx_tq */           0, //WP_UNUSED,
                        /* rx_queuedepth */   50,
                };
        WP_tx_binding  tx_binding_enet_config =
                {
                        /* res0 */      0,
                        /* dci_mode;*/  WP_IW_DYN_CH_INSERT_ENABLE,
                        /* maxt;*/      100,
                };

        /* create XGI device rx and tx channels */
        xgi_rx_ch_handle = WP_ChannelCreate(ch_tag++, 
                                            xgi_dev, 
                                            qniw, 
                                            WP_CH_RX, 
                                            WP_ENET, 
                                            &gbe_ch[0]);
        App_TerminateOnError(xgi_rx_ch_handle, "WP_ChannelCreate() Enet RX XGI",__LINE__);

        /* Create XGI TX channels */
        for (jj = 0; jj < NR_PRIORITY_CHANNELS; jj++)
        {
                gbe_ch[0].tx_tq = jj;

                xgi_tx_ch_handle[jj] = WP_ChannelCreate(ch_tag++, 
                                                        shaper_l2_handles[XGI_L2_SHAPER_GROUP_ENTRY],
                                                        qniw, 
                                                        WP_CH_TX, 
                                                        WP_ENET, 
                                                        &gbe_ch[0]);
                App_TerminateOnError(xgi_tx_ch_handle[jj], "WP_ChannelCreate() Enet TX XGI",__LINE__);
   
                /* Create Tx binding to the Tx Channel XGI */
                status = WP_IwTxBindingCreate(xgi_tx_ch_handle[jj], WP_IW_TX_BINDING, &tx_binding_l2pi[0]);
                App_TerminateOnError(status, "WP_IwTxBindingCreate() ENET TX XGI",__LINE__);
        }

        /* Channels for 10 GEs */
        for (ii = 0; ii < NR_GBE; ii++)
        {
                gbe[ii].rx_chan_enet = WP_ChannelCreate(0, gbe[ii].dev_enet, qniw, WP_CH_RX, WP_ENET, &enet_ch_config);
                App_TerminateOnError(gbe[ii].rx_chan_enet, "WP_Channel_Create() Fast RX", __LINE__);

                /* Create GBE TX channels */
                for (jj = 0; jj < NR_PRIORITY_CHANNELS; jj++)
                {
                        enet_ch_config.tx_tq = jj;

                        gbe[ii].tx_chan_enet[jj] = WP_ChannelCreate(0, shaper_l2_handles[ii],
                                                                    qniw, WP_CH_TX, WP_ENET, &enet_ch_config);
                        App_TerminateOnError(gbe[ii].tx_chan_enet[jj], "WP_Channel_Create() Fast TX", __LINE__);

                        status = WP_IwTxBindingCreate(gbe[ii].tx_chan_enet[jj], WP_IW_TX_BINDING, &tx_binding_enet_config);
                        App_TerminateOnError(status, "WP_IwTxBindingCreate Fast TX", __LINE__);
                }
        }

        return ;
}


/*****************************************************************************
 * Function name: App_IwSystemInit
 * Description  : 
 * Input  params: None
 * Output params: None
 * Return val   : None
 *****************************************************************************/   
void App_IwSystemInit(void)
{
        WP_iw_sys_bridging iw_sys_bridging[1]=
                {
                        {
                                /* tag */                   0,
                                /* max_flows; */            8,
                                /* classification_mode */   WP_IW_NO_CLASSIFIER,
                                /* classifier_config */     {0,0,0,NULL},
                                /* learning_mode;*/         WP_IW_BRIDGE_LEARNING_DISABLE,
                                /* learning_queue */
                                {
                                        /*int_queue_num*/        WP_IW_IRQT1,
                                        /*learning_queue_size*/  LEARNING_QUEUE_SIZE,
                                        /*interrupt_enbale*/     WP_LEARNING_INTENABLE,
                                        /*interrupt_rate*/       1
                                },
                                /*forwarding_table_size*/   WP_IW_HASH_UNUSED,
                                /*member_set_size*/         WP_IW_HASH_UNUSED,
                                /*buffer_gap;*/             0x80,
                                /*max_bridging_ports*/      IW_SYS_MAX_BPORTS,
                                /*dfc_info:*/               NULL,
                                /*svl_mode*/                WP_IW_SVL_DISABLED, 
                                /*stag_ether_type*/         0/*DEFAULT VALUE*/,
                        }
                };	

        /* Create interworking system */
        dl_general_iwsys_bridge = WP_IwSystemCreate(WP_WINPATH(DEFAULT_WPID), WP_IW_VLAN_AWARE_BRIDGING_MODE, &iw_sys_bridging[0]);
        App_TerminateOnError(dl_general_iwsys_bridge, "WP_IwSystemCreate",__LINE__);

        // Create host handler, host agg, host iw port
        WPE_HostCreate();

        /* Create XGI & 10 GEs bridging ports */
        WPE_BridgePortsCreate(dl_general_iwsys_bridge);

        /* Binding the RX channels to bridging ports */
        WPE_RxBindingCreate(dl_general_iwsys_bridge);

        /* Create TX flow aggregation */
        WPE_TxAggCreate();
}


/*****************************************************************************
 * Function name: App_SystemEnable
 * Description  : 
 * Input  params: None
 * Output params: None
 * Return val   : None
 *****************************************************************************/   
void App_SystemEnable(void)
{
        WP_status status;
        WP_U16 ii, jj;

        status = WP_PortEnable(xgi_port, WP_DIRECTION_DUPLEX);
        App_TerminateOnError(status, "WP_PortEnable gbe_port",__LINE__);
   
        status = WP_DeviceEnable(xgi_dev, WP_DIRECTION_DUPLEX);
        App_TerminateOnError(status, "WP_DeviceEnable gbe_dev", __LINE__);

        status = WP_ChannelEnable(xgi_rx_ch_handle);
        App_TerminateOnError(status, "WP_ChannelEnable Rx XGI",__LINE__);

        for (jj = 0; jj < NR_PRIORITY_CHANNELS; jj++)
        {
                status = WP_ChannelEnable(xgi_tx_ch_handle[jj]);
                App_TerminateOnError(status, "WP_ChannelEnable Tx XGI",__LINE__);
        }
        
        for (ii = 0; ii < NR_GBE; ii++)
        {
                status = WP_PortEnable(gbe[ii].port_enet, WP_DIRECTION_DUPLEX);
                App_TerminateOnError(status, "WP_PortEnable",__LINE__);
   
                status = WP_DeviceEnable(gbe[ii].dev_enet, WP_DIRECTION_DUPLEX);
                App_TerminateOnError(status, "WP_DeviceEnable", __LINE__);

                status = WP_ChannelEnable(gbe[ii].rx_chan_enet);
                App_TerminateOnError(status, "WP_ChannelEnable",__LINE__);

                for (jj = 0; jj < NR_PRIORITY_CHANNELS; jj++)
                {
                        status = WP_ChannelEnable(gbe[ii].tx_chan_enet[jj]);
                        App_TerminateOnError(status, "WP_ChannelEnable",__LINE__);
                }
        }

        return ;
}


/*****************************************************************************
 * Function name: App_PceInterfaceCreate
 * Description  : 
 * Input  params: None
 * Output params: None
 * Return val   : None
 *****************************************************************************/   
void App_PceInterfaceCreate(void)
{
        // vlan flooding and multicast filters for DL & UL
        WPE_MulticastInit();
        WPE_VlanInit();

        // Create all filters
        WPE_CreatePceFilters();

        // Create filter sets
        WPE_CreatePceFilterSets();

        // Create pce interfaces
        WPE_CreatePceInterface(dl_general_iwsys_bridge);

        // Reset statistics
        WPE_CheckPceFilterStatsReset();
	
        return ;
}

/*****************************************************************************
 * Function name: App_perform_freerun
 * Description  : 
 * Input  params: None
 * Output params: None
 * Return val   : None
 *****************************************************************************/   
void App_perform_freerun(void)
{
        // TODO:

        return ;
}


/*****************************************************************************
 * Function name: App_LearningThread
 * Description  : 
 * Input  params: None
 * Output params: None
 * Return val   : None
 *****************************************************************************/   
void * App_LearningThread(void *arg)
{
        WP_U32 ii, size;
        WP_status status;
        WP_U32 rules_returned;
        WP_pce_rule_forwarding rule_fwd = {0};
        WP_U32 StartTimeStamp = 0, CurrentTimeStamp = 0;

        size = (NR_PM_TEST_REQUIRED_RULES < WP_PCE_LEARNING_QUEUE_MAX_SIZE)?
                NR_PM_TEST_REQUIRED_RULES : WP_PCE_LEARNING_QUEUE_MAX_SIZE;

        while (1)
        {
#if LEARNING_MODE_INTERRUPT
                /* semaphore increased in the learning interrupt handler */
                WPL_SemDecrement(&learning_sem, 1);
#else
                WP_Delay(5);
#endif

                /* For performance test */
                if (PMTest_enable)
                {
                        StartTimeStamp = WT_TimeRead();
                }

                /* Receive for learning Q */
                status = WP_PceLearningQueueService(WP_WINPATH(DEFAULT_WPID), 
                                                    &(learned_rules_list[0]),
                                                    size,
                                                    &rules_returned);
                App_TerminateOnError(status, "WP_PceLearningQueueService", __LINE__);

                if (!learning_enable || !rules_returned)
                {
                        continue;
                }

                for (ii = 0; ii < rules_returned; ii++)
                {
                        memcpy(&rule_fwd, &(learned_rules_list[ii].fwd_rule), sizeof(WP_pce_rule_forwarding));

                        /* Enable aging and set weight */
                        rule_fwd.match_result[1].param.aging.mode = WP_PCE_AGING_ENABLE;
                        rule_fwd.match_result[1].param.aging.weight = aging_weight;
                        
                        PCE_rule_handle = WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID),
                                                           WP_PCE_RULE_FORWARDING,
                                                           &rule_fwd);
                        if(!WP_ERROR_P(PCE_rule_handle))
                        {
                                learnt_rules++;
                        }
                        else
                        {
                                if (WP_ERROR(PCE_rule_handle) == WP_ERR_PCE_RULE_ALREADY_EXISTS)
                                {
                                        continue;
                                }
                                else
                                {
                                        App_TerminateOnError(PCE_rule_handle, "WP_PceRuleCreate", __LINE__);
                                }
                        }

                        if (!PMTest_enable)
                        {
                                WPE_AddFDBEntry(&rule_fwd,PCE_rule_handle);
                        }
                }

                /* For performance test */
                if (PMTest_enable)
                {
                        CurrentTimeStamp = WT_TimeRead();

                        PMTest_delta += WT_TimeDelta(CurrentTimeStamp, StartTimeStamp);

                        printf("Performance Test: Already learnt %d rules\n", learnt_rules);
                        
                        if (learnt_rules >= NR_PM_TEST_REQUIRED_RULES)
                        {
                                printf("Performance Test: learnt %d rules, in Time delta (in micro sec) %d\n", learnt_rules, PMTest_delta);
                                PMTest_enable = 0;
                        }
                }
        }

        return arg;
}

/*****************************************************************************
 * Function name: App_LearningThread
 * Description  : 
 * Input  params: None
 * Output params: None
 * Return val   : None
 *****************************************************************************/   
void * App_AgingFunc(void *arg)
{
        static WP_handle aging_iterator_handle = 0;
        WP_handle rule_handle;
        WP_U32 aged_rules = 0;
        WP_status status;
        WP_U32 StartTimeStamp = 0, CurrentTimeStamp = 0, delta = 0;

        /* Iterator for run through the Filter rules */
        if (!aging_iterator_handle)
        {
                aging_iterator_handle = WP_PceIteratorCreate(WP_WINPATH(DEFAULT_WPID), 
                                                             WP_PCE_ITERATOR_TYPE_AGED_RULE, 
                                                             (void *)NULL);
                
                App_TerminateOnError(aging_iterator_handle,"WP_PceIteratorCreate",__LINE__);
        }

        rule_handle = WP_PceIteratorGetFirst(aging_iterator_handle);
        if (!WP_ERROR_P(rule_handle))
        {
                aged_rules++;

                StartTimeStamp = WT_TimeRead();
                
                while(1)
                {
                        WPE_DelFDBEntry(rule_handle);

                        /* delete the aged rules */
                        status = WP_PceRuleDelete(rule_handle);
                        App_TerminateOnError(status, "WP_PceRuleDelete", __LINE__);

                        rule_handle = WP_PceIteratorGetNext(aging_iterator_handle);
                        if (WP_ERROR_P(rule_handle))
                        {
                                if (WP_ERROR(rule_handle) != WP_ERR_PCE_ITERATOR_NO_ITEMS)
                                        printf("\n\nerror %s \n", WP_ErrorString(rule_handle));
                                break;
                        }

                        aged_rules++;
                }

                CurrentTimeStamp = WT_TimeRead();
                delta += WT_TimeDelta(CurrentTimeStamp, StartTimeStamp);

                printf("Aging : Already aging out %d rules, in Time delta (in micro sec) %d\n", aged_rules, delta);
        }
        else
        {
                if (WP_ERROR(rule_handle) != WP_ERR_PCE_ITERATOR_NO_ITEMS)
                        printf("\n\nerror %s \n", WP_ErrorString(rule_handle));
        }

        return arg;
}

void * App_AgingThread(void *arg)
{
        while (1)
        {
                WPL_SemDecrement(&aging_sem, 1);
                
                if (!aging_enable)  continue;
                
                App_AgingFunc(NULL);
        }
}

// for init ram
#include "wt_partition_ecc_util.c"
