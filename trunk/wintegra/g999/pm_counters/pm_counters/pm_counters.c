/*****************************************************************************
 * (C) Copyright PMC - WIN division (Wintegra) 2012.  All rights reserved.
 * PMC-Sierra (WINTEGRA) CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of PMC-Sierra (Wintegra).
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/
 
 /*****************************************************************************
 *  Appplication: pm counters
 *  File Name   : pm_counters.c
 *
 *****************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "api/wp_wddi.h"
#include "api/wp_host_io.h"

#include "wpx_app_data.h"
#include "wpx_board_if.h"
#include "wpx_board_data.h"
#include "wp_sim.h"

#include "wpi_gpe_pecs_wddi_shared.h"
#include "api/wp_pce.h"
#include "wpx_enet_phy.h"
#define USE_XGI 0

#define USE_RGMII 0

#if USE_XGI
#define APP_ENET_PORT1 WP_PORT_XGI1
#elif USE_RGMII 
#define APP_ENET_PORT1 WP_PORT_ENET1
#else
#define APP_ENET_PORT1 WP_PORT_ENET7
#endif

#define APP_ENET_PORT2 WP_PORT_ENET8

#define NUM_OF_L1_GROUP 1
#define NUM_OF_L2_GROUP 1

#define NUM_OF_FRAMES 5
#define NUM_OF_BUFS               25
#define N_RINGS                         10
#define RING_LENGTH                     (2 * NUM_OF_BUFS * NUM_OF_FRAMES)
#define N_BUFFERS                       (N_RINGS * RING_LENGTH)

#define IW_BUFFER_SIZE 768

#define NUM_OF_G999_ENET_TX_CHANNELS 8
#define NUM_OF_G999_ENET_RX_CHANNELS 1

#define G999_MAX_FRAGMENT_LENGTH 448

#define NUM_OF_GE_ENET_TX_CHANNELS 8
#define NUM_OF_GE_ENET_RX_CHANNELS 1

#define SDU_SIZE                   (9000)
#define MTU_SIZE_GE                 1000
#define MTU_SIZE_G999               1200
#define MAXT_SIZE_GE                1500
#define MAXT_SIZE_G999              2000

#define AGG_TAG_BASE                1
#define CHANNEL_TAG_BASE            50
#define BRIDGE_PORT_ENET_TAG        100
#define BRIDGE_PORT_HOST_TAG        110
#define BRIDGE_PORT_HIER_ENET_TAG   120
#define BRIDGE_PORT_MC_TAG          130


#define MAX_NUM_OF_POLICER  2
#define MAX_POLICERS_INTERNAL  10
#define MAX_POLICERS_EXTERNAL  10


WP_U32     agg_tag_counter = AGG_TAG_BASE;
WP_U32     channel_tag_counter = CHANNEL_TAG_BASE;
WP_handle  port_enet_ge;
WP_handle  port_enet_g999;
WP_handle  dev_enet_ge;
WP_handle  dev_enet_g999;

WP_handle  port_host, dev_host, bport_host, agg_host, rx_host_channel, tx_host_channel;
WP_handle rx_host_channel_array[4];
WP_handle agg_host_array[4];

WP_handle l1_shaping_block_ge;
WP_handle l1_shaping_block_g999;

WP_handle l2_shaping_block_ge;
WP_handle l2_shaping_block_g999;
WP_handle l1_group_ge[NUM_OF_L1_GROUP];
WP_handle l2_group_ge[NUM_OF_L1_GROUP][NUM_OF_L2_GROUP];
WP_handle l1_group_g999[NUM_OF_L1_GROUP];
WP_handle l2_group_g999[NUM_OF_L1_GROUP][NUM_OF_L2_GROUP];

WP_U8 enet_prefix_header[12] = {0x12, 0x34, 0x56, 0x78, 0x9a, 0x01,
                                0x12, 0x34, 0x56, 0x00, 0x00, 0x00};

WP_handle g999_stream;


WP_handle  iw_sys;
WP_handle  iw_qnode, qnode_host, qnode_mc;
WP_handle  adjunct_buffer_pool, pool_host, pool_ring_host;
WP_handle  bport_enet_g999,bport_enet_ge;

WP_handle rx_ge_enet_channel,rx_g999_enet_channel,rx_g999_enet_channel_on_dev;

WP_handle tx_ge_enet_channel[NUM_OF_GE_ENET_TX_CHANNELS];

WP_handle tx_g999_enet_channel[NUM_OF_G999_ENET_TX_CHANNELS];

WP_handle ge_enet_tx_flow_agg[NUM_OF_GE_ENET_TX_CHANNELS];

WP_handle g999_enet_tx_flow_agg[NUM_OF_G999_ENET_TX_CHANNELS];


WP_handle PCE_filter[2];

WP_handle filter_set_handle;

WP_handle policer_action_handle[2];
WP_handle policer_handle_v2[2];
WP_handle policer_handle_v1[2];

WP_handle group_handle[2];
WP_handle mc_flowagg_handle[2];
WP_handle member_handle[20];

WP_handle dfc_filter_handle;
WP_handle dfc_flow_handle[2];

#define NUM_OF_MC_MEMBERS_PER_GROUP 10

void App_TerminateOnError(WP_handle handle, WP_CHAR *s, WP_U32 line);
void    WPE_SysInit(void);
void    WPE_CreateIWBridgeSystem(void);
void    WPE_InitHWCards(void);
void    WPE_CreateBufferPools(void);
#if 0
void    WPE_CreateSchedulingUnit(void);
#endif
void    WPE_CreateGeEnetPortDevice(void);
void    WPE_CreateG999EnetPortDevice(void);
void    WPE_G999StreamsCreate(void);
void    WPE_CreateHostPortDevice(void);
void	WPE_EnableG999Stream(void);
void    WPE_DisplayFlowAggStats(WP_handle agg);
void    WPE_DisplayPCEStats(WP_handle filter_handle);
WP_U32 WPE_DisplayMcMemberStats(WP_handle group,WP_handle member);
void WPE_MulticastInit(void);
void WPE_V1PolicerCreate(void);
void    WPE_V2PolicerCreate_1(WP_U32 cir,WP_U32 eir);
void    WPE_V2PolicerCreate_2(WP_U32 cir,WP_U32 eir);
void    WPE_TestCfgInit(void);

void    WPE_BridgingFlowAdd(void);
void    WPE_FlowStatDisplay(WP_handle statistic_handle );
void  WPE_HostTestCfgInit_1(void);
void  WPE_HostTestCfgInit_2(void);
void  WPE_HostTestCfgInit_3(void);
void  WPE_HostTestCfgInit_4(void);
void  WPE_HostTestCfgInit_5(void);
void  WPE_HostTestCfgInit_6(void);
void  WPE_HostTestCfgInit_7(void);
void  WPE_HostTestCfgInit_8(void);

#include "pm_counters_menu.c"

#include "pm_counters_util.h"

#include "pm_counters_util.c"

WP_device_enet_g999 device_enet_g999_cfg = {0};
WP_device_enet_g999_ex 		  device_enet_g999_ex = {0};

WP_pce_parser_info pce_parser_info;



WP_module_pce_init pce_init =
{
        &pce_parser_info, /* parser_info */
        100, /* hardware_rules_num */
        100, /* filters_num */
        10000, /* pkt_rx_channel_interfaces_num */
        10000, /* iw_tx_channel_interfaces_num */
        WP_PCE_DISCARD_MODE_SMART_DISCARD, /* pce_discard_mode */
        WP_ENABLE, /* bridge_loop_prevention_mode */
        WP_DISABLE, /* learning_mode */

};


WP_module_congestion congestion_init =
{
   /* max_profiles */2,
};


/* Policer Structures */
WP_policer_action policer_action_cfg[2] =
{
   {
      //green:
      {
         /* WP_policer_action_mode action_mode; */   WP_POLICER_ACTION_PRIORITY_MAPPING,
         /* WP_policer_action_prefix_remarking prefix_remarking_data; */ {{0,0,0,0}, {0,0,0,0}},
         /* WP_U8 priority_mapping_value; */         0,
         /* WP_U16 congestion_profile_entry; */      0,
      },
      //yellow:
      {
         /* WP_policer_action_mode action_mode; */   WP_POLICER_ACTION_PRIORITY_MAPPING,
         /* WP_policer_action_prefix_remarking prefix_remarking_data; */ {{0,0,0,0}, {0,0,0,0}},
         /* WP_U8 priority_mapping_value; */         0,
         /* WP_U16 congestion_profile_entry; */      0,
      },
      //red:
      {
         /* WP_policer_action_mode action_mode; */   WP_POLICER_ACTION_PRIORITY_MAPPING,
         /* WP_policer_action_prefix_remarking prefix_remarking_data; */ {{0,0,0,0}, {0,0,0,0}},
         /* WP_U8 priority_mapping_value; */         0,
         /* WP_U16 congestion_profile_entry; */      0,
      },
   },
   {
      //green:
      {
         /* WP_policer_action_mode action_mode; */   WP_POLICER_ACTION_PRIORITY_MAPPING,
         /* WP_policer_action_prefix_remarking prefix_remarking_data; */ {{0,0,0,0}, {0,0,0,0}},
         /* WP_U8 priority_mapping_value; */         0,
         /* WP_U16 congestion_profile_entry; */      0,
      },
      //yellow:
      {
         /* WP_policer_action_mode action_mode; */   WP_POLICER_ACTION_PRIORITY_MAPPING,
         /* WP_policer_action_prefix_remarking prefix_remarking_data; */ {{0,0,0,0}, {0,0,0,0}},
         /* WP_U8 priority_mapping_value; */         0,
         /* WP_U16 congestion_profile_entry; */      0,
      },
      //red:
      {
         /* WP_policer_action_mode action_mode; */   WP_POLICER_ACTION_PRIORITY_MAPPING,
         /* WP_policer_action_prefix_remarking prefix_remarking_data; */ {{0,0,0,0}, {0,0,0,0}},
         /* WP_U8 priority_mapping_value; */         0,
         /* WP_U16 congestion_profile_entry; */      0,
      },
   }
};


WP_policer_v2 policer_cfg[2] = 
{
   { /* Internal Policer */
      /* WP_policer_method method; */ WP_POLICER_METHOD_MEF_WITH_COUPLING_FLAG,
      /* WP_policer_v2_type type; */  WP_POLICER_V2_TYPE_INTERNAL,
      /* WP_color_mode color_mode */  WP_POLICER_COLOR_BLIND,
      /* WP_red_packets_action red_packets_action */  WP_POLICER_DROP_RED_PACKETS,
      /* WP_U32 cir */                20000000,
      /* WP_U32 cbs */                81920,
      /* WP_U32 eir */                0,
      /* WP_U32 ebs */                0,
      /* WP_handle policer_action */  0, /* policer_action_handle, will be set after ActionCreate */
      /* WP_U8 frame_len_correction */ WP_POLICER_LENGTH_CORRECT_DISABLE,
      /* WP_frame_len_crct_action frame_len_crct_action */ WP_POLICER_FRAME_LEN_CORRECTION_ADD,
   },
   { /* Internal Policer */
      /* WP_policer_method method; */ WP_POLICER_METHOD_MEF_WITH_COUPLING_FLAG,
      /* WP_policer_v2_type type; */  WP_POLICER_V2_TYPE_INTERNAL,
      /* WP_color_mode color_mode */  WP_POLICER_COLOR_BLIND,
      /* WP_red_packets_action red_packets_action */  WP_POLICER_DROP_RED_PACKETS,
      /* WP_U32 cir */                10000000,
      /* WP_U32 cbs */                81920,
      /* WP_U32 eir */                0,
      /* WP_U32 ebs */                0,
      /* WP_handle policer_action */  0, /* policer_action_handle, will be set after ActionCreate */
      /* WP_U8 frame_len_correction */ WP_POLICER_LENGTH_CORRECT_DISABLE,
      /* WP_frame_len_crct_action frame_len_crct_action */ WP_POLICER_FRAME_LEN_CORRECTION_ADD,
   }
};


void WPE_V2PolicerCreate_1(WP_U32 cir,WP_U32 eir)
{
    policer_action_handle[0] = WP_PolicerActionCreate(WP_WINPATH(0), &policer_action_cfg[0]);
    App_TerminateOnError(policer_action_handle[0], "WP_PolicerActionCreate", __LINE__);

    policer_cfg[0].cir = cir;
    policer_cfg[0].eir = eir;
    policer_cfg[0].policer_action = policer_action_handle[0];
    policer_handle_v2[0] = WP_PolicerCreate(WP_WINPATH(0), WP_POLICER_V2, &policer_cfg[0]);
    App_TerminateOnError(policer_handle_v2[0], "WP_PolicerCreate", __LINE__);
}

void WPE_V2PolicerCreate_2(WP_U32 cir,WP_U32 eir)
    {
    policer_action_handle[1] = WP_PolicerActionCreate(WP_WINPATH(0), &policer_action_cfg[1]);
    App_TerminateOnError(policer_action_handle[1], "WP_PolicerActionCreate", __LINE__);

    policer_cfg[1].cir = cir;
    policer_cfg[1].eir = eir;
    policer_cfg[1].policer_action = policer_action_handle[1];
    policer_handle_v2[1] = WP_PolicerCreate(WP_WINPATH(0), WP_POLICER_V2, &policer_cfg[1]);
    App_TerminateOnError(policer_handle_v2[1], "WP_PolicerCreate", __LINE__);
}



void WPE_V1PolicerCreate(void)
{

   WP_policer_dlb_config dlb_config[1] =
{
   {
      /*leaky_bucket_size_one;*/ 100,
      /*leaky_bucket_size_two;*/ 100,
          /*excess_info_rate;*/      10000
   }   
};

WP_policer_v1 policer_config[1] =
{
   {
      /* *leaky_bucket; */ &dlb_config[0],
          /* committed_info_rate; */ 10000,
          /* policer_mode; */ WP_IW_DISCARD_NONCONFORMING_PKT,
      /* tx_handle; */ 0,
      /* leaky_bucket_mode; */ WP_IW_DUAL_LEAKY_BUCKET,
      /* frame_len_correction; */ WP_IW_PLC_FRM_LEN_CORRECT_DISABLE
   }   
};

   policer_handle_v1[0] = WP_PolicerCreate(WP_WINPATH(0), WP_POLICER_V1, policer_config);
   App_TerminateOnError(policer_handle_v1[0], "WP_PolicerCreate() v1 P1", __LINE__);

    dlb_config[0].excess_info_rate = 5000;
    policer_config[0].committed_info_rate = 5000;

   policer_handle_v1[1] = WP_PolicerCreate(WP_WINPATH(0), WP_POLICER_V1, policer_config);
   App_TerminateOnError(policer_handle_v1[1], "WP_PolicerCreate() v1 P2", __LINE__);

}



void WPE_PceModulesInit(void)
{
    WP_status status;

    /* PCE module initialization */    
    memset(&pce_init, 0, sizeof (WP_module_pce_init));
    pce_init.parser_info = &pce_parser_info;
    pce_init.hardware_rules_num = 4000;
    pce_init.filters_num = 50;
    pce_init.pkt_rx_channel_interfaces_num = 20;
    pce_init.iw_tx_channel_interfaces_num = 20;
    pce_init.pce_discard_mode = WP_PCE_DISCARD_MODE_SMART_DISCARD;
    pce_init.bridge_loop_prevention_mode = WP_ENABLE;
    pce_init.learning_mode = WP_DISABLE;
    pce_init.device_interfaces_num = 20;
    
    status = WP_ModuleInit(WP_SYSHANDLE(0), WP_WDDI_MODULE_PCE, &pce_init);
    App_TerminateOnError(status, "WPI_PceModuleInit()",__LINE__);
}


void WPE_CongestionModulesInit(void)
{
    WP_status status;
   status = WP_ModuleInit(WP_SYSHANDLE(0), WP_WDDI_MODULE_CONGESTION, &congestion_init);
   App_TerminateOnError(status, "WPI_PceModuleInit()",__LINE__);
}


void WPE_PolicerModulesInit(void)
{
    WP_status status;
    WP_policer_limits_v2 policer_limits_v2 =
   {
      /* max_internal_policers*/ MAX_POLICERS_INTERNAL, //change to MAX_POLICERS_INTERNAL
      /* max_external_policers*/ MAX_POLICERS_EXTERNAL,

      /* max_policer_actions*/   10
   };

    WP_policer_limits_v1 policer_limits_v1 =
	{
			/* max_policers  */ 2,
			/* tcm_policer_limits */
			{
					/* max_vlan_priority_tcm_policers */ 0,
					/* max_dscp_tcm_policers */ 0,
			}
	};

   WP_module_policer module_policer =
   {
      /* v2_policer_limits */  &policer_limits_v2,
      /* v1_policer_limits */  &policer_limits_v1,
      /* atm_policer_limits */ 0,
      /* mode */ WP_POLICER_STATS_ENABLE
   };

    status = WP_ModuleInit(WP_SYSHANDLE(WP_WINPATH(0)),WP_WDDI_MODULE_POLICER,&module_policer);
   App_TerminateOnError(status, "WP_ModuleInit (WP_WDDI_MODULE_POLICER)", __LINE__);
}

/*****************************************************************************
 * Function name: App_PceMoudleInit
 * Description  : This function initialize the modules
 * Input  params: None
 * Output params: None
 * Return val   : None
 *****************************************************************************/
void WPE_ModulesInit(void)
{
    WPE_PceModulesInit();
    WPE_CongestionModulesInit();
    WPE_PolicerModulesInit();
}


WP_S32 main(WP_S32 argc, WP_CHAR **argv)
{
    /* Start Menu Engine */
    CLI_T_Main();
    return 0;
}




void    WPE_CreateGeEnetL1FMUGroups(void)
{
   WP_U32 entry;



   WP_fmu_shaping_cir_eir l1_cir_eir_shaping_params = 
    {
        /* cir;*/     500000000,/*bits/second, 500Mbps*/
        /* cbs;*/     81920,    /* Committed Burst Size in bits */
        /* eir;*/     0,       /* bits/second */
        /* ebs;*/     0,    /* Committed Burst Size in bits */
        /* flags */   WP_FMU_SHAPING_CIR_EIR_SELECT_MODE_RR,
    };


 
   WP_shaping_group_type_packet enet_group_l1_config =
      {   
         /* group_level */     WP_L1_GROUP,

         /* tx_shaping_type*/  WP_FMU_SHAPING_TYPE_CIR_EIR,
         /* tx_shaping_params*/&l1_cir_eir_shaping_params,

         /* num_fifos */	WP_UNUSED,
         /* block_handle */     0,
         /* block_level */      0, // not used without ShapingBlock
         /* group_mode */       0, // not relevant for L1
      };

      l1_shaping_block_ge = WP_ShapingBlockCreate(WP_SYSHANDLE(WP_WINPATH(0)), 
                                         WP_L1_GROUP, 1, WP_UNUSED);
   App_TerminateOnError(l1_shaping_block_ge, "WP_ShapingBlockCreate for L1",__LINE__);
   
    enet_group_l1_config.block_handle = l1_shaping_block_ge;
    enet_group_l1_config.block_level = 0;

   for(entry = 0; entry < NUM_OF_L1_GROUP; entry ++)
   {
      l1_group_ge[entry] = WP_ShapingGroupCreate(dev_enet_ge, WP_SHAPING_GROUP_TYPE_PACKET, &enet_group_l1_config);
      App_TerminateOnError(l1_group_ge[entry] , "WP_ShapingGroupCreate l1 Ge",__LINE__);
   }
}



