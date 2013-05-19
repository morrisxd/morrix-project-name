/*****************************************************************************
 * (C) Copyright PMC - WIN division (Wintegra) 2011.  All rights reserved.
 * PMC-Sierra (WINTEGRA) CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of PMC-Sierra (Wintegra).
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *  Application: Automatic Protection Switch
 *  File Name   : auto_protection_switch_ufe.c
 *  Version     : 1
 *  Date        : 20 Jan 2012
 *  Accompanies : TBD
 *****************************************************************************/

/***************************************************************************
 *  Description:
 *
 * This application contains UFE Datapath configurations of HDLC_OVER_IP.
 * 
 *  Bridging System (bridge1) between ENET ---> HDLC
 *  Switching + Bridge System (bridge2) between HDLC and ENET
 *
 * For detailed implementation description look for the following functions definition and comments 
 *  App_CreateEnetHdlcBridgeForIP();  
 *  App_CreateHdlcEnetBridgeForIP();  
 *
 ************************************************************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "wp_wddi.h"
#include "wp_sim.h"
#include "wpx_oc12_pmc5357.h"

#include "wp_host_io.h"
#include "wpx_app_data.h"
#include "wpx_oc3_pmc5351.h"

#include "wpx_pin_mux_selector.h"

#include "wpx_enet_phy.h"

// #include "auto_protection_switch_ufe.h"
#include "aps_ufe.h"
#include "ufe_utility.h"
#include "flexmux_util.h"
#include "flexmux_util.c"
#include "ufe_utility.c"
// #include "auto_protection_switch_util.c"
#include "aps_ufe_util.c"
// #include "auto_protection_switch_stats.c"
#include "aps_ufe_stats.c"


void App_Quit (WP_U32 status);
static void App_perform_freerun (void);


void id_thread (int i)
{
    static int in = 0;

    in = i;

    printf ("id_thread: new thread started\n");

    while (1)
    {
        WP_Delay (5000000);         /*Check Statistics */
        ;
    }
}

/****************************************************************************************************************************
* Function name: App_EnableAlarms()
* Description  :   Alarms handling are enabled in this function
*
*
* Input  params: none
* Output params: 
* Return val   : none
*****************************************************************************************************************************/
void App_EnableAlarms (void)
{
    int thread_id = 0;
    int thread_params = 999;
    int status = 0;


    thread_id = 0;
    thread_params = 999;
    status = 0;

    WTI_enable_alarms (0);
}

void wait_for_continue (char * msg)
{
    char ch = 'n';

    ch = 'y';

    printf ("\n\n\n\n\n\n\n\n\n\n\n");
    printf ("\n\n\n\n\n\n\n\n\n\n\n");
    printf ("\n\n\n\n\n\n\n\n\n\n\n");
    printf ("\n\n\n\n\n\n\n\n\n\n\n");
    printf ("\n\n\n\n\n\n\n\n\n\n\n");
    printf ("=================================================================\n");
    printf ("=================================================================\n");
    printf ("=================================================================\n");
    printf (" APS(Automatic Protection Switch) Application --- PMC-Sierra\n");
    printf ("=================================================================\n");
    printf ("=================================================================\n");
    printf ("=================================================================\n");
    printf ("\n\n\n");
    do
    {
        if ('y' == ch || 'Y' == ch)
        {
            break;
        }
        printf ("%s:\r", msg);
    }
    while ((ch = getchar ()));
}

void App_UfeRelease (void)
{
    WPX_FRMR_LowLevelCommsDisableAccess(WP_WINPATH(0));
    
    WPX_Ufe412CpldInterruptMaskSet(WP_WINPATH(0), 0x7f);
    
    WUFE_UfeCoreHwInterruptDisable (0);
    
    WUFE_SystemInterruptDisable (0);
    
    WPL_IntDisable(WP_WINPATH(0),WPL_Eint3Ufe4); 
    
#if 0
    WUFE_UfeFramerHwInterruptDisable (0);
    printf ("WUFE_UfeFramerHwInterruptDisable ok\n");
#endif
}

static void App_ResetStatistics (void)
{
    int ii = 0;
    WP_stats_enet s_hs_enet = {
        0
    };
    WP_bport_stats bport_stats = {
        0
    };
    WP_iw_flow_stats flow_stats = {
        0
    };

    WP_DeviceStatisticsReset (h_device_enet1, &s_hs_enet);
    WP_DeviceStatisticsReset (h_device_enet2, &s_hs_enet);
    WP_IwPortStatisticsReset (h_bridge1_bport_enet1, &bport_stats);
    WP_IwPortStatisticsReset (h_bridge2_bport_enet2, &bport_stats);
    WP_IwFlowStatisticsReset (h_flow_agg_control, WP_IW_FLOW_STAT, &flow_stats);
    WP_IwFlowStatisticsReset (h_bridge1_default_agg, WP_IW_FLOW_STAT, &flow_stats);
    WP_IwFlowStatisticsReset (h_bridge2_default_agg, WP_IW_FLOW_STAT, &flow_stats);
    for (ii = 0; ii < APP_MAX_PORT_HDLC; ii ++)
    {
        WP_IwPortStatisticsReset (h_bridge1_bport_hdlc[ii], &bport_stats);
        WP_IwPortStatisticsReset (h_bridge2_bport_hdlc[ii], &bport_stats);
    }
    for (ii = 0; ii < APP_MAX_FLOW_AGG_HDLC; ii ++)
    {
        WP_IwFlowStatisticsReset (h_flow_agg_hdlc[ii], WP_IW_FLOW_STAT, &flow_stats);
        WP_IwFlowStatisticsReset (h_flow_agg_enet2[ii], WP_IW_FLOW_STAT, &flow_stats);
    }
    for (ii = 0; ii < HDLC_NUM; ii ++)
    {
        WP_IwFlowStatisticsReset (h_flow_agg_pppsw_link[ii], WP_IW_FLOW_STAT, &flow_stats);
    }
    printf ("All statistics have been reseted\n");
}

void App_DisableUFESystem (void);
void my_h(WP_U32 wpid, WP_U32 signal_info)
{
    ; // do nothing
}

/****************************************************************************************************************************
* Function name: main()
* Description  : 
*           Main function will call different functions for intialising tags, clocks, creating ports, channels and devices.
*           For testing one user menu is created through which user can select to send packets and see the statistics.
*
* Input  params: none
* Output params:
* Return val   : none
*****************************************************************************************************************************/
int main (int argc, char **argv)
{
	int i = 0;
    WP_status status;
    app_task *task, a_task;
    WP_CHAR comm;

    if (argc < 1)
    {
        printf ("\n_______________________________________________________________________________________________\n");
        printf ("Before running this program, please ensure the working port has been loopbacked already.\n");
        wait_for_continue ("Please loopback the WORKING PORT with fiber and press 'Y' to continue:");
    }


    dp_interface_val = HDLC_OVER_IP;

    /*   Initialize various tags and Context Configurations used in this test  */
    App_InitConfig ();

    /* Create devices and ports */
    App_DeviceAndPortCreate ();

    /*Create memory pool, control register and scheduler */
    App_MemPoolNodeAndSchedCreate ();

    /* After all the necessary resources are created the system is committed by calling WP_SysCommit() */
    status = WP_SysCommit ();   /* Commit system resources */
    App_TerminateOnError (status, "WP_SysCommit()");

    /* Enable emphy port before initialize the framer */
    App_EnableEmphyPort ();

    /* Initialize the framer. Configure and create UFE line and PHY create */
    App_UfeSystem ();

    App_EnableAlarms ();

    /*Create channels and PQBlocks */
    App_ChannelAndPQBlockCreate ();

    /* Sets up ENET-->HDLC interworking */
    App_CreateEnetHdlcBridgeForIP ();

    /* Sets up HDLC-->ENET interworking */
    App_CreateHdlcEnetBridgeForIP ();

    App_EnableUFESystem ();

    /* Enable all sytem, ports, device and channels */
    App_EnableSystem ();

    App_ResetStatistics ();

    if (argc < 1)
    {
        printf ("\n______________________________________________________________________________________\n");
        wait_for_continue ("Please loopback the PROTECTION PORT with fiber and press 'Y' to continue:");
    
        printf ("\n\n\n\n\n\n\n\n\n\n\n");
        printf ("\n\n\n\n\n\n\n\n\n\n\n");
        printf ("\n\n\n\n\n\n\n\n\n\n\n");
        printf ("\n\n\n\n\n\n\n\n\n\n\n");
        printf ("__________________________________________________________________________________________\n");
        printf ("Make sure the PROTECTION PORT is loopbacked by fiber now, then\n");
        printf ("unplug the loopback fiber from the WORKING port now, and\n");
        printf ("send packets with pressing 'p' and check  the result by pressing 'a' in the main MENU:\n");
    }

printf ("argc=(%d)\n", argc);
	for (i = 0; i < argc; i ++)
	{
		printf ("arg(%d)=(%s)\n", i, argv[i]);
	}

    if (argc < 1)
    {
        /* Menu for testing the application */
        while (1)

        {
			printf ("\n\nMENU\n");
			printf ("==============================================\n");
            printf ("Enter choice(press letter, no need return) \n");
            printf ("a. Stats\n");
            printf ("p. Send Packet\n");
            printf ("x. Exit\n");

	do {
            comm = getchar ();
	} while (comm == 0x10 || comm == 0x13);

            switch (comm)
            {
            case 'a':
                App_ShowStats ();
                break;
            case 'p':
                App_DataSend (h_enet1_tx, h_pool_buffer_iw);
                break;
            case 'x':
                App_UfeRelease ();
                WP_DriverRelease();
                exit (0);
            case 'r':
                WT_Reboot ();
                return 0;
            }
            WPI_SimulateInterrupts ();
            while ((task = next_task (irq_task_list, &a_task)) != NULL)
            {
                app_perform_action (task);
            }
        }
    }
    else if ((3 == argc) && (!strcmp (argv[1], "freerun")))
    {
        /* freerun mode */
        printf ("Starting freerun now ...\n");
        App_perform_freerun ();
    }
    else
    {
        printf
            ("Wrong parameter! Only \"go\" or \"go freerun\" supported!");
        /*Quit test */
        App_Quit (0);
    }

    return 0;
}

