/*****************************************************************************
 * (C) Copyright PMC - WIN division (Wintegra) 2012.  All rights reserved.
 * PMC-Sierra (WINTEGRA) CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of PMC-Sierra (Wintegra).
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************/
/*                       External Includes                                   */
/*****************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*****************************************************************************/
/*                       Internal Include                                    */
/*****************************************************************************/

#include "wp_wddi.h"
#include "wp_host_io.h"
#include "wp_types.h"
//#include "wp_enet_oam.h"

#include "enet_oam_util.h"
#include "wpui_gendefs.h"
#include "wpui_dump.h"
#include "wpui_colors.h"
#include "wpui_args.h"
#include "wpu_util.h"

extern WPE_EOAM_DB eoam_system;
extern WP_boolean  eoam_running;

extern void WPUI_PrintWinGineUtil(WP_U32 wp_no, WPU_util_info *util, WP_boolean isShortView);

WP_U32      eoam_lock;
WP_U32      ModFlagEvent = 0;     
WP_U32      ModRxErrEvent = 0;
WP_U8       eoam_debug_level = WPE_DEBUG_WARNING;
WP_boolean  eoam_performance_test = WP_FALSE;

WP_U32      eoam_total_ma_num = 0;
WP_U32      eoam_mep_num_per_ma = 0;

WP_U32 EOAM_freq_table[]=
{
   WP_IW_ENET_OAM_FREQ_RATE_DISABLE,
   WP_IW_ENET_OAM_FREQ_RATE_300_PER_SEC,
   WP_IW_ENET_OAM_FREQ_RATE_100_PER_SEC,
   WP_IW_ENET_OAM_FREQ_RATE_10_PER_SEC,
   WP_IW_ENET_OAM_FREQ_RATE_1_PER_SEC,
   WP_IW_ENET_OAM_FREQ_RATE_1_PER_10_SEC,
   WP_IW_ENET_OAM_FREQ_RATE_1_PER_MIN,
   WP_IW_ENET_OAM_FREQ_RATE_1_PER_10_MIN,
};

WP_data_segment bufs[NUM_OF_BUFS];


#ifdef EOAM_DEBUG_PRINT
    #define EOAM_DEBUG_LOG(level, ...) \
        if (eoam_debug_level >= level) {printf(__VA_ARGS__);}
#else
    #define EOAM_DEBUG_LOG(level, ...) do {} while(0);
#endif
/******************************* Internal API *************************/

WPE_EOAM_MA_T* WPE_Eoam_Ma_FindEntryByMAID(WP_U32 ma_id);
WPE_EOAM_MA_T* WPE_Eoam_Ma_FindEntryByMac(WP_U8 mac[6]);
WPE_EOAM_MA_T* WPE_Eoam_Ma_FindEntryByHandle(WP_handle ma_handle);
WPE_EOAM_MA_T* WPE_Eoam_Ma_FindEmptyEntry(void);
void WPE_Eoam_Ma_EntryInit(WPE_EOAM_MA_T* pEntry);

WPE_EOAM_RMMEP_T* WPE_Eoam_Mep_FindEntryByHandleInMa(WPE_EOAM_MA_T* pMaEntry, WP_handle mep_handle);
WPE_EOAM_RMMEP_T* WPE_Eoam_Mep_FindEntryByMEPID(WPE_EOAM_MA_T* pMaEntry, WP_U16 mep_id);
WPE_EOAM_RMMEP_T* WPE_Eoam_Mep_FindEntryByHandle(WP_handle mep_handle);
WPE_EOAM_RMMEP_T* WPE_Eoam_Mep_FindEntryByMac(WP_U8 mac[6]);
WPE_EOAM_RMMEP_T* WPE_Eoam_Mep_FindEntryByVlan(WPE_EOAM_MA_T* pMaEntry, WP_U16 vlan);
WPE_EOAM_RMMEP_T* WPE_Eoam_Mep_FindEmptyEntry(WPE_EOAM_MA_T* pMaEntry);
void WPE_Eoam_Mep_ConfigInit(WP_iw_enet_oam_remote_mep* pConfig);
void WPE_Eoam_Mep_EntryInit(WPE_EOAM_RMMEP_T* pEntry);
void WPE_Eoam_Msg_EntryInit(WPE_EOAM_MSG_T* pEntry);

WPE_EOAM_FLOW_T* WPE_Eoam_Flow_FindEntry(WPE_EOAM_RMMEP_T* pMepEntry, WP_boolean ul_dl,WP_U8 s_mac[6], WP_U8 d_mac[6],WP_U16 vlan);
WPE_EOAM_FLOW_T* WPE_Eoam_Flow_GetEmptyEntry(WPE_EOAM_RMMEP_T* pMepEntry, WP_boolean ul_dl);
WP_handle WPE_Eoam_Flow_AddFlowRule(WP_handle filter,
                                    WP_handle iw_port,
                                    WP_handle flow_aggregation,
                                    WP_handle lm_stats_handle,
                                    WP_U8 type,
                                    WP_U8 s_mac[6],
                                    WP_U8 d_mac[6],
                                    WP_U16 vlan,
                                    WP_U8 md_level);
WP_status WPE_Eoam_FlowRuleModify(WP_handle flow_handle, 
                                    WP_handle flow_aggregation,
                                    WP_handle lm_stats_handle, 
                                    WP_U8 type, 
                                    WP_U8 md_level);

void WPE_Eoam_CCM_RX_CONNECTIVITY_RETURNED(WP_iw_enet_oam_mq_entry_ccm_rx_error* pCcm);                                 
void WPE_Eoam_CCM_RX_CONNECTIVITY(WP_iw_enet_oam_mq_entry_ccm_rx_error* pCcm);
void WPE_Eoam_CCM_LM_MessageEvent(WP_iw_enet_oam_mq_entry_ccm_lm* pCcm);
void WPE_Eoam_LMR_MessageEvent(WP_iw_enet_oam_mq_entry_lmr* pLmr);
void WPE_Eoam_DMR_MessageEvent(WP_iw_enet_oam_mq_entry_dm* pDm);
void WPE_Eoam_1DM_MessageEvent(WP_iw_enet_oam_mq_entry_dm* pDm);
void WPE_Eoam_ModFlagEvent(WP_tag tag, WP_U32 event, WP_U32 eventx);
void WPE_Eoam_ModRxErrEvent(WP_tag remote_mep_handle, WP_U32 event, WP_U32 eventx);
void WPE_Eoam_MessageQueueEvent(WP_U32 type, WP_U32 res1, WP_U32 res2);
void WPE_Eoam_DataReceive(WP_tag tag, WP_U32 data, WP_U32 info);
void WPE_Eoam_PmMessageReceive(void* arg);
void WPE_Eoam_CfmMessageReceive(void* arg);
void *WPE_Eoam_Msg_Thread(void* arg);
void WPE_Eoam_ResetAllPmStats(void);
char* WPE_Eoam_MsgTypeToString(WP_iw_enet_oam_message_type rate);


void WPE_Eoam_DeleteMepEntry(WPE_EOAM_RMMEP_T* pMepEntry, WP_U8 md_level)
{
    int kk;
    WPE_EOAM_MSG_T* pMsgEntry;
    WP_handle handle;
    WP_iw_flow_rule_enet_oam_stats enet_oam_stats = {0};
    
    if ((NULL == pMepEntry) || (EOAM_HANDLE_INVALID == pMepEntry->mep_handle))
    {
        return;
    }

    // delete ul flow
    for (kk = 0; kk < EOAM_MAX_ULFLOW_PER_MEP; kk++)
    {
        handle = pMepEntry->ul_flow[kk].handle;
        if (EOAM_HANDLE_INVALID != handle)
        {
            WP_PceRuleDisable(handle);
            WP_PceRuleDelete(handle);
        }           
    }

    // delete dl flow
    for (kk = 0; kk < EOAM_MAX_DLFLOW_PER_MEP; kk++)
    {
        handle = pMepEntry->dl_flow[kk].handle;
        if (EOAM_HANDLE_INVALID != handle)
        {
            WP_PceRuleDisable(handle);
            WP_PceRuleDelete(handle);

        }            
    }            

    // delete msg 
    for (kk = 0; kk < EOAM_MAX_MSG_NUM; kk++)
    {
        pMsgEntry = pMepEntry->eoam_msg_table + kk;
        if (EOAM_HANDLE_INVALID != pMsgEntry->msg_handle)
        {
            WP_IwEnetOamMessageDisable(pMsgEntry->msg_handle);
            WP_IwEnetOamMessageDelete(pMsgEntry->msg_handle);
        }
        
        WPE_Eoam_Msg_EntryInit(pMsgEntry);
    }

    // delete lookup entry
    for (kk = 0; kk < EOAM_MAX_VLAN_PER_MEP; kk++)
    {
        if (pMepEntry->vlan[kk].stag_vlan_id)
        {              
            WP_IwEnetOamLookupEntryDelete(pMepEntry->mep_handle,
                                               WP_IW_ENET_OAM_REMOTE_MEP_PM_ENTRY,
                                               &pMepEntry->vlan[kk]);                    
        }
    }

    // delete remote mep
    handle = pMepEntry->mep_handle;
    if (EOAM_HANDLE_INVALID != handle)
    {
        if (WP_ENABLE == pMepEntry->mep_config.cfm_params.cfm_mode)
        {
            WP_IwEnetOamRemoteMepModify(pMepEntry->mep_handle,
                                        WP_IW_ENET_OAM_REMOTE_MEP_MOD_DISABLE,
                                        &pMepEntry->mep_config);
        }
        WP_IwEnetOamRemoteMepDelete(pMepEntry->mep_handle);
    }

    // delete stats table
    handle = pMepEntry->mep_config.rxtxfcl_stats_handle;
    if (EOAM_HANDLE_INVALID != handle)
    {
        WP_IwFlowStatisticsReset(handle,
                                WP_IW_FLOW_CLASS_RULE_ENET_OAM_LM_STATS,
                                &enet_oam_stats);            
        WP_IwFlowStatisticsTableDelete(handle, WP_IW_FLOW_CLASS_RULE_ENET_OAM_LM);
    }            
    
}

void WPE_Eoam_PerformanceTest_Release(void)
{
    int ii, jj, kk, ttl;
    WPE_EOAM_MA_PD_T* pMaEntry;
    WPE_EOAM_RMMEP_PD_T* pMepPdEntry;
    WPE_EOAM_MSG_T* pMsgEntry;
    WP_iw_enet_oam_remote_mep_vlan_params vlan;
    WP_status status;

    if (eoam_system.perf_ma_table)
    {
        for (ii = 0; ii < EOAM_PERF_TEST_MAX_MA_NUM; ii++)
        {
            pMaEntry = &eoam_system.perf_ma_table[ii];
            if ((NULL == pMaEntry) || (EOAM_HANDLE_INVALID == pMaEntry->ma_handle))
            {
                return;
            }

            for (jj = 0; jj < EOAM_PERF_TEST_MAX_MEP_NUM_PER_MA; jj++) 
            {
                pMepPdEntry = pMaEntry->eoam_mep_table + jj;
                if ((NULL == pMepPdEntry) || (EOAM_HANDLE_INVALID == pMepPdEntry->mep_handle)) 
                {
                    return;
                }
                                           
                WP_IwEnetOamRemoteMepModify(pMepPdEntry->mep_handle,
                                            WP_IW_ENET_OAM_REMOTE_MEP_MOD_DISABLE,
                                            &pMepPdEntry->mep_config);                
                // disable msg
                printf("Stop MSG index %04d-%04d.\r", ii, jj); 
                for (kk = 0; kk < EOAM_MAX_MSG_NUM; kk++)
                {
                    pMsgEntry = pMepPdEntry->eoam_msg_table + kk;
                    if (EOAM_HANDLE_INVALID != pMsgEntry->msg_handle)
                    {                        
                        ttl = 0;
                        status = WP_IwEnetOamMessageDisable(pMsgEntry->msg_handle);                            
                        while ((WP_ERROR(status) == WP_ERR_PREVIOUS_HOST_CMD_FAILED) && (ttl < 50))
                        {
                            status = WP_IwEnetOamMessageDisable(pMsgEntry->msg_handle);
                            ttl++;
                        }
                    }
                    
                }
                
                vlan.ctag_vlan_id = pMepPdEntry->mep_config.ctag_vlan_id;
                vlan.stag_vlan_id = pMepPdEntry->mep_config.stag_vlan_id;
                WP_IwEnetOamLookupEntryDelete(pMepPdEntry->mep_handle, WP_IW_ENET_OAM_REMOTE_MEP_PM_ENTRY, &vlan);                

           }
        }

        WP_Delay(100);
        printf("\n");

        for (ii = 0; ii < EOAM_PERF_TEST_MAX_MA_NUM; ii ++)
        {
            pMaEntry = &eoam_system.perf_ma_table[ii];

            for (jj = 0; jj < EOAM_PERF_TEST_MAX_MEP_NUM_PER_MA; jj++) 
            {
                pMepPdEntry = &pMaEntry->eoam_mep_table[jj];

                // delete msg
                printf("Delete MSG index %04d-%04d.\r", ii, jj);
                for (kk = 0; kk < EOAM_MAX_MSG_NUM; kk++)
                {
                    pMsgEntry = pMepPdEntry->eoam_msg_table + kk;
                    if (EOAM_HANDLE_INVALID != pMsgEntry->msg_handle)
                    {
                        ttl = 0;
                        status = WP_IwEnetOamMessageDelete(pMsgEntry->msg_handle);
                        while ((WP_ERROR(status) == WP_ERR_PREVIOUS_HOST_CMD_FAILED) && (ttl < 50))
                        {
                            status = WP_IwEnetOamMessageDelete(pMsgEntry->msg_handle);
                            ttl++;
                        }
                        WPE_Eoam_Msg_EntryInit(pMsgEntry);
                    }                        
                }
                
                if (EOAM_HANDLE_INVALID != pMepPdEntry->mep_handle)
                {                                                   
                    WP_IwEnetOamRemoteMepDelete(pMepPdEntry->mep_handle);
                }
                        
            }

            free(pMaEntry->eoam_mep_table);
            pMaEntry->eoam_mep_table = NULL;
        
            // delete ma
            if (EOAM_HANDLE_INVALID != pMaEntry->ma_handle)
            {
                WP_IwEnetOamMaDelete(pMaEntry->ma_handle);    
            }
        }

        free(eoam_system.perf_ma_table);
        eoam_system.perf_ma_table = NULL;
        printf("\nDone.\n");
    }

    if (EOAM_HANDLE_INVALID != eoam_system.lm_dummy)
    {
        WP_IwFlowStatisticsTableDelete(eoam_system.lm_dummy, WP_IW_FLOW_CLASS_RULE_ENET_OAM_LM);
        eoam_system.lm_dummy = EOAM_HANDLE_INVALID;
    }
}
/***************************************************************
 * Func name  : WPE_Eoam_SystemRelease
 * Description: Release eoam resource.
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void WPE_Eoam_SystemRelease()
{
    int ii, jj;
    WP_handle handle;
    WPE_EOAM_MA_T* pMaEntry;
    WPE_EOAM_RMMEP_T* pMepEntry;

    WPE_Eoam_ResetAllPmStats();

    if (eoam_performance_test)
    {
        WPE_Eoam_PerformanceTest_Release();
    }
    else
    if (eoam_system.ma_num)
    {
        pMaEntry = eoam_system.eoam_ma_table;
        for (ii = 0; ii < EOAM_MAX_MA_NUM; ii++, pMaEntry++)
        {
            if (EOAM_HANDLE_INVALID == pMaEntry->ma_handle)
            {
                continue;
            }
            
            pMepEntry = pMaEntry->eoam_mep_table; 
            for (jj = 0; jj < EOAM_MEP_NUM_PER_MA; jj++, pMepEntry++)
            {
                WPE_Eoam_DeleteMepEntry(pMepEntry, pMaEntry->ma_config.maintenance_domain_level);
            }

            // delete ma
            handle = pMaEntry->ma_handle;
            if (EOAM_HANDLE_INVALID != handle)
            {
                WP_IwEnetOamMaDelete(handle);    
            }
        }
    }
    
   
}

/***************************************************************
 * Func name  : WPE_Eoam_Quit
 * Description: 
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void WPE_Eoam_Quit(WP_U32 status)
{
    if (status)
    {
        WPE_Reboot();        
    }
    eoam_running = FALSE;

    WPE_Eoam_SystemRelease();
}

void WPE_Eoam_SetDebugLevel(WP_U8 level)
{
    printf("Eoam debug level change from %u to %u\n", eoam_debug_level, level);
    eoam_debug_level = level;
}

/*****************************************************************************
 * Status / Error checking subroutine
 *****************************************************************************/
static void WPE_TerminateOnError(WP_handle handle, WP_CHAR *s, WP_U32 LineNum)
{
    if (WP_ERROR_P(handle))
    { 
        printf("Test Abort %s %s 0x%x Line:%d\n", s,
             WP_error_name[WP_ERROR(handle)], handle, LineNum);
        printf("Test Failed\n");
        WPE_Eoam_Quit(0);
    }
    else
    {
        /*    printf("Bridging App: Handle/Status returned from %s is %#8.8x \n", s, handle);*/
    }
}/* WPE_TerminateOnError */


/* WPE_TerminateOnMallocFailed */
static void WPE_TerminateOnMallocFailed(void *pointer, WP_CHAR *s)
{
    if (NULL == pointer) 
    {
        printf("Terminal malloc error %s\n", s);        
        WPE_Eoam_Quit(0);
    } 
    else 
    {
#if ( DEBUG_PRINT & WT_DEBUG_COMMON_PRINTS)
         printf(" %s memory allocation - OK, pointer = 0x%x\n", s, pointer);
#endif
    }
}

/***************************************************************
 * Func name  : WPE_ResetHandles
 * Description: Reset buf which used for host to receive packets.
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
static void WPE_ResetHandles(void)
{
   WP_U32 i;

   for(i=0; i < NUM_OF_BUFS; i++)
   {
      bufs[i].next = &bufs[i+1];
      bufs[i].displacement = 0;
   }
   bufs[NUM_OF_BUFS -1].next = NULL;
}

/***************************************************************
 * Func name  : WPE_Eoam_DbInit
 * Description: Init oam database.
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void WPE_Eoam_DbInit(void)
{
    eoam_performance_test = WP_FALSE;
    
    eoam_system.ma_num = 0;

    eoam_system.perf_ma_table = NULL;

    eoam_system.lm_dummy = EOAM_HANDLE_INVALID;

    memset(eoam_system.eoam_ma_table, 0, sizeof(eoam_system.eoam_ma_table));
    
    WPE_ResetHandles();
}

void WPE_Eoam_Init()
{
    WP_status status;
    WP_iw_enet_oam_set_clock_params clock_params =
    {
    /* seconds */  0,
    /* nanoseconds */0
    };

    status = WP_IwEnetOamClockInit(WP_WINPATH(0),
                                 WP_IW_ENET_OAM_CLOCK_INTERNAL,
                                 &clock_params);
    WPE_TerminateOnError(status, "WP_IwEnetOamGlobalClockInit", __LINE__);

    WPE_Eoam_DbInit();
    
}

/***************************************************************
 * Func name  : WPE_Eoam_Msg_Thread
 * Description: Thread to receive host packets and oam messages.
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void *WPE_Eoam_Msg_Thread(void* arg)
{
    app_task *task, a_task;
    WPL_Lock(WPL_THREAD_LOCK_KEY, &eoam_lock);
    while(1)
    {
        WPI_SimulateInterrupts();
        while((task = next_task(irq_task_list, &a_task)) != NULL)
            app_perform_action(task);     
    }

    return arg;
}

/***************************************************************
 * Func name  : WPE_Eoam_ConvertDelayToTime
 * Description: Convert delay to time format.
 * Input      : WP_U64 total_nanosecs
 * OutPut     : WPE_TIME_T* time
 * Return Val :
 ***************************************************************/
void WPE_Eoam_ConvertDelayToTime(WP_U64 total_nanosecs, WPE_TIME_T* time)
{
   time->nanosecs   = total_nanosecs % 1000;
   time->microsecs  = (total_nanosecs / 1000) % 1000;
   time->milisecs   = (total_nanosecs / 1000000) % 1000;
   time->secs       = (total_nanosecs / 1000000000) % 1000;
}

/***************************************************************
 * Func name  : WPE_Eoam_FreqRateToValue
 * Description: Freq rate value to index.
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
WP_U8 WPE_Eoam_FreqRateToValue(WP_iw_enet_oam_freq_rate rate)
{
    WP_U8 i;
    for (i = 0; i <= MENU_FREQ_RATE_MAX; i++)
    {
        if (rate == EOAM_freq_table[i])
        {
            return i;
        }
    }

    return 0;
}

/***************************************************************
 * Func name  : WPE_Eoam_FreqRateToString
 * Description: Freq rate value to string.
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
char* WPE_Eoam_FreqRateToString(WP_iw_enet_oam_freq_rate rate)
{
    switch (rate)
    {
        case WP_IW_ENET_OAM_FREQ_RATE_DISABLE:      return "disable  "; break;
        case WP_IW_ENET_OAM_FREQ_RATE_300_PER_SEC:  return "300/sec  "; break;
        case WP_IW_ENET_OAM_FREQ_RATE_100_PER_SEC:  return "100/sec  "; break;
        case WP_IW_ENET_OAM_FREQ_RATE_10_PER_SEC:   return "10/sec   "; break;
        case WP_IW_ENET_OAM_FREQ_RATE_1_PER_SEC:    return "1/sec    "; break;
        case WP_IW_ENET_OAM_FREQ_RATE_1_PER_10_SEC: return "1/10 sec "; break;
        case WP_IW_ENET_OAM_FREQ_RATE_1_PER_MIN:    return "1/min    "; break;
        case WP_IW_ENET_OAM_FREQ_RATE_1_PER_10_MIN: return "1/10 min "; break;
        default:                                    return "unkown   "; break;    
    }
}

/***************************************************************
 * Func name  : WPE_Eoam_MsgTypeToString
 * Description: OAM message type value to display string.
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
char* WPE_Eoam_MsgTypeToString(WP_iw_enet_oam_message_type rate)
{
    switch (rate)
    {
        case WP_IW_ENET_OAM_MESSAGE_TYPE_CCM:  return "CCM"; break;
        case WP_IW_ENET_OAM_MESSAGE_TYPE_1DM:  return "1DM"; break;
        case WP_IW_ENET_OAM_MESSAGE_TYPE_DMM:  return "DMM"; break;
        case WP_IW_ENET_OAM_MESSAGE_TYPE_LMM:  return "LMM"; break;
        default:                               return "???"; break;    
    }
}

/***************************************************************
 * Func name  : WPE_Eoam_IndexToFreqRate
 * Description: Index to freq rate value.
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
WP_iw_enet_oam_freq_rate WPE_Eoam_IndexToFreqRate(WP_U8 index)
{
    if ((index >= 0 ) && (index <= MENU_FREQ_RATE_MAX))
    {
        return EOAM_freq_table[index];
    }

    return WP_IW_ENET_OAM_FREQ_RATE_DISABLE;
}

/***************************************************************
 * Func name  : WPE_Eoam_ModFlagEvent
 * Description: Mod flag event callback.
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void WPE_Eoam_ModFlagEvent(WP_tag tag, WP_U32 event, WP_U32 eventx)
{
    
    ModFlagEvent++;
    
    EOAM_DEBUG_LOG(WPE_DEBUG_ERROR, "WPE_IwEnetOamModFlagEvent, tag=0x%x, event=0x%x, eventx=0x%x\n", tag, event, eventx);
}

/***************************************************************
 * Func name  : WPE_Eoam_ModRxErrEvent
 * Description: Mod rx err event callback.
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void WPE_Eoam_ModRxErrEvent(WP_tag remote_mep_handle, WP_U32 event, WP_U32 eventx)
{
    
    ModRxErrEvent++;
    
    EOAM_DEBUG_LOG(WPE_DEBUG_ERROR, "WPE_IwEnetOamModRxErrEvent, remote_mep_handle=0x%x, event=0x%x, eventx=0x%x\n", 
            remote_mep_handle, event, eventx);
    
}

/***************************************************************
 * Func name  : WPE_Eoam_CCM_LM_MessageEvent
 * Description: CCM message event callback, calculate frame loss.
 * Input      : WP_iw_enet_oam_mq_entry_ccm_lm* pCcm
 * OutPut     :
 * Return Val :
 ***************************************************************/
void WPE_Eoam_CCM_LM_MessageEvent(WP_iw_enet_oam_mq_entry_ccm_lm* pCcm)
{
    WP_U32 frame_loss_nearend_temp = 0;
    WP_U32 frame_loss_farend_temp = 0;
    WPE_EOAM_RMMEP_T* pMepEntry = NULL;

    EOAM_DEBUG_LOG(WPE_DEBUG_INFO, "WPE_Eoam_CCM_LM_MessageEvent\nmep handle=0x%x, rxfcl_tp=%u, txfcf_tp=%u, rxfcb_tp=%u, txfcb_tp=%u, "
                    "rxfcl_tc=%u, txfcf_tc=%u, rxfcb_tc=%u, txfcb_tc=%u\n", 
                    pCcm->remote_mep_handle, 
                    pCcm->rxfcl_tp, /* The value of RxFCl at the reception time of the before last CCM.*/
                    pCcm->txfcf_tp, /* The value of TxFcf - taken from the before last received CCM.*/
                    pCcm->rxfcb_tp, /* The value of RxFcb - taken from the before last received CCM.*/
                    pCcm->txfcb_tp, /* The value of TxFcb - taken from the before last received CCM.*/

                    pCcm->rxfcl_tc, /* The value of RxFCl at the reception time of the last CCM*/
                    pCcm->txfcf_tc, /* The value of TxFcf - taken from the last received CCM.*/
                    pCcm->rxfcb_tc, /* The value of RxFcb - taken from the last received CCM.*/
                    pCcm->txfcb_tc /* The value of TxFcb - taken from the last received CCM.*/);
                    
    pMepEntry = WPE_Eoam_Mep_FindEntryByHandle(pCcm->remote_mep_handle);
    if (NULL == pMepEntry)
    {
        return;
    }
    else
    if (EOAM_HANDLE_INVALID == pMepEntry->eoam_msg_table[WP_IW_ENET_OAM_MESSAGE_TYPE_CCM].msg_handle)
    {
        return;
    }
    
    frame_loss_farend_temp = (
    /* The number of frames sent to the remote MEP */
    abs(pCcm->txfcb_tc - pCcm->txfcb_tp) -
    /* The number of frames received by the remote MEP */
    abs(pCcm->rxfcb_tc - pCcm->rxfcb_tp));

    frame_loss_nearend_temp = (
    /* The number of frames sent by the remote MEP. */
    abs(pCcm->txfcf_tc - pCcm->txfcf_tp) -
    /* The number of frames received from the remote MEP. */
    abs(pCcm->rxfcl_tc - pCcm->rxfcl_tp));

    EOAM_DEBUG_LOG(WPE_DEBUG_ALL, "frame_loss_nearend_temp=%d, frame_loss_farend_temp=%d\n", 
                                    frame_loss_nearend_temp, frame_loss_farend_temp);
    if (frame_loss_nearend_temp)
    {
        pMepEntry->lm_info.frame_loss_nearend += frame_loss_nearend_temp;
    }
    
    if (frame_loss_farend_temp)
    {
        pMepEntry->lm_info.frame_loss_farend += frame_loss_farend_temp;
    }
    
}

