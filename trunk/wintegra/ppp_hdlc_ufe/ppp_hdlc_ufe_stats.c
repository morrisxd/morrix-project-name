/*****************************************************************************
 * (C) Copyright PMC - WIN division (Wintegra) 2011.  All rights reserved.
 * PMC-Sierra (WINTEGRA) CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of PMC-Sierra (Wintegra).
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *  Application : HDLC/PPP over IP/MPLS with QoS
 *  File Name   : ppp_hdlc_ufe_stats.c
 *  Version     : 1
 *  Date        : 28 November 2011
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

void App_ShowDeviceStats(void)
{
   WP_status status;
   WP_stats_enet s_hs_enet = {0};
   WP_stats_upi_hdlc upi_hdlc_stats={0};
   WP_U32 ii;

   memset(&s_hs_enet,0,sizeof(s_hs_enet));
   printf("----------------------------------------------------------\n");
   printf(" ENET1 statistics \n");
   printf("----------------------------------------------------------\n");
   status = WP_DeviceStatistics(h_device_enet1, &s_hs_enet);
   App_TerminateOnError(status, "WP_DeviceStatistics");
   printf(" txrx_frames_64            : %08d\n", (WP_U32)s_hs_enet.txrx_frames_64);
   printf(" txrx_frames_127           : %08d\n", (WP_U32)s_hs_enet.txrx_frames_127);
   printf(" txrx_frames_255           : %08d\n", (WP_U32)s_hs_enet.txrx_frames_255);
   printf(" txrx_frames_511           : %08d\n", (WP_U32)s_hs_enet.txrx_frames_511);
   printf(" txrx_frames_1023          : %08d\n", (WP_U32)s_hs_enet.txrx_frames_1023);
   printf(" txrx_frames_1518          : %08d\n", (WP_U32)s_hs_enet.txrx_frames_1518);
   printf(" txrx_frames_1522          : %08d\n", (WP_U32)s_hs_enet.txrx_frames_1522);
   printf(" rx_bytes                  : %08d\n", (WP_U32)s_hs_enet.rx_bytes);
   printf(" rx_packets                : %08d\n", (WP_U32)s_hs_enet.rx_packets);
   printf(" rx_err_fcs                : %08d\n", (WP_U32)s_hs_enet.rx_err_fcs);
   printf(" rx_multicast              : %08d\n", (WP_U32)s_hs_enet.rx_multicast);
   printf(" rx_broadcast              : %08d\n", (WP_U32)s_hs_enet.rx_broadcast);
   printf(" rx_mac_control            : %08d\n", (WP_U32)s_hs_enet.rx_mac_control);
   printf(" rx_mac_pause              : %08d\n", (WP_U32)s_hs_enet.rx_mac_pause);
   printf(" rx_mac_unknown            : %08d\n", (WP_U32)s_hs_enet.rx_mac_unknown);
   printf(" rx_err_alignment          : %08d\n", (WP_U32)s_hs_enet.rx_err_alignment);
   printf(" rx_err_length             : %08d\n", (WP_U32)s_hs_enet.rx_err_length);
   printf(" rx_err_code               : %08d\n", (WP_U32)s_hs_enet.rx_err_code);
   printf(" rx_false_carrier          : %08d\n", (WP_U32)s_hs_enet.rx_false_carrier);
   printf(" rx_undersize              : %08d\n", (WP_U32)s_hs_enet.rx_undersize);
   printf(" rx_oversize               : %08d\n", (WP_U32)s_hs_enet.rx_oversize);
   printf(" rx_fragments              : %08d\n", (WP_U32)s_hs_enet.rx_fragments);
   printf(" rx_jabber                 : %08d\n", (WP_U32)s_hs_enet.rx_jabber);
   printf(" rx_dropped                : %08d\n\n", (WP_U32)s_hs_enet.rx_dropped);
   printf(" tx_bytes                  : %08d\n", (WP_U32)s_hs_enet.tx_bytes);
   printf(" tx_packets                : %08d\n", (WP_U32)s_hs_enet.tx_packets);
   printf(" tx_multicast              : %08d\n", (WP_U32)s_hs_enet.tx_multicast);
   printf(" tx_broadcast              : %08d\n", (WP_U32)s_hs_enet.tx_broadcast);
   printf(" tx_mac_pause              : %08d\n", (WP_U32)s_hs_enet.tx_mac_pause);
   printf(" tx_defer                  : %08d\n", (WP_U32)s_hs_enet.tx_defer);
   printf(" tx_excess_defer           : %08d\n", (WP_U32)s_hs_enet.tx_excess_defer);
   printf(" tx_single_collision       : %08d\n", (WP_U32)s_hs_enet.tx_single_collision);
   printf(" tx_multi_collision        : %08d\n", (WP_U32)s_hs_enet.tx_multi_collision);
   printf(" tx_late_collision         : %08d\n", (WP_U32)s_hs_enet.tx_late_collision);
   printf(" tx_excess_collision       : %08d\n", (WP_U32)s_hs_enet.tx_excess_collision);
   printf(" tx_no_collision           : %08d\n", (WP_U32)s_hs_enet.tx_no_collision);
   printf(" tx_mac_pause_honored      : %08d\n", (WP_U32)s_hs_enet.tx_mac_pause_honored);
   printf(" tx_dropped                : %08d\n", (WP_U32)s_hs_enet.tx_dropped);
   printf(" tx_jabber                 : %08d\n", (WP_U32)s_hs_enet.tx_jabber);
   printf(" tx_err_fcs                : %08d\n", (WP_U32)s_hs_enet.tx_err_fcs);
   printf(" tx_control                : %08d\n", (WP_U32)s_hs_enet.tx_control);
   printf(" tx_oversize               : %08d\n", (WP_U32)s_hs_enet.tx_oversize);
   printf(" tx_undersize              : %08d\n", (WP_U32)s_hs_enet.tx_undersize);
   printf(" tx_fragments              : %08d\n\n", (WP_U32)s_hs_enet.tx_fragments);
   printf(" rx_host_frames            : %08d\n", (WP_U32)s_hs_enet.rx_host_frames);
   printf(" rx_iw_frames              : %08d\n", (WP_U32)s_hs_enet.rx_iw_frames);
   printf(" rx_err_host_full          : %08d\n", (WP_U32)s_hs_enet.rx_err_host_full);
   printf(" rx_err_fbp_underrun       : %08d\n", (WP_U32)s_hs_enet.rx_err_fbp_underrun);
   printf(" rx_err_nonvalid_mac       : %08d\n", (WP_U32)s_hs_enet.rx_err_nonvalid_mac);
   printf(" rx_err_mru                : %08d\n", (WP_U32)s_hs_enet.rx_err_mru);
   printf(" rx_err_sdu                : %08d\n", (WP_U32)s_hs_enet.rx_err_sdu);
   printf(" tx_err_underrun           : %08d\n", (WP_U32)s_hs_enet.tx_err_underrun);
   printf(" rx_err_overrun            : %08d\n", (WP_U32)s_hs_enet.rx_err_overrun);
   printf(" tx_frames                 : %08d\n", (WP_U32)s_hs_enet.tx_frames);
   printf(" g999_rx_pause_frames      : %08d\n", (WP_U32)s_hs_enet.g999_rx_pause_frames);
   printf(" g999_rx_errored_fragments : %08d\n", (WP_U32)s_hs_enet.g999_rx_errored_fragments);
   printf(" g999_rx_length_error      : %08d\n", (WP_U32)s_hs_enet.g999_rx_length_error);
   WP_DeviceStatisticsReset(h_device_enet1, &s_hs_enet);
   memset(&s_hs_enet,0,sizeof(s_hs_enet));
   printf("-----------------------------------------------------------------\n");
   printf(" ENET2 statisics \n");
   printf("-----------------------------------------------------------------\n");
   status = WP_DeviceStatistics(h_device_enet2, &s_hs_enet);
   App_TerminateOnError(status, "WP_DeviceStatistics");
   printf(" txrx_frames_64            : %08d\n", (WP_U32)s_hs_enet.txrx_frames_64);
   printf(" txrx_frames_127           : %08d\n", (WP_U32)s_hs_enet.txrx_frames_127);
   printf(" txrx_frames_255           : %08d\n", (WP_U32)s_hs_enet.txrx_frames_255);
   printf(" txrx_frames_511           : %08d\n", (WP_U32)s_hs_enet.txrx_frames_511);
   printf(" txrx_frames_1023          : %08d\n", (WP_U32)s_hs_enet.txrx_frames_1023);
   printf(" txrx_frames_1518          : %08d\n", (WP_U32)s_hs_enet.txrx_frames_1518);
   printf(" txrx_frames_1522          : %08d\n", (WP_U32)s_hs_enet.txrx_frames_1522);
   printf(" rx_bytes                  : %08d\n", (WP_U32)s_hs_enet.rx_bytes);
   printf(" rx_packets                : %08d\n", (WP_U32)s_hs_enet.rx_packets);
   printf(" rx_err_fcs                : %08d\n", (WP_U32)s_hs_enet.rx_err_fcs);
   printf(" rx_multicast              : %08d\n", (WP_U32)s_hs_enet.rx_multicast);
   printf(" rx_broadcast              : %08d\n", (WP_U32)s_hs_enet.rx_broadcast);
   printf(" rx_mac_control            : %08d\n", (WP_U32)s_hs_enet.rx_mac_control);
   printf(" rx_mac_pause              : %08d\n", (WP_U32)s_hs_enet.rx_mac_pause);
   printf(" rx_mac_unknown            : %08d\n", (WP_U32)s_hs_enet.rx_mac_unknown);
   printf(" rx_err_alignment          : %08d\n", (WP_U32)s_hs_enet.rx_err_alignment);
   printf(" rx_err_length             : %08d\n", (WP_U32)s_hs_enet.rx_err_length);
   printf(" rx_err_code               : %08d\n", (WP_U32)s_hs_enet.rx_err_code);
   printf(" rx_false_carrier          : %08d\n", (WP_U32)s_hs_enet.rx_false_carrier);
   printf(" rx_undersize              : %08d\n", (WP_U32)s_hs_enet.rx_undersize);
   printf(" rx_oversize               : %08d\n", (WP_U32)s_hs_enet.rx_oversize);
   printf(" rx_fragments              : %08d\n", (WP_U32)s_hs_enet.rx_fragments);
   printf(" rx_jabber                 : %08d\n", (WP_U32)s_hs_enet.rx_jabber);
   printf(" rx_dropped                : %08d\n\n", (WP_U32)s_hs_enet.rx_dropped);
   printf(" tx_bytes                  : %08d\n", (WP_U32)s_hs_enet.tx_bytes);
   printf(" tx_packets                : %08d\n", (WP_U32)s_hs_enet.tx_packets);
   printf(" tx_multicast              : %08d\n", (WP_U32)s_hs_enet.tx_multicast);
   printf(" tx_broadcast              : %08d\n", (WP_U32)s_hs_enet.tx_broadcast);
   printf(" tx_mac_pause              : %08d\n", (WP_U32)s_hs_enet.tx_mac_pause);
   printf(" tx_defer                  : %08d\n", (WP_U32)s_hs_enet.tx_defer);
   printf(" tx_excess_defer           : %08d\n", (WP_U32)s_hs_enet.tx_excess_defer);
   printf(" tx_single_collision       : %08d\n", (WP_U32)s_hs_enet.tx_single_collision);
   printf(" tx_multi_collision        : %08d\n", (WP_U32)s_hs_enet.tx_multi_collision);
   printf(" tx_late_collision         : %08d\n", (WP_U32)s_hs_enet.tx_late_collision);
   printf(" tx_excess_collision       : %08d\n", (WP_U32)s_hs_enet.tx_excess_collision);
   printf(" tx_no_collision           : %08d\n", (WP_U32)s_hs_enet.tx_no_collision);
   printf(" tx_mac_pause_honored      : %08d\n", (WP_U32)s_hs_enet.tx_mac_pause_honored);
   printf(" tx_dropped                : %08d\n", (WP_U32)s_hs_enet.tx_dropped);
   printf(" tx_jabber                 : %08d\n", (WP_U32)s_hs_enet.tx_jabber);
   printf(" tx_err_fcs                : %08d\n", (WP_U32)s_hs_enet.tx_err_fcs);
   printf(" tx_control                : %08d\n", (WP_U32)s_hs_enet.tx_control);
   printf(" tx_oversize               : %08d\n", (WP_U32)s_hs_enet.tx_oversize);
   printf(" tx_undersize              : %08d\n", (WP_U32)s_hs_enet.tx_undersize);
   printf(" tx_fragments              : %08d\n\n", (WP_U32)s_hs_enet.tx_fragments);
   printf(" rx_host_frames            : %08d\n", (WP_U32)s_hs_enet.rx_host_frames);
   printf(" rx_iw_frames              : %08d\n", (WP_U32)s_hs_enet.rx_iw_frames);
   printf(" rx_err_host_full          : %08d\n", (WP_U32)s_hs_enet.rx_err_host_full);
   printf(" rx_err_fbp_underrun       : %08d\n", (WP_U32)s_hs_enet.rx_err_fbp_underrun);
   printf(" rx_err_nonvalid_mac       : %08d\n", (WP_U32)s_hs_enet.rx_err_nonvalid_mac);
   printf(" rx_err_mru                : %08d\n", (WP_U32)s_hs_enet.rx_err_mru);
   printf(" rx_err_sdu                : %08d\n", (WP_U32)s_hs_enet.rx_err_sdu);
   printf(" tx_err_underrun           : %08d\n", (WP_U32)s_hs_enet.tx_err_underrun);
   printf(" rx_err_overrun            : %08d\n", (WP_U32)s_hs_enet.rx_err_overrun);
   printf(" tx_frames                 : %08d\n", (WP_U32)s_hs_enet.tx_frames);
   printf(" g999_rx_pause_frames      : %08d\n", (WP_U32)s_hs_enet.g999_rx_pause_frames);
   printf(" g999_rx_errored_fragments : %08d\n", (WP_U32)s_hs_enet.g999_rx_errored_fragments);
   printf(" g999_rx_length_error      : %08d\n", (WP_U32)s_hs_enet.g999_rx_length_error);
   WP_DeviceStatisticsReset(h_device_enet2, &s_hs_enet);

    for(ii = 0;ii < APP_MAX_DEV_HDLC; ii++)
    {

       printf("-----------------------------------------------------------\n");
       printf(" Device statistics HDLC [%d] \n",ii);
       printf("-----------------------------------------------------------\n");
       memset(&upi_hdlc_stats,0,sizeof(WP_stats_tdi_hdlc));
       status = WP_DeviceStatistics(h_dev_hdlc[ii], &upi_hdlc_stats);
       App_TerminateOnError(status, "WP_Devicestatistics");

       printf("rx_frames                  : %08d\n", (WP_U32)upi_hdlc_stats.rx_frames);             
       printf("tx_frames                  : %08d\n", (WP_U32)upi_hdlc_stats.tx_frames);             
       printf("rx_err_crc                 : %08d\n", (WP_U32)upi_hdlc_stats.rx_err_crc);            
       printf("rx_err_phy                 : %08d\n", (WP_U32)upi_hdlc_stats.rx_err_phy);            
       printf("rx_err_addr_mismatch       : %08d\n", (WP_U32)upi_hdlc_stats.rx_err_addr_mismatch);  
       printf("rx_err_buffer_overrun      : %08d\n", (WP_U32)upi_hdlc_stats.rx_err_buffer_overrun); 
       printf("rx_err_overrun             : %08d\n", (WP_U32)upi_hdlc_stats.rx_err_overrun);        
       printf("rx_err_sop_eop             : %08d\n", (WP_U32)upi_hdlc_stats.rx_err_sop_eop);
       printf("rx_err_iw_mru              : %08d\n", (WP_U32)upi_hdlc_stats.rx_err_iw_mru);         
       printf("rx_err_parity              : %08d\n", (WP_U32)upi_hdlc_stats.rx_err_parity);         
       printf("rx_err_iw_mru              : %08d\n", (WP_U32)upi_hdlc_stats.rx_err_iw_mru);         
       printf("rx_iw_frames               : %08d\n", (WP_U32)upi_hdlc_stats.rx_iw_frames);          
       printf("rx_err_maxsdu              : %08d\n", (WP_U32)upi_hdlc_stats.rx_err_maxsdu);         
       printf("rx_bytes                   : %08d\n", (WP_U32)upi_hdlc_stats.rx_bytes);              
       printf("rx_err_bytes               : %08d\n", (WP_U32)upi_hdlc_stats.rx_err_bytes);          
       printf("tx_bytes                   : %08d\n", (WP_U32)upi_hdlc_stats.tx_bytes);              
       printf("rx_err_abort               : %08d\n\n", (WP_U32)upi_hdlc_stats.rx_err_abort);          
       printf("serial_dps_if_stats\n");  
       printf("tx_frames                  : %08d\n", (WP_U32)upi_hdlc_stats.serial_dps_if_stats.tx_frames);  
       printf("tx_bytes                   : %08d\n", (WP_U32)upi_hdlc_stats.serial_dps_if_stats.tx_bytes);  
       printf("rx_frames                  : %08d\n", (WP_U32)upi_hdlc_stats.serial_dps_if_stats.rx_frames);  
       printf("rx_bytes                   : %08d\n", (WP_U32)upi_hdlc_stats.serial_dps_if_stats.rx_bytes);
       printf("rx_dropped_frames          : %08d\n", (WP_U32)upi_hdlc_stats.serial_dps_if_stats.rx_dropped_frames);  
       printf("rx_pce_denied_frames       : %08d\n", (WP_U32)upi_hdlc_stats.serial_dps_if_stats.rx_pce_denied_frames);
       WP_DeviceStatisticsReset(h_dev_hdlc[ii], &upi_hdlc_stats);

    }
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

void App_ShowBportStats(void)
{
   WP_status status;
   WP_bport_stats bport_stats = {0};
   WP_U32 ii;


   printf("--------------------------------------------------------\n");
   printf("Bridge Port Statistics ENET1 \n");
   printf("--------------------------------------------------------\n");
   memset(&bport_stats, 0, sizeof(bport_stats));
   status = WP_IwPortStatistics(h_bridge1_bport_enet1, &bport_stats);
   App_TerminateOnError(status, "WP_BportStatistics");

   printf("   %-39s : %d\n","rx_valid_packets",(WP_U32)bport_stats.rx_valid_packets);
   printf("   %-39s : %d\n","discard_vlan_acceptable_filter",(WP_U32)bport_stats.discard_vlan_acceptable_filter);
   printf("   %-39s : %d\n","discard_ingress_filter",(WP_U32)bport_stats.discard_ingress_filter);
   printf("   %-39s : %d\n","discard_bridge_classifier",(WP_U32)bport_stats.discard_bridge_classifier);
   printf("   %-39s : %d\n","discard_unk_macsa",(WP_U32)bport_stats.discard_unk_macsa);
   printf("   %-39s : %d\n","deny_mac_sa",(WP_U32)bport_stats.deny_mac_sa);
   printf("   %-39s : %d\n","deny_mac_da",(WP_U32)bport_stats.deny_mac_da);
   printf("   %-39s : %d\n","rx_bc_valid_packets",(WP_U32)bport_stats.rx_bc_valid_packets);
   printf("   %-39s : %d\n","rx_mc_valid_packets",(WP_U32)bport_stats.rx_mc_valid_packets);
   printf("   %-39s : %d\n","forwarded_uc_packets",(WP_U32)bport_stats.forwarded_uc_packets);
   printf("   %-39s : %d\n","forwarded_bc_packets",(WP_U32)bport_stats.forwarded_bc_packets);
   printf("   %-39s : %d\n","forwarded_mc_packets",(WP_U32)bport_stats.forwarded_mc_packets);
   printf("   %-39s : %d\n","protocol_error",(WP_U32)bport_stats.protocol_error);
   printf("   %-39s : %d\n","discard_pce_filter[0]",(WP_U32)bport_stats.discard_pce_filter[0] );
   printf("   %-39s : %d\n","ingress_policer_non_conforming_packets",(WP_U32)bport_stats.ingress_policer_non_conforming_packets);
   printf("   %-39s : %d\n","rx_discard_lpm",(WP_U32)bport_stats.rx_discard_lpm);
   printf("   %-39s : %d\n","iw_mirror_fbp_underrun",(WP_U32)bport_stats.iw_mirror_fbp_underrun);
   printf("   %-39s : %d\n","iw_mirror_iwbg_fbp_underrun",(WP_U32)bport_stats.iw_mirror_iwbg_fbp_underrun);
    WP_IwPortStatisticsReset(h_bridge1_bport_enet1, &bport_stats);

   for(ii = 0;ii < APP_MAX_PORT_HDLC ;ii++)
   {
      printf("-------------------------------------------------------------\n");
      printf("Bridge Port Statistics HDLC %d\n",ii);
      printf("-------------------------------------------------------------\n");
      memset(&bport_stats, 0, sizeof(bport_stats));
      status = WP_IwPortStatistics(h_bridge1_bport_hdlc[ii], &bport_stats);
      App_TerminateOnError(status, "BportStatistics");

      printf("   %-39s : %d\n","rx_valid_packets",(WP_U32)bport_stats.rx_valid_packets);
      printf("   %-39s : %d\n","discard_vlan_acceptable_filter",(WP_U32)bport_stats.discard_vlan_acceptable_filter);
      printf("   %-39s : %d\n","discard_ingress_filter",(WP_U32)bport_stats.discard_ingress_filter);
      printf("   %-39s : %d\n","discard_bridge_classifier",(WP_U32)bport_stats.discard_bridge_classifier);
      printf("   %-39s : %d\n","discard_unk_macsa",(WP_U32)bport_stats.discard_unk_macsa);
      printf("   %-39s : %d\n","deny_mac_sa",(WP_U32)bport_stats.deny_mac_sa);
      printf("   %-39s : %d\n","deny_mac_da",(WP_U32)bport_stats.deny_mac_da);
      printf("   %-39s : %d\n","rx_bc_valid_packets",(WP_U32)bport_stats.rx_bc_valid_packets);
      printf("   %-39s : %d\n","rx_mc_valid_packets",(WP_U32)bport_stats.rx_mc_valid_packets);
      printf("   %-39s : %d\n","forwarded_uc_packets",(WP_U32)bport_stats.forwarded_uc_packets);
      printf("   %-39s : %d\n","forwarded_bc_packets",(WP_U32)bport_stats.forwarded_bc_packets);
      printf("   %-39s : %d\n","forwarded_mc_packets",(WP_U32)bport_stats.forwarded_mc_packets);
      printf("   %-39s : %d\n","protocol_error",(WP_U32)bport_stats.protocol_error);
      printf("   %-39s : %d\n","discard_pce_filter[0]",(WP_U32)bport_stats.discard_pce_filter[0] );
      printf("   %-39s : %d\n","ingress_policer_non_conforming_packets",(WP_U32)bport_stats.ingress_policer_non_conforming_packets);
      printf("   %-39s : %d\n","rx_discard_lpm",(WP_U32)bport_stats.rx_discard_lpm);
      printf("   %-39s : %d\n","iw_mirror_fbp_underrun",(WP_U32)bport_stats.iw_mirror_fbp_underrun);
      printf("   %-39s : %d\n","iw_mirror_iwbg_fbp_underrun",(WP_U32)bport_stats.iw_mirror_iwbg_fbp_underrun);
      WP_IwPortStatisticsReset(h_bridge1_bport_hdlc[ii], &bport_stats);
   }
   for(ii = 0;ii < APP_MAX_PORT_HDLC ;ii++)
   {

      printf("---------------------------------------------------------------\n");
      printf("Bridge2 Port Statistics HDLC %d\n",ii);
      printf("---------------------------------------------------------------\n");
      memset(&bport_stats, 0, sizeof(bport_stats));
      status = WP_IwPortStatistics(h_bridge2_bport_hdlc[ii], &bport_stats);
      App_TerminateOnError(status, "BportStatistics");

      printf("   %-39s : %d\n","rx_valid_packets",(WP_U32)bport_stats.rx_valid_packets);
      printf("   %-39s : %d\n","discard_vlan_acceptable_filter",(WP_U32)bport_stats.discard_vlan_acceptable_filter);
      printf("   %-39s : %d\n","discard_ingress_filtern",(WP_U32)bport_stats.discard_ingress_filter);
      printf("   %-39s : %d\n","discard_bridge_classifier",(WP_U32)bport_stats.discard_bridge_classifier);
      printf("   %-39s : %d\n","discard_unk_macsa",(WP_U32)bport_stats.discard_unk_macsa);
      printf("   %-39s : %d\n","deny_mac_sa",(WP_U32)bport_stats.deny_mac_sa);
      printf("   %-39s : %d\n","deny_mac_da",(WP_U32)bport_stats.deny_mac_da);
      printf("   %-39s : %d\n","rx_bc_valid_packets",(WP_U32)bport_stats.rx_bc_valid_packets);
      printf("   %-39s : %d\n","rx_mc_valid_packets",(WP_U32)bport_stats.rx_mc_valid_packets);
      printf("   %-39s : %d\n","forwarded_uc_packets",(WP_U32)bport_stats.forwarded_uc_packets);
      printf("   %-39s : %d\n","forwarded_bc_packets",(WP_U32)bport_stats.forwarded_bc_packets);
      printf("   %-39s : %d\n","forwarded_mc_packets",(WP_U32)bport_stats.forwarded_mc_packets);
      printf("   %-39s : %d\n","protocol_error",(WP_U32)bport_stats.protocol_error);
      printf("   %-39s : %d\n","discard_pce_filter[0]",(WP_U32)bport_stats.discard_pce_filter[0] );
      printf("   %-39s : %d\n","ingress_policer_non_conforming_packets",(WP_U32)bport_stats.ingress_policer_non_conforming_packets);
      printf("   %-39s : %d\n","rx_discard_lpm",(WP_U32)bport_stats.rx_discard_lpm);
      printf("   %-39s : %d\n","iw_mirror_fbp_underrun",(WP_U32)bport_stats.iw_mirror_fbp_underrun);
      printf("   %-39s : %d\n","iw_mirror_iwbg_fbp_underrun",(WP_U32)bport_stats.iw_mirror_iwbg_fbp_underrun);
      WP_IwPortStatisticsReset(h_bridge2_bport_hdlc[ii], &bport_stats);
   }

   printf("-------------------------------------------------------------------\n");
   printf("Bridge2 Port Enet2:       \n");
   printf("-------------------------------------------------------------------\n");
   memset(&bport_stats, 0, sizeof(bport_stats));
   status = WP_IwPortStatistics(h_bridge2_bport_enet2, &bport_stats);
   App_TerminateOnError(status, "BportStatistics");

   printf("   %-39s : %d\n","rx_valid_packets",(WP_U32)bport_stats.rx_valid_packets);
   printf("   %-39s : %d\n","discard_vlan_acceptable_filter",(WP_U32)bport_stats.discard_vlan_acceptable_filter);
   printf("   %-39s : %d\n","discard_ingress_filter",(WP_U32)bport_stats.discard_ingress_filter);
   printf("   %-39s : %d\n","discard_bridge_classifier",(WP_U32)bport_stats.discard_bridge_classifier);
   printf("   %-39s : %d\n","discard_unk_macsa",(WP_U32)bport_stats.discard_unk_macsa);
   printf("   %-39s : %d\n","deny_mac_sa",(WP_U32)bport_stats.deny_mac_sa);
   printf("   %-39s : %d\n","deny_mac_da",(WP_U32)bport_stats.deny_mac_da);
   printf("   %-39s : %d\n","rx_bc_valid_packets",(WP_U32)bport_stats.rx_bc_valid_packets);
   printf("   %-39s : %d\n","rx_mc_valid_packets",(WP_U32)bport_stats.rx_mc_valid_packets);
   printf("   %-39s : %d\n","forwarded_uc_packets",(WP_U32)bport_stats.forwarded_uc_packets);
   printf("   %-39s : %d\n","forwarded_bc_packets",(WP_U32)bport_stats.forwarded_bc_packets);
   printf("   %-39s : %d\n","forwarded_mc_packets",(WP_U32)bport_stats.forwarded_mc_packets);
   printf("   %-39s : %d\n","protocol_error",(WP_U32)bport_stats.protocol_error);
   printf("   %-39s : %d\n","discard_pce_filter[0]",(WP_U32)bport_stats.discard_pce_filter[0] );
   printf("   %-39s : %d\n","ingress_policer_non_conforming_packets",(WP_U32)bport_stats.ingress_policer_non_conforming_packets);
   printf("   %-39s : %d\n","rx_discard_lpm",(WP_U32)bport_stats.rx_discard_lpm);
   printf("   %-39s : %d\n","iw_mirror_fbp_underrun",(WP_U32)bport_stats.iw_mirror_fbp_underrun);
   printf("   %-39s : %d\n","iw_mirror_iwbg_fbp_underrun",(WP_U32)bport_stats.iw_mirror_iwbg_fbp_underrun);
   WP_IwPortStatisticsReset(h_bridge2_bport_enet2, &bport_stats);

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

void App_ShowFlowAggStats(void)
{
   WP_status status;
   WP_iw_flow_stats flow_stats = {0};
   WP_U32 ii;

   for(ii = 0;ii < APP_MAX_FLOW_AGG_HDLC ;ii++)
   {
      printf("-----------------------------------------------------------------\n");
      printf(" Flow statistics HDLC %d\n",ii);
      printf("-----------------------------------------------------------------\n");
      memset(&flow_stats,0,sizeof(flow_stats));
      status = WP_IwFlowStatistics(h_flow_agg_hdlc[ii],WP_IW_FLOW_STAT,&flow_stats);
      App_TerminateOnError(status, "WP_IwFlowStatistics");

      printf("   %-33s : %d\n","forward_packet",(WP_U32)flow_stats.forward_packet);
      printf("   %-33s : %d\n","fbp_drop_packets",(WP_U32)flow_stats.fbp_drop_packets);
      printf("   %-33s : %d\n","mtu_drop_packets",(WP_U32)flow_stats.mtu_drop_packets);
      printf("   %-33s : %d\n","ttl_drop_packets",(WP_U32)flow_stats.ttl_drop_packets);
      printf("   %-33s : %d\n","tx_queue_drop_packets",(WP_U32)flow_stats.tx_queue_drop_packets);
      printf("   %-33s : %d\n","mpls_drop",(WP_U32)flow_stats.mpls_drop);
      printf("   %-33s : %d\n","denied_packets",(WP_U32)flow_stats.denied_packets);
      printf("   %-33s : %d\n","group_filtered_packets",(WP_U32)flow_stats.group_filtered_packets);
      printf("   %-33s : %d\n","forwarded_bytes",(WP_U32)flow_stats.forwarded_bytes);
      printf("   %-33s : %d\n","gtp_bad_headers",(WP_U32)flow_stats.gtp_bad_headers);
      printf("   %-33s : %d\n","policer_non_conforming_packets",(WP_U32)flow_stats.policer_non_conforming_packets);
      printf("   %-33s : %d\n","cfm_mac_in_mac_drop",(WP_U32)flow_stats.cfm_mac_in_mac_drop);
      printf("   %-33s : %d\n","out_of_sequence_packets",(WP_U32)flow_stats.out_of_sequence_packets);
      printf("   %-33s : %d\n","pwe_switchover_events",(WP_U32)flow_stats.pwe_switchover_events);
      printf("   %-33s : %d\n","non_zero_seq_num",(WP_U32)flow_stats.non_zero_seq_num);
      WP_IwFlowStatisticsReset(h_flow_agg_hdlc[ii],WP_IW_FLOW_STAT,&flow_stats);
   }
   for(ii = 0;ii < PQBLOCK_SIZE;ii++)
   {
      printf("--------------------------------------------------------------------\n");
      printf(" Flow statistics PPPSW %d\n",ii);
      printf("--------------------------------------------------------------------\n");
      memset(&flow_stats,0,sizeof(flow_stats));
      status = WP_IwFlowStatistics(h_flow_agg_pppsw_link[ii],WP_IW_FLOW_STAT,&flow_stats);
      App_TerminateOnError(status, "WP_IwFlowStatistics");

      printf("   %-33s : %d\n","forward_packet",(WP_U32)flow_stats.forward_packet);
      printf("   %-33s : %d\n","fbp_drop_packets",(WP_U32)flow_stats.fbp_drop_packets);
      printf("   %-33s : %d\n","mtu_drop_packets",(WP_U32)flow_stats.mtu_drop_packets);
      printf("   %-33s : %d\n","ttl_drop_packets",(WP_U32)flow_stats.ttl_drop_packets);
      printf("   %-33s : %d\n","tx_queue_drop_packets",(WP_U32)flow_stats.tx_queue_drop_packets);
      printf("   %-33s : %d\n","mpls_drop",(WP_U32)flow_stats.mpls_drop);
      printf("   %-33s : %d\n","denied_packets",(WP_U32)flow_stats.denied_packets);
      printf("   %-33s : %d\n","group_filtered_packets",(WP_U32)flow_stats.group_filtered_packets);
      printf("   %-33s : %d\n","forwarded_bytes",(WP_U32)flow_stats.forwarded_bytes);
      printf("   %-33s : %d\n","gtp_bad_headers",(WP_U32)flow_stats.gtp_bad_headers);
      printf("   %-33s : %d\n","policer_non_conforming_packets",(WP_U32)flow_stats.policer_non_conforming_packets);
      printf("   %-33s : %d\n","cfm_mac_in_mac_drop",(WP_U32)flow_stats.cfm_mac_in_mac_drop);
      printf("   %-33s : %d\n","out_of_sequence_packets",(WP_U32)flow_stats.out_of_sequence_packets);
      printf("   %-33s : %d\n","pwe_switchover_events",(WP_U32)flow_stats.pwe_switchover_events);
      printf("   %-33s : %d\n","non_zero_seq_num",(WP_U32)flow_stats.non_zero_seq_num);
      WP_IwFlowStatisticsReset(h_flow_agg_pppsw_link[ii],WP_IW_FLOW_STAT,&flow_stats);
   }
   printf("------------------------------------------------------------------------\n");
   printf(" Flow statistics Cisco Control \n");
   printf("------------------------------------------------------------------------\n");
   memset(&flow_stats,0,sizeof(flow_stats));
   status = WP_IwFlowStatistics(h_flow_agg_control,WP_IW_FLOW_STAT,&flow_stats);
   App_TerminateOnError(status, "WP_IwFlowStatistics");

   printf("   %-33s : %d\n","forward_packet",(WP_U32)flow_stats.forward_packet);
   printf("   %-33s : %d\n","fbp_drop_packets",(WP_U32)flow_stats.fbp_drop_packets);
   printf("   %-33s : %d\n","mtu_drop_packets",(WP_U32)flow_stats.mtu_drop_packets);
   printf("   %-33s : %d\n","ttl_drop_packets",(WP_U32)flow_stats.ttl_drop_packets);
   printf("   %-33s : %d\n","tx_queue_drop_packets",(WP_U32)flow_stats.tx_queue_drop_packets);
   printf("   %-33s : %d\n","mpls_drop",(WP_U32)flow_stats.mpls_drop);
   printf("   %-33s : %d\n","denied_packets",(WP_U32)flow_stats.denied_packets);
   printf("   %-33s : %d\n","group_filtered_packets",(WP_U32)flow_stats.group_filtered_packets);
   printf("   %-33s : %d\n","forwarded_bytes",(WP_U32)flow_stats.forwarded_bytes);
   printf("   %-33s : %d\n","gtp_bad_headers",(WP_U32)flow_stats.gtp_bad_headers);
   printf("   %-33s : %d\n","policer_non_conforming_packets",(WP_U32)flow_stats.policer_non_conforming_packets);
   printf("   %-33s : %d\n","cfm_mac_in_mac_drop",(WP_U32)flow_stats.cfm_mac_in_mac_drop);
   printf("   %-33s : %d\n","out_of_sequence_packets",(WP_U32)flow_stats.out_of_sequence_packets);
   printf("   %-33s : %d\n","pwe_switchover_events",(WP_U32)flow_stats.pwe_switchover_events);
   printf("   %-33s : %d\n","non_zero_seq_num",(WP_U32)flow_stats.non_zero_seq_num);
   WP_IwFlowStatisticsReset(h_flow_agg_control,WP_IW_FLOW_STAT,&flow_stats);

   if( PPP_OVER_IP == dp_interface_val || PPP_OVER_MPLS == dp_interface_val)
   {
      printf("-------------------------------------------------------------------------\n");
      printf(" Flow statistics LCP\n");
      printf("-------------------------------------------------------------------------\n");
      memset(&flow_stats,0,sizeof(flow_stats));
      status = WP_IwFlowStatistics(h_flow_agg_lcp,WP_IW_FLOW_STAT,&flow_stats);
      App_TerminateOnError(status, "WP_IwFlowStatistics");

      printf("   %-33s : %d\n","forward_packet",(WP_U32)flow_stats.forward_packet);
      printf("   %-33s : %d\n","fbp_drop_packets",(WP_U32)flow_stats.fbp_drop_packets);
      printf("   %-33s : %d\n","mtu_drop_packets",(WP_U32)flow_stats.mtu_drop_packets);
      printf("   %-33s : %d\n","ttl_drop_packets",(WP_U32)flow_stats.ttl_drop_packets);
      printf("   %-33s : %d\n","tx_queue_drop_packets",(WP_U32)flow_stats.tx_queue_drop_packets);
      printf("   %-33s : %d\n","mpls_drop",(WP_U32)flow_stats.mpls_drop);
      printf("   %-33s : %d\n","denied_packets",(WP_U32)flow_stats.denied_packets);
      printf("   %-33s : %d\n","group_filtered_packets",(WP_U32)flow_stats.group_filtered_packets);
      printf("   %-33s : %d\n","forwarded_bytes",(WP_U32)flow_stats.forwarded_bytes);
      printf("   %-33s : %d\n","gtp_bad_headers",(WP_U32)flow_stats.gtp_bad_headers);
      printf("   %-33s : %d\n","policer_non_conforming_packets",(WP_U32)flow_stats.policer_non_conforming_packets);
      printf("   %-33s : %d\n","cfm_mac_in_mac_drop",(WP_U32)flow_stats.cfm_mac_in_mac_drop);
      printf("   %-33s : %d\n","out_of_sequence_packets",(WP_U32)flow_stats.out_of_sequence_packets);
      printf("   %-33s : %d\n","pwe_switchover_events",(WP_U32)flow_stats.pwe_switchover_events);
      printf("   %-33s : %d\n","non_zero_seq_num",(WP_U32)flow_stats.non_zero_seq_num);
      WP_IwFlowStatisticsReset(h_flow_agg_lcp,WP_IW_FLOW_STAT,&flow_stats);
   }

    for(ii = 0;ii < APP_MAX_FLOW_AGG_HDLC ;ii++)
    {
        printf("------------------------------------------------------------------------\n");
        printf(" Flow statistics ENET2 FA[%d]\n",ii);
        printf("------------------------------------------------------------------------\n");
        memset(&flow_stats,0,sizeof(flow_stats));
        status = WP_IwFlowStatistics(h_flow_agg_enet2[ii],WP_IW_FLOW_STAT,&flow_stats);
        App_TerminateOnError(status, "WP_IwFlowStatistics");

        printf("   %-33s : %d\n","forward_packet",(WP_U32)flow_stats.forward_packet);
        printf("   %-33s : %d\n","fbp_drop_packets",(WP_U32)flow_stats.fbp_drop_packets);
        printf("   %-33s : %d\n","mtu_drop_packets",(WP_U32)flow_stats.mtu_drop_packets);
        printf("   %-33s : %d\n","ttl_drop_packets",(WP_U32)flow_stats.ttl_drop_packets);
        printf("   %-33s : %d\n","tx_queue_drop_packets",(WP_U32)flow_stats.tx_queue_drop_packets);
        printf("   %-33s : %d\n","mpls_drop",(WP_U32)flow_stats.mpls_drop);
        printf("   %-33s : %d\n","denied_packets",(WP_U32)flow_stats.denied_packets);
        printf("   %-33s : %d\n","group_filtered_packets",(WP_U32)flow_stats.group_filtered_packets);
        printf("   %-33s : %d\n","forwarded_bytes",(WP_U32)flow_stats.forwarded_bytes);
        printf("   %-33s : %d\n","gtp_bad_headers",(WP_U32)flow_stats.gtp_bad_headers);
        printf("   %-33s : %d\n","policer_non_conforming_packets",(WP_U32)flow_stats.policer_non_conforming_packets);
        printf("   %-33s : %d\n","cfm_mac_in_mac_drop",(WP_U32)flow_stats.cfm_mac_in_mac_drop);
        printf("   %-33s : %d\n","out_of_sequence_packets",(WP_U32)flow_stats.out_of_sequence_packets);
        printf("   %-33s : %d\n","pwe_switchover_events",(WP_U32)flow_stats.pwe_switchover_events);
        printf("   %-33s : %d\n","non_zero_seq_num",(WP_U32)flow_stats.non_zero_seq_num);
        WP_IwFlowStatisticsReset(h_flow_agg_enet2[ii],WP_IW_FLOW_STAT,&flow_stats);
    }
    printf("-----------------------------------------------------------------------\n");
    printf(" Flow statisics Default Bridge  \n");
    printf("-----------------------------------------------------------------------\n");
    memset(&flow_stats,0,sizeof(flow_stats));
    status = WP_IwFlowStatistics(h_bridge1_default_agg,WP_IW_FLOW_STAT,&flow_stats);
    App_TerminateOnError(status, "WP_IwFlowStatistics");

    printf("   %-33s : %d\n","forward_packet",(WP_U32)flow_stats.forward_packet);
    printf("   %-33s : %d\n","fbp_drop_packets",(WP_U32)flow_stats.fbp_drop_packets);
    printf("   %-33s : %d\n","mtu_drop_packets",(WP_U32)flow_stats.mtu_drop_packets);
    printf("   %-33s : %d\n","ttl_drop_packets",(WP_U32)flow_stats.ttl_drop_packets);
    printf("   %-33s : %d\n","tx_queue_drop_packets",(WP_U32)flow_stats.tx_queue_drop_packets);
    printf("   %-33s : %d\n","mpls_drop",(WP_U32)flow_stats.mpls_drop);
    printf("   %-33s : %d\n","denied_packets",(WP_U32)flow_stats.denied_packets);
    printf("   %-33s : %d\n","group_filtered_packets",(WP_U32)flow_stats.group_filtered_packets);
    printf("   %-33s : %d\n","forwarded_bytes",(WP_U32)flow_stats.forwarded_bytes);
    printf("   %-33s : %d\n","gtp_bad_headers",(WP_U32)flow_stats.gtp_bad_headers);
    printf("   %-33s : %d\n","policer_non_conforming_packets",(WP_U32)flow_stats.policer_non_conforming_packets);
    printf("   %-33s : %d\n","cfm_mac_in_mac_drop",(WP_U32)flow_stats.cfm_mac_in_mac_drop);
    printf("   %-33s : %d\n","out_of_sequence_packets",(WP_U32)flow_stats.out_of_sequence_packets);
    printf("   %-33s : %d\n","pwe_switchover_events",(WP_U32)flow_stats.pwe_switchover_events);
    printf("   %-33s : %d\n","non_zero_seq_num",(WP_U32)flow_stats.non_zero_seq_num);
    WP_IwFlowStatisticsReset(h_bridge1_default_agg,WP_IW_FLOW_STAT,&flow_stats);


    printf("-----------------------------------------------------------------------\n");
    printf(" Flow statisics Bridge2 Default  FA \n");
    printf("-----------------------------------------------------------------------\n");
    memset(&flow_stats,0,sizeof(flow_stats));
    status = WP_IwFlowStatistics(h_bridge2_default_agg,WP_IW_FLOW_STAT,&flow_stats);
    App_TerminateOnError(status, "WP_IwFlowStatistics");

    printf("   %-33s : %d\n","forward_packet",(WP_U32)flow_stats.forward_packet);
    printf("   %-33s : %d\n","fbp_drop_packets",(WP_U32)flow_stats.fbp_drop_packets);
    printf("   %-33s : %d\n","mtu_drop_packets",(WP_U32)flow_stats.mtu_drop_packets);
    printf("   %-33s : %d\n","ttl_drop_packets",(WP_U32)flow_stats.ttl_drop_packets);
    printf("   %-33s : %d\n","tx_queue_drop_packets",(WP_U32)flow_stats.tx_queue_drop_packets);
    printf("   %-33s : %d\n","mpls_drop",(WP_U32)flow_stats.mpls_drop);
    printf("   %-33s : %d\n","denied_packets",(WP_U32)flow_stats.denied_packets);
    printf("   %-33s : %d\n","group_filtered_packets",(WP_U32)flow_stats.group_filtered_packets);
    printf("   %-33s : %d\n","forwarded_bytes",(WP_U32)flow_stats.forwarded_bytes);
    printf("   %-33s : %d\n","gtp_bad_headers",(WP_U32)flow_stats.gtp_bad_headers);
    printf("   %-33s : %d\n","policer_non_conforming_packets",(WP_U32)flow_stats.policer_non_conforming_packets);
    printf("   %-33s : %d\n","cfm_mac_in_mac_drop",(WP_U32)flow_stats.cfm_mac_in_mac_drop);
    printf("   %-33s : %d\n","out_of_sequence_packets",(WP_U32)flow_stats.out_of_sequence_packets);
    printf("   %-33s : %d\n","pwe_switchover_events",(WP_U32)flow_stats.pwe_switchover_events);
    printf("   %-33s : %d\n","non_zero_seq_num",(WP_U32)flow_stats.non_zero_seq_num);
    WP_IwFlowStatisticsReset(h_bridge2_default_agg,WP_IW_FLOW_STAT,&flow_stats);
}

void App_ResetStats(void)
{
    int ii;
    WP_stats_enet s_hs_enet;
    WP_bport_stats bport_stats;
    WP_iw_flow_stats flow_stats;
    WP_stats_upi_hdlc upi_hdlc_stats;
    
    WP_DeviceStatisticsReset(h_device_enet1, &s_hs_enet);
    WP_DeviceStatisticsReset(h_device_enet2, &s_hs_enet);

    WP_IwPortStatisticsReset(h_bridge1_bport_enet1, &bport_stats);
    WP_IwPortStatisticsReset(h_bridge2_bport_enet2, &bport_stats);
    for(ii = 0; ii < APP_MAX_DEV_HDLC; ii++)
    {
        WP_DeviceStatisticsReset(h_dev_hdlc[ii], &upi_hdlc_stats);
        
        WP_IwPortStatisticsReset(h_bridge1_bport_hdlc[ii], &bport_stats);
        WP_IwPortStatisticsReset(h_bridge2_bport_hdlc[ii], &bport_stats);

        WP_IwFlowStatisticsReset(h_flow_agg_hdlc[ii], WP_IW_FLOW_STAT, &flow_stats);
        WP_IwFlowStatisticsReset(h_flow_agg_pppsw_link[ii], WP_IW_FLOW_STAT, &flow_stats);
        WP_IwFlowStatisticsReset(h_flow_agg_enet2[ii], WP_IW_FLOW_STAT, &flow_stats);
    }
 
    WP_IwFlowStatisticsReset(h_bridge1_default_agg, WP_IW_FLOW_STAT, &flow_stats);
    WP_IwFlowStatisticsReset(h_bridge2_default_agg, WP_IW_FLOW_STAT, &flow_stats);
    WP_IwFlowStatisticsReset(h_flow_agg_control,WP_IW_FLOW_STAT,&flow_stats);
    
    if( PPP_OVER_IP == dp_interface_val || PPP_OVER_MPLS == dp_interface_val)
    {
        WP_IwFlowStatisticsReset(h_flow_agg_lcp,WP_IW_FLOW_STAT,&flow_stats);
    }
}

/****************************************************************************************************************************
 * Function name: App_CheckDataStats()
 * Description  :  Provide menu for displaying device , flow aggregation and ports statistics
 *
 * Input  params: 
 * Output params: none
 * Return val   : none
 *****************************************************************************************************************************/
