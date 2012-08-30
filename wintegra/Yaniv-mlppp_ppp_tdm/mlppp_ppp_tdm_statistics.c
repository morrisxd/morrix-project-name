/*****************************************************************************
 * (C) Copyright PMC - WIN division (Wintegra) 2012.  All rights reserved.
 * PMC-Sierra (WINTEGRA) CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of PMC-Sierra (Wintegra).
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

 /*****************************************************************************
 *  Appplication: MLPPP over IP with QoS
 *  File Name   : mlppp_ppp_tdm_statistics.c
 *
 *****************************************************************************/


/****************************************************************************************************************************
 * Function name: App_ShowEnetDeviceStats()
 * Description  : display enet device related statistics
 *
 * Input  params: 
 * Output params: none
 * Return val   : none
 *****************************************************************************************************************************/

void App_ShowEnetDeviceStats(WP_handle enet_device_handle, WP_stats_enet s_hs_enet)
{
   memset(&s_hs_enet,0,sizeof(s_hs_enet));
   status = WP_DeviceStatistics(enet_device_handle, &s_hs_enet);
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
}

/****************************************************************************************************************************
 * Function name: App_ShowTdmDeviceStats()
 * Description  : display TDM device related stats
 *
 * Input  params: 
 * Output params: none
 * Return val   : none
 *****************************************************************************************************************************/

void App_ShowTdmDeviceStats(WP_handle tdm_device_handle, WP_stats_tdi_hdlc tdi_hdlc_stats)
{
   memset(&tdi_hdlc_stats,0,sizeof(WP_stats_tdi_hdlc));
   status = WP_DeviceStatistics(tdm_device_handle,&tdi_hdlc_stats);
   App_TerminateOnError(status, "WP_Devicestatistics");

   printf("rx_frames                  : %08d\n", (WP_U32)tdi_hdlc_stats.rx_frames);
   printf("tx_frames                  : %08d\n", (WP_U32)tdi_hdlc_stats.tx_frames);
   printf("rx_err_crc                 : %08d\n", (WP_U32)tdi_hdlc_stats.rx_err_crc);
   printf("rx_err_abort               : %08d\n", (WP_U32)tdi_hdlc_stats.rx_err_abort);
   printf("rx_err_addr_mismatch       : %08d\n", (WP_U32)tdi_hdlc_stats.rx_err_addr_mismatch);
   printf("rx_err_buffer_overrun      : %08d\n", (WP_U32)tdi_hdlc_stats.rx_err_buffer_overrun);
   printf("rx_err_overrun             : %08d\n", (WP_U32)tdi_hdlc_stats.rx_err_overrun);
   printf("rx_err_iw_buffer_underrun  : %08d\n", (WP_U32)tdi_hdlc_stats.rx_err_iw_buffer_underrun);
   printf("rx_err_iw_mru              : %08d\n", (WP_U32)tdi_hdlc_stats.rx_err_iw_mru);
   printf("tx_err_underrun            : %08d\n", (WP_U32)tdi_hdlc_stats.tx_err_underrun);
   printf("rx_err_maxsdu              : %08d\n", (WP_U32)tdi_hdlc_stats.rx_err_maxsdu);
   printf("rx_iw_frames               : %08d\n", (WP_U32)tdi_hdlc_stats.rx_iw_frames);
   printf("rx_err_non_octet           : %08d\n", (WP_U32)tdi_hdlc_stats.rx_err_non_octet);
   printf("rx_bytes                   : %08d\n", (WP_U32)tdi_hdlc_stats.rx_bytes);
   printf("rx_err_bytes               : %08d\n", (WP_U32)tdi_hdlc_stats.rx_err_bytes);
   printf("tx_bytes                   : %08d\n", (WP_U32)tdi_hdlc_stats.tx_bytes);
   printf("rx_err_iw_l2_parse         : %08d\n", (WP_U32)tdi_hdlc_stats.rx_err_iw_l2_parse);
}

/****************************************************************************************************************************
 * Function name: App_ShowBridgePortStats()
 * Description  : display bridge port related stats
 *
 * Input  params: 
 * Output params: none
 * Return val   : none
 *****************************************************************************************************************************/

