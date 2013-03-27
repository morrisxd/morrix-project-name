/*****************************************************************************
 * (C) Copyright 2000-2010, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: wufei_hw_clk_rec.h
 *
 * Purpose: The UFE4 Clock Recovery type macro and structures
 *
 ****************************************************************************/
#ifndef WUFEI_HW_CLK_REC_H
#define WUFEI_HW_CLK_REC_H

#ifndef WUFEI_HW_TYPES_H
#include "wufei_hw_types.h"
#endif


/***********************************************************************
 *                   Clock Recovery direct memory space
 **********************************************************************/

typedef struct
{
      WUFEI_reg_space trib_id; /* CR stuffing loss indication for trib ID x in lane A or B */
}WUFEI_hw_clk_rec_tx_stf_loss_trib_id;

typedef struct
{
      WUFEI_reg_space clk_rec_tx_delta_gap_high; /* CR TX  */
      WUFEI_reg_space clk_rec_tx_delta_gap_low;  /* CR TX  */
}WUFEI_hw_clk_rec_tx_delta_gap;

#define WUFEI_CLK_REC_MODE_RX_EN_S (0)
#define WUFEI_CLK_REC_MODE_RX_EN_W (1)
#define WUFEI_CLK_REC_MODE_RX_EN_F(v) \
   WUFEI_FIELD(v, WUFEI_CLK_REC_MODE_RX_EN_S)
#define WUFEI_CLK_REC_MODE_RX_EN_V(f) \
   WUFEI_VALUE(f, WUFEI_CLK_REC_MODE_RX_EN_S, WUFEI_CLK_REC_MODE_RX_EN_W)
#define WUFEI_CLK_REC_MODE_RX_EN_M \
   WUFEI_MASK(WUFEI_CLK_REC_MODE_RX_EN_S, WUFEI_CLK_REC_MODE_RX_EN_W)

/* Information for internal testing and debug function */
#define WUFEI_CLK_REC_TS_STUB_EN_S (1)
#define WUFEI_CLK_REC_TS_STUB_EN_W (1)
#define WUFEI_CLK_REC_TS_STUB_EN_F(v) \
   WUFEI_FIELD(v, WUFEI_CLK_REC_TS_STUB_EN_S)
#define WUFEI_CLK_REC_TS_STUB_EN_V(f) \
   WUFEI_VALUE(f, WUFEI_CLK_REC_TS_STUB_EN_S, WUFEI_CLK_REC_TS_STUB_EN_W)
#define WUFEI_CLK_REC_TS_STUB_EN_M \
   WUFEI_MASK(WUFEI_CLK_REC_TS_STUB_EN_S, WUFEI_CLK_REC_TS_STUB_EN_W)

#define WUFEI_CLK_REC_MODE_TX_EN_S (8)
#define WUFEI_CLK_REC_MODE_TX_EN_W (1)
#define WUFEI_CLK_REC_MODE_TX_EN_F(v) \
   WUFEI_FIELD(v, WUFEI_CLK_REC_MODE_TX_EN_S)
#define WUFEI_CLK_REC_MODE_TX_EN_V(f) \
   WUFEI_VALUE(f, WUFEI_CLK_REC_MODE_TX_EN_S, WUFEI_CLK_REC_MODE_TX_EN_W)
#define WUFEI_CLK_REC_MODE_TX_EN_M \
   WUFEI_MASK(WUFEI_CLK_REC_MODE_TX_EN_S, WUFEI_CLK_REC_MODE_TX_EN_W)

/* DCO memory map*/
typedef struct
{
   WUFEI_reg_space mode; /* used in CAD mode only */
   WUFEI_reg_space integer;
   WUFEI_reg_space remainder_msb;
   WUFEI_reg_space remainder_lsb;
}WUFEI_hw_clk_rec_dco;

/*
** DCO mode registers - divisor configuration
*/
/* '1' - enabled */
#define WUFEI_HW_CLK_REC_DCO_MODE_EN_S (0)
#define WUFEI_HW_CLK_REC_DCO_MODE_EN_W (1)
#define WUFEI_HW_CLK_REC_DCO_MODE_EN_F(v) WUFEI_FIELD(v, \
        WUFEI_HW_CLK_REC_DCO_MODE_EN_S)
