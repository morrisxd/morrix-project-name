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
#define _GNU_SOURCE

#include <stdio.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <linx.h>
#include <sys/time.h>
#include <time.h>

#include "spp_wddi_server.h"
#include "spp_wddi_common.h"
#include <search.h>  //hash table API
#include "wddi_log.h"
#include "np_common.h"
//#include <tree.h>
#include <arpa/inet.h>

#define MAX_PORTS    3600*2    /* # of port_src in search table. */
#define MAX_FLOWS    65535*2 /*FLOWS in search table*/
#define MAX_ARPS  400 /*ARPS in search table*/
#define MAX_BVCIS  65535*2 /*BVCIS in search table*/
#define MAX_TRUNKS  336*2 /*TRUNKS in search table*/

#define LINX_CLIENT_PRINT(a, b, fmt, arg...) do {                       \
        char *clientName=NULL;                                          \
        if(0 == linx_get_name(a,b,&clientName))                         \
        {                                                               \
            pid_t clientPid = 0;                                        \
            char clientAppname[100];                                    \
            if(sscanf(clientName,"%*[a-zA-Z_]%d",&clientPid) == 0)      \
            {                                                           \
                WDDI_WARNING("client: %s client =0x%x",clientName,b);   \
            }                                                           \
            else                                                        \
            {                                                           \
                if(-1 == getAppnameByPID(clientAppname,clientPid))      \
                {                                                       \
                    WDDI_WARNING("getAppnameByPID failed ,clientPid = %d",clientPid); \
                }                                                       \
                WDDI_WARNING("client: %s client =0x%x clientAppname = %s",clientName,b,clientAppname); \
            }                                                           \
            WDDI_WARNING(fmt,##arg);                                    \
            linx_free_name(a,&clientName);                              \
        }                                                               \
    }while(0)                                                           \

FILE *debug_log_file = NULL;
struct dl_rule_node
{
    STRU_SET_DOWNLINK_PATH_RULE rule;
    struct dl_rule_node * next;
};

struct dl_rule_node * dl_rule_list = NULL;
unsigned int dl_rule_node_count = 0;

char port_ptr[MAX_PORTS];
u16 g_Vport_no;
//STRU_VPORT_INFO  g_VportInfo[MAX_PORTS];
STRU_LINX_SERVER_INFO g_LinxSeverInfo={NULL,UNUSED};
STRU_LINX_SERVER_INFO g_LinxSeverRxFrthreadInfo={NULL,UNUSED};
STRU_LINX_SERVER_INFO g_LinxSeverRxEththreadInfo={NULL,UNUSED};

STRU_SET_INITAIL_CONFIG g_npinitial = {0};
STRU_SET_INITAIL_CONFIG g_np_reconfig = {0};
extern WP_U32 nri_bit_length;

struct hsearch_data *ht;
struct hsearch_data *uplink_ht;
struct hsearch_data *downlink_ht;
struct hsearch_data *arp_ht;
struct hsearch_data *update_bvci_ht;
struct hsearch_data *trunk_ht;
struct hsearch_data *fr_ul_link_table_ht;
struct hsearch_data *fr_dl_link_table_ht;
struct hsearch_data *peer_vport_ht;


struct NODE* vport_nodehead=NULL;
struct NODE* uplink_nodehead=NULL;
struct NODE* downlink_nodehead=NULL;
struct NODE* arp_nodehead=NULL;
struct NODE* update_bvci_nodehead=NULL;
struct NODE* trunk_nodehead=NULL;
struct NODE* fr_ul_link_table_nodehead=NULL;
struct NODE* fr_dl_link_table_nodehead=NULL;

struct NODE* fr_dl_bvci_nodehead[MAX_FLOWS]={NULL};
struct NODE* fr_dl_dport_nodehead[MAX_FLOWS]={NULL};
u32 npu_set_initial_cfg_info(char *filename, STRU_SET_INITAIL_CONFIG *np_configure_ptr);
extern 	char xmlFileLocation[100];
extern int np_waittime;
int g_logLevel =2;/*AppRxEvent can't use log*/

extern WP_handle iw_sys_bridging;
unsigned int total_count = 0;
int add_dl_rule_and_build()
{
    WP_S32 status;
    int flag = 0;
    int ret;
    struct dl_rule_node * p;
    struct dl_rule_node * temp;
    int count = 0;
    p = dl_rule_list;
    while(p != NULL)
    {
        count++;
        ret = np_dl_route_table_add(&(p->rule));
        if (ret > 0)
        {
            // this flag indicate we have to build the rules
            flag = 1;
        }
        total_count++;
        temp = p;
        p = p->next;
        free(temp);
    }

    WDDI_WARNING("add downlink rule and build count, count %d, dl_rule_node_count %d, total =%d\n", count, dl_rule_node_count, total_count);

    dl_rule_node_count = 0;
    dl_rule_list = NULL;

    // build anyway
    if (flag)
    {
        status = WP_IwSystemBuild(iw_sys_bridging);
        terminate_on_error(status, "WP_IwSystemBuild - down link rule");
    }
    return 0;
}

time_t sysInitTime, sysRealTime;


int linx_init(void)
{
    LINX *linx;

	/* Open LINX socket */
	linx = linx_open(SERVER_NAME, 0, NULL);
	if (linx == NULL)
	{
        WDDI_ERR("linx_open() %s", strerror(errno));
        return -1;
	}
	npu_set_server_linx_info(linx);

    WDDI_INFO("SPP WDDI Server started.\n");
    /*add for npumon to get sys time*/
	sysInitTime = time(NULL);
    return  linx_get_descriptor(linx);
}

