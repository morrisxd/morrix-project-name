#include "spp_npu_link.h"

WP_status txfr_stat_get(WP_U32 Channelno, void *p);
WP_status rxfr_stat_get(WP_U32 Channelno, void *p);
WP_status upi_stat_get_by_trunk(WP_U32 trunkid, STRU_UPI_STATICS *p);
WP_status upi_stm1_stat_get(WP_U32 stm1id,STRU_UPI_STATICS *p);
WP_status enet_stat_get(WP_U32 ethno, void *p);
void iw_port_stat(WP_handle iw_port);
void StatIwFlowAggregation(WP_handle flow_handle);
void printHsEnetStats(WP_handle);

