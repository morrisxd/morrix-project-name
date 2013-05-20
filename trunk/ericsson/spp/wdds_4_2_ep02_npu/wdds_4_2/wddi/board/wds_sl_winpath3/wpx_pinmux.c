/*****************************************************************************
 * (C) Copyright 2000-2009, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: wpx_pinmux.c
 *
 *****************************************************************************/
#include <stdio.h>
#include <string.h>

#ifndef WP_TYPES_H
#include "api/wp_types.h"
#endif
#ifndef WPI_HW_H
#include <include/core/hardware/wpi_hw.h>
#endif
#ifndef WPX_BOARD_IF_H
#include "../board/wpx_board_if.h"
#endif

#include "wpx_pinmux.h"
#include "wpx_board_data.h"

wpx_pinmux pinmux_configurations[WPX_CONFIGURE_MAX_CONFIGURATIONS] = 
{
   /* WPX_CONFIGURE_DEFAULT_CHECKIN  */
   /* UPI1 - UPI3, level2 16 bit 256 phys*/
   {
      {

         /* pins 000-007 */0xFFFFFFFF,
         /* pins 008-015 */0xFFFFFFFF,
         /* pins 016-023 */0xFFFFFFFF,
         /* pins 024-031 */0xFFFFFFFF,
         /* pins 032-039 */0xFFFFFFFF,
         /* pins 040-047 */0xFFFFFFFF,
         /* pins 048-055 */0xFFFFFFFF,
         /* pins 056-063 */0xFFFFFFFF,
         /* pins 064-071 */0xFFFFFFFF,
         /* pins 072-079 */0xFFFFFFFF,
         /* pins 080-087 */0xFFFFFFFF,
         /* pins 088-095 */0xFFFFFFFF,
         /* pins 096-103 */0xFFFFFFFF,
         /* pins 104-111 */0xFFFFFFFF,
         /* pins 112-119 */0xFFFFFFFF,
         /* pins 120-127 */0xFFFFFFFF,
         /* pins 128-135 */0xFFFFFFFF,
         /* pins 136-143 */0xFFFFFFFF,
         /* pins 144-151 */0xFFFFFFFF,
         /* pins 152-159 */0xFFFFFFFF,
         /* pins 160-167 */0xFFFFFFFF,
         /* pins 168-175 */0xFFFFFFFF,
         /* pins 176-183 */0xFFFFFFFF,
         /* pins 184-191 */0xFFFFFFFF,
         /* pins 192-199 */0xFFFFFFFF,
         /* pins 200-207 */0xFFFFFFFF,
         /* pins 208-215 */0xFF111111,
         /* pins 216-223 */0xFFFFFF00
       }
   }, 
   /* WPX_CONFIGURE_CHECKIN_TDI_16             */
   /*  tdi 1 - 16 */
   {
      {
         /* pins 000-007 */0x88888888,
         /* pins 008-015 */0x88888888,
         /* pins 016-023 */0x88888888,
         /* pins 024-031 */0x88888888,
         /* pins 032-039 */0x88888888,
         /* pins 040-047 */0x88888888,
         /* pins 048-055 */0x88888888,
         /* pins 056-063 */0x88888888,
         /* pins 064-071 */0xFFFFFFFF,
         /* pins 072-079 */0xFFFFFFFF,
         /* pins 080-087 */0xFFFFFFFF,
         /* pins 088-095 */0xFFFFFFFF,
         /* pins 096-103 */0xFFFFFFFF,
         /* pins 104-111 */0xFFFFFFFF,
         /* pins 112-119 */0xFFFFFFFF,
         /* pins 120-127 */0xFFFFFFFF,
         /* pins 128-135 */0xFFFFFFFF,
         /* pins 136-143 */0xFFFFFFFF,
         /* pins 144-151 */0xFF888888,
         /* pins 152-159 */0x88888888,
         /* pins 160-167 */0x88888888,
         /* pins 168-175 */0x88888888,
         /* pins 176-183 */0x88888888,
         /* pins 184-191 */0x88888888,
         /* pins 192-199 */0x88888888,
         /* pins 200-207 */0x88888888,
         /* pins 208-215 */0x88111111,
         /* pins 216-223 */0xFFFFFF00
      }
   },
   /* WPX_CONFIGURE_UFE_UPI2_TDM               */
   {{0}},
   /* WPX_CONFIGURE_TDI_16_UPI1                */
   {{0}},
   /* WPX_CONFIGURE_2UPI_2XGI_6SGMII2G         */
   /* upi 1 - pos level 2 32 bit master 256 phys*/
   /* upi 2 - pos level 2 32 bit master 256 phys*/
   {
      {
         /* pins 000-007 */0x44444444,
         /* pins 008-015 */0x44444444,
         /* pins 016-023 */0x44444444,
         /* pins 024-031 */0x44444444,
         /* pins 032-039 */0x44444444,
         /* pins 040-047 */0x44444444,
         /* pins 048-055 */0x44444444,
         /* pins 056-063 */0x44444444,
         /* pins 064-071 */0x44444444,
         /* pins 072-079 */0x44444444,
         /* pins 080-087 */0x44444444,
         /* pins 088-095 */0x44444444,
         /* pins 096-103 */0x44444444,
         /* pins 104-111 */0x44444444,
         /* pins 112-119 */0x44444444,
         /* pins 120-127 */0x44444444,
         /* pins 128-135 */0x44444444,
         /* pins 136-143 */0x44444444,
         /* pins 144-151 */0x44444444,
         /* pins 152-159 */0x44444444,
         /* pins 160-167 */0x44444444,
         /* pins 168-175 */0x44444444,
         /* pins 176-183 */0x44444444,
         /* pins 184-191 */0x44444444,
         /* pins 192-199 */0x44444444,
         /* pins 200-207 */0x44444444,
         /* pins 208-215 */0x44FFFFFF,
         /* pins 216-223 */0xFFFFFF00
      }
   },
   /* WPX_CONFIGURE_1UPI_1XGI_4GMII_8SGMII     */
   /* enet 1-4 - RMII, upi 2 - pos level 2 256 phys*/
   {
      {
         /* pins 000-007 */0x0F000000,
         /* pins 008-015 */0x00000FF0,
         /* pins 016-023 */0x00000000,
         /* pins 024-031 */0x0F0F0000,
         /* pins 032-039 */0x0000000F,
         /* pins 040-047 */0xF0000000,
         /* pins 048-055 */0x000F0F00,
         /* pins 056-063 */0x00000000,
         /* pins 064-071 */0x0FF00000,
         /* pins 072-079 */0x00000F0F,
         /* pins 080-087 */0x00000000,
         /* pins 088-095 */0x000FF000,
         /* pins 096-103 */0x0000000F,
         /* pins 104-111 */0xFF444444,
         /* pins 112-119 */0x44444444,
         /* pins 120-127 */0x44444444,
         /* pins 128-135 */0x44444444,
         /* pins 136-143 */0x44444444,
         /* pins 144-151 */0x44444444,
         /* pins 152-159 */0x44444444,
         /* pins 160-167 */0x44444444,
         /* pins 168-175 */0x44444444,
         /* pins 176-183 */0x44444444,
         /* pins 184-191 */0x44444444,
         /* pins 192-199 */0x44444444,
         /* pins 200-207 */0x44444444,
         /* pins 208-215 */0x44FFFFFF,
         /* pins 216-223 */0xFFFFFF00
      }
   },
   /* WPX_CONFIGURE_1XGI_6RGMII_6SGMII    */
   /* enet 1-6 - RGMII, upi 2 - pos level 2 256 phys*/
   {
{
         /* pins 000-007 */0x33333333,
         /* pins 008-015 */0x3333F333,
         /* pins 016-023 */0x33333333,
         /* pins 024-031 */0x3F333333,
         /* pins 032-039 */0x333333F3,
         /* pins 040-047 */0x33333333,
         /* pins 048-055 */0x333F3333,
         /* pins 056-063 */0x33333333,
         /* pins 064-071 */0xF3333333,
         /* pins 072-079 */0x33333FFF,
         /* pins 080-087 */0xFFFFFFFF,
         /* pins 088-095 */0xFFFFFFFF,
         /* pins 096-103 */0xFFFFFFFF,
         /* pins 104-111 */0xFFffffff,
         /* pins 112-119 */0xffffffff,
         /* pins 120-127 */0xffffffff,
         /* pins 128-135 */0xfffff333,
         /* pins 136-143 */0x33333333,
         /* pins 144-151 */0x3f333333,
         /* pins 152-159 */0x333333f3,
         /* pins 160-167 */0x33333333,
         /* pins 168-175 */0x333f3333,
         /* pins 176-183 */0x33333333,
         /* pins 184-191 */0xf3333333,
         /* pins 192-199 */0x33333f33,
         /* pins 200-207 */0x33333333,
         /* pins 208-215 */0x33FFFFFF,
         /* pins 216-223 */0xFFFFFF00
      }
   },
   /* WPX_CONFIGURE_1UPI_1XGI_6RTBI_6SGMII     */
   /* enet 1-6 - RTDI, upi 2 - pos level 2 256 phys*/
   {
      {
         /* pins 000-007 */0x33333333,
         /* pins 008-015 */0x33333333,
         /* pins 016-023 */0x33333333,
         /* pins 024-031 */0x33333333,
         /* pins 032-039 */0x3333333F,
         /* pins 040-047 */0xFFFFFFFF,
         /* pins 048-055 */0xFFFF3333,
         /* pins 056-063 */0x33333333,
         /* pins 064-071 */0x33333333,
         /* pins 072-079 */0x333333FF,
         /* pins 080-087 */0xFFFFFFFF,
         /* pins 088-095 */0xFFFFFFFF,
         /* pins 096-103 */0xFFFFFFFF,
         /* pins 104-111 */0xFF444444,
         /* pins 112-119 */0x44444444,
         /* pins 120-127 */0x44444444,
         /* pins 128-135 */0x44444444,
         /* pins 136-143 */0x44444444,
         /* pins 144-151 */0x44444444,
         /* pins 152-159 */0x44444444,
         /* pins 160-167 */0x44444444,
         /* pins 168-175 */0x44444444,
         /* pins 176-183 */0x44444444,
         /* pins 184-191 */0x44444444,
         /* pins 192-199 */0x44444444,
         /* pins 200-207 */0x44444444,
         /* pins 208-215 */0x44FFFFFF,
         /* pins 216-223 */0xFFFFFF00
      }
   },
   /* WPX_CONFIGURE_1XGI_6SMII_6SGMII     */
   /* enet 1-12 - SMII or SS-SMII */
   {
      {
         /* pins 000-007 */0x11111111,
         /* pins 008-015 */0x11111111,
         /* pins 016-023 */0x11111111,
         /* pins 024-031 */0x11111111,
         /* pins 032-039 */0x11111111,
         /* pins 040-047 */0x11111111,
         /* pins 048-055 */0x11111111,
         /* pins 056-063 */0x11111111,
         /* pins 064-071 */0x11111111,
         /* pins 072-079 */0x11111111,
         /* pins 080-087 */0x11111111,
         /* pins 088-095 */0x11111111,
         /* pins 096-103 */0x11111111,
         /* pins 104-111 */0x11111111,
         /* pins 112-119 */0x11111111,
         /* pins 120-127 */0x11111111,
         /* pins 128-135 */0x11111111,
         /* pins 136-143 */0x11111111,
         /* pins 144-151 */0x11111111,
         /* pins 152-159 */0x11111111,
         /* pins 160-167 */0x11111111,
         /* pins 168-175 */0x11111111,
         /* pins 176-183 */0x11111111,
         /* pins 184-191 */0x11111111,
         /* pins 192-199 */0x11111111,
         /* pins 200-207 */0x11111111,
         /* pins 208-215 */0x11FFFFFF,
         /* pins 216-223 */0xFFFFFF00
      }
   },
   /* WPX_CONFIGURE_2RMII_8TDI_1XGI_10SGMII  */
   /* enet 1-2 - RMII, tdi 9-16 */
   {
      {
         /* pins 000-007 */0x22222222,
         /* pins 008-015 */0x22222222,
         /* pins 016-023 */0x22222222,
         /* pins 024-031 */0x22222222,
         /* pins 032-039 */0x22222222,
         /* pins 040-047 */0x22222222,
         /* pins 048-055 */0x22222222,
         /* pins 056-063 */0x22FFFFFF,
         /* pins 064-071 */0xFFFFFFFF,
         /* pins 072-079 */0xFFFFFFFF,
         /* pins 080-087 */0xFFFFFFFF,
         /* pins 088-095 */0xFFFFFFFF,
         /* pins 096-103 */0xFFFFFFFF,
         /* pins 104-111 */0xFFFFFFFF,
         /* pins 112-119 */0xFFFFFFFF,
         /* pins 120-127 */0xFFFFFFFF,
         /* pins 128-135 */0xFFFFFFFF,
         /* pins 136-143 */0xFFFFFFFF,
         /* pins 144-151 */0xFF888888,
         /* pins 152-159 */0x88888888,
         /* pins 160-167 */0x88888888,
         /* pins 168-175 */0x88888888,
         /* pins 176-183 */0x88888888,
         /* pins 184-191 */0x88888888,
         /* pins 192-199 */0x88888888,
         /* pins 200-207 */0x88888888,
         /* pins 208-215 */0x88FFFFFF,
         /* pins 216-223 */0xFFFFFF00
      }
   },
   /* WPX_CONFIGURE_8TDI_1UPI_1XGI_10SGMII  */
   /* tdi 1 - 8, upi 2 pos level 2 256 phys*/
   {
      {
         /* pins 000-007 */0x88888888,
         /* pins 008-015 */0x88888888,
         /* pins 016-023 */0x88888888,
         /* pins 024-031 */0x88888888,
         /* pins 032-039 */0x88888888,
         /* pins 040-047 */0x88888888,
         /* pins 048-055 */0x88888888,
         /* pins 056-063 */0x88888888,
         /* pins 064-071 */0xFFFFFFFF,
         /* pins 072-079 */0xFFFFFFFF,
         /* pins 080-087 */0xFFFFFFFF,
         /* pins 088-095 */0xFFFFFFFF,
         /* pins 096-103 */0xFFFFFFFF,
         /* pins 104-111 */0xFF444444,
         /* pins 112-119 */0x44444444,
         /* pins 120-127 */0x44444444,
         /* pins 128-135 */0x44444444,
         /* pins 136-143 */0x444444FF,
         /* pins 144-151 */0x44FFFF44,
         /* pins 152-159 */0x44444444,
         /* pins 160-167 */0x44444444,
         /* pins 168-175 */0x44444444,
         /* pins 176-183 */0x44444444,
         /* pins 184-191 */0x44444444,
         /* pins 192-199 */0x44444444,
         /* pins 200-207 */0x44444444,
         /* pins 208-215 */0x44FFFFFF,
         /* pins 216-223 */0xFFFFFF00
      }
   },
   /* WPX_CONFIGURE_2UPI_1XGI_10SGMII  */
   /* upi 1 and 2, pos level 2 256 phys 32 bit*/
   {
      {
         /* pins 000-007 */0x44444444,
         /* pins 008-015 */0x44444444,
         /* pins 016-023 */0x44444444,
         /* pins 024-031 */0x44444444,
         /* pins 032-039 */0x44444444,
         /* pins 040-047 */0x44444444,
         /* pins 048-055 */0x44444444,
         /* pins 056-063 */0x44444444,
         /* pins 064-071 */0x44444444,
         /* pins 072-079 */0x44444444,
         /* pins 080-087 */0x44444444,
         /* pins 088-095 */0x44444444,
         /* pins 096-103 */0x44444444,
         /* pins 104-111 */0xff444444,
         /* pins 112-119 */0x44444444,
         /* pins 120-127 */0x44444444,
         /* pins 128-135 */0x44444444,
         /* pins 136-143 */0x44444444,
         /* pins 144-151 */0x44444444,
         /* pins 152-159 */0x44444444,
         /* pins 160-167 */0x44444444,
         /* pins 168-175 */0x44444444,
         /* pins 176-183 */0x44444444,
         /* pins 184-191 */0x44444444,
         /* pins 192-199 */0x44444444,
         /* pins 200-207 */0x44444444,
         /* pins 208-215 */0x44FFFFFF,
         /* pins 216-223 */0xFFFFFF00
      }
   },
   /* WPX_CONFIGURE_2UPI_1XGI_10SGMII_UFE4  */
   {{0}},
   /* WPX_CONFIGURE_2UPI_16bit_1XGI_10SGMII  */
   {{0}},
   /*WPX_CONFIGURE_BRG_MUX*/
   {{0}},
   /*WPX_CONFIGURE_PPS_FCLK_SELECT*/
   {{0}},
   /*WPX_CONFIGURE_SMA_DIRECTION*/
   {{0}},
   /*WPX_CONFIGURE_2SGMII_2SGMII2G_16TDI*/
   {
      {
         /* pins 000-007 */0x88888888,
         /* pins 008-015 */0x88888888,
         /* pins 016-023 */0x88888888,
         /* pins 024-031 */0x88888888,
         /* pins 032-039 */0x88888888,
         /* pins 040-047 */0x88888888,
         /* pins 048-055 */0x88888888,
         /* pins 056-063 */0x88888888,
         /* pins 064-071 */0xFFFFFFFF,
         /* pins 072-079 */0xFFFFFFFF,
         /* pins 080-087 */0xFFFFFFFF,
         /* pins 088-095 */0xFFFFFFFF,
         /* pins 096-103 */0xFFFFFFFF,
         /* pins 104-111 */0xFFFFFFFF,
         /* pins 112-119 */0xFFFFFFFF,
         /* pins 120-127 */0xFFFFFFFF,
         /* pins 128-135 */0xFFFFFFFF,
         /* pins 136-143 */0xFFFFFFFF,
         /* pins 144-151 */0xFF888888,
         /* pins 152-159 */0x88888888,
         /* pins 160-167 */0x88888888,
         /* pins 168-175 */0x88888888,
         /* pins 176-183 */0x88888888,
         /* pins 184-191 */0x88888888,
         /* pins 192-199 */0x88888888,
         /* pins 200-207 */0x88888888,
         /* pins 208-215 */0x88111111,
         /* pins 216-223 */0xFFFFFF00
      }
   }
};