void App_CheckTrafficDataStats(WP_status *test_result)
{
    WP_status status;
   
    WP_stats_enet s_hs_enet={0};
    WP_bport_stats bport_stats = {0};
    WP_iw_flow_stats flow_stats = {0};
   
    WP_stats_upi_hdlc upi_hdlc_stats[APP_MAX_HDLC_PORTS];
 

   *test_result = WP_OK;

   /*Check enet1 stats*/
   memset(&s_hs_enet,0,sizeof(s_hs_enet));
   status = WP_DeviceStatistics(h_device_enet1, &s_hs_enet);
   App_TerminateOnError(status, "WP_DeviceStatistics");

   if (s_hs_enet.txrx_frames_127 != APP_MAX_PKTS_NUM * 2)
   {
       *test_result = (-1);
       printf("ENET1 txrx_frames_64 check fail!\n");
   }

   /* some times will receive pause frame */
   if (s_hs_enet.rx_packets != APP_MAX_PKTS_NUM)
   {
       *test_result = (-1);
       printf("ENET1 rx_packets check fail!\n");
   }
   
   if (s_hs_enet.tx_packets != APP_MAX_PKTS_NUM)
   {
       *test_result = (-1);
       printf("ENET1 tx_packets check fail!\n");
   }
   /*Check ENET1 errors*/
   if (s_hs_enet.rx_err_fcs || s_hs_enet.rx_err_alignment || 
       s_hs_enet.rx_err_length || s_hs_enet.rx_err_code || 
       s_hs_enet.rx_false_carrier || s_hs_enet.rx_undersize || 
       s_hs_enet.rx_oversize || s_hs_enet.rx_dropped || 
       s_hs_enet.tx_dropped || s_hs_enet.tx_err_fcs ||
       s_hs_enet.tx_oversize || s_hs_enet.tx_undersize ||
       s_hs_enet.rx_err_host_full || s_hs_enet.rx_err_fbp_underrun ||
       s_hs_enet.rx_err_nonvalid_mac || s_hs_enet.rx_err_mru ||
       s_hs_enet.rx_err_sdu || s_hs_enet.tx_err_underrun ||
       s_hs_enet.rx_err_overrun)
   {
       *test_result = (-1);
       printf("ENET1 error check fail!\n");
   }

   /*Check enet2 stats*/
   memset(&s_hs_enet,0,sizeof(s_hs_enet));
   status = WP_DeviceStatistics(h_device_enet2, &s_hs_enet);
   App_TerminateOnError(status, "WP_DeviceStatistics");

   if (s_hs_enet.txrx_frames_127 != APP_MAX_PKTS_NUM)
   {
      *test_result = (-1);
       printf("ENET2 txrx_frames_127 check fail!\n");
   }

   /* maybe tx pause frame sometimes*/
   if (s_hs_enet.tx_packets != APP_MAX_PKTS_NUM)
   {
      *test_result = (-1);
       printf("ENET2 tx_packets check fail!\n");
   }

   /*Check ENET2 errors*/
   if (s_hs_enet.rx_err_fcs || s_hs_enet.rx_err_alignment || 
       s_hs_enet.rx_err_length || s_hs_enet.rx_err_code || 
       s_hs_enet.rx_false_carrier || s_hs_enet.rx_undersize || 
       s_hs_enet.rx_oversize || s_hs_enet.rx_dropped || 
       s_hs_enet.tx_dropped || s_hs_enet.tx_err_fcs ||
       s_hs_enet.tx_oversize || s_hs_enet.tx_undersize ||
       s_hs_enet.rx_err_host_full || s_hs_enet.rx_err_fbp_underrun ||
       s_hs_enet.rx_err_nonvalid_mac || s_hs_enet.rx_err_mru ||
       s_hs_enet.rx_err_sdu || s_hs_enet.tx_err_underrun ||
       s_hs_enet.rx_err_overrun)
   {
       *test_result = (-1);
       printf("ENET2 error check fail!\n");
   }
   
   /*Check hdlc device stats*/

   //for(ii = 0;ii < APP_MAX_DEV_HDLC; ii++)
   {
       memset(&upi_hdlc_stats[0], 0, sizeof(upi_hdlc_stats[0]));
       status = WP_DeviceStatistics(h_dev_hdlc[0], &upi_hdlc_stats[0]);
       App_TerminateOnError(status, "WP_Devicestatistics");


       /*Check tdm errors*/
        if (upi_hdlc_stats[0].rx_err_abort || upi_hdlc_stats[0].rx_err_addr_mismatch || 
            upi_hdlc_stats[0].rx_err_buffer_overrun || upi_hdlc_stats[0].rx_err_crc || 
            upi_hdlc_stats[0].rx_err_overrun || upi_hdlc_stats[0].rx_err_bytes 
            )
        {
          *test_result = (-1);
          printf("HDLC device %d error check fail!\n", 0);
        }
   }

   if ((upi_hdlc_stats[0].tx_frames != upi_hdlc_stats[0].rx_iw_frames)
       || (APP_MAX_PKTS_NUM != upi_hdlc_stats[0].tx_frames))
   {
       *test_result = (-1);
       printf("HDLC device 0 total tx rx check fail!\n");       
   }

   /*Check enet1 bport stats*/
   memset(&bport_stats, 0, sizeof(bport_stats));
   status = WP_IwPortStatistics(h_bridge1_bport_enet1, &bport_stats);
   App_TerminateOnError(status, "WP_BportStatistics");
   
   if (bport_stats.rx_valid_packets != APP_MAX_PKTS_NUM)
   {
       *test_result = (-1);
       printf("ENET1 boport1 rx_valid_packets check fail!\n");
   }

   if (bport_stats.protocol_error || bport_stats.bad_ip_version || 
       bport_stats.checksum_error || bport_stats.ip_protocol_error)
   {
       *test_result = (-1);
        printf("ENET1 boport1 error check fail!\n");
   }

   /* Check bridge 1  stats*/
   memset(&bport_stats, 0, sizeof(bport_stats));
   status = WP_IwPortStatistics(h_bridge1_bport_hdlc[0], &bport_stats);
   App_TerminateOnError(status, "WP_BportStatistics");
   
   if (APP_MAX_PKTS_NUM != bport_stats.forwarded_uc_packets)
   {
       *test_result = (-1);
       printf("ENET1 boport hdlc rx_valid_packets check fail!\n");
   }

   if (bport_stats.protocol_error || bport_stats.bad_ip_version || 
       bport_stats.checksum_error || bport_stats.ip_protocol_error)
   {
       *test_result = (-1);
        printf("ENET1 boport hdlc error check fail!\n");
   }

   /* Check bridge 2 hdlc stats*/
   memset(&bport_stats, 0, sizeof(bport_stats));
   status = WP_IwPortStatistics(h_bridge2_bport_hdlc[0], &bport_stats);
   App_TerminateOnError(status, "WP_BportStatistics");
   
   if (APP_MAX_PKTS_NUM != bport_stats.rx_valid_packets)
   {
       *test_result = (-1);
       printf("ENET2 boport hdlc rx_valid_packets check fail!\n");
   }

   if (bport_stats.protocol_error || bport_stats.bad_ip_version || 
       bport_stats.checksum_error || bport_stats.ip_protocol_error)
   {
       *test_result = (-1);
        printf("ENET1 hdcl error check fail!\n");
   }

   memset(&bport_stats, 0, sizeof(bport_stats));
   status = WP_IwPortStatistics(h_bridge2_bport_enet2, &bport_stats);
   App_TerminateOnError(status, "WP_BportStatistics");
   
   if (bport_stats.forwarded_uc_packets != APP_MAX_PKTS_NUM)
   {
       *test_result = (-1);
       printf("ENET2 boport2 rx_valid_packets check fail!\n");
   }

   if (bport_stats.protocol_error || bport_stats.bad_ip_version || 
       bport_stats.checksum_error || bport_stats.ip_protocol_error)
   {
       *test_result = (-1);
        printf("ENET2 boport2 error check fail!\n");
   }

   /* check flow agg mlppp stats*/
   memset(&flow_stats, 0, sizeof(flow_stats));
   status = WP_IwFlowStatistics(h_flow_agg_hdlc[0], WP_IW_FLOW_STAT, &flow_stats);
   App_TerminateOnError(status, "WP_IwFlowStatistics");

   if (APP_MAX_PKTS_NUM != flow_stats.forward_packet)
   {
      *test_result = (-1);
      printf("Flowagg hdlc 0 forward_packet check fail!\n");
   }

   if (flow_stats.fbp_drop_packets || flow_stats.mtu_drop_packets ||
       flow_stats.ttl_drop_packets || flow_stats.tx_queue_drop_packets ||
       flow_stats.mpls_drop || flow_stats.denied_packets || 
       flow_stats.gtp_bad_headers || flow_stats.cfm_mac_in_mac_drop ||
       flow_stats.out_of_sequence_packets)
   {
       *test_result = (-1);
        printf("Flowagg hdlc 0 error check fail!\n");
   }

   // h_flow_agg_pppsw_link_
   memset(&flow_stats, 0, sizeof(flow_stats));
   status = WP_IwFlowStatistics(h_flow_agg_pppsw_link[0], WP_IW_FLOW_STAT, &flow_stats);
   App_TerminateOnError(status, "WP_IwFlowStatistics");

   if (APP_MAX_PKTS_NUM != flow_stats.forward_packet)
   {
      *test_result = (-1);
      printf("Flowagg pppsw 0 forward_packet check fail!\n");
   }

   if (flow_stats.fbp_drop_packets || flow_stats.mtu_drop_packets ||
       flow_stats.ttl_drop_packets || flow_stats.tx_queue_drop_packets ||
       flow_stats.mpls_drop || flow_stats.denied_packets || 
       flow_stats.gtp_bad_headers || flow_stats.cfm_mac_in_mac_drop ||
       flow_stats.out_of_sequence_packets)
   {
       *test_result = (-1);
        printf("egress flow agg 1 error check fail!\n");
   }

   // h_flow_agg_enet2
   memset(&flow_stats, 0, sizeof(flow_stats));
   status = WP_IwFlowStatistics(h_flow_agg_enet2[0], WP_IW_FLOW_STAT, &flow_stats);
   App_TerminateOnError(status, "WP_IwFlowStatistics");

   if (APP_MAX_PKTS_NUM != flow_stats.forward_packet)
   {
      *test_result = (-1);
      printf("Flowagg enet2 forward_packet check fail!\n");
   }

   if (flow_stats.fbp_drop_packets || flow_stats.mtu_drop_packets ||
       flow_stats.ttl_drop_packets || flow_stats.tx_queue_drop_packets ||
       flow_stats.mpls_drop || flow_stats.denied_packets || 
       flow_stats.gtp_bad_headers || flow_stats.cfm_mac_in_mac_drop ||
       flow_stats.out_of_sequence_packets)
   {
       *test_result = (-1);
        printf("Flowagg enet2  error check fail!\n");
   }   

}

