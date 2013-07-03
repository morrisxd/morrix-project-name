/* COPYRIGHT-ENEA-SRC-R2 *
**************************************************************************
* Copyright (C) 2009 by Enea AB.
* All rights reserved.
*
* This Software is furnished under a software license agreement and
* may be used only in accordance with the terms of such agreement.
* Any other use or reproduction is prohibited. No title to and
* ownership of the Software is hereby transferred.
*
* PROPRIETARY NOTICE
* This Software consists of confidential information.
* Trade secret law and copyright law protect this Software.
* The above notice of copyright on this Software does not indicate
* any actual or intended publication of such Software.
**************************************************************************
* COPYRIGHT-END */
/*----------------------------------------------------------------------------
 * File Name	:	  np_ufe_fr.c
 * File Mark	 :	  none
 * Description	:	  npu ufe frame relay file
 * Others		 :	  none
 * Version       :       v1.00
 * Author		 :
 * Date 		 :
 *
 * Modify History		:
 *	   Date              Version         Author           Modification
 *-----------------------------------------------------------------------------
 2011-4-14              initial version   Richard zhou
 ----------------------------------------------------------------------------*/

#define WUFE_UFE4

#include "np_common.h"
#include "spp_wddi_server.h"
#include "wpu_util.h"
#include "wpui_hw_util.h"
#include "wti_flexmux_util.h"

#define WPI_MASK_BITS(n) ((1 << (n)) - 1)
#define WPI_INDEX_MASK WPI_MASK_BITS(18)
#define WPI_SHIFT_MASK(x,s,m) ((((WP_U32)(x)) >> (s)) & (m))
#define WPI_HANDLE_INDEX(x) WPI_SHIFT_MASK(x, 0, WPI_INDEX_MASK)

extern void terminate_on_error(WP_handle handle, WP_CHAR *s);
extern void WPU_CoreWinGineUtilGet(WPU_util_info* utilization);
extern WP_handle enet_iw_flow_defrag_aggregation;
extern WP_handle ipdefrag_agg;
void WT_Eint3Interrupt(WP_U32 wpid, WP_U32 signal_info);


WP_port_upi_emphy_ext emphy_config[1] =
{

    {
        /* tx_maxsdu */           N_FR_MAX_SDU,
        /* protocols */           WP_EMPHY_FR,
        /* trans_emphy_devices */ N_UFE_MAX_PHYS,
        /* max_ds1 */             1344
    }

};

WP_port_pkt_limits pkt_limits[1] =
{
   	{
		/*WP_U16 max_tx_channels;*/N_UFE_MAX_PHYS*2,/*both for host/iw*/
		/*WP_U16 max_rx_channels;*/N_UFE_MAX_PHYS*2,
		/*WP_U16 pq_block_size;  */0,
		/*WP_U16 n_pq_blocks;	 */0,
		/*WP_U16 emphy_devices;  */N_UFE_MAX_PHYS
    }
};

WP_port_upi_packet_ext upi_packet_config[1] =
{
    {
        /*WP_U32 packet_characteristics;*/WP_UPI_POS_CRC_ENABLE,//WP_UPI_POS_CRC_DISABLE
        /*WP_U32 slow_phy_tx_bitrate;   */ 50000000,
        /*WP_port_pkt_limits *pkt_limits;*/pkt_limits
    }
};

static WP_port_upi_iw_ext emphy_iw_config[1] =
{
    {
        1,
        0
    }
};

/* UFE UPI port configuration*/
WP_port_upi port_config_ufe[1] =
{
    {

        /* upimode */			   WP_UPI_UFE4,
        /* loopbackmode */ 	   WP_UPI_NORMAL,
        /* tx_characteristics */
        WP_UPI_MASTER |
        WP_UPI_SINGLE |
        WP_UPI_PRT_EVEN |     /*WHY ?*/
        WP_UPI_FIXED |
        WP_UPI_PRTCHK_ON |
        WP_UPI_EMPHY2_13 |
        WP_UPI_BUS_WIDTH_16,
        /* rx_characteristics */
        WP_UPI_SINGLE |
        WP_UPI_MASTER |
        WP_UPI_PRT_EVEN |
        WP_UPI_ROBIN |
        WP_UPI_PRTCHK_OFF |
        WP_UPI_BUS_WIDTH_16,
        /* atm_ext */		   0,
        /* packet_ext */	   upi_packet_config,
        /* iw_ext */		   emphy_iw_config,
        /* emphy_ext */	   emphy_config
    }

};

WP_device_emphy_fr fr_device_config[1] =
{  {
        /*tx_bit_rate; If tx unused, enter 0 */                     /* slr 155000000,*/
        /*tx_bit_rate; If tx unused, enter 0 */                     155000000,
        /*tx_slave_addr;Required only for slave mode for PHY 0 */   0,
        /*tx_statmode;*/                                            WP_PKT_STAT_ENABLE,
        /*tx_maxsdu;*/                                              N_FR_MAX_SDU,
        /*rx_bit_rate; If rx unused, enter 0 */                     155000000,
        /*rx_bit_rate; If rx unused, enter 0 */                     /* 155000000,*/
        /* rx_slave_addr; Required only for slave mode for PHY 0 */ 0,
        /*rx_intmode;*/                                             WP_PKT_INTMODE_DISABLE,
        /*rx_statmode;*/                                            WP_PKT_STAT_ENABLE,
        /*rx_opmode;*/                                              WP_FRCH_OP_NORMAL,
        /*rx_fr_header_type;*/                                      WP_FRCH_HEADER_2BYTES,
        /*rx_lookupmode;*/                                          WP_FR_HASH_LOOKUP_TABLE,
        /*rx_n_dlci;*/                                              1024 /*N_FR_MAX_RX_HT_CHANNEL*/,
        /*rx_min_dlci;*/                                            0,
        /*rx_maxsdu;*/                                              N_FR_MAX_SDU,
        /*rx_queue;*/                                               0,
        /*tx_tq_types[4]; Array of WP_packet_scheduling_type */     {0},
        /*crctype;*/                                                WP_HDLC_CRC16
    }
};
/*Port and device handles*/
WP_handle upi_ufe_port;
/*Qnodes and pool handles*/
WP_handle fr_buffer_pool;
WP_handle iw_fr_buffer_pool;

WP_handle qnode_fr_host;
WP_handle qnode_fr_iw;
/*FR host */
WP_handle fr_iw_host_port,fr_iw_host_device;

/*Channel Handles*/
WP_handle tx_fr_host_handle[N_FR_MAX_RX_HT_CHANNEL];

WUFE_handle           ufe_line_handle[N_UFE_MAX_LINES_T1]={0};

WUFE_handle           ufe_phy_handle[N_UFE_MAX_LINES_T1][32]={{0}};/*max slot in one line is 32*/

WUFE_handle           ufe_device_handle[N_UFE_MAX_LINES_T1][32]={{0}};/*max slot in one line is 32*/
u32 E1MapTable[N_UFE_MAX_LINES_T1]= {0};
/* Number of frames the application will send and receive */
#define NUM_OF_FRAMES 1
/* Number of buffers per frame */
#define NUM_OF_BUFS 1
/* Define frame byte pattern*/
WP_U8 pattern = 0x01;
WP_U32 payload_size=100;
#define RECONFIG_TRUNK_MAX (SPP_MAX_TRUNK_NUM*2)
WP_U8 reconfigStm1_createE1_flag[RECONFIG_TRUNK_MAX] = {0};

static void WTUFE_config_system( WP_U32 ufe_lb_mode,WP_U32 ufe_port);
static void WTUFE_config_lines(WP_U32 line_lb_mode);
static void WTUFE_config_tmx336(WP_U32 ufe_port);
void WTUFE_config_phys(WP_U32 line_index,WP_U32 groupId,WP_U32 slot_selection,WP_U32 line_index_base/*,WP_handle *phy_handle,WP_handle *device_handle*/);

WP_CHAR input[20];
#define WT_UFE_HANDLE_INVALID 0xffffffff;
STRU_STM1_ALARM_INFO np_ufe4_ararm_info={0};

static void WT_UfeTerminateOnError(WP_handle handle, WP_CHAR *s,WP_U32 id,WP_U32 LineNum)
{
    if(handle != WUFE_OK)
    {
        Fprintf("Test Abort %s %d %s 0x%x Line:%d\n", s,id,
                WUFE_error_name[(handle&0x03ffffff)], handle, LineNum);

        exit(1);

    }
#if DBG_MODE > 0
    else
    {
        Fprintf("Test info %s %d 0x%x Line:%d\n", s,id, handle, LineNum);
    }
#endif
}


/*check if support FR system*/
void np_initial_fr_qnode(WP_U32 ufe_port)
{
    WP_handle 		status;
    WP_pool_buffer_data buffer_pool_config;

    buffer_pool_config.n_buffers = 5000;
    buffer_pool_config.offset = 0;
    buffer_pool_config.size = N_FR_MAX_SDU;
    buffer_pool_config.pad = 0;
    buffer_pool_config.bus = WP_BUS_PACKET;//WP_BUS_HOST;
    buffer_pool_config.bank = APP_BANK_PACKET;//APP_BANK_HOST;

    fr_buffer_pool = WP_PoolCreate(WP_WINPATH(0),
                                   WP_pool_frbuffer,
                                   &buffer_pool_config);
    terminate_on_error(fr_buffer_pool, "WP_PoolCreate() - WP_pool_frbuffer");


    WP_qnode_fr_hostq qn_fr_host;
    qn_fr_host.interruptqueue = WP_IRQT1;
    qn_fr_host.adjunct_pool = fr_buffer_pool;
    qn_fr_host.num_buffers = 20000;
    qnode_fr_host = WP_QNodeCreate(WP_WINPATH(0),
                                   WP_QNODE_FR_HOSTQ,
                                   &qn_fr_host);


    /*add iwbuffer pool for frame relay*/
	//buffer_pool_config.n_buffers = 5000;
	buffer_pool_config.n_buffers = 10000;
	buffer_pool_config.offset = 0;
	buffer_pool_config.size = N_FR_MAX_SDU;
	buffer_pool_config.pad = 0;
	buffer_pool_config.bus = WP_BUS_PACKET,//WP_BUS_HOST;
        buffer_pool_config.bank = APP_BANK_PACKET;

	iw_fr_buffer_pool = WP_PoolCreate(WP_WINPATH(0),
                                      WP_pool_iwbuffer,
                                      &buffer_pool_config);
    terminate_on_error(iw_fr_buffer_pool, "WP_PoolCreate() - WP_pool_iwbuffer");



    WP_qnode_iwq qnode_iw_config;
	qnode_iw_config.interruptqueue = WP_IRQT1;
	qnode_iw_config.num_buffers = 32000;
	qnode_iw_config.adjunct_pool = iw_fr_buffer_pool;


	qnode_fr_iw = WP_QNodeCreate(WP_WINPATH(0),
                                 WP_QNODE_IWQ,
                                 &qnode_iw_config);

    terminate_on_error(qnode_fr_iw, "WP_QNodeCreate() - qnode_iw_config");

	/* Create virtual host ports and devices. */
	fr_iw_host_port =  WP_PortCreate(WP_WINPATH(0),WP_PORT_IW_HOST,NULL);
	terminate_on_error(fr_iw_host_port,"Host Port Create");

#if 0
	fr_device_config[0].rx_queue = qnode_fr_host;
	fr_device_config[0].tx_tq_types[0] = WP_PKT_SCHED_WRR;
	fr_device_config[0].tx_tq_types[1] = WP_PKT_SCHED_WRR;
	fr_device_config[0].tx_tq_types[2] = WP_PKT_SCHED_WRR;
	fr_device_config[0].tx_tq_types[3] = WP_PKT_SCHED_WRR;


	fr_iw_host_device = WP_DeviceCreate(fr_iw_host_port,0,
                                        WP_DEVICE_FR,
                                        &fr_device_config[0]);

#endif
#if 1
	fr_iw_host_device = WP_DeviceCreate(fr_iw_host_port, 0, WP_DEVICE_IW_HOST, NULL);
	terminate_on_error(fr_iw_host_device, "WP_DeviceCreate() IW Host");
#endif

	if (ufe_port == WTUFE_UPI_PORT_1)
	{
        /*Create UFE port on UPI1*/
        upi_ufe_port = WP_PortCreate(WP_WINPATH(0), WP_PORT_UPI1, &port_config_ufe[0]);
        terminate_on_error(upi_ufe_port, "WP_PortCreate() UFE on UPI1");
	}
	else if (ufe_port == WTUFE_UPI_PORT_2)
	{
        /*Configure BRG for UPI2*/
        status = WP_SysClockCreate(WP_WINPATH(0), WP_BRG1, WP_BRG_SRC_BRGIN2, 2);
        terminate_on_error(status, "WP_SysClockCreate() UPI2");
        /*Create UFE port on UPI2*/
        upi_ufe_port = WP_PortCreate(WP_WINPATH(0), WP_PORT_UPI2, &port_config_ufe[0]);
        terminate_on_error(upi_ufe_port, "WP_PortCreate() UFE on UPI2");
	}
	else
	{
        Fprintf("UPI Port Not Valid, UPI1 configured by default\n");
	}
	Fprintf("np np_initial_fr_qnode completed \n");
}

void fr_system_init(WP_U32 ufe_port)
{

   memset(reconfigStm1_createE1_flag, 0, RECONFIG_TRUNK_MAX);
    /*Initialise and Configure the UFE System*/
#if UFE_LOOPBACK
    WTUFE_config_system(UFE_LB_SYSTEM,ufe_port);
    Fprintf("UFE System Configured\n");
    WTUFE_config_lines(UFE_LB_SYSTEM);
#else
    WTUFE_config_system(UFE_NO_LOOPBK,ufe_port);
    Fprintf("UFE System Configured\n");
    WTUFE_config_lines(UFE_NO_LOOPBK);
#endif
    Fprintf("UFE Lines Created\n");
    WP_status status;
    WUFE_events             ufe_events;
    memset(&ufe_events, 0, sizeof(WUFE_events));

    ufe_events.emphy_events[0].event_reg 		   = WUFE_EVENT_EMPHY;
    ufe_events.emphy_events[0].phy_not_valid_handle = 0;

    ufe_events.tdm_events.event_reg				   = WUFE_EVENT_TDM;

    ufe_events.fm_events.event_reg				   = WUFE_EVENT_FM&((~WUFE_EVENT_FM_TX_DDR_UNDERRUN)&(~WUFE_EVENT_FM_TX_ECC_ONE_BIT)&(~WUFE_EVENT_FM_RX_ECC_ONE_BIT));

    ufe_events.machine_events.rx_event_reg		   = WUFE_EVENT_MACHINE_RX;
    ufe_events.machine_events.tx_event_reg		   = WUFE_EVENT_MACHINE_TX;

    ufe_events.bs_events.pll_lost_lock_event_reg    = 0x06ff;
    ufe_events.bs_events.absent_event_reg		   = 0x000f;
    ufe_events.bs_events.los_event_reg			   = 0x000f;
    status = WUFE_UfeEventEnable(0, &ufe_events);
    terminate_on_error(status, "WUFE_UfeEventEnable");
}

WP_U32 np_fraddr_pack(WP_U32 card, WP_U32 trunk, WP_U32 tsgroup, WP_U32 dlci)
{
	fr_addr_t a = {0};
	assert(sizeof(fr_addr_t) == 4);
	a.card = card-1;
	a.trunk = trunk-1;
	a.tsgroup = tsgroup;
	a.dlci = dlci;
	return *(WP_U32*)&a;
}
void np_fraddr_unpack(WP_U32 frport, WP_U32* card, WP_U32* trunk, WP_U32* tsgroup, WP_U32* dlci)
{
	fr_addr_t* a;
	assert(sizeof(fr_addr_t) == 4);
	a = (fr_addr_t*)&frport;
	*card = (WP_U32)(a->card) + 1;
	*trunk = (WP_U32)(a->trunk) + 1;
	*tsgroup = a->tsgroup;
	*dlci = a->dlci;
}

int np_creat_fr_vport(WP_U32 vfport)
{
	STRU_VPORT_INFO  *VportInfo;
	VportInfo = npu_get_vport_info(vfport);
    STRU_SET_INITAIL_CONFIG *initial_cfg = npu_get_initial_cfg_info();
	if(VportInfo != NULL)/*don't have to judge if it is used*/
	{
        WP_U32 trunk;
        WP_U32 group;
        WP_U32 stm1Id;

        stm1Id = VportInfo->ch_info.card - 1;
        if(initial_cfg->stru_stm1_cfg_info[stm1Id].E1T1type == NPU_MODE_E1)
        {
            trunk = VportInfo->ch_info.trunk - 1 + stm1Id*N_UFE_MAX_LINES_E1/2;
        }
        else/*T1*/
        {
            trunk = VportInfo->ch_info.trunk - 1 + stm1Id*N_UFE_MAX_LINES_T1/2;
        }

        group = VportInfo->ch_info.group;

        /*Configure UFE Phys and EMPHY devices*/
        if(ufe_device_handle[trunk][group] != 0)
        {
            VportInfo->ch_info.phy_handle = ufe_phy_handle[trunk][group];
            VportInfo->ch_info.device_handle= ufe_device_handle[trunk][group];
        }
        else
        {
            WDDI_WARNING("Error: np_creat_fr_vport::ufe_device_handle[%d][%d] is not create yet! \n",trunk,group);
            return -1;
        }
        WDDI_INFO("Info: np_creat_fr_vport, stm1Id:%d, trunk:%d, group:%d\n", stm1Id, trunk, group);
        /*if fr host channel not created,then creat it*/
        if((VportInfo->ch_info.rx_host_handle == 0)&&(VportInfo->ch_info.tx_host_handle == 0))
        {
            np_creat_fr_host_channel(&VportInfo->ch_info);
        }
        else
        {
            if(UNUSED == VportInfo->UsedFlag)
            {
                np_enable_fr_host_channel(&VportInfo->ch_info);
                WDDI_WARNING("Warn: np_creat_fr_vport::np_enable_fr_host_channel, vport:%d\n",vfport);
            }
        }

        /*creat fr iw channel for FR switch system*/
        if(VportInfo->ch_info.rx_iw_handle == 0)
        {
            np_creat_fr_iw_channel(&VportInfo->ch_info);
        }
        else
        {
            if(UNUSED == VportInfo->UsedFlag)
            {

                np_enable_fr_iw_channel(&VportInfo->ch_info);
            }
        }
        if(VportInfo->ch_info.agg_enet2_to_fr == 0)
        {
            np_dl_enet2_to_fr_rule_add(&VportInfo->ch_info);
        }
	}
	else
	{
        WDDI_WARNING("Warn: np_creat_fr_vport::npu_get_initial_cfg_info fail!\n");
        return -1;
	}
    return 0;
}
int np_close_fr_vport(WP_U32 vfport)
{
	STRU_VPORT_INFO  *VportInfo;
	VportInfo = npu_get_vport_info(vfport);
	int i;
	if(VportInfo != NULL)
	{
        if(VportInfo->ch_info.device_handle == 0)
        {
            WDDI_WARNING("Warn: np_close_fr_vport, ufe_device_handle hasn't created ,vport = %d \n",vfport);
            return -1;
        }
        /*get the created phy and device handle*/
        else
        {   /*maybe we should disable/delete iw_channel first,because it will use
              ch_info->agg_fr_rx_host_flow when binding
              WP_ERR_IW_FLOW_UNINITIALIZED error when call host_receive() may caused by this reason*/
            WDDI_INFO("Info: np_close_fr_vport, vfport:0x%x\n", vfport);
            if((VportInfo->ch_info.rx_iw_handle != 0) && (VportInfo->ch_info.tx_iw_handle != 0))
            {
                np_disable_fr_iw_channel(&VportInfo->ch_info);

                //np_delete_fr_iw_channel(&VportInfo->ch_info);
            }

            if((VportInfo->ch_info.rx_host_handle != 0) && (VportInfo->ch_info.tx_host_handle != 0))
            {
                np_disable_fr_host_channel(&VportInfo->ch_info);

                //np_delete_fr_host_channel(&VportInfo->ch_info);

            }
        }
#if 0
        WP_U32 stm1Id;
        WP_U32 trunk;
        WP_U32 group;
        WP_status status;
        STRU_SET_INITAIL_CONFIG *initial_cfg = npu_get_initial_cfg_info();
        stm1Id = VportInfo->ch_info.card - 1;
        if(initial_cfg->stru_stm1_cfg_info[stm1Id].E1T1type == NPU_MODE_E1)
        {
            trunk = VportInfo->ch_info.trunk - 1 + stm1Id*N_UFE_MAX_LINES_E1/2;
        }
        else/*T1*/
        {
            trunk = VportInfo->ch_info.trunk - 1 + stm1Id*N_UFE_MAX_LINES_T1/2;
        }

        group = VportInfo->ch_info.group;
        status = np_Channel_is_None(stm1Id, trunk, group);
        if(WP_OK == status)
        {
            np_DeviceDelete(trunk, group);
            status = np_Ufe_Phy_Delete(trunk,group);
            if(WP_OK != status)
            {
                Fprintf("np_Ufe_Phy_Delete failed! status = %d\n",status);
                return status;
            }
            initial_cfg->stru_stm1_cfg_info[stm1Id].trunk_info[trunk].group_info[group].groupValid = NPU_INVALID;
        }
#endif
	}
	else
	{
        WDDI_WARNING("Warn: np_close_fr_vport::npu_get_vport_info fail! vfport:0x%x\n", vfport);
		return -1;
	}
	return 0;
}

static WP_data_unit data_unit= {0};
static WP_data_segment tx_buf;
static WP_U8 *data_ptr;

void npu_send_fr_packet(WP_U8 *p, WP_U32 len,WP_U32 vfport)
{
    FR_CHANNEL_INFO * ch_info;
    STRU_VPORT_INFO  *VportInfo = npu_get_vport_info(vfport);
	if(VportInfo != NULL)
    {
		ch_info = &VportInfo->ch_info;
		np_fr_host_send(ch_info->tx_host_handle,len,(WP_CHAR *)p,ch_info->dlci,vfport);

    }
    else
    {
        WDDI_WARNING("Warn: npu_send_fr_packet::npu_get_vport_info fail! vport = %d \n ",vfport);
    }
}

