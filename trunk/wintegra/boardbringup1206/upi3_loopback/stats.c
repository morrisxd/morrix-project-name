/*****************************************************************************
 * (C) Copyright 2009, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra, Inc.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 *****************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "wp_wddi.h"
#include "include/core/registry/wpi_handle.h"
#include "include/core/utility/wpi_utils.h"

void terminate_on_error(WP_handle, WP_CHAR*);

void printAtmDeviceStats(WP_handle handle) {
  WP_status status;
  WP_stats_upi_atm atm_upi_stats = {0};

  status = WP_DeviceStatistics(handle, &atm_upi_stats);
  terminate_on_error(handle, "WP_DeviceStatistics()");
  printf("\n");
  printf("Atm Device Statistics:\n");
  printf("\n");
  printf("Rx short/long cells= %#x%x\n",\
         (WP_U32)(atm_upi_stats.rx_err_slc >> 32),\
          (WP_U32)atm_upi_stats.rx_err_slc);
  printf("Rx Error Parity= %#x%x\n",\
         (WP_U32)(atm_upi_stats.rx_err_parity >> 32),\
          (WP_U32)atm_upi_stats.rx_err_parity);
  printf("Rx address mismatch= %#x%x\n",\
         (WP_U32)(atm_upi_stats.rx_err_addr_mismatch >> 32),\
          (WP_U32)atm_upi_stats.rx_err_addr_mismatch);
  printf("Rx cell count= %#x%x\n",\
         (WP_U32)(atm_upi_stats.ext_stats.rx_cells >> 32),\
          (WP_U32)atm_upi_stats.ext_stats.rx_cells);
  printf("rx_clp0_cells= %#x%x\n",\
         (WP_U32)(atm_upi_stats.ext_stats.rx_clp0_cells >> 32),\
          (WP_U32)atm_upi_stats.ext_stats.rx_clp0_cells);
  printf("rx_clp1_cells= %#x%x\n",\
         (WP_U32)(atm_upi_stats.ext_stats.rx_clp1_cells >> 32),\
          (WP_U32)atm_upi_stats.ext_stats.rx_clp1_cells);
  printf("rx_gfc_cells= %#x%x\n",\
         (WP_U32)(atm_upi_stats.ext_stats.rx_gfc_cells >> 32),\
          (WP_U32)atm_upi_stats.ext_stats.rx_gfc_cells);
  printf("rx_oam_cells= %#x%x\n",\
         (WP_U32)(atm_upi_stats.ext_stats.rx_oam_cells >> 32),\
          (WP_U32)atm_upi_stats.ext_stats.rx_oam_cells);
  printf("rx_rm_cells= %#x%x\n",\
         (WP_U32)(atm_upi_stats.ext_stats.rx_rm_cells >> 32),\
          (WP_U32)atm_upi_stats.ext_stats.rx_rm_cells);
  printf("rx_last_unknown_addr= %#x%x\n",\
         (WP_U32)(atm_upi_stats.ext_stats.rx_last_unknown_addr >> 32),\
          (WP_U32)atm_upi_stats.ext_stats.rx_last_unknown_addr);
  printf("tx_cells= %#x%x\n",(WP_U32)(atm_upi_stats.ext_stats.tx_cells >> 32),\
          (WP_U32)atm_upi_stats.ext_stats.tx_cells);
  printf("tx_clp0_cells= %#x%x\n",\
         (WP_U32)(atm_upi_stats.ext_stats.tx_clp0_cells >> 32),\
          (WP_U32)atm_upi_stats.ext_stats.tx_clp0_cells);
  printf("tx_clp1_cells= %#x%x\n",\
         (WP_U32)(atm_upi_stats.ext_stats.tx_clp1_cells >> 32),\
          (WP_U32)atm_upi_stats.ext_stats.tx_clp1_cells);
  printf("tx_efci_cells= %#x%x\n",\
         (WP_U32)(atm_upi_stats.ext_stats.tx_efci_cells >> 32),\
          (WP_U32)atm_upi_stats.ext_stats.tx_efci_cells);
  printf("tx_rm_cells= %#x%x\n",\
         (WP_U32)(atm_upi_stats.ext_stats.tx_rm_cells >> 32),\
          (WP_U32)atm_upi_stats.ext_stats.tx_rm_cells);
  printf("tx_oam_cells= %#x%x\n",\
         (WP_U32)(atm_upi_stats.ext_stats.tx_oam_cells >> 32),\
          (WP_U32)atm_upi_stats.ext_stats.tx_oam_cells);
  printf("tx_idle_cells= %#x%x\n",\
         (WP_U32)(atm_upi_stats.ext_stats.tx_idle_cells >> 32),\
          (WP_U32)atm_upi_stats.ext_stats.tx_idle_cells);
  printf("\n");
}

void printChannelStat(WP_handle handle) {
  WP_status status;
  WP_stats_aal5_rx rx_aal5_stats = {0};
  WP_stats_aal5_tx tx_aal5_stats = {0};
  WP_stats_aal0_rx rx_aal0_stats = {0};
  WP_stats_aal0_tx tx_aal0_stats = {0};
  WP_stats_aal2_rx rx_aal2_stats = {0};
  WP_stats_atmsw_rx rx_atmsw_stats ={0};
  WP_stats_atmsw_tx tx_atmsw_stats ={0};

  switch (WPI_HANDLE_SUBTYPE(handle)) {
    case WP_ATM_AAL0:
      if(WPI_HANDLE_RX_CHANNEL_P(handle)) {
        status = WP_ChannelStatistics(handle, &rx_aal0_stats);
        terminate_on_error(handle, "WP_ChannelStatistics() WP_ATM_AAL0");
        printf("Atm Rx Channel Statistics:\n");
        printf("\n");
        printf("rx_cells= %#x%x\n",\
         (WP_U32)(rx_aal0_stats.rx_cells >> 32),\
          (WP_U32)rx_aal0_stats.rx_cells);
        printf("rx_err_crc= %#x%x\n",\
         (WP_U32)(rx_aal0_stats.err_crc >> 32),\
          (WP_U32)rx_aal0_stats.err_crc);
        printf("iw_cells= %#x%x\n",\
         (WP_U32)(rx_aal0_stats.iw_cells >> 32),\
          (WP_U32)rx_aal0_stats.iw_cells);
        printf("err_underflow= %#x%x\n",\
         (WP_U32)(rx_aal0_stats.err_underflow >> 32),\
          (WP_U32)rx_aal0_stats.err_underflow);
        printf("\n");
      } else if (WPI_HANDLE_TX_CHANNEL_P(handle)) {
        status = WP_ChannelStatistics(handle, &tx_aal0_stats);
        terminate_on_error(handle, "WP_ChannelStatistics() WP_ATM_AAL0");
        printf("Atm Tx Channel Statistics:\n");
        printf("\n");
        printf("tx_cells= %#x%x\n",\
         (WP_U32)(tx_aal0_stats.tx_cells >> 32),\
          (WP_U32)tx_aal0_stats.tx_cells);
        printf("\n");
      }
      break;
    case WP_ATM_AAL1:
      /* status = WP_ChannelStatistics(handle, &aal1_stats); */
      break;
    case WP_ATM_AAL2:
      if (WPI_HANDLE_RX_CHANNEL_P(handle)) {
         status = WP_ChannelStatistics(handle, &rx_aal2_stats);
         terminate_on_error(handle, "WP_ChannelStatistics() WP_ATM_AAL2");
         printf("\n");
         printf("Atm Rx Channel Statistics:\n");
         printf("\n");
         printf("CPS Underflow errors= %#x%x\n",\
         (WP_U32)(rx_aal2_stats.err_cps_underflow >> 32),\
          (WP_U32)rx_aal2_stats.err_cps_underflow);
         printf("CPS Event errors= %#x%x\n",\
         (WP_U32)(rx_aal2_stats.err_cps_event >> 32),\
          (WP_U32)rx_aal2_stats.err_cps_event);
         printf("CPS Protocol errors= %#x%x\n",\
         (WP_U32)(rx_aal2_stats.err_protocol >> 32),\
          (WP_U32)rx_aal2_stats.err_protocol);
         printf("SS Length errors= %#x%x\n",\
         (WP_U32)(rx_aal2_stats.err_ss_length >> 32),\
          (WP_U32)rx_aal2_stats.err_ss_length);
         printf("SS CRC errors= %#x%x\n",\
         (WP_U32)(rx_aal2_stats.err_ss_crc>> 32),\
          (WP_U32)rx_aal2_stats.err_ss_crc);
         printf("SS Underflow errors= %#x%x\n",\
         (WP_U32)(rx_aal2_stats.err_ss_underflow>> 32),\
          (WP_U32)rx_aal2_stats.err_ss_underflow);
         printf("SS CPS errors= %#x%x\n",\
         (WP_U32)(rx_aal2_stats.err_ss_cps>> 32),\
          (WP_U32)rx_aal2_stats.err_ss_cps);
         printf("SW CPS errors= %#x%x\n",\
         (WP_U32)(rx_aal2_stats.err_sw_cps>> 32),\
          (WP_U32)rx_aal2_stats.err_sw_cps);
         printf("SW Overflow errors= %#x%x\n",\
         (WP_U32)(rx_aal2_stats.err_sw_overflow>> 32),\
          (WP_U32)rx_aal2_stats.err_sw_overflow);
         printf("SW Underflow errors= %#x%x\n",\
         (WP_U32)(rx_aal2_stats.err_sw_underflow>> 32),\
          (WP_U32)rx_aal2_stats.err_sw_underflow);
         printf("\n");
      } else {
        /* currently no channel-level tx channel statistics are defined */
      }
      break;
    case WP_ATM_AAL5:
       if (WPI_HANDLE_RX_CHANNEL_P(handle)) {
          status = WP_ChannelStatistics(handle, &rx_aal5_stats);
          terminate_on_error(handle, "WP_ChannelStatistics() WP_ATM_AAL5");
          printf("\n");
          printf("Atm Rx Channel Statistics:\n");
          printf("\n");
          printf("rx_frames= %#x%x\n",(WP_U32)(rx_aal5_stats.rx_frames >> 32),\
          (WP_U32)rx_aal5_stats.rx_frames);
          printf("about_frames= %#x%x\n",(WP_U32)(rx_aal5_stats.aborted_frames >> 32),\
          (WP_U32)rx_aal5_stats.aborted_frames);
          printf("err_crc= %#x%x\n",(WP_U32)(rx_aal5_stats.err_crc >> 32),\
          (WP_U32)rx_aal5_stats.err_crc);
          printf("err_size= %#x%x\n",(WP_U32)(rx_aal5_stats.err_size >> 32),\
          (WP_U32)rx_aal5_stats.err_size);
          printf("err_maxsdu= %#x%x\n",\
          (WP_U32)(rx_aal5_stats.err_maxsdu>> 32),\
          (WP_U32)rx_aal5_stats.err_maxsdu);
          printf("err_iw_underrun= %#x%x\n",\
         (WP_U32)(rx_aal5_stats.err_iw_underrun>> 32),\
          (WP_U32)rx_aal5_stats.err_iw_underrun);
          printf("err_iw_maxmru= %#x%x\n",\
         (WP_U32)(rx_aal5_stats.err_iw_maxmru>> 32),\
          (WP_U32)rx_aal5_stats.err_iw_maxmru);
          printf("err_ras_timeout= %#x%x\n",\
         (WP_U32)(rx_aal5_stats.err_ras_timeout>> 32),\
          (WP_U32)rx_aal5_stats.err_ras_timeout);
          printf("err_underflow= %#x%x\n",\
         (WP_U32)(rx_aal5_stats.err_underflow>> 32),\
          (WP_U32)rx_aal5_stats.err_underflow);
          printf("err_iw_underrun= %#x%x\n",\
         (WP_U32)(rx_aal5_stats.err_iw_underrun>> 32),\
          (WP_U32)rx_aal5_stats.err_iw_underrun);
          printf("err_cpi= %#x%x\n",\
         (WP_U32)(rx_aal5_stats.err_cpi>> 32),\
          (WP_U32)rx_aal5_stats.err_cpi);
          printf("clp_frames= %#x%x\n",\
         (WP_U32)(rx_aal5_stats.clp_frames>> 32),\
          (WP_U32)rx_aal5_stats.clp_frames);
          printf("l3_iw_frames= %#x%x\n",\
         (WP_U32)(rx_aal5_stats.l3_iw_frames>> 32),\
          (WP_U32)rx_aal5_stats.l3_iw_frames);
          printf("rx_clp0_cells= %#x%x\n",\
         (WP_U32)(rx_aal5_stats.rx_clp0_cells>> 32),\
          (WP_U32)rx_aal5_stats.rx_clp0_cells);
          printf("rx_clp1_cells= %#x%x\n",\
         (WP_U32)(rx_aal5_stats.rx_clp1_cells>> 32), \
         (WP_U32)rx_aal5_stats.rx_clp1_cells);
          printf("\n");
         } else if (WPI_HANDLE_TX_CHANNEL_P(handle)){
          status = WP_ChannelStatistics(handle, &tx_aal5_stats);
          terminate_on_error(handle, "WP_ChannelStatistics() WP_ATM_AAL5");
          printf("\n");
          printf("Atm Tx Channel Statistics:\n");
          printf("\n");
          printf("tx_frames= %#x%x\n",\
         (WP_U32)(tx_aal5_stats.tx_frames >> 32),\
          (WP_U32)tx_aal5_stats.tx_frames);
          printf("tx_clp0_cells= %#x%x\n",\
         (WP_U32)(tx_aal5_stats.tx_clp0_cells>> 32),\
          (WP_U32)tx_aal5_stats.tx_clp0_cells);
          printf("tx_clp1_cells= %#x%x\n",\
         (WP_U32)(tx_aal5_stats.tx_clp1_cells>> 32),\
          (WP_U32)tx_aal5_stats.tx_clp1_cells);
          printf("\n");
        }
        break;
    case WP_ATM_ATMSW:
       if (WPI_HANDLE_RX_CHANNEL_P(handle)) {
          status = WP_ChannelStatistics(handle, &rx_atmsw_stats);
          terminate_on_error(handle, "WP_ChannelStatistics() WP_ATM_ATMSW");
          printf("\nATMSW Rx Statistics \n");
          printf("ATMsw Rx clp0 = %#x%x\n",\
          (WP_U32)(rx_atmsw_stats.rx_clp0 >> 32),\
         (WP_U32)rx_atmsw_stats.rx_clp0);
          printf("ATMsw Rx clp1 = %#x%x\n",\
          (WP_U32)(rx_atmsw_stats.rx_clp1 >> 32),\
         (WP_U32)rx_atmsw_stats.rx_clp1);
          printf("ATMsw Rx nonconf_clp0 = %#x%x\n",\
          (WP_U32)(rx_atmsw_stats.upc_nonconform_clp0 >> 32),\
          (WP_U32)rx_atmsw_stats.upc_nonconform_clp0);
          printf("ATMsw Rx nonconf_clp1 = %#x%x\n",\
          (WP_U32)(rx_atmsw_stats.upc_nonconform_clp1 >> 32),\
          (WP_U32)rx_atmsw_stats.upc_nonconform_clp1);
          printf("ATMsw Rx upc_tagged = %#x%x\n",\
          (WP_U32)(rx_atmsw_stats.upc_tagged >> 32),\
          (WP_U32)rx_atmsw_stats.upc_tagged);
          printf("ATMsw Rx cong_discard = %#x%x\n",\
          (WP_U32)(rx_atmsw_stats.congest_discard >> 32),\
          (WP_U32)rx_atmsw_stats.congest_discard);
          printf("ATMsw Rx cong_discard_clp0 = %#x%x\n",\
          (WP_U32)(rx_atmsw_stats.congest_discard_clp0 >> 32),\
          (WP_U32)rx_atmsw_stats.congest_discard_clp0);
          printf("ATMsw Rx cong_discard_clp0 = %#x%x\n",\
          (WP_U32)(rx_atmsw_stats.congest_discard_clp1 >> 32),\
          (WP_U32)rx_atmsw_stats.congest_discard_clp1);
          printf("\n");
         } else if (WPI_HANDLE_TX_CHANNEL_P(handle)) {
          status = WP_ChannelStatistics(handle, &tx_atmsw_stats);
          terminate_on_error(handle, "WP_ChannelStatistics() WP_ATM_ATMSW");
          printf("\nATMSW Tx Statistics \n");
          printf("ATMsw Tx clp0 = %#x%x\n",\
          (WP_U32)(tx_atmsw_stats.tx_clp0 >> 32),\
         (WP_U32)tx_atmsw_stats.tx_clp0);
          printf("ATMsw Tx clp1 = %#x%x\n",\
          (WP_U32)(tx_atmsw_stats.tx_clp1 >> 32),\
         (WP_U32)tx_atmsw_stats.tx_clp1);
          printf("ATMsw Tx efci = %#x%x\n",\
          (WP_U32)(tx_atmsw_stats.tx_efci >> 32),\
          (WP_U32)tx_atmsw_stats.tx_efci);
          printf("ATMsw Tx aal5frames = %#x%x\n",\
          (WP_U32)(tx_atmsw_stats.tx_aal5frames >> 32),\
          (WP_U32)tx_atmsw_stats.tx_aal5frames);
          printf("\n");
        }
        break;
  }
  return;
}

