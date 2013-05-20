/*****************************************************************************
 * (C) Copyright 2000-2006, Wintegra. All rights reserved.
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

void WPX_FpgaFirmwareInitializationStart(WP_U32 wpid, WP_U32 connector_id);

/* Physical address of Winpath register base */
WP_U32 WPX_AppWpIntRegBaseGet(WP_U32 wpid);

/* Physical address of CPLD (not present on WEB) */
WP_U32 WPX_MapGsBoardMuxGet(WP_U32 wpid);

/* offset to memory on internal bus */
#if defined(WPI_WP2_PCI_MEMMAP_WA)
#define WPI_INT_MEM_RAM_OFFSET 0x20000
#define WPI_INT_MEM_RIF_OFFSET 0
#else
#define WPI_INT_MEM_RAM_OFFSET 0
#define WPI_INT_MEM_RIF_OFFSET 0x20000
#endif

#define WPI_INTERNAL_BUS_SIZE   0x00008000
#define WPI_INTERNAL_BUS_RAM_BASE (0x1d000000+WPI_INT_MEM_RAM_OFFSET)

#define WPI_INTERNAL_BUS_RIF_SIZE   0x00040000
#define WPI_INTERNAL_BUS_RIF_BASE   (0x1d000000+WPI_INT_MEM_RIF_OFFSET)

#define WPI_PARAM_BUS_SIZE    0x00400000
#define WPI_PARAM_BUS_ADDR    0x10000000

#define WPI_PACKET_BUS_SIZE   0x08000000
#define WPI_PACKET_BUS_ADDR   0x08000000

/* Write a word; the high-order byte is what counts. */
typedef struct  {
      WP_U32 config0;      /* 0 */
      WP_U32 pad1[1];
      WP_U32 config1;      /* 8 */
      WP_U32 pad2[1];
      WP_U32 revision;     /* 0x10 */
      WP_U32 pad3[3];
      WP_U32 interrupts;   /* 0x20 */
      WP_U32 pad4[1];
      WP_U32 int_mask;     /* 0x28 */
      WP_U32 pad5[1];
      WP_U32 int_polarity; /* 0x30 */
      WP_U32 pad6[3];
      WP_U32 card_ctl;     /* 0x40 */
      WP_U32 pad7[1];
      WP_U32 reset;        /* 0x48 */
      WP_U32 pad8[1];
      WP_U32 wol;          /* 0x50 */
      WP_U32 pad9[1];
      WP_U32 pll_ctl;      /* 0x58 */
      WP_U32 pad10[1];
      WP_U32 status;       /* 0x60 */
}wpx_cpld;

typedef struct{
      WP_U32 rev_id;       /* 0x10 */
      WP_U32 pad1[3];
      WP_U32 ser_ctl;      /* 0x20 */
      WP_U32 pad2[1];
      WP_U32 tx_disable;   /* 0x28 */
      WP_U32 pad3[1];
      WP_U32 odt;          /* 0x30 */
      WP_U32 pad4[3];
      WP_U32 leds;         /* 0x40 */
      WP_U32 pad5[3];
      WP_U32 ok_led;       /* 0x50 */
      WP_U32 pad6[3];
      WP_U32 card_detect;  /* 0x60 */
}wpx_cpld_sec;

WP_U32 WPX_CpldSet(WP_U32 port, WP_U32 mode);

#define WPX_PHY_REG_MULTIPLIER 8

#endif
