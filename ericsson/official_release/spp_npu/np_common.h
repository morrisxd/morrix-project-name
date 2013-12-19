#ifndef __NPU_COMMON_H
#define __NPU_COMMON_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <assert.h>
#include "api/wp_wddi.h"
#include "api/wp_host_io.h"

#include "wpi_veneer.h"
#include "wpx_app_data.h"
#include "wpx_enet_phy.h"
#include "wpx_pin_mux_selector.h"
#include <wpx_board_data.h>
#include "np_stats.h"

#include <wufe_types.h>
#include <wufe_errors.h>
#include <wufei_user_config.h>
#include <wufe_user_config.h>
#include <wufe_ufe.h>
#include <wufe_events.h>
#include <wpx_ufe/wpx_frmr/flexmux/WO_FRMR_API_public.h> //for ufe4.2.2

//#include <wpx_ufe/wpx_frmr/flexmux/WPX_UFE_FRAMER_API_public.h>

#include "wti_flexmux_util.h"

#include "wddi_log.h"
#include "spp_npu_link.h"
#include "tun_dev.h"
#include "wp_bfd.h"

typedef WP_U32 WPI_PTR;


#ifndef WPI_LOOKUP_H
#include "include/core/wpi_lookup.h"
#endif
#define UFE_LOOPBACK 0
#define ENEA_RELEASE 1
#define USE_WDDI_LOG 0
#define BFD_OVERETH 0
#define IP_FRAG 1
#define IP_DEFRAG 1
#define START_SCHEDULER 1
#define FRAG_SIZE 1496
#define NPU_HOST_OLP  1
#define NPU_PRIORITY_MSG 1
#define NPU_WAIT_FINISHED 0xff
#define NPU_FR_RX_MAXT 20
#define NPU_FR_TX_MAXT 20
#define NPU_FR_RX_MAXT_PRI 4000

#if ENEA_RELEASE
#define NPU_WAIT_TIME     2
#else
#define NPU_WAIT_TIME     2
#endif
#if ENEA_RELEASE
#define printf(fmt, ...)   WDDI_WARNING(fmt,## __VA_ARGS__)
#endif
/*Clock selection*/
/*
  #define WPX_UFE_FRAMER_TRANSMIT_TIMING_RX_PATH                                 			(0)
  #define WPX_UFE_FRAMER_TRANSMIT_TIMING_INPUT_REFERENCE_CLOCK                   			(1)
  #define WPX_UFE_FRAMER_TRANSMIT_TIMING_SONET_TIMING			                  			(2)
  #define WPX_UFE_FRAMER_TRANSMIT_TIMING_WINPATH			                  				(3)
  #define WPX_UFE_FRAMER_TRANSMIT_TIMING_MAX				                  				(4)
*/
#define UFE_CLOCK_MASTER_MODE WPX_UFE_FRAMER_TRANSMIT_TIMING_SONET_TIMING//WPX_UFE_FRAMER_TRANSMIT_TIMING_SONET_TIMING
#define UFE_CLOCK_SLAVE_MODE  WPX_UFE_FRAMER_TRANSMIT_TIMING_RX_PATH
extern FILE *debug_log_file;
#if ENEA_RELEASE
#define Fprintf(fmt, ...) do {                      \
    if (debug_log_file != NULL)                     \
        fprintf(debug_log_file,fmt,## __VA_ARGS__); \
    else printf(fmt,## __VA_ARGS__); } while (0)
#else
#define Fprintf(fmt, ...) printf(fmt,## __VA_ARGS__)
#endif

#define NPU_DEBUG_FILE_ENABLE() do{ char fileName[60];                  \
	struct tm *p;                                                       \
    time_t timep;                                                       \
    time(&timep);                                                       \
    p= localtime(&timep);                                               \
    sprintf(fileName,"/var/log/spp_wddi_debug_test_%4d-%02d-%02d_%2.2d:%2.2d:%2.2d.log",p->tm_year+1900,p->tm_mon+1,p->tm_mday,p->tm_hour,p->tm_min,p->tm_sec); \
    debug_log_file = fopen(fileName,"w");                               \
    }while(0)

#define NPU_DEBUG_FILE_DISABLE() do {                   \
	if (debug_log_file != NULL)fclose(debug_log_file);  \
    debug_log_file = NULL;                              \
    }while(0)

