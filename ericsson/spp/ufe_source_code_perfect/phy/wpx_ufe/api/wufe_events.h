/*****************************************************************************
 * (C) Copyright 2000-2006, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: wufe_events.h
 *
 * Purpose: External interface for UFE2 events
 *
 ****************************************************************************/
#ifndef WUFE_EVENTS_H
#define WUFE_EVENTS_H

#ifndef WUFE_TYPES_H
#include "wufe_types.h"
#endif

/* UFE core events */
#define WUFE_EVENT_CORE_INGRESS_FBP_OV           0x0001
#define WUFE_EVENT_CORE_EGRESS_FBP_OV            0x0100
#define WUFE_EVENT_CORE_EGRESS_PHY_UNDERRUN      0x4000

#ifdef WUFE_UFE3
/* EMPHY events */
#define WUFE_EVENT_EMPHY_INGRESS_UN              0x0002
#define WUFE_EVENT_EMPHY_INGRESS_BUS             0x0004
#define WUFE_EVENT_EMPHY_INGRESS_IF              0x0008
#define WUFE_EVENT_EMPHY_EGRESS_PHY_NOT_VALID    0x0040
#define WUFE_EVENT_EMPHY_EGRESS_OV               0x0200
#define WUFE_EVENT_EMPHY_EGRESS_BUS              0x0400
#define WUFE_EVENT_EMPHY_EGRESS_IF               0x0800
#define WUFE_EVENT_EMPHY_EGRESS_PARITY           0x1000
#endif

/* CAD events */
#define WUFE_EVENT_CAD_RX_OVERRUN                0x0001
#define WUFE_EVENT_CAD_RX_SYNC_LOSS              0x0002
#define WUFE_EVENT_CAD_TX_UNDERRUN               0x0001
#define WUFE_EVENT_CAD_TX_SYNC_LOSS              0x0002

#define WUFE_EVENT_CAD (WUFE_EVENT_CAD_RX_OVERRUN |\
                        WUFE_EVENT_CAD_RX_SYNC_LOSS |\
                        WUFE_EVENT_CAD_TX_UNDERRUN |\
                        WUFE_EVENT_CAD_TX_SYNC_LOSS)

/* SBI events */

#define WUFE_EVENT_SBI_V5_SYNC_LOSS_CORE0        0x0001 /* UFE2,UFE2+,UFE3 */
#define WUFE_EVENT_SBI_RX_SYNC_LOSS_CORE0        0x0002 /* UFE2,UFE2+,UFE3 */
#define WUFE_EVENT_SBI_TX_AC1FP_SYNC_LOSS        0x0004 /* UFE2,UFE2+,UFE3 */
#define WUFE_EVENT_SBI_PARITY_ERROR_CORE0        0x0008 /* UFE2,UFE2+,UFE3 */
#define WUFE_EVENT_SBI_TRIBUTARY_COLLISION_CORE0 0x0010 /* UFE2 only */
#define WUFE_EVENT_SBI_V5_SYNC_LOSS_CORE1        0x0020 /* UFE3 */
#define WUFE_EVENT_SBI_RX_SYNC_LOSS_CORE1        0x0040 /* UFE3 */
#define WUFE_EVENT_SBI_PARITY_ERROR_CORE1        0x0080 /* UFE3 */
#define WUFE_EVENT_SBI_RX_SOC_FIFO_OVERRUN_CORE0 0x0100 /* UFE3 */
#define WUFE_EVENT_SBI_RX_SOC_FIFO_OVERRUN_CORE1 0x0200 /* UFE3 */
#define WUFE_EVENT_SBI_RX_ALM_INDICATOR_CORE0    0x0400 /* UFE3 */
#define WUFE_EVENT_SBI_RX_ALM_INDICATOR_CORE1    0x0800 /* UFE3 */
#define WUFE_EVENT_SBI_RX_BIU_FIFO_OVERRUN_CORE0 0x1000 /* UFE3 */
#define WUFE_EVENT_SBI_RX_BIU_FIFO_OVERRUN_CORE1 0x2000 /* UFE3 */
#define WUFE_EVENT_SBI_TX_BUS_FIFO_OCTET_CORE0   0x4000 /* UFE3 */
#define WUFE_EVENT_SBI_TX_BUS_FIFO_OCTET_CORE1   0x8000 /* UFE3 */

