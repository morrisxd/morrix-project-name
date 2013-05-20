//#include "stdincs.h"
//#include "h2n2h.h"
//#include "pwpr.h"
//#include "linux/if.h" //IFNAMSIZ
#include <stdlib.h>
#include <malloc.h>
#include <stdio.h>
#include "expat.h"
#include <arpa/inet.h>
#include "np_common.h"
#include "wddi_log.h"
#include <assert.h>
#include <glob.h>
//#include <alloc.h>
extern int g_logLevel;

#ifndef IFNAMSIZ
#define IFNAMSIZ 16
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#define    CLI_ASSERT(x)                                        \
    {                                                           \
        if (x<0)                                                \
        {                                                       \
            printf("\n%s", cli_error_str);                      \
            printf(("CLI assert error at File=%s,Line=%d\n",    \
                    __FILE__,__LINE__));                        \
            exit(1);                                            \
        };                                                      \
    }
#ifndef TYPE_DEF
#define TYPE_DEF
typedef unsigned char u8;
typedef unsigned short int u16;
typedef unsigned int u32;
typedef unsigned long long u64;
#endif

#define ARGLIST_BUFLEN 50
#define ARGV_MAX_CHARACTERS    255 // Need to accommmodate for DNS names and filenames
#define STRING_LIST_MAX 8192


#define PROGNAME_POS     0
#define CLASS_POS         1
#define METHOD_POS         2
#define ATTRIBUTE_POS     3

#define CLI_OPTION_U64             4
#define CLI_OPTION_STRING         5
#define CLI_OPTION_NAME_ONLY    6
#define CLI_OPTION_ENUM            7
#define CLI_OPTION_IPV4_ADDRESS    8
#define CLI_OPTION_FLOAT        9
#define CLI_OPTION_INT_LIST        10
#define CLI_OPTION_IP_ENDPOINT_ADDRESS        11
#define CLI_OPTION_STRING_LIST        12
#define CLI_OPTION_BITMAP        13
#define CLI_OPTION_S64             14
#define CLI_OPTION_DNS_NAME        15

#define CLI_OPTION_CALLBACK     50


#define    METHOD_ACCESSIBLE        1
#define    METHOD_NOT_ACCESSIBLE    0

#define XML_PARSER_BUFFER_SIZE 8192
#define IFNAME_MAX_LENGTH 256
#define IP_ADDRESS_MAX_LENGTH    12+3+1+2
//#define FILENAME_MAX 256

/***** TYPES *******/

typedef struct _expat_data
{
    char inside_class;
    char inside_attribute;
    int     argc_local;
    char** argv_local;
    XML_Parser parser;
	STRU_SET_INITAIL_CONFIG *np_configure_ptr; //new added
} expat_data_t;


typedef  u16 nsei_t;
typedef  u16 groupid_t; //ATM1
typedef  u16 nsvci_t;
typedef  u16 bvci_t;
typedef  u32 nri_t;
typedef  u32 udp_port_t;
typedef  u32 ip_address_t;

typedef struct
{
    const char* name; //Name of this instance
    const char* external_name; //name or alias used in some other context (like sangoma options, etc
} cli_enum_instance_t;
#define END_OF_ENUM_INSTANCE {NULL,NULL}

typedef struct _cli_attribute_t
{
    const char* name;
    int type;
    char value[256];
    u64 min;
    u64 max;
    void* default_value;
    cli_enum_instance_t* list_of_enums;
    void (*completion_handler)(struct _cli_attribute_t *arglist);
    char* description;
} cli_attribute_t;
#define EMPTY_CLI_ATTRIBUTE_VALUE    ""
#define END_OF_CLI_ATTRIBUTE {NULL,0,EMPTY_CLI_ATTRIBUTE_VALUE,0,0,NULL,NULL,NULL,NULL}

/* --------------- test for server useage ---------------*/
#if 0
#define SPP_MAX_STM1_PORT_NUM 2
#define SPP_MAX_TRUNK_NUM 63
#define SPP_MAX_GROUP_NUM 32

#define SPP_MAX_ROUTE_NUM 64

typedef struct STRU_E1_GROUP_INFO_TAG
{
    u8 crc_type;             /*NPU_E1T1_CRC16
	                           NPU_E1T1_CRC32 */
	u16 max_frame_size;      /*MTU on this phy*/
    u8 groupValid;          /*NPU_VALID or NPU_INVALID*/
    u32 slot_selection;      /*slot_selection = 0xfffefffe; means use slot1~slot15 slot17~slot31*/

}STRU_E1_GROUP_INFO;
typedef struct STRU_E1_TRUNK_INFO_TAG
{
    u8 trunkValid;      /*NPU_VALID or NPU_INVALID*/
    STRU_E1_GROUP_INFO group_info[SPP_MAX_GROUP_NUM];
	u32 clock_mode;    /*NPU_E1T1_CLOCK_MASTER
						 NPU_E1T1_CLOCK_SLAVE */
}STRU_E1_TRUNK_INFO;

typedef struct STRU_STM1_CFG_INFO_TAG
{
    u8 stm1Valid;     /*NPU_VALID or NPU_INVALID*/
    STRU_E1_TRUNK_INFO trunk_info[SPP_MAX_TRUNK_NUM];
	/*will add other pramaters later*/
    u32 E1T1type;      /*NPU_MODE_T1 OR NPU_MODE_E1*/
    u32 Opt_Mode;      /* details in E1T1OptMode struct ,for T1 means framing for Ericsson */
}STRU_STM1_CFG_INFO;

/*route info dst and gateway*/
typedef struct STRU_ROUTE_INFO_TAG
{
    u32 IpDst;
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
	STRU_STM1_CFG_INFO stru_stm1_cfg_info[SPP_MAX_STM1_PORT_NUM];
	STRU_ROUTE_INFO routeInfo[SPP_MAX_ROUTE_NUM];
}STRU_SET_INITAIL_CONFIG;
#endif
/* --------------- test for server useage ---------------*/


typedef void(*method_handler_t)(cli_attribute_t *arglist,STRU_SET_INITAIL_CONFIG *np_configure_ptr);
typedef struct
{
    const char* name;
    int    accessible;
    method_handler_t method;
    char** mandatory_arguments;
    char** optional_arguments;
    char* description;
} cli_method_t;
#define END_OF_CLI_METHOD {NULL,0,NULL,NULL,NULL,NULL}


typedef struct
{
    const char* name;
    cli_method_t *methods;
    cli_attribute_t *attributes;
    char* description;
} cli_class_t;


#if 0


typedef struct //parameters common for all object instances
{
    void*     next_of_same_class;
    activation_status_t activation_status;
    // NOTE, members above are not sinificant when comparing objects.
    // if more members are added Update object_compare() with sizeof !!!!!!!
    char      class_name[CLASS_NAME_SIZE];
    char      name[NAME_SIZE]; //names that are to be represented as linux interface names must be limited to IFNAMSIZ =16
    char    connected_to[NAME_SIZE];
} object_common_t;

typedef struct _e1_port
{
    object_common_t    common_params; // parameters common to all object instances
    u32        number;
    char    channelized[ENUM_NAME_SIZE];
    char    line_code[ENUM_NAME_SIZE];
    char    clock_mode[ENUM_NAME_SIZE];
    char    cable_impedance[ENUM_NAME_SIZE];
} cli_e1_port_t;
typedef struct _t1_port
{
    object_common_t    common_params; // parameters common to all object instances
    u32        number;
    char    line_code[ENUM_NAME_SIZE];
    char    clock_mode[ENUM_NAME_SIZE];
    char    cable_characteristics[ENUM_NAME_SIZE];
    char    framing[ENUM_NAME_SIZE];
} cli_t1_port_t;

typedef struct _cli_frame_relay_fraction
{
    object_common_t    common_params; // parameters common to all object instances
    u32     fraction_id;           // TR HK69444
    char    timeslots[INT_LIST_SIZE];
    char    interface_type[ENUM_NAME_SIZE];
    char    lmi_standard[ENUM_NAME_SIZE];
    u32     max_frame_size;
    u32     t391;
    u32     t392;
    u32     n391;
    u32     n392;
    u32     n393;
    char    crc_type[ENUM_NAME_SIZE];
} cli_frame_relay_fraction_t;

typedef struct _cli_frame_relay_pvc
{
    object_common_t    common_params; // parameters common to all object instances
    u32        dlci;
    u32        cir;
    u32        bc;
    u32        be;
} cli_frame_relay_pvc_t;

typedef struct _cli_nsvc_t
{
    object_common_t    common_params; // parameters common to all object instances
    nsei_t        nsei;
    nsvci_t        nsvci;
    char        deployed_at[HOST_NAME_MAX];
} cli_nsvc_t;


typedef struct
{
    object_common_t    common_params; // parameters common to all object instances
    nsei_t     nsei;
    char       sgsn_pool_or_sgsn_name[SGSN_NAME_SIZE];
    u32        port_base;
    u32        nse_sgsn_nsei_base;
    u32        general_debug_level;
    u32        traffic_debug_level;
} cli_sgsn_pool_proxy_service_t;

//ATM1
typedef struct
{
    object_common_t common_params; // parameters common to all object instances
    groupid_t group_id;
    char nsei_bsc_list[NSE_GROUP_LIST_LEN];
    u32 port_base;
    u32 nse_sgsn_nsei_base;
} cli_nse_group_t;

// fix TR HI79918
typedef struct
{
    object_common_t    common_params; // parameters common to all object instances
    nri_t    nri;
    float   capacity;
    char    administrative_state[ENUM_NAME_SIZE];
    char    ip_endpoint_address[IP_ENDPOINT_STRING_LENGTH];
} cli_sgsn_t;

typedef struct
{
    object_common_t    common_params; // parameters common to all object instances
    u8    nri_size;
    char  sgsn_list[SGSN_LIST_STRING_LENGTH];
} cli_sgsn_pool_t;

typedef struct
{
    object_common_t    common_params; // parameters common to all object instances
    u8    t_ns_reset;
    u8    t_ns_block;
    u8    t_ns_alive;
    u8    t_ns_test;
    u8    t_sns_prov;
} cli_network_service_t;

typedef struct
{
    object_common_t    common_params; // parameters common to all object instances
    nsei_t    nsei;
    u8    allowed_features;
    u8    t2_bvc_reset;
    u8    t_cell_periodic;
} cli_bssgp_service_t;

typedef struct
{
    object_common_t    common_params; // parameters common to all object instances
    char short_name[HOST_NAME_MAX];
} cli_node_t;

typedef struct
{
    object_common_t    common_params; // parameters common to all object instances
    char name_servers[IP_ADDRESS_LIST_MAX_LENGTH];
    char searched_domains[DNS_DOMAIN_NAME_MAX_LENGTH];
} cli_dns_client_t;

typedef struct
{
    object_common_t    common_params; // parameters common to all object instances
    char time_servers[IP_ADDRESS_LIST_MAX_LENGTH];
} cli_ntp_client_t;
typedef struct
{
    object_common_t    common_params; // parameters common to all object instances
    char destination[IP_NETWORK_NAME_SIZE];
    char gateway[IP_ADDRESS_MAX_LENGTH];
    char interface[IFNAMSIZ+1];
} cli_ip_route_t;
typedef struct
{
    object_common_t    common_params; // parameters common to all object instances
    u32 id;
} cli_vlan_endpoint_t;
typedef struct
{
    object_common_t    common_params; // parameters common to all object instances
    ip_address_t address;
    u8    mask;
    char interface[IFNAMSIZ+1]; // common_params::connected_to contains full specification and interface only last part
                                // which is typically the VLAN interface name
} cli_ip_t;

/* TR HI82318 add lom_ip into configuration file. exiyyin 07/02/2009 */
typedef struct
{
    object_common_t    common_params;
    ip_address_t address;        /* the ip address of LOM board */
} cli_lom_ip_t;

typedef struct
{
    object_common_t    common_params;
    u8 interface_id;
    char fr_type[FR_TYPE_LENGTH];
    u8 num_of_port;
    char port_list[PORT_LIST_LENGTH];
} cli_stm1_interface_t;

typedef struct
{
    // The configuration is the conceptional repository for all configuration of the node
    // All classe of the system have a root element from this struct.
    // All instances of the same class are then linked together
    // NOTE !!! When adding new members. MAKE SURE TO MODIFY cli_configuration_delete() as well
    cli_sgsn_pool_proxy_service_t *sgsn_pool_proxy_service;
    cli_nse_group_t *nse_group; //ATM1
    cli_bssgp_service_t    *bssgp_service;
    cli_sgsn_t        *sgsn;
    cli_sgsn_pool_t        *sgsn_pool;
    cli_sgsn_pool_proxy_service_common_t    *sgsn_pool_proxy_service_common;
    cli_network_service_t    *network_service;
    cli_nsvc_t        *nsvc;
    cli_frame_relay_pvc_t *frame_relay_pvc;
    cli_frame_relay_fraction_t *frame_relay_fraction;
    cli_e1_port_t        *e1_port; //root
    cli_t1_port_t        *t1_port; //root
    cli_node_t            *node;
    cli_dns_client_t    *dns_client;
    cli_ntp_client_t    *ntp_client;
    cli_ip_route_t        *ip_route;
    cli_vlan_endpoint_t    *vlan_endpoint;
    cli_ip_t            *ip;
    cli_lom_ip_t        *lom_ip;  // TR HI82318
    cli_stm1_interface_t   *stm1_interface;
} configuration_t;
#endif

