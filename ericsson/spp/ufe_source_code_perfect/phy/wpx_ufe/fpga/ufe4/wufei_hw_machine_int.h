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

WUFE_status WUFEI_HwCesPhyEnable(WUFEI_system_registry* system_registry, WP_U32 coreid, WP_U32 phy_id, WP_U32 direction);
WUFE_status WUFEI_HwCesPhyDisable(WUFEI_system_registry* system_registry, WP_U32 coreid, WP_U32 phy_id, WP_U32 direction);
WUFE_status WUFEI_HwCesConfigureSlots(WP_U32 ufeid,
                                      WP_U32 coreid,
                                      WUFE_SlotGroup *slot_group,
                                      WP_U32 phy_id,
                                      WP_U32 direction);
WUFE_status WUFEI_HwCesPhyCreate(WUFEI_system_registry* system_registry,
                                 WP_U32 coreid,
                                 WP_U32 phy_id,
                                 WUFE_SlotGroup *slot_group,
                                 WP_U16 data_unit_size,
                                 WP_U32 direction);
WUFE_status WUFEI_HwPhyDelete(WUFEI_system_registry* system_registry,
                              WP_U32 coreid,
                              WP_U32 phy_id,
                              WP_U32 direction);
WUFE_status WUFEI_HwCesCasConfig(WUFEI_system_registry* system_registry,
                                 WP_U32 coreid,
                                 WP_U32 line_id);

void WUFEI_HwCesCasDisplay(WUFEI_system_registry* system_registry,
                           WP_U32 coreid,
                           WP_U32 line_id);

WUFE_status WUFEI_HwHdlcPhyEnable(WUFEI_system_registry* system_registry,
                               WP_U32 coreid,
                               WP_U32 phy_id,
                               WP_U32 direction);
WUFE_status WUFEI_HwHdlcPhyDisable(WUFEI_system_registry* system_registry,
                                WP_U32 coreid,
                                WP_U32 phy_id,
                                WP_U32 direction);
WP_U8 WUFEI_HwHdlcPhyUnderrunGet(WUFEI_system_registry* system_registry,
                                WP_U32 coreid,
                                 WP_U32 phy_id);
WUFE_status WUFEI_HwHdlcPhyCreate(WUFEI_system_registry* system_registry,
                                 WP_U32 coreid,
                                 WP_U32 phy_id,
                                 WUFE_phy *ufe_phy_cfg,
                                  WP_U32 direction);

WUFE_status WUFEI_HwHdlcMinFlagsReconfig(WP_U32 id,
                                         WP_U32 coreid,
                                         WP_U32 phy_id,
                                         WP_U32 min_flags);

WUFE_status WUFEI_HwHdlcNoFlagsReconfig(WP_U32 id,
                                        WP_U32 coreid,
                                        WP_U32 phy_id,
                                        WP_U32 no_flags);

WUFE_status WUFEI_HwHdlcCrcReconfig(WP_U32 id,
                                    WP_U32 coreid,
                                    WP_U32 phy_id,
                                    WP_U32 direction,
                                    WP_U32 crc_size);

WUFE_status WUFEI_HwPosMinFlagsReconfig(WP_U32 id,
                                        WP_U32 coreid,
                                        WP_U32 phy_id,
                                        WP_U32 min_flags);

WUFE_status WUFEI_HwPosCrcReconfig(WP_U32 id,
                                   WP_U32 coreid,
                                   WP_U32 phy_id,
                                   WP_U32 direction,
                                   WP_U32 crc_size);

WUFE_status WUFEI_HwPosScramblingReconfig(WP_U32 ufeid,
                                          WP_U32 coreid,
                                          WP_U32 phy_id,
                                          WP_U32 direction,
                                          WP_U32 scrambler_enable);

WUFE_status WUFEI_HwRptDisplay(WUFEI_system_registry* system_registry,
                               WP_U32 coreid,
                               WP_U32 phy_id);
WUFE_status WUFEI_HwTptDisplay(WUFEI_system_registry* system_registry,
                               WP_U32 coreid,
                               WP_U32 phy_id);

WUFE_status WUFEI_HwAtmPhyEnable(WUFEI_system_registry* system_registry,
                              WP_U32 coreid,
                              WP_U32 phy_id,
                              WP_U32 direction);
WUFE_status WUFEI_HwAtmPhyDisable(WUFEI_system_registry* system_registry,
                               WP_U32 coreid,
                               WP_U32 phy_id,
                               WP_U32 direction);
WUFE_status WUFEI_HwAtmPhyStateGet(WUFEI_system_registry* system_registry,
                                   WP_U32 coreid,
                                   WP_U32 phy_id,
                                   WUFE_phy_state* sync_state);
