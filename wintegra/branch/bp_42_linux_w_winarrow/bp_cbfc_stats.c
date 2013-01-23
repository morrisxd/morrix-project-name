#ifndef __BP_CBFC_STATS_C__
#define __BP_CBFC_STATS_C__

/******************************************************************************
 * Function name:   WPT_LocalDisplayDeviceStats ()
 * Description  :   Displays Bridge Port Statistics.
 *
 * Input  params:   WP_handle enet_handle: Handle of ENET port.
 * Output params: 
 * Return val   : none
 ******************************************************************************/
WP_U32 WPT_LocalDisplayDeviceStats (WP_handle enet_handle)
{
    WP_stats_enet enet_stats;
    WP_status status;

    memset (&enet_stats, 0, sizeof (WP_stats_enet));

    status = WP_DeviceStatistics (enet_handle, &enet_stats);
    if (status)
    {
        printf ("Error Reading Enet Statistics\n");
        exit (1);
    }
    printf ("\n----------------------------------------------------\n");

    printf ("ENET Port Statistics (HW)\n");
#if 1
    printf ("TxRx Frame  64:              %08x%08x\n", ((WPT_StatField*) &enet_stats.txrx_frames_64)->part.high, ((WPT_StatField*) &enet_stats.txrx_frames_64)->part.low);
    printf ("Tx Bytes:                    %08x%08x\n", ((WPT_StatField*) &enet_stats.tx_bytes)->part.high, ((WPT_StatField*) &enet_stats.tx_bytes)->part.low);
    printf ("TxRx Frame 127:              %08x%08x\n", ((WPT_StatField*) &enet_stats.txrx_frames_127)->part.high, ((WPT_StatField*) &enet_stats.txrx_frames_127)->part.low);
    printf ("Tx Packets:                  %08x%08x\n", ((WPT_StatField*) &enet_stats.tx_packets)->part.high, ((WPT_StatField*) &enet_stats.tx_packets)->part.low);
    printf ("TxRx Frame 255:              %08x%08x\n", ((WPT_StatField*) &enet_stats.txrx_frames_255)->part.high, ((WPT_StatField*) &enet_stats.txrx_frames_255)->part.low);
    printf ("Tx Multicast:                %08x%08x\n", ((WPT_StatField*) &enet_stats.tx_multicast)->part.high, ((WPT_StatField*) &enet_stats.tx_multicast)->part.low);
    printf ("TxRx Frame 1023:             %08x%08x\n", ((WPT_StatField*) &enet_stats.txrx_frames_1023)->part.high, ((WPT_StatField*) &enet_stats.txrx_frames_1023)->part.low);
    printf ("Tx Broadcast:                %08x%08x\n", ((WPT_StatField*) &enet_stats.tx_broadcast)->part.high, ((WPT_StatField*) &enet_stats.tx_broadcast)->part.low);
    printf ("TxRx Frame 1518:             %08x%08x\n", ((WPT_StatField*) &enet_stats.txrx_frames_1518)->part.high, ((WPT_StatField*) &enet_stats.txrx_frames_1518)->part.low);
    printf ("Tx Mac Pause:                %08x%08x\n", ((WPT_StatField*) &enet_stats.tx_mac_pause)->part.high, ((WPT_StatField*) &enet_stats.tx_mac_pause)->part.low);
    printf ("TxRx Frame 1522:             %08x%08x\n", ((WPT_StatField*) &enet_stats.txrx_frames_1522)->part.high, ((WPT_StatField*) &enet_stats.txrx_frames_1522)->part.low);
    printf ("Tx Defer:                    %08x%08x\n", ((WPT_StatField*) &enet_stats.tx_defer)->part.high, ((WPT_StatField*) &enet_stats.tx_defer)->part.low);
    printf ("Rx Bytes:                    %08x%08x\n", ((WPT_StatField*) &enet_stats.rx_bytes)->part.high, ((WPT_StatField*) &enet_stats.rx_bytes)->part.low);
    printf ("Tx Exess Defer:              %08x%08x\n", ((WPT_StatField*) &enet_stats.tx_excess_defer)->part.high, ((WPT_StatField*) &enet_stats.tx_excess_defer)->part.low);
    printf ("Rx Packets:                  %08x%08x\n", ((WPT_StatField*) &enet_stats.rx_packets)->part.high, ((WPT_StatField*) &enet_stats.rx_packets)->part.low);
    printf ("Tx Single Collision:         %08x%08x\n", ((WPT_StatField*) &enet_stats.tx_single_collision)->part.high, ((WPT_StatField*) &enet_stats.tx_single_collision)->part.low);
    printf ("Rx Error FCS:                %08x%08x\n", ((WPT_StatField*) &enet_stats.rx_err_fcs)->part.high, ((WPT_StatField*) &enet_stats.rx_err_fcs)->part.low);
    printf ("Tx Multi Collision:          %08x%08x\n", ((WPT_StatField*) &enet_stats.tx_multi_collision)->part.high, ((WPT_StatField*) &enet_stats.tx_multi_collision)->part.low);
    printf ("Rx Multicast:                %08x%08x\n", ((WPT_StatField*) &enet_stats.rx_multicast)->part.high, ((WPT_StatField*) &enet_stats.rx_multicast)->part.low);
    printf ("Tx Late Collision:           %08x%08x\n", ((WPT_StatField*) &enet_stats.tx_late_collision)->part.high, ((WPT_StatField*) &enet_stats.tx_late_collision)->part.low);
    printf ("Rx Broadcast:                %08x%08x\n", ((WPT_StatField*) &enet_stats.rx_broadcast)->part.high, ((WPT_StatField*) &enet_stats.rx_broadcast)->part.low);
    printf ("Tx Excess Collision:         %08x%08x\n", ((WPT_StatField*) &enet_stats.tx_excess_collision)->part.high, ((WPT_StatField*) &enet_stats.tx_excess_collision)->part.low);
    printf ("Rx Mac Control:              %08x%08x\n", ((WPT_StatField*) &enet_stats.rx_mac_control)->part.high, ((WPT_StatField*) &enet_stats.rx_mac_control)->part.low);
    printf ("Tx No Collision:             %08x%08x\n", ((WPT_StatField*) &enet_stats.tx_no_collision)->part.high, ((WPT_StatField*) &enet_stats.tx_no_collision)->part.low);
    printf ("Rx Mac Pause:                %08x%08x\n", ((WPT_StatField*) &enet_stats.rx_mac_pause)->part.high, ((WPT_StatField*) &enet_stats.rx_mac_pause)->part.low);
    printf ("Tx Mac Pause Honored:        %08x%08x\n", ((WPT_StatField*) &enet_stats.tx_mac_pause_honored)->part.high, ((WPT_StatField*) &enet_stats.tx_mac_pause_honored)->part.low);
    printf ("Rx Mac Unknown:              %08x%08x\n", ((WPT_StatField*) &enet_stats.rx_mac_unknown)->part.high, ((WPT_StatField*) &enet_stats.rx_mac_unknown)->part.low);
    printf ("Tx Dropped:                  %08x%08x\n", ((WPT_StatField*) &enet_stats.tx_dropped)->part.high, ((WPT_StatField*) &enet_stats.tx_dropped)->part.low);
    printf ("Rx Error Alignment:          %08x%08x\n", ((WPT_StatField*) &enet_stats.rx_err_alignment)->part.high, ((WPT_StatField*) &enet_stats.rx_err_alignment)->part.low);
    printf ("Tx Jabber:                   %08x%08x\n", ((WPT_StatField*) &enet_stats.tx_jabber)->part.high, ((WPT_StatField*) &enet_stats.tx_jabber)->part.low);
    printf ("Rx Error LEN:                %08x%08x\n", ((WPT_StatField*) &enet_stats.rx_err_length)->part.high, ((WPT_StatField*) &enet_stats.rx_err_length)->part.low);
    printf ("Tx Errors FCS:               %08x%08x\n", ((WPT_StatField*) &enet_stats.tx_err_fcs)->part.high, ((WPT_StatField*) &enet_stats.tx_err_fcs)->part.low);
    printf ("Rx Error Code:               %08x%08x\n", ((WPT_StatField*) &enet_stats.rx_err_code)->part.high, ((WPT_StatField*) &enet_stats.rx_err_code)->part.low);
    printf ("Tx Control:                  %08x%08x\n", ((WPT_StatField*) &enet_stats.tx_control)->part.high, ((WPT_StatField*) &enet_stats.tx_control)->part.low);
    printf ("Rx False Carrier:            %08x%08x\n", ((WPT_StatField*) &enet_stats.rx_false_carrier)->part.high, ((WPT_StatField*) &enet_stats.rx_false_carrier)->part.low);
    printf ("Tx Oversize:                 %08x%08x\n", ((WPT_StatField*) &enet_stats.tx_oversize)->part.high, ((WPT_StatField*) &enet_stats.tx_oversize)->part.low);
    printf ("Rx Undersize:                %08x%08x\n", ((WPT_StatField*) &enet_stats.rx_undersize)->part.high, ((WPT_StatField*) &enet_stats.rx_undersize)->part.low);
    printf ("Tx Undersize:                %08x%08x\n", ((WPT_StatField*) &enet_stats.tx_undersize)->part.high, ((WPT_StatField*) &enet_stats.tx_undersize)->part.low);
    printf ("Rx Oversize:                 %08x%08x\n", ((WPT_StatField*) &enet_stats.rx_oversize)->part.high, ((WPT_StatField*) &enet_stats.rx_oversize)->part.low);
    printf ("Tx Fragments:                %08x%08x\n", ((WPT_StatField*) &enet_stats.tx_fragments)->part.high, ((WPT_StatField*) &enet_stats.tx_fragments)->part.low);
    printf ("Rx Fragments:                %08x%08x\n", ((WPT_StatField*) &enet_stats.rx_fragments)->part.high, ((WPT_StatField*) &enet_stats.rx_fragments)->part.low);
    printf ("Rx Jabber:                   %08x%08x\n", ((WPT_StatField*) &enet_stats.rx_jabber)->part.high, ((WPT_StatField*) &enet_stats.rx_jabber)->part.low);
    printf ("Rx Dropped:                  %08x%08x\n", ((WPT_StatField*) &enet_stats.rx_dropped)->part.high, ((WPT_StatField*) &enet_stats.rx_dropped)->part.low);
#endif
    printf ("HS ENET Device Statistics (DPS)\n");

    printf ("Rx Host Frames:              %08x%08x\n", ((WPT_StatField*) &enet_stats.rx_host_frames)->part.high, ((WPT_StatField*) &enet_stats.rx_host_frames)->part.low);
    printf ("Rx Iw Frames:                %08x%08x\n", ((WPT_StatField*) &enet_stats.rx_iw_frames)->part.high, ((WPT_StatField*) &enet_stats.rx_iw_frames)->part.low);
    printf ("Rx Error Host Full:          %08x%08x\n", ((WPT_StatField*) &enet_stats.rx_err_host_full)->part.high, ((WPT_StatField*) &enet_stats.rx_err_host_full)->part.low);
    printf ("Rx Error Fbp Underrun:       %08x%08x\n", ((WPT_StatField*) &enet_stats.rx_err_fbp_underrun)->part.high, ((WPT_StatField*) &enet_stats.rx_err_fbp_underrun)->part.low);
    printf ("Rx Error Nonvalid Mac:       %08x%08x\n", ((WPT_StatField*) &enet_stats.rx_err_nonvalid_mac)->part.high, ((WPT_StatField*) &enet_stats.rx_err_nonvalid_mac)->part.low);
    printf ("Rx Error Mru:                %08x%08x\n", ((WPT_StatField*) &enet_stats.rx_err_mru)->part.high, ((WPT_StatField*) &enet_stats.rx_err_mru)->part.low);
    printf ("Rx Error Sdu:                %08x%08x\n", ((WPT_StatField*) &enet_stats.rx_err_sdu)->part.high, ((WPT_StatField*) &enet_stats.rx_err_sdu)->part.low);
    printf ("Tx Error Underrun:           %08x%08x\n", ((WPT_StatField*) &enet_stats.tx_err_underrun)->part.high, ((WPT_StatField*) &enet_stats.tx_err_underrun)->part.low);
    printf ("Rx Error Overrun:            %08x%08x\n", ((WPT_StatField*) &enet_stats.rx_err_overrun)->part.high, ((WPT_StatField*) &enet_stats.rx_err_overrun)->part.low);
    printf ("Tx Frames:                   %08x%08x\n", ((WPT_StatField*) &enet_stats.tx_frames)->part.high, ((WPT_StatField*) &enet_stats.tx_frames)->part.low);
    printf ("----------------------------------------------------\n");

    return 0;
}

