/*****************************************************************************
 * (C) Copyright PMC - WIN division (Wintegra) 2011.  All rights reserved.
 * PMC-Sierra (WINTEGRA) CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of PMC-Sierra (Wintegra).
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *  Application : Autmatic Protection Switch
 *  File Name   : ufe_utility.c
 *  Version     : 1
 *  Date        : 20 Jan 2012
 *  Accompanies : TBD
 *****************************************************************************/

#include <string.h>
#include "wp_wddi.h"
#include <wpl_platform.h>

#undef VERBOSE
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
framer_task_list the_alarm_task_list[1] = { {0, 0, ALARM_TASK_LIST_SIZE, 0, the_alarm_tasks} };

#define PM_TASK_LIST_SIZE 4096
framer_task the_pm_tasks[PM_TASK_LIST_SIZE];
framer_task_list the_pm_task_list[1] = { {0, 0, PM_TASK_LIST_SIZE, 0, the_pm_tasks} };

#define SIGNAL_TASK_LIST_SIZE 4096
framer_task the_signal_tasks[PM_TASK_LIST_SIZE];
framer_task_list the_signal_task_list[1] = { {0, 0, SIGNAL_TASK_LIST_SIZE, 0, the_signal_tasks} };

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

void add_framer_task (framer_task_list * task_list, framer_task * task)
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


    WPL_Unlock (WPL_THREAD_LOCK_KEY, &private_thread_lock);
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

framer_task *next_framer_task (framer_task_list * task_list, framer_task * result)
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
#if WTI_FLEXMUX_ENABLE_PERFORMANCE_MONITORING

    /* Allocate storage for the performance monitoring data */

#define SONET_SDH_PORT_STORAGE_NUM_ELEMENTS      120
#define SONET_SDH_HO_PATH_STORAGE_NUM_ELEMENTS   120
#define SONET_SDH_LO_PATH_STORAGE_NUM_ELEMENTS   120
#define PDH_STORAGE_NUM_ELEMENTS                 120

    void *space;
    int element_size;
    WUFE_status status;

    element_size = sizeof (WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_PORT_ENDPOINT_TYPE);
    space = malloc (SONET_SDH_PORT_STORAGE_NUM_ELEMENTS * element_size);
    if (space == NULL)
    WTI_FlexmuxCheckStatus ("PM storage allocation", WPX_UFE_FRAMER_ERROR_PARAMETER_OUT_OF_RANGE, __LINE__);
    status = WPX_FRMR_PM_SonetSdhPortStorage (space, SONET_SDH_PORT_STORAGE_NUM_ELEMENTS);
    if (status != WUFE_OK)
    WTI_FlexmuxCheckStatus ("PM storage allocation", WPX_UFE_FRAMER_ERROR_PARAMETER_OUT_OF_RANGE, __LINE__);

    element_size = sizeof (WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_HO_PATH_ENDPOINT_TYPE);
    space = malloc (SONET_SDH_HO_PATH_STORAGE_NUM_ELEMENTS * element_size);
    if (space == NULL)
    WTI_FlexmuxCheckStatus ("PM storage allocation", WPX_UFE_FRAMER_ERROR_PARAMETER_OUT_OF_RANGE, __LINE__);
    status = WPX_FRMR_PM_SonetSdhHOPathStorage (space, SONET_SDH_HO_PATH_STORAGE_NUM_ELEMENTS);
    if (status != WUFE_OK)
    WTI_FlexmuxCheckStatus ("PM storage allocation", WPX_UFE_FRAMER_ERROR_PARAMETER_OUT_OF_RANGE, __LINE__);

    element_size = sizeof (WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_LO_PATH_ENDPOINT_TYPE);
    space = malloc (SONET_SDH_LO_PATH_STORAGE_NUM_ELEMENTS * element_size);
    if (space == NULL)
    WTI_FlexmuxCheckStatus ("PM storage allocation", WPX_UFE_FRAMER_ERROR_PARAMETER_OUT_OF_RANGE, __LINE__);
    status = WPX_FRMR_PM_SonetSdhLOPathStorage (space, SONET_SDH_LO_PATH_STORAGE_NUM_ELEMENTS);
    if (status != WUFE_OK)
    WTI_FlexmuxCheckStatus ("PM storage allocation", WPX_UFE_FRAMER_ERROR_PARAMETER_OUT_OF_RANGE, __LINE__);

    element_size = sizeof (WPX_UFE_FRAMER_PDH_PERFORMANCE_MONITORING_ENDPOINT_TYPE);
    space = malloc (PDH_STORAGE_NUM_ELEMENTS * element_size);
    if (space == NULL)
    WTI_FlexmuxCheckStatus ("PM storage allocation", WPX_UFE_FRAMER_ERROR_PARAMETER_OUT_OF_RANGE, __LINE__);
    status = WPX_FRMR_PM_PdhStorage (space, PDH_STORAGE_NUM_ELEMENTS);
    if (status != WUFE_OK)
    WTI_FlexmuxCheckStatus ("PM storage allocation", WPX_UFE_FRAMER_ERROR_PARAMETER_OUT_OF_RANGE, __LINE__);

