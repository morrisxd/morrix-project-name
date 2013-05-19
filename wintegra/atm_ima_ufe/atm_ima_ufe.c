/**********************************************************************************************
 * (C) Copyright PMC - WIN division (Wintegra) 2012.  All rights reserved.
 * PMC-Sierra (WINTEGRA) CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of PMC-Sierra (Wintegra).
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ***********************************************************************************************/

/**********************************************************************************************
 *  Appplication : ATM IMA UFE
 *  File Name    : atm_ima_ufe.c
 *
 ***********************************************************************************************/
/***********************************************************************************************
 *  Description: 
 *    IMA ATM application is demo application for the IMA traffic flow. In this application
 *    maximum 8 number of IMA groups can be created with 8 TDM/UFE links as IMA group links. For this
 *    demo two links are attached with the IMA group. Host packet is generated and send from the 
 *    Egress side via IMA device. TDM/UFE links are looped back and the traffic is received through 
 *    IMA device in the Ingress and then sent to Enet. AAL5 protocol is used in this demo.
 *  Egress:
 *    MPLS labeled host packet is generated and send from the Host through Enet1 port. Enet Rx
 *    channel points to the Bridging Interworking System. EMC filter is applied and The packet 
 *    is classified with VC label field. Classified packet is sent to Bridging flow aggregation
 *    where the Enet and MPLS headers are extracted and sent to the ATM Tx channel. ATM Tx channel
 *    in turn sends the traffic to the IMA group and then to EMPHY port. 
 *
 *    Apart from forwarding, policing action also applied. Two channels are created for the demo 
 *    with different tx_tq values. Each tx channel is mapped to one flow aggregation. For every
 *    flow aggregation, one dfc flow is created with MPLS classification. 
 *
 *  Ingress:
 *    Ingress side traffic is received through IMA device and then send to the IMA group linked to
 *    these IMA devices. In this application, since the UFE4 is looped back, the traffic is received
 *    by the same IMA group in Rx side. The IMA group points to ATM Rx channel which in turn points
 *    to ATM PWE3 flow aggregation via a directmap interworking system. ATM traffic is send via the
 *    Enet2 port.
 ***********************************************************************************************/

#define WTI_CESOP_REGRESSION_TEST 1

/* Number of IMA DPS generated ticks in 1 second */
#define TICKS_PER_SECOND 2
#define SECONDS(n) (n * TICKS_PER_SECOND)
#define ENABLE_EMPHY_BEFORE_UFE_INIT

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
#include "wpx_tdm_comet.h"
#include "wpx_enet_phy.h"

// #include <mt_api.h>

#define PHY_TYPE_T1	0
#define PHY_TYPE_E1	1

#include "atm_ima_ufe.h"

#include "ufe_utility.h"

extern WUFE_status WUFE_SystemDisplay (WP_U32 wufe_id);

#include "atm_ima_ufe_util.c"
#include "atm_ima_ufe_stats.c"

WP_U8 prbs_result[336];

#include "flexmux_util.c"
// #include "flexmux_alarms_and_pm.c"
#include "ufe_utility.c"

WUFE_init_config ufe4_config;
WP_ima_event a_task;

static void release_test (void)
{
   WP_DriverRelease ();

   exit (1);
}

void WT_UfeTerminateOnError (WUFE_status handle, WP_CHAR * s, WP_U32 id,
                             WP_U32 LineNum)
{
   if (status != WUFE_OK)
   {
      printf ("UFE Error %s %s\n", WUFE_error_name[(status & 0x03ffffff)],
              s);
      release_test ();
   }

}

void App_ResetUfe (void)
{
   WPX_Ufe4HwReset (0, WP_PORT_UPI1);

   if (WTI_INITIALIZE_FIRMWARE)
   {
      // Announce that UFE firmware will be initialized through host and not through JTAG 
      printf
         ("\n******************************************************\n");
      printf (" UFE build configuration is loaded through the host!!!\n");
      printf
         ("******************************************************\n\n");
      memset (&ufe4_config, 0, sizeof (WUFE_init_config));
      ufe4_config.ufe_id = 0;
      status = WUFE_UfeInitializeFirmware (&ufe4_config);
      App_TerminateOnError (status, "WUFE_Ufe4InitializeFirmware");

   }                            // if(WTI_INITIALIZE_FIRMWARE)
}

/****************************************************************************************************************************
 * Function name: main()
 * Description  : Main function of the application. 
 *
 * Input  params: none
 * Output params: 
 * Return val   : none
 *****************************************************************************************************************************/
int main (int argc, char *argv[])
{
   WP_status status;
   app_task *task, a_task;
   WP_CHAR comm;

   status = WP_DriverInit ();
   App_TerminateOnError (status, "WP_DriverInit()");

   /* Initialize the Tags */
   App_TestInit ();

   /* Configure the context and initialize the System */
   App_SysInit ();

   /* Commit system resources */
   /* Enet Physical ports and UFE Card are initialzed */
   App_InitHW ();

   App_SystemSetup ();

   App_ResetUfe ();

   /* Create Enet and emphy ports */
   App_PortsCreate ();

   /* Create Host, Enet devices and allocate IMA(emphy) devices */
   App_DevicesAllocCreate ();

   /* Create Host, Buffer Pools and Qnodes */
   App_BufferPoolsCreate ();
   /* Initialize the Control registers, Create IMA system and Commit the system */

   status = WP_SysCommit ();
   App_TerminateOnError (status, "WP_SysCommit()");

   App_enableEmphyPort ();

   /*Initialize the framer. Configure and create UFE line and phy create */
   App_ufeSystem ();

   /* ASU sheduler Enable */
   status = WP_SysAsuEnable (WP_WINPATH (0));
   App_TerminateOnError (status, "WP_SysAsuEnable()");

   /* Create IMA groups and add links to the group */
   App_ImaSetup ();

   /* IMA group event generate */
   App_ImaGroupEvent ();

   /* Create Host, Enet and Host Channels */
   App_ChannelsCreate ();

   /* Create ATM channels for IMA */
   App_ImaChannelsCreate ();

   /* Sets up ENET --> UFE bridging interworking */
   App_CreateEnetUfeIwSystem ();

   /* Sets up Ufe  --> ENET interworking */
   App_CreateUfeEnetIwSystem ();

   /* Enable the ports and devices created earlier */
   App_PortsDevicesEnable ();

   /* Enable the Channels created earlier */
   App_ChannelsEnable ();

   display_events ();
   WP_Delay (1000000);
   display_events ();

   if (argc <= 1)
   {
      /*default mode, no parameters */
      while (1)
      {
         printf ("\n");
         printf ("MENU\n");
         printf ("=====================\n");
         printf ("a. Stats & Status\n");
         printf ("d. Debug\n");
         printf ("p. Send Packet\n");
         printf ("s. Simulate Interrupts\n");
         printf ("c. clear events\n");
         printf ("r. Reboot\n");
         printf ("x. Exit\n");

	do {
         	comm = getchar ();
	} while (0x10 == comm || 0x13 == comm);

         switch (comm)
         {
         case 'a':
            App_ShowStats ();
            break;
         case 'p':
            App_DataSend (h_enet1_tx, h_pool_buffer_iw);
            display_events_noima ();
            break;
            /*Not supported....Added code for future use. 
               case 'd':
               App_Debug(debug_on);
               break;
               case 's':
               WPI_SimulateInterrupts();
               break; */
         case 'c':
            display_events_noima ();
            break;
         case 'r':
            WPX_Reboot ();
            break;
         case 'x':
            {
               WP_DriverRelease ();
               exit (0);
            }
         }
         WPI_SimulateInterrupts ();
         while ((task = next_task (irq_task_list, &a_task)) != NULL)
            app_perform_action (task);
      }
   }
   else if ((argc == 3) && (!strcmp (argv[1], "freerun")))
   {
      /*free run mode */
      App_perform_freerun ();

   }
   else
   {
      printf ("Wrong parameter! Only \"go\" or \"go freerun\" supported!");
      /*Quit test */
      App_Quit (1);
   }
   return 0;
}

