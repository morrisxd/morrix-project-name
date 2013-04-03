/*****************************************************************************
 * (C) Copyright PMC - WIN division (Wintegra) 2011.  All rights reserved.
 * PMC-Sierra (WINTEGRA) CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of PMC-Sierra (Wintegra).
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/****************************************************************************
 *
 * Example: flexmux_util.c
 * Demonstrates the use of Omiino Flexmux API.
 *
 ****************************************************************************/

#include <string.h>
#include "wp_wddi.h"

#define WTI_UFE412_REVA 1

int ho_transfer_type_in_use;
int lo_transfer_type_in_use;

WP_U8 prbs_result[336];

void WTI_FlexmuxTerminateOnError (WP_CHAR * pFatalErrorMessage,
                                  WP_U8 error_code)
{
    printf ("Test Abort: %s Line:%d Error code %d\n", pFatalErrorMessage,
            __LINE__, (WP_U32) error_code);
    exit (-1);
}

void WTI_FlexmuxCheckStatus (WP_CHAR * Message, WP_U8 status, WP_U32 line)
{
    extern char *OMIINO_FRAMER_ErrorCodeToTxt (WP_U32 ErrorCode);

    if (status)
    {
        printf
            ("Test Abort: function %s in Line:%d returned Error code %s\n",
             Message, line,
             OMIINO_FRAMER_ErrorCodeToTxt ((WP_U32) status));
        exit (-1);
    }
    else
    {
#if WTI_FLEXMUX_DBG_MODE
        printf ("Status returned from %s is WPX_UFE_FRAMER_OK\n", Message);
        return;
#endif
    }
}

void WTI_allocate_pm_storage_areas (void);
WP_THREAD_ID mailbox_thread_id = 0;
WP_THREAD_ID alarm_and_pm_thread_id = 0;

WP_S32 WTI_FlexmuxInit (WP_U32 upi_index, WP_U8 flexmux_id,
                        WTI_flexmux_physical_conf flexmux_hw_config,
                        WP_U8 flexmux_mode,
                        WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4,
                        WTI_Flexmux_global_setup * flexmux_cfg)
{
    WUFE_status status;
    WP_U8 max_line_ports = 0, line_increment = 0;
    WP_U8 wti_flexmux_build_personality = 0;
    WTI_flexmux_line_port_mode line_port_mode = 0;

    if (flexmux_id > WTI_FLEXMUX_NUM_DEVICES)
        WTI_FlexmuxTerminateOnError ("flexmux_id is out of bounds !", 0);

	/* 
	 * parameter check 
	 */
    if (flexmux_hw_config != WT_FLEXMUX_UFE_412_1_PORT_OC12 &&
        flexmux_hw_config != WT_FLEXMUX_UFE_412_2_PORTS_OC3 &&
        flexmux_hw_config != WT_FLEXMUX_UFE_412_4_PORTS_OC3 &&
        flexmux_hw_config != WT_FLEXMUX_UFE_448_4_PORTS_OC12
        && flexmux_hw_config != WT_FLEXMUX_UFE_448_16_PORTS_OC3
        && flexmux_hw_config !=
        WT_FLEXMUX_UFE_448_4_PORTS_OC12_PLUS_4_PROTECTION
        && flexmux_hw_config !=
        WT_FLEXMUX_UFE_412_4_PORTS_OC3_CARD_PROTECTION_MASTER
        && flexmux_hw_config !=
        WT_FLEXMUX_UFE_412_4_PORTS_OC3_CARD_PROTECTION_SLAVE)
        WTI_FlexmuxTerminateOnError ("Invalid flexmux_hw_config !", 0);

    if (flexmux_mode != WPX_UFE_FRAMER_DEVICE_MODE_SONET
        && flexmux_mode != WPX_UFE_FRAMER_DEVICE_MODE_SDH)
        WTI_FlexmuxTerminateOnError ("Invalid flexmux_mode !", 0);

    WTI_allocate_pm_storage_areas ();

	/*
 	 * decide line_port_mode by flexmux_hw_config & flexmux_mode.
	 */
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
        wti_flexmux_build_personality =
            WPX_UFE_FRAMER_BUILD_PERSONALITY_1_PLUS_1_OC3_OC12;
    }
    else if (flexmux_hw_config == WT_FLEXMUX_UFE_412_4_PORTS_OC3
             || flexmux_hw_config ==
             WT_FLEXMUX_UFE_412_4_PORTS_OC3_CARD_PROTECTION_MASTER
             || flexmux_hw_config ==
             WT_FLEXMUX_UFE_412_4_PORTS_OC3_CARD_PROTECTION_SLAVE)
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
        wti_flexmux_build_personality =
            WPX_UFE_FRAMER_BUILD_PERSONALITY_4_PLUS_4_OC3;
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
        wti_flexmux_build_personality =
            WPX_UFE_FRAMER_BUILD_PERSONALITY_2_PLUS_2_OC3;
    }
    else if (flexmux_hw_config == WT_FLEXMUX_UFE_448_4_PORTS_OC12)
    {
        if (flexmux_mode == WPX_UFE_FRAMER_DEVICE_MODE_SONET)
        {
            line_port_mode = WT_FLEXMUX_LINE_PORT_OC12;
        }
        else if (flexmux_mode == WPX_UFE_FRAMER_DEVICE_MODE_SDH)
        {
            line_port_mode = WT_FLEXMUX_LINE_PORT_STM4;
        }
        max_line_ports = 4;
        line_increment = 1;
        wti_flexmux_build_personality =
            WPX_UFE_FRAMER_BUILD_PERSONALITY_4_PLUS_4_OC3_OC12;
    }
    else if (flexmux_hw_config == WT_FLEXMUX_UFE_448_16_PORTS_OC3)
    {
        if (flexmux_mode == WPX_UFE_FRAMER_DEVICE_MODE_SONET)
        {
            line_port_mode = WT_FLEXMUX_LINE_PORT_OC3;
        }
        else if (flexmux_mode == WPX_UFE_FRAMER_DEVICE_MODE_SDH)
        {
            line_port_mode = WT_FLEXMUX_LINE_PORT_STM1;
        }
        max_line_ports = 16;
        line_increment = 1;
        wti_flexmux_build_personality =
            WPX_UFE_FRAMER_BUILD_PERSONALITY_16_OC3;
    }
    else if (flexmux_hw_config ==
             WT_FLEXMUX_UFE_448_4_PORTS_OC12_PLUS_4_PROTECTION)
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
        wti_flexmux_build_personality =
            WPX_UFE_FRAMER_BUILD_PERSONALITY_4_PLUS_4_OC3_OC12;
    }
    else if (flexmux_hw_config ==
             WT_FLEXMUX_UFE_448_8_PORTS_OC3_PLUS_8_PROTECTION)
    {
        if (flexmux_mode == WPX_UFE_FRAMER_DEVICE_MODE_SONET)
        {
            line_port_mode = WT_FLEXMUX_LINE_PORT_OC3;
        }
        else if (flexmux_mode == WPX_UFE_FRAMER_DEVICE_MODE_SDH)
        {
            line_port_mode = WT_FLEXMUX_LINE_PORT_STM1;
        }
        max_line_ports = 8;
        line_increment = 1;
        wti_flexmux_build_personality =
            WPX_UFE_FRAMER_BUILD_PERSONALITY_8_PLUS_8_OC3;
    }
    else
    {
        WTI_FlexmuxTerminateOnError ("Invalid flexmux_hw_config !", 0);
		printf ("WTI_FlexmuxInit, 8: set parameter here"); 
    }


    memset (flexmux_cfg, 0, sizeof (WTI_Flexmux_global_setup));

    /* set flexmux ID */
    flexmux_cfg->flexmux_id = flexmux_id;

    /* Mask all framer interrupts in the CPLD so that northbound messaging is disabled */
    WPX_Ufe412CpldInterruptMaskSet (0, 0x7e);

    /* Initialize the Flexmux device */
    status =
        WPX_UFE_FRAMER_FlexmuxInit (flexmux_cfg->flexmux_id, upi_index);
    WTI_FlexmuxCheckStatus ("WPX_UFE_FRAMER_FlexmuxInit", status,
                            __LINE__);

    /* service the mailboxes in a separate thread */
    {
        WP_U32 thread_params[2];
		int thread_status = 0;

        thread_params[0] = flexmux_cfg->flexmux_id;
        thread_params[1] = upi_index;

        if (mailbox_thread_id == 0)
			thread_status =  WPL_ThreadInit (&mailbox_thread_id,
                            (void *(*)(void *))
                            WPX_UFE_FRAMER_FlexmuxServiceMailbox,
                            (void *) thread_params);
    	WTI_FlexmuxCheckStatus ("WPL_ThreadInit ", thread_status,
                            __LINE__);
    }


    /* service the alarms and PM callbacks in a separate thread */
    {
        WP_U32 thread_params[2] = { 0, 1 };
        void process_framer_task_list (void);
		int thread_status2 = 0;

#define WPL_THREAD_LOCK_KEY WPL_LOCK_KEY_CREATE(WPL_HW_LOCK, WPL_PRIVATE_LOCK, 7, 0)
        extern WP_U32 private_thread_lock;

        WPL_LockKeyInit (WPL_THREAD_LOCK_KEY, &private_thread_lock);

        thread_status2 = WPL_ThreadInit (&alarm_and_pm_thread_id,
                        (void *(*)(void *)) process_framer_task_list,
                        (void *) thread_params);
    	WTI_FlexmuxCheckStatus ("WPL_ThreadInit 2", thread_status2,
                            __LINE__);
    }


    /* set the Flexmux device build personality, 
		wti_flexmux_build_personality = WPX_UFE_FRAMER_BUILD_PERSONALITY_1_PLUS_1_OC3_OC12 */
    status =
        WPX_UFE_FRAMER_FlexmuxSetBuildPersonality (flexmux_cfg->flexmux_id,
                                                   wti_flexmux_build_personality,
                                                   upi_index);
    WTI_FlexmuxCheckStatus ("WPX_UFE_FRAMER_FlexmuxSetBuildPersonality",
                            status, __LINE__);

#if WTI_UFE412_REVA
    /* set the gpt source clock required with wds-ufe4-s6 board */
    WPX_FRMR_DEMO_ONLY_Configure_GTP_ClockSource (flexmux_cfg->flexmux_id);
#endif

#if WTI_FRAMER_SCRIPT
    return 0;
#endif
    /* set device mode (SONET\SDH) */
    status =
        WPX_FRMR_DEV_SetMode (0, flexmux_cfg->flexmux_id, flexmux_mode);
    WTI_FlexmuxCheckStatus ("WPX_UFE_FRAMER_CONFIGURATION_DEVICE_SetMode",
                            status, __LINE__);
#if WTI_FLEXMUX_DBG_MODE
    U8 pDeviceProtectionMode;
    U8 pCardProtectionOperatingMode;
#endif
    /* Set line protection according to the build personality in use */
    if (flexmux_hw_config ==
        WT_FLEXMUX_UFE_412_4_PORTS_OC3_CARD_PROTECTION_MASTER)
    {
        status =
            WPX_FRMR_SetDevProtectMode (0, flexmux_cfg->flexmux_id,
                                        WPX_UFE_DEVICE_PROTECTION_MODE_CARD);
        WTI_FlexmuxCheckStatus ("WPX_FRMR_SetDevProtectMode", status,
                                __LINE__);
#if WTI_FLEXMUX_DBG_MODE
        WPX_FRMR_GetDevProtectMode (flexmux_cfg->flexmux_id,
                                    &pDeviceProtectionMode);
        if (pDeviceProtectionMode != WPX_UFE_DEVICE_PROTECTION_MODE_CARD)
            WTI_FlexmuxTerminateOnError
                ("WPX_FRMR_GetDevProtectMode != WPX_UFE_DEVICE_PROTECTION_MODE_CARD\n",
                 0);
#endif
        status =
            WPX_FRMR_SetDevCardProtectOperatingMode (0,
                                                     flexmux_cfg->
                                                     flexmux_id,
                                                     WPX_UFE_DEVICE_CARD_PROTECTION_OPERATING_MODE_MASTER);
        WTI_FlexmuxCheckStatus ("WPX_FRMR_SetDevCardProtectOperatingMode",
                                status, __LINE__);
#if WTI_FLEXMUX_DBG_MODE
        WPX_FRMR_GetDevCardProtectOperatingMode (flexmux_cfg->flexmux_id,
                                                 &pCardProtectionOperatingMode);
        if (pCardProtectionOperatingMode !=
            WPX_UFE_DEVICE_CARD_PROTECTION_OPERATING_MODE_MASTER)
            WTI_FlexmuxTerminateOnError
                ("WPX_FRMR_GetDevCardProtectOperatingMode != WPX_UFE_DEVICE_CARD_PROTECTION_OPERATING_MODE_MASTER\n",
                 0);
#endif
    }
    else if (flexmux_hw_config ==
             WT_FLEXMUX_UFE_412_4_PORTS_OC3_CARD_PROTECTION_SLAVE)
    {
        status =
            WPX_FRMR_SetDevProtectMode (0, flexmux_cfg->flexmux_id,
                                        WPX_UFE_DEVICE_PROTECTION_MODE_CARD);
        WTI_FlexmuxCheckStatus ("WPX_FRMR_SetDevProtectMode", status,
                                __LINE__);
#if WTI_FLEXMUX_DBG_MODE
        WPX_FRMR_GetDevProtectMode (flexmux_cfg->flexmux_id,
                                    &pDeviceProtectionMode);
        if (pDeviceProtectionMode != WPX_UFE_DEVICE_PROTECTION_MODE_CARD)
            WTI_FlexmuxTerminateOnError
                ("WPX_FRMR_GetDevProtectMode != WPX_UFE_DEVICE_PROTECTION_MODE_CARD\n",
                 0);
#endif
        status =
            WPX_FRMR_SetDevCardProtectOperatingMode (0,
                                                     flexmux_cfg->
                                                     flexmux_id,
                                                     WPX_UFE_DEVICE_CARD_PROTECTION_OPERATING_MODE_SLAVE);
        WTI_FlexmuxCheckStatus ("WPX_FRMR_SetDevCardProtectOperatingMode",
                                status, __LINE__);
#if WTI_FLEXMUX_DBG_MODE
        WPX_FRMR_GetDevCardProtectOperatingMode (flexmux_cfg->flexmux_id,
                                                 &pCardProtectionOperatingMode);
        if (pCardProtectionOperatingMode !=
            WPX_UFE_DEVICE_CARD_PROTECTION_OPERATING_MODE_SLAVE)
            WTI_FlexmuxTerminateOnError
                ("WPX_FRMR_GetDevCardProtectOperatingMode != WPX_UFE_DEVICE_CARD_PROTECTION_OPERATING_MODE_SLAVE\n",
                 0);
#endif

    }
    else
    {
        status =
            WPX_FRMR_SetDevProtectMode (0, flexmux_cfg->flexmux_id,
                                        WPX_UFE_DEVICE_PROTECTION_MODE_LINE);
        WTI_FlexmuxCheckStatus ("WPX_FRMR_SetDevProtectMode", status,
                                __LINE__);
#if WTI_FLEXMUX_DBG_MODE
        WPX_FRMR_GetDevProtectMode (flexmux_cfg->flexmux_id,
                                    &pDeviceProtectionMode);
        if (pDeviceProtectionMode != WPX_UFE_DEVICE_PROTECTION_MODE_LINE)
            WTI_FlexmuxTerminateOnError
                ("WPX_FRMR_GetDevProtectMode != WPX_UFE_DEVICE_PROTECTION_MODE_LINE\n",
                 0);
#endif
    }