/***************************************************************
 * Func name  : WPE_Eoam_LMR_MessageEvent
 * Description: LMR message event callback, calculate frame loss.
 * Input      : WP_iw_enet_oam_mq_entry_lmr* pLmr
 * OutPut     :
 * Return Val :
 ***************************************************************/
void WPE_Eoam_LMR_MessageEvent(WP_iw_enet_oam_mq_entry_lmr* pLmr)
{
    WP_U32 frame_loss_nearend_temp = 0;
    WP_U32 frame_loss_farend_temp = 0;
    WPE_EOAM_RMMEP_T* pMepEntry = NULL;
    
    EOAM_DEBUG_LOG(WPE_DEBUG_INFO, "WPE_Eoam_LMR_MessageEvent\nmep handle=0x%x, rxfcl_tp=%u, txfcf_tp=%u, rxfcf_tp=%u, txfcb_tp=%u, "
                    "rxfcl_tc=%u, txfcf_tc=%u, rxfcf_tc=%u, txfcb_tc=%u\n", 
                    pLmr->remote_mep_handle, 
                    pLmr->rxfcl_tp, /* The value of RxFCl at the reception time of the before last CCM.*/
                    pLmr->txfcf_tp, /* The value of TxFcf - taken from the before last received CCM.*/
                    pLmr->rxfcf_tp, /* The value of RxFcb - taken from the before last received CCM.*/
                    pLmr->txfcb_tp, /* The value of TxFcb - taken from the before last received CCM.*/

                    pLmr->rxfcl_tc, /* The value of RxFCl at the reception time of the last CCM*/
                    pLmr->txfcf_tc, /* The value of TxFcf - taken from the last received CCM.*/
                    pLmr->rxfcf_tc, /* The value of RxFcb - taken from the last received CCM.*/
                    pLmr->txfcb_tc /* The value of TxFcb - taken from the last received CCM.*/);
                    
    pMepEntry = WPE_Eoam_Mep_FindEntryByHandle(pLmr->remote_mep_handle);
    if (NULL == pMepEntry)
    {
        return;
    }
    else
    if (EOAM_HANDLE_INVALID == pMepEntry->eoam_msg_table[WP_IW_ENET_OAM_MESSAGE_TYPE_LMM].msg_handle)
    {
        return;
    }
    
    frame_loss_farend_temp = (
    /* The number of frames sent to the remote MEP */
    abs(pLmr->txfcf_tc - pLmr->txfcf_tp) -
    /* The number of frames received by the remote MEP */
    abs(pLmr->rxfcf_tc - pLmr->rxfcf_tp));

    frame_loss_nearend_temp = (
    /* The number of frames sent by the remote MEP. */
    abs(pLmr->txfcb_tc - pLmr->txfcb_tp) -
    /* The number of frames received from the remote MEP. */
    abs(pLmr->rxfcl_tc - pLmr->rxfcl_tp));

    EOAM_DEBUG_LOG(WPE_DEBUG_ALL, "frame_loss_nearend_temp=%d, frame_loss_farend_temp=%d\n", 
                                    frame_loss_nearend_temp, frame_loss_farend_temp);
                                    
    if (frame_loss_nearend_temp)
    {
        pMepEntry->lm_info.frame_loss_nearend += frame_loss_nearend_temp;
    }

    if (frame_loss_farend_temp)
    {
        pMepEntry->lm_info.frame_loss_farend += frame_loss_farend_temp;
    }
}

/***************************************************************
 * Func name  : WPE_Eoam_DMR_MessageEvent
 * Description: DMR message event callback, calculate delay.
 * Input      : WP_iw_enet_oam_mq_entry_dm* pDm
 * OutPut     :
 * Return Val :
 ***************************************************************/
void WPE_Eoam_DMR_MessageEvent(WP_iw_enet_oam_mq_entry_dm* pDm)
{
    WP_U64 diff_ns;
    WPE_EOAM_RMMEP_T* pMepEntry = NULL;

    EOAM_DEBUG_LOG(WPE_DEBUG_INFO, "WPE_Eoam_DMR_MessageEvent mep handle=0x%x, tx: s=%u ns=%u, rx: s=%u ns=%u\n", 
                    pDm->remote_mep_handle, 
                    pDm->txtimestampf_seconds,
                    pDm->txtimestampf_nanoseconds,
                    pDm->rxtimestampf_seconds,
                    pDm->rxtimestampf_nanoseconds);

    pMepEntry = WPE_Eoam_Mep_FindEntryByHandle(pDm->remote_mep_handle);
    if (NULL == pMepEntry)
    {
        EOAM_DEBUG_LOG(WPE_DEBUG_ERROR, "can't find remote mep\n");
        
        return;
    }
    else
    if (EOAM_HANDLE_INVALID == pMepEntry->eoam_msg_table[WP_IW_ENET_OAM_MESSAGE_TYPE_DMM].msg_handle)
    {
        return;
    }    

    diff_ns = (pDm->rxtimestampf_seconds * SEC_TO_NANO + 
               pDm->rxtimestampf_nanoseconds) -
               (pDm->txtimestampf_seconds * SEC_TO_NANO + 
               pDm->txtimestampf_nanoseconds);

    pMepEntry->dm_info.dm_counter++;
    pMepEntry->dm_info.total_delay += diff_ns;
    if (pMepEntry->dm_info.dm_counter)
    {
        pMepEntry->dm_info.mean_delay = pMepEntry->dm_info.total_delay / pMepEntry->dm_info.dm_counter;
    }
    
    pMepEntry->dm_info.last_delay = diff_ns;

    if (pMepEntry->dm_info.max_delay < diff_ns)
    {
        pMepEntry->dm_info.max_delay = diff_ns;
    }
    
}

/***************************************************************
 * Func name  : WPE_Eoam_1DM_MessageEvent
 * Description: 1DM message event callback, calculate delay.
 * Input      : WP_iw_enet_oam_mq_entry_dm* pDm
 * OutPut     :
 * Return Val :
 ***************************************************************/
void WPE_Eoam_1DM_MessageEvent(WP_iw_enet_oam_mq_entry_dm* pDm)
{
    WP_U64 diff_ns;

    WPE_EOAM_RMMEP_T* pMepEntry = NULL;

    EOAM_DEBUG_LOG(WPE_DEBUG_INFO, "WPE_Eoam_1DM_MessageEvent mep handle=0x%x, tx: s=%u ns=%u, rx: s=%u ns=%u\n", 
                    pDm->remote_mep_handle, 
                    pDm->txtimestampf_seconds,
                    pDm->txtimestampf_nanoseconds,
                    pDm->rxtimestampf_seconds,
                    pDm->rxtimestampf_nanoseconds);    
 
    pMepEntry = WPE_Eoam_Mep_FindEntryByHandle(pDm->remote_mep_handle);
    if (NULL == pMepEntry)
    {
        return;
    }
    else
    if (EOAM_HANDLE_INVALID == pMepEntry->eoam_msg_table[WP_IW_ENET_OAM_MESSAGE_TYPE_1DM].msg_handle)
    {
        return;
    }      

    diff_ns = (pDm->rxtimestampf_seconds * SEC_TO_NANO + 
               pDm->rxtimestampf_nanoseconds) -
               (pDm->txtimestampf_seconds * SEC_TO_NANO + 
               pDm->txtimestampf_nanoseconds);

    pMepEntry->dm_info.dm_counter++;
    pMepEntry->dm_info.total_delay += diff_ns;
    if (pMepEntry->dm_info.dm_counter)
    {
        pMepEntry->dm_info.mean_delay = pMepEntry->dm_info.total_delay / pMepEntry->dm_info.dm_counter;
    }
    
    pMepEntry->dm_info.last_delay = diff_ns;

    if (pMepEntry->dm_info.max_delay < diff_ns)
    {
        pMepEntry->dm_info.max_delay = diff_ns;
    }    
    
}

/***************************************************************
 * Func name  : WPE_Eoam_MessageQueueEvent
 * Description: Oam message queue event callback.
 * Input      : WP_U32 type
 * OutPut     :
 * Return Val :
 ***************************************************************/
void WPE_Eoam_MessageQueueEvent(WP_U32 type, WP_U32 res1, WP_U32 res2)
{
    add_task(irq_task_list, WP_EVENT_IW_ENET_OAM_MESSAGE_QUEUE, type, NULL);
}

/***************************************************************
 * Func name  : WPE_Eoam_MessageQueueReceive
 * Description: Check oam message queue and receive message.
 * Input      : 
 * OutPut     :
 * Return Val :
 ***************************************************************/
void WPE_Eoam_MessageQueueReceive(WP_U32 type)
{
    
    if (WP_IW_ENET_OAM_PM_MESSAGE_QUEUE == type)
    {
        WPE_Eoam_PmMessageReceive(NULL);
    }
    else
    if (WP_IW_ENET_OAM_CFM_MESSAGE_QUEUE == type)
    {
        WPE_Eoam_CfmMessageReceive(NULL);
    }
}

/***************************************************************
 * Func name  : WPE_Eoam_DataReceive
 * Description: Receive data forward to host.
 * Input      : 
 * OutPut     :
 * Return Val :
 ***************************************************************/
void WPE_Eoam_DataReceive(WP_tag tag, WP_U32 data, WP_U32 info)
{
    WP_data_unit data_unit;

    WP_status status;

    WPE_ResetHandles();
    
    data_unit.segment = &bufs[0];
    data_unit.n_segments = NUM_OF_BUFS;
    data_unit.n_active = NUM_OF_BUFS;
    data_unit.type = WP_DATA_IW;   

    status = WP_HostReceive(eoam_system.rx_res_enet_oam_host, &data_unit); 
    if (WP_ERROR(status) == WP_ERR_HST_NO_DATA_TO_GET)
    {
        return;
    }
    WPE_TerminateOnError(status, "WP_HostReceive()",__LINE__);
        
    EOAM_DEBUG_LOG(WPE_DEBUG_ALL, "host receive tag=0x%x, data=0x%x, info=0x%x\n", tag, data, info);

    
    if(data_unit.n_active > 0)
    {
        WP_U32 jj, kk;
        static WP_U8 buf[1024];
        WP_data_segment *curr_buff_ptr = data_unit.segment;
        WP_MEM_BYTES_GET(buf, curr_buff_ptr->data, curr_buff_ptr->data_size);

        for (jj = 0; jj < data_unit.n_active; ++jj, ++curr_buff_ptr)
        {
            for (kk = 0; kk < curr_buff_ptr->data_size; kk++) 
            {
                EOAM_DEBUG_LOG(WPE_DEBUG_ALL, "%2.2x", buf[kk]);
            }
            EOAM_DEBUG_LOG(WPE_DEBUG_ALL, "( %d bytes )\n", curr_buff_ptr->data_size);
            WP_PoolFree(curr_buff_ptr->pool_handle, curr_buff_ptr->data);
        }
    }
}

/***************************************************************
 * Func name  : WPE_Eoam_CCM_RX_CONNECTIVITY
 * Description: CCM message event callback.
 * Input      : WP_iw_enet_oam_mq_entry_ccm_rx_error* pCcm
 * OutPut     :
 * Return Val :
 ***************************************************************/
void WPE_Eoam_CCM_RX_CONNECTIVITY(WP_iw_enet_oam_mq_entry_ccm_rx_error* pCcm)
{
    int i;
    WP_U16 vlan;
    WP_status status;
    WP_handle filter;
    WP_handle iw_port;
    WP_handle flow_aggregation;
    WPE_EOAM_MA_T* pMaEntry = NULL;
    WPE_EOAM_RMMEP_T* pMepEntry = NULL;
    WPE_EOAM_FLOW_T* pFlowEntry = NULL;

    EOAM_DEBUG_LOG(WPE_DEBUG_WARNING, "RX_CONNECTIVITY ma=0x%x, mep=0x%x\n", pCcm->ma_handle, pCcm->remote_mep_handle);
  
    pMaEntry = WPE_Eoam_Ma_FindEntryByHandle(pCcm->ma_handle);
    if (NULL == pMaEntry)
    {
        return;
    }
    
    pMepEntry = WPE_Eoam_Mep_FindEntryByHandleInMa(pMaEntry, pCcm->remote_mep_handle);    
    if (NULL == pMepEntry)
    {
        return;
    }

    vlan = pMepEntry->eoam_msg_table[WP_IW_ENET_OAM_MESSAGE_TYPE_CCM].svlan;
    filter = eoam_system.h_pce_filter[0];
    iw_port = eoam_system.bports[ENET1_BPORT];
    flow_aggregation = eoam_system.flow_agg[FA_PORT_ENET3]; // switch to GE3
    
    pFlowEntry = pMepEntry->dl_flow;

    for (i = 0; i < EOAM_MAX_DLFLOW_PER_MEP; i++, pFlowEntry++)
    {
        if (EOAM_HANDLE_INVALID == pFlowEntry->handle)
        {
            continue;
        }

        if ((vlan == pFlowEntry->param.vlan)
        && (WP_FALSE == pFlowEntry->backupline))
        {
            status = WP_PceRuleDelete(pFlowEntry->handle);

            WPE_TerminateOnError(status, "WP_PceRuleDelete",__LINE__);

            pFlowEntry->handle = WPE_Eoam_Flow_AddFlowRule(filter, iw_port, flow_aggregation, 
                                      pMepEntry->mep_config.rxtxfcl_stats_handle,
                                      WPE_EXT_ACTION_TXFCL_FLOW_AGG,
                                      pFlowEntry->param.s_mac,
                                      pFlowEntry->param.d_mac,
                                      pFlowEntry->param.vlan,
                                      pMaEntry->ma_config.maintenance_domain_level);
                                      
            pFlowEntry->backupline = WP_TRUE;                                      
            printf("switch to backup line ENET3 flow 0x%08x\n", pFlowEntry->handle);
        }
    
    }
}

/***************************************************************
 * Func name  : WPE_Eoam_CCM_RX_CONNECTIVITY_RETURNED
 * Description: CCM message event callback.
 * Input      : WP_iw_enet_oam_mq_entry_ccm_rx_error* pCcm
 * OutPut     :
 * Return Val :
 ***************************************************************/
void WPE_Eoam_CCM_RX_CONNECTIVITY_RETURNED(WP_iw_enet_oam_mq_entry_ccm_rx_error* pCcm)
{
    int i;
    WP_U16 vlan;
    WP_status status;
    WP_handle filter;
    WP_handle iw_port;
    WP_handle flow_aggregation;      
    WPE_EOAM_MA_T* pMaEntry = NULL;
    WPE_EOAM_RMMEP_T* pMepEntry = NULL;
    WPE_EOAM_FLOW_T* pFlowEntry = NULL;

    EOAM_DEBUG_LOG(WPE_DEBUG_WARNING, "RX_CONNECTIVITY_RETURNED ma=0x%x, mep=0x%x\n", pCcm->ma_handle, pCcm->remote_mep_handle);

    pMaEntry = WPE_Eoam_Ma_FindEntryByHandle(pCcm->ma_handle);
    if (NULL == pMaEntry)
    {
        return;
    }
    
    pMepEntry = WPE_Eoam_Mep_FindEntryByHandleInMa(pMaEntry, pCcm->remote_mep_handle);    
    if (NULL == pMepEntry)
    {
        return;
    }

    vlan = pMepEntry->eoam_msg_table[WP_IW_ENET_OAM_MESSAGE_TYPE_CCM].svlan;
    filter = eoam_system.h_pce_filter[0];
    iw_port = eoam_system.bports[ENET1_BPORT];
    flow_aggregation = eoam_system.flow_agg[FA_PORT_ENET2]; // switch to GE2
    
    pFlowEntry = pMepEntry->dl_flow;

    for (i = 0; i < EOAM_MAX_DLFLOW_PER_MEP; i++, pFlowEntry++)
    {
        if (EOAM_HANDLE_INVALID == pFlowEntry->handle)
        {
            continue;
        }

        if ((vlan == pFlowEntry->param.vlan)
        && (WP_TRUE == pFlowEntry->backupline))
        {
            status = WP_PceRuleDelete(pFlowEntry->handle);
            WPE_TerminateOnError(status, "WP_PceRuleDelete",__LINE__);

            pFlowEntry->handle = WPE_Eoam_Flow_AddFlowRule(filter, iw_port, flow_aggregation, 
                                      pMepEntry->mep_config.rxtxfcl_stats_handle,
                                      WPE_EXT_ACTION_TXFCL_FLOW_AGG,
                                      pFlowEntry->param.s_mac,
                                      pFlowEntry->param.d_mac,
                                      pFlowEntry->param.vlan,
                                      pMaEntry->ma_config.maintenance_domain_level);

            pFlowEntry->backupline = WP_FALSE;                                     
            printf("switch to original line ENET2 flow 0x%08x\n", pFlowEntry->handle);
        }

    }
 
}

/***************************************************************
 * Func name  : WPE_Eoam_CfmMessageReceive
 * Description: Receive message from cfm message queue.
 * Input      : 
 * OutPut     :
 * Return Val :
 ***************************************************************/
void WPE_Eoam_CfmMessageReceive(void* arg)
{
    static WP_iw_enet_oam_message_queue_entry ret_entries[EOAM_MSG_QUEUE_RATE];
    WP_U32 num_of_ret_entries = 1;
    WP_status status;
    WP_U32 ii = 0;

    while(num_of_ret_entries)
    {
        memset(ret_entries, 0, sizeof(ret_entries));
        status = WP_IwEnetOamMessageQueueService(WP_WINPATH(0),
                                               WP_IW_ENET_OAM_CFM_MESSAGE_QUEUE,
                                               EOAM_MSG_QUEUE_RATE,
                                               ret_entries,
                                               &num_of_ret_entries);
        WPE_TerminateOnError(status,"WP_IwEnetOamMessageQueueService",__LINE__);

        if ((num_of_ret_entries) && (WP_FALSE == eoam_performance_test))
        {
            for (ii = 0; ii < num_of_ret_entries; ii++)
            {
                switch(ret_entries[ii].entry_type)
                {
                case WP_IW_ENET_OAM_MQ_ENTRY_TYPE_CCM_LM:
                     WPE_Eoam_CCM_LM_MessageEvent(&(ret_entries[ii].info.ccm_lm));
                     break;
                     
                case WP_IW_ENET_OAM_MQ_ENTRY_TYPE_LMR:
                     WPE_Eoam_LMR_MessageEvent(&(ret_entries[ii].info.lmr));
                     break;
                
                case WP_IW_ENET_OAM_MQ_ENTRY_TYPE_DMR:
                     WPE_Eoam_DMR_MessageEvent(&(ret_entries[ii].info.dm));
                     break;
                     
                case WP_IW_ENET_OAM_MQ_ENTRY_TYPE_1DM:
                     WPE_Eoam_1DM_MessageEvent(&(ret_entries[ii].info.dm));
                     break;
                
                case WP_IW_ENET_OAM_MQ_ENTRY_TYPE_CCM_FLAG_MOD:
            
                     EOAM_DEBUG_LOG(WPE_DEBUG_WARNING, "CCM_FLAG_MOD ma=0x%x, mep=0x%x, flags=0x%x\n", ret_entries[ii].info.ccm_flag_mod.ma_handle,
                                  ret_entries[ii].info.ccm_flag_mod.remote_mep_handle,
                                  ret_entries[ii].info.ccm_flag_mod.ccm_flags);
                     break;
                     
                case WP_IW_ENET_OAM_MQ_ENTRY_TYPE_CCM_RX_CONNECTIVITY:
                     WPE_Eoam_CCM_RX_CONNECTIVITY(&ret_entries[ii].info.ccm_rx_error);
                     break;
                     
                case WP_IW_ENET_OAM_MQ_ENTRY_TYPE_CCM_RX_CONNECTIVITY_RETURNED:
                     WPE_Eoam_CCM_RX_CONNECTIVITY_RETURNED(&ret_entries[ii].info.ccm_rx_error);
                     break;
                     
                case WP_IW_ENET_OAM_MQ_ENTRY_TYPE_CCM_PORT_STATUS_MOD:
                case WP_IW_ENET_OAM_MQ_ENTRY_TYPE_CCM_INTERFACE_STATUS_MOD:
                case WP_IW_ENET_OAM_MQ_ENTRY_TYPE_CCM_INVALID_PORT_STATUS:
                case WP_IW_ENET_OAM_MQ_ENTRY_TYPE_CCM_INVALID_INTERFACE_STATUS:
                case WP_IW_ENET_OAM_MQ_ENTRY_TYPE_CCM_INVALID_SEQUENCE_NUMBER:
                case WP_IW_ENET_OAM_MQ_ENTRY_TYPE_CCM_INVALID_MA_ID:
             
                     EOAM_DEBUG_LOG(WPE_DEBUG_WARNING, "type=%d, ma=0x%x, mep=0x%x, istatus=%u, pstatus=%u, cseq=0x%x, lseq=0x%x\n",
                            ret_entries[ii].entry_type,
                            ret_entries[ii].info.ccm_rx_error.ma_handle,
                            ret_entries[ii].info.ccm_rx_error.remote_mep_handle,
                            ret_entries[ii].info.ccm_rx_error.interface_status,
                            ret_entries[ii].info.ccm_rx_error.port_status,
                            ret_entries[ii].info.ccm_rx_error.currSeqNum,
                            ret_entries[ii].info.ccm_rx_error.lastSeqNum);
                            
                     break;

                default:
                   break;
                }
            }   
        }

    }

}

/***************************************************************
 * Func name  : WPE_Eoam_PmMessageReceive
 * Description: Receive message from pm message queue.
 * Input      : 
 * OutPut     :
 * Return Val :
 ***************************************************************/
void WPE_Eoam_PmMessageReceive(void* arg)
{
    static WP_iw_enet_oam_message_queue_entry ret_entries[EOAM_MSG_QUEUE_RATE];
    WP_U32 num_of_ret_entries = 1;
    WP_status status;
    WP_U32 ii = 0;

    while(num_of_ret_entries)
    {
        memset(ret_entries, 0, sizeof(ret_entries));
        status = WP_IwEnetOamMessageQueueService(WP_WINPATH(0),
                                               WP_IW_ENET_OAM_PM_MESSAGE_QUEUE,
                                               EOAM_MSG_QUEUE_RATE,
                                               ret_entries,
                                               &num_of_ret_entries);
        WPE_TerminateOnError(status,"WP_IwEnetOamMessageQueueService",__LINE__);

        if ((num_of_ret_entries) && (WP_FALSE == eoam_performance_test))
        {
            for (ii = 0; ii < num_of_ret_entries; ii++)
            {
                switch(ret_entries[ii].entry_type)
                {
                case WP_IW_ENET_OAM_MQ_ENTRY_TYPE_CCM_LM:
                     WPE_Eoam_CCM_LM_MessageEvent(&(ret_entries[ii].info.ccm_lm));
                     break;
                     
                case WP_IW_ENET_OAM_MQ_ENTRY_TYPE_LMR:
                     WPE_Eoam_LMR_MessageEvent(&(ret_entries[ii].info.lmr));
                     break;
                
                case WP_IW_ENET_OAM_MQ_ENTRY_TYPE_DMR:
                     WPE_Eoam_DMR_MessageEvent(&(ret_entries[ii].info.dm));
                     break;
                     
                case WP_IW_ENET_OAM_MQ_ENTRY_TYPE_1DM:
                     WPE_Eoam_1DM_MessageEvent(&(ret_entries[ii].info.dm));
                     break;
                
                case WP_IW_ENET_OAM_MQ_ENTRY_TYPE_CCM_FLAG_MOD:
                case WP_IW_ENET_OAM_MQ_ENTRY_TYPE_CCM_RX_CONNECTIVITY:
                case WP_IW_ENET_OAM_MQ_ENTRY_TYPE_CCM_RX_CONNECTIVITY_RETURNED:
                case WP_IW_ENET_OAM_MQ_ENTRY_TYPE_CCM_PORT_STATUS_MOD:
                case WP_IW_ENET_OAM_MQ_ENTRY_TYPE_CCM_INTERFACE_STATUS_MOD:
                case WP_IW_ENET_OAM_MQ_ENTRY_TYPE_CCM_INVALID_PORT_STATUS:
                case WP_IW_ENET_OAM_MQ_ENTRY_TYPE_CCM_INVALID_INTERFACE_STATUS:
                case WP_IW_ENET_OAM_MQ_ENTRY_TYPE_CCM_INVALID_SEQUENCE_NUMBER:
                case WP_IW_ENET_OAM_MQ_ENTRY_TYPE_CCM_INVALID_MA_ID:
                     EOAM_DEBUG_LOG(WPE_DEBUG_WARNING, "CCM event %d.\n", ret_entries[ii].entry_type);
                     break;

                default:
                   break;
                }
            }   
        }
    }
}

/***************************************************************
 * Func name  : WPE_Eoam_RegisterEvent
 * Description: Register event callback function.
 * Input      : 
 * OutPut     :
 * Return Val :
 ***************************************************************/
void WPE_Eoam_RegisterEvent(void)
{
   WP_ControlRegister(WP_EVENT_IW_ENET_OAM_CCM_FLAG_MOD, WPE_Eoam_ModFlagEvent);
   WP_ControlRegister(WP_EVENT_IW_ENET_OAM_CCM_RX_ERROR, WPE_Eoam_ModRxErrEvent);
   WP_ControlRegister(WP_EVENT_IW_ENET_OAM_MESSAGE_QUEUE, WPE_Eoam_MessageQueueEvent);
}

void WPE_Eoam_Msg_EntryInit(WPE_EOAM_MSG_T* pEntry)
{
    if (pEntry)
    {
        pEntry->msg_handle = EOAM_HANDLE_INVALID;
        pEntry->svlan = 0;
        pEntry->cvlan = 0;   // Jeff add
        if (pEntry->cfg.message_data.data)
        {
            free(pEntry->cfg.message_data.data);
            pEntry->cfg.message_data.data = NULL;
        }
        memset(&pEntry->cfg, 0, sizeof(WP_iw_enet_oam_msg_cfg));
    }
}

/***************************************************************
 * Func name  : WPE_Eoam_Msg_CreateMsg
 * Description: Convert delay to time format.
 * Input      : 
   pMaEntry     pointer to MA entry
   pMepEntry    pointer to MEP entry
   pMsg         pointer to oam msg struct
   vlan         vlan tag filled in the message
 * OutPut     : 
 * Return Val :
 ***************************************************************/
