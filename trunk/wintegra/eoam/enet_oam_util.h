/*****************************************************************************
 * (C) Copyright 2007, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

#ifndef EOAM_UTIL_H
#define EOAM_UTIL_H

#define DEFAULT_STAG_TYPE       ((WP_U16)0x9100)
#define DEFAULT_VLAN_TAG        100
#define OAM_MSG_STAG_TYPE       DEFAULT_STAG_TYPE
#define OAM_MSG_CTAG_TYPE       ((WP_U16)0x8100)

#define CFM_OPCODE_CCM  1
#define CFM_OPCODE_LBR  2
#define CFM_OPCODE_LBM  3
#define CFM_OPCODE_LTR  4
#define CFM_OPCODE_LTM  5

#define CFM_OPCODE_LMR  42
#define CFM_OPCODE_LMM  43
#define CFM_OPCODE_1DM  45
#define CFM_OPCODE_DMR  46
#define CFM_OPCODE_DMM  47

#define CFM_DMM_SIZE 37
#define CFM_1DM_SIZE 21
#define CFM_LMM_SIZE 17
#define CFM_LMR_SIZE 17
#define CFM_CCM_SIZE 75

#define PD_CFM_OFFSET   22 // s-tag and c-tag
#define CFM_OFFSET      18 // only one tag
#define LMM_TLV_OFFSET  12
#define DM1_TLV_OFFSET  16
#define DMM_TLV_OFFSET  32
#define CCM_TLV_OFFSET  70 

#define EOAM_MAX_MA_NUM         16
#define EOAM_MEP_NUM_PER_MA     1
#define EOAM_MAX_MEP_NUM        (EOAM_MAX_MA_NUM * EOAM_MEP_NUM_PER_MA)
#define EOAM_MAX_MSG_NUM        WP_IW_ENET_OAM_MESSAGE_TYPE_USER_CONFIG
#define EOAM_MD_LEVEL           3
#define EOAM_MAX_ULFLOW_PER_MEP 2
#define EOAM_MAX_DLFLOW_PER_MEP EOAM_MAX_ULFLOW_PER_MEP
#define EOAM_MAX_FLOW_NUM       (EOAM_MAX_MEP_NUM * (EOAM_MAX_ULFLOW_PER_MEP + EOAM_MAX_DLFLOW_PER_MEP)) * 2
#define EOAM_MAX_VLAN_PER_MEP   (EOAM_MAX_MSG_NUM * 2)


#define EOAM_ID_INVALID         0xffffffff
#define EOAM_HANDLE_INVALID     0
#define EOAM_LM_PM_DISABLE      WP_ENET_OAM_LM_STATS_DISABLE

#define EOAM_MSG_LENGTH         256
#define EOAM_MSG_QUEUE_RATE     (100)

#define NUM_OF_BUFS             10

#define EOAM_LOCK_KEY           (134 + EOAM_MD_LEVEL)

#define SEC_TO_NANO             1000000000

#if !defined(WPL_THREAD_LOCK_KEY)
#define WPL_THREAD_LOCK_KEY WPL_LOCK_KEY_CREATE(WPL_HW_LOCK, WPL_PRIVATE_LOCK, 7, 0)
#endif

#define EOAM_DEBUG_PRINT        1

/* Perfomance test macros */
#define EOAM_PERF_TEST_MAX_MA_NUM			8
#define EOAM_PERF_TEST_MAX_MEP_NUM_PER_MA	1000

#define EOAM_PERF_TEST_MAX_MEP_NUM          (EOAM_PERF_TEST_MAX_MA_NUM * EOAM_PERF_TEST_MAX_MEP_NUM_PER_MA)
#define EOAM_PERF_TEST_MAX_SVLAN_NUM  		8
#define EOAM_PERF_TEST_MAX_CVLAN_NUM  		8

#define EOAM_PERF_TEST_MAX_MAC_NUM          (EOAM_PERF_TEST_MAX_MEP_NUM / 2)

