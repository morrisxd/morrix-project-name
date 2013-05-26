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
 * File Name	:	  np_stats.c
 * File Mark	 :	  none
 * Description	:	  npu stats file
 * Others		 :	  none
 * Version       :       v1.00
 * Author		 :
 * Date 		 :
 *
 * Modify History		:
 *	   Date              Version         Author           Modification
 *-----------------------------------------------------------------------------
 2011-3-6              initial version   Richard zhou
 ----------------------------------------------------------------------------*/

#include "np_common.h"
#include "spp_wddi_server.h"
extern void terminate_on_error(WP_handle handle, WP_CHAR *s);
extern WP_handle gbedev1, gbedev2;
extern WUFE_handle ufe_device_handle[N_UFE_MAX_LINES_T1][32];

typedef union StatField
{
    WP_U64 field;
    struct {
        WP_U32 low;
        WP_U32 high;
    } part;
} WPE_StatField;


WP_status txfr_stat_get(WP_U32 Channelno, void *p) {
    WP_status status;
    WP_U32 size;
    WP_stats_fr_tx fr_host,fr_iw;
    STRU_VPORT_INFO	*VportInfo = npu_get_vport_info(Channelno);
    if(VportInfo == NULL)
    {
        WDDI_WARNING(" can't get fr vport info vport = %d \n ",Channelno);
        return -1;
    }
    else if(VportInfo->UsedFlag == UNUSED)
    {
        WDDI_WARNING(" can't get fr vport info ,fr vport closed ! vport = %d \n ",Channelno);
        return -1;
    }
    FR_CHANNEL_INFO *ch_info = &VportInfo->ch_info;
    memset (p, 0, sizeof(WP_stats_fr_tx));
    memset (&fr_host, 0, sizeof(WP_stats_fr_tx));
    memset (&fr_iw, 0, sizeof(WP_stats_fr_tx));
    status = WP_ChannelStatistics(ch_info->tx_iw_handle, (void *)&fr_host);
    if (status)
    {
        Fprintf("Error Reading txfr Stat\n");
        return status;
    }

    status = WP_ChannelStatistics(ch_info->tx_host_handle, (void *)&fr_iw);
    if (status)
    {
        Fprintf("Error Reading txfr Stat\n");
        return status;
    }

    /*add host and iw*/
    WP_U32 i;
    for(i = 0;i < sizeof(WP_stats_fr_tx)/sizeof(WP_U64);i++)
    {
        *((WP_U64 *)p + i) = *((WP_U64 *)&fr_host + i) + *((WP_U64 *)&fr_iw + i);
    }
    return WP_OK;
}

WP_status rxfr_stat_get(WP_U32 Channelno, void *p) {
    WP_status status;
    WP_U32 size;
    STRU_VPORT_INFO	*VportInfo = npu_get_vport_info(Channelno);
    if(VportInfo == NULL)
    {
        WDDI_WARNING(" can't get fr vport info vport = %d \n ",Channelno);
        return -1;
    }
    else if(VportInfo->UsedFlag == UNUSED)
    {
        WDDI_WARNING(" can't get fr vport info ,vport unused ! vport = %d \n ",Channelno);
        return -1;
    }
    FR_CHANNEL_INFO *ch_info = &VportInfo->ch_info;

    memset (p, 0, sizeof(WP_stats_fr_rx));
    status = WP_ChannelStatistics(ch_info->rx_iw_handle, (WP_stats_fr_rx *)p);
    if (status)
    {
        Fprintf("Error Reading rxfr Stat\n");
        return status;
    }
    return WP_OK;
}