WP_handle WPE_Eoam_Msg_CreateMsg(WPE_EOAM_MA_T* pMaEntry,
                             WPE_EOAM_RMMEP_T* pMepEntry,
                             WP_iw_enet_oam_msg_cfg* pMsg,
                             WP_U16 vlan)
{

    WP_handle msg_handle;
    WP_U16 offset = pMsg->message_data.cfm_offset;
    WP_U8* cfm_data = pMsg->message_data.data;
    WPE_EOAM_RMMEP_T* pEntry = WPE_Eoam_Mep_FindEntryByMac(pMaEntry->ma_config.mac_address);
    if (NULL == pEntry)
    {
        EOAM_DEBUG_LOG(WPE_DEBUG_ERROR, "Can't find match address.\n");
        return EOAM_HANDLE_INVALID;
    }  
    
    cfm_data[0]  = pMepEntry->mep_config.mac_sa[0]; 
    cfm_data[1]  = pMepEntry->mep_config.mac_sa[1]; 
    cfm_data[2]  = pMepEntry->mep_config.mac_sa[2]; 
    cfm_data[3]  = pMepEntry->mep_config.mac_sa[3]; 
    cfm_data[4] =  pMepEntry->mep_config.mac_sa[4]; 
    cfm_data[5] =  pMepEntry->mep_config.mac_sa[5];

    cfm_data[6]  = pMaEntry->ma_config.mac_address[0]; 
    cfm_data[7]  = pMaEntry->ma_config.mac_address[1]; 
    cfm_data[8]  = pMaEntry->ma_config.mac_address[2]; 
    cfm_data[9]  = pMaEntry->ma_config.mac_address[3]; 
    cfm_data[10] = pMaEntry->ma_config.mac_address[4]; 
    cfm_data[11] = pMaEntry->ma_config.mac_address[5];

    cfm_data[12] = OAM_MSG_STAG_TYPE >> 8;
    cfm_data[13] = OAM_MSG_STAG_TYPE & 0xff;
    cfm_data[14] = vlan >> 8;
    cfm_data[15] = vlan & 0xff;
    
    cfm_data[16] = 0x89;
    cfm_data[17] = 0x02;

    cfm_data[offset + 0] = pMsg->md_level << 5;/* MD level and version */

    switch (pMsg->message_type)
    {
        case WP_IW_ENET_OAM_MESSAGE_TYPE_DMM:
        {
            cfm_data[offset + 1] = CFM_OPCODE_DMM;/* OPCODE */
            cfm_data[offset + 2] = 0x00;/* FLAGS */
            cfm_data[offset + 3] = DMM_TLV_OFFSET;/* TLV Offset */
        }
        break;

        case WP_IW_ENET_OAM_MESSAGE_TYPE_1DM:
        {
            cfm_data[offset + 1] = CFM_OPCODE_1DM;/* OPCODE */
            cfm_data[offset + 2] = 0x00;/* FLAGS */
            cfm_data[offset + 3] = DM1_TLV_OFFSET;/* TLV Offset */
        }
        break;

        case WP_IW_ENET_OAM_MESSAGE_TYPE_LMM:
        {
            cfm_data[offset + 1] = CFM_OPCODE_LMM;/* OPCODE */
            cfm_data[offset + 2] = 0x00;/* FLAGS */
            cfm_data[offset + 3] = LMM_TLV_OFFSET;/* TLV Offset */
        }
        break;

        case WP_IW_ENET_OAM_MESSAGE_TYPE_CCM:
        {
            cfm_data[offset + 1] = CFM_OPCODE_CCM;/* OPCODE */
            //cfm_data[offset + 2] = 0x00;/* FLAGS */
            cfm_data[offset + 2] = WPE_Eoam_FreqRateToValue(pMsg->freq_rate);
            cfm_data[offset + 3] = CCM_TLV_OFFSET;/* TLV Offset */
            /* MEP ID */
            cfm_data[offset + 8] = (pEntry->mep_config.remote_mep_id >> 8) & 0x1f;
            cfm_data[offset + 9] = pEntry->mep_config.remote_mep_id & 0xff;                              
        }
        break;

        default:
            return EOAM_HANDLE_INVALID;
            break;
    }

    pMsg->message_data.tx_iw_system = eoam_system.h_pce_if[1];
    pMsg->message_data.tx_input_port = eoam_system.bports[ENET_OAM_DUMMY_BPORT];
    pMsg->message_data.tx_qnode_handle = eoam_system.h_qnode_iwq;
    pMsg->message_data.pool_handle = eoam_system.h_pool_256;
    
    msg_handle = WP_IwEnetOamMessageCreate(WP_WINPATH(0), pMsg);
    WPE_TerminateOnError(msg_handle, "WP_IwEnetOamMessageCreate()" , __LINE__);
    
    return msg_handle;
}

/***************************************************************
 * Func name  : WPE_Eoam_LM_StatisticsReset
 * Description: Reset lm statistics.
 * Input      : 
 * OutPut     : 
 * Return Val :
 ***************************************************************/
void WPE_Eoam_LM_StatisticsReset(WP_handle statistics_table_handle)
{
    WP_status status;

    WP_iw_flow_rule_enet_oam_stats enet_oam_stats = {0};

    status = WP_IwFlowStatisticsReset(statistics_table_handle,
                                      WP_IW_FLOW_CLASS_RULE_ENET_OAM_LM_STATS,
                                      &enet_oam_stats);
    WPE_TerminateOnError(status, "WP_IwFlowStatistics()",__LINE__);
}


WPE_EOAM_MA_T* WPE_Eoam_Ma_FindEntryByMAID(WP_U32 ma_id)
{
    int ii;
    WPE_EOAM_MA_T* pMaEntry = eoam_system.eoam_ma_table;

    for (ii = 0; ii < EOAM_MAX_MA_NUM; ii++, pMaEntry++)
    {
        if (pMaEntry->ma_config.ma_id[0] == ma_id)
        {
            return pMaEntry;
        }
    }

    return NULL;
}

WPE_EOAM_MA_T* WPE_Eoam_Ma_FindEntryByMac(WP_U8 mac[6])
{
    int ii;
    WPE_EOAM_MA_T* pMaEntry = eoam_system.eoam_ma_table;

    for (ii = 0; ii < EOAM_MAX_MA_NUM; ii++, pMaEntry++)
    {
        if (EOAM_HANDLE_INVALID == pMaEntry->ma_handle)
        {
            continue;
        }
        
        if (0 == memcmp(mac, pMaEntry->ma_config.mac_address, 6))
        {
            return pMaEntry;
        }
    }

    return NULL;
}

WPE_EOAM_MA_T* WPE_Eoam_Ma_FindEntryByHandle(WP_handle ma_handle)
{
    int ii;
    WPE_EOAM_MA_T* pMaEntry = eoam_system.eoam_ma_table;
    if (EOAM_HANDLE_INVALID ==  ma_handle)
    {
        return NULL;
    }

    for (ii = 0; ii < EOAM_MAX_MA_NUM; ii++, pMaEntry++)
    {
        if (pMaEntry->ma_handle == ma_handle)
        {
            return pMaEntry;
        }        
    }

    return NULL;
}

WPE_EOAM_MA_T* WPE_Eoam_Ma_FindEmptyEntry(void)
{
    int ii;
    WPE_EOAM_MA_T* pMaEntry = eoam_system.eoam_ma_table;

    for (ii = 0; ii < EOAM_MAX_MA_NUM; ii++, pMaEntry++)
    {
        if (EOAM_HANDLE_INVALID == pMaEntry->ma_handle)
        {
            return pMaEntry;
        }
        
    }

    return NULL;
}

void WPE_Eoam_Ma_EntryInit(WPE_EOAM_MA_T* pEntry)
{
    if (pEntry)
    {
        memset(pEntry, 0, sizeof(WPE_EOAM_MA_T));
        pEntry->ma_handle = EOAM_HANDLE_INVALID;
        pEntry->ma_config.ma_id[0] = EOAM_ID_INVALID;
    }
}

/***************************************************************
 * Func name  : WPE_Eoam_MA_ShowInfo
 * Description: Show all valid MA instance.
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void WPE_Eoam_MA_ShowInfo(void)
{
    int ii;
    WPE_EOAM_MA_T* pMaEntry;

    
    printf("-----------------------------------------------------------------\n");
    printf("| MA ID  |  MA handle  | MD level | Freq rate |  Mac address\n");
    printf("-----------------------------------------------------------------\n");
    
    pMaEntry = eoam_system.eoam_ma_table;

    for (ii = 0; ii < EOAM_MAX_MA_NUM; ii++, pMaEntry++)
    {
        if (EOAM_HANDLE_INVALID == pMaEntry->ma_handle)
        {
            continue;
        }
        printf(" %8d   0x%08x     %3d       %s  %02x:%02x:%02x:%02x:%02x:%02x\n", 
            pMaEntry->ma_config.ma_id[0], 
            pMaEntry->ma_handle,
            pMaEntry->ma_config.maintenance_domain_level,
            WPE_Eoam_FreqRateToString(pMaEntry->ma_config.freq_rate),
            pMaEntry->ma_config.mac_address[0], 
            pMaEntry->ma_config.mac_address[1],
            pMaEntry->ma_config.mac_address[2], 
            pMaEntry->ma_config.mac_address[3], 
            pMaEntry->ma_config.mac_address[4], 
            pMaEntry->ma_config.mac_address[5]);

    }
    printf("\n");
    
}

/***************************************************************
 * Func name  : WPE_Eoam_MA_CreateMA
 * Description: Create MA entity.
 * Input      : 
   ma_id        Must be unique in system
   rate         Specify the frequency rate fo the CCMs.
   mac          Will be used as source mac address of oam message
 * OutPut     : 
 * Return Val :
 ***************************************************************/
WP_S32 WPE_Eoam_MA_CreateMA(WP_U32 ma_id, WP_U8 rate, WP_U8 mac[6])
{
    WP_handle ma_handle;
    WP_iw_enet_oam_ma ma_config;
    WPE_EOAM_MA_T* pMaEntry = NULL;

    if (eoam_performance_test)
    {
        EOAM_DEBUG_LOG(WPE_DEBUG_ERROR, "Performance test is running(%d)!\n", ma_id);
        
        return 0;        
    }
    
    pMaEntry = WPE_Eoam_Ma_FindEntryByMAID(ma_id);

    if (pMaEntry)
    {
        EOAM_DEBUG_LOG(WPE_DEBUG_WARNING, "MA ID %d already created.\n", ma_id);
        
        return 1;
    }

    pMaEntry = WPE_Eoam_Ma_FindEmptyEntry();
    if (NULL == pMaEntry)
    {
        EOAM_DEBUG_LOG(WPE_DEBUG_ERROR, "MA table full.\n");
        
        return 1;
    }
    
    memset(&ma_config, 0, sizeof(ma_config));
    ma_config.freq_rate = WPE_Eoam_IndexToFreqRate(rate);
    ma_config.maintenance_domain_level = EOAM_MD_LEVEL;
    ma_config.ma_id[0] = ma_id;
    ma_config.ma_id_check = WP_DISABLE;
    memcpy(ma_config.mac_address, mac, sizeof(ma_config.mac_address));
    
    ma_handle = WP_IwEnetOamMaCreate(WP_WINPATH(0), WP_IW_ENET_OAM_MA_TYPE_SINGLE_MAC, &ma_config);
    WPE_TerminateOnError(ma_handle  , "WP_IwEnetOamMaCreate",__LINE__);

    pMaEntry->ma_handle = ma_handle;
    memcpy(&pMaEntry->ma_config, &ma_config, sizeof(WP_iw_enet_oam_ma));
    eoam_system.ma_num++;
    
    return 0;
}


/***************************************************************
 * Func name  : WPE_Eoam_MA_DeleteMA
 * Description: Delete MA according maid.
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
WP_S32 WPE_Eoam_MA_DeleteMA(WP_U32 ma_id)
{
    WP_status status;
    WPE_EOAM_MA_T* pMaEntry;

    if (eoam_performance_test)
    {
        EOAM_DEBUG_LOG(WPE_DEBUG_ERROR, "Performance test is running!(%d)\n", ma_id);
        
        return 0;        
    }
    
    pMaEntry = WPE_Eoam_Ma_FindEntryByMAID(ma_id);
    if (NULL == pMaEntry)
    {
        EOAM_DEBUG_LOG(WPE_DEBUG_ERROR, "Can't find MA ID %d .\n", ma_id);
        
        return 1;
    }
    
    status = WP_IwEnetOamMaDelete(pMaEntry->ma_handle);
    WPE_TerminateOnError(status  , "WP_IwEnetOamMaDelete",__LINE__);

    WPE_Eoam_Ma_EntryInit(pMaEntry);

    if (eoam_system.ma_num)
    {
        eoam_system.ma_num--;
    }
    
    return 0;
}


WPE_EOAM_RMMEP_T* WPE_Eoam_Mep_FindEntryByMEPID(WPE_EOAM_MA_T* pMaEntry, WP_U16 mep_id)
{
    int ii;
    WPE_EOAM_RMMEP_T* pEntry = NULL;

    if (NULL == pMaEntry)
    {
        return NULL;
    }

    pEntry = pMaEntry->eoam_mep_table;

    for (ii = 0; ii < EOAM_MEP_NUM_PER_MA; ii++, pEntry++)
    {
        if (pEntry->mep_config.remote_mep_id == mep_id)
        {
            return pEntry;
        }
        
    }

    return NULL;
}

WPE_EOAM_RMMEP_T* WPE_Eoam_Mep_FindEntryByHandleInMa(WPE_EOAM_MA_T* pMaEntry, WP_handle mep_handle)
{
    int ii;
    WPE_EOAM_RMMEP_T* pEntry = NULL;

    if ((NULL == pMaEntry) || (EOAM_HANDLE_INVALID == mep_handle))
    {
        return NULL;
    }

    pEntry = pMaEntry->eoam_mep_table;

    for (ii = 0; ii < EOAM_MEP_NUM_PER_MA; ii++, pEntry++)
    {
        if (pEntry->mep_handle == mep_handle)
        {
            return pEntry;
        }

        
    }

    return NULL;
}

WPE_EOAM_RMMEP_T* WPE_Eoam_Mep_FindEntryByHandle(WP_handle mep_handle)
{
    int ii, jj;
    WPE_EOAM_MA_T* pMaEntry = eoam_system.eoam_ma_table;
    WPE_EOAM_RMMEP_T* pEntry = NULL;
    if (EOAM_HANDLE_INVALID == mep_handle)
    {
        return NULL;
    }

    for (jj = 0; jj < EOAM_MAX_MA_NUM; jj++, pMaEntry++)
    {
        if (EOAM_HANDLE_INVALID == pMaEntry->ma_handle)
        {
            continue;
        }
        
        pEntry = pMaEntry->eoam_mep_table;
        for (ii = 0; ii < EOAM_MEP_NUM_PER_MA; ii++, pEntry++)
        {
            if (pEntry->mep_handle == mep_handle)
            {
                return pEntry;
            }
        }    
    }

    return NULL;
}

WPE_EOAM_RMMEP_T* WPE_Eoam_Mep_FindEntryByMac(WP_U8 mac[6])
{
    int ii, jj;
    WPE_EOAM_MA_T* pMaEntry = eoam_system.eoam_ma_table;
    WPE_EOAM_RMMEP_T* pEntry;

    for (jj = 0; jj < EOAM_MAX_MA_NUM; jj++, pMaEntry++)
    {
        if (EOAM_HANDLE_INVALID == pMaEntry->ma_handle)
        {
            continue;
        }
        
        pEntry = pMaEntry->eoam_mep_table;
        for (ii = 0; ii < EOAM_MEP_NUM_PER_MA; ii++, pEntry++)
        {
            if (EOAM_HANDLE_INVALID == pEntry->mep_handle)
            {
                continue;
            }

            if (0 == memcmp(mac, pEntry->mep_config.mac_sa, 6))
            {
                return pEntry;
            }
        }    
    }

    return NULL;
}

WPE_EOAM_RMMEP_T* WPE_Eoam_Mep_FindEmptyEntry(WPE_EOAM_MA_T* pMaEntry)
{
    int ii;
    WPE_EOAM_RMMEP_T* pEntry;
    if (NULL == pMaEntry)
    {
        return NULL;
    }

    pEntry = pMaEntry->eoam_mep_table;
    for (ii = 0; ii < EOAM_MEP_NUM_PER_MA; ii++, pEntry++)
    {
        if (EOAM_HANDLE_INVALID == pEntry->mep_handle)
        {
            return pEntry;
        }
    }

    return NULL;
}

WPE_EOAM_RMMEP_T* WPE_Eoam_Mep_FindEntryByVlan(WPE_EOAM_MA_T* pMaEntry, WP_U16 vlan)
{
    int ii;
    WPE_EOAM_RMMEP_T* pEntry;
    if (NULL == pMaEntry)
    {
        return NULL;
    }

    pEntry = pMaEntry->eoam_mep_table;    
    for (ii = 0; ii < EOAM_MEP_NUM_PER_MA; ii++, pEntry++)
    {
        if (pEntry->mep_config.stag_vlan_id == vlan)
        {
            return pEntry;
        }
    }

    return NULL;
}

void WPE_Eoam_Mep_ConfigInit(WP_iw_enet_oam_remote_mep* pConfig)
{
    if (pConfig)
    {
        pConfig->remote_mep_id = 0;
        pConfig->message_queue_mode = WP_IW_ENET_OAM_MSGQ_DISABLE;
        pConfig->cfm_params.cfm_mode = WP_DISABLE;
        pConfig->pm_params.dm_mode = WP_IW_ENET_OAM_DM_DISABLE;
        pConfig->pm_params.lm_mode = WP_IW_ENET_OAM_LM_DISABLE;
        pConfig->pm_params.process_mode = WP_IW_ENET_OAM_PROCESS_DROP;
        pConfig->rxtxfcl_stats_handle = EOAM_HANDLE_INVALID;
        pConfig->ctag_vlan_id = 0;
        pConfig->stag_vlan_id = 0;
        memset(pConfig->mac_sa, 0, sizeof(pConfig->mac_sa));
    }
}

void WPE_Eoam_Mep_EntryInit(WPE_EOAM_RMMEP_T* pEntry)
{
    if (pEntry)
    {
        memset(pEntry, 0, sizeof(WPE_EOAM_RMMEP_T));
        pEntry->mep_handle = EOAM_HANDLE_INVALID;
        WPE_Eoam_Mep_ConfigInit(&pEntry->mep_config);
    }
}


/***************************************************************
 * Func name  : WPE_Eoam_MEP_ShowInfo
 * Description: Show all valid MEP information.
 * Input      :
 * OutPut     :
 * Return Val :
 ***************************************************************/
void WPE_Eoam_MEP_ShowInfo(void)
{
    int ii, jj;
    WPE_EOAM_MA_T* pMaEntry;
    WPE_EOAM_RMMEP_T* pEntry = NULL;
    
    pMaEntry = eoam_system.eoam_ma_table;
    printf("--------------------------------------------------------------------------------------\n");
    printf("| MA ID  | MEP ID | MEP handle | s-vlan | stats handle | lm | dm | Mac addr\n");
    printf("--------------------------------------------------------------------------------------\n");
    for (jj = 0; jj < EOAM_MAX_MA_NUM; jj++, pMaEntry++)
    {
        if (EOAM_HANDLE_INVALID == pMaEntry->ma_handle)
        {
            continue;
        }
        
        pEntry = pMaEntry->eoam_mep_table;
        for (ii = 0; ii < EOAM_MEP_NUM_PER_MA; ii++, pEntry++)
        {
            if (EOAM_HANDLE_INVALID == pEntry->mep_handle)
            {
                continue;
            }
            printf(" %8d  %6d   0x%08x    %4d    0x%08x     %1d     %1d   %02x:%02x:%02x:%02x:%02x:%02x\n", 
                pMaEntry->ma_config.ma_id[0],
                pEntry->mep_config.remote_mep_id,
                pEntry->mep_handle,
                pEntry->mep_config.stag_vlan_id,
                pEntry->mep_config.rxtxfcl_stats_handle,
                pEntry->mep_config.pm_params.lm_mode,
                pEntry->mep_config.pm_params.dm_mode,
                pEntry->mep_config.mac_sa[0],
                pEntry->mep_config.mac_sa[1],
                pEntry->mep_config.mac_sa[2],
                pEntry->mep_config.mac_sa[3],
                pEntry->mep_config.mac_sa[4],
                pEntry->mep_config.mac_sa[5]);

        }
    }
    
    printf("\n");
}


/***************************************************************
 * Func name  : WPE_Eoam_MEP_CreateRemoteMEP
 * Description: Create remote MEP entity.
 * Input      : 
   ma_id        Must be unique in system
   mep_id       Must be unique in MA
   mac          Remote MEP mac address
   vlan         Vlan tag for this MEP
 * OutPut     : 
 * Return Val :
 ***************************************************************/
WP_S32 WPE_Eoam_MEP_CreateRemoteMEP(WP_U32 ma_id, WP_U16 mep_id, WP_U8 mac[6], WP_U16 vlan)
{
    WP_handle remote_mep_handle;
    WPE_EOAM_MA_T* pMaEntry = NULL;
    WPE_EOAM_RMMEP_T* pMepEntry = NULL;
    WP_iw_enet_oam_remote_mep mep_config;

    pMaEntry = WPE_Eoam_Ma_FindEntryByMac(mac);
    if (NULL == pMaEntry)
    {
        EOAM_DEBUG_LOG(WPE_DEBUG_ERROR, "Can't find match MAC.\n");      
        return 1;
    }    
    
    pMaEntry = WPE_Eoam_Ma_FindEntryByMAID(ma_id);
    if (NULL == pMaEntry)
    {
        EOAM_DEBUG_LOG(WPE_DEBUG_ERROR, "Can't find MA ID %d .\n", ma_id);
        
        return 1;
    }
    
    pMepEntry = WPE_Eoam_Mep_FindEntryByMEPID(pMaEntry, mep_id);
    if (pMepEntry)
    {
        EOAM_DEBUG_LOG(WPE_DEBUG_ERROR, "MEP ID %d already created.\n", mep_id);
        
        return 2;
    }
    
    pMepEntry = WPE_Eoam_Mep_FindEmptyEntry(pMaEntry);
    if (NULL == pMepEntry)
    {
        EOAM_DEBUG_LOG(WPE_DEBUG_ERROR, "MEP ID table full.\n");
        
        return 3;
    }
    
    WPE_Eoam_Mep_EntryInit(pMepEntry);
    
    WPE_Eoam_Mep_ConfigInit(&mep_config);

    mep_config.iw_port = eoam_system.bports[ENET2_BPORT];
    mep_config.cfm_params.flow_agg = eoam_system.flow_agg[FA_RES_ENET_OAM];

    mep_config.cfm_params.cfm_mode = WP_DISABLE;
    mep_config.cfm_params.status_detection_mode = WP_ENABLE;

    mep_config.cfm_params.process_mode = WP_IW_ENET_OAM_PROCESS_PASS;

    mep_config.pm_params.pm_reply_flow_aggregation = eoam_system.flow_agg[FA_PORT_ENET2];

    mep_config.pm_params.process_mode = WP_IW_ENET_OAM_PROCESS_HT;

    mep_config.pm_params.lm_mode = WP_IW_ENET_OAM_LM_DISABLE;
    mep_config.pm_params.dm_mode = WP_IW_ENET_OAM_DM_DISABLE;
    mep_config.stag_vlan_id = vlan;
    mep_config.remote_mep_id = mep_id;    
    mep_config.message_queue_mode = WP_IW_ENET_OAM_PM_MESSAGE_QUEUE | WP_IW_ENET_OAM_CFM_MESSAGE_QUEUE;
    memcpy(mep_config.mac_sa, mac, sizeof(mep_config.mac_sa));

    mep_config.rxtxfcl_stats_handle = WP_IwFlowStatisticsTableCreate(WP_WINPATH(0), WP_IW_FLOW_CLASS_RULE_ENET_OAM_LM);
    WPE_TerminateOnError(mep_config.rxtxfcl_stats_handle , "WP_IwFlowStatisticsTableCreate",__LINE__);

    remote_mep_handle = WP_IwEnetOamRemoteMepCreate(pMaEntry->ma_handle, &mep_config);
    WPE_TerminateOnError(remote_mep_handle , "WP_IwEnetOamRemoteMepCreate",__LINE__);    

    memcpy(&pMepEntry->mep_config, &mep_config, sizeof(WP_iw_enet_oam_remote_mep));
    pMepEntry->mep_handle = remote_mep_handle;
    
    return WP_OK;
}

/***************************************************************
 * Func name  : WPE_Eoam_MEP_DeleteRemoteMEP
 * Description: Delete remote MEP entity.
 * Input      : 
   ma_id        Must be unique in system
   mep_id       Must be unique in MA
 * OutPut     : 
 * Return Val :
 ***************************************************************/
WP_S32 WPE_Eoam_MEP_DeleteRemoteMEP(WP_U32 ma_id, WP_U16 mep_id)
{
    int i;
    WP_status status;
    WPE_EOAM_MA_T* pMaEntry = NULL;
    WPE_EOAM_RMMEP_T* pMepEntry = NULL;  
    WP_iw_enet_oam_remote_mep_vlan_params* pVlan = NULL;
    pMaEntry = WPE_Eoam_Ma_FindEntryByMAID(ma_id);
    if (NULL == pMaEntry)
    {
        EOAM_DEBUG_LOG(WPE_DEBUG_ERROR, "Can't find MA ID %d .\n", ma_id);
        
        return 1;
    }
    
    pMepEntry = WPE_Eoam_Mep_FindEntryByMEPID(pMaEntry, mep_id);
    if (NULL == pMepEntry)
    {
        EOAM_DEBUG_LOG(WPE_DEBUG_ERROR, "Can't find MEP ID %d .\n", mep_id);
        
        return 2;
    }

    if (WP_ENABLE == pMepEntry->mep_config.cfm_params.cfm_mode)
    {
        status = WP_IwEnetOamRemoteMepModify(pMepEntry->mep_handle,
                                            WP_IW_ENET_OAM_REMOTE_MEP_MOD_DISABLE,
                                            &pMepEntry->mep_config);
        WPE_TerminateOnError(status,"WP_IwEnetOamRemoteMepModify",__LINE__);                                        
    }

    pVlan = pMepEntry->vlan;
    for (i = 0; i < EOAM_MAX_VLAN_PER_MEP; i++, pVlan++)
    {
        if (pVlan->stag_vlan_id)
        {
            status = WP_IwEnetOamLookupEntryDelete(pMepEntry->mep_handle,
                                               WP_IW_ENET_OAM_REMOTE_MEP_PM_ENTRY,
                                               pVlan);
            WPE_TerminateOnError(status,"WP_IwEnetOamLookupEntryDelete",__LINE__);
            memset(pVlan, 0, sizeof(WP_iw_enet_oam_remote_mep_vlan_params));
        }
    }    
    status = WP_IwEnetOamRemoteMepDelete(pMepEntry->mep_handle);
    WPE_TerminateOnError(status,"WP_IwEnetOamRemoteMepGetStatus",__LINE__);

    WPE_Eoam_LM_StatisticsReset(pMepEntry->mep_config.rxtxfcl_stats_handle);
    status = WP_IwFlowStatisticsTableDelete(pMepEntry->mep_config.rxtxfcl_stats_handle, WP_IW_FLOW_CLASS_RULE_ENET_OAM_LM);
    WPE_TerminateOnError(status,"WP_IwFlowStatisticsTableDelete",__LINE__);
    
    WPE_Eoam_Mep_EntryInit(pMepEntry);
    
    
    return WP_OK;
}

WP_S32 WPE_Eoam_Mep_PmVlanEntryAdd(WPE_EOAM_RMMEP_T* pMepEntry, WP_U16 vlan)
{
    int i;
    WP_status status;
    WP_iw_enet_oam_remote_mep_vlan_params entry;
    WP_iw_enet_oam_remote_mep_vlan_params* pVlan = pMepEntry->vlan;
    WP_iw_enet_oam_remote_mep_vlan_params* pEmpty = NULL;
    if (0 == vlan)
    {
        return -2;
    }
    
    for (i = 0; i < EOAM_MAX_VLAN_PER_MEP; i++, pVlan++)
    {
        if (vlan == pVlan->stag_vlan_id)
        {
            return 0;
        }

        if ((NULL == pEmpty) && (0 == pVlan->stag_vlan_id))
        {
            pEmpty = pVlan;
        }
    }
    
    if (NULL == pEmpty)
    {
        return -1;
    }
    
    memset(&entry, 0, sizeof(entry));
    entry.stag_vlan_id = vlan;
    
    status = WP_IwEnetOamLookupEntryAdd(pMepEntry->mep_handle,
                                       WP_IW_ENET_OAM_REMOTE_MEP_PM_ENTRY,
                                       &entry);
    WPE_TerminateOnError(status,"WP_IwEnetOamLookupEntryAdd",__LINE__);

    memcpy(pEmpty, &entry, sizeof(entry));

    return 0;
}

