/*****************************************************************************
 * (C) Copyright PMC - WIN division (Wintegra) 2012.  All rights reserved.
 * PMC-Sierra (WINTEGRA) CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of PMC-Sierra (Wintegra).
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *  Appplication: ATM IMA UFE
 *  File Name   : atm_ima_tdm_stats.c
 *
 *****************************************************************************/


void
m_showWUFE (void)
{
	WUFE_SystemDisplay (0);

	return ;
}



char *link_s(int n);

void m_showLnkStatus (void)
{
	WP_ima_link_state_info link_state[NUM_OF_LINKS];
	int i = 0;

	for (i = 0; i < NUM_OF_LINKS; i ++)
	{
		printf ("\n");
		printf ("ima link[%d] status\n", i);
		printf ("================================================\n");
       		status = WP_ImaLinkState(ima_link_handle[i], &link_state[i]);
	        App_TerminateOnError(status, "WP_ImaLinkState()");
		printf ("link_state[i].ne_ima_id	=%d\n", link_state[i].ne_ima_id);
		printf ("link_state[i].fe_ima_id	=%d\n", link_state[i].fe_ima_id);
		printf ("link_state[i].ne_tx_lsm	=%s\n", link_s(link_state[i].ne_rx_lsm));
		printf ("link_state[i].ne_tx_lsm	=%s\n", link_s(link_state[i].ne_tx_lsm));
		printf ("link_state[i].fe_rx_lsm	=%s\n", link_s(link_state[i].fe_rx_lsm));
		printf ("link_state[i].fe_tx_lsm	=%s\n", link_s(link_state[i].fe_tx_lsm));
// 		printf ("link_state[i].line		=%s\n", lline_state[link_state[i].fe_tx_lsm]);
		printf ("link_state[i].delay		=%d\n", link_state[i].delay);
		printf ("link_state[i].rx_port		=%d\n", link_state[i].rx_port);
		printf ("link_state[i].tx_port		=%d\n", link_state[i].tx_port);
	}

	return ;
}


char * ima_s_fe (int n);
char * ima_s (int n);

void m_showGrpStatus (void)
{
	WP_status status;
	WP_ima_group_state_info group_state[NUM_OF_GROUPS];
	int i = 0;

       		status = WP_ImaGroupState(ima_group_handle[i], &group_state[i]);
	        App_TerminateOnError(status, "WP_ImaGroupState()");
		printf ("\n");
		printf ("Group[%d] status:\n", 0);
		printf ("=================================================\n");
	        printf("group_state[0].ne_state           =%s\n", ima_s(group_state[i].ne_state));
	        printf("group_state[0].fe_state           =%s\n", ima_s_fe(group_state[i].fe_state));
	        printf("group_state[0].g_id               =%d\n", group_state[i].g_id);
	        printf("group_state[0].ne_ima_id          =%d\n", group_state[i].ne_ima_id);
	        printf("group_state[0].ne_symmetry        =%d\n", group_state[i].ne_symmetry);
	        printf("group_state[0].ne_trl_port        =%d\n", group_state[i].ne_trl_port);
	        printf("group_state[0].ne_trl_id          =%d\n", group_state[i].ne_trl_id);
	        printf("group_state[0].ne_frame_size      =%d\n", group_state[i].ne_frame_size);
	        printf("group_state[0].ne_clock_mode      =%d\n", group_state[i].ne_clock_mode);
	        printf("group_state[0].fe_ima_id          =%d\n", group_state[i].fe_ima_id);
	        printf("group_state[0].fe_symmetry        =%d\n", group_state[i].fe_symmetry);
	        printf("group_state[0].fe_rl_id           =%d\n", group_state[i].fe_trl_id);
	        printf("group_state[0].fe_m               =%d\n", group_state[i].fe_m);
	        printf("group_state[0].allowed_delay      =%d\n", group_state[i].allowed_delay);
	        printf("group_state[0].delay      	  =%d\n", group_state[i].delay);
	        printf("group_state[0].linke_delay_delta  =%d\n", group_state[i].link_delay_delta);
	        printf("group_state[0].short_delay_port   =%d\n", group_state[i].short_delay_port);
	        printf("group_state[0].long_delay_port    =%d\n", group_state[i].long_delay_port);
	        printf("group_state[0].tx_min_links       =%d\n", group_state[i].tx_min_links);
	        printf("group_state[0].rx_min_links       =%d\n", group_state[i].rx_min_links);
	        printf("group_state[0].ne_alarm_state     =%d\n", group_state[i].ne_alarm_state);
	        printf("group_state[0].ne_alarm_type      =%d\n", group_state[i].ne_alarm_type);
	        printf("group_state[0].fe_alarm_state     =%d\n", group_state[i].fe_alarm_state);
	        printf("group_state[0].fe_alarm_type      =%d\n", group_state[i].fe_alarm_type);
		i ++;
}





/*****************************************************************************
 * Function name: App_ShowStats
 * Description  : App_ShowStats function shows all the statistics with the option menu to   
 * 				select the statistics of interest   
 * Input  params: None
 * Output params: None
 * Return val   : None
 *****************************************************************************/

void App_ShowStats(void)
{
	WP_U32 ii;
	WP_stats_enet s_hs_enet={0};
	WP_stats_tdi_atm tdi_atm_stats={0};
	WP_bport_stats bport_stats = {0};
	WP_iw_flow_stats flow_stats = {0};
	WP_iw_pwe3_stats pwe3_flow_stats = {0};
	WP_stats_aal5_tx atm_tx_channel_stats = {0};
	WP_stats_aal5_rx atm_rx_channel_stats = {0}; 
	WP_U8  ch, stats_all=0;

	do
	{       if(stats_all==0)
		{
			printf("\nMENU------------------------------------------------");
			printf("\nEnter 1 to print  Device Stats");
			printf("\nEnter 2 to print  Bport  Stats");
			printf("\nEnter 3 to print  FlwAgg Stats");
			printf("\nEnter 4 to print  Channel Stats");
			printf("\nEnter 5 to print  IMA Stats");
			printf("\nEnter 6 to print  All Stats");
			printf("\nEnter 8 to restart group");
			printf("\nEnter 9 to show group status");
			printf("\nEnter a to show links' status");
			printf("\nEnter b to show WUFE_SystemDisplay' status");
			printf("\nEnter c to handle events");
            printf("\nEnter r to reboot the system");
			printf("\nEnter x to previous menu");
			printf("\n");

			ch=getchar();
			if(ch=='6')
				stats_all='6';
		}
		else
		{
			ch= '6'-stats_all;
			stats_all--;
		}
		switch(ch)
		{
			case '1':
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
					printf(" Device statisics IMA %d       \n",ii);
					printf("-------------------------------\n");
					App_ShowTdmAtmDeviceStats(h_dev_ima[ii], tdi_atm_stats);
				}
				if(ch== '1')  
					break;
			case '2':
				printf("-------------------------------\n");
				printf("Bridge Port Statistics ENET1 \n");
				printf("-------------------------------\n");
				App_ShowBridgePortStats(h_bridge1_bport_enet1, bport_stats);
				if(ch=='2')
					break;
			case '3':
				for(ii = 0;ii < MAX_ATM_TX_CHANNEL;ii++)
				{
					printf("---------------------------------------------------\n");
					printf(" Flow statistics IMA h_flow_agg_trans_bridge_egress[%d] \n",ii);
					printf("---------------------------------------------------\n");
					App_ShowFlowAggStats(h_flow_agg_trans_bridge_egress[ii], flow_stats);
				}
				printf("----------------------------------------------------\n");
				printf(" Flow statistics IMA h_flow_agg_pwe3_atm_ingress[0] \n");
				printf("----------------------------------------------------\n");
				App_ShowPwe3FlowAggStats(h_flow_agg_pwe3_atm_ingress[0], pwe3_flow_stats);
				if(ch=='3')
					break;
				break;
			case '4':

				for(ii=0;ii<MAX_ATM_TX_CHANNEL;ii++)
				{
					printf("-------------------------------\n");
					printf(" Channel Statistics ATM Tx [%d]    \n",ii);
					printf("-------------------------------\n");
					App_AtmTxChannelStats(ima_tx_ch_handle[ii], atm_tx_channel_stats);
				}

				for(ii=0;ii<NUM_OF_GROUPS;ii++)
				{
					printf("-------------------------------\n");
					printf(" Channel Statistics ATM Rx [%d]    \n",ii);
					printf("-------------------------------\n");
					App_AtmRxChannelStats(ima_rx_ch_handle[ii], atm_rx_channel_stats);
				}

#if 1

WP_ChannelStatisticsReset (ima_rx_ch_handle[ii], &atm_rx_channel_stats);
WP_ChannelStatisticsReset (ima_tx_ch_handle[ii], &atm_tx_channel_stats);

#endif
				if(ch=='4')
					break;
				break;    

			case '5':
				App_ShowImaGroupStats();
				App_ShowImaLinkStats();
				if(ch=='5')
					break;
				break;

			case '6':        
				break;
			case 'x':
				return;
				break;
			case '8':
				printf("-------------------------------\n");
				printf ("WP_ImaGroupRestart()\n");
				printf("-------------------------------\n");
				#ifdef _WP_MODULE_IMA_AR_ENABLE_
				WP_ImaGroupRestart (ima_group_handle[0], WP_IMA_USE_EXISTING_GID, 0);
				#else 
				WP_ImaGroupRestart (ima_group_handle[0]);
				#endif
				break;
			case '9':
				printf("-------------------------------\n");
				printf("show ima group state\n");
				printf("-------------------------------\n");
				m_showGrpStatus ();
				break;
			case 'a':
				printf("-------------------------------\n");
				printf("show ima link state\n");
				printf("-------------------------------\n");
				m_showLnkStatus ();
				break;
			case 'b':
				printf("-------------------------------\n");
				printf("show WUFE_system_status\n");
				printf("-------------------------------\n");
				m_showWUFE();
				break;
			case 'c':
				printf("-------------------------------\n");
				printf("clear events\n");
				printf("-------------------------------\n");
				display_events();
				break;
            case 'r':
                WPX_Reboot ();
                break;
			default:
				printf("Invalid Entry\n");
				break;
		}
	}while(1);

}

