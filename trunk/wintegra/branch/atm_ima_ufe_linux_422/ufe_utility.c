/*****************************************************************************
 * (C) Copyright PMC - WIN division (Wintegra) 2012.  All rights reserved.
 * PMC-Sierra (WINTEGRA) CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of PMC-Sierra (Wintegra).
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *  Application : ATM IMA UFE
 *  File Name   : ufe_utility.c
 *
 *****************************************************************************/

/****************************************************************************
 *
 * File: ufe_utility.c
 * Demonstrates handling OMIINO alarm and performance monitoring callbacks
 *
 ****************************************************************************/

#include <string.h>
#include "wp_wddi.h"

#include <wpl_platform.h>

#undef USE_ANT

WP_U32 private_thread_lock = 0;

void WT_ReinstateInterruptMask (WP_U32 wpid);
extern char *OMIINO_FRAMER_ErrorCodeToTxt (WP_U32 ErrorCode);

/* Alarm and PM handling structures */
typedef struct
{
   WP_U8 device;
   WP_U8 line_port_id;
   WP_U8 alarm_category;
   WP_U8 is_asserted;
} APP_PORT_ALARM_TASK;

typedef struct
{
   WP_U8 device;
   WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE endpoint;
   WP_U8 alarm_category;
   WP_U8 is_asserted;
} APP_PATH_ALARM_TASK;

typedef struct
{
   WP_U8 device;
   WP_U8 table_index;
   WP_U16 pad;
   WP_U32 timestamp;
} APP_PM_TASK;

typedef struct
{
   WP_U8 device;
   WP_U8 line_port_id;
   WP_U8 k1_value;
   WP_U8 k2_value;
} APP_K1_K2_SIGNAL_TASK;

typedef struct
{
   WP_U8 device;
   WP_U8 line_port_id;
   WP_U8 s1_value;
} APP_S1_SIGNAL_TASK;

#define PORT_ALARM_TYPE   1
#define PATH_ALARM_TYPE   2
#define K1K2_SIGNAL_TYPE  3
#define S1_SIGNAL_TYPE    4
#define PM_PORT_TYPE      5
#define PM_HO_PATH_TYPE   6
#define PM_LO_PATH_TYPE   7
#define PM_PDH_TYPE       8

typedef struct
{
   WP_U32 task_type;
   union
   {
      APP_PORT_ALARM_TASK port_alarm_task;
      APP_PATH_ALARM_TASK path_alarm_task;
      APP_PM_TASK pm_task;
      APP_K1_K2_SIGNAL_TASK k1k2_signal_task;
      APP_S1_SIGNAL_TASK s1_signal_task;
   } u;
} framer_task;

