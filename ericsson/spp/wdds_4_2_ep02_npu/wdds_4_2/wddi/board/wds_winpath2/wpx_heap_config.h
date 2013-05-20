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
 * For:  Board WEB2
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

/* WPI_HOST_START_ADDRESS and WPI_HOST_SIZE are used only by Winmon,
 * but they need to be defined for all supported targets.
 *For Winmon, memory layout is:
 * application code/data/bss/stack
 * WPI_HOST_START_ADDRESS           non-cached (veneer) heap
 *  + WPI_HOST_SIZE                 cached (bsp) heap
 *  + WPL_HEAP_SIZE - 1             end of WDDI-accessible memory on host bus */
#if defined(WPX_TARGET_mips_winmon)
#define WPI_HOST_START_ADDRESS   0x2000000      /* 32 M */
#define WPI_HOST_SIZE            0x1800000      /* 24 M */
#define WPL_HEAP_SIZE            0x4000000      /* cached (bsp) heap 64 M */

#elif defined(WPX_TARGET_ppc_winmon)
#define WPI_HOST_START_ADDRESS   0x1000000      /* 16 M */
#define WPI_HOST_SIZE            0x1800000      /* 24 M */
#define WPL_HEAP_SIZE            0x4000000      /* cached (bsp) heap 64 M */

#elif defined(WPX_TARGET_mips_vxworks_6_x)
#define WPI_HOST_START_ADDRESS   WPX_UNUSED
#define WPI_HOST_SIZE            WPX_UNUSED
#define WPL_HEAP_SIZE            0x4000000      /* cached (bsp) heap 64 M */

#elif defined(WPX_TARGET_mips_vxworks_6_3)
#define WPI_HOST_START_ADDRESS   WPX_UNUSED
#define WPI_HOST_SIZE            WPX_UNUSED
#define WPL_HEAP_SIZE            0x2800000      /* cached (bsp) heap 40 M */

#elif defined(WPX_TARGET_mips_vxworks_5_5)
#define WPI_HOST_START_ADDRESS   WPX_UNUSED
#define WPI_HOST_SIZE            WPX_UNUSED
#define WPL_HEAP_SIZE            0x2800000      /* cached (bsp) heap 40 M */

#elif defined(WPX_TARGET_ppc_vxworks_5_5)
#define WPI_HOST_START_ADDRESS   WPX_UNUSED
#define WPI_HOST_SIZE            WPX_UNUSED
#define WPL_HEAP_SIZE            0x2800000      /* cached (bsp) heap 40 M */

#elif defined(WPX_TARGET_mips_linux)
#define WPI_HOST_START_ADDRESS   WPX_UNUSED
#define WPI_HOST_SIZE            WPX_UNUSED
#define WPL_HEAP_SIZE            0x0          /* heap is managed by the OS*/

#elif defined(WPX_TARGET_ppc_linux)
#define WPI_HOST_START_ADDRESS   WPX_UNUSED
#define WPI_HOST_SIZE            WPX_UNUSED
#define WPL_HEAP_SIZE            0x0          /* heap is managed by the OS*/

#else
#error No recognized target was defined.
#endif

#endif
