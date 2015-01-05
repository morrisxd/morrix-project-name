/*****************************************************************************
 * (C) Copyright PMC - WIN division (Wintegra) 2011.  All rights reserved.
 * PMC-Sierra (WINTEGRA) CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of PMC-Sierra (Wintegra).
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/****************************************************************************
 *
 * File: wti_flexmux_alarms_and_pm.c
 * Demonstrates handling OMIINO alarm and performance monitoring callbacks
 *
 ****************************************************************************/

#include <string.h>
#if defined(__linux__)
#include <unistd.h>
#endif
#include "wp_wddi.h"

#ifndef WTI_FLEXMUX_UTIL_H
#include "wti_flexmux_util.h"
#endif
#ifndef _WPL_PLATFORM_H__
#include <wpl_platform.h>
#endif

#ifdef WT_UFE_FRAMER

//#define VERBOSE

extern char *OMIINO_FRAMER_ErrorCodeToTxt(WP_U32 ErrorCode);

/* Alarm and PM handling structures */
typedef struct {
   WP_U8 device;
   WP_U8 line_port_id;
   WP_U8 alarm_category;
   WP_U8 is_asserted;
} APP_PORT_ALARM_TASK;

typedef struct {
   WP_U8 device;
   WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE endpoint;
   WP_U8 alarm_category;
   WP_U8 is_asserted;
} APP_PATH_ALARM_TASK;

typedef struct {
   WP_U8 device;
   WP_U8 table_index;
   WP_U16 pad;
   WP_U32 timestamp;
} APP_PM_TASK;

typedef struct {
   WP_U8 device;
   WP_U8 line_port_id;
   WP_U8 k1_value;
   WP_U8 k2_value;
} APP_K1_K2_SIGNAL_TASK;

typedef struct {
   WP_U8 device;
   WP_U8 line_port_id;
   WP_U8 s1_value;
} APP_S1_SIGNAL_TASK;

typedef struct {
   WP_U8 device;
   WP_U32 socket_client_pdh;
   WP_U8 alarm_category;
   WP_U8 is_asserted;
} APP_PDH_ALARM_TASK;

#define PORT_ALARM_TYPE   1
#define PATH_ALARM_TYPE   2
#define K1K2_SIGNAL_TYPE  3
#define S1_SIGNAL_TYPE    4
#define PDH_ALARM_TYPE    5
#define PM_PORT_TYPE      6
#define PM_HO_PATH_TYPE   7
#define PM_LO_PATH_TYPE   8
#define PM_PDH_TYPE       9

typedef struct {
   WP_U32 task_type;
   union {
      APP_PORT_ALARM_TASK   port_alarm_task;
      APP_PATH_ALARM_TASK   path_alarm_task;
      APP_PM_TASK           pm_task;
      APP_K1_K2_SIGNAL_TASK k1k2_signal_task;
      APP_S1_SIGNAL_TASK    s1_signal_task;
      APP_PDH_ALARM_TASK    pdh_alarm_task;
   } u;
} framer_task;

typedef struct {
   WP_U16 head;
   WP_U16 tail;
   WP_U16 num_elements;
   WP_U16 pad;
   framer_task *task;
} framer_task_list;

/*************************************************/
/*  The task lists                               */
/*************************************************/

#define ALARM_TASK_LIST_SIZE 4096
framer_task the_alarm_tasks[ALARM_TASK_LIST_SIZE];
framer_task_list the_alarm_task_list[1] = { { 0, 0, ALARM_TASK_LIST_SIZE, 0, the_alarm_tasks} };
#define PM_TASK_LIST_SIZE 4096
framer_task the_pm_tasks[PM_TASK_LIST_SIZE];
framer_task_list the_pm_task_list[1] = { { 0, 0, PM_TASK_LIST_SIZE, 0, the_pm_tasks} };
#define SIGNAL_TASK_LIST_SIZE 4096
framer_task the_signal_tasks[PM_TASK_LIST_SIZE];
framer_task_list the_signal_task_list[1] = { { 0, 0, SIGNAL_TASK_LIST_SIZE, 0, the_signal_tasks} };


/*************************************************************
 *
 * This function adds a pre-formatted task to the task list.
 *
 * Inputs
 *   task_list:    pointer to the task list
 *   task:         pointer to a task definition to be added
 *                 (the task definition is copied into the
 *                 task list)
 *
 * ************************************************************/

WP_U32 num_tasks = 0;
WP_boolean pending_los = WP_FALSE;

void add_framer_task(framer_task_list *task_list, framer_task *task)
{
   WP_U16 tail = task_list->tail;
   WP_U32 next = tail + 1;

   if (next == task_list->num_elements)
      next = 0;

   if (next != task_list->head)
   {
      task_list->task[tail] = *task;
      task_list->tail = next;

      ++num_tasks;
   }

#if WTI_COLLECT_TRACE_INFORMATION
   record_action(47, WP_TimeRead());
#endif

   WTI_flexmux_release_server_lock();
}

/**************************************************
 * This function gets a task from a task queue.
 *
 * Inputs
 *   task_list:    pointer to the task list
 *   result:       pointer to where to copy task
 *
 * Output
 *   *result:      the task
 *
 * Return value
 *   NULL if no tasks were on the list
 *   result if a task was on the list
 *
 ******************************************************/

framer_task *next_framer_task(framer_task_list *task_list, framer_task *result)
{
   WP_U32 head = task_list->head;
   WP_U32 tail = task_list->tail;

   if (head == tail)
      return NULL;

   *result = task_list->task[head];
   if (++head == task_list->num_elements)
      head = 0;
   task_list->head = head;

   --num_tasks;

   return result;
}

void WTI_allocate_pm_storage_areas(void)
{
#if WTI_FLEXMUX_ENABLE_PERFORMANCE_MONITORING

   /* Allocate storage for the performance monitoring data */

#define SONET_SDH_PORT_STORAGE_NUM_ELEMENTS      120
#define SONET_SDH_HO_PATH_STORAGE_NUM_ELEMENTS   120
#define SONET_SDH_LO_PATH_STORAGE_NUM_ELEMENTS   120
#define PDH_STORAGE_NUM_ELEMENTS                 120

   void *space;
   int element_size;
   WUFE_status status;
   WP_U32 i;
   
   for (i=0;i<WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES;++i)
   {
      element_size = sizeof(WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_PORT_ENDPOINT_TYPE);
      space = malloc(SONET_SDH_PORT_STORAGE_NUM_ELEMENTS * element_size);
      if (space == NULL)
         WTI_FlexmuxCheckStatus("PM storage allocation",
                                WPX_UFE_FRAMER_ERROR_PARAMETER_OUT_OF_RANGE, __LINE__);
      status = WPX_FRMR_PM_SonetSdhPortStorage(i, space, SONET_SDH_PORT_STORAGE_NUM_ELEMENTS);
      if (status != WUFE_OK)
         WTI_FlexmuxCheckStatus("PM storage allocation",
                                WPX_UFE_FRAMER_ERROR_PARAMETER_OUT_OF_RANGE, __LINE__);
      element_size = sizeof(WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_HO_PATH_ENDPOINT_TYPE);
      space = malloc(SONET_SDH_HO_PATH_STORAGE_NUM_ELEMENTS * element_size);
      if (space == NULL)
         WTI_FlexmuxCheckStatus("PM storage allocation",
                                WPX_UFE_FRAMER_ERROR_PARAMETER_OUT_OF_RANGE, __LINE__);
      status = WPX_FRMR_PM_SonetSdhHOPathStorage(i, space, SONET_SDH_HO_PATH_STORAGE_NUM_ELEMENTS);
      if (status != WUFE_OK)
         WTI_FlexmuxCheckStatus("PM storage allocation",
                                WPX_UFE_FRAMER_ERROR_PARAMETER_OUT_OF_RANGE, __LINE__);

      element_size = sizeof(WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_LO_PATH_ENDPOINT_TYPE);
      space = malloc(SONET_SDH_LO_PATH_STORAGE_NUM_ELEMENTS * element_size);
      if (space == NULL)
         WTI_FlexmuxCheckStatus("PM storage allocation",
                                WPX_UFE_FRAMER_ERROR_PARAMETER_OUT_OF_RANGE, __LINE__);
      status = WPX_FRMR_PM_SonetSdhLOPathStorage(i, space, SONET_SDH_LO_PATH_STORAGE_NUM_ELEMENTS);
      if (status != WUFE_OK)
         WTI_FlexmuxCheckStatus("PM storage allocation",
                                WPX_UFE_FRAMER_ERROR_PARAMETER_OUT_OF_RANGE, __LINE__);

      element_size = sizeof(WPX_UFE_FRAMER_PDH_PERFORMANCE_MONITORING_ENDPOINT_TYPE);
      space = malloc(PDH_STORAGE_NUM_ELEMENTS * element_size);
      if (space == NULL)
         WTI_FlexmuxCheckStatus("PM storage allocation",
                                WPX_UFE_FRAMER_ERROR_PARAMETER_OUT_OF_RANGE, __LINE__);
      status = WPX_FRMR_PM_PdhStorage(i, space, PDH_STORAGE_NUM_ELEMENTS);
      if (status != WUFE_OK)
         WTI_FlexmuxCheckStatus("PM storage allocation",
                                WPX_UFE_FRAMER_ERROR_PARAMETER_OUT_OF_RANGE, __LINE__);
   }

#endif
}

