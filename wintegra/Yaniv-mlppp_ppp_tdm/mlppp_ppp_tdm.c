/*****************************************************************************
 * (C) Copyright PMC - WIN division (Wintegra) 2012.  All rights reserved.
 * PMC-Sierra (WINTEGRA) CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of PMC-Sierra (Wintegra).
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/
 
 /*****************************************************************************
 *  Appplication: MLPPP over IP with QoS
 *  File Name   : mlppp_ppp_tdm.c
 *
 *****************************************************************************/
/******************************************************************************
 *  Description:
 *
 *   Bridging System (bridge1) between ENET ---> PPP/MLPPP
 *   PPPSwitching + Bridge System (bridge2) between PPP/MLPPP and ENET
 *
 * Traffic
 *
 *    Bridge1: 
 *    Host sends a packet on ENET1 (port is in internal loopback mode)
 *    The packet is received into Bridge1. A DFC classifier rule based on VLAN + DEST ETHER MAC is applied that 
 *    forwards the PACKET either to TDM port for PPP or to MLPPP bundle for MLPPP flow. TDM 1 was arbitrarily  
 *    picked, it could have been any TDM. 
 *
 *    Note that the flow aggregation that forwards the packet on TDM(PPP)/MLPPPbundles,strips off the Ethernet 
 *    header and prefixes a 4 byte PPP header to the packet before sending it out on the TDM (ppp_prefix_header).
 *    If you want to change the ppp_prefix_header, you will have to recompile the test. ppp_prefix_header vlaues
 *    can be configured to 0xff03c021 (LCP for PPP only), 0xff038021 (IPCP for MLPPP) and 0xff030021 (for traffic).
 *    It is possible to configure any other values but to receive in the Enet2 side, the rule string needs to be 
 *    modified accordingly.
 *
 *    The TDM port is in loopback mode so the packet is received back into the PPP Switching System.
 *
 *    PPP Switch:
 *    NOTE: Address and Protocol Field compression are enabled on the PPP SWITCHING RX BINDING. So a packet that 
 *          starts with the PPP header will have AFC/PFC applied to it. 
 *
 *    So 0xFF03C021 is received back into the PPP Switching system as 0xC021 after applying AFC and PFC( Now not in use in this application)
 *    So 0xFF038021 is received back into the PPP Switching system as 0x8021 after applying AFC and PFC
 *    So 0xFF030021 is received back into the PPP Switching system as 0x21 after applying AFC and PFC
 *
 *    The PPP SWITCHING default flow aggregation (1 per TDM port) adds a dummy 18 byte Ethernet header(including VLAN header) and forwards the packet to Bridge2
 *
 *    Bridge2:
 *    In Bridge2 we 2 user defined filters that look for a user defined field starting at the ETHER_TYPE reference point of the packet.
 *
 *    The two filters (Filter0, Filter1) are used 
 *
 *    One DFC rule is added for Filter0:  Based on VLAN tag
 *    (1) For MLPPP IPCP packets (0x8021). If Vlan tag is 0xb , This packet is sent to a flow agg named h_flow_agg_mlppp_control and host terminated
 *
 *    One rule is added for Filter1: 
 *    (1) For MLPPP-IP DATA packets (0x21).If Vlan tag is 0xa This packet is sent to the flow agg named h_flow_agg_enet2_mlppp and sent out on ENET2
 *    
 *
 *    Since ENET2 is in normal mode, all packets sent is not printed. Use ENET2 statistics to check the packet is sent or not
 *
 *    Finally look at App_Debug that toggles the debug mode for Bridge1 to ether host terminate packets or forward packets on the TDM
 *
 ************************************************************************************************************************/    
 /**********************************  QoS for MLPPP  ********************************************************************
 *  Egress: 
 *  There are four channels created over the MLPPP bundle. These channels are prioritized through a PQblock.
 *  Filter is created with DSCP so that the classification can be done based on the priority set in the DSCP.
 *  Here the priority is set in order like 0,1,2 and 3. To modify the order, the code needs to be modified 
 *  and compiled. QoS action is based on the PRIORITY_MAPPING, PRIORITY_REMARKING and POLICING.
 *  
 *  Weighter Fair Queue scheduling (WFQ) values can be changed from the structure wfq[1] (mlppp_ppp_tdm.h file). 
 *  
 *  Ingress:
 *  In the ingress side the packet in the Bridge2 does not have the MAC address hence can not use the DSCP based
 *  classification. Instead of DSCP based classification, User programmable field is used for the classification
 *  Four channels are created over the Enet2 device. The priority is set in tx_tq. Here also the QoS action is 
 *  based on the PRIORITY_MAPPING, PRIORITY_REMARKING. The priority is done over the VLAN.
 *  
 ************************************************************************************************************************/

#define APP_DEBUG 0

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


#include "mlppp_ppp_tdm.h"
#include "mlppp_ppp_tdm_util.c"
#include "mlppp_ppp_tdm_statistics.c"

void reset_serdes (void);
WP_status WPX_HwSerdesResetX (WP_U32 wpid, WP_U32 port);
WP_status WPX_BoardSerdesReset(WP_U32 wpid, WP_port port);

/****************************************************************************************************************************
* Function name: main()
* Description  : Main function of the application. Calls all the functions.
*
* Input  params: none
* Output params: 
* Return val   : none
*****************************************************************************************************************************/

int main(int argc, char *argv[])
{

   WP_status status;
   app_task *task, a_task;
   WP_CHAR comm;

   if (argc <= 1)
   {
       freerun = 0;
   }
   else
   if ((argc == 3) && (!strcmp(argv[1], "freerun")))
   {
       freerun = 1;
       interface_ch = CONTROL_DATA;
   }
   else
   {
       freerun = -1;
       printf("Wrong parameter! Only \"go\" or \"go freerun\" supported!");
       /*Quit test*/
       exit(0);       
   }

   status = WP_DriverInit();
   App_TerminateOnError(status,"WP_DriverInit()");

   /*initializes various tags and Context Configurations used in this test */
   App_TestInit();
   
   status = WP_SysInit(APP_MASTER_WPID, &context_cfg);
   App_TerminateOnError(status, "WP_SysInit()");

   /*Initializes Enet Physical ports and create/initializes the clocks and TDM card*/
   App_InitHW();

   /*Creates the enet, TDM ports and devices. Pools and Qnodes are created in this function. MLPPP system also        
     created in this function*/
   App_SystemSetup();
   
   /* Commit system resources */
   status = WP_SysCommit();
   App_TerminateOnError(status,"WP_SysCommit()");

#if DISABLE_TDM_COMET_CARD 
   /*Creates host, enet, TDM(hdlc) and MLPPP(hdlc) channels. MLPPP bundle is created in this function. 
     PQblock and associated MLPPP channels are created in this function  */
   App_ChannelsCreate();

   /* Sets up ENET-->HDLC interworking */
   App_CreateEnetHdlcBridge(); 

   /* Sets up HDLC-->ENET interworking */
   App_CreateHdlcEnetBridge(); 

   status = WP_SysPsuEnable(WP_WINPATH(0));
   App_TerminateOnError(status, "WP_SysPsuEnable()");
#endif

   /* Enet, TDM ports are enebled in the function*/
   App_PortsDevicesEnable();

/*-------------------------------------------------------*\
\*-------------------------------------------------------*/
#if 0
   WPX_BoardSerdesReset (WP_WINPATH(0), WP_PORT_ENET7);
#endif

   /*Enables the Enet, host and MLPPP channels created earlier	*/
   App_ChannelsEnable();

   if (0 == freerun)
   {
      /*default mode, no parameters*/
      while(1)
      {
         printf("\n");
         printf("a. Stats\n");
         /*printf("d. Debug\n");*/ /*Not in use now given for future support*/
         printf("p. Send Packet\n");
         /*printf("s. Simulate Interrupts\n");*/ /*Not in use now given for future support*/
         printf("k. Exit without release\n");
         printf("r. Reboot\n");
         printf("x. Exit\n");
         // comm = getchar();
         comm = 'k';
         switch(comm)
         {
            case 'a':
               App_ShowStats();
               break;
            case 'p':
               App_DataSend(h_enet1_tx, h_pool_buffer_iw);
               break;
            case 'k':
               exit (0);
               break;
            case 'r':
               WT_Reboot ();
               break;
            case 'x':
            {
               WP_DriverRelease();
               exit(0);
            }
         
               /*Not in use now given for future support*/
               /*case 'd':
                 App_Debug(debug_on);
                 break;*/

               /*Not in use now given for future support*/
               /*case 's':
                 WPI_SimulateInterrupts();
                 break;*/
         }
         WPI_SimulateInterrupts();
         while((task = next_task(irq_task_list, &a_task)) != NULL)
            app_perform_action(task);
      }
   }
   else
   if (1 == freerun)
   {
       /*free run mode*/
       App_perform_freerun();    
   }
   
   return 0;
}

/****************************************************************************************************************************
* Function name: App_TestInit()
* Description  :  App_TestInit function initializes various tags and Context Configurations  
*  			used in this test    
*
* Input  params: none
* Output params: 
* Return val   : none
*****************************************************************************************************************************/