/******************************************************************************
 * Function name:   WPT_DisplayRoutingPortStats ()
 * Description  :   Display the statistics counter of the routing bport of the IW
 *                  system.
 *
 * Input  params:   port_handle: handle of the bridging port in te IW; 
 *                  type: type name of the IW bridging port.
 * Output params: 
 * Return val   :   none
 ******************************************************************************/
void WPT_DisplayRoutingPortStats (WP_handle port_handle, WP_CHAR * type)
{
    WP_route_port_stats rport_stats = { 0 };
    WP_status status = 0;

    memset (&rport_stats, 0, sizeof (WP_route_port_stats));
    status = WP_IwPortStatistics (port_handle, &rport_stats);
    terminate_on_error (status, "WP_IwPortStatistics()", __LINE__);
    printf ("\n%s Route port statistics of port %d (%x)\n", type, (port_handle & 0x000000ff), port_handle);
    printf ("--------- GBE Port -----------\n");
    printf ("rx_valid_packets = %d\n", (WP_U32) rport_stats.rx_valid_packets);
    printf ("rx_ipv4_option_packets = %d\n", (WP_U32) rport_stats.rx_ipv4_option_packets);
    printf ("rx_non_ip_packets = %d\n", (WP_U32) rport_stats.rx_non_ip_packets);
    printf ("rx_compressed_packets = %d\n", (WP_U32) rport_stats.rx_compressed_packets);
    printf ("rx_valid_mpls_packets = %d\n", (WP_U32) rport_stats.rx_valid_mpls_packets);
    printf ("rx_protocol_error = %d\n", (WP_U32) rport_stats.rx_protocol_error);
    printf ("rx_checksum_error = %d\n", (WP_U32) rport_stats.rx_checksum_error);
    printf ("rx_discard_classifier = %d\n", (WP_U32) rport_stats.rx_discard_classifier);
    printf ("rx_mpls_lookup_drop = %d\n", (WP_U32) rport_stats.rx_mpls_lookup_drop);
    printf ("tx_forwarded_packets = %d\n", (WP_U32) rport_stats.tx_forwarded_packets);
    printf ("\n");
}

