/*****************************************************************************
 * (C) Copyright 2000-2006, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: wufei_hw_phy.h
 *
 * Purpose: the UFE Phy types macro and structures
 *
 ****************************************************************************/
#ifndef WUFEI_HW_PHY_H
#define WUFEI_HW_PHY_H

/****************************************/
/*********  FIFO MAMAGEER ***************/
/****************************************/

typedef struct
{
      WP_U16 res1;
      WP_U16 res2;
      WP_U16 cntrl1; /* pri table index */
      WP_U16 write_bd_ptr;
      WP_U16 max_buff_buff_count;
      WP_U16 read_bd_ptr;
}
WUFEI_hw_rx_phy_fifom;

typedef struct
{
      WP_U16 cntrl1;               /* fast, pri table index, wait index wait count*/
      WP_U16 tx_start_th_max_buff; /* Strat Transmition threshold , Max num of Buff*/
      WP_U16 next_read_bd_ptr;
      WP_U16 read_bd_ptr;
      WP_U16 last_count_buff_count;/* Last Counter ,Buffer counter */
      WP_U16 write_bd_ptr;
}
WUFEI_hw_tx_phy_fifom;

/****************************************/
/************** COMMON  *****************/
/****************************************/
typedef struct
{
      WP_U16 cntrl1;
      WP_U16 cntrl2;
      WP_U16 cntrl3;
      WP_U16 cntrl4;
      WP_U16 cntrl5;
      WP_U16 cntrl6;
      WP_U16 cntrl7;
      WP_U16 cntrl8;
      WP_U16 mode;
      WP_U16 cntrl10;
}
WUFEI_hw_rx_phy_common;

typedef struct
{
      WP_U16 cntrl1;
      WP_U16 cntrl2;
      WP_U16 cntrl3;
      WP_U16 cntrl4;
      WP_U16 cntrl5;
      WP_U16 cntrl6;
      WP_U16 cntrl7;
      WP_U16 cntrl8;
      WP_U16 mode;
      WP_U16 cntrl10;
}
WUFEI_hw_tx_phy_common;

/****************************************/
/************* TRANSPARENT **************/
/****************************************/
typedef struct
{
      WP_U16 cntrl1;
      WP_U16 cntrl2;
      WP_U16 cntrl3;
      WP_U16 cntrl4;
      WP_U16 cntrl5;
      WP_U16 cntrl6;
      WP_U16 cntrl7;
      WP_U16 cntrl8;
      WP_U16 mode;
      WP_U16 clk_rec_ts;
}
WUFEI_hw_rx_phy_trans;

typedef struct
{
      WP_U16 cntrl1;
      WP_U16 cntrl2;
      WP_U16 cntrl3;
      WP_U16 cntrl4;
      WP_U16 cntrl5;
      WP_U16 cntrl6;
      WP_U16 cntrl7;
      WP_U16 cntrl8;
      WP_U16 mode;
      WP_U16 clk_rec_ts;
}
WUFEI_hw_tx_phy_trans;

/****************************************/
/**************  HDLC   *****************/
/****************************************/
typedef struct
{
      WP_U16 cntrl1;
      WP_U16 cntrl2;
      WP_U16 cntrl3;
      WP_U16 cntrl4;
      WP_U16 crc1;
      WP_U16 crc2;
      WP_U16 data1;
      WP_U16 data2;
      WP_U16 mode;
      WP_U16 res;
}
WUFEI_hw_rx_phy_hdlc;

typedef struct
{
      WP_U16 cntrl1;
      WP_U16 cntrl2;
      WP_U16 cntrl3;
      WP_U16 cntrl4;
      WP_U16 crc1;
      WP_U16 crc2;
      WP_U16 data1;
      WP_U16 data2;
      WP_U16 mode;
      WP_U16 res;
}
WUFEI_hw_tx_phy_hdlc;

/****************************************/
/**************  ATM    *****************/
/****************************************/
typedef struct
{
      WP_U16 cntrl1;
      WP_U16 cntrl2;
      WP_U16 data1;
      WP_U16 data2;
      WP_U16 data3;
      WP_U16 scrambeld1;
      WP_U16 scrambeld2;
      WP_U16 scrambeld3;
      WP_U16 mode;
      WP_U16 alfa_delta;
}
WUFEI_hw_rx_phy_atm;

typedef struct
{
      WP_U16 res1;
      WP_U16 res2;
      WP_U16 cntrl1;
      WP_U16 data1;
      WP_U16 data2;
      WP_U16 cntrl2;
      WP_U16 scrambeld1;
      WP_U16 scrambeld2;
      WP_U16 mode;
      WP_U16 res3;
}
WUFEI_hw_tx_phy_atm;

/****************************************/
/**************  POS    *****************/
/****************************************/
typedef struct
{
      WP_U16 cntrl1;
      WP_U16 cntrl2;
      WP_U16 cntrl3;
      WP_U16 cntrl4;
      WP_U16 crc1;
      WP_U16 crc2;
      WP_U16 data1;
      WP_U16 data2;
      WP_U16 mode;
      WP_U16 res;
}
WUFEI_hw_rx_phy_pos;

typedef struct
{
      WP_U16 cntrl1;
      WP_U16 cntrl2;
      WP_U16 cntrl3;
      WP_U16 cntrl4;
      WP_U16 crc1;
      WP_U16 crc2;
      WP_U16 data1;
      WP_U16 data2;
      WP_U16 mode;
      WP_U16 res;
}
WUFEI_hw_tx_phy_pos;

typedef struct
{
      union
      {
            WUFEI_hw_rx_phy_common common;
            WUFEI_hw_rx_phy_trans trans;
            WUFEI_hw_rx_phy_hdlc hdlc;
            WUFEI_hw_rx_phy_atm  atm;
            WUFEI_hw_rx_phy_pos  pos;
      }type ;
      WUFEI_hw_rx_phy_fifom fifom;
}
WUFEI_rspt;

typedef struct
{
      union
      {
            WUFEI_hw_tx_phy_common common;
            WUFEI_hw_tx_phy_trans trans;
            WUFEI_hw_tx_phy_hdlc hdlc;
            WUFEI_hw_tx_phy_atm atm;
            WUFEI_hw_tx_phy_pos pos;
      }type ;
      WUFEI_hw_tx_phy_fifom fifom;
}
WUFEI_tspt;

enum phy_mode_type
{
   WUFE_PHY_MODE_HDLC=0,
   WUFE_PHY_MODE_ATM =1,
   WUFE_PHY_MODE_TRANS =2
};

/********************************************************************/
/***********************     PHY RSPT  FIFO MANAGER *****************/
/********************************************************************/