/*UFE3 Specific Defines - Single Core, Single STM-1*/
#define N_UFE_CORES 1
#define N_UFE_MAX_LINES_T1 168//336
#define N_UFE_MAX_LINES_E1 126//252
#define N_UFE_MAX_PHYS 3200
#define N_UFE_MAX_LINES_SPE_E1 21
#define N_UFE_MAX_LINES_SPE_T1 28
#define N_MAX_SPE_PER_STM1 3
#define N_UFE_SPE_MAX_INDX WUFE_MAX_N_CORE*WUFE_MAX_N_STM1*WUFE_MAX_N_SPE
#define UFE_ID 0
#define N_FR_MAX_RX_HT_CHANNEL  3600
#define N_FR_MAX_IW_TX_CHANNEL  3600
#define N_FR_MAX_IW_RX_CHANNEL  3600
#define N_FR_MAX_SDU  1984
#define BPORT_DEFAULT_VLAN_TAG 0x0a
#define N_MAX_IP_FRAME_SIZE  8192

#define WT_MIN_CH_RATE    6000    /* calendar parameter 6K~3M bits/s*/
#define WT_MAX_CH_RATE    3000000 /* calendar parameter */

#define WTUFE_UPI_PORT_1 0
#define WTUFE_UPI_PORT_2 1

#define UFE_LB_SYSTEM 1
#define UFE_LB_LINE 2
#define UFE_NO_LOOPBK 3

#define ERR_UPLINX_FLOW_ADD       -1
#define ERR_DOWNLINX_FLOW_ADD     -2
#define ERR_UPLINX_FLOW_DELETE    -3
#define ERR_DOWNLINX_FLOW_DELETE  -4

#define ERR_BVCI_FLOW_ADD         -5
#define ERR_BVCI_FLOW_DELETE      -6
#define ERR_DOWNLINX_FLOW_EXIST   -7

#define  ERR_UL_RELEASE_RESOURCE   -8
#define  ERR_DL_RELEASE_RESOURCE   -9
#define  ERR_UPLINX_FLOW_ADD_ARP_FAIL   -10


#define N_ETH_MAX_HT_CHANNEL  10

#define N_FR_HT_RX_CHANNEL_TAG_BASE 0
#define N_ETH_HT_RX_CHANNEL_TAG_BASE N_FR_MAX_RX_HT_CHANNEL

#define RX_ETH_INT_TAG 65530
#define RX_ETH_BFD_INT_TAG 65531
#define RX_ETH_BFD_INT_TMO_TAG 65532
#define RX_ETH_INT_PRIO_MSG_TAG 65533
#define RX_FR_INT_PRIO_MSG_TAG 65534

#define UDP_BFD_PORT 3784
#define UDP_BFD_TMP_SPORT 49153

#define WP_PRIORIYT_MSG_FR_TYPE 0
#define WP_PRIORIYT_MSG_IP_TYPE 1

#define WP_FR_MAX_RSV_LOWPRIO_NUMBER  5
#define WP_FR_MAX_RSV_HIPRIO_NUMBER   20

#define WP_ETH_MAX_RSV_LOWPRIO_NUMBER 10
#define WP_ETH_MAX_RSV_HIPRIO_NUMBER  20


typedef struct
{
   	WP_U64 TxNsBytes;
	WP_U64 TxNsDataPackets;
	WP_U64 TxNsSignalPackets;
	WP_U64 TxBssgpBytes;
	WP_U64 TxBssgpDataPackets;
	WP_U64 TxBssgpSignalPackets;
}HOST_NS_BSSGP_STATICS;


typedef struct FR_CHANNEL_INFO_TAG
{
    WP_handle rx_iw_handle;
    WP_handle tx_iw_handle;
    WP_handle rx_host_handle;
    WP_handle tx_host_handle;
    WP_handle agg_fr_rx_host_flow;
    WP_handle agg_tx2Enet1ModifyFlag;
    WP_handle agg_tx2Enet1;/*dps will modify ip_dst port_src &port_dst*/
    WP_handle agg_fr_to_enet2;
    WP_handle phy_handle;
    WP_handle device_handle;/*host device*/
    WP_handle device_iw_handle;/*iw device*/
    WP_U32    vfport;
    WP_U32    QosEn;
    WP_U32    be;
    WP_U32    bc;
    WP_U32    cir;
    WP_U32    dlci;
    WP_U32    card;
    WP_U32    trunk;
    WP_U32    group;
    WP_U32    slot_selection;
    WP_handle agg_downlink_tx2fr_device;
    WP_handle agg_enet2_to_fr;
    WP_handle flow_enet2_to_fr;
}FR_CHANNEL_INFO;

