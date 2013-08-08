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

#ifndef _WPX_UFE_FRAMER_API_PUBLIC_
#include "WO_FRMR_API_public.h"
#endif

#define USE_POLLING 0

/* Used for testing alarms with ANT-20 */
#define NO_APS_ON_LOS 1

#define WTI_MAX_LINE_PORTS_PER_FLEXMUX_DEVICE 16
#define WTI_FLEXMUX_DBG_MODE 0

#if WTI_FRAMER_OTE_TESTS
#define WTI_FLEXMUX_ENABLE_ALARMS 1
#define WTI_PRINT_ALARM_DATA      0
#define WTI_FLEXMUX_ENABLE_PERFORMANCE_MONITORING 1
#else
#define WTI_FLEXMUX_ENABLE_ALARMS 0
#define WTI_PRINT_ALARM_DATA      0
#define WTI_FLEXMUX_ENABLE_PERFORMANCE_MONITORING 0
#endif

#define WTI_COLLECT_TIMING_INFORMATION 0  // APS and other actions
#define WTI_COLLECT_TRACE_INFORMATION  0  // thread locking tracking

#define WTI_FLEXMUX_ALARM_AND_PM_THREAD (WTI_FLEXMUX_ENABLE_ALARMS + WTI_FLEXMUX_ENABLE_PERFORMANCE_MONITORING)
#define WTI_COLLECT_TRACKING_DATA       (WTI_COLLECT_TIMING_INFORMATION + WTI_COLLECT_TRACE_INFORMATION)

#define WT_RUN_PRBS_CHECK 0

#define CHECK_CORE_HW_INTERRUPT_ASSERTED 1

#if WTI_CESOP_CLOCK_RECOVERY_ENABLE
#define WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC3 /* VC3 or VC4 (AU3 or AU4) */
#else /* not WTI_CESOP_CLOCK_RECOVERY_ENABLE */
#define WTI_FLEXMUX_UFE_FRAMER_WUFE_SDH_TYPE_VC WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC4 /* VC3 or VC4 (AU3 or AU4) */
#endif /* not WTI_CESOP_CLOCK_RECOVERY_ENABLE */
#ifdef __WT_UFE412__
#if DEFAULT_DEMO_MODE
#define WTI_FLEXMUX_PHYSICAL_CONF  WT_FLEXMUX_UFE_412_1_PORT_OC12
#else
#define WTI_FLEXMUX_PHYSICAL_CONF WT_FLEXMUX_UFE_412_1_PORT_OC12
#endif
#else
#define WTI_FLEXMUX_PHYSICAL_CONF WT_FLEXMUX_UFE_448_4_PORTS_OC12
#endif

#define WTI_FLEXMUX_CLOCK_REC_GAIN_FAST MAX_TIMESTAMP_GAIN /* 0x0001FFFF */
#define WTI_FLEXMUX_CLOCK_REC_GAIN_SLOW 0x000001FF

#define WTI_FLEXMUX_RECV_PDH_LINE_CLK_0 0
#define WTI_FLEXMUX_RECV_PDH_LINE_CLK_1 1

#define WTI_FLEXMUX_LINE_TIMING_CT_MODE_ENABLE 0 /* 0 - WPX_UFE_FRAMER_TRANSMIT_TIMING_WINPATH 1 - WPX_UFE_FRAMER_TRANSMIT_TIMING_INPUT_REFERENCE_CLOCK
													CR must be turned off for CT clock to be enabled.
													CR takes precedence over CT clock
												 */

#define WT_FLEXMUX_ALARM_PORT_LOS_OFFSET                                            (0)
#define WT_FLEXMUX_ALARM_PORT_LOF_OFFSET                                            (2)
#define WT_FLEXMUX_ALARM_PORT_OOF_OFFSET                                            (4)
#define WT_FLEXMXU_ALARM_PORT_RS_TIM_OFFSET                                         (6)
#define WT_FLEXMXU_ALARM_PORT_MS_AIS_OFFSET                                         (8)
#define WT_FLEXMUX_ALARM_PORT_MS_RDI_OFFSET                                         (10)
#define WT_FLEXMUX_ALARM_PORT_MS_EXC_OFFSET                                         (12)
#define WT_FLEXMUX_ALARM_PORT_MS_DEG_OFFSET                                         (14)
#define WT_FLEXMUX_ALARM_PORT_RS_EXC_OFFSET                                         (16)
#define WT_FLEXMUX_ALARM_PORT_RS_DEG_OFFSET                                         (18)