WUFE_status WUFEI_HwAtmPhyCreate(WUFEI_system_registry* system_registry,
                                 WP_U32 coreid,
                                 WP_U32 phy_id,
                                 WUFE_phy *ufe_phy_cfg,
                                 WP_U32 direction);
WUFE_status WUFEI_HwAtmScramblingReconfig(WP_U32 ufeid,
                                         WP_U32 coreid,
                                         WP_U32 phy_id,
                                         WP_U32 direction,
                                         WP_U32 scrambler_enable);
WUFE_status WUFEI_HwAtmCosetReconfig(WP_U32 ufeid,
                                         WP_U32 coreid,
                                         WP_U32 phy_id,
                                         WP_U32 direction,
                                         WP_U32 coset_enable);
WUFE_status WUFEI_HwAtmIdleCellModeReconfig(WP_U32 ufeid,
                                        WP_U32 coreid,
                                        WP_U32 phy_id,
                                        WP_U32 idle_cell_enable);
WUFE_status WUFEI_HwAtmUnassignedCellModeReconfig(WP_U32 ufeid,
                                                  WP_U32 coreid,
                                                  WP_U32 phy_id,
                                                  WP_U32 unassigned_cell_enable);
WUFE_status WUFEI_HwAtmHecErrorDiscardModeReconfig(WP_U32 ufeid,
                                                   WP_U32 coreid,
                                                   WP_U32 phy_id,
                                                   WP_U32 hec_error_discard);
WUFE_status WUFEI_HwAtmHecErrorCorrectionModeReconfig (WP_U32 ufeid,
                                                       WP_U32 coreid,
                                                       WP_U32 phy_id,
                                                       WP_U32 hec_error_correction);
WUFE_status WUFEI_HwAtmCellDelineationModeReconfig (WP_U32 ufeid,
                                                    WP_U32 coreid,
                                                    WP_U32 phy_id,
                                                    WP_U32 cell_delineation);
WUFE_status WUFEI_HwAtmRxAlphaReconfig (WP_U32 ufeid,
                                        WP_U32 coreid,
                                        WP_U32 phy_id,
                                        WP_U32 rx_alpha);
WUFE_status WUFEI_HwAtmRxDeltaReconfig (WP_U32 ufeid,
                                        WP_U32 coreid,
                                        WP_U32 phy_id,
                                        WP_U32 rx_delta);
WUFE_status WUFEI_HwAtmIdleUnassignedCellModeReconfig (WP_U32 ufeid,
                                                       WP_U32 coreid,
                                                       WP_U32 phy_id,
                                                       WP_U32 idle_unassinged_mode);
WP_U8 WUFEI_HwCesPhyUnderrunGet(WUFEI_system_registry* system_registry,
                                WP_U32 coreid,
                                WP_U32 phy_id);
WP_U8 WUFEI_HwAtmPhyUnderrunGet(WUFEI_system_registry* system_registry,
                                WP_U32 coreid,
                                WP_U32 phy_id);

void WUFEI_HwMachineEventRegSet(WP_U32 ufeid, WP_U16 coreid, WP_U16 value, WP_U16 direction);
void WUFEI_HwMachineEventRegGet(WP_U32 ufeid, WP_U16 coreid, WP_U16* value, WP_U16 direction);
void WUFEI_HwMachineEventMaskRegSet(WP_U32 ufeid, WP_U16 coreid, WP_U16 value, WP_U16 direction);
void WUFEI_HwMachineEventMaskRegGet(WP_U32 ufeid, WP_U16 coreid, WP_U16* value, WP_U16 direction);

void WUFEI_HwMachineRxPortIdGet(WP_U32 ufeid, WP_U16 coreid, WP_U16* value);
void WUFEI_HwMachineTxPortIdGet(WP_U32 ufeid, WP_U16 coreid, WP_U16* value);

WUFE_status WUFEI_HwPosPhyCreate(WUFEI_system_registry* system_registry,
                                 WP_U32 coreid,
                                 WP_U32 phy_id,
                                 WUFE_phy *ufe_phy_cfg,
                                 WP_U32 direction);
WUFE_status WUFEI_HwPosPhyEnable(WUFEI_system_registry* system_registry,
                                 WP_U32 coreid,
                                 WP_U32 phy_id,
                                 WP_U32 direction);
WUFE_status WUFEI_HwPosPhyDisable(WUFEI_system_registry* system_registry,
                                  WP_U32 coreid,
                                  WP_U32 phy_id,
                                  WP_U32 direction);
WUFE_status WUFEI_HwMachineSetup(WP_U32 ufeid, WP_U16 coreid);

#endif /* WUFEI_HW_MACHINE_INT_H */
