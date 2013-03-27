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
#ifndef WUFEI_HW_EMPHY_H
#define WUFEI_HW_EMPHY_H

#ifndef WUFE_SYSTEM_H
#include "wufe_system.h"
#endif

#ifndef WUFEI_HW_TYPES_H
#include "wufei_hw_types.h"
#endif

typedef struct
{
   WP_U8 pri_function;
}WUFE_hw_rx_priority;

typedef struct
{
   WP_U16 pri_function;
}WUFE_hw_tx_priority;

typedef struct
{
      WUFEI_reg_space   mode; /* EMPHY mode register */
      WUFEI_reg_space   event_reg; /*EMPHY interrupt register*/
      WUFEI_reg_space   event_mask_reg; /*EMPHY mask interrupt register*/
      WUFEI_reg_space   event_phy_nv; /*EMPHY non valid interrupt port register*/
      WUFEI_reg_space   reserved1[12];
      WUFEI_reg_space   rx_priority_desc[WUFE_N_PRI_DESCRIPTOR/2]; /* EMPHY rx priority */
      WUFEI_reg_space   reserved2[110];
      WUFEI_reg_space   tx_priority_desc[WUFE_N_PRI_DESCRIPTOR]; /* EMPHY tx priority */
      WUFEI_reg_space   reserved3[124];
}WUFEI_hw_emphy_n_direct;

typedef struct
{
      WUFEI_reg_space reserved1[16];
      WUFEI_reg_space core1_port_id_offset; /*EMPHY Core 1 port ID offset register*/
      WUFEI_reg_space reserved2[239];
      WUFEI_hw_emphy_n_direct emphy_direct_reg[WUFE_MAX_N_EMPHYS];
      WUFEI_reg_space reserved3[512];
      WUFEI_reg_space reserved4[2816]; /*Reserved for timestamp registers*/
}WUFEI_hw_emphy;

/*emphy setup mode options*/
enum
{
   WUFEI_EMPHY_SETUP_MODE_MUX = 0,
   WUFEI_EMPHY_SETUP_MODE_DUAL = 1
};

/*EMPHY Core1 Port ID Offset Register*/

#define WUFEI_EMPHY_PORT_ID_OFFSET_S (0)
#define WUFEI_EMPHY_PORT_ID_OFFSET_W (13)
#define WUFEI_EMPHY_PORT_ID_OFFSET_F(v) WUFEI_FIELD(v, WUFEI_EMPHY_PORT_ID_OFFSET_S)
#define WUFEI_EMPHY_PORT_ID_OFFSET_V(f) \
        WUFEI_VALUE(f, WUFEI_EMPHY_PORT_ID_OFFSET_S, WUFEI_EMPHY_PORT_ID_OFFSET_W)
#define WUFEI_EMPHY_PORT_ID_OFFSET_M \
        WUFEI_MASK(WUFEI_EMPHY_PORT_ID_OFFSET_S, WUFEI_EMPHY_PORT_ID_OFFSET_W)

#define WUFEI_EMPHY_PORT_ID_OFFSET_MAX  0x1000

/*EMPHY Mode Register*/

#define WUFEI_EMPHY_RX_MODE_ENABLE_S (0)
#define WUFEI_EMPHY_RX_MODE_ENABLE_W (1)
#define WUFEI_EMPHY_RX_MODE_ENABLE_F(v) WUFEI_FIELD(v, WUFEI_EMPHY_RX_MODE_ENABLE_S)
#define WUFEI_EMPHY_RX_MODE_ENABLE_V(f) \
        WUFEI_VALUE(f, WUFEI_EMPHY_RX_MODE_ENABLE_S, WUFEI_EMPHY_RX_MODE_ENABLE_W)
#define WUFEI_EMPHY_RX_MODE_ENABLE_M \
        WUFEI_MASK(WUFEI_EMPHY_RX_MODE_ENABLE_S, WUFEI_EMPHY_RX_MODE_ENABLE_W)

#define WUFEI_EMPHY_RX_PARITY_EVEN_S (1)
#define WUFEI_EMPHY_RX_PARITY_EVEN_W (1)
#define WUFEI_EMPHY_RX_PARITY_EVEN_F(v) WUFEI_FIELD(v, WUFEI_EMPHY_RX_PARITY_EVEN_S)
#define WUFEI_EMPHY_RX_PARITY_EVEN_V(f) \
        WUFEI_VALUE(f, WUFEI_EMPHY_RX_PARITY_EVEN_S, WUFEI_EMPHY_RX_PARITY_EVEN_W)
#define WUFEI_EMPHY_RX_PARITY_EVEN_M \
        WUFEI_MASK(WUFEI_EMPHY_RX_PARITY_EVEN_S, WUFEI_EMPHY_RX_PARITY_EVEN_W)

#define WUFEI_EMPHY_TX_MODE_ENABLE_S (8)
#define WUFEI_EMPHY_TX_MODE_ENABLE_W (1)
#define WUFEI_EMPHY_TX_MODE_ENABLE_F(v) WUFEI_FIELD(v, WUFEI_EMPHY_TX_MODE_ENABLE_S)
#define WUFEI_EMPHY_TX_MODE_ENABLE_V(f) \
        WUFEI_VALUE(f, WUFEI_EMPHY_TX_MODE_ENABLE_S, WUFEI_EMPHY_TX_MODE_ENABLE_W)
#define WUFEI_EMPHY_TX_MODE_ENABLE_M \
        WUFEI_MASK(WUFEI_EMPHY_TX_MODE_ENABLE_S, WUFEI_EMPHY_TX_MODE_ENABLE_W)

#define WUFEI_EMPHY_TX_PARITY_EVEN_S (9)
#define WUFEI_EMPHY_TX_PARITY_EVEN_W (1)
#define WUFEI_EMPHY_TX_PARITY_EVEN_F(v) WUFEI_FIELD(v, WUFEI_EMPHY_TX_PARITY_EVEN_S)
#define WUFEI_EMPHY_TX_PARITY_EVEN_V(f) \
        WUFEI_VALUE(f, WUFEI_EMPHY_TX_PARITY_EVEN_S, WUFEI_EMPHY_TX_PARITY_EVEN_W)
#define WUFEI_EMPHY_TX_PARITY_EVEN_M \
        WUFEI_MASK(WUFEI_EMPHY_TX_PARITY_EVEN_S, WUFEI_EMPHY_TX_PARITY_EVEN_W)

#define WUFEI_EMPHY_TX_PARITY_CHECK_S (10)
#define WUFEI_EMPHY_TX_PARITY_CHECK_W (1)
#define WUFEI_EMPHY_TX_PARITY_CHECK_F(v) WUFEI_FIELD(v, WUFEI_EMPHY_TX_PARITY_CHECK_S)
#define WUFEI_EMPHY_TX_PARITY_CHECK_V(f) \
        WUFEI_VALUE(f, WUFEI_EMPHY_TX_PARITY_CHECK_S, WUFEI_EMPHY_TX_PARITY_CHECK_W)
#define WUFEI_EMPHY_TX_PARITY_CHECK_M \
        WUFEI_MASK(WUFEI_EMPHY_TX_PARITY_CHECK_S, WUFEI_EMPHY_TX_PARITY_CHECK_W)

#endif /* WUFEI_HW_EMPHY_H*/
