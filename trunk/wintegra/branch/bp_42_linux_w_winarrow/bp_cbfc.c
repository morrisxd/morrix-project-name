/*************************************************************
 * (C) Copyright 2001-2011, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ************************************************************/

/*************************************************************
 *  Appplication : Back Pressure base on Flow Control
 *  File Name    : bp_cbfc.c
 *  Version      : 1
 *  Date         : 23 Feb 2012
 *  Accompanies  : PMC-2113031 Issue 1 BP CBFC Ap. Note
 ************************************************************/



/*************************************************************
 *  Description:
 *      BP CBFC application is demo application for the traffic 
 *      flow control. 
 *  Freerun Mode:
 *      After downloading to the WinMon, run this command to lunch 
 *      the program in 
 *      freerun mode.
 *                  WinMon> go 0x80400000 freerun 0
 *
 *
 *  Manual Test Mode:
 *      Connect the XGI port with SC-LC fiber to the IXIA 10G 
 *      test module.
 *      Create UDP data stream and CBFC flow control stream in 
 *      the IXIA and send them in sequence. Watch the delay of 
 *      the UDP data packets.
 ************************************************************/

#if _WT_MODULE_TEST_COMMENT_ENABLE_
#pragma check_tests pool_alloc 1
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "api/wp_wddi.h"
#include "api/wp_host_io.h"
#include "wt_util.h"
#include "wt_partition_ecc_util.h"
#include "wp_sim.h"

#include "wpi_veneer.h"
#include "wpx_app_data.h"
#include "wpx_oc12_pmc5357.h"
#include "wpx_gei_phy.h"
#include "wpx_pin_mux_selector.h"
#include "app_convert.h"

#include "wpx_enet_phy.h"

#include "include/core/registry/wpi_registry.h"

#include "api/wp_pce.h"

#include "bp_cbfc.h"


WP_handle devhost;
WP_handle filter_handle[NUM_OF_FLOWS];
WP_handle default_agg;
WP_handle pool_2048, pool_144;

WP_handle gbe_port1, gbe_dev1, gbe_port3, gbe_port4;


WP_handle MultiClass_Device_h[NUM_OF_FLOWS];
WP_handle MultiClass_Tx_Channel_h[NUM_OF_FLOWS];

WP_handle l1_group_h[NUM_OF_FLOWS];
WP_handle l2_group_h[NUM_OF_FLOWS];


WP_handle qniw;
WP_handle h_iw_port_gbe_1;
WP_handle rx_host_handle;
WP_handle bport_host;
WP_handle gbe1_rx_ch_handle, gbe1_tx_ch_handle;


WP_handle dl_iwsys_routing;

WP_U32 payload_size[MAX_NUM_OF_CHANNELS][NUM_OF_PACKETS_PER_CHANNEL];
WP_U32 l2_l3_size[MAX_NUM_OF_CHANNELS][NUM_OF_PACKETS_PER_CHANNEL];

WP_CHAR ip_protocol[MAX_NUM_OF_CHANNELS][NUM_OF_PACKETS_PER_CHANNEL][100];
WP_CHAR ip_src[MAX_NUM_OF_CHANNELS][NUM_OF_PACKETS_PER_CHANNEL][100];
WP_CHAR ip_dst[MAX_NUM_OF_CHANNELS][NUM_OF_PACKETS_PER_CHANNEL][100];

WP_handle host_channel_psu_2[1];
WP_U8 expected_data[MAX_NUM_OF_CHANNELS][NUM_OF_PACKETS_PER_CHANNEL][3536];

WP_U32 user_channel = 0;

#include "bp_cbfc_util.c"

WP_U8 scenario_number = 1;
WP_U32 loopback_mode;
WP_boolean global_failed = WP_FALSE, scenario_failed;

/* PCE */
WP_pce_global_stat pce_global_stats;
WP_handle PCE_filter_flow_ctrl[32];
WP_handle PCE_filter_data[32];

WP_handle flow_ctrl_filter_set_lvl0_handle;

WP_handle data_filter_set_lvl0_handle;

WP_handle PCE_rule_flow_ctrl_h[64];
WP_handle PCE_rule_data_h[64];

WP_pce_filter_stat filter_stat;

WP_U16 wpe_test_failures = 0;

WP_handle flow_agg_add_header[NUM_OF_FLOWS];


/******************************************************************************
 * Function name:   App_ResetDevice ()
 * Description  :   Reset all statistics before we start to do anything in avoiding 
 *                  the remained random value in the memory when initializing the 
 *                  WDS3 board.
 *
 * Input  params: none
 * Output params: 
 * Return val   : none
 ******************************************************************************/
void App_ResetDevice (void)
{
    WP_stats_enet s_hs_enet = {
        0
    };
    
    WP_DeviceStatisticsReset (gbe_dev1, &s_hs_enet);

    printf ("All ports' statistics have been reseted\n");
}


static void App_CreateIWSystem (void);

WP_flow_class_ext_action action;
WP_U8 action_type[3];
WP_flow_class_action_info action_info[3];
WP_CHAR dfc_classifier_string[300];
WP_U32 temp, offset;
WP_flow_class_forwarding_action forwarding_action;
WP_flow_class_priority_action priority_action;
WP_U32 freerun_mode = 0;

/******************************************************************************
 * Function name: main ()
 * Description  :   Main function will call different functions for intialising 
 *                  tags, clocks, creating ports, channels and devices.
 *                  For testing one user menu is created through which user 
 *                  can select to  send packets and see the statistics.
 *                  For autorun mode, "[go] 0x80400000 freerun 0" will be the 
 *                  comamnd line paramters.
 *
 * Input  params:   argc: counter of parameters in the command line.
 *                  argv[]: array to store all parameters in command line.
 * Output params:   none
 * Return val   :   none
 ******************************************************************************/
WP_S32 main (WP_S32 argc, WP_CHAR ** argv)
{
    WP_handle status;

    status = 0;

#if 0
{
	WP_U8 sbuffer[1024];
	sscanf ("%s", (void *)sbuffer);
}
#endif

    if ((3 == argc) && (!strcmp (argv[1], "freerun")))
    {
        freerun_mode = WP_TRUE;
        loopback_mode = WP_TRUE;
    } else {
        freerun_mode = WP_FALSE;
        loopback_mode = WP_FALSE;
    }

    printf ("BP CBFC: DEFAULT_WPID(%d)\n", DEFAULT_WPID);

    App_LayerTwoSystemCreate ();

    status = WP_SysCommit ();
    terminate_on_error (status, "WP_SysCommit", __LINE__);
#if DEBUG
    WP_Display (WP_WINPATH (DEFAULT_WPID), gbe_port1, 0, 0);
#endif

    App_ShapingGroupsCreate ();

    App_ChannelsCreate ();

    App_CreateIWSystem ();

    App_HostAggCreate (dl_iwsys_routing);
    App_DlRxBindingCreate (dl_iwsys_routing);

    App_SystemEnable ();

    /* Configure DL Flow Aggregations for IWF packet editing */
    App_DlForwardingTableCreate ();

    status = WP_IwSystemBuild (dl_iwsys_routing);
    terminate_on_error (status, "WP_IwSystemBuild()", __LINE__);

    /* Creates the DL PCE filters and rules (and filter sets) */
    App_CreateDlPceRulesFiltersSets (dl_iwsys_routing);

    App_ResetFlowCtrlPceFilter ();
    App_ResetDataPceFilter ();
    App_ResetDevice ();

    WP_Delay (1000);

    if (argc < 1)
    {
        App_CLI ();
    }
    else if ((3 == argc) && (!strcmp (argv[1], "freerun")))
    {
        App_perform_freerun ();
    } else {
        printf ("Wrong parameter! Only \"go\" or \"go freerun\" supported!");
        /*Quit test */
        App_Quit (0);

    }

    exit (0);
    return 1;
}

