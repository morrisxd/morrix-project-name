/*****************************************************************************
 * (C) Copyright 2000-2006, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: wufei_hw_sbi.h
 *
 * Purpose: the UFE Line from SBI type macro and structures
 *
 ****************************************************************************/
#ifndef WUFEI_HW_SBI_H
#define WUFEI_HW_SBI_H

#include "wufe_line.h"

#ifndef WUFEI_HW_TYPES_H
#include "wufei_hw_types.h"
#endif
/***********************************************************************
 *                      GENERAL SYSTEM registers and structures
 **********************************************************************/
#define WUFEI_SBI_ID_0 0
/***********************************************************************
 *                      MODE register
 **********************************************************************/
#define WUFEI_SBI_SYS_LOOP_S (1)
#define WUFEI_SBI_SYS_LOOP_W (1)
#define WUFEI_SBI_SYS_LOOP_F(v) WUFEI_FIELD(v, WUFEI_SBI_SYS_LOOP_S)
#define WUFEI_SBI_SYS_LOOP_V(f) \
        WUFEI_VALUE(f, WUFEI_SBI_SYS_LOOP_S, WUFEI_SBI_SYS_LOOP_W)
#define WUFEI_SBI_SYS_LOOP_M \
        WUFEI_MASK(WUFEI_SBI_SYS_LOOP_S, WUFEI_SBI_SYS_LOOP_W)

#define WUFEI_SBI_LIN_LOOP_S (2)
#define WUFEI_SBI_LIN_LOOP_W (1)
#define WUFEI_SBI_LIN_LOOP_F(v) WUFEI_FIELD(v, WUFEI_SBI_LIN_LOOP_S)
#define WUFEI_SBI_LIN_LOOP_V(f) \
        WUFEI_VALUE(f, WUFEI_SBI_LIN_LOOP_S, WUFEI_SBI_LIN_LOOP_W)
#define WUFEI_SBI_LIN_LOOP_M \
        WUFEI_MASK(WUFEI_SBI_LIN_LOOP_S, WUFEI_SBI_LIN_LOOP_W)

#define WUFEI_SBI_MODE_RX_PRTY_CHK_S (3)
#define WUFEI_SBI_MODE_RX_PRTY_CHK_W (1)
#define WUFEI_SBI_MODE_RX_PRTY_CHK_F(v) WUFEI_FIELD(v, WUFEI_SBI_MODE_RX_PRTY_CHK_S)
#define WUFEI_SBI_MODE_RX_PRTY_CHK_V(f) \
        WUFEI_VALUE(f, WUFEI_SBI_MODE_RX_PRTY_CHK_S, WUFEI_SBI_MODE_RX_PRTY_CHK_W)
#define WUFEI_SBI_MODE_RX_PRTY_CHK_M \
        WUFEI_MASK(WUFEI_SBI_MODE_RX_PRTY_CHK_S, WUFEI_SBI_MODE_RX_PRTY_CHK_W)

#define WUFEI_SBI_MODE_RX_PRTY_EVEN_S (4)
#define WUFEI_SBI_MODE_RX_PRTY_EVEN_W (1)
#define WUFEI_SBI_MODE_RX_PRTY_EVEN_F(v) WUFEI_FIELD(v, WUFEI_SBI_MODE_RX_PRTY_EVEN_S)
#define WUFEI_SBI_MODE_RX_PRTY_EVEN_V(f) \
        WUFEI_VALUE(f, WUFEI_SBI_MODE_RX_PRTY_EVEN_S, WUFEI_SBI_MODE_RX_PRTY_EVEN_W)
#define WUFEI_SBI_MODE_RX_PRTY_EVEN_M \
        WUFEI_MASK(WUFEI_SBI_MODE_RX_PRTY_EVEN_S, WUFEI_SBI_MODE_RX_PRTY_EVEN_W)

#define WUFEI_SBI_MODE_TX_PRTY_EVEN_S (5)
#define WUFEI_SBI_MODE_TX_PRTY_EVEN_W (1)
#define WUFEI_SBI_MODE_TX_PRTY_EVEN_F(v) WUFEI_FIELD(v, WUFEI_SBI_MODE_TX_PRTY_EVEN_S)
#define WUFEI_SBI_MODE_TX_PRTY_EVEN_V(f) \
        WUFEI_VALUE(f, WUFEI_SBI_MODE_TX_PRTY_EVEN_S, WUFEI_SBI_MODE_TX_PRTY_EVEN_W)
