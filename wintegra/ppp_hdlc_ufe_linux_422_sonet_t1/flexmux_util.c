/*****************************************************************************
 * (C) Copyright PMC - WIN division (Wintegra) 2011.  All rights reserved.
 * PMC-Sierra (WINTEGRA) CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of PMC-Sierra (Wintegra).
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *  Application : ATM PWE3 UFE4
 *  File Name   : flexmux_util.c
 *  Version     : 1
 *  Date        : 02 Oct 2011
 *  Accompanies : TBD
 *****************************************************************************/

/****************************************************************************
 *
 * Example: wti_flexmux_util.c
 * Demonstrates the use of Omiino Flexmux API.
 *
 ****************************************************************************/

#include <string.h>
#include <stdio.h>
#include "wp_wddi.h"


#ifndef WTI_UFE_UTILITY_H
#include "ufe_utility.h"
#endif


#ifndef WTI_FLEXMUX_UTIL_H
#include "flexmux_util.h"
#endif


#ifndef _WPL_PLATFORM_H__
#include <wpl_platform.h>
#endif

#define WTI_UFE412_REVA 1
/****************************************************************************************************************************
 * Function name: WTI_FlexmuxTerminateOnError()
 * Description    :
 *
 *
 * Input  params: none
 * Output params: 
 * Return val   : none
 *****************************************************************************************************************************/
void WTI_FlexmuxTerminateOnError(WP_CHAR * pFatalErrorMessage, WP_U8 error_code)
{
	printf("Test Abort: %s Line:%d Error code %d\n", pFatalErrorMessage, __LINE__, (WP_U32) error_code);
	exit(-1);
}
/****************************************************************************************************************************
 * Function name: WTI_FlexmuxCheckStatus()
 * Description    :
 *
 *
 * Input  params: none
 * Output params: 
 * Return val   : none
 *****************************************************************************************************************************/
void WTI_FlexmuxCheckStatus(WP_CHAR * Message, WP_U8 status, WP_U32 line)
{
	extern char *OMIINO_FRAMER_ErrorCodeToTxt(WP_U32 ErrorCode);
	if (status)
	{
		printf("Test Abort: function %s in Line:%d returned Error code %s\n",
				Message, line, OMIINO_FRAMER_ErrorCodeToTxt((WP_U32) status));
		exit(-1);
	}
	else
	{
#if WTI_FLEXMUX_DBG_MODE
		printf("Status returned from %s is WPX_UFE_FRAMER_OK\n", Message);
		return;
#endif
	}
}


#if WTI_FLEXMUX_ENABLE_ALARMS

void cb_port_alarm (WP_U8 device, WP_U8 line_port_id, WP_U8 alarm_category, WP_U8 is_asserted)
{
#if 0
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
#endif
}
#endif



extern void WPL_InterruptConfigureEint3(WP_U32 wpid);
extern char *OMIINO_FRAMER_ErrorCodeToTxt (WP_U32 ErrorCode);
static char *socket_client_side_alarm_names[] =
{
   "WPX_UFE_FRAMER_SOCKET_CLIENT_E_RFI",
   "WPX_UFE_FRAMER_SOCKET_CLIENT_A_RAI",
   "WPX_UFE_FRAMER_SOCKET_CLIENT_LOS_AIS"
};


void process_framer_task_list (void)
{
}



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



WP_U8 last_interrupt_mask;
void WT_ReinstateInterruptMask (WP_U32 wpid)
{
    /* Reset the CPLD interrupt mask */
    WPX_Ufe412CpldInterruptMaskSet (wpid, last_interrupt_mask);
}





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



int WTI_ConfigUfeInterrupts (void)
{
    WPL_IntConnect (WP_WINPATH(0), WPL_Eint3Ufe4, 0, WT_Eint3Interrupt);

#if 0
    WPL_IntDisable (WP_WINPATH (0), WPL_IntGlobalState);
    WPL_InterruptConfigureEint3 (WP_WINPATH (0));
    WPL_IntEnable (WP_WINPATH (0), WPL_IntGlobalState);
#endif

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

#if 0
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
#endif


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
	printf ("now it is E1\n");
        start_alarm = WPX_UFE_FRAMER_SDH_LOS;
        last_alarm = WPX_UFE_FRAMER_SDH_LOS;
    }
    else
    {
	printf ("now it is T1\n");
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
                 "   %d     SDH/SONET EVENT %d FAILED with %scode(%d)\n" \
    "********************************************************************\n", 
		__LINE__,j, OMIINO_FRAMER_ErrorCodeToTxt (status), status);
            WTI_FlexmuxCheckStatus ("WPX_FRMR_DEV_DRV_SONET_SDH_EnableAlarm ", 
		status, __LINE__);
        } else {
		printf ("SDH_EnableAlarm() OK on alarm(%3d)\n", j);
		fflush (stdout);
	}
    }

    j = socket_client_side_alarm_names[0][0];

    printf ("***************** ALARMS ENABLED ************************************\n");
#else
#error WTI_FLEXMUX_ENABLE_ALARMS_should_defined
#endif
}



/****************************************************************************************************************************
 * Function name: WTI_FlexmuxInit()
 * Description    :
 *
 *
 * Input  params: none
 * Output params: 
 * Return val   : none
 *****************************************************************************************************************************/
