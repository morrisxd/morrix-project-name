#include "np_common.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <syslog.h>
#include <errno.h>

#include <arpa/inet.h>
#include <netinet/ether.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/if.h>

#include "tun_dev.h"
#include "wddi_log.h"
#include "spp_npu_link.h"
#include "spp_wddi_server.h"

#include "xs_ioctl.h"

#define HAVE_LINUX_IF_TUN_H
#define IF_NAME "sfp2"
//static char tap_ifname[20] = {'s', 'f', 'p', '2', 0};
char* get_ifname(void){ return IF_NAME;}
//void  set_ifname(char *ifname){ strcpy(tap_ifname, ifname);}
/*
 * Allocate TUN device, returns opened fd.
 * Stores dev name in the first arg(must be large enough).
 */
static int tun_open_common0(char *dev, int istun)
{
    char tunname[14];
    int i, fd, err;

    if( *dev ) {
        sprintf(tunname, "/dev/%s", dev);
        return open(tunname, O_RDWR);
    }

    sprintf(tunname, "/dev/%s", istun ? "tun" : "tap");
    err = 0;
    for(i=0; i < 255; i++){
        sprintf(tunname + 8, "%d", i);
        /* Open device */
        if( (fd=open(tunname, O_RDWR)) > 0 ) {
            strcpy(dev, tunname + 5);
            return fd;
        }
        else if (errno != ENOENT)
            err = errno;
        else if (i)	/* don't try all 256 devices */
            break;
    }
    if (err)
        errno = err;
    return -1;
}

#ifdef HAVE_LINUX_IF_TUN_H /* New driver support */
#include <linux/if_tun.h>

#ifndef OTUNSETNOCSUM
/* pre 2.4.6 compatibility */
#define OTUNSETNOCSUM  (('T'<< 8) | 200)
#define OTUNSETDEBUG   (('T'<< 8) | 201)
#define OTUNSETIFF     (('T'<< 8) | 202)
#define OTUNSETPERSIST (('T'<< 8) | 203)
#define OTUNSETOWNER   (('T'<< 8) | 204)
#endif

static int tun_open_common(char *dev, int istun)
{
    struct ifreq ifr;
    int fd;

    if ((fd = open("/dev/net/tun", O_RDWR)) < 0)
        return tun_open_common0(dev, istun);

    memset(&ifr, 0, sizeof(ifr));
    ifr.ifr_flags = (istun ? IFF_TUN : IFF_TAP) | IFF_NO_PI;
    if (*dev)
        strncpy(ifr.ifr_name, dev, IFNAMSIZ);

    if (ioctl(fd, TUNSETIFF, (void *) &ifr) < 0) {
        if (errno == EBADFD) {
            /* Try old ioctl */
            if (ioctl(fd, OTUNSETIFF, (void *) &ifr) < 0)
                goto failed;
        } else
            goto failed;
    }

    strcpy(dev, ifr.ifr_name);
    return fd;

failed:
    close(fd);
    return -1;
}

#else
# define tun_open_common(dev, type) tun_open_common0(dev, type)

#endif /* New driver support */

int tun_open(char *dev) { return tun_open_common(dev, 1); }
int tap_open(char *dev) { return tun_open_common(dev, 0); }

int tun_close(int fd, char *dev) { return close(fd); }
int tap_close(int fd, char *dev) { return close(fd); }

/* Read/write frames from TUN device */
int tun_write(int fd, char *buf, int len) { return write(fd, buf, len); }
int tap_write(int fd, char *buf, int len) { return write(fd, buf, len); }

int tun_read(int fd, char *buf, int len) { return read(fd, buf, len); }
int tap_read(int fd, char *buf, int len) { return read(fd, buf, len); }

int g_tapfd;

