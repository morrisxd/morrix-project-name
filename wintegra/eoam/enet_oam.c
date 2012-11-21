/**********************************************************************************************
 * (C) Copyright PMC - WIN division (Wintegra) 2012.  All rights reserved.
 * PMC-Sierra (WINTEGRA) CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of PMC-Sierra (Wintegra).
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ***********************************************************************************************/

/**********************************************************************************************
 *  Application : ENET OAM
 *  File Name   : enet_oam.c
 *  Version     : 1
 *  Date        : 10 Feb 2012
 *  Accompanies : 
 ***********************************************************************************************/
/***********************************************************************************************
 *  Description: 
 *    ENET OAM application is demo application for the Ethernet OAM packets. In this application
 *    ...
 ***********************************************************************************************/


#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "wp_wddi.h"
#include "wp_host_io.h"
#include "wpx_app_data.h"
#include "wpx_board_data.h"
// #include "wpx_enet_phy.h"

//#include "wp_enet_oam.h"
#include "enet_oam_util.h"
#include "enet_oam.h"
#include "wt_partition_ecc_util.h"

WPE_EOAM_DB eoam_system;
WP_handle status;
WP_boolean  eoam_running = WP_TRUE;
/*
 * Global variables used during test execution
 */
WP_U8 wt_mac_enet1[6]    = {0x45, 0x6e, 0x65, 0x74, 0x23, 0x31};
WP_U8 wt_mac_enet2[6]    = {0x45, 0x6e, 0x65, 0x74, 0x23, 0x32};
WP_U8 wt_mac_enet3[6]    = {0x45, 0x6e, 0x65, 0x74, 0x23, 0x33};

#include "enet_oam_menu.c"
#include "enet_oam_util.c"

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
void App_TerminateOnError(WP_handle handle, WP_CHAR *s)
{
   if (WPI_SimulateInterrupts(), WP_ERROR_P(handle)) 
   {
      printf("Test Abort %s %s 0x%x\n", s, WP_error_name[WP_ERROR(handle)], handle);
      /* Release WDDI */
      WP_DriverRelease();
      exit(1);
   }
   else
   {
      if(handle == WP_OK)
         printf("Status returned from %s : WP_OK\n",s);
      else
         printf("Handle returned from %s is %#8.8x\n", s,handle);
   }
}

/****************************************************************************************************************************
 * Function name: add_task()
 * Description  :  Event handling 
 *
 * Input  params: 
 * Output params: none
 * Return val   : none
 *****************************************************************************************************************************/

void add_task(app_task_list *task_list, WP_U32 event_type, WP_tag event_tag,
              void *event_param)
{
    WP_U16 tail = task_list->tail;
    WP_U32 next = tail + 1;


    if (next == task_list->num_elements)
      next = 0;

    if (next != task_list->head) 
    {
      task_list->task[tail].event_tag = event_tag;
      task_list->task[tail].event_type = event_type;
      task_list->task[tail].event_param = event_param;
      task_list->tail = next;
    }
}

/****************************************************************************************************************************
 * Function name: next_task()
 * Description  :  Event handling 
 *
 * Input  params: 
 * Output params: none
 * Return val   : none
 *****************************************************************************************************************************/

app_task *
next_task(app_task_list *task_list, app_task *result)
{
   WP_U32 head = task_list->head;
   WP_U32 tail = task_list->tail;

   if (head == tail)
      return NULL;

   *result = task_list->task[head];
   if (++head == task_list->num_elements)
      head = 0;
   task_list->head = head;
   return result;
}

void app_perform_action(app_task *task)
{
   WP_U32 tag = task->event_tag;
   switch(task->event_type)
   {
      case WP_EVENT_RX_INDICATE:
      {
         WPE_Eoam_DataReceive(tag, 0, 0);
         break;
      }
      case WP_EVENT_TX_INDICATE:
      {
         break;
      }
      case WP_EVENT_IW_ENET_OAM_MESSAGE_QUEUE:
      {
         WPE_Eoam_MessageQueueReceive(tag);
         break;
      }      
      case WP_EVENT_STATUS_INDICATE:
         break;
      default: 
         printf("Not processing unknown event\n");
   }
}


void App_EventRxIndicate(WP_tag tag, WP_U32 data, WP_U32 info)
{
   add_task(irq_task_list, WP_EVENT_RX_INDICATE, tag, NULL);
}