WP_S32 WTI_FlexmuxInit(WP_U32 upi_index,
		WP_U8 flexmux_id,
		WTI_flexmux_physical_conf flexmux_hw_config,
		WP_U8 flexmux_mode,
		WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4,
		WTI_Flexmux_global_setup *flexmux_cfg)
{
	WUFE_status status;
	WP_U8 max_line_ports=0, line_increment=0;
	WP_U8 wti_flexmux_build_personality=0;
	WTI_flexmux_line_port_mode line_port_mode=0;

	if (flexmux_id > WTI_FLEXMUX_NUM_DEVICES)
		WTI_FlexmuxTerminateOnError("flexmux_id is out of bounds !", 0);

	if (flexmux_hw_config != WT_FLEXMUX_UFE_412_1_PORT_OC12 &&
			flexmux_hw_config != WT_FLEXMUX_UFE_412_2_PORTS_OC3 &&
			flexmux_hw_config != WT_FLEXMUX_UFE_412_4_PORTS_OC3 &&
			flexmux_hw_config != WT_FLEXMUX_UFE_448_4_PORTS_OC12_PLUS_4_PROTECTION &&
			flexmux_hw_config != WT_FLEXMUX_UFE_412_4_PORTS_OC3_CARD_PROTECTION_MASTER &&
			flexmux_hw_config != WT_FLEXMUX_UFE_412_4_PORTS_OC3_CARD_PROTECTION_SLAVE)
		WTI_FlexmuxTerminateOnError("Invalid flexmux_hw_config !", 0);

	if (flexmux_mode != WPX_UFE_FRAMER_DEVICE_MODE_SONET &&
			flexmux_mode != WPX_UFE_FRAMER_DEVICE_MODE_SDH)
		WTI_FlexmuxTerminateOnError("Invalid flexmux_mode !", 0);

	//WTI_allocate_pm_storage_areas();

	if (flexmux_hw_config == WT_FLEXMUX_UFE_412_1_PORT_OC12)
	{
		if (flexmux_mode == WPX_UFE_FRAMER_DEVICE_MODE_SONET)
		{
			line_port_mode = WT_FLEXMUX_LINE_PORT_OC12;
		}
		else if (flexmux_mode == WPX_UFE_FRAMER_DEVICE_MODE_SDH)
		{
			line_port_mode = WT_FLEXMUX_LINE_PORT_STM4;
		}
		max_line_ports = 1;
		line_increment = 1;
		wti_flexmux_build_personality = WPX_UFE_FRAMER_BUILD_PERSONALITY_1_PLUS_1_OC3_OC12;
	}
	else if (flexmux_hw_config == WT_FLEXMUX_UFE_412_4_PORTS_OC3 ||
			flexmux_hw_config == WT_FLEXMUX_UFE_412_4_PORTS_OC3_CARD_PROTECTION_MASTER ||
			flexmux_hw_config == WT_FLEXMUX_UFE_412_4_PORTS_OC3_CARD_PROTECTION_SLAVE)
	{
		if (flexmux_mode == WPX_UFE_FRAMER_DEVICE_MODE_SONET)
		{
			line_port_mode = WT_FLEXMUX_LINE_PORT_OC3;
		}
		else if (flexmux_mode == WPX_UFE_FRAMER_DEVICE_MODE_SDH)
		{
			line_port_mode = WT_FLEXMUX_LINE_PORT_STM1;
		}
		max_line_ports = 4;
		line_increment = 1;
		wti_flexmux_build_personality = WPX_UFE_FRAMER_BUILD_PERSONALITY_4_PLUS_4_OC3;
	}
	else if (flexmux_hw_config == WT_FLEXMUX_UFE_412_2_PORTS_OC3)
	{
		if (flexmux_mode == WPX_UFE_FRAMER_DEVICE_MODE_SONET)
		{
			line_port_mode = WT_FLEXMUX_LINE_PORT_OC3;
		}
		else if (flexmux_mode == WPX_UFE_FRAMER_DEVICE_MODE_SDH)
		{
			line_port_mode = WT_FLEXMUX_LINE_PORT_STM1;
		}
		max_line_ports = 2;
		line_increment = 1;
		wti_flexmux_build_personality = WPX_UFE_FRAMER_BUILD_PERSONALITY_2_PLUS_2_OC3;
	}
	else if (flexmux_hw_config == WT_FLEXMUX_UFE_448_4_PORTS_OC12_PLUS_4_PROTECTION)
	{
		if (flexmux_mode == WPX_UFE_FRAMER_DEVICE_MODE_SONET)
		{
			line_port_mode = WT_FLEXMUX_LINE_PORT_OC12;
		}
		else if (flexmux_mode == WPX_UFE_FRAMER_DEVICE_MODE_SDH)
		{
			line_port_mode = WT_FLEXMUX_LINE_PORT_STM4;
		}
		max_line_ports = 8;
		line_increment = 2;
		wti_flexmux_build_personality = WPX_UFE_FRAMER_BUILD_PERSONALITY_4_PLUS_4_OC3_OC12;
	}
	else
	{
		WTI_FlexmuxTerminateOnError("Invalid flexmux_hw_config !", 0);
	}
	memset(flexmux_cfg, 0, sizeof(WTI_Flexmux_global_setup));

	/* set flexmux ID */
	flexmux_cfg->flexmux_id = flexmux_id;

	/* Initialize the Flexmux device */
	status = WPX_UFE_FRAMER_FlexmuxInit(flexmux_cfg->flexmux_id,
			upi_index);
	WTI_FlexmuxCheckStatus("WPX_UFE_FRAMER_FlexmuxInit", status, __LINE__);

	/* service the mailboxes in a separate thread */
	{
		WP_THREAD_ID mailbox_thread_id;
		WP_U32 thread_params[2];

		thread_params[0] = flexmux_cfg->flexmux_id;
		thread_params[1] = upi_index;

		WPL_ThreadInit(&mailbox_thread_id,
				(void *(*)(void *)) WPX_UFE_FRAMER_FlexmuxServiceMailbox,
				(void *) thread_params);
	}

#if WTI_FLEXMUX_ALARM_AND_PM_THREAD

	/* service the alarms and PM callbacks in a separate thread */
	{
		WP_THREAD_ID alarm_and_pm_thread_id;
		WP_U32 thread_params[2] = { 0, 1 };
		void process_framer_task_list(void);

		WPL_ThreadInit(&alarm_and_pm_thread_id,
				(void *(*)(void *)) process_framer_task_list,
				(void *) thread_params);
	}

#endif

	/* set the Flexmux device build personality */
	status = WPX_UFE_FRAMER_FlexmuxSetBuildPersonality(flexmux_cfg->flexmux_id,
			wti_flexmux_build_personality,
			upi_index);
	WTI_FlexmuxCheckStatus("WPX_UFE_FRAMER_FlexmuxSetBuildPersonality", status, __LINE__);

#if WTI_UFE412_REVA
	/* set the gpt source clock required with wds-ufe4-s6 board*/
	WPX_FRMR_DEMO_ONLY_Configure_GTP_ClockSource(flexmux_cfg->flexmux_id);
#endif
#if WTI_FLEXMUX_DBG_MODE
	U8 pDeviceProtectionMode;
	U8 pCardProtectionOperatingMode;
#endif
	/* Set line protection according to the build personality in use*/
	if(flexmux_hw_config == WT_FLEXMUX_UFE_412_4_PORTS_OC3_CARD_PROTECTION_MASTER){
		status = WPX_FRMR_SetDevProtectMode( 0, flexmux_cfg->flexmux_id, WPX_UFE_DEVICE_PROTECTION_MODE_CARD);
		WTI_FlexmuxCheckStatus("WPX_FRMR_SetDevProtectMode", status, __LINE__);
#if WTI_FLEXMUX_DBG_MODE
		WPX_FRMR_GetDevProtectMode (flexmux_cfg->flexmux_id,&pDeviceProtectionMode);
		if(pDeviceProtectionMode != WPX_UFE_DEVICE_PROTECTION_MODE_CARD)
			WTI_FlexmuxTerminateOnError("WPX_FRMR_GetDevProtectMode != WPX_UFE_DEVICE_PROTECTION_MODE_CARD\n", 0);
#endif
		status = WPX_FRMR_SetDevCardProtectOperatingMode(0,flexmux_cfg->flexmux_id,WPX_UFE_DEVICE_CARD_PROTECTION_OPERATING_MODE_MASTER);
		WTI_FlexmuxCheckStatus("WPX_FRMR_SetDevCardProtectOperatingMode", status, __LINE__);
#if WTI_FLEXMUX_DBG_MODE
		WPX_FRMR_GetDevCardProtectOperatingMode(flexmux_cfg->flexmux_id, &pCardProtectionOperatingMode);
		if(pCardProtectionOperatingMode!=WPX_UFE_DEVICE_CARD_PROTECTION_OPERATING_MODE_MASTER)
			WTI_FlexmuxTerminateOnError("WPX_FRMR_GetDevCardProtectOperatingMode != WPX_UFE_DEVICE_CARD_PROTECTION_OPERATING_MODE_MASTER\n", 0);
#endif
	}
	else if(flexmux_hw_config == WT_FLEXMUX_UFE_412_4_PORTS_OC3_CARD_PROTECTION_SLAVE){
		status = WPX_FRMR_SetDevProtectMode( 0, flexmux_cfg->flexmux_id, WPX_UFE_DEVICE_PROTECTION_MODE_CARD);
		WTI_FlexmuxCheckStatus("WPX_FRMR_SetDevProtectMode", status, __LINE__);
#if WTI_FLEXMUX_DBG_MODE
		WPX_FRMR_GetDevProtectMode (flexmux_cfg->flexmux_id,&pDeviceProtectionMode);
		if(pDeviceProtectionMode != WPX_UFE_DEVICE_PROTECTION_MODE_CARD)
			WTI_FlexmuxTerminateOnError("WPX_FRMR_GetDevProtectMode != WPX_UFE_DEVICE_PROTECTION_MODE_CARD\n", 0);
#endif
		status = WPX_FRMR_SetDevCardProtectOperatingMode(0,flexmux_cfg->flexmux_id,WPX_UFE_DEVICE_CARD_PROTECTION_OPERATING_MODE_SLAVE);
		WTI_FlexmuxCheckStatus("WPX_FRMR_SetDevCardProtectOperatingMode", status, __LINE__);
#if WTI_FLEXMUX_DBG_MODE
		WPX_FRMR_GetDevCardProtectOperatingMode(flexmux_cfg->flexmux_id, &pCardProtectionOperatingMode);
		if(pCardProtectionOperatingMode!=WPX_UFE_DEVICE_CARD_PROTECTION_OPERATING_MODE_SLAVE)
			WTI_FlexmuxTerminateOnError("WPX_FRMR_GetDevCardProtectOperatingMode != WPX_UFE_DEVICE_CARD_PROTECTION_OPERATING_MODE_SLAVE\n", 0);
#endif

	}
	else{
		status = WPX_FRMR_SetDevProtectMode( 0, flexmux_cfg->flexmux_id, WPX_UFE_DEVICE_PROTECTION_MODE_LINE);
		WTI_FlexmuxCheckStatus("WPX_FRMR_SetDevProtectMode", status, __LINE__);
#if WTI_FLEXMUX_DBG_MODE
		WPX_FRMR_GetDevProtectMode (flexmux_cfg->flexmux_id,&pDeviceProtectionMode);
		if(pDeviceProtectionMode != WPX_UFE_DEVICE_PROTECTION_MODE_LINE)
			WTI_FlexmuxTerminateOnError("WPX_FRMR_GetDevProtectMode != WPX_UFE_DEVICE_PROTECTION_MODE_LINE\n", 0);
#endif
	}
	/* set device mode (SONET\SDH) */
	status = WPX_FRMR_DEV_SetMode(0, flexmux_cfg->flexmux_id, flexmux_mode);
	WTI_FlexmuxCheckStatus("WPX_UFE_FRAMER_CONFIGURATION_DEVICE_SetMode", status, __LINE__);
#if WTI_FLEXMUX_DBG_MODE
	U8 pDeviceMode;
	status = WPX_FRMR_DEV_GetMode(flexmux_cfg->flexmux_id,&pDeviceMode );
	WTI_FlexmuxCheckStatus("WPX_FRMR_DEV_GetMode", status, __LINE__);
	switch(flexmux_mode)
	{
		case WPX_UFE_FRAMER_DEVICE_MODE_SDH:
			if(pDeviceMode != WPX_UFE_FRAMER_DEVICE_MODE_SDH)
			{
				WTI_FlexmuxTerminateOnError("WPX_FRMR_DEV_GetMode != WPX_UFE_DEVICE_MODE_SDH\n", 0);
			}
			break;
		case WPX_UFE_FRAMER_DEVICE_MODE_SONET:
			if(pDeviceMode != WPX_UFE_FRAMER_DEVICE_MODE_SONET)
			{
				WTI_FlexmuxTerminateOnError("WPX_FRMR_DEV_GetMode != WPX_UFE_DEVICE_MODE_SONET\n", 0);
			}
			break;
	}
#endif
	/* set flexmux physical configuration */
	flexmux_cfg->flexmux_hw_config = flexmux_hw_config;

	/* set flexmux build personality */
	flexmux_cfg->flexmux_build_personality = wti_flexmux_build_personality;

	/* save device parameters in registry */
	flexmux_reg[flexmux_id].is_initialized = WP_TRUE;
	flexmux_reg[flexmux_id].flexmux_hw_config = flexmux_hw_config;
	flexmux_reg[flexmux_id].flexmux_build_personality = wti_flexmux_build_personality;
	flexmux_reg[flexmux_id].flexmux_mode = flexmux_mode;
	flexmux_reg[flexmux_id].max_line_ports = max_line_ports;

	if (wti_flexmux_build_personality == WPX_UFE_FRAMER_BUILD_PERSONALITY_4_PLUS_4_OC3_OC12 ||
			wti_flexmux_build_personality == WPX_UFE_FRAMER_BUILD_PERSONALITY_1_PLUS_1_OC3_OC12 ||
			wti_flexmux_build_personality == WPX_UFE_FRAMER_BUILD_PERSONALITY_2_PLUS_2_OC3      ||
			wti_flexmux_build_personality == WPX_UFE_FRAMER_BUILD_PERSONALITY_4_PLUS_4_OC3)
	{
		WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE line_end_point = {0};

#if WTI_ADM_MODE
		/* in CR when using the ADM for testing */
		U8 TX_C2=0x1, EX_C2=0x1;
		WP_CHAR *TX_J1="0000000000000000", *EX_J1="0000000000000000",*pTX_TraceString="WG RS-TRACE",*pEX_TraceString="WG RS-TRACE";
#else /*  WTI_ADM_MODE*/
		U8 TX_C2=0x2, EX_C2=0x2;
		WP_CHAR *TX_J1="WG HP-TRACE", *EX_J1="WG HP-TRACE",*pTX_TraceString="WG RS-TRACE",*pEX_TraceString="WG RS-TRACE";
#endif/* WTI_ADM_MODE */


		status = WPX_FRMR_DEV_DRV_SONET_SDH_EnableAlarm(0,WPX_UFE_FRAMER_SDH_HP_PLM);
		status = WPX_FRMR_DEV_DRV_SONET_SDH_EnableAlarm(0,WPX_UFE_FRAMER_SDH_HP_UNEQ);

		if (flexmux_mode == WPX_UFE_FRAMER_DEVICE_MODE_SONET)
		{
			WP_U32 sts12, sts3, sts1;
			line_end_point.TransferType = WPX_UFE_FRAMER_WUFE_SONET_TYPE_STS1_SPE;

			if(line_port_mode == WT_FLEXMUX_LINE_PORT_OC3)
			{
				sts12 = 0;
				line_end_point.u.SONET.sts12 = sts12;
				for (sts3=0; sts3<max_line_ports; sts3+=line_increment)
				{
					WTI_FlexmuxLinePortConfig(0,
							sts3,/* Odd ports are used for protection */
							line_port_mode);
					WPX_FRMR_SONET_SDH_PORT_SEC_J0_SetMode(0, flexmux_id, sts3, WPX_UFE_FRAMER_PATH_TRACE_MODE_16_BYTE);
					status = WPX_FRMR_SONET_SDH_PORT_SEC_J0_SetTX(0, flexmux_id, sts3, pTX_TraceString);
					WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_SOH_J0_SetTX", status, __LINE__);

					status = WPX_FRMR_SONET_SDH_PORT_SEC_J0_SetEX(0, flexmux_id, sts3, pEX_TraceString);
					WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_SOH_J0_SetEX", status, __LINE__);

					line_end_point.u.SONET.sts3 = sts3;
					for(sts1=0; sts1<3; ++sts1)
					{
						line_end_point.u.SONET.sts1 = sts1;
						status = WPX_FRMR_SONET_SDH_CreateFacility(0, flexmux_id, &line_end_point);
						WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_CreateFacility (VC3/4)", status, __LINE__);

						/* set the transmitted and expected C2 values */
						status = WPX_FRMR_SONET_SDH_HO_PATH_C2_SetTX(0, flexmux_id, &line_end_point, TX_C2);
						WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_HO_PATH_C2_SetTX", status, __LINE__);
#if WTI_FLEXMUX_DBG_MODE
						U8 pTX_C2;
						status =WPX_FRMR_SONET_SDH_HO_PATH_C2_GetTX(flexmux_id,&line_end_point,&pTX_C2 );
						WTI_FlexmuxCheckStatus("WPX_FRMR_DEV_GetMode", status, __LINE__);
						if(pTX_C2!=TX_C2)
						{
							WTI_FlexmuxTerminateOnError("WPX_FRMR_SONET_SDH_HO_PATH_C2_GetTX != TX_C2\n", 0);
						}
#endif
						status = WPX_FRMR_SONET_SDH_HO_PATH_C2_SetEX(0, flexmux_id, &line_end_point, EX_C2);
						WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_HO_PATH_C2_SetEX", status, __LINE__);
#if WTI_FLEXMUX_DBG_MODE
						U8 pEX_C2;
						status =WPX_FRMR_SONET_SDH_HO_PATH_C2_GetTX(flexmux_id,&line_end_point,&pEX_C2 );
						WTI_FlexmuxCheckStatus("WPX_FRMR_DEV_GetMode", status, __LINE__);
						if(pTX_C2!=TX_C2)
						{
							WTI_FlexmuxTerminateOnError("WPX_FRMR_SONET_SDH_HO_PATH_C2_GetTX != EX_C2\n", 0);
						}
#endif


						WPX_FRMR_SONET_SDH_HO_PATH_J1_SetMode(0, flexmux_id, &line_end_point, WPX_UFE_FRAMER_PATH_TRACE_MODE_16_BYTE);
						status = WPX_FRMR_SONET_SDH_HO_PATH_J1_SetTX(0, flexmux_id, &line_end_point, TX_J1);
						WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_HO_PATH_J1_SetTX", status, __LINE__);
#if WTI_FLEXMUX_DBG_MODE
						WP_CHAR *pTX_J1;
						status =WPX_FRMR_SONET_SDH_HO_PATH_J1_GetTX(flexmux_id,&line_end_point,pTX_J1 );
						WTI_FlexmuxCheckStatus("WPX_FRMR_DEV_GetMode", status, __LINE__);
						if(pTX_J1!=TX_J1)
						{
							WTI_FlexmuxTerminateOnError("WPX_FRMR_SONET_SDH_HO_PATH_J1_GetTX != TX_J1\n", 0);
						}
#endif

						status = WPX_FRMR_SONET_SDH_HO_PATH_J1_SetEX(0, flexmux_id, &line_end_point, EX_J1);
						WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_HO_PATH_J1_SetEX", status, __LINE__);
#if WTI_FLEXMUX_DBG_MODE
						WP_CHAR *pEX_J1;
						status =WPX_FRMR_SONET_SDH_HO_PATH_J1_GetEX(flexmux_id,&line_end_point,pEX_J1 );
						WTI_FlexmuxCheckStatus("WPX_FRMR_DEV_GetMode", status, __LINE__);
						if(pEX_J1!=EX_J1)
						{
							WTI_FlexmuxTerminateOnError("WPX_FRMR_SONET_SDH_HO_PATH_J1_GetEX != EX_J1\n", 0);
						}
#endif
					}

				}
			}
			else if(line_port_mode == WT_FLEXMUX_LINE_PORT_OC12)
			{
				for (sts12=0; sts12<max_line_ports; sts12+=line_increment)
				{
					line_end_point.u.SONET.sts12 = sts12;
					WTI_FlexmuxLinePortConfig(0,
							sts12,/* Odd ports are used for protection */
							line_port_mode);
					WPX_FRMR_SONET_SDH_PORT_SEC_J0_SetMode(0, flexmux_id, sts12, WPX_UFE_FRAMER_PATH_TRACE_MODE_16_BYTE);
					status = WPX_FRMR_SONET_SDH_PORT_SEC_J0_SetTX(0, flexmux_id, sts12, pTX_TraceString);
					WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_SOH_J0_SetTX", status, __LINE__);

					status = WPX_FRMR_SONET_SDH_PORT_SEC_J0_SetEX(0, flexmux_id, sts12, pEX_TraceString);
					WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_SOH_J0_SetEX", status, __LINE__);

					for (sts3=0; sts3<4; ++sts3)
					{
						line_end_point.u.SONET.sts3 = sts3;
						for (sts1=0; sts1<3; ++sts1)
						{
							line_end_point.u.SONET.sts1 = sts1;
							status = WPX_FRMR_SONET_SDH_CreateFacility(0, flexmux_id, &line_end_point);
							WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_CreateFacility (VC2/4)", status, __LINE__);

							/* set the transmitted and expected C2 values */
							status = WPX_FRMR_SONET_SDH_HO_PATH_C2_SetTX(0, flexmux_id, &line_end_point, TX_C2);
							WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_HO_PATH_C2_SetTX", status, __LINE__);

#if WTI_FLEXMUX_DBG_MODE
							U8 pTX_C2;
							status =WPX_FRMR_SONET_SDH_HO_PATH_C2_GetTX(flexmux_id,&line_end_point,&pTX_C2 );
							WTI_FlexmuxCheckStatus("WPX_FRMR_DEV_GetMode", status, __LINE__);
							if(pTX_C2!=TX_C2)
							{
								WTI_FlexmuxTerminateOnError("WPX_FRMR_SONET_SDH_HO_PATH_C2_GetTX != TX_C2\n", 0);
							}
#endif
							status = WPX_FRMR_SONET_SDH_HO_PATH_C2_SetEX(0, flexmux_id, &line_end_point, EX_C2);
							WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_HO_PATH_C2_SetEX", status, __LINE__);
#if WTI_FLEXMUX_DBG_MODE
							U8 pEX_C2;
							status =WPX_FRMR_SONET_SDH_HO_PATH_C2_GetTX(flexmux_id,&line_end_point,&pEX_C2 );
							WTI_FlexmuxCheckStatus("WPX_FRMR_DEV_GetMode", status, __LINE__);
							if(pTX_C2!=TX_C2)
							{
								WTI_FlexmuxTerminateOnError("WPX_FRMR_SONET_SDH_HO_PATH_C2_GetTX != EX_C2\n", 0);
							}
#endif
							WPX_FRMR_SONET_SDH_HO_PATH_J1_SetMode(0, flexmux_id, &line_end_point, WPX_UFE_FRAMER_PATH_TRACE_MODE_16_BYTE);
							status = WPX_FRMR_SONET_SDH_HO_PATH_J1_SetTX(0, flexmux_id, &line_end_point, TX_J1);
							WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_HO_PATH_J1_SetTX", status, __LINE__);
#if WTI_FLEXMUX_DBG_MODE
							WP_CHAR *pTX_J1;
							status =WPX_FRMR_SONET_SDH_HO_PATH_J1_GetTX(flexmux_id,&line_end_point,pTX_J1 );
							WTI_FlexmuxCheckStatus("WPX_FRMR_DEV_GetMode", status, __LINE__);
							if(pTX_J1!=TX_J1)
							{
								WTI_FlexmuxTerminateOnError("WPX_FRMR_SONET_SDH_HO_PATH_J1_GetTX != TX_J1\n", 0);
							}
#endif

							status = WPX_FRMR_SONET_SDH_HO_PATH_J1_SetEX(0, flexmux_id, &line_end_point, EX_J1);
							WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_HO_PATH_J1_SetEX", status, __LINE__);
#if WTI_FLEXMUX_DBG_MODE
							WP_CHAR *pEX_J1;
							status =WPX_FRMR_SONET_SDH_HO_PATH_J1_GetEX(flexmux_id,&line_end_point,pEX_J1 );
							WTI_FlexmuxCheckStatus("WPX_FRMR_DEV_GetMode", status, __LINE__);
							if(pEX_J1!=EX_J1)
							{
								WTI_FlexmuxTerminateOnError("WPX_FRMR_SONET_SDH_HO_PATH_J1_GetEX != EX_J1\n", 0);
							}
#endif
						}

					}
				}
			}
		}
		else if (flexmux_mode == WPX_UFE_FRAMER_DEVICE_MODE_SDH)
		{
			WP_U32 stm4, stm1,stm0, num_for_vc3_or_vc4;

			line_end_point.TransferType = vc_3_4;
			/* for VC3 we need to configure 3 AU-3 containers
			   and for VC4 we need to configure only 1 AU-4 container  */
			if (vc_3_4 == WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC3)
			{
				num_for_vc3_or_vc4 = 3;
			}
			else
			{
				num_for_vc3_or_vc4 = 1;
			}

			if (line_port_mode == WT_FLEXMUX_LINE_PORT_STM1)
			{
				stm4 = 0;
				line_end_point.u.SDH.stm4 = stm4;
				for (stm1=0; stm1<max_line_ports; stm1+=line_increment)
				{
					line_end_point.u.SDH.stm1 = stm1;
					WTI_FlexmuxLinePortConfig(0,
							stm1,/* Odd ports are used for protection */
							line_port_mode);
					WPX_FRMR_SONET_SDH_PORT_SEC_J0_SetMode(0, flexmux_id, stm1, WPX_UFE_FRAMER_PATH_TRACE_MODE_16_BYTE);
					status = WPX_FRMR_SONET_SDH_PORT_SEC_J0_SetTX(0, flexmux_id, stm1, pTX_TraceString);
					WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_SOH_J0_SetTX", status, __LINE__);

					status = WPX_FRMR_SONET_SDH_PORT_SEC_J0_SetEX(0, flexmux_id, stm1, pEX_TraceString);
					WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_SOH_J0_SetEX", status, __LINE__);

					for(stm0=0; stm0<num_for_vc3_or_vc4; ++stm0)
					{
						line_end_point.u.SDH.stm0 = stm0;
						status = WPX_FRMR_SONET_SDH_CreateFacility(0, flexmux_id, &line_end_point);
						WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_CreateFacility (VC3/4)", status, __LINE__);

						/* set the transmitted and expected C2 values */
						status = WPX_FRMR_SONET_SDH_HO_PATH_C2_SetTX(0, flexmux_id, &line_end_point, TX_C2);
						WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_HO_PATH_C2_SetTX", status, __LINE__);
#if WTI_FLEXMUX_DBG_MODE
						U8 pTX_C2;
						status =WPX_FRMR_SONET_SDH_HO_PATH_C2_GetTX(flexmux_id,&line_end_point,&pTX_C2 );
						WTI_FlexmuxCheckStatus("WPX_FRMR_DEV_GetMode", status, __LINE__);
						if(pTX_C2!=TX_C2)
						{
							WTI_FlexmuxTerminateOnError("WPX_FRMR_SONET_SDH_HO_PATH_C2_GetTX != TX_C2\n", 0);
						}
#endif
						status = WPX_FRMR_SONET_SDH_HO_PATH_C2_SetEX(0, flexmux_id, &line_end_point, EX_C2);
						WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_HO_PATH_C2_SetEX", status, __LINE__);
#if WTI_FLEXMUX_DBG_MODE
						U8 pEX_C2;
						status =WPX_FRMR_SONET_SDH_HO_PATH_C2_GetTX(flexmux_id,&line_end_point,&pEX_C2 );
						WTI_FlexmuxCheckStatus("WPX_FRMR_DEV_GetMode", status, __LINE__);
						if(pTX_C2!=TX_C2)
						{
							WTI_FlexmuxTerminateOnError("WPX_FRMR_SONET_SDH_HO_PATH_C2_GetTX != EX_C2\n", 0);
						}
#endif
						WPX_FRMR_SONET_SDH_HO_PATH_J1_SetMode(0, flexmux_id, &line_end_point, WPX_UFE_FRAMER_PATH_TRACE_MODE_16_BYTE);
						status = WPX_FRMR_SONET_SDH_HO_PATH_J1_SetTX(0, flexmux_id, &line_end_point, TX_J1);
						WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_HO_PATH_J1_SetTX", status, __LINE__);
#if WTI_FLEXMUX_DBG_MODE
						WP_CHAR *pTX_J1;
						status =WPX_FRMR_SONET_SDH_HO_PATH_J1_GetTX(flexmux_id,&line_end_point,pTX_J1 );
						WTI_FlexmuxCheckStatus("WPX_FRMR_DEV_GetMode", status, __LINE__);
						if(pTX_J1!=TX_J1)
						{
							WTI_FlexmuxTerminateOnError("WPX_FRMR_SONET_SDH_HO_PATH_J1_GetTX != TX_J1\n", 0);
						}
#endif

						status = WPX_FRMR_SONET_SDH_HO_PATH_J1_SetEX(0, flexmux_id, &line_end_point, EX_J1);
						WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_HO_PATH_J1_SetEX", status, __LINE__);
#if WTI_FLEXMUX_DBG_MODE
						WP_CHAR *pEX_J1;
						status =WPX_FRMR_SONET_SDH_HO_PATH_J1_GetEX(flexmux_id,&line_end_point,pEX_J1 );
						WTI_FlexmuxCheckStatus("WPX_FRMR_DEV_GetMode", status, __LINE__);
						if(pEX_J1!=EX_J1)
						{
							WTI_FlexmuxTerminateOnError("WPX_FRMR_SONET_SDH_HO_PATH_J1_GetEX != EX_J1\n", 0);
						}
#endif
					}
				}
			}
			else if (line_port_mode == WT_FLEXMUX_LINE_PORT_STM4)
			{
				for (stm4=0; stm4<max_line_ports; stm4+=line_increment)
				{
					line_end_point.u.SDH.stm4 = stm4;
					WTI_FlexmuxLinePortConfig(0,
							stm4,/* Odd ports are used for protection */
							line_port_mode);
					WPX_FRMR_SONET_SDH_PORT_SEC_J0_SetMode(0, flexmux_id, stm4, WPX_UFE_FRAMER_PATH_TRACE_MODE_16_BYTE);
					status = WPX_FRMR_SONET_SDH_PORT_SEC_J0_SetTX(0, flexmux_id, stm4, pTX_TraceString);
					WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_SOH_J0_SetTX", status, __LINE__);

					status = WPX_FRMR_SONET_SDH_PORT_SEC_J0_SetEX(0, flexmux_id, stm4, pEX_TraceString);
					WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_SOH_J0_SetEX", status, __LINE__);

					for (stm1=0; stm1<4; ++stm1)
					{
						line_end_point.u.SDH.stm1 = stm1;
						for(stm0=0; stm0<num_for_vc3_or_vc4; ++stm0)
						{
							line_end_point.u.SDH.stm0 = stm0;
							status = WPX_FRMR_SONET_SDH_CreateFacility(0, flexmux_id, &line_end_point);
							WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_CreateFacility (VC3/4)", status, __LINE__);

							/* set the transmitted and expected C2 values */
							status = WPX_FRMR_SONET_SDH_HO_PATH_C2_SetTX(0, flexmux_id, &line_end_point, TX_C2);
							WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_HO_PATH_C2_SetTX", status, __LINE__);
#if WTI_FLEXMUX_DBG_MODE
							U8 pTX_C2;
							status =WPX_FRMR_SONET_SDH_HO_PATH_C2_GetTX(flexmux_id,&line_end_point,&pTX_C2 );
							WTI_FlexmuxCheckStatus("WPX_FRMR_DEV_GetMode", status, __LINE__);
							if(pTX_C2!=TX_C2)
							{
								WTI_FlexmuxTerminateOnError("WPX_FRMR_SONET_SDH_HO_PATH_C2_GetTX != TX_C2\n", 0);
							}
#endif
							status = WPX_FRMR_SONET_SDH_HO_PATH_C2_SetEX(0, flexmux_id, &line_end_point, EX_C2);
							WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_HO_PATH_C2_SetEX", status, __LINE__);
#if WTI_FLEXMUX_DBG_MODE
							U8 pEX_C2;
							status =WPX_FRMR_SONET_SDH_HO_PATH_C2_GetTX(flexmux_id,&line_end_point,&pEX_C2 );
							WTI_FlexmuxCheckStatus("WPX_FRMR_DEV_GetMode", status, __LINE__);
							if(pTX_C2!=TX_C2)
							{
								WTI_FlexmuxTerminateOnError("WPX_FRMR_SONET_SDH_HO_PATH_C2_GetTX != EX_C2\n", 0);
							}
#endif
							WPX_FRMR_SONET_SDH_HO_PATH_J1_SetMode(0, flexmux_id, &line_end_point, WPX_UFE_FRAMER_PATH_TRACE_MODE_16_BYTE);
							status = WPX_FRMR_SONET_SDH_HO_PATH_J1_SetTX(0, flexmux_id, &line_end_point, TX_J1);
							WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_HO_PATH_J1_SetTX", status, __LINE__);
#if WTI_FLEXMUX_DBG_MODE
							WP_CHAR *pTX_J1;
							status =WPX_FRMR_SONET_SDH_HO_PATH_J1_GetTX(flexmux_id,&line_end_point,pTX_J1 );
							WTI_FlexmuxCheckStatus("WPX_FRMR_DEV_GetMode", status, __LINE__);
							if(pTX_J1!=TX_J1)
							{
								WTI_FlexmuxTerminateOnError("WPX_FRMR_SONET_SDH_HO_PATH_J1_GetTX != TX_J1\n", 0);
							}
#endif

							status = WPX_FRMR_SONET_SDH_HO_PATH_J1_SetEX(0, flexmux_id, &line_end_point, EX_J1);
							WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_HO_PATH_J1_SetEX", status, __LINE__);
#if WTI_FLEXMUX_DBG_MODE
							WP_CHAR *pEX_J1;
							status =WPX_FRMR_SONET_SDH_HO_PATH_J1_GetEX(flexmux_id,&line_end_point,pEX_J1 );
							WTI_FlexmuxCheckStatus("WPX_FRMR_DEV_GetMode", status, __LINE__);
							if(pEX_J1!=EX_J1)
							{
								WTI_FlexmuxTerminateOnError("WPX_FRMR_SONET_SDH_HO_PATH_J1_GetEX != EX_J1\n", 0);
							}
#endif
						}
					}
				}
			}
		}
	}
	else
	{
		WTI_FlexmuxTerminateOnError("Unsupported Build personality", 0);
	}

	/* Set which SONET\SDH port Rx clock drives the RECV_LINE_CLK (which drive the External PLL diff 0/1 output) */
	/* WPX_FRMR_DEV_DIAG_DRV_RecClkSet(U32 TransactionIdentifier, U8 iDevice, U8 OutputSelector, U8 iLinePort)   */

	status = WPX_FRMR_DEV_DIAG_DRV_RecClkSet(0, 0, 0 ,0);
	WTI_FlexmuxCheckStatus("WPX_FRMR_DEV_DIAG_DRV_RecClkSet", status, __LINE__);


	status = WPX_FRMR_DEV_DIAG_DRV_RecClkSet(0, 0, 1 ,0);
	WTI_FlexmuxCheckStatus("WPX_FRMR_DEV_DIAG_DRV_RecClkSet", status, __LINE__);


#if WTI_FLEXMUX_DBG_MODE
	U8 piLinePort;
	status = WPX_FRMR_DEV_DIAG_DRV_RecClkGet(flexmux_id,&piLinePort );
	WTI_FlexmuxCheckStatus("WPX_FRMR_DEV_GetMode", status, __LINE__);
	if(piLinePort!=0)
	{
		WTI_FlexmuxTerminateOnError("WPX_FRMR_DEV_DIAG_DRV_RecClkGet != 0\n", 0);
	}
#endif

	return 0;
}

