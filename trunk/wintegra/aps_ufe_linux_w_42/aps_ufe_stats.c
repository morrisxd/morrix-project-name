/*****************************************************************************
 * (C) Copyright PMC - WIN division (Wintegra) 2011.  All rights reserved.
 * PMC-Sierra (WINTEGRA) CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of PMC-Sierra (Wintegra).
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *  Application: Automatic Protection Switch
 *  File Name   : auto_protection_switch_stats.c
 *  Version     : 1
 *  Date        : 20 Jan 2012
 *  Accompanies : TBD
 *****************************************************************************/

/****************************************************************************************************************************
* Function name: App_ShowDeviceStats()
* Description  : display device statistics  
*
*
* Input  params: none
* Output params: 
* Return val   : none
*****************************************************************************************************************************/


void App_ShowDeviceStats (void)
{
    WP_status status;

    WP_stats_enet s_hs_enet = {
        0
    };

    memset (&s_hs_enet, 0, sizeof (s_hs_enet));
    printf ("----------------------------------------------------------\n");
    printf (" ENET1 statistics \n");
    printf ("----------------------------------------------------------\n");
    status = WP_DeviceStatistics (h_device_enet1, &s_hs_enet);
    App_TerminateOnError (status, "WP_DeviceStatistics");
    printf (" txrx_frames_64            : %08d\n", (WP_U32) s_hs_enet.txrx_frames_64);
    printf (" txrx_frames_127           : %08d\n", (WP_U32) s_hs_enet.txrx_frames_127);
    printf (" txrx_frames_255           : %08d\n", (WP_U32) s_hs_enet.txrx_frames_255);
    printf (" rx_bytes                  : %08d\n", (WP_U32) s_hs_enet.rx_bytes);
    printf (" rx_packets                : %08d\n", (WP_U32) s_hs_enet.rx_packets);
    printf (" rx_err_fcs                : %08d\n", (WP_U32) s_hs_enet.rx_err_fcs);
    printf (" rx_multicast              : %08d\n", (WP_U32) s_hs_enet.rx_multicast);
    printf (" rx_dropped                : %08d\n\n", (WP_U32) s_hs_enet.rx_dropped);
    printf (" tx_bytes                  : %08d\n", (WP_U32) s_hs_enet.tx_bytes);
    printf (" tx_packets                : %08d\n", (WP_U32) s_hs_enet.tx_packets);
    printf (" tx_multicast              : %08d\n", (WP_U32) s_hs_enet.tx_multicast);
    printf (" tx_broadcast              : %08d\n", (WP_U32) s_hs_enet.tx_broadcast);
    printf (" tx_mac_pause              : %08d\n", (WP_U32) s_hs_enet.tx_mac_pause);
    printf (" tx_defer                  : %08d\n", (WP_U32) s_hs_enet.tx_defer);
    printf (" tx_frames                 : %08d\n", (WP_U32) s_hs_enet.tx_frames);
    WP_DeviceStatisticsReset (h_device_enet1, &s_hs_enet);
    memset (&s_hs_enet, 0, sizeof (s_hs_enet));
    printf ("-----------------------------------------------------------------\n");
    printf (" ENET2 statisics \n");
    printf ("-----------------------------------------------------------------\n");
    status = WP_DeviceStatistics (h_device_enet2, &s_hs_enet);
    App_TerminateOnError (status, "WP_DeviceStatistics");
    printf (" txrx_frames_64            : %08d\n", (WP_U32) s_hs_enet.txrx_frames_64);
    printf (" txrx_frames_127           : %08d\n", (WP_U32) s_hs_enet.txrx_frames_127);
    printf (" txrx_frames_255           : %08d\n", (WP_U32) s_hs_enet.txrx_frames_255);
    printf (" rx_bytes                  : %08d\n", (WP_U32) s_hs_enet.rx_bytes);
    printf (" rx_packets                : %08d\n", (WP_U32) s_hs_enet.rx_packets);
    printf (" rx_err_fcs                : %08d\n", (WP_U32) s_hs_enet.rx_err_fcs);
    printf (" rx_multicast              : %08d\n", (WP_U32) s_hs_enet.rx_multicast);
    printf (" rx_dropped                : %08d\n\n", (WP_U32) s_hs_enet.rx_dropped);
    printf (" tx_bytes                  : %08d\n", (WP_U32) s_hs_enet.tx_bytes);
    printf (" tx_packets                : %08d\n", (WP_U32) s_hs_enet.tx_packets);
    printf (" tx_multicast              : %08d\n", (WP_U32) s_hs_enet.tx_multicast);
    printf (" tx_broadcast              : %08d\n", (WP_U32) s_hs_enet.tx_broadcast);
    printf (" tx_mac_pause              : %08d\n", (WP_U32) s_hs_enet.tx_mac_pause);
    printf (" tx_defer                  : %08d\n", (WP_U32) s_hs_enet.tx_defer);
    printf (" tx_frames                 : %08d\n", (WP_U32) s_hs_enet.tx_frames);
    WP_DeviceStatisticsReset (h_device_enet2, &s_hs_enet);

}

