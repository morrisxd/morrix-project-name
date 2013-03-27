/*****************************************************************************
 * (C) Copyright 2000-2006, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: wufe_clk_rec.h
 *
 * Purpose: Clock recovery user configuration structures
 *
 ****************************************************************************/

#ifndef WUFE_CLK_REC_H
#define WUFE_CLK_REC_H

#ifndef WUFE_TYPES_H
#include "wufe_types.h"
#endif

typedef enum
{
   WUFE_CLOCK_REC_SYNC_POS = 0,
   WUFE_CLOCK_REC_SYNC_NEG = 1
}WUFE_clock_rec_sync;

typedef enum
{
   WUFE_CLOCK_REC_ACTIVE_HIGH = 0,
   WUFE_CLOCK_REC_ACTIVE_LOW =  1
}WUFE_clock_rec_active;

enum
{
   WUFE_CLOCK_REC_METHOD_ADAPTIVE = 0,
   WUFE_CLOCK_REC_METHOD_DIFFERENTIAL,
   WUFE_CLOCK_REC_METHOD_NA
};

typedef enum
{
   WUFE_CLOCK_REC_INFO_HW,
   WUFE_CLOCK_REC_INFO_SYSTEM,
   WUFE_CLOCK_REC_INFO_PHY,
   WUFE_CLOCK_REC_INFO_LINE,
   WUFE_CLOCK_REC_INFO_RAM_LINE
}WUFE_clock_rec_info_type;

enum
{
   WUFE_CLOCK_REC_REF_CLK_T1 = 0,
   WUFE_CLOCK_REC_REF_CLK_E1,
   WUFE_CLOCK_REC_REF_CLK_DS3,
   WUFE_CLOCK_REC_REF_CLK_E3,
   WUFE_CLOCK_REC_REF_CLK_TOTAL
};

enum
{
   WUFE_GPIO_0 = 0,
   WUFE_GPIO_1 = 1
};

/* GPIO output clock types */
enum
{
   WUFE_GPIO_CLK_OUT_RECV_PDH_TX_CLK_0 = 0,
   WUFE_GPIO_CLK_OUT_RECV_PDH_TX_CLK_1,
   WUFE_GPIO_CLK_OUT_RECV_PDH_LINE_CLK_0,
   WUFE_GPIO_CLK_OUT_RECV_PDH_LINE_CLK_1,
   WUFE_GPIO_CLK_OUT_GP_CLK_IN_0,
   WUFE_GPIO_CLK_OUT_GP_CLK_IN_1,
   WUFE_GPIO_CLK_OUT_SYSTEM_CLK,
   WUFE_GPIO_CLK_OUT_XO_PLL_REF_CLK,
   WUFE_GPIO_CLK_OUT_RECV_SDH_LINE_CLK_1,    /* Line clock of the second SDH. */
   WUFE_GPIO_CLK_OUT_NONE
};

enum
{
   WUFE_GPIO_REF_CLK_OUTPUT_XO_REF_CLK,
   WUFE_GPIO_REF_CLK_OUTPUT_DIFF_REF_CLK
};

typedef struct
{
      WP_U8  enable;        /* Relevant for UFE3 only */
      WP_U16 buffer_size;   /* Relevant for UFE3 only */
      WP_U32 if_id;         /* Relevant for UFE3 only */
}WUFE_clk_rec_info;

typedef struct
{
      WUFE_clk_rec_info tx; /* Relevant for UFE3 only */
}WUFE_clk_rec_info_phy;

typedef struct
{
      WP_U16  max_rx_clk_rec_if;          /* Relevant for UFE3 only */
      WP_U16  max_tx_clk_rec_if;          /* Relevant for UFE3 only */
      WP_U16  max_ext_diff_rx_clk_rec_if; /* Relevant for UFE3 only */
      WP_U16  max_ext_diff_tx_clk_rec_if; /* Relevant for UFE3 only */
      WP_U8   method;                     /* Relevant for UFE3 only */
}WUFE_clk_rec_info_system;