void App_ShowBridgePortStats(WP_handle bport_handle, WP_bport_stats bport_stats)
{
   memset(&bport_stats, 0, sizeof(bport_stats));
   status = WP_IwPortStatistics(bport_handle, &bport_stats);
   App_TerminateOnError(status, "WP_BportStatistics");

   printf("rx_valid_packets                       : %08d\n",(WP_U32)bport_stats.rx_valid_packets);
   printf("discard_vlan_acceptable_filter         : %08d\n",(WP_U32)bport_stats.discard_vlan_acceptable_filter);
   printf("discard_ingress_filter                 : %08d\n",(WP_U32)bport_stats.discard_ingress_filter);
   printf("discard_bridge_classifier              : %08d\n",(WP_U32)bport_stats.discard_bridge_classifier);
   printf("discard_unk_macsa                      : %08d\n",(WP_U32)bport_stats.discard_unk_macsa);
   printf("deny_mac_sa                            : %08d\n",(WP_U32)bport_stats.deny_mac_sa);
   printf("deny_mac_da                            : %08d\n",(WP_U32)bport_stats.deny_mac_da);
   printf("rx_bc_valid_packets                    : %08d\n",(WP_U32)bport_stats.rx_bc_valid_packets);
   printf("rx_mc_valid_packets                    : %08d\n",(WP_U32)bport_stats.rx_mc_valid_packets);
   printf("forwarded_uc_packets                   : %08d\n",(WP_U32)bport_stats.forwarded_uc_packets);
   printf("forwarded_bc_packets                   : %08d\n",(WP_U32)bport_stats.forwarded_bc_packets);
   printf("forwarded_mc_packets                   : %08d\n",(WP_U32)bport_stats.forwarded_mc_packets);
   printf("protocol_error                         : %08d\n",(WP_U32)bport_stats.protocol_error);
   printf("res_mc_mac_deny                        : %08d\n",(WP_U32)bport_stats.res_mc_mac_deny);
   printf("discard_dfc_filter[0]                  : %08d\n",(WP_U32)bport_stats.discard_dfc_filter[0] );
   printf("deny_cfm_frames                        : %08d\n",(WP_U32)bport_stats.deny_cfm_frames);
   printf("bad_ip_version                         : %08d\n",(WP_U32)bport_stats.bad_ip_version);
   printf("checksum_error                         : %08d\n",(WP_U32)bport_stats.checksum_error);
   printf("ip_protocol_error                      : %08d\n",(WP_U32)bport_stats.ip_protocol_error);
   printf("discard_pce_filter[0]                  : %08d\n",(WP_U32)bport_stats.discard_pce_filter[0] );
   printf("ingress_policer_non_conforming_packets : %08d\n",(WP_U32)bport_stats.ingress_policer_non_conforming_packets);
   printf("rx_discard_lpm                         : %08d\n",(WP_U32)bport_stats.rx_discard_lpm);
   printf("iw_mirror_fbp_underrun                 : %08d\n",(WP_U32)bport_stats.iw_mirror_fbp_underrun);
   printf("iw_mirror_iwbg_fbp_underrun            : %08d\n",(WP_U32)bport_stats.iw_mirror_iwbg_fbp_underrun);
}

/****************************************************************************************************************************
 * Function name: App_ShowFlowAggStats()
 * Description  : display flow aggregation related stats
 *
 * Input  params: 
 * Output params: none
 * Return val   : none
 *****************************************************************************************************************************/

void App_ShowFlowAggStats(WP_handle flow_agg_handle, WP_iw_flow_stats flow_stats)
{
   memset(&flow_stats,0,sizeof(flow_stats));
   status = WP_IwFlowStatistics(flow_agg_handle,WP_IW_FLOW_STAT,&flow_stats);
   App_TerminateOnError(status, "WP_IwFlowStatistics");

   printf("Forward Packet            : %08d\n",(WP_U32)flow_stats.forward_packet);
   printf("FBP Drop Packets          : %08d\n",(WP_U32)flow_stats.fbp_drop_packets);
   printf("MTU Drop Packets          : %08d\n",(WP_U32)flow_stats.mtu_drop_packets);
   printf("TTL Drop Packets          : %08d\n",(WP_U32)flow_stats.ttl_drop_packets);
   printf("TX Queue Drop Packets     : %08d\n",(WP_U32)flow_stats.tx_queue_drop_packets);
   printf("MPLS Drop                 : %08d\n",(WP_U32)flow_stats.mpls_drop);
   printf("Denied Packets            : %08d\n",(WP_U32)flow_stats.denied_packets);
   printf("Group Filtered Packets    : %08d\n",(WP_U32)flow_stats.group_filtered_packets);
   printf("Forwarded Bytes           : %08d\n",(WP_U32)flow_stats.forwarded_bytes);
   printf("Gtp Bad Headers           : %08d\n",(WP_U32)flow_stats.gtp_bad_headers);
   printf("Bad Policer Packets       : %08d\n",(WP_U32)flow_stats.policer_non_conforming_packets);
   printf("cfm_mac_in_mac_drop       : %08d\n",(WP_U32)flow_stats.cfm_mac_in_mac_drop);
   printf("out_of_sequence_packets   : %08d\n",(WP_U32)flow_stats.out_of_sequence_packets);
   printf("pwe_switchover_events     : %08d\n",(WP_U32)flow_stats.pwe_switchover_events);
   printf("non_zero_seq_num          : %08d\n",(WP_U32)flow_stats.non_zero_seq_num);
   printf("\n");
}

/****************************************************************************************************************************
 * Function name: App_ShowMLPPPBundleStats()
 * Description  : display MLPPP bundle related stats
 *
 * Input  params: 
 * Output params: none
 * Return val   : none
 *****************************************************************************************************************************/

