/*******************************************************************************
 * (C) Copyright 2001-2008, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ************************************************************/

/*******************************************************************************
 *
 * Module: AlexR_3.0_Statistics.c
 * Purpose: Provide Wrappers for Statistics functions
 *
 * Change History:
 * +------------+--------------------+------------------------------------------
 * | Date       | By                 | Description
 * +------------+--------------------+------------------------------------------
 * | 25/11/2007 | Alexander Rahlis   | All Statistics for Enet, ATM, POS.
 * +------------+--------------------+------------------------------------------
 * |            |                    |
 * +------------+--------------------+------------------------------------------
 *
 ******************************************************************************/
#ifdef __cplusplus
extern "C"{
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef __linux__
#include <unistd.h>
#endif

// #include "wp_types.h"
#include "wp_wddi.h"

extern int wp3_uart_printf(const char *fmt, ...);

#ifdef __cplusplus
}
#endif
//void WPE_App_print_U64 (WP_CHAR *name, WP_U64 *ptr);
//void WPE_TerminateOnErrorL(WP_handle handle, WP_CHAR *s);
//void WPE_DisplayAtmTxChannelStat(WP_handle channel);
//void WPE_App_print_stats_enet (WP_stats_enet *var_ptr);
void WPE_DisplayEnetDeviceStat(WP_handle dev_enet);
//void WPE_DisplayHSPOSDeviceStat(WP_handle dev_pos);
//void WPE_DisplayPOSDeviceStat(WP_handle dev_pos);
//void WPE_DisplayAtmDeviceStat(WP_handle dev_atm);
void WPE_DisplayFlowAggStats(WP_handle agg);
void WPE_DisplayIWPortStats(WP_handle iw_port_handle);
void WPE_DisplayIWSystemStats(WP_handle port_handle);
//void WPE_DisplayAtmRxChannelStat(WP_handle channel);

typedef union StatField {
      WP_U64 field;
      struct {
            WP_U32 high;
            WP_U32 low;
      } part;
} WPE_StatField;


static void WPE_App_print_U64 (WP_CHAR *name, WP_U64 *ptr)
{
   wp3_uart_printf ("  %-36s: 0x%08x%08x\n", name,
           (WP_U32) (*ptr >> 32), (WP_U32) (*ptr & 0xffffffff));
}


static void WPE_TerminateOnErrorL(WP_handle handle, WP_CHAR *s)
{
   if (WP_ERROR_P(handle)) {
      wp3_uart_printf("Test Abort %s %s 0x%x\n", s, WP_error_name[WP_ERROR(handle)], handle);
      wp3_uart_printf("Test Failed\n");
      /* Release WDDI */
      WP_DriverRelease();
      exit(1);
   }
}


/*****************************************************************************************/
/*****************************************************************************************/
/**************************** Stats Functions ********************************************/
/*****************************************************************************************/
/*****************************************************************************************/