void np_fr_host_send(WP_handle tx_host_handle,WP_S32 DataLength, WP_CHAR* pTxData,WP_U32 dlci,WP_U32 port)
{
    WP_status status;
    WP_data_segment* curr_buf;
    data_unit.data_size = DataLength + 2;/*frame relay header 2 byte*/
    data_unit.n_active = 1;

    tx_buf.data_size = data_unit.data_size;
    data_ptr = WP_PoolAlloc(fr_buffer_pool);
    tx_buf.data = data_ptr;
    tx_buf.pool_handle = fr_buffer_pool;
    tx_buf.displacement = 0;
    tx_buf.user_info = 0;
    tx_buf.next = NULL;
    tx_buf.buffer_class = 0;

    /*data_unit.control*/
#if 0
    data_unit.control =
        WP_HT_IW_B_MODE_FIELD(WP_HT_IW_B_FAST_MODE)|
        WP_HT_IW_B_VA_DEST_TYPE_FIELD(WP_HT_IW_B_VA_DESTINATION_FLOW_AGG)|
        WP_HT_IW_FSP_FLOW_AGG_FIELD(agg_enet_1) |
        WP_HT_IW_CRC_FIELD(0);
#endif
    data_unit.segment = (&tx_buf);
    data_unit.n_segments = 1;
    data_unit.n_active = 1;
    data_unit.type = WP_DATA_FRAME_RELAY;
    data_unit.data_return = 0;
    data_unit.status = 0;

    //WP_MEM_BYTES_FILL(data_ptr, 0, tx_buf.data_size);
    WP_FR_HEADER_2OCT_SET(data_ptr,dlci,0,0,0,0);
#if NPU_HOST_OLP
    pcimem_copy(data_ptr+2, pTxData, DataLength);
#else
    WP_MEM_BYTES_SET(data_ptr+2, pTxData, DataLength);
#endif
    //WP_MEM_BYTES_SET (data_ptr ,hexa_buffer,DATA_LENGTH);

    WDDI_INFO("Info: np_fr_host_send, FR-packet, dlci:%d, vport:0x%0x\n",dlci, port);
    curr_buf = data_unit.segment;

    WDDI_STRING(curr_buf->data, curr_buf->data_size);
    npu_mon_capture_packet((char *)data_ptr,data_unit.data_size,port,1);
    status = WP_HostSend(tx_host_handle, &data_unit);
    //terminate_on_error(status, "FR WP_HostSend()");

    if(WP_ERROR_P(status))
    {
        if (WP_ERROR(status) == WP_ERR_WMM_FIFO_PUT)
        {
            WDDI_WARNING("np_fr_host_send fail,status=%d,tx_host_handle=0x%x\n",status,tx_host_handle);

        }
        else
        {
            WDDI_WARNING("np_fr_host_send fail,WP_PoolFree,status=%d,tx_host_handle=0x%x\n",status,tx_host_handle);
            WP_PoolFree(curr_buf->pool_handle, curr_buf->data);
        }
    }
}


int np_fr_host_receive(WP_handle rx_host_handle,WP_S32 *DataLength, WP_CHAR* pTxData)
{
    WP_status status;
    WP_data_unit du = {0};
    WP_data_segment seg;
    WP_data_segment* curr_buf;
    WP_U32 j;
    du.segment = &seg;
    du.type = WP_DATA_IW;
    du.n_segments = 1;

    if (WP_OK != (status = WP_HostReceive(rx_host_handle, &du)))
    {
        if ( strcmp("WP_ERR_HST_NO_DATA_TO_GET",  WP_error_name[WP_ERROR(status)]) != 0)
            //terminate_on_error(status,"WP_HostReceive error");
            return -1;
    }
    else {
        curr_buf = du.segment;
        for (j = 0; j < du.n_active; j++, curr_buf++)
        {
#if 0
            for (k = 0; k < curr_buf->data_size; k++)
            {
                pTxData[k]=curr_buf->data[k];
            }
#endif
#if NPU_HOST_OLP
            pcimem_copy(pTxData,curr_buf->data,curr_buf->data_size);
#else
            memcpy(pTxData,curr_buf->data,curr_buf->data_size);/**/
#endif

            WDDI_STRING(pTxData, (curr_buf->data_size));
            *DataLength = curr_buf->data_size;
            if(*DataLength >N_FR_MAX_SDU)
            {
                WDDI_WARNING("np_fr_host_receive a big packet\n");
                *DataLength = N_FR_MAX_SDU;
            }
            //DEBUG("(%d bytes) \n", curr_buf->data_size);
            WP_PoolFree(curr_buf->pool_handle, curr_buf->data);
        }
        //*DataLength = u16DataIndex;
    }
    return status;
}


void np_disable_fr_host_channel(FR_CHANNEL_INFO * ch_info)
{
	WP_handle	   status;
	u32 card;
	u32 trunk;
	u32 group;
	u32 dlci;
	np_fraddr_unpack(ch_info->vfport,&card,&trunk,&group,&dlci);

	/*Disable FR host rx and tx channels*/
	WP_U32 i = 0;
	WP_boolean ch_disabled_state = WP_FALSE;
#if 0
    if(ch_info->rx_host_handle == 0) return 0;
   	(void)WP_ChannelStatus(ch_info->rx_host_handle, WP_CH_STATUS_DISABLED, &ch_disabled_state);
    if(WP_FALSE == ch_disabled_state)
	{
		status = WP_ChannelDisable(ch_info->rx_host_handle);
		if ( strcmp("WP_ERR_CH_ALREADY_DISABLED",  WP_error_name[WP_ERROR(status)]) != 0)
		{
			terminate_on_error(status, "WP_ChannelDisable() rx_fr_host_handle");
		}
		do
	    {
	    	(void)WP_ChannelStatus(ch_info->rx_host_handle, WP_CH_STATUS_DISABLED, &ch_disabled_state);
			WP_Delay(1000);
			i ++;
			if(100 == i)
			{
	            break;
			}
		}while(WP_FALSE == ch_disabled_state);
	}
#endif
	ch_disabled_state = WP_FALSE;
	if(ch_info->tx_host_handle == 0)
	{
        WDDI_INFO("Warn: np_disable_fr_host_channel, ch_info->tx_host_handle:0\n");
        return 0;
	}

   	(void)WP_ChannelStatus(ch_info->tx_host_handle, WP_CH_STATUS_DISABLED, &ch_disabled_state);
    if(WP_FALSE == ch_disabled_state)
    {
        WDDI_INFO("np_disable_fr_host_channel vfort = 0x%x (%d): S%dP%dFR%dD%d \n",ch_info->vfport,htonl(ch_info->vfport),card,trunk,group,dlci);
	    i = 0;
	    do
	    {
	    	status = WP_ChannelDisable(ch_info->tx_host_handle);
			if ( strcmp("WP_ERR_CH_ALREADY_DISABLED",  WP_error_name[WP_ERROR(status)]) != 0)
			{
				terminate_on_error(status, "WP_ChannelDisable() tx_fr_host_handle");
			}

	    	(void)WP_ChannelStatus(ch_info->tx_host_handle, WP_CH_STATUS_DISABLED, &ch_disabled_state);
			WP_Delay(100);
			i ++;
			if(1000 == i)
			{
	            break;
			}
		}while(WP_FALSE == ch_disabled_state);
	}
}
void np_delete_fr_host_channel(FR_CHANNEL_INFO * ch_info)
{
	WP_handle	   status;
	if(ch_info->rx_host_handle == 0)return;
	/*Delete FR host rx and tx channels*/
	status = WP_ChannelDelete(ch_info->rx_host_handle);
	//if ( strcmp("WP_ERR_CH_ALREADY_DISABLED",  WP_error_name[WP_ERROR(status)]) != 0)
	{
		terminate_on_error(status, "WP_ChannelDelete() rx_fr_host_handle");
		ch_info->rx_host_handle = 0;
	}
	if(ch_info->tx_host_handle == 0)return;
	status = WP_ChannelDelete(ch_info->tx_host_handle);
	//if ( strcmp("WP_ERR_CH_ALREADY_DISABLED",  WP_error_name[WP_ERROR(status)]) != 0)
	{
		terminate_on_error(status, "WP_ChannelDelete() tx_fr_host_handle");
		ch_info->tx_host_handle = 0;
	}
}

void np_enable_fr_host_channel(FR_CHANNEL_INFO * ch_info)
{
    WP_handle	   status;
	u32 card;
	u32 trunk;
	u32 group;
	u32 dlci;
	np_fraddr_unpack(ch_info->vfport,&card,&trunk,&group,&dlci);

	WDDI_INFO("np_enable_fr_host_channel vfort = 0x%x (%d): S%dP%dFR%dD%d \n",ch_info->vfport,htonl(ch_info->vfport),card,trunk,group,dlci);

	WP_boolean ch_disabled_state = WP_FALSE;
	if(ch_info->rx_host_handle == 0)return;
   	(void)WP_ChannelStatus(ch_info->rx_host_handle, WP_CH_STATUS_DISABLED, &ch_disabled_state);
	if(WP_TRUE== ch_disabled_state)
	{
		/*Enable FR host rx and tx channels*/
		status = WP_ChannelEnable(ch_info->rx_host_handle);
		if ( strcmp("WP_ERR_CH_ALREADY_ENABLED",  WP_error_name[WP_ERROR(status)]) != 0)
		{
			terminate_on_error(status, "WP_ChannelEnable() rx_fr_host_handle");
		}
	}
	if(ch_info->tx_host_handle == 0)return;
   	(void)WP_ChannelStatus(ch_info->tx_host_handle, WP_CH_STATUS_DISABLED, &ch_disabled_state);
	if(WP_TRUE== ch_disabled_state)
	{
		status = WP_ChannelEnable(ch_info->tx_host_handle);
		if ( strcmp("WP_ERR_CH_ALREADY_ENABLED",  WP_error_name[WP_ERROR(status)]) != 0)
		{
			terminate_on_error(status, "WP_ChannelEnable() tx_fr_host_handle");
		}
	}

}

void np_disable_fr_iw_channel(FR_CHANNEL_INFO * ch_info)
{
	WP_handle	   status;
    WP_tx_binding tx_binding;
    WP_rx_binding_fr_switching rx_binding_config;
    NP_SYSTEM_INFO * np_system;
	WP_U32 i = 0;
	u32 card;
	u32 trunk;
	u32 group;
	u32 dlci;
	np_fraddr_unpack(ch_info->vfport,&card,&trunk,&group,&dlci);

	WP_boolean ch_disabled_state = WP_FALSE;

	np_system = np_get_system_info();
    if(ch_info->rx_iw_handle == 0)
    {
    	WDDI_INFO("Warn: np_disable_fr_iw_channel::np_get_system_info fail!\n");
        return;
    }
	/*Disable FR host rx and tx channels*/
   	(void)WP_ChannelStatus(ch_info->rx_iw_handle, WP_CH_STATUS_DISABLED, &ch_disabled_state);
    if(WP_FALSE == ch_disabled_state)
    {
    	WDDI_INFO("Info: np_disable_fr_iw_channel, vfort:0x%x (%d): S%dP%dFR%dD%d\n",ch_info->vfport,htonl(ch_info->vfport),card,trunk,group,dlci);
		memset(&rx_binding_config, 0, sizeof(WP_rx_binding_fr_switching));
		rx_binding_config.default_aggregation = ch_info->agg_fr_rx_host_flow;
	    status = WP_IwRxBindingModify(ch_info->rx_iw_handle,
									  np_system->system_fr_handle,
									  qnode_fr_iw,
	                                  WP_IW_RX_BIND_MOD_DISABLE,
	                                  &rx_binding_config);
	    terminate_on_error(status, "WP_IwRxBindingModify disable");
		WP_Delay(50);
#if 0
		status = WP_ChannelDisable(ch_info->rx_iw_handle);
		if ( strcmp("WP_ERR_CH_ALREADY_DISABLED",  WP_error_name[WP_ERROR(status)]) != 0)
		{
			terminate_on_error(status, "WP_ChannelDisable() rx_iw_handle");
		}
#endif
		do
	    {
	    	status = WP_ChannelDisable(ch_info->rx_iw_handle);
			if ( strcmp("WP_ERR_CH_ALREADY_DISABLED",  WP_error_name[WP_ERROR(status)]) != 0)
			{
				terminate_on_error(status, "WP_ChannelDisable() rx_iw_handle");
			}

	    	(void)WP_ChannelStatus(ch_info->rx_iw_handle, WP_CH_STATUS_DISABLED, &ch_disabled_state);
			WP_Delay(50);
			i ++;
			if(1000 == i)
			{
	            break;
			}
		}while(WP_FALSE == ch_disabled_state);
	}
#if 0
	ch_disabled_state = WP_FALSE;
	if(ch_info->tx_iw_handle == 0)return;
	(void)WP_ChannelStatus(ch_info->tx_iw_handle, WP_CH_STATUS_DISABLED, &ch_disabled_state);
    if(WP_FALSE == ch_disabled_state)
    {
	    memset(&tx_binding, 0, sizeof(WP_tx_binding));
		tx_binding.maxt = 0;
	    tx_binding.dci_mode = WP_IW_DYN_CH_INSERT_DISABLE;
	    status = WP_IwTxBindingModify(ch_info->tx_iw_handle,
                                      WP_IW_TX_BIND_MOD_MAXT | WP_IW_TX_BIND_MOD_DCI,
                                      WP_IW_TX_BINDING,
                                      &tx_binding);
	    terminate_on_error(status, "WP_IwTxBindingModify disable");
	    WP_Delay(50);

		i = 0;
		do
	    {
	    	status = WP_ChannelDisable(ch_info->tx_iw_handle);
			if ( strcmp("WP_ERR_CH_ALREADY_DISABLED",  WP_error_name[WP_ERROR(status)]) != 0)
			{
				terminate_on_error(status, "WP_ChannelDisable() tx_iw_handle");
			}

	    	(void)WP_ChannelStatus(ch_info->tx_iw_handle, WP_CH_STATUS_DISABLED, &ch_disabled_state);
			WP_Delay(50);
			i ++;
			if(1000 == i)
			{
	            break;
			}
		}while(WP_FALSE == ch_disabled_state);
	}

    //WP_Delay(50);
#endif
}

void np_delete_fr_iw_channel(FR_CHANNEL_INFO * ch_info)
{
	WP_handle	   status;
	if(ch_info->rx_iw_handle == 0)return;
	/*Delete FR host rx and tx channels*/
	status = WP_ChannelDelete(ch_info->rx_iw_handle);
	//if ( strcmp("WP_ERR_CH_ALREADY_DISABLED",  WP_error_name[WP_ERROR(status)]) != 0)
	{
		terminate_on_error(status, "WP_ChannelDelete() rx_iw_handle");
		ch_info->rx_iw_handle = 0;
	}
	if(ch_info->tx_iw_handle == 0)return;
	status = WP_ChannelDelete(ch_info->tx_iw_handle);
	//if ( strcmp("WP_ERR_CH_ALREADY_DISABLED",  WP_error_name[WP_ERROR(status)]) != 0)
	{
		terminate_on_error(status, "WP_ChannelDelete() tx_iw_handle");
		ch_info->tx_iw_handle = 0;
	}
}

void np_enable_fr_iw_channel(FR_CHANNEL_INFO * ch_info)
{
    WP_handle	   status;
    WP_tx_binding tx_binding;
	u32 card;
	u32 trunk;
	u32 group;
	u32 dlci;
	np_fraddr_unpack(ch_info->vfport,&card,&trunk,&group,&dlci);

	WDDI_INFO("np_enable_fr_iw_channel vfort = 0x%x (%d): S%dP%dFR%dD%d \n",ch_info->vfport,htonl(ch_info->vfport),card,trunk,group,dlci);

	WP_boolean ch_disabled_state = WP_FALSE;
    WP_rx_binding_fr_switching rx_binding_config;
    NP_SYSTEM_INFO * np_system;
	np_system = np_get_system_info();
	if(ch_info->rx_iw_handle == 0)return;
   	(void)WP_ChannelStatus(ch_info->rx_iw_handle, WP_CH_STATUS_DISABLED, &ch_disabled_state);
	if(WP_TRUE== ch_disabled_state)
	{
		/*Enable FR host rx and tx channels*/
		memset(&rx_binding_config, 0, sizeof(WP_rx_binding_fr_switching));
		rx_binding_config.default_aggregation = ch_info->agg_fr_rx_host_flow;
		rx_binding_config.mru = N_FR_MAX_SDU ;
		status = WP_IwRxBindingModify(ch_info->rx_iw_handle,
									  np_system->system_fr_handle,
									  qnode_fr_iw,
									  WP_IW_RX_BIND_MOD_MRU|WP_IW_RX_BIND_MOD_DEFAULT_AGG,
									  &rx_binding_config);
		terminate_on_error(status, "WP_IwRxBindingModify disable");
		WP_Delay(50);

		status = WP_ChannelEnable(ch_info->rx_iw_handle);
		if ( strcmp("WP_ERR_CH_ALREADY_ENABLED",  WP_error_name[WP_ERROR(status)]) != 0)
		{
			terminate_on_error(status, "WP_ChannelEnable() np_enable_fr_iw_rx channel");
		}

	}

	if(ch_info->tx_iw_handle == 0)return;
   	(void)WP_ChannelStatus(ch_info->tx_iw_handle, WP_CH_STATUS_DISABLED, &ch_disabled_state);
	if(WP_TRUE== ch_disabled_state)
	{
		tx_binding.maxt = NPU_FR_TX_MAXT;
		tx_binding.dci_mode = WP_IW_DYN_CH_INSERT_ENABLE;
		status = WP_IwTxBindingModify(ch_info->tx_iw_handle,
                                      WP_IW_TX_BIND_MOD_MAXT | WP_IW_TX_BIND_MOD_DCI,
                                      WP_IW_TX_BINDING,
                                      &tx_binding);

		status = WP_ChannelEnable(ch_info->tx_iw_handle);
		if ( strcmp("WP_ERR_CH_ALREADY_ENABLED",  WP_error_name[WP_ERROR(status)]) != 0)
		{
			terminate_on_error(status, "WP_ChannelEnable() np_enable_fr_iw_tx channel");
		}
	}

}

WP_iw_agg_directmap tx2brg_agg_cfg[1] =
{
    {
        /* tag */                  0,
        /* txfunc */               0,
        /* rfcs */                 WP_IW_RFCS_DISABLE,
        /* l2header_insert_mode;*/ WP_IW_L2H_INSERT_ENABLE,//WP_IW_L2H_INSERT_ENABLE
        /* interruptqueue;*/       WP_IW_IRQT1,
        /* error_pkt_mode*/        WP_IW_ERRPKT_DISCARD,
        /* intmode;*/              WP_IW_INT_DISABLE,
#if STATISTICS_ENABLE
        /* statmode;*/             WP_IW_STAT_ENABLE,
#else
        /* statmode;*/             WP_IW_STAT_DISABLE,
#endif
        /* timestamp_mode;*/       WP_IW_TIME_STAMP_DISABLE,
        /* ov_pool_mode*/          0,
        /* fbp_drop_threshold;*/   0,
        /* mtu;*/                  1536,
        /* prefix_length */        46, /* MAC header with VLAN */
        /* extraction_length */    2,
        /* prefix_header[68];*/    {	0x00, 0x22, 0x33, 0x44, 0x55, 0x66, 0x00, 0x00,
                                        0x00, 0x00, 0x00, 0x04, 0x81, 0x00, 0x00, 0x00, 0x08, 0x00,
                                        0x45, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
                                        0x40, 0x11, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x02,
                                        0x00, 0x00, 0x00, 0x01},
        /* policer_enable */       0,
        /* policer_config */       NULL,
        /* cong_mode */            0,
        /* cong_threshold_param */ NULL,
        /* dynamic_length_update*/ WP_IW_LENGTH_UPDATE_L3,//WP_IW_LENGTH_UPDATE_L3,//WP_IW_LENGTH_UPDATE_DISABLED,
        /* length_offset*/         18
    }
};

extern WP_handle iw_sys_bridging;
extern WP_handle iw_enet_1;
extern WP_handle default_host_aggregation;
extern WP_handle agg_enet_1,iw_rx_host_handle;
extern WP_iw_agg_directmap bridge_iw_agg_cfg[];