wpx_pinmux *WPX_PinmuxBaseGet(WP_U32 wpid)
{
   return (wpx_pinmux *) (WPX_AppWpIntRegBaseGet(wpid) + WPI_PINMUX_BASE);
}

WP_status WPX_PinMuxSet(WP_U32 wpid, wpx_board_configuration configuration)
{
   WP_U32 ii;
   wpx_pinmux *pinmux_reg;
   
   wpx_pinmux* data = &pinmux_configurations[configuration];
   
   pinmux_reg = WPX_PinmuxBaseGet(wpid);
 
   for (ii = 0; ii < WPX_PINMUX_REGS; ii++)
   {
      if(ii<WPX_PINMUX_REGS_UNUSED_START || ii>WPX_PINMUX_REGS_UNUSED_START)
         WPI_MEM_SET(pinmux_reg->pinmux_control[ii], data->pinmux_control[ii]);
   }
   
   return WP_OK;
}


WP_status WPX_PinMuxPullDownSet(WP_U32 wpid, WP_U32 port)
{
   wpx_pinmux *pinmux_reg;
   pinmux_reg = WPX_PinmuxBaseGet(wpid);
   WP_U32 reg = 0, temp=0;
   switch(port)
   {
      case WP_PORT_UPI1:
         WPI_REG_GET(reg, pinmux_reg->pinmux_pad_control[WPX_PINMUX_UPI1_SOC_PAD/2]);
         temp = WPX_PINMUX_WEAK_PULL_DOWN_OFFSET(WPX_PINMUX_UPI1_SOC_PAD % 2);
         reg |= 1<<temp;
         WPI_REG_SET(pinmux_reg->pinmux_pad_control[WPX_PINMUX_UPI1_SOC_PAD/2], reg);

         WPI_REG_GET(reg, pinmux_reg->pinmux_pad_control[WPX_PINMUX_UPI1_EOP_PAD/2]);
         temp = WPX_PINMUX_WEAK_PULL_DOWN_OFFSET(WPX_PINMUX_UPI1_EOP_PAD % 2);
         reg |= 1<<temp;
         WPI_REG_SET(pinmux_reg->pinmux_pad_control[WPX_PINMUX_UPI1_EOP_PAD/2], reg);

         WPI_REG_GET(reg, pinmux_reg->pinmux_pad_control[WPX_PINMUX_UPI1_ERR_PAD/2]);
         temp = WPX_PINMUX_WEAK_PULL_DOWN_OFFSET(WPX_PINMUX_UPI1_ERR_PAD % 2);
         reg |= 1<<temp;
         WPI_REG_SET(pinmux_reg->pinmux_pad_control[WPX_PINMUX_UPI1_ERR_PAD/2], reg);
         break;
      case WP_PORT_UPI2:
         WPI_REG_GET(reg, pinmux_reg->pinmux_pad_control[WPX_PINMUX_UPI2_SOC_PAD/2]);
         temp = WPX_PINMUX_WEAK_PULL_DOWN_OFFSET(WPX_PINMUX_UPI2_SOC_PAD % 2);
         reg |= 1<<temp | 0x001f001f;
         WPI_REG_SET(pinmux_reg->pinmux_pad_control[WPX_PINMUX_UPI2_SOC_PAD/2], reg);

         WPI_REG_GET(reg, pinmux_reg->pinmux_pad_control[WPX_PINMUX_UPI2_EOP_PAD/2]);
         temp = WPX_PINMUX_WEAK_PULL_DOWN_OFFSET(WPX_PINMUX_UPI2_EOP_PAD % 2);
         reg |= 1<<temp | 0x001f001f;
         WPI_REG_SET(pinmux_reg->pinmux_pad_control[WPX_PINMUX_UPI2_EOP_PAD/2], reg);

         WPI_REG_GET(reg, pinmux_reg->pinmux_pad_control[WPX_PINMUX_UPI2_ERR_PAD/2]);
         temp = WPX_PINMUX_WEAK_PULL_DOWN_OFFSET(WPX_PINMUX_UPI2_ERR_PAD % 2);
         reg |= 1<<temp;
         WPI_REG_SET(pinmux_reg->pinmux_pad_control[WPX_PINMUX_UPI2_ERR_PAD/2], reg);
         break;
      case WP_PORT_UPI3:
         WPI_REG_GET(reg, pinmux_reg->pinmux_pad_control[WPX_PINMUX_UPI3_SOC_PAD/2]);
         temp = WPX_PINMUX_WEAK_PULL_DOWN_OFFSET(WPX_PINMUX_UPI3_SOC_PAD % 2);
         reg |= 1<<temp;
         WPI_REG_SET(pinmux_reg->pinmux_pad_control[WPX_PINMUX_UPI3_SOC_PAD/2], reg);

         WPI_REG_GET(reg, pinmux_reg->pinmux_pad_control[WPX_PINMUX_UPI3_EOP_PAD/2]);
         temp = WPX_PINMUX_WEAK_PULL_DOWN_OFFSET(WPX_PINMUX_UPI3_EOP_PAD % 2);
         reg |= 1<<temp;
         WPI_REG_SET(pinmux_reg->pinmux_pad_control[WPX_PINMUX_UPI3_EOP_PAD/2], reg);

         WPI_REG_GET(reg, pinmux_reg->pinmux_pad_control[WPX_PINMUX_UPI3_ERR_PAD/2]);
         temp = WPX_PINMUX_WEAK_PULL_DOWN_OFFSET(WPX_PINMUX_UPI3_ERR_PAD % 2);
         reg |= 1<<temp;
         WPI_REG_SET(pinmux_reg->pinmux_pad_control[WPX_PINMUX_UPI3_ERR_PAD/2], reg);
         break;
      default:
         return WPI_ERROR(WP_ERR_CONFIG);
   }
   return WP_OK;
}