#if WTI_FLEXMUX_DBG_MODE
    U8 pDeviceMode;

    status = WPX_FRMR_DEV_GetMode (flexmux_cfg->flexmux_id, &pDeviceMode);
    WTI_FlexmuxCheckStatus ("WPX_FRMR_DEV_GetMode", status, __LINE__);
    switch (flexmux_mode)
    {
    case WPX_UFE_FRAMER_DEVICE_MODE_SDH:
        if (pDeviceMode != WPX_UFE_FRAMER_DEVICE_MODE_SDH)
        {
            WTI_FlexmuxTerminateOnError
                ("WPX_FRMR_DEV_GetMode != WPX_UFE_DEVICE_MODE_SDH\n", 0);
        }
        break;
    case WPX_UFE_FRAMER_DEVICE_MODE_SONET:
        if (pDeviceMode != WPX_UFE_FRAMER_DEVICE_MODE_SONET)
        {
            WTI_FlexmuxTerminateOnError
                ("WPX_FRMR_DEV_GetMode != WPX_UFE_DEVICE_MODE_SONET\n", 0);
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
    flexmux_reg[flexmux_id].flexmux_build_personality =
        wti_flexmux_build_personality;
    flexmux_reg[flexmux_id].flexmux_mode = flexmux_mode;
    flexmux_reg[flexmux_id].max_line_ports = max_line_ports;

    if (wti_flexmux_build_personality ==
        WPX_UFE_FRAMER_BUILD_PERSONALITY_4_PLUS_4_OC3_OC12
        || wti_flexmux_build_personality ==
        WPX_UFE_FRAMER_BUILD_PERSONALITY_8_PLUS_8_OC3
        || wti_flexmux_build_personality ==
        WPX_UFE_FRAMER_BUILD_PERSONALITY_1_PLUS_1_OC3_OC12
        || wti_flexmux_build_personality ==
        WPX_UFE_FRAMER_BUILD_PERSONALITY_2_PLUS_2_OC3
        || wti_flexmux_build_personality ==
        WPX_UFE_FRAMER_BUILD_PERSONALITY_4_PLUS_4_OC3)
    {
        WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE line_end_point =
            { 0 };

#if WTI_ADM_MODE
/* in CR when using the ADM for testing */
        U8 TX_C2 = 0x1, EX_C2 = 0x1;
        WP_CHAR *TX_J1 = "0000000000000000", *EX_J1 =
            "0000000000000000", *pTX_TraceString =
            "WG RS-TRACE", *pEX_TraceString = "WG RS-TRACE";
#else /*  WTI_ADM_MODE */
        U8 TX_C2 = 0x2, EX_C2 = 0x2;
        WP_CHAR *TX_J1 = "WG HP-TRACE", *EX_J1 =
            "WG HP-TRACE", *pTX_TraceString =
            "WG RS-TRACE", *pEX_TraceString = "WG RS-TRACE";
#endif /* WTI_ADM_MODE */

        //status = WPX_FRMR_DEV_DRV_SONET_SDH_EnableAlarm(0,WPX_UFE_FRAMER_SDH_HP_PLM);
        //status = WPX_FRMR_DEV_DRV_SONET_SDH_EnableAlarm(0,WPX_UFE_FRAMER_SDH_HP_UNEQ);

        if (flexmux_mode == WPX_UFE_FRAMER_DEVICE_MODE_SONET)
        {
            WP_U32 sts12, sts3, sts1;

            line_end_point.TransferType =
                WPX_UFE_FRAMER_WUFE_SONET_TYPE_STS1_SPE;

            if (line_port_mode == WT_FLEXMUX_LINE_PORT_OC3)
            {
                sts12 = 0;
                line_end_point.u.SONET.sts12 = sts12;
                for (sts3 = 0; sts3 < max_line_ports;
                     sts3 += line_increment)
                {
                    WTI_FlexmuxLinePortConfig (0, sts3, /* Odd ports are used for protection */
                                               line_port_mode);
                    WPX_FRMR_SONET_SDH_PORT_SEC_J0_SetMode (0, flexmux_id,
                                                            sts3,
                                                            WPX_UFE_FRAMER_PATH_TRACE_MODE_16_BYTE);
                    status =
                        WPX_FRMR_SONET_SDH_PORT_SEC_J0_SetTX (0,
                                                              flexmux_id,
                                                              sts3,
                                                              pTX_TraceString);
                    WTI_FlexmuxCheckStatus
                        ("WPX_FRMR_SONET_SDH_SOH_J0_SetTX", status,
                         __LINE__);

                    status =
                        WPX_FRMR_SONET_SDH_PORT_SEC_J0_SetEX (0,
                                                              flexmux_id,
                                                              sts3,
                                                              pEX_TraceString);
                    WTI_FlexmuxCheckStatus
                        ("WPX_FRMR_SONET_SDH_SOH_J0_SetEX", status,
                         __LINE__);

                    line_end_point.u.SONET.sts3 = sts3;
                    for (sts1 = 0; sts1 < 3; ++sts1)
                    {
                        line_end_point.u.SONET.sts1 = sts1;
                        status =
                            WPX_FRMR_SONET_SDH_CreateFacility (0,
                                                               flexmux_id,
                                                               &line_end_point);
                        WTI_FlexmuxCheckStatus
                            ("WPX_FRMR_SONET_SDH_CreateFacility (VC3/4)",
                             status, __LINE__);

                        /* set the transmitted and expected C2 values */
                        status =
                            WPX_FRMR_SONET_SDH_HO_PATH_C2_SetTX (0,
                                                                 flexmux_id,
                                                                 &line_end_point,
                                                                 TX_C2);
                        WTI_FlexmuxCheckStatus
                            ("WPX_FRMR_SONET_SDH_HO_PATH_C2_SetTX", status,
                             __LINE__);
#if WTI_FLEXMUX_DBG_MODE
                        U8 pTX_C2;

                        status =
                            WPX_FRMR_SONET_SDH_HO_PATH_C2_GetTX
                            (flexmux_id, &line_end_point, &pTX_C2);
                        WTI_FlexmuxCheckStatus ("WPX_FRMR_DEV_GetMode",
                                                status, __LINE__);
                        if (pTX_C2 != TX_C2)
                        {
                            WTI_FlexmuxTerminateOnError
                                ("WPX_FRMR_SONET_SDH_HO_PATH_C2_GetTX != TX_C2\n",
                                 0);
                        }
#endif
                        status =
                            WPX_FRMR_SONET_SDH_HO_PATH_C2_SetEX (0,
                                                                 flexmux_id,
                                                                 &line_end_point,
                                                                 EX_C2);
                        WTI_FlexmuxCheckStatus
                            ("WPX_FRMR_SONET_SDH_HO_PATH_C2_SetEX", status,
                             __LINE__);
#if WTI_FLEXMUX_DBG_MODE
                        U8 pEX_C2;

                        status =
                            WPX_FRMR_SONET_SDH_HO_PATH_C2_GetTX
                            (flexmux_id, &line_end_point, &pEX_C2);
                        WTI_FlexmuxCheckStatus ("WPX_FRMR_DEV_GetMode",
                                                status, __LINE__);
                        if (pTX_C2 != TX_C2)
                        {
                            WTI_FlexmuxTerminateOnError
                                ("WPX_FRMR_SONET_SDH_HO_PATH_C2_GetTX != EX_C2\n",
                                 0);
                        }
#endif

                        WPX_FRMR_SONET_SDH_HO_PATH_J1_SetMode (0,
                                                               flexmux_id,
                                                               &line_end_point,
                                                               WPX_UFE_FRAMER_PATH_TRACE_MODE_16_BYTE);
                        status =
                            WPX_FRMR_SONET_SDH_HO_PATH_J1_SetTX (0,
                                                                 flexmux_id,
                                                                 &line_end_point,
                                                                 TX_J1);
                        WTI_FlexmuxCheckStatus
                            ("WPX_FRMR_SONET_SDH_HO_PATH_J1_SetTX", status,
                             __LINE__);
#if WTI_FLEXMUX_DBG_MODE
                        WP_CHAR *pTX_J1;

                        status =
                            WPX_FRMR_SONET_SDH_HO_PATH_J1_GetTX
                            (flexmux_id, &line_end_point, pTX_J1);
                        WTI_FlexmuxCheckStatus ("WPX_FRMR_DEV_GetMode",
                                                status, __LINE__);
                        if (pTX_J1 != TX_J1)
                        {
                            WTI_FlexmuxTerminateOnError
                                ("WPX_FRMR_SONET_SDH_HO_PATH_J1_GetTX != TX_J1\n",
                                 0);
                        }
#endif

                        status =
                            WPX_FRMR_SONET_SDH_HO_PATH_J1_SetEX (0,
                                                                 flexmux_id,
                                                                 &line_end_point,
                                                                 EX_J1);
                        WTI_FlexmuxCheckStatus
                            ("WPX_FRMR_SONET_SDH_HO_PATH_J1_SetEX", status,
                             __LINE__);
#if WTI_FLEXMUX_DBG_MODE
                        WP_CHAR *pEX_J1;

                        status =
                            WPX_FRMR_SONET_SDH_HO_PATH_J1_GetEX
                            (flexmux_id, &line_end_point, pEX_J1);
                        WTI_FlexmuxCheckStatus ("WPX_FRMR_DEV_GetMode",
                                                status, __LINE__);
                        if (pEX_J1 != EX_J1)
                        {
                            WTI_FlexmuxTerminateOnError
                                ("WPX_FRMR_SONET_SDH_HO_PATH_J1_GetEX != EX_J1\n",
                                 0);
                        }
#endif
                    }

                }
            }
            else if (line_port_mode == WT_FLEXMUX_LINE_PORT_OC12)
            {
                for (sts12 = 0; sts12 < max_line_ports;
                     sts12 += line_increment)
                {
                    line_end_point.u.SONET.sts12 = sts12;
                    WTI_FlexmuxLinePortConfig (0, sts12,    /* Odd ports are used for protection */
                                               line_port_mode);
                    WPX_FRMR_SONET_SDH_PORT_SEC_J0_SetMode (0, flexmux_id,
                                                            sts12,
                                                            WPX_UFE_FRAMER_PATH_TRACE_MODE_16_BYTE);
                    status =
                        WPX_FRMR_SONET_SDH_PORT_SEC_J0_SetTX (0,
                                                              flexmux_id,
                                                              sts12,
                                                              pTX_TraceString);
                    WTI_FlexmuxCheckStatus
                        ("WPX_FRMR_SONET_SDH_SOH_J0_SetTX", status,
                         __LINE__);

                    status =
                        WPX_FRMR_SONET_SDH_PORT_SEC_J0_SetEX (0,
                                                              flexmux_id,
                                                              sts12,
                                                              pEX_TraceString);
                    WTI_FlexmuxCheckStatus
                        ("WPX_FRMR_SONET_SDH_SOH_J0_SetEX", status,
                         __LINE__);

                    for (sts3 = 0; sts3 < 4; ++sts3)
                    {
                        line_end_point.u.SONET.sts3 = sts3;
                        for (sts1 = 0; sts1 < 3; ++sts1)
                        {
                            line_end_point.u.SONET.sts1 = sts1;
                            status =
                                WPX_FRMR_SONET_SDH_CreateFacility (0,
                                                                   flexmux_id,
                                                                   &line_end_point);
                            WTI_FlexmuxCheckStatus
                                ("WPX_FRMR_SONET_SDH_CreateFacility (VC2/4)",
                                 status, __LINE__);

                            /* set the transmitted and expected C2 values */
                            status =
                                WPX_FRMR_SONET_SDH_HO_PATH_C2_SetTX (0,
                                                                     flexmux_id,
                                                                     &line_end_point,
                                                                     TX_C2);
                            WTI_FlexmuxCheckStatus
                                ("WPX_FRMR_SONET_SDH_HO_PATH_C2_SetTX",
                                 status, __LINE__);

#if WTI_FLEXMUX_DBG_MODE
                            U8 pTX_C2;

                            status =
                                WPX_FRMR_SONET_SDH_HO_PATH_C2_GetTX
                                (flexmux_id, &line_end_point, &pTX_C2);
                            WTI_FlexmuxCheckStatus ("WPX_FRMR_DEV_GetMode",
                                                    status, __LINE__);
                            if (pTX_C2 != TX_C2)
                            {
                                WTI_FlexmuxTerminateOnError
                                    ("WPX_FRMR_SONET_SDH_HO_PATH_C2_GetTX != TX_C2\n",
                                     0);
                            }
#endif
                            status =
                                WPX_FRMR_SONET_SDH_HO_PATH_C2_SetEX (0,
                                                                     flexmux_id,
                                                                     &line_end_point,
                                                                     EX_C2);
                            WTI_FlexmuxCheckStatus
                                ("WPX_FRMR_SONET_SDH_HO_PATH_C2_SetEX",
                                 status, __LINE__);
#if WTI_FLEXMUX_DBG_MODE
                            U8 pEX_C2;

                            status =
                                WPX_FRMR_SONET_SDH_HO_PATH_C2_GetTX
                                (flexmux_id, &line_end_point, &pEX_C2);
                            WTI_FlexmuxCheckStatus ("WPX_FRMR_DEV_GetMode",
                                                    status, __LINE__);
                            if (pTX_C2 != TX_C2)
                            {
                                WTI_FlexmuxTerminateOnError
                                    ("WPX_FRMR_SONET_SDH_HO_PATH_C2_GetTX != EX_C2\n",
                                     0);
                            }
#endif
                            WPX_FRMR_SONET_SDH_HO_PATH_J1_SetMode (0,
                                                                   flexmux_id,
                                                                   &line_end_point,
                                                                   WPX_UFE_FRAMER_PATH_TRACE_MODE_16_BYTE);
                            status =
                                WPX_FRMR_SONET_SDH_HO_PATH_J1_SetTX (0,
                                                                     flexmux_id,
                                                                     &line_end_point,
                                                                     TX_J1);
                            WTI_FlexmuxCheckStatus
                                ("WPX_FRMR_SONET_SDH_HO_PATH_J1_SetTX",
                                 status, __LINE__);
#if WTI_FLEXMUX_DBG_MODE
                            WP_CHAR *pTX_J1;

                            status =
                                WPX_FRMR_SONET_SDH_HO_PATH_J1_GetTX
                                (flexmux_id, &line_end_point, pTX_J1);
                            WTI_FlexmuxCheckStatus ("WPX_FRMR_DEV_GetMode",
                                                    status, __LINE__);
                            if (pTX_J1 != TX_J1)
                            {
                                WTI_FlexmuxTerminateOnError
                                    ("WPX_FRMR_SONET_SDH_HO_PATH_J1_GetTX != TX_J1\n",
                                     0);
                            }
#endif

                            status =
                                WPX_FRMR_SONET_SDH_HO_PATH_J1_SetEX (0,
                                                                     flexmux_id,
                                                                     &line_end_point,
                                                                     EX_J1);
                            WTI_FlexmuxCheckStatus
                                ("WPX_FRMR_SONET_SDH_HO_PATH_J1_SetEX",
                                 status, __LINE__);
#if WTI_FLEXMUX_DBG_MODE
                            WP_CHAR *pEX_J1;

                            status =
                                WPX_FRMR_SONET_SDH_HO_PATH_J1_GetEX
                                (flexmux_id, &line_end_point, pEX_J1);
                            WTI_FlexmuxCheckStatus ("WPX_FRMR_DEV_GetMode",
                                                    status, __LINE__);
                            if (pEX_J1 != EX_J1)
                            {
                                WTI_FlexmuxTerminateOnError
                                    ("WPX_FRMR_SONET_SDH_HO_PATH_J1_GetEX != EX_J1\n",
                                     0);
                            }
#endif
                        }

                    }
                }
            }
        }
        else if (flexmux_mode == WPX_UFE_FRAMER_DEVICE_MODE_SDH)
        {
            WP_U32 stm4, stm1, stm0, num_for_vc3_or_vc4;

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
                for (stm1 = 0; stm1 < max_line_ports;
                     stm1 += line_increment)
                {
                    line_end_point.u.SDH.stm1 = stm1;
                    WTI_FlexmuxLinePortConfig (0, stm1, /* Odd ports are used for protection */
                                               line_port_mode);
                    WPX_FRMR_SONET_SDH_PORT_SEC_J0_SetMode (0, flexmux_id,
                                                            stm1,
                                                            WPX_UFE_FRAMER_PATH_TRACE_MODE_16_BYTE);
                    status =
                        WPX_FRMR_SONET_SDH_PORT_SEC_J0_SetTX (0,
                                                              flexmux_id,
                                                              stm1,
                                                              pTX_TraceString);
                    WTI_FlexmuxCheckStatus
                        ("WPX_FRMR_SONET_SDH_SOH_J0_SetTX", status,
                         __LINE__);

                    status =
                        WPX_FRMR_SONET_SDH_PORT_SEC_J0_SetEX (0,
                                                              flexmux_id,
                                                              stm1,
                                                              pEX_TraceString);
                    WTI_FlexmuxCheckStatus
                        ("WPX_FRMR_SONET_SDH_SOH_J0_SetEX", status,
                         __LINE__);

                    status =
                        WPX_FRMR_SONET_SDH_PORT_SEC_B1_SetExcessiveTh (510
                                                                       +
                                                                       stm1,
                                                                       0,
                                                                       stm1,
                                                                       WPX_UFE_FRAMER_THRESHOLD_10E4);
                    WTI_FlexmuxCheckStatus
                        ("WPX_FRMR_SONET_SDH_PORT_SEC_B1_SetExcessiveTh",
                         status, __LINE__);

                    status =
                        WPX_FRMR_SONET_SDH_PORT_SEC_B1_SetSignalDegradeTh
                        (410 + stm1, 0, stm1,
                         WPX_UFE_FRAMER_THRESHOLD_10E5);
                    WTI_FlexmuxCheckStatus
                        ("WPX_FRMR_SONET_SDH_PORT_SEC_B1_SetSignalDegradeTh",
                         status, __LINE__);

                    status =
                        WPX_FRMR_SONET_SDH_PORT_LINE_B2_SetExcessiveTh (540
                                                                        +
                                                                        stm1,
                                                                        0,
                                                                        stm1,
                                                                        WPX_UFE_FRAMER_THRESHOLD_10E5);
                    WTI_FlexmuxCheckStatus
                        ("WPX_FRMR_SONET_SDH_PORT_LINE_B2_SetExcessiveTh",
                         status, __LINE__);

                    status =
                        WPX_FRMR_SONET_SDH_PORT_LINE_B2_SetSigDegradeTh
                        (440 + stm1, 0, stm1,
                         WPX_UFE_FRAMER_THRESHOLD_10E6);
                    WTI_FlexmuxCheckStatus
                        ("WPX_FRMR_SONET_SDH_PORT_LINE_B2_SetSigDegradeTh",
                         status, __LINE__);

                    for (stm0 = 0; stm0 < num_for_vc3_or_vc4; ++stm0)
                    {
                        line_end_point.u.SDH.stm0 = stm0;
                        status =
                            WPX_FRMR_SONET_SDH_CreateFacility (0,
                                                               flexmux_id,
                                                               &line_end_point);
                        WTI_FlexmuxCheckStatus
                            ("WPX_FRMR_SONET_SDH_CreateFacility (VC3/4)",
                             status, __LINE__);

                        /* set the transmitted and expected C2 values */
                        status =
                            WPX_FRMR_SONET_SDH_HO_PATH_C2_SetTX (0,
                                                                 flexmux_id,
                                                                 &line_end_point,
                                                                 TX_C2);
                        WTI_FlexmuxCheckStatus
                            ("WPX_FRMR_SONET_SDH_HO_PATH_C2_SetTX", status,
                             __LINE__);
#if WTI_FLEXMUX_DBG_MODE
                        U8 pTX_C2;

                        status =
                            WPX_FRMR_SONET_SDH_HO_PATH_C2_GetTX
                            (flexmux_id, &line_end_point, &pTX_C2);
                        WTI_FlexmuxCheckStatus ("WPX_FRMR_DEV_GetMode",
                                                status, __LINE__);
                        if (pTX_C2 != TX_C2)
                        {
                            WTI_FlexmuxTerminateOnError
                                ("WPX_FRMR_SONET_SDH_HO_PATH_C2_GetTX != TX_C2\n",
                                 0);
                        }
#endif
                        status =
                            WPX_FRMR_SONET_SDH_HO_PATH_C2_SetEX (0,
                                                                 flexmux_id,
                                                                 &line_end_point,
                                                                 EX_C2);
                        WTI_FlexmuxCheckStatus
                            ("WPX_FRMR_SONET_SDH_HO_PATH_C2_SetEX", status,
                             __LINE__);
#if WTI_FLEXMUX_DBG_MODE
                        U8 pEX_C2;

                        status =
                            WPX_FRMR_SONET_SDH_HO_PATH_C2_GetTX
                            (flexmux_id, &line_end_point, &pEX_C2);
                        WTI_FlexmuxCheckStatus ("WPX_FRMR_DEV_GetMode",
                                                status, __LINE__);
                        if (pTX_C2 != TX_C2)
                        {
                            WTI_FlexmuxTerminateOnError
                                ("WPX_FRMR_SONET_SDH_HO_PATH_C2_GetTX != EX_C2\n",
                                 0);
                        }
#endif
                        WPX_FRMR_SONET_SDH_HO_PATH_J1_SetMode (0,
                                                               flexmux_id,
                                                               &line_end_point,
                                                               WPX_UFE_FRAMER_PATH_TRACE_MODE_16_BYTE);
                        status =
                            WPX_FRMR_SONET_SDH_HO_PATH_J1_SetTX (0,
                                                                 flexmux_id,
                                                                 &line_end_point,
                                                                 TX_J1);
                        WTI_FlexmuxCheckStatus
                            ("WPX_FRMR_SONET_SDH_HO_PATH_J1_SetTX", status,
                             __LINE__);
#if WTI_FLEXMUX_DBG_MODE
                        WP_CHAR *pTX_J1;

                        status =
                            WPX_FRMR_SONET_SDH_HO_PATH_J1_GetTX
                            (flexmux_id, &line_end_point, pTX_J1);
                        WTI_FlexmuxCheckStatus ("WPX_FRMR_DEV_GetMode",
                                                status, __LINE__);
                        if (pTX_J1 != TX_J1)
                        {
                            WTI_FlexmuxTerminateOnError
                                ("WPX_FRMR_SONET_SDH_HO_PATH_J1_GetTX != TX_J1\n",
                                 0);
                        }
#endif

                        status =
                            WPX_FRMR_SONET_SDH_HO_PATH_J1_SetEX (0,
                                                                 flexmux_id,
                                                                 &line_end_point,
                                                                 EX_J1);
                        WTI_FlexmuxCheckStatus
                            ("WPX_FRMR_SONET_SDH_HO_PATH_J1_SetEX", status,
                             __LINE__);
#if WTI_FLEXMUX_DBG_MODE
                        WP_CHAR *pEX_J1;

                        status =
                            WPX_FRMR_SONET_SDH_HO_PATH_J1_GetEX
                            (flexmux_id, &line_end_point, pEX_J1);
                        WTI_FlexmuxCheckStatus ("WPX_FRMR_DEV_GetMode",
                                                status, __LINE__);
                        if (pEX_J1 != EX_J1)
                        {
                            WTI_FlexmuxTerminateOnError
                                ("WPX_FRMR_SONET_SDH_HO_PATH_J1_GetEX != EX_J1\n",
                                 0);
                        }
#endif
                        status =
                            WPX_FRMR_SONET_SDH_HO_PATH_B3_SetExcessiveTh
                            (0, flexmux_id, &line_end_point,
                             WPX_UFE_FRAMER_THRESHOLD_10E4);
                        WTI_FlexmuxCheckStatus
                            ("WPX_FRMR_SONET_SDH_HO_PATH_B3_SetExcessiveTh",
                             status, __LINE__);

                        status =
                            WPX_FRMR_SONET_SDH_HO_PATH_B3_SetSigDegradeTh
                            (0, flexmux_id, &line_end_point,
                             WPX_UFE_FRAMER_THRESHOLD_10E6);
                        WTI_FlexmuxCheckStatus
                            ("WPX_FRMR_SONET_SDH_HO_PATH_B3_SetSigDegradeTh",
                             status, __LINE__);
                        printf ("set excessive th\n");
                    }
                }
            }
            else if (line_port_mode == WT_FLEXMUX_LINE_PORT_STM4)
            {
                for (stm4 = 0; stm4 < max_line_ports;
                     stm4 += line_increment)
                {
                    line_end_point.u.SDH.stm4 = stm4;
                    WTI_FlexmuxLinePortConfig (0, stm4, /* Odd ports are used for protection */
                                               line_port_mode);
                    WPX_FRMR_SONET_SDH_PORT_SEC_J0_SetMode (0, flexmux_id,
                                                            stm4,
                                                            WPX_UFE_FRAMER_PATH_TRACE_MODE_16_BYTE);
                    status =
                        WPX_FRMR_SONET_SDH_PORT_SEC_J0_SetTX (0,
                                                              flexmux_id,
                                                              stm4,
                                                              pTX_TraceString);
                    WTI_FlexmuxCheckStatus
                        ("WPX_FRMR_SONET_SDH_SOH_J0_SetTX", status,
                         __LINE__);

                    status =
                        WPX_FRMR_SONET_SDH_PORT_SEC_J0_SetEX (0,
                                                              flexmux_id,
                                                              stm4,
                                                              pEX_TraceString);
                    WTI_FlexmuxCheckStatus
                        ("WPX_FRMR_SONET_SDH_SOH_J0_SetEX", status,
                         __LINE__);

                    status =
                        WPX_FRMR_SONET_SDH_PORT_SEC_B1_SetExcessiveTh (510
                                                                       +
                                                                       stm1,
                                                                       flexmux_id,
                                                                       stm4,
                                                                       WPX_UFE_FRAMER_THRESHOLD_10E4);
                    WTI_FlexmuxCheckStatus
                        ("WPX_FRMR_SONET_SDH_PORT_SEC_B1_SetExcessiveTh",
                         status, __LINE__);

                    status =
                        WPX_FRMR_SONET_SDH_PORT_SEC_B1_SetSignalDegradeTh
                        (410 + stm1, flexmux_id, stm4,
                         WPX_UFE_FRAMER_THRESHOLD_10E5);
                    WTI_FlexmuxCheckStatus
                        ("WPX_FRMR_SONET_SDH_PORT_SEC_B1_SetSignalDegradeTh",
                         status, __LINE__);

                    status =
                        WPX_FRMR_SONET_SDH_PORT_LINE_B2_SetExcessiveTh (540
                                                                        +
                                                                        stm1,
                                                                        flexmux_id,
                                                                        stm4,
                                                                        WPX_UFE_FRAMER_THRESHOLD_10E5);
                    WTI_FlexmuxCheckStatus
                        ("WPX_FRMR_SONET_SDH_PORT_LINE_B2_SetExcessiveTh",
                         status, __LINE__);

                    status =
                        WPX_FRMR_SONET_SDH_PORT_LINE_B2_SetSigDegradeTh
                        (440 + stm1, flexmux_id, stm4,
                         WPX_UFE_FRAMER_THRESHOLD_10E6);
                    WTI_FlexmuxCheckStatus
                        ("WPX_FRMR_SONET_SDH_PORT_LINE_B2_SetSigDegradeTh",
                         status, __LINE__);

                    for (stm1 = 0; stm1 < 4; ++stm1)
                    {
                        line_end_point.u.SDH.stm1 = stm1;
                        for (stm0 = 0; stm0 < num_for_vc3_or_vc4; ++stm0)
                        {
                            line_end_point.u.SDH.stm0 = stm0;
                            status =
                                WPX_FRMR_SONET_SDH_CreateFacility (0,
                                                                   flexmux_id,
                                                                   &line_end_point);
                            WTI_FlexmuxCheckStatus
                                ("WPX_FRMR_SONET_SDH_CreateFacility (VC3/4)",
                                 status, __LINE__);

                            /* set the transmitted and expected C2 values */
                            status =
                                WPX_FRMR_SONET_SDH_HO_PATH_C2_SetTX (0,
                                                                     flexmux_id,
                                                                     &line_end_point,
                                                                     TX_C2);
                            WTI_FlexmuxCheckStatus
                                ("WPX_FRMR_SONET_SDH_HO_PATH_C2_SetTX",
                                 status, __LINE__);
#if WTI_FLEXMUX_DBG_MODE
                            U8 pTX_C2;

                            status =
                                WPX_FRMR_SONET_SDH_HO_PATH_C2_GetTX
                                (flexmux_id, &line_end_point, &pTX_C2);
                            WTI_FlexmuxCheckStatus ("WPX_FRMR_DEV_GetMode",
                                                    status, __LINE__);
                            if (pTX_C2 != TX_C2)
                            {
                                WTI_FlexmuxTerminateOnError
                                    ("WPX_FRMR_SONET_SDH_HO_PATH_C2_GetTX != TX_C2\n",
                                     0);
                            }
#endif
                            status =
                                WPX_FRMR_SONET_SDH_HO_PATH_C2_SetEX (0,
                                                                     flexmux_id,
                                                                     &line_end_point,
                                                                     EX_C2);
                            WTI_FlexmuxCheckStatus
                                ("WPX_FRMR_SONET_SDH_HO_PATH_C2_SetEX",
                                 status, __LINE__);
#if WTI_FLEXMUX_DBG_MODE
                            U8 pEX_C2;

                            status =
                                WPX_FRMR_SONET_SDH_HO_PATH_C2_GetTX
                                (flexmux_id, &line_end_point, &pEX_C2);
                            WTI_FlexmuxCheckStatus ("WPX_FRMR_DEV_GetMode",
                                                    status, __LINE__);
                            if (pTX_C2 != TX_C2)
                            {
                                WTI_FlexmuxTerminateOnError
                                    ("WPX_FRMR_SONET_SDH_HO_PATH_C2_GetTX != EX_C2\n",
                                     0);
                            }
#endif
                            WPX_FRMR_SONET_SDH_HO_PATH_J1_SetMode (0,
                                                                   flexmux_id,
                                                                   &line_end_point,
                                                                   WPX_UFE_FRAMER_PATH_TRACE_MODE_16_BYTE);
                            status =
                                WPX_FRMR_SONET_SDH_HO_PATH_J1_SetTX (0,
                                                                     flexmux_id,
                                                                     &line_end_point,
                                                                     TX_J1);
                            WTI_FlexmuxCheckStatus
                                ("WPX_FRMR_SONET_SDH_HO_PATH_J1_SetTX",
                                 status, __LINE__);
#if WTI_FLEXMUX_DBG_MODE
                            WP_CHAR *pTX_J1;

                            status =
                                WPX_FRMR_SONET_SDH_HO_PATH_J1_GetTX
                                (flexmux_id, &line_end_point, pTX_J1);
                            WTI_FlexmuxCheckStatus ("WPX_FRMR_DEV_GetMode",
                                                    status, __LINE__);
                            if (pTX_J1 != TX_J1)
                            {
                                WTI_FlexmuxTerminateOnError
                                    ("WPX_FRMR_SONET_SDH_HO_PATH_J1_GetTX != TX_J1\n",
                                     0);
                            }
#endif

                            status =
                                WPX_FRMR_SONET_SDH_HO_PATH_J1_SetEX (0,
                                                                     flexmux_id,
                                                                     &line_end_point,
                                                                     EX_J1);
                            WTI_FlexmuxCheckStatus
                                ("WPX_FRMR_SONET_SDH_HO_PATH_J1_SetEX",
                                 status, __LINE__);
#if WTI_FLEXMUX_DBG_MODE
                            WP_CHAR *pEX_J1;

                            status =
                                WPX_FRMR_SONET_SDH_HO_PATH_J1_GetEX
                                (flexmux_id, &line_end_point, pEX_J1);
                            WTI_FlexmuxCheckStatus ("WPX_FRMR_DEV_GetMode",
                                                    status, __LINE__);
                            if (pEX_J1 != EX_J1)
                            {
                                WTI_FlexmuxTerminateOnError
                                    ("WPX_FRMR_SONET_SDH_HO_PATH_J1_GetEX != EX_J1\n",
                                     0);
                            }
#endif
                            status =
                                WPX_FRMR_SONET_SDH_HO_PATH_B3_SetExcessiveTh
                                (700, 0, &line_end_point,
                                 WPX_UFE_FRAMER_THRESHOLD_10E4);
                            WTI_FlexmuxCheckStatus
                                ("WPX_FRMR_SONET_SDH_HO_PATH_B3_SetExcessiveTh",
                                 status, __LINE__);

                            status =
                                WPX_FRMR_SONET_SDH_HO_PATH_B3_SetSigDegradeTh
                                (701, 0, &line_end_point,
                                 WPX_UFE_FRAMER_THRESHOLD_10E6);
                            WTI_FlexmuxCheckStatus
                                ("WPX_FRMR_SONET_SDH_HO_PATH_B3_SetSigDegradeTh",
                                 status, __LINE__);
                        }
                    }
                }
            }
        }
    }
    else
    {
        WTI_FlexmuxTerminateOnError ("Unsupported Build personality", 0);
    }

    /* Set which SONET\SDH port Rx clock drives the RECV_LINE_CLK (which drive the External PLL diff 0/1 output) */
    /* WPX_FRMR_DEV_DIAG_DRV_RecClkSet(U32 TransactionIdentifier, U8 iDevice, U8 OutputSelector, U8 iLinePort)   */

    status = WPX_FRMR_DEV_DIAG_DRV_RecClkSet (0, 0, 0, 0);
    WTI_FlexmuxCheckStatus ("WPX_FRMR_DEV_DIAG_DRV_RecClkSet", status,
                            __LINE__);

    status = WPX_FRMR_DEV_DIAG_DRV_RecClkSet (0, 0, 1, 0);
    WTI_FlexmuxCheckStatus ("WPX_FRMR_DEV_DIAG_DRV_RecClkSet", status,
                            __LINE__);

#if WTI_FLEXMUX_DBG_MODE
    U8 piLinePort;

    status = WPX_FRMR_DEV_DIAG_DRV_RecClkGet (flexmux_id, &piLinePort);
    WTI_FlexmuxCheckStatus ("WPX_FRMR_DEV_GetMode", status, __LINE__);
    if (piLinePort != 0)
    {
        WTI_FlexmuxTerminateOnError
            ("WPX_FRMR_DEV_DIAG_DRV_RecClkGet != 0\n", 0);
    }
#endif

    return 0;
}