typedef struct
{
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
framer_task_list the_alarm_task_list[1] =
   { {0, 0, ALARM_TASK_LIST_SIZE, 0, the_alarm_tasks} };

#define PM_TASK_LIST_SIZE 4096
framer_task the_pm_tasks[PM_TASK_LIST_SIZE];
framer_task_list the_pm_task_list[1] =
   { {0, 0, PM_TASK_LIST_SIZE, 0, the_pm_tasks} };

#define SIGNAL_TASK_LIST_SIZE 4096
framer_task the_signal_tasks[PM_TASK_LIST_SIZE];
framer_task_list the_signal_task_list[1] =
   { {0, 0, SIGNAL_TASK_LIST_SIZE, 0, the_signal_tasks} };

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

framer_task *next_framer_task (framer_task_list * task_list,
                               framer_task * result)
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

void WTI_allocate_pm_storage_areas (void)
{
   /* Allocate storage for the performance monitoring data */

#define SONET_SDH_PORT_STORAGE_NUM_ELEMENTS      120
#define SONET_SDH_HO_PATH_STORAGE_NUM_ELEMENTS   120
#define SONET_SDH_LO_PATH_STORAGE_NUM_ELEMENTS   120
#define PDH_STORAGE_NUM_ELEMENTS                 120

   void *space;
   int element_size;
   WUFE_status status;

   element_size =
      sizeof
      (WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_PORT_ENDPOINT_TYPE);
   space = malloc (SONET_SDH_PORT_STORAGE_NUM_ELEMENTS * element_size);
   if (space == NULL)
      WTI_FlexmuxCheckStatus ("PM storage allocation",
                              WPX_UFE_FRAMER_ERROR_PARAMETER_OUT_OF_RANGE,
                              __LINE__);
   status =
      WPX_FRMR_PM_SonetSdhPortStorage (space,
                                       SONET_SDH_PORT_STORAGE_NUM_ELEMENTS);
   if (status != WUFE_OK)
      WTI_FlexmuxCheckStatus ("PM storage allocation",
                              WPX_UFE_FRAMER_ERROR_PARAMETER_OUT_OF_RANGE,
                              __LINE__);

   element_size =
      sizeof
      (WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_HO_PATH_ENDPOINT_TYPE);
   space = malloc (SONET_SDH_HO_PATH_STORAGE_NUM_ELEMENTS * element_size);
   if (space == NULL)
      WTI_FlexmuxCheckStatus ("PM storage allocation",
                              WPX_UFE_FRAMER_ERROR_PARAMETER_OUT_OF_RANGE,
                              __LINE__);
   status =
      WPX_FRMR_PM_SonetSdhHOPathStorage (space,
                                         SONET_SDH_HO_PATH_STORAGE_NUM_ELEMENTS);
   if (status != WUFE_OK)
      WTI_FlexmuxCheckStatus ("PM storage allocation",
                              WPX_UFE_FRAMER_ERROR_PARAMETER_OUT_OF_RANGE,
                              __LINE__);

   element_size =
      sizeof
      (WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_LO_PATH_ENDPOINT_TYPE);
   space = malloc (SONET_SDH_LO_PATH_STORAGE_NUM_ELEMENTS * element_size);
   if (space == NULL)
      WTI_FlexmuxCheckStatus ("PM storage allocation",
                              WPX_UFE_FRAMER_ERROR_PARAMETER_OUT_OF_RANGE,
                              __LINE__);
   status =
      WPX_FRMR_PM_SonetSdhLOPathStorage (space,
                                         SONET_SDH_LO_PATH_STORAGE_NUM_ELEMENTS);
   if (status != WUFE_OK)
      WTI_FlexmuxCheckStatus ("PM storage allocation",
                              WPX_UFE_FRAMER_ERROR_PARAMETER_OUT_OF_RANGE,
                              __LINE__);

   element_size =
      sizeof (WPX_UFE_FRAMER_PDH_PERFORMANCE_MONITORING_ENDPOINT_TYPE);
   space = malloc (PDH_STORAGE_NUM_ELEMENTS * element_size);
   if (space == NULL)
      WTI_FlexmuxCheckStatus ("PM storage allocation",
                              WPX_UFE_FRAMER_ERROR_PARAMETER_OUT_OF_RANGE,
                              __LINE__);
   status = WPX_FRMR_PM_PdhStorage (space, PDH_STORAGE_NUM_ELEMENTS);
   if (status != WUFE_OK)
      WTI_FlexmuxCheckStatus ("PM storage allocation",
                              WPX_UFE_FRAMER_ERROR_PARAMETER_OUT_OF_RANGE,
                              __LINE__);

}

#define WTI_LINE_SIDE_ALARM_DISPLAY 1

static char *line_side_alarm_names[] = {
   "WPX_UFE_FRAMER_SDH_LOS",    /*   0 */
   "WPX_UFE_FRAMER_SDH_LOF",    /*   1 */
   "WPX_UFE_FRAMER_SDH_RS_TIM", /*   2 */
   "WPX_UFE_FRAMER_SDH_MS_AIS", /*   3 */
   "WPX_UFE_FRAMER_SDH_MS_RDI", /*   4 */
   "WPX_UFE_FRAMER_SDH_OOF",    /*   5 */
   "WPX_UFE_FRAMER_SDH_MS_EXC", /*   6 */
   "WPX_UFE_FRAMER_SDH_MS_DEG", /*   7 */
   "WPX_UFE_FRAMER_SDH_AU_AIS", /*   8 */
   "WPX_UFE_FRAMER_SDH_AU_LOP", /*   9 */
   "WPX_UFE_FRAMER_SDH_HP_LOM", /*  10 */
   "WPX_UFE_FRAMER_SDH_HP_TIM", /*  11 */
   "WPX_UFE_FRAMER_SDH_HP_PLM", /*  12 */
   "WPX_UFE_FRAMER_SDH_HP_UNEQ", /*  13 */
   "WPX_UFE_FRAMER_SDH_HP_RDI", /*  14 */
   "WPX_UFE_FRAMER_SDH_HP_EXC", /*  15 */
   "WPX_UFE_FRAMER_SDH_HP_DEG", /*  16 */
   "WPX_UFE_FRAMER_SDH_LP_TIM", /*  17 */
   "WPX_UFE_FRAMER_SDH_LP_PLM", /*  18 */
   "WPX_UFE_FRAMER_SDH_LP_UNEQ", /*  19 */
   "WPX_UFE_FRAMER_SDH_LP_RDI", /*  20 */
   "WPX_UFE_FRAMER_SDH_LP_EXC", /*  21 */
   "WPX_UFE_FRAMER_SDH_LP_DEG", /*  22 */
   "WPX_UFE_FRAMER_SDH_TU_AIS", /*  23 */
   "WPX_UFE_FRAMER_SDH_TU_LOP", /*  24 */
   "WPX_UFE_FRAMER_SONET_LOS",  /*  25 */
   "WPX_UFE_FRAMER_SONET_LOF",  /*  26 */
   "WPX_UFE_FRAMER_SONET_TIM_S", /*  27 */
   "WPX_UFE_FRAMER_SONET_AIS_L", /*  28 */
   "WPX_UFE_FRAMER_SONET_RDI_L", /*  29 */
   "WPX_UFE_FRAMER_SONET_OOF",  /*  30 */
   "WPX_UFE_FRAMER_SONET_EXC_L", /*  31 */
   "WPX_UFE_FRAMER_SONET_DEG_L", /*  32 */
   "WPX_UFE_FRAMER_SONET_LOM_P", /*  33 */
   "WPX_UFE_FRAMER_SONET_TIM_P", /*  34 */
   "WPX_UFE_FRAMER_SONET_PLM_P", /*  35 */
   "WPX_UFE_FRAMER_SONET_UNEQ_P",   /*  36 */
   "WPX_UFE_FRAMER_SONET_RDI_P", /*  37 */
   "WPX_UFE_FRAMER_SONET_EXC_P", /*  38 */
   "WPX_UFE_FRAMER_SONET_DEG_P", /*  39 */
   "WPX_UFE_FRAMER_SONET_TIM_V", /*  40 */
   "WPX_UFE_FRAMER_SONET_PLM_V", /*  41 */
   "WPX_UFE_FRAMER_SONET_UNEQ_V",   /*  42 */
   "WPX_UFE_FRAMER_SONET_RDI_V", /*  43 */
   "WPX_UFE_FRAMER_SONET_EXC_V", /*  44 */
   "WPX_UFE_FRAMER_SONET_DEG_V", /*  45 */
   "WPX_UFE_FRAMER_SONET_AIS_V", /*  46 */
   "WPX_UFE_FRAMER_SONET_LOP_V", /*  47 */
   "WPX_UFE_FRAMER_SONET_AIS_P", /*  48 */
   "WPX_UFE_FRAMER_SONET_LOP_P", /*  49 */
   "WPX_UFE_FRAMER_SDH_RS_EXC", /*  50 */
   "WPX_UFE_FRAMER_SDH_RS_DEG", /*  51 */
   "WPX_UFE_FRAMER_SONET_EXC_S", /*  52 */
   "WPX_UFE_FRAMER_SONET_DEG_S" /*  53 */
};

static char *socket_client_side_alarm_names[] = {
   "WPX_UFE_FRAMER_SOCKET_CLIENT_E_RFI",
   "WPX_UFE_FRAMER_SOCKET_CLIENT_A_RAI",
   "WPX_UFE_FRAMER_SOCKET_CLIENT_LOS_AIS"
};

void service_path_alarm (WP_U8 device_id,
                         WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *
                         p_SDH_Sonet_line_endpoint_type,
                         WP_U8 alarm_category, WP_U8 is_asserted)
{

   printf ("PATH ALARM:  alarm_category = %s %s\n",
           line_side_alarm_names[alarm_category],
           is_asserted ? "Asserted" : "Unasserted");
}

void service_port_alarm (WP_U8 device, WP_U8 line_port_id,
                         WP_U8 alarm_category, WP_U8 is_asserted)
{

   printf ("PORT ALARM:  line_port_id = %d, alarm_category = %s %s\n",
           line_port_id, line_side_alarm_names[alarm_category],
           is_asserted ? "Asserted" : "Unasserted");
}

void service_k1_k2_signal (WP_U8 device, WP_U8 line_port_id, WP_U8 k1,
                           WP_U8 k2)
{

   printf ("K1K2 SIGNAL:  iLinePort = %d K1 %d, K2 %d\n", line_port_id, k1,
           k2);

}

void service_s1_signal (WP_U8 device, WP_U8 line_port_id, WP_U8 s1)
{

   printf ("S1 SIGNAL:  iLinePort = %d S1 %d\n", line_port_id, s1);

}

WP_U32 B1_count, B2_count, M1_count;

void service_pm_port_data (WP_U8 device_id, WP_U8 table_index,
                           WP_U32 timestamp)
{

   WP_U32 count;
   WP_U8 result;

   printf
      ("PM PORT:  device_id = %d, table_index = %d, timestamp = 0x%x\n",
       device_id, table_index, timestamp);

   if ((result =
        WPX_FRMR_STATUS_PM_SONET_SDH_Port_ReadData (device_id, table_index,
                                                    0,
                                                    WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_B1,
                                                    &count)) == 0)
   {
      printf ("port %d: B1 count %d\n", device_id, count);
      B1_count += count;
   }
   else
      printf ("Port_ReadData failed on B1: 0x%x\n", result);

   if ((result =
        WPX_FRMR_STATUS_PM_SONET_SDH_Port_ReadData (device_id, table_index,
                                                    0,
                                                    WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_B2,
                                                    &count)) == 0)
   {
      printf ("port %d: B2 count %d\n", device_id, count);
      B2_count += count;
   }
   else
      printf ("Port_ReadData failed on B2: 0x%x\n", result);

   if ((result =
        WPX_FRMR_STATUS_PM_SONET_SDH_Port_ReadData (device_id, table_index,
                                                    0,
                                                    WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_M1,
                                                    &count)) == 0)
   {
      printf ("port %d: M1 count %d\n", device_id, count);
      M1_count += count;
   }
   else
      printf ("Port_ReadData failed on M1: 0x%x\n", result);

}

void service_pm_ho_path_data (WP_U8 device_id, WP_U8 table_index,
                              WP_U32 timestamp)
{

   WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE LineEndpointType;
   U32 count = 0;
   U32 max_stm1, max_stm0, max_tug2, max_tu;
   U32 stm1, stm0, tug2, tu;
   U8 result;

   memset (&LineEndpointType, 0,
           sizeof (WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE));

   printf ("HO PATH:  table_index = %d timestamp = 0x%x\n", table_index,
           timestamp);

   LineEndpointType.TransferType = ho_transfer_type_in_use;
   LineEndpointType.u.SDH.stm4 = 0;

   if (LineEndpointType.TransferType == WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC3)
      max_stm0 = 3;
   else
      max_stm0 = 1;

   max_stm1 = 4;
   max_tug2 = max_tu = 1;

   for (stm1 = 0; stm1 < max_stm1; ++stm1)
      for (stm0 = 0; stm0 < max_stm0; ++stm0)
         for (tug2 = 0; tug2 < max_tug2; ++tug2)
            for (tu = 0; tu < max_tu; ++tu)
            {
               LineEndpointType.u.SDH.stm1 = stm1;
               LineEndpointType.u.SDH.stm0 = stm0;
               LineEndpointType.u.SDH.tug2 = tug2;
               LineEndpointType.u.SDH.tu = tu;

               result =
                  WPX_FRMR_STATUS_PM_SONET_SDH_HO_PATH_ReadData (device_id,
                                                                 table_index,
                                                                 &LineEndpointType,
                                                                 WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_B3,
                                                                 &count);

               if (result)
                  printf
                     ("    ---->  HO_Path_ReadData failed on %d:%d:%d:%d with %d\n",
                      stm1, stm0, tug2, tu, result);
               else
                  printf ("    ---->  B3 count on %d:%d:%d:%d is %d\n",
                          stm1, stm0, tug2, tu, count);

               result =
                  WPX_FRMR_STATUS_PM_SONET_SDH_HO_PATH_ReadData (device_id,
                                                                 table_index,
                                                                 &LineEndpointType,
                                                                 WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_G1,
                                                                 &count);

               if (result)
                  printf
                     ("    ---->  HO_Path_ReadData failed on %d:%d:%d:%d with %d\n",
                      stm1, stm0, tug2, tu, result);
               else
                  printf ("    ---->  G1 count on %d:%d:%d:%d is %d\n",
                          stm1, stm0, tug2, tu, count);
            }

}

void service_pm_lo_path_data (WP_U8 device_id, WP_U8 table_index,
                              WP_U32 timestamp)
{

   WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE LineEndpointType;
   U32 count = 0;
   U32 max_stm1, max_stm0, max_tug2, max_tu;
   U32 stm1, stm0, tug2, tu;
   U8 result;

   memset (&LineEndpointType, 0,
           sizeof (WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE));

   printf ("LO PATH:  table_index = %d timestamp = 0x%x\n", table_index,
           timestamp);

   LineEndpointType.TransferType = lo_transfer_type_in_use;
   LineEndpointType.u.SDH.stm4 = 0;

   max_stm1 = 1;
   max_stm0 = 1;
   max_tug2 = 1;
   max_tu = 3;

   for (stm1 = 0; stm1 < max_stm1; ++stm1)
      for (stm0 = 0; stm0 < max_stm0; ++stm0)
         for (tug2 = 0; tug2 < max_tug2; ++tug2)
            for (tu = 0; tu < max_tu; ++tu)
            {
               LineEndpointType.u.SDH.stm1 = stm1;
               LineEndpointType.u.SDH.stm0 = stm0;
               LineEndpointType.u.SDH.tug2 = tug2;
               LineEndpointType.u.SDH.tu = tu;

               result =
                  WPX_FRMR_STATUS_PM_SONET_SDH_LO_PATH_ReadData (device_id,
                                                                 table_index,
                                                                 &LineEndpointType,
                                                                 WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_V5_BIP,
                                                                 &count);

               if (result)
                  printf
                     ("    ---->  LO_Path_ReadData failed on %d:%d:%d:%d with %d\n",
                      stm1, stm0, tug2, tu, result);
               else
                  printf ("    ---->  V5_BIP count on %d:%d:%d:%d is %d\n",
                          stm1, stm0, tug2, tu, count);

               result =
                  WPX_FRMR_STATUS_PM_SONET_SDH_LO_PATH_ReadData (device_id,
                                                                 table_index,
                                                                 &LineEndpointType,
                                                                 WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_V5_REI,
                                                                 &count);

               if (result)
                  printf
                     ("    ---->  LO_Path_ReadData failed on %d:%d:%d:%d with %d\n",
                      stm1, stm0, tug2, tu, result);
               else
                  printf ("    ---->  V5_REI count on %d:%d:%d:%d is %d\n",
                          stm1, stm0, tug2, tu, count);
            }
}

void service_pm_pdh_data (WP_U8 device_id, WP_U8 table_index,
                          WP_U32 timestamp)
{
   printf ("PDH:  table_index = %d timestamp = 0x%x\n", table_index,
           timestamp);
}

void process_framer_task_list (void)
{
   framer_task a_task;
   APP_PORT_ALARM_TASK *port_alarm;
   APP_PATH_ALARM_TASK *path_alarm;
   APP_PM_TASK *pm_task;
   APP_K1_K2_SIGNAL_TASK *k1k2_signal;
   APP_S1_SIGNAL_TASK *s1_signal;

   while (1)
   {
      WPL_Lock (WPL_THREAD_LOCK_KEY, &private_thread_lock);

      while (WP_TRUE)
      {
         if (next_framer_task (the_alarm_task_list, &a_task) != NULL)
         {
            switch (a_task.task_type)
            {
            case PORT_ALARM_TYPE:

               port_alarm = &a_task.u.port_alarm_task;

               service_port_alarm (port_alarm->device,
                                   port_alarm->line_port_id,
                                   port_alarm->alarm_category,
                                   port_alarm->is_asserted);

               break;

            case PATH_ALARM_TYPE:

               path_alarm = &a_task.u.path_alarm_task;

               service_path_alarm (path_alarm->device,
                                   &path_alarm->endpoint,
                                   path_alarm->alarm_category,
                                   path_alarm->is_asserted);

               break;

            default:

               break;
            }
         }

         else if (next_framer_task (the_signal_task_list, &a_task) != NULL)
         {
            switch (a_task.task_type)
            {
            case K1K2_SIGNAL_TYPE:

               k1k2_signal = &a_task.u.k1k2_signal_task;

               service_k1_k2_signal (k1k2_signal->device,
                                     k1k2_signal->line_port_id,
                                     k1k2_signal->k1_value,
                                     k1k2_signal->k2_value);

               break;

            case S1_SIGNAL_TYPE:

               s1_signal = &a_task.u.s1_signal_task;

               service_s1_signal (s1_signal->device,
                                  s1_signal->line_port_id,
                                  s1_signal->s1_value);

               break;

            default:

               break;
            }
         }

         else if (next_framer_task (the_pm_task_list, &a_task) != NULL)
         {
            switch (a_task.task_type)
            {
            case PM_PORT_TYPE:

               pm_task = &a_task.u.pm_task;

               service_pm_port_data (pm_task->device, pm_task->table_index,
                                     pm_task->timestamp);

               break;

            case PM_HO_PATH_TYPE:

               pm_task = &a_task.u.pm_task;

               service_pm_ho_path_data (pm_task->device,
                                        pm_task->table_index,
                                        pm_task->timestamp);

               break;

            case PM_LO_PATH_TYPE:

               pm_task = &a_task.u.pm_task;

               service_pm_lo_path_data (pm_task->device,
                                        pm_task->table_index,
                                        pm_task->timestamp);

               break;

            case PM_PDH_TYPE:

               pm_task = &a_task.u.pm_task;

               service_pm_pdh_data (pm_task->device, pm_task->table_index,
                                    pm_task->timestamp);

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

void WTI_enable_alarms (int type)
{
   WP_boolean is_sdh = (type == 0);

   int j;
   WP_U8 status;
   int start_alarm, last_alarm;

//   WPX_FRMR_RegisterSonetSdhPathAlarmCallback(&cb_path_alarm);    // -- morris
//   WPX_FRMR_RegisterSonetSdhPortAlarmCallback(&cb_port_alarm);
//   WPX_FRMR_RegisterSocketClientPdhAlarmCallback(&cb_pdh_alarm);
//    WPX_FRMR_RegisterSonetSdhSectionK1K2AnnounceCallback(&cb_k1k2_signal);

   if (is_sdh)
   {
      start_alarm = WPX_UFE_FRAMER_SDH_LOS;
      last_alarm = WPX_UFE_FRAMER_SDH_HP_DEG;
   }
   else
   {
      start_alarm = WPX_UFE_FRAMER_SONET_LOS;
      last_alarm = WPX_UFE_FRAMER_SONET_LOP_P;
   }

   for (j = start_alarm; j <= last_alarm; ++j)
   {
      status = WPX_FRMR_DEV_DRV_SONET_SDH_EnableAlarm (j, j);
      if (status != WPX_UFE_FRAMER_OK)
      {
         printf
            ("********************************************************************\n"
             "   %d     SDH/SONET EVENT %d FAILED with %s\n"
             "********************************************************************\n",
             __LINE__, j, OMIINO_FRAMER_ErrorCodeToTxt (status));
         WTI_FlexmuxCheckStatus
            ("WTI_enable_alarms_and_performance_monitoring", status,
             __LINE__);
      }
   }

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
      status = WPX_FRMR_DEV_DRV_SONET_SDH_EnableAlarm (j, j);
      if (status != WPX_UFE_FRAMER_OK)
      {
         printf
            ("********************************************************************\n"
             "   %d     SDH/SONET EVENT %d FAILED with %s\n"
             "********************************************************************\n",
             __LINE__, j, OMIINO_FRAMER_ErrorCodeToTxt (status));
         WTI_FlexmuxCheckStatus
            ("WTI_enable_alarms_and_performance_monitoring", status,
             __LINE__);
      }
   }

   for (j = WPX_UFE_FRAMER_SOCKET_CLIENT_E_RFI;
        j < WPX_UFE_FRAMER_SOCKET_CLIENT_MAX_ALARM_CATEGORIES; ++j)
   {
      status = WPX_FRMR_DEV_DRV_SOCKET_EnableAlarm (j, j);
      if (status != WPX_UFE_FRAMER_OK)
      {
         printf
            ("********************************************************************\n"
             "          SOCKET EVENT %d FAILED\n"
             "********************************************************************\n",
             j);
         WTI_FlexmuxCheckStatus
            ("WTI_enable_alarms_and_performance_monitoring", status,
             __LINE__);
      }
      else
         printf ("%s enabled\n", socket_client_side_alarm_names[j]);
   }

   printf
      ("***************** ALARMS ENABLED ************************************\n");

}

void WTI_enable_performance_monitoring (int type)
{

   // WPX_FRMR_PM_RegisterSonetSdhPortAnnounceCallback(&cb_pm_port);    // -- morris
//    WPX_FRMR_PM_RegisterSonetSdhHOPathAnnounceCallback(&cb_pm_ho_path);
//    WPX_FRMR_PM_RegisterSonetSdhLOPathAnnounceCallback(&cb_pm_lo_path);
//    WPX_FRMR_PM_RegisterSocketClientPdhAnnounceCallback(&cb_pm_pdh);

}

void WTI_enable_performance_monitoring_points (int type, int is_e1,
                                               int vc_type,
                                               WUFE_un_framed framing_mode)
{

   WP_boolean is_sdh = (type == 0);
   WP_U32 transaction_number;
   U8 result;

   WP_U32 number_lo_points = 0;

   /* for SDH: stm1, stm0, tug2, tu */
   WP_U32 stm1, stm0, tug2, tu;
   WP_U32 max_stm1, max_stm0, max_tug2, max_tu;

   /* for SONET:  sts3, sts1, vt_group, vt */
   WP_U32 sts3, sts1, vt_group, vt;

   WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE LineEndpointType;

   /* Enable Port PM */
   static WP_U32 line_port = 0;

   WPX_FRMR_PM_SONET_SDH_Port_EnableMonitoring (101, 0, line_port,
                                                WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_B1);
   WPX_FRMR_PM_SONET_SDH_Port_EnableMonitoring (102, 0, line_port,
                                                WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_B2);
   WPX_FRMR_PM_SONET_SDH_Port_EnableMonitoring (103, 0, line_port,
                                                WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_M1);

   if (is_e1)
   {
      /* For HO paths */
      LineEndpointType.u.SDH.stm4 = 0;
      LineEndpointType.u.SDH.tug2 = 0;
      LineEndpointType.u.SDH.tu = 0;

      transaction_number = 200;

      if (is_sdh)
      {
         if (vc_type == 0)
            LineEndpointType.TransferType =
               WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC3;
         else
            LineEndpointType.TransferType =
               WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC4;

         ho_transfer_type_in_use = LineEndpointType.TransferType;

         if (LineEndpointType.TransferType ==
             WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC3)
            max_stm0 = 3;
         else
            max_stm0 = 1;

         max_stm1 = 4;

         for (stm1 = 0; stm1 < max_stm1; ++stm1)
            for (stm0 = 0; stm0 < max_stm0; ++stm0)
            {
               LineEndpointType.u.SDH.stm1 = stm1;
               LineEndpointType.u.SDH.stm0 = stm0;

               result =
                  WPX_FRMR_PM_SONET_SDH_HO_Path_EnableMonitoring
                  (transaction_number++, 0, &LineEndpointType,
                   WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_B3);
               if (result)
                  printf ("B3 failure on %d:%d:%d:%d --> %d\n", stm1, stm0,
                          0, 0, result);
               else
                  printf ("B3 enabled on %d:%d:%d:%d --> %d\n", stm1, stm0,
                          0, 0, result);

               result =
                  WPX_FRMR_PM_SONET_SDH_HO_Path_EnableMonitoring
                  (transaction_number++, 0, &LineEndpointType,
                   WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_G1);
               if (result)
                  printf ("G1 failure on %d:%d:%d:%d --> %d\n", stm1, stm0,
                          0, 0, result);
               else
                  printf ("G1 enabled on %d:%d:%d:%d --> %d\n", stm1, stm0,
                          0, 0, result);
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

                     result =
                        WPX_FRMR_PM_SONET_SDH_LO_Path_EnableMonitoring
                        (transaction_number++, 0, &LineEndpointType,
                         WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_V5_BIP);
                     if (result)
                        printf ("V5_BIP failure on %d:%d:%d:%d --> %d\n",
                                stm1, stm0, tug2, tu, result);
                     else
                        ++number_lo_points;

                     result =
                        WPX_FRMR_PM_SONET_SDH_LO_Path_EnableMonitoring
                        (transaction_number++, 0, &LineEndpointType,
                         WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_V5_REI);
                     if (result)
                        printf ("V5_REI failure on %d:%d:%d:%d --> %d\n",
                                stm1, stm0, tug2, tu, result);
                     else
                        ++number_lo_points;
                  }
         printf ("****************** number_lo_points %d\n",
                 number_lo_points);
      }
      else
      {
         LineEndpointType.TransferType =
            WPX_UFE_FRAMER_WUFE_SONET_TYPE_VT2;

         for (sts3 = 0; sts3 < 4; ++sts3)
            for (sts1 = 0; sts1 < 3; ++sts1)
               for (vt_group = 0; vt_group < 1; ++vt_group)
                  for (vt = 0; vt < 1; ++vt)
                  {
                     LineEndpointType.u.SONET.sts3 = sts3;
                     LineEndpointType.u.SONET.sts1 = sts1;
                     LineEndpointType.u.SONET.vt_group = vt_group;
                     LineEndpointType.u.SONET.vt = vt;

                     result =
                        WPX_FRMR_PM_SONET_SDH_HO_Path_EnableMonitoring
                        (transaction_number++, 0, &LineEndpointType,
                         WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_B3);
                     if (result)
                        printf ("B3 failure on %d:%d:%d:%d --> %d\n", sts3,
                                sts1, vt_group, vt, result);
                     else
                        printf ("B3 enabled on %d:%d:%d:%d --> %d\n", sts3,
                                sts1, vt_group, vt, result);

                     result =
                        WPX_FRMR_PM_SONET_SDH_HO_Path_EnableMonitoring
                        (transaction_number++, 0, &LineEndpointType,
                         WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_G1);
                     if (result)
                        printf ("G1 failure on %d:%d:%d:%d --> %d\n", sts3,
                                sts1, vt_group, vt, result);
                  }
      }

      /* Limited number of points can be concurrently monitored.  CRC monitoring performed only if framed */

      if (framing_mode == WUFE_FRAMED)
      {
         int j, k;
         WP_U8 result;

         for (j = k = 0; k < 80; ++j)
         {
            if ((j % 4) < 3)
            {
               if ((result =
                    WPX_FRMR_PM_E1_EnableMonitoringCrc
                    (transaction_number++, 0, j)) != WPX_UFE_FRAMER_OK)
               {
                  printf ("E1 CRC Monitoring failed on %d with %s\n", j,
                          OMIINO_FRAMER_ErrorCodeToTxt (result));
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
      LineEndpointType.u.SDH.stm4 = 0;
      transaction_number = 200;

      if (is_sdh)
      {
         if (vc_type == 0)
            LineEndpointType.TransferType =
               WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC3;
         else
            LineEndpointType.TransferType =
               WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC4;

         if (LineEndpointType.TransferType ==
             WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC3)
            max_stm0 = 3;
         else
            max_stm0 = 1;

         max_tug2 = max_tu = 1;
         max_tug2 = 7;
         max_tu = 4;
         max_stm0 = 3;

         // TEMPORARY
         max_stm1 = 4;

         for (stm1 = 0; stm1 < max_stm1; ++stm1)
            for (stm0 = 0; stm0 < max_stm0; ++stm0)
               for (tug2 = 0; tug2 < max_tug2; ++tug2)
                  for (tu = 0; tu < max_tu; ++tu)
                  {
                     LineEndpointType.u.SDH.stm1 = stm1;
                     LineEndpointType.u.SDH.stm0 = stm0;
                     LineEndpointType.u.SDH.tug2 = tug2;
                     LineEndpointType.u.SDH.tu = tu;

                     result =
                        WPX_FRMR_PM_SONET_SDH_HO_Path_EnableMonitoring
                        (transaction_number++, 0, &LineEndpointType,
                         WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_B3);
                     if (result)
                        printf ("B3 failure on %d:%d:%d:%d --> %d\n", stm1,
                                stm0, tug2, tu, result);

                     result =
                        WPX_FRMR_PM_SONET_SDH_HO_Path_EnableMonitoring
                        (transaction_number++, 0, &LineEndpointType,
                         WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_G1);
                     if (result)
                        printf ("G1 failure on %d:%d:%d:%d --> %d\n", stm1,
                                stm0, tug2, tu, result);
                  }

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

                     result =
                        WPX_FRMR_PM_SONET_SDH_LO_Path_EnableMonitoring
                        (transaction_number++, 0, &LineEndpointType,
                         WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_V5_BIP);
                     if (result)
                        printf ("V5_BIP failure on %d:%d:%d:%d --> %d\n",
                                stm1, stm0, tug2, tu, result);

                     result =
                        WPX_FRMR_PM_SONET_SDH_LO_Path_EnableMonitoring
                        (transaction_number++, 0, &LineEndpointType,
                         WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_V5_REI);
                     if (result)
                        printf ("V5_REI failure on %d:%d:%d:%d --> %d\n",
                                stm1, stm0, tug2, tu, result);
                  }
      }
      else
      {
         LineEndpointType.TransferType = WPX_UFE_FRAMER_WUFE_SONET_TYPE_T1;

         for (sts3 = 0; sts3 < 4; ++sts3)
            for (sts1 = 0; sts1 < 3; ++sts1)
               for (vt_group = 0; vt_group < 1; ++vt_group)
                  for (vt = 0; vt < 1; ++vt)
                  {
                     LineEndpointType.u.SONET.sts3 = sts3;
                     LineEndpointType.u.SONET.sts1 = sts1;
                     LineEndpointType.u.SONET.vt_group = vt_group;
                     LineEndpointType.u.SONET.vt = vt;

                     result =
                        WPX_FRMR_PM_SONET_SDH_HO_Path_EnableMonitoring
                        (transaction_number++, 0, &LineEndpointType,
                         WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_B3);
                     if (result)
                        printf ("B3 failure on %d:%d:%d:%d --> %d\n", sts3,
                                sts1, vt_group, vt, result);

                     result =
                        WPX_FRMR_PM_SONET_SDH_HO_Path_EnableMonitoring
                        (transaction_number++, 0, &LineEndpointType,
                         WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_G1);
                     if (result)
                        printf ("G1 failure on %d:%d:%d:%d --> %d\n", sts3,
                                sts1, vt_group, vt, result);

                     result =
                        WPX_FRMR_PM_SONET_SDH_LO_Path_EnableMonitoring
                        (transaction_number++, 0, &LineEndpointType,
                         WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_V5_BIP);
                     if (result)
                        printf ("V5_BIP failure on %d:%d:%d:%d --> %d\n",
                                sts3, sts1, vt_group, vt, result);

                     result =
                        WPX_FRMR_PM_SONET_SDH_LO_Path_EnableMonitoring
                        (transaction_number++, 0, &LineEndpointType,
                         WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_V5_REI);
                     if (result)
                        printf ("V5_REI failure on %d:%d:%d:%d --> %d\n",
                                sts3, sts1, vt_group, vt, result);
                  }
      }

      if (framing_mode == WUFE_FRAMED)
      {
         int j, k;
         WP_U8 result;

         for (j = k = 0; k < 80; j += 2)
         {
            if ((result =
                 WPX_FRMR_PM_T1_EnableMonitoringCrc (transaction_number++,
                                                     0,
                                                     j)) !=
                WPX_UFE_FRAMER_OK)
            {
               printf ("T1 CRC Monitoring failed on %d with %s\n", j,
                       OMIINO_FRAMER_ErrorCodeToTxt (result));
               break;
            }
            else
               ++k;
         }
      }
   }

   printf
      ("*****************     PM ENABLED ************************************\n");
}

#define SERIAL_1_CPLD_INTERRUPT_MASK 0x6f

WP_U8 last_interrupt_mask = SERIAL_1_CPLD_INTERRUPT_MASK;

void WT_ReinstateInterruptMask (WP_U32 wpid)
{

   /* Reset the CPLD interrupt mask */
   WPX_Ufe412CpldInterruptMaskSet (wpid, last_interrupt_mask);
}
