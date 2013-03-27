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

#ifndef WUFEI_PHY_H
#define WUFEI_PHY_H

#ifndef WUFE_TYPES_H
#include "wufe_types.h"
#endif

#define WUFEI_LINE_NOT_CONNECTED 0xffff
#define WUFEI_MAX_BUF_SIZE_64 64
#define WUFEI_MAX_BUF_SIZE_128 128
#define WUFEI_MAX_BUF_SIZE_256 256

#define WUFEI_PHY_ID_TO_PHY_HANDLE(coreid,offset,phy_id) \
        (coreid == 0)? phy_id : (phy_id + offset)

#define WUFEI_PHY_HANDLE_TO_PHY_ID(coreid,offset,phy_id) \
        (coreid == 0)? phy_id : (phy_id - offset)

#endif /*  WUFEI_PHY_H*/