#define WUFEI_HW_CLK_REC_DCO_MODE_EN_V(f) \
        WUFEI_VALUE(f, WUFEI_HW_CLK_REC_DCO_MODE_EN_S, WUFEI_HW_CLK_REC_DCO_MODE_EN_W)
#define WUFEI_HW_CLK_REC_DCO_MODE_EN_M \
        WUFEI_MASK(WUFEI_HW_CLK_REC_DCO_MODE_EN_S, WUFEI_HW_CLK_REC_DCO_MODE_EN_W)

/* E1/T1 */
#define WUFEI_HW_CLK_REC_DCO_MODE_E1T1_S (1)
#define WUFEI_HW_CLK_REC_DCO_MODE_E1T1_W (1)
#define WUFEI_HW_CLK_REC_DCO_MODE_E1T1_F(v) WUFEI_FIELD(v, \
        WUFEI_HW_CLK_REC_DCO_MODE_E1T1_S)
#define WUFEI_HW_CLK_REC_DCO_MODE_E1T1_V(f) \
        WUFEI_VALUE(f, WUFEI_HW_CLK_REC_DCO_MODE_E1T1_S, WUFEI_HW_CLK_REC_DCO_MODE_E1T1_W)
#define WUFEI_HW_CLK_REC_DCO_MODE_E1T1_M \
        WUFEI_MASK(WUFEI_HW_CLK_REC_DCO_MODE_E1T1_S, WUFEI_HW_CLK_REC_DCO_MODE_E1T1_W)


typedef struct
{
      WUFEI_reg_space clk_rec_ind_add_low;                /* CR indirect address low */
      WUFEI_reg_space clk_rec_ind_add_high;               /* CR indirect address high */
      WUFEI_reg_space clk_rec_ind_cmd;                    /* CR indirect command */
      WUFEI_reg_space clk_rec_ind_data;                   /* CR indirect data */
      WUFEI_reg_space reserved1[0x4];
      WUFEI_reg_space clk_rec_interrupt;                  /* CR interrupt register */
      WUFEI_reg_space clk_rec_interrupt_mask;             /* CR interrupt mask register */
      WUFEI_hw_clk_rec_tx_stf_loss_trib_id clk_rec_tx_stf_loss_trib_id[0x2]; /* CR stuffing loss indication for lanes A and B */
      WUFEI_reg_space reserved2[0x4];
      WUFEI_reg_space clk_rec_mode_reg;                   /* CR mode register */
      WUFEI_reg_space clk_rec_diff_sck_ratio_init_h;      /* CR Diff-to-Socket Ratio sampler initial value high */
      WUFEI_reg_space clk_rec_diff_sck_ratio_init_l;      /* CR Diff-to-Socket Ratio sampler initial value low */
      WUFEI_reg_space clk_rec_xo_sck_ratio_init_h;        /* CR Oscillator-to-Socket Ratio sampler initial value high */
      WUFEI_reg_space clk_rec_xo_sck_ratio_init_l;        /* CR Oscillator-to-Socket Ratio sampler initial value low */
      WUFEI_reg_space reserved3[0xeb];
      WUFEI_hw_clk_rec_tx_delta_gap clk_rec_tx_delta_gap_lane_a[0x2a0]; /* CR TX delta gap for lane A */
      WUFEI_reg_space reserved4[0x2c0];
      WUFEI_hw_clk_rec_tx_delta_gap clk_rec_tx_delta_gap_lane_b[0x2a0]; /* CR TX delta gap for lane B */
      WUFEI_reg_space reserved5[0x1c0];
      WUFEI_hw_clk_rec_dco dco[2];
}WUFEI_hw_clk_rec;


/***********************************************************************
 *                 Clock Recovery indirect memory space
 **********************************************************************/
typedef struct
{
      WP_U16 trib_mode;
}WUFEI_hw_clk_rec_trib_mode_ram;

#define WUFEI_HW_CLK_REC_TX_TRIB_MODE_TRIB_EN_S (0)
#define WUFEI_HW_CLK_REC_TX_TRIB_MODE_TRIB_EN_W (1)
#define WUFEI_HW_CLK_REC_TX_TRIB_MODE_TRIB_EN_F(v) \
   WUFEI_FIELD(v, WUFEI_HW_CLK_REC_TX_TRIB_MODE_TRIB_EN_S)