/****************************************************************************************************************************
* Function name: App_InitConfig()
* Description  : App_InitConfig function initializes Enet Physical ports and create/initializes the clocks and 
*                initializes the system.    
*
*
* Input  params: none
* Output params: 
* Return val   : none
*****************************************************************************************************************************/
void App_InitConfig (void)
{
    WP_status status;
    WP_U32 ii;

    /*  Initialize various tags and Context Configurations used in this test */
    /* NOTE: Tags numbers could be any number but it should be unique. */
    tag_agg_enet = 500;
    tag_iw_sys_bridge1 = 600;
    tag_iw_sys_bridge2 = 601;
    tag_bport_enet1 = 700;
    tag_bport_enet2 = 701;
    for (ii = 0; ii < APP_MAX_PORT_HDLC; ii++)

    {
        tag_bridge1_bport_hdlc[ii] = 802 + ii;
        tag_bridge2_bport_hdlc[ii] = 902 + ii;
    }
    tag_rport_enet2 = 1000;
    for (ii = 0; ii < APP_MAX_PORT_HDLC; ii++)

    {
        tag_agg_hdlc[ii] = 1100 + ii;
        tag_agg_pppsw[ii] = 1200 + ii;
    }
    tag_agg_lcp = 1250;
    tag_agg_control = 1251;
    for (ii = 0; ii < APP_MAX_FLOW_AGG_HDLC; ii++)

    {
        tag_agg_enet2[ii] = 1300 + ii;
    }
    tag_agg_default_bridge1 = 1400;
    tag_agg_default_bridge2 = 1500;
    tag_enet1_rx = 1600;
    tag_enet1_tx = 1700;
    tag_enet2_rx = 1800;
    tag_enet2_tx = 1900;
    for (ii = 0; ii < APP_MAX_CHANNEL; ii++)

    {
        tag_enet2_tx_ch[ii] = 1901 + ii;
    }

    /* Context configuration */
    context_cfg.int_queue_data = &int_queue_table_cfg;
    context_cfg.atm_data = &atm_global_cfg;
    context_cfg.iw_data = &iw_global_cfg;
    context_cfg.pool_limit = 8;
    context_cfg.max_qnodes = 4;
    status = WP_DriverInit ();
    App_TerminateOnError (status, "WP_DriverInit()");
    status = WP_SysInit (APP_MASTER_WPID, &context_cfg);
    App_TerminateOnError (status, "WP_SysInit()");

/*
    status = WPX_BoardConfigure(0, WPX_CONFIGURE_CHECKIN_TDI_16);
    App_TerminateOnError(status, "board configure WPX_CONFIGURE_CHECKIN_TDI_16");
*/
    status = WPX_BoardConfigure (0, WPX_CONFIGURE_2UPI_1XGI_10SGMII_UFE4);
    App_TerminateOnError (status, "WPX_CONFIGURE_2UPI_1XGI_10SGMII_UFE4");

    // FGPA HW reset
    // (NOTE: WPX_Ufe2HwReset function should only be called when using WDS3 !!!
    //  Otherwise, this function should be modified for customer board)
    WPX_Ufe4HwReset (0, WTI_EMPHY_PORT);
    if (WTI_INITIALIZE_FIRMWARE)

    {

        // Announce that UFE firmware will be initialized through host and not through JTAG
        printf
            ("\n******************************************************\n");
        printf
            (" UFE build configuration is loaded through the host!!!\n");
        printf
            ("******************************************************\n\n");
        WT_UfeInitializeFirmware (0);
    }                           // if(WTI_INITIALIZE_FIRMWARE)

    status = WPX_BoardSerdesInit (0, WP_PORT_ENET7, WPX_SERDES_LOOPBACK);   //WPX_SERDES_LOOPBACK  Loopback on Fast ENET
    /*NOTE: disable loopback from device config else loopback will not work */
    App_TerminateOnError (status, "WPX_BoardSerdesInit 7()");
    status = WPX_BoardSerdesInit (0, WP_PORT_ENET8, WPX_SERDES_NORMAL);
    App_TerminateOnError (status, "WPX_BoardSerdesInit 8()");

    status = WP_ModuleInit (0, WP_WDDI_MODULE_POLICER, policer);
    App_TerminateOnError (status, "WP_ModuleInit");

    /* Create BRG1 & BRG3 */
    status =
        WP_SysClockCreate (WP_WINPATH (0), WP_BRG1, WP_BRG_SRC_BRGIN2, 2);
    status =
        WP_SysClockCreate (WP_WINPATH (0), WP_BRG2, WP_BRG_SRC_BRGIN2, 2);
    status =
        WP_SysClockCreate (WP_WINPATH (0), WP_BRG3, WP_BRG_SRC_BRGIN2, 2);
    status =
        WP_SysClockCreate (WP_WINPATH (0), WP_BRG4, WP_BRG_SRC_BRGIN2, 2);
    status =
        WP_SysClockCreate (WP_WINPATH (0), WP_BRG5, WP_BRG_SRC_BRGIN2, 2);
}

/****************************************************************************************************************************
* Function name: App_DeviceAndPortCreate()
* Description  : create the ENET ports and devices
*
* Input  params: none
* Output params: 
* Return val   : none
*****************************************************************************************************************************/
void App_DeviceAndPortCreate ()
{

    /* Create Enet1 port */
    h_port_enet1 =
        WP_PortCreate (APP_MASTER_WPID, WP_PORT_ENET7, &port_enet_cfg);

    App_TerminateOnError (h_port_enet1, "WP_PortCreate() Enet1");

    /* Create device on Enet1 */
    MAC_COPY (device_enet_cfg.mac_addr, wt_mac_enet1);
    h_device_enet1 =
        WP_DeviceCreate (h_port_enet1, WP_PHY (0), WP_DEVICE_ENET,
                         &device_enet_cfg);
    App_TerminateOnError (h_device_enet1, "WP_DeviceCreate() Enet1");

    /* Create Enet2 port */
    h_port_enet2 =
        WP_PortCreate (APP_MASTER_WPID, WP_PORT_ENET8, &port_enet_cfg);

    App_TerminateOnError (h_port_enet2, "WP_PortCreate() Enet2");

    /* Create device on Enet2 */
    /* To increase the number of channels in ENET 2, modify here */
    /* Note : pkt_limits is set to 8 in the port configuration      */
    device_enet_cfg.max_tx_channels = 5;
    MAC_COPY (device_enet_cfg.mac_addr, wt_mac_enet2);
    h_device_enet2 =
        WP_DeviceCreate (h_port_enet2, WP_PHY (0), WP_DEVICE_ENET,
                         &device_enet_cfg);
    App_TerminateOnError (h_device_enet2, "WP_DeviceCreate() Enet2");

    /* Create emphy port */
    emphy_port =
        WP_PortCreate (WP_WINPATH (0), WTI_EMPHY_PORT,
                       &upi_emphy_port_config);
    App_TerminateOnError (emphy_port, "WP_PortCreate() emphy_port");

    /* Create an IW Host port */
    h_port_iwhost = WP_PortCreate (WP_WINPATH (0), WP_PORT_IW_HOST, NULL);
    App_TerminateOnError (h_port_iwhost, "WP_PortCreate() IW Host");

    /* Create an IW Host Device */
    h_dev_iwhost =
        WP_DeviceCreate (h_port_iwhost, 0, WP_DEVICE_IW_HOST, NULL);
    App_TerminateOnError (h_dev_iwhost, "WP_DeviceCreate() IW Host");
}

