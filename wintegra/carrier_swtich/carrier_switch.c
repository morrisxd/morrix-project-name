/*****************************************************************************
 * (C) Copyright PMC - WIN division (Wintegra) 2013.  All rights reserved.
 * PMC-Sierra (WINTEGRA) CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of PMC-Sierra (Wintegra).
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "api/wp_wddi.h"
#include "api/wp_host_io.h"
#include "wp_sim.h"

#include "wpx_app_data.h"
#include "wpx_oc12_pmc5357.h"
#include "wpx_gei_phy.h"
#include "wpx_pin_mux_selector.h"
#include "wt_partition_ecc_util.h"

#include "wpi_gpe_pecs_wddi_shared.h"
#include "brouter_pecs.h"
#include "brouter_v2_pecs.h"

#include "carrier_switch.h"

#include "carrier_switch_util.h"



#include "wpx_enet_phy.h"

#ifdef WP_HW_WINPATH3
#include "wpu_debug_sw.h"
#endif

#include "api/wp_pce.h"

#include "carrier_switch.h"

#include "carrier_switch_util.h"

BOARD_TYPE boardtype = BOARD_TYPE_4XGI; // default

WT_TP_PORT hw_port[NR_GBE] = {{WP_PORT_ENET13, }, {WP_PORT_ENET14, },
                              {WP_PORT_ENET15, }, {WP_PORT_ENET16, },
                              {WP_PORT_ENET3, }, {WP_PORT_ENET5, },
                              {WP_PORT_ENET4, }, {WP_PORT_ENET6, }};

WPE_TP_VPNPORT   vpn_port[NUM_OF_VPN_PORT];
WPE_IN_POLICER  ing_policer[NUM_OF_POLICER];


WP_handle devhost, iwp, TEST_pool;
WP_handle qniw;
WP_handle qniw_mc;

WP_handle buffer_pool[WPE_XGI_PORTS_NUM][WTE_TMU_FBP_L3_PER_L2_NUM];
WP_handle bd_pool[WPE_XGI_PORTS_NUM][WTE_TMU_FBP_L2_NUM];
WP_handle iw_hier_qnode[WPE_XGI_PORTS_NUM];


WP_handle pecs_handles[NUM_OF_PECS];
WT_VsiSystemInfo vpn_system[MAX_IW_SYSTEM];
WP_U8 system_num[SYSTEM_TYPE_VPWS + 1] = {0};
WT_VsiSystemInfo common_system;


WP_U32 aging_enable = 1;
WP_U32 aging_weight = 1;
WP_U32 aging_period = 60 * 5;  /* 5 Minutes */

WP_THREAD_ID aging_tid;

WP_U32 learning_enable = 1;
WP_U32 learnt_rules = 0;
WP_THREAD_ID learning_tid;

WP_U32 PMTest_enable = 0;
WP_U32 PMTest_vid = 0;
WP_U32 PMTest_delta = 0;

WP_handle UNI_PCE_filter[FILTER_LAST];
WP_handle NNI_PCE_filter[FILTER_F8];

WP_handle filter_set_uni_no_subport, filter_set_uni_subport, filter_set_vpls, filter_set_nni_pre, filter_set_nni_vrf, filter_set_nni_vpws;

WP_pce_learned_forwarding_rule learned_rules_list[1000];


WP_U32 max_mac_addresses = 100;

WP_U32 bridge_lock;

WP_SEM_ID aging_sem;
WP_SEM_ID learning_sem;

WP_boolean demo_running = WP_TRUE;

WP_int_queue_table int_queue_tables = {{{65500, 1}, {10, 1}, {10, 1}, {10, 1}}};
WP_pce_parser_info pce_parser_info;

WP_sw_fdb_info sw_fdb_info =
{
        WP_BUS_PARAM, /* sw_fdb_bus */ 
        0, /* fdb_threshold */
        (4*1024 - 1), /* max_fdb_rules */
};

WP_module_pce_init pce_init =
{
        &pce_parser_info, /* parser_info */
        32000, /* hardware_rules_num */
        20, /* filters_num */
        50, /* pkt_rx_channel_interfaces_num */
        30, /* iw_tx_channel_interfaces_num */
        WP_PCE_DISCARD_MODE_SMART_DISCARD, /* pce_discard_mode */
        WP_ENABLE, /* bridge_loop_prevention_mode */
        WP_ENABLE | WP_PCE_LEARNING_PROTECTION_ENABLE, /* learning_mode */
        {/* learning_queue_config */
                1000, //WP_PCE_LEARNING_QUEUE_MAX_SIZE, /* learning_queue_size */
#if LEARNING_MODE_INTERRUPT
                WP_LEARNING_INTENABLE, /* interrupt_mode */
#else
                WP_LEARNING_POLLING_MODE, /* interrupt_mode */
#endif
                WP_IW_IRQT0, /* int_queue_num */
                1, /* interrupt_rate */
                4, /* protection_table_size */
        },
        100, /* iw_next_round_pce_interfaces_num*/
        0, /* interfaces_on_atm_rx_mode */
        0,/* atm_rx_channel_interfaces_num */
        0,/* device_interfaces_num */
        WP_NO_ENHANCMENT/*WP_SW_FDB_ENABLE_MODE*/,/* enhanced_mode */
        NULL,//&sw_fdb_info, /* sw_fdb_info  */
};

WP_gpe_pecs gpe_pecs_cfg[NUM_OF_PECS] = 
{
        {
                &brouter_pecs_pecs_image, /*pecs_image_info*/
                &WPE_GpeBrouterPecsService /*pecs_service_fp*/
        },

};


/* IW-GPMT */
WP_handle iw_gpmt_table_handle[MAX_GPMT_TABLE];
WP_iw_gpmt iw_gpmt_cfg[MAX_GPMT_TABLE];
WP_iw_gpmt_priority_params iw_gpmt_action =
{
      {/* green */
            WP_IW_GPMT_PRIORITY_MAPPING | WP_IW_GPMT_PREFIX_REMARKING,
            {
                  {
                        WP_IW_GPMT_REMARKING_TYPE_PREFIX_REMARKING_VAL,
                        WP_IW_GPMT_REMARKING_TYPE_PREFIX_REMARKING_VAL,
                        WP_IW_GPMT_REMARKING_TYPE_PREFIX_REMARKING_VAL,
                        WP_IW_GPMT_REMARKING_TYPE_PREFIX_REMARKING_VAL,
                  },
                  {
                        5, 5, 5, 5
                  }
            },
            /* priority_mapping_value */ /*0x11*/0x0,
            /* congestion_profile_entry */ 0
      },
      {/* yellow */
            WP_IW_GPMT_PRIORITY_MAPPING | WP_IW_GPMT_PREFIX_REMARKING,
            {
                  {
                        WP_IW_GPMT_REMARKING_TYPE_PREFIX_REMARKING_VAL,
                        WP_IW_GPMT_REMARKING_TYPE_PREFIX_REMARKING_VAL,
                        WP_IW_GPMT_REMARKING_TYPE_PREFIX_REMARKING_VAL,
                        WP_IW_GPMT_REMARKING_TYPE_PREFIX_REMARKING_VAL,
                  },
                  {
                        4, 4, 4, 4
                  }
            },
            /* priority_mapping_value */ /*0x12*/0x0,
            /* congestion_profile_entry */ 0
      },
      {/* red */
            WP_IW_GPMT_PRIORITY_MAPPING | WP_IW_GPMT_PREFIX_REMARKING,
            {
                  {
                        WP_IW_GPMT_REMARKING_TYPE_PREFIX_REMARKING_VAL,
                        WP_IW_GPMT_REMARKING_TYPE_PREFIX_REMARKING_VAL,
                        WP_IW_GPMT_REMARKING_TYPE_PREFIX_REMARKING_VAL,
                        WP_IW_GPMT_REMARKING_TYPE_PREFIX_REMARKING_VAL,
                  },
                  {
                        6, 6, 6, 6
                  }
            },
            /* priority_mapping_value */ /*0x14*/0x0,
            /* congestion_profile_entry */ 0
      },
};


WP_bridge_port bport_conf =
{
    /* tag */                   0,
    /* direct_mapping*/         WP_IW_DIRECT_MAP_ENABLE,
    /* flow_agg */              0,
    /* termination_mode*/       WP_IW_HOST_TERM_MODE,
    /* learning_mode */         WP_IW_LEARNING_DISABLE,
    /* in_filter_mode */        WP_IW_INGRESS_FILTER_DISABLE,
    /* vlan_param */
    {
        /* vlan_acceptance_mode */WP_IW_ACCEPT_TAGGED_UNTAGGED, //WP_IW_ACCEPT_TAGGED_ONLY,
        /* vlan_tag */0,
        /*vlan_tunnel*/WP_IW_VLAN_TUNNEL_1Q_IN_1Q_DISABLE,
        /*vlan_priority_enforce_mode*/WP_IW_VLAN_PRIORITY_ENFORCE_DISABLED,
        /*stag_vlan_etype*/ DEFAULT_STAG_TYPE, //WP_IW_BPORT_STAG_VLAN_ETYPE_DISABLE
    },
    /* max_mac_addresses */ 300,
    /* group_tag */ 0,
    /*group_filtering_mode*/     WP_IW_GROUP_FILTER_DISABLE,
    /*unk_mac_sa_filter*/        WP_IW_UNK_MACSA_FILTER_DISABLE,
    /*unk_mc_mode*/              WP_IW_UNK_MC_DROP,
    /*bc_ht_mode*/               WP_IW_BC_HT_DISABLE,
    /*unk_input_filters_mask*/   0,
    /*unk_output_filters_mask*/  0,
    /*statmode*/                 WP_IW_PORT_STAT_ENABLE,
    /*unk_uc_mode*/              WP_IW_UNK_UC_SR_ENABLE,
    /*classification_flag*/      WP_IW_BPORT_CLASSIFICATION_ENABLED,
    /*adv_unk_lookup_mode*/      WP_IW_ADV_UNK_LOOKUP_ENABLED,
    /*cfi_ht_mode*/              WP_IW_CFI_HT_DISABLE,
    /*reserved_mc_ht_mode*/      WP_IW_RES_MC_HT_DISABLE,
    /*predefined_ht_mode*/       WP_DISABLE,
    /*res_mc_action_table */     {0},
    /*WP_iw_bport_enet_oam_params */ NULL
};


#include "carrier_switch_api.c"
#include "carrier_switch_util.c"
#include "carrier_switch_menu.c"

#ifndef __linux__
#include "os_timer.c"
#endif

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
        if (WP_ERROR_P(handle)) 
        {
            printf("%s: Terminal error (handle 0x%x) %s\n", appname, handle, s);
            printf("%s: Error type - %s\n", appname, WP_ErrorString(handle));
            printf("line = %d\n",line);
            printf("Test Failed\n");

            WP_DriverRelease();

            WPE_Reboot();
            exit(0);
        }
#if DEUBG_ENABLE        
        else
        {
          if(handle == WP_OK)
             printf("Status returned from %s : WP_OK\n",s);
          else
             printf("Handle returned from %s is %#8.8x\n", s,handle);
       }
#endif       
}


