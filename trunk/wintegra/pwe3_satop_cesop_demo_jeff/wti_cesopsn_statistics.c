/*****************************************************************************
 * (C) Copyright PMC - WIN division (Wintegra) 2011.  All rights reserved.
 * PMC-Sierra (WINTEGRA) CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of PMC-Sierra (Wintegra).
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

WP_U64 WT_IwFlowStatisticsTdm2PsnCheck(WP_stats_iw_cesop_tdm2psn * cesop_stats_tdm2psn,WP_U32 pw_index, WP_U32 global_display)
{
   if(cesop_stats_tdm2psn->forwarded_packets ==0)
   {
#if WTI_DEBUG_LEVEL>1
      WTI_PRINT_ERROR(("\nWT_IwFlowStatisticsTdm2PsnCheck WARNING: no traffic %d\n",pw_index));
#endif
      the_system->stat_error = 1;

   }

   if( cesop_stats_tdm2psn-> fbp_drop_packets !=0 ||
         cesop_stats_tdm2psn-> mtu_drop_packets !=0 ||
         cesop_stats_tdm2psn-> tx_queue_drop_packets !=0)
   {
      if(!global_display)
      {
         
         if(cesop_stats_tdm2psn-> fbp_drop_packets !=0)
            printf("fbp_drop_packets\n");
         if(cesop_stats_tdm2psn-> mtu_drop_packets !=0)
            printf("mtu_drop_packets\n");
         if(cesop_stats_tdm2psn-> tx_queue_drop_packets !=0)
            printf("tx_queue_drop_packets\n");
      }
      
      the_system->stat_error = 1;
#if WTI_DEBUG_LEVEL>1
      WTI_PRINT_ERROR(("\nWT_IwFlowStatisticsTdm2PsnCheck WARNING: IW flow statistics may indicate problem %d \n",pw_index));
#endif
   }

   return cesop_stats_tdm2psn->forwarded_packets;

}



WP_U64  WT_IwFlowStatisticsPsn2TdmCheck( WP_stats_iw_cesop_psn2tdm *cesop_stats,WP_U32 pw_index, WP_U32 global_display)
{
   if(cesop_stats->forwarded_packets ==0)
   {
#if WTI_DEBUG_LEVEL>1
      WTI_PRINT_ERROR(("\nWT_IwFlowStatisticsPsn2TdmCheck WARNING: IW flow statistics No traffic %d \n",pw_index));
#endif
      the_system->stat_error = 1;
   }


   if(cesop_stats-> fbp_drop_packets !=0 ||
         cesop_stats-> out_of_window_packets !=0 ||
         cesop_stats-> buffer_overun_dropped_packets !=0 ||
         cesop_stats-> window_switchover !=0 ||
         cesop_stats-> buffer_overun_events !=0 ||
         cesop_stats-> stray_packets !=0 ||
         cesop_stats-> malformed_packets !=0 ||
         cesop_stats-> cw_ais_drop_packets !=0 ||
         cesop_stats-> multiple_packets !=0 ||
         cesop_stats-> mpls_drop_packets !=0 ||
         cesop_stats-> denied_packets !=0 ||
         cesop_stats-> out_of_sequence_packets !=0 ||
         cesop_stats-> out_of_band_cas_packets !=0 ||
         cesop_stats-> rdi_dropped_packets !=0 ||
         cesop_stats-> rai_packets !=0)
   {
      if(!global_display)
      {
         
         if(cesop_stats-> fbp_drop_packets !=0)
            printf("fbp_drop_packets\n");
         if(cesop_stats-> out_of_window_packets !=0)
            printf("out_of_window_packets\n");
         if(cesop_stats-> buffer_overun_dropped_packets !=0)
            printf("buffer_overun_dropped_packets\n");
         if(cesop_stats-> window_switchover !=0)
            printf("window_switchover\n");
         if(cesop_stats-> buffer_overun_events !=0)
            printf("buffer_overun_events\n");
         if(cesop_stats-> stray_packets !=0)
            printf("stray_packets\n");
         if(cesop_stats-> malformed_packets !=0)
            printf("malformed_packets\n");
         if(cesop_stats-> cw_ais_drop_packets !=0)
            printf("cw_ais_drop_packets\n");
         if(cesop_stats-> multiple_packets !=0)
            printf("multiple_packets\n");
         if(cesop_stats-> mpls_drop_packets !=0)
            printf("mpls_drop_packets\n");
         if(cesop_stats-> denied_packets !=0)
            printf("denied_packets\n");
         if(cesop_stats-> out_of_sequence_packets !=0)
            printf("out_of_sequence_packets\n");
         if(cesop_stats-> out_of_band_cas_packets !=0)
            printf("out_of_band_cas_packets\n");
         if(cesop_stats-> rdi_dropped_packets !=0)
            printf("rdi_dropped_packets\n");
         if(cesop_stats-> rai_packets !=0)
            printf("rai_packets\n");
   }
   
      the_system->stat_error = 1;
#if WTI_DEBUG_LEVEL>1
      WTI_PRINT_ERROR(("\nWT_IwFlowStatisticsPsn2TdmCheck WARNING: IW flow statistics may indicate problem %d\n",pw_index));
#endif
   }
   return cesop_stats->forwarded_packets;

}


WP_U64 WT_IwFlowStatisticsCheck(WP_iw_flow_stats *flow_stats,WP_U32 pw_index)
{


   if(flow_stats->forward_packet ==0)
   {
#if WTI_DEBUG_LEVEL>1
      WTI_PRINT_ERROR(("\nWT_IwFlowStatisticsCheck  WARNING: should be no traffic %d \n",pw_index));
#endif
      the_system->stat_error = 1;
   }


   if (flow_stats->fbp_drop_packets ==0 ||
         flow_stats->mtu_drop_packets ==0 ||
         flow_stats->ttl_drop_packets ==0 ||
         flow_stats->tx_queue_drop_packets ==0 ||
         flow_stats->mpls_drop ==0 ||
         flow_stats->denied_packets ==0 ||
         flow_stats->group_filtered_packets ==0 ||
         flow_stats->forwarded_bytes ==0 ||
         flow_stats->gtp_bad_headers ==0 ||
#if _WP_MODULE_SWAN_LAKE_ENABLE_
         flow_stats->tx_queue_drop_fr ==0 ||
#endif
         flow_stats->policer_non_conforming_packets ==0)

      the_system->stat_error = 1;

   if ( the_system->stat_error)
   {
#if WTI_DEBUG_LEVEL>1
      WTI_PRINT_ERROR(("\nWT_IwFlowStatisticsCheck  WARNING: IW flow statistics may indicate problem %d\n",pw_index));
#endif
   }
   return flow_stats->forward_packet;
}


#if WTI_CESOP_TDI
WP_U64  WT_TdiChannelTransRxCheck(WP_stats_tdi_trans_pwe3_rx *rx_stats,WP_U32 pw_index)
{
   if (rx_stats->empty_fbp !=0)
   {
#if WTI_DEBUG_LEVEL>1
      WTI_PRINT_ERROR(("\nWT_TdiChannelTransRxCheck  WARNING: IW flow statistics may indicate problem %d\n",pw_index));
#endif
      the_system->stat_error = 1;
   }

   if ( rx_stats->valid_packets ==0)
   {
#if WTI_DEBUG_LEVEL>1
      WTI_PRINT_ERROR(("\nWT_TdiChannelTransRxCheck  WARNING: no traffic %d\n",pw_index));
#endif
      the_system->stat_error = 1;
   }
   return rx_stats->valid_packets;
}
WP_U64  WT_TdiChannelTransTxCheck(WP_stats_tdi_trans_pwe3_tx  *tx_stats,WP_U32 pw_index)
{

   if ( tx_stats->dummy_packet !=0||
         tx_stats->buffer_underrun !=0||
         tx_stats->underrun_dataunit !=0)
   {
#if WTI_DEBUG_LEVEL>1
      WTI_PRINT_ERROR(("\nWT_TdiChannelTransTxCheck WARNING: IW flow statistics may indicate problem %d\n",pw_index));
#endif
      the_system->stat_error = 1;
   }

   if ( tx_stats->valid_packets ==0)
   {
#if WTI_DEBUG_LEVEL>1
      WTI_PRINT_ERROR(("\nWT_TdiChannelTransTxCheck  WARNING: no traffic %d \n",pw_index));
#endif
      the_system->stat_error = 1;
   }
   return tx_stats->valid_packets;
}

#else

WP_U64  WT_UfeChannelTransRxCheck(WP_stats_emphy_trans_pwe3_rx *rx_stats,WP_U32 pw_index)
{
	WP_U32 temp1, temp2;

	if (rx_stats->empty_fbp !=0 ||
			rx_stats->channel_out_of_sync !=0 ||
			rx_stats->rx_ts_sync_lost != 0)
	{
		printf("pw_index %d:   ",pw_index);
		if(rx_stats->channel_out_of_sync !=0)
		{
			temp1 = rx_stats->channel_out_of_sync;
			temp2 = rx_stats->channel_out_of_sync >> 32;
			printf("channel_out_of_sync:");
			printf("%.8x%.8x    ", temp2,temp1);
		}
		if(rx_stats->empty_fbp !=0  )
		{
			temp1 = rx_stats->empty_fbp;
			temp2 = rx_stats->empty_fbp >> 32;
			printf("empty_fbp:");
			printf("%.8x%.8x    ", temp2,temp1);
		}
		if(rx_stats->rx_ts_sync_lost != 0)
		{
			temp1 = rx_stats->rx_ts_sync_lost;
			temp2 = rx_stats->rx_ts_sync_lost >> 32;
			printf("rx_ts_sync_lost:");
			printf("%.8x%.8x\n", temp2,temp1);
		}

#if WTI_DEBUG_LEVEL>1
		WTI_PRINT_ERROR(("\nWT_UfeChannelTransRxCheck  WARNING:  may indicate problem %d\n",pw_index));
#endif
		the_system->stat_error = 1;
	}

#if WTI_CES_SHAPING_ENABLED
	if(test_setup.line_cr_operation_mode[(pw_index+(pw_index/21)*7)] == WP_IW_CLOCK_REC_ADAPTIVE_MODE)
	{
		if(rx_stats->ces_shaping_stats.underrun !=0 ||
				rx_stats->ces_shaping_stats.fifo_put_error !=0 ||
				rx_stats->ces_shaping_stats.overrun !=0 ||
				rx_stats->ces_shaping_stats.fifo_get_error !=0 ||
				rx_stats->ces_shaping_stats.invalid_delta_ts !=0 )
		{

			if(rx_stats->ces_shaping_stats.underrun !=0)
			{
				temp1 = rx_stats->ces_shaping_stats.underrun;
				temp2 = rx_stats->ces_shaping_stats.underrun >> 32;
				printf("ces_shaping_underrun:");
				printf("%.8x%.8x\n", temp2,temp1);
			}
			if(rx_stats->ces_shaping_stats.fifo_put_error !=0)
			{
				temp1 = rx_stats->ces_shaping_stats.fifo_put_error;
				temp2 = rx_stats->ces_shaping_stats.fifo_put_error >> 32;
				printf("ces_shaping_fifo_put_error:");
				printf("%.8x%.8x\n", temp2,temp1);
			}
			if(rx_stats->ces_shaping_stats.overrun !=0)
			{
				temp1 = rx_stats->ces_shaping_stats.overrun;
				temp2 = rx_stats->ces_shaping_stats.overrun >> 32;
				printf("ces_shaping_overrun:");
				printf("%.8x%.8x\n", temp2,temp1);
			}
			if(rx_stats->ces_shaping_stats.fifo_get_error !=0)
			{
				temp1 = rx_stats->ces_shaping_stats.fifo_get_error;
				temp2 = rx_stats->ces_shaping_stats.fifo_get_error >> 32;
				printf("ces_shaping_fifo_get_error:");
				printf("%.8x%.8x\n", temp2,temp1);
			}
			if(rx_stats->ces_shaping_stats.invalid_delta_ts !=0)
			{
				temp1 = rx_stats->ces_shaping_stats.invalid_delta_ts;
				temp2 = rx_stats->ces_shaping_stats.invalid_delta_ts >> 32;
				printf("ces_shaping_invalid_delta_ts:");
				printf("%.8x%.8x\n", temp2,temp1);
			}

                        the_system->stat_error = 1;
		}


	}
#endif

	if ( rx_stats->valid_packets ==0)
	{
#if WTI_DEBUG_LEVEL>1
		WTI_PRINT_ERROR(("\nWT_UfeChannelTransRxCheck WARNING: no traffic %d\n",pw_index));
#endif
		the_system->stat_error = 1;
	}
	return rx_stats->valid_packets;
}



WP_U64  WT_UfeChannelTransTxCheck(WP_stats_emphy_trans_pwe3_tx  *tx_stats,WP_U32 pw_index)
{

   if (tx_stats->dummy_packet !=0 ||
         tx_stats->buffer_underrun !=0 ||
         tx_stats->underrun_dataunit !=0)
   {
#if WTI_DEBUG_LEVEL>1
      WTI_PRINT_ERROR(("\nWT_UfeChannelTransTxCheck  WARNING:  may indicate problem %d\n",pw_index));
#endif
      if(tx_stats->dummy_packet !=0)
         printf("dummy_packet\n");      
      if(tx_stats->buffer_underrun !=0)
         printf("buffer_underrun\n");
      if(tx_stats->underrun_dataunit !=0)
         printf("underrun_dataunit\n");
      the_system->stat_error = 1;
   }

   if ( tx_stats->valid_packets ==0)
   {
#if WTI_DEBUG_LEVEL>1
      WTI_PRINT_ERROR(("\nWT_UfeChannelTransTxCheck WARNING: no traffic %d\n", pw_index));
#endif
      the_system->stat_error = 1;
   }
   return tx_stats->valid_packets;
}

#endif

/***************************************************************
 * Func name  :
 * Description:
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void CLI_F_Psn2TdmFlowAggStatisticsClear(char *StrPrm)
{
   WP_stats_iw_cesop_psn2tdm cesop_stats = {0};
   WP_status status;
   WP_U32 res, cmd, val;

   res = sscanf(StrPrm, "%d %d", &cmd, &val);

   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
   if (val >= total_number_of_pw)
   {
      WTI_TerminatePrintError("CLI_F_Psn2TdmFlowAggStatistics", __LINE__);
      return;
   }

   status = WP_IwFlowStatisticsReset(the_system->pw[val].psn2tdm_flow_agg,
                                     WP_IW_FLOW_STAT_CESOP_PSN2TDM,
                                     &cesop_stats);
   WTI_TerminateOnError(status, "WP_IwFlowStatisticsReset", __LINE__);

   return;
}

WP_U32 CLI_F_Pwe3StatisticsClear(char *StrPrm)
{

   WP_CHAR temp_buf[100];
   WP_U32 res, cmd, val,i;
   res = sscanf(StrPrm, "%d %d", &cmd, &val);

   if (res != 2 && res != 1)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return WT_FAIL;
   }
   if (res == 1)
      val = total_number_of_pw;
   else if (val > total_number_of_pw)
   {
      printf("max number_of_pw %d\n",total_number_of_pw);

      WTI_TerminatePrintError("CLI_F_Pwe3RxChannelStatisticsClear", __LINE__);
      return WT_FAIL;
   }



   for(i=0;i<val;i++)
   {
      WP_U32 qdepth;
      WP_status status;

      sprintf(temp_buf,"0 %d", i);
      CLI_F_Pwe3RxChannelStatisticsClear(temp_buf);
      CLI_F_Pwe3TxChannelStatisticsClear(temp_buf);
      CLI_F_Psn2TdmFlowAggStatisticsClear(temp_buf);
      sprintf(temp_buf,"0 %d 0", i);
      CLI_F_Tdm2PsnFlowAggStatisticsClear(temp_buf);

      status = WP_ChannelQDepth(the_system->pw[i].trans_tx, &qdepth);
      if (status != WP_OK)
      {
         WTI_TerminatePrintError("CLI_F_Pwe3RxChannelStatisticsClear", __LINE__);
         return WT_FAIL;
      }

      the_system->tx_ch_depth[i]=(WP_U16)qdepth;


   }

   return WT_PASS;
}


WP_U32 CLI_F_Pwe3StatisticsCheck(char *StrPrm)
{
   WP_CHAR temp_buf[100];
   WP_U8  result[WTI_MAX_PW];
   WP_U32 res, cmd, val,i,qdepth,global_display, rx_ts_check = 1;
   res = sscanf(StrPrm, "%d %d %d %d", &cmd, &rx_ts_check, &val, &global_display);

   if (res != 4 && res != 3 && res != 2 && res != 1)
   {
      WTI_TerminatePrintError("CLI_F_Pwe3StatisticsCheck Invalid number of parameters", __LINE__);
      return WT_FAIL;
   }
   if (res == 1)
   {
      val = total_number_of_pw;
      rx_ts_check = 1;
   }
   else if (res == 2)
   {
      val = total_number_of_pw;
      if (rx_ts_check != 0 && rx_ts_check != 1)
      {
         WTI_TerminatePrintError("CLI_F_Pwe3StatisticsCheck Invalid parameter: rx_ts_check (0 or 1)", __LINE__);
         return WT_FAIL;
      }
   }
   else
   {
      if (val > total_number_of_pw)
      {
         printf("max number_of_pw %d\n",total_number_of_pw);
         WTI_TerminatePrintError("CLI_F_Pwe3StatisticsCheck ", __LINE__);
         return WT_FAIL;
      }
      if (rx_ts_check != 0 && rx_ts_check != 1)
      {
         WTI_TerminatePrintError("CLI_F_Pwe3StatisticsCheck Invalid parameter: rx_ts_check (0 or 1)", __LINE__);
         return WT_FAIL;
      }
   }

   if (res == 4)
   {
      global_display = 1;
   }
   else
   {
      global_display = 0;
   }

   the_system->stat_error=0;

   for(i=0;i<val;i++)

   {
      WP_stats_iw_cesop_psn2tdm cesop_stats_psn2tdm;
      WP_stats_iw_cesop_tdm2psn cesop_stats_tdm2psn;
#if WTI_CESOP_TDI
      WP_stats_tdi_trans_pwe3_rx    rx_stats;
      WP_stats_tdi_trans_pwe3_tx    tx_stats;
#else
      WP_stats_emphy_trans_pwe3_rx  rx_stats;
      WP_stats_emphy_trans_pwe3_tx  tx_stats;
#endif

      WP_status status;
      /************************************************/
      memset(&cesop_stats_psn2tdm,0,sizeof(WP_stats_iw_cesop_psn2tdm));

      status = WP_IwFlowStatistics(the_system->pw[i].psn2tdm_flow_agg,
                                   WP_IW_FLOW_STAT_CESOP_PSN2TDM,
                                   &cesop_stats_psn2tdm);
      if (status != WP_OK){
#if WTI_DEBUG_LEVEL>1
         printf("ERROR in statistics of index %d WP_IW_FLOW_STAT_CESOP_PSN2TDM \n",i);
#endif
         WTI_TerminateOnError(status, "WP_IwFlowStatistics", __LINE__);
         return 1;
      }
      WT_IwFlowStatisticsPsn2TdmCheck(&cesop_stats_psn2tdm,i, global_display);

      /************************************************/

      memset(&cesop_stats_tdm2psn,0,sizeof(WP_stats_iw_cesop_tdm2psn));

      status = WP_IwFlowStatistics(the_system->pw[i].tdm2psn_flow_agg,
                                   WP_IW_FLOW_STAT_CESOP_TDM2PSN,
                                   &cesop_stats_tdm2psn);
      if (status != WP_OK){
#if WTI_DEBUG_LEVEL>1
         printf("ERROR in statistics of index %d WP_IW_FLOW_STAT_CESOP_TDM2PSN \n",i);
#endif
         WTI_TerminateOnError(status, "WP_IwFlowStatistics", __LINE__);
         return WT_FAIL;

      }
      WT_IwFlowStatisticsTdm2PsnCheck(&cesop_stats_tdm2psn,i, global_display);

      /************************************************/
