/*****************************************************************************
 * (C) Copyright 2000-2006, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: wpx_ufe2_data.h
 *
 * For:  Board WEB2 UFE2 specific addresses
 *
 * Purpose: Define board-specific data needed to build the UFE2 utilities.
 *
 * The addresses defined here must be physical addresses only.  The phy routines
 * cannot use these addresses directly.  They must translate these to virtual
 * addresses using the target-specific macro WPL_PHY_VIRTUAL(wpid, physical_address).
 *
 ****************************************************************************/

#ifndef WPX_UFE2_DATA_H
#define WPX_UFE2_DATA_H

#ifndef WPX_PHY_DATA_H
#include "wpx_phy_data.h"
#endif

/* Address of TEMUX84 on the UFE2 card */
#define APP_UFE_BASE_TEMUX0_UPI1  0
#define APP_UFE_BASE_TEMUX0_UPI2  0

/* Address of ARROW622 on the UFE2 card */
#define APP_UFE_BASE_ARROW0_UPI1  0
#define APP_UFE_BASE_ARROW0_UPI2  0

/* Address of UFE2 FPGA on the UFE2 card */
#define APP_UFE_BASE_UFE0_UPI1  APP_UFE_UPI1
#define APP_UFE_BASE_UFE0_UPI2  APP_UFE_UPI2

#define APP_UFE_MEM_SPACE_CONNECTORS 2

#endif /*WPX_UFE2_DATA_H*/
