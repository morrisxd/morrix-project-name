/*****************************************************************************
 * (C) Copyright 2000-2005, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: wpx_phy_data.h
 *
 * Purpose: Define board-specific data used by either the phy utilities or
 * by the wpx_board_data.c.
 *
 * The data in this file is defined in a form which can be used for all targets
 * using the board.
 *
 * This file may be included only by wpx_phy_data.h and by wpx_board_data.c.
 *
 * This file must not be included by any file in wddi/sources, since a build
 * of WDDI should always result in a board-independent object.
 *
 * The addresses defined here must be physical addresses only.  Any target must
 * do its own translation of these addresses to virtual addresses.
 *
 * The data contained in this file should not be needed by an
 * application.
 ****************************************************************************/

#ifndef WPX_BOARD_DATA_H
#define WPX_BOARD_DATA_H

#ifndef WPX_BOARD_IF_H
#include "../board/wpx_board_if.h"
#endif

/* Simulator support for BE=0xff transactions */
#define WPI_SIM_SUPPORT_64BIT_TRANSACTION 1


/* Physical address of Winpath register base    */
/* the value is equal to host_offset + 0x400000 *
 * which is winsim's internal bus offset it is equal
 * to wpi_registry.pwp_block[0].reg_bases       */
WP_U32 WPX_AppWpIntRegBaseGet(WP_U32 wpid);

/* Physical address of CPLD (not present on virtual) */
WP_U32 WPX_MapGsBoardMuxGet(WP_U32 wpid);

WP_U32 WPX_CpldSet(WP_U32 port, WP_U32 mode);

void WPX_FpgaFirmwareInitializationStart(WP_U32 wpid);

#define WPX_PHY_REG_MULTIPLIER 1
#endif