void struct_pos_fr_add(WP_stats_pos_fr* p1, WP_stats_pos_fr* p2)
{
    p1->rx_frames = p1->rx_frames + p2->rx_frames;
    p1->rx_bytes = p1->rx_bytes + p2->rx_bytes;
    p1->rx_frames_de_set = p1->rx_frames_de_set + p2->rx_frames_de_set;
    p1->rx_bytes_de_set = p1->rx_bytes_de_set + p2->rx_bytes_de_set;
    p1->rx_frames_fecn_set = p1->rx_frames_fecn_set + p2->rx_frames_fecn_set;
    p1->rx_frames_becn_set = p1->rx_frames_becn_set + p2->rx_frames_becn_set;
    p1->rx_discarded_err_crc = p1->rx_discarded_err_crc + p2->rx_discarded_err_crc;
    p1->rx_discarded_err_alignment = p1->rx_discarded_err_alignment + p2->rx_discarded_err_alignment;
    p1->rx_discarded_length_violation = p1->rx_discarded_length_violation + p2->rx_discarded_length_violation;
    p1->rx_discarded_illegal_header = p1->rx_discarded_illegal_header + p2->rx_discarded_illegal_header;
    p1->rx_discarded_abort = p1->rx_discarded_abort + p2->rx_discarded_abort;
    p1->rx_discarded_unsupported_dlci = p1->rx_discarded_unsupported_dlci + p2->rx_discarded_unsupported_dlci;
    p1->rx_last_unsupported_dlci = p1->rx_last_unsupported_dlci + p2->rx_last_unsupported_dlci;
    p1->rx_discarded_fbp_empty = p1->rx_discarded_fbp_empty + p2->rx_discarded_fbp_empty;
    p1->rx_discarded_hostq_full = p1->rx_discarded_hostq_full + p2->rx_discarded_hostq_full;
    p1->rx_discarded_err_parity = p1->rx_discarded_err_parity + p2->rx_discarded_err_parity;
    p1->rx_discarded_err_sop_eop = p1->rx_discarded_err_sop_eop + p2->rx_discarded_err_sop_eop;
    p1->rx_total_err_bytes = p1->rx_total_err_bytes + p2->rx_total_err_bytes;
    p1->tx_frames = p1->tx_frames + p2->tx_frames;
    p1->tx_bytes = p1->tx_bytes + p2->tx_bytes;
    p1->tx_frames_de_set = p1->tx_frames_de_set + p2->tx_frames_de_set;
    p1->tx_bytes_de_set = p1->tx_bytes_de_set + p2->tx_bytes_de_set;
    p1->tx_frames_fecn_set = p1->tx_frames_fecn_set + p2->tx_frames_fecn_set;
    p1->tx_frames_becn_set = p1->tx_frames_becn_set + p2->tx_frames_becn_set;
    p1->tx_underrun = p1->tx_underrun + p2->tx_underrun;
    p1->tx_aborted_frames = p1->tx_aborted_frames + p2->tx_aborted_frames;
    p1->serial_dps_if_stats.tx_frames = p1->serial_dps_if_stats.tx_frames + p2->serial_dps_if_stats.tx_frames;
    p1->serial_dps_if_stats.tx_bytes = p1->serial_dps_if_stats.tx_bytes + p2->serial_dps_if_stats.tx_bytes;
    p1->serial_dps_if_stats.rx_frames = p1->serial_dps_if_stats.rx_frames + p2->serial_dps_if_stats.rx_frames;
    p1->serial_dps_if_stats.rx_bytes = p1->serial_dps_if_stats.rx_bytes + p2->serial_dps_if_stats.rx_bytes;
    p1->serial_dps_if_stats.rx_dropped_frames = p1->serial_dps_if_stats.rx_dropped_frames + p2->serial_dps_if_stats.rx_dropped_frames;
    p1->serial_dps_if_stats.rx_pce_denied_frames = p1->serial_dps_if_stats.rx_pce_denied_frames + p2->serial_dps_if_stats.rx_pce_denied_frames;
}