WP_S32 WTI_FlexmuxLinePortConfig (WP_U8 flexmux_id, WP_U8 line_port_id,
                                  WTI_flexmux_line_port_mode line_port_cfg)
{
    WP_U8 status = 0;
    if (flexmux_id > WTI_FLEXMUX_NUM_DEVICES)
        WTI_FlexmuxTerminateOnError ("flexmux_id is out of bounds !", 0);

    if (flexmux_reg[flexmux_id].is_initialized != WP_TRUE)
        WTI_FlexmuxTerminateOnError ("Flexmux device is not initialized!",
                                     0);

    if (line_port_id >= WTI_MAX_LINE_PORTS_PER_FLEXMUX_DEVICE)
        WTI_FlexmuxTerminateOnError ("Invalid line_port_id!", 0);

    if ((flexmux_reg[flexmux_id].flexmux_mode ==
         WPX_UFE_FRAMER_DEVICE_MODE_SONET)
        && (line_port_cfg != WT_FLEXMUX_LINE_PORT_OC3
            && line_port_cfg != WT_FLEXMUX_LINE_PORT_OC12))
        WTI_FlexmuxTerminateOnError ("Invalid line_port_cfg!", 0);

    if ((flexmux_reg[flexmux_id].flexmux_mode ==
         WPX_UFE_FRAMER_DEVICE_MODE_SDH)
        && (line_port_cfg != WT_FLEXMUX_LINE_PORT_STM1
            && line_port_cfg != WT_FLEXMUX_LINE_PORT_STM4))
        WTI_FlexmuxTerminateOnError ("Invalid line_port_cfg!", 0);

#undef WTI_FLEXMUX_DBG_MODE
#define WTI_FLEXMUX_DBG_MODE 1
    if (line_port_cfg == WT_FLEXMUX_LINE_PORT_OC3)
    {
        status =
            WPX_FRMR_SONET_SDH_PORT_SetRate (0, flexmux_id, line_port_id,
                                             WPX_UFE_FRAMER_LINE_PORT_RATE_OC3);
        WTI_FlexmuxCheckStatus ("WPX_FRMR_SONET_SDH_PORT_SetRate", status,
                                __LINE__);
#if WTI_FLEXMUX_DBG_MODE
        {
            U8 pPortRate;

            status =
                WPX_FRMR_SONET_SDH_PORT_GetRate (flexmux_id, line_port_id,
                                                 &pPortRate);
            WTI_FlexmuxCheckStatus ("WPX_FRMR_SONET_SDH_PORT_GetRate",
                                    status, __LINE__);
            if (pPortRate != WPX_UFE_FRAMER_LINE_PORT_RATE_OC3)
            {
                WTI_FlexmuxTerminateOnError
                    ("WPX_FRMR_SONET_SDH_PORT_GetRate != WPX_UFE_FRAMER_LINE_PORT_RATE_STS3C\n",
                     0);
            }
        }
#endif
    }
    else if (line_port_cfg == WT_FLEXMUX_LINE_PORT_OC12)
    {
        status =
            WPX_FRMR_SONET_SDH_PORT_SetRate (0, flexmux_id, line_port_id,
                                             WPX_UFE_FRAMER_LINE_PORT_RATE_OC12);
        WTI_FlexmuxCheckStatus ("WPX_FRMR_SONET_SDH_PORT_SetRate", status,
                                __LINE__);
#if WTI_FLEXMUX_DBG_MODE
        {
        U8 pPortRate;

        status =
            WPX_FRMR_SONET_SDH_PORT_GetRate (flexmux_id, line_port_id,
                                             &pPortRate);
        WTI_FlexmuxCheckStatus ("WPX_FRMR_SONET_SDH_PORT_GetRate", status,
                                __LINE__);
        if (pPortRate != WPX_UFE_FRAMER_LINE_PORT_RATE_OC12)
        {
            WTI_FlexmuxTerminateOnError
                ("WPX_FRMR_SONET_SDH_PORT_GetRate != WPX_UFE_FRAMER_LINE_PORT_RATE_STS12C\n",
                 0);
        }
        }
#endif

    }
    else if (line_port_cfg == WT_FLEXMUX_LINE_PORT_STM1)
    {
        status =
            WPX_FRMR_SONET_SDH_PORT_SetRate (0, flexmux_id, line_port_id,
                                             WPX_UFE_FRAMER_LINE_PORT_RATE_STM1);
        WTI_FlexmuxCheckStatus ("WPX_FRMR_SONET_SDH_PORT_SetRate", status,
                                __LINE__);
#if WTI_FLEXMUX_DBG_MODE
        {
        U8 pPortRate;

        status =
            WPX_FRMR_SONET_SDH_PORT_GetRate (flexmux_id, line_port_id,
                                             &pPortRate);
        WTI_FlexmuxCheckStatus ("WPX_FRMR_SONET_SDH_PORT_GetRate", status,
                                __LINE__);
        if (pPortRate != WPX_UFE_FRAMER_LINE_PORT_RATE_STM1)
        {
            WTI_FlexmuxTerminateOnError
                ("WPX_FRMR_SONET_SDH_PORT_GetRate != WPX_UFE_FRAMER_LINE_PORT_RATE_STM1\n",
                 0);
        }
        }
#endif
    }
    else if (line_port_cfg == WT_FLEXMUX_LINE_PORT_STM4)
    {
        status =
            WPX_FRMR_SONET_SDH_PORT_SetRate (0, flexmux_id, line_port_id,
                                             WPX_UFE_FRAMER_LINE_PORT_RATE_STM4);
        WTI_FlexmuxCheckStatus ("WPX_FRMR_SONET_SDH_PORT_SetRate", status,
                                __LINE__);

#if WTI_FLEXMUX_DBG_MODE
	{
        U8 pPortRate;

        status =
            WPX_FRMR_SONET_SDH_PORT_GetRate (flexmux_id, line_port_id,
                                             &pPortRate);
        WTI_FlexmuxCheckStatus ("WPX_FRMR_SONET_SDH_PORT_GetRate", status,
                                __LINE__);
        if (pPortRate != WPX_UFE_FRAMER_LINE_PORT_RATE_STM4)
        {
            WTI_FlexmuxTerminateOnError
                ("WPX_FRMR_SONET_SDH_PORT_GetRate != WPX_UFE_FRAMER_LINE_PORT_RATE_STM4\n",
                 0);
        }
	}
#endif
    }
#undef WTI_FLEXMUX_DBG_MODE
#define WTI_FLEXMUX_DBG_MODE 0

    /* save line port rate in the registry */
    flexmux_reg[flexmux_id].line_ports_reg[line_port_id].line_port_mode =
        line_port_cfg;

    return 0;
}