//static void WPE_DisplayAtmTxChannelStat(WP_handle channel)
//{
//
//   WP_stats_aal5_tx  aal5_tx;
//   WP_status       status;
//
//   memset (&aal5_tx,0, sizeof(WP_stats_aal5_tx));
//
//   /* Get Tx channel statistics */
//   status = WP_ChannelStatistics(channel, &aal5_tx);
//   WPE_TerminateOnErrorL(status, "WP_ChannelStatistics");
//
//   wp3_uart_printf("AAL5 TX CHANNEL STATISTICS:\n");
//   wp3_uart_printf("----------------------------------\n");
//
//   wp3_uart_printf("Transmitted frames         = %#8.8x%8.8x\n",
//          (WP_U32)(aal5_tx.tx_frames >> 32),
//          (WP_U32)(aal5_tx.tx_frames & 0xffff));
//   wp3_uart_printf("Transmitted tx_clp0_cells  = %#8.8x%8.8x\n",
//          (WP_U32)(aal5_tx.tx_clp0_cells >> 32),
//          (WP_U32)(aal5_tx.tx_clp0_cells & 0xffff));
//   wp3_uart_printf("Transmitted tx_clp1_cells  = %#8.8x%8.8x\n",
//          (WP_U32)(aal5_tx.tx_clp1_cells >> 32),
//          (WP_U32)(aal5_tx.tx_clp1_cells & 0xffff));
//   wp3_uart_printf("Transmitted tx_bytes       = %#8.8x%8.8x\n",
//          (WP_U32)(aal5_tx.tx_bytes >> 32),
//          (WP_U32)(aal5_tx.tx_bytes & 0xffff));
//   wp3_uart_printf("Transmitted tx_clp1_frames = %#8.8x%8.8x\n",
//          (WP_U32)(aal5_tx.tx_clp1_frames >> 32),
//          (WP_U32)(aal5_tx.tx_clp1_frames & 0xffff));
//
//
//   wp3_uart_printf("\n\n");
//}
//
//
//static void WPE_DisplayAtmRxChannelStat(WP_handle channel)
//{
//
//   WP_stats_aal5_rx  aal5_rx;
//   WP_status         status;
//
//   memset (&aal5_rx,0, sizeof(WP_stats_aal5_rx));
//
//   /* Get Rx channel statistics */
//   status = WP_ChannelStatistics(channel, &aal5_rx);
//   WPE_TerminateOnErrorL(status, "WP_ChannelStatistics");
//   wp3_uart_printf("AAL5 RX CHANNEL STATISTICS:\n");
//   wp3_uart_printf("----------------------------------\n");
//
//
//   wp3_uart_printf("rx_frames           = %#8.8x%8.8x\n",
//          (WP_U32)(aal5_rx.rx_frames >> 32),
//          (WP_U32)(aal5_rx.rx_frames & 0xffff));
//   wp3_uart_printf("aborted_frames      = %#8.8x%8.8x\n",
//          (WP_U32)(aal5_rx.aborted_frames >> 32),
//          (WP_U32)(aal5_rx.aborted_frames & 0xffff));
//   wp3_uart_printf("err_crc             = %#8.8x%8.8x\n",
//          (WP_U32)(aal5_rx.err_crc >> 32),
//          (WP_U32)(aal5_rx.err_crc & 0xffff));
//   wp3_uart_printf("err_size            = %#8.8x%8.8x\n",
//          (WP_U32)(aal5_rx.err_size >> 32),
//          (WP_U32)(aal5_rx.err_size & 0xffff));
//   wp3_uart_printf("err_underflow       = %#8.8x%8.8x\n",
//          (WP_U32)(aal5_rx.err_underflow >> 32),
//          (WP_U32)(aal5_rx.err_underflow & 0xffff));
//   wp3_uart_printf("err_maxsdu          = %#8.8x%8.8x\n",
//          (WP_U32)(aal5_rx.err_maxsdu >> 32),
//          (WP_U32)(aal5_rx.err_maxsdu & 0xffff));
//   wp3_uart_printf("err_iw_underrun     = %#8.8x%8.8x\n",
//          (WP_U32)(aal5_rx.err_iw_underrun >> 32),
//          (WP_U32)(aal5_rx.err_iw_underrun & 0xffff));
//   wp3_uart_printf("err_iw_maxmru       = %#8.8x%8.8x\n",
//          (WP_U32)(aal5_rx.err_iw_maxmru >> 32),
//          (WP_U32)(aal5_rx.err_iw_maxmru & 0xffff));
//   wp3_uart_printf("err_cpi             = %#8.8x%8.8x\n",
//          (WP_U32)(aal5_rx.err_cpi >> 32),
//          (WP_U32)(aal5_rx.err_cpi & 0xffff));
//   wp3_uart_printf("clp_frames          = %#8.8x%8.8x\n",
//          (WP_U32)(aal5_rx.clp_frames >> 32),
//          (WP_U32)(aal5_rx.clp_frames & 0xffff));
//   wp3_uart_printf("l3_iw_frames        = %#8.8x%8.8x\n",
//          (WP_U32)(aal5_rx.l3_iw_frames >> 32),
//          (WP_U32)(aal5_rx.l3_iw_frames & 0xffff));
//   wp3_uart_printf("err_ras_timeout     = %#8.8x%8.8x\n",
//          (WP_U32)(aal5_rx.err_ras_timeout >> 32),
//          (WP_U32)(aal5_rx.err_ras_timeout & 0xffff));
//   wp3_uart_printf("rx_clp0_cells       = %#8.8x%8.8x\n",
//          (WP_U32)(aal5_rx.rx_clp0_cells >> 32),
//          (WP_U32)(aal5_rx.rx_clp0_cells & 0xffff));
//   wp3_uart_printf("rx_clp1_cells       = %#8.8x%8.8x\n",
//          (WP_U32)(aal5_rx.rx_clp1_cells >> 32),
//          (WP_U32)(aal5_rx.rx_clp1_cells & 0xffff));
//
//   wp3_uart_printf("rx_bytes            = %#8.8x%8.8x\n",
//          (WP_U32)(aal5_rx.rx_bytes >> 32),
//          (WP_U32)(aal5_rx.rx_bytes & 0xffff));
//
//   wp3_uart_printf("rx_clp1_cells       = %#8.8x%8.8x\n",
//          (WP_U32)(aal5_rx.rx_clp1_cells >> 32),
//          (WP_U32)(aal5_rx.rx_clp1_cells & 0xffff));
//
//   wp3_uart_printf("\n\n");
//
//}


