/*****************************************************************************
 * (C) Copyright 2003-2006, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: wpx_phy_data.h
 *
 * For:  Board WEB
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

#define APP_GENET_HOST_IF 0x1f010818
#define APP_UPI_HOST_IF 0x1f010818
#define APP_TDM_HOST_IF 0x1f010820
#define APP_UFE_HOST_IF 0x1f010810

#define APP_BIM_TDM_HOST_IF 0x0  /* present on BIM only */

/* Address of Gigabit ethernet. */
#define APP_GENET1_MAC_BASE 0x1f100000
#define APP_GENET2_MAC_BASE 0x1f102000
#define APP_GENET_REGS 0x1f100000

/* Address of Oc12 Pmc5357 Card */
#define APP_UPI1_OC12_PMC5357 0x1f100000
#define APP_UPI2_OC12_PMC5357 0x1f102000

/* Address of Oc12 Pmc5351 Card */
#define APP_UPI1_OC3_PMC5351 0x1f100000
#define APP_UPI2_OC3_PMC5351 0x1f102000

/* Address of FPGA daughter card*/
#define APP_UPI1_FPGA_DC 0x1f100000
#define APP_UPI2_FPGA_DC 0x1f200000
#define APP_UPI3_FPGA_DC 0x1f300000

#define APP_OC12_CRSI_POLARITY 0x0

/* Address of qfalc Card */
#define APP_TDM4_QFALC 0x1f204000
#define APP_TDM8_QFALC 0x1f200000

/* Address of qfalc Card of winpath 1 on BIM */
#define APP_TDM4_QFALC_BIM 0x0  /* present on BIM only */
#define APP_TDM8_QFALC_BIM 0x0  /* present on BIM only */

/* TDI card definitions QFALC_OCTAL is the card of revb*/
#define APP_TDM_QFALC_CLOCK WPU_MODE_TDM_CLOCK_8MHZ
#define QFALC_QUAD 0
#define QFALC_OCTAL  1
#define APP_TDM_QFALC_TYPE QFALC_QUAD

/* Nonexistent on this board */
#define APP_RTDM4_QFALC_BIM 0x0  /* present on BIM only */
#define APP_RTDM8_QFALC_BIM 0x0  /* present on BIM only */
#define APP_RTDM4_QFALC 0x1f110000
#define APP_RTDM8_QFALC 0x1f112000


/* UFE BOARD SPECIFIC */

/********************************************************
 * the UFE is placed on bus  : UFE_WINPATH_BUS
 * the chip select used      : UFE_WINPATH_CHIP_SELECET
 * number of shift for the
 * addres bus from WP to UFE : UFE2_ADDRSHIFT
 * buffer external memory (ZBT SIZE) :
 *                         WUFEI_INDIRECT_MEM_ADD_EXT_FIFO_SIZE
 *********************************************************/
#define WUFE_WINPATH_BUS           WP_BUS_HOST
#define WUFE_WINPATH_CHIP_SELECT  2 /*the third chip select*/
#define WUFE_ADDRSHIFT (3)

#define WUFEI_INDIRECT_MEM_ADD_EXT_FIFO_SIZE  ( 0x100000 ) /* 1 Mbyte ZBT*/



/* Address of TEMUX84 on the UFE2 card */
#define APP_UFE2_UPI1_TEMUX      0x1f140000
#define APP_UFE2_UPI2_TEMUX      0x1f180000

/* Address of ARROW622 on the UFE2 card */
#define APP_UFE2_UPI1_ARROW      0x1f150000
#define APP_UFE2_UPI2_ARROW      0x1f190000

/* Address of qfalc Card over the UFE2 card */
#define APP_UFE_UPI1_TDM4_QFALC  0x1f170000
#define APP_UFE_UPI1_TDM8_QFALC  0x1f172000
#define APP_UFE_UPI2_TDM4_QFALC  0x1f1b0000
#define APP_UFE_UPI2_TDM8_QFALC  0x1f1b2000

/* Address of UFE2 FPGA on the UFE2 card */
#define APP_UFE_UPI1             0x1f178000
#define APP_UFE_UPI2             0x1f1b8000

/* Address of Glue logic (board support) the UFE2 card */
#define APP_UFE2_UPI1_GLUE_LOGIC 0x1f17c000
#define APP_UFE2_UPI2_GLUE_LOGIC 0x1f1bc000

/*Macros that not used for winsim or verilog, but needed for compilation  */
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
#endif