void struct_upi_statics_add(STRU_UPI_STATICS* p1, STRU_UPI_STATICS* p2)
{
    p1->rx_frames = p1->rx_frames + p2->rx_frames;
    p1->rx_bytes = p1->rx_bytes + p2->rx_bytes;
    p1->rx_frames_de_set = p1->rx_frames_de_set + p2->rx_frames_de_set;
    p1->rx_bytes_de_set = p1->rx_bytes_de_set + p2->rx_bytes_de_set;
    p1->rx_frames_fecn_set = p1->rx_frames_fecn_set + p2->rx_frames_fecn_set;
    p1->rx_frames_becn_set = p1->rx_frames_becn_set + p2->rx_frames_becn_set;
    p1->rx_discarded_err_crc = p1->rx_discarded_err_crc + p2->rx_discarded_err_crc;
    p1->rx_discarded_err_alignment = p1->rx_discarded_err_alignment + p2->rx_discarded_err_alignment;
    p1->rx_discarded_length_violation = p1->rx_discarded_length_violation + p2->rx_discarded_length_violation;
    p1->rx_discarded_illegal_header = p1->rx_discarded_illegal_header + p2->rx_discarded_illegal_header;
    p1->rx_discarded_abort = p1->rx_discarded_abort + p2->rx_discarded_abort;
    p1->rx_discarded_unsupported_dlci = p1->rx_discarded_unsupported_dlci + p2->rx_discarded_unsupported_dlci;
    p1->rx_last_unsupported_dlci = p1->rx_last_unsupported_dlci + p2->rx_last_unsupported_dlci;
    p1->rx_discarded_fbp_empty = p1->rx_discarded_fbp_empty + p2->rx_discarded_fbp_empty;
    p1->rx_discarded_hostq_full = p1->rx_discarded_hostq_full + p2->rx_discarded_hostq_full;
    p1->rx_discarded_err_parity = p1->rx_discarded_err_parity + p2->rx_discarded_err_parity;
    p1->rx_discarded_err_sop_eop = p1->rx_discarded_err_sop_eop + p2->rx_discarded_err_sop_eop;
    p1->rx_total_err_bytes = p1->rx_total_err_bytes + p2->rx_total_err_bytes;
    p1->tx_frames = p1->tx_frames + p2->tx_frames;
    p1->tx_bytes = p1->tx_bytes + p2->tx_bytes;
    p1->tx_frames_de_set = p1->tx_frames_de_set + p2->tx_frames_de_set;
    p1->tx_bytes_de_set = p1->tx_bytes_de_set + p2->tx_bytes_de_set;
    p1->tx_frames_fecn_set = p1->tx_frames_fecn_set + p2->tx_frames_fecn_set;
    p1->tx_frames_becn_set = p1->tx_frames_becn_set + p2->tx_frames_becn_set;
    p1->tx_underrun = p1->tx_underrun + p2->tx_underrun;
    p1->tx_aborted_frames = p1->tx_aborted_frames + p2->tx_aborted_frames;
    p1->serial_dps_if_tx_frames = p1->serial_dps_if_tx_frames + p2->serial_dps_if_tx_frames;
    p1->serial_dps_if_tx_bytes = p1->serial_dps_if_tx_bytes + p2->serial_dps_if_tx_bytes;
    p1->serial_dps_if_rx_frames = p1->serial_dps_if_rx_frames + p2->serial_dps_if_rx_frames;
    p1->serial_dps_if_rx_bytes = p1->serial_dps_if_rx_bytes + p2->serial_dps_if_rx_bytes;
    p1->serial_dps_if_rx_dropped_frames = p1->serial_dps_if_rx_dropped_frames + p2->serial_dps_if_rx_dropped_frames;
    p1->serial_dps_if_rx_pce_denied_frames = p1->serial_dps_if_rx_pce_denied_frames + p2->serial_dps_if_rx_pce_denied_frames;
}

WP_status upi_stat_get_by_trunk(WP_U32 trunkid, STRU_UPI_STATICS *p)
{
    WP_status status;
    WP_handle device_handle;
    WP_U32 Groupid;
    WP_U16 i;
    NP_TRUNK_INFO * PtrunkInfo = npu_get_trunk_info(trunkid);
    WP_stats_pos_fr pos_fr,pos_fr_old;
    if(PtrunkInfo == NULL)
    {
        WDDI_WARNING(" can't get trunk info trunkid = %d \n ",trunkid);
        return -1;
    }
    if(PtrunkInfo->GroupNum > 31)
   	{
        Fprintf("!!!!!!\n");
        return -1;
   	}
    memset (&pos_fr, 0, sizeof(WP_stats_pos_fr));
    for(i = 0; i < (PtrunkInfo->GroupNum); i++)
    {
        Groupid = PtrunkInfo->Groupid[i];
        if(Groupid >31)
        {
            Fprintf("Groupid %d invalid \n",Groupid);
            return -1;
        }
        device_handle = ufe_device_handle[trunkid][Groupid];
        memset (&pos_fr_old, 0, sizeof(WP_stats_pos_fr));
        if(device_handle == 0)
        {
            continue;
        }
        status = WP_DeviceStatistics(device_handle,(void *)&pos_fr_old);
        if (status)
        {
            Fprintf("Error Reading UPI Stat\n");
            return status;
        }
        struct_pos_fr_add(&pos_fr, &pos_fr_old);
    }
    memcpy((void *)p,(void *)&pos_fr,sizeof(STRU_UPI_STATICS));
    return WP_OK;
}

