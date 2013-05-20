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
 * File Name	:	  npumon.c
 * File Mark	 :	  none
 * Description	:	  npu packet capture file
 * Others		 :	  none
 * Version       :    v1.00
 * Author		 :
 * Date 		 :
 n *
 * Modify History		:
 *	   Date              Version         Author           Modification
 *-----------------------------------------------------------------------------
 2011-6-23             v1.00   Richard zhou
 ----------------------------------------------------------------------------*/

#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <limits.h>
#include "spp_npu_link.h"
#include "spp_wddi_common.h"
#include <assert.h>
#include <string.h>
#include <getopt.h>
#include "npumon.h"
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/time.h>
#include <time.h>
#include <stdarg.h>
#include <syslog.h>

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




#define OPT_PCAP_FILE_NAME 256
#define OPT_DLCI_VALUE 257
#define OPT_LEH        258
#define DEFAULT_FILE_NAME "npu_trace"
struct option long_opts[] = {
    {"interface", 1, NULL, 'i'},
    {"fraction", 1, NULL, 'f'},
    {"stm1", 1, NULL, 's'},
    {"port", 1, NULL, 'u'},
    {"command", 1, NULL, 'c'},
    {"pcap", 1, NULL, OPT_PCAP_FILE_NAME},
    {"dlci", 1, NULL, OPT_DLCI_VALUE},
    {"len", 1, NULL, OPT_LEH},
    {"help", 0, NULL, 'h'},
    {NULL, 0, NULL, 0}
};

typedef struct npumon_cmd_tag
{
    char cmdStr[10];
	unsigned int cmd;
}npumon_cmd;

npumon_cmd cmdTable[]={
	{"xm",NPU_MON_CMD_XM},
    {"xl",NPU_MON_CMD_XL},
    {"xru",NPU_MON_CMD_XRU},
    {"ti",NPU_MON_CMD_TI},
    {"ti4",NPU_MON_CMD_TI4},
    {"tr",NPU_MON_CMD_TR},
    {"dd",NPU_MON_CMD_DD},
    {"dc",NPU_MON_CMD_DC},
    {"cd",NPU_MON_CMD_CD},
    {"cc",NPU_MON_CMD_CC}
};

static char *usage =
    "capture packets \n"
    "IP interface: npumon  -i <ip-addr> -u <port> -c <command> [--pcap <filename>] [--len <packetlength>]\n"
    "Frame relay   npumon  -s <STM1-port> -i <E1/T1-port> -c <command> [-f <fraction-number> --dlci <DLCI>] [--pcap <filename>] [--len <packet-length>]\n"
    "OPTIONS:\n"
    "   -c, <command>  xl ru ti tr dd dc \n"
    "Card Status:        xl Link Status    (ex: Connected)\n"
    "Card Status:        ru Display NPU UP time  (ex:   hour(s),   minute(s),    seconds.)\n"
    "Trace Data :        ti Trace and Interpret frames\n"
    "Trace Data :        tr Trace ALL frames, in RAW format\n"
    "Driver Statistics   dd Display Driver Statistics\n"
    "Clear Statistics    dc Clear Driver Statistics\n"

    "   -i <ip-addr> ,ip-addr src ip-addr or dst ipaddr\n"
    "\n"
    "   -u, <port> ,port number , u=1 capture packet on all opened udp port\n"
    "\n"
    "   -s <STM1-port> 1 or 2\n"
    "   -i, <E1/T1-port> number\n"
    "       Specify the E1/T1 port, e.g. 1. 1~63 for E1 1~84 for T1\n"
    "\n"
    "   -f, <fraction-number>\n"
    "       Specify the group number in E1/T1, e.g. 0~31.\n"
    "\n"
    "   --dlci, <DLCI>\n"
    "       Specify the dlci number, e.g. 1 0~1024.\n"
    "\n"
    "   --pcap, <filename>\n"
    "       Specify the capture file's name , example : npu_capture  .\n"
    "   --len <packetlength>\n"
    "       Specify the packet length to be captured.default packet length captured is 300 bytes\n"
    "\n"
    "[press Ctrl+c to exit]"
    "\n";