#define WUFEI_SBI_MODE_TX_PRTY_EVEN_M \
        WUFEI_MASK(WUFEI_SBI_MODE_TX_PRTY_EVEN_S, WUFEI_SBI_MODE_TX_PRTY_EVEN_W)

#define WUFEI_SBI_MODE_RX_ENABLE_S (14)
#define WUFEI_SBI_MODE_RX_ENABLE_W (1)
#define WUFEI_SBI_MODE_RX_ENABLE_F(v) WUFEI_FIELD(v, WUFEI_SBI_MODE_RX_ENABLE_S)
#define WUFEI_SBI_MODE_RX_ENABLE_V(f) \
        WUFEI_VALUE(f, WUFEI_SBI_MODE_RX_ENABLE_S, WUFEI_SBI_MODE_RX_ENABLE_W)
#define WUFEI_SBI_MODE_RX_ENABLE_M \
        WUFEI_MASK(WUFEI_SBI_MODE_RX_ENABLE_S, WUFEI_SBI_MODE_RX_ENABLE_W)

#define WUFEI_SBI_MODE_TX_ENABLE_S (15)
#define WUFEI_SBI_MODE_TX_ENABLE_W (1)
#define WUFEI_SBI_MODE_TX_ENABLE_F(v) \
        WUFEI_FIELD(v, WUFEI_SBI_MODE_TX_ENABLE_S)
#define WUFEI_SBI_MODE_TX_ENABLE_V(f) \
        WUFEI_VALUE(f, WUFEI_SBI_MODE_TX_ENABLE_S, WUFEI_SBI_MODE_TX_ENABLE_W)
#define WUFEI_SBI_MODE_TX_ENABLE_M \
        WUFEI_MASK(WUFEI_SBI_MODE_TX_ENABLE_S, WUFEI_SBI_MODE_TX_ENABLE_W)

/***********************************************************************
 *                      SPE 1,2 or 3  register
 **********************************************************************/
#define WUFEI_SBI_SPE_LOC(spe) (spe == 0) ? 0 : (1<<(spe+1))

#define WUFEI_SBI_SPE_X_ENABLE_S(spe) WUFEI_SBI_SPE_LOC(spe)
#define WUFEI_SBI_SPE_X_ENABLE_W (1)
#define WUFEI_SBI_SPE_X_ENABLE_F(v, spe) WUFEI_FIELD(v, WUFEI_SBI_SPE_X_ENABLE_S(spe))
#define WUFEI_SBI_SPE_X_ENABLE_V(f, spe) \
        WUFEI_VALUE(f, WUFEI_SBI_SPE_X_ENABLE_S(spe), WUFEI_SBI_SPE_X_ENABLE_W)
#define WUFEI_SBI_SPE_X_ENABLE_M(spe) \
        WUFEI_MASK(WUFEI_SBI_SPE_X_ENABLE_S(spe), WUFEI_SBI_SPE_X_ENABLE_W)

#define WUFEI_SBI_SPE_X_TRANFER_TYPE_S(spe) (1+(WUFEI_SBI_SPE_LOC(spe)))
#define WUFEI_SBI_SPE_X_TRANFER_TYPE_W (3)
#define WUFEI_SBI_SPE_X_TRANFER_TYPE_F(v, spe) WUFEI_FIELD(v, WUFEI_SBI_SPE_X_TRANFER_TYPE_S(spe))
#define WUFEI_SBI_SPE_X_TRANFER_TYPE_V(f, spe) \
        WUFEI_VALUE(f, WUFEI_SBI_SPE_X_TRANFER_TYPE_S(spe), WUFEI_SBI_SPE_X_TRANFER_TYPE_W)
#define WUFEI_SBI_SPE_X_TRANFER_TYPE_M(spe) \
        WUFEI_MASK(WUFEI_SBI_SPE_X_TRANFER_TYPE_S(spe), WUFEI_SBI_SPE_X_TRANFER_TYPE_W)