WP_status upi_stat_get_by_group(WP_U32 trunkid, WP_U32 groupid,STRU_UPI_STATICS *p)
{
    WP_status status;
    WP_handle device_handle;
    WP_stats_pos_fr pos_fr;
    memset (&pos_fr, 0, sizeof(WP_stats_pos_fr));

    device_handle = ufe_device_handle[trunkid][groupid];
    memset (p, 0, sizeof(STRU_UPI_STATICS));
    if(device_handle == 0)
    {
        Fprintf("Error Reading UPI Stat device not exist! trunkid = %d groupid = %d\n");
        return -1;
    }
    status = WP_DeviceStatistics(device_handle, (void *)p);
    if (status)
    {
        Fprintf("Error Reading UPI Stat\n");
        return status;
    }
    return WP_OK;
}

WP_status upi_stm1_stat_get(WP_U32 stm1id,STRU_UPI_STATICS *p)
{
    WP_status status;
    WP_U16 i;
    STRU_UPI_STATICS pos_fr;
    STRU_SET_INITAIL_CONFIG *initial_cfg = npu_get_initial_cfg_info();
    WP_U32 line_index_base,line_index_counts;

    if(initial_cfg->stru_stm1_cfg_info[stm1id].E1T1type == NPU_MODE_E1)
    {
        line_index_base = stm1id*N_UFE_MAX_LINES_E1/2;
        line_index_counts = (1+stm1id)*N_UFE_MAX_LINES_E1/2;
    }
    else
    {
        line_index_base = stm1id*N_UFE_MAX_LINES_T1/2;
        line_index_counts = (1+stm1id)*N_UFE_MAX_LINES_T1/2;

    }

    if(NPU_VALID == initial_cfg->stru_stm1_cfg_info[stm1id].stm1Valid)
    {
        memset (&pos_fr, 0, sizeof(STRU_UPI_STATICS));
        for(i = line_index_base; i < line_index_counts; i ++)
        {
            if(NPU_VALID == initial_cfg->stru_stm1_cfg_info[stm1id].trunk_info[i-line_index_base].trunkValid)
            {
                memset(p, 0, sizeof(STRU_UPI_STATICS));
                status = upi_stat_get_by_trunk(i,p);
                if(-1 == status)
                {
                    continue;
                }
                if (status)
                {
                    Fprintf("Error upi_stat_get_by_trunk!\n");
                    return status;
                }
                struct_upi_statics_add(&pos_fr, p);
            }
        }
        memcpy((void *)p,(void *)&pos_fr,sizeof(pos_fr));
    }
    else
    {
        Fprintf("This stm1 %d is not configed!\n",stm1id);
        return -1;
    }
    return WP_OK;
}

WP_status enet_stat_get(WP_U32 ethno, void *p) {
    /*WP_stats_enet enet_stats;*/
    WP_status status;
    WP_handle enet_device;
    WP_U32 size;

    memset (p, 0, sizeof(WP_stats_enet));
    if(1 == ethno)
    {
        enet_device = gbedev1;
    }
    else if(2 == ethno)
    {
        enet_device = gbedev2;
    }
    status = WP_DeviceStatistics(enet_device, (WP_stats_enet *)p);
    if (status)
    {
        Fprintf("Error Reading Hs Enet Stat\n");
        return status;
    }
    return WP_OK;
}



