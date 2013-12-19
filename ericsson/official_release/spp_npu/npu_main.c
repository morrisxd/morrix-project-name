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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <semaphore.h>
#include <sys/select.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <netinet/in.h>
#include "spp_wddi_server.h"
#include "tun_dev.h"
#include "wddi_log.h"
#include <assert.h>
#include "api/wp_wddi.h"
#include "np_common.h"
#include "wpu_buses.h"
#include "xs_ioctl.h"
#include <limits.h>
#include <pthread.h>
#include <libgen.h>
#include <sys/syscall.h>
#include <glob.h>
#define gettid()   syscall(__NR_gettid)
extern int add_dl_rule_and_build();
extern unsigned int dl_rule_node_count;
unsigned int dl_rule_build_waittime = 0;

//#include "wpui_memory.h"
int g_npinitialFlag=NPU_UNINITALIZED;
int g_npnonpoolFlag=NPU_POOL;
int g_npNseAggFlag=NPU_NSE_AGG_OFF;
extern STRU_SET_INITAIL_CONFIG g_npinitial;
extern WP_U32 nri_bit_length;
extern WP_handle iw_rx_host_handle;
#define __LITTLE_ENDIAN_BITFIELD
#define FRAME_MAX_LENGTH 4096
#define ETH_P_8021Q 0x8100
struct iphdr {
#if defined(__LITTLE_ENDIAN_BITFIELD)
	__u8	ihl:4,
		version:4;
#elif defined (__BIG_ENDIAN_BITFIELD)
	__u8	version:4,
  		ihl:4;
#else
#error	"Please fix <asm/byteorder.h>"
#endif
	__u8	tos;
	__be16	tot_len;
	__be16	id;
	__be16	frag_off;
	__u8	ttl;
	__u8	protocol;
	__sum16	check;
	__be32	saddr;
	__be32	daddr;
	/*The options start here. */
};


#define ETH_ALEN	6		/* Octets in one ethernet addr	 */
/*
 *	This is an Ethernet frame header.
 */

struct ethhdr {
	unsigned char	h_dest[ETH_ALEN];	/* destination eth addr	*/
	unsigned char	h_source[ETH_ALEN];	/* source ether addr	*/
	__be16		h_proto;		/* packet type ID field	*/
} ;

struct vlan_ethhdr {
	unsigned char	h_dest[ETH_ALEN];
	unsigned char	h_source[ETH_ALEN];
	__be16		h_vlan_proto;
	__be16		h_vlan_TCI;
	__be16		h_vlan_encapsulated_proto;
};


struct udphdr {
	__be16	source;
	__be16	dest;
	__be16	len;
	__sum16	check;
};

struct Eth_Regs {
    u32 SERDES_MUX_CTRL_0;
    u32 SERDES3_IO_CTRL_0;
    u32 SERDES3_IO_CTRL_1;
    u32 SERDES3_IO_CTRL_2;
    u32 SERDES3_IO_CTRL_3;
    u32 SERDES3_IO_CTRL_4;
    u32 SERDES3_IO_STAT;
    u32 SERDES_INTERRUPT_1;
    u32 SERDES_INTERRUPT_1_MASK;
    u32 SERDES_RX_MUX_CONTROL_0;
    u32 SERDES_RX_MUX_CONTROL_1;
    u32 WN2_WNC1;
    u32 WN2_WNC2;
    u32 WN3_WNC1;
    u32 WN3_WNC2;
    u32 WN2_TCNTR_0;
    u32 WN2_TCNTR_1;
    u32 WN2_TSTAT_0;
    u32 WN2_TSTAT_1;
    u32 WN3_TCNTR_0;
    u32 WN3_TCNTR_1;
    u32 WN3_TSTAT_0;
    u32 WN3_TSTAT_1;
    u32 WN2_TADIR_0;
    u32 WN2_TADIR_1;
    u32 WN2_TANDBG_0;
    u32 WN2_TANDBG_1;
    u32 WN3_TANDBG_0;
    u32 WN3_TANDBG_1;
    u32 WN2_IFC_0;
    u32 WN3_IFC_0;
};

u32 read_reg(u32 addr)
{
    u32 ret;
    ret = *((volatile unsigned int *)(unsigned long)(addr + WPL_RIF_VIRTUAL(0, 0)));
    ret = htonl(ret);
    return ret;
}

void display_eth_regs(struct Eth_Regs* pregs)
{
    WDDI_INFO("SERDES_MUX_CTRL_0:0x%08x\n",pregs->SERDES_MUX_CTRL_0);
    WDDI_INFO("SERDES3_IO_CTRL_0:0x%08x\n",pregs->SERDES3_IO_CTRL_0);
    WDDI_INFO("SERDES3_IO_CTRL_1:0x%08x\n",pregs->SERDES3_IO_CTRL_1);
    WDDI_INFO("SERDES3_IO_CTRL_2:0x%08x\n",pregs->SERDES3_IO_CTRL_2);
    WDDI_INFO("SERDES3_IO_CTRL_3:0x%08x\n",pregs->SERDES3_IO_CTRL_3);
    WDDI_INFO("SERDES3_IO_CTRL_4:0x%08x\n",pregs->SERDES3_IO_CTRL_4);
    WDDI_INFO("SERDES3_IO_STAT:0x%08x\n",pregs->SERDES3_IO_STAT);
    WDDI_INFO("SERDES_INTERRUPT_1:0x%08x\n",pregs->SERDES_INTERRUPT_1);
    WDDI_INFO("SERDES_INTERRUPT_1_MASK:0x%08x\n",pregs->SERDES_INTERRUPT_1_MASK);
    WDDI_INFO("SERDES_RX_MUX_CONTROL_0:0x%08x\n",pregs->SERDES_RX_MUX_CONTROL_0);
    WDDI_INFO("SERDES_RX_MUX_CONTROL_1:0x%08x\n",pregs->SERDES_RX_MUX_CONTROL_1);
    WDDI_INFO("WN2_WNC1:0x%08x\n",pregs->WN2_WNC1);
    WDDI_INFO("WN2_WNC2:0x%08x\n",pregs->WN2_WNC2);
    WDDI_INFO("WN3_WNC1:0x%08x\n",pregs->WN3_WNC1);
    WDDI_INFO("WN3_WNC2:0x%08x\n",pregs->WN3_WNC2);
    WDDI_INFO("WN2_TCNTR_0:0x%08x\n",pregs->WN2_TCNTR_0);
    WDDI_INFO("WN2_TCNTR_1:0x%08x\n",pregs->WN2_TCNTR_1);
    WDDI_INFO("WN2_TSTAT_0:0x%08x\n",pregs->WN2_TSTAT_0);
    WDDI_INFO("WN2_TSTAT_1:0x%08x\n",pregs->WN2_TSTAT_1);
    WDDI_INFO("WN3_TCNTR_0:0x%08x\n",pregs->WN3_TCNTR_0);
    WDDI_INFO("WN3_TCNTR_1:0x%08x\n",pregs->WN3_TCNTR_1);
    WDDI_INFO("WN3_TSTAT_0:0x%08x\n",pregs->WN3_TSTAT_0);
    WDDI_INFO("WN3_TSTAT_1:0x%08x\n",pregs->WN3_TSTAT_1);
    WDDI_INFO("WN2_TADIR_0:0x%08x\n",pregs->WN2_TADIR_0);
    WDDI_INFO("WN2_TADIR_1:0x%08x\n",pregs->WN2_TADIR_1);
    WDDI_INFO("WN2_TANDBG_0:0x%08x\n",pregs->WN2_TANDBG_0);
    WDDI_INFO("WN2_TANDBG_1:0x%08x\n",pregs->WN2_TANDBG_1);
    WDDI_INFO("WN3_TANDBG_0:0x%08x\n",pregs->WN3_TANDBG_0);
    WDDI_INFO("WN3_TANDBG_1:0x%08x\n",pregs->WN3_TANDBG_1);
    WDDI_INFO("WN2_IFC_0:0x%08x\n",pregs->WN2_IFC_0);
    WDDI_INFO("WN3_IFC_0:0x%08x\n",pregs->WN3_IFC_0);
}