int g_loopFlag = 1;

/* Add  bitfiled for Frame relay packet */
typedef struct
{
    unsigned int dlci:16;
    unsigned int tsgroup:6;
    unsigned int trunk:7;
    unsigned int card:3;
} fraddr_t;

typedef struct STRU_NPUMON_FR_STRUCT_TAG
{
    u8 EA0:1,
        C_R:1,
        DLCI_MSB:6;
    u8 EA1:1,
        DE:1,
        BECN:1,
        FECN:1,
        DLCI_LSB:4;
}STRU_NPUMON_FR_STRUCT;


unsigned int npu_parser_cmd(npumon_cmd *cmdTable,int elementNum,char *inStr)
{
    int i;
	for(i =0;i < elementNum;i++)
	{
        if( strcmp(cmdTable[i].cmdStr,inStr)== 0)
        {
            return cmdTable[i].cmd;
		}
	}
	return NPUMON_NO_ARGS;
}

u32
io_fraddr_pack(u32 card, u32 trunk, u32 tsgroup, u32 dlci)
{
    fraddr_t a = {0}; //Remember must cleanup memory at first!
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

static uint32_t get_u32(const char *opt, const char *arg)
{
    char *end;
    uint32_t u32;

    u32 = (uint32_t)strtoul(arg, &end, 0);
    if (*end != '\0') {
        printf("npumon: error in %s\n"
               "Try 'npumon --help' for more information\n",
               opt);
        exit(1);
    }
    return u32;
}

FILE *  npu_pcap_file_open(char *filename)
{
    FILE *fp;
    fp = fopen(filename,"a+");
    return fp;
}

int npu_pcap_file_close(FILE * fp)
{
    return (fclose(fp));
}
#define TIMESTRLEN 256
int npu_pcap_file_record(FILE *fp ,char *buf,unsigned int len)
{
    struct tm *p;
    struct timeval tv;
    char timestr[TIMESTRLEN];
    time_t timep;
    time(&timep);
    int i;
    p= localtime(&timep);
    gettimeofday(&tv,NULL);
    snprintf(timestr, sizeof(timestr), "%d:%d:%d.%04ld",p->tm_hour,p->tm_min,p->tm_sec,tv.tv_usec/1000);
    /*  printf("time is %s \n",timestr);*/
    fprintf(fp,"%s \n",timestr);
    fprintf(fp,"%s ","0000");
    for(i = 0;i< len;i++)
    {
        fprintf(fp,"%02x ",(unsigned char)buf[i]);
    }
    fprintf(fp,"\n");
    /*flush file before other user write*/
    fflush(fp);
    return 0;
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

static void
sig_release_handle(int signo)
{
    if (signo != SIGINT && signo != SIGTERM )
    {
        printf("unexpected signal \n");
    }
	g_loopFlag = 0;
	printf("NPU MON stoped!!g_loopFlag = %d\n",g_loopFlag);

}

#define IPADDRLEN 128
#define CMDLEN 64
#define LINXNAMELEN 128
char npumon_linx_name[LINXNAMELEN];

int main(int argc, char *argv[])
{
	int c;
	int status;
	char i_data[IPADDRLEN]={0};
	unsigned int ip_addr;
	int ipaddr_Flag = NPUMON_NO_ARGS;
	int filenameFlag = NPUMON_NO_ARGS;
	STRU_NPUMON_INFO info =
		{
            NPUMON_NO_ARGS,
            NPUMON_NO_ARGS,
            NPUMON_NO_ARGS,
            NPUMON_NO_ARGS,
            NPUMON_NO_ARGS,
            NPUMON_NO_ARGS,
            NPUMON_NO_ARGS,
            NPUMON_NO_ARGS,
            NPUMON_NO_ARGS,
            {0,}};
	LINX *linx;
	LINX_SPID server;
	char cmdInStr[CMDLEN];
	FILE *fp = NULL;
	struct sigaction siginfo;
	sigaction(SIGINT, NULL, &siginfo);
	siginfo.sa_sigaction = (void *) sig_release_handle;
	sigemptyset(&siginfo.sa_mask);
	siginfo.sa_flags = SA_SIGINFO | SA_RESTART;

	if (sigaction(SIGINT, &siginfo, NULL) == -1){
        printf("can't catch SIGINT \n");
	}

	sigaction(SIGTERM, NULL, &siginfo);
	siginfo.sa_sigaction = (void *) sig_release_handle;
	sigemptyset(&siginfo.sa_mask);
	siginfo.sa_flags = SA_SIGINFO | SA_RESTART;

	if (sigaction(SIGTERM, &siginfo, NULL) == -1){
        printf("can't catch SIGINT");
	}

	for (c = 0; c != -1;)
	{

		c = getopt_long(argc, argv, "hi:u:c:s:f:", long_opts, NULL);
		switch (c) {
		case 'h':
            puts("Usage:\n");
            puts(usage);
            return 0;
		case 'i':
            snprintf(i_data,sizeof(i_data), "%s",optarg);
            ipaddr_Flag = 1;
            break;
		case 'u':
            info.portSrc = htons(get_u32("-u",optarg));
            info.ifType = VP_TYPE_UDP;
            break;
		case 'c':
            snprintf(cmdInStr,sizeof(cmdInStr),"%s",optarg);
            info.cmd= npu_parser_cmd(cmdTable,sizeof(cmdTable)/sizeof(npumon_cmd),cmdInStr);
            if(info.cmd == NPUMON_NO_ARGS
               || info.cmd == NPU_MON_CMD_XM
               || info.cmd == NPU_MON_CMD_DC
               || info.cmd == NPU_MON_CMD_CC)
            {
                printf("npumon: error in -c parameters: "
                       "please check it\n");
                return 1;
            }
            break;
		case 's':
            info.stm1= get_u32("-s",optarg);
            info.ifType = VP_TYPE_FR;
            if(info.stm1 >2)
            {
                printf("npumon: error in stm1-number: \n"
                       "should be 0,1\n");
            }
            break;
		case 'f':
            info.group= get_u32("-f",optarg);
            if (info.group > 31) {
                printf("npumon: error in fraction-number: \n"
                       "bigger than 31\n");
                return 1;
            }
            break;

		case OPT_PCAP_FILE_NAME:
            snprintf(info.PcapFileName, sizeof(info.PcapFileName), "%s",optarg);
            filenameFlag = 1;
            break;
		case OPT_DLCI_VALUE:
            info.dlci = get_u32("--dlci", optarg);
            if (info.dlci > 1027) {
                printf("npumon: error in dlci: "
                       "bigger than 1027\n");
                return 1;
            }
            break;
		case OPT_LEH:
            info.len = get_u32("--len", optarg);
            break;
		case '?':
            return 1;
		}
	}
	if(info.ifType == VP_TYPE_UDP)
	{
		if(info.portSrc == NPUMON_NO_ARGS)
		{
		    printf("npumon: missing -u <port>\n"
                   "Try 'npumon -h' for more information\n");
            return 1;
		}
		if(ipaddr_Flag == NPUMON_NO_ARGS)
		{
		    printf("npumon: missing -i <ip-addr>\n"
                   "Try 'npumon -h' for more information\n");
            return 1;
		}
		else
		{   if((ip_addr = inet_addr(i_data)) >0)
			{
				info.ipaddr = ip_addr;
			}
		    else
		    {
			    printf("npumon: error format -i <ip-addr>\n"
                       "Try 'npumon -h' for more information\n");
	            return 1;
		    }
		}
	}
	else if(info.ifType == VP_TYPE_FR)
	{
	    if(info.portSrc != NPUMON_NO_ARGS)
	    {
            printf(" -f -u existed at same time\n");
			return 1;
		}
		printf("npumon: interface is fr \n");
#if 0
        if(info.group== NPUMON_NO_ARGS)
        {
		    printf("npumon: missing -f <fraction-number>\n"
                   "Try 'npumon -h' for more information\n");
            return 1;
		}
#endif
		if(ipaddr_Flag == NPUMON_NO_ARGS)
        {
		    printf("npumon: missing -i <E1/T1-portr>\n"
                   "Try 'npumon -h' for more information\n");
            return 1;
		}
		else
		{
            info.TrunkNumber = atoi(i_data);
		}
	}
	else
	{
		printf("npumon: missing -s <STM1-port> -u <port>\n"
               "Try 'npumon -h' for more information\n");
        return 1;
	}
	if(info.len == NPUMON_NO_ARGS)
	{
        info.len = 300;
	}
    if(filenameFlag == NPUMON_NO_ARGS)
    {
		snprintf(info.PcapFileName, sizeof(info.PcapFileName), "%s",DEFAULT_FILE_NAME);
	}

    char pcaptxtfileName[PCAPFILENAMELEN];
	snprintf(pcaptxtfileName, sizeof(pcaptxtfileName), "%s.txt",info.PcapFileName);
	fp = npu_pcap_file_open(pcaptxtfileName);
	if(fp == NULL)
	{
	    printf("can't open file %s \n",info.PcapFileName);
	    return 1;
	}
	printf("Npumon version %s \n",NPU_VERSION_NUM);
    printf("PcapFileName = %s\n",info.PcapFileName);
	printf("npumon: Capture TYPE is %s",(info.ifType == VP_TYPE_UDP)?"IP UDP":"FR");
	if(info.ifType == VP_TYPE_UDP)
	{
		printf("ipaddr = %s (0x%x) port = %d len = %d \n",i_data,info.ipaddr,info.portSrc,info.len);
	}
	else
	{
		printf("Stm1 = %d TrunkNumber = %d group = %d dlci = %d len =%d \n",info.stm1,info.TrunkNumber,info.group,info.dlci,info.len);
	}
    /*printf("cmd = %d \n",info.cmd);*/
	snprintf(npumon_linx_name, sizeof(npumon_linx_name), "%s%d", NPUMON_NAME,getpid());
	linx = linx_open(npumon_linx_name, 0, NULL);
	if (linx == NULL)
	{
		printf("linx_open() failed");
		return ERR_LINX_OPEN;
	}

	status = find_server(linx, SERVER_NAME,&server);
	if(status != STATUS_OK)
	{
		return ERR_LINX_FIND_SERVER;
	}
    printf("Capture START ![press Ctrl+c to exit]\n");
	status = npu_send_npumon_request(linx, server, NPU_MON_SET_CAPTURE_START,&info);
	if(status != STATUS_OK)
	{
		return ERR_LINX_SEND;
	}
	while(g_loopFlag)/*only crrl+c can set g_loopFlag to 0*/
	{
		(void)npu_receive_npumon_ack(linx,&info,fp);
	}

	printf("NPU MON stoped!!\n");
	npu_pcap_file_close(fp);

	switch(info.cmd)
	{
    case NPU_MON_CMD_TI4:
    case NPU_MON_CMD_TI:
    case NPU_MON_CMD_TR:
    {
        np_save2pcap_file(&info,fp);
        break;
    }
    default:break;
    }
	/*close linx */
	if (linx_close(linx) == -1)
	{
		printf("linx_close() failed");
		return ERR_LINX_CLOSE;
	}
	return 0;
}

int npu_send_npumon_request(LINX *linx, LINX_SPID server, u32 cmd,STRU_NPUMON_INFO *info)
{
	union LINX_SIGNAL *sig;

	switch(cmd)
	{
    case NPU_MON_SET_CAPTURE_START:
    {
        sig = linx_alloc(linx, sizeof(struct stru_npu_request_sig) + sizeof(STRU_NPUMON_INFO), NPU_REQUEST_SIG);
        if (sig == NULL)
        {
            printf("linx_alloc() failed\n");
            return ERR_LINX_ALLOC;
        }
        /*fill info struct*/
        sig->npu_request.cmdType = NPU_SET_REQUEST_TYPE;
        sig->npu_request.cmd = cmd;
        memcpy(sig->npu_request.pargv,(void *)info,sizeof(STRU_NPUMON_INFO));
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
	return 0;
}

int npu_receive_npumon_ack(LINX *linx,STRU_NPUMON_INFO *info,FILE *fp)
{

    union LINX_SIGNAL *sig;
	u32 cmd,result;
	int len,direction;
	/*wait receive signal */
	//LINX_SIGSELECT sigsel_any[] = { 2,NPU_GET_REQUEST_ACK_SIG,NPU_SET_REQUEST_ACK_SIG };
	LINX_SIGSELECT sigsel_any[] ={ 0 };
	if (linx_receive_w_tmo(linx, &sig, 500,sigsel_any) == -1)
	{
		//printf("linx_receive() time out");
		return ERR_LINX_RSV_FAIL;
	}
	if(sig == LINX_NIL)
	{
		//printf("Client: received sig time out.\n");
		return ERR_LINX_RSV_TMOUT;
	}
	else
	{
		switch (sig->sig_no)
		{
        case NPU_GET_REQUEST_ACK_SIG:
        {
            //printf("npumon: NPU_GET_REQUEST_ACK_SIG received.\n");
            cmd = sig->npu_get_request_ack.cmd;
            result = sig->npu_get_request_ack.result;

            switch(cmd)
            {
            case NPU_MON_GET_CAPTURE_FILE:
            {
                switch(info->cmd)
                {
                case NPU_MON_CMD_XM:
                {

                    break;
                }
                case NPU_MON_CMD_XL:
                {
                    len = sig->npu_get_request_ack.pdata[0]<<8 | sig->npu_get_request_ack.pdata[1];
                    if(len != 1)/*maybe send error by linx*/
                    {
                        break;
                    }

                    printf("                         ------------------------------------\n"
                           "                                s%dp%dfr%d: AFT LINK STATUS\n"
                           "                         ------------------------------------\n",info->stm1,info->TrunkNumber,info->group);
                    printf("Device Link Status:     %s\n",(sig->npu_get_request_ack.pdata[3] == 1)?"Connected":"DisConnected");
                    break;
                }
                case NPU_MON_CMD_XRU:
                {
                    len = sig->npu_get_request_ack.pdata[0]<<8 | sig->npu_get_request_ack.pdata[1];
                    if(len != 4)/*maybe send error by linx*/
                    {
                        break;
                    }
                    /*pdata[0] high len,pdata[1] low len pdata[2] direction*/
                    u16 hours = sig->npu_get_request_ack.pdata[3]<<8|sig->npu_get_request_ack.pdata[4];
                    u8	minutes = sig->npu_get_request_ack.pdata[5];
                    u8	seconds = sig->npu_get_request_ack.pdata[6];
                    printf("                         ------------------------------------\n"
                           "                                s%dp%dfr%d: p1fr1: ROUTER UP TIME\n"
                           "                         ------------------------------------\n",info->stm1,info->TrunkNumber,info->group);
                    printf("Router UP Time: %d hour(s), %d minute(s),  %d seconds\n",hours,minutes,seconds);

                    break;
                }
                case NPU_MON_CMD_TI4:
                case NPU_MON_CMD_TI:
                case NPU_MON_CMD_TR:
                {

                    len = sig->npu_get_request_ack.pdata[0]<<8 | sig->npu_get_request_ack.pdata[1];
                    direction = sig->npu_get_request_ack.pdata[2];
                    int i ;
                    if(info->cmd == NPU_MON_CMD_TI)
                    {
                        if(direction)
                        {
                            printf("OUTGOING Len = %d Raw (HEX): ",len);
                        }
                        else
                        {
                            printf("INCOMING Len = %d Raw (HEX): ",len);
                        }

                        for(i = 0;i < len;i++)
                        {
                            if(i%32 == 0)printf("\n");
                            printf("%2.2x ",sig->npu_get_request_ack.pdata[i+3]);

                        }
                        printf("\n");

                    }
                    if(info->cmd == NPU_MON_CMD_TI && info->ifType == VP_TYPE_FR)
                    {
                        STRU_NPUMON_FR_STRUCT *frHd;
                        frHd = (STRU_NPUMON_FR_STRUCT *)&sig->npu_get_request_ack.pdata[3];
                        printf("FR decode       DLCI=%d C/R=%d EA=%d FECN=%d BECN=%d DE=%d EA=%d\n",
                               (frHd->DLCI_MSB<<4 |frHd->DLCI_LSB),frHd->C_R,frHd->EA0,frHd->FECN,frHd->BECN,frHd->DE,frHd->EA1);
                    }
                    else
                    {
                        /*ip do nothing*/
                    }
                    npu_pcap_file_record(fp,(char *)&sig->npu_get_request_ack.pdata[3],len);
                    break;
                }
                case NPU_MON_CMD_DD:
                case NPU_MON_CMD_CD:
                {
                    STRU_GET_UPI_GROUP_STATICS get_upistatics;
                    len = sig->npu_get_request_ack.pdata[0]<<8 | sig->npu_get_request_ack.pdata[1];
                    if(len != sizeof(get_upistatics))/*maybe send error by linx*/
                    {
                        printf("get_device statics len %d is invalid \n",len);
                        break;
                    }
                    printf("                         ------------------------------------\n"
                           "                                s%dp%dfr%d: p1fr1: DEVICE OPERATIONAL STATISTICS \n"
                           "                         ------------------------------------\n",info->stm1,info->TrunkNumber,info->group);
                    memcpy((void *)&get_upistatics,(void *)&sig->npu_get_request_ack.pdata[3],sizeof(get_upistatics));
                    upi_stats_show(get_upistatics.stru_upi_statics);
                    break;
                }
                case NPU_MON_CMD_DC:
                {

                    break;
                }
                case NPU_MON_CMD_CC:
                {

                    break;
                }
                default:
                {
                    printf("Unkonw cmd type %d \n",info->cmd);
                    break;
                }
                }
            }
            break;
            default:
            {
                break;
            }
            }
            break;
        }
        case NPU_SET_REQUEST_ACK_SIG:
        {
            printf("npumon: NPU_SET_REQUEST_ACK_SIG received.\n");
            result = sig->npu_set_request_ack.result;
            if(result == STATUS_OK)
            {
                printf("npumon set succsss!\n");
            }
            else
            {
                if(result == NPU_SET_NPUMON_CMD_ALREAD_START_ERROR)
                {
                    printf("other users is using,try again later!\n");
                    exit(-1);
                }
                printf("npumon set failed! ret = %d \n",result);
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
#define CMDSTRLEN 1024
int np_save2pcap_file(STRU_NPUMON_INFO *info,FILE *fp)
{
	char cmdstr[CMDSTRLEN];
	int ret;
	if(info->ifType == VP_TYPE_FR)
	{
		/*107 is frame relay L1 type*/
		snprintf(cmdstr,sizeof(cmdstr), "text2pcap -q -l 107 -t \"%%H:%%M:%%S.\" %s.txt %s.pcap",info->PcapFileName,info->PcapFileName);

	}
	else
	{
		snprintf(cmdstr,sizeof(cmdstr), "text2pcap -q -t \"%%H:%%M:%%S.\" %s.txt %s.pcap",info->PcapFileName,info->PcapFileName);

	}
	if ((ret = system(cmdstr)) < 0)
    {
        printf("error system command create pcap file %s.pcap\n",info->PcapFileName);
    }
	snprintf(cmdstr, sizeof(cmdstr), "rm -f /var/log/%s.txt",info->PcapFileName);

	if ((ret = system(cmdstr)) < 0)
    {
        printf("error system command clear pcap file %s.pcap\n",info->PcapFileName);
    }
	else
	{
		printf("%s.pcap is generated \n",info->PcapFileName);

	}
	return 0;
}

typedef union StatField
{
    u64 field;
    struct {
        u32 low;
        u32 high;
    } part;
} WPE_StatField;

void upi_stats_show(STRU_UPI_STATICS upi_stats)
{



    printf("----------------------------------------------------\n");

    printf ("rx_frames:                           %08x%08x\n",
            ((WPE_StatField)upi_stats.rx_frames).part.high,
            ((WPE_StatField)upi_stats.rx_frames).part.low);
    printf ("rx_bytes:                            %08x%08x\n",
            ((WPE_StatField)upi_stats.rx_bytes).part.high,
            ((WPE_StatField)upi_stats.rx_bytes).part.low);
    printf ("rx_frames_de_set:                    %08x%08x\n",
            ((WPE_StatField)upi_stats.rx_frames_de_set).part.high,
            ((WPE_StatField)upi_stats.rx_frames_de_set).part.low);
    printf ("rx_bytes_de_set:                     %08x%08x\n",
            ((WPE_StatField)upi_stats.rx_bytes_de_set).part.high,
            ((WPE_StatField)upi_stats.rx_bytes_de_set).part.low);
    printf ("rx_frames_fecn_set:                  %08x%08x\n",
            ((WPE_StatField)upi_stats.rx_frames_fecn_set).part.high,
            ((WPE_StatField)upi_stats.rx_frames_fecn_set).part.low);
    printf ("rx_frames_becn_set:                  %08x%08x\n",
            ((WPE_StatField)upi_stats.rx_frames_becn_set).part.high,
            ((WPE_StatField)upi_stats.rx_frames_becn_set).part.low);
    printf ("rx_discarded_err_crc:                %08x%08x\n",
            ((WPE_StatField)upi_stats.rx_discarded_err_crc).part.high,
            ((WPE_StatField)upi_stats.rx_discarded_err_crc).part.low);
    printf ("rx_discarded_err_alignment:          %08x%08x\n",
            ((WPE_StatField)upi_stats.rx_discarded_err_alignment).part.high,
            ((WPE_StatField)upi_stats.rx_discarded_err_alignment).part.low);
    printf ("rx_discarded_length_violation:       %08x%08x\n",
            ((WPE_StatField)upi_stats.rx_discarded_length_violation).part.high,
            ((WPE_StatField)upi_stats.rx_discarded_length_violation).part.low);
    printf ("rx_discarded_illegal_header:         %08x%08x\n",
            ((WPE_StatField)upi_stats.rx_discarded_illegal_header).part.high,
            ((WPE_StatField)upi_stats.rx_discarded_illegal_header).part.low);
    printf ("rx_discarded_abort:                  %08x%08x\n",
            ((WPE_StatField)upi_stats.rx_discarded_abort).part.high,
            ((WPE_StatField)upi_stats.rx_discarded_abort).part.low);
    printf ("rx_discarded_unsupported_dlci:       %08x%08x\n",
            ((WPE_StatField)upi_stats.rx_discarded_unsupported_dlci).part.high,
            ((WPE_StatField)upi_stats.rx_discarded_unsupported_dlci).part.low);
    printf ("rx_last_unsupported_dlci:            %016x\n",
            upi_stats.rx_last_unsupported_dlci);
    printf ("rx_discarded_fbp_empty:              %08x%08x\n",
            ((WPE_StatField)upi_stats.rx_discarded_fbp_empty).part.high,
            ((WPE_StatField)upi_stats.rx_discarded_fbp_empty).part.low);
    printf ("rx_discarded_hostq_full:             %08x%08x\n",
            ((WPE_StatField)upi_stats.rx_discarded_hostq_full).part.high,
            ((WPE_StatField)upi_stats.rx_discarded_hostq_full).part.low);
    printf ("rx_discarded_err_parity:             %08x%08x\n",
            ((WPE_StatField)upi_stats.rx_discarded_err_parity).part.high,
            ((WPE_StatField)upi_stats.rx_discarded_err_parity).part.low);
    printf ("rx_discarded_err_sop_eop:            %08x%08x\n",
            ((WPE_StatField)upi_stats.rx_discarded_err_sop_eop).part.high,
            ((WPE_StatField)upi_stats.rx_discarded_err_sop_eop).part.low);
    printf ("rx_total_err_bytes:                  %08x%08x\n",
            ((WPE_StatField)upi_stats.rx_total_err_bytes).part.high,
            ((WPE_StatField)upi_stats.rx_total_err_bytes).part.low);
    printf ("tx_frames:                           %08x%08x\n",
            ((WPE_StatField)upi_stats.tx_frames).part.high,
            ((WPE_StatField)upi_stats.tx_frames).part.low);
    printf ("tx_bytes:                            %08x%08x\n",
            ((WPE_StatField)upi_stats.tx_bytes).part.high,
            ((WPE_StatField)upi_stats.tx_bytes).part.low);
    printf ("tx_frames_de_set:                    %08x%08x\n",
            ((WPE_StatField)upi_stats.tx_frames_de_set).part.high,
            ((WPE_StatField)upi_stats.tx_frames_de_set).part.low);
    printf ("tx_bytes_de_set:                     %08x%08x\n",
            ((WPE_StatField)upi_stats.tx_bytes_de_set).part.high,
            ((WPE_StatField)upi_stats.tx_bytes_de_set).part.low);
    printf ("tx_frames_fecn_set:                  %08x%08x\n",
            ((WPE_StatField)upi_stats.tx_frames_fecn_set).part.high,
            ((WPE_StatField)upi_stats.tx_frames_fecn_set).part.low);
    printf ("tx_frames_becn_set:                  %08x%08x\n",
            ((WPE_StatField)upi_stats.tx_frames_becn_set).part.high,
            ((WPE_StatField)upi_stats.tx_frames_becn_set).part.low);
    printf ("tx_underrun:                         %08x%08x\n",
            ((WPE_StatField)upi_stats.tx_underrun).part.high,
            ((WPE_StatField)upi_stats.tx_underrun).part.low);
    printf ("tx_aborted_frames:                   %08x%08x\n",
            ((WPE_StatField)upi_stats.tx_aborted_frames).part.high,
            ((WPE_StatField)upi_stats.tx_aborted_frames).part.low);
    printf ("serial_dps_if_tx_frames:             %08x%08x\n",
            ((WPE_StatField)upi_stats.serial_dps_if_tx_frames).part.high,
            ((WPE_StatField)upi_stats.serial_dps_if_tx_frames).part.low);
    printf ("serial_dps_if_tx_bytes:              %08x%08x\n",
            ((WPE_StatField)upi_stats.serial_dps_if_tx_bytes).part.high,
            ((WPE_StatField)upi_stats.serial_dps_if_tx_bytes).part.low);
    printf ("serial_dps_if_rx_frames:             %08x%08x\n",
            ((WPE_StatField)upi_stats.serial_dps_if_rx_frames).part.high,
            ((WPE_StatField)upi_stats.serial_dps_if_rx_frames).part.low);
    printf ("serial_dps_if_rx_bytes:              %08x%08x\n",
            ((WPE_StatField)upi_stats.serial_dps_if_rx_bytes).part.high,
            ((WPE_StatField)upi_stats.serial_dps_if_rx_bytes).part.low);
    printf ("serial_dps_if_rx_dropped_frames:     %08x%08x\n",
            ((WPE_StatField)upi_stats.serial_dps_if_rx_dropped_frames).part.high,
            ((WPE_StatField)upi_stats.serial_dps_if_rx_dropped_frames).part.low);
    printf ("serial_dps_if_rx_pce_denied_frames:  %08x%08x\n",
            ((WPE_StatField)upi_stats.serial_dps_if_rx_pce_denied_frames).part.high,
            ((WPE_StatField)upi_stats.serial_dps_if_rx_pce_denied_frames).part.low);
    printf("\n");

}
