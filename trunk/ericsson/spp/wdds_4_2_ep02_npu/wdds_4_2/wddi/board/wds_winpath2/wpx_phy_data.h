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
 * For:  Board WDS2
 *
 * Purpose: Define board-specific data needed to build the phy utilities.
 *
 * The data in this file is defined in a form which can be used for all targets
 * using the board.
 *
 * This file may be #included only by files in the wddi/phy directory.
 *
 * This file must not be included by any file in wddi/sources, since a build
 * of WDDI should always result in a board-independent object.
 *
 * The addresses defined here must be physical addresses only.  The phy routines
 * cannot use these addresses directly.  They must translate these to virtual
 * addresses using the target-specific macro WPL_PHY_VIRTUAL(wpid, physical_address).
 *
 * The data contained in this file should generally not be needed by an
 * application.
 *
 ****************************************************************************/

#ifndef WPX_PHY_DATA_H
#define WPX_PHY_DATA_H

#ifndef WPX_BOARD_DATA_H
#include "wpx_board_data.h"
#endif

#define WPI_WDS2_SERIAL1_BASE 0x1da00000 /* UPI1-WinNet */
#define WPI_WDS2_SERIAL2_BASE 0x1dc00000 /* UPI2-TDI[9-16] */
#define WPI_WDS2_SERIAL3_BASE 0x1de00000 /* WinNet/TDI[1-8] */
#define WPI_WDS2_SERIAL4_BASE 0x1d900000 /* WinNet backhand side */
#define WPI_WDS2_CPLD_BASE    0x1d800000 /* CPLD */
#define WPI_WDS2_SEC_CPLD_BASE    0x1d810000 /* Secondary CPLD (JTAG)*/

#define CPLD_FIBER_EN           (WPI_WDS2_CPLD_BASE + 0x0)
#define CPLD_JTAG               (WPI_WDS2_SEC_CPLD_BASE + 0x10)

#define APP_TDM1_COMET          WPI_WDS2_SERIAL3_BASE
#define APP_TDM9_COMET          WPI_WDS2_SERIAL2_BASE
#define APP_TDM1_UFE_COMET      0
#define APP_CPLD_TDM1_COMET     (WPI_WDS2_SERIAL3_BASE+0x8000)
#define APP_CPLD_TDM9_COMET     (WPI_WDS2_SERIAL2_BASE+0x8000)
#define APP_CPLD_TDM_UFE_COMET  0

/*match register for host chip select 1 */
#define APP_GENET_HOST_IF 0x1d030808
#define APP_UPI_HOST_IF 0x1d030808
#define APP_TDM_HOST_IF 0x1d030808
#define APP_UFE_HOST_IF 0x1d030808
#define APP_PTP_HOST_IF 0x1d030808
#define APP_BIM_TDM_HOST_IF 0x0  /* present on BIM only */

/* Address of Gigabit ethernet. */
#define APP_GENET1_MAC_BASE 0x1da00000
#define APP_GENET2_MAC_BASE 0x1dc00000
#define APP_GENET_REGS      0x1d900000

/* Address of Oc12 Pmc5357 Card */
#define APP_UPI1_OC12_PMC5357 WPI_WDS2_SERIAL1_BASE
#define APP_UPI2_OC12_PMC5357 WPI_WDS2_SERIAL2_BASE


#define APP_OC12_CRSI_POLARITY 0x80

/* Address of Oc3 Pmc5351 Card */
#define APP_UPI1_OC3_PMC5351 WPI_WDS2_SERIAL1_BASE
#define APP_UPI2_OC3_PMC5351 WPI_WDS2_SERIAL2_BASE
/* Address of qfalc Card */
#define APP_TDM4_QFALC  WPI_WDS2_SERIAL3_BASE
#define APP_TDM8_QFALC  (WPI_WDS2_SERIAL3_BASE + 0x2000)
#define APP_RTDM4_QFALC WPI_WDS2_SERIAL2_BASE /*The same address as UPI1 */
#define APP_RTDM8_QFALC (WPI_WDS2_SERIAL2_BASE + 0x2000) /*The same address as UPI1 */
/* Address of qfalc Card of winpath 1 on BIM */
#define APP_TDM4_QFALC_BIM 0x0  /* present on BIM only */
#define APP_TDM8_QFALC_BIM 0x0  /* present on BIM only */
#define APP_RTDM4_QFALC_BIM 0x0  /* present on BIM only */
#define APP_RTDM8_QFALC_BIM 0x0  /* present on BIM only */
/* TDI card definitions QFALC_OCTAL is the card of revb*/
#define APP_TDM_QFALC_CLOCK WPU_MODE_TDM_CLOCK_16MHZ
#define QFALC_QUAD 0
#define QFALC_OCTAL  1
#define APP_TDM_QFALC_TYPE QFALC_OCTAL

/* Address of qfalc Card over the UFE card */
#define APP_UFE_UPI1_TDM4_QFALC  0
#define APP_UFE_UPI1_TDM8_QFALC  0
#define APP_UFE_UPI2_TDM4_QFALC  0
#define APP_UFE_UPI2_TDM8_QFALC  0

/* Base addresses of UFE card */
#define APP_UFE_UPI1      WPI_WDS2_SERIAL1_BASE
#define APP_UFE_UPI2      WPI_WDS2_SERIAL2_BASE

/* Base addresses of PTP card */
#define APP_PTP  WPI_WDS2_SERIAL1_BASE

#endif