void dump_eth_regs(struct Eth_Regs* pregs)
{
    pregs->SERDES_MUX_CTRL_0 = read_reg(0x1e008280);
    pregs->SERDES3_IO_CTRL_0 = read_reg(0x1e0082b8);
    pregs->SERDES3_IO_CTRL_1 = read_reg(0x1e0082bc);
    pregs->SERDES3_IO_CTRL_2 = read_reg(0x1e0082c0);
    pregs->SERDES3_IO_CTRL_3 = read_reg(0x1e0082c4);
    pregs->SERDES3_IO_CTRL_4 = read_reg(0x1e0082c8);
    pregs->SERDES3_IO_STAT = read_reg(0x1e0082cc);
    pregs->SERDES_INTERRUPT_1 = read_reg(0x1e0082ec);
    pregs->SERDES_INTERRUPT_1_MASK = read_reg(0x1e0082f4);
    pregs->SERDES_RX_MUX_CONTROL_0 = read_reg(0x1e008300);
    pregs->SERDES_RX_MUX_CONTROL_1 = read_reg(0x1e008304);
    pregs->WN2_WNC1 = read_reg(0x1e016000);
    pregs->WN2_WNC2 = read_reg(0x1e016004);
    pregs->WN3_WNC1 = read_reg(0x1e018000);
    pregs->WN3_WNC2 = read_reg(0x1e018004);
    pregs->WN2_TCNTR_0 = read_reg(0x1e016200);
    pregs->WN2_TCNTR_1 = read_reg(0x1e016240);
    pregs->WN2_TSTAT_0 = read_reg(0x1e016204);
    pregs->WN2_TSTAT_1 = read_reg(0x1e016244);
    pregs->WN3_TCNTR_0 = read_reg(0x1e018200);
    pregs->WN3_TCNTR_1 = read_reg(0x1e018240);
    pregs->WN3_TSTAT_0 = read_reg(0x1e018204);
    pregs->WN3_TSTAT_1 = read_reg(0x1e018244);
    pregs->WN2_TADIR_0 = read_reg(0x1e016234);
    pregs->WN2_TADIR_1 = read_reg(0x1e016274);
    pregs->WN2_TANDBG_0 = read_reg(0x1e01623c);
    pregs->WN2_TANDBG_1 = read_reg(0x1e01627c);
    pregs->WN3_TANDBG_0 = read_reg(0x1e01823c);
    pregs->WN3_TANDBG_1 = read_reg(0x1e01827c);
    pregs->WN2_IFC_0 = read_reg(0x1e0160e0);
    pregs->WN3_IFC_0 = read_reg(0x1e0180e0);
}

struct Eth_Regs g_eth_regs_up,g_eth_regs_down,g_eth_regs_last,g_eth_regs_tmp;

int pipefd[2]={0};
int pipefd_RxFrHprio[2]={0};
int pipefdRxEth[2]={0};
int pipefdRxEthHprio[2]={0};
static int pipeAlarm[2]={0};
int xsreset(void);
int getmodulebyname(const char *name);


void npu_handle_heart_beat(int heartfd)
{
    char rxchar;

    read(heartfd,&rxchar,  1);
	if(g_logLevel ==0 )
	{
		Fprintf("child receive keep alive signal %c\n", rxchar);
	}
    write(heartfd, &rxchar, 1);
}

struct timeval tv;
long max = 0, min = LONG_MAX;
long clk;
long numrsv=0;
float ack = 0;

long get_time(struct timeval *tv)
{
	/* NOTE: This code does not handle wrapping of the timer. */

	long sDiff = -tv->tv_sec;
	long uDiff = -tv->tv_usec;

	if (gettimeofday(tv, 0))
		Fprintf("gettimeofday() failed.");

	sDiff += tv->tv_sec;
	uDiff += tv->tv_usec;

	return sDiff * 1000000 + uDiff;
}

void
restart_heartbeat()
{
    char hb_cmd[255];
    //sprintf(hb_cmd,"sudo /etc/init.d/heartbeat restart &");
    sprintf(hb_cmd,"bash -c \". /opt/spp/active/functions.sh >> /dev/null; _heart_beat_restart_ > /dev/null 2>&1\"");
    system(hb_cmd);
}

void np_handle_ethernet_frame(char * buffer, int lenth, int tapfd)
{
    int size;
    struct ethhdr * ethheader;
    struct vlan_ethhdr * vlan_ethheader;
    struct iphdr *ipheader;
    struct udphdr *udpheader;


    vlan_ethheader = (struct vlan_ethhdr  *)buffer;
    ethheader = (struct ethhdr  *)buffer;

    size = lenth;

    if(!(((ntohs(vlan_ethheader->h_vlan_proto) == ETH_P_8021Q) && (ntohs(vlan_ethheader->h_vlan_encapsulated_proto) == 0x0800))
         ||
         (ntohs(ethheader->h_proto) == 0x0800)))
    {
        size = tap_write(tapfd, buffer, size);
        if(size < 0)
        {
            WDDI_WARNING("write tun0 %s", strerror(errno));
        }
        else
            WDDI_PRINT("write none IPV4 packet %d bytes\n", size);
    }
    else
    {
        if((ntohs(vlan_ethheader->h_vlan_proto) == ETH_P_8021Q) && (ntohs(vlan_ethheader->h_vlan_encapsulated_proto) == 0x0800))
        {
            //assert (size >  sizeof(struct vlan_ethhdr) + sizeof( struct iphdr ) + sizeof(struct udphdr));
            ipheader = ( struct iphdr * )(vlan_ethheader+1);
            size -= 18;
        }
        else if(ntohs(ethheader->h_proto) == 0x0800)
        {
            //assert (size >  sizeof(struct ethhdr) + sizeof( struct iphdr ) + sizeof(struct udphdr));
            ipheader = ( struct iphdr * )(ethheader+1);
            size -=14;
        }
        else
        {
            WDDI_WARNING("never reach here.\n");
        }

        WDDI_PRINT("receive IPV4 packet %d bytes\n", lenth);
        if(0 != npu_udp_cal_chksum((unsigned char *)ipheader, (ipheader->ihl <<2)))
        {
            WDDI_WARNING("receive IPV4 packet with wrong checksum \n");
            return ;
		}

		/**************ip fragement check,if fraged wite to tap************************
                     check ip fragements
		*******************************************************************************/

        udpheader = (struct udphdr  *) ((char *)ipheader + (ipheader->ihl <<2));
        /*******************************************************/
        if((ipheader->protocol  ==  IPPROTO_UDP)  && (  NULL != npu_get_vport_info(udpheader->dest)) )
        {
            //TODO just hander over payload to uplayer
            NPU_HOST_RSV_PACKET_INFO st_packet_info;

            //st_packet_info.ipaddr_src = ipheader->saddr;
            // st_packet_info.length = size - (ipheader->ihl <<2) - 8;
            st_packet_info.length = ntohs(udpheader->len) -8 ;/*get udp data length directly,remove udp 8 byte header len*/
            st_packet_info.port = udpheader->dest;
            st_packet_info.ipaddr_dst = ipheader->daddr;
            st_packet_info.ipaddr_src = ipheader->saddr;
            st_packet_info.port_src = udpheader->source;
            st_packet_info.pdata = (u8 *)(udpheader+1);
            st_packet_info.vp_type = VP_TYPE_UDP;
            WDDI_PRINT("dispach udp packet to client port = %d port_src = %d ipaddr_dst =0x%x ipaddr_src = 0x%x length = %d \n",htons(st_packet_info.port),htons(st_packet_info.port_src),st_packet_info.ipaddr_dst,st_packet_info.ipaddr_src,st_packet_info.length);
            npu_dispatch_packet_to_client(&st_packet_info);
			/*capture all data from this vport ,no matter dispatch success or not*/
			npu_mon_capture_packet(buffer,lenth,udpheader->dest,0);
        }
        else
        {
            size = tap_write(tapfd, buffer, lenth);
            if(size < 0)
            {
                WDDI_WARNING("write tun0 %s", strerror(errno));
            }
            else
                WDDI_INFO("Info: np_handle_ethernet_frame, write none udp IPV4 packet %d bytes\n", size);
        }
    }

}
void np_handle_fr_frame(char * buffer, int lenth, u32 vfport)
{
	NPU_HOST_RSV_PACKET_INFO st_packet_info;
	st_packet_info.ipaddr_src = 0;
	st_packet_info.length = lenth-2;/*remove the fr header*/
	/* vfport infomation*/
	st_packet_info.port = vfport;
	st_packet_info.ipaddr_src = 0;
	st_packet_info.port_src = 0;
	st_packet_info.pdata = (u8 *)buffer + 2;/*rm fr 2byte header*/
	st_packet_info.vp_type = VP_TYPE_FR;

	u32 card;
	u32 trunk;
	u32 group;
	u32 dlci;
	np_fraddr_unpack(st_packet_info.port,&card,&trunk,&group,&dlci);
	WDDI_PRINT("dispach FR packet to client port = %d : S%dP%dFR%dD%d \n",htonl(st_packet_info.port),card,trunk,group,dlci);
	npu_dispatch_packet_to_client(&st_packet_info);
	/*capture all data from this vport ,no matter dispatch success or not*/
	WP_FR_HEADER_2OCT_SET(buffer,dlci,0,0,0,0);
	npu_mon_capture_packet(buffer,lenth,vfport,0);
}