/********* LOCAL FUNCTIONS *********/

static void cli_e1_port_create_internal(cli_attribute_t *arglist,STRU_SET_INITAIL_CONFIG *np_configure_ptr);
static void cli_t1_port_create_internal(cli_attribute_t *arglist,STRU_SET_INITAIL_CONFIG *np_configure_ptr);

static void cli_frame_relay_fraction_create_internal(cli_attribute_t *arglist,STRU_SET_INITAIL_CONFIG *np_configure_ptr);
static void cli_frame_relay_pvc_create_internal(cli_attribute_t *arglist,STRU_SET_INITAIL_CONFIG *np_configure_ptr);
static void cli_sgsn_pool_create_internal(cli_attribute_t *arglist,STRU_SET_INITAIL_CONFIG *np_configure_ptr);
static void cli_ip_route_create_internal(cli_attribute_t *arglist,STRU_SET_INITAIL_CONFIG *np_configure_ptr);
static void cli_vlan_endpoint_create_internal(cli_attribute_t *arglist, STRU_SET_INITAIL_CONFIG *np_configure_ptr);
static void cli_ip_create_internal(cli_attribute_t *arglist,STRU_SET_INITAIL_CONFIG *np_configure_ptr);

//show all stm1 interface counters
static void cli_stm1_interface_create_internal(cli_attribute_t *arglist,STRU_SET_INITAIL_CONFIG *np_configure_ptr);

static cli_attribute_t* get_attribute_from_arglist(cli_attribute_t *arglist,const char* attr_name);
// elinbii add for TR HI99732
static int get_fault_id_from_arglist(cli_attribute_t *arglist,const char* attr_name,int arglist_counter);

static int get_u32_from_arglist(cli_attribute_t *arglist,char* attr_name, u32 *integer);
static int get_u16_from_arglist(cli_attribute_t *arglist,char* attr_name, u16 *integer);
static int get_u8_from_arglist(cli_attribute_t *arglist,char* attr_name, u8 *integer);
//static int get_r64_from_arglist(cli_attribute_t *arglist,char* attr_name, r64 *r64value);
//fix TR HI79918
//static int get_r32_from_arglist(cli_attribute_t *arglist,char* attr_name, r32 *r32value);

static int get_enum_from_arglist(cli_attribute_t *arglist,char* attr_name, char string[]);
static int get_string_from_arglist(cli_attribute_t *arglist,char* attr_name, char* string);
static int get_ip_address_from_arglist(cli_attribute_t *arglist,char* attr_name, ip_address_t *ip_address);

/**** EXTERNAL VARIABLES *****/
char cli_error_str[256];
char cli_completion_str[4096];
u32 om_arg_nsei_bsc;


/***** LOCAL VARIABLES *****/
static char parse_error; //Global variable used to signal errors found during parsing of XML files.
static char filename_current[64] = "";
static char* current_filename; //The name of the currently parsed XML file
//int class_num = 0;
//STRU_SET_INITAIL_CONFIG g_npinitial;


// Start of E1 port

static cli_enum_instance_t e1_port_channelized_enums[] =
{
    {
        "false",//const char* name;
        ""//const char* external_name;
    },
    {
        "true_crc4",//const char* name;
        ""//const char* external_name;
    },
    {
        "true_not_crc4",//const char* name;
        ""//const char* external_name;
    },
    END_OF_ENUM_INSTANCE
};
static cli_enum_instance_t e1_port_line_code_enums[] =
{
    {
        "AMI",//const char* name;
        ""//const char* external_name;
    },
    {
        "HDB3",//const char* name;
        ""//const char* external_name;
    },
    END_OF_ENUM_INSTANCE
};
static cli_enum_instance_t e1_port_clock_mode_enums[] =
{
    {
        "master",//const char* name;
        ""//const char* external_name;
    },
    {
        "slave",//const char* name;
        ""//const char* external_name;
    },
    END_OF_ENUM_INSTANCE
};
static cli_enum_instance_t e1_port_cable_impedance_enums[] =
{
    {
        "75_ohm",//const char* name;
        ""//const char* external_name;
    },
    {
        "120_ohm",//const char* name;
        ""//const char* external_name;
    },
    END_OF_ENUM_INSTANCE
};

static cli_attribute_t e1_port_attribute_table[] =
{
    {
        "number",//const char* name;
        CLI_OPTION_U64,//int type;
        EMPTY_CLI_ATTRIBUTE_VALUE,//void* value;
        1,//u64 min;
        169,//u64 max;
        NULL,//void* attr_default;
        NULL,//cli_enum_instance_t* list_of_enums;
        NULL,//completion_handler;
        "number descr"//char* description;
    },

    {
        "connected_to",//const char* name;
        CLI_OPTION_NAME_ONLY,//int type;
        "",//void* value;
        0,//u64 min;
        0,//u64 max;
        NULL,//void* attr_default;
        NULL,//cli_enum_instance_t* list_of_enums;
        NULL,//completion_handler;
        ""//char* description;
    },

#if 1
    {
        "channelized",//const char* name;
        CLI_OPTION_ENUM,//int type;
        EMPTY_CLI_ATTRIBUTE_VALUE,//void* value;
        0,//u64 min;
        0,//u64 max;
        "true_crc4",//void* attr_default;
        e1_port_channelized_enums,//cli_enum_instance_t* list_of_enums;
        NULL,//completion_handler;
        "Channelized descr"//char* description;
    },
    {
        "line_code",//const char* name;
        CLI_OPTION_ENUM,//int type;
        EMPTY_CLI_ATTRIBUTE_VALUE,//void* value;
        0,//u64 min;
        0,//u64 max;
        "HDB3",//void* attr_default;
        e1_port_line_code_enums,//cli_enum_instance_t* list_of_enums;
        NULL,//completion_handler;
        "line_code descr"//char* description;
    },

#endif
    {
        "clock_mode",//const char* name;
        CLI_OPTION_ENUM,//int type;
        EMPTY_CLI_ATTRIBUTE_VALUE,//void* value;
        0,//u64 min;
        0,//u64 max;
        "master",//void* attr_default;
        e1_port_clock_mode_enums,//cli_enum_instance_t* list_of_enums;
        NULL,//completion_handler;
        "clock_mode descr"//char* description;
    },
    {
        "cable_impedance",//const char* name;
        CLI_OPTION_ENUM,//int type;
        EMPTY_CLI_ATTRIBUTE_VALUE,//void* value;
        0,//u64 min;
        0,//u64 max;
        "120_ohm",//void* attr_default;
        e1_port_cable_impedance_enums,//cli_enum_instance_t* list_of_enums;
        NULL,//completion_handler;
        "cable_impedance descr"//char* description;
    },
    END_OF_CLI_ATTRIBUTE
};


static cli_method_t e1_port_method_table[] =
{
    {
        "create_internal",//name
        METHOD_NOT_ACCESSIBLE,
        cli_e1_port_create_internal,//method
        (char*[]){"number","channelized","line_code","clock_mode","cable_impedance",NULL},//mandatory_arguments
        NULL,//optional_arguments
        "e1_port create description"//description
    },
    {
        "show",//name
        METHOD_ACCESSIBLE,
        //cli_e1_port_show,//method
        NULL,
        NULL,//mandatory_arguments
        NULL,//optional_arguments
        "e1_port show description"//description
    },
    END_OF_CLI_METHOD
};

// Start of T1 port
static cli_enum_instance_t t1_port_line_code_enums[] =
{
    {
        "AMI",//const char* name;
        ""//const char* external_name;
    },
    {
        "b8zs",//const char* name;
        ""//const char* external_name;
    },
    END_OF_ENUM_INSTANCE
};
static cli_enum_instance_t t1_port_framing_enums[] =
{
    {
        "esf",//const char* name;
        ""//const char* external_name;
    },
    {
        "d4",//const char* name;
        ""//const char* external_name;
    },
    END_OF_ENUM_INSTANCE
};
static cli_enum_instance_t t1_port_clock_mode_enums[] =
{
    {
        "master",//const char* name;
        ""//const char* external_name;
    },
    {
        "slave",//const char* name;
        ""//const char* external_name;
    },
    END_OF_ENUM_INSTANCE
};
static cli_enum_instance_t t1_port_cable_characteristics_enums[] =
{
    {
        "0dB",//const char* name;
        ""//const char* external_name;
    },
    {
        "7.5dB",//const char* name;
        ""//const char* external_name;
    },
    {
        "15dB",//const char* name;
        ""//const char* external_name;
    },
    {
        "22.5dB",//const char* name;
        ""//const char* external_name;
    },
    {
        "0-110ft",//const char* name;
        ""//const char* external_name;
    },
    {
        "110-220ft",//const char* name;
        ""//const char* external_name;
    },
    {
        "220-330ft",//const char* name;
        ""//const char* external_name;
    },
    {
        "330-440ft",//const char* name;
        ""//const char* external_name;
    },
    {
        "440-550ft",//const char* name;
        ""//const char* external_name;
    },
    {
        "550-660ft",//const char* name;
        ""//const char* external_name;
    },
    END_OF_ENUM_INSTANCE
};

static cli_attribute_t t1_port_attribute_table[] =
{
    {
        "number",//const char* name;
        CLI_OPTION_U64,//int type;
        EMPTY_CLI_ATTRIBUTE_VALUE,//void* value;
        1,//u64 min;
        127,//u64 max;
        NULL,//void* attr_default;
        NULL,//cli_enum_instance_t* list_of_enums;
        NULL,//completion_handler;
        "number descr"//char* description;
    },
    {
        "connected_to",//const char* name;
        CLI_OPTION_NAME_ONLY,//int type;
        "",//void* value;
        0,//u64 min;
        0,//u64 max;
        NULL,//void* attr_default;
        NULL,//cli_enum_instance_t* list_of_enums;
        NULL,//completion_handler;
        ""//char* description;
    },
    {
        "line_code",//const char* name;
        CLI_OPTION_ENUM,//int type;
        EMPTY_CLI_ATTRIBUTE_VALUE,//void* value;
        0,//u64 min;
        0,//u64 max;
        "b8zs",//void* attr_default;
        t1_port_line_code_enums,//cli_enum_instance_t* list_of_enums;
        NULL,//completion_handler;
        "line_code descr"//char* description;
    },
    {
        "framing",//const char* name;
        CLI_OPTION_ENUM,//int type;
        EMPTY_CLI_ATTRIBUTE_VALUE,//void* value;
        0,//u64 min;
        0,//u64 max;
        "esf",//void* attr_default;
        t1_port_framing_enums,//cli_enum_instance_t* list_of_enums;
        NULL,//completion_handler;
        "framing descr"//char* description;
    },
    {
        "clock_mode",//const char* name;
        CLI_OPTION_ENUM,//int type;
        EMPTY_CLI_ATTRIBUTE_VALUE,//void* value;
        0,//u64 min;
        0,//u64 max;
        "master",//void* attr_default;
        t1_port_clock_mode_enums,//cli_enum_instance_t* list_of_enums;
        NULL,//completion_handler;
        "clock_mode descr"//char* description;
    },
    {
        "cable_characteristics",//const char* name;
        CLI_OPTION_ENUM,//int type;
        EMPTY_CLI_ATTRIBUTE_VALUE,//void* value;
        0,//u64 min;
        0,//u64 max;
        "0dB",//void* attr_default;
        t1_port_cable_characteristics_enums,//cli_enum_instance_t* list_of_enums;
        NULL,//completion_handler;
        "cable_impedance descr"//char* description;
    },
    END_OF_CLI_ATTRIBUTE
};


static cli_method_t t1_port_method_table[] =
{
    {
        "create_internal",//name
        METHOD_NOT_ACCESSIBLE,
        cli_t1_port_create_internal,//method
        (char*[]){"number","framing","line_code","clock_mode","cable_characteristics",NULL},//mandatory_arguments
        NULL,//optional_arguments
        "t1_port create description"//description
    },
    {
        "show",//name
        METHOD_ACCESSIBLE,
        //cli_t1_port_show,//method
        NULL,
        NULL,//mandatory_arguments
        NULL,//optional_arguments
        "t1_port show description"//description
    },
    END_OF_CLI_METHOD
};

// Start of Frame relay

