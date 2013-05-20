/*****************************************************************************
 * (C) Copyright 2001-2005, Wintegra.  All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/************************************************************************/
/*                      GEI PHY INITIALIZATION                        */
/************************************************************************/

/* Note: This is a preliminary release, which assumes a default memory map. */
#include <stdio.h>


#ifndef WP_TYPES_H
#include "api/wp_types.h"
#endif
#ifndef WP_WDDI_H
#include "api/wp_wddi.h"
#endif
#ifndef WP_PROTOTYPES_H
#include "api/wp_prototypes.h"
#endif
#ifndef WPI_REG_H
#include "include/core/hardware/wpi_reg.h"
#endif
#ifndef WPI_REGISTRY_H
#include "include/core/registry/wpi_registry.h"
#endif
#ifndef WPI_REG_ENET_H
#include "include/core/hardware/wpi_reg_enet.h"
#endif
#ifndef WPX_ENET_PHY_H
#include "wpx_enet_phy.h"
#endif
#ifndef WPL_OS_ENET_H
#include "veneer/wpv_os_enet.h"
#endif
#ifndef WPX_PHY_DATA_H
#include "wpx_phy_data.h"
#endif
#ifndef WPI_HW_WINNET_H
#include "hardware/winpath2/wpi_hw_winnet.h"
#endif
#ifndef WPI_REG_GPIO_H
#include "hardware/winpath2/wpi_hw_reg_gpio.h"
#endif

#define WPU_WAIT_TIME 5000000
#define WPU_WAIT_ON_MIND_REG(x,y) do{WPI_REG_GET(x,y);}while( x != 0 );

static void WPI_GpRegUpdate(wpi_gpio* gpreg,
                            GPIO_PORT port_num,
                            WP_U32 value,
                            WP_U32 function)
{
   WP_U32 tmp_gpcr, tmp_gpcm1r, tmp_gpcm2r;
   wpi_gpio* tmp_gpreg;

   tmp_gpreg = (wpi_gpio *)((WP_U32 *)gpreg + WPI_GPCR_OFFSET(port_num));

   WPI_REG_GET(tmp_gpcr, tmp_gpreg->gpcr_a);
   WPI_REG_GET(tmp_gpcm1r, tmp_gpreg->gpcm1r_a);
   WPI_REG_GET(tmp_gpcm2r, tmp_gpreg->gpcm2r_a);

   switch (function)
   {
      case 0:
         break;
      case 1:
         tmp_gpcr |= value;
         tmp_gpcm1r &= ~value;
         tmp_gpcm2r &= ~value;
         break;
      case 2:
         tmp_gpcr &= ~value;
         tmp_gpcm1r |= value;
         tmp_gpcm2r &= ~value;
         break;
      case 3:
         tmp_gpcr |= value;
         tmp_gpcm1r |= value;
         tmp_gpcm2r &= ~value;
         break;
      case 4:
         tmp_gpcr &= ~value;
         tmp_gpcm1r &= ~value;
         tmp_gpcm2r |= value;
         break;
   }

   WPI_REG_SET(tmp_gpreg->gpcr_a, tmp_gpcr);
   WPI_REG_SET(tmp_gpreg->gpcm1r_a, tmp_gpcm1r);
   WPI_REG_SET(tmp_gpreg->gpcm2r_a, tmp_gpcm2r);
}