/* clock recovery events */
#define WUFE_EVENT_CR_CT_COUNTER_OVERFLOW        0x0001 /* UFE2,UFE2+, UFE3 */

#ifdef WUFE_UFE2 /*ufe2 events only*/
/* LIU events */
#define WUFE_EVENT_LIU_INGRESS                   0x0080
#define WUFE_EVENT_LIU_EGRESS                    0x8000

#define WUFE_EVENT_CORE_INGRESS_PHY_CR_UNDERRUN  0x0010
#define WUFE_EVENT_SPECIAL_CORE WUFE_EVENT_CORE_INGRESS_PHY_CR_UNDERRUN

#define WUFE_EVENT_EMPHY_EGRESS_PHY_STUCK        0x0020
#define WUFE_EVENT_SPECIAL_EMPHY WUFE_EVENT_EMPHY_EGRESS_PHY_STUCK

#define WUFE_EVENT_SBI_REG (WUFE_EVENT_SBI_V5_SYNC_LOSS_CORE0 |\
                            WUFE_EVENT_SBI_RX_SYNC_LOSS_CORE0 |\
                            WUFE_EVENT_SBI_TX_AC1FP_SYNC_LOSS |\
                            WUFE_EVENT_SBI_PARITY_ERROR_CORE0 |\
                            WUFE_EVENT_SBI_TRIBUTARY_COLLISION_CORE0)
#endif

#ifdef WUFE_UFE2PLUS /*ufe2+ events only*/
/* LIU events */
#define WUFE_EVENT_LIU_INGRESS                   0x0080
#define WUFE_EVENT_LIU_EGRESS                    0x8000

#define WUFE_EVENT_CORE_ATM_SYNC_INDICATOR       0x0010
#define WUFE_EVENT_SPECIAL_CORE WUFE_EVENT_CORE_ATM_SYNC_INDICATOR

#define WUFE_EVENT_EMPHY_EGRESS_PHY_STUCK        0x0020
#define WUFE_EVENT_SPECIAL_EMPHY WUFE_EVENT_EMPHY_EGRESS_PHY_STUCK

#define WUFE_EVENT_SBI_REG                       0x0000
#endif

#ifdef WUFE_UFE3 /*ufe3 events only*/
/* LIU events */
#define WUFE_EVENT_LIU_INGRESS                   0x0000
#define WUFE_EVENT_LIU_EGRESS                    0x0000

#define WUFE_EVENT_CORE_ATM_SYNC_INDICATOR       0x0004
#define WUFE_EVENT_CORE_INGRESS_PHY_CR_UNDERRUN  0x0002
#define WUFE_EVENT_SPECIAL_CORE (WUFE_EVENT_CORE_ATM_SYNC_INDICATOR |\
                                 WUFE_EVENT_CORE_INGRESS_PHY_CR_UNDERRUN )

