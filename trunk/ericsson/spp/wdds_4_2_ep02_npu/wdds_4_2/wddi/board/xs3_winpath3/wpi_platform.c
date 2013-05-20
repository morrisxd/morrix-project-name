/*************************************************************
 * (C) Copyright 2004-2005, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ************************************************************/

/*****************************************************************************
 *
 * File: wpi_platform.c
 *
 * Purpose:  Platform-specific capability initialization
 *           Each platform can have a file with this name.
 *           In a given build, one platform will be selected.
 *
 *           This file builds for platform WEB2.
 *
 * Functions contained in this file:
 * +----------------------------+---------------------------------------------
 * |   Function name            | Description
 * +----------------------------+---------------------------------------------
 * |   WPI_CapabilityPlatform   | Platform-specific capability initialization
 * +----------------------------+---------------------------------------------
 ****************************************************************************/
#include <stdio.h>

#ifndef WPI_TYPES_H
#include "include/core/registry/wpi_types.h"
#endif
#ifndef WPI_CAPABILITY_H
#include "include/core/features/wpi_capability.h"
#endif
#ifndef WPI_HANDLE_H
#include "include/core/registry/wpi_handle.h"
#endif


#define WPI_CAP_CALL(x) WPI_RETURN_IF_ERROR(status, x)

/************************************************************************
 *
 * Function:  WPI_CapabilityPlatform
 *
 * Purpose:   Initialize capabilities for a board
 *
 * Description:  Sets the board specific parameters
 *
 * Inputs:    wpid         Which wpid is used.
 *
 * Outputs:   wpi_registry.pwp_block[wpid].capability->xxx
 *                 has platform-specific initialization done.
 *
 * Return Value: WP_OK
 *               WP_ERR_CAPABILITY
 *
 * Called By:  WP_SysInit
 *
 **************************************************************************/
WP_status WPI_CapabilityPlatform(WP_U32 wpid)
{
   WP_S32 i;
   WP_status status;

   /* Check argument */
   if (wpid >= WP_MAX_WINPATHS)
      return WPI_ERROR(WP_ERR_CAPABILITY_PLATFORM);

   /* Enable buses */
   for (i = 0; i < 4; i++)
      WPI_CAP_CALL(WPI_CapabilitySetIndexed(wpid, WPI_CAP_BUS_PRESENT, i, 1));

   return WP_OK;
}