/*****************************************************************************
 * Function name: App_ShowEnetDeviceStats
 * Description  : Display Enet device statistics 
 * Input  params: None
 * Output params: None
 * Return val   : None
 *****************************************************************************/
void App_ShowEnetDeviceStats(WP_handle enet_device_handle, WP_stats_enet s_hs_enet)
{
	memset(&s_hs_enet,0,sizeof(s_hs_enet));
	status = WP_DeviceStatistics(enet_device_handle, &s_hs_enet);
	App_TerminateOnError(status, "WP_DeviceStatistics");
	printf(" txrx_frames_64            : %d\n", (WP_U32)s_hs_enet.txrx_frames_64);
	printf(" txrx_frames_127           : %d\n", (WP_U32)s_hs_enet.txrx_frames_127);
	printf(" txrx_frames_255           : %d\n", (WP_U32)s_hs_enet.txrx_frames_255);
	printf(" txrx_frames_511           : %d\n", (WP_U32)s_hs_enet.txrx_frames_511);
	printf(" txrx_frames_1023          : %d\n", (WP_U32)s_hs_enet.txrx_frames_1023);
	printf(" txrx_frames_1518          : %d\n", (WP_U32)s_hs_enet.txrx_frames_1518);
	printf(" txrx_frames_1522          : %d\n", (WP_U32)s_hs_enet.txrx_frames_1522);
	printf(" rx_bytes                  : %d\n", (WP_U32)s_hs_enet.rx_bytes);
	printf(" rx_packets                : %d\n", (WP_U32)s_hs_enet.rx_packets);
	printf(" rx_err_fcs                : %d\n", (WP_U32)s_hs_enet.rx_err_fcs);
	printf(" rx_multicast              : %d\n", (WP_U32)s_hs_enet.rx_multicast);
	printf(" rx_broadcast              : %d\n", (WP_U32)s_hs_enet.rx_broadcast);
	printf(" rx_mac_control            : %d\n", (WP_U32)s_hs_enet.rx_mac_control);
	printf(" rx_mac_pause              : %d\n", (WP_U32)s_hs_enet.rx_mac_pause);
	printf(" rx_mac_unknown            : %d\n", (WP_U32)s_hs_enet.rx_mac_unknown);
	printf(" rx_err_alignment          : %d\n", (WP_U32)s_hs_enet.rx_err_alignment);
	printf(" rx_err_length             : %d\n", (WP_U32)s_hs_enet.rx_err_length);
	printf(" rx_err_code               : %d\n", (WP_U32)s_hs_enet.rx_err_code);
	printf(" rx_false_carrier          : %d\n", (WP_U32)s_hs_enet.rx_false_carrier);
	printf(" rx_undersize              : %d\n", (WP_U32)s_hs_enet.rx_undersize);
	printf(" rx_oversize               : %d\n", (WP_U32)s_hs_enet.rx_oversize);
	printf(" rx_fragments              : %d\n", (WP_U32)s_hs_enet.rx_fragments);
	printf(" rx_jabber                 : %d\n", (WP_U32)s_hs_enet.rx_jabber);
	printf(" rx_dropped                : %d\n\n", (WP_U32)s_hs_enet.rx_dropped);
	printf(" tx_bytes                  : %d\n", (WP_U32)s_hs_enet.tx_bytes);
	printf(" tx_packets                : %d\n", (WP_U32)s_hs_enet.tx_packets);
	printf(" tx_multicast              : %d\n", (WP_U32)s_hs_enet.tx_multicast);
	printf(" tx_broadcast              : %d\n", (WP_U32)s_hs_enet.tx_broadcast);
	printf(" tx_mac_pause              : %d\n", (WP_U32)s_hs_enet.tx_mac_pause);
	printf(" tx_defer                  : %d\n", (WP_U32)s_hs_enet.tx_defer);
	printf(" tx_excess_defer           : %d\n", (WP_U32)s_hs_enet.tx_excess_defer);
	printf(" tx_single_collision       : %d\n", (WP_U32)s_hs_enet.tx_single_collision);
	printf(" tx_multi_collision        : %d\n", (WP_U32)s_hs_enet.tx_multi_collision);
	printf(" tx_late_collision         : %d\n", (WP_U32)s_hs_enet.tx_late_collision);
	printf(" tx_excess_collision       : %d\n", (WP_U32)s_hs_enet.tx_excess_collision);
	printf(" tx_no_collision           : %d\n", (WP_U32)s_hs_enet.tx_no_collision);
	printf(" tx_mac_pause_honored      : %d\n", (WP_U32)s_hs_enet.tx_mac_pause_honored);
	printf(" tx_dropped                : %d\n", (WP_U32)s_hs_enet.tx_dropped);
	printf(" tx_jabber                 : %d\n", (WP_U32)s_hs_enet.tx_jabber);
	printf(" tx_err_fcs                : %d\n", (WP_U32)s_hs_enet.tx_err_fcs);
	printf(" tx_control                : %d\n", (WP_U32)s_hs_enet.tx_control);
	printf(" tx_oversize               : %d\n", (WP_U32)s_hs_enet.tx_oversize);
	printf(" tx_undersize              : %d\n", (WP_U32)s_hs_enet.tx_undersize);
	printf(" tx_fragments              : %d\n\n", (WP_U32)s_hs_enet.tx_fragments);
	printf(" rx_host_frames            : %d\n", (WP_U32)s_hs_enet.rx_host_frames);
	printf(" rx_iw_frames              : %d\n", (WP_U32)s_hs_enet.rx_iw_frames);
	printf(" rx_err_host_full          : %d\n", (WP_U32)s_hs_enet.rx_err_host_full);
	printf(" rx_err_fbp_underrun       : %d\n", (WP_U32)s_hs_enet.rx_err_fbp_underrun);
	printf(" rx_err_nonvalid_mac       : %d\n", (WP_U32)s_hs_enet.rx_err_nonvalid_mac);
	printf(" rx_err_mru                : %d\n", (WP_U32)s_hs_enet.rx_err_mru);
	printf(" rx_err_sdu                : %d\n", (WP_U32)s_hs_enet.rx_err_sdu);
	printf(" tx_err_underrun           : %d\n", (WP_U32)s_hs_enet.tx_err_underrun);
	printf(" rx_err_overrun            : %d\n", (WP_U32)s_hs_enet.rx_err_overrun);
	printf(" tx_frames                 : %d\n", (WP_U32)s_hs_enet.tx_frames);
	printf(" g999_rx_pause_frames      : %d\n", (WP_U32)s_hs_enet.g999_rx_pause_frames);
	printf(" g999_rx_errored_fragments : %d\n", (WP_U32)s_hs_enet.g999_rx_errored_fragments);
	printf(" g999_rx_length_error      : %d\n", (WP_U32)s_hs_enet.g999_rx_length_error);

}