void App_TestInit(void)
{
   WP_U32 ii;

   /*NOTE:Tag values are not so important but it must be unique*/
   tag_agg_enet= 500;

   tag_iw_sys_bridge1 = 600;
   tag_iw_sys_bridge2 = 601;

   tag_bport_enet1 = 700;
   tag_bport_enet2 = 701;
   for(ii = 0;ii < APP_MAX_TDM_PORTS;ii++)
   {
      tag_bridge1_bport_tdm[ii] = 802 + ii;
      tag_bridge2_bport_tdm[ii] = 902 + ii;
   }

   for(ii = 0;ii < APP_MAX_TDM_PORTS;ii++)
   {
      tag_agg_tdm[ii] = 1100 + ii;
      tag_agg_pppsw[ii] = 1200 + ii;
   }

   tag_agg_ppp_lcp = 1250;
   tag_agg_ppp_traffic = 1251;

   tag_agg_enet2 = 1300;

   tag_agg_default_bridge1=1400;
   tag_agg_default_bridge2=1500;

   tag_enet1_rx = 1600;
   tag_enet1_tx = 1700;

   tag_enet2_rx = 1800;
   tag_enet2_tx = 1900;

   /* Tag value assignment for the MLPPP tags */
   tag_bridge1_bport_mlppp   = 802+ APP_MAX_TDM_PORTS;
   tag_bridge2_bport_mlppp   = 902+ APP_MAX_TDM_PORTS;
   tag_agg_mlppp             = 1100 + APP_MAX_TDM_PORTS;
   tag_agg_pppsw_mlppp       = 1200 + APP_MAX_TDM_PORTS ;
   tag_agg_eth2bridge_mlppp  =  1252;
   tag_agg_mlppp_control     = 1252;
   tag_agg_mlppp_traffic     = 1253;
   for(ii=0;ii<PQBLOCK_SIZE;ii++)
      tag_enet2_tx_mlppp[ii] = 1900+ii+1;

   /* Context configuration */
   context_cfg.int_queue_data = &int_queue_table_cfg;

   context_cfg.iw_data = &iw_global_cfg;

   context_cfg.atm_data = &atm_global_cfg;
   context_cfg.pool_limit = 8;
   context_cfg.max_qnodes = 4;
   //interface_ch=0;
}

#define MY_SERDES_IO_CTRL_1   (0x1e00829c + 0xA0000000)
#define MY_SHORT_TIME   (100000)

void reset_serdes (void)
{
   WP_U32 serdes_io_ctrl_1 = 0;
   volatile WP_U32 *p = NULL;

   WP_Delay (MY_SHORT_TIME);
   p = (WP_U32*)MY_SERDES_IO_CTRL_1   ;
   serdes_io_ctrl_1 =  *p;
   serdes_io_ctrl_1 &= ~0x00000800;
   *p = serdes_io_ctrl_1;

   WP_Delay (MY_SHORT_TIME);
   serdes_io_ctrl_1 |= 0x00000800;
   *p = serdes_io_ctrl_1;
   printf ("reset_serdes(): complete\n");
}

/****************************************************************************************************************************
 * Function name: App_InitHW()
 * Description  :  App_InitHW function initializes Enet Physical ports and create/initializes the clocks. 
 * 				TDM card is initialized in this function       
 *
 * Input  params: none
 * Output params: 
 * Return val   : none
 *****************************************************************************************************************************/

void App_InitHW(void)
{
   WP_U32 ii;
  
   ii = 0; 
   status = WPX_BoardConfigure(0, WPX_CONFIGURE_CHECKIN_TDI_16);
   App_TerminateOnError(status, "WPX_CONFIGURE_2UPI_1XGI_10SGMII()");

#if 0 
#error SERDES LOOPBACK MUST BE SET
   status = WPX_BoardSerdesInit(0, WP_PORT_ENET7, WPX_SERDES_LOOPBACK); //WPX_SERDES_LOOPBACK- Loopback on Fast Enet
#else
   status = WPX_BoardSerdesInit(0, WP_PORT_ENET7, WPX_SERDES_NORMAL); //WPX_SERDES_LOOPBACK- Loopback on Fast Enet

#endif

   /*NOTE: Must disable loopback from device config else loopback will not work*/
   App_TerminateOnError(status, "WPX_BoardSerdesInit 7()");

   status = WPX_BoardSerdesInit(0, WP_PORT_ENET8, WPX_SERDES_NORMAL);
   App_TerminateOnError(status, "WPX_BoardSerdesInit 8()");




   status = WP_ModuleInit(0, WP_WDDI_MODULE_POLICER, policer);
   App_TerminateOnError(status, "WP_ModuleInit");

   /* Create BRG1 & BRG3 */
   status = WP_SysClockCreate(WP_WINPATH(0), WP_BRG1, WP_BRG_SRC_BRGIN2, 2);
   status = WP_SysClockCreate(WP_WINPATH(0), WP_BRG2, WP_BRG_SRC_BRGIN2, 2);
   status = WP_SysClockCreate(WP_WINPATH(0), WP_BRG3, WP_BRG_SRC_BRGIN2, 2);
   status = WP_SysClockCreate(WP_WINPATH(0), WP_BRG4, WP_BRG_SRC_BRGIN2, 2);
   status = WP_SysClockCreate(WP_WINPATH(0), WP_BRG5, WP_BRG_SRC_BRGIN2, 2);
#if DISABLE_TDM_COMET_CARD
   status = WPU_TdmCometCardInit(WP_PORT_TDM1, WPU_INIT_COMET_CPLD_T1);
   App_TerminateOnError(status, "WPU_TdmCometCardInit 1");

   for(ii = 0;ii < APP_MAX_TDM_PORTS;ii++)
   {
      status = WPU_TdmCometCardInit(WP_PORT_TDM1 + ii, WPU_MODE_TDM_T1_MASTER);
      App_TerminateOnError(status, "WPU_TdmCometCardInit() 2");
   }
#endif

}


/****************************************************************************************************************************
* Function name: App_SystemSetup()
* Description  :  App_SystemSetup function creates the enet, TDM ports and devices.        					
*  			Pools and Qnodes are created in this function. MLPPP system also        
*  			created in this function (before System Commit). After all the          
* 			necessary resources are created the system is committed using the       
*  			funtions WP_SysCommit()         
*
* Input  params: none
* Output params: 
* Return val   : none
*****************************************************************************************************************************/