#define WUFEI_PHY_FIFOM_RX_PRI_S (0)
#define WUFEI_PHY_FIFOM_RX_PRI_W (2)
#define WUFEI_PHY_FIFOM_RX_PRI_F(v) WUFEI_FIELD(v, WUFEI_PHY_FIFOM_RX_PRI_S)
#define WUFEI_PHY_FIFOM_RX_PRI_V(f) \
        WUFEI_VALUE(f, WUFEI_PHY_FIFOM_RX_PRI_S, WUFEI_PHY_FIFOM_RX_PRI_W)
#define WUFEI_PHY_FIFOM_RX_PRI_M \
        WUFEI_MASK(WUFEI_PHY_FIFOM_RX_PRI_S, WUFEI_PHY_FIFOM_RX_PRI_W)

#define WUFEI_PHY_FIFOM_RX_BUFF_COUNT_S (0)
#define WUFEI_PHY_FIFOM_RX_BUFF_COUNT_W (8)
#define WUFEI_PHY_FIFOM_RX_BUFF_COUNT_F(v) WUFEI_FIELD(v, WUFEI_PHY_FIFOM_RX_BUFF_COUNT_S)
#define WUFEI_PHY_FIFOM_RX_BUFF_COUNT_V(f) \
        WUFEI_VALUE(f, WUFEI_PHY_FIFOM_RX_BUFF_COUNT_S, WUFEI_PHY_FIFOM_RX_BUFF_COUNT_W)
#define WUFEI_PHY_FIFOM_RX_BUFF_COUNT_M \
        WUFEI_MASK(WUFEI_PHY_FIFOM_RX_BUFF_COUNT_S, WUFEI_PHY_FIFOM_RX_BUFF_COUNT_W)

#define WUFEI_PHY_FIFOM_RX_MAX_BUFF_S (8)
#define WUFEI_PHY_FIFOM_RX_MAX_BUFF_W (8)
#define WUFEI_PHY_FIFOM_RX_MAX_BUFF_F(v) WUFEI_FIELD(v, WUFEI_PHY_FIFOM_RX_MAX_BUFF_S)
#define WUFEI_PHY_FIFOM_RX_MAX_BUFF_V(f) \
        WUFEI_VALUE(f, WUFEI_PHY_FIFOM_RX_MAX_BUFF_S, WUFEI_PHY_FIFOM_RX_MAX_BUFF_W)
#define WUFEI_PHY_FIFOM_RX_MAX_BUFF_M \
        WUFEI_MASK(WUFEI_PHY_FIFOM_RX_MAX_BUFF_S, WUFEI_PHY_FIFOM_RX_MAX_BUFF_W)

/********************************************************************/
/***********************     PHY TSPT  FIFO MANAGER *****************/
/********************************************************************/
#define WUFEI_PHY_FIFOM_TX_WAIT_COUNT_S (0)
#define WUFEI_PHY_FIFOM_TX_WAIT_COUNT_W (8)
#define WUFEI_PHY_FIFOM_TX_WAIT_COUNT_F(v) WUFEI_FIELD(v, WUFEI_PHY_FIFOM_TX_WAIT_COUNT_S)
#define WUFEI_PHY_FIFOM_TX_WAIT_COUNT_V(f) \
        WUFEI_VALUE(f, WUFEI_PHY_FIFOM_TX_WAIT_COUNT_S, WUFEI_PHY_FIFOM_TX_WAIT_COUNT_W)
#define WUFEI_PHY_FIFOM_TX_WAIT_COUNT_M \
        WUFEI_MASK(WUFEI_PHY_FIFOM_TX_WAIT_COUNT_S, WUFEI_PHY_FIFOM_TX_WAIT_COUNT_W)

#define WUFEI_PHY_FIFOM_TX_WAIT_IN_S (8)
#define WUFEI_PHY_FIFOM_TX_WAIT_IN_W (2)
#define WUFEI_PHY_FIFOM_TX_WAIT_IN_F(v) WUFEI_FIELD(v, WUFEI_PHY_FIFOM_TX_WAIT_IN_S)
#define WUFEI_PHY_FIFOM_TX_WAIT_IN_V(f) \
        WUFEI_VALUE(f, WUFEI_PHY_FIFOM_TX_WAIT_IN_S, WUFEI_PHY_FIFOM_TX_WAIT_IN_W)
#define WUFEI_PHY_FIFOM_TX_WAIT_IN_M \
        WUFEI_MASK(WUFEI_PHY_FIFOM_TX_WAIT_IN_S, WUFEI_PHY_FIFOM_TX_WAIT_IN_W)

#define WUFEI_PHY_FIFOM_TX_PRI_S (10)
#define WUFEI_PHY_FIFOM_TX_PRI_W (2)
#define WUFEI_PHY_FIFOM_TX_PRI_F(v) WUFEI_FIELD(v, WUFEI_PHY_FIFOM_TX_PRI_S)
#define WUFEI_PHY_FIFOM_TX_PRI_V(f) \
        WUFEI_VALUE(f, WUFEI_PHY_FIFOM_TX_PRI_S, WUFEI_PHY_FIFOM_TX_PRI_W)
#define WUFEI_PHY_FIFOM_TX_PRI_M \
        WUFEI_MASK(WUFEI_PHY_FIFOM_TX_PRI_S, WUFEI_PHY_FIFOM_TX_PRI_W)

#define WUFEI_PHY_FIFOM_TX_FAST_S (12)
#define WUFEI_PHY_FIFOM_TX_FAST_W (1)
#define WUFEI_PHY_FIFOM_TX_FAST_F(v) WUFEI_FIELD(v, WUFEI_PHY_FIFOM_TX_FAST_S)
#define WUFEI_PHY_FIFOM_TX_FAST_V(f) \
        WUFEI_VALUE(f, WUFEI_PHY_FIFOM_TX_FAST_S, WUFEI_PHY_FIFOM_TX_FAST_W)
#define WUFEI_PHY_FIFOM_TX_FAST_M \
        WUFEI_MASK(WUFEI_PHY_FIFOM_TX_FAST_S, WUFEI_PHY_FIFOM_TX_FAST_W)

#define WUFEI_PHY_FIFOM_TX_VALID_BD_S (15)
#define WUFEI_PHY_FIFOM_TX_VALID_BD_W (1)
#define WUFEI_PHY_FIFOM_TX_VALID_BD_F(v) WUFEI_FIELD(v, WUFEI_PHY_FIFOM_TX_VALID_BD_S)
#define WUFEI_PHY_FIFOM_TX_VALID_BD_V(f) \
        WUFEI_VALUE(f, WUFEI_PHY_FIFOM_TX_VALID_BD_S, WUFEI_PHY_FIFOM_TX_VALID_BD_W)
#define WUFEI_PHY_FIFOM_TX_VALID_BD_M \
        WUFEI_MASK(WUFEI_PHY_FIFOM_TX_VALID_BD_S, WUFEI_PHY_FIFOM_TX_VALID_BD_W)