static void WPE_App_print_stats_enet (WP_stats_enet *var_ptr)
{
   wp3_uart_printf ("  WP_stats_enet\n");
   /*WPE_App_print_U64 ("txrx_frames_64", &var_ptr->txrx_frames_64);
     WPE_App_print_U64 ("txrx_frames_127", &var_ptr->txrx_frames_127);
     WPE_App_print_U64 ("txrx_frames_255", &var_ptr->txrx_frames_255);
     WPE_App_print_U64 ("txrx_frames_511", &var_ptr->txrx_frames_511);
     WPE_App_print_U64 ("txrx_frames_1023", &var_ptr->txrx_frames_1023);
     WPE_App_print_U64 ("txrx_frames_1518", &var_ptr->txrx_frames_1518);
     WPE_App_print_U64 ("txrx_frames_1522", &var_ptr->txrx_frames_1522);*/
//   WPE_App_print_U64 ("rx_bytes", &var_ptr->rx_bytes);
   WPE_App_print_U64 ("rx_packets", &var_ptr->rx_packets);
   WPE_App_print_U64 ("rx_multicast", &var_ptr->rx_multicast);
   WPE_App_print_U64 ("rx_broadcast", &var_ptr->rx_broadcast);
   WPE_App_print_U64 ("rx_err_fcs", &var_ptr->rx_err_fcs);
//   WPE_App_print_U64 ("rx_mac_control", &var_ptr->rx_mac_control);
//   WPE_App_print_U64 ("rx_mac_pause", &var_ptr->rx_mac_pause);
   WPE_App_print_U64 ("rx_mac_unknown", &var_ptr->rx_mac_unknown);
   WPE_App_print_U64 ("rx_err_alignment", &var_ptr->rx_err_alignment);
   WPE_App_print_U64 ("rx_err_length", &var_ptr->rx_err_length);
   WPE_App_print_U64 ("rx_err_code", &var_ptr->rx_err_code);
   WPE_App_print_U64 ("rx_false_carrier", &var_ptr->rx_false_carrier);
   WPE_App_print_U64 ("rx_undersize", &var_ptr->rx_undersize);
   WPE_App_print_U64 ("rx_oversize", &var_ptr->rx_oversize);
//   WPE_App_print_U64 ("rx_fragments", &var_ptr->rx_fragments);
   WPE_App_print_U64 ("rx_jabber", &var_ptr->rx_jabber);
   WPE_App_print_U64 ("rx_dropped", &var_ptr->rx_dropped);
//   WPE_App_print_U64 ("tx_bytes", &var_ptr->tx_bytes);
   WPE_App_print_U64 ("tx_packets", &var_ptr->tx_packets);
   WPE_App_print_U64 ("tx_multicast", &var_ptr->tx_multicast);
   WPE_App_print_U64 ("tx_broadcast", &var_ptr->tx_broadcast);
//   WPE_App_print_U64 ("tx_mac_pause", &var_ptr->tx_mac_pause);
//   WPE_App_print_U64 ("tx_defer", &var_ptr->tx_defer);
//   WPE_App_print_U64 ("tx_excess_defer", &var_ptr->tx_excess_defer);
//   WPE_App_print_U64 ("tx_single_collision", &var_ptr->tx_single_collision);
//   WPE_App_print_U64 ("tx_multi_collision", &var_ptr->tx_multi_collision);
//   WPE_App_print_U64 ("tx_late_collision", &var_ptr->tx_late_collision);
//   WPE_App_print_U64 ("tx_excess_collision", &var_ptr->tx_excess_collision);
//   WPE_App_print_U64 ("tx_no_collision", &var_ptr->tx_no_collision);
//   WPE_App_print_U64 ("tx_mac_pause_honored", &var_ptr->tx_mac_pause_honored);
   WPE_App_print_U64 ("tx_dropped", &var_ptr->tx_dropped);
   WPE_App_print_U64 ("tx_jabber", &var_ptr->tx_jabber);
   WPE_App_print_U64 ("tx_err_fcs", &var_ptr->tx_err_fcs);
//   WPE_App_print_U64 ("tx_control", &var_ptr->tx_control);
   WPE_App_print_U64 ("tx_oversize", &var_ptr->tx_oversize);
   WPE_App_print_U64 ("tx_undersize", &var_ptr->tx_undersize);
//   WPE_App_print_U64 ("tx_fragments", &var_ptr->tx_fragments);
   WPE_App_print_U64 ("rx_host_frames", &var_ptr->rx_host_frames);
   WPE_App_print_U64 ("rx_iw_frames", &var_ptr->rx_iw_frames);
   WPE_App_print_U64 ("rx_err_host_full",&var_ptr->rx_err_host_full);
   WPE_App_print_U64 ("rx_err_fbp_underrun",&var_ptr->rx_err_fbp_underrun);
   WPE_App_print_U64 ("rx_err_nonvalid_mac",&var_ptr->rx_err_nonvalid_mac);
   WPE_App_print_U64 ("rx_err_mru", &var_ptr->rx_err_mru);
   WPE_App_print_U64 ("rx_err_sdu",&var_ptr->rx_err_sdu);
   WPE_App_print_U64 ("tx_err_underrun", &var_ptr->tx_err_underrun);
   WPE_App_print_U64 ("rx_err_overrun", &var_ptr->rx_err_overrun);
//   WPE_App_print_U64 ("tx_frames", &var_ptr->tx_frames);

   WPE_App_print_U64 ("rx_packets", &var_ptr->rx_packets);
   WPE_App_print_U64 ("tx_packets", &var_ptr->tx_packets);
   WPE_App_print_U64 ("tx_bytes", &var_ptr->tx_bytes);
   return;
}