void iw_port_stat(WP_handle iw_port) {
    WP_status status;
    //WP_route_port_stats iw_port_stats;
    WP_bport_stats iw_port_stats;
    if(iw_port == 0)
    {
        Fprintf("iw_port is 0 \n");
        return ;
    }
    status=WP_IwPortStatistics(iw_port, &iw_port_stats);
    terminate_on_error(status, "WP_IwPortStatistics()");

    Fprintf("Valid packets                           = %08x\n", (WP_U32)(iw_port_stats.rx_valid_packets & 0xffff));
    Fprintf("Discard VLAN acceptable filter          = %08x\n", (WP_U32)(iw_port_stats.discard_vlan_acceptable_filter & 0xffff));
    Fprintf("Discard Ingress filter                  = %08x\n", (WP_U32)(iw_port_stats.discard_ingress_filter & 0xffff));
    Fprintf("Discard Bridge filter                   = %08x\n", (WP_U32)(iw_port_stats.discard_bridge_classifier & 0xffff));
    Fprintf("Discard unknown MAC SA                  = %08x\n", (WP_U32)(iw_port_stats.discard_unk_macsa & 0xffff));
    Fprintf("Deny MAC SA                             = %08x\n", (WP_U32)(iw_port_stats.deny_mac_sa & 0xffff));
    Fprintf("Deny MAC DA                             = %08x\n", (WP_U32)(iw_port_stats.deny_mac_da & 0xffff));
    Fprintf("Discard Rx BC packets                   = %08x\n", (WP_U32)(iw_port_stats.rx_bc_valid_packets & 0xffff));
    Fprintf("Discard Rx MC packets                   = %08x\n", (WP_U32)(iw_port_stats.rx_mc_valid_packets & 0xffff));
    Fprintf("Forward UC packets                      = %08x\n", (WP_U32)(iw_port_stats.forwarded_uc_packets & 0xffff));
    Fprintf("Forward BC packets                      = %08x\n", (WP_U32)(iw_port_stats.forwarded_bc_packets & 0xffff));
    Fprintf("Forward MC packets                      = %08x\n", (WP_U32)(iw_port_stats.forwarded_mc_packets & 0xffff));
    Fprintf("Protocol error                          = %08x\n", (WP_U32)(iw_port_stats.protocol_error & 0xffff));
//WP_U64   discard_dfc_filter[WP_IW_DFC_MAX_NUM_OF_FILTERS];
    Fprintf("discard_dfc_filter[0]                   = %08x\n", (WP_U32)(iw_port_stats.discard_dfc_filter[0] & 0xffff));
    Fprintf("discard_dfc_filter[1]                   = %08x\n", (WP_U32)(iw_port_stats.discard_dfc_filter[1] & 0xffff));
    Fprintf("discard_dfc_filter[2]                   = %08x\n", (WP_U32)(iw_port_stats.discard_dfc_filter[2] & 0xffff));
    Fprintf("discard_dfc_filter[3]                   = %08x\n", (WP_U32)(iw_port_stats.discard_dfc_filter[3] & 0xffff));
    Fprintf("discard_dfc_filter[4]                   = %08x\n", (WP_U32)(iw_port_stats.discard_dfc_filter[4] & 0xffff));
    Fprintf("discard_dfc_filter[5]                   = %08x\n", (WP_U32)(iw_port_stats.discard_dfc_filter[5] & 0xffff));


    Fprintf("Deny CFM frames                         = %08x\n", (WP_U32)(iw_port_stats.deny_cfm_frames & 0xffff));
    Fprintf("Non valid IP version                    = %08x\n", (WP_U32)(iw_port_stats.bad_ip_version & 0xffff));
    Fprintf("Checksum error                          = %08x\n", (WP_U32)(iw_port_stats.checksum_error & 0xffff));
    Fprintf("IP protocol error                       = %08x\n", (WP_U32)(iw_port_stats.ip_protocol_error & 0xffff));
//WP_U64   discard_pce_filter[WP_PCE_MAX_NUM_OF_FILTERS];
    Fprintf("Ingress policer non conforming packets  = %08x\n", (WP_U32)(iw_port_stats.ingress_policer_non_conforming_packets & 0xffff));



/*  Fprintf("Valid packets                                = %08x\n", (WP_U32)(iw_port_stats.rx_valid_packets & 0xffff));
    Fprintf("IPv4 option packets                          = %08x\n", (WP_U32)(iw_port_stats.rx_ipv4_option_packets & 0xffff));
    Fprintf("Non IP packets                               = %08x\n", (WP_U32)(iw_port_stats.rx_non_ip_packets & 0xffff));
    Fprintf("Compressed packets                           = %08x\n", (WP_U32)(iw_port_stats.rx_compressed_packets & 0xffff));
    Fprintf("Valid MPLS packets                           = %08x\n", (WP_U32)(iw_port_stats.rx_valid_mpls_packets & 0xffff));
    Fprintf("Protocol errors	                           = %08x\n", (WP_U32)(iw_port_stats.rx_protocol_error & 0xffff));
    Fprintf("Checksum errors	                           = %08x\n", (WP_U32)(iw_port_stats.rx_checksum_error & 0xffff));
    Fprintf("Discard classifier                           = %08x\n", (WP_U32)(iw_port_stats.rx_discard_classifier & 0xffff));
    Fprintf("MPLS lookup drop                             = %08x\n", (WP_U32)(iw_port_stats.rx_mpls_lookup_drop & 0xffff));
    Fprintf("Tx forwarded packets                         = %08x\n", (WP_U32)(iw_port_stats.tx_forwarded_packets & 0xffff));
    Fprintf("IPv6 hop by hop host term                    = %08x\n", (WP_U32)(iw_port_stats.rx_ipv6_hop_by_hop_host_term & 0xffff));
    Fprintf("IPv6 link local host term                    = %08x\n", (WP_U32)(iw_port_stats.rx_ipv6_link_local_host_term & 0xffff));
*/

}