void np_creat_fr_iw_channel(FR_CHANNEL_INFO * ch_info)
{
	WP_U32 channeltag =  ch_info->vfport;
    NP_SYSTEM_INFO * np_system;
	np_system = np_get_system_info();

	WP_handle	   status;
	/*Create frame relay iw channels*/
	WP_ch_fr fr_ch_config;
	memset((void*)&fr_ch_config,0,sizeof(fr_ch_config));
	fr_ch_config.intmode = WP_PKTCH_INT_DISABLE;
	fr_ch_config.statmode = WP_PKTCH_STAT_ENABLE;
	fr_ch_config.iwmode = WP_PKTCH_IWM_ENABLE;
	fr_ch_config.rx_queue_threshold = 200;
	fr_ch_config.tx_opmode = WP_FRCH_OP_NORMAL;
	fr_ch_config.tx_fr_header_type = WP_FRCH_HEADER_2BYTES;
	fr_ch_config.tx_fragment_size = 0;
	fr_ch_config.tx_pqblock = 0;
	fr_ch_config.tx_pqlevel = 0;
	STRU_SET_INITAIL_CONFIG *initial_cfg = npu_get_initial_cfg_info();
	/*QOS enable*/
	//if(!initial_cfg->stru_stm1_cfg_info[ch_info->card-1].trunk_info[ch_info->trunk-1].group_info[ch_info->group].PVCQos[ch_info->dlci].adminState)
#if START_SCHEDULER
	{
        WP_pkt_shaping_cir_eir fr_shaping_param;
		fr_shaping_param.cir = initial_cfg->stru_stm1_cfg_info[ch_info->card-1].trunk_info[ch_info->trunk-1].group_info[ch_info->group].PVCQos[ch_info->dlci].cir;
		if(WT_MIN_CH_RATE > fr_shaping_param.cir)
		{
            //Fprintf("np_creat_fr_iw_channel fr_shaping_param.cir rate %d is low than %d set is to ",fr_shaping_param.cir,WT_MIN_CH_RATE);
			fr_shaping_param.cir = WT_MIN_CH_RATE ;
			//Fprintf("%d\n",fr_shaping_param.cir);
		}
		if(WT_MAX_CH_RATE < fr_shaping_param.cir)
		{
            //Fprintf("np_creat_fr_iw_channel fr_shaping_param.cir rate %d is big than %d set it to ",fr_shaping_param.cir,WT_MAX_CH_RATE);
            fr_shaping_param.cir = WT_MAX_CH_RATE ;
			//Fprintf("%d\n",fr_shaping_param.cir);
		}
		fr_shaping_param.cir_polling = fr_shaping_param.cir;
		fr_shaping_param.cbs = initial_cfg->stru_stm1_cfg_info[ch_info->card-1].trunk_info[ch_info->trunk-1].group_info[ch_info->group].PVCQos[ch_info->dlci].bc;
		fr_shaping_param.ebs= initial_cfg->stru_stm1_cfg_info[ch_info->card-1].trunk_info[ch_info->trunk-1].group_info[ch_info->group].PVCQos[ch_info->dlci].be;
		fr_shaping_param.eir_polling = WP_SHAPING_POLL_AUTOD;
		fr_shaping_param.eir = fr_shaping_param.cir;
		fr_shaping_param.eir_tq = 2;/*lower priority*/
		fr_shaping_param.eir_cwid = WP_CW_ID_A;

		fr_ch_config.tx_shaping_type = WP_PKT_SHAPING_CIR_EIR;
		fr_ch_config.tx_shaping_params = &fr_shaping_param;
		fr_ch_config.tx_tq = 1; /*TX tq=0 strict TX tq=1 shapping*/

	}
#else
	{
		//WP_pkt_shaping_ppr fr_shaping_param;
		WP_pkt_shaping_wfq fr_shaping_param;
		fr_shaping_param.weight 		   = 1;
		fr_shaping_param.weight_fraction   = 1;
		fr_ch_config.tx_shaping_type = WP_PKT_SHAPING_WFQ;
		fr_ch_config.tx_shaping_params = &fr_shaping_param;
		fr_ch_config.tx_tq = 1; /*TX tq=0 cir TX tq=1 cir_eir shapping*/

	}
#endif
	fr_ch_config.tx_cwid = WP_CW_ID_A;
	fr_ch_config.rx_fbp_drop_threshold = 2;

	ch_info->tx_iw_handle= WP_ChannelCreate(0,ch_info->device_handle,qnode_fr_iw, WP_CH_TX, WP_FRAME_RELAY ,&fr_ch_config);
	terminate_on_error(ch_info->tx_iw_handle, "WP_ChannelCreate() tx_iw_handle");

	fr_ch_config.rx_dlci = ch_info->dlci;
	ch_info->rx_iw_handle = WP_ChannelCreate(0,ch_info->device_handle,qnode_fr_iw,WP_CH_RX, WP_FRAME_RELAY ,&fr_ch_config);
	terminate_on_error(ch_info->rx_iw_handle, "WP_ChannelCreate() rx_iw_handle");





	WP_iw_agg_fr_switching	fr_iw_tx_agg_config;
	memset(&fr_iw_tx_agg_config,0,sizeof(WP_iw_agg_fr_switching));
	fr_iw_tx_agg_config.flow_agg_type=WP_IW_AGG_PRIMARY;
	fr_iw_tx_agg_config.tag = channeltag;
	fr_iw_tx_agg_config.txfunc =  0;
	fr_iw_tx_agg_config.rfcs = 0;
	fr_iw_tx_agg_config.interruptqueue = WP_IW_IRQT0;
	fr_iw_tx_agg_config.error_pkt_mode = WP_IW_ERRPKT_DISCARD;
	fr_iw_tx_agg_config.intmode = WP_IW_INT_ENABLE;
	fr_iw_tx_agg_config.statmode = WP_IW_STAT_ENABLE;
	fr_iw_tx_agg_config.timestamp_mode = WP_IW_TIME_STAMP_DISABLE;
	fr_iw_tx_agg_config.ov_pool_mode = WP_IW_OV_POOL_DISABLE;
	fr_iw_tx_agg_config.fbp_drop_threshold = 0;
	fr_iw_tx_agg_config.mtu = 2500;
	memset(fr_iw_tx_agg_config.prefix_header, 0, 32);
	fr_iw_tx_agg_config.l2_header_insert_mode = WP_IW_L2H_INSERT_DISABLE;
	fr_iw_tx_agg_config.extraction_length = 0;/*don't remove the fr header let host to do it*/
	fr_iw_tx_agg_config.prefix_length = 0;
	fr_iw_tx_agg_config.auto_protocol_conversion_mode = WP_IW_PROTOCOL_CONV_DISABLE;
	fr_iw_tx_agg_config.protocol_pdu_update_mode = WP_IW_PDU_UPDATE_DISABLE;
    /*
      agg_config.protocol_specific.flags_set_mask = set_mask;
      agg_config.protocol_specific.flags_clear_mask = clear_mask;

	*/
	/* Create Flow aggregation */
    fr_iw_tx_agg_config.txfunc = ch_info->rx_host_handle;//ch_info->rx_host_handle;
    /*agg_fr_rx_host_flow just create once,can't be deleted*/
    if(ch_info->agg_fr_rx_host_flow == 0)
    {
        ch_info->agg_fr_rx_host_flow = WP_IwFlowAggregationCreate(0,
                                                                  WP_IW_FR_SWITCHING_MODE,
                                                                  &fr_iw_tx_agg_config);
        terminate_on_error(ch_info->agg_fr_rx_host_flow, "WP_IwFlowAggregationCreate ch_info->agg_fr_rx_host_flow");
        WDDI_INFO("Info: np_creat_fr_iw_channel, created agg_fr_rx_host_flow:0x%x\n", ch_info->agg_fr_rx_host_flow);
    }
    WP_rx_binding_fr_switching rx_binding_config;
	memset(&rx_binding_config, 0, sizeof(WP_rx_binding_fr_switching));

	rx_binding_config.default_aggregation = ch_info->agg_fr_rx_host_flow;
	rx_binding_config.mru = N_FR_MAX_SDU ;
	status = WP_IwRxBindingCreate(ch_info->rx_iw_handle,
								  np_system->system_fr_handle,
								  qnode_fr_iw,
								  &rx_binding_config);

	terminate_on_error(status, "np_creat_fr_iw_channel WP_IwRxBindingCreate");


	status = WP_IwSystemBuild(np_system->system_fr_handle);

	fr_iw_tx_agg_config.tag = RX_FR_INT_PRIO_MSG_TAG;
	fr_iw_tx_agg_config.txfunc = np_system->Channel_FR_PrioRx0_host_handle;
    /*agg_fr_rx_host_flow just create once,can't be deleted*/
    if(np_system->agg_FR_PrioRx0_host_handle == 0)
    {
        np_system->agg_FR_PrioRx0_host_handle = WP_IwFlowAggregationCreate(0,
                                                                           WP_IW_FR_SWITCHING_MODE,
                                                                           &fr_iw_tx_agg_config);
        terminate_on_error(np_system->agg_FR_PrioRx0_host_handle, "WP_IwFlowAggregationCreate agg_FR_PrioRx0_host_handle");


        status = WP_ChannelEnable(np_system->Channel_FR_PrioRx0_host_handle);
        terminate_on_error(status, "WP_ChannelEnable() Channel_FR_PrioRx0_host_handle");

#if 0
        u32 FlowIndex = WPI_HANDLE_INDEX(np_system->agg_FR_PrioRx0_host_handle);
        Fprintf("np_system->agg_FR_PrioRx0_host_handle FlowIndex = 0x%x \n",FlowIndex);
        u32 tag = 0;
        int status;
        status = WP_WDDI_ResFitwGetTag(FlowIndex,&tag);
        if(status)
        {
            Fprintf("np_system->agg_FR_PrioRx0_host_handle get fail = 0x%x \n",FlowIndex);
        }
        Fprintf("np_system->agg_FR_PrioRx0_host_handle tag = 0x%x \n",tag);
#endif

#if NPU_PRIORITY_MSG
        wp_priority_msg flowTable;
        memset(&flowTable,0,sizeof(flowTable));
        flowTable.flow_num[0] = np_system->agg_FR_PrioRx0_host_handle;
        (void)WP_PriorityMsgFlowSet(0,WP_PRIORIYT_MSG_FR_TYPE,&flowTable);
#endif
    }

#if 0
	WP_flow_fr_sw flow_fr_sw;
	flow_fr_sw.aggregation = ch_info->agg_fr_rx_host_flow;
	WP_handle flow_handle;
	flow_handle = WP_IwFlowAdd(np_system->system_fr_handle,
							   WP_FLOW_FR_SWITCHING,
							   &flow_fr_sw);
#endif

	/*Enable FR host rx */
	status = WP_ChannelEnable(ch_info->rx_iw_handle);
	terminate_on_error(status, "WP_ChannelEnable() rx_fr_iw_handle");


	WP_tx_binding tx_binding_config;
	tx_binding_config.dci_mode = WP_IW_DYN_CH_INSERT_ENABLE;
    tx_binding_config.maxt = NPU_FR_TX_MAXT;
	status = WP_IwTxBindingCreate(ch_info->tx_iw_handle, WP_IW_TX_BINDING,&tx_binding_config);
	terminate_on_error(status, "WP_IwTxBindingCreate() tx_fr_iw_handle");

	/*Enable FR hostand tx channels*/
	status = WP_ChannelEnable(ch_info->tx_iw_handle);
	terminate_on_error(status, "WP_ChannelEnable() tx_fr_iw_handle");
}

WP_tx_binding  tx_fr_binding[1]=
{
    { // Ethernet Tx binding
        /* wred_mode;*/ 0,
        /* dci_mode;*/  WP_IW_DYN_CH_INSERT_ENABLE,
        /* maxt;*/NPU_FR_RX_MAXT,
    }
};


WP_ch_iw_rx ch_config_iw_fr[1]=
{
    {
        /* pqblock */ 0,
        /* pqlevel */ 0,
        /* tx_binding_type */ WP_IW_TX_BINDING,
        /* tx_binding_config */ tx_fr_binding
    }
};

WP_ch_iw_tx ch_config_iw_tx_fr[1] =
{
    {
        /* iw_system */				 0,
        /* reserved */ 				 0,
        /* group_id */ 				 WP_IW_CH_GROUP_ID_NULL,
        /* bridging_group_tag */		 0,
        /* input_port */				 0
    }
};
extern WP_handle iw_sys_bridging;
extern WP_bridge_port bport_conf[];
void np_creat_fr_host_channel(FR_CHANNEL_INFO * ch_info)
{
	WP_handle	   status;
	/*Create frame relay host channels, one Tx/Rx pair per ufe link */
	WP_ch_fr fr_ch_config;
	//WP_pkt_shaping_ppr fr_shaping_param;

	memset((void*)&fr_ch_config,0,sizeof(fr_ch_config));
	fr_ch_config.rx_fbp_drop_threshold = 2;

	fr_ch_config.intmode = WP_PKTCH_INT_DISABLE;
	fr_ch_config.statmode = WP_PKTCH_STAT_ENABLE;
	fr_ch_config.iwmode = WP_PKTCH_IWM_DISABLE;
	fr_ch_config.rx_queue_threshold = 200;
	fr_ch_config.tx_opmode = WP_FRCH_OP_NORMAL;
	fr_ch_config.tx_fr_header_type = WP_FRCH_HEADER_2BYTES;
	fr_ch_config.tx_fragment_size = 0;
	fr_ch_config.tx_pqblock = 0;
	fr_ch_config.tx_pqlevel = 0;
	STRU_SET_INITAIL_CONFIG *initial_cfg = npu_get_initial_cfg_info();
	//fr_shaping_param.ppr = 3000000;
	//fr_shaping_param.polling = WP_SHAPING_POLL_AUTOD;
#if START_SCHEDULER
    WP_pkt_shaping_cir fr_shaping_param;
    fr_ch_config.tx_shaping_type = WP_PKT_SHAPING_CIR;
#if 1 /*WP_PKT_SHAPING_CIR if necessary*/
    fr_ch_config.tx_shaping_params = &fr_shaping_param;
    fr_shaping_param.cir = initial_cfg->stru_stm1_cfg_info[ch_info->card-1].trunk_info[ch_info->trunk-1].group_info[ch_info->group].PVCQos[ch_info->dlci].cir;
    if(WT_MIN_CH_RATE > fr_shaping_param.cir)
    {
        //Fprintf("np_creat_fr_iw_channel fr_shaping_param.cir rate %d is low than %d set is to ",fr_shaping_param.cir,WT_MIN_CH_RATE);
        fr_shaping_param.cir = WT_MIN_CH_RATE ;
        //Fprintf("%d\n",fr_shaping_param.cir);
    }
    if(WT_MAX_CH_RATE < fr_shaping_param.cir)
    {
        //Fprintf("np_creat_fr_iw_channel fr_shaping_param.cir rate %d is big than %d set it to ",fr_shaping_param.cir,WT_MAX_CH_RATE);
        fr_shaping_param.cir = WT_MAX_CH_RATE ;
        //Fprintf("%d\n",fr_shaping_param.cir);
    }
	fr_shaping_param.cbs = fr_shaping_param.cir/100;
	fr_shaping_param.polling = WP_SHAPING_POLL_AUTOD;
#endif
	fr_ch_config.tx_tq = 0;/*high prority*/

#else
	WP_pkt_shaping_wfq fr_shaping_param;
	fr_shaping_param.weight            = 1;
	fr_shaping_param.weight_fraction   = 1;
	fr_ch_config.tx_shaping_type = WP_PKT_SHAPING_WFQ;
	fr_ch_config.tx_shaping_params = &fr_shaping_param;
	fr_ch_config.tx_tq = 0; /*TX tq=0 cir shapping TX tq=1 cir_eir shapping*/

#endif
	fr_ch_config.tx_cwid = WP_CW_ID_A;

	NP_SYSTEM_INFO * np_system;
	np_system = np_get_system_info();

#if 0
    WP_handle bridge_port;
	bridge_port = WP_IwPortCreate(iw_sys_bridging,bport_conf);
    terminate_on_error( bridge_port,"IwHostTxChannel() WP_IwPortCreate");
	ch_config_iw_tx_fr->input_port = bridge_port;
	ch_config_iw_tx_fr->iw_system = iw_sys_bridging;
#endif

	//ch_info->tx_host_handle= WP_ChannelCreate(0,fr_iw_host_device,qnode_fr_iw, WP_CH_TX, WP_IW_HOST ,&ch_config_iw_tx_fr[0]);

	ch_info->tx_host_handle= WP_ChannelCreate(0,ch_info->device_handle,qnode_fr_host, WP_CH_TX, WP_FRAME_RELAY ,&fr_ch_config);
	terminate_on_error(ch_info->tx_host_handle, "WP_ChannelCreate() tx_host_handle");


	//ch_info->rx_host_handle = WP_ChannelCreate(channeltag,fr_iw_host_device,qnode_fr_iw,WP_CH_RX, WP_IW_HOST ,&fr_ch_config);ch_config_iw_fr[0]
	ch_info->rx_host_handle = WP_ChannelCreate(0,fr_iw_host_device,qnode_fr_iw,WP_CH_RX, WP_IW_HOST ,&ch_config_iw_fr[0]);

	terminate_on_error(ch_info->rx_host_handle, "WP_ChannelCreate() rx_host_handle");


	WP_tx_binding  tx_fr_binding_priority[1]=
        {
            { // Ethernet Tx binding
                /* wred_mode;*/ 0,
                /* dci_mode;*/  WP_IW_DYN_CH_INSERT_ENABLE,
                /* maxt;*/NPU_FR_RX_MAXT_PRI,
            }
        };


	WP_ch_iw_rx ch_config_iw_fr_priority[1]=
        {
            {
                /* pqblock */ 0,
                /* pqlevel */ 0,
                /* tx_binding_type */ WP_IW_TX_BINDING,
                /* tx_binding_config */ tx_fr_binding_priority
            }
        };


	if(np_system->Channel_FR_PrioRx0_host_handle == 0)
	{
        np_system->Channel_FR_PrioRx0_host_handle = WP_ChannelCreate(0,fr_iw_host_device,qnode_fr_iw,WP_CH_RX, WP_IW_HOST ,&ch_config_iw_fr_priority[0]);
        terminate_on_error(np_system->Channel_FR_PrioRx0_host_handle, "WP_ChannelCreate() Channel_FR_PrioRx0_host_handle");
	}
	/*Enable FR host rx and tx channels*/

	//status = WP_ChannelEnable(np_system->Channel_FR_PrioRx0_host_handle);
	//terminate_on_error(status, "WP_ChannelEnable() Channel_FR_PrioRx0_host_handle");

	status = WP_ChannelEnable(ch_info->rx_host_handle);
	terminate_on_error(status, "WP_ChannelEnable() rx_fr_host_handle");

	status = WP_ChannelEnable(ch_info->tx_host_handle);
	terminate_on_error(status, "WP_ChannelEnable() tx_fr_host_handle");

}