#define WUFE_EVENT_SBI_REG (WUFE_EVENT_SBI_V5_SYNC_LOSS_CORE0           | \
                            WUFE_EVENT_SBI_RX_SYNC_LOSS_CORE0           | \
                            WUFE_EVENT_SBI_TX_AC1FP_SYNC_LOSS           | \
                            WUFE_EVENT_SBI_PARITY_ERROR_CORE0           | \
                            WUFE_EVENT_SBI_V5_SYNC_LOSS_CORE1           | \
                            WUFE_EVENT_SBI_RX_SYNC_LOSS_CORE1           | \
                            WUFE_EVENT_SBI_PARITY_ERROR_CORE1           | \
                            WUFE_EVENT_SBI_RX_SOC_FIFO_OVERRUN_CORE0    | \
                            WUFE_EVENT_SBI_RX_SOC_FIFO_OVERRUN_CORE1    | \
                            WUFE_EVENT_SBI_RX_ALM_INDICATOR_CORE0       | \
                            WUFE_EVENT_SBI_RX_ALM_INDICATOR_CORE1       | \
                            WUFE_EVENT_SBI_RX_BIU_RX_FIFO_OVERRUN_CORE0 | \
                            WUFE_EVENT_SBI_RX_BIU_RX_FIFO_OVERRUN_CORE1 | \
                            WUFE_EVENT_SBI_TX_BUS_FIFO_OCTET_CORE0      | \
                            WUFE_EVENT_SBI_TX_BUS_FIFO_OCTET_CORE1)
#endif

#ifndef WUFE_EVENT_SPECIAL_CORE
#define WUFE_EVENT_SPECIAL_CORE 0x0000
#endif
#ifndef WUFE_EVENT_SPECIAL_EMPHY
#define WUFE_EVENT_SPECIAL_EMPHY 0x0000
#endif

#define WUFE_EVENT_CORE  (WUFE_EVENT_CORE_INGRESS_FBP_OV | \
                          WUFE_EVENT_CORE_EGRESS_FBP_OV | \
                          WUFE_EVENT_SPECIAL_CORE |\
                          WUFE_EVENT_CORE_EGRESS_PHY_UNDERRUN)
#ifdef WUFE_UFE3
#define WUFE_EVENT_EMPHY (WUFE_EVENT_EMPHY_INGRESS_UN | \
                          WUFE_EVENT_EMPHY_INGRESS_BUS | \
                          WUFE_EVENT_EMPHY_INGRESS_IF | \
                          WUFE_EVENT_SPECIAL_EMPHY | \
                          WUFE_EVENT_EMPHY_EGRESS_PHY_NOT_VALID | \
                          WUFE_EVENT_EMPHY_EGRESS_OV | \
                          WUFE_EVENT_EMPHY_EGRESS_BUS | \
                          WUFE_EVENT_EMPHY_EGRESS_IF | \
                          WUFE_EVENT_EMPHY_EGRESS_PARITY)

#define WUFE_EVENT_MACHINE_RX 0
#define WUFE_EVENT_MACHINE_TX 0
#endif
#define WUFE_EVENT_LIU   (WUFE_EVENT_LIU_INGRESS |\
                          WUFE_EVENT_LIU_EGRESS)

#ifdef WUFE_UFE4 /*ufe4 events only*/
#define WUFE_EVENT_LIU_INGRESS                   0x0000
#define WUFE_EVENT_LIU_EGRESS                    0x0000

/* Board Support events */
#define WUFE_EVENT_BS_ABSENT_SFP_0               0x0001
#define WUFE_EVENT_BS_ABSENT_SFP_1               0x0002
#define WUFE_EVENT_BS_ABSENT_SFP_2               0x0004
#define WUFE_EVENT_BS_ABSENT_SFP_3               0x0008
#define WUFE_EVENT_BS_ABSENT_SFP_4               0x0010
#define WUFE_EVENT_BS_ABSENT_SFP_5               0x0020
#define WUFE_EVENT_BS_ABSENT_SFP_6               0x0040
#define WUFE_EVENT_BS_ABSENT_SFP_7               0x0080
#define WUFE_EVENT_BS_ABSENT_SFP_8               0x0100
#define WUFE_EVENT_BS_ABSENT_SFP_9               0x0200
#define WUFE_EVENT_BS_ABSENT_SFP_10              0x0400
#define WUFE_EVENT_BS_ABSENT_SFP_11              0x0800
#define WUFE_EVENT_BS_ABSENT_SFP_12              0x1000
#define WUFE_EVENT_BS_ABSENT_SFP_13              0x2000
#define WUFE_EVENT_BS_ABSENT_SFP_14              0x4000
#define WUFE_EVENT_BS_ABSENT_SFP_15              0x8000