void np_handle_fr_priority_frame(char * buffer, int lenth)
{
    u32 vfport;
	int status;
	u16 flowIndex = htons(*(u16 *)buffer);
	//Fprintf("np_handle_fr_priority_frame flowIndex = 0x%x\n",flowIndex);
	status = WP_WDDI_ResFitwGetTag(flowIndex,&vfport);
	if(status)
	{
        Fprintf("np_handle_fr_priority_frame can't get tag, flowIndex = 0x%x\n",flowIndex);
		return ;
	}
	STRU_VPORT_INFO  *VportInfo = npu_get_vport_info(vfport);
	NPU_HOST_RSV_PACKET_INFO st_packet_info;
	if(VportInfo !=  NULL)
	{
		st_packet_info.ipaddr_src = 0;
		st_packet_info.length = lenth-2;/*remove the fr header*/
		/* vfport infomation*/
		st_packet_info.port = vfport;
		st_packet_info.ipaddr_src = 0;
		st_packet_info.port_src = 0;
		st_packet_info.pdata = (u8 *)buffer + 2;/*rm fr 2byte header*/
		st_packet_info.vp_type = VP_TYPE_FR;

		u32 card;
		u32 trunk;
		u32 group;
		u32 dlci;
		np_fraddr_unpack(st_packet_info.port,&card,&trunk,&group,&dlci);
		WDDI_PRINT("dispach FR packet to client port = %d : S%dP%dFR%dD%d \n",htonl(st_packet_info.port),card,trunk,group,dlci);
		npu_dispatch_packet_to_client(&st_packet_info);
		/*capture all data from this vport ,no matter dispatch success or not*/
		npu_mon_capture_packet(buffer,lenth,vfport,0);

	}
}

static void
sig_release_wddi_driver(int signo)
{
    if (signo != SIGINT && signo != SIGTERM )
        WDDI_INFO("unexpected signal");
    Fprintf("release_wddi_driver \n");
    tap_socket_close();
    if(g_npinitialFlag == NPU_INITALIZED)
    {
        //WUFE_UfeRelease(0); /*maybe will cause interrupt segment fault ,because of the mailbox thread are reading interrupt */
    }
    remove_tapcfg();
    //np_ufe_release();
    //WP_DriverRelease();

    WPX_PortLedSet(0, WPX_PORT_LED_OFF, 0);
    WPX_PortLedSet(0, WPX_PORT_LED_OFF, 1);
    WPX_PortLedSet(0, WPX_PORT_LED_OFF, 2);
    WPX_PortLedSet(0, WPX_PORT_LED_OFF, 3);
    exit(0);
}


static void
sig_alarm_wddi_event(int signo)
{
    int ret;
    if (signo != SIGALRM)
    {
        WDDI_INFO("unexpected signal");
        return;
    }
    /*Fprintf("Receive alarm event\n");*/

    ret =write(pipeAlarm[1], (void *)&signo, 4);
    if(ret < 0)
    {
        WDDI_INFO("write pipe error");
    }

    alarm(1);
    return;
}

#define	MAP_WN1_TSTAT_0	 0xbe016204
#define	MAP_WN1_TSTAT_1	 0xbe016244
#define	MAP_WN2_TSTAT_0	 0xbe018204
#define MAP_WN1_TANDBG1  0xbe01627C
#define MAP_WN2_TANDBG0  0xbe01823c



int eth_link_status(int port)
{
    unsigned int tstat, tmp, reg_val;
    int status;
    tstat = 0;

    switch(port) {
    case 0 :
        tstat = MAP_WN1_TSTAT_0;
        break;
    case 1 :
        tstat = MAP_WN2_TSTAT_0;
        break;
    case 2 :
        tstat = MAP_WN1_TSTAT_1;
        break;
    default :
        tstat = MAP_WN1_TSTAT_0;
    }

    tstat &= 0x1fffffff;

    tmp = *(volatile unsigned int *)(unsigned long)(tstat + WPL_RIF_VIRTUAL(0, 0));
    status = ntohl(tmp);
    if(status & 0x4)
	{
		status = LINK_STATUS_UP;
    } else
    {
		status = LINK_STATUS_DOWN;
    }
    return status;
}
char xmlFileLocation[100];
char workingLinkName[100];
char *workingDir = NULL;

extern NP_SYSTEM_INFO np_system;

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
int getAppnameByPID(char *buf,pid_t pid)
{
    long size;
    char *ptr;
    int ret;
    char workingLink[100]={"\0"};
    char *appname = NULL;
    size = pathconf(".", _PC_PATH_MAX);
    if((ptr = (char *)malloc((size_t)size)) != NULL)
    {
        memset(ptr,0,size);
        sprintf(ptr,"/proc/%d/exe",pid);
    }else return -1;
    ret=readlink(ptr,workingLink,size);
    free(ptr);
    appname = basename(workingLink);
    if(appname != NULL)
    {
        strcpy(buf,appname);
    }
    else
    {
        Fprintf("invalid application name %s \n",workingLink);
    }
    return ret;
}

int getSystemResultBycmd(const char *cmd)
{
    char buf[256];
    FILE *fp = tmpfile();

    assert(cmd);

    /* The buf save a shell command that take pid to tmpfile */
    //sprintf(buf, "ps -A | awk '/.*%s.*/{print $1}' >&%d",
    sprintf(buf, "%s >&%d",
            cmd, fileno(fp));
    /* Run the command */
    if (-1 == system(buf))
    {
        perror("system ERROR");
        //exit(1);
        fclose(fp);
        return -1;
    }

    /* Get pid from tmpfile */
    fseek(fp, 0, SEEK_SET);
    if (NULL == fgets(buf, sizeof buf, fp))
    {
        //perror("fgets-ERROR");
        Fprintf("getSystemResultBycmd fgets is NULL \n");
        fclose(fp);
        return -1;
    }
    fclose(fp);
    Fprintf("getSystemResultBycmd result is %s \n",buf);
    return atoi(buf);
}