#endif
}



#define WTI_LINE_SIDE_ALARM_DISPLAY 0

static char *socket_client_side_alarm_names[] =
{
   "WPX_UFE_FRAMER_SOCKET_CLIENT_E_RFI",
   "WPX_UFE_FRAMER_SOCKET_CLIENT_A_RAI",
   "WPX_UFE_FRAMER_SOCKET_CLIENT_LOS_AIS"
};


void service_path_alarm (WP_U8 device_id, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE * p_SDH_Sonet_line_endpoint_type, WP_U8 alarm_category, WP_U8 is_asserted)
{
#if WTI_FLEXMUX_ENABLE_ALARMS
#if WTI_PRINT_ALARM_DATA
    WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *info = p_SDH_Sonet_line_endpoint_type;
    WP_U32 type = info->TransferType;

    if (type >= WPX_UFE_FRAMER_WUFE_SDH_TYPE_T1 && type <= WPX_UFE_FRAMER_WUFE_SDH_TYPE_VCAT)
    printf ("PATH ALARM:             STM4 = %d, STM1 = %d, STM0 = %d, TUG2 = %d, TU = %d\n", info->u.SDH.stm4, info->u.SDH.stm1, info->u.SDH.stm0, info->u.SDH.tug2, info->u.SDH.tu);
    else if (type >= WPX_UFE_FRAMER_WUFE_SONET_TYPE_T1 && type <= WPX_UFE_FRAMER_WUFE_SONET_TYPE_VCAT)
    printf
        ("PATH ALARM:             STS12 = %d, STS3 = %d, STS1 = %d, VT_GROUP = %d, CT = %d\n",
         info->u.SONET.sts12, info->u.SONET.sts3, info->u.SONET.sts1, info->u.SONET.vt_group, info->u.SONET.vt);
    else
    printf ("????? type = %d\n", type);
    printf ("             alarm_category = %s %s\n", line_side_alarm_names[alarm_category], is_asserted ? "Asserted" : "Unasserted");
#endif

#ifdef VERBOSE
    printf ("PATH ALARM:  alarm_category = %s %s\n", line_side_alarm_names[alarm_category], is_asserted ? "Asserted" : "Unasserted");
#endif
#endif
}

void service_port_alarm (WP_U8 device, WP_U8 line_port_id, WP_U8 alarm_category, WP_U8 is_asserted)
{
#if WTI_FLEXMUX_ENABLE_ALARMS
#ifdef VERBOSE
    printf ("PORT ALARM:  line_port_id = %d, alarm_category = %s %s\n", line_port_id, line_side_alarm_names[alarm_category], is_asserted ? "Asserted" : "Unasserted");
#endif
#endif
}