typedef struct
{
   WUFEI_reg_space stm1_rx[WUFE_MAX_N_STM1];
   WUFEI_reg_space stm1_tx[WUFE_MAX_N_STM1];
   WUFEI_reg_space reserved[12];
}WUFEI_hw_sbi_core;

typedef struct
{
   WUFEI_reg_space indla;
   WUFEI_reg_space indha;
   WUFEI_reg_space inddat;
   WUFEI_reg_space gmr;
   WUFEI_reg_space event_reg;
   WUFEI_reg_space event_mask_reg;
   WUFEI_reg_space reserved[10];
   WUFEI_hw_sbi_core core[WUFE_MAX_N_CORE];
}WUFEI_hw_sbi;

#define WUFEI_SBI_CAS_RAM_ENTRIES 8
#define WUFEI_SBI_CAS_RAM_SLOT_MASK 0xf
#define WUFEI_SBI_CAS_RAM_SLOT_SIZE 4
/***********************************************************************
 *                      LINE registers and structures
 **********************************************************************/
/*********************************************
 *            RX
 ********************************************/
#define WUFEI_LN_SBI_RX_ENABLE_S (0)
#define WUFEI_LN_SBI_RX_ENABLE_W (1)
#define WUFEI_LN_SBI_RX_ENABLE_F(v) WUFEI_FIELD(v, WUFEI_LN_SBI_RX_ENABLE_S)
#define WUFEI_LN_SBI_RX_ENABLE_V(f) \
        WUFEI_VALUE(f, WUFEI_LN_SBI_RX_ENABLE_S, WUFEI_LN_SBI_RX_ENABLE_W)
#define WUFEI_LN_SBI_RX_ENABLE_M \
        WUFEI_MASK(WUFEI_LN_SBI_RX_ENABLE_S, WUFEI_LN_SBI_RX_ENABLE_W)

#define WUFEI_LN_SBI_RX_FRAMED_S (1)
#define WUFEI_LN_SBI_RX_FRAMED_W (1)
#define WUFEI_LN_SBI_RX_FRAMED_F(v) WUFEI_FIELD(v, WUFEI_LN_SBI_RX_FRAMED_S)
#define WUFEI_LN_SBI_RX_FRAMED_V(f) \
        WUFEI_VALUE(f, WUFEI_LN_SBI_RX_FRAMED_S, WUFEI_LN_SBI_RX_FRAMED_W)
#define WUFEI_LN_SBI_RX_FRAMED_M \
        WUFEI_MASK(WUFEI_LN_SBI_RX_FRAMED_S, WUFEI_LN_SBI_RX_FRAMED_W)

#define WUFEI_LN_SBI_RX_MF_MODE_S (2)
#define WUFEI_LN_SBI_RX_MF_MODE_W (2)
#define WUFEI_LN_SBI_RX_MF_MODE_F(v) WUFEI_FIELD(v, WUFEI_LN_SBI_RX_MF_MODE_S)
#define WUFEI_LN_SBI_RX_MF_MODE_V(f) \
        WUFEI_VALUE(f, WUFEI_LN_SBI_RX_MF_MODE_S, WUFEI_LN_SBI_RX_MF_MODE_W)
#define WUFEI_LN_SBI_RX_MF_MODE_M \
        WUFEI_MASK(WUFEI_LN_SBI_RX_MF_MODE_S, WUFEI_LN_SBI_RX_MF_MODE_W)

#define WUFEI_LN_SBI_RX_T1oE1_MODE_S (4)
#define WUFEI_LN_SBI_RX_T1oE1_MODE_W (1)
#define WUFEI_LN_SBI_RX_T1oE1_MODE_F(v) WUFEI_FIELD(v, WUFEI_LN_SBI_RX_T1oE1_MODE_S)
#define WUFEI_LN_SBI_RX_T1oE1_MODE_V(f) \
        WUFEI_VALUE(f, WUFEI_LN_SBI_RX_T1oE1_MODE_S, WUFEI_LN_SBI_RX_T1oE1_MODE_W)