int npu_handle_linx_msg(void)
{
	LINX_SIGSELECT sigsel_any[] = { 0 };
	LINX_SPID client = (LINX_SPID)NULL;
	union LINX_SIGNAL *sig;
	union LINX_SIGNAL *reply_sig;
    STRU_LINX_SERVER_INFO * linx_info;
	int status;
	int ret;
	linx_info = npu_get_server_linx_info();
	int rsvnum = 50;
	while(rsvnum--)
	{
		/* Wait until a signal arrives */
		if (linx_receive_w_tmo(linx_info->linx, &sig, 0,
                               sigsel_any) == -1)
		{
			WDDI_WARNING("linx_receive() %s\n", strerror(errno));
			break;/*maybe ENOMEM there is not enough memory,EINTR interrupt by a signal.EBADF, EFAULT, ENOTCONN, ENOTSOCK if the underlying linx structure contains a invalid socket descriptor*/
	    }
		if(sig == LINX_NIL)
		{
			//WDDI_WARNING("Server: Idle too long, restart to receive\n");
#if 0
			if (linx_close(linx_info->linx) == -1)
			{
				WDDI_WARNING("linx_close() %s", strerror(errno));
			}
#endif
			break;
		}
		else
	    {
			client = linx_sender(linx_info->linx, &sig);
			if (client == LINX_ILLEGAL_SPID)
			{
				WDDI_WARNING("linx_sender() %s\n", strerror(errno));
			}
			if(g_npinitialFlag == NPU_UNINITALIZED)
			{
			    if (linx_free_buf(linx_info->linx, &sig) == -1)
				{
                    WDDI_WARNING("linx_free_buf() %s\n", strerror(errno));
				}
				return 0;
			}
	        switch (sig->sig_no)
            {
            case NPU_ECHO_SIG:
            {
                WDDI_PRINT("Server: NPU_ECHO_SIG received.\n");
                if(linx_send(linx_info->linx, &sig, client) == -1)
                {
                    LINX_CLIENT_PRINT(linx_info->linx,client,"linx_send() %s client = 0x%x\n", strerror(errno),client);
                    break;
                }
                /* Attach to the client */
#if 1
                LINX_OSATTREF att_ref_client;
                sig = linx_alloc(linx_info->linx, sizeof(union LINX_SIGNAL), ATTACH_CLIENT_SIG);
                att_ref_client = linx_attach(linx_info->linx,&sig,client);
                if(att_ref_client == LINX_ILLEGAL_ATTREF)
                {
                    WDDI_WARNING("npu_handle_linx_msg::NPU_ECHO_SIG::linx_attach() failed %s\n",strerror(errno));
                }
#endif

                char *clientName=NULL;
                if(0 == linx_get_name(linx_info->linx,client,&clientName))
                {

                    pid_t clientPid = 0;
                    char clientAppname[100];
                    if(sscanf(clientName,"%*[a-zA-Z_]%d",&clientPid) == 0)
                    {
                        WDDI_WARNING("client: %s client =0x%x connected to server \n",clientName,client);
                    }
                    else
                    {
                        if(-1 == getAppnameByPID(clientAppname,clientPid))
                        {
                            WDDI_WARNING("getAppnameByPID failed ,clientPid = %d \n",clientPid);
                        }
                        WDDI_WARNING("client: %s client =0x%x connected to server clientAppname = %s\n",clientName,client,clientAppname);
                    }
                    linx_free_name(linx_info->linx,&clientName);
                }
                break;
            }
            case NPU_CFG_SIG:
            {
                if(linx_free_buf(linx_info->linx,&sig) == -1)
                {
                    WDDI_WARNING("linx_free_buf() %s\n", strerror(errno));
                }
                reply_sig = linx_alloc(linx_info->linx,sizeof(struct stru_npu_ack_sig),NPU_ACK_SIG);
                if( reply_sig== NULL)
                {
                    WDDI_WARNING("linx_alloc() %s\n", strerror(errno));
                }
                /*configure NPU ,returen the result*/
                /*
       *********** npu configure funcion
       */
                reply_sig->npu_ack.result = STATUS_OK;
                if (linx_send(linx_info->linx, &reply_sig, client) == -1)
                {
                    LINX_CLIENT_PRINT(linx_info->linx,client,"linx_send() %s client = 0x%x\n", strerror(errno),client);
                }
                break;
            }
            case VPORT_PORT_OPEN_SIG:
            {
                u32 ipaddr_src,port_src;
                u8  vp_type;
                vp_type = sig->vport_open.vp_type;
                ipaddr_src = sig->vport_open.ipaddr_src;
                port_src = sig->vport_open.port_src;

                status = npu_set_vport_info(client,vp_type,ipaddr_src,port_src);
                if(status != STATUS_OK)
                {
                    WDDI_WARNING("npu_set_vport_info() status fail\n");
                    break;
                }
                if(vp_type == VP_TYPE_FR)
                {
                    u32 card;
                    u32 trunk;
                    u32 group;
                    u32 dlci;
                    np_fraddr_unpack(port_src,&card,&trunk,&group,&dlci);
                    WDDI_INFO("Info: VPORT_PORT_OPEN_SIG received. vp_type:%s, port_src:0x%x, card:trunk:group:dlci :: S%dP%dFR%dD%d\n","VP_TYPE_FR",port_src,card,trunk,group,dlci);
                }
                else
                {
                    WDDI_INFO("Info: VPORT_PORT_OPEN_SIG received. vp_type:%s, ipaddr_src:0x%x, port_src:%d\n","VP_TYPE_UDP",ipaddr_src,htons(port_src));

                }




                if(linx_free_buf(linx_info->linx,&sig) == -1)
                {
                    WDDI_WARNING("linx_free_buf() %s\n", strerror(errno));
                }
                reply_sig = linx_alloc(linx_info->linx,sizeof(struct stru_npu_ack_sig),NPU_ACK_SIG);
                if( reply_sig== NULL)
                {
                    WDDI_WARNING("linx_alloc() %s\n", strerror(errno));
                }
                if(vp_type == VP_TYPE_UDP)
                {

                }
                else if(vp_type == VP_TYPE_FR)
                {
                    status = np_creat_fr_vport(port_src);
                }
                else
                {
                    WDDI_INFO("Info: create vport with invalid vp_type: %d \n",vp_type);
                    break;
                }
                STRU_VPORT_INFO * p_VportInfo= npu_get_vport_info(port_src);
                if((STATUS_OK != status)&&(vp_type == VP_TYPE_FR))
                {
                    p_VportInfo->UsedFlag = UNUSED;
                }
                else
                {
                    p_VportInfo->UsedFlag = USED;

                }
                /*configure NPU open port,returen the result*/
                /*
       *********** npu configure funcion
       */
                reply_sig->npu_ack.result = status;
                if (linx_send(linx_info->linx, &reply_sig, client) == -1)
                {
                    LINX_CLIENT_PRINT(linx_info->linx,client,"linx_send() %s client = 0x%x\n", strerror(errno),client);
                }
                break;
            }
            case VPORT_PORT_CLOSE_SIG:
            {
                u32 port_src;
                u8  vp_type;
                port_src = sig->vport_close.port_src;
                vp_type = sig->vport_close.vp_type;
                //if(port_src >= 65536)	WDDI_WARNING("npu_clear_vport_info() port error");
                //Fprintf("server: received close vport sig  port_src = %d\n",port_src);
                /*configure NPU close port,returen the result*/
                WDDI_INFO("Info: VPORT_PORT_CLOSE_SIG received. port_src:0x%x, vp_type:%d\n",port_src, vp_type);
                if(vp_type == VP_TYPE_FR)
                {
                    ret = np_close_fr_vport(port_src);
                }
                else
                {
                    /*for ethernet port do nothing*/
                    ret = STATUS_OK;
                }

                status = npu_clear_vport_info(port_src,vp_type);
                if(status != STATUS_OK)
                {
                    WDDI_WARNING("Warn: npu_clear_vport_info() status error\n");
                }

                if(linx_free_buf(linx_info->linx,&sig) == -1)
                {
                    WDDI_WARNING("linx_free_buf() %s\n", strerror(errno));
                }
                reply_sig = linx_alloc(linx_info->linx,sizeof(struct stru_npu_ack_sig),NPU_ACK_SIG);
                if( reply_sig== NULL)
                {
                    WDDI_WARNING("linx_alloc() %s\n", strerror(errno));
                }
                /*
       *********** npu configure funcion
       */
                reply_sig->npu_ack.result = status;
                if (linx_send(linx_info->linx, &reply_sig, client) == -1)
                {
                    LINX_CLIENT_PRINT(linx_info->linx,client,"linx_send() %s client = 0x%x\n", strerror(errno),client);
                }
                break;
            }

            case SPP_SEND_SIG:
            {
                u8  vp_type;
                u32 length;
                u8  *pdata;
                u32 port_src;
                u32 port_dst;
                u32 ipaddr_dst;
                u32 ipaddr_src;
                u32 i;
                vp_type = sig->spp_send.vp_type;
                length =  sig->spp_send.length;
                port_src = sig->spp_send.port_src;
                port_dst = sig->spp_send.port_dst;
                ipaddr_dst = sig->spp_send.ipaddr_dst;
                ipaddr_src = sig->spp_send.ipaddr_src;
                pdata = sig->spp_send.pdata;
                if(vp_type == VP_TYPE_FR)
                {
                    u32 card;
                    u32 trunk;
                    u32 group;
                    u32 dlci;
                    np_fraddr_unpack(port_src,&card,&trunk,&group,&dlci);
                    WDDI_INFO("Info: SPP_SEND_SIG received. vp_type:%s, length:0x%x, port_src:%d ::S%dP%dFR%dD%d\n","VP_TYPE_FR",length,(port_src),card,trunk,group,dlci);

                }
                else
                {
                    WDDI_INFO("Info: SPP_SEND_SIG received. vp_type:%s, length:0x%x, port_src:%d port_dst:%d, ipaddr_src:0x%x, ipaddr_dst:0x%x\n","VP_TYPE_UDP",length,htons(port_src),htons(port_dst),ipaddr_src,ipaddr_dst);

                }


#if 0
                u32 k;
                for (k = 0; k < length; k++)
                {
                    Fprintf("%2.2x ", pdata[k]);
                }
                Fprintf("\n");

#endif
                if(np_waittime == NPU_WAIT_FINISHED)
                {
                    if(vp_type == VP_TYPE_UDP)
                    {
                        STRU_VPORT_INFO * VportInfo;
                        VportInfo = npu_get_vport_info(port_src);
                        if(VportInfo != NULL)
                        {
                            if(VportInfo->UsedFlag == USED)
                            {
#if ENEA_RELEASE
                                if(VportInfo->client != client)
                                {
                                    char *clientName=NULL;
                                    if(0 == linx_get_name(linx_info->linx,client,&clientName))
                                    {
                                        WDDI_WARNING("send UDP SPP_SEND_SIG client has been changed! client: %s client =0x%x old client=0x%x \n",clientName,client,VportInfo->client);
                                        linx_free_name(linx_info->linx,&clientName);
                                    }
                                }
#endif
                                VportInfo->client = client;
                                /************ npu send funcion*/
                                /*check the value, if not reasonable, do not send the data*/
                                if(port_src == 0 || port_dst == 0 || port_src > 65535 || port_dst > 65535 || ipaddr_dst == 0 || ipaddr_src == 0 || length > 1490)
                                {
                                    WDDI_WARNING("UDP packet is dropped for invalid parameters! npu_handle_linx_msg::SPP_SEND_SIG::length:%d,port_src =0x%x,port_dst=0x%x,ipaddr_dst=0x%x,ipaddr_src=0x%x,pdata=0x%x\n",
                                                 length,port_src,port_dst,ipaddr_dst,ipaddr_src,pdata);
                                    if (length > 1490)
                                    {
                                        WDDI_WARNING("UDP dropped invalid length:%d,port_src =0x%x,port_dst=0x%x,ipaddr_dst=0x%x,ipaddr_src=0x%x, \n", length,port_src,port_dst,ipaddr_dst,ipaddr_src);
                                        for (i = 0; i < 100; i++)
                                        {
                                            WDDI_WARNING("%02x", pdata[i]);
                                            if ((i+1) % 4 == 0)
                                            {
                                                WDDI_WARNING(" ");
                                            }
                                        }
                                        WDDI_WARNING("\n");
                                    }
                                }
                                else
                                {
                                    npu_send_udp_packet(pdata,length,ipaddr_src,ipaddr_dst,port_src,port_dst);
                                    /*set host statics*/
                                    np_host_set_ns_bssgp_statics(pdata,length,&VportInfo->ns_bssgp_statics);
                                }

                            }
                            else
                            {
                                WDDI_WARNING("Warn: SPP_SEND_SIG::VP_TYPE_UDP, vport:%d unused!\n",(u16)htons(port_src));
                            }
                        }

                    }
                    else if(vp_type == VP_TYPE_FR)
                    {

                        STRU_VPORT_INFO * VportInfo;
                        VportInfo = npu_get_vport_info(port_src);
                        if(VportInfo != NULL)
                        {
                            if(VportInfo->UsedFlag == USED)
                            {
#if ENEA_RELEASE
                                if(VportInfo->client != client)
                                {
                                    char *clientName=NULL;
                                    if(0 == linx_get_name(linx_info->linx,client,&clientName))
                                    {
                                        WDDI_WARNING("send FR SPP_SEND_SIG client has been changed! client: %s client =0x%x old client=0x%x \n",clientName,client,VportInfo->client);
                                        linx_free_name(linx_info->linx,&clientName);
                                    }
                                }
#endif
                                VportInfo->client = client;/*in case of client app reset*/
                                /************ npu send funcion*/
                                WDDI_INFO("Info: SPP_SEND_SIG::VP_TYPE_FR->npu_send_fr_packet, port_src:%d\n",(u16)(port_src));
                                npu_send_fr_packet(pdata,length,VportInfo->port_src);
                                /*set host statics*/
                                np_host_set_ns_bssgp_statics(pdata,length,&VportInfo->ns_bssgp_statics);

                            }
                            else
                            {
                                u32 card;
                                u32 trunk;
                                u32 group;
                                u32 dlci;
                                np_fraddr_unpack(port_src,&card,&trunk,&group,&dlci);
                                WDDI_WARNING("Warn: SPP_SEND_SIG::VP_TYPE_FR, vport:%d:S%dP%dFR%dD%d unused!\n",(u16)(port_src),card,trunk,group,dlci);
                            }

                        }
                    }
                    else
                    {
                        WDDI_WARNING("Warn: SPP_SEND_SIG, vp_type:%d invalid!\n",vp_type);
                    }

                }

                if(linx_free_buf(linx_info->linx,&sig) == -1)
                {
                    WDDI_WARNING("linx_free_buf() %s", strerror(errno));
                }
                break;
            }
            case NPU_REQUEST_SIG:
            {
                if(NPU_SET_REQUEST_TYPE == sig->npu_request.cmdType)
                {
                    switch(sig->npu_request.cmd)
                    {
                    case NPU_SET_RECONFIG:
                    {
                        STRU_SET_INITAIL_CONFIG *initial_cfg = npu_get_initial_cfg_info();
                        STRU_SET_INITAIL_CONFIG *np_reconfig = npu_get_np_reconfig();
                        memset(np_reconfig,0,sizeof(STRU_SET_INITAIL_CONFIG));
                        npu_set_initial_cfg_info(xmlFileLocation,np_reconfig);
                        memcpy(g_np_reconfig.ifMACsrc,g_npinitial.ifMACsrc,6);
                        //tap_hwadd_read(g_np_reconfig.ifMACsrc);

                        reply_sig = linx_alloc(linx_info->linx, sizeof(struct stru_npu_set_request_ack_sig), NPU_SET_REQUEST_ACK_SIG);
                        if (reply_sig == NULL)
                        {
                            WDDI_WARNING("linx_alloc() failed\n");
                            return ERR_LINX_ALLOC;
                        }
                        if(NPU_MODE_FR == np_reconfig->OpterateMode)
                        {
                            WDDI_WARNING("Warn: NPU_REQUEST_SIG::NPU_SET_RECONFIG, reconfig mode is FR!\n");

                            //np_reconfig_set_sturct(&stru_reconfig_stm1);
                            ret = WTUFE_Reconfig_Stm1_All(initial_cfg,np_reconfig);
                        }
                        else
                        {
                            WDDI_WARNING("Warn: NPU_REQUEST_SIG::NPU_SET_RECONFIG, reconfig mode is IP!\n");
                        }
                        u32 i;
                        u32 routeFlag = 0;
                        for(i = 0;i< SPP_MAX_ROUTE_NUM;i++)
                        {
                            if(initial_cfg->routeInfo[i].GatewayIP != np_reconfig->routeInfo[i].GatewayIP)
                            {
                                routeFlag =1 ;
                            }
                            if(initial_cfg->routeInfo[i].IpDst != np_reconfig->routeInfo[i].IpDst)
                            {
                                routeFlag =1 ;
                            }
                        }
                        if(initial_cfg->VlanTag != np_reconfig->VlanTag || routeFlag || initial_cfg->ifIpaddr != np_reconfig->ifIpaddr)
                        {
                            np_reconfig_vlan_in_routeSystem();
                            np_reconfig_vlan_in_iwPort();
                            tap_interface_reconfig();
                            ret = 0;
                        }
                        else
                        {
                            //WDDI_PRINT("reconfig mode Ip port_num = %u, port_used_num=%u \n",port_num,port_used_num);
                            ret = 0;
                            WDDI_WARNING("Warn: NPU_REQUEST_SIG::NPU_SET_RECONFIG, Reconfig parameters are the same!!");
                        }

                        // update the old initial's values.
                        memcpy(initial_cfg, np_reconfig,sizeof(STRU_SET_INITAIL_CONFIG));

                        if(g_npinitial.NRI_bitLen >0 && g_npinitial.NRI_bitLen<= 8)
                        {
                            nri_bit_length = g_npinitial.NRI_bitLen;
                        }
                        // create lookup table for dps, defined how to parse message, nri size will change
                        WP_LookupUpdate();



                        reply_sig->npu_set_request_ack.result = ret;
                        if (linx_send(linx_info->linx, &reply_sig, client) == -1)
                        {
                            LINX_CLIENT_PRINT(linx_info->linx,client,"linx_send() %s client = 0x%x", strerror(errno),client);
                            return ERR_LINX_SEND;
                        }
                    }
                    break;

                    case NPU_SET_UPLINK_PATH_RULE:
                    {
                        reply_sig = linx_alloc(linx_info->linx, sizeof(struct stru_npu_set_request_ack_sig), NPU_SET_REQUEST_ACK_SIG);
                        if (reply_sig == NULL)
                        {
                            WDDI_WARNING("linx_alloc() failed\n");
                            return ERR_LINX_ALLOC;
                        }
                        STRU_SET_UPLINK_PATH_RULE ul_rule={0};
                        memcpy(&ul_rule,sig->npu_request.pargv,sizeof(STRU_SET_UPLINK_PATH_RULE));
                        WDDI_INFO("info: NPU_REQUEST_SIG::NPU_SET_UPLINK_PATH_RULE, optMode:%d, NRIValue:%d, PortSrc:%d, vfport:%d\n", ul_rule.OptMode, ul_rule.NRIValue, ul_rule.PortSrc, ul_rule.u_port.vfport);
                        ret = np_ul_route_table_add(&ul_rule);
                        reply_sig->npu_set_request_ack.result = ret ;
                        if (linx_send(linx_info->linx, &reply_sig, client) == -1)
                        {
                            LINX_CLIENT_PRINT(linx_info->linx,client,"linx_send() %s client = 0x%x", strerror(errno),client);
                            return ERR_LINX_SEND;
                        }
                        break;
                    }
                    case NPU_DELETE_UPLINK_PATH_RULE:
                    {
                        reply_sig = linx_alloc(linx_info->linx, sizeof(struct stru_npu_set_request_ack_sig), NPU_SET_REQUEST_ACK_SIG);
                        if (reply_sig == NULL)
                        {
                            WDDI_WARNING("linx_alloc() failed\n");
                            return ERR_LINX_ALLOC;
                        }
                        STRU_DELETE_UPLINK_PATH_RULE ul_rule={0};
                        memcpy(&ul_rule,sig->npu_request.pargv,sizeof(STRU_DELETE_UPLINK_PATH_RULE));
                        WDDI_INFO("info: NPU_REQUEST_SIG::NPU_DELETE_UPLINK_PATH_RULE, DeleteMode:%d, NRIValue:%d, vfport:%d\n", ul_rule.DeleteMode, ul_rule.NRIValue, ul_rule.u_port.vfport);
                        if(ul_rule.DeleteMode == DEL_UL_RULE_TALBE_BASED_ON_PORT_NRI)
                        {
                            ret = np_ul_route_table_delete(ul_rule.u_port.vfport,ul_rule.NRIValue);
                        }
                        else if(ul_rule.DeleteMode == DEL_UL_RULE_TALBE_BASED_ON_PORT)
                        {
                            u32 nri;
                            /*flush 257 NRI IPEP*/
                            for(nri = 0;nri < 257 ;nri++)
                            {
                                (void)np_ul_route_table_delete(ul_rule.u_port.vfport,nri);
                                ret = STATUS_OK;
                            }
                        }
                        else
                        {
                            WDDI_WARNING("Warn: NPU_DELETE_UPLINK_PATH_RULE  DeleteMode ERROR %d",ul_rule.DeleteMode);
                            ret = NPU_DELETE_FR_UPLINK_PATH_RULE_ERROR;
                        }

                        reply_sig->npu_set_request_ack.result = ret ;
                        if (linx_send(linx_info->linx, &reply_sig, client) == -1)
                        {
                            LINX_CLIENT_PRINT(linx_info->linx,client,"linx_send() %s client = 0x%x", strerror(errno),client);
                            return ERR_LINX_SEND;
                        }
                        break;
                    }

                    case NPU_SET_UPLINK_PATH_LINK_TABLE:
                    {
                        reply_sig = linx_alloc(linx_info->linx, sizeof(struct stru_npu_set_request_ack_sig), NPU_SET_REQUEST_ACK_SIG);
                        if (reply_sig == NULL)
                        {
                            WDDI_WARNING("linx_alloc() failed\n");
                            return ERR_LINX_ALLOC;
                        }
                        STRU_SET_UPLINK_PATH_LINK_TABLE ul_link_table={0};
                        memcpy(&ul_link_table,sig->npu_request.pargv,sizeof(STRU_SET_UPLINK_PATH_LINK_TABLE));
                        WDDI_INFO("info: NPU_REQUEST_SIG::NPU_SET_UPLINK_PATH_LINK_TABLE, PortSrc:0x%x, IpEpAcitveNum:%d\n", ul_link_table.PortSrc, ul_link_table.IpEpAcitveNum);
                        if(ul_link_table.IpEpAcitveNum > 64)
                        {
                            WDDI_WARNING("NPU_SET_UPLINK_PATH_LINK_TABLE IpEpAcitveNum %d invalid \n",ul_link_table.IpEpAcitveNum);
                            reply_sig->npu_set_request_ack.result = -1 ;
                        }
                        else
                        {
                            ret = np_ul_link_table_set(&ul_link_table);
                            reply_sig->npu_set_request_ack.result = ret ;
                        }
                        if (linx_send(linx_info->linx, &reply_sig, client) == -1)
                        {
                            LINX_CLIENT_PRINT(linx_info->linx,client,"linx_send() %s client = 0x%x", strerror(errno),client);
                            return ERR_LINX_SEND;
                        }
                        break;
                    }
                    case NPU_SET_DOWNLINK_PATH_RULE:
                    {
                        reply_sig = linx_alloc(linx_info->linx, sizeof(struct stru_npu_set_request_ack_sig), NPU_SET_REQUEST_ACK_SIG);
                        if (reply_sig == NULL)
                        {
                            WDDI_WARNING("linx_alloc() failed\n");
                            return ERR_LINX_ALLOC;
                        }
                        // Buffer the request and batch handle it
                        //STRU_SET_DOWNLINK_PATH_RULE dl_rule={0};
                        struct dl_rule_node * node_p = (struct dl_rule_node *)malloc(sizeof(struct dl_rule_node));
                        memcpy(&(node_p->rule), sig->npu_request.pargv, sizeof(STRU_SET_DOWNLINK_PATH_RULE));
                        node_p->next = dl_rule_list;
                        dl_rule_list = node_p;
                        dl_rule_node_count++;
                        WDDI_INFO("info: NPU_REQUEST_SIG::NPU_SET_DOWNLINK_PATH_RULE, DportStartValue:%d, OptMode:%d, BVCI:%d, BVCInew:%d, NSEI|portSrc:%d\n", node_p->rule.DportStartValue, node_p->rule.OptMode, node_p->rule.BVCI, node_p->rule.BVCInew, node_p->rule.nsei_portSrc.NSEI);
                        //ret = np_dl_route_table_add(&dl_rule);
                        reply_sig->npu_set_request_ack.result = 0;
                        if (linx_send(linx_info->linx, &reply_sig, client) == -1)
                        {
                            LINX_CLIENT_PRINT(linx_info->linx,client,"linx_send() %s client = 0x%x", strerror(errno),client);
                            return ERR_LINX_SEND;
                        }
                        break;
                    }

                    case NPU_DELETE_DOWNLINK_PATH_RULE:
                    {
                        reply_sig = linx_alloc(linx_info->linx, sizeof(struct stru_npu_set_request_ack_sig), NPU_SET_REQUEST_ACK_SIG);
                        if (reply_sig == NULL)
                        {
                            WDDI_WARNING("linx_alloc() failed\n");
                            return ERR_LINX_ALLOC;
                        }
                        STRU_DELETE_DOWNLINK_PATH_RULE dl_rule={0};
                        memcpy(&dl_rule,sig->npu_request.pargv,sizeof(STRU_DELETE_DOWNLINK_PATH_RULE));
                        WDDI_INFO("info: NPU_REQUEST_SIG::NPU_DELETE_DOWNLINK_PATH_RULE, DportStartValue:%d, BVCI:%d, OptMode:%d\n", dl_rule.DportStartValue, dl_rule.BVCI, dl_rule.OptMode);
                        ret = np_dl_route_table_delete(dl_rule.DportStartValue,dl_rule.BVCI,dl_rule.OptMode);

                        reply_sig->npu_set_request_ack.result = ret;
                        if (linx_send(linx_info->linx, &reply_sig, client) == -1)
                        {
                            LINX_CLIENT_PRINT(linx_info->linx,client,"linx_send() %s client = 0x%x", strerror(errno),client);
                            return ERR_LINX_SEND;
                        }
                        break;
                    }

                    case NPU_SET_UPDATE_BVCI_PATH_RULE:
                    {
                        reply_sig = linx_alloc(linx_info->linx, sizeof(struct stru_npu_set_request_ack_sig), NPU_SET_REQUEST_ACK_SIG);
                        if (reply_sig == NULL)
                        {
                            WDDI_WARNING("linx_alloc() failed\n");
                            return ERR_LINX_ALLOC;
                        }
                        STRU_SET_UPDATE_BVCI_PATH_RULE bvci_rule={0};
                        memcpy(&bvci_rule,sig->npu_request.pargv,sizeof(STRU_SET_UPDATE_BVCI_PATH_RULE));
                        WDDI_INFO("info: NPU_REQUEST_SIG::NPU_SET_UPDATE_BVCI_PATH_RULE, OptMode:%d, NS_BVCI:%d, NS_BVCI_old:%d, vfport:%d\n", bvci_rule.OptMode, bvci_rule.NS_BVCI, bvci_rule.NS_BVCI_old, bvci_rule.u_port.vfport);
                        ret = np_updateBVCI_flow_add(&bvci_rule);
                        reply_sig->npu_set_request_ack.result = ret;
                        if (linx_send(linx_info->linx, &reply_sig, client) == -1)
                        {
                            LINX_CLIENT_PRINT(linx_info->linx,client,"linx_send() %s client = 0x%x", strerror(errno),client);
                            return ERR_LINX_SEND;
                        }
                        break;
                    }

                    case NPU_DELETE_UPDATE_BVCI_PATH_RULE:
                    {
                        reply_sig = linx_alloc(linx_info->linx, sizeof(struct stru_npu_set_request_ack_sig), NPU_SET_REQUEST_ACK_SIG);
                        if (reply_sig == NULL)
                        {
                            WDDI_WARNING("linx_alloc() failed\n");
                            return ERR_LINX_ALLOC;
                        }
                        STRU_DELETE_UPDATE_BVCI_PATH_RULE bvci_rule={0};
                        memcpy(&bvci_rule,sig->npu_request.pargv,sizeof(STRU_DELETE_UPDATE_BVCI_PATH_RULE));
                        WDDI_INFO("info: NPU_REQUEST_SIG::NPU_DELETE_UPDATE_BVCI_PATH_RULE, OptMode:%d, NS_BVCI_old:%d, vfport:%d\n", bvci_rule.OptMode, bvci_rule.NS_BVCI_old, bvci_rule.u_port.vfport);
                        ret = np_updateBVCI_flow_delete(bvci_rule.u_port.vfport,bvci_rule.NS_BVCI_old,bvci_rule.OptMode);
                        reply_sig->npu_set_request_ack.result = ret;
                        if (linx_send(linx_info->linx, &reply_sig, client) == -1)
                        {
                            LINX_CLIENT_PRINT(linx_info->linx,client,"linx_send() %s client = 0x%x", strerror(errno),client);
                            return ERR_LINX_SEND;
                        }
                        break;
                    }
                    case NPU_SET_FR_DOWNLINK_PATH_LINK_TABLE:
                    {
                        reply_sig = linx_alloc(linx_info->linx, sizeof(struct stru_npu_set_request_ack_sig), NPU_SET_REQUEST_ACK_SIG);
                        if (reply_sig == NULL)
                        {
                            WDDI_WARNING("linx_alloc() failed\n");
                            return ERR_LINX_ALLOC;
                        }
                        STRU_SET_FR_DOWNLINK_PATH_LINK_TABLE dl_link_table={0};
                        memcpy(&dl_link_table,sig->npu_request.pargv,sizeof(STRU_SET_FR_DOWNLINK_PATH_LINK_TABLE));
                        WDDI_INFO("info: NPU_REQUEST_SIG::NPU_SET_FR_DOWNLINK_PATH_LINK_TABLE, FrVportAcitveNum:%d, NSEI:%d\n", dl_link_table.FrVportAcitveNum, dl_link_table.NSEI);
                        if(dl_link_table.FrVportAcitveNum > 16)
                        {
                            WDDI_WARNING("NPU_SET_FR_DOWNLINK_PATH_LINK_TABLE FrVportAcitveNum %dinvalid \n",dl_link_table.FrVportAcitveNum);
                            reply_sig->npu_set_request_ack.result = -1;
                        }
                        else
                        {
                            ret = np_dl_link_table_set(&dl_link_table);
                            reply_sig->npu_set_request_ack.result = ret;

                        }
                        if (linx_send(linx_info->linx, &reply_sig, client) == -1)
                        {
                            LINX_CLIENT_PRINT(linx_info->linx,client,"linx_send() %s client = 0x%x", strerror(errno),client);
                            return ERR_LINX_SEND;
                        }
                        break;
                    }

                    /*npumon*/
                    case NPU_MON_SET_CAPTURE_START:
                    {
                        reply_sig = linx_alloc(linx_info->linx, sizeof(struct stru_npu_set_request_ack_sig), NPU_SET_REQUEST_ACK_SIG);
                        if (reply_sig == NULL)
                        {
                            WDDI_WARNING("linx_alloc() failed\n");
                            return ERR_LINX_ALLOC;
                        }
                        STRU_NPUMON_INFO NpuMonInfo={0};
                        memcpy(&NpuMonInfo,sig->npu_request.pargv,sizeof(STRU_NPUMON_INFO));
                        ret = npu_hanlde_npumon_request(&NpuMonInfo,client);
                        reply_sig->npu_set_request_ack.result = ret;
                        if (linx_send(linx_info->linx, &reply_sig, client) == -1)
                        {
                            LINX_CLIENT_PRINT(linx_info->linx,client,"linx_send() %s client = 0x%x", strerror(errno),client);
                            return ERR_LINX_SEND;
                        }
                        /*add attach for npumon ,to close capture function on vport */
                        union LINX_SIGNAL *sigAttch;
                        LINX_OSATTREF att_ref_client;
                        sigAttch = linx_alloc(linx_info->linx, sizeof(union LINX_SIGNAL), ATTACH_NPUMON_SIG);
                        att_ref_client = linx_attach(linx_info->linx,&sigAttch,client);
                        if(att_ref_client == LINX_ILLEGAL_ATTREF)
                        {
                            WDDI_WARNING("linx_attach() failed");
                        }
                        break;
                    }
                    case NPU_SET_LMI_PVC_STATUS:
                    {
                        reply_sig = linx_alloc(linx_info->linx, sizeof(struct stru_npu_set_request_ack_sig), NPU_SET_REQUEST_ACK_SIG);
                        if (reply_sig == NULL)
                        {
                            WDDI_WARNING("linx_alloc() failed\n");
                            return ERR_LINX_ALLOC;
                        }
                        STRU_NPU_SET_LMI_PVC_STATUS PvcStatus={0};
                        memcpy(&PvcStatus,sig->npu_request.pargv,sizeof(STRU_NPU_SET_LMI_PVC_STATUS));
                        WDDI_INFO("info: NPU_REQUEST_SIG::NPU_SET_LMI_PVC_STATUS, status:%d, vfport:%d\n", PvcStatus.status, PvcStatus.vfport);
                        if(PvcStatus.status == SET_PVC_DISABLE)
                        {

                            STRU_VPORT_INFO * p_VportInfo= npu_get_vport_info(PvcStatus.vfport);
                            if(p_VportInfo != NULL)
                            {
                                if(p_VportInfo->UsedFlag == USED)
                                {
                                    /*maybe we should disable/delete iw_channel first,because it will use
                                      ch_info->agg_fr_rx_host_flow when binding
                                      WP_ERR_IW_FLOW_UNINITIALIZED error when call host_receive() may caused by this reason*/
                                    np_disable_fr_iw_channel(&p_VportInfo->ch_info);
                                    np_disable_fr_host_channel(&p_VportInfo->ch_info);
                                }

                            }
                        }
                        else if (PvcStatus.status == SET_PVC_ENABLE)
                        {
                            STRU_VPORT_INFO * p_VportInfo= npu_get_vport_info(PvcStatus.vfport);
                            if(p_VportInfo != NULL)
                            {
                                if(p_VportInfo->UsedFlag == USED)
                                {
                                    np_enable_fr_host_channel(&p_VportInfo->ch_info);
                                    np_enable_fr_iw_channel(&p_VportInfo->ch_info);
                                }
                            }
                        }
                        else
                        {

                        }

                        reply_sig->npu_set_request_ack.result = STATUS_OK;
                        if (linx_send(linx_info->linx, &reply_sig, client) == -1)
                        {
                            LINX_CLIENT_PRINT(linx_info->linx,client,"linx_send() %s client = 0x%x", strerror(errno),client);
                            return ERR_LINX_SEND;
                        }
                        break;
                    }
                    case NPU_SET_LOG_DEBUG_LEVEL:
                    {
                        char * wddi_log_lever[8] = {"LOG_DEBUG",
                                                    "LOG_INFO",
                                                    "LOG_NOTICE",
                                                    "LOG_WARNING",
                                                    "LOG_ERR",
                                                    "LOG_CRIT",
                                                    "LOG_ALERT",
                                                    "LOG_EMERG"
                        };
                        reply_sig = linx_alloc(linx_info->linx, sizeof(struct stru_npu_set_request_ack_sig), NPU_SET_REQUEST_ACK_SIG);
                        if (reply_sig == NULL)
                        {
                            WDDI_WARNING("linx_alloc() failed\n");
                            return ERR_LINX_ALLOC;
                        }
                        STRU_NPU_SET_LOG_DEBUG_LEVEL logLevel={0};
                        memcpy(&logLevel,sig->npu_request.pargv,sizeof(STRU_NPU_SET_LOG_DEBUG_LEVEL));
                        WDDI_WARNING("set log level %d \n",logLevel.level);
                        if(logLevel.level > 7)logLevel.level= 2;
                        g_logLevel = logLevel.level;
                        wddi_log_set_mask(wddi_log_lever[logLevel.level]);
                        reply_sig->npu_set_request_ack.result = STATUS_OK;
                        if (linx_send(linx_info->linx, &reply_sig, client) == -1)
                        {
                            LINX_CLIENT_PRINT(linx_info->linx,client,"linx_send() %s client = 0x%x", strerror(errno),client);
                            return ERR_LINX_SEND;
                        }
                        break;
                    }
                    case NPU_SET_NS_BSSGP_STATICS_ZERO:
                    {
                        u32 vfport;
                        STRU_SET_NS_BSSGP_STATICS_ZERO set_ns_bssgp_statics;
                        memcpy(&set_ns_bssgp_statics,sig->npu_request.pargv,sizeof(STRU_SET_NS_BSSGP_STATICS_ZERO));
                        ret = np_clear_ns_bssgp_statics(set_ns_bssgp_statics.vfport);
                        reply_sig = linx_alloc(linx_info->linx, sizeof(struct stru_npu_set_request_ack_sig), NPU_SET_REQUEST_ACK_SIG);
                        if (reply_sig == NULL)
                        {
                            WDDI_WARNING("linx_alloc() failed\n");
                            return ERR_LINX_ALLOC;
                        }
                        reply_sig->npu_set_request_ack.result = ret;
                        if (linx_send(linx_info->linx, &reply_sig, client) == -1)
                        {
                            LINX_CLIENT_PRINT(linx_info->linx,client,"linx_send() %s client = 0x%x", strerror(errno),client);
                            return ERR_LINX_SEND;
                        }
                        break;
                    }
                    case NPU_SET_NONPOOL_STATE:
                    {
                        NODE *temp;
                        ENTRY item={0};
                        ENTRY *found_item;
                        NP_UL_FLOW_INFO *p_FrFlowInfo;
                        u32 nri;
                        temp=uplink_nodehead;

                        /*clears all UL routes*/
                        while(temp!=NULL)
                        {
                            item.key = temp->ht_key;
                            temp=temp->next;

                            if(0 == hsearch_r(item,FIND,&found_item,uplink_ht))
                            {
                                WDDI_WARNING("When find ul_route_ht_link failed\n");
                            }
                            else
                            {
                                p_FrFlowInfo = (NP_UL_FLOW_INFO *)found_item->data;
                                if(USED == p_FrFlowInfo->UsedFlag)
                                {
                                    /*flush 257 NRI IPEP*/
                                    for(nri = 0;nri < 257 ;nri++)
                                    {
                                        (void)np_ul_route_table_delete(p_FrFlowInfo->vport,nri);
                                    }
                                }
                            }
                        }

                        STRU_SET_NPU_NONPOOL_STATE set_npu_nonpool;
                        memcpy(&set_npu_nonpool,sig->npu_request.pargv,sizeof(STRU_SET_NPU_NONPOOL_STATE));
                        Fprintf("Get set_npu_nonpool.nonpoolflag = %d\n",set_npu_nonpool.nonpoolflag);
                        g_npnonpoolFlag = set_npu_nonpool.nonpoolflag;
                        ret = STATUS_OK;

                        reply_sig = linx_alloc(linx_info->linx, sizeof(struct stru_npu_set_request_ack_sig), NPU_SET_REQUEST_ACK_SIG);
                        if (reply_sig == NULL)
                        {
                            WDDI_WARNING("linx_alloc() failed\n");
                            return ERR_LINX_ALLOC;
                        }
                        reply_sig->npu_set_request_ack.result = ret;
                        if (linx_send(linx_info->linx, &reply_sig, client) == -1)
                        {
                            LINX_CLIENT_PRINT(linx_info->linx,client,"linx_send() %s client = 0x%x", strerror(errno),client);
                            return ERR_LINX_SEND;
                        }
                        break;
                    }
                    case NPU_SET_NSEAGG_FLAG:
                    {
                        u32 ret;

                        STRU_SET_NPU_NSEAGG_FLAG set_nseAgg_flag;
                        memcpy(&set_nseAgg_flag,sig->npu_request.pargv,sizeof(STRU_SET_NPU_NSEAGG_FLAG));
                        Fprintf("Get set_nseAgg_flag.nseAggFlag = %d\n",set_nseAgg_flag.nseAggFlag);
                        g_npNseAggFlag = set_nseAgg_flag.nseAggFlag;
                        ret = STATUS_OK;
                        /*set NSE Aggregation off*/
                        WP_NseAgg_set(g_npNseAggFlag);

                        reply_sig = linx_alloc(linx_info->linx, sizeof(struct stru_npu_set_request_ack_sig), NPU_SET_REQUEST_ACK_SIG);
                        if (reply_sig == NULL)
                        {
                            WDDI_WARNING("linx_alloc() failed\n");
                            return ERR_LINX_ALLOC;
                        }
                        reply_sig->npu_set_request_ack.result = ret;
                        if (linx_send(linx_info->linx, &reply_sig, client) == -1)
                        {
                            LINX_CLIENT_PRINT(linx_info->linx,client,"linx_send() %s client = 0x%x", strerror(errno),client);
                            return ERR_LINX_SEND;
                        }
                        break;
                    }

                    }
                }

                else if(NPU_GET_REQUEST_TYPE == sig->npu_request.cmdType)
                {
                    switch(sig->npu_request.cmd)
                    {
                    case NPU_GET_ETH_STATICS:
                    {
                        u32 EthNumber;
                        STRU_GET_ETH_STATICS get_ethstatics;
                        EthNumber = ((STRU_GET_ETH_STATICS *)(sig->npu_request.pargv))->EthNumber;
                        reply_sig = linx_alloc(linx_info->linx, sizeof(struct stru_npu_get_request_ack_sig)+sizeof(STRU_GET_ETH_STATICS), NPU_GET_REQUEST_ACK_SIG);
                        if (reply_sig == NULL)
                        {
                            WDDI_WARNING("linx_alloc() failed\n");
                            return ERR_LINX_ALLOC;
                        }
                        get_ethstatics.EthNumber = EthNumber;
                        if((EthNumber !=1)&& (EthNumber !=2))
                        {
                            reply_sig->npu_get_request_ack.result = -1;
                        }
                        else
                        {
                            status = enet_stat_get(EthNumber, (void *)&(get_ethstatics.stru_eth_statics));
                        }
                        reply_sig->npu_get_request_ack.result = status;
                        memcpy(reply_sig->npu_get_request_ack.pdata,&get_ethstatics,sizeof(get_ethstatics));
                        reply_sig->npu_get_request_ack.cmd = NPU_GET_ETH_STATICS;
                        if (linx_send(linx_info->linx, &reply_sig, client) == -1)
                        {
                            LINX_CLIENT_PRINT(linx_info->linx,client,"linx_send() %s client = 0x%x", strerror(errno),client);
                            return ERR_LINX_SEND;
                        }
                        break;
                    }
                    case NPU_GET_UPI_TRUNK_STATICS:
                    {
                        u32 trunkid;
                        u32 stm1Id;
                        STRU_GET_UPI_TRUNK_STATICS get_upistatics;
                        stm1Id = ((STRU_GET_UPI_TRUNK_STATICS *)(sig->npu_request.pargv))->stm1Id;
                        if((stm1Id != 1)&&(stm1Id != 2))
                        {
                            WDDI_WARNING("NPU_GET_UPI_TRUNK_STATICS stm1Id %d invalid \n",stm1Id);
                            break;
                        }
                        trunkid = ((STRU_GET_UPI_TRUNK_STATICS *)(sig->npu_request.pargv))->trunkid;
                        reply_sig = linx_alloc(linx_info->linx, sizeof(struct stru_npu_get_request_ack_sig)+sizeof(STRU_GET_UPI_TRUNK_STATICS), NPU_GET_REQUEST_ACK_SIG);
                        if (reply_sig == NULL)
                        {
                            WDDI_WARNING("linx_alloc() failed\n");
                            return ERR_LINX_ALLOC;
                        }
                        get_upistatics.trunkid = trunkid;
                        get_upistatics.stm1Id = stm1Id;
                        if(trunkid > (SPP_MAX_TRUNK_NUM-1))
                        {
                            WDDI_WARNING("NPU_GET_UPI_TRUNK_STATICS trunkid %d invalid \n",trunkid);
                            break;
                        }
                        STRU_SET_INITAIL_CONFIG *initial_cfg = npu_get_initial_cfg_info();
                        if(initial_cfg->stru_stm1_cfg_info[stm1Id-1].E1T1type == NPU_MODE_E1)
                        {
                            trunkid = (stm1Id -1) * N_UFE_MAX_LINES_E1/2 + trunkid -1 ;
                        }
                        else
                        {
                            trunkid = (stm1Id -1) * N_UFE_MAX_LINES_T1/2 + trunkid -1 ;
                        }
                        status = upi_stat_get_by_trunk(trunkid, &(get_upistatics.stru_upi_statics));
                        reply_sig->npu_get_request_ack.result = status;
                        memcpy(reply_sig->npu_get_request_ack.pdata,(void *)&get_upistatics,sizeof(get_upistatics));
                        reply_sig->npu_get_request_ack.cmd = NPU_GET_UPI_TRUNK_STATICS;
                        if (linx_send(linx_info->linx, &reply_sig, client) == -1)
                        {
                            LINX_CLIENT_PRINT(linx_info->linx,client,"linx_send() %s client = 0x%x", strerror(errno),client);
                            return ERR_LINX_SEND;
                        }
                        break;
                    }
                    case NPU_GET_UPI_GROUP_STATICS:
                    {
                        u32 trunkid;
                        u32 stm1Id;
                        u32 groupId;
                        STRU_GET_UPI_GROUP_STATICS get_upistatics;
                        stm1Id = ((STRU_GET_UPI_GROUP_STATICS *)(sig->npu_request.pargv))->stm1Id;
                        if((stm1Id != 1)&&(stm1Id != 2))
                        {
                            WDDI_WARNING("NPU_GET_UPI_GROUP_STATICS stm1Id %d invalid \n",stm1Id);
                            break;
                        }
                        trunkid = ((STRU_GET_UPI_GROUP_STATICS *)(sig->npu_request.pargv))->trunkid;
                        groupId = ((STRU_GET_UPI_GROUP_STATICS *)(sig->npu_request.pargv))->groupid;
                        if((trunkid > (SPP_MAX_TRUNK_NUM-1)) ||(groupId > (SPP_MAX_GROUP_NUM -1)))
                        {
                            WDDI_WARNING("NPU_GET_UPI_GROUP_STATICS trunkid %d groupId %d invalid \n",trunkid,groupId);
                            break;
                        }
                        reply_sig = linx_alloc(linx_info->linx, sizeof(struct stru_npu_get_request_ack_sig)+sizeof(STRU_GET_UPI_GROUP_STATICS), NPU_GET_REQUEST_ACK_SIG);
                        if (reply_sig == NULL)
                        {
                            WDDI_WARNING("linx_alloc() failed\n");
                            return ERR_LINX_ALLOC;
                        }
                        get_upistatics.trunkid = trunkid;
                        get_upistatics.stm1Id = stm1Id;
                        get_upistatics.groupid= groupId;
                        STRU_SET_INITAIL_CONFIG *initial_cfg = npu_get_initial_cfg_info();
                        if(initial_cfg->stru_stm1_cfg_info[stm1Id-1].E1T1type == NPU_MODE_E1)
                        {
                            trunkid = (stm1Id -1) * N_UFE_MAX_LINES_E1/2 + trunkid -1 ;
                        }
                        else
                        {
                            trunkid = (stm1Id -1) * N_UFE_MAX_LINES_T1/2 + trunkid -1 ;
                        }
                        status = upi_stat_get_by_group(trunkid,groupId,&(get_upistatics.stru_upi_statics));
                        reply_sig->npu_get_request_ack.result = status;
                        memcpy(reply_sig->npu_get_request_ack.pdata,(void *)&get_upistatics,sizeof(get_upistatics));
                        reply_sig->npu_get_request_ack.cmd = NPU_GET_UPI_GROUP_STATICS;
                        if (linx_send(linx_info->linx, &reply_sig, client) == -1)
                        {
                            LINX_CLIENT_PRINT(linx_info->linx,client,"linx_send() %s client = 0x%x", strerror(errno),client);
                            return ERR_LINX_SEND;
                        }
                        break;
                    }

                    case NPU_GET_UPI_STM1_STATICS:
                    {
                        u32 stm1id;
                        STRU_GET_UPI_STM1_STATICS get_upistatics;
                        stm1id = ((STRU_GET_UPI_STM1_STATICS *)(sig->npu_request.pargv))->stm1id -1 ;
                        reply_sig = linx_alloc(linx_info->linx, sizeof(struct stru_npu_get_request_ack_sig)+sizeof(STRU_GET_UPI_STM1_STATICS), NPU_GET_REQUEST_ACK_SIG);
                        if (reply_sig == NULL)
                        {
                            WDDI_WARNING("linx_alloc() failed\n");
                            return ERR_LINX_ALLOC;
                        }
                        if((stm1id != 0)&&(stm1id != 1))
                        {
                            WDDI_WARNING("NPU_GET_UPI_STM1_STATICS stm1Id %d invalid \n",stm1id);
                            break;
                        }
                        get_upistatics.stm1id = stm1id;
                        status = upi_stm1_stat_get(stm1id, &(get_upistatics.stru_upi_statics));
                        reply_sig->npu_get_request_ack.result = status;
                        memcpy(reply_sig->npu_get_request_ack.pdata,(void *)&get_upistatics,sizeof(get_upistatics));
                        reply_sig->npu_get_request_ack.cmd = NPU_GET_UPI_STM1_STATICS;
                        if (linx_send(linx_info->linx, &reply_sig, client) == -1)
                        {
                            LINX_CLIENT_PRINT(linx_info->linx,client,"linx_send() %s client = 0x%x", strerror(errno),client);
                            return ERR_LINX_SEND;
                        }
                        break;
                    }

                    case NPU_GET_RXFR_STATICS:
                    {
                        u32 vfport;
                        STRU_GET_RXFR_STATICS get_rxfrstatics;
                        vfport = ((STRU_GET_RXFR_STATICS *)(sig->npu_request.pargv))->vfport;
                        reply_sig = linx_alloc(linx_info->linx, sizeof(struct stru_npu_get_request_ack_sig)+sizeof(STRU_GET_RXFR_STATICS), NPU_GET_REQUEST_ACK_SIG);
                        if (reply_sig == NULL)
                        {
                            WDDI_WARNING("linx_alloc() failed\n");
                            return ERR_LINX_ALLOC;
                        }
                        get_rxfrstatics.vfport = vfport;
                        status = rxfr_stat_get(vfport, (void *)&(get_rxfrstatics.stru_rxfr_statics));
                        reply_sig->npu_get_request_ack.result = status;
                        memcpy(reply_sig->npu_get_request_ack.pdata,&get_rxfrstatics,sizeof(get_rxfrstatics));
                        reply_sig->npu_get_request_ack.cmd = NPU_GET_RXFR_STATICS;
                        if (linx_send(linx_info->linx, &reply_sig, client) == -1)
                        {
                            LINX_CLIENT_PRINT(linx_info->linx,client,"linx_send() %s client = 0x%x", strerror(errno),client);
                            return ERR_LINX_SEND;
                        }
                        break;
                    }

                    case NPU_GET_TXFR_STATICS:
                    {
                        u32 vfport;
                        STRU_GET_TXFR_STATICS get_txfrstatics;
                        vfport = ((STRU_GET_TXFR_STATICS *)(sig->npu_request.pargv))->vfport;
                        reply_sig = linx_alloc(linx_info->linx, sizeof(struct stru_npu_get_request_ack_sig)+sizeof(STRU_GET_TXFR_STATICS), NPU_GET_REQUEST_ACK_SIG);
                        if (reply_sig == NULL)
                        {
                            WDDI_WARNING("linx_alloc() failed\n");
                            return ERR_LINX_ALLOC;
                        }
                        get_txfrstatics.vfport = vfport;
                        status = txfr_stat_get(vfport, (void *)&(get_txfrstatics.stru_txfr_statics));
                        reply_sig->npu_get_request_ack.result = status;
                        memcpy(reply_sig->npu_get_request_ack.pdata,&get_txfrstatics,sizeof(get_txfrstatics));
                        reply_sig->npu_get_request_ack.cmd = NPU_GET_TXFR_STATICS;
                        if (linx_send(linx_info->linx, &reply_sig, client) == -1)
                        {
                            LINX_CLIENT_PRINT(linx_info->linx,client,"linx_send() %s client = 0x%x", strerror(errno),client);
                            return ERR_LINX_SEND;
                        }
                        break;
                    }

                    case NPU_DEBUG_CHANNEL:
                    {
                        u32 vfport;
                        u32 debugtype;
                        STRU_DEBUG_CHANNEL np_debug_channel;
                        vfport = ((STRU_DEBUG_CHANNEL *)(sig->npu_request.pargv))->vfport;
                        debugtype = ((STRU_DEBUG_CHANNEL *)(sig->npu_request.pargv))->DebugType;
                        STRU_VPORT_INFO	*VportInfo = npu_get_vport_info(vfport);
                        if(VportInfo == NULL)
                        {
                            WDDI_WARNING(" can't get fr vport info vport = %d \n ",vfport);
                            break;
                        }
                        else if(VportInfo->UsedFlag == UNUSED)
                        {
                            WDDI_WARNING(" can't get fr vport info ,fr vport closed ! vport = %d \n ",vfport);
                            break;
                        }
                        FR_CHANNEL_INFO *ch_info = &VportInfo->ch_info;
                        if(debugtype == 0)
                        {
                            u32 depth=0;
                            WP_status ret=-1;
                            ret = WP_ChannelQDepth(ch_info->tx_host_handle,&depth);
                            WDDI_WARNING("WP_ChannelQDepth: tx_host_handle=0x%x,vport=%u,ret=%d,depth=%u\n",ch_info->tx_host_handle,      vfport,ret,depth);
                            ret=-1;
                            depth=0;
                            ret = WP_ChannelQDepth(ch_info->tx_iw_handle,&depth);
                            WDDI_WARNING("WP_ChannelQDepth: tx_iw_handle=0x%x,vport=%u,ret=%d,depth=%u\n",ch_info->tx_iw_handle,vfport,ret,depth);
                        }
                        else if(debugtype == 1)
                        {
                            WP_status ret=-1;
                            ret = WP_Display(0,ch_info->tx_host_handle,WP_DISPLAY_DEFAULT,0);
                            WDDI_WARNING("WP_Display channel: tx_host_handle=0x%x,vport=%u,ret=%d\n",ch_info->tx_host_handle,vfport,ret);
                        }
                        else if(debugtype == 2)
                        {
                            WP_status ret=-1;
                            WP_status status;
                            extern WP_handle upi_ufe_port;
                            status = WP_ChannelDisable(ch_info->rx_iw_handle);
                            status = WP_PortDisable(upi_ufe_port, WP_DIRECTION_DUPLEX);
                            sleep(1);
                            ret = WP_DeviceFlush(ch_info->device_handle);
                            sleep(1);
                            status = WP_ChannelDisable(ch_info->tx_host_handle);
                            sleep(1);
                            status = WP_ChannelEnable(ch_info->tx_host_handle);
                            status = WP_PortEnable(upi_ufe_port, WP_DIRECTION_DUPLEX);
                            status = WP_ChannelEnable(ch_info->rx_iw_handle);
                            WDDI_WARNING("WP_DeviceFlush: tx_host_handle=0x%x,vport=%u,ret=%d\n",ch_info->tx_host_handle,vfport,ret);
                        }
                        else if(debugtype == 3)
                        {
                            WP_status ret=-1;
                            ret = WP_Display(0,ch_info->phy_handle,WP_DISPLAY_DEFAULT,0);
                            WDDI_WARNING("WP_Display phy: tx_host_handle=0x%x,phy_handle=0x%x,vport=%u,ret=%d\n",ch_info->tx_host_handle,ch_info->phy_handle,vfport,ret);
                        }
                        else if(debugtype == 4)
                        {
                            WP_boolean ch_disabled_state = WP_TRUE;
                            WP_status ret=-1;
                            ret = WP_ChannelStatus(ch_info->tx_host_handle, WP_CH_STATUS_DISABLED, &ch_disabled_state);
                            WDDI_WARNING("WP_ChannelStatus: tx_host_handle=0x%x,vport=%u,ret=%d,disabled=%d\n",ch_info->tx_host_handle,vfport,ret,ch_disabled_state);
                        }
                        else if(debugtype == 5)
                        {
                            WP_status status;
                            WP_stats_emphy emphy_stats;
                            status=WP_EmphyPortStatistics(WP_PORT_UPI1,&emphy_stats);
                            WDDI_WARNING("WP_EmphyPortStatistics: status=0x%x\n",status);
                            WDDI_WARNING("    Hi: rx_unsupported_device=0x%x\n",*((int*)(&emphy_stats.rx_unsupported_device)));
                            WDDI_WARNING("    Lo: rx_unsupported_device=0x%x\n",*((int*)(&emphy_stats.rx_unsupported_device)+1));
                            WDDI_WARNING("    Hi: tx_unsupported_device=0x%x\n",*((int*)(&emphy_stats.tx_unsupported_device)));
                            WDDI_WARNING("    Lo: tx_unsupported_device=0x%x\n",*((int*)(&emphy_stats.tx_unsupported_device)+1));
                            WDDI_WARNING("    Hi: rx_err_parity=0x%x\n",*((int*)(&emphy_stats.rx_err_parity)));
                            WDDI_WARNING("    Lo: rx_err_parity=0x%x\n",*((int*)(&emphy_stats.rx_err_parity)+1));
                            WDDI_WARNING("    Hi: rx_err_sop_eop=0x%x\n",*((int*)(&emphy_stats.rx_err_sop_eop)));
                            WDDI_WARNING("    Lo: rx_err_sop_eop=0x%x\n",*((int*)(&emphy_stats.rx_err_sop_eop)+1));
                        }

                        break;
                    }

                    case NPU_GET_UFE_SYS:
                    {
                        Fprintf("UFE System Display\n");
                        np_display_ufe_system();

                        displayUfeEvents();
                        STRU_MEMORY_STATICS memory_statics;
                        Np_ShowMemoryAvailable(&memory_statics);
                        Fprintf("NP App:	PacketMemLeftByte = %d\n", memory_statics.PacketMemLeftByte);
                        Fprintf("NP App:	ParamMemLeftByte = %d\n", memory_statics.ParamMemLeftByte);
                        Fprintf("NP App:	InterMemLeftByte = %d\n", memory_statics.InterMemLeftByte);
                        Fprintf("NP App:	PacketMemTotalByte =%d\n", memory_statics.PacketMemTotalByte);
                        Fprintf("NP App:	ParamMemTotalByte = %d\n", memory_statics.ParamMemTotalByte);
                        Fprintf("NP App:	InterMemTotalByte = %d\n", memory_statics.InterMemTotalByte);


                        WPU_BusMonitorDump(0,0);
                        np_display_ufe4_shd_status();
                        np_display_all_ufe4_e1t1_status();
                        npu_printf_debugReg();
                        reply_sig = linx_alloc(linx_info->linx, sizeof(struct stru_npu_get_request_ack_sig), NPU_GET_REQUEST_ACK_SIG);
                        if (reply_sig == NULL)
                        {
                            WDDI_WARNING("linx_alloc() failed\n");
                            return ERR_LINX_ALLOC;
                        }

                        reply_sig->npu_get_request_ack.cmd = NPU_GET_UFE_SYS;
                        reply_sig->npu_get_request_ack.result = STATUS_OK;
                        if (linx_send(linx_info->linx, &reply_sig, client) == -1)
                        {
                            LINX_CLIENT_PRINT(linx_info->linx,client,"linx_send() %s client = 0x%x", strerror(errno),client);
                            return ERR_LINX_SEND;
                        }
                        break;
                    }

                    case NPU_DISPLAY_CONFIG_INFO:
                    {
                        Fprintf("NPU_DISPLAY_CONFIG_INFO\n");
                        display_config_info();
                        reply_sig = linx_alloc(linx_info->linx, sizeof(struct stru_npu_get_request_ack_sig), NPU_GET_REQUEST_ACK_SIG);
                        if (reply_sig == NULL)
                        {
                            WDDI_WARNING("linx_alloc() failed\n");
                            return ERR_LINX_ALLOC;
                        }
                        reply_sig->npu_get_request_ack.cmd = NPU_DISPLAY_CONFIG_INFO;
                        reply_sig->npu_get_request_ack.result = STATUS_OK;
                        if (linx_send(linx_info->linx, &reply_sig, client) == -1)
                        {
                            LINX_CLIENT_PRINT(linx_info->linx,client,"linx_send() %s client = 0x%x", strerror(errno),client);
                            return ERR_LINX_SEND;
                        }
                        break;
                    }

                    case NPU_GET_UFE_LINE:
                    {

                        u32	lineIndex;

                        lineIndex = ((STRU_GET_UFE_LINE *)(sig->npu_request.pargv))->Lineindex -1 ;
                        if(lineIndex > N_UFE_MAX_LINES_T1)
                        {
                            WDDI_WARNING("NPU_GET_UFE_LINE line %d is invalid \n",lineIndex);
                            break;
                        }
                        Fprintf("\t UFE Line Display for lineIndex 0x%x\n", lineIndex);
                        np_display_ufe_line(lineIndex);
                        reply_sig = linx_alloc(linx_info->linx, sizeof(struct stru_npu_get_request_ack_sig), NPU_GET_REQUEST_ACK_SIG);
                        if (reply_sig == NULL)
                        {
                            WDDI_WARNING("linx_alloc() failed\n");
                            return ERR_LINX_ALLOC;
                        }

                        reply_sig->npu_get_request_ack.cmd = NPU_GET_UFE_LINE;
                        reply_sig->npu_get_request_ack.result = STATUS_OK;
                        if (linx_send(linx_info->linx, &reply_sig, client) == -1)
                        {
                            WDDI_WARNING("linx_send() %s client = 0x%x", strerror(errno),client);
                            return ERR_LINX_SEND;
                        }
                        break;
                    }

                    case NPU_GET_UFE_PHY:
                    {
                        u32	value;

                        value = ((STRU_GET_UFE *)(sig->npu_request.pargv))->PortNumber;
                        Fprintf("\t UFE PHY Display for Port 0x%x\n", value);
                        np_display_ufe_phy(value);
                        reply_sig = linx_alloc(linx_info->linx, sizeof(struct stru_npu_get_request_ack_sig), NPU_GET_REQUEST_ACK_SIG);
                        if (reply_sig == NULL)
                        {
                            WDDI_WARNING("linx_alloc() failed\n");
                            return ERR_LINX_ALLOC;
                        }

                        reply_sig->npu_get_request_ack.cmd = NPU_GET_UFE_PHY;
                        reply_sig->npu_get_request_ack.result = STATUS_OK;
                        if (linx_send(linx_info->linx, &reply_sig, client) == -1)
                        {
                            WDDI_WARNING("linx_send() %s client = 0x%x", strerror(errno),client);
                            return ERR_LINX_SEND;
                        }
                        break;
                    }
                    case NPU_GET_FLOW_STATICS:
                    {
                        u32	vfort;
                        NPU_DEBUG_FILE_ENABLE();
                        STRU_VPORT_INFO  *VportInfo;
                        vfort = ((STRU_GET_UFE*)(sig->npu_request.pargv))->PortNumber;
                        NP_SYSTEM_INFO * the_system = np_get_system_info();
                        np_display_iw_port_stat();
                        np_dispaly_system_flow_agg();
                        np_display_frag_flow_info();
                        np_display_defrag_info(the_system->ip_defrag_group);
#if BFD_OVERETH
                        np_bfd_flow_statistics(the_system->agg_bfd_rx_handle);
                        np_bfd_sesstion_statistics(the_system->bfd_rx_table_handle);
                        //extern WP_handle tx_enet_handle_1,gbedev1;
                        //WP_Display(0,tx_enet_handle_1,WP_DISPLAY_DEFAULT,0);
                        //WP_Display(0,gbedev1,WP_DISPLAY_DEFAULT,0);
#endif
                        /*show flow statics and bport info*/
                        VportInfo = npu_get_vport_info(vfort);

                        if(VportInfo == NULL)
                        {
                            NPU_DEBUG_FILE_DISABLE();
                            return -1;
                        }
                        np_display_flow_agg(vfort);

                        /*turn off write debug file*/
                        NPU_DEBUG_FILE_DISABLE();

                        //WPU_WF(0,0,WPU_WF_CMD_GET_VERSION,WPU_wf_info * winfarms)
                        //WPU_BusMonitorDump(0,0);
                        //WP_Display(0,WP_DISPLAY_TA,0,NULL);
                        WP_Display(0,VportInfo->ch_info.rx_host_handle,WP_DISPLAY_DEFAULT,0);
                        WP_Display(0,VportInfo->ch_info.tx_host_handle,WP_DISPLAY_DEFAULT,0);
                        //WP_Display(0,VportInfo->ch_info.device_handle,WP_DISPLAY_DEFAULT,0);
                        //WP_Display(0,WP_DISPLAY_SA,0,NULL);
                        //WPU_TARegs(0,0);
                        reply_sig = linx_alloc(linx_info->linx, sizeof(struct stru_npu_get_request_ack_sig), NPU_GET_REQUEST_ACK_SIG);
                        if (reply_sig == NULL)
                        {
                            WDDI_WARNING("linx_alloc() failed\n");
                            return ERR_LINX_ALLOC;
                        }

                        reply_sig->npu_get_request_ack.cmd = NPU_GET_FLOW_STATICS;
                        reply_sig->npu_get_request_ack.result = STATUS_OK;
                        if (linx_send(linx_info->linx, &reply_sig, client) == -1)
                        {
                            WDDI_WARNING("linx_send() %s client = 0x%x", strerror(errno),client);
                            return ERR_LINX_SEND;
                        }
                        break;
                    }
                    case NPU_GET_CPU_STATICS:
                    {
                        STRU_CPU_STATICS get_cpu_statics;
                        Np_Get_Utilization(&get_cpu_statics);
                        /*get_cpu_statics.totalUtili = 10;*/
                        reply_sig = linx_alloc(linx_info->linx, sizeof(struct stru_npu_get_request_ack_sig)+sizeof(STRU_CPU_STATICS), NPU_GET_REQUEST_ACK_SIG);
                        if (reply_sig == NULL)
                        {
                            WDDI_WARNING("linx_alloc() failed\n");
                            return ERR_LINX_ALLOC;
                        }
                        reply_sig->npu_get_request_ack.cmd = NPU_GET_CPU_STATICS;
                        reply_sig->npu_get_request_ack.result = STATUS_OK;
                        memcpy(reply_sig->npu_get_request_ack.pdata,&get_cpu_statics,sizeof(get_cpu_statics));
                        if (linx_send(linx_info->linx, &reply_sig, client) == -1)
                        {
                            WDDI_WARNING("linx_send() %s client = 0x%x", strerror(errno),client);
                            return ERR_LINX_SEND;
                        }
                        break;
                    }
                    case NPU_GET_MEMORY_STATICS:
                    {
                        STRU_MEMORY_STATICS get_memory_statics;
                        Np_ShowMemoryAvailable(&get_memory_statics);
                        reply_sig = linx_alloc(linx_info->linx, sizeof(struct stru_npu_get_request_ack_sig)+sizeof(STRU_MEMORY_STATICS), NPU_GET_REQUEST_ACK_SIG);
                        if (reply_sig == NULL)
                        {
                            WDDI_WARNING("linx_alloc() failed\n");
                            return ERR_LINX_ALLOC;
                        }
                        reply_sig->npu_get_request_ack.cmd = NPU_GET_MEMORY_STATICS;
                        reply_sig->npu_get_request_ack.result = STATUS_OK;
                        memcpy(reply_sig->npu_get_request_ack.pdata,&get_memory_statics,sizeof(get_memory_statics));
                        if (linx_send(linx_info->linx, &reply_sig, client) == -1)
                        {
                            WDDI_WARNING("linx_send() %s client = 0x%x", strerror(errno),client);
                            return ERR_LINX_SEND;
                        }
                        break;
                    }
                    case NPU_GET_NS_BSSGP_STATICS_STATICS:
                    {
                        STRU_GET_NS_BSSGP_STATICS get_ns_bssgp_statics;
                        u32 vfport;
                        vfport = ((STRU_GET_NS_BSSGP_STATICS *)(sig->npu_request.pargv))->vfport;
                        ret = np_get_ns_bssgp_statics(vfport,(void *)&(get_ns_bssgp_statics));
                        reply_sig = linx_alloc(linx_info->linx, sizeof(struct stru_npu_get_request_ack_sig)+sizeof(STRU_GET_NS_BSSGP_STATICS), NPU_GET_REQUEST_ACK_SIG);
                        if (reply_sig == NULL)
                        {
                            WDDI_WARNING("linx_alloc() failed\n");
                            return ERR_LINX_ALLOC;
                        }
                        reply_sig->npu_get_request_ack.cmd = NPU_GET_NS_BSSGP_STATICS_STATICS;
                        reply_sig->npu_get_request_ack.result = ret;
                        get_ns_bssgp_statics.vfport = vfport;
                        memcpy(reply_sig->npu_get_request_ack.pdata,&get_ns_bssgp_statics,sizeof(get_ns_bssgp_statics));
                        if (linx_send(linx_info->linx, &reply_sig, client) == -1)
                        {
                            WDDI_WARNING("linx_send() %s client = 0x%x", strerror(errno),client);
                            return ERR_LINX_SEND;
                        }
                        break;
                    }

                    case NPU_GET_STM1_E1T1_STATUS:
                    {
                        STRU_NPU_GET_STM1_E1T1_STATUS stm1_e1t1_status;
                        u32 stm1id;
                        stm1id = ((STRU_NPU_GET_STM1_E1T1_STATUS *)(sig->npu_request.pargv))->stm1id -1;
                        if((stm1id != 0)&&(stm1id != 1))
                        {
                            WDDI_WARNING("NPU_GET_STM1_E1T1_STATUS stm1Id %d invalid \n",stm1id);
                            break;
                        }

                        reply_sig = linx_alloc(linx_info->linx, sizeof(struct stru_npu_get_request_ack_sig)+sizeof(STRU_NPU_GET_STM1_E1T1_STATUS), NPU_GET_REQUEST_ACK_SIG);
                        if (reply_sig == NULL)
                        {
                            WDDI_WARNING("linx_alloc() failed\n");
                            return ERR_LINX_ALLOC;
                        }
                        reply_sig->npu_get_request_ack.cmd = NPU_GET_STM1_E1T1_STATUS;
                        reply_sig->npu_get_request_ack.result = STATUS_OK;
                        STRU_SET_INITAIL_CONFIG *initial_cfg = npu_get_initial_cfg_info();
                        stm1_e1t1_status.stm1id = stm1id;
                        if(initial_cfg->stru_stm1_cfg_info[stm1id].stm1Valid == NPU_VALID)
                        {
                            stm1_e1t1_status.status.stm1_status = np_check_ufe4_stm1_status(stm1id);
                        }
                        else
                        {
                            stm1_e1t1_status.status.stm1_status = LINK_STATUS_DISABLED;
                        }
                        int i;
                        for(i = 0; i < SPP_MAX_TRUNK_NUM;i++)
                        {
                            if(initial_cfg->stru_stm1_cfg_info[stm1id].trunk_info[i].trunkValid == NPU_VALID)
                            {
                                stm1_e1t1_status.status.e1t1_status[i] = np_check_ufe4_e1t1_status(stm1id,i);
                            }
                            else
                            {
                                stm1_e1t1_status.status.e1t1_status[i] = LINK_STATUS_DISABLED;
                            }
                        }
                        memcpy(reply_sig->npu_get_request_ack.pdata,&stm1_e1t1_status,sizeof(stm1_e1t1_status));
                        if (linx_send(linx_info->linx, &reply_sig, client) == -1)
                        {
                            WDDI_WARNING("linx_send() %s client = 0x%x", strerror(errno),client);
                            return ERR_LINX_SEND;
                        }
                        break;
                    }
                    case NPU_GET_TRUNK_INFO:
                    {
                        NP_TRUNK_INFO *pTrunkInfo;
                        u32 trunkID, i;
                        trunkID = ((STRU_GET_TRUNKINFO*)(sig->npu_request.pargv))->trunkID;
                        pTrunkInfo = npu_get_trunk_info(trunkID);
                        if(0 == pTrunkInfo)
                        {
                            WDDI_WARNING("Warn: NPU_GET_TRUNK_INFO, could not get trunk:%d (0x%x) info!\n",trunkID, trunkID);
                        }
                        else
                        {
                            if(trunkID != pTrunkInfo->trunkid)
                                Fprintf("Error: trunkID is different!\n");
                            Fprintf("turnk ID:%d, GroupNum:%d\n", trunkID, pTrunkInfo->GroupNum);
                            for(i = 0; i < pTrunkInfo->GroupNum; i++)
                            {
                                Fprintf("Groupid[%d]:%d\n", i, pTrunkInfo->Groupid[i]);
                            }
                        }

                        reply_sig = linx_alloc(linx_info->linx, sizeof(struct stru_npu_get_request_ack_sig), NPU_GET_REQUEST_ACK_SIG);
                        if (reply_sig == NULL)
                        {
                            WDDI_WARNING("linx_alloc() failed\n");
                            return ERR_LINX_ALLOC;
                        }
                        reply_sig->npu_get_request_ack.cmd = NPU_GET_TRUNK_INFO;
                        reply_sig->npu_get_request_ack.result = STATUS_OK;

                        if (linx_send(linx_info->linx, &reply_sig, client) == -1)
                        {
                            WDDI_WARNING("linx_send() %s client = 0x%x", strerror(errno),client);
                            return ERR_LINX_SEND;
                        }
                        break;
                    }
                    case NPU_GET_DPORT_STATISTIC_INFO:
                    {
                        u16 dportID;
                        dportID = ((STRU_GET_DPORT_STATISTICS*)(sig->npu_request.pargv))->dportID;
                        np_display_dport_flow_info(dportID);

                        reply_sig = linx_alloc(linx_info->linx, sizeof(struct stru_npu_get_request_ack_sig), NPU_GET_REQUEST_ACK_SIG);
                        if (reply_sig == NULL)
                        {
                            WDDI_WARNING("linx_alloc() failed\n");
                            return ERR_LINX_ALLOC;
                        }
                        reply_sig->npu_get_request_ack.cmd = NPU_GET_DPORT_STATISTIC_INFO;
                        reply_sig->npu_get_request_ack.result = STATUS_OK;

                        if (linx_send(linx_info->linx, &reply_sig, client) == -1)
                        {
                            WDDI_WARNING("linx_send() %s client = 0x%x", strerror(errno),client);
                            return ERR_LINX_SEND;
                        }
                        break;
                    }
                    case NPU_GET_TABLE_ITEM:
                    {
                        STRU_GET_TABLE_ITEM stru_get_tblItem = {0};
                        memcpy(&stru_get_tblItem, (void*)sig->npu_request.pargv, sizeof(STRU_GET_TABLE_ITEM));
                        np_display_table_item(&stru_get_tblItem);

                        reply_sig = linx_alloc(linx_info->linx, sizeof(struct stru_npu_get_request_ack_sig), NPU_GET_REQUEST_ACK_SIG);
                        if (reply_sig == NULL)
                        {
                            WDDI_WARNING("linx_alloc() failed\n");
                            return ERR_LINX_ALLOC;
                        }
                        reply_sig->npu_get_request_ack.cmd = NPU_GET_TRUNK_INFO;
                        reply_sig->npu_get_request_ack.result = STATUS_OK;

                        if (linx_send(linx_info->linx, &reply_sig, client) == -1)
                        {
                            LINX_CLIENT_PRINT(linx_info->linx,client,"linx_send() %s client = 0x%x", strerror(errno),client);
                            return ERR_LINX_SEND;
                        }
                        break;
                    }

                    default:
                    {
                        break;
                    }
                    }

                }
                else
                {

                }

                if(linx_free_buf(linx_info->linx,&sig) == -1)
                {
                    WDDI_WARNING("linx_free_buf() %s", strerror(errno));
                }

                /*what will do next ?*/
                break;
            }
            case ATTACH_CLIENT_SIG:
            {
                char *clientName=NULL;
                if(0==linx_get_name(linx_info->linx,client,&clientName))
                {
                    WDDI_WARNING("Server: Contact with client lost. client =0x%x name:%s \n",client,clientName);
                    linx_free_name(linx_info->linx,&clientName);
                }
                else
                {
                    LINX_CLIENT_PRINT(linx_info->linx,client,"Server: Contact with client lost. client =0x%x\n",client);

                }

                linx_free_buf(linx_info->linx,&sig);
                /*what will do next ? release all vport related resource*/
#if ENEA_RELEASE
                int ret;
                ret = np_close_vport_for_client(client);
                if(ret != STATUS_OK)
                {
                    WDDI_WARNING("np_close_vport_for_client fail client 0x%0x \n",client);
                }
#endif
                break;
            }
            case ATTACH_NPUMON_SIG:
            {
                LINX_CLIENT_PRINT(linx_info->linx,client,"Server: Contact with NPUMON lost. client =0x%x\n",client);
                linx_free_buf(linx_info->linx,&sig);
                (void)np_close_vport_for_npumon(client);
                break;
            }
            default:
            {
                WDDI_WARNING("Server: Unexpected signal received "
                             "(sig_no = %d) - ignored\n",
                             sig->sig_no);
                if (linx_free_buf(linx_info->linx, &sig) == -1)
                {
                    WDDI_WARNING("linx_free_buf() %s", strerror(errno));
                }
                break;
            }
            }
        }
    }
	return 0;
}