typedef struct NP_UL_FLOW_INFO_TAG
{
	WP_U32 UsedFlag;
	WP_U32 vport;
	WP_U32 NRI;
	WP_U32 portSrc;
}NP_UL_FLOW_INFO;

typedef struct NP_UPDATE_BVCI_FLOW_INFO_TAG
{
	WP_handle flowHandle_bvci;
    WP_handle agg_updateBVCI_handle;
    WP_U32 UsedFlag;
	WP_U32  vport;
	WP_U16  NS_BVCI;
	WP_U16  NS_BVCI_old;
}NP_UPDATE_BVCI_FLOW_INFO;


typedef struct NP_DL_FLOW_INFO_TAG
{
	WP_handle flowHandle;
    WP_handle agg_dl_dportGrp_handle;
	WP_U32 UsedFlag;
    STRU_SET_DOWNLINK_PATH_RULE rule;
}NP_DL_FLOW_INFO;


typedef struct NP_ARP_INFO_TAG
{
	WP_U32 Ip;
	WP_U8 Mac[6];
	WP_handle agg_ipfrag;
	WP_handle agg_tx2eth1;
	WP_handle fragGrpHandle;
	WP_handle flowHandle;
	WP_U8 MacOld[6];
}NP_ARP_INFO;

typedef struct NP_TRUNK_INFO_TAG
{
    WP_U32 trunkid;
	WP_U32 GroupNum;
	WP_U32 Groupid[32];
}NP_TRUNK_INFO;

typedef struct NP_SYSTEM_INFO_TAG
{
    WP_handle system_fr_handle;
    WP_handle Bport_enet1_handle;
    WP_handle Bport_enet2host_handle;
    WP_handle agg_enet_default_host_handle;
    WP_handle agg_enet2_to_enet1;
    WP_handle agg_dl_ip_to_enet2_handle;
    /*add for npumon capture packet*/
    WP_U32    npumon_capture_enable_flag;/*NPU_NPUMON_CAPTURE_ENABLE NPU_NPUMON_CAPTURE_DISALBE*/
    WP_handle flow_default_mfc;
    WP_handle ip_defrag_group;
    WP_handle system_route_send2eth1_handle;
    WP_handle Rport_enet1_hanlde; /*route port*/
    WP_handle system_bfd_handle;
    WP_handle Bport_bfd_handle;
    WP_handle agg_bfd_tx_handle;
    WP_handle bfd_tx_table_handle;
    WP_handle agg_bfd_rx_handle;
    WP_handle bfd_rx_table_handle;
    WP_handle flow_bdf_handle;
    WP_handle Bport_mac_filter_handle;
    WP_handle agg_mac_filter_handle;
    WP_handle flow_mac_filter_handle;
    WP_handle agg_mac_filter_from_eth2_handle;
    WP_handle Bport_mac_filter_from_eth2_handle;
    WP_handle Channel_IP_PrioRx0_host_handle;
    WP_handle agg_enet_PrioRx0_host_handle;
    WP_handle Channel_FR_PrioRx0_host_handle;
    WP_handle agg_FR_PrioRx0_host_handle;
    WP_handle agg_host_arp_agg;
    WP_handle agg_after_defrag;
}NP_SYSTEM_INFO;

typedef struct NP_ALARM_STATUS_TAG
{
    u32 stm1_status;
    u32 e1_status;
}NP_ALARM_STATUS;

typedef struct
{
	unsigned int dlci:16;
	unsigned int tsgroup:6;
    unsigned int trunk:7;
	unsigned int card:3;
}fr_addr_t;

typedef enum
{
    NPU_INITALIZED = 0,
    NPU_UNINITALIZED
}NP_Flag;

/*add for QOS parameters*/
typedef struct STRU_PVC_QOS_INFO_TAG
{
    u8    adminState;/*0:enable 1:disable*/
	u32	  be;
	u32	  bc;
	u32	  cir;
}STRU_PVC_QOS_INFO;