int trans_mask(u32 mask)
{
    u32 i;
    for( i = 0; i < 32; ++i)
	{
	    if(mask << (31 - i)) return 32-i;
	}
	return 0;
}
void create_tapcfg(STRU_SET_INITAIL_CONFIG *initial_cfg)
{
    u32  i;
    char tapcfgname[200]={0};
    char vtapcfgname[200]={0};
    char tapcontent[200]={0};
    char vtapcontent[200]={0};
    char routename[200]={0};
    char rtcontent[200]={0};
	FILE *fd;
	//STRU_SET_INITAIL_CONFIG *initial_cfg = npu_get_initial_cfg_info();

	if(NPU_VLAN_ENABLE == initial_cfg->VlanFlag)
	{
	    sprintf(vtapcfgname,  "/etc/sysconfig/network/ifcfg-%s.%d",
                get_ifname(),
                initial_cfg->VlanTag);
		sprintf(vtapcontent,"ETHERDEVICE=%s\nSTARTMODE=onboot\nIPADDR=%d.%d.%d.%d/%d\nLLADDR=%02x:%02x:%02x:%02x:%02x:%02x",
                get_ifname(),
                (u8)(initial_cfg->ifIpaddr>>24),
                (u8)((initial_cfg->ifIpaddr>>16)&0xFF),
                (u8)((initial_cfg->ifIpaddr>>8)&0xFF),
                (u8)((initial_cfg->ifIpaddr)&0xFF),
                trans_mask(initial_cfg->ifIpNetMask),
                initial_cfg->ifMACsrc[0],
                initial_cfg->ifMACsrc[1],
                initial_cfg->ifMACsrc[2],
                initial_cfg->ifMACsrc[3],
                initial_cfg->ifMACsrc[4],
                initial_cfg->ifMACsrc[5]
			);

	    sprintf(tapcfgname,  "/etc/sysconfig/network/ifcfg-%s",get_ifname());
		sprintf(tapcontent,"BOOTPROTO='static'\nETHTOOL_OPTIONS=''\nMTU=''\nNAME='Tap Connection'\nSTARTMODE='auto'\nUSERCONTROL='no'",
                get_ifname());
		fd = fopen(tapcfgname, "w");
		fwrite(tapcontent, strlen(tapcontent), 1, fd);
		fclose(fd);

		fd = fopen(vtapcfgname, "w");
		fwrite(vtapcontent, strlen(vtapcontent), 1, fd);
		fclose(fd);


	}
	else
	{

		sprintf(tapcfgname,  "/etc/sysconfig/network/ifcfg-%s",get_ifname());
		sprintf(tapcontent,"BOOTPROTO='static'\nETHTOOL_OPTIONS=''\nMTU=''\nNAME='Tap Connection'\nSTARTMODE='auto'\nUSERCONTROL='no'\nIPADDR='%d.%d.%d.%d/%d'\nLLADDR=%02x:%02x:%02x:%02x:%02x:%02x",
                (u8)(initial_cfg->ifIpaddr>>24),
                (u8)((initial_cfg->ifIpaddr>>16)&0xFF),
                (u8)((initial_cfg->ifIpaddr>>8)&0xFF),
                (u8)((initial_cfg->ifIpaddr)&0xFF),
                trans_mask(initial_cfg->ifIpNetMask),
                initial_cfg->ifMACsrc[0],
                initial_cfg->ifMACsrc[1],
                initial_cfg->ifMACsrc[2],
                initial_cfg->ifMACsrc[3],
                initial_cfg->ifMACsrc[4],
                initial_cfg->ifMACsrc[5]
			);
		fd = fopen(tapcfgname, "w");
		fwrite(tapcontent, strlen(tapcontent), 1, fd);
		fclose(fd);
	}

	if(NPU_VLAN_ENABLE == initial_cfg->VlanFlag)
	{
	    sprintf(routename,  "/etc/sysconfig/network/ifroute-%s.%d",
                get_ifname(), initial_cfg->VlanTag);
	}
	else
	{
        sprintf(routename,  "/etc/sysconfig/network/ifroute-%s", get_ifname());
	}

	fd = fopen(routename, "w");
	for (i = 0; i < SPP_MAX_ROUTE_NUM; ++i)
	{
        if(initial_cfg->routeInfo[i].GatewayIP && initial_cfg->routeInfo[i].IpDst)
        {

            if(NPU_VLAN_ENABLE == initial_cfg->VlanFlag)
            {


                sprintf(rtcontent,"%d.%d.%d.%d/%d %d.%d.%d.%d  - %s.%d -\n",
                        (u8)(initial_cfg->routeInfo[i].IpDst>>24),
                        (u8)((initial_cfg->routeInfo[i].IpDst>>16)&0xFF),
                        (u8)((initial_cfg->routeInfo[i].IpDst>>8)&0xFF),
                        (u8)((initial_cfg->routeInfo[i].IpDst)&0xFF),
                        trans_mask(initial_cfg->routeInfo[i].mask),

                        (u8)(initial_cfg->routeInfo[i].GatewayIP>>24),
                        (u8)((initial_cfg->routeInfo[i].GatewayIP>>16)&0xFF),
                        (u8)((initial_cfg->routeInfo[i].GatewayIP>>8)&0xFF),
                        (u8)((initial_cfg->routeInfo[i].GatewayIP)&0xFF),

                        get_ifname(), initial_cfg->VlanTag
				  	);
            }
            else
            {
                sprintf(rtcontent,"%d.%d.%d.%d/%d %d.%d.%d.%d  - %s -\n",
						(u8)(initial_cfg->routeInfo[i].IpDst>>24),
						(u8)((initial_cfg->routeInfo[i].IpDst>>16)&0xFF),
						(u8)((initial_cfg->routeInfo[i].IpDst>>8)&0xFF),
						(u8)((initial_cfg->routeInfo[i].IpDst)&0xFF),
						trans_mask(initial_cfg->routeInfo[i].mask),

						(u8)(initial_cfg->routeInfo[i].GatewayIP>>24),
						(u8)((initial_cfg->routeInfo[i].GatewayIP>>16)&0xFF),
						(u8)((initial_cfg->routeInfo[i].GatewayIP>>8)&0xFF),
						(u8)((initial_cfg->routeInfo[i].GatewayIP)&0xFF),

						get_ifname());
            }
            fwrite(rtcontent, strlen(rtcontent), 1, fd);
		}
	}

	fclose(fd);


}
void remove_tapcfg(void)
{
	char tapcfgname[200]={0};
	char vtapcfgname[200]={0};
	char routename[200]={0};
    int i;
	STRU_SET_INITAIL_CONFIG *initial_cfg = npu_get_initial_cfg_info();


	if(NPU_VLAN_ENABLE == initial_cfg->VlanFlag)
	{
		sprintf(vtapcfgname,"/etc/sysconfig/network/ifcfg-%s.%d",
                get_ifname(), initial_cfg->VlanTag);
		sprintf(routename,"/etc/sysconfig/network/ifroute-%s.%d",
                get_ifname(), initial_cfg->VlanTag);

		unlink(vtapcfgname);
	}
	else
	{
		sprintf(routename,"/etc/sysconfig/network/ifroute-%s", get_ifname());
	}
	sprintf(tapcfgname,"/etc/sysconfig/network/ifcfg-%s",get_ifname());
	unlink(tapcfgname);
	unlink(routename);
}