void WT_SerdesInterrupt(WP_U32 wpid, WP_U32 signal_info)
{
    unsigned int i, int_reg, sta_reg, tmp;

    printf("serdes\n"); fflush(stdout);

    // read and clear
    tmp = *(volatile unsigned int *)(unsigned long)(0x1e0082ec + WPL_RIF_VIRTUAL(0, 0));
    *(volatile unsigned int *)(unsigned long)(0x1e0082ec + WPL_RIF_VIRTUAL(0,0)) = tmp;
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


static void WTUFE_config_system( WP_U32 ufe_lb_mode,WP_U32 ufe_port)
{
    WUFE_init_config wufe_config = {0};
    WUFE_system wufe_system_config = {0};
    WUFE_status status;
    WP_U32 i, j, k, property;
    //STRU_SET_INITAIL_CONFIG *initial_cfg = npu_get_initial_cfg_info();
    //Initialise the UFE system
    for(i = 0;i<1;i++)
   	{
        wufe_config.ufe_id = UFE_ID+i;
        if(ufe_port == WTUFE_UPI_PORT_1)
            wufe_config.attributes = WUFE_ATTRIB_CONNECT_OFFSET_F(0);
        else
            wufe_config.attributes = WUFE_ATTRIB_CONNECT_OFFSET_F(1);


        status = WUFE_UfeInit(&wufe_config);
        //terminate_on_error(status, "WUFE_UfeInit()");
        WT_UfeTerminateOnError(status,"WUFE_UfeInit",i,__LINE__);

        status = WUFE_DeviceTypeSet(0, WUFE_DEVICE_TYPE_UFE412_M4);
        WT_UfeTerminateOnError(status, "WUFE_DeviceTypeSet", i, __LINE__);

        // Configure the UFE system

        wufe_system_config.max_line_number = N_UFE_MAX_LINES_T1;
        wufe_system_config.max_phy_number =  N_UFE_MAX_PHYS;
        wufe_system_config.line_interface = WUFE_INTERFACE_SDH;
        wufe_system_config.fifo_manager.loopback	= WUFE_FM_LOOPBACK_DISABLE;
        wufe_system_config.fifo_manager.rx_fbp_size = N_UFE_MAX_PHYS *4;
        wufe_system_config.fifo_manager.tx_fbp_size = N_UFE_MAX_PHYS *4;
        wufe_system_config.max_buff_num  = WUFE_MAX_BUF_SIZE_64;

        wufe_system_config.idle_pattern.tx_idle_byte_data_pattern = 0x81;
        wufe_system_config.idle_pattern.tx_idle_nibble_cas_pattern = 4;
        wufe_system_config.emphy.rx_parity_mode = WUFE_RX_PARITY_EVEN;
        wufe_system_config.emphy.tx_parity_mode = WUFE_RX_PARITY_EVEN;
        wufe_system_config.emphy.tx_parity_check = WUFE_TX_PARITY_IGNORE;

        wufe_system_config.system_clock_rec_enable = WUFE_SYSTEM_CLOCK_REC_DISABLE;
        wufe_system_config.clk_rec_system_params = NULL;

        wufe_system_config.clock_rec_sys_params = NULL;

        status = WUFE_SystemConfig(UFE_ID+i, &wufe_system_config);
        //terminate_on_error(status, "WUFE_SystemConfig()");
        WT_UfeTerminateOnError(status,"WUFE_SystemConfig",i,__LINE__);

        Fprintf("UFE System Configuration Complete ufeId = %d\n",i);

        /* Enable the UFE system*/
        property = WUFE_SYS_EMPHY;
        status = WUFE_SystemEnable(UFE_ID+i, property, WUFE_FULL_DUPLEX);
        //terminate_on_error(status, "WUFE_SystemEnable()");
        WT_UfeTerminateOnError(status,"WUFE_SystemEnable",i,__LINE__);
   	}
    /*new*/
#if 1
    WPL_IntConnect(WP_WINPATH(0), WPL_Eint3Ufe4, 0, WT_Eint3Interrupt);
    WPL_IntEnable (WP_WINPATH(0), WPL_Eint3Ufe4);

#if 0
    WPL_IntConnect(WP_WINPATH(0), WPL_SerdesInterrupt, 0, WT_SerdesInterrupt);
    WPL_IntEnable (WP_WINPATH(0), WPL_SerdesInterrupt);

    WPL_IntConnect(WP_WINPATH(0), WPL_SgmiiAn, 0, WT_SgmiiAnInterrupt);
    WPL_IntEnable (WP_WINPATH(0), WPL_SgmiiAn);


    // Enable connected SerDes interrupts
    *(volatile unsigned int *)(unsigned long)(0x1e0082f0 + WPL_RIF_VIRTUAL(0, 0)) = 0;
    *(volatile unsigned int *)(unsigned long)(0x1e0082f4 + WPL_RIF_VIRTUAL(0, 0)) = htonl(0x80000777);
#endif
    // set VPP for SFP2/SFP3 vpp 0.616
#if ENEA_RELEASE
	*(volatile unsigned int *)(unsigned long)(0x1e0082c0 + WPL_RIF_VIRTUAL(0, 0)) = htonl(0x05618000);
	*(volatile unsigned int *)(unsigned long)(0x1e0082c4 + WPL_RIF_VIRTUAL(0, 0)) = htonl(0x05618000);
	*(volatile unsigned int *)(unsigned long)(0x1e0082c8 + WPL_RIF_VIRTUAL(0, 0)) = htonl(0x05618000);
	*(volatile unsigned int *)(unsigned long)(0x1e0082bc + WPL_RIF_VIRTUAL(0, 0)) = htonl(0x05618000);
#endif

    status = WUFE_UfeFramerHwInterruptEnable(0);
    WT_UfeTerminateOnError(status, "WUFE_UfeFramerHwInterruptEnable",0,__LINE__);

    //status = WUFE_UfeCoreHwInterruptEnable(0);
    //WT_UfeTerminateOnError(status, "WUFE_UfeCoreHwInterruptEnable",0,__LINE__);
    WUFE_UfeCoreHwInterruptDisable(0);
    WT_UfeTerminateOnError(status, "WUFE_UfeCoreHwInterruptDisable",0,__LINE__);
    status = WUFE_SystemInterruptDisable(0);
    WT_UfeTerminateOnError(status, "WUFE_SystemInterruptDisable ",0,__LINE__);
    //status = WUFE_SystemInterruptEnable(0);
    //WT_UfeTerminateOnError(status, "WUFE_SystemInterruptEnable ",0,__LINE__);

    status = WUFE_UfeCoreHwInterruptDisable(0);
    WT_UfeTerminateOnError(status, "WUFE_UfeCoreHwInterruptDisable",0,__LINE__);
#endif
    /*Enable the UFE EMPHY Port*/
#if 0 /*for debug*/
    WP_U32 pll_data;
    WUFEI_BoardReadZarlinkPllReg(0, 0x00, &pll_data);
    Fprintf("PLL REG	0: %02x\n", pll_data);
    WUFEI_BoardReadZarlinkPllReg(0, 0x61, &pll_data);
    Fprintf("PLL REG 61: %02x\n", pll_data);
#endif

	WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4;
	WTI_flexmux_physical_conf physical_configuration ;
	WTI_Flexmux_global_setup flex_global_params;
	WP_U8 flexmux_mode=0;

	physical_configuration = WT_FLEXMUX_UFE_412_4_PORTS_OC3;
	//physical_configuration = WT_FLEXMUX_UFE_412_1_PORT_OC12;
	//physical_configuration = WT_FLEXMUX_UFE_412_2_PORTS_OC3;//WT_FLEXMUX_UFE_412_4_PORTS_OC3;
	vc_3_4 = WPX_UFE_FRAMER_WUFE_SONET_TYPE_STS3;
	flexmux_mode = WPX_UFE_FRAMER_DEVICE_MODE_SONET;

	WTI_FlexmuxInit(ufe_port,
					0,
					physical_configuration,
					flexmux_mode,
					vc_3_4,
					&flex_global_params);

	/*set lineport1 protection mode*/
	status = WTI_FlexmuxForceB(1);
    terminate_on_error(status, "WPX_FRMR_SONET_SDH_PORT_ForceB");
}

static void WTUFE_config_lines(WP_U32 line_lb_mode)
{
    WP_U32 line_index;
    WP_U32 line_index_counts;
    static WP_U32 E1MapIndex = 0;
    WP_U16 framing_mode;
    WP_U8 cas_enable = WUFE_CAS_ENABLE;
    WP_U8 e1_mode = NPU_MODE_E1;
    WP_U32 clock_mode = UFE_CLOCK_SLAVE_MODE;
    STRU_SET_INITAIL_CONFIG *initial_cfg = npu_get_initial_cfg_info();
    if(initial_cfg->OpterateMode == NPU_MODE_IP)
    {
        return ;
    }
    /*for E1 mode*/
    /*both stm1 configured*/
    if(initial_cfg->stru_stm1_cfg_info[0].stm1Valid == NPU_VALID && initial_cfg->stru_stm1_cfg_info[1].stm1Valid == NPU_VALID)
    {
        /*make sure mode must be same*/
        if(initial_cfg->stru_stm1_cfg_info[0].E1T1type != initial_cfg->stru_stm1_cfg_info[1].E1T1type)
        {
            WDDI_ERR("can't support STM1-1 STM-2 configured to different mode \n");
        }
        else
        {
            if(initial_cfg->stru_stm1_cfg_info[0].E1T1type == NPU_MODE_E1)
            {
                e1_mode = NPU_MODE_E1;
            }
            else
            {
                e1_mode = NPU_MODE_T1;
            }
        }
    }/*signal stm1 configured*/
    else if(initial_cfg->stru_stm1_cfg_info[0].stm1Valid == NPU_VALID)
    {
        e1_mode = initial_cfg->stru_stm1_cfg_info[0].E1T1type;
    }
    else if(initial_cfg->stru_stm1_cfg_info[1].stm1Valid == NPU_VALID)
    {
        e1_mode = initial_cfg->stru_stm1_cfg_info[1].E1T1type;
    }
    else
    {
        // WDDI_ERR("no STM1-1 valid configured\n");
    }
    if(e1_mode == NPU_MODE_E1)
    {
        Fprintf("NPU_MODE_E1\n");
        line_index_counts = N_UFE_MAX_LINES_E1;
        for (line_index=0; line_index < line_index_counts; line_index++)
        {
            if((line_index%N_UFE_MAX_LINES_SPE_T1)>=N_UFE_MAX_LINES_SPE_E1)
            {
                line_index_counts++;
                continue;
            }
            E1MapTable[E1MapIndex++] = line_index;/*E1MapTable[]={0~20 ,28~48,56~76}*/
        }
        for (line_index=0; line_index < N_UFE_MAX_LINES_E1; line_index++)
        {

            u32 stm1Id,trunkId;
            stm1Id = line_index/(N_UFE_MAX_LINES_E1/2);
            trunkId = line_index%(N_UFE_MAX_LINES_E1/2);
            if(OPMODE_E1_FALSE == initial_cfg->stru_stm1_cfg_info[stm1Id].trunk_info[trunkId].Opt_Mode)
            {
                framing_mode = WUFE_UNFRAMED;
                cas_enable = WUFE_CAS_DISABLE;
            }/*OPMODE_E1_TRUE_CRC4*/
            else
            {
	   // WDDI_ERR("no STM1-1 valid configured\n");
                framing_mode = WUFE_FRAMED;
				int i;
                if(initial_cfg->stru_stm1_cfg_info[stm1Id].trunk_info[trunkId].trunkValid)
                {
					for(i = 0;i < 32;i++)
					{
						if(initial_cfg->stru_stm1_cfg_info[stm1Id].trunk_info[trunkId].group_info[i].groupValid)
						{
							if(initial_cfg->stru_stm1_cfg_info[stm1Id].trunk_info[trunkId].group_info[i].slot_selection&0x00010000)
							{
								cas_enable = WUFE_CAS_DISABLE;
								break;
							}
						}
					}
				}
            }

            if(initial_cfg->stru_stm1_cfg_info[stm1Id].trunk_info[trunkId].trunkValid)
            {
                if(initial_cfg->stru_stm1_cfg_info[stm1Id].trunk_info[trunkId].clock_mode == NPU_E1T1_CLOCK_MASTER)
                {
                    clock_mode = UFE_CLOCK_MASTER_MODE;
                }
                else
                {
                    clock_mode = UFE_CLOCK_SLAVE_MODE;
                }
                Fprintf("line_index = %d framing_mode = %d cas_enable = %d clock_mode = %d \n",line_index,framing_mode,cas_enable,clock_mode);
                WT_Np_UfeLineConfigureAndCreateE1(line_index, framing_mode, cas_enable,line_lb_mode,clock_mode);
                reconfigStm1_createE1_flag[line_index] = 1;
            }
            else // for these un-configured E1s, we need create the defaut object to clear the LOP alarms on the peer transmission device;
            {
                Fprintf("Create Default E1s for these un-configured E1s: line_index = %d framing_mode = %d cas_enable = %d clock_mode = %d \n",line_index,WUFE_FRAMED,cas_enable,WUFE_CAS_DISABLE);
                WT_Np_UfeLineConfigureAndCreateE1(line_index, WUFE_FRAMED, WUFE_CAS_DISABLE,line_lb_mode,UFE_CLOCK_SLAVE_MODE);
                reconfigStm1_createE1_flag[line_index] = 1;
            }

        }
    } /*for T1 mode*/
    else
    {
        Fprintf("NPU_MODE_T1\n");
        line_index_counts = N_UFE_MAX_LINES_T1;

        for (line_index=0; line_index < line_index_counts; line_index++)
        {
            u32 stm1Id,trunkId;
            stm1Id	= line_index/(N_UFE_MAX_LINES_T1/2);
            trunkId = line_index%(N_UFE_MAX_LINES_T1/2);

			if(OPMODE_T1_D4 == initial_cfg->stru_stm1_cfg_info[stm1Id].trunk_info[trunkId].Opt_Mode)
			{
				framing_mode = WUFE_FRAMED;
				cas_enable = WUFE_CAS_DISABLE;
			}
			else /*default: OPMODE_T1_ESF*/
			{
				framing_mode = WUFE_FRAMED;
				cas_enable = WUFE_CAS_ENABLE;
			}
			E1MapTable[E1MapIndex++] = line_index;
			if(initial_cfg->stru_stm1_cfg_info[stm1Id].trunk_info[trunkId].trunkValid)
			{

				if(initial_cfg->stru_stm1_cfg_info[stm1Id].trunk_info[trunkId].clock_mode == NPU_E1T1_CLOCK_MASTER)
				{
					clock_mode = UFE_CLOCK_MASTER_MODE;
				}
				else
				{
					clock_mode = UFE_CLOCK_SLAVE_MODE;
				}
				WT_Np_UfeLineConfigureAndCreateT1(line_index, framing_mode, cas_enable,line_lb_mode,clock_mode);
                reconfigStm1_createE1_flag[line_index] = 1;
			}
        }
    }
}


void WTUFE_config_phys(WP_U32 line_index,WP_U32 groupId,WP_U32 slot_selection,WP_U32 line_index_base/*,WP_handle *phy_handle,WP_handle *device_handle*/)
{
	WUFE_phy phy_cfg;
    WUFE_SlotGroup slot_group;
	WP_U32 nslots, slots, j;
	WP_status status;
	NP_TRUNK_INFO* ptrunkinfo;
	NP_TRUNK_INFO trunkinfo;
	STRU_SET_INITAIL_CONFIG *initial_cfg = npu_get_initial_cfg_info();
	memset((void *) &phy_cfg, 0, sizeof(WUFE_phy));
    phy_cfg.struct_id = WUFE_STRUCT_PHY_HDLC;
	phy_cfg.rx_fifo_param.transmit_th = 0;
	phy_cfg.rx_fifo_param.fifo_max_size = 4;
	phy_cfg.rx_fifo_param.priority_table = 0;
	phy_cfg.tx_fifo_param.fifo_max_size = 4;
	phy_cfg.tx_fifo_param.priority_table = 0;
	phy_cfg.tx_fifo_param.fast_mode = WUFE_PHY_FIFO_MODE_SLOW;
	phy_cfg.tx_fifo_param.transmit_th = 0;
	phy_cfg.tx_fifo_param.wait_type = WUFE_PHY_TX_WAIT_TYPE_D;

	//phy_cfg.type.hdlc.rx_frame_include_crc_mode =WUFE_PHY_HDLC_CRC_ENABLE;//WUFE_PHY_HDLC_CRC_DISABLE;
	//phy_cfg.type.hdlc.tx_frame_include_crc_mode =WUFE_PHY_HDLC_CRC_ENABLE;
	int stm1Id;
	if(line_index_base == 0)
	{
        stm1Id = 0;
	}
	else
	{
        stm1Id = 1;
	}
	if(NPU_E1T1_CRC32 == initial_cfg->stru_stm1_cfg_info[stm1Id].trunk_info[line_index - line_index_base].group_info[groupId].crc_type)
	{
		phy_cfg.type.hdlc.tx_frame_crc_size = WUFE_PHY_HDLC_CRC_32;
		phy_cfg.type.hdlc.rx_frame_crc_size = WUFE_PHY_HDLC_CRC_32;
	}
	else
	{
		phy_cfg.type.hdlc.tx_frame_crc_size = WUFE_PHY_HDLC_CRC_16;
		phy_cfg.type.hdlc.rx_frame_crc_size = WUFE_PHY_HDLC_CRC_16;
	}
	phy_cfg.type.hdlc.tx_no_flag_between_frames =WUFE_PHY_HDLC_TX_FLAGS_BETWEEN_FRAMES;//WUFE_PHY_HDLC_TX_FLAGS_BETWEEN_FRAMES;
	phy_cfg.type.hdlc.tx_min_flag_num = 1;

	memset (&slot_group, 0, sizeof(WUFE_SlotGroup));

	slots = slot_selection;

	for (j = 0, nslots = 0, slots = slot_selection; slots > 0; ++j, slots >>= 1)
	{
        if (slots & 1)
        {
            slot_group.slot_arr[nslots] = j;
            ++nslots;
            Fprintf("===== nslots:%u, value:%u\n", nslots, j);
        }
	}
    slot_group.n_slots = nslots;

	/*phy_cfg.tx_slot_group = phy_cfg.rx_slot_group = slot_group;*/
	memcpy(&phy_cfg.rx_slot_group,&slot_group,sizeof(slot_group));
	memcpy(&phy_cfg.tx_slot_group,&slot_group,sizeof(slot_group));

    /*Create and Enable UFE Phy per UFE line*/

	status = WUFE_PhyCreate(&ufe_phy_handle[line_index][groupId],
                            ufe_line_handle[line_index],
                            &phy_cfg,
                            WUFE_FULL_DUPLEX);

	//terminate_on_error(status, "WUFE_PhyCreate()");
	WT_UfeTerminateOnError(status,"WUFE_PhyCreate",line_index,__LINE__);
	status = WUFE_PhyEnable(ufe_phy_handle[line_index][groupId], WUFE_FULL_DUPLEX);
	terminate_on_error(status, "WUFE_PhyEnable()");

	/*Create EMPHY Frame Relay Device per Phy*/
	fr_device_config[0].rx_queue = qnode_fr_iw;//qnode_fr_host;
#if START_SCHEDULER
	fr_device_config[0].tx_tq_types[0] = WP_PKT_SCHED_SHAPING;
	fr_device_config[0].tx_tq_types[1] = WP_PKT_SCHED_SHAPING;
	fr_device_config[0].tx_tq_types[2] = WP_PKT_SCHED_SHAPING;/*eir use*/
	fr_device_config[0].tx_tq_types[3] = WP_PKT_SCHED_WFQ;
#else
	fr_device_config[0].tx_tq_types[0] = WP_PKT_SCHED_WFQ;
	fr_device_config[0].tx_tq_types[1] = WP_PKT_SCHED_WFQ;
	fr_device_config[0].tx_tq_types[2] = WP_PKT_SCHED_WFQ;
	fr_device_config[0].tx_tq_types[3] = WP_PKT_SCHED_WFQ;
#endif


	fr_device_config[0].tx_maxsdu = initial_cfg->stru_stm1_cfg_info[stm1Id].trunk_info[line_index - line_index_base].group_info[groupId].max_frame_size;
	fr_device_config[0].rx_maxsdu = initial_cfg->stru_stm1_cfg_info[stm1Id].trunk_info[line_index - line_index_base].group_info[groupId].max_frame_size;
   	ufe_device_handle[line_index][groupId]= WP_DeviceCreate(upi_ufe_port,
                                                            WP_PHY(WUFE_PHY_INDEX_GET(ufe_phy_handle[line_index][groupId])),
                                                            WP_DEVICE_FR,
                                                            &fr_device_config[0]);
   	terminate_on_error(ufe_device_handle[line_index][groupId], "WP_DeviceCreate ufe device()");
	status = WP_DeviceEnable( ufe_device_handle[line_index][groupId], WP_DIRECTION_DUPLEX);
	terminate_on_error(status, "WP_DeviceEnable, EMPHY");


	ptrunkinfo = npu_get_trunk_info(line_index);
    if(NULL != ptrunkinfo)
    {
        for(j = 0; j < 32; j++)
        {
            if(ptrunkinfo->Groupid[j] == groupId)
            {
                break;
			}
		}
		if(j>=32)
		{
		    ptrunkinfo->trunkid = line_index;
            ptrunkinfo->Groupid[(ptrunkinfo->GroupNum)] = groupId;
			(ptrunkinfo->GroupNum)++;
		}
        (void)npu_set_trunk_info(line_index, ptrunkinfo);
	}
	else
	{
		trunkinfo.trunkid = line_index;
		trunkinfo.Groupid[0] = groupId;
		trunkinfo.GroupNum = 1;
        (void)npu_set_trunk_info(line_index, &trunkinfo);
	}
}

WP_status WTUFE_Reconfig_phys(WP_U32 trunkid,WP_U32 groupid,WP_U32 slot_selection,WP_U32 crc_type)
{
    WUFE_phy usr_phy_cfg;
    WUFE_status status=WUFE_OK;
    volatile WP_U32 timeout;
    WP_U32 slots,j,nslots,i;
    WUFE_SlotGroup slot_group;

    memset(&usr_phy_cfg, 0xff, sizeof(WUFE_phy));

    memset (&slot_group, 0xff, sizeof(WUFE_SlotGroup));
    timeout = 0;

    for (j = 0, nslots = 0, slots = slot_selection; slots > 0; ++j, slots >>= 1)
    {
        if (slots & 1)
        {
            slot_group.slot_arr[nslots] = j;
            ++nslots;
        }
    }
    slot_group.n_slots = nslots;

    memcpy(&usr_phy_cfg.rx_slot_group,&slot_group,sizeof(slot_group));
    memcpy(&usr_phy_cfg.tx_slot_group,&slot_group,sizeof(slot_group));
    usr_phy_cfg.rx_slot_group.hmvip_id = 0;
    usr_phy_cfg.tx_slot_group.hmvip_id = 0;
#if 0
    Fprintf("slot_group.hmvip_id= %d,slot_group.n_slots= %d\n",usr_phy_cfg.rx_slot_group.hmvip_id,usr_phy_cfg.rx_slot_group.n_slots);
    for(i=0;i<32;i++)
    {
        Fprintf("slot_group.slot_arr[%d]= %d\n",i,usr_phy_cfg.rx_slot_group.slot_arr[i]);

    }
#endif


    usr_phy_cfg.type.hdlc.tx_no_flag_between_frames =WUFE_PHY_HDLC_TX_FLAGS_BETWEEN_FRAMES;//WUFE_PHY_HDLC_TX_FLAGS_BETWEEN_FRAMES;
    usr_phy_cfg.type.hdlc.tx_min_flag_num = 1;
    usr_phy_cfg.struct_id = WUFE_STRUCT_PHY_HDLC;

    if(NPU_E1T1_CRC32 == crc_type)
    {
        usr_phy_cfg.type.hdlc.tx_frame_crc_size = WUFE_PHY_HDLC_CRC_32;
        usr_phy_cfg.type.hdlc.rx_frame_crc_size = WUFE_PHY_HDLC_CRC_32;
    }
    else
    {
        usr_phy_cfg.type.hdlc.tx_frame_crc_size = WUFE_PHY_HDLC_CRC_16;
        usr_phy_cfg.type.hdlc.rx_frame_crc_size = WUFE_PHY_HDLC_CRC_16;
    }

    do
    {
        status = WUFE_PhyReconfigure(ufe_phy_handle[trunkid][groupid],&usr_phy_cfg);
        if (status != WUFE_OK && status != WUFE_ERR_PHY_FIFO_NOT_EMPTY)
            break;

        if (++timeout > 100000)
        {
            Fprintf("---------------- Timeout -------------\n");
            break;
        }

        WP_Delay(10);
    }
    while (status != WUFE_OK);

    return WUFE_OK;
}

WP_status WTUFE_Reconfig_Stm1(WP_U32 stm1id, WP_U32 line_index_base,STRU_STM1_RECFG *reconfig_info,STRU_SET_INITAIL_CONFIG *p2)
{
	WP_U32 trunkid,crc_type,i,j;
	WP_U32 groupid,groupValid,slot_selection,max_frame_size;
	WUFE_status status;
    STRU_SET_INITAIL_CONFIG *initial_cfg = npu_get_initial_cfg_info();
	WP_U16 framing_mode;
	WP_U8 cas_enable = WUFE_CAS_ENABLE;
    WP_U32 clock_mode;
	trunkid = reconfig_info->trunkId;


    /*disable phy and device*/
	for(groupid = 0;groupid < 32; groupid ++)
	{
	    groupValid = reconfig_info->group_info[groupid].groupValid;
	    slot_selection = reconfig_info->group_info[groupid].slot_selection;
		crc_type = reconfig_info->group_info[groupid].crc_type;
		max_frame_size = reconfig_info->group_info[groupid].max_frame_size;
		if(NPU_VALID == groupValid)
		{
			initial_cfg->stru_stm1_cfg_info[stm1id].trunk_info[trunkid-line_index_base].group_info[groupid].groupValid = NPU_VALID;
			initial_cfg->stru_stm1_cfg_info[stm1id].trunk_info[trunkid-line_index_base].group_info[groupid].crc_type = crc_type;
			initial_cfg->stru_stm1_cfg_info[stm1id].trunk_info[trunkid-line_index_base].group_info[groupid].max_frame_size = max_frame_size;
			initial_cfg->stru_stm1_cfg_info[stm1id].trunk_info[trunkid-line_index_base].group_info[groupid].slot_selection = slot_selection;
			//Fprintf("WTUFE_Reconfig_Stm1:stm1id = %d,trunk = %d,groupid = %d,trunkid=%d\n",stm1id,trunkid-line_index_base,groupid,trunkid);
		    if(ufe_device_handle[trunkid][groupid] != 0)
		    {
				if(ufe_phy_handle[trunkid][groupid] != 0)
				{
#if 1
					status = WUFE_PhyDisable(ufe_phy_handle[trunkid][groupid], WUFE_FULL_DUPLEX);
					if(WUFE_OK !=status)
					{
					    Fprintf("WUFE_PhyDisable error %d",status);
						return status;
					}
#endif
				}
			    (void)WP_DeviceDisable(ufe_device_handle[trunkid][groupid], WP_DIRECTION_DUPLEX);
				if(ufe_phy_handle[trunkid][groupid] != 0)
				{
					status = WTUFE_Reconfig_phys(trunkid,groupid,slot_selection,crc_type);
					if(WUFE_OK !=status)
					{
						return status;
					}
				}
				if(N_FR_MAX_SDU < max_frame_size)
				{
					Fprintf("max_frame_size is (%d)larger than The MAX_SDU\n",max_frame_size);
					return -1;
				}
				fr_device_config[0].tx_maxsdu = max_frame_size;
				fr_device_config[0].rx_maxsdu = max_frame_size;
				status = WP_DeviceModify(ufe_device_handle[trunkid][groupid],WP_DEV_MOD_UPI_FR_TX_MAXSDU | WP_DEV_MOD_UPI_FR_RX_MAXSDU,&fr_device_config[0]);
				if(WUFE_OK != status)
				{
					return status;
				}
				if(ufe_phy_handle[trunkid][groupid] != 0)
				{
					status = WUFE_PhyEnable(ufe_phy_handle[trunkid][groupid], WUFE_FULL_DUPLEX);
					if(WUFE_OK !=status)
					{
					    Fprintf("WUFE_PhyEnable error %d",status);
						return status;
					}

				}
				(void)WP_DeviceEnable(ufe_device_handle[trunkid][groupid], WP_DIRECTION_DUPLEX);
			}
			else
			{
                if(trunkid >= RECONFIG_TRUNK_MAX)
                {
                    Fprintf("Error: WTUFE_Reconfig_Stm1, trunk ID is invalid\n");
                    return -1;
                }
                if(reconfigStm1_createE1_flag[trunkid] == 0)
                {
                    /*creat new line*/
                    if(p2->stru_stm1_cfg_info[stm1id].E1T1type == NPU_MODE_E1)
                    {
#if 0//bugs
                    	if((trunkid%N_UFE_MAX_LINES_SPE_T1)>=N_UFE_MAX_LINES_SPE_E1)
                    	{
                    		continue;
                    	}
                    	j = trunkid%27;
                    	i = trunkid/21;
                    	E1MapTable[i*20+j] = trunkid;/*E1MapTable[]={0~20 ,28~48,56~76}*/
#endif
                    	if(OPMODE_E1_FALSE == p2->stru_stm1_cfg_info[stm1id].trunk_info[trunkid-line_index_base].Opt_Mode)
                    	{
                    		framing_mode = WUFE_UNFRAMED;
                    		cas_enable = WUFE_CAS_DISABLE;
                    	}/*OPMODE_E1_TRUE_CRC4*/
                    	else
                    	{
                            framing_mode = WUFE_FRAMED;
                            if(p2->stru_stm1_cfg_info[stm1id].trunk_info[trunkid-line_index_base].trunkValid)
                            {
                                for(i =0 ;i< 32;i++)
                                {
                                    if(p2->stru_stm1_cfg_info[stm1id].trunk_info[trunkid-line_index_base].group_info[i].groupValid)
                                    {
                                        if(p2->stru_stm1_cfg_info[stm1id].trunk_info[trunkid-line_index_base].group_info[i].slot_selection&0x00010000)
                                        {
                                            cas_enable = WUFE_CAS_DISABLE;
                                            break;
                                        }
                                    }
                                }

                            }
                    	}

                    	if(p2->stru_stm1_cfg_info[stm1id].trunk_info[trunkid-line_index_base].clock_mode == NPU_E1T1_CLOCK_MASTER)
                    	{
                    		clock_mode = UFE_CLOCK_MASTER_MODE;
                    	}
                    	else
                    	{
                    		clock_mode = UFE_CLOCK_SLAVE_MODE;
                    	}
                        Fprintf("WTUFE_Reconfig_Stm1::trunkid = %d framing_mode = %d cas_enable = %d clock_mode = %d\n",trunkid,framing_mode,cas_enable,clock_mode);
                    	WT_Np_UfeLineConfigureAndCreateE1(trunkid, framing_mode, cas_enable,UFE_NO_LOOPBK,clock_mode);
                    }
                    else
                    {
                        if(OPMODE_T1_D4 == p2->stru_stm1_cfg_info[stm1id].trunk_info[trunkid-line_index_base].Opt_Mode)
                        {
                            framing_mode = WUFE_FRAMED;
                            cas_enable = WUFE_CAS_DISABLE;
                        }
                        else /*default: OPMODE_T1_ESF*/
                        {
                            framing_mode = WUFE_FRAMED;
                            cas_enable = WUFE_CAS_ENABLE;
                        }
                    	if(p2->stru_stm1_cfg_info[stm1id].trunk_info[trunkid-line_index_base].clock_mode == NPU_E1T1_CLOCK_MASTER)
                    	{
                    		clock_mode = UFE_CLOCK_MASTER_MODE;
                    	}
                    	else
                    	{
                    		clock_mode = UFE_CLOCK_SLAVE_MODE;
                    	}
                    	WT_Np_UfeLineConfigureAndCreateT1(trunkid, framing_mode, cas_enable,UFE_NO_LOOPBK,clock_mode);
                    	//E1MapTable[trunkid] = trunkid;
                    }
                    reconfigStm1_createE1_flag[trunkid] = 1;
                }

				initial_cfg->stru_stm1_cfg_info[stm1id].trunk_info[trunkid-line_index_base].Opt_Mode = p2->stru_stm1_cfg_info[stm1id].trunk_info[trunkid-line_index_base].Opt_Mode;
				initial_cfg->stru_stm1_cfg_info[stm1id].trunk_info[trunkid-line_index_base].clock_mode = p2->stru_stm1_cfg_info[stm1id].trunk_info[trunkid-line_index_base].clock_mode;
				initial_cfg->stru_stm1_cfg_info[stm1id].E1T1type = p2->stru_stm1_cfg_info[stm1id].E1T1type;

			    /*creat new phys*/
				WTUFE_config_phys(trunkid,groupid,slot_selection,line_index_base);
			}
		}
	}
	return WUFE_OK;
}


WP_status WTUFE_Reconfig_Stm1_inLoop(WP_U32 stm1id, WP_U32 trunkid, WP_U32 groupid, WP_U32 line_index_base,STRU_STM1_RECFG *reconfig_info,STRU_SET_INITAIL_CONFIG *p2)
{
	WP_U32 crc_type,i,j;
	WP_U32 groupValid,slot_selection,max_frame_size;
	WUFE_status status;
    STRU_SET_INITAIL_CONFIG *initial_cfg = npu_get_initial_cfg_info();
	WP_U16 framing_mode;
	WP_U8 cas_enable = WUFE_CAS_ENABLE;
    WP_U32 clock_mode;

    /*disable phy and device*/
    groupValid = reconfig_info->group_info[groupid].groupValid;
    slot_selection = reconfig_info->group_info[groupid].slot_selection;
	crc_type = reconfig_info->group_info[groupid].crc_type;
	max_frame_size = reconfig_info->group_info[groupid].max_frame_size;
	if(NPU_VALID == groupValid)
	{
        WDDI_INFO("WTUFE_Reconfig_Stm1_inLoop::groupid:%d\n", groupid);
		initial_cfg->stru_stm1_cfg_info[stm1id].trunk_info[trunkid-line_index_base].group_info[groupid].groupValid = NPU_VALID;
		initial_cfg->stru_stm1_cfg_info[stm1id].trunk_info[trunkid-line_index_base].group_info[groupid].crc_type = crc_type;
		initial_cfg->stru_stm1_cfg_info[stm1id].trunk_info[trunkid-line_index_base].group_info[groupid].max_frame_size = max_frame_size;
		initial_cfg->stru_stm1_cfg_info[stm1id].trunk_info[trunkid-line_index_base].group_info[groupid].slot_selection = slot_selection;
		//Fprintf("WTUFE_Reconfig_Stm1:stm1id = %d,trunk = %d,groupid = %d,trunkid=%d\n",stm1id,trunkid-line_index_base,groupid,trunkid);
	    if(ufe_device_handle[trunkid][groupid] != 0)
	    {
			if(ufe_phy_handle[trunkid][groupid] != 0)
			{
#if 1
				status = WUFE_PhyDisable(ufe_phy_handle[trunkid][groupid], WUFE_FULL_DUPLEX);
				if(WUFE_OK !=status)
				{
				    Fprintf("WUFE_PhyDisable error %d",status);
					return status;
				}
#endif
			}
		    (void)WP_DeviceDisable(ufe_device_handle[trunkid][groupid], WP_DIRECTION_DUPLEX);
			if(ufe_phy_handle[trunkid][groupid] != 0)
			{
				status = WTUFE_Reconfig_phys(trunkid,groupid,slot_selection,crc_type);
				if(WUFE_OK !=status)
				{
					return status;
				}
			}
			if(N_FR_MAX_SDU < max_frame_size)
			{
				Fprintf("max_frame_size is (%d)larger than The MAX_SDU\n",max_frame_size);
				return -1;
			}
			fr_device_config[0].tx_maxsdu = max_frame_size;
			fr_device_config[0].rx_maxsdu = max_frame_size;
			status = WP_DeviceModify(ufe_device_handle[trunkid][groupid],WP_DEV_MOD_UPI_FR_TX_MAXSDU | WP_DEV_MOD_UPI_FR_RX_MAXSDU,&fr_device_config[0]);
			if(WUFE_OK != status)
			{
				return status;
			}
			if(ufe_phy_handle[trunkid][groupid] != 0)
			{
				status = WUFE_PhyEnable(ufe_phy_handle[trunkid][groupid], WUFE_FULL_DUPLEX);
				if(WUFE_OK !=status)
				{
				    Fprintf("WUFE_PhyEnable error %d",status);
					return status;
				}

			}
            WDDI_INFO("WTUFE_Reconfig_Stm1_inLoop, device modify ufe_device_handle[%d][%d] \n", trunkid, groupid);
			(void)WP_DeviceEnable(ufe_device_handle[trunkid][groupid], WP_DIRECTION_DUPLEX);
		}
		else
		{
            if(trunkid >= RECONFIG_TRUNK_MAX)
            {
                Fprintf("Error: WTUFE_Reconfig_Stm1_inLoop, trunk ID is invalid\n");
                return -1;
            }
            if(reconfigStm1_createE1_flag[trunkid] == 0)
            {
                /*creat new line*/
			    if(p2->stru_stm1_cfg_info[stm1id].E1T1type == NPU_MODE_E1)
			    {
#if 0//bugs
					if((trunkid%N_UFE_MAX_LINES_SPE_T1)>=N_UFE_MAX_LINES_SPE_E1)
					{
						continue;
					}
					j = trunkid%27;
					i = trunkid/21;
					E1MapTable[i*20+j] = trunkid;/*E1MapTable[]={0~20 ,28~48,56~76}*/
#endif
					if(OPMODE_E1_FALSE == p2->stru_stm1_cfg_info[stm1id].trunk_info[trunkid-line_index_base].Opt_Mode)
					{
						framing_mode = WUFE_UNFRAMED;
						cas_enable = WUFE_CAS_DISABLE;
					}/*OPMODE_E1_TRUE_CRC4*/
					else
					{
                        framing_mode = WUFE_FRAMED;
                        if(p2->stru_stm1_cfg_info[stm1id].trunk_info[trunkid-line_index_base].trunkValid)
                        {
                            for(i =0 ;i< 32;i++)
                            {
                                if(p2->stru_stm1_cfg_info[stm1id].trunk_info[trunkid-line_index_base].group_info[i].groupValid)
                                {
                                    if(p2->stru_stm1_cfg_info[stm1id].trunk_info[trunkid-line_index_base].group_info[i].slot_selection&0x00010000)
                                    {
                                        cas_enable = WUFE_CAS_DISABLE;
                                        break;
                                    }
                                }
                            }

                        }
					}

					if(p2->stru_stm1_cfg_info[stm1id].trunk_info[trunkid-line_index_base].clock_mode == NPU_E1T1_CLOCK_MASTER)
					{
						clock_mode = UFE_CLOCK_MASTER_MODE;
					}
					else
					{
						clock_mode = UFE_CLOCK_SLAVE_MODE;
					}
                    Fprintf("WTUFE_Reconfig_Stm1_inLoop::trunkid = %d framing_mode = %d cas_enable = %d clock_mode = %d\n",trunkid,framing_mode,cas_enable,clock_mode);
					WT_Np_UfeLineConfigureAndCreateE1(trunkid, framing_mode, cas_enable,UFE_NO_LOOPBK,clock_mode);
				}
				else
				{
                    if(OPMODE_T1_D4 == p2->stru_stm1_cfg_info[stm1id].trunk_info[trunkid-line_index_base].Opt_Mode)
                    {
                        framing_mode = WUFE_FRAMED;
                        cas_enable = WUFE_CAS_DISABLE;
                    }
                    else /*default: OPMODE_T1_ESF*/
                    {
                        framing_mode = WUFE_FRAMED;
                        cas_enable = WUFE_CAS_ENABLE;
                    }
					if(p2->stru_stm1_cfg_info[stm1id].trunk_info[trunkid-line_index_base].clock_mode == NPU_E1T1_CLOCK_MASTER)
					{
						clock_mode = UFE_CLOCK_MASTER_MODE;
					}
					else
					{
						clock_mode = UFE_CLOCK_SLAVE_MODE;
					}
					WT_Np_UfeLineConfigureAndCreateT1(trunkid, framing_mode, cas_enable,UFE_NO_LOOPBK,clock_mode);
					//E1MapTable[trunkid] = trunkid;
				}
                reconfigStm1_createE1_flag[trunkid] = 1;
            }

            initial_cfg->stru_stm1_cfg_info[stm1id].trunk_info[trunkid-line_index_base].Opt_Mode = p2->stru_stm1_cfg_info[stm1id].trunk_info[trunkid-line_index_base].Opt_Mode;
            initial_cfg->stru_stm1_cfg_info[stm1id].trunk_info[trunkid-line_index_base].clock_mode = p2->stru_stm1_cfg_info[stm1id].trunk_info[trunkid-line_index_base].clock_mode;
            initial_cfg->stru_stm1_cfg_info[stm1id].E1T1type = p2->stru_stm1_cfg_info[stm1id].E1T1type;

            /*creat new phys*/
            WDDI_INFO("WTUFE_Reconfig_Stm1_inLoop::WTUFE_config_phys,trunkid[%d], groupid[%d] \n", trunkid, groupid);
            WTUFE_config_phys(trunkid,groupid,slot_selection,line_index_base);
		}
	}
	return WUFE_OK;
}



WP_status WTUFE_Reconfig_Fr_Channel(WP_U32 card, WP_U32 trunk,WP_U32 tsgroup,WP_U32 dlci,STRU_SET_INITAIL_CONFIG *p2)
{
	STRU_VPORT_INFO  *VportInfo;
	WP_U32 port_src;
	WP_ch_fr fr_ch_config;
	FR_CHANNEL_INFO *ch_info;
	WP_handle	   status;
	WP_boolean ch_disabled_state = WP_FALSE;
	WP_U32 i = 0;
	WP_tx_binding tx_binding;
    STRU_SET_INITAIL_CONFIG *initial_cfg = npu_get_initial_cfg_info();
	memset((void*)&fr_ch_config,0,sizeof(fr_ch_config));
	port_src = np_fraddr_pack(card,trunk,tsgroup,dlci);
	VportInfo = npu_get_vport_info(port_src);

	if(NULL != VportInfo)
	{
	    if(UNUSED == VportInfo->UsedFlag)
	    {
            Fprintf("WTUFE_Reconfig_Fr_Channel vport not used! vport = 0x%x\n",port_src);
			return 0;
		}
		ch_info = &(VportInfo->ch_info);

		/*QOS enable*/
#if START_SCHEDULER
		{
			WP_pkt_shaping_cir_eir fr_shaping_param;
			memset((void*)&fr_shaping_param,0,sizeof(fr_shaping_param));

			fr_shaping_param.cir = p2->stru_stm1_cfg_info[ch_info->card-1].trunk_info[ch_info->trunk-1].group_info[ch_info->group].PVCQos[ch_info->dlci].cir;
			initial_cfg->stru_stm1_cfg_info[ch_info->card-1].trunk_info[ch_info->trunk-1].group_info[ch_info->group].PVCQos[ch_info->dlci].cir = fr_shaping_param.cir;

			if(WT_MIN_CH_RATE > fr_shaping_param.cir)
			{
				Fprintf("Reconfig fr_iw_channel fr_shaping_param.cir rate %d is low than %d set is to ",fr_shaping_param.cir,WT_MIN_CH_RATE);
				fr_shaping_param.cir = WT_MIN_CH_RATE ;
				Fprintf("%d\n",fr_shaping_param.cir);
			}
			if(WT_MAX_CH_RATE < fr_shaping_param.cir)
			{
				Fprintf("Reconfig fr_iw_channel fr_shaping_param.cir rate %d is big than %d set it to ",fr_shaping_param.cir,WT_MAX_CH_RATE);
				fr_shaping_param.cir = WT_MAX_CH_RATE ;
				Fprintf("%d\n",fr_shaping_param.cir);
			}
			fr_shaping_param.cir_polling = fr_shaping_param.cir;
			fr_shaping_param.cbs = p2->stru_stm1_cfg_info[ch_info->card-1].trunk_info[ch_info->trunk-1].group_info[ch_info->group].PVCQos[ch_info->dlci].bc;
			fr_shaping_param.ebs = p2->stru_stm1_cfg_info[ch_info->card-1].trunk_info[ch_info->trunk-1].group_info[ch_info->group].PVCQos[ch_info->dlci].be;
            //Fprintf("fr_shaping_param.cbs = %d,fr_shaping_param.ebs = %d\n",fr_shaping_param.cbs,fr_shaping_param.ebs);
			initial_cfg->stru_stm1_cfg_info[ch_info->card-1].trunk_info[ch_info->trunk-1].group_info[ch_info->group].PVCQos[ch_info->dlci].bc = fr_shaping_param.cbs;
			initial_cfg->stru_stm1_cfg_info[ch_info->card-1].trunk_info[ch_info->trunk-1].group_info[ch_info->group].PVCQos[ch_info->dlci].be = fr_shaping_param.ebs;
			fr_shaping_param.eir_polling = WP_SHAPING_POLL_AUTOD;
			fr_shaping_param.eir = fr_shaping_param.cir;
			fr_shaping_param.eir_tq = 2;
			fr_shaping_param.eir_cwid = WP_CW_ID_A;

			fr_ch_config.tx_shaping_type = WP_PKT_SHAPING_CIR_EIR;
			fr_ch_config.tx_shaping_params = &fr_shaping_param;
			fr_ch_config.tx_tq = 1;

		}
#endif

		if(ch_info->tx_iw_handle!= 0)
		{

#if 0
			status = WP_ChannelDisable(ch_info->tx_iw_handle);
			if ( strcmp("WP_ERR_CH_ALREADY_DISABLED",  WP_error_name[WP_ERROR(status)]) != 0)
			{
				terminate_on_error(status, "WP_ChannelDisable() tx_iw_handle");
			}
            do
            {
            	(void)WP_ChannelStatus(ch_info->tx_iw_handle, WP_CH_STATUS_DISABLED, &ch_disabled_state);
				WP_Delay(1000);
				i ++;
				if(100 == i)
				{
                    break;
				}
			}while(WP_FALSE == ch_disabled_state);
#endif
			ch_disabled_state = WP_FALSE;
			(void)WP_ChannelStatus(ch_info->tx_iw_handle, WP_CH_STATUS_DISABLED, &ch_disabled_state);
			if(WP_FALSE == ch_disabled_state)
			{
			    memset(&tx_binding, 0, sizeof(WP_tx_binding));
				tx_binding.maxt = 0;
			    tx_binding.dci_mode = WP_IW_DYN_CH_INSERT_DISABLE;
			    status = WP_IwTxBindingModify(ch_info->tx_iw_handle,
                                              WP_IW_TX_BIND_MOD_MAXT | WP_IW_TX_BIND_MOD_DCI,
                                              WP_IW_TX_BINDING,
                                              &tx_binding);
			    terminate_on_error(status, "WTUFE_Reconfig_Fr_Channel WP_IwTxBindingModify disable");
			    WP_Delay(50);

				i = 0;
				do
			    {
			    	status = WP_ChannelDisable(ch_info->tx_iw_handle);
					if ( strcmp("WP_ERR_CH_ALREADY_DISABLED",  WP_error_name[WP_ERROR(status)]) != 0)
					{
						terminate_on_error(status, "WP_ChannelDisable() tx_iw_handle");
					}

			    	(void)WP_ChannelStatus(ch_info->tx_iw_handle, WP_CH_STATUS_DISABLED, &ch_disabled_state);
					WP_Delay(50);
					i ++;
					if(1000 == i)
					{
			            break;
					}
				}while(WP_FALSE == ch_disabled_state);
			}
			if(ch_disabled_state == WP_FALSE)
			{
				Fprintf("WTUFE_Reconfig_Fr_Channel not Complete!\n");
				return WUFE_OK;/*just return ,don't change any parameters,otherwise WP_ChannelModify will cause coredump*/
			}
			status = WP_ChannelModify(ch_info->tx_iw_handle,WP_CH_MOD_FR_TX_TRAFFIC,&fr_ch_config);
			terminate_on_error(status, "WP_ChannelModify() tx_iw_handle");

#if 0
			status = WP_ChannelEnable(ch_info->tx_iw_handle);
			if ( strcmp("WP_ERR_CH_ALREADY_ENABLED",  WP_error_name[WP_ERROR(status)]) != 0)
			{
				terminate_on_error(status, "WP_ChannelEnable() tx_iw_handle");
			}

			WP_Delay(500);
#endif
            /*enalbe it*/
			tx_binding.maxt = NPU_FR_TX_MAXT;
			tx_binding.dci_mode = WP_IW_DYN_CH_INSERT_ENABLE;
			status = WP_IwTxBindingModify(ch_info->tx_iw_handle,
                                          WP_IW_TX_BIND_MOD_MAXT | WP_IW_TX_BIND_MOD_DCI,
                                          WP_IW_TX_BINDING,
                                          &tx_binding);

			status = WP_ChannelEnable(ch_info->tx_iw_handle);
			if ( strcmp("WP_ERR_CH_ALREADY_ENABLED",  WP_error_name[WP_ERROR(status)]) != 0)
			{
				terminate_on_error(status, "WP_ChannelEnable() np_enable_fr_iw_tx channel");
			}
		}
		Fprintf("WTUFE_Reconfig_Fr_Channel Complete!\n");
	}
	return WUFE_OK;
}

WP_status WTUFE_Reconfig_Stm1_All(STRU_SET_INITAIL_CONFIG *p1, STRU_SET_INITAIL_CONFIG *p2)
{
	WP_U32 trunkid,groupid,i,line_index_base,line_index_counts,j,k;
	STRU_STM1_RECFG reconfig_info;
	WUFE_status status;
    STRU_SET_INITAIL_CONFIG *initial_cfg = npu_get_initial_cfg_info();
    /*clear the flags when enter this entry*/
    // memset(reconfigStm1_createE1_flag, 0, RECONFIG_TRUNK_MAX);

	for(i = 0; i < SPP_MAX_STM1_PORT_NUM; i ++)
	{
	    if(NPU_VALID == p2->stru_stm1_cfg_info[i].stm1Valid)
	    {
	        if(NPU_VALID != initial_cfg->stru_stm1_cfg_info[i].stm1Valid)
	        {
				initial_cfg->stru_stm1_cfg_info[i].stm1Valid = NPU_VALID;
				initial_cfg->stm1_num ++;
			}
			initial_cfg->stru_stm1_cfg_info[i].E1T1type = p2->stru_stm1_cfg_info[i].E1T1type;
			if(p2->stru_stm1_cfg_info[i].E1T1type == NPU_MODE_E1)
			{
				line_index_base = i*N_UFE_MAX_LINES_E1/2;
				line_index_counts = (1+i)*N_UFE_MAX_LINES_E1/2;
			}
			else
			{
				line_index_base = i*N_UFE_MAX_LINES_T1/2;
				line_index_counts = (1+i)*N_UFE_MAX_LINES_T1/2;
			}
#if 1
			for(trunkid = line_index_base; trunkid < line_index_counts; trunkid ++)
			{
				if(NPU_VALID == p2->stru_stm1_cfg_info[i].trunk_info[trunkid-line_index_base].trunkValid)
				{
				    initial_cfg->stru_stm1_cfg_info[i].trunk_info[trunkid-line_index_base].trunkValid = NPU_VALID;
					memset(&reconfig_info,0,sizeof(STRU_STM1_RECFG));
					reconfig_info.trunkId = trunkid;

					for(groupid = 0; groupid < 32; groupid ++)
					{
						if(NPU_VALID == p2->stru_stm1_cfg_info[i].trunk_info[trunkid-line_index_base].group_info[groupid].groupValid)
						{
							for(k = 0; k < 1025; k++)
							{
								if(0!=(p2->stru_stm1_cfg_info[i].trunk_info[trunkid-line_index_base].group_info[groupid].PVCQos[k].cir))
								{
								    /*only changed channel do reconfiguration*/
								    if((p1->stru_stm1_cfg_info[i].trunk_info[trunkid-line_index_base].group_info[groupid].PVCQos[k].cir == p2->stru_stm1_cfg_info[i].trunk_info[trunkid-line_index_base].group_info[groupid].PVCQos[k].cir)
                                       &&(p1->stru_stm1_cfg_info[i].trunk_info[trunkid-line_index_base].group_info[groupid].PVCQos[k].bc == p2->stru_stm1_cfg_info[i].trunk_info[trunkid-line_index_base].group_info[groupid].PVCQos[k].bc)
                                       &&(p1->stru_stm1_cfg_info[i].trunk_info[trunkid-line_index_base].group_info[groupid].PVCQos[k].be == p2->stru_stm1_cfg_info[i].trunk_info[trunkid-line_index_base].group_info[groupid].PVCQos[k].be)
                                        )
                                    {
										continue;
                                    }
									status = WTUFE_Reconfig_Fr_Channel(i+1,(trunkid-line_index_base)+1,groupid,k,p2);
									if(WUFE_OK != status)
									{
										return status;
									}
								}
							}
						    initial_cfg->stru_stm1_cfg_info[i].trunk_info[trunkid-line_index_base].group_info[groupid].groupValid = NPU_VALID;
							if(((p1->stru_stm1_cfg_info[i].trunk_info[trunkid-line_index_base].group_info[groupid].crc_type) == (p2->stru_stm1_cfg_info[i].trunk_info[trunkid-line_index_base].group_info[groupid].crc_type))
                               &&((p1->stru_stm1_cfg_info[i].trunk_info[trunkid-line_index_base].group_info[groupid].max_frame_size) == (p2->stru_stm1_cfg_info[i].trunk_info[trunkid-line_index_base].group_info[groupid].max_frame_size))
                               &&((p1->stru_stm1_cfg_info[i].trunk_info[trunkid-line_index_base].group_info[groupid].slot_selection) == (p2->stru_stm1_cfg_info[i].trunk_info[trunkid-line_index_base].group_info[groupid].slot_selection)))
							{
								continue;
							}
							else
							{
								reconfig_info.group_info[groupid].groupValid = NPU_VALID;
								reconfig_info.group_info[groupid].crc_type = p2->stru_stm1_cfg_info[i].trunk_info[trunkid-line_index_base].group_info[groupid].crc_type;
								reconfig_info.group_info[groupid].max_frame_size = p2->stru_stm1_cfg_info[i].trunk_info[trunkid-line_index_base].group_info[groupid].max_frame_size;
								reconfig_info.group_info[groupid].slot_selection = p2->stru_stm1_cfg_info[i].trunk_info[trunkid-line_index_base].group_info[groupid].slot_selection;

								Fprintf("###reconfig_info slot_selection is 0x%x\n",reconfig_info.group_info[groupid].slot_selection);
								Fprintf("stm1id = %d,trunkid = %d,line_index_base = %d,groupid = %d \n",i,trunkid,line_index_base,groupid);
#if 1
								status = WTUFE_Reconfig_Stm1_inLoop(i, trunkid, groupid, line_index_base,&reconfig_info,p2);
								if(WUFE_OK != status)
								{
									return status;
								}
#endif
							}
						}
						else
						{
	                        initial_cfg->stru_stm1_cfg_info[i].trunk_info[trunkid-line_index_base].group_info[groupid].groupValid = NPU_INVALID;
						}
					}
#if 0
					status = WTUFE_Reconfig_Stm1(i,line_index_base,&reconfig_info,p2);
					if(WUFE_OK != status)
					{
						return status;
					}
#endif

					/*reconfig clock mode*/
					if((p1->stru_stm1_cfg_info[i].trunk_info[trunkid-line_index_base].clock_mode) != (p2->stru_stm1_cfg_info[i].trunk_info[trunkid-line_index_base].clock_mode))
					{
                        WP_U32 clock_mode;
                        if(p2->stru_stm1_cfg_info[i].trunk_info[trunkid-line_index_base].clock_mode == NPU_E1T1_CLOCK_MASTER)
                        {
                            clock_mode = UFE_CLOCK_MASTER_MODE;
                        }
                        else
                        {
                            clock_mode = UFE_CLOCK_SLAVE_MODE;
                        }
					    npu_ufe_reconfig_clock_mode(trunkid,p2->stru_stm1_cfg_info[i].E1T1type,clock_mode);
					}
				}
				else
				{
    	            initial_cfg->stru_stm1_cfg_info[i].trunk_info[trunkid-line_index_base].trunkValid = NPU_INVALID;
				}
			}
#endif
		}
		else
		{
			if(0 != initial_cfg->stm1_num && NPU_VALID == initial_cfg->stru_stm1_cfg_info[i].stm1Valid)
			{
				(initial_cfg->stm1_num)--;
			}
			initial_cfg->stru_stm1_cfg_info[i].stm1Valid = NPU_INVALID;
			WPX_PortLedSet(0, WPX_PORT_LED_OFF, i*3);
#if 0
		    for(trunkid = line_index_base; trunkid < line_index_counts; trunkid ++)
		    {
				for(groupid = 0;groupid < 32; groupid ++)
				{
					if(ufe_phy_handle[trunkid][groupid] != 0)
					{

					    (void)WUFE_PhyDisable(ufe_phy_handle[trunkid][groupid], WUFE_FULL_DUPLEX);
						for(j=0; j<100000;j++)
						{
							status = WUFE_PhyDelete(ufe_phy_handle[trunkid][groupid]);
							if(status == WUFE_OK)
                                break;
							if (status != WUFE_ERR_PHY_FIFO_NOT_EMPTY)
                                return status;
							WP_Delay(10);
						}
					}

					if(ufe_device_handle[trunkid][groupid] != 0)
					{

					    (void)WP_DeviceDisable(ufe_device_handle[trunkid][groupid], WP_DIRECTION_DUPLEX);
						(void)WP_DeviceDelete(ufe_device_handle[trunkid][groupid]);
					}
				}
			}
            continue;
#endif
		}
	}
	return WUFE_OK;
}

void np_display_ufe_system(void)
{
	WP_handle	   status;
	status = WUFE_SystemDisplay(0);
	if (status != WUFE_OK)
	{
		Fprintf("UFE system display error\n");
		Fprintf("Error type - %s\n", WP_error_name[WP_ERROR(status)]);
	}
	WP_CHAR pHardwareInformation[100];

	WPX_FRMR_STATUS_DEV_VERSION_GetHardwareInfo(0,&pHardwareInformation);

	printf("UFE version %s \n",pHardwareInformation);
}
int loopbkflag[N_UFE_MAX_LINES_T1] ={0};
void np_display_ufe_line(WP_U32 lineIndex)
{
	WP_handle	   status;
	if(lineIndex > N_UFE_MAX_LINES_T1 -1)return;

    if(ufe_line_handle[lineIndex]!=0)
    {
    	WP_U16 stm1;
    	WP_U16 client_port;
    	STRU_SET_INITAIL_CONFIG *initial_cfg = npu_get_initial_cfg_info();
    	stm1 = lineIndex/SPP_MAX_TRUNK_NUM;
    	WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE line_params;
        if(initial_cfg->stru_stm1_cfg_info[stm1].E1T1type == NPU_MODE_E1)
        {
    		client_port = WTI_FlexmuxClientPortIndexGet(E1MapTable[lineIndex], WUFE_SDH_TYPE_E1);
    		line_params.u.SDH.stm4 = lineIndex / 252;
    		line_params.u.SDH.stm1 = (lineIndex % 252) / 63;
    		line_params.u.SDH.stm0 = (lineIndex % 63) / 21;
    		line_params.u.SDH.tug2 = (lineIndex % 21) / 3;
    		line_params.u.SDH.tu   = ((lineIndex % 21) % 3);
        }
    	else
    	{
    		client_port = WTI_FlexmuxClientPortIndexGet(E1MapTable[lineIndex], WUFE_SDH_TYPE_T1);
    		line_params.u.SDH.stm4 = lineIndex / 336;
    		line_params.u.SDH.stm1 = (lineIndex % 336) / 84;
    		line_params.u.SDH.stm0 = (lineIndex % 84) / 28;
    		line_params.u.SDH.tug2 = (lineIndex % 28) / 4;
    		line_params.u.SDH.tu   = (lineIndex % 4);
    	}
    	if(loopbkflag[lineIndex] == 0)
    	{
       		status = WPX_FRMR_SONET_SDH_DIAG_LpbkDisable(0,0,&line_params);
       		terminate_on_error(status, "WPX_FRMR_SONET_SDH_DIAG_LpbkDisable ");

    		status = WPX_FRMR_SONET_SDH_PORT_DIAG_LpbkEnableShallowLine(0,0,client_port);
    		//status = WUFE_LineLoopbackConfigure(ufe_line_handle[lineIndex], WUFE_LINE_LOOPBACK_TYPE_LINE);
    		terminate_on_error(status, "WPX_FRMR_SONET_SDH_PORT_DIAG_LpbkEnableShallowLine ");


    		status = WPX_FRMR_SOCKET_PDH_DIAG_LpbkDisableDeepLine(0,0,client_port);
    		terminate_on_error(status, "WPX_FRMR_SOCKET_PDH_DIAG_LpbkDisableDeepLine ");

    		loopbkflag[lineIndex]++;
    		Fprintf("line %d WPX_FRMR_SONET_SDH_PORT_DIAG_LpbkEnableShallowLine !\n",lineIndex);
    	}
    	else if(loopbkflag[lineIndex] == 1)
    	{
    		status = WPX_FRMR_SONET_SDH_DIAG_LpbkEnable(0,0,&line_params);
    		terminate_on_error(status, "WPX_FRMR_SONET_SDH_DIAG_LpbkEnable ");

    		status = WPX_FRMR_SONET_SDH_PORT_DIAG_LpbkDisableShallowLine(0,0,client_port);
    		terminate_on_error(status, "WPX_FRMR_SONET_SDH_PORT_DIAG_LpbkDisableShallowLine ");

    		status = WPX_FRMR_SOCKET_PDH_DIAG_LpbkDisableDeepLine(0,0,client_port);
    		terminate_on_error(status, "WPX_FRMR_SOCKET_PDH_DIAG_LpbkDisableDeepLine ");
    		loopbkflag[lineIndex]++;
    		Fprintf("line %d WPX_FRMR_SONET_SDH_DIAG_LpbkEnable !\n",lineIndex);
    	}
    	else if(loopbkflag[lineIndex] == 2)
    	{
    		status = WPX_FRMR_SONET_SDH_DIAG_LpbkDisable(0,0,&line_params);
    		terminate_on_error(status, "WPX_FRMR_SONET_SDH_DIAG_LpbkDisable ");

    		status = WPX_FRMR_SONET_SDH_PORT_DIAG_LpbkDisableShallowLine(0,0,client_port);
    		terminate_on_error(status, "WPX_FRMR_SONET_SDH_PORT_DIAG_LpbkDisableShallowLine ");

    		status = WPX_FRMR_SOCKET_PDH_DIAG_LpbkEnableDeepLine(0,0,client_port);
    		terminate_on_error(status, "WPX_FRMR_SOCKET_PDH_DIAG_LpbkEnableDeepLine ");
    		loopbkflag[lineIndex]++;
    		Fprintf("line %d WPX_FRMR_SOCKET_PDH_DIAG_LpbkEnableDeepLine !\n",lineIndex);
    	}
    	else
    	{
    		WPX_FRMR_SONET_SDH_PORT_DIAG_LpbkDisableShallowLine(0,0,client_port);
    		terminate_on_error(status, "WPX_FRMR_SONET_SDH_PORT_DIAG_LpbkDisableShallowLine");

    		status = WPX_FRMR_SONET_SDH_DIAG_LpbkDisable(0,0,&line_params);
    		terminate_on_error(status, "WPX_FRMR_SONET_SDH_DIAG_LpbkDisable ");

    		status = WPX_FRMR_SOCKET_PDH_DIAG_LpbkDisableDeepLine(0,0,client_port);
    		terminate_on_error(status, "WPX_FRMR_SOCKET_PDH_DIAG_LpbkDisableDeepLine ");

    		loopbkflag[lineIndex] = 0;
    		Fprintf("line %d cancel loopback !\n",lineIndex);
    	}
    	status = WUFE_LineDisplay(ufe_line_handle[lineIndex]);
    }
	else
	{
        Fprintf("np_display_ufe_line invalid lineIndex %d\n",lineIndex);
	}
	if (status != WUFE_OK)
	{
		Fprintf("UFE line display error\n");
		Fprintf("Error type - %s\n", WP_error_name[WP_ERROR(status)]);
	}
}
void np_dispaly_system_flow_agg(void)
{
    NP_SYSTEM_INFO * the_system = np_get_system_info();
    Fprintf("\n agg_enet_default_host_handle info\n");
	StatIwFlowAggregation(the_system->agg_enet_default_host_handle);
    Fprintf("\n agg_host_arp_agg info\n");
	StatIwFlowAggregation(the_system->agg_host_arp_agg);
    Fprintf("\n agg_enet_PrioRx0_host_handle info\n");
	StatIwFlowAggregation(the_system->agg_enet_PrioRx0_host_handle);
    Fprintf("\n agg_FR_PrioRx0_host_handle info\n");
	StatIwFlowAggregation(the_system->agg_FR_PrioRx0_host_handle);
	Fprintf("###\n enet_iw_flow_defrag_aggregation info###\n");
	StatIwFlowAggregation(enet_iw_flow_defrag_aggregation);
	Fprintf("####\n ipdefrag_agg route type agg info####\n");
	StatIwFlowAggregation(ipdefrag_agg);
    Fprintf("\n agg_enet_1 tx to enet1 channel info\n");
	StatIwFlowAggregation(agg_enet_1);

    Fprintf("\n the_system->agg_enet2_to_enet1 info\n");
	StatIwFlowAggregation(the_system->agg_enet2_to_enet1);

    Fprintf("\n the_system->agg_dl_ip_to_enet2_handle info\n");
	StatIwFlowAggregation(the_system->agg_dl_ip_to_enet2_handle);
#if BFD_OVERETH
	Fprintf("\n the_system->agg_bfd_tx_handle info\n");
	StatIwFlowAggregation(the_system->agg_bfd_tx_handle);
#endif
	Fprintf("\n the_system->agg_mac_filter_handle info\n");
	StatIwFlowAggregation(the_system->agg_mac_filter_handle);
	Fprintf("\n the_system->agg_mac_filter_from_eth2_handle info\n");
	StatIwFlowAggregation(the_system->agg_mac_filter_from_eth2_handle);
}

void np_display_flow_agg(WP_U32 vport)
{
    FR_CHANNEL_INFO * ch_info;
    STRU_VPORT_INFO  *VportInfo = npu_get_vport_info(vport);
    if(VportInfo ==  NULL)return;
	STRU_SET_INITAIL_CONFIG *initial_cfg = npu_get_initial_cfg_info();
	if(initial_cfg->OpterateMode == NPU_MODE_FR)
	{
		ch_info = &VportInfo->ch_info;
		Fprintf("\n ch_info->agg_fr_rx_host_flow\n");
		StatIwFlowAggregation(ch_info->agg_fr_rx_host_flow);
		Fprintf("\n ch_info->agg_downlink_tx2fr_device\n");
		StatIwFlowAggregation(ch_info->agg_downlink_tx2fr_device);
		Fprintf("\n ch_info->agg_tx2Enet1\n");
		StatIwFlowAggregation(ch_info->agg_tx2Enet1);

		Fprintf("\n ch_info->agg_fr_to_enet2\n");
		StatIwFlowAggregation(ch_info->agg_fr_to_enet2);
		Fprintf("\n ch_info->agg_enet2_to_fr\n");
		StatIwFlowAggregation(ch_info->agg_enet2_to_fr);
	}
	else
	{

		Fprintf("####\n VportInfo->agg_ip2iptx2host info####\n");
		StatIwFlowAggregation(VportInfo->agg_ip2iptx2host);

		Fprintf("####\n VportInfo->agg_ul_ip2iptx2Enet1 info####\n");
		StatIwFlowAggregation(VportInfo->agg_ul_ip2iptx2Enet1);

		Fprintf("####\n VportInfo->agg_dl_ip2iptx2Enet1 info####\n");
		StatIwFlowAggregation(VportInfo->agg_dl_ip2iptx2Enet1);

		Fprintf("####\n VportInfo->agg_ul_ip_to_enet2_handle info####\n");
		StatIwFlowAggregation(VportInfo->agg_ul_ip_to_enet2_handle);

	}

}

void np_display_ufe_phy(WP_U32 vport)
{
	WP_handle	   status;
    FR_CHANNEL_INFO * ch_info;
    STRU_VPORT_INFO  *VportInfo = npu_get_vport_info(vport);
	if(VportInfo ==  NULL)return;
	ch_info = &VportInfo->ch_info;

	status = WUFE_PhyDisplay(ch_info->phy_handle);
	if (status != WUFE_OK)
	{
		Fprintf("UFE phy display error\n");
		Fprintf("Error type - %s\n", WP_error_name[WP_ERROR(status)]);
	}
}



void np_frIwSystemCreate(NP_SYSTEM_INFO *np_system)
{

	WP_iw_sys_fr_switching fr_sw_system_cfg;
    fr_sw_system_cfg.max_flows = N_FR_MAX_IW_RX_CHANNEL;
	fr_sw_system_cfg.buffer_gap = 64;/*how to judge this value ?*/
	np_system->system_fr_handle = WP_IwSystemCreate(WP_WINPATH(0),
                                                    WP_IW_FR_SWITCHING_MODE,
                                                    &fr_sw_system_cfg);
	terminate_on_error(np_system->system_fr_handle, "WP_IwSystemCreate - WP_IW_FR_SWITCHING_MODE");


}


void displayUfeEvents(void)
{
    WUFE_events events;
    WP_status   status;
    WP_U16      event;
    WP_U32      i;

    WUFE_UfeReadEvents(0, &events);
    terminate_on_error(status, "WUFE_UfeReadEvents()");

    Fprintf("\nUFE Events:\n");
    Fprintf("-----------\n");

    // EMPHY
    event = events.emphy_events[0].event_reg;
    if(event & WUFE_EVENT_EMPHY) {
        if (event & WUFE_EVENT_EMPHY_INGRESS_BUS)
            Fprintf("\tEMPHY: WUFE_EVENT_EMPHY_INGRESS_BUS\n");
        if (event & WUFE_EVENT_EMPHY_INGRESS_MAX_BUFF_SIZE)
            Fprintf("\tEMPHY: WUFE_EVENT_EMPHY_INGRESS_MAX_BUFF_SIZE\n");
        if (event & WUFE_EVENT_EMPHY_INGRESS_IF)
            Fprintf("\tEMPHY: WUFE_EVENT_EMPHY_INGRESS_IF\n");
        if (event & WUFE_EVENT_EMPHY_EGRESS_PHY_NOT_VALID)
            Fprintf("\tEMPHY: WUFE_EVENT_EMPHY_EGRESS_PHY_NOT_VALID (%0x%x)\n", events.emphy_events[0].phy_not_valid_handle);
        if (event & WUFE_EVENT_EMPHY_EGRESS_BUS)
            Fprintf("\tEMPHY: WUFE_EVENT_EMPHY_EGRESS_BUS\n");
        if (event & WUFE_EVENT_EMPHY_EGRESS_IF )
            Fprintf("\tEMPHY: WUFE_EVENT_EMPHY_EGRESS_IF\n");
        if (event &WUFE_EVENT_EMPHY_EGRESS_PARITY )
            Fprintf("\tEMPHY: WUFE_EVENT_EMPHY_EGRESS_PARITY\n");

    } else {
        Fprintf("\tEMPHY: No events\n");
    }

    // TDM
    event = events.tdm_events.event_reg;
    if (event & WUFE_EVENT_TDM)	{
        if (event & WUFE_EVENT_TDM_RX_PARITY)
            Fprintf("\tTDM  : WUFE_EVENT_TDM_RX_PARITY\n");
        if (event & WUFE_EVENT_TDM_TX_PARITY)
            Fprintf("\tTDM  : WUFE_EVENT_TDM_TX_PARITY\n");
    } else {
        Fprintf("\tTDM  : No events\n");
    }

    // FM
    event = events.fm_events.event_reg;
    if (event & WUFE_EVENT_FM) {
        if (event & WUFE_EVENT_FM_RX_DDR_OVERRUN)
            Fprintf("\tFM   : WUFE_EVENT_FM_RX_DDR_OVERRUN\n");
        if (event & WUFE_EVENT_FM_RX_BURST_OVERRUN)
            Fprintf("\tFM   : WUFE_EVENT_FM_RX_BURST_OVERRUN\n");
        if (event & WUFE_EVENT_FM_TX_DDR_UNDERRUN)
            Fprintf("\tFM   : WUFE_EVENT_FM_TX_DDR_UNDERRUN\n");
        if (event & WUFE_EVENT_FM_TX_BURST_UNDERRUN)
            Fprintf("\tFM   : WUFE_EVENT_FM_TX_BURST_UNDERRUN\n");
        if (event & WUFE_EVENT_FM_RX_ECC_ONE_BIT)
            Fprintf("\tFM   : WUFE_EVENT_FM_RX_ECC_ONE_BIT\n");
        if (event & WUFE_EVENT_FM_TX_ECC_ONE_BIT)
            Fprintf("\tFM   : WUFE_EVENT_FM_TX_ECC_ONE_BIT\n");
        if (event & WUFE_EVENT_FM_TX_READ_FIFO_OVERRUN)
            Fprintf("\tFM   : WUFE_EVENT_FM_TX_READ_FIFO_OVERRUN\n");
    } else {
        Fprintf("\tFM   : No events\n");
    }

    // MACHINE
    event = events.machine_events.rx_event_reg;
    if (event & WUFE_EVENT_MACHINE_RX) {
        if (event & WUFE_EVENT_MACHINE_CES_RX_LOSS)
            Fprintf("\tRX MACHINE : WUFE_EVENT_MACHINE_CES_RX_LOSS\n");
        if (event & WUFE_EVENT_MACHINE_CES_RX_MF_LOSS)
            Fprintf("\tRX MACHINE : WUFE_EVENT_MACHINE_CES_RX_MF_LOSS\n");
        if (event & WUFE_EVENT_MACHINE_ATM_RX_LCD)
            Fprintf("\tRX MACHINE : WUFE_EVENT_MACHINE_ATM_RX_LCD\n");
        if (event & WUFE_EVENT_MACHINE_HDLC_RX_ABORT)
            Fprintf("\tRX MACHINE : WUFE_EVENT_MACHINE_HDLC_RX_ABORT\n");
        if (event & WUFE_EVENT_MACHINE_HDLC_RX_CRC_ERR)
            Fprintf("\tRX MACHINE : WUFE_EVENT_MACHINE_HDLC_RX_CRC_ERR\n");
        if (event & WUFE_EVENT_MACHINE_HDLC_RX_NON_OCTET)
            Fprintf("\tRX MACHINE : WUFE_EVENT_MACHINE_HDLC_RX_NON_OCTET\n");
        if (event & WUFE_EVENT_MACHINE_POS_RX_ABORT)
            Fprintf("\tRX MACHINE : WUFE_EVENT_MACHINE_POS_RX_ABORT\n");
        if (event & WUFE_EVENT_MACHINE_POS_CRC_ERR)
            Fprintf("\tRX MACHINE : WUFE_EVENT_MACHINE_POS_CRC_ERR\n");
        if (event & WUFE_EVENT_MACHINE_RX_OVERRUN_BURST)
            Fprintf("\tRX MACHINE : WUFE_EVENT_MACHINE_RX_OVERRUN_BURST\n");
        if (event & WUFE_EVENT_MACHINE_RX_OVERRUN_DDR)
            Fprintf("\tRX MACHINE : WUFE_EVENT_MACHINE_RX_OVERRUN_DDR\n");
    } else {
        Fprintf("\tRX MACHINE : No events\n");
    }

    event = events.machine_events.tx_event_reg;

    if (event & WUFE_EVENT_MACHINE_TX) {
        if (event & WUFE_EVENT_MACHINE_CES_TX_LOSS)
            Fprintf("\tTX MACHINE : WUFE_EVENT_MACHINE_CES_TX_LOSS\n");
        if (event & WUFE_EVENT_MACHINE_TX_UNDERRUN_BURST)
            Fprintf("\tTX MACHINE : WUFE_EVENT_MACHINE_TX_UNDERRUN_BURST\n");
        if (event & WUFE_EVENT_MACHINE_TX_UNDERRUN_DDR)
            Fprintf("\tTX MACHINE : WUFE_EVENT_MACHINE_TX_UNDERRUN_DDR\n");
    } else {
        Fprintf("\tTX MACHINE : No events\n");
    }

    // BS
    event = events.bs_events.pll_lost_lock_event_reg;
    if(event & 0x400) Fprintf("\tBS LOCK : TX EMPHY PLL LOST CLOCK\n");
    if(event & 0x200) Fprintf("\tBS LOCK : RX EMPHY PLL LOST CLOCK\n");
    if(event & 0x080) Fprintf("\tBS LOCK : DDR1 PLL LOST CLOCK\n");
    if(event & 0x040) Fprintf("\tBS LOCK : DDR0 PLL LOST CLOCK\n");
    if(event & 0x020) Fprintf("\tBS LOCK : DIFF PLL LOST CLOCK\n");
    if(event & 0x010) Fprintf("\tBS LOCK : ADAPT PLL LOST CLOCK\n");
    if(event & 0x008) Fprintf("\tBS LOCK : EXT PLL LOST CLOCK\n");
    if(event & 0x004) Fprintf("\tBS LOCK : FRMR PLL LOST CLOCK\n");
    if(event & 0x003) Fprintf("\tBS LOCK : DDR PLL LOST CLOCK\n");
    if(event & 0x002) Fprintf("\tBS LOCK : SYSCLK PLL LOST CLOCK\n");

    event = events.bs_events.absent_event_reg;
    for(i=0; i<4; i++) {
        if(event & (1 << i)) {
            Fprintf("\tBS SFP : SFP absent\n");
        } else {
            Fprintf("\tBS SFP : SFP OK\n");
        }
    }

    event = events.bs_events.los_event_reg;
    for(i=0; i<4; i++) {
        if(event & (1 << i)) {
            Fprintf("\tBS LOS : Loss of signal\n");
        } else {
            Fprintf("\tBS LOS : Signal OK\n");
        }
    }



    return;
}

#if 0
void NP_Vt100CrsrDown(WP_U32 down)
{
    Fprintf("\033[%dB",down);
}


void NP_Vt100CrsrUp(WP_U32 up)
{
    Fprintf("\033[%dA",up);
}

void NP_DisplayWingineUtilization ( WP_U32 *wg_util, WP_U32 total_util)
{
    static WP_U32 first_print_screen;

    if (!first_print_screen)
    {
        Fprintf("    *****************************************\n");
        Fprintf("              Average utilization :         \n");
        Fprintf("              ---------------------         \n");
        Fprintf("     WG1:%02d%%  WG2:%02d%%  WG3:%02d%%  WG4:%02d%%     \n",
                wg_util[WP_WINGINE1],
                wg_util[WP_WINGINE2],
                wg_util[WP_WINGINE3],
                wg_util[WP_WINGINE4]);
        Fprintf("                                            \n");
        Fprintf("     Total Utilization : %d%%                \n", total_util);
        Fprintf("                                            \n");
        Fprintf("    ****************************************\n");
        Fprintf("\n");

        first_print_screen = WP_TRUE;
    }
    else
    {
        NP_Vt100CrsrUp(4);
        Fprintf("     Total Utilization : %d%%                \n", total_util);
        NP_Vt100CrsrUp(3);
        Fprintf("     WG1:%02d%%  WG2:%02d%%  WG3:%02d%%  WG4:%02d%%     \n",
                wg_util[WP_WINGINE1],
                wg_util[WP_WINGINE2],
                wg_util[WP_WINGINE3],
                wg_util[WP_WINGINE4]);
        NP_Vt100CrsrDown(6);
    }
}
#endif

void NP_HwWinGineUtil(WPU_util_info* utilization, WP_U32  *wcom_av )
{
    WP_U8 winpath_wingines;
    (void)WPUI_WinPathGetWinginesOption(0,&winpath_wingines);
    /*WP_U32  wcom_av=0;*/
    WP_U32 ii, jj = 0;

    /************************************
     * Wingine utilization
     ************************************/
	for(ii = 0; ii < MAX_NUM_OF_WINGINES / 2; ii++)
	{
		if(winpath_wingines & (1 << ii))
		{
			jj++;
			*wcom_av += utilization->wg[ii];
		}
	}
	/* second winfarm */
	for(ii = MAX_NUM_OF_WINGINES / 2; ii < MAX_NUM_OF_WINGINES; ii++)
	{
		if(winpath_wingines & (1 << ii))
		{
			jj++;
			*wcom_av += utilization->wg[ii];
		}
	}
	/* average */
	*wcom_av = *wcom_av / jj;
}

void Np_Get_Utilization(STRU_CPU_STATICS *get_cpu_statics)
{
    WP_U32  wcom_av=0;
    WPU_util_info utilization;
    int i;
    WPU_WinGineUtilGet(0,&utilization);
    NP_HwWinGineUtil(&utilization, &wcom_av);
    for(i = 0;i < 12;i ++)
    {
        get_cpu_statics->WinGinUtili[i] = utilization.wg[i];

    }
    get_cpu_statics->totalUtili = wcom_av;

}

void Np_ShowMemoryAvailable(STRU_MEMORY_STATICS *get_memory_statics)
{
	WP_status	 status;
	WP_U32		bytes;

	STRU_SET_INITAIL_CONFIG *initial_cfg = npu_get_initial_cfg_info();

	get_memory_statics->InterMemTotalByte = initial_cfg->InterMemTotalByte;
	get_memory_statics->PacketMemTotalByte = initial_cfg->PacketMemTotalByte;
	get_memory_statics->ParamMemTotalByte = initial_cfg->ParamMemTotalByte;
	/* get the available memeory of the packet bus */
	status = WP_MemoryAvailable(WP_WINPATH(0), (u32)WP_BUS_PACKET, APP_BANK_PACKET, &bytes);
	get_memory_statics->PacketMemLeftByte = bytes;
	/* check the result */
	terminate_on_error(status, "np_mem_available:packet \n");
	//Fprintf("NP App:	Packet bus bytes left = 0x%x, \n", bytes);

	/* get the available memeory of the parameter bus */
	status = WP_MemoryAvailable(WP_WINPATH(0), (u32)WP_BUS_PARAM, APP_BANK_PARAM, &bytes);
	get_memory_statics->ParamMemLeftByte = bytes;
	/* check the result */
	terminate_on_error(status, "np_mem_available:parameter \n");
	//Fprintf("NP App:	Param bus bytes left = 0x%x\n", bytes);

	/* get the available memeory of the internal bus */
	status = WP_MemoryAvailable(WP_WINPATH(0), (u32)WP_BUS_INTERNAL, APP_BANK_INTERNAL,&bytes);
	get_memory_statics->InterMemLeftByte = bytes;
	/* check the result */
	terminate_on_error(status, "np_mem_available:internal \n");
	//Fprintf("NP App:	Internal bus bytes left = 0x%x\n", bytes);
}


void WT_Np_UfeLineSdhConfig(WT_ufe_line_sdh_params *line_sdh_params,
                            WUFE_line_sdh *line_cfg)
{
    memset(line_cfg, 0, sizeof(WUFE_line_sdh));

    line_cfg->struct_id = WUFE_STRUCT_LINE_SDH;
    if (line_sdh_params->stm4 > 3)
    {
        Fprintf("Invalid stm4 parameter, use 0-3 only \n");
        exit(1);
    }
    else line_cfg->stm4 = line_sdh_params->stm4;

    if (line_sdh_params->stm1 > 3)
    {
        Fprintf("Invalid stm1 parameter, use 0-3 only \n");
        exit(1);
    }
    else line_cfg->stm1 = line_sdh_params->stm1;

    if (line_sdh_params->stm0 > 2)
    {
        Fprintf("Invalid stm0 parameter, use 0-2 only \n");
        exit(1);
    }
    else line_cfg->stm0 = line_sdh_params->stm0;

    if (line_sdh_params->tug2 > 6)
    {
        Fprintf("Invalid tug2 parameter,use 0-6 only \n");
        exit(1);
    }
    else line_cfg->tug2 = line_sdh_params->tug2;

    if (line_sdh_params->tu > 3)
    {
        Fprintf("Invalid tu parameter,use 0-6 only \n");
        exit(1);
    }
    else if ((line_sdh_params->tu == 3)&&
             ((line_sdh_params->transfer_type == WUFE_SDH_TYPE_E1) ||
              (line_sdh_params->transfer_type == WUFE_SDH_TYPE_VC12)))
    {
        Fprintf("Invalid tu parameter, slot 3 not valid for E1 or VC12\n");
        exit(1);
    }
    else line_cfg->tu = line_sdh_params->tu;

    line_cfg->transfer_type = line_sdh_params->transfer_type;
    line_cfg->framed = line_sdh_params->framed;
    //line_cfg->cas_enable = line_sdh_params->cas_enable;
    line_cfg->cas_enable = WUFE_CAS_DISABLE;
    /* Copy clock recovery line parameters */
    if (line_sdh_params->clock_rec_line_params != NULL)
    {
        WUFE_line_clock_rec_params *line_cr_params = line_sdh_params->clock_rec_line_params;
        if (line_cr_params->tx_clk_rec_enable !=0 && line_cr_params->tx_clk_rec_enable != 1)
        {
            Fprintf("Invalid TX Clock Recovery mode\n");
            exit(1);
        }
        if (line_cr_params->clk_rec_physical_clock_enable != 0 && line_cr_params->clk_rec_physical_clock_enable != 1)
        {
            Fprintf("Invalid Physical clock mode\n");
            exit(1);
        }
        if (line_cr_params->clk_rec_physical_clock_id < 0 || line_cr_params->clk_rec_physical_clock_id > 1)
        {
            Fprintf("Invalid Physical clock ID, must be '0' or '1'\n");
            exit(1);
        }
        line_cfg->clock_rec_line_params = line_cr_params;
    }
}
#define HWno
void WT_Np_UfeLineConfigureAndCreateE1(WP_U32 line_index, WP_U16 framing_mode, WP_U8 cas_enable,WP_U32 line_lb_mode,WP_U32 clock_mode)
{
	WT_ufe_line_sdh_params line_params;
	WUFE_line_sdh line_cfg;

	memset(&line_params, 0, sizeof(WT_ufe_line_sdh_params));
#if 0
	line_params.stm4 = line_index / 336;
	line_params.stm1 = (line_index % 336) / 84;
	line_params.stm0 = (line_index % 84) / 28;
	line_params.tug2 = (line_index % 28) / 3;
	line_params.tu = ((line_index % 28) % 3);
#endif

#ifdef HW  /*huawei  TU3 + TU2 *3 + TU12*21*/
	line_params.stm4 = line_index / 252;
	line_params.stm1 = (line_index % 252) / 63;
	line_params.tu =   (line_index % 63) / 21;
	line_params.tug2 = (line_index % 21) / 3;
	line_params.stm0 = ((line_index % 21) % 3);


#else/*Lucent TU12 + TU2 *3 +  TU3*21*/
	line_params.stm4 = line_index / 252;
	line_params.stm1 = (line_index % 252) / 63;
	line_params.stm0 = (line_index % 63) / 21;
	line_params.tug2 = (line_index % 21) / 3;
	line_params.tu = ((line_index % 21) % 3);

#endif

	line_params.transfer_type = WUFE_SDH_TYPE_E1;
	line_params.framed = framing_mode;
	line_params.cas_enable = cas_enable;
	line_params.clock_rec_line_params = NULL;
	line_params.clock_mode = clock_mode;
    WT_Np_UfeLineSdhConfig(&line_params,&line_cfg);
    WT_Np_UfeLineCreate(line_index,&line_cfg,&line_params,line_lb_mode);
}

void WT_Np_UfeLineConfigureAndCreateT1(WP_U32 line_index, WP_U16 framing_mode, WP_U8 cas_enable,WP_U32 line_lb_mode,WP_U32 clock_mode)
{
	STRU_SET_INITAIL_CONFIG *p = npu_get_initial_cfg_info();
    if (p->stru_stm1_cfg_info[0].physicalLayer == NPU_PHYSICAL_SDH 
     && p->stru_stm1_cfg_info[1].physicalLayer == NPU_PHYSICAL_SDH)
    {
      Fprintf("Transfer Type SDH.\n");
      WT_ufe_line_sdh_params line_params;
      WUFE_line_sdh line_cfg;
      memset(&line_params, 0, sizeof(WT_ufe_line_sdh_params));
      line_params.stm4 = line_index / 336;
      line_params.stm1 = (line_index % 336) / 84;
      line_params.stm0 = (line_index % 84) / 28;
      line_params.tug2 = (line_index % 28) / 4;
      line_params.tu = (line_index % 4);
      line_params.transfer_type = WUFE_SDH_TYPE_T1;
      line_params.framed = framing_mode;
      line_params.cas_enable = cas_enable;
      line_params.clock_rec_line_params = NULL;
      line_params.clock_mode = clock_mode;
      WT_Np_UfeLineSdhConfig(&line_params,&line_cfg);
      WT_Np_UfeLineCreate(line_index,&line_cfg,&line_params,line_lb_mode);
    }  
    else if (p->stru_stm1_cfg_info[0].physicalLayer == NPU_PHYSICAL_SONET 
          || p->stru_stm1_cfg_info[1].physicalLayer == NPU_PHYSICAL_SONET)
    {
      Fprintf("Transfer Type SONET.\n");
      WT_ufe_line_sonet_params line_params;
      WUFE_line_sonet line_sonet_cfg;
/*
      memset(&line_params, 0, sizeof(WUFE_line_sonet));
      line_params.transfer_type = WUFE_SONET_TYPE_T1;
      line_params.framed = framing_mode;
      line_params.sts12 = line_index / 336;
      line_params.sts3 = (line_index % 336) / 84;
      line_params.sts1 = (line_index % 84) / 28;
      line_params.vt_group = (line_index % 28) / 4;
      line_params.vt = (line_index % 4);
*/
      WP_U32 line_tmp = line_index;
      memset(&line_params, 0, sizeof(WUFE_line_sonet));
      line_params.transfer_type = WUFE_SONET_TYPE_T1;
      line_params.framed = framing_mode;
      line_params.sts12 = line_tmp / 336;
      line_tmp %= 336;
      line_params.sts3 = line_tmp / 84;
      line_tmp %= 84;
      line_params.sts1 = line_tmp / 28;
      line_tmp %= 28;
      line_params.vt_group = line_tmp / 4;
      line_tmp %= 4;
      line_params.vt = line_tmp;

      line_params.cas_enable = cas_enable;
      line_params.clock_rec_line_params = NULL;
      //line_params.clock_mode = clock_mode;

      memset(&line_sonet_cfg, 0, sizeof(WUFE_line_sonet));
      line_sonet_cfg.struct_id = WUFE_STRUCT_LINE_SONET; 
      line_sonet_cfg.sts12 = line_params.sts12;
      line_sonet_cfg.sts3  = line_params.sts3;
      line_sonet_cfg.sts1  = line_params.sts1;
      line_sonet_cfg.vt_group = line_params.vt_group;
      line_sonet_cfg.vt    = line_params.vt;
      line_sonet_cfg.transfer_type = line_params.transfer_type;
      line_sonet_cfg.framed= line_params.framed;      
      line_sonet_cfg.cas_enable = WUFE_CAS_DISABLE;
        
      
      WT_Np_UfeLineCreateSonet(line_index,&line_sonet_cfg,&line_params,line_lb_mode);

    }
}

WP_S32 WTI_Np_FlexmuxClientPortIndexGet(WP_U32 ufe_line_index, WUFE_line_transf_type transfer_type)
{
    switch (transfer_type)
    {
    case (WUFE_SONET_TYPE_E1):
    case (WUFE_SDH_TYPE_E1):
    {
        WP_U32 spe_num = (ufe_line_index / 28);
        WP_U32 tug2_num = ((ufe_line_index % 28) / 3);
        WP_U32 offset_within_tug2 = ((ufe_line_index % 28) % 3);

        /* In E1 mode skip indices 3,7,11.. due to TUG restriction */
        return ((spe_num*28) + (tug2_num*4) + offset_within_tug2);
    }
    case (WUFE_SONET_TYPE_T1):
    case (WUFE_SDH_TYPE_T1):
    {
        return ufe_line_index % 84;
    }
    default:
        Fprintf("Unsupported transfer_type %d\n", transfer_type);
        exit(-1);
    }
    return 0;
}

WP_S32 WTI_Np_FlexmuxLinePortIndexGet(WP_U32 ufe_line_index, WP_U32 flexmux_id)
{
    WP_U32 result = -1;
    if (flexmux_reg[flexmux_id].flexmux_build_personality != WPX_UFE_FRAMER_BUILD_PERSONALITY_4_PLUS_4_OC3_OC12 &&
        flexmux_reg[flexmux_id].flexmux_build_personality != WPX_UFE_FRAMER_BUILD_PERSONALITY_1_PLUS_1_OC3_OC12 &&
        flexmux_reg[flexmux_id].flexmux_build_personality != WPX_UFE_FRAMER_BUILD_PERSONALITY_2_PLUS_2_OC3      &&
        flexmux_reg[flexmux_id].flexmux_build_personality != WPX_UFE_FRAMER_BUILD_PERSONALITY_4_PLUS_4_OC3)
        WTI_FlexmuxTerminateOnError("Invalid flexmux_build_personality !", 0);

    /* In V6 protection is used and even ports are the working ports while odd
       ports are the protected ports */
    if (flexmux_reg[flexmux_id].flexmux_build_personality == WPX_UFE_FRAMER_BUILD_PERSONALITY_4_PLUS_4_OC3_OC12)
    {
        result = (ufe_line_index / 336);
        /* currently only 4XOC12 ports are supported */
        if (result > 3)
        {
            result = -1;
        }
        else
        {
            result = result * 2; /* Multipication by 2 in order to skip the odd protection lines*/
        }
    }
    /* In S6 protection is not used - port #0 is the only working line port */
    else if ((flexmux_reg[flexmux_id].flexmux_build_personality == WPX_UFE_FRAMER_BUILD_PERSONALITY_1_PLUS_1_OC3_OC12) ||
             (flexmux_reg[flexmux_id].flexmux_build_personality == WPX_UFE_FRAMER_BUILD_PERSONALITY_2_PLUS_2_OC3)      ||
             (flexmux_reg[flexmux_id].flexmux_build_personality == WPX_UFE_FRAMER_BUILD_PERSONALITY_4_PLUS_4_OC3))
    {
        /* In case a single OC12 port is used, then line port index is always 0*/
        if (flexmux_reg[flexmux_id].flexmux_hw_config == WT_FLEXMUX_UFE_412_1_PORT_OC12)
        {
            result = 0;
        }
        /* In case 4 OC3 ports are used, then line port index changes between 0-3*/
        else if ((flexmux_reg[flexmux_id].flexmux_hw_config == WT_FLEXMUX_UFE_412_4_PORTS_OC3) ||
                 (flexmux_reg[flexmux_id].flexmux_hw_config == WT_FLEXMUX_UFE_412_2_PORTS_OC3))
        {
            result = (ufe_line_index / 84);
            if (result > 3)
            {
                result = -1;
            }
        }
    }

    return result;
}

void WT_Np_UfeLineCreate(WP_U32 line_index,WUFE_line_sdh *line_cfg,WT_ufe_line_sdh_params *line_sdh_params,WP_U32 line_lb_mode)
{

    WUFE_status status;
	status = WUFE_LineCreate(&ufe_line_handle[line_index],0,WUFE_STRUCT_LINE_SDH,line_cfg);
	terminate_on_error(status, "SDH WUFE_LineCreate");
	status = WUFE_LineEnable(ufe_line_handle[line_index], WUFE_FULL_DUPLEX);
	terminate_on_error(status, "SDH WUFE_LineEnable");

	int client_port, line_port;
	WTI_flexmux_connection_type con_type =0;
	client_port = WTI_FlexmuxClientPortIndexGet(E1MapTable[line_index], line_sdh_params->transfer_type);
	line_port = WTI_FlexmuxLinePortIndexGet(E1MapTable[line_index], 0);
	if (line_port == -1)
	{
        Fprintf("Illegal line port index %d\n",line_index);
        exit(1);
	}/*rizh need to just line_sdh_params->cas_enable later*/
	switch (line_sdh_params->transfer_type)
	{
    case (WUFE_SDH_TYPE_E1):
    {
        if(line_sdh_params->framed == WUFE_FRAMED && line_sdh_params->cas_enable == WUFE_CAS_ENABLE)
        {
            //con_type = WT_FLEXMUX_CONNECTION_TYPE_E1_FRAMED_CAS;
            con_type = WT_FLEXMUX_CONNECTION_TYPE_E1_FRAMED;
        }
        else if(line_sdh_params->framed == WUFE_FRAMED && line_sdh_params->cas_enable == WUFE_CAS_DISABLE)
        {
            con_type = WT_FLEXMUX_CONNECTION_TYPE_E1_FRAMED;
        }
        else
        {
            con_type = WT_FLEXMUX_CONNECTION_TYPE_E1_UNFRAMED;
        }
        break;
    }
    case (WUFE_SDH_TYPE_T1):
    {
        if(line_sdh_params->cas_enable == WUFE_CAS_DISABLE)
        {
            con_type = WT_FLEXMUX_CONNECTION_TYPE_T1_FRAMED;
        }
        else /*default: OPMODE_T1_ESF*/
        {
            con_type = WT_FLEXMUX_CONNECTION_TYPE_T1_FRAMED_CAS;
        }
        break;
    }
    default:
        Fprintf("Unsupported transfer_type %d\n", line_sdh_params->transfer_type);
        exit(1);
	}
	Fprintf("lineIndex = %d line_port = %d client_port = %d \n",line_index,line_port,client_port);
	(void)WTI_FlexmuxConnectionCreate(0,
                                      line_port,
                                      client_port,
                                      con_type,
                                      (void *)line_sdh_params);
	if(line_lb_mode == UFE_LB_SYSTEM)
	{
		status = WUFE_LineLoopbackConfigure(ufe_line_handle[line_index], WUFE_LINE_LOOPBACK_TYPE_SYSTEM);
		terminate_on_error(status, "WUFE_LineLoopbackConfigure SYSTEM");
	}
	else if(line_lb_mode == UFE_LB_LINE)
	{
		status = WUFE_LineLoopbackConfigure(ufe_line_handle[line_index], WUFE_LINE_LOOPBACK_TYPE_LINE);
		terminate_on_error(status, "WUFE_LineLoopbackConfigure SYSTEM");
	}
	else/*WTI_FlexmuxConnectionCreate has been set to normal*/
	{
	}

}

void WT_Np_UfeLineCreateSonet(WP_U32 line_index,WUFE_line_sonet *line_cfg,WT_ufe_line_sonet_params *line_sonet_params,WP_U32 line_lb_mode)
{

    WUFE_status status;
	status = WUFE_LineCreate(&ufe_line_handle[line_index],0,WUFE_STRUCT_LINE_SONET,line_cfg);
	terminate_on_error(status, "SONET WUFE_LineCreate");
	status = WUFE_LineEnable(ufe_line_handle[line_index], WUFE_FULL_DUPLEX);
	terminate_on_error(status, "SONET WUFE_LineEnable");

	int client_port, line_port;
	WTI_flexmux_connection_type con_type =0;
	client_port = WTI_FlexmuxClientPortIndexGet(E1MapTable[line_index], line_sonet_params->transfer_type);
	line_port = WTI_FlexmuxLinePortIndexGet(E1MapTable[line_index], 0);
	if (line_port == -1)
	{
        Fprintf("Illegal line port index %d\n",line_index);
        exit(1);
	}/*rizh need to just line_sdh_params->cas_enable later*/
//tyson
	switch (line_sonet_params->transfer_type)
	{
    case (WUFE_SONET_TYPE_E1):
    {
        if(line_sonet_params->framed == WUFE_FRAMED && line_sonet_params->cas_enable == WUFE_CAS_ENABLE)
        {
            //con_type = WT_FLEXMUX_CONNECTION_TYPE_E1_FRAMED_CAS;
            con_type = WT_FLEXMUX_CONNECTION_TYPE_E1_FRAMED;
        }
        else if(line_sonet_params->framed == WUFE_FRAMED && line_sonet_params->cas_enable == WUFE_CAS_DISABLE)
        {
            con_type = WT_FLEXMUX_CONNECTION_TYPE_E1_FRAMED;
        }
        else
        {
            con_type = WT_FLEXMUX_CONNECTION_TYPE_E1_UNFRAMED;
        }
        break;
    }
    case (WUFE_SONET_TYPE_T1):
    {
        if(line_sonet_params->cas_enable == WUFE_CAS_DISABLE)
        {
            con_type = WT_FLEXMUX_CONNECTION_TYPE_T1_FRAMED;
        }
        else /*default: OPMODE_T1_ESF*/
        {
            con_type = WT_FLEXMUX_CONNECTION_TYPE_T1_FRAMED_CAS;
        }
        break;
    }
    default:
        Fprintf("Unsupported transfer_type %d\n", line_sonet_params->transfer_type);
        exit(1);
	}
	Fprintf("lineIndex = %d line_port = %d client_port = %d \n",line_index,line_port,client_port);
	(void)WTI_FlexmuxConnectionCreate(0,
                                      line_port,
                                      client_port,
                                      con_type,
                                      (void *)line_sonet_params);
	if(line_lb_mode == UFE_LB_SYSTEM)
	{
		status = WUFE_LineLoopbackConfigure(ufe_line_handle[line_index], WUFE_LINE_LOOPBACK_TYPE_SYSTEM);
		terminate_on_error(status, "WUFE_LineLoopbackConfigure SYSTEM");
	}
	else if(line_lb_mode == UFE_LB_LINE)
	{
		status = WUFE_LineLoopbackConfigure(ufe_line_handle[line_index], WUFE_LINE_LOOPBACK_TYPE_LINE);
		terminate_on_error(status, "WUFE_LineLoopbackConfigure SYSTEM");
	}
	else/*WTI_FlexmuxConnectionCreate has been set to normal*/
	{
	}

}

void np_display_ufe4_shd_status(void)
{
	STRU_STM1_ALARM_INFO *alamInfo = np_get_ufe4_aralm_info();
    STRU_STM1_SDH_ALARM * AlarmSDH;
	int stm1;
	for(stm1 = 0; stm1< SPP_MAX_STM1_PORT_NUM;stm1++)
	{
		AlarmSDH = &alamInfo->SDH_alarm[stm1];
		Fprintf("STM1 %d STATUS:\n",stm1);
	    STRU_SET_INITAIL_CONFIG *p = npu_get_initial_cfg_info();
        if (p->stru_stm1_cfg_info[0].physicalLayer == NPU_PHYSICAL_SDH && p->stru_stm1_cfg_info[1].physicalLayer == NPU_PHYSICAL_SDH)
        {
            Fprintf("SDH_LOS = %d\n",AlarmSDH->SDH_LOS);
            Fprintf("SDH_LOF = %d\n",AlarmSDH->SDH_LOF);
            Fprintf("SDH_RS_TIM = %d\n",AlarmSDH->SDH_RS_TIM);
            Fprintf("SDH_MS_AIS = %d\n",AlarmSDH->SDH_MS_AIS);
            Fprintf("SDH_MS_RDI = %d\n",AlarmSDH->SDH_MS_RDI);
            Fprintf("SDH_OOF = %d\n",AlarmSDH->SDH_OOF);
            Fprintf("SDH_MS_EXC = %d\n",AlarmSDH->SDH_MS_EXC);
            Fprintf("SDH_MS_DEG = %d\n",AlarmSDH->SDH_MS_DEG);
            Fprintf("SDH_AU_AIS = %d\n",AlarmSDH->SDH_AU_AIS);
            Fprintf("SDH_AU_LOP = %d\n",AlarmSDH->SDH_AU_LOP);
            Fprintf("SDH_HP_LOM = %d\n",AlarmSDH->SDH_HP_LOM);
            Fprintf("SDH_HP_TIM = %d\n",AlarmSDH->SDH_HP_TIM);
            Fprintf("SDH_HP_PLM = %d\n",AlarmSDH->SDH_HP_PLM);
            Fprintf("SDH_HP_UNEQ = %d\n",AlarmSDH->SDH_HP_UNEQ);
            Fprintf("SDH_HP_RDI = %d\n",AlarmSDH->SDH_HP_RDI);
            Fprintf("SDH_HP_EXC = %d\n",AlarmSDH->SDH_HP_EXC);
            Fprintf("SDH_HP_DEG = %d\n",AlarmSDH->SDH_HP_DEG);
            Fprintf("SDH_LP_TIM = %d\n",AlarmSDH->SDH_LP_TIM);
            Fprintf("SDH_LP_PLM = %d\n",AlarmSDH->SDH_LP_PLM);
            Fprintf("SDH_LP_UNEQ = %d\n",AlarmSDH->SDH_LP_UNEQ);
            Fprintf("SDH_LP_RDI = %d\n",AlarmSDH->SDH_LP_RDI);
            Fprintf("SDH_LP_EXC = %d\n",AlarmSDH->SDH_LP_EXC);
            Fprintf("SDH_LP_DEG = %d\n",AlarmSDH->SDH_LP_DEG);
            Fprintf("SDH_TU_AIS = %d\n",AlarmSDH->SDH_TU_AIS);
            Fprintf("SDH_TU_LOP = %d\n",AlarmSDH->SDH_TU_LOP);
            Fprintf("\n");
        }
        else if (p->stru_stm1_cfg_info[0].physicalLayer == NPU_PHYSICAL_SONET || p->stru_stm1_cfg_info[1].physicalLayer == NPU_PHYSICAL_SONET)
        {
            Fprintf("SONET_LOS = %d\n", AlarmSDH->SONET_LOS);
            Fprintf("SONET_LOF = %d\n", AlarmSDH->SONET_LOF);
            Fprintf("SONET_TIM_S = %d\n", AlarmSDH->SONET_TIM_S);
            Fprintf("SONET_AIS_L = %d\n", AlarmSDH->SONET_AIS_L);
            Fprintf("SONET_RDI_L = %d\n", AlarmSDH->SONET_RDI_L);    
            Fprintf("SONET_OOF = %d\n",   AlarmSDH->SONET_OOF);    
            Fprintf("SONET_EXC_L = %d\n", AlarmSDH->SONET_EXC_L);
            Fprintf("SONET_DEG_L = %d\n", AlarmSDH->SONET_DEG_L);
            Fprintf("SONET_LOM_P = %d\n", AlarmSDH->SONET_LOM_P);
            Fprintf("SONET_TIM_P = %d\n", AlarmSDH->SONET_TIM_P);
            Fprintf("SONET_PLM_P = %d\n", AlarmSDH->SONET_PLM_P);
            Fprintf("SONET_UNEQ_P = %d\n", AlarmSDH->SONET_UNEQ_P);
            Fprintf("SONET_RDI_P = %d\n", AlarmSDH->SONET_RDI_P);
            Fprintf("SONET_EXC_P = %d\n", AlarmSDH->SONET_EXC_P);
            Fprintf("SONET_DEG_P = %d\n", AlarmSDH->SONET_DEG_P);
            Fprintf("SONET_TIM_V = %d\n", AlarmSDH->SONET_TIM_V);
            Fprintf("SONET_PLM_V = %d\n", AlarmSDH->SONET_PLM_V);
            Fprintf("SONET_UNEQ_V = %d\n", AlarmSDH->SONET_UNEQ_V);
            Fprintf("SONET_RDI_V = %d\n", AlarmSDH->SONET_RDI_V);
            Fprintf("SONET_EXC_V = %d\n", AlarmSDH->SONET_EXC_V);
            Fprintf("SONET_DEG_V = %d\n", AlarmSDH->SONET_DEG_V);  
            Fprintf("SONET_AIS_V = %d\n", AlarmSDH->SONET_AIS_V);
            Fprintf("SONET_LOP_V = %d\n", AlarmSDH->SONET_LOP_V);
            Fprintf("SONET_AIS_P = %d\n", AlarmSDH->SONET_AIS_P);
            Fprintf("SONET_LOP_P = %d\n", AlarmSDH->SONET_LOP_P);
            Fprintf("\n");
        }
	}
	return;
}

void np_display_all_ufe4_e1t1_status()
{
    WP_U16 i;
    WP_U16 stm1;

    STRU_SET_INITAIL_CONFIG *initial_cfg = npu_get_initial_cfg_info();
    for (stm1 = 0; stm1 < SPP_MAX_STM1_PORT_NUM; stm1++)
    {
        if(initial_cfg->stru_stm1_cfg_info[stm1].stm1Valid == NPU_VALID)
        {
            for(i = 0; i < SPP_MAX_TRUNK_NUM; i++)
            {
                if(initial_cfg->stru_stm1_cfg_info[stm1].trunk_info[i].trunkValid == NPU_VALID)
                {
                    np_display_ufe4_e1t1_status(stm1, i);
                }
            }
        }
    }
}

void np_display_ufe4_e1t1_status(WP_U32 LinePort ,WP_U32 lineIndex)
{
    if(lineIndex > SPP_MAX_TRUNK_NUM)
        return -1;
    if(LinePort > SPP_MAX_STM1_PORT_NUM)
        return -1;
	STRU_STM1_ALARM_INFO *alamInfo = np_get_ufe4_aralm_info();
    STRU_STM1_E1T1_ALARM_INFO * AlarmE1T1 = &alamInfo->E1T1_alarm[LinePort][lineIndex];

    if (AlarmE1T1->LOS || AlarmE1T1->LOF ||
        AlarmE1T1->AIS || AlarmE1T1->AIS_CI ||
        AlarmE1T1->RAI || AlarmE1T1->RAI_CI)
    {
        Fprintf("STM1 %d Line %d STATUS:\n", LinePort+1, lineIndex+1);
        Fprintf("LOS = %d\n",AlarmE1T1->LOS);
        Fprintf("LOF = %d\n",AlarmE1T1->LOF);
        Fprintf("AIS = %d\n",AlarmE1T1->AIS);
        Fprintf("AIS_CI = %d\n",AlarmE1T1->AIS_CI);
        Fprintf("RAI = %d\n",AlarmE1T1->RAI);
        Fprintf("RAI = %d\n",AlarmE1T1->RAI_CI);
        Fprintf("\n");
    }
	return;
}

STRU_STM1_ALARM_INFO* np_get_ufe4_aralm_info(void)
{
	return &np_ufe4_ararm_info;
}

void np_set_ufe4_aralm_info(WP_U32 LinePort, WP_U32 AlarmCategory, WP_U32 IsAsserted)
{
    if(LinePort > SPP_MAX_STM1_PORT_NUM) return;
	if(AlarmCategory > WPX_UFE_FRAMER_SONET_LOP_P)return;
	STRU_STM1_ALARM_INFO *alamInfo = np_get_ufe4_aralm_info();
    STRU_STM1_SDH_ALARM * AlarmSDH = &alamInfo->SDH_alarm[LinePort];
    *(&AlarmSDH->SDH_LOS + AlarmCategory) = IsAsserted;
	return ;
}
void np_set_ufe4_E1T1_alarm_info(WP_U8 Stm1, WP_U16 LineIndex, WP_U32 AlarmCategory, WP_U32 IsAsserted)
{
    STRU_STM1_ALARM_INFO *alamInfo = np_get_ufe4_aralm_info();
	STRU_STM1_E1T1_ALARM_INFO * AlarmE1T1 = &alamInfo->E1T1_alarm[Stm1][LineIndex];
	if(AlarmCategory == WPX_UFE_FRAMER_SDH_LP_PLM || AlarmCategory == WPX_UFE_FRAMER_SONET_RDI_V) // ?
	{
		*(&AlarmE1T1->AIS) = IsAsserted;
	}
	return ;
}

void np_set_ufe4_E1T1_aralm_info_fromClientPort( WP_U32 ClientPort, WP_U32 AlarmCategory, WP_U32 IsAsserted)
{

	WP_U16 lineIndex;
	WP_U16 stm1;
	WP_U16 client_port;
	STRU_SET_INITAIL_CONFIG *initial_cfg = npu_get_initial_cfg_info();
	for(lineIndex = 0;lineIndex < N_UFE_MAX_LINES_T1;lineIndex++)
	{
	    stm1 = lineIndex/SPP_MAX_TRUNK_NUM;
	    if(initial_cfg->stru_stm1_cfg_info[stm1].E1T1type == NPU_MODE_E1)
	    {
			client_port = WTI_FlexmuxClientPortIndexGet(E1MapTable[lineIndex], WUFE_SDH_TYPE_E1);
	    }
		else
		{
			client_port = WTI_FlexmuxClientPortIndexGet(E1MapTable[lineIndex], WUFE_SDH_TYPE_T1);
		}
		if(client_port == ClientPort)
		{
            break;
		}
	}
	if(lineIndex == N_UFE_MAX_LINES_T1)
	{
        return;
	}
	//lineIndex = lineIndex%SPP_MAX_TRUNK_NUM;
    if(initial_cfg->stru_stm1_cfg_info[stm1].E1T1type == NPU_MODE_E1)
    {
        stm1 = lineIndex/(N_UFE_MAX_LINES_E1/2);
        lineIndex = lineIndex%(N_UFE_MAX_LINES_E1/2);
    }
    else
    {
        stm1 = lineIndex/(N_UFE_MAX_LINES_T1/2);
        lineIndex = lineIndex%(N_UFE_MAX_LINES_T1/2);
    }
    STRU_STM1_ALARM_INFO *alamInfo = np_get_ufe4_aralm_info();
    STRU_STM1_E1T1_ALARM_INFO * AlarmE1T1 = &alamInfo->E1T1_alarm[stm1][lineIndex];
    *(&AlarmE1T1->LOS + AlarmCategory) = IsAsserted;
	return ;
}

int np_check_ufe4_stm1_status(WP_U32 LinePort)
{
	STRU_STM1_ALARM_INFO *alamInfo = np_get_ufe4_aralm_info();
    STRU_STM1_SDH_ALARM * AlarmSDH = &alamInfo->SDH_alarm[LinePort];
	if(AlarmSDH->SDH_OOF)//LOF miss when initial    ? tyson SONET_OOF?
	{
	    return LINK_STATUS_DOWN;
	}
	else
	{
	    return LINK_STATUS_UP;
	}
}


int np_check_ufe4_e1t1_status(WP_U32 LinePort ,WP_U32 lineIndex)
{
    if(lineIndex > SPP_MAX_TRUNK_NUM) return -1;
	if(LinePort > SPP_MAX_STM1_PORT_NUM) return -1;
	STRU_STM1_ALARM_INFO *alamInfo = np_get_ufe4_aralm_info();
    STRU_STM1_E1T1_ALARM_INFO * AlarmE1T1 = &alamInfo->E1T1_alarm[LinePort][lineIndex];
	if(AlarmE1T1->AIS_CI ||AlarmE1T1->AIS)//AIS_CI == LOF
	{
	    return LINK_STATUS_DOWN;
	}
	else
	{
	    return LINK_STATUS_UP;
	}
}

void np_config_all_phys(void)
{
	int i,j,k;
	WP_U32 slot_selection;
	WP_U32 line_index_base,line_index_counts,phy_counts;
	STRU_SET_INITAIL_CONFIG *initial_cfg = npu_get_initial_cfg_info();

	for(i = 0; i < SPP_MAX_STM1_PORT_NUM; i ++)
	{
        if(NPU_VALID == initial_cfg->stru_stm1_cfg_info[i].stm1Valid)
        {
            if(initial_cfg->stru_stm1_cfg_info[i].E1T1type == NPU_MODE_E1)
            {
                line_index_base = i*N_UFE_MAX_LINES_E1/2;
                line_index_counts = (1+i)*N_UFE_MAX_LINES_E1/2;
                phy_counts = 32;
            }
            else
            {
                line_index_base = i*N_UFE_MAX_LINES_T1/2;
                line_index_counts = (1+i)*N_UFE_MAX_LINES_T1/2;
                phy_counts = 24;
            }
            for(j = line_index_base; j < line_index_counts; j ++)
            {
                if(NPU_VALID == initial_cfg->stru_stm1_cfg_info[i].trunk_info[j - line_index_base].trunkValid)
                {
                    for(k = 0; k < phy_counts; k ++)
                    {
                        if(NPU_VALID == initial_cfg->stru_stm1_cfg_info[i].trunk_info[j - line_index_base].group_info[k].groupValid)
                        {
                            if(ufe_device_handle[j][k] == 0)
                            {
                                slot_selection = initial_cfg->stru_stm1_cfg_info[i].trunk_info[j - line_index_base].group_info[k].slot_selection;
                                WTUFE_config_phys(j,k,slot_selection,line_index_base);
                                Fprintf("UFE phy on stm1 %d trunk %d ,group %d is Created slot_selection = 0x%x \n",i,j+1,k,slot_selection);
                            }
                        }
                    }
                }
                else // for these un-configured E1s, we need create the defaut object to clear the LOP alarms on the peer transmission device;
                {
                    //Create UFE phy with 1 slot for un-configured E1/T1:
                    //WTUFE_config_phys(j,1,0x2,line_index_base);
                    //Fprintf("Create UFE phy for un-configured E1/T1: UFE phy on stm1 %d trunk %d ,group %d is Created slot_selection = 0x%x \n",i,j+1,1,0x2);
                }
            }
        }
	}
}
void np_DeviceDelete(WP_U32 trunkid,WP_U32 groupid)
{
    WP_status status;
	if(0 != ufe_device_handle[trunkid][groupid])
	{
	    status = WP_DeviceDisable(ufe_device_handle[trunkid][groupid], WP_DIRECTION_DUPLEX);
		if(WP_OK != status)
		{
	        Fprintf("np_DeviceDelete WP_DeviceDisable failed! status = %d\n",status);
		}
	    status = WP_DeviceDelete(ufe_device_handle[trunkid][groupid]);
		if(WP_OK != status)
		{
	        Fprintf("np_DeviceDelete WP_DeviceDelete failed! status = %d\n",status);
		}
		else
		{
	        ufe_device_handle[trunkid][groupid] = 0;
		}
	}
}

WP_status np_Ufe_Phy_Delete(WP_U32 trunkid,WP_U32 groupid)
{
    WP_status status;
    WP_U32 j;

    if(0 != ufe_phy_handle[trunkid][groupid])
    {
	    status = WUFE_PhyDisable(ufe_phy_handle[trunkid][groupid], WUFE_FULL_DUPLEX);
		if(status != WUFE_OK)
		{
	        Fprintf("np_Ufe_Phy_Delete WUFE_PhyDisable failed! status = %d\n",status);
		}

	    for(j=0; j<100000;j++)
	    {
	    	status = WUFE_PhyDelete(ufe_phy_handle[trunkid][groupid]);
	    	if(status == WUFE_OK)
	    	{
	            ufe_phy_handle[trunkid][groupid] = 0;
				break;
			}
	    	if (status != WUFE_ERR_PHY_FIFO_NOT_EMPTY)
                return status;
	    	WP_Delay(10);
	    }
	}
}

WP_status np_Channel_is_None(WP_U32 stm1Id, WP_U32 trunkid, WP_U32 groupid)
{
    WP_U32 vfport,dlci;
    STRU_VPORT_INFO  *VportInfo;

    for(dlci = 0; dlci < 1025; dlci ++)
    {
        vfport = np_fraddr_pack(stm1Id+1, trunkid+1, groupid, dlci);
	    VportInfo = npu_get_vport_info(vfport);
        if(VportInfo != NULL)
        {
	        if((VportInfo->ch_info.rx_host_handle != 0) ||(VportInfo->ch_info.rx_iw_handle != 0)||(VportInfo->ch_info.tx_iw_handle != 0))
	        {
	            return -1;
		    }
	    }
    }
    return WP_OK;
}

#if 0

#ifndef SERIAL_1_CPLD_INTERRUPT_MASK
#define SERIAL_1_CPLD_INTERRUPT_MASK 0x6f
#endif

WP_U8 last_interrupt_mask = SERIAL_1_CPLD_INTERRUPT_MASK;

void WT_ReinstateInterruptMask(WP_U32 wpid)
{

    /* Reset the CPLD interrupt mask */
    WPX_Ufe412CpldInterruptMaskSet(wpid, last_interrupt_mask);
}
#endif
void WT_Eint3Interrupt(WP_U32 wpid, WP_U32 signal_info)
{
    WP_boolean is_framer_int, is_core_int;
    WUFE_events active_events;
    extern void WPX_UFE_FRAMER_ISR(WP_U8 iDevice);

    //Fprintf("------------------------------> WT_Eint3Interrupt\n");fflush(stdout);

    memset(&active_events, 0, sizeof(WUFE_events));

    is_framer_int = WUFE_UfeFramerHwInterruptGet(0);
    is_core_int   = WUFE_UfeCoreHwInterruptGet(0);

    if (is_framer_int) {
        //Fprintf("Framer interrupt\n");

        //sem_post(&mbox_sema);

        WPX_UFE_FRAMER_ISR(0);
    }

    if (is_core_int) {
        //Fprintf("Core interrupt\n");
        //displayUfeEvents();
    }
    /*37442   10/17/2011 Working UFE4 dirver will cause CPU occupancy rate always to 100%*/
#if !defined(WPL_MAILBOX_LOCK_KEY)
#define WPL_MAILBOX_LOCK_KEY WPL_LOCK_KEY_CREATE(WPL_HW_LOCK, WPL_PRIVATE_LOCK, 6, 0)
#endif

    WPL_Unlock(WPL_MAILBOX_LOCK_KEY, 0);

    //WT_ReinstateInterruptMask(wpid);

    return;
}

void np_ufe_release(void)
{
    WPL_IntDisable (WP_WINPATH(0), WPL_Eint3Ufe4);
	sleep(1);
	WPL_Unlock(WPL_MAILBOX_LOCK_KEY, 0);
    WPX_FRMR_LowLevelCommsDisableAccess(0);
    //WPL_ThreadDestroy(&mailbox_thread_id);
}

void npu_ufe_reconfig_clock_mode( WP_U32 line_index ,WP_U32 trans_type ,WP_U32 clock_mode)
{
    WP_S32 client_port;
    WP_S32 status;
	if(trans_type == NPU_MODE_E1)
	{
		client_port = WTI_FlexmuxClientPortIndexGet(E1MapTable[line_index], WUFE_SDH_TYPE_E1);
	}
	else
	{
	    client_port = WTI_FlexmuxClientPortIndexGet(E1MapTable[line_index], WUFE_SDH_TYPE_T1);
	}
	Fprintf("npu_ufe_reconfig_clock_mode clock_mode=%d\n",clock_mode);
	/* Set the PDH Tx line timing  */
	status = WPX_FRMR_SOCKET_PDH_SetClkRec(0, 0, client_port, clock_mode);
	WTI_FlexmuxCheckStatus("npu_ufe_reconfig_clock_mode WPX_FRMR_SOCKET_PDH_SetClkRec", status, __LINE__);
	return;
}

void npu_enable_ufe_serdes_interrupt(void)
{
#if 0
	WPL_IntConnect(WP_WINPATH(0), WPL_SerdesInterrupt, 0, WT_SerdesInterrupt);
	WPL_IntEnable (WP_WINPATH(0), WPL_SerdesInterrupt);

	WPL_IntConnect(WP_WINPATH(0), WPL_SgmiiAn, 0, WT_SgmiiAnInterrupt);
	WPL_IntEnable (WP_WINPATH(0), WPL_SgmiiAn);

	// Enable connected SerDes interrupts
    *(volatile unsigned int *)(unsigned long)(0x1e0082f0 + WPL_RIF_VIRTUAL(0, 0)) = 0;
    *(volatile unsigned int *)(unsigned long)(0x1e0082f4 + WPL_RIF_VIRTUAL(0, 0)) = htonl(0x80000777);
#endif

}