/*****************************************************************************
 * Function name: App_ShowTdmDeviceStats
 * Description  : Display TDM device statistics
 * Input  params: None
 * Output params: None
 * Return val   : None
 *****************************************************************************/
void App_ShowTdmDeviceStats(WP_handle tdm_device_handle, WP_stats_tdi_hdlc tdi_hdlc_stats)
{
	memset(&tdi_hdlc_stats,0,sizeof(WP_stats_tdi_hdlc));
	status = WP_DeviceStatistics(tdm_device_handle,&tdi_hdlc_stats);
	App_TerminateOnError(status, "WP_Devicestatistics");
	printf("rx_frames                  : %d\n", (WP_U32)tdi_hdlc_stats.rx_frames);
	printf("tx_frames                  : %d\n", (WP_U32)tdi_hdlc_stats.tx_frames);
	printf("rx_err_crc                 : %d\n", (WP_U32)tdi_hdlc_stats.rx_err_crc);
	printf("rx_err_abort               : %d\n", (WP_U32)tdi_hdlc_stats.rx_err_abort);
	printf("rx_err_addr_mismatch       : %d\n", (WP_U32)tdi_hdlc_stats.rx_err_addr_mismatch);
	printf("rx_err_buffer_overrun      : %d\n", (WP_U32)tdi_hdlc_stats.rx_err_buffer_overrun);
	printf("rx_err_overrun             : %d\n", (WP_U32)tdi_hdlc_stats.rx_err_overrun);
	printf("rx_err_iw_buffer_underrun  : %d\n", (WP_U32)tdi_hdlc_stats.rx_err_iw_buffer_underrun);
	printf("rx_err_iw_mru              : %d\n", (WP_U32)tdi_hdlc_stats.rx_err_iw_mru);
	printf("tx_err_underrun            : %d\n", (WP_U32)tdi_hdlc_stats.tx_err_underrun);
	printf("rx_err_maxsdu              : %d\n", (WP_U32)tdi_hdlc_stats.rx_err_maxsdu);
	printf("rx_iw_frames               : %d\n", (WP_U32)tdi_hdlc_stats.rx_iw_frames);
	printf("rx_err_non_octet           : %d\n", (WP_U32)tdi_hdlc_stats.rx_err_non_octet);
	printf("rx_bytes                   : %d\n", (WP_U32)tdi_hdlc_stats.rx_bytes);
	printf("rx_err_bytes               : %d\n", (WP_U32)tdi_hdlc_stats.rx_err_bytes);
	printf("tx_bytes                   : %d\n", (WP_U32)tdi_hdlc_stats.tx_bytes);
	printf("rx_err_iw_l2_parse         : %d\n", (WP_U32)tdi_hdlc_stats.rx_err_iw_l2_parse);
}
/*****************************************************************************
 * Function name: App_ShowTdmAtmDeviceStats
 * Description  :  Display TDI ATM device statistics
 * Input  params: None
 * Output params: None
 * Return val   : None
 *****************************************************************************/
void App_ShowTdmAtmDeviceStats(WP_handle tdm_device_handle, WP_stats_tdi_atm tdi_atm_stats)
{
	memset(&tdi_atm_stats,0,sizeof(WP_stats_tdi_atm));
	status = WP_DeviceStatistics(tdm_device_handle,&tdi_atm_stats);
	App_TerminateOnError(status, "WP_Devicestatistics");

	printf(" rx_err_hec              : %d\n", (WP_U32)tdi_atm_stats.rx_err_hec);
	printf(" rx_err_addr_mismatch    : %d\n", (WP_U32)tdi_atm_stats.rx_err_addr_mismatch);
	printf(" rx_err_overrun          : %d\n", (WP_U32)tdi_atm_stats.rx_err_overrun);
	printf(" rx_out_of_sync          : %d\n", (WP_U32)tdi_atm_stats.rx_out_of_sync);
	printf(" rx_enter_sync           : %d\n\n", (WP_U32)tdi_atm_stats.rx_enter_sync);
	printf(" trans_rx_err_overrun    : %d\n", (WP_U32)tdi_atm_stats.trans_rx_err_overrun);
	printf(" trans_tx_err_underrun   : %d\n", (WP_U32)tdi_atm_stats.trans_tx_err_underrun);
	printf(" rx_sync_state           : %d\n\n", (WP_U32)tdi_atm_stats.rx_sync_state);
	/*ATM ext stats*/
	printf(" rx_cells                : %d\n", (WP_U32)tdi_atm_stats.ext_stats.rx_cells);
	printf(" rx_last_unknown_addr    : %d\n", (WP_U32)tdi_atm_stats.ext_stats.rx_last_unknown_addr);
	printf(" rx_clp1_cells           : %d\n", (WP_U32)tdi_atm_stats.ext_stats.rx_clp1_cells);
	printf(" rx_gfc_cells            : %d\n", (WP_U32)tdi_atm_stats.ext_stats.rx_gfc_cells);
	printf(" tx_cells                : %d\n", (WP_U32)tdi_atm_stats.ext_stats.tx_cells);
	printf(" tx_clp1_cells           : %d\n", (WP_U32)tdi_atm_stats.ext_stats.tx_clp1_cells);
	printf(" tx_efci_cells           : %d\n", (WP_U32)tdi_atm_stats.ext_stats.tx_efci_cells);
	printf(" rx_oam_cells            : %d\n", (WP_U32)tdi_atm_stats.ext_stats.rx_oam_cells);
	printf(" rx_rm_cells             : %d\n", (WP_U32)tdi_atm_stats.ext_stats.rx_rm_cells);
	printf(" tx_oam_cells            : %d\n", (WP_U32)tdi_atm_stats.ext_stats.tx_oam_cells);
	printf(" tx_rm_cells             : %d\n", (WP_U32)tdi_atm_stats.ext_stats.tx_rm_cells);
	printf(" rx_efci_cells           : %d\n", (WP_U32)tdi_atm_stats.ext_stats.rx_efci_cells);
	printf(" tx_idle_cells           : %d\n", (WP_U32)tdi_atm_stats.ext_stats.tx_idle_cells);
	printf(" rx_clp0_cells           : %d\n", (WP_U32)tdi_atm_stats.ext_stats.rx_clp0_cells);
	printf(" tx_clp0_cells           : %d\n", (WP_U32)tdi_atm_stats.ext_stats.tx_clp0_cells);
	printf(" rx_crc_err_cells        : %d\n", (WP_U32)tdi_atm_stats.ext_stats.rx_crc_err_cells);
	printf(" rx_unsupported_cells    : %d\n", (WP_U32)tdi_atm_stats.ext_stats.rx_unsupported_cells);
	printf(" aal5_rx_frame_counter   : %d\n", (WP_U32)tdi_atm_stats.ext_stats.aal5_rx_frame_counter);
	printf(" aal5_rx_bytes_counter   : %d\n", (WP_U32)tdi_atm_stats.ext_stats.aal5_rx_bytes_counter);
	printf(" 64 aal5_tx_frame_counter: %d\n", (WP_U32)tdi_atm_stats.ext_stats.aal5_tx_frame_counter);
	printf(" aal5_tx_bytes_counter   : %d\n", (WP_U32)tdi_atm_stats.ext_stats.aal5_tx_bytes_counter);

}