#if WTI_CESOP_TDI
      memset(&rx_stats,0,sizeof(WP_stats_tdi_trans_pwe3_rx));
      memset(&tx_stats,0,sizeof(WP_stats_tdi_trans_pwe3_tx));
#else
      memset(&rx_stats,0,sizeof(WP_stats_emphy_trans_pwe3_rx));
      memset(&tx_stats,0,sizeof(WP_stats_emphy_trans_pwe3_tx));
#endif

      status = WP_ChannelStatistics(the_system->pw[i].trans_rx, &rx_stats);
      if (status != WP_OK) return 1;
      status = WP_ChannelStatistics(the_system->pw[i].trans_tx, &tx_stats);
      if (status != WP_OK) return 1;

#if !WTI_CESOP_TDI
      /* If selected, eliminate the check for RX TS */
      if (rx_ts_check == 0)
         rx_stats.rx_ts_sync_lost = 0;
#endif /* not in TDI */

#if WTI_CESOP_TDI
      WT_TdiChannelTransRxCheck(&rx_stats,i);
      WT_TdiChannelTransTxCheck(&tx_stats,i);

#else
      WT_UfeChannelTransRxCheck(&rx_stats,i);
      WT_UfeChannelTransTxCheck(&tx_stats,i);
#endif

      if(!global_display)
      {

         if (the_system->stat_error !=0)
         {
#if WTI_DEBUG_LEVEL>1
            printf("ERROR in statistics of index %d\n",i);
#endif
            break;
         }


         status = WP_ChannelQDepth(the_system->pw[i].trans_tx, &qdepth);
         if (status != WP_OK)
         {
#if WTI_DEBUG_LEVEL>1
            printf("ERROR in WP_ChannelQDepth %d\n",i);
#endif
            return WT_FAIL;
         }



         if (the_system->tx_ch_depth[i]> qdepth+1 ||
             the_system->tx_ch_depth[i]< qdepth-1 )
         {
#if WTI_DEBUG_LEVEL>1
            printf("ERROR unstable queue at index %d (originaly %d now %d)\n",i,the_system->tx_ch_depth[i],qdepth);
#endif
         }

      }
      else
      {
         /* global_display */
         if (the_system->stat_error)
         {
            result[i] = 1;
         }
         else
         {
            result[i] = 0;
         }
         the_system->stat_error = 0;
      }
   }

   if(global_display)
   {
      for(i=0;i<val;i++)
      {
         if(result[i])
         {
            sprintf(temp_buf,"PW Index:%4d FAIL. ",i);
            WTI_PRINT_ERROR((temp_buf));
         }
         else
         {
            sprintf(temp_buf,"PW Index:%4d PASS. ",i);
            WTI_PRINT_NOTE((temp_buf));
         }
      }
      printf("\n\n");
      return WT_PASS;
   }
   else
   {
      if (the_system->stat_error == 0)
      {
         WTI_PRINT_NOTE(("\n statistics check passed \n\n"));
         return WT_PASS;
      }
      else
      {
         WTI_PRINT_ERROR(("\n statistics check failed \n\n"));
         return WT_FAIL;
      }
   }

}

