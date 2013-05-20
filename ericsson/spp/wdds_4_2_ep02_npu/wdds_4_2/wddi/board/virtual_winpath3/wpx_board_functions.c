/*****************************************************************************
 * (C) Copyright 2003-2006, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

#ifndef WP_TYPES_H
#include "api/wp_types.h"
#endif
#ifndef WP_ERRORS_H
#include "api/wp_errors.h"
#endif
#ifndef WPI_BOARD_DATA_INT_H
#include "veneer/wpi_board_data_int.h"
#endif
#include "../board/wpx_board_if.h"

/*****************************************************************************
 *
 * File: wpx_reboot.c
 *
 * For:  Board virtual
 *
 * Purpose: Reboot if it is permitted by the target.
 ****************************************************************************/

void WPX_Reboot(void)
{
}

/*******************************************************************
 * Function to indicate some progress.
 * For Winsim, this does nothing.
 *******************************************************************/
WP_status WP_IndicateProgress(void)
{
    return WP_OK;
}

WP_U32 WPX_CpldSet(WP_U32 port, WP_U32 mode)
{
   return WP_OK;
}

WP_status WPX_BoardConfigure(WP_U32 wpid, wpx_board_configuration configuration)
{
   return WP_OK;
}

/* temporary defined here, must be winutil function */
WP_status WPUI_TraceEnableDefault(void)
{
   return WP_OK;
}

WP_status WPX_BoardExtCardInit(WP_U32 mode, WP_U32 connector, wpi_match_mask *save)
{
   return WP_OK;
}

void WPX_FpgaFirmwareInitializationStart(WP_U32 wpid)
{
   return;
   
}