#define WUFEI_HW_CLK_REC_TX_TRIB_MODE_TRIB_EN_V(f) \
   WUFEI_VALUE(f, WUFEI_HW_CLK_REC_TX_TRIB_MODE_TRIB_EN_S, WUFEI_HW_CLK_REC_TX_TRIB_MODE_TRIB_EN_W)
#define WUFEI_HW_CLK_REC_TX_TRIB_MODE_TRIB_EN_M \
   WUFEI_MASK(WUFEI_HW_CLK_REC_TX_TRIB_MODE_TRIB_EN_S, WUFEI_HW_CLK_REC_TX_TRIB_MODE_TRIB_EN_W)

#define WUFEI_HW_CLK_REC_TX_TRIB_MODE_TRIB_TYPE_S (1)
#define WUFEI_HW_CLK_REC_TX_TRIB_MODE_TRIB_TYPE_W (1)
#define WUFEI_HW_CLK_REC_TX_TRIB_MODE_TRIB_TYPE_F(v) \
   WUFEI_FIELD(v, WUFEI_HW_CLK_REC_TX_TRIB_MODE_TRIB_TYPE_S)
#define WUFEI_HW_CLK_REC_TX_TRIB_MODE_TRIB_TYPE_V(f) \
   WUFEI_VALUE(f, WUFEI_HW_CLK_REC_TX_TRIB_MODE_TRIB_TYPE_S, WUFEI_HW_CLK_REC_TX_TRIB_MODE_TRIB_TYPE_W)
#define WUFEI_HW_CLK_REC_TX_TRIB_MODE_TRIB_TYPE_M \
   WUFEI_MASK(WUFEI_HW_CLK_REC_TX_TRIB_MODE_TRIB_TYPE_S, WUFEI_HW_CLK_REC_TX_TRIB_MODE_TRIB_TYPE_W)

typedef struct
{
      WP_U16 trib_stf_ind;
}WUFEI_hw_clk_rec_trib_stf_ind_ram;

#define WUFEI_HW_CLK_REC_TX_TRIB_STF_IND_S (0)
#define WUFEI_HW_CLK_REC_TX_TRIB_STF_IND_W (2)
#define WUFEI_HW_CLK_REC_TX_TRIB_STF_IND_F(v) \
   WUFEI_FIELD(v, WUFEI_HW_CLK_REC_TX_TRIB_STF_IND_S)
#define WUFEI_HW_CLK_REC_TX_TRIB_STF_IND_V(f) \
   WUFEI_VALUE(f, WUFEI_HW_CLK_REC_TX_TRIB_STF_IND_S, WUFEI_HW_CLK_REC_TX_TRIB_STF_IND_W)
#define WUFEI_HW_CLK_REC_TX_TRIB_STF_IND_M \
   WUFEI_MASK(WUFEI_HW_CLK_REC_TX_TRIB_STF_IND_S, WUFEI_HW_CLK_REC_TX_TRIB_STF_IND_W)


typedef struct
{
      WP_U16 trib_gap_cnt;
}WUFEI_hw_clk_rec_trib_gap_count_ram;

#define WUFEI_HW_CLK_REC_TX_TRIB_MAX_GAP_PTR_S (0)
#define WUFEI_HW_CLK_REC_TX_TRIB_MAX_GAP_PTR_W (8)
#define WUFEI_HW_CLK_REC_TX_TRIB_MAX_GAP_PTR_F(v) \
   WUFEI_FIELD(v, WUFEI_HW_CLK_REC_TX_TRIB_MAX_GAP_PTR_S)
#define WUFEI_HW_CLK_REC_TX_TRIB_MAX_GAP_PTR_V(f) \
   WUFEI_VALUE(f, WUFEI_HW_CLK_REC_TX_TRIB_MAX_GAP_PTR_S, WUFEI_HW_CLK_REC_TX_TRIB_MAX_GAP_PTR_W)
#define WUFEI_HW_CLK_REC_TX_TRIB_MAX_GAP_PTR_M \
   WUFEI_MASK(WUFEI_HW_CLK_REC_TX_TRIB_MAX_GAP_PTR_S, WUFEI_HW_CLK_REC_TX_TRIB_MAX_GAP_PTR_W)