/****************************************************************************************************************************
* Function name: App_MemPoolNodeAndSchedCreate()
* Description  : Creates memory pool and scheduler system
*
* Input  params: none
* Output params: 
* Return val   : none
*****************************************************************************************************************************/
void App_MemPoolNodeAndSchedCreate ()
{
    WP_status status;

    /* Create Buffer pools */
    h_pool_buffer_iw =
        WP_PoolCreate (APP_MASTER_WPID, WP_pool_iwbuffer,
                       &pool_buffer_data_iw_cfg);
    App_TerminateOnError (h_pool_buffer_iw, "WP_PoolCreate iwbuffer iw");

    /* Pool for host termination buffers */
    h_pool_256 =
        WP_PoolCreate (WP_WINPATH (0), WP_pool_buffer,
                       &pool_buffer_data_256_cfg);
    App_TerminateOnError (h_pool_256, "PoolCreate() pool_256");
    h_pool_ring_host =
        WP_PoolCreate (WP_WINPATH (0), WP_pool_ring, &pool_ring_data_cfg);
    App_TerminateOnError (h_pool_ring_host, "PoolCreate() pool_ring_host");

    /* Create an interworking queue node */
    qnode_iwq_cfg.adjunct_pool = h_pool_buffer_iw;
    h_qnode_iwq =
        WP_QNodeCreate (APP_MASTER_WPID,
                        WP_QNODE_IWQ | WP_QNODE_OPT_DEDICATED_RX_HWQ |
                        WP_QNODE_OPT_FMU, &qnode_iwq_cfg);
    App_TerminateOnError (h_qnode_iwq, "WP_QNodeCreate() h_qnode_iwq");

    /* Create an interworking queue node */
    qnode_iwq_cfg.adjunct_pool = h_pool_buffer_iw;
    h_qnode_iwq_hdlc =
        WP_QNodeCreate (APP_MASTER_WPID, WP_QNODE_IWQ, &qnode_iwq_cfg);
    App_TerminateOnError (h_qnode_iwq_hdlc,
                          "WP_QNodeCreate() h_qnode_iwq_hdlc");

    /* Host termination qnode */
    qnode_hostq_cfg.pool_buffer = h_pool_256;
    qnode_hostq_cfg.pool_ring = h_pool_ring_host;
    h_qnode_host =
        WP_QNodeCreate (WP_WINPATH (0), WP_QNODE_HOSTQ, &qnode_hostq_cfg);
    App_TerminateOnError (h_qnode_host, "QnodeCreate() h_qnode_mp");
    status =
        WP_ControlRegister (WP_EVENT_RX_INDICATE, App_EventRxIndicate);
    App_TerminateOnError (status, "WP_ControlRegister()");
    status =
        WP_ControlRegister (WP_EVENT_TX_INDICATE, App_EventTxIndicate);
    App_TerminateOnError (status, "WP_ControlRegister()");
    status =
        WP_ControlRegister (WP_EVENT_STATUS_INDICATE,
                            App_EventErrorIndicate);
    App_TerminateOnError (status, "WP_ControlRegister()");
    status = WP_SysSchedulerCreate (WP_WINPATH (0), cw_config);
    App_TerminateOnError (status, "WP_SysSchedulerCreate()");
}

/****************************************************************************************************************************
* Function name: App_ChannelAndPQBlockCreate()
* Description  : creates host, ENET, and UFE(hdlc) channels. PQblock and associated ENET 2 channels are created in this function
*
*
* Input  params: none
* Output params: 
* Return val   : none
*****************************************************************************************************************************/
void App_ChannelAndPQBlockCreate (void)
{
    WP_U32 ii;
    WP_U32 block_index;
    WP_U32 tag1, tag2;
    WP_status status;

    /* Create ENET 1 channels */
    h_enet1_rx =
        WP_ChannelCreate (tag_enet1_rx, h_device_enet1, h_qnode_iwq,
                          WP_CH_RX, WP_ENET, &ch_enet_cfg);
    App_TerminateOnError (h_enet1_rx, "WP_ChannelCreate() Enet 1 Rx");

    /* NOTE this channel is in NON-IW mode, used to send out a packet from the host out of ENET1 */
    ch_enet_cfg.iwmode = WP_PKTCH_IWM_DISABLE;
    h_enet1_tx =
        WP_ChannelCreate (tag_enet1_tx++, h_device_enet1, h_qnode_iwq,
                          WP_CH_TX, WP_ENET, &ch_enet_cfg);
    App_TerminateOnError (h_enet1_tx, "WP_ChannelCreate() Enet 1 Tx");
    tx_binding_cfg.dci_mode = WP_IW_DYN_CH_INSERT_ENABLE;
    status =
        WP_IwTxBindingCreate (h_enet1_tx, WP_IW_TX_BINDING,
                              &tx_binding_cfg);
    App_TerminateOnError (status, "WP_IwTxBindingCreate() h_enet1_tx");

    /* Create ENET 2 channels */
    ch_enet_cfg.iwmode = WP_PKTCH_IWM_DISABLE;
    h_enet2_rx =
        WP_ChannelCreate (tag_enet2_rx, h_device_enet2, h_qnode_iwq,
                          WP_CH_RX, WP_ENET, &ch_enet_cfg);
    App_TerminateOnError (h_enet2_rx, "WP_ChannelCreate() Enet 2 Rx");
    ch_enet_cfg.iwmode = WP_PKTCH_IWM_ENABLE;
    ch_enet_cfg.tx_tq = HDLC_NUM;    // only use a size, not really used
    h_enet2_tx =
        WP_ChannelCreate (tag_enet2_tx, h_device_enet2, h_qnode_iwq,
                          WP_CH_TX, WP_ENET, &ch_enet_cfg);
    App_TerminateOnError (h_enet2_tx, "WP_ChannelCreate() Enet 2 Tx");
    tx_binding_cfg.dci_mode = WP_IW_DYN_CH_INSERT_ENABLE;
    status =
        WP_IwTxBindingCreate (h_enet2_tx, WP_IW_TX_BINDING,
                              &tx_binding_cfg);
    App_TerminateOnError (h_enet2_tx, "WP_IwTxBindingCreate() h_enet2_tx");

    /*  4 channels create over the enet2 device */
    for (ii = 0; ii < APP_MAX_CHANNEL; ii++)
    {
        ch_enet_cfg.tx_tq = ii; // 4 is already used by h_enet2_tx
        h_enet2_tx_ch[ii] =
            WP_ChannelCreate (tag_enet2_tx_ch[ii], h_device_enet2,
                              h_qnode_iwq, WP_CH_TX, WP_ENET,
                              &ch_enet_cfg);
        App_TerminateOnError (h_enet2_tx_ch[ii],
                              "WP_ChannelCreate() Enet Tx with QoS");
        tx_binding_cfg.dci_mode = WP_IW_DYN_CH_INSERT_ENABLE;
        status =
            WP_IwTxBindingCreate (h_enet2_tx_ch[ii], WP_IW_TX_BINDING,
                                  &tx_binding_cfg);
        App_TerminateOnError (h_enet2_tx_ch[ii], "WP_IwTxBindingCreate()");
    }

    /* Create HDLC devices */
    for (ii = 0; ii < /* 4 */ APP_MAX_DEV_HDLC; ii++)

    {
        h_dev_hdlc[ii] =
            WP_DeviceCreate (emphy_port,
                             WP_PHY (WUFE_PHY_INDEX_GET
                                     (ufe4_app_system.phy_handle[ii])),
                             WP_DEVICE_HDLC, &device_emphy_hdlc_cfg[0]);
        App_TerminateOnError (h_dev_hdlc[ii], "WP_DeviceCreate() hdlc");
    }
    tag1 = tag_hdlcrx;    // 200
    tag2 = tag_hdlctx;    // 300

    /* Create HDLC RX and TX Channels */
    for (ii = 0; ii < /* 4 */ APP_MAX_CHANNEL; ii++)

    {
        ch_hdlc_cfg.iwmode = WP_PKTCH_IWM_ENABLE;
        ch_hdlc_cfg.intmode = WP_PKTCH_INT_ENABLE;
        h_hdlc_rx[ii] =
            WP_ChannelCreate (tag1++, h_dev_hdlc[ii], h_qnode_host,
                              WP_CH_RX, WP_HDLC, &ch_hdlc_cfg);
        App_TerminateOnError (h_hdlc_rx[ii],
                              "WP_ChannelCreate() h_hdlc_rx");
    }

    for (block_index = 0; block_index < HDLC_NUM; block_index++)
    {
        ch_hdlc_cfg.iwmode = WP_PKTCH_IWM_ENABLE;

        h_hdlc_tx[/* 4 */ block_index] =
                WP_ChannelCreate (0, h_dev_hdlc[block_index],
                                  h_qnode_iwq_hdlc, WP_CH_TX, WP_HDLC,
                                  &ch_hdlc_cfg);
        App_TerminateOnError (h_hdlc_tx[block_index],
                                  "WP_ChannelCreate()  Tx");

        /* tx binding for hdlc channel */
        status =
                WP_IwTxBindingCreate (h_hdlc_tx[block_index],
                                      WP_IW_TX_BINDING, &tx_binding_cfg);
        App_TerminateOnError (status, "WP_IwTxBindingCreate()");
    }

    /* Crate IW HOST termination channels one for each bridge */
    tx_binding_cfg.dci_mode = WP_IW_DYN_CH_INSERT_DISABLE;
    ch_iw_rx_cfg.tx_binding_config = &tx_binding_cfg;
    h_bridge1_iwhost =
        WP_ChannelCreate (90, h_dev_iwhost, h_qnode_iwq, WP_CH_RX,
                          WP_IW_HOST, &ch_iw_rx_cfg);
    App_TerminateOnError (h_bridge1_iwhost, "WP_ChannelCreate() Host atm");
    h_bridge2_iwhost =
        WP_ChannelCreate (91, h_dev_iwhost, h_qnode_iwq, WP_CH_RX,
                          WP_IW_HOST, &ch_iw_rx_cfg);
    App_TerminateOnError (h_bridge2_iwhost, "WP_ChannelCreate() Host atm");
}


