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
 * Demonstrates handling UFE4 framer alarm and performance monitoring callbacks
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

#define VERBOSE
#define MAX_PORTS_NUMBER     8
#define MAX_HO_PATH_NUMBER   12
#define MAX_LO_PATH_NUMBER   336
#define MAX_PDH_NUMBER       336

#if WTI_FRAMER_OTE_TESTS
#undef VERBOSE

//WP_U32 build_personality_in_use = WPX_UFE_FRAMER_BUILD_PERSONALITY_1_PLUS_1_OC3_OC12;
//WP_U32 ho_transfer_type_in_use = WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC3;
//WP_U32 lo_transfer_type_in_use = WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC12;

WP_boolean test_passed;
WP_U32 print_data = 0;
WP_U32 check_type = 0;

/* These two arrays must be initialized to 0xFF in the begining of the test */
WP_U8 sonet_sdh_level_alarm_status[MAX_LO_PATH_NUMBER][WPX_UFE_FRAMER_SONET_SDH_MAX_ALARM_CATEGORIES];
WP_U8 pdh_level_alarm_status[MAX_PDH_NUMBER][WPX_UFE_FRAMER_SOCKET_CLIENT_MAX_ALARM_CATEGORIES];

WP_U32 pm_ports_counter[MAX_PORTS_NUMBER][WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_MAX_DATA_POINTS_PER_PORT] = {{0},{0}};
WP_U32 pm_ho_path_counter[MAX_HO_PATH_NUMBER][WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_MAX_DATA_POINTS_PER_HO_PATH] = {{0},{0}};
WP_U32 pm_lo_path_counter[MAX_LO_PATH_NUMBER][WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_MAX_DATA_POINTS_PER_LO_PATH] = {{0},{0}};
WP_U32 pm_pdhs_counter[MAX_PDH_NUMBER][(WPX_UFE_FRAMER_PDH_E1_T1_CRC + 1)] = {{0},{0}};

WP_U8 enable_pm_print_port[WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_MAX_DATA_POINTS_PER_PORT] = {0};
WP_U8 enable_pm_print_ho_path[WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_MAX_DATA_POINTS_PER_HO_PATH] = {0};
WP_U8 enable_pm_print_lo_path[WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_MAX_DATA_POINTS_PER_LO_PATH] = {0};
WP_U8 enable_pm_print_pdh[WPX_UFE_FRAMER_PDH_E1_T1_CRC + 1] = {0};

WP_boolean enable_peek_print = WP_TRUE;

#endif

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
#else /*!WTI_FLEXMUX_ENABLE_ALARMS*/
#if WTI_PRINT_ALARM_DATA
#define WTI_LINE_SIDE_ALARM_DISPLAY 1
#else
#define WTI_LINE_SIDE_ALARM_DISPLAY 0
#endif /* WTI_PRINT_ALARM_DATA*/
#endif

#if 1 //#if WTI_LINE_SIDE_ALARM_DISPLAY

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
#endif

static char *socket_client_side_alarm_names[] =
{
   "WPX_UFE_FRAMER_SOCKET_CLIENT_E_RFI",
   "WPX_UFE_FRAMER_SOCKET_CLIENT_A_RAI",
   "WPX_UFE_FRAMER_SOCKET_CLIENT_LOS_AIS",
   "WPX_UFE_FRAMER_SOCKET_CLIENT_OOF_LOF",
   "WPX_UFE_FRAMER_SOCKET_CLIENT_E3T3_XA",
   "WPX_UFE_FRAMER_SOCKET_CLIENT_E3T3_AIS",
   "WPX_UFE_FRAMER_SOCKET_CLIENT_E3T3_LOF",
   "WPX_UFE_FRAMER_SOCKET_CLIENT_E3T3_FEAC"
};

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

#if WTI_FRAMER_OTE_TESTS
   WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *info = p_SDH_Sonet_line_endpoint_type;
   WP_U32 lo_index, ho_index;


   if (((WPX_UFE_FRAMER_SDH_AU_AIS <= alarm_category) && (WPX_UFE_FRAMER_SDH_HP_DEG >= alarm_category))||   /* SDH*/
       ((WPX_UFE_FRAMER_SONET_LOM_P <= alarm_category) && (WPX_UFE_FRAMER_SONET_DEG_P >= alarm_category))|| /* SONET*/
       ((WPX_UFE_FRAMER_SONET_AIS_P <= alarm_category) && (WPX_UFE_FRAMER_SONET_LOP_P >= alarm_category)))  /* SONET*/
   {
         ho_index = 3 * info->u.SDH.stm1 + info->u.SDH.stm0;
      //printf("ho_index %d, stm1 %d, stm0 %d\n", ho_index, info->u.SDH.stm1, info->u.SDH.stm0);
      sonet_sdh_level_alarm_status[ho_index][alarm_category] = is_asserted;
   }
   else if (((WPX_UFE_FRAMER_SDH_TU_LOP >= alarm_category) && (WPX_UFE_FRAMER_SDH_LP_TIM <= alarm_category)) || /* SDH*/
            ((WPX_UFE_FRAMER_SONET_LOP_V >= alarm_category) && (WPX_UFE_FRAMER_SONET_TIM_V <= alarm_category))) /* SONET*/
   {
      lo_index = 84 * info->u.SDH.stm1 + 28 * info->u.SDH.stm0 + 4 * info->u.SDH.tug2 + info->u.SDH.tu;
//      printf("lo_index %d\n", lo_index);
      sonet_sdh_level_alarm_status[lo_index][alarm_category] = is_asserted;
   }
#else
#ifdef VERBOSE
   WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *info = p_SDH_Sonet_line_endpoint_type;

   printf("PATH ALARM:  device %d <%d:%d:%d:%d>: alarm_category = %s %s\n",
          device_id, info->u.SDH.stm1, info->u.SDH.stm0, info->u.SDH.tug2, info->u.SDH.tu,
          line_side_alarm_names[alarm_category], is_asserted ? "Asserted" : "Unasserted");
#endif /* VERBOSE */
#endif /* WTI_FRAMER_OTE_TESTS */
#endif /* WTI_FLEXMUX_ENABLE_ALARMS */
}