void App_SystemSetup(void)
{
   WP_U32 ii; 

#if 1
   /* Create Enet1 port */
   h_port_enet1 = WP_PortCreate(APP_MASTER_WPID, WP_PORT_ENET7,
                                &port_enet_cfg);
#else
   h_port_enet1 = WP_PortCreate(APP_MASTER_WPID, WP_PORT_ENET3,
                                &port_enet_cfg);
#endif
   ii = 0;
   App_TerminateOnError(h_port_enet1, "WP_PortCreate() Enet1");

   /* Create device on Enet1 */
   MAC_COPY(device_enet_cfg.mac_addr, wt_mac_enet7);
   h_device_enet1 = WP_DeviceCreate(h_port_enet1, WP_PHY(0), WP_DEVICE_ENET,
                                    &device_enet_cfg);
   App_TerminateOnError(h_device_enet1, "WP_DeviceCreate() Enet1");

   /* Create Enet2 port */
   h_port_enet2 = WP_PortCreate(APP_MASTER_WPID, WP_PORT_ENET8,
                                &port_enet_cfg);
   App_TerminateOnError(h_port_enet2, "WP_PortCreate() Enet2");


   /* To increase the number of channels in the enet2, modify here */
   /* Note : pkt_limits is set to 8 in the port configuration      */
   device_enet_cfg.max_tx_channels= 5; 
   MAC_COPY(device_enet_cfg.mac_addr, wt_mac_enet8);
   h_device_enet2 = WP_DeviceCreate(h_port_enet2, WP_PHY(0), WP_DEVICE_ENET,
                                    &device_enet_cfg);
   App_TerminateOnError(h_device_enet2, "WP_DeviceCreate() Enet2");

#if DISABLE_TDM_COMET_CARD
   /* Create TDM ports and devices */
   for(ii = 0;ii < APP_MAX_TDM_PORTS;ii++)
   {
      //port_tdm_cfg.tdmmode = WP_TDM_LOOPBACK;
      h_port_tdm[ii] = WP_PortCreate(WP_WINPATH(0), WP_PORT_TDM1 + ii, &port_tdm_cfg);
      App_TerminateOnError(h_port_tdm[ii],"WP_PortCreate() TDM1");

      h_dev_tdm[ii] = WP_DeviceCreate(h_port_tdm[ii], WP_PHY(0), WP_DEVICE_HDLC,
                                      &device_tdm_hdlc_cfg);
      App_TerminateOnError(h_dev_tdm[ii],"WP_DeviceCreate() TDM1");
   }
#endif

   /* Create an IW Host port */
   h_port_iwhost = WP_PortCreate(WP_WINPATH(0), WP_PORT_IW_HOST, NULL);
   App_TerminateOnError(h_port_iwhost, "WP_PortCreate() IW Host");

   /* Create an IW Host Device */
   h_dev_iwhost = WP_DeviceCreate(h_port_iwhost, 0, WP_DEVICE_IW_HOST, NULL);
   App_TerminateOnError(h_dev_iwhost, "WP_DeviceCreate() IW Host");

   /* Create Buffer pools */
   h_pool_buffer_iw = WP_PoolCreate(APP_MASTER_WPID, WP_pool_iwbuffer,
                                    &pool_buffer_data_iw_cfg);
   App_TerminateOnError(h_pool_buffer_iw, "WP_PoolCreate iwbuffer iw");


   /* Pool for host termination buffers */
   h_pool_256 = WP_PoolCreate(WP_WINPATH(0), WP_pool_buffer,
                              &pool_buffer_data_256_cfg);
   App_TerminateOnError (h_pool_256, "PoolCreate() pool_256");

   h_pool_ring_host = WP_PoolCreate(WP_WINPATH(0), WP_pool_ring,
	                            &pool_ring_data_cfg);
   App_TerminateOnError(h_pool_ring_host, "PoolCreate() pool_ring_host");

   /* Create an interworking queue node */
   qnode_iwq_cfg.adjunct_pool = h_pool_buffer_iw;
   h_qnode_iwq = WP_QNodeCreate(APP_MASTER_WPID,WP_QNODE_IWQ | WP_QNODE_OPT_DEDICATED_RX_HWQ | WP_QNODE_OPT_FMU,
                                &qnode_iwq_cfg);
   App_TerminateOnError(h_qnode_iwq, "WP_QNodeCreate() h_qnode_iwq");

   /* Create an interworking queue node */
   qnode_iwq_cfg.adjunct_pool = h_pool_buffer_iw;
   h_qnode_iwq_tdm = WP_QNodeCreate(APP_MASTER_WPID,WP_QNODE_IWQ, &qnode_iwq_cfg);
   App_TerminateOnError(h_qnode_iwq_tdm, "WP_QNodeCreate() h_qnode_iwq_tdm");

   /* Host termination qnode */
   qnode_hostq_cfg.pool_buffer = h_pool_256;
   qnode_hostq_cfg.pool_ring = h_pool_ring_host;
   h_qnode_host = WP_QNodeCreate(WP_WINPATH(0), WP_QNODE_HOSTQ, &qnode_hostq_cfg);
   App_TerminateOnError(h_qnode_host, "QnodeCreate() h_qnode_mp");

   status = WP_ControlRegister(WP_EVENT_RX_INDICATE, App_EventRxIndicate);
   App_TerminateOnError(status, "WP_ControlRegister()");

   status = WP_ControlRegister(WP_EVENT_TX_INDICATE, App_EventTxIndicate);
   App_TerminateOnError(status, "WP_ControlRegister()");

   status = WP_ControlRegister(WP_EVENT_STATUS_INDICATE, App_EventErrorIndicate);
   App_TerminateOnError(status, "WP_ControlRegister()");

   status = WP_SysSchedulerCreate(WP_WINPATH(0), cw_config);
   App_TerminateOnError(status, "WP_SysSchedulerCreate()");

   /*   MLPPP system create      */
   mlppp_sys_handle = WP_MpSystemCreate(WP_WINPATH(0), mp_sys_cfg);
   App_TerminateOnError(mlppp_sys_handle, "WP_MpSystemCreate()");
   qnode_iwq_cfg.adjunct_pool = h_pool_buffer_iw;

   /* Qnode for MPBundleCreate */
   mlppp_qn = WP_QNodeCreate(WP_WINPATH(0),WP_QNODE_MLPPP, &qnode_iwq_cfg);
   App_TerminateOnError(mlppp_qn, "WP_QNodeCreate() mlppp_qn");
}

               
/****************************************************************************************************************************
* Function name: App_ChannelsCreate()
* Description  : App_ChannelsCreate function creates host, enet, TDM(hdlc) and MLPPP(hdlc) channels.    
*  		 MLPPP bundle is created in this function. PQblock and associated MLPPP channels are    
*  		 created in this function.
*
* Input  params: none
* Output params: 
* Return val   : none
*****************************************************************************************************************************/

void App_ChannelsCreate(void)
{
   WP_U32 ii;
   WP_U32 tag1,tag2;

   /* Create Enet channels */
   h_enet1_rx = WP_ChannelCreate(tag_enet1_rx, h_device_enet1,
                                 h_qnode_iwq,WP_CH_RX, WP_ENET,
                                 &ch_enet_cfg);
   App_TerminateOnError(h_enet1_rx, "WP_ChannelCreate() Enet Rx");

   /* NOTE this channel is in NON-IW mode, used to send out a packet from the host out of ENET7 */
   ch_enet_cfg.iwmode = WP_PKTCH_IWM_DISABLE;
   h_enet1_tx = WP_ChannelCreate(tag_enet1_tx++, h_device_enet1,
                                 h_qnode_iwq,WP_CH_TX, WP_ENET,
                                 &ch_enet_cfg);
   App_TerminateOnError(h_enet1_tx, "WP_ChannelCreate() Enet Tx");

   status = WP_IwTxBindingCreate(h_enet1_tx, WP_IW_TX_BINDING, &tx_binding_cfg);
   App_TerminateOnError(h_enet1_tx, "WP_IwTxBindingCreate()");

   /* Create Enet 2 channels */
   ch_enet_cfg.iwmode = WP_PKTCH_IWM_DISABLE;
   h_enet2_rx = WP_ChannelCreate(tag_enet2_rx, h_device_enet2,
                                 h_qnode_iwq,WP_CH_RX, WP_ENET,
                                 &ch_enet_cfg);
   App_TerminateOnError(h_enet2_rx, "WP_ChannelCreate() Enet Rx");

   ch_enet_cfg.iwmode = WP_PKTCH_IWM_ENABLE;
   ch_enet_cfg.tx_tq = 4;  // 0 to 3 are used for MLPPP
   h_enet2_tx = WP_ChannelCreate(tag_enet2_tx, h_device_enet2,
                                 h_qnode_iwq,WP_CH_TX, WP_ENET,
                                 &ch_enet_cfg);
   App_TerminateOnError(h_enet2_tx, "WP_ChannelCreate() Enet Tx");
   /*  4 channels create over the enet2 device */
   for(ii =0; ii<PQBLOCK_SIZE; ii++)
   {
      ch_enet_cfg.tx_tq = ii;  // 4 is already used by h_enet2_tx 
      h_enet2_tx_mlppp[ii] = WP_ChannelCreate(tag_enet2_tx_mlppp[ii], h_device_enet2,
                                              h_qnode_iwq,WP_CH_TX, WP_ENET,
                                              &ch_enet_cfg);
      App_TerminateOnError(h_enet2_tx_mlppp[ii], "WP_ChannelCreate() Enet Tx for MLPPP with QoS");

      status = WP_IwTxBindingCreate(h_enet2_tx_mlppp[ii], WP_IW_TX_BINDING, &tx_binding_cfg);
      App_TerminateOnError(status, "WP_IwTxBindingCreate()");
   }

   status = WP_IwTxBindingCreate(h_enet2_tx, WP_IW_TX_BINDING, &tx_binding_cfg);
   App_TerminateOnError(status, "WP_IwTxBindingCreate()");

   tag1 = tag_tdmrx;
   tag2 = tag_tdmtx;


#if DISABLE_TDM_COMET_CARD 
   /* Create TDM RX and TX Channels */
   for(ii = 0;ii < APP_MAX_TDM_PORTS;ii++)
   {
      ch_hdlc_cfg.intmode = WP_PKTCH_INT_ENABLE;
      h_tdm_rx[ii] = WP_ChannelCreate(tag1++, h_dev_tdm[ii], h_qnode_host,
                                      WP_CH_RX, WP_HDLC, &ch_hdlc_cfg);
      App_TerminateOnError(h_tdm_rx[ii], "WP_ChannelCreate() h_tdm_rx");

      ch_hdlc_cfg.intmode = WP_PKTCH_INT_DISABLE;
      h_tdm_tx[ii] = WP_ChannelCreate(tag2++, h_dev_tdm[ii], h_qnode_iwq_tdm,
                                      WP_CH_TX, WP_HDLC, &ch_hdlc_cfg);
      App_TerminateOnError(h_tdm_tx[ii], "WP_ChannelCreate() h_tdm_tx1");

      status = WP_IwTxBindingCreate(h_tdm_tx[ii], WP_IW_TX_BINDING, &tx_binding_cfg);
      App_TerminateOnError(status, "WP_IwTxBindingCreate()");
   }
#endif

   /* MLPPP bundle create */
   mp_config->qn_mp = mlppp_qn;
   mlppp_group_handle[0] = WP_MpBundleCreate (mlppp_sys_handle, mp_config);
   App_TerminateOnError(mlppp_group_handle[0], "WP_MpBundleCreate()");
   
   /* PQ Block creation for the MLPPP QoS testing */
   mlppp_bundle_pqblock_handle[0] = WP_IwPQBlockCreate(mlppp_sys_handle,WP_HDLC);
   App_TerminateOnError(mlppp_bundle_pqblock_handle[0], "WP_PQBlockCreate MP");
   ch_config_hdlc[0].tx_pqblock = mlppp_bundle_pqblock_handle[0];
   ch_config_hdlc[0].iwmode = WP_PKTCH_IWM_ENABLE;
   for(ii=0;ii<PQBLOCK_SIZE; ii++)
   {
      ch_config_hdlc[0].tx_pqlevel = ii;    /* priority queue level */
      ch_config_hdlc[0].tx_tq = ii;
      mlppp_tx_ch_handle[ii] = WP_ChannelCreate(0, mlppp_group_handle[0], h_qnode_iwq_tdm,
                                                WP_CH_TX, WP_HDLC, &ch_config_hdlc[0]);
      App_TerminateOnError(mlppp_tx_ch_handle[ii], "WP_ChannelCreate() MP Tx");
      /* tx binding for MLPPP channel */
      status =  WP_IwTxBindingCreate(mlppp_tx_ch_handle[ii], WP_IW_TX_BINDING, &tx_binding_cfg);
      App_TerminateOnError(status, "WP_IwTxBindingCreate()");
   }
   mlppp_rx_ch_handle = WP_ChannelCreate(0, mlppp_group_handle[0], h_qnode_host, 
                                         WP_CH_RX, WP_HDLC, &ch_config_hdlc[0]);
   App_TerminateOnError(mlppp_rx_ch_handle, "WP_ChannelCreate() MP Rx");

   /* Crate IW HOST termination channels one for each bridge */
   ch_iw_rx_cfg.tx_binding_config = &tx_binding_cfg;
   h_bridge1_iwhost = WP_ChannelCreate(90, h_dev_iwhost, h_qnode_iwq,
                                       WP_CH_RX, WP_IW_HOST, &ch_iw_rx_cfg);
   App_TerminateOnError(h_bridge1_iwhost, "WP_ChannelCreate() Host atm");

   h_bridge2_iwhost = WP_ChannelCreate(91,h_dev_iwhost, h_qnode_iwq,
                                       WP_CH_RX,WP_IW_HOST, &ch_iw_rx_cfg);
   App_TerminateOnError(h_bridge2_iwhost, "WP_ChannelCreate() Host atm");

}