/*****************************************************************************
 * Function name: App_TestInit
 * Description  : Initializes the Tags used in this application. These Tags can 
 *                be of any value but need to be unique, i.e, two different Tags
 *                can not have the same value.
 * Input  params: None
 * Output params: None
 * Return val   : None
 *****************************************************************************/
void App_TestInit (void)
{
   tag_iw_sys_bridge1 = 600;

   tag_bport_enet1 = 700;

   tag_enet1_rx = 1600;
   tag_enet1_tx = 1700;

   tag_enet2_rx = 1800;
   tag_enet2_tx = 1900;

   tag_iwhost_rx1 = 90;
   tag_iwhost_rx2 = 91;

   /* IMA tags */
   tag_bridge1_bport_ima = 850;
   tag_agg_ima1 = 1150;
}

/*****************************************************************************
 * Function name: App_SysInit
 * Description  : Configures the context and Initializes the System with the 
 *                context 
 * Input  params: None
 * Output params: None
 * Return val   : None
 *****************************************************************************/
void App_SysInit (void)
{

   /* Context configuration */
   context_cfg.int_queue_data = &int_queue_table_cfg;
   context_cfg.atm_data = &atm_global_cfg;
   context_cfg.iw_data = &iw_global_cfg;
   context_cfg.pool_limit = 8;
   context_cfg.max_qnodes = 4;

   status = WP_SysInit (APP_MASTER_WPID, &context_cfg);
   App_TerminateOnError (status, "WP_SysInit()");
}

/*****************************************************************************
 * Function name: App_InitHW
 * Description  : Initializes Enet Physical ports and create/initializes the 
 *                clocks. UFE card is initialized in this function.
 * Input  params: None
 * Output params: None
 * Return val   : None
 *****************************************************************************/
void App_InitHW (void)
{

   status = WPX_BoardConfigure (0, WPX_CONFIGURE_2UPI_1XGI_10SGMII_UFE4);
   App_TerminateOnError (status,
                         "board configure WPX_CONFIGURE_2UPI_1XGI_10SGMII_UFE4");

   status = WPX_BoardSerdesInit (0, WP_PORT_ENET7, WPX_SERDES_LOOPBACK);   //WPX_SERDES_LOOPBACK  Loopback on Fast Enet
   /*NOTE: disable loopback from device config else loopback will not work */
   App_TerminateOnError (status, "WPX_BoardSerdesInit 7()");
   status = WPX_BoardSerdesInit (0, WP_PORT_ENET8, WPX_SERDES_LOOPBACK);
   App_TerminateOnError (status, "WPX_BoardSerdesInit 8()");

   /* Create BRG1 & BRG3 */
   status =
      WP_SysClockCreate (WP_WINPATH (0), WP_BRG1, WP_BRG_SRC_BRGIN2, 2);
   // status = WP_SysClockCreate(WP_WINPATH(0), WP_BRG2, WP_BRG_SRC_BRGIN2, 2);
   status =
      WP_SysClockCreate (WP_WINPATH (0), WP_BRG3, WP_BRG_SRC_BRGIN2, 2);
   //status = WP_SysClockCreate(WP_WINPATH(0), WP_BRG4, WP_BRG_SRC_BRGIN2, 2);
   //status = WP_SysClockCreate(WP_WINPATH(0), WP_BRG5, WP_BRG_SRC_BRGIN2, 2);
   /*Important: If we enable all the above clocks, IMA system will not become operational */

}

/*****************************************************************************
 * Function name: App_PortsCreate
 * Description  : App_PortsCreate function creates all the required enet and 
 *     emphy ports for this application Two enet ports are created for egress and 
 *     ingress. Number of UFE ports are configurable. With UFE card maximum 
 *     number of UFE ports can be 252(E1)/336(T1). Apart from enet and emphy, the IW host port 
 *     also created in this function. 
 * Input  params: None
 * Output params: None
 * Return val   : None
 *****************************************************************************/
void App_PortsCreate (void)
{

   /* Create Enet1 port */
   h_port_enet1 = WP_PortCreate (APP_MASTER_WPID, WP_PORT_ENET7,
                                 &port_enet_cfg);
   App_TerminateOnError (h_port_enet1, "WP_PortCreate() Enet7");

   /* Create Enet2 port */
   h_port_enet2 = WP_PortCreate (APP_MASTER_WPID, WP_PORT_ENET8,
                                 &port_enet_cfg);
   App_TerminateOnError (h_port_enet2, "WP_PortCreate() Enet8");

   /* Create an IW Host port */
   h_port_iwhost = WP_PortCreate (WP_WINPATH (0), WP_PORT_IW_HOST, NULL);
   App_TerminateOnError (h_port_iwhost, "WP_PortCreate() IW Host");

   /* Create EMPHY port */
   /* create UPI port as UFE */
   emphy_port = WP_PortCreate (WP_WINPATH (0),
                               APP_EMPHY_PORT, &upi_emphy_port_config);
   App_TerminateOnError (emphy_port,
                         "WP_PortCreate UPI EMPHY Transparent");

}