#include "bp_cbfc_stats.c"

/******************************************************************************
 * Function name:   App_CreateIWSystem ()
 * Description  :   Creates the interworking system for the application usage.
 *
 * Input  params:   none
 * Output params:   none
 * Return val   :   none
 ******************************************************************************/
static void App_CreateIWSystem (void)
{
    /* Create DL IW system */
    dl_iwsys_routing = WP_IwSystemCreate (WP_WINPATH (DEFAULT_WPID), WP_IW_ROUTING_MODE, &iw_sys_one_routing[0]);
    terminate_on_error (dl_iwsys_routing, "WP_IwSystemCreate", __LINE__);
}



/******************************************************************************
 * Function name:   App_ResetFlowCtrlPceFilter ()
 * Description  :   Reset statistic counter for the flow control filter inside 
 *                  the PCE module before we start to do anything in avoiding 
 *                  the remained random value in the memory when initializing the 
 *                  WDS3 board.
 *
 * Input  params:   none
 * Output params:   none
 * Return val   :   none
 ******************************************************************************/
void App_ResetFlowCtrlPceFilter (void)
{
    WP_U8 i;

    for (i = 0; i < NUM_FLOW_CTRL_FILTERS; i++)
    {
        WP_PceFilterStatisticsReset (PCE_filter_flow_ctrl[i], &filter_stat);
    }
}

/******************************************************************************
 * Function name:   App_ResetDataPceFilter ()
 * Description  :   Reset all PCE data flow filter statistics before we start 
 *                  to do anything in avoiding the remained random value 
 *                  in the memory when initializing the WDS3 board.
 *
 * Input  params:   none
 * Output params:   none
 * Return val   :   none
 ******************************************************************************/
void App_ResetDataPceFilter (void)
{
    WP_U8 i;

    for (i = 0; i < NUM_DATA_FILTERS; i++)
    {
        WP_PceFilterStatisticsReset (PCE_filter_data[i], &filter_stat);
    }
}


/******************************************************************************
 * Function name:   App_SetupRamPartiion ()
 * Description  :   Initializ the RAM partition for PCE usage in this application.
 *
 * Input  params:   none
 * Output params:   none
 * Return val   :   none
 ******************************************************************************/
static void App_SetupRamPartiion (void)
{
    /* Init PCE hardware rules (4K rules) */
    int_ram_partition.policer_entries = 0;
    int_ram_partition.pce_hw_rules_num = 4095;
    WT_SetIntRamPartition (WP_WINPATH (DEFAULT_WPID), &int_ram_partition);
}

 
/******************************************************************************
 * Function name:   App_InitConfig ()
 * Description  :   Initializ the Serdeses used in the application.
 *
 * Input  params:   none
 * Output params:   none
 * Return val   :   none
 ******************************************************************************/
static void App_InitConfig (void)
{
    WP_status status;
    WP_CHAR output_string[256];

    status = WPX_BoardConfigure (WP_WINPATH (DEFAULT_WPID), WPX_CONFIGURE_DEFAULT_CHECKIN);
    terminate_on_error (status, "WPX_BoardConfigure()", __LINE__);

    status = WPX_BoardXgiConfig (WP_WINPATH (DEFAULT_WPID), WP_PORT_XGI1);
    terminate_on_error (status, "WPX_BoardXgiConfig()", __LINE__);
    printf ("App_LayerTwoSystemCreate: WPX_BoardXgiConfig() OK\n");

    status = WPX_BoardSerdesInit (WP_WINPATH (DEFAULT_WPID), WP_PORT_ENET13, loopback_mode);
    sprintf (output_string, "WPX_BoardSerdesInit ENET13 (WPX_BoardSerdesSetLoopback() )");
    terminate_on_error (status, output_string, __LINE__);

    status = WPX_BoardSerdesInit (WP_WINPATH (DEFAULT_WPID), WP_PORT_ENET14, loopback_mode);
    sprintf (output_string, "WPX_BoardSerdesInit ENET14 (WPX_BoardSerdesSetLoopback() )");
    terminate_on_error (status, output_string, __LINE__);

    status = WPX_BoardSerdesInit (WP_WINPATH (DEFAULT_WPID), WP_PORT_XGI1, loopback_mode);
    sprintf (output_string, "WPX_BoardSerdesInit XGI1 (WPX_BoardSerdesSetLoopback() )");
    terminate_on_error (status, output_string, __LINE__);
    printf ("App_LayerTwoSystemCreate: WPX_BoardSerdesInit(loopback) OK\n");

}

/******************************************************************************
 * Function name:   App_PceInit ()
 * Description  :   Initializ the PCE module used in the application.
 *
 * Input  params:   none
 * Output params:   none
 * Return val   :   none
 ******************************************************************************/
static void App_PceInit (void)
{
    WP_status status;

    pce_init.parser_info = &pce_parser_info;
    pce_init.pkt_rx_channel_interfaces_num = 20;
    pce_init.iw_tx_channel_interfaces_num = 20;
    pce_init.learning_queue_config.learning_queue_size = 20;

    status = WP_ModuleInit (WP_SYSHANDLE (DEFAULT_WPID), WP_WDDI_MODULE_PCE, &pce_init);
    terminate_on_error (status, "WPI_PceModuleInit()", __LINE__);
}

/******************************************************************************
 * Function name:   App_CreateQNodes ()
 * Description  :   Initializ the Queue Nodes used in the application.
 *
 * Input  params:   none
 * Output params:   none
 * Return val   :   none
 ******************************************************************************/
static void App_CreateQNodes (void)
{
    /*Creates buffer pools */

    pool_144 = WP_PoolCreate (WP_WINPATH (DEFAULT_WPID), WP_pool_buffer, buffer_data_144);
    terminate_on_error (pool_144, "WP_PoolCreate() pool_144", __LINE__);

    pool_2048 = WP_PoolCreate (WP_WINPATH (DEFAULT_WPID), WP_pool_iwbuffer, buffer_data_2048);
    terminate_on_error (pool_2048, "WP_PoolCreate() pool_2048", __LINE__);

    /* Create an interworking queue node */
    qn_iw->adjunct_pool = pool_2048;
    qniw = WP_QNodeCreate (WP_WINPATH (DEFAULT_WPID), WP_QNODE_IWQ | WP_QNODE_OPT_FMU, qn_iw);
    terminate_on_error (qniw, "WP_QNodeCreate() qniw", __LINE__);
}


/******************************************************************************
 * Function name:   App_CreatePortsDevices ()
 * Description  :   Creates host port, the 10G gbe1(XGI1) port, 8 MultiClass devices 
 *                  used in this application.
 *
 * Input  params:   none
 * Output params:   none
 * Return val   :   none
 ******************************************************************************/
