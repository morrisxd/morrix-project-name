#ifndef STATS_PRINTF
#define STATS_PRINTF MY_PRINTF
#endif

#ifdef PRINT_STAT_FIELD
#undef PRINT_STAT_FIELD
#endif
#define PRINT_STAT_FIELD(field) STATS_PRINTF( "%08x%08x", (WP_U32)(field>>32), (WP_U32)field)

#define MY_PRINTF_DATA_UNIT(data_unit) \
do \
{\
   MY_PRINTF( "data_unit.segment->data: 0x%x\n", (WP_U32)data_unit.segment->data); \
   MY_PRINTF( "\n\n\n"                                \
      "data_unit.type                     0x%08X\n"   \
      "data_unit.data_size                0x%08X\n"   \
      "data_unit.n_active                 0x%08X\n"   \
      "data_unit.n_segments               0x%08X\n"   \
      "data_unit.control                  0x%08X\n"   \
      "data_unit.segment.data_size        0x%08X\n"   \
      "data_unit.segment.data             0x%08X\n"   \
      "data_unit.segment.pool_handle      0x%08X\n"   \
      "data_unit.segment.next             0x%08X\n"   \
      "data_unit.segment.displacement     0x%08X\n",  \
      data_unit.type,                                 \
      data_unit.data_size,                            \
      data_unit.n_active,                             \
      data_unit.n_segments,                           \
      data_unit.control,                              \
      data_unit.segment->data_size,                   \
      (WP_U32) data_unit.segment->data,               \
      data_unit.segment->pool_handle,                 \
      (WP_U32) data_unit.segment->next,               \
      data_unit.segment->displacement);               \
   MY_PRINTF("Packet Data:");                         \
   MY_PRINTF("ascii_buffer = %s\n", ascii_buffer);    \
} while (0)


/*****************************************************************************
 * Function name: WTE_IpChecksumCalc
 * Description  : calculate checksum of IP header
 * Input  params: buff - pointer to buffer, len - size of IP header
 * Output params:
 * Return val   : calculated checksum
 *****************************************************************************/
WP_U16 WTE_IpChecksumCalc(WP_U8 *buff, WP_U16 len)
{
   WP_U16 bu16[100];
   WP_U32 sum=0;
   WP_U16 i;
   memcpy(bu16,buff,len);
   /* make 16 bit words out of every two adjacent
      8 bit words in the packet and add them up */
   len = len/2;
   for (i=0;i<len;i++){
      sum = sum + (WP_U32) bu16[i];
   }

   /* take only 16 bits out of the 32 bit sum
      and add up the carries */
   while (sum>>16)
      sum = (sum & 0xFFFF)+(sum >> 16);

   /* one's complement the result */
   sum = ~sum;

   return ((WP_U16) sum);
}

/* terminate_on_error */
void terminate_on_error(WP_handle handle, WP_CHAR *s)
{
   if (WP_ERROR_P(handle)) {
      printf("%s: Terminal error (handle 0x%x) %s\n", appname, handle, s);
      printf("%s: Error type - %s\n", appname, WP_error_name[WP_ERROR(handle)]);
      WT_Reboot();
      exit(1);
   }
#ifdef DEBUG_PRINT
   else
   {
         printf("%s: Handle/Status returned from %s is 0x%x \n", appname, s, handle);
   }
#endif
}

void WPE_TerminateOnError(WP_handle handle, WP_CHAR *s, WP_U32 line)
{
   if (WP_ERROR_P(handle))
   {
      printf("Test Abort %s %s 0x%x\n", s, WP_ErrorString(handle), handle);
      printf("Test Failed, Line : %d\n", line);
      WT_Reboot();
      exit(1);
   }
#ifdef DEBUG_PRINT
   else
   {
      printf("%s: Handle/Status returned from %s is %#8.8x \n", appname, s, handle);
   }
#endif
}

void WPE_TerminateOnErrorNoPrintIfOK(WP_handle handle, WP_CHAR *s, WP_U32 line)
{
   if (WP_ERROR_P(handle))
   {
      printf("Test Abort %s %s 0x%x\n", s, WP_ErrorString(handle), handle);
      printf("Test Failed, Line : %d\n", line);
      WT_Reboot();
      exit(1);
   }
}

/**
 * Displays Bridge Port Statistics.
 */
WP_U32 WPT_DisplayBPortStats( WP_handle port_handle, WP_CHAR* type)
{
   WP_U32 i;
   WP_U32 rx_packets = 0;
   WP_bport_stats bport_stats;
   memset( &bport_stats, 0, sizeof(WP_bport_stats) );
   WP_IwBridgingStatistics( port_handle, &bport_stats );
   STATS_PRINTF ("\n%s Bridge port statistics of port %d (%x)\n",
           type,(port_handle & 0x000000ff),port_handle);
   STATS_PRINTF ("============================================\n");
   STATS_PRINTF ("Rx Valid Packets:                       ");
   PRINT_STAT_FIELD(bport_stats.rx_valid_packets);
   STATS_PRINTF ("\nDiscard VLAN acceptable filter:         ");
   PRINT_STAT_FIELD(bport_stats.discard_vlan_acceptable_filter);
   STATS_PRINTF ("\nDiscard ingress filter:                 ");
   PRINT_STAT_FIELD(bport_stats.discard_ingress_filter);
   STATS_PRINTF ("\nDiscard bridge classifier:              ");
   PRINT_STAT_FIELD(bport_stats.discard_bridge_classifier);
   STATS_PRINTF ("\nDiscard unknown MAC SA:                 ");
   PRINT_STAT_FIELD(bport_stats.discard_unk_macsa);
   /* STATS_PRINTF ("\nDiscard egress filter:                  ");
   PRINT_STAT_FIELD(bport_stats.discard_egress_filter); */
   STATS_PRINTF ("\nDeny MAC SA:                            ");
   PRINT_STAT_FIELD(bport_stats.deny_mac_sa);
   STATS_PRINTF ("\nDeny MAC DA:                            ");
   PRINT_STAT_FIELD(bport_stats.deny_mac_da);
   STATS_PRINTF ("\nRx BC valid packets:                    ");
   PRINT_STAT_FIELD(bport_stats.rx_bc_valid_packets);
   STATS_PRINTF ("\nRx MC valid packets:                    ");
   PRINT_STAT_FIELD(bport_stats.rx_mc_valid_packets);
   STATS_PRINTF ("\nForwarded UC packets:                   ");
   PRINT_STAT_FIELD(bport_stats.forwarded_uc_packets);
   STATS_PRINTF ("\nForwarded BC packets:                   ");
   PRINT_STAT_FIELD(bport_stats.forwarded_bc_packets);
   STATS_PRINTF ("\nForwarded MC packets:                   ");
   PRINT_STAT_FIELD(bport_stats.forwarded_mc_packets);
   STATS_PRINTF ("\nProtocol Error      :                   ");
   PRINT_STAT_FIELD(bport_stats.protocol_error);

   for(i=0; i<WP_IW_DFC_MAX_NUM_OF_FILTERS; i++)
   {
      if(bport_stats.discard_dfc_filter[i])
      {
         STATS_PRINTF ("\ndiscard_dfc_filter %03d:                 ", i);
         PRINT_STAT_FIELD(bport_stats.discard_dfc_filter[i]);
      }
   }

   for(i=0; i<WP_PCE_MAX_NUM_OF_FILTERS; i++)
   {
      if(bport_stats.discard_pce_filter[i])
      {
         STATS_PRINTF ("\ndiscard_pce_filter %03d:                 ", i);
         PRINT_STAT_FIELD(bport_stats.discard_pce_filter[i]);
      }
   }

   STATS_PRINTF ("\ningress_policer_non_conforming_packets: ");
   PRINT_STAT_FIELD(bport_stats.ingress_policer_non_conforming_packets);

   STATS_PRINTF("\n\n");
   rx_packets = (WP_U32)bport_stats.rx_valid_packets;
   return rx_packets;
}

/**
 * Displays Route Port Statistics.
 */
WP_U32 WPT_DisplayRoutePortStats(WP_handle port_handle, WP_CHAR* type)
{
   WP_U32 i;
   WP_U32 rx_packets = 0;
   WP_route_port_stats rport_stats;
   memset( &rport_stats, 0, sizeof(WP_route_port_stats));
   WP_IwPortStatistics(port_handle, &rport_stats );
   STATS_PRINTF ("\n%s Route port statistics of port %d (%x)\n",
           type,(port_handle & 0x000000ff),port_handle);
   STATS_PRINTF ("\n============================================\n");
   STATS_PRINTF ("\nRx Valid Packets:                       ");
   PRINT_STAT_FIELD(rport_stats.rx_valid_packets);
   STATS_PRINTF ("\nrx_ipv4_option_packets:                 ");
   PRINT_STAT_FIELD(rport_stats.rx_ipv4_option_packets);
   STATS_PRINTF ("\nrx_non_ip_packets:                      ");
   PRINT_STAT_FIELD(rport_stats.rx_non_ip_packets);
   STATS_PRINTF ("\nrx_compressed_packets:                  ");
   PRINT_STAT_FIELD(rport_stats.rx_compressed_packets);
   STATS_PRINTF ("\nrx_valid_mpls_packets:                  ");
   PRINT_STAT_FIELD(rport_stats.rx_valid_mpls_packets);
   STATS_PRINTF ("\nrx_protocol_error:                      ");
   PRINT_STAT_FIELD(rport_stats.rx_protocol_error);
   STATS_PRINTF ("\nrx_checksum_error:                      ");
   PRINT_STAT_FIELD(rport_stats.rx_checksum_error);
   STATS_PRINTF ("\nrx_discard_classifier:                  ");
   PRINT_STAT_FIELD(rport_stats.rx_discard_classifier);
   STATS_PRINTF ("\nrx_mpls_lookup_drop:                    ");
   PRINT_STAT_FIELD(rport_stats.rx_mpls_lookup_drop);
   STATS_PRINTF ("\ntx_forwarded_packets:                   ");
   PRINT_STAT_FIELD(rport_stats.tx_forwarded_packets);
   STATS_PRINTF ("\nrx_ipv6_hop_by_hop_host_term:           ");
   PRINT_STAT_FIELD(rport_stats.rx_ipv6_hop_by_hop_host_term);
   STATS_PRINTF ("\nrx_ipv6_link_local_host_term:           ");
   PRINT_STAT_FIELD(rport_stats.rx_ipv6_link_local_host_term);
   STATS_PRINTF ("\nrx_discard_lpm:                         ");
   PRINT_STAT_FIELD(rport_stats.rx_discard_lpm);

   for(i=0; i<WP_IW_DFC_MAX_NUM_OF_FILTERS; i++)
   {
      if(rport_stats.discard_dfc_filter[i])
      {
         STATS_PRINTF ("\ndiscard_dfc_filter %03d:                 ", i);
         PRINT_STAT_FIELD(rport_stats.discard_dfc_filter[i]);
      }
   }

   for(i=0; i<WP_PCE_MAX_NUM_OF_FILTERS; i++)
   {
      if(rport_stats.discard_pce_filter[i])
      {
         STATS_PRINTF ("\ndiscard_pce_filter %03d:                 ", i);
         PRINT_STAT_FIELD(rport_stats.discard_pce_filter[i]);
      }
   }

   STATS_PRINTF ("\ningress_policer_non_conforming_packets: ");
   PRINT_STAT_FIELD(rport_stats.ingress_policer_non_conforming_packets);

   STATS_PRINTF("\n\n");
   rx_packets = (WP_U32)rport_stats.rx_valid_packets;
   return rx_packets;
}

/**
 * Return Flow Aggregation's number of forwarded packets.
 */
WP_U32 WPT_GetFlowForwardedPackets(WP_handle flow_aggregation)
{
   WP_iw_flow_stats iw_flow_stats;
   WP_U32 forward_packets = 0;

   memset(&iw_flow_stats,0,sizeof(iw_flow_stats));
   WP_IwFlowStatistics(flow_aggregation,
                       WP_IW_FLOW_STAT,
                       &iw_flow_stats);
   forward_packets = (WP_U32)iw_flow_stats.forward_packet;
   MY_PRINTF("Get Flow Aggregation 0x%08X fwd packets: %d\n", flow_aggregation, forward_packets);
   return forward_packets;
}


/**
 * Displays Flow Aggregations Statistics.
 */
WP_U32 WPT_DisplayFlowStats(WP_handle agg, WP_CHAR *type)
{
   WP_iw_flow_stats iw_flow_stats;
   WP_U32 forward_packets = 0;

   memset(&iw_flow_stats,0,sizeof(iw_flow_stats));
   WP_IwFlowStatistics(agg,
                       WP_IW_FLOW_STAT,
                       &iw_flow_stats);
   STATS_PRINTF ("\n%s Aggregation flow statistics of flow %d (%x)\n",
           type,(agg & 0x000000ff),agg);
   STATS_PRINTF ("==========================================================\n");
   STATS_PRINTF ("Forward Packet:          ");
   PRINT_STAT_FIELD(iw_flow_stats.forward_packet);
   STATS_PRINTF( "\nFBP Drop Packets:        ");
   PRINT_STAT_FIELD(iw_flow_stats.fbp_drop_packets);
   STATS_PRINTF( "\nMTU Drop Packets:        ");
   PRINT_STAT_FIELD(iw_flow_stats.mtu_drop_packets);
   STATS_PRINTF( "\nTTL Drop Packets:        ");
   PRINT_STAT_FIELD(iw_flow_stats.ttl_drop_packets);
   STATS_PRINTF( "\nTX Queue Drop Packets:   ");
   PRINT_STAT_FIELD(iw_flow_stats.tx_queue_drop_packets);
   STATS_PRINTF( "\nMPLS Drop:               ");
   PRINT_STAT_FIELD(iw_flow_stats.mpls_drop);
   STATS_PRINTF("\n\n");
   forward_packets = (WP_U32)iw_flow_stats.forward_packet;
   return forward_packets;
}

/**
 * Displays Generic Flow Aggregations Statistics.
 */
WP_U32 WPT_DisplayGenericFlowStats(WP_handle agg, WP_CHAR *type)
{
   WP_iw_generic_flow_stats iw_flow_stats;
   WP_U32 forward_packets = 0;

   memset(&iw_flow_stats,0,sizeof(iw_flow_stats));
   WP_IwFlowStatistics(agg,
                       WP_IW_FLOW_STAT_GENERIC,
                       &iw_flow_stats);
   STATS_PRINTF ("\n%s Generic Flow Aggregation (%x) statistics \n",
           type, agg);
   STATS_PRINTF ("==========================================================\n");
   STATS_PRINTF ("Forward Packet:                  ");
   PRINT_STAT_FIELD(iw_flow_stats.forward_packet);
   STATS_PRINTF ("\nForwarded Bytes:                 ");
   PRINT_STAT_FIELD(iw_flow_stats.forwarded_bytes);
   STATS_PRINTF( "\nMTU Drop Packets:                ");
   PRINT_STAT_FIELD(iw_flow_stats.mtu_drop_packets);
   STATS_PRINTF( "\nTX Queue Drop Packets:           ");
   PRINT_STAT_FIELD(iw_flow_stats.tx_queue_drop_packets);
   STATS_PRINTF( "\nDenied Packets:                  ");
   PRINT_STAT_FIELD(iw_flow_stats.denied_packets);
   STATS_PRINTF( "\nPolicer Non-Conforming Packets:  ");
   PRINT_STAT_FIELD(iw_flow_stats.policer_non_conforming_packets);
   STATS_PRINTF("\n\n");
   forward_packets = (WP_U32)iw_flow_stats.forward_packet;
   return forward_packets;
}

/**
 * Displays UPI or ENET device statistics
 */