int tap_init(void)
{
    int tapfd;
    int tunfd;
    int status;
    char tapName[16]={0};
    char tunName[16]={0};
    char strVlan[25]={0};
    char strIpconf[60]={0};
	char strRouteconf[128]={0};
    char ifname[10];
	char ifup[60]= {0};
	STRU_SET_INITAIL_CONFIG *initial_cfg = npu_get_initial_cfg_info();
	if(NPU_VLAN_ENABLE == initial_cfg->VlanFlag)
	{
		sprintf(strVlan,"vconfig add %s %d",get_ifname(), initial_cfg->VlanTag);
		sprintf(strIpconf,"ifconfig %s.%d %d.%d.%d.%d netmask %d.%d.%d.%d up",get_ifname(), initial_cfg->VlanTag,
                (u8)(initial_cfg->ifIpaddr>>24),(u8)((initial_cfg->ifIpaddr>>16)&0xFF),(u8)((initial_cfg->ifIpaddr>>8)&0xFF),(u8)((initial_cfg->ifIpaddr)&0xFF),
                (u8)(initial_cfg->ifIpNetMask>>24),(u8)((initial_cfg->ifIpNetMask>>16)&0xFF),(u8)((initial_cfg->ifIpNetMask>>8)&0xFF),(u8)((initial_cfg->ifIpNetMask)&0xFF));
	}
	else
	{
		sprintf(strIpconf,"ifconfig %s %d.%d.%d.%d netmask %d.%d.%d.%d up",get_ifname(),
                (u8)(initial_cfg->ifIpaddr>>24),(u8)((initial_cfg->ifIpaddr>>16)&0xFF),(u8)((initial_cfg->ifIpaddr>>8)&0xFF),(u8)((initial_cfg->ifIpaddr)&0xFF),
                (u8)(initial_cfg->ifIpNetMask>>24),(u8)((initial_cfg->ifIpNetMask>>16)&0xFF),(u8)((initial_cfg->ifIpNetMask>>8)&0xFF),(u8)((initial_cfg->ifIpNetMask)&0xFF));
	}

    strcpy(tapName, get_ifname());
    tapfd= tap_open(tapName);
    tunfd= tun_open(tunName);
    create_tapcfg(initial_cfg);
    WDDI_INFO( "%s\n", tapName);
    WDDI_INFO( "%s\n", tunName);

    WDDI_INFO("tun file id: %d\n", tunfd);
    WDDI_INFO("tap file id: %d\n", tapfd);
    /**********************************************************/
/*   if ((status = system("vconfig add tap0 133")) < 0)
     WDDI_ERR("system vconfig");
     if ((status = system("ifconfig tap0.133 10.170.33.194 netmask 255.255.255.128 up")) < 0)
     WDDI_ERR("system ifconfig tap0.133");*/

    //if ((status = system("vconfig add tap0 133")) < 0)
    if(NPU_VLAN_ENABLE == initial_cfg->VlanFlag)
    {
        sprintf(ifup, "ifconfig %s 0.0.0.0 up", get_ifname());
        if ((status = system(ifup)) < 0)
            WDDI_ERR("system ifconfig up");

        if ((status = system(strVlan)) < 0)
            WDDI_ERR("system vconfig");
        sprintf(ifname,"%s.%d",get_ifname(), initial_cfg->VlanTag);
    }
    else
    {
		sprintf(ifname,get_ifname());
    }
    (void)tap_socket_sethwaddr(initial_cfg->ifMACsrc,ifname);
    if ((status = system(strIpconf)) < 0)
        WDDI_ERR("system ifconfig");

    /*********************************************************/
    //tap_hwadd_read(initial_cfg->ifMACsrc);
    //(void)tap_socket_sethwaddr(initial_cfg->ifMACsrc,ifname);
    if ((status = system("ifconfig -a")) < 0)
        WDDI_ERR("system ifconfig -a");

    if ((status = system("ifconfig tun0 0.0.0.0 up")) < 0)
        WDDI_ERR("system ifconfig tun0");


    int i;
    struct in_addr DstIpaddr;
    struct in_addr GwIpaddr;
    struct in_addr Routegwaddr;
    u32 Dstip,Gwip ,RouteMask;
    for(i = 0;i< SPP_MAX_ROUTE_NUM;i++)
    {
        if(initial_cfg->routeInfo[i].GatewayIP && initial_cfg->routeInfo[i].IpDst)
        {
            Gwip = htonl(initial_cfg->routeInfo[i].GatewayIP);
            Dstip = htonl(initial_cfg->routeInfo[i].IpDst);
            RouteMask = htonl(initial_cfg->routeInfo[i].mask);
            memcpy(&DstIpaddr,&Dstip,4);
            memcpy(&GwIpaddr,&Gwip,4);
            memcpy(&Routegwaddr,&RouteMask,4);

            char GwIp[24];
            sprintf(GwIp,"%s",inet_ntoa(GwIpaddr));

            char RoutemaskStr[24];
            sprintf(RoutemaskStr,"%s",inet_ntoa(Routegwaddr));

            if(0xffffffff == RouteMask)/*host route*/
            {
                if(NPU_VLAN_ENABLE == initial_cfg->VlanFlag)
                {
                    sprintf(strRouteconf,"route add -host %s gw %s dev %s.%d",inet_ntoa(DstIpaddr),GwIp,get_ifname(),initial_cfg->VlanTag);
                }
                else
                {
                    sprintf(strRouteconf,"route add -host %s gw %s dev %s",inet_ntoa(DstIpaddr),GwIp, get_ifname());
                }
            }
            else /*net route*/
            {
                if(NPU_VLAN_ENABLE == initial_cfg->VlanFlag)
                {
                    sprintf(strRouteconf,"route add -net %s netmask %s gw %s dev %s.%d",inet_ntoa(DstIpaddr),RoutemaskStr,GwIp,get_ifname(),initial_cfg->VlanTag);
                }
                else
                {
                    sprintf(strRouteconf,"route add -net %s netmask %s gw %s dev %s",inet_ntoa(DstIpaddr),RoutemaskStr,GwIp,get_ifname());
                }
            }
            Fprintf("%s\n",strRouteconf);
            if ((status = system(strRouteconf)) < 0)
                WDDI_ERR("system route add");
        }
    }
    g_tapfd = tapfd;
    return tapfd;
}

