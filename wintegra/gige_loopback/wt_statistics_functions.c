/*************************************************************
 * (C) Copyright 2001-2010, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ************************************************************/

#ifndef __WT_STATISTICS_C__
#define __WT_STATISTICS_C__



#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "wp_wddi.h"
WP_status WP_DeviceStatistics(WP_handle device, void *stats);
WP_status WP_IwFlowStatistics(WP_handle handle, WP_read_flow_statmode statmode, void *stats);
WP_status WP_IwPortStatistics(WP_handle iw_port, void *stats);
typedef union StatField
{
   WP_U64 field;
   struct {
      WP_U32 high;
      WP_U32 low;
   } part;
} WPE_StatField;


/*----------------Device Statistics------------------------*/
void WPE_GetDeviceStatistics(WP_handle hs_device, WP_stats_enet* enet_stats)
{
   WP_status status;
   memset (enet_stats,0,sizeof(WP_stats_enet));
   status = WP_DeviceStatistics(hs_device, enet_stats);
   if (status)
   {
      printf("Error Reading Hs Enet Stat\n");
      exit(1);
   }
}

void WPE_PrintStatHsEnet(WP_handle hs_device, WP_stats_enet* enet_stats)
{
   printf("----------------------------------------------------\n");

   printf ("HS ENET Device Statistics (HW)\n");

   if(0 != enet_stats->txrx_frames_64)
   printf ("TxRx Frame  64:              %08x%08x\n",
           ((WPE_StatField)enet_stats->txrx_frames_64).part.high,
           ((WPE_StatField)enet_stats->txrx_frames_64).part.low);

   if(0 !=enet_stats->txrx_frames_127)
   printf ("TxRx Frame 127:              %08x%08x\n",
           ((WPE_StatField)enet_stats->txrx_frames_127).part.high,
           ((WPE_StatField)enet_stats->txrx_frames_127).part.low);

   if(0 !=enet_stats->txrx_frames_255 )
   printf ("TxRx Frame 255:              %08x%08x\n",
           ((WPE_StatField)enet_stats->txrx_frames_255).part.high,
           ((WPE_StatField)enet_stats->txrx_frames_255).part.low);

   if(0 !=enet_stats->txrx_frames_511 )
   printf ("TxRx Frame 511:              %08x%08x\n",
           ((WPE_StatField)enet_stats->txrx_frames_511).part.high,
           ((WPE_StatField)enet_stats->txrx_frames_511).part.low);

   if(0 !=enet_stats->txrx_frames_1023 )
   printf ("TxRx Frame 1023:             %08x%08x\n",
           ((WPE_StatField)enet_stats->txrx_frames_1023).part.high,
           ((WPE_StatField)enet_stats->txrx_frames_1023).part.low);

   if(0 !=enet_stats->txrx_frames_1518 )
   printf ("TxRx Frame 1518:             %08x%08x\n",
           ((WPE_StatField)enet_stats->txrx_frames_1518).part.high,
           ((WPE_StatField)enet_stats->txrx_frames_1518).part.low);

   if(0 !=enet_stats->txrx_frames_1518 )
   printf ("TxRx Frame 1522:             %08x%08x\n",
           ((WPE_StatField)enet_stats->txrx_frames_1522).part.high,
           ((WPE_StatField)enet_stats->txrx_frames_1522).part.low);

   if(0 !=enet_stats->rx_bytes )
   printf ("Rx Bytes:                    %08x%08x\n",
           ((WPE_StatField)enet_stats->rx_bytes).part.high,
           ((WPE_StatField)enet_stats->rx_bytes).part.low);

   if(0 !=enet_stats->rx_packets )
   printf ("Rx Packets:                  %08x%08x\n",
           ((WPE_StatField)enet_stats->rx_packets).part.high,
           ((WPE_StatField)enet_stats->rx_packets).part.low);

   if(0 !=enet_stats->rx_err_fcs )
   printf ("Rx Error FCS:                %08x%08x\n",
           ((WPE_StatField)enet_stats->rx_err_fcs).part.high,
           ((WPE_StatField)enet_stats->rx_err_fcs).part.low);

   if(0 !=enet_stats->rx_multicast )
   printf ("Rx Multicast:                %08x%08x\n",
           ((WPE_StatField)enet_stats->rx_multicast).part.high,
           ((WPE_StatField)enet_stats->rx_multicast).part.low);

   if(0 !=enet_stats->rx_broadcast )
   printf ("Rx Broadcast:                %08x%08x\n",
           ((WPE_StatField)enet_stats->rx_broadcast).part.high,
           ((WPE_StatField)enet_stats->rx_broadcast).part.low);

   if(0 !=enet_stats->rx_mac_control )
   printf ("Rx Mac Control:              %08x%08x\n",
           ((WPE_StatField)enet_stats->rx_mac_control).part.high,
           ((WPE_StatField)enet_stats->rx_mac_control).part.low);

   if(0 !=enet_stats->rx_mac_pause )
   printf ("Rx Mac Pause:                %08x%08x\n",
           ((WPE_StatField)enet_stats->rx_mac_pause).part.high,
           ((WPE_StatField)enet_stats->rx_mac_pause).part.low);

   if(0 !=enet_stats->rx_mac_unknown )
   printf ("Rx Mac Unknown:              %08x%08x\n",
           ((WPE_StatField)enet_stats->rx_mac_unknown).part.high,
           ((WPE_StatField)enet_stats->rx_mac_unknown).part.low);

   if(0 !=enet_stats->rx_err_alignment )
   printf ("Rx Error Alignment:          %08x%08x\n",
           ((WPE_StatField)enet_stats->rx_err_alignment).part.high,
           ((WPE_StatField)enet_stats->rx_err_alignment).part.low);

   if(0 !=enet_stats->rx_err_length )
   printf ("Rx Error LEN:                %08x%08x\n",
           ((WPE_StatField)enet_stats->rx_err_length).part.high,
           ((WPE_StatField)enet_stats->rx_err_length).part.low);

   if(0 !=enet_stats->rx_err_code )
   printf ("Rx Error Code:               %08x%08x\n",
           ((WPE_StatField)enet_stats->rx_err_code).part.high,
           ((WPE_StatField)enet_stats->rx_err_code).part.low);

   if(0 !=enet_stats->rx_false_carrier )
   printf ("Rx False Carrier:            %08x%08x\n",
           ((WPE_StatField)enet_stats->rx_false_carrier).part.high,
           ((WPE_StatField)enet_stats->rx_false_carrier).part.low);

   if(0 !=enet_stats->rx_undersize )
   printf ("Rx Undersize:                %08x%08x\n",
           ((WPE_StatField)enet_stats->rx_undersize).part.high,
           ((WPE_StatField)enet_stats->rx_undersize).part.low);

   if(0 !=enet_stats->rx_oversize )
   printf ("Rx Oversize:                 %08x%08x\n",
           ((WPE_StatField)enet_stats->rx_oversize).part.high,
           ((WPE_StatField)enet_stats->rx_oversize).part.low);

   if(0 !=enet_stats->rx_fragments )
   printf ("Rx Fragments:                %08x%08x\n",
           ((WPE_StatField)enet_stats->rx_fragments).part.high,
           ((WPE_StatField)enet_stats->rx_fragments).part.low);
   if(0 !=enet_stats->rx_jabber )
   printf ("Rx Jabber:                   %08x%08x\n",
           ((WPE_StatField)enet_stats->rx_jabber).part.high,
           ((WPE_StatField)enet_stats->rx_jabber).part.low);
   if(0 !=enet_stats->rx_dropped )
   printf ("Rx Dropped:                  %08x%08x\n",
           ((WPE_StatField)enet_stats->rx_dropped).part.high,
           ((WPE_StatField)enet_stats->rx_dropped).part.low);

   if(0 !=enet_stats->tx_bytes )
   printf ("Tx Bytes:                    %08x%08x\n",
           ((WPE_StatField)enet_stats->tx_bytes).part.high,
           ((WPE_StatField)enet_stats->tx_bytes).part.low);

   if(0 !=enet_stats->tx_packets )
   printf ("Tx Packets:                  %08x%08x\n",
           ((WPE_StatField)enet_stats->tx_packets).part.high,
           ((WPE_StatField)enet_stats->tx_packets).part.low);

   if(0 !=enet_stats->tx_multicast )
   printf ("Tx Multicast:                %08x%08x\n",
           ((WPE_StatField)enet_stats->tx_multicast).part.high,
           ((WPE_StatField)enet_stats->tx_multicast).part.low);

   if(0 !=enet_stats->tx_broadcast )
   printf ("Tx Broadcast:                %08x%08x\n",
           ((WPE_StatField)enet_stats->tx_broadcast).part.high,
           ((WPE_StatField)enet_stats->tx_broadcast).part.low);

   if(0 !=enet_stats->tx_mac_pause )
   printf ("Tx Mac Pause:                %08x%08x\n",
           ((WPE_StatField)enet_stats->tx_mac_pause).part.high,
           ((WPE_StatField)enet_stats->tx_mac_pause).part.low);

   if(0 !=enet_stats->tx_defer )
   printf ("Tx Defer:                    %08x%08x\n",
           ((WPE_StatField)enet_stats->tx_defer).part.high,
           ((WPE_StatField)enet_stats->tx_defer).part.low);

   if(0 !=enet_stats->tx_excess_defer )
   printf ("Tx Exess Defer:              %08x%08x\n",
           ((WPE_StatField)enet_stats->tx_excess_defer).part.high,
           ((WPE_StatField)enet_stats->tx_excess_defer).part.low);

   if(0 !=enet_stats->tx_single_collision )
   printf ("Tx Single Collision:         %08x%08x\n",
           ((WPE_StatField)enet_stats->tx_single_collision).part.high,
           ((WPE_StatField)enet_stats->tx_single_collision).part.low);

   if(0 !=enet_stats->tx_multi_collision )
   printf ("Tx Multi Collision:          %08x%08x\n",
           ((WPE_StatField)enet_stats->tx_multi_collision).part.high,
           ((WPE_StatField)enet_stats->tx_multi_collision).part.low);

   if(0 !=enet_stats->tx_late_collision )
   printf ("Tx Late Collision:           %08x%08x\n",
           ((WPE_StatField)enet_stats->tx_late_collision).part.high,
           ((WPE_StatField)enet_stats->tx_late_collision).part.low);

   if(0 !=enet_stats->tx_excess_collision )
   printf ("Tx Excess Collision:         %08x%08x\n",
           ((WPE_StatField)enet_stats->tx_excess_collision).part.high,
           ((WPE_StatField)enet_stats->tx_excess_collision).part.low);

   if(0 !=enet_stats->tx_no_collision )
   printf ("Tx No Collision:             %08x%08x\n",
           ((WPE_StatField)enet_stats->tx_no_collision).part.high,
           ((WPE_StatField)enet_stats->tx_no_collision).part.low);

   if(0 !=enet_stats->tx_mac_pause_honored )
   printf ("Tx Mac Pause Honored:        %08x%08x\n",
           ((WPE_StatField)enet_stats->tx_mac_pause_honored).part.high,
           ((WPE_StatField)enet_stats->tx_mac_pause_honored).part.low);

   if(0 !=enet_stats->tx_dropped )
   printf ("Tx Dropped:                  %08x%08x\n",
           ((WPE_StatField)enet_stats->tx_dropped).part.high,
           ((WPE_StatField)enet_stats->tx_dropped).part.low);

   if(0 !=enet_stats->tx_jabber )
   printf ("Tx Jabber:                   %08x%08x\n",
           ((WPE_StatField)enet_stats->tx_jabber).part.high,
           ((WPE_StatField)enet_stats->tx_jabber).part.low);

   if(0 !=enet_stats->tx_err_fcs )
   printf ("Tx Errors FCS:               %08x%08x\n",
           ((WPE_StatField)enet_stats->tx_err_fcs).part.high,
           ((WPE_StatField)enet_stats->tx_err_fcs).part.low);

   if(0 !=enet_stats->tx_control )
   printf ("Tx Control:                  %08x%08x\n",
           ((WPE_StatField)enet_stats->tx_control).part.high,
           ((WPE_StatField)enet_stats->tx_control).part.low);

   if(0 !=enet_stats->tx_oversize )
   printf ("Tx Oversize:                 %08x%08x\n",
           ((WPE_StatField)enet_stats->tx_oversize).part.high,
           ((WPE_StatField)enet_stats->tx_oversize).part.low);

   if(0 !=enet_stats->tx_undersize )
   printf ("Tx Undersize:                %08x%08x\n",
           ((WPE_StatField)enet_stats->tx_undersize).part.high,
           ((WPE_StatField)enet_stats->tx_undersize).part.low);

   if(0 !=enet_stats->tx_fragments )
   printf ("Tx Fragments:                %08x%08x\n",
           ((WPE_StatField)enet_stats->tx_fragments).part.high,
           ((WPE_StatField)enet_stats->tx_fragments).part.low);


   printf ("HS ENET Device Statistics (DPS)\n");

   if(0 !=enet_stats->rx_host_frames )
   printf ("Rx Host Frames:              %08x%08x\n",
           ((WPE_StatField)enet_stats->rx_host_frames).part.high,
           ((WPE_StatField)enet_stats->rx_host_frames).part.low);
   if(0 !=enet_stats->rx_iw_frames )
   printf ("Rx Iw Frames:                %08x%08x\n",
           ((WPE_StatField)enet_stats->rx_iw_frames).part.high,
           ((WPE_StatField)enet_stats->rx_iw_frames).part.low);
   if(0 !=enet_stats->rx_err_host_full )
   printf ("Rx Error Host Full:          %08x%08x\n",
           ((WPE_StatField)enet_stats->rx_err_host_full).part.high,
           ((WPE_StatField)enet_stats->rx_err_host_full).part.low);
   if(0 !=enet_stats->rx_err_fbp_underrun )
   printf ("Rx Error Fbp Underrun:       %08x%08x\n",
           ((WPE_StatField)enet_stats->rx_err_fbp_underrun).part.high,
           ((WPE_StatField)enet_stats->rx_err_fbp_underrun).part.low);
   if(0 !=enet_stats->rx_err_nonvalid_mac )
   printf ("Rx Error Nonvalid Mac:       %08x%08x\n",
           ((WPE_StatField)enet_stats->rx_err_nonvalid_mac).part.high,
           ((WPE_StatField)enet_stats->rx_err_nonvalid_mac).part.low);
   if(0 !=enet_stats->rx_err_mru )
   printf ("Rx Error Mru:                %08x%08x\n",
           ((WPE_StatField)enet_stats->rx_err_mru).part.high,
           ((WPE_StatField)enet_stats->rx_err_mru).part.low);
   if(0 !=enet_stats->rx_err_sdu )
   printf ("Rx Error Sdu:                %08x%08x\n",
           ((WPE_StatField)enet_stats->rx_err_sdu).part.high,
           ((WPE_StatField)enet_stats->rx_err_sdu).part.low);
   if(0 !=enet_stats->rx_err_overrun )
   printf ("Rx Error Overrun:            %08x%08x\n",
           ((WPE_StatField)enet_stats->rx_err_overrun).part.high,
           ((WPE_StatField)enet_stats->rx_err_overrun).part.low);
   if(0 !=enet_stats->tx_frames )
   printf ("Tx Frames:                   %08x%08x\n",
           ((WPE_StatField)enet_stats->tx_frames).part.high,
           ((WPE_StatField)enet_stats->tx_frames).part.low);
   printf("----------------------------------------------------\n");

   printf ("HS ENET Device Statistics (Between Serial Interface and DPS)\n");
   printf("----------------------------------------------------\n");
   if(0 !=enet_stats->serial_dps_if_stats.tx_frames )
   printf ("Tx Frames:                   %08x%08x\n",
           ((WPE_StatField)enet_stats->serial_dps_if_stats.tx_frames).part.high,
           ((WPE_StatField)enet_stats->serial_dps_if_stats.tx_frames).part.low);
   if(0 !=enet_stats->serial_dps_if_stats.tx_bytes )
   printf ("Tx Bytes:                    %08x%08x\n",
           ((WPE_StatField)enet_stats->serial_dps_if_stats.tx_bytes).part.high,
           ((WPE_StatField)enet_stats->serial_dps_if_stats.tx_bytes).part.low);
   if(0 !=enet_stats->serial_dps_if_stats.rx_frames )
   printf ("Rx Frames:                   %08x%08x\n",
           ((WPE_StatField)enet_stats->serial_dps_if_stats.tx_frames).part.high,
           ((WPE_StatField)enet_stats->serial_dps_if_stats.tx_frames).part.low);
   if(0 !=enet_stats->serial_dps_if_stats.rx_bytes )
   printf ("Rx Bytes:                    %08x%08x\n",
           ((WPE_StatField)enet_stats->serial_dps_if_stats.tx_bytes).part.high,
           ((WPE_StatField)enet_stats->serial_dps_if_stats.tx_bytes).part.low);
   if(0 !=enet_stats->serial_dps_if_stats.rx_dropped_frames )
   printf ("Rx Dropped Frames:           %08x%08x\n",
           ((WPE_StatField)enet_stats->serial_dps_if_stats.rx_dropped_frames).part.high,
           ((WPE_StatField)enet_stats->serial_dps_if_stats.rx_dropped_frames).part.low);
   if(0 !=enet_stats->serial_dps_if_stats.rx_pce_denied_frames )
   printf ("Rx PCE denied Frames:        %08x%08x\n",
           ((WPE_StatField)enet_stats->serial_dps_if_stats.rx_pce_denied_frames).part.high,
           ((WPE_StatField)enet_stats->serial_dps_if_stats.rx_pce_denied_frames).part.low);
}











