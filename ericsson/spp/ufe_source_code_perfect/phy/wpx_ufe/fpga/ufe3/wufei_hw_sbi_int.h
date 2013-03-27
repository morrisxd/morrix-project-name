/*****************************************************************************
 * (C) Copyright 2000-2006, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: wufei_hw_sbi_int.h
 *
 * Purpose: The UFE SBI function declaration
 *
 ****************************************************************************/
#ifndef WUFEI_HW_SBI_INT_H
#define WUFEI_HW_SBI_INT_H

#ifndef WUFE_TYPES_H
#include "wufe_types.h"
#endif
#ifndef WUFEI_REGISTRY_H
#include "wufei_registry.h"
#endif
#ifndef WUFEI_CLK_TRANSLATOR_H
#include "wufei_clk_translator.h"
#endif

WUFE_status WUFEI_HwSbiGmrSetup(WUFEI_system_registry* system_registry);
WUFE_status WUFEI_HwSbiSpeModeSetup(WUFEI_system_registry* system_registry, WP_U32 coreid);
WUFE_status WUFEI_HwSbiRamSetup(WUFEI_system_registry* system_registry, WP_U32 coreid);
WUFE_status WUFEI_HwSbiGmrDisable(WUFEI_system_registry* system_registry,
                                  WP_U32 property);
WUFE_status WUFEI_HwSbiGmrEnable(WUFEI_system_registry* system_registry,
                                 WP_U32 property);
WUFE_status WUFEI_HwSbiSpeModeDisable(WUFEI_system_registry* system_registry,
                                      WP_U32 property, WP_U32 coreid);
WUFE_status WUFEI_HwSbiSpeModeEnable(WUFEI_system_registry* system_registry,
                                     WP_U32 property, WP_U32 coreid);
WUFE_status WUFEI_HwSbiGmrReconfigure(WUFEI_system_registry* system_registry,
                                      WUFE_sbi_global_reconfig *config);
WUFE_status WUFEI_HwSbiSpeModeReconfigure(WP_U32 ufeid, WP_U32 coreid,
                                          WP_U32 stm1, WP_U32 spe,
                                          WUFE_line_transf_type spe_type);
WUFE_status WUFEI_HwSbiRamConfig( WUFEI_system_registry *system_registry,
                                  WUFEI_line_registry   *line_registry);
WUFE_status WUFEI_HwSbiRxConfig(WUFEI_system_registry *system_registry,
                                WUFEI_line_registry *line_registry,
                                WP_U8 enable);
WUFE_status WUFEI_HwSbiTxConfig(WUFEI_system_registry *system_registry,
                                WUFEI_line_registry *line_registry,
                                WP_U8 enable);
WUFE_status WUFE_HwSbiRxDisable(WUFEI_line_registry *line_registry,
                                WP_U32 wufe_id);
WUFE_status WUFE_HwSbiTxDisable(WUFEI_line_registry *line_registry,
                                WP_U32 wufe_id);
WUFE_status WUFEI_HwSbiLineDisplayE1T1Rx(WUFEI_line_registry *line_registry, WP_U32 wufe_id);
WUFE_status WUFEI_HwSbiLineDisplayE1T1Tx(WUFEI_line_registry *line_registry, WP_U32 wufe_id);
WUFE_status WUFEI_HwSbiLineDisplayDS3E3Rx(WUFEI_line_registry *line_registry, WP_U32 wufe_id);
WUFE_status WUFEI_HwSbiLineDisplayDS3E3Tx(WUFEI_line_registry *line_registry, WP_U32 wufe_id);
WUFE_status WUFEI_HwSbiLineReconfigure(WP_U32 wufe_id,
                                       WUFEI_system_registry *system_registry,
                                       WUFEI_line_registry *line_registry,
                                       WUFE_line_sbi_reconfig *config);
WUFE_status WUFEI_HwSbiRamDelete(WUFEI_system_registry *system_registry,
                                 WUFEI_line_registry   *line_registry, WP_U32 core_id);

void WUFEI_HwSbiEventRegSet(WP_U32 ufeid, WP_U16 value);
void WUFEI_HwSbiEventRegGet(WP_U32 ufeid, WP_U16* value);
void WUFEI_HwSbiEventMaskRegSet(WP_U32 ufeid, WP_U16 value);
void WUFEI_HwSbiEventMaskRegGet(WP_U32 ufeid, WP_U16* value);
void WUFEI_HwSbiGlobalDisplay(WUFEI_system_registry *system_registry);

WUFE_status WUFEI_HwSbiSlotTxCasDisplay(WUFEI_line_registry *line_registry, WP_U32 slot, WP_U32 wufe_id);
WUFE_status WUFEI_HwSbiSlotRxCasDisplay(WUFEI_line_registry *line_registry, WP_U32 slot, WP_U32 wufe_id);
WUFE_status WUFEI_HwSbiE3AtmClear(WP_U32 ufeid,
                                  WUFEI_line_registry *line_registry,
                                  WP_U32 direction);
WUFE_status WUFEI_HwSbiE3AtmConfig(WP_U32 ufeid,
                                   WUFEI_line_registry *line_registry,
                                   WP_U32 direction);

#endif /* WUFEI_HW_SBI_INT_H */