pthread_t rx_pid;
int np_waittime = NPU_WAIT_TIME;

int configuration_files_exist(char* directory)
{
    glob_t globs;
    int ret=-1;
    int flags = 0;
    char tmp_file[FILENAME_MAX];
    if ( directory != NULL )
    {
        sprintf(tmp_file,"%s/*.xml",directory);
        ret = glob(tmp_file,flags,NULL,&globs);
        globfree(&globs);
        if(ret==0) return 1;
    }
    return 0;
}

#include <sys/stat.h>
#define LOCKFILE "/var/run/mydaemon.pid"
#define LOCKMODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

/* set advisory lock on file */
int lockfile(int fd)
{
    struct flock fl;
    fl.l_type = F_WRLCK;  /* write lock */
    fl.l_start = 0;
    fl.l_whence = SEEK_SET;
    fl.l_len = 0;  //lock the whole file
    return(fcntl(fd, F_SETLK, &fl));
}

int already_running(const char *filename)
{
    int fd;
    char buf[256];

    fd = open(filename, O_RDWR | O_CREAT, LOCKMODE);
    if (fd < 0) {
        Fprintf("-----------can't open %s\n", filename);
        return 1;
    }

    if (lockfile(fd) == -1) {

        if (errno == EACCES || errno == EAGAIN) {
            Fprintf("-----------file: %s already locked\n", filename);
            close(fd);
            return 1;
        }
        Fprintf("-----------can't lock %s\n", filename);
        return 1;
    }

    ftruncate(fd, 0);
    memset(buf,0,sizeof(buf));
    sprintf(buf, "%ld", (long)getpid());
    write(fd, buf, strlen(buf) + 1);
    Fprintf("----------first time to lock the file\n");
    return 0;
}

/*
 * this will call np_init()
 *	- wddi_log_open ()
 *	- configuration_files_exist
 *	- getmodulebyname
 *	- xsreset ()
 *     - npu_init_vport_info();
 *     - npu_init_uplink_info();
 *     - npu_init_downlink_info();
 *     - npu_init_arp_info();
 *     - npu_init_update_bvci_info();
 *     - npu_init_trunk_info();
 *     - npu_init_fr_ul_link_table_info();
 *     - npu_init_fr_dl_link_table_info();
 *     - npu_init_peer_vport_info();
 *	- fcntl
 *	- pipe
 *	- np_control_interrupt
 *	---------------------------------------- np_init
 *	- WDDI_INFO
 *	- restart_heartbeat
 *	- linx_init
 *	- FD_ZERO
 *	- sigemptyset
 *	- sigaction
 *	- pthread_create
 *	- getSystemResultBycmd
 *	- WP_NseAgg_set
 *	- select
 *	- np_eth_host_send
 *	- npu_handle_linx_msg
 *	- read ()
 *	- WPU_BUSES
 *	- npu_printf_debugReg
 *	- add_dl_rule_and_build
 *	- npu_handle_heart_beat
 *     - remove_tapcfg();
 *     - tap_socket_close();
 *     - WP_DriverRelease();
 */
