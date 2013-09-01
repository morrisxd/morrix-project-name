/*****************************************************************************
 * (C) Copyright PMC - WIN division (Wintegra) 2011.  All rights reserved.
 * PMC-Sierra (WINTEGRA) CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of PMC-Sierra (Wintegra).
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/
WP_U64 WT_IwFlowStatisticsTdm2PsnCheck(WP_stats_iw_cesop_tdm2psn * cesop_stats_tdm2psn,WP_U32 pw_index, WP_U32 global_display);
WP_U64  WT_IwFlowStatisticsPsn2TdmCheck( WP_stats_iw_cesop_psn2tdm *cesop_stats,WP_U32 pw_index, WP_U32 global_display);
WP_U64 WT_IwFlowStatisticsCheck(WP_iw_flow_stats *flow_stats,WP_U32 pw_index);


#if WTI_CESOP_TDI
WP_U64  WT_TdiChannelTransTxCheck(WP_stats_tdi_trans_pwe3_tx  *tx_stats,WP_U32 pw_index);
WP_U64  WT_TdiChannelTransRxCheck(WP_stats_tdi_trans_pwe3_rx *rx_stats,WP_U32 pw_index);

#else
WP_U64  WT_UfeChannelTransTxCheck(WP_stats_emphy_trans_pwe3_tx  *tx_stats,WP_U32 pw_index);
WP_U64  WT_UfeChannelTransRxCheck(WP_stats_emphy_trans_pwe3_rx *rx_stats,WP_U32 pw_index);

#endif

extern void CLI_F_Psn2TdmFlowAggStatisticsClear(char *StrPrm);
WP_U32 CLI_F_Pwe3StatisticsClear(char *StrPrm);
WP_U32 CLI_F_Pwe3StatisticsCheck(char *StrPrm);
WP_U32 CLI_F_GlobalStatisticsCheck(char *StrPrm);
WP_U32 CLI_F_GlobalClockRecoveryShowState(char *StrPrm);
void WT_PrintTestInfo(WP_U32 status);

/*
 * Defenition is marked ou also
 */
#if 0
WP_U32 CLI_F_ClockRecoveryConvergenceState(char *StrPrm);
#endif