#define EOAM_PERF_TEST_MAX_MSG_NUM			(EOAM_PERF_TEST_MAX_MEP_NUM * WP_IW_ENET_OAM_MESSAGE_TYPE_USER_CONFIG)
#define EOAM_PERF_TEST_CCM_MSG_LEN          92

#define EOAM_PERF_TEST_MSG_FREQ_RATE        WP_IW_ENET_OAM_FREQ_RATE_300_PER_SEC
/* end of Perfomance test macros*/


#define EOAM_DB_MAX_MA_NUM              (/* EOAM_MAX_MA_NUM + */EOAM_PERF_TEST_MAX_MA_NUM)
#define EOAM_DB_MAX_MEP_NUM_PER_MA      (EOAM_MEP_NUM_PER_MA + EOAM_PERF_TEST_MAX_MEP_NUM_PER_MA)
#define EOAM_DB_MAX_MEP_NUM             (EOAM_DB_MAX_MA_NUM * EOAM_DB_MAX_MEP_NUM_PER_MA)
#define EOAM_DB_MAC_MSG_NUM             (EOAM_MAX_MSG_NUM + EOAM_PERF_TEST_MAX_MSG_NUM)

enum WPE_Action_type
{
   WPE_EXT_ACTION_FLOW_AGG,
   WPE_EXT_ACTION_RXFCL,
   WPE_EXT_ACTION_RXFCL_FLOW_AGG,
   WPE_EXT_ACTION_TXFCL,
   WPE_EXT_ACTION_TXFCL_FLOW_AGG
};

enum WPE_Debug_level
{
   WPE_DEBUG_CRIT = 0,
   WPE_DEBUG_ERROR,
   WPE_DEBUG_WARNING,
   WPE_DEBUG_INFO,
   WPE_DEBUG_ALL,
};

enum WPE_stat_type
{
   WPE_STAT_ENET = 1,
   WPE_STAT_BPORT,
   WPE_STAT_FLOW,
};

typedef struct struct_EOAM_DM_HEADER
{
   WP_U8  MdLevel;
   WP_U8  Version;
   WP_U8  Opcode;
   WP_U32 TxTimeStampfS;
   WP_U32 TxTimeStampfNS;
   WP_U32 RxTimeStampfS;
   WP_U32 RxTimeStampfNS;
   WP_U32 TxTimeStampbS;
   WP_U32 TxTimeStampbNS;
   WP_U32 RxTimeStampbS;
   WP_U32 RxTimeStampbNS;
}WPE_DM_HEADER_T;

typedef struct struct_EOAM_TIME
{
   WP_U16 nanosecs;
   WP_U16 microsecs;
   WP_U16 milisecs;
   WP_U16 secs;
}WPE_TIME_T;

typedef struct struct_EOAM_FLOW_PARAM
{
    WP_U16      vlan;
    WP_U8       s_mac[6];
    WP_U8       d_mac[6];

}WPE_EOAM_FLOW_PARAM_T;  

typedef struct struct_EOAM_FLOW
{
    WP_handle   handle;
    WP_boolean  enable;
    WP_boolean  backupline;
    WP_handle   flow_agg;
    WPE_EOAM_FLOW_PARAM_T param;
}WPE_EOAM_FLOW_T;

typedef struct struct_EOAM_DM_INFO
{
    WP_U64   dm_counter;
    WP_U64   total_delay;
    WP_U64   mean_delay;
    WP_U64   max_delay;
    WP_U64   last_delay;
    
}WPE_EOAM_DM_INFO_T;

typedef struct struct_EOAM_LM_INFO
{
    WP_U32 frame_loss_farend;
    WP_U32 frame_loss_nearend;
}WPE_EOAM_LM_INFO_T;

typedef struct struct_EOAM_MSG
{
    WP_handle   msg_handle;
    WP_U16      svlan;
    WP_U16      cvlan;
    WP_iw_enet_oam_msg_cfg cfg;
}WPE_EOAM_MSG_T;

