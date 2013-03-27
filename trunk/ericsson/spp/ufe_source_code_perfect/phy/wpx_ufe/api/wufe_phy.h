/*****************************************************************************
 * (C) Copyright 2000-2006, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: wufei_phy.h
 *
 * Purpose: Define the stuctures used by the HW
 *
 ****************************************************************************/

#ifndef WUFE_PHY_H
#define WUFE_PHY_H

typedef enum
{
   WUFE_PHY_TX_WAIT_TYPE_A = 0,
   WUFE_PHY_TX_WAIT_TYPE_B = 1,
   WUFE_PHY_TX_WAIT_TYPE_C = 2,
   WUFE_PHY_TX_WAIT_TYPE_D = 3
}WUFE_PhyTxWaittype;

typedef enum
{
   WUFE_PHY_MOD_RX_SLOT  = 1,
   WUFE_PHY_MOD_TX_SLOT  = 2,
   WUFE_PHY_MOD_RX_FIFO  = 4,
   WUFE_PHY_MOD_TX_FIFO  = 8,
   WUFE_PHY_MOD_PROTOCL  = 16,
   WUFE_PHY_MOD_PROTOCOL  = WUFE_PHY_MOD_PROTOCL
}WUFE_phy_mod;

typedef enum
{ /* defines how many DPS threads can be opened per Phy at the same time */
   WUFE_UFE_FIFO_MODE_SLOW =0, /*one DPS thread */
   WUFE_PHY_FIFO_MODE_SLOW =0, /*one DPS thread */
   WP_UFE_FIFO_MODE_SLOW = WUFE_PHY_FIFO_MODE_SLOW, /* Api backword competability */
   WUFE_UFE_FIFO_MODE_FAST =1, /*up to 3 DPS threads */
   WUFE_PHY_FIFO_MODE_FAST =1, /*up to 3 DPS threads */
   WP_UFE_FIFO_MODE_FAST =WUFE_PHY_FIFO_MODE_FAST /* Api backword competability */
}WUFE_PhyFifoFastMode;


typedef enum
{
   WUFE_UFE_PHY_INT_DISABLE =0,
   WUFE_UFE_PHY_INT_ENABLE  =1
}WUFE_PhyIntMode;


/*****************************************/
/********     HDLC CONST            *******/
/*****************************************/

typedef enum
{
   WUFE_PHY_HDLC_CRC_ENABLE   = 1,
   WUFE_PHY_HDLC_CRC_DISABLE  = 0
}WUFE_PhyHdlcCrcEnb;

typedef enum
{
   WUFE_PHY_HDLC_CRC_16 = 0,
   WUFE_PHY_HDLC_CRC_32 = 1
}WUFE_PhyHdlcCrcSize;

#define WUFE_PHY_POS_CRC_16 WUFE_PHY_HDLC_CRC_16
#define WUFE_PHY_POS_CRC_32 WUFE_PHY_HDLC_CRC_32

typedef enum
{
   WUFE_PHY_HDLC_TX_NO_FLAGS              = 1,
   WUFE_PHY_HDLC_TX_IDLE                  = WUFE_PHY_HDLC_TX_NO_FLAGS,
   WUFE_PHY_HDLC_TX_FLAGS_BETWEEN_FRAMES  = 0
}WUFE_PhyHdlcTxNfbf;

/*****************************************/
/********     POS CONST            *******/
/*****************************************/
typedef enum
{
   WUFE_PHY_POS_RX_DESCRAMBLER_DISABLE  = 0,
   WUFE_PHY_POS_RX_DESCRAMBLER_ENABLE   = 1
}WUFE_PhyPosRxDescramber;

typedef enum
{
   WUFE_PHY_POS_TX_SCRAMBLER_DISABLE  = 0,
   WUFE_PHY_POS_TX_SCRAMBLER_ENABLE   = 1
}WUFE_PhyPosTxscramber;


/*****************************************/
/********     ATM CONST            *******/
/*****************************************/

typedef enum
{
   WUFE_PHY_ATM_RX_DESCRAMBLER_DISABLE  = 0,
   WUFE_PHY_ATM_RX_DESCRAMBLER_ENABLE   = 1
}WUFE_PhyAtmRxDescramber;

typedef enum
{
   WUFE_PHY_ATM_RX_NO_COSET   = 0,
   WUFE_PHY_ATM_RX_ADD_COSET  = 1
}WUFE_PhyAtmRxCoset;

typedef enum
{
   WUFE_PHY_ATM_RX_IDLE_CELL_TRANSFER  = 0,
   WUFE_PHY_ATM_RX_IDLE_CELL_DISCARD   = 1
}WUFE_PhyAtmRxIdleCell;

typedef enum
{
   WUFE_PHY_ATM_RX_UNASSIGNED_CELL_TRANSFER  = 0,
   WUFE_PHY_ATM_RX_UNASSIGNED_CELL_DISCARD   = 1
}WUFE_PhyAtmRxUnasgndCell;

typedef enum
{
   WUFE_PHY_ATM_RX_HEC_ERR_TRANSFER = 0,
   WUFE_PHY_ATM_RX_HEC_ERR_DISCARD = 1
}WUFE_PhyAtmRxHecErr;

typedef enum
{
   WUFE_PHY_ATM_RX_HEC_CORECTION_DISABLE = 0,
   WUFE_PHY_ATM_RX_HEC_CORECTION_ENABLE = 1
}WUFE_PhyAtmRxHecCorection;

