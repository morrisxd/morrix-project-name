/*****************************************************************************
 * (C) Copyright 2001-2006, Wintegra.  All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/************************************************************************/
/*                      OC3 CARD INITIALIZATION                         */
/************************************************************************/

/* This initialization code assumes that the host port for the PMC5351
   is implemented as a 64-bit port. */

/* Note: This is a preliminary release, which assumes a default memory map. */

#ifndef WPX_OC3_PMC5351_H
#define WPX_OC3_PMC5351_H

#include "api/wp_types.h"
#include "wpx_oc_common.h"

/* Defined modes of operation */
#define WPU_MODE_OC3_ATM 0x00000
#define WPU_MODE_OC3_POS 0x10000

#define WPU_MODE_OC3_PHY0_LOOPBACK_OFF 0x0000
#define WPU_MODE_OC3_PHY0_LOOPBACK_CLOSE 0x0001
#define WPU_MODE_OC3_PHY0_LOOPBACK_FAR 0x0002

#define WPU_MODE_OC3_PHY1_LOOPBACK_OFF 0x0000
#define WPU_MODE_OC3_PHY1_LOOPBACK_CLOSE 0x0010
#define WPU_MODE_OC3_PHY1_LOOPBACK_FAR 0x0020

#define WPU_MODE_OC3_PHY2_LOOPBACK_OFF 0x0000
#define WPU_MODE_OC3_PHY2_LOOPBACK_CLOSE 0x0100
#define WPU_MODE_OC3_PHY2_LOOPBACK_FAR 0x0200

#define WPU_MODE_OC3_PHY3_LOOPBACK_OFF 0x0000
#define WPU_MODE_OC3_PHY3_LOOPBACK_CLOSE 0x1000
#define WPU_MODE_OC3_PHY3_LOOPBACK_FAR 0x2000


/* This structure includes the registers used in this initialization.
   It is not a complete representation of a PMC5351 register set. */
typedef struct {
      wpx_card_reg_size     master_reset;                /* 0x0 */
      wpx_card_reg_size     master_config_1;             /* 0x1 */
      wpx_card_reg_size     master_config_2;             /* 0x2 */
      wpx_card_reg_size     reserved3[0x2];
      wpx_card_reg_size     channel_reset;               /* 0x5 */
      wpx_card_reg_size     reserved3b[0x1];
      wpx_card_reg_size     master_cntl;                 /* 0x7 */
      wpx_card_reg_size     reserved4[0x40];
      wpx_card_reg_size     tpop_path_signal_label;      /* 0x48 */
      wpx_card_reg_size     reserved5[0xb];
      wpx_card_reg_size     sptb_exp_path_signal_label;  /* 0x54 */
      wpx_card_reg_size     reserved6[0xb];
      wpx_card_reg_size     rxcp_config_1;               /* 0x60 */
      wpx_card_reg_size     rxcp_config_2;               /* 0x61 */
      wpx_card_reg_size     rxcp_fifo_cntl;              /* 0x62 */
      wpx_card_reg_size     reserved7[0x1d];
      wpx_card_reg_size     txcp_config_1;               /* 0x80 */
      wpx_card_reg_size     reserved8[0x1];
      wpx_card_reg_size     txcp_cell_status;            /* 0x82 */
      wpx_card_reg_size     reserved9[0xd];
      wpx_card_reg_size     rul3_config;                 /* 0x90 */
      wpx_card_reg_size     reserved10[0x1];
      wpx_card_reg_size     tul3_config;                 /* 0x92 */
      wpx_card_reg_size     reserved11[0x3];
      wpx_card_reg_size     dll_rfclk_reset;             /* 0x96 */
      wpx_card_reg_size     reserved12[0x3];
      wpx_card_reg_size     dll_tfclk_reset;             /* 0x9a */
      wpx_card_reg_size     reserved13[0x3];
      wpx_card_reg_size     dll_ptclk_reset;             /* 0x9e */
      wpx_card_reg_size     reserved14[0x1];
      wpx_card_reg_size     rxfp_config;                 /* 0xa0 */
      wpx_card_reg_size     reserved15[0x6];
      wpx_card_reg_size     rxfp_rx_high_watemark;       /* 0xa7 */
      wpx_card_reg_size     reserved16[0x19];
      wpx_card_reg_size     txfp_config;                 /* 0xc1 */
      wpx_card_reg_size     txfp_tx_control;             /* 0xc2 */
      wpx_card_reg_size     txfp_tx_low_watermark;       /* 0xc3 */
      wpx_card_reg_size     txfp_tx_high_watermark;      /* 0xc4 */
      wpx_card_reg_size     reserved20[0x3b];
} wpu_pmc5351_phy;

typedef struct {
      wpu_pmc5351_phy phy[4];
} wpu_pmc5351;

enum WPU_Oc3Pmc5351Ops {
   WPU_OC3_PMC5351_HCS_PASS,
   WPU_OC3_PMC5351_HCS_FILTER,
   WPU_OC3_PMC5351_PHY_DISABLE,
   WPU_OC3_PMC5351_PHY_ENABLE,
   WPU_OC3_PMC5351_PHY_BASE_4,
   WPU_OC3_PMC5351_POS_PHY_BASE_4
};

WP_status WPU_Oc3Pmc5351CardInit(WP_U32 port, WP_U32 mode);
WP_status WPU_Oc3Pmc5351Control(WP_U32 port, WP_U32 phy, WP_U32 op);
#endif