void StatIwFlowAggregation(WP_handle flow_handle)
{
    WP_iw_flow_stats  flow_stats;
    WP_status status;
    if(flow_handle == 0)
    {
        Fprintf("flow_handle is 0\n");
        return ;
    }
    memset (&flow_stats, 0, sizeof(flow_stats));
    status = WP_IwFlowStatistics(flow_handle, WP_IW_FLOW_STAT, &flow_stats);

    if (status)
    {
        Fprintf("Error Reading Flow Stats\n");
        exit(1);
    }

    Fprintf ("Forwarded Packets:                %08x%08x\n",
             ((WPE_StatField)flow_stats.forward_packet).part.high,
             ((WPE_StatField)flow_stats.forward_packet).part.low);
    Fprintf ("FBP Drop Packets:                 %08x%08x\n",
             ((WPE_StatField)flow_stats.fbp_drop_packets).part.high,
             ((WPE_StatField)flow_stats.fbp_drop_packets).part.low);
    Fprintf ("MTU Drop Packets:                 %08x%08x\n",
             ((WPE_StatField)flow_stats.mtu_drop_packets).part.high,
             ((WPE_StatField)flow_stats.mtu_drop_packets).part.low);
    Fprintf ("TTL Drop Packets:                 %08x%08x\n",
             ((WPE_StatField)flow_stats.ttl_drop_packets).part.high,
             ((WPE_StatField)flow_stats.ttl_drop_packets).part.low);
    Fprintf ("Tx Queue Drop Packets:            %08x%08x\n",
             ((WPE_StatField)flow_stats.tx_queue_drop_packets).part.high,
             ((WPE_StatField)flow_stats.tx_queue_drop_packets).part.low);
    Fprintf ("Denied Packets:                   %08x%08x\n",
             ((WPE_StatField)flow_stats.denied_packets).part.high,
             ((WPE_StatField)flow_stats.denied_packets).part.low);
    Fprintf ("Group Filtered Packets:           %08x%08x\n\n",
             ((WPE_StatField)flow_stats.group_filtered_packets).part.high,
             ((WPE_StatField)flow_stats.group_filtered_packets).part.low);
    Fprintf("----------------------------------------------------\n\n");

}

int npu_upi_stat_clear(WP_U32 trunkid,WP_U32 groupid)
{
	WP_handle device_handle;
	WP_stats_pos_fr pos_fr;
	WP_status status;
	device_handle = ufe_device_handle[trunkid][groupid];
	if(device_handle!= 0)
	{
		status = WP_DeviceStatisticsReset(device_handle,&pos_fr);
		if (status)
		{
            Fprintf("Error npu_upi_stat_clear UPI Stat\n");
            return status;
		}
	}
	else
	{
		Fprintf("invalid parameters npu_upi_stat_clear UPI Stat trunkid = %d groupid = %d\n",trunkid,groupid);
        return -1;
	}
    return 0;
}
int npu_fr_channel_stat_clear(WP_handle channelHandle ,int RxorTx)/*1 Rx ,0 Tx*/
{
    WP_status status;
	WP_stats_fr_tx fr_tx;
	WP_stats_fr_rx fr_rx;
	if(channelHandle == 0)return -1;
	if(RxorTx)
	{
		status = WP_ChannelStatisticsReset(channelHandle,(void *)&fr_rx);
	}
	else
	{
		status = WP_ChannelStatisticsReset(channelHandle,(void *)&fr_tx);
	}
}

WP_U32 iw_route_port_stat(WP_handle iw_port)
{
    WP_route_port_stats rport_stats = {0};
    WP_status status = 0;
    if(iw_port == 0) return -1;
    memset(&rport_stats, 0, sizeof(WP_route_port_stats));
    status = WP_IwPortStatistics(iw_port,&rport_stats);
    terminate_on_error(status, "WP_IwPortStatistics()");
    WPE_PrintIWPortStats(&rport_stats);
    return WP_OK;
}

