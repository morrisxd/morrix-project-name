#define TRUE     1
#define FALSE    0

#define WPE_TEST_CHECK_RESULT(result, exp_result, x) \
        do{ \
            result = (x); \
            if(result != exp_result) { \
            printf("TEST FAILURE in file %s,  function %s,  line: %d \n" \
                   "   Returned %s. Expected Value %s\n",  \
                   __FILE__, __FUNCTION__, __LINE__, \
                   WP_error_name[result & 0xFFFF], WP_error_name[exp_result & 0xFFFF]);\
            wpe_test_failures++; \
          } \
        } while(0)

#define WPE_TEST_CHECK_HANDLE(handle, x) \
        do{ \
            handle = (x); \
            if(WPI_handle_error == WPI_HANDLE_TYPE(handle)) {\
              printf("TEST FAILURE in file %s,  function %s,  line: %d \n" \
                     "   Expected Handle. Returned error %s\n",  \
                     __FILE__, __FUNCTION__, __LINE__, \
                     WP_error_name[handle & 0xFFFF]);\
              wpe_test_failures++;\
            }\
        } while(0)

#define WPE_TEST_ADD_ERROR(error_str) \
        do{ \
            wpe_test_failures++; \
            printf("TEST FAILURE in file %s,  function %s,  line: %d \n    %s\n", \
                  __FILE__, __FUNCTION__, __LINE__, error_str); \
        } while(0)

#define WPE_STATS_CHECK(strct, field, val)\
        do {\
           WP_U32 strct_u32;\
           strct_u32 = strct->field;\
           if(strct_u32 != val)\
           {\
              WP_CHAR log_text[300];\
              sprintf(log_text, "WPE_STATS_CHECK ERROR: value is %d should be %d\n",\
                 strct_u32, val);\
              WPE_TEST_ADD_ERROR(log_text);\
           }\
        } while(0)

#define WPE_STATS_DIFF_CHECK(before, after, field, diff)\
        do {\
           WP_U32 after_u32, before_u32;\
           after_u32 = after->field;\
           before_u32 = before->field;\
           if(after_u32 - before_u32 != diff)\
           {\
              WP_CHAR log_text[300];\
              sprintf(log_text, "WPE_STATS_DIFF_CHECK ERROR: value is %d should be %d\n",\
                 (after_u32 - before_u32), diff);\
              WPE_TEST_ADD_ERROR(log_text);\
           }\
        } while(0)

typedef struct
{
   WP_CHAR mac_da[13];
   WP_CHAR mac_sa[13];
   WP_CHAR vlan_stag[5];
   WP_CHAR vlan_ctag[5];
   WP_CHAR ipv4_da[9];
   WP_CHAR ipv4_sa[9];
} WPE_pce_packet;

WP_U16 WTE_IpChecksumCalc(WP_U8 *buff, WP_U16 len);
void terminate_on_error(WP_handle handle, WP_CHAR *s);
void WPE_TerminateOnError(WP_handle handle, WP_CHAR *s, WP_U32 line);
void WPE_TerminateOnErrorNoPrintIfOK(WP_handle handle, WP_CHAR *s, WP_U32 line);
WP_U32 WPT_DisplayBPortStats(WP_handle port_handle, WP_CHAR* type);
WP_U32 WPT_DisplayRoutePortStats(WP_handle port_handle, WP_CHAR* type);
WP_U32 WPT_GetFlowForwardedPackets(WP_handle flow_aggregation);
WP_U32 WPT_DisplayFlowStats(WP_handle agg, WP_CHAR *type);
WP_U32 WPT_DisplayGenericFlowStats(WP_handle agg, WP_CHAR *type);
WP_U32 WPT_DisplayDeviceStats(WP_handle device_handle, WP_device device, WP_CHAR* type);
void WTE_DisplayXgiDeviceStats(WP_handle device_handle, WP_CHAR* type);
void WTE_DisplayEnetDeviceStats(WP_handle device_handle, WP_CHAR* type);

WP_U8 WPE_DataUnitReceive(WP_handle rx_channel_handle, WP_U8 *hexa_ret);

void WPE_CommonDataUnitSendOnly(const WP_CHAR* mac_dst, const WP_CHAR* mac_src, const WP_CHAR* svlan, const WP_CHAR* cvlan, 
                                         const WP_CHAR* ip_dst, const WP_CHAR* ip_src, WP_handle tx_channel_handle, WP_U8 packet_type);

WP_U8 WPE_DataUnitReceiveOnly(const WP_CHAR* mac_dst, const WP_CHAR* mac_src, const WP_CHAR* svlan, const WP_CHAR* cvlan, 
                                     const WP_CHAR* ip_dst, const WP_CHAR* ip_src, WP_handle rx_channel_handle);
WP_U8 WPE_DataUnitSendReceive(const WP_CHAR* mac_dst, const WP_CHAR* mac_src, const WP_CHAR* svlan, const WP_CHAR* cvlan,
                                     const WP_CHAR* ip_dst, const WP_CHAR* ip_src,
                                     WP_handle tx_channel_handle, WP_handle rx_channel_handle);
void WPE_DataUnitSendOnly(const WP_CHAR* mac_dst, const WP_CHAR* mac_src, const WP_CHAR* svlan, const WP_CHAR* cvlan, 
                                 const WP_CHAR* ip_dst, const WP_CHAR* ip_src, WP_handle tx_channel_handle);
