/**********************************************************************************************
 * (C) Copyright PMC - WIN division (Wintegra) 2012.  All rights reserved.
 * PMC-Sierra (WINTEGRA) CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of PMC-Sierra (Wintegra).
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ***********************************************************************************************/

/*****************************************************************************
 *  Application : ATM IMA UFE
 *  File Name   : flexmux_util.h
 *
 *****************************************************************************/

/****************************************************************************
 *
 * Example: flexmux_util.h
 * Demonstrates prototypes and definitions of the FLEXMUX FRAMER\MAPPER
 * utility functions
 *
 ****************************************************************************/

#ifndef WTI_FLEXMUX_UTIL_H
#define WTI_FLEXMUX_UTIL_H

#ifndef _WPX_UFE_FRAMER_API_PUBLIC_
#include "WO_FRMR_API_public.h"
#endif

#define WTI_MAX_FLEXMUX_DEVICES_SUPPORTED_BY_DRIVER 4
#define WTI_FLEXMUX_NUM_DEVICES  1
#define WTI_MAX_LINE_PORTS_PER_FLEXMUX_DEVICE 16
#define WTI_FLEXMUX_DBG_MODE 0

#define WTI_FLEXMUX_ENABLE_ALARMS 1
#define WTI_PRINT_ALARM_DATA      0
#define WTI_FLEXMUX_ENABLE_PERFORMANCE_MONITORING 1

#define WTI_COLLECT_TIMING_INFORMATION 0    // APS and other actions
#define WTI_COLLECT_TRACE_INFORMATION  0    // thread locking tracking

#define WTI_FLEXMUX_ALARM_AND_PM_THREAD (WTI_FLEXMUX_ENABLE_ALARMS + WTI_FLEXMUX_ENABLE_PERFORMANCE_MONITORING)
#define WTI_COLLECT_TRACKING_DATA       (WTI_COLLECT_TIMING_INFORMATION + WTI_COLLECT_TRACE_INFORMATION)

#define WT_RUN_PRBS_CHECK 0

#define WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC4    /* VC3 or VC4 (AU3 or AU4) */

#define WTI_FLEXMUX_PHYSICAL_CONF WT_FLEXMUX_UFE_412_1_PORT_OC12

#define WTI_FLEXMUX_CLOCK_REC_GAIN_FAST MAX_TIMESTAMP_GAIN  /* 0x0001FFFF */
#define WTI_FLEXMUX_CLOCK_REC_GAIN_SLOW 0x000001FF

#define WTI_FLEXMUX_RECV_PDH_LINE_CLK_0 0
#define WTI_FLEXMUX_RECV_PDH_LINE_CLK_1 1

#define WTI_FLEXMUX_LINE_TIMING_CT_MODE_ENABLE 0    /* 0 - WPX_UFE_FRAMER_TRANSMIT_TIMING_WINPATH 1 - WPX_UFE_FRAMER_TRANSMIT_TIMING_INPUT_REFERENCE_CLOCK */

typedef enum
{
    WT_FLEXMUX_UFE_412_1_PORT_OC12,
    WT_FLEXMUX_UFE_412_2_PORTS_OC3,
    WT_FLEXMUX_UFE_412_4_PORTS_OC3,
    WT_FLEXMUX_UFE_412_1_PORT_OC12_PLUS_1_PROTECTION,
    WT_FLEXMUX_UFE_412_4_PORTS_OC3_PLUS_4_PROTECTION,
    WT_FLEXMUX_UFE_448_4_PORTS_OC12,
    WT_FLEXMUX_UFE_448_16_PORTS_OC3,
    WT_FLEXMUX_UFE_448_4_PORTS_OC12_PLUS_4_PROTECTION,
    WT_FLEXMUX_UFE_448_8_PORTS_OC3_PLUS_8_PROTECTION,
    WT_FLEXMUX_UFE_412_4_PORTS_OC3_CARD_PROTECTION_MASTER,
    WT_FLEXMUX_UFE_412_4_PORTS_OC3_CARD_PROTECTION_SLAVE,
} WTI_flexmux_physical_conf;

typedef enum
{
    WT_FLEXMUX_NONE_LOOPBACK,
    WT_FLEXMUX_SHALLOW_LINE_LOOPBACK,
    WT_FLEXMUX_DEEP_LINE_LOOPBACK,
    WT_FLEXMUX_SHALLOW_SYSTEM_LOOPBACK,
    WT_FLEXMUX_DEEP_SYSTEM_LOOPBACK,
} WTI_flexmux_loopback_mode;

typedef enum
{
    WT_FLEXMUX_LINE_PORT_OC3,
    WT_FLEXMUX_LINE_PORT_OC12,
    WT_FLEXMUX_LINE_PORT_STM1,
    WT_FLEXMUX_LINE_PORT_STM4
} WTI_flexmux_line_port_mode;

