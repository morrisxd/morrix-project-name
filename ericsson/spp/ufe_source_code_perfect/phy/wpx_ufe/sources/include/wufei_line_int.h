/*****************************************************************************
 * (C) Copyright 2000-2006, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: wufei_emphy.h
 *
 * Purpose: the UFE EMPHY macro and structures
 *
 ****************************************************************************/

#ifndef WUFEI_TYPE_LINE_H
#define WUFEI_TYPE_LINE_H

#ifndef WUFE_TYPES_H
#include "wufe_types.h"
#endif

#ifndef WUFEI_HW_TDM_H
#include "wufei_hw_tdm.h"
#endif

#ifndef WUFEI_REGISTRY_H
#include "wufei_registry.h"
#endif

#define WUFEI_LineTransferTypeGet(line_registry) line_registry->transfer_type
#define WUFEI_LineFramedModeGet(line_registry) line_registry->framed
#define WUFEI_LineClockRecIfRxGet(line_registry) line_registry->rx_clock_rec_if_id
#define WUFEI_LineClockRecIfTxGet(line_registry) line_registry->tx_clock_rec_if_id
#define WUFEI_LineClockRecModeGet(line_registry) line_registry->clock_rec_mode

WUFE_status WUFEI_LineSystemSetup(WUFEI_system_registry *system_registry);
WUFE_status WUFEI_LineSystemDelete(WUFEI_system_registry* system_registry);

WUFE_status WUFEI_LineGetTdmHwPtr(WUFEI_hw_tdm_entry **tdm_ram_slot_ptr,
                                  WP_U16               direction,
                                  WUFEI_line_registry *line_registry,
                                  WP_U16               slot_id);

WUFE_status WUFEI_LineRegistryGet(WUFEI_system_registry *system_registry,
                                  WP_U16 line_id,
                                  WUFEI_line_registry **line_registry);

WUFE_status WUFEI_LineSetPhyClockRecTxCreate(WP_U32 line_id,
                                             WUFEI_system_registry* system_registry);

WUFE_status WUFEI_LineSetPhyClockRecRxCreate(WP_U32 line_id,
                                             WUFEI_system_registry* system_registry);

WUFE_status WUFEI_LineSetPhyClockRecTxDelete(WP_U32 line_id,
                                             WUFEI_system_registry* system_registry);

WUFE_status WUFEI_LineSetPhyClockRecRxDelete(WP_U32 line_id,
                                             WUFEI_system_registry* system_registry);

void WUFEI_PhyClockRecRxModeDisable(WUFEI_phy_registry* phy_registry);

#endif /*  WUFEI_TYPE_LINE_H*/