#define WUFEI_PHY_FIFOM_TX_MAX_BUFF_S (0)
#define WUFEI_PHY_FIFOM_TX_MAX_BUFF_W (8)
#define WUFEI_PHY_FIFOM_TX_MAX_BUFF_F(v) WUFEI_FIELD(v, WUFEI_PHY_FIFOM_TX_MAX_BUFF_S)
#define WUFEI_PHY_FIFOM_TX_MAX_BUFF_V(f) \
        WUFEI_VALUE(f, WUFEI_PHY_FIFOM_TX_MAX_BUFF_S, WUFEI_PHY_FIFOM_TX_MAX_BUFF_W)
#define WUFEI_PHY_FIFOM_TX_MAX_BUFF_M \
        WUFEI_MASK(WUFEI_PHY_FIFOM_TX_MAX_BUFF_S, WUFEI_PHY_FIFOM_TX_MAX_BUFF_W)

#define WUFEI_PHY_FIFOM_TX_BUFF_TH_S (8)
#define WUFEI_PHY_FIFOM_TX_BUFF_TH_W (8)
#define WUFEI_PHY_FIFOM_TX_BUFF_TH_F(v) WUFEI_FIELD(v, WUFEI_PHY_FIFOM_TX_BUFF_TH_S)
#define WUFEI_PHY_FIFOM_TX_BUFF_TH_V(f) \
        WUFEI_VALUE(f, WUFEI_PHY_FIFOM_TX_BUFF_TH_S, WUFEI_PHY_FIFOM_TX_BUFF_TH_W)
#define WUFEI_PHY_FIFOM_TX_BUFF_TH_M \
        WUFEI_MASK(WUFEI_PHY_FIFOM_TX_BUFF_TH_S, WUFEI_PHY_FIFOM_TX_BUFF_TH_W)

#define WUFEI_PHY_FIFOM_TX_BUFF_COUNT_S (0)
#define WUFEI_PHY_FIFOM_TX_BUFF_COUNT_W (8)
#define WUFEI_PHY_FIFOM_TX_BUFF_COUNT_F(v) WUFEI_FIELD(v, WUFEI_PHY_FIFOM_TX_BUFF_COUNT_S)
#define WUFEI_PHY_FIFOM_TX_BUFF_COUNT_V(f) \
        WUFEI_VALUE(f, WUFEI_PHY_FIFOM_TX_BUFF_COUNT_S, WUFEI_PHY_FIFOM_TX_BUFF_COUNT_W)
#define WUFEI_PHY_FIFOM_TX_BUFF_COUNT_M \
        WUFEI_MASK(WUFEI_PHY_FIFOM_TX_BUFF_COUNT_S, WUFEI_PHY_FIFOM_TX_BUFF_COUNT_W)

#define WUFEI_PHY_FIFOM_TX_LAST_COUNT_S (8)
#define WUFEI_PHY_FIFOM_TX_LAST_COUNT_W (8)
#define WUFEI_PHY_FIFOM_TX_LAST_COUNT_F(v) WUFEI_FIELD(v, WUFEI_PHY_FIFOM_TX_LAST_COUNT_S)
#define WUFEI_PHY_FIFOM_TX_LAST_COUNT_V(f) \
        WUFEI_VALUE(f, WUFEI_PHY_FIFOM_TX_LAST_COUNT_S, WUFEI_PHY_FIFOM_TX_LAST_COUNT_W)
#define WUFEI_PHY_FIFOM_TX_LAST_COUNT_M \
        WUFEI_MASK(WUFEI_PHY_FIFOM_TX_LAST_COUNT_S, WUFEI_PHY_FIFOM_TX_LAST_COUNT_W)

#define WUFEI_PHY_RX_MODE_TYPE_S (13)
#define WUFEI_PHY_RX_MODE_TYPE_W (3)
#define WUFEI_PHY_RX_MODE_TYPE_F(v) WUFEI_FIELD(v, WUFEI_PHY_RX_MODE_TYPE_S)
#define WUFEI_PHY_RX_MODE_TYPE_V(f) \
        WUFEI_VALUE(f, WUFEI_PHY_RX_MODE_TYPE_S, WUFEI_PHY_RX_MODE_TYPE_W)
#define WUFEI_PHY_RX_MODE_TYPE_M \
        WUFEI_MASK(WUFEI_PHY_RX_MODE_TYPE_S, WUFEI_PHY_RX_MODE_TYPE_W)

#define WUFEI_PHY_TX_MODE_TYPE_S (13)
#define WUFEI_PHY_TX_MODE_TYPE_W (3)
#define WUFEI_PHY_TX_MODE_TYPE_F(v) WUFEI_FIELD(v, WUFEI_PHY_TX_MODE_TYPE_S)
#define WUFEI_PHY_TX_MODE_TYPE_V(f) \
        WUFEI_VALUE(f, WUFEI_PHY_TX_MODE_TYPE_S, WUFEI_PHY_TX_MODE_TYPE_W)
#define WUFEI_PHY_TX_MODE_TYPE_M \
        WUFEI_MASK(WUFEI_PHY_ATM_TX_MODE_TYPE_S, WUFEI_PHY_TX_MODE_TYPE_W)

/********************************************************************/
/***********************     PHY RSPT  ATM          *****************/
/********************************************************************/
#define WUFEI_PHY_ATM_RX_INIT_CNTRL1   WUFEI_PHY_ATM_RX_CNTRL1_SHIFT_COUNTER_F(6)

#define WUFEI_PHY_ATM_RX_MODE_ENABLE_S (12)
#define WUFEI_PHY_ATM_RX_MODE_ENABLE_W (1)
#define WUFEI_PHY_ATM_RX_MODE_ENABLE_F(v) WUFEI_FIELD(v, WUFEI_PHY_ATM_RX_MODE_ENABLE_S)
#define WUFEI_PHY_ATM_RX_MODE_ENABLE_V(f) \
        WUFEI_VALUE(f, WUFEI_PHY_ATM_RX_MODE_ENABLE_S, WUFEI_PHY_ATM_RX_MODE_ENABLE_W)
#define WUFEI_PHY_ATM_RX_MODE_ENABLE_M \
        WUFEI_MASK(WUFEI_PHY_ATM_RX_MODE_ENABLE_S, WUFEI_PHY_ATM_RX_MODE_ENABLE_W)

#define WUFEI_PHY_ATM_RX_MODE_DESCRAMBLER_S (11)
#define WUFEI_PHY_ATM_RX_MODE_DESCRAMBLER_W (1)
#define WUFEI_PHY_ATM_RX_MODE_DESCRAMBLER_F(v) WUFEI_FIELD(v, WUFEI_PHY_ATM_RX_MODE_DESCRAMBLER_S)
#define WUFEI_PHY_ATM_RX_MODE_DESCRAMBLER_V(f) \
        WUFEI_VALUE(f, WUFEI_PHY_ATM_RX_MODE_DESCRAMBLER_S, WUFEI_PHY_ATM_RX_MODE_DESCRAMBLER_W)