void npu_init_vport_info(void)
{
	int len;
	len = sizeof(struct hsearch_data);
	/* Create table; no error check now */
	ht = malloc(len);
	memset(ht,0,len);
	hcreate_r(MAX_PORTS, ht);

}

void npu_init_uplink_info(void)
{
	int len;
	len = sizeof(struct hsearch_data);
	/* Create table; no error check now */
	uplink_ht = malloc(len);
	memset(uplink_ht,0,len);
	hcreate_r(MAX_FLOWS, uplink_ht);
}

void npu_init_downlink_info(void)
{
	int len;
	len = sizeof(struct hsearch_data);
	/* Create table; no error check now */
	downlink_ht = malloc(len);
	memset(downlink_ht,0,len);
	hcreate_r(MAX_FLOWS, downlink_ht);
}
void npu_init_arp_info(void)
{
	int len;
	len = sizeof(struct hsearch_data);
	/* Create table; no error check now */
	arp_ht = malloc(len);
	memset(arp_ht,0,len);
	hcreate_r(MAX_ARPS, arp_ht);
}

void npu_init_update_bvci_info(void)
{
	int len;
	len = sizeof(struct hsearch_data);
	/* Create table; no error check now */
	update_bvci_ht = malloc(len);
	memset(update_bvci_ht,0,len);
	hcreate_r(MAX_BVCIS, update_bvci_ht);
}