void App_SetPortRate (void)
{
    status = WPX_FRMR_SONET_SDH_PORT_SetRate(0, 0/*flexmux_id*/, 0/*line_port_id*/, WPX_UFE_FRAMER_LINE_PORT_RATE_OC12);
    WT_UfeTerminateOnError (status, "WPX_FRMR_SONET_SDH_PORT_SetRate", 0, __LINE__);
}



/****************************************************************************************************************************
 * Function name: App_ufeSystem()
 * Description  : Initialize UFE and framer. create line and PHY interface for UFE. Create and enable the UFE system

 * Input  params: none
 * Output params: 
 * Return val   : none
 *****************************************************************************************************************************/
void App_UfeSystem (void)
{
    WUFE_status ufe_status;
    WP_U32 property, i, num_of_lines;
    WTI_Flexmux_global_setup flex_global_params;
    WUFE_line_transf_type transfer_type;
    WUFE_un_framed framing_mode;
    WUFE_cas_enable cas_mode;
    WT_ufe_line_sdh_params ufe_line_params;
    WTI_flexmux_connection_type conn_type;
    WP_U32 client_port, line_port;
    WUFE_line_clock_rec_params line_cr_params;
    WUFE_init_config ufe4_config;

    num_of_lines = WT_MAX_LINE_INDEX;
    transfer_type = WUFE_SDH_TYPE_T1;
    framing_mode = WUFE_UNFRAMED;
    cas_mode = WUFE_CAS_DISABLE;

    /* Initialize the UFE4 */
    memset (&ufe4_config, 0, sizeof (WUFE_init_config));
    ufe4_config.ufe_id = 0;

    /* Host-Load UFE FW */
    status = WUFE_UfeInitializeFirmware (&ufe4_config);
    App_TerminateOnError (status, "WUFE_Ufe4InitializeFirmware");
    status = WUFE_UfeInit (&ufe4_config);
    App_TerminateOnError (status, "WUFE_Ufe4Init");

    /* Define UFE System */
    ufe4_system_cfg.clock_rec_sys_params = 0;

    /* Initialize the UFE4 system (system id = 0)  */
    ufe_status = WUFE_SystemConfig (0, &ufe4_system_cfg);
    WT_UfeTerminateOnError (ufe_status, "WUFE_SystemConfig", 0, __LINE__);


    // Initialize the framer:
    // calls:
    // 1. WPX_UFE_FRAMER_FlexmuxInit()
    // 2. WPX_UFE_FRAMER_FlexmuxSetBuildPersonality()
    // 3. WPX_FRMR_DEV_SetMode()
    // 4. WPX_FRMR_SONET_SDH_CreateFacility()
    WTI_FlexmuxInit (WTI_EMPHY_PORT, 0, 
                        WT_FLEXMUX_UFE_412_1_PORT_OC12, 
                        WPX_UFE_FRAMER_DEVICE_MODE_SDH, 
                        WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC4,   //WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC3,
                         &flex_global_params);

    // Configure and create UFE lines for T1 SDH mode
    for (i = WT_FIRST_LINE_INDEX;
         i < (num_of_lines + WT_FIRST_LINE_INDEX); ++i)

    {
        /* UFE line configuration, creation and enabling */
        /* Create Framer Connection */
        status =
            WUFE_LineCreate (&ufe4_app_system.line_handle[i],
                             ufe4_app_system.ufeid,
                             WUFE_STRUCT_LINE_SDH, &line_cfg);
        WT_UfeTerminateOnError (status, "WUFE_LineCreate", i, __LINE__);
        if (line_cfg->transfer_type == WUFE_SDH_TYPE_E1)
            client_port = i + (i / 3);

        else
            client_port = i;
        line_port = WTI_FlexmuxLinePortIndexGet (i, 0);
        memset (&ufe_line_params, 0, sizeof (WT_ufe_line_sdh_params));
        ufe_line_params.transfer_type = line_cfg->transfer_type;
        ufe_line_params.stm4 = line_cfg->stm4;
        ufe_line_params.stm1 = line_cfg->stm1;
        ufe_line_params.stm0 = line_cfg->stm0;
        ufe_line_params.tug2 = line_cfg->tug2;
        ufe_line_params.tu = line_cfg->tu;
        ufe_line_params.clock_rec_line_params = &line_cr_params;
        ufe_line_params.clock_rec_line_params->tx_clk_rec_enable = 0;
        if (line_cfg->transfer_type == WUFE_SDH_TYPE_T1)
            conn_type = WT_FLEXMUX_CONNECTION_TYPE_T1_FRAMED;

        else
            conn_type = WT_FLEXMUX_CONNECTION_TYPE_E1_FRAMED;

        /* Create Framer Connection */
        WTI_FlexmuxConnectionCreate (0, line_port, client_port,
                                     conn_type, &ufe_line_params);

        // UFE line is configured to work NOT in loopback mode (WUFE_LINE_LOOPBACK_TYPE_NONE)
        // In case UFE loopback is wanted, one should change the define WTI_SOCKET_LOOPBACK_MODE
        // to be: WUFE_LINE_LOOPBACK_TYPE_LINE (in case of loopback from line to line),
        // or to: WUFE_LINE_LOOPBACK_TYPE_SYSTEM (in case of loopback from UFE to UFE)
        ufe_status =
            WUFE_LineLoopbackConfigure (ufe4_app_system.line_handle[i],
                                        WTI_SOCKET_LOOPBACK_MODE);
        WT_UfeTerminateOnError (ufe_status,
                                "WUFE_LineLoopbackConfigure", 0, __LINE__);
        ufe_status =
            WUFE_PhyCreate (&ufe4_app_system.phy_handle[i],
                            ufe4_app_system.line_handle[i],
                            &ufe4_phy_cfg, WUFE_FULL_DUPLEX);
        WT_UfeTerminateOnError (ufe_status, "WUFE_PhyCreate", i, __LINE__);
        printf ("UFE link(%3d) of %d created\r", i, num_of_lines);

        /* Increment the Tributary Unit */
        ++line_cfg->tu;
        if ((line_cfg->tu % NUM_TU) == 0)

        {
            line_cfg->tu = 0;
            ++line_cfg->tug2;
            if ((line_cfg->tug2 % 7) == 0)
            {
                line_cfg->tug2 = 0;
                ++line_cfg->stm0;
                if ((line_cfg->stm0 % 3) == 0)
                {
                    line_cfg->stm0 = 0;
                    ++line_cfg->stm1;
                }
            }
        }
    }
    property = WUFE_SYS_EMPHY;
    ufe_status =
        WUFE_SystemEnable (ufe4_app_system.ufeid, property,
                           WUFE_FULL_DUPLEX);
    WT_UfeTerminateOnError (ufe_status, "WUFE_SystemEnable ", 0, __LINE__);
}