void App_ShowMLPPPBundleStats(void)
{
   WP_status status;
   WP_stats_mp_bundle mp_bundle_stats={0};
   WP_U32 ii;
   WP_stats_mp_link stats_mp_link;

   printf("-------------------------------\n");
   printf(" MLPPP Tx Bundle statisics	 \n");
   printf("-------------------------------\n");
   memset(&mp_bundle_stats,0,sizeof(mp_bundle_stats));
   status = WP_MpBundleStatistics(mlppp_group_handle[0],&mp_bundle_stats);
   App_TerminateOnError(status, "WP_MpBundleStatistics");

   printf("received frames       : %08d\n",(WP_U32)mp_bundle_stats.rx_frames);
   printf("received iw frames    : %08d\n",(WP_U32)mp_bundle_stats.rx_iw_frames);
   printf("rx_err_mrru           : %08d\n",(WP_U32)mp_bundle_stats.rx_err_mrru);     
   printf("rx_err_iw_mru         : %08d\n",(WP_U32)mp_bundle_stats.rx_err_iw_mru);  
   printf("rx_err_buffer_overrun : %08d\n",(WP_U32)mp_bundle_stats.rx_err_buffer_overrun);
   printf("tx_frames             : %08d\n",(WP_U32)mp_bundle_stats.tx_frames);
   printf("tx_err_mrru           : %08d\n",(WP_U32)mp_bundle_stats.tx_err_mrru);
   printf("rx_err_iw_fbp         : %08d\n",(WP_U32)mp_bundle_stats.rx_err_iw_fbp);
   printf("rx_flushed_fragments  : %08d\n",(WP_U32)mp_bundle_stats.rx_flushed_fragments);



   for(ii=0; ii < 2; ii++)
   {

      memset(&stats_mp_link,0,sizeof(WP_stats_mp_link));
      status = WP_MpLinkStatistics( mlppp_group_handle[0], h_dev_tdm[ii], &stats_mp_link);
      App_TerminateOnError(status, "WP_MpLinkStatistics");

      printf("-------------------------------\n");
      printf(" MLPPP Links statisics link %d \n", ii);
      printf("-------------------------------\n");

      printf("tx_fragments                      : %08d\n",(WP_U32)stats_mp_link.tx_fragments);
      printf("tx_fillers                        : %08d\n",(WP_U32)stats_mp_link.tx_fillers);
      printf("rx_fragments                      : %08d\n",(WP_U32)stats_mp_link.rx_fragments);
      printf("rx_fillers                        : %08d\n",(WP_U32)stats_mp_link.rx_fillers);
      printf("rx_fragments_overrun              : %08d\n",(WP_U32)stats_mp_link.rx_fragments_overrun);
      printf("rx_err_fragments                  : %08d\n",(WP_U32)stats_mp_link.rx_err_fragments);
      printf("rx_err_mp_rcvd_during_single_link : %08d\n",(WP_U32)stats_mp_link.rx_err_fragments);
   }
}

/****************************************************************************************************************************
 * Function name: App_ShowStats()
 * Description  :  Provide menu for displaying device , flow aggregation and ports statistics
 *
 * Input  params: 
 * Output params: none
 * Return val   : none
 *****************************************************************************************************************************/