#define WUFE_EVENT_BS_LOS_SFP_0                  0x0001
#define WUFE_EVENT_BS_LOS_SFP_1                  0x0002
#define WUFE_EVENT_BS_LOS_SFP_2                  0x0004
#define WUFE_EVENT_BS_LOS_SFP_3                  0x0008
#define WUFE_EVENT_BS_LOS_SFP_4                  0x0010
#define WUFE_EVENT_BS_LOS_SFP_5                  0x0020
#define WUFE_EVENT_BS_LOS_SFP_6                  0x0040
#define WUFE_EVENT_BS_LOS_SFP_7                  0x0080
#define WUFE_EVENT_BS_LOS_SFP_8                  0x0100
#define WUFE_EVENT_BS_LOS_SFP_9                  0x0200
#define WUFE_EVENT_BS_LOS_SFP_10                 0x0400
#define WUFE_EVENT_BS_LOS_SFP_11                 0x0800
#define WUFE_EVENT_BS_LOS_SFP_12                 0x1000
#define WUFE_EVENT_BS_LOS_SFP_13                 0x2000
#define WUFE_EVENT_BS_LOS_SFP_14                 0x4000
#define WUFE_EVENT_BS_LOS_SFP_15                 0x8000

#define WUFE_EVENT_BS_LOCK_LOST_SYS_CLK          0x0001
#define WUFE_EVENT_BS_LOCK_LOST_DDR              0x0002
#define WUFE_EVENT_BS_LOCK_LOST_FRMR             0x0004
#define WUFE_EVENT_BS_LOCK_LOST_EXT              0x0008
#define WUFE_EVENT_BS_LOCK_LOST_ADAPT            0x0010
#define WUFE_EVENT_BS_LOCK_LOST_DIFF             0x0020
#define WUFE_EVENT_BS_LOCK_LOST_DDR0             0x0040
#define WUFE_EVENT_BS_LOCK_LOST_DDR1             0x0080
#define WUFE_EVENT_BS_LOCK_LOST_EMRX_CLK         0x0200
#define WUFE_EVENT_BS_LOCK_LOST_EMTX_CLK         0x0400

/* EMPHY events */
#define WUFE_EVENT_EMPHY_INGRESS_BUS             0x0001
#define WUFE_EVENT_EMPHY_INGRESS_MAX_BUFF_SIZE   0x0002
#define WUFE_EVENT_EMPHY_INGRESS_IF              0x0004
#define WUFE_EVENT_EMPHY_EGRESS_PHY_NOT_VALID    0x0100
#define WUFE_EVENT_EMPHY_EGRESS_BUS              0x0200
#define WUFE_EVENT_EMPHY_EGRESS_IF               0x0400
#define WUFE_EVENT_EMPHY_EGRESS_PARITY           0x0800

#define WUFE_EVENT_EMPHY (WUFE_EVENT_EMPHY_INGRESS_BUS | \
                          WUFE_EVENT_EMPHY_INGRESS_MAX_BUFF_SIZE | \
                          WUFE_EVENT_EMPHY_INGRESS_IF | \
                          WUFE_EVENT_EMPHY_EGRESS_PHY_NOT_VALID | \
                          WUFE_EVENT_EMPHY_EGRESS_BUS | \
                          WUFE_EVENT_EMPHY_EGRESS_IF | \
                          WUFE_EVENT_EMPHY_EGRESS_PARITY)

/* TDM events */
#define WUFE_EVENT_TDM_RX_PARITY   0x0001
#define WUFE_EVENT_TDM_TX_PARITY   0x0002

#define WUFE_EVENT_TDM (WUFE_EVENT_TDM_RX_PARITY | \
                        WUFE_EVENT_TDM_TX_PARITY)

