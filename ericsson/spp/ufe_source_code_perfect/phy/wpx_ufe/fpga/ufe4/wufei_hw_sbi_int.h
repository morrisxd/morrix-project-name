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

#define WUFEI_HwSbiGmrSetup( system_registry) WUFE_OK
#define WUFEI_HwSbiSpeModeSetup( system_registry,  coreid) WUFE_OK
#define WUFEI_HwSbiRamSetup( system_registry,  coreid) WUFE_OK
#define WUFEI_HwSbiGmrDisable( system_registry,property) WUFE_OK
#define WUFEI_HwSbiGmrEnable( system_registry, property) WUFE_OK
#define WUFEI_HwSbiSpeModeDisable( system_registry,property,  coreid) WUFE_OK
#define WUFEI_HwSbiSpeModeEnable( system_registry,property,  coreid) WUFE_OK
#define WUFEI_HwSbiGmrReconfigure( system_registry,config) WUFE_OK
#define WUFEI_HwSbiSpeModeReconfigure( ufeid,  coreid,stm1,  spe,spe_type) WUFE_OK
#define WUFEI_HwSbiRamConfig( system_registry,line_registry) WUFE_OK
#define WUFEI_HwSbiRxConfig(system_registry,line_registry,enable) WUFE_OK
#define WUFEI_HwSbiTxConfig(system_registry,line_registry, enable) WUFE_OK
#define WUFE_HwSbiRxDisable(line_registry,wufe_id) WUFE_OK
#define WUFE_HwSbiTxDisable(line_registry,wufe_id) WUFE_OK
#define WUFEI_HwSbiLineDisplayE1T1Rx(line_registry,  wufe_id) WUFE_OK
#define WUFEI_HwSbiLineDisplayE1T1Tx(line_registry,  wufe_id) WUFE_OK
#define WUFEI_HwSbiLineDisplayDS3E3Rx(line_registry,  wufe_id) WUFE_OK
#define WUFEI_HwSbiLineDisplayDS3E3Tx(line_registry,  wufe_id) WUFE_OK
#define WUFEI_HwSbiLineReconfigure( wufe_id,system_registry,line_registry,config) WUFE_OK
#define WUFEI_HwSbiRamDelete(system_registry,line_registry,  core_id) WUFE_OK

#define WUFEI_HwSbiEventRegSet( ufeid,  value)
#define WUFEI_HwSbiEventRegGet( ufeid,  value)
#define WUFEI_HwSbiEventMaskRegSet( ufeid,  value)
#define WUFEI_HwSbiEventMaskRegGet( ufeid,  value)
#define WUFEI_HwSbiGlobalDisplay(system_registry)

#define WUFEI_HwSbiSlotTxCasDisplay(line_registry,  slot,  wufe_id) WUFE_OK
#define WUFEI_HwSbiSlotRxCasDisplay(line_registry,  slot,  wufe_id) WUFE_OK

#define WUFEI_HwSbiE3AtmClear(ufeid,line_registry,direction) WUFE_OK
#define WUFEI_HwSbiE3AtmConfig(ufeid,line_registry,direction) WUFE_OK

#endif /* WUFEI_HW_SBI_INT_H */