static cli_enum_instance_t frame_relay_fraction_interface_type_enums[] =
{
    {
        "dte",//const char* name;
        ""//const char* external_name;
    },
    {
        "dce",//const char* name;
        ""//const char* external_name;
    },
    END_OF_ENUM_INSTANCE
};
static cli_enum_instance_t frame_relay_fraction_lmi_standard_enums[] =
{
    {
        "q933",//const char* name;
        ""//const char* external_name;
    },
    {
        "ansi1991",//const char* name;
        ""//const char* external_name;
    },
    {
        "lmi",//const char* name;
        ""//const char* external_name;
    },
    END_OF_ENUM_INSTANCE
};

static cli_enum_instance_t frame_relay_fraction_crc_type_enums[] =
{
    {
        "crc16",//const char* name;
        ""//const char* external_name;
    },
    {
        "crc32",//const char* name;
        ""//const char* external_name;
    },
    END_OF_ENUM_INSTANCE
};

static cli_attribute_t frame_relay_fraction_attribute_table[] =
{
// name not specified
    {
        "connected_to",//const char* name;
        CLI_OPTION_STRING,//int type;
        EMPTY_CLI_ATTRIBUTE_VALUE,//void* value;
        1,//u64 min;
        15,//u64 max; //==IFNAMSIZ
        NULL,//void* attr_default;
        NULL,//cli_enum_instance_t* list_of_enums;
        NULL,//completion_handler;
        "connected_to descr"//char* description;
    },
    // TR HK69444
    {
        "fraction_id",//const char* name;
        CLI_OPTION_U64,//int type;
        EMPTY_CLI_ATTRIBUTE_VALUE,//void* value;
        0,//u64 min;
        31,//u64 max;
        "0",//void* attr_default;
        NULL,//cli_enum_instance_t* list_of_enums;
        NULL,//completion_handler;
        "fraction ID"//char* description;
    },
    {
        "timeslots",//const char* name;
        CLI_OPTION_INT_LIST,//int type;
        EMPTY_CLI_ATTRIBUTE_VALUE,//void* value;
        0,//u64 min;
        31,//u64 max;
        NULL,//void* attr_default;
        NULL,//cli_enum_instance_t* list_of_enums;
        NULL,//completion_handler;
        "timeslots descr"//char* description;
    },
    {
        "interface_type",//const char* name;
        CLI_OPTION_ENUM,//int type;
        EMPTY_CLI_ATTRIBUTE_VALUE,//void* value;
        0,//u64 min;
        0,//u64 max;
        "dte",//void* attr_default;
        frame_relay_fraction_interface_type_enums,//cli_enum_instance_t* list_of_enums;
        NULL,//completion_handler;
        "interface_type descr"//char* description;
    },
    {
        "lmi_standard",//const char* name;
        CLI_OPTION_ENUM,//int type;
        EMPTY_CLI_ATTRIBUTE_VALUE,//void* value;
        0,//u64 min;
        0,//u64 max;
        "q933",//void* attr_default;
        frame_relay_fraction_lmi_standard_enums,//cli_enum_instance_t* list_of_enums;
        NULL,//completion_handler;
        "lmi_standard descr"//char* description;
    },
    {
        "max_frame_size",//const char* name;
        CLI_OPTION_U64,//int type;
        EMPTY_CLI_ATTRIBUTE_VALUE,//void* value;
        262,//u64 min;
        4096,//u64 max;
        "1600",//void* attr_default;
        NULL,//cli_enum_instance_t* list_of_enums;
        NULL,//completion_handler;
        "max_frame_size descr"//char* description;
    },
    {
        "t391",//const char* name;
        CLI_OPTION_U64,//int type;
        EMPTY_CLI_ATTRIBUTE_VALUE,//void* value;
        5,//u64 min;
        29,//u64 max;
        "10",//void* attr_default;
        NULL,//cli_enum_instance_t* list_of_enums;
        NULL,//completion_handler;
        "t391 descr"//char* description;
    },
    {
        "t392",//const char* name;
        CLI_OPTION_U64,//int type;
        EMPTY_CLI_ATTRIBUTE_VALUE,//void* value;
        6,//u64 min;
        30,//u64 max;
        "15",//void* attr_default;
        NULL,//cli_enum_instance_t* list_of_enums;
        NULL,//completion_handler;
        "t392 descr"//char* description;
    },
    {
        "n391",//const char* name;
        CLI_OPTION_U64,//int type;
        EMPTY_CLI_ATTRIBUTE_VALUE,//void* value;
        1,//u64 min;
        255,//u64 max;
        "6",//void* attr_default;
        NULL,//cli_enum_instance_t* list_of_enums;
        NULL,//completion_handler;
        "n391 descr"//char* description;
    },
    {
        "n392",//const char* name;
        CLI_OPTION_U64,//int type;
        EMPTY_CLI_ATTRIBUTE_VALUE,//void* value;
        1,//u64 min;
        10,//u64 max;
        "3",//void* attr_default; // Should be 1-N393
        NULL,//cli_enum_instance_t* list_of_enums;
        NULL,//completion_handler;
        "n392 descr"//char* description;
    },
    {
        "n393",//const char* name;
        CLI_OPTION_U64,//int type;
        EMPTY_CLI_ATTRIBUTE_VALUE,//void* value;
        1,//u64 min;
        10,//u64 max;
        "4",//void* attr_default;
        NULL,//cli_enum_instance_t* list_of_enums;
        NULL,//completion_handler;
        "n393 descr"//char* description;
    },
    {
        "crc_type",//const char* name;
        CLI_OPTION_ENUM,//int type;
        EMPTY_CLI_ATTRIBUTE_VALUE,//void* value;
        0,//u64 min;
        0,//u64 max;
        "crc16",//void* attr_default;
        frame_relay_fraction_crc_type_enums,//cli_enum_instance_t* list_of_enums;
        NULL,//completion_handler;
        "crc_type descr"//char* description;
    },
    END_OF_CLI_ATTRIBUTE
};


static cli_method_t frame_relay_fraction_method_table[] =
{
    {
        "create_internal",//name
        METHOD_NOT_ACCESSIBLE,
        cli_frame_relay_fraction_create_internal,//method
        (char*[]){"connected_to","timeslots","interface_type","lmi_standard","max_frame_size",
                  "t391","t392","n391","n392","n393","crc_type",NULL},//mandatory_arguments
        NULL,//optional_arguments
        "frame_relay_fraction create_internal description"//description
    },
    {
        "show",//name
        METHOD_ACCESSIBLE,
        //cli_frame_relay_fraction_show,//method
        NULL,
        NULL,//mandatory_arguments
        NULL,//optional_arguments
        "frame_relay_fraction show description"//description
    },
    END_OF_CLI_METHOD
};

static cli_attribute_t frame_relay_pvc_attribute_table[] =
{
// name not specified
    {
        "connected_to",//const char* name;
        CLI_OPTION_STRING,//int type;
        EMPTY_CLI_ATTRIBUTE_VALUE,//void* value;
        1,//u64 min;
        15,//u64 max; //==IFNAMSIZ
        NULL,//void* attr_default;
        NULL,//cli_enum_instance_t* list_of_enums;
        NULL,//completion_handler;
        "connected_to descr"//char* description;
    },
    {
        "dlci",//const char* name;
        CLI_OPTION_U64,//int type;
        EMPTY_CLI_ATTRIBUTE_VALUE,//void* value;
        16,//u64 min;
        1007,//u64 max;
        NULL,//void* attr_default;
        NULL,//cli_enum_instance_t* list_of_enums;
        NULL,//completion_handler;
        "dlci descr"//char* description;
    },
    {
        "cir",//const char* name;
        CLI_OPTION_U64,//int type;
        EMPTY_CLI_ATTRIBUTE_VALUE,//void* value;
        0,//u64 min;
        42949672,//u64 max;
        NULL,//void* attr_default;
        NULL,//cli_enum_instance_t* list_of_enums;
        NULL,//completion_handler;
        "cir descr"//char* description;
    },
    {
        "bc",//const char* name;
        CLI_OPTION_U64,//int type;
        EMPTY_CLI_ATTRIBUTE_VALUE,//void* value;
        0,//u64 min;
        42949672,//u64 max;
        NULL,//void* attr_default;
        NULL,//cli_enum_instance_t* list_of_enums;
        NULL,//completion_handler;
        "bc descr"//char* description;
    },
    {
        "be",//const char* name;
        CLI_OPTION_U64,//int type;
        EMPTY_CLI_ATTRIBUTE_VALUE,//void* value;
        0,//u64 min;
        42949672,//u64 max;
        NULL,//void* attr_default;
        NULL,//cli_enum_instance_t* list_of_enums;
        NULL,//completion_handler;
        "be descr"//char* description;
    },
    END_OF_CLI_ATTRIBUTE
};

static cli_method_t frame_relay_pvc_method_table[] =
{
    {
        "create_internal",//name
        METHOD_NOT_ACCESSIBLE,
        //NULL,
        cli_frame_relay_pvc_create_internal,//method
        (char*[]){"connected_to","dlci","cir","bc","be",NULL},//mandatory_arguments
        NULL,//optional_arguments
        "frame_relay_pvc create_internal description"//description
    },
    {
        "show",//name
        METHOD_ACCESSIBLE,
        //cli_frame_relay_pvc_show,//method
        NULL,
        NULL,//mandatory_arguments
        NULL,//optional_arguments
        "frame_relay_pvc show description"//description
    },
    END_OF_CLI_METHOD
};


// Start of sgsn_pool
static cli_attribute_t sgsn_pool_attribute_table[] =
{
    {
        "name",//const char* name;
        CLI_OPTION_STRING,//int type;
        EMPTY_CLI_ATTRIBUTE_VALUE,//void* value;
        1,//u64 min;
        32,//u64 max;
        NULL,//void* attr_default;
        NULL,//cli_enum_instance_t* list_of_enums;
        NULL,//completion_handler;
        ""//char* description;
    },
    {
        "nri_size",//const char* name;
        CLI_OPTION_U64,//int type;
        EMPTY_CLI_ATTRIBUTE_VALUE,//int value;
        1,//u64 min;
        8,//u64 max;
        "5",//void* attr_default;
        NULL,//cli_enum_instance_t* list_of_enums;
        NULL,//completion_handler;
        ""//char* description;
    },
#if 1
    {
        "sgsn_list",//const char* name;
        CLI_OPTION_STRING_LIST,//int type;
        EMPTY_CLI_ATTRIBUTE_VALUE,//int value;
        1,//u64 min;
        1024,//u64 max;
        NULL,//void* attr_default;
        NULL,//cli_enum_instance_t* list_of_enums;
        NULL,//completion_handler;
        ""//char* description;
    },
#endif
    END_OF_CLI_ATTRIBUTE
};

static cli_method_t sgsn_pool_method_table[] =
{
    {
        "create_internal",//name
        METHOD_NOT_ACCESSIBLE,
        cli_sgsn_pool_create_internal,//method
        (char*[]){"name","nri_size","sgsn_list",NULL},//mandatory_arguments
        NULL,//optional_arguments
        "" //description
    },
    {
        "show",//name
        METHOD_ACCESSIBLE,
        //cli_sgsn_pool_show,//method
        NULL,
        NULL,//mandatory_arguments
        NULL,//optional_arguments
        "" //description
    },
    END_OF_CLI_METHOD
};

// start of ip_route

static cli_attribute_t ip_route_attribute_table[] =
{
    {
        "destination",//const char* name;
        CLI_OPTION_STRING,//int type;
        EMPTY_CLI_ATTRIBUTE_VALUE,//int value;
        1,//u64 min;
        IP_ADDRESS_MAX_LENGTH,//u64 max;
        NULL,//void* attr_default;
        NULL,//cli_enum_instance_t* list_of_enums;
        NULL,//completion_handler;
        ""//char* description;
    },
    {
        "gateway",//const char* name;
        CLI_OPTION_STRING,//int type;
        EMPTY_CLI_ATTRIBUTE_VALUE,//int value;
        1,//u64 min;
        IP_ADDRESS_MAX_LENGTH,//u64 max;
        NULL,//void* attr_default;
        NULL,//cli_enum_instance_t* list_of_enums;
        NULL,//completion_handler;
        ""//char* description;
    },
#if 1
    {
        "interface",//const char* name;
        CLI_OPTION_STRING,//int type;
        EMPTY_CLI_ATTRIBUTE_VALUE,//int value;
        1,//u64 min;
        IFNAMSIZ,//u64 max;
        NULL,//void* attr_default;
        NULL,//cli_enum_instance_t* list_of_enums;
        NULL,//completion_handler;
        ""//char* description;
    },
#endif
    END_OF_CLI_ATTRIBUTE
};
static cli_method_t ip_route_method_table[] =
{
    {
        "create_internal",//name
        METHOD_NOT_ACCESSIBLE,
        cli_ip_route_create_internal,//method
        NULL,//mandatory_arguments
        NULL,//optional_arguments
        "" //description
    },
    {
        "show",//name
        METHOD_ACCESSIBLE,
        //cli_ip_route_show,//method
        NULL,
        NULL,//mandatory_arguments
        NULL,//optional_arguments
        "" //description
    },
    END_OF_CLI_METHOD
};