typedef enum
{
    WT_FLEXMUX_CONNECTION_TYPE_E1_UNFRAMED,
    WT_FLEXMUX_CONNECTION_TYPE_E1_FRAMED,
    WT_FLEXMUX_CONNECTION_TYPE_E1_FRAMED_CAS,
    WT_FLEXMUX_CONNECTION_TYPE_T1_UNFRAMED,
    WT_FLEXMUX_CONNECTION_TYPE_T1_FRAMED,
    WT_FLEXMUX_CONNECTION_TYPE_T1_FRAMED_CAS,
    WT_FLEXMUX_CONNECTION_TYPE_E3,
    WT_FLEXMUX_CONNECTION_TYPE_DS3,
    WT_FLEXMUX_CONNECTION_TYPE_VC11,
    WT_FLEXMUX_CONNECTION_TYPE_VC12,
    WT_FLEXMUX_CONNECTION_TYPE_VC3,
    WT_FLEXMUX_CONNECTION_TYPE_VC4,
    WT_FLEXMUX_CONNECTION_TYPE_VC4_4C,
    WT_FLEXMUX_CONNECTION_TYPE_VT1_5,
    WT_FLEXMUX_CONNECTION_TYPE_VT2,
    WT_FLEXMUX_CONNECTION_TYPE_STS1_SPE,
    WT_FLEXMUX_CONNECTION_TYPE_STS3,
    WT_FLEXMUX_CONNECTION_TYPE_STS12,
    WT_FLEXMUX_CONNECTION_TYPE_VCAT
} WTI_flexmux_connection_type;

typedef struct
{
    WP_U8 flexmux_id;
    WTI_flexmux_physical_conf flexmux_hw_config;
    WP_U8 flexmux_build_personality;
    WP_U8 flexmux_mode;
    WTI_flexmux_loopback_mode flexmux_loopback_mode;
} WTI_Flexmux_global_setup;

typedef struct
{
    WP_boolean is_initialized;
} WTI_Flexmux_connection_registry;

typedef struct
{
    WP_boolean is_initialized;
    WP_U8 line_port_mode;
} WTI_Flexmux_line_port_registry;

typedef struct
{
    WP_boolean is_initialized;
    WTI_flexmux_physical_conf flexmux_hw_config;
    WP_U8 flexmux_build_personality;
    WP_U8 flexmux_mode;
    WP_U8 max_line_ports;
    WTI_Flexmux_line_port_registry line_ports_reg[WTI_MAX_LINE_PORTS_PER_FLEXMUX_DEVICE];
} WTI_Flexmux_device_registry;

WTI_Flexmux_device_registry flexmux_reg[WTI_MAX_FLEXMUX_DEVICES_SUPPORTED_BY_DRIVER];

WP_S32 WTI_FlexmuxInit (WP_U32 upi_index, WP_U8 flexmux_id, WTI_flexmux_physical_conf flexmux_hw_config, WP_U8 flexmux_mode, WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4, WTI_Flexmux_global_setup * flexmux_cfg);

WP_S32 WTI_FlexmuxLinePortConfig (WP_U8 flexmux_id, WP_U8 line_port_id, WTI_flexmux_line_port_mode line_port_cfg);

WP_S32 WTI_FlexmuxConnectionCreate (WP_U8 flexmux_id, WP_U8 line_port_id, WP_U32 client_port_id, WTI_flexmux_connection_type type, void *ufe_line_params);

WP_S32 WTI_FlexmuxConnectionDelete (WP_U8 flexmux_id, WP_U32 client_port_id, void *ufe_line_params);

WP_S32 WTI_FlexmuxClientPortIndexGet (WP_U32 ufe_line_index, WUFE_line_transf_type transfer_type);

WP_S32 WTI_FlexmuxLinePortIndexGet (WP_U32 ufe_line_index, WP_U32 flexmux_id);

void WTI_FlexmuxDisplayVersionAndConfig (WP_U32 flexmux_id);

WP_U8 WTI_FlexmuxEnableLoopback (U8 flexmux_id, WP_U32 line_port_id, WP_U32 client_port_id, WTI_flexmux_loopback_mode loopback_type);
WP_U8 WTI_FlexmuxDisableLoopback (U8 flexmux_id, WP_U32 line_port_id, WP_U32 client_port_id, WTI_flexmux_loopback_mode loopback_type);
WP_U8 WTI_FlexmuxGetLoopbackState (U8 flexmux_id, WP_U32 line_port_id, WP_U32 client_port_id, WTI_flexmux_loopback_mode loopback_type);
WP_U8 WTI_FlexmuxCheckPrbsResult (U8 flexmux_id, WP_U32 client_port_id, WUFE_un_framed framing_mode);
void WTI_FlexmuxPDHOutputClockSet (WP_U8 output_select, WP_U32 line_index, WUFE_line_transf_type transfer_type, WP_U32 clock_rate);

WUFE_status WPX_FRMR_PM_SonetSdhPortStorage (void *space, WP_U32 element_type);
WUFE_status WPX_FRMR_PM_SonetSdhHOPathStorage (void *space, WP_U32 element_type);
WUFE_status WPX_FRMR_PM_SonetSdhLOPathStorage (void *space, WP_U32 element_type);
WUFE_status WPX_FRMR_PM_PdhStorage (void *space, WP_U32 element_type);

#endif /* WTI_FLEXMUX_UTIL_H */