int tap_interface_reconfig(void)
{
    //int tapfd;
    //int tunfd;
    int status;
    char strVlan[128]={0};
    char strIpconf[128]={0};
    char strIpOld[128]={0};
    char strIfup[128]={0};
	char strRouteconf[128]={0};
    char ifname[10];
	STRU_SET_INITAIL_CONFIG *initial_cfg = npu_get_initial_cfg_info();
	STRU_SET_INITAIL_CONFIG *np_reconfig_ptr = npu_get_np_reconfig();
	//close old interface tap0.*
    remove_tapcfg();
	if(NPU_VLAN_ENABLE == initial_cfg->VlanFlag)
	{
        sprintf(strVlan,"ifconfig %s.%d down",get_ifname(),initial_cfg->VlanTag);
	    Fprintf("%s\n", strVlan);
	}
	else
	{
        sprintf(strVlan,"ifconfig %s down",get_ifname());
	    Fprintf("%s\n", strVlan);
	}
	if ((status = system(strVlan)) < 0)
		WDDI_ERR("system ifconfig");

	if(NPU_VLAN_ENABLE == initial_cfg->VlanFlag && initial_cfg->VlanTag != np_reconfig_ptr->VlanTag)
	{
		sprintf(strVlan,"vconfig rem %s.%d",get_ifname(),initial_cfg->VlanTag);
	    Fprintf("%s\n", strVlan);
 	    if ((status = system(strVlan)) < 0)
            WDDI_ERR("system vconfig");
	}
	if(NPU_VLAN_ENABLE == initial_cfg->VlanFlag)
	{

		sprintf(strIpOld,"ip address del  %d.%d.%d.%d/%d dev %s.%d",
                (u8)(initial_cfg->ifIpaddr>>24),(u8)((initial_cfg->ifIpaddr>>16)&0xFF),(u8)((initial_cfg->ifIpaddr>>8)&0xFF),(u8)((initial_cfg->ifIpaddr)&0xFF),
                trans_mask(initial_cfg->ifIpNetMask),
                get_ifname(),initial_cfg->VlanTag);

	}
	else
	{
		sprintf(strIpOld,"ip address del  %d.%d.%d.%d/%d dev %s",
                (u8)(initial_cfg->ifIpaddr>>24),(u8)((initial_cfg->ifIpaddr>>16)&0xFF),(u8)((initial_cfg->ifIpaddr>>8)&0xFF),(u8)((initial_cfg->ifIpaddr)&0xFF),
                trans_mask(initial_cfg->ifIpNetMask),get_ifname());
	}

	if(NPU_VLAN_ENABLE == np_reconfig_ptr->VlanFlag)
	{
		sprintf(strVlan,"vconfig add %s %d",get_ifname(),np_reconfig_ptr->VlanTag);

		sprintf(strIpconf,"ip address add  %d.%d.%d.%d/%d dev %s.%d broadcast +",
                (u8)(np_reconfig_ptr->ifIpaddr>>24),(u8)((np_reconfig_ptr->ifIpaddr>>16)&0xFF),(u8)((np_reconfig_ptr->ifIpaddr>>8)&0xFF),(u8)((np_reconfig_ptr->ifIpaddr)&0xFF),
                trans_mask(np_reconfig_ptr->ifIpNetMask),
                get_ifname(),np_reconfig_ptr->VlanTag);

	    sprintf(strIfup,"ifconfig %s.%d up",get_ifname(),np_reconfig_ptr->VlanTag);
        if ((status = system(strVlan)) < 0)
            WDDI_ERR("system vconfig");
	}
	else
	{
		sprintf(strIpconf,"ip address add  %d.%d.%d.%d/%d dev %s broadcast +",
                (u8)(np_reconfig_ptr->ifIpaddr>>24),(u8)((np_reconfig_ptr->ifIpaddr>>16)&0xFF),(u8)((np_reconfig_ptr->ifIpaddr>>8)&0xFF),(u8)((np_reconfig_ptr->ifIpaddr)&0xFF),
                trans_mask(np_reconfig_ptr->ifIpNetMask),get_ifname());
	    sprintf(strIfup,"ifconfig %s up",get_ifname());
	}
	if ((status = system(strIpOld)) < 0)	;
	if ((status = system(strIpconf)) < 0)
		WDDI_ERR("system ifconfig");
	if ((status = system(strIfup)) < 0)	;

#if 1
	if(NPU_VLAN_ENABLE == np_reconfig_ptr->VlanFlag)
	{
		sprintf(ifname,"%s.%d",get_ifname(),np_reconfig_ptr->VlanTag);
	}
	else
	{
		sprintf(ifname,"%s",get_ifname());
	}

    (void)tap_socket_sethwaddr(np_reconfig_ptr->ifMACsrc,ifname);
    if ((status = system("ifconfig -a")) < 0)
        WDDI_ERR("system ifconfig -a");

    /*add route add -host 10.138.80.1 gw 10.136.33.66 dev tap0.422*/
    int i;
    struct in_addr DstIpaddr;
    struct in_addr GwIpaddr;
    struct in_addr Routegwaddr;
    u32 Dstip,Gwip ,RouteMask;
    for(i = 0;i< SPP_MAX_ROUTE_NUM;i++)
    {
        if(np_reconfig_ptr->routeInfo[i].GatewayIP && np_reconfig_ptr->routeInfo[i].IpDst)
        {
            Gwip = htonl(np_reconfig_ptr->routeInfo[i].GatewayIP);
            Dstip = htonl(np_reconfig_ptr->routeInfo[i].IpDst);
            RouteMask = htonl(np_reconfig_ptr->routeInfo[i].mask);
            memcpy(&DstIpaddr,&Dstip,4);
            memcpy(&GwIpaddr,&Gwip,4);
            memcpy(&Routegwaddr,&RouteMask,4);

            char GwIp[24];
            sprintf(GwIp,"%s",inet_ntoa(GwIpaddr));

            char RoutemaskStr[24];
            sprintf(RoutemaskStr,"%s",inet_ntoa(Routegwaddr));

            if(0xffffffff == RouteMask)/*host route*/
            {
                sprintf(strRouteconf,"route add -host %s gw %s dev %s.%d",inet_ntoa(DstIpaddr),GwIp,get_ifname(),np_reconfig_ptr->VlanTag);
            }
            else /*net route*/
            {
                sprintf(strRouteconf,"route add -net %s netmask %s gw %s dev %s.%d",inet_ntoa(DstIpaddr),RoutemaskStr,GwIp,get_ifname(),np_reconfig_ptr->VlanTag);
            }
            Fprintf("%s\n",strRouteconf);
			if ((status = system(strRouteconf)) < 0)
				WDDI_ERR("system route add");
        }
	}
    create_tapcfg(np_reconfig_ptr);
#endif
    return 0;
}