WP_S32 WPE_Eoam_Mep_PmVlanEntryDel(WPE_EOAM_RMMEP_T* pMepEntry, WP_U16 vlan)
{
    int i;
    WP_status status;
    WP_iw_enet_oam_remote_mep_vlan_params* pVlan = pMepEntry->vlan;
    WP_iw_enet_oam_remote_mep_vlan_params* pEntry = NULL;
    if (0 == vlan)
    {
        return -3;
    }
    
    for (i = 0; i < EOAM_MAX_VLAN_PER_MEP; i++, pVlan++)
    {
        if (vlan == pVlan->stag_vlan_id)
        {
            pEntry = pVlan;
            break;
        }
    }
    
    if (NULL == pEntry)
    {
        return -1;
    }
    
    status = WP_IwEnetOamLookupEntryDelete(pMepEntry->mep_handle,
                                       WP_IW_ENET_OAM_REMOTE_MEP_PM_ENTRY,
                                       pEntry);
    WPE_TerminateOnError(status,"WP_IwEnetOamLookupEntryDelete",__LINE__);

    memset(pEntry, 0, sizeof(WP_iw_enet_oam_remote_mep_vlan_params));

    return 0;
}

/***************************************************************
 * Func name  : WPE_Eoam_MSG_ShowInfo
 * Description: Show all message information.
 * Input      : 
 * OutPut     : 
 * Return Val :
 ***************************************************************/
void WPE_Eoam_MSG_ShowInfo(void)
{
    int ii, jj, kk, flag;
    WPE_EOAM_MA_T* pMaEntry;
    WPE_EOAM_RMMEP_T* pEntry = NULL;
    WPE_EOAM_MSG_T* pMsg = NULL;
    
    pMaEntry = eoam_system.eoam_ma_table;
    printf("-----------------------------------------------------------------\n");
    printf("| MA ID  | MEP ID | vlan | type | MSG handle | vlan | freq rate \n");
    printf("-----------------------------------------------------------------\n");
    for (jj = 0; jj < EOAM_MAX_MA_NUM; jj++, pMaEntry++)
    {
        if (EOAM_HANDLE_INVALID == pMaEntry->ma_handle)
        {
            continue;
        }
        flag = 0;
        pEntry = pMaEntry->eoam_mep_table;
        for (ii = 0; ii < EOAM_MEP_NUM_PER_MA; ii++, pEntry++)
        {
            if (EOAM_HANDLE_INVALID == pEntry->mep_handle)
            {
                continue;
            }

            pMsg = pEntry->eoam_msg_table;
            for (kk = 0; kk < EOAM_MAX_MSG_NUM; kk++, pMsg++)
            {
                if (EOAM_HANDLE_INVALID == pMsg->msg_handle)
                {
                    continue;
                }
                flag++;
                printf(" %8d  %6d   %4d   %s    0x%08x   %4d   %s\n", 
                    pMaEntry->ma_config.ma_id[0],
                    pEntry->mep_config.remote_mep_id,
                    pEntry->mep_config.stag_vlan_id,
                    WPE_Eoam_MsgTypeToString(pMsg->cfg.message_type),
                    pMsg->msg_handle,
                    pMsg->svlan,
                    WPE_Eoam_FreqRateToString(pMsg->cfg.freq_rate));                
            }
        }
        if (flag)
        {
            printf("\n");
        }
    }
    
    printf("\n");
}

/***************************************************************
 * Func name  : WPE_Eoam_CCM_GetCCMStatus
 * Description: Show CCM status.
 * Input      : 
 * OutPut     : 
 * Return Val :
 ***************************************************************/
WP_S32 WPE_Eoam_CCM_GetCCMStatus()
{
    WP_status status;
    int ii, jj;
    WP_U32 temp1, temp2;
    WPE_EOAM_MA_T* pMaEntry;
    WPE_EOAM_RMMEP_T* pEntry;
    WPE_EOAM_MSG_T* pMsgEntry;
    WP_iw_enet_oam_tx_statistics tx_statistics = {0};
    WP_iw_enet_oam_remote_mep_status remote_mep_status = {0};
    
    pMaEntry = eoam_system.eoam_ma_table;
    printf("----------------------------------------------------------------------------------------------------------------\n");    
    printf(" MAID  |  MEP ID | vlan |ccm vlan|  Freq rate |lm|last_ccm_flags|num_of_missed_ccm|last_seq_num|ccm_tx_frames\n");
    printf("----------------------------------------------------------------------------------------------------------------\n");    
    for (ii = 0; ii < EOAM_MAX_MA_NUM; ii++, pMaEntry++)
    {
        for (jj = 0; jj < EOAM_MEP_NUM_PER_MA; jj++, pEntry++)
        {
            pEntry = pMaEntry->eoam_mep_table;
            if ((NULL == pEntry) || (EOAM_HANDLE_INVALID == pEntry->mep_handle))
            {
                continue;
            }
            pMsgEntry = &pEntry->eoam_msg_table[WP_IW_ENET_OAM_MESSAGE_TYPE_CCM];

            if (EOAM_HANDLE_INVALID == pMsgEntry->msg_handle)
            {
                continue;
            }
            
            status = WP_IwEnetOamRemoteMepGetStatus(pEntry->mep_handle, &remote_mep_status);
            WPE_TerminateOnError(status,"WP_IwEnetOamRemoteMepGetStatus",__LINE__);
            status = WP_IwEnetOamStatistics(pMsgEntry->msg_handle, 
                                            WP_IW_ENET_OAM_TX_STATISTICS,
                                            &tx_statistics);
            WPE_TerminateOnError(status,"WP_IwEnetOamStatistics",__LINE__);
            temp1 = tx_statistics.ccm_tx_frames;
            temp2 = tx_statistics.ccm_tx_frames >> 32;

            printf(" %6d   %6d   %4d   %4d     %9s   %1u   0x%08x       0x%08x     0x%08x   %.8x%.8x\n",
                    pMaEntry->ma_config.ma_id[0],
                    pEntry->mep_config.remote_mep_id,
                    pEntry->mep_config.stag_vlan_id,
                    pMsgEntry->svlan,
                    WPE_Eoam_FreqRateToString(pMsgEntry->cfg.freq_rate),
                    pMsgEntry->cfg.type.ccm_msg.update_lm_params,
                    remote_mep_status.last_msg_ccm_flags, 
                    remote_mep_status.num_of_missed_ccm, 
                    remote_mep_status.last_msg_seq_num,
                    temp2, temp1);
        }

    }
    printf("\n");
    
    return WP_OK;
}


/***************************************************************
 * Func name  : WPE_Eoam_CCM_EnableTxCCM
 * Description: Enable tx CCM messages.
 * Input      : 
 * OutPut     : 
 * Return Val :
 ***************************************************************/
WP_S32 WPE_Eoam_CCM_EnableTxCCM(WP_U32 ma_id, WP_U16 mep_id, WP_boolean lm_enable, WP_U16 vlan)
{
    WP_status status;
    WP_handle ccm_handle;
    WPE_EOAM_MA_T* pMaEntry = NULL;
    WPE_EOAM_RMMEP_T* pMepEntry = NULL;
    WPE_EOAM_MSG_T* pMsgEntry = NULL;
    WP_iw_enet_oam_msg_cfg msg_cfg;
    WP_iw_enet_oam_remote_mep mep_config;
    WP_command command = 0;
    
    pMaEntry = WPE_Eoam_Ma_FindEntryByMAID(ma_id);
    if (NULL == pMaEntry)
    {
        EOAM_DEBUG_LOG(WPE_DEBUG_ERROR, "Can't find MA ID %d .\n", ma_id);
        
        return 1;
    }
    
    pMepEntry = WPE_Eoam_Mep_FindEntryByMEPID(pMaEntry, mep_id);
    if (NULL == pMepEntry)
    {
        EOAM_DEBUG_LOG(WPE_DEBUG_ERROR, "Can't find MEP ID %d .\n", mep_id);
        
        return 2;
    }
    
    pMsgEntry = &pMepEntry->eoam_msg_table[WP_IW_ENET_OAM_MESSAGE_TYPE_CCM];
    if (EOAM_HANDLE_INVALID != pMsgEntry->msg_handle)
    {
        EOAM_DEBUG_LOG(WPE_DEBUG_ERROR, "CCM already enabled.\n");
        
        return 3;
    }
    
    memcpy(&mep_config, &pMepEntry->mep_config, sizeof(mep_config));
    
    if (lm_enable)
    {
        if (WP_IW_ENET_OAM_LM_DISABLE != mep_config.pm_params.lm_mode)
        {
            EOAM_DEBUG_LOG(WPE_DEBUG_ERROR, "LM is already enabled!\n");
            
            return 4;
        }
        
        mep_config.pm_params.lm_mode = WP_IW_ENET_OAM_LM_DUAL_ENDED;
        command |= WP_IW_ENET_OAM_REMOTE_MEP_MOD_LM_MODE;
    }   
        
    if (WP_DISABLE == mep_config.cfm_params.cfm_mode)
    {
        mep_config.cfm_params.cfm_mode = WP_ENABLE;
        command |= WP_IW_ENET_OAM_REMOTE_MEP_MOD_CFM_MODE;
    }
    
    if (command)
    {
        status = WP_IwEnetOamRemoteMepModify(pMepEntry->mep_handle,
                                            command,
                                            &mep_config);
        WPE_TerminateOnError(status,"WP_IwEnetOamRemoteMepModify",__LINE__);
        memcpy(&pMepEntry->mep_config, &mep_config, sizeof(mep_config));        
    }
    
    msg_cfg.message_data.data = (WP_U8 *)malloc(sizeof(WP_U8) * EOAM_MSG_LENGTH);
    WPE_TerminateOnMallocFailed(msg_cfg.message_data.data, "Message buffer allocation");
    memset(msg_cfg.message_data.data, 0, EOAM_MSG_LENGTH);

    msg_cfg.md_level = pMaEntry->ma_config.maintenance_domain_level;
    msg_cfg.ignore_oam_lookup = WP_DISABLE;
    msg_cfg.freq_rate = pMaEntry->ma_config.freq_rate;
    msg_cfg.message_type = WP_IW_ENET_OAM_MESSAGE_TYPE_CCM;

    msg_cfg.message_data.cfm_offset = CFM_OFFSET;
    msg_cfg.message_data.data_length = CFM_OFFSET + CFM_CCM_SIZE;

    msg_cfg.type.ccm_msg.update_lm_params = lm_enable;
    msg_cfg.type.ccm_msg.remote_mep_handle = pMepEntry->mep_handle;
    msg_cfg.type.ccm_msg.seq_num_mode = WP_IW_ENET_OAM_SEQUENCE_NUMBER_DISABLED;
    
    ccm_handle = WPE_Eoam_Msg_CreateMsg(pMaEntry, pMepEntry, &msg_cfg, vlan);
    if (EOAM_HANDLE_INVALID == ccm_handle)
    {
        if (msg_cfg.message_data.data)
        {
            free(msg_cfg.message_data.data);
        }    
        return 4;
    }
    
    status = WP_IwEnetOamMessageEnable(ccm_handle);
    WPE_TerminateOnError(status, "WP_IwEnetOamMessageEnable()",__LINE__);
    memcpy(&pMsgEntry->cfg, &msg_cfg, sizeof(WP_iw_enet_oam_msg_cfg));
    pMsgEntry->msg_handle = ccm_handle;
    pMsgEntry->svlan = vlan;
    
    return WP_OK;
    
}

/***************************************************************
 * Func name  : WPE_Eoam_CCM_DisableTxCCM
 * Description: 
 * Input      : 
 * OutPut     : 
 * Return Val :
 ***************************************************************/
WP_S32 WPE_Eoam_CCM_DisableTxCCM(WP_U32 ma_id, WP_U16 mep_id)
{
    WP_status status;

    WPE_EOAM_MA_T* pMaEntry = NULL;
    WPE_EOAM_RMMEP_T* pMepEntry = NULL;
    WPE_EOAM_MSG_T* pMsgEntry = NULL;
    WP_iw_enet_oam_remote_mep mep_config;
    WP_command command = 0;
    
    pMaEntry = WPE_Eoam_Ma_FindEntryByMAID(ma_id);
    if (NULL == pMaEntry)
    {
        EOAM_DEBUG_LOG(WPE_DEBUG_ERROR, "Can't find MA ID %d .\n", ma_id);
        
        return 1;
    }
    
    pMepEntry = WPE_Eoam_Mep_FindEntryByMEPID(pMaEntry, mep_id);
    if (NULL == pMepEntry)
    {
        EOAM_DEBUG_LOG(WPE_DEBUG_ERROR, "Can't find MEP ID %d .\n", mep_id);
        
        return 2;
    }

    pMsgEntry = &pMepEntry->eoam_msg_table[WP_IW_ENET_OAM_MESSAGE_TYPE_CCM];
    if (EOAM_HANDLE_INVALID == pMsgEntry->msg_handle)
    {
        EOAM_DEBUG_LOG(WPE_DEBUG_ERROR, "CCM already disabled.\n");
        
        return 3;
    }
    
    memcpy(&mep_config, &pMepEntry->mep_config, sizeof(mep_config));
    
    status = WP_IwEnetOamMessageDisable(pMsgEntry->msg_handle);
    WPE_TerminateOnError(status, "WP_IwEnetOamMessageDisable()" , __LINE__);

    status = WP_IwEnetOamMessageDelete(pMsgEntry->msg_handle);
    WPE_TerminateOnError(status, "WP_IwEnetOamMessageDelete()" , __LINE__);    
    
    if (WP_IW_ENET_OAM_LM_DUAL_ENDED == mep_config.pm_params.lm_mode)
    {
        command |= WP_IW_ENET_OAM_REMOTE_MEP_MOD_LM_MODE;
        mep_config.pm_params.lm_mode = WP_IW_ENET_OAM_LM_DISABLE;
    }  
    
    if (command)
    {
        status = WP_IwEnetOamRemoteMepModify(pMepEntry->mep_handle,
                                            command,
                                            &mep_config);
        WPE_TerminateOnError(status,"WP_IwEnetOamRemoteMepModify",__LINE__);
        memcpy(&pMepEntry->mep_config, &mep_config, sizeof(mep_config));        
    }
    
    WPE_Eoam_Msg_EntryInit(pMsgEntry);
    
    return WP_OK;
}

/***************************************************************
 * Func name  : WPE_Eoam_LM_EnableLMM
 * Description: 
 * Input      : 
 * OutPut     : 
 * Return Val :
 ***************************************************************/
WP_S32 WPE_Eoam_LM_EnableLMM(WP_U32 ma_id, WP_U16 mep_id, WP_U16 vlan, WP_U32 rate)
{
    WP_status status;
    WP_handle lmm_handle;
    WPE_EOAM_MA_T* pMaEntry = NULL;
    WPE_EOAM_RMMEP_T* pMepEntry = NULL;
    WPE_EOAM_MSG_T* pMsgEntry = NULL;
    WP_iw_enet_oam_msg_cfg msg_cfg;
    WP_iw_enet_oam_remote_mep mep_config;
    WP_command command = 0;

    
    pMaEntry = WPE_Eoam_Ma_FindEntryByMAID(ma_id);
    if (NULL == pMaEntry)
    {
        EOAM_DEBUG_LOG(WPE_DEBUG_ERROR, "Can't find MA ID %d .\n", ma_id);
        
        return 1;
    }
    
    pMepEntry = WPE_Eoam_Mep_FindEntryByMEPID(pMaEntry, mep_id);
    if (NULL == pMepEntry)
    {
        EOAM_DEBUG_LOG(WPE_DEBUG_ERROR, "Can't find MEP ID %d .\n", mep_id);
        
        return 2;
    }
     
    pMsgEntry = &pMepEntry->eoam_msg_table[WP_IW_ENET_OAM_MESSAGE_TYPE_LMM];
    if (EOAM_HANDLE_INVALID != pMsgEntry->msg_handle)
    {
        EOAM_DEBUG_LOG(WPE_DEBUG_ERROR, "CCM already enabled.\n");
        
        return 3;
    }
   
    memcpy(&mep_config, &pMepEntry->mep_config, sizeof(mep_config));
    if (WP_IW_ENET_OAM_LM_DISABLE != mep_config.pm_params.lm_mode)
    {
        EOAM_DEBUG_LOG(WPE_DEBUG_ERROR, "LM mode is already enabled.\n");
        
        return 4;
    }
    
    if (WP_IW_ENET_OAM_LM_SINGLE_ENDED != mep_config.pm_params.lm_mode)
    {
        command |= WP_IW_ENET_OAM_REMOTE_MEP_MOD_LM_MODE;
        mep_config.pm_params.lm_mode = WP_IW_ENET_OAM_LM_SINGLE_ENDED;
    }

    if (command)
    {
        status = WP_IwEnetOamRemoteMepModify(pMepEntry->mep_handle,
                                            command,
                                            &mep_config);
                                            
        WPE_TerminateOnError(status,"WP_IwEnetOamRemoteMepModify",__LINE__);
        
        memcpy(&pMepEntry->mep_config, &mep_config, sizeof(mep_config));        
    }

    if (WPE_Eoam_Mep_PmVlanEntryAdd(pMepEntry, vlan))
    {
        return 5;
    }
   
    msg_cfg.message_data.data = (WP_U8 *)malloc(sizeof(WP_U8) * EOAM_MSG_LENGTH);
    WPE_TerminateOnMallocFailed(msg_cfg.message_data.data, "Message buffer allocation");
    memset(msg_cfg.message_data.data, 0, EOAM_MSG_LENGTH);

    msg_cfg.md_level = pMaEntry->ma_config.maintenance_domain_level;
    msg_cfg.ignore_oam_lookup = WP_DISABLE;
    msg_cfg.freq_rate = WPE_Eoam_IndexToFreqRate(rate);
    msg_cfg.message_type = WP_IW_ENET_OAM_MESSAGE_TYPE_LMM;

    msg_cfg.message_data.cfm_offset = CFM_OFFSET;
    msg_cfg.message_data.data_length = CFM_OFFSET + CFM_LMM_SIZE;

    msg_cfg.type.lmm_msg.remote_mep_handle = pMepEntry->mep_handle;
    
    lmm_handle = WPE_Eoam_Msg_CreateMsg(pMaEntry, pMepEntry, &msg_cfg, vlan);
    if (EOAM_HANDLE_INVALID == lmm_handle)
    {
        if (msg_cfg.message_data.data)
        {
            free(msg_cfg.message_data.data);
        }          
        return 4;
    }
    
    status = WP_IwEnetOamMessageEnable(lmm_handle);
    WPE_TerminateOnError(status, "WP_IwEnetOamMessageEnable()",__LINE__);
    memcpy(&pMsgEntry->cfg, &msg_cfg, sizeof(WP_iw_enet_oam_msg_cfg));
    pMsgEntry->msg_handle = lmm_handle;
    pMsgEntry->svlan = vlan;
    
    return WP_OK;
}

/***************************************************************
 * Func name  : WPE_Eoam_LM_DisableLMM
 * Description: 
 * Input      : 
 * OutPut     : 
 * Return Val :
 ***************************************************************/
WP_S32 WPE_Eoam_LM_DisableLMM(WP_U32 ma_id, WP_U16 mep_id, WP_U16 vlan)
{
    WP_status status;
    WPE_EOAM_MA_T* pMaEntry = NULL;
    WPE_EOAM_RMMEP_T* pMepEntry = NULL;
    WPE_EOAM_MSG_T* pMsgEntry = NULL;
    WP_iw_enet_oam_remote_mep mep_config;
    WP_command command = 0;

    
    pMaEntry = WPE_Eoam_Ma_FindEntryByMAID(ma_id);
    if (NULL == pMaEntry)
    {
        EOAM_DEBUG_LOG(WPE_DEBUG_ERROR, "Can't find MA ID %d .\n", ma_id);
        
        return 1;
    }
    
    pMepEntry = WPE_Eoam_Mep_FindEntryByMEPID(pMaEntry, mep_id);
    if (NULL == pMepEntry)
    {
        EOAM_DEBUG_LOG(WPE_DEBUG_ERROR, "Can't find MEP ID %d .\n", mep_id);
        
        return 2;
    }

    pMsgEntry = &pMepEntry->eoam_msg_table[WP_IW_ENET_OAM_MESSAGE_TYPE_LMM];
    if (EOAM_HANDLE_INVALID == pMsgEntry->msg_handle)
    {
       EOAM_DEBUG_LOG(WPE_DEBUG_ERROR, "LMM already disabled.\n");
        
        return 3;
    }

    if (vlan != pMsgEntry->svlan)
    {
        EOAM_DEBUG_LOG(WPE_DEBUG_ERROR, "Vlan not match!\n");
        return 4;
    }
    
    memcpy(&mep_config, &pMepEntry->mep_config, sizeof(mep_config));
    if (WP_IW_ENET_OAM_LM_SINGLE_ENDED != mep_config.pm_params.lm_mode)
    {
        EOAM_DEBUG_LOG(WPE_DEBUG_ERROR, "LM mode = %d\n", mep_config.pm_params.lm_mode);
        
        return 4;
    }
    
    if (WP_IW_ENET_OAM_LM_DISABLE != mep_config.pm_params.lm_mode)
    {
        command |= WP_IW_ENET_OAM_REMOTE_MEP_MOD_LM_MODE;
        mep_config.pm_params.lm_mode = WP_IW_ENET_OAM_LM_DISABLE;
    }

    if (command)
    {
        status = WP_IwEnetOamRemoteMepModify(pMepEntry->mep_handle,
                                            command,
                                            &mep_config);
                                            
        WPE_TerminateOnError(status,"WP_IwEnetOamRemoteMepModify",__LINE__);
        
        memcpy(&pMepEntry->mep_config, &mep_config, sizeof(mep_config));        
    }
    
    status = WP_IwEnetOamMessageDisable(pMsgEntry->msg_handle);
    WPE_TerminateOnError(status, "WP_IwEnetOamMessageDisable()" , __LINE__);
    
    status = WP_IwEnetOamMessageDelete(pMsgEntry->msg_handle);
    WPE_TerminateOnError(status, "WP_IwEnetOamMessageDelete()" , __LINE__);    

    WPE_Eoam_Msg_EntryInit(pMsgEntry);
    
    return WP_OK;
}

/***************************************************************
 * Func name  : WPE_Eoam_LM_GetPMStats
 * Description: Show txfcl and rxfcl counter.
 * Input      : 
 * OutPut     : 
 * Return Val :
 ***************************************************************/

WP_S32 WPE_Eoam_LM_GetPMStats(WP_U32 ma_id, WP_U16 mep_id)
{
    WP_status status;

    WP_U32 temp1, temp2;
    WPE_EOAM_MA_T* pMaEntry = NULL;
    WPE_EOAM_RMMEP_T* pMepEntry = NULL;   
    WP_iw_flow_rule_enet_oam_stats enet_oam_stats = {0};

    
    pMaEntry = WPE_Eoam_Ma_FindEntryByMAID(ma_id);
    if (NULL == pMaEntry)
    {
        EOAM_DEBUG_LOG(WPE_DEBUG_ERROR, "Can't find MA ID %d .\n", ma_id);
        
        return 1;
    }
    
    pMepEntry = WPE_Eoam_Mep_FindEntryByMEPID(pMaEntry, mep_id);
    if (NULL == pMepEntry)
    {
        EOAM_DEBUG_LOG(WPE_DEBUG_ERROR, "Can't find MEP ID %d .\n", mep_id);
        
        return 2;
    }
    
    status = WP_IwFlowStatistics(pMepEntry->mep_config.rxtxfcl_stats_handle,
                                WP_IW_FLOW_CLASS_RULE_ENET_OAM_LM_STATS,
                                &enet_oam_stats);
    WPE_TerminateOnError(status,"WP_IwFlowStatistics()",__LINE__);

    temp1 = enet_oam_stats.rxfcl_counter;
    temp2 = enet_oam_stats.rxfcl_counter >> 32;
    printf("MA ID = %6d,  MEP ID = %6d\n", ma_id, mep_id);
    printf("rxfcl_counter:         %.8x%.8x\n", temp2, temp1);

    temp1 = enet_oam_stats.txfcl_counter;
    temp2 = enet_oam_stats.txfcl_counter >> 32;
    printf("txfcl_counter:         %.8x%.8x\n\n", temp2, temp1);
    
    return WP_OK;
}

/***************************************************************
 * Func name  : WPE_Eoam_LM_GetPMStats
 * Description: Show nearend and faredn frame loss.
 * Input      : 
 * OutPut     : 
 * Return Val :
 ***************************************************************/
WP_S32 WPE_Eoam_LM_GetLossRatio(WP_U32 ma_id, WP_U16 mep_id)
{
    WPE_EOAM_MA_T* pMaEntry = NULL;
    WPE_EOAM_RMMEP_T* pMepEntry = NULL;   

    
    pMaEntry = WPE_Eoam_Ma_FindEntryByMAID(ma_id);
    if (NULL == pMaEntry)
    {
        EOAM_DEBUG_LOG(WPE_DEBUG_ERROR, "Can't find MA ID %d .\n", ma_id);
        
        return 1;
    }
    
    pMepEntry = WPE_Eoam_Mep_FindEntryByMEPID(pMaEntry, mep_id);
    if (NULL == pMepEntry)
    {
        EOAM_DEBUG_LOG(WPE_DEBUG_ERROR, "Can't find MEP ID %d .\n", mep_id);
        
        return 2;
    }
    
    printf("MA ID = %6d,  MEP ID = %d\n", ma_id, mep_id);
    printf("frame_loss_farend  = %d\n", pMepEntry->lm_info.frame_loss_farend);
    printf("frame_loss_nearend = %d\n\n", pMepEntry->lm_info.frame_loss_nearend);
    
    return 0;
}

/***************************************************************
 * Func name  : WPE_Eoam_Dm_ComputeDmHeaderDelay
 * Description: Calculate delay.
 * Input      : 
 * OutPut     : 
 * Return Val :
 ***************************************************************/
WP_U64 WPE_Eoam_Dm_ComputeDmHeaderDelay(WPE_DM_HEADER_T* dm_header)
{
   WP_U64 before_ns = 0, after_ns = 0;
   WP_U64 diff_ns;
   WP_U32 i;
   WP_U32 secs = 0;

   WP_U64 one_sec = (1000*1000*1000);

   secs = dm_header->TxTimeStampfS;
   before_ns = (WP_U64)(dm_header->TxTimeStampfNS);

   for (i = 0; i < secs; i++)
   {
      before_ns += one_sec;
   }

   if (dm_header->Opcode == CFM_OPCODE_1DM)
   {
      secs = dm_header->RxTimeStampfS;
      after_ns = dm_header->RxTimeStampfNS;
   }
   
   if (dm_header->Opcode == CFM_OPCODE_DMR)
   {
      secs = dm_header->RxTimeStampbS;
      after_ns = dm_header->RxTimeStampbNS;
   }

   for (i = 0; i < secs; i++)
   {
      after_ns += one_sec;
   }

   diff_ns = (after_ns - before_ns);

    
   return diff_ns;
}