void WPE_CommonPacketSendOnly(WPE_pce_packet *packet, WP_handle tx_channel_handle, WP_U8 packet_type);
void WPE_PacketSendOnly(WPE_pce_packet *packet, WP_handle tx_channel_handle);
void WPE_DelayAfterPacketSend(void);
WP_U8 WPE_PacketReceiveOnly(WPE_pce_packet *packet, WP_handle rx_channel_handle);
WP_U8 WPE_PacketSendReceive(WPE_pce_packet *packet, WP_handle tx_channel_handle, WP_handle rx_channel_handle);
void WPE_PrintPceGlobalStats(void);
void WPE_PrintPceFilterStats(WP_handle filter);
void WPE_ResetPceFilterStats(WP_handle filter);
void WPE_ComputeClocksPerMsec(void);
void WPE_SetHexChar(WP_CHAR *char_ptr, WP_U8 var);

typedef enum
{
   WT_IF_TYPE_SGMII,
   WT_IF_TYPE_XGI,
   WT_IF_TYPE_UPI_HSPOS,
   WT_IF_TYPE_HOST,
   WT_IF_TYPE_MAX,
} WT_PhysicalPortType;

#define WT_STATUS_UNINITIALIZED          0
#define WT_STATUS_INITIALIZED            1

#define WT_MAX_INTERFACES                15

#define WT_MAX_IF_ENET_TX_CHANNELS       8
#define WT_MAX_IF_L1_GROUPS              1
#define WT_MAX_IF_L2_BLOCKS              1
#define WT_MAX_IF_L2_GROUPS              1
#define WT_MAX_IF_FLOW_AGGREGATIONS      32768
#define WT_MAX_ENETS                     10
#define WT_MAX_XGI_PORTS                 2
#define WT_MAX_UPI_PORTS                 2

#define WT_TX_CHANNEL_TAG                3000
#define WT_RX_CHANNEL_TAG                3100

#define WT_IW_PORT_TAG                   4000

typedef struct
{
   WP_U32 status;
   WT_PhysicalPortType type;
   WP_handle port;
   WP_handle device;
   WP_handle rx_channel;
   WP_handle tx_channel;
   WP_U8 tx_channels_number;
   WP_handle l1_groups[10];
   WP_handle l2_blocks[10];
   WP_handle l2_groups[10];
   WP_handle tx_channels[WT_MAX_IF_ENET_TX_CHANNELS];
   WP_handle flow_aggregation;
   WP_handle flow_aggregation2;
   WP_handle flow_aggregations[WT_MAX_IF_FLOW_AGGREGATIONS];
   WP_U16 num_of_flow_aggregations;
   WP_handle iw_port;
   WP_U8 default_flow_agg_number;
   WP_U16 flow_agg_vlan_id;
   WP_handle pce_interface_handle;
   WP_handle qnode_rx;
} WT_Interface;

typedef struct
{
   WP_iw_system_type iw_system_type;
   WP_handle iw_system;
   WT_Interface *interfaces;
} WT_TestGlobalInfo;

typedef enum
{
   WT_PACKET_TYPE_ETHERNET_IP_UDP,
   WT_PACKET_TYPE_ETHERNET_IPV6_UDP,
   WT_PACKET_TYPE_ETHERNET_SC_IP_UDP,
   WT_PACKET_TYPE_ETHERNET_SS_IP_UDP,
   WT_PACKET_TYPE_ETHERNET_IP_TCP,
   WT_PACKET_TYPE_ETHERNET_IP_TCP_ACK,
   WT_PACKET_TYPE_ETHERNET_IP_TCP_SYN,
   WT_PACKET_TYPE_ETHERNET_IP_TCP_ACK_SYN,
   WT_PACKET_TYPE_ETHERNET_IP_MPLS,
   WT_PACKET_TYPE_ETHERNET_IP_GRE,
   WT_PACKET_TYPE_ETHERNET_IP_GRE_MPLS,
   WT_PACKET_TYPE_ETHERNET_ARP,
   WT_PACKET_TYPE_ETHERNET_RARP,
   WT_PACKET_TYPE_ETHERNET_PPPoE_D_IP_UDP,
   WT_PACKET_TYPE_ETHERNET_PPPoE_S_IP_UDP,
   WT_PACKET_TYPE_UPI_HSPOS1_IP_UDP,
   WT_PACKET_TYPE_UPI_HSPOS2_IP_UDP,
   WT_PACKET_TYPE_UPI_HSPOS3_IP_UDP,
   WT_PACKET_TYPE_UPI_HSPOS4_IP_UDP,
}
WT_packet_type;

WP_U8 WT_Init(void);
void WT_CreateFmuFbpPools(void);
WP_U8 WT_CreateInterface(WP_U8 if_number, WT_PhysicalPortType type,
                                WP_U8 physical_port_number, WP_U8 loopback_mode,
                                WP_U8 tx_channels_number,  WP_U16 num_of_flow_aggregations,
                                WP_U8 default_flow_agg, WP_U16 flow_agg_vlan_tag);
void WT_CreateIwSystemAndItsComponents(WP_iw_system_type type);
void WT_CreateIwRxBindingAndTxHostChannel(WP_iw_system_type type);
WP_status WT_PceEccDestroy(WP_handle rule_handle, WP_U32 destroy_type);