#define WUFEI_LN_SBI_RX_T1oE1_MODE_M \
        WUFEI_MASK(WUFEI_LN_SBI_RX_T1oE1_MODE_S, WUFEI_LN_SBI_RX_T1oE1_MODE_W)

#define WUFEI_LN_SBI_RX_E3_ATM_G751_MODE_S (5)
#define WUFEI_LN_SBI_RX_E3_ATM_G751_MODE_W (1)
#define WUFEI_LN_SBI_RX_E3_ATM_G751_MODE_F(v) WUFEI_FIELD(v, WUFEI_LN_SBI_RX_E3_ATM_G751_MODE_S)
#define WUFEI_LN_SBI_RX_E3_ATM_G751_MODE_V(f) \
        WUFEI_VALUE(f, WUFEI_LN_SBI_RX_E3_ATM_G751_MODE_S, WUFEI_LN_SBI_RX_E3_ATM_G751_MODE_W)
#define WUFEI_LN_SBI_RX_E3_ATM_G751_MODE_M \
        WUFEI_MASK(WUFEI_LN_SBI_RX_E3_ATM_G751_MODE_S, WUFEI_LN_SBI_RX_E3_ATM_G751_MODE_W)

/*********************************************
 *            TX
 ********************************************/
#define WUFEI_LN_SBI_TX_ENABLE_S (0)
#define WUFEI_LN_SBI_TX_ENABLE_W (1)
#define WUFEI_LN_SBI_TX_ENABLE_F(v) WUFEI_FIELD(v, WUFEI_LN_SBI_TX_ENABLE_S)
#define WUFEI_LN_SBI_TX_ENABLE_V(f) \
        WUFEI_VALUE(f, WUFEI_LN_SBI_TX_ENABLE_S, WUFEI_LN_SBI_TX_ENABLE_W)
#define WUFEI_LN_SBI_TX_ENABLE_M \
        WUFEI_MASK(WUFEI_LN_SBI_TX_ENABLE_S, WUFEI_LN_SBI_TX_ENABLE_W)

#define WUFEI_LN_SBI_TX_CLK_MASTER_S (1)
#define WUFEI_LN_SBI_TX_CLK_MASTER_W (1)
#define WUFEI_LN_SBI_TX_CLK_MASTER_F(v) WUFEI_FIELD(v, WUFEI_LN_SBI_TX_CLK_MASTER_S)
#define WUFEI_LN_SBI_TX_CLK_MASTER_V(f) \
        WUFEI_VALUE(f, WUFEI_LN_SBI_TX_CLK_MASTER_S, WUFEI_LN_SBI_TX_CLK_MASTER_W)
#define WUFEI_LN_SBI_TX_CLK_MASTER_M \
        WUFEI_MASK(WUFEI_LN_SBI_TX_CLK_MASTER_S, WUFEI_LN_SBI_TX_CLK_MASTER_W)

#define WUFEI_LN_SBI_TX_FRAMED_S (2)
#define WUFEI_LN_SBI_TX_FRAMED_W (1)
#define WUFEI_LN_SBI_TX_FRAMED_F(v) WUFEI_FIELD(v, WUFEI_LN_SBI_TX_FRAMED_S)
#define WUFEI_LN_SBI_TX_FRAMED_V(f) \
        WUFEI_VALUE(f, WUFEI_LN_SBI_TX_FRAMED_S, WUFEI_LN_SBI_TX_FRAMED_W)
#define WUFEI_LN_SBI_TX_FRAMED_M \
        WUFEI_MASK(WUFEI_LN_SBI_TX_FRAMED_S, WUFEI_LN_SBI_TX_FRAMED_W)

#define WUFEI_LN_SBI_TX_MF_MODE_S (3)
#define WUFEI_LN_SBI_TX_MF_MODE_W (2)
#define WUFEI_LN_SBI_TX_MF_MODE_F(v) WUFEI_FIELD(v, WUFEI_LN_SBI_TX_MF_MODE_S)
#define WUFEI_LN_SBI_TX_MF_MODE_V(f) \
        WUFEI_VALUE(f, WUFEI_LN_SBI_TX_MF_MODE_S, WUFEI_LN_SBI_TX_MF_MODE_W)