/****************************************************************************************************************************
 * Function name: WTI_FlexmuxLinePortConfig()
 * Description    :
 *
 *
 * Input  params: none
 * Output params: 
 * Return val   : none
 *****************************************************************************************************************************/
WP_S32 WTI_FlexmuxLinePortConfig(WP_U8 flexmux_id,
		WP_U8 line_port_id,
		WTI_flexmux_line_port_mode line_port_cfg)
{
	WP_U8 status =0;
		U8 pPortRate;
	if (flexmux_id > WTI_FLEXMUX_NUM_DEVICES)
		WTI_FlexmuxTerminateOnError("flexmux_id is out of bounds !", 0);

	if (flexmux_reg[flexmux_id].is_initialized != WP_TRUE)
		WTI_FlexmuxTerminateOnError("Flexmux device is not initialized!", 0);

	if (line_port_id >= WTI_MAX_LINE_PORTS_PER_FLEXMUX_DEVICE)
		WTI_FlexmuxTerminateOnError("Invalid line_port_id!", 0);

	if ((flexmux_reg[flexmux_id].flexmux_mode == WPX_UFE_FRAMER_DEVICE_MODE_SONET) &&
			(line_port_cfg != WT_FLEXMUX_LINE_PORT_OC3 && line_port_cfg != WT_FLEXMUX_LINE_PORT_OC12))
		WTI_FlexmuxTerminateOnError("Invalid line_port_cfg!", 0);

	if ((flexmux_reg[flexmux_id].flexmux_mode == WPX_UFE_FRAMER_DEVICE_MODE_SDH) &&
			(line_port_cfg != WT_FLEXMUX_LINE_PORT_STM1 && line_port_cfg != WT_FLEXMUX_LINE_PORT_STM4))
		WTI_FlexmuxTerminateOnError("Invalid line_port_cfg!", 0);

#undef WTI_FLEXMUX_DBG_MODE
#define WTI_FLEXMUX_DBG_MODE 1
	if (line_port_cfg == WT_FLEXMUX_LINE_PORT_OC3)
	{
		status = WPX_FRMR_SONET_SDH_PORT_SetRate(0, flexmux_id, line_port_id, WPX_UFE_FRAMER_LINE_PORT_RATE_OC3);
		WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_PORT_SetRate", status, __LINE__);
#if WTI_FLEXMUX_DBG_MODE
		{
			U8 pPortRate;
			status =WPX_FRMR_SONET_SDH_PORT_GetRate(flexmux_id,line_port_id,&pPortRate );
			WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_PORT_GetRate", status, __LINE__);
			if(pPortRate!=WPX_UFE_FRAMER_LINE_PORT_RATE_OC3)
			{
				WTI_FlexmuxTerminateOnError("WPX_FRMR_SONET_SDH_PORT_GetRate != WPX_UFE_FRAMER_LINE_PORT_RATE_STS3C\n", 0);
			}
		}
#endif
	}
	else if (line_port_cfg == WT_FLEXMUX_LINE_PORT_OC12)
	{
		status = WPX_FRMR_SONET_SDH_PORT_SetRate(0, flexmux_id, line_port_id, WPX_UFE_FRAMER_LINE_PORT_RATE_OC12);
		WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_PORT_SetRate", status, __LINE__);
#if WTI_FLEXMUX_DBG_MODE

		status =WPX_FRMR_SONET_SDH_PORT_GetRate(flexmux_id,line_port_id,&pPortRate );
		WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_PORT_GetRate", status, __LINE__);
		if(pPortRate!=WPX_UFE_FRAMER_LINE_PORT_RATE_OC12)
		{
			WTI_FlexmuxTerminateOnError("WPX_FRMR_SONET_SDH_PORT_GetRate != WPX_UFE_FRAMER_LINE_PORT_RATE_STS12C\n", 0);
		}
#endif

	}
	else if (line_port_cfg == WT_FLEXMUX_LINE_PORT_STM1)
	{
		status = WPX_FRMR_SONET_SDH_PORT_SetRate(0, flexmux_id, line_port_id, WPX_UFE_FRAMER_LINE_PORT_RATE_STM1);
		WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_PORT_SetRate", status, __LINE__);
#if WTI_FLEXMUX_DBG_MODE

		status =WPX_FRMR_SONET_SDH_PORT_GetRate(flexmux_id,line_port_id,&pPortRate );
		WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_PORT_GetRate", status, __LINE__);
		if(pPortRate!=WPX_UFE_FRAMER_LINE_PORT_RATE_STM1)
		{
			WTI_FlexmuxTerminateOnError("WPX_FRMR_SONET_SDH_PORT_GetRate != WPX_UFE_FRAMER_LINE_PORT_RATE_STM1\n", 0);
		}
#endif
	}
	else if (line_port_cfg == WT_FLEXMUX_LINE_PORT_STM4)
	{
		status = WPX_FRMR_SONET_SDH_PORT_SetRate(0, flexmux_id, line_port_id, WPX_UFE_FRAMER_LINE_PORT_RATE_STM4);
		WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_PORT_SetRate", status, __LINE__);
#if WTI_FLEXMUX_DBG_MODE

		status =WPX_FRMR_SONET_SDH_PORT_GetRate(flexmux_id,line_port_id,&pPortRate );
		WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_PORT_GetRate", status, __LINE__);
		if(pPortRate!=WPX_UFE_FRAMER_LINE_PORT_RATE_STM4)
		{
			WTI_FlexmuxTerminateOnError("WPX_FRMR_SONET_SDH_PORT_GetRate != WPX_UFE_FRAMER_LINE_PORT_RATE_STM4\n", 0);
		}
#endif
	}
#undef WTI_FLEXMUX_DBG_MODE
#define WTI_FLEXMUX_DBG_MODE 0

	/* save line port rate in the registry */
	flexmux_reg[flexmux_id].line_ports_reg[line_port_id].line_port_mode = line_port_cfg;

	return 0;
}
/****************************************************************************************************************************
 * Function name: WTI_FlexmuxDccEnable()
 * Description    :
 *
 *
 * Input  params: none
 * Output params: 
 * Return val   : none
 *****************************************************************************************************************************/

