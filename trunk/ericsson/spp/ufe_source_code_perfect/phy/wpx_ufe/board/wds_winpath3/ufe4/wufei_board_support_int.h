/*****************************************************************************
 * (C) Copyright 2000-2006, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: wufei_board_support_int.h
 *
 * Purpose: the UFE Glue logic function declaration
 *
 ******x**********************************************************************/
#ifndef WUFEI_BOARD_SUPPORT_INT_H
#define WUFEI_BOARD_SUPPORT_INT_H

#include "wufe_types.h"

#ifndef WUFE_LINE_H
#include "wufe_line.h"
#endif

WUFE_status WUFEI_BsFpgaRelease(WP_U32 ufeid);
WUFE_status WUFEI_BsFpgaInit(WP_U32 ufeid);
WUFE_status WUFEI_BsFpgaReset(WP_U32 ufeid);
void WUFEI_HwBsEventRegSet(WP_U32 ufeid, WP_U16 value);
void WUFEI_HwBsEventRegGet(WP_U32 ufeid, WP_U16* value);
void WUFEI_HwBsEventMaskRegSet(WP_U32 ufeid, WP_U16 value);
void WUFEI_HwBsEventMaskRegGet(WP_U32 ufeid, WP_U16* value);
WUFE_status WUFEI_HwBsZarlinkPllConfig(WP_U32 ufeid, WP_U32 setup_cmd);
void WUFEI_BoardWriteZarlinkPll1FreeRunFreqOffset(WP_U32 ufeid, WP_S32 pll_ppb_offset);
WUFE_status WUFEI_BsFpgaGpioClockSelect(WP_U32 ufeid, WP_U32 gpio_select, WP_U16 gpio_clock);
WUFE_status WUFEI_BsFpgaGpioClockGet(WP_U32 ufeid, WP_U16 *gpio_register);
WUFE_status WUFEI_BsFpgaPllClockStatusGet(WP_U32 ufeid);
WUFE_status WUFEI_BsUfeInterruptEnable(WP_U32 ufeid);
WUFE_status WUFEI_BsFramerInterruptEnable(WP_U32 ufeid);
WUFE_status WUFEI_BsExtPllInterruptEnable(WP_U32 ufeid);
WUFE_status WUFEI_BsLockLostInterruptEnable(WP_U32 ufeid);
WUFE_status WUFEI_BsSfpInterruptEnable(WP_U32 ufeid);
WUFE_status WUFEI_BsUfeInterruptDisable(WP_U32 ufeid);
WUFE_status WUFEI_BsFramerInterruptDisable(WP_U32 ufeid);
WUFE_status WUFEI_BsExtPllInterruptDisable(WP_U32 ufeid);
WUFE_status WUFEI_BsLockLostInterruptDisable(WP_U32 ufeid);
WUFE_status WUFEI_BsSfpInterruptDisable(WP_U32 ufeid);
WUFE_status WUFEI_BsUfeInterruptGet(WP_U32 ufeid, WP_boolean *interrupt_result);
WUFE_status WUFEI_BsFramerInterruptGet(WP_U32 ufeid, WP_boolean *interrupt_result);
WUFE_status WUFEI_BsUfeExtPllInterruptGet(WP_U32 ufeid, WP_boolean *interrupt_result);
WUFE_status WUFEI_BsUfeLockLostInterruptGet(WP_U32 ufeid, WP_boolean *interrupt_result);
WUFE_status WUFEI_BsUfeSfpInterruptGet(WP_U32 ufeid, WP_boolean *interrupt_result);

void WUFEI_HwBsLockLostRegSet(WP_U32 ufeid, WP_U16 value);
void WUFEI_HwBsLockLostRegGet(WP_U32 ufeid, WP_U16* value);
void WUFEI_HwBsLockLostMaskRegSet(WP_U32 ufeid, WP_U16 value);
void WUFEI_HwBsLockLostMaskRegGet(WP_U32 ufeid, WP_U16* value);
void WUFEI_HwBsSfpAbsentRegSet(WP_U32 ufeid, WP_U16 value);
void WUFEI_HwBsSfpAbsentRegGet(WP_U32 ufeid, WP_U16* value);
void WUFEI_HwBsSfpAbsentMaskRegSet(WP_U32 ufeid, WP_U16 value);
void WUFEI_HwBsSfpAbsentMaskRegGet(WP_U32 ufeid, WP_U16* value);
void WUFEI_HwBsSfpLosRegSet(WP_U32 ufeid, WP_U16 value);
void WUFEI_HwBsSfpLosRegGet(WP_U32 ufeid, WP_U16* value);
void WUFEI_HwBsSfpLosMaskRegSet(WP_U32 ufeid, WP_U16 value);
void WUFEI_HwBsSfpLosMaskRegGet(WP_U32 ufeid, WP_U16* value);

#define WUFEI_BsLineCadSetSyncType( ufeid,sys_line_interface,line_id,type) WUFE_OK
#define WUFEI_BsLineCadClockRecTypeSelect( ufeid,sys_line_interface,line_id,cd_id) WUFE_OK
#define WUFEI_BsLineCadClockRecEnable( ufeid,sys_line_interface,line_id) WUFE_OK
#define WUFEI_BsLineCadClockRecDisable( ufeid,sys_line_interface,line_id) WUFE_OK 
#define WUFEI_BsTiclkInSpeConnect( ufeid,  spe,  ja) WUFE_OK
#define WUFEI_Ac1fpDelaySet( ufeid,  delay) WUFE_OK
#define WUFEI_BsCadLineIfCheck(wufe_id,sys_line_interface) WUFE_OK

#endif /* WUFEI_BOARD_SUPPORT_INT_H*/