// start of vlan_endpoint

static cli_attribute_t vlan_endpoint_attribute_table[] =
{
    {
        "id",//const char* name;
        CLI_OPTION_U64,//int type;
        EMPTY_CLI_ATTRIBUTE_VALUE,//int value;
        1,//u64 min;
        4095,//u64 max;
        NULL,//void* attr_default;
        NULL,//cli_enum_instance_t* list_of_enums;
        NULL,//completion_handler;
        ""//char* description;
    },
#if 1
    {
        "connected_to",//const char* name;
        CLI_OPTION_STRING,//int type;
        EMPTY_CLI_ATTRIBUTE_VALUE,//int value;
        1,//u64 min;
        IFNAMSIZ,//u64 max;
        NULL,//void* attr_default;
        NULL,//cli_enum_instance_t* list_of_enums;
        NULL,//completion_handler;
        ""//char* description;
    },
#endif
    END_OF_CLI_ATTRIBUTE
};

static cli_method_t vlan_endpoint_method_table[] =
{
    {
        "create_internal",//name
        METHOD_NOT_ACCESSIBLE,
        cli_vlan_endpoint_create_internal,//method
        NULL,//mandatory_arguments
        NULL,//optional_arguments
        "" //description
    },
    {
        "show",//name
        METHOD_ACCESSIBLE,
        //cli_vlan_endpoint_show,//method
        NULL,
        NULL,//mandatory_arguments
        NULL,//optional_arguments
        "" //description
    },
    END_OF_CLI_METHOD
};


// start of ip

static cli_attribute_t ip_attribute_table[] =
{
    {
        "address",//const char* name;
        CLI_OPTION_DNS_NAME,//int type;
        EMPTY_CLI_ATTRIBUTE_VALUE,//int value;
        1,//u64 min;
        IP_ADDRESS_MAX_LENGTH,//u64 max;
        NULL,//void* attr_default;
        NULL,//cli_enum_instance_t* list_of_enums;
        NULL,//completion_handler;
        ""//char* description;
    },
#if 1
    {
        "connected_to",//const char* name;
        CLI_OPTION_STRING,//int type;
        EMPTY_CLI_ATTRIBUTE_VALUE,//int value;
        1,//u64 min;
        IFNAMSIZ,//u64 max;
        NULL,//void* attr_default;
        NULL,//cli_enum_instance_t* list_of_enums;
        NULL,//completion_handler;
        ""//char* description;
    },
#endif
    END_OF_CLI_ATTRIBUTE
};
static cli_method_t ip_method_table[] =
{
    {
        "create_internal",//name
        METHOD_NOT_ACCESSIBLE,
        cli_ip_create_internal,//method
        NULL,//mandatory_arguments
        NULL,//optional_arguments
        "" //description
    },
    {
        "show",//name
        METHOD_ACCESSIBLE,
        //cli_ip_show,//method
        NULL,
        NULL,//mandatory_arguments
        NULL,//optional_arguments
        "" //description
    },
    END_OF_CLI_METHOD
};

//add stm1 interface show
static cli_attribute_t stm1_interface_attribute_table[] =
{
    {
        "interface_id",//const char* name;
        CLI_OPTION_U64,//int type;
        EMPTY_CLI_ATTRIBUTE_VALUE,//void* value;
        1,//u64 min;
        2,//u64 max;
        NULL,//void* attr_default;
        NULL,//cli_enum_instance_t* list_of_enums;
        NULL,//completion_handler;
        ""//char* description;
    },
    {
        "fr_type",//const char* name;
        CLI_OPTION_STRING,//int type;
        EMPTY_CLI_ATTRIBUTE_VALUE,//void* value;
        2,//u64 min;
        5,//u64 max;
        NULL,//void* attr_default;
        NULL,//cli_enum_instance_t* list_of_enums;
        NULL,//completion_handler;
        ""//char* description;
    },
    {
        "operate_mode",//const char* name;
        CLI_OPTION_STRING,//int type;
        EMPTY_CLI_ATTRIBUTE_VALUE,//void* value;
        0,//u64 min;
        0xf8,//u64 max;
        NULL,//void* attr_default;
        NULL,//cli_enum_instance_t* list_of_enums;
        NULL,//completion_handler;
        ""//char* description;
    },
    {
        "num_of_port",//const char* name;
        CLI_OPTION_U64,//int type;
        EMPTY_CLI_ATTRIBUTE_VALUE,//void* value;
        1,//u64 min;
        128,//u64 max;
        NULL,//void* attr_default;
        NULL,//cli_enum_instance_t* list_of_enums;
        NULL,//completion_handler;
        ""//char* description;
    },
    {
        "port_list",//const char* name;
        CLI_OPTION_STRING,//int type;
        EMPTY_CLI_ATTRIBUTE_VALUE,//void* value;
        1,//u64 min;
        512,//u64 max;
        NULL,//void* attr_default;
        NULL,//cli_enum_instance_t* list_of_enums;
        NULL,//completion_handler;
        ""//char* description;
    },
    END_OF_CLI_ATTRIBUTE
};



static cli_method_t stm1_interface_method_table[] =
{
    {
        "create_internal",//name
        METHOD_NOT_ACCESSIBLE,
        cli_stm1_interface_create_internal,//method
        (char*[]){"interface_id","fr_type","num_of_port","port_list",NULL},//mandatory_arguments
        NULL,//optional_arguments
        "" //description
    },
    {
        "show",//name
        METHOD_ACCESSIBLE,
        //cli_stm1_interface_show,//method
        NULL,
        NULL,//mandatory_arguments
        NULL,//optional_arguments
        "" //description
    },
    END_OF_CLI_METHOD
};


////////////////////////              CLASSES        //////////////////////////////////////////////////
// All classes visible in the CLI are listed here:
static cli_class_t class_table[] =
{
    {
        "e1_port",//name
        e1_port_method_table,//methods
        e1_port_attribute_table,//attributes
        "E1 Port"//description
    },
    {
        "t1_port",//name
        t1_port_method_table,//methods
        t1_port_attribute_table,//attributes
        "T1 Port"//description
    },
    {
        "frame_relay_fraction",//name
        frame_relay_fraction_method_table,//methods
        frame_relay_fraction_attribute_table,//attributes
        "Frame Relay Fraction"//description
    },
    {
        "frame_relay_pvc",//name
        frame_relay_pvc_method_table,//methods
        frame_relay_pvc_attribute_table,//attributes
        "Frame Relay PVC"//description
    },
    {
        "sgsn_pool",//name
        sgsn_pool_method_table,//methods
        sgsn_pool_attribute_table,//attributes
        ""//description
    },
    {
        "ip_route",//name
        ip_route_method_table,//methods
        ip_route_attribute_table,//attributes
        ""//description
    },
    {
        "vlan_endpoint",//name
        vlan_endpoint_method_table,//methods
        vlan_endpoint_attribute_table,//attributes
        ""//description
    },
    {
        "ip",//name
        ip_method_table,//methods
        ip_attribute_table,//attributes
        ""//description
    },
    {
        "stm1_interface",//name
        stm1_interface_method_table,//methods
        stm1_interface_attribute_table,//attributes
        "stm1 interface description"//description
    },
    {
        NULL,//name
        NULL,//methods
        NULL,//attributes
        NULL//description
    }
};

u32 g_stm1_id,g_trunk_id,g_group_id,g_dlci;

static void
cli_e1_port_create_internal(cli_attribute_t *arglist,STRU_SET_INITAIL_CONFIG *np_configure_ptr)
{
	STRU_SET_INITAIL_CONFIG *p = np_configure_ptr;
	u32 stm1_id, trunk_id;

	char clock_mode[10],connect2stm[8] = "";

	get_u32_from_arglist(arglist,"number",&trunk_id);
	{
		assert(0 != trunk_id);
		trunk_id -= 1;
	}

	get_string_from_arglist(arglist,"connected_to",connect2stm);
	{
		if(!(sscanf(connect2stm, "s%u", &stm1_id) == 1))
		{
			WDDI_ERR("error cli_frame_relay_fraction_create_internal from connected_to ");
			return;
		}


		assert(0 != stm1_id);

		stm1_id -= 1;

		if(stm1_id <2)
		{
			//if(stm1_id == g_stm1_id)
            p->stru_stm1_cfg_info[stm1_id].trunk_info[trunk_id].trunkValid = NPU_VALID;
			//else
            //WDDI_ERR("error from connected_to: stm1_id = %d. ",stm1_id);
		}
		else
			WDDI_ERR("error from connected_to: stm1_id = %d. ",stm1_id);
	}
    char opt_mode[32];
	get_string_from_arglist(arglist,"channelized", opt_mode);
	{
		if (strcmp((const char *)opt_mode, "true_crc4") == 0)
			p->stru_stm1_cfg_info[stm1_id].trunk_info[trunk_id].Opt_Mode= OPMODE_E1_TRUE_CRC4;
		else if (strcmp((const char *)opt_mode, "false") == 0)
			p->stru_stm1_cfg_info[stm1_id].trunk_info[trunk_id].Opt_Mode = OPMODE_E1_FALSE;
	}

	get_enum_from_arglist(arglist,"clock_mode",clock_mode);
	{
		if (strcmp((const char *)clock_mode, "master") == 0)
			p->stru_stm1_cfg_info[stm1_id].trunk_info[trunk_id].clock_mode = NPU_E1T1_CLOCK_MASTER;
		else
			p->stru_stm1_cfg_info[stm1_id].trunk_info[trunk_id].clock_mode = NPU_E1T1_CLOCK_SLAVE;

		//printf("stru_stm1_cfg_info[%d].trunk_info[%d].clock_mode:  %d \n", stm1_id, trunk_id, p->stru_stm1_cfg_info[stm1_id].trunk_info[trunk_id].clock_mode);
	}

	//printf("stru_stm1_cfg_info[%d].trunk_info[%d].clock_mode:  %d \n", stm1_id, trunk_id, p->stru_stm1_cfg_info[stm1_id].trunk_info[trunk_id].clock_mode);

}
static void
cli_t1_port_create_internal(cli_attribute_t *arglist,STRU_SET_INITAIL_CONFIG *np_configure_ptr)
{
	STRU_SET_INITAIL_CONFIG *p = np_configure_ptr;
	u32 stm1_id, trunk_id;

	char clock_mode[10],connect2stm[8] = "";

	get_u32_from_arglist(arglist,"number",&trunk_id);
	{
		assert(0 != trunk_id);
		trunk_id -= 1;
	}

	get_string_from_arglist(arglist,"connected_to",connect2stm);
	{
		if(!(sscanf(connect2stm, "s%u", &stm1_id) == 1))
		{
			WDDI_ERR("error cli_frame_relay_fraction_create_internal from connected_to ");
			return;
		}


		assert(0 != stm1_id);

		stm1_id -= 1;

		if(stm1_id <2)
		{
			//if(stm1_id == g_stm1_id)
            p->stru_stm1_cfg_info[stm1_id].trunk_info[trunk_id].trunkValid = NPU_VALID;
			//else
            //WDDI_ERR("error from connected_to: stm1_id = %d. ",stm1_id);
		}
		else
			WDDI_ERR("error from connected_to: stm1_id = %d. ",stm1_id);
	}
    char opt_mode[32];
	get_string_from_arglist(arglist,"framing", opt_mode);
	{
		if (strcmp((const char *)opt_mode, "esf") == 0)
			p->stru_stm1_cfg_info[stm1_id].trunk_info[trunk_id].Opt_Mode= OPMODE_T1_ESF;
		else if (strcmp((const char *)opt_mode, "d4") == 0)
			p->stru_stm1_cfg_info[stm1_id].trunk_info[trunk_id].Opt_Mode = OPMODE_T1_D4;
	}

	get_enum_from_arglist(arglist,"clock_mode",clock_mode);
	{
		if (strcmp((const char *)clock_mode, "master") == 0)
			p->stru_stm1_cfg_info[stm1_id].trunk_info[trunk_id].clock_mode = NPU_E1T1_CLOCK_MASTER;
		else
			p->stru_stm1_cfg_info[stm1_id].trunk_info[trunk_id].clock_mode = NPU_E1T1_CLOCK_SLAVE;

		//printf("stru_stm1_cfg_info[%d].trunk_info[%d].clock_mode:  %d \n", stm1_id, trunk_id, p->stru_stm1_cfg_info[stm1_id].trunk_info[trunk_id].clock_mode);
	}

	//printf("stru_stm1_cfg_info[%d].trunk_info[%d].clock_mode:  %d \n", stm1_id, trunk_id, p->stru_stm1_cfg_info[stm1_id].trunk_info[trunk_id].clock_mode);

}



/* ===  FUNCTION  ============================================================
 *        @Name:  cli_frame_relay_fraction_create_internal
 * @Description:  verification of the frame relay fraction configuration.
 *       @input:  cli_attribute_t *arglist, argument list.
 *      @Output:  null.
 *      @Return:  void.
 * @Modification:
 *   1. 07/06/2009  exiyyin Fix TR HK69444, add fraction id to the structure.
 * ======================================================================== */