/*****************************************************************************
 * Function name: App_ShowBridgePortStats
 * Description  : Display bridge port statistics
 * Input  params: None
 * Output params: None
 * Return val   : None
 *****************************************************************************/
void App_ShowBridgePortStats(WP_handle bport_handle, WP_bport_stats bport_stats)
{
	memset(&bport_stats, 0, sizeof(bport_stats));
	status = WP_IwPortStatistics(bport_handle, &bport_stats);
	App_TerminateOnError(status, "WP_BportStatistics");
	printf("rx_valid_packets                       : %d\n",(WP_U32)bport_stats.rx_valid_packets);
	printf("discard_vlan_acceptable_filter         : %d\n",(WP_U32)bport_stats.discard_vlan_acceptable_filter);
	printf("discard_ingress_filter                 : %d\n",(WP_U32)bport_stats.discard_ingress_filter);
	printf("discard_bridge_classifier              : %d\n",(WP_U32)bport_stats.discard_bridge_classifier);
	printf("discard_unk_macsa                      : %d\n",(WP_U32)bport_stats.discard_unk_macsa);
	printf("deny_mac_sa                            : %d\n",(WP_U32)bport_stats.deny_mac_sa);
	printf("deny_mac_da                            : %d\n\n",(WP_U32)bport_stats.deny_mac_da);
	printf("rx_bc_valid_packets                    : %d\n",(WP_U32)bport_stats.rx_bc_valid_packets);
	printf("rx_mc_valid_packets                    : %d\n",(WP_U32)bport_stats.rx_mc_valid_packets);
	printf("forwarded_uc_packets                   : %d\n",(WP_U32)bport_stats.forwarded_uc_packets);
	printf("forwarded_bc_packets                   : %d\n",(WP_U32)bport_stats.forwarded_bc_packets);
	printf("forwarded_mc_packets                   : %d\n",(WP_U32)bport_stats.forwarded_mc_packets);
	printf("protocol_error                         : %d\n",(WP_U32)bport_stats.protocol_error);
	printf("res_mc_mac_deny                        : %d\n",(WP_U32)bport_stats.res_mc_mac_deny);
	printf("discard_dfc_filter[0]                  : %d\n",(WP_U32)bport_stats.discard_dfc_filter[0] );
	printf("deny_cfm_frames                        : %d\n\n",(WP_U32)bport_stats.deny_cfm_frames);
	printf("bad_ip_version                         : %d\n",(WP_U32)bport_stats.bad_ip_version);
	printf("checksum_error                         : %d\n",(WP_U32)bport_stats.checksum_error);
	printf("ip_protocol_error                      : %d\n",(WP_U32)bport_stats.ip_protocol_error);
	printf("discard_pce_filter[0]                  : %d\n",(WP_U32)bport_stats.discard_pce_filter[0] );
	printf("ingress_policer_non_conforming_packets : %d\n",(WP_U32)bport_stats.ingress_policer_non_conforming_packets);
	printf("rx_discard_lpm                         : %d\n",(WP_U32)bport_stats.rx_discard_lpm);
	printf("iw_mirror_fbp_underrun                 : %d\n",(WP_U32)bport_stats.iw_mirror_fbp_underrun);
	printf("iw_mirror_iwbg_fbp_underrun            : %d\n",(WP_U32)bport_stats.iw_mirror_iwbg_fbp_underrun);
}
/*****************************************************************************
 * Function name: App_ShowFlowAggStats
 * Description  : Display flow aggregation statistics
 * Input  params: None
 * Output params: None
 * Return val   : None
 *****************************************************************************/
void App_ShowFlowAggStats(WP_handle flow_agg_handle, WP_iw_flow_stats flow_stats)
{
	memset(&flow_stats,0,sizeof(flow_stats));
	status = WP_IwFlowStatistics(flow_agg_handle,WP_IW_FLOW_STAT,&flow_stats);
	App_TerminateOnError(status, "WP_IwFlowStatistics");

	printf("Forward Packet            : %d\n",(WP_U32)flow_stats.forward_packet);
	printf("FBP Drop Packets          : %d\n",(WP_U32)flow_stats.fbp_drop_packets);
	printf("MTU Drop Packets          : %d\n",(WP_U32)flow_stats.mtu_drop_packets);
	printf("TTL Drop Packets          : %d\n",(WP_U32)flow_stats.ttl_drop_packets);
	printf("TX Queue Drop Packets     : %d\n",(WP_U32)flow_stats.tx_queue_drop_packets);
	printf("MPLS Drop                 : %d\n",(WP_U32)flow_stats.mpls_drop);
	printf("Denied Packets            : %d\n",(WP_U32)flow_stats.denied_packets);
	printf("Group Filtered Packets    : %d\n",(WP_U32)flow_stats.group_filtered_packets);
	printf("Forwarded Bytes           : %d\n",(WP_U32)flow_stats.forwarded_bytes);
	printf("Gtp Bad Headers           : %d\n",(WP_U32)flow_stats.gtp_bad_headers);
	printf("Bad Policer Packets       : %d\n",(WP_U32)flow_stats.policer_non_conforming_packets);
	printf("cfm_mac_in_mac_drop       : %d\n",(WP_U32)flow_stats.cfm_mac_in_mac_drop);
	printf("out_of_sequence_packets   : %d\n",(WP_U32)flow_stats.out_of_sequence_packets);
	printf("pwe_switchover_events     : %d\n",(WP_U32)flow_stats.pwe_switchover_events);
	printf("non_zero_seq_num          : %d\n",(WP_U32)flow_stats.non_zero_seq_num);
}
/*****************************************************************************
 * Function name: App_ShowPwe3FlowAggStats
 * Description  :  Display PWE3 flow aggregation stats
 * Input  params: None
 * Output params: None
 * Return val   : None
 *****************************************************************************/
