/* COPYRIGHT-ENEA-SRC-R2 *
**************************************************************************
* Copyright (C) 2009 by Enea AB.
* All rights reserved.
*
* This Software is furnished under a software license agreement and
* may be used only in accordance with the terms of such agreement.
* Any other use or reproduction is prohibited. No title to and
* ownership of the Software is hereby transferred.
*
* PROPRIETARY NOTICE
* This Software consists of confidential information.
* Trade secret law and copyright law protect this Software.
* The above notice of copyright on this Software does not indicate
* any actual or intended publication of such Software.
**************************************************************************
* COPYRIGHT-END */
/*----------------------------------------------------------------------------
 * File Name	:	  spp_wddi_client.c
 * File Mark	 :	  none
 * Description	:	  npu stats file
 * Others		 :	  none
 * Version       :    v1.00
 * Author		 :
 * Date 		 :
 *
 * Modify History		:
 *	   Date              Version         Author           Modification
 *-----------------------------------------------------------------------------
 2011-3-16             v1.00   Richard zhou
 ----------------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <stdarg.h>
#include <syslog.h>
#include <libgen.h>
#include "spp_npu_link.h"
#include "spp_wddi_common.h"

/*add for debug message write to syslog*/
static const unsigned int facility = LOG_LOCAL0;

int wddi_log_msg(const unsigned int priority, char *file, unsigned int line, char *log_msg, ...);
#define FILE_NOPATH (char *)(strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#define WDDI_WARNINGNL(...)                                             \
    do                                                                  \
    {                                                                   \
        (void) wddi_log_msg(LOG_WARNING, FILE_NOPATH, __LINE__, "\n" __VA_ARGS__); \
    } while (0)

#define printf(fmt, ...)   WDDI_WARNINGNL(fmt,## __VA_ARGS__)


int
wddi_log_msg(const unsigned int priority, char *file, unsigned int line, char *log_msg, ...)
{

    va_list args;
    va_start(args, log_msg);       /*lint !e530 varargs */
    char log_entry_str[1024];
    int cnt = 0;
    cnt = snprintf(log_entry_str, 1024, "%s", " ");

    if(cnt < 0)
    {
        cnt = 0;
    }

    (void)vsnprintf(&(log_entry_str[cnt]), 1024 - cnt, log_msg, args);

    /* Write into the specified syslog. */
    syslog(facility | priority, log_entry_str);
    va_end(args);

    return 0;
}


/* Add  bitfiled for Frame relay packet */
typedef struct
{
    unsigned int dlci:16;
    unsigned int tsgroup:6;
    unsigned int trunk:7;
    unsigned int card:3;
} fraddr_t;
u32
io_fraddr_pack(u32 card, u32 trunk, u32 tsgroup, u32 dlci)
{
    fraddr_t a={0};
    assert(sizeof(fraddr_t) == 4);
    a.card = card-1;
    a.trunk = trunk-1;
    a.tsgroup = tsgroup;
    a.dlci = dlci;
    return *(u32*)&a;
}
void
io_fraddr_unpack(u32 frport, u32* card, u32* trunk, u32* tsgroup, u32* dlci)
{
    fraddr_t* a;
    assert(sizeof(fraddr_t) == 4);
    a = (fraddr_t*)&frport;
    *card = (u32)(a->card) + 1;
    *trunk = (u32)(a->trunk) + 1;
    *tsgroup = a->tsgroup;
    *dlci = a->dlci;
}


void debug_menu_show(void)
{
    char ch;
    int i=0;
    int j=0,hang;
    char string[50][100];
    FILE *f;

    f=fopen("menu.txt","r");
    if(NULL == f)
    {
        printf("fopen file failed!\n");
        return;
    }
    ch=fgetc(f);
    string[j][i]=ch;
    while(ch!=EOF)
    {
        if(ch=='\n')
        {
            string[j++][i]='\0';
            i=0;
        }
        /*putchar(ch);*/
        string[j][i++]=ch;
        ch=fgetc(f);
        hang=j;
    }
    printf("\n");
    /*printf("hang=%d\n",hang+1);*/
    for(j=0;j<=hang;j++)
    {
        for(i=0;string[j][i]!='\0';i++)
        {
            putchar(string[j][i]);
        }
    }
    fclose(f);
    getchar();
}

typedef union StatField
{
    u64 field;
    struct {
        u32 low;
        u32 high;
    } part;
} WPE_StatField;

