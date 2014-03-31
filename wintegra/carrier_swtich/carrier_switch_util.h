/*****************************************************************************
 * (C) Copyright PMC - WIN division (Wintegra) 2012.  All rights reserved.
 * PMC-Sierra (WINTEGRA) CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of PMC-Sierra (Wintegra).
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/


#ifndef __CARRIER_SWITCH_UTIL_H__
#define __CARRIER_SWITCH_UTIL_H__

typedef union StatField
{
        WP_U64 field;
        struct {
                WP_U32 high;
                WP_U32 low;
        } part;
} WPT_StatField;

#define NR_GBE                          24

#define MAX_SUBPORT_PER_PORT            10
#define MAX_UNIPORT_PER_SYSTEM          12
#define MAX_NNIPORT_PER_SYSTEM          6
#define MAX_PORT_PER_SYSTEM             (MAX_UNIPORT_PER_SYSTEM + MAX_NNIPORT_PER_SYSTEM)

#define NUM_OF_MC_GROUPS 10
#define NUM_OF_VLAN_GROUPS 10

#define STAG_ETHER_TYPE     0x9100

#define IW_SYS_MAX_ENET_PORT        8   //max count of ENET ports 
#define IW_SYS_MAX_MC_MEMBERS       IW_SYS_MAX_ENET_PORT //max count of ports in one MC group
#define IW_SYS_MAX_VLAN_MEMBERS     IW_SYS_MAX_ENET_PORT //max count of ports in one vlan group

#define WPE_ERROR        0xffffffff

#ifndef DEFAULT_WPID
#define DEFAULT_WPID 0
#endif

#define PCE_RULE_VLAN_TYPE_1TAG_MASK    0x02
#define PCE_RULE_VLAN_TYPE_1TAG_VALUE   0x02
#define PCE_RULE_VLAN_TYPE_2TAG_MASK    0x04
#define PCE_RULE_VLAN_TYPE_2TAG_VALUE   0x04

#define L2_HEADER_LENGTH        12  // (da 6 + sa 6)
#define L2_HEADER_LENGTH_VLAN   16
#define L2_MPLS_ADD_SIZE        22  //(da 6 + sa 6 + vlan 4 + type 2 + mpls 4 + [lsp 4])

#define WT_MEGA     (1000000UL)

#define MAX_RATE    (WP_U32)(0xffffffff)    // 32bits

enum UNNIfilters
{
    FILTER_F1,
    FILTER_F2,
    FILTER_F3,
    FILTER_F4,
    FILTER_F5,
    FILTER_F6,
    FILTER_F7,
    FILTER_F8,
    FILTER_F9,
    FILTER_F10,
    FILTER_F11,
    FILTER_F12,
    FILTER_LAST,
};

enum AFMode
{
    AF_MODE_NA  = 0,
    AF_MODE_SP  = 1,
    AF_MODE_WFQ = 2,
};

enum PortType
{
    PORT_TYPE_NA  = 0,
    PORT_TYPE_UNI = 1,
    PORT_TYPE_NNI = 2,

};

enum VSITYPE
{
    SYSTEM_TYPE_NA,
    SYSTEM_TYPE_VSI,
    SYSTEM_TYPE_VRF,
    SYSTEM_TYPE_VPWS,
};

enum BROUTER_PECS_FLOW_INFO_TYPE
{
    PECS_FLOW_DEFAULT   = 0,
    PECS_FLOW_UNI       = 1,
    PECS_FLOW_NNI       = 2,
};

enum VLAN_EDIT_OPTION
{
    VLAN_EDIT_DEFAULT     = 0,
    VLAN_EDIT_ADD_SUBVLAN = 1,
    VLAN_EDIT_DEL_SUBVLAN = 2,
};

typedef enum board_type_e
{
    BOARD_TYPE_DEFAULT = 0,
    BOARD_TYPE_4XGI,
    BOARD_TYPE_2XGI_20SGMII,
    BOARD_TYPE_8SGMII,
    BOARD_TYPE_LAST
}BOARD_TYPE;


typedef enum port_type_e
{
    PORT_HW_NA = 0,
    PORT_HW_XGI,
    PORT_HW_SGMII,
}PORT_HW_TYPE;



#define MAX_LEARNING_FLOWAGG_RULES      (NR_GBE * NUM_OF_VLAN_GROUPS * 2)

typedef struct
{
        WP_U16    pid;          // port id
        WP_U16    vid;          // vlan id
        WP_handle handle;       // handle for the rule
        WP_U16    valid;        // 0 for not valid, others for valid
} WPE_lrn_flwagg_rule;


typedef struct
{
    WP_handle flow;
    WP_handle bport;
}WT_VsiBport;

typedef struct
{
    WP_U8       priority;
    WP_U8       mapping;
}WT_QOS_MAPPING;

typedef struct
{
    WP_U8       enable;

    WP_U32      cir;
    WP_U32      eir;

}WT_POLICER;

typedef struct
{
    WP_handle   l1_groups[NUM_L1_GROUPS];
    WP_handle   l2_blocks[NUM_L2_BLOCKS];
    WP_handle   l2_groups[NUM_L2_GROUPS]; 

    WP_handle   tx_chan_enet[NUM_TX_CHANNELS];

}WT_QOS_FMU;

typedef struct
{
    WP_U8       mode;   // SP/WFQ
    WP_U16      weight[4];  // WFQ    

}WT_QOS_TYPE;

typedef struct
{
    WT_QOS_TYPE af; 
    WT_QOS_FMU  fmu;

}WT_QOS;

typedef struct
{
    WP_U8       index;
    WP_U8       valid;
    WP_U8       sa_mac[6];
    WP_U8       da_mac[6];
    WP_U16      vlan;
    WP_U16      defvlan;
    WP_U16      num_vpnport;
    
    WP_handle   bport_enet;
    WP_handle   agg_enet;
    WP_handle   rule_iwport;
    WP_handle   rule_intpri[8];
    WP_handle   rule_extpri[8];

    WT_QOS      qos;
    
}WT_SUBPORT;

typedef struct
{
    PORT_HW_TYPE    hw_type;
    WP_port         hw_port;
    WP_handle       port_enet;
    WP_handle       dev_enet;
    WP_handle       bport_enet;
    WP_handle       agg_enet;
    WP_handle       rx_channel;
    WP_handle       l1_blocks[NUM_L1_BLOCKS];

    WP_U8       index;
    WP_U8       type;   // 0-UNI or 1-NNI
    WP_U8       sa_mac[6];
    WP_U8       da_mac[6];
    WP_U16      defvlan;
    WP_U16      loopback;
    
    WP_U16      num_subport;
    WP_U16      num_vpnport;
    WT_SUBPORT  subport[MAX_SUBPORT_PER_PORT];
    WP_handle   rule_intpri[8];
    WP_handle   rule_extpri[8];

    WP_handle   vsi_next_round; // or nni
    WP_handle   vrf_next_round;
    WP_handle   vpws_next_round;

    WT_QOS      qos;
    
}WT_TP_PORT;

typedef struct
{
    WP_U16      s_tag;
    WP_U16      c_tag;
    WP_U16      egress_stag;
    WP_U16      egress_ctag;

}TP_UNI;

typedef struct
{
        WP_U32      pw_label;
        WP_U32      lsp_label;
    
}TP_NNI;

typedef union 
{
    TP_UNI uni;
    TP_NNI nni;        
}PARAM;

typedef struct
{
    WP_U16          vpnport;  // vpn port index
    WP_handle       handle;
    WP_policer_v2   v2policer;
}WPE_IN_POLICER;

typedef struct
{
    WP_U8           type;   // 0-UNI or 1-NNI
    WP_U8           valid;
    WP_U16          index;
    WP_U8           portindex;  // index of port
    WP_U8           subport;
    WP_U16          vpnid;  // VPN system belongs to
    WP_U16          item_num; // num of static mac/ ip route
    WP_U16          mac_num; // num of learnt mac
    PARAM           param;
    
    WP_handle       bport;
    WP_handle       vlanrule; //pce vlan tranlation rule
    WP_handle       vpwsrule; // pce 
    WP_handle       flowagg;
    WPE_IN_POLICER* policer;
    WP_handle       nextround;  // pce next round for NNI, iw system update for UNI
    WP_handle       bcmember;
    WP_handle       learning;   // pce rule for learning
}WPE_TP_VPNPORT;

/*the vlan flooding function use mc flow agg to realize*/
typedef struct
{
    WP_U16      valid;      
    WP_U16      count;  
    WP_handle   group_handle;       //vlan group handle
    WP_handle   agg_handle;         //the flooding flow agg handle
    WP_handle   dum_bport_handle;
    WP_handle   member_handle[MAX_PORT_PER_SYSTEM];
    WP_handle   uni_rule;
    WP_handle   nni_rule;
}WPE_FLOODING_GROUP;