void WPE_DisplayEnetDeviceStat(WP_handle dev_enet)
{   
   WP_stats_enet stats_enet;
   WP_status     status;

   memset(&stats_enet,0,sizeof(WP_stats_enet));

   if ( (status = WP_DeviceStatistics(dev_enet, &stats_enet)) ) {
      wp3_uart_printf("GetStatistics error Enet : %d\n",dev_enet);
      return;
   }
   WPE_App_print_stats_enet(&stats_enet);
   return;   
}

//static void WPE_DisplayHSPOSDeviceStat(WP_handle dev_pos)
//{   
//   WP_stats_upi_hspos  stats_upi;
//   WP_status           status;
//   WP_U32              temp1, temp2;
//
//   memset( &stats_upi, 0, sizeof(WP_stats_upi_hspos) );
//
//   if ( (status = WP_DeviceStatistics(dev_pos, &stats_upi)) ) {
//      wp3_uart_printf("GetStatistics error pos \n");
//      return;
//   }
//
//   wp3_uart_printf("\nStatistics for HS POS device:\n");
//   wp3_uart_printf ("==========================================================\n");
//
//   temp1 = stats_upi.rx_host_frames;
//   temp2 = stats_upi.rx_host_frames >> 32;
//   wp3_uart_printf("rx_host_frames:                  %.8x%.8x\n", temp2,temp1);
//
//   temp1 = stats_upi.rx_iw_frames;
//   temp2 = stats_upi.rx_iw_frames >> 32;
//   wp3_uart_printf("rx_iw_frames:                    %.8x%.8x\n", temp2,temp1);
//
//   temp1 = stats_upi.rx_err_host_full;
//   temp2 = stats_upi.rx_err_host_full >> 32;
//   wp3_uart_printf("rx_err_host_full:                %.8x%.8x\n", temp2,temp1);
//
//   temp1 = stats_upi.rx_err_fbp_underrun;
//   temp2 = stats_upi.rx_err_fbp_underrun >> 32;
//   wp3_uart_printf("rx_err_fbp_underrun:             %.8x%.8x\n", temp2,temp1);
//
//   temp1 = stats_upi.rx_err_mru;
//   temp2 = stats_upi.rx_err_mru >> 32;
//   wp3_uart_printf("rx_err_mru:                      %.8x%.8x\n", temp2,temp1);
//
//   temp1 = stats_upi.rx_err_sdu;
//   temp2 = stats_upi.rx_err_sdu >> 32;
//   wp3_uart_printf("rx_err_sdu:                      %.8x%.8x\n", temp2,temp1);
//
//   temp1 = stats_upi.rx_err_sm;
//   temp2 = stats_upi.rx_err_sm >> 32;
//   wp3_uart_printf("rx_err_sm:                       %.8x%.8x\n", temp2,temp1);
//
//   temp1 = stats_upi.rx_err_parity;
//   temp2 = stats_upi.rx_err_parity >> 32;
//   wp3_uart_printf("rx_err_parity:                   %.8x%.8x\n", temp2,temp1);
//
//   temp1 = stats_upi.rx_err_phy;
//   temp2 = stats_upi.rx_err_phy >> 32;
//   wp3_uart_printf("rx_err_phy:                      %.8x%.8x\n", temp2,temp1);
//
//   temp1 = stats_upi.tx_frames;
//   temp2 = stats_upi.tx_frames >> 32;
//   wp3_uart_printf("tx_frames:                       %.8x%.8x\n", temp2,temp1);
//
//}


