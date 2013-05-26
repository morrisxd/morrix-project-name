#ifndef __SPP_NPU_LINK_H
#define __SPP_NPU_LINK_H

#ifndef ENEA_DEBUG
#include "stdincs.h"
#endif

#ifdef ENEA_DEBUG
/*remove in futhure*/
#ifndef TYPE_DEF
#define TYPE_DEF
typedef unsigned char u8;
typedef unsigned short int u16;
typedef unsigned int u32;
typedef unsigned long long u64;
#endif

#endif /*end if ENEA_DEBUG*/


#ifdef  ENEA_DEBUG
#define L(fmt, ...)                             \
    printf( fmt,## __VA_ARGS__)
#endif

#define VP_TYPE_UDP 1
#define VP_TYPE_FR  2
#define VP_TYPE_LINX 3

#define STATUS_OK 0
#define NPU_VERSION_NUM "PMCV1.1.90"

/*******************************************************************************
 * npu_vport_open
 * npu_open_vport will open a vport on NPU
 * input  : u8 vp_type  VP_TYPE_UDP or VP_TYPE_FR
 * input  : u32 ipaddr_src
 * input  : u32 port_src
 * return : return ok or fail ,value <0 fail
 *******************************************************************************/
int npu_vport_open(u8 vp_type,u32 ipaddr_src,u32 port_src);

/*******************************************************************************
 * npu_vport_close
 * npu_open_vport will open a vport on NPU
 * input  : u32 Socketfd
 * input  : u8  vp_type
 * return : return ok or fail ,value <0 fail
 *******************************************************************************/
int npu_vport_close(int Socketfd,u8 vp_type,u32 port_src);


/*******************************************************************************
 * npu_vport_send
 * npu_vport_send will send data to vport via linx link
 * input  : u32 Socketfd
 * input  : u8 vp_type  VP_TYPE_UDP or VP_TYPE_FR
 * input  : u8* p    data buffer point
 * input  : u32 len  data length
 * input  : u32 ipaddr_src
 * input  : u32 port_src
 * input  : u32 ipaddr_dst
 * input  : u32 port_dst
 * return : return ok or fail ,value <0 fail
 *******************************************************************************/
int npu_vport_send(int Socketfd,u8 vp_type,u8* p, u32 len,u32 ipaddr_src,u32 port_src,u32 ipaddr_dst, u32 port_dst);


/*******************************************************************************
 * npu_vport_recv
 * npu_vport_recv will receive data from vport via linx link
 * input  : u32 Socketfd
 * input  : u8 vp_type  VP_TYPE_UDP or VP_TYPE_FR
 * input  : u8* buf    data buffer point  should alloc by user
 * input  : u32 maxlen
 * output : u8 *vp_type
 * output : ipaddr_dst which ipaddr data sendto
 * output : u32* port  which port received data
 * output : u32* ipaddr_src  which ipaddr data come from
 * output : u16* udp_port_src which port data come from
 * return : return ok or fail ,value <0 fail value > 0 ,return the size of the received data .
 *******************************************************************************/
int npu_vport_recv(int Socketfd,u8* buf, u32 maxlen,u8* vp_type, u32* ipaddr_dst,u32* port,u32* ipaddr_src, u16* udp_port_src);

/*******************************************************************************
 * npu_creat_linx
 * npu_creat_linx will creat linx
 * input  : u8 BlockFlag
 * output : int *Socketfd
 * return : return ok or fail ,value <0 fail.
 *******************************************************************************/
u32 npu_creat_linx(int *Socketfd, u8 BlockFlag);

/*******************************************************************************
 * npu_ioctrl
 * npu_ioctrl creat a interface between SPP instance and WDDI client that used to control NPU
 * u32 cmd
 * u32 arg
 * return : return ok or fail ,value <0 fail
 *******************************************************************************/
int npu_ioctrl(u32 cmd ,void *arg);

int npu_ioctrl2(u32 cmd ,void *arg);
#pragma pack (4)

/* Statistics */
typedef struct STRU_ETH_STATICS_TAG
{
    /* The following statistics are maintained in HW registers */
    u64 txrx_frames_64;
    u64 txrx_frames_127;
    u64 txrx_frames_255;
    u64 txrx_frames_511;
    u64 txrx_frames_1023;
    u64 txrx_frames_1518;
    u64 txrx_frames_1522;
    u64 rx_bytes;
    u64 rx_packets;
    u64 rx_err_fcs;
    u64 rx_multicast;
    u64 rx_broadcast;
    u64 rx_mac_control;
    u64 rx_mac_pause;
    u64 rx_mac_unknown;
    u64 rx_err_alignment;
    u64 rx_err_length;
    u64 rx_err_code;
    u64 rx_false_carrier;
    u64 rx_undersize;
    u64 rx_oversize;
    u64 rx_fragments;
    u64 rx_jabber;
    u64 rx_dropped;
    u64 tx_bytes;
    u64 tx_packets;
    u64 tx_multicast;
    u64 tx_broadcast;
    u64 tx_mac_pause;
    u64 tx_defer;
    u64 tx_excess_defer;
    u64 tx_single_collision;
    u64 tx_multi_collision;
    u64 tx_late_collision;
    u64 tx_excess_collision;
    u64 tx_no_collision;
    u64 tx_mac_pause_honored;
    u64 tx_dropped;
    u64 tx_jabber;
    u64 tx_err_fcs;
    u64 tx_control;
    u64 tx_oversize;
    u64 tx_undersize;
    u64 tx_fragments;

    /* These statistics are maintained in DPS */
    u64 rx_host_frames;
    u64 rx_iw_frames;
    u64 rx_err_host_full;
    u64 rx_err_fbp_underrun;
    u64 rx_err_nonvalid_mac;
    u64 rx_err_mru;
    u64 rx_err_sdu;
    u64 tx_err_underrun;
    u64 rx_err_overrun;
    u64 tx_frames;

    /* statistics raised between serial interface and DPS */
    u64 dps_tx_frames;
    u64 dps_tx_bytes;
    u64 dps_rx_frames;
    u64 dps_rx_bytes;
    u64 dps_rx_dropped_frames;
    u64 dps_rx_pce_denied_frames;
} STRU_ETH_STATICS;


typedef struct STRU_BPORT_STATICS_TAG
{
    u64 rx_valid_packets;
    u64 discard_vlan_acceptable_filter;
    u64 discard_ingress_filter;
    u64 discard_bridge_classifier; /* for backward compatibility */
    u64 discard_unk_macsa;
    u64 deny_mac_sa;
    u64 deny_mac_da;
    u64 rx_bc_valid_packets;
    u64 rx_mc_valid_packets;
    u64 forwarded_uc_packets;
    u64 forwarded_bc_packets;
    u64 forwarded_mc_packets;
    u64 protocol_error;
    u64 res_mc_mac_deny;
    u64 discard_dfc_filter[32];
    u32 deny_cfm_frames;
    u64 bad_ip_version;
    u64 checksum_error;
    u64 ip_protocol_error;
    u64 discard_pce_filter[256];
    u64 ingress_policer_non_conforming_packets;
}
    STRU_BPORT_STATICS;

typedef struct STRU_UPI_STATICS_TAG
{
	u64	  rx_frames;
	u64	  rx_bytes;
	u64	  rx_frames_de_set;
	u64	  rx_bytes_de_set;
	u64	  rx_frames_fecn_set;
	u64	  rx_frames_becn_set;
	u64	  rx_discarded_err_crc;
	u64	  rx_discarded_err_alignment;
	u64	  rx_discarded_length_violation;
	u64	  rx_discarded_illegal_header;
	u64	  rx_discarded_abort;
	u64	  rx_discarded_unsupported_dlci;
	u32	  rx_last_unsupported_dlci;
	u64	  rx_discarded_fbp_empty;
	u64	  rx_discarded_hostq_full;
	u64	  rx_discarded_err_parity;
	u64	  rx_discarded_err_sop_eop;
	u64   rx_total_err_bytes;
	u64	  tx_frames;
	u64	  tx_bytes;
	u64	  tx_frames_de_set;
	u64	  tx_bytes_de_set;
	u64	  tx_frames_fecn_set;
	u64	  tx_frames_becn_set;
	u64	  tx_underrun;
	u64	  tx_aborted_frames;
	u64 serial_dps_if_tx_frames;
	u64 serial_dps_if_tx_bytes;
	u64 serial_dps_if_rx_frames;
	u64 serial_dps_if_rx_bytes;
	u64 serial_dps_if_rx_dropped_frames;
	u64 serial_dps_if_rx_pce_denied_frames;
}
    STRU_UPI_STATICS;


typedef struct STRU_RXFR_STATICS_TAG
{
    u64 rx_frames;
    u64 rx_bytes;
    u64 rx_frames_de_set;
    u64 rx_bytes_de_set;
    u64 rx_frames_fecn_set;
    u64 rx_frames_becn_set;
    u64 rx_discarded_err_crc;
    u64 rx_discarded_err_alignment;
    u64 rx_discarded_length_violation;
    u64 rx_discarded_abort;
    u64 rx_discarded_fbp_empty;
    u64 rx_discarded_hostq_full;
}
    STRU_RXFR_STATICS;

typedef struct STRU_TXFR_STATICS_TAG
{
    u64 tx_frames;
    u64 tx_bytes;
    u64 tx_frames_de_set;
    u64 tx_bytes_de_set;
    u64 tx_frames_fecn_set;
    u64 tx_frames_becn_set;
    u64 tx_aborted_frames;
}
    STRU_TXFR_STATICS;


typedef struct STRU_CPU_STATICS_TAG
{
    u32 totalUtili;
	u32 WinGinUtili[12];/*winpath3 has 12 wingin max*/
}STRU_CPU_STATICS;

typedef struct STRU_MEMORY_STATICS_TAG
{
    u32 PacketMemLeftByte;
	u32 ParamMemLeftByte;
	u32 InterMemLeftByte;

    u32 PacketMemTotalByte;
	u32 ParamMemTotalByte;
	u32 InterMemTotalByte;
}STRU_MEMORY_STATICS;


typedef struct STRU_NS_BSSGP_STATICS_TAG
{
	u64  bssgp_in_octets; /*in direction, equal to NS unit data counter*/
	u64  bssgp_in_packets;
	u64  bssgp_in_packets_signalling;/*not BSSGP unit packet*/
	u64  bssgp_out_octets;/*out direction,equal to NS unit data counter*/
	u64  bssgp_out_packets;
	u64  bssgp_out_packets_signalling;
	u64  ns_in_octets; /*in direction,equal to NS unit + NS signal */
	u64  ns_in_packets;
	u64  ns_in_packets_signalling;
	u64  ns_out_octets;
	u64  ns_out_packets;
	u64  ns_out_packets_signalling;
}STRU_NS_BSSGP_STATICS;

typedef enum
{
    NPU_VLAN_ENABLE = 0,
    NPU_VLAN_DISABLE
}Vlan_Flage;

typedef enum
{
    NPU_MODE_FR = 0,
    NPU_MODE_IP
}Operate_mode;

typedef enum
{
    NPU_INVALID = 0,
    NPU_VALID
}Valid_flag;

typedef enum
{
    NPU_MODE_E1 = 0,
    NPU_MODE_T1,
    NPU_E1T1_CLOCK_MASTER,
    NPU_E1T1_CLOCK_SLAVE,
    NPU_E1T1_CRC16,
    NPU_E1T1_CRC32
}E1T1type;

typedef enum
{
    NPU_NPUMON_CAPTURE_ENABLE = 1,
    NPU_NPUMON_CAPTURE_DISABLE = 0
}npumon_capture;

typedef enum
{
    SET_RULE_TABLE = 0,
	MODIFY_RULE_TABLE,
    DEL_UL_RULE_TALBE_BASED_ON_PORT,
    DEL_UL_RULE_TALBE_BASED_ON_PORT_NRI,
    DEL_DL_RULE_TALBE_BASED_ON_DPORT,
    DEL_DL_RULE_TALBE_BASED_ON_BVCI,
    DEL_DL_RULE_TALBE_BASED_ON_DPORT_BVCI,
    DEL_UL_UPDATE_BVCI_TALBE_BASED_ON_PORT_BVCI,
    DEL_UL_UPDATE_BVCI_TALBE_BASED_ON_PORT
}Table_Optmode;

typedef enum
{
	LOCAL_FLAG =0,
    PEER_FLAG = 16

}local_remote_flag;

/*ADD for LMI interface*/
typedef enum
{
    SET_PVC_ENABLE =0,
    SET_PVC_DISABLE
}Vport_status;

typedef enum
{
	LINK_STATUS_DOWN = 0,
	LINK_STATUS_UP,
	LINK_STATUS_DISABLED/*link is not configured*/
}link_status;


typedef enum
{
	NPU_LOG_DEBUG = 0,
 	NPU_LOG_INFO,
	NPU_LOG_NOTICE,
	NPU_LOG_WARNING,
	NPU_LOG_ERR,
	NPU_LOG_CRIT,
	NPU_LOG_ALERT,
	NPU_LOG_EMERG
}log_debug_level;

typedef enum
{
    NPU_BLOCK = 0,
    NPU_UNBLOCK
}bolck_mode_flag;

typedef enum
{
    NPU_POOL = 0,
    NPU_NONPOOL
}NP_Pool_Flag;


typedef enum
{
    NPU_NSE_AGG_OFF = 0,
    NPU_NSE_AGG_ON
}NP_NSEAGG_FLAG;


#define NPU_GET_CMD_BASE           0x00
#define NPU_SET_CMD_BASE          0x64

#define SPP_MAX_STM1_PORT_NUM 2
#define SPP_MAX_TRUNK_NUM 84
#define SPP_MAX_GROUP_NUM 32

#define SPP_MAX_ROUTE_NUM 64

#define NPU_SET_INITIAL_CONFIG_ERR                   -1
#define NPU_SET_FR_UPLINK_PATH_RULE_ERR              -2
#define NPU_MODIFY_FR_UPLINK_PATH_RULE_ERROR     -3
#define NPU_DELETE_FR_UPLINK_PATH_RULE_ERROR     -4
#define NPU_SET_FR_DOWNLINK_PATH_RULE_ERROR      -5
#define NPU_MODIFY_FR_DOWNLINK_PATH_RULE_ERROR   -6
#define NPU_DELETE_FR_DOWNLINK_PATH_RULE_ERROR   -7
#define NPU_GET_ETH_STATICS_ERROR                -8
#define NPU_GET_UPI_STATICS_ERROR                -9
#define NPU_GET_RXFR_STATICS_ERROR               -10
#define NPU_GET_TXFR_STATICS_ERROR               -11
#define NPU_SET_NPUMON_CMD_PORT_ERROR            -12
#define NPU_SET_NPUMON_CMD_ALREAD_START_ERROR     -13

#define NPU_SET_LMI_PVC_STATUS_FAIL               -14

/*error message type define*/
#define ERR_LINX_OPEN         -1
#define ERR_LINX_CLOSE        -2
#define ERR_LINX_HUNT         -3
#define ERR_LINX_RCV_TMOUT    -4
#define ERR_LINX_HUNT_TMOUT   -5
#define ERR_LINX_GET_SENDER   -6
#define ERR_LINX_FREE_SIG     -7
#define ERR_LINX_FIND_SERVER  -8
#define ERR_LINX_ALLOC        -9
#define ERR_LINX_SEND         -10
#define ERR_GET_LINX_INFO     -11
#define ERR_GET_VPORT_INFO    -12

#define ERR_LINX_RSV_FAIL     -13
#define ERR_LINX_RSV_TMOUT    -14

#define ERR_LINX_RSV_OVER_SIZE -15
#define ERR_UNKNOWN_ARG        -16
#define ERR_LINX_DISCONNECTED  -17


/*for npumon*/
typedef enum
{
    NPU_MON_CMD_XM=0,
    NPU_MON_CMD_XL,
	NPU_MON_CMD_XRU,
	NPU_MON_CMD_TI,
	NPU_MON_CMD_TI4,
	NPU_MON_CMD_TR,
	NPU_MON_CMD_DD,
	NPU_MON_CMD_DC,
	NPU_MON_CMD_CD,
	NPU_MON_CMD_CC
}NPU_MON_CMD;


#define NPUMON_NO_ARGS  0xffff

/*NP io_ctrl set type command defination */
#define NPU_SET_RECONFIG          (NPU_SET_CMD_BASE + 0)

typedef struct STRU_IP_EP_TAG
{
	u32 IpDst;
	u16 Flag;         /*LOCAL_FLAG or PEER_FLAG*/
	u16 PortDst;
}STRU_IP_EP;
typedef struct STRU_NSVC_TABLE_TAG
{
	u16 Flag;              /*LOCAL_FLAG or PEER_FLAG*/
    u32 vfort;

}STRU_NSVC_TABLE;


/**************************************************************************
                    uplink route table
***************************************************************************/
#define NPU_SET_UPLINK_PATH_RULE    (NPU_SET_CMD_BASE + 1)
typedef struct STRU_SET_UPLINKPATH_RULE_TAG
{
    u32 OptMode;      /*SET_RULE_TABLE
                        MODIFY_RULE_TABLE */
	union
	{
	    u32 vfport;       /*for FR interface*/
		u32 portDst;      /*for IP interface,Max is 3200 ,value is 0~64k*/
	}u_port;
    u32 NRIValue;     /*1~255*/
	u32 PortSrc;      /*vfport+NRIValue => PortSrc*/
}STRU_SET_UPLINK_PATH_RULE;

#define NPU_DELETE_UPLINK_PATH_RULE    (NPU_SET_CMD_BASE + 2)
typedef struct STRU_DELETE_UPLINKPATH_RULE_TAG
{
    u32 DeleteMode;  /*DEL_UL_RULE_TALBE_BASED_ON_PORT
	                   DEL_UL_RULE_TALBE_BASED_ON_PORT_NRI */
	union
	{
        u32 vfport;		 /*for FR interface*/
        u32 portDst; 	 /*for IP interface*/
	}u_port;

    u32 NRIValue;/*vfport&NRIValue can identify a uplink flow */
}STRU_DELETE_UPLINK_PATH_RULE;

/**************************************************************************
                    uplink link table
***************************************************************************/

#define NPU_SET_UPLINK_PATH_LINK_TABLE    (NPU_SET_CMD_BASE + 3)
#define NPU_SET_DOWNLINK_PATH_LINK_TABLE NPU_SET_UPLINK_PATH_LINK_TABLE /*For IP<->IP*/
typedef struct STRU_SET_UPLINKPATH_LINK_TABLE_TAG
{
    u32 PortSrc;        /*PortSrc => IpEp[64]*/
	u32 IpEpAcitveNum;  /*the number of active link,IpEpAcitveNum = 0 then all packet will be droped*/
	STRU_IP_EP IpEp[64];/*max ip_ep is 64 for load banlence*/
}STRU_SET_UPLINK_PATH_LINK_TABLE;
typedef STRU_SET_UPLINK_PATH_LINK_TABLE STRU_SET_DOWNLINK_PATH_LINK_TABLE;/*For IP<->IP*/
/**************************************************************************
                    uplink update BVCI route table
***************************************************************************/
#define NPU_SET_UPDATE_BVCI_PATH_RULE    (NPU_SET_CMD_BASE + 4)
typedef struct STRU_SET_UPDATE_BVCI_PATH_RULE_TAG
{
    u32  OptMode;       /*SET_RULE_TABLE*/
	union
	{
		u32 vfport;       /*for FR interface vfport+NS_BVCI_old=>NS_BVCI*/
		u32 portDst;      /*for IP interface portDst+NS_BVCI_old=>NS_BVCI*/
	}u_port;
	u16  NS_BVCI_old;   /*old BVCI*/
	u16  NS_BVCI;       /*new BVCI*/
}STRU_SET_UPDATE_BVCI_PATH_RULE;

#define NPU_DELETE_UPDATE_BVCI_PATH_RULE    (NPU_SET_CMD_BASE + 5)
typedef struct STRU_DELETE_UPDATE_BVCI_PATH_RULE_TAG
{
    u32  OptMode;        /*DEL_UL_UPDATE_BVCI_TALBE_BASED_ON_PORT_BVCI
	                       DEL_UL_UPDATE_BVCI_TALBE_BASED_ON_PORT*/
	union
	{
        u32 vfport;		 /*for FR interface vfport+NS_BVCI_old=>NS_BVCI*/
        u32 portDst; 	 /*for IP interface portDst+NS_BVCI_old=>NS_BVCI*/
	}u_port;
	u16  NS_BVCI_old;    /*vfport&NS_BVCI_old can identify a flow */
}STRU_DELETE_UPDATE_BVCI_PATH_RULE;

/**************************************************************************
                    downlink route table
***************************************************************************/
#define NPU_SET_DOWNLINK_PATH_RULE    (NPU_SET_CMD_BASE + 6)
typedef struct STRU_SET_DOWNLINK_PATH_RULE_TAG
{
	u32 OptMode;          /*SET_RULE_TABLE
                            MODIFY_RULE_TABLE*/
	u16 DportStartValue;  /*begin value of the port group,port group size = PortPoolSize*/
	u16 BVCI;             /*bvci used in downlink classify*/
	u16 BVCInew;
	union
	{
        u16 NSEI;			  /* for FR ,bvci+ DportGrp[n..m] ==> NSEI+ BVCInew */
        u16 portSrc;          /*for IP */
	}nsei_portSrc;
    u16 PortSize;
}STRU_SET_DOWNLINK_PATH_RULE;

#define NPU_DELETE_DOWNLINK_PATH_RULE    (NPU_SET_CMD_BASE + 7)
typedef struct STRU_DELETE_DOWNLINK_PATH_RULE_TAG
{
    u32 OptMode;          /*DEL_DL_RULE_TALBE_BASED_ON_DPORT
	                        DEL_DL_RULE_TALBE_BASED_ON_BVCI
	                        DEL_DL_RULE_TALBE_BASED_ON_DPORT_BVCI */
    u16 DportStartValue;  /*used when delete,delete DportStartValue~PortEndValue*/
	u16 BVCI;
}STRU_DELETE_DOWNLINK_PATH_RULE;

/**************************************************************************
                    downlink link table
***************************************************************************/
#define NPU_SET_FR_DOWNLINK_PATH_LINK_TABLE    (NPU_SET_CMD_BASE + 8)
typedef struct STRU_SET_FR_DOWNLINKPATH_LINK_TABLE_TAG
{
    u16 NSEI;              /*a part of NSEI_BVCInew*/

	u32 FrVportAcitveNum;  /*number of active nsvc(vfort)FrVportAcitveNum=0 packet will be droped*/
	STRU_NSVC_TABLE NsvcGrp[16];  /*NSEI => NsvcGrp max nsvc is 16 for fr load banlence*/
}STRU_SET_FR_DOWNLINK_PATH_LINK_TABLE;


/*add for npumon*/
#define NPU_MON_SET_CAPTURE_START   (NPU_SET_CMD_BASE + 9)
/*use STRU_NPUMON_INFO struct*/
#define NPU_MON_SET_CAPTURE_STOP   (NPU_SET_CMD_BASE + 10)


/*add for LMI interface*/
#define NPU_SET_LMI_PVC_STATUS   (NPU_SET_CMD_BASE + 11)
typedef struct STRU_NPU_SET_LMI_PVC_STATUS_TAG
{
    u32 vfport;
	u32 status; /*SET_PVC_ENABLE or SET_PVC_DISABLE*/
}STRU_NPU_SET_LMI_PVC_STATUS;

/*add for LOG_DEBUG level*/
#define NPU_SET_LOG_DEBUG_LEVEL   (NPU_SET_CMD_BASE + 12)
typedef struct STRU_NPU_SET_LOG_DEBUG_LEVEL_TAG
{
	u32 level;   /*
                   NPU_LOG_INFO = 0,
                   NPU_LOG_NOTICE,
                   NPU_LOG_WARNING,
                   NPU_LOG_ERR,
                   NPU_LOG_CRIT,
                   NPU_LOG_ALERT,
                   NPU_LOG_EMERG


	             */
}STRU_NPU_SET_LOG_DEBUG_LEVEL;

/*add for clear statistics*/
#define NPU_SET_NS_BSSGP_STATICS_ZERO      (NPU_SET_CMD_BASE + 13)
typedef struct STRU_SET_NS_BSSGP_STATICS_ZERO_TAG
{
    u32 vfport;
}STRU_SET_NS_BSSGP_STATICS_ZERO;

#define NPU_SET_NONPOOL_STATE      (NPU_SET_CMD_BASE + 14)
typedef struct STRU_SET_NPU_NONPOOL_STATE_TAG
{
    u32 nonpoolflag;
}STRU_SET_NPU_NONPOOL_STATE;

#define NPU_SET_NSEAGG_FLAG      (NPU_SET_CMD_BASE + 15)
typedef struct STRU_SET_NPU_NSEAGG_FLAG_TAG
{
    u32 nseAggFlag;
}STRU_SET_NPU_NSEAGG_FLAG;

#define PCAPFILENAMELEN 256
/*NPU MON*/
typedef struct STRU_NPUMON_INFO_TAG
{
    unsigned int cmd;    /**/
	int ifType;/*VP_TYPE_UDP OR VP_TYPE_FR*/
	unsigned int TrunkNumber;
	unsigned int portSrc;
	unsigned int dlci;
	unsigned int group;
	unsigned int stm1;
	unsigned int ipaddr;
	unsigned int len;
	char PcapFileName[PCAPFILENAMELEN];
}STRU_NPUMON_INFO;

/*NP io_ctrl get type command defination */
#define NPU_GET_ETH_STATICS       (NPU_GET_CMD_BASE + 0)
typedef struct STRU_GET_ETH_STATICS_TAG
{
	u32 EthNumber;/* 1 :traffic, 2 NPU<->NPU*/
	u32 reserved;
	STRU_ETH_STATICS stru_eth_statics;

}STRU_GET_ETH_STATICS;

#define NPU_GET_UPI_GROUP_STATICS       (NPU_GET_CMD_BASE + 1)
/*get group device statics*/
typedef struct STRU_GET_UPI_GROUP_STATICS_TAG
{
    u32 stm1Id;  /* 1 or 2*/
	u32 trunkid; /*E1 1~63, T1 1~84*/
	u32 groupid; /*0~31*/
	STRU_UPI_STATICS stru_upi_statics;
}STRU_GET_UPI_GROUP_STATICS;

#define NPU_GET_UPI_TRUNK_STATICS       (NPU_GET_CMD_BASE + 2)
typedef struct STRU_GET_UPI_TRUNK_STATICS_TAG
{
    u32 stm1Id;  /* 1 or 2*/
	u32 trunkid; /*E1 1~63, T1 1~84*/
	STRU_UPI_STATICS stru_upi_statics;

}STRU_GET_UPI_TRUNK_STATICS;

#define NPU_GET_RXFR_STATICS      (NPU_GET_CMD_BASE + 3)
typedef struct STRU_GET_RXFR_STATICS_TAG
{
	u32 vfport;
	STRU_RXFR_STATICS stru_rxfr_statics;

}STRU_GET_RXFR_STATICS;

#define NPU_GET_TXFR_STATICS      (NPU_GET_CMD_BASE + 4)
typedef struct STRU_GET_TXFR_STATICS_TAG
{
	u32 vfport;
	STRU_TXFR_STATICS stru_txfr_statics;

}STRU_GET_TXFR_STATICS;

#define NPU_GET_CPU_STATICS      (NPU_GET_CMD_BASE + 5)
typedef struct STRU_GET_CPU_STATICS_TAG
{
	STRU_CPU_STATICS stru_cpu_statics;
}STRU_GET_CPU_STATICS;

#define NPU_GET_MEMORY_STATICS      (NPU_GET_CMD_BASE + 6)
typedef struct STRU_GET_MEMORY_STATICS_TAG
{
	STRU_MEMORY_STATICS stru_mem_statics;
}STRU_GET_MEMORY_STATICS;

#define NPU_GET_NS_BSSGP_STATICS_STATICS      (NPU_GET_CMD_BASE + 7)
typedef struct STRU_GET_NS_BSSGP_STATICS_TAG
{
    u32 vfport;
    STRU_NS_BSSGP_STATICS ns_bssgp_statics;
}STRU_GET_NS_BSSGP_STATICS;

/*use no struct FOR npumon*/
#define NPU_MON_GET_CAPTURE_FILE   (NPU_GET_CMD_BASE + 8)

#define NPU_GET_UPI_STM1_STATICS       (NPU_GET_CMD_BASE + 9)
typedef struct STRU_GET_UPI_STM1_STATICS_TAG
{
	u32 stm1id;                      /*1 or 2*/
	u32 reserved;
	STRU_UPI_STATICS stru_upi_statics;

}STRU_GET_UPI_STM1_STATICS;


/*add for STM1 E1/T1 satus check*/
typedef struct STRU_STM1_E1T1_STATUS_TAG
{
    u8 stm1_status;                   /*LINK_STATUS_UP or LINK_STATUS_DOWN or LINK_STATUS_DISABLED*/
	u8 e1t1_status[SPP_MAX_TRUNK_NUM];/*LINK_STATUS_UP or LINK_STATUS_DOWN or LINK_STATUS_DISABLED*/
}STRU_STM1_E1T1_STATUS;

#define NPU_GET_STM1_E1T1_STATUS      (NPU_GET_CMD_BASE + 10)

typedef struct STRU_NPU_GET_STM1_E1T1_STATUS_TAG
{
    u32  stm1id;                     /*which stm1 you want to check status ,valuse 1 or 2*/
	STRU_STM1_E1T1_STATUS status;

}STRU_NPU_GET_STM1_E1T1_STATUS;


/*NP debug get type command defination ,can't support by io_ctrl */
#define NPU_GET_UFE_SYS                  (NPU_GET_CMD_BASE + 20)
#define NPU_GET_UFE_LINE                 (NPU_GET_CMD_BASE + 21)
#define NPU_GET_UFE_PHY                  (NPU_GET_CMD_BASE + 22)
#define NPU_GET_FLOW_STATICS             (NPU_GET_CMD_BASE + 23)
#define NPU_DISPLAY_CONFIG_INFO          (NPU_GET_CMD_BASE + 24)
#define NPU_GET_TRUNK_INFO               (NPU_GET_CMD_BASE + 25)
#define NPU_GET_DPORT_STATISTIC_INFO     (NPU_GET_CMD_BASE + 26)
#define NPU_GET_TABLE_ITEM               (NPU_GET_CMD_BASE + 27)

#define NPU_DEBUG_CHANNEL      (NPU_GET_CMD_BASE + 37)
typedef struct STRU_DEBUG_CHANNEL_TAG
{
	u32 vfport;
	u32 DebugType;
}STRU_DEBUG_CHANNEL;

typedef struct STRU_GET_UFE_LINE_TAG
{
    u32 Lineindex;
}STRU_GET_UFE_LINE;

typedef struct STRU_GET_UFE_TAG
{
    u32 PortNumber;
}STRU_GET_UFE;

typedef struct STRU_GET_TRUNKINFO_TAG
{
    u32 trunkID;
}STRU_GET_TRUNKINFO;

typedef struct STRU_GET_DPORT_TAG
{
    u16 dportID;
}STRU_GET_DPORT_STATISTICS;

#define SPPMAXPARA 10

typedef struct STRU_GET_TBLITEM_TAG
{
    u16 tableLoc[SPPMAXPARA];
}STRU_GET_TABLE_ITEM;
typedef enum
{
    UL_ROUTE = 0,
    UL_LINK,
    DL_ROUTE,
    DL_LINK,
    BVCI
}GET_TBL_ITEM;

#pragma pack ()

#endif
