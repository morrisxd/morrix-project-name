/*****************************************************************************
 * (C) Copyright 2000-2010, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: wpx_ufe4_data.h
 *
 * For:  Board WDS3 UFE4
 *
 * Purpose: Define board-specific data needed to build the UFE4 utilities.
 *
 * The addresses defined here must be physical addresses only.  The phy routines
 * cannot use these addresses directly.  They must translate these to virtual
 * addresses using the target-specific macro WPI_PHY_VIRTUAL(wpid, physical_address).
 *
 ****************************************************************************/

#ifndef WPX_UFE4_DATA_H
#define WPX_UFE4_DATA_H

#ifndef WPX_PHY_DATA_H
#include "wpx_phy_data.h"
#endif

/* Address  on the UFE4 card */
#define APP_FLEXMUX_BASE_UPI1    (APP_UFE_UPI1+0x40000)
#define APP_FLEXMUX_BASE_UPI2    (APP_UFE_UPI2+0x40000)
#define APP_FLEXMUX_BASE_UPI3    (APP_UFE_UPI3+0x40000)

/* The APP_UFE_MEM_SPACE_CONNECTORS for WDS3 specifies the
 * memory space for UFE over UPI1, UPI2 and UPI3. The real applications
 * with hard coded UFE memory definition can change this define*/
#define APP_UFE_MEM_SPACE_CONNECTORS 3

#endif /*WPX_UFE4_DATA_H*/
