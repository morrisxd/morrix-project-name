#ifndef __WDDI_COMMON_H
#define __WDDI_COMMON_H
#include <linx.h>

#ifndef FULSE
#define FULSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

//#define STATUS_OK 0

#define SERVER_NAME 	"wddi_server"
#define CLIENT_NAME  	"wddi_client_"
#define DEBUG_NAME  	"wddi_debug_"
#define NPUMON_NAME  	"npumon_"

#define NPU_RECEIVE_TIMEOUT 1000 /*find server timeout*/
#define NPU_SERVER_TIMEOUT 100
#define NPU_RECEIVE_IOCTL_TIMEOUT 300 /* miliseconds */


#pragma pack (4)

#define NPU_SET_REQUEST_TYPE 0
#define NPU_GET_REQUEST_TYPE 1


#define LINX_SIG_BASE 0x8400

#define NPU_ECHO_SIG (LINX_SIG_BASE + 1)
struct stru_npu_echo_sig {
	LINX_SIGSELECT  sig_no;
};

#define NPU_ACK_SIG (LINX_SIG_BASE + 2)
struct stru_npu_ack_sig {
	LINX_SIGSELECT  sig_no;
	int             result;
};

#define NPU_CFG_SIG (LINX_SIG_BASE + 3)
struct stru_npu_cfg_sig {
	LINX_SIGSELECT  sig_no;
};

#define SPP_SEND_SIG (LINX_SIG_BASE + 4)
struct stru_spp_send_sig {
	LINX_SIGSELECT  sig_no;
	u8 vp_type;
	u32 length;
    u32 ipaddr_src;
	u32 ipaddr_dst;
	u32 port_src;
	u32 port_dst;
	u8 pdata[1];

};

#define SPP_RECEIVE_SIG (LINX_SIG_BASE + 5)
struct stru_spp_receive_sig {
	LINX_SIGSELECT  sig_no;
	u8 vp_type;
	u32 length;
    u32 port;
	u32 ipaddr_dst;
	u32 ipaddr_src;
	u16 port_src;
	u8  pdata[1];
};



#define VPORT_PORT_OPEN_SIG (LINX_SIG_BASE + 8)
struct stru_vport_open_sig {
	LINX_SIGSELECT  sig_no;
	u8  vp_type;
	u32 ipaddr_src;
	u32 port_src;
};

#define VPORT_PORT_CLOSE_SIG (LINX_SIG_BASE + 9)
struct stru_vport_close_sig {
	LINX_SIGSELECT  sig_no;
	u8  vp_type;
	u32 port_src;
};

#define ATTACH_SERVER_SIG  (LINX_SIG_BASE + 10)
#define ATTACH_CLIENT_SIG  (LINX_SIG_BASE + 11)


#define NPU_REQUEST_SIG  (LINX_SIG_BASE + 12)
struct stru_npu_request_sig {
	LINX_SIGSELECT  sig_no;
	u32 cmd;
	u32 cmdType;
    u8  pargv[1];
};
#define NPU_SET_REQUEST_ACK_SIG (LINX_SIG_BASE + 13)
struct stru_npu_set_request_ack_sig {
	LINX_SIGSELECT  sig_no;
	int result;
};


#define NPU_GET_REQUEST_ACK_SIG (LINX_SIG_BASE + 14)
struct stru_npu_get_request_ack_sig {
	LINX_SIGSELECT  sig_no;
	u32 cmd;
	int result;
	u8  pdata[1];
};

#define ATTACH_NPUMON_SIG  (LINX_SIG_BASE + 15)

union LINX_SIGNAL
{
	LINX_SIGSELECT  sig_no;
	struct stru_npu_ack_sig npu_ack;
	struct stru_npu_cfg_sig npu_cfg;
	struct stru_spp_send_sig spp_send;
	struct stru_spp_receive_sig spp_receive;

	struct stru_vport_open_sig vport_open;
	struct stru_vport_close_sig vport_close;
	struct stru_npu_request_sig npu_request;
	struct stru_npu_set_request_ack_sig npu_set_request_ack;
	struct stru_npu_get_request_ack_sig npu_get_request_ack;
};

#pragma pack ()

#if 0
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
#endif

/*
 *  Interface specification between GEP3 and NPU (PDU type)
 */
int linx_init(void);
int np_get_slots_selection(u32 card,u32 trunk,u32 group);
#endif