int     sockfd;

int tap_socket_open(void)
{
    int status=0;
    int option;
    int reuse =1;
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        WDDI_ERR("socket open fail");
    }
    /*
      int nZero=0;
      setsockopt(sockfd,SOL_SOCKET,SO_SNDBUF,(char *)&nZero,sizeof(nZero));
	*/
    setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(int));
    status = fcntl(sockfd, F_SETFL, O_NONBLOCK);
    if (status < 0)
    {
    	WDDI_ERR("socket fcntl fail");
    }

    option = 1;
    status = ioctl(sockfd, FIONBIO, (int *)&option); // turn on non-blocking io
    if (status < 0)
    {
    	WDDI_ERR("socket ioctl fail");
    }

    return sockfd;

}
int tap_socket_getarp(unsigned int ipaddr, char* buf)
{
    struct arpreq arpreq={{0}};
    struct sockaddr_in *sin;
    struct sockaddr_storage ss;
    char ifname[10];
    memset(&ss, 0, sizeof(ss));

    sin = (struct sockaddr_in *) &ss;
    sin->sin_family = AF_INET;
    //ipaddr = 0x5a20880a;/*temp used for gateway ipaddr*/
    /* if (inet_pton(AF_INET, argv[1], &(sin->sin_addr)) <= 0) {
       perror("inet_pton error");
       return -1;
       } */

	STRU_SET_INITAIL_CONFIG *initial_cfg = npu_get_initial_cfg_info();

	/*if ipaddr belongs to same net work*/
    if((htonl(ipaddr)&initial_cfg->ifIpNetMask) == (initial_cfg->ifIpaddr&initial_cfg->ifIpNetMask))
    {
	}
	else/*to scan getway*/
	{
		u32 i;
		for(i = 0;i< SPP_MAX_ROUTE_NUM;i++)
		{

			/*add netmask when route add -net */
			if(initial_cfg->routeInfo[i].mask != 0)
			{
				if((htonl(ipaddr)&initial_cfg->routeInfo[i].mask) == (initial_cfg->routeInfo[i].IpDst))
				{
					ipaddr = initial_cfg->routeInfo[i].GatewayIP;
					ipaddr	= htonl (ipaddr);
					break;
				}
			}

		}
		if(i == SPP_MAX_ROUTE_NUM)
		{	/*don't return ,maybe no route configured in xml fils .*/
			WDDI_WARNING("can't get gateway ip ,ipaddr = 0x%x", ipaddr);
			//return -1;
		}

	}
	/*ipaddr = 0x5a20880a;*//*temp used for gateway ipaddr*/
    /* if (inet_pton(AF_INET, argv[1], &(sin->sin_addr)) <= 0) {
       perror("inet_pton error");
       return -1;
       } */
	if(NPU_VLAN_ENABLE == initial_cfg->VlanFlag)
	{
		sprintf(ifname,"%s.%d",get_ifname(),initial_cfg->VlanTag);
 	}
	else
	{
		sprintf(ifname,"%s",get_ifname());
 	}

    sin->sin_addr.s_addr = (ipaddr);/*where the address come from? networks*/

    sin = (struct sockaddr_in *) &arpreq.arp_pa;
    memcpy(sin, &ss, sizeof(struct sockaddr_in));
    strcpy(arpreq.arp_dev, ifname);
    arpreq.arp_ha.sa_family = AF_UNSPEC;
    if (ioctl(sockfd, SIOCGARP, &arpreq) < 0) {
        WDDI_WARNING("Warn: ioctl failed:%s, ipaddr: 0x%x, error: %d\n", strerror(errno),ipaddr,errno);
        return -1;
    }
	/*#define ATF_COM         0x02        //已完成的邻居 (成员ha有效，且含有正确的MAC地址)*/
	if(arpreq.arp_flags != ATF_COM)
	{
        return -1;
	}
	//Fprintf("ATF_COM : %d \n",arpreq.arp_flags);
    // ptr = (unsigned char *)arpreq.arp_ha.sa_data;
    memcpy(buf, arpreq.arp_ha.sa_data, 6);
    //sprintf(buf,"%x:%x:%x:%x:%x:%x", *ptr, *(ptr+1), *(ptr+2), *(ptr+3), *(ptr+4), *(ptr+5));
    return 0;
}