WP_S32 WTI_FlexmuxDccEnable(WP_U8 flexmux_id,
		WP_U8 line_port_id,
		WP_U32 dcc_mode)
{
	WP_U8 status =0;

	/* This function Enables a DCC channel on a port */
	if (flexmux_id > WTI_FLEXMUX_NUM_DEVICES)
		WTI_FlexmuxTerminateOnError("flexmux_id is out of bounds !", 0);

	/* Enable the DCC*/
	status = WPX_FRMR_SONET_SDH_PORT_DCC_Enable(0 ,flexmux_id, line_port_id, dcc_mode);
	WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_PORT_DCC_Enable", status, __LINE__);

	return 0;
}
/****************************************************************************************************************************
 * Function name: WTI_FlexmuxDccDisable()
 * Description    :
 *
 *
 * Input  params: none
 * Output params: 
 * Return val   : none
 *****************************************************************************************************************************/

WP_S32 WTI_FlexmuxDccDisable(WP_U8 flexmux_id,
		WP_U8 line_port_id,
		WP_U32 dcc_mode)
{
	WP_U8 status =0;

	/* This function Disables a DCC channel on a port */
	if (flexmux_id > WTI_FLEXMUX_NUM_DEVICES)
		WTI_FlexmuxTerminateOnError("flexmux_id is out of bounds !", 0);

	/* Disable the DCC*/
	status = WPX_FRMR_SONET_SDH_PORT_DCC_Disable(0 ,flexmux_id, line_port_id, dcc_mode);
	WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_PORT_DCC_Disable", status, __LINE__);

	return 0;
}
/****************************************************************************************************************************
 * Function name: WTI_FlexmuxConnectionCreate()
 * Description    :
 *
 *
 * Input  params: none
 * Output params: 
 * Return val   : none
 *****************************************************************************************************************************/