void txfr_stats_show(u32 vfport, STRU_TXFR_STATICS txfr_stats)
{
	printf("----------------------vfport=%u------------------------\n",vfport);
	printf ("tx_frames:	                       %08x%08x\n",
            ((WPE_StatField)txfr_stats.tx_frames).part.high,
            ((WPE_StatField)txfr_stats.tx_frames).part.low);
	printf ("tx_bytes:	                       %08x%08x\n",
            ((WPE_StatField)txfr_stats.tx_bytes).part.high,
            ((WPE_StatField)txfr_stats.tx_bytes).part.low);
	printf ("tx_frames_de_set:	               %08x%08x\n",
            ((WPE_StatField)txfr_stats.tx_frames_de_set).part.high,
            ((WPE_StatField)txfr_stats.tx_frames_de_set).part.low);
	printf ("tx_bytes_de_set:	               %08x%08x\n",
            ((WPE_StatField)txfr_stats.tx_bytes_de_set).part.high,
            ((WPE_StatField)txfr_stats.tx_bytes_de_set).part.low);
	printf ("tx_frames_fecn_set:	               %08x%08x\n",
            ((WPE_StatField)txfr_stats.tx_frames_fecn_set).part.high,
            ((WPE_StatField)txfr_stats.tx_frames_fecn_set).part.low);
	printf ("tx_frames_becn_set:	               %08x%08x\n",
            ((WPE_StatField)txfr_stats.tx_frames_becn_set).part.high,
            ((WPE_StatField)txfr_stats.tx_frames_becn_set).part.low);
	printf ("tx_aborted_frames:	               %08x%08x\n",
            ((WPE_StatField)txfr_stats.tx_aborted_frames).part.high,
            ((WPE_StatField)txfr_stats.tx_aborted_frames).part.low);
#if 0
	printf("tx_frames       = %#x%x\n",(u32)(txfr_stats.tx_frames >> 32), (u32)txfr_stats.tx_frames);
	printf("tx_bytes       = %#x%x\n",(u32)(txfr_stats.tx_bytes >> 32), (u32)txfr_stats.tx_bytes);
	printf("tx_frames_de_set       = %#x%x\n",(u32)(txfr_stats.tx_frames_de_set >> 32), (u32)txfr_stats.tx_frames_de_set);
	printf("tx_bytes_de_set       = %#x%x\n",(u32)(txfr_stats.tx_bytes_de_set >> 32), (u32)txfr_stats.tx_bytes_de_set);
	printf("tx_frames_fecn_set       = %#x%x\n",(u32)(txfr_stats.tx_frames_fecn_set >> 32), (u32)txfr_stats.tx_frames_fecn_set);
	printf("tx_frames_becn_set       = %#x%x\n",(u32)(txfr_stats.tx_frames_becn_set >> 32), (u32)txfr_stats.tx_frames_becn_set);
	printf("tx_aborted_frames       = %#x%x\n",(u32)(txfr_stats.tx_aborted_frames >> 32), (u32)txfr_stats.tx_aborted_frames);
#endif
	printf("\n");
	return;

}
void rxfr_stats_show(u32 vfport, STRU_RXFR_STATICS rxfr_stats)
{
	printf("\n");
	//printf("Frame relay  Rx Channel 0x%x Statistics:\n",((WPE_StatField)rxfr_stats.rx_frames).part.low /*vfport*/);
	printf("---------------------------------------------------\n");
	printf ("rx_frames: 	               %08x%08x\n",
            ((WPE_StatField)rxfr_stats.rx_frames).part.high,
            ((WPE_StatField)rxfr_stats.rx_frames).part.low);
	printf ("rx_bytes:	               %08x%08x\n",
            ((WPE_StatField)rxfr_stats.rx_bytes).part.high,
            ((WPE_StatField)rxfr_stats.rx_bytes).part.low);
	printf ("rx_frames_de_set:    	       %08x%08x\n",
            ((WPE_StatField)rxfr_stats.rx_frames_de_set).part.high,
            ((WPE_StatField)rxfr_stats.rx_frames_de_set).part.low);
	printf ("rx_bytes_de_set:	       %08x%08x\n",
            ((WPE_StatField)rxfr_stats.rx_bytes_de_set).part.high,
            ((WPE_StatField)rxfr_stats.rx_bytes_de_set).part.low);
	printf ("rx_frames_fecn_set:	       %08x%08x\n",
            ((WPE_StatField)rxfr_stats.rx_frames_fecn_set).part.high,
            ((WPE_StatField)rxfr_stats.rx_frames_fecn_set).part.low);
	printf ("rx_frames_becn_set:	       %08x%08x\n",
            ((WPE_StatField)rxfr_stats.rx_frames_becn_set).part.high,
            ((WPE_StatField)rxfr_stats.rx_frames_becn_set).part.low);
	printf ("rx_discarded_err_crc:	       %08x%08x\n",
            ((WPE_StatField)rxfr_stats.rx_discarded_err_crc).part.high,
            ((WPE_StatField)rxfr_stats.rx_discarded_err_crc).part.low);
	printf ("rx_discarded_err_alignment:    %08x%08x\n",
            ((WPE_StatField)rxfr_stats.rx_discarded_err_alignment).part.high,
            ((WPE_StatField)rxfr_stats.rx_discarded_err_alignment).part.low);
	printf ("rx_discarded_length_violation: %08x%08x\n",
            ((WPE_StatField)rxfr_stats.rx_discarded_length_violation).part.high,
            ((WPE_StatField)rxfr_stats.rx_discarded_length_violation).part.low);
	printf ("rx_discarded_abort:	       %08x%08x\n",
            ((WPE_StatField)rxfr_stats.rx_discarded_abort).part.high,
            ((WPE_StatField)rxfr_stats.rx_discarded_abort).part.low);
	printf ("rx_discarded_fbp_empty:	       %08x%08x\n",
            ((WPE_StatField)rxfr_stats.rx_discarded_fbp_empty).part.high,
            ((WPE_StatField)rxfr_stats.rx_discarded_fbp_empty).part.low);
	printf ("rx_discarded_hostq_full:       %08x%08x\n",
            ((WPE_StatField)rxfr_stats.rx_discarded_hostq_full).part.high,
            ((WPE_StatField)rxfr_stats.rx_discarded_hostq_full).part.low);
#if 0
	printf("rx_frames		= %#x%x\n",(u32)(rxfr_stats.rx_frames >> 32), (u32)rxfr_stats.rx_frames);
	printf("rx_bytes	 = %#x%x\n",(u32)(rxfr_stats.rx_bytes >> 32), (u32)rxfr_stats.rx_bytes);
	printf("rx_frames_de_set	   = %#x%x\n",(u32)(rxfr_stats.rx_frames_de_set >> 32), (u32)rxfr_stats.rx_frames_de_set);
	printf("rx_bytes_de_set  = %#x%x\n",(u32)(rxfr_stats.rx_bytes_de_set >> 32), (u32)rxfr_stats.rx_bytes_de_set);

	printf("rx_frames_fecn_set		 = %#x%x\n",(u32)(rxfr_stats.rx_frames_fecn_set >> 32), (u32)rxfr_stats.rx_frames_fecn_set);
	printf("rx_frames_becn_set	   = %#x%x\n",(u32)(rxfr_stats.rx_frames_becn_set >> 32), (u32)rxfr_stats.rx_frames_becn_set);
	printf("rx_discarded_err_crc	   = %#x%x\n",(u32)(rxfr_stats.rx_discarded_err_crc >> 32), (u32)rxfr_stats.rx_discarded_err_crc);
	printf("rx_discarded_err_alignment	= %#x%x\n",(u32)(rxfr_stats.rx_discarded_err_alignment >> 32), (u32)rxfr_stats.rx_discarded_err_alignment);


	printf("rx_discarded_length_violation	  = %#x%x\n",(u32)(rxfr_stats.rx_discarded_length_violation >> 32), (u32)rxfr_stats.rx_discarded_length_violation);
	printf("rx_discarded_abort		 = %#x%x\n",(u32)(rxfr_stats.rx_discarded_abort >> 32), (u32)rxfr_stats.rx_discarded_abort);
	printf("rx_discarded_fbp_empty	= %#x%x\n",(u32)(rxfr_stats.rx_discarded_fbp_empty >> 32), (u32)rxfr_stats.rx_discarded_fbp_empty);
	printf("rx_discarded_hostq_full  = %#x%x\n",(u32)(rxfr_stats.rx_discarded_hostq_full >> 32), (u32)rxfr_stats.rx_discarded_hostq_full);
#endif
	printf("\n");
	return;

}