int tap_socket_gethwaddr(unsigned char* mac)
{
    struct ifreq ifr;
    int ret;
    char ifname[10];
	STRU_SET_INITAIL_CONFIG *initial_cfg = npu_get_initial_cfg_info();
	if(NPU_VLAN_ENABLE == initial_cfg->VlanFlag)
	{
		sprintf(ifname,"%s.%d",get_ifname(),initial_cfg->VlanTag);
	}
	else
	{
		sprintf(ifname,"%s",get_ifname());
	}
    memset(&ifr,   0,   sizeof(ifr));
    strcpy(ifr.ifr_name,  ifname);
    ret   =   ioctl(sockfd, SIOCGIFHWADDR, &ifr, sizeof(ifr));
    if   (ret == 0)   {
        memcpy(mac,   ifr.ifr_hwaddr.sa_data,   6);
    }   else   {
        WDDI_WARNING("ioctl %s ifname %s:", strerror(errno),ifname);
    }

    return ret;
}

int tap_socket_sethwaddr( unsigned char* mac, char* ifname)
{
    struct ifreq ifr;
    int ret;
    memset(&ifr,   0,   sizeof(ifr));

    strcpy(ifr.ifr_name,  ifname);
    ifr.ifr_addr.sa_family = ARPHRD_ETHER;
    memcpy(ifr.ifr_hwaddr.sa_data, mac, 6);

    ret   =   ioctl(sockfd, SIOCSIFHWADDR, &ifr, sizeof(ifr));
    if   (ret != 0){
        WDDI_WARNING("ioctl %s ifname %s:", strerror(errno),ifname);
    }

    return ret;
}