WP_status WPX_PinMuxTdiDriveStrength(WP_U32 wpid)
{

   /* This change is required in WP3 PWE3-CES TDI when CR is in use 
      The code decrease the current driven by the WP3 to 4mA on the TX clock and sync of lines 7 & 15
      the current decrease is required because in the WDS3 the TX clock and sync of lines 7 & 15 
      are also connected to the FPGA and create a returns which damage the falling edge of the clock  
      This change is related to the WDS3 board design                                               */

   wpx_pinmux *pinmux_reg;
   pinmux_reg = WPX_PinmuxBaseGet(wpid);
   WP_U32 reg = 0;

   /* Set PM_SP_CTRL_x_y - change the Drive Strength of SPxxx (TDIx_TX_CLK & TDIx_TX_SYNC) to 4mA */
   WPI_REG_SET(pinmux_reg->pinmux_pad_control[WPX_PINMUX_TDI0_CLK_SYNC_PAD/2], reg);
   WPI_REG_SET(pinmux_reg->pinmux_pad_control[WPX_PINMUX_TDI1_CLK_SYNC_PAD/2], reg);
   WPI_REG_SET(pinmux_reg->pinmux_pad_control[WPX_PINMUX_TDI2_CLK_SYNC_PAD/2], reg);
   WPI_REG_SET(pinmux_reg->pinmux_pad_control[WPX_PINMUX_TDI3_CLK_SYNC_PAD/2], reg);
   WPI_REG_SET(pinmux_reg->pinmux_pad_control[WPX_PINMUX_TDI4_CLK_SYNC_PAD/2], reg);
   WPI_REG_SET(pinmux_reg->pinmux_pad_control[WPX_PINMUX_TDI5_CLK_SYNC_PAD/2], reg);
   WPI_REG_SET(pinmux_reg->pinmux_pad_control[WPX_PINMUX_TDI6_CLK_SYNC_PAD/2], reg);
   WPI_REG_SET(pinmux_reg->pinmux_pad_control[WPX_PINMUX_TDI7_CLK_SYNC_PAD/2], reg);

   /* Leave #8 out. It cause LSS errors on that line, if clearing the register. */
   /* WPI_REG_SET(pinmux_reg->pinmux_pad_control[WPX_PINMUX_TDI8_CLK_SYNC_PAD/2], reg); */
   WPI_REG_SET(pinmux_reg->pinmux_pad_control[WPX_PINMUX_TDI9_CLK_SYNC_PAD/2], reg);
   WPI_REG_SET(pinmux_reg->pinmux_pad_control[WPX_PINMUX_TDI10_CLK_SYNC_PAD/2], reg);
   WPI_REG_SET(pinmux_reg->pinmux_pad_control[WPX_PINMUX_TDI11_CLK_SYNC_PAD/2], reg);
   WPI_REG_SET(pinmux_reg->pinmux_pad_control[WPX_PINMUX_TDI12_CLK_SYNC_PAD/2], reg);
   WPI_REG_SET(pinmux_reg->pinmux_pad_control[WPX_PINMUX_TDI13_CLK_SYNC_PAD/2], reg);
   WPI_REG_SET(pinmux_reg->pinmux_pad_control[WPX_PINMUX_TDI14_CLK_SYNC_PAD/2], reg);
   WPI_REG_SET(pinmux_reg->pinmux_pad_control[WPX_PINMUX_TDI15_CLK_SYNC_PAD/2], reg);

   return WP_OK;
}