#define WUFEI_HW_CLK_REC_TX_TRIB_CURRENT_GAP_CNT_S (8)
#define WUFEI_HW_CLK_REC_TX_TRIB_CURRENT_GAP_CNT_W (4)
#define WUFEI_HW_CLK_REC_TX_TRIB_CURRENT_GAP_CNT_F(v) \
   WUFEI_FIELD(v, WUFEI_HW_CLK_REC_TX_TRIB_CURRENT_GAP_CNT_S)
#define WUFEI_HW_CLK_REC_TX_TRIB_CURRENT_GAP_CNT_V(f) \
   WUFEI_VALUE(f, WUFEI_HW_CLK_REC_TX_TRIB_CURRENT_GAP_CNT_S, WUFEI_HW_CLK_REC_TX_TRIB_CURRENT_GAP_CNT_W)
#define WUFEI_HW_CLK_REC_TX_TRIB_CURRENT_GAP_CNT_M \
   WUFEI_MASK(WUFEI_HW_CLK_REC_TX_TRIB_CURRENT_GAP_CNT_S, WUFEI_HW_CLK_REC_TX_TRIB_CURRENT_GAP_CNT_W)


typedef struct
{
      WP_U16 trib_gap_frac_cnt_h;
      WP_U16 trib_gap_frac_cnt_l;
}WUFEI_hw_clk_rec_trib_gap_frac_count_ram;


typedef struct
{
      WP_U16 trib_delta_gap_h;
      WP_U16 trib_delta_gap_l;
}WUFEI_hw_clk_rec_trib_delta_gap_ram;

/* Interrupt register bits */
#define WUFEI_CLK_REC_INT_RATIO_LOSS_1_S (11)
#define WUFEI_CLK_REC_INT_RATIO_LOSS_1_W (1)
#define WUFEI_CLK_REC_INT_RATIO_LOSS_1_F(v) \
   WUFEI_FIELD(v, WUFEI_CLK_REC_INT_RATIO_LOSS_1_S)
#define WUFEI_CLK_REC_INT_RATIO_LOSS_1_V(f) \
   WUFEI_VALUE(f, WUFEI_CLK_REC_INT_RATIO_LOSS_1_S, WUFEI_CLK_REC_INT_RATIO_LOSS_1_W)
#define WUFEI_CLK_REC_INT_RATIO_LOSS_1_M \
   WUFEI_MASK(WUFEI_CLK_REC_INT_RATIO_LOSS_1_S, WUFEI_CLK_REC_INT_RATIO_LOSS_1_W)

#define WUFEI_CLK_REC_INT_RATIO_LOSS_0_S (10)
#define WUFEI_CLK_REC_INT_RATIO_LOSS_0_W (1)
#define WUFEI_CLK_REC_INT_RATIO_LOSS_0_F(v) \
   WUFEI_FIELD(v, WUFEI_CLK_REC_INT_RATIO_LOSS_0_S)
#define WUFEI_CLK_REC_INT_RATIO_LOSS_0_V(f) \
   WUFEI_VALUE(f, WUFEI_CLK_REC_INT_RATIO_LOSS_0_S, WUFEI_CLK_REC_INT_RATIO_LOSS_0_W)
#define WUFEI_CLK_REC_INT_RATIO_LOSS_0_M \
   WUFEI_MASK(WUFEI_CLK_REC_INT_RATIO_LOSS_0_S, WUFEI_CLK_REC_INT_RATIO_LOSS_0_W)

#define WUFEI_CLK_REC_INT_STUFF_LOSS_LANE_B_S (9)
#define WUFEI_CLK_REC_INT_STUFF_LOSS_LANE_B_W (1)
#define WUFEI_CLK_REC_INT_STUFF_LOSS_LANE_B_F(v) \
   WUFEI_FIELD(v, WUFEI_CLK_REC_INT_STUFF_LOSS_LANE_B_S)
#define WUFEI_CLK_REC_INT_STUFF_LOSS_LANE_B_V(f) \
   WUFEI_VALUE(f, WUFEI_CLK_REC_INT_STUFF_LOSS_LANE_B_S, WUFEI_CLK_REC_INT_STUFF_LOSS_LANE_B_W)
#define WUFEI_CLK_REC_INT_STUFF_LOSS_LANE_B_M \
   WUFEI_MASK(WUFEI_CLK_REC_INT_STUFF_LOSS_LANE_B_S, WUFEI_CLK_REC_INT_STUFF_LOSS_LANE_B_W)