#if defined __WT_UFE412__ || defined __WT_UFE448__

void CLI_F_ClockRecStatistics(char *StrPrm)
{
#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
   WP_U32 res, cmd, pw_index, line_index, clock_rec_if_id;
   WP_clock_rec_stats stats;
   WP_status status;
   WP_U32 t1, t2;
   struct t_64 {
      WP_U32 high;
      WP_U32 low;
   } *temp_stat;
   WP_U32 line_cr_operation_mode;

   res = sscanf(StrPrm, "%d %d", &cmd, &pw_index);

    if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }

   if (pw_index >= total_number_of_pw)
   {
	   WTI_TerminatePrintError("Invalid PW index", __LINE__);
	   return;
   }

   line_index = the_system->pw[pw_index].line_index;
   clock_rec_if_id = the_system->clock_rec[line_index].clock_rec_if_id;

   /* Clock domain per line */
   if (pw_index < WTI_CESOP_MAX_CLOCK_REC_INTERFACES)
   {
	   if (pw_index >= MAX_UFE_LINES_USED)
	   {
		   WTI_TerminatePrintError("Invalid number of regular clock domain", __LINE__);
		   return;
	   }
   }

   /* Extract the line operation mode */
   line_cr_operation_mode = the_system->clock_rec[line_index].operation_mode;

   Vt100GotoXy(1,1);
   Vt100ClearScreen();

   while (1)
   {
      t1 = WP_TimeRead();

      if (WTI_SampleSerial())
      {
         break;
      }
      while (1)
      {
         t2 = WP_TimeRead();
         if( (t2-t1) >= FACTOR)
         {
            memset(&stats, 0, sizeof(WP_clock_rec_stats));
            status = WP_ClockRecStatistics(the_system->wpid, clock_rec_if_id, &stats,WP_IW_CESOP_PORT_UFE);
            if (status != WP_OK)
            {
               printf("WP_ClockRecStatistics failed: %s\n", WP_error_name[WP_ERROR(status)]);
               break;
            }

            Vt100GotoXy(1,1);

            printf("---- CLOCK RECOVERY STATISTICS FOR TABLE %d ----\n", clock_rec_if_id);
            printf("----         LINE %3d PW INDEX %3d         ----\n", line_index, pw_index);
            printf("--------------------------------------------------\n");
            printf("\n");

            temp_stat = (struct t_64 *)&stats.reset_events;
            printf("reset events:                   %08x%08x\n", temp_stat->high, temp_stat->low);
            temp_stat = (struct t_64 *)&stats.holdover_period_expired_events;
            printf("holdover period expired events: %08x%08x\n", temp_stat->high, temp_stat->low);
            if (line_cr_operation_mode != WP_IW_CLOCK_REC_DIFFERENTIAL_MODE)
            {
               /* Statistics for ACR modes */
               temp_stat = (struct t_64 *)&stats.lost_over_limit;
               printf("lost over limit:                %08x%08x\n", temp_stat->high, temp_stat->low);
               temp_stat = (struct t_64 *)&stats.phase_error_holdover;
               printf("phase error holdover:           %08x%08x\n", temp_stat->high, temp_stat->low);
            }
            else
            {
               /* Statistics for DCR modes */
               temp_stat = (struct t_64 *)&stats.invalid_input_timestamp;
               printf("invalid input timestamp:        %08x%08x\n", temp_stat->high, temp_stat->low);
               temp_stat = (struct t_64 *)&stats.invalid_output_timestamp;
               printf("invalid output timestamp:       %08x%08x\n", temp_stat->high, temp_stat->low);
            }

            /* Statistics for ufe4 */
            temp_stat = (struct t_64 *)&stats.invalid_phase_gap;
            printf("invalid phase gap:              %08x%08x\n", temp_stat->high, temp_stat->low);
            printf("service to nominal clock ppb:   %16d\n", stats.ppb_service_to_nominal );
            printf("socket to diff clock ppb:       %16d\n", stats.ppb_socket_to_diff );
            printf("socket to XO clock ppb:         %16d\n", stats.ppb_socket_to_xo );
            printf("network average noise  :            %8d\n", stats.network_ave_noise );
            printf("\n");

            printf("invalid emphy rx timestamps:            %8d\n", stats.invalid_emphy_rx_timestamp);


#ifdef AUTOMATION_MODE
            return;
#else
            break;
#endif
         }
      }
   }
#endif
}

#else

void CLI_F_ClockRecStatistics(char *StrPrm)
{
#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
   WP_U32 res, cmd, val;
   WP_clock_rec_stats stats;
   WP_status status;
   WP_U32 t1, t2;
   struct t_64 {
      WP_U32 high;
      WP_U32 low;
   } *temp_stat;
   WP_U32 line_cr_operation_mode;

   res = sscanf(StrPrm, "%d %d", &cmd, &val);

   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }
#if defined __WT_UFE412__ || defined __WT_UFE448__
 /* Clock domain per line */
 if (val < WTI_CESOP_MAX_CLOCK_REC_INTERFACES)
   {
      if (val >= MAX_UFE_LINES_USED)
      {
         WTI_TerminatePrintError("Invalid number of regular clock domain", __LINE__);
         return;
      }
   }
#else /* Not __WT_UFE412__ || __WT_UFE448__ */
   if (val < WTI_CESOP_MAX_CLOCK_REC_INTERFACES)
   {
      if (val >= global_active_cr_async_mode)
      {
         WTI_TerminatePrintError("Invalid number of regular clock domain", __LINE__);
         return;
      }
   }
   else
   {
      if (val-WTI_CESOP_MAX_CLOCK_REC_INTERFACES >= global_active_cr_extdiff_mode)
      {
         WTI_TerminatePrintError("Invalid number of extended differential clock domain", __LINE__);
         return;
      }
   }
#endif /* Not __WT_UFE412__ || __WT_UFE448__*/

   /* Extract the line operation mode */
   line_cr_operation_mode = the_system->clock_rec[val].operation_mode;

   Vt100GotoXy(1,1);
   Vt100ClearScreen();

   while (1)
   {
      t1 = WP_TimeRead();

      if (WTI_SampleSerial())
      {
         break;
      }
      while (1)
      {
         t2 = WP_TimeRead();
         if( (t2-t1) >= FACTOR)
         {
            memset(&stats, 0, sizeof(WP_clock_rec_stats));
            status = WP_ClockRecStatistics(the_system->wpid, val, &stats,
#if WTI_CESOP_TDI
                                           WP_IW_CESOP_PORT_TDM
#else
                                           WP_IW_CESOP_PORT_UFE
#endif
            );
            if (status != WP_OK)
            {
               printf("WP_ClockRecStatistics failed: %s\n", WP_error_name[WP_ERROR(status)]);
               break;
            }

            Vt100GotoXy(1,1);

            printf("---- CLOCK RECOVERY STATISTICS FOR TABLE %d ----\n", val);
            printf("--------------------------------------------------\n");
            printf("\n");

            temp_stat = (struct t_64 *)&stats.reset_events;
            printf("reset events:                   %08x%08x\n", temp_stat->high, temp_stat->low);
            temp_stat = (struct t_64 *)&stats.holdover_period_expired_events;
            printf("holdover period expired events: %08x%08x\n", temp_stat->high, temp_stat->low);
            if (line_cr_operation_mode != WP_IW_CLOCK_REC_DIFFERENTIAL_MODE)
            {
               /* Statistics for ACR modes */
               temp_stat = (struct t_64 *)&stats.lost_over_limit;
               printf("lost over limit:                %08x%08x\n", temp_stat->high, temp_stat->low);
               temp_stat = (struct t_64 *)&stats.phase_error_holdover;
               printf("phase error holdover:           %08x%08x\n", temp_stat->high, temp_stat->low);
            }
            else
            {
               /* Statistics for DCR modes */
               temp_stat = (struct t_64 *)&stats.invalid_input_timestamp;
               printf("invalid input timestamp:        %08x%08x\n", temp_stat->high, temp_stat->low);
               temp_stat = (struct t_64 *)&stats.invalid_output_timestamp;
               printf("invalid output timestamp:       %08x%08x\n", temp_stat->high, temp_stat->low);
            }
#if defined __WT_UFE412__ || defined __WT_UFE448__
            /* Statistics for ufe4 */
            temp_stat = (struct t_64 *)&stats.invalid_phase_gap;
            printf("invalid phase gap:              %08x%08x\n", temp_stat->high, temp_stat->low);
            printf("service to nominal clock ppb:   %16d\n", stats.ppb_service_to_nominal );
            printf("socket to diff clock ppb:       %16d\n", stats.ppb_socket_to_diff );
            printf("socket to XO clock ppb:         %16d\n", stats.ppb_socket_to_xo );
            printf("\n");

            printf("invalid emphy rx timestamps:            %8d\n", stats.invalid_emphy_rx_timestamp);


#endif   /* __WT_UFE412__ || __WT_UFE448__ */


#ifdef AUTOMATION_MODE
            return;
#else
            break;
#endif
         }
      }
   }
#endif
}
#endif