//static void WPE_DisplayPOSDeviceStat(WP_handle dev_pos)
//{   
//   WP_stats_upi_hdlc   stats_upi;
//   WP_status           status;
//   WP_U32              temp1, temp2;
//
//   memset( &stats_upi, 0, sizeof(WP_stats_upi_hdlc) );
//
//   if ( (status = WP_DeviceStatistics(dev_pos, &stats_upi)) ) {
//      wp3_uart_printf("GetStatistics error pos \n");
//      return;
//   }
//
//   wp3_uart_printf("\nStatistics for POS device:\n");
//   wp3_uart_printf ("==========================================================\n");
//
//   temp1 = stats_upi.rx_frames;
//   temp2 = stats_upi.rx_frames >> 32;
//   wp3_uart_printf("rx_frames:                  %.8x%.8x\n", temp2,temp1);
//
//   temp1 = stats_upi.tx_frames;
//   temp2 = stats_upi.tx_frames >> 32;
//   wp3_uart_printf("tx_frames:                  %.8x%.8x\n", temp2,temp1);
//
//   temp1 = stats_upi.rx_err_crc;
//   temp2 = stats_upi.rx_err_crc >> 32;
//   wp3_uart_printf("rx_err_crc:                 %.8x%.8x\n", temp2,temp1);
//
//   temp1 = stats_upi.rx_err_phy;
//   temp2 = stats_upi.rx_err_phy >> 32;
//   wp3_uart_printf("rx_err_phy:                 %.8x%.8x\n", temp2,temp1);
//
//   temp1 = stats_upi.rx_err_addr_mismatch;
//   temp2 = stats_upi.rx_err_addr_mismatch >> 32;
//   wp3_uart_printf("rx_err_addr_mismatch:       %.8x%.8x\n", temp2,temp1);
//
//   temp1 = stats_upi.rx_err_buffer_overrun;
//   temp2 = stats_upi.rx_err_buffer_overrun >> 32;
//   wp3_uart_printf("rx_err_buffer_overrun:      %.8x%.8x\n", temp2,temp1);
//
//   temp1 = stats_upi.rx_err_overrun;
//   temp2 = stats_upi.rx_err_overrun >> 32;
//   wp3_uart_printf("rx_err_overrun:             %.8x%.8x\n", temp2,temp1);
//
//   temp1 = stats_upi.rx_err_sop_eop;
//   temp2 = stats_upi.rx_err_sop_eop >> 32;
//   wp3_uart_printf("rx_err_sop_eop:             %.8x%.8x\n", temp2,temp1);
//
//   temp1 = stats_upi.rx_err_parity;
//   temp2 = stats_upi.rx_err_parity >> 32;
//   wp3_uart_printf("rx_err_parity:              %.8x%.8x\n", temp2,temp1);
//
//   temp1 = stats_upi.rx_err_iw_buffer_underrun;
//   temp2 = stats_upi.rx_err_iw_buffer_underrun >> 32;
//   wp3_uart_printf("rx_err_iw_buffer_underrun:  %.8x%.8x\n", temp2,temp1);
//
//   temp1 = stats_upi.rx_err_iw_mru;
//   temp2 = stats_upi.rx_err_iw_mru >> 32;
//   wp3_uart_printf("rx_err_iw_mru:              %.8x%.8x\n", temp2,temp1);
//
//   temp1 = stats_upi.rx_err_maxsdu;
//   temp2 = stats_upi.rx_err_maxsdu >> 32;
//   wp3_uart_printf("rx_err_maxsdu:              %.8x%.8x\n", temp2,temp1);
//
//   temp1 = stats_upi.rx_iw_frames;
//   temp2 = stats_upi.rx_iw_frames >> 32;
//   wp3_uart_printf("rx_iw_frames:               %.8x%.8x\n", temp2,temp1);
//
//   temp1 = stats_upi.rx_bytes;
//   temp2 = stats_upi.rx_bytes >> 32;
//   wp3_uart_printf("rx_bytes:                   %.8x%.8x\n", temp2,temp1);
//
//   temp1 = stats_upi.rx_err_bytes;
//   temp2 = stats_upi.rx_err_bytes >> 32;
//   wp3_uart_printf("rx_err_bytes:               %.8x%.8x\n", temp2,temp1);
//
//   temp1 = stats_upi.tx_bytes;
//   temp2 = stats_upi.tx_bytes >> 32;
//   wp3_uart_printf("tx_bytes:                   %.8x%.8x\n", temp2,temp1);
//
//   temp1 = stats_upi.rx_err_abort;
//   temp2 = stats_upi.rx_err_abort >> 32;
//   wp3_uart_printf("rx_err_abort:               %.8x%.8x\n", temp2,temp1);
//
//   temp1 = stats_upi.rx_err_iw_l2_parse;
//   temp2 = stats_upi.rx_err_iw_l2_parse >> 32;
//   wp3_uart_printf("rx_err_iw_l2_parse:         %.8x%.8x\n", temp2,temp1);
//
//   wp3_uart_printf("\n");         
//
//   return;   
//}