int main(int argc, char **argv)
{
    int nfds;
    int size=0;
    int lenth;
    int  ret;
    int flag;
    int opt;
    char *buffer;
    int tapfd=0;
    int linxfd=0;
    int status = 0;
    int heartfd =0;
    fd_set rdset;
    char loglever[16];
    int intpoolfeature = 1;
    WP_tag tag = 0;
    int serverfd;
    FILE * fp;
    struct sigaction siginfo;

    if(already_running(LOCKFILE)) return 1;

#if ENEA_RELEASE
    wddi_log_open("spp_npu");
#endif

    strcpy(loglever, "LOG_WARNING");
    strcpy(xmlFileLocation, "/etc/spp/active");
    while((opt = getopt(argc, argv, "n:p:l:"))!= -1) {

        switch(opt){
        case 'p':
            heartfd = atoi(optarg);
            Fprintf("The heart beat fd: %d\n",heartfd);
            break;
        case 'n':
            strcpy(loglever, optarg);
            if(strcmp(loglever,"LOG_DEBUG")== 0)
            {
                g_logLevel = 0;
            }
            Fprintf("The wddi server log lever: %s\n",loglever);
            break;
        case 'l':
            strcpy(xmlFileLocation, optarg);
            Fprintf("The wddi server configure file location: %s\n",xmlFileLocation);
         	break;
        default:
            Fprintf("Unknown option matter: %s\n",argv[1]);
            //return -1;
            break;
        }
    }

    if(!configuration_files_exist(xmlFileLocation))
    {
        Fprintf("Error: there is no *.xml file under %s, exit!\n",xmlFileLocation);
        return -1;
    }

    //wddi_log_set_mask(loglever);
    memset(&g_npinitial,0,sizeof(STRU_SET_INITAIL_CONFIG));
    memset(&np_system,0,sizeof(NP_SYSTEM_INFO));
    npu_set_initial_cfg_info(xmlFileLocation,&g_npinitial);


    if(getpath(workingLinkName) == -1)
    {
        Fprintf("can't get current working dir\n");
    }
    else
    {
        Fprintf("working dir is %s \n",workingLinkName);
    }
    workingDir = dirname(workingLinkName);
    chdir(workingDir);
    //sprintf(xmlFileLocation,"%s/wp_mmap_drv.ko",workingDir);
    if((serverfd = access ("wp_mmap_drv.ko",F_OK)) != 0)
    {
        Fprintf("wp_mmap_drv.ko doesn't exist or has no execute privilige.\n");
        return (errno);
    }

    if((serverfd = access ("linx.ko",F_OK)) != 0)
    {
        Fprintf("linx.ko doesn't exist or has no execute privilige.\n");
        return (errno);
    }
    /*reset board*/
    ret = getmodulebyname("wp_mmap_drv");
    if(ret==0)
    {
        Fprintf("wp_mmap_drv.ko exists, will be unloaded now!\n");
        ret = system("rmmod wp_mmap_drv.ko");
        if(ret <0)
        {
            perror("rmmod wp_mmap_drv.ko");
            exit(1);
        }

    }
    else
    {
        Fprintf("wp_mmap_drv.ko does not exist!\n");
    }
    Fprintf("now start to load wp_mmap_drv.ko!\n");
    ret = system("./load.sh");
    if(ret <0 )
    {
        perror("insmod wp_mmap_drv.ko");
        exit(1);
    }

    ret = getmodulebyname("linx");
    if(ret!=0)
    {
        Fprintf("linx.ko is not existed,will be loaded now!\n");
        ret = system("sudo insmod linx.ko linx_max_sockets_per_link=4096 linx_max_spids=4096 linx_max_attrefs=4096 linx_max_tmorefs=4096");
        if(ret <0 )
        {
            perror("insmod linx.ko ");
            exit(1);
        }
    }

    //fprintf(stderr,"-----------------Error type - %s,%d\n", WP_error_name[WP_ERROR(0xfc000138)],0xfc000138);

    Fprintf("starting xsreset ,please wait ...,pid=%d\n",getpid());
    if((ret=xsreset())!= 0)
    {
        Fprintf("xsreset ret=%d\n",ret);
        exit(1);
    }
    Fprintf("xsreset success \n");
    tap_hwadd_read(g_npinitial.ifMACsrc);
    npu_init_vport_info();
    npu_init_uplink_info();
    npu_init_downlink_info();
    npu_init_arp_info();
    npu_init_update_bvci_info();
    npu_init_trunk_info();
    npu_init_fr_ul_link_table_info();
    npu_init_fr_dl_link_table_info();
    npu_init_peer_vport_info();
    if(pipe(pipefd) < 0)
    {
        WDDI_ERR("create pipe");
    }
    status = fcntl(pipefd[0], F_SETFL, O_NONBLOCK);
    if (status < 0)
    {
        WDDI_ERR("pipe0 fcntl fail");
        return 0;
    }

    status = fcntl(pipefd[1], F_SETFL, O_NONBLOCK);
    if (status < 0)
    {
        WDDI_ERR("pipe1 fcntl fail");
        return 0;
    }

    if(pipe(pipefd_RxFrHprio) < 0)
    {
        WDDI_ERR("create pipe failed");
    }
    status = fcntl(pipefd_RxFrHprio[0], F_SETFL, O_NONBLOCK);
    if (status < 0)
    {
        WDDI_ERR("pipe0 fcntl fail");
        return 0;
    }

    status = fcntl(pipefd_RxFrHprio[1], F_SETFL, O_NONBLOCK);
    if (status < 0)
    {
        WDDI_ERR("pipe1 fcntl fail");
        return 0;
    }

    /*for RX ethernet*/
    if(pipe(pipefdRxEth) < 0)
    {
        WDDI_ERR("create pipe");
    }
    status = fcntl(pipefdRxEth[0], F_SETFL, O_NONBLOCK);
    if (status < 0)
    {
        WDDI_ERR("pipefdRxEth0 fcntl fail");
        return 0;
    }
    status = fcntl(pipefdRxEth[1], F_SETFL, O_NONBLOCK);
    if (status < 0)
    {
        WDDI_ERR("pipefdRxEth1 fcntl fail");
        return 0;
    }
    /*for RX ethernetHigh priority*/
    if(pipe(pipefdRxEthHprio) < 0)
    {
        WDDI_ERR("create high pipe");
    }
    status = fcntl(pipefdRxEthHprio[0], F_SETFL, O_NONBLOCK);
    if (status < 0)
    {
        WDDI_ERR("pipefdRxEth high fcntl fail");
        return 0;
    }
    status = fcntl(pipefdRxEthHprio[1], F_SETFL, O_NONBLOCK);
    if (status < 0)
    {
        WDDI_ERR("pipefdRxEth high fcntl fail");
        return 0;
    }
    if(pipe(pipeAlarm) < 0)
    {
        WDDI_ERR("create pipe");
    }
    status = fcntl(pipeAlarm[0], F_SETFL, O_NONBLOCK);
    if (status < 0)
    {
        WDDI_ERR("pipeAlarm0 fcntl fail");
        return 0;
    }
    status = fcntl(pipeAlarm[1], F_SETFL, O_NONBLOCK);
    if (status < 0)
    {
        WDDI_ERR("pipeAlarm0 fcntl fail");
        return 0;
    }
    Fprintf("NPU version : %s\n",NPU_VERSION_NUM/*"V1.0.3"*/);
    WDDI_INFO("waitting for np initial...");


    //np_inital_set_sturct(&g_npinitial);
    if(g_npinitial.NRI_bitLen >0 && g_npinitial.NRI_bitLen<= 8)
    {
        nri_bit_length = g_npinitial.NRI_bitLen;
    }
    np_control_interrupt(0,0);
    np_control_interrupt(0,1);
    if(g_npinitialFlag == NPU_UNINITALIZED)
    {
        np_init();
    }
#if 0
    int loopnum = 100000;
    unsigned int *valbuf;

    valbuf = malloc(loopnum);

    get_time(&tv); /*record time */

    memcpy(valbuf, 0xe8baf000, loopnum);
    clk = get_time(&tv);/*read time */

    Fprintf("memory read time = %dus\n",clk);

    return 0;
#endif
    WDDI_INFO("np initialized");

    tap_socket_open();
    WDDI_INFO("open tap socket");

    tapfd = tap_init();
    WDDI_INFO("initialize tap driver");


    status = fcntl(tapfd, F_SETFL, O_NONBLOCK);
    if (status < 0)
    {
    	WDDI_ERR("tap_init fcntl fail");
    }
#if ENEA_RELEASE
	restart_heartbeat();
#endif

	linxfd = linx_init();
    Fprintf("initialize linx ok\n");
    //nfds = pipefd[0] > tapfd ? pipefd[0]: tapfd;
    nfds = tapfd;
    nfds = pipeAlarm[0] > nfds ?  pipeAlarm[0]: nfds;
    //linxfd = linx_init();
    //WDDI_INFO("initialize linx");

    nfds = linxfd > nfds ? linxfd: nfds;
    if(heartfd !=0 )
    {
		nfds = heartfd > nfds ? heartfd: nfds;
	}

    FD_ZERO(&rdset);
    buffer = malloc(FRAME_MAX_LENGTH);
    if(buffer == 0)
    {
        WDDI_ERR("malloc buffer");
    }
#if 0
#define WPL_SIGNAL_RT_SER 35 /*SIGRTMIN*/
#define WPL_SIGNAL_RT_OV (WPL_SIGNAL_RT_SER + 1)
#define WPL_SIGNAL_RT_BUSERR (WPL_SIGNAL_RT_SER + 2)
    sigset_t sigmask;
    sigemptyset(&sigmask);
    sigaddset(&sigmask,WPL_SIGNAL_RT_OV);
    sigprocmask(SIG_BLOCK, &sigmask, NULL);
#endif
    sigaction(SIGINT, NULL, &siginfo);
    siginfo.sa_sigaction = (void *) sig_release_wddi_driver;
    sigemptyset(&siginfo.sa_mask);
    siginfo.sa_flags = SA_SIGINFO | SA_RESTART;

    if (sigaction(SIGINT, &siginfo, NULL) == -1){
        WDDI_ERR("can't catch SIGINT");
    }

    sigaction(SIGTERM, NULL, &siginfo);
    siginfo.sa_sigaction = (void *) sig_release_wddi_driver;
    sigemptyset(&siginfo.sa_mask);
    siginfo.sa_flags = SA_SIGINFO | SA_RESTART;

    if (sigaction(SIGTERM, &siginfo, NULL) == -1){
        WDDI_ERR("can't catch SIGINT");
    }


    sigaction(SIGALRM, NULL, &siginfo);
    siginfo.sa_sigaction = (void *) sig_alarm_wddi_event;
    sigemptyset(&siginfo.sa_mask);
    siginfo.sa_flags = SA_SIGINFO | SA_RESTART;

    if (sigaction(SIGALRM, &siginfo, NULL) == -1){
        WDDI_ERR("can't catch SIGALRM");
    }
    alarm(1);//give a shot
    /*create a thread to handle rx packet*/
    int err;
    int ParamFr[3]={pipefd[0], pipefd_RxFrHprio[0], tapfd};
    err = pthread_create(&rx_pid,NULL,(void *)handle_rx_fr_packet,ParamFr);
    if(err!= 0)
    {
        WDDI_ERR("pthread_create handle_rx_fr_packet error");
    }
    /*for RX Ethernet*/
    int ParamEth[3]={pipefdRxEth[0],pipefdRxEthHprio[0],tapfd};
    err = pthread_create(&rx_pid,NULL,(void *)handle_rx_eth_packet,ParamEth);
    if(err!= 0)
    {
        WDDI_ERR("pthread_create handle_rx_eth_packet error");
    }

    //np_control_interrupt(1,0);
    //np_control_interrupt(1,1);
    /*Get the POOL feature status */
#if 0
    fp=popen("/usr/local/bin/spp feature show_value name SGSN_Pool_Proxy_GSM","r");
    //fp=popen("/usr/src/stwa/bin/test.sh","r");
	//fgets(poolfeature,sizeof(poolfeature),fp);
    //intpoolfeature = atoi(poolfeature);
    int rdret = 0;
	if(fp == NULL)
	{
		WDDI_WARNING("read name SGSN_Pool_Proxy_GSM fail fp is NULL \n");
	}
	else
	{
		rdret = fscanf(fp,"%d",&intpoolfeature);
		if(rdret <= 0)
		{
			WDDI_WARNING("read name SGSN_Pool_Proxy_GSM fail rdret = %d \n",rdret);
		}
	    pclose(fp);
	}
	Fprintf("intpoolfeature = %d \n",intpoolfeature);
#endif
    int rdret = 0;

	/*rdret = getSystemResultBycmd("/usr/local/bin/spp feature show_value name SGSN_Pool_Proxy_GSM");
      if(rdret == -1)
      {
      WDDI_WARNING("getSystemResultBycmd read name SGSN_Pool_Proxy_GSM fail rdret = %d \n",rdret);
      }
      else
      {
      intpoolfeature = rdret;
      }
      Fprintf("getSystemResultBycmd intpoolfeature = %d \n",intpoolfeature);
      if(intpoolfeature == 0)
      {
      intpoolfeature = NPU_NONPOOL;
      }
      else
      {
      intpoolfeature = NPU_POOL;
      }
      Fprintf("Get the POOL feature status is: %d\n",intpoolfeature);
      g_npnonpoolFlag = intpoolfeature;
    */

	rdret = getSystemResultBycmd("/usr/local/bin/spp feature show_value name NSE_Aggregation");
	if(rdret == -1)
	{
		WDDI_WARNING("getSystemResultBycmd read name NSE_Aggregation fail rdret = %d \n",rdret);
	}
    else
    {
    	Fprintf("getSystemResultBycmd NSE_Aggregation = %d \n",rdret);
        g_npNseAggFlag = rdret;
        WP_NseAgg_set(g_npNseAggFlag);
    }

    while (1) {
        FD_ZERO(&rdset);
        //FD_SET(pipefd[0], &rdset);
        FD_SET(pipeAlarm[0], &rdset);
        FD_SET(tapfd, &rdset);
        FD_SET(linxfd, &rdset);
        if(heartfd !=0 )
            FD_SET(heartfd, &rdset);

    again:
        ret = select(nfds+1, &rdset, NULL, NULL, NULL);
        if(ret < 0)
        {
            /*WDDI_PRINT("server select ret = %d errno = %s \n",ret,strerror(errno));*/
            goto again;
        }
        if(FD_ISSET(tapfd, &rdset))
        {       int loopNum = 20;
            while(loopNum--)
            {
                size = tap_read(tapfd, buffer, FRAME_MAX_LENGTH);
                if(size < 0)break;
                WDDI_PRINT("read %d bytes\n", size);
                if(np_waittime == NPU_WAIT_FINISHED)
                {
                    np_eth_host_send(size,buffer);
                }
            }
        }


        if(FD_ISSET(linxfd, &rdset))
        {
            npu_handle_linx_msg();
        }

        if(FD_ISSET(pipeAlarm[0], &rdset))
        {
            ret =read(pipeAlarm[0], &tag, 4);
            if(ret < 0)
                WDDI_WARNING("read pipe %s", strerror(errno));

            np_poll_report_alarm();
            int i;
            WPU_buses_info buses;
            WPU_BUSES(0, 0, WPU_BUSES_CMD_ERROR_REPORT, &buses);
            for(i = 0; i < WPU_SIU_TYPE_LAST; i++)
                if((buses.bus_error_report[i].is_error) &&
                   (buses.bus_error_report[i].bus_error_source == WPU_BUS_ERR_SOURCE_WMM))
                {
                    //    strcpy(argv[0], "trs");
                    //     sprintf(argv[1], "%d", buses.bus_error_report[i].trs);
                    //    WPU_TRSDump(2, argv);
                    //fflush(stdout);
                    //WPI_Display(0,0,WP_DISPLAY_POOL);
                    fflush(stdout);
                    char strconfig[200];

                    sprintf(strconfig,"%s/winutil wfc list 0 14",workingDir);

                    Fprintf("%s\n",strconfig);
                    if ((status = system(strconfig)) < 0)
                        WDDI_ERR("systemwinutil tb init");
                    usleep(1000);
                    fflush(stdout);
                    //usleep(1000);
                    npu_printf_debugReg();
#if 0
                    while(1){
                        sleep(5);
                        WDDI_WARNING("NPU bus error ,suspending.....");
                    }
#endif
                    WDDI_ERR("NPU bus error, wddi server will be reboot");
                }
		    /*flash to /var/log/spp_wddi.log*/
            fflush(stdout);
            if(np_waittime != NPU_WAIT_FINISHED)
	        {
				if(np_waittime != 0)
				{
					np_waittime--;
				}
				else
				{
					np_control_interrupt(1,0);
					np_control_interrupt(1,1);
					np_waittime = NPU_WAIT_FINISHED;
				}

			}

            if (dl_rule_node_count > 0)
            {
                dl_rule_build_waittime++;
				if (dl_rule_build_waittime > 1)
				{
                    add_dl_rule_and_build();
                    dl_rule_build_waittime = 0;
				}
            }
        }

        if(heartfd != 0)
        {
            if(FD_ISSET(heartfd, &rdset))
            {
                npu_handle_heart_beat(heartfd);
            }
        }
		//np_set_over_interrupt();


    }

    /*it seems no useful.*/

    remove_tapcfg();
    tap_socket_close();
    WP_DriverRelease();

    return 0;

}