#if defined __WT_UFE412__ || __WT_UFE448__
void CLI_F_ClockRecStatisticsNAN(char *StrPrm)
{

#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
	WP_U32 res, cmd, pw_index, line_index, clock_rec_if_id;
	WP_clock_rec_stats stats;
	WP_status status;
	WP_U32 t1, t2;

	res = sscanf(StrPrm, "%d", &cmd);

	if (res > 1)
	{
		WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
		return;
	}

	Vt100ClearScreen();

	while (1)
	{
		t1 = WP_TimeRead();

		if (WTI_SampleSerial())
		{
			break;
		}
		while (1)
		{
			t2 = WP_TimeRead();
			if( (t2-t1) >= FACTOR)
			{
				Vt100GotoXy(1,1);
				for(pw_index = 0; pw_index < total_number_of_pw; pw_index++)
				{
					line_index = the_system->pw[pw_index].line_index;
					clock_rec_if_id = the_system->clock_rec[line_index].clock_rec_if_id;

					memset(&stats, 0, sizeof(WP_clock_rec_stats));
					status = WP_ClockRecStatistics(the_system->wpid, clock_rec_if_id, &stats,WP_IW_CESOP_PORT_UFE);
					if (status != WP_OK)
					{
						printf("WP_ClockRecStatistics failed: %s\n", WP_error_name[WP_ERROR(status)]);
						return;
					}

					printf("PW %3d: %3d   ", pw_index, stats.network_ave_noise);

					if(pw_index%7 == 6)
						printf("\n");

				}
				break;
			}
		}
	}
	printf("\n");
#endif

}
#endif   /* __WT_UFE412__ || __WT_UFE448__ */

WP_U32 CLI_F_ClockRecStatisticsPPB(char *StrPrm)
{
#if defined __WT_UFE412__ || defined __WT_UFE448__
#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
   WP_U32 line_index,cr_index,auto_check_mode=0,max_lines_oc12=0;
   WP_clock_rec_stats stats;
   WP_status status;
   WP_U32 t1, t2;
   WP_S32 res,cmd,ppb_offset=0,ppb_margins=0,max_ppb=0,min_ppb=0,avg_ppb=0,prev_avg_ppb=0,variance=0,line_counter=0;
   WP_U32 status_check = WT_PASS,see_all_mode=0;


   /* default values */
   ppb_offset =0;
   ppb_margins = 1000;  /* 1 ppm */


   res = sscanf(StrPrm, "%d %d %d", &cmd,&ppb_margins,&see_all_mode);
   if (res != 1 && res != 2 && res != 3)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return WT_FAIL;
   }

   if (res == 2 && ppb_margins < 0)
   {
      WTI_TerminatePrintError("margin must be positive", __LINE__);
      return WT_FAIL;
   }


   if(the_system->ufe.line_params[0].transfer_type == WUFE_SDH_TYPE_T1 ||
      the_system->ufe.line_params[0].transfer_type == WUFE_SONET_TYPE_T1)
   {  /* T1 */
      max_lines_oc12 = 336;
   }
   else
   { /* E1 */
      max_lines_oc12 = 252;
   }




   Vt100GotoXy(1,1);
   Vt100ClearScreen();


/* calculate the average ppb */

#if defined __WT_UFE412__ || defined __WT_UFE448__
   for(line_index = 0,line_counter=0;line_index <  WTI_CESOP_MAX_CLOCK_REC_INTERFACES ;line_index++)
   {

      if(!the_system->clock_rec[line_index].operation_mode)
      {
         continue;
      }

      cr_index = the_system->clock_rec[line_index].clock_rec_if_id;

      memset(&stats, 0, sizeof(WP_clock_rec_stats));
      status = WP_ClockRecStatistics(the_system->wpid,cr_index, &stats,
#if WTI_CESOP_TDI
                                     WP_IW_CESOP_PORT_TDM
#else
                                     WP_IW_CESOP_PORT_UFE
#endif
         );
      if (status != WP_OK)
      {
         printf("WP_ClockRecStatistics failed: %s\n", WP_error_name[WP_ERROR(status)]);
         break;
      }

      avg_ppb+=stats.ppb_service_to_nominal;

      line_counter++;
   }

   avg_ppb = avg_ppb/line_counter;
   ppb_offset = avg_ppb;

#endif

   if (res != 1)
   {
      auto_check_mode =1;
      max_ppb = ppb_offset + ppb_margins;
      min_ppb = ppb_offset - ppb_margins;
   }

   if(auto_check_mode)
   {

      printf("--------------------------------------------------\n");
      printf("---- CLOCK RECOVERY STATISTICS PPB AUTO CHECK ----\n");
      printf("--------------------------------------------------\n");
      printf("Check  %d < average ppb = %d < %d\n",min_ppb,ppb_offset,max_ppb);

      memset(&stats, 0, sizeof(WP_clock_rec_stats));
      status = WP_ClockRecStatistics(the_system->wpid,0, &stats,
#if WTI_CESOP_TDI
                                     WP_IW_CESOP_PORT_TDM
#else
                                     WP_IW_CESOP_PORT_UFE
#endif
         );

//      /* First check ratios (offset = 0ppm)  */
//      if(stats.ppb_socket_to_diff > ppb_margins ||  stats.ppb_socket_to_diff < -ppb_margins)
//      {
//         WTI_PRINT_WARNING(("socket to diff clock ppb:          %8d\n", stats.ppb_socket_to_diff ));
//         status_check = WT_FAIL;
//      }
//      if(stats.ppb_socket_to_xo > ppb_margins ||  stats.ppb_socket_to_xo < -ppb_margins)
//      {
//         WTI_PRINT_WARNING(("socket to XO clock ppb:          %8d\n", stats.ppb_socket_to_xo ));
//         status_check = WT_FAIL;
//      }

      /* Now check all lines  */

      for(line_index = 0,line_counter=0;line_index <  WTI_CESOP_MAX_CLOCK_REC_INTERFACES ;line_index++)
      {

         if(!the_system->clock_rec[line_index].operation_mode)
         {
            continue;
         }

         cr_index = the_system->clock_rec[line_index].clock_rec_if_id;

         memset(&stats, 0, sizeof(WP_clock_rec_stats));
         status = WP_ClockRecStatistics(the_system->wpid,cr_index, &stats,
#if WTI_CESOP_TDI
                                        WP_IW_CESOP_PORT_TDM
#else
                                        WP_IW_CESOP_PORT_UFE
#endif
            );
         if (status != WP_OK)
         {
            printf("WP_ClockRecStatistics failed: %s\n", WP_error_name[WP_ERROR(status)]);
            break;
         }

         if(stats.ppb_service_to_nominal > max_ppb ||  stats.ppb_service_to_nominal < min_ppb)
         {
            WTI_PRINT_ERROR(("%3d(%3d):%8d ",line_counter,cr_index,stats.ppb_service_to_nominal ));
            status_check = WT_FAIL;
         }
         else
         {
            if(see_all_mode)
               WTI_PRINT_NOTE(("%3d(%3d):%8d ",line_counter,cr_index,stats.ppb_service_to_nominal ));

         }

         if (line_counter%7 == 6)
            printf("\n");



         line_counter++;
      }
      printf("\n");

      return status_check;
   }



   while (1)
   {
      t1 = WP_TimeRead();

      if (WTI_SampleSerial())
      {
         break;
      }
      while (1)
      {
         t2 = WP_TimeRead();
         if( (t2-t1) >= FACTOR)
         {

            Vt100GotoXy(1,1);

            /* find the first active line to print the general data */
            for(line_index = 0,line_counter=0; line_index <  WTI_CESOP_MAX_CLOCK_REC_INTERFACES ;line_index++)
            {

               if(!the_system->clock_rec[line_index].operation_mode)
               {
                  continue;
               }
               else
               {

               memset(&stats, 0, sizeof(WP_clock_rec_stats));
               status = WP_ClockRecStatistics(the_system->wpid,0, &stats,
#if WTI_CESOP_TDI
                                              WP_IW_CESOP_PORT_TDM
#else
                                              WP_IW_CESOP_PORT_UFE
#endif
                  );
#if WTI_DUAL_EMPHY
               WP_U32 first_line_second_ufe = 336;
               WP_clock_rec_stats stats_second_ufe;

               memset(&stats_second_ufe, 0, sizeof(WP_clock_rec_stats));
               status = WP_ClockRecStatistics(the_system->wpid,first_line_second_ufe, &stats_second_ufe, WP_IW_CESOP_PORT_UFE);
#endif

               printf("--------------------------------------------------\n");
               printf("---- CLOCK RECOVERY STATISTICS PPB ----\n");
               printf("--------------------------------------------------\n");
               printf("\n");

#if defined __WT_UFE412__ || defined __WT_UFE448__
#if WTI_DUAL_EMPHY
               printf("First UFE ratios:\n");
#endif
               printf("socket to diff clock ppb:          %8d\n", stats.ppb_socket_to_diff );
               printf("socket to XO clock ppb:            %8d\n", stats.ppb_socket_to_xo );
#if WTI_DUAL_EMPHY
               printf("Second UFE ratios:\n");
               printf("socket to diff clock ppb:          %8d\n", stats_second_ufe.ppb_socket_to_diff );
               printf("socket to XO clock ppb:            %8d\n", stats_second_ufe.ppb_socket_to_xo );
#endif
               printf("average ppb:                       %8d\n", avg_ppb );
               printf("variance:                          %8d\n", variance );
               printf("--------------------------------------------------\n");
               printf("count/line/cr:service to nominal clock ppb:      \n");

               break;
               }
            }


            /* go over all the active lines*/
            for(line_index = 0,avg_ppb=0,variance=0,line_counter=0; line_index <  WTI_CESOP_MAX_CLOCK_REC_INTERFACES ;
                line_index++)
            {

               if(!the_system->clock_rec[line_index].operation_mode)
               {
                  continue;
               }


               cr_index = the_system->clock_rec[line_index].clock_rec_if_id;

               memset(&stats, 0, sizeof(WP_clock_rec_stats));
               status = WP_ClockRecStatistics(the_system->wpid,cr_index,&stats,
#if WTI_CESOP_TDI
                                              WP_IW_CESOP_PORT_TDM
#else
                                              WP_IW_CESOP_PORT_UFE
#endif
                  );
               if (status != WP_OK)
               {
                  printf("WP_ClockRecStatistics failed: %s\n", WP_error_name[WP_ERROR(status)]);
                  break;
               }


               if (line_counter%24 == 0)
                  printf("\n");


               if (line_counter%max_lines_oc12 == 0 && line_counter != 0)
                  printf("\n\n");

               printf("%-6d ",stats.ppb_service_to_nominal );

               variance += ((stats.ppb_service_to_nominal - prev_avg_ppb) * (stats.ppb_service_to_nominal - prev_avg_ppb));
               avg_ppb+=stats.ppb_service_to_nominal;

            line_counter++;
            }

            avg_ppb = avg_ppb/line_counter;
            prev_avg_ppb = avg_ppb;
            variance = variance / line_counter;


#endif   /* __WT_UFE412__ || __WT_UFE448__ */


            break;
         }
      }
   }
   printf("\n");
   return WT_PASS;
#else
   return WT_FAIL;
#endif
#else  /* WT_UFE4 */
   return WT_PASS;
#endif /* WT_UFE4 */
}