typedef struct
{
      WP_U16  max_rx_clk_rec_if;          /* Relevant for UFE3 only */
      WP_U16  max_tx_clk_rec_if;          /* Relevant for UFE3 only */
      WP_U16  max_ext_diff_rx_clk_rec_if; /* Relevant for UFE3 only */
      WP_U16  max_ext_diff_tx_clk_rec_if; /* Relevant for UFE3 only */
}WUFE_clk_rec_info_hw;

typedef struct
{
      WP_U32  clock_rec_if_id;          /* Relevant for UFE4 only */
}WUFE_clk_rec_info_line;

typedef struct
{
      WP_U8  stuffing_ind;               /* Relevant for UFE4 only */
      WP_U8  current_gap_count;
      WP_U8  max_gap_vector;
      WP_U32 current_gap_fraction;
      WP_U32 delta_gap;
}WUFE_clk_rec_info_ram_line;


/*
 * Reference clock for Digital Controlled Oscillator frequency (DRC)
 * This structure is used in order to set the nominal divisors
 * for the DRC clock. These values are set in the specific clock
 * dividers registers located at the board support package of the UFE FPGA
 */
typedef struct
{
      WP_U8  nominal_int;
      WP_U32 nominal_rem;
}WUFE_clk_rec_drc_divisor;

typedef struct
{
      WUFE_clk_rec_drc_divisor divisor[WUFE_CLOCK_REC_REF_CLK_TOTAL];
}WUFE_clk_rec_drc_divisor_list;

/*
 * Timestamp adaptive/differential reference clock (TSRC)
 * This clock is used by the timestamp generation unit
 * in the differential/adaptive mode.
 */

/*************************************************
 **************** CLOCK RECOVERY SYSTEM **********
 ************************************************/
/*
 * The default dco clock generators list
 * Calculated for 100MHz/100PPM dco reference clock
 * static WUFE_clk_rec_drc_divisor_list wufe_drc_diviso_default =
 * {
 *   {
 *     { 0x1f, 0x6227caf1 }, - T1
 *     { 0x17, 0x6a000000 }, - E1
 *     { 0x03, 0x787dfc56 },  - DS3
 *     { 0x04, 0xd1c206ac }   - E3
 *   }
 * };
 *
 * For extended differential clock operation mode the default values are:
 * (for 100MHz/100PPM DCO clock derive from the diff clock using dco_diff_ratio)
 * static WUFE_clk_rec_drc_divisor_list wufe_drc_diviso_default =
 * {
 *   {
 *     { 0x1f, 0x6227caf1 },  - T1
 *     { 0x17, 0x6a000000 },  - E1
 *     { 0x03, 0x787dfc56 },  - DS3
 *     { 0x04, 0xd1c206ac }   - E3
 *   }
 * };
 *
 */

typedef struct
{
      WP_U16                        max_rx_clk_rec_if;
      WP_U16                        max_tx_clk_rec_if;
      WP_U16                        max_ext_diff_rx_clk_rec_if;
      WP_U16                        max_ext_diff_tx_clk_rec_if;
      WP_U8                         method; /* Adap/Diff mode for regular CR (first 32 CD) */
      WUFE_clock_rec_sync           sync_mode;/*Select output sync is to the falling or rising edge*/
      WUFE_clock_rec_active         active_mode; /* Select output sync active low or active high */
      WUFE_clk_rec_drc_divisor_list *drc;  /* Reference clock for Digital Controlled Oscillator */
      WUFE_clk_rec_drc_divisor_list *ext_drc; /* Reference clock for DCO in extended differential */
} WUFE_clock_rec_system;  /* Relevant for UFE3 only */

typedef struct
{
      WP_U32         xo_ref_clk;  /* Not relevant in UFE3 */
}WUFE_clk_rec_system_params;

#endif /*WUFE_CLK_REC_H*/