#if WTI_PRINT_ALARM_DATA
static char *line_transfer_type_names[] =
{
   "WPX_UFE_FRAMER_WUFE_CAD_TYPE_T1",
   "WPX_UFE_FRAMER_WUFE_CAD_TYPE_E1",
   "WPX_UFE_FRAMER_WUFE_CAD_TYPE_HMVIP_4_T1",
   "WPX_UFE_FRAMER_WUFE_CAD_TYPE_HMVIP_4_E1",
   "WPX_UFE_FRAMER_WUFE_CAD_TYPE_E3",
   "WPX_UFE_FRAMER_WUFE_CAD_TYPE_DS3",

   "WPX_UFE_FRAMER_WUFE_SBI_TYPE_T1",
   "WPX_UFE_FRAMER_WUFE_SBI_TYPE_E1",
   "WPX_UFE_FRAMER_WUFE_SBI_TYPE_DS3",
   "WPX_UFE_FRAMER_WUFE_SBI_TYPE_E3",
   "WPX_UFE_FRAMER_WUFE_SBI_TYPE_IDLE",

   "WPX_UFE_FRAMER_WUFE_GENERIC_TYPE_T1",
   "WPX_UFE_FRAMER_WUFE_GENERIC_TYPE_E1",
   "WPX_UFE_FRAMER_WUFE_GENERIC_TYPE_DS3",
   "WPX_UFE_FRAMER_WUFE_GENERIC_TYPE_E3",

   "WPX_UFE_FRAMER_WUFE_SDH_TYPE_T1",
   "WPX_UFE_FRAMER_WUFE_SDH_TYPE_E1",
   "WPX_UFE_FRAMER_WUFE_SDH_TYPE_DS3",
   "WPX_UFE_FRAMER_WUFE_SDH_TYPE_E3",
   "WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC11",
   "WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC12",
   "WPX_UFE_FRAMER_WUFE_SDH_TYPE_TU3",
   "WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC3",
   "WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC4",
   "WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC4_4C",
   "WPX_UFE_FRAMER_WUFE_SDH_TYPE_VCAT",

   "WPX_UFE_FRAMER_WUFE_SONET_TYPE_T1",
   "WPX_UFE_FRAMER_WUFE_SONET_TYPE_E1",
   "WPX_UFE_FRAMER_WUFE_SONET_TYPE_DS3",
   "WPX_UFE_FRAMER_WUFE_SONET_TYPE_E3",
   "WPX_UFE_FRAMER_WUFE_SONET_TYPE_VT1_5",
   "WPX_UFE_FRAMER_WUFE_SONET_TYPE_VT2",
   "WPX_UFE_FRAMER_WUFE_SONET_TYPE_STS1_SPE",
   "WPX_UFE_FRAMER_WUFE_SONET_TYPE_STS3",
   "WPX_UFE_FRAMER_WUFE_SONET_TYPE_STS12",
   "WPX_UFE_FRAMER_WUFE_SONET_TYPE_VCAT"
};
#endif

#if WTI_FLEXMUX_ENABLE_ALARMS
#ifdef VERBOSE
#define WTI_LINE_SIDE_ALARM_DISPLAY 1
#else
#if WTI_PRINT_ALARM_DATA
#define WTI_LINE_SIDE_ALARM_DISPLAY 1
#else
#define WTI_LINE_SIDE_ALARM_DISPLAY 0
#endif
#endif

#if WTI_LINE_SIDE_ALARM_DISPLAY

static char *line_side_alarm_names[] =
{
   "WPX_UFE_FRAMER_SDH_LOS",        /*   0 */
   "WPX_UFE_FRAMER_SDH_LOF",        /*   1 */
   "WPX_UFE_FRAMER_SDH_RS_TIM",     /*   2 */
   "WPX_UFE_FRAMER_SDH_MS_AIS",     /*   3 */
   "WPX_UFE_FRAMER_SDH_MS_RDI",     /*   4 */
   "WPX_UFE_FRAMER_SDH_OOF",        /*   5 */
   "WPX_UFE_FRAMER_SDH_MS_EXC",     /*   6 */
   "WPX_UFE_FRAMER_SDH_MS_DEG",     /*   7 */
   "WPX_UFE_FRAMER_SDH_AU_AIS",     /*   8 */
   "WPX_UFE_FRAMER_SDH_AU_LOP",     /*   9 */
   "WPX_UFE_FRAMER_SDH_HP_LOM",     /*  10 */
   "WPX_UFE_FRAMER_SDH_HP_TIM",     /*  11 */
   "WPX_UFE_FRAMER_SDH_HP_PLM",     /*  12 */
   "WPX_UFE_FRAMER_SDH_HP_UNEQ",    /*  13 */
   "WPX_UFE_FRAMER_SDH_HP_RDI",     /*  14 */
   "WPX_UFE_FRAMER_SDH_HP_EXC",     /*  15 */
   "WPX_UFE_FRAMER_SDH_HP_DEG",     /*  16 */
   "WPX_UFE_FRAMER_SDH_LP_TIM",     /*  17 */
   "WPX_UFE_FRAMER_SDH_LP_PLM",     /*  18 */
   "WPX_UFE_FRAMER_SDH_LP_UNEQ",    /*  19 */
   "WPX_UFE_FRAMER_SDH_LP_RDI",     /*  20 */
   "WPX_UFE_FRAMER_SDH_LP_EXC",     /*  21 */
   "WPX_UFE_FRAMER_SDH_LP_DEG",     /*  22 */
   "WPX_UFE_FRAMER_SDH_TU_AIS",     /*  23 */
   "WPX_UFE_FRAMER_SDH_TU_LOP",     /*  24 */
   "WPX_UFE_FRAMER_SONET_LOS",      /*  25 */
   "WPX_UFE_FRAMER_SONET_LOF",      /*  26 */
   "WPX_UFE_FRAMER_SONET_TIM_S",    /*  27 */
   "WPX_UFE_FRAMER_SONET_AIS_L",    /*  28 */
   "WPX_UFE_FRAMER_SONET_RDI_L",    /*  29 */
   "WPX_UFE_FRAMER_SONET_OOF",      /*  30 */
   "WPX_UFE_FRAMER_SONET_EXC_L",    /*  31 */
   "WPX_UFE_FRAMER_SONET_DEG_L",    /*  32 */
   "WPX_UFE_FRAMER_SONET_LOM_P",    /*  33 */
   "WPX_UFE_FRAMER_SONET_TIM_P",    /*  34 */
   "WPX_UFE_FRAMER_SONET_PLM_P",    /*  35 */
   "WPX_UFE_FRAMER_SONET_UNEQ_P",   /*  36 */
   "WPX_UFE_FRAMER_SONET_RDI_P",    /*  37 */
   "WPX_UFE_FRAMER_SONET_EXC_P",    /*  38 */
   "WPX_UFE_FRAMER_SONET_DEG_P",    /*  39 */
   "WPX_UFE_FRAMER_SONET_TIM_V",    /*  40 */
   "WPX_UFE_FRAMER_SONET_PLM_V",    /*  41 */
   "WPX_UFE_FRAMER_SONET_UNEQ_V",   /*  42 */
   "WPX_UFE_FRAMER_SONET_RDI_V",    /*  43 */
   "WPX_UFE_FRAMER_SONET_EXC_V",    /*  44 */
   "WPX_UFE_FRAMER_SONET_DEG_V",    /*  45 */
   "WPX_UFE_FRAMER_SONET_AIS_V",    /*  46 */
   "WPX_UFE_FRAMER_SONET_LOP_V",    /*  47 */
   "WPX_UFE_FRAMER_SONET_AIS_P",    /*  48 */
   "WPX_UFE_FRAMER_SONET_LOP_P",    /*  49 */
   "WPX_UFE_FRAMER_SDH_RS_EXC",     /*  50 */
   "WPX_UFE_FRAMER_SDH_RS_DEG",     /*  51 */
   "WPX_UFE_FRAMER_SONET_EXC_S",    /*  52 */
   "WPX_UFE_FRAMER_SONET_DEG_S"     /*  53 */
};

static char *socket_client_side_alarm_names[] =
{
   "WPX_UFE_FRAMER_SOCKET_CLIENT_E_RFI",
   "WPX_UFE_FRAMER_SOCKET_CLIENT_A_RAI",
   "WPX_UFE_FRAMER_SOCKET_CLIENT_LOS_AIS"
};

#endif
#endif