static void App_CreatePortsDevices (void)
{
    WP_U32 ii;
    WP_handle iwp1;
    WP_status status;

    /* Create an IW Host port */
    iwp1 = WP_PortCreate (WP_WINPATH (DEFAULT_WPID), WP_PORT_IW_HOST, NULL);
    terminate_on_error (iwp1, "WP_PortCreate() IW Host", __LINE__);

    /* Create an IW Host Device */
    devhost = WP_DeviceCreate (iwp1, 0, WP_DEVICE_IW_HOST, NULL);
    terminate_on_error (devhost, "WP_DeviceCreate() IW Host", __LINE__);

    /* Create an GEnet Port (the device that use "regular" mode) */

    enet_s_fast->flowmode = WP_ENET_FMU_HIERARCHICAL_SHAPING_MODE;

#if (WT_TRIAL_MASK == 0x4)
    enet_s_fast->interface_mode = WP_ENET_XAUI;
    gbe_port1 = WP_PortCreate (WP_WINPATH (DEFAULT_WPID), WP_PORT_XGI1, enet_s_fast);
    terminate_on_error (gbe_port1, "WP_PortCreate()", __LINE__);
#endif

    enet_s_fast->interface_mode = WP_ENET_SGMII_1000;


    /* Create an GEnet Device (the device that use "regular" mode) */
    enet_dev->max_tx_channels = MAX_NUM_OF_CHANNELS;

    enet_dev->extended_params = NULL;
    gbe_dev1 = WP_DeviceCreate (gbe_port1, 0, WP_DEVICE_ENET, enet_dev);
    terminate_on_error (gbe_dev1, "WP_DeviceCreate() GENET13", __LINE__);

    enet_dev->max_tx_channels = MAX_NUM_OF_CHANNELS;
    enet_dev->extended_params = enet_device_extended_config;

/**************************************************************************/
/* Set extended parameters for enabling the Multi-Class mode */
    enet_device_extended_config->tx_flowcontrol = WP_ENET_FLOW_MULTI_CLASS_CBFC_TIMERS;

    multi_class_config->multi_class_mode = WP_ENET_MULTI_CLASS_EXTERNAL_ONLY;
    multi_class_config->max_multi_class_devices = NUM_OF_FLOWS;
    enet_device_extended_config->multi_class_config = multi_class_config;

    enet_dev->extended_params = enet_device_extended_config;

    status = WP_DeviceModify (gbe_dev1, WP_DEV_MOD_ENET_TX_STATMODE | WP_DEV_MOD_ENET_RX_STATMODE | WP_DEV_MOD_ENET_TX_FLOWCONTROL | WP_DEV_MOD_ENET_MULTI_CLASS_CONFIG, enet_dev);

    terminate_on_error (status, "WP_DeviceModify() GENET13", __LINE__);

    for (ii = 0; ii < NUM_OF_FLOWS; ii++)
    {

        MultiClass_Device_h[ii] = WP_DeviceCreate (gbe_dev1, WP_PHY (0), WP_DEVICE_ENET_MULTI_CLASS, enet_multi_class_dev_cfg);

        terminate_on_error (MultiClass_Device_h[ii], "WP_DeviceCreate() MultiClass", __LINE__);
    }
}

/******************************************************************************
 * Function name:   App_ModifyDev2 ()
 * Description  :   Modify some attributes of the port gbe2 
 *
 * Input  params:   none
 * Output params:   none
 * Return val   :   none
 ******************************************************************************/


/******************************************************************************
 * Function name:   App_LayerTwoSystemCreate ()
 * Description  :   Setup and initializing the RAM partition. Creates ports and their 
 *                  devices including multiclass devices and modify their attributes.
 *
 * Input  params:   none
 * Output params:   none
 * Return val   :   none
 ******************************************************************************/
static void App_LayerTwoSystemCreate (void)
{
    WP_status status;

    /* Call DriverInit, but if it is already initialized, release WDDI and call it again */
    status = WP_DriverInit ();
    if (WP_ERROR (status) == WP_ERR_WDDI_ALREADY_INITIALIZED)
    {
        status = WP_DriverRelease ();
        terminate_on_error (status, "WP_DriverRelease", __LINE__);

        status = WP_DriverInit ();
    }
    terminate_on_error (status, "WP_DriverInit", __LINE__);

    App_SetupRamPartiion ();

    status = WP_SysInit (WP_WINPATH (DEFAULT_WPID), context);
    terminate_on_error (status, "WP_SysInit", __LINE__);

    App_InitConfig ();

    App_PceInit ();

    App_CreateQNodes ();

    App_CreatePortsDevices();
}


/******************************************************************************
 * Function name:   App_SystemEnable ()
 * Description  :   Enable ports, devices, shaping groups, and channels created 
 *                  before.
 *
 * Input  params:   none
 * Output params:   none
 * Return val   :   none
 ******************************************************************************/
static void App_SystemEnable (void)
{
    WP_status status;
    WP_U32 ii;

    /* Enable the L2pi Port (for fast "regular") */
    status = WP_PortEnable (gbe_port1, WP_DIRECTION_DUPLEX);
    terminate_on_error (status, "WP_PortEnable gbe_port1", __LINE__);

    /* L2pi-Enet To L2pi-Enet */
    status = WP_DeviceEnable (gbe_dev1, WP_DIRECTION_DUPLEX);
    terminate_on_error (status, "WP_DeviceEnable gbe_dev1", __LINE__);

/* enable multiclass devices, only TX direction */

    for (ii = 0; ii < NUM_OF_FLOWS; ii++)
    {
        status = WP_DeviceEnable (MultiClass_Device_h[ii], WP_DIRECTION_TX);
        terminate_on_error (status, "WP_DeviceEnable MultiClass_Device_h", __LINE__);
    }

    status = WP_ChannelEnable (gbe1_rx_ch_handle);
    terminate_on_error (status, "WP_ChannelEnable() rx_enet", __LINE__);

    for (ii = 0; ii < NUM_OF_FLOWS; ii++)
    {
        status = WP_ShapingGroupEnable (l1_group_h[ii]);
        terminate_on_error (status, "WP_ShapingGroupEnable l1", __LINE__);

        status = WP_ShapingGroupEnable (l2_group_h[ii]);
        terminate_on_error (status, "WP_ShapingGroupEnable l1", __LINE__);

        status = WP_ChannelEnable (MultiClass_Tx_Channel_h[ii]);
        terminate_on_error (status, "WP_ChannelEnable() tx_enet", __LINE__);
    }

    status = WP_ChannelEnable (host_channel_psu_2[0]);
    terminate_on_error (status, "WP_ChannelEnable() rx host Enet Psu", __LINE__);
}

/******************************************************************************
 * Function name:   App_DlForwardingTableCreate ()
 * Description  :   Creates the flow aggregation for sending packets from PCE to 
 *                  the XGI port in manual test mode, but pointing to the host port
 *                  in freerun mode.
 *                  Path in freerun mode:
 *                      Packets are sent into multiclass devices, go throught channels
 *                      then arrive at XGI port, and then was looped back in the XGI
 *                      port and return to the PCE, and finnaly goes into the host port.
 *                  Path in manual test mode:
 *                      Packets are send from IXIA monitor and then go into the XGI port, 
 *                      and then go into PCE module, and then will be looped back 
 *                      inside the IW system and return to the PCE, to the multiclass
 *                      devices, and finally to the XGI port again and return to 
 *                      the IXIA.
 *
 * Input  params:   none
 * Output params:   none
 * Return val   :   none
 ******************************************************************************/
