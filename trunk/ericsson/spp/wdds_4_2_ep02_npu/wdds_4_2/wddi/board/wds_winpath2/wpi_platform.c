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
#ifndef WPI_WDDI_TYPEDEFS_H
#include "include/core/registry/wpi_wddi_typedefs.h"
#endif
#ifndef WPI_CAPABILITY_H
#include "include/core/features/wpi_capability.h"
#endif
#ifndef WPI_HANDLE_H
#include "include/core/registry/wpi_handle.h"
#endif
#ifndef WPI_REGISTRY_H
#include "include/core/registry/wpi_registry.h"
#endif

#define WPI_CAP_CALL(x) WPI_RETURN_IF_ERROR(status, x)

/************************************************************************
 *
 * Function:  WPI_CapabilityPlatform
 *
 * Purpose:   Initialize capabilities for a platform
 *
 * Description:  Check that a capability structure has been allocated.
 *
 *               Bugfixes:       Disable.  CapabilityFamily will set or clear.
 *               Bus present:    Enable, unless known that a bus is not used.
 *               Serial config:  Enable, unless known that a serial is not present.
 *                               CapabilityFamily will further disable based on version/registers.
 *               BRG config:     Enable.  CapabilityFamily can reduce availability.
 *               Capabilities:   Zero.  CapabilityFamily will set or clear.
 *               Quantities:     Zero.  CapabilityFamily will set proper values.
 *
 *               Enable other capabilities.  A separate function will do the
 *               additional analysis based on processor registers.
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
   WP_S32 phy;
   WP_status status;

   /* Check argument */
   if (wpid >= WP_MAX_WINPATHS)
      return WPI_ERROR(WP_ERR_CAPABILITY_PLATFORM);

   /************* Bugfixes *************/

   /************* Clock recovery *************/
   WPI_CAP_CALL(WPI_CapabilitySet(wpid, WPI_CAP_CLK_REC_GLOBAL_DCO_INIT, 1));
   WPI_CAP_CALL(WPI_CapabilitySet(wpid, WPI_CAP_CLK_REC_TIMESTAMP_LOGIC, 0));

   /************* Buses **************/
   /* Enable buses */
   for (i = 0; i < 4; i++)
      WPI_CAP_CALL(WPI_CapabilitySetIndexed(wpid, WPI_CAP_BUS_PRESENT, i, 1));

   /************* Serial configuration **************/
   /* Configure UPI */
   for (i = 0; i < 2; i++) {
      WPI_CAP_CALL(WPI_CapabilitySetIndexed(wpid, WPI_CAP_UPI_BUS_WIDTH_8, i, 1));
      WPI_CAP_CALL(WPI_CapabilitySetIndexed(wpid, WPI_CAP_UPI_BUS_WIDTH_16, i, 1));
      WPI_CAP_CALL(WPI_CapabilitySetIndexed(wpid, WPI_CAP_UPI_EMPHY2, i, 1));
      WPI_CAP_CALL(WPI_CapabilitySetIndexed(wpid, WPI_CAP_UPI_EMPHY2_12, i, 0));
      WPI_CAP_CALL(WPI_CapabilitySetIndexed(wpid, WPI_CAP_UPI_POS_2, i, 1));
      WPI_CAP_CALL(WPI_CapabilitySetIndexed(wpid, WPI_CAP_UPI_POS_SLAVE, i, 1));
      WPI_CAP_CALL(WPI_CapabilitySetIndexed(wpid, WPI_CAP_UPI_UTOPIA_2, i, 1));
      WPI_CAP_CALL(WPI_CapabilitySetIndexed(wpid, WPI_CAP_UPI_POS_MIB, i, 1));
      for (phy = 0; phy < 127; phy++) {
         WPI_CAP_CALL(WPI_CapabilitySetDoublyIndexed(wpid, WPI_CAP_UPI_PHY_AVAILABLE, i, phy, 1));
      }
   }

   /* Configure ENET -- WEB2 supports MII */
   for (i = 0; i < 2; i++) {
      WPI_CAP_CALL(WPI_CapabilitySetDoublyIndexed(wpid, WPI_CAP_ENET_GMII, i, 0, 1));
      WPI_CAP_CALL(WPI_CapabilitySetDoublyIndexed(wpid, WPI_CAP_ENET_TBI, i, 0, 1));
      WPI_CAP_CALL(WPI_CapabilitySetDoublyIndexed(wpid, WPI_CAP_ENET_MII, i, 0, 1));
      WPI_CAP_CALL(WPI_CapabilitySetDoublyIndexed(wpid, WPI_CAP_ENET_RMII, i, 0, 1));
}
   /* Configure TDI -- WEB2 does not support 8 pins or CT */

   for (i = 0; i < 16; i++) {
      WPI_CAP_CALL(WPI_CapabilitySetIndexed(wpid, WPI_CAP_TDI_6_PIN, i, 1));
      WPI_CAP_CALL(WPI_CapabilitySetIndexed(wpid, WPI_CAP_TDI_8_PIN, i, 1));
      WPI_CAP_CALL(WPI_CapabilitySetIndexed(wpid, WPI_CAP_TDI_CAS, i, 1));
      WPI_CAP_CALL(WPI_CapabilitySetIndexed(wpid, WPI_CAP_TDI_CT, i, 1));
      WPI_CAP_CALL(WPI_CapabilitySetIndexed(wpid, WPI_CAP_TDI_CT_CAS, i, 1));
   }

   /* Configure BRG -- WEB2 supports it. */
   for (i = 0; i < 8; i++) {
      WPI_CAP_CALL(WPI_CapabilitySetIndexed(wpid, WPI_CAP_BRG_IN, i, (i < 4)));
      WPI_CAP_CALL(WPI_CapabilitySetIndexed(wpid, WPI_CAP_BRG_OUT, i, 1));
   }

   return WP_OK;
}