#define WT_FLEXMUX_ALARM_PATH_AU_AIS_OFFSET                                            (0)
#define WT_FLEXMUX_ALARM_PATH_AU_LOP_OFFSET                                            (2)
#define WT_FLEXMUX_ALARM_PATH_HP_UNEQ_OFFSET                                            (4)
#define WT_FLEXMXU_ALARM_PATH_HP_TIM_OFFSET                                         (6)
#define WT_FLEXMXU_ALARM_PATH_HP_RDI_OFFSET                                         (8)
#define WT_FLEXMUX_ALARM_PATH_HP_EXC_OFFSET                                         (10)
#define WT_FLEXMUX_ALARM_PATH_HP_DEG_OFFSET                                         (12)
#define WT_FLEXMUX_ALARM_PATH_HP_PLM_OFFSET                                         (14)
#define WT_FLEXMUX_ALARM_PATH_HP_LOM_OFFSET                                         (16)

#define WT_FLEXMUX_ALARM_PATH_TU_AIS_OFFSET                                            (0)
#define WT_FLEXMUX_ALARM_PATH_TU_LOP_OFFSET                                            (2)
#define WT_FLEXMUX_ALARM_PATH_LP_UNEQ_OFFSET                                            (4)
#define WT_FLEXMXU_ALARM_PATH_LP_TIM_OFFSET                                         (6)
#define WT_FLEXMXU_ALARM_PATH_LP_RDI_OFFSET                                         (8)
#define WT_FLEXMUX_ALARM_PATH_LP_EXC_OFFSET                                         (10)
#define WT_FLEXMUX_ALARM_PATH_LP_DEG_OFFSET                                         (12)
#define WT_FLEXMUX_ALARM_PATH_LP_PLM_OFFSET                                         (14)

#define WT_FLEXMUX_ALARM_PDH_LOS_AIS_OFFSET                                         (0) 
#define WT_FLEXMUX_ALARM_PDH_OOF_LOF_OFFSET                                         (2)
#define WT_FLEXMUX_ALARM_PDH_RAI_OFFSET                                        (4)
#define WT_FLEXMUX_ALARM_PDH_RFI_OFFSET                                         (6)
#define WT_FLEXMUX_ALARM_PDH_E3T3_AIS_OFFSET                                         (8)
#define WT_FLEXMUX_ALARM_PDH_E3T3_LOF_OFFSET                                         (10)
#define WT_FLEXMUX_ALARM_PDH_E3T3_XA_OFFSET                                         (12)
#define WT_FLEXMUX_ALARM_PDH_E3T3_FEAC_OFFSET                                         (14)


#define WTI_FLEXMUX_PRIMARY_UPI_INDEX WTI_EMPHY_PORT
#define WTI_FLEXMUX_PRIMARY_FLEXMUX_ID 0
#define WTI_FLEXMUX_PRIMARY_DEVICE_ID  0

#if WTI_DUAL_EMPHY
#define WTI_FLEXMUX_SECOND_UPI_INDEX WTI_SECOND_EMPHY_PORT
#define WTI_FLEXMUX_SECOND_FLEXMUX_ID 1
#define WTI_FLEXMUX_SECOND_DEVICE_ID  1
#endif

typedef enum
{
   WT_FLEXMUX_DEVICE_MODE_SDH = WPX_UFE_FRAMER_DEVICE_MODE_SDH,
   WT_FLEXMUX_DEVICE_MODE_SONET = WPX_UFE_FRAMER_DEVICE_MODE_SONET
}WTI_flexmux_mode;

typedef enum
{
   WT_FLEXMUX_UFE_412_1_PORT_OC12,
   WT_FLEXMUX_UFE_412_2_PORTS_OC3,
   WT_FLEXMUX_UFE_412_4_PORTS_OC3,
   WT_FLEXMUX_UFE_412_1_PORT_OC12_PLUS_1_PROTECTION,
   WT_FLEXMUX_UFE_412_4_PORTS_OC3_PLUS_4_PROTECTION,
   WT_FLEXMUX_UFE_448_4_PORTS_OC12,
   WT_FLEXMUX_UFE_448_8_PORTS_OC3_PLUS_8_PROTECTION,
   WT_FLEXMUX_UFE_448_4_PORTS_OC12_CARD_PROTECTION_MASTER,
   WT_FLEXMUX_UFE_448_4_PORTS_OC12_CARD_PROTECTION_SLAVE,
   WT_FLEXMUX_UFE_412_4_PORTS_OC3_CARD_PROTECTION_MASTER,
   WT_FLEXMUX_UFE_412_4_PORTS_OC3_CARD_PROTECTION_SLAVE,
   WT_FLEXMUX_UFE_412_4_PORTS_OC3_CLEAR_CHANNEL,
   WT_FLEXMUX_UFE_412_1_PORT_OC12_CLEAR_CHANNEL,
#if WTI_DUAL_EMPHY
   WT_FLEXMUX_UFE_412_4_PORTS_OC3_CARD_PROTECTION_UPI1_MASTER_UPI3_SLAVE
#endif
}WTI_flexmux_physical_conf;