void upi_stats_show(u32 showid, STRU_UPI_STATICS upi_stats)
{
#if 1
    printf("----------------------------------------------------\n");

    printf ("EMPHY frame relay Stats on showid 0x%x\n",showid);

    printf ("rx_frames:	    	              %08x%08x\n",
			((WPE_StatField)upi_stats.rx_frames).part.high,
			((WPE_StatField)upi_stats.rx_frames).part.low);
    printf ("rx_bytes:			      %08x%08x\n",
			((WPE_StatField)upi_stats.rx_bytes).part.high,
			((WPE_StatField)upi_stats.rx_bytes).part.low);
    printf ("rx_frames_de_set:		      %08x%08x\n",
			((WPE_StatField)upi_stats.rx_frames_de_set).part.high,
			((WPE_StatField)upi_stats.rx_frames_de_set).part.low);
    printf ("rx_bytes_de_set:		      %08x%08x\n",
			((WPE_StatField)upi_stats.rx_bytes_de_set).part.high,
			((WPE_StatField)upi_stats.rx_bytes_de_set).part.low);
	printf ("rx_frames_fecn_set:		      %08x%08x\n",
			((WPE_StatField)upi_stats.rx_frames_fecn_set).part.high,
			((WPE_StatField)upi_stats.rx_frames_fecn_set).part.low);
    printf ("rx_frames_becn_set:		      %08x%08x\n",
			((WPE_StatField)upi_stats.rx_frames_becn_set).part.high,
			((WPE_StatField)upi_stats.rx_frames_becn_set).part.low);
	printf ("rx_discarded_err_crc:		      %08x%08x\n",
			((WPE_StatField)upi_stats.rx_discarded_err_crc).part.high,
			((WPE_StatField)upi_stats.rx_discarded_err_crc).part.low);
	printf ("rx_discarded_err_alignment:	      %08x%08x\n",
			((WPE_StatField)upi_stats.rx_discarded_err_alignment).part.high,
			((WPE_StatField)upi_stats.rx_discarded_err_alignment).part.low);
	printf ("rx_discarded_length_violation:	      %08x%08x\n",
			((WPE_StatField)upi_stats.rx_discarded_length_violation).part.high,
			((WPE_StatField)upi_stats.rx_discarded_length_violation).part.low);
	printf ("rx_discarded_illegal_header:	      %08x%08x\n",
			((WPE_StatField)upi_stats.rx_discarded_illegal_header).part.high,
			((WPE_StatField)upi_stats.rx_discarded_illegal_header).part.low);
	printf ("rx_discarded_abort:		      %08x%08x\n",
			((WPE_StatField)upi_stats.rx_discarded_abort).part.high,
			((WPE_StatField)upi_stats.rx_discarded_abort).part.low);
	printf ("rx_discarded_unsupported_dlci:	      %08x%08x\n",
			((WPE_StatField)upi_stats.rx_discarded_unsupported_dlci).part.high,
			((WPE_StatField)upi_stats.rx_discarded_unsupported_dlci).part.low);
	printf ("rx_last_unsupported_dlci:	      %016x\n",
			upi_stats.rx_last_unsupported_dlci);
	printf ("rx_discarded_fbp_empty:		      %08x%08x\n",
			((WPE_StatField)upi_stats.rx_discarded_fbp_empty).part.high,
			((WPE_StatField)upi_stats.rx_discarded_fbp_empty).part.low);
	printf ("rx_discarded_hostq_full:	      %08x%08x\n",
			((WPE_StatField)upi_stats.rx_discarded_hostq_full).part.high,
			((WPE_StatField)upi_stats.rx_discarded_hostq_full).part.low);
	printf ("rx_discarded_err_parity:	      %08x%08x\n",
			((WPE_StatField)upi_stats.rx_discarded_err_parity).part.high,
			((WPE_StatField)upi_stats.rx_discarded_err_parity).part.low);
	printf ("rx_discarded_err_sop_eop:	      %08x%08x\n",
			((WPE_StatField)upi_stats.rx_discarded_err_sop_eop).part.high,
			((WPE_StatField)upi_stats.rx_discarded_err_sop_eop).part.low);
	printf ("rx_total_err_bytes:		      %08x%08x\n",
			((WPE_StatField)upi_stats.rx_total_err_bytes).part.high,
			((WPE_StatField)upi_stats.rx_total_err_bytes).part.low);
	printf ("tx_frames:		              %08x%08x\n",
			((WPE_StatField)upi_stats.tx_frames).part.high,
			((WPE_StatField)upi_stats.tx_frames).part.low);
	printf ("tx_bytes:		              %08x%08x\n",
			((WPE_StatField)upi_stats.tx_bytes).part.high,
			((WPE_StatField)upi_stats.tx_bytes).part.low);
	printf ("tx_frames_de_set:		      %08x%08x\n",
			((WPE_StatField)upi_stats.tx_frames_de_set).part.high,
			((WPE_StatField)upi_stats.tx_frames_de_set).part.low);
	printf ("tx_bytes_de_set:		      %08x%08x\n",
			((WPE_StatField)upi_stats.tx_bytes_de_set).part.high,
			((WPE_StatField)upi_stats.tx_bytes_de_set).part.low);
	printf ("tx_frames_fecn_set:		      %08x%08x\n",
			((WPE_StatField)upi_stats.tx_frames_fecn_set).part.high,
			((WPE_StatField)upi_stats.tx_frames_fecn_set).part.low);
	printf ("tx_frames_becn_set:		      %08x%08x\n",
			((WPE_StatField)upi_stats.tx_frames_becn_set).part.high,
			((WPE_StatField)upi_stats.tx_frames_becn_set).part.low);
	printf ("tx_underrun:		              %08x%08x\n",
			((WPE_StatField)upi_stats.tx_underrun).part.high,
			((WPE_StatField)upi_stats.tx_underrun).part.low);
	printf ("tx_aborted_frames:		      %08x%08x\n",
			((WPE_StatField)upi_stats.tx_aborted_frames).part.high,
			((WPE_StatField)upi_stats.tx_aborted_frames).part.low);
	printf ("serial_dps_if_tx_frames:	      %08x%08x\n",
			((WPE_StatField)upi_stats.serial_dps_if_tx_frames).part.high,
			((WPE_StatField)upi_stats.serial_dps_if_tx_frames).part.low);
	printf ("serial_dps_if_tx_bytes:		      %08x%08x\n",
			((WPE_StatField)upi_stats.serial_dps_if_tx_bytes).part.high,
			((WPE_StatField)upi_stats.serial_dps_if_tx_bytes).part.low);
	printf ("serial_dps_if_rx_frames:	      %08x%08x\n",
			((WPE_StatField)upi_stats.serial_dps_if_rx_frames).part.high,
			((WPE_StatField)upi_stats.serial_dps_if_rx_frames).part.low);
	printf ("serial_dps_if_rx_bytes:		      %08x%08x\n",
			((WPE_StatField)upi_stats.serial_dps_if_rx_bytes).part.high,
			((WPE_StatField)upi_stats.serial_dps_if_rx_bytes).part.low);
	printf ("serial_dps_if_rx_dropped_frames:      %08x%08x\n",
			((WPE_StatField)upi_stats.serial_dps_if_rx_dropped_frames).part.high,
			((WPE_StatField)upi_stats.serial_dps_if_rx_dropped_frames).part.low);
	printf ("serial_dps_if_rx_pce_denied_frames:   %08x%08x\n",
			((WPE_StatField)upi_stats.serial_dps_if_rx_pce_denied_frames).part.high,
			((WPE_StatField)upi_stats.serial_dps_if_rx_pce_denied_frames).part.low);
	printf("\n");
#endif
	return;

}


