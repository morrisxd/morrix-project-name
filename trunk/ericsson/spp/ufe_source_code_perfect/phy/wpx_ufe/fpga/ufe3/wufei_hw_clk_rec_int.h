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

void WUFEI_HwClockRecEventRegSet(WP_U32 ufeid, WP_U16 value);
void WUFEI_HwClockRecEventRegGet(WP_U32 ufeid, WP_U16* value);
void WUFEI_HwClockRecEventMaskRegSet(WP_U32 ufeid, WP_U16 value);
void WUFEI_HwClockRecEventMaskRegGet(WP_U32 ufeid, WP_U16* value);
void WUFEI_HwClockRecIdirectAddrSet(WP_U32 ufeid, void* indirect_addr);
void WUFEI_HwClockRecIdirectDataSet(WP_U32 ufeid, WP_U16 value);
void WUFEI_HwClockRecIdirectDataGet(WP_U32 ufeid, WP_U16* value);
void WUFEI_HwClockRecIdirectMemset(WP_U32 ufeid, void* addr, WP_U16 value, WP_U32 size);

/*Clock recovery general registes*/
WP_U32 WUFEI_HwClockRecNumOfRxDiffUnitsGet(WP_U32 ufeid);
WP_U32 WUFEI_HwClockRecNumOfRxAdapUnitsGet(WP_U32 ufeid);
WP_U32 WUFEI_HwClockRecNumOfTxDiffUnitsGet(WP_U32 ufeid);
WP_U32 WUFEI_HwClockRecNumOfTxAdapUnitsGet(WP_U32 ufeid);

WUFE_status WUFEI_HwClockRecMethodSet(WP_U32 ufeid, WP_U8 method);

/*Clock recovery timestamp registers handling*/
void WUFEI_HwClockRecRxTsModeRegSet(WP_U32 ufeid, WP_U32 timestamp_id, WP_U16 value);
void WUFEI_HwClockRecRxTsModeRegGet(WP_U32 ufeid, WP_U32 timestamp_id, WP_U16* value);
void WUFEI_HwClockRecTxTsModeRegSet(WP_U32 ufeid, WP_U32 timestamp_id, WP_U16 value);
void WUFEI_HwClockRecTxTsModeRegGet(WP_U32 ufeid, WP_U32 timestamp_id, WP_U16* value);
void WUFEI_HwClockRecTxTsBufferSizeRegSet(WP_U32 ufeid, WP_U32 timestamp_id, WP_U16 value);
void WUFEI_HwClockRecTxTimestampRegGet(WP_U32 ufeid, WP_U32 timestamp_id, WP_U32* value);
void WUFEI_HwClockRecRxTimestampRegGet(WP_U32 ufeid, WP_U32 timestamp_id, WP_U32* value);
void WUFEI_HwClockRecTxTsBufferSizeRegGet(WP_U32 ufeid, WP_U32 timestamp_id, WP_U16* value);
void WUFEI_HwClockRecRxTsBufferSizeRegSet(WP_U32 ufeid, WP_U32 timestamp_id, WP_U16 value);
void WUFEI_HwClockRecRxTsBufferSizeRegGet(WP_U32 ufeid, WP_U32 timestamp_id, WP_U16 value);
void WUFEI_HwClockRecRxTsBufferEnable(WP_U32 ufeid, WP_U32 timestamp_id);
void WUFEI_HwClockRecRxTsBufferDisable(WP_U32 ufeid, WP_U32 timestamp_id);

/*Clock divisor configuration - wufei_clk_rec.c*/
WUFE_status WUFEI_HwClockRecDcoEnable(WP_U32 ufeid, WP_U32 dco_id, WP_U8 enable);
WUFE_status WUFEI_HwClockRecDcoInit(WP_U32 ufeid, WP_U32 dco_id);
WUFE_status WUFEI_HwClockRecDcoModeSet(WP_U32 ufeid, WP_U32 dco_id,
                                       WP_U16 e1t1_mode, WP_U16 sync_mode, WP_U16 active_mode);