/*****************************************************************************
 * Function name: App_DevicesAllocCreate
 * Description  : This function creates Enet and IMA devices over the Enet
 *     ports created earlier. Also interworking host device is created for 
 *     the host generation and termination of the packets. 
 * Input  params: None
 * Output params: None
 * Return val   : None
 *****************************************************************************/
void App_DevicesAllocCreate (void)
{
   /* Create device on Enet1 */
   MAC_COPY (device_enet_cfg.mac_addr, wt_mac_enet1);
   h_device_enet1 =
      WP_DeviceCreate (h_port_enet1, WP_PHY (0), WP_DEVICE_ENET,
                       &device_enet_cfg);
   App_TerminateOnError (h_device_enet1, "WP_DeviceCreate() Enet1");
   /* Create device on Enet2 */
   device_enet_cfg.loopbackmode = WP_ENET_NORMAL;  /* no loop back */
   MAC_COPY (device_enet_cfg.mac_addr, wt_mac_enet2);
   h_device_enet2 =
      WP_DeviceCreate (h_port_enet2, WP_PHY (0), WP_DEVICE_ENET,
                       &device_enet_cfg);
   App_TerminateOnError (h_device_enet2, "WP_DeviceCreate() Enet2");

   /* Create an IW Host Device */
   h_dev_iwhost =
      WP_DeviceCreate (h_port_iwhost, 0, WP_DEVICE_IW_HOST, NULL);
   App_TerminateOnError (h_dev_iwhost, "WP_DeviceCreate() IW Host");
}

/*****************************************************************************
 * Function name: App_TdmDevicesCreate
 * Description  : This function creates IMA device over the emphy ports for the 
 *     devices allocation is done earlier.       
 * Input  params: None
 * Output params: None
 * Return val   : None
 *****************************************************************************/
void App_IMADevicesCreate (void)
{

   WP_U32 ii;

   /*create ATM device and ports */
   for (ii = 0; ii < APP_MAX_IMA_DEVICE; ii++)
   {
      h_dev_ima[ii] = WP_DeviceCreate (emphy_port,
                                       WP_EMPHY_PHY (0,
                                                     WUFE_PHY_INDEX_GET
                                                     (ufe4_app_system.
                                                      phy_handle[ii])),
                                       /*WP_PHY(0), */
                                       //WP_DEVICE_ATM, ufe_device_cfg);
                                       WP_DEVICE_IMA, ufe_device_cfg);
      App_TerminateOnError (h_dev_ima[ii], "WP_DeviceCreate() IMA device");
   }
}

/*****************************************************************************
 * Function name: App_BufferPoolsCreate
 * Description  : This function creates the buffer pools and Qnodes required for
 *     the application.   
 * Input  params: None
 * Output params: None
 * Return val   : None
 *****************************************************************************/
void App_BufferPoolsCreate (void)
{

   /* Create Buffer pools */
   h_pool_buffer_iw = WP_PoolCreate (APP_MASTER_WPID, WP_pool_iwbuffer,
                                     &pool_buffer_data_iw_cfg);
   App_TerminateOnError (h_pool_buffer_iw, "WP_PoolCreate iwbuffer iw");

   /* Pool for host termination buffers */
   h_pool_256 = WP_PoolCreate (WP_WINPATH (0), WP_pool_buffer,
                               &pool_buffer_data_256_cfg);
   App_TerminateOnError (h_pool_256, "PoolCreate() pool_256");

   h_pool_ring_host = WP_PoolCreate (WP_WINPATH (0), WP_pool_ring,
                                     &pool_ring_data_cfg);
   App_TerminateOnError (h_pool_ring_host, "PoolCreate() pool_ring_host");

   /* Create an interworking queue node */
   qnode_iwq_cfg.adjunct_pool = h_pool_buffer_iw;
   h_qnode_iwq = WP_QNodeCreate (APP_MASTER_WPID, WP_QNODE_IWQ |
                                 WP_QNODE_OPT_DEDICATED_RX_HWQ |
                                 WP_QNODE_OPT_FMU, &qnode_iwq_cfg);
   App_TerminateOnError (h_qnode_iwq, "WP_QNodeCreate() h_qnode_iwq");

   /* Host termination qnode */
   qnode_hostq_cfg.pool_buffer = h_pool_256;
   qnode_hostq_cfg.pool_ring = h_pool_ring_host;
   h_qnode_host =
      WP_QNodeCreate (WP_WINPATH (0), WP_QNODE_HOSTQ, &qnode_hostq_cfg);
   App_TerminateOnError (h_qnode_host, "WP_QnodeCreate() h_qnode_host");

   /* Qnode for ATM(IMA) channel */
   qn_iw_ima_cfg.adjunct_pool = h_pool_buffer_iw;
   ima_qn = WP_QNodeCreate (WP_WINPATH (0), WP_QNODE_IWQ, &qn_iw_ima_cfg);
   App_TerminateOnError (ima_qn, "WP_QNodeCreate() ima_qn");
}

/*****************************************************************************
 * Function name: App_SystemSetup
 * Description  : This function initialzies the Control Register with the call
 *    back functions and creates IMA system. The system resources are commited
 *    with the function WP_SysCommit()   
 * Input  params: None
 * Output params: None
 * Return val   : None
 *****************************************************************************/