void enet_stats_show(u32 EthNumber, STRU_ETH_STATICS enet_stats)
{
    printf("----------------------------------------------------\n");

    printf ("HS ENET Port %d Statistics (HW)\n",EthNumber);

    printf ("TxRx Frame  64:			  %08x%08x\n",
            ((WPE_StatField)enet_stats.txrx_frames_64).part.high,
            ((WPE_StatField)enet_stats.txrx_frames_64).part.low);

    printf ("TxRx Frame 127:			  %08x%08x\n",
            ((WPE_StatField)enet_stats.txrx_frames_127).part.high,
            ((WPE_StatField)enet_stats.txrx_frames_127).part.low);

    printf ("TxRx Frame 255:			  %08x%08x\n",
            ((WPE_StatField)enet_stats.txrx_frames_255).part.high,
            ((WPE_StatField)enet_stats.txrx_frames_255).part.low);

    printf ("TxRx Frame 1023:		  %08x%08x\n",
            ((WPE_StatField)enet_stats.txrx_frames_1023).part.high,
            ((WPE_StatField)enet_stats.txrx_frames_1023).part.low);

    printf ("TxRx Frame 1518:		  %08x%08x\n",
            ((WPE_StatField)enet_stats.txrx_frames_1518).part.high,
            ((WPE_StatField)enet_stats.txrx_frames_1518).part.low);

    printf ("TxRx Frame 1522:		  %08x%08x\n",
            ((WPE_StatField)enet_stats.txrx_frames_1522).part.high,
            ((WPE_StatField)enet_stats.txrx_frames_1522).part.low);

    printf ("Rx Bytes:			  %08x%08x\n",
            ((WPE_StatField)enet_stats.rx_bytes).part.high,
            ((WPE_StatField)enet_stats.rx_bytes).part.low);

    printf ("Rx Packets:			  %08x%08x\n",
            ((WPE_StatField)enet_stats.rx_packets).part.high,
            ((WPE_StatField)enet_stats.rx_packets).part.low);

    printf ("Rx Error FCS:			  %08x%08x\n",
            ((WPE_StatField)enet_stats.rx_err_fcs).part.high,
            ((WPE_StatField)enet_stats.rx_err_fcs).part.low);

    printf ("Rx Multicast:			  %08x%08x\n",
            ((WPE_StatField)enet_stats.rx_multicast).part.high,
            ((WPE_StatField)enet_stats.rx_multicast).part.low);

    printf ("Rx Broadcast:			  %08x%08x\n",
            ((WPE_StatField)enet_stats.rx_broadcast).part.high,
            ((WPE_StatField)enet_stats.rx_broadcast).part.low);

    printf ("Rx Mac Control:			  %08x%08x\n",
            ((WPE_StatField)enet_stats.rx_mac_control).part.high,
            ((WPE_StatField)enet_stats.rx_mac_control).part.low);

    printf ("Rx Mac Pause:			  %08x%08x\n",
            ((WPE_StatField)enet_stats.rx_mac_pause).part.high,
            ((WPE_StatField)enet_stats.rx_mac_pause).part.low);

    printf ("Rx Mac Unknown:			  %08x%08x\n",
            ((WPE_StatField)enet_stats.rx_mac_unknown).part.high,
            ((WPE_StatField)enet_stats.rx_mac_unknown).part.low);

    printf ("Rx Error Alignment:		  %08x%08x\n",
            ((WPE_StatField)enet_stats.rx_err_alignment).part.high,
            ((WPE_StatField)enet_stats.rx_err_alignment).part.low);

    printf ("Rx Error LEN:			  %08x%08x\n",
            ((WPE_StatField)enet_stats.rx_err_length).part.high,
            ((WPE_StatField)enet_stats.rx_err_length).part.low);

    printf ("Rx Error Code: 			  %08x%08x\n",
            ((WPE_StatField)enet_stats.rx_err_code).part.high,
            ((WPE_StatField)enet_stats.rx_err_code).part.low);

    printf ("Rx False Carrier:		  %08x%08x\n",
            ((WPE_StatField)enet_stats.rx_false_carrier).part.high,
            ((WPE_StatField)enet_stats.rx_false_carrier).part.low);

    printf ("Rx Undersize:			  %08x%08x\n",
            ((WPE_StatField)enet_stats.rx_undersize).part.high,
            ((WPE_StatField)enet_stats.rx_undersize).part.low);

    printf ("Rx Oversize:			  %08x%08x\n",
            ((WPE_StatField)enet_stats.rx_oversize).part.high,
            ((WPE_StatField)enet_stats.rx_oversize).part.low);

    printf ("Rx Fragments:			  %08x%08x\n",
            ((WPE_StatField)enet_stats.rx_fragments).part.high,
            ((WPE_StatField)enet_stats.rx_fragments).part.low);
    printf ("Rx Jabber: 			  %08x%08x\n",
            ((WPE_StatField)enet_stats.rx_jabber).part.high,
            ((WPE_StatField)enet_stats.rx_jabber).part.low);
    printf ("Rx Dropped:			  %08x%08x\n",
            ((WPE_StatField)enet_stats.rx_dropped).part.high,
            ((WPE_StatField)enet_stats.rx_dropped).part.low);

    printf ("Tx Bytes:			  %08x%08x\n",
            ((WPE_StatField)enet_stats.tx_bytes).part.high,
            ((WPE_StatField)enet_stats.tx_bytes).part.low);

    printf ("Tx Packets:			  %08x%08x\n",
            ((WPE_StatField)enet_stats.tx_packets).part.high,
            ((WPE_StatField)enet_stats.tx_packets).part.low);

    printf ("Tx Multicast:			  %08x%08x\n",
            ((WPE_StatField)enet_stats.tx_multicast).part.high,
            ((WPE_StatField)enet_stats.tx_multicast).part.low);

    printf ("Tx Broadcast:			  %08x%08x\n",
            ((WPE_StatField)enet_stats.tx_broadcast).part.high,
            ((WPE_StatField)enet_stats.tx_broadcast).part.low);

    printf ("Tx Mac Pause:			  %08x%08x\n",
            ((WPE_StatField)enet_stats.tx_mac_pause).part.high,
            ((WPE_StatField)enet_stats.tx_mac_pause).part.low);

    printf ("Tx Defer:			  %08x%08x\n",
            ((WPE_StatField)enet_stats.tx_defer).part.high,
            ((WPE_StatField)enet_stats.tx_defer).part.low);

    printf ("Tx Exess Defer:			  %08x%08x\n",
            ((WPE_StatField)enet_stats.tx_excess_defer).part.high,
            ((WPE_StatField)enet_stats.tx_excess_defer).part.low);

    printf ("Tx Single Collision:		  %08x%08x\n",
            ((WPE_StatField)enet_stats.tx_single_collision).part.high,
            ((WPE_StatField)enet_stats.tx_single_collision).part.low);

    printf ("Tx Multi Collision:		  %08x%08x\n",
            ((WPE_StatField)enet_stats.tx_multi_collision).part.high,
            ((WPE_StatField)enet_stats.tx_multi_collision).part.low);

    printf ("Tx Late Collision: 		  %08x%08x\n",
            ((WPE_StatField)enet_stats.tx_late_collision).part.high,
            ((WPE_StatField)enet_stats.tx_late_collision).part.low);

    printf ("Tx Excess Collision:		  %08x%08x\n",
            ((WPE_StatField)enet_stats.tx_excess_collision).part.high,
            ((WPE_StatField)enet_stats.tx_excess_collision).part.low);

    printf ("Tx No Collision:		  %08x%08x\n",
            ((WPE_StatField)enet_stats.tx_no_collision).part.high,
            ((WPE_StatField)enet_stats.tx_no_collision).part.low);

    printf ("Tx Mac Pause Honored:		  %08x%08x\n",
            ((WPE_StatField)enet_stats.tx_mac_pause_honored).part.high,
            ((WPE_StatField)enet_stats.tx_mac_pause_honored).part.low);

    printf ("Tx Dropped:			  %08x%08x\n",
            ((WPE_StatField)enet_stats.tx_dropped).part.high,
            ((WPE_StatField)enet_stats.tx_dropped).part.low);

    printf ("Tx Jabber: 			  %08x%08x\n",
            ((WPE_StatField)enet_stats.tx_jabber).part.high,
            ((WPE_StatField)enet_stats.tx_jabber).part.low);

    printf ("Tx Errors FCS: 			  %08x%08x\n",
            ((WPE_StatField)enet_stats.tx_err_fcs).part.high,
            ((WPE_StatField)enet_stats.tx_err_fcs).part.low);

    printf ("Tx Control:			  %08x%08x\n",
            ((WPE_StatField)enet_stats.tx_control).part.high,
            ((WPE_StatField)enet_stats.tx_control).part.low);

    printf ("Tx Oversize:			  %08x%08x\n",
            ((WPE_StatField)enet_stats.tx_oversize).part.high,
            ((WPE_StatField)enet_stats.tx_oversize).part.low);

    printf ("Tx Undersize:			  %08x%08x\n",
            ((WPE_StatField)enet_stats.tx_undersize).part.high,
            ((WPE_StatField)enet_stats.tx_undersize).part.low);

    printf ("Tx Fragments:			  %08x%08x\n",
            ((WPE_StatField)enet_stats.tx_fragments).part.high,
            ((WPE_StatField)enet_stats.tx_fragments).part.low);


    printf ("HS ENET Device Statistics (DPS)\n");

    printf ("Rx Host Frames:			  %08x%08x\n",
            ((WPE_StatField)enet_stats.rx_host_frames).part.high,
            ((WPE_StatField)enet_stats.rx_host_frames).part.low);
    printf ("Rx Iw Frames:			  %08x%08x\n",
            ((WPE_StatField)enet_stats.rx_iw_frames).part.high,
            ((WPE_StatField)enet_stats.rx_iw_frames).part.low);
    printf ("Rx Error Mru:			  %08x%08x\n",
            ((WPE_StatField)enet_stats.rx_err_mru).part.high,
            ((WPE_StatField)enet_stats.rx_err_mru).part.low);
    printf ("Rx Error Overrun:		  %08x%08x\n",
            ((WPE_StatField)enet_stats.rx_err_overrun).part.high,
            ((WPE_StatField)enet_stats.rx_err_overrun).part.low);
    printf ("Rx Error Host Full:		  %08x%08x\n",
            ((WPE_StatField)enet_stats.rx_err_host_full).part.high,
            ((WPE_StatField)enet_stats.rx_err_host_full).part.low);
    printf ("Rx Error FBP Underrun: 		  %08x%08x\n",
            ((WPE_StatField)enet_stats.rx_err_fbp_underrun).part.high,
            ((WPE_StatField)enet_stats.rx_err_fbp_underrun).part.low);
    printf ("Rx Error Non Valid MAC:	 	  %08x%08x\n",
            ((WPE_StatField)enet_stats.rx_err_nonvalid_mac).part.high,
            ((WPE_StatField)enet_stats.rx_err_nonvalid_mac).part.low);
    printf ("Rx Error SDU:			  %08x%08x\n",
            ((WPE_StatField)enet_stats.rx_err_sdu).part.high,
            ((WPE_StatField)enet_stats.rx_err_sdu).part.low);
    printf ("Tx Error Underrun: 		  %08x%08x\n",
            ((WPE_StatField)enet_stats.tx_err_underrun).part.high,
            ((WPE_StatField)enet_stats.tx_err_underrun).part.low);
    printf ("Tx Frames: 			  %08x%08x\n",
            ((WPE_StatField)enet_stats.tx_frames).part.high,
            ((WPE_StatField)enet_stats.tx_frames).part.low);
    printf("----------------------------------------------------\n");

	return;

}