typedef struct struct_EOAM_REMOTE_MEP
{
    WP_handle                   mep_handle;
    WP_iw_enet_oam_remote_mep   mep_config;
    WPE_EOAM_FLOW_T             ul_flow[EOAM_MAX_ULFLOW_PER_MEP];
    WPE_EOAM_FLOW_T             dl_flow[EOAM_MAX_DLFLOW_PER_MEP];
    WPE_EOAM_MSG_T              eoam_msg_table[EOAM_MAX_MSG_NUM];
    WPE_EOAM_LM_INFO_T          lm_info;
    WPE_EOAM_DM_INFO_T          dm_info;
    WP_iw_enet_oam_remote_mep_vlan_params vlan[EOAM_MAX_VLAN_PER_MEP];
}WPE_EOAM_RMMEP_T;

typedef struct struct_EOAM_REMOTE_MEP_PD
{
    WP_handle                   mep_handle;
    WP_iw_enet_oam_remote_mep   mep_config;
    WPE_EOAM_MSG_T              eoam_msg_table[EOAM_MAX_MSG_NUM];
}WPE_EOAM_RMMEP_PD_T;


typedef struct struct_EOAM_MA
{
    WP_handle           ma_handle;
    WP_iw_enet_oam_ma   ma_config;
    WPE_EOAM_RMMEP_T    eoam_mep_table[EOAM_MEP_NUM_PER_MA];
}WPE_EOAM_MA_T;

typedef struct struct_EOAM_MA_PD
{
    WP_handle            ma_handle;
    WP_iw_enet_oam_ma    ma_config;
    WPE_EOAM_RMMEP_PD_T* eoam_mep_table;
}WPE_EOAM_MA_PD_T;

void WPE_Eoam_Init(void);
void WPE_Reboot(void);
void WPE_Eoam_SystemRelease(void);
void WPE_Eoam_Quit(WP_U32 status);
void WPE_Eoam_SetDebugLevel(WP_U8 level);

void WPE_Eoam_RegisterEvent(void);
void WPE_Eoam_MessageQueueReceive(WP_U32 type);
void WPE_Eoam_ConvertDelayToTime(WP_U64 total_nanosecs, WPE_TIME_T* time);
void WPE_Eoam_LM_StatisticsReset(WP_handle statistics_table_handle);

void WPE_Eoam_MA_ShowInfo(void);
WP_S32 WPE_Eoam_MA_CreateMA(WP_U32 ma_id, WP_U8 rate, WP_U8 mac[6]);
WP_S32 WPE_Eoam_MA_DeleteMA(WP_U32 ma_id);

void WPE_Eoam_MSG_ShowInfo(void);
void WPE_Eoam_MEP_ShowInfo(void);
WP_S32 WPE_Eoam_MEP_CreateRemoteMEP(WP_U32 ma_id, WP_U16 mep_id, WP_U8 mac[6], WP_U16 vlan);
WP_S32 WPE_Eoam_MEP_DeleteRemoteMEP(WP_U32 ma_id, WP_U16 mep_id);

WP_S32 WPE_Eoam_CCM_GetCCMStatus(void);
WP_S32 WPE_Eoam_CCM_EnableTxCCM(WP_U32 ma_id, WP_U16 mep_id, WP_boolean lm_enable, WP_U16 vlan);
WP_S32 WPE_Eoam_CCM_DisableTxCCM(WP_U32 ma_id, WP_U16 mep_id);

WP_S32 WPE_Eoam_LM_EnableLMM(WP_U32 ma_id, WP_U16 mep_id, WP_U16 vlan, WP_U32 rate);
WP_S32 WPE_Eoam_LM_DisableLMM(WP_U32 ma_id, WP_U16 mep_id, WP_U16 vlan);
WP_S32 WPE_Eoam_LM_GetPMStats(WP_U32 ma_id, WP_U16 mep_id);
WP_S32 WPE_Eoam_LM_GetLossRatio(WP_U32 ma_id, WP_U16 mep_id);

