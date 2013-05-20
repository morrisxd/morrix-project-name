/*****************************************************************************
 * (C) Copyright 2000-2005, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: wpx_pinmux.h
 *
 * Purpose: pinmux registers initialization for wp3 
 *
 ****************************************************************************/
#ifndef WPX_PINMUX_H
#define WPX_PINMUX_H

#define WPX_PINMUX_PADS 222
#define WPX_PINMUX_REGS 28
#define WPX_PINMUX_REGS_UNUSED_START 9
#define WPI_PINMUX_REGS_UNUSED_STOP  16
typedef enum
{
   WPX_PINMUX_ENET_GMII = 0,
   WPX_PINMUX_ENET_SMII_SSSMII = 1,
   WPX_PINMUX_ENET_RMII = 2,
   WPX_PINMUX_ENET_RGMII_RTBI = 3,
   WPX_PINMUX_UPI_LEVEL2 = 4,
   WPX_PINMUX_UPI_LEVEL3_16_32BIT = 5,
   WPX_PINMUX_UPI_LEVEL3_8BIT = 6,
   WPX_PINMUX_SERDES = 7,
   WPX_PINMUX_TDI = 8,
   WPX_PINMUX_ASRAM = 9,
   WPX_PINMUX_EMPHY_UPI0_UPI1 = 10
} WPX_PINMUX_CONFIG;

typedef struct
{
      WP_U32 pinmux_control[WPX_PINMUX_REGS]; /* offset 0x0 - 0x6c*/
      WP_U32 reserved[4];
      WP_U32 pinmux_pad_control[WPX_PINMUX_PADS/2];
      
} wpx_pinmux;
#define WPX_PINMUX_WEAK_PULL_DOWN_OFFSET(n) (7+16*(1-n))
#define WPX_PINMUX_DRIVE_STRENGTH_OFFSET(n) (16*(1-n))

#define WPX_PINMUX_UPI1_SOC_PAD 16
#define WPX_PINMUX_UPI2_SOC_PAD 193
#define WPX_PINMUX_UPI3_SOC_PAD 186
#define WPX_PINMUX_UPI1_EOP_PAD 28
#define WPX_PINMUX_UPI2_EOP_PAD 181
#define WPX_PINMUX_UPI3_EOP_PAD 90
#define WPX_PINMUX_UPI1_ERR_PAD 29
#define WPX_PINMUX_UPI2_ERR_PAD 180
#define WPX_PINMUX_UPI3_ERR_PAD 89

#define WPX_PINMUX_TDI0_CLK_SYNC_PAD   (42)  /* PM_SP_CTRL_042_043 */
#define WPX_PINMUX_TDI1_CLK_SYNC_PAD   (36)  /* PM_SP_CTRL_036_037 */
#define WPX_PINMUX_TDI2_CLK_SYNC_PAD   (30)  /* PM_SP_CTRL_030_031 */
#define WPX_PINMUX_TDI3_CLK_SYNC_PAD   (24)  /* PM_SP_CTRL_024_025 */
#define WPX_PINMUX_TDI4_CLK_SYNC_PAD   (18)  /* PM_SP_CTRL_018_019 */
#define WPX_PINMUX_TDI5_CLK_SYNC_PAD   (12)  /* PM_SP_CTRL_012_013 */
#define WPX_PINMUX_TDI6_CLK_SYNC_PAD   ( 6)  /* PM_SP_CTRL_006_007 */
#define WPX_PINMUX_TDI7_CLK_SYNC_PAD   ( 0)  /* PM_SP_CTRL_000_001 */

#define WPX_PINMUX_TDI8_CLK_SYNC_PAD  (166)  /* PM_SP_CTRL_166_167 */
#define WPX_PINMUX_TDI9_CLK_SYNC_PAD  (172)  /* PM_SP_CTRL_172_173 */
#define WPX_PINMUX_TDI10_CLK_SYNC_PAD (178)  /* PM_SP_CTRL_178_179 */
#define WPX_PINMUX_TDI11_CLK_SYNC_PAD (184)  /* PM_SP_CTRL_184_185 */
#define WPX_PINMUX_TDI12_CLK_SYNC_PAD (190)  /* PM_SP_CTRL_190_191 */
#define WPX_PINMUX_TDI13_CLK_SYNC_PAD (196)  /* PM_SP_CTRL_196_197 */
#define WPX_PINMUX_TDI14_CLK_SYNC_PAD (202)  /* PM_SP_CTRL_202_203 */
#define WPX_PINMUX_TDI15_CLK_SYNC_PAD (208)  /* PM_SP_CTRL_208_209 */


WP_status WPX_PinMuxSet(WP_U32 wpid, wpx_board_configuration configuration);
WP_status WPX_PinMuxPullDownSet(WP_U32 wpid, WP_U32 port);
WP_status WPX_PinMuxTdiDriveStrength(WP_U32 wpid);
WP_status WPX_PinMuxSetMgmtWinnet(WP_U32 wpid, WP_U32 mgmt_winnet);
WP_status WPX_PinMuxPadControlSet(WP_U32 wpid);
#endif