/* Fifo Manager events */
#define WUFE_EVENT_FM_RX_DDR_OVERRUN        0x0001
#define WUFE_EVENT_FM_RX_BURST_OVERRUN      0x0002
#define WUFE_EVENT_FM_TX_DDR_UNDERRUN       0x0004
#define WUFE_EVENT_FM_TX_BURST_UNDERRUN     0x0008
#define WUFE_EVENT_FM_RX_ECC_ONE_BIT        0x0010
#define WUFE_EVENT_FM_TX_ECC_ONE_BIT        0x0040
#define WUFE_EVENT_FM_TX_READ_FIFO_OVERRUN  0x0100
#define WUFE_EVENT_FM_FATAL_ERROR           0x4000
#define WUFE_EVENT_FM_RX_VFP_OVERRUN        0x8000

#define WUFE_EVENT_FM (WUFE_EVENT_FM_RX_DDR_OVERRUN | \
                       WUFE_EVENT_FM_RX_BURST_OVERRUN | \
                       WUFE_EVENT_FM_TX_DDR_UNDERRUN |\
                       WUFE_EVENT_FM_TX_BURST_UNDERRUN |\
                       WUFE_EVENT_FM_RX_ECC_ONE_BIT |\
                       WUFE_EVENT_FM_TX_ECC_ONE_BIT |\
                       WUFE_EVENT_FM_TX_READ_FIFO_OVERRUN |\
                       WUFE_EVENT_FM_FATAL_ERROR |\
                       WUFE_EVENT_FM_RX_VFP_OVERRUN)

/* Machine events */
#define WUFE_EVENT_MACHINE_CES_RX_LOSS        0x0001
#define WUFE_EVENT_MACHINE_CES_RX_MF_LOSS     0x0002
#define WUFE_EVENT_MACHINE_ATM_RX_LCD         0x0004
#define WUFE_EVENT_MACHINE_HDLC_RX_ABORT      0x0008
#define WUFE_EVENT_MACHINE_HDLC_RX_CRC_ERR    0x0010
#define WUFE_EVENT_MACHINE_HDLC_RX_NON_OCTET  0x0020
#define WUFE_EVENT_MACHINE_POS_RX_ABORT       0x0040
#define WUFE_EVENT_MACHINE_POS_CRC_ERR        0x0080
#define WUFE_EVENT_MACHINE_RX_OVERRUN_BURST   0x4000
#define WUFE_EVENT_MACHINE_RX_OVERRUN_DDR     0x8000
#define WUFE_EVENT_MACHINE_CES_TX_LOSS        0x0001
#define WUFE_EVENT_MACHINE_TX_UNDERRUN_BURST  0x4000
#define WUFE_EVENT_MACHINE_TX_UNDERRUN_DDR    0x8000

#define WUFE_EVENT_MACHINE_RX (WUFE_EVENT_MACHINE_CES_RX_LOSS | \
                               WUFE_EVENT_MACHINE_CES_RX_MF_LOSS | \
                               WUFE_EVENT_MACHINE_ATM_RX_LCD | \
                               WUFE_EVENT_MACHINE_HDLC_RX_ABORT | \
                               WUFE_EVENT_MACHINE_HDLC_RX_CRC_ERR | \
                               WUFE_EVENT_MACHINE_HDLC_RX_NON_OCTET | \
                               WUFE_EVENT_MACHINE_POS_RX_ABORT | \
                               WUFE_EVENT_MACHINE_POS_CRC_ERR | \
                               WUFE_EVENT_MACHINE_RX_OVERRUN_BURST | \
                               WUFE_EVENT_MACHINE_RX_OVERRUN_DDR)
#define WUFE_EVENT_MACHINE_TX (WUFE_EVENT_MACHINE_CES_TX_LOSS |   \
                               WUFE_EVENT_MACHINE_TX_UNDERRUN_BURST | \
                               WUFE_EVENT_MACHINE_TX_UNDERRUN_DDR)