void App_ShowPwe3FlowAggStats(WP_handle flow_agg_handle, WP_iw_pwe3_stats flow_stats)
{
	memset(&flow_stats,0,sizeof(WP_iw_pwe3_stats));
	status = WP_IwFlowStatistics(flow_agg_handle,WP_IW_FLOW_STAT,&flow_stats);
	App_TerminateOnError(status, "WP_IwFlowStatistics");

	printf("forwarded_packets               : %d\n",(WP_U32)flow_stats.forwarded_packets);
	printf("forwarded_bytes                 : %d\n",(WP_U32)flow_stats.forwarded_bytes);
	printf("fbp_drop_packets                : %d\n",(WP_U32)flow_stats.fbp_drop_packets);
	printf("mtu_drop_packets                : %d\n",(WP_U32)flow_stats.mtu_drop_packets);
	printf("ttl_drop_packets                : %d\n\n",(WP_U32)flow_stats.ttl_drop_packets);
	printf("tx_queue_drop_packets           : %d\n",(WP_U32)flow_stats.tx_queue_drop_packets);
	printf("mpls_drop_packets               : %d\n",(WP_U32)flow_stats.mpls_drop_packets);
	printf("denied_packets                  : %d\n",(WP_U32)flow_stats.denied_packets);
	printf("out_of_seq_packets              : %d\n",(WP_U32)flow_stats.out_of_seq_packets);
	printf("non_zero_sn_packets             : %d\n",(WP_U32)flow_stats.non_zero_sn_packets);
	printf("policer_non_conforming_packets  : %d\n",(WP_U32)flow_stats.policer_non_conforming_packets);
	printf("switchover_events               : %d\n",(WP_U32)flow_stats.switchover_events);
	printf("hash_fail_cells                 : %d\n",(WP_U32)flow_stats.hash_fail_cells);
}
/*****************************************************************************
 * Function name: App_AtmTxChannelStats
 * Description  : Display ATM Tx channel statistics
 * Input  params: None
 * Output params: None
 * Return val   : None
 *****************************************************************************/
void App_AtmTxChannelStats(WP_handle atm_tx_ch_handle, WP_stats_aal5_tx atm_tx_channel_stats)
{
	memset(&atm_tx_channel_stats,0,sizeof(WP_stats_aal5_tx));
	status = WP_ChannelStatistics(atm_tx_ch_handle,&atm_tx_channel_stats);
	App_TerminateOnError(status, "WP_ChannelStatistics");

	printf("tx_frames           : %d\n",(WP_U32)atm_tx_channel_stats.tx_frames);
	printf("tx_clp0_cells       : %d\n",(WP_U32)atm_tx_channel_stats.tx_clp0_cells);
	printf("tx_clp1_cells       : %d\n",(WP_U32)atm_tx_channel_stats.tx_clp1_cells);
	printf("tx_bytes            : %d\n",(WP_U32)atm_tx_channel_stats.tx_bytes);
	printf("tx_clp1_frames      : %d\n\n",(WP_U32)atm_tx_channel_stats.tx_clp1_frames);
	printf("tx_oam_cc_cells     : %d\n",(WP_U32)atm_tx_channel_stats.tx_oam_cc_cells);
	printf("tx_oam_ais_cells    : %d\n",(WP_U32)atm_tx_channel_stats.tx_oam_ais_cells);
	printf("tx_oam_rdi_cells    : %d\n",(WP_U32)atm_tx_channel_stats.tx_oam_rdi_cells);
}

/*****************************************************************************
 * Function name: App_AtmRxChannelStats
 * Description  :  added for future use
 * Input  params: None
 * Output params: None
 * Return val   : None
 *****************************************************************************/
void App_AtmRxChannelStats(WP_handle atm_rx_ch_handle, WP_stats_aal5_rx atm_rx_channel_stats)
{
	memset(&atm_rx_channel_stats,0,sizeof(WP_stats_aal5_tx));
	status = WP_ChannelStatistics(atm_rx_ch_handle,&atm_rx_channel_stats);
	App_TerminateOnError(status, "WP_ChannelStatistics");

	printf(" rx_frames         : %d\n",(WP_U32)atm_rx_channel_stats.rx_frames);
	printf(" aborted_frames    : %d\n",(WP_U32)atm_rx_channel_stats.aborted_frames);
	printf(" err_crc           : %d\n",(WP_U32)atm_rx_channel_stats.err_crc);
	printf(" err_size          : %d\n",(WP_U32)atm_rx_channel_stats.err_size);
	printf(" err_underflow     : %d\n",(WP_U32)atm_rx_channel_stats.err_underflow);
	printf(" err_maxsdu        : %d\n",(WP_U32)atm_rx_channel_stats.err_maxsdu);
	printf(" err_iw_underrun   : %d\n",(WP_U32)atm_rx_channel_stats.err_iw_underrun);
	printf(" err_iw_maxmru     : %d\n",(WP_U32)atm_rx_channel_stats.err_iw_maxmru);
	printf(" err_cpi           : %d\n",(WP_U32)atm_rx_channel_stats.err_cpi);
	printf(" clp_frames        : %d\n",(WP_U32)atm_rx_channel_stats.clp_frames);
	printf(" l3_iw_frames      : %d\n",(WP_U32)atm_rx_channel_stats.l3_iw_frames);
	printf(" err_ras_timeout   : %d\n",(WP_U32)atm_rx_channel_stats.err_ras_timeout);
	printf(" rx_clp0_cells     : %d\n",(WP_U32)atm_rx_channel_stats.rx_clp0_cells);
	printf(" rx_bytes          : %d\n",(WP_U32)atm_rx_channel_stats.rx_bytes);
	printf(" rx_clp1_cells     : %d\n",(WP_U32)atm_rx_channel_stats.rx_clp1_cells);

}

char imass[64];
char * ima_s (int n)
{
	memset (imass, 0, 64);
	switch (n) 
	{
	case WP_IMA_GSM_START_UP:
		sprintf (imass, "%s", "WP_IMA_GSM_START_UP");
		break;
	case WP_IMA_GSM_START_UP_ACK:
		sprintf (imass, "%s", "WP_IMA_GSM_START_UP_ACK");
		break;
	case WP_IMA_GSM_CONFIG_ABORTED:
		sprintf (imass, "%s", "WP_IMA_GSM_CONFIG_ABORTED");
		break;
	case WP_IMA_GSM_INSUFFICIENT:
		sprintf (imass, "%s", "WP_IMA_GSM_INSUFFICIENT");
		break;
	case WP_IMA_GSM_BLOCKED:
		sprintf (imass, "%s", "WP_IMA_GSM_BLOCKED");
		break;
	case WP_IMA_GSM_OPERATIONAL:
		sprintf (imass, "%s", "WP_IMA_GSM_OPERATIONAL");
		break;
	case WP_IMA_GSM_NOT_CONFIGURED:
		sprintf (imass, "%s", "WP_IMA_GSM_NOT_CONFIGURED");
		break;
	case WP_IMA_GSM_NUM_STATES:
		sprintf (imass, "%s", "WP_IMA_GSM_NUM_STATES");
		break;
	default:
		sprintf (imass, "%s", "WP_IMA_GSM_UNKOWN_STATE");
		break;
	
	}
	return imass;
}







char * s_name_fe[] = {
"WP_IMA_FE_GSM_START_UP              0",
"WP_IMA_FE_GSM_START_UP_ACK          1",
"WP_IMA_FE_GSM_CONFIG_ABORTED_M      2",
"WP_IMA_FE_GSM_CONFIG_ABORTED_SYM    3",
"WP_IMA_FE_GSM_CONFIG_ABORTED_VER    4",
"WP_IMA_FE_GSM_CONFIG_ABORTED_RES1   5",
"WP_IMA_FE_GSM_CONFIG_ABORTED_RES2   6",
"WP_IMA_FE_GSM_CONFIG_ABORTED_OTH    7",
"WP_IMA_FE_GSM_INSUFFICIENT          8",
"WP_IMA_FE_GSM_BLOCKED               9",
"WP_IMA_FE_GSM_OPERATIONAL          10",
"WP_IMA_FE_GSM_RESERVED_1           11",
"WP_IMA_FE_GSM_RESERVED_2           12",
"WP_IMA_FE_GSM_RESERVED_3           13",
"WP_IMA_FE_GSM_RESERVED_4           14",
"WP_IMA_FE_GSM_RESERVED_5           15",
"WP_IMA_NO_ICP_RECEIVED             16",
""
};