/****************************************************************************************************************************
* Function name: App_PortsDevicesEnable()
* Description  : Enet, TDM ports are enebled in the function App_PortsDevicesEnable. This function adds links to the *
*                MLPPP bundle created earlier (this enables the links as well).  MLPPP bundle is enabled with a      *
*                configuration of DUPLEX mode so that the same bundle can be used for Tx and Rx channels     	   
*
* Input  params: none
* Output params: 
* Return val   : none
*****************************************************************************************************************************/

void App_PortsDevicesEnable(void)
{
   
   WP_U32 ii;
   ii = 0;   
   /* Enable all ports and devices */
   status = WP_PortEnable(h_port_enet1, WP_DIRECTION_DUPLEX);
   App_TerminateOnError(status, "WP_PortEnable() Enet1");
   
   status = WP_DeviceEnable(h_device_enet1, WP_DIRECTION_DUPLEX);
   App_TerminateOnError(status, "WP_DeviceEnable() Enet1");
   
   status = WP_PortEnable(h_port_enet2, WP_DIRECTION_DUPLEX);
   App_TerminateOnError(status, "WP_PortEnable() Enet2");

   status = WP_DeviceEnable(h_device_enet2, WP_DIRECTION_DUPLEX);
   App_TerminateOnError(status, "WP_DeviceEnable() Enet2");
   
#if DISABLE_TDM_COMET_CARD 
   for(ii = 0;ii < APP_MAX_TDM_PORTS;ii++)
   {
      status = WP_PortEnable(h_port_tdm[ii], WP_DIRECTION_DUPLEX);
      App_TerminateOnError(status,"WP_PortEnable() TDM ");
      
      status = WP_DeviceEnable(h_dev_tdm[ii], WP_DIRECTION_DUPLEX);
      App_TerminateOnError(status,"WP_DeviceEnable()");
   }
   
   status = WP_MpBundleEnable(mlppp_group_handle[0],WP_DIRECTION_DUPLEX);
   App_TerminateOnError(status, "WP_MpBundleEnable");
   
   /* MLPPP links added here */
   /* MpLinkAdd creates and enables the link:: no need to enable separately */
   mlppp_link_handle[0] = WP_MpLinkAdd(mlppp_group_handle[0],h_dev_tdm[0], mp_link_config);
   App_TerminateOnError(mlppp_link_handle[0], "WP_MpLinkAdd()");
   
   mlppp_link_handle[1] = WP_MpLinkAdd(mlppp_group_handle[0],h_dev_tdm[1], mp_link_config);
   App_TerminateOnError(mlppp_link_handle[1], "WP_MpLinkAdd()");
#endif
   
}


/****************************************************************************************************************************
* Function name: App_ChannelsEnable()
* Description  :   App_ChannelsEnable function enables the Enet, host and MLPPP channels created earlier	   
*
* Input  params: none
* Output params: 
* Return val   : none
*****************************************************************************************************************************/

void App_ChannelsEnable(void)
{
   WP_U32 ii;

   status = WP_ChannelEnable(h_enet1_rx);
   App_TerminateOnError(status, "WP_ChannelEnable ENET1 RX");

   status = WP_ChannelEnable(h_enet1_tx);
   App_TerminateOnError(status, "WP_ChannelEnable ENET1 RX");

   status = WP_ChannelEnable(h_enet2_rx);
   App_TerminateOnError(status, "WP_ChannelEnable ENET2 RX");

   status = WP_ChannelEnable(h_enet2_tx);
   App_TerminateOnError(status, "WP_ChannelEnable ENET2 TX");

   /* Enable the Enet2 channels used for MLPPP traffic flow  */
   for(ii =0; ii<PQBLOCK_SIZE; ii++)
   {
      status = WP_ChannelEnable(h_enet2_tx_mlppp[ii]);
      App_TerminateOnError(status, "WP_ChannelEnable ENET2 TX MLPPP");
   }


#if DISABLE_TDM_COMET_CARD 
   for(ii = 0;ii < APP_MAX_TDM_PORTS;ii++)
   {
      status = WP_ChannelEnable(h_tdm_tx[ii]);
      App_TerminateOnError(status, "WP_ChannelEnable TDM TX");
   }

   /* Enable the MLPPP channel and Bundle  */
   status = WP_ChannelEnable(mlppp_tx_ch_handle[0]);
   App_TerminateOnError(status, "WP_ChannelEnable MLPPP TX");
#endif

}


/****************************************************************************************************************************
 * Function name: App_CreateHdlcEnetBridge()
 * Description  :   App_CreateHdlcEnetBridge function creates a PPP switch and VLAN aware bridge interworking systems.   
 * 		   This function creates the filters and flows in the HDLC to Enet direction  
 *
 * Input  params: none
 * Output params: 
 * Return val   : none
 *****************************************************************************************************************************/