#define WUFEI_PHY_ATM_RX_MODE_DESCRAMBLER_M \
        WUFEI_MASK(WUFEI_PHY_ATM_RX_MODE_DESCRAMBLER_S, WUFEI_PHY_ATM_RX_MODE_DESCRAMBLER_W)

#define WUFEI_PHY_ATM_RX_MODE_COSET_S (10)
#define WUFEI_PHY_ATM_RX_MODE_COSET_W (1)
#define WUFEI_PHY_ATM_RX_MODE_COSET_F(v) WUFEI_FIELD(v, WUFEI_PHY_ATM_RX_MODE_COSET_S)
#define WUFEI_PHY_ATM_RX_MODE_COSET_V(f) \
        WUFEI_VALUE(f, WUFEI_PHY_ATM_RX_MODE_COSET_S, WUFEI_PHY_ATM_RX_MODE_COSET_W)
#define WUFEI_PHY_ATM_RX_MODE_COSET_M \
        WUFEI_MASK(WUFEI_PHY_ATM_RX_MODE_COSET_S, WUFEI_PHY_ATM_RX_MODE_COSET_W)

#define WUFEI_PHY_ATM_RX_MODE_IDLE_C_DISCARD_S (9)
#define WUFEI_PHY_ATM_RX_MODE_IDLE_C_DISCARD_W (1)
#define WUFEI_PHY_ATM_RX_MODE_IDLE_C_DISCARD_F(v) \
        WUFEI_FIELD(v, WUFEI_PHY_ATM_RX_MODE_IDLE_C_DISCARD_S)
#define WUFEI_PHY_ATM_RX_MODE_IDLE_C_DISCARD_V(f) \
        WUFEI_VALUE(f,WUFEI_PHY_ATM_RX_MODE_IDLE_C_DISCARD_S,\
                    WUFEI_PHY_ATM_RX_MODE_IDLE_C_DISCARD_W)
#define WUFEI_PHY_ATM_RX_MODE_IDLE_C_DISCARD_M \
        WUFEI_MASK(WUFEI_PHY_ATM_RX_MODE_IDLE_C_DISCARD_S, \
                   WUFEI_PHY_ATM_RX_MODE_IDLE_C_DISCARD_W)

#define WUFEI_PHY_ATM_RX_MODE_UNSIGN_C_DISCARD_S (8)
#define WUFEI_PHY_ATM_RX_MODE_UNSIGN_C_DISCARD_W (1)
#define WUFEI_PHY_ATM_RX_MODE_UNSIGN_C_DISCARD_F(v) \
        WUFEI_FIELD(v, WUFEI_PHY_ATM_RX_MODE_UNSIGN_C_DISCARD_S)
#define WUFEI_PHY_ATM_RX_MODE_UNSIGN_C_DISCARD_V(f) \
    WUFEI_VALUE(f,WUFEI_PHY_ATM_RX_MODE_UNSIGN_C_DISCARD_S,\
                WUFEI_PHY_ATM_RX_MODE_UNSIGN_C_DISCARD_W)
#define WUFEI_PHY_ATM_RX_MODE_UNSIGN_C_DISCARD_M \
       WUFEI_MASK(WUFEI_PHY_ATM_RX_MODE_UNSIGN_C_DISCARD_S,\
                  WUFEI_PHY_ATM_RX_MODE_UNSIGN_C_DISCARD_W)

#define WUFEI_PHY_ATM_RX_MODE_HEC_ERR_DISCARD_S (7)
#define WUFEI_PHY_ATM_RX_MODE_HEC_ERR_DISCARD_W (1)
#define WUFEI_PHY_ATM_RX_MODE_HEC_ERR_DISCARD_F(v) \
        WUFEI_FIELD(v, WUFEI_PHY_ATM_RX_MODE_HEC_ERR_DISCARD_S)
#define WUFEI_PHY_ATM_RX_MODE_HEC_ERR_DISCARD_V(f) \
    WUFEI_VALUE(f, WUFEI_PHY_ATM_RX_MODE_HEC_ERR_DISCARD_S,\
                WUFEI_PHY_ATM_RX_MODE_HEC_ERR_DISCARD_W)
#define WUFEI_PHY_ATM_RX_MODE_HEC_ERR_DISCARD_M \
        WUFEI_MASK(WUFEI_PHY_ATM_RX_MODE_HEC_ERR_DISCARD_S, \
                   WUFEI_PHY_ATM_RX_MODE_HEC_ERR_DISCARD_W)

#define WUFEI_PHY_ATM_RX_MODE_HEC_CORRECTION_S (6)
#define WUFEI_PHY_ATM_RX_MODE_HEC_CORRECTION_W (1)
#define WUFEI_PHY_ATM_RX_MODE_HEC_CORRECTION_F(v) \
        WUFEI_FIELD(v, WUFEI_PHY_ATM_RX_MODE_HEC_CORRECTION_S)
#define WUFEI_PHY_ATM_RX_MODE_HEC_CORRECTION_V(f) \
      WUFEI_VALUE(f, WUFEI_PHY_ATM_RX_MODE_HEC_CORRECTION_S,\
                  WUFEI_PHY_ATM_RX_MODE_HEC_CORRECTION_W)
#define WUFEI_PHY_ATM_RX_MODE_HEC_CORRECTION_M \
        WUFEI_MASK(WUFEI_PHY_ATM_RX_MODE_HEC_CORRECTION_S, \
                   WUFEI_PHY_ATM_RX_MODE_HEC_CORRECTION_W)

#define WUFEI_PHY_ATM_RX_MODE_CELL_DELINEATION_S (4)
#define WUFEI_PHY_ATM_RX_MODE_CELL_DELINEATION_W (2)
#define WUFEI_PHY_ATM_RX_MODE_CELL_DELINEATION_F(v) \
        WUFEI_FIELD(v, WUFEI_PHY_ATM_RX_MODE_CELL_DELINEATION_S)
#define WUFEI_PHY_ATM_RX_MODE_CELL_DELINEATION_V(f) \
    WUFEI_VALUE(f,WUFEI_PHY_ATM_RX_MODE_CELL_DELINEATION_S,\
                WUFEI_PHY_ATM_RX_MODE_CELL_DELINEATION_W)
#define WUFEI_PHY_ATM_RX_MODE_CELL_DELINEATION_M \
       WUFEI_MASK(WUFEI_PHY_ATM_RX_MODE_CELL_DELINEATION_S,\
                  WUFEI_PHY_ATM_RX_MODE_CELL_DELINEATION_W)

#define WUFEI_PHY_ATM_RX_ALFA_DELTA_ALFA_S (8)
#define WUFEI_PHY_ATM_RX_ALFA_DELTA_ALFA_W (4)
#define WUFEI_PHY_ATM_RX_ALFA_DELTA_ALFA_F(v) WUFEI_FIELD(v,\
                            WUFEI_PHY_ATM_RX_ALFA_DELTA_ALFA_S)