void printPOSDeviceStats(WP_handle handle) {
  WP_status status;
  WP_stats_upi_hspos hspos_stats = {0};

  status = WP_DeviceStatistics(handle, &hspos_stats);
  terminate_on_error(status, "WP_DeviceStatistics()");

  printf("\nDevice stats \n");
  printf("Rx Host Frames = %#x%x\n",\
          (WP_U32)(hspos_stats.rx_host_frames>>32),\
          (WP_U32)hspos_stats.rx_host_frames);
  printf("Rx IW frames = %#x%x\n",\
          (WP_U32)(hspos_stats.rx_iw_frames>>32),\
          (WP_U32)hspos_stats.rx_iw_frames);
  printf("Rx error fbp underrun = %#x%x\n",\
          (WP_U32)(hspos_stats.rx_err_fbp_underrun>>32),\
          (WP_U32)hspos_stats.rx_err_fbp_underrun);
  printf("Rx error MRU = %#x%x\n",\
          (WP_U32)(hspos_stats.rx_err_mru>>32),\
          (WP_U32)hspos_stats.rx_err_mru);
  printf("Rx error SDU = %#x%x\n",\
          (WP_U32)(hspos_stats.rx_err_sdu>>32),\
          (WP_U32)hspos_stats.rx_err_sdu);
  printf("Rx error state machine = %#x%x\n",\
          (WP_U32)(hspos_stats.rx_err_sm>>32),\
          (WP_U32)hspos_stats.rx_err_sm);
  printf("Rx error parity = %#x%x\n",\
          (WP_U32)(hspos_stats.rx_err_parity>>32),\
          (WP_U32)hspos_stats.rx_err_parity);
  printf("Tx frames = %#x%x\n",\
          (WP_U32)(hspos_stats.tx_frames>>32),\
          (WP_U32)hspos_stats.tx_frames);
}