void service_port_alarm(WP_U8 device, WP_U8 line_port_id, WP_U8 alarm_category, WP_U8 is_asserted)
{
#if WTI_COLLECT_TIMING_INFORMATION
   record_action(21, WP_TimeRead());
#endif
#if WTI_FLEXMUX_ENABLE_ALARMS
#if WTI_FRAMER_OTE_TESTS
   sonet_sdh_level_alarm_status[line_port_id][alarm_category] = is_asserted;
#else
#ifdef VERBOSE
   printf("PORT ALARM:  device %d, line_port_id = %d, alarm_category = %s %s\n",
          device, line_port_id, line_side_alarm_names[alarm_category], is_asserted ? "Asserted" : "Unasserted");
#endif /* VERBOSE */
#endif /* WTI_FRAMER_OTE_TESTS  */
#endif /* WTI_FLEXMUX_ENABLE_ALARMS*/
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

//#if !WTI_FRAMER_OTE_TESTS
//#ifdef VERBOSE
   printf("PORT LOS ALARM:  device id %d, line_port_id = %d, %s %s\n", device, line_port_id,
          line_side_alarm_names[alarm_category], is_asserted ? " ON" : "OFF");
//#endif
//#endif

#if WTI_FRAMER_OTE_TESTS
   sonet_sdh_level_alarm_status[line_port_id][alarm_category] = is_asserted;
#endif
#if NO_APS_ON_LOS
#if !WTI_FRAMER_OTE_TESTS
     printf("NO SWITCHING PERFORMED\n");
#endif
   return;
#endif

   if ((result = WPX_FRMR_PL_SONET_SDH_PORT_GetForceState(device, line_port_id, &force_state)) == WPX_UFE_FRAMER_OK)
   {
#if 1//DEBUG
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
#if 1//DEBUG
            printf("APS to protected port for line port #%d\n", line_port_id);
#endif
         }
         else
            add_framer_task(the_alarm_task_list, task);

      }
      else if (force_state == WPX_UFE_FRAMER_PROTECTION_FORCE_B)
      {
#if !WTI_FRAMER_OTE_TESTS
         if (is_asserted)
         {
            result = WPX_FRMR_PL_SONET_SDH_PORT_ForceA(transaction++, device, line_port_id);
            WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_PORT_ForceA", result, __LINE__);
#if 1//DEBUG
            printf("APS to working port for line port #%d\n", line_port_id);
#endif
         }
         else
            add_framer_task(the_alarm_task_list, task);
#endif
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
#if !WTI_FRAMER_OTE_TESTS
#if WTI_FLEXMUX_ENABLE_ALARMS
#ifdef VERBOSE

   printf("K1K2 SIGNAL:  device = %d, iLinePort = %d K1 %d, K2 %d\n", device, line_port_id, k1, k2);

#endif
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
#if WTI_FRAMER_OTE_TESTS
   pdh_level_alarm_status[socket_client_pdh_id][alarm_category] = is_asserted;
#else
#ifdef VERBOSE
   if (alarm_category == WPX_UFE_FRAMER_SOCKET_CLIENT_E3T3_FEAC)
   {
      printf("PDH ALARM:  iSocketClientPDH = %d, alarm_category = %s and FEAC code is 0x%x\n",
             socket_client_pdh_id, socket_client_side_alarm_names[alarm_category], is_asserted);
   }
   else
   {
      printf("PDH ALARM:  iSocketClientPDH = %d, alarm_category = %s is %s\n",
             socket_client_pdh_id, socket_client_side_alarm_names[alarm_category], is_asserted ? "asserted" : "unasserted");
   }
#endif /* VERBOSE */
#endif /* WTI_FRAMER_OTE_TESTS */
#endif /* WTI_FLEXMUX_ENABLE_ALARMS */
}

#if WTI_FLEXMUX_ENABLE_ALARMS

