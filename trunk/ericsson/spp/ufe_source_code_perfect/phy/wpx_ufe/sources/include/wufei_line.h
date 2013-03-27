/*****************************************************************************
 * (C) Copyright 2000-2006, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: wufei_emphy.h
 *
 * Purpose: the UFE EMPHY macro and structures
 *
 ****************************************************************************/

#ifndef WUFEI_LINE_H
#define WUFEI_LINE_H

enum line_tdm_direction
{
   WUFEI_LINE_TDM_RX=0,
   WUFEI_LINE_TDM_TX=1
};

enum line_state_mode
{
   WUFEI_LINE_EMPTY   = 0,
   WUFEI_LINE_DISABLE = 1,
   WUFEI_LINE_ENABLE  = 2
};

enum line_spe_id
{
   WUFEI_LINE_SPE_1=0,
   WUFEI_LINE_SPE_2=1,
   WUFEI_LINE_SPE_3=2,
   WUFEI_LINE_SPE_IRRELEVANT =0xff
};

enum line_stm1_id
{
   WUFEI_LINE_STM1_1=0,
   WUFEI_LINE_STM1_2=1,
   WUFEI_LINE_STM1_3=2,
   WUFEI_LINE_STM4_3=3,
   WUFEI_LINE_STM1_IRRELEVANT =0xff
};

enum line_tdm_loopback
{
   WUFEI_TDM_LOOPBACK_T1=0,
   WUFEI_TDM_LOOPBACK_E1=1,
   WUFEI_TDM_LOOPBACK_OC3=2,
   WUFEI_TDM_LOOPBACK_OC12=3,
   WUFEI_TDM_LOOPBACK_VC3 =4
};


#define WUFEI_NOT_VALID_SLOT      (0xffff)
#define WUFEI_EMPTY_TDM_CAD_ENTRY (0xffffffff)
#define WUFEI_EMPTY_TDM_ENTRY (0xffffffff)
#define WUFEI_NUM_AUG4_PER_CORE 2
#define LOOPBACK_DISABLED -1

#endif /*  WUFEI_LINE_H*/
