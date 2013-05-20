/*****************************************************************************
 * (C) Copyright 2000-2006, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: wpx_ufe2plust_data.h
 *
 * For:  Board WEB2 UFE2+
 *
 * Purpose: Define board-specific data needed to build the UFE2+ utilities.
 *
 * The addresses defined here must be physical addresses only.  The phy routines
 * cannot use these addresses directly.  They must translate these to virtual
 * addresses using the target-specific macro WPL_PHY_VIRTUAL(wpid, physical_address).
 *
 ****************************************************************************/

#ifndef WPX_UFE2PLUS_DATA_H
#define WPX_UFE2PLUS_DATA_H

#ifndef WPX_PHY_DATA_H
#include "wpx_phy_data.h"
#endif

/* Base addresses of UFE2+ FPGA */
#define APP_UFE_BASE_UFE0_UPI1      APP_UFE_UPI1
#define APP_UFE_BASE_UFE0_UPI2      APP_UFE_UPI2

/* The APP_UFE_MEM_SPACE_CONNECTORS for WDS2 specifies the
 * memory space for UFE over UPI1 and UPI2. The real applications
 * with hard coded UFE memory definition can change this define to 1 */
#define APP_UFE_MEM_SPACE_CONNECTORS 2

#endif /*WPX_UFE2PLUS_DATA_H*/
