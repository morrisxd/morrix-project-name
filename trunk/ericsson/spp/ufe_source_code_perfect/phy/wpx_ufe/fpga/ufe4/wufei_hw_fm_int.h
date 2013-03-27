/*****************************************************************************
 * (C) Copyright 2000-2010, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: wufei_hw_fm_int.h
 *
 * Purpose: The UFE Fifo Manager function declaration
 *
 ****************************************************************************/
#ifndef WUFEI_HW_FM_INT_H
#define WUFEI_HW_FM_INT_H

#ifndef WUFE_TYPES_H
#include "wufe_types.h"
#endif
#ifndef WUFEI_REGISTRY_H
#include "wufei_registry.h"
#endif

WUFE_status WUFEI_HwFmEnable(WUFEI_system_registry* system_registry, WP_U32 coreid, WP_U32 direction);
WUFE_status WUFEI_HwFmDisable(WUFEI_system_registry* system_registry, WP_U32 coreid, WP_U32 direction);
WUFE_status WUFEI_HwFmLoopbackEnable(WUFEI_system_registry* system_registry, WP_U32 coreid);
WUFE_status WUFEI_HwFmLoopbackDisable(WUFEI_system_registry* system_registry, WP_U32 coreid);
WUFE_status WUFEI_HwFmLoopbackReconfigure(WUFEI_system_registry* system_registry, WUFE_fifo_manager *config);
WUFE_status WUFEI_HwFmSetPriorityTables(WUFEI_system_registry* system_registry, WP_U32 coreid);
WUFE_status WUFEI_HwFmIndirectSetup(WUFEI_system_registry* system_registry, WP_U32 coreid);
WP_U16 WUFEI_HwFmAssignRxBankNum(WUFEI_system_registry *system_registry,
                                 WP_U32 coreid,
                                 WP_U32 phy_type,
                                 WP_U32 num_of_slots);
WP_U16 WUFEI_HwFmAssignTxBankNum(WUFEI_system_registry *system_registry,
                                 WP_U32 coreid,
                                 WP_U32 phy_type,
                                 WP_U32 num_of_slots);
WUFE_status WUFEI_HwFmRemoveBankValue(WUFEI_system_registry *system_registry,
                                      WP_U32 coreid,
                                      WP_U32 bank_num,
                                      WP_U32 phy_type, 
                                      WP_U32 num_of_slots);
WUFE_status WUFEI_HwFmPhyCreate(WUFEI_system_registry *system_registry,
                                WP_U32 core_id,
                                WUFEI_phy_registry   *phy_registry,
                                WP_U32 phy_type,
                                WP_U8 framed,
                                WP_U32 direction);
WUFE_status WUFEI_HwFmDeletePhy(WUFEI_system_registry *system_registry,
                                WP_U32 coreid,
                                WUFEI_phy_registry   *phy_registry,
                                WP_U32 phy_type,
                                WP_U8 framed,
                                WP_U32 direction);
void WUFEI_HwFmModeDisplay(WP_U32 ufe_id, WP_U32 coreid);
WUFE_status WUFEI_HwFmPhyDisplay(WP_U32 wufe_id,
                                 WP_U32 coreid,
                                 WP_U32 emphy_port_id);
WUFE_status WUFEI_HwFmIndirectDump(WP_U32 wufe_id,
                                   WP_U32 coreid);
WUFE_status WUFEI_HwFmPhyCheckMemPtr(WP_U32 wufe_id,
                                     WP_U32 coreid,
                                     WP_U32 emphy_port_id,
                                     WP_U32 direction);
WUFE_status WUFEI_HwFmPhyClearPtr(WP_U32 wufe_id,
                                  WP_U32 coreid,
                                  WP_U32 emphy_port_id,
                                  WP_U32 direction);
void WUFEI_HwFmInterruptReadRxDisplay(WP_U32 ufe_id, WP_U32 coreid);
void WUFEI_HwFmInterruptReadTxDisplay(WP_U32 ufe_id, WP_U32 coreid);
void WUFEI_HwFmEventRegSet(WP_U32 ufeid, WP_U16 coreid, WP_U16 value);
void WUFEI_HwFmEventRegGet(WP_U32 ufeid, WP_U16 coreid, WP_U16* value);
void WUFEI_HwFmEventMaskRegSet(WP_U32 ufeid, WP_U16 coreid, WP_U16 value);
void WUFEI_HwFmEventMaskRegGet(WP_U32 ufeid, WP_U16 coreid, WP_U16* value);
void WUFEI_HwFmVfpTxSet(WP_U32 ufeid, WP_U16 coreid, WP_U32 index, WP_U32 line_type);
void WUFEI_HwFmVfpRxReset(WP_U32 ufeid, WP_U16 coreid, WP_U32 vfp_index);
WP_U16  WUFEI_HwFmVfpInterruptInfoGet(WP_U32 ufeid, WP_U32 coreid);
void WUFEI_HwFmVfpTxStartThresholdSet(WP_U32 ufeid, WP_U16 coreid,
                                      WP_U32 vfp_index, WP_U32 line_type, WP_U32 struct_id);

#endif /* WUFEI_HW_FM_INT_H */