#define WUFEI_PHY_ATM_RX_ALFA_DELTA_ALFA_V(f) \
        WUFEI_VALUE(f, WUFEI_PHY_ATM_RX_ALFA_DELTA_ALFA_S, \
                    WUFEI_PHY_ATM_RX_ALFA_DELTA_ALFA_W)
#define WUFEI_PHY_ATM_RX_ALFA_DELTA_ALFA_M \
        WUFEI_MASK(WUFEI_PHY_ATM_RX_ALFA_DELTA_ALFA_S, \
                   WUFEI_PHY_ATM_RX_ALFA_DELTA_ALFA_W)

#define WUFEI_PHY_ATM_RX_ALFA_DELTA_DELTA_S (12)
#define WUFEI_PHY_ATM_RX_ALFA_DELTA_DELTA_W (4)
#define WUFEI_PHY_ATM_RX_ALFA_DELTA_DELTA_F(v) WUFEI_FIELD(v, \
                              WUFEI_PHY_ATM_RX_ALFA_DELTA_DELTA_S)
#define WUFEI_PHY_ATM_RX_ALFA_DELTA_DELTA_V(f) \
        WUFEI_VALUE(f, WUFEI_PHY_ATM_RX_ALFA_DELTA_DELTA_S, \
                    WUFEI_PHY_ATM_RX_ALFA_DELTA_DELTA_W)
#define WUFEI_PHY_ATM_RX_ALFA_DELTA_DELTA_M \
        WUFEI_MASK(WUFEI_PHY_ATM_RX_ALFA_DELTA_DELTA_S, \
                   WUFEI_PHY_ATM_RX_ALFA_DELTA_DELTA_W)

#define WUFEI_PHY_ATM_RX_CNTRL1_SHIFT_COUNTER_S (8)
#define WUFEI_PHY_ATM_RX_CNTRL1_SHIFT_COUNTER_W (3)
#define WUFEI_PHY_ATM_RX_CNTRL1_SHIFT_COUNTER_F(v) \
        WUFEI_FIELD(v, WUFEI_PHY_ATM_RX_CNTRL1_SHIFT_COUNTER_S)
#define WUFEI_PHY_ATM_RX_CNTRL1_SHIFT_COUNTER_V(f) \
    WUFEI_VALUE(f, WUFEI_PHY_ATM_RX_CNTRL1_SHIFT_COUNTER_S,\
                WUFEI_PHY_ATM_RX_CNTRL1_SHIFT_COUNTER_W)
#define WUFEI_PHY_ATM_RX_CNTRL1_SHIFT_COUNTER_M \
        WUFEI_MASK(WUFEI_PHY_ATM_RX_CNTRL1_SHIFT_COUNTER_S, \
                   WUFEI_PHY_ATM_RX_CNTRL1_SHIFT_COUNTER_W)


/**************************************************************/
/****************     PHY  ATM  STATE        *****************/
/************************************************************/

#define WUFEI_PHY_ATM_PORT_INSYNC_STATE_S (7)
#define WUFEI_PHY_ATM_PORT_INSYNC_STATE_W (1)
#define WUFEI_PHY_ATM_PORT_INSYNC_STATE_F(v) WUFEI_FIELD(v, \
                            WUFEI_PHY_ATM_PORT_INSYNC_STATE_S)
#define WUFEI_PHY_ATM_PORT_INSYNC_STATE_V(f) \
        WUFEI_VALUE(f, WUFEI_PHY_ATM_PORT_INSYNC_STATE_S,\
                    WUFEI_PHY_ATM_PORT_INSYNC_STATE_W)
#define WUFEI_PHY_ATM_PORT_INSYNC_STATE_M \
        WUFEI_MASK(WUFEI_PHY_ATM_PORT_INSYNC_STATE_S, \
                   WUFEI_PHY_ATM_PORT_INSYNC_STATE_W)


#define WUFEI_PHY_ATM_PORT_OUTSYNC_STATE_S (6)
#define WUFEI_PHY_ATM_PORT_OUTSYNC_STATE_W (1)
#define WUFEI_PHY_ATM_PORT_OUTSYNC_STATE_F(v) WUFEI_FIELD(v, \
                            WUFEI_PHY_ATM_PORT_OUTSYNC_STATE_S)
#define WUFEI_PHY_ATM_PORT_OUTSYNC_STATE_V(f) \
        WUFEI_VALUE(f, WUFEI_PHY_ATM_PORT_OUTSYNC_STATE_S,\
                    WUFEI_PHY_ATM_PORT_OUTSYNC_STATE_W)
#define WUFEI_PHY_ATM_PORT_OUTSYNC_STATE_M \
        WUFEI_MASK(WUFEI_PHY_ATM_PORT_OUTSYNC_STATE_S, \
                   WUFEI_PHY_ATM_PORT_OUTSYNC_STATE_W)

/********************************************************************/
/***********************     PHY TSPT  ATM          *****************/
/********************************************************************/
#define WUFEI_PHY_ATM_TX_INIT_CNTRL1  0xc

#define WUFEI_PHY_ATM_TX_MODE_ENABLE_S (12)
#define WUFEI_PHY_ATM_TX_MODE_ENABLE_W (1)
#define WUFEI_PHY_ATM_TX_MODE_ENABLE_F(v) WUFEI_FIELD(v,\
                           WUFEI_PHY_ATM_TX_MODE_ENABLE_S)
#define WUFEI_PHY_ATM_TX_MODE_ENABLE_V(f) \
        WUFEI_VALUE(f, WUFEI_PHY_ATM_TX_MODE_ENABLE_S, \
                    WUFEI_PHY_ATM_TX_MODE_ENABLE_W)
#define WUFEI_PHY_ATM_TX_MODE_ENABLE_M \
        WUFEI_MASK(WUFEI_PHY_ATM_TX_MODE_ENABLE_S, \
                   WUFEI_PHY_ATM_TX_MODE_ENABLE_W)

#define WUFEI_PHY_ATM_TX_MODE_SCRAMBLER_S (11)
#define WUFEI_PHY_ATM_TX_MODE_SCRAMBLER_W (1)
#define WUFEI_PHY_ATM_TX_MODE_SCRAMBLER_F(v) WUFEI_FIELD(v,\
                            WUFEI_PHY_ATM_TX_MODE_SCRAMBLER_S)
#define WUFEI_PHY_ATM_TX_MODE_SCRAMBLER_V(f) \
        WUFEI_VALUE(f, WUFEI_PHY_ATM_TX_MODE_SCRAMBLER_S, \
                    WUFEI_PHY_ATM_TX_MODE_SCRAMBLER_W)
