/*****************************************************************************
 * (C) Copyright PMC - WIN division (Wintegra) 2012.  All rights reserved.
 * PMC-Sierra (WINTEGRA) CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of PMC-Sierra (Wintegra).
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

#ifndef __BRIDGE_H__
#define __BRIDGE_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "api/wp_wddi.h"
#include "api/wp_host_io.h"
#include "wp_sim.h"

#include "wpx_app_data.h"
#include "wpx_oc12_pmc5357.h"
#include "wpx_gei_phy.h"
#include "wpx_pin_mux_selector.h"
#include "wt_partition_ecc_util.h"

#include "wpi_gpe_pecs_wddi_shared.h"
#include "brouter_pecs.h"
#include "mpls_pecs.h"

#include "provider_bridge_util.h"

#include "wpx_enet_phy.h"

#include "wpu_debug_sw.h"
#include "api/wp_pce.h"


#define NR_GBE                          10
#define NR_L1_SHAPER_GROUP_ENTRIES      (NR_GBE + 1)
#define NR_L2_SHAPER_GROUP_ENTRIES      (NR_GBE + 1)
#define XGI_L1_SHAPER_GROUP_ENTRY       (NR_GBE)
#define XGI_L2_SHAPER_GROUP_ENTRY       (NR_GBE)
#define DEFAULT_WPID                    0

#define WT_MAX_FRAME_SIZE               (1536)

#define DEMO_LOCK_KEY                   (134 + 8)

#if !defined(WPL_THREAD_LOCK_KEY)
#define WPL_THREAD_LOCK_KEY             WPL_LOCK_KEY_CREATE(WPL_HW_LOCK, WPL_PRIVATE_LOCK, 7, 0)
#endif


#define TRUE                            1
#define FALSE                           0

#define POOL_LIMIT                      20
#define MAX_FLOW_AGG                    60
#define MAX_IW_PROGRAMMABLE_FILTERS     5
#define MAX_IW_POLICERS                 0 //50

#define MAX_NUM_OF_TX_CHANNELS          8
#define MAX_NUM_OF_RX_CHANNELS          1
#define NR_PQ_BLOCK_SIZE                MAX_NUM_OF_TX_CHANNELS
#define NR_PQ_BLOCK                     0

#define NR_PRIORITY_CHANNELS            MAX_NUM_OF_TX_CHANNELS
#define VLAN_PRIORITY_OFFSET            13

#define BUFFER_BUS                      WP_BUS_PACKET
#define BUFFER_BANK                     APP_BANK_PACKET

#define NUM_OF_PECS_GLOBAL_INFO         10
#define LEARNING_QUEUE_SIZE             30
#define IW_SYS_MAX_BPORTS               30


#define NR_PM_TEST_REQUIRED_RULES       16000

#define LEARNING_MODE_INTERRUPT         0

typedef struct APP_gbe_enet_s
{
        WP_port   hw_port;
        WP_handle port_enet;
        WP_handle dev_enet;
        WP_handle bport_enet;
        WP_handle agg_enet;
        WP_handle rx_chan_enet;
        WP_handle tx_chan_enet[NR_PRIORITY_CHANNELS];
} APP_gbe_enet_t;

#define NUM_OF_PECS	2

enum filters
{
        FILTER_SET_PRIORITY_MAPPING,
        FILTER_SET_UNKNOWNUNICAST_CLASSIFICATION,
        FILTER_SET_CLASSIFICATION,
        FILTER_SET_FORWARDING,
        FILTER_SET_LEARNING,
        FILTER_SET_LAST,
};

extern APP_gbe_enet_t gbe[NR_GBE];

extern void App_TerminateOnError(WP_handle handle, WP_CHAR *s, WP_U32 line);
extern void App_SysInit(void);
extern void App_InitHW(void);
extern void App_PortsDevicesCreate(void);
extern void App_BufferPoolsCreate(void);
extern void App_ModulesInit(void);
extern void App_EventsInit(void);
extern void App_ChannelsCreate(void);
extern void App_IwSystemInit(void);
extern void App_SystemEnable(void);
extern void App_PceInterfaceCreate(void);
extern void App_perform_freerun(void);
extern void App_ShapingGroupCreateAndEnable(void);
extern void * App_LearningThread(void *arg);
extern void * App_AgingTimer(void *arg);
extern void * App_AgingThread(void *arg);

extern WP_U32 dps_ProgramImage[];
extern WP_U16 dps_PC_Table_Init[];

extern WP_pecs_image_info brouter_pecs_pecs_image;
extern WP_pecs_image_info mpls_pecs_pecs_image;

extern WP_SEM_ID aging_sem;
extern WP_SEM_ID learning_sem;

extern WP_U32 aging_period;
extern int OS_AgingTimerCreate(void);
extern int OS_AgingTimerStart(void);
extern int OS_AgingTimerDelete(void);

#endif