static void App_DlForwardingTableCreate (void)
{
    WP_U32 flow_index = 0;

    for (flow_index = 0; flow_index < NUM_OF_FLOWS; flow_index++)
    {
        if (WP_TRUE == freerun_mode)
        {
            dl_tx_agg_gbe->txfunc = host_channel_psu_2[0];
        } else {
            dl_tx_agg_gbe->txfunc = MultiClass_Tx_Channel_h[flow_index];
        }

        /* Update the Eth/IP/Udp Headers fields from IWF Table */
//       dl_tx_agg_gbe->prefix_header[WT_MAC_SA_OFFSET + 5] = 0x03;
//       dl_tx_agg_gbe->prefix_header[WT_MAC_DA_OFFSET + 5] = IWF_Table[flow_index][10];
//       dl_tx_agg_gbe->prefix_header[WT_MAC_HEADER_SIZE + WT_IPV4_SA_OFFSET] =
//         IWF_Table[flow_index][5] >> 24;
//       dl_tx_agg_gbe->prefix_header[WT_MAC_HEADER_SIZE + WT_IPV4_SA_OFFSET + 1] =
//         (IWF_Table[flow_index][5] >> 16) & 0xff;
//       dl_tx_agg_gbe->prefix_header[WT_MAC_HEADER_SIZE + WT_IPV4_SA_OFFSET + 2] =
//         (IWF_Table[flow_index][5] >> 8) & 0xff;
//       dl_tx_agg_gbe->prefix_header[WT_MAC_HEADER_SIZE + WT_IPV4_SA_OFFSET + 3] =
//         IWF_Table[flow_index][5] & 0xff;
//       dl_tx_agg_gbe->prefix_header[WT_MAC_HEADER_SIZE + WT_IPV4_DA_OFFSET] =
//         IWF_Table[flow_index][6] >> 24;
//       dl_tx_agg_gbe->prefix_header[WT_MAC_HEADER_SIZE + WT_IPV4_DA_OFFSET + 1] =
//         (IWF_Table[flow_index][6] >> 16) & 0xff;
//       dl_tx_agg_gbe->prefix_header[WT_MAC_HEADER_SIZE + WT_IPV4_DA_OFFSET + 2] =
//         (IWF_Table[flow_index][6] >> 8) & 0xff;
//       dl_tx_agg_gbe->prefix_header[WT_MAC_HEADER_SIZE + WT_IPV4_DA_OFFSET + 3] =
//         IWF_Table[flow_index][6] & 0xff;
//       dl_tx_agg_gbe->prefix_header[WT_MAC_HEADER_SIZE +
//                                    WT_IPV4_HEADER_SIZE +
//                                    WT_L4_SRC_PORT_OFFSET] = IWF_Table[flow_index][7] >> 8;
//       dl_tx_agg_gbe->prefix_header[WT_MAC_HEADER_SIZE +
//                                    WT_IPV4_HEADER_SIZE +
//                                    WT_L4_SRC_PORT_OFFSET + 1] = IWF_Table[flow_index][7] & 0xff;
//       dl_tx_agg_gbe->prefix_header[WT_MAC_HEADER_SIZE +
//                                    WT_IPV4_HEADER_SIZE +
//                                    WT_L4_DST_PORT_OFFSET] = IWF_Table[flow_index][8] >> 8;
//       dl_tx_agg_gbe->prefix_header[WT_MAC_HEADER_SIZE +
//                                    WT_IPV4_HEADER_SIZE +
//                                    WT_L4_DST_PORT_OFFSET + 1] = IWF_Table[flow_index][8] & 0xff;
//       dl_tx_agg_gbe->prefix_header[WT_MAC_HEADER_SIZE +
//                                    WT_IPV4_PROTOCOL_OFFSET] = IWF_Table[flow_index][9];

        flow_agg_add_header[flow_index] = WP_IwFlowAggregationCreate (WP_WINPATH (DEFAULT_WPID), WP_IW_DIRECTMAP_MODE, dl_tx_agg_gbe);
        terminate_on_error (flow_agg_add_header[flow_index], "WP_IwFlowAggregationCreate() flow_agg_add_header", __LINE__);
    }
}

/******************************************************************************
 * Function name:   App_ShapingGroupsCreate ()
 * Description  :   Creates the L1, L2 shaping group upon the multiclass devices
 *                  based on the XGI MPE port.
 *
 * Input  params:   none
 * Output params:   none
 * Return val   :   none
 ******************************************************************************/
static void App_ShapingGroupsCreate (void)
{

    WP_U32 ii;

    memset (l1_group_shaping_params, 0, sizeof (l1_group_shaping_params));
    l1_group_shaping_params[0].cir = 200000000;
    l1_group_shaping_params[0].cbs = 800000;
    l1_group_shaping_params[0].eir = 200000000;
    l1_group_shaping_params[0].ebs = 800000;
    l1_group_shaping_params[0].flags = 0;

    memset (packet_group_l1_config, 0, sizeof (packet_group_l1_config));
    packet_group_l1_config[0].group_level = WP_L1_GROUP;
    packet_group_l1_config[0].tx_shaping_type = WP_FMU_SHAPING_TYPE_CIR_EIR;
    packet_group_l1_config[0].tx_shaping_params = &l1_group_shaping_params[0];
    packet_group_l1_config[0].num_fifos = WP_NUM_FIFOS_8;
    packet_group_l1_config[0].block_level = 0;
    packet_group_l1_config[0].group_mode = WP_MODE_HW;

    memset (l2_group_shaping_params, 0, sizeof (l2_group_shaping_params));
    l2_group_shaping_params[0].cir = 200000000;
    l2_group_shaping_params[0].cbs = 800000;
    l2_group_shaping_params[0].eir = 200000000;
    l2_group_shaping_params[0].ebs = 800000;
    l2_group_shaping_params[0].flags = 0;

    memset (packet_group_l2_config, 0, sizeof (packet_group_l2_config));
    packet_group_l2_config[0].group_level = WP_L2_GROUP;
    packet_group_l2_config[0].tx_shaping_type = WP_FMU_SHAPING_TYPE_CIR_EIR;
    packet_group_l2_config[0].tx_shaping_params = &l2_group_shaping_params[0];
    packet_group_l2_config[0].num_fifos = WP_NUM_FIFOS_8;
    packet_group_l2_config[0].block_level = 0;
    packet_group_l2_config[0].group_mode = WP_MODE_HW;

    for (ii = 0; ii < NUM_OF_FLOWS; ii++)
    {

        l1_group_h[ii] = WP_ShapingGroupCreate (MultiClass_Device_h[ii], WP_SHAPING_GROUP_TYPE_ENET, &packet_group_l1_config[0]);

        terminate_on_error (l1_group_h[ii], "l1_group create", __LINE__);

        l2_group_h[ii] = WP_ShapingGroupCreate (l1_group_h[ii], WP_SHAPING_GROUP_TYPE_ENET, &packet_group_l2_config[0]);
        terminate_on_error (l2_group_h[ii], "l2_group[0] create", __LINE__);
    }

}

/******************************************************************************
 * Function name:   App_ChannelsCreate ()
 * Description  :   Creates channles connecting host port to the IW system, 
 *                  connecting XGI port to the IW system, connecting PCE module 
 *                  to the host port and XGI port.
 *
 * Input  params:   none
 * Output params:   none
 * Return val   :   none
 ******************************************************************************/