/*----------------Flow Statistics------------------------*/
void WPE_GetFlowStatistics(WP_handle agg, WP_iw_flow_stats* iw_flow_stats)
{
   WP_status status;
   memset(iw_flow_stats,0,sizeof(WP_iw_flow_stats));
   status = WP_IwFlowStatistics( agg, WP_IW_FLOW_STAT, iw_flow_stats);
   if(status != WP_OK)
   {
     printf("Error reading Flow statistics\n");
     exit(1);
   }
}


void WPE_PrintFlowStats(WP_handle agg, WP_iw_flow_stats* iw_flow_stats)
{
   WP_U32 forward_packets = 0;
   printf ("\n Aggregation flow statistics of flow %d (%x)\n",
           (agg & 0x000000ff),agg);
   printf ("=================================================\n");

      printf ("Forward Packet:          %08x%08x\n",
              ((WPE_StatField)iw_flow_stats->forward_packet).part.high,
              ((WPE_StatField)iw_flow_stats->forward_packet).part.low);
      forward_packets = ((WPE_StatField)iw_flow_stats->forward_packet).part.low;

    if(0 != iw_flow_stats->fbp_drop_packets)
      printf( "FBP Drop Packets:        %08x%08x\n",
              ((WPE_StatField)iw_flow_stats->fbp_drop_packets).part.high,
    ((WPE_StatField)iw_flow_stats->fbp_drop_packets).part.low);
    if(0 != iw_flow_stats->mtu_drop_packets)
      printf( "MTU Drop Packets:        %08x%08x\n",
              ((WPE_StatField)iw_flow_stats->mtu_drop_packets).part.high,
              ((WPE_StatField)iw_flow_stats->mtu_drop_packets).part.low);
    if(0 != iw_flow_stats->ttl_drop_packets)
      printf( "TTL Drop Packets:        %08x%08x\n",
               ((WPE_StatField)iw_flow_stats->ttl_drop_packets).part.high,
               ((WPE_StatField)iw_flow_stats->ttl_drop_packets).part.low);
     if(0 != iw_flow_stats->tx_queue_drop_packets)
       printf( "TX Queue Drop Packets:   %08x%08x\n",
               ((WPE_StatField)iw_flow_stats->tx_queue_drop_packets).part.high,
               ((WPE_StatField)iw_flow_stats->tx_queue_drop_packets).part.low);
     if(0 != iw_flow_stats->mpls_drop)
       printf( "MPLS Drop:               %08x%08x\n",
               ((WPE_StatField)iw_flow_stats->mpls_drop).part.high,
               ((WPE_StatField)iw_flow_stats->mpls_drop).part.low);
     if(0 != iw_flow_stats->denied_packets)
       printf( "Denied Packets:          %08x%08x\n",
               ((WPE_StatField)iw_flow_stats->denied_packets).part.high,
               ((WPE_StatField)iw_flow_stats->denied_packets).part.low);
     if(0 != iw_flow_stats->group_filtered_packets)
       printf( "Group filtered packets:  %08x%08x\n",
               ((WPE_StatField)iw_flow_stats->group_filtered_packets).part.high,
               ((WPE_StatField)iw_flow_stats->group_filtered_packets).part.low);
    if(0 != iw_flow_stats->forwarded_bytes)
       printf( "forwarded_bytes:         %08x%08x\n",
               ((WPE_StatField)iw_flow_stats->forwarded_bytes).part.high,
               ((WPE_StatField)iw_flow_stats->forwarded_bytes).part.low);
    if(0 != iw_flow_stats->gtp_bad_headers)
       printf( "GTP bad headers:         %08x%08x\n",
               ((WPE_StatField)iw_flow_stats->gtp_bad_headers).part.high,
               ((WPE_StatField)iw_flow_stats->gtp_bad_headers).part.low);
    if(0 != iw_flow_stats->policer_non_conforming_packets)
       printf( "Policer Non Conforming:  %08x%08x\n",
               ((WPE_StatField)iw_flow_stats->policer_non_conforming_packets).part.high,
               ((WPE_StatField)iw_flow_stats->policer_non_conforming_packets).part.low);
    if(0 != iw_flow_stats->cfm_mac_in_mac_drop)
       printf( "CFM MAC in MAC Drop:     %08x%08x\n",
               ((WPE_StatField)iw_flow_stats->cfm_mac_in_mac_drop).part.high,
               ((WPE_StatField)iw_flow_stats->cfm_mac_in_mac_drop).part.low);
    printf("\n");
}


