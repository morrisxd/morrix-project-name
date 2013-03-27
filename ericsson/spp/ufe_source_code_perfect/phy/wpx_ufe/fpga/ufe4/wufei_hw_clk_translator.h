/*****************************************************************************
 * (C) Copyright 2000-2006, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: wufei_hw_clk_translator.h
 *
 * Purpose: the UFE Line from clock translator type macro and structures
 *
 ****************************************************************************/
#ifndef WUFEI_HW_CLK_TRANSLATOR_H
#define WUFEI_HW_CLK_TRANSLATOR_H

#include "wufe_line.h"

#ifndef WUFEI_HW_TYPES_H
#include "wufei_hw_types.h"
#endif

/*********************************************
 *            TX CLOCK TRANSLATOR
 ********************************************/
#define WUFEI_SBI_CT_NON_ACTIVE_INSTANCE  0xFFFF

typedef struct
{
      WP_U16 ct_tx; /* Clock Translator config register */
}WUFEI_hw_clk_rec_ct_tx_ram;

#define WUFEI_HW_SBI_TX_CT_ENABLE_S (15)
#define WUFEI_HW_SBI_TX_CT_ENABLE_W (1)
#define WUFEI_HW_SBI_TX_CT_ENABLE_F(v) WUFEI_FIELD(v, WUFEI_HW_SBI_TX_CT_ENABLE_S)
#define WUFEI_HW_SBI_TX_CT_ENABLE_V(f) \
        WUFEI_VALUE(f, WUFEI_HW_SBI_TX_CT_ENABLE_S, WUFEI_HW_SBI_TX_CT_ENABLE_W)
#define WUFEI_HW_SBI_TX_CT_ENABLE_M \
        WUFEI_MASK(WUFEI_HW_SBI_TX_CT_ENABLE_S, WUFEI_HW_SBI_TX_CT_ENABLE_W)

/* CT_trib_id (adap+diff) and 'valid' bits (RAM) */
#define WUFEI_HW_SBI_TX_CT_DIFF_VALID_S (15)
#define WUFEI_HW_SBI_TX_CT_DIFF_VALID_W (1)
#define WUFEI_HW_SBI_TX_CT_DIFF_VALID_F(v) \
    WUFEI_FIELD(v, WUFEI_HW_SBI_TX_CT_DIFF_VALID_S)
#define WUFEI_HW_SBI_TX_CT_DIFF_VALID_V(f) \
    WUFEI_VALUE(f, WUFEI_HW_SBI_TX_CT_DIFF_VALID_S, WUFEI_HW_SBI_TX_CT_DIFF_VALID_W)
#define WUFEI_HW_SBI_TX_CT_DIFF_VALID_M \
    WUFEI_MASK(WUFEI_HW_SBI_TX_CT_DIFF_VALID_S, WUFEI_HW_SBI_TX_CT_DIFF_VALID_W)

#define WUFEI_HW_SBI_TX_CT_DIFF_UNIT_ID_S (6)
#define WUFEI_HW_SBI_TX_CT_DIFF_UNIT_ID_W (9)
#define WUFEI_HW_SBI_TX_CT_DIFF_UNIT_ID_F(v) \
    WUFEI_FIELD(v, WUFEI_HW_SBI_TX_CT_DIFF_UNIT_ID_S)
#define WUFEI_HW_SBI_TX_CT_DIFF_UNIT_ID_V(f) \
    WUFEI_VALUE(f, WUFEI_HW_SBI_TX_CT_DIFF_UNIT_ID_S, WUFEI_HW_SBI_TX_CT_DIFF_UNIT_ID_W)
#define WUFEI_HW_SBI_TX_CT_DIFF_UNIT_ID_M \
    WUFEI_MASK(WUFEI_HW_SBI_TX_CT_DIFF_UNIT_ID_S, WUFEI_HW_SBI_TX_CT_DIFF_UNIT_ID_W)

#define WUFEI_HW_SBI_TX_CT_ADAP_VALID_S (5)
#define WUFEI_HW_SBI_TX_CT_ADAP_VALID_W (1)
#define WUFEI_HW_SBI_TX_CT_ADAP_VALID_F(v) \
    WUFEI_FIELD(v, WUFEI_HW_SBI_TX_CT_ADAP_VALID_S)
