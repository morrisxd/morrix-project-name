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

#ifndef WPX_GEI_PHY_H
#define WPX_GEI_PHY_H

#include "api/wp_types.h"
#include "wpx_enet_phy.h"

/* Defined modes of operation */
/*#define WPU_MODE_GMII_OV_FIBER           0x10000
#define WPU_MODE_GMII_OV_COPPER          0x20000
#define WPU_MODE_GMII_OV_FIBER_ANEG_OFF  0x30000
#define WPU_MODE_GMII_OV_COPPER_ANEG_OFF 0x40000
#define WPU_MODE_MII_ANEG_ON             0x50000
#define WPU_MODE_TBI                     0x60000
#define WPU_MODE_GMII_MODE_AUTO_DETECT   0x70000

#define WPU_MODE_GEI_LOOPBACK_OFF 0
#define WPU_MODE_GEI_LOOPBACK_ON 1
*/
WP_status WPU_GeiPhyInit(WP_U32 port, WP_U32 mode);
#endif