void service_los_alarm (WP_U8 device, WP_U8 line_port_id, WP_U8 alarm_category, WP_U8 is_asserted, framer_task * task)
{
#if WTI_FLEXMUX_ENABLE_ALARMS
    char buffer[64];
    WP_U8 force_state;
    WP_U8 result;
    static WP_U32 transaction = 700;
    extern WP_U8 WPX_FRMR_PL_SONET_SDH_PORT_ForceA (WP_U32 TransactionIdentifier, WP_U8 iDevice, WP_U8 iLinePort);
    extern WP_U8 WPX_FRMR_PL_SONET_SDH_PORT_ForceB (WP_U32 TransactionIdentifier, WP_U8 iDevice, WP_U8 iLinePort);
    extern WP_U8 WPX_FRMR_PL_SONET_SDH_PORT_GetForceState (WP_U8 iDevice, WP_U8 iLinePort, WP_U8 * pForceSide);


#ifdef VERBOSE
    printf ("PORT LOS ALARM:  line_port_id = %d, %s %s\n", line_port_id, line_side_alarm_names[alarm_category], is_asserted ? " ON" : "OFF");
#endif

#ifdef USE_ANT
    printf ("NO SWITCHING PERFORMED\n");
    return;
#endif

    result = WPX_FRMR_PL_SONET_SDH_PORT_GetForceState (device, 0 /*line_port_id */ ,
                                                   &force_state);

    if (result == WPX_UFE_FRAMER_OK)
    {
#if DEBUG
    if (force_state == WPX_UFE_FRAMER_PROTECTION_FORCE_A)
        printf ("Current Force State is A, line_port_id(%d)\n", line_port_id);
    else if (force_state == WPX_UFE_FRAMER_PROTECTION_FORCE_B)
        printf ("Current Force State is B, line_port_id(%d)\n", line_port_id);
#endif

    if (force_state == WPX_UFE_FRAMER_PROTECTION_FORCE_A)
    {
        if ((line_port_id & 1) == 0)
        {
            result = WPX_FRMR_PL_SONET_SDH_PORT_ForceB (transaction++, device, 0    /*line_port_id */
                );
            WTI_FlexmuxCheckStatus ("WPX_FRMR_SONET_SDH_PORT_ForceB", result, __LINE__);
            printf ("____________________________________________________________________\n");
            printf ("service_los_alarm: switch to PROTECTION PORT now\n");
				printf ("You can:\n");
				printf ("[1] insert back the loopback fiber to WORKING PORT again and\n");
				printf ("\tunplug fiber of PROTECTION PORT now to try another switch\n");
				printf ("[2] In the main MENU, send packets and\n");
				printf ("\tcheck the result to see if the traffic is OK now\n");
        }
        else
            add_framer_task (the_alarm_task_list, task);

    }
    else if (force_state == WPX_UFE_FRAMER_PROTECTION_FORCE_B)
    {
        if ((line_port_id & 1) == 1)
        {
            result = WPX_FRMR_PL_SONET_SDH_PORT_ForceA (transaction++, device, 0    /*line_port_id */
                );
            WTI_FlexmuxCheckStatus ("WPX_FRMR_SONET_SDH_PORT_ForceA", result, __LINE__);
            printf ("____________________________________________________________________\n");
            printf ("service_los_alarm: switch to WORKING PORT now\n");
				printf ("You can:\n");
				printf ("[1] insert back the loopback fiber to PROTECTION PORT again and\n");
				printf ("\tunplug fiber of WORKING PORT now to try another switch\n");
				printf ("[2] In the main MENU, send packets and\n");
				printf ("\tcheck the result to see if the traffic is OK now\n");
        }
        else
            add_framer_task (the_alarm_task_list, task);
    }
    else
        printf ("On %d:  force_state unknown %d\n", line_port_id, force_state);
    }
    else if (result == WPX_UFE_FRAMER_ERROR_PORT_RATE_NOT_CONFIGURED)
    {
    sprintf (buffer, "%s", (force_state == WPX_UFE_FRAMER_PROTECTION_FORCE_A) ? "is A" : (force_state == WPX_UFE_FRAMER_PROTECTION_FORCE_B ? "is B" : "UNKNOWN"));

    printf ("GetForceState line_port_id(%d), returns WPX_UFE_FRAMER_ERROR_PORT_RATE_NOT_CONFIGURED, current(%s)\n", line_port_id, buffer);
    }
    else
    {
    printf ("GetForceState returns %d\n", result);
    }


#endif
}