WP_S32 WTI_FlexmuxConnectionCreate(WP_U8 flexmux_id,
		WP_U8 line_port_id,
		WP_U32 client_port_id,
		WTI_flexmux_connection_type type,
		void *ufe_line_params)
{
	/* This function creates the client and line port facilities and connects them */
	WP_U8 status =0,tx_clk_rec_enable=0;
	WP_U32 line_timing=0;
	WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE line_end_point;

	if (flexmux_id > WTI_FLEXMUX_NUM_DEVICES)
		WTI_FlexmuxTerminateOnError("flexmux_id is out of bounds !", 0);

	/* create client port facility */
	if (type == WT_FLEXMUX_CONNECTION_TYPE_E1_UNFRAMED)
	{
		status = WPX_FRMR_SOCKET_PDH_CreateFacility(0, flexmux_id, client_port_id, WPX_UFE_FRAMER_CLIENT_E1);
		WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_CreateFacility", status, __LINE__);

		status = WPX_FRMR_SOCKET_PDH_SetFraming(0, flexmux_id, client_port_id, WPX_UFE_FRAMER_CLIENT_UNFRAMED);
		WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_SetFraming", status, __LINE__);
#if WTI_FLEXMUX_DBG_MODE
		U8 pFraming;
		status =WPX_FRMR_SOCKET_PDH_GetFraming(flexmux_id,client_port_id,&pFraming );
		WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_GetFraming", status, __LINE__);
		if(pFraming!=WPX_UFE_FRAMER_CLIENT_UNFRAMED)
		{
			WTI_FlexmuxTerminateOnError("WPX_FRMR_SOCKET_PDH_GetFraming != WPX_UFE_FRAMER_CLIENT_UNFRAMED\n", 0);
		}
#endif
		status = WPX_FRMR_SOCKET_PDH_SetMapping(0, flexmux_id, client_port_id, WPX_UFE_FRAMER_CLIENT_MAPPING_BIT_ASYNC);
		WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_SetMapping", status, __LINE__);
#if WTI_FLEXMUX_DBG_MODE
		U8 pMapping;
		status =WPX_FRMR_SOCKET_PDH_GetMapping(flexmux_id,client_port_id,&pMapping );
		WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_GetMapping", status, __LINE__);
		if(pMapping!=WPX_UFE_FRAMER_CLIENT_MAPPING_BIT_ASYNC)
		{
			WTI_FlexmuxTerminateOnError("WPX_FRMR_SOCKET_PDH_GetMapping != WPX_UFE_FRAMER_CLIENT_MAPPING_BIT_ASYNC\n", 0);
		}
#endif

	}
	else if (type == WT_FLEXMUX_CONNECTION_TYPE_E1_FRAMED)
	{
		status = WPX_FRMR_SOCKET_PDH_CreateFacility(0, flexmux_id, client_port_id, WPX_UFE_FRAMER_CLIENT_E1);
		WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_CreateFacility", status, __LINE__);

		status = WPX_FRMR_SOCKET_PDH_SetFraming(0, flexmux_id, client_port_id, WPX_UFE_FRAMER_CLIENT_PCM31CRC);
		WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_SetFraming", status, __LINE__);

#if WTI_FLEXMUX_DBG_MODE
		U8 pFraming;
		status =WPX_FRMR_SOCKET_PDH_GetFraming(flexmux_id,client_port_id,&pFraming );
		WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_GetFraming", status, __LINE__);
		if(pFraming!=WPX_UFE_FRAMER_CLIENT_PCM31CRC)
		{
			WTI_FlexmuxTerminateOnError("WPX_FRMR_SOCKET_PDH_GetFraming != WPX_UFE_FRAMER_CLIENT_PCM31CRC\n", 0);
		}
#endif

		status = WPX_FRMR_SOCKET_PDH_SetMapping(0, flexmux_id, client_port_id, WPX_UFE_FRAMER_CLIENT_MAPPING_BIT_ASYNC);
		WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_SetMapping", status, __LINE__);

#if WTI_FLEXMUX_DBG_MODE
		U8 pMapping;
		status =WPX_FRMR_SOCKET_PDH_GetMapping(flexmux_id,client_port_id,&pMapping );
		WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_GetMapping", status, __LINE__);
		if(pMapping!=WPX_UFE_FRAMER_CLIENT_MAPPING_BIT_ASYNC)
		{
			WTI_FlexmuxTerminateOnError("WPX_FRMR_SOCKET_PDH_GetMapping != WPX_UFE_FRAMER_CLIENT_MAPPING_BIT_ASYNC\n", 0);
		}
#endif

		status = WPX_FRMR_SOCKET_PDH_DisableCAS(0, flexmux_id, client_port_id);
		WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_DisableCAS", status, __LINE__);

#if WTI_FLEXMUX_DBG_MODE
		U8 pCAS_IsEnabled;
		status =WPX_FRMR_SOCKET_PDH_Get_CAS_State(flexmux_id,client_port_id,&pCAS_IsEnabled );
		WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_Get_CAS_State", status, __LINE__);
		if(pCAS_IsEnabled!=0)
		{
			WTI_FlexmuxTerminateOnError("WPX_FRMR_SOCKET_PDH_Get_CAS_State != 0\n", 0);
		}
#endif

	}
	else if (type == WT_FLEXMUX_CONNECTION_TYPE_E1_FRAMED_CAS)
	{
		status = WPX_FRMR_SOCKET_PDH_CreateFacility(0, flexmux_id, client_port_id, WPX_UFE_FRAMER_CLIENT_E1);
		WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_CreateFacility", status, __LINE__);

		status = WPX_FRMR_SOCKET_PDH_SetFraming(0, flexmux_id, client_port_id, WPX_UFE_FRAMER_CLIENT_PCM31CRC);
		WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_SetFraming", status, __LINE__);

#if WTI_FLEXMUX_DBG_MODE
		U8 pFraming;
		status =WPX_FRMR_SOCKET_PDH_GetFraming(flexmux_id,client_port_id,&pFraming );
		WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_GetFraming", status, __LINE__);
		if(pFraming!=WPX_UFE_FRAMER_CLIENT_PCM31CRC)
		{
			WTI_FlexmuxTerminateOnError("WPX_FRMR_SOCKET_PDH_GetFraming != WPX_UFE_FRAMER_CLIENT_PCM31CRC\n", 0);
		}
#endif

		status = WPX_FRMR_SOCKET_PDH_SetMapping(0, flexmux_id, client_port_id, WPX_UFE_FRAMER_CLIENT_MAPPING_BIT_ASYNC);
		WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_SetMapping", status, __LINE__);

#if WTI_FLEXMUX_DBG_MODE
		U8 pMapping;
		status =WPX_FRMR_SOCKET_PDH_GetMapping(flexmux_id,client_port_id,&pMapping );
		WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_GetMapping", status, __LINE__);
		if(pMapping!=WPX_UFE_FRAMER_CLIENT_MAPPING_BIT_ASYNC)
		{
			WTI_FlexmuxTerminateOnError("WPX_FRMR_SOCKET_PDH_GetMapping != WPX_UFE_FRAMER_CLIENT_MAPPING_BIT_ASYNC\n", 0);
		}
#endif


		status = WPX_FRMR_SOCKET_PDH_EnableCAS(0, flexmux_id, client_port_id);
		WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_DisableCAS", status, __LINE__);

#if WTI_FLEXMUX_DBG_MODE
		U8 pCAS_IsEnabled;
		status =WPX_FRMR_SOCKET_PDH_Get_CAS_State(flexmux_id,client_port_id,&pCAS_IsEnabled );
		WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_Get_CAS_State", status, __LINE__);
		if(pCAS_IsEnabled!=1)
		{
			WTI_FlexmuxTerminateOnError("WPX_FRMR_SOCKET_PDH_Get_CAS_State != 1\n", 0);
		}
#endif

	}
	else if (type == WT_FLEXMUX_CONNECTION_TYPE_T1_UNFRAMED)
	{
		status = WPX_FRMR_SOCKET_PDH_CreateFacility(0, flexmux_id, client_port_id, WPX_UFE_FRAMER_CLIENT_T1);
		WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_CreateFacility", status, __LINE__);

		status = WPX_FRMR_SOCKET_PDH_SetFraming(0, flexmux_id, client_port_id, WPX_UFE_FRAMER_CLIENT_UNFRAMED);
		WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_SetFraming", status, __LINE__);

#if WTI_FLEXMUX_DBG_MODE
		U8 pFraming;
		status =WPX_FRMR_SOCKET_PDH_GetFraming(flexmux_id,client_port_id,&pFraming );
		WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_GetFraming", status, __LINE__);
		if(pFraming!=WPX_UFE_FRAMER_CLIENT_UNFRAMED)
		{
			WTI_FlexmuxTerminateOnError("WPX_FRMR_SOCKET_PDH_GetFraming != WPX_UFE_FRAMER_CLIENT_UNFRAMED\n", 0);
		}
#endif

		status = WPX_FRMR_SOCKET_PDH_SetMapping(0, flexmux_id, client_port_id, WPX_UFE_FRAMER_CLIENT_MAPPING_BIT_ASYNC);
		WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_SetMapping", status, __LINE__);

#if WTI_FLEXMUX_DBG_MODE
		U8 pMapping;
		status =WPX_FRMR_SOCKET_PDH_GetMapping(flexmux_id,client_port_id,&pMapping );
		WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_GetMapping", status, __LINE__);
		if(pMapping!=WPX_UFE_FRAMER_CLIENT_MAPPING_BIT_ASYNC)
		{
			WTI_FlexmuxTerminateOnError("WPX_FRMR_SOCKET_PDH_GetMapping != WPX_UFE_FRAMER_CLIENT_MAPPING_BIT_ASYNC\n", 0);
		}
#endif

	}
	else if (type == WT_FLEXMUX_CONNECTION_TYPE_T1_FRAMED)
	{
		status = WPX_FRMR_SOCKET_PDH_CreateFacility(0, flexmux_id, client_port_id, WPX_UFE_FRAMER_CLIENT_T1);
		WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_CreateFacility", status, __LINE__);

		status = WPX_FRMR_SOCKET_PDH_SetFraming(0, flexmux_id, client_port_id, WPX_UFE_FRAMER_CLIENT_FRAMING_ESF);
		WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_SetFraming", status, __LINE__);

#if WTI_FLEXMUX_DBG_MODE
		U8 pFraming;
		status =WPX_FRMR_SOCKET_PDH_GetFraming(flexmux_id,client_port_id,&pFraming );
		WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_GetFraming", status, __LINE__);
		if(pFraming!=WPX_UFE_FRAMER_CLIENT_FRAMING_SF_D4)
		{
			WTI_FlexmuxTerminateOnError("WPX_FRMR_SOCKET_PDH_GetFraming != WPX_UFE_FRAMER_CLIENT_FRAMING_SF_D4\n", 0);
		}
#endif

		status = WPX_FRMR_SOCKET_PDH_SetMapping(0, flexmux_id, client_port_id, WPX_UFE_FRAMER_CLIENT_MAPPING_BIT_ASYNC);
		WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_SetMapping", status, __LINE__);

#if WTI_FLEXMUX_DBG_MODE
		U8 pMapping;
		status =WPX_FRMR_SOCKET_PDH_GetMapping(flexmux_id,client_port_id,&pMapping );
		WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_GetMapping", status, __LINE__);
		if(pMapping!=WPX_UFE_FRAMER_CLIENT_MAPPING_BIT_ASYNC)
		{
			WTI_FlexmuxTerminateOnError("WPX_FRMR_SOCKET_PDH_GetMapping != WPX_UFE_FRAMER_CLIENT_MAPPING_BIT_ASYNC\n", 0);
		}
#endif

		status = WPX_FRMR_SOCKET_PDH_DisableCAS(0, flexmux_id, client_port_id);
		WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_DisableCAS", status, __LINE__);

#if WTI_FLEXMUX_DBG_MODE
		U8 pCAS_IsEnabled;
		status =WPX_FRMR_SOCKET_PDH_Get_CAS_State(flexmux_id,client_port_id,&pCAS_IsEnabled );
		WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_Get_CAS_State", status, __LINE__);
		if(pCAS_IsEnabled!=0)
		{
			WTI_FlexmuxTerminateOnError("WPX_FRMR_SOCKET_PDH_Get_CAS_State != 0\n", 0);
		}
#endif
	}
	else if (type == WT_FLEXMUX_CONNECTION_TYPE_T1_FRAMED_CAS)
	{
		status = WPX_FRMR_SOCKET_PDH_CreateFacility(0, flexmux_id, client_port_id, WPX_UFE_FRAMER_CLIENT_T1);
		WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_CreateFacility", status, __LINE__);

		status = WPX_FRMR_SOCKET_PDH_SetFraming(0, flexmux_id, client_port_id, WPX_UFE_FRAMER_CLIENT_FRAMING_ESF);
		WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_SetFraming", status, __LINE__);

#if WTI_FLEXMUX_DBG_MODE
		U8 pFraming;
		status =WPX_FRMR_SOCKET_PDH_GetFraming(flexmux_id,client_port_id,&pFraming );
		WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_GetFraming", status, __LINE__);
		if(pFraming!=WPX_UFE_FRAMER_CLIENT_FRAMING_ESF)
		{
			WTI_FlexmuxTerminateOnError("WPX_FRMR_SOCKET_PDH_GetFraming != WPX_UFE_FRAMER_CLIENT_FRAMING_ESF\n", 0);
		}
#endif

		status = WPX_FRMR_SOCKET_PDH_SetMapping(0, flexmux_id, client_port_id, WPX_UFE_FRAMER_CLIENT_MAPPING_BIT_ASYNC);
		WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_SetMapping", status, __LINE__);

#if WTI_FLEXMUX_DBG_MODE
		U8 pMapping;
		status =WPX_FRMR_SOCKET_PDH_GetMapping(flexmux_id,client_port_id,&pMapping );
		WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_GetMapping", status, __LINE__);
		if(pMapping!=WPX_UFE_FRAMER_CLIENT_MAPPING_BIT_ASYNC)
		{
			WTI_FlexmuxTerminateOnError("WPX_FRMR_SOCKET_PDH_GetMapping != WPX_UFE_FRAMER_CLIENT_MAPPING_BIT_ASYNC\n", 0);
		}
#endif

		status = WPX_FRMR_SOCKET_PDH_EnableCAS(0, flexmux_id, client_port_id);
		WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_EnableCAS", status, __LINE__);

#if WTI_FLEXMUX_DBG_MODE
		U8 pCAS_IsEnabled;
		status =WPX_FRMR_SOCKET_PDH_Get_CAS_State(flexmux_id,client_port_id,&pCAS_IsEnabled );
		WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_Get_CAS_State", status, __LINE__);
		if(pCAS_IsEnabled!=1)
		{
			WTI_FlexmuxTerminateOnError("WPX_FRMR_SOCKET_PDH_Get_CAS_State != 1\n", 0);
		}
#endif
	}
	else
	{
		WTI_FlexmuxTerminateOnError("Connection type is not supported !", 0);
	}

	/* configure the line end point in SONET or SDH mode */
	if (flexmux_reg[flexmux_id].flexmux_mode == WPX_UFE_FRAMER_DEVICE_MODE_SONET)
	{
		WT_ufe_line_sonet_params *sonet_line_params = (WT_ufe_line_sonet_params*) ufe_line_params;
		U8 TX_V5_SignalLabel=0x2, EX_V5_SignalLabel=0x2;
		WP_CHAR *TX_J2_TraceString = "WG LP-TRACE",*EX_J2_TraceString = "WG LP-TRACE";

		line_end_point.u.SONET.sts12 = sonet_line_params->sts12;
		line_end_point.u.SONET.sts3 = sonet_line_params->sts3;
		line_end_point.u.SONET.sts1 = sonet_line_params->sts1;
		line_end_point.u.SONET.vt_group = sonet_line_params->vt_group;
		line_end_point.u.SONET.vt = sonet_line_params->vt;

#if WTI_FLEXMUX_DBG_MODE
		printf("connecting client_port_id %d to line_port_id %d\n",client_port_id,line_port_id);
		printf("sts12 %d sts3 %d sts1 %d vt_group %d vt %d\n", sonet_line_params->sts12,
				sonet_line_params->sts3,
				sonet_line_params->sts1,
				sonet_line_params->vt_group,
				sonet_line_params->vt);
#endif

		if (sonet_line_params->transfer_type == WUFE_SONET_TYPE_E1)
			line_end_point.TransferType = WPX_UFE_FRAMER_WUFE_SONET_TYPE_VT2;
		else if (sonet_line_params->transfer_type == WUFE_SONET_TYPE_T1)
			line_end_point.TransferType = WPX_UFE_FRAMER_WUFE_SONET_TYPE_VT1_5;
		else
			WTI_FlexmuxTerminateOnError("Invalid line transfer type", 0);


		status = WPX_FRMR_SONET_SDH_CreateFacility(0, flexmux_id, &line_end_point);
		WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_CreateFacility (Vt1.5 or Vt2)", status, __LINE__);

		/* set the transmitted and expected V5 values */
		status = WPX_FRMR_SONET_SDH_LO_PATH_V5_SigLabel_SetTX(0, flexmux_id, &line_end_point, TX_V5_SignalLabel);
		WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_LO_PATH_V5_SigLabel_SetTX", status, __LINE__);

#if WTI_FLEXMUX_DBG_MODE
		U8 pTX_V5_SignalLabel;
		status =WPX_FRMR_SONET_SDH_LO_PATH_V5_SigLabel_GetTX(flexmux_id,&line_end_point,&pTX_V5_SignalLabel );
		WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_LO_PATH_V5_SigLabel_GetTX", status, __LINE__);
		if(pTX_V5_SignalLabel!=TX_V5_SignalLabel)
		{
			WTI_FlexmuxTerminateOnError("WPX_FRMR_SONET_SDH_LO_PATH_V5_SigLabel_GetTX != TX_V5_SignalLabel\n", 0);
		}
#endif

		status = WPX_FRMR_SONET_SDH_LO_PATH_V5_SigLabel_SetEX(0, flexmux_id, &line_end_point, EX_V5_SignalLabel);
		WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_LO_PATH_V5_SigLabel_SetEX", status, __LINE__);

#if WTI_FLEXMUX_DBG_MODE
		U8 pEX_V5_SignalLabel;
		status =WPX_FRMR_SONET_SDH_LO_PATH_V5_SigLabel_GetEX(flexmux_id,&line_end_point,&pEX_V5_SignalLabel );
		WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_LO_PATH_V5_SigLabel_GetEX", status, __LINE__);
		if(pEX_V5_SignalLabel!=EX_V5_SignalLabel)
		{
			WTI_FlexmuxTerminateOnError("WPX_FRMR_SONET_SDH_LO_PATH_V5_SigLabel_GetEX != EX_V5_SignalLabel\n", 0);
		}
#endif


		status = WPX_FRMR_SONET_SDH_LO_PATH_J2_SetTX(0, flexmux_id, &line_end_point, TX_J2_TraceString);
		WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_LO_PATH_J2_SetTX", status, __LINE__);

#if WTI_FLEXMUX_DBG_MODE
		WP_CHAR *pTX_J2_TraceString;
		status =WPX_FRMR_SONET_SDH_LO_PATH_J2_GetTX(flexmux_id,&line_end_point,pTX_J2_TraceString );
		WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_LO_PATH_J2_SetTX", status, __LINE__);
		if(*pTX_J2_TraceString!=*TX_J2_TraceString)
		{
			WTI_FlexmuxTerminateOnError("WPX_FRMR_SONET_SDH_LO_PATH_J2_GetTX != TX_J2_TraceString\n", 0);
		}
#endif

		status = WPX_FRMR_SONET_SDH_LO_PATH_J2_SetEX(0, flexmux_id, &line_end_point, EX_J2_TraceString);
		WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_LO_PATH_J2_SetEX", status, __LINE__);

#if WTI_FLEXMUX_DBG_MODE
		WP_CHAR *pEX_J2_TraceString;
		status =WPX_FRMR_SONET_SDH_LO_PATH_J2_GetEX(flexmux_id,&line_end_point,pEX_J2_TraceString );
		WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_LO_PATH_J2_GetEX", status, __LINE__);
		if(*pEX_J2_TraceString!=*EX_J2_TraceString)
		{
			WTI_FlexmuxTerminateOnError("WPX_FRMR_SONET_SDH_LO_PATH_J2_GetEX != EX_J2_TraceString\n", 0);
		}
#endif

		if (sonet_line_params->clock_rec_line_params != NULL)
		{
			tx_clk_rec_enable = sonet_line_params->clock_rec_line_params->tx_clk_rec_enable;
		}
	}
	else if (flexmux_reg[flexmux_id].flexmux_mode == WPX_UFE_FRAMER_DEVICE_MODE_SDH)
	{
		WT_ufe_line_sdh_params *sdh_line_params = (WT_ufe_line_sdh_params*) ufe_line_params;
		U8 TX_V5_SignalLabel=0x2, EX_V5_SignalLabel=0x2;
		WP_CHAR *TX_J2_TraceString = "WG LP-TRACE",*EX_J2_TraceString = "WG LP-TRACE";

		/* create the VC3/4 facility */
		line_end_point.u.SDH.stm4 = sdh_line_params->stm4;
		line_end_point.u.SDH.stm1 = sdh_line_params->stm1;
		line_end_point.u.SDH.stm0 = sdh_line_params->stm0;
		line_end_point.u.SDH.tug2 = sdh_line_params->tug2;
		line_end_point.u.SDH.tu = sdh_line_params->tu;

#if WTI_FLEXMUX_DBG_MODE
		printf("connecting client_port_id %d to line_port_id %d\n",client_port_id,line_port_id);
		printf("stm4 %d stm1 %d stm0 %d tug2 %d tu %d\n", sdh_line_params->stm4,sdh_line_params->stm1,sdh_line_params->stm0,sdh_line_params->tug2,sdh_line_params->tu);
#endif
		if (sdh_line_params->transfer_type == WUFE_SDH_TYPE_E1)
		{
			line_end_point.TransferType = WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC12;
		}

		else if (sdh_line_params->transfer_type == WUFE_SDH_TYPE_T1)
		{
			line_end_point.TransferType = WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC11;
		}
		else
			WTI_FlexmuxTerminateOnError("Invalid line transfer type", 0);

		if (sdh_line_params->clock_rec_line_params != NULL)
		{
			tx_clk_rec_enable = sdh_line_params->clock_rec_line_params->tx_clk_rec_enable;
		}


		/* create the line port VC11\VC12 facility */
		status = WPX_FRMR_SONET_SDH_CreateFacility(0, flexmux_id, &line_end_point);
		WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_CreateFacility (VC11 or VC12)", status, __LINE__);

		/* set the transmitted and expected V5 values */
		status = WPX_FRMR_SONET_SDH_LO_PATH_V5_SigLabel_SetTX(0, flexmux_id, &line_end_point, TX_V5_SignalLabel);
		WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_LO_PATH_V5_SigLabel_SetTX", status, __LINE__);

#if WTI_FLEXMUX_DBG_MODE
		U8 pTX_V5_SignalLabel;
		status =WPX_FRMR_SONET_SDH_LO_PATH_V5_SigLabel_GetTX(flexmux_id,&line_end_point,&pTX_V5_SignalLabel );
		WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_LO_PATH_V5_SigLabel_GetTX", status, __LINE__);
		if(pTX_V5_SignalLabel!=TX_V5_SignalLabel)
		{
			WTI_FlexmuxTerminateOnError("WPX_FRMR_SONET_SDH_LO_PATH_V5_SigLabel_GetTX != TX_V5_SignalLabel\n", 0);
		}
#endif

		status = WPX_FRMR_SONET_SDH_LO_PATH_V5_SigLabel_SetEX(0, flexmux_id, &line_end_point, EX_V5_SignalLabel);
		WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_LO_PATH_V5_SigLabel_SetEX", status, __LINE__);

#if WTI_FLEXMUX_DBG_MODE
		U8 pEX_V5_SignalLabel;
		status =WPX_FRMR_SONET_SDH_LO_PATH_V5_SigLabel_GetEX(flexmux_id,&line_end_point,&pEX_V5_SignalLabel );
		WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_LO_PATH_V5_SigLabel_GetEX", status, __LINE__);
		if(pEX_V5_SignalLabel!=EX_V5_SignalLabel)
		{
			WTI_FlexmuxTerminateOnError("WPX_FRMR_SONET_SDH_LO_PATH_V5_SigLabel_GetEX != EX_V5_SignalLabel\n", 0);
		}
#endif
		status = WPX_FRMR_SONET_SDH_LO_PATH_J2_SetTX(0, flexmux_id, &line_end_point, TX_J2_TraceString);
		WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_LO_PATH_J2_SetTX", status, __LINE__);

#if WTI_FLEXMUX_DBG_MODE
		WP_CHAR *pTX_J2_TraceString;
		status =WPX_FRMR_SONET_SDH_LO_PATH_J2_GetTX(flexmux_id,&line_end_point,pTX_J2_TraceString );
		WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_LO_PATH_J2_SetTX", status, __LINE__);
		if(*pTX_J2_TraceString!=*TX_J2_TraceString)
		{
			WTI_FlexmuxTerminateOnError("WPX_FRMR_SONET_SDH_LO_PATH_J2_GetTX != TX_J2_TraceString\n", 0);
		}
#endif

		status = WPX_FRMR_SONET_SDH_LO_PATH_J2_SetEX(0, flexmux_id, &line_end_point, EX_J2_TraceString);
		WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_LO_PATH_J2_SetEX", status, __LINE__);

#if WTI_FLEXMUX_DBG_MODE
		WP_CHAR *pEX_J2_TraceString;
		status =WPX_FRMR_SONET_SDH_LO_PATH_J2_GetEX(flexmux_id,&line_end_point,pEX_J2_TraceString );
		WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_LO_PATH_J2_GetEX", status, __LINE__);
		if(*pEX_J2_TraceString!=*EX_J2_TraceString)
		{
			WTI_FlexmuxTerminateOnError("WPX_FRMR_SONET_SDH_LO_PATH_J2_GetEX != EX_J2_TraceString\n", 0);
		}
#endif
	}

	/* create the connection between the client port and the line port */
	status = WPX_FRMR_CONNECTIONS_AddDropLineToSocketCreate(0, flexmux_id, &line_end_point, client_port_id);
	WTI_FlexmuxCheckStatus("WPX_FRMR_CONNECTIONS_AddDropLineToSocketCreate", status, __LINE__);

	/* Set the line timing mode */
	/* The PDH timing mode can be one of the follwoing :
	   WPX_UFE_FRAMER_TRANSMIT_TIMING_RX_PATH  - PDH looptiming
	   WPX_UFE_FRAMER_TRANSMIT_TIMING_INPUT_REFERENCE_CLOCK - Common transmit (CT) clock
	   WPX_UFE_FRAMER_TRANSMIT_TIMING_SONET_TIMING - System timing from the SONET/SDH Tx clock
	   WPX_UFE_FRAMER_TRANSMIT_TIMING_WINPATH - ACR/DCR - Winpath control the PDH timing
	 */
	if (tx_clk_rec_enable)
	{

#if(WTI_CESOP_REGRESSION_TEST)
		/* When regression mode is set the framer is configured to SONET timing in order to make sure the PDH Tx clock stable */
		/* All other configuration in the UFE and WP are set to work with CR                                                  */
		line_timing = WPX_UFE_FRAMER_TRANSMIT_TIMING_SONET_TIMING;
#if WTI_FLEXMUX_DBG_MODE
		printf("WPX_UFE_FRAMER_TRANSMIT_TIMING_SONET_TIMING\n");
#endif
#else /* NOT WTI_CESOP_REGRESSION_TEST */
		/* Framer uses justification commands from Winpath via UFE */
		line_timing = WPX_UFE_FRAMER_TRANSMIT_TIMING_WINPATH;
#endif

#if WTI_FLEXMUX_DBG_MODE
		printf("WPX_UFE_FRAMER_TRANSMIT_TIMING_WINPATH\n");
#endif
	}
	else
	{
#if(WTI_CESOP_REGRESSION_TEST)
		/* Working in OC12/48 loopback the timing should be CT or SDH/SONET clock */
		line_timing = WPX_UFE_FRAMER_TRANSMIT_TIMING_SONET_TIMING;
#if WTI_FLEXMUX_DBG_MODE
		printf("WPX_UFE_FRAMER_TRANSMIT_TIMING_SONET_TIMING\n");
#endif
#else
		/* CR is not enable and working with TDM external tester - use PDH loop timing */
		line_timing =  WPX_UFE_FRAMER_TRANSMIT_TIMING_RX_PATH;    /* Loop timing */

#if WTI_FLEXMUX_DBG_MODE
		printf("WPX_UFE_FRAMER_TRANSMIT_TIMING_RX_PATH\n");
#endif
#endif

	}

	/* Clock rec Rx timestamps */
	if (tx_clk_rec_enable)
	{
		status = WPX_FRMR_SOCKET_PDH_EnableTimeStamp(0, flexmux_id, client_port_id,WTI_FLEXMUX_CLOCK_REC_GAIN_SLOW);
		WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_EnableTimeStamp", status, __LINE__);
	}

	/* Set the PDH Tx line timing  */
	status = WPX_FRMR_SOCKET_PDH_SetClkRec(0, flexmux_id, client_port_id, line_timing);
	WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_SetClkRec", status, __LINE__);



	/* Set line or system loopback */
	status = WTI_FlexmuxEnableLoopback(flexmux_id, line_port_id, client_port_id, WT_FLEXMUX_NONE_LOOPBACK);
	WTI_FlexmuxCheckStatus("WTI_FlexmuxEnableLoopback", status, __LINE__);
#if WTI_FLEXMUX_DBG_MODE
	status = WTI_FlexmuxGetLoopbackState(flexmux_id, line_port_id, client_port_id, WT_FLEXMUX_NONE_LOOPBACK);
	WTI_FlexmuxCheckStatus("WTI_FlexmuxGetLoopbackState", status, __LINE__);
#endif

	return 0;
}
/****************************************************************************************************************************
 * Function name: WTI_FlexmuxConnectionDelete()
 * Description    :
 *
 *
 * Input  params: none
 * Output params: 
 * Return val   : none
 *****************************************************************************************************************************/