WP_status WPX_PinMuxSetMgmtWinnet(WP_U32 wpid, WP_U32 mgmt_winnet)
{
   /*per mgmt_winnet 2 pins 102 and 103 should be set to 1*/
   /*per mgmt_winnet 3 pins 106 and 107 should be set to 1*/

   WP_U32 offset = 0,  mask=0, value=0;
   WP_U32 pinmux_value;
   wpx_pinmux *pinmux_reg = WPX_PinmuxBaseGet(wpid);
   if(mgmt_winnet == 2)
   {
    /*winnet2 managment configuration:
      pins 102 and 103 should be set to 1: 
      102/8 = 12, 102%8 = 6 103/8 = 12 103%8 = 7,
      so fields 6 and 7 at offset 12 should be updated*/
      offset= 12;
      mask = 0x000000ff;
      value =  0x00000011;
   }
   else if(mgmt_winnet == 3)
   {
   /*winnet3 managment configuration:
     pins 106 and 107 should be set to 1: 
     106/8 = 13, 106%8 = 2 107/8 = 13 107%8 = 3,
     so fields 2 and 3 at offset 13 should be updated*/
      offset= 13;
      mask  = 0x00ff0000;
      value = 0x00110000;
   }
   else
      return WPI_ERROR(WP_ERR_WDDI);
   
   WPI_MEM_GET(pinmux_value, pinmux_reg->pinmux_control[offset]);
   pinmux_value &= ~mask;
   pinmux_value |= value;
   WPI_MEM_SET(pinmux_reg->pinmux_control[offset],pinmux_value);
   return WP_OK;
}

WP_status WPX_PinMuxPadControlSet(WP_U32 wpid)
{
   wpx_pinmux *pinmux_reg;
   WP_U32 i;

   pinmux_reg = WPX_PinmuxBaseGet(wpid);

   for (i=0; i < WPX_PINMUX_PADS/2; i++)
   {
      WPI_REG_SET(pinmux_reg->pinmux_pad_control[i], 0x00920092);
   }

   return WP_OK;
}
