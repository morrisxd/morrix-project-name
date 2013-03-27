/*****************************************************************************
 * (C) Copyright 2001-2006, Wintegra.  All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/************************************************************************/
/*                      OC12 CARD INITIALIZATION                        */
/************************************************************************/

/* This initialization code assumes that the host port for the PMC5357
   is implemented as a 64-bit port. */

/* Note: This is a preliminary release, which assumes a default memory map. */

#ifndef WPX_OC_COMMON_H
#define WPX_OC_COMMON_H

#include "api/wp_types.h"
#include "wpx_board_data.h"

#define WPU_64_BYTES  0x000000
#define WPU_128_BYTES 0x100000


typedef struct
{
   WP_U8 reg[WPX_PHY_REG_MULTIPLIER];
}wpx_card_reg_size;

#endif