WUFE_status WUFEI_HwClockRecDcoDivisorSet(WP_U32 ufeid, WP_U32 dco_id,
                                          WP_U16 integer, WP_U32 remainder);

/*Clock recovery HW configuration - used by wufei_hw_core.c*/
WUFE_status WUFEI_HwClockRecHwConfigSetup(WP_U32 ufeid,
                                          WUFEI_clk_rec_hw_config **hw_config,
                                          WP_U32 RxEnable, 
                                          WP_U32 TxEnable);
WUFE_status WUFEI_HwClockRecHwConfigDelete(WUFEI_clk_rec_hw_config *hw_config);

void WUFEI_HwClockRecHwConfigDisplay(WP_U32 ufeid, WUFEI_clk_rec_hw_config *clk_rec_hw_config);

/*Configures the jitter attenuator clock mux */
WUFE_status WUFEI_HwClockRecInclkMuxSelConfig(WP_U32 ufeid, WP_U32 speid, WP_U8 ds3e3_mode,
                                          WP_U8 framed_mode, WP_U8 inclk_div, WP_U8 inclk_sel);

void WUFEI_HwClockRecInclkMuxSelDisplay(WP_U32 ufeid);
void WUFEI_HwClockRecDcoDisplay(WP_U32 ufeid, WP_U32 dco_id);
void WUFEI_HwClockRecRegsDisplay(WUFEI_system_registry *system_registry);

/* UFE4 functionality */
#define WUFEI_HwClockRecTxTribModeRamSet(wufe_id, line_id, line_type_e1,enable) WUFE_OK
#define WUFEI_HwClockRecTxTribStfIndRamRead(wufe_id, line_id, trib_stf_ind_ram) WUFE_OK
#define WUFEI_HwClockRecTxTribGapCountRamRead(wufe_id, line_id, trib_gap_count_ram) WUFE_OK
#define WUFEI_HwClockRecTxTribGapFracCountRamRead(wufe_id, line_id, trib_gap_frac_count_ram) WUFE_OK
#define WUFEI_HwClockRecTxTribDeltaGapRamRead(wufe_id, line_id, trib_delta_gap_ram) WUFE_OK
#define WUFEI_HwClkRecEnable(system_registry, direction) WUFE_OK
#define WUFEI_HwClkRecDisable(system_registry, direction) WUFE_OK
#define WUFEI_HwClkRecEventsMaskRegSet(ufeid, value)
#define WUFEI_HwClkRecEventsMaskRegGet(ufeid, value)
#define WUFEI_HwClkRecModeDisplay(system_registry)
#define WUFEI_HwClkRecRatioCounterDisplay(system_registry)
#define WUFEI_HwClockRecTxLineModeRamDisplay(wufe_id, line_id) WUFE_OK
#define WUFEI_HwClockRecTxLineStuffIndicationRamDisplay(wufe_id, line_id) WUFE_OK
#define WUFEI_HwClockRecTxLineGapCountRamDisplay(wufe_id, line_id) WUFE_OK
#define WUFEI_HwClockRecTxLineFracGapCountRamDisplay(wufe_id, line_id) WUFE_OK
#define WUFEI_HwClockRecTxLineDeltaGapRamDisplay(wufe_id, line_id) WUFE_OK
#define WUFEI_HwClockRecEventsDispatch(wufe_id, events)
#define WUFEI_HwClkRecRatioCountersSet(system_registry, value, value2)
#define WUFEI_HwClockRecDcoSet(ufeid, dco_id, e1_indication, enable) WUFE_OK
#define WUFEI_HwClockRecRAMGet(system_registry,ufe_line_handle,clk_rec_info_ram_line) WUFE_OK
/* Function for internal testing and debug */
#define WUFEI_HwClkRecTsSet(system_registry, enable) WUFE_OK
/* End of function for internal testing and debug */

#endif /* WUFEI_HW_CLK_REC_INT_H */