//static void WPE_DisplayAtmDeviceStat(WP_handle dev_atm)
//{   
//   WP_stats_upi_atm    stats_upi   = {0};
//   WP_status           status;
//   WP_U32              temp1, temp2;
//
//   memset( &stats_upi, 0, sizeof(WP_stats_upi_atm) );
//
//   if ( (status = WP_DeviceStatistics(dev_atm, &stats_upi)) ) {
//      wp3_uart_printf("GetStatistics error atm \n");
//      return;
//   }
//
//   wp3_uart_printf("\nStatistics for ATM device:\n");
//   wp3_uart_printf ("==========================================================\n");
//
//   temp1 = stats_upi.rx_err_slc;
//   temp2 = stats_upi.rx_err_slc >> 32;
//   wp3_uart_printf("rx_err_slc:           %.8x%.8x\n", temp2,temp1);
//
//   temp1 = stats_upi.rx_err_parity;
//   temp2 = stats_upi.rx_err_parity >> 32;
//   wp3_uart_printf("rx_err_parity:        %.8x%.8x\n", temp2,temp1);
//
//   temp1 = stats_upi.rx_err_addr_mismatch;
//   temp2 = stats_upi.rx_err_addr_mismatch >> 32;
//   wp3_uart_printf("rx_err_addr_mismatch: %.8x%.8x\n", temp2,temp1);
//
//   temp1 = stats_upi.ext_stats.rx_cells;
//   temp2 = stats_upi.ext_stats.rx_cells >> 32;
//   wp3_uart_printf("rx_cells:             %.8x%.8x\n", temp2,temp1);
//
//   temp1 = stats_upi.ext_stats.rx_last_unknown_addr;
//   temp2 = stats_upi.ext_stats.rx_last_unknown_addr >> 32;
//   wp3_uart_printf("rx_last_unknown_addr: %.8x%.8x\n", temp2,temp1);
//
//   temp1 = stats_upi.ext_stats.rx_clp1_cells;
//   temp2 = stats_upi.ext_stats.rx_clp1_cells >> 32;
//   wp3_uart_printf("rx_clp1_cells:        %.8x%.8x\n", temp2,temp1);
//
//   temp1 = stats_upi.ext_stats.rx_gfc_cells;
//   temp2 = stats_upi.ext_stats.rx_gfc_cells >> 32;
//   wp3_uart_printf("rx_gfc_cells:         %.8x%.8x\n", temp2,temp1);
//
//   temp1 = stats_upi.ext_stats.tx_cells;
//   temp2 = stats_upi.ext_stats.tx_cells >> 32;
//   wp3_uart_printf("tx_cells:             %.8x%.8x\n", temp2,temp1);
//
//   temp1 = stats_upi.ext_stats.tx_clp1_cells;
//   temp2 = stats_upi.ext_stats.tx_clp1_cells >> 32;
//   wp3_uart_printf("tx_clp1_cells:        %.8x%.8x\n", temp2,temp1);
//
//   temp1 = stats_upi.ext_stats.tx_efci_cells;
//   temp2 = stats_upi.ext_stats.tx_efci_cells >> 32;
//   wp3_uart_printf("tx_efci_cells:        %.8x%.8x\n", temp2,temp1);
//
//   temp1 = stats_upi.ext_stats.rx_oam_cells;
//   temp2 = stats_upi.ext_stats.rx_oam_cells >> 32;
//   wp3_uart_printf("rx_oam_cells:         %.8x%.8x\n", temp2,temp1);
//
//   temp1 = stats_upi.ext_stats.rx_rm_cells;
//   temp2 = stats_upi.ext_stats.rx_rm_cells >> 32;
//   wp3_uart_printf("rx_rm_cells:          %.8x%.8x\n", temp2,temp1);
//
//   temp1 = stats_upi.ext_stats.tx_oam_cells;
//   temp2 = stats_upi.ext_stats.tx_oam_cells >> 32;
//   wp3_uart_printf("tx_oam_cells:         %.8x%.8x\n", temp2,temp1);
//
//   temp1 = stats_upi.ext_stats.tx_rm_cells;
//   temp2 = stats_upi.ext_stats.tx_rm_cells >> 32;
//   wp3_uart_printf("tx_rm_cells:          %.8x%.8x\n", temp2,temp1);
//
//   temp1 = stats_upi.ext_stats.rx_efci_cells;
//   temp2 = stats_upi.ext_stats.rx_efci_cells >> 32;
//   wp3_uart_printf("rx_efci_cells:        %.8x%.8x\n", temp2,temp1);
//
//   temp1 = stats_upi.ext_stats.tx_idle_cells;
//   temp2 = stats_upi.ext_stats.tx_idle_cells >> 32;
//   wp3_uart_printf("tx_idle_cells:        %.8x%.8x\n", temp2,temp1);
//
//   temp1 = stats_upi.ext_stats.rx_clp0_cells;
//   temp2 = stats_upi.ext_stats.rx_clp0_cells >> 32;
//   wp3_uart_printf("rx_clp0_cells:        %.8x%.8x\n", temp2,temp1);
//
//   temp1 = stats_upi.ext_stats.tx_clp0_cells;
//   temp2 = stats_upi.ext_stats.tx_clp0_cells >> 32;
//   wp3_uart_printf("tx_clp0_cells:        %.8x%.8x\n", temp2,temp1);
//
//   temp1 = stats_upi.ext_stats.rx_crc_err_cells;
//   temp2 = stats_upi.ext_stats.rx_crc_err_cells >> 32;
//   wp3_uart_printf("rx_crc_err_cells:     %.8x%.8x\n", temp2,temp1);
//
//   temp1 = stats_upi.ext_stats.rx_unsupported_cells;
//   temp2 = stats_upi.ext_stats.rx_unsupported_cells >> 32;
//   wp3_uart_printf("rx_unsupported_cells: %.8x%.8x\n", temp2,temp1);
//
//
//   wp3_uart_printf("\n");         
//
//
//   return;   
//}