void service_path_alarm(WP_U8 device_id,
                        WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *p_SDH_Sonet_line_endpoint_type,
                        WP_U8 alarm_category,
                        WP_U8 is_asserted)
{
#if WTI_COLLECT_TIMING_INFORMATION
   record_action(20, WP_TimeRead());
#endif
#if WTI_FLEXMUX_ENABLE_ALARMS
#if PRINT_ALARM_DATA
   WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *info = p_SDH_Sonet_line_endpoint_type;
   WP_U32 type = info->TransferType;

   if (type >= WPX_UFE_FRAMER_WUFE_SDH_TYPE_T1 && type <= WPX_UFE_FRAMER_WUFE_SDH_TYPE_VCAT)
      printf("PATH ALARM: <%d:%d:%d:%d:%d>  %s %s\n",
             info->u.SDH.stm4, info->u.SDH.stm1, info->u.SDH.stm0, info->u.SDH.tug2, info->u.SDH.tu,
             line_side_alarm_names[alarm_category], is_asserted ? "Asserted" : "Unasserted");
   else if (type >= WPX_UFE_FRAMER_WUFE_SONET_TYPE_T1 && type <= WPX_UFE_FRAMER_WUFE_SONET_TYPE_VCAT)
      printf("PATH ALARM: <%d:%d:%d:%d:%d>  %s %s\n",
             info->u.SONET.sts12, info->u.SONET.sts3, info->u.SONET.sts1, info->u.SONET.vt_group, info->u.SONET.vt,
             line_side_alarm_names[alarm_category], is_asserted ? "Asserted" : "Unasserted");
   else
      printf("????? type = %d\n", type);
   printf("             alarm_category = %s %s\n",
          line_side_alarm_names[alarm_category], is_asserted ? "Asserted" : "Unasserted");
#endif

#ifdef VERBOSE
   WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *info = p_SDH_Sonet_line_endpoint_type;

   printf("PATH ALARM:  device %d <%d:%d:%d:%d>: alarm_category = %s %s\n",
          device_id, info->u.SDH.stm1, info->u.SDH.stm0, info->u.SDH.tug2, info->u.SDH.tu,
          line_side_alarm_names[alarm_category], is_asserted ? "Asserted" : "Unasserted");
#endif
#endif
}

void service_port_alarm(WP_U8 device, WP_U8 line_port_id, WP_U8 alarm_category, WP_U8 is_asserted)
{
#if WTI_COLLECT_TIMING_INFORMATION
   record_action(21, WP_TimeRead());
#endif
#if WTI_FLEXMUX_ENABLE_ALARMS
#ifdef VERBOSE
   printf("PORT ALARM:  device %d, line_port_id = %d, alarm_category = %s %s\n",
          device, line_port_id, line_side_alarm_names[alarm_category], is_asserted ? "Asserted" : "Unasserted");
#endif
#endif
}

void service_los_alarm(WP_U8 device, WP_U8 line_port_id, WP_U8 alarm_category, WP_U8 is_asserted, framer_task *task)
{
#if WTI_FLEXMUX_ENABLE_ALARMS
   WP_U8 force_state;
   WP_U8 result;
   static WP_U32 transaction = 700;
   extern WP_U8 WPX_FRMR_PL_SONET_SDH_PORT_ForceA(WP_U32 TransactionIdentifier, WP_U8 iDevice, WP_U8 iLinePort);
   extern WP_U8 WPX_FRMR_PL_SONET_SDH_PORT_ForceB(WP_U32 TransactionIdentifier, WP_U8 iDevice, WP_U8 iLinePort);
   extern WP_U8 WPX_FRMR_PL_SONET_SDH_PORT_GetForceState(WP_U8 iDevice, WP_U8 iLinePort, WP_U8 * pForceSide);

#if WTI_COLLECT_TIMING_INFORMATION
   record_action(22, WP_TimeRead());
#endif

#ifdef VERBOSE
   printf("PORT LOS ALARM:  device id %d, line_port_id = %d, %s %s\n", line_port_id,
         device, line_side_alarm_names[alarm_category], is_asserted ? " ON" : "OFF");
#endif

#if NO_APS_ON_LOS
   printf("NO SWITCHING PERFORMED\n");
   return;
#endif

   if ((result = WPX_FRMR_PL_SONET_SDH_PORT_GetForceState(device, line_port_id, &force_state)) == WPX_UFE_FRAMER_OK)
   {
#if DEBUG
       if (force_state == WPX_UFE_FRAMER_PROTECTION_FORCE_A)
          printf("Current Force State is A\n");
       else if (force_state == WPX_UFE_FRAMER_PROTECTION_FORCE_B)
          printf("Current Force State is B\n");
#endif

       if (force_state == WPX_UFE_FRAMER_PROTECTION_FORCE_A)
       {
          if (is_asserted)
          {
             result = WPX_FRMR_PL_SONET_SDH_PORT_ForceB(transaction++, device, line_port_id);
             WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_PORT_ForceB", result, __LINE__);
          }
          else
             add_framer_task(the_alarm_task_list, task);

       }
       else if (force_state == WPX_UFE_FRAMER_PROTECTION_FORCE_B)
       {
          if (is_asserted)
          {
             result = WPX_FRMR_PL_SONET_SDH_PORT_ForceA(transaction++, device, line_port_id);
             WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_PORT_ForceA", result, __LINE__);
          }
          else
             add_framer_task(the_alarm_task_list, task);
       }
       else
          printf("On %d:  force_state unknown %d\n", line_port_id, force_state);
   }
   else if (result == WPX_UFE_FRAMER_ERROR_PORT_RATE_NOT_CONFIGURED)
      printf("GetForceState returns WPX_UFE_FRAMER_ERROR_PORT_RATE_NOT_CONFIGURED\n");
   else
      printf("GetForceState returns %d\n", result);

#if WTI_COLLECT_TIMING_INFORMATION
   record_action(23, WP_TimeRead());
#endif

#endif
}

void service_k1_k2_signal(WP_U8 device, WP_U8 line_port_id, WP_U8 k1, WP_U8 k2)
{
#if WTI_COLLECT_TIMING_INFORMATION
   record_action(24, WP_TimeRead());
#endif
#if WTI_FLEXMUX_ENABLE_ALARMS
#ifdef VERBOSE

   printf("K1K2 SIGNAL:  device = %d, iLinePort = %d K1 %d, K2 %d\n", device, line_port_id, k1, k2);

#endif
#endif
}

void service_s1_signal(WP_U8 device, WP_U8 line_port_id, WP_U8 s1)
{
#if WTI_COLLECT_TIMING_INFORMATION
   record_action(25, WP_TimeRead());
#endif
#if WTI_FLEXMUX_ENABLE_ALARMS
#ifdef VERBOSE

   printf("S1 SIGNAL:  device = %d, iLinePort = %d S1 %d\n", device, line_port_id, s1);

#endif
#endif
}

void service_pdh_alarm(WP_U8 device, WP_U32 socket_client_pdh_id, WP_U8 alarm_category, WP_U8 is_asserted)
{
#if WTI_COLLECT_TIMING_INFORMATION
   record_action(41, WP_TimeRead());
#endif
#if WTI_FLEXMUX_ENABLE_ALARMS
#ifdef VERBOSE
   printf("PDH ALARM:  iSocketClientPDH = %d, alarm_category = %s is %s\n",
         socket_client_pdh_id, socket_client_side_alarm_names[alarm_category], is_asserted ? "asserted" : "unasserted");
#endif
#endif
}

#if WTI_FLEXMUX_ENABLE_ALARMS

void cb_alarm_map_complete(WP_U8 device)
{
   void cb_port_alarm(WP_U8 device, WP_U8 line_port_id, WP_U8 alarm_category, WP_U8 is_asserted);
   void cb_path_alarm(WP_U8 device_id,
                   WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *p_SDH_Sonet_line_endpoint_type,
                   WP_U8 alarm_category, WP_U8 is_asserted);
   void cb_pdh_alarm(WP_U8 device_id, WP_U32 iSocketClientPDH, U8 alarm_category, U8 is_asserted);

   WPX_FRMR_RegisterSonetSdhPortAlarmCallback((void *)(int)&cb_port_alarm);
   WPX_FRMR_RegisterSonetSdhPathAlarmCallback((void *)(int)&cb_path_alarm);
   WPX_FRMR_RegisterSocketClientPdhAlarmCallback((void *)(int)&cb_pdh_alarm);
}

void cb_path_alarm(WP_U8 device_id, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *p_SDH_Sonet_line_endpoint_type,
                   WP_U8 alarm_category, WP_U8 is_asserted)
{
   framer_task task;
   APP_PATH_ALARM_TASK *path_alarm = &task.u.path_alarm_task;

#if WTI_PRINT_ALARM_DATA 

   WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *info = p_SDH_Sonet_line_endpoint_type;
   WP_U32 type = info->TransferType;
   static int first_time = 1;

   if (first_time)
   {
      first_time = 0;
      printf("PATH ALARM:  LTT = %s\n", line_transfer_type_names[type]);
   }

   if (type >= WPX_UFE_FRAMER_WUFE_SDH_TYPE_T1 && type <= WPX_UFE_FRAMER_WUFE_SDH_TYPE_VCAT)
      printf("PATH ALARM: <%d:%d:%d:%d:%d>  %s %s\n",
             info->u.SDH.stm4, info->u.SDH.stm1, info->u.SDH.stm0, info->u.SDH.tug2, info->u.SDH.tu,
             line_side_alarm_names[alarm_category], is_asserted ? "Asserted" : "Unasserted");
   else if (type >= WPX_UFE_FRAMER_WUFE_SONET_TYPE_T1 && type <= WPX_UFE_FRAMER_WUFE_SONET_TYPE_VCAT)
      printf("PATH ALARM: <%d:%d:%d:%d:%d>  %s %s\n",
             info->u.SONET.sts12, info->u.SONET.sts3, info->u.SONET.sts1, info->u.SONET.vt_group, info->u.SONET.vt,
             line_side_alarm_names[alarm_category], is_asserted ? "Asserted" : "Unasserted");
   else
      printf("UNKNOWN PATH ALARM TYPE %d\n", type);

#endif

   task.task_type = PATH_ALARM_TYPE;
   path_alarm->device = device_id;
   path_alarm->endpoint = *p_SDH_Sonet_line_endpoint_type;
   path_alarm->alarm_category = alarm_category;
   path_alarm->is_asserted = is_asserted;

   add_framer_task(the_alarm_task_list, &task);
#if WTI_COLLECT_TIMING_INFORMATION
   record_action(26, WP_TimeRead());
#endif
}