void App_CreateHdlcEnetBridge(void)
{
   WP_U32 ii=0,jj=0;
   WP_status   status;
   WP_U8 dummy_prefix_header[18]; /* dst and src mac + eth type and VLAN */
   WP_dfc_flow_info dfc_flow_info;
   WP_flow_class_rule flow_class_rule;
   WP_CHAR rule_string[128];
   WP_classification_filter classification_filter = {0};
   WP_flow_class_priority_action  priority_action;
   WP_flow_class_ext_action action;
   WP_U8 action_type[3];
   WP_flow_class_action_info action_info[3];
   WP_flow_class_forwarding_action forwarding_action;
   WP_iw_agg_bridging iw_agg_bridging_cfg;
   WP_rx_binding_ppp_switching rx_binding_pppsw;


   /* Dummy mac addresses used to send into Bridge2 */
   for(jj = 0;jj < 12;jj++)
   {
      dummy_prefix_header[jj] = jj;  /* src and dst mac */
   }

   dummy_prefix_header[12] = 0x08;   /* eth type = IP */
   dummy_prefix_header[13] = 0x00;
   dummy_prefix_header[12] = 0x81;
   dummy_prefix_header[13] = 0x00;
   dummy_prefix_header[14] = 0x00;
   dummy_prefix_header[15] = 0x0a;
   dummy_prefix_header[16] = 0x08;   /* eth type = IP */
   dummy_prefix_header[17] = 0x00;

   /*According to interface choice you can add vlan tag here because according to vlan tag bridge2 IW
     will classify the packet and decide where to send packet( ENET device or HOST
     In this application if vlan tag is 0xa then packet will be forwarded to ENET8 device and if vlan tag is 
     0xb then packet will be forwarded to HOST*/
   switch(interface_ch)
   {
      case TRAFFIC_DATA: 
         dummy_prefix_header[15] = 0x0a;
         break;
      case CONTROL_DATA:
         dummy_prefix_header[15] = 0x0b;
         break;
   }

   /* PPP Switching System */
   h_iw_sys_pppsw = WP_IwSystemCreate(WP_WINPATH(0), WP_IW_PPP_SWITCHING_MODE,
                                      &iw_sys_pppsw_cfg);
   App_TerminateOnError(h_iw_sys_pppsw, "WP_IwSystemCreate() pppsw");

   /* Bridge2*/
   iw_sys_bridge.tag = tag_iw_sys_bridge2;
   h_iw_sys_bridge2 = WP_IwSystemCreate(APP_MASTER_WPID,
                                        WP_IW_VLAN_AWARE_BRIDGING_MODE,
                                        &iw_sys_bridge);
   App_TerminateOnError(h_iw_sys_bridge2," WP_IwSystemCreate() Bridge2");

   classification_filter.fields_mask = WP_FIELD_IDS_ARRAY;
   classification_filter.filter_type = WP_CLASS_FILTER_EMC; 
   classification_filter.field_ids_array[0] = WP_FIELD_ID_IN_PORT;
   classification_filter.field_ids_array[1] = WP_FIELD_ID_VLAN_ID;
   classification_filter.field_ids_array[2] = WP_FIELD_ID_LAST;

   classification_filter.no_match_action = WP_CLASS_FILTER_NO_MATCH_CONTINUE; //DENY;
   h_bridge2_filter[0] = WP_IwClassFilterAdd(h_iw_sys_bridge2, &classification_filter);
   App_TerminateOnError(h_bridge2_filter[0]," filter 0: WP_IwClassFilterAdd() Bridge2");

   classification_filter.fields_mask = WP_FIELD_IDS_ARRAY;
   classification_filter.filter_type = WP_CLASS_FILTER_EXT_ACTION_EMC; 
   classification_filter.field_ids_array[0] = WP_FIELD_ID_IN_PORT;
   classification_filter.field_ids_array[1] = WP_FIELD_ID_VLAN_ID;
   classification_filter.field_ids_array[2] = WP_FIELD_ID_IPV4_DSCP;  /* If DSCP does not work use TOS */
   classification_filter.field_ids_array[3] = WP_FIELD_ID_LAST;

   classification_filter.no_match_action = WP_CLASS_FILTER_NO_MATCH_DENY;
   h_bridge2_filter[1] = WP_IwClassFilterAdd(h_iw_sys_bridge2, &classification_filter);
   App_TerminateOnError(h_bridge2_filter[1]," filter 1: WP_IwClassFilterAdd() Bridge2");


   memcpy(&iw_agg_bridging_cfg, &default_iw_agg_bridging_cfg, sizeof(WP_iw_agg_bridging));

   iw_agg_bridging_cfg.intmode = WP_IW_INT_ENABLE;

   /* Create Default Aggregation for Bridge */
   iw_agg_bridging_cfg.tag = tag_agg_default_bridge2;
   iw_agg_bridging_cfg.txfunc = h_bridge2_iwhost;
   iw_agg_bridging_cfg.input_bport = 0;
   iw_agg_bridging_cfg.intmode = WP_IW_INT_ENABLE;
   iw_agg_bridging_cfg.vlan_tag_mode =WP_IW_VLAN_TAG_ENABLE;

   h_bridge2_default_agg = WP_IwFlowAggregationCreate(
      APP_MASTER_WPID,
      WP_IW_VLAN_AWARE_BRIDGING_MODE,
      &iw_agg_bridging_cfg);
   App_TerminateOnError(h_bridge2_default_agg," WP_IwFlowAggregationCreate() Bridge2 Default");

   iw_agg_bridging_cfg.vlan_tag_mode =WP_IW_VLAN_TAG_DISABLE; 
   iw_agg_bridging_cfg.intmode = WP_IW_INT_DISABLE;

   for(ii = 0;ii < APP_MAX_TDM_PORTS;ii++)
   {
      /* Create a bridge port for each TDM port */
      bridge_port_cfg.tag = tag_bridge2_bport_tdm[ii];
      bridge_port_cfg.flow_agg = h_bridge2_default_agg;
      bridge_port_cfg.input_filters_mask =0xf0000000;
      bridge_port_cfg.classification_flag = WP_IW_IWPORT_CLASSIFICATION_ENABLED;
      h_bridge2_bport_tdm[ii] = WP_IwPortCreate(h_iw_sys_bridge2,
                                                &bridge_port_cfg);
      App_TerminateOnError(h_bridge2_bport_tdm[ii],"WP_IwPortCreate() Bridge2 TDM");

      /* Create a PPP Switching flow agg for each TDM port */
      iw_agg_ppp_switching.tag = tag_agg_pppsw[ii];
      iw_agg_ppp_switching.txfunc = h_iw_sys_bridge2;
      iw_agg_ppp_switching.iw_port = h_bridge2_bport_tdm[ii];
      iw_agg_ppp_switching.prefix_length = 18;
      iw_agg_ppp_switching.l2_header_insert_mode = WP_IW_L2H_INSERT_ENABLE;
      memcpy(iw_agg_ppp_switching.prefix_header, dummy_prefix_header,18);

      h_flow_agg_pppsw_link[ii] = WP_IwFlowAggregationCreate(
         WP_WINPATH(0),
         WP_IW_PPP_SWITCHING_MODE,
         &iw_agg_ppp_switching);
      App_TerminateOnError(h_flow_agg_pppsw_link[ii]," WP_IwFlowAggregationCreate() PPPSW");
      memcpy(&iw_agg_ppp_switching_tdm[ii], &iw_agg_ppp_switching, sizeof(WP_iw_agg_ppp_switching));

      /* Bind the TDM RX channel to the PPP Switching System using this flow agg */
      rx_binding_pppsw.aggregation = h_flow_agg_pppsw_link[ii];
      rx_binding_pppsw.mru = 1536;
      rx_binding_pppsw.encap_mode = WP_IW_VCMUX;
      rx_binding_pppsw.acfc_mode = WP_IW_ACFC_ENABLE; 
      rx_binding_pppsw.pfc_mode = WP_IW_PFC_ENABLE;
      rx_binding_pppsw.pppmux_ncp_filter = WP_IW_PPPMUX_NCP_FILTER_SWITCH;
      rx_binding_pppsw.forward_ppp_to_bundle_mode = WP_IW_PPPSW_FORWARD_PPP_TO_LINK;
      rx_binding_pppsw.cisco_hdlc_mode=WP_IW_CISCO_HDLC_DISABLE;

#if DISABLE_TDM_COMET_CARD 
      status = WP_IwRxBindingCreate(h_tdm_rx[ii],
                                    h_iw_sys_pppsw,
                                    h_qnode_iwq,
                                    &rx_binding_pppsw);
      App_TerminateOnError(status," WP_IwRxBindingCreate() PPPSW");
#endif

   }

   /* Create a bridge port for MLPPP */
   bridge_port_cfg.tag = tag_bridge2_bport_mlppp;
   bridge_port_cfg.flow_agg = h_bridge2_default_agg;
   bridge_port_cfg.input_filters_mask =0xf0000000;
   bridge_port_cfg.classification_flag = WP_IW_IWPORT_CLASSIFICATION_ENABLED;
   h_bridge2_bport_mlppp = WP_IwPortCreate(h_iw_sys_bridge2,
                                           &bridge_port_cfg);
   App_TerminateOnError(h_bridge2_bport_mlppp,"WP_IwPortCreate() Bridge2 MLPPP");

   /* Create a PPP Switching flow agg for MLPPP */
   iw_agg_ppp_switching.tag = tag_agg_pppsw_mlppp;
   iw_agg_ppp_switching.txfunc = h_iw_sys_bridge2;
   iw_agg_ppp_switching.iw_port = h_bridge2_bport_mlppp;
   iw_agg_ppp_switching.prefix_length =  22; 
   iw_agg_ppp_switching.l2_header_insert_mode = WP_IW_L2H_INSERT_ENABLE;
   memcpy(iw_agg_ppp_switching.prefix_header, dummy_prefix_header,18); 

   h_flow_agg_pppsw_link_mlppp = WP_IwFlowAggregationCreate( WP_WINPATH(0),
                                                             WP_IW_PPP_SWITCHING_MODE,
                                                             &iw_agg_ppp_switching);
   App_TerminateOnError(h_flow_agg_pppsw_link_mlppp," WP_IwFlowAggregationCreate() PPPSW for MLPPP");
   memcpy(&iw_agg_ppp_switching_link, &iw_agg_ppp_switching, sizeof(WP_iw_agg_ppp_switching));

   /* Bind the MLPPP RX channel to the PPP Switching System using this flow agg */
   rx_binding_pppsw.mru = 1536;
   rx_binding_pppsw.encap_mode = WP_IW_VCMUX;
   rx_binding_pppsw.acfc_mode = WP_IW_ACFC_ENABLE;
   rx_binding_pppsw.pfc_mode = WP_IW_PFC_ENABLE;
   rx_binding_pppsw.pppmux_ncp_filter = WP_IW_PPPMUX_NCP_FILTER_SWITCH;
   rx_binding_pppsw.forward_ppp_to_bundle_mode = WP_IW_PPPSW_FORWARD_PPP_TO_LINK;
   rx_binding_pppsw.cisco_hdlc_mode=WP_IW_CISCO_HDLC_DISABLE;
   rx_binding_pppsw.aggregation = h_flow_agg_pppsw_link_mlppp;
   status = WP_IwRxBindingCreate(mlppp_rx_ch_handle,
                                 h_iw_sys_pppsw,
                                 h_qnode_iwq,
                                 &rx_binding_pppsw);
   App_TerminateOnError(status," WP_IwRxBindingCreate() PPPSW -> MLPPP");

   /* Create a bridge port to represent ENET8 */
   bridge_port_cfg.tag = tag_bport_enet2;
   bridge_port_cfg.flow_agg = h_bridge2_default_agg;
   bridge_port_cfg.input_filters_mask = 0;
   bridge_port_cfg.classification_flag = WP_IW_IWPORT_CLASSIFICATION_DISABLED;
   h_bridge2_bport_enet2 = WP_IwPortCreate(h_iw_sys_bridge2,
                                           &bridge_port_cfg);
   App_TerminateOnError(h_bridge2_bport_enet2,"WP_IwPortCreate() ENET2");

   /* IPCP flow agg - MLPPP IPCP packets (0x8021) go here */
   iw_agg_directmap_cfg.tag = tag_agg_mlppp_control;
   iw_agg_directmap_cfg.txfunc = h_bridge2_iwhost;  
   iw_agg_directmap_cfg.iw_port = 0;
   iw_agg_directmap_cfg.extraction_length = 18; //extract mac header and send rest of the packet to host
   iw_agg_directmap_cfg.prefix_length = 0;
   iw_agg_directmap_cfg.l2_header_insert_mode = WP_IW_L2H_INSERT_DISABLE;
   iw_agg_directmap_cfg.intmode = WP_IW_INT_ENABLE;

   h_flow_agg_mlppp_control  = WP_IwFlowAggregationCreate(WP_WINPATH(0), WP_IW_DIRECTMAP_MODE,
                                                          &iw_agg_directmap_cfg);
   App_TerminateOnError(h_flow_agg_mlppp_control," WP_IwFlowAggregationCreate() MLPPP control");

   /* Sending to enet8 device  */
   iw_agg_bridging_cfg.tag = tag_agg_enet2;
   iw_agg_bridging_cfg.txfunc = h_device_enet2; 
   iw_agg_bridging_cfg.iw_port = h_bridge2_bport_enet2;

   h_flow_agg_enet2_mlppp  = WP_IwFlowAggregationCreate( APP_MASTER_WPID,
                                                         WP_IW_VLAN_AWARE_BRIDGING_MODE,
                                                         &iw_agg_bridging_cfg);
   App_TerminateOnError(h_flow_agg_enet2_mlppp," WP_IwFlowAggregationCreate() ENET2  ");


   /* Add DFC flows */

   memset( &flow_class_rule, 0, sizeof(WP_flow_class_rule));
   /* NOTE for this test we are using TDM#1 to send out and receive the packet, so we use the bridge port for TDM 1 */
   dfc_flow_info.input_port = h_bridge2_bport_tdm[1];  /* Note that Input_port is one of the fields in the filter */
   dfc_flow_info.output_port = 0;
   /* PPP IP flow */ /* Note the use of FILTER 2 */
   dfc_flow_info.filter_handle = h_bridge2_filter[0];
   sprintf(rule_string,"n;n;n;n;n;n;n;n;n;n;0xb;n;n;n");
   forwarding_action.flow_aggregation = h_flow_agg_mlppp_control;
   flow_class_rule.classifier_string = rule_string;
   flow_class_rule.dfc_info = &dfc_flow_info;
   flow_class_rule.match_action = WP_FLOW_CLASS_MODE_FLOW_AGG;
   flow_class_rule.action_info.forwarding_action = forwarding_action;

   h_bridge2_dfcflow[1] = WP_IwFlowAdd(h_iw_sys_bridge2, WP_FLOW_CLASS_RULE, &flow_class_rule);
   App_TerminateOnError(h_bridge2_dfcflow[1]," WP_IwFlowAdd() DFC Bridge2");


   /* MLPPP data flow with QoS */
   /* NOTE: for this test we use MLPPP bridge port */
   dfc_flow_info.input_port = h_bridge2_bport_mlppp;  /* Note that Input_port is one of the fields in the filter */
   dfc_flow_info.output_port = 0;
   dfc_flow_info.filter_handle = h_bridge2_filter[1];
   priority_action.priority_value = 0;
   priority_action.field_type = WP_FLOW_PRIORITY_VLAN;  /* Priority remarking is done with VLAN */
   priority_action.action_mode =  WP_FLOW_PRIORITY_MAPPING | WP_FLOW_PRIORITY_REMARKING;
   /* Testing PRIORITY_REMARKING :                                                                 *
    * make the priority_action.action_mode to only WP_FLOW_PRIORITY_REMARKING; and then change the *
    * forwarding_action.flow_aggregation to h_bridge2_default_agg.Whatever the priority set in the * 
    * priority_action.priority_value will be reflected in the VLAN tag field. This can be seen in  *
    * WinMon print                                                                                 */
   for(ii=0; ii<PQBLOCK_SIZE;ii++)
   {
      priority_action.priority_value = ii + 1;     /* Priority value is set here */
      sprintf(rule_string,"n;n;n;n;n;n;n;n;n;n;0xa;n;n;n;n;n;n;n;n;n;n;n;n;n;n;n;n;n;n;n;n;n;n;n;n;n;n;n;n;n;n;n;n;n;0x%x;n",ii);
      forwarding_action.flow_aggregation = h_flow_agg_enet2_mlppp; 
      flow_class_rule.classifier_string = rule_string;
      flow_class_rule.dfc_info = &dfc_flow_info;
      flow_class_rule.match_action = WP_FLOW_CLASS_MODE_EXT_ACTION;
      action_type[1] = WP_FLOW_CLASS_MODE_FLOW_AGG;
      action_type[0] = WP_FLOW_CLASS_MODE_PRIORITY;
      action.num_of_actions = 2;
      action.action_type = action_type;
      action_info[1].forwarding_action = forwarding_action;
      action_info[0].priority_action = priority_action;
      action.action_info = action_info;
      flow_class_rule.action_info.ext_action = action;

      h_bridge2_dfcflow_mlppp_traffic[ii] = WP_IwFlowAdd(h_iw_sys_bridge2, WP_FLOW_CLASS_RULE, &flow_class_rule);
      App_TerminateOnError(h_bridge2_dfcflow_mlppp_traffic[ii]," WP_IwFlowAdd() DFC Bridge2 -> MLPPP control");
   }
   /* Be sure to build the system */
   status = WP_IwSystemBuild(h_iw_sys_bridge2);
   App_TerminateOnError(status," WP_IwSystemBuild() Bridge2");
}