void service_k1_k2_signal (WP_U8 device, WP_U8 line_port_id, WP_U8 k1, WP_U8 k2)
{
#if WTI_FLEXMUX_ENABLE_ALARMS
#ifdef VERBOSE

    printf ("K1K2 SIGNAL:  iLinePort = %d K1 %d, K2 %d\n", line_port_id, k1, k2);

#endif
#endif
}

void service_s1_signal (WP_U8 device, WP_U8 line_port_id, WP_U8 s1)
{
#if WTI_FLEXMUX_ENABLE_ALARMS
#ifdef VERBOSE

    printf ("S1 SIGNAL:  iLinePort = %d S1 %d\n", line_port_id, s1);

#endif
#endif
}

#if WTI_FLEXMUX_ENABLE_ALARMS


void cb_port_alarm (WP_U8 device, WP_U8 line_port_id, WP_U8 alarm_category, WP_U8 is_asserted)
{
    framer_task task;
    APP_PORT_ALARM_TASK *port_alarm = &task.u.port_alarm_task;

    task.task_type = PORT_ALARM_TYPE;
    port_alarm->device = device;
    port_alarm->line_port_id = line_port_id;
    port_alarm->alarm_category = alarm_category;
    port_alarm->is_asserted = is_asserted;

    if ((alarm_category == WPX_UFE_FRAMER_SDH_LOS)
    || (alarm_category == WPX_UFE_FRAMER_SDH_LOF) || (alarm_category == WPX_UFE_FRAMER_SDH_MS_AIS) || (alarm_category == WPX_UFE_FRAMER_SONET_LOS) || (alarm_category == WPX_UFE_FRAMER_SONET_LOF))
    {
    if (is_asserted)
    {
        service_los_alarm (device, line_port_id, alarm_category, is_asserted, &task);
    }
    }
    else
    add_framer_task (the_alarm_task_list, &task);

}
#endif

#if WTI_FLEXMUX_ENABLE_PERFORMANCE_MONITORING

WP_U32 B1_count, B2_count, M1_count;

#endif

void service_pm_port_data (WP_U8 device_id, WP_U8 table_index, WP_U32 timestamp)
{
#if WTI_FLEXMUX_ENABLE_PERFORMANCE_MONITORING

    WP_U32 count;
    WP_U8 result;

#ifdef VERBOSE
    printf ("PM PORT:  device_id = %d, table_index = %d, timestamp = 0x%x\n", device_id, table_index, timestamp);
#endif

    if ((result = WPX_FRMR_STATUS_PM_SONET_SDH_Port_ReadData (device_id, table_index, 0, WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_B1, &count)) == 0)
    {
#ifdef VERBOSE
    printf ("port %d: B1 count %d\n", device_id, count);
#endif
    B1_count += count;
    }
    else
    printf ("Port_ReadData failed on B1: 0x%x\n", result);

    if ((result = WPX_FRMR_STATUS_PM_SONET_SDH_Port_ReadData (device_id, table_index, 0, WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_B2, &count)) == 0)
    {
#ifdef VERBOSE
    printf ("port %d: B2 count %d\n", device_id, count);
#endif
    B2_count += count;
    }
    else
    printf ("Port_ReadData failed on B2: 0x%x\n", result);

    if ((result = WPX_FRMR_STATUS_PM_SONET_SDH_Port_ReadData (device_id, table_index, 0, WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_M1, &count)) == 0)
    {
#ifdef VERBOSE
    printf ("port %d: M1 count %d\n", device_id, count);
#endif
    M1_count += count;
    }
    else
    printf ("Port_ReadData failed on M1: 0x%x\n", result);

#endif
}

