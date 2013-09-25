/*************************************************************
 * (C) Copyright 2001-2010, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ************************************************************/

#ifndef _STATS_C_
#define _STATS_C_

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
  printf("tx_cells= %#x%x\n",\
         (WP_U32)(atm_upi_stats.ext_stats.tx_cells >> 32),\
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


void printTdmAtmDeviceStats(WP_handle handle) {
  WP_status status;
  WP_stats_tdi_atm atm_tdi_stats = {0};

  status = WP_DeviceStatistics(handle, &atm_tdi_stats);
  terminate_on_error(handle, "WP_DeviceStatistics()");
  printf("\n");
  printf("TDM Atm Device Statistics:\n");
  printf("\n");
  printf("Rx HEC Error= %#x%x\n",(WP_U32)(atm_tdi_stats.rx_err_hec >> 32),\
         (WP_U32)atm_tdi_stats.rx_err_hec);
  printf("Rx address mismatch= %#x%x\n",(WP_U32)(atm_tdi_stats.rx_err_addr_mismatch >> 32),\
         (WP_U32)atm_tdi_stats.rx_err_addr_mismatch);
  printf("Rx overrun error= %#x%x\n",(WP_U32)(atm_tdi_stats.rx_err_overrun >> 32),\
         (WP_U32)atm_tdi_stats.rx_err_overrun);
  printf("Rx out of sync error= %#x%x\n",(WP_U32)(atm_tdi_stats.rx_out_of_sync >> 32),\
         (WP_U32)atm_tdi_stats.rx_out_of_sync);
  printf("Rx enter sync= %#x%x\n",(WP_U32)(atm_tdi_stats.rx_enter_sync >> 32),\
         (WP_U32)atm_tdi_stats.rx_enter_sync);
  printf("Rx cell count= %#x%x\n",(WP_U32)(atm_tdi_stats.ext_stats.rx_cells >> 32),\
         (WP_U32)atm_tdi_stats.ext_stats.rx_cells);
  printf("rx_clp0_cells= %#x%x\n",(WP_U32)(atm_tdi_stats.ext_stats.rx_clp0_cells >> 32),\
         (WP_U32)atm_tdi_stats.ext_stats.rx_clp0_cells);
  printf("rx_clp1_cells= %#x%x\n",(WP_U32)(atm_tdi_stats.ext_stats.rx_clp1_cells >> 32),\
         (WP_U32)atm_tdi_stats.ext_stats.rx_clp1_cells);
  printf("rx_gfc_cells= %#x%x\n",(WP_U32)(atm_tdi_stats.ext_stats.rx_gfc_cells >> 32),\
         (WP_U32)atm_tdi_stats.ext_stats.rx_gfc_cells);
  printf("rx_oam_cells= %#x%x\n",(WP_U32)(atm_tdi_stats.ext_stats.rx_oam_cells >> 32),\
         (WP_U32)atm_tdi_stats.ext_stats.rx_oam_cells);
  printf("rx_rm_cells= %#x%x\n",(WP_U32)(atm_tdi_stats.ext_stats.rx_rm_cells >> 32),\
         (WP_U32)atm_tdi_stats.ext_stats.rx_rm_cells);
  printf("rx_last_unknown_addr= %#x%x\n",\
         (WP_U32)(atm_tdi_stats.ext_stats.rx_last_unknown_addr >> 32),\
         (WP_U32)atm_tdi_stats.ext_stats.rx_last_unknown_addr);
  printf("tx_cells= %#x%x\n",(WP_U32)(atm_tdi_stats.ext_stats.tx_cells >> 32),\
         (WP_U32)atm_tdi_stats.ext_stats.tx_cells);
  printf("tx_clp0_cells= %#x%x\n",(WP_U32)(atm_tdi_stats.ext_stats.tx_clp0_cells >> 32),\
         (WP_U32)atm_tdi_stats.ext_stats.tx_clp0_cells);
  printf("tx_clp1_cells= %#x%x\n",(WP_U32)(atm_tdi_stats.ext_stats.tx_clp1_cells >> 32),\
         (WP_U32)atm_tdi_stats.ext_stats.tx_clp1_cells);
  printf("tx_efci_cells= %#x%x\n",(WP_U32)(atm_tdi_stats.ext_stats.tx_efci_cells >> 32),\
         (WP_U32)atm_tdi_stats.ext_stats.tx_efci_cells);
  printf("tx_rm_cells= %#x%x\n",(WP_U32)(atm_tdi_stats.ext_stats.tx_rm_cells >> 32),\
         (WP_U32)atm_tdi_stats.ext_stats.tx_rm_cells);
  printf("tx_oam_cells= %#x%x\n",(WP_U32)(atm_tdi_stats.ext_stats.tx_oam_cells >> 32),\
         (WP_U32)atm_tdi_stats.ext_stats.tx_oam_cells);
  printf("tx_idle_cells= %#x%x\n",(WP_U32)(atm_tdi_stats.ext_stats.tx_idle_cells >> 32),\
         (WP_U32)atm_tdi_stats.ext_stats.tx_idle_cells);
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
               (WP_U32)(rx_aal0_stats.rx_cells >> 32), (WP_U32)rx_aal0_stats.rx_cells);
        printf("rx_err_crc= %#x%x\n",\
               (WP_U32)(rx_aal0_stats.err_crc >> 32), (WP_U32)rx_aal0_stats.err_crc);
        printf("iw_cells= %#x%x\n",\
               (WP_U32)(rx_aal0_stats.iw_cells >> 32), (WP_U32)rx_aal0_stats.iw_cells);
        printf("err_underflow= %#x%x\n",\
               (WP_U32)(rx_aal0_stats.err_underflow >> 32), (WP_U32)rx_aal0_stats.err_underflow);
        printf("\n");
      } else if (WPI_HANDLE_TX_CHANNEL_P(handle)) {
        status = WP_ChannelStatistics(handle, &tx_aal0_stats);
        terminate_on_error(handle, "WP_ChannelStatistics() WP_ATM_AAL0");
        printf("Atm Tx Channel Statistics:\n");
        printf("\n");
        printf("tx_cells= %#x%x\n",\
               (WP_U32)(tx_aal0_stats.tx_cells >> 32), (WP_U32)tx_aal0_stats.tx_cells);
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
               (WP_U32)(rx_aal2_stats.err_cps_event >> 32), (WP_U32)rx_aal2_stats.err_cps_event);
        printf("CPS Protocol errors= %#x%x\n",\
               (WP_U32)(rx_aal2_stats.err_protocol >> 32), (WP_U32)rx_aal2_stats.err_protocol);
        printf("SS Length errors= %#x%x\n",\
               (WP_U32)(rx_aal2_stats.err_ss_length >> 32), (WP_U32)rx_aal2_stats.err_ss_length);
        printf("SS CRC errors= %#x%x\n",\
               (WP_U32)(rx_aal2_stats.err_ss_crc>> 32),\
               (WP_U32)rx_aal2_stats.err_ss_crc);
        printf("SS Underflow errors= %#x%x\n",\
               (WP_U32)(rx_aal2_stats.err_ss_underflow>> 32),\
               (WP_U32)rx_aal2_stats.err_ss_underflow);
        printf("SS CPS errors= %#x%x\n",\
               (WP_U32)(rx_aal2_stats.err_ss_cps>> 32), (WP_U32)rx_aal2_stats.err_ss_cps);
        printf("SW CPS errors= %#x%x\n",\
               (WP_U32)(rx_aal2_stats.err_sw_cps>> 32), (WP_U32)rx_aal2_stats.err_sw_cps);
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
          printf("rx_frames= %#x%x\n",\
               (WP_U32)(rx_aal5_stats.rx_frames >> 32),\
                (WP_U32)rx_aal5_stats.rx_frames);
          printf("about_frames= %#x%x\n",\
               (WP_U32)(rx_aal5_stats.aborted_frames >> 32),\
                (WP_U32)rx_aal5_stats.aborted_frames);
          printf("err_crc= %#x%x\n",\
               (WP_U32)(rx_aal5_stats.err_crc >> 32),\
                (WP_U32)rx_aal5_stats.err_crc);
          printf("err_size= %#x%x\n",\
               (WP_U32)(rx_aal5_stats.err_size >> 32),\
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
               (WP_U32)(rx_aal5_stats.rx_clp1_cells>> 32),\
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
                (WP_U32)(rx_atmsw_stats.rx_clp0 >> 32),(WP_U32)rx_atmsw_stats.rx_clp0);
          printf("ATMsw Rx clp1 = %#x%x\n",\
                (WP_U32)(rx_atmsw_stats.rx_clp1 >> 32),(WP_U32)rx_atmsw_stats.rx_clp1);
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

#endif
