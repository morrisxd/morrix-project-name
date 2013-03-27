/*****************************************************************************
 * (C) Copyright 2000-2006, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: wufei_hw_clk_rec_int.h
 *
 * Purpose: The UFE clock recovery function declaration
 *
 ****************************************************************************/
#ifndef WUFEI_HW_CLK_REC_INT_H
#define WUFEI_HW_CLK_REC_INT_H

#ifndef WUFE_TYPES_H
#include "wufe_types.h"
#endif

#ifndef WUFEI_REGISTRY_H
#include "wufei_registry.h"
#endif

#ifndef WUFEI_HW_EVENTS_INT_H
#include "wufei_hw_events_int.h"
#endif

#define WUFEI_HwClockRecEventRegSet( ufeid,  value)
#define WUFEI_HwClockRecEventRegGet( ufeid,  value)
#define WUFEI_HwClockRecEventMaskRegSet( ufeid,  value)
#define WUFEI_HwClockRecEventMaskRegGet( ufeid,  value)
#define WUFEI_HwClockRecIdirectAddrSet( ufeid,  indirect_addr)
#define WUFEI_HwClockRecIdirectDataSet( ufeid,  value)
#define WUFEI_HwClockRecIdirectDataGet( ufeid,  value) {*value =0;}
#define WUFEI_HwClockRecIdirectMemset( ufeid,  addr,  value,  size)

/*Clock recovery general registes*/
#define WUFEI_HwClockRecNumOfRxDiffUnitsGet( ufeid) WUFE_OK
#define WUFEI_HwClockRecNumOfRxAdapUnitsGet( ufeid) WUFE_OK
#define WUFEI_HwClockRecNumOfTxDiffUnitsGet( ufeid) WUFE_OK
#define WUFEI_HwClockRecNumOfTxAdapUnitsGet( ufeid) WUFE_OK

#define WUFEI_HwClockRecMethodSet( ufeid,  method) WUFE_OK

/*Clock recovery timestamp registers handling*/
#define WUFEI_HwClockRecRxTsModeRegSet( ufeid,  timestamp_id,  value)
#define WUFEI_HwClockRecRxTsModeRegGet( ufeid,  timestamp_id,  value) {mode =0;}
#define WUFEI_HwClockRecTxTsModeRegSet( ufeid,  timestamp_id,  value)
#define WUFEI_HwClockRecTxTsModeRegGet( ufeid,  timestamp_id,  value)
#define WUFEI_HwClockRecTxTsBufferSizeRegSet( ufeid,  timestamp_id,  value)
#define WUFEI_HwClockRecTxTimestampRegGet( ufeid,  timestamp_id,  value)
#define WUFEI_HwClockRecRxTimestampRegGet( ufeid,  timestamp_id,  value)
#define WUFEI_HwClockRecTxTsBufferSizeRegGet( ufeid,  timestamp_id,  value)
#define WUFEI_HwClockRecRxTsBufferSizeRegSet( ufeid,  timestamp_id,  value)
#define WUFEI_HwClockRecRxTsBufferSizeRegGet( ufeid,  timestamp_id,  value)
#define WUFEI_HwClockRecRxTsBufferEnable( ufeid,  timestamp_id) WUFE_OK
#define WUFEI_HwClockRecRxTsBufferDisable( ufeid,  timestamp_id) WUFE_OK

/*Clock divisor configuration - wufei_clk_rec.c*/
#define WUFEI_HwClockRecDcoInit( ufeid,  dco_id) WUFE_OK
#define WUFEI_HwClockRecDcoModeSet( ufeid,  dco_id,e1t1_mode,  sync_mode,  active_mode) WUFE_OK

/*Clock recovery HW configuration - used by wufei_hw_core.c*/
#define WUFEI_HwClockRecHwConfigSetup( ufeid,hw_config,RxEnable,TxEnable) WUFE_OK
#define WUFEI_HwClockRecHwConfigDelete(hw_config) WUFE_OK

#define WUFEI_HwClockRecHwConfigDisplay( ufeid, clk_rec_hw_config)

/*Configures the jitter attenuator clock mux */
#define WUFEI_HwClockRecInclkMuxSelConfig( ufeid,  speid,  ds3e3_mode,framed_mode,  inclk_div,  inclk_sel) WUFE_OK

#define WUFEI_HwClockRecInclkMuxSelDisplay( ufeid)
#define WUFEI_HwClockRecDcoDisplay( ufeid,  dco_id)
#define WUFEI_HwClockRecRegsDisplay(system_registry)