/****************************************************************************************************************************
 * Function name: App_CreateEnetHdlcBridge()
 * Description  :  App_CreateEnetHdlcBridge function creates a  VLAN aware bridge interworking systems.   
 * 			This function creates the filters and flows in the HDLC to Enet direction     
 *
 * Input  params: none
 * Output params: 
 * Return val   : none
 *****************************************************************************************************************************/

void App_CreateEnetHdlcBridge()
{
   WP_U32 ii=0;
   WP_status   status;
   WP_dfc_flow_info dfc_flow_info;
   WP_flow_class_rule flow_class_rule;
   WP_CHAR rule_string[128];
   WP_classification_filter classification_filter={0};
   WP_flow_class_forwarding_action forwarding_action;
   WP_iw_agg_bridging iw_agg_bridging_cfg;
   /* Strutures for QoS actions */
   WP_flow_class_priority_action  priority_action;
   WP_flow_class_ext_action action;
   WP_U8 action_type[3];
   WP_flow_class_action_info action_info[3];

   /* Header for MLPPP/PPP :: PPP is configured for LCP/Traffic, MLPPP is configured for IPCP/Traffic       *
    * Protocols: Traffic:0xff030021, LCP: 0xff03c021, IPCP: 0xff038021                                      */
   WP_U8 ppp_prefix_header[4] = {0xff,0x03,0x80,0x21};  //{0xff,0x03,0x80,0x21};  
   /*Below menu giving option of configuring interface for Traffic or Control data.... NOTE: currently LCP packet forwarding is not supported */
   /*Classification and forwarding packet will be done based on interface choice*/
   /*If user choice is TRAFFIC_DATA: packet will be forwarded to ENET8 device */
   /*If user choice is CONTROL_DATA: packet will be forwarded to HOST*/
   while(( interface_ch != TRAFFIC_DATA) && ( interface_ch != CONTROL_DATA))
   {
      printf("\n");
      printf("Enter vlan tag for forward packet to host or enet2 device\n");
      printf("1. TRAFFIC_DATA (0xff030021) \n");
      printf("2. CONTROL_DATA (0xff038021) \n");
      if (0)
         interface_ch = getchar();
      else
         interface_ch = TRAFFIC_DATA;
   }

   switch(interface_ch)
   {
      case TRAFFIC_DATA:
         ppp_prefix_header[0] = 0xff;
         ppp_prefix_header[1] = 0x03;
         ppp_prefix_header[2] = 0x00;
         ppp_prefix_header[3] = 0x21;
         break;
      case CONTROL_DATA:
         ppp_prefix_header[0] = 0xff;
         ppp_prefix_header[1] = 0x03;
         ppp_prefix_header[2] = 0x80;
         ppp_prefix_header[3] = 0x21;
         break;
   }

   /* Bridge1 ENET --> TDM */
   iw_sys_bridge.tag = tag_iw_sys_bridge1;
   h_iw_sys_bridge1 = WP_IwSystemCreate(APP_MASTER_WPID,
                                        WP_IW_VLAN_AWARE_BRIDGING_MODE,
                                        &iw_sys_bridge);
   App_TerminateOnError(h_iw_sys_bridge1," WP_IwSystemCreate() Bridge1");

   /*  Two filters are created. First filter is based on DEST_MAC and VLAN ID the second filter *
    *  is based on DSCP classification for QoS in addition to DEST_MAC and VLAN ID              */
   /*  1. DFC filter using 2 fields */
   classification_filter.fields_mask = WP_FIELD_IDS_ARRAY;
   classification_filter.no_match_action = WP_CLASS_FILTER_NO_MATCH_CONTINUE; 
   /* Filter based on DST_MAC and VLAN_ID */
   classification_filter.field_ids_array[0] = WP_FIELD_ID_DST_MAC;
   classification_filter.field_ids_array[1] = WP_FIELD_ID_VLAN_ID;
   classification_filter.field_ids_array[2] = WP_FIELD_ID_LAST;
   classification_filter.filter_type = WP_CLASS_FILTER_EMC;

   h_bridge1_filter[0] = WP_IwClassFilterAdd(h_iw_sys_bridge1, &classification_filter);
   App_TerminateOnError(h_bridge1_filter[0]," WP_IwClassFilterAdd() Bridge1");

   /* 2. DFC filter using 3 fields */
   classification_filter.fields_mask = WP_FIELD_IDS_ARRAY;
   classification_filter.no_match_action = WP_CLASS_FILTER_NO_MATCH_DENY;

   /* Filter based on DST_MAC and VLAN_ID and DSCP (for QoS) */
   classification_filter.field_ids_array[0] = WP_FIELD_ID_DST_MAC;
   classification_filter.field_ids_array[1] = WP_FIELD_ID_VLAN_ID;
   classification_filter.field_ids_array[2] = WP_FIELD_ID_IPV4_DSCP;  /* If DSCP does not work use TOS */
   classification_filter.field_ids_array[3] = WP_FIELD_ID_LAST;
   classification_filter.filter_type = WP_CLASS_FILTER_EXT_ACTION_EMC;

   h_bridge1_filter[1] = WP_IwClassFilterAdd(h_iw_sys_bridge1, &classification_filter);
   App_TerminateOnError(h_bridge1_filter[1]," WP_IwClassFilterAdd() Bridge1");

   memcpy(&iw_agg_bridging_cfg, &default_iw_agg_bridging_cfg, sizeof(WP_iw_agg_bridging));

   /* Create Default Aggregation Bridge1 */
   iw_agg_bridging_cfg.tag = tag_agg_default_bridge1;
   iw_agg_bridging_cfg.txfunc = h_bridge1_iwhost;
   iw_agg_bridging_cfg.input_bport = 0;
   iw_agg_bridging_cfg.intmode = WP_IW_INT_ENABLE;


   h_bridge1_default_agg = WP_IwFlowAggregationCreate(APP_MASTER_WPID,
                                                      WP_IW_VLAN_AWARE_BRIDGING_MODE,
                                                      &iw_agg_bridging_cfg);
   App_TerminateOnError(h_bridge1_default_agg," WP_IwFlowAggregationCreate() Default Bridge");

   iw_agg_bridging_cfg.intmode = WP_IW_INT_DISABLE;

   bridge_port_cfg.tag = tag_bport_enet1;
   bridge_port_cfg.flow_agg = h_bridge1_default_agg;
   bridge_port_cfg.input_filters_mask = 0xe0000000;
   h_bridge1_bport_enet1 = WP_IwPortCreate(h_iw_sys_bridge1,
                                           &bridge_port_cfg);
   App_TerminateOnError(h_bridge1_bport_enet1,"WP_IwPortCreate()ENET1");

   /* Rx binding ENET 7 */
   rx_binding_bridging_cfg.input_port = h_bridge1_bport_enet1;
   status = WP_IwRxBindingCreate(h_enet1_rx, h_iw_sys_bridge1, h_qnode_iwq, &rx_binding_bridging_cfg);
   App_TerminateOnError(status," WP_IwRxBindingCreate() enet1 ");

   /* EACH TDM port is represented as bridge port in Bridge1 too so a packet going from ENET to TDM has the *
    * ENET bridge port as input bridge port and TDM bridge port as the output bridge port                   *
    * This port is used for regular PPP packets                                                             */
   for(ii = 0;ii < APP_MAX_TDM_PORTS;ii++)
   {
      bridge_port_cfg.tag = tag_bridge1_bport_tdm[ii];
      bridge_port_cfg.flow_agg = h_bridge1_default_agg;
      bridge_port_cfg.max_mac_addresses = APP_MAX_MAC_ADDRESS;
      bridge_port_cfg.input_filters_mask = 0;
      bridge_port_cfg.classification_flag = WP_IW_IWPORT_CLASSIFICATION_DISABLED;
      h_bridge1_bport_tdm[ii] = WP_IwPortCreate(h_iw_sys_bridge1,
                                                &bridge_port_cfg);
      App_TerminateOnError(h_bridge1_bport_tdm[ii],"WP_IwPortCreate()TDM");
   }

   /* create iw port for MLPPP */
   bridge_port_cfg.tag =tag_bridge1_bport_mlppp ;
   bridge_port_cfg.flow_agg = h_bridge1_default_agg;
   bridge_port_cfg.max_mac_addresses = APP_MAX_MAC_ADDRESS;
   bridge_port_cfg.input_filters_mask = 0;
   bridge_port_cfg.classification_flag = WP_IW_IWPORT_CLASSIFICATION_DISABLED;
   h_bridge1_bport_mlppp = WP_IwPortCreate(h_iw_sys_bridge1,
                                           &bridge_port_cfg);
   App_TerminateOnError(h_bridge1_bport_mlppp,"WP_IwPortCreate() MLPPP");

   /* Create flow aggregations that send the packet out on each TDM */
   /* NOTE: THE 4 BYTE PPP header used */ 
   /* ALSO NOTE the 18 bytes being extracted */
   for(ii = 0;ii < APP_MAX_TDM_PORTS;ii++)
   {
      iw_agg_bridging_cfg.tag = tag_agg_tdm[ii];
      iw_agg_bridging_cfg.txfunc = h_tdm_tx[ii];
      iw_agg_bridging_cfg.iw_port = h_bridge1_bport_tdm[ii];
      iw_agg_bridging_cfg.extraction_length = 18;
      iw_agg_bridging_cfg.prefix_length = 4;
      iw_agg_bridging_cfg.l2_header_insert_mode = WP_IW_L2H_INSERT_ENABLE;
      memcpy(iw_agg_bridging_cfg.prefix_header, ppp_prefix_header, 4);

      h_flow_agg_tdm[ii] = WP_IwFlowAggregationCreate(
         APP_MASTER_WPID,
         WP_IW_VLAN_AWARE_BRIDGING_MODE,
         &iw_agg_bridging_cfg);
      App_TerminateOnError(h_flow_agg_tdm[ii]," WP_IwFlowAggregationCreate() TDM ");
      memcpy(&iw_agg_bridging_cfg_tdm[ii], &iw_agg_bridging_cfg, sizeof(WP_iw_agg_bridging));
   }

   /* Create flow aggregations that send the packet out on MLPPP channel */
   iw_agg_bridging_cfg.tag = tag_agg_mlppp;
   /* When QoS is enabled PQ block is used */
   iw_agg_bridging_cfg.txfunc =  mlppp_bundle_pqblock_handle[0];
   iw_agg_bridging_cfg.iw_port = h_bridge1_bport_mlppp;
   iw_agg_bridging_cfg.extraction_length = 18;
   iw_agg_bridging_cfg.prefix_length = 4;
   iw_agg_bridging_cfg.l2_header_insert_mode = WP_IW_L2H_INSERT_ENABLE;
   memcpy(iw_agg_bridging_cfg.prefix_header, ppp_prefix_header, 4);

   h_flow_agg_mlppp1 = WP_IwFlowAggregationCreate(
      APP_MASTER_WPID,
      WP_IW_VLAN_AWARE_BRIDGING_MODE,
      &iw_agg_bridging_cfg);
   App_TerminateOnError(h_flow_agg_mlppp1," WP_IwFlowAggregationCreate() TDM ");
   memcpy(&iw_agg_mlppp, &iw_agg_bridging_cfg, sizeof(iw_agg_mlppp));

   iw_agg_bridging_cfg.prefix_length = 0;
   iw_agg_bridging_cfg.l2_header_insert_mode = WP_IW_L2H_INSERT_DISABLE;
   iw_agg_bridging_cfg.extraction_length = 0;

   /* Add DFC flows */
   memset( &flow_class_rule, 0, sizeof(WP_flow_class_rule));
   /* Add DFC flow for regular PPP packet flow */
   dfc_flow_info.filter_handle = h_bridge1_filter[0];
   dfc_flow_info.input_port = h_bridge1_bport_enet1; 
   dfc_flow_info.output_port = 0;

   forwarding_action.flow_aggregation = h_flow_agg_tdm[1];

   /* Add DEST MAC ADDR = 0x00.00.00.00.00.0b  & VLAN = 10 */
   /* Note: PPP packet is classified with DEST MAC ADDR   */
   sprintf(rule_string,"n;n;n;n;n;n;n;n;n;0x00.00.00.00.00.0b;10;n"); 
								      
   flow_class_rule.classifier_string = rule_string;
   flow_class_rule.dfc_info = &dfc_flow_info;
   flow_class_rule.match_action = WP_FLOW_CLASS_MODE_FLOW_AGG;
   flow_class_rule.action_info.forwarding_action = forwarding_action;

   h_bridge1_dfcflow1 = WP_IwFlowAdd(h_iw_sys_bridge1, WP_FLOW_CLASS_RULE, &flow_class_rule);
   App_TerminateOnError(h_bridge1_dfcflow1," WP_IwFlowAdd() DFC Bridge1 rule1");

   /* Add DFC flow for MLPPP */
   dfc_flow_info.filter_handle = h_bridge1_filter[1];
   dfc_flow_info.input_port = h_bridge1_bport_enet1;
   dfc_flow_info.output_port = 0;

   forwarding_action.flow_aggregation = h_flow_agg_mlppp1;

   policer_handle = WP_PolicerCreate(WP_WINPATH(0)/*wpid*/,WP_POLICER_V1,policer_config);
   App_TerminateOnError(policer_handle," WP_IwPolicerCreate() rule1 ");

   /* Note: Input Bridge port not passed in string but passed in above */
   for(ii = 0; ii<PQBLOCK_SIZE; ii++)
   {
      /* Add DEST MAC ADDR = 0x00.00.00.00.00.0a (9th position)  & VLAN = 10 (10th positon) */
      /* Note: Postion is counted from 0 to N                                               */
      sprintf(rule_string,"n;n;n;n;n;n;n;n;n;0x00.00.00.00.00.0a;10;n;n;n;n;n;n;n;n;n;n;n;n;n;n;n;n;n;n;n;n;n;n;n;n;n;n;n;n;n;n;n;n;n;0x%x;n",ii);
      priority_action.priority_value = ii;
      priority_action.field_type = WP_FLOW_PRIORITY_DSCP;
      priority_action.action_mode = WP_FLOW_PRIORITY_MAPPING | WP_FLOW_PRIORITY_REMARKING;
      flow_class_rule.match_action = WP_FLOW_CLASS_MODE_EXT_ACTION;
      action_type[1] = WP_FLOW_CLASS_MODE_FLOW_AGG;
      action_type[0] = WP_FLOW_CLASS_MODE_PRIORITY;
      action_type[2] = WP_FLOW_CLASS_MODE_POLICER;
      action.num_of_actions = 3;
      action.action_type = action_type;
      action_info[1].forwarding_action = forwarding_action;
      action_info[0].priority_action = priority_action;
      action_info[2].policer_action.policer = policer_handle;

      action.action_info = action_info;
      flow_class_rule.action_info.ext_action = action;
      flow_class_rule.classifier_string = rule_string;
      flow_class_rule.dfc_info = &dfc_flow_info;

      h_bridge1_dfcflow_mlppp[ii] = WP_IwFlowAdd(h_iw_sys_bridge1, WP_FLOW_CLASS_RULE, &flow_class_rule);
      App_TerminateOnError(h_bridge1_dfcflow_mlppp[ii]," WP_IwFlowAdd() DFC Bridge1 rule2 (mlppp rules)");
   }

   status = WP_IwSystemBuild(h_iw_sys_bridge1);
   App_TerminateOnError(status," WP_IwSystemBuild() Bridge1");
}