void cpu_stats_show(STRU_CPU_STATICS stru_cpu_statics)
{
	int i;

	printf("Wingine utilization: \n");
	printf("---------------------------------------------------\n");

	printf("totalUtili	 = %d%%\n",stru_cpu_statics.totalUtili);
	for(i = 0; i < 12; i ++)
	{
		printf("WinGinUtili[i]	 = %d%%\n",stru_cpu_statics.WinGinUtili[i]);
	}

	printf("\n");
	return;

}
void np_set_fr_uplink_path_rule_sturct(STRU_SET_UPLINK_PATH_RULE *p)
{
#if 0
	/*gloab initial for test */
    p->IpDst = 0;
	p->IpSrc = 0;
	p->PortDst = 0;
	p->PortSrc = 0;
	p->TLLIValue = 0;
	p->TLLI_bitLen = 0;
	p->vfport = 0;
#endif
}

int npu_send_debug_request(LINX *linx, LINX_SPID server, u32 cmd, int argc, u16 *pargv)
{
	union LINX_SIGNAL *sig;

	switch(cmd)
	{
    case NPU_SET_RECONFIG:
    {

        sig = linx_alloc(linx, sizeof(struct stru_npu_request_sig), NPU_REQUEST_SIG);
        if (sig == NULL)
        {
            printf("linx_alloc() failed\n");
            return ERR_LINX_ALLOC;
        }
        /*fill info struct*/
        //np_reconfig_set_sturct(&stru_reconfig);
        sig->npu_request.cmdType = NPU_SET_REQUEST_TYPE;
        sig->npu_request.cmd = cmd;
        break;
    }
    case NPU_SET_LOG_DEBUG_LEVEL:
    {

        STRU_NPU_SET_LOG_DEBUG_LEVEL npu_set_log_debug_level;
        sig = linx_alloc(linx, sizeof(struct stru_npu_request_sig)+sizeof(STRU_NPU_SET_LOG_DEBUG_LEVEL), NPU_REQUEST_SIG);
        if (sig == NULL)
        {
            printf("linx_alloc() failed\n");
            return ERR_LINX_ALLOC;
        }
        /*fill info struct*/
        //np_reconfig_set_sturct(&stru_reconfig);
        npu_set_log_debug_level.level = *pargv;
        /*
          NPU_LOG_INFO = 0,
          NPU_LOG_NOTICE,
          NPU_LOG_WARNING,
          NPU_LOG_ERR,
          NPU_LOG_CRIT,
          NPU_LOG_ALERT,
          NPU_LOG_EMERG


        */
        sig->npu_request.cmdType = NPU_SET_REQUEST_TYPE;
        sig->npu_request.cmd = cmd;
        memcpy(sig->npu_request.pargv,(void *)&npu_set_log_debug_level,sizeof(npu_set_log_debug_level));
        break;
    }
    case NPU_SET_LMI_PVC_STATUS:
    {
        STRU_NPU_SET_LMI_PVC_STATUS npu_set_pvcstatus;
        sig = linx_alloc(linx, sizeof(struct stru_npu_request_sig)+sizeof(STRU_NPU_SET_LMI_PVC_STATUS), NPU_REQUEST_SIG);
        if (sig == NULL)
        {
            printf("linx_alloc() failed\n");
            return ERR_LINX_ALLOC;
        }
        /*fill info struct*/
        if(argc == 5)
        {
            u32 fsport = io_fraddr_pack(*pargv,*(pargv+1),*(pargv+2),*(pargv+3));
            npu_set_pvcstatus.vfport = fsport;
            npu_set_pvcstatus.status = *(pargv+4);
        }
        else
        {
            printf("input parameters numbers error!\n");
            return 0;

        }
        sig->npu_request.cmdType = NPU_SET_REQUEST_TYPE;
        sig->npu_request.cmd = cmd;
        memcpy(sig->npu_request.pargv,(void *)&npu_set_pvcstatus,sizeof(npu_set_pvcstatus));
        break;
    }
    //case nseAgg

    case NPU_SET_NS_BSSGP_STATICS_ZERO:
    {
        STRU_SET_NS_BSSGP_STATICS_ZERO npu_set_statistics;
        sig = linx_alloc(linx, sizeof(struct stru_npu_request_sig)+sizeof(STRU_SET_NS_BSSGP_STATICS_ZERO), NPU_REQUEST_SIG);
        if (sig == NULL)
        {
            printf("linx_alloc() failed\n");
            return ERR_LINX_ALLOC;
        }
        u32 vfport;
        if(*(pargv) == 0) /*0: fr MODE 1:IP MODE*/
        {
            if(argc == 5)
            {
                vfport = io_fraddr_pack(*(pargv+1),*(pargv+2),*(pargv+3),*(pargv+4));
                npu_set_statistics.vfport = vfport;
                printf("PortNumber is %d\n",vfport);
            }
            else
            {
                printf("input parameters numbers error!\n");
                return 0;
            }
        }
        else
        {
            vfport = *(pargv+1);
            if(argc == 2)
            {
                npu_set_statistics.vfport = vfport;
                printf("PortNumber is %d\n",vfport);
            }
            else
            {
                printf("input parameters numbers error!\n");
                return 0;
            }
        }

        sig->npu_request.cmdType = NPU_SET_REQUEST_TYPE;
        sig->npu_request.cmd = cmd;
        memcpy(sig->npu_request.pargv,(void *)&npu_set_statistics,sizeof(npu_set_statistics));
        break;
    }
    case NPU_SET_NSEAGG_FLAG:
    {
        STRU_SET_NPU_NSEAGG_FLAG npu_set_NSEAGG_FLAG;
        sig = linx_alloc(linx, sizeof(struct stru_npu_request_sig)+sizeof(STRU_SET_NPU_NSEAGG_FLAG), NPU_REQUEST_SIG);
        if (sig == NULL)
        {
            printf("linx_alloc() failed\n");
            return ERR_LINX_ALLOC;
        }
        /*fill info struct*/
        npu_set_NSEAGG_FLAG.nseAggFlag = *pargv;
        sig->npu_request.cmdType = NPU_SET_REQUEST_TYPE;
        sig->npu_request.cmd = cmd;
        memcpy(sig->npu_request.pargv,(void *)&npu_set_NSEAGG_FLAG,sizeof(npu_set_NSEAGG_FLAG));
        break;
    }
    case NPU_GET_ETH_STATICS:
    {
        printf("NPU_GET_ETH_STATICS in###\n");

        STRU_GET_ETH_STATICS stru_get_eth_statics;
        sig = linx_alloc(linx, sizeof(struct stru_npu_request_sig) + sizeof(stru_get_eth_statics)-sizeof(STRU_ETH_STATICS), NPU_REQUEST_SIG);
        if (sig == NULL)
        {
            printf("linx_alloc() failed\n");
            return ERR_LINX_ALLOC;
        }
        /*fill info struct*/
        stru_get_eth_statics.EthNumber = 1;
        if(argc == 1)
        {
            printf("argc is %d\n",argc);
            stru_get_eth_statics.EthNumber = *pargv;
            printf("EthNumber is %d\n",stru_get_eth_statics.EthNumber);

        }
        else
        {
            printf("input parameters numbers error!\n");
            return 0;
        }
        sig->npu_request.cmdType = NPU_GET_REQUEST_TYPE;
        sig->npu_request.cmd = cmd;
        memcpy(sig->npu_request.pargv,(void *)&stru_get_eth_statics,sizeof(stru_get_eth_statics)-sizeof(STRU_ETH_STATICS));
        break;
    }

    case NPU_GET_UPI_TRUNK_STATICS:
    {
        printf("NPU_GET_UPI_STATICS in###\n");

        STRU_GET_UPI_TRUNK_STATICS stru_get_upi_statics;
        sig = linx_alloc(linx, sizeof(struct stru_npu_request_sig) + sizeof(stru_get_upi_statics)-sizeof(STRU_UPI_STATICS), NPU_REQUEST_SIG);
        if (sig == NULL)
        {
            printf("linx_alloc() failed\n");
            return ERR_LINX_ALLOC;
        }
        /*fill info struct*/
        stru_get_upi_statics.trunkid= 0;
        if(argc == 2)
        {
            stru_get_upi_statics.stm1Id = *pargv;
            stru_get_upi_statics.trunkid= *(pargv+1);
            printf("stm1Id = %d trunkId is %d\n",stru_get_upi_statics.stm1Id,stru_get_upi_statics.trunkid);

        }
        else
        {
            printf("input parameters numbers error!\n");
            return 0;

        }
        sig->npu_request.cmdType = NPU_GET_REQUEST_TYPE;
        sig->npu_request.cmd = cmd;
        memcpy(sig->npu_request.pargv,(void *)&stru_get_upi_statics,sizeof(stru_get_upi_statics)-sizeof(STRU_UPI_STATICS));
        break;
    }
    case NPU_GET_UPI_GROUP_STATICS:
    {
        printf("NPU_GET_UPI_GROUP_STATICS in###\n");

        STRU_GET_UPI_GROUP_STATICS stru_get_upi_statics;
        sig = linx_alloc(linx, sizeof(struct stru_npu_request_sig) + sizeof(stru_get_upi_statics)-sizeof(STRU_UPI_STATICS), NPU_REQUEST_SIG);
        if (sig == NULL)
        {
            printf("linx_alloc() failed\n");
            return ERR_LINX_ALLOC;
        }
        /*fill info struct*/
        stru_get_upi_statics.trunkid= 0;
        if(argc == 3)
        {
            stru_get_upi_statics.stm1Id = *pargv;
            stru_get_upi_statics.trunkid= *(pargv+1);
            stru_get_upi_statics.groupid = *(pargv+2);
            printf("stm1Id = %d trunkId is %d groupId = %d \n",stru_get_upi_statics.stm1Id,stru_get_upi_statics.trunkid,stru_get_upi_statics.groupid);

        }
        else
        {
            printf("input parameters numbers error!\n");
            return 0;
        }
        sig->npu_request.cmdType = NPU_GET_REQUEST_TYPE;
        sig->npu_request.cmd = cmd;
        memcpy(sig->npu_request.pargv,(void *)&stru_get_upi_statics,sizeof(stru_get_upi_statics)-sizeof(STRU_UPI_STATICS));
        break;
    }

    case NPU_GET_UPI_STM1_STATICS:
    {
        printf("NPU_GET_UPI_STM1_STATICS in###\n");

        STRU_GET_UPI_STM1_STATICS stru_get_upi_statics;
        sig = linx_alloc(linx, sizeof(struct stru_npu_request_sig) + sizeof(stru_get_upi_statics)-sizeof(STRU_UPI_STATICS), NPU_REQUEST_SIG);
        if (sig == NULL)
        {
            printf("linx_alloc() failed\n");
            return ERR_LINX_ALLOC;
        }
        /*fill info struct*/
        stru_get_upi_statics.stm1id= 0;
        if(argc == 1)
        {
            stru_get_upi_statics.stm1id= *pargv;
            printf("stm1id is %d\n",stru_get_upi_statics.stm1id);

        }
        else
        {
            printf("input parameters numbers error!\n");
            return 0;
        }
        sig->npu_request.cmdType = NPU_GET_REQUEST_TYPE;
        sig->npu_request.cmd = cmd;
        memcpy(sig->npu_request.pargv,(void *)&stru_get_upi_statics,sizeof(stru_get_upi_statics)-sizeof(STRU_UPI_STATICS));
        break;
    }

    case NPU_GET_RXFR_STATICS:
    {
        printf("NPU_GET_RXFR_STATICS in###\n");

        STRU_GET_RXFR_STATICS stru_get_rxfr_statics;
        sig = linx_alloc(linx, sizeof(struct stru_npu_request_sig) + sizeof(stru_get_rxfr_statics)-sizeof(STRU_RXFR_STATICS), NPU_REQUEST_SIG);
        if (sig == NULL)
        {
            printf("linx_alloc() failed\n");
            return ERR_LINX_ALLOC;
        }
        if(argc == 4)
        {
            printf("argc is %d\n",argc);
            /*fill info struct*/
            u32 fsport = io_fraddr_pack(*pargv,*(pargv+1),*(pargv+2),*(pargv+3));
            stru_get_rxfr_statics.vfport = fsport;
            printf("rx channel index is %d\n",stru_get_rxfr_statics.vfport);

        }
        else
        {
            printf("input parameters numbers error!\n");
            return 0;
        }
        sig->npu_request.cmdType = NPU_GET_REQUEST_TYPE;
        sig->npu_request.cmd = cmd;
        memcpy(sig->npu_request.pargv,(void *)&stru_get_rxfr_statics,sizeof(stru_get_rxfr_statics)-sizeof(STRU_RXFR_STATICS));
        break;

    }

    case NPU_GET_TXFR_STATICS:
    {
        printf("NPU_GET_TXFR_STATICS in###\n");

        STRU_GET_TXFR_STATICS stru_get_txfr_statics;
        sig = linx_alloc(linx, sizeof(struct stru_npu_request_sig) + sizeof(stru_get_txfr_statics)-sizeof(STRU_TXFR_STATICS), NPU_REQUEST_SIG);
        if (sig == NULL)
        {
            printf("linx_alloc() failed\n");
            return ERR_LINX_ALLOC;
        }
        /*fill info struct*/
        stru_get_txfr_statics.vfport = 1;
        if(argc == 4)
        {
            printf("argc is %d\n",argc);
            u32 fsport = io_fraddr_pack(*pargv,*(pargv+1),*(pargv+2),*(pargv+3));
            stru_get_txfr_statics.vfport = fsport;
            printf("tx channel index is %d\n",stru_get_txfr_statics.vfport);
        }
        else
        {
            printf("input parameters numbers error!\n");
            return 0;
        }
        sig->npu_request.cmdType = NPU_GET_REQUEST_TYPE;
        sig->npu_request.cmd = cmd;
        memcpy(sig->npu_request.pargv,(void *)&stru_get_txfr_statics,sizeof(stru_get_txfr_statics)-sizeof(STRU_TXFR_STATICS));
        break;

    }

    case NPU_DEBUG_CHANNEL:
    {
        printf("NPU_DEBUG_CHANNEL in###\n");

        STRU_DEBUG_CHANNEL stru_debug_channel;
        sig = linx_alloc(linx, sizeof(struct stru_npu_request_sig) + sizeof(stru_debug_channel), NPU_REQUEST_SIG);
        if (sig == NULL)
        {
            printf("linx_alloc() failed\n");
            return ERR_LINX_ALLOC;
        }
        /*fill info struct*/
        stru_debug_channel.vfport = 1;
        if(argc == 5)
        {
            printf("argc is %d\n",argc);
            u32 fsport = io_fraddr_pack(*pargv,*(pargv+1),*(pargv+2),*(pargv+3));
            stru_debug_channel.vfport = fsport;
            stru_debug_channel.DebugType = *(pargv+4);
            printf("channel index is %d\n",stru_debug_channel.vfport);
        }
        else
        {
            printf("input parameters numbers error!\n");
            return 0;
        }
        sig->npu_request.cmdType = NPU_GET_REQUEST_TYPE;
        sig->npu_request.cmd = cmd;
        memcpy(sig->npu_request.pargv,(void *)&stru_debug_channel,sizeof(stru_debug_channel));
        break;
    }

    case NPU_GET_UFE_SYS:
    {
        printf("NPU_GET_UFE_SYS in###\n");
        sig = linx_alloc(linx, sizeof(struct stru_npu_request_sig), NPU_REQUEST_SIG);
        if (sig == NULL)
        {
            printf("linx_alloc() failed\n");
            return ERR_LINX_ALLOC;
        }
        sig->npu_request.cmdType = NPU_GET_REQUEST_TYPE;
        sig->npu_request.cmd = cmd;
        break;
    }

    case NPU_GET_UFE_LINE:
    {
        STRU_GET_UFE_LINE stru_get_ufe_line;
        printf("NPU_GET_UFE in###\n");
        sig = linx_alloc(linx, sizeof(struct stru_npu_request_sig) + sizeof(STRU_GET_UFE_LINE), NPU_REQUEST_SIG);
        if (sig == NULL)
        {
            printf("linx_alloc() failed\n");
            return ERR_LINX_ALLOC;
        }
        sig->npu_request.cmdType = NPU_GET_REQUEST_TYPE;
        sig->npu_request.cmd = cmd;
        u32 lineIndex =(*pargv);

        if(argc == 1)
        {
            stru_get_ufe_line.Lineindex = lineIndex;
            printf("ufe line is %d\n",stru_get_ufe_line.Lineindex);
        }
        else
        {
            printf("input parameters numbers error!\n");
            return 0;
        }

        memcpy(sig->npu_request.pargv,(void *)&stru_get_ufe_line,sizeof(stru_get_ufe_line));
        break;

    }
    case NPU_GET_UFE_PHY:
    case NPU_GET_FLOW_STATICS:
    {
        u32 fsport;
        STRU_GET_UFE stru_get_ufe;
        sig = linx_alloc(linx, sizeof(struct stru_npu_request_sig) + sizeof(STRU_GET_UFE), NPU_REQUEST_SIG);
        if (sig == NULL)
        {
            printf("linx_alloc() failed\n");
            return ERR_LINX_ALLOC;
        }
        sig->npu_request.cmdType = NPU_GET_REQUEST_TYPE;
        sig->npu_request.cmd = cmd;
        if(*(pargv) == 0) /*0: fr MODE 1:IP MODE*/
        {
            if(argc == 5)
            {
                fsport = io_fraddr_pack(*(pargv+1),*(pargv+2),*(pargv+3),*(pargv+4));
                stru_get_ufe.PortNumber = fsport;
                printf("PortNumber is %d\n",stru_get_ufe.PortNumber);
            }
            else
            {
                printf("input parameters numbers error!\n");
                return 0;
            }
        }
        else
        {
            fsport = *(pargv+1);
            if(argc == 2)
            {
                stru_get_ufe.PortNumber = fsport;
                printf("PortNumber is %d\n",stru_get_ufe.PortNumber);
            }
            else
            {
                printf("input parameters numbers error!\n");
                return 0;
            }
        }



        memcpy(sig->npu_request.pargv,(void *)&stru_get_ufe,sizeof(stru_get_ufe));
        break;

    }

    case NPU_DISPLAY_CONFIG_INFO:
    {
        sig = linx_alloc(linx, sizeof(struct stru_npu_request_sig), NPU_REQUEST_SIG);
        if (sig == NULL)
        {
            printf("linx_alloc() failed\n");
            return ERR_LINX_ALLOC;
        }
        sig->npu_request.cmdType = NPU_GET_REQUEST_TYPE;
        sig->npu_request.cmd = cmd;
        break;
    }

    case NPU_GET_CPU_STATICS:
    {
        sig = linx_alloc(linx, sizeof(struct stru_npu_request_sig), NPU_REQUEST_SIG);
        if (sig == NULL)
        {
            printf("linx_alloc() failed\n");
            return ERR_LINX_ALLOC;
        }
        sig->npu_request.cmdType = NPU_GET_REQUEST_TYPE;
        sig->npu_request.cmd = cmd;
        break;
    }
    case NPU_GET_MEMORY_STATICS:
    {
        sig = linx_alloc(linx, sizeof(struct stru_npu_request_sig), NPU_REQUEST_SIG);
        if (sig == NULL)
        {
            printf("linx_alloc() failed\n");
            return ERR_LINX_ALLOC;
        }
        sig->npu_request.cmdType = NPU_GET_REQUEST_TYPE;
        sig->npu_request.cmd = cmd;
        break;
    }

    case NPU_GET_TRUNK_INFO:
    {
        STRU_GET_TRUNKINFO stru_get_trunk_info;
        printf("NPU_GET_TRUNK_INFO in###\n");
        sig = linx_alloc(linx, sizeof(struct stru_npu_request_sig) + sizeof(STRU_GET_TRUNKINFO), NPU_REQUEST_SIG);
        if (sig == NULL)
        {
            printf("linx_alloc() failed\n");
            return ERR_LINX_ALLOC;
        }
        sig->npu_request.cmdType = NPU_GET_REQUEST_TYPE;
        sig->npu_request.cmd = cmd;
        u32 trunkID =(*pargv);

        if(argc == 1)
        {
            stru_get_trunk_info.trunkID = trunkID;
            printf("trunk ID is %d\n",stru_get_trunk_info.trunkID);
        }
        else
        {
            printf("input parameters numbers error!\n");
            return 0;
        }
        memcpy(sig->npu_request.pargv,(void *)&stru_get_trunk_info,sizeof(stru_get_trunk_info));
        break;
    }

    case NPU_GET_DPORT_STATISTIC_INFO:
    {
        STRU_GET_DPORT_STATISTICS stru_get_dport_statistics;
        printf("NPU_GET_TRUNK_INFO in###\n");
        sig = linx_alloc(linx, sizeof(struct stru_npu_request_sig) + sizeof(STRU_GET_DPORT_STATISTICS), NPU_REQUEST_SIG);
        if (sig == NULL)
        {
            printf("linx_alloc() failed\n");
            return ERR_LINX_ALLOC;
        }
        sig->npu_request.cmdType = NPU_GET_REQUEST_TYPE;
        sig->npu_request.cmd = cmd;
        u16 dportID =(*pargv);

        if(argc == 1)
        {
            stru_get_dport_statistics.dportID = dportID;
            printf("dport ID is %d\n",stru_get_dport_statistics.dportID);
        }
        else
        {
            printf("input parameters numbers error!\n");
            return 0;
        }
        memcpy(sig->npu_request.pargv,(void *)&stru_get_dport_statistics,sizeof(stru_get_dport_statistics));
        break;
    }

    case NPU_GET_TABLE_ITEM:
    {
        sig = linx_alloc(linx, sizeof(struct stru_npu_request_sig) + sizeof(STRU_GET_TABLE_ITEM), NPU_REQUEST_SIG);
        if (sig == NULL)
        {
            printf("linx_alloc() failed\n");
            return ERR_LINX_ALLOC;
        }
        sig->npu_request.cmdType = NPU_GET_REQUEST_TYPE;
        sig->npu_request.cmd = cmd;

        memcpy(sig->npu_request.pargv,(void *)pargv,sizeof(STRU_GET_TABLE_ITEM));
        break;
    }

    default:
    {
        printf("unknown cmd,exit!\n");
        return ERR_UNKNOWN_ARG;
    }



	}

	if (linx_send(linx, &sig, server) == -1)
	{
		printf("linx_send() failed");
		return ERR_LINX_SEND;
	}

	printf("linx_send() success###\n");
    return 0;

}