void CLI_F_ClockRecStatisticsReset(char *StrPrm)
{
#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
   WP_U32 res, cmd, val;
   WP_status status;

   res = sscanf(StrPrm, "%d %d", &cmd, &val);

#if defined __WT_UFE412__ || defined __WT_UFE448__
   WP_U32 line_index = the_system->pw[val].line_index;
   val = the_system->clock_rec[line_index].clock_rec_if_id;
#endif

   if (res != 2)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }

   status = WP_ClockRecStatisticsReset(the_system->wpid, val,
#if WTI_CESOP_TDI
                                       WP_IW_CESOP_PORT_TDM
#else
                                       WP_IW_CESOP_PORT_UFE
#endif
   );
   if (status != WP_OK)
   {
      printf("WP_ClockRecStatistics failed: %s\n", WP_error_name[WP_ERROR(status)]);
      return;
   }
#endif
}

#define WTI_CRT_HOST_ADDRESS 0xbeccb100 
#define WTI_CRT_B254_ADDRESS 0xbc249500 

void CLI_F_ClockRecDebugPrint (char *StrPrm)
{
#if WTI_CESOP_CLOCK_RECOVERY_ENABLE


   WP_U32 cmd=0,res=0,pw=0,seconds =0,host_ptr_cr_base =0,iterations=0,ii=0,delay=0;
   WP_S32 *host_ptr =0;
   WP_status status;
   WP_clock_rec_state state;
   WP_clock_rec_stats stats={0};
   WP_U32 line_index, clock_rec_if_id;

   res = sscanf(StrPrm, "%d %d %d", &cmd, &pw,&seconds);


#ifdef WTI_CRT_HOST_ADDRESS
   host_ptr_cr_base = WTI_CRT_HOST_ADDRESS;
#endif

   if (res !=2 && res !=3)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return;
   }

   if(!host_ptr_cr_base)
   {
      WTI_TerminatePrintError("Invalid cr base (enter manually in code)", __LINE__);
      return;
   }


   line_index = the_system->pw[pw].line_index;
   clock_rec_if_id = the_system->clock_rec[line_index].clock_rec_if_id;

   if(global_cr_operation_mode == WP_IW_CLOCK_REC_ADAPTIVE_MODE)
   {
      /* ACR */
      iterations = seconds/the_system->clock_rec[pw].divisor_update_period;
      if(!iterations)
         iterations = 1;
      delay = the_system->clock_rec[pw].divisor_update_period * 1000000;
   }
   else
	   {
      /* DCR */
     iterations = seconds*the_system->clock_rec[pw].divisor_update_period;
      if(!iterations)
         iterations = 1;
      delay = 1000000 / the_system->clock_rec[pw].divisor_update_period;
	   }

   printf("iter %d  delay %d   period %d ",iterations,delay,the_system->clock_rec[pw].divisor_update_period);
   printf("host_ptr_cr_base 0x%08x \n",(WP_U32)host_ptr_cr_base);


   while(ii<iterations)
			   {
      ii++;

      status = WP_CesopClockRecStateGet(the_system->pw[pw].psn2tdm_flow_agg,
                                        &state);
      printf("%d ",state);

      if(global_cr_operation_mode == WP_IW_CLOCK_REC_ADAPTIVE_MODE)
      {
         /* ACR */
         /* DivisorLow */
         host_ptr  = (WP_S32 *)(host_ptr_cr_base + pw*0x80 + 0x48);
         printf("DIV( %x %d )",*host_ptr,*host_ptr);

         /* Phase error */
         host_ptr  = (WP_S32 *)(host_ptr_cr_base + pw*0x80 + 0x14);
         printf("Ph( %d )",*host_ptr);

         /* Delta timestamps */
         host_ptr   = (WP_S32 *)(WTI_CRT_B254_ADDRESS + pw*0x40 + 0x8);
         printf("TS( %d %d )",*host_ptr++,*host_ptr);


         /* Stats */
         status = WP_ClockRecStatistics(the_system->wpid, clock_rec_if_id, &stats,WP_IW_CESOP_PORT_UFE);
         printf("Dev( %d )",stats.network_ave_noise);

      }
      else
      {
         /* DCR */

         /* DivisorLow */
         host_ptr  = (WP_S32 *)(host_ptr_cr_base + pw*0x80 + 0x18);
         printf("DIV( %x %d )",*host_ptr,*host_ptr);

         /* Ave divisor  */
         host_ptr  = (WP_S32 *)(host_ptr_cr_base + pw*0x80 + 0x1C);
         printf("%d ",*host_ptr);

      }

      /* Delta timestamps */
      host_ptr   = (WP_S32 *)(WTI_CRT_B254_ADDRESS + pw*0x40 + 0x8);
      printf("TS( %d %d )",*host_ptr++,*host_ptr);


				   printf("\n");
      WP_Delay(delay);
			   }


#endif //WTI_CESOP_CLOCK_RECOVERY_ENABLE
}




#if defined __WT_UFE412__ || defined __WT_UFE448__

WP_U32 CLI_F_ClockRecSystemsStatisticsReset(char *StrPrm)
{
#if WTI_CESOP_CLOCK_RECOVERY_ENABLE

   WP_CHAR temp_buf[100];
   WP_U32 res, cmd, pw_index = 0;
   res = sscanf(StrPrm, "%d", &cmd);

   if (res != 1)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return WT_FAIL;
   }

   printf("Reset Statistics from CRS %d to CRS %d \n", 0, total_number_of_pw);
   for(pw_index = 0; pw_index < total_number_of_pw; pw_index++)
   {
      sprintf(temp_buf,"0 %d", pw_index);
      CLI_F_ClockRecStatisticsReset(temp_buf);
   }
   return WT_PASS;
#else
   printf("Clock recovery features are disabled\n");
   return WT_PASS;
#endif
}

#else

WP_U32 CLI_F_ClockRecSystemsStatisticsReset(char *StrPrm)
{
#if WTI_CESOP_CLOCK_RECOVERY_ENABLE

   WP_CHAR temp_buf[100];
   WP_U32 res, cmd, i, cr_mode_start=0, cr_mode_end=335;
   WP_U32 cr_array[399] = {0};
   res = sscanf(StrPrm, "%d", &cmd);

   if (res != 1)
   {
      WTI_TerminatePrintError("Invalid number of parameters", __LINE__);
      return WT_FAIL;
   }
#if defined __WT_UFE412__ || defined __WT_UFE448__
/* find active CRS */
   for (i=0; i<336; i++)
      cr_array[i] = 1;

#else /* not __WT_UFE412__ || __WT_UFE448__ */


   /* find active CRS */
   for (i=0; i<global_active_cr_async_mode; i++)
      cr_array[i] = 1;
#if WTI_EXT_DIFF_MODE_ACTIVE
   for (i=64; i<64+global_active_cr_extdiff_mode; i++)
      cr_array[i] = 1;

#endif  /* WTI_EXT_DIFF_MODE_ACTIVE */

/* find first and last CRS (for display) */
   for (i=0; i<399; i++)
      if (cr_array[i] != 0)
      {
         cr_mode_start = i;
         break;
      }
   for (i=399; i>0; i--)
      if (cr_array[i] != 0)
      {
         cr_mode_end = i;
         break;
      }
#endif /* __WT_UFE412__ || __WT_UFE448__ */



   printf("Reset Statistics from CRS %d to CRS %d \n", cr_mode_start, cr_mode_end);
   for(i=0; i<399; i++)
   {
      if (cr_array[i] == 0)
         continue;

      sprintf(temp_buf,"0 %d", i);
      CLI_F_ClockRecStatisticsReset(temp_buf);
   }
   return WT_PASS;
#else
   printf("Clock recovery features are disabled\n");
   return WT_PASS;
#endif
}
#endif

#if defined __WT_UFE412__ || defined __WT_UFE448__
WP_U32 CLI_F_ClockRecSystemsStatisticsStatus(char *StrPrm)
{
#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
   WP_CHAR temp_buf[100];
   WP_status status;
   WP_U32 res, cmd,pw_index, fails = 0;
   WP_U32 line_index, clock_rec_if_id;
   WP_U32 line_cr_operation_mode;

   WP_clock_rec_stats stats;
   res = sscanf(StrPrm, "%d", &cmd);


   if (res != 1)
   {
      WTI_TerminatePrintError("CLI_F_ClockRecSystemsStatisticsStatus Invalid number of parameters", __LINE__);
      return WT_FAIL;
   }

   for(pw_index = 0; pw_index < total_number_of_pw; pw_index++)
   {
	   line_index = the_system->pw[pw_index].line_index;
	   clock_rec_if_id = the_system->clock_rec[line_index].clock_rec_if_id;

	   memset(&stats, 0, sizeof(WP_clock_rec_stats));
	   status = WP_ClockRecStatistics(the_system->wpid, clock_rec_if_id, &stats, WP_IW_CESOP_PORT_UFE);

	   if (status != WP_OK)
	   {
		   printf("WP_ClockRecStatistics failed: %s\n", WP_error_name[WP_ERROR(status)]);
		   break;
	   }

	   /* Extract the CR operation mode */
	   line_cr_operation_mode = the_system->clock_rec[line_index].operation_mode;

	   WP_Delay(100); /* Prevent a MIPS Exaption error  */

	   if (line_cr_operation_mode == WP_IW_CLOCK_REC_DIFFERENTIAL_MODE)
	   {
		   /* Differential mode or extended differential mode */
		   if((stats.reset_events)||
				   (stats.holdover_period_expired_events)||
				   (stats.invalid_input_timestamp)||
				   (stats.invalid_output_timestamp)||
				   (stats.invalid_emphy_rx_timestamp))
		   {
			   sprintf(temp_buf,"Statistics of PW %d are not zero - FAIL \n", pw_index);
			   WTI_PRINT_ERROR((temp_buf));
                           if(stats.reset_events)
                           {
                              WTI_PRINT_ERROR(("reset_events\n"));
                           }
                           if(stats.holdover_period_expired_events)
                           {
                              WTI_PRINT_ERROR(("holdover_period_expired_events\n"));
                           }
                           if(stats.invalid_input_timestamp)
                           {
                              WTI_PRINT_ERROR(("invalid_input_timestamp\n"));
                           }
                           if(stats.invalid_output_timestamp)
                           {
                              WTI_PRINT_ERROR(("invalid_output_timestamp\n"));
                           }
                           if(stats.invalid_emphy_rx_timestamp)
                           {
                              WTI_PRINT_ERROR(("invalid_emphy_rx_timestamp\n"));
                           }
                           printf("\n");
			   fails++;
		   }
	   }
	   else if (line_cr_operation_mode == WP_IW_CLOCK_REC_ADAPTIVE_MODE)
	   {
		   /* Adaptive mode */
		   if((stats.reset_events)||
				   (stats.holdover_period_expired_events)||
				   (stats.lost_over_limit)||
				   (stats.phase_error_holdover)||
				   (stats.invalid_emphy_rx_timestamp))
		   {
			   sprintf(temp_buf,"Statistics of PW %d are not zero - FAIL \n", pw_index);
			   WTI_PRINT_ERROR((temp_buf));
                           if(stats.reset_events)
                           {
                              WTI_PRINT_ERROR(("reset_events\n"));
                           }
                           if(stats.holdover_period_expired_events)
                           {
                              WTI_PRINT_ERROR(("holdover_period_expired_events\n"));
                           }
                           if(stats.lost_over_limit)
                           {
                              WTI_PRINT_ERROR(("lost_over_limit\n"));
                           }
                           if(stats.phase_error_holdover)
                           {
                              WTI_PRINT_ERROR(("phase_error_holdover\n"));
                           }
                           if(stats.invalid_emphy_rx_timestamp)
                           {
                              WTI_PRINT_ERROR(("invalid_emphy_rx_timestamp\n"));
                           }
                           printf("\n");                           
			   fails++;
		   }
	   }
   }
   if (!fails)
   {
      sprintf(temp_buf,"Statistics of all CRS are zero - PASS \n");
      WTI_PRINT_NOTE((temp_buf));
      return WT_PASS;
   }
   else return WT_FAIL;
#else
   printf("Clock recovery features are disabled\n");
   return WT_PASS;
#endif
}