/****************************************************************************************************************************
* Function name: App_ShowBportStats()
* Description  : display all ports statistics  
*
*
* Input  params: none
* Output params: 
* Return val   : none
*****************************************************************************************************************************/
void App_ShowBportStats (void)
{
    WP_status status;

    WP_bport_stats bport_stats = {
        0
    };
    WP_U32 ii;

    printf ("--------------------------------------------------------\n");
    printf ("Bridge Port Statistics ENET1 \n");
    printf ("--------------------------------------------------------\n");
    memset (&bport_stats, 0, sizeof (bport_stats));
    status = WP_IwPortStatistics (h_bridge1_bport_enet1, &bport_stats);
    App_TerminateOnError (status, "WP_BportStatistics");
    printf ("   %-39s : %d\n", "rx_valid_packets", (WP_U32) bport_stats.rx_valid_packets);
    printf ("   %-39s : %d\n", "discard_vlan_acceptable_filter", (WP_U32) bport_stats.discard_vlan_acceptable_filter);
    printf ("   %-39s : %d\n", "discard_ingress_filter", (WP_U32) bport_stats.discard_ingress_filter);
    printf ("   %-39s : %d\n", "discard_bridge_classifier", (WP_U32) bport_stats.discard_bridge_classifier);
    printf ("   %-39s : %d\n", "discard_unk_macsa", (WP_U32) bport_stats.discard_unk_macsa);
    printf ("   %-39s : %d\n", "deny_mac_sa", (WP_U32) bport_stats.deny_mac_sa);
    printf ("   %-39s : %d\n", "deny_mac_da", (WP_U32) bport_stats.deny_mac_da);
    printf ("   %-39s : %d\n", "rx_bc_valid_packets", (WP_U32) bport_stats.rx_bc_valid_packets);
    printf ("   %-39s : %d\n", "rx_mc_valid_packets", (WP_U32) bport_stats.rx_mc_valid_packets);
    printf ("   %-39s : %d\n", "forwarded_uc_packets", (WP_U32) bport_stats.forwarded_uc_packets);
    printf ("   %-39s : %d\n", "forwarded_bc_packets", (WP_U32) bport_stats.forwarded_bc_packets);
    printf ("   %-39s : %d\n", "forwarded_mc_packets", (WP_U32) bport_stats.forwarded_mc_packets);
    printf ("   %-39s : %d\n", "protocol_error", (WP_U32) bport_stats.protocol_error);
    printf ("   %-39s : %d\n", "discard_pce_filter[0]", (WP_U32) bport_stats.discard_pce_filter[0]);
    printf ("   %-39s : %d\n", "ingress_policer_non_conforming_packets", (WP_U32) bport_stats.ingress_policer_non_conforming_packets);
    printf ("   %-39s : %d\n", "rx_discard_lpm", (WP_U32) bport_stats.rx_discard_lpm);
    printf ("   %-39s : %d\n", "iw_mirror_fbp_underrun", (WP_U32) bport_stats.iw_mirror_fbp_underrun);
    printf ("   %-39s : %d\n", "iw_mirror_iwbg_fbp_underrun", (WP_U32) bport_stats.iw_mirror_iwbg_fbp_underrun);
    WP_IwPortStatisticsReset (h_bridge1_bport_enet1, &bport_stats);
    for (ii = 0; ii < APP_MAX_PORT_HDLC; ii++)

    {
        printf ("-------------------------------------------------------------\n");
        printf ("Bridge Port Statistics HDLC %d\n", ii);
        printf ("-------------------------------------------------------------\n");
        memset (&bport_stats, 0, sizeof (bport_stats));
        status = WP_IwPortStatistics (h_bridge1_bport_hdlc[ii], &bport_stats);
        App_TerminateOnError (status, "BportStatistics");
        printf ("   %-39s : %d\n", "rx_valid_packets", (WP_U32) bport_stats.rx_valid_packets);
        printf ("   %-39s : %d\n", "discard_vlan_acceptable_filter", (WP_U32) bport_stats.discard_vlan_acceptable_filter);
        printf ("   %-39s : %d\n", "discard_ingress_filter", (WP_U32) bport_stats.discard_ingress_filter);
        printf ("   %-39s : %d\n", "discard_bridge_classifier", (WP_U32) bport_stats.discard_bridge_classifier);
        printf ("   %-39s : %d\n", "discard_unk_macsa", (WP_U32) bport_stats.discard_unk_macsa);
        printf ("   %-39s : %d\n", "deny_mac_sa", (WP_U32) bport_stats.deny_mac_sa);
        printf ("   %-39s : %d\n", "deny_mac_da", (WP_U32) bport_stats.deny_mac_da);
        printf ("   %-39s : %d\n", "rx_bc_valid_packets", (WP_U32) bport_stats.rx_bc_valid_packets);
        printf ("   %-39s : %d\n", "rx_mc_valid_packets", (WP_U32) bport_stats.rx_mc_valid_packets);
        printf ("   %-39s : %d\n", "forwarded_uc_packets", (WP_U32) bport_stats.forwarded_uc_packets);
        printf ("   %-39s : %d\n", "forwarded_bc_packets", (WP_U32) bport_stats.forwarded_bc_packets);
        printf ("   %-39s : %d\n", "forwarded_mc_packets", (WP_U32) bport_stats.forwarded_mc_packets);
        printf ("   %-39s : %d\n", "protocol_error", (WP_U32) bport_stats.protocol_error);
        printf ("   %-39s : %d\n", "discard_pce_filter[0]", (WP_U32) bport_stats.discard_pce_filter[0]);
        printf ("   %-39s : %d\n", "ingress_policer_non_conforming_packets", (WP_U32) bport_stats.ingress_policer_non_conforming_packets);
        printf ("   %-39s : %d\n", "rx_discard_lpm", (WP_U32) bport_stats.rx_discard_lpm);
        printf ("   %-39s : %d\n", "iw_mirror_fbp_underrun", (WP_U32) bport_stats.iw_mirror_fbp_underrun);
        printf ("   %-39s : %d\n", "iw_mirror_iwbg_fbp_underrun", (WP_U32) bport_stats.iw_mirror_iwbg_fbp_underrun);
        WP_IwPortStatisticsReset (h_bridge1_bport_hdlc[ii], &bport_stats);
    }
    for (ii = 0; ii < APP_MAX_PORT_HDLC; ii++)

    {
        printf ("---------------------------------------------------------------\n");
        printf ("Bridge2 Port Statistics HDLC %d\n", ii);
        printf ("---------------------------------------------------------------\n");
        memset (&bport_stats, 0, sizeof (bport_stats));
        status = WP_IwPortStatistics (h_bridge2_bport_hdlc[ii], &bport_stats);
        App_TerminateOnError (status, "BportStatistics");
        printf ("   %-39s : %d\n", "rx_valid_packets", (WP_U32) bport_stats.rx_valid_packets);
        printf ("   %-39s : %d\n", "discard_vlan_acceptable_filter", (WP_U32) bport_stats.discard_vlan_acceptable_filter);
        printf ("   %-39s : %d\n", "discard_ingress_filtern", (WP_U32) bport_stats.discard_ingress_filter);
        printf ("   %-39s : %d\n", "discard_bridge_classifier", (WP_U32) bport_stats.discard_bridge_classifier);
        printf ("   %-39s : %d\n", "discard_unk_macsa", (WP_U32) bport_stats.discard_unk_macsa);
        printf ("   %-39s : %d\n", "deny_mac_sa", (WP_U32) bport_stats.deny_mac_sa);
        printf ("   %-39s : %d\n", "deny_mac_da", (WP_U32) bport_stats.deny_mac_da);
        printf ("   %-39s : %d\n", "rx_bc_valid_packets", (WP_U32) bport_stats.rx_bc_valid_packets);
        printf ("   %-39s : %d\n", "rx_mc_valid_packets", (WP_U32) bport_stats.rx_mc_valid_packets);
        printf ("   %-39s : %d\n", "forwarded_uc_packets", (WP_U32) bport_stats.forwarded_uc_packets);
        printf ("   %-39s : %d\n", "forwarded_bc_packets", (WP_U32) bport_stats.forwarded_bc_packets);
        printf ("   %-39s : %d\n", "forwarded_mc_packets", (WP_U32) bport_stats.forwarded_mc_packets);
        printf ("   %-39s : %d\n", "protocol_error", (WP_U32) bport_stats.protocol_error);
        printf ("   %-39s : %d\n", "discard_pce_filter[0]", (WP_U32) bport_stats.discard_pce_filter[0]);
        printf ("   %-39s : %d\n", "ingress_policer_non_conforming_packets", (WP_U32) bport_stats.ingress_policer_non_conforming_packets);
        printf ("   %-39s : %d\n", "rx_discard_lpm", (WP_U32) bport_stats.rx_discard_lpm);
        printf ("   %-39s : %d\n", "iw_mirror_fbp_underrun", (WP_U32) bport_stats.iw_mirror_fbp_underrun);
        printf ("   %-39s : %d\n", "iw_mirror_iwbg_fbp_underrun", (WP_U32) bport_stats.iw_mirror_iwbg_fbp_underrun);
        WP_IwPortStatisticsReset (h_bridge2_bport_hdlc[ii], &bport_stats);
    }
    printf ("-------------------------------------------------------------------\n");
    printf ("Bridge2 Port Enet2:       \n");
    printf ("-------------------------------------------------------------------\n");
    memset (&bport_stats, 0, sizeof (bport_stats));
    status = WP_IwPortStatistics (h_bridge2_bport_enet2, &bport_stats);
    App_TerminateOnError (status, "BportStatistics");
    printf ("   %-39s : %d\n", "rx_valid_packets", (WP_U32) bport_stats.rx_valid_packets);
    printf ("   %-39s : %d\n", "discard_vlan_acceptable_filter", (WP_U32) bport_stats.discard_vlan_acceptable_filter);
    printf ("   %-39s : %d\n", "discard_ingress_filter", (WP_U32) bport_stats.discard_ingress_filter);
    printf ("   %-39s : %d\n", "discard_bridge_classifier", (WP_U32) bport_stats.discard_bridge_classifier);
    printf ("   %-39s : %d\n", "discard_unk_macsa", (WP_U32) bport_stats.discard_unk_macsa);
    printf ("   %-39s : %d\n", "deny_mac_sa", (WP_U32) bport_stats.deny_mac_sa);
    printf ("   %-39s : %d\n", "deny_mac_da", (WP_U32) bport_stats.deny_mac_da);
    printf ("   %-39s : %d\n", "rx_bc_valid_packets", (WP_U32) bport_stats.rx_bc_valid_packets);
    printf ("   %-39s : %d\n", "rx_mc_valid_packets", (WP_U32) bport_stats.rx_mc_valid_packets);
    printf ("   %-39s : %d\n", "forwarded_uc_packets", (WP_U32) bport_stats.forwarded_uc_packets);
    printf ("   %-39s : %d\n", "forwarded_bc_packets", (WP_U32) bport_stats.forwarded_bc_packets);
    printf ("   %-39s : %d\n", "forwarded_mc_packets", (WP_U32) bport_stats.forwarded_mc_packets);
    printf ("   %-39s : %d\n", "protocol_error", (WP_U32) bport_stats.protocol_error);
    printf ("   %-39s : %d\n", "discard_pce_filter[0]", (WP_U32) bport_stats.discard_pce_filter[0]);
    printf ("   %-39s : %d\n", "ingress_policer_non_conforming_packets", (WP_U32) bport_stats.ingress_policer_non_conforming_packets);
    printf ("   %-39s : %d\n", "rx_discard_lpm", (WP_U32) bport_stats.rx_discard_lpm);
    printf ("   %-39s : %d\n", "iw_mirror_fbp_underrun", (WP_U32) bport_stats.iw_mirror_fbp_underrun);
    printf ("   %-39s : %d\n", "iw_mirror_iwbg_fbp_underrun", (WP_U32) bport_stats.iw_mirror_iwbg_fbp_underrun);
    WP_IwPortStatisticsReset (h_bridge2_bport_enet2, &bport_stats);
}