WP_U32 WPT_DisplayDeviceStats(WP_handle device_handle, WP_device device, WP_CHAR* type)
{
   WP_U32 temp1, temp2;
   WP_stats_enet stats_hs_enet = {0};
   WP_status status;
   STATS_PRINTF ("\n%s Device statistics of device %d (%x)\n",
           type,(device_handle & 0x000000ff),device_handle);
   STATS_PRINTF ("============================================\n");

   switch (device)
   {
      case WP_DEVICE_ENET:
         memset( &stats_hs_enet, 0, sizeof(WP_stats_enet) );
         if((status = WP_DeviceStatistics(device_handle, &stats_hs_enet)))
         {
            STATS_PRINTF("GetStatistics error\n");
            return 0;
         }
         STATS_PRINTF("\nStatistics for HS ENET device:\n");
         temp1 = stats_hs_enet.rx_packets;
         temp2 = stats_hs_enet.rx_packets >> 32;
         STATS_PRINTF("rx_packets:           %.8x%.8x\n", temp2,temp1);
         temp1 = stats_hs_enet.tx_packets;
         temp2 = stats_hs_enet.tx_packets >> 32;
         STATS_PRINTF("tx_packets:           %.8x%.8x\n", temp2,temp1);
         temp1 = stats_hs_enet.rx_err_overrun;
         temp2 = stats_hs_enet.rx_err_overrun >> 32;
         STATS_PRINTF("rx_err_overrun:       %.8x%.8x\n", temp2,temp1);
         temp1 = stats_hs_enet.rx_err_sdu;
         temp2 = stats_hs_enet.rx_err_sdu >> 32;
         STATS_PRINTF("rx_err_maxsdu:        %.8x%.8x\n", temp2,temp1);
         temp1 = stats_hs_enet.rx_err_nonvalid_mac;
         temp2 = stats_hs_enet.rx_err_nonvalid_mac >> 32;
         STATS_PRINTF("rx_err_mac_address:   %.8x%.8x\n", temp2,temp1);
         temp1 = stats_hs_enet.rx_err_mru;
         temp2 = stats_hs_enet.rx_err_mru >> 32;
         STATS_PRINTF("rx_err_mru:           %.8x%.8x\n", temp2,temp1);
         temp1 = stats_hs_enet.tx_err_underrun;
         temp2 = stats_hs_enet.tx_err_underrun >> 32;
         STATS_PRINTF("rx_err_underrun:      %.8x%.8x\n", temp2,temp1);
         temp1 = stats_hs_enet.rx_err_host_full;
         temp2 = stats_hs_enet.rx_err_host_full >> 32;
         STATS_PRINTF("rx_err_host_full:     %.8x%.8x\n", temp2,temp1);
         temp1 = stats_hs_enet.rx_err_fbp_underrun;
         temp2 = stats_hs_enet.rx_err_fbp_underrun >> 32;
         STATS_PRINTF("rx_err_fbp_underrun:  %.8x%.8x\n", temp2,temp1);
         temp1 = stats_hs_enet.rx_iw_frames;
         temp2 = stats_hs_enet.rx_iw_frames >> 32;
         STATS_PRINTF("rx_iw_frames:         %.8x%.8x\n", temp2,temp1);
         temp1 = stats_hs_enet.tx_frames;
         temp2 = stats_hs_enet.tx_frames >> 32;
         STATS_PRINTF("tx_good_frames:       %.8x%.8x\n", temp2,temp1);
         STATS_PRINTF("\n");
         temp1 = stats_hs_enet.rx_host_frames;
         temp2 = stats_hs_enet.rx_host_frames >> 32;
         STATS_PRINTF("rx_host_frames:       %.8x%.8x\n", temp2,temp1);
         STATS_PRINTF("\n");
/*
         STATS_PRINTF("SERIAL DPS IF tx_frames:               %.8x\n", stats_hs_enet.serial_dps_if_stats.tx_frames);
         STATS_PRINTF("SERIAL DPS IF tx_bytes:                %.8x\n", stats_hs_enet.serial_dps_if_stats.tx_bytes);
         STATS_PRINTF("SERIAL DPS IF rx_frames:               %.8x\n", stats_hs_enet.serial_dps_if_stats.rx_frames);
         STATS_PRINTF("SERIAL DPS IF rx_bytes:                %.8x\n", stats_hs_enet.serial_dps_if_stats.rx_bytes);
         STATS_PRINTF("SERIAL DPS IF rx_pce_denied_frames:    %.8x\n", stats_hs_enet.serial_dps_if_stats.rx_pce_denied_frames);
         STATS_PRINTF("SERIAL DPS IF rx_dropped_frames        %.8x\n", stats_hs_enet.serial_dps_if_stats.rx_dropped_frames);
*/
         return stats_hs_enet.rx_packets;
   }
   return 0;
}