void WPE_DisplayFlowAggStats(WP_handle agg)
{

   WP_iw_flow_stats iw_flow_stats;

   memset(&iw_flow_stats,0,sizeof(iw_flow_stats));

   WP_IwFlowStatistics(agg, WP_IW_FLOW_STAT, &iw_flow_stats);

   wp3_uart_printf ("\nAggregation flow statistics of flow (%d) (%x)\n", (agg & 0x00000fff),agg);
   wp3_uart_printf ("==========================================================\n");

   wp3_uart_printf ("Forward Packet:          %08x%08x\n",
           ((WPE_StatField*)&iw_flow_stats.forward_packet)->part.high,
           ((WPE_StatField*)&iw_flow_stats.forward_packet)->part.low);
   wp3_uart_printf( "FBP Drop Packets:        %08x%08x\n",
           ((WPE_StatField*)&iw_flow_stats.fbp_drop_packets)->part.high,
           ((WPE_StatField*)&iw_flow_stats.fbp_drop_packets)->part.low);
   wp3_uart_printf( "MTU Drop Packets:        %08x%08x\n",
           ((WPE_StatField*)&iw_flow_stats.mtu_drop_packets)->part.high,
           ((WPE_StatField*)&iw_flow_stats.mtu_drop_packets)->part.low);
   wp3_uart_printf( "TTL Drop Packets:        %08x%08x\n",
           ((WPE_StatField*)&iw_flow_stats.ttl_drop_packets)->part.high,
           ((WPE_StatField*)&iw_flow_stats.ttl_drop_packets)->part.low);
   wp3_uart_printf( "TX Queue Drop Packets:   %08x%08x\n",
           ((WPE_StatField*)&iw_flow_stats.tx_queue_drop_packets)->part.high,
           ((WPE_StatField*)&iw_flow_stats.tx_queue_drop_packets)->part.low);
   wp3_uart_printf( "MPLS Drop:               %08x%08x\n",
           ((WPE_StatField*)&iw_flow_stats.mpls_drop)->part.high,
           ((WPE_StatField*)&iw_flow_stats.mpls_drop)->part.low);
   wp3_uart_printf( "Denied Packets:          %08x%08x\n",
           ((WPE_StatField*)&iw_flow_stats.denied_packets)->part.high,
           ((WPE_StatField*)&iw_flow_stats.denied_packets)->part.low);
   wp3_uart_printf( "Group Filtered Packets:  %08x%08x\n",
           ((WPE_StatField*)&iw_flow_stats.group_filtered_packets)->part.high,
           ((WPE_StatField*)&iw_flow_stats.group_filtered_packets)->part.low);
   wp3_uart_printf( "Forwarded Bytes:         %08x%08x\n",
           ((WPE_StatField*)&iw_flow_stats.forwarded_bytes)->part.high,
           ((WPE_StatField*)&iw_flow_stats.forwarded_bytes)->part.low);
   wp3_uart_printf( "Gtp Bad Headers:         %08x%08x\n",
           ((WPE_StatField*)&iw_flow_stats.gtp_bad_headers)->part.high,
           ((WPE_StatField*)&iw_flow_stats.gtp_bad_headers)->part.low);
   wp3_uart_printf( "Bad Policer Packets:     %08x%08x\n",
           ((WPE_StatField*)&iw_flow_stats.policer_non_conforming_packets)->part.high,
           ((WPE_StatField*)&iw_flow_stats.policer_non_conforming_packets)->part.low);
   wp3_uart_printf ("Forward Packet:          %08x%08x\n",
           ((WPE_StatField*)&iw_flow_stats.forward_packet)->part.high,
           ((WPE_StatField*)&iw_flow_stats.forward_packet)->part.low);

   wp3_uart_printf( "TX Queue Drop Packets:   %08x%08x\n",
           ((WPE_StatField*)&iw_flow_stats.tx_queue_drop_packets)->part.high,
           ((WPE_StatField*)&iw_flow_stats.tx_queue_drop_packets)->part.low);
   wp3_uart_printf("\n");
}