u8 E1_status[SPP_MAX_STM1_PORT_NUM][SPP_MAX_TRUNK_NUM] = {{LINK_STATUS_DOWN},};
u8 E1_status_old[SPP_MAX_STM1_PORT_NUM][SPP_MAX_TRUNK_NUM] = {{LINK_STATUS_DOWN},};
u8 STM1_status[SPP_MAX_STM1_PORT_NUM] = {LINK_STATUS_DOWN,};
u8 STM1_status_old[SPP_MAX_STM1_PORT_NUM] = {LINK_STATUS_DOWN,};
u8 ETH_status[2] = {LINK_STATUS_DOWN,};
u8 ETH_status_old[2] = {LINK_STATUS_DOWN,};

u8 STM1_status_firsttime[SPP_MAX_STM1_PORT_NUM] = {0,};

u8 STM1_status_flag[SPP_MAX_STM1_PORT_NUM] = {LINK_STATUS_DOWN,};

u8 E1_status_firsttime[SPP_MAX_STM1_PORT_NUM][SPP_MAX_TRUNK_NUM] = {{0},};
u8 ETH_status_firsttime[2] = {0,};
extern STRU_STM1_ALARM_INFO np_ufe4_ararm_info;

int np_poll_report_alarm(void)
{
    u32 i,j;
	u16 statusBits;
	int ret;
	char strAlarm[240]= {0};
    STRU_SET_INITAIL_CONFIG *initial_cfg = npu_get_initial_cfg_info();
	if(initial_cfg->OpterateMode == NPU_MODE_FR)
	{
		for(i = 0; i < SPP_MAX_STM1_PORT_NUM; i ++)
		{
			if(initial_cfg->stru_stm1_cfg_info[i].stm1Valid)
			{
				for(j = 0; j < SPP_MAX_TRUNK_NUM; j ++)
				{
					if(initial_cfg->stru_stm1_cfg_info[i].trunk_info[j].trunkValid)
					{
                        E1_status[i][j] = np_check_ufe4_e1t1_status(i,j);
                        if(E1_status[i][j] != E1_status_old[i][j] ||(E1_status_firsttime[i][j]==0))
                        {
                            E1_status_firsttime[i][j] =1;
                            if(E1_status[i][j] == LINK_STATUS_UP)
                            {
                                STM1_status_flag[i] = LINK_STATUS_UP;
                            }
                            E1_status_old[i][j]=E1_status[i][j];
                            Fprintf("The status of stm1 %d E1/T1 %d is %s \n",i+1,j+1,(E1_status[i][j] == LINK_STATUS_UP)?"UP":"DOWN");/*for debug*/
                            /*status changed ,should generate alarm mesg*/
#if ENEA_RELEASE
                            sprintf(strAlarm,"O&M event=%s stm1=%d e1t1=%d",(E1_status[i][j] == LINK_STATUS_UP)? "E1T1_LOS_ON":"E1T1_LOS_OFF",i+1,j+1);
                            WDDI_WARNING(strAlarm);
#endif
                        }
					}
				}
				STM1_status[i] = np_check_ufe4_stm1_status(i);
                if(STM1_status[i] != STM1_status_old[i] || STM1_status_firsttime[i]==0)
                {
                    if(STM1_status_firsttime[i] == 0)
                    {
                        STM1_status_firsttime[i]=1;
                        //STM1_status[i] = STM1_status_flag[i];
                        if(STM1_status[i]==LINK_STATUS_DOWN)
                        {
                            np_ufe4_ararm_info.SDH_alarm[i].SDH_OOF= 1;
                        }
                    }
                    STM1_status_old[i]=STM1_status[i];
                    Fprintf("The status of stm1 %d is %s \n",i+1,(STM1_status_old[i] == LINK_STATUS_UP)?"UP":"DOWN");/*for debug*/
                    /*status changed ,should generate alarm mesg*/
#if ENEA_RELEASE
                    sprintf(strAlarm,"O&M event=%s stm1=%d",(STM1_status[i] == LINK_STATUS_UP)? "STM1_LOS_ON":"STM1_LOS_OFF",i+1);
                    WDDI_WARNING(strAlarm);
#endif
					/*control LED */
					/*
					  SFP PORT			ETH PORT		  STM-1 PORT
					  ----------------------------------------------
					  0 				-				  0
					  1 				2				  -  ENET4(master)
					  2 				1				  -  ENET5(slave)
					  3 				0				  1
					*/
					/*STM1 port LED*/
					if(STM1_status[i] == LINK_STATUS_UP)
					{
						WPX_PortLedSet(0, WPX_PORT_LED_ON, i*3);
					}
					else
					{
						WPX_PortLedSet(0, WPX_PORT_LED_OFF, i*3);
					}
                }

			}
		}

	}

	for(i = 0; i< 2; i ++)/*eth port0 for traffic ,port1 for slave-port*/
	{
		dump_eth_regs(&g_eth_regs_tmp);
		ETH_status[i] = eth_link_status(2-i);
		if(ETH_status_old[i] != ETH_status[i] || ETH_status_firsttime[i]==0)
		{
		    ETH_status_firsttime[i] = 1;
			ETH_status_old[i] = ETH_status[i];
			Fprintf("The status of eth port %d is %s \n",i,(ETH_status[i] == LINK_STATUS_UP)?"UP":"DOWN");/*for debug*/
			/*status changed ,should generate alarm mesg*/
#if ENEA_RELEASE
			sprintf(strAlarm,"O&M event=%s ethport=%d",(ETH_status[i] == LINK_STATUS_UP)? "ETH_LINK_UP":"ETH_LINK_DOWN",i);
            WDDI_WARNING(strAlarm);
#endif

			/*control LED */
			/*
			  SFP PORT			ETH PORT		  STM-1 PORT
			  ----------------------------------------------
			  0 				-				  0
			  1 				2				  -
			  2 				1				  -
			  3 				0				  1
			*/
			/*ETH port LED*/
			if(ETH_status[i] == LINK_STATUS_UP)
			{
			    WPX_PortLedSet(0, WPX_PORT_LED_ON, i+1);

			    dump_eth_regs(&g_eth_regs_up);
			    memcpy(&g_eth_regs_last,&g_eth_regs_up,sizeof(g_eth_regs_up));
			}
			else
			{
			    WPX_PortLedSet(0, WPX_PORT_LED_OFF, i+1);

			    dump_eth_regs(&g_eth_regs_down);
			    WDDI_INFO("-------------------------WPL_RIF_VIRTUAL(0, 0)=0x%x\n",WPL_RIF_VIRTUAL(0, 0));
			    WDDI_INFO("-------------------------the last up eth regs:\n");
			    display_eth_regs(&g_eth_regs_up);
			    WDDI_INFO("-------------------------the previous eth regs:\n");
			    display_eth_regs(&g_eth_regs_last);
			    WDDI_INFO("-----------i=%d--------------the down eth regs:\n",i);
			    display_eth_regs(&g_eth_regs_down);

				unsigned int tmp;
				unsigned int ctrl_reg;
			    if(i == 0) /*sfp2*/
				{
                    tmp = *(volatile unsigned int *)(unsigned long)(0x1e01627C + WPL_RIF_VIRTUAL(0, 0));
                    tmp = ntohl(tmp);
                    ctrl_reg = *(volatile unsigned int *)(unsigned long)(0x1e0082c4 + WPL_RIF_VIRTUAL(0, 0));
                    ctrl_reg = ntohl(ctrl_reg);
                    *(volatile unsigned int *)(unsigned long)(0x1e0082c4 + WPL_RIF_VIRTUAL(0, 0)) = htonl(0x01618000);
                    usleep(10);
                    *(volatile unsigned int *)(unsigned long)(0x1e0082c4 + WPL_RIF_VIRTUAL(0, 0)) = htonl(0x05618000);

				}
                else    /*sfp3*/
				{
                    memcpy(&g_eth_regs_last,&g_eth_regs_tmp,sizeof(g_eth_regs_tmp));

                    tmp = *(volatile unsigned int *)(unsigned long)(0x1e01823c + WPL_RIF_VIRTUAL(0, 0));
                    tmp = ntohl(tmp);
                    ctrl_reg = *(volatile unsigned int *)(unsigned long)(0x1e0082c0 + WPL_RIF_VIRTUAL(0, 0));
                    ctrl_reg = ntohl(ctrl_reg);
                    *(volatile unsigned int *)(unsigned long)(0x1e0082c0 + WPL_RIF_VIRTUAL(0, 0)) = htonl(0x01618000);
                    usleep(10);
                    *(volatile unsigned int *)(unsigned long)(0x1e0082c0 + WPL_RIF_VIRTUAL(0, 0)) = htonl(0x05618000);
				}

                Fprintf("eth port %d has been reset! TANDBG=0x%x IO_CTRL1=0x%x\n",i,tmp,ctrl_reg);
			}
		}
	}
	return 0;
}
#ifndef O_NOFOLLOW
#define O_NOFOLLOW     00400000        /* don't follow links */
#endif

