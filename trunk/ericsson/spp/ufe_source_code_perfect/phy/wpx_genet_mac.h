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

#ifndef WPX_GENET_MAC_H
#define WPX_GENET_MAC_H

#include "api/wp_types.h"

WP_status WPU_GenetMacInit(WP_U32 extend_turnoff);
void *WPU_GenetMacBase(WP_U32 port_id);

#endif