#else /* not __WT_UFE412__ || __WT_UFE448__ */

WP_U32 CLI_F_ClockRecSystemsStatisticsStatus(char *StrPrm)
{
#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
   WP_CHAR temp_buf[100];
   WP_status status;
   WP_U32 res, cmd,i, fails = 0;
   WP_U32 cr_array[399] = {0};
   WP_U32 line_cr_operation_mode;
   WP_clock_rec_stats stats;
   res = sscanf(StrPrm, "%d", &cmd);


   if (res != 1)
   {
      WTI_TerminatePrintError("CLI_F_ClockRecSystemsStatisticsStatus Invalid number of parameters", __LINE__);
      return WT_FAIL;
   }
#if defined __WT_UFE412__ || defined __WT_UFE448__
/* find active CRS */
   for (i=0; i<336; i++)
      cr_array[i] = 1;
#else /* not __WT_UFE412__ || __WT_UFE448__ */
   /* find active CRS */
   for (i=0; i<global_active_cr_async_mode; i++)
      cr_array[i] = 1;
#if WTI_EXT_DIFF_MODE_ACTIVE
   for (i=64; i<64+global_active_cr_extdiff_mode; i++)
      cr_array[i] = 1;
#endif

#endif /* not __WT_UFE412__ || __WT_UFE448__ */

   for(i=0; i<399; i++)
   {
      if (cr_array[i] == 0)
         continue;

      memset(&stats, 0, sizeof(WP_clock_rec_stats));
      status = WP_ClockRecStatistics(the_system->wpid, i, &stats,
#if WTI_CESOP_TDI
                                     WP_IW_CESOP_PORT_TDM
#else
                                     WP_IW_CESOP_PORT_UFE
#endif
      );

      if (status != WP_OK)
      {
         printf("WP_ClockRecStatistics failed: %s\n", WP_error_name[WP_ERROR(status)]);
         break;
      }

      /* Extract the CR operation mode */
      if (i < WTI_CESOP_MAX_CLOCK_REC_INTERFACES)
         line_cr_operation_mode = the_system->clock_rec[i].operation_mode;
      else
         line_cr_operation_mode = the_system->clock_rec[i-WTI_CESOP_MAX_CLOCK_REC_INTERFACES+global_active_cr_async_mode].operation_mode;

      WP_Delay(100); /* Prevent a MIPS Exaption error  */

      if (line_cr_operation_mode == WP_IW_CLOCK_REC_DIFFERENTIAL_MODE)
      {
         /* Differential mode or extended differential mode */
         if((stats.reset_events)||
            (stats.holdover_period_expired_events)||
            (stats.invalid_input_timestamp)||
            (stats.invalid_output_timestamp)||
            (stats.invalid_emphy_rx_timestamp))
         {
            sprintf(temp_buf,"Statistics of CRS %d are not zero - FAIL \n",i);
            WTI_PRINT_ERROR((temp_buf));
            fails++;
         }
      }
      else if (line_cr_operation_mode == WP_IW_CLOCK_REC_ADAPTIVE_MODE)
      {
         /* Adaptive mode */
         if((stats.reset_events)||
            (stats.holdover_period_expired_events)||
            (stats.lost_over_limit)||
            (stats.phase_error_holdover))
         {
            sprintf(temp_buf,"Statistics of CRS %d are not zero - FAIL \n",i);
            WTI_PRINT_ERROR((temp_buf));
            fails++;
         }
      }
   }
   if (!fails)
   {
      sprintf(temp_buf,"Statistics of all CRS are zero - PASS \n");
      WTI_PRINT_NOTE((temp_buf));
      return WT_PASS;
   }
   else return WT_FAIL;
#else
   printf("Clock recovery features are disabled\n");
   return WT_PASS;
#endif
}
#endif /* __WT_UFE412__ || __WT_UFE448__ */

WP_U32 CLI_F_GlobalClockRecoveryShowState(char *StrPrm)
{
#if WTI_CESOP_CLOCK_RECOVERY_ENABLE

   WP_U32 *cr_array;
   WP_U32 *timeout_array;
   WP_U32 res, cmd, pw_index, i, j, iterations, active_clocks_count=0, normal_state_count=0, line_cr_operation_mode=0;
   WP_status status;
   WP_clock_rec_state state;
   WP_CHAR state_array[5][30]={"NORMAL_STATE","HOLDOVER_STATE","DEFAULT_WAIT_STATE","CONVERGENCE_STATE","PMSM_STATE"};

   cr_array = (WP_U32*)malloc(total_number_of_pw * sizeof(WP_U32));
   memset (cr_array, WTI_UNUSED_SLOT, total_number_of_pw * sizeof(WP_U32));

   timeout_array = (WP_U32*)malloc(total_number_of_pw * sizeof(WP_U32));
   memset (timeout_array, 0, total_number_of_pw * sizeof(WP_U32));

   res = sscanf(StrPrm, "%d %d", &cmd, &iterations);
   if (res != 2 && res != 1)
   {
      WTI_TerminatePrintError("CLI_F_GlobalClockRecoveryShowState Invalid number of parameters", __LINE__);
      return WT_FAIL;
   }
   else if (res == 2)
   {
      if( (iterations < 1) || (iterations > 10) )
      {
         WTI_TerminatePrintError("CLI_F_GlobalClockRecoveryShowState Invalid number of Iterations\n iterations is >0 and <10", __LINE__);
         return WT_FAIL;
      }
   }
   else if (res == 1)
      iterations = 5;


   for(pw_index = 0; pw_index < total_number_of_pw ;pw_index++)
   {
      if(the_system->pw[pw_index].clock_rec_tx_enable)
      {
         cr_array[pw_index] = WP_IW_CLOCK_REC_DEFAULT_WAIT_STATE;
         active_clocks_count++;
      }
   }

   for (j=0; j<iterations;)
   {
      for(pw_index=0; pw_index < total_number_of_pw; pw_index++)
      {

         if (cr_array[pw_index] == WTI_UNUSED_SLOT)
            continue;

         if(the_system->pw[pw_index].clock_rec_tx_enable)
         {
            /* Get CR operation mode */
            line_cr_operation_mode = the_system->clock_rec[pw_index].operation_mode;
            /* Get CR system state */
            status = WP_CesopClockRecStateGet(the_system->pw[pw_index].psn2tdm_flow_agg,&state);
            WTI_TerminateOnError(status , "CLI_F_GlobalClockRecoveryShowState",__LINE__);

            switch (state)
            {
            case (WP_IW_CLOCK_REC_NORMAL_STATE):
               if (cr_array[pw_index] != WP_IW_CLOCK_REC_NORMAL_STATE)
                  normal_state_count++;
            break;

            case (WP_IW_CLOCK_REC_HOLDOVER_STATE):
               if (1)
               {
                  timeout_array[pw_index]=0;
                  if (cr_array[pw_index] == WP_IW_CLOCK_REC_NORMAL_STATE)
                     normal_state_count--;
               }
               else
               {
                  WTI_TerminatePrintError("CLI_F_GlobalClockRecoveryShowState HOLDOVER STATE Is Invalid", __LINE__);
                  printf("ERROR in index %d \n",pw_index);
                  return WT_FAIL;

               }
            break;

            case (WP_IW_CLOCK_REC_DEFAULT_WAIT_STATE):
               if (cr_array[pw_index] == WP_IW_CLOCK_REC_DEFAULT_WAIT_STATE)
               {
                  WP_Delay(1000000);
                  status = WP_CesopClockRecStateGet(the_system->pw[pw_index].psn2tdm_flow_agg,&state);
                  WTI_TerminateOnError(status , "CLI_F_ClockRecoveryShowState",__LINE__);
                  if ( state == WP_IW_CLOCK_REC_DEFAULT_WAIT_STATE )
                  {
                     WTI_TerminatePrintError("CLI_F_GlobalClockRecoveryShowState DEFAULT_WAIT_STATE timeout", __LINE__);
                     printf("ERROR in index %d \n",pw_index);
                     return WT_FAIL;
                  }
               }
               else
               {
                  WTI_TerminatePrintError("CLI_F_GlobalClockRecoveryShowState DEFAULT_WAIT_STATE Is Invalid", __LINE__);
                  printf("ERROR in index %d \n",pw_index);
                  return WT_FAIL;
               }
            break;

            case (WP_IW_CLOCK_REC_CONVERGENCE_STATE):
               if ( cr_array[pw_index] == WP_IW_CLOCK_REC_NORMAL_STATE )
               {
                  WTI_TerminatePrintError("CLI_F_GlobalClockRecoveryShowState CONVERGENCE State Is Invalid", __LINE__);
                  printf("ERROR in index %d \n",pw_index);
                  return WT_FAIL;
               }
               else timeout_array[pw_index]++;
            break;

            case (WP_IW_CLOCK_REC_PMSM_STATE):
               if ( (cr_array[pw_index] == WP_IW_CLOCK_REC_NORMAL_STATE) ||
                     (cr_array[pw_index] == WP_IW_CLOCK_REC_CONVERGENCE_STATE) )
               {
                  WTI_TerminatePrintError("CLI_F_GlobalClockRecoveryShowState PMSM State Is Invalid", __LINE__);
                  printf("ERROR in index %d \n",pw_index);
                  return WT_FAIL;
               }
               else timeout_array[pw_index]++;
            break;

            default:
               WTI_TerminatePrintError("CLI_F_GlobalClockRecoveryShowState Invalid value for WP_clock_rec_state ", __LINE__);
               return WT_FAIL;
            }

            cr_array[pw_index]=state;
            if ( timeout_array[pw_index] >= iterations )
            {
               if (line_cr_operation_mode != WP_IW_CLOCK_REC_DIFFERENTIAL_MODE)   /* Adaptive mode */
               {
                  printf("Index %d is still in %s \n",pw_index,state_array[state]);
                  printf("It takes 30 seconds to get a NORMAL state in adaptive Clock Recovery!\n");
                  printf("Please wait or press any key to ignore\n");
                  for (i=0; i<30; i++)
                  {
                     if (WTI_SampleSerial())
                     {
                       printf("CLI_F_GlobalClockRecoveryShowState function aborted by user!\n");
                       return WT_PASS;
                     }
                     else WP_Delay(1000000);
                  }
                  iterations = iterations + 5;
               }
               else
               {
                  printf("Index %d is still in %s \n",pw_index,state_array[state]);
                  WTI_TerminatePrintError("CLI_F_GlobalClockRecoveryShowState failed on Timeout", __LINE__);
                  printf("Index %d is not in NORMAL State \n",pw_index);
                  return WT_FAIL;
               }
            }
         }
      }

      if ( normal_state_count == active_clocks_count )
      {
         j++;
         WP_Delay(500000);
      }
      else WP_Delay(200);

   }


   if ( (j >= iterations) && (normal_state_count == active_clocks_count) )
   {
      WTI_PRINT_NOTE(("CLI_F_GlobalClockRecoveryShowState Passed, All Clocks On NORMAL State \n\n"));
      return WT_PASS;
   }
   else
   {
      WTI_TerminatePrintError("CLI_F_GlobalClockRecoveryShowState Failed\n", __LINE__);
      return WT_FAIL;
   }

#else
   printf("Clock recovery features are disabled\n");
   return WT_PASS;
#endif
}

