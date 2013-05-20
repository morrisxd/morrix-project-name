/*****************************************************************************
 * (C) Copyright 2000-2006, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: wpx_heap_data.h
 *
 * For:  Board WDS3
 *
 * Purpose: Define heap structure values which are not strictly constrained
 * by the board.  For most applications, the default values here are acceptable.
 * While these values must be defined in this file, some targets may ignore them.
 *
 * This file must not be included by any file in wddi/sources, since a build
 * of WDDI should always result in a board-independent object.
 *
 * A specific target accesses these values only through their appearance in
 * the wpi_board_data structure instantiated in wpx_board_data.c.
 *
 * The addresses defined here must be physical addresses only.  Any target must
 * do its own translation of these addresses to virtual addresses.
 *
 * The data contained in this file should not be needed by an application.
 ****************************************************************************/

#ifndef WPX_HEAP_CONFIG_H
#define WPX_HEAP_CONFIG_H

#define WPX_UNUSED 0


#define WPI_HOST_START_ADDRESS WPX_UNUSED   /* take from linker end */
#define WPI_HOST_SIZE          0x04000000   /* 64M define for WinMon */
#ifndef WPL_HEAP_SIZE
#define WPL_HEAP_SIZE          WPX_UNUSED   /* heap is managed by the WinMon or OS*/
#endif

#endif
