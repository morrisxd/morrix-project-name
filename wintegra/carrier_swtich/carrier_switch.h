/*****************************************************************************
 * (C) Copyright PMC - WIN division (Wintegra) 2012.  All rights reserved.
 * PMC-Sierra (WINTEGRA) CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of PMC-Sierra (Wintegra).
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

#ifndef __CARRIER_SWITCH_H__
#define __CARRIER_SWITCH_H__


#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "api/wp_wddi.h"
#include "api/wp_host_io.h"
#include "wp_sim.h"




#define IW_SYS_MAX_BPORTS               50




#define DEFAULT_WPID                    0

#define WT_MAX_FRAME_SIZE               (1536 + 2*512)

#define DEMO_LOCK_KEY                   (134 + 8)

#if !defined(WPL_THREAD_LOCK_KEY)
#define WPL_THREAD_LOCK_KEY             WPL_LOCK_KEY_CREATE(WPL_HW_LOCK, WPL_PRIVATE_LOCK, 7, 0)
#endif

#ifndef TRUE
#define TRUE                            1
#define FALSE                           0
#endif

#define POOL_LIMIT                      100
#define MAX_FLOW_AGG                    30
#define MAX_IW_PROGRAMMABLE_FILTERS     5
#define MAX_IW_POLICERS                 (20*1024)   //50

#define MAX_NUM_OF_CHANNELS             1

#define BUFFER_BUS                      WP_BUS_PACKET
#define BUFFER_BANK                     APP_BANK_PACKET

#define NUM_OF_PECS_GLOBAL_INFO         10
#define LEARNING_QUEUE_SIZE             30

#define MAX_MTU                         1536

#define NR_PM_TEST_REQUIRED_RULES       16000

#define LEARNING_MODE_INTERRUPT         0

#define VLAN_UNWARE                     0xffff
#define DEFAULT_STAG_TYPE               0x8100


#define MAX_BRIDGING_SYSTEM             4
#define MAX_ROUTING_SYSTEM              4
#define MAX_MPLS_SYSTEM                 4
#define MAX_IW_SYSTEM                   (MAX_BRIDGING_SYSTEM + MAX_ROUTING_SYSTEM + MAX_MPLS_SYSTEM)
#define MAX_GPMT_TABLE                  4   // same with vpn system type num now

#define NUM_OF_PECS	1

#define NUM_TX_CHANNELS     8
#define NUM_L1_GROUPS       (MAX_SUBPORT_PER_PORT + 1)
#define NUM_L1_BLOCKS       1

#define NUM_L2_GROUPS       3
#define NUM_L2_BLOCKS       1


#define NUM_OF_VPN_PORT     100
#define NUM_OF_POLICER      NUM_OF_VPN_PORT

#define DEFAULT_TTL         0x40

#define DEBUG_ENABLE        0

#define POLICER_TYPE_INT    1

#define WPE_XGI_PORTS_NUM    4

//#ifdef WP_HW_WINPATH4
#ifdef  _HW_FIFO_NUM_4_
#define WP_NUM_FIFOS_DEFAULT 4
#else
#define WP_NUM_FIFOS_DEFAULT 8
#endif


#define WTE_PACKET_PER_FLOW_NUM 100

#define FLOWS_PER_PORT WTE_FLOWS_PER_XGI_PORT

/****** STATS CONFIGURATION *******/
/* DEVICE */
#define WTE_DEV_STATS_TX_MODE    WP_DISABLE
#define WTE_DEV_STATS_RX_MODE    WP_DISABLE
/* IWP */
#define WTE_IWP_STATS_MODE       WP_IW_PORT_STAT_ENABLE
/* FLOW AGGREGATION */
#define WTE_FLOW_AGG_STATS_MODE  WP_IW_STAT_ENABLE
/************/

/****** BUS CONFIGURATIONS ******/

/* RX DEVICE STATS, TX DEVICE STATS */
#define WTE_STATISTICS_BUS       WP_BUS_INTERNAL
#define WTE_STATISTICS_BANK      APP_BANK_INTERNAL
/* IWP STATS , FIWT STATS */
#define WTE_STATISTICS2_BUS      WP_BUS_INTERNAL_MEMORY2
#define WTE_STATISTICS2_BANK     0

#define WTE_STATISTICS3_BUS      WP_BUS_INTERNAL
#define WTE_STATISTICS3_BANK     APP_BANK_INTERNAL
/* FIWT */
#ifdef WP_HW_WINPATH4
#define WTE_FIWT_BUS             WP_BUS_INTERNAL_MEMORY2
#define WTE_FIWT_BANK            0
#else
#define WTE_FIWT_BUS             WP_BUS_INTERNAL_MEMORY2
#define WTE_FIWT_BANK            0
#endif

#define WTE_POLICER_BUS          WP_BUS_INTERNAL
#define WTE_POLICER_BANK         APP_BANK_INTERNAL

