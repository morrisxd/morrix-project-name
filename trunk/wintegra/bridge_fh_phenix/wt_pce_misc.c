#include "wpi_veneer_memory.h"
#include "wpu_bus_mon.h"
#include "wpu_wgu.h"
#include "wpu_sa_actual.h"
#include "wpu_ta_base.h"
#include "wpu_ta_error.h"
#include "wpu_ta_ord.h"
#include "wpu_ta_stat.h"
#include "wpu_sa_alloc.h"
#include "wpu_sa_limits.h"
#include "wpu_tb.h"

#ifdef PRINT_STAT_FIELD
#undef PRINT_STAT_FIELD
#endif
#define PRINT_STAT_FIELD(field) printf( "%08x%08x", (WP_U32)(field>>32), (WP_U32)field)

/*****************************************************************************
 * Function name: WTE_IpChecksumCalc
 * Description  : calculate checksum of IP header
 * Input  params: buff - pointer to buffer, len - size of IP header
 * Output params:
 * Return val   : calculated checksum
 *****************************************************************************/
static WP_U16 WTE_IpChecksumCalc(WP_U8 *buff, WP_U16 len)
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
static void terminate_on_error(WP_handle handle, WP_CHAR *s)
{
   if (WP_ERROR_P(handle)) {
      printf("%s: Terminal error (handle 0x%x) %s\n", appname, handle, s);
      printf("%s: Error type - %s\n", appname, WP_error_name[WP_ERROR(handle)]);
      exit(1);
   } else {
#ifdef DEBUG_PRINT_OUT
         printf("%s: Handle/Status returned from %s is 0x%x \n", appname, s, handle);
#endif
   }
}




/**
 * Displays Route Port Statistics.
 */
WP_U32 WPT_DisplayRoutePortStats( WP_handle port_handle, WP_CHAR* type)
{
   WP_U32 i;
   WP_U32 rx_packets = 0;
   WP_route_port_stats rport_stats;
   memset( &rport_stats, 0, sizeof(WP_route_port_stats));
   WP_IwPortStatistics(port_handle, &rport_stats );
   printf ("\n%s Route port statistics of port %d (%x)\n",
           type,(port_handle & 0x000000ff),port_handle);
   printf ("============================================\n");
   printf ("Rx Valid Packets:                 ");
   PRINT_STAT_FIELD(rport_stats.rx_valid_packets);
   printf ("\nrx_ipv4_option_packets:           ");
   PRINT_STAT_FIELD(rport_stats.rx_ipv4_option_packets);
   printf ("\nrx_non_ip_packets:                ");
   PRINT_STAT_FIELD(rport_stats.rx_non_ip_packets);
   printf ("\nrx_compressed_packets:            ");
   PRINT_STAT_FIELD(rport_stats.rx_compressed_packets);
   printf ("\nrx_valid_mpls_packets:            ");
   PRINT_STAT_FIELD(rport_stats.rx_valid_mpls_packets);
   printf ("\nrx_protocol_error:                ");
   PRINT_STAT_FIELD(rport_stats.rx_protocol_error);
   printf ("\nrx_checksum_error:                ");
   PRINT_STAT_FIELD(rport_stats.rx_checksum_error);
   printf ("\nrx_discard_classifier:            ");
   PRINT_STAT_FIELD(rport_stats.rx_discard_classifier);
   printf ("\nrx_mpls_lookup_drop:              ");
   PRINT_STAT_FIELD(rport_stats.rx_mpls_lookup_drop);
   printf ("\ntx_forwarded_packets:             ");
   PRINT_STAT_FIELD(rport_stats.tx_forwarded_packets);
   printf ("\nrx_ipv6_hop_by_hop_host_term:     ");
   PRINT_STAT_FIELD(rport_stats.rx_ipv6_hop_by_hop_host_term);
   printf ("\nrx_ipv6_link_local_host_term:     ");
   PRINT_STAT_FIELD(rport_stats.rx_ipv6_link_local_host_term);
   printf ("\nrx_discard_lpm:                   ");
   PRINT_STAT_FIELD(rport_stats.rx_discard_lpm);
   for(i=0; i < WP_IW_DFC_MAX_NUM_OF_FILTERS; i++)
   {
      if (rport_stats.discard_dfc_filter[i])
      {
         printf ("\ndiscard_dfc_filter %03d:                   ", i);
         PRINT_STAT_FIELD(rport_stats.discard_dfc_filter[i]);
      }
   }
   for(i=0; i < WP_PCE_MAX_NUM_OF_FILTERS; i++)
   {
      if(rport_stats.discard_pce_filter[i])
      {
         printf ("\ndiscard_pce_filter %03d:                ", i);
         PRINT_STAT_FIELD(rport_stats.discard_pce_filter[i]);
      }
   }
   printf ("\ningress_policer_non_conforming_packets:   ");
   PRINT_STAT_FIELD(rport_stats.ingress_policer_non_conforming_packets);

   rx_packets = (WP_U32)rport_stats.rx_valid_packets;
   return rx_packets;
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
   printf ("\n%s Bridge port statistics of port %d (%x)\n",
           type,(port_handle & 0x000000ff),port_handle);
   printf ("============================================\n");
   printf ("Rx Valid Packets:               ");
   PRINT_STAT_FIELD(bport_stats.rx_valid_packets);
   printf ("\nDiscard VLAN acceptable filter: ");
   PRINT_STAT_FIELD(bport_stats.discard_vlan_acceptable_filter);
   printf ("\nDiscard ingress filter:         ");
   PRINT_STAT_FIELD(bport_stats.discard_ingress_filter);
   printf ("\nDiscard bridge classifier:      ");
   PRINT_STAT_FIELD(bport_stats.discard_bridge_classifier);
   printf ("\nDiscard unknown MAC SA:         ");
   PRINT_STAT_FIELD(bport_stats.discard_unk_macsa);
   printf ("\nDeny MAC SA:                    ");
   PRINT_STAT_FIELD(bport_stats.deny_mac_sa);
   printf ("\nDeny MAC DA:                    ");
   PRINT_STAT_FIELD(bport_stats.deny_mac_da);
   printf ("\nRx BC valid packets:            ");
   PRINT_STAT_FIELD(bport_stats.rx_bc_valid_packets);
   printf ("\nRx MC valid packets:            ");
   PRINT_STAT_FIELD(bport_stats.rx_mc_valid_packets);
   printf ("\nForwarded UC packets:           ");
   PRINT_STAT_FIELD(bport_stats.forwarded_uc_packets);
   printf ("\nForwarded BC packets:           ");
   PRINT_STAT_FIELD(bport_stats.forwarded_bc_packets);
   printf ("\nForwarded MC packets:           ");
   PRINT_STAT_FIELD(bport_stats.forwarded_mc_packets);
   printf ("\nProtocol Error      :           ");
   PRINT_STAT_FIELD(bport_stats.protocol_error);
   for(i=0; i < WP_PCE_MAX_NUM_OF_FILTERS; i++)
   {
      if (bport_stats.discard_pce_filter[i])
      {
         printf ("\nPCE Filter %03d denied packets:  ", i);
         PRINT_STAT_FIELD(bport_stats.discard_pce_filter[i]);
      }
   }
   printf("\n\n");
   rx_packets = (WP_U32)bport_stats.rx_valid_packets;
   return rx_packets;
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
   printf ("\n%s Aggregation flow statistics of flow %d (%x)\n",
           type,(agg & 0x000000ff),agg);
   printf ("==========================================================\n");
   printf ("Forward Packet:          ");
   PRINT_STAT_FIELD(iw_flow_stats.forward_packet);
   printf( "\nFBP Drop Packets:        ");
   PRINT_STAT_FIELD(iw_flow_stats.fbp_drop_packets);
   printf( "\nMTU Drop Packets:        ");
   PRINT_STAT_FIELD(iw_flow_stats.mtu_drop_packets);
   printf( "\nTTL Drop Packets:        ");
   PRINT_STAT_FIELD(iw_flow_stats.ttl_drop_packets);
   printf( "\nTX Queue Drop Packets:   ");
   PRINT_STAT_FIELD(iw_flow_stats.tx_queue_drop_packets);
   printf( "\nMPLS Drop:               ");
   PRINT_STAT_FIELD(iw_flow_stats.mpls_drop);
   printf("\n\n");
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
   printf ("\n%s Generic Flow Aggregation (%x) statistics \n",
           type, agg);
   printf ("==========================================================\n");
   printf ("Forward Packet:          ");
   PRINT_STAT_FIELD(iw_flow_stats.forward_packet);
   printf ("\nForwarded Bytes:         ");
   PRINT_STAT_FIELD(iw_flow_stats.forwarded_bytes);
   printf( "\nMTU Drop Packets:        ");
   PRINT_STAT_FIELD(iw_flow_stats.mtu_drop_packets);
   printf( "\nTX Queue Drop Packets:   ");
   PRINT_STAT_FIELD(iw_flow_stats.tx_queue_drop_packets);
   printf("\n\n");
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
   printf ("\n%s Device statistics of device %d (%x)\n",
           type,(device_handle & 0x000000ff),device_handle);
   printf ("============================================\n");
   
   switch (device)
   {
      case WP_DEVICE_ENET:
         memset( &stats_hs_enet, 0, sizeof(WP_stats_enet) );
         if((status = WP_DeviceStatistics(device_handle, &stats_hs_enet)))
         {
            printf("GetStatistics error\n");
            return 0;
         }
         printf("\nStatistics for HS ENET device:\n");
         temp1 = stats_hs_enet.rx_packets;
         temp2 = stats_hs_enet.rx_packets >> 32;
         printf("rx_packets:           %.8x%.8x\n", temp2,temp1);
         temp1 = stats_hs_enet.tx_packets;
         temp2 = stats_hs_enet.tx_packets >> 32;
         printf("tx_packets:           %.8x%.8x\n", temp2,temp1);
         temp1 = stats_hs_enet.rx_err_overrun;
         temp2 = stats_hs_enet.rx_err_overrun >> 32;
         printf("rx_err_overrun:       %.8x%.8x\n", temp2,temp1);
         temp1 = stats_hs_enet.rx_err_sdu;
         temp2 = stats_hs_enet.rx_err_sdu >> 32;
         printf("rx_err_maxsdu:        %.8x%.8x\n", temp2,temp1);
         temp1 = stats_hs_enet.rx_err_nonvalid_mac;
         temp2 = stats_hs_enet.rx_err_nonvalid_mac >> 32;
         printf("rx_err_mac_address:   %.8x%.8x\n", temp2,temp1);
         temp1 = stats_hs_enet.rx_err_mru;
         temp2 = stats_hs_enet.rx_err_mru >> 32;
         printf("rx_err_mru:           %.8x%.8x\n", temp2,temp1);
         temp1 = stats_hs_enet.tx_err_underrun;
         temp2 = stats_hs_enet.tx_err_underrun >> 32;
         printf("rx_err_underrun:      %.8x%.8x\n", temp2,temp1);
         temp1 = stats_hs_enet.rx_err_host_full;
         temp2 = stats_hs_enet.rx_err_host_full >> 32;
         printf("rx_err_host_full:     %.8x%.8x\n", temp2,temp1);
         temp1 = stats_hs_enet.rx_err_fbp_underrun;
         temp2 = stats_hs_enet.rx_err_fbp_underrun >> 32;
         printf("rx_err_fbp_underrun:  %.8x%.8x\n", temp2,temp1);
         temp1 = stats_hs_enet.rx_iw_frames;
         temp2 = stats_hs_enet.rx_iw_frames >> 32;
         printf("rx_iw_frames:         %.8x%.8x\n", temp2,temp1);
         temp1 = stats_hs_enet.tx_frames;
         temp2 = stats_hs_enet.tx_frames >> 32;
         printf("tx_good_frames:       %.8x%.8x\n", temp2,temp1);
         printf("\n");
         temp1 = stats_hs_enet.rx_host_frames;
         temp2 = stats_hs_enet.rx_host_frames >> 32;
         printf("rx_host_frames:       %.8x%.8x\n", temp2,temp1);
         printf("\n");

         return stats_hs_enet.rx_packets;
   }
   return 0;
}