void    WPE_CreateGeEnetL2FMUGroups(void)
{
   WP_U32 i, entry;
#if 0
   WP_fmu_shaping_cir_eir l2_cir_eir_shaping_params = 
    {
        /* cir;*/     500000000,/* bits/second, 500Mbps*/
        /* cbs;*/     800000,    /* Committed Burst Size in bits */
        /* eir;*/     0,       /* bits/second */
        /* ebs;*/     800000,    /* Committed Burst Size in bits */
        /* flags */   WP_FMU_SHAPING_CIR_EIR_SELECT_MODE_RR,
    };
#endif
   WP_shaping_group_type_packet enet_group_l2_config =
      {   
         /* group_level */	WP_L2_GROUP,

         /* tx_shaping_type*/  WP_FMU_SHAPING_TYPE_STRICT,//WP_FMU_SHAPING_TYPE_CIR_EIR,
         /* tx_shaping_params*/NULL,//&l2_cir_eir_shaping_params,
         
         /* num_fifos */	WP_NUM_FIFOS_8,
         /* block_handle */     0,
         /* block_level */      0, // not used without ShapingBlock
         /* group_mode */       WP_MODE_HW,
      };


   l2_shaping_block_ge = WP_ShapingBlockCreate(WP_SYSHANDLE(WP_WINPATH(0)), WP_L2_GROUP, NUM_OF_L2_GROUP, WP_NUM_FIFOS_8);
   App_TerminateOnError(l2_shaping_block_ge, "WP_ShapingBlockCreate",__LINE__);
   enet_group_l2_config.block_handle = l2_shaping_block_ge;


   for(i = 0; i < NUM_OF_L1_GROUP; i ++)
   {
      for(entry = 0; entry < NUM_OF_L2_GROUP; entry ++)
      {
         enet_group_l2_config.block_level = 0; // used only if scenario with shaping block was chosen

         l2_group_ge[i][entry] = WP_ShapingGroupCreate(l1_group_ge[i], WP_SHAPING_GROUP_TYPE_PACKET, &enet_group_l2_config);
         App_TerminateOnError(l2_group_ge[i][entry] , "WP_ShapingGroupCreate l2 Ge",__LINE__);

      }
   }

}





void    WPE_CreateG999EnetL1FMUGroups(void)
{
   WP_U32 entry;


   WP_fmu_shaping_cir_eir l1_cir_eir_shaping_params = 
    {
        /* cir;*/     750000000,/* bits/second, 750Mbps*/
        /* cbs;*/     81920,    /* Committed Burst Size in bits */
        /* eir;*/     0,//750000000,       /* bits/second */
        /* ebs;*/     0,    /* Committed Burst Size in bits */
        /* flags */   WP_FMU_SHAPING_CIR_EIR_SELECT_MODE_RR,
    };


 
   WP_shaping_group_type_packet enet_group_l1_config =
      {   
         /* group_level */     WP_L1_GROUP,

         /* tx_shaping_type*/  WP_FMU_SHAPING_TYPE_CIR_EIR,
         /* tx_shaping_params*/&l1_cir_eir_shaping_params,

         /* num_fifos */	WP_NUM_FIFOS_8,
         /* block_handle */     0,
         /* block_level */      0, // not used without ShapingBlock
         /* group_mode */       WP_MODE_HW, // not relevant for L1
      };

    l1_shaping_block_g999 = WP_ShapingBlockCreate(WP_SYSHANDLE(WP_WINPATH(0)), WP_L1_GROUP, 1, WP_UNUSED);
   App_TerminateOnError(l1_shaping_block_g999, "WP_ShapingBlockCreate for L1",__LINE__);
   
    enet_group_l1_config.block_handle = l1_shaping_block_g999;
    enet_group_l1_config.block_level = 0;
   

   for(entry = 0; entry < NUM_OF_L1_GROUP; entry ++)
   {
      l1_group_g999[entry] = WP_ShapingGroupCreate(g999_stream, WP_SHAPING_GROUP_TYPE_PACKET, &enet_group_l1_config);
      App_TerminateOnError(l1_group_g999[entry] , "WP_ShapingGroupCreate l1 G999",__LINE__);
   }
}

void    WPE_CreateG999EnetL2FMUGroups(void)
{
   WP_U32 i, entry;
#if 0
   WP_fmu_shaping_cir_eir l2_cir_eir_shaping_params = 
    {
        /* cir;*/     750000000,/* bits/second, 750Mbps*/
        /* cbs;*/     800000,    /* Committed Burst Size in bits */
        /* eir;*/     0,//750000000,       /* bits/second */
        /* ebs;*/     800000,    /* Committed Burst Size in bits */
        /* flags */   WP_FMU_SHAPING_CIR_EIR_SELECT_MODE_RR,
    };
#endif
   WP_shaping_group_type_packet enet_group_l2_config =
      {   
         /* group_level */	WP_L2_GROUP,

         /* tx_shaping_type*/  WP_FMU_SHAPING_TYPE_STRICT,//WP_FMU_SHAPING_TYPE_CIR_EIR,
         /* tx_shaping_params*/NULL,//&l2_cir_eir_shaping_params,

         /* num_fifos */	WP_NUM_FIFOS_8,
         /* block_handle */     0,
         /* block_level */      0, // not used without ShapingBlock
         /* group_mode */       WP_MODE_HW,
      };


   l2_shaping_block_g999 = WP_ShapingBlockCreate(WP_SYSHANDLE(WP_WINPATH(0)), WP_L2_GROUP, NUM_OF_L2_GROUP, WP_NUM_FIFOS_8);
   App_TerminateOnError(l2_shaping_block_g999, "WP_ShapingBlockCreate",__LINE__);
   enet_group_l2_config.block_handle = l2_shaping_block_g999;


   for(i = 0; i < NUM_OF_L1_GROUP; i ++)
   {
      for(entry = 0; entry < NUM_OF_L2_GROUP; entry ++)
      {
         enet_group_l2_config.block_level = 0; // used only if scenario with shaping block was chosen

         l2_group_g999[i][entry] = WP_ShapingGroupCreate(l1_group_g999[i], WP_SHAPING_GROUP_TYPE_PACKET, &enet_group_l2_config);
         App_TerminateOnError(l2_group_g999[i][entry] , "WP_ShapingGroupCreate l2 G999",__LINE__);
      }
   }

}



void    WPE_CreateHostIwRxChannel(void)
{
    // This is TX from DPS point of view but RX from Host point of view
    WP_tx_binding tx_host_binding_config =
    {
    /* res0 */      0, // Unused
    /* dci_mode */  WP_IW_DYN_CH_INSERT_DISABLE,
    /* maxt */      10
    };

    WP_ch_iw_rx   iw_host_ch_config =
    {
    /* pqblock */           0,
    /* pqlevel */           0,
    /* tx_binding_type */   WP_IW_TX_BINDING,
    /* tx_binding_config */ &tx_host_binding_config
    };

    rx_host_channel = WP_ChannelCreate(0, dev_host, iw_qnode, WP_CH_RX, WP_IW_HOST, &iw_host_ch_config);
    App_TerminateOnError(rx_host_channel, "WP_ChannelCreate RX Host channel",__LINE__);

}

void    WPE_CreateHostIwRxChannelArray(void)
{
    WP_U32 i = 0;
   // This is TX from DPS point of view but RX from Host point of view
   WP_tx_binding tx_host_binding_config =
      {
         /* res0 */      0, // Unused
         /* dci_mode */  WP_IW_DYN_CH_INSERT_DISABLE,
         /* maxt */      10
      };

   WP_ch_iw_rx   iw_host_ch_config =
      {
         /* pqblock */           0,
         /* pqlevel */           0,
         /* tx_binding_type */   WP_IW_TX_BINDING,
         /* tx_binding_config */ &tx_host_binding_config
      };
    for(i=0;i<4;i++)
    {
        rx_host_channel_array[i] = WP_ChannelCreate(0, dev_host, iw_qnode, WP_CH_RX, WP_IW_HOST, &iw_host_ch_config);
        App_TerminateOnError(rx_host_channel_array[i], "WP_ChannelCreate RX Host channel",__LINE__);
    }

}





// This is default flow aggregation
// In case you want to drop all the traffic that arrives to Default FlowAgg (or any other FA)
// You can set the MTU_SIZE=0; or set in tx_binding  maxt = 0
void    WPE_CreateHostTermFlowAgg(void)
{

    WP_iw_agg_bridging  agg_host_config_vlan_aware =

      {         
      /*tag                  */ 0,
      /*txfunc;              */ 0,
      /*iw_port;             */ 0,
      /*rfcs                 */ WP_IW_RFCS_DISABLE,
      /*l2header_insert_mode;*/ WP_IW_L2H_INSERT_DISABLE,
      /*vlan_tag_mode        */ WP_IW_VLAN_TAG_DISABLE,
      /*interruptqueue;      */ WP_IW_IRQT0,
      /*error_pkt_mode       */ WP_IW_ERRPKT_DISCARD,
      /*intmode;             */ WP_IW_INT_DISABLE,
      /*statmode;            */ WP_IW_STAT_ENABLE,
      /*timestamp_mode;      */ WP_IW_TIME_STAMP_DISABLE,
      /*ov_pool_mode         */ WP_IW_OV_POOL_DISABLE,
      /*fbp_drop_threshold;  */ 0,
      /*replace_vlan_id      */ WP_IW_REPLACE_VTAG_DISABLE,
      /*vlan_id              */ 0,
      /*vpmt_handle          */ 0,
      /*mtu;                 */ 1500,
      /*prefix_length        */ 0,
      /*prefix_header[36];   */ {},
      };


   
    agg_host_config_vlan_aware.txfunc = rx_host_channel;
    agg_host = WP_IwFlowAggregationCreate(WP_WINPATH(0), WP_IW_VLAN_AWARE_BRIDGING_MODE, &agg_host_config_vlan_aware);
    App_TerminateOnError(agg_host, "WP_IwFlowAggregationCreate() Host",__LINE__);

}


void    WPE_CreateHostTermFlowAggArray(void)
{
    WP_U32 i = 0;
    WP_iw_agg_bridging  agg_host_config_vlan_aware =

    {
      /*tag                  */ 0,
      /*txfunc;              */ 0,
      /*iw_port;             */ 0,
      /*rfcs                 */ WP_IW_RFCS_DISABLE,
      /*l2header_insert_mode;*/ WP_IW_L2H_INSERT_DISABLE,
      /*vlan_tag_mode        */ WP_IW_VLAN_TAG_DISABLE,
      /*interruptqueue;      */ WP_IW_IRQT0,
      /*error_pkt_mode       */ WP_IW_ERRPKT_DISCARD,
      /*intmode;             */ WP_IW_INT_DISABLE,
      /*statmode;            */ WP_IW_STAT_ENABLE,
      /*timestamp_mode;      */ WP_IW_TIME_STAMP_DISABLE,
      /*ov_pool_mode         */ WP_IW_OV_POOL_DISABLE,
      /*fbp_drop_threshold;  */ 0,
      /*replace_vlan_id      */ WP_IW_REPLACE_VTAG_DISABLE,
      /*vlan_id              */ 0,
      /*vpmt_handle          */ 0,
      /*mtu;                 */ 500,
      /*prefix_length        */ 0,
      /*prefix_header[36];   */ {},
    };
    

    for(i=0;i<2;i++)
    {
        agg_host_config_vlan_aware.txfunc = rx_host_channel_array[i];
        agg_host_array[i] = WP_IwFlowAggregationCreate(WP_WINPATH(0), WP_IW_VLAN_AWARE_BRIDGING_MODE, &agg_host_config_vlan_aware);
        App_TerminateOnError(agg_host_array[i], "WP_IwFlowAggregationCreate() Host Array",__LINE__);
    }

    WP_iw_agg_hq agg_host_config =
    {
        /* tag */            0,
        /* txfunc */         0,
        /* hq_agg_mode */    WP_IW_HQ_NOT_RESERVED,
        /* mtu */            1000,
        /* interruptqueue */ WP_IW_IRQT0,
        /* intmode */        WP_IW_INT_DISABLE,
        /* statmode */       WP_IW_STAT_ENABLE,
        /* timestamp_mode */ WP_IW_TIME_STAMP_DISABLE,
        /* ov_pool_mode */   WP_IW_OV_POOL_DISABLE,
        /* fbp_drop_threshold */ 0,
        /* tx_direction */   WP_IW_NETWORK_GLOBAL,
        /* flow_analyze */   WP_IW_FLOW_PASS,
        /* force_analyze */  WP_IW_FORCE_NO,
        /* l4_checksum_recal */ WP_IW_L4_NO_CHECKSUM,
        /* nexthop */        0,
        /* flow_agg_type */  WP_IW_AGG_PRIMARY,
        /* monitor_queue_size */ 0,
        /* iw_port */        0,
        /* host_term_control */ WP_IW_HQ_CONTROL_NONE
    };
    for(i=2;i<4;i++)
    {
        agg_host_config.txfunc = rx_host_channel_array[i];
        agg_host_array[i] = WP_IwFlowAggregationCreate(WP_WINPATH(0), WP_IW_HOST_QUEUE_MODE, &agg_host_config);
        App_TerminateOnError(agg_host_array[i], "WP_IwFlowAggregationCreate() Host",__LINE__);
    }

}


void    WPE_CreateHostTermFlowAggArrayWithPolicerV1(void)
{
    WP_U32 i = 0;
    WP_iw_agg_bridging  agg_host_config_vlan_aware =

   {
      /*tag                  */ 0,
      /*txfunc;              */ 0,
      /*iw_port;             */ 0,
      /*rfcs                 */ WP_IW_RFCS_DISABLE,
      /*l2header_insert_mode;*/ WP_IW_L2H_INSERT_DISABLE,
      /*vlan_tag_mode        */ WP_IW_VLAN_TAG_DISABLE,
      /*interruptqueue;      */ WP_IW_IRQT0,
      /*error_pkt_mode       */ WP_IW_ERRPKT_DISCARD,
      /*intmode;             */ WP_IW_INT_DISABLE,
      /*statmode;            */ WP_IW_STAT_ENABLE,
      /*timestamp_mode;      */ WP_IW_TIME_STAMP_DISABLE,
      /*ov_pool_mode         */ WP_IW_OV_POOL_DISABLE,
      /*fbp_drop_threshold;  */ 0,
      /*replace_vlan_id      */ WP_IW_REPLACE_VTAG_DISABLE,
      /*vlan_id              */ 0,
      /*vpmt_handle          */ 0,
      /*mtu;                 */ 1500,
      /*prefix_length        */ 0,
      /*prefix_header[36];   */ {},
   };
    

    for(i=0;i<2;i++)
    {
        agg_host_config_vlan_aware.txfunc = rx_host_channel_array[i];
        agg_host_config_vlan_aware.policer_enable = WP_IW_FLOW_AGG_POLICER_HANDLE;
        agg_host_config_vlan_aware.policer_handle = policer_handle_v1[0];
        agg_host_array[i] = WP_IwFlowAggregationCreate(WP_WINPATH(0), WP_IW_VLAN_AWARE_BRIDGING_MODE, &agg_host_config_vlan_aware);
        App_TerminateOnError(agg_host_array[i], "WP_IwFlowAggregationCreate() Host Array",__LINE__);
    }

    WP_iw_agg_hq agg_host_config =
    {
        /* tag */            0,
        /* txfunc */         0,
        /* hq_agg_mode */    WP_IW_HQ_NOT_RESERVED,
        /* mtu */            1500,
        /* interruptqueue */ WP_IW_IRQT0,
        /* intmode */        WP_IW_INT_DISABLE,
        /* statmode */       WP_IW_STAT_ENABLE,
        /* timestamp_mode */ WP_IW_TIME_STAMP_DISABLE,
        /* ov_pool_mode */   WP_IW_OV_POOL_DISABLE,
        /* fbp_drop_threshold */ 0,
        /* tx_direction */   WP_IW_NETWORK_GLOBAL,
        /* flow_analyze */   WP_IW_FLOW_PASS,
        /* force_analyze */  WP_IW_FORCE_NO,
        /* l4_checksum_recal */ WP_IW_L4_NO_CHECKSUM,
        /* nexthop */        0,
        /* flow_agg_type */  WP_IW_AGG_PRIMARY,
        /* monitor_queue_size */ 0,
        /* iw_port */        0,
        /* host_term_control */ WP_IW_HQ_CONTROL_NONE
    };
    for(i=2;i<4;i++)
    {
        agg_host_config.txfunc = rx_host_channel_array[i];
        agg_host_config.policer_mode = WP_IW_FLOW_AGG_POLICER_HANDLE;
        agg_host_config.policer_handle = policer_handle_v1[1];
        agg_host_array[i] = WP_IwFlowAggregationCreate(WP_WINPATH(0), WP_IW_HOST_QUEUE_MODE, &agg_host_config);
        App_TerminateOnError(agg_host_array[i], "WP_IwFlowAggregationCreate() Host",__LINE__);
    }
   
}


void    WPE_CreateHostTermFlowAggArrayWithPolicerV2(void)
{
    WP_U32 i = 0;
    WP_iw_agg_bridging  agg_host_config_vlan_aware =

    {
      /*tag                  */ 0,
      /*txfunc;              */ 0,
      /*iw_port;             */ 0,
      /*rfcs                 */ WP_IW_RFCS_DISABLE,
      /*l2header_insert_mode;*/ WP_IW_L2H_INSERT_DISABLE,
      /*vlan_tag_mode        */ WP_IW_VLAN_TAG_DISABLE,
      /*interruptqueue;      */ WP_IW_IRQT0,
      /*error_pkt_mode       */ WP_IW_ERRPKT_DISCARD,
      /*intmode;             */ WP_IW_INT_DISABLE,
      /*statmode;            */ WP_IW_STAT_ENABLE,
      /*timestamp_mode;      */ WP_IW_TIME_STAMP_DISABLE,
      /*ov_pool_mode         */ WP_IW_OV_POOL_DISABLE,
      /*fbp_drop_threshold;  */ 0,
      /*replace_vlan_id      */ WP_IW_REPLACE_VTAG_DISABLE,
      /*vlan_id              */ 0,
      /*vpmt_handle          */ 0,
      /*mtu;                 */ 1500,
      /*prefix_length        */ 0,
      /*prefix_header[36];   */ {},
    };
    

    for(i=0;i<2;i++)
{
        agg_host_config_vlan_aware.txfunc = rx_host_channel_array[i];
        agg_host_array[i] = WP_IwFlowAggregationCreate(WP_WINPATH(0), WP_IW_VLAN_AWARE_BRIDGING_MODE, &agg_host_config_vlan_aware);
        App_TerminateOnError(agg_host_array[i], "WP_IwFlowAggregationCreate() Host Array",__LINE__);
    }

    WP_iw_agg_generic agg_enet_config =
    {
        
        /*tag*/ agg_tag_counter++,
        /* txfunc */         0,
        /* iw_port */        0,
        /*rfcs*/WP_IW_RFCS_ENABLE,
        /*interruptqueue;*/WP_IW_IRQT1,
        /*error_pkt_mode*/WP_IW_ERRPKT_DISCARD,
        /*intmode;*/WP_IW_INT_DISABLE,
        /*statmode;*/WP_IW_STAT_ENABLE,
        /*timestamp_mode;*/WP_IW_TIME_STAMP_DISABLE,
        /*mtu;*/1500,
        /*flow_agg_type;*/WP_IW_FLOW_AGG_PRIMARY,
        /*policer_handle;*/0,
        /*pecs_handle;*/0,
        /*pecs_flow_info;*/0,
        /*pecs_global_info_handle;*/0,
        /*iw_gpmt_handle*/0,
        
    };
    for(i=2;i<4;i++)
    {
        agg_enet_config.txfunc = rx_host_channel_array[i];
        agg_enet_config.policer_handle = policer_handle_v2[i-2];
        agg_host_array[i] = WP_IwFlowAggregationCreate(WP_WINPATH(0), WP_IW_GENERIC_MODE, &agg_enet_config);
        App_TerminateOnError(agg_host_array[i], "WP_IwFlowAggregationCreate() Host",__LINE__);
    }
   
}