/***************************************************************
 * Func name  : WPE_Eoam_DM_EnableDM1
 * Description: 
 * Input      : 
 * OutPut     : 
 * Return Val :
 ***************************************************************/
WP_S32 WPE_Eoam_DM_EnableDM1(WP_U32 ma_id, WP_U16 mep_id, WP_U16 vlan, WP_U32 rate)
{
    WP_status status;
    WP_handle dm1_handle;
    WPE_EOAM_MA_T* pMaEntry = NULL;
    WPE_EOAM_RMMEP_T* pMepEntry = NULL;
    WPE_EOAM_MSG_T* pMsgEntry = NULL;
    WP_iw_enet_oam_msg_cfg msg_cfg;
    WP_iw_enet_oam_remote_mep mep_config;
    WP_command command = 0;
    
    pMaEntry = WPE_Eoam_Ma_FindEntryByMAID(ma_id);
    if (NULL == pMaEntry)
    {
        EOAM_DEBUG_LOG(WPE_DEBUG_ERROR, "Can't find MA ID %d .\n", ma_id);
        
        return 1;
    }
    
    pMepEntry = WPE_Eoam_Mep_FindEntryByMEPID(pMaEntry, mep_id);
    if (NULL == pMepEntry)
    {
        EOAM_DEBUG_LOG(WPE_DEBUG_ERROR, "Can't find MEP ID %d .\n", mep_id);
        
        return 2;
    }
    pMsgEntry = &pMepEntry->eoam_msg_table[WP_IW_ENET_OAM_MESSAGE_TYPE_1DM];
    if (EOAM_HANDLE_INVALID != pMsgEntry->msg_handle)
    {
        EOAM_DEBUG_LOG(WPE_DEBUG_ERROR, "DM1 already enabled.\n");
        
        return 3;
    }
    
    memcpy(&mep_config, &pMepEntry->mep_config, sizeof(mep_config));

    if (WP_IW_ENET_OAM_DM_DISABLE != mep_config.pm_params.dm_mode)
    {
        EOAM_DEBUG_LOG(WPE_DEBUG_ERROR, "DM already enabled.\n");
        
        return 4;
    }

    if (WP_IW_ENET_OAM_ONE_WAY_DM != mep_config.pm_params.dm_mode)
    {
        mep_config.pm_params.dm_mode = WP_IW_ENET_OAM_ONE_WAY_DM;
        command |= WP_IW_ENET_OAM_REMOTE_MEP_MOD_DM_MODE;
    }

    if (command)
    {
        status = WP_IwEnetOamRemoteMepModify(pMepEntry->mep_handle,
                                            command,
                                            &mep_config);
        WPE_TerminateOnError(status,"WP_IwEnetOamRemoteMepModify",__LINE__);
        memcpy(&pMepEntry->mep_config, &mep_config, sizeof(mep_config));        
    }
    
    if (WPE_Eoam_Mep_PmVlanEntryAdd(pMepEntry, vlan))
    {
        return 5;
    }
    
    msg_cfg.message_data.data = (WP_U8 *)malloc(sizeof(WP_U8) * EOAM_MSG_LENGTH);
    WPE_TerminateOnMallocFailed(msg_cfg.message_data.data, "Message buffer allocation");
    memset(msg_cfg.message_data.data, 0, EOAM_MSG_LENGTH);

    msg_cfg.md_level = pMaEntry->ma_config.maintenance_domain_level;
    msg_cfg.ignore_oam_lookup = WP_DISABLE;
    msg_cfg.freq_rate = WPE_Eoam_IndexToFreqRate(rate);
    msg_cfg.message_type = WP_IW_ENET_OAM_MESSAGE_TYPE_1DM;

    msg_cfg.message_data.cfm_offset = CFM_OFFSET;
    msg_cfg.message_data.data_length = CFM_OFFSET + CFM_1DM_SIZE;

    msg_cfg.type.dm_msg.remote_mep_handle = pMepEntry->mep_handle;
    
    dm1_handle = WPE_Eoam_Msg_CreateMsg(pMaEntry, pMepEntry, &msg_cfg, vlan);
    if (EOAM_HANDLE_INVALID == dm1_handle)
    {
        if (msg_cfg.message_data.data)
        {
            free(msg_cfg.message_data.data);
        }          
        return 4;
    }

    status = WP_IwEnetOamMessageEnable(dm1_handle);
    WPE_TerminateOnError(status, "WP_IwEnetOamMessageEnable()",__LINE__);
    memset(&pMepEntry->dm_info, 0, sizeof(pMepEntry->dm_info));
    memcpy(&pMsgEntry->cfg, &msg_cfg, sizeof(WP_iw_enet_oam_msg_cfg));
    pMsgEntry->msg_handle = dm1_handle;
    pMsgEntry->svlan = vlan;

    return WP_OK;

}

/***************************************************************
 * Func name  : WPE_Eoam_DM_DisableDM1
 * Description: 
 * Input      : 
 * OutPut     : 
 * Return Val :
 ***************************************************************/
WP_S32 WPE_Eoam_DM_DisableDM1(WP_U32 ma_id, WP_U16 mep_id, WP_U16 vlan)
{
    WP_status status;

    WPE_EOAM_MA_T* pMaEntry = NULL;
    WPE_EOAM_RMMEP_T* pMepEntry = NULL;
    WPE_EOAM_MSG_T* pMsgEntry = NULL;

    WP_iw_enet_oam_remote_mep mep_config;
    WP_command command = 0;
    
    pMaEntry = WPE_Eoam_Ma_FindEntryByMAID(ma_id);
    if (NULL == pMaEntry)
    {
        EOAM_DEBUG_LOG(WPE_DEBUG_ERROR, "Can't find MA ID %d .\n", ma_id);
        
        return 1;
    }
    
    pMepEntry = WPE_Eoam_Mep_FindEntryByMEPID(pMaEntry, mep_id);
    if (NULL == pMepEntry)
    {
        EOAM_DEBUG_LOG(WPE_DEBUG_ERROR, "Can't find MEP ID %d .\n", mep_id);
        
        return 2;
    }
    
    pMsgEntry = &pMepEntry->eoam_msg_table[WP_IW_ENET_OAM_MESSAGE_TYPE_1DM];
    if (EOAM_HANDLE_INVALID == pMsgEntry->msg_handle)
    {
        EOAM_DEBUG_LOG(WPE_DEBUG_ERROR, "DM1 already disabled.\n");
        
        return 3;
    }

    if (vlan != pMsgEntry->svlan)
    {
        EOAM_DEBUG_LOG(WPE_DEBUG_ERROR, "Vlan not match!\n");
        return 4;
    }
    memcpy(&mep_config, &pMepEntry->mep_config, sizeof(mep_config));

    if (WP_IW_ENET_OAM_ONE_WAY_DM != mep_config.pm_params.dm_mode)
    {
        EOAM_DEBUG_LOG(WPE_DEBUG_ERROR, "Not in DM1 state.\n");
        
        return 4;
    }
    
    if (WP_IW_ENET_OAM_DM_DISABLE != mep_config.pm_params.dm_mode)
    {
        mep_config.pm_params.dm_mode = WP_IW_ENET_OAM_DM_DISABLE;
        command |= WP_IW_ENET_OAM_REMOTE_MEP_MOD_DM_MODE;
    }

    if (command)
    {
        status = WP_IwEnetOamRemoteMepModify(pMepEntry->mep_handle,
                                            command,
                                            &mep_config);
        WPE_TerminateOnError(status,"WP_IwEnetOamRemoteMepModify",__LINE__);
        memcpy(&pMepEntry->mep_config, &mep_config, sizeof(mep_config));        
    }    
    
    status = WP_IwEnetOamMessageDisable(pMsgEntry->msg_handle);
    WPE_TerminateOnError(status, "WP_IwEnetOamMessageDisable()", __LINE__);
    
    status = WP_IwEnetOamMessageDelete(pMsgEntry->msg_handle);
    WPE_TerminateOnError(status, "WP_IwEnetOamMessageDelete()", __LINE__);    

    WPE_Eoam_Msg_EntryInit(pMsgEntry);
    
    return WP_OK;

}

/***************************************************************
 * Func name  : WPE_Eoam_DM_EnableDMM
 * Description: 
 * Input      : 
 * OutPut     : 
 * Return Val :
 ***************************************************************/
WP_S32 WPE_Eoam_DM_EnableDMM(WP_U32 ma_id, WP_U16 mep_id, WP_U16 vlan, WP_U32 rate)
{
    WP_status status;
    WP_handle dmm_handle;
    WPE_EOAM_MA_T* pMaEntry = NULL;
    WPE_EOAM_RMMEP_T* pMepEntry = NULL;
    WPE_EOAM_MSG_T* pMsgEntry = NULL;
    WP_iw_enet_oam_msg_cfg msg_cfg;
    WP_iw_enet_oam_remote_mep mep_config;
    WP_command command = 0;

    
    pMaEntry = WPE_Eoam_Ma_FindEntryByMAID(ma_id);
    if (NULL == pMaEntry)
    {
        EOAM_DEBUG_LOG(WPE_DEBUG_ERROR, "Can't find MA ID %d .\n", ma_id);
        
        return 1;
    }
    
    pMepEntry = WPE_Eoam_Mep_FindEntryByMEPID(pMaEntry, mep_id);
    if (NULL == pMepEntry)
    {
        EOAM_DEBUG_LOG(WPE_DEBUG_ERROR, "Can't find MEP ID %d .\n", mep_id);
        
        return 2;
    }
    
    pMsgEntry = &pMepEntry->eoam_msg_table[WP_IW_ENET_OAM_MESSAGE_TYPE_DMM];
    if (EOAM_HANDLE_INVALID != pMsgEntry->msg_handle)
    {
        EOAM_DEBUG_LOG(WPE_DEBUG_ERROR, "DMM already enabled.\n");
        
        return 3;
    }
    
    memcpy(&mep_config, &pMepEntry->mep_config, sizeof(mep_config));
    if (WP_IW_ENET_OAM_DM_DISABLE != mep_config.pm_params.dm_mode)
    {
        EOAM_DEBUG_LOG(WPE_DEBUG_ERROR, "DM already enabled.\n");
        
        return 4;
    }
    
    if (WP_IW_ENET_OAM_TWO_WAY_DM != mep_config.pm_params.dm_mode)
    {
        mep_config.pm_params.dm_mode = WP_IW_ENET_OAM_TWO_WAY_DM;
        command |= WP_IW_ENET_OAM_REMOTE_MEP_MOD_DM_MODE;
    }

    if (command)
    {
        status = WP_IwEnetOamRemoteMepModify(pMepEntry->mep_handle,
                                            command,
                                            &mep_config);
        WPE_TerminateOnError(status,"WP_IwEnetOamRemoteMepModify",__LINE__);
        memcpy(&pMepEntry->mep_config, &mep_config, sizeof(mep_config));        
    }
    
    if (WPE_Eoam_Mep_PmVlanEntryAdd(pMepEntry, vlan))
    {
        return 5;
    }
    
    msg_cfg.message_data.data = (WP_U8 *)malloc(sizeof(WP_U8) * EOAM_MSG_LENGTH);
    WPE_TerminateOnMallocFailed(msg_cfg.message_data.data, "Message buffer allocation");
    memset(msg_cfg.message_data.data, 0, EOAM_MSG_LENGTH);

    msg_cfg.md_level = pMaEntry->ma_config.maintenance_domain_level;
    msg_cfg.ignore_oam_lookup = WP_DISABLE;
    msg_cfg.freq_rate = WPE_Eoam_IndexToFreqRate(rate);
    msg_cfg.message_type = WP_IW_ENET_OAM_MESSAGE_TYPE_DMM;

    msg_cfg.message_data.cfm_offset = CFM_OFFSET;
    msg_cfg.message_data.data_length = CFM_OFFSET + CFM_DMM_SIZE;

    msg_cfg.type.dm_msg.remote_mep_handle = pMepEntry->mep_handle;
    
    dmm_handle = WPE_Eoam_Msg_CreateMsg(pMaEntry, pMepEntry, &msg_cfg, vlan);
    if (EOAM_HANDLE_INVALID == dmm_handle)
    {
        if (msg_cfg.message_data.data)
        {
            free(msg_cfg.message_data.data);
        }          
        return 4;
    }
    
    status = WP_IwEnetOamMessageEnable(dmm_handle);
    WPE_TerminateOnError(status, "WP_IwEnetOamMessageEnable()",__LINE__);
    memset(&pMepEntry->dm_info, 0, sizeof(pMepEntry->dm_info));
    memcpy(&pMsgEntry->cfg, &msg_cfg, sizeof(WP_iw_enet_oam_msg_cfg));
    pMsgEntry->msg_handle = dmm_handle;
    pMsgEntry->svlan = vlan; 
    
    return WP_OK;

}

/***************************************************************
 * Func name  : WPE_Eoam_DM_DisableDMM
 * Description: 
 * Input      : 
 * OutPut     : 
 * Return Val :
 ***************************************************************/
WP_S32 WPE_Eoam_DM_DisableDMM(WP_U32 ma_id, WP_U16 mep_id, WP_U16 vlan)
{
    WP_status status;
    WPE_EOAM_MA_T* pMaEntry = NULL;
    WPE_EOAM_RMMEP_T* pMepEntry = NULL;
    WPE_EOAM_MSG_T* pMsgEntry = NULL;

    WP_iw_enet_oam_remote_mep mep_config;
    WP_command command = 0;

    
    pMaEntry = WPE_Eoam_Ma_FindEntryByMAID(ma_id);
    if (NULL == pMaEntry)
    {
        EOAM_DEBUG_LOG(WPE_DEBUG_ERROR, "Can't find MA ID %d .\n", ma_id);
        
        return 1;
    }
    
    pMepEntry = WPE_Eoam_Mep_FindEntryByMEPID(pMaEntry, mep_id);
    if (NULL == pMepEntry)
    {
        EOAM_DEBUG_LOG(WPE_DEBUG_ERROR, "Can't find MEP ID %d .\n", mep_id);
        
        return 2;
    }
    pMsgEntry = &pMepEntry->eoam_msg_table[WP_IW_ENET_OAM_MESSAGE_TYPE_DMM];
    if (EOAM_HANDLE_INVALID == pMsgEntry->msg_handle)
    {
        EOAM_DEBUG_LOG(WPE_DEBUG_ERROR, "DMM already disabled.\n");
        
        return 3;
    }

    if (vlan != pMsgEntry->svlan)
    {
        EOAM_DEBUG_LOG(WPE_DEBUG_ERROR, "Vlan not match!\n");
        return 4;
    }
    
    memcpy(&mep_config, &pMepEntry->mep_config, sizeof(mep_config));
    if (WP_IW_ENET_OAM_TWO_WAY_DM != mep_config.pm_params.dm_mode)
    {
        EOAM_DEBUG_LOG(WPE_DEBUG_ERROR, "Not in two way DM state.\n");
        
        return 4;
    }
    
    if (WP_IW_ENET_OAM_DM_DISABLE != mep_config.pm_params.dm_mode)
    {
        mep_config.pm_params.dm_mode = WP_IW_ENET_OAM_DM_DISABLE;
        command |= WP_IW_ENET_OAM_REMOTE_MEP_MOD_DM_MODE;
    }
    
    if (command)
    {
        status = WP_IwEnetOamRemoteMepModify(pMepEntry->mep_handle,
                                            command,
                                            &mep_config);
        WPE_TerminateOnError(status,"WP_IwEnetOamRemoteMepModify",__LINE__);
        memcpy(&pMepEntry->mep_config, &mep_config, sizeof(mep_config));        
    }    

    status = WP_IwEnetOamMessageDisable(pMsgEntry->msg_handle);
    WPE_TerminateOnError(status, "WP_IwEnetOamMessageDisable()" , __LINE__);
    
    status = WP_IwEnetOamMessageDelete(pMsgEntry->msg_handle);
    WPE_TerminateOnError(status, "WP_IwEnetOamMessageDelete()" , __LINE__);    

    WPE_Eoam_Msg_EntryInit(pMsgEntry);
    
    return WP_OK;
    
}

/***************************************************************
 * Func name  : WPE_Eoam_DM_GetDMCounters
 * Description: 
 * Input      : 
 * OutPut     : 
 * Return Val :
 ***************************************************************/
WP_S32 WPE_Eoam_DM_GetDMCounters(WP_U32 ma_id, WP_U16 mep_id)
{
    WPE_EOAM_MA_T* pMaEntry = NULL;
    WPE_EOAM_RMMEP_T* pMepEntry = NULL;
    WP_U32 temp1, temp2;

    
    pMaEntry = WPE_Eoam_Ma_FindEntryByMAID(ma_id);
    if (NULL == pMaEntry)
    {
        EOAM_DEBUG_LOG(WPE_DEBUG_ERROR, "Can't find MA ID %d .\n", ma_id);
        
        return 1;
    }
    
    pMepEntry = WPE_Eoam_Mep_FindEntryByMEPID(pMaEntry, mep_id);
    if (NULL == pMepEntry)
    {
        EOAM_DEBUG_LOG(WPE_DEBUG_ERROR, "Can't find MEP ID %d .\n", mep_id);
        
        return 2;
    }
    
    printf("MA ID= %d, MEP ID= %d\n", ma_id, mep_id);
    temp1 = pMepEntry->dm_info.dm_counter;
    temp2 = pMepEntry->dm_info.dm_counter >> 32;    
    printf("dm counter :        %.8x%.8x\n\n", temp2, temp1);
    
    return WP_OK;
    
}

/***************************************************************
 * Func name  : WPE_Eoam_DM_CalculateDelay
 * Description: 
 * Input      : 
 * OutPut     : 
 * Return Val :
 ***************************************************************/
WP_S32 WPE_Eoam_DM_CalculateDelay(WP_U32 ma_id, WP_U16 mep_id)
{
    WPE_EOAM_MA_T* pMaEntry = NULL;
    WPE_EOAM_RMMEP_T* pMepEntry = NULL;
    WPE_TIME_T time_data;
    
    pMaEntry = WPE_Eoam_Ma_FindEntryByMAID(ma_id);
    if (NULL == pMaEntry)
    {
        EOAM_DEBUG_LOG(WPE_DEBUG_ERROR, "Can't find MA ID %d .\n", ma_id);
        
        return 1;
    }
    
    pMepEntry = WPE_Eoam_Mep_FindEntryByMEPID(pMaEntry, mep_id);
    if (NULL == pMepEntry)
    {
        EOAM_DEBUG_LOG(WPE_DEBUG_ERROR, "Can't find MEP ID %d .\n", mep_id);
        
        return 2;
    }

    printf("MA ID= %d, MEP ID= %d\n", ma_id, mep_id);

    WPE_Eoam_ConvertDelayToTime(pMepEntry->dm_info.mean_delay, &time_data);
    printf("Mean delay       = %u.%03u,%03u,%03u\n", 
                time_data.secs, time_data.milisecs, time_data.microsecs, time_data.nanosecs);

    WPE_Eoam_ConvertDelayToTime(pMepEntry->dm_info.max_delay, &time_data);
    printf("Max  delay       = %u.%03u,%03u,%03u\n", 
                time_data.secs, time_data.milisecs, time_data.microsecs, time_data.nanosecs);

    WPE_Eoam_ConvertDelayToTime(pMepEntry->dm_info.last_delay, &time_data);
    printf("Last delay       = %u.%03u,%03u,%03u\n", 
                time_data.secs, time_data.milisecs, time_data.microsecs, time_data.nanosecs);
    
    return WP_OK;
}

/***************************************************************
 * Func name  : WPE_Eoam_Flow_FindEntry
 * Description: 
 * Input      : 
 * OutPut     : 
 * Return Val :
 ***************************************************************/
WPE_EOAM_FLOW_T* WPE_Eoam_Flow_FindEntry(WPE_EOAM_RMMEP_T* pMepEntry, 
                                        WP_boolean ul_dl, 
                                        WP_U8 s_mac[6], 
                                        WP_U8 d_mac[6], 
                                        WP_U16 vlan)
{
    WPE_EOAM_FLOW_T* pFlow = NULL;
    WPE_EOAM_FLOW_PARAM_T param;
    int ii, max;
    param.vlan = vlan;
    memcpy(param.d_mac, d_mac, 6);
    memcpy(param.s_mac, s_mac, 6);
    if (ul_dl)
    {
        pFlow = pMepEntry->ul_flow;
        max = EOAM_MAX_ULFLOW_PER_MEP;
    }
    else
    {
        pFlow = pMepEntry->dl_flow;
        max = EOAM_MAX_DLFLOW_PER_MEP;
    }
    
    for (ii = 0; ii < max; ii++, pFlow++)
    {
        if (0 == memcmp(&param, &(pFlow->param), sizeof(param)))
        {
            return pFlow;
        }
    }

    return NULL;
}

WPE_EOAM_FLOW_T* WPE_Eoam_Flow_GetEmptyEntry(WPE_EOAM_RMMEP_T* pMepEntry, WP_boolean ul_dl)
{
    WPE_EOAM_FLOW_T* pFlow = NULL;
    int ii, max;

    if (ul_dl)
    {
        pFlow = pMepEntry->ul_flow;
        max = EOAM_MAX_ULFLOW_PER_MEP;
    }
    else
    {
        pFlow = pMepEntry->dl_flow;
        max = EOAM_MAX_DLFLOW_PER_MEP;
    }
    
    for (ii = 0; ii < max; ii++)
    {
        if (EOAM_HANDLE_INVALID == pFlow->handle)
        {
            return pFlow;
        }
    }

    return NULL;
}


WP_handle WPE_Eoam_Flow_AddFlowRule(WP_handle filter,
                                    WP_handle iw_port,
                                    WP_handle flow_aggregation,
                                    WP_handle lm_stats_handle,
                                    WP_U8 type,
                                    WP_U8 s_mac[6],
                                    WP_U8 d_mac[6],
                                    WP_U16 vlan,
                                    WP_U8 md_level)
{
   WP_pce_rule_classification rule_cfg = { 0 };
   WP_U16 i;
   WP_handle pce_rule;

   /* Common parameters */
   rule_cfg.enabled = WP_ENABLE;
   rule_cfg.filter_handle = filter;
   
   rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_MAC_DA;
   for(i=0; i<6; i++)      
   {
   rule_cfg.rule_fields[0].value.mac_addr[i] = d_mac[i];
   }
   
   rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_MAC_SA;
   for(i=0; i<6; i++)      
   {
   rule_cfg.rule_fields[1].value.mac_addr[i] = s_mac[i];
   }
  
   rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_INT_VLAN_TAG;
   rule_cfg.rule_fields[2].value.vlan_tag = vlan;

   rule_cfg.rule_fields[3].field_id = WP_PCE_FIELD_ID_INPUT_IW_PORT;
   rule_cfg.rule_fields[3].value.iw_port_handle = iw_port;
   
   rule_cfg.rule_fields[4].field_id = WP_PCE_FIELD_ID_LAST;
   rule_cfg.match_action = WP_PCE_RULE_MATCH_ACCEPT;
   
   switch (type)
   {
      case WPE_EXT_ACTION_FLOW_AGG:
      {
         rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
         rule_cfg.match_result[0].param.flow_agg.flow_aggregation = flow_aggregation;
         rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;   
      }
      break;
      
      case WPE_EXT_ACTION_RXFCL:
      {
         rule_cfg.match_result[0].result_type = WP_PCE_RESULT_STATISTICS;
         rule_cfg.match_result[0].param.statistics.subtype = WP_PCE_STATISTICS_SUBTYPE_LM_UPDATE_RXFCL;
         rule_cfg.match_result[0].param.statistics.statistics_handle = lm_stats_handle;
         rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;  
      }
      break;

      case WPE_EXT_ACTION_RXFCL_FLOW_AGG:
      {
         rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
         rule_cfg.match_result[0].param.flow_agg.flow_aggregation = flow_aggregation;

         rule_cfg.match_result[1].result_type = WP_PCE_RESULT_STATISTICS;
         rule_cfg.match_result[1].param.statistics.subtype = WP_PCE_STATISTICS_SUBTYPE_LM_UPDATE_RXFCL;
         rule_cfg.match_result[1].param.statistics.statistics_handle = lm_stats_handle;

         rule_cfg.match_result[2].result_type = WP_PCE_RESULT_LAST;  
      }
      break;

      case WPE_EXT_ACTION_TXFCL:
      {
         rule_cfg.match_result[0].result_type = WP_PCE_RESULT_STATISTICS;
         rule_cfg.match_result[0].param.statistics.subtype = WP_PCE_STATISTICS_SUBTYPE_LM_UPDATE_TXFCL;
         rule_cfg.match_result[0].param.statistics.statistics_handle = lm_stats_handle;
         rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;  
      }
      break;

      case WPE_EXT_ACTION_TXFCL_FLOW_AGG:
      {
         rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
         rule_cfg.match_result[0].param.flow_agg.flow_aggregation = flow_aggregation;

         rule_cfg.match_result[1].result_type = WP_PCE_RESULT_STATISTICS;
         rule_cfg.match_result[1].param.statistics.subtype = WP_PCE_STATISTICS_SUBTYPE_LM_UPDATE_TXFCL;
         rule_cfg.match_result[1].param.statistics.statistics_handle = lm_stats_handle;
         rule_cfg.match_result[2].result_type = WP_PCE_RESULT_LAST;  
      }
      break;

      default:
          return EOAM_HANDLE_INVALID;
          break;
    }

   pce_rule = WP_PceRuleCreate (WP_WINPATH (0),WP_PCE_RULE_CLASSIFICATION, &rule_cfg);
   WPE_TerminateOnError(pce_rule,"WP_PceRuleCreate()",__LINE__);
  
   return pce_rule;
}