char * ima_s_fe (int n)
{
    if (16 < n)
    {
        printf ("fe_status error\n");
        return NULL;
    }
   
    if (n != 10) printf ("fe status(%d)\n", n);

    return s_name_fe[n % 17];
}



































/*****************************************************************************
 * Function name: App_ShowImaGroupStats
 * Description  : Display IMA group statistics
 * Input  params: None
 * Output params: None
 * Return val   : None
 *****************************************************************************/
void App_ShowImaGroupStats(void)
{
	WP_U32 ii;
	WP_status status;
	WP_ima_group_statistics ima_group_statistics;

	memset(&ima_group_statistics, 0, sizeof(WP_ima_group_statistics));
	for(ii=0;ii<NUM_OF_GROUPS;ii++)
	{ 
		status = WP_ImaGroupStatistics(ima_group_handle[ii] , &ima_group_statistics);
		App_TerminateOnError(status, "WP_ImaGroupStatistics");
		printf("-------------------------------\n");
		printf(" IMA Group[%d] statisics       \n",ii);
		printf("-------------------------------\n");
		printf("failures_ne                      : %d\n", (WP_U32)ima_group_statistics.failures_ne);
		printf("failures_fe                      : %d\n", (WP_U32)ima_group_statistics.failures_fe);
		printf("rx_err_addr_mismatch             : %d\n", (WP_U32)ima_group_statistics.rx_err_addr_mismatch);
		printf("running_seconds                  : %d\n", (WP_U32)ima_group_statistics.running_seconds);
		printf("unavailable_seconds              : %d\n\n", (WP_U32)ima_group_statistics.unavailable_seconds);
		/* Extended IMA group statistics */
		printf("\nEXTENDED GROUP STATISTICS\n");
		printf("-------------------------------\n");
		printf("ext_stats.rx_cells               : %d\n", (WP_U32)ima_group_statistics.ext_stats.rx_cells);
		printf("ext_stats.rx_last_unknown_addr   : %d\n", (WP_U32)ima_group_statistics.ext_stats.rx_last_unknown_addr);
		printf("ext_stats.rx_clp1_cells          : %d\n", (WP_U32)ima_group_statistics.ext_stats.rx_clp1_cells);
		printf("ext_stats.rx_gfc_cells           : %d\n", (WP_U32)ima_group_statistics.ext_stats.rx_gfc_cells);
		printf("ext_stats.tx_cells               : %d\n\n", (WP_U32)ima_group_statistics.ext_stats.tx_cells);
		printf("ext_stats.tx_clp1_cells          : %d\n", (WP_U32)ima_group_statistics.ext_stats.tx_clp1_cells);
		printf("ext_stats.tx_efci_cells          : %d\n", (WP_U32)ima_group_statistics.ext_stats.tx_efci_cells);
		printf("ext_stats.rx_oam_cells           : %d\n", (WP_U32)ima_group_statistics.ext_stats.rx_oam_cells);
		printf("ext_stats.rx_rm_cells            : %d\n", (WP_U32)ima_group_statistics.ext_stats.rx_rm_cells);
		printf("ext_stats.tx_oam_cells           : %d\n\n", (WP_U32)ima_group_statistics.ext_stats.tx_oam_cells);
		printf("ext_stats.tx_rm_cells            : %d\n", (WP_U32)ima_group_statistics.ext_stats.tx_rm_cells);
		printf("ext_stats.rx_efci_cells          : %d\n", (WP_U32)ima_group_statistics.ext_stats.rx_efci_cells);
		printf("ext_stats.tx_idle_cells          : %d\n", (WP_U32)ima_group_statistics.ext_stats.tx_idle_cells);
		printf("ext_stats.rx_clp0_cells          : %d\n", (WP_U32)ima_group_statistics.ext_stats.rx_clp0_cells);
		printf("ext_stats.tx_clp0_cells          : %d\n\n", (WP_U32)ima_group_statistics.ext_stats.tx_clp0_cells);
		printf("ext_stats.rx_crc_err_cells       : %d\n", (WP_U32)ima_group_statistics.ext_stats.rx_crc_err_cells);
		printf("ext_stats.rx_unsupported_cells   : %d\n", (WP_U32)ima_group_statistics.ext_stats.rx_unsupported_cells);
		printf("ext_stats.aal5_rx_frame_counter  : %d\n", (WP_U32)ima_group_statistics.ext_stats.aal5_rx_frame_counter);
		printf("ext_stats.aal5_rx_bytes_counter  : %d\n", (WP_U32)ima_group_statistics.ext_stats.aal5_rx_bytes_counter);
		printf("ext_stats.aal5_tx_frame_counter  : %d\n", (WP_U32)ima_group_statistics.ext_stats.aal5_tx_frame_counter);
		printf("ext_stats.aal5_tx_bytes_counter  : %d\n", (WP_U32)ima_group_statistics.ext_stats.aal5_tx_bytes_counter);

	}

}

char linkss[64];
char *link_s(int n)
{
	switch (n)
	{
		case WP_IMA_LSM_NOT_IN_GROUP:
			sprintf (linkss, "%s", "NOT_IN_GROUP");
			break;
		case WP_IMA_LSM_UNUSABLE:
			sprintf (linkss, "%s", "UNUSABLE");
			break;
		case WP_IMA_LSM_FAULT:
			sprintf (linkss, "%s", "FAULT");
			break;
		case WP_IMA_LSM_MIS_CONNECTED:
			sprintf (linkss, "%s", "MIS_CONNECTED");
			break;
		case WP_IMA_LSM_INHIBITED:
			sprintf (linkss, "%s", "INHIBITED");
			break;
		case WP_IMA_LSM_FAILED:
			sprintf (linkss, "%s", "FAILED");
			break;
		case WP_IMA_LSM_USABLE:
			sprintf (linkss, "%s", "USABLE");
			break;
		case WP_IMA_LSM_ACTIVE:
			sprintf (linkss, "%s", "ACTIVE");
			break;
		case WP_IMA_LSM_UNKNOWN:
			sprintf (linkss, "%s", "UNKNOWN");
			break;
		case WP_IMA_LSM_NUM_STATES:
			sprintf (linkss, "%s", "NUM_STATES");
			break;
		default:
			sprintf (linkss, "%s", "XXXXXXXXXXXXXXXXXX");
			break;
			

	}
	return linkss;
}


WP_CHAR *lline_state[] = {
   "No Error",
   "ICP Errors",
   "Not Delay Synchronized",
   "Looped Back"
};




/*****************************************************************************
 * Function name: App_ShowImaLinkStats
 * Description  : Disploay IMA link statistics
 * Input  params: None
 * Output params: None
 * Return val   : None
 *****************************************************************************/