#define WUFEI_PHY_ATM_TX_MODE_SCRAMBLER_M \
        WUFEI_MASK(WUFEI_PHY_ATM_TX_MODE_SCRAMBLER_S, \
                   WUFEI_PHY_ATM_TX_MODE_SCRAMBLER_W)

#define WUFEI_PHY_ATM_TX_MODE_COSET_S (10)
#define WUFEI_PHY_ATM_TX_MODE_COSET_W (1)
#define WUFEI_PHY_ATM_TX_MODE_COSET_F(v) WUFEI_FIELD(v,\
                           WUFEI_PHY_ATM_TX_MODE_COSET_S)
#define WUFEI_PHY_ATM_TX_MODE_COSET_V(f) \
        WUFEI_VALUE(f, WUFEI_PHY_ATM_TX_MODE_COSET_S, \
                    WUFEI_PHY_ATM_TX_MODE_COSET_W)
#define WUFEI_PHY_ATM_TX_MODE_COSET_M \
        WUFEI_MASK(WUFEI_PHY_ATM_TX_MODE_COSET_S,WUFEI_PHY_ATM_TX_MODE_COSET_W)

#define WUFEI_PHY_ATM_TX_MODE_UNDERRUN_INT_S (9)
#define WUFEI_PHY_ATM_TX_MODE_UNDERRUN_INT_W (1)
#define WUFEI_PHY_ATM_TX_MODE_UNDERRUN_INT_F(v) WUFEI_FIELD(v, \
                            WUFEI_PHY_ATM_TX_MODE_UNDERRUN_INT_S)
#define WUFEI_PHY_ATM_TX_MODE_UNDERRUN_INT_V(f) \
        WUFEI_VALUE(f, WUFEI_PHY_ATM_TX_MODE_UNDERRUN_INT_S,\
                    WUFEI_PHY_ATM_TX_MODE_UNDERRUN_INT_W)
#define WUFEI_PHY_ATM_TX_MODE_UNDERRUN_INT_M \
        WUFEI_MASK(WUFEI_PHY_ATM_TX_MODE_UNDERRUN_INT_S, \
                   WUFEI_PHY_ATM_TX_MODE_UNDERRUN_INT_W)

#define WUFEI_PHY_ATM_TX_MODE_CLP_S (8)
#define WUFEI_PHY_ATM_TX_MODE_CLP_W (1)
#define WUFEI_PHY_ATM_TX_MODE_CLP_F(v) WUFEI_FIELD(v,\
                               WUFEI_PHY_ATM_TX_MODE_CLP_S)
#define WUFEI_PHY_ATM_TX_MODE_CLP_V(f) \
        WUFEI_VALUE(f, WUFEI_PHY_ATM_TX_MODE_CLP_S, \
                    WUFEI_PHY_ATM_TX_MODE_CLP_W)
#define WUFEI_PHY_ATM_TX_MODE_CLP_M \
        WUFEI_MASK(WUFEI_PHY_ATM_TX_MODE_CLP_S, \
                   WUFEI_PHY_ATM_TX_MODE_CLP_W)

/********************************************************************/
/***********************     PHY RSPT HDLC          *****************/
/********************************************************************/
#define WUFEI_PHY_HDLC_RX_INIT_CNTRL1  0x0001

#define WUFEI_PHY_HDLC_RX_MODE_CRC_SIZE_S (10)
#define WUFEI_PHY_HDLC_RX_MODE_CRC_SIZE_W (1)
#define WUFEI_PHY_HDLC_RX_MODE_CRC_SIZE_F(v) WUFEI_FIELD(v,\
                          WUFEI_PHY_HDLC_RX_MODE_CRC_SIZE_S)
#define WUFEI_PHY_HDLC_RX_MODE_CRC_SIZE_V(f) \
        WUFEI_VALUE(f, WUFEI_PHY_HDLC_RX_MODE_CRC_SIZE_S, \
                    WUFEI_PHY_HDLC_RX_MODE_CRC_SIZE_W)
#define WUFEI_PHY_HDLC_RX_MODE_CRC_SIZE_M \
        WUFEI_MASK(WUFEI_PHY_HDLC_RX_MODE_CRC_SIZE_S, \
                   WUFEI_PHY_HDLC_RX_MODE_CRC_SIZE_W)

#define WUFEI_PHY_HDLC_RX_MODE_IN_CRC_S (11)
#define WUFEI_PHY_HDLC_RX_MODE_IN_CRC_W (1)
#define WUFEI_PHY_HDLC_RX_MODE_IN_CRC_F(v) WUFEI_FIELD(v,\
                             WUFEI_PHY_HDLC_RX_MODE_IN_CRC_S)
#define WUFEI_PHY_HDLC_RX_MODE_IN_CRC_V(f) \
        WUFEI_VALUE(f, WUFEI_PHY_HDLC_RX_MODE_IN_CRC_S, \
                    WUFEI_PHY_HDLC_RX_MODE_IN_CRC_W)
#define WUFEI_PHY_HDLC_RX_MODE_IN_CRC_M \
        WUFEI_MASK(WUFEI_PHY_HDLC_RX_MODE_IN_CRC_S, \
                   WUFEI_PHY_HDLC_RX_MODE_IN_CRC_W)

/********************************************************************/
/***********************     PHY TSPT HDLC          *****************/
/********************************************************************/
#define WUFEI_PHY_HDLC_TX_MINUMFL_S (5)
#define WUFEI_PHY_HDLC_TX_MINUMFL_W (4)
#define WUFEI_PHY_HDLC_TX_MINUMFL_F(v) WUFEI_FIELD(v,\
                            WUFEI_PHY_HDLC_TX_MINUMFL_S)
#define WUFEI_PHY_HDLC_TX_MINUMFL_V(f) \
        WUFEI_VALUE(f, WUFEI_PHY_HDLC_TX_MINUMFL_S,\
                    WUFEI_PHY_HDLC_TX_MINUMFL_W)
#define WUFEI_PHY_HDLC_TX_MINUMFL_M \
        WUFEI_MASK(WUFEI_PHY_HDLC_TX_MINUMFL_S, \
                   WUFEI_PHY_HDLC_TX_MINUMFL_W)

#define WUFEI_PHY_HDLC_TX_MODE_NFBF_S (9)
#define WUFEI_PHY_HDLC_TX_MODE_NFBF_W (1)
#define WUFEI_PHY_HDLC_TX_MODE_NFBF_F(v) WUFEI_FIELD(v,\
                          WUFEI_PHY_HDLC_TX_MODE_NFBF_S)
#define WUFEI_PHY_HDLC_TX_MODE_NFBF_V(f) \
        WUFEI_VALUE(f, WUFEI_PHY_HDLC_TX_MODE_NFBF_S,\
                    WUFEI_PHY_HDLC_TX_MODE_NFBF_W)