typedef struct STRU_E1_GROUP_INFO_TAG
{
    u8 crc_type;             /*NPU_E1T1_CRC16
	                           NPU_E1T1_CRC32 */
	u16 max_frame_size;      /*MTU on this phy*/
    u8 groupValid;          /*NPU_VALID or NPU_INVALID*/
    u32 slot_selection;      /*slot_selection = 0xfffefffe; means use slot1~slot15 slot17~slot31*/
	STRU_PVC_QOS_INFO PVCQos[1025];/*dlci 0~1024*/

}STRU_E1_GROUP_INFO;

typedef struct STRU_E1_TRUNK_INFO_TAG
{
    u32 Opt_Mode;      /* details in E1T1OptMode struct ,for T1 means framing for Ericsson */
    u8 trunkValid;      /*NPU_VALID or NPU_INVALID*/
	u32 clock_mode;    /*NPU_E1T1_CLOCK_MASTER
						 NPU_E1T1_CLOCK_SLAVE */
    STRU_E1_GROUP_INFO group_info[SPP_MAX_GROUP_NUM];
}STRU_E1_TRUNK_INFO;

typedef struct STRU_STM1_CFG_INFO_TAG
{
    u8 stm1Valid;     /*NPU_VALID or NPU_INVALID*/
    STRU_E1_TRUNK_INFO trunk_info[SPP_MAX_TRUNK_NUM];
	/*will add other pramaters later*/
    u32 E1T1type;      /*NPU_MODE_T1 OR NPU_MODE_E1*/
}STRU_STM1_CFG_INFO;

typedef struct STRU_STM1_RECFG_TAG
{
    u32 trunkId;
    STRU_E1_GROUP_INFO group_info[SPP_MAX_GROUP_NUM];
}STRU_STM1_RECFG;

/*route info dst and gateway*/
typedef struct STRU_ROUTE_INFO_TAG
{
    u32 IpDst;
	u32 mask;
	u32 GatewayIP;
}STRU_ROUTE_INFO;

typedef struct STRU_SET_INITAIL_CONFIG_TAG
{
    u32 VlanFlag;        /*NPU_VLAN_ENABLE or NPU_VLAN_DISABLE*/
	u32 VlanTag;         /*vlan tag value only valid when VlanFlag = NPU_VLAN_ENABLE*/
	u32 OpterateMode;    /*NPU_MODE_FR or NPU_MODE_IP*/
	u8  ifMACsrc[6];     /*tap0 interface MAC SRC*/
	u32 ifIpaddr;        /*tap0 interface ipaddr*/
	u32 ifIpNetMask;     /*tap0 interface netmask*/
	u32 stm1_num;        /*the total using number stm-1 port*/
	u32 NRI_bitLen;	     /*gloable configure value is 1~8*/
	u32 PortPoolSize;    /*how many port from DportStartValue*/
    u32 PacketMemTotalByte;
	u32 ParamMemTotalByte;
	u32 InterMemTotalByte;
	STRU_STM1_CFG_INFO stru_stm1_cfg_info[SPP_MAX_STM1_PORT_NUM];
	STRU_ROUTE_INFO routeInfo[SPP_MAX_ROUTE_NUM];
}STRU_SET_INITAIL_CONFIG;

typedef enum
{
	OPMODE_T1_D4 = 0x0001,
	OPMODE_T1_ESF   = 0x0002,

	OPMODE_E1_TRUE_CRC4 = 0x0008,
	OPMODE_E1_FALSE = 0x0010,
}E1T1OptMode;

typedef struct STRU_STM1_SDH_ALARM_TAG
{
	u8 SDH_LOS;
	u8 SDH_LOF;
	u8 SDH_RS_TIM;
	u8 SDH_MS_AIS;
	u8 SDH_MS_RDI;
	u8 SDH_OOF;
	u8 SDH_MS_EXC;
	u8 SDH_MS_DEG;
	u8 SDH_AU_AIS;
	u8 SDH_AU_LOP;
	u8 SDH_HP_LOM;
	u8 SDH_HP_TIM;
	u8 SDH_HP_PLM;
	u8 SDH_HP_UNEQ;
	u8 SDH_HP_RDI;
	u8 SDH_HP_EXC;
	u8 SDH_HP_DEG;
	u8 SDH_LP_TIM;
	u8 SDH_LP_PLM;
	u8 SDH_LP_UNEQ;
	u8 SDH_LP_RDI;
	u8 SDH_LP_EXC;
	u8 SDH_LP_DEG;
	u8 SDH_TU_AIS;
	u8 SDH_TU_LOP;
}STRU_STM1_SDH_ALARM;