void WPE_DisplayIWPortStats(WP_handle iw_port_handle)
{
   WP_bport_stats bport_stats;
   WP_handle status;

   wp3_uart_printf ("\nIW Port Statistics of Port \n");
   wp3_uart_printf ("==========================================================\n");

   memset(&bport_stats, 0, sizeof(WP_bport_stats));

   status = WP_IwPortStatistics(iw_port_handle,&bport_stats);
   WPE_TerminateOnErrorL(status, "WP_IwPortStatistics()");

   wp3_uart_printf("\n");

   wp3_uart_printf ("rx_valid_packets :                %08x%08x\n",
           ((WPE_StatField*)&bport_stats.rx_valid_packets)->part.high,
           ((WPE_StatField*)&bport_stats.rx_valid_packets)->part.low);
   wp3_uart_printf ("discard_vlan_acceptable_filter :  %08x%08x\n",
           ((WPE_StatField*)&bport_stats.discard_vlan_acceptable_filter)->part.high,
           ((WPE_StatField*)&bport_stats.discard_vlan_acceptable_filter)->part.low);
   wp3_uart_printf ("discard_ingress_filter :          %08x%08x\n",
           ((WPE_StatField*)&bport_stats.discard_ingress_filter)->part.high,
           ((WPE_StatField*)&bport_stats.discard_ingress_filter)->part.low);
   wp3_uart_printf ("discard_bridge_classifier :       %08x%08x\n",
           ((WPE_StatField*)&bport_stats.discard_bridge_classifier)->part.high,
           ((WPE_StatField*)&bport_stats.discard_bridge_classifier)->part.low);
   wp3_uart_printf ("discard_unk_macsa :               %08x%08x\n",
           ((WPE_StatField*)&bport_stats.discard_unk_macsa)->part.high,
           ((WPE_StatField*)&bport_stats.discard_unk_macsa)->part.low);
   wp3_uart_printf ("deny_mac_sa :                     %08x%08x\n",
           ((WPE_StatField*)&bport_stats.deny_mac_sa)->part.high,
           ((WPE_StatField*)&bport_stats.deny_mac_sa)->part.low);
   wp3_uart_printf ("deny_mac_da :                     %08x%08x\n",
           ((WPE_StatField*)&bport_stats.deny_mac_da)->part.high,
           ((WPE_StatField*)&bport_stats.deny_mac_da)->part.low);
   wp3_uart_printf ("rx_bc_valid_packets :             %08x%08x\n",
           ((WPE_StatField*)&bport_stats.rx_bc_valid_packets)->part.high,
           ((WPE_StatField*)&bport_stats.rx_bc_valid_packets)->part.low);
   wp3_uart_printf ("rx_mc_valid_packets :             %08x%08x\n",
           ((WPE_StatField*)&bport_stats.rx_mc_valid_packets)->part.high,
           ((WPE_StatField*)&bport_stats.rx_mc_valid_packets)->part.low);
   wp3_uart_printf ("forwarded_uc_packets :            %08x%08x\n",
           ((WPE_StatField*)&bport_stats.forwarded_uc_packets)->part.high,
           ((WPE_StatField*)&bport_stats.forwarded_uc_packets)->part.low);
   wp3_uart_printf ("forwarded_bc_packets :            %08x%08x\n",
           ((WPE_StatField*)&bport_stats.forwarded_bc_packets)->part.high,
           ((WPE_StatField*)&bport_stats.forwarded_bc_packets)->part.low);
   wp3_uart_printf ("forwarded_mc_packets :            %08x%08x\n",
           ((WPE_StatField*)&bport_stats.forwarded_mc_packets)->part.high,
           ((WPE_StatField*)&bport_stats.forwarded_mc_packets)->part.low);
   wp3_uart_printf ("protocol_error :                  %08x%08x\n",
           ((WPE_StatField*)&bport_stats.protocol_error)->part.high,
           ((WPE_StatField*)&bport_stats.protocol_error)->part.low);

   wp3_uart_printf("\n");
   return;
   
}


void WPE_DisplayIWSystemStats(WP_handle port_handle)
{
   WP_iw_global_stats iw_stats;

   memset(&iw_stats, 0, sizeof(WP_iw_global_stats));
   WP_IwStatistics(port_handle, &iw_stats);

   wp3_uart_printf ("\nIW Port Statistics of Port \n");
   wp3_uart_printf ("==========================================================\n");


   wp3_uart_printf ("\nDenied packets:          %08x%08x\n",
           ((WPE_StatField*)&iw_stats.denied_packets)->part.high,
           ((WPE_StatField*)&iw_stats.denied_packets)->part.low);
   wp3_uart_printf ("IW BG FBP underrun:      %08x%08x\n",
           ((WPE_StatField*)&iw_stats.iwbg_fbp_underrun)->part.high,
           ((WPE_StatField*)&iw_stats.iwbg_fbp_underrun)->part.low);
   wp3_uart_printf("\n");
}