#define WUFEI_PHY_HDLC_TX_MODE_NFBF_M \
        WUFEI_MASK(WUFEI_PHY_HDLC_TX_MODE_NFBF_S, \
                   WUFEI_PHY_HDLC_TX_MODE_NFBF_W)

#define WUFEI_PHY_HDLC_TX_MODE_CRC_SIZE_S (10)
#define WUFEI_PHY_HDLC_TX_MODE_CRC_SIZE_W (1)
#define WUFEI_PHY_HDLC_TX_MODE_CRC_SIZE_F(v) WUFEI_FIELD(v,\
                             WUFEI_PHY_HDLC_TX_MODE_CRC_SIZE_S)
#define WUFEI_PHY_HDLC_TX_MODE_CRC_SIZE_V(f) \
        WUFEI_VALUE(f, WUFEI_PHY_HDLC_TX_MODE_CRC_SIZE_S, \
                    WUFEI_PHY_HDLC_TX_MODE_CRC_SIZE_W)
#define WUFEI_PHY_HDLC_TX_MODE_CRC_SIZE_M \
        WUFEI_MASK(WUFEI_PHY_HDLC_TX_MODE_CRC_SIZE_S,\
                   WUFEI_PHY_HDLC_TX_MODE_CRC_SIZE_W)

#define WUFEI_PHY_HDLC_TX_MODE_IN_CRC_S (11)
#define WUFEI_PHY_HDLC_TX_MODE_IN_CRC_W (1)
#define WUFEI_PHY_HDLC_TX_MODE_IN_CRC_F(v) WUFEI_FIELD(v,\
                            WUFEI_PHY_HDLC_TX_MODE_IN_CRC_S)
#define WUFEI_PHY_HDLC_TX_MODE_IN_CRC_V(f) \
        WUFEI_VALUE(f, WUFEI_PHY_HDLC_TX_MODE_IN_CRC_S, \
                    WUFEI_PHY_HDLC_TX_MODE_IN_CRC_W)
#define WUFEI_PHY_HDLC_TX_MODE_IN_CRC_M \
        WUFEI_MASK(WUFEI_PHY_HDLC_TX_MODE_IN_CRC_S, \
                   WUFEI_PHY_HDLC_TX_MODE_IN_CRC_W)


#define WUFEI_PHY_HDLC_TX_MODE_UNDERRUN_INT_S (3)
#define WUFEI_PHY_HDLC_TX_MODE_UNDERRUN_INT_W (1)
#define WUFEI_PHY_HDLC_TX_MODE_UNDERRUN_INT_F(v) WUFEI_FIELD(v, \
                            WUFEI_PHY_HDLC_TX_MODE_UNDERRUN_INT_S)
#define WUFEI_PHY_HDLC_TX_MODE_UNDERRUN_INT_V(f) \
        WUFEI_VALUE(f, WUFEI_PHY_HDLC_TX_MODE_UNDERRUN_INT_S,\
                    WUFEI_PHY_HDLC_TX_MODE_UNDERRUN_INT_W)
#define WUFEI_PHY_HDLC_TX_MODE_UNDERRUN_INT_M \
        WUFEI_MASK(WUFEI_PHY_HDLC_TX_MODE_UNDERRUN_INT_S, \
                   WUFEI_PHY_HDLC_TX_MODE_UNDERRUN_INT_W)

/********************************************************************/
/***********************     PHY TSPT TRANS         *****************/
/********************************************************************/

#define WUFEI_PHY_TRANS_TX_MODE_NSPP_S (6)
#define WUFEI_PHY_TRANS_TX_MODE_NSPP_W (5)
#define WUFEI_PHY_TRANS_TX_MODE_NSPP_F(v) WUFEI_FIELD(v, \
                           WUFEI_PHY_TRANS_TX_MODE_NSPP_S)
#define WUFEI_PHY_TRANS_TX_MODE_NSPP_V(f) \
        WUFEI_VALUE(f, WUFEI_PHY_TRANS_TX_MODE_NSPP_S, \
                    WUFEI_PHY_TRANS_TX_MODE_NSPP_W)
#define WUFEI_PHY_TRANS_TX_MODE_NSPP_M \
        WUFEI_MASK(WUFEI_PHY_TRANS_TX_MODE_NSPP_S, \
                   WUFEI_PHY_TRANS_TX_MODE_NSPP_W)


#define WUFEI_PHY_TRANS_TX_MODE_UNDERRUN_INT_S (3)
#define WUFEI_PHY_TRANS_TX_MODE_UNDERRUN_INT_W (1)
#define WUFEI_PHY_TRANS_TX_MODE_UNDERRUN_INT_F(v) WUFEI_FIELD(v, \
                            WUFEI_PHY_TRANS_TX_MODE_UNDERRUN_INT_S)
#define WUFEI_PHY_TRANS_TX_MODE_UNDERRUN_INT_V(f) \
        WUFEI_VALUE(f, WUFEI_PHY_TRANS_TX_MODE_UNDERRUN_INT_S,\
                    WUFEI_PHY_TRANS_TX_MODE_UNDERRUN_INT_W)
#define WUFEI_PHY_TRANS_TX_MODE_UNDERRUN_INT_M \
        WUFEI_MASK(WUFEI_PHY_TRANS_TX_MODE_UNDERRUN_INT_S, \
                   WUFEI_PHY_TRANS_TX_MODE_UNDERRUN_INT_W)

#define WUFEI_PHY_TRANS_TX_CLK_REC_TS_EN_S (15)
#define WUFEI_PHY_TRANS_TX_CLK_REC_TS_EN_W (1)
#define WUFEI_PHY_TRANS_TX_CLK_REC_TS_EN_F(v) WUFEI_FIELD(v, \
                            WUFEI_PHY_TRANS_TX_CLK_REC_TS_EN_S)
#define WUFEI_PHY_TRANS_TX_CLK_REC_TS_EN_V(f) \
        WUFEI_VALUE(f, WUFEI_PHY_TRANS_TX_CLK_REC_TS_EN_S,\
                    WUFEI_PHY_TRANS_TX_CLK_REC_TS_EN_W)
#define WUFEI_PHY_TRANS_TX_CLK_REC_TS_EN_M \
        WUFEI_MASK(WUFEI_PHY_TRANS_TX_CLK_REC_TS_EN_S, \
                   WUFEI_PHY_TRANS_TX_CLK_REC_TS_EN_W)

#define WUFEI_PHY_TRANS_TX_CLK_REC_IF_SELECT_S (8)
#define WUFEI_PHY_TRANS_TX_CLK_REC_IF_SELECT_W (6)
#define WUFEI_PHY_TRANS_TX_CLK_REC_IF_SELECT_F(v) WUFEI_FIELD(v, \
                            WUFEI_PHY_TRANS_TX_CLK_REC_IF_SELECT_S)