typedef struct STRU_STM1_E1T1_ALARM_INFO_TAG
{
	u8 LOS;
    u8 LOF;
    u8 AIS;
    u8 AIS_CI;
    u8 RAI;
    u8 RAI_CI;
}STRU_STM1_E1T1_ALARM_INFO;

typedef struct STRU_STM1_SDH_ALARM_INFO_TAG
{
	STRU_STM1_SDH_ALARM SDH_alarm[SPP_MAX_STM1_PORT_NUM];
	STRU_STM1_E1T1_ALARM_INFO E1T1_alarm[SPP_MAX_STM1_PORT_NUM][SPP_MAX_TRUNK_NUM];
}STRU_STM1_ALARM_INFO;

typedef struct DportGrpaggInfo_TAG
{
	WP_handle handle;
	u16 PortSize;
}DportGrpaggInfo;

extern int g_npinitialFlag;
extern int g_npnonpoolFlag;
extern int g_npNseAggFlag;

WP_S32 np_init(void);
void np_display_ufe_system(void);
void np_display_ufe_phy(WP_U32 phyIndex);
void np_display_ufe_line(WP_U32 lineIndex);
void np_eth_host_send(WP_S32 DataLength, WP_CHAR* pTxData);
int np_eth_host_receive(WP_handle rx_host_handle,WP_S32 *DataLength, WP_CHAR* pTxData);
void npu_send_fr_packet(WP_U8 *p, WP_U32 len,WP_U32 vfport);
void np_fr_host_send(WP_handle tx_host_handle,WP_S32 DataLength, WP_CHAR* pTxData,WP_U32 dlci,WP_U32 port);
int np_fr_host_receive(WP_handle rx_host_handle,WP_S32 *DataLength, WP_CHAR* pTxData);
WP_U32 np_fraddr_pack(WP_U32 card, WP_U32 trunk, WP_U32 tsgroup, WP_U32 dlci);
void np_fraddr_unpack(WP_U32 frport, WP_U32* card, WP_U32* trunk, WP_U32* tsgroup, WP_U32* dlci);
int np_creat_fr_vport(WP_U32 vfport);
void np_creat_fr_host_channel(FR_CHANNEL_INFO * ch_info);
int np_close_fr_vport(WP_U32 vfport);
void np_disable_fr_host_channel(FR_CHANNEL_INFO * ch_info);
void np_delete_fr_host_channel(FR_CHANNEL_INFO * ch_info);

void np_enable_fr_host_channel(FR_CHANNEL_INFO * ch_info);
void np_frIwSystemCreate(NP_SYSTEM_INFO *np_system);
void np_creat_fr_iw_channel(FR_CHANNEL_INFO * ch_info);
NP_SYSTEM_INFO *np_get_system_info(void);
void np_display_flow_agg(WP_U32 vport);
void np_display_iw_port_stat(void);
void np_enable_fr_iw_channel(FR_CHANNEL_INFO * ch_info);
void np_disable_fr_iw_channel(FR_CHANNEL_INFO * ch_info);
void np_delete_fr_iw_channel(FR_CHANNEL_INFO * ch_info);
int np_ul_route_table_add(STRU_SET_UPLINK_PATH_RULE *uplink_rule);
int np_ul_route_table_delete(WP_U32 vfport,WP_U32 NRIValue);
int np_dl_route_table_add(STRU_SET_DOWNLINK_PATH_RULE * rule);
int np_dl_route_table_delete(WP_U16 DportStartValue,WP_U16 bvc,WP_U32 OptMode);
int np_updateBVCI_flow_delete(WP_U32 vfport,WP_U16 NS_BVCI_old,WP_U32 OptMode);
int np_updateBVCI_flow_add(STRU_SET_UPDATE_BVCI_PATH_RULE *rule);
int np_poll_report_alarm(void);
int np_ul_link_table_set(STRU_SET_UPLINK_PATH_LINK_TABLE *link_table);
int np_ul_link_table_delete(WP_U32 portsrc);
int np_dl_link_table_set(STRU_SET_FR_DOWNLINK_PATH_LINK_TABLE * link_table);
int npu_dl_link_table_delete(WP_U32 vfort);
void Np_ShowMemoryAvailable(STRU_MEMORY_STATICS *get_memory_statics);
void Np_Get_Utilization(STRU_CPU_STATICS *get_cpu_statics);
int np_ipdefrag_flow_add(void);
void IwHostTxChannel(WP_handle flow_agg);
void creat_IwHostTxChannel(WP_handle flow_agg);
void terminate_on_error(WP_handle handle, WP_CHAR *s);
int np_dl_release_vport_resource(u32 vport);
int np_ul_release_vport_resource(u32 vport);
int np_get_ns_bssgp_statics(u32 vport,STRU_GET_NS_BSSGP_STATICS * statics);
int np_clear_ns_bssgp_statics(u32 vport);
void np_host_set_ns_bssgp_statics(u8 * buf,u32 len,HOST_NS_BSSGP_STATICS * statics);
int np_dl_enet2_to_fr_rule_add(FR_CHANNEL_INFO * ch_info);
int np_dl_ip_forward_to_enet2_create(void);
int np_ul_fr_to_enet2_create(FR_CHANNEL_INFO * ch_info);
int np_enet2_to_enet1_rule_add(void);
int np_ul_reconfig_vport_vlan(u32 vport);
int np_dl_reconfig_vport_vlan(u32 vport);
void displayUfeEvents(void);
int tap_interface_reconfig(void);



