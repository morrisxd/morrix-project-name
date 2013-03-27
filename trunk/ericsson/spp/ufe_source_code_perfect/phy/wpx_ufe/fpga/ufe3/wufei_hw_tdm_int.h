/*****************************************************************************
 * (C) Copyright 2000-2010, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: wufei_hw_tdm_int.h
 *
 * Purpose: The UFE TDM function declaration
 *
 ****************************************************************************/
#ifndef WUFEI_HW_TDM_INT_H
#define WUFEI_HW_TDM_INT_H

#ifndef WUFE_TYPES_H
#include "wufe_types.h"
#endif
#ifndef WUFEI_REGISTRY_H
#include "wufei_registry.h"
#endif

#define WUFEI_HwTdmEnable(system_registry, coreid, direction) WUFE_OK
#define WUFEI_HwTdmDisable(system_registry, coreid,direction) WUFE_OK
#define WUFEI_HwTdmIdleCasSet(system_registry, idle_data, idle_cas, coreid) WUFE_OK
#define WUFEI_HwTdmIndirectSetup(system_registry, coreid) WUFE_OK
#define WUFEI_HwTdmLineConfig(system_registry,line_registry,tdm_entry) WUFE_OK
#define WUFEI_HwTdmLineDelete( system_registry, coreid, tdm_entry) WUFE_OK
#define WUFEI_HwTdmSlotsSet( wufe_id,core_id,emphy_port_id,direction,slot_group,line_registry) WUFE_OK
#define WUFEI_HwTdmActiveEnableSet(wufe_id, core_id, emphy_port_id, slot_group, line_registry) WUFE_OK
#define WUFEI_HwTdmSlotsClear( ufeid,core_id,emphy_port_id,direction,slot_group,line_registry) WUFE_OK
#define WUFEI_HwTdmSlotsDisplay( wufe_id,core_id,emphy_port_id,direction,slot_group,line_registry) WUFE_OK
#define WUFEI_HwTdmLineDisplay( wufe_id,core_id,tdm_entry) WUFE_OK
#define WUFEI_HwTdmIndirectDump( wufe_id,core_id) WUFE_OK
#define WUFEI_HwTdmDisplay( ufe_id,  coreid)
#define WUFEI_HwTdmEventRegSet( ufeid,  coreid,  value) WUFE_OK
#define WUFEI_HwTdmEventRegGet( ufeid,  coreid,  value) WUFE_OK
#define WUFEI_HwTdmEventMaskRegSet( ufeid,  coreid,  value) WUFE_OK
#define WUFEI_HwTdmEventMaskRegGet( ufeid,  coreid,  value) WUFE_OK
#define WUFEI_HwTdmLoopbackSet(system_registry, enable, framed, cas_enable, e1_t1, coreid) WUFE_OK
#define WUFEI_HwTdmPdhLoopbackConfig(wufe_id,core_id,loopback_index,tdm_entry,type) WUFE_OK
#define WUFEI_HwTdmPdhLoopbackSet(system_registry,lpbk_index,lpbk_type,coreid,enable) WUFE_OK
#define WUFEI_HwTdmPdhLoopbackGetIndex(type,system_registry,loopback_index) WUFE_OK
#endif /* WUFEI_HW_TDM_INT_H */