#define WUFEI_CLK_REC_INT_STUFF_LOSS_LANE_A_S (8)
#define WUFEI_CLK_REC_INT_STUFF_LOSS_LANE_A_W (1)
#define WUFEI_CLK_REC_INT_STUFF_LOSS_LANE_A_F(v) \
   WUFEI_FIELD(v, WUFEI_CLK_REC_INT_STUFF_LOSS_LANE_A_S)
#define WUFEI_CLK_REC_INT_STUFF_LOSS_LANE_A_V(f) \
   WUFEI_VALUE(f, WUFEI_CLK_REC_INT_STUFF_LOSS_LANE_A_S, WUFEI_CLK_REC_INT_STUFF_LOSS_LANE_A_W)
#define WUFEI_CLK_REC_INT_STUFF_LOSS_LANE_A_M \
   WUFEI_MASK(WUFEI_CLK_REC_INT_STUFF_LOSS_LANE_A_S, WUFEI_CLK_REC_INT_STUFF_LOSS_LANE_A_W)

#define WUFEI_CLK_REC_MASK_INT_STUFF_LOSS_LANE_B_S (9)
#define WUFEI_CLK_REC_MASK_INT_STUFF_LOSS_LANE_B_W (1)
#define WUFEI_CLK_REC_MASK_INT_STUFF_LOSS_LANE_B_F(v) \
   WUFEI_FIELD(v, WUFEI_CLK_REC_MASK_INT_STUFF_LOSS_LANE_B_S)
#define WUFEI_CLK_REC_MASK_INT_STUFF_LOSS_LANE_B_V(f) \
   WUFEI_VALUE(f, WUFEI_CLK_REC_MASK_INT_STUFF_LOSS_LANE_B_S, WUFEI_CLK_REC_MASK_INT_STUFF_LOSS_LANE_B_W)
#define WUFEI_CLK_REC_MASK_INT_STUFF_LOSS_LANE_B_M \
   WUFEI_MASK(WUFEI_CLK_REC_MASK_INT_STUFF_LOSS_LANE_B_S, WUFEI_CLK_REC_MASK_INT_STUFF_LOSS_LANE_B_W)




/***************************************************************
 ***************** UFE3 functionality  *************************
 **************************************************************/

/*
** Clock Recovery RX timestamp registers
** One Register Set is defined per line
*/
/* Clock Recovery enable indicator*/
#define WUFEI_HW_CLK_REC_RX_MODE_EN_S (0)
#define WUFEI_HW_CLK_REC_RX_MODE_EN_W (1)
#define WUFEI_HW_CLK_REC_RX_MODE_EN_F(v) WUFEI_FIELD(v, WUFEI_HW_CLK_REC_RX_MODE_EN_S)
#define WUFEI_HW_CLK_REC_RX_MODE_EN_V(f) \
        WUFEI_VALUE(f, WUFEI_HW_CLK_REC_RX_MODE_EN_S, WUFEI_HW_CLK_REC_RX_MODE_EN_W)
#define WUFEI_HW_CLK_REC_RX_MODE_EN_M \
        WUFEI_MASK(WUFEI_HW_CLK_REC_RX_MODE_EN_S, WUFEI_HW_CLK_REC_RX_MODE_EN_W)

/* 1-Mask RAM is used as extension for the Port RAM  */
#define WUFEI_HW_CLK_REC_RX_NO_MASK_S (1)
#define WUFEI_HW_CLK_REC_RX_NO_MASK_W (1)
#define WUFEI_HW_CLK_REC_RX_NO_MASK_F(v) WUFEI_FIELD(v, WUFEI_HW_CLK_REC_RX_NO_MASK_S)
#define WUFEI_HW_CLK_REC_RX_NO_MASK_V(f) \
        WUFEI_VALUE(f, WUFEI_HW_CLK_REC_RX_NO_MASK_S, WUFEI_HW_CLK_REC_RX_NO_MASK_W)
#define WUFEI_HW_CLK_REC_RX_NO_MASK_M \
        WUFEI_MASK(WUFEI_HW_CLK_REC_RX_NO_MASK_S, WUFEI_HW_CLK_REC_RX_NO_MASK_W)