void npu_init_trunk_info(void)
{
	int len;
	len = sizeof(struct hsearch_data);
	/* Create table; no error check now */
	trunk_ht = malloc(len);
	memset(trunk_ht,0,len);
	hcreate_r(MAX_TRUNKS, trunk_ht);
}

void npu_init_fr_ul_link_table_info(void)
{
	int len;
	len = sizeof(struct hsearch_data);
	/* Create table; no error check now */
	fr_ul_link_table_ht = malloc(len);
	memset(fr_ul_link_table_ht,0,len);
	hcreate_r(MAX_FLOWS, fr_ul_link_table_ht);
}

void npu_init_fr_dl_link_table_info(void)
{
	int len;
	len = sizeof(struct hsearch_data);
	/* Create table; no error check now */
	fr_dl_link_table_ht = malloc(len);
	memset(fr_dl_link_table_ht,0,len);
	hcreate_r(MAX_FLOWS, fr_dl_link_table_ht);
}
void npu_init_peer_vport_info(void)
{
	int len;
	len = sizeof(struct hsearch_data);
	/* Create table; no error check now */
	peer_vport_ht = malloc(len);
	memset(peer_vport_ht,0,len);
	hcreate_r(MAX_PORTS, peer_vport_ht);

}


int npu_set_vport_info(LINX_SPID client,u8 vp_type,u32 ipaddr_src,u32 port_src)
{

	ENTRY item={0};
	ENTRY *found_item;
	char itemstr[16]={0};
	STRU_VPORT_INFO *p_VportInfo;
	item.key = itemstr;

	sprintf(item.key,"%d",port_src);

	if(0 == hsearch_r(item,FIND,&found_item,ht))//fail, first time to add
	{
		p_VportInfo = malloc(sizeof(STRU_VPORT_INFO));
		memset(p_VportInfo,0,sizeof(STRU_VPORT_INFO));
		item.key = malloc(16);
		memset(item.key,0,16);
		sprintf(item.key,"%d",port_src);
		//WDDI_INFO("set item.key = %s \n",item.key);
		/******************donot support mutli-thread*******************/
		item.data = (void *)p_VportInfo;
		(void)hsearch_r(item, ENTER,&found_item,ht) ;
        create_ht_link(&vport_nodehead, item.key);
	}
	else
	{
	    p_VportInfo = (STRU_VPORT_INFO *)found_item->data; //already set in hash table
	}

	if(p_VportInfo->UsedFlag == USED)
	{
	    if(vp_type == VP_TYPE_FR)
	    {
			u32 card;
			u32 trunk;
			u32 group;
			u32 dlci;
			np_fraddr_unpack(port_src,&card,&trunk,&group,&dlci);
			WDDI_WARNING("warnning :npu_set_vport_info vport has been used !port = %d : S%dP%dFR%dD%d\n",htons(port_src),card,trunk,group,dlci);
	    }
		else
		{
			WDDI_WARNING("warnning :npu_set_vport_info vport has been used !port = %d\n",htons(port_src));
		}
    }
	p_VportInfo->client = client;
	p_VportInfo->vp_type = vp_type;
	p_VportInfo->ipaddr_src = ipaddr_src;
	p_VportInfo->port_src = port_src;
	//p_VportInfo->UsedFlag = USED;


    if(vp_type == VP_TYPE_FR)
    {
		/*set FR channel info*/
		u32 card;
		u32 trunk;
		u32 group;
		u32 dlci;
		np_fraddr_unpack(port_src,&card,&trunk,&group,&dlci);

		//p_VportInfo->ch_info.phy_handle = 0;
		//p_VportInfo->ch_info.device_handle = 0;
		//p_VportInfo->ch_info.rx_host_handle= 0;
		//p_VportInfo->ch_info.tx_host_handle= 0;
		p_VportInfo->ch_info.card = card;
		p_VportInfo->ch_info.vfport = port_src;
		p_VportInfo->ch_info.trunk = trunk;
		p_VportInfo->ch_info.group = group;
		p_VportInfo->ch_info.dlci = dlci;
		p_VportInfo->ch_info.slot_selection = np_get_slots_selection(card,trunk,group);

		STRU_SET_INITAIL_CONFIG *initial_cfg = npu_get_initial_cfg_info();
		p_VportInfo->ch_info.QosEn = initial_cfg->stru_stm1_cfg_info[card-1].trunk_info[trunk-1].group_info[group].PVCQos[dlci].adminState;
		p_VportInfo->ch_info.be = initial_cfg->stru_stm1_cfg_info[card-1].trunk_info[trunk-1].group_info[group].PVCQos[dlci].be;
		p_VportInfo->ch_info.bc = initial_cfg->stru_stm1_cfg_info[card-1].trunk_info[trunk-1].group_info[group].PVCQos[dlci].bc;
		p_VportInfo->ch_info.cir= initial_cfg->stru_stm1_cfg_info[card-1].trunk_info[trunk-1].group_info[group].PVCQos[dlci].cir;
#if 0
		p_VportInfo->ch_info.QosEn =1;
		p_VportInfo->ch_info.be = 0;
		p_VportInfo->ch_info.bc = 0;
		p_VportInfo->ch_info.cir= 2048;
#endif
	}
	return STATUS_OK;
}

int npu_set_uplink_info(NP_UL_FLOW_INFO* flowinfo)
{

	ENTRY item={0};
	ENTRY *found_item;
	char itemstr[30]={0};
	NP_UL_FLOW_INFO *p_FlowInfo;
	item.key = itemstr;

	sprintf(item.key,"%d%d",flowinfo->vport,flowinfo->NRI);
	if(0 == hsearch_r(item,FIND,&found_item,uplink_ht))//fail, first time to add
	{
		p_FlowInfo = malloc(sizeof(NP_UL_FLOW_INFO));
		memset(p_FlowInfo,0,sizeof(NP_UL_FLOW_INFO));
		item.key = malloc(30);
		memset(item.key,0,30);
		sprintf(item.key,"%d%d",flowinfo->vport,flowinfo->NRI);
		//WDDI_INFO("set item.key = %s \n",item.key);
		/******************donot support mutli-thread*******************/
		item.data = (void *)p_FlowInfo;
		(void)hsearch_r(item, ENTER,&found_item,uplink_ht) ;
		create_ht_link(&uplink_nodehead, item.key);
	}
	else
	{
	    p_FlowInfo = (NP_UL_FLOW_INFO *)found_item->data; //already set in hash table
	}
	if(USED == p_FlowInfo->UsedFlag)
	{
		WDDI_WARNING("warnning :npu_set_uplink_info: This item has been used! vfport = %d,NRIValue = %d\n",flowinfo->vport,flowinfo->NRI);
    }
    memcpy(p_FlowInfo,flowinfo,sizeof(NP_UL_FLOW_INFO));
	return STATUS_OK;
}

int npu_set_downlink_info(NP_DL_FLOW_INFO* flowinfo)
{

	ENTRY item={0};
	ENTRY *found_item;
	char itemstr[30]={0};
	NP_DL_FLOW_INFO *p_FlowInfo;
	item.key = itemstr;
	u32 key1 = flowinfo->rule.DportStartValue;
	u32 key2 = flowinfo->rule.BVCI;

	sprintf(item.key,"%d%d",key1,key2);
	if(0 == hsearch_r(item,FIND,&found_item,downlink_ht))//fail, first time to add
	{
		p_FlowInfo = malloc(sizeof(NP_DL_FLOW_INFO));
		memset(p_FlowInfo,0,sizeof(NP_DL_FLOW_INFO));
		item.key = malloc(30);
		memset(item.key,0,30);
		sprintf(item.key,"%d%d",key1,key2);
		//WDDI_INFO("set item.key = %s \n",item.key);
		/******************donot support mutli-thread*******************/
		item.data = (void *)p_FlowInfo;
		(void)hsearch_r(item, ENTER,&found_item,downlink_ht) ;
		create_ht_link(&downlink_nodehead, item.key);
		create_ht_link(&fr_dl_bvci_nodehead[flowinfo->rule.BVCI],item.key);
		create_ht_link(&fr_dl_dport_nodehead[flowinfo->rule.DportStartValue],item.key);
	}
	else
	{
	    p_FlowInfo = (NP_DL_FLOW_INFO *)found_item->data; //already set in hash table
	}
	if(USED == p_FlowInfo->UsedFlag)
	{
		WDDI_WARNING("warnning :npu_set_downlink_info: This item has been used! bvci = 0x%x,DportStartValue = 0x%x(%d)\n",key2,htons(key1),htons(key1));
    }
    memcpy(p_FlowInfo,flowinfo,sizeof(NP_DL_FLOW_INFO));
	return STATUS_OK;
}