void App_HwPortInit(void)
{
    WP_U8 ii = 0;

    memset(hw_port, 0, sizeof(hw_port));
    
    switch (boardtype)
    {
        case BOARD_TYPE_DEFAULT:
            hw_port[0].hw_port = WP_PORT_XGI1;
            hw_port[2].hw_port = WP_PORT_XGI3;  
            
            hw_port[4].hw_port = WP_PORT_ENET6;
            hw_port[5].hw_port = WP_PORT_ENET8;
            hw_port[6].hw_port = WP_PORT_ENET21;
            hw_port[7].hw_port = WP_PORT_ENET23;
            
            hw_port[8].hw_port = WP_PORT_ENET14;
            hw_port[9].hw_port = WP_PORT_ENET3;
            hw_port[10].hw_port = WP_PORT_ENET5;
            
            hw_port[11].hw_port = WP_PORT_ENET4;
            
            hw_port[12].hw_port = WP_PORT_ENET13;
            hw_port[13].hw_port = WP_PORT_ENET15;            
            hw_port[14].hw_port = WP_PORT_ENET10;
            hw_port[15].hw_port = WP_PORT_ENET7;
            
            hw_port[16].hw_port = WP_PORT_ENET9;
            hw_port[17].hw_port = WP_PORT_ENET11;

        break;
        
        case BOARD_TYPE_4XGI:
            hw_port[0].hw_port = WP_PORT_XGI1;  
            hw_port[1].hw_port = WP_PORT_XGI2; 
            hw_port[2].hw_port = WP_PORT_XGI3;  
            hw_port[3].hw_port = WP_PORT_XGI4; 
            hw_port[4].hw_port = WP_PORT_ENET14;
            hw_port[5].hw_port = WP_PORT_ENET3;
/*
            hw_port[6].hw_port = WP_PORT_ENET17;
            hw_port[7].hw_port = WP_PORT_ENET18;
            hw_port[8].hw_port = WP_PORT_ENET19;
            hw_port[9].hw_port = WP_PORT_ENET20;
            hw_port[10].hw_port = WP_PORT_ENET21;
            hw_port[11].hw_port = WP_PORT_ENET22;
            hw_port[12].hw_port = WP_PORT_ENET23;
            hw_port[13].hw_port = WP_PORT_ENET24;
            hw_port[14].hw_port = WP_PORT_ENET16;
*/
        break;
        
        case BOARD_TYPE_2XGI_20SGMII:
            hw_port[0].hw_port = WP_PORT_XGI1;  // TODO
            hw_port[1].hw_port = WP_PORT_XGI2;  // TODO

            for (ii = WPE_XGI_PORTS_NUM; ii < NR_GBE; ii++)
            {
                hw_port[ii].hw_port = WP_PORT_ENET1 + ii - WPE_XGI_PORTS_NUM; // TODO
            }

        break;

        case BOARD_TYPE_8SGMII:
            hw_port[4].hw_port = WP_PORT_ENET13;
            hw_port[5].hw_port = WP_PORT_ENET14;
            hw_port[6].hw_port = WP_PORT_ENET15;
            hw_port[7].hw_port = WP_PORT_ENET16;
            
            hw_port[8].hw_port = WP_PORT_ENET3;
            hw_port[9].hw_port = WP_PORT_ENET5;
            hw_port[10].hw_port = WP_PORT_ENET4;
            hw_port[11].hw_port = WP_PORT_ENET6;
            
        break;

        default:            
            printf("NOT supported.\n");
            exit(0);
        break;
    }


    for (ii = 0; ii < NR_GBE; ii++)
    {
        if ((hw_port[ii].hw_port <= WP_PORT_XGI4) 
            && (hw_port[ii].hw_port >= WP_PORT_XGI1))
        {    
            hw_port[ii].hw_type = PORT_HW_XGI;
        }
        else
        if ((hw_port[ii].hw_port <= WP_PORT_ENET24) 
            && (hw_port[ii].hw_port >= WP_PORT_ENET1))
        {    
            hw_port[ii].hw_type = PORT_HW_SGMII;
        }    
    }    
}

void App_SetBoardType(void)
{
    boardtype = BOARD_TYPE_4XGI;
    //boardtype = CLI_GetNumber("Please set board type(1 - XGI*4, 2 - XGI*2 & SGMII*20)", BOARD_TYPE_4XGI, BOARD_TYPE_2XGI_20SGMII);
}