WP_status WPE_Eoam_FlowRuleModify( WP_handle flow_handle, 
                                    WP_handle flow_aggregation,
                                    WP_handle lm_stats_handle, 
                                    WP_U8 type, 
                                    WP_U8 md_level)
{

   WP_status status;
   WP_pce_rule_classification rule_cfg = { 0 };

   switch (type)
   {
      case WPE_EXT_ACTION_FLOW_AGG:
      {
         rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
         rule_cfg.match_result[0].param.flow_agg.flow_aggregation = flow_aggregation;
         rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;   
      }
      break;
      
      case WPE_EXT_ACTION_RXFCL:
      {
         rule_cfg.match_result[0].result_type = WP_PCE_RESULT_STATISTICS;
         rule_cfg.match_result[0].param.statistics.subtype = WP_PCE_STATISTICS_SUBTYPE_LM_UPDATE_RXFCL;
         rule_cfg.match_result[0].param.statistics.statistics_handle = lm_stats_handle;
         rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;  
      }
      break;

      case WPE_EXT_ACTION_RXFCL_FLOW_AGG:
      {
         rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
         rule_cfg.match_result[0].param.flow_agg.flow_aggregation = flow_aggregation;

         rule_cfg.match_result[1].result_type = WP_PCE_RESULT_STATISTICS;
         rule_cfg.match_result[1].param.statistics.subtype = WP_PCE_STATISTICS_SUBTYPE_LM_UPDATE_RXFCL;
         rule_cfg.match_result[1].param.statistics.statistics_handle = lm_stats_handle;

         rule_cfg.match_result[2].result_type = WP_PCE_RESULT_LAST;  
      }
      break;

      case WPE_EXT_ACTION_TXFCL:
      {
         rule_cfg.match_result[0].result_type = WP_PCE_RESULT_STATISTICS;
         rule_cfg.match_result[0].param.statistics.subtype = WP_PCE_STATISTICS_SUBTYPE_LM_UPDATE_TXFCL;
         rule_cfg.match_result[0].param.statistics.statistics_handle = lm_stats_handle;
         rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;  
      }
      break;

      case WPE_EXT_ACTION_TXFCL_FLOW_AGG:
      {
         rule_cfg.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
         rule_cfg.match_result[0].param.flow_agg.flow_aggregation = flow_aggregation;

         rule_cfg.match_result[1].result_type = WP_PCE_RESULT_STATISTICS;
         rule_cfg.match_result[1].param.statistics.subtype = WP_PCE_STATISTICS_SUBTYPE_LM_UPDATE_TXFCL;
         rule_cfg.match_result[1].param.statistics.statistics_handle = lm_stats_handle;
         rule_cfg.match_result[2].result_type = WP_PCE_RESULT_LAST;  
      }
      break;

      default:
         return (WP_ERR_CONFIG);
         break;
   }

   status = WP_PceRuleModify(flow_handle, WP_PCE_RULE_MODIFY_COMMAND_RESULT, &rule_cfg);
   return status;
}


/***************************************************************
 * Func name  : WPE_Eoam_FLOW_ShowInfo
 * Description: Show all valid flows
 * Input      : 
 * OutPut     : 
 * Return Val :
 ***************************************************************/
WP_S32 WPE_Eoam_FLOW_ShowInfo(void)
{
    WP_U8 ii, jj, index, flag;
    WPE_EOAM_FLOW_T* pFlowEntry = NULL;
    WPE_EOAM_MA_T* pMaEntry = NULL;
    WPE_EOAM_RMMEP_T* pEntry = NULL;

    
    pMaEntry = eoam_system.eoam_ma_table;
    printf("-------------------------------------------------------------------------------------\n");
    printf("| MA ID  | MEP ID |ul|  handle   |en| vlan |b|       s-mac       |       d-mac\n");
    printf("-------------------------------------------------------------------------------------\n");
    for (jj = 0; jj < EOAM_MAX_MA_NUM; jj++, pMaEntry++)
    {
        if (EOAM_HANDLE_INVALID == pMaEntry->ma_handle)
        {
            continue;
        }
        flag = 0;
        pEntry = pMaEntry->eoam_mep_table;
        for (ii = 0; ii < EOAM_MEP_NUM_PER_MA; ii++, pEntry++)
        {
            if (EOAM_HANDLE_INVALID == pEntry->mep_handle)
            {
                continue;
            }
            
            pFlowEntry = pEntry->ul_flow;
            for (index = 0; index < EOAM_MAX_ULFLOW_PER_MEP; index++, pFlowEntry++)
            {
                if (EOAM_HANDLE_INVALID == pFlowEntry->handle)
                {
                    continue;
                }
                flag++;
                printf(" %8d  %6d  1  0x%8x  %d   %4d  %1d  %02x:%02x:%02x:%02x:%02x:%02x   %02x:%02x:%02x:%02x:%02x:%02x\n",
                    pMaEntry->ma_config.ma_id[0],
                    pEntry->mep_config.remote_mep_id,                
                    pFlowEntry->handle,
                    pFlowEntry->enable,
                    pFlowEntry->param.vlan,
                    pFlowEntry->backupline,
                    pFlowEntry->param.s_mac[0], pFlowEntry->param.s_mac[1], pFlowEntry->param.s_mac[2],
                    pFlowEntry->param.s_mac[3], pFlowEntry->param.s_mac[4], pFlowEntry->param.s_mac[5],
                    pFlowEntry->param.d_mac[0], pFlowEntry->param.d_mac[1], pFlowEntry->param.d_mac[2],
                    pFlowEntry->param.d_mac[3], pFlowEntry->param.d_mac[4], pFlowEntry->param.d_mac[5]);
            }

            pFlowEntry = pEntry->dl_flow;
            for (index = 0; index < EOAM_MAX_DLFLOW_PER_MEP; index++, pFlowEntry++)
            {
                if (EOAM_HANDLE_INVALID == pFlowEntry->handle)
                {
                    continue;
                }
                flag++;
                printf(" %8d  %6d  0  0x%8x  %d   %4d  %1d  %02x:%02x:%02x:%02x:%02x:%02x   %02x:%02x:%02x:%02x:%02x:%02x\n",
                    pMaEntry->ma_config.ma_id[0],
                    pEntry->mep_config.remote_mep_id,                
                    pFlowEntry->handle,
                    pFlowEntry->enable,
                    pFlowEntry->param.vlan,
                    pFlowEntry->backupline,
                    pFlowEntry->param.s_mac[0], pFlowEntry->param.s_mac[1], pFlowEntry->param.s_mac[2],
                    pFlowEntry->param.s_mac[3], pFlowEntry->param.s_mac[4], pFlowEntry->param.s_mac[5],
                    pFlowEntry->param.d_mac[0], pFlowEntry->param.d_mac[1], pFlowEntry->param.d_mac[2],
                    pFlowEntry->param.d_mac[3], pFlowEntry->param.d_mac[4], pFlowEntry->param.d_mac[5]);
            }

        }
        if (flag)
        {
            printf("\n");
        }
    }
    
    printf("\n");
    
    return WP_OK;
}

/*
create DL FLOW - smac=1 dmac=2 vlan=10 MAID=1 MEPID=1
Create UL flow = smac=1 dmac=2 vlan=10 MAID=2 MEPID=2

create UL Flow - smac=2 dmac=1 vlan=10 MAID=1 MEPID=1
create DL flow = smac=2 dmac=1 vlan=10 MAID=2 MEPID=2
*/
WP_S32 WPE_Eoam_FLOW_CreateFlow(WP_boolean ul_dl, WP_U8 s_mac[6], WP_U8 d_mac[6], WP_U16 vlan, WP_U32 ma_id, WP_U16 mep_id)
{
    WP_U8 type;
    WP_handle filter;
    WP_handle iw_port;
    WP_handle flow_aggregation;
    WP_handle flow_handle;
    WPE_EOAM_MA_T* pMaEntry = NULL;
    WPE_EOAM_RMMEP_T* pMepEntry = NULL;
    WPE_EOAM_FLOW_T* pFlowEntry = NULL;

    pMaEntry = WPE_Eoam_Ma_FindEntryByMAID(ma_id);
    if (NULL == pMaEntry)
    {
        EOAM_DEBUG_LOG(WPE_DEBUG_ERROR, "Can't find MA ID %d .\n", ma_id);
        
        return 1;
    }
    
    pMepEntry = WPE_Eoam_Mep_FindEntryByMEPID(pMaEntry, mep_id);
    if (NULL == pMepEntry)
    {
        EOAM_DEBUG_LOG(WPE_DEBUG_ERROR, "Can't find MEP ID %d .\n", mep_id);
        
        return 2;
    }

    pFlowEntry = WPE_Eoam_Flow_FindEntry(pMepEntry, ul_dl, s_mac, d_mac, vlan);
    if (pFlowEntry)
    {
        EOAM_DEBUG_LOG(WPE_DEBUG_ERROR, "Flow already exist.\n");
        
        return 3;
    }

    pFlowEntry = WPE_Eoam_Flow_GetEmptyEntry(pMepEntry, ul_dl);
    if (NULL == pFlowEntry)
    {
       EOAM_DEBUG_LOG(WPE_DEBUG_ERROR, "Flow table full.\n");
        
        return 4;
    }    

    type = ul_dl ? WPE_EXT_ACTION_RXFCL_FLOW_AGG : WPE_EXT_ACTION_TXFCL_FLOW_AGG;
    filter =  eoam_system.h_pce_filter[0];
    iw_port = ul_dl ? eoam_system.bports[ENET2_BPORT] : eoam_system.bports[ENET1_BPORT];
    flow_aggregation = ul_dl ? eoam_system.flow_agg[FA_PORT_ENET1] : eoam_system.flow_agg[FA_PORT_ENET2]; 
    
    flow_handle = WPE_Eoam_Flow_AddFlowRule(filter, iw_port, flow_aggregation, 
                    pMepEntry->mep_config.rxtxfcl_stats_handle, 
                    type, s_mac, d_mac, vlan,
                    pMaEntry->ma_config.maintenance_domain_level);

    if (EOAM_HANDLE_INVALID == flow_handle)
    {        
        return 5;
    }

    pFlowEntry->handle = flow_handle;
    pFlowEntry->flow_agg = flow_aggregation;
    pFlowEntry->enable = WP_TRUE;
    pFlowEntry->param.vlan   = vlan;    
    memcpy(pFlowEntry->param.s_mac, s_mac, 6);
    memcpy(pFlowEntry->param.d_mac, d_mac, 6);
    
    return WP_OK;
}

/***************************************************************
 * Func name  : WPE_Eoam_FLOW_DeleteFlow
 * Description: 
 * Input      : 
 * OutPut     : 
 * Return Val :
 ***************************************************************/
WP_S32 WPE_Eoam_FLOW_DeleteFlow(WP_boolean ul_dl, WP_U8 s_mac[6], WP_U8 d_mac[6], WP_U16 vlan, WP_U32 ma_id, WP_U16 mep_id)
{
    WP_status status;
    WPE_EOAM_MA_T* pMaEntry = NULL;
    WPE_EOAM_RMMEP_T* pMepEntry = NULL;
    WPE_EOAM_FLOW_T* pFlowEntry = NULL;
    
    pMaEntry = WPE_Eoam_Ma_FindEntryByMAID(ma_id);
    if (NULL == pMaEntry)
    {
       EOAM_DEBUG_LOG(WPE_DEBUG_ERROR, "Can't find MA ID %d .\n", ma_id);
        
        return 1;
    }
    
    pMepEntry = WPE_Eoam_Mep_FindEntryByMEPID(pMaEntry, mep_id);
    if (NULL == pMepEntry)
    {
        EOAM_DEBUG_LOG(WPE_DEBUG_ERROR, "Can't find MEP ID %d .\n", mep_id);
        
        return 2;
    }

    pFlowEntry = WPE_Eoam_Flow_FindEntry(pMepEntry, ul_dl, s_mac, d_mac, vlan);
    if ((NULL == pFlowEntry) 
        || (EOAM_HANDLE_INVALID == pFlowEntry->handle))
    {
        
        return 3;
    }

    status = WP_PceRuleDelete (pFlowEntry->handle);
    WPE_TerminateOnError(status, "WP_PceRuleDelete",__LINE__);
    memset(pFlowEntry, 0, sizeof(WPE_EOAM_FLOW_T));
    
    return WP_OK;
}

/***************************************************************
 * Func name  : WPE_Eoam_FLOW_EnableTXFCLCount
 * Description: 
 * Input      : 
 * OutPut     : 
 * Return Val :
 ***************************************************************/
WP_S32 WPE_Eoam_FLOW_EnableTXFCLCount(WP_U8 s_mac[6], WP_U8 d_mac[6], WP_U16 vlan, WP_U32 ma_id, WP_U16 mep_id)
{
    WP_status status;
    WPE_EOAM_MA_T* pMaEntry = NULL;
    WPE_EOAM_RMMEP_T* pMepEntry = NULL;
    WPE_EOAM_FLOW_T* pFlowEntry = NULL;    

    pMaEntry = WPE_Eoam_Ma_FindEntryByMAID(ma_id);
    if (NULL == pMaEntry)
    {
        EOAM_DEBUG_LOG(WPE_DEBUG_ERROR, "Can't find MA ID %d .\n", ma_id);
        
        return 1;
    }
    
    pMepEntry = WPE_Eoam_Mep_FindEntryByMEPID(pMaEntry, mep_id);
    if (NULL == pMepEntry)
    {
        EOAM_DEBUG_LOG(WPE_DEBUG_ERROR, "Can't find MEP ID %d .\n", mep_id);
        
        return 2;
    }

    pFlowEntry = WPE_Eoam_Flow_FindEntry(pMepEntry, WP_FALSE, s_mac, d_mac, vlan);
    if (NULL == pFlowEntry)
    {
        EOAM_DEBUG_LOG(WPE_DEBUG_ERROR, "Can't find such flow.\n");
        
        return 3;
    }
    
    if (WP_TRUE == pFlowEntry->enable)
    {
        
        return 4;
    }

    status = WPE_Eoam_FlowRuleModify(pFlowEntry->handle,
                    pFlowEntry->flow_agg,
                    pMepEntry->mep_config.rxtxfcl_stats_handle,
                    WPE_EXT_ACTION_TXFCL_FLOW_AGG,
                    pMaEntry->ma_config.maintenance_domain_level);
    WPE_TerminateOnError(status, "WPE_Eoam_FLOW_ModifyFlowRule",__LINE__);
    
    pFlowEntry->enable = WP_TRUE;
    
    return WP_OK;
}

/***************************************************************
 * Func name  : WPE_Eoam_FLOW_DisableTXFCLCount
 * Description: 
 * Input      : 
 * OutPut     : 
 * Return Val :
 ***************************************************************/
WP_S32 WPE_Eoam_FLOW_DisableTXFCLCount(WP_U8 s_mac[6], WP_U8 d_mac[6], WP_U16 vlan, WP_U32 ma_id, WP_U16 mep_id)
{
    WP_status status;
    WPE_EOAM_MA_T* pMaEntry = NULL;
    WPE_EOAM_RMMEP_T* pMepEntry = NULL;
    WPE_EOAM_FLOW_T* pFlowEntry = NULL;
    

    pMaEntry = WPE_Eoam_Ma_FindEntryByMAID(ma_id);
    if (NULL == pMaEntry)
    {
        EOAM_DEBUG_LOG(WPE_DEBUG_ERROR, "Can't find MA ID %d .\n", ma_id);
        
        return 1;
    }
    
    pMepEntry = WPE_Eoam_Mep_FindEntryByMEPID(pMaEntry, mep_id);
    if (NULL == pMepEntry)
    {
        EOAM_DEBUG_LOG(WPE_DEBUG_ERROR, "Can't find MEP ID %d .\n", mep_id);
        
        return 2;
    }

    pFlowEntry = WPE_Eoam_Flow_FindEntry(pMepEntry, WP_FALSE, s_mac, d_mac, vlan);
    if (NULL == pFlowEntry)
    {
        EOAM_DEBUG_LOG(WPE_DEBUG_ERROR, "Can't find such flow.\n");
        
        return 3;
    }
    
    if (WP_FALSE == pFlowEntry->enable)
    {        
        return 4;
    }

    status = WPE_Eoam_FlowRuleModify(pFlowEntry->handle,
                                        pFlowEntry->flow_agg,
                                        EOAM_LM_PM_DISABLE,
                                        WPE_EXT_ACTION_FLOW_AGG,
                                        pMaEntry->ma_config.maintenance_domain_level);
    WPE_TerminateOnError(status, "WPE_Eoam_FLOW_ModifyFlowRule",__LINE__);
    
    pFlowEntry->enable = WP_FALSE;
    
    return WP_OK;
}

/***************************************************************
 * Func name  : WPE_Eoam_FLOW_EnableRXFCLCount
 * Description: 
 * Input      : 
 * OutPut     : 
 * Return Val :
 ***************************************************************/
WP_S32 WPE_Eoam_FLOW_EnableRXFCLCount(WP_U8 s_mac[6], WP_U8 d_mac[6], WP_U16 vlan, WP_U32 ma_id, WP_U16 mep_id)
{
    WP_status status;
    WPE_EOAM_MA_T* pMaEntry = NULL;
    WPE_EOAM_RMMEP_T* pMepEntry = NULL;
    WPE_EOAM_FLOW_T* pFlowEntry = NULL;
    

    pMaEntry = WPE_Eoam_Ma_FindEntryByMAID(ma_id);
    if (NULL == pMaEntry)
    {
        EOAM_DEBUG_LOG(WPE_DEBUG_ERROR, "Can't find MA ID %d .\n", ma_id);
        
        return 1;
    }
    
    pMepEntry = WPE_Eoam_Mep_FindEntryByMEPID(pMaEntry, mep_id);
    if (NULL == pMepEntry)
    {
        EOAM_DEBUG_LOG(WPE_DEBUG_ERROR, "Can't find MEP ID %d .\n", mep_id);
        
        return 2;
    }

    pFlowEntry = WPE_Eoam_Flow_FindEntry(pMepEntry, WP_TRUE, s_mac, d_mac, vlan);
    if (NULL == pFlowEntry)
    {
        EOAM_DEBUG_LOG(WPE_DEBUG_ERROR, "Can't find such flow.\n");
        
        return 3;
    }
    
    if (WP_TRUE == pFlowEntry->enable)
    {
        
        return 4;
    }
    
    status = WPE_Eoam_FlowRuleModify(pFlowEntry->handle,
                                        pFlowEntry->flow_agg,
                                        pMepEntry->mep_config.rxtxfcl_stats_handle,
                                        WPE_EXT_ACTION_RXFCL_FLOW_AGG,
                                        pMaEntry->ma_config.maintenance_domain_level);
    WPE_TerminateOnError(status, "WPE_Eoam_FLOW_ModifyFlowRule",__LINE__);

    pFlowEntry->enable = WP_TRUE;
    
    return WP_OK;
}

/***************************************************************
 * Func name  : WPE_Eoam_FLOW_DisableRXFCLCount
 * Description: 
 * Input      : 
 * OutPut     : 
 * Return Val :
 ***************************************************************/
WP_S32 WPE_Eoam_FLOW_DisableRXFCLCount(WP_U8 s_mac[6], WP_U8 d_mac[6], WP_U16 vlan, WP_U32 ma_id, WP_U16 mep_id)
{
    WP_status status;
    WPE_EOAM_MA_T* pMaEntry = NULL;
    WPE_EOAM_RMMEP_T* pMepEntry = NULL;
    WPE_EOAM_FLOW_T* pFlowEntry = NULL;
    

    pMaEntry = WPE_Eoam_Ma_FindEntryByMAID(ma_id);
    if (NULL == pMaEntry)
    {
        EOAM_DEBUG_LOG(WPE_DEBUG_ERROR, "Can't find MA ID %d .\n", ma_id);
        
        return 1;
    }
    
    pMepEntry = WPE_Eoam_Mep_FindEntryByMEPID(pMaEntry, mep_id);
    if (NULL == pMepEntry)
    {
        EOAM_DEBUG_LOG(WPE_DEBUG_ERROR, "Can't find MEP ID %d .\n", mep_id);
        
        return 2;
    }

    pFlowEntry = WPE_Eoam_Flow_FindEntry(pMepEntry, WP_TRUE, s_mac, d_mac, vlan);
    if (NULL == pFlowEntry)
    {
        EOAM_DEBUG_LOG(WPE_DEBUG_ERROR, "Can't find such flow.\n");
        
        return 3;
    }
    
    if (WP_FALSE == pFlowEntry->enable)
    {
        
        return 4;
    }
    
    status = WPE_Eoam_FlowRuleModify(pFlowEntry->handle,
                                        pFlowEntry->flow_agg,
                                        EOAM_LM_PM_DISABLE,
                                        WPE_EXT_ACTION_FLOW_AGG,
                                        pMaEntry->ma_config.maintenance_domain_level);
    WPE_TerminateOnError(status, "WPE_Eoam_FLOW_ModifyFlowRule",__LINE__);

    pFlowEntry->enable = WP_FALSE;
    
    return WP_OK;
}


/****************************************************************************************************************************
 * Function name: WPE_Eoam_PM_ShowEnetDeviceStats()
 * Description  : display enet device related statistics
 *
 * Input  params: 
 * Output params: none
 * Return val   : none
 *****************************************************************************************************************************/