void App_ShowStats(void)
{
   WP_U32 ii;
   WP_stats_enet s_hs_enet={0};
   WP_stats_tdi_hdlc tdi_hdlc_stats={0};
   WP_bport_stats bport_stats = {0};
   WP_iw_flow_stats flow_stats = {0};
   WP_CHAR tmp[20];
   WP_U32  ch;
   
   do
   {
      printf("\nMENU");
      printf("\nEnter 1 to print  Device Stats");
      printf("\nEnter 2 to print  Bport  Stats");
      printf("\nEnter 3 to print  FlwAgg Stats");
      printf("\nEnter 4 to print  MLPPP Bundle Stats");
      printf("\nEnter 5 to print  All Stats");
      printf("\nEnter 6 to previous menu");
      printf("\n");
      
      gets(tmp);
      sscanf(tmp, "%d", &ch);
      switch(ch)
      {
         case 1:
            printf("-------------------------------\n");
            printf(" ENET1 statisics               \n");
            printf("-------------------------------\n");
            App_ShowEnetDeviceStats(h_device_enet1, s_hs_enet);
            printf("-------------------------------\n");
            printf(" ENET2 statisics               \n");
            printf("-------------------------------\n");
            App_ShowEnetDeviceStats(h_device_enet2, s_hs_enet);
            for(ii = 0;ii < APP_MAX_TDM_PORTS;ii++)
            {
               printf("-------------------------------\n");
               printf(" Device statisics TDM %d \n",ii);
               printf("-------------------------------\n");
               App_ShowTdmDeviceStats(h_dev_tdm[ii], tdi_hdlc_stats);
            }
            
            break;
         case 2:
            printf("-------------------------------\n");
            printf("Bridge Port Statistics ENET7 \n");
            printf("-------------------------------\n");
            App_ShowBridgePortStats(h_bridge1_bport_enet1, bport_stats);
            for(ii = 0;ii < APP_MAX_TDM_PORTS;ii++)
            {
               printf("-------------------------------\n");
               printf("Bridge1 Port Statistics TDM %d\n",ii);
               printf("-------------------------------\n");
               App_ShowBridgePortStats(h_bridge1_bport_tdm[ii], bport_stats); 
            }
            printf("-------------------------------\n");
            printf("Bridge1 Port Statistics MLPPP  \n");
            printf("-------------------------------\n");
            App_ShowBridgePortStats(h_bridge1_bport_mlppp, bport_stats);
			
            for(ii = 0;ii < APP_MAX_TDM_PORTS;ii++)
            {
               printf("-------------------------------\n");
               printf("Bridge2 Port Statistics TDM %d\n",ii);
               printf("-------------------------------\n");
               App_ShowBridgePortStats(h_bridge2_bport_tdm[ii], bport_stats);
            }
            printf("-------------------------------\n");
            printf("Bridge2 Port Statistics MLPPP  \n");
            printf("-------------------------------\n");
            App_ShowBridgePortStats(h_bridge2_bport_mlppp, bport_stats);
            break;
         case 3:
            for(ii = 0;ii < APP_MAX_TDM_PORTS;ii++)
            {
               printf("-------------------------------\n");
               printf(" Flow statistics TDM %d\n",ii);
               printf("-------------------------------\n");
               App_ShowFlowAggStats(h_flow_agg_tdm[ii], flow_stats);
            }
			
            printf("-------------------------------\n");
            printf(" Flow statistics h_flow_agg_mlppp %d\n");
            printf("-------------------------------\n");
            App_ShowFlowAggStats(h_flow_agg_mlppp1, flow_stats);

			
            for(ii = 0;ii < APP_MAX_TDM_PORTS;ii++)
            {
               printf("-------------------------------\n");
               printf(" Flow statistics PPPSW %d\n",ii);
               printf("-------------------------------\n");
               App_ShowFlowAggStats(h_flow_agg_pppsw_link[ii], flow_stats);
            }
            printf("-------------------------------\n");
            printf(" Flow statistics MLPPP at PPP switch\n");
            printf("-------------------------------\n");
            App_ShowFlowAggStats(h_flow_agg_pppsw_link_mlppp, flow_stats);
            printf("-------------------------------\n");
            printf(" Flow statistics MLPPP control at Bridge2 \n");
            printf("-------------------------------\n");
            App_ShowFlowAggStats(h_flow_agg_mlppp_control, flow_stats);
            printf("-------------------------------\n");
            printf(" Flow statisics Default flow Bridge1  \n");
            printf("-------------------------------\n");
            App_ShowFlowAggStats(h_bridge1_default_agg, flow_stats);
            printf("-------------------------------\n");
            printf(" Flow statisics Default flow Bridge2  \n");
            printf("-------------------------------\n");
            App_ShowFlowAggStats(h_bridge2_default_agg, flow_stats);
            printf("-------------------------------\n");
            printf(" Flow statisics MLPPP ENET2  \n");
            printf("-------------------------------\n");
            App_ShowFlowAggStats(h_flow_agg_enet2_mlppp, flow_stats);
            break;
         case 4:
            App_ShowMLPPPBundleStats();
            break;
            
         case 5:           
            printf("-------------------------------\n");
            printf(" ENET1 statisics               \n");
            printf("-------------------------------\n");
            App_ShowEnetDeviceStats(h_device_enet1, s_hs_enet);
            printf("-------------------------------\n");
            printf(" ENET2 statisics               \n");
            printf("-------------------------------\n");
            App_ShowEnetDeviceStats(h_device_enet2, s_hs_enet);
            
            printf("-------------------------------\n");
            printf("Bridge Port Statistics ENET7 \n");
            printf("-------------------------------\n");
            App_ShowBridgePortStats(h_bridge1_bport_enet1, bport_stats);
            for(ii = 0;ii < APP_MAX_TDM_PORTS;ii++)
            {
               printf("-------------------------------\n");
               printf("Bridge1 Port Statistics TDM %d\n",ii);
               printf("-------------------------------\n");
               App_ShowBridgePortStats(h_bridge1_bport_tdm[ii], bport_stats);
            }
            printf("-------------------------------\n");
            printf("Bridge1 Port Statistics MLPPP  \n");
            printf("-------------------------------\n");
            App_ShowBridgePortStats(h_bridge1_bport_mlppp, bport_stats);
			
            for(ii = 0;ii < APP_MAX_TDM_PORTS;ii++)
            {
               printf("-------------------------------\n");
               printf("Bridge2 Port Statistics TDM %d\n",ii);
               printf("-------------------------------\n");
               App_ShowBridgePortStats(h_bridge2_bport_tdm[ii], bport_stats);
            }
            printf("-------------------------------\n");
            printf("Bridge2 Port Statistics MLPPP  \n");
            printf("-------------------------------\n");
            App_ShowBridgePortStats(h_bridge2_bport_mlppp, bport_stats);
            
            for(ii = 0;ii < APP_MAX_TDM_PORTS;ii++)
            {
               printf("-------------------------------\n");
               printf(" Flow statistics TDM %d\n",ii);
               printf("-------------------------------\n");
               App_ShowFlowAggStats(h_flow_agg_tdm[ii], flow_stats);
            }
            for(ii = 0;ii < APP_MAX_TDM_PORTS;ii++)
            {
               printf("-------------------------------\n");
               printf(" Flow statistics PPPSW %d\n",ii);
               printf("-------------------------------\n");
               App_ShowFlowAggStats(h_flow_agg_pppsw_link[ii], flow_stats);
            }
            printf("-------------------------------\n");
            printf(" Flow statistics MLPPP at PPP switch\n");
            printf("-------------------------------\n");
            App_ShowFlowAggStats(h_flow_agg_pppsw_link_mlppp, flow_stats);
            printf("-------------------------------\n");
            printf(" Flow statistics MLPPP control at Bridge2 \n");
            printf("-------------------------------\n");
            App_ShowFlowAggStats(h_flow_agg_mlppp_control, flow_stats);
            printf("-------------------------------\n");
            printf(" Flow statisics Default flow Bridge1  \n");
            printf("-------------------------------\n");
            App_ShowFlowAggStats(h_bridge1_default_agg, flow_stats);
            printf("-------------------------------\n");
            printf(" Flow statisics Default flow Bridge2  \n");
            printf("-------------------------------\n");
            App_ShowFlowAggStats(h_bridge2_default_agg, flow_stats);
            printf("-------------------------------\n");
            printf(" Flow statisics MLPPP ENET2  \n");
            printf("-------------------------------\n");
            App_ShowFlowAggStats(h_flow_agg_enet2_mlppp, flow_stats);
            
            App_ShowMLPPPBundleStats();
            break;
         case 6:
            return;
            break;
         default:
            printf("Invalid Entry\n");
            break;
      }
   }while(1);
   
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
    WP_IwPortStatisticsReset(h_bridge1_bport_mlppp, &bport_stats);
    WP_IwPortStatisticsReset(h_bridge2_bport_mlppp, &bport_stats);
	
    for(ii = 0; ii < APP_MAX_TDM_PORTS; ii++)
    {
        WP_DeviceStatisticsReset(h_dev_tdm[ii], &upi_hdlc_stats);
        
        WP_IwPortStatisticsReset(h_bridge1_bport_tdm[ii], &bport_stats);
        WP_IwPortStatisticsReset(h_bridge2_bport_tdm[ii], &bport_stats);

        WP_IwFlowStatisticsReset(h_flow_agg_tdm[ii], WP_IW_FLOW_STAT, &flow_stats);
        WP_IwFlowStatisticsReset(h_flow_agg_pppsw_link[ii], WP_IW_FLOW_STAT, &flow_stats);
    }
 
    WP_IwFlowStatisticsReset(h_flow_agg_pppsw_link_mlppp, WP_IW_FLOW_STAT, &flow_stats);
    WP_IwFlowStatisticsReset(h_flow_agg_enet2_mlppp, WP_IW_FLOW_STAT, &flow_stats);
	


    
}