#define WUFEI_LN_SBI_TX_MF_MODE_M \
        WUFEI_MASK(WUFEI_LN_SBI_TX_MF_MODE_S, WUFEI_LN_SBI_TX_MF_MODE_W)

#define WUFEI_LN_SBI_TX_T1oE1_MODE_S (5)
#define WUFEI_LN_SBI_TX_T1oE1_MODE_W (1)
#define WUFEI_LN_SBI_TX_T1oE1_MODE_F(v) WUFEI_FIELD(v, WUFEI_LN_SBI_TX_T1oE1_MODE_S)
#define WUFEI_LN_SBI_TX_T1oE1_MODE_V(f) \
        WUFEI_VALUE(f, WUFEI_LN_SBI_TX_T1oE1_MODE_S, WUFEI_LN_SBI_TX_T1oE1_MODE_W)
#define WUFEI_LN_SBI_TX_T1oE1_MODE_M \
        WUFEI_MASK(WUFEI_LN_SBI_TX_T1oE1_MODE_S, WUFEI_LN_SBI_TX_T1oE1_MODE_W)

#define WUFEI_LN_SBI_TX_E3_ATM_G751_MODE_S (5)
#define WUFEI_LN_SBI_TX_E3_ATM_G751_MODE_W (1)
#define WUFEI_LN_SBI_TX_E3_ATM_G751_MODE_F(v) WUFEI_FIELD(v, WUFEI_LN_SBI_TX_E3_ATM_G751_MODE_S)
#define WUFEI_LN_SBI_TX_E3_ATM_G751_MODE_V(f) \
        WUFEI_VALUE(f, WUFEI_LN_SBI_TX_E3_ATM_G751_MODE_S, WUFEI_LN_SBI_TX_E3_ATM_G751_MODE_W)
#define WUFEI_LN_SBI_TX_E3_ATM_G751_MODE_M \
        WUFEI_MASK(WUFEI_LN_SBI_TX_E3_ATM_G751_MODE_S, WUFEI_LN_SBI_TX_E3_ATM_G751_MODE_W)

#define WUFEI_LN_SBI_TX_CLKT_SEL_S 6
#define WUFEI_LN_SBI_TX_CLKT_SEL_W 10
#define WUFEI_LN_SBI_TX_CLKT_SEL_F(v) WUFEI_FIELD(v, WUFEI_LN_SBI_TX_CLKT_SEL_S)
#define WUFEI_LN_SBI_TX_CLKT_SEL_V(f) \
        WUFEI_VALUE(f, WUFEI_LN_SBI_TX_CLKT_SEL_S, WUFEI_LN_SBI_TX_CLKT_SEL_W)
#define WUFEI_LN_SBI_TX_CLKT_SEL_M \
        WUFEI_MASK(WUFEI_LN_SBI_TX_CLKT_SEL_S, WUFEI_LN_SBI_TX_CLKT_SEL_W)

#define WUFEI_SBI_RAM_CHECK_ALIVE_MAX_SAMPLE 10

typedef struct
{
   WP_U16 mode;
}WUFEI_hw_sbi_mode_ram;

typedef struct
{
   WP_U16 dummy;
}WUFEI_hw_sbi_tx_state_ram;

typedef struct
{
   WP_U16 dummy0;
   WP_U16 dummy1;
}WUFEI_hw_sbi_rx_state_ram;

typedef struct
{
   WP_U16 cas_ram[WUFEI_SBI_CAS_RAM_ENTRIES];
}WUFEI_hw_sbi_cas_ram;

/*********************************************
 *            ALM
 ********************************************/
#define WUFEI_LN_SBI_RX_ALM_0_S (0)
#define WUFEI_LN_SBI_RX_ALM_0_W (1)
#define WUFEI_LN_SBI_RX_ALM_0_F(v) \
        WUFEI_FIELD(v, WUFEI_LN_SBI_RX_ALM_0_S)
#define WUFEI_LN_SBI_RX_ALM_0_V(f) \
        WUFEI_VALUE(f, WUFEI_LN_SBI_RX_ALM_0_S, WUFEI_LN_SBI_RX_ALM_0_W)
