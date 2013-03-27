/*****************************************************************************
 * (C) Copyright 2000-2010, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: wufei_hw_machine_int.h
 *
 * Purpose: The UFE Machine function declaration
 *
 ****************************************************************************/
#ifndef WUFEI_HW_MACHINE_INT_H
#define WUFEI_HW_MACHINE_INT_H

#ifndef WUFE_TYPES_H
#include "wufe_types.h"
#endif
#ifndef WUFEI_REGISTRY_H
#include "wufei_registry.h"
#endif

#define WUFEI_HwCesPhyEnable( system_registry,  coreid,  phy_id,  direction) WUFE_OK
#define WUFEI_HwCesPhyDisable( system_registry,  coreid,  phy_id,  direction) WUFE_OK
#define WUFEI_HwCesConfigureSlots( ufeid,  coreid, slot_group, phy_id, direction) WUFE_OK
#define WUFEI_HwCesPhyCreate( system_registry, coreid, phy_id, slot_group, data_unit_size,direction) WUFE_OK
#define WUFEI_HwPhyDelete( system_registry, coreid, phy_id,direction) WUFE_OK
#define WUFEI_HwCesCasConfig( system_registry, coreid, line_id) WUFE_OK
#define WUFEI_HwCesCasDisplay(system_registry, coreid, line_id)
#define WUFEI_HwRptDisplay( system_registry, coreid, phy_id)
#define WUFEI_HwTptDisplay( system_registry, coreid, phy_id)
#define WUFEI_HwMachineEventRegSet( ufeid,  coreid,  value) WUFE_OK
#define WUFEI_HwMachineEventRegGet( ufeid,  coreid,  value) WUFE_OK
#define WUFEI_HwMachineEventMaskRegSet( ufeid,  coreid,  value) WUFE_OK
#define WUFEI_HwMachineEventMaskRegGet( ufeid,  coreid,  value) WUFE_OK
#define WUFEI_HwAtmPhyEnable(system_registry, coreid, phy_id, direction) WUFE_OK
#define WUFEI_HwAtmPhyDisable(system_registry, coreid, phy_id, direction) WUFE_OK
#define WUFEI_HwAtmPhyStateGet(system_registry,coreid,phy_id,sync_state) WUFE_OK
#define WUFEI_HwAtmPhyCreate(system_registry,coreid,phy_id,ufe_phy_cfg,direction) WUFE_OK
#define WUFEI_HwAtmPhyUnderrunGet(system_registry,coreid,phy_id) WUFE_OK
#define WUFEI_HwCesPhyUnderrunGet(system_registry,coreid,phy_id) WUFE_OK
#define WUFEI_HwHdlcPhyEnable(system_registry,coreid,phy_id,direction) WUFE_OK
#define WUFEI_HwHdlcPhyDisable(system_registry,coreid,phy_id,direction) WUFE_OK
#define WUFEI_HwHdlcPhyCreate(system_registry,coreid,phy_id,ufe_phy_cfg,direction) WUFE_OK
#define WUFEI_HwHdlcPhyUnderrunGet(system_registry,coreid,phy_id) WUFE_OK
#define WUFEI_HwHdlcMinFlagsReconfig(id,coreid,phy_id,min_flags) WUFE_OK
#define WUFEI_HwHdlcNoFlagsReconfig(id,coreid,phy_id,no_flags) WUFE_OK
#define WUFEI_HwHdlcCrcReconfig(id,coreid,phy_id,direction,crc_size) WUFE_OK
#define WUFEI_HwAtmScramblingReconfig( ufeid, coreid, phy_id, direction, scrambler_enable) WUFE_OK
#define WUFEI_HwAtmCosetReconfig(ufeid, coreid, phy_id, direction, coset_enable) WUFE_OK
#define WUFEI_HwAtmIdleCellModeReconfig(ufeid, coreid, phy_id, idle_cell_enable) WUFE_OK
#define WUFEI_HwAtmUnassignedCellModeReconfig(ufeid, coreid, phy_id, unassigned_cell_enable) WUFE_OK
#define WUFEI_HwAtmHecErrorDiscardModeReconfig(ufeid, coreid, phy_id, hec_error_discard) WUFE_OK
#define WUFEI_HwAtmHecErrorCorrectionModeReconfig(ufeid, coreid, phy_id, hec_error_correction) WUFE_OK
#define WUFEI_HwAtmCellDelineationModeReconfig(ufeid, coreid, phy_id, cell_delineation) WUFE_OK
#define WUFEI_HwAtmRxAlphaReconfig(ufeid, coreid, phy_id, rx_alpha) WUFE_OK
#define WUFEI_HwAtmRxDeltaReconfig(ufeid, coreid, phy_id, rx_delta) WUFE_OK
#define WUFEI_HwAtmIdleUnassignedCellModeReconfig(ufeid, coreid, phy_id, idle_unassinged_mode) WUFE_OK
#define WUFEI_HwPosPhyCreate(system_registry,coreid,phy_id,ufe_phy_cfg,direction) WUFE_OK
#define WUFEI_HwPosPhyEnable(system_registry,coreid,phy_id,direction) WUFE_OK
#define WUFEI_HwPosPhyDisable(system_registry,coreid,phy_id,direction) WUFE_OK
#define WUFEI_HwMachineSetup(id,coreid) WUFE_OK

#define WUFEI_HwPosCrcReconfig(id,coreid,phy_id,direction,crc_size) WUFE_OK
#define WUFEI_HwPosScramblingReconfig(ufeid, coreid, phy_id, direction, scrambler_enable) WUFE_OK
#define WUFEI_HwPosMinFlagsReconfig(id, coreid, phy_id, min_flags) WUFE_OK


#endif /* WUFEI_HW_MACHINE_INT_H */
