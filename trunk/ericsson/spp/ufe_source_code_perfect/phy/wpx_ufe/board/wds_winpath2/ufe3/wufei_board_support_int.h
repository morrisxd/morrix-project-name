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
WUFE_status WUFEI_BsLineCadSetSyncType(WP_U32 ufeid,
                                       WP_U32 sys_line_interface,
                                       WP_U32 line_id,
                                       WUFE_line_transf_type type);
WUFE_status WUFEI_BsLineCadClockRecTypeSelect(WP_U32 ufeid,
                                              WP_U32 sys_line_interface,
                                              WP_U32 line_id,
                                              WP_U32 cd_id);
WUFE_status WUFEI_BsLineCadClockRecEnable(WP_U32 ufeid,
                                          WP_U32 sys_line_interface,
                                          WP_U32 line_id);
WUFE_status WUFEI_BsLineCadClockRecDisable(WP_U32 ufeid,
                                           WP_U32 sys_line_interface,
                                           WP_U32 line_id);
WUFE_status WUFEI_BsTiclkInSpeConnect(WP_U32 ufeid, WP_U16 spe, WP_U8 ja);

WUFE_status WUFEI_Ac1fpDelaySet(WP_U32 ufeid, WP_U16 delay);

#define WUFEI_BsUfeInterruptEnable(ufeid) WUFE_IRRELEVANT
#define WUFEI_BsFramerInterruptEnable(ufeid) WUFE_IRRELEVANT
#define WUFEI_BsExtPllInterruptEnable(ufeid) WUFE_IRRELEVANT
#define WUFEI_BsUfeInterruptDisable(ufeid) WUFE_IRRELEVANT
#define WUFEI_BsFramerInterruptDisable(ufeid) WUFE_IRRELEVANT
#define WUFEI_BsExtPllInterruptDisable(ufeid) WUFE_IRRELEVANT
#define WUFEI_BsUfeInterruptGet(ufeid,interrupt_result) WUFE_IRRELEVANT
#define WUFEI_BsFramerInterruptGet(ufeid,interrupt_result) WUFE_IRRELEVANT
#define WUFEI_BsUfeExtPllInterruptGet(ufeid,interrupt_result) WUFE_IRRELEVANT
#define WUFEI_BsLockLostInterruptEnable( ufeid) WUFE_IRRELEVANT
#define WUFEI_BsSfpInterruptEnable( ufeid) WUFE_IRRELEVANT
#define WUFEI_BsLockLostInterruptDisable( ufeid) WUFE_IRRELEVANT
#define WUFEI_BsSfpInterruptDisable( ufeid) WUFE_IRRELEVANT
#define WUFEI_BsUfeLockLostInterruptGet( ufeid, interrupt_result) WUFE_IRRELEVANT
#define WUFEI_BsUfeSfpInterruptGet( ufeid, interrupt_result) WUFE_IRRELEVANT

#define WUFEI_BsCadLineIfCheck(wufe_id,sys_line_interface) WUFE_OK
#define WUFEI_BsFpgaFoTransmitSet(ufeid) WUFE_OK
#define WUFEI_BsFpgaPllClockStatusGet(ufeid) WUFE_OK


/*************************************************
 **********     BOARD SUPPORT CLOCK DIVIDER  *****
 ************************************************/
#define WUFEI_BsClockDivInit(ufeid,clock_div_id) WUFE_OK
#define WUFEI_BsClockDivE1T1ModeSet(ufeid,clock_div_id,transfer_mode) WUFE_OK

#define WUFEI_BsClockDivSyncActiveModeSet(ufeid,clock_div_id,active_mode) WUFE_OK
#define WUFEI_BsClockDivSyncPosNegModeSet(ufeid,clock_div_id,pos_neg_mode) WUFE_OK
#define WUFEI_BsClockDivEnable(ufeid,clock_div_id) WUFE_OK
#define WUFEI_BsClockDivDisable(ufeid,clock_div_id) WUFE_OK
#define WUFEI_BsClockDivDivisorRead(ufeid,clock_div_id,divisor) WUFE_OK
#define WUFEI_BsClockDivDivisorWrite(ufeid,clock_div_id,divisor) WUFE_OK
#define WUFEI_BsClockDivRemainderRead(ufeid,clock_div_id,remainder) WUFE_OK
#define WUFEI_BsClockDivRemainderWrite(ufeid,clock_div_id,remainder) WUFE_OK
#define WUFEI_BsClockDivDisplay(ufeid,clock_div_id) WUFE_OK

#define WUFEI_BsFpgaGpioClockSelect(ufeid, gpio_select, gpio_clock) WUFE_OK
#define WUFEI_BsFpgaGpioClockGet(ufeid, gpio_register) WUFE_OK
/*************************************************
 **********     BOARD SUPPORT PMC          *****
 ************************************************/
#define WUFEI_SetInpClkSource(ufeid, clk)
#define WUFEI_SetInpTIClKCTClkClk52M(ufeid, spe, clk52M_freq, recvr_mode)
#define WUFEI_SetC1FPForCas(ufeid, set_c1fp) WUFE_OK

#define WUFEI_GLUE_LGC_IND0_SBI_PLL_ACLK_M WUFE_IRRELEVANT

typedef enum
{
   WUFEI_CLK52M_FREQ_44  = 0,
   WUFEI_CLK52M_FREQ_51  = 1,
   WUFEI_CLK52M_FREQ_178 = WUFEI_CLK52M_FREQ_44,
   WUFEI_CLK52M_FREQ_137 = WUFEI_CLK52M_FREQ_51
}WUFEI_clk52M_freq;

typedef enum
{
   WUFEI_RECVR_MODE_DS3_UNFRM = 0,
   WUFEI_RECVR_MODE_E3_UNFRM,
   WUFEI_RECVR_MODE_DS3_FRM,
   WUFEI_RECVR_MODE_E3_FRM,
   WUFEI_RECVR_MODE_NA
}WUFEI_recvr_mode;

#endif /* WUFEI_BOARD_SUPPORT_INT_H*/