/*-------------------Port Statistics --------------------------*/
void WPE_GetPortStatistics(WP_handle iw_port_handle, WP_route_port_stats* iwport_stats)
{
   WP_status status;
   memset(iwport_stats, 0, sizeof(WP_route_port_stats));
   status = WP_IwPortStatistics(iw_port_handle,iwport_stats);
   if (status == WP_OK)
   {
      printf("Error Reading Port Statistics\n");
   }
}

void WPE_PrintIWPortStats(WP_handle iw_port_handle, WP_route_port_stats* iwport_stats)
{
   printf ("\nIW Port Statistics of Port \n");
   printf ("==========================================================\n");

   printf("\n");

   printf ("rx_valid_packets :                       %08x%08x\n",
           ((WPE_StatField)iwport_stats->rx_valid_packets).part.high,
           ((WPE_StatField)iwport_stats->rx_valid_packets).part.low);
   printf ("rx_ipv4_option_packets :                 %08x%08x\n",
           ((WPE_StatField)iwport_stats->rx_ipv4_option_packets).part.high,
           ((WPE_StatField)iwport_stats->rx_ipv4_option_packets).part.low);
   printf ("rx_non_ip_packets :                      %08x%08x\n",
           ((WPE_StatField)iwport_stats->rx_non_ip_packets).part.high,
           ((WPE_StatField)iwport_stats->rx_non_ip_packets).part.low);
   printf ("rx_compressed_packets :                  %08x%08x\n",
           ((WPE_StatField)iwport_stats->rx_compressed_packets).part.high,
           ((WPE_StatField)iwport_stats->rx_compressed_packets).part.low);
   printf ("rx_valid_mpls_packets :                  %08x%08x\n",
           ((WPE_StatField)iwport_stats->rx_valid_mpls_packets).part.high,
           ((WPE_StatField)iwport_stats->rx_valid_mpls_packets).part.low);
   printf ("rx_protocol_error :                      %08x%08x\n",
           ((WPE_StatField)iwport_stats->rx_protocol_error).part.high,
           ((WPE_StatField)iwport_stats->rx_protocol_error).part.low);
   printf ("rx_checksum_error :                      %08x%08x\n",
           ((WPE_StatField)iwport_stats->rx_checksum_error).part.high,
           ((WPE_StatField)iwport_stats->rx_checksum_error).part.low);
   printf ("rx_discard_classifier :                  %08x%08x\n",
           ((WPE_StatField)iwport_stats->rx_discard_classifier).part.high,
           ((WPE_StatField)iwport_stats->rx_discard_classifier).part.low);
   printf ("rx_mpls_lookup_drop :                    %08x%08x\n",
           ((WPE_StatField)iwport_stats->rx_mpls_lookup_drop).part.high,
           ((WPE_StatField)iwport_stats->rx_mpls_lookup_drop).part.low);
   printf ("tx_forwarded_packets :                   %08x%08x\n",
           ((WPE_StatField)iwport_stats->tx_forwarded_packets).part.high,
           ((WPE_StatField)iwport_stats->tx_forwarded_packets).part.low);
   printf ("rx_ipv6_hop_by_hop_host_term :           %08x%08x\n",
           ((WPE_StatField)iwport_stats->rx_ipv6_hop_by_hop_host_term).part.high,
           ((WPE_StatField)iwport_stats->rx_ipv6_hop_by_hop_host_term).part.low);
   printf ("rx_ipv6_link_local_host_term :           %08x%08x\n",
           ((WPE_StatField)iwport_stats->rx_ipv6_link_local_host_term).part.high,
           ((WPE_StatField)iwport_stats->rx_ipv6_link_local_host_term).part.low);
   printf ("rx_discard_lpm :                         %08x%08x\n",
           ((WPE_StatField)iwport_stats->rx_discard_lpm).part.high,
           ((WPE_StatField)iwport_stats->rx_discard_lpm).part.low);
   printf ("discard_dfc_filter[0] :                  %08x%08x\n",
           ((WPE_StatField)iwport_stats->discard_dfc_filter[0]).part.high,
           ((WPE_StatField)iwport_stats->discard_dfc_filter[0]).part.low);
   printf ("discard_dfc_filter[1] :                  %08x%08x\n",
           ((WPE_StatField)iwport_stats->discard_dfc_filter[1]).part.high,
           ((WPE_StatField)iwport_stats->discard_dfc_filter[1]).part.low);
   printf ("ingress_policer_non_conforming_packets : %08x%08x\n",
           ((WPE_StatField)iwport_stats->ingress_policer_non_conforming_packets).part.high,
           ((WPE_StatField)iwport_stats->ingress_policer_non_conforming_packets).part.low);

   printf("\n");
   return;
}




#endif