void cb_port_alarm(WP_U8 device, WP_U8 line_port_id, WP_U8 alarm_category, WP_U8 is_asserted)
{
   framer_task task;
   APP_PORT_ALARM_TASK *port_alarm = &task.u.port_alarm_task;

   task.task_type = PORT_ALARM_TYPE;
   port_alarm->device = device;
   port_alarm->line_port_id = line_port_id;
   port_alarm->alarm_category = alarm_category;
   port_alarm->is_asserted = is_asserted;

   if ((alarm_category == WPX_UFE_FRAMER_SDH_LOS)    ||
       (alarm_category == WPX_UFE_FRAMER_SDH_LOF)    ||
       (alarm_category == WPX_UFE_FRAMER_SDH_MS_AIS) ||
       (alarm_category == WPX_UFE_FRAMER_SONET_LOS)  ||
       (alarm_category == WPX_UFE_FRAMER_SONET_LOF))
   {
      if (is_asserted)
      {
#if WTI_COLLECT_TIMING_INFORMATION
         record_action(27, WP_TimeRead());
#endif
         service_los_alarm(device, line_port_id, alarm_category, is_asserted, &task);
      }
      else
         add_framer_task(the_alarm_task_list, &task);
   }
   else
      add_framer_task(the_alarm_task_list, &task);

#if WTI_COLLECT_TIMING_INFORMATION
   record_action(28, WP_TimeRead());
#endif
}

void cb_k1k2_signal(WP_U8 device_id, WP_U8 iLinePort, WPX_UFE_FRAMER_SONET_SDH_PORT_LINE_K1K2_TYPE *pRX_K1K2)
{
   framer_task task;
   APP_K1_K2_SIGNAL_TASK *k1k2 = &task.u.k1k2_signal_task;

   task.task_type = K1K2_SIGNAL_TYPE;
   k1k2->device = device_id;
   k1k2->line_port_id = iLinePort;
   k1k2->k1_value = pRX_K1K2->K1;
   k1k2->k2_value = pRX_K1K2->K2;

   add_framer_task(the_signal_task_list, &task);
#if WTI_COLLECT_TIMING_INFORMATION
   record_action(29, WP_TimeRead());
#endif
}

void cb_s1_signal(WP_U8 device_id, WP_U8 iLinePort, U8 s1_value)
{
   framer_task task;
   APP_S1_SIGNAL_TASK *s1 = &task.u.s1_signal_task;

   task.task_type = S1_SIGNAL_TYPE;
   s1->device = device_id;
   s1->line_port_id = iLinePort;
   s1->s1_value = s1_value;

   add_framer_task(the_signal_task_list, &task);
#if WTI_COLLECT_TIMING_INFORMATION
   record_action(30, WP_TimeRead());
#endif
}

void cb_pdh_alarm(WP_U8 device_id, WP_U32 iSocketClientPDH, U8 alarm_category, U8 is_asserted)
{
   framer_task task;
   APP_PDH_ALARM_TASK *pdh = &task.u.pdh_alarm_task;

   task.task_type = PDH_ALARM_TYPE;
   pdh->device = device_id;
   pdh->socket_client_pdh = iSocketClientPDH;
   pdh->alarm_category = alarm_category;
   pdh->is_asserted = is_asserted;

   add_framer_task(the_alarm_task_list, &task);
#if WTI_COLLECT_TIMING_INFORMATION
   record_action(31, WP_TimeRead());
#endif
}

/* These 3 functions are used during alarm map reporting */
void cb_port_alarm_report_only(WP_U8 device, WP_U8 line_port_id, WP_U8 alarm_category, WP_U8 is_asserted)
{                                    
#if WTI_FLEXMUX_ENABLE_ALARMS
#ifdef VERBOSE
   printf("PORT ALARM REPORTING::  line_port_id = %d, alarm_category = %s %s\n",
          line_port_id, line_side_alarm_names[alarm_category], is_asserted ? "Asserted" : "Unasserted");
#endif                              
#endif
}     

void cb_path_alarm_report_only(WP_U8 device_id,
                        WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *p_SDH_Sonet_line_endpoint_type,
                        WP_U8 alarm_category,
                        WP_U8 is_asserted)
{
#if WTI_FLEXMUX_ENABLE_ALARMS
#ifdef VERBOSE
   WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *info = p_SDH_Sonet_line_endpoint_type;

   printf("PATH ALARM REPORTING::  device %d <%d:%d:%d:%d>: alarm_category = %s %s\n",
          device_id, info->u.SDH.stm1, info->u.SDH.stm0, info->u.SDH.tug2, info->u.SDH.tu,
          line_side_alarm_names[alarm_category], is_asserted ? "Asserted" : "Unasserted");
#endif
#endif
}

void cb_pdh_alarm_report_only(WP_U8 device_id, WP_U32 iSocketClientPDH, U8 alarm_category, U8 is_asserted)
{
#if WTI_FLEXMUX_ENABLE_ALARMS
#ifdef VERBOSE
   printf("PDH ALARM REPORTING:  iSocketClientPDH = %d, alarm_category = %s is %s\n",
         iSocketClientPDH, socket_client_side_alarm_names[alarm_category], is_asserted ? "asserted" : "unasserted");
#endif
#endif
}

#endif

#if WTI_FLEXMUX_ENABLE_PERFORMANCE_MONITORING

WP_U32 B1_count, B2_count, M1_count;

#endif

void service_pm_port_data(WP_U8 device_id, WP_U8 table_index, WP_U32 timestamp)
{
#if WTI_COLLECT_TIMING_INFORMATION
   record_action(32, WP_TimeRead());
#endif
#if WTI_FLEXMUX_ENABLE_PERFORMANCE_MONITORING

   WP_U32 count;
   WP_U8 result;

#ifdef VERBOSE
   printf("PM PORT:  device_id = %d, table_index = %d, timestamp = 0x%x\n",
          device_id, table_index, timestamp);
#endif

   if ((result = WPX_FRMR_STATUS_PM_SONET_SDH_Port_ReadData(
                   device_id, table_index, 0,
                   WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_B1, &count)) == 0)
   {
#ifdef VERBOSE
      if (count > 0)
         printf("port %d: B1 count %d\n", device_id, count);
#endif
      B1_count += count;
   }
   else
      printf("Port_ReadData failed on B1: 0x%x\n", result);

   if ((result = WPX_FRMR_STATUS_PM_SONET_SDH_Port_ReadData(
                   device_id, table_index, 0,
                   WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_B2, &count)) == 0)
   {
#ifdef VERBOSE
      if (count > 0)
         printf("port %d: B2 count %d\n", device_id, count);
#endif
      B2_count += count;
   }
   else
      printf("Port_ReadData failed on B2: 0x%x\n", result);

   if ((result = WPX_FRMR_STATUS_PM_SONET_SDH_Port_ReadData(
                   device_id, table_index, 0,
                   WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_M1, &count)) == 0)
   {
#ifdef VERBOSE
      if (count > 0)
         printf("port %d: M1 count %d\n", device_id, count);
#endif
      M1_count += count;
   }
   else
      printf("Port_ReadData failed on M1: 0x%x\n", result);

#endif
}