WP_status WPU_EnetPinMuxAllocateToPort(WP_U32 port, WP_U32 mode)
{
   WP_U32 wpid=0;
   wpi_gpio *gpreg = WPI_GPIO(WPX_AppWpIntRegBaseGet(wpid));
/*  volatile WP_U32 *board_mux = (WP_U32 *)WPL_PHY_VIRTUAL(0, WPX_MapGsBoardMuxGet());*/
   WP_U32 winnet = 0, group = 0;
   WP_U32 gpio, gpcr, temp_val;

   WP_U32 slot = mode & WPU_SLOT_MASK;
   WP_U32 major_mode = mode & WPU_MAJOR_MODE_MASK;

   switch (port)
   {
      case WP_PORT_ENET1:
         break;
      case WP_PORT_ENET2:
         group = 1;
         break;
      case WP_PORT_ENET3:
         winnet = 1;
         break;
      case WP_PORT_ENET4:
         group = 1;
         winnet = 1;
         break;
      default:
         return 0xffffffffU;
   }
   gpio = (winnet == 1) ? ((slot == WPU_SLOT_UPI1) ? GPIO_D : GPIO_B) :
      ((slot == WPU_SLOT_UPI1) ? GPIO_C : GPIO_A);
   gpcr = (winnet == 1) ? GPIO_H:  GPIO_G;

   switch (major_mode)
   {
      case WPU_MODE_GMII:
         if (group == 0)
         {
            WPI_GpRegUpdate(gpreg, gpcr, 0xFFCBFFC0, 1);
         }
         WPI_GpRegUpdate(gpreg, gpio, 0x5FFC3FF8, 2);
         break;
      case WPU_MODE_TBI:
         if (group == 0)
         {
            WPI_GpRegUpdate(gpreg, gpcr, 0xFFEFFFE0, 1);
         }
         WPI_GpRegUpdate(gpreg, gpio, 0x7FFE3FFC, 2);
         break;
      case WPU_MODE_MII:
         if (group == 0)
         {
            WPI_GpRegUpdate(gpreg, gpcr, 0xF0F1E1E0, 1);
         }
         WPI_GpRegUpdate(gpreg, gpio, 0x6F0F3C30, 2);
         break;
      case WPU_MODE_RMII:
         /* affects BRG bits!!!! */
         if (group == 0)
         {
            WPI_GpRegUpdate(gpreg, gpcr, 0xFFFFFF97, 1);
         }
         WPI_GpRegUpdate(gpreg, gpio, 0x9FFFFFFE, 2);
         break;
      case WPU_MODE_SS_SMII:
         if (group == 0)
         {
            WPI_GpRegUpdate(gpreg, gpcr, 0xF083E1C0, 1);
         }
         WPI_GpRegUpdate(gpreg, gpio, 0x5FF83FF0, 2);
         break;
      case WPU_MODE_SMII:
         if (group == 0)
         {
            WPI_GpRegUpdate(gpreg, gpcr, 0xF003E100, 1);
         }
         WPI_GpRegUpdate(gpreg, gpio, 0x5FF83FF0, 2);
         break;

      /**** ON BOARD ****/
      case WPU_MODE_RGMII:
/*         printf("RGMII: gpcr = %d\n", gpcr);

         temp_val = (group == 0) ? 0xF80BF000 : 0x07C40FC0;
         WPI_GpRegUpdate(gpreg, gpcr, temp_val, 1);*/
         break;
      case WPU_MODE_RTBI:
         temp_val = (group == 0) ? 0xF82BF000 : 0x07D40FC0;
         WPI_GpRegUpdate(gpreg, gpcr, temp_val, 1);
         break;
      default:
         break;
   }

   return WP_OK;
}


WP_status WPU_EnetPinMuxReset(void)
{
   WP_U32 wpid=0;
   wpi_gpio *gpreg = WPI_GPIO(WPX_AppWpIntRegBaseGet(wpid));

   /* WinNet 1 */
   WPI_REG_SET(gpreg->gpcr_b, 0xffffffff);
   WPI_REG_SET(gpreg->gpcm1r_b, 0x00000000);
   WPI_REG_SET(gpreg->gpcm2r_b, 0x00000000);
   /* WinNet 2 */
   WPI_REG_SET(gpreg->gpcr_a, 0xffffffff);
   WPI_REG_SET(gpreg->gpcm1r_a, 0x00000000);
   WPI_REG_SET(gpreg->gpcm2r_a, 0x00000000);
   /* WinNet 1 */
   WPI_REG_SET(gpreg->gpcr_d, 0xffffffff);
   WPI_REG_SET(gpreg->gpcm1r_d, 0x00000000);
   WPI_REG_SET(gpreg->gpcm2r_d, 0x00000000);
   /* WinNet 2 */
   WPI_REG_SET(gpreg->gpcr_c, 0xffffffff);
   WPI_REG_SET(gpreg->gpcm1r_c, 0x00000000);
   WPI_REG_SET(gpreg->gpcm2r_c, 0x00000000);

   WPI_REG_SET(gpreg->gpcr_e, 0xffffffff);
   WPI_REG_SET(gpreg->gpcm1r_e, 0x00000000);
   WPI_REG_SET(gpreg->gpcm2r_e, 0x00000000);

   WPI_REG_SET(gpreg->gpcr_f, 0xffffffff);
   WPI_REG_SET(gpreg->gpcm1r_f, 0x00000000);
   WPI_REG_SET(gpreg->gpcm2r_f, 0x00000000);
   /* RMII GPIO */
   WPI_REG_SET(gpreg->gpcr_g, 0xffcfffc0);
   WPI_REG_SET(gpreg->gpcm1r_g, 0x0000001f);
   WPI_REG_SET(gpreg->gpcm2r_g, 0x00000000);
   /* init GPIO for WinNet 2 and MDC/MDIO */
   WPI_REG_SET(gpreg->gpcr_h, 0xffcfffdf);
   WPI_REG_SET(gpreg->gpcm1r_h, 0x00000007);
   WPI_REG_SET(gpreg->gpcm2r_h, 0x00000000);

   return WP_OK;
}

