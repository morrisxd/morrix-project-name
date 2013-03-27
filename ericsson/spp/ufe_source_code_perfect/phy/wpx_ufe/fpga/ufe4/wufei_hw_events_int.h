/*****************************************************************************
 * (C) Copyright 2000-2006, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File:  wufei_hw_events_int.h
 *
 * Purpose:  the internal UFE event interfaces
 *
 ****************************************************************************/

#ifndef WUFEI_HW_EVENTS_INT_H
#define WUFEI_HW_EVENTS_INT_H

#ifndef WUFE_EVENTS_H
#include "wufe_events.h"
#endif

#define WUFEI_HW_EVENT_SBI 0

void WUFEI_HwEventDispatch(WP_U32 ufeid,
                           WUFEI_system_registry* system_registry,
                           WUFE_events *events);
WUFE_status WUFEI_HwEventEnable(WP_U32 ufeid, WUFE_events *events);
WUFE_status WUFEI_HwEventDisable(WP_U32 ufeid, WUFE_events *events);

WUFE_status WUFEI_HwEventSetup(WP_U32 ufeid);

#define WUFEI_HwEventSbiEnable(ufeid, sbi_events) {ufeid =0;}
#define WUFEI_HwEventSbiDisable(ufeid, sbi_events) {ufeid =0;}

#endif /*  WUFEI_EVENTS_H*/