int npu_set_arp_info(u32 ip, NP_ARP_INFO* arpinfo)
{

	ENTRY item={0};
	ENTRY *found_item;
	char itemstr[30]={0};
	NP_ARP_INFO *p_ArpInfo;
	item.key = itemstr;

	sprintf(item.key,"%d",ip);
	if(0 == hsearch_r(item,FIND,&found_item,arp_ht))//fail, first time to add
	{
		p_ArpInfo = malloc(sizeof(NP_ARP_INFO));
		memset(p_ArpInfo,0,sizeof(NP_ARP_INFO));
		item.key = malloc(16);
		memset(item.key,0,16);
		sprintf(item.key,"%d",ip);
		/******************donot support mutli-thread*******************/
		item.data = (void *)p_ArpInfo;
		(void)hsearch_r(item, ENTER,&found_item,arp_ht) ;
		create_ht_link(&arp_nodehead, item.key);
	}
	else
	{
	    p_ArpInfo = (NP_ARP_INFO *)found_item->data; //already set in hash table
	}
    //memcpy(p_ArpInfo,arpinfo,sizeof(NP_ARP_INFO));
    p_ArpInfo->Ip = arpinfo->Ip;
	memcpy(p_ArpInfo->Mac,arpinfo->Mac,6);
	return STATUS_OK;
}

int npu_set_update_bvci_info(u32 vfport, u16 NS_BVCI_old, NP_UPDATE_BVCI_FLOW_INFO* flowinfo,STRU_VPORT_INFO  *VportInfo)
{

	ENTRY item={0};
	ENTRY *found_item;
	char itemstr[30]={0};
	NP_UPDATE_BVCI_FLOW_INFO *p_UpdateBvciInfo;
	item.key = itemstr;

	sprintf(item.key,"%d%d",vfport,NS_BVCI_old);
	if(0 == hsearch_r(item,FIND,&found_item,update_bvci_ht))//fail, first time to add
	{
		p_UpdateBvciInfo = malloc(sizeof(NP_UPDATE_BVCI_FLOW_INFO));
		memset(p_UpdateBvciInfo,0,sizeof(NP_UPDATE_BVCI_FLOW_INFO));
		item.key = malloc(30);
		memset(item.key,0,30);
		sprintf(item.key,"%d%d",vfport,NS_BVCI_old);
		WDDI_INFO("Info: npu_set_update_bvci_info, set item.key = %s \n",item.key);
		/******************donot support mutli-thread*******************/
		item.data = (void *)p_UpdateBvciInfo;
		(void)hsearch_r(item, ENTER,&found_item,update_bvci_ht) ;
		create_ht_link(&update_bvci_nodehead, item.key);
		/*keep bvci link table for every vport*/
		create_ht_link(&VportInfo->bvci_nodehead, item.key);

	}
	else
	{
	    p_UpdateBvciInfo = (NP_UPDATE_BVCI_FLOW_INFO *)found_item->data; //already set in hash table
	}
	if(USED == p_UpdateBvciInfo->UsedFlag)
	{
		WDDI_WARNING("warnning :npu_set_update_bvci_info: This item has been used! vfport = 0x%x,NS_BVCI_old = 0x%x\n",vfport,NS_BVCI_old);
    }
    memcpy(p_UpdateBvciInfo,flowinfo,sizeof(NP_UPDATE_BVCI_FLOW_INFO));
	return STATUS_OK;
}

int npu_set_trunk_info(u32 trunkid, NP_TRUNK_INFO* trunkinfo)
{

	ENTRY item={0};
	ENTRY *found_item;
	char itemstr[30]={0};
	NP_TRUNK_INFO *p_trunkInfo;
	item.key = itemstr;

	WDDI_INFO("######set trunkid = %d \n",trunkid);
	sprintf(item.key,"%d",trunkid);
	if(0 == hsearch_r(item,FIND,&found_item,trunk_ht))//fail, first time to add
	{
		p_trunkInfo = malloc(sizeof(NP_TRUNK_INFO));
		memset(p_trunkInfo,0,sizeof(NP_TRUNK_INFO));
		item.key = malloc(16);
		memset(item.key,0,16);
		sprintf(item.key,"%d",trunkid);
		WDDI_INFO("set item.key = %s \n",item.key);
		/******************donot support mutli-thread*******************/
		item.data = (void *)p_trunkInfo;
		(void)hsearch_r(item, ENTER,&found_item,trunk_ht) ;
		create_ht_link(&trunk_nodehead, item.key);
	}
	else
	{
	    p_trunkInfo = (NP_TRUNK_INFO *)found_item->data; //already set in hash table
	}

    memcpy(p_trunkInfo,trunkinfo,sizeof(NP_TRUNK_INFO));
	return STATUS_OK;
}

int npu_set_fr_ul_link_table_info(STRU_SET_UPLINK_PATH_LINK_TABLE* linkInfo)
{

	ENTRY item={0};
	ENTRY *found_item;
	char itemstr[30]={0};
	STRU_SET_UPLINK_PATH_LINK_TABLE *p_LinkInfo;
	item.key = itemstr;

	sprintf(item.key,"%d",linkInfo->PortSrc);
	if(0 == hsearch_r(item,FIND,&found_item,fr_ul_link_table_ht))
	{
		p_LinkInfo = malloc(sizeof(STRU_SET_UPLINK_PATH_LINK_TABLE));
		memset(p_LinkInfo,0,sizeof(STRU_SET_UPLINK_PATH_LINK_TABLE));
		item.key = malloc(30);
		memset(item.key,0,30);
		sprintf(item.key,"%d",linkInfo->PortSrc);
		WDDI_INFO("Info: npu_set_fr_ul_link_table_info, set item.key: %s \n",item.key);
		/******************donot support mutli-thread*******************/
		item.data = (void *)p_LinkInfo;
		(void)hsearch_r(item, ENTER,&found_item,fr_ul_link_table_ht) ;
		create_ht_link(&fr_ul_link_table_nodehead, item.key);
	}
	else
	{
	    p_LinkInfo = (STRU_SET_UPLINK_PATH_LINK_TABLE *)found_item->data; //already set in hash table
	}
    memcpy(p_LinkInfo,linkInfo,sizeof(STRU_SET_UPLINK_PATH_LINK_TABLE));
	return STATUS_OK;
}


int npu_set_fr_dl_link_table_info(STRU_SET_FR_DOWNLINK_PATH_LINK_TABLE* linkInfo)
{

	ENTRY item={0};
	ENTRY *found_item;
	char itemstr[30]={0};
	STRU_SET_FR_DOWNLINK_PATH_LINK_TABLE *p_LinkInfo;
	item.key = itemstr;

	sprintf(item.key,"%d",linkInfo->NSEI);
	if(0 == hsearch_r(item,FIND,&found_item,fr_dl_link_table_ht))
	{
		p_LinkInfo = malloc(sizeof(STRU_SET_FR_DOWNLINK_PATH_LINK_TABLE));
		memset(p_LinkInfo,0,sizeof(STRU_SET_FR_DOWNLINK_PATH_LINK_TABLE));
		item.key = malloc(30);
		memset(item.key,0,30);
		sprintf(item.key,"%d",linkInfo->NSEI);
		WDDI_INFO("Info: npu_set_fr_dl_link_table_info, set item.key = %s \n",item.key);
		/******************donot support mutli-thread*******************/
		item.data = (void *)p_LinkInfo;
		(void)hsearch_r(item, ENTER,&found_item,fr_dl_link_table_ht) ;
		create_ht_link(&fr_dl_link_table_nodehead, item.key);
	}
	else
	{
	    p_LinkInfo = (STRU_SET_FR_DOWNLINK_PATH_LINK_TABLE *)found_item->data; //already set in hash table
	}
    memcpy(p_LinkInfo,linkInfo,sizeof(STRU_SET_FR_DOWNLINK_PATH_LINK_TABLE));
	return STATUS_OK;
}

int npu_dl_link_table_delete(WP_U32 vfort)
{
    NODE *temp;
	ENTRY item={0};
	ENTRY *found_item;
	STRU_SET_FR_DOWNLINK_PATH_LINK_TABLE *p_linkInfo;
	STRU_SET_FR_DOWNLINK_PATH_LINK_TABLE NewlinkInfo;
	u32 i = 0,j = 0;
	int status;
    temp=fr_dl_link_table_nodehead;
	memset(&NewlinkInfo, 0, sizeof(STRU_SET_FR_DOWNLINK_PATH_LINK_TABLE));
    while(temp!=NULL)
    {
        item.key = temp->ht_key;
        temp=temp->next;

		if(0 == hsearch_r(item,FIND,&found_item,fr_dl_link_table_ht))
		{
			WDDI_WARNING("When np_dl_link_table_delete failed\n");
		}
		else
		{
			p_linkInfo = (STRU_SET_FR_DOWNLINK_PATH_LINK_TABLE *)found_item->data;
            for(i = 0; i< p_linkInfo->FrVportAcitveNum; i ++)
            {
                if(vfort == p_linkInfo->NsvcGrp[i].vfort)
                {
                    break;
				}
			}
			if(i < p_linkInfo->FrVportAcitveNum)
			{
                NewlinkInfo.NSEI = p_linkInfo->NSEI;
                for(i = 0; i< p_linkInfo->FrVportAcitveNum; i ++)
                {
                    if(vfort != p_linkInfo->NsvcGrp[i].vfort)
                    {
                        NewlinkInfo.FrVportAcitveNum ++;
						NewlinkInfo.NsvcGrp[j].Flag = p_linkInfo->NsvcGrp[i].Flag;
						NewlinkInfo.NsvcGrp[j].vfort = p_linkInfo->NsvcGrp[i].vfort;
						j ++;
					}
				}
                status = np_dl_link_table_set(&NewlinkInfo);
				if(0 != status)
				{
                    return status;
				}
			}
		}
    }
    return 0;
}

int npu_clear_vport_info(u32 port_src,u8 vp_type)
{
	ENTRY item={0};
	ENTRY *found_item;
	char itemstr[30];
	STRU_VPORT_INFO *p_VportInfo;
	item.key = itemstr;
	int status;
    sprintf(item.key,"%d",port_src);
	WDDI_INFO("Info: npu_clear_vport_info, item.key:%s\n",item.key);
	//if((found_item = hsearch(item,FIND)) == NULL)
	if(0 == hsearch_r(item,FIND,&found_item,ht))//fail, first time to add
	{
		WDDI_WARNING("Warn: npu_clear_vport_info, When clear vport,hash not found port_src: %d\n",port_src);
		return -1;
	}
	p_VportInfo = (STRU_VPORT_INFO *)found_item->data;
	if(p_VportInfo->UsedFlag == UNUSED)
	{
		WDDI_WARNING("Warn: npu_clear_vport_info, When clear vport,vport unused: %d\n",port_src);
		return -1;
	}

    u32 nri=0;

    while(p_VportInfo->nri_valid_talbe[nri] !=0)/*nri_valid_talbe[0~255] 0 not valid ,nri= nri_valid_talbe[i]-1*/
    {
		(void)np_ul_route_table_delete(port_src,p_VportInfo->nri_valid_talbe[nri]-1);
		nri++;
		if(nri>256)break;/*max 256 NRI*/
	}
    memset(p_VportInfo->nri_valid_talbe,0,sizeof(p_VportInfo->nri_valid_talbe));

	if(vp_type == VP_TYPE_UDP)
	{
	    (void)np_ul_link_table_delete(port_src);
	}

	/*delete bvci table*/
	status = np_updateBVCI_flow_delete(port_src,0,DEL_UL_UPDATE_BVCI_TALBE_BASED_ON_PORT);
	if(status != STATUS_OK)
	{
		WDDI_WARNING("npu_clear_vport_info np_updateBVCI_flow_delete error");
		//return status;
	}
	if(vp_type == VP_TYPE_UDP)
	{
		/*delete DL route table,only for UDP type vport*/
		STRU_SET_INITAIL_CONFIG *initial_cfg = npu_get_initial_cfg_info();
		u32 Dport;
		extern WP_handle DportGrpflow_handle[];
		extern DportGrpaggInfo DportGrpagg_handle[];
		for(Dport = 0;Dport<65535;Dport++)
		{
			if(DportGrpflow_handle[Dport]!= 0)
			{
				if(htons(port_src) >= htons(Dport) && htons(port_src) <= (htons(Dport) + DportGrpagg_handle[Dport].PortSize-1))
				{
					/*Fprintf("npu_clear_vport_info np_dl_route_table_delete port_src = 0x%x , Dport = 0x%x \n",port_src,Dport);*/
					status = np_dl_route_table_delete(Dport,0,DEL_DL_RULE_TALBE_BASED_ON_DPORT);
					/*release FMC related resource*/
					np_dl_release_FMC_resource(Dport);	/*rebuild system will take a long time*/
					break;
				}
			}
		}
	}
	if(vp_type == VP_TYPE_FR)
	{
        status = npu_dl_link_table_delete(port_src);
		if(status != STATUS_OK)
		{
			WDDI_WARNING("npu_dl_link_table_delete() status error");
			//return status;
		}
	}

	status =  np_ul_release_vport_resource(port_src);
	if(status != STATUS_OK)
	{
		WDDI_WARNING("np_ul_release_vport_resource() status error");
		//return status;
	}

	status =  np_dl_release_vport_resource(port_src);
	if(status != STATUS_OK)
	{
		WDDI_WARNING("np_dl_release_vport_resource() status error");
		//return status;
	}

	if(p_VportInfo->UsedFlag == USED)
	{
		p_VportInfo->UsedFlag = UNUSED;
	}
	else
	{
		WDDI_WARNING("Warn: npu_clear_vport_info, vport have been set to unused !port = %d \n",port_src);
		//return -1;
	}
	if(vp_type == VP_TYPE_FR)
	{

	}

	return STATUS_OK;
}

STRU_VPORT_INFO * npu_get_vport_info(u32 port_src)
{
	ENTRY item={0};
	ENTRY *found_item;
	char itemstr[30];
	item.key = itemstr;
    sprintf(item.key,"%d",port_src);
    //item.key = "10000";
	//WDDI_INFO("get item.key = %s \n",item.key);
	//if((found_item = hsearch(item,FIND)) == NULL)
	if(0 == hsearch_r(item,FIND,&found_item,ht))//fail, first time to add
	{
#if ENEA_RELEASE
        WDDI_INFO("Info: npu_get_vport_info fail. port_src: %ld\n",port_src);
#else
        WDDI_WARNING("Warn: npu_get_vport_info fail. port_src: %ld\n",port_src);
#endif
	    return NULL;
	}
	else
	{   STRU_VPORT_INFO *p_VportInfo;
	    p_VportInfo = (STRU_VPORT_INFO *)found_item->data;
	    if(((STRU_VPORT_INFO *)found_item->data)->UsedFlag == USED)
	    {
		}
		else
		{
		    if(((STRU_VPORT_INFO *)found_item->data)->vp_type == VP_TYPE_FR)
		    {
				u32 card;
				u32 trunk;
				u32 group;
				u32 dlci;
				np_fraddr_unpack(p_VportInfo->port_src,&card,&trunk,&group,&dlci);
				WDDI_WARNING("Warn: npu_get_vport_info::vport unused ! port = 0x%x : S%dP%dFR%dD%d\n",port_src,
                             card,trunk,group,dlci);
		    }
			else
			{
#if ENEA_RELEASE
				WDDI_INFO("Warn: npu_get_vport_info::vport unused ! port = %d\n",port_src);
#else
				WDDI_WARNING("npu_get_vport_info failed vport unused ! port = %d\n",port_src);
#endif

			}
		}
		return (STRU_VPORT_INFO *)found_item->data;


	}
}

NP_UL_FLOW_INFO * npu_get_uplink_info(u32 vfport,u32 NRIValue)
{
	ENTRY item={0};
	ENTRY *found_item;
	char itemstr[30];
	item.key = itemstr;
	sprintf(item.key,"%d%d",vfport,NRIValue);
	//WDDI_INFO("get item.key = %s \n",item.key);
	if(0 == hsearch_r(item,FIND,&found_item,uplink_ht))//fail, first time to add
	{
		//WDDI_WARNING("When get uplink info,hash not found vfport: %ld, NRIValue: %ld\n",vfport,NRIValue);
	    return NULL;
	}
    else
    {
        if(((NP_UL_FLOW_INFO *)found_item->data)->UsedFlag == USED)
        {
		}
		else
		{
			//WDDI_WARNING("npu_get_uplink_info failed! This item is unused ! vfport = %d,NRIValue = %d\n",vfport,NRIValue);
		}
		return (NP_UL_FLOW_INFO *)found_item->data;
	}
}

NP_DL_FLOW_INFO * npu_get_downlink_info(u16 DportStartValue,u16 bvci)
{
	ENTRY item={0};
	ENTRY *found_item;
	char itemstr[30];
	item.key = itemstr;
	sprintf(item.key,"%d%d",DportStartValue,bvci);
	WDDI_INFO("Info: npu_get_downlink_info, item.key:%s \n",item.key);
	if(0 == hsearch_r(item,FIND,&found_item,downlink_ht))//fail, first time to add
	{
		//WDDI_WARNING("When get downlink info,hash not found DportStartValue: 0x%x ns_bvci = 0x%x\n",DportStartValue,bvci);
	    return NULL;
	}
	else
	{
        if(((NP_DL_FLOW_INFO *)found_item->data)->UsedFlag == USED)
        {
		}
		else
		{
			WDDI_WARNING("npu_get_downlink_info failed! This item is unused ! DportStartValue = 0x%x,ns_bvci = 0x%x\n",DportStartValue,bvci);
		}
		return (NP_DL_FLOW_INFO *)found_item->data;
	}
}

NP_DL_FLOW_INFO * npu_get_dl_route_table_info(char * key)
{
	ENTRY item={0};
	ENTRY *found_item;
	NP_DL_FLOW_INFO *p_EthFlowInfo;
	char itemstr[30];
	item.key = itemstr;
	sprintf(item.key,"%s",key);
	if(0 == hsearch_r(item,FIND,&found_item,downlink_ht))
	{
	    return NULL;
	}
	else
	{
		p_EthFlowInfo = (NP_DL_FLOW_INFO *)found_item->data;

        return p_EthFlowInfo;

	}
	return NULL;
}

NP_UPDATE_BVCI_FLOW_INFO * npu_get_ul_bvci_table_info(char * key)
{
	ENTRY item={0};
	ENTRY *found_item;
	NP_UPDATE_BVCI_FLOW_INFO *p_FlowInfo;
	char itemstr[30];
	item.key = itemstr;
	sprintf(item.key,"%s",key);
	if(0 == hsearch_r(item,FIND,&found_item,update_bvci_ht))
	{
	    return NULL;
	}
	else
	{
		p_FlowInfo = (NP_UPDATE_BVCI_FLOW_INFO *)found_item->data;

        return p_FlowInfo;

	}
	return NULL;
}


STRU_SET_UPLINK_PATH_LINK_TABLE * npu_get_ul_link_table_info(u32 PortSrc)
{
	ENTRY item={0};
	ENTRY *found_item;
	STRU_SET_UPLINK_PATH_LINK_TABLE *p_linkTable;
	char itemstr[30] = {0};
	item.key = itemstr;
	sprintf(item.key,"%d",PortSrc);
	if(0 == hsearch_r(item,FIND,&found_item,fr_ul_link_table_ht))
	{
        WDDI_INFO("Info: npu_get_ul_link_table_info, get ul-link fail, key:%s\n", item.key);
	    return NULL;
	}
	else
	{
		p_linkTable = (STRU_SET_UPLINK_PATH_LINK_TABLE *)found_item->data;
		return p_linkTable;
	}
}

STRU_SET_FR_DOWNLINK_PATH_LINK_TABLE * npu_get_dl_link_table_info(u16 NSEI)
{
	ENTRY item={0};
	ENTRY *found_item;
	STRU_SET_FR_DOWNLINK_PATH_LINK_TABLE *p_linkTable;
	char itemstr[30];
	item.key = itemstr;
	sprintf(item.key,"%d",NSEI);
	if(0 == hsearch_r(item,FIND,&found_item,fr_dl_link_table_ht))
	{
	    return NULL;
	}
	else
	{
		p_linkTable = (STRU_SET_FR_DOWNLINK_PATH_LINK_TABLE *)found_item->data;
		return p_linkTable;
	}
}

NP_ARP_INFO * npu_get_arp_info(u32 ip)
{
	ENTRY item={0};
	ENTRY *found_item;
	char itemstr[30];
	item.key = itemstr;
	sprintf(item.key,"%d",ip);
	//WDDI_INFO("get item.key = %s \n",item.key);
	if(0 == hsearch_r(item,FIND,&found_item,arp_ht))//fail, first time to add
	{
		WDDI_WARNING("When get arp info,hash not found ip: %ld\n",ip);
	    return NULL;
	}
	else
	{
	    return (NP_ARP_INFO *)found_item->data;
	}
}

NP_UPDATE_BVCI_FLOW_INFO * npu_get_update_bvci_info(u32 vfport, u16 NS_BVCI_old)
{
	ENTRY item={0};
	ENTRY *found_item;
	char itemstr[30];
	item.key = itemstr;
	sprintf(item.key,"%d%d",vfport,NS_BVCI_old);
	//WDDI_INFO("get item.key = %s \n",item.key);
	if(0 == hsearch_r(item,FIND,&found_item,update_bvci_ht))//fail, first time to add
	{
		WDDI_WARNING("When get upgrade bvci info,hash not found vfport: %ld NS_BVCI_old: %d\n",vfport,NS_BVCI_old);
	    return NULL;
	}
	else
	{
	    return (NP_UPDATE_BVCI_FLOW_INFO *)found_item->data;
	}
}

NP_TRUNK_INFO * npu_get_trunk_info(u32 Trunkid)
{
	ENTRY item={0};
	ENTRY *found_item;
	char itemstr[30];
	item.key = itemstr;
	sprintf(item.key,"%d",Trunkid);
	//WDDI_INFO("get item.key = %s \n",item.key);
	if(0 == hsearch_r(item,FIND,&found_item,trunk_ht))//fail, first time to add
	{
		WDDI_WARNING("When get trunk info,hash not found Trunkid: %ld\n",Trunkid);
	    return NULL;
	}
	else
	{
	    return (NP_TRUNK_INFO *)found_item->data;
	}
}

