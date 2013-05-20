/*****************************************************************************
 * (C) Copyright 2003-2006, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: wpx_app_data.h
 *
 * For:  Board WEB2
 *
 * Purpose: Define board-specific data, which is used by the application.
 *
 * The data in this file is defined in a form which can be used for all targets
 * using the board.
 *
 * This file must not be included by any file in wddi/sources, since a build
 * of WDDI should always result in a board-independent object.
 *
 * The addresses defined here must be physical addresses only.  The phy routines
 * cannot use these addresses directly.  They must translate these to virtual
 * addresses using the target-specific macro WPL_PHY_VIRTUAL(wpid, physical_address).
 *
 ****************************************************************************/

#ifndef WPX_APP_DATA_H
#define WPX_APP_DATA_H

/************************************
 * Bus values defined for this board.
 ************************************/
#define APP_BANK_PARAM 1
#define APP_BANK_PACKET 0
#define APP_BANK_INTERNAL 0

/* For WP2 application bus is used - chipselect 0*/
/* If host bus is used - chipselect 1 */
#define APP_BANK_HOST 0
#define APP_BANK_ATOMIC 5

#endif