typedef enum
{
   WUFEI_HW_CLK_REC_MUX_INCLK_DIV_SEL_0 = 0,
   WUFEI_HW_CLK_REC_MUX_INCLK_DIV_SEL_32,
   WUFEI_HW_CLK_REC_MUX_INCLK_DIV_SEL_1074,
   WUFEI_HW_CLK_REC_MUX_INCLK_DIV_SEL_1398
}WUFEI_hw_clk_rec_mux_inclk_div_sel;

/* UFE4 functions */
WUFE_status WUFEI_HwClockRecTxTribModeRamSet(WP_U16 wufe_id, WP_U16 line_id, WP_U8 line_type_e1, WP_U8 enable);
WUFE_status WUFEI_HwClockRecTxTribStfIndRamRead(WP_U16 wufe_id, WP_U16 line_id, WP_U16 *trib_stf_ind_ram);
WUFE_status WUFEI_HwClockRecTxTribGapCountRamRead(WP_U16 wufe_id, WP_U16 line_id, WP_U16 *trib_gap_count_ram);
WUFE_status WUFEI_HwClockRecTxTribGapFracCountRamRead(WP_U16 wufe_id, WP_U16 line_id, WP_U32 *trib_gap_frac_count_ram);
WUFE_status WUFEI_HwClockRecTxTribDeltaGapRamRead(WP_U16 wufe_id, WP_U16 line_id, WP_U32 *trib_delta_gap_ram);

WUFE_status WUFEI_HwClkRecEnable(WUFEI_system_registry* system_registry, WP_U32 direction);
WUFE_status WUFEI_HwClkRecDisable(WUFEI_system_registry* system_registry, WP_U32 direction);
/* Function for internal testing and debug */
WUFE_status WUFEI_HwClkRecTsSet(WUFEI_system_registry* system_registry, WP_U32 enable);
/* End of function for internal testing and debug */
void WUFEI_HwClkRecModeDisplay(WUFEI_system_registry* system_registry);
void WUFEI_HwClkRecRatioCountersSet(WUFEI_system_registry* system_registry, WP_U32 diff_sck_value, WP_U32 xo_sck_value);
void WUFEI_HwClkRecRatioCountersGet(WUFEI_system_registry* system_registry, WP_U32 *diff_sck_value, WP_U32 *xo_sck_value);

void WUFEI_HwClkRecRatioCounterDisplay(WUFEI_system_registry* system_registry);
WUFE_status WUFEI_HwClockRecDcoDivisorSet(WP_U32 ufeid, WP_U32 dco_id, WP_U16 integer, WP_U32 remainder);
WUFE_status WUFEI_HwClockRecDcoSet(WP_U32 ufeid, WP_U32 dco_id, WP_U32 e1_indication, WP_U8 enable);
WUFE_status WUFEI_HwClockRecDcoEnable(WP_U32 ufeid, WP_U32 dco_id, WP_U8 enable);
WUFE_status WUFEI_HwClockRecTxLineModeRamDisplay(WP_U32 wufe_id, WP_U32 line_id);
WUFE_status WUFEI_HwClockRecTxLineStuffIndicationRamDisplay(WP_U32 wufe_id, WP_U32 line_id);
WUFE_status WUFEI_HwClockRecTxLineGapCountRamDisplay(WP_U32 wufe_id, WP_U32 line_id);
WUFE_status WUFEI_HwClockRecTxLineFracGapCountRamDisplay(WP_U32 wufe_id, WP_U32 line_id);
WUFE_status WUFEI_HwClockRecTxLineDeltaGapRamDisplay(WP_U32 wufe_id, WP_U32 line_id);
WUFE_status WUFEI_HwClockRecTxTribModeRamGet(WP_U16 wufe_id, WP_U16 line_id, WP_U16 *trib_mode);
void WUFEI_HwClockRecEventsDispatch(WP_U32 wufe_id, WUFE_events_clock_recovery *events);

void WUFEI_HwClkRecEventsRegSet(WP_U32 ufeid, WP_U16 value);
void WUFEI_HwClkRecEventsRegGet(WP_U32 ufeid, WP_U16* value);
void WUFEI_HwClkRecEventsMaskRegSet(WP_U32 ufeid, WP_U16 value);
void WUFEI_HwClkRecEventsMaskRegGet(WP_U32 ufeid, WP_U16* value);
void WUFEI_HwClkRecTxStuffLossTribIdRegGet(WP_U32 ufeid, WP_U32 index, WP_U16* value);
WUFE_status WUFEI_HwClockRecRAMGet(WUFEI_system_registry *,WUFE_handle,WUFE_clk_rec_info_ram_line*);



#endif /* WUFEI_HW_CLK_REC_INT_H */