/*****************************************************************************
 * Function name: App_ParameterSwitch
 * Description  : Change prefix header for the other case.
 * Input  params: None
 * Output params: None
 * Return val   : None
 *****************************************************************************/
WP_status App_ParameterSwitch(WP_CHAR switchto)
{
    int jj;
    WP_status result;
    WP_U8 dummy_prefix_header1[18];
    WP_U8 dummy_prefix_header2[4];

    if ((switchto == interface_ch))
    {
        return (WP_ERR_CONFIG);
    }
    
    if ((CONTROL_DATA != switchto) && (TRAFFIC_DATA != switchto))
    {
        return (WP_ERR_CONFIG);
    }

    interface_ch = switchto;

    for(jj = 0; jj < 12; jj++)
    {
       dummy_prefix_header1[jj] = jj;  /* src and dst mac */
    }

    dummy_prefix_header1[12] = 0x08;   /* eth type = IP */
    dummy_prefix_header1[13] = 0x00;
    dummy_prefix_header1[12] = 0x81;
    dummy_prefix_header1[13] = 0x00;
    dummy_prefix_header1[14] = 0x00;
    dummy_prefix_header1[15] = 0x0a;
    dummy_prefix_header1[16] = 0x08;   /* eth type = IP */
    dummy_prefix_header1[17] = 0x00;

    if (CONTROL_DATA == switchto)
    {
        dummy_prefix_header1[15] = 0x0b;

        dummy_prefix_header2[0] = 0xff;
        dummy_prefix_header2[1] = 0x03;
        dummy_prefix_header2[2] = 0x80;
        dummy_prefix_header2[3] = 0x21;  
    }
    else
    if (TRAFFIC_DATA == switchto)
    {
        dummy_prefix_header1[15] = 0x0a;

        dummy_prefix_header2[0] = 0xff;
        dummy_prefix_header2[1] = 0x03;
        dummy_prefix_header2[2] = 0x00;
        dummy_prefix_header2[3] = 0x21;        
    }


    /* Mod some parameters*/
   for (jj = 0; jj < APP_MAX_TDM_PORTS; jj++)
   {
      memcpy(iw_agg_ppp_switching_tdm[jj].prefix_header, dummy_prefix_header1, 18);
      result = WP_IwFlowAggregationModify(h_flow_agg_pppsw_link[jj], WP_IW_FLOW_AGG_MOD_PRX, &iw_agg_ppp_switching_tdm[jj]);
      App_TerminateOnError(result," WP_IwFlowAggregationModify() PPP TDM");

      memcpy(iw_agg_bridging_cfg_tdm[jj].prefix_header, dummy_prefix_header2, 4);
      result = WP_IwFlowAggregationModify(h_flow_agg_tdm[jj], WP_IW_FLOW_AGG_MOD_PRX, &iw_agg_bridging_cfg_tdm[jj]);
      App_TerminateOnError(result," WP_IwFlowAggregationModify() TDM");
   }

   memcpy(iw_agg_ppp_switching_link.prefix_header, dummy_prefix_header1, 18);
   result = WP_IwFlowAggregationModify(h_flow_agg_pppsw_link_mlppp, WP_IW_FLOW_AGG_MOD_PRX, &iw_agg_ppp_switching_link);
   App_TerminateOnError(result," WP_IwFlowAggregationModify() LINK MLPPP");   

   memcpy(iw_agg_mlppp.prefix_header, dummy_prefix_header2, 4);
   result = WP_IwFlowAggregationModify(h_flow_agg_mlppp1, WP_IW_FLOW_AGG_MOD_PRX, &iw_agg_mlppp);
   App_TerminateOnError(result," WP_IwFlowAggregationModify() LINK MLPPP");

   return WP_OK;
}