/*****************************************************************************
 * Function name: App_ShowImaGroupStats
 * Description  : Display IMA group statistics
 * Input  params: None
 * Output params: None
 * Return val   : None
 *****************************************************************************/
void App_CheckTrafficStats(WP_status *test_result)
{
   WP_status status;
   WP_stats_enet s_hs_enet={0};
   WP_stats_tdi_hdlc tdi_hdlc_stats={0};
   WP_bport_stats bport_stats = {0};
   WP_iw_flow_stats flow_stats = {0};
   WP_stats_mp_bundle mp_bundle_stats={0};
   WP_stats_mp_link stats_mp_link = {0};
   WP_U8 ii;

   *test_result = WP_OK;

   /*Check enet1 stats*/
   memset(&s_hs_enet,0,sizeof(s_hs_enet));
   status = WP_DeviceStatistics(h_device_enet1, &s_hs_enet);
   App_TerminateOnError(status, "WP_DeviceStatistics");

   if (s_hs_enet.txrx_frames_127 != APP_MAX_PKTS_NUM * 2){
   	*test_result = (-1);
       printf("ENET1 txrx_frames_64 check fail!\n");
   }

   if (s_hs_enet.rx_packets != APP_MAX_PKTS_NUM){
   	*test_result = (-1);
       printf("ENET1 rx_packets check fail!\n");
   }
   
   if (s_hs_enet.tx_packets != APP_MAX_PKTS_NUM){
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

   if (s_hs_enet.txrx_frames_127 != APP_MAX_PKTS_NUM){
      *test_result = (-1);
       printf("ENET2 txrx_frames_127 check fail!\n");
   }
   if (s_hs_enet.tx_packets != APP_MAX_PKTS_NUM){
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

   /*Check tdm device stats*/
   for(ii = 0;ii < APP_MAX_TDM_PORTS;ii++)
   {
      memset(&tdi_hdlc_stats,0,sizeof(WP_stats_tdi_hdlc));
      status = WP_DeviceStatistics(h_dev_tdm[ii],&tdi_hdlc_stats);
      App_TerminateOnError(status, "WP_Devicestatistics");

      if (tdi_hdlc_stats.rx_frames == 0){
         *test_result = (-1);
	  printf("TDM device %d rx_frames check fail!\n", ii);

      }
      if (tdi_hdlc_stats.tx_frames == 0){
         *test_result = (-1);
	  printf("TDM device %d tx_frames check fail!\n", ii);
      	}
       /*Check tdm errors*/
	if (tdi_hdlc_stats.rx_err_crc || tdi_hdlc_stats.rx_err_abort ||
	    tdi_hdlc_stats.rx_err_addr_mismatch || tdi_hdlc_stats.rx_err_buffer_overrun ||
	    tdi_hdlc_stats.rx_err_overrun || tdi_hdlc_stats.rx_err_iw_buffer_underrun || 
	    tdi_hdlc_stats.rx_err_iw_mru || tdi_hdlc_stats.tx_err_underrun || 
	    tdi_hdlc_stats.rx_err_maxsdu || tdi_hdlc_stats.rx_err_non_octet || 
	    tdi_hdlc_stats.rx_err_iw_l2_parse
	    )
	{
	  *test_result = (-1);
	  printf("TDM device %d error check fail!\n", ii);
	}
	  
   }

   /*Check enet1 bport stats*/
   memset(&bport_stats, 0, sizeof(bport_stats));
   status = WP_IwPortStatistics(h_bridge1_bport_enet1, &bport_stats);
   App_TerminateOnError(status, "WP_BportStatistics");

   if (bport_stats.rx_valid_packets != APP_MAX_PKTS_NUM)
   {
      *test_result = (-1);
       printf("ENET7 bport stats rx_valid_packets check fail!\n");
   }

   if (bport_stats.protocol_error || bport_stats.bad_ip_version || 
   	bport_stats.checksum_error || bport_stats.ip_protocol_error)
   {
   	*test_result = (-1);
	printf("ENET7 bport error check fail!\n");
   }

   /*Check TDM Bridge1 port stats*/
   for(ii = 0;ii < APP_MAX_TDM_PORTS;ii++)
   {
      memset(&bport_stats, 0, sizeof(bport_stats));
      status = WP_IwPortStatistics(h_bridge1_bport_tdm[ii], &bport_stats);
      App_TerminateOnError(status, "WP_BportStatistics");

/*      if (bport_stats.rx_valid_packets != 0)
      {
         *test_result = (-1);
          printf("Bridge1 port TMD %d stats rx_valid_packets check fail!\n", ii);
      }*/

      if (bport_stats.protocol_error || bport_stats.bad_ip_version || 
      	   bport_stats.checksum_error || bport_stats.ip_protocol_error)
      {
          *test_result = (-1);
	   printf("Bridge1 port TDM %d error check fail!\n", ii);
      }
   }

   /*Check TDM Bridge2 port stats*/
   for(ii = 0;ii < APP_MAX_TDM_PORTS;ii++)
   {
      memset(&bport_stats, 0, sizeof(bport_stats));
      status = WP_IwPortStatistics(h_bridge2_bport_tdm[ii], &bport_stats);
      App_TerminateOnError(status, "WP_BportStatistics");

/*      if (bport_stats.rx_valid_packets != 0)
      {
         *test_result = (-1);
          printf("Bridge2 port TDM %d stats rx_valid_packets check fail!\n", ii);
      }*/

      if (bport_stats.protocol_error || bport_stats.bad_ip_version || 
      	   bport_stats.checksum_error || bport_stats.ip_protocol_error)
      {
          *test_result = (-1);
	   printf("Bridge 2 port TDM %d error check fail!\n", ii);
      }
   }

   /*Check MLPPP Bridge2 port stats*/
   memset(&bport_stats, 0, sizeof(bport_stats));
   status = WP_IwPortStatistics(h_bridge2_bport_mlppp, &bport_stats);
   App_TerminateOnError(status, "WP_BportStatistics");

   if (bport_stats.rx_valid_packets != APP_MAX_PKTS_NUM)
   {
      *test_result = (-1);
       printf("Bridge 2 port MLPPP stats rx_valid_packets check fail!\n");
   }

   if (bport_stats.protocol_error || bport_stats.bad_ip_version || 
   	bport_stats.checksum_error || bport_stats.ip_protocol_error)
   {
   	*test_result = (-1);
	printf("Bridge 2 port MLPPP error check fail!\n");
   }

   /*Check Flow statistics MLPPP at PPP switch*/
   memset(&flow_stats,0,sizeof(flow_stats));
   status = WP_IwFlowStatistics(h_flow_agg_pppsw_link_mlppp,WP_IW_FLOW_STAT,&flow_stats);
   App_TerminateOnError(status, "WP_IwFlowStatistics");

   if (flow_stats.forward_packet != APP_MAX_PKTS_NUM)
   {
      *test_result = (-1);
       printf("Flow statistics MLPPP at PPP switch forward_packet check fail!\n");
   }
   
   /*Check Flow statisics MLPPP ENET2*/
   memset(&flow_stats,0,sizeof(flow_stats));
   status = WP_IwFlowStatistics(h_flow_agg_enet2_mlppp,WP_IW_FLOW_STAT,&flow_stats);
   App_TerminateOnError(status, "WP_IwFlowStatistics");

   if (flow_stats.forward_packet != APP_MAX_PKTS_NUM)
   {
      *test_result = (-1);
       printf("Flow statisics MLPPP ENET2 forward_packet check fail!\n");
   }

   /* MLPPP Tx Bundle statisics*/
   memset(&mp_bundle_stats,0,sizeof(mp_bundle_stats));
   status = WP_MpBundleStatistics(mlppp_group_handle[0],&mp_bundle_stats);
   App_TerminateOnError(status, "WP_MpBundleStatistics");

   if (mp_bundle_stats.rx_iw_frames != APP_MAX_PKTS_NUM)
   {
      *test_result = (-1);
       printf("MLPPP Tx Bundle statisics rx_iw_frames check fail!\n");
   }

   if (mp_bundle_stats.tx_frames != APP_MAX_PKTS_NUM)
   {
      *test_result = (-1);
       printf("MLPPP Tx Bundle statisics tx_frames check fail!\n");
   }

   for(ii=0; ii < 2; ii++)
   {
      memset(&stats_mp_link,0,sizeof(WP_stats_mp_link));
      status = WP_MpLinkStatistics( mlppp_group_handle[0], h_dev_tdm[ii], &stats_mp_link);
      App_TerminateOnError(status, "WP_MpLinkStatistics");

      if (stats_mp_link.tx_fragments == 0)
      {
          *test_result = (-1);
          printf("MLPPP Links statisics link %d tx_fragments check fail!\n", ii);
      }

      if (stats_mp_link.rx_fragments == 0)
      {
          *test_result = (-1);
          printf("MLPPP Links statisics link %d rx_fragments check fail!\n", ii);
      }
   }

   /*Dump all statistics if failed*/
   if (*test_result != WP_OK)
   {
            printf("-------------------------------\n");
            printf(" ENET1 statisics               \n");
            printf("-------------------------------\n");
            App_ShowEnetDeviceStats(h_device_enet1, s_hs_enet);
            printf("-------------------------------\n");
            printf(" ENET2 statisics               \n");
            printf("-------------------------------\n");
            App_ShowEnetDeviceStats(h_device_enet2, s_hs_enet);
            
            printf("-------------------------------\n");
            printf("Bridge Port Statistics ENET7 \n");
            printf("-------------------------------\n");
            App_ShowBridgePortStats(h_bridge1_bport_enet1, bport_stats);
            for(ii = 0;ii < APP_MAX_TDM_PORTS;ii++)
            {
               printf("-------------------------------\n");
               printf("Bridge1 Port Statistics TDM %d\n",ii);
               printf("-------------------------------\n");
               App_ShowBridgePortStats(h_bridge1_bport_tdm[ii], bport_stats);
            }
            for(ii = 0;ii < APP_MAX_TDM_PORTS;ii++)
            {
               printf("-------------------------------\n");
               printf("Bridge2 Port Statistics TDM %d\n",ii);
               printf("-------------------------------\n");
               App_ShowBridgePortStats(h_bridge2_bport_tdm[ii], bport_stats);
            }
            printf("-------------------------------\n");
            printf("Bridge2 Port Statistics MLPPP  \n");
            printf("-------------------------------\n");
            App_ShowBridgePortStats(h_bridge2_bport_mlppp, bport_stats);
            
            for(ii = 0;ii < APP_MAX_TDM_PORTS;ii++)
            {
               printf("-------------------------------\n");
               printf(" Flow statistics TDM %d\n",ii);
               printf("-------------------------------\n");
               App_ShowFlowAggStats(h_flow_agg_tdm[ii], flow_stats);
            }
            for(ii = 0;ii < APP_MAX_TDM_PORTS;ii++)
            {
               printf("-------------------------------\n");
               printf(" Flow statistics PPPSW %d\n",ii);
               printf("-------------------------------\n");
               App_ShowFlowAggStats(h_flow_agg_pppsw_link[ii], flow_stats);
            }
            printf("-------------------------------\n");
            printf(" Flow statistics MLPPP at PPP switch\n");
            printf("-------------------------------\n");
            App_ShowFlowAggStats(h_flow_agg_pppsw_link_mlppp, flow_stats);
            printf("-------------------------------\n");
            printf(" Flow statistics MLPPP control at Bridge2 \n");
            printf("-------------------------------\n");
            App_ShowFlowAggStats(h_flow_agg_mlppp_control, flow_stats);
            printf("-------------------------------\n");
            printf(" Flow statisics Default flow Bridge1  \n");
            printf("-------------------------------\n");
            App_ShowFlowAggStats(h_bridge1_default_agg, flow_stats);
            printf("-------------------------------\n");
            printf(" Flow statisics Default flow Bridge2  \n");
            printf("-------------------------------\n");
            App_ShowFlowAggStats(h_bridge2_default_agg, flow_stats);
            printf("-------------------------------\n");
            printf(" Flow statisics MLPPP ENET2  \n");
            printf("-------------------------------\n");
            App_ShowFlowAggStats(h_flow_agg_enet2_mlppp, flow_stats);
            
            App_ShowMLPPPBundleStats();
   }

   return;
}

/*****************************************************************************
 * Function name: App_ShowImaGroupStats
 * Description  : Display IMA group statistics
 * Input  params: None
 * Output params: None
 * Return val   : None
 *****************************************************************************/
void App_CheckControlStats(WP_status *test_result)
{
   WP_status status;
   WP_stats_enet s_hs_enet={0};
   WP_bport_stats bport_stats = {0};
   WP_iw_flow_stats flow_stats = {0};
   WP_U32 stats_sum1 = 0;
   WP_U8 ii;

   *test_result = WP_OK;

   /*Check enet1 stats*/
   memset(&s_hs_enet,0,sizeof(s_hs_enet));
   status = WP_DeviceStatistics(h_device_enet1, &s_hs_enet);
   App_TerminateOnError(status, "WP_DeviceStatistics");

   if (s_hs_enet.txrx_frames_127 != APP_MAX_PKTS_NUM * 2){
   	*test_result = (-1);
       printf("ENET1 txrx_frames_64 check fail!\n");
   }

   if (s_hs_enet.rx_packets != APP_MAX_PKTS_NUM){
   	*test_result = (-1);
       printf("ENET1 rx_packets check fail!\n");
   }
   
   if (s_hs_enet.tx_packets != APP_MAX_PKTS_NUM){
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
   
   /*Check ENET2 stats*/
   memset(&s_hs_enet,0,sizeof(s_hs_enet));
   status = WP_DeviceStatistics(h_device_enet2, &s_hs_enet);
   App_TerminateOnError(status, "WP_DeviceStatistics");

   if (s_hs_enet.txrx_frames_127 != 0){
      *test_result = (-1);
       printf("ENET2 txrx_frames_127 check fail!\n");
   }
   if (s_hs_enet.tx_packets != 0){
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
   
   /*Check enet1 bport stats*/
   memset(&bport_stats, 0, sizeof(bport_stats));
   status = WP_IwPortStatistics(h_bridge1_bport_enet1, &bport_stats);
   App_TerminateOnError(status, "WP_BportStatistics");

   if (bport_stats.rx_valid_packets != APP_MAX_PKTS_NUM)
   {
      *test_result = (-1);
       printf("ENET7 bport stats rx_valid_packets check fail!\n");
   }

   if (bport_stats.protocol_error || bport_stats.bad_ip_version || 
   	bport_stats.checksum_error || bport_stats.ip_protocol_error)
   {
   	*test_result = (-1);
	printf("ENET7 bport error check fail!\n");
   }

   /*Check TDM Bridge1 port stats*/
   stats_sum1 = 0;
   
   for(ii = 0;ii < APP_MAX_TDM_PORTS;ii++)
   {
      memset(&bport_stats, 0, sizeof(bport_stats));
      status = WP_IwPortStatistics(h_bridge1_bport_tdm[ii], &bport_stats);
      App_TerminateOnError(status, "WP_BportStatistics");

      stats_sum1 += bport_stats.forwarded_uc_packets;

      if (bport_stats.protocol_error || bport_stats.bad_ip_version || 
          bport_stats.checksum_error || bport_stats.ip_protocol_error)
      {
          *test_result = (-1);
          printf("Bridge1 port TDM %d error check fail!\n", ii);
      }
   }
   
   if (stats_sum1 == 0)
   {
      *test_result = (-1);
       printf("Bridge1 port TMD stats rx_valid_packets check fail!\n");
   }

   /*Check TDM Bridge2 port stats*/
   stats_sum1 = 0;
   
   for(ii = 0;ii < APP_MAX_TDM_PORTS;ii++)
   {
      memset(&bport_stats, 0, sizeof(bport_stats));
      status = WP_IwPortStatistics(h_bridge2_bport_tdm[ii], &bport_stats);
      App_TerminateOnError(status, "WP_BportStatistics");

      stats_sum1 += bport_stats.rx_valid_packets;

      if (bport_stats.protocol_error || bport_stats.bad_ip_version || 
          bport_stats.checksum_error || bport_stats.ip_protocol_error)
      {
          *test_result = (-1);
	   printf("Bridge2 port TDM %d error check fail!\n", ii);
      }
   }

   if (stats_sum1 == 0)
   {
      *test_result = (-1);
       printf("Bridge2 port TMD stats rx_valid_packets check fail!\n");
   }
   
   /*Check TDM 1 flow agg stats*/
   stats_sum1 = 0;
   
   for(ii = 0;ii < APP_MAX_TDM_PORTS;ii++)
   {
      memset(&flow_stats,0,sizeof(flow_stats));
      status = WP_IwFlowStatistics(h_flow_agg_tdm[ii],WP_IW_FLOW_STAT,&flow_stats);
      App_TerminateOnError(status, "WP_IwFlowStatistics");

      stats_sum1 += flow_stats.forward_packet;
   }

   if (stats_sum1 == 0)
   {
      *test_result = (-1);
       printf("Flow statistics TDM forward_packet check fail!\n");
   }

   /*Check PPPSW 1 flow agg stats*/
   stats_sum1 = 0;
   
   for(ii = 0;ii < APP_MAX_TDM_PORTS;ii++)
   {
      memset(&flow_stats,0,sizeof(flow_stats));
      status = WP_IwFlowStatistics(h_flow_agg_pppsw_link[ii],WP_IW_FLOW_STAT,&flow_stats);
      App_TerminateOnError(status, "WP_IwFlowStatistics");

      stats_sum1 += flow_stats.forward_packet;
   }

   if (stats_sum1 == 0)
   {
      *test_result = (-1);
       printf("Flow statistics TDM forward_packet check fail!\n");
   }
   
   /*Check Flow statisics MLPPP control at Bridge2*/
   memset(&flow_stats,0,sizeof(flow_stats));
   status = WP_IwFlowStatistics(h_flow_agg_mlppp_control,WP_IW_FLOW_STAT,&flow_stats);
   App_TerminateOnError(status, "WP_IwFlowStatistics");

   if (flow_stats.forward_packet != APP_MAX_PKTS_NUM)
   {
      *test_result = (-1);
       printf("Flow statisics MLPPP control at Bridge2 forward_packet check fail!\n");
   }

   /*Dump all statistics if failed*/
   if (*test_result != WP_OK)
   {
            printf("-------------------------------\n");
            printf(" ENET1 statisics               \n");
            printf("-------------------------------\n");
            App_ShowEnetDeviceStats(h_device_enet1, s_hs_enet);
            printf("-------------------------------\n");
            printf(" ENET2 statisics               \n");
            printf("-------------------------------\n");
            App_ShowEnetDeviceStats(h_device_enet2, s_hs_enet);
            
            printf("-------------------------------\n");
            printf("Bridge Port Statistics ENET7 \n");
            printf("-------------------------------\n");
            App_ShowBridgePortStats(h_bridge1_bport_enet1, bport_stats);
            for(ii = 0;ii < APP_MAX_TDM_PORTS;ii++)
            {
               printf("-------------------------------\n");
               printf("Bridge1 Port Statistics TDM %d\n",ii);
               printf("-------------------------------\n");
               App_ShowBridgePortStats(h_bridge1_bport_tdm[ii], bport_stats);
            }
            for(ii = 0;ii < APP_MAX_TDM_PORTS;ii++)
            {
               printf("-------------------------------\n");
               printf("Bridge2 Port Statistics TDM %d\n",ii);
               printf("-------------------------------\n");
               App_ShowBridgePortStats(h_bridge2_bport_tdm[ii], bport_stats);
            }
            printf("-------------------------------\n");
            printf("Bridge2 Port Statistics MLPPP  \n");
            printf("-------------------------------\n");
            App_ShowBridgePortStats(h_bridge2_bport_mlppp, bport_stats);
            
            for(ii = 0;ii < APP_MAX_TDM_PORTS;ii++)
            {
               printf("-------------------------------\n");
               printf(" Flow statistics TDM %d\n",ii);
               printf("-------------------------------\n");
               App_ShowFlowAggStats(h_flow_agg_tdm[ii], flow_stats);
            }
            for(ii = 0;ii < APP_MAX_TDM_PORTS;ii++)
            {
               printf("-------------------------------\n");
               printf(" Flow statistics PPPSW %d\n",ii);
               printf("-------------------------------\n");
               App_ShowFlowAggStats(h_flow_agg_pppsw_link[ii], flow_stats);
            }
            printf("-------------------------------\n");
            printf(" Flow statistics MLPPP at PPP switch\n");
            printf("-------------------------------\n");
            App_ShowFlowAggStats(h_flow_agg_pppsw_link_mlppp, flow_stats);
            printf("-------------------------------\n");
            printf(" Flow statistics MLPPP control at Bridge2 \n");
            printf("-------------------------------\n");
            App_ShowFlowAggStats(h_flow_agg_mlppp_control, flow_stats);
            printf("-------------------------------\n");
            printf(" Flow statisics Default flow Bridge1  \n");
            printf("-------------------------------\n");
            App_ShowFlowAggStats(h_bridge1_default_agg, flow_stats);
            printf("-------------------------------\n");
            printf(" Flow statisics Default flow Bridge2  \n");
            printf("-------------------------------\n");
            App_ShowFlowAggStats(h_bridge2_default_agg, flow_stats);
            printf("-------------------------------\n");
            printf(" Flow statisics MLPPP ENET2  \n");
            printf("-------------------------------\n");
            App_ShowFlowAggStats(h_flow_agg_enet2_mlppp, flow_stats);
            
            App_ShowMLPPPBundleStats();
   }

   return;
}

void App_CheckStats(WP_status *test_result)
{
    if (TRAFFIC_DATA == interface_ch)
    {
        App_CheckTrafficStats(test_result);
    }
    else
    if (CONTROL_DATA == interface_ch)
    {
        App_CheckControlStats(test_result);
    }
}