#define WUFEI_PHY_TRANS_TX_CLK_REC_IF_SELECT_V(f) \
        WUFEI_VALUE(f, WUFEI_PHY_TRANS_TX_CLK_REC_IF_SELECT_S,\
                    WUFEI_PHY_TRANS_TX_CLK_REC_IF_SELECT_W)
#define WUFEI_PHY_TRANS_TX_CLK_REC_IF_SELECT_M \
        WUFEI_MASK(WUFEI_PHY_TRANS_TX_CLK_REC_IF_SELECT_S, \
                   WUFEI_PHY_TRANS_TX_CLK_REC_IF_SELECT_W)

/********************************************************************/
/***********************     PHY RSPT TRANS         *****************/
/********************************************************************/

#define WUFEI_PHY_TRANS_RX_MODE_NSPP_S (6)
#define WUFEI_PHY_TRANS_RX_MODE_NSPP_W (5)
#define WUFEI_PHY_TRANS_RX_MODE_NSPP_F(v) WUFEI_FIELD(v, \
                           WUFEI_PHY_TRANS_RX_MODE_NSPP_S)
#define WUFEI_PHY_TRANS_RX_MODE_NSPP_V(f) \
        WUFEI_VALUE(f, WUFEI_PHY_TRANS_RX_MODE_NSPP_S, \
                    WUFEI_PHY_TRANS_RX_MODE_NSPP_W)
#define WUFEI_PHY_TRANS_RX_MODE_NSPP_M \
        WUFEI_MASK(WUFEI_PHY_TRANS_RX_MODE_NSPP_S, \
                   WUFEI_PHY_TRANS_RX_MODE_NSPP_W)

#define WUFEI_PHY_TRANS_RX_MODE_DATA_U_SIZE_S (0)
#define WUFEI_PHY_TRANS_RX_MODE_DATA_U_SIZE_W (6)
#define WUFEI_PHY_TRANS_RX_MODE_DATA_U_SIZE_F(v) WUFEI_FIELD(v, \
                            WUFEI_PHY_TRANS_RX_MODE_DATA_U_SIZE_S)
#define WUFEI_PHY_TRANS_RX_MODE_DATA_U_SIZE_V(f) \
        WUFEI_VALUE(f, WUFEI_PHY_TRANS_RX_MODE_DATA_U_SIZE_S,\
                    WUFEI_PHY_TRANS_RX_MODE_DATA_U_SIZE_W)
#define WUFEI_PHY_TRANS_RX_MODE_DATA_U_SIZE_M \
        WUFEI_MASK(WUFEI_PHY_TRANS_RX_MODE_DATA_U_SIZE_S, \
                   WUFEI_PHY_TRANS_RX_MODE_DATA_U_SIZE_W)

#define WUFEI_PHY_TRANS_RX_CLK_REC_TS_EN_S (15)
#define WUFEI_PHY_TRANS_RX_CLK_REC_TS_EN_W (1)
#define WUFEI_PHY_TRANS_RX_CLK_REC_TS_EN_F(v) WUFEI_FIELD(v, \
                            WUFEI_PHY_TRANS_RX_CLK_REC_TS_EN_S)
#define WUFEI_PHY_TRANS_RX_CLK_REC_TS_EN_V(f) \
        WUFEI_VALUE(f, WUFEI_PHY_TRANS_RX_CLK_REC_TS_EN_S,\
                    WUFEI_PHY_TRANS_RX_CLK_REC_TS_EN_W)
#define WUFEI_PHY_TRANS_RX_CLK_REC_TS_EN_M \
        WUFEI_MASK(WUFEI_PHY_TRANS_RX_CLK_REC_TS_EN_S, \
                   WUFEI_PHY_TRANS_RX_CLK_REC_TS_EN_W)

#define WUFEI_PHY_TRANS_RX_CLK_REC_THRESHOLD_S (8)
#define WUFEI_PHY_TRANS_RX_CLK_REC_THRESHOLD_W (2)
#define WUFEI_PHY_TRANS_RX_CLK_REC_THRESHOLD_F(v) WUFEI_FIELD(v, \
                            WUFEI_PHY_TRANS_RX_CLK_REC_THRESHOLD_S)
#define WUFEI_PHY_TRANS_RX_CLK_REC_THRESHOLD_V(f) \
        WUFEI_VALUE(f, WUFEI_PHY_TRANS_RX_CLK_REC_THRESHOLD_S,\
                    WUFEI_PHY_TRANS_RX_CLK_REC_THRESHOLD_W)
#define WUFEI_PHY_TRANS_RX_CLK_REC_THRESHOLD_M \
        WUFEI_MASK(WUFEI_PHY_TRANS_RX_CLK_REC_THRESHOLD_S, \
                   WUFEI_PHY_TRANS_RX_CLK_REC_THRESHOLD_W)

#define WUFEI_PHY_TRANS_RX_CLK_REC_MASK_S (0)
#define WUFEI_PHY_TRANS_RX_CLK_REC_MASK_W (1)
#define WUFEI_PHY_TRANS_RX_CLK_REC_MASK_F(v) WUFEI_FIELD(v, \
                            WUFEI_PHY_TRANS_RX_CLK_REC_MASK_S)
#define WUFEI_PHY_TRANS_RX_CLK_REC_MASK_V(f) \
        WUFEI_VALUE(f, WUFEI_PHY_TRANS_RX_CLK_REC_MASK_S,\
                    WUFEI_PHY_TRANS_RX_CLK_REC_MASK_W)
#define WUFEI_PHY_TRANS_RX_CLK_REC_MASK_M \
        WUFEI_MASK(WUFEI_PHY_TRANS_RX_CLK_REC_MASK_S, \
                   WUFEI_PHY_TRANS_RX_CLK_REC_MASK_W)

#define WUFEI_PHY_TRANS_RX_CLK_REC_THRESHOLD_TEMP_S (14)
#define WUFEI_PHY_TRANS_RX_CLK_REC_THRESHOLD_TEMP_W (2)
#define WUFEI_PHY_TRANS_RX_CLK_REC_THRESHOLD_TEMP_F(v) WUFEI_FIELD(v, \
                            WUFEI_PHY_TRANS_RX_CLK_REC_THRESHOLD_TEMP_S)
#define WUFEI_PHY_TRANS_RX_CLK_REC_THRESHOLD_TEMP_V(f) \
        WUFEI_VALUE(f, WUFEI_PHY_TRANS_RX_CLK_REC_THRESHOLD_TEMP_S,\
                    WUFEI_PHY_TRANS_RX_CLK_REC_THRESHOLD_TEMP_W)
#define WUFEI_PHY_TRANS_RX_CLK_REC_THRESHOLD_TEMP_M \
        WUFEI_MASK(WUFEI_PHY_TRANS_RX_CLK_REC_THRESHOLD_TEMP_S, \
                   WUFEI_PHY_TRANS_RX_CLK_REC_THRESHOLD_TEMP_W)

#endif /* WUFEI_HW_PHY_H*/