static void cli_frame_relay_fraction_create_internal(cli_attribute_t *arglist, STRU_SET_INITAIL_CONFIG *np_configure_ptr)
{
	STRU_SET_INITAIL_CONFIG *p = np_configure_ptr;

	u16 max_frame_size;
	char timeslot[10],crc_type[10],connect2e1[8] = "",i;
	u32 stm1_id, trunk_id, group_id;

	get_u32_from_arglist(arglist,"fraction_id",&group_id);

	get_string_from_arglist(arglist,"connected_to", connect2e1);
	{
		if(!(sscanf(connect2e1,"s%up%u", &stm1_id, &trunk_id) == 2))
		{
			WDDI_ERR("error cli_frame_relay_fraction_create_internal from connected_to ");
			return;
		}
		assert(stm1_id && trunk_id);

		/* the used stm1&trunk id in npu server start from zero */
		stm1_id -=1;
		trunk_id -=1;

		//if((stm1_id == g_stm1_id) && (trunk_id == g_trunk_id))
        p->stru_stm1_cfg_info[stm1_id].trunk_info[trunk_id].group_info[group_id].groupValid = NPU_VALID;
		//else
        //WDDI_ERR("error from connected_to: stm1_id = %d, trunk_id = %d", stm1_id,trunk_id);
	}
    get_string_from_arglist(arglist,"timeslots", timeslot);
	{
		int slot_start, slot_end;
		char *p_slot = NULL,*p_timeslot = timeslot;
		//printf("####timeslot = %s !\n", *timeslot);
		do
		{
			p_slot = index((const char *)p_timeslot,'-');
			if(NULL != p_slot)
			{
				*(p_slot++) = '\0';
				slot_start = atoi((const char*)p_timeslot);
				slot_end = atoi((const char*)p_slot);
				p->stru_stm1_cfg_info[stm1_id].trunk_info[trunk_id].group_info[group_id].slot_selection = 0; //reset at first
				for(i=slot_start; i<=slot_end;i++)
				{
					p->stru_stm1_cfg_info[stm1_id].trunk_info[trunk_id].group_info[group_id].slot_selection |= 0x1<<i;
				}
				p_timeslot = index((const char *)p_slot,',');
			}
			else
			{
				slot_start = atoi((const char*)p_timeslot);
				p->stru_stm1_cfg_info[stm1_id].trunk_info[trunk_id].group_info[group_id].slot_selection = 0; //reset at first
				p->stru_stm1_cfg_info[stm1_id].trunk_info[trunk_id].group_info[group_id].slot_selection |= 0x1<<slot_start;
                p_timeslot = NULL;
			}
		}while(NULL != p_timeslot++);
		//printf("The stru_stm1_cfg_info[%d].trunk_info[%d].group_info[%d].slot_selection = %x\n", stm1_id, trunk_id, group_id,p->stru_stm1_cfg_info[stm1_id].trunk_info[trunk_id].group_info[group_id].slot_selection);
	}
	get_u16_from_arglist(arglist,"max_frame_size",&max_frame_size);
	{
		p->stru_stm1_cfg_info[stm1_id].trunk_info[trunk_id].group_info[group_id].max_frame_size = max_frame_size;
	}
	get_enum_from_arglist(arglist,"crc_type",crc_type);
	{
		if (strcmp((const char *)crc_type, "crc16") == 0)
			p->stru_stm1_cfg_info[stm1_id].trunk_info[trunk_id].group_info[group_id].crc_type = NPU_E1T1_CRC16;
		else
			p->stru_stm1_cfg_info[stm1_id].trunk_info[trunk_id].group_info[group_id].crc_type = NPU_E1T1_CRC32;
	}
#if 0
	printf("-------- frame_relay_fractionclass ---------\n");
	printf("fraction_id = %d, timeslot = %s, max_frame_size = %d, crc_type = %s\n",group_id,timeslot, max_frame_size, crc_type);

	printf("stru_stm1_cfg_info[%d].trunk_info[%d].group_info[%d].crc_type:  %d \n",stm1_id,trunk_id,group_id, p->stru_stm1_cfg_info[stm1_id].trunk_info[trunk_id].group_info[group_id].crc_type);
	printf("max_frame_size:  %d \n", p->stru_stm1_cfg_info[stm1_id].trunk_info[trunk_id].group_info[group_id].max_frame_size);
	g_stm1_id = stm1_id;
	g_trunk_id = trunk_id;
	g_group_id = group_id;
#endif

}

static void
cli_frame_relay_pvc_create_internal(cli_attribute_t *arglist, STRU_SET_INITAIL_CONFIG *np_configure_ptr)
{
	STRU_SET_INITAIL_CONFIG *p = np_configure_ptr;
	u32 dlci, cir, bc, be;
	u32 stm1_id, trunk_id, group_id;
	char connect2fr[20] = "",i;

	get_string_from_arglist(arglist,"connected_to", connect2fr);
	{
		//printf("connected_to %s \n",connect2fr);
		if(!(sscanf(connect2fr, "s%up%ufr%u",&stm1_id,&trunk_id,&group_id)== 3))
		{
			WDDI_ERR("error cli_frame_relay_pvc_create_internal from connected_to connect2fr = %s\n",connect2fr);
			return;
		}


		assert(stm1_id && trunk_id);

		/* the used stm1&trunk id in npu server start from zero */
		stm1_id -=1;
		trunk_id -=1;
		g_stm1_id = stm1_id;
		g_trunk_id = trunk_id;
		g_group_id = group_id;
	}

	get_u32_from_arglist(arglist,"dlci",&dlci);
	g_dlci = dlci;

	//if((stm1_id == g_stm1_id) && (trunk_id == g_trunk_id) && (group_id == g_group_id))
	{
		get_u32_from_arglist(arglist,"cir",&cir);
		{
			p->stru_stm1_cfg_info[stm1_id].trunk_info[trunk_id].group_info[group_id].PVCQos[dlci].cir = cir;
		}

		get_u32_from_arglist(arglist,"bc",&bc);
		{
			p->stru_stm1_cfg_info[stm1_id].trunk_info[trunk_id].group_info[group_id].PVCQos[dlci].bc = bc;
		}

		get_u32_from_arglist(arglist,"be",&be);
		{
			p->stru_stm1_cfg_info[stm1_id].trunk_info[trunk_id].group_info[group_id].PVCQos[dlci].be = be;
		}

	}
	//else
    //WDDI_ERR("error from connected_to: stm1_id = %d, trunk_id = %d", stm1_id,trunk_id);




}


static void
cli_sgsn_pool_create_internal(cli_attribute_t *arglist, STRU_SET_INITAIL_CONFIG *np_configure_ptr)
{
	u32 nri_size;
	char *p_pool = malloc(100);
	char *p_pool_bak = p_pool;
	/*gloab initial for test */
	STRU_SET_INITAIL_CONFIG *p = np_configure_ptr;
	//p->OpterateMode = NPU_MODE_IP;
	//p->stm1_num = 0;

	get_u32_from_arglist(arglist,"nri_size", &p->NRI_bitLen);

	//printf("p->NRI_bitLen = %d\n", p->NRI_bitLen);
	get_string_from_arglist(arglist,"sgsn_list", p_pool);
	//printf("Test: The sgsn_list is %s.\n", p_pool);

	p->PortPoolSize = 1;

	while(*p_pool != '\0')
	{
		if(*p_pool == ',')
		{
			p->PortPoolSize++;
		}
		p_pool++;
	}

	free(p_pool_bak);
}

int route_num = 0;
static void cli_ip_route_create_internal(cli_attribute_t *arglist,STRU_SET_INITAIL_CONFIG *np_configure_ptr)
{
	//char *p_ip;
	char ipaddr[24], *p_ipaddr;
    char ipmask[24]= ""	;
	/*gloab initial for test */
	STRU_SET_INITAIL_CONFIG *p =np_configure_ptr;
	char interfaceFullName[24];
    char ifname[24] = {0};
	get_string_from_arglist(arglist,"interface", interfaceFullName);
	if(!(sscanf(interfaceFullName,"%[^.]",ifname) == 1) )
	{
        return;
	}
	if(strcmp(ifname,get_ifname()) == 0||strcmp(ifname,"enet0") == 0)
	{
		get_string_from_arglist(arglist,"destination", ipaddr);
		{
			//strcpy((char *)ipaddr, (const char *)p_ip);
			p_ipaddr = index((const char *)ipaddr, '/');
			/*calc netmask*/
			u8 IpMaskValue;/*1~32*/
			if(p_ipaddr != NULL)
			{
				*p_ipaddr = '\0';
				IpMaskValue = atoi((const char *)++p_ipaddr);
			}
			else
			{
				printf("can't find mask,default 32\n");
				IpMaskValue = 32;
			}
			int i;
			u32 Mask = 0;
			for(i =0 ;i < IpMaskValue;i++)
			{
				Mask |= 1<<(31-i);
			}
			sprintf(ipmask,"%d.%d.%d.%d",Mask>>24&0xff,Mask>>16&0xff,Mask>>8&0xff,Mask&0xff);
			//printf("gw ipmask = %s \n",ipmask);
			inet_pton(AF_INET, (const char *)ipaddr , &p->routeInfo[route_num].IpDst);
		    inet_pton(AF_INET, (const char *)ipmask , &p->routeInfo[route_num].mask);
			p->routeInfo[route_num].IpDst = ntohl(p->routeInfo[route_num].IpDst);
			p->routeInfo[route_num].mask= ntohl(p->routeInfo[route_num].mask);
		}
		//printf("p->routeInfo[%d].IpDst:  %x \n", route_num, p->routeInfo[route_num].IpDst);


		memset(ipaddr, 0, sizeof(ipaddr));
		get_string_from_arglist(arglist,"gateway", ipaddr);
		{
			//strcpy((char *)ipaddr, (const char *)p_ip);
			inet_pton(AF_INET, (const char *)ipaddr , &p->routeInfo[route_num].GatewayIP);
			p->routeInfo[route_num].GatewayIP = ntohl(p->routeInfo[route_num].GatewayIP);
		}
		//printf("p->routeInfo[%d].GatewayIP:  %x \n", route_num, p->routeInfo[route_num].GatewayIP);

		route_num++;
	}


}


static void cli_vlan_endpoint_create_internal(cli_attribute_t *arglist,STRU_SET_INITAIL_CONFIG *np_configure_ptr)
{
	STRU_SET_INITAIL_CONFIG *p = np_configure_ptr;
    char connectTo[24];
	get_string_from_arglist(arglist,"connected_to", connectTo);
	if(strcmp(connectTo,get_ifname()) == 0 || strcmp(connectTo,"enet0") == 0)
	{
		get_u32_from_arglist(arglist,"id", &p->VlanTag);
		if(NPU_VLAN_DISABLE == p->VlanFlag)
			p->VlanFlag = NPU_VLAN_ENABLE;
	}

}
static void cli_ip_create_internal(cli_attribute_t *arglist,STRU_SET_INITAIL_CONFIG *np_configure_ptr)
{
	char ipaddr[24], connectTo[24], ipmask[24]= "255.255.255.", *p_ipaddr;
	STRU_SET_INITAIL_CONFIG *p = np_configure_ptr;
	get_string_from_arglist(arglist,"address", ipaddr);
	get_string_from_arglist(arglist,"connected_to", connectTo);
	char ifname[24] = {0};
	if(!(sscanf(connectTo,"%[^.]",ifname) == 1) )
	{
	    WDDI_ERR("ifname is error !\n",ifname);
        return;
	}
	if(strcmp(ifname,get_ifname()) == 0 || strcmp(ifname,"enet0") == 0)
	{
        if (strcmp(ifname, get_ifname()) == 0)
        { 
	        STRU_SET_INITAIL_CONFIG *p = np_configure_ptr;
	        p->OpterateMode = NPU_MODE_FR;
        }
        p_ipaddr = index((const char *)ipaddr, '/');
        /*calc netmask*/
        u8 IpMaskValue;/*1~32*/
        if(p_ipaddr != NULL)
        {
            *p_ipaddr = '\0';
            IpMaskValue = atoi((const char *)++p_ipaddr);
        }
        else
        {
            printf("can't find mask,default 32\n");
            IpMaskValue = 32;
        }
        int i;
        u32 Mask = 0;
        for(i =0 ;i < IpMaskValue;i++)
        {
            Mask |= 1<<(31-i);
        }
        sprintf(ipmask,"%d.%d.%d.%d",Mask>>24&0xff,Mask>>16&0xff,Mask>>8&0xff,Mask&0xff);
        //printf("ipmask = %s \n",ipmask);
        //strcat((char *)ipmask, (const char *)++p_ipaddr);

        inet_pton(AF_INET, (const char *)ipaddr , &p->ifIpaddr);
        inet_pton(AF_INET, (const char *)ipmask , &p->ifIpNetMask);
        p->ifIpaddr = ntohl(p->ifIpaddr);
        p->ifIpNetMask = ntohl(p->ifIpNetMask);
        //printf("ifIpaddr:  %x \n", p->ifIpaddr);
        //printf("ifIpNetMask:  %x \n", p->ifIpNetMask);
	}

}