void service_pm_ho_path_data (WP_U8 device_id, WP_U8 table_index, WP_U32 timestamp)
{
#if WTI_FLEXMUX_ENABLE_PERFORMANCE_MONITORING

    WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE LineEndpointType;
    U32 count = 0;
    U32 max_stm1, max_stm0, max_tug2, max_tu;
    U32 stm1, stm0, tug2, tu;
    U8 result;

    memset (&LineEndpointType, 0, sizeof (WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE));

#ifdef VERBOSE
    printf ("HO PATH:  table_index = %d timestamp = 0x%x\n", table_index, timestamp);
#endif

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

                result = WPX_FRMR_STATUS_PM_SONET_SDH_HO_PATH_ReadData (device_id, table_index, &LineEndpointType, WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_B3, &count);

                if (result)
                    printf ("    ---->  HO_Path_ReadData failed on %d:%d:%d:%d with %d\n", stm1, stm0, tug2, tu, result);
#ifdef VERBOSE
                else
                    printf ("    ---->  B3 count on %d:%d:%d:%d is %d\n", stm1, stm0, tug2, tu, count);
#endif

                result = WPX_FRMR_STATUS_PM_SONET_SDH_HO_PATH_ReadData (device_id, table_index, &LineEndpointType, WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_G1, &count);

                if (result)
                    printf ("    ---->  HO_Path_ReadData failed on %d:%d:%d:%d with %d\n", stm1, stm0, tug2, tu, result);
#ifdef VERBOSE
                else
                    printf ("    ---->  G1 count on %d:%d:%d:%d is %d\n", stm1, stm0, tug2, tu, count);
#endif
            }

#endif
}

void service_pm_lo_path_data (WP_U8 device_id, WP_U8 table_index, WP_U32 timestamp)
{
#if WTI_FLEXMUX_ENABLE_PERFORMANCE_MONITORING

    WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE LineEndpointType;
    U32 count = 0;
    U32 max_stm1, max_stm0, max_tug2, max_tu;
    U32 stm1, stm0, tug2, tu;
    U8 result;

    memset (&LineEndpointType, 0, sizeof (WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE));

#ifdef VERBOSE
    printf ("LO PATH:  table_index = %d timestamp = 0x%x\n", table_index, timestamp);
#endif

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

                result = WPX_FRMR_STATUS_PM_SONET_SDH_LO_PATH_ReadData (device_id, table_index, &LineEndpointType, WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_V5_BIP, &count);

                if (result)
                    printf ("    ---->  LO_Path_ReadData failed on %d:%d:%d:%d with %d\n", stm1, stm0, tug2, tu, result);
#ifdef VERBOSE
                else
                    printf ("    ---->  V5_BIP count on %d:%d:%d:%d is %d\n", stm1, stm0, tug2, tu, count);
#endif

                result = WPX_FRMR_STATUS_PM_SONET_SDH_LO_PATH_ReadData (device_id, table_index, &LineEndpointType, WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_V5_REI, &count);

                if (result)
                    printf ("    ---->  LO_Path_ReadData failed on %d:%d:%d:%d with %d\n", stm1, stm0, tug2, tu, result);
#ifdef VERBOSE
                else
                    printf ("    ---->  V5_REI count on %d:%d:%d:%d is %d\n", stm1, stm0, tug2, tu, count);
#endif
            }
#endif
}

