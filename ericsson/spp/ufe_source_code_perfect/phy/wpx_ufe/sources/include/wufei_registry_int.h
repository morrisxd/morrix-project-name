/*****************************************************************************
 * (C) Copyright 2000-2006, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: wufei_registry.h
 *
 * Purpose: the registry srtucture
 *
 ****************************************************************************/

#ifndef WUFEI_REGISTRY_INT_H
#define WUFEI_REGISTRY_INT_H

#ifndef WUFE_SYSTEM_H
#include "wufe_system.h"
#endif
#ifndef WUFE_PHY_H
#include "wufe_phy.h"
#endif
#ifndef WUFE_WDDI_UTIL
#include "wufe_api.h"
#endif
#ifndef WUFE_LINE_H
#include "wufe_line.h"
#endif
#ifndef WUFEI_HW_CONFIG_H
#include "wufei_hw_config.h"
#endif
#ifndef WUFE_TYPES_H
#include "wufe_types.h"
#endif

typedef struct wufei_system_registry WUFEI_system_registry;
typedef struct wufei_mapping_block WUFEI_mapping_block;
typedef struct wufei_line_registry WUFEI_line_registry;
typedef struct wufei_phy_registry WUFEI_phy_registry;
WP_boolean WUFEI_CheckBoardSupport(WP_U32 ufeid);

WUFE_status WUFEI_UfeCoreHwInterruptEnable(WP_U32 ufeid);
WUFE_status WUFEI_UfeFramerHwInterruptEnable(WP_U32 ufeid);
WUFE_status WUFEI_UfeExtPllHwInterruptEnable(WP_U32 ufeid);
WUFE_status WUFEI_UfeLockLostHwInterruptEnable(WP_U32 ufeid);
WUFE_status WUFEI_UfeSfpHwInterruptEnable(WP_U32 ufeid);
WUFE_status WUFEI_UfeCoreHwInterruptDisable(WP_U32 ufeid);
WUFE_status WUFEI_UfeFramerHwInterruptDisable(WP_U32 ufeid);
WUFE_status WUFEI_UfeExtPllHwInterruptDisable(WP_U32 ufeid);
WUFE_status WUFEI_UfeLockLostHwInterruptDisable(WP_U32 ufeid);
WUFE_status WUFEI_UfeSfpHwInterruptDisable(WP_U32 ufeid);
WUFE_status WUFEI_UfeCoreHwInterruptGet(WP_U32 ufeid, WP_boolean *interrupt_result);
WUFE_status WUFEI_UfeFramerHwInterruptGet(WP_U32 ufeid, WP_boolean *interrupt_result);
WUFE_status WUFEI_UfeExtPllHwInterruptGet(WP_U32 ufeid, WP_boolean *interrupt_result);
WUFE_status WUFEI_UfeLockLostHwInterruptGet(WP_U32 ufeid, WP_boolean *interrupt_result);
WUFE_status WUFEI_UfeSfpHwInterruptGet(WP_U32 ufeid, WP_boolean *interrupt_result);

WUFE_status WUFEI_MappingAllocate(WP_U32 ufeid);

WUFE_status WUFEI_MappingInit(WP_U32 ufeid, WP_U32 attributes);

WP_U32 WUFEI_MappingUfeBaseGet(WP_U32 ufeid);

WP_U32 WUFEI_MappingAttribGet(WP_U32 ufeid);

WUFE_status WUFEI_MappingDelete(WP_U32 ufeid);

void WUFEI_ChipSelectWaitSet(WP_U32 wufe_id, WP_U8 connector_index, WP_U32 attributes);

void WUFEI_ChipSelectWaitRestore(WP_U32 wufe_id);

void WUFEI_UfeRegistryLock(WP_U32 wufe_id);

void WUFEI_UfeRegistryUnLock(WP_U32 wufe_id);

WUFE_status WUFEI_SystemeRegistrySet(WP_U32 wufe_id, WUFEI_system_registry* system_registry);

WUFEI_system_registry* WUFEI_SystemRegistryGet(WP_U32 wufe_id);

WUFE_status WUFEI_SystemRegistryGetAndLock(WP_U32 wufe_id,
                                           WUFEI_system_registry** system_registry);

void WUFEI_RegDbgSet(WP_U32 command, WP_U32 ufe_id);

WP_U8 WUFEI_DbgGet(WP_U32 ufe_id);

WUFE_status WUFEI_SystemRegistryUfeEmphyNumSet(WP_U32 wufe_id);

WUFE_status WUFEI_DeviceTypeSet(WP_U32 ufeid,
                                WUFE_device_type device_type);

WUFE_status WUFEI_DeviceTypeGet(WP_U32 ufeid, 
                                WUFE_device_type *device_type);

WUFE_status WUFEI_UfeSynBuildTimeSet(WP_U32 ufeid, 
                                     WP_U32 syn_build_time);

WUFE_status WUFEI_UfeSynBuildTimeGet(WP_U32 ufeid, 
                                     WP_U32 *syn_build_time);

#endif /*WUFEI_REGISTRY_INT_H*/