void service_pm_ho_path_data(WP_U8 device_id, WP_U8 table_index, WP_U32 timestamp)
{
#if WTI_COLLECT_TIMING_INFORMATION
   record_action(33, WP_TimeRead());
#endif
#if WTI_FLEXMUX_ENABLE_PERFORMANCE_MONITORING

   WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE LineEndpointType;
   U32 count = 0;
   U32 max_stm1, max_stm0, max_tug2, max_tu;
   U32 stm1, stm0, tug2, tu;
   U8 result;
   U32 stm1_increment;

   memset(&LineEndpointType, 0, sizeof(WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE));

#ifdef VERBOSE
   printf("HO PATH:  device = %d, table_index = %d timestamp = 0x%x\n", device_id, table_index, timestamp);
#endif

   LineEndpointType.TransferType = ho_transfer_type_in_use;
   LineEndpointType.u.SDH.stm4 = 0;

   if (LineEndpointType.TransferType == WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC3)
      max_stm0 = 3;
   else
      max_stm0 = 1;

   max_tug2 = max_tu = 1;

   if (build_personality_in_use == WPX_UFE_FRAMER_BUILD_PERSONALITY_4_PLUS_4_OC3)
   {
      stm1_increment = 2;
      max_stm1 = 4;
   }
   else
   {
      stm1_increment = 1;
      max_stm1 = 4;
   }

   for (stm1 = 0; stm1 < max_stm1; stm1 += stm1_increment)
      for (stm0 = 0; stm0 < max_stm0; ++stm0)
         for (tug2 = 0; tug2 < max_tug2; ++tug2)
            for (tu = 0; tu < max_tu; ++tu)
            {
               LineEndpointType.u.SDH.stm1 = stm1;
               LineEndpointType.u.SDH.stm0 = stm0;
               LineEndpointType.u.SDH.tug2 = tug2;
               LineEndpointType.u.SDH.tu   = tu;

               result = WPX_FRMR_STATUS_PM_SONET_SDH_HO_PATH_ReadData(device_id, table_index,
                        &LineEndpointType, WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_B3,
                        &count);

               if (result)
                  printf("    ---->  HO_Path_ReadData for B3 failed on %d:%d:%d:%d on device %d with %s\n",
                         LineEndpointType.u.SDH.stm1,
                         LineEndpointType.u.SDH.stm0,
                         LineEndpointType.u.SDH.tug2,
                         LineEndpointType.u.SDH.tu,
                         device_id,
                         OMIINO_FRAMER_ErrorCodeToTxt(result));
#ifdef VERBOSE
               else if (count > 0)
                  printf("    ---->  B3 count on %d:%d:%d:%d is %d\n", stm1, stm0, tug2, tu, count);
#endif

               result = WPX_FRMR_STATUS_PM_SONET_SDH_HO_PATH_ReadData(device_id, table_index,
                        &LineEndpointType, WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_G1,
                        &count);

               if (result)
                  printf("    ---->  HO_Path_ReadData for G1 failed on %d:%d:%d:%d on device %d with %s\n",
                         LineEndpointType.u.SDH.stm1,
                         LineEndpointType.u.SDH.stm0,
                         LineEndpointType.u.SDH.tug2,
                         LineEndpointType.u.SDH.tu,
                         device_id,
                         OMIINO_FRAMER_ErrorCodeToTxt(result));
#ifdef VERBOSE
               else if (count > 0)
                  printf("    ---->  G1 count on %d:%d:%d:%d is %d\n", stm1, stm0, tug2, tu, count);
#endif
            }

#endif
}

void service_pm_lo_path_data(WP_U8 device_id, WP_U8 table_index, WP_U32 timestamp)
{
#if WTI_COLLECT_TIMING_INFORMATION
   record_action(34, WP_TimeRead());
#endif
#if WTI_FLEXMUX_ENABLE_PERFORMANCE_MONITORING

   WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE LineEndpointType;
   U32 count = 0;
   U32 max_stm1, max_stm0, max_tug2, max_tu;
   U32 stm1, stm0, tug2, tu;
   U8 result;

   memset(&LineEndpointType, 0, sizeof(WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE));

#ifdef VERBOSE
   printf("LO PATH:  device = %d, table_index = %d timestamp = 0x%x\n", device_id, table_index, timestamp);
#endif

   LineEndpointType.TransferType = lo_transfer_type_in_use;
   LineEndpointType.u.SDH.stm4 = 0;

   max_stm1 = 1;
   max_stm0 = 1;
   max_tug2 = 1;
   max_tu   = 3;

   for (stm1 = 0; stm1 < max_stm1; ++stm1)
      for (stm0 = 0; stm0 < max_stm0; ++stm0)
         for (tug2 = 0; tug2 < max_tug2; ++tug2)
            for (tu = 0; tu < max_tu; ++tu)
            {
               LineEndpointType.u.SDH.stm1 = stm1;
               LineEndpointType.u.SDH.stm0 = stm0;
               LineEndpointType.u.SDH.tug2 = tug2;
               LineEndpointType.u.SDH.tu   = tu;

               result = WPX_FRMR_STATUS_PM_SONET_SDH_LO_PATH_ReadData(device_id, table_index,
                        &LineEndpointType, WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_V5_BIP,
                        &count);

               if (result)
                  printf("    ---->  LO_Path_ReadData for V5_BIP failed on %d:%d:%d:%d on device %d with %s\n",
                         LineEndpointType.u.SDH.stm1,
                         LineEndpointType.u.SDH.stm0,
                         LineEndpointType.u.SDH.tug2,
                         LineEndpointType.u.SDH.tu,
                         device_id,
                         OMIINO_FRAMER_ErrorCodeToTxt(result));
#ifdef VERBOSE
               else if (count > 0)
                  printf("    ---->  V5_BIP count on %d:%d:%d:%d is %d\n", stm1, stm0, tug2, tu, count);
#endif

               result = WPX_FRMR_STATUS_PM_SONET_SDH_LO_PATH_ReadData(device_id, table_index,
                        &LineEndpointType, WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_V5_REI,
                        &count);

               if (result)
                  printf("    ---->  LO_Path_ReadData for V5_REI failed on %d:%d:%d:%d on device %d with %s\n",
                         LineEndpointType.u.SDH.stm1,
                         LineEndpointType.u.SDH.stm0,
                         LineEndpointType.u.SDH.tug2,
                         LineEndpointType.u.SDH.tu,
                         device_id,
                         OMIINO_FRAMER_ErrorCodeToTxt(result));
#ifdef VERBOSE
               else if (count > 0)
                  printf("    ---->  V5_REI count on %d:%d:%d:%d is %d\n", stm1, stm0, tug2, tu, count);
#endif
            }
#endif
}

void service_pm_pdh_data(WP_U8 device_id, WP_U8 table_index, WP_U32 timestamp)
{
#if WTI_COLLECT_TIMING_INFORMATION
   record_action(35, WP_TimeRead());
#endif
#if WTI_FLEXMUX_ENABLE_PERFORMANCE_MONITORING
#ifdef VERBOSE
   printf("PDH:  table_index = %d timestamp = 0x%x\n", table_index, timestamp);
#endif
#endif
}

#if WTI_FLEXMUX_ENABLE_PERFORMANCE_MONITORING

void cb_pm_port(WP_U8 device_id, WP_U8 table_index, WP_U32 timestamp)
{
   framer_task task;
   APP_PM_TASK *pm_task = &task.u.pm_task;

   task.task_type = PM_PORT_TYPE;
   pm_task->device = device_id;
   pm_task->table_index = table_index;
   pm_task->timestamp = timestamp;

   add_framer_task(the_pm_task_list, &task);
#if WTI_COLLECT_TIMING_INFORMATION
   record_action(36, WP_TimeRead());
#endif
}

void cb_pm_ho_path(WP_U8 device_id, WP_U8 table_index, WP_U32 timestamp)
{
   framer_task task;
   APP_PM_TASK *pm_task = &task.u.pm_task;

   task.task_type = PM_HO_PATH_TYPE;
   pm_task->device = device_id;
   pm_task->table_index = table_index;
   pm_task->timestamp = timestamp;

   add_framer_task(the_pm_task_list, &task);
#if WTI_COLLECT_TIMING_INFORMATION
   record_action(37, WP_TimeRead());
#endif
}

void cb_pm_lo_path(WP_U8 device_id, WP_U8 table_index, WP_U32 timestamp)
{
   framer_task task;
   APP_PM_TASK *pm_task = &task.u.pm_task;

   task.task_type = PM_LO_PATH_TYPE;
   pm_task->device = device_id;
   pm_task->table_index = table_index;
   pm_task->timestamp = timestamp;

   add_framer_task(the_pm_task_list, &task);
#if WTI_COLLECT_TIMING_INFORMATION
   record_action(38, WP_TimeRead());
#endif
}

void cb_pm_pdh(WP_U8 device_id, WP_U8 table_index, WP_U32 timestamp)
{
   framer_task task;
   APP_PM_TASK *pm_task = &task.u.pm_task;

   task.task_type = PM_PDH_TYPE;
   pm_task->device = device_id;
   pm_task->table_index = table_index;
   pm_task->timestamp = timestamp;

   add_framer_task(the_pm_task_list, &task);
#if WTI_COLLECT_TIMING_INFORMATION
   record_action(39, WP_TimeRead());
#endif
}

#endif