typedef struct FdbEntry
{
    WP_U8   vpnport;
    WP_U8   static_mac;
    WP_U8   mac[6];
    WP_handle pce_rule;
    struct FdbEntry* next;
}Node;

typedef struct ipv4_flow
{
    WP_U8       vpnid;
    WP_U8       vpnport;
    WP_U32      destip;
    WP_U32      prefix;
    WP_handle   flow_handle;
    struct ipv4_flow* next;
}WPE_IPV4_FLOW;

typedef struct
{
    WP_U8       valid;
    WP_U8       index;
    WP_U8       aging_en;
    WP_U8       type;   // VSI, VRF, VPSW
    WP_U8       aging_weight;
    WP_U8       learn_en;
    WP_U8       gpmt_id;

    WP_handle   iw_system;
    WP_handle   default_flow_aggregation;
    WP_handle   host_rx_channel;
    WP_handle   host_bport;

    WP_handle   pce_gmpt;
    
    WP_U8       uni_num;
    WP_U8       nni_num;

    WP_U16      path_num; // static mac or ip route count
    
    WPE_TP_VPNPORT* pUNI;   // for VPWS 
    WPE_TP_VPNPORT* pNNI;   // for VPWS

    WPE_FLOODING_GROUP  flooding;
    Node*   Fdbhead;
    Node*   static_mac;
    WPE_IPV4_FLOW* route;
} WT_VsiSystemInfo;

void WPE_PortListDisplay(void);
WP_boolean WPE_HWPortIdValid(WP_U8 port);
void WPE_PrintFDBList(void);
void  WPE_DeleteAllTagFDBNode(WP_U16 sysid);
Node *WPE_DeleteFDBNodeByVlan(WP_U16 sysid, WP_U16 vlan);
Node *WPE_DeleteFDBNodeByPort(WT_VsiSystemInfo* pSystem, WP_U16 port);
Node *WPE_DeleteFDBNodeByPortAndVlan(WP_U16 sysid, WP_U16 port, WP_U16 vlan);
void WPE_AddFDBNode(WP_pce_rule_forwarding *rule);

WT_VsiSystemInfo* WPE_GetSystemByHandle(WP_handle handle);

void WPE_CreateDefaultFloodingGroup(void);
void WPE_CreateVlan(WP_U16 port, WP_U16 vlan, WP_U16 system);

void WPE_CreatePceInterface(WP_handle iw_system, WP_handle rx_channel, WP_handle filter_set, WP_boolean subport_en);
WP_handle WPE_CreateNextRoundInterface(WP_U8 system_type);


#endif//__CARRIER_SWITCH_UTIL_H__