WP_U64 WPE_Eoam_Dm_ComputeDmHeaderDelay(WPE_DM_HEADER_T* dm_header);
WP_S32 WPE_Eoam_DM_EnableDM1(WP_U32 ma_id, WP_U16 mep_id, WP_U16 vlan, WP_U32 rate);
WP_S32 WPE_Eoam_DM_DisableDM1(WP_U32 ma_id, WP_U16 mep_id, WP_U16 vlan);
WP_S32 WPE_Eoam_DM_EnableDMM(WP_U32 ma_id, WP_U16 mep_id, WP_U16 vlan, WP_U32 rate);
WP_S32 WPE_Eoam_DM_DisableDMM(WP_U32 ma_id, WP_U16 mep_id, WP_U16 vlan);
WP_S32 WPE_Eoam_DM_GetDMCounters(WP_U32 ma_id, WP_U16 mep_id);
WP_S32 WPE_Eoam_DM_CalculateDelay(WP_U32 ma_id, WP_U16 mep_id);

WP_S32 WPE_Eoam_FLOW_ShowInfo(void);
WP_S32 WPE_Eoam_FLOW_CreateFlow(WP_boolean ul_dl, WP_U8 s_mac[6], WP_U8 d_mac[6], WP_U16 vlan, WP_U32 ma_id, WP_U16 mep_id);
WP_S32 WPE_Eoam_FLOW_DeleteFlow(WP_boolean ul_dl, WP_U8 s_mac[6], WP_U8 d_mac[6], WP_U16 vlan, WP_U32 ma_id, WP_U16 mep_id);
WP_S32 WPE_Eoam_FLOW_EnableTXFCLCount(WP_U8 s_mac[6], WP_U8 d_mac[6], WP_U16 vlan, WP_U32 ma_id, WP_U16 mep_id);
WP_S32 WPE_Eoam_FLOW_DisableTXFCLCount(WP_U8 s_mac[6], WP_U8 d_mac[6], WP_U16 vlan, WP_U32 ma_id, WP_U16 mep_id);
WP_S32 WPE_Eoam_FLOW_EnableRXFCLCount(WP_U8 s_mac[6], WP_U8 d_mac[6], WP_U16 vlan, WP_U32 ma_id, WP_U16 mep_id);
WP_S32 WPE_Eoam_FLOW_DisableRXFCLCount(WP_U8 s_mac[6], WP_U8 d_mac[6], WP_U16 vlan, WP_U32 ma_id, WP_U16 mep_id);

void WPE_Eoam_PM_ShowEnetDeviceStats(WP_handle enet_device_handle, WP_U8 reset);
void WPE_Eoam_PM_ShowFlowAggStats(WP_handle flow_agg_handle, WP_U8 reset);
void WPE_Eoam_PM_ShowBridgePortStats(WP_handle bport_handle, WP_U8 reset);
WP_S32 WPE_Eoam_PM_ShowStats(WP_U8 type, WP_U8 reset);
WP_S32 WPE_Eoam_PM_SetPortLoopback(WP_U8 type, WP_U8 reset);
extern void WPI_SimulateInterrupts(void);

WP_S32 WPE_Eoam_Performance_TestStart(void);
WP_S32 WPE_Eoam_Performance_TestExit(void);
WP_S32 WPE_Eoam_Performance_CheckUtil(void);

void WPE_AssemblePacket(WP_U8 *data_ptr, WP_U16 size, WP_U32 value);
void WPE_HostSendPacket(WP_handle pool, WP_handle txchan, WP_U8 *data_ptr, WP_U16 size);

void WPE_Eoam_CfmMessageReceive(void* arg);
void WPE_Eoam_PmMessageReceive(void* arg);

void WPE_Eoam_Pce_ShowInfo(WP_handle filter_handle);

#endif//EOAM_UTIL_H