void WPE_PrintIWPortStats(WP_route_port_stats* iwport_stats)
{
    Fprintf ("\nIW Port Statistics of route Port \n");
    Fprintf ("==========================================================\n");

    Fprintf("\n");

    Fprintf ("rx_valid_packets :                       %08x%08x\n",
             ((WPE_StatField)iwport_stats->rx_valid_packets).part.high,
             ((WPE_StatField)iwport_stats->rx_valid_packets).part.low);
    Fprintf ("rx_ipv4_option_packets :                 %08x%08x\n",
             ((WPE_StatField)iwport_stats->rx_ipv4_option_packets).part.high,
             ((WPE_StatField)iwport_stats->rx_ipv4_option_packets).part.low);
    Fprintf ("rx_non_ip_packets :                      %08x%08x\n",
             ((WPE_StatField)iwport_stats->rx_non_ip_packets).part.high,
             ((WPE_StatField)iwport_stats->rx_non_ip_packets).part.low);
    Fprintf ("rx_compressed_packets :                  %08x%08x\n",
             ((WPE_StatField)iwport_stats->rx_compressed_packets).part.high,
             ((WPE_StatField)iwport_stats->rx_compressed_packets).part.low);
    Fprintf ("rx_valid_mpls_packets :                  %08x%08x\n",
             ((WPE_StatField)iwport_stats->rx_valid_mpls_packets).part.high,
             ((WPE_StatField)iwport_stats->rx_valid_mpls_packets).part.low);
    Fprintf ("rx_protocol_error :                      %08x%08x\n",
             ((WPE_StatField)iwport_stats->rx_protocol_error).part.high,
             ((WPE_StatField)iwport_stats->rx_protocol_error).part.low);
    Fprintf ("rx_checksum_error :                      %08x%08x\n",
             ((WPE_StatField)iwport_stats->rx_checksum_error).part.high,
             ((WPE_StatField)iwport_stats->rx_checksum_error).part.low);
    Fprintf ("rx_discard_classifier :                  %08x%08x\n",
             ((WPE_StatField)iwport_stats->rx_discard_classifier).part.high,
             ((WPE_StatField)iwport_stats->rx_discard_classifier).part.low);
    Fprintf ("rx_mpls_lookup_drop :                    %08x%08x\n",
             ((WPE_StatField)iwport_stats->rx_mpls_lookup_drop).part.high,
             ((WPE_StatField)iwport_stats->rx_mpls_lookup_drop).part.low);
    Fprintf ("tx_forwarded_packets :                   %08x%08x\n",
             ((WPE_StatField)iwport_stats->tx_forwarded_packets).part.high,
             ((WPE_StatField)iwport_stats->tx_forwarded_packets).part.low);
    Fprintf ("rx_ipv6_hop_by_hop_host_term :           %08x%08x\n",
             ((WPE_StatField)iwport_stats->rx_ipv6_hop_by_hop_host_term).part.high,
             ((WPE_StatField)iwport_stats->rx_ipv6_hop_by_hop_host_term).part.low);
    Fprintf ("rx_ipv6_link_local_host_term :           %08x%08x\n",
             ((WPE_StatField)iwport_stats->rx_ipv6_link_local_host_term).part.high,
             ((WPE_StatField)iwport_stats->rx_ipv6_link_local_host_term).part.low);
    Fprintf ("rx_discard_lpm :                         %08x%08x\n",
             ((WPE_StatField)iwport_stats->rx_discard_lpm).part.high,
             ((WPE_StatField)iwport_stats->rx_discard_lpm).part.low);
    Fprintf ("discard_dfc_filter[0] :                  %08x%08x\n",
             ((WPE_StatField)iwport_stats->discard_dfc_filter[0]).part.high,
             ((WPE_StatField)iwport_stats->discard_dfc_filter[0]).part.low);
    Fprintf ("discard_dfc_filter[1] :                  %08x%08x\n",
             ((WPE_StatField)iwport_stats->discard_dfc_filter[1]).part.high,
             ((WPE_StatField)iwport_stats->discard_dfc_filter[1]).part.low);
    Fprintf ("ingress_policer_non_conforming_packets : %08x%08x\n",
             ((WPE_StatField)iwport_stats->ingress_policer_non_conforming_packets).part.high,
             ((WPE_StatField)iwport_stats->ingress_policer_non_conforming_packets).part.low);

    Fprintf("\n");
    return;
}