void WTE_DisplayEnetDeviceStats(WP_handle device_handle, WP_CHAR* type)
{
   WP_U32 temp1, temp2;
   WP_stats_enet stats_hs_enet = {0};
   WP_status status;
   STATS_PRINTF ("\n%s Device statistics of device %d (%x)\n",
           type,(device_handle & 0x000000ff),device_handle);
   STATS_PRINTF ("============================================\n");

   memset( &stats_hs_enet, 0, sizeof(WP_stats_enet) );
   if((status = WP_DeviceStatistics(device_handle, &stats_hs_enet)))
   {
      STATS_PRINTF("GetStatistics error\n");
      return;
   }
   STATS_PRINTF("\nEnet device statistics:\n");
   STATS_PRINTF("Hardware statistics:\n");
   temp1 = stats_hs_enet.rx_packets;
   temp2 = stats_hs_enet.rx_packets >> 32;
   STATS_PRINTF("rx_packets:           %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.tx_packets;
   temp2 = stats_hs_enet.tx_packets >> 32;
   STATS_PRINTF("tx_packets:           %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.txrx_frames_64;
   temp2 = stats_hs_enet.txrx_frames_64 >> 32;
   STATS_PRINTF("txrx_frames_64:       %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.txrx_frames_127;
   temp2 = stats_hs_enet.txrx_frames_127 >> 32;
   STATS_PRINTF("txrx_frames_127:      %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.txrx_frames_255;
   temp2 = stats_hs_enet.txrx_frames_255 >> 32;
   STATS_PRINTF("txrx_frames_255:      %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.txrx_frames_511;
   temp2 = stats_hs_enet.txrx_frames_511 >> 32;
   STATS_PRINTF("txrx_frames_511:      %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.txrx_frames_1023;
   temp2 = stats_hs_enet.txrx_frames_1023 >> 32;
   STATS_PRINTF("txrx_frames_1023:     %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.txrx_frames_1518;
   temp2 = stats_hs_enet.txrx_frames_1518 >> 32;
   STATS_PRINTF("txrx_frames_1518:     %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.txrx_frames_1522;
   temp2 = stats_hs_enet.txrx_frames_1522 >> 32;
   STATS_PRINTF("txrx_frames_1522:     %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.rx_bytes;
   temp2 = stats_hs_enet.rx_bytes >> 32;
   STATS_PRINTF("rx_bytes:             %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.rx_err_fcs;
   temp2 = stats_hs_enet.rx_err_fcs >> 32;
   STATS_PRINTF("rx_err_fcs:           %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.rx_multicast;
   temp2 = stats_hs_enet.rx_multicast >> 32;
   STATS_PRINTF("rx_multicast:         %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.rx_broadcast;
   temp2 = stats_hs_enet.rx_broadcast >> 32;
   STATS_PRINTF("rx_broadcast:         %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.rx_mac_control;
   temp2 = stats_hs_enet.rx_mac_control >> 32;
   STATS_PRINTF("rx_mac_control:       %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.rx_mac_pause;
   temp2 = stats_hs_enet.rx_mac_pause >> 32;
   STATS_PRINTF("rx_mac_pause:         %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.rx_mac_unknown;
   temp2 = stats_hs_enet.rx_mac_unknown >> 32;
   STATS_PRINTF("rx_mac_unknown:       %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.rx_err_alignment;
   temp2 = stats_hs_enet.rx_err_alignment >> 32;
   STATS_PRINTF("rx_err_alignment:     %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.rx_err_length;
   temp2 = stats_hs_enet.rx_err_length >> 32;
   STATS_PRINTF("rx_err_length:        %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.rx_err_code;
   temp2 = stats_hs_enet.rx_err_code >> 32;
   STATS_PRINTF("rx_err_code:          %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.rx_false_carrier;
   temp2 = stats_hs_enet.rx_false_carrier >> 32;
   STATS_PRINTF("rx_false_carrier:     %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.rx_undersize;
   temp2 = stats_hs_enet.rx_undersize >> 32;
   STATS_PRINTF("rx_undersize:         %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.rx_oversize;
   temp2 = stats_hs_enet.rx_oversize >> 32;
   STATS_PRINTF("rx_oversize:          %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.rx_fragments;
   temp2 = stats_hs_enet.rx_fragments >> 32;
   STATS_PRINTF("rx_fragments:         %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.rx_jabber;
   temp2 = stats_hs_enet.rx_jabber >> 32;
   STATS_PRINTF("rx_jabber:            %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.rx_dropped;
   temp2 = stats_hs_enet.rx_dropped >> 32;
   STATS_PRINTF("rx_dropped:           %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.tx_bytes;
   temp2 = stats_hs_enet.tx_bytes >> 32;
   STATS_PRINTF("tx_bytes:             %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.tx_multicast;
   temp2 = stats_hs_enet.tx_multicast >> 32;
   STATS_PRINTF("tx_multicast:         %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.tx_broadcast;
   temp2 = stats_hs_enet.tx_broadcast >> 32;
   STATS_PRINTF("tx_broadcast:         %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.tx_mac_pause;
   temp2 = stats_hs_enet.tx_mac_pause >> 32;
   STATS_PRINTF("tx_mac_pause:         %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.tx_defer;
   temp2 = stats_hs_enet.tx_defer >> 32;
   STATS_PRINTF("tx_defer:             %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.tx_excess_defer;
   temp2 = stats_hs_enet.tx_excess_defer >> 32;
   STATS_PRINTF("tx_excess_defer:      %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.tx_single_collision;
   temp2 = stats_hs_enet.tx_single_collision >> 32;
   STATS_PRINTF("tx_single_collision:  %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.tx_multi_collision;
   temp2 = stats_hs_enet.tx_multi_collision >> 32;
   STATS_PRINTF("tx_multi_collision:   %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.tx_late_collision;
   temp2 = stats_hs_enet.tx_late_collision >> 32;
   STATS_PRINTF("tx_late_collision:    %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.tx_excess_collision;
   temp2 = stats_hs_enet.tx_excess_collision >> 32;
   STATS_PRINTF("tx_excess_collision:  %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.tx_no_collision;
   temp2 = stats_hs_enet.tx_no_collision >> 32;
   STATS_PRINTF("tx_no_collision:      %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.tx_mac_pause_honored;
   temp2 = stats_hs_enet.tx_mac_pause_honored >> 32;
   STATS_PRINTF("tx_mac_pause_honored: %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.tx_dropped;
   temp2 = stats_hs_enet.tx_dropped >> 32;
   STATS_PRINTF("tx_dropped:           %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.tx_jabber;
   temp2 = stats_hs_enet.tx_jabber >> 32;
   STATS_PRINTF("tx_jabber:            %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.tx_err_fcs;
   temp2 = stats_hs_enet.tx_err_fcs >> 32;
   STATS_PRINTF("tx_err_fcs:           %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.tx_control;
   temp2 = stats_hs_enet.tx_control >> 32;
   STATS_PRINTF("tx_control:           %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.tx_oversize;
   temp2 = stats_hs_enet.tx_oversize >> 32;
   STATS_PRINTF("tx_oversize:          %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.tx_undersize;
   temp2 = stats_hs_enet.tx_undersize >> 32;
   STATS_PRINTF("tx_undersize:         %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.tx_fragments;
   temp2 = stats_hs_enet.tx_fragments >> 32;
   STATS_PRINTF("tx_fragments:         %.8x%.8x\n", temp2,temp1);

   STATS_PRINTF("DPS statistics:\n");
   temp1 = stats_hs_enet.rx_err_overrun;
   temp2 = stats_hs_enet.rx_err_overrun >> 32;
   STATS_PRINTF("rx_err_overrun:       %.8x%.8x\n", temp2,temp1);
   temp1 = stats_hs_enet.rx_err_sdu;
   temp2 = stats_hs_enet.rx_err_sdu >> 32;
   STATS_PRINTF("rx_err_maxsdu:        %.8x%.8x\n", temp2,temp1);
   temp1 = stats_hs_enet.rx_err_nonvalid_mac;
   temp2 = stats_hs_enet.rx_err_nonvalid_mac >> 32;
   STATS_PRINTF("rx_err_mac_address:   %.8x%.8x\n", temp2,temp1);
   temp1 = stats_hs_enet.rx_err_mru;
   temp2 = stats_hs_enet.rx_err_mru >> 32;
   STATS_PRINTF("rx_err_mru:           %.8x%.8x\n", temp2,temp1);
   temp1 = stats_hs_enet.tx_err_underrun;
   temp2 = stats_hs_enet.tx_err_underrun >> 32;
   STATS_PRINTF("tx_err_underrun:      %.8x%.8x\n", temp2,temp1);
   temp1 = stats_hs_enet.rx_err_host_full;
   temp2 = stats_hs_enet.rx_err_host_full >> 32;
   STATS_PRINTF("rx_err_host_full:     %.8x%.8x\n", temp2,temp1);
   temp1 = stats_hs_enet.rx_err_fbp_underrun;
   temp2 = stats_hs_enet.rx_err_fbp_underrun >> 32;
   STATS_PRINTF("rx_err_fbp_underrun:  %.8x%.8x\n", temp2,temp1);
   temp1 = stats_hs_enet.rx_iw_frames;
   temp2 = stats_hs_enet.rx_iw_frames >> 32;
   STATS_PRINTF("rx_iw_frames:         %.8x%.8x\n", temp2,temp1);
   temp1 = stats_hs_enet.tx_frames;
   temp2 = stats_hs_enet.tx_frames >> 32;
   STATS_PRINTF("tx_good_frames:       %.8x%.8x\n", temp2,temp1);
   temp1 = stats_hs_enet.rx_host_frames;
   temp2 = stats_hs_enet.rx_host_frames >> 32;
   STATS_PRINTF("rx_host_frames:       %.8x%.8x\n", temp2,temp1);
   STATS_PRINTF("\n");

   STATS_PRINTF("Serial DPS IF statistics (CFU statistics):\n");
   temp1 = stats_hs_enet.serial_dps_if_stats.tx_frames;
   temp2 = stats_hs_enet.serial_dps_if_stats.tx_frames >> 32;
   STATS_PRINTF("Serial DPS IF tx frames            %.8x%.8x\n", temp2,temp1);
   temp1 = stats_hs_enet.serial_dps_if_stats.tx_bytes;
   temp2 = stats_hs_enet.serial_dps_if_stats.tx_bytes >> 32;
   STATS_PRINTF("Serial DPS IF tx bytes             %.8x%.8x\n", temp2,temp1);
   temp1 = stats_hs_enet.serial_dps_if_stats.rx_frames;
   temp2 = stats_hs_enet.serial_dps_if_stats.rx_frames >> 32;
   STATS_PRINTF("Serial DPS IF rx frames            %.8x%.8x\n", temp2,temp1);
   temp1 = stats_hs_enet.serial_dps_if_stats.rx_bytes;
   temp2 = stats_hs_enet.serial_dps_if_stats.rx_bytes >> 32;
   STATS_PRINTF("Serial DPS IF rx bytes             %.8x%.8x\n", temp2,temp1);
   temp1 = stats_hs_enet.serial_dps_if_stats.rx_pce_denied_frames;
   temp2 = stats_hs_enet.serial_dps_if_stats.rx_pce_denied_frames >> 32;
   STATS_PRINTF("Serial DPS IF rx pce denied frames %.8x%.8x\n", temp2,temp1);
   temp1 = stats_hs_enet.serial_dps_if_stats.rx_dropped_frames;
   temp2 = stats_hs_enet.serial_dps_if_stats.rx_dropped_frames >> 32;
   STATS_PRINTF("Serial DPS IF rx dropped frames    %.8x%.8x\n", temp2,temp1);
   STATS_PRINTF("\n");
}

void WTE_DisplayXgiDeviceStats(WP_handle device_handle, WP_CHAR* type)
{
   WP_U32 temp1, temp2;
   WP_stats_xgi xgi_stats = {0};
   WP_stats_xgi *xgi_statistics;
   WP_status status;
   STATS_PRINTF ("\n%s Device statistics of device %d (%x)\n",
           type,(device_handle & 0x000000ff),device_handle);
   STATS_PRINTF ("============================================\n");

   memset( &xgi_stats, 0, sizeof(WP_stats_xgi) );

   xgi_statistics = &xgi_stats;

   if((status = WP_DeviceStatistics(device_handle, xgi_statistics)))
   {
      STATS_PRINTF("GetStatistics error\n");
      return;
   }
   STATS_PRINTF("\nXGI device statistics:\n");
   STATS_PRINTF("Hardware statistics:\n");

   temp1 = xgi_statistics->rx_packets;
   temp2 = xgi_statistics->rx_packets >> 32;
   STATS_PRINTF("rx_packets:                        %.8x%.8x\n", temp2,temp1);

   temp1 = xgi_statistics->tx_packets;
   temp2 = xgi_statistics->tx_packets >> 32;
   STATS_PRINTF("tx_packets:                        %.8x%.8x\n", temp2,temp1);

   temp1 = xgi_statistics->rx_frames_64;
   temp2 = xgi_statistics->rx_frames_64 >> 32;
   STATS_PRINTF("rx_frames_64:                      %.8x%.8x\n", temp2,temp1);

   temp1 = xgi_statistics->tx_frames_64;
   temp2 = xgi_statistics->tx_frames_64 >> 32;
   STATS_PRINTF("tx_frames_64:                      %.8x%.8x\n", temp2,temp1);

   temp1 = xgi_statistics->rx_frames_65_127;
   temp2 = xgi_statistics->rx_frames_65_127 >> 32;
   STATS_PRINTF("rx_frames_65_127:                  %.8x%.8x\n", temp2,temp1);

   temp1 = xgi_statistics->tx_frames_65_127;
   temp2 = xgi_statistics->tx_frames_65_127 >> 32;
   STATS_PRINTF("tx_frames_65_127:                  %.8x%.8x\n", temp2,temp1);

   temp1 = xgi_statistics->rx_frames_128_255;
   temp2 = xgi_statistics->rx_frames_128_255 >> 32;
   STATS_PRINTF("rx_frames_128_255:                 %.8x%.8x\n", temp2,temp1);

   temp1 = xgi_statistics->tx_frames_128_255;
   temp2 = xgi_statistics->tx_frames_128_255 >> 32;
   STATS_PRINTF("tx_frames_128_255:                 %.8x%.8x\n", temp2,temp1);

   temp1 = xgi_statistics->rx_frames_256_511;
   temp2 = xgi_statistics->rx_frames_256_511 >> 32;
   STATS_PRINTF("rx_frames_256_511:                 %.8x%.8x\n", temp2,temp1);

   temp1 = xgi_statistics->tx_frames_256_511;
   temp2 = xgi_statistics->tx_frames_256_511 >> 32;
   STATS_PRINTF("tx_frames_256_511:                 %.8x%.8x\n", temp2,temp1);

   temp1 = xgi_statistics->rx_frames_512_1023;
   temp2 = xgi_statistics->rx_frames_512_1023 >> 32;
   STATS_PRINTF("rx_frames_512_1023:                %.8x%.8x\n", temp2,temp1);

   temp1 = xgi_statistics->tx_frames_512_1023;
   temp2 = xgi_statistics->tx_frames_512_1023 >> 32;
   STATS_PRINTF("tx_frames_512_1023:                %.8x%.8x\n", temp2,temp1);

   temp1 = xgi_statistics->rx_frames_1024_1518;
   temp2 = xgi_statistics->rx_frames_1024_1518 >> 32;
   STATS_PRINTF("rx_frames_1024_1518:               %.8x%.8x\n", temp2,temp1);

   temp1 = xgi_statistics->tx_frames_1024_1518;
   temp2 = xgi_statistics->tx_frames_1024_1518 >> 32;
   STATS_PRINTF("tx_frames_1024_1518:               %.8x%.8x\n", temp2,temp1);

   temp1 = xgi_statistics->rx_frames_1519_max;
   temp2 = xgi_statistics->rx_frames_1519_max >> 32;
   STATS_PRINTF("rx_frames_1519_max:                %.8x%.8x\n", temp2,temp1);

   temp1 = xgi_statistics->tx_frames_1519_max;
   temp2 = xgi_statistics->tx_frames_1519_max >> 32;
   STATS_PRINTF("tx_frames_1519_max:                %.8x%.8x\n", temp2,temp1);

   temp1 = xgi_statistics->rx_bytes;
   temp2 = xgi_statistics->rx_bytes >> 32;
   STATS_PRINTF("rx_bytes:                          %.8x%.8x\n", temp2,temp1);

   temp1 = xgi_statistics->rx_err_fcs;
   temp2 = xgi_statistics->rx_err_fcs >> 32;
   STATS_PRINTF("rx_err_fcs:                        %.8x%.8x\n", temp2,temp1);

   temp1 = xgi_statistics->rx_multicast;
   temp2 = xgi_statistics->rx_multicast >> 32;
   STATS_PRINTF("rx_multicast:                      %.8x%.8x\n", temp2,temp1);

   temp1 = xgi_statistics->rx_broadcast;
   temp2 = xgi_statistics->rx_broadcast >> 32;
   STATS_PRINTF("rx_broadcast:                      %.8x%.8x\n", temp2,temp1);

   temp1 = xgi_statistics->rx_mac_pause;
   temp2 = xgi_statistics->rx_mac_pause >> 32;
   STATS_PRINTF("rx_mac_pause:                      %.8x%.8x\n", temp2,temp1);

   temp1 = xgi_statistics->rx_err_length;
   temp2 = xgi_statistics->rx_err_length >> 32;
   STATS_PRINTF("rx_err_length:                     %.8x%.8x\n", temp2,temp1);

   temp1 = xgi_statistics->rx_err_code;
   temp2 = xgi_statistics->rx_err_code >> 32;
   STATS_PRINTF("rx_err_code:                       %.8x%.8x\n", temp2,temp1);

   temp1 = xgi_statistics->rx_undersize;
   temp2 = xgi_statistics->rx_undersize >> 32;
   STATS_PRINTF("rx_undersize:                      %.8x%.8x\n", temp2,temp1);

   temp1 = xgi_statistics->rx_oversize;
   temp2 = xgi_statistics->rx_oversize >> 32;
   STATS_PRINTF("rx_oversize:                       %.8x%.8x\n", temp2,temp1);

   temp1 = xgi_statistics->rx_jabber;
   temp2 = xgi_statistics->rx_jabber >> 32;
   STATS_PRINTF("rx_jabber:                         %.8x%.8x\n", temp2,temp1);

   temp1 = xgi_statistics->tx_bytes;
   temp2 = xgi_statistics->tx_bytes >> 32;
   STATS_PRINTF("tx_bytes:                          %.8x%.8x\n", temp2,temp1);

   temp1 = xgi_statistics->tx_multicast;
   temp2 = xgi_statistics->tx_multicast >> 32;
   STATS_PRINTF("tx_multicast:                      %.8x%.8x\n", temp2,temp1);

   temp1 = xgi_statistics->tx_broadcast;
   temp2 = xgi_statistics->tx_broadcast >> 32;
   STATS_PRINTF("tx_broadcast:                      %.8x%.8x\n", temp2,temp1);

   temp1 = xgi_statistics->tx_mac_pause;
   temp2 = xgi_statistics->tx_mac_pause >> 32;
   STATS_PRINTF("tx_mac_pause:                      %.8x%.8x\n", temp2,temp1);

   temp1 = xgi_statistics->tx_err_packets;
   temp2 = xgi_statistics->tx_err_packets >> 32;
   STATS_PRINTF("tx_err_packets:                    %.8x%.8x\n", temp2,temp1);

   STATS_PRINTF("DPS statistics:\n");
   temp1 = xgi_statistics->rx_err_overrun;
   temp2 = xgi_statistics->rx_err_overrun >> 32;
   STATS_PRINTF("rx_err_overrun:                    %.8x%.8x\n", temp2,temp1);
   temp1 = xgi_statistics->rx_err_sdu;
   temp2 = xgi_statistics->rx_err_sdu >> 32;
   STATS_PRINTF("rx_err_maxsdu:                     %.8x%.8x\n", temp2,temp1);
   temp1 = xgi_statistics->rx_err_nonvalid_mac;
   temp2 = xgi_statistics->rx_err_nonvalid_mac >> 32;
   STATS_PRINTF("rx_err_mac_address:                %.8x%.8x\n", temp2,temp1);
   temp1 = xgi_statistics->rx_err_mru;
   temp2 = xgi_statistics->rx_err_mru >> 32;
   STATS_PRINTF("rx_err_mru:                        %.8x%.8x\n", temp2,temp1);
   temp1 = xgi_statistics->tx_err_underrun;
   temp2 = xgi_statistics->tx_err_underrun >> 32;
   STATS_PRINTF("tx_err_underrun:                   %.8x%.8x\n", temp2,temp1);
   temp1 = xgi_statistics->rx_err_host_full;
   temp2 = xgi_statistics->rx_err_host_full >> 32;
   STATS_PRINTF("rx_err_host_full:                  %.8x%.8x\n", temp2,temp1);
   temp1 = xgi_statistics->rx_err_fbp_underrun;
   temp2 = xgi_statistics->rx_err_fbp_underrun >> 32;
   STATS_PRINTF("rx_err_fbp_underrun:               %.8x%.8x\n", temp2,temp1);
   temp1 = xgi_statistics->rx_iw_frames;
   temp2 = xgi_statistics->rx_iw_frames >> 32;
   STATS_PRINTF("rx_iw_frames:                      %.8x%.8x\n", temp2,temp1);
   temp1 = xgi_statistics->tx_frames;
   temp2 = xgi_statistics->tx_frames >> 32;
   STATS_PRINTF("tx_good_frames:                    %.8x%.8x\n", temp2,temp1);
   STATS_PRINTF("\n");
   temp1 = xgi_statistics->rx_host_frames;
   temp2 = xgi_statistics->rx_host_frames >> 32;
   STATS_PRINTF("rx_host_frames:                    %.8x%.8x\n", temp2,temp1);
   STATS_PRINTF("\n");

   STATS_PRINTF("Serial DPS IF statistics (CFU statistics) :\n");
   temp1 = xgi_statistics->serial_dps_if_stats.tx_frames;
   temp2 = xgi_statistics->serial_dps_if_stats.tx_frames >> 32;
   STATS_PRINTF("Serial DPS IF tx frames            %.8x%.8x\n", temp2,temp1);
   temp1 = xgi_statistics->serial_dps_if_stats.tx_bytes;
   temp2 = xgi_statistics->serial_dps_if_stats.tx_bytes >> 32;
   STATS_PRINTF("Serial DPS IF tx bytes             %.8x%.8x\n", temp2,temp1);
   temp1 = xgi_statistics->serial_dps_if_stats.rx_frames;
   temp2 = xgi_statistics->serial_dps_if_stats.rx_frames >> 32;
   STATS_PRINTF("Serial DPS IF rx frames            %.8x%.8x\n", temp2,temp1);
   temp1 = xgi_statistics->serial_dps_if_stats.rx_bytes;
   temp2 = xgi_statistics->serial_dps_if_stats.rx_bytes >> 32;
   STATS_PRINTF("Serial DPS IF rx bytes             %.8x%.8x\n", temp2,temp1);
   temp1 = xgi_statistics->serial_dps_if_stats.rx_pce_denied_frames;
   temp2 = xgi_statistics->serial_dps_if_stats.rx_pce_denied_frames >> 32;
   STATS_PRINTF("Serial DPS IF rx pce denied frames %.8x%.8x\n", temp2,temp1);
   temp1 = xgi_statistics->serial_dps_if_stats.rx_dropped_frames;
   temp2 = xgi_statistics->serial_dps_if_stats.rx_dropped_frames >> 32;
   STATS_PRINTF("Serial DPS IF rx dropped frames    %.8x%.8x\n", temp2,temp1);
}

WP_U8 WPE_DataUnitReceive(WP_handle rx_channel_handle, WP_U8 *hexa_ret)
{
   WP_status status;
   WP_data_segment tx_buf;
   WP_data_unit data_unit;
   WP_U32 ii, j, non_sim_delay = 10, num_of_polling_rx_data = 10;
   WP_U8 *data_ptr=NULL;
   WP_U8 result = TRUE;

   WP_data_segment *curr_buff_ptr=NULL;
   WP_U32 rx_counter=0;
   WP_CHAR hexa_buffer[DATA_LENGTH*2];
   WP_U32 k;

   memset(&data_unit, 0, sizeof( data_unit));
   memset(&tx_buf, 0, sizeof( tx_buf));

   data_unit.data_size = DATA_LENGTH;
   data_unit.n_active = 1;

   tx_buf.data_size = data_unit.data_size;
   /* data_ptr = WP_PoolAlloc(data_pool_handle); */
   tx_buf.data = data_ptr;
   /* tx_buf.pool_handle = data_pool_handle; */
   tx_buf.next = NULL;
   tx_buf.displacement = 0;

   /* now moving to receive */
   data_unit.segment = (&tx_buf);
   data_unit.n_segments = 1;
   data_unit.n_active = 0;

   switch(WPI_HANDLE_SUBTYPE(rx_channel_handle))
   {
   case WP_IW_HOST:
      data_unit.type = WP_DATA_IW;
      /* data_unit.type = WP_DATA_IW | WP_DATA_RETURN_ENABLE; */
      break;
   case WP_ENET:
      data_unit.type = WP_DATA_ENET;
      break;
   }

   tx_buf.data = NULL;

   for (ii = 0; ii < num_of_polling_rx_data; ++ii)
   {
      status = WP_HostReceive (rx_channel_handle, &data_unit);

      if (WP_ERROR(status) == WP_ERR_HST_NO_DATA_TO_GET) /* didn't get yet? */
      {
#ifdef WINSIM
            WP_Delay(80000);
#endif
#ifdef VERILOG
            WP_Delay(4000000);
#endif
         WP_Delay(non_sim_delay);
         if(ii > 3)
            non_sim_delay = 1500;
      }
      else if(WP_OK != status)
      {
         result = FALSE;
         WPE_TerminateOnError(status,"WP_HostReceive()", __LINE__);
         break;
      }
      else
      {
         result = TRUE;
         curr_buff_ptr = data_unit.segment;
         rx_counter += data_unit.n_active;

         /* print segment */
         MY_PRINTF("recieved %d segments, %d long each\n", data_unit.n_active, curr_buff_ptr->data_size);
         WP_MEM_BYTES_GET(((WP_U8 *)hexa_buffer),curr_buff_ptr->data,curr_buff_ptr->data_size);
         MY_PRINTF("after mem set\n");
         for (j = 0; j < data_unit.n_active; ++j)
         {
            for(k=0;k<curr_buff_ptr->data_size;++k)
            {
               hexa_ret[k] = (WP_U8)(hexa_buffer[k]);
               MY_PRINTF("%2.2x", (WP_U8)hexa_buffer[k]);
               if ((k % 12) == 11)
                  MY_PRINTF("\n");
            }
            MY_PRINTF("\n\n");

            WP_PoolFree(curr_buff_ptr->pool_handle, curr_buff_ptr->data);
            MY_PRINTF("next = %d \n", (WP_U32)curr_buff_ptr->next);
            curr_buff_ptr = curr_buff_ptr->next;
         }
         if(rx_counter == 1)
            break;
         else
         {
#ifdef WINSIM
            /* a big delay for winsim */
            WP_Delay(1000000);
#endif

#ifdef VERILOG
            WP_Delay(10000000);
#endif

            WP_Delay(100000);
         }
      }
   }

   if(ii >= num_of_polling_rx_data)
   {
      result = FALSE;
   }
   else
   {
      MY_PRINTF("Received packet:\n");
   }
   return result;
}


/****************************************************/
/* This function is used to receive in GCSI tests   */
/****************************************************/
WP_U8 WPE_DataUnitReceiveGCSI(WP_handle rx_channel_handle, WP_U8 *hexa_ret, WP_U16* PacketTxSize)
{
   WP_status status;
   WP_data_segment tx_buf;
   WP_data_unit data_unit;
   WP_U32 ii, j;
   WP_U8 *data_ptr=NULL;
   WP_U8 result = TRUE;

   WP_data_segment *curr_buff_ptr=NULL;
   WP_U32 rx_counter=0;
   WP_CHAR hexa_buffer[DATA_LENGTH*2];
   WP_U32 k;

   memset(&data_unit, 0, sizeof( data_unit));
   memset(&tx_buf, 0, sizeof( tx_buf));

   data_unit.data_size = *PacketTxSize; //liorbaz DATA_LENGTH;
   data_unit.n_active = 1;

   tx_buf.data_size = data_unit.data_size;
   /* data_ptr = WP_PoolAlloc(data_pool_handle); */
   tx_buf.data = data_ptr;
   /* tx_buf.pool_handle = data_pool_handle; */
   tx_buf.next = NULL;
   tx_buf.displacement = 0;

   /* now moving to receive */
   data_unit.segment = (&tx_buf);
   data_unit.n_segments = 1;
   data_unit.n_active = 0;

   switch(WPI_HANDLE_SUBTYPE(rx_channel_handle))
   {
   case WP_IW_HOST:
      data_unit.type = WP_DATA_IW;
      /* data_unit.type = WP_DATA_IW | WP_DATA_RETURN_ENABLE; */
      break;
   case WP_ENET:
      data_unit.type = WP_DATA_ENET;
      break;
   }

   tx_buf.data = NULL;

//   printf("File:%s:%d WP_HostReceive: data_unit.type:0x%x\n", __FILE__,__LINE__, data_unit.type );//liorbaz
//   printf("File:%s:%d WP_HostReceive: data_unit.size:%d\n", __FILE__,__LINE__, data_unit.data_size );//liorbaz
   for (ii = 0; ii < 1; ++ii)
   {
      status = WP_HostReceive (rx_channel_handle, &data_unit);
//      printf("File:%s:%d After WP_HostReceive[%d]:\n", __FILE__,__LINE__, ii );//liorbaz

      if (WP_ERROR(status) == WP_ERR_HST_NO_DATA_TO_GET) /* didn't get yet? */
      {
         printf("Delay0[%d]:\n", ii );//liorbaz
//         WP_Delay(2000000);
         WP_Delay(200000);
      }
      else if(WP_OK != status)
      {
         result = FALSE;
         WPE_TerminateOnError(status,"WP_HostReceive()", __LINE__);
         break;
      }
      else
      {
         result = TRUE;
         curr_buff_ptr = data_unit.segment;
         rx_counter += data_unit.n_active;

//#ifdef DEBUG_PRINT
         /* print segment */
         MY_PRINTF("received %d segments, %d long each\n", data_unit.n_active, curr_buff_ptr->data_size);
//#endif

         *PacketTxSize = curr_buff_ptr->data_size; // Fix the received data length
         WP_MEM_BYTES_GET(((WP_U8 *)hexa_buffer),curr_buff_ptr->data,curr_buff_ptr->data_size);

#ifdef DEBUG_PRINT
         printf("after mem set\n");
#endif
         for (j = 0; j < data_unit.n_active; ++j)
         {
            for(k=0;k<curr_buff_ptr->data_size;++k)
            {
               hexa_ret[k] = (WP_U8)(hexa_buffer[k]);
#ifdef DEBUG_PRINT
               printf("%2.2x", (WP_U8)hexa_buffer[k]);
               if ((k % 12) == 11)
                  printf("\n");
#endif
            }
#ifdef DEBUG_PRINT
            printf("\n\n");
#endif

            //   WP_PoolFree(curr_buff_ptr->pool_handle, curr_buff_ptr->data);
//#ifdef DEBUG_PRINT
            MY_PRINTF("next = %d \n", (WP_U32)curr_buff_ptr->next);
//#endif
            curr_buff_ptr = curr_buff_ptr->next;
         }
         if(rx_counter == 1)
            break;
         else
         {
#ifdef WINSIM
            /* a big delay for winsim */
            WP_Delay(1000000);
            printf("Delay1[%d]:\n", ii );//liorbaz
#endif

#ifdef VERILOG
            WP_Delay(10000000);
            printf("Delay2[%d]:\n", ii );//liorbaz
#endif

            WP_Delay(100000);
            printf("Delay3[%d]:\n", ii );//liorbaz
         }
      }
   }

   if(ii == 20000)
   {
      result = FALSE;
   }
#ifdef DEBUG_PRINT
   else
   {
      printf("Received packet result = %s:\n",
             (result != FALSE ? "OK" : "NOT OK !"));
   }
#endif
   return result;
}

WP_U8 dont_check_packet_mac_da=0;
WP_U8 WPE_DataUnitReceiveOnly(const WP_CHAR* mac_dst, const WP_CHAR* mac_src, const WP_CHAR* svlan, const WP_CHAR* cvlan,
                                     const WP_CHAR* ip_dst, const WP_CHAR* ip_src, WP_handle rx_channel_handle)
{
   WP_U8 hexa_ret[2000];
   WP_U32 packet_counter=0;
   WP_U8 result;

   packet_counter++;
   MY_PRINTF("\nRECV: Packet no %i. \n MAC DA %s, MAC SA %s, VLAN TAG %s, IP SRC %s, IP DST %s",
      packet_counter, mac_dst, mac_src, svlan, ip_src, ip_dst);

   if(cvlan[0] != 0)
      MY_PRINTF(" CVLAN TAG %s", cvlan);

   MY_PRINTF("\n Receiving to channel %X ...\n", rx_channel_handle);

   result = WPE_DataUnitReceive(rx_channel_handle, &(hexa_ret[0]));
   MY_PRINTF( "RECV: Packet no %i. Receive Result = 0x%X\n\n", packet_counter, result);

   if(result == TRUE)
   {
      if(dont_check_packet_mac_da == 0)
      {
         WP_CHAR ch_byte[3];
         WP_U32 i, byte;
         MY_PRINTF("Checking the packet:\n   expected dst mac: 0x%s \n            dst mac: 0x%02X%02X%02X%02X%02X%02X \n",
            mac_dst, hexa_ret[0], hexa_ret[1], hexa_ret[2], hexa_ret[3], hexa_ret[4], hexa_ret[5]);
         for(i = 0; i<6; i++)
         {
            ch_byte[0] = mac_dst[i*2];
            ch_byte[1] = mac_dst[i*2+1];
            ch_byte[2] = 0;
            sscanf(ch_byte, "%02X", &byte);
            if (hexa_ret[i] != byte)
            {
               printf("The packet is wrong\n");
               return FALSE;
            }
         }
      }
      MY_PRINTF("RECV: Packet no %d is received.\n", packet_counter);
   }

   return result;
}

WP_U16 make_parser_deny = 0;
WP_U16 make_packet_short = 0;

void WPE_DataUnitSendToEnet(const WP_CHAR* mac_dst, const WP_CHAR* mac_src,
                                         const WP_CHAR* svlan, const WP_CHAR* cvlan,
                                         const WP_CHAR* ip_dst, const WP_CHAR* ip_src,
                                         WP_handle tx_channel_handle)
{
   WP_status status;
   WP_U8 *km_data = NULL;
   WP_CHAR ascii_buffer[DATA_LENGTH*2+1];
   WP_CHAR hex_buffer[DATA_LENGTH*2];
   WP_data_segment data_segment;
   WP_data_unit data_unit;
   WP_U16 checksum, i;
   WP_U32 ip_start;

   memset (hex_buffer, 0, DATA_LENGTH*2);
   memset (ascii_buffer, 0, DATA_LENGTH*2+1);
   memset( &data_unit, 0, sizeof( data_unit));
   memset( &data_segment, 0, sizeof( data_segment));
   km_data = WP_PoolAlloc(iw_pool);
   if(km_data == NULL)
   {
      printf("ERROR: No data To Allocate from IW Pool 0x%08X!!!\n", iw_pool);
      WT_Reboot();
      exit(1);
   }

   data_unit.data_size = 60;/*DATA_LENGTH;*/
   data_unit.n_active = 1;

   data_segment.data_size = data_unit.data_size;
   data_segment.data = km_data;
   data_segment.pool_handle = iw_pool;
   data_segment.next = NULL;
   data_segment.displacement = 0;

   data_unit.n_segments = 1;
   switch(WPI_HANDLE_SUBTYPE(tx_channel_handle))
   {
   case WP_IW_HOST:
      data_unit.type = WP_DATA_IW;
      /* data_unit.type = WP_DATA_IW | WP_DATA_RETURN_ENABLE; */
      break;
   case WP_ENET:
      data_unit.type = WP_DATA_ENET;
      break;
   case WP_HDLC:
      data_unit.type = WP_DATA_PPP_HDLC;
      MY_PRINTF("data_unit.type = WP_DATA_PPP_HDLC\n");
      break;
   default:
      printf("ERROR: unsupported tx channel subtype for this test !!!\n");
      WT_Reboot();
      exit(1);
   }

   data_unit.control = 0;

   data_unit.segment = (&data_segment);

   if(data_unit.type == WP_DATA_PPP_HDLC)
   {
      strcpy (ascii_buffer, "FF030021");/* HDLC PPP -> IPv4 */
      ip_start = 4;
   }
   else
   {
      ip_start = 14;
      strcpy (ascii_buffer, mac_dst);/* MAC dst address */
      strcat (ascii_buffer, mac_src);/* MAC src address */
      if(svlan[0] != 0)
      {
         ip_start += 4;
         strcat (ascii_buffer, "8100");/* ETHER_TYPE */
         strcat (ascii_buffer, svlan); /* SVLAN TAG */
      }
      if(cvlan[0] != 0)
      {
         ip_start += 4;
         strcat (ascii_buffer, "8100");/* ETHER_TYPE */
         strcat (ascii_buffer, cvlan); /* CVLAN TAG */
      }
      if(make_parser_deny != 0)
      {
         WP_CHAR tmp_str[6];
         sprintf(tmp_str, "%04X", make_parser_deny);
         strcat (ascii_buffer, tmp_str);/* Parser will deny the packet */
      }
      strcat (ascii_buffer, "0800");/* IP TYPE */
   }

   strcat (ascii_buffer, "45040044");/* tos = 4 */
   strcat (ascii_buffer, "001b0000");
   strcat (ascii_buffer, "0a110000");/* protocol = 0x11, empty checksum */
   strcat (ascii_buffer, ip_src);/* IP src address */
   strcat (ascii_buffer, ip_dst);/* IP src address */
   strcat (ascii_buffer, "00190015");
   strcat (ascii_buffer, "00280000");/* length, UDP with no checksum */

   for(i=ip_start; i<32; i++)
   {
      strcat (ascii_buffer, "12");
   }
   if(make_packet_short != 0)
   {
      data_unit.data_size = 50;/*DATA_LENGTH;*/
      data_segment.data_size = data_unit.data_size;
   }

   MY_PRINTF("transmitted data length: %d\n", data_unit.data_size);
   App_TranslateAsciiToHex(hex_buffer, ascii_buffer, data_unit.data_size);

   checksum = WTE_IpChecksumCalc((WP_U8*)(hex_buffer + ip_start), 20);

   memcpy (hex_buffer + ip_start + 10, &checksum, 2);

   MY_PRINTF("km_data = 0x%08X iw_buffer_data->size = %d\n", (WP_U32) km_data, iw_buffer_data->size);
   WP_MEM_BYTES_FILL(km_data, '0', iw_buffer_data->size);
   MY_PRINTF("km_data = 0x%08X hex_buffer = 0x%08X iw_buffer_data->size = %d\n", (WP_U32) km_data, (WP_U32) hex_buffer, iw_buffer_data->size);
   WP_MEM_BYTES_SET(km_data, hex_buffer, data_unit.data_size);
   MY_PRINTF("3\n");

   MY_PRINTF_DATA_UNIT(data_unit);

   status = WP_HostSend(tx_channel_handle, &data_unit);
   WPE_TerminateOnError(status,"WP_HostSend", __LINE__);

   return;
}

WP_U8 packet_type;

typedef struct
{
   WP_U32 ethernet_used;
}
WT_common_packet;

void WPE_CommonDataUnitSend(const WP_CHAR* mac_dst, const WP_CHAR* mac_src, const WP_CHAR* svlan, const WP_CHAR* cvlan,
                                   const WP_CHAR* ip_dst, const WP_CHAR* ip_src, WP_handle tx_channel_handle, WP_U8 packet_type)
{
   WP_status status;
   WP_U8 *km_data = NULL;
   WP_CHAR ascii_buffer[DATA_LENGTH*2+1];
   WP_CHAR hex_buffer[DATA_LENGTH*2];
   WP_data_segment data_segment;
   WP_data_unit data_unit;
   WP_U16 checksum, i, payload_to_fill, ipv4_start, l3_start, ipv4=0;

   memset (hex_buffer, 0, DATA_LENGTH*2);
   memset (ascii_buffer, 0, DATA_LENGTH*2+1);
   memset( &data_unit, 0, sizeof( data_unit));
   memset( &data_segment, 0, sizeof( data_segment));
   km_data = WP_PoolAlloc(iw_pool);
   if(km_data == NULL)
   {
      printf("ERROR: No data To Allocate from IW Pool 0x%08X!!!\n", iw_pool);
      WT_Reboot();
      exit(1);
   }

   data_unit.data_size = 60;/*DATA_LENGTH;*/
   data_unit.n_active = 1;

   data_segment.data_size = data_unit.data_size;
   data_segment.data = km_data;
   data_segment.pool_handle = iw_pool;
   data_segment.next = NULL;
   data_segment.displacement = 0;

   data_unit.n_segments = 1;
   switch(WPI_HANDLE_SUBTYPE(tx_channel_handle))
   {
   case WP_IW_HOST:
      data_unit.type = WP_DATA_IW;
      /* data_unit.type = WP_DATA_IW | WP_DATA_RETURN_ENABLE; */
      break;
   case WP_ENET:
      data_unit.type = WP_DATA_ENET;
      break;
   case WP_HDLC:
      data_unit.type = WP_DATA_PPP_HDLC;
      MY_PRINTF("data_unit.type = WP_DATA_PPP_HDLC\n");
      break;
   default:
      printf("ERROR: unsupported tx channel subtype for this test !!!\n");
      WT_Reboot();
      exit(1);
   }

   data_unit.control = 0;

   data_unit.segment = (&data_segment);

   /* LAYER 2 Ethernet or HSPOS 4 headers possible */
   switch(packet_type)
   {
   case WT_PACKET_TYPE_UPI_HSPOS1_IP_UDP:
      strcpy (ascii_buffer, "21");/* HDLC PPP -> IPv4 */
      l3_start = 1;
      break;
   case WT_PACKET_TYPE_UPI_HSPOS2_IP_UDP:
      strcpy (ascii_buffer, "0021");/* HDLC PPP -> IPv4 */
      l3_start = 2;
      break;
   case WT_PACKET_TYPE_UPI_HSPOS3_IP_UDP:
      strcpy (ascii_buffer, "FF0321");/* HDLC PPP -> IPv4 */
      l3_start = 3;
      break;
   case WT_PACKET_TYPE_UPI_HSPOS4_IP_UDP:
      strcpy (ascii_buffer, "FF030021");/* HDLC PPP -> IPv4 */
      l3_start = 4;
      break;
   case WT_PACKET_TYPE_ETHERNET_IP_UDP:
   case WT_PACKET_TYPE_ETHERNET_IPV6_UDP:
   case WT_PACKET_TYPE_ETHERNET_SC_IP_UDP:
   case WT_PACKET_TYPE_ETHERNET_SS_IP_UDP:
   case WT_PACKET_TYPE_ETHERNET_IP_TCP:
   case WT_PACKET_TYPE_ETHERNET_IP_TCP_ACK:
   case WT_PACKET_TYPE_ETHERNET_IP_TCP_SYN:
   case WT_PACKET_TYPE_ETHERNET_IP_TCP_ACK_SYN:
   case WT_PACKET_TYPE_ETHERNET_IP_MPLS:
   case WT_PACKET_TYPE_ETHERNET_IP_GRE:
   case WT_PACKET_TYPE_ETHERNET_IP_GRE_MPLS:
   case WT_PACKET_TYPE_ETHERNET_ARP:
   case WT_PACKET_TYPE_ETHERNET_RARP:
   case WT_PACKET_TYPE_ETHERNET_PPPoE_D_IP_UDP:
   case WT_PACKET_TYPE_ETHERNET_PPPoE_S_IP_UDP:
      l3_start = 14;
      strcpy (ascii_buffer, mac_dst);/* MAC dst address */
      strcat (ascii_buffer, mac_src);/* MAC src address */
      if(svlan[0] != 0)
      {
         l3_start += 4;
         if(packet_type == WT_PACKET_TYPE_ETHERNET_SC_IP_UDP || packet_type == WT_PACKET_TYPE_ETHERNET_SS_IP_UDP)
            strcat (ascii_buffer, "9100");/* ETHER_TYPE */
         else
            strcat (ascii_buffer, "8100");/* ETHER_TYPE */
         strcat (ascii_buffer, svlan); /* SVLAN TAG */
      }
      if(cvlan[0] != 0)
      {
         l3_start += 4;
         if(packet_type == WT_PACKET_TYPE_ETHERNET_SS_IP_UDP)
            strcat (ascii_buffer, "9100");/* ETHER_TYPE */
         else
            strcat (ascii_buffer, "8100");/* ETHER_TYPE */
         strcat (ascii_buffer, cvlan); /* CVLAN TAG */
      }
      if(make_parser_deny != 0)
      {
         WP_CHAR tmp_str[6];
         sprintf(tmp_str, "%04X", make_parser_deny);
         strcat (ascii_buffer, tmp_str);/* Parser will deny the packet */
      }
      if(packet_type == WT_PACKET_TYPE_ETHERNET_ARP)
         strcat (ascii_buffer, "0806");/* ARP type of ethernet */
      else if(packet_type == WT_PACKET_TYPE_ETHERNET_RARP)
         strcat (ascii_buffer, "8035");/* RARP type of ethernet */
      else if(packet_type == WT_PACKET_TYPE_ETHERNET_PPPoE_D_IP_UDP)
         strcat (ascii_buffer, "8863");/* PPPeE (discovery stage) type of ethernet */
      else if(packet_type == WT_PACKET_TYPE_ETHERNET_PPPoE_S_IP_UDP)
         strcat (ascii_buffer, "8864");/* PPPeE (session stage) type of ethernet */
      else if(packet_type == WT_PACKET_TYPE_ETHERNET_IPV6_UDP)
         strcat (ascii_buffer, "86dd");/* IPv6 type of ethernet */
      else
         strcat (ascii_buffer, "0800");/* IP type of ethernet */
      break;
   default:
      printf("\nERROR: UNDEFINED PACKET TYPE!\n");
      WT_Reboot();
      exit(1);
      break;
   }

   payload_to_fill = 60;
   /* LAYER 3 */
   if(packet_type == WT_PACKET_TYPE_ETHERNET_ARP || packet_type == WT_PACKET_TYPE_ETHERNET_RARP)
   {
      strcat (ascii_buffer, "0001");   /* ARP/RARP - Hardware type - Ethernet */
      strcat (ascii_buffer, "0800");   /* ARP/RARP - Protocol type - IP */
      strcat (ascii_buffer, "00");     /* ARP/RARP - Hardware address length */
      strcat (ascii_buffer, "00");     /* ARP/RARP - Protocol address length */
      strcat (ascii_buffer, "0003");   /* ARP/RARP - Opcode */
      payload_to_fill -= 8;
   }
   else if(packet_type == WT_PACKET_TYPE_ETHERNET_IPV6_UDP)
   {
      strcat (ascii_buffer, "6");        /* Version = Ipv6 */
      strcat (ascii_buffer, "cd");       /* Traffic Class - 8 bit */
      strcat (ascii_buffer, "97531");    /* Flow label */
      strcat (ascii_buffer, "0000");     /* Payload Length */
      strcat (ascii_buffer, "11");       /* Next header (ip_protocol) - UDP */
      strcat (ascii_buffer, "00");       /* Hop limit */
      payload_to_fill -= 8;

      /* SRC & DST address */
      strcat (ascii_buffer, "0123456789ABCDEF0123456789ABCDEF"); /* IPv6 SRC address */
      strcat (ascii_buffer, "FEDCBA9876543210FEDCBA9876543210"); /* IPv6 DST address */
      payload_to_fill -= 32;

      /* UDP header */
      strcat (ascii_buffer, "00190015");
      strcat (ascii_buffer, "00280000");/* length, UDP with no checksum */
      payload_to_fill -= 8;
   }
   else
   {/* IP - IPv4 or IPv6 */
      ipv4_start = l3_start;
      if(packet_type == WT_PACKET_TYPE_ETHERNET_PPPoE_D_IP_UDP || packet_type == WT_PACKET_TYPE_ETHERNET_PPPoE_S_IP_UDP)
      {
         strcat (ascii_buffer, "1100");/* PPPoE: version 1, type 1, Code 00 */
         strcat (ascii_buffer, "5678");/* PPPoE: session ID */
         strcat (ascii_buffer, "0002");/* PPPoE: length 0 */
         strcat (ascii_buffer, "0021");/* PPPoE: version ID - IPv4 */
         payload_to_fill -= 8;
         ipv4_start += 8;
      }
      ipv4 = 1;
      strcat (ascii_buffer, "4");         /* Version = IPv4 */
      strcat (ascii_buffer, "5");         /* IHL = 5 */
      strcat (ascii_buffer, "04");        /* TOS = 04 */
      strcat (ascii_buffer, "0044");      /* Total length */
      strcat (ascii_buffer, "001b");      /* Identifications */
      strcat (ascii_buffer, "0");         /* Flags */
      strcat (ascii_buffer, "000");       /* Fragment offset */
      strcat (ascii_buffer, "0a");        /* TTL */

      switch(packet_type)
      {/* set the protocol in IPv4 header */
      case WT_PACKET_TYPE_UPI_HSPOS1_IP_UDP:
      case WT_PACKET_TYPE_UPI_HSPOS2_IP_UDP:
      case WT_PACKET_TYPE_UPI_HSPOS3_IP_UDP:
      case WT_PACKET_TYPE_UPI_HSPOS4_IP_UDP:
      case WT_PACKET_TYPE_ETHERNET_IP_UDP:
      case WT_PACKET_TYPE_ETHERNET_IPV6_UDP:
      case WT_PACKET_TYPE_ETHERNET_SC_IP_UDP:
      case WT_PACKET_TYPE_ETHERNET_SS_IP_UDP:
      case WT_PACKET_TYPE_ETHERNET_PPPoE_D_IP_UDP:
      case WT_PACKET_TYPE_ETHERNET_PPPoE_S_IP_UDP:
         strcat (ascii_buffer, "11");
         break;
      case WT_PACKET_TYPE_ETHERNET_IP_TCP:
      case WT_PACKET_TYPE_ETHERNET_IP_TCP_ACK:
      case WT_PACKET_TYPE_ETHERNET_IP_TCP_SYN:
      case WT_PACKET_TYPE_ETHERNET_IP_TCP_ACK_SYN:
         strcat (ascii_buffer, "06");
         break;
      case WT_PACKET_TYPE_ETHERNET_IP_GRE_MPLS:
      case WT_PACKET_TYPE_ETHERNET_IP_GRE:
         strcat (ascii_buffer, "2f");
         break;
      case WT_PACKET_TYPE_ETHERNET_IP_MPLS:
         strcat (ascii_buffer, "89");
         break;
      default:
         break;
      }

      strcat (ascii_buffer, "0000");/* empty checksum */
      strcat (ascii_buffer, ip_src);/* IP src address */
      strcat (ascii_buffer, ip_dst);/* IP dst address */

      payload_to_fill -= 20;

      /* LAYER 4 in IPv4 */
      switch(packet_type)
      {/* set the protocol in IPv4 header */
      case WT_PACKET_TYPE_UPI_HSPOS1_IP_UDP:
      case WT_PACKET_TYPE_UPI_HSPOS2_IP_UDP:
      case WT_PACKET_TYPE_UPI_HSPOS3_IP_UDP:
      case WT_PACKET_TYPE_UPI_HSPOS4_IP_UDP:
      case WT_PACKET_TYPE_ETHERNET_IP_UDP:
      case WT_PACKET_TYPE_ETHERNET_SC_IP_UDP:
      case WT_PACKET_TYPE_ETHERNET_SS_IP_UDP:
      case WT_PACKET_TYPE_ETHERNET_PPPoE_D_IP_UDP:
      case WT_PACKET_TYPE_ETHERNET_PPPoE_S_IP_UDP:
         strcat (ascii_buffer, "00190015");
         strcat (ascii_buffer, "00280000");/* length, UDP with no checksum */
         payload_to_fill -= 8;
         break;
      case WT_PACKET_TYPE_ETHERNET_IP_TCP:
      case WT_PACKET_TYPE_ETHERNET_IP_TCP_ACK:
      case WT_PACKET_TYPE_ETHERNET_IP_TCP_SYN:
      case WT_PACKET_TYPE_ETHERNET_IP_TCP_ACK_SYN:
         strcat (ascii_buffer, "00190015");
         strcat (ascii_buffer, "00000123");/* TCP sequence number */
         strcat (ascii_buffer, "00000120");/* TCP acknowledgment number */
         strcat (ascii_buffer, "00");
         if(packet_type == WT_PACKET_TYPE_ETHERNET_IP_TCP)
            strcat (ascii_buffer, "00");
         else if(packet_type == WT_PACKET_TYPE_ETHERNET_IP_TCP_ACK_SYN)
            strcat (ascii_buffer, "12");
         else if(packet_type == WT_PACKET_TYPE_ETHERNET_IP_TCP_ACK)
            strcat (ascii_buffer, "10");
         else if(packet_type == WT_PACKET_TYPE_ETHERNET_IP_TCP_SYN)
            strcat (ascii_buffer, "02");
         strcat (ascii_buffer, "0000"); /* TCP window - not used */
         strcat (ascii_buffer, "00000000"); /* TCP checksum and urgent pointer - not used */
         payload_to_fill -= 20;
         break;
      case WT_PACKET_TYPE_ETHERNET_IP_GRE:
         strcat (ascii_buffer, "20001234");/* key only, GRE version 0, protocol 1234 */
         //strcat (ascii_buffer, "30001234");/* Key & S, GRE version 0, protocol 1234 */
         strcat (ascii_buffer, "ABCDEF12");/* key */
         payload_to_fill -= 8;
         break;
      case WT_PACKET_TYPE_ETHERNET_IP_GRE_MPLS:
         strcat (ascii_buffer, "20008847");/* key only, GRE version 0, protocol 8847 - MPLS UC */
         //strcat (ascii_buffer, "30008847");/* Key & S, GRE version 0, protocol 8847 - MPLS UC */
         strcat (ascii_buffer, "ABCDEF12");/* key */
         payload_to_fill -= 8;
      case WT_PACKET_TYPE_ETHERNET_IP_MPLS:
         strcat (ascii_buffer, "12345155");/* label: 12345 ;;; last 1; TTL: 55 */
         payload_to_fill -= 4;
         break;
      default:
         break;
      }
   }

   for(i=l3_start; i<payload_to_fill; i++)
   {
      strcat (ascii_buffer, "AD");
   }

   if(make_packet_short != 0)
   {
      data_unit.data_size = 50; /*DATA_LENGTH;*/
      data_segment.data_size = data_unit.data_size;
   }

   MY_PRINTF("transmitted data length: %d\n", data_unit.data_size);
   App_TranslateAsciiToHex(hex_buffer, ascii_buffer, data_unit.data_size);

   if(ipv4 == 1)
   {
      /* update the checksim in IPv4 header */
      checksum = WTE_IpChecksumCalc((WP_U8*)(hex_buffer + ipv4_start), 20);
      memcpy (hex_buffer + ipv4_start + 10, &checksum, 2);
   }

   MY_PRINTF("km_data = 0x%08X iw_buffer_data->size = %d\n", (WP_U32) km_data, iw_buffer_data->size);
   WP_MEM_BYTES_FILL(km_data, '0', iw_buffer_data->size);
   MY_PRINTF("km_data = 0x%08X hex_buffer = 0x%08X iw_buffer_data->size = %d\n", (WP_U32) km_data, (WP_U32) hex_buffer, iw_buffer_data->size);
   WP_MEM_BYTES_SET(km_data, hex_buffer, data_unit.data_size);
   MY_PRINTF("3\n");

   MY_PRINTF_DATA_UNIT(data_unit);

   status = WP_HostSend(tx_channel_handle, &data_unit);
   WPE_TerminateOnError(status,"WP_HostSend", __LINE__);

   return;
}

WP_U32 packet_counter = 0;
void WPE_CommonDataUnitSendOnly(const WP_CHAR* mac_dst, const WP_CHAR* mac_src, const WP_CHAR* svlan, const WP_CHAR* cvlan,
                                       const WP_CHAR* ip_dst, const WP_CHAR* ip_src, WP_handle tx_channel_handle, WP_U8 packet_type)
{
   packet_counter++;
   MY_PRINTF("\nSEND: Packet no %i. \n IP SRC %s, IP DST %s",
      packet_counter, ip_src, ip_dst);

   MY_PRINTF("\n Sending to Channel %X ...\n", tx_channel_handle);

   WPE_CommonDataUnitSend(mac_dst, mac_src, svlan, cvlan, ip_dst, ip_src, tx_channel_handle, packet_type);
   MY_PRINTF("SEND: Packet no %d is sent.\n", packet_counter);
}

void WPE_DataUnitSendOnly(const WP_CHAR* mac_dst, const WP_CHAR* mac_src, const WP_CHAR* svlan, const WP_CHAR* cvlan,
                                 const WP_CHAR* ip_dst, const WP_CHAR* ip_src, WP_handle tx_channel_handle)
{
   packet_counter++;
   MY_PRINTF("\nSEND: Packet no %i. \n MAC DA %s, MAC SA %s, SVLAN TAG %s, IP SRC %s, IP DST %s",
      packet_counter, mac_dst, mac_src, svlan, ip_src, ip_dst);

   if(cvlan[0] != 0)
      MY_PRINTF(" CVLAN TAG %s", cvlan);

   MY_PRINTF("\n Sending to Channel %X ...\n", tx_channel_handle);

   WPE_DataUnitSendToEnet(mac_dst, mac_src, svlan, cvlan, ip_dst, ip_src, tx_channel_handle);
   MY_PRINTF("SEND: Packet no %d is sent.\n", packet_counter);
}

WP_U8 WPE_DataUnitSendReceive(const WP_CHAR* mac_dst, const WP_CHAR* mac_src, const WP_CHAR* svlan, const WP_CHAR* cvlan,
                                     const WP_CHAR* ip_dst, const WP_CHAR* ip_src,
                                     WP_handle tx_channel_handle, WP_handle rx_channel_handle)
{
   WP_U8 hexa_ret[2000];
   WP_U32 packet_counter=0;
   WP_U8 result;
   packet_counter++;
   MY_PRINTF("\nSEND: Packet no %i. \n MAC DA %s, MAC SA %s, SVLAN TAG %s, IP SRC %s, IP DST %s",
      packet_counter, mac_dst, mac_src, svlan, ip_src, ip_dst);

   if(cvlan[0] != 0)
      MY_PRINTF(" CVLAN TAG %s", cvlan);

   MY_PRINTF("\n Sending to Enet %X ...\n", tx_channel_handle);

   WPE_DataUnitSendToEnet(mac_dst, mac_src, svlan, cvlan, ip_dst, ip_src, tx_channel_handle);
   MY_PRINTF("SENDRECV: Packet no %d is sent.\n", packet_counter);
   MY_PRINTF("SENDRECV: Pause of about 10 seconds ...\n");
#ifdef WINSIM
   WP_Delay(20000000);
#else
#ifdef VERILOG
   WP_Delay(10000000);
#else
   WP_Delay(100000);
#endif
#endif

   MY_PRINTF( "SENDRECV: Packet no %i. Receiving ...\n", packet_counter);
   result = WPE_DataUnitReceive(rx_channel_handle, &(hexa_ret[0]));
   MY_PRINTF( "SENDRECV: Packet no %i. Receive Result = 0x%X\n\n", packet_counter, result);

   if(result == TRUE)
   {
      WP_CHAR ch_byte[3];
      WP_U32 i, byte;
      MY_PRINTF("Checking the packet:\n   expected dst mac: 0x%s \n            dst mac: 0x%02X%02X%02X%02X%02X%02X \n",
         mac_dst, hexa_ret[0], hexa_ret[1], hexa_ret[2], hexa_ret[3], hexa_ret[4], hexa_ret[5]);
      for(i = 0; i<6; i++)
      {
         ch_byte[0] = mac_dst[i*2];
         ch_byte[1] = mac_dst[i*2+1];
         ch_byte[2] = 0;
         sscanf(ch_byte, "%02X", &byte);
         if (hexa_ret[i] != byte)
         {
            printf("The packet is wrong\n");
            result = FALSE;
            break;
         }
      }
      MY_PRINTF("SENDRECV: Packet no %d is received.\n", packet_counter);
   }

   return result;
}

void WPE_CommonPacketSendOnly(WPE_pce_packet *packet, WP_handle tx_channel_handle, WP_U8 packet_type)
{
   WPE_CommonDataUnitSendOnly(
      packet->mac_da, packet->mac_sa, packet->vlan_stag, packet->vlan_ctag,
      packet->ipv4_da, packet->ipv4_sa, tx_channel_handle, packet_type);
}

void WPE_PacketSendOnly(WPE_pce_packet *packet, WP_handle tx_channel_handle)
{
   WPE_DataUnitSendOnly(
      packet->mac_da, packet->mac_sa,
      packet->vlan_stag, packet->vlan_ctag,
      packet->ipv4_da, packet->ipv4_sa, tx_channel_handle);
}

void WPE_DelayAfterPacketSend(void)
{
#ifdef WINSIM
   WP_Delay(10000000);
#endif
#ifdef VERILOG
   WP_Delay(100000000);
#endif
}

WP_U8 WPE_PacketReceiveOnly(WPE_pce_packet *packet, WP_handle rx_channel_handle)
{
   return WPE_DataUnitReceiveOnly(
      packet->mac_da, packet->mac_sa,
      packet->vlan_stag, packet->vlan_ctag,
      packet->ipv4_da, packet->ipv4_sa, rx_channel_handle);
}

WP_U8 WPE_PacketSendReceive(WPE_pce_packet *packet, WP_handle tx_channel_handle, WP_handle rx_channel_handle)
{
   return WPE_DataUnitSendReceive(
      packet->mac_da, packet->mac_sa,
      packet->vlan_stag, packet->vlan_ctag,
	  packet->ipv4_da, packet->ipv4_sa, tx_channel_handle, rx_channel_handle);
}

void WPE_PrintPceGlobalStats(void)
{
   WP_U32 temp1, temp2;
   WP_PceGlobalStatistics(WP_WINPATH(DEFAULT_WPID), &pce_global_stats);

   MY_PRINTF("\n\nPCE Global Statistics\n");

   temp1 = pce_global_stats.parser_header_access_errors;
   temp2 = pce_global_stats.parser_header_access_errors >> 32;
   MY_PRINTF("parser_header_access_errors %08X %08X\n", temp2, temp1);

   temp1 = pce_global_stats.parser_timeout_err;
   temp2 = pce_global_stats.parser_timeout_err >> 32;
   MY_PRINTF("parser_timeout_err          %08X %08X\n", temp2, temp1);

   temp1 = pce_global_stats.classifier_timeout_err;
   temp2 = pce_global_stats.classifier_timeout_err >> 32;
   MY_PRINTF("classifier_timeout_err      %08X %08X\n", temp2, temp1);

   temp1 = pce_global_stats.parser_accept;
   temp2 = pce_global_stats.parser_accept >> 32;
   MY_PRINTF("parser_accept               %08X %08X\n", temp2, temp1);

   MY_PRINTF("\n\n");
}

void WPE_PrintPceFilterStats(WP_handle filter)
{
   WP_U32 index;
   WP_U32 temp1, temp2;

   index = filter & 0xFF;

   WP_PceFilterStatistics (filter, &(pce_filter_stats[index]));

   STATS_PRINTF("PCE Filter %d Statistics\n", index);

   temp1 = pce_filter_stats[index].denied_pkts;
   temp2 = pce_filter_stats[index].denied_pkts >> 32;
   STATS_PRINTF("denied_pkts            %08X %08X\n", temp2, temp1);

   temp1 = pce_filter_stats[index].matched_pkts;
   temp2 = pce_filter_stats[index].matched_pkts >> 32;
   STATS_PRINTF("matched_pkts           %08X %08X\n", temp2, temp1);

   temp1 = pce_filter_stats[index].no_matched_pkts;
   temp2 = pce_filter_stats[index].no_matched_pkts >> 32;
   STATS_PRINTF("no_matched_pkts        %08X %08X\n", temp2, temp1);

   STATS_PRINTF("\n\n\n");
}

void WPE_ResetPceFilterStats(WP_handle filter)
{
   WP_U32 index;

   index = filter & 0xFF;

   WP_PceFilterStatisticsReset (filter, &(pce_filter_stats[index]));
}

/*************************************************************************************
 *
 * System tests common definitions
 *
 ************************************************************************************/
#define WPE_OK                      0
#define WPE_ERROR                   1

WP_handle wt_temp_host_port, wt_temp_host_device, wt_temp_host_rx_channel, wt_temp_flow_aggregation;

WP_port WT_available_wnets[WT_MAX_ENETS] =
{
#ifdef USE_SOCKET
   WP_PORT_ENET1,
   WP_PORT_ENET2,
   WP_PORT_ENET3,
   WP_PORT_ENET4,
   WP_PORT_ENET5,
   WP_PORT_ENET6,
   WP_PORT_ENET7,
   WP_PORT_ENET8,
   WP_PORT_ENET13,
   WP_PORT_ENET14,
#else
#ifdef WP_BOARD_WDS3_SL
   WP_PORT_ENET1,
   WP_PORT_ENET2,
   WP_PORT_ENET9,
   WP_PORT_ENET10,
   WP_PORT_ENET11,
   WP_PORT_ENET12,
#else
   WP_PORT_ENET13,
   WP_PORT_ENET14,
   WP_PORT_ENET15,
   WP_PORT_ENET16,
   WP_PORT_ENET3,
   WP_PORT_ENET4,
   WP_PORT_ENET5,
   WP_PORT_ENET6,
   WP_PORT_ENET7,
   WP_PORT_ENET8,
#endif//SLB
#endif
};

WP_port WT_available_xgi_ports[WT_MAX_XGI_PORTS] =
{
   WP_PORT_XGI1,
   WP_PORT_XGI2,
};

WP_port WT_available_upi_ports[WT_MAX_UPI_PORTS] =
{
   WP_PORT_UPI1,
   WP_PORT_UPI2,
};

void WT_Create_pools(void)
{
   WP_handle iw_pool_handle;
   iw_buffer_data->n_buffers = 1000;

#ifdef WINSIM
   iw_buffer_data[0].bus = WP_BUS_HOST;
   iw_buffer_data[0].bank = APP_BANK_HOST;
#endif

   iw_pool = iw_pool_handle = WP_PoolCreate(WP_WINPATH(DEFAULT_WPID), WP_pool_iwbuffer, iw_buffer_data);
   WPE_TerminateOnError(iw_pool_handle, "WP_PoolCreate 4", __LINE__);

   /* Create an interworking queue node */
   qn_iw->adjunct_pool = iw_pool_handle;
   qniw_enet = WP_QNodeCreate(WP_WINPATH(DEFAULT_WPID), WP_QNODE_IWQ | WP_QNODE_OPT_FMU, qn_iw);
}

typedef struct
{
   WP_U8 initialized;
   WP_handle data_pool_handle[4];
   WP_handle bd_pool[4];
   WP_handle adjunct_pool;
   WP_U32    buffer_data_size;
   WP_handle qn_hier;  /*Hierarchical Qnode*/
   WP_handle qn_lb;    /*Load Balancing Qnode*/
   WP_handle qn_sf;    /*Single fifo FBP*/
}WT_FmuFbpPool;

WT_FmuFbpPool poolFmuFbp =
{
   /* initialized */ WP_FALSE,
};

void WT_CreateFmuFbpPools()
{
   WP_U32    wpid=WP_WINPATH(DEFAULT_WPID);

   WP_pool_buffer_data buffer_data[1] =
   {
      {
         /* n_buffers */ 2000,
         /* offset */ 64,
         /* size */ 1900,//APP_HOST_BUFFER_SIZE,
         /* pad */ 0,
#ifdef USE_SOCKET
         /* bus */ WP_BUS_HOST,  /* This should really be WP_BUS_PACKET */
         /* bank */ APP_BANK_HOST
#else
#ifdef WP_BOARD_WDS3_SL
         /* bus */ WP_BUS_HOST,
         /* bank */ APP_BANK_HOST
#else
         /* bus */ WP_BUS_PACKET,
         /* bank */ APP_BANK_PACKET
#endif//SLB
#endif
      }
   };

   WP_pool_iw_bd bd_conf[1] =
   {
      {
         /*num_buffer_descriptors*/ 2000,
         /*buffer_pool*/0,
         /*bd_bus*/WP_BUS_INTERNAL,
         /*wmm_num_buffer_descriptors*/ 0
      }
   };

   WP_fmu_shaping_wrr weight_params[1] =
   {
      {
         /*weight*/ 5000,
         /*flags*/0,
      }
   };

   WP_qnode_iw_group group_highp [1] =
   {
      {
         /*num_of_bd_pools*/2,
         /*bd_pool*/   {0,0},
         /*fifo_selection_method*/WP_QNODE_IW_SELECTION_WRR,
         /*bd_pool_weight*/ /*{NULL,NULL}*/{&weight_params[0],&weight_params[0]}
      }
   };

   WP_qnode_iw_group group_lowp [1]=
   {
      {

         /*num_of_bd_pools*/2,
         /*bd_pool*/   {0,0},
         /*fifo_selection_method*/WP_QNODE_IW_SELECTION_WRR,
         /*bd_pool_weight*//*{NULL,NULL}*/{&weight_params[0],&weight_params[0]}
      }
   };

   WP_qnode_iw_hier iw_hierq[1] =
   {
      {
         /*num_of_groups*/2,
         /*iw_group*/ {group_highp[0],group_lowp[0]},
         /*group_selection_method*/ WP_QNODE_IW_SELECTION_STRICT,
         /*group_weight*/{NULL,NULL}

      }
   };

   /******* First - Create Regular Hierarchical Qnode pools ***********/
   buffer_data[0].bus = WP_BUS_HOST;
   buffer_data[0].bank = APP_BANK_HOST;
   poolFmuFbp.data_pool_handle[0] = WP_PoolCreate(wpid, WP_pool_iwbuffer, buffer_data);
   WPE_TerminateOnError(poolFmuFbp.data_pool_handle[0], "WP_PoolCreate() data pool 0",__LINE__);

#ifdef WP_BOARD_WDS3_SL
   buffer_data[0].bus = WP_BUS_HOST;
   buffer_data[0].bank = APP_BANK_HOST;
#else
   buffer_data[0].bus = WP_BUS_PACKET;
   buffer_data[0].bank = APP_BANK_PACKET;
#endif
   poolFmuFbp.data_pool_handle[1] = WP_PoolCreate(wpid, WP_pool_iwbuffer, buffer_data);
   WPE_TerminateOnError(poolFmuFbp.data_pool_handle[1], "WP_PoolCreate() data pool 1",__LINE__);

   buffer_data[0].bus = WP_BUS_HOST;
   buffer_data[0].bank = APP_BANK_HOST;
   poolFmuFbp.data_pool_handle[2] = WP_PoolCreate(wpid, WP_pool_iwbuffer, buffer_data);
   WPE_TerminateOnError(poolFmuFbp.data_pool_handle[2], "WP_PoolCreate() data pool 2",__LINE__);


#ifdef WP_BOARD_WDS3_SL
   buffer_data[0].bus = WP_BUS_HOST;
   buffer_data[0].bank = APP_BANK_HOST;
#else
   buffer_data[0].bus = WP_BUS_PACKET;
   buffer_data[0].bank = APP_BANK_PACKET;
#endif

   poolFmuFbp.data_pool_handle[3] = WP_PoolCreate(wpid, WP_pool_iwbuffer, buffer_data);
   WPE_TerminateOnError(poolFmuFbp.data_pool_handle[3], "WP_PoolCreate() data pool 3",__LINE__);

   bd_conf->buffer_pool = poolFmuFbp.data_pool_handle[0];
   bd_conf->bd_bus = WP_BUS_INTERNAL;
   poolFmuFbp.bd_pool[0] = WP_PoolCreate(WP_WINPATH(DEFAULT_WPID),WP_POOL_IW_BD, bd_conf);
   WPE_TerminateOnError(poolFmuFbp.bd_pool[0], "WP_PoolCreate() bd_pool 0", __LINE__);

   bd_conf->buffer_pool = poolFmuFbp.data_pool_handle[1];
   bd_conf->bd_bus = WP_BUS_INTERNAL;
   poolFmuFbp.bd_pool[1] = WP_PoolCreate(WP_WINPATH(DEFAULT_WPID),WP_POOL_IW_BD, bd_conf);
   WPE_TerminateOnError(poolFmuFbp.bd_pool[1], "WP_PoolCreate() bd_pool l", __LINE__);

   bd_conf->buffer_pool = poolFmuFbp.data_pool_handle[2];
   bd_conf->bd_bus = WP_BUS_PARAM;
   poolFmuFbp.bd_pool[2] = WP_PoolCreate(WP_WINPATH(DEFAULT_WPID),WP_POOL_IW_BD, bd_conf);
   WPE_TerminateOnError(poolFmuFbp.bd_pool[2], "WP_PoolCreate() bd_pool 2", __LINE__);

   bd_conf->buffer_pool = poolFmuFbp.data_pool_handle[3];
   bd_conf->bd_bus = WP_BUS_PARAM;
   poolFmuFbp.bd_pool[3] = WP_PoolCreate(WP_WINPATH(DEFAULT_WPID),WP_POOL_IW_BD, bd_conf);
   WPE_TerminateOnError(poolFmuFbp.bd_pool[3], "WP_PoolCreate() bd_pool 3", __LINE__);

   /* Configure and create Hierarchical Qnode */
   group_highp[0].bd_pool[0] = poolFmuFbp.bd_pool[0];
   group_highp[0].bd_pool[1] = poolFmuFbp.bd_pool[1];

   group_lowp[0].bd_pool[0] = poolFmuFbp.bd_pool[2];
   group_lowp[0].bd_pool[1] = poolFmuFbp.bd_pool[3];

   iw_hierq->iw_group[0] = group_highp[0];
   iw_hierq->iw_group[1] = group_lowp[0];

   poolFmuFbp.qn_hier = WP_QNodeCreate(WP_WINPATH(DEFAULT_WPID), WP_QNODE_IW_HIERARCHICAL, iw_hierq);
   WPE_TerminateOnError(poolFmuFbp.qn_hier, "WP_QNodeCreate() qn_hier", __LINE__);

   poolFmuFbp.initialized = WP_TRUE;

   //poolFmuFbp.qn_hier = qniw_enet;
}

WP_U8 WT_Init(void)
{
   WP_U16 i;
   for(i=0; i<WT_MAX_INTERFACES; i++)
      global_interfaces[i].status = WT_STATUS_UNINITIALIZED;

   global_info.interfaces = &(global_interfaces[0]);

   WT_Create_pools();

   wt_temp_host_port = WP_PortCreate(WP_WINPATH(DEFAULT_WPID), WP_PORT_IW_HOST, NULL);
   WPE_TerminateOnError(wt_temp_host_port, "Temp Host Port Create", __LINE__);

   wt_temp_host_device = WP_DeviceCreate(wt_temp_host_port, WP_PHY(0), WP_DEVICE_IW_HOST, NULL);
   WPE_TerminateOnError(wt_temp_host_device, "Temp Host Device Create", __LINE__);

   return WP_OK;
}

WP_handle WT_CreateEnetPortSgmii(WP_U8 physical_port_number, WP_U8 loopback_mode)
{
#ifndef USE_SOCKET
   WP_status status;
#endif
   WP_handle port_handle;

   if(physical_port_number > WT_MAX_ENETS)
   {
      printf("ERROR: Illegal Port Number\n");
      WT_Reboot();
      exit(1);
   }

#ifdef USE_SOCKET
   enet_s->interface_mode = WP_ENET_SGMII_1000;
#else
   enet_s->interface_mode = WP_ENET_SGMII_1000;

   status = WPX_BoardSerdesInit(WP_WINPATH(DEFAULT_WPID), WT_available_wnets[physical_port_number-1], loopback_mode);
   WPE_TerminateOnError(status, "WPX_BoardSerdesSetLoopback()", __LINE__);      
#endif

   port_handle = WP_PortCreate(WP_WINPATH(DEFAULT_WPID), WT_available_wnets[physical_port_number-1], enet_s);
   WPE_TerminateOnError(port_handle, "WP_PortCreate Enet", __LINE__);

   return port_handle;
}

WP_handle WT_CreateEnetPortXgi(WP_U8 physical_port_number, WP_U8 loopback_mode)
{
#ifndef USE_SOCKET
   WP_status status;
#endif
   WP_handle port_handle;

   if(physical_port_number > WT_MAX_XGI_PORTS)
   {
      printf("ERROR: Illegal XGI Port Number\n");
      WT_Reboot();
      exit(1);
   }

#ifndef USE_SOCKET
   if(loopback_mode == WP_FALSE)
   {
#ifndef WP_BOARD_WDS3_SL
      status = WPX_BoardXgiConfig(WP_WINPATH(DEFAULT_WPID), WT_available_xgi_ports[physical_port_number-1]);
      WPE_TerminateOnError(status, "WPX_BoardXgiConfig()", __LINE__);
#endif//SLB
   }

   status = WPX_BoardSerdesInit(WP_WINPATH(DEFAULT_WPID), WT_available_xgi_ports[physical_port_number-1], loopback_mode);
   WPE_TerminateOnError(status, "WPX_BoardSerdesInit()", __LINE__);
#endif

   /* Device creation must be done with no extended params */
   port_handle = WP_PortCreate(WP_WINPATH(DEFAULT_WPID), WT_available_xgi_ports[physical_port_number-1], enet_x);
   WPE_TerminateOnError(port_handle, "WP_PortCreate XGI", __LINE__);

   return port_handle;
}

WP_handle WT_CreateUpiPort(WP_U8 physical_port_number, WP_U8 loopback_mode)
{
   WP_handle port_handle;

   if(physical_port_number > WT_MAX_UPI_PORTS)
   {
      printf("ERROR: Illegal UPI Port Number\n");
      WT_Reboot();
      exit(1);
   }

   upi_pos_s[0].tx_characteristics =
      WP_UPI_MASTER |
      WP_UPI_PRT_ODD |
      WP_UPI_ROBIN |
      WP_TX_PRIORITY_8 |
      WP_UPI_BUS_WIDTH_16 |
      WP_UPI_FMU_HIERARCHICAL_SHAPING_MODE |
      WP_TQ_FAST_MODE |
      WP_UPI_SINGLE;

   upi_pos_s[0].rx_characteristics =
      WP_UPI_SLAVE |
      WP_UPI_SINGLE |
      WP_UPI_PRT_ODD |
      WP_UPI_ROBIN |
      WP_UPI_PRTCHK_OFF |
      WP_UPI_BUS_WIDTH_16;

   upi_pos_s[0].loopback = WP_UPI_LOOPBACK;

   port_handle = WP_PortCreate(WP_WINPATH(DEFAULT_WPID), WT_available_upi_ports[physical_port_number-1], upi_pos_s);
   WPE_TerminateOnError(port_handle, "WP_PortCreate UPI", __LINE__);

   return port_handle;
}

WP_handle WT_CreateEnetDevice(WP_handle port_handle, WP_U8 if_type, WP_U8 loopback_mode)
{
   WP_status status;
   WP_handle device_handle;

   enet_dev->loopbackmode = WP_ENET_NORMAL;
   enet_dev->extended_params = NULL;

#ifdef USE_SOCKET
   if(loopback_mode == WP_TRUE)
      enet_dev->loopbackmode = WP_ENET_LOOPBACK;
#endif

//      enet_dev->loopbackmode = WP_ENET_LOOPBACK;
   device_handle = WP_DeviceCreate(port_handle, WP_PHY(0), WP_DEVICE_ENET, enet_dev);
   WPE_TerminateOnError(device_handle, "WP_DeviceCreate Enet", __LINE__);

   switch(if_type)
   {
   case WT_IF_TYPE_SGMII:
      enet_dev->extended_params = enet_dev_ex;
      status = WP_DeviceModify(device_handle,
                               WP_DEV_MOD_ENET_PHYSTATMODE |
                               WP_DEV_MOD_ENET_TX_STATMODE |
                               WP_DEV_MOD_ENET_RX_STATMODE,
                               &enet_dev);
      WPE_TerminateOnError(status, "WP_DeviceModify() ENET", __LINE__);
      break;
   case WT_IF_TYPE_XGI:
      enet_dev->extended_params = enet_dev_ex;
      status = WP_DeviceModify(device_handle,
                               WP_DEV_MOD_ENET_PHYSTATMODE /*|
                               WP_DEV_MOD_ENET_TX_STATMODE |
                               WP_DEV_MOD_ENET_RX_STATMODE*/,
                               &enet_dev);
      WPE_TerminateOnError(status, "WP_DeviceModify() ENET", __LINE__);
      break;
   default:
      break;
   }

   return device_handle;
}

WP_handle WT_CreateHsPosDevice(WP_handle port_handle, WP_U8 loopback_mode)
{
   WP_handle device_handle;

   device_handle = WP_DeviceCreate(port_handle, WP_PHY(0), WP_DEVICE_HSPOS, upi_hspos_device);
   WPE_TerminateOnError(device_handle, "WP_DeviceCreate HSPOS", __LINE__);

   return device_handle;
}

void WT_CreateEnetChannelsAndTxBindings(WP_U8 if_number)
{
   WP_U8 i, tx_channels_number;
   WP_handle device;
   WP_status status;
   WP_handle qn_tx, qn_rx;

   switch(global_interfaces[if_number].type)
   {
   case WT_IF_TYPE_SGMII:
      qn_tx = qn_rx = qniw_enet;
      break;
   case WT_IF_TYPE_XGI:
      qn_tx = qniw_enet;
      qn_rx = poolFmuFbp.qn_hier;
      break;
   default:
      break;
   }

   device = global_interfaces[if_number].device;
   tx_channels_number = global_interfaces[if_number].tx_channels_number;

   for(i=0;i<1;i++)
   {
      ch_config_enet[0].iwmode = WP_PKTCH_IWM_ENABLE;
      global_interfaces[if_number].tx_channels[i] = WP_ChannelCreate(WT_TX_CHANNEL_TAG + if_number, device, qn_tx, WP_CH_TX, WP_ENET, &ch_config_enet[0]);
      WPE_TerminateOnError(global_interfaces[if_number].tx_channels[i], "WP_ChannelCreate() Enet TX", __LINE__);

      status = WP_IwTxBindingCreate(global_interfaces[if_number].tx_channels[i], WP_IW_TX_BINDING, tx_binding_cfg);
      WPE_TerminateOnError(status, "WP_IwTxBindingCreate()", __LINE__);
   }
   global_interfaces[if_number].tx_channel = global_interfaces[if_number].tx_channels[0];

   ch_config_enet[0].iwmode = WP_PKTCH_IWM_ENABLE;
   global_interfaces[if_number].rx_channel = WP_ChannelCreate(WT_RX_CHANNEL_TAG + if_number, device, qn_rx, WP_CH_RX, WP_ENET, &ch_config_enet[0]);
   WPE_TerminateOnError(global_interfaces[if_number].rx_channel, "WP_ChannelCreate() Enet RX", __LINE__);
   global_interfaces[if_number].qnode_rx = qn_rx;
}


void WT_GroupsCreateUPI(WP_U8 if_number)
{
   /* create shaping groups and blocks */
   WP_handle upi_device;

   upi_device = global_interfaces[if_number].device;

   l1_group_config->tx_shaping_type = WP_FMU_SHAPING_TYPE_CIR_EIR;
   l1_group_config->tx_shaping_params = &cir_eir_shaping_params[0];
   cir_eir_shaping_params[0].flags = WP_FMU_SHAPING_CIR_EIR_SELECT_MODE_RR;

   global_interfaces[if_number].l1_groups[0] = WP_ShapingGroupCreate(upi_device,
                                                           WP_SHAPING_GROUP_TYPE_PACKET,
                                                           l1_group_config);
   WPE_TerminateOnError(global_interfaces[if_number].l1_groups[0] , "WP_ShapingGroupCreate l1 group", __LINE__);
   printf("WP_ShapingGroupCreate l1 group\n");

   global_interfaces[if_number].l2_blocks[0] = WP_ShapingBlockCreate( WP_SYSHANDLE(DEFAULT_WPID),
                                                            WP_L2_GROUP,
                                                            8,
                                                            WP_NUM_FIFOS_8);
   WPE_TerminateOnError(global_interfaces[if_number].l2_blocks[0] , "WP_ShapingBlockCreate", __LINE__);
   printf("WP_ShapingBlockCreate l2 block\n");

   l2_group_config->block_level = 0;
   l2_group_config->block_handle = global_interfaces[if_number].l2_blocks[0];
   global_interfaces[if_number].l2_groups[0] = WP_ShapingGroupCreate(global_interfaces[if_number].l1_groups[0],
                                                           WP_SHAPING_GROUP_TYPE_PACKET,
                                                           l2_group_config);
   WPE_TerminateOnError(global_interfaces[if_number].l2_groups[0] , "WP_ShapingGroupCreate l2 group", __LINE__);
   printf("WP_ShapingGroupCreate l2 group\n");
}

void WT_CreateHdlcChannelsAndTxBindings(WP_U8 if_number)
{
   WP_handle device;
   WP_status status;
   WP_handle qn_tx, qn_rx;

   qn_tx = qn_rx = qniw_enet;

   WT_GroupsCreateUPI(if_number);

   device = global_interfaces[if_number].device;

   ch_config_hdlc[0].iwmode = WP_PKTCH_IWM_ENABLE;
   ch_config_hdlc[0].tx_tq = 1;
   global_interfaces[if_number].tx_channels[0] =
      WP_ChannelCreate (WT_TX_CHANNEL_TAG + if_number,
                        global_interfaces[if_number].l2_groups[0],
                        qn_tx,
                        WP_CH_TX,
                        WP_HDLC,
                        &ch_config_hdlc[0]);
   WPE_TerminateOnError(global_interfaces[if_number].tx_channels[0], "WP_ChannelCreate() HDLC TX", __LINE__);

   status = WP_IwTxBindingCreate(global_interfaces[if_number].tx_channels[0], WP_IW_TX_BINDING, tx_binding_cfg);
   WPE_TerminateOnError(status, "WP_IwTxBindingCreate()", __LINE__);

   global_interfaces[if_number].tx_channel = global_interfaces[if_number].tx_channels[0];

   //ch_config_hdlc[0].iwmode = WP_PKTCH_IWM_DISABLE;
   global_interfaces[if_number].rx_channel =
      WP_ChannelCreate (WT_RX_CHANNEL_TAG + if_number,
                        device,
                        qn_rx,
                        WP_CH_RX,
                        WP_HDLC,
                        &ch_config_hdlc[0]);
   WPE_TerminateOnError(global_interfaces[if_number].rx_channel, "WP_ChannelCreate() HDLC RX", __LINE__);
}

WP_U8 WT_CreateInterface(WP_U8 if_number, WT_PhysicalPortType type, WP_U8 physical_port_number,
                               WP_U8 loopback_mode, WP_U8 tx_channels_number, WP_U16 num_of_flow_aggregations,
                               WP_U8 default_flow_agg_number, WP_U16 flow_agg_vlan_id)
{
   if(if_number > WT_MAX_INTERFACES)
   {
      printf("ERROR: Illegal Interface Number\n");
      WT_Reboot();
      exit(1);
   }

   if(global_interfaces[if_number].status != WT_STATUS_UNINITIALIZED)
   {
      printf("ERROR: Interface is already Initialized\n");
      WT_Reboot();
      exit(1);
   }

   if(loopback_mode != WP_TRUE && loopback_mode != WP_FALSE)
   {
      printf("ERROR: Illegal loopback mode\n");
      WT_Reboot();
      exit(1);
   }

   global_interfaces[if_number].status = WT_STATUS_INITIALIZED;
   global_interfaces[if_number].type = type;
   global_interfaces[if_number].default_flow_agg_number = default_flow_agg_number;
   global_interfaces[if_number].flow_agg_vlan_id = flow_agg_vlan_id;
   global_interfaces[if_number].num_of_flow_aggregations = num_of_flow_aggregations;

   switch(type)
   {
   case WT_IF_TYPE_SGMII:
      if(tx_channels_number > WT_MAX_IF_ENET_TX_CHANNELS)
      {
         printf("ERROR: Illegal TX Channels Number\n");
         WT_Reboot();
         exit(1);
      }

      global_interfaces[if_number].port = WT_CreateEnetPortSgmii(physical_port_number, loopback_mode);
      global_interfaces[if_number].device = WT_CreateEnetDevice(global_interfaces[if_number].port, type, loopback_mode);
      global_interfaces[if_number].tx_channels_number = tx_channels_number;
      break;
   case WT_IF_TYPE_UPI_HSPOS:
      if(tx_channels_number > WT_MAX_IF_ENET_TX_CHANNELS)
      {
         printf("ERROR: Illegal TX Channels Number\n");
         WT_Reboot();
         exit(1);
      }

      if(loopback_mode != WP_TRUE)
      {
         printf("ERROR: UPI is not with loopback\n");
         WT_Reboot();
         exit(1);
      }

      global_interfaces[if_number].port = WT_CreateUpiPort(physical_port_number, loopback_mode);
      global_interfaces[if_number].device = WT_CreateHsPosDevice(global_interfaces[if_number].port, loopback_mode);
      global_interfaces[if_number].tx_channels_number = tx_channels_number;
      break;
   case WT_IF_TYPE_XGI:
      if(tx_channels_number > WT_MAX_IF_ENET_TX_CHANNELS)
      {
         printf("ERROR: Illegal TX Channels Number\n");
         WT_Reboot();
         exit(1);
      }
      if(poolFmuFbp.initialized != WP_TRUE)
         WT_CreateFmuFbpPools();

      global_interfaces[if_number].port = WT_CreateEnetPortXgi(physical_port_number, loopback_mode);
      global_interfaces[if_number].device = WT_CreateEnetDevice(global_interfaces[if_number].port, type, loopback_mode);
      global_interfaces[if_number].tx_channels_number = tx_channels_number;
      break;
   case WT_IF_TYPE_HOST:
      global_interfaces[if_number].port = WP_PortCreate(WP_WINPATH(DEFAULT_WPID), WP_PORT_IW_HOST, NULL);
      WPE_TerminateOnError(global_interfaces[if_number].port, "Host Port Create", __LINE__);
      global_interfaces[if_number].device = WP_DeviceCreate(global_interfaces[if_number].port, WP_PHY(0), WP_DEVICE_IW_HOST, NULL);
      WPE_TerminateOnError(global_interfaces[if_number].device, "Host Device Create", __LINE__);
      break;
   default:
      {
         printf("ERROR: Illegal Interface Type\n");
         WT_Reboot();
         exit(1);
      }
   }

   /* create iw port */

   return WPE_OK;
}

void WT_CreateChannelsAndTxBindings(WP_U16 if_number)
{
   switch(global_interfaces[if_number].type)
   {
   case WT_IF_TYPE_SGMII:
   case WT_IF_TYPE_XGI:
      WT_CreateEnetChannelsAndTxBindings(if_number);
      break;
   case WT_IF_TYPE_UPI_HSPOS:
      WT_CreateHdlcChannelsAndTxBindings(if_number);
      break;
   case WT_IF_TYPE_HOST:
      global_interfaces[if_number].rx_channel = WP_ChannelCreate(WT_RX_CHANNEL_TAG, wt_temp_host_device, qniw_enet, WP_CH_RX, WP_IW_HOST, &iw_rx_ch_cfg);
      WPE_TerminateOnError(global_interfaces[if_number].rx_channel, "Host RX Channel Create(iw)", __LINE__);
      break;
   default:
      break;
   }
}

void WT_CreateFlowAggregationsAndBports(void)
{
   WP_U16 i, j;
   WP_status status;

   MY_PRINTF("Creating Temp Flow aggregation ...\n");
   tx_agg_bridging->txfunc = wt_temp_host_rx_channel;
   wt_temp_flow_aggregation = WP_IwFlowAggregationCreate(WP_WINPATH(DEFAULT_WPID), WP_IW_VLAN_AWARE_BRIDGING_MODE, &tx_agg_bridging);
   WPE_TerminateOnError(wt_temp_flow_aggregation,"WP_IwFlowAggregationCreate()", __LINE__);

   for(i=0; i<WT_MAX_INTERFACES; i++)
   {
      if(global_interfaces[i].status == WT_STATUS_INITIALIZED)
      {
         switch(global_info.iw_system_type)
         {
         case WP_IW_ROUTING_MODE:
            route_port_cfg.flow_agg = wt_temp_flow_aggregation;
            route_port_cfg.tag = WT_IW_PORT_TAG+i;
            global_interfaces[i].iw_port = WP_IwPortCreate(global_info.iw_system, &route_port_cfg);
            WPE_TerminateOnError(global_interfaces[i].iw_port, "WP_IwPortCreate() Routing", __LINE__);
            break;
         case WP_IW_TRANS_BRIDGING_MODE:
         case WP_IW_VLAN_AWARE_BRIDGING_MODE:
            bport_conf->flow_agg = wt_temp_flow_aggregation;
            bport_conf->tag = WT_IW_PORT_TAG+i;
            global_interfaces[i].iw_port = WP_IwBridgingPortCreate(global_info.iw_system, bport_conf);
            WPE_TerminateOnError(global_interfaces[i].iw_port, "WP_IwBridgingPortCreate() ", __LINE__);
            break;
         default:
            break;
         }

         switch(global_interfaces[i].type)
         {
         case WT_IF_TYPE_SGMII:
         case WT_IF_TYPE_UPI_HSPOS:
         case WT_IF_TYPE_XGI:
            for(j=0; j<global_interfaces[i].num_of_flow_aggregations; j++)
            {
               if(global_interfaces[i].flow_agg_vlan_id == 0)
               {
                  flow_agg_gen[0].iw_port = global_interfaces[i].iw_port;
                  flow_agg_gen[0].txfunc = global_interfaces[i].tx_channel;
                  global_interfaces[i].flow_aggregations[j] = WP_IwFlowAggregationCreate(WP_WINPATH(DEFAULT_WPID),
                                                               WP_IW_GENERIC_MODE,
                                                               &flow_agg_gen);
                  if(j<10)
                     WPE_TerminateOnError(global_interfaces[i].flow_aggregations[j], "WP_IwFlowAggregationCreate() Generic", __LINE__);
                  else
                     WPE_TerminateOnErrorNoPrintIfOK(global_interfaces[i].flow_aggregations[j], "WP_IwFlowAggregationCreate() Generic", __LINE__);
               }
               else
               {
                  tx_agg_bridging->iw_port = global_interfaces[i].iw_port;
                  tx_agg_bridging->txfunc = global_interfaces[i].tx_channel;
                  tx_agg_bridging->replace_vlan_id = WP_IW_REPLACE_VLAN_ID;
                  tx_agg_bridging->vlan_id = global_interfaces[i].flow_agg_vlan_id;
                  global_interfaces[i].flow_aggregations[j] = WP_IwFlowAggregationCreate(WP_WINPATH(DEFAULT_WPID), WP_IW_VLAN_AWARE_BRIDGING_MODE, &tx_agg_bridging);
                  if(j<10)
                     WPE_TerminateOnError(global_interfaces[i].flow_aggregations[j] ,"WP_IwFlowAggregationCreate() VLAN ID replaced", __LINE__);
                  else
                     WPE_TerminateOnErrorNoPrintIfOK(global_interfaces[i].flow_aggregations[j], "WP_IwFlowAggregationCreate() Generic", __LINE__);
               }
            }
            global_interfaces[i].flow_aggregation = global_interfaces[i].flow_aggregations[0];
            break;
         case WT_IF_TYPE_HOST:
            for(j=0; j<global_interfaces[i].num_of_flow_aggregations; j++)
            {
               if(global_interfaces[i].flow_agg_vlan_id != 0)
               {
                  tx_agg_bridging->replace_vlan_id = WP_IW_REPLACE_VLAN_ID;
                  tx_agg_bridging->vlan_id = global_interfaces[i].flow_agg_vlan_id;
               }
               tx_agg_bridging->iw_port = global_interfaces[i].iw_port;
               tx_agg_bridging->txfunc = global_interfaces[i].rx_channel;
               global_interfaces[i].flow_aggregations[j] = WP_IwFlowAggregationCreate(WP_WINPATH(DEFAULT_WPID), WP_IW_VLAN_AWARE_BRIDGING_MODE, &tx_agg_bridging);
               WPE_TerminateOnError(global_interfaces[i].flow_aggregations[j] ,"WP_IwFlowAggregationCreate()", __LINE__);
            }
            global_interfaces[i].flow_aggregation = global_interfaces[i].flow_aggregations[0];
            break;
         default:
            break;
         }
      }
   }

   for(i=0; i<WT_MAX_INTERFACES; i++)
   {
      if(global_interfaces[i].status == WT_STATUS_INITIALIZED &&
         global_interfaces[i].default_flow_agg_number != 0 &&
         global_interfaces[global_interfaces[i].default_flow_agg_number].status == WT_STATUS_INITIALIZED)
      {
         switch(global_info.iw_system_type)
         {
         case WP_IW_ROUTING_MODE:
            route_port_cfg.flow_agg = global_interfaces[global_interfaces[i].default_flow_agg_number].flow_aggregation;
            status = WP_IwPortModify(
               global_interfaces[i].iw_port,
               WP_IW_PORT_MODIFY_FLOW_AGG,
               &route_port_cfg);
            WPE_TerminateOnError(status, "WP_IwPortModify() Routing - set the default flow aggregation", __LINE__);
            break;
         case WP_IW_TRANS_BRIDGING_MODE:
         case WP_IW_VLAN_AWARE_BRIDGING_MODE:
            bport_conf->flow_agg = global_interfaces[global_interfaces[i].default_flow_agg_number].flow_aggregation;
            status = WP_IwPortModify(
               global_interfaces[i].iw_port,
               WP_IW_PORT_MODIFY_FLOW_AGG,
               &bport_conf);
            WPE_TerminateOnError(status, "WP_IwPortModify() Bridging - set the default flow aggregation", __LINE__);
            break;
         default:
            break;
         }
      }
   }
}

void WT_CreateIwRxBindingAndTxHostChannel(WP_iw_system_type type)
{
   WP_U16 ii;
   WP_handle qn_rx;
   WP_status status = WP_OK;

   for(ii=0; ii<WT_MAX_INTERFACES; ii++)
   {
      if(global_interfaces[ii].status == WT_STATUS_INITIALIZED)
      {
         qn_rx = qniw_enet;
         switch(global_interfaces[ii].type)
         {
         case WT_IF_TYPE_XGI:
            qn_rx = poolFmuFbp.qn_hier;
         case WT_IF_TYPE_SGMII:
         case WT_IF_TYPE_UPI_HSPOS:
            switch(global_info.iw_system_type)
            {
            case WP_IW_ROUTING_MODE:
               rx_binding_routing->input_port = global_interfaces[ii].iw_port;
               rx_binding_routing->pce_if_handle = global_interfaces[ii].pce_interface_handle;
               status = WP_IwRxBindingCreate(global_interfaces[ii].rx_channel,
                                             global_info.iw_system,
                                             qn_rx,
                                             &(rx_binding_routing[0]));
               WPE_TerminateOnError(status, "WP_RxBindingCreate()", __LINE__);
               break;
            case WP_IW_TRANS_BRIDGING_MODE:
            case WP_IW_VLAN_AWARE_BRIDGING_MODE:
               rx_binding_cfg->input_port = global_interfaces[ii].iw_port;
               rx_binding_cfg->pce_if_handle = global_interfaces[ii].pce_interface_handle;
               status = WP_IwRxBindingCreate(global_interfaces[ii].rx_channel,
                                             global_info.iw_system,
                                             qn_rx,
                                             rx_binding_cfg);
               WPE_TerminateOnError(status, "WP_RxBindingCreate()", __LINE__);
               break;
            default:
               break;
            }
            break;
         case WT_IF_TYPE_HOST:
            iw_tx_ch_cfg->iw_system = global_info.iw_system;
            iw_tx_ch_cfg->group_id = WP_IW_CH_GROUP_ID_NULL;
            iw_tx_ch_cfg->input_port = global_interfaces[ii].iw_port;
            iw_tx_ch_cfg->pce_if_handle = global_interfaces[ii].pce_interface_handle;
            global_interfaces[ii].tx_channels[0] = WP_ChannelCreate(WT_TX_CHANNEL_TAG+ii,
                                                                    global_interfaces[ii].device,
                                                                    qniw_enet,
                                                                    WP_CH_TX,
                                                                    WP_IW_HOST,
                                                                    &iw_tx_ch_cfg);
            WPE_TerminateOnError(global_interfaces[ii].tx_channels[0], "Host TX Channel Create(iw)", __LINE__);
            global_interfaces[ii].tx_channel = global_interfaces[ii].tx_channels[0];
            break;
         default:
            break;
         }
      }
   }
}

void WT_CreateIwSystemAndItsComponents(WP_iw_system_type type)
{
   WP_U16 i;
   WP_status status;
   /* Create the temporary host channel */
   wt_temp_host_rx_channel = WP_ChannelCreate(WT_RX_CHANNEL_TAG, wt_temp_host_device, qniw_enet, WP_CH_RX, WP_IW_HOST, &iw_rx_ch_cfg);
   WPE_TerminateOnError(wt_temp_host_rx_channel, "Temp Host RX Channel Create(iw)", __LINE__);

   for(i=0;i<WT_MAX_INTERFACES;i++)
   {
      if(global_interfaces[i].status == WT_STATUS_INITIALIZED)
      {
         WT_CreateChannelsAndTxBindings(i);
      }
   }

   global_info.iw_system_type = type;

   switch(type)
   {
   case WP_IW_ROUTING_MODE:
      global_info.iw_system = WP_IwSystemCreate(WP_WINPATH(DEFAULT_WPID), WP_IW_ROUTING_MODE, &(iw_sys_one_routing[0]));
      WPE_TerminateOnError(global_info.iw_system, "WP_IwSystemCreate() - Routing", __LINE__);
      break;
   case WP_IW_TRANS_BRIDGING_MODE:
      global_info.iw_system = WP_IwSystemCreate(WP_WINPATH(DEFAULT_WPID), WP_IW_TRANS_BRIDGING_MODE, &iw_sys);
      WPE_TerminateOnError(global_info.iw_system, "WP_IwSystemCreate() - Transparent Bridging", __LINE__);
      break;
   case WP_IW_VLAN_AWARE_BRIDGING_MODE:
      global_info.iw_system = WP_IwSystemCreate(WP_WINPATH(DEFAULT_WPID), WP_IW_VLAN_AWARE_BRIDGING_MODE, &iw_sys);
      WPE_TerminateOnError(global_info.iw_system, "WP_IwSystemCreate() - Vlan Aware Bridging", __LINE__);
      break;
   default:
      break;
   }

   WT_CreateFlowAggregationsAndBports();

   for(i=0; i<WT_MAX_INTERFACES; i++)
   {
      if(global_interfaces[i].status == WT_STATUS_INITIALIZED)
      {
         switch(global_interfaces[i].type)
         {
         case WT_IF_TYPE_UPI_HSPOS:
            status = WP_ShapingGroupEnable(global_interfaces[i].l1_groups[0]);
            WPE_TerminateOnError(status, "WP_ShapingGroupEnable L1", __LINE__);
            status = WP_ShapingGroupEnable(global_interfaces[i].l2_groups[0]);
            WPE_TerminateOnError(status, "WP_ShapingGroupEnable L2", __LINE__);
            status = WP_PortEnable(global_interfaces[i].port, WP_DIRECTION_DUPLEX);
            WPE_TerminateOnError(status, "WP_PortEnable", __LINE__);
            status = WP_DeviceEnable(global_interfaces[i].device, WP_DIRECTION_DUPLEX);
            WPE_TerminateOnError(status, "WP_DeviceEnable", __LINE__);
            status = WP_ChannelEnable(global_interfaces[i].tx_channels[0]);
            WPE_TerminateOnError(status, "WP_ChannelEnable TX", __LINE__);
            break;
         case WT_IF_TYPE_SGMII:
         case WT_IF_TYPE_XGI:
            status = WP_PortEnable(global_interfaces[i].port, WP_DIRECTION_DUPLEX);
            WPE_TerminateOnError(status, "WP_PortEnable", __LINE__);
            status = WP_DeviceEnable(global_interfaces[i].device, WP_DIRECTION_DUPLEX);
            WPE_TerminateOnError(status, "WP_DeviceEnable", __LINE__);
            status = WP_ChannelEnable(global_interfaces[i].rx_channel);
            WPE_TerminateOnError(status, "WP_ChannelEnable RX", __LINE__);
            status = WP_ChannelEnable(global_interfaces[i].tx_channels[0]);
            WPE_TerminateOnError(status, "WP_ChannelEnable TX", __LINE__);
            break;
         default:
            break;
         }
      }
   }
}

#define my_clock WP_TimeRead
void WPE_ComputeClocksPerMsec(void)
{
   WP_U32 min_CLOCKS_PER_MSEC=0xFFFFFFFF, clock_of_clock;
   WP_U32 i, timer1, timer2;

   timer1 = my_clock();
   timer2 = my_clock();
   clock_of_clock = (WP_U32) timer2-timer1;
   printf("CLOCK of my_clock: %d \n", clock_of_clock); timer1 = timer2;
   timer2 = my_clock();
   printf("CLOCK of printf & my_clock: %d \n", (WP_U32) timer2-timer1); timer1 = timer2;
   for(i=0; i<5; i++)
   {
      timer1 = my_clock();
      WP_Delay(100000);
      timer2 = my_clock();
      ClocksPerMsec = (timer2 - timer1 - clock_of_clock)/100;
      if(min_CLOCKS_PER_MSEC > ClocksPerMsec)
         min_CLOCKS_PER_MSEC = ClocksPerMsec;
   }
   ClocksPerMsec = min_CLOCKS_PER_MSEC;
}

void WPE_SetHexChar(WP_CHAR *char_ptr, WP_U8 var)
{
   if (var < 0xA)
      char_ptr[0] = var + '0';
   else
      char_ptr[0] = var + 'A' - 0xA;
}