void process_framer_task_list(void)
{
   framer_task a_task;
   APP_PORT_ALARM_TASK   *port_alarm;
   APP_PATH_ALARM_TASK   *path_alarm;
   APP_PDH_ALARM_TASK    *pdh_alarm;
   APP_PM_TASK           *pm_task;
   APP_K1_K2_SIGNAL_TASK *k1k2_signal;
   APP_S1_SIGNAL_TASK    *s1_signal;

   while (1)
   {
#if WTI_COLLECT_TRACE_INFORMATION
      record_action(48, WP_TimeRead());
#endif

      WTI_flexmux_take_server_lock();

#if WTI_COLLECT_TIMING_INFORMATION
      record_action(49, WP_TimeRead());
#endif

      while (WP_TRUE)
      {
         if (next_framer_task(the_alarm_task_list, &a_task) != NULL)
         {
            switch (a_task.task_type)
            {
               case PORT_ALARM_TYPE:

                  port_alarm = &a_task.u.port_alarm_task;

                  service_port_alarm(port_alarm->device, port_alarm->line_port_id,
                                     port_alarm->alarm_category, port_alarm->is_asserted);

                  break;

               case PATH_ALARM_TYPE:

                  path_alarm = &a_task.u.path_alarm_task;

                  service_path_alarm(path_alarm->device, &path_alarm->endpoint,
                                     path_alarm->alarm_category, path_alarm->is_asserted);

                  break;

               case PDH_ALARM_TYPE:

                  pdh_alarm = &a_task.u.pdh_alarm_task;

                  service_pdh_alarm(pdh_alarm->device, pdh_alarm->socket_client_pdh,
                                    pdh_alarm->alarm_category, pdh_alarm->is_asserted);

                  break;

               default:

                  break;
            }
         }

         else if (next_framer_task(the_signal_task_list, &a_task) != NULL)
         {
            switch(a_task.task_type)
            {
               case K1K2_SIGNAL_TYPE:

                  k1k2_signal = &a_task.u.k1k2_signal_task;

                  service_k1_k2_signal(k1k2_signal->device, k1k2_signal->line_port_id,
                                       k1k2_signal->k1_value, k1k2_signal->k2_value);

                  break;

               case S1_SIGNAL_TYPE:

                  s1_signal = &a_task.u.s1_signal_task;

                  service_s1_signal(s1_signal->device, s1_signal->line_port_id, s1_signal->s1_value);

                  break;

               default:

                  break;
            }
         }

         else if (next_framer_task(the_pm_task_list, &a_task) != NULL)
         {
            switch (a_task.task_type)
            {
               case PM_PORT_TYPE:

                  pm_task = &a_task.u.pm_task;

                  service_pm_port_data(pm_task->device, pm_task->table_index, pm_task->timestamp);

                  break;

               case PM_HO_PATH_TYPE:

                  pm_task = &a_task.u.pm_task;

                  service_pm_ho_path_data(pm_task->device, pm_task->table_index, pm_task->timestamp);

                  break;

               case PM_LO_PATH_TYPE:

                  pm_task = &a_task.u.pm_task;

                  service_pm_lo_path_data(pm_task->device, pm_task->table_index, pm_task->timestamp);

                  break;

               case PM_PDH_TYPE:

                  pm_task = &a_task.u.pm_task;

                  service_pm_pdh_data(pm_task->device, pm_task->table_index, pm_task->timestamp);

                  break;

               default:

                   break;
            }
         }

         else
            break;
      }
   }
}

void WTI_enable_alarms(int type)
{
#if WTI_FLEXMUX_ENABLE_ALARMS
   WP_boolean is_sdh = (type == 0);
   int j;
   WP_U8 status;
   int start_alarm, last_alarm;

   WPX_FRMR_RegisterSonetSdhPathAlarmCallback((void *)(int)&cb_path_alarm);
   WPX_FRMR_RegisterSonetSdhPortAlarmCallback((void *)(int)&cb_port_alarm);
   WPX_FRMR_RegisterSocketClientPdhAlarmCallback((void *)(int)&cb_pdh_alarm);
   WPX_FRMR_RegisterSonetSdhSectionK1K2AnnounceCallback((void *)(int)&cb_k1k2_signal);
   WPX_FRMR_RegisterAlarmMapCallback((void *)(int)&cb_alarm_map_complete);

   if (is_sdh)
   {
      start_alarm = WPX_UFE_FRAMER_SDH_LOS;
      last_alarm = WPX_UFE_FRAMER_SDH_TU_LOP;
   }
   else
   {
      start_alarm = WPX_UFE_FRAMER_SONET_LOS;
      last_alarm = WPX_UFE_FRAMER_SONET_LOP_P;
   }

   for (j = start_alarm; j <= last_alarm; ++j)
   {
      status = WPX_FRMR_DEV_DRV_SONET_SDH_EnableAlarm(j, j);
      if (status != WPX_UFE_FRAMER_OK)
      {
         printf("********************************************************************\n"
                "   %d     SDH/SONET EVENT %d FAILED with %s\n"
                "********************************************************************\n",
                __LINE__, j, OMIINO_FRAMER_ErrorCodeToTxt(status));
         WTI_FlexmuxCheckStatus("WTI_enable_alarms_and_performance_monitoring", status, __LINE__);
      }
   }

#if 0
   if (is_sdh)
   {
      /* Turn these alarms back off since they seem to cause an excessively large number of alarms
       * to be reported */

      for (j = WPX_UFE_FRAMER_SDH_LP_TIM; j <= WPX_UFE_FRAMER_SDH_LP_PLM; ++j)
      {
         status = WPX_FRMR_DEV_DRV_SONET_SDH_DisableAlarm(j, j);
         if (status != WPX_UFE_FRAMER_OK)
         {
            printf("********************************************************************\n"
                   "   %d     SDH/SONET EVENT %d FAILED with %s\n"
                   "********************************************************************\n",
                   __LINE__, j, OMIINO_FRAMER_ErrorCodeToTxt(status));
            WTI_FlexmuxCheckStatus("WTI_enable_alarms_and_performance_monitoring", status, __LINE__);
         }
      }
   }
#endif

   if (is_sdh)
   {
      start_alarm = WPX_UFE_FRAMER_SDH_RS_EXC;
      last_alarm = WPX_UFE_FRAMER_SDH_RS_DEG;
   }
   else
   {
      start_alarm = WPX_UFE_FRAMER_SONET_EXC_S;
      last_alarm = WPX_UFE_FRAMER_SONET_DEG_S;
   }

   for (j = start_alarm; j <= last_alarm; ++j)
   {
      status = WPX_FRMR_DEV_DRV_SONET_SDH_EnableAlarm(j, j);
      if (status != WPX_UFE_FRAMER_OK)
      {
         printf("********************************************************************\n"
                "   %d     SDH/SONET EVENT %d FAILED with %s\n"
                "********************************************************************\n",
                __LINE__, j, OMIINO_FRAMER_ErrorCodeToTxt(status));
         WTI_FlexmuxCheckStatus("WTI_enable_alarms_and_performance_monitoring", status, __LINE__);
      }
   }

   for (j = WPX_UFE_FRAMER_SOCKET_CLIENT_E_RFI; j < WPX_UFE_FRAMER_SOCKET_CLIENT_MAX_ALARM_CATEGORIES; ++j)
   {
      status = WPX_FRMR_DEV_DRV_SOCKET_EnableAlarm(j, j);
      if (status != WPX_UFE_FRAMER_OK)
      {
         printf("********************************************************************\n"
                "          SOCKET EVENT %d FAILED\n"
                "********************************************************************\n", j);
         WTI_FlexmuxCheckStatus("WTI_enable_alarms_and_performance_monitoring", status, __LINE__);
      }
#if DEBUG
      else
         printf("%s enabled\n", socket_client_side_alarm_names[j]);
#endif
   }

   printf("***************** ALARMS ENABLED ************************************\n");

#endif
}

void WTI_enable_performance_monitoring(int type)
{
#if WTI_FLEXMUX_ENABLE_PERFORMANCE_MONITORING

   WPX_FRMR_PM_RegisterSonetSdhPortAnnounceCallback(&cb_pm_port);
   WPX_FRMR_PM_RegisterSonetSdhHOPathAnnounceCallback(&cb_pm_ho_path);
   WPX_FRMR_PM_RegisterSonetSdhLOPathAnnounceCallback(&cb_pm_lo_path);
   WPX_FRMR_PM_RegisterSocketClientPdhAnnounceCallback(&cb_pm_pdh);

#endif
}

