/*****************************************************************************
 * (C) Copyright 2000-2006, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: wufei_hw_liu_int.h
 *
 * Purpose: the UFE socket interface function declaration
 *
 ****************************************************************************/
#ifndef WUFEI_HW_LIU_INT_H
#define WUFEI_HW_LIU_INT_H

#ifndef WUFE_TYPES_H
#include "wufe_types.h"
#endif

WUFE_status WUFEI_LiuSocketEnable(WP_U32 ufeid, WP_U32 coreid, WP_U32 direction);
WUFE_status WUFEI_LiuSocketDisable(WP_U32 ufeid, WP_U32 coreid, WP_U32 direction);


#endif /* WUFEI_HW_LIU_INT_H*/