int npu_send_udp_packet(u8* p, u32 len,u32 ipaddr_src,u32 ipaddr_dst,u32 port_src, u32 port_dst)
{

    STRU_NPU_ETH_VLAN_HDR *pStruEthVlanHdr = NULL;
    STRU_NPU_ETH_HDR *pStruEthHdr = NULL;
    STRU_NPU_IP_HDR *pStruIpv4Hdr = NULL;
    STRU_NPU_UDP_HDR *pStruUdpHdr = NULL;
    u8 *pu8UdpData = NULL;
    char ArraySendBuf[NPU_UDP_ETHPACK_LEN_MAX+NPU_UDP_ETHPACK_PROTECTION];
    u32 u32i;
    u32 tmp_flag = 0;

    u32 u32SendDataLen;
    u16 u16ChkSum;
    u8 mac_dst[6]={0x01,0x02,0x03,0x04,0x05,0x06};
    u8 mac_src[6]={0x11,0x12,0x13,0x14,0x15,0x16};
    int ret;
    STRU_SET_INITAIL_CONFIG *initial_cfg = npu_get_initial_cfg_info();

    memset((void *)ArraySendBuf, 0, sizeof(u8) * NPU_UDP_ETHPACK_LEN_MAX);
    (void)tap_socket_gethwaddr(mac_src);
    /*vlan enabled*/
    if(initial_cfg->VlanFlag == NPU_VLAN_ENABLE)
    {
        pStruEthVlanHdr = (STRU_NPU_ETH_VLAN_HDR *)ArraySendBuf;

        for (u32i = 0; u32i < 6; u32i++)
        {
            pStruEthVlanHdr->ether_dhost[u32i] = mac_dst[u32i];

            pStruEthVlanHdr->ether_shost[u32i] = mac_src[u32i];;
        }
        pStruEthVlanHdr->ether_type_tag = htons(NPU_UDP_VLAN_TYPE_TAG);
        STRU_SET_INITAIL_CONFIG *initial_cfg = npu_get_initial_cfg_info();

        pStruEthVlanHdr->tag = htons(initial_cfg->VlanTag);
        pStruEthVlanHdr->ether_type = htons(NPU_UDP_ETH_TYPE_IPV4);
        u32SendDataLen = sizeof(STRU_NPU_ETH_VLAN_HDR) + sizeof(STRU_NPU_IP_HDR) +
            sizeof(STRU_NPU_UDP_HDR) + len;
        /*ipv4 header*/
        pStruIpv4Hdr = (STRU_NPU_IP_HDR *)(ArraySendBuf + sizeof(STRU_NPU_ETH_VLAN_HDR));


    }
    else
    {
        pStruEthHdr = (STRU_NPU_ETH_HDR *)ArraySendBuf;
        for (u32i = 0; u32i < NPU_UDP_MAC_ADDR_LEN; u32i++)
        {
            pStruEthHdr->ether_dhost[u32i] = mac_dst[u32i];

            pStruEthHdr->ether_shost[u32i] = mac_src[u32i];;
        }
        pStruEthHdr->ether_type = htons(NPU_UDP_ETH_TYPE_IPV4);
        u32SendDataLen = sizeof(STRU_NPU_ETH_HDR) + sizeof(STRU_NPU_IP_HDR) +
            sizeof(STRU_NPU_UDP_HDR) + len;
        /*ipv4 header*/
        pStruIpv4Hdr = (STRU_NPU_IP_HDR *)(ArraySendBuf + sizeof(STRU_NPU_ETH_HDR));
    }

    pStruIpv4Hdr->ip_vhl = NPU_UDP_IPV4_VER;							  /* Version			 */

    pStruIpv4Hdr->ip_tos = NPU_UDP_IPV4_TOS;							  /* Type Of Service	 */

    pStruIpv4Hdr->ip_len = htons(sizeof(STRU_NPU_IP_HDR) +

                                 sizeof(STRU_NPU_UDP_HDR) +

                                 len);				                          /* Total Length		 */

    pStruIpv4Hdr->ip_id  = htons(NPU_UDP_IPV4_ID);							  /* Identification 	 */

    pStruIpv4Hdr->ip_off = htons(NPU_UDP_IPV4_OFF);							  /* Offset 			 */

    pStruIpv4Hdr->ip_ttl = NPU_UDP_IPV4_TTL;							  /* TTL				 */

    pStruIpv4Hdr->ip_p   = NPU_UDP_IPV4_PRO_UDP;						  /* Protocal-UDP		 */

    pStruIpv4Hdr->ip_sum = htons(NPU_UDP_IPV4_CKSUM); 						  /* Header Checksum	 */


    pStruIpv4Hdr->ip_src = ipaddr_src;
    pStruIpv4Hdr->ip_dst = ipaddr_dst;

    u16ChkSum = npu_udp_cal_chksum((u8 *)pStruIpv4Hdr, (u32)sizeof(STRU_NPU_IP_HDR));
    pStruIpv4Hdr->ip_sum = u16ChkSum;

    /*UDP header*/
    pStruUdpHdr = (STRU_NPU_UDP_HDR *)((u8 *)pStruIpv4Hdr + sizeof(STRU_NPU_IP_HDR));


    pStruUdpHdr->uh_dport = port_dst;

    pStruUdpHdr->uh_sport = port_src;

    pStruUdpHdr->uh_ulen  = htons(sizeof(STRU_NPU_UDP_HDR) + len);

    pStruUdpHdr->uh_sum   = htons(NPU_UDP_CHKSUM);
    /*UDP data*/
    pu8UdpData =(u8 *)pStruUdpHdr + sizeof(STRU_NPU_UDP_HDR);


    if (len > 1490)
    {
        WDDI_WARNING("###before memcpy: len=%d, ip_src=%0x, ip_dst=%0x, p_src=%d, p_dst=%d\n", len,ipaddr_src, ipaddr_dst, port_src,  port_dst);
        tmp_flag = 1;
    }

    memcpy(pu8UdpData,p,len);

    if (tmp_flag)
    {
        WDDI_WARNING("### after memcpy: len=%d, ip_src=%0x, ip_dst=%0x, p_src=%d, p_dst=%d\n", len,ipaddr_src, ipaddr_dst, port_src,  port_dst);
    }


    /*np_eth_host_send*/
#if 0
    NPU_HOST_RSV_PACKET_INFO StruRsvPacketInfo;
    StruRsvPacketInfo.vp_type = VP_TYPE_UDP;
    StruRsvPacketInfo.port = port_src;
    StruRsvPacketInfo.port_src = port_dst;
    StruRsvPacketInfo.ipaddr_src = ipaddr_dst;
    StruRsvPacketInfo.length = len;
    StruRsvPacketInfo.pdata = pu8UdpData;
    int ret;
    ret = npu_dispatch_packet_to_client(&StruRsvPacketInfo);
    if(ret < 0)Fprintf("npu_dispatch_packet_to_client ret = %d",ret);
#else
    WDDI_INFO("npu_send_udp_packet::ipaddr_dst=0x%x,ipaddr_src=0x%x,port_dst=0x%x,port_src=0x%x,len=0x%x,p=0x%x\n",ipaddr_dst,ipaddr_src,port_dst,port_src,len,p);
    ret = tap_socket_getarp(ipaddr_dst, ArraySendBuf);
    if(ret != -1)
    {
        np_eth_host_send(u32SendDataLen, ArraySendBuf);
    }
    else
    {
        /*this will trig arp learning process*/
        (void)tap_socket_send(ipaddr_dst, port_dst,ipaddr_src,port_src, p, len);
        //(void)tap_packet_write(ArraySendBuf,u32SendDataLen);
    }
    /*capture all data  to this vport ,no matter send to tap or np_host*/
    npu_mon_capture_packet(ArraySendBuf,u32SendDataLen,port_src,1);

#endif
    return STATUS_OK;
}
u16 npu_udp_cal_chksum(u8 *pu8ChkData, u32 u32ChkDataLen)

{

    u32 u32Sum;

    u16 u16ChkSum;

    for (u32Sum = 0; u32ChkDataLen > 1; u32ChkDataLen -= 2)

    {
        u32Sum += (u32)(*((u16 *)(void *)pu8ChkData));

        pu8ChkData += 2;
    }

    if (1 == u32ChkDataLen)

    {
        u32Sum += htons((u16)((*pu8ChkData) & 0xff) << 8);
    }

    while (0 != (u32Sum >> 16))
    {
        u32Sum = (u32Sum & 0xffff) + (u32Sum >> 16);
    }

    u16ChkSum = (u16)(~(u32Sum & 0xffff));

    return u16ChkSum;

}


int npu_dispatch_packet_to_client(NPU_HOST_RSV_PACKET_INFO *StruRsvPacketInfo)
{

    //(void)npu_host_packet_receive(&StruRsvPacketInfo,buffer);
    union LINX_SIGNAL *sig;
    STRU_LINX_SERVER_INFO *linx_info;
    STRU_VPORT_INFO * VportInfo;

    /*get client vport info according port*/
    // Fprintf("StruRsvPacketInfo->port = %d\n",StruRsvPacketInfo->port);
    VportInfo = npu_get_vport_info(StruRsvPacketInfo->port);
    if(VportInfo == NULL)
    {
        return ERR_GET_VPORT_INFO;
    }else if(VportInfo->UsedFlag == UNUSED)/*drop this udp packet when port is closed*/
    {
        return ERR_GET_VPORT_INFO;
    }
    if(StruRsvPacketInfo->vp_type == VP_TYPE_FR)
    {
        linx_info = npu_get_server_rx_fr_thread_linx_info();

    }
    else
    {
        linx_info = npu_get_server_rx_eth_thread_linx_info();

    }
    if(linx_info == NULL)
    {
        return ERR_GET_LINX_INFO;
    }

    sig = linx_alloc(linx_info->linx, sizeof(struct stru_spp_receive_sig) + StruRsvPacketInfo->length, SPP_RECEIVE_SIG);
    if (sig == NULL)
    {
        WDDI_WARNING("linx_alloc() %s", strerror(errno));
        return ERR_LINX_ALLOC;
    }
    sig->spp_receive.length =  StruRsvPacketInfo->length;
    sig->spp_receive.ipaddr_dst = StruRsvPacketInfo->ipaddr_dst;
    sig->spp_receive.ipaddr_src = StruRsvPacketInfo->ipaddr_src;
    sig->spp_receive.port = StruRsvPacketInfo->port;
    sig->spp_receive.port_src = StruRsvPacketInfo->port_src;
    sig->spp_receive.vp_type = StruRsvPacketInfo->vp_type;
    //sig->udp_receive.pdata = (u8 *)&sig->udp_receive.pdata;
	//	Fprintf(" pdata = 0x%x 0x%x 0x%x 0x%x \n",StruRsvPacketInfo->pdata[0],StruRsvPacketInfo->pdata[1],StruRsvPacketInfo->pdata[2],StruRsvPacketInfo->pdata[3]);

    memcpy(sig->spp_receive.pdata,StruRsvPacketInfo->pdata,sig->spp_receive.length);

    //Fprintf("VportInfo->client = 0x%x port= 0x%x\n",VportInfo->client,StruRsvPacketInfo->port);
    if (linx_send(linx_info->linx, &sig, VportInfo->client) == -1)
    {
        WDDI_WARNING("linx_send() %s client = 0x%x", strerror(errno),VportInfo->client);
        return ERR_LINX_SEND;
    }
    return STATUS_OK;
}


void npu_set_server_linx_info(LINX * linx)
{
    g_LinxSeverInfo.linx = linx;
    g_LinxSeverInfo.UsedFlag = USED;
}

void npu_set_server_rx_fr_thread_linx_info(LINX * linx)
{
    g_LinxSeverRxFrthreadInfo.linx = linx;
    g_LinxSeverRxFrthreadInfo.UsedFlag = USED;
}

void npu_set_server_rx_eth_thread_linx_info(LINX * linx)
{
    g_LinxSeverRxEththreadInfo.linx = linx;
    g_LinxSeverRxEththreadInfo.UsedFlag = USED;
}


STRU_LINX_SERVER_INFO * npu_get_server_linx_info(void)
{
    if(g_LinxSeverInfo.UsedFlag == USED)
    {
        return &g_LinxSeverInfo;
	}
	else
	{
 		WDDI_WARNING("npu_get_server_linx_info  failed linx unused !");
		return NULL;
	}
}

STRU_LINX_SERVER_INFO * npu_get_server_rx_fr_thread_linx_info(void)
{
    if(g_LinxSeverRxFrthreadInfo.UsedFlag == USED)
    {
        return &g_LinxSeverRxFrthreadInfo;
	}
	else
	{
 		WDDI_WARNING("npu_get_server_rx_fr_thread_linx_info  failed linx unused !");
		return NULL;
	}
}

STRU_LINX_SERVER_INFO * npu_get_server_rx_eth_thread_linx_info(void)
{
    if(g_LinxSeverRxEththreadInfo.UsedFlag == USED)
    {
        return &g_LinxSeverRxEththreadInfo;
	}
	else
	{
 		WDDI_WARNING("npu_get_server_rx_eth_thread_linx_info  failed linx unused !");
		return NULL;
	}
}

u32 npu_set_initial_cfg_info(char *filename, STRU_SET_INITAIL_CONFIG *np_configure_ptr)
{
    /*gloab initial for test */
#if 1
	STRU_SET_INITAIL_CONFIG *p =np_configure_ptr;
	//if(0 != expat_parser(filename,  p))
	if(0 != cli_apply_configuration_files_from_dir(filename,  p))
	{
        WDDI_ERR("cant' read xml files \n");
        return -1;
	}

	else
	{
	}
#endif
#if 0
	STRU_SET_INITAIL_CONFIG *p =np_configure_ptr;

	p->ifIpaddr = 0x0aaa21c1;/*10.170.33.193*/
	p->ifIpNetMask = 0xffffff80; /*255.255.255.128*/
	p->VlanFlag = NPU_VLAN_ENABLE;
	p->ifMACsrc[0] = 0x00;
	p->ifMACsrc[1] = 0x01;
	p->ifMACsrc[2] = 0x02;
	p->ifMACsrc[3] = 0x03;
	p->ifMACsrc[4] = 0x04;
	p->ifMACsrc[5] = 0x05;
	p->NRI_bitLen = 8;
	p->VlanTag =  133;
	p->stm1_num = 1;
	int i;
	for(i=1; i< 3; i++)
    {
		p->stru_stm1_cfg_info[0].stm1Valid = NPU_VALID;
		p->stru_stm1_cfg_info[0].E1T1type = NPU_MODE_E1;
		p->stru_stm1_cfg_info[0].Opt_Mode = OPMODE_E1_TRUE_CRC4;
		p->stru_stm1_cfg_info[0].trunk_info[i].trunkValid = NPU_VALID;
		p->stru_stm1_cfg_info[0].trunk_info[i].group_info[0].groupValid = NPU_VALID;
		p->stru_stm1_cfg_info[0].trunk_info[i].group_info[0].slot_selection = /*0xfffffffe*/0xfffffffe;
		p->stru_stm1_cfg_info[0].trunk_info[i].group_info[0].max_frame_size =1600;
		p->stru_stm1_cfg_info[0].trunk_info[i].group_info[0].crc_type =NPU_E1T1_CRC16;


    }
#endif
#if 0

	p->stru_stm1_cfg_info[0].stm1Valid = NPU_VALID;
	p->stru_stm1_cfg_info[0].E1T1type = NPU_MODE_E1;
	p->stru_stm1_cfg_info[0].trunk_info[1].Opt_Mode = OPMODE_E1_TRUE_CRC4;
	p->stru_stm1_cfg_info[0].trunk_info[1].trunkValid = NPU_VALID;
	p->stru_stm1_cfg_info[0].trunk_info[1].group_info[0].groupValid = NPU_VALID;
	p->stru_stm1_cfg_info[0].trunk_info[1].group_info[0].slot_selection = /*0xfffffffe*/0xfffffffe;
	p->stru_stm1_cfg_info[0].trunk_info[1].group_info[0].max_frame_size =1600;
	p->stru_stm1_cfg_info[0].trunk_info[1].group_info[0].crc_type =NPU_E1T1_CRC16;

	p->stru_stm1_cfg_info[0].stm1Valid = NPU_VALID;
	p->stru_stm1_cfg_info[0].E1T1type = NPU_MODE_E1;
	p->stru_stm1_cfg_info[0].trunk_info[22].Opt_Mode = OPMODE_E1_TRUE_CRC4;
	p->stru_stm1_cfg_info[0].trunk_info[22].trunkValid = NPU_VALID;
	p->stru_stm1_cfg_info[0].trunk_info[22].group_info[0].groupValid = NPU_VALID;
	p->stru_stm1_cfg_info[0].trunk_info[22].group_info[0].slot_selection = /*0xfffffffe*/0xfffffffe;
	p->stru_stm1_cfg_info[0].trunk_info[22].group_info[0].max_frame_size =1600;
	p->stru_stm1_cfg_info[0].trunk_info[22].group_info[0].crc_type =NPU_E1T1_CRC16;
	p->OpterateMode = NPU_MODE_FR;
	p->PortPoolSize = 3;
	p->routeInfo[0].IpDst = htonl(0xdc21aa0a);/*10.170.33.220*/;
	p->routeInfo[0].GatewayIP = htonl(0xdc21aa0a);/*10.170.33.220*/;

#endif
	return 0;
}

STRU_SET_INITAIL_CONFIG * npu_get_initial_cfg_info(void)
{
    return &g_npinitial;
}
STRU_SET_INITAIL_CONFIG * npu_get_np_reconfig(void)
{

    return &g_np_reconfig;
}

int np_get_slots_selection(u32 card,u32 trunk,u32 group)
{
    u32 stm1Port = card-1;
	return g_npinitial.stru_stm1_cfg_info[stm1Port].trunk_info[trunk-1].group_info[group].slot_selection;
}


void create_ht_link(NODE **nodehead,char *key)
{
    NODE *p,*s;

	if(NULL == *nodehead)
	{
		*nodehead = ( NODE*)malloc(sizeof(NODE));
		(*nodehead)->ht_key = key;
		(*nodehead)->next = NULL;
	}
	else
	{
		s = *nodehead;
		while(NULL != s->next)
		{
			s = s->next;
		}

		p = (NODE *)malloc(sizeof(NODE));
		p->ht_key = key;
		p->next = NULL;

		s->next= p;
	}
}

void display_vport_ht_link(void)
{
    NODE *temp;
	ENTRY item={0};
	ENTRY *found_item;
	STRU_VPORT_INFO *p_VportInfo;
    u32 i = 0;

    temp=vport_nodehead;

    while(temp!=NULL)
    {
        item.key = temp->ht_key;
        temp=temp->next;

		if(0 == hsearch_r(item,FIND,&found_item,ht))
		{
			WDDI_WARNING("When display_vport_ht_link failed\n");
		}
		else
		{

			p_VportInfo = (STRU_VPORT_INFO *)found_item->data; //already set in hash table

            if(USED == p_VportInfo->UsedFlag)
            {
                i++;
				Fprintf("			 ####item %d#####  \n",i);
			    Fprintf("/*****************vport=0x%x (%d)***************/\n",p_VportInfo->port_src,htonl(p_VportInfo->port_src));
				struct in_addr addr;
				u32 ipsrc = (p_VportInfo->ipaddr_src);
				memcpy(&addr,&ipsrc,4);
				Fprintf("ipaddr_src is: %s\n",inet_ntoa(addr));
				Fprintf("port_src is: 0x%x (%d)\n",p_VportInfo->port_src,htons(p_VportInfo->port_src));
				Fprintf("vp_type is: %s\n",(p_VportInfo->vp_type == VP_TYPE_UDP)?"VP_TYPE_UDP":"VP_TYPE_FR");

				char *clientName=NULL;
			    STRU_LINX_SERVER_INFO * linx_info;
				linx_info = npu_get_server_linx_info();
				if(0 == linx_get_name(linx_info->linx,p_VportInfo->client,&clientName))
				{
					Fprintf("clientname: %s client =0x%x  \n",clientName,p_VportInfo->client);
					linx_free_name(linx_info->linx,&clientName);
				}
				/*Fprintf("UsedFlag is: 0x%x\n",p_VportInfo->UsedFlag);*/
				if(p_VportInfo->vp_type == VP_TYPE_FR)
				{
					Fprintf("vfport is: 0x%x (%d) : ""S%dP%dFR%dD%d"" \n",p_VportInfo->ch_info.vfport,htonl(p_VportInfo->ch_info.vfport),
                            p_VportInfo->ch_info.card,p_VportInfo->ch_info.trunk,p_VportInfo->ch_info.group,p_VportInfo->ch_info.dlci);
					Fprintf("dlci is: %d\n",p_VportInfo->ch_info.dlci);
					Fprintf("card is: %d\n",p_VportInfo->ch_info.card);
					Fprintf("trunk is: %d\n",p_VportInfo->ch_info.trunk);
					Fprintf("group is: %d\n",p_VportInfo->ch_info.group);
					Fprintf("slot_selection is: 0x%x\n",p_VportInfo->ch_info.slot_selection);
					Fprintf("Qos is: %s\n",(p_VportInfo->ch_info.QosEn == 0)?"enabled":"disabled");
					Fprintf("Qos param bc: %d bits\n",p_VportInfo->ch_info.bc);
					Fprintf("Qos param be: %d bits\n",p_VportInfo->ch_info.be);
					Fprintf("Qos param cir: %d bits/second\n",p_VportInfo->ch_info.cir);
					Fprintf("\n");
				}
			}
		}
    }
}

void display_ul_route_ht_link(void)
{
    NODE *temp;
	ENTRY item={0};
	ENTRY *found_item;
	NP_UL_FLOW_INFO *p_FrFlowInfo;
	u32 NRI;
    u32 i = 0;
    temp=uplink_nodehead;
	STRU_SET_INITAIL_CONFIG *initial_cfg = npu_get_initial_cfg_info();
    while(temp!=NULL)
    {
        item.key = temp->ht_key;
        temp=temp->next;

		if(0 == hsearch_r(item,FIND,&found_item,uplink_ht))
		{
			WDDI_WARNING("When display_ul_route_ht_link failed\n");
		}
		else
		{

			p_FrFlowInfo = (NP_UL_FLOW_INFO *)found_item->data;
            if(USED == p_FrFlowInfo->UsedFlag)
			{
				i++;
				Fprintf("             ####item %d#####  \n",i);
                if(i%1024 == 0)
                    sleep(1);
				NRI = p_FrFlowInfo->NRI;
				if(initial_cfg->OpterateMode == NPU_MODE_FR)
				{
					u32 card;
					u32 trunk;
					u32 group;
					u32 dlci;
					np_fraddr_unpack(p_FrFlowInfo->vport,&card,&trunk,&group,&dlci);
					Fprintf("vport is: 0x%x (%d) : S%dP%dFR%dD%d(valid for FR vport)\n",p_FrFlowInfo->vport,htons(p_FrFlowInfo->vport),
                            card,trunk,group,dlci);
				}
				else
				{
					Fprintf("vport is: 0x%x (%d)\n",p_FrFlowInfo->vport,htons(p_FrFlowInfo->vport));

				}
				Fprintf("NRI is: %d\n",p_FrFlowInfo->NRI);
				Fprintf("portSrc is: 0x%x (%d)\n",p_FrFlowInfo->portSrc,htons(p_FrFlowInfo->portSrc));
				Fprintf("\n");

                show_dps_ul_route_item(p_FrFlowInfo);
			}

		}
    }
}

void display_dl_route_ht_link(WP_U16 DportStartValue,WP_U16 bvci,u32 OptMode)
{
    NODE *temp;
	ENTRY item={0};
	ENTRY *found_item;
	NP_DL_FLOW_INFO *p_EthFlowInfo;
	u32 i = 0;
	STRU_SET_INITAIL_CONFIG *initial_cfg = npu_get_initial_cfg_info();
	if(OptMode == DEL_DL_RULE_TALBE_BASED_ON_DPORT_BVCI)
	{
		temp=downlink_nodehead;
	}
	else if(OptMode == DEL_DL_RULE_TALBE_BASED_ON_DPORT)
	{
		temp=fr_dl_dport_nodehead[DportStartValue];

	}
	else if(OptMode == DEL_DL_RULE_TALBE_BASED_ON_BVCI)
	{
		temp=fr_dl_bvci_nodehead[bvci];

	}
	else
	{
	    return ;
	}


    while(temp!=NULL)
    {
        item.key = temp->ht_key;
        temp=temp->next;

		if(0 == hsearch_r(item,FIND,&found_item,downlink_ht))
		{
			WDDI_WARNING("When display_dl_route_ht_link failed\n");
		}
		else
		{
			p_EthFlowInfo = (NP_DL_FLOW_INFO *)found_item->data;
			if(USED == p_EthFlowInfo->UsedFlag)
			{   i++;
				Fprintf("			 ####item %d #####	\n",i);
                if(i%1024 == 0)
                    sleep(1);
				Fprintf("DportStartValue is: 0x%x (%d)\n",p_EthFlowInfo->rule.DportStartValue,htons(p_EthFlowInfo->rule.DportStartValue));
				Fprintf("NS_BVCI is: 0x%x (%d)\n",p_EthFlowInfo->rule.BVCInew,(p_EthFlowInfo->rule.BVCInew));
				Fprintf("NS_BVCI_old is: 0x%x (%d)\n",p_EthFlowInfo->rule.BVCI,(p_EthFlowInfo->rule.BVCI));
				if(initial_cfg->OpterateMode == NPU_MODE_FR)
				{
					Fprintf("NSEI is: 0x%x (%d)\n",p_EthFlowInfo->rule.nsei_portSrc.NSEI,(p_EthFlowInfo->rule.nsei_portSrc.NSEI));

				}
				else
				{
					Fprintf("portSrc is: 0x%x (%d)\n",p_EthFlowInfo->rule.nsei_portSrc.portSrc,(p_EthFlowInfo->rule.nsei_portSrc.NSEI));

				}
#if 0
				struct in_addr addr;
				memcpy(&addr,&p_EthFlowInfo->rule.,4);
				Fprintf("IpDst is: %s\n",inet_ntoa(addr));
				Fprintf("PortDst is: 0x%x (%d)\n",p_EthFlowInfo->PortDst,htons(p_EthFlowInfo->PortDst));
				Fprintf("\n");
#endif
                show_dps_dl_route_item(p_EthFlowInfo);
			}
		}
    }
}

