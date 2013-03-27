/*****************************************************************************
 * (C) Copyright 2001-2006, Wintegra.  All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/************************************************************************/
/*                      WINNET BLOCK INITIALIZATION                     */
/************************************************************************/

#ifndef WPX_ENET_PHY_H
#define WPX_ENET_PHY_H

#include "api/wp_types.h"

WP_status WPU_EnetPhyInit(WP_U32 port, WP_U32 mode);

/* Defined modes of operation */
/************** web2 **************/
#define WPU_MODE_GEI_LOOPBACK_OFF 0
#define WPU_MODE_GEI_LOOPBACK_ON 1

#define WPU_MODE_GMII_OV_FIBER           0x10000
#define WPU_MODE_GMII_OV_COPPER          0x20000
#define WPU_MODE_GMII_OV_FIBER_ANEG_OFF  0x30000
#define WPU_MODE_GMII_OV_COPPER_ANEG_OFF 0x40000
#define WPU_MODE_MII_ANEG_ON             0x50000
#define WPU_MODE_GMII_MODE_AUTO_DETECT   0x60000

/************** wds2 **************/
#define WPU_MODE_MII           0x90000

/**** common to web2 and wds2 *****/
#define WPU_MODE_TBI                     0xe0000
#define WPU_MODE_TBI_AN                  0xf0000

/**** common to wds2 and wds3 *****/
#define WPU_MODE_RGMII         0x70000
#define WPU_MODE_GMII          0x80000
#define WPU_MODE_RTBI          0xa0000
#define WPU_MODE_RMII          0xb0000
#define WPU_MODE_SMII          0xc0000
#define WPU_MODE_SS_SMII       0xd0000

/************** wds3 ****************/
#define WPU_MODE_SGMII        0x100000

/* Media */
#define WPU_OV_COPPER          0x00100
#define WPU_OV_FIBER           0x00200
#define WPU_OV_AUTODETECT      0x00300

/* Slot */
#define WPU_SLOT_UPI1          0x01000
#define WPU_SLOT_UPI2          0x02000
#define WPU_SLOT_TDI           0x03000
#define WPU_SLOT_WNET          0x04000

/* Auto negotiation */
#define WPU_ANEG_ON            0x00010
#define WPU_ANEG_OFF           0x00020

/* Masks for extracting the modes of cards */
#define WPU_MAJOR_MODE_MASK    0xff0000
#define WPU_SLOT_MASK          0x00f000
#define WPU_MEDIA_MASK         0x000f00
#define WPU_PHY_LOOPBACH_MASK  0x000001
#define WPU_PHY_AUTONEG_MASK   0x000030
#endif /* WPX_ENET_PHY_H */
