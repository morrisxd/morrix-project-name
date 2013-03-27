/*****************************************************************************
 * (C) Copyright 2001-2006, Wintegra.  All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/



#include <stdio.h>
#include "api/wp_types.h"
#include "include/core/hardware/wpi_reg.h"
#include "api/wp_wddi.h"
#include "veneer/wpi_board_data_int.h"

WP_status WPX_SetBank(wpi_match_mask *saved, wpi_match_mask *program)
{
   WP_U32 *if_addr;

    saved->match_address = program->match_address;
   /* Get the virtual address of the match/mask register pair. */
   if_addr = (WP_U32 *) WPL_PHY_VIRTUAL(0, program->match_address);

   /* Save the current match/mask values. */
   WPI_REG_GET(saved->match_value, if_addr[0]);
   WPI_REG_GET(saved->mask_value, if_addr[1]);

   /* Install the desired match/mask values. */
   if (program->mask_value != saved->mask_value ||
       program->match_value != saved->match_value)
   {
     WPI_REG_SET(if_addr[1], program->mask_value);
     WPI_REG_SET(if_addr[0], program->match_value);
   }

   return WP_OK;
}

WP_status WPX_RestoreBank(wpi_match_mask *saved)
{
   WP_U32 *if_addr;

   /* in WP3 WPX_RestoreBank() should do nothing */
   if(saved->match_address == 0xffffffff)
      return WP_OK;

   /* Get the virtual address of the match/mask register pair. */
   if_addr = (WP_U32 *) WPL_PHY_VIRTUAL(0, saved->match_address);

   /* Install the desired match/mask values. */
   WPI_REG_SET(if_addr[1], saved->mask_value);
   WPI_REG_SET(if_addr[0], saved->match_value);

   return WP_OK;
}