WP_U32 CLI_F_GlobalStatisticsCheck(char *StrPrm)
{
   WP_U32 result, i, j, res, cmd, reset, iterations=3, test_itr;
   WP_CHAR temp_buf[7000];

   WP_Delay(1000000);

   res = sscanf(StrPrm, "%d %d %d", &cmd, &reset, &test_itr);

   if (res == 1)
   {
      test_itr = 1;
      reset = 1;
   }
   else if (res == 2)
   {
      if (reset != 0 && reset != 1)
      {
         WTI_TerminatePrintError("CLI_F_GlobalStatisticsCheck Invalid value for reset. reset should be 0 or 1 ", __LINE__);
         return WT_FAIL;

      }
      test_itr = 1;
   }
   else if (res == 3)
   {
      if (reset != 0 && reset != 1)
      {
         WTI_TerminatePrintError("CLI_F_GlobalStatisticsCheck Invalid value for reset.reset should be 0 or 1 ", __LINE__);
         return WT_FAIL;

      }
      if( (test_itr < 1) || (test_itr > 20) )
      {
         WTI_TerminatePrintError("CLI_F_GlobalStatisticsCheck Invalid number of Iterations. Test iterations should > 0 and < 20", __LINE__);
         return WT_FAIL;
      }

   }
   else
   {
      WTI_TerminatePrintError("CLI_F_GlobalStatisticsCheck Invalid number of parameters", __LINE__);
      return WT_FAIL;
   }

   for (j=0;j<test_itr;j++)
   {
      if(test_itr > 1)
         printf("\n--- Iteration number: %d ---\n", j+1);

      /* Reset PWE3 statistics */
      if (reset)
      {
         result = CLI_F_Pwe3StatisticsClear("0");
         if (result == WT_FAIL)
         {
            printf("PWE-3 Clear Statistics: Failed \n");
            WTI_PRINT_ERROR(("\n Test Failed \n"));
            return WT_FAIL;
         }
         else WTI_PRINT_NOTE(("\n PWE-3 Clear Statistics: Passed \n\n"));
      }

#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
      /* Reset Clock Recovery Systems Statistics*/
      if (reset)
      {
         result = CLI_F_ClockRecSystemsStatisticsReset("0");
         if (result == WT_FAIL)
         {
            printf("Clock Recovery Reset System Statistics : Failed \n");
            WTI_PRINT_ERROR(("\n Test Failed \n"));
            WT_PrintTestInfo(WT_FAIL);
            return WT_FAIL;
         }
         else WTI_PRINT_NOTE(("\n Clock Recovery Reset System Statistics: Passed \n\n"));
      }
#endif /* WTI_CESOP_CLOCK_RECOVERY_ENABLE */

      if (reset) WP_Delay(2000000); /* Give the data time to pass */

      /* Check PWE3 statistics */
      for (i=0;i<iterations;i++)
      {
         result = CLI_F_Pwe3StatisticsCheck("0");
         if (result == WT_FAIL)
         {
            sprintf(temp_buf, "8 1 %d 1", total_number_of_pw);

            CLI_F_Pwe3StatisticsCheck(temp_buf);
            printf("PWE-3 Check Statistics: Failed \n");
            WTI_PRINT_ERROR(("\n Test Failed \n"));
            WT_PrintTestInfo(WT_FAIL);
            return WT_FAIL;
         }
         WP_Delay(2000000);
      }

      WTI_PRINT_NOTE(("\n PWE-3 Check Statistics: Passed \n\n"));

#if !WTI_CESOP_TDI

      /* Check UFE Events */

      /* The first call for WT_UfeReadEvents() used to reset the statistics*/
      WT_UfeReadEvents(&the_system->ufe);
      printf("\n");

      for (i=0;i<iterations;i++)
      {
         result = WT_UfeReadEvents4Automation(&the_system->ufe);
         if (result == WT_FAIL)
         {
            printf("There are UFE Events \n");
            WTI_PRINT_ERROR(("\n Test Failed \n"));
            WT_PrintTestInfo(WT_FAIL);
            return WT_FAIL;
         }
         printf("\n");
         WP_Delay(2000000);
      }
      WTI_PRINT_NOTE(("There are no UFE Events \n\n"));
#ifndef __linux__
#ifdef __WT_UFE3__
      WTI_Check_UFE_Temperature_4Automation("0");
#endif /* WT_UFE3 */
#endif
#endif

#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
      WP_Delay(2000000);
      /* Check Clock Recovery Systems Statistics*/
      result = CLI_F_ClockRecSystemsStatisticsStatus("0");
      if (result == WT_FAIL)
      {
         printf("Clock Recovery System Statistics Status: Failed \n");
         WTI_PRINT_ERROR(("\n Test Failed \n"));
         WT_PrintTestInfo(WT_FAIL);
         return WT_FAIL;
      }
      else WTI_PRINT_NOTE(("\n Clock Recovery System Statistics Status: Passed \n\n"));

      /* Check Clock Recovery State*/
      result = CLI_F_GlobalClockRecoveryShowState("0");
      if (result == WT_FAIL)
      {
         printf("Clock Recovery System State: Failed \n");
         WTI_PRINT_ERROR(("\n Test Failed \n"));
         WT_PrintTestInfo(WT_FAIL);
         return WT_FAIL;
      }
      else WTI_PRINT_NOTE(("\nClock Recovery State: Passed \n"));

#if defined __WT_UFE412__ || defined __WT_UFE448__
   /* Check ppb offset for each line that uses CR */
   sprintf(temp_buf, "0 1000 1"); /* cmd, ppb_margin (1ppm), display all lines */
   result = CLI_F_ClockRecStatisticsPPB(temp_buf);
   if (result == WT_FAIL)
   {
      printf("Clock Recovery ppb Statistics check: Failed\n");
      WTI_PRINT_ERROR(("\n Test Failed \n"));
      WT_PrintTestInfo(WT_FAIL);
      return WT_FAIL;
   }
   else
      WTI_PRINT_NOTE(("\nClock Recovery ppb Statistics check: Passed\n"));

#endif /* WT_UFE4 */
#endif /* WTI_CESOP_CLOCK_RECOVERY_ENABLE */

   /* Check data path using PRBS validation */
#if defined __WT_UFE412__ || defined __WT_UFE448__
#ifdef WT_UFE_FRAMER
#if ( !(WTI_CESOP_TDI) && (WTI_CESOP_REGRESSION_TEST) )

#if A_AUTOMATION
      result = CLI_F_UfeFramerPrbsCheck("0");
#else
#ifdef __WT_UFE412__
      sprintf(temp_buf, "0 0,48,100,200,300,328");
      result = CLI_F_UfeFramerPrbsCheck(temp_buf);
#elif defined __WT_UFE448__
      sprintf(temp_buf, "0 0,84,168,252,672,756,840,924");
      result = CLI_F_UfeFramerPrbsCheck(temp_buf);
#endif
#endif /* A_AUTOMATION */
      if (result == WT_FAIL)
      {
         printf("UFE Framer PRBS check: Failed\n");
         WTI_PRINT_ERROR(("\n Test Failed \n"));
         WT_PrintTestInfo(WT_FAIL);
         return WT_FAIL;
      }
      else
         WTI_PRINT_NOTE(("\nUFE Framer PRBS Check: Passed\n"));
#endif /* ( !(WTI_CESOP_TDI) && (WTI_CESOP_REGRESSION_TEST) ) */
#endif /* WT_UFE_FRAMER */
#endif /* WT_UFE4 */

      if (test_itr > 1) reset=0;
   }
   WTI_PRINT_NOTE(("\n Test Passed \n\n"));
   WT_PrintTestInfo(WT_PASS);
   return WT_PASS;
}