#define WUFEI_HW_SBI_TX_CT_ADAP_VALID_V(f) \
    WUFEI_VALUE(f, WUFEI_HW_SBI_TX_CT_ADAP_VALID_S, WUFEI_HW_SBI_TX_CT_ADAP_VALID_W)
#define WUFEI_HW_SBI_TX_CT_ADAP_VALID_M \
    WUFEI_MASK(WUFEI_HW_SBI_TX_CT_ADAP_VALID_S, WUFEI_HW_SBI_TX_CT_ADAP_VALID_W)

#define WUFEI_HW_SBI_TX_CT_ADAP_UNIT_ID_S (0)
#define WUFEI_HW_SBI_TX_CT_ADAP_UNIT_ID_W (5)
#define WUFEI_HW_SBI_TX_CT_ADAP_UNIT_ID_F(v) \
    WUFEI_FIELD(v, WUFEI_HW_SBI_TX_CT_ADAP_UNIT_ID_S)
#define WUFEI_HW_SBI_TX_CT_ADAP_UNIT_ID_V(f) \
    WUFEI_VALUE(f, WUFEI_HW_SBI_TX_CT_ADAP_UNIT_ID_S, WUFEI_HW_SBI_TX_CT_ADAP_UNIT_ID_W)
#define WUFEI_HW_SBI_TX_CT_ADAP_UNIT_ID_M \
    WUFEI_MASK(WUFEI_HW_SBI_TX_CT_ADAP_UNIT_ID_S, WUFEI_HW_SBI_TX_CT_ADAP_UNIT_ID_W)

#define WUFEI_HW_SBI_TX_CT_TRIB_ID_S (4)
#define WUFEI_HW_SBI_TX_CT_TRIB_ID_W (9)
#define WUFEI_HW_SBI_TX_CT_TRIB_ID_F(v) \
    WUFEI_FIELD(v, WUFEI_HW_SBI_TX_CT_TRIB_ID_S)
#define WUFEI_HW_SBI_TX_CT_TRIB_ID_V(f) \
    WUFEI_VALUE(f, WUFEI_HW_SBI_TX_CT_TRIB_ID_S, WUFEI_HW_SBI_TX_CT_TRIB_ID_W)
#define WUFEI_HW_SBI_TX_CT_TRIB_ID_M \
    WUFEI_MASK(WUFEI_HW_SBI_TX_CT_TRIB_ID_S, WUFEI_HW_SBI_TX_CT_TRIB_ID_W)

#define WUFEI_HW_SBI_TX_CT_TRIB_FRAMED_S (3)
#define WUFEI_HW_SBI_TX_CT_TRIB_FRAMED_W (1)
#define WUFEI_HW_SBI_TX_CT_TRIB_FRAMED_F(v) \
        WUFEI_FIELD(v, WUFEI_HW_SBI_TX_CT_TRIB_FRAMED_S)
#define WUFEI_HW_SBI_TX_CT_TRIB_FRAMED_V(f) \
        WUFEI_VALUE(f, WUFEI_HW_SBI_TX_CT_TRIB_FRAMED_S, WUFEI_HW_SBI_TX_CT_TRIB_FRAMED_W)
#define WUFEI_HW_SBI_TX_CT_TRIB_FRAMED_M \
        WUFEI_MASK(WUFEI_HW_SBI_TX_CT_TRIB_FRAMED_S, WUFEI_HW_SBI_TX_CT_TRIB_FRAMED_W)

#define WUFEI_HW_SBI_TX_CT_TRIB_TYPE_S (0)
#define WUFEI_HW_SBI_TX_CT_TRIB_TYPE_W (1)
#define WUFEI_HW_SBI_TX_CT_TRIB_TYPE_F(v) WUFEI_FIELD(v, WUFEI_HW_SBI_TX_CT_TRIB_TYPE_S)
#define WUFEI_HW_SBI_TX_CT_TRIB_TYPE_V(f) \
        WUFEI_VALUE(f, WUFEI_HW_SBI_TX_CT_TRIB_TYPE_S, WUFEI_HW_SBI_TX_CT_TRIB_TYPE_W)
