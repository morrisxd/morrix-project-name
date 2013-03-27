/*****************************************************************************
 * (C) Copyright 2000-2006, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: wufei_hw_clk_translator_int.h
 *
 * Purpose: The UFE clock translator function declaration
 *
 ****************************************************************************/
#ifndef WUFEI_HW_CLK_TRANSLATOR_INT_H
#define WUFEI_HW_CLK_TRANSLATOR_INT_H

#ifndef WUFE_TYPES_H
#include "wufe_types.h"
#endif

void WUFEI_HwSbiClockTranslatorGet(WP_U32 ufeid, WP_U16 ct_id, WP_U16* value);
void WUFEI_HwSbiClockTranslatorSet(WP_U32 ufeid, WP_U16 ct_id, WP_U16 value);
void WUFEI_HwSbiClockTranslatorIndirectGet(WP_U32 ufeid, WP_U16 ct_id, WP_U16* value);
void WUFEI_HwSbiClockTranslatorIndirectSet(WP_U32 ufeid, WP_U16 ct_id, WP_U16 value);
void WUFEI_HwClockRecIndirectCtAddrSet(WP_U32 ufeid, void* indirect_addr);
void WUFEI_HwClockRecIndirectCtDataSet(WP_U32 ufeid, WP_U16* value);
void WUFEI_HwClockRecIndirectCtDataGet(WP_U32 ufeid, WP_U16* value);
WP_U32 WUFEI_HwSbiClockTranslatorAddrGet(WP_U32 ufeid, WP_U16 ct_id);

void WUFEI_HwSbiClockTranslatorEventGet(WP_U32 ufeid, WP_U16* ct_reg_arr);
void WUFEI_HwSbiClockTranslatorEventSet(WP_U32 ufeid, WP_U16* ct_reg_arr);
void WUFEI_HwSbiClockTranslatorEventMaskGet(WP_U32 ufeid, WP_U16* ct_reg_arr);
void WUFEI_HwSbiClockTranslatorEventMaskSet(WP_U32 ufeid, WP_U16* ct_reg_arr);

void WUFEI_HwSbiClockTranslatorTribUpdate(WP_U32 ufeid,
                                          WP_U32 ct_id,
                                          WP_U16 tributary_id);

void WUFEI_HwClockTranslatorWrite(WP_U32 wufe_id,
                                  WP_U32 ct_id,
                                  WP_U32 adap_ct_id,
                                  WP_U32 diff_ct_id,
                                  WP_U16 tributary_type,
                                  WP_U16 framed_mode,
                                  WP_U16 tributary_id);

WP_U16 WUFEI_HwSbiClockTranslatorTribIdGet(WP_U8 core, WP_U8 stm1, WP_U8 spe, WP_U8 link);
void WUFEI_HwSbiClockTranslatorFindActive(WP_U32 wufe_id, WP_U16 clock_translator_id,
                                          WP_U16 *number_of_active);
void WUFEI_HwSbiClockTranslatorInfoGet(WP_U32 wufe_id, WP_U16 ct_id, WP_U16 line_trib_id,
                                       WP_U16 *ct_trib_id, WP_U16 *enable);
void WUFEI_HwClockRecCtRamEntryRead(WP_U32 wufe_id, WP_U16 *ct_ram, WP_U16 entry);

#endif /* WUFEI_HW_CLK_TRANSLATOR_INT_H */