/******************************************************************************
 * Function name:   App_CheckPceFilterStats ()
 * Description  :   Print out how many packets we got inside the PCE module.
 *
 * Input  params:   WP_handle filter: the handle of the filter.
 * Output params:   none
 * Return val   :   none
 ******************************************************************************/
void App_CheckPceFilterStats (WP_handle filter)
{
    WP_U32 temp1, temp2;
    WP_U8 index = (WP_U8) (filter & 0xFF);

    memset (&filter_stat, 0, sizeof (WP_pce_filter_stat));

    WP_PceFilterStatistics (filter, &filter_stat);

    printf ("\n\nComparing PCE Filter %d Statistic Changes\n", index);

    temp1 = filter_stat.denied_pkts;
    temp2 = filter_stat.denied_pkts >> 32;
    printf ("denied_pkts            %08X %08X       diff %04X\n", temp2, temp1, temp2-temp1);

    temp1 = filter_stat.matched_pkts;
    temp2 = filter_stat.matched_pkts >> 32;
    printf ("matched_pkts           %08X %08X       diff %04X\n", temp2, temp1, temp2 - temp1);

    temp1 = filter_stat.no_matched_pkts;
    temp2 = filter_stat.no_matched_pkts >> 32;
    printf ("no_matched_pkts        %08X %08X       diff %04X\n", temp2, temp1, temp2 - temp1);

    printf ("\n\n");
}