void display_arp_ht_link(void)
{
    NODE *temp;
	ENTRY item={0};
	ENTRY *found_item;
	NP_ARP_INFO *p_ArpInfo;
    temp=arp_nodehead;
    while(temp!=NULL)
    {
        item.key = temp->ht_key;
        temp=temp->next;

		if(0 == hsearch_r(item,FIND,&found_item,arp_ht))
		{
			WDDI_WARNING("When display_vport_ht_link failed\n");
		}
		else
		{
			p_ArpInfo = (NP_ARP_INFO *)found_item->data;
			Fprintf("/***********************************************/\n");
			struct in_addr addr;
			u32 ip = (p_ArpInfo->Ip);
			memcpy(&addr,&ip,4);
			Fprintf("ip is: %s ",inet_ntoa(addr));
			Fprintf("Mac is: 0x%x:0x%x:0x%x:0x%x:0x%x:0x%x\n",p_ArpInfo->Mac[0],p_ArpInfo->Mac[1],p_ArpInfo->Mac[2],
                    p_ArpInfo->Mac[3],p_ArpInfo->Mac[4],p_ArpInfo->Mac[5]);
			Fprintf("\n");


		}
    }
}

void display_update_bvci_ht_link(void)
{
    NODE *temp;
	ENTRY item={0};
	ENTRY *found_item;
	NP_UPDATE_BVCI_FLOW_INFO *p_BvciFlowInfo;
	u32 i = 0;
    temp=update_bvci_nodehead;
    while(temp!=NULL)
    {
        item.key = temp->ht_key;
        temp=temp->next;

		if(0 == hsearch_r(item,FIND,&found_item,update_bvci_ht))
		{
			WDDI_WARNING("When display_update_bvci_ht_link failed\n");
		}
		else
		{
			p_BvciFlowInfo = (NP_UPDATE_BVCI_FLOW_INFO *)found_item->data;
			if(USED == p_BvciFlowInfo->UsedFlag)
			{   i++;
				Fprintf("			 ####item %d #####	\n",i);
                if(i%1024 == 0)
                    sleep(1);
				Fprintf("Port is: 0x%x (%d) \n",p_BvciFlowInfo->vport,htons(p_BvciFlowInfo->vport));
				Fprintf("NS_BVCI is: 0x%x (%d)\n",p_BvciFlowInfo->NS_BVCI,(p_BvciFlowInfo->NS_BVCI));
				Fprintf("NS_BVCI_old is: 0x%x (%d)\n",p_BvciFlowInfo->NS_BVCI_old,(p_BvciFlowInfo->NS_BVCI_old));
				Fprintf("\n");
                show_dps_bvci_item(p_BvciFlowInfo);
			}
		}
    }
}


void display_ul_link_table_ht_link(void)
{
    NODE *temp;
	ENTRY item={0};
	ENTRY *found_item;
	STRU_SET_UPLINK_PATH_LINK_TABLE *p_linkInfo;
	u32 i = 0;
    temp=fr_ul_link_table_nodehead;
    while(temp!=NULL)
    {
        item.key = temp->ht_key;
        temp=temp->next;

		if(0 == hsearch_r(item,FIND,&found_item,fr_ul_link_table_ht))
		{
			WDDI_WARNING("When display_dl_route_ht_link failed\n");
		}
		else
		{   i++;
			p_linkInfo = (STRU_SET_UPLINK_PATH_LINK_TABLE *)found_item->data;
            if(0 != p_linkInfo->IpEpAcitveNum)
            {
			    Fprintf("			 ####item %d #####	\n",i);
                if(i%1024 == 0)
                    sleep(1);
				u32 k,ipdst;
				struct in_addr addr;
				Fprintf("IpEpAcitveNum is: %d\n",p_linkInfo->IpEpAcitveNum);
				Fprintf("PortSrc is: 0x%x (%d)\n",p_linkInfo->PortSrc,htons(p_linkInfo->PortSrc));
				for(k = 0;k<p_linkInfo->IpEpAcitveNum;k++)
				{
					ipdst = p_linkInfo->IpEp[k].IpDst;
					memcpy(&addr,&ipdst,4);
					Fprintf(" IP_EP%d: \n",k);
					Fprintf("Flag is: %s \n",(p_linkInfo->IpEp[k].Flag == LOCAL_FLAG)?"LOCAL":"REMOTE");
					Fprintf("IpDst is: %s\n",inet_ntoa(addr));
					Fprintf("PortDst is: 0x%x (%d)\n",p_linkInfo->IpEp[k].PortDst,htons(p_linkInfo->IpEp[k].PortDst));
					Fprintf("\n");
				}
				Fprintf("\n");
                show_dps_ul_link_item(p_linkInfo);
			}

		}
    }
}

extern void show_dps_dl_link_item(STRU_SET_FR_DOWNLINK_PATH_LINK_TABLE *p_linkInfo);

void display_dl_link_table_ht_link(void)
{
    NODE *temp;
	ENTRY item={0};
	ENTRY *found_item;
	STRU_SET_FR_DOWNLINK_PATH_LINK_TABLE *p_linkInfo;
	u32 i = 0;
    temp=fr_dl_link_table_nodehead;
    while(temp!=NULL)
    {
        item.key = temp->ht_key;
        temp=temp->next;

		if(0 == hsearch_r(item,FIND,&found_item,fr_dl_link_table_ht))
		{
			WDDI_WARNING("When display_dl_route_ht_link failed\n");
		}
		else
		{   i++;
			p_linkInfo = (STRU_SET_FR_DOWNLINK_PATH_LINK_TABLE *)found_item->data;
            if(0 != p_linkInfo->FrVportAcitveNum)
            {
			    Fprintf("			 ####item %d #####	\n",i);
                if(i%1024 == 0)
                    sleep(1);
				u32 k;
				Fprintf("FrVportAcitveNum is: %d\n",p_linkInfo->FrVportAcitveNum);
				Fprintf("NSEI is: 0x%x (%d)\n",p_linkInfo->NSEI,(p_linkInfo->NSEI));
				for(k = 0;k<p_linkInfo->FrVportAcitveNum;k++)
				{
					Fprintf("Flag is: %s \n",(p_linkInfo->NsvcGrp[k].Flag == LOCAL_FLAG)?"LOCAL":"REMOTE");
					Fprintf("vport is: 0x%x\n",p_linkInfo->NsvcGrp[k].vfort);
					Fprintf("\n");
				}
				Fprintf("\n");
                show_dps_dl_link_item(p_linkInfo);
			}
		}
    }
}

void display_config_info(void)
{
    int i,j,k;
	NPU_DEBUG_FILE_ENABLE();
    STRU_SET_INITAIL_CONFIG *initial_cfg = npu_get_initial_cfg_info();
	Fprintf("\n");
	Fprintf("/**************NP Initial Configure**********************/\n");
	Fprintf("NPU version : %s\n",NPU_VERSION_NUM);
    Fprintf("g_npnonpoolFlag is %d\n",g_npnonpoolFlag);
	Fprintf("VlanFlag is: %s \n",(initial_cfg->VlanFlag == NPU_VLAN_ENABLE)?"NPU_VLAN_ENABLE":"NPU_VLAN_DISABLE");
	Fprintf("VlanTag is: %d\n",initial_cfg->VlanTag);
	Fprintf("OpterateMode is: %s\n",(initial_cfg->OpterateMode == NPU_MODE_FR)?"NPU_MODE_FR ":"NPU_MODE_IP");
	Fprintf("ifMACsrc is: 0x%x:0x%x:0x%x:0x%x:0x%x:0x%x\n",initial_cfg->ifMACsrc[0],initial_cfg->ifMACsrc[1],initial_cfg->ifMACsrc[2],
            initial_cfg->ifMACsrc[3],initial_cfg->ifMACsrc[4],initial_cfg->ifMACsrc[5]);
	Fprintf("PortPoolSize is: %d\n",initial_cfg->PortPoolSize);
    Fprintf("NRI_bitLen is: %d\n",initial_cfg->NRI_bitLen);
    Fprintf("NRI Size in DPS: %d\n", nri_bit_length);
	struct in_addr addr;
	u32 ifIpaddr = ntohl(initial_cfg->ifIpaddr);
	memcpy(&addr,&ifIpaddr,4);
	Fprintf("ifIpaddr is: %s\n",inet_ntoa(addr));

    u32 ifIpNetMask = ntohl(initial_cfg->ifIpNetMask);
	memcpy(&addr,&ifIpNetMask,4);
	Fprintf("ifIpNetMask is: %s\n",inet_ntoa(addr));

	Fprintf("route configure is:\n");
	for(i = 0;i < SPP_MAX_ROUTE_NUM;i++)
	{
		if((initial_cfg->routeInfo[i].IpDst!= 0) || (initial_cfg->routeInfo[i].GatewayIP!= 0))
		{
            Fprintf("route%d :ip dst = 0x%x netmask = 0x%x gateway = 0x%0x \n",i,initial_cfg->routeInfo[i].IpDst,initial_cfg->routeInfo[i].mask,initial_cfg->routeInfo[i].GatewayIP);
		}
	}

	Fprintf("stm1_num is: 0x%x\n",initial_cfg->stm1_num);
    if( initial_cfg->stm1_num >0 &&  initial_cfg->stm1_num < 3)
    {
		for(i = 0; i < SPP_MAX_STM1_PORT_NUM; i ++)
		{
		    if(initial_cfg->stru_stm1_cfg_info[i].stm1Valid)
		    {
			    Fprintf("E1T1type is: %s\n",(initial_cfg->stru_stm1_cfg_info[i].E1T1type == NPU_MODE_E1)?"NPU_MODE_E1":"NPU_MODE_T1");
				for(j = 0; j < SPP_MAX_TRUNK_NUM; j ++)
				{
					if(initial_cfg->stru_stm1_cfg_info[i].trunk_info[j].trunkValid)
					{

					    Fprintf("clock mode is: %s\n",(initial_cfg->stru_stm1_cfg_info[i].trunk_info[j].clock_mode == NPU_E1T1_CLOCK_MASTER)?"Master":"Slave");
                        for(k = 0; k < SPP_MAX_GROUP_NUM; k ++)
                        {

							if(initial_cfg->stru_stm1_cfg_info[i].trunk_info[j].group_info[k].groupValid)
							{
								Fprintf("stru_stm1_cfg_info[%d].trunk_info[%d].group_info[%d].slot_selection is: 0x%x max_frame_size = %d crc_type = %s\n",i,j,k,initial_cfg->stru_stm1_cfg_info[i].trunk_info[j].group_info[k].slot_selection,
                                        initial_cfg->stru_stm1_cfg_info[i].trunk_info[j].group_info[k].max_frame_size,
                                        (initial_cfg->stru_stm1_cfg_info[i].trunk_info[j].group_info[k].crc_type == NPU_E1T1_CRC16 ? "CRC16":"CRC32"));

							}
						}
						Fprintf("\n");
					}
				}
                Fprintf("\n");
			}
		}
	}
	Fprintf("\n/**************display_vport_ht************************/\n");
	display_vport_ht_link();
	Fprintf("/***************display_ul_route_table_ht************************/\n");
	display_ul_route_ht_link();
	if(initial_cfg->OpterateMode == NPU_MODE_FR)
	{
	    Fprintf("/***************display_ul_link_table_ht**************************/\n");
	}
	else
	{
	    Fprintf("/***************display_link_table_ht**************************/\n");
	}
    display_ul_link_table_ht_link();
	Fprintf("/***************display_dl_route_table_ht**********************/\n");
	display_dl_route_ht_link(0,0,DEL_DL_RULE_TALBE_BASED_ON_DPORT_BVCI);
	if(initial_cfg->OpterateMode == NPU_MODE_FR)
	{
		Fprintf("/***************display_dl_link_table_ht**************************/\n");
		display_dl_link_table_ht_link();
	}
	Fprintf("/***************display_bvci_ht**************************/\n");
	display_update_bvci_ht_link();

	/*for debug*/
#if 0
	Fprintf("/***************display_fr_ul_route_table_based_on dport_ht**********************/\n");
	display_dl_route_ht_link(10000,0x0708,DEL_DL_RULE_TALBE_BASED_ON_DPORT);
	Fprintf("/***************display_fr_ul_route_table_based_on bvci_ht**********************/\n");
	display_dl_route_ht_link(10000,0x0708,DEL_DL_RULE_TALBE_BASED_ON_BVCI);
#endif

	Fprintf("/***************display_arp_ht***************************/\n");
	display_arp_ht_link();
	NPU_DEBUG_FILE_DISABLE();
}


/*add for npumon*/

int npu_hanlde_npumon_request(STRU_NPUMON_INFO * info,LINX_SPID client)
{
	int ret;
	STRU_VPORT_INFO * VportInfo;
    STRU_SET_INITAIL_CONFIG *initial_cfg = npu_get_initial_cfg_info();

    u32 lineIndex,groupIndex;
    if(info->ifType == VP_TYPE_FR)
    {

        if(initial_cfg->stru_stm1_cfg_info[info->stm1-1].E1T1type == NPU_MODE_E1)
        {
            lineIndex = (info->stm1-1)*(N_UFE_MAX_LINES_E1/2) + (info->TrunkNumber-1);
        }
        else
        {
            lineIndex = (info->stm1-1)*(N_UFE_MAX_LINES_T1/2) + (info->TrunkNumber-1);

        }
        Fprintf("npumon:Stm1 = %d TrunkNumber = %d group = %d dlci = %d \n",info->stm1,info->TrunkNumber,info->group,info->dlci);
    }


	/*write information*/
	switch(info->cmd)
	{
    case NPU_MON_CMD_XM:
    {

        break;
    }
    case NPU_MON_CMD_XL:
    {
        if(info->ifType == VP_TYPE_FR)
        {
            /*check if ufe_device_handle[]][] is 0*/
            char buf[3];
            extern unsigned int ufe_device_handle[N_UFE_MAX_LINES_T1][32];
            if(ufe_device_handle[lineIndex][info->group])
            {
                buf[0] = 1;/*Connected*/
            }
            else
            {
                buf[0] = 0;/*DisConnected*/
            }
            (void)np_mon_send_capture_data(buf,1,client,0,VP_TYPE_LINX);
        }
        break;
    }
    case NPU_MON_CMD_XRU:
    {
        sysRealTime = time(NULL);
        double timeSeconds;
        u16 hours;
        u8  minutes;
        u8  seconds;
        timeSeconds = difftime(sysRealTime,sysInitTime);
        minutes = timeSeconds/60;
        seconds = (unsigned long int)timeSeconds%60;
        hours = timeSeconds/(60*60);
        //Fprintf("npu run time: hours %d minutes%d seconds%d \n",hours,minutes,seconds);
        char buf[6];/*Hours:buf[0] buf[1],minutes: buf[2] seconds: buf[3]*/
        buf[0] = hours>>8;
        buf[1] = hours&0xff;
        buf[2] = minutes;
        buf[3] = seconds;
        (void)np_mon_send_capture_data(buf,4,client,0,VP_TYPE_LINX);
        break;
    }
    /*trace data*/
    case NPU_MON_CMD_TI4:
    case NPU_MON_CMD_TI:
    case NPU_MON_CMD_TR:
    {
        int ret;
        ret = np_enable_vport_capture_for_npumon(info,client);
        if(ret != 0)
        {
            return ret;
        }
        break;
    }
    case NPU_MON_CMD_DD:
    case NPU_MON_CMD_CD:
    {
        if(info->ifType == VP_TYPE_FR)
        {
            int ret;
            /*clear device statics*/
            if(info->cmd == NPU_MON_CMD_CD)
            {
                if(npu_upi_stat_clear(lineIndex,info->group)!= 0)
                {
                    return NPU_GET_UPI_STATICS_ERROR;
                }
            }
            STRU_GET_UPI_GROUP_STATICS get_upistatics;
            get_upistatics.trunkid = info->TrunkNumber;
            get_upistatics.stm1Id = info->stm1;
            get_upistatics.groupid = info->group;
            ret = upi_stat_get_by_group(lineIndex, info->group,&get_upistatics.stru_upi_statics);
            if(ret != 0)
            {
                return ret;
            }
            (void)np_mon_send_capture_data((char *)&get_upistatics,sizeof(get_upistatics),client,0,VP_TYPE_LINX);
        }
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
        Fprintf("Unkonw cmd type %d \n",info->cmd);
        break;
    }
	}
    return STATUS_OK;
}

void npu_mon_capture_packet(char * buffer, u32 len,u32 port,int direction)
{
	STRU_VPORT_INFO * VportInfo;
	VportInfo = npu_get_vport_info(port);
	if(VportInfo == NULL)
	{
        return ;
	}
	if(VportInfo->UsedFlag == UNUSED)
	{
        return ;
	}
	if(VportInfo->npumon_info.captureEnableFlag == NPU_NPUMON_CAPTURE_ENABLE
       && VportInfo->npumon_info.client!= 0)
	{
        if(len >= VportInfo->npumon_info.len)
        {
            len = VportInfo->npumon_info.len;
        }
        if(VportInfo->npumon_info.ifType == VP_TYPE_UDP)
        {
            //VportInfo->npumon_info.ipaddr; /*ip filter will be used ,we don't konw how to use*/

        }
        if(direction == 0)/*income,use different thread linx send to client*/
        {
            (void)np_mon_send_capture_data(buffer,len,VportInfo->npumon_info.client,direction,VportInfo->npumon_info.ifType);
        }
        else/*out going ,use main process linx*/
        {
            (void)np_mon_send_capture_data(buffer,len,VportInfo->npumon_info.client,direction,VP_TYPE_LINX);/*VP_TYPE_LINX*/
        }
	}
	else
	{
		if(VportInfo->npumon_info.client != 0)/* */
		{
			VportInfo->npumon_info.client = 0;
			VportInfo->npumon_info.len = 0;
		}
	}
}
int np_mon_send_capture_data(char * buffer, u32 len,LINX_SPID client,int direction,u8 type)
{
	union LINX_SIGNAL *reply_sig;
    STRU_LINX_SERVER_INFO * linx_info;
	if(type == VP_TYPE_FR)
	{
		linx_info = npu_get_server_rx_fr_thread_linx_info();
	}
	else if(type == VP_TYPE_UDP)
	{
		linx_info = npu_get_server_rx_eth_thread_linx_info();
	}
	else/*VP_TYPE_LINX*/
	{
		linx_info = npu_get_server_linx_info();

	}

	reply_sig = linx_alloc(linx_info->linx, sizeof(struct stru_npu_get_request_ack_sig)+len+3, NPU_GET_REQUEST_ACK_SIG);
	if (reply_sig == NULL)
	{
		Fprintf("linx_alloc() failed\n");
		return ERR_LINX_ALLOC;
	}
	/*send result to client*/
	reply_sig->npu_get_request_ack.cmd = NPU_MON_GET_CAPTURE_FILE;
	reply_sig->npu_get_request_ack.result = STATUS_OK;
	reply_sig->npu_get_request_ack.pdata[0] = len>>8;
	reply_sig->npu_get_request_ack.pdata[1] = len&0xff;
	reply_sig->npu_get_request_ack.pdata[2] = direction;/*0 input ,1 output*/
	memcpy(&reply_sig->npu_get_request_ack.pdata[3],buffer,len);
	if (linx_send(linx_info->linx, &reply_sig, client) == -1)
	{
		WDDI_WARNING("linx_send() %s client = 0x%x", strerror(errno),client);
		return ERR_LINX_SEND;
	}
	return STATUS_OK;
}

