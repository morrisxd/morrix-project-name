/*****************************************************************************
 * (C) Copyright 2000-2006, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/****************************************************************************
 *
 * Example: wt_tmx336_util.h
 * Demonstrates prototypes and definitions of UFE2 utility functions
 *
 ****************************************************************************/
#ifndef WT_TMX336_UTIL_H
#define WT_TMX336_UTIL_H

#ifdef __WT_PMC__
#ifdef __WT_UFE3__

#ifndef WPX_PMC_CONFIG_H
#include "wpx_pmc_config.h"
#endif

#define WT_TMX336_DBG_MODE 0

/* mode 0-internal, 1-loopt; type 0-t1 unframed, 1-t1 framed, 2-e1 unframed, 3-e1 framed */
int WT_Tmx336SbiSetup(int mode, int type);

#endif /*__WT_PMC__*/
#endif /*__WT_UFE3__*/

#endif /*WT_TMX336_UTIL_H*/