void App_SystemSetup (void)
{
   WP_U32 ii;

   /*   IMA system create      */
   ima_sys_handle = WP_ImaSystemCreate (WP_WINPATH (0), &ima_sys_params);
   App_TerminateOnError (ima_sys_handle, "WP_ImaSystemCreate()");

   status = WP_ControlRegister (WP_EVENT_RX_INDICATE, App_EventRxIndicate);
   App_TerminateOnError (status, "WP_ControlRegister()");

   status = WP_ControlRegister (WP_EVENT_TX_INDICATE, App_EventTxIndicate);
   App_TerminateOnError (status, "WP_ControlRegister()");

   status =
      WP_ControlRegister (WP_EVENT_POLLED_TX_INDICATE,
                          App_EventTxIndicatePolled);
   App_TerminateOnError (status, "WP_ControlRegister()");

   status =
      WP_ControlRegister (WP_EVENT_STATUS_INDICATE,
                          App_EventErrorIndicate);
   App_TerminateOnError (status, "WP_ControlRegister()");

   /* IMA Event callback initilization */
   status = WP_ControlRegister (WP_EVENT_IMA, &App_ImaEvent);
   App_TerminateOnError (status, "WP_ControlRegister()");

   status = WP_SysSchedulerCreate (WP_WINPATH (0), cw_config);
   App_TerminateOnError (status, "WP_SysSchedulerCreate()");

   /* Initialize Rate Table */
   for (ii = 0; (ii < RATE_TABLE_SLOTS) && VP_PER_IMA_GROUP; ii++)
   {
      shaping_table[ii] = ii % VP_PER_IMA_GROUP;
   }
   shaping_table[RATE_TABLE_SLOTS] = WP_HIER_SHAPING_TABLE_TERM;

}

extern void WPX_UFE_FRAMER_FlexmuxTerminateOnError (char
                                                    *pFatalErrorMessage);

/****************************************************************************************************************************
 * Function name: App_ufeSystem()
 * Description  :   Initialise UFE and framer. create line and phy interface for UFE. Create and enable the UFE system

 * Input  params: none
 * Output params: 
 * Return val   : none
 *****************************************************************************************************************************/
void App_ufeSystem (void)
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

   property = 0;
   num_of_lines = WT_MAX_LINE_INDEX;
   transfer_type = WUFE_SDH_TYPE_E1;
   framing_mode = WUFE_UNFRAMED;
   cas_mode = WUFE_CAS_DISABLE;

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
   WTI_FlexmuxInit (APP_EMPHY_PORT, 0, WT_FLEXMUX_UFE_412_1_PORT_OC12, WPX_UFE_FRAMER_DEVICE_MODE_SDH, WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC4,  //WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC3,
                    &flex_global_params);

   // Configure and create UFE lines for T1 SDH mode
   for (i = WT_FIRST_LINE_INDEX; i < (num_of_lines + WT_FIRST_LINE_INDEX);
        ++i)
   {
      /* UFE line configuration, creation and enabling */
      /* Create Framer Connection */

      status = WUFE_LineCreate (&ufe4_app_system.line_handle[i],
                                ufe4_app_system.ufeid,
                                WUFE_STRUCT_LINE_SDH, &line_cfg);

      WT_UfeTerminateOnError (status, "WUFE_LineCreate", i, __LINE__);

      if (line_cfg->transfer_type == WUFE_SDH_TYPE_E1)
      {
         printf ("WUFE_SDH_TYPE_E1 link(%3d) created, %d all\r", i,
                 num_of_lines);
         client_port = i + (i / 3);
      }
      else
      {
         printf ("WUFE_SDH_TYPE_T1 link(%3d) created, %d all\r", i,
                 num_of_lines);
         client_port = i;
      }

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
      // or to: WUFE_LINE_LOOPBACK_TYPE_SYSTEM (in case of loopback from ufe to ufe)

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

   for (i = WT_FIRST_LINE_INDEX; i < (num_of_lines + WT_FIRST_LINE_INDEX);
        ++i)
   {
      ufe_status = WUFE_PhyCreate (&ufe4_app_system.phy_handle[i],
                                   ufe4_app_system.line_handle[i],
                                   &ufe4_phy_cfg, WUFE_FULL_DUPLEX);
      WT_UfeTerminateOnError (ufe_status, "WUFE_PhyCreate", i, __LINE__);
   }

   /*Create ATM Device */
   App_IMADevicesCreate ();
}

/*****************************************************************************
 * Function name: App_ImaSetup
 * Description  : IMA groups and links are created in this function. It takes 
 *    some time for the IMA group to reach operational state. A check is done
 *    for the IMA group state, for reaching operational state with a 20 sec 
 *    timeout before initial traffic flow.
 * Input  params: None
 * Output params: None
 * Return val   : None
 *****************************************************************************/