void WPE_Eoam_PM_ShowEnetDeviceStats(WP_handle enet_device_handle, WP_U8 reset)
{
    WP_status status;
    WP_stats_enet stats_enet;
    
    WP_U32 temp1, temp2;

    memset(&stats_enet,0,sizeof(stats_enet));
    status = WP_DeviceStatistics(enet_device_handle, &stats_enet);
    WPE_TerminateOnError(status, "WP_DeviceStatistics",__LINE__);
    
    if (stats_enet.txrx_frames_64)
    {
         temp1 = stats_enet.txrx_frames_64;
         temp2 = stats_enet.txrx_frames_64 >> 32;
         printf("txrx_frames_64:                     %.8x%.8x\n", temp2,temp1);
    }
    if (stats_enet.txrx_frames_127)
    {
         temp1 = stats_enet.txrx_frames_127;
         temp2 = stats_enet.txrx_frames_127 >> 32;
         printf("txrx_frames_127:                    %.8x%.8x\n", temp2,temp1);
    }
    if (stats_enet.txrx_frames_255)
    {
         temp1 = stats_enet.txrx_frames_255;
         temp2 = stats_enet.txrx_frames_255 >> 32;
         printf("txrx_frames_255:                    %.8x%.8x\n", temp2,temp1);
    }
    if (stats_enet.txrx_frames_511)
    {
         temp1 = stats_enet.txrx_frames_511;
         temp2 = stats_enet.txrx_frames_511 >> 32;
         printf("txrx_frames_511:                    %.8x%.8x\n", temp2,temp1);
    }
    if (stats_enet.txrx_frames_1023)
    {
         temp1 = stats_enet.txrx_frames_1023;
         temp2 = stats_enet.txrx_frames_1023 >> 32;
         printf("txrx_frames_1023:                   %.8x%.8x\n", temp2,temp1);
    }
    if (stats_enet.txrx_frames_1518)
    {
         temp1 = stats_enet.txrx_frames_1518;
         temp2 = stats_enet.txrx_frames_1518 >> 32;
         printf("txrx_frames_1518:                   %.8x%.8x\n", temp2,temp1);
    }
    if (stats_enet.txrx_frames_1522)
    {
         temp1 = stats_enet.txrx_frames_1522;
         temp2 = stats_enet.txrx_frames_1522 >> 32;
         printf("txrx_frames_1522:                   %.8x%.8x\n", temp2,temp1);
    }
    if (stats_enet.rx_bytes)
    {
         temp1 = stats_enet.rx_bytes;
         temp2 = stats_enet.rx_bytes >> 32;
         printf("rx_bytes:                           %.8x%.8x\n", temp2,temp1);
    }
    if (stats_enet.rx_packets)
    {
         temp1 = stats_enet.rx_packets;
         temp2 = stats_enet.rx_packets >> 32;
         printf("rx_packets:                         %.8x%.8x\n", temp2,temp1);
    }
    if (stats_enet.rx_err_fcs)
    {
         temp1 = stats_enet.rx_err_fcs;
         temp2 = stats_enet.rx_err_fcs >> 32;
         printf("rx_err_fcs:                         %.8x%.8x\n", temp2,temp1);   
    }
    if (stats_enet.rx_multicast)
    {
         temp1 = stats_enet.rx_multicast;
         temp2 = stats_enet.rx_multicast >> 32;
         printf("rx_multicast:                       %.8x%.8x\n", temp2,temp1);
    }
    if (stats_enet.rx_broadcast)
    {
         temp1 = stats_enet.rx_broadcast;
         temp2 = stats_enet.rx_broadcast >> 32;
         printf("rx_broadcast:                       %.8x%.8x\n", temp2,temp1);
    }
    if (stats_enet.rx_mac_control)
    {
         temp1 = stats_enet.rx_mac_control;
         temp2 = stats_enet.rx_mac_control >> 32;
         printf("rx_mac_control:                     %.8x%.8x\n", temp2,temp1);
    }
    if (stats_enet.rx_mac_pause)
    {
         temp1 = stats_enet.rx_mac_pause;
         temp2 = stats_enet.rx_mac_pause >> 32;
         printf("rx_mac_pause:                       %.8x%.8x\n", temp2,temp1);
    }
    if (stats_enet.rx_mac_unknown)
    {
         temp1 = stats_enet.rx_mac_unknown;
         temp2 = stats_enet.rx_mac_unknown >> 32;
         printf("rx_mac_unknown:                     %.8x%.8x\n", temp2,temp1);
    }
    if (stats_enet.rx_err_alignment)
    {
         temp1 = stats_enet.rx_err_alignment;
         temp2 = stats_enet.rx_err_alignment >> 32;
         printf("rx_err_alignment:                   %.8x%.8x\n", temp2,temp1);
    }
    if (stats_enet.rx_err_length)
    {
         temp1 = stats_enet.rx_err_length;
         temp2 = stats_enet.rx_err_length >> 32;
         printf("rx_err_length:                      %.8x%.8x\n", temp2,temp1);
    }
    if (stats_enet.rx_err_code)
    {
         temp1 = stats_enet.rx_err_code;
         temp2 = stats_enet.rx_err_code >> 32;
         printf("rx_err_code:                        %.8x%.8x\n", temp2,temp1);
    }
    if (stats_enet.rx_false_carrier)
    {
         temp1 = stats_enet.rx_false_carrier;
         temp2 = stats_enet.rx_false_carrier >> 32;
         printf("rx_false_carrier:                   %.8x%.8x\n", temp2,temp1);
    }
    if (stats_enet.rx_undersize)
    {
         temp1 = stats_enet.rx_undersize;
         temp2 = stats_enet.rx_undersize >> 32;
         printf("rx_undersize:                       %.8x%.8x\n", temp2,temp1);
    }
    if (stats_enet.rx_oversize)
    {
         temp1 = stats_enet.rx_oversize;
         temp2 = stats_enet.rx_oversize >> 32;
         printf("rx_oversize:                        %.8x%.8x\n", temp2,temp1);
    }
    if (stats_enet.rx_fragments)
    {
         temp1 = stats_enet.rx_fragments;
         temp2 = stats_enet.rx_fragments >> 32;
         printf("rx_fragments:                       %.8x%.8x\n", temp2,temp1);
    }
    if (stats_enet.rx_jabber)
    {
         temp1 = stats_enet.rx_jabber;
         temp2 = stats_enet.rx_jabber >> 32;
         printf("rx_jabber:                          %.8x%.8x\n", temp2,temp1);
    }
    if (stats_enet.rx_dropped)
    {
         temp1 = stats_enet.rx_dropped;
         temp2 = stats_enet.rx_dropped >> 32;
         printf("rx_dropped:                         %.8x%.8x\n", temp2,temp1);
    }

  
    if (stats_enet.tx_bytes)
    {
         temp1 = stats_enet.tx_bytes;
         temp2 = stats_enet.tx_bytes >> 32;
         printf("tx_bytes:                           %.8x%.8x\n", temp2,temp1);
    }
    if (stats_enet.tx_packets)
    {
         temp1 = stats_enet.tx_packets;
         temp2 = stats_enet.tx_packets >> 32;
         printf("tx_packets:                         %.8x%.8x\n", temp2,temp1);
    }
    if (stats_enet.tx_multicast)
    {
         temp1 = stats_enet.tx_multicast;
         temp2 = stats_enet.tx_multicast >> 32;
         printf("tx_multicast:                       %.8x%.8x\n", temp2,temp1);
    }
    if (stats_enet.tx_broadcast)
    {
         temp1 = stats_enet.tx_broadcast;
         temp2 = stats_enet.tx_broadcast >> 32;
         printf("tx_broadcast:                       %.8x%.8x\n", temp2,temp1);
    }
    if (stats_enet.tx_mac_pause)
    {
         temp1 = stats_enet.tx_mac_pause;
         temp2 = stats_enet.tx_mac_pause >> 32;
         printf("tx_mac_pause:                       %.8x%.8x\n", temp2,temp1);
    }
    if (stats_enet.tx_defer)
    {
         temp1 = stats_enet.tx_defer;
         temp2 = stats_enet.tx_defer >> 32;
         printf("tx_defer:                           %.8x%.8x\n", temp2,temp1);
    }
    if (stats_enet.tx_excess_defer)
    {
         temp1 = stats_enet.tx_excess_defer;
         temp2 = stats_enet.tx_excess_defer >> 32;
         printf("tx_excess_defer:                    %.8x%.8x\n", temp2,temp1);
    }
    if (stats_enet.tx_single_collision)
    {
         temp1 = stats_enet.tx_single_collision;
         temp2 = stats_enet.tx_single_collision >> 32;
         printf("tx_single_collision:                %.8x%.8x\n", temp2,temp1);
    }
    if (stats_enet.tx_multi_collision)
    {
         temp1 = stats_enet.tx_multi_collision;
         temp2 = stats_enet.tx_multi_collision >> 32;
         printf("tx_multi_collision:                 %.8x%.8x\n", temp2,temp1);
    }
    if (stats_enet.tx_late_collision)
    {
         temp1 = stats_enet.tx_late_collision;
         temp2 = stats_enet.tx_late_collision >> 32;
         printf("tx_late_collision:                  %.8x%.8x\n", temp2,temp1);
    }
    if (stats_enet.tx_excess_collision)
    {
         temp1 = stats_enet.tx_excess_collision;
         temp2 = stats_enet.tx_excess_collision >> 32;
         printf("tx_excess_collision:                %.8x%.8x\n", temp2,temp1);
    }
    if (stats_enet.tx_no_collision)
    {
         temp1 = stats_enet.tx_no_collision;
         temp2 = stats_enet.tx_no_collision >> 32;
         printf("tx_no_collision:                    %.8x%.8x\n", temp2,temp1);
    }
    if (stats_enet.tx_mac_pause_honored)
    {
         temp1 = stats_enet.tx_mac_pause_honored;
         temp2 = stats_enet.tx_mac_pause_honored >> 32;
         printf("tx_mac_pause_honored:               %.8x%.8x\n", temp2,temp1);
    }   
    if (stats_enet.tx_dropped)
    {
         temp1 = stats_enet.tx_dropped;
         temp2 = stats_enet.tx_dropped >> 32;
         printf("tx_dropped:                         %.8x%.8x\n", temp2,temp1);
    }
    if (stats_enet.tx_jabber)
    {
         temp1 = stats_enet.tx_jabber;
         temp2 = stats_enet.tx_jabber >> 32;
         printf("tx_jabber:                          %.8x%.8x\n", temp2,temp1);
    }
    if (stats_enet.tx_err_fcs)
    {
         temp1 = stats_enet.tx_err_fcs;
         temp2 = stats_enet.tx_err_fcs >> 32;
         printf("tx_err_fcs:                         %.8x%.8x\n", temp2,temp1);
    }
    if (stats_enet.tx_control)
    {
         temp1 = stats_enet.tx_control;
         temp2 = stats_enet.tx_control >> 32;
         printf("tx_control:                         %.8x%.8x\n", temp2,temp1);
    }
    if (stats_enet.tx_oversize)
    {
         temp1 = stats_enet.tx_oversize;
         temp2 = stats_enet.tx_oversize >> 32;
         printf("tx_oversize:                        %.8x%.8x\n", temp2,temp1);
    }
    if (stats_enet.tx_undersize)
    {
         temp1 = stats_enet.tx_undersize;
         temp2 = stats_enet.tx_undersize >> 32;
         printf("tx_undersize:                       %.8x%.8x\n", temp2,temp1);
    }
    if (stats_enet.tx_fragments)
    {
         temp1 = stats_enet.tx_fragments;
         temp2 = stats_enet.tx_fragments >> 32;
         printf("tx_fragments:                       %.8x%.8x\n", temp2,temp1);
    }
    
    printf("DPS statistics:\n");
    if (stats_enet.rx_host_frames)
    {
         temp1 = stats_enet.rx_host_frames;
         temp2 = stats_enet.rx_host_frames >> 32;
         printf("rx_host_frames:                     %.8x%.8x\n", temp2,temp1);    
    }
    if (stats_enet.rx_iw_frames)
    {
         temp1 = stats_enet.rx_iw_frames;
         temp2 = stats_enet.rx_iw_frames >> 32;
         printf("rx_iw_frames:                       %.8x%.8x\n", temp2,temp1);    
    }
    if (stats_enet.rx_err_host_full)
    {
         temp1 = stats_enet.rx_err_host_full;
         temp2 = stats_enet.rx_err_host_full >> 32;
         printf("rx_err_host_full:                   %.8x%.8x\n", temp2,temp1);
    }
    if (stats_enet.rx_err_fbp_underrun)
    {
         temp1 = stats_enet.rx_err_fbp_underrun;
         temp2 = stats_enet.rx_err_fbp_underrun >> 32;
         printf("rx_err_fbp_underrun:                %.8x%.8x\n", temp2,temp1);
    }
    if (stats_enet.rx_err_nonvalid_mac)
    {
         temp1 = stats_enet.rx_err_nonvalid_mac;
         temp2 = stats_enet.rx_err_nonvalid_mac >> 32;
         printf("rx_err_mac_address:                 %.8x%.8x\n", temp2,temp1);
    }
    if ( stats_enet.rx_err_mru)
    {
         temp1 = stats_enet.rx_err_mru;
         temp2 = stats_enet.rx_err_mru >> 32;
         printf("rx_err_mru:                         %.8x%.8x\n", temp2,temp1);
    }
    if (stats_enet.rx_err_sw_state_machine)
    {
         temp1 = stats_enet.rx_err_sw_state_machine;
         temp2 = stats_enet.rx_err_sw_state_machine >> 32;
         printf("rx_err_sw_state:                    %.8x%.8x\n", temp2,temp1);        
    }
    if (stats_enet.rx_err_overrun)
    {
         temp1 = stats_enet.rx_err_overrun;
         temp2 = stats_enet.rx_err_overrun >> 32;
         printf("rx_err_overrun:                     %.8x%.8x\n", temp2,temp1);
    }
    if (stats_enet.rx_err_sdu)
    {
         temp1 = stats_enet.rx_err_sdu;
         temp2 = stats_enet.rx_err_sdu >> 32;
         printf("rx_err_maxsdu:                      %.8x%.8x\n", temp2,temp1);
    }
    if (stats_enet.tx_err_underrun)
    {
         temp1 = stats_enet.tx_err_underrun;
         temp2 = stats_enet.tx_err_underrun >> 32;
         printf("tx_err_underrun:                    %.8x%.8x\n", temp2,temp1);
    }
    if (stats_enet.rx_err_overrun)
    {
         temp1 = stats_enet.rx_err_overrun;
         temp2 = stats_enet.rx_err_overrun >> 32;
         printf("rx_err_overrun:                     %.8x%.8x\n", temp2,temp1);
    }
    if (stats_enet.tx_frames)
    {
         temp1 = stats_enet.tx_frames;
         temp2 = stats_enet.tx_frames >> 32;
         printf("tx_good_frames:                     %.8x%.8x\n", temp2,temp1);
    }
    
    printf("Serial DPS IF statistics:\n");
    if (stats_enet.serial_dps_if_stats.tx_frames)
    {
         temp1 = stats_enet.serial_dps_if_stats.tx_frames;
         temp2 = stats_enet.serial_dps_if_stats.tx_frames >> 32;
         printf("Serial DPS IF tx frames             %.8x%.8x\n", temp2,temp1);
    }
    if (stats_enet.serial_dps_if_stats.tx_bytes)
    {
         temp1 = stats_enet.serial_dps_if_stats.tx_bytes;
         temp2 = stats_enet.serial_dps_if_stats.tx_bytes >> 32;
         printf("Serial DPS IF tx bytes              %.8x%.8x\n", temp2,temp1);
    }
    if (stats_enet.serial_dps_if_stats.rx_frames)
    {
         temp1 = stats_enet.serial_dps_if_stats.rx_frames;
         temp2 = stats_enet.serial_dps_if_stats.rx_frames >> 32;
         printf("Serial DPS IF rx frames             %.8x%.8x\n", temp2,temp1);
    }
    if (stats_enet.serial_dps_if_stats.rx_bytes)
    {
         temp1 = stats_enet.serial_dps_if_stats.rx_bytes;
         temp2 = stats_enet.serial_dps_if_stats.rx_bytes >> 32;
         printf("Serial DPS IF rx bytes              %.8x%.8x\n", temp2,temp1);
    }
    if (stats_enet.serial_dps_if_stats.rx_pce_denied_frames)
    {
         temp1 = stats_enet.serial_dps_if_stats.rx_pce_denied_frames;
         temp2 = stats_enet.serial_dps_if_stats.rx_pce_denied_frames >> 32;
         printf("Serial DPS IF rx pce denied frames  %.8x%.8x\n", temp2,temp1);
    }
    if (stats_enet.serial_dps_if_stats.rx_dropped_frames)
    {
         temp1 = stats_enet.serial_dps_if_stats.rx_dropped_frames;
         temp2 = stats_enet.serial_dps_if_stats.rx_dropped_frames >> 32;
         printf("Serial DPS IF rx dropped frames     %.8x%.8x\n", temp2,temp1);
    }

    printf("\n");
    
    if (reset)
    {
        WP_DeviceStatisticsReset(enet_device_handle, &stats_enet);
        WPE_TerminateOnError(status, "WP_DeviceStatisticsReset",__LINE__);
    }    
}


/****************************************************************************************************************************
 * Function name: WPE_Eoam_PM_ShowBridgePortStats()
 * Description  : display bridge port related stats
 *
 * Input  params: 
 * Output params: none
 * Return val   : none
 *****************************************************************************************************************************/

void WPE_Eoam_PM_ShowBridgePortStats(WP_handle bport_handle, WP_U8 reset)
{
    WP_status status;
    WP_bport_stats bport_stats;
    WP_U32 temp2, temp1, index;
    memset(&bport_stats, 0, sizeof(bport_stats));
    status = WP_IwPortStatistics(bport_handle, &bport_stats);
    WPE_TerminateOnError(status, "WP_BportStatistics",__LINE__);

    if (bport_stats.rx_valid_packets)
    {
        temp1 = bport_stats.rx_valid_packets ;
        temp2 = bport_stats.rx_valid_packets >> 32;
        printf("rx_valid_packets                       : %.8x%.8x\n", temp2, temp1);
    }
    if (bport_stats.discard_vlan_acceptable_filter)
    {
        temp1 = bport_stats.discard_vlan_acceptable_filter ;
        temp2 = bport_stats.discard_vlan_acceptable_filter >> 32;
        printf("discard_vlan_acceptable_filter         : %.8x%.8x\n", temp2, temp1);
    }
    if (bport_stats.discard_ingress_filter)
    {
        temp1 = bport_stats.discard_ingress_filter ;
        temp2 = bport_stats.discard_ingress_filter >> 32;
        printf("discard_ingress_filter                 : %.8x%.8x\n", temp2, temp1);
    }
    if (bport_stats.discard_bridge_classifier)
    {
        temp1 = bport_stats.discard_bridge_classifier ;
        temp2 = bport_stats.discard_bridge_classifier >> 32;
        printf("discard_bridge_classifier              : %.8x%.8x\n", temp2, temp1);
    }
    if (bport_stats.discard_unk_macsa)
    {
        temp1 = bport_stats.discard_unk_macsa ;
        temp2 = bport_stats.discard_unk_macsa >> 32;
        printf("discard_unk_macsa                      : %.8x%.8x\n", temp2, temp1);
    }
    if (bport_stats.deny_mac_sa)
    {
        temp1 = bport_stats.deny_mac_sa ;
        temp2 = bport_stats.deny_mac_sa >> 32;
        printf("deny_mac_sa                            : %.8x%.8x\n", temp2, temp1);
    }
    if (bport_stats.deny_mac_da)
    {
        temp1 = bport_stats.deny_mac_da ;
        temp2 = bport_stats.deny_mac_da >> 32;
        printf("deny_mac_da                            : %.8x%.8x\n", temp2, temp1);
    }
    if (bport_stats.rx_bc_valid_packets)
    {
        temp1 = bport_stats.rx_bc_valid_packets ;
        temp2 = bport_stats.rx_bc_valid_packets >> 32;
        printf("rx_bc_valid_packets                    : %.8x%.8x\n", temp2, temp1);
    }
    if (bport_stats.rx_mc_valid_packets)
    {
        temp1 = bport_stats.rx_mc_valid_packets ;
        temp2 = bport_stats.rx_mc_valid_packets >> 32;
        printf("rx_mc_valid_packets                    : %.8x%.8x\n", temp2, temp1);
    }
    if (bport_stats.forwarded_uc_packets)
    {
        temp1 = bport_stats.forwarded_uc_packets ;
        temp2 = bport_stats.forwarded_uc_packets >> 32;
        printf("forwarded_uc_packets                   : %.8x%.8x\n", temp2, temp1);
    }    

    if (bport_stats.forwarded_bc_packets)
    {
        temp1 = bport_stats.forwarded_bc_packets ;
        temp2 = bport_stats.forwarded_bc_packets >> 32;
        printf("forwarded_bc_packets                   : %.8x%.8x\n", temp2, temp1);
    }
    if (bport_stats.forwarded_mc_packets)
    {
        temp1 = bport_stats.forwarded_mc_packets ;
        temp2 = bport_stats.forwarded_mc_packets >> 32;
        printf("forwarded_mc_packets                   : %.8x%.8x\n", temp2, temp1);
    }
    if (bport_stats.protocol_error)
    {
        temp1 = bport_stats.protocol_error ;
        temp2 = bport_stats.protocol_error >> 32;
        printf("protocol_error                         : %.8x%.8x\n", temp2, temp1);
    }
    if (bport_stats.discard_bridge_classifier)
    {
        temp1 = bport_stats.discard_bridge_classifier ;
        temp2 = bport_stats.discard_bridge_classifier >> 32;
        printf("discard_bridge_classifier              : %.8x%.8x\n", temp2, temp1);
    }
    if (bport_stats.res_mc_mac_deny)
    {
        temp1 = bport_stats.res_mc_mac_deny ;
        temp2 = bport_stats.res_mc_mac_deny >> 32;
        printf("res_mc_mac_deny                        : %.8x%.8x\n", temp2, temp1);
    }
    for (index = 0; index < WP_IW_DFC_MAX_NUM_OF_FILTERS; index++)
    {
        if (bport_stats.discard_dfc_filter[index])
        {
            temp1 = bport_stats.discard_dfc_filter[index] ;
            temp2 = bport_stats.discard_dfc_filter[index] >> 32;        
            printf("discard_dfc_filter[%02d]                 : %.8x%.8x\n", index, temp2, temp1);
        }        
    }
    
    if (bport_stats.deny_cfm_frames)
    {
        temp1 = bport_stats.deny_cfm_frames ;
        temp2 = 0;
        printf("deny_cfm_frames                        : %.8x%.8x\n", temp2, temp1);
    }
    
    if (bport_stats.bad_ip_version)
    {
        temp1 = bport_stats.bad_ip_version ;
        temp2 = bport_stats.bad_ip_version >> 32;
        printf("bad_ip_version                         : %.8x%.8x\n", temp2, temp1);
    }
    if (bport_stats.checksum_error)
    {
        temp1 = bport_stats.checksum_error ;
        temp2 = bport_stats.checksum_error >> 32;
        printf("checksum_error                         : %.8x%.8x\n", temp2, temp1);
    }
    if (bport_stats.ip_protocol_error)
    {
        temp1 = bport_stats.ip_protocol_error ;
        temp2 = bport_stats.ip_protocol_error >> 32;
        printf("ip_protocol_error                      : %.8x%.8x\n", temp2, temp1);
    }
    for (index = 0; index < WP_PCE_MAX_NUM_OF_FILTERS; index++)
    {
        if (bport_stats.discard_pce_filter[index])
        {
            temp1 = bport_stats.discard_pce_filter[index] ;
            temp2 = bport_stats.discard_pce_filter[index] >> 32;        
            printf("discard_pce_filter[%02d]              : %.8x%.8x\n", index, temp2, temp1);
        }        
    }
    
    if (bport_stats.ingress_policer_non_conforming_packets)
    {
        temp1 = bport_stats.ingress_policer_non_conforming_packets ;
        temp2 = bport_stats.ingress_policer_non_conforming_packets >> 32;
        printf("ingress_policer_non_conforming_packets : %.8x%.8x\n", temp2, temp1);
    }   
    if (bport_stats.rx_discard_lpm)
    {
        temp1 = bport_stats.rx_discard_lpm ;
        temp2 = bport_stats.rx_discard_lpm >> 32;
        printf("rx_discard_lpm                         : %.8x%.8x\n", temp2, temp1);
    }
    if (bport_stats.iw_mirror_fbp_underrun)
    {
        temp1 = bport_stats.iw_mirror_fbp_underrun ;
        temp2 = bport_stats.iw_mirror_fbp_underrun >> 32;
        printf("iw_mirror_fbp_underrun                 : %.8x%.8x\n", temp2, temp1);
    }   
    if (bport_stats.iw_mirror_iwbg_fbp_underrun)
    {
        temp1 = bport_stats.iw_mirror_iwbg_fbp_underrun ;
        temp2 = bport_stats.iw_mirror_iwbg_fbp_underrun >> 32;
        printf("iw_mirror_iwbg_fbp_underrun            : %.8x%.8x\n", temp2, temp1);
    }
    printf("\n");
    
    if (reset)
    {
        WP_IwPortStatisticsReset(bport_handle, &bport_stats);
    }    
}


/****************************************************************************************************************************
 * Function name: WPE_Eoam_PM_ShowFlowAggStats()
 * Description  : display flow aggregation related stats
 *
 * Input  params: 
 * Output params: none
 * Return val   : none
 *****************************************************************************************************************************/

void WPE_Eoam_PM_ShowFlowAggStats(WP_handle flow_agg_handle, WP_U8 reset)
{
    WP_status status;
    WP_iw_generic_flow_stats iw_flow_stats;
    WP_U32 temp1, temp2;
    memset(&iw_flow_stats, 0, sizeof(iw_flow_stats));
    status = WP_IwFlowStatistics(flow_agg_handle, WP_IW_FLOW_STAT_GENERIC, &iw_flow_stats);
    WPE_TerminateOnError(status, "WP_IwFlowStatistics",__LINE__);

    if (iw_flow_stats.forward_packet)
    {
        temp1 =      (iw_flow_stats.forward_packet);    
        temp2 =      (iw_flow_stats.forward_packet)>> 32;
        printf ("forward_packet:                  %.8x%.8x\n", temp2, temp1);
    }       
    if (iw_flow_stats.mtu_drop_packets)
    {
        temp1 =      (iw_flow_stats.mtu_drop_packets);
        temp2 =      (iw_flow_stats.mtu_drop_packets)>> 32;
        printf ("mtu_drop_packets:                %.8x%.8x\n", temp2, temp1);
        
    }         
    if (iw_flow_stats.tx_queue_drop_packets)
    {
        
        temp1 =      (iw_flow_stats.tx_queue_drop_packets);
        temp2 =      (iw_flow_stats.tx_queue_drop_packets)>> 32;
        printf ("tx_queue_drop_packets:           %.8x%.8x\n", temp2, temp1);
        
    }
    
    if (iw_flow_stats.forwarded_bytes)
    {
        
        temp1 =      (iw_flow_stats.forwarded_bytes);
        temp2 =      (iw_flow_stats.forwarded_bytes)>> 32;
        printf ("forwarded_bytes:                 %.8x%.8x\n", temp2, temp1);
        
    }          
    if (iw_flow_stats.denied_packets)
    {
        temp1 =      (iw_flow_stats.denied_packets);
        temp2 =      (iw_flow_stats.denied_packets)>> 32;
        printf ("denied_packets:                  %.8x%.8x\n", temp2, temp1);
    }      
    
    if (iw_flow_stats.policer_non_conforming_packets)
    {
        temp1 =      (iw_flow_stats.policer_non_conforming_packets);
        temp2 =      (iw_flow_stats.policer_non_conforming_packets)>> 32;
        
        printf ("policer_non_conforming_packets:  %.8x%.8x\n", temp2, temp1);
    }          
    if (iw_flow_stats.pecs_counter_1)
    {
        temp1 =      (iw_flow_stats.policer_non_conforming_packets);
        temp2 =      (iw_flow_stats.policer_non_conforming_packets)>> 32;
        
        printf ("pecs_counter_1:                  %.8x%.8x\n", temp2, temp1);
    }         
    if (iw_flow_stats.pecs_counter_2)
    {
        temp1 =      (iw_flow_stats.pecs_counter_2);
        temp2 =      (iw_flow_stats.pecs_counter_2)>> 32;
        
        printf ("pecs_counter_2:                  %.8x%.8x\n", temp2, temp1);
    }          
    if (iw_flow_stats.pecs_counter_3)
    {
        temp1 =      (iw_flow_stats.pecs_counter_3);
        temp2 =      (iw_flow_stats.pecs_counter_3)>> 32;
        
        printf ("pecs_counter_3:                  %.8x%.8x\n", temp2, temp1);
    }          
    if (iw_flow_stats.pecs_counter_4)
    {
        temp1 =      (iw_flow_stats.pecs_counter_4);
        temp2 =      (iw_flow_stats.pecs_counter_4)>> 32;
        
        printf ("pecs_counter_4:                  %.8x%.8x\n", temp2, temp1);
    }      
    if (iw_flow_stats.pecs_counter_5)
    {
        temp1 =      (iw_flow_stats.pecs_counter_5);
        temp2 =      (iw_flow_stats.pecs_counter_5)>> 32;
        
        printf ("pecs_counter_5:                  %.8x%.8x\n", temp2, temp1);
    }

    if (iw_flow_stats.pecs_counter_6)
    {
        temp1 =      (iw_flow_stats.pecs_counter_6);
        temp2 =      (iw_flow_stats.pecs_counter_6)>> 32;
        
        printf ("pecs_counter_6:                  %.8x%.8x\n", temp2, temp1);
    }
    if (iw_flow_stats.group_filtered_packets)
    {
        temp1 =      (iw_flow_stats.group_filtered_packets);
        temp2 =      (iw_flow_stats.group_filtered_packets)>> 32;
        
        printf ("group_filtered_packets:          %.8x%.8x\n", temp2, temp1);
    }

    printf("\n");
    
    if (reset)
    {
        WP_IwFlowStatisticsReset(flow_agg_handle, WP_IW_FLOW_STAT_GENERIC, &iw_flow_stats);
    }    
}

/*****************************************************************************
 * Function name: WPE_Eoam_PM_ShowStats
 * Description  : 1 to print all Device Stats;
                  2 to print all Bport  Stats;
                  3 to print all FlwAgg Stats; 
 * Input  params: None
 * Output params: None
 * Return val   : None
 *****************************************************************************/
WP_S32 WPE_Eoam_PM_ShowStats(WP_U8 type, WP_U8 reset)
{
    int i;
    
    switch(type)
    {
    case WPE_STAT_ENET:
        for (i = 0; i < MAX_ENETS; i++)
        {
            printf("-------------------------------\n");
            printf(" ENET %d (0x%08x) statisics\n", i, eoam_system.h_device_enet[i]);
            printf("-------------------------------\n");
            WPE_Eoam_PM_ShowEnetDeviceStats(eoam_system.h_device_enet[i], reset);
        }
        break;

    case WPE_STAT_BPORT:
        for (i = 0; i < MAX_BPORTS; i++)
        {
            printf("-------------------------------\n");
            printf("Bridge Port %d (0x%08x) Statistics \n", i, eoam_system.bports[i]);
            printf("-------------------------------\n");
            WPE_Eoam_PM_ShowBridgePortStats(eoam_system.bports[i], reset);

        }
        break;

    case WPE_STAT_FLOW:
        for (i = 0; i < MAX_FLOW_AGG; i++)
        {
            printf("-------------------------------\n");
            printf(" Flow %d (0x%08x) statistics\n", i, eoam_system.flow_agg[i]);
            printf("-------------------------------\n");
            WPE_Eoam_PM_ShowFlowAggStats(eoam_system.flow_agg[i], reset);
        }
        break;

    default:
        break;
    }
    
    return WP_OK;
}

/*****************************************************************************
 * Function name: WPE_Eoam_ResetAllPmStats
 * Description  : 
 * Input  params: None
 * Output params: None
 * Return val   : None
 *****************************************************************************/
void WPE_Eoam_ResetAllPmStats(void)
{
    int i;
    WP_stats_enet s_hs_enet;
    WP_bport_stats bport_stats;
    WP_iw_flow_stats flow_stats;

    for (i = 0; i < MAX_ENETS; i++)
    {
        if(eoam_system.h_device_enet[i])
           WP_DeviceStatisticsReset(eoam_system.h_device_enet[i], &s_hs_enet);
    }
    
    for (i = 0; i < ENET2_DUMMY_BPORT; i++)
    {
        if(eoam_system.bports[i])
           WP_IwPortStatisticsReset(eoam_system.bports[i], &bport_stats);
    }

    for (i = 0; i < MAX_FLOW_AGG; i++)
    {
        if(eoam_system.flow_agg[i])
           WP_IwFlowStatisticsReset(eoam_system.flow_agg[i], WP_IW_FLOW_STAT, &flow_stats);
    }
}