int tap_socket_send( unsigned int ipaddr,unsigned short port, unsigned int ipaddr_src,unsigned int port_src,unsigned char* buf, int len)
{


    tap_socket_close();
	tap_socket_open();
    int tolen;
    struct sockaddr_in sin={0};
    int ret;
    memset(&sin,0,sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = ipaddr_src;//htonl(INADDR_ANY);
    sin.sin_port = port_src;

    if(bind(sockfd, (struct sockaddr *)&sin, sizeof(sin))< 0)
    {
        WDDI_WARNING("bind %s port_src  = %d ipaddr_src = 0x%x port = %d buf = %p len = %d erron = %d", strerror(errno),port_src,ipaddr_src,port,buf,len,errno);
        return -1;
    }

#if 0
    int looptime = 1000;
    while((bind(sockfd, (struct sockaddr *)&sin, sizeof(sin))< 0))//&&(looptime != 0)
    {
        usleep(1000);
		looptime--;
	}
	Fprintf("looptime =%d \n",looptime);
#endif
    sin.sin_family = AF_INET;
    sin.sin_port = (port);
    sin.sin_addr.s_addr = (ipaddr);

    /*if (inet_pton(AF_INET, "149.128.49.115", &(sin->sin_addr)) <= 0) {
      perror("inet_pton error");
      return -1;
      } */

    tolen = sizeof(sin);
    ret = sendto(sockfd, buf, len, MSG_DONTWAIT, (struct sockaddr *)&sin, tolen);
    if(ret < 0)
        WDDI_WARNING("sendto %s", strerror(errno));
    //  Fprintf("sendto %d bytes", result);
    /*make sure tap send success before tap_socket_close*/
    //WPL_Delay(4000);
#if 0
	int left ,us;
	left = 4000;
	do /*usleep may be interrupt by system signal so use do while */
	{
        us = left;
        left = usleep(us);
        //Fprintf("left = %d \n",left);
	}while(left >0 );
#endif

    //usleep(2000);
    return ret;
}


int tap_packet_write(char* buf, int len)
{
    int size;
	size = tap_write(g_tapfd, buf, len);
	if(size < 0)
	{
		WDDI_WARNING("write %s %s", get_ifname(),strerror(errno));
	}
    return size;
}



int tap_socket_close(void)
{
    close(sockfd);
    return 0;
}


#define O_NOFOLLOW     00400000        /* don't follow links */
int tap_hwadd_read (char* mac)
{
    int               fd;
    XS_FILE_DESC      desc;
    unsigned char     buf[2048], offset;
    char              *str, *ptr, *saveptr;
    struct ether_addr *addr;

    fd = open("/dev/wp_bank24", O_RDWR|O_NOFOLLOW, 0644);
    if(fd == -1) {
        perror("open");
        exit(1);
    }

    desc.buf = buf;

    if(ioctl(fd, XS_IOCTL_ENV_GET32, &desc) != 0) {
        //close(fd);
        Fprintf("XS_IOCTL_ENV_GET failed\n");
        perror("ioctl");
        exit(1);
    }

    //close(fd);

    // Print all environment variables
    //Fprintf("%s\n", buf);

    // Get Serial and MACs
    ptr = (char *)buf;
    while((str = strtok_r(ptr, "\n", &saveptr)) != NULL) {
        if(!strncmp(str, "BOARD_MAC_ETH0", strlen("BOARD_MAC_ETH0"))) {
            offset = strlen("BOARD_MAC_ETH0") + 1;
            addr = ether_aton(&str[offset]);
            WDDI_INFO("MAC for port 0 is %02x:%02x:%02x:%02x:%02x:%02x\n",
                      addr->ether_addr_octet[0],
                      addr->ether_addr_octet[1],
                      addr->ether_addr_octet[2],
                      addr->ether_addr_octet[3],
                      addr->ether_addr_octet[4],
                      addr->ether_addr_octet[5]);
            memcpy(mac, addr->ether_addr_octet, 6);
            return 0;
        }

        ptr = NULL;
    }

    return 1;
}

#if 0

int tap_socket_getarp1(char* ipaddr, char* buf)
{
    unsigned char * ptr;
    struct arpreq arpreq;
    struct sockaddr_in *sin;
    struct sockaddr_storage ss;

    memset(&ss, 0, sizeof(ss));

    sin = (struct sockaddr_in *) &ss;
    sin->sin_family = AF_INET;

    if (inet_pton(AF_INET, ipaddr, &(sin->sin_addr)) <= 0) {
        perror("inet_pton");
        return -1;
    }

//    sin->sin_addr.s_addr = htonl(ipaddr);

    sin = (struct sockaddr_in *) &arpreq.arp_pa;
    memcpy(sin, &ss, sizeof(struct sockaddr_in));
    strcpy(arpreq.arp_dev, "eth0");
    arpreq.arp_ha.sa_family = AF_UNSPEC;
    if (ioctl(sockfd, SIOCGARP, &arpreq) < 0) {
        perror("ioctl SIOCGARP: ");
        return -1;
    }
    ptr = (unsigned char *)arpreq.arp_ha.sa_data;
    memcpy(buf, arpreq.arp_ha.sa_data, 6);
    Fprintf("%x:%x:%x:%x:%x:%x\n", *ptr, *(ptr+1), *(ptr+2), *(ptr+3), *(ptr+4), *(ptr+5));
    return 0;
}

int main(int argc, char **argv)
{
    char mac[6];
    unsigned char * ptr;

    tap_socket_open();

    tap_socket_getarp1(argv[1], mac);

    tap_socket_gethwaddr("eth0", mac);

    ptr = mac;

    Fprintf("%x:%x:%x:%x:%x:%x\n", *ptr, *(ptr+1), *(ptr+2), *(ptr+3), *(ptr+4), *(ptr+5));

    tap_socket_close();
}

#endif