extern WP_status WP_UserDefHashTableCreate(WP_U32 wpid,
                                           WP_U32 n_entries);
extern WP_status WP_DlLookupEntryInsert(WP_handle device,
                                        WP_handle elink,
                                        WP_U16 key1,
                                        wp_dl_rt *dl_rt);
extern WP_status WP_VPortEntryInsert(WP_handle device,
                                     WP_U16 key,
                                     wp_vport_table *vport_table);
extern WP_status WP_DlLookupEntryRemove(WP_handle device,
                                        WP_handle elink,
                                        WP_U16 key1);
extern WP_status WPI_UserDefHashEntryRemove (WP_handle dev_handle,
                                             WP_U32 key1,WP_U16 key2,WP_U16 data);
extern WP_status WPI_UserDefHashEntryInsert (WP_handle dev_handle,
                                             WP_U32 key1,WP_U16 key2,WP_U16 data);
extern WP_status WP_LookupEntryInsert(WP_handle device,
                                      WP_handle elink,
                                      WP_handle flow_agg_h[],
                                      WP_U32 nri,
                                      WP_U32 port_num);
extern WP_status WPI_UserDefHashEntryInsert (WP_handle dev_handle,
                                             WP_U32 key1,WP_U16 key2,WP_U16 data);
extern WP_status WP_LookupEntryRemove(WP_handle device,
                                      WP_handle elink,
                                      WP_U32 nri);

extern WP_status WP_IPEndPointEntryInsert(WP_handle device,
                                          WP_U32 key,
                                          wp_local_rt *local_rt);

extern WP_status WTUFE_Reconfig_Stm1(WP_U32 stm1id, WP_U32 line_index_base,STRU_STM1_RECFG *reconfig_info,STRU_SET_INITAIL_CONFIG *p2);
extern WP_status WTUFE_Reconfig_Stm1_inLoop(WP_U32 stm1id, WP_U32 trunkid, WP_U32 groupid, WP_U32 line_index_base,STRU_STM1_RECFG *reconfig_info,STRU_SET_INITAIL_CONFIG *p2);
extern WP_status WTUFE_Reconfig_Stm1_All(STRU_SET_INITAIL_CONFIG *p1, STRU_SET_INITAIL_CONFIG *p2);
extern WUFE_status WUFE_PhyDisplay(WUFE_handle ufe_phy_handle);
extern WUFE_status WUFE_LineDisplay(WUFE_handle ufe_line_handle);
extern WUFE_status WUFE_SystemDisplay(WP_U32 wufe_id);
extern WP_status WP_FlowStatisticsGet(WP_handle device,
                                      WP_U32 elink,
                                      wp_ns_bssgp_statis * statis_ptr);
extern WP_status WP_FlowStatisticsClear(WP_handle device,
                                        WP_U32 elink);
extern WP_status WP_PriorityMsgFlowSet(WP_handle device,
                                       WP_U16 type,/*FR or IP ,WP_FR_TYPE WP_IP_TYPE*/
                                       wp_priority_msg *flowtable);