typedef enum
{
   WUFE_PHY_ATM_RX_CELL_DELINEATION_BIT_RESOLUTION     = 0, /*Relevant for ufe3 only*/
   WUFE_PHY_ATM_RX_CELL_DELINEATION_NIBBLE_RESOLUTION  = 1,
   WUFE_PHY_ATM_RX_CELL_DELINEATION_BYTE_RESOLUTION    = 2
}WUFE_PhyAtmRxCellDelineation;

typedef enum
{
   WUFE_PHY_ATM_TX_SCRAMBLER_DISABLE  = 0,
   WUFE_PHY_ATM_TX_SCRAMBLER_ENABLE   = 1
}WUFE_PhyAtmTxscramber;

typedef enum
{
   WUFE_PHY_ATM_TX_NO_COSET   = 0,
   WUFE_PHY_ATM_TX_ADD_COSET  = 1
}WUFE_PhyAtmTxCoset;

typedef enum
{
   WUFE_PHY_ATM_TX_UNASSIGNED_CELL_MODE = 0,
   WUFE_PHY_ATM_TX_IDLE_CELL_MODE   = 1
}WUFE_PhyAtmTxIdleUnassingednCell;

typedef enum
{
   WUFE_PHY_ATM_TX_UNDERRUN_INT_DISABLE = WUFE_UFE_PHY_INT_DISABLE,
   WUFE_PHY_ATM_TX_UNDERRUN_INT_ENABLE = WUFE_UFE_PHY_INT_ENABLE
}WUFE_PhyAtmTxUnderrunInt;

typedef enum
{
   WUFE_PHY_STATE_HUNT    = 0x00,
   WUFE_PHY_STATE_PRESYNC = 0x01,
   WUFE_PHY_STATE_SYNC    = 0x02 /*sync correction or sync detection*/
} WUFE_phy_state;

/*****************************************/
/********     General data stuct   *******/
/*****************************************/
typedef struct{
      WP_U16           hmvip_id;
      WP_U8            slot_arr[WUFE_N_SLOTS_E1];
      WP_U32           n_slots;
}WUFE_SlotGroup;

typedef struct
{
      WP_U8 fifo_max_size;
      WP_U8 priority_table;
      WP_U8 fast_mode;
      WP_U8 transmit_th;
      WP_U8 wait_type;
}
WUFE_TxFifo;

typedef struct
{
      WP_U8 fifo_max_size;
      WP_U8 priority_table;
      WP_U8 transmit_th;
}
WUFE_RxFifo;

typedef struct
{
      WP_U8 rx_descrambler_mode;
      WP_U8 rx_coset_mode;
      WP_U8 rx_idle_cell_discard;
      WP_U8 rx_unassigned_cell_discard;
      WP_U8 rx_hec_error_discard;
      WP_U8 rx_hec_correction_mode;
      WP_U8 rx_cell_delineation;
      WP_U8 rx_alpha;
      WP_U8 rx_delta;
      WP_U8 tx_scrambler_mode;
      WP_U8 tx_coset_mode;
      WP_U8 tx_idle_unassigned_cell_mode;
      WP_U8 tx_underrun_interrupt_mode; /*Relevant for UFE3 only*/
}
WUFE_Phy_atm;

typedef struct
{
      WP_U8 rx_frame_include_crc_mode;/*Relevant for UFE3 only*/
      WP_U8 rx_frame_crc_size;
      WP_U8 tx_frame_include_crc_mode;/*Relevant for UFE3 only*/
      WP_U8 tx_frame_crc_size;
      WP_U8 tx_no_flag_between_frames;
      WP_U8 tx_min_flag_num;
      WP_U8 tx_underrun_interrupt_mode;/*Relevant for UFE3 only*/
}
WUFE_Phy_hdlc;

typedef struct
{
      WP_U8 rx_frame_crc_size;
      WP_U8 rx_descrambler_mode;
      WP_U8 tx_frame_crc_size;
      WP_U8 tx_scrambler_mode;
      WP_U8 tx_min_flag_num;
}
WUFE_Phy_pos;

typedef struct
{
      WP_U8 rx_data_unit_size;
      WP_U8 tx_underrun_interrupt_mode; /*Relevant for UFE3 only*/
      WP_U8 rx_clk_rec_mode;           /* Relevant for UFE3 only */
}
WUFE_Phy_trans;

typedef struct
{
      WP_U8 tbd; /*Not Yet Implemented*/
}
WUFE_Phy_gfp;

typedef struct
{
      WUFE_struct_id   struct_id;
      WUFE_RxFifo      rx_fifo_param;
      WUFE_SlotGroup   rx_slot_group;
      WUFE_TxFifo      tx_fifo_param;
      WUFE_SlotGroup   tx_slot_group;
      union{
            WUFE_Phy_atm    atm;
            WUFE_Phy_hdlc   hdlc;
            WUFE_Phy_trans  trans;
            WUFE_Phy_pos    pos;
            WUFE_Phy_gfp    gfp;
      } type;
}
WUFE_phy;

#define WUFE_POS_NO_CHANGE 0xff
#define WUFE_HDLC_NO_CHANGE 0xff
#define WUFE_ATM_NO_CHANGE 0xff

/*****************************************/
/********     Use Macros           *******/
/*****************************************/

#define WUFE_PHY_INDEX_GET(phy_handle) (phy_handle)&((1<<16)-1)

#endif /*WUFEI_PHY_H*/