/* works only for WinNet _top_ cards and OC12 Oc3 */
WP_status WPU_EnetSlotSelect(WP_U32 port, WP_U32 mode)
{
   WP_U32 slot =  mode & WPU_SLOT_MASK;
   wpx_cpld *cpld = (wpx_cpld *)WPL_PHY_VIRTUAL(0, CPLD_FIBER_EN);

   wpx_cpld_sec *cpld2  = (wpx_cpld_sec *) WPL_PHY_VIRTUAL(0, CPLD_JTAG);
   WP_U32 temp_reg;

   WP_U32 reset_mask;
   reset_mask = 0xFFFFFFFF;

   switch (slot)
   {
      case WPU_SLOT_UPI1:
         reset_mask = 0xFDFFFFFF;
         break;
      case WPU_SLOT_UPI2:
         reset_mask = 0xF7FFFFFF;
         break;
      case WPU_SLOT_TDI:
         reset_mask = 0xDFFFFFFF;
         /* CPLD mux for WinNet card on TDI Mux */
         WPI_MEM_SET(cpld->card_ctl, 0x87FFFFFF);
         WPL_Delay(1000);
         break;
      case WPU_SLOT_WNET:
         reset_mask = 0x7FFFFFFF;
         break;
      default:
         break;
   }
   WPI_MEM_GET(temp_reg, cpld2->ser_ctl);
   temp_reg &= reset_mask;
   WPI_MEM_SET(cpld2->ser_ctl, temp_reg);
   WPL_Delay(10000000);
   temp_reg |= ~reset_mask;
   WPI_MEM_SET(cpld2->ser_ctl, temp_reg);
   WPL_Delay(10000000);

   return WP_OK;
}

WP_U32 WPU_EnetMediaSelect(WP_U32 port, WP_U32 mode)
{
   WP_U32 media = mode & WPU_MEDIA_MASK;
   wpx_cpld_sec *cpld2 = (wpx_cpld_sec *) WPL_PHY_VIRTUAL(0, CPLD_JTAG);

   switch(media)
   {
      case WPU_OV_AUTODETECT:
         /* doesn't work yet */
      case WPU_OV_FIBER:
         WPI_MEM_SET(cpld2->tx_disable, 0);
         WPL_Delay(1000);
      default:
         break;
   }
   return WP_OK;
}