/****************************************************************************************************************************
* Function name: App_ShowFlowAggStats()
* Description  : display all flow agg. statistics  
*
*
* Input  params: none
* Output params: 
* Return val   : none
*****************************************************************************************************************************/
void App_ShowFlowAggStats (void)
{
    WP_status status;

    WP_iw_flow_stats flow_stats = {
        0
    };
    WP_U32 ii;

    for (ii = 0; ii < APP_MAX_FLOW_AGG_HDLC; ii++)

    {
        printf ("-----------------------------------------------------------------\n");
        printf (" Flow statistics HDLC %d\n", ii);
        printf ("-----------------------------------------------------------------\n");
        memset (&flow_stats, 0, sizeof (flow_stats));
        status = WP_IwFlowStatistics (h_flow_agg_hdlc[ii], WP_IW_FLOW_STAT, &flow_stats);
        App_TerminateOnError (status, "WP_IwFlowStatistics");
        printf ("   %-33s : %d\n", "forward_packet", (WP_U32) flow_stats.forward_packet);
        printf ("   %-33s : %d\n", "fbp_drop_packets", (WP_U32) flow_stats.fbp_drop_packets);
        printf ("   %-33s : %d\n", "mtu_drop_packets", (WP_U32) flow_stats.mtu_drop_packets);
        printf ("   %-33s : %d\n", "ttl_drop_packets", (WP_U32) flow_stats.ttl_drop_packets);
        printf ("   %-33s : %d\n", "tx_queue_drop_packets", (WP_U32) flow_stats.tx_queue_drop_packets);
        printf ("   %-33s : %d\n", "mpls_drop", (WP_U32) flow_stats.mpls_drop);
        printf ("   %-33s : %d\n", "denied_packets", (WP_U32) flow_stats.denied_packets);
        printf ("   %-33s : %d\n", "group_filtered_packets", (WP_U32) flow_stats.group_filtered_packets);
        printf ("   %-33s : %d\n", "forwarded_bytes", (WP_U32) flow_stats.forwarded_bytes);
        printf ("   %-33s : %d\n", "gtp_bad_headers", (WP_U32) flow_stats.gtp_bad_headers);
        printf ("   %-33s : %d\n", "policer_non_conforming_packets", (WP_U32) flow_stats.policer_non_conforming_packets);
        printf ("   %-33s : %d\n", "cfm_mac_in_mac_drop", (WP_U32) flow_stats.cfm_mac_in_mac_drop);
        printf ("   %-33s : %d\n", "out_of_sequence_packets", (WP_U32) flow_stats.out_of_sequence_packets);
        printf ("   %-33s : %d\n", "pwe_switchover_events", (WP_U32) flow_stats.pwe_switchover_events);
        printf ("   %-33s : %d\n", "non_zero_seq_num", (WP_U32) flow_stats.non_zero_seq_num);
        WP_IwFlowStatisticsReset (h_flow_agg_hdlc[ii], WP_IW_FLOW_STAT, &flow_stats);
    }
    for (ii = 0; ii < HDLC_NUM; ii++)

    {
        printf ("--------------------------------------------------------------------\n");
        printf (" Flow statistics PPPSW %d\n", ii);
        printf ("--------------------------------------------------------------------\n");
        memset (&flow_stats, 0, sizeof (flow_stats));
        status = WP_IwFlowStatistics (h_flow_agg_pppsw_link[ii], WP_IW_FLOW_STAT, &flow_stats);
        App_TerminateOnError (status, "WP_IwFlowStatistics");
        printf ("   %-33s : %d\n", "forward_packet", (WP_U32) flow_stats.forward_packet);
        printf ("   %-33s : %d\n", "fbp_drop_packets", (WP_U32) flow_stats.fbp_drop_packets);
        printf ("   %-33s : %d\n", "mtu_drop_packets", (WP_U32) flow_stats.mtu_drop_packets);
        printf ("   %-33s : %d\n", "ttl_drop_packets", (WP_U32) flow_stats.ttl_drop_packets);
        printf ("   %-33s : %d\n", "tx_queue_drop_packets", (WP_U32) flow_stats.tx_queue_drop_packets);
        printf ("   %-33s : %d\n", "mpls_drop", (WP_U32) flow_stats.mpls_drop);
        printf ("   %-33s : %d\n", "denied_packets", (WP_U32) flow_stats.denied_packets);
        printf ("   %-33s : %d\n", "group_filtered_packets", (WP_U32) flow_stats.group_filtered_packets);
        printf ("   %-33s : %d\n", "forwarded_bytes", (WP_U32) flow_stats.forwarded_bytes);
        printf ("   %-33s : %d\n", "gtp_bad_headers", (WP_U32) flow_stats.gtp_bad_headers);
        printf ("   %-33s : %d\n", "policer_non_conforming_packets", (WP_U32) flow_stats.policer_non_conforming_packets);
        printf ("   %-33s : %d\n", "cfm_mac_in_mac_drop", (WP_U32) flow_stats.cfm_mac_in_mac_drop);
        printf ("   %-33s : %d\n", "out_of_sequence_packets", (WP_U32) flow_stats.out_of_sequence_packets);
        printf ("   %-33s : %d\n", "pwe_switchover_events", (WP_U32) flow_stats.pwe_switchover_events);
        printf ("   %-33s : %d\n", "non_zero_seq_num", (WP_U32) flow_stats.non_zero_seq_num);
        WP_IwFlowStatisticsReset (h_flow_agg_pppsw_link[ii], WP_IW_FLOW_STAT, &flow_stats);
    }
    printf ("------------------------------------------------------------------------\n");
    printf (" Flow statistics Cisco Control \n");
    printf ("------------------------------------------------------------------------\n");
    memset (&flow_stats, 0, sizeof (flow_stats));
    status = WP_IwFlowStatistics (h_flow_agg_control, WP_IW_FLOW_STAT, &flow_stats);
    App_TerminateOnError (status, "WP_IwFlowStatistics");
    printf ("   %-33s : %d\n", "forward_packet", (WP_U32) flow_stats.forward_packet);
    printf ("   %-33s : %d\n", "fbp_drop_packets", (WP_U32) flow_stats.fbp_drop_packets);
    printf ("   %-33s : %d\n", "mtu_drop_packets", (WP_U32) flow_stats.mtu_drop_packets);
    printf ("   %-33s : %d\n", "ttl_drop_packets", (WP_U32) flow_stats.ttl_drop_packets);
    printf ("   %-33s : %d\n", "tx_queue_drop_packets", (WP_U32) flow_stats.tx_queue_drop_packets);
    printf ("   %-33s : %d\n", "mpls_drop", (WP_U32) flow_stats.mpls_drop);
    printf ("   %-33s : %d\n", "denied_packets", (WP_U32) flow_stats.denied_packets);
    printf ("   %-33s : %d\n", "group_filtered_packets", (WP_U32) flow_stats.group_filtered_packets);
    printf ("   %-33s : %d\n", "forwarded_bytes", (WP_U32) flow_stats.forwarded_bytes);
    printf ("   %-33s : %d\n", "gtp_bad_headers", (WP_U32) flow_stats.gtp_bad_headers);
    printf ("   %-33s : %d\n", "policer_non_conforming_packets", (WP_U32) flow_stats.policer_non_conforming_packets);
    printf ("   %-33s : %d\n", "cfm_mac_in_mac_drop", (WP_U32) flow_stats.cfm_mac_in_mac_drop);
    printf ("   %-33s : %d\n", "out_of_sequence_packets", (WP_U32) flow_stats.out_of_sequence_packets);
    printf ("   %-33s : %d\n", "pwe_switchover_events", (WP_U32) flow_stats.pwe_switchover_events);
    printf ("   %-33s : %d\n", "non_zero_seq_num", (WP_U32) flow_stats.non_zero_seq_num);
    WP_IwFlowStatisticsReset (h_flow_agg_control, WP_IW_FLOW_STAT, &flow_stats);

    for (ii = 0; ii < APP_MAX_FLOW_AGG_HDLC; ii++)

    {
        printf ("------------------------------------------------------------------------\n");
        printf (" Flow statistics ENET2 FA[%d]\n", ii);
        printf ("------------------------------------------------------------------------\n");
        memset (&flow_stats, 0, sizeof (flow_stats));
        status = WP_IwFlowStatistics (h_flow_agg_enet2[ii], WP_IW_FLOW_STAT, &flow_stats);
        App_TerminateOnError (status, "WP_IwFlowStatistics");
        printf ("   %-33s : %d\n", "forward_packet", (WP_U32) flow_stats.forward_packet);
        printf ("   %-33s : %d\n", "fbp_drop_packets", (WP_U32) flow_stats.fbp_drop_packets);
        printf ("   %-33s : %d\n", "mtu_drop_packets", (WP_U32) flow_stats.mtu_drop_packets);
        printf ("   %-33s : %d\n", "ttl_drop_packets", (WP_U32) flow_stats.ttl_drop_packets);
        printf ("   %-33s : %d\n", "tx_queue_drop_packets", (WP_U32) flow_stats.tx_queue_drop_packets);
        printf ("   %-33s : %d\n", "mpls_drop", (WP_U32) flow_stats.mpls_drop);
        printf ("   %-33s : %d\n", "denied_packets", (WP_U32) flow_stats.denied_packets);
        printf ("   %-33s : %d\n", "group_filtered_packets", (WP_U32) flow_stats.group_filtered_packets);
        printf ("   %-33s : %d\n", "forwarded_bytes", (WP_U32) flow_stats.forwarded_bytes);
        printf ("   %-33s : %d\n", "gtp_bad_headers", (WP_U32) flow_stats.gtp_bad_headers);
        printf ("   %-33s : %d\n", "policer_non_conforming_packets", (WP_U32) flow_stats.policer_non_conforming_packets);
        printf ("   %-33s : %d\n", "cfm_mac_in_mac_drop", (WP_U32) flow_stats.cfm_mac_in_mac_drop);
        printf ("   %-33s : %d\n", "out_of_sequence_packets", (WP_U32) flow_stats.out_of_sequence_packets);
        printf ("   %-33s : %d\n", "pwe_switchover_events", (WP_U32) flow_stats.pwe_switchover_events);
        printf ("   %-33s : %d\n", "non_zero_seq_num", (WP_U32) flow_stats.non_zero_seq_num);
        WP_IwFlowStatisticsReset (h_flow_agg_enet2[ii], WP_IW_FLOW_STAT, &flow_stats);
    }
    printf ("-----------------------------------------------------------------------\n");
    printf (" Flow statisics Default Bridge  \n");
    printf ("-----------------------------------------------------------------------\n");
    memset (&flow_stats, 0, sizeof (flow_stats));
    status = WP_IwFlowStatistics (h_bridge1_default_agg, WP_IW_FLOW_STAT, &flow_stats);
    App_TerminateOnError (status, "WP_IwFlowStatistics");
    printf ("   %-33s : %d\n", "forward_packet", (WP_U32) flow_stats.forward_packet);
    printf ("   %-33s : %d\n", "fbp_drop_packets", (WP_U32) flow_stats.fbp_drop_packets);
    printf ("   %-33s : %d\n", "mtu_drop_packets", (WP_U32) flow_stats.mtu_drop_packets);
    printf ("   %-33s : %d\n", "ttl_drop_packets", (WP_U32) flow_stats.ttl_drop_packets);
    printf ("   %-33s : %d\n", "tx_queue_drop_packets", (WP_U32) flow_stats.tx_queue_drop_packets);
    printf ("   %-33s : %d\n", "mpls_drop", (WP_U32) flow_stats.mpls_drop);
    printf ("   %-33s : %d\n", "denied_packets", (WP_U32) flow_stats.denied_packets);
    printf ("   %-33s : %d\n", "group_filtered_packets", (WP_U32) flow_stats.group_filtered_packets);
    printf ("   %-33s : %d\n", "forwarded_bytes", (WP_U32) flow_stats.forwarded_bytes);
    printf ("   %-33s : %d\n", "gtp_bad_headers", (WP_U32) flow_stats.gtp_bad_headers);
    printf ("   %-33s : %d\n", "policer_non_conforming_packets", (WP_U32) flow_stats.policer_non_conforming_packets);
    printf ("   %-33s : %d\n", "cfm_mac_in_mac_drop", (WP_U32) flow_stats.cfm_mac_in_mac_drop);
    printf ("   %-33s : %d\n", "out_of_sequence_packets", (WP_U32) flow_stats.out_of_sequence_packets);
    printf ("   %-33s : %d\n", "pwe_switchover_events", (WP_U32) flow_stats.pwe_switchover_events);
    printf ("   %-33s : %d\n", "non_zero_seq_num", (WP_U32) flow_stats.non_zero_seq_num);
    WP_IwFlowStatisticsReset (h_bridge1_default_agg, WP_IW_FLOW_STAT, &flow_stats);
    printf ("-----------------------------------------------------------------------\n");
    printf (" Flow statisics Bridge2 Default  FA \n");
    printf ("-----------------------------------------------------------------------\n");
    memset (&flow_stats, 0, sizeof (flow_stats));
    status = WP_IwFlowStatistics (h_bridge2_default_agg, WP_IW_FLOW_STAT, &flow_stats);
    App_TerminateOnError (status, "WP_IwFlowStatistics");
    printf ("   %-33s : %d\n", "forward_packet", (WP_U32) flow_stats.forward_packet);
    printf ("   %-33s : %d\n", "fbp_drop_packets", (WP_U32) flow_stats.fbp_drop_packets);
    printf ("   %-33s : %d\n", "mtu_drop_packets", (WP_U32) flow_stats.mtu_drop_packets);
    printf ("   %-33s : %d\n", "ttl_drop_packets", (WP_U32) flow_stats.ttl_drop_packets);
    printf ("   %-33s : %d\n", "tx_queue_drop_packets", (WP_U32) flow_stats.tx_queue_drop_packets);
    printf ("   %-33s : %d\n", "mpls_drop", (WP_U32) flow_stats.mpls_drop);
    printf ("   %-33s : %d\n", "denied_packets", (WP_U32) flow_stats.denied_packets);
    printf ("   %-33s : %d\n", "group_filtered_packets", (WP_U32) flow_stats.group_filtered_packets);
    printf ("   %-33s : %d\n", "forwarded_bytes", (WP_U32) flow_stats.forwarded_bytes);
    printf ("   %-33s : %d\n", "gtp_bad_headers", (WP_U32) flow_stats.gtp_bad_headers);
    printf ("   %-33s : %d\n", "policer_non_conforming_packets", (WP_U32) flow_stats.policer_non_conforming_packets);
    printf ("   %-33s : %d\n", "cfm_mac_in_mac_drop", (WP_U32) flow_stats.cfm_mac_in_mac_drop);
    printf ("   %-33s : %d\n", "out_of_sequence_packets", (WP_U32) flow_stats.out_of_sequence_packets);
    printf ("   %-33s : %d\n", "pwe_switchover_events", (WP_U32) flow_stats.pwe_switchover_events);
    printf ("   %-33s : %d\n", "non_zero_seq_num", (WP_U32) flow_stats.non_zero_seq_num);
    WP_IwFlowStatisticsReset (h_bridge2_default_agg, WP_IW_FLOW_STAT, &flow_stats);
}

