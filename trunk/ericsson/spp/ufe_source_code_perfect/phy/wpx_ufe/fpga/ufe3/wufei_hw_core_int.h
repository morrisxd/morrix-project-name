/*****************************************************************************
 * (C) Copyright 2000-2006, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: wufei_hw_core_int.h
 *
 * Purpose: the UFE Glue logic function declaration
 *
 ****************************************************************************/
#ifndef WUFEI_HW_CORE_INT_H
#define WUFEI_HW_CORE_INT_H

#ifndef WUFE_TYPES_H
#include "wufe_types.h"
#endif
#ifndef WUFEI_REGISTRY_H
#include "wufei_registry.h"
#endif

WP_U32 WUFEI_UfeCoreBaseAddrGet(WP_U32 ufeid, WP_U32 coreid);

WP_U32 WUFEI_UfeLiuBaseAddrGet(WP_U32 ufeid);

WP_U32 WUFEI_UfeBsBaseAddrGet(WP_U32 ufeid);

WP_U32 WUFEI_UfeClkRecBaseAddrGet(WP_U32 ufeid);

WP_U32 WUFEI_UfeCoreHwModelGet(WP_U32 ufeid);

WP_U16 WUFEI_UfeCoreBuildNumGet(WP_U32 ufeid);

WUFE_status WUFEI_UfeCoreHwConfigSetup(WUFEI_system_hw_config *hw_config, WP_U32 wufe_id, WP_U16 dummy);
WUFE_status WUFEI_UfeCoreHwConfigDelete(WUFEI_system_hw_config *hw_config);

void WUFEI_UfeCoreHwConfigDisplay(WUFEI_system_hw_config *hw_config, WP_U32 ufeid);

WP_U16 WUFEI_UfeCoreGlobalIntGet(WP_U32 ufeid);
WP_U16 WUFEI_UfeCoreGlobalIntMaskGet(WP_U32 ufeid);
void WUFEI_UfeCoreGlobalIntSet(WP_U32 ufeid, WP_U16 value);

WUFE_status WUFEI_UfeCoreGlobalIntEnable(WP_U32 ufeid, WP_U32 coreid);
void WUFEI_UfeCoreGlobalIntMaskSet(WP_U32 ufeid, WP_U16 value);

WUFE_status WUFEI_UfeCoreGlobalIntDisable(WP_U32 ufeid, WP_U32 coreid);

WP_U32 WUFEI_UfeCoreLiuModeGet(WP_U32 ufeid);
WP_U32 WUFEI_UfeCoreBandwidthGet(WP_U32 ufeid);

WUFE_status WUFEI_UfeCorePhyIdBlockCreate(WUFEI_system_registry* system_registry);
WUFE_status WUFEI_UfeCorePhyIdBlockDelete(WUFEI_system_registry* system_registry);
WUFE_status WUFEI_UfeCorePhyIdBlockSet(WUFEI_system_registry* system_registry,
                                       WP_U32 coreid, WP_U16 phy_id, WP_U16* emphy_port_id);
WUFE_status WUFEI_UfeCorePhyIdBlockGet(WUFEI_system_registry* system_registry,
                                       WP_U32 coreid, WP_U16* phy_id, WP_U16 emphy_port_id);
WUFE_status WUFEI_UfeCorePhyIdBlockPut(WUFEI_system_registry* system_registry,
                                       WP_U32 coreid, WP_U32 emphy_port_id);

WP_U32 WUFEI_UfeCoreMaxPhyGet(WUFEI_system_registry* system_registry);

WUFE_status WUFEI_UfeCoreRegistrySetup(WUFEI_system_registry* system_registry);

#endif /* WUFEI_HW_CORE_INT_H*/