#if 1

static void cli_stm1_interface_create_internal(cli_attribute_t *arglist,STRU_SET_INITAIL_CONFIG *np_configure_ptr)
{
	char opt_mode[32],fr_type[32];
	u32 stm1_id;
	STRU_SET_INITAIL_CONFIG *p = np_configure_ptr;

	p->OpterateMode = NPU_MODE_FR;

	//if(NPU_VLAN_ENABLE != p->VlanFlag)
	//p->VlanFlag = NPU_VLAN_ENABLE;
	p->stm1_num++;

	get_u32_from_arglist(arglist,"interface_id", &stm1_id);
	{
		assert(0 != stm1_id);

		stm1_id -=1;
		/* only two vaild id of stm1: [0,1] */
		if(stm1_id < 2)
            p->stru_stm1_cfg_info[stm1_id].stm1Valid = NPU_VALID;
		g_stm1_id = stm1_id;
	}

#if 0
	get_string_from_arglist(arglist,"operate_mode", opt_mode);
	{
		if (strcmp((const char *)opt_mode, "d4") == 0)
			p->stru_stm1_cfg_info[stm1_id].Opt_Mode = OPMODE_T1_D4;
		else if (strcmp((const char *)opt_mode, "t1_esf") == 0)
			p->stru_stm1_cfg_info[stm1_id].Opt_Mode = OPMODE_T1_ESF;
		else if (strcmp((const char *)opt_mode, "e1_crc4") == 0)
			p->stru_stm1_cfg_info[stm1_id].Opt_Mode = OPMODE_E1_TRUE_CRC4;
		else if (strcmp((const char *)opt_mode, "false") == 0)
			p->stru_stm1_cfg_info[stm1_id].Opt_Mode = OPMODE_E1_FALSE;
	}
#endif

	get_string_from_arglist(arglist,"fr_type", fr_type);
	{
		if (strcmp((const char *)fr_type, "e1") == 0)
			p->stru_stm1_cfg_info[stm1_id].E1T1type = NPU_MODE_E1;
		else if (strcmp((const char *)fr_type, "t1") == 0)
            p->stru_stm1_cfg_info[stm1_id].E1T1type = NPU_MODE_T1;
	}

	//printf("STRU_STM1_CFG_INFO.E1T1type:  %d \n", p->stru_stm1_cfg_info[stm1_id].E1T1type);
	//printf("stru_stm1_cfg_info[%d].Opt_Mode:  %d \n", stm1_id, p->stru_stm1_cfg_info[stm1_id].Opt_Mode);


}
#endif
static cli_attribute_t*
get_attribute_from_arglist(cli_attribute_t *arglist,const char* attr_name)
{
    int arglist_counter = 0;
    while ( arglist[arglist_counter].name != NULL)
    {
        if (  ! strcmp(arglist[arglist_counter].name,attr_name) )
            return &arglist[arglist_counter];
        arglist_counter++;
    }
    return (cli_attribute_t*)NULL;
}

static int get_u32_from_arglist(cli_attribute_t *arglist,char* attr_name, u32 * integer)
{
    cli_attribute_t* attribute;
    if ( (attribute = get_attribute_from_arglist(arglist,attr_name)) )
    {
        char* eptr;
		//printf("the %s value = %s!\n", attr_name,attribute->value);
        *integer = strtol(attribute->value, &eptr, 10);
        if (*eptr != '\0')
        {
            //sprintf(cli_error_str,"Can't convert argument = %s from arglist\n",attr_name);
			WDDI_ERR("Can't convert argument = %s from arglist,end charter = %c",attr_name,*eptr);
            return -1;
        }
    }
    else
    {
        //sprintf(cli_error_str,"Can't find argument = %s from arglist\n",attr_name);
		WDDI_ERR("Can't find argument = %s from arglist",attr_name);
        return -1;
    }
    return 0;
}
static int get_u16_from_arglist(cli_attribute_t *arglist,char* attr_name, u16 * integer)
{
    cli_attribute_t* attribute;
    if ( (attribute = get_attribute_from_arglist(arglist,attr_name)) )
    {
        char* eptr;
        *integer = strtol(attribute->value, &eptr, 10);
        if (*eptr != '\0')
        {
            sprintf(cli_error_str,"Can't convert argument = %s from arglist\n",attr_name);
			WDDI_ERR("Can't convert argument = %s from arglist",attr_name);
            return -1;
        }
    }
    else
    {
        //sprintf(cli_error_str,"Can't find argument = %s from arglist\n",attr_name);
		WDDI_ERR("Can't find argument = %s from arglist",attr_name);
        return -1;
    }
    return 0;
}
static int get_u8_from_arglist(cli_attribute_t *arglist,char* attr_name, u8 * integer)
{
    cli_attribute_t* attribute;
    if ( (attribute = get_attribute_from_arglist(arglist,attr_name)) )
    {
        char* eptr;
        *integer = strtol(attribute->value, &eptr, 10);
        if (*eptr != '\0')
        {
            //sprintf(cli_error_str,"Can't convert argument = %s from arglist\n",attr_name);
			WDDI_ERR("Can't convert argument = %s from arglist",attr_name);
            return -1;
        }
    }
    else
    {
        //sprintf(cli_error_str,"Can't find argument = %s from arglist\n",attr_name);
		WDDI_ERR("Can't find argument = %s from arglist",attr_name);
        return -1;
    }
    return 0;
}

static int get_enum_from_arglist(cli_attribute_t *arglist,char* attr_name, char string[])
{
    cli_attribute_t* attribute;
    if ( (attribute = get_attribute_from_arglist(arglist,attr_name)) )
    {
    	//printf("The %s value =  %s\n", attr_name,attribute->value);
        strcpy(string, attribute->value);
    }
    else
    {
        //sprintf(cli_error_str,"Can't find argument = %s from arglist\n",attr_name);
		WDDI_ERR("Can't find argument = %s from arglist",attr_name);
        return -1;
    }
    return 0;
}

static int get_string_from_arglist(cli_attribute_t *arglist,char* attr_name, char* string)
{
    cli_attribute_t* attribute;
    if ( (attribute = get_attribute_from_arglist(arglist,attr_name)) )
    {
    	//printf("The get string -- %s = %s !\n",attr_name,attribute->value);
        strcpy(string, attribute->value);
    }
    else
        //sprintf(cli_error_str,"Can't find argument = %s from arglist\n",attr_name);
		WDDI_ERR("Can't find argument = %s from arglist",attr_name);
    return 0;
}

static int get_ip_address_from_arglist(cli_attribute_t *arglist,char* attr_name, ip_address_t * ip_address)
{
    cli_attribute_t* attribute;
    if ( (attribute = get_attribute_from_arglist(arglist,attr_name)) )
    {
#if 0
        int ret = validate_ip_address_format(attribute->value);
        if(!ret)
        {
            fprintf(stderr,"\n error = { reason = 'the ip format(%s) for %s of sgsn_pool_proxy_service_commom is incorrect'}\n",attribute->value,attr_name);
            return -1;
        }
#endif



        int ip;
        ip = inet_addr(attribute->value);
        if (ip == -1)
        {
            sprintf(cli_error_str,"Can't convert argument = %s from arglist\n",attr_name);
            return -1;
        }
        *ip_address = ip;
        return 0;
    }
    else
    {
        sprintf(cli_error_str,"Can't find argument = %s from arglist\n",attr_name);
        return -1;
    }
    return 0;
}

static int
get_attribute(char* parameter,cli_attribute_t *attributes,cli_attribute_t *attribute)
{
    int attribute_counter = 0;

    while (1)
    {
        *attribute =  attributes[attribute_counter];
        if ( (*attribute).name == NULL)
        {
            //sprintf(cli_error_str,"Parameter %s is not a valid parameter  %d\n",parameter,attribute_counter);
			WDDI_ERR("Parameter %s is not a valid parameter  %d",parameter,attribute_counter);
            return FALSE;// Attributes not found
        }
        if (!strcmp(parameter, (*attribute).name))
        {
            return TRUE;
        }
        attribute_counter++;
    };
}