void service_pm_pdh_data (WP_U8 device_id, WP_U8 table_index, WP_U32 timestamp)
{
#if WTI_FLEXMUX_ENABLE_PERFORMANCE_MONITORING
#ifdef VERBOSE
    printf ("PDH:  table_index = %d timestamp = 0x%x\n", table_index, timestamp);
#endif
#endif
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

                service_port_alarm (port_alarm->device, port_alarm->line_port_id, port_alarm->alarm_category, port_alarm->is_asserted);

                break;

            case PATH_ALARM_TYPE:

                path_alarm = &a_task.u.path_alarm_task;

                service_path_alarm (path_alarm->device, &path_alarm->endpoint, path_alarm->alarm_category, path_alarm->is_asserted);

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

                service_k1_k2_signal (k1k2_signal->device, k1k2_signal->line_port_id, k1k2_signal->k1_value, k1k2_signal->k2_value);

                break;

            case S1_SIGNAL_TYPE:

                s1_signal = &a_task.u.s1_signal_task;

                service_s1_signal (s1_signal->device, s1_signal->line_port_id, s1_signal->s1_value);

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

                service_pm_port_data (pm_task->device, pm_task->table_index, pm_task->timestamp);

                break;

            case PM_HO_PATH_TYPE:

                pm_task = &a_task.u.pm_task;

                service_pm_ho_path_data (pm_task->device, pm_task->table_index, pm_task->timestamp);

                break;

            case PM_LO_PATH_TYPE:

                pm_task = &a_task.u.pm_task;

                service_pm_lo_path_data (pm_task->device, pm_task->table_index, pm_task->timestamp);

                break;

            case PM_PDH_TYPE:

                pm_task = &a_task.u.pm_task;

                service_pm_pdh_data (pm_task->device, pm_task->table_index, pm_task->timestamp);

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


WP_U8 last_interrupt_mask;

void WT_Eint3Interrupt (WP_U32 wpid, WP_U32 signal_info)
{
    WP_boolean is_framer_int_0 = WP_FALSE, is_core_int_0 = WP_FALSE;
    WP_boolean is_framer_int_2 = WP_FALSE, is_core_int_2 = WP_FALSE;
#if CHECK_CORE_HW_INTERRUPT_ASSERTED
    WUFE_events active_events;
#endif
    WP_U8 pending;
    extern void WPX_UFE_FRAMER_ISR (WP_U8 iDevice);

#if WTI_COLLECT_TIMING_INFORMATION
    record_action (13, WP_TimeRead ());
#endif


    /* Recover the value of the CPLD interrupt mask at the time of the interrupt */
#if !defined(__linux__)
    last_interrupt_mask = WPX_Ufe412CpldInterruptMaskGet (0);
#else
    last_interrupt_mask = signal_info & 0xffff;
#endif

    pending = WPX_Ufe412CpldInterruptSourceGet (wpid);

#if WTI_COLLECT_TIMING_INFORMATION
    record_action (16, pending);
#endif

    if ((pending & WPX_FPGA_INTR_SERIAL_1_CONNECTOR) == 0)
    {
        /* Assume UFEID == 0 is on UPI1; needs to be generalized */
        is_framer_int_0 = WUFE_UfeFramerHwInterruptGet (0);
        is_core_int_0 = WUFE_UfeCoreHwInterruptGet (0);
    }

    if ((pending & WPX_FPGA_INTR_SERIAL_3_CONNECTOR) == 0)
    {
        /* Assume UFEID == 1 is on UPI3; needs to be generalized */
        is_framer_int_2 = WUFE_UfeFramerHwInterruptGet (1);
        is_core_int_2 = WUFE_UfeCoreHwInterruptGet (1);
    }

#if WTI_COLLECT_TIMING_INFORMATION

    if ((pending & WPX_FPGA_INTR_SERIAL_1_CONNECTOR) == 0)
    {
        if (is_framer_int_0 || is_core_int_0)
            record_action (43, (0 << 24) + (is_core_int_0 << 8) + is_framer_int_0);
        else
        {
            record_action (46, WUFE_UfeExtPllHwInterruptGet (0));
            record_action (45, WUFE_UfeLockLostHwInterruptGet (0));
            record_action (44, WUFE_UfeSfpHwInterruptGet (0));
        }
    }

    if ((pending & WPX_FPGA_INTR_SERIAL_3_CONNECTOR) == 0)
    {
        if (is_framer_int_2 || is_core_int_2)
            record_action (43, (2 << 24) + (is_core_int_2 << 8) + is_framer_int_2);
        else
        {
            record_action (46, (2 << 24) + WUFE_UfeExtPllHwInterruptGet (1));
            record_action (45, (2 << 24) + WUFE_UfeLockLostHwInterruptGet (1));
            record_action (44, (2 << 24) + WUFE_UfeSfpHwInterruptGet (1));
        }
    }

#endif

    if (is_framer_int_0)
        WPX_UFE_FRAMER_ISR (0); /* argument is iDevice on UPI1 */

#if CHECK_CORE_HW_INTERRUPT_ASSERTED

    if (is_core_int_0)
    {
        memset (&active_events, 0, sizeof (WUFE_events));

        WUFE_UfeReadEvents (0, &active_events); /* Assumption about UFEID (see above) */

#if CHECK_CORE_HW_INTERRUPT_ASSERTED
        if (event_contents_0 == WP_FALSE)
        {
            memcpy (&events_connector_0, &active_events, sizeof (WUFE_events));
            event_contents_0 = WP_TRUE;
        }
#endif
    }

    if (is_framer_int_2)
        WPX_UFE_FRAMER_ISR (1); /* argument is iDevice on UPI3 */

    if (is_core_int_2)
    {
        memset (&active_events, 0, sizeof (WUFE_events));

        WUFE_UfeReadEvents (1, &active_events); /* Assumption about UFEID (see above) */

#if CHECK_CORE_HW_INTERRUPT_ASSERTED
        if (event_contents_1 == WP_FALSE)
        {
            memcpy (&events_connector_2, &active_events, sizeof (WUFE_events));
            event_contents_1 = WP_TRUE;
        }
#endif
    }
#endif

#if WTI_COLLECT_TIMING_INFORMATION
    record_action (14, WP_TimeRead ());
#endif

#if !defined(WPL_MAILBOX_LOCK_KEY)
#define WPL_MAILBOX_LOCK_KEY WPL_LOCK_KEY_CREATE(WPL_HW_LOCK, WPL_PRIVATE_LOCK, 6, 0)
#endif

    if (is_framer_int_0 || is_framer_int_2)
    {
        WPL_Unlock (WPL_MAILBOX_LOCK_KEY, 0);
#if WTI_COLLECT_TRACE_INFORMATION
        record_action (40, WP_TimeRead ());
#endif
    }

    WT_ReinstateInterruptMask (wpid);
}

extern void WPL_InterruptConfigureEint3(WP_U32 wpid);
#if 0
void WPL_InterruptConfigureEint3(WP_U32 wpid)
{
   WP_U32 val;
   WP_U32 *IC_IntMode;
   extern WP_U32 wpl_rif_base;

   if (wpl_rif_base == 0)
      WPLI_BusesRifBaseGet(wpid, &wpl_rif_base);

   IC_IntMode = ((WP_U32 *)(wpl_rif_base + 0xc2dc));

   WP_MEM_GET(val, *IC_IntMode);
   val |= 0x8000;
   WP_MEM_SET(*IC_IntMode, val);
}
#endif


static void WTI_EnableUfeInterrupts (void)
{
    WUFE_status ufe_status;

    ufe_status = WUFE_UfeFramerHwInterruptEnable(ufe4_app_system.ufeid);
    WT_UfeTerminateOnError(ufe_status, "WUFE_UfeFramerHwInterruptEnable",0,__LINE__);

    ufe_status = WUFE_UfeCoreHwInterruptEnable(ufe4_app_system.ufeid);
    WT_UfeTerminateOnError(ufe_status, "WUFE_UfeCoreHwInterruptEnable",0,__LINE__);

    ufe_status = WUFE_SystemInterruptEnable(ufe4_app_system.ufeid);
    WT_UfeTerminateOnError(ufe_status, "WUFE_SystemInterruptEnable ",0,__LINE__);

    ufe_status = WUFE_UfeCoreHwInterruptDisable(ufe4_app_system.ufeid);
    WT_UfeTerminateOnError(ufe_status, "WUFE_UfeCoreHwInterruptDisable",0,__LINE__);

    return ;
}

void WTI_DisableIntrrupts(void)
{
    // WPX_Ufe412CpldInterruptMaskSet (WP_WINPATH(0), 0x00);
    WPL_IntDisable (WP_WINPATH (0), WPL_IntGlobalState);
}


int WTI_ConfigUfeInterrupts (void)
{
    WPL_IntConnect (WP_WINPATH(0), WPL_Eint3Ufe4, 0, WT_Eint3Interrupt);

    WPL_IntDisable (WP_WINPATH (0), WPL_IntGlobalState);
    WPL_InterruptConfigureEint3 (WP_WINPATH (0));
    WPL_IntEnable (WP_WINPATH (0), WPL_IntGlobalState);

    WPX_Ufe412CpldInterruptSelect (WP_WINPATH(0));

    if (WPL_OK != WPL_IntEnable (WP_WINPATH(0), WPL_Eint3Ufe4)) 
    {
        printf ("WPL_IntEnable() failed. line(%d)\n", __LINE__);
        return -1;
    }

    WPX_Ufe412CpldInterruptMaskSet (WP_WINPATH(0), 0x6e);
    WTI_EnableUfeInterrupts  ();
    
    return 0;
}

void WTI_enable_alarms (int type)
{
#if WTI_FLEXMUX_ENABLE_ALARMS
    WP_boolean is_sdh = (type == 0);

    int j;
    WP_U8 status;
    int start_alarm, last_alarm;

    WPX_FRMR_RegisterSonetSdhPortAlarmCallback ((void *) ((int) cb_port_alarm));

    if (0 != WTI_ConfigUfeInterrupts ()) return ;

    if (is_sdh)
    {
        start_alarm = WPX_UFE_FRAMER_SDH_LOS;
        last_alarm = WPX_UFE_FRAMER_SDH_LOS;
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
                 "   %d     SDH/SONET EVENT %d FAILED with %s\n" "********************************************************************\n", __LINE__, j, OMIINO_FRAMER_ErrorCodeToTxt (status));
            WTI_FlexmuxCheckStatus ("WTI_enable_alarms_and_performance_monitoring", status, __LINE__);
        }
    }

    j = socket_client_side_alarm_names[0][0];

    printf ("***************** ALARMS ENABLED ************************************\n");