#define WUFEI_LN_SBI_RX_ALM_0_M \
        WUFEI_MASK(WUFEI_LN_SBI_RX_ALM_0_S, WUFEI_LN_SBI_RX_ALM_0_W)

#define WUFEI_LN_SBI_RX_ALM_1_S (4)
#define WUFEI_LN_SBI_RX_ALM_1_W (1)
#define WUFEI_LN_SBI_RX_ALM_1_F(v) \
        WUFEI_FIELD(v, WUFEI_LN_SBI_RX_ALM_1_S)
#define WUFEI_LN_SBI_RX_ALM_1_V(f) \
        WUFEI_VALUE(f, WUFEI_LN_SBI_RX_ALM_1_S, WUFEI_LN_SBI_RX_ALM_1_W)
#define WUFEI_LN_SBI_RX_ALM_1_M \
        WUFEI_MASK(WUFEI_LN_SBI_RX_ALM_1_S, WUFEI_LN_SBI_RX_ALM_1_W)

#define WUFEI_LN_SBI_RX_ALM_2_S (8)
#define WUFEI_LN_SBI_RX_ALM_2_W (1)
#define WUFEI_LN_SBI_RX_ALM_2_F(v) \
        WUFEI_FIELD(v, WUFEI_LN_SBI_RX_ALM_2_S)
#define WUFEI_LN_SBI_RX_ALM_2_V(f) \
        WUFEI_VALUE(f, WUFEI_LN_SBI_RX_ALM_2_S, WUFEI_LN_SBI_RX_ALM_2_W)
#define WUFEI_LN_SBI_RX_ALM_2_M \
        WUFEI_MASK(WUFEI_LN_SBI_RX_ALM_2_S, WUFEI_LN_SBI_RX_ALM_2_W)

#define WUFEI_LN_SBI_RX_ALM_3_S (12)
#define WUFEI_LN_SBI_RX_ALM_3_W (1)
#define WUFEI_LN_SBI_RX_ALM_3_F(v) \
        WUFEI_FIELD(v, WUFEI_LN_SBI_RX_ALM_3_S)
#define WUFEI_LN_SBI_RX_ALM_3_V(f) \
        WUFEI_VALUE(f, WUFEI_LN_SBI_RX_ALM_3_S, WUFEI_LN_SBI_RX_ALM_3_W)
#define WUFEI_LN_SBI_RX_ALM_3_M \
        WUFEI_MASK(WUFEI_LN_SBI_RX_ALM_3_S, WUFEI_LN_SBI_RX_ALM_3_W)

/*********************************************
 *            V5 sync loss
 ********************************************/
#define WUFEI_LN_SBI_RX_V5_SYNC_0_S (1)
#define WUFEI_LN_SBI_RX_V5_SYNC_0_W (1)
#define WUFEI_LN_SBI_RX_V5_SYNC_0_F(v) \
        WUFEI_FIELD(v, WUFEI_LN_SBI_RX_V5_SYNC_0_S)
#define WUFEI_LN_SBI_RX_V5_SYNC_0_V(f) \
        WUFEI_VALUE(f, WUFEI_LN_SBI_RX_V5_SYNC_0_S, WUFEI_LN_SBI_RX_V5_SYNC_0_W)
#define WUFEI_LN_SBI_RX_V5_SYNC_0_M \
        WUFEI_MASK(WUFEI_LN_SBI_RX_V5_SYNC_0_S, WUFEI_LN_SBI_RX_V5_SYNC_0_W)

#define WUFEI_LN_SBI_RX_V5_SYNC_1_S (5)
#define WUFEI_LN_SBI_RX_V5_SYNC_1_W (1)
#define WUFEI_LN_SBI_RX_V5_SYNC_1_F(v) \
        WUFEI_FIELD(v, WUFEI_LN_SBI_RX_V5_SYNC_1_S)
#define WUFEI_LN_SBI_RX_V5_SYNC_1_V(f) \
        WUFEI_VALUE(f, WUFEI_LN_SBI_RX_V5_SYNC_1_S, WUFEI_LN_SBI_RX_V5_SYNC_1_W)