/****************************************************************************************************************************
 * Function name: App_enableEmphyPort()
 * Description  : Enable the EMPHY port
 *
 * Input  params: none
 * Output params: 
 * Return val   : none
 *****************************************************************************************************************************/
void App_EnableEmphyPort (void)
{
    /* Enable the EMPHY port */
    status = WP_PortEnable (emphy_port, WP_DIRECTION_DUPLEX);
    App_TerminateOnError (status, "WP_PortEnable() emphy_port");
}

void App_DisableUFESystem (void)
{
    WP_U32 i;
    WUFE_status ufe_status;

    for (i = 0; i < WT_MAX_LINE_INDEX; i++)
    {
        ufe_status =
            WUFE_PhyDisable (ufe4_app_system.phy_handle[i],
                            WUFE_FULL_DUPLEX);
        WT_UfeTerminateOnError (ufe_status, "WUFE_PhyEnable() UFE4", 0,
                                __LINE__);

        ufe_status =
            WUFE_LineDisable (ufe4_app_system.line_handle[i],
                             WUFE_FULL_DUPLEX);
        WT_UfeTerminateOnError (ufe_status, "SDH WUFE_LineEnable", i,
                                __LINE__);
    }
}
/****************************************************************************************************************************
 * Function name: App_EnableUFESystem()
 * Description  : Enables UFE PHY and line interface

 * Input  params: none
 * Output params: 
 * Return val   : none
 *****************************************************************************************************************************/
void App_EnableUFESystem (void)
{
    WP_U32 i;
    WUFE_status ufe_status;

    for (i = 0; i < WT_MAX_LINE_INDEX; i++)

    {
        ufe_status =
            WUFE_LineEnable (ufe4_app_system.line_handle[i],
                             WUFE_FULL_DUPLEX);
        WT_UfeTerminateOnError (ufe_status, "SDH WUFE_LineEnable", i,
                                __LINE__);
        ufe_status =
            WUFE_PhyEnable (ufe4_app_system.phy_handle[i],
                            WUFE_FULL_DUPLEX);
        WT_UfeTerminateOnError (ufe_status, "WUFE_PhyEnable() UFE4", 0,
                                __LINE__);
    }
}

/****************************************************************************************************************************
* Function name: App_EnableSystem()
* Description  :   Enet, HDLC ports  and channels are enabled in this function
*
*
* Input  params: none
* Output params: 
* Return val   : none
*****************************************************************************************************************************/
void App_EnableSystem (void)
{
    WP_status status;
    WP_U32 ii, block_index;

    status = WP_SysPsuEnable (WP_WINPATH (0));
    App_TerminateOnError (status, "WP_SysPsuEnable()");

    /* Enable all ports and devices */
    status = WP_PortEnable (h_port_enet1, WP_DIRECTION_DUPLEX);
    App_TerminateOnError (status, "WP_PortEnable() Enet1");
    status = WP_DeviceEnable (h_device_enet1, WP_DIRECTION_DUPLEX);
    App_TerminateOnError (status, "WP_DeviceEnable() Enet1");
    status = WP_PortEnable (h_port_enet2, WP_DIRECTION_DUPLEX);
    App_TerminateOnError (status, "WP_PortEnable() Enet2");
    status = WP_DeviceEnable (h_device_enet2, WP_DIRECTION_DUPLEX);
    App_TerminateOnError (status, "WP_DeviceEnable() Enet2");
    for (ii = 0; ii < APP_MAX_DEV_HDLC; ii++)

    {
        status = WP_DeviceEnable (h_dev_hdlc[ii], WP_DIRECTION_DUPLEX);
        App_TerminateOnError (status, "WP_DeviceEnable()");
    }
    status = WP_ChannelEnable (h_enet1_rx);
    App_TerminateOnError (status, "WP_ChannelEnable ENET1 RX");
    status = WP_ChannelEnable (h_enet1_tx);
    App_TerminateOnError (status, "WP_ChannelEnable ENET1 TX");
    status = WP_ChannelEnable (h_enet2_rx);
    App_TerminateOnError (status, "WP_ChannelEnable ENET2 RX");
    status = WP_ChannelEnable (h_enet2_tx);
    App_TerminateOnError (status, "WP_ChannelEnable ENET2 TX");
    for (block_index = 0; block_index < HDLC_NUM; block_index++)
    {
        status = WP_ChannelEnable (h_hdlc_tx[block_index]);
        App_TerminateOnError (status, "WP_ChannelEnable HDLC TX");
    }

    /* Enable the Enet2 channels used for  traffic flow  */
    for (ii = 0; ii < APP_MAX_CHANNEL; ii++)

    {
        status = WP_ChannelEnable (h_enet2_tx_ch[ii]);
        App_TerminateOnError (status,
                              "WP_ChannelEnable ENET2 TX for channel");
    }
}