WP_S32 WTI_FlexmuxConnectionDelete(WP_U8 flexmux_id,
		WP_U32 client_port_id,
		void *ufe_line_params)
{
	/* This function deletes an existing connection and the associated client and line port facilities */
	WP_U8 status =0;
	WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE line_end_point = {0};

	if (flexmux_id > WTI_FLEXMUX_NUM_DEVICES)
		WTI_FlexmuxTerminateOnError("flexmux_id is out of bounds !", 0);

	/* configure the line end point in SONET or SDH mode */
	if (flexmux_reg[flexmux_id].flexmux_mode == WPX_UFE_FRAMER_DEVICE_MODE_SONET)
	{
		WT_ufe_line_sonet_params *sonet_line_params = (WT_ufe_line_sonet_params*) ufe_line_params;

		if (sonet_line_params->transfer_type == WUFE_SONET_TYPE_E1)
			line_end_point.TransferType = WPX_UFE_FRAMER_WUFE_SONET_TYPE_VT2;
		else if (sonet_line_params->transfer_type == WUFE_SONET_TYPE_T1)
			line_end_point.TransferType = WPX_UFE_FRAMER_WUFE_SONET_TYPE_VT1_5;
		else
			WTI_FlexmuxTerminateOnError("Invalid line transfer type", 0);

		line_end_point.u.SONET.sts12 = sonet_line_params->sts12;
		line_end_point.u.SONET.sts3 = sonet_line_params->sts3;
		line_end_point.u.SONET.sts1 = sonet_line_params->sts1;
		line_end_point.u.SONET.vt_group = sonet_line_params->vt_group;
		line_end_point.u.SONET.vt = sonet_line_params->vt;
	}
	else if (flexmux_reg[flexmux_id].flexmux_mode == WPX_UFE_FRAMER_DEVICE_MODE_SDH)
	{
		WT_ufe_line_sdh_params *sdh_line_params = (WT_ufe_line_sdh_params*) ufe_line_params;

		if (sdh_line_params->transfer_type == WUFE_SDH_TYPE_E1)
			line_end_point.TransferType = WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC12;
		else if (sdh_line_params->transfer_type == WUFE_SDH_TYPE_T1)
			line_end_point.TransferType = WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC11;
		else
			WTI_FlexmuxTerminateOnError("Invalid line transfer type", 0);

		line_end_point.u.SDH.stm4 = sdh_line_params->stm4;
		line_end_point.u.SDH.stm1 = sdh_line_params->stm1;
		line_end_point.u.SDH.stm0 = sdh_line_params->stm0;
		line_end_point.u.SDH.tug2 = sdh_line_params->tug2;
		line_end_point.u.SDH.tu = sdh_line_params->tu;
	}

	/* delete the connection between the client port and the line port */
	status = WPX_FRMR_CONNECTIONS_AddDropLineToSocketDelete(0, flexmux_id, &line_end_point, client_port_id);
	WTI_FlexmuxCheckStatus("WPX_FRMR_CONNECTIONS_AddDropLineToSocketDelete", status, __LINE__);

	/* delete the line port facility */
	status = WPX_FRMR_SONET_SDH_DeleteFacility(0, flexmux_id, &line_end_point);
	WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_DeleteFacility", status, __LINE__);

	/* delete the client port facility */
	status = WPX_FRMR_SOCKET_PDH_DeleteFacility(0, flexmux_id, client_port_id);
	WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_DeleteFacility", status, __LINE__);

	return 0;
}
/****************************************************************************************************************************
 * Function name: WTI_FlexmuxClientPortIndexGet()
 * Description    :
 *
 *
 * Input  params: none
 * Output params: 
 * Return val   : none
 *****************************************************************************************************************************/