void    WPE_CreateGeEnetIwBport(void)
{

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
            /* default_vlan_tag */     1,
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
        /*res_mc_action_table*/ {0,0,0,0,0,0,0,0,0,0},         
        /**enet_oam_params*/    NULL,
        /* dfc_log_mode*/       0
    };

   // Only one bPort is possible for one Enet device because only one RX channel is possible

   enet_bport_config.input_filters_mask = 0x00000000;
   enet_bport_config.flow_agg = agg_host; // default flow agg
   bport_enet_ge = WP_IwPortCreate(iw_sys, &enet_bport_config);
   App_TerminateOnError(bport_enet_ge," WPE_CreateIwBportEnet Fast",__LINE__);
}


void    WPE_CreateGeEnetIwBport_Host_3(void)
{

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
            /* default_vlan_tag */     1,
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
         /*res_mc_action_table*/ {0,0,0,0,0,0,0,0,0,0},
         /**enet_oam_params*/    NULL,
         /* dfc_log_mode*/       0
      };

   // Only one bPort is possible for one Enet device because only one RX channel is possible

   enet_bport_config.input_filters_mask = 0x80000000;
   enet_bport_config.flow_agg = agg_host; // default flow agg
   bport_enet_ge = WP_IwPortCreate(iw_sys, &enet_bport_config);
   App_TerminateOnError(bport_enet_ge," WPE_CreateIwBportEnet Fast",__LINE__);
}

void    WPE_CreateGeEnetIwBport_Host_7(void)
{

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
            /* default_vlan_tag */     1,
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
         /*res_mc_action_table*/ {0,0,0,0,0,0,0,0,0,0},
         /**enet_oam_params*/    NULL,
         /* dfc_log_mode*/       0
      };

   // Only one bPort is possible for one Enet device because only one RX channel is possible

   enet_bport_config.input_filters_mask = 0x00000000;
   enet_bport_config.flow_agg = agg_host_array[0]; // default flow agg
   bport_enet_ge = WP_IwPortCreate(iw_sys, &enet_bport_config);
   App_TerminateOnError(bport_enet_ge," WPE_CreateIwBportEnet Fast",__LINE__);
}


