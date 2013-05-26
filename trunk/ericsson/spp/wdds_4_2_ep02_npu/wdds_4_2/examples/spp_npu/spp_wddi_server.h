

#ifndef __WDDI_SERVER_H
#define __WDDI_SERVER_H
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <linx.h>
#include "spp_npu_link.h"
#include "np_common.h"

#define USED   1
#define UNUSED 0

#define NPU_UDP_MAC_ADDR_LEN 6
#define NPU_UDP_VLAN_EN 	    1


#define NPU_UDP_ETHPACK_LEN_MAX         1536
#define NPU_UDP_ETHPACK_PROTECTION      64

#define NPU_UDP_ETH_TYPE_IPV4           0x0800

#define NPU_UDP_VLAN_TYPE_TAG           0x8100

#define NPU_UDP_VLAN_EN                 1

#define NPU_UDP_VLAN_DIS                0


/*IPV4*/
#define NPU_UDP_IPV4_VER                0x45

#define NPU_UDP_IPV4_LEN                4

#define NPU_UDP_IPV4_TOS                0

#define NPU_UDP_IPV4_ID                 0

#define NPU_UDP_IPV4_OFF                0x4000

#define NPU_UDP_IPV4_TTL                0x40

#define NPU_UDP_IPV4_PRO_UDP            0x11

#define NPU_UDP_IPV4_CKSUM              0

#define NPU_UDP_CHKSUM                  0

typedef struct NODE
{
    char *ht_key;
    struct NODE *next;
}NODE;

/*add for npumon*/

typedef struct STRU_VPORT_NPUMON_TAG
{
    LINX_SPID   client;
	u32 captureEnableFlag;/*NPU_NPUMON_CAPTURE_DISABLE or NPU_NPUMON_CAPTURE_ENABLE*/
	u32 len;
	int ifType;/*VP_TYPE_UDP OR VP_TYPE_FR*/
	unsigned int ipaddr;/*if ifType = VP_TYPE_UDP then it will be used*/
}STRU_VPORT_NPUMON;

typedef struct STRU_VPORT_INFO_TAG
{

	LINX_SPID      client;
	unsigned int   ipaddr_src;
	unsigned int   port_src;
	unsigned char  vp_type;
	unsigned int   UsedFlag;
	FR_CHANNEL_INFO ch_info;
	STRU_VPORT_NPUMON npumon_info;
	u32 agg_ul_ip2iptx2Enet1;
	u32 agg_dl_ip2iptx2Enet1;
	u32 agg_ip2iptx2host;
	u32 flow_ul_tx2enet1;
	u32 agg_ul_ip2iptx2Enet1ModifyFlag;
	u32 agg_ul_ip_to_enet2_handle;
	HOST_NS_BSSGP_STATICS ns_bssgp_statics;/*for ip*/
	struct NODE* bvci_nodehead;
	u16 nri_valid_talbe[257];
}STRU_VPORT_INFO;

typedef struct STRU_PEER_VPORT_INFO_TAG
{

	unsigned int   port_src;
	WP_handle agg_downlink_tx2peerfr;
}STRU_PEER_VPORT_INFO;

typedef struct STRU_LINX_SERVER_INFO_TAG
{
	LINX *linx;
    unsigned int   UsedFlag;
}STRU_LINX_SERVER_INFO;

typedef struct NPU_HOST_RSV_PACKET_INFO_TAG
{
    u32 vp_type;
	u32 length;
    u32 port; /*dst port*/
	u32 ipaddr_dst;
	u32 ipaddr_src;
	u16 port_src;
	u8  *pdata; /*udp payload*/
}NPU_HOST_RSV_PACKET_INFO;


/* Ethernet*/
typedef struct
{
    u8	ether_dhost[6];
    u8	ether_shost[6];
    u16	ether_type;
}
    STRU_NPU_ETH_HDR;

/* ETH header with VLAN */
typedef struct
{
    u8   ether_dhost[6];
    u8    ether_shost[6];
    u16   ether_type_tag;                /* 0x8100 always */
    u16   tag;                           /* Tag control information */
    /* protocol */
    /* IPv4: 0x0800
       ARP: 0x0806
       IPv6: 0x86dd */
    u16   ether_type;
}
    STRU_NPU_ETH_VLAN_HDR;

/* IPv4ͷ */
typedef struct
{

    u8	ip_vhl;			/* version << 4 | header length >> 2 */
    u8	ip_tos;			/* type of service */
    u16	ip_len;			/* total length */
    u16	ip_id;			/* identification */
    u16	ip_off;			/* fragment offset field */
    u8	ip_ttl;			/* time to live */
    u8	ip_p;			/* protocol */
    u16	ip_sum;			/* checksum */
    u32 ip_src;
    u32 ip_dst;	        /* source and dest address */
}
    STRU_NPU_IP_HDR;

/* UDPͷ */
typedef struct
{
    u16	uh_sport;		/* source port */
    u16	uh_dport;		/* destination port */
    u16	uh_ulen;		/* udp length */
    u16	uh_sum;			/* udp checksum */
}
    STRU_NPU_UDP_HDR;