void printEnetStats(WP_handle hs_device)
{
   WP_stats_enet enet_stats;
   WP_status status;
   memset (&enet_stats,0,sizeof(enet_stats));
   status = WP_DeviceStatistics(hs_device, &enet_stats);
   if (status)
   {
      printf("Error Reading Enet Stat\n");
      return;
   }
   printf("----------------------------------------------------\n");

   printf ("ENET Port Statistics (HW)\n");


   printf ("TxRx Frame  64:              %#x%x\n",
           (WP_U32)(enet_stats.txrx_frames_64 >> 32),
           ((WP_U32)enet_stats.txrx_frames_64));

   printf ("TxRx Frame 127:              %#x%x\n",
           (WP_U32)(enet_stats.txrx_frames_127 >> 32),
           ((WP_U32)enet_stats.txrx_frames_127));

   printf ("TxRx Frame 255:              %#x%x\n",
           (WP_U32)(enet_stats.txrx_frames_255>> 32),
           ((WP_U32)enet_stats.txrx_frames_255));

   printf ("TxRx Frame 1023:             %#x%x\n",
           (WP_U32)(enet_stats.txrx_frames_1023>> 32),
           ((WP_U32)enet_stats.txrx_frames_1023));

   printf ("TxRx Frame 1518:             %#x%x\n",
           (WP_U32)(enet_stats.txrx_frames_1518>> 32),
           ((WP_U32)enet_stats.txrx_frames_1518));

   printf ("TxRx Frame 1522:             %#x%x\n",
           (WP_U32)(enet_stats.txrx_frames_1522>> 32),
           ((WP_U32)enet_stats.txrx_frames_1522));

   printf ("Rx Bytes:                    %#x%x\n",
           (WP_U32)(enet_stats.rx_bytes>> 32),
           ((WP_U32)enet_stats.rx_bytes));

   printf ("Rx Packets:                  %#x%x\n",
           (WP_U32)(enet_stats.rx_packets>> 32),
           ((WP_U32)enet_stats.rx_packets));

   printf ("Rx Error FCS:                %#x%x\n",
           (WP_U32)(enet_stats.rx_err_fcs>> 32),
           ((WP_U32)enet_stats.rx_err_fcs));

   printf ("Rx Multicast:                %#x%x\n",
           (WP_U32)(enet_stats.rx_multicast>> 32),
           ((WP_U32)enet_stats.rx_multicast));

   printf ("Rx Broadcast:                %#x%x\n",
           (WP_U32)(enet_stats.rx_broadcast>> 32),
           ((WP_U32)enet_stats.rx_broadcast));

   printf ("Rx Mac Control:              %#x%x\n",
           (WP_U32)(enet_stats.rx_mac_control>> 32),
           ((WP_U32)enet_stats.rx_mac_control));

   printf ("Rx Mac Pause:                %#x%x\n",
           (WP_U32)(enet_stats.rx_mac_pause>> 32),
           ((WP_U32)enet_stats.rx_mac_pause));

   printf ("Rx Mac Unknown:              %#x%x\n",
           (WP_U32)(enet_stats.rx_mac_unknown>> 32),
           ((WP_U32)enet_stats.rx_mac_unknown));

   printf ("Rx Error Alignment:          %#x%x\n",
           (WP_U32)(enet_stats.rx_err_alignment>> 32),
           ((WP_U32)enet_stats.rx_err_alignment));

   printf ("Rx Error LEN:                %#x%x\n",
           (WP_U32)(enet_stats.rx_err_length>> 32),
           ((WP_U32)enet_stats.rx_err_length));

   printf ("Rx Error Code:               %#x%x\n",
           (WP_U32)(enet_stats.rx_err_code>> 32),
           ((WP_U32)enet_stats.rx_err_code));

   printf ("Rx False Carrier:            %#x%x\n",
           (WP_U32)(enet_stats.rx_false_carrier>> 32),
           ((WP_U32)enet_stats.rx_false_carrier));

   printf ("Rx Undersize:                %#x%x\n",
           (WP_U32)(enet_stats.rx_undersize>> 32),
           ((WP_U32)enet_stats.rx_undersize));

   printf ("Rx Oversize:                 %#x%x\n",
           (WP_U32)(enet_stats.rx_oversize>> 32),
           ((WP_U32)enet_stats.rx_oversize));

   printf ("Rx Fragments:                %#x%x\n",
           (WP_U32)(enet_stats.rx_fragments>> 32),
           ((WP_U32)enet_stats.rx_fragments));
   printf ("Rx Jabber:                   %#x%x\n",
           (WP_U32)(enet_stats.rx_jabber>> 32),
           ((WP_U32)enet_stats.rx_jabber));
   printf ("Rx Dropped:                  %#x%x\n",
           (WP_U32)(enet_stats.rx_dropped>> 32),
           ((WP_U32)enet_stats.rx_dropped));

   printf ("Tx Bytes:                    %#x%x\n",
           (WP_U32)(enet_stats.tx_bytes>> 32),
           ((WP_U32)enet_stats.tx_bytes));

   printf ("Tx Packets:                  %#x%x\n",
           (WP_U32)(enet_stats.tx_packets>> 32),
           ((WP_U32)enet_stats.tx_packets));

   printf ("Tx Multicast:                %#x%x\n",
           (WP_U32)(enet_stats.tx_multicast>> 32),
           ((WP_U32)enet_stats.tx_multicast));

   printf ("Tx Broadcast:                %#x%x\n",
           (WP_U32)(enet_stats.tx_broadcast>> 32),
           ((WP_U32)enet_stats.tx_broadcast));

   printf ("Tx Mac Pause:                %#x%x\n",
           (WP_U32)(enet_stats.tx_mac_pause>> 32),
           ((WP_U32)enet_stats.tx_mac_pause));

   printf ("Tx Defer:                    %#x%x\n",
           (WP_U32)(enet_stats.tx_defer>> 32),
           ((WP_U32)enet_stats.tx_defer));

   printf ("Tx Exess Defer:              %#x%x\n",
           (WP_U32)(enet_stats.tx_excess_defer>> 32),
           ((WP_U32)enet_stats.tx_excess_defer));

   printf ("Tx Single Collision:         %#x%x\n",
           (WP_U32)(enet_stats.tx_single_collision>> 32),
           ((WP_U32)enet_stats.tx_single_collision));

   printf ("Tx Multi Collision:          %#x%x\n",
           (WP_U32)(enet_stats.tx_multi_collision>> 32),
           ((WP_U32)enet_stats.tx_multi_collision));

   printf ("Tx Late Collision:           %#x%x\n",
           (WP_U32)(enet_stats.tx_late_collision>> 32),
           ((WP_U32)enet_stats.tx_late_collision));

   printf ("Tx Excess Collision:         %#x%x\n",
           (WP_U32)(enet_stats.tx_excess_collision>> 32),
           ((WP_U32)enet_stats.tx_excess_collision));

   printf ("Tx No Collision:             %#x%x\n",
           (WP_U32)(enet_stats.tx_no_collision>> 32),
           ((WP_U32)enet_stats.tx_no_collision));

   printf ("Tx Mac Pause Honored:        %#x%x\n",
           (WP_U32)(enet_stats.tx_mac_pause_honored>> 32),
           ((WP_U32)enet_stats.tx_mac_pause_honored));

   printf ("Tx Dropped:                  %#x%x\n",
           (WP_U32)(enet_stats.tx_dropped>> 32),
           ((WP_U32)enet_stats.tx_dropped));

   printf ("Tx Jabber:                   %#x%x\n",
           (WP_U32)(enet_stats.tx_jabber>> 32),
           ((WP_U32)enet_stats.tx_jabber));

   printf ("Tx Errors FCS:               %#x%x\n",
           (WP_U32)(enet_stats.tx_err_fcs>> 32),
           ((WP_U32)enet_stats.tx_err_fcs));

   printf ("Tx Control:                  %#x%x\n",
           (WP_U32)(enet_stats.tx_control>> 32),
           ((WP_U32)enet_stats.tx_control));
   printf ("Tx Oversize:                 %#x%x\n",
           (WP_U32)(enet_stats.tx_oversize>> 32),
           ((WP_U32)enet_stats.tx_oversize));

   printf ("Tx Undersize:                %#x%x\n",
           (WP_U32)(enet_stats.tx_undersize>> 32),
           ((WP_U32)enet_stats.tx_undersize));

   printf ("Tx Fragments:                %#x%x\n",
           (WP_U32)(enet_stats.tx_fragments>> 32),
           ((WP_U32)enet_stats.tx_fragments));

   printf ("\n");
   printf ("HS ENET Device Statistics (DPS)\n");

   printf ("Rx Host Frames:              %#x%x\n",
           (WP_U32)(enet_stats.rx_host_frames>> 32),
           ((WP_U32)enet_stats.rx_host_frames));
   printf ("Rx Iw Frames:                %#x%x\n",
           (WP_U32)(enet_stats.rx_iw_frames>> 32),
           ((WP_U32)enet_stats.rx_iw_frames));
   printf ("Rx Error Host Full:          %#x%x\n",
           (WP_U32)(enet_stats.rx_err_host_full>> 32),
           ((WP_U32)enet_stats.rx_err_host_full));
   printf ("Rx Error Fbp Underrun:       %#x%x\n",
           (WP_U32)(enet_stats.rx_err_fbp_underrun>> 32),
           ((WP_U32)enet_stats.rx_err_fbp_underrun));
   printf ("Rx Error Nonvalid Mac:       %#x%x\n",
           (WP_U32)(enet_stats.rx_err_nonvalid_mac>> 32),
           ((WP_U32)enet_stats.rx_err_nonvalid_mac));
   printf ("Rx Error Mru:                %#x%x\n",
           (WP_U32)(enet_stats.rx_err_mru>> 32),
           ((WP_U32)enet_stats.rx_err_mru));
   printf ("Rx Error Sdu:                %#x%x\n",
           (WP_U32)(enet_stats.rx_err_sdu>> 32),
           ((WP_U32)enet_stats.rx_err_sdu));
   printf ("Rx Error Underrun:           %#x%x\n",
           (WP_U32)(enet_stats.tx_err_underrun>> 32),
           ((WP_U32)enet_stats.tx_err_underrun));
   printf ("Rx Error Overrun:            %#x%x\n",
           (WP_U32)(enet_stats.rx_err_overrun>> 32),
           ((WP_U32)enet_stats.rx_err_overrun));
   printf ("Tx Frames:                   %#x%x\n",
           (WP_U32)(enet_stats.tx_frames>> 32),
           ((WP_U32)enet_stats.tx_frames));
   printf("----------------------------------------------------\n");

}
