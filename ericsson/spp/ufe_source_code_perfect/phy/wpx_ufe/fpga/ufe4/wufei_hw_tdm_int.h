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

WUFE_status WUFEI_HwTdmEnable(WUFEI_system_registry* system_registry, WP_U32 coreid, WP_U32 direction);
WUFE_status WUFEI_HwTdmDisable(WUFEI_system_registry* system_registry, WP_U32 coreid,WP_U32 direction);
WUFE_status WUFEI_HwTdmIdleCasSet(WUFEI_system_registry* system_registry, WP_U8 idle_data, WP_U8 idle_cas, WP_U32 coreid);
WUFE_status WUFEI_HwTdmIndirectSetup(WUFEI_system_registry* system_registry, WP_U32 coreid);
WUFE_status WUFEI_HwTdmLineConfig(WUFEI_system_registry* system_registry, 
                                  WUFEI_line_registry *line_registry,
                                  WP_U32 tdm_entry);
WUFE_status WUFEI_HwTdmLineDelete(WUFEI_system_registry* system_registry, 
                                  WP_U32 coreid, 
                                  WP_U32 tdm_entry);
WUFE_status WUFEI_HwTdmSlotsSet(WP_U32 wufe_id,
                                WP_U32 core_id,
                                WP_U32 emphy_port_id,
                                WP_U32 direction,
                                WUFE_SlotGroup *slot_group,
                                WUFEI_line_registry *line_registry);
WUFE_status WUFEI_HwTdmActiveEnableSet(WP_U32 wufe_id,
                                       WP_U32 core_id,
                                       WP_U32 emphy_port_id,
                                       WUFE_SlotGroup *slot_group,
                                       WUFEI_line_registry *line_registry);
WUFE_status WUFEI_HwTdmPdhLoopbackConfig(WP_U32 wufe_id,
                                       	 WP_U32 core_id,
                                       	 WP_U32 loopback_index,
                                       	 WP_U16 tdm_entry,
                                       	 WUFE_line_pdh_loopback_type type);
WUFE_status WUFEI_HwTdmPdhLoopbackSet(WUFEI_system_registry* system_registry,
                                   WP_U8 lpbk_index,
                                   WUFE_line_pdh_loopback_type lpbk_type,
                                   WP_U32 coreid,
                                   WP_U8 enable);
WUFE_status WUFEI_HwTdmPdhLoopbackGetIndex(WUFE_line_pdh_loopback_type type,
										  WUFEI_system_registry* system_registry,
										  WP_U16* loopback_index);
WUFE_status WUFEI_HwTdmSlotsClear(WP_U32 ufeid,
                                  WP_U32 core_id,
                                  WP_U32 emphy_port_id,
                                  WP_U32 direction,
                                  WUFE_SlotGroup *slot_group,
                                  WUFEI_line_registry *line_registry);
WUFE_status WUFEI_HwTdmSlotsDisplay(WP_U32 wufe_id,
                                    WP_U32 core_id,
                                    WP_U32 emphy_port_id,
                                    WP_U32 direction,
                                    WUFE_SlotGroup *slot_group,
                                    WUFEI_line_registry *line_registry);
WUFE_status WUFEI_HwTdmLineDisplay(WP_U32 wufe_id,
                                   WP_U32 core_id,
                                   WP_U16 tdm_entry);
WUFE_status WUFEI_HwTdmIndirectDump(WP_U32 wufe_id,
                                    WP_U32 core_id,
                                    WUFEI_system_registry* system_registry);
WUFE_status WUFEI_HwTdmLoopbackSet(WUFEI_system_registry* system_registry, 
                                   WP_U8 enable, 
                                   WP_U8 framed, 
                                   WP_U8 cas_enable,
                                   WP_U8 e1_t1,
                                   WP_U32 coreid);
void WUFEI_HwTdmDisplay(WP_U32 ufe_id, WP_U32 coreid);
void WUFEI_HwTdmEventRegSet(WP_U32 ufeid, WP_U16 coreid, WP_U16 value);
void WUFEI_HwTdmEventRegGet(WP_U32 ufeid, WP_U16 coreid, WP_U16* value);
void WUFEI_HwTdmEventMaskRegSet(WP_U32 ufeid, WP_U16 coreid, WP_U16 value);
void WUFEI_HwTdmEventMaskRegGet(WP_U32 ufeid, WP_U16 coreid, WP_U16* value);

#endif /* WUFEI_HW_TDM_INT_H */