void my_exit(int status, void *arg)
{
    Fprintf("before exit(), close fd=%d\n",(int)arg);
    close((int)arg);
}

int xsreset(void)
{
    int fd;

    fd = open("/dev/wp_bank24", O_RDWR | O_NOFOLLOW, 0644);
    if(fd == -1) {
        perror("open");
		return -1;
        //exit(1);
    }

    Fprintf("open fd=%d\n",fd);
    on_exit(my_exit, (void *)fd);

    if(ioctl(fd, XS_IOCTL_RESET, NULL) != 0) {
        //close(fd);
        Fprintf("xsreset failed\n");
		return -1;
        //exit(1);
    }
    //close(fd);
    Fprintf("xsreset is ok\n");
    return 0;
}

int getmodulebyname(const char *name)
{
    char buf[128];
    FILE *fp = tmpfile();

    assert(name);

    /* The buf save a shell command that take pid to tmpfile */
    sprintf(buf, "lsmod | awk '/.*%s.*/{print $1}' >&%d",
            name, fileno(fp));
    /* Run the command */
    if (-1 == system(buf))
    {
        perror("system ERROR");
        exit(1);
        return -1;
    }

    /* Get pid from tmpfile */
    fseek(fp, 0, SEEK_SET);
    if (NULL == fgets(buf, sizeof buf, fp))
    {
        //perror("fgets-ERROR");
        return -1;
    }
    fclose(fp);
    return 0;
}