void App_ImaSetup (void)
{
   WP_U32 ii = 0;
   WP_U32 expected_group_op, actual_group_op;
   WP_U16 prev_grp_state[NUM_OF_GROUPS];
   WP_U16 prev_fe_grp_state[NUM_OF_GROUPS];
   WP_U32 i, x, j;

   /* Group State information used for detecting change */
   WP_ima_group_state_info group_state[NUM_OF_GROUPS];

   /* IMA group create */
   for (ii = 0; ii < NUM_OF_GROUPS; ii++)
   {
      ima_group.group_alloc = &group_alloc[ii];
      ima_group_handle[ii] =
         WP_ImaGroupCreate (ima_sys_handle, &ima_group);
      App_TerminateOnError (ima_group_handle[ii], "WP_ImaGroupCreate()");

      /* Get the Group started before adding the link */
      status = WP_ImaGroupStartup (ima_group_handle[ii]);
      App_TerminateOnError (status, "WP_ImaGroupStartup()");

      j = 0;
#if 0
      /* Enable individual events (errors) */
      j = (1 << WP_IMA_EVENT_TTS) |
         (1 << WP_IMA_EVENT_TOS) |
         (1 << WP_IMA_EVENT_TXQEMPTY) |
         (1 << WP_IMA_EVENT_TXQFULL) |
         (1 << WP_IMA_EVENT_GSU_LASR_RX) | (1 << WP_IMA_EVENT_GSU_LASR_TX);

      printf ("TEst\n");
      status = WP_ImaGroupEventEnable (ima_group_handle[ii], j);
      App_TerminateOnError (status, "WP_ImaGroupEventEnable()");
#endif
   }

   /*Enable UFE phy system and ufe line */
   App_EnableUFESystem ();

   for (ii = 0; ii < NUM_OF_GROUPS; ii++)
   {
      ima_link_handle[2 * ii] = WP_ImaLinkAdd (ima_group_handle[ii],
                                               h_dev_ima[2 * ii],
                                               h_dev_ima[2 * ii],
                                               ima_link);
      App_TerminateOnError (ima_link_handle[2 * ii], "WP_ImaLinkAdd()");

      ima_link_handle[2 * ii + 1] = WP_ImaLinkAdd (ima_group_handle[ii],
                                                   h_dev_ima[2 * ii + 1],
                                                   h_dev_ima[2 * ii + 1],
                                                   ima_link);
      App_TerminateOnError (ima_link_handle[2 * ii + 1],
                            "WP_ImaLinkAdd()");
   }

   WP_Delay (10000);

   /* Initialize group NE state to "START_UP."  Note, this element */
   /* is updated by the WP_ImaGroupState() function.               */
   expected_group_op = 0;
   for (i = 0; i < NUM_OF_GROUPS; i++)
   {
      group_state[i].ne_state = WP_IMA_GSM_START_UP;
      group_state[i].fe_state = WP_IMA_GSM_START_UP;
      expected_group_op |= 0x80000000 >> i;
   }

   for (i = 0; i < NUM_OF_GROUPS; i++)
   {
      prev_grp_state[i] = WP_IMA_GSM_START_UP;
      prev_fe_grp_state[i] = WP_IMA_GSM_START_UP;
   }

   /* Wait until all groups are operational before sending any data */
   actual_group_op = 0;
   x = 0;

   while (expected_group_op != actual_group_op)
   {
      /* If we've waited more than 20 seconds, something is wrong */
      if (ticks > SECONDS (20))
      {

         printf ("Test Abort: Group Not Operational, check cable.\n");
         printf ("Test Failed\n");
         return;

      }
      /* Check for IMA related events */
      display_events ();
      /* Get state of groups */
      for (i = 0; i < NUM_OF_GROUPS; i++)
      {
         status = WP_ImaGroupState (ima_group_handle[i], &group_state[i]);
         App_TerminateOnError (status, "WP_ImaGroupState()");
         printf ("group_state[i].ne_state=%d\n", group_state[i].ne_state);
         if (prev_grp_state[i] != group_state[i].ne_state)
         {
            prev_grp_state[i] = group_state[i].ne_state;
            if (group_state[i].ne_state == WP_IMA_GSM_OPERATIONAL)
               actual_group_op |= 0x80000000 >> i;
         }
      }
   }
}

/*****************************************************************************
 * Function name: App_ImaGroupEvent
 * Description  : IMA groups events are enabled in this function. These events
 *    can be captured and used for debugging. The event table includes the 
 *    following events:
 *          WP_IMA_EVENT_BANDWIDTH_CHANGE
 *          WP_IMA_EVENT_TTS
 *          WP_IMA_EVENT_TOS
 *          WP_IMA_EVENT_OPERATIONAL_CHANGE
 *          WP_IMA_EVENT_TXQEMPTY
 *          WP_IMA_EVENT_TXQFULL
 *          WP_IMA_EVENT_FE_ICP_CHANGE
 *          WP_IMA_EVENT_NE_ICP_CHANGE
 *          WP_IMA_EVENT_GSU_LASR_RX
 *          WP_IMA_EVENT_GSU_LASR_TX
 * Input  params: None
 * Output params: None
 * Return val   : None
 *****************************************************************************/
void App_ImaGroupEvent (void)
{
   WP_U32 ii = 0;
   WP_U32 event = 0;

   for (ii = 0; event_table[ii].event != NULL; ii++)
      event |= (1 << event_table[ii].shift);

   for (ii = 0; ii < NUM_OF_GROUPS; ii++)
   {
      status = WP_ImaGroupEventEnable (ima_group_handle[ii], event);
      App_TerminateOnError (status, "WP_ImaGroupEventEnable()");
   }
}

/*****************************************************************************
 * Function name: App_ChannelsCreate
 * Description  : This function creates the Enet and Host channels
 * Input  params: None
 * Output params: None
 * Return val   : None
 *
 * Note : ATM channels are created in the function App_ImaChannelsCreate()
 *****************************************************************************/
void App_ChannelsCreate (void)
{
   /* Create Enet channels */
   h_enet1_rx = WP_ChannelCreate (tag_enet1_rx, h_device_enet1,
                                  h_qnode_iwq, WP_CH_RX, WP_ENET,
                                  &ch_enet_cfg);
   App_TerminateOnError (h_enet1_rx, "WP_ChannelCreate() Enet Rx");

   /* NOTE this channel is in NON-IW mode, used to send out a packet from the host out of ENET1 */
   ch_enet_cfg.iwmode = WP_PKTCH_IWM_DISABLE;
   h_enet1_tx = WP_ChannelCreate (tag_enet1_tx++, h_device_enet1,
                                  h_qnode_iwq, WP_CH_TX, WP_ENET,
                                  &ch_enet_cfg);
   App_TerminateOnError (h_enet1_tx, "WP_ChannelCreate() Enet Tx");

   tx_binding_cfg.dci_mode = WP_IW_DYN_CH_INSERT_ENABLE;
   status =
      WP_IwTxBindingCreate (h_enet1_tx, WP_IW_TX_BINDING, &tx_binding_cfg);
   App_TerminateOnError (status, "WP_IwTxBindingCreate()");

   /* Create Enet 2 channels */
   ch_enet_cfg.iwmode = WP_PKTCH_IWM_DISABLE;
   h_enet2_rx = WP_ChannelCreate (tag_enet2_rx, h_device_enet2,
                                  h_qnode_iwq, WP_CH_RX, WP_ENET,
                                  &ch_enet_cfg);
   App_TerminateOnError (h_enet2_rx, "WP_ChannelCreate() Enet Rx");

   ch_enet_cfg.iwmode = WP_PKTCH_IWM_ENABLE;
   h_enet2_tx = WP_ChannelCreate (tag_enet2_tx, h_device_enet2,
                                  h_qnode_iwq, WP_CH_TX, WP_ENET,
                                  &ch_enet_cfg);
   App_TerminateOnError (h_enet2_tx, "WP_ChannelCreate() Enet Tx");

   tx_binding_cfg.dci_mode = WP_IW_DYN_CH_INSERT_ENABLE;
   status =
      WP_IwTxBindingCreate (h_enet2_tx, WP_IW_TX_BINDING, &tx_binding_cfg);
   App_TerminateOnError (h_enet2_tx, "WP_IwTxBindingCreate()");

   /* Crate IW HOST termination channels one for each bridge */
   tx_binding_cfg.dci_mode = WP_IW_DYN_CH_INSERT_DISABLE;
   ch_iw_rx_cfg.tx_binding_config = &tx_binding_cfg;
   h_bridge1_iwhost = WP_ChannelCreate (tag_iwhost_rx1,
                                        h_dev_iwhost,
                                        h_qnode_iwq,
                                        WP_CH_RX,
                                        WP_IW_HOST, &ch_iw_rx_cfg);
   App_TerminateOnError (h_bridge1_iwhost,
                         "WP_ChannelCreate() Host Rx 1 ");

   h_bridge2_iwhost = WP_ChannelCreate (tag_iwhost_rx2,
                                        h_dev_iwhost,
                                        h_qnode_iwq,
                                        WP_CH_RX,
                                        WP_IW_HOST, &ch_iw_rx_cfg);
   App_TerminateOnError (h_bridge2_iwhost, "WP_ChannelCreate() Host Rx 2");
}