void App_PreSetup(void)
{
    App_SetBoardType();
    App_HwPortInit();
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

    App_PreSetup();
    
    /* driver, system context, ram partition init */
    App_SysInit();

    /* variables init */
    App_TestInit();
    
    /* hardware configuration */
    App_InitHW();

    /* create physical port and device */
    App_PortsDevicesCreate();

    /* create buffer pool */
    App_BufferPoolsCreate();

    /* PCE, GPE and Policer module init */
    App_ModulesInit();

    /* connect the interrupt events */
    App_EventsInit();

    /* commit the system configuration */
    status = WP_SysCommit();
    App_TerminateOnError(status, "WP_SysCommit",__LINE__);

    /* Pecs init */
    WPE_Pecs_Init(gpe_pecs_cfg, pecs_handles);

    /* create channels for RX */
    App_RxChannelsCreate();

    /* create the default IW system */
    App_DefaultIwSystemCreate();

    /* Build the system */
    App_SystemBuild();
    
    /* PCE interface create */
    App_PceInterfaceCreate();

    /* Generic Priority Mapping create*/
    App_DefaultGpmtCreate();
    
    /* semaphore and thread for learning */
    WPL_SemInit(&learning_sem);
    WPL_ThreadInit(&learning_tid, App_LearningThread, NULL);

#ifdef __linux__
            /* In WinMon verion, we do aging in simulated timer thread */
            /* In Linux version, we do it in seperately thread, and Linux system timer gives the command*/
            /* Init semaphore and thread for aging, Linux */
            WPL_SemInit(&aging_sem);
            WPL_ThreadInit(&aging_tid, App_AgingThread, NULL);
#endif

    /* CLI */
    WPL_LockKeyInit(WPL_THREAD_LOCK_KEY, &bridge_lock);

    /* Enable the system */
    //App_SystemEnable();
    OS_AgingTimerCreate();
    OS_AgingTimerStart();

    //set_default();
    
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
            /* max_iw_aggregations */ MAX_FLOW_AGG * MAX_IW_SYSTEM,
            /* max_r_iw_systems;*/ 0,
            /* max_directmap_enet_systems;*/ 0,
            /* max_b_iw_systems;*/ MAX_IW_SYSTEM + 1,
            /* max_m_iw_systems;*/ 0,
            /* max_vlan_priority_maps; */ 10,
            /* iw_bkgnd_fifo_size */0,
            /* cong_pt       */NULL,
            /* iw_host_limits */{64,0,0,64,0},
            /* mpls_config*/ {0,0},
            /* iw_modes;*/
            {
                    /*policer_mode;*/                      WP_IW_FLOW_POLICER,
                    /*statistics_bundle_stat_mode;*/       WP_IW_STATISTICS_BUNDLE_STAT_DISABLE,
                    /*l2_prefix_ext_mode;*/                WP_IW_L2_HEADER_EXTENSION_ENABLE,
                    /*enhanced_flow_stat_mode;*/           WP_IW_ENHANCED_FLOW_STAT_DISABLE,
                    /*flow_stat_mode;*/                    WP_IW_FLOW_STAT_ENABLE,
                    /*fr_tx_flow_stat_mode */              WP_IW_FR_TX_FLOW_STAT_DISABLE,
                    /*mfc_alloc_mode; */                   WP_IW_MFC_ALLOC_ENABLE,
                    /*Learning_queue_mode */               WP_IW_LEARNING_DISABLE,
                    /*port_filtering_mode */               WP_IW_PORT_FILTERING_DISABLE,
                    /* lpm_alloc_mode */                   WP_IW_LPM_IPV4_BRIDGE_ALLOC_ENABLE,
                    /*enhanced_mc_member_stat_mode*/       WP_IW_ENHANCED_MC_MEMBER_STAT_ENABLE,
                    /*routing_alloc_modes*/                WP_DISABLE,
                    /*res_mc_addresses_table_mode*/        WP_DISABLE,
                    /*port_stag_etype_mode*/               WP_IW_PORT_STAG_ETYPE_ENABLE
            },
            /* max_ch_agg_stat;*/ 0,
            /* max_fr_s_iw_systems;*/ 0,
            /*max_iw_ports;*/ IW_SYS_MAX_BPORTS * MAX_IW_SYSTEM,
            /* max_iw_mc_groups;*/MAX_BRIDGING_SYSTEM,
            /* max_nat_systems;*/0,
            /*max_iw_second_aggregations;*/0, /* secondary flow aggregations */
            /*iw_comp_limits;*/0,
            /* max_iw_mc_members;*/MAX_BRIDGING_SYSTEM*(MAX_PORT_PER_SYSTEM),
            /* max_iw_programmable_filters;*/MAX_IW_PROGRAMMABLE_FILTERS,
            /* max_iw_policers;*/0,
            /* max_dscpmt */ 0,
            /* max_wred_bindings */ 0,
            /* tcm_policer_limits */
            {
                    0,
                    0
            }

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
            {/* FIWT, QAT       */ WP_BUS_EXTERNAL_MEMORY5, 0},//WP_BUS_PARAM, APP_BANK_PARAM},
            {/* MFCPT, UPPPT, DS*/ WP_BUS_HOST, APP_BANK_HOST},
            {/* STATISTICS2     */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
            {/* STATISTICS3     */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
            {/* UNASSIGNED      */ WP_BUS_NONE, 0},
            {/* PKT_GLOBAL_TCPT */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
            {/* UNASSIGNED      */ WP_BUS_NONE, 0},
            {/* MANDATORY       */ WP_BUS_NONE, 0},
            {/* MANDATORY       */ WP_BUS_NONE, 0},
            /* ************* VIRTUAL BUS ASSIGNMENT ***************/
            {/* MCH_DATA        */ WP_BUS_PACKET, APP_BANK_PACKET}
            ,
            {/* WP_VB_OAM_FM_QUERY*/ WP_BUS_NONE, 0},
            {/* WP_VB_HASH_ON_HEAP*/ WP_BUS_NONE, 0},
            {/* WP_VB_MFC_INTERNAL*/ WP_BUS_PACKET, APP_BANK_PACKET},
            {/* WP_VB_MFR_RAS_TBL,           */ WP_BUS_NONE, 0},
            {/* WP_VB_CESOP_DATA,            */ WP_BUS_NONE, 0},
            {/* WP_VB_SECURITY_ESE,          */ WP_BUS_NONE, 0},
            {/* WP_VB_PDCP_SECURITY_ENTITY,  */ WP_BUS_NONE, 0},
            {/* WP_VB_POLICER,               */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
            {/* WP_VB_PCE_LRN,               */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
            {/* WP_VB_PCEA_PARAM,            */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
            {/* WP_VB_CONGESTION_TABLE,      */ WP_BUS_NONE, 0},
            /*WP_VB_PARAMS_PROPAGATION             */{WP_BUS_NONE, 0},
            /*WP_VB_STATS_IWPORT_PCE_FILTERS       */{WP_BUS_INTERNAL, APP_BANK_INTERNAL},

#if _WP_MODULE_CUSTOM_TIGERMILK_ENABLE_
             /*WP_VB_PROFILE_MARKING_STATS          */{WP_BUS_NONE, 0},
#endif
             /*WP_VB_TIMESTAMP_SERVICES_TABLE       */{WP_BUS_NONE, 0},
             /*WP_VB_POLICER_ACTION                 */{WP_BUS_INTERNAL, APP_BANK_INTERNAL},
             /* WP_VB_PECS_GLOBAL                   */{WP_BUS_INTERNAL, APP_BANK_INTERNAL},
             /* WP_VB_ENET_10G_LONG_PKT             */{WP_BUS_NONE, 0},
             /* WP_VB_G999_STREAM                   */{WP_BUS_NONE, 0},
             /* WP_VB_G999_STREAM_STATISTICS        */{WP_BUS_NONE, 0},
             /* WP_VB_FMU_FIFO_TO_CHANNEL_NUMBER    */{WP_BUS_NONE, 0},
             /* WP_VB_IPWORT_EXT                    */{WP_BUS_PARAM, APP_BANK_PARAM},
             /*WP_VB_IW_GPMT_TABLE                  */{WP_BUS_INTERNAL, APP_BANK_INTERNAL},//{WP_BUS_PARAM, APP_BANK_PARAM}
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
        App_TerminateOnError( status, "WP_DriverInit", __LINE__);

        /* initialize the ram partition, configure its pce rules's number */
#if POLICER_TYPE_INT        
        int_ram_partition.policer_entries = MAX_IW_POLICERS;
#else
        int_ram_partition.policer_entries = 0;
#endif
        int_ram_partition.pce_hw_rules_num = pce_init.hardware_rules_num;
        WT_SetIntRamPartition(WP_WINPATH(DEFAULT_WPID), &int_ram_partition);

        max_iw->lpm_bridging_limits.max_ipv4_bridging_flows = MAX_ROUTING_SYSTEM * 20;
        max_iw->lpm_bridging_limits.max_ipv4_lpm_bridging_iw_systems = MAX_ROUTING_SYSTEM;
        max_iw->lpm_max_ipv4_routing_flows = 0;
        max_iw->max_iw_gpmt_tables = MAX_GPMT_TABLE;
        max_iw->iw_modes.iw_characteristics |= WP_IW_MODES_GENERIC_PRIORITY_MAPPING_ENABLE;
        status = WP_SysInit(WP_WINPATH(DEFAULT_WPID), context);
        App_TerminateOnError(status, "WP_SysInit",__LINE__);

        return ;
}

/*****************************************************************************
 * Function name: App_TestInit
 * Description  : Initializes the global variables used in this application. 
 * Input  params: None
 * Output params: None
 * Return val   : None
 *****************************************************************************/
void App_TestInit(void)
{
    WP_U32    ii, jj;
    memset(system_num, 0, sizeof(system_num));
    memset(vpn_system, 0, sizeof(vpn_system));

    for (ii = 0; ii < MAX_IW_SYSTEM; ii++)
    {
        vpn_system[ii].index        = ii + 1;
    }

    for (ii = 0; ii < NR_GBE; ii++)
    {
        if (!WPE_HWPortIdValid(ii))
        {
            continue;
        }    
        hw_port[ii].index = ii + 1;
        hw_port[ii].type = PORT_TYPE_NA;
        hw_port[ii].qos.af.mode = AF_MODE_NA;

        for (jj = 0; jj < MAX_SUBPORT_PER_PORT; jj++)
        {
            hw_port[ii].subport[jj].index = jj + 1;
        }
    }

    memset(ing_policer, 0, sizeof(ing_policer));

    memset(vpn_port, 0, sizeof(vpn_port));
    for (ii = 0; ii < NUM_OF_VPN_PORT; ii++)
    {
        vpn_port[ii].index = ii + 1;
    }
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
#ifndef USE_SOCKET
    WP_status status;
    WP_U32    ii;

    /* board configure */
        
    switch (boardtype)
    {
        case BOARD_TYPE_4XGI:
            status = WPX_BoardConfigure(WP_WINPATH(DEFAULT_WPID), WPX_CONFIGURE_4XGI_2SGMII);
            App_TerminateOnError(status, "WPX_BoardConfigure()", __LINE__);            
        break;
        
        case BOARD_TYPE_2XGI_20SGMII:
            status = WPX_BoardConfigure(WP_WINPATH(DEFAULT_WPID), WPX_CONFIGURE_DEFAULT_CHECKIN);
            App_TerminateOnError(status, "WPX_BoardConfigure()", __LINE__);
        break;
        
        case BOARD_TYPE_8SGMII:
            status = WPX_BoardConfigure(WP_WINPATH(DEFAULT_WPID), WPX_CONFIGURE_DEFAULT_CHECKIN);
            App_TerminateOnError(status, "WPX_BoardConfigure()", __LINE__);
        break;

        case BOARD_TYPE_DEFAULT:
        default:
            status = WPX_BoardConfigure(WP_WINPATH(DEFAULT_WPID), WPX_CONFIGURE_DEFAULT_CHECKIN);
            App_TerminateOnError(status, "WPX_BoardConfigure()", __LINE__);
        break;
    }


    for (ii = 0; ii < NR_GBE; ii++)
    {
        if (PORT_HW_XGI == hw_port[ii].hw_type)
        {
            status = WPX_BoardXgiConfig(WP_WINPATH(DEFAULT_WPID), hw_port[ii].hw_port);
            App_TerminateOnError(status, "WPX_BoardXgiConfig()", __LINE__);
            
            status = WPX_BoardSerdesInit(WP_WINPATH(DEFAULT_WPID), hw_port[ii].hw_port, hw_port[ii].loopback); 
            App_TerminateOnError(status, "WPX_BoardSerdesInit",__LINE__);                
        }
        else
        if (PORT_HW_SGMII == hw_port[ii].hw_type)
        {
            status = WPX_BoardSerdesInit(WP_WINPATH(DEFAULT_WPID), hw_port[ii].hw_port, hw_port[ii].loopback);
            App_TerminateOnError(status, "WPX_BoardSerdesInit 3()", __LINE__);            
        }
    }


#endif
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

    WP_port_enet enet_port_config =
    {
        /* pkt_limits */
        {
            /* max_tx_channels*/ NUM_TX_CHANNELS * (MAX_SUBPORT_PER_PORT + 1),
            /* max_rx_channels*/ 1,
            /* pq_block_size*/   0,
            /* n_pq_blocks*/     0,
            /* emphy_devices*/   
        },
        /* flowmode */       WP_ENET_FMU_HIERARCHICAL_SHAPING_MODE,
        /* interface_mode */ WP_ENET_SGMII_1000,
        /* rx_iw_bkgnd */    0,       
    };

    WP_device_enet enet_dev_config =
    {
        /* max_tx_channels */ NUM_TX_CHANNELS * (MAX_SUBPORT_PER_PORT + 1),
        /* tx_maxsdu */       WT_MAX_FRAME_SIZE,
        /* operating_speed */ WP_UNUSED,
        /* mac_addr*/ {0xaa,0xaa,0xaa,0xaa,0xaa,0xaa},
        /* tx_bit_rate*/      WP_TX_BIT_RATE_UNLIMITED,
        /* loopbackmode */    WP_ENET_NORMAL,
        /* extended_params */ NULL 
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



    /* Create Giga enet port & device */
    for (ii = 0; ii < NR_GBE; ii++) 
    {
        if (PORT_HW_XGI == hw_port[ii].hw_type)
        {                
            enet_port_config.interface_mode = WP_ENET_XAUI;
        }
        else
        if (PORT_HW_SGMII == hw_port[ii].hw_type)
        {
            enet_port_config.interface_mode = WP_ENET_SGMII_1000;
        }
        else
        {
            continue;
        }

        hw_port[ii].port_enet = WP_PortCreate(WP_WINPATH(0), hw_port[ii].hw_port, &enet_port_config);
        App_TerminateOnError(hw_port[ii].port_enet, "WP_PortCreate() Fast ENET", __LINE__);

        enet_dev_config.extended_params = NULL;
        hw_port[ii].dev_enet = WP_DeviceCreate(hw_port[ii].port_enet, WP_PHY(0), WP_DEVICE_ENET, &enet_dev_config);
        App_TerminateOnError(hw_port[ii].dev_enet, "WP_DeviceCreate() Enet", __LINE__);

        // change to packet base mode
        enet_device_extended_config->tx_force_pb = WP_ENET_TX_USE_PB_DEFAULT;        
        enet_dev_config.extended_params = enet_device_extended_config;
        status = WP_DeviceModify(hw_port[ii].dev_enet,
                                 WP_DEV_MOD_ENET_TX_STATMODE |
                                 WP_DEV_MOD_ENET_RX_STATMODE |
                                 WP_DEV_MOD_ENET_PHYSTATMODE |
                                 WP_DEV_MOD_ENET_RX_FLOWCONTROL | 
                                 WP_DEV_MOD_ENET_TX_FLOWCONTROL,
                                 &enet_dev_config);
        App_TerminateOnError(status, "WP_DeviceModify() GBE",__LINE__);

    }

    /* Create Host ports and device */
    iwp = WP_PortCreate(WP_WINPATH(DEFAULT_WPID), WP_PORT_IW_HOST, NULL);
    App_TerminateOnError(iwp, "WP_PortCreate() IW Host",__LINE__);

    /* Create an IW Host Device */
    devhost = WP_DeviceCreate(iwp, 0, WP_DEVICE_IW_HOST, NULL);
    App_TerminateOnError(devhost, "WP_DeviceCreate() IW Host",__LINE__);


    return ;
}

#ifdef WP_HW_WINPATH4
void App_HierQnodeCreate(void)
{
   WP_U32 xgi_port, l2_group, l3_group;

   WP_fmu_shaping_wrr weight_params =
   {
      /*weight*/ 5000,
      /*flags*/0,
   };

   WP_qnode_iw_group group_highp =
   {
      /*num_of_bd_pools*/WTE_TMU_FBP_L3_PER_L2_NUM,
      /*bd_pool*/   {0,0,0,0},
      /*fifo_selection_method*/WP_QNODE_IW_SELECTION_WRR,
      /*bd_pool_weight*/ /*{NULL,NULL}*/{&weight_params,&weight_params, &weight_params, &weight_params}
   };

   WP_qnode_iw_hier iw_hierq =
   {
      /*num_of_groups*/WTE_TMU_FBP_L2_NUM,
      /*iw_group*/ {group_highp, group_highp, group_highp, group_highp},
      WP_QNODE_IW_SELECTION_STRICT,
//      /*group_selection_method*/ WP_QNODE_IW_SELECTION_WRR,
      /*group_weight*/{&weight_params, &weight_params, &weight_params, &weight_params}//TODO: Does the weight_params for l2 and l3 is the same
   };

   WP_U32 xgi_bd_bus[4][4] = {
      {WTE_BD_BUS1_XGI1, WTE_BD_BUS2_XGI1, WTE_BD_BUS3_XGI1, WTE_BD_BUS4_XGI1},
      {WTE_BD_BUS1_XGI2, WTE_BD_BUS2_XGI2, WTE_BD_BUS3_XGI2, WTE_BD_BUS4_XGI2},
      {WTE_BD_BUS1_XGI3, WTE_BD_BUS2_XGI3, WTE_BD_BUS3_XGI3, WTE_BD_BUS4_XGI3},
      {WTE_BD_BUS1_XGI4, WTE_BD_BUS2_XGI4, WTE_BD_BUS3_XGI4, WTE_BD_BUS4_XGI4}
   };
   WP_pool_iw_bd bd_conf[WPE_XGI_PORTS_NUM][WTE_TMU_FBP_L2_NUM];
   
   WP_U32 xgi_buffer_bus[4][4] = {
      {WTE_BUFFER_BUS1_XGI1, WTE_BUFFER_BUS2_XGI1, WTE_BUFFER_BUS3_XGI1, WTE_BUFFER_BUS4_XGI1},
      {WTE_BUFFER_BUS1_XGI2, WTE_BUFFER_BUS2_XGI2, WTE_BUFFER_BUS3_XGI2, WTE_BUFFER_BUS4_XGI2},
      {WTE_BUFFER_BUS1_XGI3, WTE_BUFFER_BUS2_XGI3, WTE_BUFFER_BUS3_XGI3, WTE_BUFFER_BUS4_XGI3},
      {WTE_BUFFER_BUS1_XGI4, WTE_BUFFER_BUS2_XGI4, WTE_BUFFER_BUS3_XGI4, WTE_BUFFER_BUS4_XGI4}
   };

   WP_U32 xgi_buffer_bank[4][4] = {
      {WTE_BUFFER_BANK1_XGI1, WTE_BUFFER_BANK2_XGI1, WTE_BUFFER_BANK3_XGI1, WTE_BUFFER_BANK4_XGI1},
      {WTE_BUFFER_BANK1_XGI2, WTE_BUFFER_BANK2_XGI2, WTE_BUFFER_BANK3_XGI2, WTE_BUFFER_BANK4_XGI2},
      {WTE_BUFFER_BANK1_XGI3, WTE_BUFFER_BANK2_XGI3, WTE_BUFFER_BANK3_XGI3, WTE_BUFFER_BANK4_XGI3},
      {WTE_BUFFER_BANK1_XGI4, WTE_BUFFER_BANK2_XGI4, WTE_BUFFER_BANK3_XGI4, WTE_BUFFER_BANK4_XGI4}
   };

   WP_pool_buffer_data buffer_data[WPE_XGI_PORTS_NUM][WTE_TMU_FBP_L3_PER_L2_NUM];

   for (xgi_port=0; xgi_port < WPE_XGI_PORTS_NUM; xgi_port++)
   {
      for (l2_group=0; l2_group < WTE_TMU_FBP_L2_NUM; l2_group++)
      {
         bd_conf[xgi_port][l2_group].num_buffer_descriptors = WTE_TMU_FBP_BUFFERS_BD_NUM;//TODO: what should be the ratio between BD number and buffers
         bd_conf[xgi_port][l2_group].buffer_pool = 0;
         bd_conf[xgi_port][l2_group].wmm_num_buffer_descriptors = 0;
         bd_conf[xgi_port][l2_group].bd_bus = xgi_bd_bus[xgi_port][l2_group];
      }
   }



   for (xgi_port=0; xgi_port < WPE_XGI_PORTS_NUM; xgi_port++)
   {
      for (l3_group=0; l3_group < WTE_TMU_FBP_L3_PER_L2_NUM; l3_group++)
      {
         buffer_data[xgi_port][l3_group].n_buffers = WTE_TMU_FBP_BUFFERS_NUM;
         buffer_data[xgi_port][l3_group].offset = 64;
         buffer_data[xgi_port][l3_group].size = WTI_BUFFER_SIZE;
         buffer_data[xgi_port][l3_group].pad = 0;
         buffer_data[xgi_port][l3_group].bus = xgi_buffer_bus[xgi_port][l3_group];
         buffer_data[xgi_port][l3_group].bank = xgi_buffer_bank[xgi_port][l3_group];

         buffer_pool[xgi_port][l3_group] = WP_PoolCreate(WP_WINPATH(DEFAULT_WPID),
                                                                  WP_POOL_IWBUFFER,
                                                                  &buffer_data[xgi_port][l3_group]);
         App_TerminateOnError(buffer_pool[xgi_port][l3_group], "WP_PoolCreate()",__LINE__);
      }
   }


   for (xgi_port = 0; xgi_port < WPE_XGI_PORTS_NUM; xgi_port++)
   {
      for (l2_group = 0; l2_group < WTE_TMU_FBP_L2_NUM; l2_group++)
      {
         for (l3_group = 0; l3_group < WTE_TMU_FBP_L3_PER_L2_NUM; l3_group++)
         {
            bd_conf[xgi_port][l2_group].buffer_pool = buffer_pool[xgi_port][l3_group];
            bd_pool[xgi_port][l2_group] = WP_PoolCreate(WP_WINPATH(DEFAULT_WPID),
                                                                 WP_POOL_IW_BD,
                                                                 &bd_conf[xgi_port][l2_group]);

            App_TerminateOnError(bd_pool[xgi_port][l2_group], "WP_PoolCreate()", __LINE__);

            group_highp.bd_pool[l3_group] = bd_pool[xgi_port][l2_group];

         }
         iw_hierq.iw_group[l2_group] = group_highp;
      }

      iw_hier_qnode[xgi_port] = WP_QNodeCreate(WP_WINPATH(DEFAULT_WPID),
                                                       WP_QNODE_IW_HIERARCHICAL,
                                                       &iw_hierq);
      App_TerminateOnError(iw_hier_qnode[xgi_port], "WP_QNodeCreate() qn_hier", __LINE__);
   }

}

void App_HostQNodeCreate(void)
{
   WP_pool_buffer_data pool_buffer_data =
   {
      /* n_buffers */ WTE_WMM_NUM_OF_BUFFERS,
      /* offset */ 64,
      /* size */ WT_BUFFER_SIZE,
      /* pad */ 0,
      /* bus */ WTE_WMM_BUFFERS_BUS,
      /* bank */ WTE_WMM_BUFFERS_BANK
   };

   WP_qnode_iwq qnode_iwq =
   {
      /* interruptqueue */ 0,
      /* num_buffers */ WTE_WMM_NUM_OF_BUFFERS,
      /* adjunct_pool */ 0
   };
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


   TEST_pool = WP_PoolCreate(WP_WINPATH(DEFAULT_WPID),
                                    WP_POOL_IWBUFFER,
                                    &pool_buffer_data);
   App_TerminateOnError(TEST_pool, "WP_PoolCreate", __LINE__);

   qnode_iwq.adjunct_pool = TEST_pool;

   qniw = WP_QNodeCreate(WP_WINPATH(DEFAULT_WPID),
                                       WP_QNODE_IWQ | WP_QNODE_OPT_FMU,
                                       &qnode_iwq);
   App_TerminateOnError(qniw, "WP_QNodeCreate IW_ENETQ", __LINE__);

   /* create Qnode */
   qniw_mc = WP_QNodeCreate(WP_WINPATH(DEFAULT_WPID), WP_QNODE_IW_MCQ, iw_mc_qn_config);
   App_TerminateOnError(qniw_mc, "WP_QNodeCreate() qniw_mc", __LINE__);

}




void App_BufferPoolsCreate(void)
{
    App_HostQNodeCreate();
    App_HierQnodeCreate();
}

#else
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
    WP_U32 xgi_port;

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
                                /* offset */ 64,
                                /* size */ 1984, /* RBS */
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
        App_TerminateOnError(bd_pool[1], "WP_PoolCreate() bd_pool", __LINE__);


        bd_conf->buffer_pool = pool_2048_2;
        bd_conf->bd_bus = WP_BUS_PARAM;
        bd_pool[2] = WP_PoolCreate(WP_WINPATH(DEFAULT_WPID),WP_POOL_IW_BD, bd_conf);
        App_TerminateOnError(bd_pool[2], "WP_PoolCreate() bd_pool", __LINE__);

        bd_conf->buffer_pool = pool_2048_3;
        bd_conf->bd_bus = WP_BUS_PARAM;
        bd_pool[3] = WP_PoolCreate(WP_WINPATH(DEFAULT_WPID),WP_POOL_IW_BD, bd_conf);
        App_TerminateOnError(bd_pool[3], "WP_PoolCreate() bd_pool", __LINE__);


        /*Configure and create Hierarchical Qnode*/
        group_highp[0].bd_pool[0] = bd_pool[0];
        group_highp[0].bd_pool[1] = bd_pool[1];

        group_lowp[0].bd_pool[0] = bd_pool[2];
        group_lowp[0].bd_pool[1] = bd_pool[3];

        iw_hierq->iw_group[0] = group_highp[0];
        iw_hierq->iw_group[1] = group_lowp[0];

        qniw = WP_QNodeCreate(WP_WINPATH(DEFAULT_WPID), WP_QNODE_IW_HIERARCHICAL,iw_hierq);
        App_TerminateOnError(qniw, "WP_QNodeCreate()",__LINE__);

   /* create Qnode */
   qniw_mc = WP_QNodeCreate(WP_WINPATH(DEFAULT_WPID), WP_QNODE_IW_MCQ, iw_mc_qn_config);
   App_TerminateOnError(qniw_mc, "WP_QNodeCreate() qniw_mc", __LINE__);


    for (xgi_port = 0; xgi_port < WPE_XGI_PORTS_NUM; xgi_port++)
    {    
       iw_hier_qnode[xgi_port] = qniw;
    }

}

#endif


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
    

            WP_policer_limits_v2 v2_policer_limits[1]=
                    {
                            {
#if POLICER_TYPE_INT       
                                    /* max_internal_policers*/NUM_OF_POLICER,
                                    /* max_external_policers*/0,
    
#else
                                    /* max_internal_policers*/0,
                                    /* max_external_policers*/NUM_OF_POLICER,
#endif
                                    /* max_policer_actions*/NUM_OF_POLICER
                            }
                    };

    WP_module_policer policer_module_config[1]=
    {
       {
          /*v2_policer_limits*/v2_policer_limits,
          /*v1_policer_limits*/0,
          /*atm_policer_limits*/0,
          /*mode*/WP_POLICER_STATS_ENABLE
       }
    };

    /* PCE module initialization */
    pce_init.parser_info = &pce_parser_info;
    status = WP_ModuleInit(WP_SYSHANDLE(DEFAULT_WPID), WP_WDDI_MODULE_PCE, &pce_init);
    App_TerminateOnError(status, "WPI_PceModuleInit()",__LINE__);

    /* GPE module initialization */
    status = WP_ModuleInit(WP_SYSHANDLE(DEFAULT_WPID), WP_WDDI_MODULE_GPE, &gpe_init);
    App_TerminateOnError(status, "WPI_GpeModuleInit()", __LINE__);

    status = WP_ModuleInit(WP_SYSHANDLE(DEFAULT_WPID), WP_WDDI_MODULE_POLICER, policer_module_config);
    App_TerminateOnError(status, "WPI_PolicerModuleInit", __LINE__);

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
 * Function name: App_ChannelsCreate
 * Description  : This function creates the Enet channels
 * Input  params: None
 * Output params: None
 * Return val   : None
 *****************************************************************************/   
void App_RxChannelsCreate(void)
{
    WP_U16 ii;
    WP_handle qnode;

    WP_ch_enet enet_ch_cfg =
    {
        /* intmode */         WP_PKTCH_INT_DISABLE,
        /* iwmode */          WP_PKTCH_IWM_ENABLE,
        /* testmode */        WP_PKTCH_TEST_DISABLE,
        /* tx_pqblock */      0,
        /* tx_pqlevel */      0,
        /* tx_shaping_type*/  WP_FMU_SHAPING_TYPE_STRICT,
        /* tx_shaping_params*/NULL,
        /* rx_maxsdu */       WT_MAX_FRAME_SIZE,
        /* tx_cwid */         WP_CW_ID_A,
        /* tx_tq */           0,
        /* rx_queuedepth */   50,
    };
    

    /* Channels for RX */
    for (ii = 0; ii < NR_GBE; ii++)
    {
        if (PORT_HW_XGI == hw_port[ii].hw_type)
        {                
            qnode = iw_hier_qnode[ii]; // hier qnode
        }
        else
        if (PORT_HW_SGMII == hw_port[ii].hw_type)
        {                    
            qnode = (ii & 1) ? iw_hier_qnode[2] : iw_hier_qnode[3]; // hier qnode                    
        }
        else
        {
            continue;
        }  
        // rx channel
        hw_port[ii].rx_channel = WP_ChannelCreate(0, hw_port[ii].dev_enet, qniw, WP_CH_RX, WP_ENET, &enet_ch_cfg);
        App_TerminateOnError(hw_port[ii].rx_channel, "WP_Channel_Create() Fast RX", __LINE__);
    }

    return ;
}


void App_HostCreate(WT_VsiSystemInfo* pSystem)
{
    WP_handle temp1, temp2, temp3;
    WP_tx_binding  tx_binding_iw[1]=
    {
        {
            /* res0;*/ 0,
            /* dci_mode;*/  WP_IW_DYN_CH_INSERT_DISABLE,
            /* maxt;*/1000,
        }
    };
    WP_ch_iw_rx ch_config_iw[1]=
    {
        {
            /* pqblock */ 0,
            /* pqlevel */ 0,
            /* tx_binding_type */ WP_IW_TX_BINDING,
            /* tx_binding_config */ tx_binding_iw
        }
    };
    WP_iw_agg_bridging br_agg_gbe[1] =
    {
        {
            /*tag*/ 2,
            /*txfunc*/ 0,
            /*input_bport*/0,
            /*rfcs*/WP_IW_RFCS_ENABLE,
            /*l2_header_insert_mode;*/WP_IW_L2H_INSERT_DISABLE,
            /*vlan_tag_mode*/WP_IW_VLAN_TAG_ENABLE,
            /*interruptqueue;*/WP_IW_IRQT1,
            /*error_pkt_mode*/WP_IW_ERRPKT_DISCARD,
            /*intmode;*/WP_IW_INT_DISABLE,
            /*statmode;*/WP_IW_STAT_ENABLE,
            /*timestamp_mode;*/WP_IW_TIME_STAMP_DISABLE,
            /*ov_pool_mode */ WP_IW_OV_POOL_DISABLE,
            /*fbp_drop_threshold;*/0,
            /*replace_vlan_id*/WP_IW_REPLACE_VTAG_DISABLE,
            /*vlan_id*/0x05,
            /*vpmt_handle */0,
            /*mtu;*/WT_MAX_FRAME_SIZE,
            /*prefix_length */ 0,
            /*prefix_header[28];*/ {0x1,0x2,0x3,0x4,0x5,0x6,0x7,
                                    0x8,0x9,0x10,0x11,0x12,0x13,0x14,
                                    0x15,0x16,0x17,0x18,0x19,0x20,0x21,
                                    0x22,0x23,0x24,0,0,0,0},
            /*policer_enable*/ WP_IW_POLICER_DISABLE,
            /**policer_config;*/NULL,
            /*cong_mode;*/WP_IW_CONGESTION_DISABLE,
            /**cong_threshold_param;*/NULL
        }
    };
    WP_bridge_port iwhost_bport_config[1] =
    {
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
                /* vlan_tunnel*/WP_IW_VLAN_TUNNEL_1Q_IN_1Q_DISABLE,
                /* vlan_priority_enforce_mode*/WP_IW_VLAN_PRIORITY_ENFORCE_DISABLED,
                /* stag_vlan_etype*/ WP_IWPORT_STAG_DISABLED
            },
            /*max_mac_addresses */ 34,
            /*group_tag */ 0,
            /*group_filtering_mode */ WP_IW_GROUP_FILTER_DISABLE,
            /*unk_mac_sa_filter*/ 0,
            /*unk_mc_mode;*/0,
            /*bc_ht_mode;*/ 0,
            /*input_filters_mask*/ 0,
            /*output_filters_mask;*/ 0,
            /*statmode*/ WP_IW_PORT_STAT_ENABLE
        }
    };

    /* create host rx channel */

    {
        temp1 = WP_ChannelCreate(0x1111,
                                          devhost,
                                          qniw,
                                          WP_CH_RX,
                                          WP_IW_HOST,
                                          &ch_config_iw[0]);
        App_TerminateOnError(temp1, "WP_ChannelCreate() Host sys",__LINE__);
        pSystem->host_rx_channel = temp1;

        /* create host flow aggregation */
        br_agg_gbe->txfunc = temp1;
        br_agg_gbe[0].tag = 2;
        temp2 =  WP_IwFlowAggregationCreate(WP_WINPATH(DEFAULT_WPID),
                                                       WP_IW_VLAN_AWARE_BRIDGING_MODE,
                                                       &br_agg_gbe[0]);
        App_TerminateOnError(temp2, "WP_IwFlowAggregationCreate()",__LINE__);
        pSystem->default_flow_aggregation = temp2;

        /* create bridging port */
        iwhost_bport_config->flow_agg = temp2;
        iwhost_bport_config[0].tag = 0x2000 + pSystem->index;
        temp3 = WP_IwPortCreate(pSystem->iw_system, &iwhost_bport_config[0]);
        App_TerminateOnError(temp3," WP_IwPortCreate()",__LINE__);
        pSystem->host_bport = temp3;
    }

}


WP_handle App_PortsCreate(WP_handle iw_sys, WP_handle flow_agg, WP_handle *iwport, WP_U16 defvlan, WP_U32 tag)
{

    if (*iwport)
    {
        return *iwport;
    }
 
    bport_conf.flow_agg = flow_agg;
    bport_conf.tag = tag;
    bport_conf.max_mac_addresses = max_mac_addresses;
    bport_conf.vlan_param.vlan_tag = defvlan;

    *iwport = WP_IwPortCreate(iw_sys, &bport_conf);// direct map
    App_TerminateOnError(*iwport, "WP_IwPortCreate ",__LINE__);

    return *iwport;
}


void App_BridgePortsCreate(void)
{
    WP_U32 ii;

    for (ii = 0; ii < NR_GBE; ii++)
    {
        if (!WPE_HWPortIdValid(ii))
        {
            continue;
        }    
        App_PortsCreate(common_system.iw_system, common_system.default_flow_aggregation, &hw_port[ii].bport_enet, ii, ii + 1);
    }
}	


WP_handle App_TxAggCreate(WP_handle txfunc, WP_handle iw_port)
{
    WP_handle flowagg;
    WP_iw_agg_generic dl_tx_agg[1] =
    {
        {
            /*tag*/ 2,
            /*txfunc*/ 0,
            /*iw_port*/0,
            /*rfcs*/WP_IW_RFCS_ENABLE,
            /*interruptqueue;*/WP_IW_IRQT1,
            /*error_pkt_mode*/WP_IW_ERRPKT_DISCARD,
            /*intmode;*/WP_IW_INT_DISABLE,
            /*statmode;*/WP_IW_STAT_ENABLE,
            /*timestamp_mode;*/WP_IW_TIME_STAMP_DISABLE,
            /*mtu;*/MAX_MTU,
            /*flow_agg_type;*/WP_IW_FLOW_AGG_PRIMARY,
            /*policer_handle;*/0,
            /*pecs_handle;*/0,
            /*pecs_flow_info;*/0,
            /*pecs_global_info_handle;*/0,
        },
    };

    WPE_gpe_brouter_pecs_flow_info brouter_pecs_flow[] = 
    {
        /*  */
        {
            WPE_GPE_BROUTER_PECS_EXTRACT_DISABLE, /* header_extract_mode */
            0, /* extraction_size */
            WPE_GPE_BROUTER_PECS_ADD_DISABLE, /* prefix_add_mode */
            4, /* prefix_add_size */
            14, /* prefix_ip_offset */
            WPE_GPE_BROUTER_PECS_REPLACE_MAC_DISABLE, /* mac_replace_mode */
            {0x11, 0x22, 0x33, 0x44, 0x55, 0x00}, /* mac_da */
            {0x66, 0x77, 0x88, 0x99, 0xaa, 0x00}, /* mac_sa */
            0, /* vlan_edit_options*/


            {0}, /* prefix_remark_options*/
            WPE_GPE_BROUTER_PECS_TTL_DISABLE, /* ttl_mode */
            WPE_GPE_BROUTER_PECS_TOS_REMARKING_DISABLE, /* tos_remarking_mode */
            {/* prefix */
                0x11, 0x22, 0x33, 0x44, 0x55, 0x00,
                0x11, 0x12, 0x13, 0x14, 0x17, 0x00,
                0x81, 0, 0, 1, 
                0x88,0x47,
                0x11,0x11, 0x10, 0x80,
                0x22,0x22, 0x21, 0x80,
                0,0,0,0,0,0,0,0, 
                0,0,0,0,0,0,0,0,
                0,0,0,0,0,0,0,0, 
                0,0,0,0,0,0,0,0,
                0,0,0,0,0,0,0,0
            }  
        }
    };

    /* DL GE TX flow agg */


    dl_tx_agg_gbe->txfunc = txfunc;
    dl_tx_agg_gbe->iw_port = iw_port;
    dl_tx_agg_gbe->pecs_handle = pecs_handles[0];
    dl_tx_agg_gbe->pecs_flow_info=(void *) &brouter_pecs_flow[0];
    flowagg =  WP_IwFlowAggregationCreate(WP_WINPATH(DEFAULT_WPID),
                                                 WP_IW_GENERIC_MODE,
                                                 &dl_tx_agg[0]);
    App_TerminateOnError(flowagg, "WP_IwFlowAggregationCreate()",__LINE__);


    return flowagg;
  
}

/*****************************************************************************
 * Function name: App_IwSystemInit
 * Description  : 
 * Input  params: None
 * Output params: None
 * Return val   : None
 *****************************************************************************/   
void App_DefaultIwSystemCreate(void)
{
    WP_handle handle;

    WP_iw_sys_bridging iw_sys_bridging[1]=
    {
        {
            /* tag */                   0,
            /* max_flows; */            MAX_FLOW_AGG,
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
            /*stag_ether_type*/         WP_IW_SYS_BRIDGE_STAG_VLAN_ETYPE_DISABLE/*DEFAULT VALUE*/,
        }
    };	

    {
        /* Create interworking system */
        iw_sys_bridging[0].tag = 2222;
        handle = WP_IwSystemCreate(WP_WINPATH(DEFAULT_WPID), WP_IW_VLAN_AWARE_BRIDGING_MODE, &iw_sys_bridging[0]);
        App_TerminateOnError(handle, "WP_IwSystemCreate",__LINE__);
        common_system.iw_system = handle;
        common_system.valid = 1;
    }
    
    // Create host handler, host agg, host iw port
    App_HostCreate(&common_system);



}

void App_DefaultGpmtCreate(void)
{
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].num_of_priority_group_entries = 8;
    // index 0
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[0].green.action_mode = WP_IW_GPMT_PRIORITY_MAPPING | WP_IW_GPMT_PREFIX_REMARKING;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[0].green.priority_mapping_value = UNI_Qos_mapping[0];
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[0].green.prefix_remarking_data.remarking_type[0] = WP_IW_GPMT_REMARKING_TYPE_PREFIX_REMARKING_VAL;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[0].green.prefix_remarking_data.value[0] = 0x1;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[0].green.prefix_remarking_data.remarking_type[1] = WP_IW_GPMT_REMARKING_TYPE_PREFIX_MPLS_EXP;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[0].green.prefix_remarking_data.value[1] = 0;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[0].green.prefix_remarking_data.remarking_type[2] = WP_IW_GPMT_REMARKING_TYPE_DO_NOT_CHANGE;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[0].green.prefix_remarking_data.value[2] = 0;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[0].green.prefix_remarking_data.remarking_type[3] = WP_IW_GPMT_REMARKING_TYPE_DO_NOT_CHANGE;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[0].green.prefix_remarking_data.value[3] = 0;


    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[0].yellow.action_mode = WP_IW_GPMT_PRIORITY_MAPPING | WP_IW_GPMT_PREFIX_REMARKING;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[0].yellow.priority_mapping_value = UNI_Qos_mapping[0];
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[0].yellow.prefix_remarking_data.remarking_type[0] = WP_IW_GPMT_REMARKING_TYPE_PREFIX_REMARKING_VAL;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[0].yellow.prefix_remarking_data.value[0] = 0x1;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[0].yellow.prefix_remarking_data.remarking_type[1] = WP_IW_GPMT_REMARKING_TYPE_PREFIX_MPLS_EXP;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[0].yellow.prefix_remarking_data.value[1] = 0;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[0].yellow.prefix_remarking_data.remarking_type[2] = WP_IW_GPMT_REMARKING_TYPE_DO_NOT_CHANGE;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[0].yellow.prefix_remarking_data.value[2] = 0x0;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[0].yellow.prefix_remarking_data.remarking_type[3] = WP_IW_GPMT_REMARKING_TYPE_DO_NOT_CHANGE;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[0].yellow.prefix_remarking_data.value[3] = 0x0;


    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[0].red.action_mode = WP_IW_GPMT_PRIORITY_MAPPING | WP_IW_GPMT_PREFIX_REMARKING;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[0].red.priority_mapping_value = UNI_Qos_mapping[0];
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[0].red.prefix_remarking_data.remarking_type[0] = WP_IW_GPMT_REMARKING_TYPE_PREFIX_REMARKING_VAL;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[0].red.prefix_remarking_data.value[0] = 0x1;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[0].red.prefix_remarking_data.remarking_type[1] = WP_IW_GPMT_REMARKING_TYPE_PREFIX_MPLS_EXP;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[0].red.prefix_remarking_data.value[1] = 0;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[0].red.prefix_remarking_data.remarking_type[2] = WP_IW_GPMT_REMARKING_TYPE_DO_NOT_CHANGE;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[0].red.prefix_remarking_data.value[2] = 0x0;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[0].red.prefix_remarking_data.remarking_type[3] = WP_IW_GPMT_REMARKING_TYPE_DO_NOT_CHANGE;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[0].red.prefix_remarking_data.value[3] = 0x0;


    // index 1
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[1].green.action_mode = WP_IW_GPMT_PRIORITY_MAPPING | WP_IW_GPMT_PREFIX_REMARKING;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[1].green.priority_mapping_value = UNI_Qos_mapping[1];
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[1].green.prefix_remarking_data.remarking_type[0] = WP_IW_GPMT_REMARKING_TYPE_PREFIX_REMARKING_VAL;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[1].green.prefix_remarking_data.value[0] = 0x2;// 0x1 << 1;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[1].green.prefix_remarking_data.remarking_type[1] = WP_IW_GPMT_REMARKING_TYPE_PREFIX_MPLS_EXP;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[1].green.prefix_remarking_data.value[1] = 2;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[1].green.prefix_remarking_data.remarking_type[2] = WP_IW_GPMT_REMARKING_TYPE_DO_NOT_CHANGE;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[1].green.prefix_remarking_data.value[2] = 0;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[1].green.prefix_remarking_data.remarking_type[3] = WP_IW_GPMT_REMARKING_TYPE_DO_NOT_CHANGE;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[1].green.prefix_remarking_data.value[3] = 0;

    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[1].yellow.action_mode = WP_IW_GPMT_PRIORITY_MAPPING | WP_IW_GPMT_PREFIX_REMARKING;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[1].yellow.priority_mapping_value = UNI_Qos_mapping[1];
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[1].yellow.prefix_remarking_data.remarking_type[0] = WP_IW_GPMT_REMARKING_TYPE_PREFIX_REMARKING_VAL;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[1].yellow.prefix_remarking_data.value[0] = 0x3;// (0x1 << 1) | 0x1;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[1].yellow.prefix_remarking_data.remarking_type[1] = WP_IW_GPMT_REMARKING_TYPE_PREFIX_MPLS_EXP;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[1].yellow.prefix_remarking_data.value[1] = 1;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[1].yellow.prefix_remarking_data.remarking_type[2] = WP_IW_GPMT_REMARKING_TYPE_DO_NOT_CHANGE;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[1].yellow.prefix_remarking_data.value[2] = 0;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[1].yellow.prefix_remarking_data.remarking_type[3] = WP_IW_GPMT_REMARKING_TYPE_DO_NOT_CHANGE;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[1].yellow.prefix_remarking_data.value[3] = 0;

    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[1].red.action_mode = WP_IW_GPMT_PRIORITY_MAPPING;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[1].red.priority_mapping_value = UNI_Qos_mapping[1];
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[1].red.prefix_remarking_data.remarking_type[0] = WP_IW_GPMT_REMARKING_TYPE_DO_NOT_CHANGE;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[1].red.prefix_remarking_data.value[0] = 0;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[1].red.prefix_remarking_data.remarking_type[1] = WP_IW_GPMT_REMARKING_TYPE_DO_NOT_CHANGE;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[1].red.prefix_remarking_data.value[1] = 0;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[1].red.prefix_remarking_data.remarking_type[2] = WP_IW_GPMT_REMARKING_TYPE_DO_NOT_CHANGE;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[1].red.prefix_remarking_data.value[2] = 0;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[1].red.prefix_remarking_data.remarking_type[3] = WP_IW_GPMT_REMARKING_TYPE_DO_NOT_CHANGE;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[1].red.prefix_remarking_data.value[3] = 0;


    // index 2
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[2].green.action_mode = WP_IW_GPMT_PRIORITY_MAPPING | WP_IW_GPMT_PREFIX_REMARKING;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[2].green.priority_mapping_value = UNI_Qos_mapping[2];
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[2].green.prefix_remarking_data.remarking_type[0] = WP_IW_GPMT_REMARKING_TYPE_PREFIX_REMARKING_VAL;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[2].green.prefix_remarking_data.value[0] = 4;// 0x2 << 1;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[2].green.prefix_remarking_data.remarking_type[1] = WP_IW_GPMT_REMARKING_TYPE_PREFIX_MPLS_EXP;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[2].green.prefix_remarking_data.value[1] = 2;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[2].green.prefix_remarking_data.remarking_type[2] = WP_IW_GPMT_REMARKING_TYPE_DO_NOT_CHANGE;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[2].green.prefix_remarking_data.value[2] = 0;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[2].green.prefix_remarking_data.remarking_type[3] = WP_IW_GPMT_REMARKING_TYPE_DO_NOT_CHANGE;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[2].green.prefix_remarking_data.value[3] = 0;

    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[2].yellow.action_mode = WP_IW_GPMT_PRIORITY_MAPPING | WP_IW_GPMT_PREFIX_REMARKING;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[2].yellow.priority_mapping_value = UNI_Qos_mapping[2];
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[2].yellow.prefix_remarking_data.remarking_type[0] = WP_IW_GPMT_REMARKING_TYPE_PREFIX_REMARKING_VAL;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[2].yellow.prefix_remarking_data.value[0] = 0x5;// (0x2 << 1) | 0x1;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[2].yellow.prefix_remarking_data.remarking_type[1] = WP_IW_GPMT_REMARKING_TYPE_PREFIX_MPLS_EXP;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[2].yellow.prefix_remarking_data.value[1] = 1;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[2].yellow.prefix_remarking_data.remarking_type[2] = WP_IW_GPMT_REMARKING_TYPE_DO_NOT_CHANGE;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[2].yellow.prefix_remarking_data.value[2] = 0;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[2].yellow.prefix_remarking_data.remarking_type[3] = WP_IW_GPMT_REMARKING_TYPE_DO_NOT_CHANGE;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[2].yellow.prefix_remarking_data.value[3] = 0;

    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[2].red.action_mode = WP_IW_GPMT_PRIORITY_MAPPING;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[2].red.priority_mapping_value = UNI_Qos_mapping[2];
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[2].red.prefix_remarking_data.remarking_type[0] = WP_IW_GPMT_REMARKING_TYPE_DO_NOT_CHANGE;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[2].red.prefix_remarking_data.value[0] = 0;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[2].red.prefix_remarking_data.remarking_type[1] = WP_IW_GPMT_REMARKING_TYPE_DO_NOT_CHANGE;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[2].red.prefix_remarking_data.value[1] = 0;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[2].red.prefix_remarking_data.remarking_type[2] = WP_IW_GPMT_REMARKING_TYPE_DO_NOT_CHANGE;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[2].red.prefix_remarking_data.value[2] = 0;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[2].red.prefix_remarking_data.remarking_type[3] = WP_IW_GPMT_REMARKING_TYPE_DO_NOT_CHANGE;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[2].red.prefix_remarking_data.value[3] = 0;

    // index 3
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[3].green.action_mode = WP_IW_GPMT_PRIORITY_MAPPING | WP_IW_GPMT_PREFIX_REMARKING;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[3].green.priority_mapping_value = UNI_Qos_mapping[3];
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[3].green.prefix_remarking_data.remarking_type[0] = WP_IW_GPMT_REMARKING_TYPE_PREFIX_REMARKING_VAL;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[3].green.prefix_remarking_data.value[0] = 6;// 3 << 1;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[3].green.prefix_remarking_data.remarking_type[1] = WP_IW_GPMT_REMARKING_TYPE_PREFIX_MPLS_EXP;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[3].green.prefix_remarking_data.value[1] = 2;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[3].green.prefix_remarking_data.remarking_type[2] = WP_IW_GPMT_REMARKING_TYPE_DO_NOT_CHANGE;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[3].green.prefix_remarking_data.value[2] = 0;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[3].green.prefix_remarking_data.remarking_type[3] = WP_IW_GPMT_REMARKING_TYPE_DO_NOT_CHANGE;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[3].green.prefix_remarking_data.value[3] = 0;

    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[3].yellow.action_mode = WP_IW_GPMT_PRIORITY_MAPPING | WP_IW_GPMT_PREFIX_REMARKING;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[3].yellow.priority_mapping_value = UNI_Qos_mapping[3];
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[3].yellow.prefix_remarking_data.remarking_type[0] = WP_IW_GPMT_REMARKING_TYPE_PREFIX_REMARKING_VAL;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[3].yellow.prefix_remarking_data.value[0] = 0x7;// (0x3 << 1) | 0x1;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[3].yellow.prefix_remarking_data.remarking_type[1] = WP_IW_GPMT_REMARKING_TYPE_PREFIX_MPLS_EXP;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[3].yellow.prefix_remarking_data.value[1] = 1;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[3].yellow.prefix_remarking_data.remarking_type[2] = WP_IW_GPMT_REMARKING_TYPE_DO_NOT_CHANGE;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[3].yellow.prefix_remarking_data.value[2] = 0;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[3].yellow.prefix_remarking_data.remarking_type[3] = WP_IW_GPMT_REMARKING_TYPE_DO_NOT_CHANGE;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[3].yellow.prefix_remarking_data.value[3] = 0;

    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[3].red.action_mode = WP_IW_GPMT_PRIORITY_MAPPING;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[3].red.priority_mapping_value = UNI_Qos_mapping[3];
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[3].red.prefix_remarking_data.remarking_type[0] = WP_IW_GPMT_REMARKING_TYPE_DO_NOT_CHANGE;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[3].red.prefix_remarking_data.value[0] = 0;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[3].red.prefix_remarking_data.remarking_type[1] = WP_IW_GPMT_REMARKING_TYPE_DO_NOT_CHANGE;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[3].red.prefix_remarking_data.value[1] = 0;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[3].red.prefix_remarking_data.remarking_type[2] = WP_IW_GPMT_REMARKING_TYPE_DO_NOT_CHANGE;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[3].red.prefix_remarking_data.value[2] = 0;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[3].red.prefix_remarking_data.remarking_type[3] = WP_IW_GPMT_REMARKING_TYPE_DO_NOT_CHANGE;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[3].red.prefix_remarking_data.value[3] = 0;


    // index 4
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[4].green.action_mode = WP_IW_GPMT_PRIORITY_MAPPING | WP_IW_GPMT_PREFIX_REMARKING;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[4].green.priority_mapping_value = UNI_Qos_mapping[4];
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[4].green.prefix_remarking_data.remarking_type[0] = WP_IW_GPMT_REMARKING_TYPE_PREFIX_REMARKING_VAL;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[4].green.prefix_remarking_data.value[0] = 8;    // 4 << 1
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[4].green.prefix_remarking_data.remarking_type[1] = WP_IW_GPMT_REMARKING_TYPE_PREFIX_MPLS_EXP;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[4].green.prefix_remarking_data.value[1] = 4;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[4].green.prefix_remarking_data.remarking_type[2] = WP_IW_GPMT_REMARKING_TYPE_DO_NOT_CHANGE;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[4].green.prefix_remarking_data.value[2] = 0;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[4].green.prefix_remarking_data.remarking_type[3] = WP_IW_GPMT_REMARKING_TYPE_DO_NOT_CHANGE;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[4].green.prefix_remarking_data.value[3] = 0;

    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[4].yellow.action_mode = WP_IW_GPMT_PRIORITY_MAPPING | WP_IW_GPMT_PREFIX_REMARKING;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[4].yellow.priority_mapping_value = UNI_Qos_mapping[4];
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[4].yellow.prefix_remarking_data.remarking_type[0] = WP_IW_GPMT_REMARKING_TYPE_PREFIX_REMARKING_VAL;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[4].yellow.prefix_remarking_data.value[0] = 9;    // 4 << 1 | 0x1
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[4].yellow.prefix_remarking_data.remarking_type[1] = WP_IW_GPMT_REMARKING_TYPE_PREFIX_MPLS_EXP;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[4].yellow.prefix_remarking_data.value[1] = 3;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[4].yellow.prefix_remarking_data.remarking_type[2] = WP_IW_GPMT_REMARKING_TYPE_DO_NOT_CHANGE;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[4].yellow.prefix_remarking_data.value[2] = 0;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[4].yellow.prefix_remarking_data.remarking_type[3] = WP_IW_GPMT_REMARKING_TYPE_DO_NOT_CHANGE;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[4].yellow.prefix_remarking_data.value[3] = 0;

    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[4].red.action_mode = WP_IW_GPMT_PRIORITY_MAPPING;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[4].red.priority_mapping_value = UNI_Qos_mapping[4];
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[4].red.prefix_remarking_data.remarking_type[0] = WP_IW_GPMT_REMARKING_TYPE_DO_NOT_CHANGE;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[4].red.prefix_remarking_data.value[0] = 0;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[4].red.prefix_remarking_data.remarking_type[1] = WP_IW_GPMT_REMARKING_TYPE_DO_NOT_CHANGE;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[4].red.prefix_remarking_data.value[1] = 0;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[4].red.prefix_remarking_data.remarking_type[2] = WP_IW_GPMT_REMARKING_TYPE_DO_NOT_CHANGE;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[4].red.prefix_remarking_data.value[2] = 0;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[4].red.prefix_remarking_data.remarking_type[3] = WP_IW_GPMT_REMARKING_TYPE_DO_NOT_CHANGE;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[4].red.prefix_remarking_data.value[3] = 0;

    // index 5
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[5].green.action_mode = WP_IW_GPMT_PRIORITY_MAPPING | WP_IW_GPMT_PREFIX_REMARKING;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[5].green.priority_mapping_value = UNI_Qos_mapping[5];
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[5].green.prefix_remarking_data.remarking_type[0] = WP_IW_GPMT_REMARKING_TYPE_PREFIX_VLAN_PRIO;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[5].green.prefix_remarking_data.value[0] = 5;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[5].green.prefix_remarking_data.remarking_type[1] = WP_IW_GPMT_REMARKING_TYPE_PREFIX_MPLS_EXP;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[5].green.prefix_remarking_data.value[1] = 5;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[5].green.prefix_remarking_data.remarking_type[2] = WP_IW_GPMT_REMARKING_TYPE_DO_NOT_CHANGE;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[5].green.prefix_remarking_data.value[2] = 0;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[5].green.prefix_remarking_data.remarking_type[3] = WP_IW_GPMT_REMARKING_TYPE_DO_NOT_CHANGE;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[5].green.prefix_remarking_data.value[3] = 0;

    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[5].yellow.action_mode = WP_IW_GPMT_PRIORITY_MAPPING | WP_IW_GPMT_PREFIX_REMARKING;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[5].yellow.priority_mapping_value = UNI_Qos_mapping[5];
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[5].yellow.prefix_remarking_data.remarking_type[0] = WP_IW_GPMT_REMARKING_TYPE_PREFIX_VLAN_PRIO;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[5].yellow.prefix_remarking_data.value[0] = 5;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[5].yellow.prefix_remarking_data.remarking_type[1] = WP_IW_GPMT_REMARKING_TYPE_PREFIX_MPLS_EXP;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[5].yellow.prefix_remarking_data.value[1] = 5;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[5].yellow.prefix_remarking_data.remarking_type[2] = WP_IW_GPMT_REMARKING_TYPE_DO_NOT_CHANGE;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[5].yellow.prefix_remarking_data.value[2] = 0;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[5].yellow.prefix_remarking_data.remarking_type[3] = WP_IW_GPMT_REMARKING_TYPE_DO_NOT_CHANGE;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[5].yellow.prefix_remarking_data.value[3] = 0;

    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[5].red.action_mode = WP_IW_GPMT_PRIORITY_MAPPING | WP_IW_GPMT_PREFIX_REMARKING;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[5].red.priority_mapping_value = UNI_Qos_mapping[5];
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[5].red.prefix_remarking_data.remarking_type[0] = WP_IW_GPMT_REMARKING_TYPE_PREFIX_VLAN_PRIO;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[5].red.prefix_remarking_data.value[0] = 5;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[5].red.prefix_remarking_data.remarking_type[1] = WP_IW_GPMT_REMARKING_TYPE_PREFIX_MPLS_EXP;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[5].red.prefix_remarking_data.value[1] = 5;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[5].red.prefix_remarking_data.remarking_type[2] = WP_IW_GPMT_REMARKING_TYPE_DO_NOT_CHANGE;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[5].red.prefix_remarking_data.value[2] = 0;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[5].red.prefix_remarking_data.remarking_type[3] = WP_IW_GPMT_REMARKING_TYPE_DO_NOT_CHANGE;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[5].red.prefix_remarking_data.value[3] = 0;

    // index 6
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[6].green.action_mode = WP_IW_GPMT_PRIORITY_MAPPING | WP_IW_GPMT_PREFIX_REMARKING;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[6].green.priority_mapping_value = UNI_Qos_mapping[6];
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[6].green.prefix_remarking_data.remarking_type[0] = WP_IW_GPMT_REMARKING_TYPE_PREFIX_VLAN_PRIO;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[6].green.prefix_remarking_data.value[0] = 6;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[6].green.prefix_remarking_data.remarking_type[1] = WP_IW_GPMT_REMARKING_TYPE_PREFIX_MPLS_EXP;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[6].green.prefix_remarking_data.value[1] = 6;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[6].green.prefix_remarking_data.remarking_type[2] = WP_IW_GPMT_REMARKING_TYPE_DO_NOT_CHANGE;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[6].green.prefix_remarking_data.value[2] = 0;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[6].green.prefix_remarking_data.remarking_type[3] = WP_IW_GPMT_REMARKING_TYPE_DO_NOT_CHANGE;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[6].green.prefix_remarking_data.value[3] = 0;

    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[6].yellow.action_mode = WP_IW_GPMT_PRIORITY_MAPPING | WP_IW_GPMT_PREFIX_REMARKING;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[6].yellow.priority_mapping_value = UNI_Qos_mapping[6];
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[6].yellow.prefix_remarking_data.remarking_type[0] = WP_IW_GPMT_REMARKING_TYPE_PREFIX_VLAN_PRIO;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[6].yellow.prefix_remarking_data.value[0] = 6;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[6].yellow.prefix_remarking_data.remarking_type[1] = WP_IW_GPMT_REMARKING_TYPE_PREFIX_MPLS_EXP;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[6].yellow.prefix_remarking_data.value[1] = 6;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[6].yellow.prefix_remarking_data.remarking_type[2] = WP_IW_GPMT_REMARKING_TYPE_DO_NOT_CHANGE;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[6].yellow.prefix_remarking_data.value[2] = 0;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[6].yellow.prefix_remarking_data.remarking_type[3] = WP_IW_GPMT_REMARKING_TYPE_DO_NOT_CHANGE;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[6].yellow.prefix_remarking_data.value[3] = 0;

    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[6].red.action_mode = WP_IW_GPMT_PRIORITY_MAPPING | WP_IW_GPMT_PREFIX_REMARKING;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[6].red.priority_mapping_value = UNI_Qos_mapping[6];
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[6].red.prefix_remarking_data.remarking_type[0] = WP_IW_GPMT_REMARKING_TYPE_PREFIX_VLAN_PRIO;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[6].red.prefix_remarking_data.value[0] = 6;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[6].red.prefix_remarking_data.remarking_type[1] = WP_IW_GPMT_REMARKING_TYPE_PREFIX_MPLS_EXP;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[6].red.prefix_remarking_data.value[1] = 6;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[6].red.prefix_remarking_data.remarking_type[2] = WP_IW_GPMT_REMARKING_TYPE_DO_NOT_CHANGE;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[6].red.prefix_remarking_data.value[2] = 0;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[6].red.prefix_remarking_data.remarking_type[3] = WP_IW_GPMT_REMARKING_TYPE_DO_NOT_CHANGE;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[6].red.prefix_remarking_data.value[3] = 0;

    // index 7
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[7].green.action_mode = WP_IW_GPMT_PRIORITY_MAPPING | WP_IW_GPMT_PREFIX_REMARKING;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[7].green.priority_mapping_value = UNI_Qos_mapping[7];
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[7].green.prefix_remarking_data.remarking_type[0] = WP_IW_GPMT_REMARKING_TYPE_PREFIX_REMARKING_VAL;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[7].green.prefix_remarking_data.value[0] = 15;   // 7 << 1 | 0x1
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[7].green.prefix_remarking_data.remarking_type[1] = WP_IW_GPMT_REMARKING_TYPE_PREFIX_MPLS_EXP;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[7].green.prefix_remarking_data.value[1] = 7;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[7].green.prefix_remarking_data.remarking_type[2] = WP_IW_GPMT_REMARKING_TYPE_DO_NOT_CHANGE;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[7].green.prefix_remarking_data.value[2] = 0;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[7].green.prefix_remarking_data.remarking_type[3] = WP_IW_GPMT_REMARKING_TYPE_DO_NOT_CHANGE;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[7].green.prefix_remarking_data.value[3] = 0;

    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[7].yellow.action_mode = WP_IW_GPMT_PRIORITY_MAPPING | WP_IW_GPMT_PREFIX_REMARKING;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[7].yellow.priority_mapping_value = UNI_Qos_mapping[7];
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[7].yellow.prefix_remarking_data.remarking_type[0] = WP_IW_GPMT_REMARKING_TYPE_PREFIX_REMARKING_VAL;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[7].yellow.prefix_remarking_data.value[0] = 15;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[7].yellow.prefix_remarking_data.remarking_type[1] = WP_IW_GPMT_REMARKING_TYPE_PREFIX_MPLS_EXP;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[7].yellow.prefix_remarking_data.value[1] = 7;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[7].yellow.prefix_remarking_data.remarking_type[2] = WP_IW_GPMT_REMARKING_TYPE_DO_NOT_CHANGE;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[7].yellow.prefix_remarking_data.value[2] = 0;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[7].yellow.prefix_remarking_data.remarking_type[3] = WP_IW_GPMT_REMARKING_TYPE_DO_NOT_CHANGE;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[7].yellow.prefix_remarking_data.value[3] = 0;

    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[7].red.action_mode = WP_IW_GPMT_PRIORITY_MAPPING | WP_IW_GPMT_PREFIX_REMARKING;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[7].red.priority_mapping_value = UNI_Qos_mapping[7];
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[7].red.prefix_remarking_data.remarking_type[0] = WP_IW_GPMT_REMARKING_TYPE_PREFIX_REMARKING_VAL;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[7].red.prefix_remarking_data.value[0] = 15;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[7].red.prefix_remarking_data.remarking_type[1] = WP_IW_GPMT_REMARKING_TYPE_PREFIX_MPLS_EXP;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[7].red.prefix_remarking_data.value[1] = 7;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[7].red.prefix_remarking_data.remarking_type[2] = WP_IW_GPMT_REMARKING_TYPE_DO_NOT_CHANGE;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[7].red.prefix_remarking_data.value[2] = 0;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[7].red.prefix_remarking_data.remarking_type[3] = WP_IW_GPMT_REMARKING_TYPE_DO_NOT_CHANGE;
    iw_gpmt_cfg[SYSTEM_TYPE_VSI].iw_gpmt_priority_entry[7].red.prefix_remarking_data.value[3] = 0;

    iw_gpmt_table_handle[SYSTEM_TYPE_VSI] = WP_IwGenericPriorityMapCreate(WP_SYSHANDLE(DEFAULT_WPID), &(iw_gpmt_cfg[SYSTEM_TYPE_VSI]));
    App_TerminateOnError(iw_gpmt_table_handle[SYSTEM_TYPE_VSI], "WP_IwGenericPriorityMapCreate", __LINE__);

    memcpy(&iw_gpmt_cfg[SYSTEM_TYPE_VPWS], &iw_gpmt_cfg[SYSTEM_TYPE_VSI], sizeof(WP_iw_gpmt));
    iw_gpmt_table_handle[SYSTEM_TYPE_VPWS] = WP_IwGenericPriorityMapCreate(WP_SYSHANDLE(DEFAULT_WPID), &(iw_gpmt_cfg[SYSTEM_TYPE_VPWS]));
    App_TerminateOnError(iw_gpmt_table_handle[SYSTEM_TYPE_VPWS], "WP_IwGenericPriorityMapCreate", __LINE__);

    memcpy(&iw_gpmt_cfg[SYSTEM_TYPE_VRF], &iw_gpmt_cfg[SYSTEM_TYPE_VSI], sizeof(WP_iw_gpmt));
    // index 0
    iw_gpmt_cfg[SYSTEM_TYPE_VRF].iw_gpmt_priority_entry[0].green.prefix_remarking_data.remarking_type[2] = WP_IW_GPMT_REMARKING_TYPE_PREFIX_MPLS_EXP;
    iw_gpmt_cfg[SYSTEM_TYPE_VRF].iw_gpmt_priority_entry[0].green.prefix_remarking_data.value[2] = 0x0;
    
    iw_gpmt_cfg[SYSTEM_TYPE_VRF].iw_gpmt_priority_entry[0].yellow.prefix_remarking_data.remarking_type[2] = WP_IW_GPMT_REMARKING_TYPE_PREFIX_MPLS_EXP;
    iw_gpmt_cfg[SYSTEM_TYPE_VRF].iw_gpmt_priority_entry[0].yellow.prefix_remarking_data.value[2] = 0x0;    

    iw_gpmt_cfg[SYSTEM_TYPE_VRF].iw_gpmt_priority_entry[0].red.prefix_remarking_data.remarking_type[2] = WP_IW_GPMT_REMARKING_TYPE_PREFIX_MPLS_EXP;
    iw_gpmt_cfg[SYSTEM_TYPE_VRF].iw_gpmt_priority_entry[0].red.prefix_remarking_data.value[2] = 0x0;

    // index 1
    iw_gpmt_cfg[SYSTEM_TYPE_VRF].iw_gpmt_priority_entry[1].green.prefix_remarking_data.remarking_type[2] = WP_IW_GPMT_REMARKING_TYPE_PREFIX_MPLS_EXP;
    iw_gpmt_cfg[SYSTEM_TYPE_VRF].iw_gpmt_priority_entry[1].green.prefix_remarking_data.value[2] = 2;
    
    iw_gpmt_cfg[SYSTEM_TYPE_VRF].iw_gpmt_priority_entry[1].yellow.prefix_remarking_data.remarking_type[2] = WP_IW_GPMT_REMARKING_TYPE_PREFIX_MPLS_EXP;
    iw_gpmt_cfg[SYSTEM_TYPE_VRF].iw_gpmt_priority_entry[1].yellow.prefix_remarking_data.value[2] = 1;    

    // index 2
    iw_gpmt_cfg[SYSTEM_TYPE_VRF].iw_gpmt_priority_entry[2].green.prefix_remarking_data.remarking_type[2] = WP_IW_GPMT_REMARKING_TYPE_PREFIX_MPLS_EXP;
    iw_gpmt_cfg[SYSTEM_TYPE_VRF].iw_gpmt_priority_entry[2].green.prefix_remarking_data.value[2] = 2;
    
    iw_gpmt_cfg[SYSTEM_TYPE_VRF].iw_gpmt_priority_entry[2].yellow.prefix_remarking_data.remarking_type[2] = WP_IW_GPMT_REMARKING_TYPE_PREFIX_MPLS_EXP;
    iw_gpmt_cfg[SYSTEM_TYPE_VRF].iw_gpmt_priority_entry[2].yellow.prefix_remarking_data.value[2] = 1;    

    // index 3
    iw_gpmt_cfg[SYSTEM_TYPE_VRF].iw_gpmt_priority_entry[3].green.prefix_remarking_data.remarking_type[2] = WP_IW_GPMT_REMARKING_TYPE_PREFIX_MPLS_EXP;
    iw_gpmt_cfg[SYSTEM_TYPE_VRF].iw_gpmt_priority_entry[3].green.prefix_remarking_data.value[2] = 2;
    
    iw_gpmt_cfg[SYSTEM_TYPE_VRF].iw_gpmt_priority_entry[3].yellow.prefix_remarking_data.remarking_type[2] = WP_IW_GPMT_REMARKING_TYPE_PREFIX_MPLS_EXP;
    iw_gpmt_cfg[SYSTEM_TYPE_VRF].iw_gpmt_priority_entry[3].yellow.prefix_remarking_data.value[2] = 1;    

    // index 4
    iw_gpmt_cfg[SYSTEM_TYPE_VRF].iw_gpmt_priority_entry[4].green.prefix_remarking_data.remarking_type[2] = WP_IW_GPMT_REMARKING_TYPE_PREFIX_MPLS_EXP;
    iw_gpmt_cfg[SYSTEM_TYPE_VRF].iw_gpmt_priority_entry[4].green.prefix_remarking_data.value[2] = 4;
    
    iw_gpmt_cfg[SYSTEM_TYPE_VRF].iw_gpmt_priority_entry[4].yellow.prefix_remarking_data.remarking_type[2] = WP_IW_GPMT_REMARKING_TYPE_PREFIX_MPLS_EXP;
    iw_gpmt_cfg[SYSTEM_TYPE_VRF].iw_gpmt_priority_entry[4].yellow.prefix_remarking_data.value[2] = 3;
    
    // index 5
    iw_gpmt_cfg[SYSTEM_TYPE_VRF].iw_gpmt_priority_entry[5].green.prefix_remarking_data.remarking_type[2] = WP_IW_GPMT_REMARKING_TYPE_PREFIX_MPLS_EXP;
    iw_gpmt_cfg[SYSTEM_TYPE_VRF].iw_gpmt_priority_entry[5].green.prefix_remarking_data.value[2] = 5;
    
    iw_gpmt_cfg[SYSTEM_TYPE_VRF].iw_gpmt_priority_entry[5].yellow.prefix_remarking_data.remarking_type[2] = WP_IW_GPMT_REMARKING_TYPE_PREFIX_MPLS_EXP;
    iw_gpmt_cfg[SYSTEM_TYPE_VRF].iw_gpmt_priority_entry[5].yellow.prefix_remarking_data.value[2] = 5;    

    iw_gpmt_cfg[SYSTEM_TYPE_VRF].iw_gpmt_priority_entry[5].red.prefix_remarking_data.remarking_type[2] = WP_IW_GPMT_REMARKING_TYPE_PREFIX_MPLS_EXP;
    iw_gpmt_cfg[SYSTEM_TYPE_VRF].iw_gpmt_priority_entry[5].red.prefix_remarking_data.value[2] = 5;

    // index 6
    iw_gpmt_cfg[SYSTEM_TYPE_VRF].iw_gpmt_priority_entry[6].green.prefix_remarking_data.remarking_type[2] = WP_IW_GPMT_REMARKING_TYPE_PREFIX_MPLS_EXP;
    iw_gpmt_cfg[SYSTEM_TYPE_VRF].iw_gpmt_priority_entry[6].green.prefix_remarking_data.value[2] = 6;
    
    iw_gpmt_cfg[SYSTEM_TYPE_VRF].iw_gpmt_priority_entry[6].yellow.prefix_remarking_data.remarking_type[2] = WP_IW_GPMT_REMARKING_TYPE_PREFIX_MPLS_EXP;
    iw_gpmt_cfg[SYSTEM_TYPE_VRF].iw_gpmt_priority_entry[6].yellow.prefix_remarking_data.value[2] = 6;    

    iw_gpmt_cfg[SYSTEM_TYPE_VRF].iw_gpmt_priority_entry[6].red.prefix_remarking_data.remarking_type[2] = WP_IW_GPMT_REMARKING_TYPE_PREFIX_MPLS_EXP;
    iw_gpmt_cfg[SYSTEM_TYPE_VRF].iw_gpmt_priority_entry[6].red.prefix_remarking_data.value[2] = 6;

    // index 7
    iw_gpmt_cfg[SYSTEM_TYPE_VRF].iw_gpmt_priority_entry[7].green.prefix_remarking_data.remarking_type[2] = WP_IW_GPMT_REMARKING_TYPE_PREFIX_MPLS_EXP;
    iw_gpmt_cfg[SYSTEM_TYPE_VRF].iw_gpmt_priority_entry[7].green.prefix_remarking_data.value[2] = 7;
    
    iw_gpmt_cfg[SYSTEM_TYPE_VRF].iw_gpmt_priority_entry[7].yellow.prefix_remarking_data.remarking_type[2] = WP_IW_GPMT_REMARKING_TYPE_PREFIX_MPLS_EXP;
    iw_gpmt_cfg[SYSTEM_TYPE_VRF].iw_gpmt_priority_entry[7].yellow.prefix_remarking_data.value[2] = 7;    

    iw_gpmt_cfg[SYSTEM_TYPE_VRF].iw_gpmt_priority_entry[7].red.prefix_remarking_data.remarking_type[2] = WP_IW_GPMT_REMARKING_TYPE_PREFIX_MPLS_EXP;
    iw_gpmt_cfg[SYSTEM_TYPE_VRF].iw_gpmt_priority_entry[7].red.prefix_remarking_data.value[2] = 7;

    iw_gpmt_table_handle[SYSTEM_TYPE_VRF] = WP_IwGenericPriorityMapCreate(WP_SYSHANDLE(DEFAULT_WPID), &(iw_gpmt_cfg[SYSTEM_TYPE_VRF]));
    App_TerminateOnError(iw_gpmt_table_handle[SYSTEM_TYPE_VRF], "WP_IwGenericPriorityMapCreate", __LINE__);


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
    WP_U16 ii;
    
    for (ii = 0; ii < NR_GBE; ii++)
    {
        if (!WPE_HWPortIdValid(ii))
        {
            continue;
        }    
        status = WP_PortEnable(hw_port[ii].port_enet, WP_DIRECTION_DUPLEX);
        App_TerminateOnError(status, "WP_PortEnable",__LINE__);

        status = WP_DeviceEnable(hw_port[ii].dev_enet, WP_DIRECTION_DUPLEX);
        App_TerminateOnError(status, "WP_DeviceEnable", __LINE__);

    }

    return ;
}

/*****************************************************************************
 * Function name: App_SystemEnable
 * Description  : 
 * Input  params: None
 * Output params: None
 * Return val   : None
 *****************************************************************************/   

void App_SystemBuild(void)
{
    WP_status status;
    
    status = WP_IwSystemBuild(common_system.iw_system);
    App_TerminateOnError(status, "WP_IwSystemBuild()",__LINE__);
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
    // Create all filters
    WPE_CreatePceFilters();

    // Create filter sets
    WPE_CreatePceFilterSets();

    // Create pce interfaces
    //WPE_CreatePceInterface(&common_system);

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

        size = sizeof(learned_rules_list)/sizeof(learned_rules_list[0]);

        while (1)
        {
#if LEARNING_MODE_INTERRUPT
                /* semaphore increased in the learning interrupt handler */
                if (learning_enable)
                {
                    WPL_SemDecrement(&learning_sem, 1);
                }
#else
                WP_Delay(5);
#endif

                /* Receive for learning Q */
                status = WP_PceLearningQueueService(WP_WINPATH(DEFAULT_WPID), 
                                                    &(learned_rules_list[0]),
                                                    size,
                                                    &rules_returned);

                if (!rules_returned)
                {
                    continue;
                }

                if (!learning_enable)
                {
                    continue;
                }

                for (ii = 0; ii < rules_returned; ii++)
                {
                    memcpy(&rule_fwd, &(learned_rules_list[ii].fwd_rule), sizeof(WP_pce_rule_forwarding));

                    WPE_AddFDBEntry(&rule_fwd);
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
    //WP_status status;
    WP_U32 StartTimeStamp = 0, CurrentTimeStamp = 0, delta = 0;
    WT_VsiSystemInfo* pSystem = NULL;

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
        StartTimeStamp = WT_TimeRead();
        
        do
        {
            pSystem = WPE_GetSystemByFdbHandle(rule_handle);
            if ((pSystem) && (pSystem->aging_en))
            {
                /* delete the aged rules */
                WPE_DelSystemFDBEntry(pSystem, rule_handle);
            }
            
            rule_handle = WP_PceIteratorGetNext(aging_iterator_handle);
            if (WP_ERROR_P(rule_handle))
            {
                if (WP_ERROR(rule_handle) != WP_ERR_PCE_ITERATOR_NO_ITEMS)
                        printf("\n\nerror %s \n", WP_ErrorString(rule_handle));
                break;
            }                
        }while(1);

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
        
        App_AgingFunc(NULL);
    }
}

// for init ram
#include "wt_partition_ecc_util.c"

