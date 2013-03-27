/*****************************************************************************
 * (C) Copyright 2001-2006, Wintegra.  All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/************************************************************************/
/*                     GENET CARD INITIALIZATION                        */
/************************************************************************/

/* This utility function maps the chip-select for the Gigabit
   Ethernet card */

#include <stdio.h>

#ifndef WP_TYPES_H
#include "api/wp_types.h"
#endif
#ifndef WP_WDDI_H
#include "api/wp_wddi.h"
#endif
#ifndef WP_PROTOTYPES_H
#include "api/wp_prototypes.h"
#endif
#ifndef WPI_REG_H
#include "include/core/hardware/wpi_reg.h"
#endif
#ifndef WPI_REGISTRY_H
#include "include/core/registry/wpi_registry.h"
#endif
#include "wpx_genet_mac.h"
#include "wpx_phy_data.h"
#include "veneer/wpi_board_data_int.h"

WP_status WPU_GenetMacInit(WP_U32 extend_turnoff)
{
   if (extend_turnoff == WP_TRUE)
   {
      wpi_board_data.match_mask->genet[0].mask_value = 0xfff0d028;
      wpi_board_data.match_mask->genet[1].mask_value = 0xfff0d028;
   }
   else
   {
      wpi_board_data.match_mask->genet[0].mask_value = 0xfff0d020;
      wpi_board_data.match_mask->genet[1].mask_value = 0xfff0d020;
   }
   return WP_OK;
}

void *WPU_GenetMacBase(WP_U32 port_id) {
   if (port_id == WP_PORT_UPI1)
      return (void *) WPL_PHY_VIRTUAL(0, APP_GENET1_MAC_BASE);
   else if (port_id == WP_PORT_UPI2)
      return (void *) WPL_PHY_VIRTUAL(0, APP_GENET2_MAC_BASE);
   else
      return 0;
}