static void App_ChannelsCreate (void)
{
    WP_tag ch_tag = 0;
    WP_status status;
    WP_U32 ii;

    /* Create Enet1 fast ("reguler") RX channel */
    gbe1_rx_ch_handle = WP_ChannelCreate (ch_tag++, gbe_dev1, qniw, WP_CH_RX, WP_ENET, &gbe_ch[0]);
    terminate_on_error (gbe1_rx_ch_handle, "WP_ChannelCreate() RX_Channel ENET13", __LINE__);

    for (ii = 0; ii < NUM_OF_FLOWS; ii++)
    {
        MultiClass_Tx_Channel_h[ii] = WP_ChannelCreate (ch_tag++, l2_group_h[ii], qniw, WP_CH_TX, WP_ENET, &gbe_ch[1]);
        terminate_on_error (MultiClass_Tx_Channel_h[ii], "WP_ChannelCreate() TX_Channel  - MultiClass  ENET13", __LINE__);
    }

    for (ii = 0; ii < NUM_OF_FLOWS; ii++)
    {
        status = WP_IwTxBindingCreate (MultiClass_Tx_Channel_h[ii], WP_IW_TX_BINDING, &tx_binding_l2pi[0]);
        terminate_on_error (status, "WP_IwTxBindingCreate() TX ENET fast device", __LINE__);
    }


    /*create host rx channel */
    rx_host_handle = WP_ChannelCreate (0x1111, devhost, qniw, WP_CH_RX, WP_IW_HOST, &ch_config_iw[0]);
    terminate_on_error (rx_host_handle, "WP_ChannelCreate() Host sys", __LINE__);

    host_ch_cfg[0].tx_binding_config = tx_binding_cfg;

    /* L2pi-Enet To L2pi-Enet */
    host_channel_psu_2[0] = WP_ChannelCreate (ch_tag++, devhost, qniw, WP_CH_RX, WP_IW_HOST, host_ch_cfg);
    terminate_on_error (host_channel_psu_2[0], "WP_ChannelCreate() host_channel_psu_2", __LINE__);
}

/******************************************************************************
 * Function name:   App_HostAggCreate ()
 * Description  :   Creates th default flow aggregation pointing to the host port.
 *
 * Input  params:   none
 * Output params:   none
 * Return val   :   none
 ******************************************************************************/
static void App_HostAggCreate (WP_handle iw_sys)
{
    WP_status status;

    /*   Build system1 - Rx GBE---> Tx GBE */

    br_agg_gbe->txfunc = rx_host_handle;
    default_agg = WP_IwFlowAggregationCreate (WP_WINPATH (DEFAULT_WPID), WP_IW_VLAN_AWARE_BRIDGING_MODE, &br_agg_gbe[0]);
    terminate_on_error (default_agg, "WP_IwFlowAggregationCreate()", __LINE__);
#if 1
	printf ("App_HostAggCreate: default_agg(%x)\n", default_agg);
#endif

    iwhost_bport_config->flow_agg = default_agg;
    bport_host = WP_IwPortCreate (iw_sys, &iwhost_bport_config[0]);
    terminate_on_error (bport_host, " WP_IwPortCreate()", __LINE__);

    br_agg_gbe->input_bport = bport_host;
    status = WP_IwFlowAggregationModify (default_agg, WP_IW_FLOW_AGG_B_MOD_BPORT, &br_agg_gbe[0]);
    terminate_on_error (status, " WP_IwFlowAggregationModify()", __LINE__);

    route_port_cfg.flow_agg = default_agg;
    route_port_cfg.tag = 0;
    route_port_cfg.forwarding_mode = WP_IW_FORWARDING_DIRECT_MAP;

    h_iw_port_gbe_1 = WP_IwPortCreate (iw_sys, &route_port_cfg);
    terminate_on_error (h_iw_port_gbe_1, "WP_IwPortCreate", __LINE__);
}

/******************************************************************************
 * Function name:   App_DlRxBindingCreate ()
 * Description  :   Creates RX binding connecting XGI port with the IW system's 
 *                  bridging port.
 *                  used in this application.
 *
 * Input  params:   WP_handle iw_sys: handle of the interworking system.
 * Output params:   none
 * Return val   :   none
 ******************************************************************************/
static void App_DlRxBindingCreate (WP_handle iw_sys)
{
    WP_status status;

    /*   Build system1 - Rx GBE ---> Tx GBE */

    rx_binding_enet_routing[0].input_port = h_iw_port_gbe_1;
    rx_binding_enet_routing[0].pce_if_handle = 0;
    status = WP_IwRxBindingCreate (gbe1_rx_ch_handle, iw_sys, qniw, &rx_binding_enet_routing[0]);
    terminate_on_error (status, "WP_IwRxBindingCreate Enet", __LINE__);
}


#if PCE_DEBUG
/******************************************************************************
 * Function name:   App_PrintPceGlobalStats ()
 * Description  :   Prints PCE global statistics counters
 *
 * Input  params:   none
 * Output params:   none
 * Return val   :   none
 ******************************************************************************/
void App_PrintPceGlobalStats (void)
{
    WP_U32 temp1, temp2;

    WP_PceGlobalStatistics (WP_WINPATH (DEFAULT_WPID), &pce_global_stats);

    printf ("PCE Global Statistics\n");

    temp1 = pce_global_stats.parser_header_access_errors;
    temp2 = pce_global_stats.parser_header_access_errors >> 32;
    printf ("parser_header_access_errors %08X %08X\n", temp1, temp2);

    temp1 = pce_global_stats.parser_timeout_err;
    temp2 = pce_global_stats.parser_timeout_err >> 32;
    printf ("parser_timeout_err          %08X %08X\n", temp1, temp2);

    temp1 = pce_global_stats.classifier_timeout_err;
    temp2 = pce_global_stats.classifier_timeout_err >> 32;
    printf ("classifier_timeout_err      %08X %08X\n", temp1, temp2);

    temp1 = pce_global_stats.parser_accept;
    temp2 = pce_global_stats.parser_accept >> 32;
    printf ("parser_accept               %08X %08X\n", temp1, temp2);

    printf ("\n\n\n");
}
#endif

/******************************************************************************
 * Function name:   App_CreateDataPceFilters ()
 * Description  :   Creates normal data flow filters inside the PCE module.
 *
 * Input  params:   none
 * Output params:   none
 * Return val   :   none
 ******************************************************************************/
static void App_CreateDataPceFilters (void)
{
    WP_pce_filter_classification filter_class = { 0 };

    /* Common parameters for all filters */
    filter_class.no_match_action = WP_PCE_FILTER_NO_MATCH_DENY;
    filter_class.no_fields_action = WP_PCE_FILTER_NO_FIELDS_DENY;
    filter_class.no_match_result[0].result_type = WP_PCE_RESULT_LAST;

    filter_class.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
    filter_class.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;
    filter_class.filter_fields[0].field_id = WP_PCE_FIELD_ID_IPV4_DA;

    filter_class.filter_fields[1].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
    filter_class.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;
    filter_class.filter_fields[1].field_id = WP_PCE_FIELD_ID_IPV4_SA;

    filter_class.filter_fields[2].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
    filter_class.filter_fields[2].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;
    filter_class.filter_fields[2].field_id = WP_PCE_FIELD_ID_L4_SRC_PORT;

    filter_class.filter_fields[3].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
    filter_class.filter_fields[3].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;
    filter_class.filter_fields[3].field_id = WP_PCE_FIELD_ID_L4_DST_PORT;

    filter_class.filter_fields[4].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
    filter_class.filter_fields[4].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;
    filter_class.filter_fields[4].field_id = WP_PCE_FIELD_ID_IPV4_PROTOCOL;

    filter_class.filter_fields[5].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
    filter_class.filter_fields[5].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;
    filter_class.filter_fields[5].field_id = WP_PCE_FIELD_ID_LAST;

    PCE_filter_data[0] = WP_PceFilterCreate (WP_WINPATH (DEFAULT_WPID), WP_PCE_FILTER_CLASSIFICATION, &filter_class);
    terminate_on_error (PCE_filter_data[0], "WP_PceFilterCreate", __LINE__);
}