void WT_PrintTestInfo(WP_U32 status)
{
	WP_CHAR output_str[100] = "", temp_buf[100] = "";
#if !WTI_CESOP_TDI
	WT_ufe *ufe = &(the_system->ufe);
#if WTI_DUAL_EMPHY
        WT_ufe *second_ufe = &(the_system->second_ufe);
#endif
	WP_U32 num_of_lines = 0, i = 0;
#endif

	if(status == WT_PASS)
		strcat(output_str,"Test Passed - ");
	else
		strcat(output_str,"Test Failed - ");

#if defined __WT_UFE412__
	strcat(output_str,test_setup.test_name);
	strcat(output_str," ");
#endif

#if WTI_CESOP_TDI

	strcat(output_str,"TDI ");

	if(tdm_config[0].op_mode) /* op_mode T1 :0 , E1 :1 */
		strcat(output_str,"E1 ");
	else
		strcat(output_str,"T1 ");

	if(tdm_config[0].op_mode) /* framing_mode unframed :0 , framed :1 */
		strcat(output_str,"Framed ");
	else
		strcat(output_str,"Unframed ");

	if(tdm_config[0].cas_mode) /* cas_mode no case :0 , cas :1 */
		strcat(output_str,"with CAS ");
	else
		strcat(output_str,"no CAS ");

	sprintf(temp_buf,"%d lines %d PWs %d PWs per line ", WTI_ACTIVE_TDI_PORTS, total_number_of_pw, total_number_of_pw/WTI_ACTIVE_TDI_PORTS);
	strcat(output_str,temp_buf);

#else

#if defined __WT_UFE412__
	strcat(output_str,"UFE412 ");
#elif defined __WT_UFE448__
	strcat(output_str,"UFE448 ");
#else
	strcat(output_str,"UFE3 ");
#endif

	switch(ufe->line_params[0].transfer_type)
	{
	case WUFE_SDH_TYPE_T1:
		strcat(output_str,"SDH T1 ");
		break;
	case WUFE_SDH_TYPE_E1:
		strcat(output_str,"SDH E1 ");
		break;
	case WUFE_SONET_TYPE_T1:
		strcat(output_str,"SONET T1 ");
		break;
	case WUFE_SONET_TYPE_E1:
		strcat(output_str,"SONET E1 ");
		break;
	case WUFE_CAD_TYPE_T1:
		strcat(output_str,"CAD T1 ");
		break;
	case WUFE_CAD_TYPE_E1:
		strcat(output_str,"CAD E1 ");
		break;
	case WUFE_SBI_TYPE_T1:
		strcat(output_str,"SBI T1 ");
		break;
	case WUFE_SBI_TYPE_E1:
		strcat(output_str,"SBI E1 ");
		break;
	default:
		break;
	}

	if(ufe->line_params[0].framed == WUFE_FRAMED)
		strcat(output_str,"Framed ");
	else
		strcat(output_str,"Unframed ");

	for(i = 0; i < ufe->max_lines; i++)
		if(ufe->line_params[i].valid)
			num_of_lines++;
#if WTI_DUAL_EMPHY
	for(i = 0; i < second_ufe->max_lines; i++)
		if(ufe->line_params[i].valid)
			num_of_lines++;
#endif

	sprintf(temp_buf,"%d lines %d PWs %d PWs per line ", num_of_lines, total_number_of_pw, total_number_of_pw/num_of_lines);
	strcat(output_str,temp_buf);
#endif

	if(global_cr_operation_mode == WP_IW_CLOCK_REC_ADAPTIVE_MODE)
		strcat(output_str,"Adaptive Clock Recovery\n");
	else
		strcat(output_str,"Differential Clock Recovery\n");

	if(status == WT_PASS)
		WTI_PRINT_NOTE((output_str));
	else
		WTI_PRINT_ERROR((output_str));
}


void WTI_FLR_Calculation(void)
{
   struct t_64 {
      WP_U32 high;
      WP_U32 low;
   } *temp_stat;
  
   WP_status status; 
   WP_stats_emphy_trans_pwe3_tx  tx_stats;
   WP_stats_iw_cesop_psn2tdm cesop_stats_psn2tdm;
   WP_U32 pw_index;
   WP_U64 dummy_packets, valid_packets;
   WP_U64 stray_packets, malformed_packets, rdi_dropped_packets, cw_ais_dropped_packets;
   WP_U64 temp1, temp2, frame_loss_ratio;

for (pw_index = 0; pw_index < total_number_of_pw; pw_index ++)
{
   dummy_packets = valid_packets = 0;
   stray_packets = malformed_packets = rdi_dropped_packets = cw_ais_dropped_packets = 0;

   memset(&tx_stats,0,sizeof(WP_stats_emphy_trans_pwe3_tx));
   memset(&cesop_stats_psn2tdm,0,sizeof(WP_stats_iw_cesop_psn2tdm));

   status = WP_ChannelStatistics(the_system->pw[pw_index].trans_tx, &tx_stats);
   if (status != WP_OK) {
   	WTI_TerminateOnError(status, "WP_ChannelStatistics", __LINE__);
   	return;
   }
   
   dummy_packets = tx_stats.dummy_packet;
   valid_packets = tx_stats.valid_packets;

   status = WP_IwFlowStatistics(the_system->pw[pw_index].psn2tdm_flow_agg,
                                   WP_IW_FLOW_STAT_CESOP_PSN2TDM,
                                   &cesop_stats_psn2tdm);
   if (status != WP_OK){
      WTI_TerminateOnError(status, "WP_IwFlowStatistics", __LINE__);
      return;
   }

   stray_packets = cesop_stats_psn2tdm.stray_packets;
   malformed_packets = cesop_stats_psn2tdm.malformed_packets;
   rdi_dropped_packets = cesop_stats_psn2tdm.rdi_dropped_packets;
   cw_ais_dropped_packets = cesop_stats_psn2tdm.cw_ais_drop_packets;

   temp1 = dummy_packets-(stray_packets + malformed_packets + cw_ais_dropped_packets + rdi_dropped_packets);
   temp2 = valid_packets + temp1;

   if (temp2 == 0) {
   	printf("sum of valid and dropped packets is ZERO for PW %d!\n", pw_index);
   	continue;
   }
   
   frame_loss_ratio = temp1/temp2;

   temp_stat = (struct t_64 *)&frame_loss_ratio;
   printf("FLR for PW %d is:  %08x%08x\n", pw_index, temp_stat->high, temp_stat->low);
}

}

void WTI_FER_Calculation(void)

{
   struct t_64 {
      WP_U32 high;
      WP_U32 low;
   } *temp_stat;

   WP_status status;     
   WP_U32 pw_index;
   WP_stats_emphy_trans_pwe3_tx  tx_stats;
   WP_U64 dummy_packets, valid_packets, underrun_dataunit;
   WP_U64 tx_dataunit_size, payload_size;
   WP_U64 data_units_per_packet;
   WP_U64 temp1, temp2, frame_error_ratio;

for (pw_index = 0; pw_index < total_number_of_pw; pw_index ++)
{
   dummy_packets = valid_packets = underrun_dataunit = 0;

   tx_dataunit_size = payload_size = 0;
   data_units_per_packet = 0;

   memset(&tx_stats,0,sizeof(WP_stats_emphy_trans_pwe3_tx));

   status = WP_ChannelStatistics(the_system->pw[pw_index].trans_tx, &tx_stats);
   if (status != WP_OK) {
   	WTI_TerminateOnError(status, "WP_ChannelStatistics", __LINE__);
   	return;
   }
   
   dummy_packets = tx_stats.dummy_packet;
   valid_packets = tx_stats.valid_packets;
   underrun_dataunit = tx_stats.underrun_dataunit;

   tx_dataunit_size = pw_info_base[pw_index].upi_trans_device.tx_dataunit_size;

   payload_size = pw_info_base[pw_index].psn2tdm_agg_param.payload_size;

   if (tx_dataunit_size == 0) {
   	printf("tx_dataunit_size is ZERO for PW %d!\n", pw_index);
   	continue;
   }
   
   data_units_per_packet = payload_size/tx_dataunit_size;

   if (data_units_per_packet == 0) {
   	printf("data_units_per_packet is ZERO for PW %d!\n", pw_index);
   	continue;
   }

   temp1 = dummy_packets + underrun_dataunit/data_units_per_packet;
   temp2 = temp1 + valid_packets;

   if (temp2 == 0) {
   	printf("totoal data transmitted by channel %d is ZERO!\n", pw_index);
   	continue;
   }
   
   frame_error_ratio = temp1/temp2;
   
   temp_stat = (struct t_64 *)&frame_error_ratio;
   printf("FER for PW %d is:  %08x%08x\n", pw_index, temp_stat->high, temp_stat->low);
}

}

/*
 *
 * This function checks if the system is converged.
 * Should replace the ANT-20 when in regression tests
 * Still in development and this is why it is marked out with #if 0
 *
 * Ohad Benjamin 1/1/2012
 *
 */
#if 0
WP_U32 CLI_F_ClockRecoveryConvergenceState(char *StrPrm)
{
#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
	WP_U32 line_index, line_counter, cr_index, avg_window_size = 30, i = 0, wait_period_in_seconds = 1, converged;
	WP_S32 avg_ppb[WTI_CESOP_MAX_CLOCK_REC_INTERFACES] = {0},ppb_samples[WTI_CESOP_MAX_CLOCK_REC_INTERFACES][avg_window_size], avg_min, avg_max;
	WP_CHAR temp_buf[20];
	WP_clock_rec_stats stats;
	WP_status status;

	for(i = 0; i < avg_window_size; i++)
	{
		for (line_index = 0; line_index < WTI_CESOP_MAX_CLOCK_REC_INTERFACES; line_index++)
		{

			if (!the_system->clock_rec[line_index].operation_mode)
				continue;

			cr_index = the_system->clock_rec[line_index].clock_rec_if_id;

			memset(&stats, 0, sizeof(WP_clock_rec_stats));
			status = WP_ClockRecStatistics(the_system->wpid, cr_index, &stats, WP_IW_CESOP_PORT_UFE);
			if (status != WP_OK)
			{
				printf("WP_ClockRecStatistics failed: %s\n",
						WP_error_name[WP_ERROR(status)]);
				break;
			}

			if(the_system->clock_rec[line_index].operation_mode == WP_IW_CLOCK_REC_ADAPTIVE_MODE)
			{
				if(the_system->clock_rec[line_index].divisor_update_period > wait_period_in_seconds)
				{
					wait_period_in_seconds = the_system->clock_rec[line_index].divisor_update_period;
				}
			}

			avg_ppb[line_index] += stats.ppb_service_to_nominal;
			ppb_samples[line_index][i] = stats.ppb_service_to_nominal;
		}

		WP_Delay(wait_period_in_seconds * 1000000);
	}

	for (line_index = 0, line_counter = 0; line_index < WTI_CESOP_MAX_CLOCK_REC_INTERFACES; line_index++)
	{
		if (!the_system->clock_rec[line_index].operation_mode)
			continue;

		cr_index = the_system->clock_rec[line_index].clock_rec_if_id;

		memset(&stats, 0, sizeof(WP_clock_rec_stats));
		status = WP_ClockRecStatistics(the_system->wpid, cr_index, &stats, WP_IW_CESOP_PORT_UFE);
		if (status != WP_OK)
		{
			printf("WP_ClockRecStatistics failed: %s\n",
					WP_error_name[WP_ERROR(status)]);
			break;
		}

		avg_ppb[line_index] = avg_ppb[line_index]/avg_window_size;

		if(avg_ppb[line_index] > 0)
		{
			avg_min = avg_ppb[line_index] - avg_ppb[line_index]/10;
			avg_max = avg_ppb[line_index] + avg_ppb[line_index]/10;
		}
		else
		{
			avg_min = avg_ppb[line_index] + avg_ppb[line_index]/10;
			avg_max = avg_ppb[line_index] - avg_ppb[line_index]/10;
		}


		converged = WP_TRUE;

		for(i = 0; i < avg_window_size; i++)
		{
			if(avg_min > ppb_samples[line_index][i] || avg_max < ppb_samples[line_index][i])
			{
				converged = WP_FALSE;
			}
		}

		if(converged == WP_TRUE)
		{
			sprintf(temp_buf, "%d. line %d CONVERGED     ", line_counter, line_index);
			WTI_PRINT_NOTE((temp_buf));
		}
		else
		{
			sprintf(temp_buf, "%d. line %d NOT CONVERGED ", line_counter, line_index);
			WTI_PRINT_ERROR((temp_buf));
		}

		if (line_counter++%7 == 4)
			printf("\n");
	}
	printf("\n");
	return WT_PASS;
#else
   printf("Clock recovery features are disabled\n");
   return WT_PASS;
#endif
}
#endif