WP_S32 WTI_FlexmuxClientPortIndexGet(WP_U32 ufe_line_index, WUFE_line_transf_type transfer_type)
{
	switch (transfer_type)
	{
		case (WUFE_SONET_TYPE_E1):
		case (WUFE_SDH_TYPE_E1):
			{
				WP_U32 spe_num = (ufe_line_index / 28);
				WP_U32 tug2_num = ((ufe_line_index % 28) / 3);
				WP_U32 offset_within_tug2 = ((ufe_line_index % 28) % 3);

				/* In E1 mode skip indices 3,7,11.. due to TUG restriction */
				return ((spe_num*28) + (tug2_num*4) + offset_within_tug2);
			}
		case (WUFE_SONET_TYPE_T1):
		case (WUFE_SDH_TYPE_T1):
			{
				return ufe_line_index;
			}
		default:
			printf("Unsupported transfer_type %d\n", transfer_type);
			exit(-1);
	}
	return 0;
}
/****************************************************************************************************************************
 * Function name: WTI_FlexmuxLinePortIndexGet()
 * Description    :
 *
 *
 * Input  params: none
 * Output params: 
 * Return val   : none
 *****************************************************************************************************************************/

WP_S32 WTI_FlexmuxLinePortIndexGet(WP_U32 ufe_line_index, WP_U32 flexmux_id)
{
	WP_U32 result = -1;
	if (flexmux_reg[flexmux_id].flexmux_build_personality != WPX_UFE_FRAMER_BUILD_PERSONALITY_4_PLUS_4_OC3_OC12 &&
			flexmux_reg[flexmux_id].flexmux_build_personality != WPX_UFE_FRAMER_BUILD_PERSONALITY_1_PLUS_1_OC3_OC12 &&
			flexmux_reg[flexmux_id].flexmux_build_personality != WPX_UFE_FRAMER_BUILD_PERSONALITY_2_PLUS_2_OC3      &&
			flexmux_reg[flexmux_id].flexmux_build_personality != WPX_UFE_FRAMER_BUILD_PERSONALITY_4_PLUS_4_OC3)
		WTI_FlexmuxTerminateOnError("Invalid flexmux_build_personality !", 0);

	/* In V6 protection is used and even ports are the working ports while odd
	   ports are the protected ports */
	if (flexmux_reg[flexmux_id].flexmux_build_personality == WPX_UFE_FRAMER_BUILD_PERSONALITY_4_PLUS_4_OC3_OC12)
	{
		result = (ufe_line_index / 336);
		/* currently only 4XOC12 ports are supported */
		if (result > 3)
		{
			result = -1;
		}
		else
		{
			result = result * 2; /* Multipication by 2 in order to skip the odd protection lines*/
		}
	}
	/* In S6 protection is not used - port #0 is the only working line port */
	else if ((flexmux_reg[flexmux_id].flexmux_build_personality == WPX_UFE_FRAMER_BUILD_PERSONALITY_1_PLUS_1_OC3_OC12) ||
			(flexmux_reg[flexmux_id].flexmux_build_personality == WPX_UFE_FRAMER_BUILD_PERSONALITY_2_PLUS_2_OC3)      ||
			(flexmux_reg[flexmux_id].flexmux_build_personality == WPX_UFE_FRAMER_BUILD_PERSONALITY_4_PLUS_4_OC3))
	{
		/* In case a single OC12 port is used, then line port index is always 0*/
		if (flexmux_reg[flexmux_id].flexmux_hw_config == WT_FLEXMUX_UFE_412_1_PORT_OC12)
		{
			result = 0;
		}
		/* In case 4 OC3 ports are used, then line port index changes between 0-3*/
		else if ((flexmux_reg[flexmux_id].flexmux_hw_config == WT_FLEXMUX_UFE_412_4_PORTS_OC3) ||
				(flexmux_reg[flexmux_id].flexmux_hw_config == WT_FLEXMUX_UFE_412_2_PORTS_OC3)||
				flexmux_reg[flexmux_id].flexmux_hw_config == WT_FLEXMUX_UFE_412_4_PORTS_OC3_CARD_PROTECTION_MASTER ||
				flexmux_reg[flexmux_id].flexmux_hw_config == WT_FLEXMUX_UFE_412_4_PORTS_OC3_CARD_PROTECTION_SLAVE)
		{
			result = (ufe_line_index / 84);
			if (result > 3)
			{
				result = -1;
			}
		}
	}

	return result;
}
/****************************************************************************************************************************
 * Function name: WTI_FlexmuxDisplayVersionAndConfig()
 * Description    :
 *
 *
 * Input  params: none
 * Output params: 
 * Return val   : none
 *****************************************************************************************************************************/

void WTI_FlexmuxDisplayVersionAndConfig(WP_U32 flexmux_id)
{
	WP_U8 status;
	WP_U8 build[128];

	status = WPX_FRMR_DEV_GetBuildPersonality(flexmux_id, build);
	WTI_FlexmuxCheckStatus("WPX_FRMR_DEV_GetBuildPersonality", status, __LINE__);
	printf("Flexmux Build Personality: %d\n", (WP_U32) build);
}
/****************************************************************************************************************************
 * Function name: WTI_FlexmuxEnableLoopback()
 * Description    :
 *
 *
 * Input  params: none
 * Output params: 
 * Return val   : none
 *****************************************************************************************************************************/

WP_U8 WTI_FlexmuxEnableLoopback(U8 flexmux_id, WP_U32 line_port_id, WP_U32 client_port_id, WTI_flexmux_loopback_mode loopback_type)
{
	WP_U8 status = WPX_UFE_FRAMER_OK;

	switch (loopback_type)
	{
		case WT_FLEXMUX_NONE_LOOPBACK:
			break;
		case WT_FLEXMUX_SHALLOW_LINE_LOOPBACK:
			{
				status = (WP_U8) WPX_FRMR_SONET_SDH_PORT_DIAG_LpbkEnableShallowLine(0, flexmux_id, line_port_id);
				WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_PORT_DIAG_LpbkEnableShallowLine", status, __LINE__);

				break;
			}
		case WT_FLEXMUX_DEEP_LINE_LOOPBACK:
			{
				status = (WP_U8) WPX_FRMR_SOCKET_PDH_DIAG_LpbkEnableDeepLine(0, flexmux_id, client_port_id);
				WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_DIAG_LpbkEnableDeepLine", status, __LINE__);

				break;
			}
		case WT_FLEXMUX_SHALLOW_SYSTEM_LOOPBACK:
			{
				status = (WP_U8) WPX_FRMR_SOCKET_PDH_DIAG_LpbkEnableShallowSystem(0, flexmux_id, client_port_id);
				WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_DIAG_LpbkEnableShallowSystem", status, __LINE__);

				break;
			}
		case WT_FLEXMUX_DEEP_SYSTEM_LOOPBACK:
			{
				status = (WP_U8) WPX_FRMR_SONET_SDH_PORT_DIAG_LpbkEnableDeepSystem(0, flexmux_id, line_port_id);
				WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_PORT_DIAG_LpbkEnableDeepSystem", status, __LINE__);

				break;
			}
		default:
			{
				printf("Unsupported loopback_type %d\n", loopback_type);
				exit(-1);
			}
	}

	return status;
}
/****************************************************************************************************************************
 * Function name: WTI_FlexmuxDisableLoopback()
 * Description    :
 *
 *
 * Input  params: none
 * Output params: 
 * Return val   : none
 *****************************************************************************************************************************/