static int
get_arglist(int argc,char** argv,cli_attribute_t *arglist,cli_method_t *method,cli_attribute_t *attributes)
{
    int arglist_counter = ATTRIBUTE_POS;
    int argv_counter = ATTRIBUTE_POS;
    int ret_val = TRUE;
	//printf("The argc = %d ! \n",argc);
    // Initialize arglist names because name is used as end mark
    int i = 0;
    for ( i = 0 ; i < ARGLIST_BUFLEN; i++)
    {
        arglist[i].name = NULL;
    }
    arglist[PROGNAME_POS].name = argv[PROGNAME_POS];
    arglist[CLASS_POS].name = argv[CLASS_POS];
    arglist[METHOD_POS].name = argv[METHOD_POS];
    char parameters[256];
    if ( argc == METHOD_POS+1 ) // Method without parameters supplied
    { //Put all optional and mandatory parameters in cli_completion_str
        return TRUE;
    }
    if ( attributes == NULL )
    {
        //sprintf(cli_error_str,"No parameters for method: %s\n",arglist[METHOD_POS].name);
		WDDI_ERR("No parameters for method: %s",arglist[METHOD_POS].name);
        return FALSE;
    }

#if 0
	if(attributes[1].name == NULL)
	    printf("The class_attribute_name = NULL!\n");
	else
		printf("The class_attribute_name = %s!\n",attributes[0].name);
#endif

//	class_num++;  //used to check if it's the first class in reading

    while ( 1 )
    {
    	//printf("The receive pram = %s, argv_counter = %d! \n", argv[argv_counter],argv_counter);
        ret_val = get_attribute(argv[argv_counter],attributes,&arglist[arglist_counter]);
        if ( ret_val != TRUE ) // parameter in argv not recognised among available ones
        {
        	/*
              list_parameters_not_present(arglist,method->mandatory_arguments,parameters);
              word_append(cli_completion_str,parameters);
              list_parameters_not_present(arglist,method->optional_arguments,parameters);
              word_append(cli_completion_str,parameters);
            */
            //return     ret_val;
        }
#if 1
        switch ( arglist[arglist_counter].type )
        {
        case  CLI_OPTION_U64:
            if (argv_counter < argc -1 )
            {
                //printf("\n SW type = CLI_OPTION_U64\n");
                argv_counter++;
                //printf("the type value = %s!\n", argv[argv_counter]);
                //ret_val = validate_u64(argv[argv_counter], &arglist[arglist_counter]);
            }
            else // argv_counter == argc
            {
                if ( arglist[arglist_counter].completion_handler != NULL)
                {
                    //sprintf(cli_error_str,"Valid options for %s:\n",arglist[arglist_counter].name);
                    WDDI_ERR("Valid options for %s:",arglist[arglist_counter].name);
                    (*arglist[arglist_counter].completion_handler)(arglist);
                    ret_val = FALSE;
                }
                else
                    //sprintf(cli_error_str,"%s needs an argument!\n",arglist[arglist_counter].name);
                    WDDI_ERR("%s needs an argument!",arglist[arglist_counter].name);
                ret_val = FALSE;
            }
            break;
        case CLI_OPTION_STRING:
            if (argv_counter < argc -1)
            {
                //printf("\n SW type = CLI_OPTION_STRING\n");
                argv_counter++;
                //printf("the type value = %s!\n", argv[argv_counter]);
                //ret_val = validate_string(argv[argv_counter], &arglist[arglist_counter]);
            }
            else
            {
                if ( arglist[arglist_counter].completion_handler != NULL)
                {
                    //sprintf(cli_error_str,"Valid options for %s:\n",arglist[arglist_counter].name);
                    WDDI_ERR("Valid options for %s:",arglist[arglist_counter].name);
                    (*arglist[arglist_counter].completion_handler)(arglist);
                    ret_val = FALSE;
                }
                else
                    //sprintf(cli_error_str,"%s needs an argument!\n",arglist[arglist_counter].name);
                    WDDI_ERR("%s needs an argument!",arglist[arglist_counter].name);
                ret_val = FALSE;
            }
            break;
        case CLI_OPTION_ENUM:
            if (argv_counter < argc -1)
            {
                argv_counter++;
                //ret_val = validate_enum(argv[argv_counter], &arglist[arglist_counter]);
            }
            else
            {
                if ( arglist[arglist_counter].completion_handler != NULL)
                {
                    //sprintf(cli_error_str,"Valid options for %s:\n",arglist[arglist_counter].name);
                    WDDI_ERR("Valid options for %s:",arglist[arglist_counter].name);
                    (*arglist[arglist_counter].completion_handler)(arglist);
                    ret_val = FALSE;
                }
                else
                    //sprintf(cli_error_str,"%s needs an argument!\n",arglist[arglist_counter].name);
                    WDDI_ERR("%s needs an argument!",arglist[arglist_counter].name);
                ret_val = FALSE;
            }
            break;
        case CLI_OPTION_INT_LIST:
            if (argv_counter < argc -1)
            {
                argv_counter++;
                //ret_val = validate_int_list(argv[argv_counter], &arglist[arglist_counter]);
            }
            else
            {
                if ( arglist[arglist_counter].completion_handler != NULL)
                {
                    //sprintf(cli_error_str,"Valid options for %s:\n",arglist[arglist_counter].name);
                    WDDI_ERR("Valid options for %s:",arglist[arglist_counter].name);
                    (*arglist[arglist_counter].completion_handler)(arglist);
                    ret_val = FALSE;
                }
                else
                    //sprintf(cli_error_str,"%s needs an argument!\n",arglist[arglist_counter].name);
                    WDDI_ERR("%s needs an argument!",arglist[arglist_counter].name);
                ret_val = FALSE;
            }
            break;
        case  CLI_OPTION_IPV4_ADDRESS:
            if (argv_counter < argc -1 )
            {
                argv_counter++;
                //ret_val = validate_ip_address(argv[argv_counter], &arglist[arglist_counter]);
            }
            else // argv_counter == argc
            {
                if ( arglist[arglist_counter].completion_handler != NULL)
                {
                    //sprintf(cli_error_str,"Valid options for %s:\n",arglist[arglist_counter].name);
                    WDDI_ERR("Valid options for %s:",arglist[arglist_counter].name);
                    (*arglist[arglist_counter].completion_handler)(arglist);
                    ret_val = FALSE;
                }
                else
                    //sprintf(cli_error_str,"%s needs an argument!\n",arglist[arglist_counter].name);
                    WDDI_ERR("%s needs an argument!",arglist[arglist_counter].name);
                ret_val = FALSE;
            }
            break;
        case  CLI_OPTION_FLOAT:
            if (argv_counter < argc -1 )
            {
                argv_counter++;
                // ret_val = validate_float(argv[argv_counter], &arglist[arglist_counter]);
            }
            else // argv_counter == argc
            {
                if ( arglist[arglist_counter].completion_handler != NULL)
                {
                    //sprintf(cli_error_str,"Valid options for %s:\n",arglist[arglist_counter].name);
                    WDDI_ERR("Valid options for %s:",arglist[arglist_counter].name);
                    (*arglist[arglist_counter].completion_handler)(arglist);
                    ret_val = FALSE;
                }
                else
                    //sprintf(cli_error_str,"%s needs an argument!\n",arglist[arglist_counter].name);
                    WDDI_ERR("%s needs an argument!",arglist[arglist_counter].name);
                ret_val = FALSE;
            }
            break;
        case  CLI_OPTION_IP_ENDPOINT_ADDRESS:
            if (argv_counter < argc -1 )
            {
                argv_counter++;
                //ret_val = validate_ip_endpoint_address(argv[argv_counter], &arglist[arglist_counter]);
            }
            else // argv_counter == argc
            {
                if ( arglist[arglist_counter].completion_handler != NULL)
                {
                    //sprintf(cli_error_str,"Valid options for %s:\n",arglist[arglist_counter].name);
                    WDDI_ERR("Valid options for %s:",arglist[arglist_counter].name);
                    (*arglist[arglist_counter].completion_handler)(arglist);
                    ret_val = FALSE;
                }
                else
                    //sprintf(cli_error_str,"%s needs an argument!\n",arglist[arglist_counter].name);
                    WDDI_ERR("%s needs an argument!",arglist[arglist_counter].name);
                ret_val = FALSE;
            }
            break;
        case  CLI_OPTION_STRING_LIST:
            if (argv_counter < argc -1 )
            {
                argv_counter++;
                //ret_val = validate_string_list(argv[argv_counter], &arglist[arglist_counter]);
            }
            else // argv_counter == argc
            {
                if ( arglist[arglist_counter].completion_handler != NULL)
                {
                    //sprintf(cli_error_str,"Valid options for %s:\n",arglist[arglist_counter].name);
                    WDDI_ERR("Valid options for %s:",arglist[arglist_counter].name);
                    (*arglist[arglist_counter].completion_handler)(arglist);
                    ret_val = FALSE;
                }
                else
                    //sprintf(cli_error_str,"%s needs an argument!\n",arglist[arglist_counter].name);
                    WDDI_ERR("%s needs an argument!",arglist[arglist_counter].name);
                ret_val = FALSE;
            }
            break;
        case CLI_OPTION_BITMAP:
            if (argv_counter < argc -1)
            {
                argv_counter++;
                //ret_val = validate_bitmap(argv[argv_counter], &arglist[arglist_counter]);
            }
            else
            {
                if ( arglist[arglist_counter].completion_handler != NULL)
                {
                    //sprintf(cli_error_str,"Valid options for %s:\n",arglist[arglist_counter].name);
                    WDDI_ERR("Valid options for %s:",arglist[arglist_counter].name);
                    (*arglist[arglist_counter].completion_handler)(arglist);
                    ret_val = FALSE;
                }
                else
                    //sprintf(cli_error_str,"%s needs an argument!\n",arglist[arglist_counter].name);
                    WDDI_ERR("%s needs an argument!",arglist[arglist_counter].name);
                ret_val = FALSE;
            }
            break;
        case CLI_OPTION_DNS_NAME:
            if (argv_counter < argc -1)
            {
                //printf("\n SW type = CLI_OPTION_STRING\n");
                argv_counter++;
                //printf("the type value = %s!\n", argv[argv_counter]);

                // ret_val = validate_dns_name(argv[argv_counter], &arglist[arglist_counter]);
            }
            else
            {
                if ( arglist[arglist_counter].completion_handler != NULL)
                {
                    //sprintf(cli_error_str,"Valid options for %s:\n",arglist[arglist_counter].name);
                    WDDI_ERR("Valid options for %s:",arglist[arglist_counter].name);
                    (*arglist[arglist_counter].completion_handler)(arglist);
                    ret_val = FALSE;
                }
                else
                    //sprintf(cli_error_str,"%s needs an argument!\n",arglist[arglist_counter].name);
                    WDDI_ERR("%s needs an argument!",arglist[arglist_counter].name);
                ret_val = FALSE;
            }
            break;
        case CLI_OPTION_NAME_ONLY:
            argv_counter++;
            ret_val = TRUE;
            break;
        default:
            //sprintf(cli_error_str,"Invalid parameter %s of type: %d\n",arglist[arglist_counter].name,arglist[arglist_counter].type);
            WDDI_ERR("Invalid parameter %s of type: %d",arglist[arglist_counter].name,arglist[arglist_counter].type);
            ret_val =  FALSE;
        }
#endif

		//printf("the first value = %s!\n", argv[argv_counter]+8);
		//strcpy(arglist[arglist_counter].value,argv[argv_counter]+8); //Assign value

		//printf("the value = %s!\n", argv[argv_counter]);
		strcpy(arglist[arglist_counter].value,argv[argv_counter]); //Assign value


        arglist_counter++;
        argv_counter++;
        if ( ret_val != TRUE )
        {
        	//printf("Test: read all!\n");
            arglist[arglist_counter].name = NULL;
            return     ret_val;
        }
        if ( argv_counter >= argc ) //Everything seems OK
        { //Add completion info if available to satisfy bash completion
#if 0
            if ( is_cursor_beyond_parameter() != TRUE )
            {
                if ( arglist[arglist_counter-1].completion_handler != NULL )
                    (*arglist[arglist_counter-1].completion_handler)(arglist);
            }
#endif
			//printf("Get arglist success!\n");
            arglist[arglist_counter].name = NULL;
            return ret_val;
        }
    }


    // fixme: add check for whether extraneous parameters are entered

}

static int
is_class(const char* name)
{
    cli_class_t class;
    int class_index = -1;
    if (name == NULL )
        return FALSE;
    while (1)
    {
        class = class_table[++class_index];
        if (class.name == NULL )
            return FALSE;
        if (strcmp(class.name,name))
            continue;
        return TRUE;
    }
}


static int
cli_find_method(int argc, char** argv,cli_method_t *method_table, cli_method_t *method)
{
    char* method_name = argv[METHOD_POS];
    cli_method_t method1;
    int method_index = -1;
    if (method_name == NULL )
    {
        sprintf(cli_error_str,"Available methods:\n");
        //cli_show_methods(method_table);
        return FALSE;
    }
    while (1)
    {
        method1 = method_table[++method_index];

        if (method1.name == NULL )
        {
            //sprintf(cli_error_str,"Method name %s not found!\nAvailable methods:\n",method_name);
			WDDI_ERR(cli_error_str,"Method name %s not found!\nAvailable methods:",method_name);
            //cli_show_methods(method_table);
            return FALSE;
        }
        if (strcmp(method1.name,method_name))
        {
            continue;
        }
        *method = method1;
		//printf("Get method_name = %s\n", method_name);
        return TRUE;
    }
}


static int
cli_find_class(int argc_local, char** argv_local, cli_class_t *class)
{
    char* class_name = argv_local[CLASS_POS];
    cli_class_t class1;
    int class_index = -1;
    if (class_name == NULL )
    {
        sprintf(cli_error_str,"Avaliable classes:\n");
        //cli_configuration_show_classes();
        return FALSE;
    }
    while (1)
    {
        class1 = class_table[++class_index];
        if (class1.name == NULL )
        {
            sprintf(cli_error_str,"Available classes:\n");
            //cli_configuration_show_classes();
            return FALSE;// class name not among available ones
        }
        if (strcmp(class1.name,class_name))
        {
            continue;//still not found
        }
        *class = class1;//found class
        //printf("\n\nGet class_name = %s\n",class_name);
        return TRUE;
    }
}

#ifndef TEST
#define TEST
#endif

#ifdef TEST
//print out the command line generated from XML to be fed into CLI excution
static void
test_print_xml_parse(expat_data_t *expat_data, const char *element)
{
    char** argv_local = expat_data->argv_local;
    int i = 0;
    printf("\nelement=%s\n",element);
    while ( i <  expat_data->argc_local )
    {
        printf("%s ",argv_local[i]);
        i++;
    }
//    printf("\nelement=%s\n%s %s %s %s %s %s %s\n",element,argv_local[0],argv_local[1],
//        argv_local[2],argv_local[3],argv_local[4],argv_local[5],argv_local[6]);
    printf("\n");
}
#endif


void
server_start_handler(void *data, const char *element, const char **attribute) //expat xml start tag handler
{
    expat_data_t *expat_data;
    expat_data  = (expat_data_t*) data;

    if ( expat_data->inside_class == FALSE && expat_data->inside_attribute == FALSE ) //could be a new class
    {
        if ( is_class(element) )
        {
            expat_data->inside_class = TRUE;
            int i;
            for (i=0 ; i < ARGLIST_BUFLEN ; i++)
            {// null terminate each argument in vector
                *expat_data->argv_local[i] = 0;
            }
            strcpy(expat_data->argv_local[CLASS_POS],element);
            strcpy(expat_data->argv_local[METHOD_POS],"create_internal");
            expat_data->argc_local = METHOD_POS+1;

        }
        else
        { // Print error for classes not recognised. Note all attributes will be flagged as error
            if (  !(strcmp(element,"configuration_fragment") == 0 || strcmp(element,"model_version") == 0) )
            {
                //parse_error = -1; //new mask, no need to use since in spp_npu we just focus on initial paramters!
                /* fprintf(stderr,"Invalid element=%s in file: %s before line=%ld\n",element,
                   filename_current, XML_GetCurrentLineNumber(expat_data->parser));    */
                //printf("========================error =============!\n");
                //parse_error = -1;
            }
        }
        return;
    }
    if ( expat_data->inside_class == TRUE && expat_data->inside_attribute == FALSE ) //Assume it is a new attribute
    {
        if ( expat_data->argc_local >= ARGLIST_BUFLEN)
        {
            fprintf(stderr,"\nXML element overflow in file: %s before line=%ld\nPossibly a missing end tag?\n",
                    filename_current, XML_GetCurrentLineNumber(expat_data->parser));
            expat_data->argc_local = METHOD_POS+1; // force counter back to prevent indexing out of bounds
            parse_error = -1;
            return;
        }
        expat_data->inside_attribute = TRUE;
        //printf("The element value = %s! \n", element);
        strcpy(expat_data->argv_local[expat_data->argc_local],element);
        expat_data->argc_local++;
    }
    return;

}  /* End of expat start handler */