void WTI_enable_performance_monitoring_points(int type, int is_e1, int vc_type, WUFE_un_framed framing_mode)
{
#if WTI_FLEXMUX_ENABLE_PERFORMANCE_MONITORING

   WP_boolean is_sdh = (type == 0);
   WP_U32 transaction_number;
   U8 result;

   WP_U32 number_lo_points = 0;
   WP_U32 number_hi_points = 0;

   /* for SDH: stm1, stm0, tug2, tu */
   WP_U32 stm1, stm0, tug2, tu;
   WP_U32 max_stm1, max_stm0;

   /* for SONET:  sts3, sts1, vt_group, vt */
   WP_U32 sts3, sts1, vt_group, vt;

   WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE LineEndpointType;

   WP_U32 device_id;

   /* Enable Port PM */
   static WP_U32 line_port = 0;

   for (device_id = 0; device_id < WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES; ++device_id)
   {
      WPX_FRMR_PM_SONET_SDH_Port_EnableMonitoring(101, device_id, line_port,
            WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_B1);
      WPX_FRMR_PM_SONET_SDH_Port_EnableMonitoring(102, device_id, line_port,
            WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_B2);
      WPX_FRMR_PM_SONET_SDH_Port_EnableMonitoring(103, device_id, line_port,
            WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_M1);

      if (is_e1)
      {
         /* For HO paths */
         LineEndpointType.u.SDH.stm4 = 0;
         LineEndpointType.u.SDH.tug2 = 0;
         LineEndpointType.u.SDH.tu   = 0;

         transaction_number = 200;

         if (is_sdh)
         {
            if (vc_type == 0)
               LineEndpointType.TransferType = WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC3;
            else
               LineEndpointType.TransferType = WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC4;

            ho_transfer_type_in_use = LineEndpointType.TransferType;

            if (LineEndpointType.TransferType == WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC3)
               max_stm0 = 3;
            else
               max_stm0 = 1;

            max_stm1 = 4;

            for (stm1 = 0; stm1 < max_stm1; ++stm1)
               for (stm0 = 0; stm0 < max_stm0; ++stm0)
               {
                  LineEndpointType.u.SDH.stm1 = stm1;
                  LineEndpointType.u.SDH.stm0 = stm0;

                  result = WPX_FRMR_PM_SONET_SDH_HO_Path_EnableMonitoring(
                                 transaction_number++, device_id, &LineEndpointType,
                                 WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_B3);
                  if (result)
                     printf("B3 failure on %d:%d:%d:%d on device %d --> %s\n",
                            LineEndpointType.u.SDH.stm1,
                            LineEndpointType.u.SDH.stm0,
                            LineEndpointType.u.SDH.tug2,
                            LineEndpointType.u.SDH.tu,
                            device_id,
                            OMIINO_FRAMER_ErrorCodeToTxt(result));
#if DEBUG
                  else
                     printf("B3 enabled on %d:%d:%d:%d --> %d\n", stm1, stm0, 0, 0, result);
#endif

                  result = WPX_FRMR_PM_SONET_SDH_HO_Path_EnableMonitoring(
                                 transaction_number++, device_id, &LineEndpointType,
                                 WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_G1);
                  if (result)
                     printf("G1 failure on %d:%d:%d:%d on device %d --> %s\n",
                            LineEndpointType.u.SDH.stm1,
                            LineEndpointType.u.SDH.stm0,
                            LineEndpointType.u.SDH.tug2,
                            LineEndpointType.u.SDH.tu,
                            device_id,
                            OMIINO_FRAMER_ErrorCodeToTxt(result));
#if DEBUG
                  else
                     printf("G1 enabled on %d:%d:%d:%d --> %d\n", stm1, stm0, 0, 0, result);
#endif
               }

            LineEndpointType.TransferType = WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC12;

            for (stm1 = 0; stm1 < 1; ++stm1)
               for (stm0 = 0; stm0 < 1; ++stm0)
                  for (tug2 = 0; tug2 < 1; ++tug2)
                     for (tu = 0; tu < 3; ++tu)
                     {
                        LineEndpointType.u.SDH.stm1 = stm1;
                        LineEndpointType.u.SDH.stm0 = stm0;
                        LineEndpointType.u.SDH.tug2 = tug2;
                        LineEndpointType.u.SDH.tu = tu;

                        result = WPX_FRMR_PM_SONET_SDH_LO_Path_EnableMonitoring(
                                       transaction_number++, device_id, &LineEndpointType,
                                       WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_V5_BIP);
                        if (result)
                           printf("V5 BIP failure on %d:%d:%d:%d on device %d --> %s\n",
                                  LineEndpointType.u.SDH.stm1,
                                  LineEndpointType.u.SDH.stm0,
                                  LineEndpointType.u.SDH.tug2,
                                  LineEndpointType.u.SDH.tu,
                                  device_id,
                                  OMIINO_FRAMER_ErrorCodeToTxt(result));
                        else
                           ++number_lo_points;

                        result = WPX_FRMR_PM_SONET_SDH_LO_Path_EnableMonitoring(
                                       transaction_number++, device_id, &LineEndpointType,
                                       WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_V5_REI);
                        if (result)
                           printf("V5 REI failure on %d:%d:%d:%d on device %d --> %s\n",
                                  LineEndpointType.u.SDH.stm1,
                                  LineEndpointType.u.SDH.stm0,
                                  LineEndpointType.u.SDH.tug2,
                                  LineEndpointType.u.SDH.tu,
                                  device_id,
                                  OMIINO_FRAMER_ErrorCodeToTxt(result));
                        else
                           ++number_lo_points;

                        result = WPX_FRMR_SONET_SDH_LO_PATH_V5_BIP2_SetExcessiveTh(800,
                                       device_id, &LineEndpointType, WPX_UFE_FRAMER_THRESHOLD_10E4);
                        if (result)
                           printf("V5 BIPS2 SetExcTh failure on %d:%d:%d:%d on device %d --> %s\n",
                                  LineEndpointType.u.SDH.stm1,
                                  LineEndpointType.u.SDH.stm0,
                                  LineEndpointType.u.SDH.tug2,
                                  LineEndpointType.u.SDH.tu,
                                  device_id,
                                  OMIINO_FRAMER_ErrorCodeToTxt(result));

                        result = WPX_FRMR_SONET_SDH_LO_PATH_V5_BIP2_SetSigDegradeTh(801, 
                                       device_id, &LineEndpointType, WPX_UFE_FRAMER_THRESHOLD_10E6);
                        if (result)
                           printf("V5 BIPS2 SetSigDeg failure on %d:%d:%d:%d on device %d --> %s\n",
                                  LineEndpointType.u.SDH.stm1,
                                  LineEndpointType.u.SDH.stm0,
                                  LineEndpointType.u.SDH.tug2,
                                  LineEndpointType.u.SDH.tu,
                                  device_id,
                                  OMIINO_FRAMER_ErrorCodeToTxt(result));
                     }
#ifdef VERBOSE
            printf("****************** number_lo_points %d\n", number_lo_points);
#endif
         }
         else
         {
            LineEndpointType.TransferType = WPX_UFE_FRAMER_WUFE_SONET_TYPE_VT2;

            for (sts3 = 0; sts3 < 4; ++sts3)
               for (sts1 = 0; sts1 < 3; ++sts1)
                  for (vt_group = 0; vt_group < 1; ++vt_group)
                     for (vt = 0; vt < 1; ++vt)
                     {
                        LineEndpointType.u.SONET.sts3     = sts3;
                        LineEndpointType.u.SONET.sts1     = sts1;
                        LineEndpointType.u.SONET.vt_group = vt_group;
                        LineEndpointType.u.SONET.vt       = vt;

                        result = WPX_FRMR_PM_SONET_SDH_HO_Path_EnableMonitoring(
                                       transaction_number++, device_id, &LineEndpointType,
                                       WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_B3);
                        if (result)
                           printf("B3 failure on %d:%d:%d:%d --> %d\n", sts3, sts1, vt_group, vt, result);
                        else
                           printf("B3 enabled on %d:%d:%d:%d --> %d\n", sts3, sts1, vt_group, vt, result);

                        result = WPX_FRMR_PM_SONET_SDH_HO_Path_EnableMonitoring(
                                       transaction_number++, device_id, &LineEndpointType,
                                       WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_G1);
                        if (result)
                           printf("G1 failure on %d:%d:%d:%d --> %d\n", sts3, sts1, vt_group, vt, result);
                     }
         }

         /* Limited number of points can be concurrently monitored.  CRC monitoring performed only if framed */

         if (framing_mode == WUFE_FRAMED)
         {
            int j, k;
            WP_U8 result;

            for (j = k = 0; k < 70 ; ++j)
            {
               if ((j % 4) < 3)
               {
                  if ((result = WPX_FRMR_PM_E1_EnableMonitoringCrc(
                                      transaction_number++, device_id, j)) != WPX_UFE_FRAMER_OK)
                  {
                     printf("E1 CRC Monitoring failed on %d with %s\n", j, OMIINO_FRAMER_ErrorCodeToTxt(result));
                     break;
                  }
                  else
                     ++k;
               }
            }
         }
      }
      else
      {
         /* For HO paths */
         LineEndpointType.u.SDH.stm4 = 0;
         LineEndpointType.u.SDH.tug2 = 0;
         LineEndpointType.u.SDH.tu   = 0;

         transaction_number = 200;

         if (is_sdh)
         {
            if (vc_type == 0)
               LineEndpointType.TransferType = WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC3;
            else
               LineEndpointType.TransferType = WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC4;

            ho_transfer_type_in_use = LineEndpointType.TransferType;

            if (LineEndpointType.TransferType == WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC3)
               max_stm0 = 3;
            else
               max_stm0 = 1;

            max_stm1 = 4;

            for (stm1 = 0; stm1 < max_stm1; ++stm1)
               for (stm0 = 0; stm0 < max_stm0; ++stm0)
               {
                  LineEndpointType.u.SDH.stm1 = stm1;
                  LineEndpointType.u.SDH.stm0 = stm0;

                  result = WPX_FRMR_PM_SONET_SDH_HO_Path_EnableMonitoring(
                           transaction_number++, device_id, &LineEndpointType,
                           WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_B3);
                  if (result)
                     printf("B3 failure on %d:%d:%d:%d on device %d --> %s\n",
                            LineEndpointType.u.SDH.stm1,
                            LineEndpointType.u.SDH.stm0,
                            LineEndpointType.u.SDH.tug2,
                            LineEndpointType.u.SDH.tu,
                            device_id,
                            OMIINO_FRAMER_ErrorCodeToTxt(result));
                  else
                     ++number_hi_points;

                  result = WPX_FRMR_PM_SONET_SDH_HO_Path_EnableMonitoring(
                           transaction_number++, device_id, &LineEndpointType,
                           WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_G1);
                  if (result)
                     printf("G1 failure on %d:%d:%d:%d on device %d --> %d\n",
                            LineEndpointType.u.SDH.stm1,
                            LineEndpointType.u.SDH.stm0,
                            LineEndpointType.u.SDH.tug2,
                            LineEndpointType.u.SDH.tu,
                            device_id,
                            OMIINO_FRAMER_ErrorCodeToTxt(result));
                  else
                     ++number_hi_points;
               }

#ifdef VERBOSE
            printf("****************** number_hi_points %d\n", number_hi_points);
#endif

            LineEndpointType.TransferType = WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC11;

            for (stm1 = 0; stm1 < 1; ++stm1)
               for (stm0 = 0; stm0 < 1; ++stm0)
                  for (tug2 = 0; tug2 < 1; ++tug2)
                     for (tu = 0; tu < 3; ++tu)
                     {
                        LineEndpointType.u.SDH.stm1 = stm1;
                        LineEndpointType.u.SDH.stm0 = stm0;
                        LineEndpointType.u.SDH.tug2 = tug2;
                        LineEndpointType.u.SDH.tu = tu;

                        result = WPX_FRMR_PM_SONET_SDH_LO_Path_EnableMonitoring(
                                 transaction_number++, device_id, &LineEndpointType,
                                 WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_V5_BIP);
                        if (result)
                           printf("V5_BIP failure on %d:%d:%d:%d on device %d --> %d\n",
                                  LineEndpointType.u.SDH.stm1,
                                  LineEndpointType.u.SDH.stm0,
                                  LineEndpointType.u.SDH.tug2,
                                  LineEndpointType.u.SDH.tu,
                                  device_id,
                                  OMIINO_FRAMER_ErrorCodeToTxt(result));
                        else
                           ++number_lo_points;

                        result = WPX_FRMR_PM_SONET_SDH_LO_Path_EnableMonitoring(
                                 transaction_number++, device_id, &LineEndpointType,
                                 WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_V5_REI);
                        if (result)
                           printf("V5_REI failure on %d:%d:%d:%d on device %d --> %d\n",
                                  LineEndpointType.u.SDH.stm1,
                                  LineEndpointType.u.SDH.stm0,
                                  LineEndpointType.u.SDH.tug2,
                                  LineEndpointType.u.SDH.tu,
                                  device_id,
                                  OMIINO_FRAMER_ErrorCodeToTxt(result));
                        else
                           ++number_lo_points;

                        result = WPX_FRMR_SONET_SDH_LO_PATH_V5_BIP2_SetExcessiveTh(800, 
                                       device_id, &LineEndpointType, WPX_UFE_FRAMER_THRESHOLD_10E4);
                        if (result)
                           printf("V5_BIPS2_SetExcTh failure on %d:%d:%d:%d on device %d --> %d\n",
                                  LineEndpointType.u.SDH.stm1,
                                  LineEndpointType.u.SDH.stm0,
                                  LineEndpointType.u.SDH.tug2,
                                  LineEndpointType.u.SDH.tu,
                                  device_id,
                                  OMIINO_FRAMER_ErrorCodeToTxt(result));
                        else
                           ++number_lo_points;

                        result = WPX_FRMR_SONET_SDH_LO_PATH_V5_BIP2_SetSigDegradeTh(801, 
                                       device_id, &LineEndpointType, WPX_UFE_FRAMER_THRESHOLD_10E6);
                        if (result)
                           printf("V5_BIPS2_SetSigDeg failure on %d:%d:%d:%d on device %d --> %d\n",
                                  LineEndpointType.u.SDH.stm1,
                                  LineEndpointType.u.SDH.stm0,
                                  LineEndpointType.u.SDH.tug2,
                                  LineEndpointType.u.SDH.tu,
                                  device_id,
                                  OMIINO_FRAMER_ErrorCodeToTxt(result));
                        else
                           ++number_lo_points;
                     }

#ifdef VERBOSE
            printf("****************** number_lo_points %d\n", number_lo_points);
#endif
         }
         else
         {
            LineEndpointType.TransferType = WPX_UFE_FRAMER_WUFE_SONET_TYPE_T1;

            for (sts3 = 0; sts3 < 4; ++sts3)
               for (sts1 = 0; sts1 < 3; ++sts1)
                  for (vt_group = 0; vt_group < 1; ++vt_group)
                     for (vt = 0; vt < 1; ++vt)
                     {
                        LineEndpointType.u.SONET.sts3     = sts3;
                        LineEndpointType.u.SONET.sts1     = sts1;
                        LineEndpointType.u.SONET.vt_group = vt_group;
                        LineEndpointType.u.SONET.vt       = vt;

                        result = WPX_FRMR_PM_SONET_SDH_HO_Path_EnableMonitoring(
                                       transaction_number++, device_id, &LineEndpointType,
                                       WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_B3);
                        if (result)
                           printf("B3 failure on %d:%d:%d:%d --> %d\n", sts3, sts1, vt_group, vt, result);

                        result = WPX_FRMR_PM_SONET_SDH_HO_Path_EnableMonitoring(
                                       transaction_number++, device_id, &LineEndpointType,
                                       WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_G1);
                        if (result)
                           printf("G1 failure on %d:%d:%d:%d --> %d\n", sts3, sts1, vt_group, vt, result);

                        result = WPX_FRMR_PM_SONET_SDH_LO_Path_EnableMonitoring(
                                       transaction_number++, device_id, &LineEndpointType,
                                       WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_V5_BIP);
                        if (result)
                           printf("V5_BIP failure on %d:%d:%d:%d --> %d\n", sts3, sts1, vt_group, vt, result);

                        result = WPX_FRMR_PM_SONET_SDH_LO_Path_EnableMonitoring(
                                       transaction_number++, device_id, &LineEndpointType,
                                       WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_V5_REI);
                        if (result)
                           printf("V5_REI failure on %d:%d:%d:%d --> %d\n", sts3, sts1, vt_group, vt, result);
                     }
         }

         if (framing_mode == WUFE_FRAMED)
         {
            int j, k;
            WP_U8 result;

            for (j = k = 0; k < 70 ; j += 2)
            {
               if ((result = WPX_FRMR_PM_T1_EnableMonitoringCrc(
                                   transaction_number++, device_id, j)) != WPX_UFE_FRAMER_OK)
               {
                  printf("T1 CRC Monitoring failed on %d with %s\n", j, OMIINO_FRAMER_ErrorCodeToTxt(result));
                  break;
               }
               else
                  ++k;
            }
         }
      }
   }

   printf("*****************     PM ENABLED ************************************\n");
#endif
}

