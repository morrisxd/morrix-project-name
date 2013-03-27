/*****************************************************************************
 * (C) Copyright 2000-2006, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: wufei_hw_cad_int.h
 *
 * Purpose: The UFE Cad function declaration
 *
 ****************************************************************************/
#ifndef WUFEI_HW_CAD_INT_H
#define WUFEI_HW_CAD_INT_H

#ifndef WUFE_TYPES_H
#include "wufe_types.h"
#endif

WUFE_status WUFEI_HwCadSetup(WP_U32 ufeid, WP_U32 max_n_cad);
void WUFEI_HwCadModeSet(WP_U32 ufeid, WP_U32 cad_id, WP_U16 value);
void WUFEI_HwCadIdSet(WP_U32 ufeid, WP_U32 cad_id, WP_U16 value);
WP_U32 WUFEI_HwCadModeGet(WP_U32 ufeid, WP_U32 cad_id);
WP_U32 WUFEI_HwCadIdGet(WP_U32 ufeid, WP_U32 cad_id);
WUFE_status WUFEI_HwCadCheckAndConfig(WP_U16               *rx_cad_mod,
                                      WUFE_line_cad        *line_cfg,
                                      WUFEI_line_registry  *line_registry);
WUFE_status WUFEI_HwCadTdmConfig(WUFEI_system_registry *system_registry,
                                 WUFEI_line_registry *line_registry,
                                 WP_U16 *hw_cad_tdm_ram_id);

#endif /* WUFEI_HW_CAD_INT_H */
