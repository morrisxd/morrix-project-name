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
      WUFEI_reg_space   event_reg; /*EMPHY interrupt register*/
      WUFEI_reg_space   event_mask_reg; /*EMPHY mask interrupt register*/
}WUFEI_hw_emphy_event;

typedef struct
{
      WUFEI_reg_space mode; /*EMPHY mode register*/
      WUFEI_reg_space event_phy_non_valid; /*EMPHY non valid interrupt port register*/
      WUFEI_reg_space ports_table_threshold; /*EMPHY ports table threshold register*/
      WUFEI_reg_space reserved[13];
}WUFEI_hw_emphy_mode;

typedef struct
{
      WUFEI_reg_space reserved1[8];
      WUFEI_hw_emphy_event emphy_event_reg[WUFE_MAX_N_EMPHYS];
      WUFEI_reg_space reserved2[4];
      WUFEI_hw_emphy_mode emphy_mode_reg[WUFE_MAX_N_EMPHYS];
      WUFEI_reg_space core1_port_id_offset; /*EMPHY Core 1 port ID offset register*/
}WUFEI_hw_emphy;

/*emphy setup mode options*/
enum
{
   WUFEI_EMPHY_SETUP_MODE_SINGLE = 1,
   WUFEI_EMPHY_SETUP_MODE_DUAL = 2,
   WUFEI_EMPHY_SETUP_MODE_MUX = 3
};

#define WUFEI_EMPHY_PORT_TABLE_THERSHOLD 32

/*EMPHY Non Valid Port ID Register*/

#define WUFEI_EMPHY_NON_VALID_PORT_ID_S (0)
#define WUFEI_EMPHY_NON_VALID_PORT_ID_W (13)
#define WUFEI_EMPHY_NON_VALID_PORT_ID_F(v) WUFEI_FIELD(v, WUFEI_EMPHY_NON_VALID_PORT_ID_S)
#define WUFEI_EMPHY_NON_VALID_PORT_ID_V(f) \
        WUFEI_VALUE(f, WUFEI_EMPHY_NON_VALID_PORT_ID_S, WUFEI_EMPHY_NON_VALID_PORT_ID_W)
#define WUFEI_EMPHY_NON_VALID_PORT_ID_M \
        WUFEI_MASK(WUFEI_EMPHY_NON_VALID_PORT_ID_S, WUFEI_EMPHY_NON_VALID_PORT_ID_W)

/*EMPHY Port Table Threshold Register*/

#define WUFEI_EMPHY_PORT_TABLE_TH_S (0)
#define WUFEI_EMPHY_PORT_TABLE_TH_W (7)
#define WUFEI_EMPHY_PORT_TABLE_TH_F(v) WUFEI_FIELD(v, WUFEI_EMPHY_PORT_TABLE_TH_S)
#define WUFEI_EMPHY_PORT_TABLE_TH_V(f) \
        WUFEI_VALUE(f, WUFEI_EMPHY_PORT_TABLE_TH_S, WUFEI_EMPHY_PORT_TABLE_TH_W)
#define WUFEI_EMPHY_PORT_TABLE_TH_M \
        WUFEI_MASK(WUFEI_EMPHY_PORT_TABLE_TH_S, WUFEI_EMPHY_PORT_TABLE_TH_W)

/*EMPHY Core1 Port ID Offset Register*/

#define WUFEI_EMPHY_PORT_ID_OFFSET_S (0)
#define WUFEI_EMPHY_PORT_ID_OFFSET_W (15)
#define WUFEI_EMPHY_PORT_ID_OFFSET_F(v) WUFEI_FIELD(v, WUFEI_EMPHY_PORT_ID_OFFSET_S)
#define WUFEI_EMPHY_PORT_ID_OFFSET_V(f) \
        WUFEI_VALUE(f, WUFEI_EMPHY_PORT_ID_OFFSET_S, WUFEI_EMPHY_PORT_ID_OFFSET_W)
#define WUFEI_EMPHY_PORT_ID_OFFSET_M \
        WUFEI_MASK(WUFEI_EMPHY_PORT_ID_OFFSET_S, WUFEI_EMPHY_PORT_ID_OFFSET_W)

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