/****************************************************************************************************************************
* Function name: App_CreateEnetHdlcBridgeForIP()
* Description  : Create interworking system bridge1 (h_iw_sys_bridge1) for traffic HDLC OVER IP
*
*
*    Bridge1: 
*    Host sends a packet on ENET1 (port is in internal loopback mode)
*    The packet is received into Bridge1. A DFC classifier rule based on DEST_MAC, VLAN and DSCP is applied and
*    forwards the packet to one of the HDLC ports.
*    In this example HDLC[ii] is taken. HDLC[ ii] was picked based on FOR LOOP index, it could have been any HDLC.
*    For this flow priority mapping, priority remarking and policing is done. Priority remarking is based on DSCP.
*
*    HDLC is used based on for loop index. It could have been any HDLC.
*
*    Note that the flow aggregation that forwards the packet on HDLC ( through PQ Block) strips off the Ethernet
*    header and prefixes a 4 byte HDLC header to the packet before sending it out on the HDLC (hdlc_prefix_header)
*    If you want to change the hdlc_prefix_header, you will have to recompile the test. Here choice of two header configuration is given
*
*
* Input  params: none
* Output params: 
* Return val   : none
*****************************************************************************************************************************/
void App_CreateEnetHdlcBridgeForIP ()
{
    WP_U32 ii = 0, jj = 0;
    WP_status status;
    WP_dfc_flow_info dfc_flow_info;
    WP_flow_class_rule flow_class_rule;
    WP_CHAR rule_string[128];
    WP_classification_filter classification_filter = {
        0
    };
    WP_flow_class_forwarding_action forwarding_action;
    WP_iw_agg_bridging iw_agg_bridging_cfg;
    WP_U8 hdlc_prefix_header[4] = {
        0
    };
    WP_flow_class_priority_action priority_action;
    WP_flow_class_ext_action action;
    WP_U8 action_type[3];
    WP_flow_class_action_info action_info[3];

    /* Bridge1 ENET --> HDLC */
    iw_sys_bridge.tag = tag_iw_sys_bridge1;
    h_iw_sys_bridge1 =
        WP_IwSystemCreate (APP_MASTER_WPID,
                           WP_IW_VLAN_AWARE_BRIDGING_MODE, &iw_sys_bridge);
    App_TerminateOnError (h_iw_sys_bridge1,
                          " WP_IwSystemCreate() Bridge1");

    /* DFC filter using 2 fields */
    classification_filter.fields_mask = WP_FIELD_IDS_ARRAY;
    classification_filter.no_match_action = WP_CLASS_FILTER_NO_MATCH_DENY;

    /* Filter based on DST_MAC,VLAN_ID and DSCP */
    classification_filter.field_ids_array[0] = WP_FIELD_ID_DST_MAC;
    classification_filter.field_ids_array[1] = WP_FIELD_ID_VLAN_ID;
    classification_filter.field_ids_array[2] = WP_FIELD_ID_IPV4_DSCP;
    classification_filter.field_ids_array[3] = WP_FIELD_ID_LAST;
    classification_filter.filter_type = WP_CLASS_FILTER_EXT_ACTION_EMC;
    h_bridge1_filter1 =
        WP_IwClassFilterAdd (h_iw_sys_bridge1, &classification_filter);
    App_TerminateOnError (h_bridge1_filter1,
                          " WP_IwClassFilterAdd() Bridge1");
    memcpy (&iw_agg_bridging_cfg, &default_iw_agg_bridging_cfg,
            sizeof (WP_iw_agg_bridging));

    /* Create Default Aggregation Bridge1 */
    iw_agg_bridging_cfg.tag = tag_agg_default_bridge1;
    iw_agg_bridging_cfg.txfunc = h_bridge1_iwhost;    // pointing to the host channel, morris
    iw_agg_bridging_cfg.input_bport = 0;
    iw_agg_bridging_cfg.intmode = WP_IW_INT_ENABLE;
    h_bridge1_default_agg =
        WP_IwFlowAggregationCreate (APP_MASTER_WPID,
                                    WP_IW_VLAN_AWARE_BRIDGING_MODE,
                                    &iw_agg_bridging_cfg);
    App_TerminateOnError (h_bridge1_default_agg,
                          " WP_IwFlowAggregationCreate() Default Bridge");
    iw_agg_bridging_cfg.intmode = WP_IW_INT_DISABLE;
    bridge_port_cfg.tag = tag_bport_enet1;
    bridge_port_cfg.flow_agg = h_bridge1_default_agg;    // key, morris
    bridge_port_cfg.input_filters_mask = 0x80000000;
    h_bridge1_bport_enet1 =
        WP_IwBridgingPortCreate (h_iw_sys_bridge1, &bridge_port_cfg);
    App_TerminateOnError (h_bridge1_bport_enet1,
                          "WP_IwBridgingPortCreate()ENET1");

    /* Rx binding ENET 1 */
    rx_binding_bridging_cfg.input_port = h_bridge1_bport_enet1;
    status =
        WP_IwRxBindingCreate (h_enet1_rx, h_iw_sys_bridge1,
                              h_qnode_iwq, &rx_binding_bridging_cfg);
    App_TerminateOnError (status, " WP_IwRxBindingCreate() enet1 ");
/*------------------------------------------------------------------------------*\
end of the above, morris
\*------------------------------------------------------------------------------*/

    /* EACH HDLC port is represented as bridge port in Bridge1 too so a packet going from ENET to HDLC has the
     * ENET bridge port as input bridge port and HDLC bridge port as the output bridge port */
    for (ii = 0; ii < /* 4 */ APP_MAX_PORT_HDLC; ii++)

    {
        bridge_port_cfg.tag = tag_bridge1_bport_hdlc[ii];
        bridge_port_cfg.flow_agg = h_bridge1_default_agg;
        bridge_port_cfg.max_mac_addresses = APP_MAX_MAC_ADDRESS;
        bridge_port_cfg.input_filters_mask = 0;
        bridge_port_cfg.classification_flag =
            WP_IW_IWPORT_CLASSIFICATION_DISABLED;
        /*
          * now we create 4 same bridging port, morris
          */
        h_bridge1_bport_hdlc[ii] =
            WP_IwBridgingPortCreate (h_iw_sys_bridge1, &bridge_port_cfg);
        App_TerminateOnError (h_bridge1_bport_hdlc[ii],
                              "WP_IwBridgingPortCreate()HDLC");
    }

    /* Here Giving choice of configuring HDLC Header */
    /* For adding different header, modify here and compile the code */
    config_choice = TRAFFIC;    // we set fixed value of '1' here, morris

    {
        hdlc_prefix_header[0] = 0x0f;
        hdlc_prefix_header[1] = 0x00;
        hdlc_prefix_header[2] = 0x08;
        hdlc_prefix_header[3] = 0x00;
    }

    /* Create flow aggregations that send the packet out on each HDLC */
    /* NOTE the 18 bytes being extracted */
    /* NOTE THE 4 BYTE HDLC header in inserted */
    /*FA and PQBlock mapping is one to one... */
    for (ii = 0; ii < HDLC_NUM; ii++)
    {
        iw_agg_bridging_cfg.tag = tag_agg_hdlc[ii];
        iw_agg_bridging_cfg.txfunc = h_hdlc_tx[ii];    /*PQ Block forward the packet to associated HDLC channel */
        iw_agg_bridging_cfg.iw_port = h_bridge1_bport_hdlc[ii];
        iw_agg_bridging_cfg.extraction_length = 18;
        iw_agg_bridging_cfg.prefix_length = 4;
        iw_agg_bridging_cfg.l2_header_insert_mode =
            WP_IW_L2H_INSERT_ENABLE;
        memcpy (iw_agg_bridging_cfg.prefix_header, hdlc_prefix_header, 4);
        h_flow_agg_hdlc[ii] =
            WP_IwFlowAggregationCreate (APP_MASTER_WPID,
                                        WP_IW_VLAN_AWARE_BRIDGING_MODE,
                                        &iw_agg_bridging_cfg);
        App_TerminateOnError (h_flow_agg_hdlc[ii],
                              " WP_IwFlowAggregationCreate() HDLC ");
    }
    iw_agg_bridging_cfg.prefix_length = 0;
    iw_agg_bridging_cfg.l2_header_insert_mode = WP_IW_L2H_INSERT_DISABLE;
    iw_agg_bridging_cfg.extraction_length = 0;
    memset (&flow_class_rule, 0, sizeof (WP_flow_class_rule));
    memset (&dfc_flow_info, 0, sizeof (WP_dfc_flow_info));

    /* Add DFC flow */
    dfc_flow_info.filter_handle = h_bridge1_filter1;
    dfc_flow_info.input_port = h_bridge1_bport_enet1;
    dfc_flow_info.output_port = 0;
    policer_handle =
        WP_PolicerCreate (WP_WINPATH (0) /*wpid */ , WP_POLICER_V1,
                          policer_config);
    App_TerminateOnError (policer_handle,
                          " WP_IwPolicerCreate() bridge1 ");

    /*Creating (HDLC_NUM*APP_MAX_HDLC_PORTS) DFC flow rule. Rule DST MAC and VLAN is same but DSCP value is, FOR loop index   */
    /*All rule can be configured separately and accordingly Flow agg. can be chosen */
    for (ii = 0; ii < HDLC_NUM; ii++)
    {
        for (jj = 0; jj < APP_MAX_HDLC_PORTS; jj++)

        {
            /*here DSCP value is FOR LOOP index. it can be configured as different value. as per requirement */
            sprintf (rule_string,
                     "n;n;n;n;n;n;n;n;n;0x00.00.22.33.12.06;10;n;n;n;n;n;n;n;n;n;n;n;n;n;n;n;n;n;n;n;n;n;n;n;n;n;n;n;n;n;n;n;n;n;0x%x;n",
                     ii + jj);
            priority_action.priority_value = jj;
            priority_action.field_type = WP_FLOW_PRIORITY_DSCP;
            priority_action.action_mode =
                WP_FLOW_PRIORITY_MAPPING | WP_FLOW_PRIORITY_REMARKING;
            flow_class_rule.match_action = WP_FLOW_CLASS_MODE_EXT_ACTION;
            action_type[0] = WP_FLOW_CLASS_MODE_PRIORITY;
            action_type[1] = WP_FLOW_CLASS_MODE_FLOW_AGG;
            action_type[2] = WP_FLOW_CLASS_MODE_POLICER;
            action.num_of_actions = 3;
            action.action_type = action_type;
            forwarding_action.flow_aggregation = h_flow_agg_hdlc[ii];   /* Note here HDLC FA is chosen according to FOR LOOP index */

            /*It can be configured to different FA as per requirement */
            action_info[0].priority_action = priority_action;
            action_info[1].forwarding_action = forwarding_action;
            action_info[2].policer_action.policer = policer_handle;
            action.action_info = action_info;
            flow_class_rule.action_info.ext_action = action;
            flow_class_rule.classifier_string = rule_string;
            flow_class_rule.dfc_info = &dfc_flow_info;
            h_bridge1_dfcflow1[ii][jj] =
                WP_IwFlowAdd (h_iw_sys_bridge1, WP_FLOW_CLASS_RULE,
                              &flow_class_rule);
            App_TerminateOnError (h_bridge1_dfcflow1[ii][jj],
                                  " WP_IwFlowAdd() DFC Bridge1 rule1 ");
        }
    }
    status = WP_IwSystemBuild (h_iw_sys_bridge1);
    App_TerminateOnError (status, " WP_IwSystemBuild() Bridge1");
}

