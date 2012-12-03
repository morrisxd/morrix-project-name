/******************************************
Add functions in debug folders used for 
******************************************/
int npu_send_npumon_request(LINX *linx, LINX_SPID server, u32 cmd,STRU_NPUMON_INFO *info);

int npu_receive_npumon_ack(LINX *linx,STRU_NPUMON_INFO *info,FILE *fp);
int np_save2pcap_file(STRU_NPUMON_INFO *info,FILE *fp);
void upi_stats_show(STRU_UPI_STATICS upi_stats);