/****************************************************************************************************************************
 * Function name: App_CheckDataStats()
 * Description  :  Provide menu for displaying device , flow aggregation and ports statistics
 *
 * Input  params: 
 * Output params: none
 * Return val   : none
 *****************************************************************************************************************************/
void App_CheckControlDataStats(WP_status *test_result)
{
     WP_status status;
   
    WP_stats_enet s_hs_enet={0};
    WP_bport_stats bport_stats = {0};
    WP_iw_flow_stats flow_stats = {0};
   
    WP_stats_upi_hdlc upi_hdlc_stats[APP_MAX_HDLC_PORTS];
 

   *test_result = WP_OK;

   /*Check enet1 stats*/
   memset(&s_hs_enet,0,sizeof(s_hs_enet));
   status = WP_DeviceStatistics(h_device_enet1, &s_hs_enet);
   App_TerminateOnError(status, "WP_DeviceStatistics");

   if (s_hs_enet.txrx_frames_127 != APP_MAX_PKTS_NUM * 2)
   {
       *test_result = (-1);
       printf("ENET1 txrx_frames_64 check fail!\n");
   }

   /* some times will receive pause frame */
   if (s_hs_enet.rx_packets != APP_MAX_PKTS_NUM)
   {
       *test_result = (-1);
       printf("ENET1 rx_packets check fail!\n");
   }
   
   if (s_hs_enet.tx_packets != APP_MAX_PKTS_NUM)
   {
       *test_result = (-1);
       printf("ENET1 tx_packets check fail!\n");
   }
   /*Check ENET1 errors*/
   if (s_hs_enet.rx_err_fcs || s_hs_enet.rx_err_alignment || 
       s_hs_enet.rx_err_length || s_hs_enet.rx_err_code || 
       s_hs_enet.rx_false_carrier || s_hs_enet.rx_undersize || 
       s_hs_enet.rx_oversize || s_hs_enet.rx_dropped || 
       s_hs_enet.tx_dropped || s_hs_enet.tx_err_fcs ||
       s_hs_enet.tx_oversize || s_hs_enet.tx_undersize ||
       s_hs_enet.rx_err_host_full || s_hs_enet.rx_err_fbp_underrun ||
       s_hs_enet.rx_err_nonvalid_mac || s_hs_enet.rx_err_mru ||
       s_hs_enet.rx_err_sdu || s_hs_enet.tx_err_underrun ||
       s_hs_enet.rx_err_overrun)
   {
       *test_result = (-1);
       printf("ENET1 error check fail!\n");
   }
   
   /*Check hdlc device stats*/

   //for(ii = 0;ii < APP_MAX_DEV_HDLC; ii++)
   {
       memset(&upi_hdlc_stats[0], 0, sizeof(upi_hdlc_stats[0]));
       status = WP_DeviceStatistics(h_dev_hdlc[0], &upi_hdlc_stats[0]);
       App_TerminateOnError(status, "WP_Devicestatistics");


       /*Check tdm errors*/
        if (upi_hdlc_stats[0].rx_err_abort || upi_hdlc_stats[0].rx_err_addr_mismatch || 
            upi_hdlc_stats[0].rx_err_buffer_overrun || upi_hdlc_stats[0].rx_err_crc || 
            upi_hdlc_stats[0].rx_err_overrun || upi_hdlc_stats[0].rx_err_bytes 
            )
        {
          *test_result = (-1);
          printf("HDLC device %d error check fail!\n", 0);
        }
   }

   if ((upi_hdlc_stats[0].tx_frames != upi_hdlc_stats[0].rx_iw_frames)
       || (APP_MAX_PKTS_NUM != upi_hdlc_stats[0].tx_frames))
   {
       *test_result = (-1);
       printf("HDLC device 0 total tx rx check fail!\n");       
   }

   /*Check enet1 bport stats*/
   memset(&bport_stats, 0, sizeof(bport_stats));
   status = WP_IwPortStatistics(h_bridge1_bport_enet1, &bport_stats);
   App_TerminateOnError(status, "WP_BportStatistics");
   
   if (bport_stats.rx_valid_packets != APP_MAX_PKTS_NUM)
   {
       *test_result = (-1);
       printf("ENET1 boport1 rx_valid_packets check fail!\n");
   }

   if (bport_stats.protocol_error || bport_stats.bad_ip_version || 
       bport_stats.checksum_error || bport_stats.ip_protocol_error)
   {
       *test_result = (-1);
        printf("ENET1 boport1 error check fail!\n");
   }

   /* Check bridge 1  stats*/
   memset(&bport_stats, 0, sizeof(bport_stats));
   status = WP_IwPortStatistics(h_bridge1_bport_hdlc[0], &bport_stats);
   App_TerminateOnError(status, "WP_BportStatistics");
   
   if (APP_MAX_PKTS_NUM != bport_stats.forwarded_uc_packets)
   {
       *test_result = (-1);
       printf("ENET1 boport hdlc rx_valid_packets check fail!\n");
   }

   if (bport_stats.protocol_error || bport_stats.bad_ip_version || 
       bport_stats.checksum_error || bport_stats.ip_protocol_error)
   {
       *test_result = (-1);
        printf("ENET1 boport hdlc error check fail!\n");
   }

   /* Check bridge 2 hdlc stats*/
   memset(&bport_stats, 0, sizeof(bport_stats));
   status = WP_IwPortStatistics(h_bridge2_bport_hdlc[0], &bport_stats);
   App_TerminateOnError(status, "WP_BportStatistics");
   
   if (APP_MAX_PKTS_NUM != bport_stats.rx_valid_packets)
   {
       *test_result = (-1);
       printf("ENET2 boport hdlc rx_valid_packets check fail!\n");
   }

   if (bport_stats.protocol_error || bport_stats.bad_ip_version || 
       bport_stats.checksum_error || bport_stats.ip_protocol_error)
   {
       *test_result = (-1);
        printf("ENET1 hdcl error check fail!\n");
   }

   /* check flow agg mlppp stats*/
   memset(&flow_stats, 0, sizeof(flow_stats));
   status = WP_IwFlowStatistics(h_flow_agg_hdlc[0], WP_IW_FLOW_STAT, &flow_stats);
   App_TerminateOnError(status, "WP_IwFlowStatistics");

   if (APP_MAX_PKTS_NUM != flow_stats.forward_packet)
   {
      *test_result = (-1);
      printf("Flowagg hdlc 0 forward_packet check fail!\n");
   }

   if (flow_stats.fbp_drop_packets || flow_stats.mtu_drop_packets ||
       flow_stats.ttl_drop_packets || flow_stats.tx_queue_drop_packets ||
       flow_stats.mpls_drop || flow_stats.denied_packets || 
       flow_stats.gtp_bad_headers || flow_stats.cfm_mac_in_mac_drop ||
       flow_stats.out_of_sequence_packets)
   {
       *test_result = (-1);
        printf("Flowagg hdlc 0 error check fail!\n");
   }

   // h_flow_agg_pppsw_link_
   memset(&flow_stats, 0, sizeof(flow_stats));
   status = WP_IwFlowStatistics(h_flow_agg_pppsw_link[0], WP_IW_FLOW_STAT, &flow_stats);
   App_TerminateOnError(status, "WP_IwFlowStatistics");

   if (APP_MAX_PKTS_NUM != flow_stats.forward_packet)
   {
      *test_result = (-1);
      printf("Flowagg pppsw 0 forward_packet check fail!\n");
   }

   if (flow_stats.fbp_drop_packets || flow_stats.mtu_drop_packets ||
       flow_stats.ttl_drop_packets || flow_stats.tx_queue_drop_packets ||
       flow_stats.mpls_drop || flow_stats.denied_packets || 
       flow_stats.gtp_bad_headers || flow_stats.cfm_mac_in_mac_drop ||
       flow_stats.out_of_sequence_packets)
   {
       *test_result = (-1);
        printf("egress flow agg 1 error check fail!\n");
   }

   // h_flow_agg_control
   memset(&flow_stats, 0, sizeof(flow_stats));
   status = WP_IwFlowStatistics(h_flow_agg_control, WP_IW_FLOW_STAT, &flow_stats);
   App_TerminateOnError(status, "WP_IwFlowStatistics");

   if (APP_MAX_PKTS_NUM != flow_stats.forward_packet)
   {
      *test_result = (-1);
      printf("Flowagg Cisco Control forward_packet check fail!\n");
   }

   if (flow_stats.fbp_drop_packets || flow_stats.mtu_drop_packets ||
       flow_stats.ttl_drop_packets || flow_stats.tx_queue_drop_packets ||
       flow_stats.mpls_drop || flow_stats.denied_packets || 
       flow_stats.gtp_bad_headers || flow_stats.cfm_mac_in_mac_drop ||
       flow_stats.out_of_sequence_packets)
   {
       *test_result = (-1);
        printf("Flowagg Cisco Control  error check fail!\n");
   }   


}


void App_CheckDataStats(WP_status *test_result)
{
    if (TRAFFIC == config_choice)
    {
        App_CheckTrafficDataStats(test_result);
    }
    else
    if (CONTROL == config_choice)
    {
        App_CheckControlDataStats(test_result);
    }
    else
    {
        return;
    }

   /*Dump all statistics if failed*/
   if (*test_result != WP_OK)
    {
        App_ShowDeviceStats();
        App_ShowBportStats();
        App_ShowFlowAggStats();
    }    
}