/******************************************************************************
 * Function name:   WPT_FlowStatistics ()
 * Description  :   Print out how many packets we got inside the flow aggregation.
 *
 * Input  params:   WP_handle filter: the handle of the filter.
 * Output params:   none
 * Return val   :   none
 ******************************************************************************/
void WPT_FlowStatistics (WP_handle agg)
{
    WP_iw_flow_stats iw_flow_stats;
    WP_U32 forward_packets = 0;
    WP_status status;

    memset (&iw_flow_stats, 0, sizeof (iw_flow_stats));
    status = WP_IwFlowStatistics (agg, WP_IW_FLOW_STAT, &iw_flow_stats);
    terminate_on_error (status, "WP_IwFlowStatistics", __LINE__);

    printf ("\nAggregation flow statistics of flow %d (%x)\n", (agg & 0x000000ff), agg);
    printf ("=================================================\n");
    printf ("Forward Packet:          %08x%08x\n", ((WPT_StatField*) &iw_flow_stats.forward_packet)->part.high, ((WPT_StatField*) &iw_flow_stats.forward_packet)->part.low);

    forward_packets = ((WPT_StatField*) &iw_flow_stats.forward_packet)->part.low;

    printf ("FBP Drop Packets:        %08x%08x\n", ((WPT_StatField*) &iw_flow_stats.fbp_drop_packets)->part.high, ((WPT_StatField*) &iw_flow_stats.fbp_drop_packets)->part.low);
    printf ("MTU Drop Packets:        %08x%08x\n", ((WPT_StatField*) &iw_flow_stats.mtu_drop_packets)->part.high, ((WPT_StatField*) &iw_flow_stats.mtu_drop_packets)->part.low);
    printf ("TTL Drop Packets:        %08x%08x\n", ((WPT_StatField*) &iw_flow_stats.ttl_drop_packets)->part.high, ((WPT_StatField*) &iw_flow_stats.ttl_drop_packets)->part.low);
    printf ("TX Queue Drop Packets:   %08x%08x\n", ((WPT_StatField*) &iw_flow_stats.tx_queue_drop_packets)->part.high, ((WPT_StatField*) &iw_flow_stats.tx_queue_drop_packets)->part.low);
    printf ("MPLS Drop:               %08x%08x\n", ((WPT_StatField*) &iw_flow_stats.mpls_drop)->part.high, ((WPT_StatField*) &iw_flow_stats.mpls_drop)->part.low);
    printf ("Denied Packets:          %08x%08x\n", ((WPT_StatField*) &iw_flow_stats.denied_packets)->part.high, ((WPT_StatField*) &iw_flow_stats.denied_packets)->part.low);
    printf ("Group filtered packets:  %08x%08x\n", ((WPT_StatField*) &iw_flow_stats.group_filtered_packets)->part.high, ((WPT_StatField*) &iw_flow_stats.group_filtered_packets)->part.low);
    printf ("forwarded_bytes:         %08x%08x\n", ((WPT_StatField*) &iw_flow_stats.forwarded_bytes)->part.high, ((WPT_StatField*) &iw_flow_stats.forwarded_bytes)->part.low);
    printf ("GTP bad headers:         %08x%08x\n", ((WPT_StatField*) &iw_flow_stats.gtp_bad_headers)->part.high, ((WPT_StatField*) &iw_flow_stats.gtp_bad_headers)->part.low);
    printf ("Policer Non Conforming:  %08x%08x\n", ((WPT_StatField*) &iw_flow_stats.policer_non_conforming_packets)->part.high, ((WPT_StatField*) &iw_flow_stats.policer_non_conforming_packets)->part.low);
}


#endif

