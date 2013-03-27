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

#ifndef WPX_TDM_COMET_H
#define WPX_TDM_COMET_H

#ifndef WPX_TDM_MODES_H
#include "wpx_tdm_modes.h"
#endif

#include "api/wp_types.h"
#include "wpx_board_data.h"

#define WPU_INIT_COMET_CPLD_CR_T1    0xfc
#define WPU_INIT_COMET_CPLD_CR_E1    0xfd
#define WPU_INIT_COMET_CPLD_T1       0xfe
#define WPU_INIT_COMET_CPLD_E1       0xff

/* This structure represents the registers used for Comet   */
/* initialization.                                          */
/* Note: when WPX_PHY_REG_MULTIPLIER is not 1               */
/* each 8-bit register resides on a 64-bit boundary.        */
/* There are 256 registers (256 x 8 = 2048).                */
typedef struct {
      WP_U8     reg[256*WPX_PHY_REG_MULTIPLIER];
} wpu_comet_phy;

typedef struct {
      WP_U8    reg[25*WPX_PHY_REG_MULTIPLIER];
} wpu_comet_cpld;

/* Register definitions used in configuration */
#define REG_INDEX(x) x*WPX_PHY_REG_MULTIPLIER

#define WPU_G_CFG       REG_INDEX(0x00)
#define WPU_CDCR_CFG    REG_INDEX(0x10)
#define WPU_RXELST_CFG  REG_INDEX(0x1c)
#define WPU_TXELST_CFG  REG_INDEX(0x20)
#define WPU_T1FRMR_CFG  REG_INDEX(0x48)
#define WPU_SIGX_CFG    REG_INDEX(0x50)
#define WPU_T1XBAS_CFG  REG_INDEX(0x54)
#define WPU_T1ALMI_CFG  REG_INDEX(0x60)
#define WPU_TPSC_CFG    REG_INDEX(0x6c)
#define WPU_RPSC_CFG    REG_INDEX(0x70)
#define WPU_CSU_CFG     REG_INDEX(0xd6)
#define WPU_XLPG_CFG    REG_INDEX(0xf0)

#define WPU_MAX_BUSY_READ 0x5
#define WPU_BIT_BUSY      0x80

#define WPU_PCCE_ENABLE 0x1
#define WPU_IND_ENABLE  0x2

typedef struct {
      wpu_comet_phy phy[8];
} wpu_comet;

WP_status WPU_EnableIndirectRegRW(wpu_comet_phy* phy, WP_U32 reg_type_address);
WP_status
WPU_ReadIndirectReg(wpu_comet_phy* phy, WP_U32 reg_type_address, WP_U32 offset, WP_U8* value);
WP_status
WPU_WriteIndirectReg(wpu_comet_phy* phy, WP_U32 reg_type_address, WP_U32 offset, WP_U8 value);
WP_status WPU_TdmCometCardInit(WP_U32 group, WP_U32 mode);
WP_status WPU_TdmCometRegMod(WP_U32 port, WP_U32 reg, WP_U8 value);
WP_status WPU_TdmCometRegRead(WP_U32 port, WP_U32 reg, WP_U8 *value);
WP_status WPU_TdmCpldRegMod(WP_U32 port, WP_U32 reg, WP_U8 value);
WP_status WPU_TdmCpldRegRead(WP_U32 port, WP_U32 reg, WP_U8 *reg_value);

#define WPU_INIT_ALL_PORTS 0xff

#endif