WP_U32 get_sp(void)
{
	WP_U32 mygp = 0;
	WP_U32 mysp = 0;
	WP_U32 myfp = 0;
	WP_U32 myra = 0;

	mygp = 0;
	mysp = 0;
	myra = 0;
	myfp = 0;

	__asm__ volatile ("move %0, $28" : "=r"(mygp));
	__asm__ volatile ("move %0, $29" : "=r"(mysp));
	__asm__ volatile ("move %0, $30" : "=r"(myfp));
	__asm__ volatile ("move %0, $31" : "=r"(myra));

	printf ("#############################################\n");
	printf ("##### mygp(0x%x) #####\n", mygp);
	printf ("##### mysp(0x%x) #####\n", mysp);
	printf ("##### myfp(0x%x) #####\n", myfp);
	printf ("##### myra(0x%x) #####\n", myra);
	printf ("#############################################\n");


	return (mysp);
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

    memset(&eoam_system, 0 , sizeof(WPE_EOAM_DB));


    status = WP_DriverInit();
    App_TerminateOnError(status,"WP_DriverInit()");

    /* Initialize the Tags */
    App_TestInit();

    /* initialize the ram partition*/
    App_RamInit();

    /* Configure the context and initialize the System */
    App_SysInit();

    /* Enet Physical ports are initialzed*/
    App_InitHW();

    /* Create Enet ports */
    App_PortsCreate();

    /* Create Host and Enet devices */
    App_DevicesAllocCreate();

    /* Create Host, Buffer Pools and Qnodes */
    App_BufferPoolsCreate();

    /* Register callback before commit*/
    WPE_Eoam_RegisterEvent();
    
    /* Initialize the Control registers, Create bridging system and Commit the system */
    App_SystemSetup();

    
    /* PTP module */
    WPE_PTPModuleInitWP3();
    
    /* Commit system resources */
    status = WP_SysCommit();    
    App_TerminateOnError(status,"WP_SysCommit()");

   /* Pecs create */
   WPE_PecsCreate ();
   
    /* Create Host, Enet and Host Channels */
    App_ChannelsCreate();
    
    /* Create testing system*/
    App_EnetOamSystemInit();

    status = WP_SysAsuEnable(APP_MASTER_WPID);
    App_TerminateOnError(status, "WP_SysPsuEnable()");

    /* Enable the ports and devices created earlier */
    App_PortsDevicesEnable();

    /* Enable the Channels created earlier */
    App_ChannelsEnable();

    /* Oam data init*/
    WPE_Eoam_Init();

    WPL_LockKeyInit(WPL_THREAD_LOCK_KEY, &eoam_lock);

    WPL_ThreadInit(&eoam_system.tid, WPE_Eoam_Msg_Thread, NULL);    

	get_sp ();
    if (argc < 1)
    {
       /* Start CLI*/
       CLI_T_Main(&eoam_system);
   
       WPL_ThreadDestroy(&eoam_system.tid);
       
       WPL_Delay(10);
       
       WPL_LockDestroy(WPL_THREAD_LOCK_KEY, &eoam_lock);
   
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
        WPE_Eoam_Quit (1);
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
void App_TestInit(void)
{
   tag_iw_sys_bridge1 = 600;
   
   tag_bport_enet1 = 700;
   tag_bport_enet2 = 800;
   tag_bport_enet3 = 900;
   
   tag_enet1_rx = 10;
   tag_enet1_tx = 11;
   
   tag_enet2_rx = 20;
   tag_enet2_tx = 21;

   tag_enet3_rx = 30;
   tag_enet3_tx = 31;
   
   tag_iwhost_rx1 = 90;
   tag_iwhost_rx_oam = 180;

}

void App_RamInit(void)
{
   /* initialize the ram partition, configure its pce rules's number */
   int_ram_partition.policer_entries = 8000;
   int_ram_partition.pce_hw_rules_num = APP_NUM_HW_PCE_RULES;
   WT_SetIntRamPartition (WP_WINPATH (0), &int_ram_partition);
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
   
   /* Context configuration */
   context_cfg.int_queue_data = &int_queue_tables;
   context_cfg.atm_data = NULL;
   context_cfg.iw_data = iw_global_cfg;
   context_cfg.pool_limit = 8;

   status = WP_SysInit(APP_MASTER_WPID, &context_cfg);
   App_TerminateOnError(status, "WP_SysInit()");

   WPE_EthernetOamModuleInit();
   WPE_PceGpeModuleInit();
   
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
   WP_status status;

   /* when use host send to trigger background traffic, loopback = 7, use tester loopback = 6 */
   WP_U32 loopback = 7;
   
#ifdef WP_HW_WINPATH3
   status = WPX_BoardConfigure(APP_MASTER_WPID, WPX_CONFIGURE_DEFAULT_CHECKIN);
   WPE_TerminateOnError(status, "WPX_BoardConfigure()",__LINE__);

#endif

#ifdef BOARD_WP3

   printf("please input ENET1,2,3 loopback mode at bit0,1,2. e.g. 7=all loop\n");
#if 0
   loopback = getchar();
#else
	loopback = '6';
#endif


   if(loopback >= '0' && loopback <= '9')
       loopback = loopback - '0';
   printf("the input = 0x%x\n", loopback);

   printf("ENET1 loopback mode = %s\n", (loopback & 0x1) ? "loopback" : "normal");
   printf("ENET2 loopback mode = %s\n", (loopback & 0x2) ? "loopback" : "normal");
   printf("ENET3 loopback mode = %s\n", (loopback & 0x4) ? "loopback" : "normal");

   status = WPX_BoardSerdesInit(APP_MASTER_WPID, WP_PORT_ENET6, 
       (loopback & 0x1) ? WPX_SERDES_LOOPBACK : WPX_SERDES_NORMAL); 
   App_TerminateOnError(status, "WPX_BoardSerdesInit 6()");

   status = WPX_BoardSerdesInit(APP_MASTER_WPID, WP_PORT_ENET7, 
       (loopback & 0x2) ? WPX_SERDES_LOOPBACK : WPX_SERDES_NORMAL); 
   App_TerminateOnError(status, "WPX_BoardSerdesInit 7()");

   status = WPX_BoardSerdesInit(APP_MASTER_WPID, WP_PORT_ENET8, 
       (loopback & 0x4) ? WPX_SERDES_LOOPBACK : WPX_SERDES_NORMAL); 
   App_TerminateOnError(status, "WPX_BoardSerdesInit 8()");

#else /* for WP3 SLB board*/ 

   status = WPX_BoardSerdesInit(APP_MASTER_WPID, WP_PORT_ENET9, WPX_SERDES_NORMAL); 
   App_TerminateOnError(status, "WPX_BoardSerdesInit 9()");

   status = WPX_BoardSerdesInit(APP_MASTER_WPID, WP_PORT_ENET10, WPX_SERDES_LOOPBACK);
   App_TerminateOnError(status, "WPX_BoardSerdesInit 10()");

   status = WPX_BoardSerdesInit(APP_MASTER_WPID, WP_PORT_ENET11, WPX_SERDES_LOOPBACK);
   App_TerminateOnError(status, "WPX_BoardSerdesInit 11()");

#endif

   /* Create BRG1 & BRG3 */
   /* for multi-winpath - take the BRG5 to not be used by test */
   status = WP_SysClockCreate( APP_MASTER_WPID, WP_BRG5, WP_BRG_SRC_BRGIN2, 2);
   WPE_TerminateOnError(status,"WP_SysClockCreate",__LINE__);

   status = WP_SysClockCreate( APP_MASTER_WPID, WP_BRG1, WP_BRG_SRC_BRGIN2, 2);
   WPE_TerminateOnError(status,"WP_SysClockCreate",__LINE__);

   status =WP_SysClockCreate( APP_MASTER_WPID, WP_BRG3, WP_BRG_SRC_BRGIN2, 2);
   WPE_TerminateOnError(status,"WP_SysClockCreate",__LINE__);

   status  = WP_SysSchedulerCreate(APP_MASTER_WPID, calendar);
   WPE_TerminateOnError(status, "WP_SysSchedulerCreate()",__LINE__);

}

/*****************************************************************************
 * Function name: App_PortsCreate
 * Description  : App_PortsCreate function creates all the required enet ports for 
 * this application Three enet ports are created for uplink and downlink.
 * Input  params: None
 * Output params: None
 * Return val   : None
 *****************************************************************************/
void App_PortsCreate(void)
{

   
#ifdef BOARD_WP3
   /* Create Enet1 port */
   eoam_system.h_port_enet[ENET1] = WP_PortCreate(APP_MASTER_WPID, WP_PORT_ENET6,
                                &port_enet_cfg);
   App_TerminateOnError(eoam_system.h_port_enet[ENET1], "WP_PortCreate() Enet6");
   
   /* Create Enet2 port */
   eoam_system.h_port_enet[ENET2] = WP_PortCreate(APP_MASTER_WPID, WP_PORT_ENET7,
                                &port_enet_cfg);
   App_TerminateOnError(eoam_system.h_port_enet[ENET2], "WP_PortCreate() Enet7");

   /* Create Enet3 port */
   eoam_system.h_port_enet[ENET3] = WP_PortCreate(APP_MASTER_WPID, WP_PORT_ENET8,
                                &port_enet_cfg);
   App_TerminateOnError(eoam_system.h_port_enet[ENET3], "WP_PortCreate() Enet8");
#else /* for WP3 SLB board*/ 
   /* Create Enet1 port */
   eoam_system.h_port_enet[ENET1] = WP_PortCreate(APP_MASTER_WPID, WP_PORT_ENET9,
                                &port_enet_cfg);
   App_TerminateOnError(eoam_system.h_port_enet[ENET1], "WP_PortCreate() Enet9");

   /* Create Enet2 port */
   eoam_system.h_port_enet[ENET2] = WP_PortCreate(APP_MASTER_WPID, WP_PORT_ENET10,
                                &port_enet_cfg);
   App_TerminateOnError(eoam_system.h_port_enet[ENET2], "WP_PortCreate() Enet10");

   /* Create Enet3 port */
   eoam_system.h_port_enet[ENET3] = WP_PortCreate(APP_MASTER_WPID, WP_PORT_ENET11,
                                &port_enet_cfg);
   App_TerminateOnError(eoam_system.h_port_enet[ENET3], "WP_PortCreate() Enet10");
   
#endif
   
   /* Create an IW Host port */
   eoam_system.h_port_iwhost= WP_PortCreate(APP_MASTER_WPID, WP_PORT_IW_HOST, NULL);
   App_TerminateOnError(eoam_system.h_port_iwhost, "WP_PortCreate() IW Host");
   
}

/*****************************************************************************
 * Function name: App_DevicesAllocCreate
 * Description  : This function creates Enet devices over the Enet
 *     ports created earlier. Also interworking host device is created for 
 *     the host generation and termination of the packets. 
 * Input  params: None
 * Output params: None
 * Return val   : None
 *****************************************************************************/
void App_DevicesAllocCreate(void)
{
    WP_device_enet_ex enet_device_extended_config[1] =
    {
        {
            /*duplexmode*/       WP_ENET_FULLDUPLEX,
            /*rx_maclookup_mode*/       WP_ENET_MACLOOKUP_TABLE,
            /*rx_flowcontrol*/       WP_ENET_FLOW_ENABLE,
            /*tx_flowcontrol*/       WP_ENET_FLOW_ENABLE,
            /*rx_addrmode*/       WP_ENET_ADDR_ACCEPTALL,
            /*phystatmode*/       WP_ENET_STAT_ENABLE,
            /*tx_statmode*/       WP_ENABLE,
            /*rx_statmode*/       WP_ENABLE,
            /*tx_duplicate*/       WP_DISABLE,
            /*rx_filters*/       0,
            /*rx_timestamp*/       WP_ENABLE,
            /*timestamp_type*/       WP_ENET_IEEE_1588_80BIT_TS1,
            /*max_tx_slow_channels*/  64,
            /* tx_timestamp */       WP_ENABLE
        },
    };
   
   /* Create device on Enet1 */
   device_enet_cfg.loopbackmode=WP_ENET_NORMAL; /* no loop back*/   
   MAC_COPY(device_enet_cfg.mac_addr, wt_mac_enet1);
   eoam_system.h_device_enet[ENET1] = WP_DeviceCreate(eoam_system.h_port_enet[ENET1], WP_PHY(0), WP_DEVICE_ENET,
                                    &device_enet_cfg);
   App_TerminateOnError(eoam_system.h_device_enet[ENET1], "WP_DeviceCreate() Enet1");
   
   /* Create device on Enet2 */
   device_enet_cfg.loopbackmode=WP_ENET_NORMAL; /* no loop back*/
   MAC_COPY(device_enet_cfg.mac_addr, wt_mac_enet2);
   eoam_system.h_device_enet[ENET2] = WP_DeviceCreate(eoam_system.h_port_enet[ENET2], WP_PHY(0), WP_DEVICE_ENET,
                                    &device_enet_cfg);
   App_TerminateOnError(eoam_system.h_device_enet[ENET2], "WP_DeviceCreate() Enet2");

    enet_device_extended_config->duplexmode = WP_ENET_FULLDUPLEX;
    enet_device_extended_config->rx_maclookup_mode = WP_ENET_MACLOOKUP_TABLE;
    enet_device_extended_config->rx_flowcontrol = WP_ENET_FLOW_ENABLE;
    enet_device_extended_config->tx_flowcontrol = WP_ENET_FLOW_ENABLE;
    enet_device_extended_config->rx_addrmode = WP_ENET_ADDR_ACCEPTALL;
    enet_device_extended_config->phystatmode = WP_ENET_STAT_ENABLE;
    enet_device_extended_config->tx_statmode = WP_ENABLE;
    enet_device_extended_config->rx_statmode = WP_ENABLE;
    enet_device_extended_config->tx_duplicate = WP_DISABLE;
    enet_device_extended_config->rx_filters = 0;
    enet_device_extended_config->rx_timestamp = WP_ENABLE;
    enet_device_extended_config->tx_timestamp_type = WP_ENET_IEEE_1588_80BIT_TS1;
    enet_device_extended_config->max_tx_slow_channels = 64;
    enet_device_extended_config->tx_timestamp = WP_ENABLE;
    enet_device_extended_config->rx_timestamp_type = WP_ENET_IEEE_1588_80BIT_TS1;
    device_enet_cfg.extended_params = enet_device_extended_config;
    status = WP_DeviceModify(eoam_system.h_device_enet[ENET2],
                            WP_DEV_MOD_ENET_TIMESTAMP_TYPE |
                            WP_DEV_MOD_ENET_TX_TIMESTAMP |
                            WP_DEV_MOD_ENET_RX_TIMESTAMP,
                            &device_enet_cfg);
    WPE_TerminateOnError(status, "WP_DeviceModify() GENET2",__LINE__);

   /* Create device on Enet3 */
   device_enet_cfg.extended_params = NULL;
   device_enet_cfg.loopbackmode=WP_ENET_NORMAL; /* no loop back*/
   MAC_COPY(device_enet_cfg.mac_addr, wt_mac_enet3);
   eoam_system.h_device_enet[ENET3] = WP_DeviceCreate(eoam_system.h_port_enet[ENET3], WP_PHY(0), WP_DEVICE_ENET,
                                    &device_enet_cfg);
   App_TerminateOnError(eoam_system.h_device_enet[ENET3], "WP_DeviceCreate() Enet3");
   
   /* Create an IW Host Device */
   eoam_system.h_device_iwhost= WP_DeviceCreate(eoam_system.h_port_iwhost, 0, WP_DEVICE_IW_HOST, NULL);
   App_TerminateOnError(eoam_system.h_device_iwhost, "WP_DeviceCreate() IW Host");
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

   /* Create Buffer pools */
   eoam_system.h_pool_buffer_iw = WP_PoolCreate(APP_MASTER_WPID, WP_pool_iwbuffer,
                                    &pool_buffer_data_iw_cfg);
   App_TerminateOnError(eoam_system.h_pool_buffer_iw, "WP_PoolCreate iwbuffer iw");


   /* Pool for host termination buffers */
   eoam_system.h_pool_256 = WP_PoolCreate(APP_MASTER_WPID, WP_pool_buffer,
                              &pool_buffer_data_256_cfg);
   App_TerminateOnError (eoam_system.h_pool_256, "PoolCreate() pool_256");

   eoam_system.h_pool_ring_host = WP_PoolCreate(APP_MASTER_WPID, WP_pool_ring,
                                    &pool_ring_data_cfg);
   App_TerminateOnError(eoam_system.h_pool_ring_host, "PoolCreate() pool_ring_host");

   /* Create an interworking queue node */
   qnode_iwq_cfg.adjunct_pool = eoam_system.h_pool_buffer_iw;
   eoam_system.h_qnode_iwq = WP_QNodeCreate(APP_MASTER_WPID,WP_QNODE_IWQ  | 
                                WP_QNODE_OPT_DEDICATED_RX_HWQ |
                                WP_QNODE_OPT_FMU,
                                &qnode_iwq_cfg);
   App_TerminateOnError(eoam_system.h_qnode_iwq, "WP_QNodeCreate() h_qnode_iwq");

   /* Host termination qnode */
   qnode_hostq_cfg.pool_buffer = eoam_system.h_pool_256;
   qnode_hostq_cfg.pool_ring = eoam_system.h_pool_ring_host;
   eoam_system.h_qnode_host = WP_QNodeCreate(APP_MASTER_WPID, WP_QNODE_HOSTQ, &qnode_hostq_cfg);
   App_TerminateOnError(eoam_system.h_qnode_host, "WP_QnodeCreate() h_qnode_host");

   status = WP_ControlRegister(WP_EVENT_RX_INDICATE, App_EventRxIndicate);
   App_TerminateOnError(status, "WP_ControlRegister()");
   
}

/*****************************************************************************
 * Function name: App_SystemSetup
 * Description  : This function initialzies the Control Register with the call
 *    back functions and creates bridging system. The system resources are commited
 *    with the function WP_SysCommit()   
 * Input  params: None
 * Output params: None
 * Return val   : None
 *****************************************************************************/
void App_SystemSetup(void)
{

#if WT_DEBUG_MODE
    WP_status   status;
   status = WP_ControlRegister(WP_DEBUG_CALLBACK_FILE_LINE_ERROR,WPE_DebugCallback);
   App_TerminateOnError(status, "WP_ControlRegister()");

   status = WP_ControlRegister(WP_DEBUG_CALLBACK_WDDI_CALL,WPE_WddiCall);
   App_TerminateOnError(status, "WP_ControlRegister()");
#endif
   /*   Interworking system create      */ 

   eoam_system.h_iw_sys_bridge[UL_IW_SYS] = WP_IwSystemCreate(APP_MASTER_WPID,
                                             WP_IW_VLAN_AWARE_BRIDGING_MODE,
                                             iw_sys_bridge_param);
   WPE_TerminateOnError(eoam_system.h_iw_sys_bridge[UL_IW_SYS]," WP_IwSystemCreate()",__LINE__);

   eoam_system.h_iw_sys_bridge[DL_IW_SYS] = WP_IwSystemCreate(APP_MASTER_WPID,
                                             WP_IW_VLAN_AWARE_BRIDGING_MODE,
                                             iw_sys_bridge_param);
   WPE_TerminateOnError(eoam_system.h_iw_sys_bridge[DL_IW_SYS]," WP_IwSystemCreate()",__LINE__);

}

/*****************************************************************************
 * Function name: App_ChannelsCreate
 * Description  : This function creates the Enet channels, host channels will be created later
 * Input  params: None
 * Output params: None
 * Return val   : None
 *
 * Note : ATM channels are created in the function App_ImaChannelsCreate()
 *****************************************************************************/   
void App_ChannelsCreate(void)
{

   /* Create Enet channels */
   ch_enet_cfg.iwmode = WP_PKTCH_IWM_ENABLE;
   eoam_system.rx_enet_handle[ENET1] = WP_ChannelCreate(tag_enet1_rx, eoam_system.h_device_enet[ENET1],
                                 eoam_system.h_qnode_iwq,WP_CH_RX, WP_ENET,
                                 &ch_enet_cfg);
   App_TerminateOnError(eoam_system.rx_enet_handle[ENET1], "WP_ChannelCreate() Enet Rx");
   
   //ch_enet_cfg.iwmode = WP_PKTCH_IWM_DISABLE;
   eoam_system.tx_enet_handle[ENET1] = WP_ChannelCreate(tag_enet1_tx++, eoam_system.h_device_enet[ENET1],
                                      eoam_system.h_qnode_iwq,WP_CH_TX, WP_ENET,
                                      &ch_enet_cfg);
   App_TerminateOnError(eoam_system.tx_enet_handle[ENET1], "WP_ChannelCreate() Enet Tx");
   
   /* Create Enet 2 channels */
   //ch_enet_cfg.iwmode = WP_PKTCH_IWM_DISABLE;
   eoam_system.rx_enet_handle[ENET2] = WP_ChannelCreate(tag_enet2_rx, eoam_system.h_device_enet[ENET2],
                                 eoam_system.h_qnode_iwq,WP_CH_RX, WP_ENET,
                                 &ch_enet_cfg);
   App_TerminateOnError(eoam_system.rx_enet_handle[ENET2], "WP_ChannelCreate() Enet Rx");

   /* NOTE this channel is in NON-IW mode, used to send out a packet from the host out of ENET2 */
   //ch_enet_cfg.iwmode = WP_PKTCH_IWM_DISABLE;
   eoam_system.tx_enet_handle[ENET2] = WP_ChannelCreate(tag_enet2_tx, eoam_system.h_device_enet[ENET2],
                                 eoam_system.h_qnode_iwq,WP_CH_TX, WP_ENET,
                                 &ch_enet_cfg);
   App_TerminateOnError(eoam_system.tx_enet_handle[ENET2], "WP_ChannelCreate() Enet Tx");

   /* Create Enet 3 channels */
   //ch_enet_cfg.iwmode = WP_PKTCH_IWM_ENABLE;
   eoam_system.rx_enet_handle[ENET3] = WP_ChannelCreate(tag_enet3_rx, eoam_system.h_device_enet[ENET3],
                                 eoam_system.h_qnode_iwq,WP_CH_RX, WP_ENET,
                                 &ch_enet_cfg);
   App_TerminateOnError(eoam_system.rx_enet_handle[ENET3], "WP_ChannelCreate() Enet Rx");
   
   eoam_system.tx_enet_handle[ENET3] = WP_ChannelCreate(tag_enet3_tx++, eoam_system.h_device_enet[ENET3],
                                      eoam_system.h_qnode_iwq,WP_CH_TX, WP_ENET,
                                      &ch_enet_cfg);
   App_TerminateOnError(eoam_system.tx_enet_handle[ENET3], "WP_ChannelCreate() Enet Tx");

}

/*****************************************************************************
 * Function name: App_PortsDevicesEnable
 * Description  : Enet ports and devices are enebled in this function
 * Input  params: None
 * Output params: None
 * Return val   : None
 *****************************************************************************/
void App_PortsDevicesEnable(void)
{

    WP_status   status;
   /* Enable all ports and devices */
   status = WP_PortEnable(eoam_system.h_port_enet[ENET1], WP_DIRECTION_DUPLEX);
   App_TerminateOnError(status, "WP_PortEnable() Enet1");

   status = WP_DeviceEnable(eoam_system.h_device_enet[ENET1], WP_DIRECTION_DUPLEX);
   App_TerminateOnError(status, "WP_DeviceEnable() Enet1");

   status = WP_PortEnable(eoam_system.h_port_enet[ENET2], WP_DIRECTION_DUPLEX);
   App_TerminateOnError(status, "WP_PortEnable() Enet2");

   status = WP_DeviceEnable(eoam_system.h_device_enet[ENET2], WP_DIRECTION_DUPLEX);
   App_TerminateOnError(status, "WP_DeviceEnable() Enet2");

   status = WP_PortEnable(eoam_system.h_port_enet[ENET3], WP_DIRECTION_DUPLEX);
   App_TerminateOnError(status, "WP_PortEnable() Enet3");

   status = WP_DeviceEnable(eoam_system.h_device_enet[ENET3], WP_DIRECTION_DUPLEX);
   App_TerminateOnError(status, "WP_DeviceEnable() Enet3");
   
}

/*****************************************************************************
 * Function name: App_ChannelsEnable
 * Description  : Enet channels are enebled in this function
 * Input  params: None
 * Output params: None
 * Return val   : None
 *****************************************************************************/
void App_ChannelsEnable(void)

{
WP_status   status;

   status = WP_ChannelEnable(eoam_system.rx_enet_handle[ENET1]);
   App_TerminateOnError(status, "WP_ChannelEnable ENET1 RX");

   status = WP_ChannelEnable(eoam_system.tx_enet_handle[ENET1]);
   App_TerminateOnError(status, "WP_ChannelEnable ENET1 TX");

   status = WP_ChannelEnable(eoam_system.rx_enet_handle[ENET2]);
   App_TerminateOnError(status, "WP_ChannelEnable ENET2 RX");

   status = WP_ChannelEnable(eoam_system.tx_enet_handle[ENET2]);
   App_TerminateOnError(status, "WP_ChannelEnable ENET2 TX");

   status = WP_ChannelEnable(eoam_system.rx_enet_handle[ENET3]);
   App_TerminateOnError(status, "WP_ChannelEnable ENET3 RX");

   status = WP_ChannelEnable(eoam_system.tx_enet_handle[ENET3]);
   App_TerminateOnError(status, "WP_ChannelEnable ENET3 TX");

}

void App_PtpClockCreate(void)
{
#ifdef WP_HW_WINPATH3
   WP_ptp_clock_domain ptp_cd_descriptor={0};

   ptp_cd_descriptor.clock_source_mode = WP_PTP_DCO;
   ptp_cd_descriptor.clock_source_input = WP_PTP_SYSTEM_CLK;
   ptp_cd_descriptor.service_clock_frequency = 10000000;
   ptp_cd_descriptor.clock_domain_hw_id = 1;
   ptp_cd_descriptor.ts_source_mode = 1;
   ptp_cd_descriptor.clock_source_frequency = (WPL_GetProcessorSpeed()/1000)*1000;
   ptp_cd_descriptor.ts_source_frequency = (WPL_GetProcessorSpeed()/1000)*1000;
   //printf("clock_frequency = %u\n\n",clock_frequency);
   eoam_system.h_clock_domain = WP_PtpClockDomainCreate(APP_MASTER_WPID, &ptp_cd_descriptor);
   WPE_TerminateOnError(eoam_system.h_clock_domain, "WP_PtpClockDomainCreate",__LINE__);
#endif
}

/*****************************************************************************
 * Function name: App_EnetOamSystemInit
 * Description  : Initializes the Tags used in this application. These Tags can 
 *                be of any value but need to be unique, i.e, two different Tags
 *                can not have the same value.
 * Input  params: None
 * Output params: None
 * Return val   : None
 *****************************************************************************/
void App_EnetOamSystemInit(void)
{
   /* Sets up a Clock Domain entity */
   App_PtpClockCreate();
   
   /*Creates bridge port and flow aggregations.*/
   WPE_HostCreate();

   /*create filters in the system, pceif for rx binding */
   WPE_FilterDefinition();

   /* Create an interworking (Rxbinding,Tx binding, flow aggregations create and flows add) */
   WPE_IwSystemBuild();

   /*create basic supporting pce rules in the system */
   WPE_BasicPceRulesCreate();

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
    WP_status result = WP_OK;

    //do the performance test
    result |= WPE_Eoam_Performance_TestStart();
    WP_Delay (5000000);
    result |= WPE_Eoam_Performance_TestExit();

    App_CheckStats (&result);

    if (result == WP_OK)
    {
        printf ("test passed!\n");
    }
    else
    {
        printf ("test failed!\n");
    }                           

   	printf("This is the last test case in this application!\n");
   	printf("This test case need further manually test!\n");

    /*Quit test */
    WPE_Eoam_Quit (1);
}

/***************************************************************************** 
* Function name: App_CheckStats 
* Description  : Check statistics correctivity of performance demo 
* Input  params: None 
* Output params: None * Return val   : None 
*****************************************************************************/
void App_CheckStats (WP_status * test_result)
{
    WP_status status;
    WP_stats_enet stats_enet;
    WP_bport_stats bport_stats;
    WP_iw_flow_stats iw_flow_stats;

    /*Check ENET2 stats*/
    memset(&stats_enet,0,sizeof(stats_enet));
    status = WP_DeviceStatistics(eoam_system.h_device_enet[ENET2], &stats_enet);
    WPE_TerminateOnError(status, "WP_DeviceStatistics",__LINE__);

    if (stats_enet.txrx_frames_127 == 0)
    {
   	*test_result = (-1);
       printf("ENET2 txrx_frames_127 check fail!\n");
    }

    if (stats_enet.txrx_frames_64 == 0)
    {
        *test_result = (-1);
           printf("ENET2 txrx_frames_64 check fail!\n");    
    }

    if ((stats_enet.rx_packets == 0) || (stats_enet.tx_packets == 0) || (stats_enet.rx_packets != stats_enet.tx_packets))
    {
   	*test_result = (-1);
       printf("ENET2 tx_packets & rx_packets check fail!\n");
    }

    /*Check Bport*/
	
    memset(&bport_stats, 0, sizeof(bport_stats));
    status = WP_IwPortStatistics(eoam_system.bports[ENET_OAM_DUMMY_BPORT], &bport_stats);
    WPE_TerminateOnError(status, "WP_BportStatistics",__LINE__);

    if (bport_stats.rx_valid_packets == 0)
    {
   	*test_result = (-1);
       printf("Bport ENET2 rx_valid_packets check fail!\n");
    }

    memset(&bport_stats, 0, sizeof(bport_stats));
    status = WP_IwPortStatistics(eoam_system.bports[ENET2_DUMMY_BPORT], &bport_stats);
    WPE_TerminateOnError(status, "WP_BportStatistics",__LINE__);	

    if (bport_stats.deny_cfm_frames == 0)
    {
   	*test_result = (-1);
       printf("Bport ENET2 deny_cfm_frames check fail!\n");
    }

    memset(&iw_flow_stats, 0, sizeof(iw_flow_stats));
    status = WP_IwFlowStatistics(eoam_system.flow_agg[FA_PORT_ENET2], WP_IW_FLOW_STAT, &iw_flow_stats);
    WPE_TerminateOnError(status, "WP_IwFlowStatistics",__LINE__);

    if (iw_flow_stats.forward_packet == 0)
    {
   	*test_result = (-1);
       printf("Flow Agg ENET2 forward_packet check fail!\n");
    }

    /*Dump all statistics if failed */
    if (*test_result != WP_OK)
    {
        WPE_Eoam_PM_ShowStats(WPE_STAT_ENET, 1);
        WPE_Eoam_PM_ShowStats(WPE_STAT_BPORT, 1);
	 WPE_Eoam_PM_ShowStats(MAX_FLOW_AGG, 1);
    }
}

void  WPE_PTPModuleInitWP3(void)
{
   WP_ptp_system    ptp_system_params = {0};
   WP_status status = WP_OK;

   ptp_system_params.max_ptp_ports = WPE_MAX_PTP_PORTS ;

   /* WDDI */
   status = WP_ModuleInit(WP_SYSHANDLE(0),WP_WDDI_MODULE_PTP, &ptp_system_params);
   WPE_TerminateOnError(status, "WP_ModuleInit(WP_WDDI_MODULE_PTP)",__LINE__);
}

void WPE_PecsCreate (void)
{
   WP_U32 ii;
   WP_gpe_pecs gpe_pecs_cfg[2] = {
      {
      &brouter_pecs_pecs_image,   /*pecs_image_info */
      &WPE_GpeBrouterPecsService  /*pecs_service_fp */
      }
      ,
      {
      &mpls_pecs_pecs_image,      /*pecs_image_info */
      &WPE_GpeMplsPecsService     /*pecs_service_fp */
      }
   };

   /* only brouter_pecs are used in this demo. if load mpls_pces
   this demo will over size in demo board RevA, so not load it */
   for (ii = 0; ii < 1; ii++)
   {
      /* It loads and relocates the PECS DPS image from the predefined PECS library
       * to the DPS program RAM. It also registers the PECS Service Function.
       */
      eoam_system.h_pecs[ii] =
         WP_GpePecsCreate (WP_WINPATH (0), WP_GPE_PECS,
                           (void *) &gpe_pecs_cfg[ii]);
      //printf ("WP_GpePecsCreate: handle returned: %x\n", eoam_system.h_pecs[ii]);
      WPE_TerminateOnError (eoam_system.h_pecs[ii], "WP_GpePecsCreate", __LINE__);
   }
}

/*
 *this function creates
 *          host tx,rx channels.
 *         flow aggregtion to the host.
 *         bridge port to the host.
 */
static void WPE_HostCreate()
{


   static WP_tx_binding  tx_binding_iw[1]=
      {
         {
            /* res0;*/ 0,
            /* dci_mode;*/  WP_IW_DYN_CH_INSERT_DISABLE,
            /* maxt;*/55,
         }
      };
   static WP_ch_iw_rx ch_config_iw[1]=
      {
         {
            /* pqblock */ 0,
            /* pqlevel */ 0,
            /* tx_binding_type */ WP_IW_TX_BINDING,
            /* tx_binding_config */ tx_binding_iw
         }
      };

   WP_iw_agg_hq flooding_tx[1] =
      {
         {
            /* tag */            0,
            /* txfunc */         0,
            /* hq_agg_mode */    WP_IW_HQ_NOT_RESERVED,
            /* mtu */            1536,
            /* interruptqueue */ WP_IW_IRQT0,
            /* intmode */        WP_IW_INT_ENABLE,
            /* statmode */       WP_IW_STAT_ENABLE,
            /* timestamp_mode */ WP_IW_TIME_STAMP_ENABLE,
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
         }

      };

   WP_bridge_port iwhost_bport_config[1] =
      {
         {
            /* tag */ 0x2000,
            /* direct_mapping */ WP_IW_DIRECT_MAP_DISABLE,
            /* flow_agg */ 0,
            /* flooding_term_mode */ WP_IW_HOST_TERM_MODE,
            /* learning_mode */ WP_IW_LEARNING_DISABLE,
            /* in_filter_mode */ WP_IW_INGRESS_FILTER_DISABLE,
             /* vlan_param */
            {
               /* vlan_acceptance_mode */ WP_IW_ACCEPT_TAGGED_UNTAGGED,
               /* vlan_tag */ 0x5,
               /* vlan_tunnel*/ 0,
               /* vlan_priority_enforce_mode*/0,
               /* stag_vlan_etype*/DEFAULT_STAG_TYPE

            },
            /*max_mac_addresses */ 100,
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
   /* The following structure is used to configure the Host send (tx) channel.
      For information on the parameters see the Ingress chapter */
   WP_ch_iw_tx ch_iw_tx_config[1] =
      {
         {
            /* iw_system */ 0,
            /* reserved */ 0,
            /* group_id */ WP_IW_CH_GROUP_ID_NULL,
            /* bridging_group_tag */ 0,
            /* input_port */ 0
         }
      };

   /*create host rx channel*/
  eoam_system.rx_host_handle = WP_ChannelCreate(tag_iwhost_rx1,
                                        eoam_system.h_device_iwhost,
                                        eoam_system.h_qnode_iwq,
                                        WP_CH_RX,
                                        WP_IW_HOST,
                                        &ch_config_iw[0]);
   WPE_TerminateOnError(eoam_system.rx_host_handle, "WP_ChannelCreate() Host sys1",__LINE__);


   /*creates flow Aggregation for the host */
   flooding_tx[0].txfunc = eoam_system.rx_host_handle;

   eoam_system.flow_agg[FLOW_AGG_HOST] = WP_IwFlowAggregationCreate(APP_MASTER_WPID,
                                                            WP_IW_HOST_QUEUE_MODE,
                                                            &flooding_tx);
   WPE_TerminateOnError(eoam_system.flow_agg[FLOW_AGG_HOST]," WP_IwFlowAggregationCreate()",__LINE__);


   /* Create host iw bport for iw tx channel. */
   iwhost_bport_config[0].flow_agg = eoam_system.flow_agg[FLOW_AGG_HOST];

   eoam_system.bports[HOST_BPORT_DL] = WP_IwPortCreate(eoam_system.h_iw_sys_bridge[DL_IW_SYS], &iwhost_bport_config[0]);
   WPE_TerminateOnError(eoam_system.bports[HOST_BPORT_DL]," WP_IwPortCreate()",__LINE__);

   eoam_system.bports[HOST_BPORT_UL] = WP_IwPortCreate(eoam_system.h_iw_sys_bridge[UL_IW_SYS], &iwhost_bport_config[0]);
   WPE_TerminateOnError(eoam_system.bports[HOST_BPORT_UL]," WP_IwPortCreate()",__LINE__);

   /*modify the flow aggregation bridge port*/
   eoam_system.rx_res_enet_oam_host = WP_ChannelCreate(tag_iwhost_rx_oam,
                                           eoam_system.h_device_iwhost,
                                           eoam_system.h_qnode_iwq,
                                           WP_CH_RX,
                                           WP_IW_HOST,
                                           &ch_config_iw[0]);
   WPE_TerminateOnError(eoam_system.rx_res_enet_oam_host, "WP_ChannelCreate() Host sys1",__LINE__);

   flooding_tx[0].input_bport = eoam_system.bports[HOST_BPORT_UL];
   flooding_tx[0].txfunc = eoam_system.rx_res_enet_oam_host;
   flooding_tx[0].hq_agg_mode = WP_IW_HQ_ENET_OAM_TERMINATION;
   eoam_system.flow_agg[FA_RES_ENET_OAM] = WP_IwFlowAggregationCreate(APP_MASTER_WPID,
                                                                      WP_IW_HOST_QUEUE_MODE,
                                                                      &flooding_tx);
   WPE_TerminateOnError(eoam_system.flow_agg[FA_RES_ENET_OAM]," WP_IwFlowAggregationCreate()",__LINE__);


   /* Create iw host tx channel */
   ch_iw_tx_config[0].input_port = eoam_system.bports[HOST_BPORT_DL];
   ch_iw_tx_config[0].iw_system = eoam_system.h_iw_sys_bridge[DL_IW_SYS];
   eoam_system.tx_host_handle = WP_ChannelCreate(tag_iwhost_rx1+1,
                                                 eoam_system.h_device_iwhost,
                                                 eoam_system.h_qnode_iwq,
                                                 WP_CH_TX,
                                                 WP_IW_HOST,
                                                 &ch_iw_tx_config[0]);
   WPE_TerminateOnError(eoam_system.tx_host_handle," WP_ChannelCreate()",__LINE__);
}

static void
WPE_BridgingPortCreate()
{
    int ii;
   WP_iw_bport_enet_oam_params enet_oam_params;

   /* Create Bridge Port which support Ethernet OAM.*/
   enet_oam_params.enet_oam_mode = WP_ENABLE;
   enet_oam_params.use_macsa_rem_mep_lookup = WP_ENABLE;

    for (ii = 0; ii < WP_IW_ENET_OAM_MAX_MD_LEVELS; ii++)
    {
        enet_oam_params.md_action[ii].ma_vlan_lookup_mode = 
                        WP_IW_ENET_OAM_VLAN_LOOKUP_USE_STAG_VLAN_ID;
        enet_oam_params.md_action[ii].remote_mep_vlan_lookup_mode = 
                        WP_IW_ENET_OAM_VLAN_LOOKUP_USE_STAG_VLAN_ID;

        //enet_oam_params.md_action[ii].default_action = WP_IW_ENET_OAM_DA_DROP;
        enet_oam_params.md_action[ii].default_action = WP_IW_ENET_OAM_DA_FORWARD;
        enet_oam_params.md_action[ii].md_flow_aggregation = eoam_system.flow_agg[FA_RES_ENET_OAM];
    }

   bport_conf[0].enet_oam_params = &enet_oam_params;

   bport_conf[0].tag = ENET2_BPORT;
   bport_conf[0].flow_agg = eoam_system.flow_agg[FLOW_AGG_HOST];
   bport_conf[0].vlan_param.vlan_tag = DEFAULT_VLAN_TAG;     
   
   eoam_system.bports[ENET2_BPORT] = WP_IwPortCreate(eoam_system.h_iw_sys_bridge[UL_IW_SYS],
                                         bport_conf);
    App_TerminateOnError(eoam_system.bports[ENET2_BPORT],"WP_IwBridgingPortCreate(ENET2_BPORT)");
    
   enet_oam_params.enet_oam_mode = WP_DISABLE;     //Disable ENET OAM for ENET1 and ENET3
   bport_conf[0].enet_oam_params = NULL;
   bport_conf[0].tag = ENET1_BPORT;
   bport_conf[0].vlan_param.vlan_tag = DEFAULT_VLAN_TAG;
   eoam_system.bports[ENET1_BPORT] = WP_IwPortCreate(eoam_system.h_iw_sys_bridge[DL_IW_SYS],
                                        bport_conf);
    App_TerminateOnError(eoam_system.bports[ENET1_BPORT],"WP_IwBridgingPortCreate(ENET1_BPORT)");
    
   bport_conf[0].tag = ENET3_BPORT;
   bport_conf[0].vlan_param.vlan_tag = DEFAULT_VLAN_TAG;
   eoam_system.bports[ENET3_BPORT] = WP_IwPortCreate(eoam_system.h_iw_sys_bridge[UL_IW_SYS],
                                        bport_conf);
    App_TerminateOnError(eoam_system.bports[ENET3_BPORT],"WP_IwBridgingPortCreate(ENET3_BPORT)");

   bport_conf[0].classification_flag = WP_IW_IWPORT_CLASSIFICATION_DISABLED;
   bport_conf[0].tag = ENET_OAM_DUMMY_BPORT;
   bport_conf[0].vlan_param.vlan_tag = DEFAULT_VLAN_TAG;
   bport_conf[0].enet_oam_params = NULL;
   bport_conf->flow_agg = eoam_system.flow_agg[FLOW_AGG_HOST]; /* deafult flow aggregation. for the ports*/
   eoam_system.bports[ENET_OAM_DUMMY_BPORT] = WP_IwPortCreate(eoam_system.h_iw_sys_bridge[DL_IW_SYS],
                                                        bport_conf);
    App_TerminateOnError(eoam_system.bports[ENET_OAM_DUMMY_BPORT],"WP_IwBridgingPortCreate(ENET_OAM_DUMMY_BPORT)");

    /* Create ethernet dummy Port which support Ethernet OAM.*/
   memset(&enet_oam_params, 0, sizeof(WP_iw_bport_enet_oam_params));
        
   enet_oam_params.enet_oam_mode = WP_ENABLE;
   enet_oam_params.use_macsa_rem_mep_lookup = WP_ENABLE;
     
   for (ii = 0; ii < WP_IW_ENET_OAM_MAX_MD_LEVELS; ii++)
   {
      enet_oam_params.md_action[ii].ma_vlan_lookup_mode =
         WP_IW_ENET_OAM_VLAN_LOOKUP_USE_STAG_VLAN_ID |
         WP_IW_ENET_OAM_VLAN_LOOKUP_USE_CTAG_VLAN_ID ;  //STAG & CTAG

      enet_oam_params.md_action[ii].remote_mep_vlan_lookup_mode =
         WP_IW_ENET_OAM_VLAN_LOOKUP_USE_STAG_VLAN_ID |
         WP_IW_ENET_OAM_VLAN_LOOKUP_USE_CTAG_VLAN_ID ;  //STAG & CTAG

         enet_oam_params.md_action[ii].default_action = WP_IW_ENET_OAM_DA_DROP;
   }
     
   bport_conf[0].enet_oam_params = &enet_oam_params;
     
   bport_conf[0].tag = ENET2_DUMMY_BPORT;
   bport_conf[0].flow_agg = eoam_system.flow_agg[FLOW_AGG_HOST];  //default flow agg
   bport_conf[0].vlan_param.vlan_tag = DEFAULT_VLAN_TAG;     
   bport_conf[0].classification_flag = WP_IW_IWPORT_CLASSIFICATION_ENABLED;
   
   eoam_system.bports[ENET2_DUMMY_BPORT] = WP_IwPortCreate(eoam_system.h_iw_sys_bridge[UL_IW_SYS],
                                              bport_conf);
    App_TerminateOnError(eoam_system.bports[ENET2_DUMMY_BPORT],"WP_IwBridgingPortCreate(ENET2_DUMMY_BPORT)");

}

WP_status WPE_DebugCallback(const WP_CHAR *file, const WP_CHAR *function,
                            WP_S32 line, WP_S32 error_id)
{
   if (error_id)
      printf("\n\nfile = %s , line = %d\nfunction = %s\n",file,line,function);
   return WP_OK;
}

void WPE_WddiCall(const WP_CHAR *file,
                  const WP_CHAR *function,
                  WP_S32 line)
{

}

static void WPE_IwSystemBuild()
{
   /* creates flow aggergations */

   WPE_BridgingPortCreate();

   WPE_FlowAggregationCreate();

   WPE_IwPortModify();
   
   /*bind rx and tx channels to bridge port*/
   WPE_BindRx();
   WPE_BindTx();
}

static void WPE_FlowAggregationCreate()
{
   WP_iw_agg_generic tx_agg_enet[1] = {
      {
       /*tag */ 2,
       /*txfunc */ 0,
       /*iw_port */ 0,
       /*rfcs */ WP_IW_RFCS_ENABLE,
       /*interruptqueue; */ WP_IW_IRQT1,
       /*error_pkt_mode */ WP_IW_ERRPKT_DISCARD,
       /*intmode; */ WP_IW_INT_DISABLE,
       /*statmode; */ WP_IW_STAT_ENABLE,
       /*timestamp_mode; */ WP_IW_TIME_STAMP_DISABLE,
       /*mtu; */ 9216,
       /*flow_agg_type; */ WP_IW_FLOW_AGG_PRIMARY,
       /*policer_handle; */ 0,
       /*pecs_handle; */ 0,
       /*pecs_flow_info; */ 0,
       /*pecs_global_info_handle; */ 0,
       },
   };

   WPE_gpe_brouter_pecs_flow_info brouter_pecs_flow_info[] = {
      /*  */
      {
       WPE_GPE_BROUTER_PECS_EXTRACT_DISABLE, /* header_extract_mode */
       0,                       /* extraction_size */
       WPE_GPE_BROUTER_PECS_ADD_DISABLE,  /* prefix_add_mode */
       4,                       /* prefix_add_size */
       14,                      /* prefix_ip_offset */
       WPE_GPE_BROUTER_PECS_REPLACE_MAC_DISABLE,   /* mac_replace_mode */
       {0x11, 0x22, 0x33, 0x44, 0x55, 0x00}, /* mac_da */
       {0x66, 0x77, 0x88, 0x99, 0xaa, 0x00}, /* mac_sa */
       0,                       /* vlan_edit_options */

       {0},                     /* prefix_remark_options */
       WPE_GPE_BROUTER_PECS_TTL_DISABLE,  /* ttl_mode */
       WPE_GPE_BROUTER_PECS_TOS_REMARKING_DISABLE, /* tos_remarking_mode */
       {                        /* prefix */
        0x11, 0x22, 0x33, 0x44, 0x55, 0x00,
        0x11, 0x12, 0x13, 0x14, 0x17, 0x00,
        0x81, 0, 0, 1,
        0x88, 0x47,
        0x11, 0x11, 0x10, 0x80,
        0x22, 0x22, 0x21, 0x80,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0}
       }
   };   

   /*enet2*/
   tx_agg_enet->txfunc = eoam_system.tx_enet_handle[ENET2];
   tx_agg_enet->iw_port = eoam_system.bports[ENET2_BPORT];
   tx_agg_enet->pecs_handle = eoam_system.h_pecs[0];
   tx_agg_enet->pecs_flow_info = (void *) &brouter_pecs_flow_info[0];

   eoam_system.flow_agg[FA_PORT_ENET2] = WP_IwFlowAggregationCreate(APP_MASTER_WPID,
                                                                    WP_IW_GENERIC_MODE,
                                                                    tx_agg_enet);
   WPE_TerminateOnError(eoam_system.flow_agg[FA_PORT_ENET2]," WP_IwFlowAggregationCreate()",__LINE__);

   /*enet3*/
   tx_agg_enet->iw_port = eoam_system.bports[ENET3_BPORT];
   tx_agg_enet->txfunc = eoam_system.tx_enet_handle[ENET3];

   eoam_system.flow_agg[FA_PORT_ENET3] = WP_IwFlowAggregationCreate(APP_MASTER_WPID,
                                                                    WP_IW_GENERIC_MODE,
                                                                    tx_agg_enet);
   WPE_TerminateOnError(eoam_system.flow_agg[FA_PORT_ENET3]," WP_IwFlowAggregationCreate()",__LINE__);

   /*enet1*/
   tx_agg_enet->iw_port = eoam_system.bports[ENET1_BPORT];
   tx_agg_enet->txfunc = eoam_system.tx_enet_handle[ENET1];

   eoam_system.flow_agg[FA_PORT_ENET1] = WP_IwFlowAggregationCreate(APP_MASTER_WPID,
                                                                    WP_IW_GENERIC_MODE,
                                                                    tx_agg_enet);
   WPE_TerminateOnError(eoam_system.flow_agg[FA_PORT_ENET1]," WP_IwFlowAggregationCreate()",__LINE__);

}

static void WPE_IwPortModify()
{

   /*bport of enet2*/
   bport_conf->flow_agg = eoam_system.flow_agg[FA_PORT_ENET1]; /*  enet2 default flow agg is enet1*/
   bport_conf[0].tag = ENET2_BPORT;
   bport_conf[0].vlan_param.vlan_tag = DEFAULT_VLAN_TAG;
   
   status = WP_IwPortModify(eoam_system.bports[ENET2_BPORT],
                            WP_IW_BPORT_MOD_FLOW_AGG,
                            bport_conf);
   WPE_TerminateOnError(status,"WP_IwPortModify()",__LINE__);

   /*bport of enet3*/
   bport_conf->flow_agg = eoam_system.flow_agg[FA_PORT_ENET1]; /*  enet3 default flow agg is enet1*/
   bport_conf[0].tag = ENET3_BPORT;
   bport_conf[0].enet_oam_params = NULL;
   bport_conf[0].vlan_param.vlan_tag = DEFAULT_VLAN_TAG;
   
   status = WP_IwPortModify(eoam_system.bports[ENET3_BPORT],
                            WP_IW_BPORT_MOD_FLOW_AGG,
                            bport_conf);
   WPE_TerminateOnError(status,"WP_IwPortModify()",__LINE__);

   /*bport of enet1*/
   bport_conf->flow_agg = eoam_system.flow_agg[FA_PORT_ENET2]; /*  enet1 default flow agg is enet2*/
   bport_conf[0].tag = ENET1_BPORT;
   bport_conf[0].vlan_param.vlan_tag = DEFAULT_VLAN_TAG;
   
   status = WP_IwPortModify(eoam_system.bports[ENET1_BPORT],
                            WP_IW_BPORT_MOD_FLOW_AGG,
                            bport_conf);
   WPE_TerminateOnError(status,"WP_IwPortModify()",__LINE__);

   bport_conf->flow_agg = eoam_system.flow_agg[FA_PORT_ENET1]; /*  enet2 default flow agg is enet1*/
   bport_conf[0].tag = ENET2_DUMMY_BPORT;
   bport_conf[0].vlan_param.vlan_tag = DEFAULT_VLAN_TAG;
   
   status = WP_IwPortModify(eoam_system.bports[ENET2_DUMMY_BPORT],
                            WP_IW_BPORT_MOD_FLOW_AGG,
                            bport_conf);
   WPE_TerminateOnError(status,"WP_IwPortModify()",__LINE__);

   bport_conf[0].classification_flag = WP_IW_IWPORT_CLASSIFICATION_DISABLED;
   bport_conf[0].tag = ENET_OAM_DUMMY_BPORT;
   bport_conf[0].vlan_param.vlan_tag = DEFAULT_VLAN_TAG;
   bport_conf[0].enet_oam_params = NULL;
   bport_conf->flow_agg = eoam_system.flow_agg[FA_PORT_ENET2];
   status = WP_IwPortModify(eoam_system.bports[ENET_OAM_DUMMY_BPORT],
                            WP_IW_BPORT_MOD_FLOW_AGG,
                            bport_conf);
   WPE_TerminateOnError(eoam_system.bports[ENET_OAM_DUMMY_BPORT],"WP_IwPortModify()",__LINE__);

}

void WPE_PceFilterCreate(void)
{
   WP_pce_filter_stat stats;
   WP_pce_filter_classification filter_class = { 0 };

   memset(&stats, 0, sizeof(stats));
   memset (&filter_class, 0, sizeof (filter_class));

   //creating filter1
   filter_class.no_match_action = WP_PCE_FILTER_NO_MATCH_CONTINUE;
   filter_class.no_fields_action = WP_PCE_FILTER_NO_FIELDS_CONTINUE; 

   filter_class.no_match_result[0].result_type = WP_PCE_RESULT_LAST;

   filter_class.filter_fields[0].field_id = WP_PCE_FIELD_ID_MAC_DA;
   filter_class.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_class.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   filter_class.filter_fields[1].field_id = WP_PCE_FIELD_ID_MAC_SA;
   filter_class.filter_fields[1].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_class.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   filter_class.filter_fields[2].field_id = WP_PCE_FIELD_ID_VLAN_TAG;
   filter_class.filter_fields[2].field_mode =WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_class.filter_fields[2].mask_mode = WP_PCE_FIELD_MASK_USED;
   filter_class.filter_fields[2].mask.vlan_tag = WP_PCE_MASK_VLAN_ID;

   filter_class.filter_fields[3].field_id = WP_PCE_FIELD_ID_INPUT_IW_PORT;
   filter_class.filter_fields[3].field_mode =WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_class.filter_fields[3].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

   filter_class.filter_fields[4].field_id = WP_PCE_FIELD_ID_LAST;

   eoam_system.h_pce_filter[0] =WP_PceFilterCreate (WP_WINPATH (0),
                          WP_PCE_FILTER_CLASSIFICATION, &filter_class);
   WPE_TerminateOnError (eoam_system.h_pce_filter[0], "WP_PceFilterCreate", __LINE__);

   WP_PceFilterStatisticsReset(eoam_system.h_pce_filter[0], &stats);

   //creating filter2
   filter_class.no_match_action = WP_PCE_FILTER_NO_MATCH_CONTINUE;
   filter_class.no_fields_action = WP_PCE_FILTER_NO_FIELDS_CONTINUE;
   
   filter_class.no_match_result[0].result_type = WP_PCE_RESULT_LAST;
   
   filter_class.filter_fields[0].field_id = WP_PCE_FIELD_ID_ETHER_TYPE;
   filter_class.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_class.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;
   
   filter_class.filter_fields[1].field_id = WP_PCE_FIELD_ID_LAST;
   filter_class.filter_fields[1].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
   filter_class.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;
   
   eoam_system.h_pce_filter[1] = WP_PceFilterCreate(WP_WINPATH(0), WP_PCE_FILTER_CLASSIFICATION, &filter_class);
   WPE_TerminateOnError(eoam_system.h_pce_filter[1], "WP_PceFilterCreate() ", __LINE__);

   WP_PceFilterStatisticsReset(eoam_system.h_pce_filter[1], &stats);

}

void WPE_PceFilterSetCreate (void)
{
   WP_pce_filter_set fs_level;
   WP_pce_filter_set fs_cfg;
   
   memset(&fs_level,0,sizeof(fs_level));
   memset(&fs_cfg,0,sizeof(fs_cfg));
   /* filter set*/
   fs_level.filter_set_level = 0;
   fs_level.next_filter_set = WP_UNUSED;
   fs_level.filters[0] = eoam_system.h_pce_filter[0];
   fs_level.filters[1] = WP_UNUSED;
   eoam_system.h_pce_filterset[0] = WP_PceFilterSetCreate (WP_WINPATH (0), &fs_level);
   WPE_TerminateOnError (eoam_system.h_pce_filterset[0], "WP_PceFilterSetCreate",__LINE__);

   fs_cfg.filter_set_level = 0;
   fs_cfg.next_filter_set = WP_UNUSED;
   fs_cfg.filters[0] = eoam_system.h_pce_filter[1];
   fs_cfg.filters[1] = WP_UNUSED;
   eoam_system.h_pce_filterset[1] = WP_PceFilterSetCreate(WP_WINPATH(0), &fs_cfg);
   WPE_TerminateOnError(eoam_system.h_pce_filterset[1], "WP_PceFilterSetCreate",__LINE__);

}

void WPE_PceInterfaceCreate (void)
{
   WP_pce_if_params_pkt_rx_channel pce_if_params = { 0 };
   WP_pce_if_params_iw_tx_channel interface_params2 = {0};
   
   pce_if_params.mode = WP_PCE_IW_PORT_CONNECTION_ENABLED;
   pce_if_params.parser_start_type = WP_PCE_PARSER_START_TYPE_ETHERNET;
   pce_if_params.filter_set_handle = eoam_system.h_pce_filterset[0];
   pce_if_params.ip_header_validation = WP_DISABLE;
   pce_if_params.user_programmable_fields_handle = 0;

   eoam_system.h_pce_if[0]= WP_PceInterfaceCreate (WP_PCE_IF_TYPE_PKT_RX_CHANNEL,
                             &pce_if_params);
   WPE_TerminateOnError (eoam_system.h_pce_if[0], "WP_PceInterfaceCreate()",__LINE__);

   interface_params2.mode = WP_PCE_IW_PORT_CONNECTION_ENABLED;
   interface_params2.parser_start_type = WP_PCE_PARSER_START_TYPE_ETHERNET;
   interface_params2.filter_set_handle = eoam_system.h_pce_filterset[1];
   interface_params2.ip_header_validation = WP_DISABLE;
   interface_params2.sub_port_vlan = WP_DISABLE;

   eoam_system.h_pce_if[1]=WP_PceInterfaceCreate(WP_PCE_IF_TYPE_OAM_TX,&interface_params2);
   WPE_TerminateOnError(eoam_system.h_pce_if[1], "WP_PceInterfaceCreate",__LINE__);

}

void WPE_BasicPceRulesCreate (void)
{
   WP_pce_rule_classification rule_cfg;
   memset(&rule_cfg, 0, sizeof(WP_pce_rule_classification));
   rule_cfg.enabled = WP_ENABLE;
   rule_cfg.filter_handle = eoam_system.h_pce_filter[1];
   rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_ETHER_TYPE;
   rule_cfg.rule_fields[0].value.ethertype = 0x8902;
   rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_LAST;
   rule_cfg.match_action = WP_PCE_RULE_MATCH_ACCEPT;
   rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
   rule_cfg.match_result[0].param.flow_agg.flow_aggregation = eoam_system.flow_agg[FA_PORT_ENET2];
   rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;
   eoam_system.h_pce_rule = WP_PceRuleCreate(WP_WINPATH(0), WP_PCE_RULE_CLASSIFICATION, &rule_cfg);
   WPE_TerminateOnError(eoam_system.h_pce_rule, "WP_PceRuleCreate() ", __LINE__);

}


static void WPE_BindRx()
{
   rx_binding_bri[0].pce_if_handle = eoam_system.h_pce_if[0];
   
   rx_binding_bri[0].input_port = eoam_system.bports[ENET1_BPORT];
   status = WP_IwRxBindingCreate(eoam_system.rx_enet_handle[ENET1],
                                 eoam_system.h_iw_sys_bridge[DL_IW_SYS],
                                 eoam_system.h_qnode_iwq,
                                 &rx_binding_bri[0]);
   WPE_TerminateOnError(status," WP_IwRxBindingCreate() Enet",__LINE__);

   rx_binding_bri[0].input_port = eoam_system.bports[ENET2_BPORT];
   status = WP_IwRxBindingCreate(eoam_system.rx_enet_handle[ENET2],
                                 eoam_system.h_iw_sys_bridge[UL_IW_SYS],
                                 eoam_system.h_qnode_iwq,
                                 &rx_binding_bri[0]);
   WPE_TerminateOnError(status," WP_IwRxBindingCreate() Enet",__LINE__);

   rx_binding_bri[0].input_port = eoam_system.bports[ENET3_BPORT];
   status = WP_IwRxBindingCreate(eoam_system.rx_enet_handle[ENET3],
                                 eoam_system.h_iw_sys_bridge[UL_IW_SYS],
                                 eoam_system.h_qnode_iwq,
                                 &rx_binding_bri[0]);
   WPE_TerminateOnError(status," WP_IwRxBindingCreate() Enet",__LINE__);

}

static void WPE_BindTx()
{

      status = WP_IwTxBindingCreate(eoam_system.tx_enet_handle[ENET1],
                                    WP_IW_TX_BINDING,
                                    &tx_bind_enet[0]);
      WPE_TerminateOnError(status," WP_IwTxBindngCreate()",__LINE__);

      status = WP_IwTxBindingCreate(eoam_system.tx_enet_handle[ENET2],
                                    WP_IW_TX_BINDING,
                                    &tx_bind_enet[0]);
      WPE_TerminateOnError(status," WP_IwTxBindngCreate()",__LINE__);

      status = WP_IwTxBindingCreate(eoam_system.tx_enet_handle[ENET3],
                                    WP_IW_TX_BINDING,
                                    &tx_bind_enet[0]);
      WPE_TerminateOnError(status," WP_IwTxBindngCreate()",__LINE__);	  

}


static void WPE_FilterDefinition()
{
   WPE_PceFilterCreate();

   WPE_PceFilterSetCreate();

   WPE_PceInterfaceCreate();
}


static void WPE_EthernetOamModuleInit(void)
{
   WP_status status;
   WP_module_iw_enet_oam module_cfg[1] =
      {
         {
            /* rx_oam_msgs_intmode */ WP_ENABLE,
            /* rx_oam_msgs_intqueue */WP_IRQT0,
            /* max_ma*/EOAM_DB_MAX_MA_NUM,
            /* max_remote_mep */EOAM_DB_MAX_MEP_NUM,
            /* hash_size */WP_IW_32K_HASH_ENTRIES,
            /* max_tx_message_entities */ EOAM_DB_MAC_MSG_NUM,
            /* message_queue_mode */
            (WP_IW_ENET_OAM_PM_MESSAGE_QUEUE | WP_IW_ENET_OAM_CFM_MESSAGE_QUEUE),
            /* pm_message_queue_params*/
            {
               /*message_queue_size */ MSG_Q_SIZE,
               /*  intmode */ WP_ENABLE,
               /* intrate */MSG_Q_RATE

            },
            /* cfm_message_queue_params */
            {
               /*message_queue_size */ 64,
               /*  intmode */ WP_ENABLE,
               /* intrate */MSG_Q_RATE
            },
            /* ma_id_mode */WP_ENABLE,
            /* max_lm_stats_tables */2000
         }
      };
   status = WP_ModuleInit(WP_SYSHANDLE(0),
                          WP_WDDI_MODULE_IW_ENET_OAM,
                          module_cfg);
   WPE_TerminateOnError(status, "WP_ModuleInit()",__LINE__);
}

static void WPE_PceGpeModuleInit(void)
{
 
   WP_status status;
   
   WP_sw_fdb_info sw_fdb_info = {
      WP_BUS_PARAM,                /* sw_fdb_bus */
      0,                           /* fdb_threshold */
      (256 * 1024 - 1),            /* max_fdb_rules */
   };
   
   WP_module_pce_init pce_init = {
      &pce_parser_info,            /* parser_info */
      APP_NUM_HW_PCE_RULES,        /* hardware_rules_num */
      100,                         /* filters_num */
      10000,                       /* pkt_rx_channel_interfaces_num */
      10000,                       /* iw_tx_channel_interfaces_num */
      WP_PCE_DISCARD_MODE_SMART_DISCARD,  /* pce_discard_mode */
      WP_ENABLE,                   /* bridge_loop_prevention_mode */
      WP_ENABLE,                   /* learning_mode */
      {                            /* learning_queue_config */
       WP_PCE_LEARNING_QUEUE_MAX_SIZE, /* learning_queue_size */
       WP_LEARNING_INTENABLE,      /* interrupt_mode */
       WP_IW_IRQT0,                /* int_queue_num */
       1,                          /* interrupt_rate */
       4,                          /* protection_table_size */
       }
      ,
      2,                           /* iw_next_round_pce_interfaces_num */
      0,                           /* interfaces_on_atm_rx_mode */
      0,                           /* atm_rx_channel_interfaces_num */
      0,                           /* device_interfaces_num */
      WP_NO_ENHANCMENT,            /* enhanced_mode */
      &sw_fdb_info,                /* sw_fdb_info  */
      WP_ENABLE,                   /* pce_user_programmable_fields_mode */
   };
   
   WP_module_gpe_init gpe_init = {
     2,              /* max_num_of_pecs */
     10,             /* max_num_of_pecs_global_info */
   };
   
   /* PCE module initialization */
   pce_init.parser_info = &pce_parser_info;
   status =WP_ModuleInit (WP_SYSHANDLE (0), WP_WDDI_MODULE_PCE,
                    &pce_init);
   WPE_TerminateOnError (status, "WP_ModuleInit(PCE)", __LINE__);
   
   /* GPE module initialization */
   status =WP_ModuleInit (WP_SYSHANDLE (0), WP_WDDI_MODULE_GPE,
                    &gpe_init);
   WPE_TerminateOnError (status, "WP_ModuleInit(GPE)", __LINE__);
}


/***************************************************************
 * Func name  : WPE_Reboot
 * Description: Reboot board.
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void WPE_Reboot(void)
{
#if !defined(__linux__)
/* Rebooting is specific to board and target.  The test
 * directory should not be concerned how this is done.
 * Instead, it calls a board-specific function. */
   if (WPI_REBOOT_ENABLE) 
   {
      printf("Test Rebooting winmon by WT_Reboot\nWinMon>\n");
      WPX_Reboot();
   }
   else
      printf("Not Rebooting with WT_Reboot\nWinMon>\n");
#endif
}

/***************************************************************
 * Func name  : WPE_SystemRelease
 * Description: Release all system resource.
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void WPE_SystemRelease()
{
    WPE_Eoam_SystemRelease();
    // WPE_ResourceRelease();
    WP_DriverRelease();
}

#include "wt_partition_ecc_util.c"


