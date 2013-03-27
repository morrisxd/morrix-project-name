/*****************************************************************************
 * (C) Copyright 2000-2006, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: wufei_hw_emphy_int.h
 *
 * Purpose: The UFE EMPHY function declaration
 *
 ****************************************************************************/
#ifndef WUFEI_HW_EMPHY_INT_H
#define WUFEI_HW_EMPHY_INT_H

#ifndef WUFE_TYPES_H
#include "wufe_types.h"
#endif
#ifndef WUFEI_REGISTRY_H
#include "wufei_registry.h"
#endif
WP_U32 WUFEI_UfeEmphyRegBaseAddrGet(WP_U32 ufeid);

WUFE_status WUFEI_HwEmphySetup(WUFEI_system_registry* system_registry, WP_U32 coreid);
WUFE_status WUFEI_HwEmphyEnable(WUFEI_system_registry* system_registry,
                                WP_U32 coreid,
                                WP_U32 direction);
WUFE_status WUFEI_HwEmphyDisable(WUFEI_system_registry* system_registry,
                                 WP_U32 coreid,
                                 WP_U32 direction);

void WUFEI_HwEmphyEventRegSet(WP_U32 ufeid, WP_U32 coreid, WP_U16 value);
void WUFEI_HwEmphyEventRegGet(WP_U32 ufeid, WP_U32 coreid, WP_U16* value);
void WUFEI_HwEmphyEventMaskRegSet(WP_U32 ufeid, WP_U32 coreid, WP_U16 value);
void WUFEI_HwEmphyEventMaskRegGet(WP_U32 ufeid, WP_U32 coreid, WP_U16* value);
void WUFEI_HwEmphyPhyNotValidGet(WP_U32 ufeid, WP_U32 coreid, WP_U16* value);
void WUFEI_HwEmphyDisplay(WP_U32 ufe_id, WP_U32 coreid);

#endif /* WUFEI_HW_EMPHY_INT_H */