/*****************************************************************************
 * Function name: App_perform_freerun
 * Description  : function send traffic from host CPU and then check statistics. Developer of 
 *                application should be responsible for matain this function for each application.
 * Input  params: None
 * Output params: None
 * Return val   : None
 *****************************************************************************/
static void App_perform_freerun(void)
{
    WP_status result, testresult1, testresult2;
    WP_U32 i;

    /* check traffic data */
    interface_ch = CONTROL_DATA;
    App_ResetStats();
    
   /*Send packets*/
   for ( i= 0; i < APP_MAX_PKTS_NUM; i ++) 
   {
      App_DataSend(h_enet1_tx, h_pool_buffer_iw);
   }

   WP_Delay(1000000);

   /*Check Statistics*/
   App_CheckStats(&testresult1);  

   if (testresult1 == WP_OK)
   {
       printf("Data testcase ok!\n");
   }
   else 
   {
       printf("Data testcase fail!\n");
   }

    /* check control data */
    App_ResetStats();
    result = App_ParameterSwitch(TRAFFIC_DATA);
    if (WP_OK !=  result)
    {
        // exception 
        printf("Test Failed!\n");
        printf("This is the last test case in this application!\n");
        printf("This test case DO NOT need further manually test!\n");
        App_Quit(1);
    }

   /*Send packets*/
   for ( i= 0; i < APP_MAX_PKTS_NUM; i ++) 
   {
      App_DataSend(h_enet1_tx, h_pool_buffer_iw);
   }

   WP_Delay(1000000);

   /*Check Statistics*/
   App_CheckStats(&testresult2);

   if ((WP_OK == testresult2) && (WP_OK == testresult1))
   {
       printf("test passed!\n");
   }
   else 
   {
       printf("test failed!\n");
   }

   printf("This is the last test case in this application!\n");
   printf("This test case DO NOT need further manually test!\n");
   
   /*Quit test*/
   App_Quit(1);

}