#define WTE_POLICER_ACTION_BUS          WP_BUS_INTERNAL
#define WTE_POLICER_ACTION_BANK         APP_BANK_INTERNAL
/* BD */
//#define WTE_BD_BUS1_XGI1              WP_BUS_EXTERNAL_MEMORY5
//#define WTE_BD_BUS2_XGI1              WP_BUS_EXTERNAL_MEMORY6
//#define WTE_BD_BUS3_XGI1              WP_BUS_EXTERNAL_MEMORY7
//#define WTE_BD_BUS4_XGI1              WP_BUS_INTERNAL
//
//#define WTE_BD_BUS1_XGI2              WP_BUS_EXTERNAL_MEMORY5
//#define WTE_BD_BUS2_XGI2              WP_BUS_EXTERNAL_MEMORY6
//#define WTE_BD_BUS3_XGI2              WP_BUS_EXTERNAL_MEMORY7
//#define WTE_BD_BUS4_XGI2              WP_BUS_PACKET
//
//#define WTE_BD_BUS1_XGI3              WP_BUS_EXTERNAL_MEMORY5
//#define WTE_BD_BUS2_XGI3              WP_BUS_EXTERNAL_MEMORY6
//#define WTE_BD_BUS3_XGI3              WP_BUS_EXTERNAL_MEMORY7
//#define WTE_BD_BUS4_XGI3              WP_BUS_EXTERNAL_MEMORY5
//
//#define WTE_BD_BUS1_XGI4              WP_BUS_EXTERNAL_MEMORY5
//#define WTE_BD_BUS2_XGI4              WP_BUS_EXTERNAL_MEMORY6
//#define WTE_BD_BUS3_XGI4              WP_BUS_EXTERNAL_MEMORY7
//#define WTE_BD_BUS4_XGI4              WP_BUS_PARAM



#define WTE_BD_BUS1_XGI1              WP_BUS_INTERNAL //WP_BUS_PACKET //WP_BUS_EXTERNAL_MEMORY5
#define WTE_BD_BUS2_XGI1              WP_BUS_PARAM //WP_BUS_HOST   //WP_BUS_EXTERNAL_MEMORY6
#define WTE_BD_BUS3_XGI1              WP_BUS_PACKET //WP_BUS_PARAM  //WP_BUS_EXTERNAL_MEMORY7
#define WTE_BD_BUS4_XGI1              WP_BUS_PACKET

#define WTE_BD_BUS1_XGI2              WP_BUS_INTERNAL //WP_BUS_PACKET //WP_BUS_EXTERNAL_MEMORY5
#define WTE_BD_BUS2_XGI2              WP_BUS_PARAM //WP_BUS_HOST   //WP_BUS_EXTERNAL_MEMORY6
#define WTE_BD_BUS3_XGI2              WP_BUS_PACKET //WP_BUS_PARAM  //WP_BUS_EXTERNAL_MEMORY7
#define WTE_BD_BUS4_XGI2              WP_BUS_PACKET //WP_BUS_PACKET

#define WTE_BD_BUS1_XGI3              WP_BUS_INTERNAL //WP_BUS_PACKET //WP_BUS_EXTERNAL_MEMORY5
#define WTE_BD_BUS2_XGI3              WP_BUS_PARAM //WP_BUS_HOST   //WP_BUS_EXTERNAL_MEMORY6
#define WTE_BD_BUS3_XGI3              WP_BUS_PACKET //WP_BUS_PARAM  //WP_BUS_EXTERNAL_MEMORY7
#define WTE_BD_BUS4_XGI3              WP_BUS_PACKET //WP_BUS_EXTERNAL_MEMORY5

#define WTE_BD_BUS1_XGI4              WP_BUS_INTERNAL //WP_BUS_PACKET //WP_BUS_EXTERNAL_MEMORY5
#define WTE_BD_BUS2_XGI4              WP_BUS_PARAM //WP_BUS_HOST   //WP_BUS_EXTERNAL_MEMORY6
#define WTE_BD_BUS3_XGI4              WP_BUS_PACKET //WP_BUS_PARAM  //WP_BUS_EXTERNAL_MEMORY7
#define WTE_BD_BUS4_XGI4              WP_BUS_PACKET //WP_BUS_PARAM

/* BUFFERS */
#define WTI_BUFFER_SIZE (768 - 64)
#define WT_BUFFER_SIZE       768

#define WTE_BUFFER_BUS1_XGI1              WP_BUS_PACKET
#define WTE_BUFFER_BANK1_XGI1             APP_BANK_PACKET

#define WTE_BUFFER_BUS2_XGI1              WP_BUS_HOST
#define WTE_BUFFER_BANK2_XGI1             APP_BANK_HOST

