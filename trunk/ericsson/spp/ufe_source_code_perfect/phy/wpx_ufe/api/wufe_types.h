/*****************************************************************************
 * (C) Copyright 2000-2006, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: wufe_type.h
 *
 * Purpose:  This module contains platform-specific data types.
 *
 ****************************************************************************/

#ifndef WUFE_TYPES_H
#define WUFE_TYPES_H

#ifndef WPL_TYPES_H
#include "wpl_types.h"
#endif

#ifndef WPL_TYPES_H
#error "UFE driver-wpx_ufe need to be under ufe"
#endif

typedef WP_U32 WUFE_status;
typedef WP_U32 WUFE_handle;

#define WUFE_NULL 0

enum flage_value
{
   WUFE_FALSE = 0,
   WUFE_TRUE
};

enum en_dis_able
{
   WUFE_DISABLE=0,
   WUFE_ENABLE=1
};

typedef enum
{
   WUFE_RX_DIRECTION = 1,
   WUFE_TX_DIRECTION = 2,
   WUFE_FULL_DUPLEX  = WUFE_RX_DIRECTION | WUFE_TX_DIRECTION
}WUFE_Direction;

typedef enum
{
   WUFE_HMVIP_NON   = 1,
   WUFE_HMVIP_NUM_4 = 4
}WUFE_hmvip_type;

#define WUFE_N_SLOTS_E1   32
#define WUFE_N_SLOTS_T1   24

#define WUFE_IRRELEVANT   1
/************************************************
 ****************   STRUCTURE IDENTIFIER   ******
 ************************************************/
typedef enum
{
   WUFE_STRUCT_NULL=0,
   WUFE_STRUCT_LINE_CAD,
   WUFE_STRUCT_LINE_SBI,
   WUFE_STRUCT_LINE_GENERIC,
   WUFE_STRUCT_LINE_SONET,
   WUFE_STRUCT_LINE_SDH,
   WUFE_STRUCT_LINE_DCC,
   WUFE_STRUCT_PHY_ATM,
   WUFE_STRUCT_PHY_HDLC,
   WUFE_STRUCT_PHY_TRANS,
   WUFE_STRUCT_PHY_POS,
   WUFE_STRUCT_PHY_GFP
}WUFE_struct_id;

#endif /* WUFE_TYPES_H*/
