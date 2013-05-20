/*****************************************************************************
 * (C) Copyright 2003-2006, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: wpx_board_if.h
 *
 * Purpose: Define board-independent interfaces.
 *
 * The functions used in this file let an application interact with a board
 * through ways that may or may not covered by the WDDI API.
 *
 * This file must not be included by any files in wddi/sources.
 *
 ****************************************************************************/

#ifndef WPX_BOARD_IF_H
#define WPX_BOARD_IF_H

#ifndef WPI_BOARD_DATA_H
#include "veneer/wpi_board_data_int.h"
#endif

#define WPX_SERDES_LOOPBACK 1
#define WPX_SERDES_NORMAL 0

// XALYO
#define WPX_SERDES_INTERNAL_CLOCK 0x80
#define WPX_SERDES_EXTERNAL_CLOCK 0

void WPX_Reboot(void);

/*****************************************************************************
 *
 * Board configuration is done by calling WPX_BoardConfigure
 * It should configure the board for the requested purpose,
 * setting GPIO, CPLD, or whatever else is necessary.
 *
 * The functions will typically be implemented in the board/<board>/wpx_board_functions.c .
 * If the function can program the board correctly, it does so and returns WP_OK.
 * If the function cannot program the board correctly, it returns WP_ERR_CONFIGURATION.
 *
 ****************************************************************************/

typedef enum{
   WPX_CONFIGURE_DEFAULT_CHECKIN,             /* 0 - TDI1-8,UPI1, UPI2 for web, wds2,
                                                 XGI1 and 10 SGMII for wds3*/
      WPX_CONFIGURE_CHECKIN_TDI_16,           /* 1 - 16 TDI/RTDI, UPI1 for web, wds2,
                                                 16 TDI, 10 SGMII for wds3 */
      WPX_CONFIGURE_UFE_UPI2_TDM,             /* 2 - UFE with OCTAL over UPI2 over WDS2 */
      WPX_CONFIGURE_TDI_16_UPI1,              /* 3 - web and wds2 */
      WPX_CONFIGURE_2UPI_2XGI_6SGMII2G,       /* 4 - wds3 */
      WPX_CONFIGURE_1UPI_1XGI_4GMII_8SGMII,   /* 5 - wds3 */
      WPX_CONFIGURE_1XGI_6RGMII_6SGMII,       /* 6 - wds3 */
      WPX_CONFIGURE_1UPI_1XGI_6RTBI_6SGMII,   /* 7 - wds3 */
      WPX_CONFIGURE_1XGI_6SMII_6SGMII,        /* 8 - wds3 */
      WPX_CONFIGURE_2RMII_8TDI_1XGI_10SGMII,  /* 9 - wds3 */
      WPX_CONFIGURE_8TDI_1UPI_1XGI_10SGMII,   /* 10 - wds3 */
      WPX_CONFIGURE_2UPI_1XGI_10SGMII,        /* 11 - wds3 */
      WPX_CONFIGURE_2UPI_1XGI_10SGMII_UFE4,   /* 12 - wds3 */
      WPX_CONFIGURE_2UPI_16bit_1XGI_10SGMII,  /* 13 - wds3 */
      WPX_CONFIGURE_BRG_MUX,                  /* 14 - wds3 */
      WPX_CONFIGURE_PPS_FCLK_SELECT,          /* 15 - wds3 */
      WPX_CONFIGURE_SMA_DIRECTION,            /* 16 - wds3 */
      WPX_CONFIGURE_2SGMII_2SGMII2G_16TDI,    /* 17 - wds3 */
      WPX_CONFIGURE_2UPI_1XGI_10SGMII_UPI_CLK_100M, /* 18 - wds3 */
      WPX_CONFIGURE_6SGMII_1SGMII2G_2SMII,    /* 19 - wds3*/
      WPX_CONFIGURE_2UPI_L3_2XGI_6SGMII,      /* 20 - wds3*/
      WPX_CONFIGURE_UPI1_TDI8_16_10SGMII,     /* 21 - wds3 */
      WPX_CONFIGURE_2XGI_6SGMII,              /* 22 - wds3*/
      WPX_CONFIGURE_2UPI_2XGI_6SGMII2G_UFE448, /* 23 - wds3 */
      WPX_CONFIGURE_UPI3_UFE412,              /* 24 - wds3 */

    // XALYO
    WPX_CONFIGURE_XS_1UPI_10SGMII,          /* 25 xs3 */
    WPX_CONFIGURE_XS_1UPI_1XGI_S2_6SGMII,   /* 26 xs3 */
    WPX_CONFIGURE_XS_1UPI_1XGI_S3_6SGMII,   /* 27 xs3 */
    WPX_CONFIGURE_XS_1UPI_2XGI_2SGMII,      /* 28 xs3 */
    WPX_CONFIGURE_XS_1POS_10SGMII,          /* 29 xs3 */
    WPX_CONFIGURE_XS_1POS_1XGI_S2_6SGMII,   /* 30 xs3 */
    WPX_CONFIGURE_XS_1POS_1XGI_S3_6SGMII,   /* 31 xs3 */
    WPX_CONFIGURE_XS_1POS_2XGI_2SGMII,      /* 32 xs3 */
    WPX_CONFIGURE_XS_3200,                  /* 33 xs3200 */
    WPX_CONFIGURE_XS_1UPI_2SGMII,           /* 34 xs3100 ATM */
    WPX_CONFIGURE_XS_1POS_2SGMII,           /* 35 xs3100 POS */

      WPX_CONFIGURE_SERDES_REF_CLK,           /* wds3 */
      WPX_CONFIGURE_PLL_SOURCE,               /* wds3 */
      WPX_CONFIGURE_MAX_CONFIGURATIONS        
}wpx_board_configuration;

typedef enum{
   WPX_MODE_OC12_PMC5357,
   WPX_MODE_OC3_PMC5351,
   WPX_MODE_FPGA_DC,
}wpx_board_mode;

typedef enum{
   WPX_SERIAL_CONNECTOR_0,
   WPX_SERIAL_CONNECTOR_1,
   WPX_SERIAL_CONNECTOR_2,
}wpx_board_connector;

WP_status WPX_BoardConfigure(WP_U32 wpid, wpx_board_configuration configuration);

WP_status WPX_BoardExtCardInit(WP_U32 mode, WP_U32 connector, wpi_match_mask *save);

WP_status WPX_SetBank(wpi_match_mask *saved, wpi_match_mask *program);

WP_status WPX_RestoreBank(wpi_match_mask *saved);

#endif
