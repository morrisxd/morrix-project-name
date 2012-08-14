/*****************************************************************************
 * (C) Copyright PMC - WIN division (Wintegra) 2012.  All rights reserved.
 * PMC-Sierra (WINTEGRA) CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of PMC-Sierra (Wintegra).
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

#ifndef __BRIDGE_UTIL_H__
#define __BRIDGE_UTIL_H__

#define MORRIS_ENABLE_YELLOW  (1)



typedef union StatField
{
        WP_U64 field;
        struct {
                WP_U32 high;
                WP_U32 low;
        } part;
} WPT_StatField;


#define NUM_OF_MC_GROUPS 100
#define NUM_OF_VLAN_GROUPS 100



#define IW_SYS_MAX_MC_MEMBERS  11 //max count of ports in one MC group
#define IW_SYS_MAX_VLAN_MEMBERS  11 //max count of ports in one vlan group
#define IW_SYS_MAX_ENET_PORT  11 //max count of ENET ports 
#define WPE_ERROR        0xffffffff

#ifndef DEFAULT_WPID
#define DEFAULT_WPID 0
#endif


typedef struct
{
        WP_U32    valid;               //if the mc group is configured
        WP_handle group_handle;       //mc group handle
        WP_handle agg_handle;         //the mc flow agg handle
        WP_handle dum_bport_handle;
        WP_handle member_handle[IW_SYS_MAX_MC_MEMBERS]; //port member in mc group
        WP_handle PCE_rule_handle;
        WP_U16    vlan;   //mc group vlan
        WP_U16    count;   //mc group member count
        char      mc_mac[6]; //mc mac
}WPE_mc_group;

/*the vlan flooding function use mc flow agg to realize*/
typedef struct
{
        WP_U32    valid;               //if the vlan group is configured
        WP_handle group_handle;       //vlan group handle
        WP_handle agg_handle;         //the vlan flooding flow agg handle
        WP_handle dum_bport_handle;
        WP_handle member_handle[IW_SYS_MAX_VLAN_MEMBERS]; //port member in vlan group
        WP_handle PCE_rule_handle;
        WP_U16    vlan;   //vlan group vlan
        WP_U16    count;   //vlan group menber count
}WPE_vlan_group;

#define MAX_LEARNING_FLOWAGG_RULES      (NR_GBE * NUM_OF_VLAN_GROUPS * 2)

typedef struct
{
        WP_U16    pid;          // port id
        WP_U16    vid;          // vlan id
        WP_handle handle;       // handle for the rule
        WP_U16    valid;        // 0 for not valid, others for valid
} WPE_lrn_flwagg_rule;

extern WP_iw_agg_ipv6_routing rout_agg_ipv6[1];

#endif