WP_status WPU_EnetPhyInit(WP_U32 port, WP_U32 mode)
{
   WP_U32 wpid=0;
   WP_U32 major_mode = mode & WPU_MAJOR_MODE_MASK;
   WP_U32 winnet = 0, group = 0;
   WP_U32 phy_num = 0, mind =0;
   wpi_winnet *hw_winnet;
   WP_status status = WP_OK, lock_status = WP_OK;
   wpi_res_registry *wpi_registry_ptr = NULL;

   switch (port)
   {
      case WP_PORT_ENET1:
         break;
      case WP_PORT_ENET2:
         phy_num = 1;
         group = 1;
         break;
      case WP_PORT_ENET3:
         phy_num = 2;
         winnet= 1;
         break;
      case WP_PORT_ENET4:
         phy_num = 3;
         group = 1;
         winnet = 1;
         break;
      default:
         return 0xffffffffU;
   }
   WPI_REGISTRY_GLOBAL_LOCK_GET(lock_status, wpi_registry_ptr);
   WPI_RETURN_IF_ERROR_STATUS(lock_status);
   /* if SED is active - SED uses the first Enet port and there is
      no need to change the pinmux configuration of the first port */
   if (wpid == WPI_OS_ENET_WINPATH &&
       wpi_registry_ptr->os_enet->enet_usage == WPL_os_enet_keepalive &&
       port == WPI_SED_PORT)
      return WPI_ERROR(WP_ERR_ENET_PORT_IN_USE_BY_BSP);

   hw_winnet = WPI_WINNET((WP_U32)WPL_RegBaseGet(0), 0);

   WPI_REG_SET(hw_winnet->mgmt.mcfg, 0x00000007);
   WPI_REG_SET(hw_winnet->mgmt.mcmd, 0x0000);

   /* 1. PinMux allocate
      2. Select slot
      3. Select media
   */
   switch (major_mode)
   {
      /* ss-smii card */
      case WPU_MODE_SMII:
      case WPU_MODE_SS_SMII:
      case WPU_MODE_RMII:
         WPI_RETURN_IF_ERROR(status,
                             WPU_EnetPinMuxAllocateToPort(port, mode));
         WPI_RETURN_IF_ERROR(status,
                             WPU_EnetSlotSelect(port, mode));
         break;
         /* mii card */
      case WPU_MODE_MII:
      case WPU_MODE_GMII:
      case WPU_MODE_TBI:
      case WPU_MODE_TBI_AN:
         WPI_RETURN_IF_ERROR(status,
                             WPU_EnetPinMuxAllocateToPort(port, mode));
         WPI_RETURN_IF_ERROR(status,
                             WPU_EnetSlotSelect(port, mode));
         WPI_RETURN_IF_ERROR(status,
                             WPU_EnetMediaSelect(port, mode));
/*         WPU_EnetResetPhys();*/
         break;
      /* on board */
      case WPU_MODE_RGMII:
      case WPU_MODE_RTBI:
         WPI_RETURN_IF_ERROR(status,
           WPU_EnetPinMuxAllocateToPort(port, mode));

         WPI_RETURN_IF_ERROR(status,
                             WPU_EnetMediaSelect(port, mode));
/*         WPU_EnetResetPhys(); */
         /* init mdio */
         WPI_REG_SET(hw_winnet->mgmt.mcfg, 0x00000007);
         WPI_REG_SET(hw_winnet->mgmt.mcmd, 0x0000);
         /* set extended regs */
         WPI_REG_SET(hw_winnet->mgmt.madr,(phy_num<<8)| 0x001f);
         WPI_REG_SET(hw_winnet->mgmt.mwtd, 0x0001);
         /* Setting Auto Detect MSB is 0 */
         WPU_WAIT_ON_MIND_REG(mind, hw_winnet->mgmt.mind);
         /* set sigdet polarity to 1 */
         WPI_REG_SET(hw_winnet->mgmt.madr,(phy_num<<8)| 0x0013);
         WPI_REG_SET(hw_winnet->mgmt.mwtd, 0x0001);

         WPU_WAIT_ON_MIND_REG(mind, hw_winnet->mgmt.mind);
         /* clear extended regs */
         WPI_REG_SET(hw_winnet->mgmt.madr,(phy_num<<8)| 0x001f);
         WPI_REG_SET(hw_winnet->mgmt.mwtd, 0x0000);
         WPU_WAIT_ON_MIND_REG(mind, hw_winnet->mgmt.mind);
         break;
      default:
         return WPI_ERROR(WP_ERR_ENET_BAD_INTERFACE_MODE);
   }
   return WP_OK;
}

/************************************************************************/
/*                      GEI PHY INITIALIZATION                        */
/************************************************************************/
/* Note this function is for backward compatibility to WP1 applications.
*/

WP_status WPU_GeiPhyInit(WP_U32 port, WP_U32 mode)
{
   WP_U32 major_mode = mode & 0x000F0000;

   switch (major_mode)
   {
      case WPU_MODE_GMII_OV_FIBER:
         return WPU_EnetPhyInit(port, WPU_MODE_RGMII | WPU_OV_FIBER | WPU_ANEG_ON);
      case WPU_MODE_GMII_OV_COPPER:
         return WPU_EnetPhyInit(port, WPU_MODE_RGMII | WPU_OV_COPPER | WPU_ANEG_ON);
      case WPU_MODE_GMII_OV_FIBER_ANEG_OFF:
         return WPU_EnetPhyInit(port, WPU_MODE_RGMII | WPU_OV_FIBER | WPU_ANEG_OFF);
      case WPU_MODE_GMII_OV_COPPER_ANEG_OFF:
         return WPU_EnetPhyInit(port, WPU_MODE_RGMII | WPU_OV_COPPER | WPU_ANEG_OFF);
      case WPU_MODE_MII_ANEG_ON:
         return WPU_EnetPhyInit(port, WPU_MODE_MII | WPU_ANEG_ON);
      case WPU_MODE_TBI:
         return WPU_EnetPhyInit(port, WPU_MODE_TBI);
      case WPU_MODE_GMII_MODE_AUTO_DETECT:
         return WPU_EnetPhyInit(port, WPU_MODE_RGMII | WPU_OV_AUTODETECT);
      default:
         return 0xffffffffU;
   }
}
