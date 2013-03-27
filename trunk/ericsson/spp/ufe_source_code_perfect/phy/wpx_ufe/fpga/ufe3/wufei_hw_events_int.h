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

/* SBI events */

#define WUFEI_HW_EVENT_SBI_V5_SYNC_LOSS_CORE0            0x0001
#define WUFEI_HW_EVENT_SBI_V5_SYNC_LOSS_CORE1            0x0002
#define WUFEI_HW_EVENT_SBI_RX_SYNC_LOSS_CORE0            0x0004
#define WUFEI_HW_EVENT_SBI_RX_SYNC_LOSS_CORE1            0x0008
#define WUFEI_HW_EVENT_SBI_PARITY_ERROR_CORE0            0x0010
#define WUFEI_HW_EVENT_SBI_PARITY_ERROR_CORE1            0x0020
#define WUFEI_HW_EVENT_SBI_RX_SOC_FIFO_OVERRUN_CORE0     0x0040
#define WUFEI_HW_EVENT_SBI_RX_SOC_FIFO_OVERRUN_CORE1     0x0080
#define WUFEI_HW_EVENT_SBI_RX_ALM_INDICATOR_CORE0        0x0100
#define WUFEI_HW_EVENT_SBI_RX_ALM_INDICATOR_CORE1        0x0200
#define WUFEI_HW_EVENT_SBI_RX_BIU_FIFO_OVERRUN_CORE0     0x0400
#define WUFEI_HW_EVENT_SBI_RX_BIU_FIFO_OVERRUN_CORE1     0x0800
#define WUFEI_HW_EVENT_SBI_TX_BUS_FIFO_OCTET_CORE0       0x1000
#define WUFEI_HW_EVENT_SBI_TX_BUS_FIFO_OCTET_CORE1       0x2000
#define WUFEI_HW_EVENT_SBI_TX_AC1FP_SYNC_LOSS            0x4000

#define WUFEI_HW_EVENT_SBI_V5_SYNC_LOSS_CORE0_S          0
#define WUFEI_HW_EVENT_SBI_V5_SYNC_LOSS_CORE1_S          1
#define WUFEI_HW_EVENT_SBI_RX_SYNC_LOSS_CORE0_S          2
#define WUFEI_HW_EVENT_SBI_RX_SYNC_LOSS_CORE1_S          3
#define WUFEI_HW_EVENT_SBI_PARITY_ERROR_CORE0_S          4
#define WUFEI_HW_EVENT_SBI_PARITY_ERROR_CORE1_S          5
#define WUFEI_HW_EVENT_SBI_RX_SOC_FIFO_OVERRUN_CORE0_S   6
#define WUFEI_HW_EVENT_SBI_RX_SOC_FIFO_OVERRUN_CORE1_S   7
#define WUFEI_HW_EVENT_SBI_RX_ALM_INDICATOR_CORE0_S      8
#define WUFEI_HW_EVENT_SBI_RX_ALM_INDICATOR_CORE1_S      9
#define WUFEI_HW_EVENT_SBI_RX_BIU_FIFO_OVERRUN_CORE0_S   10
#define WUFEI_HW_EVENT_SBI_RX_BIU_FIFO_OVERRUN_CORE1_S   11
#define WUFEI_HW_EVENT_SBI_TX_BUS_FIFO_OCTET_CORE0_S     12
#define WUFEI_HW_EVENT_SBI_TX_BUS_FIFO_OCTET_CORE1_S     13
#define WUFEI_HW_EVENT_SBI_TX_AC1FP_SYNC_LOSS_S          14

#define WUFEI_HW_EVENT_SBI (WUFEI_HW_EVENT_SBI_V5_SYNC_LOSS_CORE0        |\
                            WUFEI_HW_EVENT_SBI_V5_SYNC_LOSS_CORE1        |\
                            WUFEI_HW_EVENT_SBI_RX_SYNC_LOSS_CORE0        |\
                            WUFEI_HW_EVENT_SBI_RX_SYNC_LOSS_CORE1        |\
                            WUFEI_HW_EVENT_SBI_PARITY_ERROR_CORE0        |\
                            WUFEI_HW_EVENT_SBI_PARITY_ERROR_CORE1        |\
                            WUFEI_HW_EVENT_SBI_RX_SOC_FIFO_OVERRUN_CORE0 |\
                            WUFEI_HW_EVENT_SBI_RX_SOC_FIFO_OVERRUN_CORE1 |\
                            WUFEI_HW_EVENT_SBI_RX_ALM_INDICATOR_CORE0    |\
                            WUFEI_HW_EVENT_SBI_RX_ALM_INDICATOR_CORE1    |\
                            WUFEI_HW_EVENT_SBI_RX_BIU_FIFO_OVERRUN_CORE0 |\
                            WUFEI_HW_EVENT_SBI_RX_BIU_FIFO_OVERRUN_CORE1 |\
                            WUFEI_HW_EVENT_SBI_TX_BUS_FIFO_OCTET_CORE0   |\
                            WUFEI_HW_EVENT_SBI_TX_BUS_FIFO_OCTET_CORE1   |\
                            WUFEI_HW_EVENT_SBI_TX_AC1FP_SYNC_LOSS)

/* CR events */

#define WUFEI_HW_EVENT_CT_COUNTER_OVERFLOW     0x0001

#define WUFEI_HW_EVENT_CT_COUNTER_OVERFLOW_S   0

#define WUFEI_HW_EVENT_CR WUFEI_HW_EVENT_CT_COUNTER_OVERFLOW

void WUFEI_HwEventDispatch(WP_U32 ufeid,
                           WUFEI_system_registry* system_registry,
                           WUFE_events *events);
WUFE_status WUFEI_HwEventEnable(WP_U32 ufeid, WUFE_events *events);
WUFE_status WUFEI_HwEventDisable(WP_U32 ufeid, WUFE_events *events);

WUFE_status WUFEI_HwEventSetup(WP_U32 ufeid);

void WUFEI_HwEventSbiEnable(WP_U32 ufeid, WP_U16 sbi_events);
void WUFEI_HwEventSbiDisable(WP_U32 ufeid, WP_U16 sbi_events);

#endif /*  WUFEI_EVENTS_H*/