WP_S32 WTI_FlexmuxConnectionCreate (WP_U8 flexmux_id, WP_U8 line_port_id,
                                    WP_U32 client_port_id,
                                    WTI_flexmux_connection_type type,
                                    void *ufe_line_params)
{
    /* This function creates the client and line port facilities and connects them */
    WP_U8 status = 0, tx_clk_rec_enable = 0;
    WP_U32 line_timing = 0;
    WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE line_end_point;

    if (flexmux_id > WTI_FLEXMUX_NUM_DEVICES)
        WTI_FlexmuxTerminateOnError ("flexmux_id is out of bounds !", 0);

    /* create client port facility */
    if (type == WT_FLEXMUX_CONNECTION_TYPE_E1_UNFRAMED)
    {
        status =
            WPX_FRMR_SOCKET_PDH_CreateFacility (0, flexmux_id,
                                                client_port_id,
                                                WPX_UFE_FRAMER_CLIENT_E1);
        WTI_FlexmuxCheckStatus ("WPX_FRMR_SOCKET_PDH_CreateFacility",
                                status, __LINE__);

        status =
            WPX_FRMR_SOCKET_PDH_SetFraming (0, flexmux_id, client_port_id,
                                            WPX_UFE_FRAMER_CLIENT_UNFRAMED);
        WTI_FlexmuxCheckStatus ("WPX_FRMR_SOCKET_PDH_SetFraming", status,
                                __LINE__);
#if WTI_FLEXMUX_DBG_MODE
        U8 pFraming;

        status =
            WPX_FRMR_SOCKET_PDH_GetFraming (flexmux_id, client_port_id,
                                            &pFraming);
        WTI_FlexmuxCheckStatus ("WPX_FRMR_SOCKET_PDH_GetFraming", status,
                                __LINE__);
        if (pFraming != WPX_UFE_FRAMER_CLIENT_UNFRAMED)
        {
            WTI_FlexmuxTerminateOnError
                ("WPX_FRMR_SOCKET_PDH_GetFraming != WPX_UFE_FRAMER_CLIENT_UNFRAMED\n",
                 0);
        }
#endif
        status =
            WPX_FRMR_SOCKET_PDH_SetMapping (0, flexmux_id, client_port_id,
                                            WPX_UFE_FRAMER_CLIENT_MAPPING_BIT_ASYNC);
        WTI_FlexmuxCheckStatus ("WPX_FRMR_SOCKET_PDH_SetMapping", status,
                                __LINE__);
#if WTI_FLEXMUX_DBG_MODE
        U8 pMapping;

        status =
            WPX_FRMR_SOCKET_PDH_GetMapping (flexmux_id, client_port_id,
                                            &pMapping);
        WTI_FlexmuxCheckStatus ("WPX_FRMR_SOCKET_PDH_GetMapping", status,
                                __LINE__);
        if (pMapping != WPX_UFE_FRAMER_CLIENT_MAPPING_BIT_ASYNC)
        {
            WTI_FlexmuxTerminateOnError
                ("WPX_FRMR_SOCKET_PDH_GetMapping != WPX_UFE_FRAMER_CLIENT_MAPPING_BIT_ASYNC\n",
                 0);
        }
#endif

    }
    else if (type == WT_FLEXMUX_CONNECTION_TYPE_E1_FRAMED)
    {
        status =
            WPX_FRMR_SOCKET_PDH_CreateFacility (0, flexmux_id,
                                                client_port_id,
                                                WPX_UFE_FRAMER_CLIENT_E1);
        WTI_FlexmuxCheckStatus ("WPX_FRMR_SOCKET_PDH_CreateFacility",
                                status, __LINE__);

        status =
            WPX_FRMR_SOCKET_PDH_SetFraming (0, flexmux_id, client_port_id,
                                            WPX_UFE_FRAMER_CLIENT_PCM31CRC);
        WTI_FlexmuxCheckStatus ("WPX_FRMR_SOCKET_PDH_SetFraming", status,
                                __LINE__);

#if WTI_FLEXMUX_DBG_MODE
        U8 pFraming;

        status =
            WPX_FRMR_SOCKET_PDH_GetFraming (flexmux_id, client_port_id,
                                            &pFraming);
        WTI_FlexmuxCheckStatus ("WPX_FRMR_SOCKET_PDH_GetFraming", status,
                                __LINE__);
        if (pFraming != WPX_UFE_FRAMER_CLIENT_PCM31CRC)
        {
            WTI_FlexmuxTerminateOnError
                ("WPX_FRMR_SOCKET_PDH_GetFraming != WPX_UFE_FRAMER_CLIENT_PCM31CRC\n",
                 0);
        }
#endif

        status =
            WPX_FRMR_SOCKET_PDH_SetMapping (0, flexmux_id, client_port_id,
                                            WPX_UFE_FRAMER_CLIENT_MAPPING_BIT_ASYNC);
        WTI_FlexmuxCheckStatus ("WPX_FRMR_SOCKET_PDH_SetMapping", status,
                                __LINE__);

#if WTI_FLEXMUX_DBG_MODE
        U8 pMapping;

        status =
            WPX_FRMR_SOCKET_PDH_GetMapping (flexmux_id, client_port_id,
                                            &pMapping);
        WTI_FlexmuxCheckStatus ("WPX_FRMR_SOCKET_PDH_GetMapping", status,
                                __LINE__);
        if (pMapping != WPX_UFE_FRAMER_CLIENT_MAPPING_BIT_ASYNC)
        {
            WTI_FlexmuxTerminateOnError
                ("WPX_FRMR_SOCKET_PDH_GetMapping != WPX_UFE_FRAMER_CLIENT_MAPPING_BIT_ASYNC\n",
                 0);
        }
#endif

        status =
            WPX_FRMR_SOCKET_PDH_DisableCAS (0, flexmux_id, client_port_id);
        WTI_FlexmuxCheckStatus ("WPX_FRMR_SOCKET_PDH_DisableCAS", status,
                                __LINE__);

#if WTI_FLEXMUX_DBG_MODE
        U8 pCAS_IsEnabled;

        status =
            WPX_FRMR_SOCKET_PDH_Get_CAS_State (flexmux_id, client_port_id,
                                               &pCAS_IsEnabled);
        WTI_FlexmuxCheckStatus ("WPX_FRMR_SOCKET_PDH_Get_CAS_State",
                                status, __LINE__);
        if (pCAS_IsEnabled != 0)
        {
            WTI_FlexmuxTerminateOnError
                ("WPX_FRMR_SOCKET_PDH_Get_CAS_State != 0\n", 0);
        }
#endif

    }
    else if (type == WT_FLEXMUX_CONNECTION_TYPE_E1_FRAMED_CAS)
    {
        status =
            WPX_FRMR_SOCKET_PDH_CreateFacility (0, flexmux_id,
                                                client_port_id,
                                                WPX_UFE_FRAMER_CLIENT_E1);
        WTI_FlexmuxCheckStatus ("WPX_FRMR_SOCKET_PDH_CreateFacility",
                                status, __LINE__);

        status =
            WPX_FRMR_SOCKET_PDH_SetFraming (0, flexmux_id, client_port_id,
                                            WPX_UFE_FRAMER_CLIENT_PCM31CRC);
        WTI_FlexmuxCheckStatus ("WPX_FRMR_SOCKET_PDH_SetFraming", status,
                                __LINE__);

#if WTI_FLEXMUX_DBG_MODE
        U8 pFraming;

        status =
            WPX_FRMR_SOCKET_PDH_GetFraming (flexmux_id, client_port_id,
                                            &pFraming);
        WTI_FlexmuxCheckStatus ("WPX_FRMR_SOCKET_PDH_GetFraming", status,
                                __LINE__);
        if (pFraming != WPX_UFE_FRAMER_CLIENT_PCM31CRC)
        {
            WTI_FlexmuxTerminateOnError
                ("WPX_FRMR_SOCKET_PDH_GetFraming != WPX_UFE_FRAMER_CLIENT_PCM31CRC\n",
                 0);
        }
#endif

        status =
            WPX_FRMR_SOCKET_PDH_SetMapping (0, flexmux_id, client_port_id,
                                            WPX_UFE_FRAMER_CLIENT_MAPPING_BIT_ASYNC);
        WTI_FlexmuxCheckStatus ("WPX_FRMR_SOCKET_PDH_SetMapping", status,
                                __LINE__);

#if WTI_FLEXMUX_DBG_MODE
        U8 pMapping;

        status =
            WPX_FRMR_SOCKET_PDH_GetMapping (flexmux_id, client_port_id,
                                            &pMapping);
        WTI_FlexmuxCheckStatus ("WPX_FRMR_SOCKET_PDH_GetMapping", status,
                                __LINE__);
        if (pMapping != WPX_UFE_FRAMER_CLIENT_MAPPING_BIT_ASYNC)
        {
            WTI_FlexmuxTerminateOnError
                ("WPX_FRMR_SOCKET_PDH_GetMapping != WPX_UFE_FRAMER_CLIENT_MAPPING_BIT_ASYNC\n",
                 0);
        }
#endif

        status =
            WPX_FRMR_SOCKET_PDH_EnableCAS (0, flexmux_id, client_port_id);
        WTI_FlexmuxCheckStatus ("WPX_FRMR_SOCKET_PDH_DisableCAS", status,
                                __LINE__);

#if WTI_FLEXMUX_DBG_MODE
        U8 pCAS_IsEnabled;

        status =
            WPX_FRMR_SOCKET_PDH_Get_CAS_State (flexmux_id, client_port_id,
                                               &pCAS_IsEnabled);
        WTI_FlexmuxCheckStatus ("WPX_FRMR_SOCKET_PDH_Get_CAS_State",
                                status, __LINE__);
        if (pCAS_IsEnabled != 1)
        {
            WTI_FlexmuxTerminateOnError
                ("WPX_FRMR_SOCKET_PDH_Get_CAS_State != 1\n", 0);
        }
#endif

    }
    else if (type == WT_FLEXMUX_CONNECTION_TYPE_T1_UNFRAMED)
    {
        status =
            WPX_FRMR_SOCKET_PDH_CreateFacility (0, flexmux_id,
                                                client_port_id,
                                                WPX_UFE_FRAMER_CLIENT_T1);
        WTI_FlexmuxCheckStatus ("WPX_FRMR_SOCKET_PDH_CreateFacility",
                                status, __LINE__);

        status =
            WPX_FRMR_SOCKET_PDH_SetFraming (0, flexmux_id, client_port_id,
                                            WPX_UFE_FRAMER_CLIENT_UNFRAMED);
        WTI_FlexmuxCheckStatus ("WPX_FRMR_SOCKET_PDH_SetFraming", status,
                                __LINE__);

#if WTI_FLEXMUX_DBG_MODE
        U8 pFraming;

        status =
            WPX_FRMR_SOCKET_PDH_GetFraming (flexmux_id, client_port_id,
                                            &pFraming);
        WTI_FlexmuxCheckStatus ("WPX_FRMR_SOCKET_PDH_GetFraming", status,
                                __LINE__);
        if (pFraming != WPX_UFE_FRAMER_CLIENT_UNFRAMED)
        {
            WTI_FlexmuxTerminateOnError
                ("WPX_FRMR_SOCKET_PDH_GetFraming != WPX_UFE_FRAMER_CLIENT_UNFRAMED\n",
                 0);
        }
#endif

        status =
            WPX_FRMR_SOCKET_PDH_SetMapping (0, flexmux_id, client_port_id,
                                            WPX_UFE_FRAMER_CLIENT_MAPPING_BIT_ASYNC);
        WTI_FlexmuxCheckStatus ("WPX_FRMR_SOCKET_PDH_SetMapping", status,
                                __LINE__);

#if WTI_FLEXMUX_DBG_MODE
        U8 pMapping;

        status =
            WPX_FRMR_SOCKET_PDH_GetMapping (flexmux_id, client_port_id,
                                            &pMapping);
        WTI_FlexmuxCheckStatus ("WPX_FRMR_SOCKET_PDH_GetMapping", status,
                                __LINE__);
        if (pMapping != WPX_UFE_FRAMER_CLIENT_MAPPING_BIT_ASYNC)
        {
            WTI_FlexmuxTerminateOnError
                ("WPX_FRMR_SOCKET_PDH_GetMapping != WPX_UFE_FRAMER_CLIENT_MAPPING_BIT_ASYNC\n",
                 0);
        }
#endif

    }
    else if (type == WT_FLEXMUX_CONNECTION_TYPE_T1_FRAMED)
    {
        status =
            WPX_FRMR_SOCKET_PDH_CreateFacility (0, flexmux_id,
                                                client_port_id,
                                                WPX_UFE_FRAMER_CLIENT_T1);
        WTI_FlexmuxCheckStatus ("WPX_FRMR_SOCKET_PDH_CreateFacility",
                                status, __LINE__);

        status =
            WPX_FRMR_SOCKET_PDH_SetFraming (0, flexmux_id, client_port_id,
                                            WPX_UFE_FRAMER_CLIENT_FRAMING_ESF);
        WTI_FlexmuxCheckStatus ("WPX_FRMR_SOCKET_PDH_SetFraming", status,
                                __LINE__);

#if WTI_FLEXMUX_DBG_MODE
        U8 pFraming;

        status =
            WPX_FRMR_SOCKET_PDH_GetFraming (flexmux_id, client_port_id,
                                            &pFraming);
        WTI_FlexmuxCheckStatus ("WPX_FRMR_SOCKET_PDH_GetFraming", status,
                                __LINE__);
        if (pFraming != WPX_UFE_FRAMER_CLIENT_FRAMING_SF_D4)
        {
            WTI_FlexmuxTerminateOnError
                ("WPX_FRMR_SOCKET_PDH_GetFraming != WPX_UFE_FRAMER_CLIENT_FRAMING_SF_D4\n",
                 0);
        }
#endif

        status =
            WPX_FRMR_SOCKET_PDH_SetMapping (0, flexmux_id, client_port_id,
                                            WPX_UFE_FRAMER_CLIENT_MAPPING_BIT_ASYNC);
        WTI_FlexmuxCheckStatus ("WPX_FRMR_SOCKET_PDH_SetMapping", status,
                                __LINE__);

#if WTI_FLEXMUX_DBG_MODE
        U8 pMapping;

        status =
            WPX_FRMR_SOCKET_PDH_GetMapping (flexmux_id, client_port_id,
                                            &pMapping);
        WTI_FlexmuxCheckStatus ("WPX_FRMR_SOCKET_PDH_GetMapping", status,
                                __LINE__);
        if (pMapping != WPX_UFE_FRAMER_CLIENT_MAPPING_BIT_ASYNC)
        {
            WTI_FlexmuxTerminateOnError
                ("WPX_FRMR_SOCKET_PDH_GetMapping != WPX_UFE_FRAMER_CLIENT_MAPPING_BIT_ASYNC\n",
                 0);
        }
#endif

        status =
            WPX_FRMR_SOCKET_PDH_DisableCAS (0, flexmux_id, client_port_id);
        WTI_FlexmuxCheckStatus ("WPX_FRMR_SOCKET_PDH_DisableCAS", status,
                                __LINE__);

#if WTI_FLEXMUX_DBG_MODE
        U8 pCAS_IsEnabled;

        status =
            WPX_FRMR_SOCKET_PDH_Get_CAS_State (flexmux_id, client_port_id,
                                               &pCAS_IsEnabled);
        WTI_FlexmuxCheckStatus ("WPX_FRMR_SOCKET_PDH_Get_CAS_State",
                                status, __LINE__);
        if (pCAS_IsEnabled != 0)
        {
            WTI_FlexmuxTerminateOnError
                ("WPX_FRMR_SOCKET_PDH_Get_CAS_State != 0\n", 0);
        }
#endif
    }
    else if (type == WT_FLEXMUX_CONNECTION_TYPE_T1_FRAMED_CAS)
    {
        status =
            WPX_FRMR_SOCKET_PDH_CreateFacility (0, flexmux_id,
                                                client_port_id,
                                                WPX_UFE_FRAMER_CLIENT_T1);
        WTI_FlexmuxCheckStatus ("WPX_FRMR_SOCKET_PDH_CreateFacility",
                                status, __LINE__);

        status =
            WPX_FRMR_SOCKET_PDH_SetFraming (0, flexmux_id, client_port_id,
                                            WPX_UFE_FRAMER_CLIENT_FRAMING_ESF);
        WTI_FlexmuxCheckStatus ("WPX_FRMR_SOCKET_PDH_SetFraming", status,
                                __LINE__);

#if WTI_FLEXMUX_DBG_MODE
        U8 pFraming;

        status =
            WPX_FRMR_SOCKET_PDH_GetFraming (flexmux_id, client_port_id,
                                            &pFraming);
        WTI_FlexmuxCheckStatus ("WPX_FRMR_SOCKET_PDH_GetFraming", status,
                                __LINE__);
        if (pFraming != WPX_UFE_FRAMER_CLIENT_FRAMING_ESF)
        {
            WTI_FlexmuxTerminateOnError
                ("WPX_FRMR_SOCKET_PDH_GetFraming != WPX_UFE_FRAMER_CLIENT_FRAMING_ESF\n",
                 0);
        }
#endif

        status =
            WPX_FRMR_SOCKET_PDH_SetMapping (0, flexmux_id, client_port_id,
                                            WPX_UFE_FRAMER_CLIENT_MAPPING_BIT_ASYNC);
        WTI_FlexmuxCheckStatus ("WPX_FRMR_SOCKET_PDH_SetMapping", status,
                                __LINE__);

#if WTI_FLEXMUX_DBG_MODE
        U8 pMapping;

        status =
            WPX_FRMR_SOCKET_PDH_GetMapping (flexmux_id, client_port_id,
                                            &pMapping);
        WTI_FlexmuxCheckStatus ("WPX_FRMR_SOCKET_PDH_GetMapping", status,
                                __LINE__);
        if (pMapping != WPX_UFE_FRAMER_CLIENT_MAPPING_BIT_ASYNC)
        {
            WTI_FlexmuxTerminateOnError
                ("WPX_FRMR_SOCKET_PDH_GetMapping != WPX_UFE_FRAMER_CLIENT_MAPPING_BIT_ASYNC\n",
                 0);
        }
#endif

        status =
            WPX_FRMR_SOCKET_PDH_EnableCAS (0, flexmux_id, client_port_id);
        WTI_FlexmuxCheckStatus ("WPX_FRMR_SOCKET_PDH_EnableCAS", status,
                                __LINE__);

#if WTI_FLEXMUX_DBG_MODE
        U8 pCAS_IsEnabled;

        status =
            WPX_FRMR_SOCKET_PDH_Get_CAS_State (flexmux_id, client_port_id,
                                               &pCAS_IsEnabled);
        WTI_FlexmuxCheckStatus ("WPX_FRMR_SOCKET_PDH_Get_CAS_State",
                                status, __LINE__);
        if (pCAS_IsEnabled != 1)
        {
            WTI_FlexmuxTerminateOnError
                ("WPX_FRMR_SOCKET_PDH_Get_CAS_State != 1\n", 0);
        }
#endif
    }
    else
    {
        WTI_FlexmuxTerminateOnError ("Connection type is not supported !",
                                     0);
    }

    /* configure the line end point in SONET or SDH mode */
    if (flexmux_reg[flexmux_id].flexmux_mode ==
        WPX_UFE_FRAMER_DEVICE_MODE_SONET)
    {
        WT_ufe_line_sonet_params *sonet_line_params =
            (WT_ufe_line_sonet_params *) ufe_line_params;
        U8 TX_V5_SignalLabel = 0x2, EX_V5_SignalLabel = 0x2;
        WP_CHAR *TX_J2_TraceString = "WG LP-TRACE", *EX_J2_TraceString =
            "WG LP-TRACE";

        line_end_point.u.SONET.sts12 = sonet_line_params->sts12;
        line_end_point.u.SONET.sts3 = sonet_line_params->sts3;
        line_end_point.u.SONET.sts1 = sonet_line_params->sts1;
        line_end_point.u.SONET.vt_group = sonet_line_params->vt_group;
        line_end_point.u.SONET.vt = sonet_line_params->vt;

#if WTI_FLEXMUX_DBG_MODE
        printf ("WTI_FlexmuxConnectionCreate: connecting client_port_id %d to line_port_id %d\n",
                client_port_id, line_port_id);
        printf ("sts12 %d sts3 %d sts1 %d vt_group %d vt %d\n",
                sonet_line_params->sts12, sonet_line_params->sts3,
                sonet_line_params->sts1, sonet_line_params->vt_group,
                sonet_line_params->vt);
#endif

        if (sonet_line_params->transfer_type == WUFE_SONET_TYPE_E1)
            line_end_point.TransferType =
                WPX_UFE_FRAMER_WUFE_SONET_TYPE_VT2;
        else if (sonet_line_params->transfer_type == WUFE_SONET_TYPE_T1)
            line_end_point.TransferType =
                WPX_UFE_FRAMER_WUFE_SONET_TYPE_VT1_5;
        else
            WTI_FlexmuxTerminateOnError ("Invalid line transfer type", 0);

        status =
            WPX_FRMR_SONET_SDH_CreateFacility (0, flexmux_id,
                                               &line_end_point);
        WTI_FlexmuxCheckStatus
            ("WPX_FRMR_SONET_SDH_CreateFacility (Vt1.5 or Vt2)", status,
             __LINE__);

        /* set the transmitted and expected V5 values */
        status =
            WPX_FRMR_SONET_SDH_LO_PATH_V5_SigLabel_SetTX (0, flexmux_id,
                                                          &line_end_point,
                                                          TX_V5_SignalLabel);
        WTI_FlexmuxCheckStatus
            ("WPX_FRMR_SONET_SDH_LO_PATH_V5_SigLabel_SetTX", status,
             __LINE__);

#if WTI_FLEXMUX_DBG_MODE
        U8 pTX_V5_SignalLabel;

        status =
            WPX_FRMR_SONET_SDH_LO_PATH_V5_SigLabel_GetTX (flexmux_id,
                                                          &line_end_point,
                                                          &pTX_V5_SignalLabel);
        WTI_FlexmuxCheckStatus
            ("WPX_FRMR_SONET_SDH_LO_PATH_V5_SigLabel_GetTX", status,
             __LINE__);
        if (pTX_V5_SignalLabel != TX_V5_SignalLabel)
        {
            WTI_FlexmuxTerminateOnError
                ("WPX_FRMR_SONET_SDH_LO_PATH_V5_SigLabel_GetTX != TX_V5_SignalLabel\n",
                 0);
        }
#endif

        status =
            WPX_FRMR_SONET_SDH_LO_PATH_V5_SigLabel_SetEX (0, flexmux_id,
                                                          &line_end_point,
                                                          EX_V5_SignalLabel);
        WTI_FlexmuxCheckStatus
            ("WPX_FRMR_SONET_SDH_LO_PATH_V5_SigLabel_SetEX", status,
             __LINE__);

#if WTI_FLEXMUX_DBG_MODE
        U8 pEX_V5_SignalLabel;

        status =
            WPX_FRMR_SONET_SDH_LO_PATH_V5_SigLabel_GetEX (flexmux_id,
                                                          &line_end_point,
                                                          &pEX_V5_SignalLabel);
        WTI_FlexmuxCheckStatus
            ("WPX_FRMR_SONET_SDH_LO_PATH_V5_SigLabel_GetEX", status,
             __LINE__);
        if (pEX_V5_SignalLabel != EX_V5_SignalLabel)
        {
            WTI_FlexmuxTerminateOnError
                ("WPX_FRMR_SONET_SDH_LO_PATH_V5_SigLabel_GetEX != EX_V5_SignalLabel\n",
                 0);
        }
#endif

        status =
            WPX_FRMR_SONET_SDH_LO_PATH_J2_SetTX (0, flexmux_id,
                                                 &line_end_point,
                                                 TX_J2_TraceString);
        WTI_FlexmuxCheckStatus ("WPX_FRMR_SONET_SDH_LO_PATH_J2_SetTX",
                                status, __LINE__);

#if WTI_FLEXMUX_DBG_MODE
        WP_CHAR *pTX_J2_TraceString;

        status =
            WPX_FRMR_SONET_SDH_LO_PATH_J2_GetTX (flexmux_id,
                                                 &line_end_point,
                                                 pTX_J2_TraceString);
        WTI_FlexmuxCheckStatus ("WPX_FRMR_SONET_SDH_LO_PATH_J2_SetTX",
                                status, __LINE__);
        if (*pTX_J2_TraceString != *TX_J2_TraceString)
        {
            WTI_FlexmuxTerminateOnError
                ("WPX_FRMR_SONET_SDH_LO_PATH_J2_GetTX != TX_J2_TraceString\n",
                 0);
        }
#endif

        status =
            WPX_FRMR_SONET_SDH_LO_PATH_J2_SetEX (0, flexmux_id,
                                                 &line_end_point,
                                                 EX_J2_TraceString);
        WTI_FlexmuxCheckStatus ("WPX_FRMR_SONET_SDH_LO_PATH_J2_SetEX",
                                status, __LINE__);

#if WTI_FLEXMUX_DBG_MODE
        WP_CHAR *pEX_J2_TraceString;

        status =
            WPX_FRMR_SONET_SDH_LO_PATH_J2_GetEX (flexmux_id,
                                                 &line_end_point,
                                                 pEX_J2_TraceString);
        WTI_FlexmuxCheckStatus ("WPX_FRMR_SONET_SDH_LO_PATH_J2_GetEX",
                                status, __LINE__);
        if (*pEX_J2_TraceString != *EX_J2_TraceString)
        {
            WTI_FlexmuxTerminateOnError
                ("WPX_FRMR_SONET_SDH_LO_PATH_J2_GetEX != EX_J2_TraceString\n",
                 0);
        }
#endif

        if (sonet_line_params->clock_rec_line_params != NULL)
        {
            tx_clk_rec_enable =
                sonet_line_params->clock_rec_line_params->
                tx_clk_rec_enable;
        }
    }
    else if (flexmux_reg[flexmux_id].flexmux_mode ==
             WPX_UFE_FRAMER_DEVICE_MODE_SDH)
    {
        WT_ufe_line_sdh_params *sdh_line_params =
            (WT_ufe_line_sdh_params *) ufe_line_params;
        U8 TX_V5_SignalLabel = 0x2, EX_V5_SignalLabel = 0x2;
        WP_CHAR *TX_J2_TraceString = "WG LP-TRACE", *EX_J2_TraceString =
            "WG LP-TRACE";

        /* create the VC3/4 facility */
        line_end_point.u.SDH.stm4 = sdh_line_params->stm4;
        line_end_point.u.SDH.stm1 = sdh_line_params->stm1;
        line_end_point.u.SDH.stm0 = sdh_line_params->stm0;
        line_end_point.u.SDH.tug2 = sdh_line_params->tug2;
        line_end_point.u.SDH.tu = sdh_line_params->tu;

#if WTI_FLEXMUX_DBG_MODE
        printf ("connecting client_port_id %d to line_port_id %d\n",
                client_port_id, line_port_id);
        printf ("stm4 %d stm1 %d stm0 %d tug2 %d tu %d\n",
                sdh_line_params->stm4, sdh_line_params->stm1,
                sdh_line_params->stm0, sdh_line_params->tug2,
                sdh_line_params->tu);
#endif
        if (sdh_line_params->transfer_type == WUFE_SDH_TYPE_E1)
        {
            line_end_point.TransferType =
                WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC12;
        }

        else if (sdh_line_params->transfer_type == WUFE_SDH_TYPE_T1)
        {
            line_end_point.TransferType =
                WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC11;
        }
        else
            WTI_FlexmuxTerminateOnError ("Invalid line transfer type", 0);

#if WTI_FLEXMUX_ENABLE_PERFORMANCE_MONITORING
        lo_transfer_type_in_use = line_end_point.TransferType;
#endif

        if (sdh_line_params->clock_rec_line_params != NULL)
        {
            tx_clk_rec_enable =
                sdh_line_params->clock_rec_line_params->tx_clk_rec_enable;
        }

        /* create the line port VC11\VC12 facility */
        status =
            WPX_FRMR_SONET_SDH_CreateFacility (0, flexmux_id,
                                               &line_end_point);
        WTI_FlexmuxCheckStatus
            ("WPX_FRMR_SONET_SDH_CreateFacility (VC11 or VC12)", status,
             __LINE__);

        /* set the transmitted and expected V5 values */
        status =
            WPX_FRMR_SONET_SDH_LO_PATH_V5_SigLabel_SetTX (0, flexmux_id,
                                                          &line_end_point,
                                                          TX_V5_SignalLabel);
        WTI_FlexmuxCheckStatus
            ("WPX_FRMR_SONET_SDH_LO_PATH_V5_SigLabel_SetTX", status,
             __LINE__);

#if WTI_FLEXMUX_DBG_MODE
        U8 pTX_V5_SignalLabel;

        status =
            WPX_FRMR_SONET_SDH_LO_PATH_V5_SigLabel_GetTX (flexmux_id,
                                                          &line_end_point,
                                                          &pTX_V5_SignalLabel);
        WTI_FlexmuxCheckStatus
            ("WPX_FRMR_SONET_SDH_LO_PATH_V5_SigLabel_GetTX", status,
             __LINE__);
        if (pTX_V5_SignalLabel != TX_V5_SignalLabel)
        {
            WTI_FlexmuxTerminateOnError
                ("WPX_FRMR_SONET_SDH_LO_PATH_V5_SigLabel_GetTX != TX_V5_SignalLabel\n",
                 0);
        }
#endif

        status =
            WPX_FRMR_SONET_SDH_LO_PATH_V5_SigLabel_SetEX (0, flexmux_id,
                                                          &line_end_point,
                                                          EX_V5_SignalLabel);
        WTI_FlexmuxCheckStatus
            ("WPX_FRMR_SONET_SDH_LO_PATH_V5_SigLabel_SetEX", status,
             __LINE__);

#if WTI_FLEXMUX_DBG_MODE
        U8 pEX_V5_SignalLabel;

        status =
            WPX_FRMR_SONET_SDH_LO_PATH_V5_SigLabel_GetEX (flexmux_id,
                                                          &line_end_point,
                                                          &pEX_V5_SignalLabel);
        WTI_FlexmuxCheckStatus
            ("WPX_FRMR_SONET_SDH_LO_PATH_V5_SigLabel_GetEX", status,
             __LINE__);
        if (pEX_V5_SignalLabel != EX_V5_SignalLabel)
        {
            WTI_FlexmuxTerminateOnError
                ("WPX_FRMR_SONET_SDH_LO_PATH_V5_SigLabel_GetEX != EX_V5_SignalLabel\n",
                 0);
        }
#endif
        status =
            WPX_FRMR_SONET_SDH_LO_PATH_J2_SetTX (0, flexmux_id,
                                                 &line_end_point,
                                                 TX_J2_TraceString);
        WTI_FlexmuxCheckStatus ("WPX_FRMR_SONET_SDH_LO_PATH_J2_SetTX",
                                status, __LINE__);

#if WTI_FLEXMUX_DBG_MODE
        WP_CHAR *pTX_J2_TraceString;

        status =
            WPX_FRMR_SONET_SDH_LO_PATH_J2_GetTX (flexmux_id,
                                                 &line_end_point,
                                                 pTX_J2_TraceString);
        WTI_FlexmuxCheckStatus ("WPX_FRMR_SONET_SDH_LO_PATH_J2_SetTX",
                                status, __LINE__);
        if (*pTX_J2_TraceString != *TX_J2_TraceString)
        {
            WTI_FlexmuxTerminateOnError
                ("WPX_FRMR_SONET_SDH_LO_PATH_J2_GetTX != TX_J2_TraceString\n",
                 0);
        }
#endif

        status =
            WPX_FRMR_SONET_SDH_LO_PATH_J2_SetEX (0, flexmux_id,
                                                 &line_end_point,
                                                 EX_J2_TraceString);
        WTI_FlexmuxCheckStatus ("WPX_FRMR_SONET_SDH_LO_PATH_J2_SetEX",
                                status, __LINE__);

#if WTI_FLEXMUX_DBG_MODE
        WP_CHAR *pEX_J2_TraceString;

        status =
            WPX_FRMR_SONET_SDH_LO_PATH_J2_GetEX (flexmux_id,
                                                 &line_end_point,
                                                 pEX_J2_TraceString);
        WTI_FlexmuxCheckStatus ("WPX_FRMR_SONET_SDH_LO_PATH_J2_GetEX",
                                status, __LINE__);
        if (*pEX_J2_TraceString != *EX_J2_TraceString)
        {
            WTI_FlexmuxTerminateOnError
                ("WPX_FRMR_SONET_SDH_LO_PATH_J2_GetEX != EX_J2_TraceString\n",
                 0);
        }
#endif
    }

    /* create the connection between the client port and the line port */
    status =
        WPX_FRMR_CONNECTIONS_AddDropLineToSocketCreate (0, flexmux_id,
                                                        &line_end_point,
                                                        client_port_id);
    WTI_FlexmuxCheckStatus
        ("WPX_FRMR_CONNECTIONS_AddDropLineToSocketCreate", status,
         __LINE__);

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
        printf ("WPX_UFE_FRAMER_TRANSMIT_TIMING_SONET_TIMING\n");
#endif
#else /* NOT WTI_CESOP_REGRESSION_TEST */
        /* Framer uses justification commands from Winpath via UFE */
#if WTI_FLEXMUX_LINE_TIMING_CT_MODE_ENABLE
        line_timing = WPX_UFE_FRAMER_TRANSMIT_TIMING_INPUT_REFERENCE_CLOCK;
#else
        line_timing = WPX_UFE_FRAMER_TRANSMIT_TIMING_WINPATH;
#endif /* WTI_FLEXMUX_LINE_TIMING_CT_MODE_ENABLE */
#endif

#if WTI_FLEXMUX_DBG_MODE
        printf ("WPX_UFE_FRAMER_TRANSMIT_TIMING_WINPATH\n");
#endif
    }
    else
    {
#if(WTI_CESOP_REGRESSION_TEST)
        /* Working in OC12/48 loopback the timing should be CT or SDH/SONET clock */
        line_timing = WPX_UFE_FRAMER_TRANSMIT_TIMING_SONET_TIMING;
#if WTI_FLEXMUX_DBG_MODE
        printf ("WPX_UFE_FRAMER_TRANSMIT_TIMING_SONET_TIMING\n");
#endif
#else
        /* CR is not enable and working with TDM external tester - use PDH loop timing */
        line_timing = WPX_UFE_FRAMER_TRANSMIT_TIMING_RX_PATH;   /* Loop timing */

#if WTI_FLEXMUX_DBG_MODE
        printf ("WPX_UFE_FRAMER_TRANSMIT_TIMING_RX_PATH\n");
#endif
#endif

    }

    /* Clock rec Rx timestamps */
    if (tx_clk_rec_enable)
    {
        status =
            WPX_FRMR_SOCKET_PDH_EnableTimeStamp (0, flexmux_id,
                                                 client_port_id,
                                                 WTI_FLEXMUX_CLOCK_REC_GAIN_SLOW);
        WTI_FlexmuxCheckStatus ("WPX_FRMR_SOCKET_PDH_EnableTimeStamp",
                                status, __LINE__);
    }

    /* Set the PDH Tx line timing  */
    status =
        WPX_FRMR_SOCKET_PDH_SetClkRec (0, flexmux_id, client_port_id,
                                       line_timing);
    WTI_FlexmuxCheckStatus ("WPX_FRMR_SOCKET_PDH_SetClkRec", status,
                            __LINE__);

    /* Set line or system loopback */
    status =
        WTI_FlexmuxEnableLoopback (flexmux_id, line_port_id,
                                   client_port_id,
                                   WT_FLEXMUX_NONE_LOOPBACK);
    WTI_FlexmuxCheckStatus ("WTI_FlexmuxEnableLoopback", status, __LINE__);