void
server_data_handler(void *handler_data, const char *xml_data, int length) {
    expat_data_t *expat_data;
    expat_data = (expat_data_t*)handler_data;
    if ( length > ARGV_MAX_CHARACTERS )
    {
        parse_error = -1;
        fprintf(stderr,"\nXML data overflow in file: %s before line=%ld\nDon't allow XML data to larger than %u characters\n",
                filename_current, XML_GetCurrentLineNumber(expat_data->parser), ARGV_MAX_CHARACTERS);
        return;
    }

    if (expat_data->inside_attribute == TRUE && xml_data != NULL ) //Should be the value of an attribute
    {
        if ( expat_data->argc_local >= ARGLIST_BUFLEN)
        {
            parse_error = -1;
            fprintf(stderr,"\nXML element overflow in file: %s before line=%ld\nPossibly a missing end tag?\n",
                    filename_current, XML_GetCurrentLineNumber(expat_data->parser));
            expat_data->argc_local = METHOD_POS+1; // force counter back to prevent indexing out of bounds
            return;
        }
	    //data may be divided into two parts by xml parser buffer with limited size.

	    //printf("\n************** The attribute value = %s! length = %d **************\n", xml_data,length);
	    //memset(expat_data->argv_local[expat_data->argc_local],'\0', length+1);  //new added, for first class read error!

        strncat(expat_data->argv_local[expat_data->argc_local],xml_data,length);

		//strncpy(expat_data->argv_local[expat_data->argc_local],xml_data,length);

    }
} /* End of data handler */


void
server_end_handler(void *data, const char *element)
{
    expat_data_t *expat_data;
    expat_data = (expat_data_t*)data;
    char **argv_local=expat_data->argv_local;
    int argc_local = expat_data->argc_local;

    if ( expat_data->inside_class == TRUE && expat_data->inside_attribute == FALSE ) //should be end of class
    {
        if ( is_class(element) )
        {
            expat_data->inside_class = FALSE;
//#if TEST
            //test_print_xml_parse(expat_data, "Calling CLI with string:");
//#endif
            cli_class_t class;
            cli_method_t method;
            cli_attribute_t arglist[ARGLIST_BUFLEN];
            //Reset these to empty
            strcpy(cli_error_str,"");
            strcpy(cli_completion_str,"");

            if ( cli_find_class(argc_local,argv_local,&class) &&
                 cli_find_method(argc_local,argv_local,class.methods,&method) &&
                 get_arglist(argc_local,argv_local,arglist,&method,class.attributes)
                )
            {
                //Defaults not working yet
                //        add_defaults_to_arglist(arglist,method.optional_arguments,class.attributes);
#if 0
                print_arglist(arglist);
#endif
				//expat_data->np_configure_ptr = &g_npinitial;
                (*method.method)(arglist,expat_data->np_configure_ptr);
                int i;
                for (i=0 ; i < ARGLIST_BUFLEN ; i++)
                {// null terminate each argument in vector
                    *argv_local[i] = 0;
                }
            }
            else
            {
                parse_error = -1;
                fprintf(stderr,"\nError in file: %s before line=%ld",filename_current, XML_GetCurrentLineNumber(expat_data->parser));
                fprintf(stderr,cli_error_str);
                fprintf(stderr,cli_completion_str);
                fprintf(stderr,"\n");
            }
        }
        return;
    }
    if ( expat_data->inside_class == TRUE && expat_data->inside_attribute == TRUE) { //should be end of attribute
        expat_data->inside_attribute = FALSE;
        expat_data->argc_local++;
    }
    return;

}  /* End of end handler */

static void
free_argv_local(char** argv_local)
{    // free argv_local
    int i;
    for (i = 0; i < ARGLIST_BUFLEN; i++ )
    {
        if ( argv_local[i] != NULL )
        {
            free (argv_local[i]);
            argv_local[i] = NULL;
        }
    }
    if ( argv_local != NULL )
    {
        free(argv_local);
        argv_local = NULL;
    }
}
int
expat_parser(char* filename,STRU_SET_INITAIL_CONFIG *np_configure_ptr)
{

    expat_data_t expat_data;
    expat_data.inside_class = 0;
    expat_data.inside_attribute = 0;
    expat_data.argc_local = PROGNAME_POS;

    current_filename = filename;// Save filename globally

    expat_data.argv_local = (char**) malloc( (ARGLIST_BUFLEN+1) * sizeof(char*));
    if ( expat_data.argv_local == NULL ) return -1;
    int i;
    for (i = 0; i < ARGLIST_BUFLEN; i++ )
    {
        expat_data.argv_local[i] = malloc( (ARGV_MAX_CHARACTERS+1)*sizeof(char));
        if ( expat_data.argv_local[i] == NULL ) return -1;
    }
    strcpy(expat_data.argv_local[PROGNAME_POS],"spp");

    strcpy(filename_current,filename);

    XML_Parser parser = XML_ParserCreate(NULL);
    expat_data.parser = parser;
    expat_data.np_configure_ptr = np_configure_ptr; //new added

    if (! parser ) {
        fprintf(stderr, "error {reason='Couldn't allocate memory for parser',filename=%s}\n",filename);
        free_argv_local(expat_data.argv_local);
        return -1;
    }

    XML_SetElementHandler(parser, server_start_handler, server_end_handler);
    XML_SetCharacterDataHandler(parser, server_data_handler);
    XML_SetUserData(parser,(void*)&expat_data);

    FILE *xml_file;
    if ( (xml_file = fopen(filename,"r") ) == NULL )
    {
        fprintf(stderr,"error {reason='error opening file',filename=%s}\n",filename);
        free_argv_local(expat_data.argv_local);
        return -1;
    }

    int done = 0;
    int len = 0;

    char buff[XML_PARSER_BUFFER_SIZE + 1] = "";

    for (;;) {
        memset(buff, '\0',  XML_PARSER_BUFFER_SIZE + 1);
        len = fread(buff, 1, XML_PARSER_BUFFER_SIZE, xml_file);
        if (ferror(xml_file)) {
            fprintf(stderr,"error {reason='error reading file',filename=%s}\n",filename);
            XML_ParserFree(parser);
            free_argv_local(expat_data.argv_local);
            fclose(xml_file);
            return -1;
        }
        done = feof(xml_file);

        if (! XML_Parse(parser, buff, len, done)) {
            fprintf(stderr, "error {reason='error parsing file',filename=%s, line=%ld, specific_reason=%s}\n",
                    filename,
                    XML_GetCurrentLineNumber(parser),
                    XML_ErrorString(XML_GetErrorCode(parser)));
            XML_ParserFree(parser);
            free_argv_local(expat_data.argv_local);
            fclose(xml_file);
            return -1;
        }
        if (done)
            break;
    }

    free_argv_local(expat_data.argv_local);
    XML_ParserFree(parser);
    fclose(xml_file);

#if 1
    //p->ifIpaddr = 0x0aaa21c1;/*10.170.33.193*/
    //p->ifIpNetMask = 0xffffff80; /*255.255.255.128*/
    //p->VlanFlag = NPU_VLAN_ENABLE;
    np_configure_ptr->ifMACsrc[0] = 0x00;
    np_configure_ptr->ifMACsrc[1] = 0x01;
    np_configure_ptr->ifMACsrc[2] = 0x02;
    np_configure_ptr->ifMACsrc[3] = 0x03;
    np_configure_ptr->ifMACsrc[4] = 0x04;
    np_configure_ptr->ifMACsrc[5] = 0x05;
#endif

#if 0
    //route_num--;
    printf("------------- STRU_SET_INITAIL_CONFIG \n");
    printf("stm1_id = %d, trunk_id = %d, group_id = %d,\n", g_stm1_id,g_trunk_id, g_group_id);
    printf("VlanFlag:  %d \n", np_configure_ptr->VlanFlag);
    printf("VlanTag:	%d \n", np_configure_ptr->VlanTag);
    printf("OpterateMode:  %d \n", np_configure_ptr->OpterateMode);
    for(i = 0;i<6;i++){
        printf("ifMACsrc[%d]:  %x \n", i,np_configure_ptr->ifMACsrc[i]);
    }
    printf("ifIpaddr:  %x \n", np_configure_ptr->ifIpaddr);
    printf("ifIpNetMask:	%x \n", np_configure_ptr->ifIpNetMask);
    printf("stm1_num:  %d \n", np_configure_ptr->stm1_num);
    printf("NRI_bitLen:  %d \n", np_configure_ptr->NRI_bitLen);
    printf("PortPoolSize:  %d \n",np_configure_ptr->PortPoolSize);
    printf("STRU_STM1_CFG_INFO.E1T1type:	%d \n", np_configure_ptr->stru_stm1_cfg_info[g_stm1_id].E1T1type);
//	  printf("stru_stm1_cfg_info[%d].Opt_Mode:	%d \n", g_stm1_id,np_configure_ptr->stru_stm1_cfg_info[g_stm1_id].Opt_Mode);
    printf("stru_stm1_cfg_info[%d].trunk_info[%d].Opt_Mode:	%d \n", g_stm1_id,g_trunk_id,np_configure_ptr->stru_stm1_cfg_info[g_stm1_id].trunk_info[g_trunk_id].Opt_Mode);
    printf("stru_stm1_cfg_info[%d].trunk_info[%d].clock_mode:  %d \n", g_stm1_id, g_trunk_id, np_configure_ptr->stru_stm1_cfg_info[g_stm1_id].trunk_info[g_trunk_id].clock_mode);
    printf("stru_stm1_cfg_info[%d].trunk_info[%d].group_info[%d].crc_type:  %d \n",g_stm1_id,g_trunk_id,g_group_id, np_configure_ptr->stru_stm1_cfg_info[g_stm1_id].trunk_info[g_trunk_id].group_info[g_group_id].crc_type);
    printf("stru_stm1_cfg_info[%d].trunk_info[%d].group_info[%d].slot_selection:  %x \n",g_stm1_id,g_trunk_id,g_group_id, np_configure_ptr->stru_stm1_cfg_info[g_stm1_id].trunk_info[g_trunk_id].group_info[g_group_id].slot_selection);
    printf("stru_stm1_cfg_info[%d].trunk_info[%d].group_info[%d].PVCQos[%d].cir:  %d \n",g_stm1_id,g_trunk_id,g_group_id,g_dlci, np_configure_ptr->stru_stm1_cfg_info[g_stm1_id].trunk_info[g_trunk_id].group_info[g_group_id].PVCQos[g_dlci].cir);
    printf("stru_stm1_cfg_info[%d].trunk_info[%d].group_info[%d].PVCQos[%d].be:  %d \n",g_stm1_id,g_trunk_id,g_group_id,g_dlci, np_configure_ptr->stru_stm1_cfg_info[g_stm1_id].trunk_info[g_trunk_id].group_info[g_group_id].PVCQos[g_dlci].be);
    printf("max_frame_size:  %d \n", np_configure_ptr->stru_stm1_cfg_info[g_stm1_id].trunk_info[g_trunk_id].group_info[g_group_id].max_frame_size);
    printf("p->routeInfo[%d].IpDst:  %x \n", route_num-1, np_configure_ptr->routeInfo[route_num-1].IpDst);
    printf("p->routeInfo[%d].GatewayIP:  %x \n", route_num-1, np_configure_ptr->routeInfo[route_num-1].GatewayIP);
    printf("------------- STRU_SET_INITAIL_CONFIG \n");
#endif

    return 0;
}

int
cli_apply_conf_files( glob_t glob_p,STRU_SET_INITAIL_CONFIG *np_configure_ptr )
{
    int file_counter;
    int ret = 0;

    file_counter = glob_p.gl_pathc;

    //printf("\nthe file_counter number = %d.\n", file_counter);

    /* Note: This part used to set those default parameter and global variable*/
    np_configure_ptr->OpterateMode = NPU_MODE_IP;
    np_configure_ptr->stm1_num = 0;
    np_configure_ptr->VlanFlag = NPU_VLAN_DISABLE;
    route_num = 0;

    while ( file_counter > 0 && ret >= 0)
    {
        ret = expat_parser(glob_p.gl_pathv[file_counter -1], np_configure_ptr);
        if ( ret < 0 || parse_error < 0 )
            break;
        file_counter--;
        //class_num = 0; //new added
    }

    if ( ret < 0 || parse_error < 0)
        return -1;
    else
        return 0;
}



static int
find_configuration_files(glob_t *pglob, char* directory)
{
    int flags = 0;
    char tmp_file[FILENAME_MAX];
    if ( directory != NULL )
    {
        sprintf(tmp_file,"%s/*.xml",directory);
        return glob(tmp_file,flags,NULL,pglob);
    }
    return 1;
}


int
cli_apply_configuration_files_from_dir(char* configuration_dir,STRU_SET_INITAIL_CONFIG *np_configure_ptr)
{
    glob_t glob_p;
    int ret;

    ret = find_configuration_files(&glob_p,configuration_dir);
    if (ret != 0 )
    {
        globfree(&glob_p);
        return ret;
    }
	printf("\n find the correct file in %s. \n",configuration_dir);
    ret = cli_apply_conf_files(glob_p, np_configure_ptr);
    if (ret < 0 )
    {
        globfree(&glob_p);
        return ret;
    }
    globfree(&glob_p);
    return ret;
}