WP_handle WPE_Eoam_Performance_TestEnableMsg(WPE_EOAM_MA_PD_T* pMa, WPE_EOAM_RMMEP_PD_T* pMep, WP_iw_enet_oam_msg_cfg* pMsg, WP_U8 type)
{
    WP_U8 * cfm_data;
    WP_U16 mepid, offset;
    WP_handle handle;
    WP_status status;
    
    cfm_data = pMsg->message_data.data = (WP_U8 *)malloc(sizeof(WP_U8) * EOAM_MSG_LENGTH);
    WPE_TerminateOnMallocFailed(pMsg->message_data.data, "Message buffer allocation");
    memset(pMsg->message_data.data, 0, EOAM_MSG_LENGTH);

    pMsg->message_data.tx_iw_system = eoam_system.h_iw_sys_bridge[DL_IW_SYS];
    pMsg->message_data.tx_input_port = eoam_system.bports[ENET_OAM_DUMMY_BPORT];
    pMsg->message_data.tx_qnode_handle = eoam_system.h_qnode_iwq;
    pMsg->message_data.pool_handle = eoam_system.h_pool_256;
    pMsg->md_level = pMa->ma_config.maintenance_domain_level;
    pMsg->freq_rate = EOAM_PERF_TEST_MSG_FREQ_RATE;
    pMsg->ignore_oam_lookup = WP_DISABLE;
    pMsg->message_type = type;
    
    /* MAC DA */
    memcpy(cfm_data, pMep->mep_config.mac_sa, 6);
    
    /* MAC SA */
    memcpy(cfm_data + 6, pMa->ma_config.mac_address, 6);
   
    /* s-tag, c-tag*/
    cfm_data[12] = 0x91;
    cfm_data[13] = 0x00;
    cfm_data[14] = pMep->mep_config.stag_vlan_id >> 8;
    cfm_data[15] = pMep->mep_config.stag_vlan_id & 0xff;
  
    cfm_data[16] = 0x81;
    cfm_data[17] = 0x00;
    cfm_data[18] = pMep->mep_config.ctag_vlan_id >> 8;
    cfm_data[19] = pMep->mep_config.ctag_vlan_id & 0xff;

    cfm_data[20] = 0x89;
    cfm_data[21] = 0x02;    

    offset = pMsg->message_data.cfm_offset = PD_CFM_OFFSET;

    cfm_data[offset] = pMsg->md_level << 5;/* MD level and version */

    switch (pMsg->message_type)
    {
        case WP_IW_ENET_OAM_MESSAGE_TYPE_DMM:
        {
            cfm_data[offset + 1] = CFM_OPCODE_DMM;/* OPCODE */
            cfm_data[offset + 2] = 0x00;/* FLAGS */
            cfm_data[offset + 3] = DMM_TLV_OFFSET;/* TLV Offset */
            pMsg->type.dm_msg.remote_mep_handle = pMep->mep_handle;
            pMsg->message_data.data_length = PD_CFM_OFFSET + CFM_DMM_SIZE;
        }
        break;

        case WP_IW_ENET_OAM_MESSAGE_TYPE_1DM:
        {
            cfm_data[offset + 1] = CFM_OPCODE_1DM;/* OPCODE */
            cfm_data[offset + 2] = 0x00;/* FLAGS */
            cfm_data[offset + 3] = DM1_TLV_OFFSET;/* TLV Offset */
            pMsg->type.dm_msg.remote_mep_handle = pMep->mep_handle;
            pMsg->message_data.data_length = PD_CFM_OFFSET + CFM_1DM_SIZE;
        }
        break;

        case WP_IW_ENET_OAM_MESSAGE_TYPE_LMM:
        {
            cfm_data[offset + 1] = CFM_OPCODE_LMM;/* OPCODE */
            cfm_data[offset + 2] = 0x00;/* FLAGS */
            cfm_data[offset + 3] = LMM_TLV_OFFSET;/* TLV Offset */
            pMsg->type.lmm_msg.remote_mep_handle = pMep->mep_handle;
            pMsg->message_data.data_length = PD_CFM_OFFSET + CFM_LMM_SIZE;
            
        }
        break;

        case WP_IW_ENET_OAM_MESSAGE_TYPE_CCM:
        {
            cfm_data[offset + 1] = CFM_OPCODE_CCM;/* OPCODE */
            //cfm_data[offset + 2] = 0x00;/* FLAGS */
            cfm_data[offset + 2] = WPE_Eoam_FreqRateToValue(pMsg->freq_rate);
            cfm_data[offset + 3] = CCM_TLV_OFFSET;/* TLV Offset */
            /* MEP ID */            
            mepid = (EOAM_PERF_TEST_MAX_MEP_NUM + 1 - pMep->mep_config.remote_mep_id);
            cfm_data[offset + 8] = (mepid >> 8) & 0x1f;
            cfm_data[offset + 9] = mepid & 0xff;
            pMsg->type.ccm_msg.remote_mep_handle = pMep->mep_handle;
            pMsg->type.ccm_msg.update_lm_params = WP_FALSE;
            pMsg->type.ccm_msg.seq_num_mode = WP_IW_ENET_OAM_SEQUENCE_NUMBER_DISABLED;
            pMsg->message_data.data_length = PD_CFM_OFFSET + CFM_CCM_SIZE;
        }
        break;

        default:
            return EOAM_HANDLE_INVALID;
            break;
    }    

    handle = WP_IwEnetOamMessageCreate(WP_WINPATH(0), pMsg);
    WPE_TerminateOnError(handle, "WP_IwEnetOamMessageCreate()" , __LINE__);

    status = WP_IwEnetOamMessageEnable(handle);
    WPE_TerminateOnError(status, "WP_IwEnetOamMessageEnable()" , __LINE__);

    pMep->eoam_msg_table[type].msg_handle = handle;
    pMep->eoam_msg_table[type].cvlan = pMep->mep_config.ctag_vlan_id;
    pMep->eoam_msg_table[type].svlan = pMep->mep_config.stag_vlan_id;
    memcpy(&pMep->eoam_msg_table[type].cfg, pMsg, sizeof(WP_iw_enet_oam_msg_cfg));

    return handle;
}

/********************************************************************************
* Func name  : WPE_Eoam_Performance_TestStart
* Description: This function execute performance demo. 8 MA levels, 8 Remote MEP are createed.  
 *                   One MEP per MA. 8 Vlan services (S-tag) to identify the MA level. 8 Vlan services 
 *                   (C-tag) per MA to identify the MEP within an MA level. Enable CCM on each MEP with 
 *                   the highest rate possible(300 CCMs per second). Check wingine/Bus utilization.
*
* Input      :
* OutPut     :
* Return Val :
********************************************************************************/

WP_S32 WPE_Eoam_Performance_TestStart()
{
    WP_status status;
    WP_iw_enet_oam_ma ma_config;
    WP_iw_enet_oam_remote_mep rmep_config;
    WP_iw_enet_oam_msg_cfg msg_cfg;

    WPE_EOAM_MA_PD_T* pMaEntry =NULL;
    WPE_EOAM_RMMEP_PD_T* pMepEntry = NULL;
    WP_iw_enet_oam_msg_cfg* pMsg = &msg_cfg;
    WP_iw_enet_oam_remote_mep_vlan_params vlan;
    WP_iw_agg_bridging tx_agg_enet;


    WP_U16 ii = 0, jj = 0;

    WP_rx_binding_bridging  rx_binding_bri[1]=
    {
             {
                /*  encap_mode */0,
                /*  mru;*/1536,
                /*  vcfcs;*/ 0,
                /*  input_port;*/0
             }
    };

    if (eoam_system.ma_num)
    {
        printf("MA not empty! Please delete all manuly created items.\n");
        return 1;
    }

    if (eoam_performance_test)
    {
        printf("Performance test already enabled.\n");
        return 0;
    }
    
    printf("Starting...Please wait for a while.\n");

    eoam_performance_test = WP_TRUE;
    eoam_debug_level = WPE_DEBUG_CRIT;

    eoam_system.perf_ma_table = (WPE_EOAM_MA_PD_T*)malloc(sizeof(WPE_EOAM_MA_PD_T) * EOAM_PERF_TEST_MAX_MA_NUM);
    if (NULL == eoam_system.perf_ma_table)
    {
        printf("MA malloc failed.\n");
        WPE_Eoam_Quit(0);
        return 2;
    }

    memset(eoam_system.perf_ma_table, 0, sizeof(WPE_EOAM_MA_PD_T) * EOAM_PERF_TEST_MAX_MA_NUM);
    
    for (ii = 0; ii < EOAM_PERF_TEST_MAX_MA_NUM; ii++)
    {
        eoam_system.perf_ma_table[ii].eoam_mep_table = (WPE_EOAM_RMMEP_PD_T*)malloc(sizeof(WPE_EOAM_RMMEP_PD_T) * EOAM_PERF_TEST_MAX_MEP_NUM_PER_MA);
        if (NULL == eoam_system.perf_ma_table[ii].eoam_mep_table)
        {
            printf("MEP malloc failed num %d.\n", ii);
            WPE_Eoam_Quit(0);        
            break;
        }
    }    

    /*Modify Rx Binding*/
    rx_binding_bri[0].input_port = eoam_system.bports[ENET2_DUMMY_BPORT];
    rx_binding_bri[0].pce_if_handle = eoam_system.h_pce_if[0];
    status = WP_IwRxBindingModify (eoam_system.rx_enet_handle[ENET2],
                                 eoam_system.h_iw_sys_bridge[UL_IW_SYS],
                                 eoam_system.h_qnode_iwq,
                                WP_IW_RX_BIND_B_MOD_BPORT | WP_IW_RX_BIND_MOD_PCE_INTERFACE,
                                &rx_binding_bri[0]);    
    WPE_TerminateOnError(status," WP_IwRxBindingModify() Enet",__LINE__);

    /*Modify Rx Binding*/
    tx_agg_enet.iw_port = eoam_system.bports[ENET2_DUMMY_BPORT];
    status = WP_IwFlowAggregationModify(eoam_system.flow_agg[FA_PORT_ENET2], 
                                        WP_IW_FLOW_AGG_MOD_IWPORT,
                                        &tx_agg_enet);
    WPE_TerminateOnError(status," WP_IwFlowAggregationModify() Enet",__LINE__);

    eoam_system.lm_dummy = WP_IwFlowStatisticsTableCreate(WP_WINPATH(0), WP_IW_FLOW_CLASS_RULE_ENET_OAM_LM);
    WPE_TerminateOnError(eoam_system.lm_dummy," WP_IwFlowStatisticsTableCreate() Enet",__LINE__);

    /* Create 8 MAs */
    memset(&ma_config, 0, sizeof(WP_iw_enet_oam_ma));
    ma_config.freq_rate = EOAM_PERF_TEST_MSG_FREQ_RATE;
    ma_config.ma_id_check = WP_DISABLE;

    pMaEntry = eoam_system.perf_ma_table;
    for (ii = 0; ii < EOAM_PERF_TEST_MAX_MA_NUM; ii++, pMaEntry++)
    {
        printf("Create MA  index %04d.\r", ii);    
        ma_config.maintenance_domain_level = EOAM_MD_LEVEL;
        ma_config.mac_address[5] = (ii + 1);
       
        pMaEntry->ma_handle = WP_IwEnetOamMaCreate(WP_WINPATH(0),
                                              WP_IW_ENET_OAM_MA_TYPE_SINGLE_MAC,
                                              &ma_config);
        WPE_TerminateOnError(pMaEntry->ma_handle, "WP_IwEnetOamMaCreate()",__LINE__);

        memcpy(&pMaEntry->ma_config, &ma_config, sizeof(WP_iw_enet_oam_ma));
    }

    printf("\n");
    /* Enable all MEPS*/
    WPE_Eoam_Mep_ConfigInit(&rmep_config);
    
    rmep_config.iw_port = eoam_system.bports[ENET2_DUMMY_BPORT];
    rmep_config.cfm_params.flow_agg = eoam_system.flow_agg[FA_RES_ENET_OAM];
    rmep_config.cfm_params.cfm_mode = WP_ENABLE;
    rmep_config.cfm_params.status_detection_mode = WP_DISABLE;
    rmep_config.cfm_params.process_mode = WP_IW_ENET_OAM_PROCESS_DROP;

    rmep_config.pm_params.pm_reply_flow_aggregation = eoam_system.flow_agg[FA_PORT_ENET2];
    rmep_config.pm_params.process_mode = WP_IW_ENET_OAM_PROCESS_DROP;

    rmep_config.rxtxfcl_stats_handle = eoam_system.lm_dummy;
    rmep_config.pm_params.lm_mode = WP_IW_ENET_OAM_LM_SINGLE_ENDED;
    rmep_config.pm_params.dm_mode = WP_IW_ENET_OAM_TWO_WAY_DM;
    rmep_config.message_queue_mode = WP_IW_ENET_OAM_MSGQ_DISABLE; // will be mass of events, disable the message queue. 
    

    pMaEntry = eoam_system.perf_ma_table;
    for (ii = 0; ii < EOAM_PERF_TEST_MAX_MA_NUM; ii++, pMaEntry++)
    {
        rmep_config.mac_sa[5] = EOAM_PERF_TEST_MAX_MA_NUM - ii;
        
        pMepEntry = pMaEntry->eoam_mep_table;
        for (jj = 0; jj < EOAM_PERF_TEST_MAX_MEP_NUM_PER_MA; jj++, pMepEntry++) 
        {
            printf("Create MEP index %04d-%04d.\r", ii, jj); 
            rmep_config.remote_mep_id = ((EOAM_PERF_TEST_MAX_MA_NUM - ii) * EOAM_PERF_TEST_MAX_MEP_NUM_PER_MA) - jj;
            
            if (ii >= (EOAM_PERF_TEST_MAX_MA_NUM/2))
            {
                rmep_config.stag_vlan_id = EOAM_PERF_TEST_MAX_MA_NUM - ii;
                rmep_config.ctag_vlan_id = rmep_config.remote_mep_id;
            }
            else
            {
                rmep_config.stag_vlan_id = ii + 1;
                rmep_config.ctag_vlan_id = EOAM_PERF_TEST_MAX_MEP_NUM + 1 - rmep_config.remote_mep_id;
            }

            pMepEntry->mep_handle = WP_IwEnetOamRemoteMepCreate(pMaEntry->ma_handle, &rmep_config);
            WPE_TerminateOnError(pMepEntry->mep_handle, "WP_IwEnetOamRemoteMepCreate",__LINE__);
            
            memcpy(&pMepEntry->mep_config, &rmep_config, sizeof(WP_iw_enet_oam_remote_mep));
       }
    }
    printf("\n");
    /*create and enable CCM/DMM/LMM message*/
    pMaEntry = eoam_system.perf_ma_table;
    for (ii = 0; ii < EOAM_PERF_TEST_MAX_MA_NUM; ii++, pMaEntry++)
    {
        pMepEntry = pMaEntry->eoam_mep_table;
        for (jj = 0; jj < EOAM_PERF_TEST_MAX_MEP_NUM_PER_MA; jj++, pMepEntry++)
        {

            printf("Enable MSG index %04d-%04d.\r", ii, jj); 
            vlan.ctag_vlan_id = pMepEntry->mep_config.ctag_vlan_id;
            vlan.stag_vlan_id = pMepEntry->mep_config.stag_vlan_id;
            
            status = WP_IwEnetOamLookupEntryAdd(pMepEntry->mep_handle,
                                               WP_IW_ENET_OAM_REMOTE_MEP_PM_ENTRY,
                                               &vlan);
            WPE_TerminateOnError(status,"WP_IwEnetOamLookupEntryAdd",__LINE__);  

            WPE_Eoam_Performance_TestEnableMsg(pMaEntry, pMepEntry, pMsg, WP_IW_ENET_OAM_MESSAGE_TYPE_CCM);
            WPE_Eoam_Performance_TestEnableMsg(pMaEntry, pMepEntry, pMsg, WP_IW_ENET_OAM_MESSAGE_TYPE_DMM);
            WPE_Eoam_Performance_TestEnableMsg(pMaEntry, pMepEntry, pMsg, WP_IW_ENET_OAM_MESSAGE_TYPE_LMM);
          
        }
    }

    printf("\nDone.\n");
    
    //WP_Delay(5000000);
    return WP_OK;
   
}


/********************************************************************************
 * Func name  : WPE_Eoam_Performance_TestExit
 * Description: Disable and delete the CCM message. 
 * 
 * Input      :
 * OutPut     :
 * Return Val :
 ********************************************************************************/

WP_S32 WPE_Eoam_Performance_TestExit()
{
    WP_status status;

    WP_iw_agg_bridging tx_agg_enet;

    WP_rx_binding_bridging  rx_binding_bri[1]=
    {
             {
                /*  encap_mode */0,
                /*  mru;*/1536,
                /*  vcfcs;*/ 0,
                /*  input_port;*/0
             }
    };

    if (WP_FALSE == eoam_performance_test)
    {
        printf("Performance test not enabled.\n");
        return 1;
    }

    printf("Stoping...please wait for a while\n");

    WPE_Eoam_PerformanceTest_Release();

    /*Modify Rx Binding*/
    rx_binding_bri[0].pce_if_handle = eoam_system.h_pce_if[0];
    rx_binding_bri[0].input_port = eoam_system.bports[ENET2_BPORT];
    status = WP_IwRxBindingModify (eoam_system.rx_enet_handle[ENET2],
                                 eoam_system.h_iw_sys_bridge[UL_IW_SYS],
                                 eoam_system.h_qnode_iwq,
                                WP_IW_RX_BIND_B_MOD_BPORT|WP_IW_RX_BIND_MOD_PCE_INTERFACE,
                                &rx_binding_bri[0]);
    WPE_TerminateOnError(status," WP_IwRxBindingModify() Enet",__LINE__);

    tx_agg_enet.iw_port = eoam_system.bports[ENET2_BPORT];
    status = WP_IwFlowAggregationModify(eoam_system.flow_agg[FA_PORT_ENET2], 
                                        WP_IW_FLOW_AGG_MOD_IWPORT,
                                        &tx_agg_enet);
    WPE_TerminateOnError(status," WP_IwFlowAggregationModify() Enet",__LINE__);  

    eoam_debug_level = WPE_DEBUG_WARNING;    
    eoam_performance_test = WP_FALSE;

    return WP_OK;
}

WP_U32 WPE_PrintBusUtil(WP_U32 wp_no, WPU_util_info* utilization)
{
WP_U32 status = WP_OK;

WPU_DUMP("\t\t\t\t%sSystem utilization (in percents):%s \n",COLOR_INVERT,COLOR_NORMAL);

    WPU_DUMP("==============================================================================================\n");
    WPU_DUMP("            ||  siu3 (internal) | siu4x | wse1 | wse2  | Parser | Classifier\n");
    WPU_DUMP("==============================================================================================\n");
    WPU_DUMP("Current     || ");

WPU_DUMP("      %s%3d%s        | %s%3d%s   | %s%3d%s  | %s%3d%s   |  %s%3d%s   |  %s%3d%s ",
COLOR(utilization->internal.total),(utilization->internal).total,COLOR_NORMAL,
COLOR(utilization->siu4x),utilization->siu4x,COLOR_NORMAL,

COLOR(utilization->wse[WPU_WSE1]),utilization->wse[WPU_WSE1],COLOR_NORMAL,
COLOR(utilization->wse[WPU_WSE2]),utilization->wse[WPU_WSE2],COLOR_NORMAL,

COLOR(utilization->pce_parser),utilization->pce_parser,COLOR_NORMAL,
COLOR(utilization->pce_classifier),utilization->pce_classifier,COLOR_NORMAL);

    WPU_DUMP("\n==============================================================================================\n");

    WPU_DUMP("\t\t\t\t%sDDR Utilization (in percents) :%s \n",COLOR_INVERT,COLOR_NORMAL);
    WPU_DUMP("==============================================================================================\n");
    WPU_DUMP("            ||        Controller Utilization                              \n");
    WPU_DUMP("==============================================================================================\n");
    WPU_DUMP("            ||    siu1    |    siu2    |     siu4d        \n");

    WPU_DUMP("----------------------------------------------------------------------------------------------\n");
    WPU_DUMP("Total       ||    %s%3d%s     |    %s%3d%s     |      %s%3d%s         \n",
            COLOR(utilization->ddr_cntlr[WPU_DDR_SIU1].total),utilization->ddr_cntlr[WPU_DDR_SIU1].total,COLOR_NORMAL,
            COLOR(utilization->ddr_cntlr[WPU_DDR_SIU2].total),utilization->ddr_cntlr[WPU_DDR_SIU2].total,COLOR_NORMAL,
            COLOR(utilization->ddr_cntlr[WPU_DDR_SIU4D].total),utilization->ddr_cntlr[WPU_DDR_SIU4D].total,COLOR_NORMAL
    );

    WPU_DUMP("----------------------------------------------------------------------------------------------\n");
    WPU_DUMP("Write       ||    %3d     |    %3d     |      %3d        \n",
         utilization->ddr_cntlr[WPU_DDR_SIU1].write,utilization->ddr_cntlr[WPU_DDR_SIU2].write,utilization->ddr_cntlr[WPU_DDR_SIU4D].write
    );
    WPU_DUMP("----------------------------------------------------------------------------------------------\n");
    WPU_DUMP("Read        ||    %3d     |    %3d     |      %3d          \n",
         utilization->ddr_cntlr[WPU_DDR_SIU1].read,utilization->ddr_cntlr[WPU_DDR_SIU2].read,utilization->ddr_cntlr[WPU_DDR_SIU4D].read
    );

    WPU_DUMP("----------------------------------------------------------------------------------------------\n");
    WPU_DUMP("Actual      ||    %3d     |", 100 - utilization->ddr_cntlr[WPU_DDR_SIU1].empty);

    WPU_DUMP("    %3d     |",100 - utilization->ddr_cntlr[WPU_DDR_SIU2].empty);

    WPU_DUMP("      %3d     \n",100 - utilization->ddr_cntlr[WPU_DDR_SIU4D].empty);

    WPU_DUMP("utilization ||            |            |              \n");

    WPU_RETURN_IF_ERROR(status,WPU_UTIL(wp_no, 0, WPU_UTIL_CMD_GET_INTERNAL_ALL, utilization));
    WPU_DUMP("==============================================================================================\n");
    WPU_DUMP("\t\t\t\t%sWMM to SIU3 (internal) Utilization (in percents) :%s \n",COLOR_INVERT,COLOR_NORMAL);
    WPU_DUMP("==============================================================================================\n");
    WPU_DUMP("            ||    WMM3    |    WMM5    |      FMU     \n");

    WPU_DUMP("----------------------------------------------------------------------------------------------\n");
    WPU_DUMP("Write bus   ||    %3d     |    %3d     |     %3d       \n",
            utilization->internal.internal_util[WPU_INTERNAL_WMM3].write,utilization->internal.internal_util[WPU_INTERNAL_WMM5].write,
            utilization->internal.internal_util[WPU_INTERNAL_FMU].write);
    WPU_DUMP("----------------------------------------------------------------------------------------------\n");
    WPU_DUMP("Read bus    ||    %3d     |    %3d     |     %3d       \n",
            utilization->internal.internal_util[WPU_INTERNAL_WMM3].read,utilization->internal.internal_util[WPU_INTERNAL_WMM5].read,
            utilization->internal.internal_util[WPU_INTERNAL_FMU].read);
   WPU_DUMP("==============================================================================================\n");
  
   return WP_OK;

}

/********************************************************************************
 * Func name  : WPE_Eoam_Performance_CheckUtil
 * Description: Check Wingine and Bus utilization
 * 
 * Input      :
 * OutPut     :
 * Return Val :
 ********************************************************************************/
WP_S32 WPE_Eoam_Performance_CheckUtil()
{
    WP_status status;
    WPU_util_info utilization;

    if (WP_FALSE == eoam_performance_test)
    {
        printf("Performance test not enabled.\n");
        return 1;
    }

    memset(&utilization, 0, sizeof(WPU_util_info));

    //display Bus util
    WPU_RETURN_IF_ERROR(status,WPU_UTIL(APP_MASTER_WPID, 0, WPU_UTIL_CMD_GET_BUSES_PCE_WSE, &utilization));
    WPU_RETURN_IF_ERROR(status,WPE_PrintBusUtil(APP_MASTER_WPID, &utilization));

    //display wingine util
    WPU_RETURN_IF_ERROR(status,WPU_UTIL(APP_MASTER_WPID, 0, WPU_UTIL_CMD_GET_WG , &utilization));
    WPUI_PrintWinGineUtil(APP_MASTER_WPID, &utilization, WP_FALSE);

    return WP_OK;
}

/*****************************************************************************
 * Function name: WPE_Eoam_PM_SetPortLoopback
 * Description  : 1 to set port 0 loopback;
                  2 to set port 1 loopback;
                  3 to set port 2 loopback;
 * Input  params: None
 * Output params: None
 * Return val   : None
 *****************************************************************************/
WP_S32 WPE_Eoam_PM_SetPortLoopback(WP_U8 enet, WP_U8 reset)
{
    int i = enet;
    WP_U32 lb = reset ? 0: 1;
    if(enet > 2)
    {
      printf(" invalid ENET %d \n", enet);
      return WP_OK;
    }
    printf("-------------------------------\n");
    printf(" set ENET %d (0x%08x) port loopback to %s\n", i, eoam_system.h_port_enet[i], (lb ? "loopback" : "normal"));

   lb = lb ? WPX_SERDES_LOOPBACK : WPX_SERDES_NORMAL;
#ifdef BOARD_WP3

   status = WPX_BoardSerdesInit(APP_MASTER_WPID, WP_PORT_ENET6 + i, lb); 
   App_TerminateOnError(status, "WPX_BoardSerdesInit");

#else /* for WP3 SLB board*/ 

   status = WPX_BoardSerdesInit(APP_MASTER_WPID, WP_PORT_ENET9 + i, lb); 
   App_TerminateOnError(status, "WPX_BoardSerdesInit");
#endif

    return WP_OK;
}

/* msb copied first */
void WPE_AssemblePacket(WP_U8 *data_ptr, WP_U16 size, WP_U32 value)
{
   WP_U16 i;
   if(size > 4)
      size = 4;
   for(i = 0; i < size; i++)
   {
      data_ptr[i] = (value >> (8*(size - 1 - i))) & 0xff;
   }
}

void WPE_HostSendPacket(WP_handle pool, WP_handle txchan, WP_U8 *data_ptr, WP_U16 size)
{
   WP_U32 k;
   WP_status status;
   WP_data_segment* curr_buf;
   WP_data_unit data_unit;
   WP_data_segment tx_buf;
   
   if(pool == 0)
   {
      printf("invalid pool!\n");
      return ;
   }
   if(txchan == 0)
   {
      printf("invalid tx channel!\n");
      return ;
   }
   
   data_unit.data_size = size;
   data_unit.n_active = 1;
   
   tx_buf.data_size = data_unit.data_size;
   tx_buf.data = data_ptr;
   tx_buf.pool_handle = pool;
   tx_buf.displacement = 0;
   tx_buf.user_info = 0;
   tx_buf.next = NULL;
   tx_buf.buffer_class = 0;
   
   data_unit.control = 0;
   data_unit.segment = (&tx_buf);
   data_unit.n_segments = 1;
   data_unit.n_active = 1;
   data_unit.type = WP_DATA_ENET;
   data_unit.data_return = 0;
   data_unit.status = 0;
   
   curr_buf = data_unit.segment;
   for (k = 0; k < curr_buf->data_size; k++)
   {
      if(k %16 == 0) printf("\n0x%04x: ", k);
      if(k %4 == 0 && k %16 != 0) printf(" %d", k);
      printf("%2.2x", curr_buf->data[k]);
   }
   printf("\n");
   
   status = WP_HostSend(txchan, &data_unit);
   WPE_TerminateOnError (status, "WP_HostSend()", __LINE__);

}

void WPE_Eoam_Pce_ShowInfo(WP_handle filter_handle)
{
    WP_pce_filter_stat stats;
    WP_U32 temp2, temp1;

    memset(&stats, 0, sizeof(stats));
    status = WP_PceFilterStatistics(filter_handle, &stats);
    WPE_TerminateOnError(status, "WP_PceFilterStatistics()", __LINE__);

    printf("-------------------------------\n");
    printf(" Filter (0x%08x) statisics\n", filter_handle);
    printf("-------------------------------\n");
    if (stats.denied_pkts)
    {
        temp1 = stats.denied_pkts ;
        temp2 = stats.denied_pkts >> 32;
        printf("denied_pkts                       : %.8x%.8x\n", temp2, temp1);
    }
    if (stats.matched_pkts)
    {
        temp1 = stats.matched_pkts ;
        temp2 = stats.matched_pkts >> 32;
        printf("matched_pkts                      : %.8x%.8x\n", temp2, temp1);
    }
    if (stats.no_matched_pkts)
    {
        temp1 = stats.no_matched_pkts ;
        temp2 = stats.no_matched_pkts >> 32;
        printf("no_matched_pkts                   : %.8x%.8x\n", temp2, temp1);
    }

}