void np_bfd_flow_statistics(WP_handle BfdAggHandle)
{
	WP_bfd_flow_agg_statistics bfd_flow_agg_statistics;
	WP_status status;

    if(BfdAggHandle == 0)
    {
        Fprintf("BfdAggHandle is NULL \n");
        return ;
    }
    memset(&bfd_flow_agg_statistics,0,sizeof( bfd_flow_agg_statistics));
    status = WP_IwFlowStatistics(BfdAggHandle,WP_IW_FLOW_STAT_BFD,&bfd_flow_agg_statistics);
    terminate_on_error(status, "WP_IwFlowStatistics()");

    Fprintf(" \n\n BFD flow statistics : \n\n ");
    Fprintf(" enqueued_packets = %08x%08x  \n ",
			((WPE_StatField)bfd_flow_agg_statistics.enqueued_packets).part.high,
			((WPE_StatField)bfd_flow_agg_statistics.enqueued_packets).part.low);

    Fprintf("version_error = %08x%08x \n ",
			((WPE_StatField)bfd_flow_agg_statistics.version_error).part.high,
			((WPE_StatField)bfd_flow_agg_statistics.version_error).part.low);
    Fprintf("multi_point_error = %08x%08x \n",
			((WPE_StatField)bfd_flow_agg_statistics.multi_point_error).part.high,
			((WPE_StatField)bfd_flow_agg_statistics.multi_point_error).part.low);

    Fprintf(" fbp_drop_packets = %08x%08x \n",
			((WPE_StatField)bfd_flow_agg_statistics.fbp_drop_packets).part.high,
			((WPE_StatField)bfd_flow_agg_statistics.fbp_drop_packets).part.low);

    Fprintf("mtu_drop_packets = %08x%08x \n ",
			((WPE_StatField)bfd_flow_agg_statistics.mtu_drop_packets).part.high,
			((WPE_StatField)bfd_flow_agg_statistics.mtu_drop_packets).part.low);

    Fprintf("detection_multiplier_error = %08x%08x \n ",
			((WPE_StatField)bfd_flow_agg_statistics.detection_multiplier_error).part.high,
			((WPE_StatField)bfd_flow_agg_statistics.detection_multiplier_error).part.low);

    Fprintf("tx_queue_drop_packets = %08x%08x \n ",
			((WPE_StatField)bfd_flow_agg_statistics.tx_queue_drop_packets).part.high,
			((WPE_StatField)bfd_flow_agg_statistics.tx_queue_drop_packets).part.low);

    Fprintf("your_discriminator_and_state_error = %08x%08x \n ",
			((WPE_StatField)bfd_flow_agg_statistics.your_discriminator_and_state_error).part.high,
			((WPE_StatField)bfd_flow_agg_statistics.your_discriminator_and_state_error).part.low);

    Fprintf("my_discriminator_error = %08x%08x \n ",
			((WPE_StatField)bfd_flow_agg_statistics.my_discriminator_error).part.high,
			((WPE_StatField)bfd_flow_agg_statistics.my_discriminator_error).part.low);

    Fprintf("length_too_short_error  = %08x%08x \n ",
			((WPE_StatField)bfd_flow_agg_statistics.length_too_short_error).part.high,
			((WPE_StatField)bfd_flow_agg_statistics.length_too_short_error).part.low);

    Fprintf("length_too_long_error	= %08x%08x \n ",
			((WPE_StatField)bfd_flow_agg_statistics.length_too_long_error).part.high,
			((WPE_StatField)bfd_flow_agg_statistics.length_too_long_error).part.low);

    Fprintf("discriminator_not_in_use_error = %08x%08x \n",
			((WPE_StatField)bfd_flow_agg_statistics.discriminator_not_in_use_error).part.high,
			((WPE_StatField)bfd_flow_agg_statistics.discriminator_not_in_use_error).part.low);

    Fprintf(" last_discarded_discriminator = %08x%08x \n",
			((WPE_StatField)bfd_flow_agg_statistics.last_discarded_discriminator).part.high,
			((WPE_StatField)bfd_flow_agg_statistics.last_discarded_discriminator).part.low);
    return;
}
void np_bfd_sesstion_statistics(WP_handle rx_table_handle)
{
    WP_status status;
	WP_bfd_session_statistics session_statistics ;
	if(rx_table_handle == 0)return;
	status = WP_IwBfdSessionStatistics(rx_table_handle,&session_statistics);
	terminate_on_error(status, "WP_IwBfdSessionStatistics()");
	Fprintf(" \n\n session statistics:\n");
	Fprintf(" received_packets = %08x%08x  \n authentication_failures = %08x%08x \n timeout_expiries = %08x%08x \n",
            ((WPE_StatField)session_statistics.received_packets).part.high,
            ((WPE_StatField)session_statistics.received_packets).part.low,
            ((WPE_StatField)session_statistics.authentication_failures).part.high,
            ((WPE_StatField)session_statistics.authentication_failures).part.low,
            ((WPE_StatField)session_statistics.timeout_expiries).part.high,
            ((WPE_StatField)session_statistics.timeout_expiries).part.low);
    return ;
}
