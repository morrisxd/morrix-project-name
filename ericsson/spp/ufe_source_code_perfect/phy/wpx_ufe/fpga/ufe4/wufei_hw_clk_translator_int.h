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

#define WUFEI_HwSbiClockTranslatorGet( ufeid,  ct_id,  value)
#define WUFEI_HwSbiClockTranslatorSet( ufeid,  ct_id,  value)
#define WUFEI_HwSbiClockTranslatorIndirectGet( ufeid,  ct_id,  value)
#define WUFEI_HwSbiClockTranslatorIndirectSet( ufeid,  ct_id,  value)
#define WUFEI_HwClockRecIndirectCtAddrSet( ufeid,  indirect_addr)
#define WUFEI_HwClockRecIndirectCtDataSet( ufeid,  value)
#define WUFEI_HwClockRecIndirectCtDataGet( ufeid,  value)
#define WUFEI_HwSbiClockTranslatorAddrGet( ufeid,  ct_id) WUFE_OK

#define WUFEI_HwSbiClockTranslatorEventGet( ufeid,  ct_reg_arr)
#define WUFEI_HwSbiClockTranslatorEventSet( ufeid,  ct_reg_arr)
#define WUFEI_HwSbiClockTranslatorEventMaskGet( ufeid,  ct_reg_arr)
#define WUFEI_HwSbiClockTranslatorEventMaskSet( ufeid,  ct_reg_arr)

#define WUFEI_HwSbiClockTranslatorTribUpdate( ufeid,ct_id,tributary_id)

#define WUFEI_HwClockTranslatorWrite( wufe_id,ct_id,adap_ct_id, diff_ct_id,   tributary_type,   framed_mode,tributary_id)

#define WUFEI_HwSbiClockTranslatorTribIdGet( core,  stm1,  spe,  link) WUFE_OK
#define WUFEI_HwSbiClockTranslatorFindActive( wufe_id,  clock_translator_id,number_of_active)
#define WUFEI_HwSbiClockTranslatorInfoGet( wufe_id, ct_id,  line_trib_id,ct_trib_id,  enable)
#define WUFEI_HwClockRecCtRamEntryRead( wufe_id, ct_ram,  entry)

#endif /* WUFEI_HW_CLK_TRANSLATOR_INT_H */