/* CR events */
#define WUFE_EVENT_CLOCK_REC_XO_RATIO_LOSS_1   0x2000
#define WUFE_EVENT_CLOCK_REC_XO_RATIO_LOSS     0x1000
#define WUFE_EVENT_CLOCK_REC_DIFF_RATIO_LOSS_1 0x0800
#define WUFE_EVENT_CLOCK_REC_DIFF_RATIO_LOSS   0x0400
#define WUFE_EVENT_CLOCK_REC_STUFF_LOSS        0x0100

#define WUFE_EVENT_CLOCK_REC (WUFE_EVENT_CLOCK_REC_XO_RATIO_LOSS_1   | \
                              WUFE_EVENT_CLOCK_REC_XO_RATIO_LOSS     | \
                              WUFE_EVENT_CLOCK_REC_DIFF_RATIO_LOSS_1 | \
                              WUFE_EVENT_CLOCK_REC_DIFF_RATIO_LOSS   | \
                              WUFE_EVENT_CLOCK_REC_STUFF_LOSS)

#else
#define WUFE_EVENT_TDM 0
#define WUFE_EVENT_MACHINE 0
#define WUFE_EVENT_FM 0
#define WUFE_EVENT_CLOCK_REC 0
#endif /* WUFE_UFE4 */

#ifdef WUFE_UFE3
#define WUFE_CAD_EVENTS 64
#else
#define WUFE_CAD_EVENTS 21
#endif
#define WUFE_SBI_EVENTS 1

typedef struct
{
   WP_U16 event_reg;
   WP_U32 tx_phy_underrun_handle;
   WP_U32 rx_phy_underrun_handle;
} WUFE_events_core;

typedef struct
{
   WP_U16 event_reg;
   WP_U32 phy_not_valid_handle;
} WUFE_events_emphy;

typedef struct
{
   WP_U16 event_reg;
} WUFE_events_sbi;

typedef struct
{
   WP_U16 event_reg;
} WUFE_events_tdm;

typedef struct
{
   WP_U16 event_reg;
   WP_U16 info[WUFE_MAX_N_CORE];
} WUFE_events_fm;

typedef struct
{
   WP_U16 pll_lost_lock_event_reg;
   WP_U16 absent_event_reg;
   WP_U16 los_event_reg;
} WUFE_events_bs;

typedef struct
{
   WP_U16 rx_event_reg;
   WP_U16 tx_event_reg;
   WP_U16 rx_event_phy_num;
   WP_U16 tx_event_phy_num;
} WUFE_events_machine;

typedef struct
{
   WP_U8 rx_event_reg;
   WP_U8 tx_event_reg;
} WUFE_events_cad;

typedef struct
{
   WP_U16 event_reg;
   WUFE_events_sbi sbi_events[WUFE_SBI_EVENTS];
   WUFE_events_cad cad_events[WUFE_CAD_EVENTS];
} WUFE_events_liu;

typedef struct
{
   WP_U16 event_reg;
   WP_U8  ct_events[WUFE_MAX_N_CLOCK_TRANSLATORS]; /* Relevant for UFE3 only */
   WP_U16 stuffing_loss_indication_line_id; /* Not relevant in UFE3 */
} WUFE_events_clock_recovery;

typedef struct
{
   WUFE_events_core           core_events[WUFE_MAX_N_CORE]; /*Relevant for UFE3 only*/
   WUFE_events_emphy          emphy_events[WUFE_MAX_N_EMPHYS];
   WUFE_events_liu            liu_events; /*Relevant for UFE3 only*/
   WUFE_events_clock_recovery cr_events;
   WUFE_events_tdm            tdm_events; /*Relevant for UFE4 only*/
   WUFE_events_fm             fm_events; /*Relevant for UFE4 only*/
   WUFE_events_machine        machine_events; /*Relevant for UFE4 only*/
   WUFE_events_bs             bs_events; /*Relevant for UFE4 only*/
} WUFE_events;

/*********************************************************/

#endif /* WUFE_UFE_H*/
