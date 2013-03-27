/*****************************************************************************
 * (C) Copyright 2001-2006, Wintegra.  All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/************************************************************************/
/*                      TDM CARD INITIALIZATION                         */
/************************************************************************/

/* This initialization code assumes that the host port for the QuadFALC
   is implemented as a 64-bit port. */

/* Note: This is a preliminary release, which assumes a default memory map. */

#ifndef WPX_TDM_QFALC_H
#define WPX_TDM_QFALC_H

#include "api/wp_types.h"

/* Defined modes of operation */
#define WPU_MODE_TDM_CLOCK_8MHZ   0
#define WPU_MODE_TDM_CLOCK_16MHZ  1
#define WPU_MODE_TDM_E1           2
#define WPU_MODE_TDM_E1_S         3
#define WPU_MODE_TDM_E1_X         4
#define WPU_MODE_TDM_E1_MASTER    5
#define WPU_MODE_TDM_E1_SLAVE     6
#define WPU_MODE_TDM_T1           7
#define WPU_MODE_TDM_T1_S         8
#define WPU_MODE_TDM_T1_MASTER    9
#define WPU_MODE_TDM_T1_SLAVE     10

/* Defined modes of operation for Transparent protocol  */
#define WPU_MODE_TDM_TRANS_E1_MASTER            11
#define WPU_MODE_TDM_TRANS_E1_SLAVE             12
#define WPU_MODE_TDM_TRANS_E1_FRAMELESS_MASTER  13
#define WPU_MODE_TDM_TRANS_E1_FRAMELESS_SLAVE   14
#define WPU_MODE_TDM_TRANS_E1_CAS_MASTER        15
#define WPU_MODE_TDM_TRANS_E1_CAS_SLAVE         16
#define WPU_MODE_TDM_TRANS_T1_MASTER            17
#define WPU_MODE_TDM_TRANS_T1_SLAVE             18
#define WPU_MODE_TDM_TRANS_T1_FRAMELESS_MASTER  19
#define WPU_MODE_TDM_TRANS_T1_FRAMELESS_SLAVE   20
#define WPU_MODE_TDM_TRANS_T1_CAS_MASTER        21
#define WPU_MODE_TDM_TRANS_T1_CAS_SLAVE         22

#define WPU_MODE_TDM_UFE_E1_FRAMED              23
#define WPU_MODE_TDM_UFE_E1_UNFRAMED            24
#define WPU_MODE_TDM_UFE_T1_FRAMED              25
#define WPU_MODE_TDM_UFE_T1_UNFRAMED            26

#define WPU_MODE_TDM_UFE_E1_FRAMED_EXT_CLK      27
#define WPU_MODE_TDM_UFE_E1_UNFRAMED_EXT_CLK    28
#define WPU_MODE_TDM_UFE_T1_FRAMED_EXT_TX_CLK   29
#define WPU_MODE_TDM_UFE_T1_UNFRAMED_EXT_CLK    30

#define WPU_MODE_TDM_UFE_E1_SLAVE_CAS           31
#define WPU_MODE_TDM_UFE_TRANS_T1_CAS_MASTER    32
#define WPU_MODE_TDM_UFE_TRANS_T1_CAS_SLAVE     33

#define WPU_MODE_TDM_TRANS_E1_MASTER_4M         34
#define WPU_MODE_TDM_TRANS_E1_MASTER_8M         35

/* This structure includes the registers used in this initialization.
   It is not a complete representation of a QuadFALC register set. */
