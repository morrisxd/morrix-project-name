#ifndef _WT_STATISTICS_H__
#define _WT_STATISTICS_H__

void WPE_PrintIWPortStats(WP_handle iw_port_handle, WP_route_port_stats * iwport_stats);
void WPE_GetPortStatistics(WP_handle iw_port_handle, WP_route_port_stats * iwport_stats);
void WPE_PrintFlowStats(WP_handle agg, WP_iw_flow_stats* iw_flow_stats);
void WPE_GetFlowStatistics(WP_handle agg, WP_iw_flow_stats* iw_flow_stats);
void WPE_PrintStatHsEnet(WP_handle hs_device, WP_stats_enet* enet_stats);
void WPE_GetDeviceStatistics(WP_handle hs_device, WP_stats_enet* enet_stats);

#endif
