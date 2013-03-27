/*****************************************************************************
 * (C) Copyright 2000-2006, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: wufei_hw_events.h
 *
 * Purpose: the UFE Interrupt  macro and structures
 *
 *
 ****************************************************************************/
#ifndef WUFEI_HW_EVENTS_H
#define WUFEI_HW_EVENTS_H

typedef struct
{
   WP_U16 event_reg;
   WP_U16 event_mask_reg;
   WP_U16 event_phy_underrun_tx;
   WP_U16 reserved[4];
   WP_U16 event_phy_underrun_rx;
} WUFEI_hw_events;

#endif /* WUFEI_HW_EVENTS_H */