WP_U8 WTI_FlexmuxDisableLoopback(U8 flexmux_id, WP_U32 line_port_id, WP_U32 client_port_id, WTI_flexmux_loopback_mode loopback_type)
{
	WP_U8 status = WPX_UFE_FRAMER_OK;

	switch (loopback_type)
	{
		case WT_FLEXMUX_NONE_LOOPBACK:
			break;
		case WT_FLEXMUX_SHALLOW_LINE_LOOPBACK:
			{
				status = (WP_U8) WPX_FRMR_SONET_SDH_PORT_DIAG_LpbkDisableShallowLine(0, flexmux_id, line_port_id);
				WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_PORT_DIAG_LpbkDisableShallowLine", status, __LINE__);

				break;
			}
		case WT_FLEXMUX_DEEP_LINE_LOOPBACK:
			{
				status = (WP_U8) WPX_FRMR_SOCKET_PDH_DIAG_LpbkDisableDeepLine(0, flexmux_id, client_port_id);
				WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_DIAG_LpbkDisableDeepLine", status, __LINE__);

				break;
			}
		case WT_FLEXMUX_SHALLOW_SYSTEM_LOOPBACK:
			{
				status = (WP_U8) WPX_FRMR_SOCKET_PDH_DIAG_LpbkDisableShallowSystem(0,flexmux_id, client_port_id);
				WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_DIAG_LpbkDisableShallowSystem", status, __LINE__);

				break;
			}
		case WT_FLEXMUX_DEEP_SYSTEM_LOOPBACK:
			{
				status = (WP_U8) WPX_FRMR_SONET_SDH_PORT_DIAG_LpbkDisableDeepSystem(0, flexmux_id, line_port_id);
				WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_PORT_DIAG_LpbkDisableDeepSystem", status, __LINE__);

				break;
			}
		default:
			{
				printf("Unsupported loopback_type %d\n", loopback_type);
				exit(-1);
			}
	}
	return status;
}
/****************************************************************************************************************************
 * Function name: WTI_FlexmuxGetLoopbackState()
 * Description    :
 *
 *
 * Input  params: none
 * Output params: 
 * Return val   : none
 *****************************************************************************************************************************/

WP_U8 WTI_FlexmuxGetLoopbackState(U8 flexmux_id, WP_U32 line_port_id, WP_U32 client_port_id, WTI_flexmux_loopback_mode loopback_type)
{
	WP_U8 status = WPX_UFE_FRAMER_OK, loopback_enabled = 0;

	switch (loopback_type)
	{
		case WT_FLEXMUX_NONE_LOOPBACK:
			break;
		case WT_FLEXMUX_SHALLOW_LINE_LOOPBACK:
			{
				status = (WP_U8) WPX_FRMR_SONET_SDH_PORT_DIAG_LpbkGetShallowLineState(0, line_port_id, &loopback_enabled );
				WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_PORT_DIAG_LpbkGetShallowLineState", status, __LINE__);
				if (loopback_enabled)
				{
					printf("SHALLOW_LINE");
				}

				break;
			}
		case WT_FLEXMUX_DEEP_LINE_LOOPBACK:
			{
				status = (WP_U8) WPX_FRMR_SOCKET_PDH_DIAG_LpbkGetDeepLineState(0,client_port_id, &loopback_enabled);
				WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_DIAG_LpbkGetDeepLineState", status, __LINE__);
				if (loopback_enabled)
				{
					printf("DEEP_LINE");
				}

				break;
			}
		case WT_FLEXMUX_SHALLOW_SYSTEM_LOOPBACK:
			{
				status = (WP_U8) WPX_FRMR_SOCKET_PDH_DIAG_LpbkGetShallowSystemState(0,client_port_id,&loopback_enabled);
				WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_DIAG_LpbkGetShallowSystemState", status, __LINE__);
				if (loopback_enabled)
				{
					printf("SHALLOW_SYSTEM");
				}

				break;
			}
		case WT_FLEXMUX_DEEP_SYSTEM_LOOPBACK:
			{
				status = (WP_U8) WPX_FRMR_SONET_SDH_PORT_DIAG_LpbkGetDeepSystemState(0, line_port_id, &loopback_enabled);
				WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_PORT_DIAG_LpbkGetDeepSystemState", status, __LINE__);
				if (loopback_enabled)
				{
					printf("DEEP_SYSTEM");
				}

				break;
			}
		default:
			{
				printf("Unsupported loopback_type %d\n", loopback_type);
				exit(-1);
			}
	}
	if (loopback_enabled)
	{
		printf(" Loopback is enabled\n");
	}
	else
	{
		printf(" Loopback is not enabled\n");
	}

	return status;
}

// WP_U8 WTI_FlexmuxCheckPrbsResult(U8 flexmux_id, WP_U32 client_port_id, WUFE_un_framed framing_mode)
// {
//    WP_U8 prbs_state = WPX_UFE_FRAMER_PRBS_STATE_IDLE;
//    WP_U8 ufe_framer_status;
//    WP_U8 status = WPX_UFE_FRAMER_OK;

//    /* enable the Egress PRBS generator for the client port*/
//    status = WPX_FRMR_SOCKET_PDH_DIAG_EgressGenPrbs_Set(0, flexmux_id, client_port_id, WPX_UFE_FRAMER_PRBS_23);
//    WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_DIAG_EgressGenPrbs_Set", status, __LINE__);


//    if(framing_mode == WUFE_UNFRAMED)
//    {
//       /* enable the Egress PRBS detector for the client port*/
//       status = WPX_FRMR_SOCKET_PDH_DIAG_EgressTermPrbsSet(0, flexmux_id, client_port_id, WPX_UFE_FRAMER_PRBS_23);
//       WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_DIAG_EgressTermPrbsSet", status, __LINE__);
//    }
//    else
//    {
//       /* enable the Ingress PRBS detector for the client port*/
//       status = WPX_FRMR_SOCKET_PDH_DIAG_IngressTermPrbsSet(0, flexmux_id, client_port_id, WPX_UFE_FRAMER_PRBS_23);
//       WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_DIAG_IngressTermPrbsSet", status, __LINE__);
//    }

//    WPL_Delay(1000000);

//    if(framing_mode == WUFE_UNFRAMED)
//    {
//       ufe_framer_status = WPX_FRMR_STATUS_SOCKET_PDH_DIAG_EgressPrbsGetState(0, client_port_id , &prbs_state);
//       WTI_FlexmuxCheckStatus("WPX_FRMR_STATUS_SOCKET_PDH_DIAG_EgressPrbsGetState", ufe_framer_status, __LINE__);
//       //prbs_result[client_port_id] = prbs_state;
//    }
//    else
//    {
//       ufe_framer_status = WPX_FRMR_STATUS_SOCKET_PDH_DIAG_IngressPrbsGetState(0, client_port_id , &prbs_state);
//       WTI_FlexmuxCheckStatus("WPX_FRMR_STATUS_SOCKET_PDH_DIAG_IngressPrbsGetState", ufe_framer_status, __LINE__);
//       //prbs_result[client_port_id] = prbs_state;
//    }


//    /* disable the Egress PRBS generator for the client port*/
//    status = WPX_FRMR_SOCKET_PDH_DIAG_EgressGenPrbs_Set(0, flexmux_id, client_port_id, WPX_UFE_FRAMER_PRBS_DISABLED);
//    WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_DIAG_EgressGenPrbs_Set", status, __LINE__);

//    if(framing_mode == WUFE_UNFRAMED)
//    {
//       /* disable the Egress PRBS detector for the client port*/
//       status = WPX_FRMR_SOCKET_PDH_DIAG_EgressTermPrbsSet(0, flexmux_id, client_port_id, WPX_UFE_FRAMER_PRBS_DISABLED);
//       WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_DIAG_EgressTermPrbsSet", status, __LINE__);
//    }
//    else
//    {
//      /* disable the Ingress PRBS detector for the client port*/
//       status = WPX_FRMR_SOCKET_PDH_DIAG_IngressTermPrbsSet(0, flexmux_id, client_port_id, WPX_UFE_FRAMER_PRBS_DISABLED);
//       WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_DIAG_IngressTermPrbsSet", status, __LINE__);
//    }


//    if (prbs_result[client_port_id] == WPX_UFE_FRAMER_PRBS_STATE_CLEAR)
//    {
//       return 0;
//    }
//    else
//    {
//       switch (prbs_result[client_port_id])
//       {
//          case WPX_UFE_FRAMER_PRBS_STATE_IDLE: printf("PRBS is disabled for client_port_id %d\n",client_port_id);
//             break;
//          case WPX_UFE_FRAMER_PRBS_STATE_ERRORS: printf("PRBS data check failed for client_port_id %d\n",client_port_id);
//             break;
//          case WPX_UFE_FRAMER_PRBS_STATE_INACTIVE: printf("PRBS is inactive for client_port_id %d\n",client_port_id);
//             break;
//       }
//       return 1;
//    }
// }
/****************************************************************************************************************************
 * Function name: WTI_FlexmuxForceA()
 * Description    :
 *
 *
 * Input  params: none
 * Output params: 
 * Return val   : none
 *****************************************************************************************************************************/

void WTI_FlexmuxForceA(WP_U32 line_port_id)
{
	WP_U8 status = WPX_UFE_FRAMER_OK;
#if DEBUG
	WP_U8 force_side = 2;
	WP_time before ,after;
	WP_U32 sec, usec;

	WPL_TimestampGet(&before);
#endif
	status = WPX_FRMR_SONET_SDH_PORT_ForceA(0, 0, line_port_id);
	WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_PORT_ForceA", status, __LINE__);
#if DEBUG
	status =WPX_FRMR_SONET_SDH_PORT_GetForceState(0, line_port_id, &force_side);
	WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_PORT_GetForceState", status, __LINE__);
	WPL_TimestampGet(&after);
	sec = (after.sec)-(before.sec);
	usec = (after.usec)-(before.usec);
	if (usec<0)
	{
		sec = sec - 1;
		usec = usec + 1000000;
	}
	printf("In line %d forcing port A took %d.%06d seconds\n",line_port_id,sec,usec);
	if (status == WPX_UFE_FRAMER_OK)
	{
		if (force_side == WPX_UFE_FRAMER_PROTECTION_FORCE_A)
		{
			printf("From ForceA: In line %d traffic is received from port A\n",line_port_id);
		}
		else if (force_side == WPX_UFE_FRAMER_PROTECTION_FORCE_B)
		{
			printf("From ForceA: In line %d traffic is received from port B\n",line_port_id);
		}
	}
#endif
}
/****************************************************************************************************************************
 * Function name: WTI_FlexmuxForceB()
 * Description    :
 *
 *
 * Input  params: none
 * Output params: 
 * Return val   : none
 *****************************************************************************************************************************/

void WTI_FlexmuxForceB(WP_U32 line_port_id)
{
	WP_U8 status = WPX_UFE_FRAMER_OK;
#if DEBUG
	WP_U8 force_side = 2;
	WP_time before ,after;
	WP_U32 sec, usec;

	WPL_TimestampGet(&before);
#endif
	status =WPX_FRMR_SONET_SDH_PORT_ForceB(0, 0, line_port_id);
	WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_PORT_ForceB", status, __LINE__);
#if DEBUG
	status =WPX_FRMR_SONET_SDH_PORT_GetForceState(0, line_port_id, &force_side);
	WTI_FlexmuxCheckStatus("WPX_FRMR_SONET_SDH_PORT_GetForceState", status, __LINE__);
	WPL_TimestampGet(&after);
	sec = (after.sec)-(before.sec);
	usec = (after.usec)-(before.usec);
	if (usec<0)
	{
		sec = sec - 1;
		usec = usec + 1000000;
	}
	printf("In line %d forcing port B took %d.%06d seconds\n",line_port_id,sec,usec);
	if (status == WPX_UFE_FRAMER_OK)
	{
		if (force_side == WPX_UFE_FRAMER_PROTECTION_FORCE_A)
		{
			printf("From ForceB: In line %d traffic is received from port A\n",line_port_id);
		}
		else if (force_side == WPX_UFE_FRAMER_PROTECTION_FORCE_B)
		{
			printf("From ForceB: In line %d traffic is received from port B\n",line_port_id);
		}
	}
#endif
}

/****************************************************************************************************************************
 * Function name: WTI_FlexmuxPDHOutputClockSet()
 * Description    :
 *
 *
 * Input  params: none
 * Output params: 
 * Return val   : none
 *****************************************************************************************************************************/
void WTI_FlexmuxPDHOutputClockSet(WP_U8 output_select, WP_U32 line_index,WUFE_line_transf_type transfer_type,WP_U32 clock_rate)
{
	WP_U32 client_port_id=0;
	WP_U8 status = WPX_UFE_FRAMER_OK;

	client_port_id = WTI_FlexmuxClientPortIndexGet(line_index,transfer_type);
	/* Set the Recover PDH clock line (Rx) to RECV_PDH_LINE_CLK_0   - the call must be after the SocketClientPDH was created */
	/* WPX_FRMR_SOCKET_PDH_DIAG_DRV_RecClkSet(U32 TransactionIdentifier,U8 iDevice,U8 OutputSelector,U32 iSocketClientPDH,U32 ClockRate)*/
	status = WPX_FRMR_SOCKET_PDH_DIAG_DRV_RecClkSet(0,0,output_select,client_port_id,clock_rate); /* or _1544 */
	WTI_FlexmuxCheckStatus("WPX_FRMR_SOCKET_PDH_DIAG_DRV_RecClkSet", status, __LINE__);

#if WTI_FLEXMUX_DBG_MODE
	printf("Setting PDH Rx clock %d to line %d at %dKhz\n",output_select,client_port_id,clock_rate);
#endif
}
