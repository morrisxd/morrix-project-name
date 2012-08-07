#define TRUE     1
#define FALSE    0

#define WPI_TEST_CHECK_RESULT(result, exp_result, x) \
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

#define WPI_TEST_CHECK_HANDLE(handle, x) \
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

#define WPI_TEST_ADD_ERROR(error_str) \
        do{ \
            wpe_test_failures++; \
            printf("TEST FAILURE in file %s,  function %s,  line: %d \n    %s\n", \
                  __FILE__, __FUNCTION__, __LINE__, error_str); \
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

static WP_U16 WTE_IpChecksumCalc(WP_U8 *buff, WP_U16 len);
static void terminate_on_error(WP_handle handle, WP_CHAR *s);
WP_U32 WPT_DisplayRoutePortStats( WP_handle port_handle, WP_CHAR* type);
WP_U32 WPT_DisplayBPortStats( WP_handle port_handle, WP_CHAR* type);
WP_U32 WPT_DisplayFlowStats(WP_handle agg, WP_CHAR *type);
WP_U32 WPT_DisplayGenericFlowStats(WP_handle agg, WP_CHAR *type);
WP_U32 WPT_DisplayDeviceStats(WP_handle device_handle, WP_device device, WP_CHAR* type);
void WTE_DisplayEnetDeviceStats(WP_handle device_handle, WP_CHAR* type);
void App_TranslateAsciiToHex(WP_U8 *Hex,WP_CHAR *Ascii,WP_U32 length);

static WP_U8 WPE_DataUnitReceive(WP_handle rx_channel_handle, WP_U8 *hexa_ret);

static WP_U8 WPE_DataUnitSendReceive(const WP_CHAR* mac_dst, const WP_CHAR* mac_src, const WP_CHAR* svlan, const WP_CHAR* cvlan,
                                     const WP_CHAR* ip_dst, const WP_CHAR* ip_src,
                                     WP_handle tx_channel_handle, WP_handle rx_channel_handle);
static void WPE_PrintPceFilterStats(WP_handle filter);

static void WPE_PrintPolicerStatistics(void);

static void WPE_ResetPolicerStatistics(void);

// The declaration of the following functions are put in comment as currently they are not in use 

// static WP_U8 WPE_DataUnitReceiveOnly(const WP_CHAR* mac_dst, const WP_CHAR* mac_src, const WP_CHAR* svlan, const WP_CHAR* cvlan, 
//                                      const WP_CHAR* ip_dst, const WP_CHAR* ip_src, WP_handle rx_channel_handle);
// static void WPE_DataUnitSendOnly(const WP_CHAR* mac_dst, const WP_CHAR* mac_src, const WP_CHAR* svlan, const WP_CHAR* cvlan, 
//                                  const WP_CHAR* ip_dst, const WP_CHAR* ip_src, WP_handle tx_channel_handle);
//static void WPE_PacketSendOnly(WPE_pce_packet *packet, WP_handle tx_channel_handle);
//static WP_U8 WPE_PacketReceiveOnly(WPE_pce_packet *packet, WP_handle rx_channel_handle);
//static WP_U8 WPE_PacketSendReceive(WPE_pce_packet *packet, WP_handle tx_channel_handle, WP_handle rx_channel_handle);
//static void WPE_PrintPceGlobalStats(void);

//static void WPE_ResetPceFilterStats(WP_handle filter);

typedef enum
{
   WT_PHPORT_TYPE_SGMII,
   WT_PHPORT_TYPE_XGI,
   WT_PHPORT_TYPE_UPI,
   WT_PHPORT_TYPE_HOST,
   WT_PHPORT_TYPE_MAX,
} WT_PhysicalPortType;

typedef enum
{
   DEBUG_LEVEL_0 = 0,
   DEBUG_LEVEL_1,
   DEBUG_LEVEL_2,
   DEBUG_LEVEL_3,
   DEBUG_LEVEL_4,
   DEBUG_LEVEL_5,
   DEBUG_LEVEL_6,
   DEBUG_LEVEL_7,
   DEBUG_LEVEL_8,
   DEBUG_LEVEL_9,
   DEBUG_LEVEL_10
}WT_Debug_level;

   
#define STR_ENTER_COMMAND  "# Enter your command:"

#define WT_STATUS_UNINITIALIZED          0
#define WT_STATUS_INITIALIZED            1

#define WT_MAX_INTERFACES                30

#define WT_MAX_IF_ENET_TX_CHANNELS       8
#define WT_MAX_ENETS                     10

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
   WP_handle tx_channels[WT_MAX_IF_ENET_TX_CHANNELS];
   WP_handle flow_aggregation;
   WP_handle iw_port;
   WP_U8 default_flow_agg_number;
   WP_U16 flow_agg_vlan_id;
} WT_Interface;

typedef struct
{
   WP_iw_system_type iw_system_type;
   WP_handle iw_system;
   WT_Interface *interfaces;
} WT_TestGlobalInfo;

typedef union StatField
{
      WP_U64 field;
      struct {
            WP_U32 high;
            WP_U32 low;
      } part;
} WT_StatField;

//static WP_U8 WT_Init(void);
// static WP_U8 WT_CreateInterface(WP_U8 if_number, WT_PhysicalPortType type,
//                                 WP_U8 physical_port_number, WP_U8 loopback_mode,
//                                 WP_U8 tx_channels_number, WP_U8 default_flow_agg,
//                                 WP_U16 flow_agg_vlan_tag);
//static void WT_CreateIwSystemAndItsComponents(WP_iw_system_type type);
static void WT_UtilitiesMenu(void);