void handle_rx_fr_packet(int *Param)
{
	fd_set rdset;
	int nfds;
	int rxpipefd = Param[0];
	int rxpipefd_hprio = Param[1];
	int tapfd = Param[2];
    nfds = rxpipefd;
	int ret;
	int status = 0;
	WP_tag tag = 0;
    int lenth;
	char *buffer;
	int rsvNumber = 0;
	buffer = malloc(FRAME_MAX_LENGTH);
	NP_SYSTEM_INFO * the_system = np_get_system_info();
#if 0//NPU_HOST_OLP
    unsigned long tmp;
    tmp = (unsigned long)buffer + 128;
    tmp = (unsigned long)buffer & ~127;
    buffer = (unsigned long *)tmp;
#endif
    if(buffer == 0)
    {
        WDDI_ERR("malloc buffer");
    }

    if (nfds < rxpipefd_hprio)
    {
        nfds = rxpipefd_hprio;
    }

	int linx_fd = linx_init_thread_rx_fr();
	status = fcntl(linx_fd, F_SETFL, O_NONBLOCK);
	if (status < 0)
	{
        WDDI_ERR("linxfd fcntl fail");
        return 0;
	}
	Fprintf("rx_fr_packet threadId is %u \n",gettid());
    while (1)
    {
        FD_ZERO(&rdset);
        FD_SET(rxpipefd, &rdset);
        FD_SET(rxpipefd_hprio, &rdset);
	again:
        ret = select(nfds+1, &rdset, NULL, NULL, NULL);
        if(ret < 0)
        {
            WDDI_PRINT("handle_rx_fr_packet select ret = %d errno = %s \n",ret,strerror(errno));
            goto again;
        }

		if(FD_ISSET(rxpipefd_hprio, &rdset))
		{
		    rsvNumber=0;
		    while(rsvNumber < WP_FR_MAX_RSV_HIPRIO_NUMBER)
		    {
                ret = read(rxpipefd_hprio, &tag, 4);
                if (ret < 0)
                {
                    break;
                }
				if(the_system->Channel_FR_PrioRx0_host_handle!= 0)
				{
					WDDI_INFO("np_fr_host_priority_receive");
					status = np_fr_host_receive(the_system->Channel_FR_PrioRx0_host_handle,&lenth,buffer);
					if(status == 0)
					{
						np_handle_fr_priority_frame(buffer,lenth);
						rsvNumber++;
					}
					else
					{
						break;
					}
				}
				else
				{
					WDDI_WARNING("rx channel is not valid channelindex = %d\n ",tag);
				}
			}
		}
	    if(FD_ISSET(rxpipefd, &rdset))
	    {
	    	rsvNumber = 0;
			while(rsvNumber < WP_FR_MAX_RSV_LOWPRIO_NUMBER)
		    {
                ret = read(rxpipefd, &tag, 4);
                if (ret < 0)
                {
                    break;
                }
				FR_CHANNEL_INFO * ch_info;
				u32 vfport = 0;
				STRU_VPORT_INFO  *VportInfo = npu_get_vport_info(tag);
				if(VportInfo !=  NULL)
				{
					ch_info = &VportInfo->ch_info;
					if(ch_info->rx_host_handle != 0)
					{
						vfport = ch_info->vfport;
                        WDDI_INFO("Info: handle_rx_fr_packet::np_fr_host_receive on vfport:0x%x\n", vfport);
                        status = np_fr_host_receive(ch_info->rx_host_handle,&lenth,buffer);
                        if(status == 0)
                        {
                            np_handle_fr_frame(buffer,lenth,vfport);
                            rsvNumber++;
                        }
                        else
                        {
                            break;
                        }
					}
					else
					{
						WDDI_WARNING("rx channel is not valid channelindex = %d\n ",tag);
					}
				}
				else
				{
                    WDDI_WARNING(" can't get fr vport info vport = %d \n ",tag);
				}

            }
	    }
    }
}
int pipeReadEventNum =0;
extern int pipeWriteEventNum;
void handle_rx_eth_packet(int *Param)
{
	fd_set rdset;
	int nfds;
	int rxpipefd = Param[0];
    int rxpipefd_hprio = Param[1];
	int tapfd = Param[2];
    nfds = rxpipefd;
	int ret;
	int status = 0;
	WP_tag tag = 0;
    int lenth;
	char *buffer;
	int rsvNumber = 0;
	buffer = malloc(FRAME_MAX_LENGTH);
	NP_SYSTEM_INFO * the_system = np_get_system_info();
#if 0//NPU_HOST_OLP
    unsigned long tmp;
    tmp = (unsigned long)buffer + 128;
    tmp = (unsigned long)buffer & ~127;
    buffer = (unsigned long *)tmp;
#endif
    if(buffer == 0)
    {
        WDDI_ERR("malloc buffer");
    }
    if (nfds < rxpipefd_hprio)
    {
        nfds = rxpipefd_hprio;
    }
	int linx_fd = linx_init_thread_rx_eth();
	status = fcntl(linx_fd, F_SETFL, O_NONBLOCK);
	if (status < 0)
	{
        WDDI_ERR("linxfd fcntl fail");
        return 0;
	}
	Fprintf("rx_eth_packet threadId is %u ,main pid = %d\n",gettid(),getpid());
    while (1)
    {
        FD_ZERO(&rdset);
        FD_SET(rxpipefd, &rdset);
		FD_SET(rxpipefd_hprio, &rdset);
	again:
        ret = select(nfds+1, &rdset, NULL, NULL, NULL);
        if(ret < 0)
        {
            WDDI_PRINT("handle_rx_eth_packet select ret = %d errno = %s \n",ret,strerror(errno));
            goto again;
        }
        if(FD_ISSET(rxpipefd_hprio, &rdset))
        {
            rsvNumber = 0;
            while(rsvNumber <WP_ETH_MAX_RSV_HIPRIO_NUMBER)
            {
                ret =read(rxpipefd_hprio, &tag, 4);
                if(ret < 0)
                {
                    break;
                }
                pipeReadEventNum++;
                status = np_eth_host_receive(the_system->Channel_IP_PrioRx0_host_handle,&lenth,buffer);
                if(status == 0)
                {
                    WDDI_PRINT("np_eth_host_priority_receive length = %d\n",lenth);
                    np_handle_ethernet_frame(buffer, lenth, tapfd);
                    rsvNumber++;
                }
                else
                {
                    break;
                }
            }
        }

	    if(FD_ISSET(rxpipefd, &rdset))
	    {
		    rsvNumber = 0;
		    while(rsvNumber < WP_ETH_MAX_RSV_LOWPRIO_NUMBER)
		    {
				ret =read(rxpipefd, &tag, 4);
				if(ret < 0 )
				{
				    break;
				}
				pipeReadEventNum++;
				//printf("WriteEventNum = %d ReadEventNum =%d diff= %d\n",pipeWriteEventNum,pipeReadEventNum,pipeWriteEventNum-pipeReadEventNum);
				status = np_eth_host_receive(iw_rx_host_handle,&lenth,buffer);
				if(status == 0)
				{
					WDDI_PRINT("np_eth_host_receive length = %d\n",lenth);
					np_handle_ethernet_frame(buffer, lenth, tapfd);
					rsvNumber++;
				}
				else
				{
					break;
				}
			}
        }
    }
}
