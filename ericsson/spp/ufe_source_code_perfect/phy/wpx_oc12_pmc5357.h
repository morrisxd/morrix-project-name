/*****************************************************************************
 * (C) Copyright 2001-2006, Wintegra.  All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/************************************************************************/
/*                      OC12 CARD INITIALIZATION                        */
/************************************************************************/

/* This initialization code assumes that the host port for the PMC5357
   is implemented as a 64-bit port. */

/* Note: This is a preliminary release, which assumes a default memory map. */

#ifndef WPX_OC12_PMC5357_H
#define WPX_OC12_PMC5357_H

#include "api/wp_types.h"
#include "wpx_board_data.h"
#include "wpx_oc_common.h"

/* Defined modes of operation */
#define WPU_MODE_OC12_ATM 0x00000
#define WPU_MODE_OC12_POS 0x10000
#define WPU_MODE_OC12_SPI 0x20000

#define WPU_MODE_OC12_LOOPBACK_OFF 0
#define WPU_MODE_OC12_LOOPBACK_CLOSE 1
#define WPU_MODE_OC12_LOOPBACK_MIDDLE 2
#define WPU_MODE_OC12_LOOPBACK_FAR 3

#define WPU_MODE_OC12_EXT_CLOCK 0x0
#define WPU_MODE_OC12_INT_CLOCK 0x01000000

/* This structure includes the registers used in this initialization.
   It is not a complete representation of the PMC5357 register set. */
typedef struct {
      wpx_card_reg_size     master_reset;                /* 0x0 */
      wpx_card_reg_size     master_config_1;             /* 0x8 */
      wpx_card_reg_size     master_config_2;             /* 0x10 */
      wpx_card_reg_size     reserved3[0x45];
      wpx_card_reg_size     tpop_path_signal_label;      /* 0x240 */
      wpx_card_reg_size     reserved4[0xb];
      wpx_card_reg_size     sptb_exp_path_signal_label;  /* 0x2a0 */
      wpx_card_reg_size     reserved5[0x7];
      wpx_card_reg_size     crsi_config;                 /* 0x2e0 */
      wpx_card_reg_size     reserved6[0x5];
      wpx_card_reg_size     rxcp_fifo_cntl;              /* 0x310 */
      wpx_card_reg_size     reserved7[0x1d];
      wpx_card_reg_size     txcp_config_1;               /* 0x400 */
      wpx_card_reg_size     reserved8[0xf];
      wpx_card_reg_size     rul3_config;                 /* 0x480 */
      wpx_card_reg_size     reserved9[0x1];
      wpx_card_reg_size     tul3_config;                 /* 0x490 */
      wpx_card_reg_size     reserved10[0x3];
      wpx_card_reg_size     dll_rfclk_reset;             /* 0x4b0 */
      wpx_card_reg_size     reserved11[0x3];
      wpx_card_reg_size     dll_tfclk_reset;             /* 0x4d0 */
      wpx_card_reg_size     reserved12[0x3];
      wpx_card_reg_size     dll_ptclk_reset;             /* 0x4f0 */
      wpx_card_reg_size     reserved13[0x1];
      wpx_card_reg_size     rxfp_config;                 /* 0x500 */
      wpx_card_reg_size     reserved14[0x6];
      wpx_card_reg_size     rxfp_rx_high_watermark;       /* 0x538 */
      wpx_card_reg_size     reserved15[0x19];
      wpx_card_reg_size     txfp_config;                 /* 0x608 */
      wpx_card_reg_size     txfp_tx_control;             /* 0x610 */
      wpx_card_reg_size     txfp_tx_low_watermark;       /* 0x618 */
      wpx_card_reg_size     txfp_tx_high_watermark;      /* 0x620 */
      wpx_card_reg_size     reserved19[0x3b];
} wpu_pmc5357;

WP_status WPU_Oc12Pmc5357CardInit(WP_U32 port, WP_U32 mode);

#endif