void    WPE_CreateG999EnetIwBport(void)
{

   WP_bridge_port enet_bport_config = 
      { 
         /* tag */               BRIDGE_PORT_HIER_ENET_TAG,
         /* direct_mapping */    WP_IW_DIRECT_MAP_DISABLE, 
         /* flow_agg */          0,
         /* flooding_term_mode*/ WP_IW_HOST_TERM_MODE, 
         /* learning_mode */     WP_IW_LEARNING_DISABLE, 
         /* in_filter_mode */    WP_IW_INGRESS_FILTER_DISABLE,
         /* vlan_param */ 
         { 
            /* vlan_acceptance_mode */ WP_IW_ACCEPT_TAGGED_UNTAGGED, 
            /* default_vlan_tag */     1,
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
         /*res_mc_action_table*/ {0,0,0,0,0,0,0,0,0,0},
         /**enet_oam_params*/    NULL,
         /* dfc_log_mode*/       0
      };



   enet_bport_config.input_filters_mask = 0x00000000;
   enet_bport_config.flow_agg = agg_host; // default flow agg
   bport_enet_g999 = WP_IwPortCreate(iw_sys, &enet_bport_config);
   App_TerminateOnError(bport_enet_g999," WP_IwPortCreate Enet G999",__LINE__);
      
}



void    WPE_CreateG999EnetIwBport_Host_7(void)
{

   WP_bridge_port enet_bport_config = 
      { 
         /* tag */               BRIDGE_PORT_HIER_ENET_TAG,
         /* direct_mapping */    WP_IW_DIRECT_MAP_DISABLE, 
         /* flow_agg */          0,
         /* flooding_term_mode*/ WP_IW_HOST_TERM_MODE, 
         /* learning_mode */     WP_IW_LEARNING_DISABLE, 
         /* in_filter_mode */    WP_IW_INGRESS_FILTER_DISABLE,
         /* vlan_param */ 
         { 
            /* vlan_acceptance_mode */ WP_IW_ACCEPT_TAGGED_UNTAGGED, 
            /* default_vlan_tag */     1,
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
         /*res_mc_action_table*/ {0,0,0,0,0,0,0,0,0,0},
         /**enet_oam_params*/    NULL,
         /* dfc_log_mode*/       0
      };



   enet_bport_config.input_filters_mask = 0x00000000;
   enet_bport_config.flow_agg = agg_host_array[1]; // default flow agg
   bport_enet_g999 = WP_IwPortCreate(iw_sys, &enet_bport_config);
   App_TerminateOnError(bport_enet_g999," WP_IwPortCreate Enet G999",__LINE__);
      
}




void    WPE_CreateGeEnetFlowAgg(void)
{
    WP_U32 i;

    for(i=0;i < NUM_OF_GE_ENET_TX_CHANNELS; i++)
    {

        WP_iw_agg_generic agg_enet_config =
        {
            
            /*tag*/ agg_tag_counter++,
            /*txfunc*/ 0,
            /*iw_port*/0,
            /*rfcs*/WP_IW_RFCS_ENABLE,
            /*interruptqueue;*/WP_IW_IRQT1,
            /*error_pkt_mode*/WP_IW_ERRPKT_DISCARD,
            /*intmode;*/WP_IW_INT_DISABLE,
            /*statmode;*/WP_IW_STAT_ENABLE,
            /*timestamp_mode;*/WP_IW_TIME_STAMP_DISABLE,
            /*mtu;*/MTU_SIZE_GE,
            /*flow_agg_type;*/WP_IW_FLOW_AGG_PRIMARY,
            /*policer_handle;*/0,
            /*pecs_handle;*/0,
            /*pecs_flow_info;*/0,
            /*pecs_global_info_handle;*/0,
            /*iw_gpmt_handle*/0,
            
        };
        agg_enet_config.iw_port = bport_enet_ge;

        agg_enet_config.txfunc = tx_ge_enet_channel[i];

        ge_enet_tx_flow_agg[i] =  WP_IwFlowAggregationCreate(WP_WINPATH(0),
                                          WP_IW_GENERIC_MODE,
                                          &agg_enet_config);   
        App_TerminateOnError(ge_enet_tx_flow_agg[i], "WP_IwFlowAggregationCreate GE ENET",__LINE__);
    }
}



void    WPE_CreateG999EnetFlowAgg(void)
{
    WP_U32 i;

    for(i=0;i < NUM_OF_G999_ENET_TX_CHANNELS; i++)
    {

        WP_iw_agg_generic agg_enet_config =
        {
            
            /*tag*/ agg_tag_counter++,
            /*txfunc*/ 0,
            /*iw_port*/0,
            /*rfcs*/WP_IW_RFCS_ENABLE,
            /*interruptqueue;*/WP_IW_IRQT1,
            /*error_pkt_mode*/WP_IW_ERRPKT_DISCARD,
            /*intmode;*/WP_IW_INT_DISABLE,
            /*statmode;*/WP_IW_STAT_ENABLE,
            /*timestamp_mode;*/WP_IW_TIME_STAMP_DISABLE,
            /*mtu;*/MTU_SIZE_G999,
            /*flow_agg_type;*/WP_IW_FLOW_AGG_PRIMARY,
            /*policer_handle;*/0,
            /*pecs_handle;*/0,
            /*pecs_flow_info;*/0,
            /*pecs_global_info_handle;*/0,
            /*iw_gpmt_handle*/0,
           
        };
        agg_enet_config.iw_port = bport_enet_g999;

        agg_enet_config.txfunc = tx_g999_enet_channel[i];

        g999_enet_tx_flow_agg[i] =  WP_IwFlowAggregationCreate(WP_WINPATH(0),
                                          WP_IW_GENERIC_MODE,
                                          &agg_enet_config);  
        App_TerminateOnError(g999_enet_tx_flow_agg[i], "WP_IwFlowAggregationCreate G999 ENET",__LINE__);
    }
}



void    WPE_CreateGeEnetRxTxBinding(void)
{
   WP_handle status;
   WP_U32 i;

   WP_rx_binding_bridging  rx_binding_enet_config =
      {
         /*  encap_mode */   0, // Not Used in Enet
         /*  mru;*/          SDU_SIZE,
         /*  vcfcs;*/        0,
         /*  input_port;*/   bport_enet_ge
      };

   WP_tx_binding  tx_binding_enet_config =
      {
         /* res0 */      0, // Unused
         /* dci_mode;*/  WP_IW_DYN_CH_INSERT_ENABLE,
         /* maxt;*/      MAXT_SIZE_GE
      };

   // Ge Enet
   status = WP_IwRxBindingCreate(rx_ge_enet_channel, iw_sys, iw_qnode, &rx_binding_enet_config);
   App_TerminateOnError(status, "WP_IwRxBindingCreate() rx_binding_enet Ge",__LINE__);

   for(i=0;i < NUM_OF_GE_ENET_TX_CHANNELS; i++) 
   {
      status = WP_IwTxBindingCreate(tx_ge_enet_channel[i], WP_IW_TX_BINDING, &tx_binding_enet_config);
      App_TerminateOnError(status, "WP_IwTxBindingCreate Ge ENET",__LINE__);
   }
   
}


void    WPE_CreateG999EnetRxTxBinding(void)
{  
   WP_handle status;
   WP_U32 i;

   WP_rx_binding_bridging  rx_binding_enet_config =
      {
         /*  encap_mode */   0, // Not Used in Enet
         /*  mru;*/          SDU_SIZE,
         /*  vcfcs;*/        0,
         /*  input_port;*/   bport_enet_g999
      };

   WP_tx_binding  tx_binding_enet_config =
      {
         /* res0 */      0, // Unused
         /* dci_mode;*/  WP_IW_DYN_CH_INSERT_DISABLE,
         /* maxt;*/      MAXT_SIZE_G999
      };


   // G999 Enet
   rx_binding_enet_config.input_port = bport_enet_g999;
   status = WP_IwRxBindingCreate(rx_g999_enet_channel, iw_sys, iw_qnode, &rx_binding_enet_config);
   App_TerminateOnError(status, "WP_IwRxBindingCreate() rx_binding_enet G999",__LINE__);
   for(i=0;i < NUM_OF_G999_ENET_TX_CHANNELS; i++) 
   {
      status = WP_IwTxBindingCreate(tx_g999_enet_channel[i], WP_IW_TX_BINDING, &tx_binding_enet_config);
      App_TerminateOnError(status, "WP_IwTxBindingCreate G999 ENET",__LINE__);
   }
   
}




void    WPE_CreateGeEnetRxTxChannel(void)
{
    WP_U32 i;
   WP_ch_enet enet_ch_config =
      {
         /* intmode */         WP_PKTCH_INT_DISABLE,
         /* iwmode */          WP_PKTCH_IWM_ENABLE,
         /* testmode */        WP_PKTCH_TEST_DISABLE,
         /* tx_pqblock */      0, // Unused in HW mode
         /* tx_pqlevel */      0, // Unused in HW mode
         /* tx_shaping_type*/  WP_FMU_SHAPING_TYPE_STRICT,//WP_FMU_SHAPING_TYPE_STRICT,
         /* tx_shaping_params*/NULL,
         /* rx_maxsdu */       SDU_SIZE,
         /* tx_cwid */         WP_CW_ID_A, // Unused in HW mode
         /* tx_tq */           0,
         /* rx_queuedepth */   10,
      };

   rx_ge_enet_channel = WP_ChannelCreate(channel_tag_counter++, dev_enet_ge, iw_qnode, WP_CH_RX, WP_ENET, &enet_ch_config);
   App_TerminateOnError(rx_ge_enet_channel, "WP_Channel_Create() GE RX",__LINE__);

    for(i=0;i < NUM_OF_GE_ENET_TX_CHANNELS; i++)
    {
        enet_ch_config.tx_tq = i;
        tx_ge_enet_channel[i] = WP_ChannelCreate(channel_tag_counter++, l2_group_ge[0][0], iw_qnode, WP_CH_TX, WP_ENET, &enet_ch_config);
        App_TerminateOnError(tx_ge_enet_channel[i], "WP_Channel_Create() GE TX",__LINE__);
    }
   

}

void    WPE_CreateG999EnetRxTxChannel(void)
{
   WP_U32 i;

   WP_ch_enet enet_ch_config =
      {
         /* intmode */         WP_PKTCH_INT_DISABLE,
         /* iwmode */          WP_PKTCH_IWM_DISABLE,
         /* testmode */        WP_PKTCH_TEST_DISABLE,
         /* tx_pqblock */      0, // Unused in HW mode
         /* tx_pqlevel */      0, // Unused in HW mode

         /* tx_shaping_type*/  WP_FMU_SHAPING_TYPE_STRICT,
         /* tx_shaping_params*/NULL,

         /* rx_maxsdu */       SDU_SIZE,
         /* tx_cwid */         WP_CW_ID_A, // Unused in HW mode
         /* tx_tq */           0,
         /* rx_queuedepth */   10,
      };

   rx_g999_enet_channel_on_dev = WP_ChannelCreate(channel_tag_counter++, dev_enet_g999, iw_qnode, WP_CH_RX, WP_ENET, &enet_ch_config);
   App_TerminateOnError(rx_g999_enet_channel_on_dev, "WP_Channel_Create() Gige G999 RX",__LINE__);


   enet_ch_config.iwmode  = WP_PKTCH_IWM_ENABLE;

   rx_g999_enet_channel = WP_ChannelCreate(channel_tag_counter++, g999_stream, iw_qnode, WP_CH_RX, WP_ENET, &enet_ch_config);
   App_TerminateOnError(rx_g999_enet_channel, "WP_Channel_Create() Gige G999 RX",__LINE__);

    for(i=0;i < NUM_OF_G999_ENET_TX_CHANNELS; i++)
    {
        enet_ch_config.tx_tq = i;
        tx_g999_enet_channel[i] = WP_ChannelCreate(channel_tag_counter++, l2_group_g999[0][0], iw_qnode, WP_CH_TX, WP_ENET, &enet_ch_config);
        App_TerminateOnError(tx_g999_enet_channel[i], "WP_Channel_Create() G999 TX",__LINE__);
    }
}




void WPE_CreatePceFilters(void)
{

        WP_pce_filter_classification filter_class = {0};

//filter1
        /* create classification filter */
        filter_class.no_match_action = WP_PCE_FILTER_NO_MATCH_DENY;
        filter_class.no_fields_action = WP_PCE_FILTER_NO_FIELDS_DENY;

        filter_class.no_match_result[0].result_type = WP_PCE_RESULT_LAST;

        filter_class.filter_fields[0].field_id = WP_PCE_FIELD_ID_INPUT_IW_PORT;
        filter_class.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
        filter_class.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

        filter_class.filter_fields[1].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
        filter_class.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_USED;
        filter_class.filter_fields[1].mask.vlan_tag = WP_PCE_FIELD_MASK_VLAN_ID;
        filter_class.filter_fields[1].field_id = WP_PCE_FIELD_ID_VLAN_TAG;
        
        filter_class.filter_fields[2].field_id = WP_PCE_FIELD_ID_LAST;
        
        PCE_filter[0] = WP_PceFilterCreate(WP_WINPATH(0),
                                                                   WP_PCE_FILTER_CLASSIFICATION,
                                                                   &filter_class);
        App_TerminateOnError(PCE_filter[0], "WP_PceFilterCreate",__LINE__);

//filter 2
        memset(&filter_class,0,sizeof(WP_pce_filter_classification));
                /* create classification filter */
        filter_class.no_match_action = WP_PCE_FILTER_NO_MATCH_CONTINUE;
        filter_class.no_fields_action = WP_PCE_FILTER_NO_FIELDS_DENY;

        filter_class.no_match_result[0].result_type = WP_PCE_RESULT_LAST;

        filter_class.filter_fields[0].field_id = WP_PCE_FIELD_ID_INPUT_IW_PORT;
        filter_class.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
        filter_class.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;


        filter_class.filter_fields[1].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
	filter_class.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;
	filter_class.filter_fields[1].field_id = WP_PCE_FIELD_ID_MAC_SA;
        
        filter_class.filter_fields[2].field_id = WP_PCE_FIELD_ID_LAST;
        
        PCE_filter[1] = WP_PceFilterCreate(WP_WINPATH(0),
                                                                   WP_PCE_FILTER_CLASSIFICATION,
                                                                   &filter_class);
        App_TerminateOnError(PCE_filter[1], "WP_PceFilterCreate",__LINE__);
}


void WPE_CreatePceFilterSets(void)
{
    WP_pce_filter_set fs_level;

    /* filter set with learning */
    fs_level.filter_set_level = 0;
    fs_level.next_filter_set = WP_UNUSED;
    fs_level.filters[0] = PCE_filter[0];
    fs_level.filters[1] = PCE_filter[1];
    fs_level.filters[2] = WP_UNUSED;

    filter_set_handle = WP_PceFilterSetCreate(WP_WINPATH(0), &fs_level);
    App_TerminateOnError(filter_set_handle, "WP_PceFilterSetCreate",__LINE__);
}


void WPE_CreateRxChannelPceInterface(WP_handle iw_sys)
{
    WP_pce_if_params_pkt_rx_channel pce_if_params = {0};
    WP_handle pce_if_handle;
    WP_status status;
    WP_rx_binding_bridging  rx_binding_cfg[1]=
        {
                {
                        /*  encap_mode */0,
                        /*  mru;*/512,
                        /*  vcfcs;*/0,
                        /*  input_port;*/0
                }
        };

    pce_if_params.mode = WP_PCE_IW_PORT_CONNECTION_ENABLED;
    pce_if_params.parser_start_type = WP_PCE_PARSER_START_TYPE_ETHERNET;
    pce_if_params.filter_set_handle = filter_set_handle;
    pce_if_params.ip_header_validation = WP_DISABLE;

    pce_if_handle = WP_PceInterfaceCreate(WP_PCE_IF_TYPE_PKT_RX_CHANNEL, &pce_if_params);
    App_TerminateOnError(pce_if_handle,"WP_PceInterfaceCreate()",__LINE__);

    /* Modify the PCE interface for  RX channel handle */
    rx_binding_cfg[0].pce_if_handle = pce_if_handle;
    status = WP_IwRxBindingModify(rx_ge_enet_channel,
                                  iw_sys,
                                  iw_qnode,
                                  WP_IW_RX_BIND_MOD_PCE_INTERFACE,
                                  &rx_binding_cfg[0]);
    App_TerminateOnError(status,"WP_IwRxBindingModify",__LINE__);
}


void WPE_CreateG999DevicePceInterface(void)
{
    WP_pce_if_params_pkt_rx_channel pce_if_params = {0};
    WP_handle pce_if_handle;
    WP_status status;
    WP_rx_binding_bridging  rx_binding_cfg[1]=
    {
    {
                        /*  encap_mode */0,
                        /*  mru;*/512,
                        /*  vcfcs;*/0,
                        /*  input_port;*/0
    }
        };

    pce_if_params.mode = WP_PCE_IW_PORT_CONNECTION_ENABLED;
    pce_if_params.parser_start_type = WP_PCE_PARSER_START_TYPE_ETHERNET;
    pce_if_params.filter_set_handle = filter_set_handle;
    pce_if_params.ip_header_validation = WP_DISABLE;

    pce_if_handle = WP_PceInterfaceCreate(WP_PCE_IF_TYPE_PKT_RX_CHANNEL, &pce_if_params);
    App_TerminateOnError(pce_if_handle,"WP_PceInterfaceCreate()",__LINE__);

    /* Modify the PCE interface for  RX channel handle */
    rx_binding_cfg[0].pce_if_handle = pce_if_handle;
    status = WP_IwRxBindingModify(rx_g999_enet_channel,
                                  iw_sys,
                                  iw_qnode,
                                  WP_IW_RX_BIND_MOD_PCE_INTERFACE,
                                  &rx_binding_cfg[0]);
    App_TerminateOnError(status,"WP_IwRxBindingModify",__LINE__);
}



void	WPE_EnableGeShapingGroup(void)
{
    WP_status status;
    WP_U32 entry, i;
    for(entry = 0; entry < NUM_OF_L1_GROUP; entry ++)
    {
        status = WP_ShapingGroupEnable(l1_group_ge[entry]);
        App_TerminateOnError(status, "L1 WP_ShapingGroupEnable",  __LINE__);
        for(i = 0; i < NUM_OF_L2_GROUP; i ++)
        {
            status = WP_ShapingGroupEnable(l2_group_ge[entry][i]);
            App_TerminateOnError(status, "L2 WP_ShapingGroupEnable",  __LINE__);
        }
    }

}


void	WPE_EnableG999ShapingGroup(void)
{
    WP_status status;
    WP_U32 entry, i;
    for(entry = 0; entry < NUM_OF_L1_GROUP; entry ++)
    {
        status = WP_ShapingGroupEnable(l1_group_g999[entry]);
        App_TerminateOnError(status, "L1 WP_ShapingGroupEnable",  __LINE__);
        for(i = 0; i < NUM_OF_L2_GROUP; i ++)
        {
            status = WP_ShapingGroupEnable(l2_group_g999[entry][i]);
            App_TerminateOnError(status, "L2 WP_ShapingGroupEnable",  __LINE__);
        }
    }

}

/****************************************************************************************************************************
* Function name: App_DataSend()
* Description  : Send a Ethernet packet on the channel handle passed in 
*
* Input  params:  h_tx:   enet1 host tx
*                 h_pool: memory pool buffer
* Output params: 
* Return val   : none
*****************************************************************************************************************************/
void App_DataSend(WP_handle h_tx, WP_handle h_pool)
{
#define APP_DATA_LENGTH 80

   WP_data_unit data_unit = {0} ;
   WP_data_segment bufs = {0};
   WP_status status;
   WP_U8 * km_data_ptr;
   WP_U32 ii;
   WP_U8 dst_mac[6] = {0x00,0x00,0x22,0x33,0x12,0x06};
   WP_U8 src_mac[6] = {0x00,0x00,0x00,0x00,0x00,0x02};
   WP_U8 vlan_1[4] = {0x81,0x00,0x00,0xa};
   WP_U8 e_type[2] = {0x08,0x00}; /* IP type*/
   WP_U32 l2_header_len = sizeof(dst_mac) + sizeof(src_mac) + sizeof(vlan_1) + 
         sizeof(e_type);

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
   data_unit.status= 0;
   data_unit.control= 0;/* Tx protocol specific settings    */

   km_data_ptr = WP_PoolAlloc(h_pool);
   bufs.data = km_data_ptr;
   if(bufs.data == NULL)
   {
      printf("Pool Alloc failure ! \n");
   }

   WP_MEM_BYTES_FILL(km_data_ptr, 0, IW_BUFFER_SIZE);
   offset = 0;
   memcpy(km_data_ptr + offset,dst_mac,sizeof(dst_mac));
   offset += sizeof(dst_mac);
   memcpy(km_data_ptr+offset,src_mac,sizeof(src_mac));
   offset += sizeof(src_mac);
   memcpy(km_data_ptr+offset,vlan_1,sizeof(vlan_1));
   offset += sizeof(vlan_1);
   memcpy(km_data_ptr+offset,e_type,sizeof(e_type));
   offset += sizeof(e_type);

   payload_len = APP_DATA_LENGTH - l2_header_len;



   memset(km_data_ptr+offset,0xa,payload_len);

   printf("sending packet( ethernet vlan IP) of length %d\n",APP_DATA_LENGTH);
   for(ii = 0;ii < APP_DATA_LENGTH;ii++)
   {
      printf("%.2x",*(km_data_ptr+ii));
      if((ii != 0) && !(ii % 40))
         printf("\n");
   }
   printf("\n");
   /* Use interworking qnode and interworking buffer pool for Gige */
   status = WP_HostSend(h_tx, &data_unit);
   App_TerminateOnError(status, "WP_HostSend()",__LINE__);
   WP_Delay(500000);
   WPI_SimulateInterrupts();
}

void    WPE_EnableSystem(void)
{
   WP_handle status;
   WP_U32    i;

   /* Enable the Enet Channels First TX than RX*/
   for(i=0;i < NUM_OF_GE_ENET_TX_CHANNELS; i++) 
   {
      status = WP_ChannelEnable(tx_ge_enet_channel[i]);
      App_TerminateOnError(status, "WP_ChannelEnable() tx ge",__LINE__);
   }

   for(i=0;i < NUM_OF_G999_ENET_TX_CHANNELS; i++) 
   {
      status = WP_ChannelEnable(tx_g999_enet_channel[i]);
      App_TerminateOnError(status, "WP_ChannelEnable() tx g999",__LINE__);
   }
  
   status = WP_ChannelEnable(rx_ge_enet_channel);
   App_TerminateOnError(status, "WP_ChannelEnable() rx ge",__LINE__);


   status = WP_ChannelEnable(rx_g999_enet_channel);
   App_TerminateOnError(status, "WP_ChannelEnable() rx g999",__LINE__);

   status = WP_ChannelEnable(rx_g999_enet_channel_on_dev);
   App_TerminateOnError(status, "WP_ChannelEnable() rx g999 on device",__LINE__);


   status = WP_PortEnable(port_enet_ge, WP_DIRECTION_DUPLEX);
   App_TerminateOnError(status, "WP_PortEnable Ge Enet",__LINE__);

   status = WP_DeviceEnable(dev_enet_ge, WP_DIRECTION_DUPLEX);
   App_TerminateOnError(status, "WP_DeviceEnable Ge Enet",__LINE__);


   status = WP_PortEnable(port_enet_g999, WP_DIRECTION_DUPLEX);
   App_TerminateOnError(status, "WP_PortEnable G999 Enet",__LINE__);

   status = WP_DeviceEnable(dev_enet_g999, WP_DIRECTION_DUPLEX);
   App_TerminateOnError(status, "WP_DeviceEnable G999 Enet",__LINE__);

   WPE_EnableG999ShapingGroup();

   WPE_EnableGeShapingGroup();
 
   WPE_EnableG999Stream();


}


void    WPE_EnableSystem_Host(void)
{
   WP_handle status;
   WP_U32    i;

   /* Enable the Enet Channels First TX than RX*/
   for(i=0;i < NUM_OF_GE_ENET_TX_CHANNELS; i++) 
   {
      status = WP_ChannelEnable(tx_ge_enet_channel[i]);
      App_TerminateOnError(status, "WP_ChannelEnable() tx ge",__LINE__);
   }


   status = WP_ChannelEnable(rx_ge_enet_channel);
   App_TerminateOnError(status, "WP_ChannelEnable() rx ge",__LINE__);


   status = WP_PortEnable(port_enet_ge, WP_DIRECTION_DUPLEX);
   App_TerminateOnError(status, "WP_PortEnable Ge Enet",__LINE__);

   status = WP_DeviceEnable(dev_enet_ge, WP_DIRECTION_DUPLEX);
   App_TerminateOnError(status, "WP_DeviceEnable Ge Enet",__LINE__);


   WPE_EnableGeShapingGroup();


}





void    WPE_CreateHostPortDevice(void)
{
   port_host = WP_PortCreate(WP_WINPATH(0), WP_PORT_IW_HOST, NULL);
   App_TerminateOnError(port_host, "WP_PortCreate() HOST",__LINE__);

   dev_host = WP_DeviceCreate(port_host, 0, WP_DEVICE_IW_HOST, NULL);
   App_TerminateOnError(dev_host, "WP_DeviceCreate() Host",__LINE__);
}



void    WPE_CreateGeEnetPortDevice(void)
{
    WP_status status;

   WP_port_enet enet_port_config =
      {
         /* pkt_limits */
         {
            /* max_tx_channels*/ NUM_OF_GE_ENET_TX_CHANNELS,
            /* max_rx_channels*/ NUM_OF_GE_ENET_RX_CHANNELS,
            /* pq_block_size*/   0,
            /* n_pq_blocks*/     0,
            /* emphy_devices*/   WP_UNUSED
         },
         /* flowmode */       WP_ENET_FMU_HIERARCHICAL_SHAPING_MODE,
         /* interface_mode */ WP_ENET_SGMII_1000,
         /* rx_iw_bkgnd */    0,       
      };

   WP_device_enet enet_dev_config =
      {
         /* max_tx_channels */ NUM_OF_GE_ENET_TX_CHANNELS,
         /* tx_maxsdu */       SDU_SIZE,
         /* operating_speed */ WP_UNUSED,       // Valid for RMII, RGMII only
         /* mac_addr*/         {0xaa,0xaa,0xaa,0xaa,0xaa,0xaa}, // MAC of this device
         /* tx_bit_rate*/      WP_TX_BIT_RATE_UNLIMITED,
         /* loopbackmode */    WP_ENET_NORMAL,
         /* extended_params */ NULL             // Must be NULL at device creation
      };

      WP_enet_tmu_compensation enet_device_tmu_compensation =
        
        {
                WP_ENET_TMU_COMPENSATION_ENABLE,
                WP_ENET_TMU_COMPENSATION_FCS,
                0,
        };



   WP_device_enet_ex enet_device_extended_config = //{0};
#if USE_XGI
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
        /*rx_filters*/           NULL,
        /*rx_timestamp*/         WP_DISABLE,
        /*timestamp_type*/       0,
        /*max_tx_slow_channels*/ 0,
     };
#else
    {0};
#endif

#if USE_XGI
        /* XGI port*/
        enet_port_config.interface_mode = WP_ENET_XAUI;
        port_enet_ge = WP_PortCreate(WP_WINPATH(0), APP_ENET_PORT1, &enet_port_config);
        App_TerminateOnError(port_enet_ge, "WP_PortCreate XGI1",__LINE__);

        /* Create an XGI Device */
        enet_dev_config.extended_params = NULL;
        dev_enet_ge = WP_DeviceCreate(port_enet_ge, 0,WP_DEVICE_ENET, &enet_dev_config);
        App_TerminateOnError(dev_enet_ge, "WP_DeviceCreate() XGI",__LINE__);

        enet_device_extended_config.tx_force_pb = WP_ENET_TX_USE_PB_FORCE;
        enet_device_extended_config.tx_mode_10G = WP_ENET_TX_MODE_10G_LONG_PKT;
        enet_dev_config.extended_params = &enet_device_extended_config;
        status = WP_DeviceModify(dev_enet_ge,
                                 WP_DEV_MOD_ENET_TX_STATMODE |
                                 WP_DEV_MOD_ENET_PHYSTATMODE |
                                 WP_DEV_MOD_ENET_RX_FLOWCONTROL |
                                 WP_DEV_MOD_ENET_TX_FLOWCONTROL |
                                 WP_DEV_MOD_ENET_FORCE_PB_MODE |
                                 WP_DEV_MOD_ENET_10G_LONG_PKT_MODE |
                                 WP_DEV_MOD_ENET_RX_STATMODE ,
                                 &enet_dev_config);
        App_TerminateOnError(status, "WP_DeviceModify() XGI",__LINE__);
#elif USE_RGMII
    enet_port_config.interface_mode = WP_ENET_RGMII_1000;
   port_enet_ge = WP_PortCreate(WP_WINPATH(0), APP_ENET_PORT1, &enet_port_config);
   App_TerminateOnError(port_enet_ge, "WP_PortCreate() GE ENET",__LINE__);
   
   dev_enet_ge = WP_DeviceCreate(port_enet_ge, WP_PHY(0), WP_DEVICE_ENET, &enet_dev_config);
   App_TerminateOnError(dev_enet_ge, "WP_DeviceCreate() GE Enet",__LINE__);

#else
   port_enet_ge = WP_PortCreate(WP_WINPATH(0), APP_ENET_PORT1, &enet_port_config);
   App_TerminateOnError(port_enet_ge, "WP_PortCreate() GE ENET",__LINE__);
   
   dev_enet_ge = WP_DeviceCreate(port_enet_ge, WP_PHY(0), WP_DEVICE_ENET, &enet_dev_config);
   App_TerminateOnError(dev_enet_ge, "WP_DeviceCreate() GE Enet",__LINE__);


   enet_device_extended_config.tmuc = &enet_device_tmu_compensation;

   enet_dev_config.extended_params = &enet_device_extended_config;

   status = WP_DeviceModify(dev_enet_ge,WP_DEV_MOD_ENET_TMU_COMPENSATION,&enet_dev_config);
   App_TerminateOnError(status, "WP_DeviceModify() GE Enet",__LINE__);
#endif
}



void    WPE_CreateG999EnetPortDevice(void)
{
    WP_status status;

    WP_port_enet enet_port_config =
      {
         /* pkt_limits */
         {
            /* max_tx_channels*/ NUM_OF_G999_ENET_TX_CHANNELS,
            /* max_rx_channels*/ NUM_OF_G999_ENET_TX_CHANNELS,
            /* pq_block_size*/   0,
            /* n_pq_blocks*/     0,
            /* emphy_devices*/   WP_UNUSED
         },
         /* flowmode */       WP_ENET_FMU_HIERARCHICAL_SHAPING_MODE,
         /* interface_mode */ WP_ENET_SGMII_1000,
         /* rx_iw_bkgnd */    0,       
      };

   memset(&device_enet_g999_cfg, 0, sizeof (WP_device_enet_g999));
   device_enet_g999_cfg.g999_modes = WP_G999_MODE_ENET;
   device_enet_g999_cfg.max_g999_streams = 1;
   device_enet_g999_cfg.max_fragment_length = G999_MAX_FRAGMENT_LENGTH;
   device_enet_g999_cfg.pce_if_handle = 0;
   device_enet_g999_cfg.loopbackmode = WP_ENET_NORMAL; // Currently works in serdes loopback
   device_enet_g999_cfg.extended_params = NULL;
   device_enet_g999_cfg.g999_atm = NULL;

   port_enet_g999 = WP_PortCreate(WP_WINPATH(0), APP_ENET_PORT2, &enet_port_config);
   App_TerminateOnError(port_enet_g999, "WP_PortCreate() G999 ENET",__LINE__);
   
   dev_enet_g999 = WP_DeviceCreate(port_enet_g999, WP_PHY(0), WP_DEVICE_ENET_G999, &device_enet_g999_cfg);
   App_TerminateOnError(dev_enet_g999, "WP_DeviceCreate() G999 Enet",__LINE__);

    memset (&device_enet_g999_ex, 0, sizeof (device_enet_g999_ex));
	device_enet_g999_ex.statistics_mode = WP_ENABLE;
	//	device_enet_g999_ex.fragmentation_mode = WP_ENABLE;
	device_enet_g999_ex.enet_encap_mode = WP_ENABLE; // Ethernet adaptation
	device_enet_g999_ex.length_mode = WP_ENABLE;
	device_enet_g999_ex.preamble_size = 7;
	device_enet_g999_ex.inter_packet_gap = 12;
	//	device_enet_g999_ex.padding_mode = WP_ENABLE;
	device_enet_g999_ex.max_tx_slow_channels = 0;
//	device_enet_g999_ex.timestamp_type = 0;
	device_enet_g999_ex.rx_timestamp = WP_DISABLE;
	device_enet_g999_ex.tx_timestamp = WP_DISABLE;

    device_enet_g999_cfg.extended_params = &device_enet_g999_ex;
    status = WP_DeviceModify(dev_enet_g999, WP_DEV_MOD_G999_EXTENDED_MODES | WP_DEV_MOD_ENET_RX_STATMODE | WP_DEV_MOD_ENET_TX_STATMODE, &device_enet_g999_cfg);
	App_TerminateOnError(status, "WP_DeviceModify WP_DEV_MOD_G999_EXTENDED_MODES",__LINE__);
   
}


void WPE_G999StreamsCreate(void)
{
    WP_g999_stream  g999_stream_cfg;

    memset(&g999_stream_cfg, 0, sizeof (WP_g999_stream));
    g999_stream_cfg.stream_id = 0;
    memcpy (g999_stream_cfg.enet_encap_header, enet_prefix_header, sizeof (WP_U8) * 12);
    g999_stream_cfg.tx_bit_rate = 1000000000;

	g999_stream = WP_G999StreamCreate(dev_enet_g999, WP_G999_STREAM_ENET, &g999_stream_cfg);
	App_TerminateOnError(g999_stream, "WP_G999StreamCreate", __LINE__);
}


void	WPE_EnableG999Stream(void)
{
	WP_status status;
	status = WP_G999StreamTxEnable(g999_stream);
	App_TerminateOnError(status, "WP_G999StreamEnable g999_stream",__LINE__);
}





// Create IW (Non Ring) Pools 
void    WPE_CreateIWQnode(void)
{
   
   /* Host (Adjunct) pool part of IW QNode */
   WP_pool_buffer_data adjunct_buffer_config =
      {
         /* n_buffers */     2000, //100, //2000  fbp_debugging can change this
         /* offset */        64,
         /* size */          IW_BUFFER_SIZE,
         /* pad */           0,
         /* databustype */   WP_BUS_PARAM,
         /* bank */          APP_BANK_PARAM
      };


   /* IW QNode (Free Buffer Pool part of IW QNode) */
   WP_qnode_iwq qn_iw_config =
      {
         /* interruptqueue */  0, // for IW TX channels only
         /* num_buffers */     200000,
         /* adjunct_pool */    0
      };

   adjunct_buffer_pool = WP_PoolCreate(WP_WINPATH(0), WP_pool_iwbuffer, &adjunct_buffer_config);
   App_TerminateOnError(adjunct_buffer_pool, "WP_PoolCreate()",__LINE__);

   qn_iw_config.adjunct_pool = adjunct_buffer_pool;
   iw_qnode = WP_QNodeCreate(WP_WINPATH(0),WP_QNODE_IWQ | WP_QNODE_OPT_DEDICATED_RX_HWQ | WP_QNODE_OPT_FMU, &qn_iw_config);
   App_TerminateOnError(iw_qnode, "WP_QNodeCreate()",__LINE__);
}



void    WPE_CreateHostQnode(void)
{
   WP_handle pool_host, pool_ring_host;
   
   WP_pool_buffer_data buffers_host_config =
    {
        /* n_buffers */ N_BUFFERS,
        /* offset */ 64,
        /* size */ IW_BUFFER_SIZE,
        /* pad */ 0,
        /* bus */ WP_BUS_HOST,
        /* bank */ APP_BANK_HOST

    };
    
   WP_pool_ring_data ring_host_config =
    {

        /* n_rings */ N_RINGS,
        /* ring_length */ RING_LENGTH,
        /* bus */ WP_BUS_HOST,
        /* bank */ APP_BANK_HOST

    };

   WP_qnode_hostq qn_host_config =
      {
         /* pool_buffer */    0,
         /* pool_ring */      0,
         /* interruptqueue */ WP_IRQT1,
      };

   // Create the buffers
   pool_host = WP_PoolCreate(WP_WINPATH(0), WP_pool_buffer, &buffers_host_config);
   App_TerminateOnError(pool_host, "WP_PoolCreate() pool_host",__LINE__);

   // Create the pointers to buffers - rings
   pool_ring_host = WP_PoolCreate(WP_WINPATH(0), WP_pool_ring, &ring_host_config);
   App_TerminateOnError(pool_ring_host, "WP_PoolCreate() pool_ring_host",__LINE__);

   qn_host_config.pool_buffer = pool_host;
   qn_host_config.pool_ring = pool_ring_host;
   qnode_host = WP_QNodeCreate(0, WP_QNODE_HOSTQ, &qn_host_config);
   App_TerminateOnError(qnode_host, "WP_QNodeCreate() qnode_host",__LINE__);
    
}



void    WPE_CreateMultiCastQnode(void)
{
    WP_qnode_iw_mcq iw_mc_qn_config =
    {
        /* interruptqueue*/ 0,
        /* num_buffers */   10240,
        /* offset */        64,
        /* size */          IW_BUFFER_SIZE,
        /* pad */           0
    };
    qnode_mc = WP_QNodeCreate(WP_WINPATH(0), WP_QNODE_IW_MCQ, &iw_mc_qn_config);
    App_TerminateOnError(qnode_mc, "WP_QNodeCreate MCQ()",__LINE__);
}



void    WPE_CreateBufferPools(void)
{
   WPE_CreateIWQnode();
   WPE_CreateHostQnode();
   WPE_CreateMultiCastQnode();
}

void    WPE_InitHWCards(void)
{
   WP_handle status;
#if USE_RGMII
   status = WPX_BoardConfigure(0, WPX_CONFIGURE_1XGI_6RGMII_6SGMII);
   App_TerminateOnError(status, "WPX_CONFIGURE_1XGI_6RGMII_6SGMII()",__LINE__);
   status = WPX_EnetPhyInit(APP_ENET_PORT1, 
                                        WPU_MODE_RGMII | WPU_MODE_GEI_LOOPBACK_ON | WPU_ANEG_ON);
#else
   status = WPX_BoardConfigure(0, WPX_CONFIGURE_DEFAULT_CHECKIN);
   App_TerminateOnError(status, "WPX_CONFIGURE_2UPI_1XGI_10SGMII()",__LINE__);
#if USE_XGI
           /* XGI */
        status = WPX_BoardXgiConfig(WP_WINPATH(0), APP_ENET_PORT1);
        App_TerminateOnError(status, "WPX_BoardXgiConfig()", __LINE__);

    status = WPX_BoardSerdesInit(WP_WINPATH(0), APP_ENET_PORT1, WPX_SERDES_NORMAL /* WP_FALSE */);
        App_TerminateOnError(status, "WPX_BoardSerdesInit",__LINE__);
#else
 
   status = WPX_BoardSerdesInit(0, APP_ENET_PORT1, WPX_SERDES_NORMAL); 
   App_TerminateOnError(status, "WPX_BoardSerdesInit 7()",__LINE__);
#endif/*USE_XGI*/
#endif/*USE_RGMII*/
   status = WPX_BoardSerdesInit(0, APP_ENET_PORT2, WPX_SERDES_NORMAL);
   App_TerminateOnError(status, "WPX_BoardSerdesInit 8()",__LINE__);
 
}

void WPE_CreateIWBridgeSystem(void)
{

   WP_dfc_sys_info dfc_sys_info =
      {
         /* classification_hash_size */  WP_IW_2K_HASH_ENTRIES,
         /* port_filtering */            WP_IW_PORT_FILTERING_INPUT,
         /* deny_eq_ip */                WP_IW_DENY_EQUAL_IP_DISABLE
      };


   WP_iw_sys_bridging iw_sys_bridge =
      {
         /* tag */                           0,
         /* max_flows; */                    100,
         /* classification_mode */           WP_IW_CLASSIFIER_BASED_BRIDGING,
         /* classifier_config */
         {
            /*classification_parsing_mode*/ WP_IW_DYNAMIC_CLASS,
            /*max_classification_rules;*/   100,
            /*number_parse_fields*/         0,
            /**parse_keys*/                 NULL
         },
         /* learning_mode;*/                 WP_IW_BRIDGE_LEARNING_DISABLE,
         /* learning_queue */
         {
            /*int_queue_num*/               0,
            /*learning_queue_size*/         0,
            /*interrupt_enbale*/            0,
            /*interrupt_rate*/              0
         },
         /*forwarding_table_size*/           WP_IW_32K_HASH_ENTRIES,
         /*member_set_size*/                 WP_IW_32K_HASH_ENTRIES,
         /*buffer_gap;*/                     64,
         /*max_iw_ports*/                    8,
         /*dfc info*/                        &dfc_sys_info,
         /*svl_mode*/                        WP_IW_SVL_DISABLED, 
         /*stag_ether_type*/                 WP_IW_SYS_BRIDGE_STAG_VLAN_ETYPE_DISABLE,
         /*parsing_mode;*/                   0,
         /*dfc_log_mode;*/                   0
      };

   iw_sys = WP_IwSystemCreate(WP_WINPATH(0), WP_IW_VLAN_AWARE_BRIDGING_MODE, &iw_sys_bridge);
   App_TerminateOnError(iw_sys," WP_IwSystemCreate - Bridging",__LINE__);
}



void WPE_SysInit(void)
{
   WP_handle status;
   extern WP_U32 dps_ProgramImage[];
   extern WP_U16 dps_PC_Table_Init[];


/* Interrupt queue configuration used with WP_SysInit */
   WP_int_queue_table int_queue_tables =
      {{/*size, rate */
         {10,   1}, /* WP_IRQT0  highest */
         {10,   1}, /* WP_IRQT1 */
         {10,   1}, /* WP_IRQT2 */
         {10,   1}  /* WP_IRQT3  lowest */
      }};


   WP_iw_global  max_iw =
      {
         /* max_iw_aggregations */          100, 
         /* max_routing_systems;*/          0,
         /* max_directmap_enet_systems */   0,
         /* max_bridging_systems;*/         1,
         /* max_mpls_systems;*/             0,
         /* max_vlan_priority_maps; */      1,
         /* iw_bkgnd_fifo_size */           1000,
         /* cong_pt       */                NULL,
         /* iw_host_limits */               
         {
            /* max_ht_rx_channels */        5,   
            /* pq_block_size */             0,
            /* n_pq_blocks */               0,
            /* max_ht_tx_channels */        1,
            /* max_tx_groups */             0
         },
         /* mpls_config*/                   {0,0},
         /* iw_modes*/ 
         {  /* policer_mode*/                 WP_IW_FLOW_POLICER,
            /* statistics_bundle_stat_mode*/  WP_IW_STATISTICS_BUNDLE_STAT_ENABLE,
            /* l2_prefix_ext_mode;*/          WP_IW_L2_HEADER_EXTENSION_DISABLE,
            /* enhanced_flow_stat_mode;*/     WP_IW_ENHANCED_FLOW_STAT_ENABLE,
            /* flow_stat_mode;*/              WP_IW_FLOW_STAT_ENABLE,
            /* fr_tx_flow_stat_mode*/         WP_IW_FR_TX_FLOW_STAT_DISABLE,
            /* mfc_alloc_mode; */             WP_IW_MFC_ALLOC_ENABLE,
            /* learning_queue_mode */         WP_IW_LEARNING_ENABLE,
            /* port_filtering_mode*/          WP_IW_PORT_FILTERING_ENABLE,
            /* lpm_alloc_mode */              WP_IW_LPM_ALLOC_DISABLE,
            /* enhanced_mc_member_stat_mode*/ WP_ENABLE,
            /* routing_alloc_modes */         0,
            /* res_mc_addresses_table_mode */ 0,
            /* port_stag_etype_mode*/         0,
            /* iw_characteristics*/           0
         },    
         /* max_iw_stat_bundles*/           0,
         /* max_fr_sw_systems*/             0,
         /* max_iw_ports */                 10,
         /* max_iw_mc_groups */             2, 
         /* max_nat_systems */              0, 
         /* max_iw_second_aggregations */   0, 
         /* iw_comp_limits */               NULL, 
         /* max_iw_mc_members */            20, 
         /* max_iw_programmable_filters */  32, 
         /* max_iw_policers */              20,
         /* max_dscpmt; */                  0,
         /* max_wred_bindings; */           0,
         /* tcm_policer_limits; */          {0,0},
         /* max_flow_rule_stat_tables;*/    16,
         /* max_indirect_action_tables;*/   0,
         /* lpm_dfc_limits;*/               {0,0},
         /* brigding_time_stamp_info;*/     {0,0},
         /* iwgp_bkgnd_fifo_size;*/         0,
         /* lpm_max_ipv4_routing_flows;*/   0,
         /* lpm_max_ipv6_routing_flows;*/   0,
         /* max_iw_gpmt_tables;*/           0
      };

// To Do: put NONE everywhere for non crucial masters
   WP_context context =
      {
         10,
         /* the first four assignments are mandatory bus assignments */
         {
            {/* BUS_PARAM                */ WP_BUS_PARAM,    APP_BANK_PARAM},
            {/* BUS_PACKET               */ WP_BUS_PACKET,   APP_BANK_PACKET},
            {/* BUS_INTERNAL             */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
            {/* BUS_HOST                 */ WP_BUS_HOST,     APP_BANK_HOST},
            {/* INTQUEUE_DEBUG           */ WP_BUS_HOST,     APP_BANK_HOST},
            {/* TIMESTAMP                */ WP_BUS_HOST,     APP_BANK_HOST},
            {/* CAS                      */ WP_BUS_NONE,     0},
            {/* TRANS_TSAT               */ WP_BUS_NONE,     0},
            {/* TPT                      */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
            {/* CW                       */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
            {/* ASU_TXQUEUE              */ WP_BUS_PARAM,    APP_BANK_PARAM},
            {/* STATISTICS               */ WP_BUS_HOST,     APP_BANK_HOST},
            {/* ATM_ADDLOOKUP            */ WP_BUS_PARAM,    APP_BANK_PARAM},
            {/* ATM_CPT_EBMT_EXTHEADER   */ WP_BUS_PARAM,    APP_BANK_PARAM},
            {/* ETH HDLC CPT             */ WP_BUS_PARAM,    APP_BANK_PARAM},
            {/* FBP BD                   */ WP_BUS_PARAM,    APP_BANK_PARAM},
            {/* FBP DATA                 */ WP_BUS_PARAM,   APP_BANK_PARAM},
            {/* L2_BKGND                 */ WP_BUS_PARAM,   APP_BANK_PARAM},
            {/* BRIDGE_LEARNING_DFC_PFMT */ WP_BUS_PARAM,    APP_BANK_PARAM},
            {/* IMA_MLPPP_STATE          */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
            {/* IMA_MLPPP_BUFFER         */ WP_BUS_PARAM,    APP_BANK_PARAM},
            {/* HASH_IW_LPM_EMC          */ WP_BUS_PARAM,   APP_BANK_PARAM},
            {/* IW_MFC_RULES             */ WP_BUS_PARAM,    APP_BANK_PARAM},
            {/* FIWT_QAT                 */ WP_BUS_PARAM,    APP_BANK_PARAM},
            {/* MFCPT_DIFFSERV_UPPT_DFCT */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
            {/* STATISTICS2              */ WP_BUS_PARAM,   APP_BANK_PARAM},
            {/* STATISTICS3              */ WP_BUS_PARAM,   APP_BANK_PARAM},
            {/* UNASSIGNED6              */ WP_BUS_NONE,     0},
            {/* PKT_GLOBAL_TCPT          */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
            {/* AAL2_QSR                 */ WP_BUS_NONE,     0},
            {/* UNASSIGNED9              */ WP_BUS_NONE,     0},
            {/* UNASSIGNED10             */ WP_BUS_NONE,     0},
            {/* MCH_DATA                 */ WP_BUS_PARAM,   APP_BANK_PARAM},
            {/* OAM_FM_QUERY             */ WP_BUS_NONE,     0},
            {/* HASH_ON_HEAP             */ WP_BUS_NONE,     0},
            {/* MFC_INTERNAL             */ WP_BUS_PARAM,   APP_BANK_PARAM},
            {/* MFR_RAS_TBL              */ WP_BUS_NONE,     0},
            {/* CESOP_DATA               */ WP_BUS_NONE,     0},
            {/* SECURITY_ESE,            */ WP_BUS_NONE,     0},
            {/* PDCP_SECURITY_ENTITY,    */ WP_BUS_NONE,     0},
            {/* POLICER,                 */ WP_BUS_PARAM, APP_BANK_PARAM},
            {/* PCE_LRN,                 */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
            {/* PCEA_PARAM,              */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
            {/* CONGESTION_TABLE,        */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
            {/* PARAMS_PROPAGATION,      */ WP_BUS_NONE,     0},  
            {/* STATS_IWPORT_PCE_FILTERS,*/ WP_BUS_PARAM, APP_BANK_PARAM},  
            {/* TIMESTAMP_SERVICES_TABLE,*/ WP_BUS_NONE,     0},  
            {/* POLICER_ACTION,          */ WP_BUS_PARAM, APP_BANK_PARAM},
            {/* PECS_GLOBAL,             */ WP_BUS_PARAM, APP_BANK_PARAM},
            {/* ENET_10G_LONG_PKT,       */ WP_BUS_NONE,     0},
            {/* G999_STREAM,             */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
            {/* G999_STREAM_STATISTICS,  */ WP_BUS_INTERNAL, APP_BANK_INTERNAL},
            {/* FMU_FIFO_TO_CHANNEL_NUMBER*/WP_BUS_INTERNAL, APP_BANK_INTERNAL},
            {/* IPWORT_EXT,              */ WP_BUS_PARAM,   APP_BANK_PARAM},
            {/* IW_GPMT_TABLE,           */ WP_BUS_PARAM, APP_BANK_PARAM},
            {/* EMPHY_PORT_LOOKUP,       */ WP_BUS_NONE,     0},

         },
         {
            dps_ProgramImage, dps_ProgramImage
         },
         {
            dps_PC_Table_Init, dps_PC_Table_Init
         },
         &int_queue_tables,    /* Interrupt queue tables */
         10,              /* Maxumum number of pools */
         NULL,                 /* ATM global parameters */
         &max_iw,              /* Interworking global parameters */
         0,                    /* dps_winfarm_mask */
      };

   status = WP_SysInit(WP_WINPATH(0), &context);
   App_TerminateOnError(status, "WP_SysInit",__LINE__);
}

WP_CHAR appname[] = "ASB PM COUNTER APP";

void App_TerminateOnError(WP_handle handle, WP_CHAR *s, WP_U32 line)
{
    if (WP_ERROR_P(handle)) {
            printf("%s: Terminal error (handle 0x%x) %s\n", appname, handle, s);
            printf("%s: Error type - %s\n", appname, WP_ErrorString(handle));
            printf("line = %d\n",line);
            printf("Test Failed\n");
            WP_DriverRelease();
            exit(0);
    }
    else
    {
        printf("%s: Handle/Status returned from %s is 0x%x ;line = %d\n", appname, s, handle,line);
    }
}

typedef union StatField
{
      WP_U64 field;
      struct {
            WP_U32 high;
            WP_U32 low;
      } part;
} WPE_StatField;


void WPE_DisplayGenericFlowAggStats(WP_handle agg)
{

	WP_iw_generic_flow_stats iw_flow_stats;

   memset(&iw_flow_stats,0,sizeof(WP_iw_generic_flow_stats));

   WP_IwFlowStatistics(agg, WP_IW_FLOW_STAT_GENERIC, &iw_flow_stats);

   printf ("\nGeneric aggregation flow statistics of flow (%d) (%x)\n", (agg & 0x00000fff),agg);
   printf ("==========================================================\n");
#if _WP_MODULE_PM_COUNTER_ENABLE_
   printf ("Dropped Packet:          %08x%08x\n",
           ((WPE_StatField)iw_flow_stats.dropped_packet).part.high,
           ((WPE_StatField)iw_flow_stats.dropped_packet).part.low);

   printf ("Dropped bytes:          %08x%08x\n",
           ((WPE_StatField)iw_flow_stats.dropped_bytes).part.high,
           ((WPE_StatField)iw_flow_stats.dropped_bytes).part.low);
#endif
   printf ("Forward Packet:          %08x%08x\n",
           ((WPE_StatField)iw_flow_stats.forward_packet).part.high,
           ((WPE_StatField)iw_flow_stats.forward_packet).part.low);
   printf( "MTU Drop Packets:        %08x%08x\n",
           ((WPE_StatField)iw_flow_stats.mtu_drop_packets).part.high,
           ((WPE_StatField)iw_flow_stats.mtu_drop_packets).part.low);
   printf( "TX Queue Drop Packets:   %08x%08x\n",
           ((WPE_StatField)iw_flow_stats.tx_queue_drop_packets).part.high,
           ((WPE_StatField)iw_flow_stats.tx_queue_drop_packets).part.low);
   printf( "Forwarded Bytes:         %08x%08x\n",
           ((WPE_StatField)iw_flow_stats.forwarded_bytes).part.high,
           ((WPE_StatField)iw_flow_stats.forwarded_bytes).part.low);
   printf( "Denied Packets:          %08x%08x\n",
           ((WPE_StatField)iw_flow_stats.denied_packets).part.high,
           ((WPE_StatField)iw_flow_stats.denied_packets).part.low);
   printf( "Bad Policer Packets:     %08x%08x\n",
           ((WPE_StatField)iw_flow_stats.policer_non_conforming_packets).part.high,
           ((WPE_StatField)iw_flow_stats.policer_non_conforming_packets).part.low);
   printf( "pecs_counter_1:          %08x%08x\n",
		   ((WPE_StatField)iw_flow_stats.pecs_counter_1).part.high,
		   ((WPE_StatField)iw_flow_stats.pecs_counter_1).part.low);
   printf( "pecs_counter_2:          %08x%08x\n",
		   ((WPE_StatField)iw_flow_stats.pecs_counter_2).part.high,
		   ((WPE_StatField)iw_flow_stats.pecs_counter_2).part.low);
   printf( "pecs_counter_3:          %08x%08x\n",
		   ((WPE_StatField)iw_flow_stats.pecs_counter_3).part.high,
		   ((WPE_StatField)iw_flow_stats.pecs_counter_3).part.low);
   printf( "pecs_counter_4:          %08x%08x\n",
		   ((WPE_StatField)iw_flow_stats.pecs_counter_4).part.high,
		   ((WPE_StatField)iw_flow_stats.pecs_counter_4).part.low);
   printf( "pecs_counter_5:          %08x%08x\n",
		   ((WPE_StatField)iw_flow_stats.pecs_counter_5).part.high,
		   ((WPE_StatField)iw_flow_stats.pecs_counter_5).part.low);
   printf( "pecs_counter_6:          %08x%08x\n",
		   ((WPE_StatField)iw_flow_stats.pecs_counter_6).part.high,
		   ((WPE_StatField)iw_flow_stats.pecs_counter_6).part.low);

   printf("\n");
}

void WPE_DisplayFlowAggStats(WP_handle agg)
{

   WP_iw_flow_stats iw_flow_stats;

   memset(&iw_flow_stats,0,sizeof(iw_flow_stats));

   WP_IwFlowStatistics(agg, WP_IW_FLOW_STAT, &iw_flow_stats);

   printf ("\nAggregation flow statistics of flow (%d) (%x)\n", (agg & 0x00000fff),agg);
   printf ("==========================================================\n");

   printf ("Dropped Packet:          %08x%08x\n",
           ((WPE_StatField)iw_flow_stats.dropped_packet).part.high,
           ((WPE_StatField)iw_flow_stats.dropped_packet).part.low);
   printf ("Dropped Bytes:          %08x%08x\n",
           ((WPE_StatField)iw_flow_stats.dropped_bytes).part.high,
           ((WPE_StatField)iw_flow_stats.dropped_bytes).part.low);
   printf ("Forward Packet:          %08x%08x\n",
           ((WPE_StatField)iw_flow_stats.forward_packet).part.high,
           ((WPE_StatField)iw_flow_stats.forward_packet).part.low);
   printf( "FBP Drop Packets:        %08x%08x\n",
           ((WPE_StatField)iw_flow_stats.fbp_drop_packets).part.high,
           ((WPE_StatField)iw_flow_stats.fbp_drop_packets).part.low);
   printf( "MTU Drop Packets:        %08x%08x\n",
           ((WPE_StatField)iw_flow_stats.mtu_drop_packets).part.high,
           ((WPE_StatField)iw_flow_stats.mtu_drop_packets).part.low);
   printf( "TTL Drop Packets:        %08x%08x\n",
           ((WPE_StatField)iw_flow_stats.ttl_drop_packets).part.high,
           ((WPE_StatField)iw_flow_stats.ttl_drop_packets).part.low);
   printf( "TX Queue Drop Packets:   %08x%08x\n",
           ((WPE_StatField)iw_flow_stats.tx_queue_drop_packets).part.high,
           ((WPE_StatField)iw_flow_stats.tx_queue_drop_packets).part.low);
   printf( "MPLS Drop:               %08x%08x\n",
           ((WPE_StatField)iw_flow_stats.mpls_drop).part.high,
           ((WPE_StatField)iw_flow_stats.mpls_drop).part.low);
   printf( "Denied Packets:          %08x%08x\n",
           ((WPE_StatField)iw_flow_stats.denied_packets).part.high,
           ((WPE_StatField)iw_flow_stats.denied_packets).part.low);
   printf( "Group Filtered Packets:  %08x%08x\n",
           ((WPE_StatField)iw_flow_stats.group_filtered_packets).part.high,
           ((WPE_StatField)iw_flow_stats.group_filtered_packets).part.low);
   printf( "Forwarded Bytes:         %08x%08x\n",
           ((WPE_StatField)iw_flow_stats.forwarded_bytes).part.high,
           ((WPE_StatField)iw_flow_stats.forwarded_bytes).part.low);
   printf( "Gtp Bad Headers:         %08x%08x\n",
           ((WPE_StatField)iw_flow_stats.gtp_bad_headers).part.high,
           ((WPE_StatField)iw_flow_stats.gtp_bad_headers).part.low);
   printf( "Bad Policer Packets:     %08x%08x\n",
           ((WPE_StatField)iw_flow_stats.policer_non_conforming_packets).part.high,
           ((WPE_StatField)iw_flow_stats.policer_non_conforming_packets).part.low);

   printf("\n");
}


void WPE_FlowStatDisplay(WP_handle statistic_handle )
{
   WP_status status;
   WP_iw_flow_rule_stats iw_flow_stats;

   memset( &iw_flow_stats, 0, sizeof(WP_iw_flow_rule_stats));

   printf("Statistics For Handle Index 0x%8x\n\n", statistic_handle);

   status = WP_IwFlowStatistics(statistic_handle,
                                WP_IW_FLOW_CLASS_RULE_STAT,
                                &iw_flow_stats);
   App_TerminateOnError(status, "WP_IwFlowStatistics",__LINE__);
}
void WPE_ShowIwPortStatistics(WP_handle iw_port_handle)
{
   WP_bport_stats bport_stats;
   WP_handle status;

   printf ("\nIW Bport Statistics of Port 0x%x \n",iw_port_handle);
   printf ("==========================================================\n");

   memset(&bport_stats, 0, sizeof(WP_bport_stats));

   status = WP_IwPortStatistics(iw_port_handle,&bport_stats);
   App_TerminateOnError(status, "WP_IwPortStatistics()",__LINE__);

   printf("\n");

   printf ("rx_valid_packets :                %08x%08x\n",
           ((WPE_StatField)bport_stats.rx_valid_packets).part.high,
           ((WPE_StatField)bport_stats.rx_valid_packets).part.low);
   printf ("discard_vlan_acceptable_filter :  %08x%08x\n",
           ((WPE_StatField)bport_stats.discard_vlan_acceptable_filter).part.high,
           ((WPE_StatField)bport_stats.discard_vlan_acceptable_filter).part.low);
   printf ("discard_ingress_filter :          %08x%08x\n",
           ((WPE_StatField)bport_stats.discard_ingress_filter).part.high,
           ((WPE_StatField)bport_stats.discard_ingress_filter).part.low);
   printf ("discard_bridge_classifier :       %08x%08x\n",
           ((WPE_StatField)bport_stats.discard_bridge_classifier).part.high,
           ((WPE_StatField)bport_stats.discard_bridge_classifier).part.low);
   printf ("discard_unk_macsa :               %08x%08x\n",
           ((WPE_StatField)bport_stats.discard_unk_macsa).part.high,
           ((WPE_StatField)bport_stats.discard_unk_macsa).part.low);
   printf ("deny_mac_sa :                     %08x%08x\n",
           ((WPE_StatField)bport_stats.deny_mac_sa).part.high,
           ((WPE_StatField)bport_stats.deny_mac_sa).part.low);
   printf ("deny_mac_da :                     %08x%08x\n",
           ((WPE_StatField)bport_stats.deny_mac_da).part.high,
           ((WPE_StatField)bport_stats.deny_mac_da).part.low);
   printf ("rx_bc_valid_packets :             %08x%08x\n",
           ((WPE_StatField)bport_stats.rx_bc_valid_packets).part.high,
           ((WPE_StatField)bport_stats.rx_bc_valid_packets).part.low);
   printf ("rx_mc_valid_packets :             %08x%08x\n",
           ((WPE_StatField)bport_stats.rx_mc_valid_packets).part.high,
           ((WPE_StatField)bport_stats.rx_mc_valid_packets).part.low);
   printf ("forwarded_uc_packets :            %08x%08x\n",
           ((WPE_StatField)bport_stats.forwarded_uc_packets).part.high,
           ((WPE_StatField)bport_stats.forwarded_uc_packets).part.low);
   printf ("forwarded_bc_packets :            %08x%08x\n",
           ((WPE_StatField)bport_stats.forwarded_bc_packets).part.high,
           ((WPE_StatField)bport_stats.forwarded_bc_packets).part.low);
   printf ("forwarded_mc_packets :            %08x%08x\n",
           ((WPE_StatField)bport_stats.forwarded_mc_packets).part.high,
           ((WPE_StatField)bport_stats.forwarded_mc_packets).part.low);
   printf ("discard_dfc_filter[0] :           %08x%08x\n",
           ((WPE_StatField)bport_stats.discard_dfc_filter[0]).part.high,
           ((WPE_StatField)bport_stats.discard_dfc_filter[0]).part.low);
   printf ("discard_dfc_filter[1] :           %08x%08x\n",
           ((WPE_StatField)bport_stats.discard_dfc_filter[1]).part.high,
           ((WPE_StatField)bport_stats.discard_dfc_filter[1]).part.low);
   printf ("protocol_error :                  %08x%08x\n",
           ((WPE_StatField)bport_stats.protocol_error).part.high,
           ((WPE_StatField)bport_stats.protocol_error).part.low);

   printf("\n");
   return;

}
#define PRINT_STAT_FIELD(field) printf( "%08x%08x", (WP_U32)(field>>32), (WP_U32)field)
WP_U32 WPE_DisplayMcMemberStats(WP_handle group,WP_handle member)
{
   WP_status status;

   WP_iw_mc_member_enhanced_stats member_stats;

   memset(&member_stats,0,sizeof(WP_iw_mc_member_enhanced_stats));
   status = WP_IwMcMemberStatistics(group,member,&member_stats);
   App_TerminateOnError(status,"WP_IwMcMemberStatistics", __LINE__);

   printf ("\nMc member statistics on group directing to \n");
   printf ("==========================================================\n");


      printf ("dropped packets:                ");
      PRINT_STAT_FIELD(member_stats.dropped_packets);
      printf("\n");
      printf ("dropped bytes:                ");
      PRINT_STAT_FIELD(member_stats.dropped_bytes);
      printf("\n");
      printf ("buffers_counter:                ");
      PRINT_STAT_FIELD(member_stats.buffers_counter);
      printf("\n");
      printf ("threshold_dropped_frames:       ");
      PRINT_STAT_FIELD(member_stats.threshold_dropped_frames);
      printf("\n");
      printf ("forwarded_frames:               ");
      PRINT_STAT_FIELD(member_stats.forwarded_frames);
      printf("\n");
      printf ("fbp_drop_packets:               ");
      PRINT_STAT_FIELD(member_stats.fbp_drop_packets);
      printf("\n");
      printf ("mtu_drop_packets:               ");
      PRINT_STAT_FIELD(member_stats.mtu_drop_packets);
      printf("\n");
      printf ("ttl_drop_packets:               ");
      PRINT_STAT_FIELD(member_stats.ttl_drop_packets);
      printf("\n");
      printf ("tx_queue_drop_packets:          ");
      PRINT_STAT_FIELD(member_stats.tx_queue_drop_packets);
      printf("\n");
      printf ("mpls_drop:                      ");
      PRINT_STAT_FIELD(member_stats.mpls_drop);
      printf("\n");
      printf ("denied_packets:                 ");
      PRINT_STAT_FIELD(member_stats.denied_packets);
      printf("\n");
      printf ("group_filtered_packets:         ");
      PRINT_STAT_FIELD(member_stats.group_filtered_packets);
      printf("\n");
      printf ("forwarded_bytes:                ");
      PRINT_STAT_FIELD(member_stats.forwarded_bytes);
      printf("\n");
      printf ("gtp_bad_headers:                ");
      PRINT_STAT_FIELD(member_stats.gtp_bad_headers);
      printf("\n");
      printf ("policer_non_conforming_packets: ");
      PRINT_STAT_FIELD(member_stats.policer_non_conforming_packets);
      printf("\n");
      printf ("fbp_underrun:                   ");
      PRINT_STAT_FIELD(member_stats.fbp_underrun);
      printf("\n");
      printf ("src_ip_filter_dropped_frames:   ");
      PRINT_STAT_FIELD(member_stats.src_ip_filter_dropped_frames);
      printf("\n");
   printf("\n");

   return member_stats.buffers_counter;
}


#define WTE_PRINT_STAT_HIGH_LOW(printf_string, field) \
   printf (printf_string, ((WPE_StatField)field).part.high, ((WPE_StatField)field).part.low)

#define WTE_PRINT_STAT_IF_NOT_ZERO(printf_string, field) \
   do                                                    \
   {                                                     \
      if (field != 0)                                    \
         WTE_PRINT_STAT_HIGH_LOW(printf_string, field);  \
   }while(0)

void WPE_App_print_stats_enet (WP_stats_enet *var_ptr)
{
   printf ("  WP_stats_enet\n");
   /*WPE_App_print_U64 ("txrx_frames_64", &var_ptr->txrx_frames_64);
     WPE_App_print_U64 ("txrx_frames_127", &var_ptr->txrx_frames_127);
     WPE_App_print_U64 ("txrx_frames_255", &var_ptr->txrx_frames_255);
     WPE_App_print_U64 ("txrx_frames_511", &var_ptr->txrx_frames_511);
     WPE_App_print_U64 ("txrx_frames_1023", &var_ptr->txrx_frames_1023);
     WPE_App_print_U64 ("txrx_frames_1518", &var_ptr->txrx_frames_1518);
     WPE_App_print_U64 ("txrx_frames_1522", &var_ptr->txrx_frames_1522);*/
   WTE_PRINT_STAT_IF_NOT_ZERO("rx_bytes:                      %08x%08x\n",
                              var_ptr->rx_bytes);
   WTE_PRINT_STAT_IF_NOT_ZERO("rx_packets:                    %08x%08x\n",
                              var_ptr->rx_packets);
   WTE_PRINT_STAT_IF_NOT_ZERO("rx_multicast:                  %08x%08x\n",
                              var_ptr->rx_multicast);
   WTE_PRINT_STAT_IF_NOT_ZERO("rx_broadcast:                  %08x%08x\n",
                              var_ptr->rx_broadcast);
   WTE_PRINT_STAT_IF_NOT_ZERO("rx_err_fcs:                    %08x%08x\n",
                              var_ptr->rx_err_fcs);
   WTE_PRINT_STAT_IF_NOT_ZERO("rx_mac_control:                %08x%08x\n",
                              var_ptr->rx_mac_control);
   WTE_PRINT_STAT_IF_NOT_ZERO("rx_mac_pause:                  %08x%08x\n",
                              var_ptr->rx_mac_pause);
   WTE_PRINT_STAT_IF_NOT_ZERO("rx_mac_unknown:                %08x%08x\n",
                              var_ptr->rx_mac_unknown);
   WTE_PRINT_STAT_IF_NOT_ZERO("rx_err_alignment:              %08x%08x\n",
                              var_ptr->rx_err_alignment);
   WTE_PRINT_STAT_IF_NOT_ZERO("rx_err_length:                 %08x%08x\n",
                              var_ptr->rx_err_length);
   WTE_PRINT_STAT_IF_NOT_ZERO("rx_err_code:                   %08x%08x\n",
                              var_ptr->rx_err_code);
   WTE_PRINT_STAT_IF_NOT_ZERO("rx_false_carrier:              %08x%08x\n",
                              var_ptr->rx_false_carrier);
   WTE_PRINT_STAT_IF_NOT_ZERO("rx_undersize:                  %08x%08x\n",
                              var_ptr->rx_undersize);
   WTE_PRINT_STAT_IF_NOT_ZERO("rx_oversize:                   %08x%08x\n",
                              var_ptr->rx_oversize);
   WTE_PRINT_STAT_IF_NOT_ZERO("rx_fragments:                  %08x%08x\n",
                              var_ptr->rx_fragments);
   WTE_PRINT_STAT_IF_NOT_ZERO("rx_jabber:                     %08x%08x\n",
                              var_ptr->rx_jabber);
   WTE_PRINT_STAT_IF_NOT_ZERO("rx_dropped:                    %08x%08x\n",
                              var_ptr->rx_dropped);
   WTE_PRINT_STAT_IF_NOT_ZERO("tx_bytes:                      %08x%08x\n",
                              var_ptr->tx_bytes);
   WTE_PRINT_STAT_IF_NOT_ZERO("tx_packets:                    %08x%08x\n",
                              var_ptr->tx_packets);
   WTE_PRINT_STAT_IF_NOT_ZERO("tx_multicast:                  %08x%08x\n",
                              var_ptr->tx_multicast);
   WTE_PRINT_STAT_IF_NOT_ZERO("tx_broadcast:                  %08x%08x\n",
                              var_ptr->tx_broadcast);
   WTE_PRINT_STAT_IF_NOT_ZERO("tx_mac_pause:                  %08x%08x\n",
                              var_ptr->tx_mac_pause);
   WTE_PRINT_STAT_IF_NOT_ZERO("rx_host_frames:                %08x%08x\n",
                              var_ptr->rx_host_frames);
   WTE_PRINT_STAT_IF_NOT_ZERO("rx_iw_frames:                  %08x%08x\n",
                              var_ptr->rx_iw_frames);
   WTE_PRINT_STAT_IF_NOT_ZERO("rx_err_host_full:              %08x%08x\n",
                              var_ptr->rx_err_host_full);
   WTE_PRINT_STAT_IF_NOT_ZERO("rx_err_fbp_underrun:           %08x%08x\n",
                              var_ptr->rx_err_fbp_underrun);
   WTE_PRINT_STAT_IF_NOT_ZERO("rx_err_nonvalid_mac:           %08x%08x\n",
                              var_ptr->rx_err_nonvalid_mac);
   WTE_PRINT_STAT_IF_NOT_ZERO("rx_err_mru:                    %08x%08x\n",
                              var_ptr->rx_err_mru);
   WTE_PRINT_STAT_IF_NOT_ZERO("rx_err_sdu:                    %08x%08x\n",
                              var_ptr->rx_err_sdu);
   WTE_PRINT_STAT_IF_NOT_ZERO("rx_err_overrun:                %08x%08x\n",
                              var_ptr->rx_err_overrun);
//   WTE_PRINT_STAT_IF_NOT_ZERO ("tx_frames", var_ptr->tx_frames);
   WTE_PRINT_STAT_IF_NOT_ZERO("tx_defer:                      %08x%08x\n",
                              var_ptr->tx_defer);
   WTE_PRINT_STAT_IF_NOT_ZERO("tx_excess_defer:               %08x%08x\n",
                              var_ptr->tx_excess_defer);
   WTE_PRINT_STAT_IF_NOT_ZERO("tx_single_collision:           %08x%08x\n",
                              var_ptr->tx_single_collision);
   WTE_PRINT_STAT_IF_NOT_ZERO("tx_multi_collision:            %08x%08x\n",
                              var_ptr->tx_multi_collision);
   WTE_PRINT_STAT_IF_NOT_ZERO("tx_late_collision:             %08x%08x\n",
                              var_ptr->tx_late_collision);
   WTE_PRINT_STAT_IF_NOT_ZERO("tx_excess_collision:           %08x%08x\n",
                              var_ptr->tx_excess_collision);
   WTE_PRINT_STAT_IF_NOT_ZERO("tx_no_collision:               %08x%08x\n",
                              var_ptr->tx_no_collision);
   WTE_PRINT_STAT_IF_NOT_ZERO("tx_mac_pause_honored:          %08x%08x\n",
                              var_ptr->tx_mac_pause_honored);
   WTE_PRINT_STAT_IF_NOT_ZERO("tx_dropped:                    %08x%08x\n",
                              var_ptr->tx_dropped);
   WTE_PRINT_STAT_IF_NOT_ZERO("tx_jabber:                     %08x%08x\n",
                              var_ptr->tx_jabber);
   WTE_PRINT_STAT_IF_NOT_ZERO("tx_err_fcs:                    %08x%08x\n",
                              var_ptr->tx_err_fcs);
   WTE_PRINT_STAT_IF_NOT_ZERO("tx_control:                    %08x%08x\n",
                              var_ptr->tx_control);
   WTE_PRINT_STAT_IF_NOT_ZERO("tx_oversize:                   %08x%08x\n",
                              var_ptr->tx_oversize);
   WTE_PRINT_STAT_IF_NOT_ZERO("tx_undersize:                  %08x%08x\n",
                              var_ptr->tx_undersize);
   WTE_PRINT_STAT_IF_NOT_ZERO("tx_fragments:                  %08x%08x\n",
                              var_ptr->tx_fragments);
   WTE_PRINT_STAT_IF_NOT_ZERO("tx_err_underrun:               %08x%08x\n",
                              var_ptr->tx_err_underrun);
   WTE_PRINT_STAT_IF_NOT_ZERO("DPS rx_host_frames:            %08x%08x\n",
                              var_ptr->rx_host_frames);
   WTE_PRINT_STAT_IF_NOT_ZERO("DPS rx_iw_frames:              %08x%08x\n",
                              var_ptr->rx_iw_frames);
   WTE_PRINT_STAT_IF_NOT_ZERO("DPS rx_err_host_full:          %08x%08x\n",
                              var_ptr->rx_err_host_full);
   WTE_PRINT_STAT_IF_NOT_ZERO("DPS rx_err_fbp_underrun:       %08x%08x\n",
                              var_ptr->rx_err_fbp_underrun);
   WTE_PRINT_STAT_IF_NOT_ZERO("DPS rx_err_nonvalid_mac:       %08x%08x\n",
                              var_ptr->rx_err_nonvalid_mac);
   WTE_PRINT_STAT_IF_NOT_ZERO("DPS rx_err_mru:                %08x%08x\n",
                              var_ptr->rx_err_mru);
   WTE_PRINT_STAT_IF_NOT_ZERO("DPS rx_err_sdu:                %08x%08x\n",
                              var_ptr->rx_err_sdu);
   WTE_PRINT_STAT_IF_NOT_ZERO("DPS tx_err_underrun:           %08x%08x\n",
                              var_ptr->tx_err_underrun);
   WTE_PRINT_STAT_IF_NOT_ZERO("DPS rx_err_overrun:            %08x%08x\n",
                              var_ptr->rx_err_overrun);
   WTE_PRINT_STAT_IF_NOT_ZERO("DPS tx_frames:                 %08x%08x\n",
                              var_ptr->tx_frames);
   WTE_PRINT_STAT_IF_NOT_ZERO("DPS g999_rx_pause_frames:      %08x%08x\n",
                              var_ptr->g999_rx_pause_frames);
   WTE_PRINT_STAT_IF_NOT_ZERO("DPS g999_rx_errored_fragments: %08x%08x\n",
                              var_ptr->g999_rx_errored_fragments);

   WTE_PRINT_STAT_IF_NOT_ZERO("DPS_IF tx_frames:              %08x%08x\n",
                              var_ptr->serial_dps_if_stats.tx_frames);
   WTE_PRINT_STAT_IF_NOT_ZERO("DPS_IF tx_bytes:               %08x%08x\n",
                              var_ptr->serial_dps_if_stats.tx_bytes);
   WTE_PRINT_STAT_IF_NOT_ZERO("DPS_IF rx_frames:              %08x%08x\n",
                              var_ptr->serial_dps_if_stats.rx_frames);
   WTE_PRINT_STAT_IF_NOT_ZERO("DPS_IF rx_bytes:               %08x%08x\n",
                              var_ptr->serial_dps_if_stats.rx_bytes);
   WTE_PRINT_STAT_IF_NOT_ZERO("DPS_IF rx_dropped_frames:      %08x%08x\n",
                              var_ptr->serial_dps_if_stats.rx_dropped_frames);
   WTE_PRINT_STAT_IF_NOT_ZERO("DPS_IF rx_pce_denied_frames:   %08x%08x\n",
                              var_ptr->serial_dps_if_stats.rx_pce_denied_frames);

   return;
}

void WPE_ShowDeviceStatistics(WP_handle dev_enet)
{
   WP_stats_enet stats_enet;
   WP_status     status;

   memset(&stats_enet,0,sizeof(WP_stats_enet));

   if ( (status = WP_DeviceStatistics(dev_enet, &stats_enet)) ) {
      printf("GetStatistics error Enet : 0x%x\n",dev_enet);
      return;
   }
   WPE_App_print_stats_enet(&stats_enet);
   return;
}


void WPE_DisplayPCEStats(WP_handle filter_handle)
{
   WP_U32 temp1, temp2;
   WP_status status;
   WP_pce_filter_stat pce_stats;

   printf (" PCE statistics of filter (%d) \n", filter_handle);
   printf ("============================================\n");

   memset(&pce_stats, 0, sizeof(WP_pce_filter_stat) );

   if((status = WP_PceFilterStatistics(filter_handle, &pce_stats)) )
   {
      printf("PCE GetStatistics error\n");
      return;
   }
   temp1 = pce_stats.matched_pkts;
   temp2 = pce_stats.matched_pkts >> 32;
   printf("matched_pkts:           %.8x%.8x\n", temp2,temp1);

   temp1 = pce_stats.no_matched_pkts;
   temp2 = pce_stats.no_matched_pkts >> 32;
   printf("no_matched_pkts:        %.8x%.8x\n", temp2,temp1);

   temp1 = pce_stats.denied_pkts;
   temp2 = pce_stats.denied_pkts >> 32;
   printf("denied_pkts:            %.8x%.8x\n", temp2,temp1);
}

void WPE_CreateG999PceClassification(void)
{
    WP_pce_filter_classification filter_class = {0};
    WP_handle pce_filter_handle;
    WP_pce_rule_classification rule_cfg = {0};
    WP_pce_filter_set filter_set = {0};
    WP_pce_if_params_device		  pce_if_params_device_cfg = {0};
    WP_handle pce_rule_handle;
    WP_handle pce_filter_set_handle;
    WP_handle pce_if_handle;
    WP_status status;
    WP_device_enet_g999			   	dev_enet_g999_cfg = {0};

	memset(&filter_class, 0, sizeof (WP_pce_filter_classification));


	/* Common parameters for all filters*/
	filter_class.tag = 1;
	filter_class.no_match_action = WP_PCE_FILTER_NO_MATCH_DENY;
	filter_class.no_fields_action = WP_PCE_FILTER_NO_FIELDS_DENY;
	filter_class.no_match_result[0].result_type = WP_PCE_RESULT_LAST;


	filter_class.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
	filter_class.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;
	filter_class.filter_fields[0].field_id = WP_PCE_FIELD_ID_DEVICE;

	filter_class.filter_fields[1].field_id = WP_PCE_FIELD_ID_G999_STREAM_ID;
	filter_class.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_USED;
	filter_class.filter_fields[1].mask.g999_stream_id = 0x3FF;

	filter_class.filter_fields[2].field_id = WP_PCE_FIELD_ID_LAST;

    pce_filter_handle = WP_PceFilterCreate(WP_WINPATH(0), WP_PCE_FILTER_CLASSIFICATION, &filter_class);
	App_TerminateOnError(pce_filter_handle, "WPI_PceFilterCreate()", __LINE__);



    memset(&rule_cfg, 0, sizeof (WP_pce_rule_classification));

	/* Common parameters */

    rule_cfg.filter_handle = pce_filter_handle;
    rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_DEVICE;
    rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_G999_STREAM_ID;
    rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_LAST;
    rule_cfg.match_action = WP_PCE_RULE_MATCH_ACCEPT;
    rule_cfg.match_result[0].result_type = WP_PCE_RESULT_G999_STREAM;
    rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;
    
    rule_cfg.enabled = WP_ENABLE;
    rule_cfg.rule_fields[0].value.device_handle = dev_enet_g999;
    rule_cfg.rule_fields[1].value.g999_stream_id = 0;
    rule_cfg.match_result[0].param.g999_stream.g999_stream_handle = g999_stream;
    pce_rule_handle = WP_PceRuleCreate(WP_WINPATH(0), WP_PCE_RULE_CLASSIFICATION, &rule_cfg);
	App_TerminateOnError(pce_rule_handle, "WPI_PceRuleCreate()",__LINE__);


    memset(&filter_set, 0, sizeof (WP_pce_filter_set));

	filter_set.filter_set_level = 1; 
	filter_set.next_filter_set = WP_UNUSED;
	filter_set.filters[0] = pce_filter_handle;
	filter_set.filters[1] = WP_UNUSED;

	pce_filter_set_handle = WP_PceFilterSetCreate(WP_WINPATH(0), &filter_set);
    App_TerminateOnError(filter_set_handle, "WP_PceFilterSetCreate",__LINE__);



    memset(&pce_if_params_device_cfg, 0, sizeof (WP_pce_if_params_device));

	pce_if_params_device_cfg.filter_set_handle = pce_filter_set_handle;
	pce_if_params_device_cfg.parser_start_type = WP_PCE_PARSER_START_TYPE_G999_ENET_ENCAP;
	pce_if_params_device_cfg.mode = WP_PCE_CONNECTION_ENABLED;
    pce_if_handle = WP_PceInterfaceCreate(WP_PCE_IF_TYPE_DEVICE, &pce_if_params_device_cfg);


    dev_enet_g999_cfg.pce_if_handle = pce_if_handle;


    status = WP_DeviceModify(dev_enet_g999, WP_DEV_MOD_G999_PCE_INTERFACE, &dev_enet_g999_cfg);
    App_TerminateOnError(status, "WP_DeviceModify WP_DEV_MOD_G999_PCE_INTERFACE", __LINE__);

}

static void WPE_BridgingFilterDefinition(WP_handle iw_system)
{
   WP_classification_filter filter_cfg;

   filter_cfg.filter_type = WP_CLASS_FILTER_EMC;
   filter_cfg.no_match_action = WP_CLASS_FILTER_NO_MATCH_DENY; // Changing to FDB & DFC
   filter_cfg.fields_mask = WP_FIELD_VLAN_ID;
   //filter_cfg.field_ids_array[0] = WP_FIELD_ID_IN_PORT;
   //filter_cfg.field_ids_array[0] = WP_FIELD_ID_VLAN_ID;
   //filter_cfg.field_ids_array[1] = WP_FIELD_ID_LAST;
   /* Filter  0 */
   dfc_filter_handle = WP_IwClassFilterAdd(iw_system,&filter_cfg);
   App_TerminateOnError(dfc_filter_handle,"WP_IwClassFilterAdd()",__LINE__);
}

void WPE_BridgingFlowAdd(void)
{
   WP_status status;
   WP_flow_class_rule flow_rule;
   WP_dfc_flow_info dfc_info[1];


   WP_CHAR class_str[200];
   memset(class_str,0,200);
   /****************************************************
   /* add rules for filter 0. Flow Aggregation

   /****************************************************/

      // Filter 0:
	  //==========
      flow_rule.action_info.forwarding_action.flow_aggregation = agg_host_array[0];
      flow_rule.match_action = WP_FLOW_CLASS_MODE_FLOW_AGG;

      dfc_info[0].input_port = 0;//bport_enet_ge;
      dfc_info[0].output_port = 0;
      dfc_info[0].filter_handle = dfc_filter_handle;
      flow_rule.dfc_info = dfc_info;
      sprintf (class_str,"n;n;n;n;n;n;n;n;n;n;1;n;");
      flow_rule.classifier_string = class_str;
      dfc_flow_handle[0] = WP_IwFlowAdd(iw_sys,
                                             WP_FLOW_CLASS_RULE,
                                             &flow_rule);
      App_TerminateOnError(dfc_flow_handle[0], "WP_IwFlowAdd ",__LINE__);

         /* add rules for filter 1. Flow Aggregation
   /****************************************************/
   memset(class_str,0,200);
      // Filter 1:
	  //==========
      flow_rule.action_info.forwarding_action.flow_aggregation = agg_host_array[2];
      flow_rule.match_action = WP_FLOW_CLASS_MODE_FLOW_AGG;

      dfc_info[0].input_port = 0;//bport_enet_ge;
      dfc_info[0].output_port = 0;
      dfc_info[0].filter_handle = dfc_filter_handle;
      flow_rule.dfc_info = dfc_info;
      sprintf (class_str,"n;n;n;n;n;n;n;n;n;n;2;n;");
      flow_rule.classifier_string = class_str;
      dfc_flow_handle[1] = WP_IwFlowAdd(iw_sys,
                                             WP_FLOW_CLASS_RULE,
                                             &flow_rule);
      App_TerminateOnError(dfc_flow_handle[1], "WP_IwFlowAdd ",__LINE__);
     

}

void  WPE_TestCfgInit(void)
{
    WP_handle status;

    status = WP_DriverInit();
    App_TerminateOnError(status, "WP_DriverInit()",__LINE__);
    int_ram_partition.cfu_tx_buffers_num = 8095;
    int_ram_partition.policer_entries = 20;
    int_ram_partition.pce_hw_rules_num = 4000;
    WT_SetIntRamPartition(WP_WINPATH(0), &int_ram_partition);

    WPE_SysInit();
    
    WPE_InitHWCards();


    WPE_CreateBufferPools();


    WPE_CreateGeEnetPortDevice();
   
    WPE_CreateG999EnetPortDevice();

    WPE_CreateHostPortDevice();


    WPE_ModulesInit();

    status = WP_SysCommit();
    App_TerminateOnError(status, "WP_SysCommit",__LINE__);    
     
    WPE_G999StreamsCreate();

    WPE_CreateIWBridgeSystem();

    WPE_CreateHostIwRxChannel();   
    //WPE_CreateHostIwTxChannel();
    WPE_CreateHostTermFlowAgg();
    //WPE_CreateIwBportHost();

    WPE_CreateG999PceClassification();
    WPE_CreatePceFilters();
    WPE_CreatePceFilterSets();
    

    WPE_CreateG999EnetIwBport();
    WPE_CreateG999EnetL1FMUGroups();
    WPE_CreateG999EnetL2FMUGroups();
    WPE_CreateG999EnetRxTxChannel();
    WPE_CreateG999EnetRxTxBinding();
    WPE_CreateG999EnetFlowAgg();
    WPE_CreateG999DevicePceInterface();
	
    

    WPE_CreateGeEnetL1FMUGroups();
    WPE_CreateGeEnetL2FMUGroups();
    WPE_CreateGeEnetIwBport();
    WPE_CreateGeEnetRxTxChannel();
    WPE_CreateGeEnetRxTxBinding();
    WPE_CreateGeEnetFlowAgg();    
    WPE_CreateRxChannelPceInterface(iw_sys);


    status = WP_IwSystemBuild(iw_sys);
    App_TerminateOnError(status, "WP_IwSystemBuild",__LINE__);

    WPE_EnableSystem();

    
    
}
#define NUM_OF_PECS     2
#define NUM_OF_PECS_GLOBAL_INFO      10
extern WP_pecs_image_info header_ext_add_pecs_pecs_image;
#include "wpi_gpe_pecs_wddi_shared.h"

#include "header_ext_add_pecs.h"

static void WPE_Pecs_Init(WP_gpe_pecs gpe_pecs_cfg[],WP_handle pecs_handles[])
{
   WP_U32 ii;
   
   for (ii = 0 ; ii < NUM_OF_PECS ; ii++)
   {
      pecs_handles[ii] = WP_GpePecsCreate(WP_WINPATH(0),WP_GPE_PECS,(void*)&gpe_pecs_cfg[ii]);
      App_TerminateOnError(pecs_handles[ii],"WP_GpePecsCreate",__LINE__);  
   }
}

void  WPE_TestCfgInitwithPecs(void)
{
    WP_handle status;
    WP_handle pecs_handles[NUM_OF_PECS];
    WP_U32 i = 0;

    WP_module_gpe_init gpe_init =
    {
       NUM_OF_PECS, /* max_num_of_pecs */
       NUM_OF_PECS_GLOBAL_INFO, /* max_num_of_pecs_global_info */
    };

    WPE_gpe_header_ext_add_pecs_flow_info header_ext_add_pecs_flow_info[] = 
    {
       {
          0,/* copy_pce_result */
          0,/* extract_l2_header */
          1,/* add_flow_prefix */
          0,/* global_info_mode */
          0,/* pecs_stat_mode */
          0,/* extract_size */
          18,/* flow_prefix_size */
          0,/* global_prefix_size */
          0,/* global_prefix_offset */
          { /* flow_prefix */
             0xff,0xee,0xdd,0xcc,0xbb,0xaa,0x99,0x88,
             0x77,0x66,0x55,0x44,0x33,0x22,0x11,0xfe, 
             0x08,0x00,0,0,0,0,0,0,
             0,0,0,0,0,0,0,0, 
             0,0,0,0,0,0,0,0,
             0,0,0,0,0,0,0,0, 
             0,0,0,0,0,0,0,0,
             0,0,0,0,0,0,0,0, 
          }      
       },

       {
          0,/* copy_pce_result */
          1,/* extract_l2_header */
          0,/* add_flow_prefix */
          0,/* global_info_mode */
          0,/* pecs_stat_mode */
          18,/* extract_size */
          0,/* flow_prefix_size */
          0,/* global_prefix_size */
          0,/* global_prefix_offset */
          { /* flow_prefix */
             0xff,0xee,0xdd,0xcc,0xbb,0xaa,0x99,0x88,
             0x77,0x66,0x55,0x44,0x33,0x22,0x11,0xfe, 
             0x08,0x00,0,0,0,0,0,0,
             0,0,0,0,0,0,0,0, 
             0,0,0,0,0,0,0,0,
             0,0,0,0,0,0,0,0, 
             0,0,0,0,0,0,0,0,
             0,0,0,0,0,0,0,0, 
       }
       }
    
    };

    //pecs cfg array
    WP_gpe_pecs gpe_pecs_cfg[2] = 
    {
       {
          &header_ext_add_pecs_pecs_image, /*pecs_image_info*/
          &WPE_GpeHeaderExtAddPecsService /*pecs_service_fp*/
       },
       
       {
          &header_ext_add_pecs_pecs_image, /*pecs_image_info*/
          &WPE_GpeHeaderExtAddPecsService /*pecs_service_fp*/
       }
    };

    status = WP_DriverInit();
    App_TerminateOnError(status, "WP_DriverInit()",__LINE__);
    int_ram_partition.cfu_tx_buffers_num = 8095;
    int_ram_partition.policer_entries = 20;
    int_ram_partition.pce_hw_rules_num = 4000;
    WT_SetIntRamPartition(WP_WINPATH(0), &int_ram_partition);

    WPE_SysInit();
    
    WPE_InitHWCards();


    WPE_CreateBufferPools();


    WPE_CreateGeEnetPortDevice();
   
    WPE_CreateG999EnetPortDevice();

    WPE_CreateHostPortDevice();


    WPE_ModulesInit();
    status = WP_ModuleInit(WP_SYSHANDLE(0), WP_WDDI_MODULE_GPE, &gpe_init);
    App_TerminateOnError(status, "WPI_GpeModuleInit()",__LINE__);

    status = WP_SysCommit();
    App_TerminateOnError(status, "WP_SysCommit",__LINE__);    

       //init pecs - pecs add for each used pecs
    WPE_Pecs_Init(gpe_pecs_cfg,pecs_handles);
     
    WPE_G999StreamsCreate();

    WPE_CreateIWBridgeSystem();

    WPE_CreateHostIwRxChannel();   
    //WPE_CreateHostIwTxChannel();
    WPE_CreateHostTermFlowAgg();
    //WPE_CreateIwBportHost();

    WPE_CreateG999PceClassification();
    WPE_CreatePceFilters();
    WPE_CreatePceFilterSets();
    

    WPE_CreateG999EnetIwBport();
    WPE_CreateG999EnetL1FMUGroups();
    WPE_CreateG999EnetL2FMUGroups();
    WPE_CreateG999EnetRxTxChannel();
    WPE_CreateG999EnetRxTxBinding();
    WPE_CreateG999EnetFlowAgg();
    WPE_CreateG999DevicePceInterface();
    for(i=0;i < NUM_OF_G999_ENET_TX_CHANNELS; i++)
    {

        WP_iw_agg_generic agg_enet_config =
        {
            
            /*tag*/ agg_tag_counter++,
            /*txfunc*/ 0,
            /*iw_port*/0,
            /*rfcs*/WP_IW_RFCS_ENABLE,
            /*interruptqueue;*/WP_IW_IRQT1,
            /*error_pkt_mode*/WP_IW_ERRPKT_DISCARD,
            /*intmode;*/WP_IW_INT_DISABLE,
            /*statmode;*/WP_IW_STAT_ENABLE,
            /*timestamp_mode;*/WP_IW_TIME_STAMP_DISABLE,
            /*mtu;*/MTU_SIZE_G999,
            /*flow_agg_type;*/WP_IW_FLOW_AGG_PRIMARY,
            /*policer_handle;*/0,
            /*pecs_handle;*/0,
            /*pecs_flow_info;*/0,
            /*pecs_global_info_handle;*/0,
            /*iw_gpmt_handle*/0,
           
        };
        agg_enet_config.pecs_handle = pecs_handles[0];

        agg_enet_config.pecs_flow_info = (void *) &header_ext_add_pecs_flow_info[0];

        status =  WP_IwFlowAggregationModify(g999_enet_tx_flow_agg[i],
                                          WP_IW_FLOW_AGG_GENERIC_MOD_PECS,
                                          &agg_enet_config);  
        App_TerminateOnError(status, "WP_IwFlowAggregationModify G999 ENET",__LINE__);
    }
    


    

    WPE_CreateGeEnetL1FMUGroups();
    WPE_CreateGeEnetL2FMUGroups();
    WPE_CreateGeEnetIwBport();
    WPE_CreateGeEnetRxTxChannel();
    WPE_CreateGeEnetRxTxBinding();
    WPE_CreateGeEnetFlowAgg();    
    WPE_CreateRxChannelPceInterface(iw_sys);

    for(i=0;i < NUM_OF_GE_ENET_TX_CHANNELS; i++)
    {

        WP_iw_agg_generic agg_enet_config =
        {
            
            /*tag*/ agg_tag_counter++,
            /*txfunc*/ 0,
            /*iw_port*/0,
            /*rfcs*/WP_IW_RFCS_ENABLE,
            /*interruptqueue;*/WP_IW_IRQT1,
            /*error_pkt_mode*/WP_IW_ERRPKT_DISCARD,
            /*intmode;*/WP_IW_INT_DISABLE,
            /*statmode;*/WP_IW_STAT_ENABLE,
            /*timestamp_mode;*/WP_IW_TIME_STAMP_DISABLE,
            /*mtu;*/MTU_SIZE_GE,
            /*flow_agg_type;*/WP_IW_FLOW_AGG_PRIMARY,
            /*policer_handle;*/0,
            /*pecs_handle;*/0,
            /*pecs_flow_info;*/0,
            /*pecs_global_info_handle;*/0,
            /*iw_gpmt_handle*/0,
            
        };
        agg_enet_config.pecs_handle = pecs_handles[1];

        agg_enet_config.pecs_flow_info = (void *) &header_ext_add_pecs_flow_info[1];

        status =  WP_IwFlowAggregationModify(ge_enet_tx_flow_agg[i],
                                          WP_IW_FLOW_AGG_GENERIC_MOD_PECS,
                                          &agg_enet_config);  
        App_TerminateOnError(status, "WP_IwFlowAggregationModify GE ENET",__LINE__);
    }
	
    

    status = WP_IwSystemBuild(iw_sys);
    App_TerminateOnError(status, "WP_IwSystemBuild",__LINE__);

    WPE_EnableSystem();

    
    
}


void  WPE_HostTestCfgInit_1(void)
{
    WP_status status;

    status = WP_DriverInit();
    App_TerminateOnError(status, "WP_DriverInit()",__LINE__);
    int_ram_partition.cfu_tx_buffers_num = 8095;
    int_ram_partition.policer_entries = 20;
    int_ram_partition.pce_hw_rules_num = 4000;
    WT_SetIntRamPartition(WP_WINPATH(0), &int_ram_partition);

    WPE_SysInit();
    
    WPE_InitHWCards();


    WPE_CreateBufferPools();


    WPE_CreateGeEnetPortDevice();
   
    WPE_CreateG999EnetPortDevice();

    WPE_CreateHostPortDevice();


    WPE_ModulesInit();

    status = WP_SysCommit();
    App_TerminateOnError(status, "WP_SysCommit",__LINE__);    
     
    WPE_G999StreamsCreate();

    WPE_CreateIWBridgeSystem();

    WPE_CreateHostIwRxChannel();   

    WPE_CreateHostTermFlowAgg();

    WPE_CreateHostIwRxChannelArray();   

    WPE_CreateHostTermFlowAggArray();

    WPE_CreateG999PceClassification();
    WPE_CreatePceFilters();
    WPE_CreatePceFilterSets();
    

    WPE_CreateG999EnetIwBport();
    WPE_CreateG999EnetL1FMUGroups();
    WPE_CreateG999EnetL2FMUGroups();
    WPE_CreateG999EnetRxTxChannel();
    WPE_CreateG999EnetRxTxBinding();
    WPE_CreateG999EnetFlowAgg();
    WPE_CreateG999DevicePceInterface();
	
    

    WPE_CreateGeEnetL1FMUGroups();
    WPE_CreateGeEnetL2FMUGroups();
    WPE_CreateGeEnetIwBport();
    WPE_CreateGeEnetRxTxChannel();
    WPE_CreateGeEnetRxTxBinding();
    WPE_CreateGeEnetFlowAgg();
    WPE_CreateRxChannelPceInterface(iw_sys);


    status = WP_IwSystemBuild(iw_sys);
    App_TerminateOnError(status, "WP_IwSystemBuild",__LINE__);

    WPE_EnableSystem();

    
    
}

void  WPE_HostTestCfgInit_2(void)
{
    WPE_HostTestCfgInit_1();
    WPE_V2PolicerCreate_1(1000000,1000000);
    WPE_V2PolicerCreate_2(500000,500000);
   
}

void  WPE_HostTestCfgInit_3(void)
{
    WP_status status;

    status = WP_DriverInit();
    App_TerminateOnError(status, "WP_DriverInit()",__LINE__);
    int_ram_partition.cfu_tx_buffers_num = 8095;
    int_ram_partition.policer_entries = 20;
    int_ram_partition.pce_hw_rules_num = 4000;
    WT_SetIntRamPartition(WP_WINPATH(0), &int_ram_partition);

    WPE_SysInit();
    
    WPE_InitHWCards();


    WPE_CreateBufferPools();


    WPE_CreateGeEnetPortDevice();
  
    WPE_CreateHostPortDevice();


    WPE_CongestionModulesInit();
    WPE_PolicerModulesInit();

    status = WP_SysCommit();
    App_TerminateOnError(status, "WP_SysCommit",__LINE__);    
 
    WPE_CreateIWBridgeSystem();

    WPE_V1PolicerCreate();
    
    WPE_CreateHostIwRxChannel();   

    WPE_CreateHostTermFlowAgg();
    
    WPE_CreateHostIwRxChannelArray();   

    WPE_CreateHostTermFlowAggArrayWithPolicerV1();

    WPE_CreateGeEnetL1FMUGroups();
    WPE_CreateGeEnetL2FMUGroups();
    WPE_CreateGeEnetIwBport_Host_3();
    WPE_CreateGeEnetRxTxChannel();
    WPE_CreateGeEnetRxTxBinding();
    WPE_CreateGeEnetFlowAgg();
    
    WPE_BridgingFilterDefinition(iw_sys);
    WPE_BridgingFlowAdd();

    status = WP_IwSystemBuild(iw_sys);
    App_TerminateOnError(status, "WP_IwSystemBuild",__LINE__);

    WPE_EnableSystem_Host();

}


void  WPE_HostTestCfgInit_4(void)
{
    WP_status status;

    status = WP_DriverInit();
    App_TerminateOnError(status, "WP_DriverInit()",__LINE__);
    int_ram_partition.cfu_tx_buffers_num = 8095;
    int_ram_partition.policer_entries = 20;
    int_ram_partition.pce_hw_rules_num = 4000;
    WT_SetIntRamPartition(WP_WINPATH(0), &int_ram_partition);

    WPE_SysInit();
    
    WPE_InitHWCards();


    WPE_CreateBufferPools();


    WPE_CreateGeEnetPortDevice();
   
    WPE_CreateG999EnetPortDevice();

    WPE_CreateHostPortDevice();


    WPE_ModulesInit();

    status = WP_SysCommit();
    App_TerminateOnError(status, "WP_SysCommit",__LINE__);    
     
    WPE_G999StreamsCreate();

    WPE_CreateIWBridgeSystem();

    WPE_V2PolicerCreate_1(1000000,1000000);
    WPE_V2PolicerCreate_2(500000,500000);

    WPE_CreateHostIwRxChannel();   

    WPE_CreateHostTermFlowAgg();

    WPE_CreateHostIwRxChannelArray();   

    WPE_CreateHostTermFlowAggArrayWithPolicerV2();

    WPE_CreateG999PceClassification();
    WPE_CreatePceFilters();
    WPE_CreatePceFilterSets();
    

    WPE_CreateG999EnetIwBport();
    WPE_CreateG999EnetL1FMUGroups();
    WPE_CreateG999EnetL2FMUGroups();
    WPE_CreateG999EnetRxTxChannel();
    WPE_CreateG999EnetRxTxBinding();
    WPE_CreateG999EnetFlowAgg();
    WPE_CreateG999DevicePceInterface();
	
    

    WPE_CreateGeEnetL1FMUGroups();
    WPE_CreateGeEnetL2FMUGroups();
    WPE_CreateGeEnetIwBport();
    WPE_CreateGeEnetRxTxChannel();
    WPE_CreateGeEnetRxTxBinding();
    WPE_CreateGeEnetFlowAgg();    
    WPE_CreateRxChannelPceInterface(iw_sys);


    status = WP_IwSystemBuild(iw_sys);
    App_TerminateOnError(status, "WP_IwSystemBuild",__LINE__);

    WPE_EnableSystem();

}
    
    
void  WPE_HostTestCfgInit_5(void)
{
    WPE_HostTestCfgInit_1();   
}


void  WPE_HostTestCfgInit_6(void)
{ 
    WPE_HostTestCfgInit_1();
}

void  WPE_HostTestCfgInit_7(void)
{
    WP_status status;

    status = WP_DriverInit();
    App_TerminateOnError(status, "WP_DriverInit()",__LINE__);
    int_ram_partition.cfu_tx_buffers_num = 8095;
    int_ram_partition.policer_entries = 20;
    int_ram_partition.pce_hw_rules_num = 4000;
    WT_SetIntRamPartition(WP_WINPATH(0), &int_ram_partition);

    WPE_SysInit();
    
    WPE_InitHWCards();


    WPE_CreateBufferPools();


    WPE_CreateGeEnetPortDevice();
   
    WPE_CreateG999EnetPortDevice();

    WPE_CreateHostPortDevice();


    WPE_ModulesInit();

    status = WP_SysCommit();
    App_TerminateOnError(status, "WP_SysCommit",__LINE__);    
     
    WPE_G999StreamsCreate();

    WPE_CreateIWBridgeSystem();

    WPE_CreateHostIwRxChannel();   

    WPE_CreateHostTermFlowAgg();

    WPE_CreateHostIwRxChannelArray();   

    WPE_CreateHostTermFlowAggArray();

    WPE_CreateG999PceClassification();
    WPE_CreatePceFilters();
    WPE_CreatePceFilterSets();
    

    WPE_CreateG999EnetIwBport_Host_7();
    WPE_CreateG999EnetL1FMUGroups();
    WPE_CreateG999EnetL2FMUGroups();
    WPE_CreateG999EnetRxTxChannel();
    WPE_CreateG999EnetRxTxBinding();
    WPE_CreateG999EnetFlowAgg();
    WPE_CreateG999DevicePceInterface();
	
    

    WPE_CreateGeEnetL1FMUGroups();
    WPE_CreateGeEnetL2FMUGroups();
    WPE_CreateGeEnetIwBport_Host_7();
    WPE_CreateGeEnetRxTxChannel();
    WPE_CreateGeEnetRxTxBinding();
    WPE_CreateGeEnetFlowAgg();    
    WPE_CreateRxChannelPceInterface(iw_sys);


    status = WP_IwSystemBuild(iw_sys);
    App_TerminateOnError(status, "WP_IwSystemBuild",__LINE__);

    WPE_EnableSystem();  
    
}


void  WPE_HostTestCfgInit_8(void)
{
    WPE_HostTestCfgInit_7();    
}

#define MTU_MC_FLOW_AGG 1500


void WPE_MulticastInit(void)
{
    WP_U32 entry = 0;
    WP_iw_mc_group mc_group_config =
    {
       
      /* type */WP_IW_MC_UNRESTRICTED,
      /* mode */WP_IW_MC_MODE_MEMBERS_STAT_ENABLE | WP_IW_MC_MODE_GENERIC_GROUP | WP_IW_MC_MODE_MEMBERS_ENHANCED_STAT_ENABLE,
      /* max_iw_mc_members */NUM_OF_MC_MEMBERS_PER_GROUP,
      /* qnode */0,
      /* encaps_gap */64,
      /* classes_config */NULL,
      /* identifier */WP_UNUSED
       
    };


    WP_iw_agg_multicast mc_agg_config =

   {
      /* tag */0,
      /* type */WP_IW_FLOW_AGG_PRIMARY,
      /* mc_group */0,
      /* mtu */MTU_MC_FLOW_AGG,
      /* intmode */WP_IW_INT_DISABLE,
      /* interruptqueue */WP_IW_IRQT1,
      /* statmode */WP_IW_STAT_ENABLE,
      /* timestamp_mode */WP_IW_TIME_STAMP_DISABLE,
      /* ov_pool_mode */WP_IW_OV_POOL_ENABLE,
      /* fbp_drop_threshold */0,
      /* policer_enable */WP_IW_POLICER_DISABLE,
      /* *policer_config */NULL
   };

    WP_iw_mc_member mc_member_config =
   {
      /* txfunc */0,
      /* service_class_index */0,
      /* buffers_threshold */0,
      /* iw_port */0,
      /* edit_size */0x20,
      /* data_copy_mode */WP_IW_MC_MEMBER_DATACOPY_DISABLE,
      /*iw_port_check_mode*/WP_IW_MC_MEMBER_IWPORT_CHECK_ENABLE
   };


 //create mc group 1 piont to ge  
    mc_group_config.qnode = qnode_mc;

    group_handle[0] = WP_IwMcGroupCreate(WP_WINPATH(0), &mc_group_config);
    App_TerminateOnError(group_handle[0],"WP_IwMcGroupCreate() Mc group 1", __LINE__);

    /* Create multicast flow agg group 1*/
    mc_agg_config.mtu = 1000;
    mc_agg_config.mc_group = group_handle[0];
    mc_flowagg_handle[0] = WP_IwFlowAggregationCreate(WP_WINPATH(0),WP_IW_GENERIC_MC_MODE,&mc_agg_config);
    App_TerminateOnError(mc_flowagg_handle[0],"WP_IwFlowAggregationCreate() mc agg 1", __LINE__);

    /* Create Members for the mc Group */
    for(entry=0; entry<8; entry ++)
    {
         mc_member_config.txfunc = ge_enet_tx_flow_agg[entry];//enet tx flow agg
         mc_member_config.iw_port = bport_enet_ge; //output iw port connected to the flow agg which point to tx_ge_enet_channel[entry]
         member_handle[entry] = WP_IwMcMemberAdd(
            group_handle[0], &mc_member_config);
         App_TerminateOnError(member_handle[entry],"WP_IwMcMemberAdd() mc member group 1", __LINE__);
    }

/***create multicast group 2 piont to g999*****/

    group_handle[1] = WP_IwMcGroupCreate(WP_WINPATH(0), &mc_group_config);
    App_TerminateOnError(group_handle[1],"WP_IwMcGroupCreate() Mc group 2", __LINE__);

    /* Create broadcast flow agg using the mc group handle.*/
    mc_agg_config.mtu = 1200;
    mc_agg_config.mc_group = group_handle[1];
    mc_flowagg_handle[1] = WP_IwFlowAggregationCreate(WP_WINPATH(0),WP_IW_GENERIC_MC_MODE,&mc_agg_config);
    App_TerminateOnError(mc_flowagg_handle[1],"WP_IwFlowAggregationCreate() mc agg 2", __LINE__);

    /* Create Members for the mc Group */
    for(entry=0; entry<8; entry ++)
    {
         mc_member_config.txfunc = g999_enet_tx_flow_agg[entry];//enet tx flow agg
         mc_member_config.iw_port = bport_enet_g999; //output iw port connected to the flow agg which point to enet_tx_channel[entry]
         member_handle[entry+8] = WP_IwMcMemberAdd(
            group_handle[1], &mc_member_config);
         App_TerminateOnError(member_handle[entry+8],"WP_IwMcMemberAdd() mc member group 2", __LINE__);
    }
}