#endif
}


void WT_ReinstateInterruptMask (WP_U32 wpid)
{
    /* Reset the CPLD interrupt mask */
    WPX_Ufe412CpldInterruptMaskSet (wpid, last_interrupt_mask);
}

/****************************************************************************************************************************
 * Function name: WT_UfeTerminateOnError()
 * Description  :   

 * Input  params: none
 * Output params: 
 * Return val   : none
 *****************************************************************************************************************************/

void WT_UfeTerminateOnError (WUFE_status handle, WP_CHAR * s, WP_U32 id, WP_U32 LineNum)
{
    if (handle != WUFE_OK)
    {
        printf ("Test Abort %s %d %s 0x%x Line:%d\n", s, id, WUFE_error_name[(handle & 0x03ffffff)], handle, LineNum);
        exit (1);
    }

}

/****************************************************************************************************************************
 * Function name: WT_UfeInitializeFirmware()
 * Description  :   

 * Input  params: none
 * Output params: 
 * Return val   : none
 *****************************************************************************************************************************/

void WT_UfeInitializeFirmware (WP_U32 ufe_id)   // burns the synthesis (func #2 - after WP_SysInit() )
{
    WUFE_init_config config;

    memset (&config, 0, sizeof (WUFE_init_config));
    config.ufe_id = ufe_id;
    if (WTI_EMPHY_PORT == WP_PORT_UPI2)
    config.attributes |= WUFE_ATTRIB_CONNECT_OFFSET_F (1);

    WUFE_UfeInitializeFirmware (&config);

}                               // WT_UfeInitializeFirmware