void App_ShowImaLinkStats(void)
{
	WP_U32 ii;
	WP_status status;
	WP_ima_link_statistics ima_link_statistics;

	memset(&ima_link_statistics, 0, sizeof(WP_ima_link_statistics));
	for(ii=0;ii<NUM_OF_LINKS;ii++)
	{
		status = WP_ImaLinkStatistics(ima_link_handle[ii] , &ima_link_statistics);
		App_TerminateOnError(status, "WP_ImaLinkStatistics");
		printf("-------------------------------\n");
		printf(" IMA Link[%d] statisics        \n",ii);
		printf("-------------------------------\n");
		printf("rx_icp_cells            : %d\n", ima_link_statistics.rx_icp_cells);
		printf("rx_err_icp_cells        : %d\n", ima_link_statistics.rx_err_icp_cells);
		printf("rx_lods                 : %d\n", ima_link_statistics.rx_lods);
		printf("rx_icp_violations       : %d\n\n", ima_link_statistics.rx_icp_violations);
		printf("rx_stuff                : %d\n", ima_link_statistics.rx_stuff);
		printf("rx_ne_ses               : %d\n", ima_link_statistics.rx_ne_ses);
		printf("rx_ne_uas               : %d\n", ima_link_statistics.rx_ne_uas);
		printf("rx_ne_uus               : %d\n", ima_link_statistics.rx_ne_uus);
		printf("rx_ne_failure           : %d\n\n", ima_link_statistics.rx_ne_failure);
		printf("rx_fe_defects           : %d\n", ima_link_statistics.rx_fe_defects);
		printf("rx_fe_failure           : %d\n", ima_link_statistics.rx_fe_failure);
		printf("rx_fe_uus               : %d\n", ima_link_statistics.rx_fe_uus);
		printf("rx_fe_ses               : %d\n", ima_link_statistics.rx_fe_ses);
		printf("rx_fe_uas               : %d\n\n", ima_link_statistics.rx_fe_uas);
		printf("tx_icp_cells            : %d\n", ima_link_statistics.tx_icp_cells);
		printf("tx_stuff                : %d\n", ima_link_statistics.tx_stuff);
		printf("tx_ne_uus               : %d\n", ima_link_statistics.tx_ne_uus);
		printf("tx_fe_uus               : %d\n", ima_link_statistics.tx_fe_uus);
		printf("tx_fe_failure           : %d\n", ima_link_statistics.tx_fe_failure);
		printf("tx_ne_failure           : %d\n\n", ima_link_statistics.tx_ne_failure);
		printf("tx_icp_cells-rx_icp_cells[%d][%%%d]\n", 
			ima_link_statistics.tx_icp_cells - ima_link_statistics.rx_icp_cells,
			100 * (ima_link_statistics.tx_icp_cells - ima_link_statistics.rx_icp_cells) / ima_link_statistics.tx_icp_cells 
			);

	} 
}

/*****************************************************************************
 * Function name: App_ShowImaGroupStats
 * Description  : Display IMA group statistics
 * Input  params: None
 * Output params: None
 * Return val   : None
 *****************************************************************************/