/****************************************************************************************************************************
* Function name: App_CreateHdlcEnetBridgeForIP()
* Description  :  This function create two interworking system PPP Switch(h_iw_sys_pppsw) and Bridge2(h_iw_sys_bridge2)
*                 for traffic HDLC OVER IP
*
*    PPP Switch:
*    NOTE: Address and Protocol Field compression are enabled on the PPP SWITCHING RX BINDING. So a packet that starts with the
*    PPP header will have AFC/PFC applied to it.
*    Any packet that does not have a PPP header (HDLC packet) will assumed to have it compressed and will be accepted as it is.
*    This is the only way to accept HDLC packets into the PPP Switching since they don't have a PPP header (0xFF03)
*
*    So 0x0F008035 is received back into the PPP Switching system as 0x0F008035 since no PPP header is present (0xFF03)
*    So 0x0F000800 is received back into the PPP Switching system as 0x0F000800 since no PPP header is present (0xFF03)
*
*    The PPP SWITCHING default flow aggregation (1 per HDLC port) adds a dummy 18 byte ethernet header( including VLAN) and
*    forwards the packet to Bridge2.
*     Dummy ethernet header is configured VLAN id as follows
*           0x0a for traffic
*           0x0b for control 
*    NOTE: VLAN id should add properly based on which bridge2 filter will forward the packet.
*
*    Bridge2:
*
*    The one filter (Filter0) is used for recognizing VLAN id of dummy ethernet header..
*
*    Two DFC rules are added for Filter0: 
*    (1) If VLAN id 0x0b. This packet is sent to a flow agg named h_flow_agg_control and host terminated
*    (2) If VLAN id 0x0a This packet is sent to a flow agg named h_flow_agg_enet2 and sent out of ENET2 
*        For this flow priority mapping priority remarking and policing is done. Priority remarking is based on VLAN.
*
*   flow agg h_flow_agg_control will removed dummy ethernet mac header and send rest of the packet
*   flow agg h_flow_agg_enet2  will remove dummy ethernet header and hdlc header,  add ethernet header , send packet
*     
*     Since ENET2 is in loopback mode, all packets sent out will be received back on ENET2 RX channel
*
* Input  params: none
* Output params: 
* Return val   : none
*****************************************************************************************************************************/
void App_CreateHdlcEnetBridgeForIP (void)
{
    WP_U32 ii = 0, jj = 0;
    WP_status status;
    WP_U8 dummy_prefix_header[18];
    WP_dfc_flow_info dfc_flow_info;
    WP_flow_class_rule flow_class_rule;
    WP_CHAR rule_string[128];
    WP_classification_filter classification_filter = {
        0
    };
    WP_flow_class_forwarding_action forwarding_action;
    WP_iw_agg_bridging iw_agg_bridging_cfg;
    WP_flow_class_priority_action priority_action;
    WP_flow_class_ext_action action;
    WP_U8 action_type[3];
    WP_flow_class_action_info action_info[3];

    /* Dummy MAC addresses used to send into Bridge2 */
    for (jj = 0; jj < 12; jj++)

    {
        dummy_prefix_header[jj] = jj;
    }

    /*  priority remarking is done in VLAN field. So the VLAN tag is added here           *
     * Dummy header is added for  flow. VLAN tag has the priority. The priority remarking *
     * is done in the flow add                                                            */
    dummy_prefix_header[12] = 0x81; /* eth type = VLAN */
    dummy_prefix_header[13] = 0x00;

    /* VLAN Tag        */
    /* NOTE: filter will be based on this VLAN tag,  take care while adding the VLAN tag */
    /*adding VLAN tag 0x0a for traffic and 0x0b for control.... accordingly filter and flow rule is also defined */
    /*If you want  different VLAN tag then modify here and recompile the code accordingly modify the filter rule also */

    {
        dummy_prefix_header[14] = 0x00;
        dummy_prefix_header[15] = 0x0a;
    }

    dummy_prefix_header[16] = 0x08; /* Eth type = IP  */
    dummy_prefix_header[17] = 0x00;

    /* PPP Switching System */
    h_iw_sys_pppsw =
        WP_IwSystemCreate (WP_WINPATH (0), WP_IW_PPP_SWITCHING_MODE,
                           &iw_sys_pppsw_cfg);
    App_TerminateOnError (h_iw_sys_pppsw, "WP_IwSystemCreate() pppsw");

    /* Bridge2 */
    iw_sys_bridge.tag = tag_iw_sys_bridge2;
    h_iw_sys_bridge2 =
        WP_IwSystemCreate (APP_MASTER_WPID,
                           WP_IW_VLAN_AWARE_BRIDGING_MODE, &iw_sys_bridge);
    App_TerminateOnError (h_iw_sys_bridge2,
                          " WP_IwSystemCreate() Bridge2");

    /* Create 2 USER PROGRAMMABLE FILTERS */
    /* Add filter for  HDLC TRAFFIC CONTROL AND DATA */
    classification_filter.fields_mask = WP_FIELD_IDS_ARRAY;
    classification_filter.filter_type = WP_CLASS_FILTER_EXT_ACTION_EMC;
    classification_filter.field_ids_array[0] = WP_FIELD_ID_IN_PORT;
    classification_filter.field_ids_array[1] = WP_FIELD_ID_VLAN_ID;
    classification_filter.field_ids_array[2] = WP_FIELD_ID_LAST;
    classification_filter.no_match_action = WP_CLASS_FILTER_NO_MATCH_DENY;
    h_bridge2_filter[0] =
        WP_IwClassFilterAdd (h_iw_sys_bridge2, &classification_filter);
    App_TerminateOnError (h_bridge2_filter[0],
                          " WP_IwClassFilterAdd() Bridge2");
    memcpy (&iw_agg_bridging_cfg, &default_iw_agg_bridging_cfg,
            sizeof (WP_iw_agg_bridging));

    /* Create Default Aggregation for Bridge */
    iw_agg_bridging_cfg.tag = tag_agg_default_bridge2;
    iw_agg_bridging_cfg.txfunc = h_bridge2_iwhost;
    iw_agg_bridging_cfg.input_bport = 0;
    iw_agg_bridging_cfg.intmode = WP_IW_INT_ENABLE;
    h_bridge2_default_agg =
        WP_IwFlowAggregationCreate (APP_MASTER_WPID,
                                    WP_IW_VLAN_AWARE_BRIDGING_MODE,
                                    &iw_agg_bridging_cfg);
    App_TerminateOnError (h_bridge2_default_agg,
                          " WP_IwFlowAggregationCreate() Bridge2 Default");
    iw_agg_bridging_cfg.intmode = WP_IW_INT_DISABLE;
    for (ii = 0; ii < APP_MAX_PORT_HDLC; ii++)

    {
        WP_rx_binding_ppp_switching rx_binding_pppsw;

        /* Create a bridge port for each HDLC port */
        bridge_port_cfg.tag = tag_bridge2_bport_hdlc[ii];
        bridge_port_cfg.flow_agg = h_bridge2_default_agg;
        bridge_port_cfg.input_filters_mask = 0x80000000;
        bridge_port_cfg.classification_flag =
            WP_IW_IWPORT_CLASSIFICATION_ENABLED;
        h_bridge2_bport_hdlc[ii] =
            WP_IwBridgingPortCreate (h_iw_sys_bridge2, &bridge_port_cfg);
        App_TerminateOnError (h_bridge2_bport_hdlc[ii],
                              "WP_IwBridgingPortCreate() Bridge2 HDLC");

        /* Create a PPP Switching flow agg for each HDLC port */
        iw_agg_ppp_switching.tag = tag_agg_pppsw[ii];
        iw_agg_ppp_switching.txfunc = h_iw_sys_bridge2;
        iw_agg_ppp_switching.iw_port = h_bridge2_bport_hdlc[ii];
        iw_agg_ppp_switching.prefix_length = 18;    /*for adding dummy mac header with vlan header */
        iw_agg_ppp_switching.l2_header_insert_mode =
            WP_IW_L2H_INSERT_ENABLE;

        //memcpy(&iw_agg_ppp_switching.prefix_header, dummy_prefix_header, 18);/*add dummy header*/
        memcpy (iw_agg_ppp_switching.prefix_header,
                dummy_prefix_header, 18);
        h_flow_agg_pppsw_link[ii] =
            WP_IwFlowAggregationCreate (WP_WINPATH (0),
                                        WP_IW_PPP_SWITCHING_MODE,
                                        &iw_agg_ppp_switching);
        App_TerminateOnError (h_flow_agg_pppsw_link[ii],
                              " WP_IwFlowAggregationCreate() PPPSW");

        /* Bind the HDLC RX channel to the PPP Switching System using this flow agg */
        rx_binding_pppsw.aggregation = h_flow_agg_pppsw_link[ii];
        rx_binding_pppsw.mru = 1536;
        rx_binding_pppsw.encap_mode = WP_IW_VCMUX;
        rx_binding_pppsw.acfc_mode = WP_IW_ACFC_ENABLE;
        rx_binding_pppsw.pfc_mode = WP_IW_PFC_ENABLE;
        rx_binding_pppsw.pppmux_ncp_filter =
            WP_IW_PPPMUX_NCP_FILTER_SWITCH;
        rx_binding_pppsw.forward_ppp_to_bundle_mode =
            WP_IW_PPPSW_FORWARD_PPP_TO_LINK;
        rx_binding_pppsw.cisco_hdlc_mode = WP_IW_CISCO_HDLC_DISABLE;
        status =
            WP_IwRxBindingCreate (h_hdlc_rx[ii], h_iw_sys_pppsw,
                                  h_qnode_iwq, &rx_binding_pppsw);
        App_TerminateOnError (status, " WP_IwRxBindingCreate() PPPSW");
    }

    /* Create a bridge port to represent ENET2 */
    bridge_port_cfg.tag = tag_bport_enet2;
    bridge_port_cfg.flow_agg = h_bridge2_default_agg;
    bridge_port_cfg.input_filters_mask = 0;
    bridge_port_cfg.classification_flag =
        WP_IW_IWPORT_CLASSIFICATION_DISABLED;
    h_bridge2_bport_enet2 =
        WP_IwBridgingPortCreate (h_iw_sys_bridge2, &bridge_port_cfg);
    App_TerminateOnError (h_bridge2_bport_enet2,
                          "WP_IwBridgingPortCreate() ENET2");

    /* HDLC Control Packets go here to this flow agg (0x0F008035)  */
    /* Remove dummy mac header from packet which is added at PPP switch */
    /* packet will be host terminated */
    iw_agg_directmap_cfg.tag = tag_agg_control;
    iw_agg_directmap_cfg.txfunc = h_bridge2_iwhost;
    iw_agg_directmap_cfg.iw_port = 0;
    iw_agg_directmap_cfg.extraction_length = 18;
    iw_agg_directmap_cfg.prefix_length = 0;
    iw_agg_directmap_cfg.l2_header_insert_mode = WP_IW_L2H_INSERT_DISABLE;
    iw_agg_directmap_cfg.intmode = WP_IW_INT_ENABLE;
    h_flow_agg_control =
        WP_IwFlowAggregationCreate (WP_WINPATH (0),
                                    WP_IW_DIRECTMAP_MODE,
                                    &iw_agg_directmap_cfg);
    App_TerminateOnError (h_flow_agg_control,
                          " WP_IwFlowAggregationCreate() CC");

    /* HDLC DATA Packets go here to this flow agg (0x0F000800)  */
    /* Remove dummy mac header ,vlan and hdlc header from packet and insert mac header */
    /* packet will be forwarded to enet2 terminated */
    iw_agg_directmap_cfg.txfunc = h_device_enet2;   /*Packet will be forwarded to Enet2 device */
    iw_agg_directmap_cfg.iw_port = h_bridge2_bport_enet2;
    iw_agg_directmap_cfg.extraction_length = 22;
    iw_agg_directmap_cfg.prefix_length = 14;
    iw_agg_directmap_cfg.l2_header_insert_mode = WP_IW_L2H_INSERT_ENABLE;
    iw_agg_directmap_cfg.intmode = WP_IW_INT_DISABLE;

    //memcpy(&iw_agg_directmap_cfg.prefix_header, dummy_prefix_header,14);
    memcpy (iw_agg_directmap_cfg.prefix_header, dummy_prefix_header, 14);

    /* create multiple flow agg for each rule */
    for (ii = 0; ii < APP_MAX_FLOW_AGG_HDLC; ii++)

    {
        iw_agg_directmap_cfg.tag = tag_agg_enet2[ii];
        h_flow_agg_enet2[ii] =
            WP_IwFlowAggregationCreate (WP_WINPATH (0),
                                        WP_IW_DIRECTMAP_MODE,
                                        &iw_agg_directmap_cfg);
        App_TerminateOnError (h_flow_agg_enet2[ii],
                              " WP_IwFlowAggregationCreate() ENET2  ");
    }

    /* Add DFC flows */
    memset (&flow_class_rule, 0, sizeof (WP_flow_class_rule));
    memset (&dfc_flow_info, 0, sizeof (WP_dfc_flow_info));

    /* NOTE for this test we are using HDLC#0 to send out and receive the packet, so we use the bridge port for HDLC 0 */
    dfc_flow_info.input_port = h_bridge2_bport_hdlc[0]; /* Note that Input_port is one of the fields in the filter */
    dfc_flow_info.output_port = 0;

    /* While adding these flows, make sure to use the correct filter for each flow */
    /* HDLC Control packets flow */
    dfc_flow_info.filter_handle = h_bridge2_filter[0];

    /*NOTE: Please set dummy_prefix_header according else DFC will fail */
    sprintf (rule_string, "n;n;n;n;n;n;n;n;n;n;0xb;n;n;n;n;n"); /*rule for control packet */
    forwarding_action.flow_aggregation = h_flow_agg_control;
    flow_class_rule.classifier_string = rule_string;
    flow_class_rule.dfc_info = &dfc_flow_info;
    flow_class_rule.match_action = WP_FLOW_CLASS_MODE_FLOW_AGG;
    flow_class_rule.action_info.forwarding_action = forwarding_action;
    h_bridge2_dfcflow_control =
        WP_IwFlowAdd (h_iw_sys_bridge2, WP_FLOW_CLASS_RULE,
                      &flow_class_rule);
    App_TerminateOnError (h_bridge2_dfcflow_control,
                          " WP_IwFlowAdd() DFC Bridge2");
    policer_handle =
        WP_PolicerCreate (WP_WINPATH (0) /*wpid */ , WP_POLICER_V1,
                          policer_config);
    App_TerminateOnError (policer_handle,
                          " WP_IwPolicerCreate() bridge2 ");

    /* Add DFC flow */
    /*Creating (HDLC_NUM*APP_MAX_HDLC_PORTS) DFC flow rule.  */
    /*Here same rule is configured for all DFC flow... */
    /*All rule can be configured separately and accordingly Flow agg. can be choosen */
    dfc_flow_info.filter_handle = h_bridge2_filter[0];
    for (ii = 0; ii < HDLC_NUM; ii++)
    {
        for (jj = 0; jj < APP_MAX_HDLC_PORTS; jj++)

        {

            /* NOTE for this test we are using HDLC#ii to send out and receive the packet, */
            dfc_flow_info.input_port = h_bridge2_bport_hdlc[ii];    /* Note that Input_port is one of the fields in the filter , port need to choosen carefully */
            dfc_flow_info.output_port = 0;

            /*NOTE: Please set dummy_prefix_header according else DFC will fail */
            /* currently using same rule for all input... we can have different rule for each input */
            sprintf (rule_string, "n;n;n;n;n;n;n;n;n;n;0xa;n;n;n;n;n"); /*rule for traffic packet */
            priority_action.priority_value = ii;
            priority_action.field_type = WP_FLOW_PRIORITY_VLAN; /*Priority remarking based on vlan */
            priority_action.action_mode =
                WP_FLOW_PRIORITY_MAPPING | WP_FLOW_PRIORITY_REMARKING;
            flow_class_rule.match_action = WP_FLOW_CLASS_MODE_EXT_ACTION;
            action_type[0] = WP_FLOW_CLASS_MODE_PRIORITY;
            action_type[1] = WP_FLOW_CLASS_MODE_FLOW_AGG;
            action_type[2] = WP_FLOW_CLASS_MODE_POLICER;
            action.num_of_actions = 3;
            action.action_type = action_type;
            forwarding_action.flow_aggregation = h_flow_agg_enet2[ii];
            action_info[0].priority_action = priority_action;
            action_info[1].forwarding_action = forwarding_action;
            action_info[2].policer_action.policer = policer_handle;
            action.action_info = action_info;
            flow_class_rule.action_info.ext_action = action;
            flow_class_rule.classifier_string = rule_string;
            flow_class_rule.dfc_info = &dfc_flow_info;
            h_bridge2_dfcflow[ii][jj] =
                WP_IwFlowAdd (h_iw_sys_bridge2, WP_FLOW_CLASS_RULE,
                              &flow_class_rule);
            App_TerminateOnError (h_bridge2_dfcflow[ii][jj],
                                  " WP_IwFlowAdd() DFC Bridge2 rule ");
        }
    }

    /* Be sure to build the system */
    status = WP_IwSystemBuild (h_iw_sys_bridge2);
    App_TerminateOnError (status, " WP_IwSystemBuild() Bridge2");
}

/***************************************************************************** 
* Function name: App_perform_freerun 
* Description  : function send traffic from host CPU and then check statistics. Developer of  
*                     application should be responsible for matain this function for each application. 
* Input  params: None 
* Output params: None 
* Return val   : None 
*****************************************************************************/
static void App_perform_freerun (void)
{
    WP_status result;
    WP_U32 i;                   

	/*Send packets */
    for (i = 0; i < APP_MAX_PKTS_NUM; i++)
    {
        printf ("call App_DataSend() now\n");
        App_DataSend (h_enet1_tx, h_pool_buffer_iw);
        WP_Delay (100000);
    }

    WP_Delay (100000);         /*Check Statistics */

    App_CheckStats (&result);

    if (result == WP_OK)
    {
        printf ("test pass!\n");
    }
    else
    {
        printf ("test fail!\n");
    }                           

   	printf("This is the last test case in this application!\n");
   	printf("This test case need further manually test!\n");

	/*Quit test */
    App_Quit (1);
}