/******************************************************************************
 * Function name:   App_CreateFlowCtrlPceFilters ()
 * Description  :   Creates CBFC flow control filter inside the PCE.
 *
 * Input  params:   none
 * Output params:   none
 * Return val   :   none
 ******************************************************************************/
static void App_CreateFlowCtrlPceFilters (void)
{
    WP_pce_filter_flow_control filter_flow_ctrl = { 0 };
    WP_pce_filter_cbfc_class_id cbfc_class;
    WP_U32 i;

    /* Common parameters for all filters */
    filter_flow_ctrl.no_match_action = WP_PCE_FILTER_NO_MATCH_CONTINUE;
    filter_flow_ctrl.no_fields_action = WP_PCE_FILTER_NO_FIELDS_DENY;
    filter_flow_ctrl.no_match_result[0].result_type = WP_PCE_RESULT_LAST;

    filter_flow_ctrl.pause_frame_type = WP_PCE_FLOW_CONTROL_CBFC;
    filter_flow_ctrl.flow_control_pause_id = &cbfc_class;

    filter_flow_ctrl.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
    filter_flow_ctrl.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;
    filter_flow_ctrl.filter_fields[0].field_id = WP_PCE_FIELD_ID_FLOW_CONTROL_BITS;

    filter_flow_ctrl.filter_fields[1].field_id = WP_PCE_FIELD_ID_DEVICE;    /*Rx CFU Port */
    filter_flow_ctrl.filter_fields[1].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
    filter_flow_ctrl.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

    filter_flow_ctrl.filter_fields[2].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
    filter_flow_ctrl.filter_fields[2].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;
    filter_flow_ctrl.filter_fields[2].field_id = WP_PCE_FIELD_ID_LAST;

    for (i = 0; i < NUM_OF_FLOWS; i++)
    {

        cbfc_class.class_id = 15 - i;

        PCE_filter_flow_ctrl[i] = WP_PceFilterCreate (WP_WINPATH (DEFAULT_WPID), WP_PCE_FILTER_FLOW_CONTROL, &filter_flow_ctrl);
        terminate_on_error (PCE_filter_flow_ctrl[i], "WP_PceFilterCreate", __LINE__);
    }
}

/******************************************************************************
 * Function name:   App_CreateFlowCtrlPceRules ()
 * Description  :   Creates rules inside the PCE module for flow control.
 *
 * Input  params:   none
 * Output params:   none
 * Return val   :   none
 ******************************************************************************/
static void App_CreateFlowCtrlPceRules (void)
{
    WP_pce_rule_classification rule_cfg;
    WP_U32 flow_index;
    WP_U32 rule_index;

    /* Common parameters */
    rule_cfg.enabled = WP_ENABLE;

    for (flow_index = 0; flow_index < NUM_OF_FLOWS; flow_index++)
    {
        rule_index = flow_index;
        rule_cfg.filter_handle = PCE_filter_flow_ctrl[flow_index];

        rule_cfg.match_action = WP_PCE_RULE_MATCH_CONTINUE;

        rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_CONTROL;
        rule_cfg.match_result[0].param.flow_control.result_flow_control = WP_PCE_RESULT_FLOW_CONTROL_TYPE_TIMER;
        rule_cfg.match_result[0].param.flow_control.counter_id = flow_index;
        rule_cfg.match_result[0].param.flow_control.pause_handle = MultiClass_Device_h[flow_index];
        rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;

        rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_FLOW_CONTROL_BITS;
        rule_cfg.rule_fields[0].value.flow_control_mode = 1;

        rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_DEVICE;
        rule_cfg.rule_fields[1].value.device_handle = gbe_dev1;

        rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_LAST;

        PCE_rule_flow_ctrl_h[rule_index] = WP_PceRuleCreate (WP_WINPATH (DEFAULT_WPID), WP_PCE_RULE_FLOW_CONTROL, &rule_cfg);
        terminate_on_error (PCE_rule_flow_ctrl_h[rule_index], "WP_PceRuleCreate()", __LINE__);

#if DEBUG
        printf ("PCE_rule_flow_ctrl_h[%d]\n", rule_index);
        WP_Display (WP_WINPATH (DEFAULT_WPID), PCE_rule_flow_ctrl_h[rule_index], 0, 0);
#endif
    }
}

/******************************************************************************
 * Function name:   App_CreateDataPceRules ()
 * Description  :   Creates  rules inside the PCE module for normal data flow.
 *
 * Input  params:   none
 * Output params:   none
 * Return val   :   none
 ******************************************************************************/