/***************************************************************************** 
* Function name: App_ShowImaGroupStats 
* Description  : Display IMA group statistics 
* Input  params: None 
* Output params: None * Return val   : None 
*****************************************************************************/
void App_CheckStats (WP_status * test_result)
{
    WP_status status;
    WP_stats_enet s_hs_enet = { 0 };
    // WP_stats_tdi_atm tdi_atm_stats = { 0 };
    WP_bport_stats bport_stats = { 0 };
    WP_iw_flow_stats flow_stats = { 0 };
    WP_iw_pwe3_stats pwe3_flow_stats = { 0 };
    // WP_stats_aal5_tx atm_tx_channel_stats = { 0 };
    // WP_stats_aal5_rx atm_rx_channel_stats = { 0 };
    // WP_ima_group_statistics ima_group_statistics = { 0 };
    // WP_ima_link_statistics ima_link_statistics = { 0 };
    // WP_U8 ii;

    *test_result = WP_OK;       /*Check enet1 stats */

	/*
 	 * check result of Enet1
	 */
    memset (&s_hs_enet, 0, sizeof (s_hs_enet));
    status = WP_DeviceStatistics (h_device_enet1, &s_hs_enet);
    App_TerminateOnError (status, "WP_DeviceStatistics");
    if (s_hs_enet.txrx_frames_127 != APP_MAX_PKTS_NUM * 2)
    {
        *test_result = (-1);
        printf ("ENET1 txrx_frames_64 check fail!\n");
    }
    if (s_hs_enet.rx_packets != APP_MAX_PKTS_NUM + 1
        && s_hs_enet.rx_packets != APP_MAX_PKTS_NUM )
    {
        *test_result = (-1);
        printf ("ENET1 rx_packets check fail!\n");
    }
    if (s_hs_enet.tx_packets != APP_MAX_PKTS_NUM + 1
         && s_hs_enet.tx_packets != APP_MAX_PKTS_NUM)
    {
        *test_result = (-1);
        printf ("ENET1 tx_packets check fail!\n");
    }                           /*Check ENET1 errors */
    if (s_hs_enet.rx_err_fcs || s_hs_enet.rx_err_alignment
        || s_hs_enet.rx_err_length || s_hs_enet.rx_err_code
        || s_hs_enet.rx_false_carrier || s_hs_enet.rx_undersize
        || s_hs_enet.rx_oversize || s_hs_enet.rx_dropped
        || s_hs_enet.tx_dropped || s_hs_enet.tx_err_fcs
        || s_hs_enet.tx_oversize || s_hs_enet.tx_undersize
        || s_hs_enet.rx_err_host_full || s_hs_enet.rx_err_fbp_underrun
        || s_hs_enet.rx_err_nonvalid_mac || s_hs_enet.rx_err_mru || s_hs_enet.rx_err_sdu || s_hs_enet.tx_err_underrun || s_hs_enet.rx_err_overrun)
    {
        *test_result = (-1);
        printf ("ENET1 error check fail!\n");
    }                           /*Check enet2 stats */

	/*
 	 * check result of Enet2
	 */
    memset (&s_hs_enet, 0, sizeof (s_hs_enet));
    status = WP_DeviceStatistics (h_device_enet2, &s_hs_enet);
    App_TerminateOnError (status, "WP_DeviceStatistics");

    if (s_hs_enet.tx_packets != (APP_MAX_PKTS_NUM + 1)
		&& s_hs_enet.tx_packets != APP_MAX_PKTS_NUM
		&& s_hs_enet.tx_packets != APP_MAX_PKTS_NUM - 1)
    {
        *test_result = (-1);
        printf ("ENET2 tx_packets check fail!\n");
    }                           /*Check ENET2 errors */
    if (s_hs_enet.rx_err_fcs || s_hs_enet.rx_err_alignment
        || s_hs_enet.rx_err_length || s_hs_enet.rx_err_code
        || s_hs_enet.rx_false_carrier || s_hs_enet.rx_undersize
        || s_hs_enet.rx_oversize || s_hs_enet.rx_dropped
        || s_hs_enet.tx_dropped || s_hs_enet.tx_err_fcs
        || s_hs_enet.tx_oversize || s_hs_enet.tx_undersize
        || s_hs_enet.rx_err_host_full || s_hs_enet.rx_err_fbp_underrun
        || s_hs_enet.rx_err_nonvalid_mac || s_hs_enet.rx_err_mru || s_hs_enet.rx_err_sdu || s_hs_enet.tx_err_underrun || s_hs_enet.rx_err_overrun)
    {
        *test_result = (-1);
        printf ("ENET2 error check fail!\n");
    }

    /* 
	 * Check enet1 bport stats 
	 */
    memset (&bport_stats, 0, sizeof (bport_stats));
    status = WP_IwPortStatistics (h_bridge1_bport_enet1, &bport_stats);
    App_TerminateOnError (status, "WP_BportStatistics");
    if (bport_stats.rx_valid_packets != APP_MAX_PKTS_NUM)
    {
        *test_result = (-1);
        printf ("ENET1 boport rx_valid_packets check fail!\n");
    }
    if (bport_stats.protocol_error || bport_stats.bad_ip_version || bport_stats.checksum_error || bport_stats.ip_protocol_error)
    {
        *test_result = (-1);
        printf ("ENET1 boport error check fail!\n");
    }                           /*Check IMA trans bridge egress flow agg */

	/*
 	 * check result for default bridge flow aggregation 1
	 */
    memset (&flow_stats, 0, sizeof (flow_stats));
    status = WP_IwFlowStatistics (h_bridge1_default_agg, WP_IW_FLOW_STAT, &flow_stats);
    App_TerminateOnError (status, "WP_IwFlowStatistics");
    if (flow_stats.fbp_drop_packets || flow_stats.mtu_drop_packets
        || flow_stats.ttl_drop_packets || flow_stats.tx_queue_drop_packets
        || flow_stats.mpls_drop || flow_stats.denied_packets || flow_stats.gtp_bad_headers || flow_stats.cfm_mac_in_mac_drop || flow_stats.out_of_sequence_packets)
    {
        *test_result = (-1);
        printf ("IMA trans bridge egress flow agg 0 error check fail!\n");
    }

	/*
 	 * check result for default bridge flow aggregation 2
	 */
    memset (&flow_stats, 0, sizeof (flow_stats));

    status = WP_IwFlowStatistics (h_bridge2_default_agg, WP_IW_FLOW_STAT, &flow_stats);
    App_TerminateOnError (status, "WP_IwFlowStatistics");
    if (flow_stats.forward_packet != 0)
    {
        *test_result = (-1);
        printf ("IMA trans bridge egress 1 forward_packet check fail(%d)!\n",
                /*ii */ 0);
    }
    if (flow_stats.fbp_drop_packets || flow_stats.mtu_drop_packets
        || flow_stats.ttl_drop_packets || flow_stats.tx_queue_drop_packets
        || flow_stats.mpls_drop || flow_stats.denied_packets || flow_stats.gtp_bad_headers || flow_stats.cfm_mac_in_mac_drop || flow_stats.out_of_sequence_packets)
    {
        *test_result = (-1);
        printf ("IMA trans bridge egress flow agg 1 error check fail!\n");
    }                           /*Check IMA PWE3 ingress flow agg */

	/*
	 * check result for hdlc flow aggregationi 0
	 */
    memset (&pwe3_flow_stats, 0, sizeof (WP_iw_pwe3_stats));

    status = WP_IwFlowStatistics (h_flow_agg_hdlc[0], WP_IW_FLOW_STAT, &pwe3_flow_stats);

    App_TerminateOnError (status, "WP_IwFlowStatistics");
    if (pwe3_flow_stats.forwarded_packets != APP_MAX_PKTS_NUM)
    {
        *test_result = (-1);
        printf ("IMA PWE3 ingress flow agg forwarded_packets check fail!(%d)\n",
                /*ii */ 0);
    }
    if (pwe3_flow_stats.fbp_drop_packets
        || pwe3_flow_stats.mtu_drop_packets
        || pwe3_flow_stats.ttl_drop_packets || pwe3_flow_stats.tx_queue_drop_packets || pwe3_flow_stats.mpls_drop_packets || pwe3_flow_stats.denied_packets || pwe3_flow_stats.hash_fail_cells)
    {
        *test_result = (-1);
        printf ("IMA PWE3 ingress flow agg error check fail!\n");
    }                           /*Check ATM TX Channel stats */

    /*Dump all statistics if failed */
    if (*test_result != WP_OK)
    {
        printf ("-------------------------------\n");
        printf (" ENET1 statisics               \n");
        printf ("-------------------------------\n");
        App_ShowDeviceStats (); // morris

        printf ("-------------------------------\n");
        printf ("Bridge Port Statistics ENET1 \n");
        printf ("-------------------------------\n");
        App_ShowBportStats ();

        printf ("----------------------------------------------------\n");
        printf (" Flow statistics IMA h_flow_agg_pwe3_atm_ingress[0] \n");
        printf ("----------------------------------------------------\n");
        App_ShowFlowAggStats ();
    }
}