void cb_alarm_map_complete(WP_U8 device)
{
   void cb_port_alarm(WP_U8 device, WP_U8 line_port_id, WP_U8 alarm_category, WP_U8 is_asserted);
   void cb_path_alarm(WP_U8 device_id,
                      WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *p_SDH_Sonet_line_endpoint_type,
                      WP_U8 alarm_category, WP_U8 is_asserted);
   void cb_pdh_alarm(WP_U8 device_id, WP_U32 iSocketClientPDH, U8 alarm_category, U8 is_asserted);

   WPX_FRMR_RegisterSonetSdhPortAlarmCallback(&cb_port_alarm);
   WPX_FRMR_RegisterSonetSdhPathAlarmCallback(&cb_path_alarm);
   WPX_FRMR_RegisterSocketClientPdhAlarmCallback(&cb_pdh_alarm);
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
#if !WTI_FRAMER_OTE_TESTS
       (alarm_category == WPX_UFE_FRAMER_SDH_LOF)    ||
       (alarm_category == WPX_UFE_FRAMER_SDH_MS_AIS) ||
#endif
       (alarm_category == WPX_UFE_FRAMER_SONET_LOS)  ||
       (alarm_category == WPX_UFE_FRAMER_SONET_LOF))
   {
//       if (is_asserted)
//       {
#if WTI_COLLECT_TIMING_INFORMATION
         record_action(27, WP_TimeRead());
#endif
         service_los_alarm(device, line_port_id, alarm_category, is_asserted, &task);
//       }
//       else
//          add_framer_task(the_alarm_task_list, &task);
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
   U32 max_stm4 = 0, max_stm1 = 0, max_stm0, max_tug2, max_tu;
   U32 stm4, stm1, stm0, tug2, tu;
   U8 result;
   U32 stm4_increment = 0, stm1_increment = 0;

   memset(&LineEndpointType, 0, sizeof(WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE));

#ifdef VERBOSE
   printf("HO PATH:  device = %d, table_index = %d timestamp = 0x%x\n", device_id, table_index, timestamp);
#endif

   LineEndpointType.TransferType = ho_transfer_type_in_use;

   if (LineEndpointType.TransferType == WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC3)
      max_stm0 = 3;
   else
      max_stm0 = 1;

   max_tug2 = max_tu = 1;
#if WTI_FRAMER_OTE_TESTS
   WP_U32 build_personality_in_use = CurrentBuildPersonality;   
#endif
   /* UFE412 build personalities */
   if (build_personality_in_use == WPX_UFE_FRAMER_BUILD_PERSONALITY_4_PLUS_4_OC3)
   {
      stm1_increment = 1;
      max_stm1 = 4;
      max_stm4 = 1;
      stm4_increment = 1;
   }
   else if (build_personality_in_use == WPX_UFE_FRAMER_BUILD_PERSONALITY_2_PLUS_2_OC3)
   {
      stm1_increment = 1;
      max_stm1 = 2;
      max_stm4 = 1;
      stm4_increment = 1;
   }
   else if (build_personality_in_use == WPX_UFE_FRAMER_BUILD_PERSONALITY_1_PLUS_1_OC3_OC12)
   {
      stm1_increment = 1;
      max_stm1 = 4;
      max_stm4 = 1;
      stm4_increment = 1;
   }
   /* UFE448 build personalities */
   else if (build_personality_in_use == WPX_UFE_FRAMER_BUILD_PERSONALITY_4_PLUS_4_OC3_OC12)
   {
      stm1_increment = 1;
      max_stm1 = 4;
      max_stm4 = 4;
      stm4_increment = 1;
   }
   else if (build_personality_in_use == WPX_UFE_FRAMER_BUILD_PERSONALITY_8_PLUS_8_OC3)
   {
      stm1_increment = 1;
      max_stm1 = 4;
      max_stm4 = 4;
      /* in this build personality we utilize only 2 STM4s (OC24 BW) but we use only
         the first STM4 on each core */
      stm4_increment = 2;
   }

   for (stm4 = 0; stm4 < max_stm4; stm4 += stm4_increment)
      for (stm1 = 0; stm1 < max_stm1; stm1 += stm1_increment)
         for (stm0 = 0; stm0 < max_stm0; ++stm0)
            for (tug2 = 0; tug2 < max_tug2; ++tug2)
               for (tu = 0; tu < max_tu; ++tu)
               {
                  LineEndpointType.u.SDH.stm4 = stm4;
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
   U32 max_stm4 = 0, max_stm1 = 0, max_stm0, max_tug2, max_tu;
   U32 stm4, stm1, stm0, tug2, tu;
   U32 stm4_increment = 0, stm1_increment = 0;
   U8 result;

   memset(&LineEndpointType, 0, sizeof(WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE));

#ifdef VERBOSE
   printf("LO PATH:  device = %d, table_index = %d timestamp = 0x%x\n", device_id, table_index, timestamp);
#endif

   LineEndpointType.TransferType = lo_transfer_type_in_use;

#if WTI_FRAMER_OTE_TESTS
   WP_U32 build_personality_in_use = CurrentBuildPersonality;   
#endif
   /* UFE412 build personalities */
   if (build_personality_in_use == WPX_UFE_FRAMER_BUILD_PERSONALITY_4_PLUS_4_OC3)
   {
      stm1_increment = 1;
      max_stm1 = 4;
      max_stm4 = 1;
      stm4_increment = 1;
   }
   else if (build_personality_in_use == WPX_UFE_FRAMER_BUILD_PERSONALITY_2_PLUS_2_OC3)
   {
      stm1_increment = 1;
      max_stm1 = 2;
      max_stm4 = 1;
      stm4_increment = 1;
   }
   else if (build_personality_in_use == WPX_UFE_FRAMER_BUILD_PERSONALITY_1_PLUS_1_OC3_OC12)
   {
      stm1_increment = 1;
      max_stm1 = 4;
      max_stm4 = 1;
      stm4_increment = 1;
   }
   /* UFE448 build personalities */
   else if (build_personality_in_use == WPX_UFE_FRAMER_BUILD_PERSONALITY_4_PLUS_4_OC3_OC12)
   {
      stm1_increment = 1;
      max_stm1 = 4;
      max_stm4 = 4;
      stm4_increment = 1;
   }
   else if (build_personality_in_use == WPX_UFE_FRAMER_BUILD_PERSONALITY_8_PLUS_8_OC3)
   {
      stm1_increment = 1;
      max_stm1 = 4;
      max_stm4 = 4;
      /* in this build personality we utilize only 2 STM4s (OC24 BW) but we use only
         the first STM4 on each core */
      stm4_increment = 2;
   }

   max_stm0 = 1;
   max_tug2 = 1;
   max_tu   = 3;

   for (stm4 = 0; stm4 < max_stm4; stm4 += stm4_increment)
      for (stm1 = 0; stm1 < max_stm1; stm1 += stm1_increment)
         for (stm0 = 0; stm0 < max_stm0; ++stm0)
            for (tug2 = 0; tug2 < max_tug2; ++tug2)
               for (tu = 0; tu < max_tu; ++tu)
               {
                  LineEndpointType.u.SDH.stm1 = stm4;
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

   U32 iSocketClientPDH = 0;
   U32 count = 0;
   U8 result;


   for (iSocketClientPDH=0; iSocketClientPDH<336; iSocketClientPDH+=28)
   {
      
   result =  WPX_FRMR_STATUS_PM_SocketPort_PDH_E3T3_ReadData(device_id, table_index, 
                                                             iSocketClientPDH, WPX_UFE_FRAMER_PDH_E3T3_PM_BIP, &count);

   if (result)
      printf("    ---->  PDH_ReadData for BIP failed on pdh %d on device %d with %s\n",
             iSocketClientPDH,
             device_id,
             OMIINO_FRAMER_ErrorCodeToTxt(result));
#ifdef VERBOSE
   else if (count > 0)
      printf("    ---->  PDH BIP count on pdh %d is %d\n", iSocketClientPDH, count);
#endif

   result =  WPX_FRMR_STATUS_PM_SocketPort_PDH_E3T3_ReadData(device_id, table_index, 
                                                             iSocketClientPDH, WPX_UFE_FRAMER_PDH_E3T3_PM_FEBE, &count);

   if (result)
      printf("    ---->  PDH_ReadData for FEBE failed on pdh %d on device %d with %s\n",
             iSocketClientPDH,
             device_id,
             OMIINO_FRAMER_ErrorCodeToTxt(result));
#ifdef VERBOSE
   else if (count > 0)
      printf("    ---->  PDH FEBE count on pdh %d is %d\n", iSocketClientPDH, count);
#endif
   }
   
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

   WPX_FRMR_RegisterSonetSdhPathAlarmCallback(&cb_path_alarm);
   WPX_FRMR_RegisterSonetSdhPortAlarmCallback(&cb_port_alarm);
   WPX_FRMR_RegisterSocketClientPdhAlarmCallback(&cb_pdh_alarm);
   WPX_FRMR_RegisterSonetSdhSectionK1K2AnnounceCallback(&cb_k1k2_signal);
   WPX_FRMR_RegisterAlarmMapCallback(&cb_alarm_map_complete);

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
          else
             printf("%s enabled\n", line_side_alarm_names[j]);
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
#if 1//DEBUG
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
   WP_U32 stm4, stm1, stm0 = 0, tug2, tu;
   U32 stm4_increment = 0, stm1_increment = 0;
   WP_U32 max_stm4 = 0, max_stm1 = 0, max_stm0 = 0;

   /* for SONET:  sts3, sts1, vt_group, vt */
   WP_U32 sts12, sts3, sts1, vt_group, vt;
   WP_U32 sts12_increment = 0, sts3_increment = 0;
   WP_U32 max_sts12 = 0, max_sts3 = 0;
   WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE LineEndpointType;

   WP_U32 device_id;

   /* Enable Port PM */
   static WP_U32 line_port = 0;

   /* UFE412 build personalities */
   if (build_personality_in_use == WPX_UFE_FRAMER_BUILD_PERSONALITY_4_PLUS_4_OC3)
   {
      max_stm1 = max_sts3 = 4;
      stm1_increment = sts3_increment = 1;
      max_stm4 = max_sts12 = 1;
      stm4_increment = sts12_increment = 1;
   }
   else if (build_personality_in_use == WPX_UFE_FRAMER_BUILD_PERSONALITY_2_PLUS_2_OC3)
   {
      max_stm1 = max_sts3 = 2;
      stm1_increment = sts3_increment = 1;
      max_stm4 = max_sts12 = 1;
      stm4_increment = sts12_increment = 1;
   }
   else if (build_personality_in_use == WPX_UFE_FRAMER_BUILD_PERSONALITY_1_PLUS_1_OC3_OC12)
   {
      max_stm1 = max_sts3 = 4;
      stm1_increment = sts3_increment = 1;
      max_stm4 = max_sts12 = 1;
      stm4_increment = sts12_increment = 1;
   }
   /* UFE448 build personalities */
   else if (build_personality_in_use == WPX_UFE_FRAMER_BUILD_PERSONALITY_4_PLUS_4_OC3_OC12)
   {
      max_stm1 = max_sts3 = 4;
      stm1_increment = sts3_increment = 1;
      max_stm4 = max_sts12 = 4;
      stm4_increment = sts12_increment = 1;
   }
   else if (build_personality_in_use == WPX_UFE_FRAMER_BUILD_PERSONALITY_8_PLUS_8_OC3)
   {
      max_stm1 = max_sts3 = 4;
      stm1_increment = sts3_increment = 1;
      max_stm4 = max_sts12 = 4;
      /* in this build personality we utilize only 2 STM4s (OC24 BW) but we use only
         the first STM4 on each core */
      stm4_increment = sts12_increment = 2;
   }

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

            for (stm4 = 0; stm4 < max_stm4; stm4 += stm4_increment)
               for (stm1 = 0; stm1 < max_stm1; stm1 += stm1_increment)
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

            for (stm4 = 0; stm4 < max_stm4; stm4 += stm4_increment)
               for (stm1 = 0; stm1 < max_stm1; stm1 += stm1_increment)
                  for (stm1 = 0; stm0 < max_stm0; ++stm0)
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

            for (sts12 = 0; sts12 < max_sts12; sts12 += sts12_increment)
               for (sts3 = 0; sts3 < max_sts3; sts3 += sts3_increment)
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
      else  /* T1 */
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

            for (stm4 = 0; stm4 < max_stm4; stm4 += stm4_increment)
               for (stm1 = 0; stm1 < max_stm1; stm1 += stm1_increment)
                  for (stm0 = 0; stm0 < max_stm0; ++stm0)
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

            for (sts12 = 0; sts12 < max_sts12; sts12 += sts12_increment)
               for (sts3 = 0; sts3 < max_sts3; sts3 += sts3_increment)
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

   res = sscanf(strParm, "%d %d", &dummy, &device_id);

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

   res = sscanf(strParm, "%d %d", &dummy, &device_id);
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

   res = sscanf(strParm, "%d %d %d %d", &dummy, &device_id, &socket_pdh_id, &enable);
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

void CLI_F_PdhFEACTowardsLine(char *strParm)
{
   WP_U32 device_id, socket_pdh_id, dummy, res, code;
   WP_U8 status;

   res = sscanf(strParm, "%d %d %d %d", &dummy, &device_id, &socket_pdh_id, &code);
   if (res != 4)
   {
      printf("INVALID NUMBER OF PARAMETERS\n");
      return;
   }

   status = WPX_FRMR_SOCKET_PDH_E3T3_SetFEAC(900, device_id, socket_pdh_id, code);
   if (status != WPX_UFE_FRAMER_OK)
   {
      printf("********************************************************************\n"
             "   %d     SOCKET PDH FEAC Towards Line FAILED with %s\n"
             "********************************************************************\n",
             __LINE__, OMIINO_FRAMER_ErrorCodeToTxt(status));
      WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_E3T3_SetFEAC", status, __LINE__);
   }
}

void CLI_F_PdhRdiTowardsLine(char *strParm)
{
   WP_U32 device_id, socket_pdh_id, dummy, res, enable;
   WP_U8 status;

   res = sscanf(strParm, "%d %d %d %d", &dummy, &device_id, &socket_pdh_id, &enable);
   if (res != 4)
   {
      printf("INVALID NUMBER OF PARAMETERS\n");
      return;
   }

   if (enable == 1)
   {
      status = WPX_FRMR_SOCKET_PDH_RDI_EnableTowardsLine(800, device_id, socket_pdh_id);
      if (status != WPX_UFE_FRAMER_OK)
      {
         printf("********************************************************************\n"
                "   %d     SOCKET PDH RDI EnableTowards Line FAILED with %s\n"
                "********************************************************************\n",
                __LINE__, OMIINO_FRAMER_ErrorCodeToTxt(status));
         WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_RDI_EnableTowardsLine", status, __LINE__);
      }
   }
   else
   {
      status = WPX_FRMR_SOCKET_PDH_RDI_DisableTowardsLine(801, device_id, socket_pdh_id);
      if (status != WPX_UFE_FRAMER_OK)
      {
         printf("********************************************************************\n"
                "   %d     SOCKET PDH RDI DisableTowards Line FAILED with %s\n"
                "********************************************************************\n",
                __LINE__, OMIINO_FRAMER_ErrorCodeToTxt(status));
         WTI_FlexmuxCheckStatus("CLI_F_PdhRdiTowardsLine", status, __LINE__);
      }
   }
}

#if WTI_FRAMER_OTE_TESTS
#if 1
WP_boolean alarm_is_port_level(WP_U32 alarm_category)
{
   WP_boolean result = WP_FALSE;

   switch(alarm_category)
   {
      case WPX_UFE_FRAMER_SDH_LOS:   /* SDH */
      case WPX_UFE_FRAMER_SDH_LOF:
      case WPX_UFE_FRAMER_SDH_RS_TIM:
      case WPX_UFE_FRAMER_SDH_MS_AIS:
      case WPX_UFE_FRAMER_SDH_MS_RDI:
      case WPX_UFE_FRAMER_SDH_OOF:
      case WPX_UFE_FRAMER_SDH_MS_EXC:
      case WPX_UFE_FRAMER_SDH_MS_DEG:
      case WPX_UFE_FRAMER_SDH_RS_EXC:
      case WPX_UFE_FRAMER_SDH_RS_DEG:
      case WPX_UFE_FRAMER_SONET_LOS: /* SONET */
      case WPX_UFE_FRAMER_SONET_LOF:
      case WPX_UFE_FRAMER_SONET_TIM_S:
      case WPX_UFE_FRAMER_SONET_AIS_L:
      case WPX_UFE_FRAMER_SONET_RDI_L:
      case WPX_UFE_FRAMER_SONET_OOF:
      case WPX_UFE_FRAMER_SONET_EXC_L:
      case WPX_UFE_FRAMER_SONET_DEG_L:
      case WPX_UFE_FRAMER_SONET_EXC_S:
      case WPX_UFE_FRAMER_SONET_DEG_S:
         result = WP_TRUE;
         break;
      default:
         result = WP_FALSE;
         break;

   }
   return result;

}

WP_boolean alarm_is_ho_level(WP_U32 alarm_category)
{
   WP_boolean result = WP_FALSE;

   switch(alarm_category)
   {
      case WPX_UFE_FRAMER_SDH_AU_AIS:  /* SDH */
      case WPX_UFE_FRAMER_SDH_AU_LOP:
      case WPX_UFE_FRAMER_SDH_HP_LOM:
      case WPX_UFE_FRAMER_SDH_HP_TIM:
      case WPX_UFE_FRAMER_SDH_HP_PLM:
      case WPX_UFE_FRAMER_SDH_HP_UNEQ:
      case WPX_UFE_FRAMER_SDH_HP_RDI:
      case WPX_UFE_FRAMER_SDH_HP_EXC:
      case WPX_UFE_FRAMER_SDH_HP_DEG:

      case WPX_UFE_FRAMER_SONET_LOM_P:  /* SONET */
      case WPX_UFE_FRAMER_SONET_TIM_P:
      case WPX_UFE_FRAMER_SONET_PLM_P:
      case WPX_UFE_FRAMER_SONET_UNEQ_P:
      case WPX_UFE_FRAMER_SONET_RDI_P:
      case WPX_UFE_FRAMER_SONET_EXC_P:
      case WPX_UFE_FRAMER_SONET_DEG_P:
      case WPX_UFE_FRAMER_SONET_AIS_P:
      case WPX_UFE_FRAMER_SONET_LOP_P:

         result = WP_TRUE;
         break;
      default:
         result = WP_FALSE;
         break;

   }
   return result;

}
WP_boolean alarm_is_lo_level(WP_U32 alarm_category)
{
   WP_boolean result = WP_FALSE;

   switch(alarm_category)
   {
      case WPX_UFE_FRAMER_SDH_LP_TIM:   /* SDH */
      case WPX_UFE_FRAMER_SDH_LP_PLM:
      case WPX_UFE_FRAMER_SDH_LP_UNEQ:
      case WPX_UFE_FRAMER_SDH_LP_RDI:
      case WPX_UFE_FRAMER_SDH_LP_EXC:
      case WPX_UFE_FRAMER_SDH_LP_DEG:
      case WPX_UFE_FRAMER_SDH_TU_AIS:
      case WPX_UFE_FRAMER_SDH_TU_LOP:

      case WPX_UFE_FRAMER_SONET_TIM_V:  /* SONET */
      case WPX_UFE_FRAMER_SONET_PLM_V:
      case WPX_UFE_FRAMER_SONET_UNEQ_V:
      case WPX_UFE_FRAMER_SONET_RDI_V:
      case WPX_UFE_FRAMER_SONET_EXC_V:
      case WPX_UFE_FRAMER_SONET_DEG_V:
      case WPX_UFE_FRAMER_SONET_AIS_V:
      case WPX_UFE_FRAMER_SONET_LOP_V:
         result = WP_TRUE;
         break;
      default:
         result = WP_FALSE;
         break;

   }
   return result;
}
WP_U32 get_alarm_category(WP_U32 alarms_index)
{
   WP_U32 result;

   switch(alarms_index)
   {
      case WPX_UFE_FRAMER_SDH_LOS:
      case WPX_UFE_FRAMER_SDH_LOF:
      case WPX_UFE_FRAMER_SDH_RS_TIM:
      case WPX_UFE_FRAMER_SDH_MS_AIS:
      case WPX_UFE_FRAMER_SDH_MS_RDI:
      case WPX_UFE_FRAMER_SDH_OOF:
      case WPX_UFE_FRAMER_SDH_MS_EXC:
      case WPX_UFE_FRAMER_SDH_MS_DEG:
      case WPX_UFE_FRAMER_SDH_AU_AIS:
      case WPX_UFE_FRAMER_SDH_AU_LOP:
      case WPX_UFE_FRAMER_SDH_HP_LOM:
      case WPX_UFE_FRAMER_SDH_HP_TIM:
      case WPX_UFE_FRAMER_SDH_HP_PLM:
      case WPX_UFE_FRAMER_SDH_HP_UNEQ:
      case WPX_UFE_FRAMER_SDH_HP_RDI:
      case WPX_UFE_FRAMER_SDH_HP_EXC:
      case WPX_UFE_FRAMER_SDH_HP_DEG:
      case WPX_UFE_FRAMER_SDH_LP_TIM:
      case WPX_UFE_FRAMER_SDH_LP_PLM:
      case WPX_UFE_FRAMER_SDH_LP_UNEQ:
      case WPX_UFE_FRAMER_SDH_LP_RDI:
      case WPX_UFE_FRAMER_SDH_LP_EXC:
      case WPX_UFE_FRAMER_SDH_LP_DEG:
      case WPX_UFE_FRAMER_SDH_TU_AIS:
      case WPX_UFE_FRAMER_SDH_TU_LOP:
      case WPX_UFE_FRAMER_SDH_RS_EXC:
      case WPX_UFE_FRAMER_SDH_RS_DEG:
         result = WPX_UFE_FRAMER_DEVICE_MODE_SDH;
         break;

      case WPX_UFE_FRAMER_SONET_LOS:
      case WPX_UFE_FRAMER_SONET_LOF:
      case WPX_UFE_FRAMER_SONET_TIM_S:
      case WPX_UFE_FRAMER_SONET_AIS_L:
      case WPX_UFE_FRAMER_SONET_RDI_L:
      case WPX_UFE_FRAMER_SONET_OOF:
      case WPX_UFE_FRAMER_SONET_EXC_L:
      case WPX_UFE_FRAMER_SONET_DEG_L:
      case WPX_UFE_FRAMER_SONET_LOM_P:
      case WPX_UFE_FRAMER_SONET_TIM_P:
      case WPX_UFE_FRAMER_SONET_PLM_P:
      case WPX_UFE_FRAMER_SONET_UNEQ_P:
      case WPX_UFE_FRAMER_SONET_RDI_P:
      case WPX_UFE_FRAMER_SONET_EXC_P:
      case WPX_UFE_FRAMER_SONET_DEG_P:
      case WPX_UFE_FRAMER_SONET_TIM_V:
      case WPX_UFE_FRAMER_SONET_PLM_V:
      case WPX_UFE_FRAMER_SONET_UNEQ_V:
      case WPX_UFE_FRAMER_SONET_RDI_V:
      case WPX_UFE_FRAMER_SONET_EXC_V:
      case WPX_UFE_FRAMER_SONET_DEG_V:
      case WPX_UFE_FRAMER_SONET_AIS_V:
      case WPX_UFE_FRAMER_SONET_LOP_V:
      case WPX_UFE_FRAMER_SONET_AIS_P:
      case WPX_UFE_FRAMER_SONET_LOP_P:
      case WPX_UFE_FRAMER_SONET_EXC_S:
      case WPX_UFE_FRAMER_SONET_DEG_S:
         result = WPX_UFE_FRAMER_DEVICE_MODE_SONET;
         break;
      default:
         result = WPX_UFE_FRAMER_MAX_DEVICE_MODES;
         break;

   }
   return result;
}

WP_boolean alarm_supported_by_device_current_mode(WP_U32 device_currnt_mode, WP_U32 alarm_index)
{
   if (device_currnt_mode == get_alarm_category(alarm_index))
   {
      return WP_TRUE;
   }
   else
   {
      return WP_FALSE;
   }

}
#endif

void CLI_F_AlarmsStatusCheck(char *StrPrm)
{
#if WTI_FLEXMUX_ENABLE_ALARMS

   WP_U32 cmd, res, alarm_index, facility_index, max_facility_index=0, pdh_index;

   res = sscanf(StrPrm, "%d %d", &cmd, &print_data);
   printf("cmd %d, print_data %d\n", cmd, print_data);

   if (res != 2)
   {
      printf("CLI_F_GetAlarmState: Invalid number of parameters, line: %d", __LINE__);
      return;
   }

   if (1 < print_data)
   {
      printf("CLI_F_GetAlarmState, line: %d", __LINE__);
      return;
   }

   test_passed = WP_TRUE;

   for (alarm_index=WPX_UFE_FRAMER_SDH_LOS;alarm_index<WPX_UFE_FRAMER_SONET_SDH_MAX_ALARM_CATEGORIES;alarm_index++)
   {
      if (WP_TRUE == alarm_supported_by_device_current_mode(CurrentDeviceMode, alarm_index))
      {
         if (WP_TRUE == alarm_is_port_level(alarm_index))
         {
            printf("%30s | ", line_side_alarm_names[alarm_index]);
            switch (CurrentBuildPersonality)
            {
               case WPX_UFE_FRAMER_BUILD_PERSONALITY_1_PLUS_1_OC3_OC12:
            max_facility_index = 2;
                  break;
               case WPX_UFE_FRAMER_BUILD_PERSONALITY_4_PLUS_4_OC3:
                  max_facility_index = 8;
                  break;
               case WPX_UFE_FRAMER_BUILD_PERSONALITY_2_PLUS_2_OC3:
                  max_facility_index = 4;
                  break;
               default:
                  printf("Invalid build personlity!!!\n");
                  return;
                  break;
            }

            for (facility_index=0;facility_index<max_facility_index;facility_index++)
            {
               if (1 == sonet_sdh_level_alarm_status[facility_index][alarm_index])
                  test_passed = WP_FALSE;
               if (print_data)
                  printf("%d ", ((1 == sonet_sdh_level_alarm_status[facility_index][alarm_index])?1:0));
            }
            printf("|\n");

         }
         else if (alarm_is_ho_level(alarm_index))
         {
            printf("%30s | ", line_side_alarm_names[alarm_index]);
            max_facility_index = 12;

            for (facility_index=0;facility_index<max_facility_index;facility_index++)
            {
               if (1 == sonet_sdh_level_alarm_status[facility_index][alarm_index])
                  test_passed = WP_FALSE;
               if (print_data)
                  printf("%d ", ((1 == sonet_sdh_level_alarm_status[facility_index][alarm_index])?1:0));
            }
            printf("|\n");
         }
         else if (alarm_is_lo_level(alarm_index))
         {
            printf("%s\n", line_side_alarm_names[alarm_index]);
            max_facility_index = 336;

            for (facility_index=0;facility_index<max_facility_index;facility_index++)
            {
               /* In order to comply the E1 skip rule, bytes with 0xFF value will not be involved */
               /* also, LO facilities that were not created or the relevant lo alarm was not enabled will not be involved */
               if (1 == sonet_sdh_level_alarm_status[facility_index][alarm_index])
               {
                  test_passed = WP_FALSE;
                  if (print_data)
                     printf("lo_index %d asserted, ", facility_index);
               }
            }
            printf("\n");

         }
         else
         {
            printf("Invalid alarm catergory!!!\n");
            return;
         }
      }
   }

   /* PDH level print */
   for(alarm_index=WPX_UFE_FRAMER_SOCKET_CLIENT_E_RFI;alarm_index<=WPX_UFE_FRAMER_SOCKET_CLIENT_OOF_LOF;alarm_index++)
   {
      printf("%s\n", socket_client_side_alarm_names[alarm_index]);
      for (pdh_index=0;pdh_index<MAX_PDH_NUMBER;pdh_index++)
      {
         if(1 == pdh_level_alarm_status[pdh_index][alarm_index])
         {
            test_passed = WP_FALSE;
            if (print_data)
               printf("pdh_index %d asserted, ", pdh_index);
         }
      }
      printf("\n");
   }


   if(WP_FALSE == test_passed)
      printf("Test Failed:Alarms detected\n");
   else
      printf("Test Passed:No alarms\n");

   //printf("%d\n", test_passed);
#endif
}


WP_U32 alarm_string_to_index(WP_CHAR *pStr)
{
   WP_U32 iii = 0;

   /* Convert the alarm string into its index */
   for (iii=WPX_UFE_FRAMER_SDH_LOS;iii<WPX_UFE_FRAMER_SONET_SDH_MAX_ALARM_CATEGORIES;iii++)
   {
      if (strstr(line_side_alarm_names[iii], pStr))
      {
         return iii;
      }
   }

   return WPX_UFE_FRAMER_SONET_SDH_MAX_ALARM_CATEGORIES;

}

WP_U32 pdh_alarm_string_to_index(WP_CHAR *pStr)
{
   WP_U32 iii = 0;

   /* Convert the alarm string into its index */
   for (iii=0;iii<WPX_UFE_FRAMER_SOCKET_CLIENT_MAX_ALARM_CATEGORIES;iii++)
   {
      if (strstr(socket_client_side_alarm_names[iii], pStr))
      {
         return iii;
      }
   }

   return WPX_UFE_FRAMER_SOCKET_CLIENT_MAX_ALARM_CATEGORIES;

}

void CLI_F_ValidateSpcificAlarmAssertion(char *StrPrm)
{
   WP_U32 res, cmd, alarm_index, facility_index, first_alarm_index, last_alarm_index, first_facility_index, last_facility_index, assertion;
   WP_CHAR alarms_range[64], facility_range[64];
   WP_CHAR *pStr;

   res = sscanf(StrPrm, "%d %s %s %d", &cmd, &alarms_range, &facility_range, &assertion);
   if (res != 4)
   {
      printf("CLI_F_ValidateSpcificAlarm: Invalid number of parameters, line: %d\n", __LINE__);
      printf("Usage: [alarms_range][facility_range][assertion]\n");
      return;
   }

   printf("%s\n", StrPrm);
   
   if (1 < assertion)
   {
      printf("CLI_F_ValidateSpcificAlarm, line: %d", __LINE__);
      return;
   }

   /* find the start alarm index */
   pStr = strtok(alarms_range, "-");
   first_alarm_index = last_alarm_index = alarm_string_to_index(pStr);
   printf("first_alarm_index %d, last_alarm_index %d, pStr %s\n", first_alarm_index, last_alarm_index, pStr);
   
   if (WPX_UFE_FRAMER_SONET_SDH_MAX_ALARM_CATEGORIES == first_alarm_index)
   {
      printf("Invalid alarm category!!!\n");
      return;
   }

   /* find the last alarm index */
   pStr = strtok(NULL, "-");
   if (pStr)
   {
      last_alarm_index = alarm_string_to_index(pStr);
      if (WPX_UFE_FRAMER_SONET_SDH_MAX_ALARM_CATEGORIES == last_alarm_index)
      {
         printf("Invalid alarm category!!!\n");
         return;
      }
   }

   /* Check first and last alarm indices are supported by the current device mode */
   if ((WP_FALSE == alarm_supported_by_device_current_mode(CurrentDeviceMode, first_alarm_index)) ||
       (WP_FALSE == alarm_supported_by_device_current_mode(CurrentDeviceMode, last_alarm_index)))
   {
      printf("Invalid alarm type - not supported by device current mode!!!\n");
      return;
   }

   /* Check first and last alarm indices belong to the same alarm level */
   if (((WP_TRUE == alarm_is_port_level(first_alarm_index)) && (WP_FALSE == alarm_is_port_level(last_alarm_index))) ||
       ((WP_TRUE == alarm_is_ho_level(first_alarm_index)) && (WP_FALSE == alarm_is_ho_level(last_alarm_index))) ||
       ((WP_TRUE == alarm_is_lo_level(first_alarm_index)) && (WP_FALSE == alarm_is_lo_level(last_alarm_index))))
   {
      printf("Error - alarms type are not belong to the same alarm level!!!\n");
      return;
   }

   /* Find the first facility index */
   pStr = strtok(facility_range, "-");
   first_facility_index = last_facility_index = atoi(pStr);

   /* Find the last facility index */
   pStr = strtok(NULL, "-");
   if (pStr)
   {
      last_facility_index = atoi(pStr);
   }


   test_passed = WP_TRUE;

   printf("first_facility_index %d,last_facility_index %d,first_alarm_index %d,last_alarm_index %d\n", first_facility_index, last_facility_index, first_alarm_index, last_alarm_index);
   for (facility_index=first_facility_index;facility_index<=last_facility_index;facility_index++)
   {
      for(alarm_index=first_alarm_index;alarm_index<=last_alarm_index;alarm_index++)
         {
            /* Only facilities that were created and their alarms were enabled will be involved */
            if (0xFF != sonet_sdh_level_alarm_status[facility_index][alarm_index])
            {
               if(assertion != sonet_sdh_level_alarm_status[facility_index][alarm_index])
               {
                  printf("sonet_sdh_level_alarm_status[%d][%d] == %d\n", facility_index, alarm_index, sonet_sdh_level_alarm_status[facility_index][alarm_index]);
                  test_passed = WP_FALSE;
               }
            }
         }
   }

   if(WP_FALSE == test_passed)
      printf("Test Failed\n");
   else
      printf("Test Passed\n");
}

void CLI_F_PDH_Alarm_Assertion(char *StrPrm)
{
   WP_U32 res, cmd, alarm_index, facility_index, first_alarm_index, last_alarm_index, first_facility_index, last_facility_index, assertion;
   WP_CHAR alarms_range[64], facility_range[64];
   WP_CHAR *pStr;

   res = sscanf(StrPrm, "%d %s %s %d", &cmd, &alarms_range, &facility_range, &assertion);
   if (res != 4)
   {
      printf("CLI_F_ValidateSpcificAlarm: Invalid number of parameters, line: %d\n", __LINE__);
      printf("Usage: [alarms_range][facility_range][assertion]\n");
      return;
   }

   printf("StrPrm %s\n", StrPrm);

   if (1 < assertion)
   {
      printf("CLI_F_ValidateSpcificAlarm, line: %d", __LINE__);
      return;
   }

   /* find the start alarm index */
   pStr = strtok(alarms_range, "-");
   first_alarm_index = last_alarm_index = pdh_alarm_string_to_index(pStr);
   if (WPX_UFE_FRAMER_SONET_SDH_MAX_ALARM_CATEGORIES == first_alarm_index)
   {
      printf("Invalid alarm category!!!\n");
      return;
   }

   /* find the last alarm index */
   pStr = strtok(NULL, "-");
   if (pStr)
   {
      last_alarm_index = alarm_string_to_index(pStr);
      if (WPX_UFE_FRAMER_SONET_SDH_MAX_ALARM_CATEGORIES == last_alarm_index)
      {
         printf("Invalid alarm category!!!\n");
         return;
      }
   }

   /* Find the first facility index */
   pStr = strtok(facility_range, "-");
   first_facility_index = last_facility_index = atoi(pStr);

   /* Find the last facility index */
   pStr = strtok(NULL, "-");
   if (pStr)
   {
      last_facility_index = atoi(pStr);
   }

   test_passed = WP_TRUE;

   printf("first_facility_index %d,last_facility_index %d,first_alarm_index %d,last_alarm_index %d, assertion %d\n", first_facility_index, last_facility_index, first_alarm_index, last_alarm_index, assertion);

   for (facility_index=first_facility_index;facility_index<=last_facility_index;facility_index++)
   {
      for(alarm_index=first_alarm_index;alarm_index<=last_alarm_index;alarm_index++)
         {
            /* Only facilities that were created and their alarms were enabled will be involved */
            if (0xFF != pdh_level_alarm_status[facility_index][alarm_index])
            {
               if(assertion != pdh_level_alarm_status[facility_index][alarm_index])
               {
                  printf("pdh_level_alarm_status[%d][%d] == %d\n", facility_index, alarm_index, pdh_level_alarm_status[facility_index][alarm_index]);
                  test_passed = WP_FALSE;
               }
            }
            else
            {
               printf("is 0xFF\n");
               printf("pdh_level_alarm_status[%d][%d] == %d\n", facility_index, alarm_index, pdh_level_alarm_status[facility_index][alarm_index]);
               //test_passed = WP_FALSE;                
            }
      
         }
   }

   if(WP_FALSE == test_passed)
      printf("Test Failed\n");
   else
      printf("Test Passed\n");
}


void CLI_F_Enable_PM_Print(char *StrPrm)
{

   WP_U32 res, cmd, pm_print_level, pm_print_point, to_enable;

   res = sscanf(StrPrm, "%d %d %d %d", &cmd, &pm_print_level, &pm_print_point, &to_enable);

   printf("pm_print_level %d, pm_print_point %d, to_enable %d\n", pm_print_level, pm_print_point, to_enable);
   if (res != 4)
   {
      printf("Invalid number of parameters line: %d", __LINE__);
      return;
   }

   if (1 < to_enable)
   {
      printf("CLI_F_Enable_PM_Print line: %d", __LINE__);
      return;
   }

   switch(pm_print_level)
   {
      case 0:
         if (WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_MAX_DATA_POINTS_PER_PORT <= pm_print_point)
         {
            printf("CLI_F_Enable_PM_Print line: %d", __LINE__);
            return;
         }
         break;
      case 1:
         if (WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_MAX_DATA_POINTS_PER_HO_PATH <= pm_print_point)
         {
            printf("CLI_F_Enable_PM_Print line: %d", __LINE__);
            return;
         }
         break;
      case 2:
         if (WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_MAX_DATA_POINTS_PER_LO_PATH <= pm_print_point)
         {
            printf("CLI_F_Enable_PM_Print line: %d", __LINE__);
            return;
         }
         break;
      case 3:
         if ((WPX_UFE_FRAMER_PDH_E1_T1_CRC + 1) <= pm_print_point)
         {
            printf("CLI_F_Enable_PM_Print line: %d", __LINE__);
            return;
         }
         break;
      case 4:
         if (1 < enable_peek_print)
         {
            printf("CLI_F_Enable_PM_Print line: %d", __LINE__);
            return;
         }
         break;
      default:
         printf("CLI_F_Enable_PM_Print line: %d", __LINE__);
         return;
         break;

   }

   printf("pm_print_level %d\n", pm_print_level);

   switch(pm_print_level)
   {
      case 0:
         enable_pm_print_port[pm_print_point] = (WP_U8)to_enable;
         break;
      case 1:
         enable_pm_print_ho_path[pm_print_point] = (WP_U8)to_enable;
         break;
      case 2:
         enable_pm_print_lo_path[pm_print_point] = (WP_U8)to_enable;
         break;
      case 3:
         enable_pm_print_pdh[pm_print_point] = (WP_U8)to_enable;
         break;
      case 4:
         if (to_enable)
            enable_peek_print = WP_TRUE;
         else
            enable_peek_print = WP_FALSE;
         break;
   }
}


void CLI_F_PdhAlarmStatusGet(char *strParm)
{
#if 0
   WP_U32  socket_pdh_id, res, cmd, num_clients, is_e1;
   WP_U8  alarm_category, is_error=0;

   res = sscanf(strParm, "%d %d %d", &cmd, &is_e1, &num_clients);
   if (res != 3)
   {
      printf("INVALID NUMBER OF PARAMETERS %d yyy\n", res);

      printf("strParm %s\n", strParm);

      return;
   }

   if (is_e1 > 1)
   {
      printf("INVALID is_e1\n");
      return;
   }

   if (num_clients > 336)
   {
      printf("INVALID num_clients\n");
      return;
   }

   for (alarm_category=0;alarm_category<WPX_UFE_FRAMER_SOCKET_CLIENT_MAX_ALARM_CATEGORIES;alarm_category++)
   {
      for (socket_pdh_id=0;socket_pdh_id<num_clients;socket_pdh_id++)
      {
         if (is_e1 && ((socket_pdh_id % 4) == 3))
            continue;

         if (WT_ASSERTED == alarm_status[alarm_category][socket_pdh_id])
         {
            printf("Test Failed: socket_pdh_id %d alarm_category %s\n",
                   socket_pdh_id,
                   socket_client_side_alarm_names[alarm_category]);
            is_error=1;
         }
      }
   }

   return;

#endif
}

void CLI_F_PMStatusCheck(char *StrPrm)
{
   test_passed = WP_TRUE;
   WP_U32 level_point_ptr, pm_point_ptr;
   WP_U32 t1, t2;

   t1 = WP_TimeRead();
   /* Reset the PM counters of the port level */
   for (level_point_ptr=0;level_point_ptr<1/*MAX_PORTS_NUMBER*/;level_point_ptr++)
      for (pm_point_ptr=0;pm_point_ptr<WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_MAX_DATA_POINTS_PER_PORT;pm_point_ptr++)
      {
         pm_ports_counter[level_point_ptr][pm_point_ptr] = 0;
      }

   t2 = WP_TimeRead();

   printf("t1 = %u, t2 %u, t2-t1=%u\n\n", t1, t2 , (t2-t1));
   /* 3 seconds delay */
   WPL_Delay(3000000);

   /* check the port level pm counters are incrementing */
   for (level_point_ptr=0;level_point_ptr<1/*MAX_PORTS_NUMBER*/;level_point_ptr++)
      for (pm_point_ptr=0;pm_point_ptr<WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_MAX_DATA_POINTS_PER_PORT;pm_point_ptr++)
      {
         if (0 == pm_ports_counter[level_point_ptr][pm_point_ptr])
         {
            test_passed = WP_FALSE;
            printf("pm_ports_counter[%d][%d] == %d\n", level_point_ptr, pm_point_ptr, pm_ports_counter[level_point_ptr][pm_point_ptr]);
         }
      }

   printf("\n");
   t1 = WP_TimeRead();
   /* Reset the PM counters of the HO path level */
   for (level_point_ptr=0;level_point_ptr<1/*MAX_HO_PATH_NUMBER*/;level_point_ptr++)
      for (pm_point_ptr=0;pm_point_ptr<WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_MAX_DATA_POINTS_PER_HO_PATH;pm_point_ptr++)
      {
         pm_ho_path_counter[level_point_ptr][pm_point_ptr] = 0;
      }

   t2 = WP_TimeRead();

   printf("t1 = %u, t2 %u, t2-t1=%u\n\n", t1, t2 , (t2-t1));
   /* 3 seconds delay */
   WPL_Delay(3000000);

   /*Check the HO path PM counter are incrementing*/
   for (level_point_ptr=0;level_point_ptr<1/*MAX_HO_PATH_NUMBER*/;level_point_ptr++)
      for (pm_point_ptr=0;pm_point_ptr<WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_MAX_DATA_POINTS_PER_HO_PATH;pm_point_ptr++)
      {
         if (0 == pm_ho_path_counter[level_point_ptr][pm_point_ptr])
         {
            test_passed = WP_FALSE;
            printf("pm_ho_path_counter[%d][%d] == %d\n", level_point_ptr, pm_point_ptr, pm_ho_path_counter[level_point_ptr][pm_point_ptr]);
         }
      }
   printf("\n");
   t1 = WP_TimeRead();

   /* Reset the PM counters of the LO path level */
   for (level_point_ptr=0;level_point_ptr</*TMP_NUM*/ MAX_LO_PATH_NUMBER;level_point_ptr++)
      for (pm_point_ptr=0;pm_point_ptr<WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_MAX_DATA_POINTS_PER_LO_PATH;pm_point_ptr++)
      {
         pm_lo_path_counter[level_point_ptr][pm_point_ptr] = 0;
      }

   t2 = WP_TimeRead();

   printf("t1 = %u, t2 %u, t2-t1=%u\n\n", t1, t2 , (t2-t1));
   /* 3 seconds delay */
   WPL_Delay(3000000);

   /* Check the LO path PM counters are incrementing */
   for (level_point_ptr=0;level_point_ptr</*TMP_NUM*/ MAX_LO_PATH_NUMBER;level_point_ptr++)
      for (pm_point_ptr=0;pm_point_ptr<WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_MAX_DATA_POINTS_PER_LO_PATH;pm_point_ptr++)
      {
         if (0 == pm_lo_path_counter[level_point_ptr][pm_point_ptr])
         {
            test_passed = WP_FALSE;
         printf("pm_lo_path_counter[%d][%d] == %d\n", level_point_ptr, pm_point_ptr, pm_lo_path_counter[level_point_ptr][pm_point_ptr]);
         }
         
      }


   printf("\n");
   t1 = WP_TimeRead();

   /* Reset the PM counters of the PDH level */
   for (level_point_ptr=0;level_point_ptr</*TMP_NUM*/ MAX_PDH_NUMBER;level_point_ptr++)
      for (pm_point_ptr=0;pm_point_ptr< (WPX_UFE_FRAMER_PDH_E1_T1_CRC+1) ;pm_point_ptr++)
      {
         pm_pdhs_counter[level_point_ptr][pm_point_ptr] = 0;
      }
   t2 = WP_TimeRead();
   printf("t1 = %u, t2 %u, t2-t1=%u\n\n", t1, t2 , (t2-t1));
   /* 3 seconds delay */
   WPL_Delay(3000000);

   /* Check the PDH  PM counter is incrementing */
   for (level_point_ptr=0;level_point_ptr</*TMP_NUM*/ MAX_PDH_NUMBER;level_point_ptr++)
      for (pm_point_ptr=0;pm_point_ptr< (WPX_UFE_FRAMER_PDH_E1_T1_CRC+1) ;pm_point_ptr++)
      {
         if (0 == pm_pdhs_counter[level_point_ptr][pm_point_ptr])
         {
            test_passed = WP_FALSE;
            printf("pm_pdhs_counter[%d][%d] == %d\n", level_point_ptr, pm_point_ptr, pm_pdhs_counter[level_point_ptr][pm_point_ptr]);
         }
      }

   printf("\n");

   if(WP_FALSE == test_passed)
      printf("Test Failed:PM failures\n");
   else
      printf("Test Passed:PM counters increment\n");
}



#endif

#endif
