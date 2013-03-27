/*****************************************************************************
 * (C) Copyright 2001-2006, Wintegra.  All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/************************************************************************/
/*                      PIN MUX INITIALIZATION                          */
/************************************************************************/


#ifndef WPX_PIN_MUX_SELECTOR_H
#define WPX_PIN_MUX_SELECTOR_H

#include "api/wp_types.h"

#define WPU_PIN_MUX_MODE_DEFAULT 0
#define WPU_PIN_MUX_MODE_TBI_ENABLE 1

#define WPU_GMII1_GPMR_F_TBI_EN_MASK 0x00000100
#define WPU_GMII2_GPMR_F_TBI_EN_MASK 0x01000000


#define WPU_GMII1_GPMR_F_MASK 0x00000001
#define WPU_GMII1_GPMR_G_MASK 0x800003CF
#define WPU_GMII2_GPMR_F_MASK 0x00010000
#define WPU_GMII2_GPMR_G_MASK 0x400F3C00
#define WPU_GMII1_BOARDMUX_MASK 0x08000000
#define WPU_GMII2_BOARDMUX_MASK 0x04000000

WP_status WPU_PinMuxAllocateToPort(WP_U32 port,WP_U32 mode);

#endif
