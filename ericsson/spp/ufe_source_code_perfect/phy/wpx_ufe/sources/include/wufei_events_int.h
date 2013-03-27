/*****************************************************************************
 * (C) Copyright 2000-2006, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: wufei_events_int.h
 *
 * Purpose: the internal UFE event interfaces
 *
 ****************************************************************************/
#ifndef WUFEI_EVENTS_INT_H
#define WUFEI_EVENTS_INT_H

/* SBI events shifts macros */

#define WUFEI_EVENT_SBI_V5_SYNC_LOSS_CORE0_S        0
#define WUFEI_EVENT_SBI_RX_SYNC_LOSS_CORE0_S        1
#define WUFEI_EVENT_SBI_TX_AC1FP_SYNC_LOSS_S        2
#define WUFEI_EVENT_SBI_PARITY_ERROR_CORE0_S        3
#define WUFEI_EVENT_SBI_TRIBUTARY_COLLISION_CORE0_S 4
#define WUFEI_EVENT_SBI_V5_SYNC_LOSS_CORE1_S        5
#define WUFEI_EVENT_SBI_RX_SYNC_LOSS_CORE1_S        6
#define WUFEI_EVENT_SBI_PARITY_ERROR_CORE1_S        7
#define WUFEI_EVENT_SBI_RX_SOC_FIFO_OVERRUN_CORE0_S 8
#define WUFEI_EVENT_SBI_RX_SOC_FIFO_OVERRUN_CORE1_S 9
#define WUFEI_EVENT_SBI_RX_ALM_INDICATOR_CORE0_S    10
#define WUFEI_EVENT_SBI_RX_ALM_INDICATOR_CORE1_S    11
#define WUFEI_EVENT_SBI_RX_BIU_FIFO_OVERRUN_CORE0_S 12
#define WUFEI_EVENT_SBI_RX_BIU_FIFO_OVERRUN_CORE1_S 13
#define WUFEI_EVENT_SBI_TX_BUS_FIFO_OCTET_CORE0_S   14
#define WUFEI_EVENT_SBI_TX_BUS_FIFO_OCTET_CORE1_S   15

/* clock recovery events shift macros */
#define WUFEI_EVENT_CR_CT_COUNTER_OVERFLOW_S         0

void WUFEI_EventEnable(WP_U32 ufeid, WP_U32 coreid, WP_U16 value);
void WUFEI_EventDisable(WP_U32 ufeid, WP_U32 coreid, WP_U16 value);

#endif /* WUFEI_EVENTS_INT_H */