#if WTI_FLEXMUX_DBG_MODE
    status =
        WTI_FlexmuxGetLoopbackState (flexmux_id, line_port_id,
                                     client_port_id,
                                     WT_FLEXMUX_NONE_LOOPBACK);
    WTI_FlexmuxCheckStatus ("WTI_FlexmuxGetLoopbackState", status,
                            __LINE__);
#endif

    return 0;
}


WP_S32 WTI_FlexmuxClientPortIndexGet (WP_U32 ufe_line_index,
                                      WUFE_line_transf_type transfer_type)
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
            return ((spe_num * 28) + (tug2_num * 4) + offset_within_tug2);
        }
    case (WUFE_SONET_TYPE_T1):
    case (WUFE_SDH_TYPE_T1):
        {
            return ufe_line_index;
        }
    default:
        printf ("Unsupported transfer_type %d\n", transfer_type);
        exit (-1);
    }
    return 0;
}


WP_S32 WTI_FlexmuxLinePortIndexGet (WP_U32 ufe_line_index,
                                    WP_U32 flexmux_id)
{
    WP_U32 result = -1;

    if (flexmux_reg[flexmux_id].flexmux_build_personality !=
        WPX_UFE_FRAMER_BUILD_PERSONALITY_4_PLUS_4_OC3_OC12
        && flexmux_reg[flexmux_id].flexmux_build_personality !=
        WPX_UFE_FRAMER_BUILD_PERSONALITY_8_PLUS_8_OC3
        && flexmux_reg[flexmux_id].flexmux_build_personality !=
        WPX_UFE_FRAMER_BUILD_PERSONALITY_1_PLUS_1_OC3_OC12
        && flexmux_reg[flexmux_id].flexmux_build_personality !=
        WPX_UFE_FRAMER_BUILD_PERSONALITY_2_PLUS_2_OC3
        && flexmux_reg[flexmux_id].flexmux_build_personality !=
        WPX_UFE_FRAMER_BUILD_PERSONALITY_4_PLUS_4_OC3)
        WTI_FlexmuxTerminateOnError ("Invalid flexmux_build_personality !",
                                     0);

    /* In V6 protection is used and even ports are the working ports while odd
       ports are the protected ports */
    if (flexmux_reg[flexmux_id].flexmux_build_personality ==
        WPX_UFE_FRAMER_BUILD_PERSONALITY_4_PLUS_4_OC3_OC12)
    {
        result = (ufe_line_index / 336);
        /* currently only 4XOC12 ports are supported */
        if (result > 3)
        {
            result = -1;
        }
        else
        {
            result = result * 2;    /* Multipication by 2 in order to skip the odd protection lines */
        }
    }
    else if (flexmux_reg[flexmux_id].flexmux_build_personality ==
             WPX_UFE_FRAMER_BUILD_PERSONALITY_8_PLUS_8_OC3)
    {
        result = (ufe_line_index / 336);

        if (result > 7)
        {
            result = -1;
        }
        else
        {
            result = result * 2;    /* Multipication by 2 in order to skip the odd protection lines */
        }
    }
    /* In S6 protection is not used - port #0 is the only working line port */
    else if ((flexmux_reg[flexmux_id].flexmux_build_personality ==
              WPX_UFE_FRAMER_BUILD_PERSONALITY_1_PLUS_1_OC3_OC12)
             || (flexmux_reg[flexmux_id].flexmux_build_personality ==
                 WPX_UFE_FRAMER_BUILD_PERSONALITY_2_PLUS_2_OC3)
             || (flexmux_reg[flexmux_id].flexmux_build_personality ==
                 WPX_UFE_FRAMER_BUILD_PERSONALITY_4_PLUS_4_OC3))
    {
        /* In case a single OC12 port is used, then line port index is always 0 */
        if (flexmux_reg[flexmux_id].flexmux_hw_config ==
            WT_FLEXMUX_UFE_412_1_PORT_OC12)
        {
            result = 0;
        }
        /* In case 4 OC3 ports are used, then line port index changes between 0-3 */
        else if ((flexmux_reg[flexmux_id].flexmux_hw_config ==
                  WT_FLEXMUX_UFE_412_4_PORTS_OC3)
                 || (flexmux_reg[flexmux_id].flexmux_hw_config ==
                     WT_FLEXMUX_UFE_412_2_PORTS_OC3)
                 || flexmux_reg[flexmux_id].flexmux_hw_config ==
                 WT_FLEXMUX_UFE_412_4_PORTS_OC3_CARD_PROTECTION_MASTER
                 || flexmux_reg[flexmux_id].flexmux_hw_config ==
                 WT_FLEXMUX_UFE_412_4_PORTS_OC3_CARD_PROTECTION_SLAVE)
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


WP_U8 WTI_FlexmuxEnableLoopback (U8 flexmux_id, WP_U32 line_port_id,
                                 WP_U32 client_port_id,
                                 WTI_flexmux_loopback_mode loopback_type)
{
    WP_U8 status = WPX_UFE_FRAMER_OK;

    switch (loopback_type)
    {
    case WT_FLEXMUX_NONE_LOOPBACK:
        break;
    case WT_FLEXMUX_SHALLOW_LINE_LOOPBACK:
        {
            status =
                (WP_U8)
                WPX_FRMR_SONET_SDH_PORT_DIAG_LpbkEnableShallowLine (0,
                                                                    flexmux_id,
                                                                    line_port_id);
            WTI_FlexmuxCheckStatus
                ("WPX_FRMR_SONET_SDH_PORT_DIAG_LpbkEnableShallowLine",
                 status, __LINE__);

            break;
        }
    case WT_FLEXMUX_DEEP_LINE_LOOPBACK:
        {
            status =
                (WP_U8) WPX_FRMR_SOCKET_PDH_DIAG_LpbkEnableDeepLine (0,
                                                                     flexmux_id,
                                                                     client_port_id);
            WTI_FlexmuxCheckStatus
                ("WPX_FRMR_SOCKET_PDH_DIAG_LpbkEnableDeepLine", status,
                 __LINE__);

            break;
        }
    case WT_FLEXMUX_SHALLOW_SYSTEM_LOOPBACK:
        {
            status =
                (WP_U8)
                WPX_FRMR_SOCKET_PDH_DIAG_LpbkEnableShallowSystem (0,
                                                                  flexmux_id,
                                                                  client_port_id);
            WTI_FlexmuxCheckStatus
                ("WPX_FRMR_SOCKET_PDH_DIAG_LpbkEnableShallowSystem",
                 status, __LINE__);

            break;
        }
    case WT_FLEXMUX_DEEP_SYSTEM_LOOPBACK:
        {
            status =
                (WP_U8)
                WPX_FRMR_SONET_SDH_PORT_DIAG_LpbkEnableDeepSystem (0,
                                                                   flexmux_id,
                                                                   line_port_id);
            WTI_FlexmuxCheckStatus
                ("WPX_FRMR_SONET_SDH_PORT_DIAG_LpbkEnableDeepSystem",
                 status, __LINE__);

            break;
        }
    default:
        {
            printf ("Unsupported loopback_type %d\n", loopback_type);
            exit (-1);
        }
    }

    return status;
}

WP_U8 WTI_FlexmuxDisableLoopback (U8 flexmux_id, WP_U32 line_port_id,
                                  WP_U32 client_port_id,
                                  WTI_flexmux_loopback_mode loopback_type)
{
    WP_U8 status = WPX_UFE_FRAMER_OK;

    switch (loopback_type)
    {
    case WT_FLEXMUX_NONE_LOOPBACK:
        break;
    case WT_FLEXMUX_SHALLOW_LINE_LOOPBACK:
        {
            status =
                (WP_U8)
                WPX_FRMR_SONET_SDH_PORT_DIAG_LpbkDisableShallowLine (0,
                                                                     flexmux_id,
                                                                     line_port_id);
            WTI_FlexmuxCheckStatus
                ("WPX_FRMR_SONET_SDH_PORT_DIAG_LpbkDisableShallowLine",
                 status, __LINE__);

            break;
        }
    case WT_FLEXMUX_DEEP_LINE_LOOPBACK:
        {
            status =
                (WP_U8) WPX_FRMR_SOCKET_PDH_DIAG_LpbkDisableDeepLine (0,
                                                                      flexmux_id,
                                                                      client_port_id);
            WTI_FlexmuxCheckStatus
                ("WPX_FRMR_SOCKET_PDH_DIAG_LpbkDisableDeepLine", status,
                 __LINE__);

            break;
        }
    case WT_FLEXMUX_SHALLOW_SYSTEM_LOOPBACK:
        {
            status =
                (WP_U8)
                WPX_FRMR_SOCKET_PDH_DIAG_LpbkDisableShallowSystem (0,
                                                                   flexmux_id,
                                                                   client_port_id);
            WTI_FlexmuxCheckStatus
                ("WPX_FRMR_SOCKET_PDH_DIAG_LpbkDisableShallowSystem",
                 status, __LINE__);

            break;
        }
    case WT_FLEXMUX_DEEP_SYSTEM_LOOPBACK:
        {
            status =
                (WP_U8)
                WPX_FRMR_SONET_SDH_PORT_DIAG_LpbkDisableDeepSystem (0,
                                                                    flexmux_id,
                                                                    line_port_id);
            WTI_FlexmuxCheckStatus
                ("WPX_FRMR_SONET_SDH_PORT_DIAG_LpbkDisableDeepSystem",
                 status, __LINE__);

            break;
        }
    default:
        {
            printf ("Unsupported loopback_type %d\n", loopback_type);
            exit (-1);
        }
    }
    return status;
}

WP_U8 WTI_FlexmuxGetLoopbackState (U8 flexmux_id, WP_U32 line_port_id,
                                   WP_U32 client_port_id,
                                   WTI_flexmux_loopback_mode loopback_type)
{
    WP_U8 status = WPX_UFE_FRAMER_OK, loopback_enabled = 0;

    switch (loopback_type)
    {
    case WT_FLEXMUX_NONE_LOOPBACK:
        break;
    case WT_FLEXMUX_SHALLOW_LINE_LOOPBACK:
        {
            status =
                (WP_U8)
                WPX_FRMR_SONET_SDH_PORT_DIAG_LpbkGetShallowLineState (0,
                                                                      line_port_id,
                                                                      &loopback_enabled);
            WTI_FlexmuxCheckStatus
                ("WPX_FRMR_SONET_SDH_PORT_DIAG_LpbkGetShallowLineState",
                 status, __LINE__);
            if (loopback_enabled)
            {
                printf ("SHALLOW_LINE");
            }

            break;
        }
    case WT_FLEXMUX_DEEP_LINE_LOOPBACK:
        {
            status =
                (WP_U8) WPX_FRMR_SOCKET_PDH_DIAG_LpbkGetDeepLineState (0,
                                                                       client_port_id,
                                                                       &loopback_enabled);
            WTI_FlexmuxCheckStatus
                ("WPX_FRMR_SOCKET_PDH_DIAG_LpbkGetDeepLineState", status,
                 __LINE__);
            if (loopback_enabled)
            {
                printf ("DEEP_LINE");
            }

            break;
        }
    case WT_FLEXMUX_SHALLOW_SYSTEM_LOOPBACK:
        {
            status =
                (WP_U8)
                WPX_FRMR_SOCKET_PDH_DIAG_LpbkGetShallowSystemState (0,
                                                                    client_port_id,
                                                                    &loopback_enabled);
            WTI_FlexmuxCheckStatus
                ("WPX_FRMR_SOCKET_PDH_DIAG_LpbkGetShallowSystemState",
                 status, __LINE__);
            if (loopback_enabled)
            {
                printf ("SHALLOW_SYSTEM");
            }

            break;
        }
    case WT_FLEXMUX_DEEP_SYSTEM_LOOPBACK:
        {
            status =
                (WP_U8)
                WPX_FRMR_SONET_SDH_PORT_DIAG_LpbkGetDeepSystemState (0,
                                                                     line_port_id,
                                                                     &loopback_enabled);
            WTI_FlexmuxCheckStatus
                ("WPX_FRMR_SONET_SDH_PORT_DIAG_LpbkGetDeepSystemState",
                 status, __LINE__);
            if (loopback_enabled)
            {
                printf ("DEEP_SYSTEM");
            }

            break;
        }
    default:
        {
            printf ("Unsupported loopback_type %d\n", loopback_type);
            exit (-1);
        }
    }
    if (loopback_enabled)
    {
        printf (" Loopback is enabled\n");
    }
    else
    {
        printf (" Loopback is not enabled\n");
    }

    return status;
}


void WTI_FlexmuxForceA (WP_U32 line_port_id)
{
    WP_U8 status = WPX_UFE_FRAMER_OK;

#if DEBUG
    WP_U8 force_side = 2;
    WP_time before, after;
    WP_U32 sec, usec;

    WPL_TimestampGet (&before);
#endif
    status = WPX_FRMR_SONET_SDH_PORT_ForceA (0, 0, line_port_id);
    WTI_FlexmuxCheckStatus ("WPX_FRMR_SONET_SDH_PORT_ForceA", status,
                            __LINE__);
#if DEBUG
    status =
        WPX_FRMR_SONET_SDH_PORT_GetForceState (0, line_port_id,
                                               &force_side);
    WTI_FlexmuxCheckStatus ("WPX_FRMR_SONET_SDH_PORT_GetForceState",
                            status, __LINE__);
    WPL_TimestampGet (&after);
    sec = (after.sec) - (before.sec);
    usec = (after.usec) - (before.usec);
    if (usec < 0)
    {
        sec = sec - 1;
        usec = usec + 1000000;
    }
    printf ("In line %d forcing port A took %d.%06d seconds\n",
            line_port_id, sec, usec);
    if (status == WPX_UFE_FRAMER_OK)
    {
        if (force_side == WPX_UFE_FRAMER_PROTECTION_FORCE_A)
        {
            printf
                ("From ForceA: In line %d traffic is received from port A\n",
                 line_port_id);
        }
        else if (force_side == WPX_UFE_FRAMER_PROTECTION_FORCE_B)
        {
            printf
                ("From ForceA: In line %d traffic is received from port B\n",
                 line_port_id);
        }
    }
#endif
}

void WTI_FlexmuxForceB (WP_U32 line_port_id)
{
    WP_U8 status = WPX_UFE_FRAMER_OK;

#if DEBUG
    WP_U8 force_side = 2;
    WP_time before, after;
    WP_U32 sec, usec;

    WPL_TimestampGet (&before);
#endif
    status = WPX_FRMR_SONET_SDH_PORT_ForceB (0, 0, line_port_id);
    WTI_FlexmuxCheckStatus ("WPX_FRMR_SONET_SDH_PORT_ForceB", status,
                            __LINE__);
#if DEBUG
    status =
        WPX_FRMR_SONET_SDH_PORT_GetForceState (0, line_port_id,
                                               &force_side);
    WTI_FlexmuxCheckStatus ("WPX_FRMR_SONET_SDH_PORT_GetForceState",
                            status, __LINE__);
    WPL_TimestampGet (&after);
    sec = (after.sec) - (before.sec);
    usec = (after.usec) - (before.usec);
    if (usec < 0)
    {
        sec = sec - 1;
        usec = usec + 1000000;
    }
    printf ("In line %d forcing port B took %d.%06d seconds\n",
            line_port_id, sec, usec);
    if (status == WPX_UFE_FRAMER_OK)
    {
        if (force_side == WPX_UFE_FRAMER_PROTECTION_FORCE_A)
        {
            printf
                ("From ForceB: In line %d traffic is received from port A\n",
                 line_port_id);
        }
        else if (force_side == WPX_UFE_FRAMER_PROTECTION_FORCE_B)
        {
            printf
                ("From ForceB: In line %d traffic is received from port B\n",
                 line_port_id);
        }
    }
#endif
}


#if WTI_COLLECT_TRACKING_DATA

typedef struct
{
    WP_U32 type;
    WP_U32 timestamp;
} PATS_RECORD;

static int fence = 0;
static int max_items = 0x10000 / sizeof (PATS_RECORD);

#if defined(__linux__)
PATS_RECORD *save_area = (PATS_RECORD *) 0x1c1ad000;
#else
PATS_RECORD *save_area = (PATS_RECORD *) 0xbc1ad000;
#endif

#define IS_UNKNOWN_RECORD  0
#define IS_ENTER_MB       18
#define IS_EXIT_MB        19

void record_action (WP_U32 type, WP_U32 timestamp)
{
    PATS_RECORD *item;
    static PATS_RECORD last_item = { IS_UNKNOWN_RECORD, 0 };

    if (fence >= max_items - 1)
        return;

    if (type == IS_ENTER_MB)
    {
        last_item.type = type;
        last_item.timestamp = timestamp;
        return;
    }

    if (type == IS_EXIT_MB)
    {
        if (last_item.type == IS_ENTER_MB)
        {
            last_item.type = IS_UNKNOWN_RECORD;
            return;
        }
    }

    if (last_item.type == IS_ENTER_MB)
    {
        item = &save_area[fence++];
        item->type = last_item.type;
        item->timestamp = last_item.timestamp;
        last_item.type = IS_UNKNOWN_RECORD;
    }

    item = &save_area[fence++];
    item->type = type;
    item->timestamp = timestamp;
}

WP_CHAR *type_name[] = {
    "SERVER ACTIVE       ",     //   0
    "ANC PORT ALARM      ",     //   1
    "ANC PATH ALARM      ",     //   2
    "ANC DC PORT ALARM   ",     //   3
    "ANC SC PORT ALARM   ",     //   4
    "ANC PORT PM         ",     //   5
    "ANC HO PATH PM      ",     //   6
    "ANC LO PATH PM      ",     //   7
    "ANC PDH PM          ",     //   8
    "ANC DC PORT PM      ",     //   9
    "ANC K1/K2 SIGNAL    ",     //  10
    "ANC S1 SIGNAL       ",     //  11
    "ANC DIAGNOSTIC TABLE",     //  12
    "ENTER INT           ",     //  13
    "EXIT INT            ",     //  14
    "REINSTATE           ",     //  15
    "ENTER HANDLER       ",     //  16
    "EXIT HANDLER        ",     //  17
    "ENTER MB            ",     //  18
    "EXIT MB             ",     //  19
    "SVC PATH ALARM      ",     //  20
    "SVC PORT ALARM      ",     //  21
    "ENTER LOS SVC       ",     //  22
    "EXIT LOS SVC        ",     //  23
    "SVC K1/K2           ",     //  24
    "SVC S1              ",     //  25
    "CB PATH ALARM       ",     //  26
    "CB LOS ALARM        ",     //  27
    "CB PORT ALARM       ",     //  28
    "CB K1/K2            ",     //  29
    "CB S1               ",     //  30
    "CB PDH              ",     //  31
    "SVC PM PORT         ",     //  32
    "SVC PM HO PATH      ",     //  33
    "SVC PM LO PATH      ",     //  34
    "SVC PM PDH          ",     //  35
    "CB PM PORT          ",     //  36
    "CB PM HO PATH       ",     //  37
    "CB PM LO PATH       ",     //  38
    "CB PM PDH           ",     //  39
    "LOS REINSTATE       ",     //  40
    "SVC LOS REINSTATE   ",     //  41
    "INTMODE             ",     //  42
    "IS_CORE/FRAMER      ",     //  43
    "IS SFP              ",     //  44
    "IS LOST LOCK        ",     //  45
    "IS EXT PLL          ",     //  46
    "LOCK AVAIL          ",     //  47
    "LOCK ACQUIRE        ",     //  48
    "LOCK TAKEN          ",     //  49
    "IS LAST             "
};

void WTI_DisplayRecords (void)
{
    WP_U32 j;
    PATS_RECORD *item;

    for (item = save_area, j = 0; j < fence; ++j, ++item)
        printf ("%s 0x%x\n", type_name[item->type], item->timestamp);

    printf ("%d items\n", fence);
    fence = 0;
}

#endif