int np_get_ns_bssgp_statics(u32 vport,STRU_GET_NS_BSSGP_STATICS * statics)
{
	STRU_SET_INITAIL_CONFIG *initial_cfg = npu_get_initial_cfg_info();
	int ret;
	STRU_VPORT_INFO  *VportInfo;
	FR_CHANNEL_INFO * ch_info;
	VportInfo = npu_get_vport_info(vport);
	wp_ns_bssgp_statis wp_statics;
	if(VportInfo == NULL)
	{
		WDDI_WARNING("np_get_ns_bssgp_statics npu_get_vport_info fail vport= 0x%x",vport);
		return ERR_UL_RELEASE_RESOURCE;
	}
	else if(VportInfo->UsedFlag == UNUSED)
	{
		WDDI_WARNING("np_get_ns_bssgp_statics npu_get_vport_info fail ,vport closed vport= 0x%x",vport);
		return ERR_UL_RELEASE_RESOURCE;
	}
	if(initial_cfg->OpterateMode == NPU_MODE_FR)
	{
	    ch_info = &VportInfo->ch_info;
		memset(&wp_statics,0,sizeof(wp_statics));
	    ret = WP_FlowStatisticsGet(0,ch_info->agg_fr_rx_host_flow,&wp_statics);/*rx*/

		statics->ns_bssgp_statics.bssgp_in_octets = wp_statics.RxBssgpBytes - (wp_statics.RxBssgpDataPackets+wp_statics.RxBssgpSignalPackets)*6;/*fr header+ns header*/
		statics->ns_bssgp_statics.bssgp_in_packets = wp_statics.RxBssgpDataPackets + wp_statics.RxBssgpSignalPackets;//SPPSP-100
		statics->ns_bssgp_statics.bssgp_in_packets_signalling = wp_statics.RxBssgpSignalPackets;

		statics->ns_bssgp_statics.ns_in_octets = wp_statics.RxNsBytes-(wp_statics.RxNsDataPackets+wp_statics.RxNsSignalPackets)*2;
		statics->ns_bssgp_statics.ns_in_packets = wp_statics.RxNsDataPackets + wp_statics.RxNsSignalPackets;//SPPSP-100
		statics->ns_bssgp_statics.ns_in_packets_signalling = wp_statics.RxNsSignalPackets;

        /*get dps statics*/
		memset(&wp_statics,0,sizeof(wp_statics));
		ret = WP_FlowStatisticsGet(0,ch_info->agg_downlink_tx2fr_device,&wp_statics);/*tx*/
		if(initial_cfg->VlanFlag == NPU_VLAN_ENABLE)
		{
			statics->ns_bssgp_statics.bssgp_out_octets = wp_statics.TxBssgpBytes -(wp_statics.TxBssgpDataPackets+ wp_statics.TxBssgpSignalPackets)*(4+46);/*remove ns header and enet header*/
		}
		else
		{
			statics->ns_bssgp_statics.bssgp_out_octets = wp_statics.TxBssgpBytes -(wp_statics.TxBssgpDataPackets+ wp_statics.TxBssgpSignalPackets)*(4+42);/*remove ns header and enet header*/
		}
		statics->ns_bssgp_statics.bssgp_out_packets = wp_statics.TxBssgpDataPackets + wp_statics.TxBssgpSignalPackets;//SPPSP-100
		statics->ns_bssgp_statics.bssgp_out_packets_signalling= wp_statics.TxBssgpSignalPackets;
		/*add bssgp host send statics*/
		statics->ns_bssgp_statics.bssgp_out_octets += VportInfo->ns_bssgp_statics.TxBssgpBytes;
		statics->ns_bssgp_statics.bssgp_out_packets += (VportInfo->ns_bssgp_statics.TxBssgpDataPackets + VportInfo->ns_bssgp_statics.TxBssgpSignalPackets);//SPPSP-100
		statics->ns_bssgp_statics.bssgp_out_packets_signalling += VportInfo->ns_bssgp_statics.TxBssgpSignalPackets;
		if(initial_cfg->VlanFlag == NPU_VLAN_ENABLE)
		{
			statics->ns_bssgp_statics.ns_out_octets = wp_statics.TxNsBytes-(wp_statics.TxNsDataPackets+wp_statics.TxNsSignalPackets)*46;
		}
		else
		{
			statics->ns_bssgp_statics.ns_out_octets = wp_statics.TxNsBytes-(wp_statics.TxNsDataPackets+wp_statics.TxNsSignalPackets)*42;
		}
		statics->ns_bssgp_statics.ns_out_packets = wp_statics.TxNsDataPackets + wp_statics.TxNsSignalPackets;//SPPSP-100
		statics->ns_bssgp_statics.ns_out_packets_signalling = wp_statics.TxNsSignalPackets;
		/*add ns host send statics*/
		statics->ns_bssgp_statics.ns_out_octets += VportInfo->ns_bssgp_statics.TxNsBytes;
		statics->ns_bssgp_statics.ns_out_packets += (VportInfo->ns_bssgp_statics.TxNsDataPackets + VportInfo->ns_bssgp_statics.TxNsSignalPackets);//SPPSP-100
		statics->ns_bssgp_statics.ns_out_packets_signalling += VportInfo->ns_bssgp_statics.TxNsSignalPackets;

	}
	else
    {
        memset(&wp_statics,0,sizeof(wp_statics));
        ret = WP_FlowStatisticsGet(0,VportInfo->agg_ip2iptx2host,&wp_statics);/*rx*/
		if(initial_cfg->VlanFlag == NPU_VLAN_ENABLE)
		{
			statics->ns_bssgp_statics.bssgp_in_octets = wp_statics.RxBssgpBytes - (wp_statics.RxBssgpDataPackets+wp_statics.RxBssgpSignalPackets)*50;/*udp header+ns header*/
		}
		else
		{
			statics->ns_bssgp_statics.bssgp_in_octets = wp_statics.RxBssgpBytes - (wp_statics.RxBssgpDataPackets+wp_statics.RxBssgpSignalPackets)*46;/*udp header+ns header*/
		}
		statics->ns_bssgp_statics.bssgp_in_packets = wp_statics.RxBssgpDataPackets + wp_statics.RxBssgpSignalPackets;//SPPSP-100
		statics->ns_bssgp_statics.bssgp_in_packets_signalling = wp_statics.RxBssgpSignalPackets;
        if(initial_cfg->VlanFlag == NPU_VLAN_ENABLE)
        {
			statics->ns_bssgp_statics.ns_in_octets = wp_statics.RxNsBytes-(wp_statics.RxNsDataPackets+wp_statics.RxNsSignalPackets)*46;
		}
		else
		{
			statics->ns_bssgp_statics.ns_in_octets = wp_statics.RxNsBytes-(wp_statics.RxNsDataPackets+wp_statics.RxNsSignalPackets)*42;
		}
		statics->ns_bssgp_statics.ns_in_packets = wp_statics.RxNsDataPackets + wp_statics.RxNsSignalPackets;//SPPSP-100
		statics->ns_bssgp_statics.ns_in_packets_signalling = wp_statics.RxNsSignalPackets;


        /*get dps statics*/
		memset(&wp_statics,0,sizeof(wp_statics));
		ret = WP_FlowStatisticsGet(0,VportInfo->agg_ul_ip2iptx2Enet1,&wp_statics);/*tx*/
		if(initial_cfg->VlanFlag == NPU_VLAN_ENABLE)
		{
			statics->ns_bssgp_statics.bssgp_out_octets = wp_statics.TxBssgpBytes -(wp_statics.TxBssgpDataPackets+ wp_statics.TxBssgpSignalPackets)*(4+46);/*remove ns header and enet header*/
		}
		else
		{
			statics->ns_bssgp_statics.bssgp_out_octets = wp_statics.TxBssgpBytes -(wp_statics.TxBssgpDataPackets+ wp_statics.TxBssgpSignalPackets)*(4+42);/*remove ns header and enet header*/
		}
		statics->ns_bssgp_statics.bssgp_out_packets = wp_statics.TxBssgpDataPackets + wp_statics.TxBssgpSignalPackets;//SPPSP-100
		statics->ns_bssgp_statics.bssgp_out_packets_signalling= wp_statics.TxBssgpSignalPackets;
        /*get dps statics*/
		memset(&wp_statics,0,sizeof(wp_statics));
		ret = WP_FlowStatisticsGet(0,VportInfo->agg_dl_ip2iptx2Enet1,&wp_statics);/*tx*/
		if(initial_cfg->VlanFlag == NPU_VLAN_ENABLE)
		{
			statics->ns_bssgp_statics.bssgp_out_octets += wp_statics.TxBssgpBytes -(wp_statics.TxBssgpDataPackets+ wp_statics.TxBssgpSignalPackets)*(4+46);/*remove ns header and enet header*/
		}
		else
		{
			statics->ns_bssgp_statics.bssgp_out_octets += wp_statics.TxBssgpBytes -(wp_statics.TxBssgpDataPackets+ wp_statics.TxBssgpSignalPackets)*(4+42);/*remove ns header and enet header*/
		}
		statics->ns_bssgp_statics.bssgp_out_packets += (wp_statics.TxBssgpDataPackets + wp_statics.TxBssgpSignalPackets);//SPPSP-100
		statics->ns_bssgp_statics.bssgp_out_packets_signalling += wp_statics.TxBssgpSignalPackets;


		/*add bssgp host send statics*/
		statics->ns_bssgp_statics.bssgp_out_octets += VportInfo->ns_bssgp_statics.TxBssgpBytes;
		statics->ns_bssgp_statics.bssgp_out_packets += (VportInfo->ns_bssgp_statics.TxBssgpDataPackets + VportInfo->ns_bssgp_statics.TxBssgpSignalPackets);//SPPSP-100
		statics->ns_bssgp_statics.bssgp_out_packets_signalling += VportInfo->ns_bssgp_statics.TxBssgpSignalPackets;
		if(initial_cfg->VlanFlag == NPU_VLAN_ENABLE)
		{
			statics->ns_bssgp_statics.ns_out_octets = wp_statics.TxNsBytes-(wp_statics.TxNsDataPackets+wp_statics.TxNsSignalPackets)*46;
		}
		else
		{
			statics->ns_bssgp_statics.ns_out_octets = wp_statics.TxNsBytes-(wp_statics.TxNsDataPackets+wp_statics.TxNsSignalPackets)*42;
		}
		statics->ns_bssgp_statics.ns_out_packets = wp_statics.TxNsDataPackets+wp_statics.TxNsSignalPackets;//SPPSP-100
		statics->ns_bssgp_statics.ns_out_packets_signalling = wp_statics.TxNsSignalPackets;
		/*add ns host send statics*/
		statics->ns_bssgp_statics.ns_out_octets += VportInfo->ns_bssgp_statics.TxNsBytes;
		statics->ns_bssgp_statics.ns_out_packets += (VportInfo->ns_bssgp_statics.TxNsDataPackets + VportInfo->ns_bssgp_statics.TxNsSignalPackets);//SPPSP-100
		statics->ns_bssgp_statics.ns_out_packets_signalling += VportInfo->ns_bssgp_statics.TxNsSignalPackets;

	}
    return 0;
}
int np_clear_ns_bssgp_statics(u32 vport)
{
	STRU_SET_INITAIL_CONFIG *initial_cfg = npu_get_initial_cfg_info();
	int ret;
	STRU_VPORT_INFO  *VportInfo;
	FR_CHANNEL_INFO * ch_info;
	VportInfo = npu_get_vport_info(vport);
	wp_ns_bssgp_statis wp_statics;
	if(VportInfo == NULL)
	{
		WDDI_WARNING("np_clear_ns_bssgp_statics npu_get_vport_info fail vport= 0x%x",vport);
		return ERR_UL_RELEASE_RESOURCE;
	}
	else if(VportInfo->UsedFlag == UNUSED)
	{
		WDDI_WARNING("np_clear_ns_bssgp_statics npu_get_vport_info fail ,vport closed vport= 0x%x",vport);
		return ERR_UL_RELEASE_RESOURCE;
	}
	if(initial_cfg->OpterateMode == NPU_MODE_FR)
	{
	    ch_info = &VportInfo->ch_info;
		(void)WP_FlowStatisticsClear(0,ch_info->agg_fr_rx_host_flow);
		(void)WP_FlowStatisticsClear(0,ch_info->agg_downlink_tx2fr_device);/*tx*/
	}
	else
    {
        (void)WP_FlowStatisticsClear(0,VportInfo->agg_ip2iptx2host);/*rx*/
		(void)WP_FlowStatisticsClear(0,VportInfo->agg_ul_ip2iptx2Enet1);/*tx*/
	}
	/*clear host send data statistics*/
	memset(&VportInfo->ns_bssgp_statics,0,sizeof(HOST_NS_BSSGP_STATICS));
    return 0;
}

void np_host_set_ns_bssgp_statics(u8 * buf,u32 len,HOST_NS_BSSGP_STATICS * statics)
{

    //if(buf[0] == 0x00 && buf[1] == 0x00)/*ns unit data*/
    if(buf[0] == 0x00)/*ns unit data*/
    {
        if(buf[4] == 0x01 || buf[4] == 0x00)/*bssgp unit data 1:UL 0:DL*/
        {
            statics->TxBssgpDataPackets++;
        }
        else
        {
            statics->TxBssgpSignalPackets++;
        }
        statics->TxBssgpBytes+=(len-4);/*remove NS header*/
        statics->TxNsDataPackets++;

    }
    else /*NS signaling*/
    {
        statics->TxNsSignalPackets++;
    }
    statics->TxNsBytes+=len;

}


int np_close_vport_for_client(LINX_SPID client)
{
	NODE *temp;
	ENTRY item={0};
	ENTRY *found_item;
	STRU_VPORT_INFO *p_VportInfo;
	temp=vport_nodehead;
	int ret;

	while(temp!=NULL)
	{
		item.key = temp->ht_key;
		temp=temp->next;

		if(0 == hsearch_r(item,FIND,&found_item,ht))
		{
			WDDI_WARNING("np_release_resource_on_vport\n");
		}
		else
		{
			p_VportInfo = (STRU_VPORT_INFO *)found_item->data;
			if(USED == p_VportInfo->UsedFlag)
			{
	            /*find client's vport*/
	            if(p_VportInfo->client == client)
	            {
	                WDDI_INFO("np_close_vport_for_client vport = 0x%x ,client = 0x%x",p_VportInfo->port_src,client);
	                if(p_VportInfo->vp_type == VP_TYPE_FR)
	                {
					    ret = np_close_fr_vport(p_VportInfo->port_src);
						if(ret != STATUS_OK) return ret;
					}
					ret = npu_clear_vport_info(p_VportInfo->port_src,p_VportInfo->vp_type);
					if(ret != STATUS_OK)
					{
                		WDDI_WARNING("np_close_vport_for_client npu_clear_vport_info() ret=%d error",ret);
						return ret;
					}
				}
			}
		}
	}
	return STATUS_OK;
}

int np_close_vport_for_npumon(LINX_SPID client)
{
	NODE *temp;
	ENTRY item={0};
	ENTRY *found_item;
	STRU_VPORT_INFO *p_VportInfo;
	temp=vport_nodehead;
	int ret;

	while(temp!=NULL)
	{
		item.key = temp->ht_key;
		temp=temp->next;

		if(0 == hsearch_r(item,FIND,&found_item,ht))
		{
			WDDI_WARNING("np_release_resource_on_vport\n");
		}
		else
		{
			p_VportInfo = (STRU_VPORT_INFO *)found_item->data;
			if(USED == p_VportInfo->UsedFlag)
			{
	            /*find client's vport*/
	            if(p_VportInfo->npumon_info.client == client)
	            {
	                WDDI_INFO("np_close_vport_for_npumon vport = 0x%x ,client = 0x%x",p_VportInfo->port_src,client);

					if(p_VportInfo->npumon_info.captureEnableFlag == NPU_NPUMON_CAPTURE_ENABLE)
					{
                        p_VportInfo->npumon_info.captureEnableFlag = NPU_NPUMON_CAPTURE_DISABLE;
						p_VportInfo->npumon_info.client = 0;
					}
				}
			}
		}
	}
	return STATUS_OK;
}

int np_enable_vport_capture_for_npumon(STRU_NPUMON_INFO * info,LINX_SPID client)
{
	NODE *temp;
	ENTRY item={0};
	ENTRY *found_item;
	STRU_VPORT_INFO *VportInfo;
	temp=vport_nodehead;
	int ret;
    u32 vport = 0;
	if(info->ifType == VP_TYPE_FR)
	{   /*group&dlci specified*/
		if(info->group != NPUMON_NO_ARGS && info->dlci!= NPUMON_NO_ARGS)
		{
			vport = np_fraddr_pack(info->stm1,info->TrunkNumber,info->group,info->dlci);
			VportInfo = npu_get_vport_info(vport);
			if(VportInfo == NULL)
			{
				Fprintf("can't get port info ,please check npumon input param port = %d \n",vport);
				return NPU_SET_NPUMON_CMD_PORT_ERROR;
			}
			if(USED == VportInfo->UsedFlag)
			{
				if(VportInfo->npumon_info.captureEnableFlag == NPU_NPUMON_CAPTURE_DISABLE)
				{
					VportInfo->npumon_info.client = client;
					VportInfo->npumon_info.captureEnableFlag = NPU_NPUMON_CAPTURE_ENABLE;
					VportInfo->npumon_info.len = info->len;
					VportInfo->npumon_info.ifType = VP_TYPE_FR;
				}
				else
				{
					return NPU_SET_NPUMON_CMD_ALREAD_START_ERROR;
				}

			}
			else
			{
                return NPU_SET_NPUMON_CMD_ALREAD_START_ERROR;
			}

		}
		else
		{
			while(temp!=NULL)
			{
				item.key = temp->ht_key;
				temp=temp->next;

				if(0 == hsearch_r(item,FIND,&found_item,ht))
				{
					WDDI_WARNING("np_enable_vport_capture_for_npumon port can't find\n");
				}
				else
				{
					VportInfo = (STRU_VPORT_INFO *)found_item->data;
					if(USED == VportInfo->UsedFlag)
					{
					    if(info->group == NPUMON_NO_ARGS)/*group not specified ,should enable all vport on this trunk*/
					    {
                            if((VportInfo->ch_info.trunk == info->TrunkNumber)&& (VportInfo->ch_info.card == info->stm1))
                            {
                                if(VportInfo->npumon_info.captureEnableFlag == NPU_NPUMON_CAPTURE_DISABLE)
                                {
                                    VportInfo->npumon_info.client = client;
                                    VportInfo->npumon_info.captureEnableFlag = NPU_NPUMON_CAPTURE_ENABLE;
                                    VportInfo->npumon_info.len = info->len;
                                    VportInfo->npumon_info.ifType = VP_TYPE_FR;
                                }
                                else
                                {
                                    return NPU_SET_NPUMON_CMD_ALREAD_START_ERROR;
                                }
							}
						}
						else/*group specify, don't care dlci. should enable all vport on this groups,this trunk*/
						{
                            if((VportInfo->ch_info.trunk == info->TrunkNumber) && (VportInfo->ch_info.group == info->group) && (VportInfo->ch_info.card == info->stm1))
                            {
                                if(VportInfo->npumon_info.captureEnableFlag == NPU_NPUMON_CAPTURE_DISABLE)
                                {   //Fprintf("client = %d\n",client);
                                    VportInfo->npumon_info.client = client;
                                    VportInfo->npumon_info.captureEnableFlag = NPU_NPUMON_CAPTURE_ENABLE;
                                    VportInfo->npumon_info.len = info->len;
                                    VportInfo->npumon_info.ifType = VP_TYPE_FR;
                                }
                                else
                                {
                                    return NPU_SET_NPUMON_CMD_ALREAD_START_ERROR;
                                }
							}
						}
					}
				}
			}

		}

	}/*for IP*/
	else
	{
		vport = info->portSrc;

        if(vport == 0x100)/*htons(1) indicate capture all opened vport*/
        {
            Fprintf("start to capture packet on all udp port\n");
			NODE *temp;
			ENTRY item={0};
			ENTRY *found_item;
			temp=vport_nodehead;
			int ret;
			while(temp!=NULL)
			{
				item.key = temp->ht_key;
				temp=temp->next;

				if(0 == hsearch_r(item,FIND,&found_item,ht))
				{
					WDDI_WARNING("np_enable_vport_capture_for_npumon port not found\n");
				}
				else
				{
					VportInfo = (STRU_VPORT_INFO *)found_item->data;
					if(USED == VportInfo->UsedFlag)
					{
					    if(VportInfo->vp_type == VP_TYPE_UDP)
					    {
							if(VportInfo->npumon_info.captureEnableFlag == NPU_NPUMON_CAPTURE_DISABLE)
							{
								VportInfo->npumon_info.client = client;
								VportInfo->npumon_info.captureEnableFlag = NPU_NPUMON_CAPTURE_ENABLE;
								VportInfo->npumon_info.len = info->len;
								VportInfo->npumon_info.ifType = VP_TYPE_UDP;
								VportInfo->npumon_info.ipaddr = info->ipaddr;
							}
							else
							{
								//return NPU_SET_NPUMON_CMD_ALREAD_START_ERROR;
							}
						}
					}
				}
			}
			return STATUS_OK;
		}
		/*otherwise capture on sigal vport*/
		VportInfo = npu_get_vport_info(vport);
		if(VportInfo == NULL)
		{
			Fprintf("can't get port info ,please check npumon input param port = %d \n",vport);
			return NPU_SET_NPUMON_CMD_PORT_ERROR;
		}
		if(USED == VportInfo->UsedFlag)
		{
			if(VportInfo->npumon_info.captureEnableFlag == NPU_NPUMON_CAPTURE_DISABLE)
			{
				VportInfo->npumon_info.client = client;
				VportInfo->npumon_info.captureEnableFlag = NPU_NPUMON_CAPTURE_ENABLE;
				VportInfo->npumon_info.len = info->len;
				VportInfo->npumon_info.ifType = VP_TYPE_UDP;
				VportInfo->npumon_info.ipaddr = info->ipaddr;
			}
			else
			{
				return NPU_SET_NPUMON_CMD_ALREAD_START_ERROR;
			}
		}
		else
		{
			return NPU_SET_NPUMON_CMD_PORT_ERROR;
		}

	}
	return STATUS_OK;
}


void np_display_frag_flow_info(void)
{
    NODE *temp;
	ENTRY item={0};
	ENTRY *found_item;
	NP_ARP_INFO *p_ArpInfo;
    temp=arp_nodehead;
	Fprintf("IP fragement info :\n");
    while(temp!=NULL)
    {
        item.key = temp->ht_key;
        temp=temp->next;

		if(0 == hsearch_r(item,FIND,&found_item,arp_ht))
		{
		}
		else
		{
			p_ArpInfo = (NP_ARP_INFO *)found_item->data;

			if(p_ArpInfo->agg_ipfrag)
			{
				struct in_addr addr;
				u32 ip = (p_ArpInfo->Ip);
				memcpy(&addr,&ip,4);
				Fprintf("ip is: %s ",inet_ntoa(addr));
				Fprintf("Mac is: 0x%x:0x%x:0x%x:0x%x:0x%x:0x%x\n",p_ArpInfo->Mac[0],p_ArpInfo->Mac[1],p_ArpInfo->Mac[2],
                        p_ArpInfo->Mac[3],p_ArpInfo->Mac[4],p_ArpInfo->Mac[5]);
				np_display_frag_info(p_ArpInfo->fragGrpHandle);
				Fprintf("\n");
				Fprintf("p_ArpInfo->agg_tx2eth1: \n");
				StatIwFlowAggregation(p_ArpInfo->agg_tx2eth1);
				Fprintf("p_ArpInfo->agg_ipfrag: \n");
				StatIwFlowAggregation(p_ArpInfo->agg_ipfrag);
			}
		}
    }
    return ;
}
int linx_init_thread_rx_fr(void)
{
    LINX *linx;

	/* Open LINX socket */
	linx = linx_open("wddi_server_rx_fr_thread", 0, NULL);
	if (linx == NULL)
	{
        WDDI_ERR("linx_open() %s", strerror(errno));
        return -1;
	}
	npu_set_server_rx_fr_thread_linx_info(linx);
    return  linx_get_descriptor(linx);
}
int linx_init_thread_rx_eth(void)
{
    LINX *linx;

	/* Open LINX socket */
	linx = linx_open("wddi_server_rx_eth_thread", 0, NULL);
	if (linx == NULL)
	{
        WDDI_ERR("linx_open() %s", strerror(errno));
        return -1;
	}
	npu_set_server_rx_eth_thread_linx_info(linx);
    return  linx_get_descriptor(linx);
}



STRU_PEER_VPORT_INFO * npu_set_peer_vport_info(u32 port_src)
{

	ENTRY item={0};
	ENTRY *found_item;
	char itemstr[16]={0};
	STRU_PEER_VPORT_INFO *p_VportInfo;
	item.key = itemstr;

	sprintf(item.key,"%d",port_src);

	if(0 == hsearch_r(item,FIND,&found_item,peer_vport_ht))//fail, first time to add
	{
		p_VportInfo = malloc(sizeof(STRU_PEER_VPORT_INFO));
		memset(p_VportInfo,0,sizeof(STRU_PEER_VPORT_INFO));
		item.key = malloc(16);
		memset(item.key,0,16);
		sprintf(item.key,"%d",port_src);
		//WDDI_INFO("set item.key = %s \n",item.key);
		/******************donot support mutli-thread*******************/
		item.data = (void *)p_VportInfo;
		(void)hsearch_r(item, ENTER,&found_item,peer_vport_ht) ;
	}
	else
	{
	    p_VportInfo = (STRU_PEER_VPORT_INFO *)found_item->data; //already set in hash table
	}
	p_VportInfo->port_src = port_src;

	return p_VportInfo;
}

STRU_PEER_VPORT_INFO * npu_get_peer_vport_info(u32 port_src)
{
	ENTRY item={0};
	ENTRY *found_item;
	char itemstr[30];
	item.key = itemstr;
    sprintf(item.key,"%d",port_src);
	if(0 == hsearch_r(item,FIND,&found_item,peer_vport_ht))
	{
	    return NULL;
	}
	else
	{
		return (STRU_PEER_VPORT_INFO *)found_item->data;

	}
}