int npu_receive_debug_ack(LINX *linx)
{

    union LINX_SIGNAL *sig;
	u32 cmd,result;
	/*wait receive signal */
	LINX_SIGSELECT sigsel_any[] = { 2,NPU_GET_REQUEST_ACK_SIG,NPU_SET_REQUEST_ACK_SIG };

    //	if (linx_receive(linx, &sig, sigsel_any) == -1)
    if (linx_receive_w_tmo(linx, &sig, NPU_RECEIVE_TIMEOUT,sigsel_any) == -1)
	{
		printf("linx_receive() failed");
		return ERR_LINX_RSV_FAIL;
	}
	if(sig == LINX_NIL)
	{
		printf("Client: received sig timeout.\n");
		return ERR_LINX_RSV_TMOUT;
	}
	else
	{
		switch (sig->sig_no)
		{
        case NPU_GET_REQUEST_ACK_SIG:
        {
            printf("Client: NPU_GET_REQUEST_ACK_SIG received.\n");
            cmd = sig->npu_get_request_ack.cmd;
            result = sig->npu_get_request_ack.result;

            switch(cmd)
            {
            case NPU_GET_ETH_STATICS:
            {
                STRU_GET_ETH_STATICS struEthStatics;
                if(STATUS_OK == result)
                {
                    memcpy((void *)&struEthStatics,sig->npu_get_request_ack.pdata,sizeof(struEthStatics));
                    /*printf_eth_data();*/
                    enet_stats_show(struEthStatics.EthNumber, struEthStatics.stru_eth_statics);
                }
                else
                {
                    printf("GET ETH STATICS failed!\n");
                }
                break;
            }
            case NPU_GET_UPI_STM1_STATICS:
            {
                STRU_GET_UPI_STM1_STATICS struUpiStatics;
                if(STATUS_OK == result)
                {
                    memcpy((void *)&struUpiStatics,sig->npu_get_request_ack.pdata,sizeof(struUpiStatics));
                    upi_stats_show(struUpiStatics.stm1id, struUpiStatics.stru_upi_statics);
                }
                else
                {
                    printf("GET UPI STM1 STATICS failed!\n");
                }
                break;
            }

            case NPU_GET_UPI_TRUNK_STATICS:
            {
                STRU_GET_UPI_TRUNK_STATICS struUpiStatics;
                if(STATUS_OK == result)
                {
                    memcpy((void *)&struUpiStatics,sig->npu_get_request_ack.pdata,sizeof(struUpiStatics));
                    upi_stats_show(struUpiStatics.trunkid, struUpiStatics.stru_upi_statics);
                }
                else
                {
                    printf("GET UPI TRUNK STATICS failed!\n");
                }
                break;
            }
            case NPU_GET_UPI_GROUP_STATICS:
            {
                STRU_GET_UPI_GROUP_STATICS struUpiStatics;
                if(STATUS_OK == result)
                {
                    memcpy((void *)&struUpiStatics,sig->npu_get_request_ack.pdata,sizeof(struUpiStatics));
                    upi_stats_show(struUpiStatics.trunkid, struUpiStatics.stru_upi_statics);
                }
                else
                {
                    printf("GET UPI GROUP STATICS failed!\n");
                }
                break;
            }

            case NPU_GET_RXFR_STATICS:
            {
                STRU_GET_RXFR_STATICS struRxfrStatics;
                if(STATUS_OK == result)
                {
                    memcpy((void *)&struRxfrStatics,(sig->npu_get_request_ack.pdata),sizeof(struRxfrStatics));

                    rxfr_stats_show(struRxfrStatics.vfport, struRxfrStatics.stru_rxfr_statics);
                }
                else
                {
                    printf("GET RXFR STATICS failed!\n");
                }
                break;
            }

            case NPU_GET_TXFR_STATICS:
            {
                STRU_GET_TXFR_STATICS struTxfrStatics;
                if(STATUS_OK == result)
                {
                    memcpy((void *)&struTxfrStatics,(void *)(sig->npu_get_request_ack.pdata),sizeof(struTxfrStatics));
                    txfr_stats_show(struTxfrStatics.vfport, struTxfrStatics.stru_txfr_statics);
                }
                else
                {
                    printf("GET TXFR STATICS failed!\n");
                }
                break;
            }

            case NPU_GET_CPU_STATICS:
            {
                STRU_CPU_STATICS stru_cpu_statics;
                if(STATUS_OK == result)
                {
                    memcpy((void *)&stru_cpu_statics,sig->npu_get_request_ack.pdata,sizeof(stru_cpu_statics));
                    cpu_stats_show(stru_cpu_statics);
                }
                else
                {
                    printf("GET CPU STATICS failed!\n");
                }
                break;
            }

            case NPU_GET_MEMORY_STATICS:
            {
                STRU_MEMORY_STATICS memory_statics;
                if(STATUS_OK == result)
                {
                    memcpy((void *)&memory_statics,sig->npu_get_request_ack.pdata,sizeof(memory_statics));
                    printf("NP App:	PacketMemLeftByte = %d\n", memory_statics.PacketMemLeftByte);
                    printf("NP App:	ParamMemLeftByte = %d\n", memory_statics.ParamMemLeftByte);
                    printf("NP App:	InterMemLeftByte = %d\n", memory_statics.InterMemLeftByte);
                    printf("NP App:	PacketMemTotalByte =%d\n", memory_statics.PacketMemTotalByte);
                    printf("NP App:	ParamMemTotalByte = %d\n", memory_statics.ParamMemTotalByte);
                    printf("NP App:	InterMemTotalByte = %d\n", memory_statics.InterMemTotalByte);

                }
                else
                {
                    printf("GET CPU STATICS failed!\n");
                }
                break;
            }
            default:
            {
                break;
            }

            }
            break;
        }
        case NPU_SET_REQUEST_ACK_SIG:
        {
            result = sig->npu_set_request_ack.result;
            if(result == STATUS_OK)
            {
                printf("set succsss!\n");
            }
            else
            {
                printf("set failed!\n");
            }
            break;
        }
        default:
            break;
		}

		if(linx_free_buf(linx,&sig) == -1)
		{
			printf("linx_free_buf() failed");
		}
	}
	return STATUS_OK;
}