extern WP_status WP_WDDI_ResWddiGet(WP_U32 fiwtIndex,wpi_res_fiwt *res_fiwt);
void WT_Np_UfeLineConfigureAndCreateE1(WP_U32 line_index, WP_U16 framing_mode, WP_U8 cas_enable,WP_U32 line_lb_mode,WP_U32 clock_mode);
void WT_Np_UfeLineConfigureAndCreateT1(WP_U32 line_index, WP_U16 framing_mode, WP_U8 cas_enable,WP_U32 line_lb_mode,WP_U32 clock_mode);
void WT_Np_UfeLineSdhConfig(WT_ufe_line_sdh_params *line_sdh_params,WUFE_line_sdh *line_cfg);
WP_S32 WTI_Np_FlexmuxClientPortIndexGet(WP_U32 ufe_line_index, WUFE_line_transf_type transfer_type);
WP_S32 WTI_Np_FlexmuxLinePortIndexGet(WP_U32 ufe_line_index, WP_U32 flexmux_id);
void WT_Np_UfeLineCreate(WP_U32 line_index,WUFE_line_sdh *line_cfg,WT_ufe_line_sdh_params *line_sdh_params,WP_U32 line_lb_mode);
void WTUFE_config_phys(WP_U32 line_index,WP_U32 groupId,WP_U32 slot_selection,WP_U32 line_index_base/*,WP_handle *phy_handle,WP_handle *device_handle*/);

STRU_STM1_ALARM_INFO* np_get_ufe4_aralm_info(void);
void np_display_ufe4_shd_status(void);
extern void np_set_ufe4_aralm_info(WP_U32 LinePort, WP_U32 AlarmCategory, WP_U32 IsAsserted);
extern void np_set_ufe4_E1T1_aralm_info(WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE * pLineEndpoint, WP_U32 AlarmCategory, WP_U32 IsAsserted);
extern void np_set_ufe4_E1T1_aralm_info_fromClientPort( WP_U32 ClientPort, WP_U32 AlarmCategory, WP_U32 IsAsserted);

int np_check_ufe4_stm1_status(WP_U32 LinePort);
void np_config_all_phys(void);
void np_dl_release_FMC_resource(u16 DportStartValue);
int npu_upi_stat_clear(WP_U32 trunkid,WP_U32 groupid);
WP_status upi_stat_get_by_group(WP_U32 trunkid, WP_U32 groupid,STRU_UPI_STATICS *p);
int npu_fr_channel_stat_clear(WP_handle channelHandle ,int RxorTx);
void np_display_defrag_info(WP_handle handle);
void np_display_frag_info(WP_handle group_handle);

void np_update_mac_in_route_system(NP_ARP_INFO *arpInfo );
void np_send2eth_route_flow_add(NP_ARP_INFO * arpInfo);
void np_send2eth_route_system_create(void);
void WPE_PrintIWPortStats(WP_route_port_stats* iwport_stats);
WP_U32 iw_route_port_stat(WP_handle iw_port);
void WPE_PrintIWPortStats(WP_route_port_stats* iwport_stats);
void np_reconfig_vlan_in_routeSystem(void);
void np_dispaly_system_flow_agg(void);
void np_DeviceDelete(WP_U32 trunkid,WP_U32 groupid);
WP_status np_Ufe_Phy_Delete(WP_U32 trunkid,WP_U32 groupid);
WP_status np_Channel_is_None(WP_U32 stm1Id, WP_U32 trunkid, WP_U32 groupid);

void npu_BfdModuleInit(void);
void npu_BfdInit(void);
void WPE_BfdTxSessionBuild(NP_SYSTEM_INFO *the_system);
void WPE_BfdRxSessionBuild(NP_SYSTEM_INFO *the_system);
void np_BFD_flowAdd(NP_SYSTEM_INFO* the_system);
void np_dst_mac_filter_flowAdd(NP_SYSTEM_INFO* the_system);

void np_control_interrupt(u8 flag,u8 Irqnum);
void handle_rx_fr_packet(int *Param);
void handle_rx_eth_packet(int *Param);
void np_create_arp_hq();
void npu_ufe_reconfig_clock_mode( WP_U32 line_index ,WP_U32 trans_type ,WP_U32 clock_mode);

#endif