#define WTE_BUFFER_BUS3_XGI1              WP_BUS_PARAM
#define WTE_BUFFER_BANK3_XGI1             APP_BANK_PARAM

#define WTE_BUFFER_BUS4_XGI1              WP_BUS_PACKET //WP_BUS_WMM5
#define WTE_BUFFER_BANK4_XGI1             APP_BANK_PACKET //0

#define WTE_BUFFER_BUS1_XGI2              WP_BUS_PACKET
#define WTE_BUFFER_BANK1_XGI2             APP_BANK_PACKET

#define WTE_BUFFER_BUS2_XGI2              WP_BUS_HOST
#define WTE_BUFFER_BANK2_XGI2             APP_BANK_HOST

#define WTE_BUFFER_BUS3_XGI2              WP_BUS_PARAM
#define WTE_BUFFER_BANK3_XGI2             APP_BANK_PARAM

#define WTE_BUFFER_BUS4_XGI2              WP_BUS_PACKET //WP_BUS_WMM5
#define WTE_BUFFER_BANK4_XGI2             APP_BANK_PACKET //0

#define WTE_BUFFER_BUS1_XGI3              WP_BUS_PACKET
#define WTE_BUFFER_BANK1_XGI3             APP_BANK_PACKET

#define WTE_BUFFER_BUS2_XGI3              WP_BUS_HOST
#define WTE_BUFFER_BANK2_XGI3             APP_BANK_HOST

#define WTE_BUFFER_BUS3_XGI3              WP_BUS_PARAM
#define WTE_BUFFER_BANK3_XGI3             APP_BANK_PARAM

#define WTE_BUFFER_BUS4_XGI3              WP_BUS_PACKET // WP_BUS_WMM5
#define WTE_BUFFER_BANK4_XGI3             APP_BANK_PACKET // 0

#define WTE_BUFFER_BUS1_XGI4              WP_BUS_PACKET
#define WTE_BUFFER_BANK1_XGI4             APP_BANK_PACKET

#define WTE_BUFFER_BUS2_XGI4              WP_BUS_HOST
#define WTE_BUFFER_BANK2_XGI4             APP_BANK_HOST

#define WTE_BUFFER_BUS3_XGI4              WP_BUS_PARAM
#define WTE_BUFFER_BANK3_XGI4             APP_BANK_PARAM

#define WTE_BUFFER_BUS4_XGI4              WP_BUS_PACKET // WP_BUS_WMM5
#define WTE_BUFFER_BANK4_XGI4             APP_BANK_PACKET // 0

#define WTE_WMM_BUFFERS_BUS               WP_BUS_PARAM
#define WTE_WMM_BUFFERS_BANK              APP_BANK_PARAM

/* HOST BD*/
#define WTE_HOST_BD_BUS          WTE_WMM_BUFFERS_BUS
#define WTE_HOST_BD_BANK         WTE_WMM_BUFFERS_BANK
/****************/


/****** TMU-FBP ******/
#define WTE_TMU_FBP_BUFFERS_BD_NUM           150 //100
#define WTE_TMU_FBP_BUFFERS_NUM              (WTE_TMU_FBP_L2_NUM * WTE_TMU_FBP_BUFFERS_BD_NUM)//600


#define WTE_TMU_FBP_L2_NUM          1 //3
#define WTE_TMU_FBP_L3_PER_L2_NUM   3 //4
/**************/

/****** TMU-TX ******/
#define WTE_MAXT 50 //0xbbcc
#define WTE_TMU_L3_SHAPPING_MODE WP_FMU_SHAPING_TYPE_STRICT
#define WTE_WMM_NUM_OF_BUFFERS ((WTE_PACKET_PER_FLOW_NUM * (NUM_TX_CHANNELS + 1)) * 4)
/*******************/


extern void App_TerminateOnError(WP_handle handle, WP_CHAR *s, WP_U32 line);
extern void App_SysInit(void);
extern void App_TestInit(void);
extern void App_InitHW(void);
extern void App_PortsDevicesCreate(void);
extern void App_BufferPoolsCreate(void);
extern void App_ModulesInit(void);
extern void App_EventsInit(void);
extern void App_RxChannelsCreate(void);
extern void App_DefaultIwSystemCreate(void);
extern void App_DefaultGpmtCreate(void);
extern void App_SystemEnable(void);
extern void App_SystemBuild(void);
extern void App_PceInterfaceCreate(void);
extern void App_perform_freerun(void);
extern void * App_LearningThread(void *arg);
extern void * App_AgingTimer(void *arg);
extern void * App_AgingThread(void *arg);
extern WP_handle App_PortsCreate(WP_handle iw_sys, WP_handle flow_agg, WP_handle *iwport, WP_U16 defvlan, WP_U32 tag);
extern WP_handle App_TxAggCreate(WP_handle txfunc, WP_handle iw_port);
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

#endif//__CARRIER_SWITCH_H__