void App_CheckStats(WP_status *test_result)
{

   WP_status status;
   WP_stats_enet s_hs_enet={0};
   WP_stats_tdi_atm tdi_atm_stats={0};
   WP_bport_stats bport_stats = {0};
   WP_iw_flow_stats flow_stats = {0};
   WP_iw_pwe3_stats pwe3_flow_stats = {0};
   WP_stats_aal5_tx atm_tx_channel_stats = {0};
   WP_stats_aal5_rx atm_rx_channel_stats = {0};
   WP_ima_group_statistics ima_group_statistics = {0};
   WP_ima_link_statistics ima_link_statistics = {0};
   WP_U32 stats_sum1 = 0, stats_sum2 = 0;
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

   if (s_hs_enet.txrx_frames_127 != APP_MAX_PKTS_NUM * 2){
   	*test_result = (-1);
       printf("ENET2 txrx_frames_64 check fail!\n");
   }
   
   if (s_hs_enet.rx_packets != APP_MAX_PKTS_NUM){
   	*test_result = (-1);
       printf("ENET2 rx_packets check fail!\n");
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
   
   /*Check IMA device stats*/
   stats_sum1 = 0;
   
   for(ii = 0;ii < APP_MAX_TDM_PORTS;ii++)
   {
      memset(&tdi_atm_stats,0,sizeof(WP_stats_tdi_atm));
      status = WP_DeviceStatistics(h_dev_ima[ii],&tdi_atm_stats);
      App_TerminateOnError(status, "WP_Devicestatistics");

      stats_sum1 += tdi_atm_stats.ext_stats.aal5_rx_frame_counter;

       /*Check tdm errors*/
	if (tdi_atm_stats.rx_err_hec || tdi_atm_stats.rx_err_addr_mismatch || 
	    tdi_atm_stats.rx_err_overrun || tdi_atm_stats.trans_rx_err_overrun || 
	    tdi_atm_stats.trans_tx_err_underrun || tdi_atm_stats.ext_stats.rx_crc_err_cells || 
	    tdi_atm_stats.ext_stats.rx_unsupported_cells
	    )
	{
	  *test_result = (-1);
	  printf("TDM device %d error check fail!\n", ii);
	}
	  
   }

   if (stats_sum1 == 0)
   {
      *test_result = (-1);
      printf("IMA device aal5_rx_frame_counter check fail!\n");
   }

   /*Check enet1 bport stats*/
   memset(&bport_stats, 0, sizeof(bport_stats));
   status = WP_IwPortStatistics(h_bridge1_bport_enet1, &bport_stats);
   App_TerminateOnError(status, "WP_BportStatistics");
   
   if (bport_stats.rx_valid_packets != APP_MAX_PKTS_NUM){
   	*test_result = (-1);
	printf("ENET1 bport rx_valid_packets check fail!\n");
   }

   if (bport_stats.protocol_error || bport_stats.bad_ip_version || 
   	bport_stats.checksum_error || bport_stats.ip_protocol_error)
   {
   	*test_result = (-1);
	printf("ENET1 bport error check fail!\n");
   }
   
   /*Check IMA trans bridge egress flow agg*/
   memset(&flow_stats,0,sizeof(flow_stats));
   status = WP_IwFlowStatistics(h_flow_agg_trans_bridge_egress[0],WP_IW_FLOW_STAT,&flow_stats);
   App_TerminateOnError(status, "WP_IwFlowStatistics");

   if (flow_stats.forward_packet != APP_MAX_PKTS_NUM){
      *test_result = (-1);
      printf("IMA trans bridge egress 0 forward_packet check fail(%d)!\n", ii);
   }

   if (flow_stats.fbp_drop_packets || flow_stats.mtu_drop_packets ||
   	flow_stats.ttl_drop_packets || flow_stats.tx_queue_drop_packets ||
   	flow_stats.mpls_drop || flow_stats.denied_packets || 
   	flow_stats.gtp_bad_headers || flow_stats.cfm_mac_in_mac_drop ||
   	flow_stats.out_of_sequence_packets)
   {
   	*test_result = (-1);
	printf("IMA trans bridge egress flow agg 0 error check fail!\n");
   }

   memset(&flow_stats,0,sizeof(flow_stats));
   status = WP_IwFlowStatistics(h_flow_agg_trans_bridge_egress[1],WP_IW_FLOW_STAT,&flow_stats);
   App_TerminateOnError(status, "WP_IwFlowStatistics");

   if (flow_stats.forward_packet != 0){
      *test_result = (-1);
      printf("IMA trans bridge egress 1 forward_packet check fail(%d)!\n", ii);
   }

   if (flow_stats.fbp_drop_packets || flow_stats.mtu_drop_packets ||
   	flow_stats.ttl_drop_packets || flow_stats.tx_queue_drop_packets ||
   	flow_stats.mpls_drop || flow_stats.denied_packets || 
   	flow_stats.gtp_bad_headers || flow_stats.cfm_mac_in_mac_drop ||
   	flow_stats.out_of_sequence_packets)
   {
   	*test_result = (-1);
	printf("IMA trans bridge egress flow agg 1 error check fail!\n");
   }
   
   /*Check IMA PWE3 ingress flow agg*/
   memset(&pwe3_flow_stats,0,sizeof(WP_iw_pwe3_stats));
   status = WP_IwFlowStatistics(h_flow_agg_pwe3_atm_ingress[0],WP_IW_FLOW_STAT,&pwe3_flow_stats);
   App_TerminateOnError(status, "WP_IwFlowStatistics");

   if (pwe3_flow_stats.forwarded_packets != APP_MAX_PKTS_NUM){
      *test_result = (-1);
      printf("IMA PWE3 ingress flow agg forwarded_packets check fail(%d)!\n", ii);
   }

   if (pwe3_flow_stats.fbp_drop_packets || pwe3_flow_stats.mtu_drop_packets ||
   	pwe3_flow_stats.ttl_drop_packets || pwe3_flow_stats.tx_queue_drop_packets || 
   	pwe3_flow_stats.mpls_drop_packets || pwe3_flow_stats.denied_packets || 
   	pwe3_flow_stats.hash_fail_cells)
   {
   	*test_result = (-1);
	printf("IMA PWE3 ingress flow agg error check fail!\n");
   }

   stats_sum1 = 0;
   stats_sum2 = 0;
   /*Check ATM TX Channel stats*/
   memset(&atm_tx_channel_stats,0,sizeof(WP_stats_aal5_tx));
   status = WP_ChannelStatistics(ima_tx_ch_handle[0],&atm_tx_channel_stats);
   App_TerminateOnError(status, "WP_ChannelStatistics");

   stats_sum1 += atm_tx_channel_stats.tx_frames;
   stats_sum2 += atm_tx_channel_stats.tx_clp0_cells;
   
   memset(&atm_tx_channel_stats,0,sizeof(WP_stats_aal5_tx));
   status = WP_ChannelStatistics(ima_tx_ch_handle[1],&atm_tx_channel_stats);
   App_TerminateOnError(status, "WP_ChannelStatistics");

   stats_sum1 += atm_tx_channel_stats.tx_frames;
   stats_sum2 += atm_tx_channel_stats.tx_clp0_cells;
   
   if (stats_sum1 == 0){
     *test_result = (-1);
      printf("ATM TX Channel tx_frames check fail!\n");
   }
   if (stats_sum2 == 0){
      *test_result = (-1);
      printf("ATM TX Channel tx_clp0_cells check fail!\n");
   }
   
   /*Check ATM RX Channel stats*/
   memset(&atm_rx_channel_stats,0,sizeof(WP_stats_aal5_rx));
   status = WP_ChannelStatistics(ima_rx_ch_handle[0],&atm_rx_channel_stats);
   App_TerminateOnError(status, "WP_ChannelStatistics");

   if (atm_rx_channel_stats.l3_iw_frames != APP_MAX_PKTS_NUM){
      *test_result = (-1);
      printf("ATM RX Channel l3_iw_frames check fail!\n");
   }
   
   if (atm_rx_channel_stats.rx_clp0_cells == 0){
      *test_result = (-1);
      printf("ATM RX Channel rx_clp0_cells check fail!\n");
   }

   if (atm_rx_channel_stats.err_crc || atm_rx_channel_stats.err_size || 
   	atm_rx_channel_stats.err_underflow || atm_rx_channel_stats.err_maxsdu ||
   	atm_rx_channel_stats.err_iw_underrun || atm_rx_channel_stats.err_iw_maxmru || 
   	atm_rx_channel_stats.err_cpi || atm_rx_channel_stats.err_ras_timeout
   	)
   {
   	*test_result = (-1);
	printf("ATM RX Channel error check fail!\n");
   }

   /*Check IMA Group Stats*/
   memset(&ima_group_statistics, 0, sizeof(WP_ima_group_statistics));
   status = WP_ImaGroupStatistics(ima_group_handle[0] , &ima_group_statistics);
   App_TerminateOnError(status, "WP_ImaGroupStatistics");
   
/*   if (ima_group_statistics.ext_stats.rx_cells == 0){
      *test_result = (-1);
      printf("IMA group 0 rx_cells check fail!\n");
   }
   if (ima_group_statistics.ext_stats.tx_cells == 0){
      *test_result = (-1);
      printf("IMA group 0 tx_cells check fail!\n");
   }
   if (ima_group_statistics.ext_stats.rx_clp0_cells == 0){
      *test_result = (-1);
      printf("IMA group 0 rx_clp0_cells check fail!\n");
   }
   if (ima_group_statistics.ext_stats.tx_clp0_cells == 0){
      *test_result = (-1);
      printf("IMA group 0 tx_clp0_cells check fail!\n");
   }
*/
   if (ima_group_statistics.ext_stats.rx_crc_err_cells || ima_group_statistics.ext_stats.rx_unsupported_cells
   	)
   {
   	*test_result = (-1);
	printf("IMA group 0 error check fail!\n");
   }
   
   /*Check IMA Link Stats*/
   for (ii = 0; ii < NUM_OF_LINKS; ii ++)
   {
      memset(&ima_link_statistics, 0, sizeof(WP_ima_link_statistics));
      status = WP_ImaLinkStatistics(ima_link_handle[ii] , &ima_link_statistics);
      App_TerminateOnError(status, "WP_ImaLinkStatistics");

      if (ima_link_statistics.rx_icp_cells == 0){
           *test_result = (-1);
           printf("IMA group %d rx_icp_cells check fail!\n", ii);
      	}
      if (ima_link_statistics.rx_stuff == 0){
           *test_result = (-1);
           printf("IMA group %d rx_stuff check fail!\n", ii);
      	}
      if (ima_link_statistics.tx_icp_cells == 0){
           *test_result = (-1);
           printf("IMA group %d tx_icp_cells check fail!\n", ii);
      	}
      if (ima_link_statistics.tx_stuff == 0){
           *test_result = (-1);
           printf("IMA group %d tx_stuff check fail!\n", ii);
      	}
/*
      if (ima_link_statistics.rx_err_icp_cells || ima_link_statistics.rx_ne_failure || 
	   ima_link_statistics.rx_fe_defects || ima_link_statistics.rx_fe_failure ||
	   ima_link_statistics.tx_fe_failure || ima_link_statistics.tx_ne_failure)
      {
          *test_result = (-1);
          printf("IMA Link %d error check fail!\n", ii);
      }
*/
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
      for(ii = 0;ii < APP_MAX_TDM_PORTS;ii++)
      {
          printf("-------------------------------\n");
          printf(" Device statisics IMA %d       \n",ii);
          printf("-------------------------------\n");
          App_ShowTdmAtmDeviceStats(h_dev_ima[ii], tdi_atm_stats);
      }
      printf("-------------------------------\n");
      printf("Bridge Port Statistics ENET1 \n");
      printf("-------------------------------\n");
      App_ShowBridgePortStats(h_bridge1_bport_enet1, bport_stats);
      for(ii = 0;ii < MAX_ATM_TX_CHANNEL;ii++)
      {
         printf("---------------------------------------------------\n");
         printf(" Flow statistics IMA h_flow_agg_trans_bridge_egress[%d] \n",ii);
         printf("---------------------------------------------------\n");
         App_ShowFlowAggStats(h_flow_agg_trans_bridge_egress[ii], flow_stats);
      }
      printf("----------------------------------------------------\n");
      printf(" Flow statistics IMA h_flow_agg_pwe3_atm_ingress[0] \n");
      printf("----------------------------------------------------\n");
      App_ShowPwe3FlowAggStats(h_flow_agg_pwe3_atm_ingress[0], pwe3_flow_stats);

      for(ii=0;ii<MAX_ATM_TX_CHANNEL;ii++)
      {
         printf("-------------------------------\n");
         printf(" Channel Statistics ATM Tx [%d]    \n",ii);
         printf("-------------------------------\n");
         App_AtmTxChannelStats(ima_tx_ch_handle[ii], atm_tx_channel_stats);
      }
            
      for(ii=0;ii<NUM_OF_GROUPS;ii++)
      {
         printf("-------------------------------\n");
         printf(" Channel Statistics ATM Rx [%d]    \n",ii);
         printf("-------------------------------\n");
         App_AtmRxChannelStats(ima_rx_ch_handle[ii], atm_rx_channel_stats);
      }
      
      App_ShowImaGroupStats();
      App_ShowImaLinkStats();
   }

}

