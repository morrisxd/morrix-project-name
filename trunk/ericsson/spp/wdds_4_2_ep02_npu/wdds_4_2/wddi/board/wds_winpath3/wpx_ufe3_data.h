/*****************************************************************************
 * (C) Copyright 2000-2006, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: wpx_ufe3plust_data.h
 *
 * For:  Board WDS2 UFE3
 *
 * Purpose: Define board-specific data needed to build the UFE3 utilities.
 *
 * The addresses defined here must be physical addresses only.  The phy routines
 * cannot use these addresses directly.  They must translate these to virtual
 * addresses using the target-specific macro WPL_PHY_VIRTUAL(wpid, physical_address).
 *
 ****************************************************************************/

#ifndef WPX_UFE3_DATA_H
#define WPX_UFE3_DATA_H

#ifndef WPX_PHY_DATA_H
#include "wpx_phy_data.h"
#endif

/* Address of TEMUX336 on the UFE3 card */
#define APP_UFE_BASE_TEMUX0_UPI1    (APP_UFE_UPI1+0x20000)
#define APP_UFE_BASE_TEMUX0_UPI2    (APP_UFE_UPI2+0x20000)
#define APP_UFE_BASE_TEMUX0_UPI3    (APP_UFE_UPI3+0x20000)

/* The APP_UFE_MEM_SPACE_CONNECTORS for WDS3 specifies the
 * memory space for UFE over UPI1, UPI2 and UPI3. The real applications
 * with hard coded UFE memory definition can change this define*/
#define APP_UFE_MEM_SPACE_CONNECTORS 3

#endif /*WPX_UFE3_DATA_H*/