typedef struct {
      WP_U8     reserved0[0x10];
      WP_U8     cmdr;                        /* 0x10 */
      WP_U8     reserved0_1[0x4f];
      WP_U8     rtr1;                        /* 0x60 */
      WP_U8     reserved0_2[0x7];
      WP_U8     rtr2;                        /* 0x68 */
      WP_U8     reserved0_3[0x7];
      WP_U8     rtr3;                        /* 0x70 */
      WP_U8     reserved0_4[0x7];
      WP_U8     rtr4;                        /* 0x78 */
      WP_U8     reserved1[0x7];
      WP_U8     ttr1;                        /* 0x80 */
      WP_U8     reserved2[0x7];
      WP_U8     ttr2;                        /* 0x88 */
      WP_U8     reserved3[0x7];
      WP_U8     ttr3;                        /* 0x90 */
      WP_U8     reserved4[0x7];
      WP_U8     ttr4;                        /* 0x98 */
      WP_U8     reserved5[0x47];
      WP_U8     fmr0;                        /* 0xe0 */
      WP_U8     reserved6[0x7];
      WP_U8     fmr1;                        /* 0xe8 */
      WP_U8     reserved7[0x7];
      WP_U8     fmr2;                        /* 0xf0 */
      WP_U8     reserved8[0x7];
      WP_U8     loop;
      WP_U8     reserved8_1[0x7];
      WP_U8     xsw;                         /* 0x100 */
      WP_U8     reserved9[0x7];
      WP_U8     xsp;                         /* 0x108 */
      WP_U8     reserved10[0x7];
      WP_U8     xc0;                         /* 0x110 */
      WP_U8     reserved11[0x7];
      WP_U8     xc1;                         /* 0x118 */
      WP_U8     reserved12[0x7];
      WP_U8     rc0;                         /* 0x120 */
      WP_U8     reserved13[0x7];
      WP_U8     rc1;                         /* 0x128 */
      WP_U8     reserved14[0x7];
      WP_U8     xpm0;                        /* 0x130 */
      WP_U8     reserved15[0x7];
      WP_U8     xpm1;                        /* 0x138 */
      WP_U8     reserved16[0x7];
      WP_U8     xpm2;                        /* 0x140 */
      WP_U8     reserved17[0x6f];
      WP_U8     lim0;                        /* 0x1b0 */
      WP_U8     reserved18[0x7];
      WP_U8     lim1;                        /* 0x1b8 */
      WP_U8     reserved19[0x7];
      WP_U8     pcd;                         /* 0x1c0 */
      WP_U8     reserved20[0x7];
      WP_U8     pcr;                         /* 0x1c8 */
      WP_U8     reserved21[0x7];
      WP_U8     lim2;                        /* 0x1d0 */
      WP_U8     reserved22[0x1f];
      WP_U8     sic1;                        /* 0x1f0 */
      WP_U8     reserved23[0x7];
      WP_U8     sic2;                        /* 0x1f8 */
      WP_U8     reserved24[0x7];
      WP_U8     sic3;                        /* 0x200 */
      WP_U8     reserved24point5[0x1f];
      WP_U8     cmr1;                        /* 0x228 */
      WP_U8     reserved25[0x7];
      WP_U8     cmr2;                        /* 0x228 */
      WP_U8     reserved26[0x1d7];
      WP_U8     pc1;                         /* 0x400 */
      WP_U8     reserved27[0x7];
      WP_U8     pc2;                         /* 0x408 */
      WP_U8     reserved28[0x7];
      WP_U8     pc3;                         /* 0x410 */
      WP_U8     reserved29[0x7];
      WP_U8     pc4;                         /* 0x418 */
      WP_U8     reserved29a[0x7];
      WP_U8     pc5;                         /* 0x420 */
      WP_U8     reserved30[0x6f];
      WP_U8     gcm1;                        /* 0x490 */
      WP_U8     reserved31[0x7];
      WP_U8     gcm2;                        /* 0x498 */
      WP_U8     reserved32[0x7];
      WP_U8     gcm3;                        /* 0x4a0 */
      WP_U8     reserved33[0x7];
      WP_U8     gcm4;                        /* 0x4a8 */
      WP_U8     reserved34[0x7];
      WP_U8     gcm5;                        /* 0x4b0 */
      WP_U8     reserved35[0x7];
      WP_U8     gcm6;                        /* 0x4b8 */
      WP_U8     reserved36[0x347];
} wpu_qfalc_phy;

typedef struct {
      wpu_qfalc_phy phy[4];
} wpu_qfalc;

WP_status WPU_TdmQfalcCardInit(WP_U32 group, WP_U32 mode);
WP_status WPU_TdmQfalcCardInitBim(WP_U32 port, WP_U32 mode);

#endif