#define WUFEI_HW_SBI_TX_CT_TRIB_TYPE_M \
        WUFEI_MASK(WUFEI_HW_SBI_TX_CT_TRIB_TYPE_S, WUFEI_HW_SBI_TX_CT_TRIB_TYPE_W)

/* fields within trib_id */

#define WUFEI_HW_SBI_TX_CT_TRIB_ID_LINK_S (0)
#define WUFEI_HW_SBI_TX_CT_TRIB_ID_LINK_W (5)
#define WUFEI_HW_SBI_TX_CT_TRIB_ID_LINK_F(v) \
    WUFEI_FIELD(v, WUFEI_HW_SBI_TX_CT_TRIB_ID_LINK_S)
#define WUFEI_HW_SBI_TX_CT_TRIB_ID_LINK_V(f) \
    WUFEI_VALUE(f, WUFEI_HW_SBI_TX_CT_TRIB_ID_LINK_S, WUFEI_HW_SBI_TX_CT_TRIB_ID_LINK_W)
#define WUFEI_HW_SBI_TX_CT_TRIB_ID_LINK_M \
    WUFEI_MASK(WUFEI_HW_SBI_TX_CT_TRIB_ID_LINK_S, WUFEI_HW_SBI_TX_CT_TRIB_ID_LINK_W)

#define WUFEI_HW_SBI_TX_CT_TRIB_ID_SPE_S (5)
#define WUFEI_HW_SBI_TX_CT_TRIB_ID_SPE_W (2)
#define WUFEI_HW_SBI_TX_CT_TRIB_ID_SPE_F(v) \
    WUFEI_FIELD(v, WUFEI_HW_SBI_TX_CT_TRIB_ID_SPE_S)
#define WUFEI_HW_SBI_TX_CT_TRIB_ID_SPE_V(f) \
    WUFEI_VALUE(f, WUFEI_HW_SBI_TX_CT_TRIB_ID_SPE_S, WUFEI_HW_SBI_TX_CT_TRIB_ID_SPE_W)
#define WUFEI_HW_SBI_TX_CT_TRIB_ID_SPE_M \
    WUFEI_MASK(WUFEI_HW_SBI_TX_CT_TRIB_ID_SPE_S, WUFEI_HW_SBI_TX_CT_TRIB_ID_SPE_W)

#define WUFEI_HW_SBI_TX_CT_TRIB_ID_STM_S (7)
#define WUFEI_HW_SBI_TX_CT_TRIB_ID_STM_W (1)
#define WUFEI_HW_SBI_TX_CT_TRIB_ID_STM_F(v) \
    WUFEI_FIELD(v, WUFEI_HW_SBI_TX_CT_TRIB_ID_STM_S)
#define WUFEI_HW_SBI_TX_CT_TRIB_ID_STM_V(f) \
    WUFEI_VALUE(f, WUFEI_HW_SBI_TX_CT_TRIB_ID_STM_S, WUFEI_HW_SBI_TX_CT_TRIB_ID_STM_W)
#define WUFEI_HW_SBI_TX_CT_TRIB_ID_STM_M \
    WUFEI_MASK(WUFEI_HW_SBI_TX_CT_TRIB_ID_STM_S, WUFEI_HW_SBI_TX_CT_TRIB_ID_STM_W)

#define WUFEI_HW_SBI_TX_CT_TRIB_ID_CORE_S (8)
#define WUFEI_HW_SBI_TX_CT_TRIB_ID_CORE_W (1)
#define WUFEI_HW_SBI_TX_CT_TRIB_ID_CORE_F(v) \
    WUFEI_FIELD(v, WUFEI_HW_SBI_TX_CT_TRIB_ID_CORE_S)
#define WUFEI_HW_SBI_TX_CT_TRIB_ID_CORE_V(f) \
    WUFEI_VALUE(f, WUFEI_HW_SBI_TX_CT_TRIB_ID_CORE_S, WUFEI_HW_SBI_TX_CT_TRIB_ID_CORE_W)
#define WUFEI_HW_SBI_TX_CT_TRIB_ID_CORE_M \
    WUFEI_MASK(WUFEI_HW_SBI_TX_CT_TRIB_ID_CORE_S, WUFEI_HW_SBI_TX_CT_TRIB_ID_CORE_W)

#endif /* WUFEI_HW_CLK_TRANSLATOR_H*/