void AIS_insertion_enable(char *strParm)
{
   WP_U32 device_id, dummy, res;
   WP_U8 status;

   res = sscanf(strParm, "%d %d", (int*)&dummy, (int *)&device_id);

   if (res != 2)
   {
      printf("INVALID NUMBER OF PARAMETERS\n");
      return;
   }

   status = WPX_FRMR_DEV_Enable_AIS_Insertion(400, device_id);
   if (status != WPX_UFE_FRAMER_OK)
   {
      printf("********************************************************************\n"
             "   %d     ENABLE AIS INSERTION FAILED with %s\n"
             "********************************************************************\n",
             __LINE__, OMIINO_FRAMER_ErrorCodeToTxt(status));
      WTI_FlexmuxCheckStatus("AIS_insertion_enable", status, __LINE__);
   }
}

void AIS_insertion_disable(char *strParm)
{
   WP_U32 device_id, dummy, res;
   WP_U8 status;

   res = sscanf(strParm, "%d %d", (int*)&dummy, (int *)&device_id);
   if (res != 2)
   {
      printf("INVALID NUMBER OF PARAMETERS\n");
      return;
   }

   status = WPX_FRMR_DEV_Disable_AIS_Insertion(400, device_id);
   if (status != WPX_UFE_FRAMER_OK)
   {
      printf("********************************************************************\n"
             "   %d     DISABLE AIS INSERTION FAILED with %s\n"
             "********************************************************************\n",
             __LINE__, OMIINO_FRAMER_ErrorCodeToTxt(status));
      WTI_FlexmuxCheckStatus("AIS_insertion_disable", status, __LINE__);
   }
}

void CLI_F_PdhAisTowardsLine(char *strParm)
{
   WP_U32 device_id, socket_pdh_id, dummy, res, enable;
   WP_U8 status;

   res = sscanf(strParm, "%d %d %d %d", (int*)&dummy, (int *)&device_id, (int *)&socket_pdh_id, (int *)&enable);
   if (res != 4)
   {
      printf("INVALID NUMBER OF PARAMETERS\n");
      return;
   }

   if (enable == 1)
   {
      status = WPX_FRMR_SOCKET_PDH_AIS_EnableTowardsLine(800, device_id, socket_pdh_id);
      if (status != WPX_UFE_FRAMER_OK)
      {
         printf("********************************************************************\n"
                "   %d     SOCKET PDH AIS EnableTowards Line FAILED with %s\n"
                "********************************************************************\n",
                __LINE__, OMIINO_FRAMER_ErrorCodeToTxt(status));
         WTI_FlexmuxCheckStatus("CLI_F_PdhAisTowardsLine", status, __LINE__);
      }
   }
   else
   {
      status = WPX_FRMR_SOCKET_PDH_AIS_DisableTowardsLine(801, device_id, socket_pdh_id);
      if (status != WPX_UFE_FRAMER_OK)
      {
         printf("********************************************************************\n"
                "   %d     SOCKET PDH AIS DisableTowards Line FAILED with %s\n"
                "********************************************************************\n",
                __LINE__, OMIINO_FRAMER_ErrorCodeToTxt(status));
         WTI_FlexmuxCheckStatus("CLI_F_PdhAisTowardsLine", status, __LINE__);
      }
   }
}

#endif