static int find_server(LINX * linx,const char * server_name,LINX_SPID * server)
{

	LINX_SIGSELECT sigsel_hunt[] = { 1,LINX_OS_HUNT_SIG };
	union LINX_SIGNAL *sig;

	if(linx_hunt(linx, server_name, NULL) == -1)
	{
		printf("linx_hunt() failed");
		return ERR_LINX_HUNT;
	}

	if (linx_receive_w_tmo(linx, &sig, NPU_RECEIVE_TIMEOUT, sigsel_hunt) == -1)
	{
		printf("linx_receive_w_tmo() failed");
		return ERR_LINX_RCV_TMOUT;
	}
	if (sig == LINX_NIL)
	{
		printf("Hunting for server on %s timed out.\n"
		       "Check that server is started "
		       "(and optional, that a link is created).\n", server_name);

		return ERR_LINX_HUNT_TMOUT;
	}
	printf("Client: Found server \"%s\".\n", server_name);
	*server = linx_sender(linx, &sig);
	if (*server == LINX_ILLEGAL_SPID)
	{
		printf("linx_sender() failed");
		return ERR_LINX_GET_SENDER;
	}
	/* Free hunt  sig */
	if (linx_free_buf(linx, &sig) == -1)
	{
		printf("linx_free_buf() failed");
		return ERR_LINX_FREE_SIG;
	}
	return STATUS_OK;
}