/* for CAD only  */
#define WUFEI_HW_CLK_REC_RX_FAST_PORT_S (2)
#define WUFEI_HW_CLK_REC_RX_FAST_PORT_W (1)
#define WUFEI_HW_CLK_REC_RX_FAST_PORT_F(v) WUFEI_FIELD(v, WUFEI_HW_CLK_REC_RX_FAST_PORT_S)
#define WUFEI_HW_CLK_REC_RX_FAST_PORT_V(f) \
        WUFEI_VALUE(f, WUFEI_HW_CLK_REC_RX_FAST_PORT_S, WUFEI_HW_CLK_REC_RX_FAST_PORT_W)
#define WUFEI_HW_CLK_REC_RX_FAST_PORT_M \
        WUFEI_MASK(WUFEI_HW_CLK_REC_RX_FAST_PORT_S, WUFEI_HW_CLK_REC_RX_FAST_PORT_W)

/*Port RAM entry*/
#define WUFEI_HW_CLK_REC_RX_PORT_EN_S (15)
#define WUFEI_HW_CLK_REC_RX_PORT_EN_W (1)
#define WUFEI_HW_CLK_REC_RX_PORT_EN_F(v) WUFEI_FIELD(v, WUFEI_HW_CLK_REC_RX_PORT_EN_S)
#define WUFEI_HW_CLK_REC_RX_PORT_EN_V(f) \
        WUFEI_VALUE(f, WUFEI_HW_CLK_REC_RX_PORT_EN_S, WUFEI_HW_CLK_REC_RX_PORT_EN_W)
#define WUFEI_HW_CLK_REC_RX_PORT_EN_M \
        WUFEI_MASK(WUFEI_HW_CLK_REC_RX_PORT_EN_S, WUFEI_HW_CLK_REC_RX_PORT_EN_W)

#define WUFEI_HW_CLK_REC_RX_PORT_LAST_S (14)
#define WUFEI_HW_CLK_REC_RX_PORT_LAST_W (1)
#define WUFEI_HW_CLK_REC_RX_PORT_LAST_F(v) WUFEI_FIELD(v, WUFEI_HW_CLK_REC_RX_PORT_LAST_S)
#define WUFEI_HW_CLK_REC_RX_PORT_LAST_V(f) \
        WUFEI_VALUE(f, WUFEI_HW_CLK_REC_RX_PORT_LAST_S, WUFEI_HW_CLK_REC_RX_PORT_LAST_W)
#define WUFEI_HW_CLK_REC_RX_PORT_LAST_M \
        WUFEI_MASK(WUFEI_HW_CLK_REC_RX_PORT_LAST_S, WUFEI_HW_CLK_REC_RX_PORT_LAST_W)

#define WUFEI_HW_CLK_REC_RX_CORE_ID_S (13)
#define WUFEI_HW_CLK_REC_RX_CORE_ID_W (1)
#define WUFEI_HW_CLK_REC_RX_CORE_ID_F(v) WUFEI_FIELD(v, WUFEI_HW_CLK_REC_RX_CORE_ID_S)
#define WUFEI_HW_CLK_REC_RX_CORE_ID_V(f) \
        WUFEI_VALUE(f, WUFEI_HW_CLK_REC_RX_CORE_ID_S, WUFEI_HW_CLK_REC_RX_CORE_ID_W)
#define WUFEI_HW_CLK_REC_RX_CORE_ID_M \
        WUFEI_MASK(WUFEI_HW_CLK_REC_RX_CORE_ID_S, WUFEI_HW_CLK_REC_RX_CORE_ID_W)

#define WUFEI_HW_CLK_REC_RX_PORT_ID_S (0)
#define WUFEI_HW_CLK_REC_RX_PORT_ID_W (12)
#define WUFEI_HW_CLK_REC_RX_PORT_ID_F(v) WUFEI_FIELD(v, WUFEI_HW_CLK_REC_RX_PORT_ID_S)
#define WUFEI_HW_CLK_REC_RX_PORT_ID_V(f) \
        WUFEI_VALUE(f, WUFEI_HW_CLK_REC_RX_PORT_ID_S, WUFEI_HW_CLK_REC_RX_PORT_ID_W)
#define WUFEI_HW_CLK_REC_RX_PORT_ID_M \
        WUFEI_MASK(WUFEI_HW_CLK_REC_RX_PORT_ID_S, WUFEI_HW_CLK_REC_RX_PORT_ID_W)