STRU_VPORT_INFO * npu_get_vport_info(u32 port_src);
int npu_send_udp_packet(u8* p, u32 len,u32 ipaddr_src,u32 ipaddr_dst,u32 port_src, u32 port_dst);
int npu_set_vport_info(LINX_SPID client,u8 vp_type,u32 ipaddr_src,u32 port_src);
int npu_set_update_bvci_info(u32 vfport, u16 NS_BVCI_old, NP_UPDATE_BVCI_FLOW_INFO* flowinfo,STRU_VPORT_INFO  *VportInfo);
u16 npu_udp_cal_chksum(u8 *pu8ChkData, u32 u32ChkDataLen);
void npu_set_server_linx_info(LINX * linx);
void npu_set_server_rx_fr_thread_linx_info(LINX * linx);
void npu_set_server_rx_eth_thread_linx_info(LINX * linx);
int npu_set_trunk_info(u32 trunkid, NP_TRUNK_INFO* trunkinfo);
int npu_clear_vport_info(u32 port_src,u8 vp_type);
STRU_LINX_SERVER_INFO * npu_get_server_linx_info(void);
STRU_LINX_SERVER_INFO * npu_get_server_rx_fr_thread_linx_info(void);
STRU_LINX_SERVER_INFO * npu_get_server_rx_eth_thread_linx_info(void);

int npu_dispatch_packet_to_client(NPU_HOST_RSV_PACKET_INFO *StruRsvPacketInfo);
int npu_get_reply(LINX *linx, union LINX_SIGNAL **sig, LINX_SPID client);
int npu_handle_linx_msg(void);
int linx_init(void);
void npu_init_vport_info(void);
void npu_init_uplink_info(void);
void npu_init_downlink_info(void);
void npu_init_arp_info(void);
void npu_init_update_bvci_info(void);
void npu_init_trunk_info(void);
NP_ARP_INFO * npu_get_arp_info(u32 ip);
int npu_set_arp_info(u32 ip, NP_ARP_INFO* arpinfo);
NP_DL_FLOW_INFO * npu_get_downlink_info(u16 DportStartValue,u16 bvci);
NP_UL_FLOW_INFO * npu_get_uplink_info(u32 vfport,u32 NRIValue);
NP_UPDATE_BVCI_FLOW_INFO * npu_get_update_bvci_info(u32 vfport, u16 NS_BVCI_old);
STRU_SET_INITAIL_CONFIG * npu_get_initial_cfg_info(void);
STRU_SET_INITAIL_CONFIG * npu_get_np_reconfig(void);

NP_TRUNK_INFO * npu_get_trunk_info(u32 Trunkid);
void create_ht_link(NODE **nodehead,char *key);
void display_vport_ht_link(void);
void display_ul_route_ht_link(void);
void display_dl_route_ht_link(WP_U16 DportStartValue,WP_U16 bvci,u32 OptMode);
void display_arp_ht_link(void);
void display_update_bvci_ht_link(void);
void display_config_info(void);
int npu_set_uplink_info(NP_UL_FLOW_INFO* flowinfo);
int npu_set_downlink_info(NP_DL_FLOW_INFO* flowinfo);
int npu_hanlde_npumon_request(STRU_NPUMON_INFO * info,LINX_SPID client);
void npu_mon_capture_packet(char * buffer, u32 len,u32 port,int direction);
int npu_set_fr_dl_link_table_info(STRU_SET_FR_DOWNLINK_PATH_LINK_TABLE* linkInfo);
int npu_set_fr_ul_link_table_info(STRU_SET_UPLINK_PATH_LINK_TABLE* linkInfo);
void npu_init_fr_ul_link_table_info(void);
void npu_init_fr_dl_link_table_info(void);
void display_dl_link_table_ht_link(void);
void display_ul_link_table_ht_link(void);
STRU_SET_UPLINK_PATH_LINK_TABLE * npu_get_ul_link_table_info(u32 PortSrc);
STRU_SET_FR_DOWNLINK_PATH_LINK_TABLE * npu_get_dl_link_table_info(u16 NSEI);
NP_DL_FLOW_INFO * npu_get_dl_route_table_info(char * key);
u32 npu_set_initial_cfg_info(char *filename, STRU_SET_INITAIL_CONFIG *np_configure_ptr);
/* new added interface for xml file parse interface. */
extern expat_parser(char* filename,STRU_SET_INITAIL_CONFIG *np_configure_ptr);
extern cli_apply_configuration_files_from_dir(char* configuration_dir,STRU_SET_INITAIL_CONFIG *np_configure_ptr);
int np_close_vport_for_client(LINX_SPID client);
NP_UPDATE_BVCI_FLOW_INFO * npu_get_ul_bvci_table_info(char * key);
int np_mon_send_capture_data(char * buffer, u32 len,LINX_SPID client,int direction,u8 type);
int np_close_vport_for_npumon(LINX_SPID client);
int np_enable_vport_capture_for_npumon(STRU_NPUMON_INFO * info,LINX_SPID client);
void np_display_frag_flow_info(void);
int np_ul_ip_forward_to_enet2_create(STRU_VPORT_INFO  *VportInfo);
extern int g_logLevel;
int linx_init_thread_rx_fr(void);
int linx_init_thread_rx_eth(void);











#endif 			/* __WDDI_SERVER_H */