char workingLinkName[100];
char *workingDir = NULL;

int getpath(char *buf)
{
    long size;
    char *ptr;
    int ret;
    size = pathconf(".", _PC_PATH_MAX);
    if((ptr = (char *)malloc((size_t)size)) != NULL)
    {
        memset(ptr,0,size);
        sprintf(ptr,"/proc/%d/exe",getpid());
    }else return -1;
    ret=readlink(ptr,buf,size);
    free(ptr);
    return ret;
}

int main(int argc, char **argv)
{
    int status = 0;
	u32 cmd;
	u16 funcargv[SPPMAXPARA],u8funcargc = 0;
	u8 i,j=0;
	char client_linx_name[24];
	LINX *linx;
	LINX_SPID server;

    if(getpath(workingLinkName) == -1)
    {
        printf("###can't get current working dir\n");
    }
    else
    {
        printf("###working dir is %s \n",workingLinkName);
    }
    workingDir = dirname(workingLinkName);
    chdir(workingDir);

    while(1)
    {
		if(argc>1)
		{
			if((2 == argc)&&(0 == strcmp("-h",argv[1])))
			{
			    debug_menu_show();
			}
			else if(argc>=2)
			{
    			cmd = atoi(argv[1]);
				u8funcargc = argc - 2;
				for(i = 2;i <argc; i++)
				{
				    funcargv[j++] = atoi(argv[i]);
                    if(j > SPPMAXPARA)
                    {
                        printf("Error: too much argument!\n");
                        return -1;
                    }
				}

                sprintf(client_linx_name,"%s%d", DEBUG_NAME,getpid());
				linx = linx_open(client_linx_name, 0, NULL);
				if (linx == NULL)
				{
					printf("linx_open() failed");
					return ERR_LINX_OPEN;
				}
				printf("spp debug tool version :%s \n",NPU_VERSION_NUM);
				status = find_server(linx, SERVER_NAME,&server);
				if(status != STATUS_OK)
				{
					return ERR_LINX_FIND_SERVER;
				}
				status = npu_send_debug_request(linx, server, cmd, u8funcargc, funcargv);
				if(status != STATUS_OK)
				{
					return ERR_LINX_SEND;
				}

				status = npu_receive_debug_ack(linx);
				if(status != STATUS_OK)
				{
					return ERR_LINX_RSV_FAIL;
				}

				/*close linx */
				if (linx_close(linx) == -1)
				{
					printf("linx_close() failed");
					return ERR_LINX_CLOSE;
				}
			}
		}
		else
		{
			printf("please input parameter!\n");
		}
		return 0;
	}

}