/*****************************************************************************
 * Function name: App_ImaChannelsCreate
 * Description  : This function creates the Tx and Rx ATM channels for traffic
 *      to send via IMA group/links. For the Tx channel, binding is done in 
 *      this function itself. For Rx channel, the binding is done in the functin
 *      App_CreateUfeEnetIwSystem()
 * Input  params: None
 * Output params: None
 * Return val   : None
 *****************************************************************************/
void App_ImaChannelsCreate (void)
{
   WP_U32 ii = 0;

   iw_ch_aal5_cfg[0].iwmode = WP_ATMCH_IWM_ENABLE;
   for (ii = 0; ii < MAX_ATM_TX_CHANNEL; ii++)
   {
      iw_ch_aal5_cfg[0].tx_tq = ii;
      ima_tx_ch_handle[ii] =
         WP_ChannelCreate (0, ima_group_handle[0], ima_qn, WP_CH_TX,
                           WP_ATM_AAL5, &iw_ch_aal5_cfg[0]);
      App_TerminateOnError (ima_tx_ch_handle[ii],
                            "WP_ChannelCreate() IMA Tx");

      /* tx binding for IMA channel */
      status =
         WP_IwTxBindingCreate (ima_tx_ch_handle[ii], WP_IW_TX_BINDING,
                               &tx_binding_cfg);
      App_TerminateOnError (status, "WP_IwTxBindingCreate()");
   }

   for (ii = 0; ii < NUM_OF_GROUPS; ii++)
   {
      /* Create ATM rx channel  */
      ima_rx_ch_handle[ii] = WP_ChannelCreate (0, ima_group_handle[ii],
                                               h_qnode_host, WP_CH_RX,
                                               WP_ATM_AAL5,
                                               &iw_ch_aal5_cfg[0]);
      App_TerminateOnError (ima_rx_ch_handle[ii],
                            "WP_ChannelCreate() IMA AAL5 Rx");
   }
}

/****************************************************************************************************************************
 * Function name: App_EnableUFESystem()
 * Description  :   Enable UFE phy and line interface
 *
 * Input  params: none
 * Output params: 
 * Return val   : none
 *****************************************************************************************************************************/

void App_EnableUFESystem (void)
{

   WP_U32 i;
   WUFE_status ufe_status;

   ufe_status =
      WUFE_SystemEnable (ufe4_app_system.ufeid, WUFE_SYS_EMPHY,
                         WUFE_FULL_DUPLEX);
   WT_UfeTerminateOnError (ufe_status, "WUFE_SystemEnable ", 0, __LINE__);

   for (i = 0; i < WT_MAX_LINE_INDEX; i++)
   {
      ufe_status =
         WUFE_LineEnable (ufe4_app_system.line_handle[i],
                          WUFE_FULL_DUPLEX);
      WT_UfeTerminateOnError (ufe_status, "SDH WUFE_LineEnable", i,
                              __LINE__);

      ufe_status =
         WUFE_PhyEnable (ufe4_app_system.phy_handle[i], WUFE_FULL_DUPLEX);
      WT_UfeTerminateOnError (ufe_status, "WUFE_PhyEnable() UFE4", 0,
                              __LINE__);
   }

}

/*****************************************************************************
 * Function name: App_PortsDevicesEnable
 * Description  : Enet ports and devices are enebled in this function
 * Input  params: None
 * Output params: None
 * Return val   : None
 *****************************************************************************/
void App_PortsDevicesEnable (void)
{

   /* Enable all ports and devices */
   status = WP_PortEnable (h_port_enet1, WP_DIRECTION_DUPLEX);
   App_TerminateOnError (status, "WP_PortEnable() Enet1");

   status = WP_DeviceEnable (h_device_enet1, WP_DIRECTION_DUPLEX);
   App_TerminateOnError (status, "WP_DeviceEnable() Enet1");

   status = WP_PortEnable (h_port_enet2, WP_DIRECTION_DUPLEX);
   App_TerminateOnError (status, "WP_PortEnable() Enet2");

   status = WP_DeviceEnable (h_device_enet2, WP_DIRECTION_DUPLEX);
   App_TerminateOnError (status, "WP_DeviceEnable() Enet2");

}

/*****************************************************************************
 * Function name: App_IMADevicesEnable
 * Description  : Enable IMA devices
 * Input  params: None
 * Output params: None
 * Return val   : None
 *****************************************************************************/

void App_IMADevicesEnable (void)
{
   WP_U32 ii;

   for (ii = 0; ii < APP_MAX_IMA_DEVICE; ii++)
   {

      status = WP_DeviceEnable (h_dev_ima[ii], WP_DIRECTION_DUPLEX);
      App_TerminateOnError (status, "WP_DeviceEnable()");
   }

}

/****************************************************************************************************************************
 * Function name: App_enableEmphyPort()
 * Description  :    Enable the EMPHY port
 *
 * Input  params: none
 * Output params: 
 * Return val   : none
 *****************************************************************************************************************************/

void App_enableEmphyPort (void)
{
   /* Enable the EMPHY port */
   status = WP_PortEnable (emphy_port, WP_DIRECTION_DUPLEX);
   App_TerminateOnError (status, "WP_PortEnable() emphy_port");
}

/*****************************************************************************
 * Function name: App_ChannelsEnable
 * Description  : Enet, and ATM(IMA) channels are enebled in this function
 * Input  params: None
 * Output params: None
 * Return val   : None
 *****************************************************************************/