#define WUFEI_LN_SBI_RX_V5_SYNC_1_M \
        WUFEI_MASK(WUFEI_LN_SBI_RX_V5_SYNC_1_S, WUFEI_LN_SBI_RX_V5_SYNC_1_W)

#define WUFEI_LN_SBI_RX_V5_SYNC_2_S (9)
#define WUFEI_LN_SBI_RX_V5_SYNC_2_W (1)
#define WUFEI_LN_SBI_RX_V5_SYNC_2_F(v) \
        WUFEI_FIELD(v, WUFEI_LN_SBI_RX_V5_SYNC_2_S)
#define WUFEI_LN_SBI_RX_V5_SYNC_2_V(f) \
        WUFEI_VALUE(f, WUFEI_LN_SBI_RX_V5_SYNC_2_S, WUFEI_LN_SBI_RX_V5_SYNC_2_W)
#define WUFEI_LN_SBI_RX_V5_SYNC_2_M \
        WUFEI_MASK(WUFEI_LN_SBI_RX_V5_SYNC_2_S, WUFEI_LN_SBI_RX_V5_SYNC_2_W)

#define WUFEI_LN_SBI_RX_V5_SYNC_3_S (13)
#define WUFEI_LN_SBI_RX_V5_SYNC_3_W (1)
#define WUFEI_LN_SBI_RX_V5_SYNC_3_F(v) \
        WUFEI_FIELD(v, WUFEI_LN_SBI_RX_V5_SYNC_3_S)
#define WUFEI_LN_SBI_RX_V5_SYNC_3_V(f) \
        WUFEI_VALUE(f, WUFEI_LN_SBI_RX_V5_SYNC_3_S, WUFEI_LN_SBI_RX_V5_SYNC_3_W)
#define WUFEI_LN_SBI_RX_V5_SYNC_3_M \
        WUFEI_MASK(WUFEI_LN_SBI_RX_V5_SYNC_3_S, WUFEI_LN_SBI_RX_V5_SYNC_3_W)

typedef struct
{
   WP_U16 line_0_3;
   WP_U16 line_4_7;
   WP_U16 line_8_11;
   WP_U16 line_12_15;
   WP_U16 line_16_19;
   WP_U16 line_20_23;
   WP_U16 line_24_27;
   WP_U16 line_28_31;
}WUFEI_hw_sbi_alm_v5;

typedef enum
{
   WUFEI_LN_SBI_MF_NONE  = 0,
   WUFEI_LN_SBI_MF_CAS   = 1,
   WUFEI_LN_SBI_MF_CRC   = 2
}WUFEI_line_mf_mode;

typedef enum
{
    WUFEI_SBI_T1         = 0, /* define 24 timeslot per frame numbered 1 to 24*/
    WUFEI_SBI_E1         = 1, /* define 32 timeslot per frame numbered 0 to 31*/
    WUFEI_SBI_DS3        = 2,
    WUFEI_SBI_E3         = 3
}WUFEI_spe_trnsfr_type;

enum sbi_state_mode
{
   WUFEI_SBI_DISABLE = 0,
   WUFEI_SBI_ENABLE  = 1
};

/* convert from WUFE_line_transf_type to WUFEI_spe_trnsfr_type*/
#define WUFEI_SBI_TRIBUTARY_TYPE(ap)  ((ap) - (WUFE_SBI_TYPE_T1))
#define WUFEI_SYS_SBI_RX(spe) (1<<(spe+1))
#define WUFEI_SYS_SBI_TX(spe) (1<<(spe+13))

#define WUFEI_HW_SBI_N_SPE(core,stm1,spe) \
        spe + stm1*WUFE_MAX_N_SPE + core*WUFE_MAX_N_SPE*WUFE_MAX_N_STM1

#define WUFEI_HW_SBI_N_LINE_PER_CORE WUFEI_MAX_N_LINE_PER_SPE*WUFEI_MAX_N_SPE*WUFEI_MAX_N_STM1
#define WUFEI_HW_SBI_TRIB_RAM_LINE_ID(line_id, core_id) \
        (core_id == 0) ? line_id : (line_id - WUFEI_HW_SBI_N_LINE_PER_CORE)

#endif /* WUFEI_HW_SBI_H*/