void WTE_DisplayEnetDeviceStats(WP_handle device_handle, WP_CHAR* type)
{
   WP_U32 temp1, temp2;
   WP_stats_enet stats_hs_enet = {0};
   WP_status status;
   printf ("\n%s Device statistics of device %d (%x)\n",
           type,(device_handle & 0x000000ff),device_handle);
   printf ("============================================\n");
   
   memset( &stats_hs_enet, 0, sizeof(WP_stats_enet) );
   if((status = WP_DeviceStatistics(device_handle, &stats_hs_enet)))
   {
      printf("GetStatistics error\n");
      return;
   }
   printf("\nEnet device statistics:\n");
   printf("Hardware statistics:\n");
   temp1 = stats_hs_enet.rx_packets;
   temp2 = stats_hs_enet.rx_packets >> 32;
   printf("rx_packets:           %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.tx_packets;
   temp2 = stats_hs_enet.tx_packets >> 32;
   printf("tx_packets:           %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.txrx_frames_64;
   temp2 = stats_hs_enet.txrx_frames_64 >> 32;
   printf("txrx_frames_64:       %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.txrx_frames_127;
   temp2 = stats_hs_enet.txrx_frames_127 >> 32;
   printf("txrx_frames_127:      %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.txrx_frames_255;
   temp2 = stats_hs_enet.txrx_frames_255 >> 32;
   printf("txrx_frames_255:      %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.txrx_frames_511;
   temp2 = stats_hs_enet.txrx_frames_511 >> 32;
   printf("txrx_frames_511:      %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.txrx_frames_1023;
   temp2 = stats_hs_enet.txrx_frames_1023 >> 32;
   printf("txrx_frames_1023:     %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.txrx_frames_1518;
   temp2 = stats_hs_enet.txrx_frames_1518 >> 32;
   printf("txrx_frames_1518:     %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.txrx_frames_1522;
   temp2 = stats_hs_enet.txrx_frames_1522 >> 32;
   printf("txrx_frames_1522:     %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.rx_bytes;
   temp2 = stats_hs_enet.rx_bytes >> 32;
   printf("rx_bytes:             %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.rx_err_fcs;
   temp2 = stats_hs_enet.rx_err_fcs >> 32;
   printf("rx_err_fcs:           %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.rx_multicast;
   temp2 = stats_hs_enet.rx_multicast >> 32;
   printf("rx_multicast:         %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.rx_broadcast;
   temp2 = stats_hs_enet.rx_broadcast >> 32;
   printf("rx_broadcast:         %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.rx_mac_control;
   temp2 = stats_hs_enet.rx_mac_control >> 32;
   printf("rx_mac_control:       %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.rx_mac_pause;
   temp2 = stats_hs_enet.rx_mac_pause >> 32;
   printf("rx_mac_pause:         %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.rx_mac_unknown;
   temp2 = stats_hs_enet.rx_mac_unknown >> 32;
   printf("rx_mac_unknown:       %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.rx_err_alignment;
   temp2 = stats_hs_enet.rx_err_alignment >> 32;
   printf("rx_err_alignment:     %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.rx_err_length;
   temp2 = stats_hs_enet.rx_err_length >> 32;
   printf("rx_err_length:        %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.rx_err_code;
   temp2 = stats_hs_enet.rx_err_code >> 32;
   printf("rx_err_code:          %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.rx_false_carrier;
   temp2 = stats_hs_enet.rx_false_carrier >> 32;
   printf("rx_false_carrier:     %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.rx_undersize;
   temp2 = stats_hs_enet.rx_undersize >> 32;
   printf("rx_undersize:         %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.rx_oversize;
   temp2 = stats_hs_enet.rx_oversize >> 32;
   printf("rx_oversize:          %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.rx_fragments;
   temp2 = stats_hs_enet.rx_fragments >> 32;
   printf("rx_fragments:         %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.rx_jabber;
   temp2 = stats_hs_enet.rx_jabber >> 32;
   printf("rx_jabber:            %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.rx_dropped;
   temp2 = stats_hs_enet.rx_dropped >> 32;
   printf("rx_dropped:           %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.tx_bytes;
   temp2 = stats_hs_enet.tx_bytes >> 32;
   printf("tx_bytes:             %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.tx_multicast;
   temp2 = stats_hs_enet.tx_multicast >> 32;
   printf("tx_multicast:         %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.tx_broadcast;
   temp2 = stats_hs_enet.tx_broadcast >> 32;
   printf("tx_broadcast:         %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.tx_mac_pause;
   temp2 = stats_hs_enet.tx_mac_pause >> 32;
   printf("tx_mac_pause:         %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.tx_defer;
   temp2 = stats_hs_enet.tx_defer >> 32;
   printf("tx_defer:             %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.tx_excess_defer;
   temp2 = stats_hs_enet.tx_excess_defer >> 32;
   printf("tx_excess_defer:      %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.tx_single_collision;
   temp2 = stats_hs_enet.tx_single_collision >> 32;
   printf("tx_single_collision:  %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.tx_multi_collision;
   temp2 = stats_hs_enet.tx_multi_collision >> 32;
   printf("tx_multi_collision:   %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.tx_late_collision;
   temp2 = stats_hs_enet.tx_late_collision >> 32;
   printf("tx_late_collision:    %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.tx_excess_collision;
   temp2 = stats_hs_enet.tx_excess_collision >> 32;
   printf("tx_excess_collision:  %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.tx_no_collision;
   temp2 = stats_hs_enet.tx_no_collision >> 32;
   printf("tx_no_collision:      %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.tx_mac_pause_honored;
   temp2 = stats_hs_enet.tx_mac_pause_honored >> 32;
   printf("tx_mac_pause_honored: %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.tx_dropped;
   temp2 = stats_hs_enet.tx_dropped >> 32;
   printf("tx_dropped:           %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.tx_jabber;
   temp2 = stats_hs_enet.tx_jabber >> 32;
   printf("tx_jabber:            %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.tx_err_fcs;
   temp2 = stats_hs_enet.tx_err_fcs >> 32;
   printf("tx_err_fcs:           %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.tx_control;
   temp2 = stats_hs_enet.tx_control >> 32;
   printf("tx_control:           %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.tx_oversize;
   temp2 = stats_hs_enet.tx_oversize >> 32;
   printf("tx_oversize:          %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.tx_undersize;
   temp2 = stats_hs_enet.tx_undersize >> 32;
   printf("tx_undersize:         %.8x%.8x\n", temp2,temp1);

   temp1 = stats_hs_enet.tx_fragments;
   temp2 = stats_hs_enet.tx_fragments >> 32;
   printf("tx_fragments:         %.8x%.8x\n", temp2,temp1);

   printf("DPS statistics:\n");
   temp1 = stats_hs_enet.rx_err_overrun;
   temp2 = stats_hs_enet.rx_err_overrun >> 32;
   printf("rx_err_overrun:       %.8x%.8x\n", temp2,temp1);
   temp1 = stats_hs_enet.rx_err_sdu;
   temp2 = stats_hs_enet.rx_err_sdu >> 32;
   printf("rx_err_maxsdu:        %.8x%.8x\n", temp2,temp1);
   temp1 = stats_hs_enet.rx_err_nonvalid_mac;
   temp2 = stats_hs_enet.rx_err_nonvalid_mac >> 32;
   printf("rx_err_mac_address:   %.8x%.8x\n", temp2,temp1);
   temp1 = stats_hs_enet.rx_err_mru;
   temp2 = stats_hs_enet.rx_err_mru >> 32;
   printf("rx_err_mru:           %.8x%.8x\n", temp2,temp1);
   temp1 = stats_hs_enet.tx_err_underrun;
   temp2 = stats_hs_enet.tx_err_underrun >> 32;
   printf("tx_err_underrun:      %.8x%.8x\n", temp2,temp1);
   temp1 = stats_hs_enet.rx_err_host_full;
   temp2 = stats_hs_enet.rx_err_host_full >> 32;
   printf("rx_err_host_full:     %.8x%.8x\n", temp2,temp1);
   temp1 = stats_hs_enet.rx_err_fbp_underrun;
   temp2 = stats_hs_enet.rx_err_fbp_underrun >> 32;
   printf("rx_err_fbp_underrun:  %.8x%.8x\n", temp2,temp1);
   temp1 = stats_hs_enet.rx_iw_frames;
   temp2 = stats_hs_enet.rx_iw_frames >> 32;
   printf("rx_iw_frames:         %.8x%.8x\n", temp2,temp1);
   temp1 = stats_hs_enet.tx_frames;
   temp2 = stats_hs_enet.tx_frames >> 32;
   printf("tx_good_frames:       %.8x%.8x\n", temp2,temp1);
   temp1 = stats_hs_enet.rx_host_frames;
   temp2 = stats_hs_enet.rx_host_frames >> 32;
   printf("rx_host_frames:       %.8x%.8x\n", temp2,temp1);
   printf("\n");

   printf("Serial DPS IF statistics:\n");
   temp1 = stats_hs_enet.serial_dps_if_stats.tx_frames;
   temp2 = stats_hs_enet.serial_dps_if_stats.tx_frames >> 32;
   printf("Serial DPS IF tx frames            %.8x%.8x\n", temp2,temp1);
   temp1 = stats_hs_enet.serial_dps_if_stats.tx_bytes;
   temp2 = stats_hs_enet.serial_dps_if_stats.tx_bytes >> 32;
   printf("Serial DPS IF tx bytes             %.8x%.8x\n", temp2,temp1);
   temp1 = stats_hs_enet.serial_dps_if_stats.rx_frames;
   temp2 = stats_hs_enet.serial_dps_if_stats.rx_frames >> 32;
   printf("Serial DPS IF rx frames            %.8x%.8x\n", temp2,temp1);
   temp1 = stats_hs_enet.serial_dps_if_stats.rx_bytes;
   temp2 = stats_hs_enet.serial_dps_if_stats.rx_bytes >> 32;
   printf("Serial DPS IF rx bytes             %.8x%.8x\n", temp2,temp1);
   temp1 = stats_hs_enet.serial_dps_if_stats.rx_pce_denied_frames;
   temp2 = stats_hs_enet.serial_dps_if_stats.rx_pce_denied_frames >> 32;
   printf("Serial DPS IF rx pce denied frames %.8x%.8x\n", temp2,temp1);
   temp1 = stats_hs_enet.serial_dps_if_stats.rx_dropped_frames;
   temp2 = stats_hs_enet.serial_dps_if_stats.rx_dropped_frames >> 32;
   printf("Serial DPS IF rx dropped frames    %.8x%.8x\n", temp2,temp1);
   printf("\n");
}


static WP_U8 WPE_DataUnitReceive(WP_handle rx_channel_handle, WP_U8 *hexa_ret)
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
   data_unit.type = WP_DATA_ENET;

   tx_buf.data = NULL;

   for (ii = 0; ii < 1000; ++ii)
   {
      status = WP_HostReceive (rx_channel_handle, &data_unit);

      if (WP_ERROR(status) == WP_ERR_HST_NO_DATA_TO_GET) /* didn't get yet? */
      {
         WP_Delay(1500);
      }
      else if(WP_OK != status)
      {
         result = FALSE;
         terminate_on_error(status,"WP_HostReceive()");
         break;
      }
      else
      {
         result = TRUE;
         curr_buff_ptr = data_unit.segment;
         rx_counter += data_unit.n_active;

         /* print segment */
         APP_PRINTF("recieved %d segments, %d long each\n", data_unit.n_active, curr_buff_ptr->data_size);
         WP_MEM_BYTES_GET(((WP_U8 *)hexa_buffer),curr_buff_ptr->data,curr_buff_ptr->data_size);
//         APP_PRINTF("after mem set\n");
         for (j = 0; j < data_unit.n_active; ++j)
         {
            for(k=0;k<curr_buff_ptr->data_size;++k)
            {
               hexa_ret[k] = (WP_U8)(hexa_buffer[k]);
               APP_PRINTF("%2.2x ", (WP_U8)hexa_buffer[k]);
//               if ((k % 12) == 11)
//                  APP_PRINTF("\n");
            }
//            APP_PRINTF("\n\n");

            WP_PoolFree(curr_buff_ptr->pool_handle, curr_buff_ptr->data);
            APP_PRINTF("next = %d \n", (WP_U32)curr_buff_ptr->next);
            curr_buff_ptr = curr_buff_ptr->next;
         }
         if(rx_counter == 1)
            break;
         else
         {
            WP_Delay(100000);
         }
      }
   }

   if(ii >= 1000)
   {
      result = FALSE;
   }
   else
   {
//      APP_PRINTF("Received packet.\n");
   }
   return result;
}


/****************************************************/
/* This function is used to receive in GCSI tests   */
/****************************************************/
// WPE_DataUnitReceiveGCSI function implementation put in comment because currently it is not used 
// static WP_U8 WPE_DataUnitReceiveGCSI(WP_handle rx_channel_handle, WP_U8 *hexa_ret, WP_U16* PacketTxSize)
// {
//    WP_status status;
//    WP_data_segment tx_buf;
//    WP_data_unit data_unit;
//    WP_U32 ii, j;
//    WP_U8 *data_ptr=NULL;
//    WP_U8 result = TRUE;

//    WP_data_segment *curr_buff_ptr=NULL;
//    WP_U32 rx_counter=0;
//    WP_CHAR hexa_buffer[DATA_LENGTH*2];
//    WP_U32 k;

//    memset(&data_unit, 0, sizeof( data_unit));
//    memset(&tx_buf, 0, sizeof( tx_buf));

//    data_unit.data_size = *PacketTxSize; //liorbaz DATA_LENGTH;
//    data_unit.n_active = 1;

//    tx_buf.data_size = data_unit.data_size;
//    /* data_ptr = WP_PoolAlloc(data_pool_handle); */
//    tx_buf.data = data_ptr;
//    /* tx_buf.pool_handle = data_pool_handle; */
//    tx_buf.next = NULL;
//    tx_buf.displacement = 0;

//    /* now moving to receive */
//    data_unit.segment = (&tx_buf);
//    data_unit.n_segments = 1;
//    data_unit.n_active = 0;
// //   data_unit.type = WP_DATA_IW;
//    data_unit.type = WP_DATA_ENET;

//    tx_buf.data = NULL;

//    for (ii = 0; ii < 1; ++ii)
//    {
//       status = WP_HostReceive (rx_channel_handle, &data_unit);

//       if (WP_ERROR(status) == WP_ERR_HST_NO_DATA_TO_GET) /* didn't get yet? */
//       {
//          printf("Delay0[%d]:\n", ii );
//          WP_Delay(200000);
//       }
//       else if(WP_OK != status)
//       {
//          result = FALSE;
//          terminate_on_error(status,"WP_HostReceive()");
//          break;
//       }
//       else
//       {
//          result = TRUE;
//          curr_buff_ptr = data_unit.segment;
//          rx_counter += data_unit.n_active;

// #ifdef DEBUG_PRINT
//          /* print segment */
//          printf("received %d segments, %d long each\n", data_unit.n_active, curr_buff_ptr->data_size);
// #endif

//          *PacketTxSize = curr_buff_ptr->data_size; // Fix the received data length
//          WP_MEM_BYTES_GET(((WP_U8 *)hexa_buffer),curr_buff_ptr->data,curr_buff_ptr->data_size);

// #ifdef DEBUG_PRINT
//          printf("after mem set\n");
// #endif
//          for (j = 0; j < data_unit.n_active; ++j)
//          {
//             for(k=0;k<curr_buff_ptr->data_size;++k)
//             {
//                hexa_ret[k] = (WP_U8)(hexa_buffer[k]);
// #ifdef DEBUG_PRINT
//                printf("%2.2x", (WP_U8)hexa_buffer[k]);
//                if ((k % 12) == 11)
//                   printf("\n");
// #endif
//             }
// #ifdef DEBUG_PRINT
//             printf("\n\n");
// #endif

//             WP_PoolFree(curr_buff_ptr->pool_handle, curr_buff_ptr->data);
// #ifdef DEBUG_PRINT
//             printf("next = %d \n", (WP_U32)curr_buff_ptr->next);
// #endif
//             curr_buff_ptr = curr_buff_ptr->next;
//          }
//          if(rx_counter == 1)
//             break;
//          else
//          {
//             WP_Delay(100000);
//             printf("Delay3[%d]:\n", ii );//liorbaz
//          }
//       }
//    }

//    if(ii == 20000)
//    {
//       result = FALSE;
//    }
// #ifdef DEBUG_PRINT
//    else
//    {
//       printf("Received packet result = %s:\n",
//              (result != FALSE ? "OK" : "NOT OK !"));
//    }
// #endif
//    return result;
// }

// WPE_DataUnitReceiveOnly function implementation put in comment because currently it is not used 
// static WP_U8 WPE_DataUnitReceiveOnly(const WP_CHAR* mac_dst, const WP_CHAR* mac_src, const WP_CHAR* svlan, const WP_CHAR* cvlan, 
//                                      const WP_CHAR* ip_dst, const WP_CHAR* ip_src, WP_handle rx_channel_handle)
// {
//    WP_U8 hexa_ret[2000];
//    static WP_U32 packet_counter=0;
//    WP_U8 result;

//    packet_counter++;
//    APP_PRINTF("\nRECV: Packet no %i. \n MAC DA %s, MAC SA %s, VLAN TAG %s, IP SRC %s, IP DST %s",
//       packet_counter, mac_dst, mac_src, svlan, ip_src, ip_dst);

//    if(cvlan[0] != 0)
//       APP_PRINTF(" CVLAN TAG %s", cvlan);

//    APP_PRINTF("\n Receiving to channel %X ...\n", rx_channel_handle);

//    result = WPE_DataUnitReceive(rx_channel_handle, &(hexa_ret[0]));
//    APP_PRINTF( "RECV: Packet no %i. Received packet result is %s:\n ", packet_counter, (result != FALSE ? "OK" : "NOT OK !"));

//    if(result == TRUE)
//    {
//       WP_CHAR ch_byte[3];
//       WP_U32 i, byte;
//       APP_PRINTF("Checking the packet:\n   expected dst mac: 0x%s \n            dst mac: 0x%02X%02X%02X%02X%02X%02X \n",
//          mac_dst, hexa_ret[0], hexa_ret[1], hexa_ret[2], hexa_ret[3], hexa_ret[4], hexa_ret[5]);
//       for(i = 0; i<6; i++)
//       {
//          ch_byte[0] = mac_dst[i*2];
//          ch_byte[1] = mac_dst[i*2+1];
//          ch_byte[2] = 0;
//          sscanf(ch_byte, "%02X", &byte);
//          if (hexa_ret[i] != byte)
//          {
//             printf("The packet is wrong\n");
//             result = FALSE;
//             break;
//          }
//       }
//       printf("RECV: Packet no %d is received.\n", packet_counter);
//    }

//    return result;
// }

static void WPE_DataUnitSendToEnet(const WP_CHAR* mac_dst, const WP_CHAR* mac_src,
                                   const WP_CHAR* svlan, const WP_CHAR* cvlan, 
                                   const WP_CHAR* ip_dst, const WP_CHAR* ip_src,
                                   WP_handle tx_channel_handle)
{
   WP_status status;
   WP_U8 *km_data = NULL;
   WP_CHAR ascii_buffer[DATA_LENGTH*2+1];
   WP_U8 hex_buffer[DATA_LENGTH*2];
   WP_data_segment data_segment;
   WP_data_unit data_unit;
   WP_U16 checksum;
   WP_U32 ip_start;
   WP_handle iw_pool;

   if(tx_channel_handle == tx_channel_enet_handle1)
      iw_pool = iw_pool1;
   else if(tx_channel_handle == tx_channel_enet_handle2)
      iw_pool = iw_pool2;
   #if 0
   else if(tx_channel_handle == tx_channel_enet_handle3)
      iw_pool = iw_pool3;
   else if(tx_channel_handle == tx_channel_enet_handle4)
      iw_pool = iw_pool4;
   #endif
   else if(tx_channel_handle == tx_host_handle11)
      iw_pool = iw_pool5;
   else
   {
      printf("ERROR tx_channel_handle no match. Not sending to enet.");
      return;
   }
   

   memset (hex_buffer, 0, DATA_LENGTH*2);
   memset (ascii_buffer, 0, DATA_LENGTH*2+1);
   memset( &data_unit, 0, sizeof( data_unit));
   memset( &data_segment, 0, sizeof( data_segment));
   km_data = WP_PoolAlloc(iw_pool);

   data_unit.data_size = 60;/*DATA_LENGTH;*/
   data_unit.n_active = 1;

   data_segment.data_size = data_unit.data_size;
   data_segment.data = km_data;
   data_segment.pool_handle = iw_pool;
   data_segment.next = NULL;
   data_segment.displacement = 0;

   data_unit.n_segments = 1;
   data_unit.type = WP_DATA_IW;
   data_unit.control = 0;

   data_unit.segment = (&data_segment);
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
   strcat (ascii_buffer, "0800");/* IP TYPE */
   strcat (ascii_buffer, "45040044");/* tos = 4 */
   strcat (ascii_buffer, "001b0000");
   strcat (ascii_buffer, "0a110000");/* protocol = 0x11, empty checksum */
   strcat (ascii_buffer, ip_src);/* IP src address */
   strcat (ascii_buffer, ip_dst);/* IP src address */
   strcat (ascii_buffer, "00190015");
   strcat (ascii_buffer, "00280000");/* length, UDP with no checksum */
   if(svlan[0] == 0)
   {/* add 4 bytes instead of CVLAN TAG to get the required packet size" */
      strcat (ascii_buffer, "12345678");
   }
   if(cvlan[0] == 0)
   {/* add 4 bytes instead of CVLAN TAG to get the required packet size" */
      strcat (ascii_buffer, "12345678");
   }
   strcat (ascii_buffer,
           "00a4c5c43a528a3f0123");

   APP_PRINTF( "transmitted data length: %d\n", strlen(ascii_buffer)/2);
   App_TranslateAsciiToHex(hex_buffer, ascii_buffer, data_unit.data_size);

   checksum = WTE_IpChecksumCalc(hex_buffer + ip_start, 20);

   memcpy (hex_buffer + ip_start + 10, &checksum, 2);

   WP_MEM_BYTES_FILL(km_data, '0', iw_buffer_data->size);
   WP_MEM_BYTES_SET(km_data, hex_buffer, data_unit.data_size);

   APP_PRINTF( "data_unit.segment->data: 0x%x\n", (WP_U32)data_unit.segment->data);

   status = WP_HostSend(tx_channel_handle, &data_unit);
   terminate_on_error(status,"WP_HostSend");

   return;
}

// WPE_DataUnitSendOnly function implementation put in comment because currently it is not used
// static void WPE_DataUnitSendOnly(const WP_CHAR* mac_dst, const WP_CHAR* mac_src, const WP_CHAR* svlan, const WP_CHAR* cvlan, 
//                                  const WP_CHAR* ip_dst, const WP_CHAR* ip_src, WP_handle tx_channel_handle)
// {
//    static WP_U32 packet_counter = 0;
//    packet_counter++;
//    APP_PRINTF("\nSEND: Packet no %i. \n MAC DA %s, MAC SA %s, SVLAN TAG %s, IP SRC %s, IP DST %s",
//       packet_counter, mac_dst, mac_src, svlan, ip_src, ip_dst);

//    if(cvlan[0] != 0)
//       APP_PRINTF(" CVLAN TAG %s", cvlan);

//    APP_PRINTF("\n Sending to Enet %X ...\n", tx_channel_handle);

//    WPE_DataUnitSendToEnet(mac_dst, mac_src, svlan, cvlan, ip_dst, ip_src, tx_channel_handle);
//    printf("SEND: Packet no %d is sent.\n", packet_counter);
// }

static WP_U8 WPE_DataUnitSendReceive(const WP_CHAR* mac_dst, const WP_CHAR* mac_src, const WP_CHAR* svlan, const WP_CHAR* cvlan,
                                     const WP_CHAR* ip_dst, const WP_CHAR* ip_src,
                                     WP_handle tx_channel_handle, WP_handle rx_channel_handle)
{
   WP_U8 hexa_ret[2000];
   static WP_U32 packet_counter=0;
   WP_U8 result;
   packet_counter++;
   APP_PRINTF("\nSEND: Packet no %i. \n\tMAC DA:    %s \n\tMAC SA:    %s \n\tSVLAN TAG: %s \n\tIP SRC:    %s \n\tIP DST:    %s",
      packet_counter, mac_dst, mac_src, svlan, ip_src, ip_dst);

   if(cvlan[0] != 0)
      APP_PRINTF("\n\tCVLAN TAG: %s", cvlan);

   APP_PRINTF("\nSending to Enet %X ...\n", tx_channel_handle);
	  
   WPE_DataUnitSendToEnet(mac_dst, mac_src, svlan, cvlan, ip_dst, ip_src, tx_channel_handle);
   printf("\nSENDRECV: Packet no %d was sent.\n", packet_counter);
//   APP_PRINTF("SENDRECV: Pause for about 0.1 seconds ...\n");
   WP_Delay(100000);

   APP_PRINTF( "SENDRECV: Packet no %i. Receiving ...\n", packet_counter);
   result = WPE_DataUnitReceive(rx_channel_handle, &(hexa_ret[0]));
   APP_PRINTF( "SENDRECV: Packet no %i. Receive Result is %s\n\n", packet_counter, (result != FALSE ? "OK" : "NOT OK !"));
   if(result == TRUE)
   {
      WP_CHAR ch_byte[3];
      WP_U32 i, byte;
      APP_PRINTF("Checking the packet:\n   expected dst mac: 0x%s \n            dst mac: 0x%02X%02X%02X%02X%02X%02X \n",
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
      printf("SENDRECV: Packet no %d was received.\n", packet_counter);
   }

   return result;
}

// WPE_PacketSendOnly function implementation put in comment because currently it is not used 
// static void WPE_PacketSendOnly(WPE_pce_packet *packet, WP_handle tx_channel_handle)
// {
//    WPE_DataUnitSendOnly(
//       packet->mac_da, packet->mac_sa,
//       packet->vlan_stag, packet->vlan_ctag,
//       packet->ipv4_da, packet->ipv4_sa,
//       tx_channel_handle);
// }

// WPE_PacketReceiveOnly function implementation put in comment because currently it is not used 
// static WP_U8 WPE_PacketReceiveOnly(WPE_pce_packet *packet, WP_handle rx_channel_handle)
// {
//    return WPE_DataUnitReceiveOnly(
//       packet->mac_da, packet->mac_sa, 
//       packet->vlan_stag, packet->vlan_ctag,
//       packet->ipv4_da, packet->ipv4_sa,
//       rx_channel_handle);
// }

// WPE_PacketSendReceive function implementation put in comment because currently it is not used 
// static WP_U8 WPE_PacketSendReceive(WPE_pce_packet *packet, WP_handle tx_channel_handle, WP_handle rx_channel_handle)
// {
//    return WPE_DataUnitSendReceive(
//       packet->mac_da, packet->mac_sa,
//       packet->vlan_stag, packet->vlan_ctag,
// 	  packet->ipv4_da, packet->ipv4_sa,
//       tx_channel_handle, rx_channel_handle);
// }

// WPE_PrintPceGlobalStats function implementation put in comment because currently it is not used 
// static void WPE_PrintPceGlobalStats(void)
// {
//    WP_U32 temp1, temp2;
//    WP_PceGlobalStatistics(WP_WINPATH(0), &pce_global_stats);

//    printf("\n\nPCE Global Statistics\n");

//    temp1 = pce_global_stats.parser_header_access_errors;
//    temp2 = pce_global_stats.parser_header_access_errors >> 32;
//    printf("parser_header_access_errors %08X %08X\n", temp2, temp1);

//    temp1 = pce_global_stats.parser_timeout_err;
//    temp2 = pce_global_stats.parser_timeout_err >> 32;
//    printf("parser_timeout_err          %08X %08X\n", temp2, temp1);

//    temp1 = pce_global_stats.classifier_timeout_err;
//    temp2 = pce_global_stats.classifier_timeout_err >> 32;
//    printf("classifier_timeout_err      %08X %08X\n", temp2, temp1);

//    temp1 = pce_global_stats.parser_accept;
//    temp2 = pce_global_stats.parser_accept >> 32;
//    printf("parser_accept               %08X %08X\n", temp2, temp1);

//    printf("\n\n");
// }

static void WPE_PrintPceFilterStats(WP_handle filter)
{
   WP_U32 index;
   WP_U32 temp1, temp2;

   index = filter & 0xFF;

   WP_PceFilterStatistics (filter, &(pce_filter_stats[index]));

   printf("PCE Filter %d Statistics:\n", index);

   temp1 = pce_filter_stats[index].denied_pkts;
   temp2 = pce_filter_stats[index].denied_pkts >> 32;
   if (temp1 || temp2)
      printf("denied_pkts            %08X %08X\n", temp2, temp1);

   temp1 = pce_filter_stats[index].matched_pkts;
   temp2 = pce_filter_stats[index].matched_pkts >> 32;
   if (temp1 || temp2)
      printf("matched_pkts           %08X %08X\n", temp2, temp1);

   temp1 = pce_filter_stats[index].no_matched_pkts;
   temp2 = pce_filter_stats[index].no_matched_pkts >> 32;
   if (temp1 || temp2)
      printf("no_matched_pkts        %08X %08X\n", temp2, temp1);
}

static void WPE_PrintPolicerStatistics(void)
{
   WP_status status_ingress;
   //WP_U32 idx =0;
   WP_stats_policer_v2 policer_stats;

   memset(&policer_stats,0,sizeof(policer_stats));
   
   printf("\n********************POLICER STATISTICS*****************************");    
   //for(idx=0;idx<MAX_IW_POLICERS_INGRESS;idx++)
   {
      status_ingress = WP_PolicerStatistics(policer_handle_ingress[0], &policer_stats);
      printf("\ncbs- \t %08x%08x\t  ",
                        ((WT_StatField*)&policer_stats.non_conforming_cbs)->part.high,
                        ((WT_StatField*)&policer_stats.non_conforming_cbs)->part.low);
      printf("\nebs- \t %08x%08x\t  ",
                        ((WT_StatField*)&policer_stats.non_conforming_ebs)->part.high,
                        ((WT_StatField*)&policer_stats.non_conforming_ebs)->part.low);      
#ifdef DEBUG_PRINT
      terminate_on_error(status_ingress, "WP_PolicerStatistics()");
#endif   
   printf("\n===============================================================================\n");      
   }
}


static void WPE_ResetPolicerStatistics(void)
{
   WP_PolicerStatisticsReset(policer_handle_ingress[0]);
}
// WPE_ResetPceFilterStats function implementation put in comment because currently it is not used 
// static void WPE_ResetPceFilterStats(WP_handle filter)
// {
//    WP_U32 index;

//    index = filter & 0xFF;

//    WP_PceFilterStatisticsReset (filter, &(pce_filter_stats[index]));
// }

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
   WP_PORT_ENET7,
   WP_PORT_ENET8,
   WP_PORT_ENET13,
   WP_PORT_ENET14,
   WP_PORT_ENET15,
   WP_PORT_ENET16,
   WP_PORT_ENET3,
   WP_PORT_ENET4,
   WP_PORT_ENET5,
   WP_PORT_ENET6
};

// WT_Create_pools function implementation put in comment because currently it is not used 
// static void WT_Create_pools(void)
// {
//    WP_handle iw_pool_handle;
//    iw_buffer_data->n_buffers = 1000;
//    iw_pool = iw_pool_handle = WP_PoolCreate(WP_WINPATH(0), WP_pool_iwbuffer, iw_buffer_data);
//    terminate_on_error(iw_pool_handle, "WP_PoolCreate 4");

//    /* Create an interworking queue node */   
//    qn_iw->adjunct_pool = iw_pool_handle;
//    qniw_enet = WP_QNodeCreate(WP_WINPATH(0), WP_QNODE_IWQ | WP_QNODE_OPT_FMU, qn_iw);
// }

// WT_Init function implementation put in comment because currently it is not used 
// static WP_U8 WT_Init(void)
// {
//    WP_U16 i;
//    for(i=0; i<WT_MAX_INTERFACES; i++)
//       global_interfaces[i].status = WT_STATUS_UNINITIALIZED;

//    global_info.interfaces = &(global_interfaces[0]);

//    WT_Create_pools();

//    wt_temp_host_port = WP_PortCreate(WP_WINPATH(0), WP_PORT_IW_HOST, NULL);
//    terminate_on_error(wt_temp_host_port, "Temp Host Port Create");

//    wt_temp_host_device = WP_DeviceCreate(wt_temp_host_port, WP_PHY(0), WP_DEVICE_IW_HOST, NULL);
//    terminate_on_error(wt_temp_host_device, "Temp Host Device Create");   

//    return WP_OK;
// }

// WT_CreateEnetPort function implementation put in comment because currently it is not used 
// static WP_handle WT_CreateEnetPort(WP_U8 physical_port_number, WP_U8 loopback_mode)
// {
//    WP_handle port_handle;

//    if(physical_port_number > WT_MAX_ENETS)
//       return WPE_ERROR;

//    enet_s->interface_mode = WP_ENET_SGMII_1000;

//    status = WPX_BoardSerdesInit(WP_WINPATH(0), WT_available_wnets[physical_port_number-1], loopback_mode);
//    terminate_on_error(status, "WPX_BoardSerdesSetLoopback()");      

//    port_handle = WP_PortCreate(WP_WINPATH(0), WT_available_wnets[physical_port_number-1], enet_s);
//    terminate_on_error(port_handle, "WP_PortCreate Enet");

//    return port_handle;
// }

// WT_CreateEnetDevice function implementation put in comment because currently it is not used 
// static WP_handle WT_CreateEnetDevice(WP_handle port_handle, WP_U8 loopback_mode)
// {
//    WP_status status;
//    WP_handle device_handle;

//    enet_dev->loopbackmode = WP_ENET_NORMAL;
//    enet_dev->extended_params = NULL;

//    device_handle = WP_DeviceCreate(port_handle, WP_PHY(0), WP_DEVICE_ENET, enet_dev);
//    terminate_on_error(device_handle, "WP_DeviceCreate Enet");

//    enet_dev->extended_params = enet_dev_ex;
//    status = WP_DeviceModify(device_handle,
//                             WP_DEV_MOD_ENET_PHYSTATMODE |
//                             WP_DEV_MOD_ENET_TX_STATMODE |
//                             WP_DEV_MOD_ENET_RX_STATMODE,
//                             &enet_dev);
//    terminate_on_error(status, "WP_DeviceModify() ENET 1");

//    return device_handle;
// }

// WT_CreateEnetChannelsAndTxBindings function implementation put in comment because currently it is not used 
// static void WT_CreateEnetChannelsAndTxBindings(WP_U8 if_number)
// {
//    WP_U8 i, tx_channels_number;
//    WP_handle device;
//    WP_status status;

//    device = global_interfaces[if_number].device;
//    tx_channels_number = global_interfaces[if_number].tx_channels_number;

//    for(i=0;i<1;i++)
//    {
//       ch_config_enet[0].iwmode = WP_PKTCH_IWM_ENABLE;
//       global_interfaces[if_number].tx_channels[i] = WP_ChannelCreate(WT_TX_CHANNEL_TAG + if_number, device, qniw_enet, WP_CH_TX, WP_ENET, &ch_config_enet[0]);
//       terminate_on_error(global_interfaces[if_number].tx_channels[i], "WP_ChannelCreate() Enet TX");

//       status = WP_IwTxBindingCreate(global_interfaces[if_number].tx_channels[i], WP_IW_TX_BINDING, tx_binding_cfg);
//       terminate_on_error(status, "WP_IwTxBindingCreate()");
//    }
//    global_interfaces[if_number].tx_channel = global_interfaces[if_number].tx_channels[0];

//    ch_config_enet[0].iwmode = WP_PKTCH_IWM_ENABLE;
//    global_interfaces[if_number].rx_channel = WP_ChannelCreate(WT_RX_CHANNEL_TAG + if_number, device, qniw_enet, WP_CH_RX, WP_ENET, &ch_config_enet[0]);
//    terminate_on_error(global_interfaces[if_number].rx_channel, "WP_ChannelCreate() Enet RX");
// }

// WT_CreateInterface function implementation put in comment because currently it is not used 
// static WP_U8 WT_CreateInterface(WP_U8 if_number, WT_PhysicalPortType type, WP_U8 physical_port_number,
//                                WP_U8 loopback_mode, WP_U8 tx_channels_number,
//                                WP_U8 default_flow_agg_number, WP_U16 flow_agg_vlan_id)
// {
//    if(if_number > WT_MAX_INTERFACES)
//       return WPE_ERROR;

//    if(global_interfaces[if_number].status != WT_STATUS_UNINITIALIZED)
//       return WPE_ERROR;

//    if(loopback_mode != WP_TRUE && loopback_mode != WP_FALSE)
//       return WPE_ERROR;

//    global_interfaces[if_number].status = WT_STATUS_INITIALIZED;
//    global_interfaces[if_number].type = type;
//    global_interfaces[if_number].default_flow_agg_number = default_flow_agg_number;
//    global_interfaces[if_number].flow_agg_vlan_id = flow_agg_vlan_id;

//    switch(type)
//    {
//    case WT_PHPORT_TYPE_SGMII:
//       global_interfaces[if_number].port = WT_CreateEnetPort(physical_port_number, loopback_mode);
//       global_interfaces[if_number].device = WT_CreateEnetDevice(global_interfaces[if_number].port, loopback_mode);
//       if(tx_channels_number > WT_MAX_IF_ENET_TX_CHANNELS)
//          return WPE_ERROR;
//       global_interfaces[if_number].tx_channels_number = tx_channels_number;
//       break;
//    case WT_PHPORT_TYPE_UPI:
//       break;
//    case WT_PHPORT_TYPE_XGI:
//       break;
//    case WT_PHPORT_TYPE_HOST:
//       global_interfaces[if_number].port = WP_PortCreate(WP_WINPATH(0), WP_PORT_IW_HOST, NULL);
//       terminate_on_error(global_interfaces[if_number].port, "Host Port Create");
//       global_interfaces[if_number].device = WP_DeviceCreate(global_interfaces[if_number].port, WP_PHY(0), WP_DEVICE_IW_HOST, NULL);
//       terminate_on_error(global_interfaces[if_number].device, "Host Device Create");   
//       break;
//    default:
//       return WPE_ERROR;
//    }

//    /* create iw port */

//    return WPE_OK;
// }

// WT_CreateChannelsAndTxBindings function implementation put in comment because currently it is not used 
// static void WT_CreateChannelsAndTxBindings(WP_U16 if_number)
// {
//    switch(global_interfaces[if_number].type)
//    {
//    case WT_PHPORT_TYPE_SGMII:
//       WT_CreateEnetChannelsAndTxBindings(if_number);
//       break;
//    case WT_PHPORT_TYPE_UPI:
//       break;
//    case WT_PHPORT_TYPE_XGI:
//       break;
//    case WT_PHPORT_TYPE_HOST:
//       global_interfaces[if_number].rx_channel = WP_ChannelCreate(WT_RX_CHANNEL_TAG, wt_temp_host_device, qniw_enet, WP_CH_RX, WP_IW_HOST, &iw_rx_ch_cfg);
//       terminate_on_error(global_interfaces[if_number].rx_channel, "Host RX Channel Create(iw)");
//       break;
//    default:
//       break;
//    }
// }

// WT_CreateFlowAggregationsAndBports function implementation put in comment because currently it is not used 
// static void WT_CreateFlowAggregationsAndBports(void)
// {
//    WP_U16 i;
//    WP_status status;

//    APP_PRINTF("Creating Temp Flow aggregation ...\n");
//    tx_agg_bridging->txfunc = wt_temp_host_rx_channel;
//    wt_temp_flow_aggregation = WP_IwFlowAggregationCreate(WP_WINPATH(0), WP_IW_VLAN_AWARE_BRIDGING_MODE, &tx_agg_bridging);
//    terminate_on_error(wt_temp_flow_aggregation,"WP_IwFlowAggregationCreate()");

//    for(i=0; i<WT_MAX_INTERFACES; i++)
//    {
//       if(global_interfaces[i].status == WT_STATUS_INITIALIZED)
//       {
//          switch(global_info.iw_system_type)
//          {
//          case WP_IW_ROUTING_MODE:
//             route_port_cfg.flow_agg = wt_temp_flow_aggregation;
//             route_port_cfg.tag = WT_IW_PORT_TAG+i;
//             global_interfaces[i].iw_port = WP_IwPortCreate(global_info.iw_system, &route_port_cfg);
//             terminate_on_error(global_interfaces[i].iw_port, "WP_IwPortCreate() Routing");
//             break;
//          case WP_IW_TRANS_BRIDGING_MODE:
//          case WP_IW_VLAN_AWARE_BRIDGING_MODE:
//             bport_conf->flow_agg = wt_temp_flow_aggregation;
//             bport_conf->tag = WT_IW_PORT_TAG+i;
//             global_interfaces[i].iw_port = WP_IwBridgingPortCreate(global_info.iw_system, bport_conf);
//             terminate_on_error(global_interfaces[i].iw_port, "WP_IwBridgingPortCreate() ");
//             break;
//          default:
//             break;
//          }

//          switch(global_interfaces[i].type)
//          {
//          case WT_PHPORT_TYPE_SGMII:
//          case WT_PHPORT_TYPE_UPI:
//          case WT_PHPORT_TYPE_XGI:
//             if(global_interfaces[i].flow_agg_vlan_id == 0)
//             {
//                flow_agg_gen[0].iw_port = global_interfaces[i].iw_port;
//                flow_agg_gen[0].txfunc = global_interfaces[i].tx_channel;
//                global_interfaces[i].flow_aggregation = WP_IwFlowAggregationCreate(WP_WINPATH(0),
//                                                             WP_IW_GENERIC_MODE,
//                                                             &flow_agg_gen);
//                terminate_on_error(global_interfaces[i].flow_aggregation, "WP_IwFlowAggregationCreate() Generic");
//             }
//             else
//             {
//                tx_agg_bridging->iw_port = global_interfaces[i].iw_port;
//                tx_agg_bridging->txfunc = global_interfaces[i].tx_channel;
//                tx_agg_bridging->replace_vlan_id = WP_IW_REPLACE_VLAN_ID;
//                tx_agg_bridging->vlan_id = global_interfaces[i].flow_agg_vlan_id;
//                global_interfaces[i].flow_aggregation = WP_IwFlowAggregationCreate(WP_WINPATH(0), WP_IW_VLAN_AWARE_BRIDGING_MODE, &tx_agg_bridging);
//                terminate_on_error(global_interfaces[i].flow_aggregation ,"WP_IwFlowAggregationCreate() VLAN ID replaced");
//             }
//             break;
//          case WT_PHPORT_TYPE_HOST:
//             if(global_interfaces[i].flow_agg_vlan_id != 0)
//             {
//                tx_agg_bridging->replace_vlan_id = WP_IW_REPLACE_VLAN_ID;
//                tx_agg_bridging->vlan_id = global_interfaces[i].flow_agg_vlan_id;
//             }
//             tx_agg_bridging->txfunc = global_interfaces[i].rx_channel;
//             global_interfaces[i].flow_aggregation = WP_IwFlowAggregationCreate(WP_WINPATH(0), WP_IW_VLAN_AWARE_BRIDGING_MODE, &tx_agg_bridging);
//             terminate_on_error(global_interfaces[i].flow_aggregation ,"WP_IwFlowAggregationCreate()");
//             break;
//          default:
//             break;
//          }
//       }
//    }

//    for(i=0; i<WT_MAX_INTERFACES; i++)
//    {
//       if(global_interfaces[i].status == WT_STATUS_INITIALIZED &&
//          global_interfaces[i].default_flow_agg_number != 0 &&
//          global_interfaces[global_interfaces[i].default_flow_agg_number].status == WT_STATUS_INITIALIZED)
//       {
//          switch(global_info.iw_system_type)
//          {
//          case WP_IW_ROUTING_MODE:
//             route_port_cfg.flow_agg = global_interfaces[global_interfaces[i].default_flow_agg_number].flow_aggregation;
//             status = WP_IwPortModify(
//                global_interfaces[i].iw_port,
//                WP_IW_PORT_MODIFY_FLOW_AGG,
//                &route_port_cfg);
//             terminate_on_error(status, "WP_IwPortModify() Routing - set the default flow aggregation");
//             break;
//          case WP_IW_TRANS_BRIDGING_MODE:
//          case WP_IW_VLAN_AWARE_BRIDGING_MODE:
//             bport_conf->flow_agg = global_interfaces[global_interfaces[i].default_flow_agg_number].flow_aggregation;
//             status = WP_IwPortModify(
//                global_interfaces[i].iw_port,
//                WP_IW_PORT_MODIFY_FLOW_AGG,
//                &bport_conf);
//             terminate_on_error(status, "WP_IwPortModify() Bridging - set the default flow aggregation");
//             break;
//          default:
//             break;
//          }
//       }
//    }
// }

// WT_CreateIwSystemAndItsComponents function implementation put in comment because currently it is not used 
// static void WT_CreateIwSystemAndItsComponents(WP_iw_system_type type)
// {
//    WP_U16 i;
//    WP_status status;

//    /* Create the temporary host channel */
//    wt_temp_host_rx_channel = WP_ChannelCreate(WT_RX_CHANNEL_TAG, wt_temp_host_device, qniw_enet, WP_CH_RX, WP_IW_HOST, &iw_rx_ch_cfg);
//    terminate_on_error(wt_temp_host_rx_channel, "Temp Host RX Channel Create(iw)");

//    for(i=0;i<WT_MAX_INTERFACES;i++)
//    {
//       if(global_interfaces[i].status == WT_STATUS_INITIALIZED)
//       {
//          WT_CreateChannelsAndTxBindings(i);
//       }
//    }

//    global_info.iw_system_type = type;

//    switch(type)
//    {
//    case WP_IW_ROUTING_MODE:
//       global_info.iw_system = WP_IwSystemCreate(WP_WINPATH(0), WP_IW_ROUTING_MODE, &(iw_sys_one_routing[0]));
//       terminate_on_error(global_info.iw_system, "WP_IwSystemCreate() - Routing");
//       break;
//    case WP_IW_TRANS_BRIDGING_MODE:
//       global_info.iw_system = WP_IwSystemCreate(WP_WINPATH(0), WP_IW_TRANS_BRIDGING_MODE, &iw_sys);
//       terminate_on_error(global_info.iw_system, "WP_IwSystemCreate() - Transparent Bridging");
//       break;
//    case WP_IW_VLAN_AWARE_BRIDGING_MODE:
//       global_info.iw_system = WP_IwSystemCreate(WP_WINPATH(0), WP_IW_VLAN_AWARE_BRIDGING_MODE, &iw_sys);
//       terminate_on_error(global_info.iw_system, "WP_IwSystemCreate() - Vlan Aware Bridging");
//       break;
//    default:
//       break;
//    }

//    WT_CreateFlowAggregationsAndBports();

//    for(i=0; i<WT_MAX_INTERFACES; i++)
//    {
//       if(global_interfaces[i].status == WT_STATUS_INITIALIZED)
//       {
//          switch(global_interfaces[i].type)
//          {
//          case WT_PHPORT_TYPE_SGMII:
//             switch(global_info.iw_system_type)
//             {
//             case WP_IW_ROUTING_MODE:
//                rx_binding_routing->input_port = global_interfaces[i].iw_port;
//                status = WP_IwRxBindingCreate(global_interfaces[i].rx_channel, global_info.iw_system, qniw_enet, &(rx_binding_routing[0]));
//                terminate_on_error(status, "WP_RxBindingCreate()");
//                break;
//             case WP_IW_TRANS_BRIDGING_MODE:
//             case WP_IW_VLAN_AWARE_BRIDGING_MODE:
//                rx_binding_cfg->input_port = global_interfaces[i].iw_port;
//                status = WP_IwRxBindingCreate(global_interfaces[i].rx_channel, global_info.iw_system, qniw_enet, rx_binding_cfg);
//                terminate_on_error(status, "WP_RxBindingCreate()");
//                break;
//             default:
//                break;
//             }
//             break;
//          case WT_PHPORT_TYPE_UPI:
//             break;
//          case WT_PHPORT_TYPE_XGI:
//             break;
//          case WT_PHPORT_TYPE_HOST:
//             iw_tx_ch_cfg->iw_system = global_info.iw_system;
//             iw_tx_ch_cfg->group_id = WP_IW_CH_GROUP_ID_NULL;
//             iw_tx_ch_cfg->input_port = global_interfaces[i].iw_port;
//             global_interfaces[i].tx_channels[0] = WP_ChannelCreate(WT_TX_CHANNEL_TAG+i, global_interfaces[i].device, qniw_enet, WP_CH_TX, WP_IW_HOST, &iw_tx_ch_cfg);
//             terminate_on_error(global_interfaces[i].tx_channels[0], "Host TX Channel Create(iw)");
//             global_interfaces[i].tx_channel = global_interfaces[i].tx_channels[0];
//             break;
//          default:
//             break;
//          }
//       }
//    }

//    for(i=0; i<WT_MAX_INTERFACES; i++)
//    {
//       if(global_interfaces[i].status == WT_STATUS_INITIALIZED)
//       {
//          switch(global_interfaces[i].type)
//          {
//          case WT_PHPORT_TYPE_SGMII:
//             status = WP_PortEnable(global_interfaces[i].port, WP_DIRECTION_DUPLEX);
//             terminate_on_error(status, "WP_PortEnable");
//             status = WP_DeviceEnable(global_interfaces[i].device, WP_DIRECTION_DUPLEX);
//             terminate_on_error(status, "WP_DeviceEnable");
//             status = WP_ChannelEnable(global_interfaces[i].rx_channel);
//             terminate_on_error(status, "WP_ChannelEnable RX");
//             status = WP_ChannelEnable(global_interfaces[i].tx_channels[0]);
//             terminate_on_error(status, "WP_ChannelEnable TX");
//             break;
//          default:
//             break;
//          }
//       }
//    }
// }

 /* Functions added for standalone demo */


void App_TranslateAsciiToHex(WP_U8 *Hex,WP_CHAR *Ascii,WP_U32 length)
{
    WP_U32 i,first=0,second=0;

   for (i=0;i<length;i++)
     {

          if ( Ascii[2*i] >= '0' &&  Ascii[2*i] <= '9')
            first =  Ascii[2*i] - '0';

          if ( Ascii[2*i] >= 'a' &&  Ascii[2*i] <= 'f')
             first =  Ascii[2*i] - 'a' + 10;

          if ( Ascii[2*i] >= 'A' &&  Ascii[2*i] <= 'F')
             first =  Ascii[2*i] - 'A' + 10;

          if ( Ascii[2*i+1] >= '0' &&  Ascii[2*i+1] <= '9')
            second =  Ascii[2*i+1] - '0';
          if ( Ascii[2*i+1] >= 'a' &&  Ascii[2*i+1] <= 'f')
            second =  Ascii[2*i+1] - 'a' + 10;
          if ( Ascii[2*i+1] >= 'A' &&  Ascii[2*i+1] <= 'F')
            second =  Ascii[2*i+1] - 'A' + 10;

          Hex[i]=(WP_U8 )(first * 16 + second);
     }
    return;
}

static void WT_UtilitiesMenu(void)
{

   WP_CHAR ch1='c';
//   WP_U32 status;

   do
   {
      printf("\n\n1) Check for Bus Error");
      printf("\n2) Wingine and Bus Utilization");
      printf("\n3) Thread Utilization");
      printf("\n4) Thread Arbiter Dump");
      printf("\n5) Serial Arbiter Dump");
      printf("\n6) DPS Hardware Trace Start");
      //printf("\n7) DPS Hardware Trace Dump");
      printf("\nM) Main Menu\n\n");

      ch1 = getchar();

      switch (ch1)
      {
         case '1' :
            WPU_BusMonitorDump(0,(WP_CHAR *[]) {""} );   
            break;
         case '2' :
            WPU_Util(0,(WP_CHAR *[]) {""});
            break;
         case '3' :
            WPU_STADump(0,(WP_CHAR *[]) {""});
            break;
         case '4' :
            WPU_TARegs(0,(WP_CHAR *[]) {""});
            printf("\n");
            WPU_OrdMonReg(0,(WP_CHAR *[]) {""});
            printf("\n");
            WPU_TAOrder(0,(WP_CHAR *[]) {""});
            printf("\n");
            WPU_TRSStat(0,(WP_CHAR *[]) {""});
            break;
         case '5' :
            WPU_HwSADump(0,(WP_CHAR *[]) {""});
            printf("\n");
            WPU_HwStl(0,(WP_CHAR *[]) {""});
            break;
         case '6' :
            WPU_TraceBuffer(2,(WP_CHAR *[]) {"","init"});
            break;
         case '7' :
            /*WPUI_WinGineTraceBufferDump();*/
            break;
      }

    } while (ch1 != 'M' && ch1 != 'm');

}