void App_ChannelsEnable (void)
{
   WP_U32 ii;

   status = WP_ChannelEnable (h_enet1_rx);
   App_TerminateOnError (status, "WP_ChannelEnable ENET1 RX");

   status = WP_ChannelEnable (h_enet1_tx);
   App_TerminateOnError (status, "WP_ChannelEnable ENET1 TX");

   status = WP_ChannelEnable (h_enet2_rx);
   App_TerminateOnError (status, "WP_ChannelEnable ENET2 RX");

   status = WP_ChannelEnable (h_enet2_tx);
   App_TerminateOnError (status, "WP_ChannelEnable ENET2 TX");

   for (ii = 0; ii < MAX_ATM_TX_CHANNEL; ii++)
   {
      status = WP_ChannelEnable (ima_tx_ch_handle[ii]);
      App_TerminateOnError (status, "WP_ChannelEnable IMA ATM TX");
   }

   for (ii = 0; ii < NUM_OF_GROUPS; ii++)
   {
      status = WP_ChannelEnable (ima_rx_ch_handle[ii]);
      App_TerminateOnError (status, "WP_ChannelEnable IMA ATM RX");
   }

}

/*****************************************************************************
 * Function name: App_CreateEnetUfeIwSystem 
 * Description  : Create a Bridging interworking system. EMC Filter is created 
 *     to match on VC label and point to Trans Bridge flow aggregation. In Trans Bridge
 *     flow aggregation, the packet is modified by extracting the ethernet and
 *     MPLS headers and send to the ATM channel. ATM channel in turn points to 
 *     IMA group and traffic is sent over the IMA(emphy) device.
 *
 *     Forwarding and the policing actions are applied on the packets. Two 
 *     channels are created for the demo with different tx_tq values. 
 *
 * Input  params: None
 * Output params: None
 * Return val   : None
 *****************************************************************************/
void App_CreateEnetUfeIwSystem (void)
{
   WP_U32 ii = 0;
   WP_status status;
   WP_dfc_flow_info dfc_flow_info;
   WP_flow_class_rule flow_class_rule;
   WP_CHAR rule_string[128];
   WP_classification_filter classification_filter = { 0 };
   WP_flow_class_forwarding_action forwarding_action;
   WP_iw_agg_bridging iw_agg_bridging_cfg;
   WP_U32 mpls_label_value = 0;

   /* Bridge1 ENET --> UFE */
   iw_sys_bridge.tag = tag_iw_sys_bridge1;
   h_iw_sys_bridge1 = WP_IwSystemCreate (APP_MASTER_WPID,
                                         WP_IW_TRANS_BRIDGING_MODE,
                                         &iw_sys_bridge);
   App_TerminateOnError (h_iw_sys_bridge1, " WP_IwSystemCreate() Bridge1");

   /*  1. DFC filter using MPLS label field */
   classification_filter.fields_mask = WP_FIELD_IDS_ARRAY;
   classification_filter.no_match_action = WP_CLASS_FILTER_NO_MATCH_DENY;

   /* Filter based on MPLS label1 (VC label) */
   classification_filter.field_ids_array[0] = WP_FIELD_ID_MPLS_LABEL1;
   classification_filter.field_ids_array[1] = WP_FIELD_ID_LAST;
   classification_filter.filter_type = WP_CLASS_FILTER_EMC;

   h_bridge1_filter[0] =
      WP_IwClassFilterAdd (h_iw_sys_bridge1, &classification_filter);
   App_TerminateOnError (h_bridge1_filter[0],
                         " WP_IwClassFilterAdd() Bridge1");

   memcpy (&iw_agg_bridging_cfg, &default_iw_agg_bridging_cfg,
           sizeof (WP_iw_agg_bridging));

   /* Create Default Aggregation Bridge1 */
   iw_agg_bridging_cfg.tag = tag_agg_default_bridge1;
   iw_agg_bridging_cfg.txfunc = h_bridge1_iwhost;
   iw_agg_bridging_cfg.input_bport = 0;
   iw_agg_bridging_cfg.intmode = WP_IW_INT_ENABLE;

   h_bridge1_default_agg = WP_IwFlowAggregationCreate (APP_MASTER_WPID,
                                                       WP_IW_TRANS_BRIDGING_MODE,
                                                       &iw_agg_bridging_cfg);
   App_TerminateOnError (h_bridge1_default_agg,
                         " WP_IwFlowAggregationCreate() Default Bridge");
   iw_agg_bridging_cfg.intmode = WP_IW_INT_DISABLE;

   bridge_port_cfg.tag = tag_bport_enet1;
   bridge_port_cfg.flow_agg = h_bridge1_default_agg;
   bridge_port_cfg.input_filters_mask = 0x80000000;
   h_bridge1_bport_enet1 = WP_IwPortCreate (h_iw_sys_bridge1,
                                            &bridge_port_cfg);
   App_TerminateOnError (h_bridge1_bport_enet1, "WP_IwPortCreate()ENET1");

   /* Rx binding ENET 1 */
   rx_binding_bridging_cfg.input_port = h_bridge1_bport_enet1;
   status = WP_IwRxBindingCreate (h_enet1_rx,
                                  h_iw_sys_bridge1,
                                  h_qnode_iwq, &rx_binding_bridging_cfg);
   App_TerminateOnError (status, " WP_IwRxBindingCreate() enet1 ");

   /* create iw port for IMA */
   bridge_port_cfg.tag = tag_bridge1_bport_ima;
   bridge_port_cfg.flow_agg = h_bridge1_default_agg;
   bridge_port_cfg.max_mac_addresses = APP_MAX_MAC_ADDRESS;
   bridge_port_cfg.input_filters_mask = 0;
   bridge_port_cfg.classification_flag =
      WP_IW_IWPORT_CLASSIFICATION_DISABLED;
   h_bridge1_bport_ima =
      WP_IwPortCreate (h_iw_sys_bridge1, &bridge_port_cfg);
   App_TerminateOnError (h_bridge1_bport_ima, "WP_IwPortCreate() IMA");

   memcpy (&iw_agg_bridging_cfg, &default_iw_agg_bridging_cfg,
           sizeof (WP_iw_agg_bridging));
   iw_agg_bridging_cfg.iw_port = h_bridge1_bport_ima;
   iw_agg_bridging_cfg.extraction_length = 18;
   iw_agg_bridging_cfg.prefix_length = 0;
   iw_agg_bridging_cfg.intmode = 0;

   for (ii = 0; ii < MAX_ATM_TX_CHANNEL; ii++)
   {
      iw_agg_bridging_cfg.txfunc = ima_tx_ch_handle[ii];
      h_flow_agg_trans_bridge_egress[ii] =
         WP_IwFlowAggregationCreate (APP_MASTER_WPID,
                                     WP_IW_TRANS_BRIDGING_MODE,
                                     &iw_agg_bridging_cfg);
      App_TerminateOnError (h_flow_agg_trans_bridge_egress[ii],
                            " WP_IwFlowAggregationCreate() IMA ");
   }

   /* Add DFC flows */
   memset (&flow_class_rule, 0, sizeof (WP_flow_class_rule));
   /* Add DFC flow for MPLS packet flow through IMA */
   dfc_flow_info.filter_handle = h_bridge1_filter[0];
   dfc_flow_info.input_port = h_bridge1_bport_enet1;
   dfc_flow_info.output_port = 0;

   for (ii = 0; ii < MAX_ATM_TX_CHANNEL; ii++)
   {
      forwarding_action.flow_aggregation =
         h_flow_agg_trans_bridge_egress[ii];
      /* MPLS field for classification */
      mpls_label_value = 19 + ii;   /* MPLS label 20 bits :: This value is configurable */
      sprintf (rule_string,
               "n;n;n;n;n;n;n;n;n;n;n;n;n;n;n;n;n;n;n;n;n;n;n;n;n;n;n;n;n;n;n;n;n;n;n;n;n;n;n;n;0x%x;n",
               mpls_label_value);
      /* position           0;1;2;3;4;5;6;7;8;9;0;1;2;3;4;5;6;7;8;9;0;1;2;3;4;5;6;7;8;9;0;1;2;3;4;5;6;7;8;9;0 ;1;2 */

      flow_class_rule.match_action = WP_FLOW_CLASS_MODE_FLOW_AGG;
      flow_class_rule.action_info.forwarding_action = forwarding_action;
      flow_class_rule.classifier_string = rule_string;
      flow_class_rule.dfc_info = &dfc_flow_info;

      h_bridge1_dfcflow1[ii] =
         WP_IwFlowAdd (h_iw_sys_bridge1, WP_FLOW_CLASS_RULE,
                       &flow_class_rule);
      App_TerminateOnError (h_bridge1_dfcflow1[ii],
                            " WP_IwFlowAdd() DFC Bridge1 rule1");
   }

   status = WP_IwSystemBuild (h_iw_sys_bridge1);
   App_TerminateOnError (status, " WP_IwSystemBuild() Bridge1");
}

