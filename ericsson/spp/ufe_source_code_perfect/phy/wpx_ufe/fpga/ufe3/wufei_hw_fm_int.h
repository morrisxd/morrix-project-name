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

#define WUFEI_HwFmEnable(system_registry, coreid, direction) WUFE_OK
#define WUFEI_HwFmDisable(system_registry, coreid, direction) WUFE_OK
#define WUFEI_HwFmLoopbackEnable(system_registry, coreid) WUFE_OK
#define WUFEI_HwFmLoopbackDisable(system_registry, coreid) WUFE_OK
#define WUFEI_HwFmLoopbackReconfigure( system_registry, config) WUFE_OK
#define WUFEI_HwFmSetPriorityTables( system_registry,  coreid) WUFE_OK
#define WUFEI_HwFmIndirectSetup( system_registry,  coreid) WUFE_OK
#define WUFEI_HwFmAssignRxBankNum(system_registry,coreid,phy_type,num_of_slots) WUFE_OK
#define WUFEI_HwFmAssignTxBankNum(system_registry,coreid,phy_type,num_of_slots) WUFE_OK
#define WUFEI_HwFmRemoveBankValue(system_registry,coreid,bank_num,phy_type, num_of_slots) WUFE_OK
#define WUFEI_HwFmPhyCreate(system_registry,core_id,phy_registry,phy_type,framed,direction) WUFE_OK
#define WUFEI_HwFmDeletePhy(system_registry,coreid,phy_registry,phy_type,framed,direction) WUFE_OK
#define WUFEI_HwFmModeDisplay( ufe_id,  coreid) {ufeid =0;}
#define WUFEI_HwFmPhyDisplay( wufe_id,core_id,emphy_port_id) WUFE_OK
#define WUFEI_HwFmIndirectDump( wufe_id,core_id) WUFE_OK
#define WUFEI_HwFmInterruptReadRxDisplay( ufe_id) WUFE_OK
#define WUFEI_HwFmInterruptReadTxDisplay( ufe_id) WUFE_OK
#define WUFEI_HwFmEventRegSet( ufeid,  coreid,  value) WUFE_OK
#define WUFEI_HwFmEventRegGet( ufeid,  coreid,  value) WUFE_OK
#define WUFEI_HwFmEventMaskRegSet( ufeid,  coreid,  value) WUFE_OK
#define WUFEI_HwFmEventMaskRegGet( ufeid,  coreid,  value) WUFE_OK
#define WUFEI_HwFmPhyCheckMemPtr(wufe_id,coreid,emphy_port_id,direction) WUFE_OK
#define WUFEI_HwFmPhyClearPtr(wufe_id,coreid,emphy_port_id,direction) WUFE_OK
#define WUFEI_HwFmVfpTxSet(ufeid, coreid, index, line_type)
#define WUFEI_HwFmVfpRxReset(ufeid, coreid, vfp_index)
#define WUFEI_HwFmVfpTxStartThresholdSet(ufeid, coreid, vfp_index, line_type, struct_id)

#endif /* WUFEI_HW_FM_INT_H */
