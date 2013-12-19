/*****************************************************************************
 * (C) Copyright PMC - WIN division (Wintegra) 2011.  All rights reserved.
 * PMC-Sierra (WINTEGRA) CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of PMC-Sierra (Wintegra).
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/****************************************************************************
 *
 * Example: wti_flexmux_util.h
 * Demonstrates prototypes and definitions of the FLEXMUX FRAMER\MAPPER
 * utility functions
 *
 ****************************************************************************/

#ifndef WTI_FLEXMUX_UTIL_H
#define WTI_FLEXMUX_UTIL_H

#ifndef _OMIINO_FRAMER_API_PUBLIC_
//#include "WPX_UFE_FRAMER_API_public.h"
#endif
#include <wufe_types.h>
#include <wufe_errors.h>
#include <wufei_user_config.h>
#include <wufe_user_config.h>
#include <wufe_ufe.h>
#include <wpx_ufe/wpx_frmr/flexmux/WO_FRMR_API_public.h> //ufe4.2.2
//#include <wpx_ufe/wpx_frmr/flexmux/WPX_UFE_FRAMER_API_public.h>

WP_U8 prbs_result[8092];
#define WTI_MAX_FLEXMUX_DEVICES_SUPPORTED_BY_DRIVER 4
#define WTI_FLEXMUX_NUM_DEVICES  1
#define WTI_MAX_LINE_PORTS_PER_FLEXMUX_DEVICE 16
#define WTI_FLEXMUX_DBG_MODE 0

#define WTI_FLEXMUX_ENABLE_PERFORMANCE_MONITORING 0

#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
#define WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC3 /* VC3 or VC4 (AU3 or AU4) */
#else /* not WTI_CESOP_CLOCK_RECOVERY_ENABLE */
#define WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC4 /* VC3 or VC4 (AU3 or AU4) */
#endif /* not WTI_CESOP_CLOCK_RECOVERY_ENABLE */
#ifdef WTI_UFE_4_1_2
#define WTI_FLEXMUX_PHYSICAL_CONF WT_FLEXMUX_UFE_412_1_PORT_OC12
#else
#define WTI_FLEXMUX_PHYSICAL_CONF WT_FLEXMUX_UFE_448_4_PORTS_OC12_PLUS_4_PROTECTION
#endif

#define WTI_FLEXMUX_CLOCK_REC_GAIN_FAST MAX_TIMESTAMP_GAIN /* 0x0001FFFF */
#define WTI_FLEXMUX_CLOCK_REC_GAIN_SLOW 0x000001FF

#define WTI_FLEXMUX_RECV_PDH_LINE_CLK_0 0
#define WTI_FLEXMUX_RECV_PDH_LINE_CLK_1 1
typedef struct
{
    WP_U8 stm4;
    WP_U8 stm1;
    WP_U8 stm0;
    WP_U8 tug2;
    WP_U8 tu;
    WUFE_line_transf_type transfer_type;
    WUFE_un_framed        framed;
    WUFE_cas_enable       cas_enable;
    WUFE_line_loopback_type loopback;
    WUFE_line_clock_rec_params *clock_rec_line_params;
    WP_U32 clock_mode;
} WT_ufe_line_sdh_params;

typedef struct
{
    WP_U8 sts12;
    WP_U8 sts3;
    WP_U8 sts1;
    WP_U8 vt_group;
    WP_U8 vt;
    WUFE_line_transf_type transfer_type;
    WUFE_un_framed        framed;
    WUFE_cas_enable       cas_enable;
    WUFE_line_loopback_type loopback;
    WUFE_line_clock_rec_params *clock_rec_line_params;
} WT_ufe_line_sonet_params;

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
    WT_FLEXMUX_UFE_448_8_PORTS_OC3_PLUS_8_PROTECTION
}WTI_flexmux_physical_conf;

typedef enum
{
    WT_FLEXMUX_NONE_LOOPBACK,
    WT_FLEXMUX_SHALLOW_LINE_LOOPBACK,
    WT_FLEXMUX_DEEP_LINE_LOOPBACK,
    WT_FLEXMUX_SHALLOW_SYSTEM_LOOPBACK,
    WT_FLEXMUX_DEEP_SYSTEM_LOOPBACK,
}WTI_flexmux_loopback_mode;

typedef enum
{
    WT_FLEXMUX_LINE_PORT_OC3,
    WT_FLEXMUX_LINE_PORT_OC12,
    WT_FLEXMUX_LINE_PORT_STM1,
    WT_FLEXMUX_LINE_PORT_STM4
}WTI_flexmux_line_port_mode;

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
    WT_FLEXMUX_CONNECTION_TYPE_VCAT,
}WTI_flexmux_connection_type;

typedef struct
{
    WP_U8                                           flexmux_id;
    WTI_flexmux_physical_conf                       flexmux_hw_config;
    WP_U8                                           flexmux_build_personality;
    WP_U8                                           flexmux_mode;
    WTI_flexmux_loopback_mode                       flexmux_loopback_mode;
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

WP_S32 WTI_FlexmuxInit(WP_U32 upi_index,
                       WP_U8 flexmux_id,
                       WTI_flexmux_physical_conf flexmux_hw_config,
                       WP_U8 flexmux_mode,
                       WPX_UFE_FRAMER_WUFE_line_transf_type vc_3_4,
                       WTI_Flexmux_global_setup *flexmux_cfg);

WP_S32 WTI_FlexmuxLinePortConfig(WP_U8 flexmux_id,
                                 WP_U8 line_port_id,
                                 WTI_flexmux_line_port_mode line_port_cfg);

WP_S32  WTI_FlexmuxConnectionCreate(WP_U8 flexmux_id,
                                    WP_U8 line_port_id,
                                    WP_U32 client_port_id,
                                    WTI_flexmux_connection_type type,
                                    void *ufe_line_params);

WP_S32 WTI_FlexmuxConnectionDelete(WP_U8 flexmux_id,
                                   WP_U32 client_port_id,
                                   void *ufe_line_params);

WP_S32 WTI_FlexmuxClientPortIndexGet(WP_U32 ufe_line_index, WUFE_line_transf_type transfer_type);

WP_S32 WTI_FlexmuxLinePortIndexGet(WP_U32 ufe_line_index,WP_U32 flexmux_id);

void WTI_FlexmuxDisplayVersionAndConfig(WP_U32 flexmux_id);

WP_U8 WTI_FlexmuxEnableLoopback(U8 flexmux_id, WP_U32 line_port_id, WP_U32 client_port_id, WTI_flexmux_loopback_mode loopback_type);
WP_U8 WTI_FlexmuxDisableLoopback(U8 flexmux_id, WP_U32 line_port_id, WP_U32 client_port_id, WTI_flexmux_loopback_mode loopback_type);
WP_U8 WTI_FlexmuxGetLoopbackState(U8 flexmux_id, WP_U32 line_port_id, WP_U32 client_port_id, WTI_flexmux_loopback_mode loopback_type);
WP_U8 WTI_FlexmuxCheckPrbsResult(U8 flexmux_id, WP_U32 client_port_id, WUFE_un_framed framing_mode);
void WTI_FlexmuxPDHOutputClockSet(WP_U8 output_select, WP_U32 line_index,WUFE_line_transf_type transfer_type,WP_U32 clock_rate);


#endif /* WTI_FLEXMUX_UTIL_H */