#define WUFEI_HW_CLK_REC_RX_MASK_LOAD_VAL_S (8)
#define WUFEI_HW_CLK_REC_RX_MASK_LOAD_VAL_W (6)
#define WUFEI_HW_CLK_REC_RX_MASK_LOAD_VAL_F(v) WUFEI_FIELD(v, WUFEI_HW_CLK_REC_RX_MASK_LOAD_VAL_S)
#define WUFEI_HW_CLK_REC_RX_MASK_LOAD_VAL_V(f) \
        WUFEI_VALUE(f, WUFEI_HW_CLK_REC_RX_MASK_LOAD_VAL_S, WUFEI_HW_CLK_REC_RX_MASK_LOAD_VAL_W)
#define WUFEI_HW_CLK_REC_RX_MASK_LOAD_VAL_M \
        WUFEI_MASK(WUFEI_HW_CLK_REC_RX_MASK_LOAD_VAL_S, WUFEI_HW_CLK_REC_RX_MASK_LOAD_VAL_W)

#define WUFEI_HW_CLK_REC_RX_MASK_CNT_S (0)
#define WUFEI_HW_CLK_REC_RX_MASK_CNT_W (6)
#define WUFEI_HW_CLK_REC_RX_MASK_CNT_F(v) WUFEI_FIELD(v, WUFEI_HW_CLK_REC_RX_MASK_CNT_S)
#define WUFEI_HW_CLK_REC_RX_MASK_CNT_V(f) \
        WUFEI_VALUE(f, WUFEI_HW_CLK_REC_RX_MASK_CNT_S, WUFEI_HW_CLK_REC_RX_MASK_CNT_W)
#define WUFEI_HW_CLK_REC_RX_MASK_CNT_M \
        WUFEI_MASK(WUFEI_HW_CLK_REC_RX_MASK_CNT_S, WUFEI_HW_CLK_REC_RX_MASK_CNT_W)

/*
** Clock Recovery TX timestamp register
** One Register Set is defined per line
*/
/* Clock Recovery enable indicator*/
#define WUFEI_HW_CLK_REC_TX_MODE_EN_S (0)
#define WUFEI_HW_CLK_REC_TX_MODE_EN_W (1)
#define WUFEI_HW_CLK_REC_TX_MODE_EN_F(v) WUFEI_FIELD(v, WUFEI_HW_CLK_REC_TX_MODE_EN_S)
#define WUFEI_HW_CLK_REC_TX_MODE_EN_V(f) \
        WUFEI_VALUE(f, WUFEI_HW_CLK_REC_TX_MODE_EN_S, WUFEI_HW_CLK_REC_TX_MODE_EN_W)
#define WUFEI_HW_CLK_REC_TX_MODE_EN_M \
        WUFEI_MASK(WUFEI_HW_CLK_REC_TX_MODE_EN_S, WUFEI_HW_CLK_REC_TX_MODE_EN_W)

/* buffer_size = num_of_slots_in_MasterPHY/data_unit_size
                 * num_of_bits_tributary [256 E1, 192-T1 frm, 193-T1 unfrm]  */
#define WUFEI_HW_CLK_REC_TX_BUFFER_SIZE_S (0)
#define WUFEI_HW_CLK_REC_TX_BUFFER_SIZE_W (14)
#define WUFEI_HW_CLK_REC_TX_BUFFER_SIZE_F(v) WUFEI_FIELD(v, WUFEI_HW_CLK_REC_TX_BUFFER_SIZE_S)
#define WUFEI_HW_CLK_REC_TX_BUFFER_SIZE_V(f) \
        WUFEI_VALUE(f, WUFEI_HW_CLK_REC_TX_BUFFER_SIZE_S, WUFEI_HW_CLK_REC_TX_BUFFER_SIZE_W)
#define WUFEI_HW_CLK_REC_TX_BUFFER_SIZE_M \
        WUFEI_MASK(WUFEI_HW_CLK_REC_TX_BUFFER_SIZE_S, WUFEI_HW_CLK_REC_TX_BUFFER_SIZE_W)

/*
** Clock Recovery RAM RX Clock Generator registers
*/