static void App_CreateDataPceRules (void)
{
    WP_pce_rule_flow_control rule_cfg;
    WP_U32 flow_index;
    WP_U32 rule_index;

    /* Common parameters */
    rule_cfg.enabled = WP_ENABLE;

    for (flow_index = 0; flow_index < NUM_OF_FLOWS; flow_index++)
    {
        rule_index = flow_index;
        rule_cfg.filter_handle = PCE_filter_data[0];

        rule_cfg.match_action = WP_PCE_RULE_MATCH_CONTINUE;

        rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
        rule_cfg.match_result[0].param.flow_agg.flow_aggregation = flow_agg_add_header[flow_index];
        rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;

        rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_IPV4_DA;
        rule_cfg.rule_fields[0].value.ipv4_addr = IWF_Table[flow_index][1];

        rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_IPV4_SA;
        rule_cfg.rule_fields[1].value.ipv4_addr = IWF_Table[flow_index][0];

        rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_L4_SRC_PORT;
        rule_cfg.rule_fields[2].value.l4_port = IWF_Table[flow_index][2];

        rule_cfg.rule_fields[3].field_id = WP_PCE_FIELD_ID_L4_DST_PORT;
        rule_cfg.rule_fields[3].value.l4_port = IWF_Table[flow_index][3];

        rule_cfg.rule_fields[4].field_id = WP_PCE_FIELD_ID_IPV4_PROTOCOL;
        rule_cfg.rule_fields[4].value.ipv4_prot = IWF_Table[flow_index][4];

        rule_cfg.rule_fields[5].field_id = WP_PCE_FIELD_ID_LAST;

        PCE_rule_data_h[rule_index] = WP_PceRuleCreate (WP_WINPATH (DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &rule_cfg);
        terminate_on_error (PCE_rule_data_h[rule_index], "WP_PceRuleCreate()", __LINE__);
    }
}

/******************************************************************************
 * Function name:   App_CreateFlowCtrlPceFilterSets ()
 * Description  :   Creates filter sets inside the PCE module for flow control.
 *
 * Input  params:   none
 * Output params:   none
 * Return val   :   none
 ******************************************************************************/
static void App_CreateFlowCtrlPceFilterSets (void)
{
    WP_U32 i;
    WP_pce_filter_set fs_level0;

    fs_level0.filter_set_level = 0;
    fs_level0.next_filter_set = WP_UNUSED;
    for (i = 0; i < NUM_FLOW_CTRL_FILTERS; i++)
    {
        fs_level0.filters[i] = PCE_filter_flow_ctrl[i];
    }

    fs_level0.filters[NUM_FLOW_CTRL_FILTERS] = WP_UNUSED;
    flow_ctrl_filter_set_lvl0_handle = WP_PceFilterSetCreate (WP_WINPATH (DEFAULT_WPID), &fs_level0);
#if DEBUG
    printf ("\n\nFlow control filter set:\n");
    WP_Display (WP_WINPATH (DEFAULT_WPID), flow_ctrl_filter_set_lvl0_handle, 0, 0);
#endif

}

/******************************************************************************
 * Function name:   App_CreateDataPceFilterSets ()
 * Description  :   Creates filter sets inside the PCE module for normal data flow.
 *
 * Input  params:   none
 * Output params:   none
 * Return val   :   none
 ******************************************************************************/
static void App_CreateDataPceFilterSets (void)
{
    WP_U32 i;
    WP_pce_filter_set fs_level0;

    fs_level0.filter_set_level = 0;
    fs_level0.next_filter_set = WP_UNUSED;
    for (i = 0; i < NUM_DATA_FILTERS; i++)
    {
        fs_level0.filters[i] = PCE_filter_data[i];
    }
    fs_level0.filters[NUM_DATA_FILTERS] = WP_UNUSED;
    data_filter_set_lvl0_handle = WP_PceFilterSetCreate (WP_WINPATH (DEFAULT_WPID), &fs_level0);

#if DEBUG
    printf ("\n\nData filter set:\n");
    WP_Display (WP_WINPATH (DEFAULT_WPID), data_filter_set_lvl0_handle, 0, 0);
#endif
}

/******************************************************************************
 * Function name:   App_CreatePceInterface ()
 * Description  :   Creates PCE interface upon the RX binding in connecting XGI port
 *                  to PCE module.
 *
 * Input  params:   WP_handle iw_sys: handle of the interworking system.
 * Output params:   none
 * Return val   :   none
 ******************************************************************************/
static void App_CreatePceInterface (WP_handle iw_sys)
{
    WP_status status;
    WP_pce_if_params_pkt_rx_channel pce_if_params = { 0 };
    WP_handle pce_if_handle;

    pce_if_params.mode = WP_PCE_IW_PORT_CONNECTION_ENABLED;
    pce_if_params.parser_start_type = WP_PCE_PARSER_START_TYPE_ETHERNET;
    pce_if_params.filter_set_handle = data_filter_set_lvl0_handle;
    pce_if_params.ip_header_validation = WP_DISABLE;

    pce_if_handle = WP_PceInterfaceCreate (WP_PCE_IF_TYPE_PKT_RX_CHANNEL, &pce_if_params);

    terminate_on_error (pce_if_handle, "WP_PceInterfaceCreate()", __LINE__);

    rx_binding_enet_routing[0].pce_if_handle = pce_if_handle;

    status = WP_IwRxBindingModify (gbe1_rx_ch_handle, iw_sys, qniw, WP_IW_RX_BIND_MOD_PCE_INTERFACE, &rx_binding_enet_routing[0]);

    terminate_on_error (status, "WP_IwRxBindingModify", __LINE__);
}


/******************************************************************************
 * Function name:   App_CreateDlPceRulesFiltersSets ()
 * Description  :   Call each individual function to:
 *                  Creates filters for flow control, filters for normal data flow,
 *                  Creates rules for flow control, rules for normal data flow.
 *                  Create filter sets.
 *                  Creates all the above inside the PCE module.
 *
 * Input  params:   none
 * Output params:   none
 * Return val   :   none
 ******************************************************************************/
static void App_CreateDlPceRulesFiltersSets (WP_handle iw_sys)
{

    App_CreateFlowCtrlPceFilters ();
    App_CreateDataPceFilters ();

    App_CreateFlowCtrlPceRules ();
    App_CreateDataPceRules ();

    App_CreateFlowCtrlPceFilterSets ();
    App_CreateDataPceFilterSets ();

    App_CreatePceInterface (iw_sys);
}

/******************************************************************************
 * Function name:   App_HostSendFlowCtrl ()
 * Description  :   Send flow control packets from host into the multiclass devices.
 *
 * Input  params:   none
 * Output params:   none
 * Return val   :   none
 ******************************************************************************/
static void App_HostSendFlowCtrl ()
{
    WP_data_segment km_tx_buf[3];
    WP_U8 km_data_ptr[3];
    WP_U8 tmp_buffer[2 * MAX_DATA_LENGTH];
    WP_data_unit data_unit;

    WP_data_segment *curr_buff_ptr;
    WP_status status;
    WP_U8 j1 = 0;

    WP_U32 temp_index;
    WP_U32 packet_index_per_channel[MAX_NUM_OF_CHANNELS];

    memset (&packet_index_per_channel, 0x00, sizeof (packet_index_per_channel));
    j1 = 0;

    for (temp_index = 0; temp_index < MAX_NUM_OF_PACKETS; temp_index++)
    {
        printf ("data_unit(%p)seg(%p), temp_index(%d)\n", (void *)&data_unit, (void *)&data_unit.segment, temp_index);
        FlowCtrlDataUnitSetup (&data_unit, &km_tx_buf[0], &km_data_ptr[0]);

        WP_MEM_BYTES_GET (tmp_buffer, data_unit.segment->data, data_unit.segment->data_size);

        /*------------------------------------------------------------*\
            send the packets
        \*------------------------------------------------------------*/
        status = WP_HostSend (MultiClass_Tx_Channel_h[0], &data_unit);

        terminate_on_error (status, "WP_HostSend()", __LINE__);
        curr_buff_ptr = data_unit.segment;

        data_unit.n_active = 0;
        // WP_Delay (100000);
    }
}

/******************************************************************************
 * Function name:   App_HostSendTraffic ()
 * Description  :   Genereating normal UDP data packets and send them into 
 *                  the multiclass devices.
 *
 * Input  params:   none
 * Output params:   none
 * Return val   :   none
 ******************************************************************************/
static void App_HostSendTraffic ()
{
    WP_data_segment km_tx_buf[3];
    WP_U8 km_data_ptr[3];
    WP_U8 tmp_buffer[2 * MAX_DATA_LENGTH];
    WP_data_unit data_unit;

    WP_data_segment *curr_buff_ptr;
    WP_status status;

    WP_CHAR SRC_MAC[100];
    WP_CHAR DST_MAC[100];

    WP_U32 temp_index;
    WP_U32 packet_index_per_channel[MAX_NUM_OF_CHANNELS];

    memset (&packet_index_per_channel, 0x00, sizeof (packet_index_per_channel));

    for (temp_index = 0; temp_index < NUM_OF_FLOWS; temp_index++)
    {
        WP_U32 packet_index = 0, channel_index;

        channel_index = temp_index;

        /* ip protocol */
        sprintf (ip_protocol[channel_index][packet_index], "%02x", IWF_Table[temp_index][4]);

        payload_size[channel_index][packet_index] = 64;

#if DEBUG
        printf ("\nHost Send to: channel_index=%d, packet_index_per_channel=%d, payload_size=%d", channel_index, packet_index, payload_size[channel_index][packet_index]);
#endif

        sprintf (SRC_MAC, "000102030402");
        sprintf (DST_MAC, "000102030403");

        /* ipv4 source address */
        sprintf (ip_src[channel_index][packet_index], "%08x", IWF_Table[temp_index][0]);

        /* ipv4 destination address */
        sprintf (ip_dst[channel_index][packet_index], "%08x", IWF_Table[temp_index][1]);

        DataUnitSetup (&data_unit,
                       &km_tx_buf[1],
                       &km_data_ptr[1],
                       DST_MAC,
                       SRC_MAC,
                       ip_protocol[channel_index][packet_index],
                       ip_src[channel_index][packet_index], ip_dst[channel_index][packet_index], payload_size[channel_index][packet_index], channel_index, packet_index);

        WP_MEM_BYTES_GET (tmp_buffer, data_unit.segment->data, data_unit.segment->data_size);

        status = WP_HostSend (MultiClass_Tx_Channel_h[channel_index], &data_unit);

        terminate_on_error (status, "WP_HostSend()", __LINE__);
        curr_buff_ptr = data_unit.segment;

        data_unit.n_active = 0;
    }
}

/******************************************************************************
 * Function name:   App_CheckReceivedTraffic ()
 * Description  :   Receive and counter the number packets got from the PCE module.
 *                  Set flag indicating the result.
 *
 * Input  params:   none
 * Output params:   none
 * Return val   :   none
 ******************************************************************************/
static void App_CheckReceivedTraffic (WP_U32 expected_count)
{
    WP_U32 index = 0;
    WP_data_unit rx_data_unit;
    WP_data_segment *curr_buff_ptr;
    WP_U32 packet_count = 0, channel_index = 0;
    WP_U8 tmp_buffer[2 * MAX_DATA_LENGTH];
    WP_U32 ii, i;
    WP_status status;
    WP_U32 number_of_segments = 4;
    WP_data_segment data_segment[number_of_segments];

    tmp_buffer[0] = ' ';
    channel_index = 0;
    curr_buff_ptr = NULL;
    index = 0;

    scenario_failed = WP_FALSE;

    for (i = 0; i < WT_RX_LOOP_LIMIT; i++)
    {
        /* receive packet from host queue */
        for (ii = 0; ii < number_of_segments; ii++)
        {
            data_segment[ii].displacement = 0;
            data_segment[ii].data_size = 0;
            data_segment[ii].pool_handle = 0;
            data_segment[ii].data = 0;
            data_segment[ii].displacement = 0;
            if (ii == number_of_segments - 1)
                data_segment[ii].next = NULL;
            else
                data_segment[ii].next = &(data_segment[ii + 1]);
        }

        rx_data_unit.segment = (WP_data_segment *) & data_segment;

        rx_data_unit.n_segments = number_of_segments;
        rx_data_unit.type = WP_DATA_IW;
        rx_data_unit.n_active = 0;
        rx_data_unit.control = WP_HT_CONTROL (0);

        status = WP_HostReceive (host_channel_psu_2[0], &rx_data_unit);

        if (status != WP_OK)
        {
            if (WP_ERROR (status) != WP_ERR_HST_NO_DATA_TO_GET)
                terminate_on_error (status, "WP_HostReceive host_channel", __LINE__);
        }
        else
        {
            WP_U32 offset = rx_data_unit.segment[0].data_size;

            for (ii = 1; ii < rx_data_unit.n_active; ii++)
            {
                memcpy (rx_data_unit.segment[0].data + offset, rx_data_unit.segment[ii].data, rx_data_unit.segment[ii].data_size);
                rx_data_unit.segment[0].data_size += rx_data_unit.segment[ii].data_size;
                offset += rx_data_unit.segment[ii].data_size;
            }
            packet_count++;
            printf ("got [%4d] packet\n", packet_count);
        }
    }

    if (packet_count != expected_count)
    {
        scenario_failed = WP_TRUE;
    }

}


/******************************************************************************
 * Function name:   App_perform_freerun ()
 * Description  :   Hide the user manual, test the application automatically without
 *                  user interfere and check the result to verify if the application
 *                  works fine.
 *
 * Input  params:   none
 * Output params:   none
 * Return val   :   none
 ******************************************************************************/
void App_perform_freerun (void)
{
    /* Sanity check - send data, receive data */
    /* Send CBFC , send data, expected to receive no data withing the receive interval */
    App_HostSendTraffic ();
    App_CheckReceivedTraffic (NUM_OF_FLOWS);
    printf ("succeed in without pause\n");

    App_HostSendFlowCtrl ();
    App_HostSendTraffic ();
    App_CheckReceivedTraffic (0);   /* No packets should be received within the small interval */
    printf ("succeed in sending pause\n");

    if (scenario_failed == WP_FALSE)
    {
        printf ("\nScenario %d is O.K\n", scenario_number);
    }
    else
    {
        printf ("\nScenario %d Failed\n", scenario_number);
        global_failed = WP_TRUE;
    }

    scenario_number++;

    /* check if the "total" test fail */
    if (global_failed == WP_FALSE)
        printf ("\ntest passed!\n");
    else
        printf ("\ntest failed!\n");

    printf("This is the last test case in this application!\n");
    printf("This test case need further manually test!\n");

    App_Quit (1);
}

/******************************************************************************
 * Function name:   App_CLI ()
 * Description  :   Show up the MENU for user, waiting for user's choice, and then
 *                  act on the user command.
 *
 * Input  params:   none
 * Output params:   none
 * Return val   :   none
 ******************************************************************************/
void App_CLI (void)
{
    WP_U8 cmd;
    WP_U32 i;

    while (1)
    {
        printf ("\n\nMENU\n");
        printf ("=============================\n");
        printf ("1: Device Stats\n");
        printf ("2: Flow Stats\n");
        printf ("3: Routing Port Stats\n");
        printf ("4: Pce filter Stats\n");
        printf ("5: Received Traffic\n");
        printf ("6: Send Pause/CBFC to pause ALL queues\n");
        printf ("7: Send Puase/CBFC to pause the first   Queue\n");
        printf ("8: Send Puase/CBFC to pause the first 2 Queues\n");
        printf ("9: Send Puase/CBFC to pause the fistt 3 Queues\n");
        printf ("k: Quit while driver still running\n");
        printf ("p: Send Traffic\n");
        printf ("r: reboot\n");
        printf ("x: Exit to WinMon\n");
        cmd = getchar ();

        if (cmd == '3')
            WPT_DisplayRoutingPortStats (h_iw_port_gbe_1, "GBE1");

        if (cmd == '1')
        {
            printf ("\nDevice GE1(gbe1) statistics");
            WPT_LocalDisplayDeviceStats (gbe_dev1);

        }
        if (cmd == '2')
        {
            for (i = 0; i < NUM_OF_FLOWS; i++)
                WPT_FlowStatistics (flow_agg_add_header[i]);
        }

        if (cmd == '4')
        {
            printf ("Flow control filter stats\n");
            for (i = 0; i < NUM_FLOW_CTRL_FILTERS; i++)
            {
                App_CheckPceFilterStats (PCE_filter_flow_ctrl[i]);
            }
            printf ("\n\nData filter stats\n");
            App_CheckPceFilterStats (PCE_filter_data[0]);
        }
        if (cmd == '5')
        {
            App_CheckReceivedTraffic (NUM_OF_FLOWS);
        }
        if (cmd == '6')
        {
            user_channel = 0xff;    // pause ALL 8 Qs
            App_HostSendFlowCtrl ();
        }

        if (cmd == '7')
        {
            user_channel = 0x01;    // pause the first Queue
            App_HostSendFlowCtrl ();
        }
        if (cmd == '8')
        {
            user_channel = 0x03;    // pause the first 2 Queues
            App_HostSendFlowCtrl ();
        }
        if (cmd == '9')
        {
            user_channel = 0x07;    // pause the first 3 Queues
            App_HostSendFlowCtrl ();
        }
        if (cmd == 'p')
            App_HostSendTraffic ();
        if (cmd == 'r')
        {
            WT_Reboot ();   // reboot the system
        }

        if (cmd == 'x')
        {
            break;
        }

		// quit while driver still running
        if (cmd == 'k')
        {
            break;
        }
    }
}




#include "wt_util.c"
#include "wt_partition_ecc_util.c"