typedef enum
{
   WT_FLEXMUX_NONE_LOOPBACK,
   WT_FLEXMUX_SHALLOW_LINE_LOOPBACK,
   WT_FLEXMUX_DEEP_LINE_LOOPBACK,
   WT_FLEXMUX_SHALLOW_SYSTEM_LOOPBACK,
   WT_FLEXMUX_DEEP_SYSTEM_LOOPBACK,
   WT_FLEXMUX_DIAGSONTIC_LOOPBACK,
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
   WT_FLEXMUX_CONNECTION_TYPE_E3_UNFRAMED,
   WT_FLEXMUX_CONNECTION_TYPE_DS3_UNFRAMED,
   WT_FLEXMUX_CONNECTION_TYPE_E3_FRAMED_G751,
   WT_FLEXMUX_CONNECTION_TYPE_E3_FRAMED_G751_ATM,
   WT_FLEXMUX_CONNECTION_TYPE_DS3_FRAMED_M23,
   WT_FLEXMUX_CONNECTION_TYPE_DS3_FRAMED_CPARITY,
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
}WTI_flexmux_connection_type;

typedef struct
{
   WP_U8                                           flexmux_id;
#if WTI_DUAL_EMPHY
   WP_U8                                           second_flexmux_id;
#endif
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
   WPX_UFE_FRAMER_WUFE_line_transf_type            vc_3_4;
} WTI_Flexmux_device_registry;

WTI_Flexmux_device_registry flexmux_reg[WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES];

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
void WTI_Flexmux_Poke(U8 flexmux_id, U32 Address, U32 Value);
void WTI_Flexmux_Peek(U8 flexmux_id, U32 Address);
void WTI_Flexmux_AddressBasePeek(U8 flexmux_id, U32 StartAddress);
void WTI_FlexmuxPDHOutputClockSet(WP_U8 output_select, WP_U32 line_index,WUFE_line_transf_type transfer_type,WP_U32 clock_rate);
void WTI_Flexmux_Squelch_RecClkSet(WP_U8 flexmux_id, WP_U8 output_selector, WP_U8 squelch_mode);
void WTI_Flexmux_Squelch_RecClkGet(WP_U8 flexmux_id, WP_U8 output_selector);
void WTI_Flexmux_Sdh_Ho_Rdi_Set_Mode(U8 flexmux_id, U8 stm4,U8 stm1,U8 stm0,U8 ho_type, U8 type);
void WTI_Flexmux_Sdh_Ho_SS_Bits_Set_Tx(U8 flexmux_id, U8 stm4,U8 stm1,U8 stm0,U8 ho_type, U8 Value);
void WTI_Flexmux_Sdh_Ho_SS_Bits_Get_Rx(U8 flexmux_id, U8 stm4,U8 stm1,U8 stm0,U8 ho_type);
WUFE_status WPX_FRMR_PM_SonetSdhPortStorage(U32 flexmux_id, void *space, WP_U32 element_type);
WUFE_status WPX_FRMR_PM_SonetSdhHOPathStorage(U32 flexmux_id, void *space, WP_U32 element_type);
WUFE_status WPX_FRMR_PM_SonetSdhLOPathStorage(U32 flexmux_id, void *space, WP_U32 element_type);
WUFE_status WPX_FRMR_PM_PdhStorage(U32 flexmux_id, void *space, WP_U32 element_type);

WP_S32 WTI_FlexmuxPDHTxTimingSet(WP_U8 flexmux_id,WP_U32 client_port_id,WP_U32 line_timing);

void WTI_FlexmuxForceA(WP_U32 flexmux_id, WP_U32 line_port_id);
void WTI_FlexmuxForceB(WP_U32 flexmux_id, WP_U32 line_port_id);
void WTI_Flexmux_ForceStateGet(WP_U32 iLinePort);

#endif /* WTI_FLEXMUX_UTIL_H */