/*Clock recovery generator id 0-31 */
#define WUFEI_HW_CLK_REC_CGENRX_ID_S (0)
#define WUFEI_HW_CLK_REC_CGENRX_ID_W (9)
#define WUFEI_HW_CLK_REC_CGENRX_ID_F(v) WUFEI_FIELD(v, WUFEI_HW_CLK_REC_CGENRX_ID_S)
#define WUFEI_HW_CLK_REC_CGENRX_ID_V(f) \
        WUFEI_VALUE(f, WUFEI_HW_CLK_REC_CGENRX_ID_S, WUFEI_HW_CLK_REC_CGENRX_ID_W)
#define WUFEI_HW_CLK_REC_CGENRX_ID_M \
        WUFEI_MASK(WUFEI_HW_CLK_REC_CGENRX_ID_S, WUFEI_HW_CLK_REC_CGENRX_ID_W)

/*Tributary last received phase */
#define WUFEI_HW_CLK_REC_CGENRX_PHASE_S (9)
#define WUFEI_HW_CLK_REC_CGENRX_PHASE_W (4)
#define WUFEI_HW_CLK_REC_CGENRX_PHASE_F(v) WUFEI_FIELD(v, WUFEI_HW_CLK_REC_CGENRX_PHASE_S)
#define WUFEI_HW_CLK_REC_CGENRX_PHASE_V(f) \
        WUFEI_VALUE(f, WUFEI_HW_CLK_REC_CGENRX_PHASE_S, WUFEI_HW_CLK_REC_CGENRX_PHASE_W)
#define WUFEI_HW_CLK_REC_CGENRX_PHASE_M \
        WUFEI_MASK(WUFEI_HW_CLK_REC_CGENRX_PHASE_S, WUFEI_HW_CLK_REC_CGENRX_PHASE_W)

/* 0 - DS3/E3; 1 - E1/T1  */
#define WUFEI_HW_CLK_REC_CGENRX_EN_S (15)
#define WUFEI_HW_CLK_REC_CGENRX_EN_W (1)
#define WUFEI_HW_CLK_REC_CGENRX_EN_F(v) WUFEI_FIELD(v, WUFEI_HW_CLK_REC_CGENRX_EN_S)
#define WUFEI_HW_CLK_REC_CGENRX_EN_V(f) \
        WUFEI_VALUE(f, WUFEI_HW_CLK_REC_CGENRX_EN_S, WUFEI_HW_CLK_REC_CGENRX_EN_W)
#define WUFEI_HW_CLK_REC_CGENRX_EN_M \
        WUFEI_MASK(WUFEI_HW_CLK_REC_CGENRX_EN_S, WUFEI_HW_CLK_REC_CGENRX_EN_W)

#define WUFEI_HW_CLK_REC_CGENRX_VALID_S (14)
#define WUFEI_HW_CLK_REC_CGENRX_VALID_W (1)
#define WUFEI_HW_CLK_REC_CGENRX_VALID_F(v) WUFEI_FIELD(v, WUFEI_HW_CLK_REC_CGENRX_VALID_S)
#define WUFEI_HW_CLK_REC_CGENRX_VALID_V(f) \
        WUFEI_VALUE(f, WUFEI_HW_CLK_REC_CGENRX_VALID_S, WUFEI_HW_CLK_REC_CGENRX_VALID_W)
#define WUFEI_HW_CLK_REC_CGENRX_VALID_M \
        WUFEI_MASK(WUFEI_HW_CLK_REC_CGENRX_VALID_S, WUFEI_HW_CLK_REC_CGENRX_VALID_W)

/* this define is in shared code but used for UFE2 only */
#define WUFEI_HW_CLK_REC_CGENRX_LINK_RATE_M 0

/***********************************************************************
 *                   Clock Recovery indirect memory space
 **********************************************************************/
#define WUFEI_MAX_N_HW_PORT_RAM_SLOT 256
#define WUFEI_MAX_N_HW_MASK_RAM_SLOT 256

typedef struct
{
   WP_U8 slot[WUFEI_MAX_N_HW_PORT_RAM_SLOT];
}WUFEI_hw_clk_rec_ts_port_ram;

typedef struct
{
   WP_U8 slot[WUFEI_MAX_N_HW_PORT_RAM_SLOT];
}WUFEI_hw_clk_rec_ts_mask_ram;

typedef struct
{
   WP_U16 cgen_rx;/* Clock Generator config. register */

}WUFEI_hw_clk_rec_cgen_rx_ram;


/***************************************************************
 ************* End of UFE3 functionality  **********************
 **************************************************************/

#endif /* WUFEI_HW_CLK_REC_H*/