/*****************************************************************************
 * Function name: App_CreateUfeEnetIwSystem
 * Description  : function creates a Direct Map interworking systems. ATM PWE3 
 *     flow aggregation is created in the INGRESS direction. ATM Rx Channel 
 *     binding is done with this flow aggregation. Ethernet and MPLS headers 
 *     are added to the packet and send to Enet2.  
 * Input  params: None
 * Output params: None
 * Return val   : None
 *****************************************************************************/
void App_CreateUfeEnetIwSystem (void)
{
   WP_U32 ii = 0, jj = 0;
   WP_status status;
   WP_U8 dummy_prefix_header[18];

   /* Dummy mac addresses used to send into Bridge2 */
   for (jj = 0; jj < 12; jj++)
   {
      dummy_prefix_header[jj] = jj;
   }
   dummy_prefix_header[12] = 0x88;  /* eth type for MPLS (2 bytes) */
   dummy_prefix_header[13] = 0x47;

   dummy_prefix_header[14] = 0x00;  /* mpls header (4 bytes)  */
   dummy_prefix_header[15] = 0x01;
   dummy_prefix_header[16] = 0x31;
   dummy_prefix_header[17] = 0x64;

   /* Directmap System */
   h_iw_sys_directmap =
      WP_IwSystemCreate (WP_WINPATH (0), WP_IW_DIRECTMAP_MODE,
                         &iw_sys_directmap_cfg);
   App_TerminateOnError (h_iw_sys_directmap,
                         "WP_IwSystemCreate() directmap");

   /* Create flow aggregations that send the packet out on ATM channel */
   pwe3_atm_flow_agg_config->direction = WP_PWE3_INGRESS;
   pwe3_atm_flow_agg_config->txfunc = h_enet2_tx;
   pwe3_atm_flow_agg_config->prefix_length = 18;
   /* Ethernet and MPLS headers are added */
   memcpy (pwe3_atm_flow_agg_config->prefix_header, dummy_prefix_header,
           18);
   h_flow_agg_pwe3_atm_ingress[0] =
      WP_IwFlowAggregationCreate (APP_MASTER_WPID, WP_IW_PWE3_MODE,
                                  pwe3_atm_flow_agg_config);
   App_TerminateOnError (h_flow_agg_pwe3_atm_ingress[0],
                         " WP_IwFlowAggregationCreate() PWE3 ATM ");

   rx_binding_directmap_cfg.default_aggregation =
      h_flow_agg_pwe3_atm_ingress[0];
   for (ii = 0; ii < NUM_OF_GROUPS; ii++)
   {
      status = WP_IwRxBindingCreate (ima_rx_ch_handle[ii],
                                     h_iw_sys_directmap,
                                     ima_qn, &rx_binding_directmap_cfg);
      App_TerminateOnError (status,
                            "WP_IwRxBindingCreate rx_aal5_channel");
   }
   /* build the system */
   status = WP_IwSystemBuild (h_iw_sys_directmap);
   App_TerminateOnError (status, " WP_IwSystemBuild() DirectMap");
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
      App_DataSend (h_enet1_tx, h_pool_buffer_iw);
   }

   WP_Delay (5000000);
   /*Check Statistics */
   App_CheckStats (&result);

   if (result == WP_OK)
   {
      printf ("test passed!\n");
   }
   else
   {
      printf ("test failed!\n");
   }

   printf ("This is the last test case in this application!\n");
   printf ("This test case DO NOT need further manually test!\n");

   /*Quit test */
   App_Quit (1);

}
